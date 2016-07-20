#include "setup-wizard.h"

const std::string EDJE_FILE = "/usr/apps/org.tizen.krate-setup-wizard/res/org.tizen.krate-setup-wizard.edj";

class WelcomePage : public Layout, public NaviItemEventListener {
public:
	WelcomePage(Widget* parent) :
		Layout(parent, EDJE_FILE, "base_layout")
	{
		createPartContents();
	}

	void createPartContents()
	{
		Button* button = new Button(this, "Next");
		button->addSignalHandler("mouse,clicked,1", "event", [this](const std::string& emission, const std::string& source) {
			elm_object_signal_emit(nativeObject, "next", "welcome");
		});

		setContent(button, "bottom_layout");
	}

	bool handleNaviItemEvent()
	{
		ui_app_exit();
		return true;
	}
};

class SecuritySetupPage : public Layout, public NaviItemEventListener {
public:
	SecuritySetupPage(Widget* parent) :
		Layout(parent, EDJE_FILE, "security_layout")
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

bool SetupWizardMainFrame::createPartContents(Layout* layout)
{
	naviframe = new NaviFrame(layout);
	layout->setContent(naviframe);

	WelcomePage* welcome = new WelcomePage(naviframe);
	SecuritySetupPage* securitySetup = new SecuritySetupPage(naviframe);

	welcome->addSignalHandler("next", "welcome", [this, securitySetup](const std::string& emission, const std::string& source) {
		NaviItem item = naviframe->push(securitySetup);
		item.setEventListener(securitySetup);
	});

	NaviItem item = naviframe->push(welcome);
	item.setEventListener(welcome);

	return true;
}
