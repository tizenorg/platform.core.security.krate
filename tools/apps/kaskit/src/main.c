/*
 * Tizen Krate launcher application
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
#include <badge.h>
#include <app_control.h>
#include <app_manager.h>
#include <package_manager.h>

#include "kaskit.h"
#include "widget.h"

static package_manager_h __pkg_mgr;

static const char* __pkg_blacklist[] = {
	"org.tizen.setting",
	"org.tizen.dpm-toolkit",
	NULL
};

struct app_icon_s{
	char* id;
	char* label;
	char* icon;
	char* package;
	bool removable;
};

static void* __create_app_icon(void* data)
{
	struct app_icon_s* app = (struct app_icon_s*)data;
	unsigned int badge_show = 0, badge_count = 0;

	_create_app_icon(app->package, app->id, app->label, app->icon, app->removable);

	int ret = badge_get_display(app->id, &badge_show);
	dlog_print(DLOG_ERROR, LOG_TAG, "badge_get_display err = %d", ret);
	if (badge_show) {
		ret =badge_get_count(app->id, &badge_count);
		dlog_print(DLOG_ERROR, LOG_TAG, "badge_get_count err = %d", ret);
		if (badge_count > 0) {
			_update_app_icon_badge(app->id, badge_count);
		}
	}

	return NULL;
}

static bool __pkg_is_removable(const char* pkg_id)
{
	bool removable = false;

	package_info_h pkg_h;

	package_info_create(pkg_id, &pkg_h);
	package_info_is_removable_package(pkg_h, &removable);
	package_info_destroy(pkg_h);

	return removable;
}

static bool __pkg_is_in_blacklist(const char* pkg_id)
{
	int i;
	for (i = 0; __pkg_blacklist[i] != NULL; i++) {
		if (strcmp(pkg_id, __pkg_blacklist[i]) == 0) {
			return true;
		}
	}
	return false;
}

static bool __get_app_info_cb(app_info_h app_h, void* user_data)
{
	struct app_icon_s app = {NULL, };
	bool nodisplay = true;

	app_info_is_nodisplay(app_h, &nodisplay);
	if (nodisplay) {
		return true;
	}

	app_info_get_package(app_h, &app.package);

	if (__pkg_is_in_blacklist(app.package)) {
		free(app.package);
		return true;
	}

	if (user_data == NULL ||  !strncmp(user_data, app.package, PATH_MAX)) {
		app_info_get_app_id(app_h, &app.id);
		app_info_get_label(app_h, &app.label);
		app_info_get_icon(app_h, &app.icon);
		app.removable = __pkg_is_removable(app.package);

		ecore_main_loop_thread_safe_call_sync(__create_app_icon, &app);

		free(app.id);
		if (app.label != NULL) {
			free(app.label);
		}
		if (app.icon != NULL) {
			free(app.icon);
		}
	}

	free(app.package);
	return true;
}

static void __create_icon_thread(void* data, Ecore_Thread* thread) {
	app_manager_foreach_app_info(__get_app_info_cb, data);
	if (data != NULL) {
		free(data);
	}
}

static void __pkg_event_cb(const char* type,
	const char* pkg_id,
	package_manager_event_type_e event_type,
	package_manager_event_state_e event_state, int progress,
	package_manager_error_e error, void* user_data)
{
	if (event_state == PACKAGE_MANAGER_EVENT_STATE_COMPLETED) {
		if (event_type == PACKAGE_MANAGER_EVENT_TYPE_INSTALL) {
			ecore_thread_run(__create_icon_thread, NULL, NULL, strdup(pkg_id));
		} else if (event_type == PACKAGE_MANAGER_EVENT_TYPE_UNINSTALL) {
			_destroy_app_icon(pkg_id);
		}
	}
}

static void __badge_changed_cb(unsigned int action, const char *app_id, unsigned int count, void *user_data) {
	_update_app_icon_badge(app_id, count);
}


static char* __get_current_zone_name() {
	struct passwd pwd, *result;
	int bufsize;

	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1) {
		bufsize = 16384;
	}

	char* ret, *buf = malloc(bufsize * sizeof(char));

	getpwuid_r(getuid(), &pwd, buf, bufsize, &result);
	if (result == NULL) {
		ret = NULL;
	} else {
		ret = strdup(result->pw_name);
	}
	free(buf);
	return ret;
}

void _icon_clicked_cb(const char *app_id)
{
	app_control_h app_control;
	app_control_create(&app_control);
        app_control_set_app_id(app_control, app_id);
	app_control_send_launch_request(app_control, NULL, NULL);
	app_control_destroy(app_control);
}

void _icon_uninstalled_cb(const char *pkg_id)
{
	package_manager_request_h pkg_mgr_req;
	int id;

	package_manager_request_create(&pkg_mgr_req);
	package_manager_request_uninstall(pkg_mgr_req, pkg_id, &id);
	package_manager_request_destroy(pkg_mgr_req);
}

static void __show_launcher()
{
	char *zone_name = __get_current_zone_name();

	_set_kaskit_window_title(zone_name);

	package_manager_set_event_status(__pkg_mgr,
		PACKAGE_MANAGER_STATUS_TYPE_INSTALL |
		PACKAGE_MANAGER_STATUS_TYPE_UNINSTALL);
	package_manager_set_event_cb(__pkg_mgr, __pkg_event_cb, NULL);

	badge_register_changed_cb(__badge_changed_cb, NULL);

	ecore_thread_run(__create_icon_thread, NULL, NULL, NULL);

	free(zone_name);
}

static bool __app_create(void *data)
{
	package_manager_create(&__pkg_mgr);

	_create_kaskit_window();
	__show_launcher();

	return true;
}

static void __app_control(app_control_h app_control, void *data)
{
}

static void __app_pause(void *data)
{
}

static void __app_resume(void *data)
{
}

static void __app_terminate(void *data)
{
	package_manager_destroy(__pkg_mgr);
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
