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
#include "kaskit.h"
#include "widget.h"
#include "conf.h"

static uidata_s ud = {0, };

static int __num_of_apps = 0;
static bool __is_edit_mode = false;
static Ecore_Timer* __app_icon_long_press_timer = NULL;

static void __block_clicked_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
	ui_app_exit();
}

static void __app_view_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	eina_list_free(ud.app_icon_list);
}

static void __set_kaskit_layout()
{
	ud.panel = _create_layout(ud.layout, ud.edj_path, "popup_layout");

	elm_object_part_content_set(ud.layout, "popup_window", ud.panel);

	ud.scroller = elm_scroller_add(ud.panel);
	elm_scroller_bounce_set(ud.scroller, EINA_FALSE, EINA_TRUE);
	elm_scroller_policy_set(ud.scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
	elm_object_part_content_set(ud.panel, "popup_content", ud.scroller);

	ud.app_view = elm_table_add(ud.scroller);
	elm_table_homogeneous_set(ud.app_view, EINA_TRUE);
	elm_object_content_set(ud.scroller, ud.app_view);

	evas_object_event_callback_add(ud.app_view, EVAS_CALLBACK_DEL, __app_view_del_cb, NULL);

	return;
}

static char* __get_res_path(const char* file)
{
	char *res_path = NULL;
	char edj_path[PATH_MAX] = "\0";

	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed get resource path");
		ui_app_exit();
	}
	snprintf(edj_path, PATH_MAX, "%s%s", res_path, file);

	free(res_path);

	return strdup(edj_path);
}

static void __back_key_cb(void *data , Evas_Object *obj , void *event_info)
{
        Evas_Object* icon;
        Eina_List* i;

	if (__is_edit_mode) {
		__is_edit_mode = false;

		EINA_LIST_FOREACH(ud.app_icon_list, i, icon) {
			elm_object_signal_emit(icon, "uninstall_button_hide", "source");
			elm_object_signal_emit(icon, "icon_sub_badge_show", "source");
		}
	} else {
	        ui_app_exit();
	}
}

void _create_kaskit_window()
{
	ud.edj_path = __get_res_path(PACKAGE ".edj");
	ud.win = _create_win(PACKAGE);
	ud.conform = _create_conformant(ud.win);
	ud.layout = _create_layout(ud.conform, ud.edj_path, "main_window");
	elm_object_content_set(ud.conform, ud.layout);
	eext_object_event_callback_add(ud.win, EEXT_CALLBACK_BACK, __back_key_cb, NULL);

	elm_object_signal_callback_add(ud.layout, "bg_clicked", "layout", __block_clicked_cb, NULL);

	__set_kaskit_layout();

	evas_object_show(ud.win);

	return;
}

static Eina_Bool __app_icon_long_press_cb(void *data)
{
	Evas_Object* icon;
	Eina_List* i;

	EINA_LIST_FOREACH(ud.app_icon_list, i, icon) {
		if (evas_object_data_get(icon, "removable")) {
			elm_object_signal_emit(icon, "uninstall_button_show", "source");
		}
		elm_object_signal_emit(icon, "icon_sub_badge_hide", "source");
	}

	__is_edit_mode = true;

	return ECORE_CALLBACK_CANCEL;
}

void _set_kaskit_window_title(const char *title)
{
	elm_object_part_text_set(ud.panel, "popup_title", title);
}

static int __icon_down_x, __icon_down_y;
static void __app_icon_down_cb(void *data, Evas *e, Evas_Object *obj, void *event_info) {
	Evas_Event_Mouse_Up* ev = event_info;

	__icon_down_x = ev->output.x;
	__icon_down_y = ev->output.y;

	__app_icon_long_press_timer = ecore_timer_add(LONG_PRESS_TIME, __app_icon_long_press_cb, NULL);
}

static void __app_icon_move_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Event_Mouse_Up* ev = event_info;

	int distance_x = (ev->output.x - __icon_down_x);
	int distance_y = (ev->output.y - __icon_down_y);
        int distance = distance_x * distance_x + distance_y * distance_y;

	if (distance > ALLOWED_ICON_DRAG_DISTANCE) {
		if (__app_icon_long_press_timer != NULL) {
			ecore_timer_del(__app_icon_long_press_timer);
			__app_icon_long_press_timer = NULL;
		}
	}
}

static void __app_icon_up_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Event_Mouse_Up* ev = event_info;

	int distance_x = (ev->output.x - __icon_down_x);
	int distance_y = (ev->output.y - __icon_down_y);
        int distance = distance_x * distance_x + distance_y * distance_y;

	if (distance <= ALLOWED_ICON_DRAG_DISTANCE && !__is_edit_mode) {
		_icon_clicked_cb(evas_object_data_get(obj, "id"));
	}

	if (__app_icon_long_press_timer != NULL) {
		ecore_timer_del(__app_icon_long_press_timer);
		__app_icon_long_press_timer = NULL;
	}
}

static void __app_icon_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	free(evas_object_data_get(obj, "id"));
	free(evas_object_data_get(obj, "package"));
}

static void __app_icon_uninstall_btn_clicked_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
	_icon_uninstalled_cb(evas_object_data_get(obj, "package"));
}

