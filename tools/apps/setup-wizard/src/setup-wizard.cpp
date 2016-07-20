#include "setup-wizard.h"

const std::string EDJE_FILE = "/usr/apps/org.tizen.krate-setup-wizard/res/org.tizen.krate-setup-wizard.edj";

class WelcomePage : public Layout, public NaviItemEventListener {
public:
	WelcomePage(Widget* parent, int mode) :
		Layout(parent, EDJE_FILE, "welcome_layout"), selectedText(text[mode])
	{
		createPartContents();
	}

	void createPartContents()
	{
		setText(selectedText[0], "message_title");
		setText(selectedText[1], "message_content");

		Button *cancelButton = new Button(this, selectedText[2]);
		cancelButton->addClickHandler([this](const std::string & emission, const std::string & source) {
			handleNaviItemEvent();
		});

		Button *nextButton = new Button(this, selectedText[3]);
		nextButton->addClickHandler([this](const std::string & emission, const std::string & source) {
			elm_object_signal_emit(nativeObject, "next", "welcome");
		});

		setContent(cancelButton, "prev_button");
		setContent(nextButton, "next_button");
	}

	bool handleNaviItemEvent()
	{
		ui_app_exit();
		return true;
	}
private:
	const std::string text[2][4] = {
		{CREATE_MESSAGE_TITLE, CREATE_MESSAGE_CONTENT, CANCEL_BUTTON, NEXT_BUTTON},
		{REMOVE_MESSAGE_TITLE, REMOVE_MESSAGE_CONTENT, CANCEL_BUTTON, REMOVE_BUTTON}
	};
	const std::string *selectedText;
};

class LabelProvider : public GenlistLabelProvider {
public:
	LabelProvider() {}
	std::string getLabel(const std::string& name, const std::string& part, int index) {
		if (name.compare("guide") == 0) {
			if (part.compare("elm.text.multiline") == 0)
				return SECURITY_UNLOCK_METHOD_INFO_TEXT;
			return NULL;
		}

		if (name.compare("unlock-method-group") == 0) {
			if (part.compare("elm.text") == 0)
				return SECURITY_GROUP_UNLOCK_METHOD;
			return NULL;
		}

		if (name.compare("unlock-method") == 0) {
			if (part.compare("elm.text") == 0)
				return unlockMethod[index];
			return NULL;
		}

		if (name.compare("security-options-group") == 0) {
			if (part.compare("elm.text") == 0)
				return SECURITY_GROUP_SECURITY_OPTIONS;
			return NULL;
		}

		if (name.compare("security-timeout") == 0) {
			if (part.compare("elm.text") == 0)
				return SECURITY_TIMEOUT_TEXT;
			else if (part.compare("elm.text.sub") == 0)
				return SECURITY_TIMEOUT_SUB_TEXT;
			return NULL;
		}
		return NULL;
	}
private:
	const std::string unlockMethod[2] = {SECURITY_PASSWORD, SECURITY_SIMPLE_PASSWORD};
};

class ContentProvider : public Widget, public GenlistContentProvider {
public:
	ContentProvider(Widget *object) : parent(object) {
	}

