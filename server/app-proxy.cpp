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

#include <unordered_map>

#include <system_settings.h>
#include <klay/auth/user.h>
#include <klay/audit/logger.h>

#include "packman.h"
#include "launchpad.h"

#include "rmi/app-proxy.h"

namespace Krate {

namespace {

struct IteratorData {
	std::string krate;
	std::vector<ApplicationInfo> list;
	unsigned int current;
};

std::unordered_map<int, IteratorData> iteratorMap;
int newIteratorId = 0;

} // namespace

AppProxy::AppProxy(KrateControlContext& ctx) :
	context(ctx)
{
	context.registerParametricMethod(this, "", (AppProxy::AppInfo)(AppProxy::getAppInfo)(std::string, std::string));

	context.registerParametricMethod(this, "", (int)(AppProxy::createIterator)(std::string));
	context.registerParametricMethod(this, "", (AppProxy::AppInfo)(AppProxy::getIteratorValue)(int));
	context.registerParametricMethod(this, "", (bool)(AppProxy::nextIterator)(int));
	context.registerParametricMethod(this, "", (int)(AppProxy::destroyIterator)(int));

	context.registerParametricMethod(this, "", (int)(AppProxy::launch)(std::string, AppProxy::Bundle));
	context.registerParametricMethod(this, "", (int)(AppProxy::resume)(std::string, std::string));
	context.registerParametricMethod(this, "", (int)(AppProxy::terminate)(std::string, std::string));
	context.registerParametricMethod(this, "", (int)(AppProxy::isRunning)(std::string, std::string));
}

AppProxy::~AppProxy()
{
}

AppProxy::AppInfo AppProxy::getAppInfo(const std::string& name, const std::string& appid)
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

		appInfo.krate = name;
		appInfo.id = appid;
		appInfo.package = appinfo.getPackage();
		appInfo.type = appinfo.getType();
		appInfo.icon = appinfo.getIcon();
		appInfo.label = appinfo.getLabel();
		appInfo.componentType = appinfo.getComponentType();
		appInfo.isNoDisplayed = appinfo.isNoDisplayed();
		appInfo.isTaskManaged = appinfo.isTaskManaged();
	} catch (runtime::Exception& e) {
		ERROR("Failed to retrieve application info installed in the krate: " + appid);
	}

	return appInfo;
}

int AppProxy::createIterator(const std::string& name)
{
	int iteratorId = -1;
	try {
		PackageManager& packman = PackageManager::instance();
		runtime::User user(name);
		IteratorData data;

		iteratorId = newIteratorId;

		data.krate = name;
		data.list = packman.getAppList(user.getUid());
		data.current = 0;

		iteratorMap.insert(std::make_pair(iteratorId, data));

		if (++newIteratorId < 0) {
			newIteratorId = 0;
		}
	} catch (runtime::Exception& e) {
		ERROR("Failed to retrieve package info installed in the krate");
	}
	return iteratorId;
}

AppProxy::AppInfo AppProxy::getIteratorValue(int iterator)
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

	appInfo.krate = it->second.krate;
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

bool AppProxy::nextIterator(int iterator)
{
	auto it = iteratorMap.find(iterator);
	if (it != iteratorMap.end()) {
		if (++it->second.current < it->second.list.size()) {
			return true;
		}
	}
	return false;
}

int AppProxy::destroyIterator(int iterator)
{
	auto it = iteratorMap.find(iterator);
	if (it != iteratorMap.end()) {
		iteratorMap.erase(it);
		return 0;
	}
	return -1;
}

int AppProxy::launch(const std::string& name, const AppProxy::Bundle& bundle)
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
		ERROR("Failed to launch app in the krate");
		return -1;
	}
	return 0;
}

int AppProxy::resume(const std::string& name, const std::string& appid)
{
	try {
		runtime::User user(name);
		Launchpad launchpad(user.getUid());
		launchpad.resume(appid);
	} catch (runtime::Exception& e) {
		ERROR("Failed to terminate app in the krate");
		return -1;
	}
	return 0;
}

int AppProxy::terminate(const std::string& name, const std::string& appid)
{
	try {
		runtime::User user(name);
		Launchpad launchpad(user.getUid());
		launchpad.terminate(appid);
	} catch (runtime::Exception& e) {
		ERROR("Failed to terminate app in the krate");
		return -1;
	}
	return 0;
}

bool AppProxy::isRunning(const std::string& name, const std::string& appid)
{
	try {
		runtime::User user(name);
		Launchpad launchpad(user.getUid());
		return launchpad.isRunning(appid);
	} catch (runtime::Exception& e) {
		ERROR("Failed to get app running state in the krate");
	}
	return false;
}

} // namespace Krate
