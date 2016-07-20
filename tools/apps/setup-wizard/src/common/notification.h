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
#ifndef __KLAY_UX_NOTIFICATION_H__
#define __KLAY_UX_NOTIFICATION_H__

#include <notification.h>
#include "app-control.h"

namespace klay {

class Notification {
public:
	enum Flags {
		setDefault = (1 << 0),
		setDisplayApplist = (1 << 1)
	};

	Notification(const std::string& title);
	Notification(const std::string& title, const std::string& content);
	Notification(const std::string& title, const std::string& content, const std::string& image);
	Notification(const std::string& title, const std::string& content, const std::string& image, AppControl* appControl);

	void post();

private:
	void create();
private:
	const std::string _title;
	const std::string _content;
	const std::string _image;
	AppControl *_appControl;
	notification_h handle;
};

} //namespace klay

#endif
