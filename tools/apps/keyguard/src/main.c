/*
 * Tizen Zone Keyguard application
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
#include "keyguard.h"
#include "widget.h"

static app_control_h __req_app_control;

void _launch_req_app()
{
	app_control_send_launch_request(__req_app_control, NULL, NULL);
	app_control_destroy(__req_app_control);
}

bool _check_password(const char* password)
{
	unsigned int attempt, max_attempt, expire_sec;
	int ret;

	ret = auth_passwd_check_passwd(AUTH_PWD_NORMAL, password, &attempt, &max_attempt, &expire_sec);

	return ret == AUTH_PASSWD_API_SUCCESS;
}

bool _has_password()
{
	unsigned int attempt, max_attempt, expire_sec;
	int ret;

	ret = auth_passwd_check_passwd_state(AUTH_PWD_NORMAL, &attempt, &max_attempt, &expire_sec);

	return ret != AUTH_PASSWD_API_ERROR_NO_PASSWORD;
}

unsigned int _get_left_attempts()
{
	unsigned int attempt = 0, max_attempt = 0, expire_sec;

        auth_passwd_check_passwd_state(AUTH_PWD_NORMAL, &attempt, &max_attempt, &expire_sec);

	if (max_attempt == 0) {
		return 0xffffffff;
	}

	return max_attempt - attempt;
}

static void __launch_zone_app(const char* zone_name, app_control_h app_control)
{
	zone_manager_h zone_mgr;
	zone_app_proxy_h zone_app;

	zone_manager_create(&zone_mgr);
	zone_app_proxy_create(zone_mgr, zone_name, &zone_app);
	zone_app_proxy_send_launch_request(zone_app, app_control);
	zone_app_proxy_destroy(zone_app);
	zone_manager_destroy(zone_mgr);
}

static void __add_shortcut(const char* zone_name)
{
	char uri[PATH_MAX];

	snprintf(uri, sizeof(uri), "zone://enter/%s", zone_name);
	shortcut_add_to_home(zone_name, LAUNCH_BY_URI, uri, "", 0, NULL, NULL);
}

static bool __app_create(void *data)
{
	return true;
}

static void __app_pause(void *data)
{
	return;
}

static void __app_resume(void *data)
{
	return;
}

static void __app_terminate(void *data)
{
	return ;
}

static void __app_control(app_control_h app_control, void *data)
{
	char* uri, *tmp;
	int ret = 0;

	ret = app_control_get_uri(app_control, &uri);
	if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get URI");
		ui_app_exit();
		return;
	}

	if (strncmp(uri, "zone://", sizeof("zone://") - 1) != 0) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Mismatched URI");
		free(uri);
		ui_app_exit();
		return;
	}

	tmp = uri + sizeof("zone://") - 1;

	if (strncmp(tmp, "setup/", sizeof("setup/") - 1) == 0) {
		char *zone_name;

		zone_name = tmp + sizeof("setup/") - 1;
		__add_shortcut(zone_name);
		ui_app_exit();
		return;
	} else if (strncmp(tmp, "enter/", sizeof("enter/") - 1) == 0) {
		char* zone_name, *launch_parameter;
		char new_uri[PATH_MAX];

		zone_name = tmp + sizeof("enter/") - 1;
		launch_parameter = strchr(zone_name, '/');
		if (launch_parameter != NULL) {
			*(launch_parameter++) = '\0';
			if (launch_parameter[0] == '\0') {
				launch_parameter = KASKIT_PACKAGE;	
			}
		} else {
			launch_parameter = KASKIT_PACKAGE;
		}
		snprintf(new_uri, PATH_MAX, "zone://launch/%s", launch_parameter);
		app_control_set_uri(app_control, new_uri);
		app_control_set_app_id(app_control, PACKAGE);

		dlog_print(DLOG_ERROR, LOG_TAG, "Wow");
		dlog_print(DLOG_ERROR, LOG_TAG, PACKAGE);
		dlog_print(DLOG_ERROR, LOG_TAG, new_uri);
		dlog_print(DLOG_ERROR, LOG_TAG, zone_name);

		__launch_zone_app(zone_name, app_control);
		ui_app_exit();
		return;
	}else if (strncmp(tmp, "launch/", sizeof("launch/") - 1) == 0) {
		char* app_id;

		app_id = tmp + sizeof("launch/") - 1;
		uri = strchr(app_id, '?');
		if (uri != NULL) {
			*(uri++) = '\0';
			if (strncmp(uri, "uri=", sizeof("uri=") - 1) == 0) {
				tmp += sizeof("uri=") - 1;
			}
		}
		app_control_clone(&__req_app_control, app_control);
		app_control_set_uri(__req_app_control, uri);
		app_control_set_app_id(__req_app_control, app_id);

		if (_has_password()) {
			_create_keyguard_window();
		} else {
			_launch_req_app();
		}
		return;
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "Invalid URI");
		ui_app_exit();
	}
	free(uri);
}

int main(int argc, char *argv[])
{
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0, };

	event_callback.create = __app_create;
	event_callback.terminate = __app_terminate;
	event_callback.pause = __app_pause;
	event_callback.resume = __app_resume;
	event_callback.app_control = __app_control;

	ret = ui_app_main(argc, argv, &event_callback, NULL);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main is failed. err = %d", ret);

	return ret;
}
