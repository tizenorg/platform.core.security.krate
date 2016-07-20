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

#ifndef __KLAY_UX_POPUP_H__
#define __KLAY_UX_POPUP_H__

#include "widget.h"

namespace klay {

class Popup : public Widget {
public:
	Popup(Widget *parent, const std::string& content)
	{
		nativeObject = elm_popup_add(Widget::object(parent));
		evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		setText(content);
	}

	Popup(Widget *parent, const std::string& title, const std::string& content)
	{
		nativeObject = elm_popup_add(Widget::object(parent));
		evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		setText(title, "title,text");
		elm_object_item_part_text_translatable_set(nativeObject, "title,text", EINA_TRUE);
		setText(content);
	}

private:
};

class TimerEventListener {
public:
	TimerEventListener() {}
	virtual ~TimerEventListener() {}

	virtual bool timerEvent(void) = 0;
};

class Timer {
public:
	Timer(TimerEventListener *listener, double time)
	{
		timer = ecore_timer_add(time, eventHandlerDispatcher, reinterpret_cast<void*>(listener));
	}

	void remove()
	{
		ecore_timer_del(timer);
	}
private:
	static Eina_Bool eventHandlerDispatcher(void *data) {
		TimerEventListener* listener = reinterpret_cast<TimerEventListener *>(data);
		return listener->timerEvent();
	}
private:
	Ecore_Timer *timer;
};

class Progressbar : public Widget {
public:
	Progressbar(Widget* parent)
	{
		nativeObject = elm_progressbar_add(Widget::object(parent));
		evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_progressbar_pulse(nativeObject, EINA_TRUE);
		elm_progressbar_pulse_set(nativeObject, EINA_TRUE);
		evas_object_show(nativeObject);
	}

	virtual ~Progressbar() {}

	void setPulseState(bool state)
	{
		elm_progressbar_pulse(nativeObject, state);
	}

	double getValue()
	{
		return elm_progressbar_value_get(nativeObject);
	}

	void setValue(double time)
	{
		elm_progressbar_value_set(nativeObject, time);
	}
};

} // namespace klay

#endif
