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

#include <efl_extension.h>

#include "window.h"

Window::Window()
{
}

Window::~Window()
{
}

void Window::onWindowDeleteInternal(void* data, Evas_Object* obj, void* event_info)
{
	Window* instance = (Window *)data;
	instance->onDelete();
}

Eina_Bool Window::eventHandlerDispatcher(void* data, int type, void *event)
{
	Window* instance = (Window *)data;
	instance->eventHandlerMap[type](type);

	return true;
}

bool Window::create(const std::string& name, Window::Type type, int flags)
{
	nativeObject = elm_win_add(NULL, name.c_str(), type);

	if (flags & SetIndicatorShow) {
		elm_win_indicator_mode_set(nativeObject, ELM_WIN_INDICATOR_SHOW);
	}

	if (flags & SetIndicatorOpacity) {
		elm_win_indicator_opacity_set(nativeObject, ELM_WIN_INDICATOR_TRANSPARENT);
	}

	if (flags & SetConformant) {
    	elm_win_conformant_set(nativeObject, EINA_TRUE);
	}

	if (flags & SetAutodel) {
    	elm_win_autodel_set(nativeObject, EINA_TRUE);
	}

	if (flags & SetAlpha) {
    	elm_win_alpha_set(nativeObject, EINA_TRUE);
	}

    evas_object_smart_callback_add(nativeObject, "delete,request", onWindowDeleteInternal, this);

	defaultLayout = new Layout(this);
	bool ret = createPartContents(defaultLayout);
	defaultLayout->show();

	return ret;
}

void Window::addEventHandler(int event, Window::EventHandler&& handler)
{
	eventHandlerMap[event] = std::move(handler);
	ecore_event_handler_add(event, eventHandlerDispatcher, this);
}

void Window::setTitle(const std::string& title)
{
	elm_win_title_set(nativeObject, title.c_str());
}

void Window::activate()
{
	elm_win_activate(nativeObject);
}

std::string Window::getTitle() const
{
	return elm_win_title_get(nativeObject);
}
