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

#ifndef __APP_PROXY_H__
#define __APP_PROXY_H__

#include "context.h"

namespace Krate {

/**
 * This class provides APIs to create/remove and manage names.
 */

class AppProxy {
public:
	AppProxy(KrateControlContext& ctxt);
	~AppProxy();

	//application information
	struct AppInfo {
		std::string krate;
		std::string id;
		std::string locale;
		std::string package;
		std::string type;
		std::string icon;
		std::string label;
		int componentType;
		bool isNoDisplayed;
		bool isTaskManaged;

		REFLECTABLE
		(
			krate, id, locale, package, type, icon, label,
			componentType, isNoDisplayed, isTaskManaged
		);
	};

	AppInfo getAppInfo(const std::string& name, const std::string& appid);

	int createIterator(const std::string& name);
	AppInfo getIteratorValue(int iterator);
	bool nextIterator(int iterator);
	int destroyIterator(int iterator);

	//application bundle
	struct Bundle {
		std::string operation;
		std::string uri;
		std::string mime;
		std::string category;
		std::string appId;
		struct Extra {
			std::string key;
			std::vector<std::string> value;
			REFLECTABLE(key, value);
		};
		std::vector<Extra> extraData;

		REFLECTABLE
		(
			operation, uri, mime, category, appId, extraData
		);
	};

	int launch(const std::string& name, const Bundle& bundle);
	int resume(const std::string& name, const std::string& appid);
	int terminate(const std::string& name, const std::string& appid);
	bool isRunning(const std::string& name, const std::string& appid);

private:
	KrateControlContext& context;
};

} // namespace Krate
#endif // __APP_PROXY_H__
