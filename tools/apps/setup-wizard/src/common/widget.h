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

#ifndef __KLAY_WIDGET_H__
#define __KLAY_WIDGET_H__

#include <string>
#include <Elementary.h>

#include "debug.h"

class Widget {
public:
	Widget() :
		nativeParent(nullptr), nativeObject(nullptr)
	{
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "Widget Default contructor\n");
	}

	Widget(Widget* parent) :
		nativeParent(parent->nativeObject), nativeObject(nullptr)
	{
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "Widget Copy contructor\n");
	}

	virtual ~Widget()
	{
	}

	virtual void show()
	{
		if (nativeObject) {
			dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "Show widget\n");
			evas_object_show(nativeObject);
		}
	}

	virtual void setText(const std::string& text) {
		elm_object_text_set(nativeObject, text.c_str());
	}

	virtual void setStyle(const std::string& style) {
		elm_object_style_set(nativeObject, style.c_str());
	}

	virtual void setContent(Widget *content) {
	}

	virtual void setContent(Widget *content, const std::string& part) {
	}
protected:
	Evas_Object* nativeParent;
	Evas_Object* nativeObject;
};

#endif //!__KLAY_WIDGET_H__