	Widget *getContent(const std::string& name, const std::string& part, int index) {
		if (name.compare("unlock-method") == 0) {
			if (group == NULL) {
				group = new Radio(parent, NULL, 0);
			}
			if (part.compare("elm.swallow.icon") == 0) {
				Radio *radio = new Radio(parent, group, index);
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

class SecuritySetupPage : public Layout, public NaviItemEventListener {
public:
	SecuritySetupPage(Widget *parent) :
		Layout(parent, EDJE_FILE, "base_layout")
	{
		createPartContents();
	}

	void createPartContents()
	{
		
		/*Genlist<LabelProvider,ContentProvider> *genlist = new Genlist<LabelProvider,ContentProvider>(this);
		genlist->append("guide", "multiline", ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, 0);
		genlist->append("unlock-method-group", "group_index", ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, 0);
		for (int index = 0; index < 2; index++) {
			GenlistItem item = genlist->append("unlock-method", "one_icon", ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, index);
			if (index == 1)
				item.setDisabled(true);
		}

		genlist->append("security-options-group", "group_index", ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, 0);
		GenlistItem item = genlist->append("security-timeout", "double_label", ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, 0);
		item.setDisabled(true);

		setContent(genlist, "content_layout");*/
		Button *prevButton = new Button(this, "Previous");
		prevButton->addClickHandler([this](const std::string& emission, const std::string& source) {
			elm_object_signal_emit(nativeObject, "previous", "security-setup");
		});
		Button *nextButton = new Button(this, "Next");
		nextButton->addClickHandler([this](const std::string& emission, const std::string& source) {
			elm_object_signal_emit(nativeObject, "next", "security-setup");
		});

		setContent(prevButton, "prev_button");
		setContent(nextButton, "next_button");
	}

	bool handleNaviItemEvent()
	{
		return true;
	}
};

class PasswordSetupPage : public Layout, public NaviItemEventListener {
public:
	PasswordSetupPage(Widget *parent) :
                Layout(parent, EDJE_FILE, "base_layout")
	{
		createPartContents();
	}

	void createPartContents()
	{
		/*Genlist<LabelProvider, ContentProvider> *genlist = new Genlist<LabelProvider, ContentProvider>(this);
		genlist->append("password-input", "full", ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, 0);

		setContent(genlist, "content_layout");*/

		Button *prevButton = new Button(this, "Previous");
		prevButton->addClickHandler([this](const std::string& emission, const std::string& source) {
			elm_object_signal_emit(nativeObject, "previous", "password-setup");
		});

		Button *nextButton = new Button(this, "Create");
		nextButton->addClickHandler([this](const std::string& emission, const std::string& source){
			elm_object_signal_emit(nativeObject, "next", "password-setup");
		});

		setContent(prevButton, "prev_button");
		setContent(nextButton, "next_button");
	}

	bool handleNaviItemEvent()
	{
		return true;
	}
};

class KrateSetupPage: public Layout, public NaviItemEventListener, public TimerEventListener{
public:
	KrateSetupPage(Widget *parent) :
		Layout(parent, EDJE_FILE, "setup_layout")
	{
		createPartContents();
	}

	void createPartContents()
	{
		setText("Creating Folder instance...", "progressbar_msg");
		setText("The folder will be created on your<br>presonal home screen.", "content_text");
		progressbar = new Progressbar(this);
		progressbar->setStyle("pending");
		setContent(progressbar, "progressbar");		
	}

	void addTimer()
	{
		progressbar->addTimer(this, 0.1);
	}

	bool handleNaviItemEvent()
	{
		return true;
	}

	bool timerEvent()
	{
		double time = progressbar->getValue();
		if (time == 1.0) {
			ui_app_exit();
			return ECORE_CALLBACK_CANCEL;
		}
		progressbar->setValue(time+=0.1);
		return ECORE_CALLBACK_RENEW;
	}
private:
	Progressbar *progressbar;
};

SetupWizardMainFrame::SetupWizardMainFrame()
{
	create("ZoneSetup", ELM_WIN_BASIC, SetIndicatorShow | SetConformant | SetAutodel | SetBackExit);
}

bool SetupWizardMainFrame::createPartContents(Layout *layout)
{
	naviframe = new NaviFrame(layout);
	layout->setContent(naviframe);

	WelcomePage* welcome = new WelcomePage(naviframe, 0);
	SecuritySetupPage* securitySetup = new SecuritySetupPage(naviframe);
	PasswordSetupPage* passwordSetup = new PasswordSetupPage(naviframe);
	KrateSetupPage* krateSetup = new KrateSetupPage(naviframe);

	welcome->addSignalHandler("next", "welcome", [this, securitySetup](const std::string & emission, const std::string & source) {
		indicatorOverlap(false);
		NaviItem item = naviframe->push(securitySetup, "Krate Security");
		item.setEventListener(securitySetup);
	});

	securitySetup->addSignalHandler("previous", "security-setup", [this](const std::string &emission, const std::string& source) {
		naviframe->pop();
	});
	securitySetup->addSignalHandler("next", "security-setup", [this, passwordSetup](const std::string &emission, const std::string& source){
		NaviItem item = naviframe->push(passwordSetup, "Password Setup");
		item.setEventListener(passwordSetup);
	});

	passwordSetup->addSignalHandler("previous", "password-setup", [this](const std::string& emission, const std::string& source) {
		naviframe->pop();
	});
	passwordSetup->addSignalHandler("next", "password-setup", [this, krateSetup](const std::string& emission, const std::string& source){
		indicatorOverlap(true);
		NaviItem item = naviframe->push(krateSetup);
		krateSetup->addTimer();
		item.setEventListener(krateSetup);
	});

	indicatorOverlap(true);
	NaviItem item = naviframe->push(welcome);
	item.setEventListener(welcome);

	return true;
}
