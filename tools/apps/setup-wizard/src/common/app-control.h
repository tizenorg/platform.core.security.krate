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

#ifndef __KLAY_UX_APPCONTROL_H__
#define __KLAY_UX_APPCONTROL_H__

#include <app.h>
#include "widget.h"

namespace klay {

class AppControl {
public:
	AppControl(const std::string &id);
	AppControl(const std::string &id, const std::string &uri);
	AppControl(app_control_h app_control);
	~AppControl();

	operator app_control_h();

	std::string getData(const std::string &key) const;
	void setData(const std::string &key, const std::string &data);
	void launch(void);
private:
	app_control_h handle;
};

} //namespace klay

#endif
