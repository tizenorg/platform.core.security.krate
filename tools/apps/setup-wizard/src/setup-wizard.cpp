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

	welcomeLayout = new Layout(naviframe, EDJE_FILE, "welcome_layout");
	naviframe->push(welcomeLayout);
	naviframe->addPopCallback(onDelete);

	return true;
}
