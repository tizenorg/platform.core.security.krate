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
#include <functional>

#include <aul.h>
#include <cynara-client.h>
#include <cynara-session.h>

#include "server.h"

using namespace std::placeholders;

namespace {

const std::string KRATE_MANAGER_ADDRESS = "/tmp/.krate-manager.sock";

} // namespace

Server::Server()
{
	service.reset(new rmi::Service(KRATE_MANAGER_ADDRESS));

	service->setPrivilegeChecker(std::bind(&Server::checkPeerPrivilege, this, _1, _2));

    service->registerParametricMethod(&zoneManager, "", (int)(ZoneManager::createZone)(std::string, std::string));
    service->registerParametricMethod(&zoneManager, "", (int)(ZoneManager::removeZone)(std::string));
    service->registerParametricMethod(&zoneManager, "", (int)(ZoneManager::lockZone)(std::string));
    service->registerParametricMethod(&zoneManager, "", (int)(ZoneManager::unlockZone)(std::string));
    service->registerParametricMethod(&zoneManager, "", (int)(ZoneManager::getZoneState)(std::string));
    service->registerParametricMethod(&zoneManager, "", (std::vector<std::string>)(ZoneManager::getZoneList)(int));
    service->registerParametricMethod(&zoneManager, "", (int)(ZoneManager::resetZonePassword)(std::string, std::string));

    service->registerParametricMethod(&appProxy, "", (ZoneAppProxy::AppInfo)(ZoneAppProxy::getAppInfo)(std::string, std::string));

    service->registerParametricMethod(&appProxy, "", (int)(ZoneAppProxy::createIterator)(std::string));
    service->registerParametricMethod(&appProxy, "", (ZoneAppProxy::AppInfo)(ZoneAppProxy::getIteratorValue)(int));
    service->registerParametricMethod(&appProxy, "", (bool)(ZoneAppProxy::nextIterator)(int));
    service->registerParametricMethod(&appProxy, "", (int)(ZoneAppProxy::destroyIterator)(int));

    service->registerParametricMethod(&appProxy, "", (int)(ZoneAppProxy::launch)(std::string, ZoneAppProxy::Bundle));
    service->registerParametricMethod(&appProxy, "", (int)(ZoneAppProxy::resume)(std::string, std::string));
    service->registerParametricMethod(&appProxy, "", (int)(ZoneAppProxy::terminate)(std::string, std::string));
    service->registerParametricMethod(&appProxy, "", (int)(ZoneAppProxy::isRunning)(std::string, std::string));

    service->registerParametricMethod(&packageProxy, "", (ZonePackageProxy::PackageInfo)(ZonePackageProxy::getPackageInfo)(std::string, std::string));
    service->registerParametricMethod(&packageProxy, "", (std::vector<std::string>)(ZonePackageProxy::getPackageList)(std::string));

    service->registerParametricMethod(&packageProxy, "", (int)(ZonePackageProxy::install)(std::string, std::string));
    service->registerParametricMethod(&packageProxy, "", (int)(ZonePackageProxy::uninstall)(std::string, std::string));

    service->createNotification("ZoneManager::created");
    service->createNotification("ZoneManager::removed");

	service->registerParametricMethod(this, "", (FileDescriptor)(Server::registerNotificationSubscriber)(std::string));
	service->registerParametricMethod(this, "", (int)(Server::unregisterNotificationSubscriber)(std::string, int));
}

Server::~Server()
{
}

void Server::run()
{
	// Prepare execution environment
	service->start(true);
}

void Server::terminate()
{
	service->stop();
}

FileDescriptor Server::registerNotificationSubscriber(const std::string& name)
{
	return FileDescriptor(service->subscribeNotification(name), true);
}

int Server::unregisterNotificationSubscriber(const std::string& name, int id)
{
	return service->unsubscribeNotification(name, id);
}

bool Server::checkPeerPrivilege(const rmi::Credentials& cred, const std::string& privilege)
{
	cynara *p_cynara;

	if (privilege.empty()) {
		return true;
	}

	if (::cynara_initialize(&p_cynara, NULL) != CYNARA_API_SUCCESS) {
		ERROR("Failure in cynara API");
		return false;
	}

	if (::cynara_check(p_cynara, cred.security.c_str(), "",
					   std::to_string(cred.uid).c_str(),
					   privilege.c_str()) != CYNARA_API_ACCESS_ALLOWED) {
		::cynara_finish(p_cynara);
		ERROR("Access denied: " + cred.security + " : " + privilege);
		return false;
	}

	::cynara_finish(p_cynara);

	return true;
}
