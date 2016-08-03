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

#ifndef __KRATE_SERVER_H__
#define __KRATE_SERVER_H__

#include <string>
#include <memory>

#include "app-proxy.h"
#include "package-proxy.h"
#include "zone.h"

#include "file-descriptor.h"
#include "filesystem.h"
#include "rmi/service.h"

class Server {
public:
	Server();
	~Server();

	void run();
	void terminate();

	template <typename... Args>
	void notify(const std::string& name, Args&&... args)
	{
		service->notify<Args...>(name, std::forward<Args>(args)...);
	}

	uid_t getPeerUid() const
	{
		return service->getPeerUid();
	}

	gid_t getPeerGid() const
	{
		return service->getPeerGid();
	}

	pid_t getPeerPid() const
	{
		return service->getPeerPid();
	}

	void createNotification(const std::string& name)
	{
		service->createNotification(name);
	}

	FileDescriptor registerNotificationSubscriber(const std::string& name);
	int unregisterNotificationSubscriber(const std::string& name, int id);

	bool checkPeerPrivilege(const rmi::Credentials& cred, const std::string& privilege);

private:
	ZoneAppProxy appProxy;
	ZonePackageProxy packageProxy;
	ZoneManager zoneManager;

	std::string securityLabel;
	std::unique_ptr<rmi::Service> service;
};

#endif //__KRATE_SERVER_H__
