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

#ifndef __POLICY_CLIENT_H__
#define __POLICY_CLIENT_H__

#include <string>
#include <memory>
#include <functional>

#include <klay/rmi/client.h>

typedef std::function<void(const char*, const char*, void*)> KrateChangeListener;
typedef std::function<void(const char*, const char*, void*)> SignalListener;

class KrateContext {
public:
	typedef std::unique_ptr<rmi::Client> KrateControlContext;

	KrateContext() noexcept;
	~KrateContext() noexcept;

	int connect() noexcept;
	int connect(const std::string& address) noexcept;
	void disconnect() noexcept;

	int subscribeSignal(const std::string& name, const SignalListener& listener, void* data);
	int unsubscribeSignal(int subscriberId);

	template<typename Krate, typename... Args>
	Krate createKrateInterface(Args&&... args) noexcept
	{
		return Krate(getKrateControlContext(), std::forward<Args>(args)...);
	}

private:
	KrateControlContext& getKrateControlContext()
	{
		return client;
	}

	KrateControlContext client;
};

KrateContext& GetKrateContext(void* handle);
#endif //__POLICY_CLIENT_H__
