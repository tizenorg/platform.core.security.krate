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
#include "rmi/package-proxy.h"

namespace Krate {

PackageProxy::PackageProxy(KrateControlContext& ctx) :
	context(ctx)
{
}

PackageProxy::~PackageProxy()
{
}

PackageProxy::PackageInfo PackageProxy::getPackageInfo(const std::string& name, const std::string& pkgid)
{
	try {
		return context->methodCall<PackageProxy::PackageInfo>("PackageProxy::getPackageInfo", name, pkgid);
	} catch (runtime::Exception& e) {}
	return PackageProxy::PackageInfo();
}

std::vector<std::string> PackageProxy::getPackageList(const std::string& zone)
{
	try {
		return context->methodCall<std::vector<std::string>>("PackageProxy::getPackageList", zone);
	} catch (runtime::Exception& e) {}
	return std::vector<std::string>();;
}

int PackageProxy::install(const std::string& name, const std::string& pkgid)
{
	try {
		return context->methodCall<int>("PackageProxy::install", name, pkgid);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int PackageProxy::uninstall(const std::string& name, const std::string& pkgid)
{
	try {
		return context->methodCall<int>("PackageProxy::uninstall", name, pkgid);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

} // namespace Krate
