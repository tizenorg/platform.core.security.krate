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
#include <app.h>
#include "common/widget.h"

#include "common/window.h"
#include "common/layout.h"
#include "common/button.h"
#include "common/naviframe.h"

#define EDJE_FILE "/usr/apps/org.tizen.krate-setup-wizard/res/org.tizen.krate-setup-wizard.edj"

class SetupWizardBaseLayout : public Layout
{
public:
        SetupWizardBaseLayout(Widget *parent) : Layout(parent, EDJE_FILE, "base_layout") {}

        virtual void setContentLayout(Widget *content) {
                this->setContent(content, "content_layout");
        }

        virtual void setBottomLayout(Widget *content) {
                this->setContent(content, "bottom_layout");
        }
};

class SetupWizardWelcomeFrame : public SetupWizardBaseLayout
{
public:
        SetupWizardWelcomeFrame(Widget *parent);
private:
	Layout *welcomeLayout;
	Layout *twoButtonLayout;
        Button *cancel_button;
        Button *next_button;
};

class SetupWizardMainFrame : public Window {
public:
	SetupWizardMainFrame();

	bool createPartContents(Layout* layout);
private:
	static void onDelete(void* data);
private:
	Naviframe *naviframe;
	SetupWizardWelcomeFrame *welcomeframe;
};
