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

#ifndef __KLAY_UX_WINDOW_H__
#define __KLAY_UX_WINDOW_H__

#include <string>
#include <functional>

#include "layout.h"
#include "conformant.h"

namespace klay {

class Window : public Widget {
public:
	typedef Elm_Win_Type Type;

	enum Flags {
		SetConformant = (1 << 0),
		SetAlpha = (1 << 1),
		SetAutodel = (1 << 2),
		SetIndicatorShow = (1 << 3),
		SetIndicatorOpacity = (1 << 4),
		SetBackExit = (1 << 5)
	};

	Window(klay::Widget* parent, const std::string& name, Type type, int flags);
	virtual ~Window();

	virtual void onDelete() {}

	void setTitle(const std::string& title);
	void activate();
	void show();
	std::string getTitle() const;
	void indicatorOverlap(bool enable)
	{
		if (enable) {
			elm_object_signal_emit(Widget::object(conformant), "elm,state,indicator,overlap", "elm");
		} else {
			elm_object_signal_emit(Widget::object(conformant), "elm,state,indicator,nooverlap", "elm");
		}
	}

protected:
	virtual bool createPartContents(Layout *layout) = 0;
	bool init(Widget* parent, const std::string& name, Type type, int style);

private:
	static void onWindowDeleteInternal(void *data, Evas_Object *obj, void *event_info);

private:
	Conformant *conformant;
	Layout *defaultLayout;
};

class ApplicationWindow : public Window {
public:
	ApplicationWindow(const std::string& name, int flags) :
		Window(nullptr, name, ELM_WIN_BASIC, flags | SetConformant | SetAutodel | SetBackExit)
	{
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizardMainFrame()\n");
	}
};

} // namespace klay

#endif //!__KLAY_WINDOW_H__
