/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#include <cstdlib>
#include <cstring>
#include <algorithm>

#include <pkgmgr-info.h>
#include <package-manager.h>
#include <tzplatform_config.h>
#include <klay/auth/user.h>

#include "krate.h"
#include "debug.h"
#include "package-proxy.h"
#include "package-info-internal.h"

#include "client.h"
#include "rmi/package-proxy.h"

using namespace DevicePolicyManager;

struct zone_package_proxy_s {
	ZonePackageProxy proxy;
	std::string zoneName;
	pkgmgr_client* pNativeHandle;
	package_manager_event_cb pCallback;
	void *pCallbackUserData;
};

static inline zone_package_proxy_s* getInstance(zone_package_proxy_h handle)
{
	return reinterpret_cast<zone_package_proxy_s *>(handle);
}

static int packageEventHandler(uid_t target_uid, int req_id,
							   const char *pkg_type, const char *pkg_name,
							   const char *key, const char *val,
							   const void *pmsg, void *data)
{
	static auto event_type = (package_manager_event_type_e)-1;
	auto event_state = PACKAGE_MANAGER_EVENT_STATE_FAILED;
	auto instance = getInstance(data);
	std::string keystr = key;
	int progress = 0;

	if (target_uid != tzplatform_getuid(TZ_SYS_GLOBALAPP_USER)) {
		try {
			runtime::User pkgOwner(target_uid);
			if (pkgOwner.getName() != instance->zoneName) {
				return PACKAGE_MANAGER_ERROR_NONE;
			}
		} catch (runtime::Exception &e) {
			return PACKAGE_MANAGER_ERROR_NONE;
		}
	}

	std::transform(keystr.begin(), keystr.end(), keystr.begin(), ::tolower);

	if (keystr == "start") {
		if (val == NULL) {
			return PACKAGE_MANAGER_ERROR_INVALID_PARAMETER;
		}

		std::string valstr = val;
		std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
		if (valstr == "install") {
			event_type = PACKAGE_MANAGER_EVENT_TYPE_INSTALL;
		} else if (valstr == "uninstall") {
			event_type = PACKAGE_MANAGER_EVENT_TYPE_UNINSTALL;
		} else if (valstr == "update") {
			event_type = PACKAGE_MANAGER_EVENT_TYPE_UPDATE;
		} else {
			return PACKAGE_MANAGER_ERROR_INVALID_PARAMETER;
		}

		event_state = PACKAGE_MANAGER_EVENT_STATE_STARTED;
	} else if (keystr == "install_percent" ||
			   keystr == "progress_percent") {
		event_state = PACKAGE_MANAGER_EVENT_STATE_PROCESSING;
		progress = std::stoi(val);
	} else if (keystr == "error") {
		event_state = PACKAGE_MANAGER_EVENT_STATE_FAILED;
	} else if (keystr == "end" ||
			   keystr == "ok") {
		event_state = PACKAGE_MANAGER_EVENT_STATE_COMPLETED;
		progress = 100;
	}

	instance->pCallback(pkg_type, pkg_name,
						event_type, event_state, progress,
						PACKAGE_MANAGER_ERROR_NONE,
						instance->pCallbackUserData);


	return PACKAGE_MANAGER_ERROR_NONE;
}

