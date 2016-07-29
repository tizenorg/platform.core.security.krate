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

#include "layout.h"

namespace klay {

Layout::Layout(Widget* parent)
{
	nativeObject = elm_layout_add(Widget::object(parent));
	evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(nativeObject, "layout", "application", "default");
}

Layout::Layout(Widget* parent, const std::string& file, const std::string& group)
{
	nativeObject = elm_layout_add(Widget::object(parent));
	evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_file_set(nativeObject, file.c_str(), group.c_str());
}

Layout::~Layout()
{
}

} // namespace klay
