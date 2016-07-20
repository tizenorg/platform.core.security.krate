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

class LabelProvider : public GenlistLabelProvider {
public:
	char *getLabel(const std::string& name, const std::string& part)
	{
		if (name.compare("guide") == 0) {
			if (part.compare("elm.text.multiline") == 0)
				return strdup(SECURITY_UNLOCK_METHOD_INFO_TEXT);
			return NULL;
		}

		if (name.compare("unlock-method-group") == 0) {
			if (part.compare("elm.text") == 0)
				return strdup(SECURITY_GROUP_UNLOCK_METHOD);
			return NULL;
		}

		if (name.compare("unlock-method") == 0) {
			if (part.compare("elm.text") == 0)
				return strdup(unlockMethod[0].c_str());
			return NULL;
		}

		if (name.compare("security-options-group") == 0) {
			if (part.compare("elm.text") == 0)
				return strdup(SECURITY_GROUP_SECURITY_OPTIONS);
			return NULL;
		}

		if (name.compare("security-timeout") == 0) {
			if (part.compare("elm.text") == 0)
				return strdup(SECURITY_TIMEOUT_TEXT);
			else if (part.compare("elm.text.sub") == 0)
				return strdup(SECURITY_TIMEOUT_SUB_TEXT);
			return NULL;
		}
		return NULL;
	}

private:
	const std::string unlockMethod[2] = {SECURITY_PASSWORD, SECURITY_SIMPLE_PASSWORD};
};

class ContentProvider : public GenlistContentProvider {
public:
	Widget *getContent(Genlist* parent, const std::string& name, const std::string& part)
	{
		if (name.compare("unlock-method") == 0) {
			if (group == NULL) {
				group = new Radio(parent, NULL, 0);
			}
			if (part.compare("elm.swallow.icon") == 0) {
				Radio *radio = new Radio(parent, group, 0);
				return radio;
			}
			return NULL;
		}
		if (name.compare("password-input") == 0) {
			if (part.compare("elm.swallow.content") == 0) {
				Layout *entryLayout = new Layout(parent, EDJE_FILE, "security_layout");
				entryLayout->setText(SECURITY_ENTER_PASSWORD, "title");

				Entry *entry = new Entry(entryLayout);
				entry->setSingleLine(true);
				entry->setPassword(true);

				entryLayout->setContent(entry, "entry");
				return entryLayout;
			}
			return NULL;
		}
		return NULL;
	}

private:
	Widget* parent;
	Radio* group;
};

class NaviButtonLayout : public Layout {
public:
	NaviButtonLayout(Widget* parent) :
		Layout(parent, EDJE_FILE, "two_button_layout")
	{
	}
};

SecuritySetupPage::SecuritySetupPage(Widget *parent) :
	Layout(parent, EDJE_FILE, "base_layout")
{
	createPartContents();
}

