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

#include "package-proxy.h"
#include "error.h"
#include "exception.h"
#include "packman.h"
#include "auth/user.h"
#include "audit/logger.h"

ZonePackageProxy::ZonePackageProxy()
{
}

ZonePackageProxy::~ZonePackageProxy()
{
}

ZonePackageProxy::PackageInfo ZonePackageProxy::getPackageInfo(const std::string& name, const std::string& pkgid)
{
	ZonePackageProxy::PackageInfo package;
	char* locale = NULL;

	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	if (locale == NULL) {
		locale = strdup("No locale");
	}

	package.zone = name;
	package.id = pkgid;
	package.locale = locale;

	free(locale);

	try {
		runtime::User user(name);
		::PackageInfo pkginfo(pkgid, user.getUid());

		package.type = pkginfo.getType();
		package.icon = pkginfo.getIcon();
		package.label = pkginfo.getLabel();
		package.description = pkginfo.getDescription();

		package.author.name = pkginfo.getAuthorName();
		package.author.email = pkginfo.getAuthorEmail();
		package.author.href = pkginfo.getAuthorHref();

		package.version = pkginfo.getVersion();
		package.apiVersion = pkginfo.getApiVersion();
		package.mainAppId = pkginfo.getMainAppId();

		package.isSystem = pkginfo.isSystem();
		package.isRemovable = pkginfo.isRemovable();
		package.isPreload = pkginfo.isPreload();
	} catch (runtime::Exception& e) {
		ERROR("Failed to retrieve package info installed in the zone");
	}

	return package;
}

std::vector<std::string> ZonePackageProxy::getPackageList(const std::string& name)
{
	try {
		runtime::User user(name);
		PackageManager& packman = PackageManager::instance();
		return packman.getPackageList(user.getUid());
	} catch (runtime::Exception& e) {
		ERROR("Failed to retrieve package info installed in the zone");
	}
	return std::vector<std::string>();
}


int ZonePackageProxy::install(const std::string& name, const std::string& pkgpath)
{
	try {
		runtime::User user(name);
		PackageManager& packman = PackageManager::instance();
		packman.installPackage(pkgpath, user.getUid());
	} catch (runtime::Exception& e) {
		ERROR("Failed to install package in the zone");
		return -1;
	}

	return 0;
}

int ZonePackageProxy::uninstall(const std::string& name, const std::string& pkgid)
{
	try {
		runtime::User user(name);
		PackageManager& packman = PackageManager::instance();
		packman.uninstallPackage(pkgid, user.getUid());
	} catch (runtime::Exception& e) {
		ERROR("Failed to uninstall package of pkgid in the zone");
		return -1;
	}
	return 0;
}
