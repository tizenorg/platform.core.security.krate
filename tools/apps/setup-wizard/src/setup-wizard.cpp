#include "setup-wizard.h"

void SetupWizardMainFrame::onDelete(void *data)
{
	ui_app_exit();
}

SetupWizardMainFrame::SetupWizardMainFrame()
{
	create(std::string("Zone Setup Wizard"), ELM_WIN_BASIC,
		   SetConformant | SetAutodel | SetIndicatorShow | SetBackExit);
}

bool SetupWizardMainFrame::createPartContents(Layout* layout)
{
	naviframe = new Naviframe(layout);
	layout->setContent(naviframe);

	welcomeframe = new SetupWizardWelcomeFrame(naviframe);
	naviframe->push(welcomeframe);
	naviframe->addPopCallback(onDelete);

	return true;
}

SetupWizardWelcomeFrame::SetupWizardWelcomeFrame(Widget *parent) :
	SetupWizardBaseLayout(parent)
{
	welcomeLayout = new Layout(this, EDJE_FILE, "welcome_layout");
	setContentLayout(welcomeLayout);

	twoButtonLayout = new Layout(this, EDJE_FILE, "two_button_layout");

	cancel_button = new Button(twoButtonLayout, "Cancel");
	twoButtonLayout->setContent(cancel_button, "prev_button");

	next_button = new Button(twoButtonLayout, "Next");
	twoButtonLayout->setContent(next_button, "next_button");

	setBottomLayout(twoButtonLayout);
}