void _create_app_icon(const char* pkg_id, const char* app_id, const char* label, const char* icon, bool removable)
{
	char string[1024] = {0, }, *default_icon;
	Evas_Object *icon_layout;
	Evas_Object *icon_image;

	icon_layout = _create_layout(ud.app_view, ud.edj_path, "icon");
	evas_object_size_hint_weight_set(icon_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(icon_layout, 0.0, 0.0);

	icon_image = elm_image_add(icon_layout);
	if (ecore_file_can_read(icon)) {
		elm_image_file_set(icon_image, icon, NULL);
	} else {
		default_icon = __get_res_path("images/default_app_icon.png");
		elm_image_file_set(icon_image, default_icon, NULL);
		free(default_icon);
	}
	evas_object_size_hint_min_set(icon_image, ICON_IMG_SIZE, ICON_IMG_SIZE);
	evas_object_size_hint_max_set(icon_image, ICON_IMG_SIZE, ICON_IMG_SIZE);

        snprintf(string, sizeof(string), "<font_size=%d><color=#%s><shadow_color=#%s>%s</shadow_color></color></font_size>",
		ICON_TXT_SIZE_NORMAL, ICON_TXT_COLOR, ICON_TXT_SHADOW_COLOR,
		label);
	elm_object_part_text_set(icon_layout, "icon_name", string);

	elm_object_part_content_set(icon_layout, "icon_content", icon_image);

	evas_object_data_set(icon_layout, "id", strdup(app_id));
	evas_object_data_set(icon_layout, "package", strdup(pkg_id));
	evas_object_data_set(icon_layout, "removable", (const void*)removable);

	evas_object_event_callback_add(icon_layout, EVAS_CALLBACK_DEL, __app_icon_del_cb, NULL);
	evas_object_event_callback_add(icon_layout, EVAS_CALLBACK_MOUSE_DOWN, __app_icon_down_cb, NULL);
	evas_object_event_callback_add(icon_layout, EVAS_CALLBACK_MOUSE_MOVE, __app_icon_move_cb, NULL);
	evas_object_event_callback_add(icon_layout, EVAS_CALLBACK_MOUSE_UP, __app_icon_up_cb, NULL);

	elm_object_signal_callback_add(icon_layout, "uninstall_button_clicked", "source", __app_icon_uninstall_btn_clicked_cb, NULL);

	elm_table_pack(ud.app_view, icon_layout, __num_of_apps % 3, __num_of_apps / 3, 1, 1);
	evas_object_size_hint_min_set(ud.app_view, 0, (__num_of_apps / 3 + 1) * ICON_SIZE_H);

	ud.app_icon_list = eina_list_append(ud.app_icon_list, icon_layout);

	__num_of_apps++;

	evas_object_show(icon_image);
	evas_object_show(icon_layout);

	if (__is_edit_mode && removable) {
		elm_object_signal_emit(icon_layout, "uninstall_button_show", "source");
	}
}

void _destroy_app_icon(const char* pkg_id)
{
	int index = 0;
	Eina_List* i, *i_next;
	Evas_Object* app_icon;
	char *app_pkg_id;

	EINA_LIST_FOREACH_SAFE(ud.app_icon_list, i, i_next, app_icon) {
		app_pkg_id = evas_object_data_get(app_icon, "package");
		if (strncmp(app_pkg_id, pkg_id, PATH_MAX)) {
			elm_table_pack(ud.app_view, app_icon, index % 3, index / 3, 1, 1);
			evas_object_size_hint_min_set(ud.app_view, 0, (__num_of_apps / 3 + 1) * ICON_SIZE_H);
			index++;
			continue;
		}
		elm_table_unpack(ud.app_view, app_icon);
		evas_object_del(app_icon);
		evas_object_size_hint_min_set(ud.app_view, 0, (__num_of_apps / 3 + 1) * ICON_SIZE_H);
		ud.app_icon_list = eina_list_remove_list(ud.app_icon_list, i);
	}
	__num_of_apps--;
	evas_object_size_hint_min_set(ud.app_view, 0, (__num_of_apps / 3 + 1) * ICON_SIZE_H);
}

void _update_app_icon_badge(const char* app_id, unsigned int count)
{
	Eina_List* i, *i_next;
	Evas_Object* app_icon;
	char str[8], *icon_app_id;

	EINA_LIST_FOREACH_SAFE(ud.app_icon_list, i, i_next, app_icon) {
		icon_app_id = evas_object_data_get(app_icon, "id");
		if (strncmp(icon_app_id, app_id, PATH_MAX) == 0) {
			if (count == 0) {
				elm_object_signal_emit(app_icon, "icon_badge_hide", "source");
				break;
			}

			if (count > MAX_BADGE_DISPLAY_COUNT) {
				snprintf(str, 8, "%d+", MAX_BADGE_DISPLAY_COUNT);
			} else {
				snprintf(str, 8, "%d", count);
			}
			elm_layout_text_set(app_icon, "badge_text", str);
			elm_object_signal_emit(app_icon, "icon_badge_show", "source");
			break;
		}
	}
}
