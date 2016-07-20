/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "button.h"

namespace klay {

Button::Button(Widget* parent, const std::string& text)
{
	onClick = []() {
	};

	nativeObject = elm_button_add(Widget::object(parent));
	evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	setText(text);
	setStyle("bottom");

	addSignalHandler("elm,action,click", "", [this](const std::string& emission, const std::string& source){
		this->onClick();
	});
}

Button::~Button()
{
}

} // namespace klay