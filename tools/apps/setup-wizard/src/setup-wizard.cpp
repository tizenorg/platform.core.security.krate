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

#include "setup-wizard.h"

using namespace klay;

const std::string EDJE_FILE = "/usr/apps/org.tizen.krate-setup-wizard/res/org.tizen.krate-setup-wizard.edj";

PasswordManager::PasswordManager()
{
}

PasswordManager::~PasswordManager()
{
}

bool PasswordManager::verify(const std::string &pwd)
{
	if (password == pwd) {
		return true;
	}
	return false;
}

void PasswordManager::setPassword(const std::string& pwd)
{
	password = pwd;
}

SecuritySetupPage::SecuritySetupPage(Widget *parent) :
	Navigator(parent, EDJE_FILE, "base_layout", "security-setup")
{
	createPartContents();
}

void SecuritySetupPage::createPartContents()
{
	class ContentProvider : public GenlistContentProvider {
	public:
		char *getLabel(const std::string& name, const std::string& part)
		{
			if ((name == "guide") && (part == "elm.text.multiline")) {
				return strdup("Select a Krate unlock method and a timeout option.");
			} else if ((name == "unlock-method-group") && (part == "elm.text")) {
				return strdup("Unlock method");
			} else if ((name == "unlock-method-password") && (part == "elm.text")) {
				return strdup("Password");
			} else if ((name == "unlock-method-pin") && (part == "elm.text")) {
				return strdup("PIN");
			} else if ((name == "security-options-group") && (part == "elm.text")) {
				return strdup("Security options");
			} else if ((name == "security-timeout") && (part == "elm.text")) {
				return strdup("Security Timeout");
			} else if ((name == "security-timeout") && (part == "elm.text.sub")) {
				return strdup("After %d minuates of inactivity");
			}

			return nullptr;
		}

		Widget *getWidget(Genlist* parent, const std::string& name, const std::string& part)
		{
			if (group == nullptr)
				group = new Radio(parent, NULL, 0);
			if ((name == "unlock-method-password") && (part == "elm.swallow.icon")) {
				return new Radio(parent, group, 0);
			} else if ((name == "unlock-method-pin") && (part == "elm.swallow.icon")) {
				return new Radio(parent, group, 1);
			}
			return nullptr;
		}
	private:
		Radio* group;
	};

	Genlist *genlist = new Genlist(this, new ContentProvider());

	genlist->append("guide", "multiline");
	genlist->append("unlock-method-group", "group_index");
	genlist->append("unlock-method-password", "one_icon");
	genlist->append("unlock-method-pin", "one_icon").setDisabled(true);
	genlist->append("security-options-group", "group_index");
	genlist->append("security-timeout", "double_label").setDisabled(true);

	setContent(genlist, "content_layout");

	Button *prevButton = new Button(this, "Previous");
	prevButton->onClick = [this]() {
		emitSignal("previous", "security-setup");
	};

	Button *nextButton = new Button(this, "Next");
	nextButton->onClick = [this]() {
		emitSignal("next", "security-setup");
	};

	setContent(prevButton, "prev_button");
	setContent(nextButton, "next_button");
}

PasswordSetupLayout::PasswordSetupLayout(Widget *parent, PasswordManager *manager, const std::string& name) :
    Navigator(parent, EDJE_FILE, "base_layout", name), page(name)
{
	pwdManager = manager;
	createPartContents();
}

char *PasswordSetupLayout::getLabel(const std::string& name, const std::string& part)
{
	return nullptr;
}

Widget *PasswordSetupLayout::getWidget(Genlist* parent, const std::string& name, const std::string& part)
{
	if ((name == "password-input")) {
		entryLayout = new Layout(parent, EDJE_FILE, "security_layout");
		entryLayout->setText("Enter Password", "title");

		entry = new Entry(entryLayout,
			Entry::SetSingleLine | Entry::SetPassword | Entry::SetReturnKeyDisabled);
		entry->setFocus(true);

		entry->onChanged = [this]() {
			const std::string entry_data = entry->get();
			if (entry_data.length() > 1) {
				nextButton->setDisabled(false);
			} else {
				nextButton->setDisabled(true);
			}
		};

		entry->onUnfocused = [this]() {
			if (page == "password-setup") {
				pwdManager->setPassword(entry->get());
			}
		};

		entryLayout->setContent(entry, "entry");
		return entryLayout;
	}
	return nullptr;
}

