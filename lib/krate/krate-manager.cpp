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

#include "krate/krate-manager.h"

#include "debug.h"
#include "array.h"

#include "client.h"
#include "rmi/krate.h"

using namespace Krate;

KrateContext& GetKrateContext(void* handle)
{
	return *reinterpret_cast<KrateContext*>(handle);
}

int zone_manager_create(zone_manager_h* handle)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext* client = new(std::nothrow) KrateContext();

	RET_ON_FAILURE(handle, KRATE_ERROR_CONNECTION_REFUSED);

	if (client->connect() < 0) {
		delete client;
		RET_ON_FAILURE(handle, KRATE_ERROR_CONNECTION_REFUSED);
	}

	*handle = reinterpret_cast<zone_manager_h>(client);

	return KRATE_ERROR_NONE;
}

int zone_manager_destroy(zone_manager_h handle)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);

	delete &GetKrateContext(handle);

	return KRATE_ERROR_NONE;
}

int zone_manager_add_event_cb(zone_manager_h handle, const char* event, zone_event_cb callback, void* user_data, int *id)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(event, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(callback, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &context = GetKrateContext(handle);
	int ret = context.subscribeSignal(std::string("ZoneManager::") + event,
									  callback, user_data);
	if (ret < 0)
		return KRATE_ERROR_INVALID_PARAMETER;

	*id = ret;
	return KRATE_ERROR_NONE;
}

int zone_manager_remove_event_cb(zone_manager_h handle, int callback_id)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(callback_id >= 0, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &context = GetKrateContext(handle);
	int ret =  context.unsubscribeSignal(callback_id);
	if (ret)
		return KRATE_ERROR_INVALID_PARAMETER;

	return KRATE_ERROR_NONE;
}

int zone_manager_create_zone(zone_manager_h handle, const char* name, const char* manifest)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(manifest, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &client = GetKrateContext(handle);
	ZoneManager zone = client.createPolicyInterface<ZoneManager>();
	return zone.createZone(name, manifest);
}

int zone_manager_destroy_zone(zone_manager_h handle, const char* name)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &client = GetKrateContext(handle);
	ZoneManager zone = client.createPolicyInterface<ZoneManager>();
	return zone.removeZone(name);
}

int zone_manager_get_zone_state(zone_manager_h handle, const char* name, zone_state_e *state)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &client = GetKrateContext(handle);
	ZoneManager zone = client.createPolicyInterface<ZoneManager>();

	int result = zone.getZoneState(name);
	if (result == 0) {
		return KRATE_ERROR_NO_DATA;
	}

	*state = (zone_state_e)result;
	return KRATE_ERROR_NONE;
}

int zone_manager_foreach_name(zone_manager_h handle, zone_state_e state,
						  zone_manager_foreach_cb callback, void* user_data)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(callback, KRATE_ERROR_INVALID_PARAMETER);

	KrateContext &client = GetKrateContext(handle);
	ZoneManager zone = client.createPolicyInterface<ZoneManager>();
	std::vector<std::string> list = zone.getZoneList(state);
	for (std::vector<std::string>::iterator it = list.begin();
		 it != list.end(); it++) {
		if (!callback((*it).c_str(), user_data))
			break;
	}

	return KRATE_ERROR_NONE;
}

int zone_manager_reset_zone_password(zone_manager_h handle, const char* name, const char* new_password)
{
	RET_ON_FAILURE(handle, KRATE_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(name, KRATE_ERROR_INVALID_PARAMETER);

	if (new_password == NULL) {
		new_password = "";
	}

	KrateContext &client = GetKrateContext(handle);
	ZoneManager zone = client.createPolicyInterface<ZoneManager>();

	return zone.resetZonePassword(name, new_password);
}