void SecuritySetupPage::createPartContents()
{
	class SecuritySetupLabelProvider : public GenlistLabelProvider {
	public:
		char *getLabel(const std::string& name, const std::string& part)
		{
			if ((name == "guide") && (part == "elm.text.multiline")) {
				return strdup(SECURITY_UNLOCK_METHOD_INFO_TEXT);
			} else if ((name == "unlock-method-group") && (part == "elm.text")) {
				return strdup(SECURITY_GROUP_UNLOCK_METHOD);
			} else if ((name == "unlock-method-password") && (part == "elm.text")) {
				return strdup("Password");
			} else if ((name == "unlock-method-pin") && (part == "elm.text")) {
				return strdup("PIN");
			} else if ((name == "security-options-group") && (part == "elm.text")) {
				return strdup(SECURITY_GROUP_SECURITY_OPTIONS);
			} else if ((name == "security-timeout") && (part == "elm.text")) {
				return strdup(SECURITY_TIMEOUT_TEXT);
			} else if ((name == "security-timeout") && (part == "elm.text.sub")) {
				return strdup(SECURITY_TIMEOUT_SUB_TEXT);
			}

			return NULL;
		}
	};

	class SecuritySetupContentProvider : public GenlistContentProvider {
	public:
		Widget *getContent(Genlist* parent, const std::string& name, const std::string& part)
		{
			#if 0
			if ((name == "unlock-method-password")) {
				return (group = new Radio(parent, NULL, 0));
			} else if ((name == "unlock-method-pin") && (part == "elm.swallow.icon")) {
				return new Radio(parent, group, 1);
			}
			#endif
			return NULL;
		}

	private:
		Radio* group;
	};

	Genlist *genlist = new Genlist(this);
	genlist->setLabelProvider(new SecuritySetupLabelProvider());
	genlist->setContentProvider(new SecuritySetupContentProvider());

	genlist->append("guide",                  "multiline",    ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
	genlist->append("unlock-method-group",    "group_index",  ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
	genlist->append("unlock-method-password", "one_icon",     ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY).setDisabled(false);
	genlist->append("unlock-method-pin",      "one_icon",     ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY).setDisabled(true);
	genlist->append("security-options-group", "group_index",  ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
	genlist->append("security-timeout",       "double_label", ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY).setDisabled(true);

	setContent(genlist, "content_layout");

	Layout *navibarLayout = new NaviButtonLayout(this);
	Button *prevButton = new Button(navibarLayout, "Previous");
	prevButton->addClickHandler([this](const std::string& emission, const std::string& source) {
		elm_object_signal_emit(nativeObject, "previous", "security-setup");
	});

	Button *nextButton = new Button(navibarLayout, "Next");
	nextButton->addClickHandler([this](const std::string& emission, const std::string& source) {
		elm_object_signal_emit(nativeObject, "next", "security-setup");
	});

	navibarLayout->setContent(prevButton, "prev_button");
	navibarLayout->setContent(nextButton, "next_button");

	setContent(navibarLayout, "bottom_layout");
}

bool SecuritySetupPage::handleNaviItemEvent()
{
	return true;
}

PasswordSetupPage::PasswordSetupPage(Widget *parent) :
    Layout(parent, EDJE_FILE, "base_layout")
{
	createPartContents();
}

void PasswordSetupPage::createPartContents()
{
	Genlist *genlist = new Genlist(this);
	genlist->append("password-input", "full", ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

	setContent(genlist, "content_layout");

	Layout *twoButtonLayout = new Layout(this, EDJE_FILE, "two_button_layout");
	Button *prevButton = new Button(twoButtonLayout, "Previous");
	prevButton->addClickHandler([this](const std::string& emission, const std::string& source) {
		elm_object_signal_emit(nativeObject, "previous", "password-setup");
	});

	Button *nextButton = new Button(twoButtonLayout, "Create");
	nextButton->addClickHandler([this](const std::string& emission, const std::string& source){
		elm_object_signal_emit(nativeObject, "next", "password-setup");
	});

	twoButtonLayout->setContent(prevButton, "prev_button");
	twoButtonLayout->setContent(nextButton, "next_button");
	setContent(twoButtonLayout, "bottom_layout");
}

bool PasswordSetupPage::handleNaviItemEvent()
{
	return true;
}

KrateSetupPage::KrateSetupPage(Widget *parent) :
	Layout(parent, EDJE_FILE, "setup_layout")
{
	createPartContents();
}

void KrateSetupPage::createPartContents()
{
	setText("Creating Folder instance...", "progressbar_msg");
	setText("The folder will be created on your<br>presonal home screen.", "content_text");
	progressbar = new Progressbar(this);
	progressbar->setStyle("pending");
	setContent(progressbar, "progressbar");
}

void KrateSetupPage::addTimer()
{
	progressbar->addTimer(this, 0.1);
}

bool KrateSetupPage::handleNaviItemEvent()
{
	return true;
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
	Layout(parent, EDJE_FILE, "welcome_layout"), selectedText(text[0])
{
	createPartContents();
}

void WelcomePage::createPartContents()
{
	setText(selectedText[0], "message_title");
	setText(selectedText[1], "message_content");

	Layout *twoButtonLayout = new Layout(this, EDJE_FILE, "two_button_layout");

	Button *cancelButton = new Button(twoButtonLayout, selectedText[2]);
	cancelButton->addClickHandler([this](const std::string & emission, const std::string & source) {
		handleNaviItemEvent();
	});

	Button *nextButton = new Button(twoButtonLayout, selectedText[3]);
	nextButton->addClickHandler([this](const std::string & emission, const std::string & source) {
		elm_object_signal_emit(nativeObject, "next", "welcome");
	});

	twoButtonLayout->setContent(cancelButton, "prev_button");
	twoButtonLayout->setContent(nextButton, "next_button");

	setContent(twoButtonLayout, "bottom_layout");
}

bool WelcomePage::handleNaviItemEvent()
{
	ui_app_exit();
	return true;
}

SetupWizardMainFrame::SetupWizardMainFrame() :
	ApplicationWindow("ZoneSetup", SetIndicatorShow)
{
	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizardMainFrame()\n");
}

void SetupWizardMainFrame::showWelcomePage()
{
	NaviItem item = naviframe->push(welcome);
	item.setEventListener(welcome);
}

void SetupWizardMainFrame::showSecuritySetupPage()
{
	NaviItem item = naviframe->push(securitySetup, "Krate Security");
	item.setEventListener(securitySetup);
}

void SetupWizardMainFrame::showPasswordSetupPage()
{
	NaviItem item = naviframe->push(passwordSetup, "Password Setup");
	item.setEventListener(passwordSetup);
}

void SetupWizardMainFrame::showKrateSetupPage()
{
	indicatorOverlap(true);
	NaviItem item = naviframe->push(passwordSetup);
	krateSetup->addTimer();
	item.setEventListener(krateSetup);
}

bool SetupWizardMainFrame::createPartContents(Layout *layout)
{
	naviframe = new NaviFrame(layout);
	layout->setContent(naviframe);

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizardMainFrame::createPartContents()\n");

	welcome = new WelcomePage(naviframe);
	welcome->addSignalHandler("next", "welcome", [this](const std::string & emission, const std::string & source) {
		indicatorOverlap(false);
		showSecuritySetupPage();
	});

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "WelcomePage created\n");

	securitySetup = new SecuritySetupPage(naviframe);
	securitySetup->addSignalHandler("previous", "security-setup", [this](const std::string &emission, const std::string& source) {
		naviframe->pop();
	});

	securitySetup->addSignalHandler("next", "security-setup", [this](const std::string &emission, const std::string& source) {
		showPasswordSetupPage();
	});

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SecuritySetupPage created\n");

#if 0
	passwordSetup = new PasswordSetupPage(naviframe);
	passwordSetup->addSignalHandler("previous", "password-setup", [this](const std::string& emission, const std::string& source) {
		naviframe->pop();
	});

	passwordSetup->addSignalHandler("next", "password-setup", [this](const std::string& emission, const std::string& source){
		showKrateSetupPage();
	});

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "PasswordSetupPage created\n");

	krateSetup = new KrateSetupPage(naviframe);

	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "KrateSetupPage created\n");
#endif
	indicatorOverlap(true);
	showWelcomePage();

	return true;
}
