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

#include "popup.h"

namespace klay {

Popup::Popup(Widget *parent, const std::string &content)
{
	create(parent, content);
}

Popup::Popup(Widget *parent, const std::string &title, const std::string &content)
{
	create(parent, content);
	setText(title, "title,text");
	elm_object_item_part_text_translatable_set(nativeObject, "title,text", EINA_TRUE);
}

void Popup::create(Widget *parent, const std::string& text)
{
	nativeObject = elm_popup_add(Widget::object(parent));
	evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	setText(text);
}

} //namespace klay
