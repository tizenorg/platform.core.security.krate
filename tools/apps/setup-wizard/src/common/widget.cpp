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

#include <Elementary.h>

#include "widget.h"

namespace klay {

Widget::Widget() :
	nativeObject(nullptr)
{
}

Widget::~Widget()
{
}

Widget::operator Evas_Object* ()
{
	return nativeObject;
}

void Widget::dispose()
{
}

void Widget::show()
{
	if (nativeObject) {
		evas_object_show(nativeObject);
	}
}

void Widget::remove()
{
	if (nativeObject) {
		evas_object_del(nativeObject);
	}
}

void Widget::setText(const std::string& text)
{
	elm_object_text_set(nativeObject, text.c_str());
}

void Widget::setText(const std::string& text, const std::string& part)
{
	elm_object_part_text_set(nativeObject, part.c_str(), text.c_str());
}

void Widget::setStyle(const std::string& style)
{
	elm_object_style_set(nativeObject, style.c_str());
}

void Widget::setContent(Widget* content)
{
	elm_object_content_set(nativeObject, content->nativeObject);
}

void Widget::setContent(Widget* content, const std::string& part)
{
	elm_object_part_content_set(nativeObject, part.c_str(), content->nativeObject);
}

void Widget::setFocus(bool enabled)
{
	elm_object_focus_set(nativeObject, enabled);
}

void Widget::setDisabled(bool disabled)
{
	elm_object_disabled_set(nativeObject, disabled);
}

void Widget::emitSignal(const std::string& emission, const std::string& source)
{
	elm_object_signal_emit(nativeObject, emission.c_str(), source.c_str());
}

Evas_Object *Widget::object()
{
	return nativeObject;
}

Evas_Object *Widget::object(Widget* rhs)
{
	if (rhs) {
		return rhs->nativeObject;
	}
	return nullptr;
}

void Widget::eventHandlerDispatcher(void *data, Evas *evas, Evas_Object *obj, void *event_info)
{
	EventInfo *info = reinterpret_cast<EventInfo *>(data);
	info->handler(event_info);
}

void Widget::signalHandlerDispatcher(void *data, Evas_Object* obj, const char* emission, const char *source)
{
	SignalInfo *info = reinterpret_cast<SignalInfo *>(data);
	info->handler(emission, source);
}

void Widget::smartHandlerDispatcher(void *data, Evas_Object* obj, void *event_info)
{
	SmartInfo *info = reinterpret_cast<SmartInfo *>(data);
	info->handler(event_info);
}

void Widget::addEventHandler(Evas_Callback_Type type, Widget::EventHandler&& handler)
{
	EventInfo *info = new EventInfo();
	info->klass = this;
	info->type = type;
	info->handler = std::move(handler);

	evas_object_event_callback_add(nativeObject, type, eventHandlerDispatcher, reinterpret_cast<void *>(info));
}

void Widget::removeEventHandler(Evas_Callback_Type type)
{
	void *data = evas_object_event_callback_del(nativeObject, type, eventHandlerDispatcher);
	if (data != NULL) {
		delete reinterpret_cast<EventInfo *>(data);
	}
}

void Widget::addSignalHandler(const std::string& emission, const std::string& source, SignalHandler&& handler)
{
	SignalInfo *info = new SignalInfo();
	info->klass = this;
	info->handler = std::move(handler);

	elm_object_signal_callback_add(nativeObject, emission.c_str(), source.c_str(), signalHandlerDispatcher, reinterpret_cast<void *>(info));
}

void Widget::removeSignalHandler(const std::string& emission, const std::string& source)
{
	void *data = elm_object_signal_callback_del(nativeObject, emission.c_str(), source.c_str(), signalHandlerDispatcher);
	if (data != NULL) {
		delete reinterpret_cast<SignalInfo *>(data);
	}
}

void Widget::addSmartHandler(const std::string& event, Widget::SmartHandler&& handler)
{
	SmartInfo *info = new SmartInfo();
	info->klass = this;
	info->handler = std::move(handler);
	evas_object_smart_callback_add(nativeObject, event.c_str(), smartHandlerDispatcher, reinterpret_cast<void*>(info));
}

void Widget::removeSmartHandler(const std::string& event)
{
	void *data = evas_object_smart_callback_del(nativeObject, event.c_str(), smartHandlerDispatcher);
	if (data != NULL) {
		delete reinterpret_cast<SmartInfo *>(data);
	}
}

} // namespace klay
