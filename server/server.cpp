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

#include <cynara-client.h>
#include <cynara-session.h>

#include "server.h"

#include "rmi/manager.h"
#include "rmi/app-proxy.h"
#include "rmi/package-proxy.h"

using namespace std::placeholders;

namespace {

const std::string KRATE_MANAGER_ADDRESS = "/tmp/.krate.sock";

std::unique_ptr<Krate::Manager> manager;
std::unique_ptr<Krate::AppProxy> appProxy;
std::unique_ptr<Krate::PackageProxy> packageProxy;

} // namespace

Server::Server()
{
	service.reset(new rmi::Service(KRATE_MANAGER_ADDRESS));

	service->setPrivilegeChecker(std::bind(&Server::checkPeerPrivilege, this, _1, _2));

	service->registerParametricMethod(this, "", (runtime::FileDescriptor)(Server::registerNotificationSubscriber)(std::string));
	service->registerParametricMethod(this, "", (int)(Server::unregisterNotificationSubscriber)(std::string, int));

	manager.reset(new Krate::Manager(*this));
	appProxy.reset(new Krate::AppProxy(*this));
	packageProxy.reset(new Krate::PackageProxy(*this));
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

runtime::FileDescriptor Server::registerNotificationSubscriber(const std::string& name)
{
	return runtime::FileDescriptor(service->subscribeNotification(name), true);
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
