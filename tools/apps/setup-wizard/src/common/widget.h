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

#ifndef __KLAY_UX_WIDGET_H__
#define __KLAY_UX_WIDGET_H__

#include <string>
#include <unordered_map>
#include <functional>

#include <app.h>
#include <string>
#include <Elementary.h>
#include <efl_extension.h>

#include "debug.h"

namespace klay {

class Widget {
public:
	typedef std::function<void(void* event)> EventHandler;
	typedef std::function<void(const std::string& emission, const std::string& source)> SignalHandler;

	Widget() :
		nativeObject(nullptr)
	{
	}

	virtual ~Widget()
	{
	}

	operator Evas_Object*()
	{
		return nativeObject;
	}

	virtual void dispose()
	{
	}

	virtual void show()
	{
		if (nativeObject) {
			evas_object_show(nativeObject);
		}
	}

	virtual void remove() {
		if (nativeObject) {
			evas_object_del(nativeObject);
		}
	}

	virtual void setText(const std::string& text)
	{
		elm_object_text_set(nativeObject, text.c_str());
	}

	virtual void setText(const std::string& text, const std::string& part)
	{
		elm_object_part_text_set(nativeObject, part.c_str(), text.c_str());
	}

	virtual void setStyle(const std::string& style)
	{
		elm_object_style_set(nativeObject, style.c_str());
	}

	virtual void setContent(Widget* content)
	{
		elm_object_content_set(nativeObject, content->nativeObject);
	}

	virtual void setContent(Widget* content, const std::string& part)
	{
		elm_object_part_content_set(nativeObject, part.c_str(), content->nativeObject);
	}

	virtual void setFocus(bool enabled)
	{
		elm_object_focus_set(nativeObject, enabled);
	}

	virtual void setDisabled(bool disabled)
	{
		elm_object_disabled_set(nativeObject, disabled);
	}

	virtual void emitSignal(const std::string& emission, const std::string& source)
	{
		elm_object_signal_emit(nativeObject, emission.c_str(), source.c_str());
	}

	void addEventHandler(Evas_Callback_Type type, EventHandler&& handler);
	void removeEventHandler(Evas_Callback_Type type);

	void addSignalHandler(const std::string& emission, const std::string& source, SignalHandler&& handler);
	void removeSignalHandler(const std::string& emission, const std::string& source);

protected:
	Evas_Object* object()
	{
		return nativeObject;
	}

	static Evas_Object* object(Widget* rhs)
	{
		if (rhs) {
			return rhs->nativeObject;
		}

		return NULL;
	}

	Evas_Object* nativeObject;

private:
	static void signalHandlerDispatcher(void *data, Evas_Object* obj, const char* emission, const char *source);
	static void eventHandlerDispatcher(void *data, Evas *evas, Evas_Object *obj, void *event_info);

private:
	struct EventInfo {
		Widget* klass;
		Evas_Callback_Type type;
		EventHandler handler;
	};

	struct SignalInfo {
		Widget* klass;
		SignalHandler handler;
	};
};

} // namespace klay

#endif //!__KLAY_WIDGET_H__
