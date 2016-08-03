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

#include <sys/stat.h>
#include <sys/types.h>
#include <system_settings.h>

#include <unordered_map>

#include "app-proxy.h"

#include "error.h"
#include "exception.h"
#include "packman.h"
#include "launchpad.h"
#include "auth/user.h"
#include "audit/logger.h"

namespace {

struct IteratorData {
	std::string zone;
	std::vector<ApplicationInfo> list;
	unsigned int current;
};

std::unordered_map<int, IteratorData> iteratorMap;
int newIteratorId = 0;

} // namespace

ZoneAppProxy::ZoneAppProxy()
{
}

ZoneAppProxy::~ZoneAppProxy()
{
}

ZoneAppProxy::AppInfo ZoneAppProxy::getAppInfo(const std::string& name, const std::string& appid)
{
	AppInfo appInfo;

	try {
		runtime::User user(name);
		ApplicationInfo appinfo(appid, user.getUid());
		char* locale;

		system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
		if (locale == NULL) {
			appInfo.locale = "No locale";
		} else {
			appInfo.locale = locale;
			free(locale);
		}

		appInfo.zone = name;
		appInfo.id = appid;
		appInfo.package = appinfo.getPackage();
		appInfo.type = appinfo.getType();
		appInfo.icon = appinfo.getIcon();
		appInfo.label = appinfo.getLabel();
		appInfo.componentType = appinfo.getComponentType();
		appInfo.isNoDisplayed = appinfo.isNoDisplayed();
		appInfo.isTaskManaged = appinfo.isTaskManaged();
	} catch (runtime::Exception& e) {
		ERROR("Failed to retrieve application info installed in the zone: " + appid);
	}

	return appInfo;
}

int ZoneAppProxy::createIterator(const std::string& name)
{
	int iteratorId = -1;
	try {
		PackageManager& packman = PackageManager::instance();
		runtime::User user(name);
		IteratorData data;

		iteratorId = newIteratorId;

		data.zone = name;
		data.list = packman.getAppList(user.getUid());
		data.current = 0;

		iteratorMap.insert(std::make_pair(iteratorId, data));

		if (++newIteratorId < 0) {
			newIteratorId = 0;
		}
	} catch (runtime::Exception& e) {
		ERROR("Failed to retrieve package info installed in the zone");
	}
	return iteratorId;
}

ZoneAppProxy::AppInfo ZoneAppProxy::getIteratorValue(int iterator)
{
	AppInfo appInfo;

	auto it = iteratorMap.find(iterator);
	if (it == iteratorMap.end()) {
		return appInfo;
	}
	if (it->second.current >= it->second.list.size()) {
		return appInfo;
	}

	const ApplicationInfo& appinfo = it->second.list.at(it->second.current);
	char* locale;

	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	if (locale == NULL) {
		appInfo.locale = "No locale";
	} else {
		appInfo.locale = locale;
		free(locale);
	}

	appInfo.zone = it->second.zone;
	appInfo.id = appinfo.getId();
	appInfo.package = appinfo.getPackage();
	appInfo.type = appinfo.getType();
	appInfo.icon = appinfo.getIcon();
	appInfo.label = appinfo.getLabel();
	appInfo.componentType = appinfo.getComponentType();
	appInfo.isNoDisplayed = appinfo.isNoDisplayed();
	appInfo.isTaskManaged = appinfo.isTaskManaged();

	return appInfo;
}

bool ZoneAppProxy::nextIterator(int iterator)
{
	auto it = iteratorMap.find(iterator);
	if (it != iteratorMap.end()) {
		if (++it->second.current < it->second.list.size()) {
			return true;
		}
	}
	return false;
}

int ZoneAppProxy::destroyIterator(int iterator)
{
	auto it = iteratorMap.find(iterator);
	if (it != iteratorMap.end()) {
		iteratorMap.erase(it);
		return 0;
	}
	return -1;
}

int ZoneAppProxy::launch(const std::string& name, const ZoneAppProxy::Bundle& bundle)
{
	try {
		runtime::User user(name);
		::Bundle b;

		if (!bundle.operation.empty()) {
			b.add("__APP_SVC_OP_TYPE__", bundle.operation);
		}
		if (!bundle.uri.empty()) {
			b.add("__APP_SVC_URI__", bundle.uri);
		}
		if (!bundle.mime.empty()) {
			b.add("__APP_SVC_MIME__", bundle.mime);
		}
		if (!bundle.category.empty()) {
			b.add("__APP_SVC_CATEGORY__", bundle.category);
		}

		for (Bundle::Extra extra : bundle.extraData) {
			if (extra.value.size() > 1) {
				b.add(extra.key, extra.value);
			} else if (extra.value.size() == 1) {
				b.add(extra.key, extra.value.at(0));
			}
		}

		Launchpad launchpad(user.getUid());
		launchpad.launch(bundle.appId, b);
	} catch (runtime::Exception& e) {
		ERROR("Failed to launch app in the zone");
		return -1;
	}
	return 0;
}

int ZoneAppProxy::resume(const std::string& name, const std::string& appid)
{
	try {
		runtime::User user(name);
		Launchpad launchpad(user.getUid());
		launchpad.resume(appid);
	} catch (runtime::Exception& e) {
		ERROR("Failed to terminate app in the zone");
		return -1;
	}
	return 0;
}

int ZoneAppProxy::terminate(const std::string& name, const std::string& appid)
{
	try {
		runtime::User user(name);
		Launchpad launchpad(user.getUid());
		launchpad.terminate(appid);
	} catch (runtime::Exception& e) {
		ERROR("Failed to terminate app in the zone");
		return -1;
	}
	return 0;
}

bool ZoneAppProxy::isRunning(const std::string& name, const std::string& appid)
{
	try {
		runtime::User user(name);
		Launchpad launchpad(user.getUid());
		return launchpad.isRunning(appid);
	} catch (runtime::Exception& e) {
		ERROR("Failed to get app running state in the zone");
	}
	return false;
}