void PasswordSetupLayout::createPartContents()
{
	Genlist *genlist = new Genlist(this, this);
	genlist->append("password-input", "full");

	setContent(genlist, "content_layout");

	Button *prevButton = new Button(this, "Previous");
	prevButton->onClick = [this]() {
		entry->set("");
		if (page == "password-setup") {
			nextButton->setDisabled(true);
			emitSignal("previous", "password-setup");
		} else {
			emitSignal("previous", "password-verify");
		}
	};

	nextButton = new Button(this, "Create");
	nextButton->setDisabled(true);
	nextButton->onClick = [this](){
		if (page == "password-setup") {
			emitSignal("next", "password-setup");
		} else {
			if (pwdManager->verify(entry->get())){
				emitSignal("next", "password-verify");
			} else {
				entryLayout->setText("Password do not matched.", "entry_info");
				entry->set("");
			}
		}
	};

	setContent(prevButton, "prev_button");
	setContent(nextButton, "next_button");
}

KrateSetupPage::KrateSetupPage(Widget *parent) :
	Navigator(parent, EDJE_FILE, "setup_layout", "krate-setup")
{
	createPartContents();
}

void KrateSetupPage::createPartContents()
{
	setText("Creating Folder instance...", "progressbar_msg");
	setText("The folder will be created on your<br>presonal home screen.", "content_text");
	progressbar = new Progressbar(this, Progressbar::SetPulse);
	progressbar->setStyle("pending");
	setContent(progressbar, "progressbar");
}

bool KrateSetupPage::timerEvent()
{
	double time = progressbar->getValue();
	if (time == 1.0) {
		ui_app_exit();
		return ECORE_CALLBACK_CANCEL;
	}
	progressbar->setValue(time+=0.1);
	return ECORE_CALLBACK_RENEW;
}

WelcomePage::WelcomePage(Widget* parent) :
	Navigator(parent, EDJE_FILE, "welcome_layout", "welcome"), text(textList[0])
{
	createPartContents();
}

void WelcomePage::createPartContents()
{
	setText(text[0], "message_title");
	setText(text[1], "message_content");

	Button *cancelButton = new Button(this, text[2]);
	cancelButton->onClick = [this]() {
		emitSignal("previous", "welcome");
	};

	Button *nextButton = new Button(this, text[3]);
	nextButton->onClick = [this]() {
		emitSignal("next", "welcome");
	};

	setContent(cancelButton, "prev_button");
	setContent(nextButton, "next_button");
}

SetupWizardMainFrame::SetupWizardMainFrame() :
	ApplicationWindow("ZoneSetup", SetIndicatorShow)
{
	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizardMainFrame()\n");
}

void SetupWizardMainFrame::showWelcomePage()
{
	naviframe->push(welcome);
}

void SetupWizardMainFrame::showSecuritySetupPage()
{
	naviframe->push(securitySetup, "Krate Security");
}

void SetupWizardMainFrame::showPasswordSetupPage()
{
	naviframe->push(passwordSetup, "Password Setup");
}

void SetupWizardMainFrame::showPasswordVerifyPage()
{
	naviframe->push(passwordVerify, "Password Verify");
}

void SetupWizardMainFrame::showKrateSetupPage()
{
	indicatorOverlap(true);
	naviframe->push(krateSetup);
	Timer timer(krateSetup, 0.1);
}

bool SetupWizardMainFrame::createPartContents(Layout *layout)
{
	naviframe = new NaviFrame(layout);
	layout->setContent(naviframe);

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizardMainFrame::createPartContents()\n");

	welcome = new WelcomePage(naviframe);
	welcome->previous = [this]() {
		ui_app_exit();
	};

	welcome->next = [this]() {
		indicatorOverlap(false);
		showSecuritySetupPage();
	};

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "WelcomePage created\n");

	securitySetup = new SecuritySetupPage(naviframe);
	securitySetup->previous = [this]() {
		indicatorOverlap(true);
		naviframe->pop();
	};
	securitySetup->next  = [this]() {
		showPasswordSetupPage();
	};

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SecuritySetupPage created\n");

	passwordSetup = new PasswordSetupLayout(naviframe, this, "password-setup");
	passwordSetup->previous = [this]() {
		naviframe->pop();
	};
	passwordSetup->next = [this]() {
		showPasswordVerifyPage();
	};

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "PasswordSetupPage created\n");

	passwordVerify = new PasswordSetupLayout(naviframe, this, "password-verify");
	passwordVerify->previous = [this]() {
		naviframe->pop();
	};
	passwordVerify->next = [this]() {
		indicatorOverlap(true);
		showKrateSetupPage();
	};

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "PasswordVerifyPage created\n");

	krateSetup = new KrateSetupPage(naviframe);

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "KrateSetupPage created\n");

	indicatorOverlap(true);
	showWelcomePage();

	return true;
}
