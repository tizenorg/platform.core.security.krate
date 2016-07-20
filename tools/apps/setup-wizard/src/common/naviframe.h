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

#ifndef __KLAY_NAVIFRAME_H__
#define __KLAY_NAVIFRAME_H__

#include <functional>

#include "widget.h"

class NaviFrame;

class NaviItemEventListener {
public:
	NaviItemEventListener()
	{
	}

	virtual ~NaviItemEventListener()
	{
	}

	virtual bool handleNaviItemEvent() = 0;
};

class NaviItem {
public:
	std::function<bool()> EventHandler;

	void setTitleEnabled(bool enabled, bool transition)
	{
		elm_naviframe_item_title_enabled_set(item, enabled, transition);
	}

	void setEventListener(NaviItemEventListener* listener)
	{
		elm_naviframe_item_pop_cb_set(item, eventHandlerDispatcher, reinterpret_cast<void *>(listener));
	}

	friend class NaviFrame;

private:
	NaviItem(Elm_Object_Item* object);
	static Eina_Bool eventHandlerDispatcher(void *data, Elm_Object_Item* it);

private:
	Elm_Object_Item* item;
};

class NaviFrame : public Widget {
public:
	NaviFrame(Widget *parent);
	virtual ~NaviFrame();

	NaviItem push(Widget* content);
	NaviItem push(Widget* content, const std::string& title);
	int isPreserved(Widget* content) {
		return elm_naviframe_content_preserve_on_pop_get(Widget::object(content));
	}
	void pop();
};

#endif //!__KLAY_NAVIFRAME_H__
