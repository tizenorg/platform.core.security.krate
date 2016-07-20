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

#include "notification.h"

namespace klay {

Notification::Notification(const std::string &title) :
	_title(title), _content(""), _image(""), _appControl(nullptr)
{
	create();
}

Notification::Notification(const std::string &title, const std::string &content) :
	_title(title), _content(content), _image(""), _appControl(nullptr)
{
	create();
}

Notification::Notification(const std::string &title, const std::string &content, const std::string &image) :
	_title(title), _content(content), _image(image), _appControl(nullptr)
{
	create();
}

Notification::Notification(const std::string &title, const std::string &content, const std::string &image, AppControl *appControl) :
	_title(title), _content(content), _image(image), _appControl(appControl)
{
	create();
}

void Notification::create()
{
	handle = notification_create(NOTIFICATION_TYPE_NOTI);
#if 0
	if (style & setDisplayApplist) {
		notification_set_display_applist(handle, NOTIFICATION_DISPLAY_APP_ALL);
	}
#endif
	if (_title != "") {
		notification_set_text(handle, NOTIFICATION_TEXT_TYPE_TITLE,
			  _title.c_str(), NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	}

	if (_content != "") {
		notification_set_text(handle, NOTIFICATION_TEXT_TYPE_CONTENT,
			  _content.c_str(), NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	}

	if (_image != "") {
		notification_set_image(handle, NOTIFICATION_IMAGE_TYPE_THUMBNAIL, _image.c_str());
	}

	if (_appControl != nullptr) {
		notification_set_launch_option(handle,
			   NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, *_appControl);
	}
}

void Notification::post()
{
	notification_post(handle);
}
} // namespace klay
