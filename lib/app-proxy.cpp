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
#include "rmi/app-proxy.h"

namespace Krate {

AppProxy::AppProxy(KrateControlContext& ctx)
	: context(ctx)
{
}

AppProxy::~AppProxy()
{
}

AppProxy::AppInfo AppProxy::getAppInfo(const std::string& name, const std::string& appid)
{
	try {
		return context->methodCall<AppProxy::AppInfo>("AppProxy::getAppInfo", name, appid);
	} catch (runtime::Exception& e) {}
	return AppProxy::AppInfo();
}

int AppProxy::createIterator(const std::string& zone)
{
	try {
		return context->methodCall<int>("AppProxy::createIterator", zone);
	} catch (runtime::Exception& e) {}
	return -1;
}

AppProxy::AppInfo AppProxy::getIteratorValue(int iterator)
{
	try {
		return context->methodCall<AppProxy::AppInfo>("AppProxy::getIteratorValue", iterator);
	} catch (runtime::Exception& e) {}
	return AppProxy::AppInfo();
}

bool AppProxy::nextIterator(int iterator)
{
	try {
		return context->methodCall<bool>("AppProxy::nextIterator", iterator);
	} catch (runtime::Exception& e) {}
	return false;
}

int AppProxy::destroyIterator(int iterator)
{
	try {
		return context->methodCall<int>("AppProxy::destroyIterator", iterator);
	} catch (runtime::Exception& e) {}
	return -1;
}

int AppProxy::launch(const std::string& name, const AppProxy::Bundle& bundle)
{
	try {
		return context->methodCall<int>("AppProxy::launch", name, bundle);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int AppProxy::resume(const std::string& name, const std::string& appid)
{
	try {
		return context->methodCall<int>("AppProxy::resume", name, appid);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int AppProxy::terminate(const std::string& name, const std::string& appid)
{
	try {
		return context->methodCall<int>("AppProxy::terminate", name, appid);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool AppProxy::isRunning(const std::string& name, const std::string& appid)
{
	try {
		return context->methodCall<bool>("AppProxy::isRunning", name, appid);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

} // namespace Krate
