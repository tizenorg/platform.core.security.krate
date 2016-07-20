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

SetupWizard::SetupWizard()
{
	krateEventHandler = []() {
	};
}

SetupWizard::~SetupWizard()
{
}

SetupWizard* SetupWizard::instance()
{
	if (setupWizard == nullptr) {
		setupWizard = new SetupWizard();
	}
	return setupWizard;
}

bool SetupWizard::onCreate()
{
	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizard::onCreate()\n");
	if (zone_manager_create(&krateManager) != ZONE_ERROR_NONE) {
		ui_app_exit();
		return false;
	}
	return true;
}

void SetupWizard::onSuspend()
{
	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizard::onSuspend()\n");
}

void SetupWizard::onTerminate()
{
	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizard::onTerminate()\n");
}

void SetupWizard::onResume()
{
	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizard::onResume()\n");
}

void SetupWizard::onAppControl(klay::AppControl* appControl)
{
	dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "SetupWizard::onAppcontrol()");

	if ((mode = appControl->getData("mode")) == "") {
		ui_app_exit();
	}
	if ((name = appControl->getData("zone")) == "") {
		ui_app_exit();
	}

	addKrateEventHandler();

	klay::Display::setBaseScale(1.0);

	if (mode == "create") {
		notification = new klay::Notification(title[0], content[0], "", appControl);
		mainframe = new SetupWizardMainFrame(SetupWizardMainFrame::CREATE_MODE);
	} else {
		notification = new klay::Notification(title[1], content[1], "", appControl);
		mainframe = new SetupWizardMainFrame(SetupWizardMainFrame::REMOVE_MODE);
	}

	krateEventHandler = [this]() {
		if (mode == "create") {
			dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "EVENT HANDLER: CREATE MODE!");
		}
		mainframe->emitSignal("request-done", "mainframe");
	};
	mainframe->show();
}

void SetupWizard::createNotification()
{
	notification->post();
}

void SetupWizard::krateCreate()
{
	std::string metaData = getKrateMetaData();
	if (zone_manager_create_zone(krateManager, name.c_str(), metaData.c_str()) != ZONE_ERROR_NONE) {
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "Failed to create krate");
		ui_app_exit();
	}
}

void SetupWizard::krateRemove()
{
	if (zone_manager_destroy_zone(krateManager, name.c_str()) != ZONE_ERROR_NONE) {
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "Failed to remove krate");
		ui_app_exit();
	}
}

void SetupWizard::addKrateEventHandler()
{
	std::string cb_event;
	if (mode == "create") {
		cb_event = "created";
	} else {
		cb_event = "removed";
	}

	if (zone_manager_add_event_cb(krateManager, cb_event.c_str(), krateEventDispatcher,
				reinterpret_cast<void*>(this), &krateEventId) != ZONE_ERROR_NONE) {
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "Failed to add krate signal callback.");
		ui_app_exit();
	}
}

void SetupWizard::krateEventDispatcher(const char *from, const char *info, void *data)
{
	SetupWizard *instance = reinterpret_cast<SetupWizard *>(data);
	if (!strcmp(info, "Error")) {
		dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "Krate request error");
		return;
	}
	instance->krateEventHandler();
}

std::string SetupWizard::getKrateMetaData() const
{
	char c;
	std::string metaData;
	std::string path = app_get_resource_path();
	path.append("data/KrateManifest.xml");

	std::ifstream file(path);
	if (file.is_open() == false) {
		return nullptr;
	}

	while (file.get(c)) {
		metaData.push_back(c);
	}
	file.close();
	return metaData;
}

SetupWizard *SetupWizard::setupWizard = SetupWizard::instance();
