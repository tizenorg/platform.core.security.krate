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

#ifndef __RMI_NOTIFICATION_H__
#define __RMI_NOTIFICATION_H__

#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <list>
#include <utility>
#include <memory>

#include "socket.h"
#include "message.h"
#include "audit/logger.h"

namespace rmi {

typedef std::pair<int, int> SubscriptionId;

class Notification {
public:
	Notification();
	Notification(const std::string& name);
	Notification(const Notification&) = default;
	Notification(Notification&&);

	SubscriptionId createSubscriber();
	int removeSubscriber(const int id);

	template<typename... Args>
	void notify(Args&&... args);

private:
	std::string signalName;
	std::list<std::shared_ptr<Socket>> subscribers;
	std::mutex subscriberLock;
};

template<typename... Args>
void Notification::notify(Args&&... args)
{
	Message msg(Message::Signal, signalName);
	msg.packParameters(std::forward<Args>(args)...);

	std::lock_guard<std::mutex> lock(subscriberLock);

	for (const std::shared_ptr<Socket>& subscriber : subscribers) {
		try {
			msg.encode(*subscriber);
		} catch (runtime::Exception& e) {
			ERROR(e.what());
		}
	}
}

} // namespae rmi
#endif //__RMI_NOTIFICATION_H__
