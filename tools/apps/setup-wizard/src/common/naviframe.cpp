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

#include "naviframe.h"

namespace klay {

Navigator::Navigator(Widget *parent, const std::string& name) :
	Layout(parent)
{
	addEventHandler(name);
}

Navigator::Navigator(Widget *parent, const std::string& file, const std::string& group, const std::string& name) :
	Layout(parent, file, group)
{
	addEventHandler(name);
}

void Navigator::addEventHandler(const std::string& name)
{
	Widget::addSignalHandler("next", name,
		[this](const std::string& emission, const std::string& source) {
			next();
	});
	Widget::addSignalHandler("previous", name,
		[this](const std::string& emission, const std::string& source) {
			previous();
	});
}

NaviItem::NaviItem(Elm_Object_Item* object) :
	item(object)
{
}

void NaviItem::setTitleEnabled(bool enabled, bool transition)
{
	elm_naviframe_item_title_enabled_set(item, enabled, transition);
}

void NaviItem::setEventHandler(Navigator *listener)
{
	elm_naviframe_item_pop_cb_set(item, eventHandlerDispatcher, reinterpret_cast<void*>(listener));
}

Eina_Bool NaviItem::eventHandlerDispatcher(void *data, Elm_Object_Item* it)
{
	Navigator* listener = reinterpret_cast<Navigator *>(data);
	listener->previous();
	return true;
}

NaviFrame::NaviFrame(Widget* parent)
{
	nativeObject = elm_naviframe_add(Widget::object(parent));
	eext_object_event_callback_add(nativeObject, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	elm_naviframe_content_preserve_on_pop_set(nativeObject, EINA_TRUE);
}

NaviFrame::~NaviFrame()
{
}

NaviItem NaviFrame::push(Navigator* content)
{
	NaviItem item(elm_naviframe_item_push(nativeObject, NULL, NULL, NULL, Widget::object(content), NULL));
	item.setTitleEnabled(false, true);
	item.setEventHandler(content);
	return item;
}

NaviItem NaviFrame::push(Navigator* content, const std::string& title)
{
	NaviItem item(elm_naviframe_item_push(nativeObject, title.c_str(), NULL, NULL, Widget::object(content), NULL));
	item.setTitleEnabled(true, true);
	item.setEventHandler(content);
	return item;
}

void NaviFrame::pop()
{
	elm_naviframe_item_pop(nativeObject);
}

} // namespace klay
