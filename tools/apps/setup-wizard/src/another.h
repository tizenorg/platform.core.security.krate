/*
 * Tizen Zone Setup-Wizard application
 *
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

#ifndef __KLAY_ANOTHER_H__
#define __KLAY_ANOTHER_H__

#include "layout.h"

class Button : public Widget {
public:
	Button(Layout* layout, const std::string& text) :
		Widget(layout)
	{
		nativeObject = elm_button_add(nativeParent);

		evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_object_text_set(nativeObject, text.c_str());
		elm_object_style_set(nativeObject, "bottom");

		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "BUtton Created\n");
	}
};

#endif //!__KLAY_ANOTHER_H__
