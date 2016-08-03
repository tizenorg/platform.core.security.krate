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

#ifndef __PACKAGE_PROXY_H__
#define __PACKAGE_PROXY_H__

#include "context.h"

namespace Krate {

/**
 * This class provides APIs to control packages in the zones.
 */

class PackageProxy {
public:
	PackageProxy(KrateControlContext& ctxt);
	~PackageProxy();

	//package information
	struct PackageInfo {
		std::string krate;
		std::string id;
		std::string locale;
		std::string type;
		std::string icon;
		std::string label;
		std::string description;
		struct {
			std::string name;
			std::string email;
			std::string href;
			REFLECTABLE(name, email, href);
		} author;
		std::string version;
		std::string apiVersion;
		std::string mainAppId;
		bool isSystem;
		bool isRemovable;
		bool isPreload;

		REFLECTABLE
		(
			krate, id, locale, type, icon, label, description, author,
			version, apiVersion, mainAppId, isSystem, isRemovable, isPreload
		);
	};

	PackageInfo getPackageInfo(const std::string& name, const std::string& pkgid);
	std::vector<std::string> getPackageList(const std::string& name);

	//package manager request
	int install(const std::string& name, const std::string& pkgpath);
	int uninstall(const std::string& name, const std::string& pkgid);

private:
	KrateControlContext& context;
};

} // namespace Krate
#endif // __PACKAGE_PROXY_H__
