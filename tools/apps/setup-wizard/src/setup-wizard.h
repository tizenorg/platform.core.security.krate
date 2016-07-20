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
#include <limits.h>
#include <fstream>
#include <zone/zone.h>
#include <zone/app-proxy.h>

#include "common/debug.h"
#include "common/application.h"
#include "common/shell.h"

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
#include "common/notification.h"

class PasswordManager {
public:
	PasswordManager();
	~PasswordManager();

	bool verify(const std::string & pwd);
	void setPassword(const std::string& pwd);
protected:
	std::string password;
};

class WelcomePage : public klay::Navigator {
public:
	WelcomePage(klay::Widget* parent, int mode);

private:
	void createPartContents();
private:
    const std::string textList[2][4] = {
        {"Welcome", "Use your applications separately<br>with SZ.<br>The folder will be created on your<br>personal home screen.", "Cancel", "Next"},
        {"Deletion", "The folder will be removed<br>from your personal home screen.", "Cancel", "Remove"}
    };
    const std::string *text;
};

class SecuritySetupPage : public klay::Navigator {
public:
	SecuritySetupPage(Widget *parent);

private:
	void createPartContents();
};

class PasswordSetupLayout : public klay::Navigator, public klay::GenlistContentProvider {
public:
	PasswordSetupLayout(Widget *parent, PasswordManager *manager, const std::string& name);

	char *getLabel(const std::string& name, const std::string& part);
	klay::Widget *getWidget(klay::Genlist* parent, const std::string& name, const std::string& part);
	void createPartContents();
private:
	klay::Layout *entryLayout;
        klay::Entry *entry;
        klay::Button *nextButton;
private:
	PasswordManager *pwdManager;
	std::string page;
};

class KrateSetupPage: public klay::Navigator {
public:
	KrateSetupPage(Widget *parent, int mode);
private:
	void createPartContents();
private:
	klay::Progressbar *progressbar;
	const std::string textList[2][2] = {
		{"Creating Folder instance...", "The folder will be created on your<br>presonal home screen."},
		{"Deleting a folder...", "The folder will be removed<br>from your personal home screen."}
	};
	const std::string *text;
};

class SetupWizardMainFrame : public klay::ApplicationWindow, public PasswordManager {
public:
	enum Flags {
		CREATE_MODE = 0,
		REMOVE_MODE
	};

	SetupWizardMainFrame(int mode);
	bool createPartContents(klay::Layout* layout);

	void showWelcomePage();
	void showSecuritySetupPage();
	void showKrateSetupPage();
	void showPasswordSetupPage();
	void showPasswordVerifyPage();

private:
	void addSignalHandlers();
private:
	WelcomePage *welcome;
	SecuritySetupPage *securitySetup;
	PasswordSetupLayout *passwordSetup;
	PasswordSetupLayout *passwordVerify;
	KrateSetupPage* krateSetup;

	klay::NaviFrame *naviframe;
	int setupMode;
};

class SetupWizard : public klay::Application {
private:
	SetupWizard();
	static SetupWizard *setupWizard;
public:
	virtual ~SetupWizard();

	bool onCreate();
	void onSuspend();
	void onTerminate();
	void onResume();
	void onAppControl(klay::AppControl* appControl);

	void krateCreate();
	void krateRemove();
	void createNotification();

	static SetupWizard* instance();

private:
	void addKrateEventHandler();
	static void krateEventDispatcher(const char *from, const char *info, void *data);
	std::function<void()> krateEventHandler;
	std::string getKrateMetaData() const;

private:
	SetupWizardMainFrame* mainframe;
	zone_manager_h krateManager;
	klay::Notification *notification;

	int krateEventId;
	std::string mode;
	std::string name;

	const std::string title[2] = {
		"Separated zone creation",
		"Separated Zone deletion"
	};
	const std::string content[2] = {
		"Tap heare to create Separated Zone.",
		"Tap here to delete Separated Zone."
	};
};
