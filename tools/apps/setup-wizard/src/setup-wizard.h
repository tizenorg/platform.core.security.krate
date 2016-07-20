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
#include "common/genlist.h"
#include "common/radio.h"
#include "common/entry.h"

class PageController {
public:
	PageController(klay::Widget* object, const std::string& name);

	std::function<void()> next;
	std::function<void()> previous;
};

class WelcomePage : public klay::Layout, public klay::NaviItemEventListener, public PageController {
public:
	WelcomePage(klay::Widget* parent);
	bool handleNaviItemEvent();

private:
	void createPartContents();

private:
    const std::string text[2][4] = {
        {CREATE_MESSAGE_TITLE, CREATE_MESSAGE_CONTENT, CANCEL_BUTTON, NEXT_BUTTON},
        {REMOVE_MESSAGE_TITLE, REMOVE_MESSAGE_CONTENT, CANCEL_BUTTON, REMOVE_BUTTON}
    };

    const std::string *selectedText;
};

class SecuritySetupPage : public klay::Layout, public klay::NaviItemEventListener, public PageController {
public:
	SecuritySetupPage(Widget *parent);
	bool handleNaviItemEvent();

private:
	void createPartContents();
};

class PasswordSetupPage : public klay::Layout, public klay::NaviItemEventListener, public PageController {
public:
	PasswordSetupPage(Widget *parent);
	bool handleNaviItemEvent();

private:
	void createPartContents();
};

class KrateSetupPage: public klay::Layout, public klay::NaviItemEventListener, public klay::TimerEventListener {
public:
	KrateSetupPage(Widget *parent);
	bool handleNaviItemEvent();
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
