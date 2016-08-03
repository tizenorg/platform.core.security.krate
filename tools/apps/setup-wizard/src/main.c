/*
 * Tizen Krate Setup-Wizard application
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
#include "krate-setup.h"
#include "widget.h"

static void __launch_krate_app(krate_manager_h krate_mgr, const char *krate_name, app_control_h app_control)
{
	krate_app_proxy_h krate_app;

	krate_app_proxy_create(krate_mgr, krate_name, &krate_app);
	krate_app_proxy_send_launch_request(krate_app, app_control);
	krate_app_proxy_destroy(krate_app);
}

static void __krate_request_done(const char *from, const char *info, void *user_data)
{
	app_control_h app_control;
	char uri[PATH_MAX];

	appdata_s *ad = (appdata_s *) user_data;
	if (!strcmp(info, "Error")) {
		ecore_main_loop_thread_safe_call_sync(krate_request_fail, ad);
		return;
	}

	if (!strcmp(ad->mode, "create")) {
		krate_manager_reset_krate_password(ad->krate_manager, ad->krate_name, ad->krate_password);

		app_control_create(&app_control);
		app_control_set_app_id(app_control, KEYGUARD_PACKAGE);
		snprintf(uri, sizeof(uri), "krate://setup/%s", ad->krate_name);
		app_control_set_uri(app_control, uri);
		app_control_send_launch_request(app_control, NULL, NULL);
		app_control_destroy(app_control);

		app_control_create(&app_control);
		app_control_set_app_id(app_control, KASKIT_PACKAGE);
		__launch_krate_app(ad->krate_manager, ad->krate_name, app_control);
		app_control_destroy(app_control);
	}

	ad->request_done = true;
}

static bool __app_create(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	if (krate_manager_create(&ad->krate_manager) != KRATE_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get krate manager handle");
		ui_app_exit();
		return false;
	}

	return true;
}

static void __app_pause(void *data)
{
}

static void __app_resume(void *data)
{
}

static void __free_data(appdata_s *ad)
{
	free(ad->krate_name);
	free(ad->mode);
}

static void __app_terminate(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	krate_manager_remove_event_cb(ad->krate_manager, ad->krate_event_cb_id);
	krate_manager_destroy(ad->krate_manager);

	__free_data(ad);

	return ;
}

static void __set_krate_callback(appdata_s *ad)
{
	char *cb_event_list[2] = {"created", "removed"};
	char *cb_event = NULL;

	if (!strcmp(ad->mode, "create"))
		cb_event = cb_event_list[0];
	else
		cb_event = cb_event_list[1];

	if (krate_manager_add_event_cb(ad->krate_manager, cb_event, __krate_request_done, ad, &ad->krate_event_cb_id) != KRATE_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add krate signal callback");
		ui_app_exit();
	}

	return;
}

static void __app_control(app_control_h app_control, void *data)
{
	appdata_s *ad = (appdata_s *) data;
	int ret = 0;

	ret = app_control_get_extra_data(app_control, "mode", &ad->mode);
	if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get mode");
		ui_app_exit();
		return;
	}

	if (strcmp(ad->mode, "create") && strcmp(ad->mode, "remove")) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Invalid mode");
		ui_app_exit();
		return;
	}

	ret = app_control_get_extra_data(app_control, "krate", &ad->krate_name);
	if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get krate name");
		ui_app_exit();
		return;
	}

	__set_krate_callback(ad);

	if (app_control_clone(&ad->app_control, app_control) != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to clone app control handler");
		ui_app_exit();
		return;
	}

	_create_base_window(ad);

	return;
}

int main(int argc, char *argv[])
{
	appdata_s ad = {0, };
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0, };

	event_callback.create = __app_create;
	event_callback.terminate = __app_terminate;
	event_callback.pause = __app_pause;
	event_callback.resume = __app_resume;
	event_callback.app_control = __app_control;

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main is failed. err = %d", ret);

	return ret;
}
