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

#include "window.h"

namespace klay {

Window::Window(Widget* parent, const std::string& name, Type type, int style) :
	conformant(nullptr), defaultLayout(nullptr)
{
	nativeObject = elm_win_add(Widget::object(parent), name.c_str(), type);

	if (style & SetIndicatorShow) {
		elm_win_indicator_mode_set(nativeObject, ELM_WIN_INDICATOR_SHOW);
	}

	if (style & SetIndicatorOpacity) {
		elm_win_indicator_opacity_set(nativeObject, ELM_WIN_INDICATOR_TRANSPARENT);
	}

	if (style & SetConformant) {
		elm_win_conformant_set(nativeObject, EINA_TRUE);
	}

	if (style & SetAutodel) {
		elm_win_autodel_set(nativeObject, EINA_TRUE);
	}

	if (style & SetAlpha) {
		elm_win_alpha_set(nativeObject, EINA_TRUE);
	}

	if (style & SetBackExit) {
		eext_object_event_callback_add(nativeObject, EEXT_CALLBACK_BACK, onWindowDeleteInternal, this);
	}

	evas_object_smart_callback_add(nativeObject, "delete,request", onWindowDeleteInternal, this);

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "Window()\n");
}

Window::~Window()
{
}

void Window::onWindowDeleteInternal(void* data, Evas_Object* obj, void* event_info)
{
	Window* instance = (Window *)data;
	instance->onDelete();
}

void Window::show()
{
	conformant = new Conformant(this);
	defaultLayout = new Layout(conformant);
	conformant->setContent(defaultLayout);

	if (createPartContents(defaultLayout)) {
		defaultLayout->show();
		conformant->show();
		evas_object_show(nativeObject);
	}
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

void Window::indicatorOverlap(bool enable)
{
	if (enable) {
		elm_object_signal_emit(Widget::object(conformant), "elm,state,indicator,overlap", "elm");
	} else {
		elm_object_signal_emit(Widget::object(conformant), "elm,state,indicator,nooverlap", "elm");
	}
}

ApplicationWindow::ApplicationWindow(const std::string& name, int flags) :
	Window(nullptr, name, ELM_WIN_BASIC, flags | SetConformant | SetAutodel | SetBackExit)
{
	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizardMainFrame()\n");
}

ApplicationWindow::~ApplicationWindow()
{
}

} // namespace klay
