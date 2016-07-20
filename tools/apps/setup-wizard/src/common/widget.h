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
	typedef std::function<void(void* event)> SmartHandler;

	Widget();
	virtual ~Widget();

	operator Evas_Object*();

	virtual void dispose();
	virtual void show();
	virtual void remove();
	virtual void setText(const std::string& text);
	virtual void setText(const std::string& text, const std::string& part);
	virtual void setStyle(const std::string& style);
	virtual void setContent(Widget* content);
	virtual void setContent(Widget* content, const std::string& part);
	virtual void setFocus(bool enabled);
	virtual void setDisabled(bool disabled);
	virtual void emitSignal(const std::string& emission, const std::string& source);

	void addEventHandler(Evas_Callback_Type type, EventHandler&& handler);
	void removeEventHandler(Evas_Callback_Type type);

	void addSignalHandler(const std::string& emission, const std::string& source, SignalHandler&& handler);
	void removeSignalHandler(const std::string& emission, const std::string& source);

	void addSmartHandler(const std::string& event, Widget::SmartHandler&& handler);
	void removeSmartHandler(const std::string& event);
protected:
	Evas_Object* object();
	static Evas_Object* object(Widget* rhs);

	Evas_Object* nativeObject;

private:
	static void signalHandlerDispatcher(void *data, Evas_Object* obj, const char* emission, const char *source);
	static void eventHandlerDispatcher(void *data, Evas *evas, Evas_Object *obj, void *event_info);
	static void smartHandlerDispatcher(void *data, Evas_Object* obj, void *event_info);
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

	struct SmartInfo {
		Widget* klass;
		SmartHandler handler;
	};
};

} // namespace klay

#endif //!__KLAY_WIDGET_H__
