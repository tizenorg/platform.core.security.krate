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
 */

#include <app.h>
#include "setup-text.h"
#include "common/widget.h"

#include "common/window.h"
#include "common/layout.h"
#include "common/button.h"
#include "common/naviframe.h"
#include "common/popup.h"
#include "common/timer.h"
#include "common/progressbar.h"
#include "common/genlist.h"
#include "common/radio.h"
#include "common/entry.h"

class WelcomePage : public klay::Navigator {
public:
	WelcomePage(klay::Widget* parent);

private:
	void createPartContents();
private:
    const std::string textList[2][4] = {
        {"Welcome", "Use your applications separately<br>with SZ.<br>The folder will be created on your<br>personal home screen.", "Cancel", "Next"},
        {"Deletion", "The folder will be removed from<br> your personal home screen.", "Cancel", "Remove"}
    };
    const std::string *text;
};

class SecuritySetupPage : public klay::Navigator {
public:
	SecuritySetupPage(Widget *parent);

private:
	void createPartContents();
};

class PasswordSetupPage : public klay::Navigator, public klay::GenlistContentProvider {
public:
	PasswordSetupPage(Widget *parent);

	char *getLabel(const std::string& name, const std::string& part);
	klay::Widget *getWidget(klay::Genlist* parent, const std::string& name, const std::string& part);
private:
	void createPartContents();
private:
	klay::Layout *entryLayout;
	klay::Entry *entry;
	klay::Button *nextButton;
};

class KrateSetupPage: public klay::Navigator, public klay::TimerEventListener {
public:
	KrateSetupPage(Widget *parent);
	void addTimer();
	bool timerEvent();

private:
	void createPartContents();

private:
	klay::Progressbar *progressbar;
};

class SetupWizardMainFrame : public klay::ApplicationWindow {
public:
	SetupWizardMainFrame();
	bool createPartContents(klay::Layout* layout);

	void showWelcomePage();
	void showSecuritySetupPage();
	void showKrateSetupPage();
	void showPasswordSetupPage();

private:
	WelcomePage *welcome;
	SecuritySetupPage *securitySetup;
	PasswordSetupPage *passwordSetup;
	KrateSetupPage* krateSetup;

	klay::NaviFrame *naviframe;
};
