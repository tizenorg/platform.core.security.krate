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
 *
 */
//#include "krate-setup.h"
#include "common/widget.h"

#include "common/debug.h"
#include "common/application.h"
#include "common/shell.h"
#include "setup-wizard.h"

class SetupWizard : public Application {
public:
	bool onCreate()
	{
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizard::onCreate()\n");
		Display::setBaseScale(1.8);
		mainframe = new SetupWizardMainFrame();
		mainframe->show();
		return true;
	}

	void onSuspend()
	{
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizard::onSuspend()\n");
	}

	void onTerminate()
	{
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizard::onTerminate()\n");
	}

	void onResume()
	{
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizard::onResume()\n");
	}

private:
	SetupWizardMainFrame* mainframe;
};

SetupWizard setupWizard;
