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
#include "krate-setup.h"
#include "widget.h"

#include "window.h"
#include "layout.h"
#include "another.h"

class SetupWizardMainFrame : public Window {
public:
	SetupWizardMainFrame()
	{
		create(std::string("Zone Setup Wizard"), ELM_WIN_BASIC,
			   SetConformant | SetAutodel | SetAlpha | SetIndicatorShow);
	}

	bool createPartContents(Layout* layout)
	{
		button = new Button(layout, "Test Button");
		button->show();
		return true;
	}

	void onDelete()
	{
		ui_app_exit();
	}
private:
	Button *button;
};
