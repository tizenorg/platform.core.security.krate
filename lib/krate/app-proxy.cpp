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

#include "krate.h"
#include "debug.h"
#include "app-proxy.h"
#include "app-info-internal.h"

#include "client.h"
#include "rmi/app-proxy.h"

using namespace DevicePolicyManager;

struct zone_app_proxy_s {
	ZoneAppProxy proxy;
	std::string zoneName;
};

static inline zone_app_proxy_s* getInstance(zone_app_proxy_h handle)
{
	return reinterpret_cast<zone_app_proxy_s *>(handle);
}

static app_info_h make_app_info_handle(const ZoneAppProxy::AppInfo& info)
{
	if (info.id.empty()) {
		return NULL;
	}

	application_x* app = (application_x*)::malloc(sizeof(application_x));
	pkgmgr_appinfo_x* pkgappinfo = (pkgmgr_appinfo_x*)::malloc(sizeof(pkgmgr_appinfo_x));
	app_info_s* appinfo = (app_info_s*)::malloc(sizeof(struct app_info_s));

	::memset(appinfo, 0, sizeof(app_info_s));
	appinfo->app_id = ::strdup(info.id.c_str());
	appinfo->pkg_app_info = pkgappinfo;

	::memset(pkgappinfo, 0, sizeof(pkgmgr_appinfo_x));
	pkgappinfo->package = ::strdup(info.package.c_str());
	pkgappinfo->locale = ::strdup(info.locale.c_str());
	pkgappinfo->app_component = info.componentType;
	pkgappinfo->app_info = app;

	::memset(app, 0, sizeof(application_x));
	app->appid = ::strdup(info.id.c_str());
	app->nodisplay = ::strdup(info.isNoDisplayed? "true":"false");
	app->taskmanage = ::strdup(info.isTaskManaged? "true":"false");
	app->type = ::strdup(info.type.c_str());

	icon_x* icon = (icon_x*)::calloc(1, sizeof(icon_x));
	icon->text = ::strdup(info.icon.c_str());
	icon->lang = ::strdup(info.locale.c_str());
	app->icon = ::g_list_append(NULL, icon);

	label_x* label = (label_x*)::calloc(1, sizeof(label_x));
	label->text = ::strdup(info.label.c_str());
	label->lang = ::strdup(info.locale.c_str());
	app->label = ::g_list_append(NULL, label);

	return reinterpret_cast<app_info_h>(appinfo);
}

int zone_app_proxy_create(zone_manager_h manager, const char* name, zone_app_proxy_h *handle)
{
	RET_ON_FAILURE(manager, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);

	zone_app_proxy_s* instance = new zone_app_proxy_s {
		GetDevicePolicyContext(manager).createPolicyInterface<ZoneAppProxy>(),
		name
	};

	*handle = reinterpret_cast<zone_app_proxy_h>(instance);
	return ZONE_ERROR_NONE;
}

int zone_app_proxy_destroy(zone_app_proxy_h handle)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);

	delete reinterpret_cast<ZoneAppProxy*>(handle);

	return ZONE_ERROR_NONE;
}

int zone_app_proxy_get_app_info(zone_app_proxy_h handle, const char* app_id, app_info_h* app_info)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(app_id, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(app_info, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;

	const auto info = proxy.getAppInfo(name, app_id);
	app_info_h ret = make_app_info_handle(info);
	if (ret == NULL) {
		return ZONE_ERROR_INVALID_PARAMETER;
	}

	*app_info = ret;

	return ZONE_ERROR_NONE;
}

int zone_app_proxy_foreach_app_info(zone_app_proxy_h handle, app_manager_app_info_cb callback, void *user_data)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(callback, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;

	int iter = proxy.createIterator(name);
	do {
		app_info_h info = make_app_info_handle(proxy.getIteratorValue(iter));
		int ret = callback(info, user_data);
		app_info_destroy(info);
		if (!ret) {
			break;
		}
	} while (proxy.nextIterator(iter));
	proxy.destroyIterator(iter);

	return ZONE_ERROR_NONE;
}

static bool appExtraCallback(app_control_h app_control, const char *key, void *user_data)
{
	ZoneAppProxy::Bundle *bundle = reinterpret_cast<ZoneAppProxy::Bundle*>(user_data);
	ZoneAppProxy::Bundle::Extra extra;
	bool isArray;

	extra.key = key;

	app_control_is_extra_data_array(app_control, key, &isArray);
	if (isArray) {
		char **values;
		int length = 0;
		app_control_get_extra_data_array(app_control, key, &values, &length);
		for (int i = 0; i < length; i++) {
			extra.value.push_back(values[i]);
			free(values[i]);
		}
		free(values);
	} else {
		char *value;
		app_control_get_extra_data(app_control, key, &value);
		extra.value.push_back(value);
		free(value);
	}
	bundle->extraData.push_back(extra);

	return true;
}

int zone_app_proxy_send_launch_request(zone_app_proxy_h handle, app_control_h app_control)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(app_control, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;
	char *str;

	ZoneAppProxy::Bundle bundle;
	app_control_get_operation(app_control, &str);
	if (str != NULL) {
		bundle.operation = str;
		free(str);
		str = NULL;
	}
	app_control_get_uri(app_control, &str);
	if (str != NULL) {
		bundle.uri = str;
		free(str);
		str = NULL;
	}
	app_control_get_mime(app_control, &str);
	if (str != NULL) {
		bundle.mime = str;
		free(str);
		str = NULL;
	}
	app_control_get_category(app_control, &str);
	if (str != NULL) {
		bundle.category = str;
		free(str);
		str = NULL;
	}
	app_control_get_app_id(app_control, &str);
	if (str != NULL) {
		bundle.appId = str;
		free(str);
		str = NULL;
	}
	app_control_foreach_extra_data(app_control, appExtraCallback, &bundle);

	return proxy.launch(name, bundle);
}

int zone_app_proxy_terminate(zone_app_proxy_h handle, const char* app_id)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(app_id, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;

	return proxy.terminate(name, app_id);
}

int zone_app_proxy_resume(zone_app_proxy_h handle, const char* app_id)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(app_id, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;

	return proxy.resume(name, app_id);
}

int zone_app_proxy_is_running(zone_app_proxy_h handle, const char* app_id, int *result)
{
	RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(app_id, ZONE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(result, ZONE_ERROR_INVALID_PARAMETER);

	auto instance = getInstance(handle);
	auto& proxy = instance->proxy;
	const std::string& name = instance->zoneName;

	*result = proxy.isRunning(name, app_id);
	return ZONE_ERROR_NONE;
}
