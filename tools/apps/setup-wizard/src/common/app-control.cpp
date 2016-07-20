/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "app-control.h"

namespace klay {

AppControl::AppControl(const std::string &id)
{
	app_control_create(&handle);
	app_control_set_app_id(handle, id.c_str());
}

AppControl::AppControl(const std::string &id, const std::string &uri)
{
	app_control_create(&handle);
	app_control_set_app_id(handle, id.c_str());
	app_control_set_uri(handle, uri.c_str());
}

AppControl::AppControl(app_control_h app_control)
{
	app_control_clone(&handle, app_control);
}
AppControl::~AppControl()
{
	app_control_destroy(handle);
}

AppControl::operator app_control_h()
{
	return handle;
}

std::string AppControl::getData(const std::string &key) const
{
	char *data;
	int ret = app_control_get_extra_data(handle, key.c_str(), &data);
	if (ret != APP_CONTROL_ERROR_NONE)
		return "";
	return data;
}
void AppControl::setData(const std::string &key, const std::string &data)
{
	app_control_add_extra_data(handle, key.c_str(), data.c_str());
}

void AppControl::launch(void)
{
	app_control_send_launch_request(handle, NULL, NULL);
}

} // namespace klay