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
#include "zone-setup.h"
#include "widget.h"

static void __create_welcome_view(appdata_s *ad);

uidata_s ud = {0, };

static Eina_Bool __naviframe_pop_cb(void *data, Elm_Object_Item *it)
{
	app_control_h svc = (app_control_h) data;

	_create_notification(svc);
	ui_app_exit();

	return EINA_FALSE;
}

static void __welcome_cancel_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;

	_create_notification(ad->app_control);
	ui_app_exit();

	return;
}

static void __welcome_next_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;

	if (!strcmp(ad->mode, "create")) {
		_create_security_view(ad);
	} else {
		if (_send_zone_remove_request(ad) != 0)
			ui_app_exit();
		_create_setup_view(ad);
	}

	return;
}

static Eina_Bool __home_key_cb(void *data, int type, void *event)
{
	app_control_h app_control = (app_control_h) data;
	Evas_Event_Key_Down *ev = event;

	if (!strcmp(ev->keyname, "XF86Home")) {
		_create_notification(app_control);
		ui_app_exit();
	}

	return EINA_TRUE;
}

void _create_two_button_layout(Evas_Object *parent, Evas_Object *left_button, Evas_Object *right_button)
{
	Evas_Object *layout;

	layout = _create_layout(parent, ud.edj_path, "two_button_layout");

	elm_object_part_content_set(layout, "prev_button", left_button);
	elm_object_part_content_set(layout, "next_button", right_button);
	elm_object_part_content_set(parent, "bottom_layout", layout);
	return;
}

static Eina_Bool __progressbar_timer_cb(void *data)
{
	appdata_s *ad = (appdata_s *)data;
	if (ad->request_done) {
		ui_app_exit();
		return ECORE_CALLBACK_CANCEL;
	}

	return ECORE_CALLBACK_RENEW;
}

void _create_base_window(appdata_s *ad)
{
	Evas_Object *win;
	Evas_Object *layout;

	char edj_path[PATH_MAX] = "\0";
	char *res_path = NULL;

	/* Initialize data */
	ad->request_done = false;

	/* Get EDJ path */
	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed get resource path");
		ui_app_exit();
	}
	snprintf(edj_path, PATH_MAX, "%s%s", res_path, "org.tizen.zone-setup-wizard.edj");

	ud.edj_path = strdup(edj_path);
	free(res_path);

	/* Create main UI widget */
	win = _create_win(PACKAGE);
	ud.conform = _create_conformant(win);
	layout = _create_layout(ud.conform, NULL, NULL);
	elm_object_content_set(ud.conform, layout);
	ud.nf = elm_naviframe_add(layout);

	ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, __home_key_cb, ad->app_control);

	__create_welcome_view(ad);

	elm_object_part_content_set(layout, "elm.swallow.content", ud.nf);
	eext_object_event_callback_add(ud.nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);

	evas_object_show(win);
	return;
}

static void __create_welcome_view(appdata_s *ad)
{
	Elm_Object_Item *nf_it;
	Evas_Object *layout, *welcome_layout;
	Evas_Object *title, *content;
	Evas_Object *left_button, *right_button;

	char *welcome_text[2][4] = {
		{WELCOME_MESSAGE_TITLE, WELCOME_MESSAGE_CONTENT, CANCEL_BUTTON, NEXT_BUTTON},
		{DELETE_MESSAGE_TITLE, DELETE_MESSAGE_CONTENT, CANCEL_BUTTON, REMOVE_BUTTON}
	};
	char **text = NULL;

	if (!strcmp(ad->mode, "create"))
		text = welcome_text[0];
	else
		text = welcome_text[1];

	elm_object_signal_emit(ud.conform, "elm,state,indicator,overlap", "elm");

	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");
	welcome_layout = _create_layout(layout, ud.edj_path, "welcome_layout");

	title = _create_textblock(welcome_layout, text[0], SUB_TITLE_STYLE_B);
	content = _create_textblock(welcome_layout, text[1], SUB_CONTENT_STYLE_B);

	elm_object_part_content_set(welcome_layout, "message_title", title);
	elm_object_part_content_set(welcome_layout, "message_content", content);

	elm_object_part_content_set(layout, "content_layout", welcome_layout);

	left_button = _create_button(layout, text[2], __welcome_cancel_cb, ad);
	right_button = _create_button(layout, text[3], __welcome_next_cb, ad);

	_create_two_button_layout(layout, left_button, right_button);

	nf_it = elm_naviframe_item_push(ud.nf, NULL, NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, __naviframe_pop_cb, ad->app_control);

	return;
}

void _create_setup_view(appdata_s *ad)
{
	Elm_Object_Item *nf_it;
	Evas_Object *setup_layout;
	Evas_Object *progressbar;
	Evas_Object *title, *content;

	char *setup_text[2][2] = {
		{SETUP_MESSAGE_TITLE, SETUP_MESSAGE_CONTENT},
		{DELETE_ONGOING_TITLE, DELETE_ONGOING_CONTENT}
	};
	char **text = NULL;

	if (!strcmp(ad->mode, "create"))
		text = setup_text[0];
	else
		text = setup_text[1];

	eext_object_event_callback_del(ud.nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb);

	setup_layout = _create_layout(ud.nf, ud.edj_path, "setup_layout");

	title = _create_textblock(setup_layout, text[0], SUB_TITLE_STYLE_W);
	content = _create_textblock(setup_layout, text[1], SUB_CONTENT_STYLE_W);

	elm_object_part_content_set(setup_layout, "progressbar_msg", title);

	progressbar = _create_progressbar(setup_layout, "pending");
	elm_object_part_content_set(setup_layout, "progressbar", progressbar);

	elm_object_part_content_set(setup_layout, "content_text", content);

	nf_it = elm_naviframe_item_push(ud.nf, NULL, NULL, NULL, setup_layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);

	/* set progressbar timer callback */
	ecore_timer_add(0.1, __progressbar_timer_cb, ad);

	return;
}
