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
#include <stdio.h>

#include <iostream>

#include <app.h>
#include <app_common.h>
#include <bundle.h>

#include "application.h"

struct RuntimeThread {
	Application* application;
};

RuntimeThread runtimeThread;

Application* ApplicationInstance()
{
	return runtimeThread.application;
}

Application::Application()
{
	runtimeThread.application = this;
}

Application::~Application()
{
}

bool Application::onCreateInternal(void* data)
{
	Application *instance = (Application *)data;
	return instance->onCreate();
}

void Application::onResumeInternal(void* data)
{
	Application *instance = (Application *)data;
	instance->onResume();
}

void Application::onPauseInternal(void* data)
{
	Application *instance = (Application *)data;
	instance->onPause();
}

void Application::onTerminateInternal(void* data)
{
	Application *instance = (Application *)data;
	instance->onTerminate();
}

int main(int argc, char *argv[])
{
	ui_app_lifecycle_callback_s eventCallback = {0, };

	eventCallback.create    = Application::onCreateInternal;
	eventCallback.terminate = Application::onTerminateInternal;
	eventCallback.pause     = Application::onPauseInternal;
	eventCallback.resume    = Application::onResumeInternal;

	int ret = ui_app_main(argc, argv, &eventCallback, runtimeThread.application);
	if (ret != APP_ERROR_NONE) {
		std::cerr << "ui_app_main is failed. err = " <<  ret << std::endl;
	}

	return ret;
}
