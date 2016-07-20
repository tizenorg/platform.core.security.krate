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

NaviItem::NaviItem(Elm_Object_Item* object) :
	item(object)
{
}

Eina_Bool NaviItem::eventHandlerDispatcher(void *data, Elm_Object_Item* it)
{
	NaviItemEventListener* listener = reinterpret_cast<NaviItemEventListener *>(data);
	return listener->handleNaviItemEvent();
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

NaviItem NaviFrame::push(Widget* content)
{
	NaviItem item(elm_naviframe_item_push(nativeObject, NULL, NULL, NULL, Widget::object(content), NULL));
	item.setTitleEnabled(false, true);
	return item;
}

NaviItem NaviFrame::push(Widget* content, const std::string& title)
{
	NaviItem item(elm_naviframe_item_push(nativeObject, title.c_str(), NULL, NULL, Widget::object(content), NULL));
	item.setTitleEnabled(true, true);

	return item;
}

void NaviFrame::pop()
{
	elm_naviframe_item_pop(nativeObject);
}

} // namespace klay
