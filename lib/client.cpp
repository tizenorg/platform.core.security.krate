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

#include "client.h"

namespace {

const std::string SUBSCRIBER_REGISTER = "Server::registerNotificationSubscriber";
const std::string SUBSCRIBER_UNREGISTER = "Server::unregisterNotificationSubscriber";

const std::string KRATE_MANAGER_ADDRESS = "/tmp/.krate.sock";

} // namespace


KrateContext::KrateContext() noexcept
{
}

KrateContext::~KrateContext() noexcept
{
	disconnect();
}

int KrateContext::connect(const std::string& address) noexcept
{
	try {
		client.reset(new rmi::Client(address));
		client->connect();
	} catch (runtime::Exception& e) {
		return -1;
	}

	return 0;
}

int KrateContext::connect() noexcept
{
	return connect(KRATE_MANAGER_ADDRESS);
}

void KrateContext::disconnect() noexcept
{
	client.reset();
}

int KrateContext::subscribeSignal(const std::string& name,
									const SignalListener& listener,
									void* data)
{
	auto listenerDispatcher = [listener, data](std::string &name, std::string &from, std::string &object) {
		listener(from.c_str(), object.c_str(), data);
	};

	try {
		return client->subscribe<std::string, std::string, std::string>(SUBSCRIBER_REGISTER,
																		name, listenerDispatcher);
	} catch (runtime::Exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
}

int KrateContext::unsubscribeSignal(int subscriberId)
{
	return client->unsubscribe(SUBSCRIBER_UNREGISTER, subscriberId);
}
