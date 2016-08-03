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

#include "debug.h"
#include "krate.h"

#include "client.h"
#include "rmi/manager.h"


using namespace Krate;

KrateContext& GetKrateContext(void* handle)
{
	return *reinterpret_cast<KrateContext*>(handle);
}

int krate_manager_create(krate_manager_h* handle)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext* client = new(std::nothrow) KrateContext();

	RET_ON_FAILURE(handle, KRATE_ERROR_CONNECTION_REFUSED);

	if (client->connect() < 0) {
		delete client;
		RET_ON_FAILURE(handle, KRATE_ERROR_CONNECTION_REFUSED);
	}

	*handle = reinterpret_cast<krate_manager_h>(client);

	return KRATE_ERROR_NONE;
}

int krate_manager_destroy(krate_manager_h handle)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);

	delete &GetKrateContext(handle);

	return KRATE_ERROR_NONE;
}

int krate_manager_add_event_cb(krate_manager_h handle, const char* event, krate_event_cb callback, void* user_data, int *id)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(event, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(callback, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &context = GetKrateContext(handle);
	int ret = context.subscribeSignal(std::string("Krate::Manager::") + event,
									  callback, user_data);
	if (ret < 0)
		return KRATE_ERROR_INVALID_PARAMETER;

	*id = ret;
	return KRATE_ERROR_NONE;
}

int krate_manager_remove_event_cb(krate_manager_h handle, int callback_id)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(callback_id >= 0, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &context = GetKrateContext(handle);
	int ret =  context.unsubscribeSignal(callback_id);
	if (ret)
		return KRATE_ERROR_INVALID_PARAMETER;

	return KRATE_ERROR_NONE;
}

int krate_manager_create_krate(krate_manager_h handle, const char* name, const char* manifest)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(manifest, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &client = GetKrateContext(handle);
	Krate::Manager krate = client.createKrateInterface<Krate::Manager>();
	return krate.createKrate(name, manifest);
}

int krate_manager_destroy_krate(krate_manager_h handle, const char* name)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &client = GetKrateContext(handle);
	Krate::Manager krate = client.createKrateInterface<Krate::Manager>();
	return krate.removeKrate(name);
}

int krate_manager_get_krate_state(krate_manager_h handle, const char* name, krate_state_e *state)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &client = GetKrateContext(handle);
	Krate::Manager krate = client.createKrateInterface<Krate::Manager>();

	int result = krate.getKrateState(name);
	if (result == 0) {
		return KRATE_ERROR_NO_DATA;
	}

	*state = (krate_state_e)result;
	return KRATE_ERROR_NONE;
}

int krate_manager_foreach_name(krate_manager_h handle, krate_state_e state,
						  krate_manager_foreach_cb callback, void* user_data)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(callback, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &client = GetKrateContext(handle);
	Krate::Manager krate = client.createKrateInterface<Krate::Manager>();
	std::vector<std::string> list = krate.getKrateList(state);
	for (std::vector<std::string>::iterator it = list.begin();
		 it != list.end(); it++) {
		if (!callback((*it).c_str(), user_data))
			break;
	}

	return KRATE_ERROR_NONE;
}

int krate_manager_reset_krate_password(krate_manager_h handle, const char* name, const char* new_password)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, KRATE_ERROR_INVALID_PARAMETER);

	if (new_password == NULL) {
		new_password = "";
	}

	KrateContext &client = GetKrateContext(handle);
	Krate::Manager krate = client.createKrateInterface<Krate::Manager>();

	return krate.resetKratePassword(name, new_password);
}
