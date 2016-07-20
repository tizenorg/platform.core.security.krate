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

class SecuritySetupPage : public Layout, public NaviItemEventListener {
public:
	SecuritySetupPage(Widget *parent) :
		Layout(parent, EDJE_FILE, "base_layout")
	{
		createPartContents();
	}

	void createPartContents()
	{
	}

	bool handleNaviItemEvent()
	{
		return true;
	}
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

	welcome->addSignalHandler("next", "welcome", [this, securitySetup](const std::string & emission, const std::string & source) {
		indicatorOverlap(false);
		NaviItem item = naviframe->push(securitySetup, "Krate Security");
		item.setEventListener(securitySetup);
	});

	indicatorOverlap(true);
	NaviItem item = naviframe->push(welcome);
	item.setEventListener(welcome);

	return true;
}
