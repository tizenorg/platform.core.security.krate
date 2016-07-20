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

#ifndef __KLAY_UX_NAVIFRAME_H__
#define __KLAY_UX_NAVIFRAME_H__

#include <functional>

#include "widget.h"
#include "layout.h"

namespace klay {

class NaviFrame;

class Navigator : public Layout {
public:
	Navigator(Widget* parent, const std::string& name);
	Navigator(Widget* parent, const std::string& file, const std::string& group, const std::string& name);

	std::function<void()> next;
	std::function<void()> previous;
private:
	void addEventHandler(const std::string& name);
};

class NaviItem {
public:
	std::function<bool()> EventHandler;

	void setTitleEnabled(bool enabled, bool transition);
	void setEventHandler(Navigator *listener);

	friend class NaviFrame;
private:
	static Eina_Bool eventHandlerDispatcher(void *data, Elm_Object_Item* it);
	NaviItem(Elm_Object_Item* object);
private:
	Elm_Object_Item* item;
};

class NaviFrame : public Widget {
public:
	NaviFrame(Widget *parent);
	virtual ~NaviFrame();

	NaviItem push(Navigator* content);
	NaviItem push(Navigator* content, const std::string& title);

	void pop();
};

} // namespace klay

#endif //!__KLAY_NAVIFRAME_H__