static package_info_h make_package_info_handle(const ZonePackageProxy::PackageInfo& info)
{
	uid_t zone_uid;

	if (info.id.empty()) {
		return NULL;
	}

	try {
		runtime::User pkgOwner(info.zone);
		zone_uid = pkgOwner.getUid();
	} catch (runtime::Exception &e) {
		return NULL;
	}

	package_x* package = (package_x*)::calloc(1, sizeof(package_x));
	pkgmgr_pkginfo_x* pkginfo = (pkgmgr_pkginfo_x*)::calloc(1, sizeof(pkgmgr_pkginfo_x));
	package_info_s* packageinfo = (package_info_s*)::calloc(1, sizeof(package_info_s));

	packageinfo->package = ::strdup(info.id.c_str());
	packageinfo->pkgmgr_pkginfo = pkginfo;

	pkginfo->uid = zone_uid;
	pkginfo->locale = ::strdup(info.locale.c_str());
	pkginfo->pkg_info = package;

	package->for_all_users = ::strdup("false");
	package->package = ::strdup(info.id.c_str());
	package->version = ::strdup(info.version.c_str());
	package->removable = ::strdup(info.isRemovable? "true":"false");
	package->preload = ::strdup(info.isPreload? "true":"false");
	package->system = ::strdup(info.isSystem? "true":"false");
	package->type = ::strdup(info.type.c_str());
	package->mainapp_id = ::strdup(info.mainAppId.c_str());
	package->api_version = ::strdup(info.apiVersion.c_str());

	icon_x* icon = (icon_x*)::calloc(1, sizeof(icon_x));
	icon->text = ::strdup(info.icon.c_str());
	icon->lang = ::strdup(info.locale.c_str());
	package->icon = ::g_list_append(NULL, icon);

	label_x* label = (label_x*)::calloc(1, sizeof(label_x));
	label->text = ::strdup(info.label.c_str());
	label->lang = ::strdup(info.locale.c_str());
	package->label = ::g_list_append(NULL, label);

	description_x* desc = (description_x*)::calloc(1, sizeof(description_x));
	desc->text = ::strdup(info.description.c_str());
	desc->lang = ::strdup(info.locale.c_str());
	package->description = ::g_list_append(NULL, desc);

	author_x* author = (author_x*)::calloc(1, sizeof(author_x));
	author->text = ::strdup(info.author.name.c_str());
	author->email = ::strdup(info.author.email.c_str());
	author->href = ::strdup(info.author.href.c_str());
	author->lang = ::strdup(info.locale.c_str());
	package->author = ::g_list_append(NULL, author);

	return reinterpret_cast<package_info_h>(packageinfo);
}

int zone_package_proxy_create(zone_manager_h manager, const char* name, zone_package_proxy_h *handle)
{
	RET_ON_FAILURE(manager, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);

	zone_package_proxy_s* instance = new zone_package_proxy_s {
		GetDevicePolicyContext(manager).
								createPolicyInterface<ZonePackageProxy>(),
		name, ::pkgmgr_client_new(PC_LISTENING), NULL, NULL
	};

	*handle = reinterpret_cast<zone_package_proxy_h>(instance);
	return ZONE_ERROR_NONE;
}

int zone_package_proxy_destroy(zone_package_proxy_h handle)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);

	zone_package_proxy_s* instance = getInstance(handle);

	::pkgmgr_client_free(instance->pNativeHandle);

	delete instance;

	return ZONE_ERROR_NONE;
}

int zone_package_proxy_get_package_info(zone_package_proxy_h handle, const char* package_id, package_info_h* package_info)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(package_id, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(package_info, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;

	const auto& info = proxy.getPackageInfo(name, package_id);
	package_info_h ret = make_package_info_handle(info);

	if (ret == NULL) {
		return ZONE_ERROR_INVALID_PARAMETER;
	}

	*package_info = ret;

	return ZONE_ERROR_NONE;
}

int zone_package_proxy_foreach_package_info(zone_package_proxy_h handle, package_manager_package_info_cb callback, void *user_data)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(callback, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;

	for (const auto& pkgid : proxy.getPackageList(name)) {
		package_info_h info_h = make_package_info_handle(proxy.getPackageInfo(name, pkgid));
		int ret = callback(info_h, user_data);
		package_info_destroy(info_h);
		if (!ret) {
			break;
		}
	}

	return ZONE_ERROR_NONE;
}

int zone_package_proxy_set_event_status(zone_package_proxy_h handle, int status_type)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);

	int ret;
	ret = pkgmgrinfo_client_set_status_type(instance->pNativeHandle, status_type);

	if (ret != PACKAGE_MANAGER_ERROR_NONE)
		return ZONE_ERROR_INVALID_PARAMETER;

	return ZONE_ERROR_NONE;
}

int zone_package_proxy_set_event_cb(zone_package_proxy_h handle, package_manager_event_cb callback, void *user_data)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(callback, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);

	instance->pCallback = callback;
	instance->pCallbackUserData = user_data;

	int ret;
	ret = pkgmgr_client_listen_status(instance->pNativeHandle, packageEventHandler, handle);

	if (ret < 0)
		return ZONE_ERROR_INVALID_PARAMETER;

	return ZONE_ERROR_NONE;
}

int zone_package_proxy_unset_event_cb(zone_package_proxy_h handle)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);

	int ret;
	ret = pkgmgr_client_remove_listen_status(instance->pNativeHandle);

	if (ret < 0)
		return ZONE_ERROR_INVALID_PARAMETER;

	return ZONE_ERROR_NONE;
}

int zone_package_proxy_install(zone_package_proxy_h handle, const char* package_path)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(package_path, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;

	return proxy.install(name, package_path);
}

int zone_package_proxy_uninstall(zone_package_proxy_h handle, const char* package_id)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(package_id, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;

	return proxy.uninstall(name, package_id);
}
