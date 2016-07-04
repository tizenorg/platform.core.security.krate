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

#define PASSPHRASE_STATE_UNDEFINED	0
#define PASSPHRASE_STATE_SETUP		1
#define PASSPHRASE_STATE_VERIFY		2

extern uidata_s ud;

struct security_lock_type {
	const char* text;
	int index;
};

struct security_lock_type security_lock_types[2] = {
	{ "Password", 0 },
	{ "PIN", 1 }
};

char* security_group_text[2] = {
	"Unlock method",
	"Security options"
};

static char* security_password_setup_data = NULL;
static char* security_password_verify_data = NULL;

static unsigned int security_passphrase_mode = PASSPHRASE_STATE_UNDEFINED;
static int security_lock_type_selected = 0;

static void create_password_setup_view(appdata_s *ad);

static char *security_multiline_text_get(void *data, Evas_Object *obj, const char *part)
{
	char text[PATH_MAX] = "\0";

	if (!strcmp(part, "elm.text.multiline")) {
		snprintf(text, PATH_MAX, "Select a Krate unlock method and a timeout option.");
		return strdup(text);
	}

	return NULL;
}

static char *security_group_text_get(void *data, Evas_Object *obj, const char *part)
{
	char *text = (char *)data;

	if (!strcmp(part, "elm.text")) {
		return strdup(text);
	}

	return NULL;
}

static char *security_double_label_text_get(void *data, Evas_Object *obj, const char *part)
{
	char text[PATH_MAX] = "\0";
	int timeout = 10; /*[TBD] get value of timeout */

	if (!strcmp(part, "elm.text"))
		snprintf(text, PATH_MAX, "Security Timeout");
	else if (!strcmp(part, "elm.text.sub"))
		snprintf(text, PATH_MAX, "After %d minuates of inactivity", timeout);

	return strdup(text);
}

static char *security_lock_type_text_get(void *data, Evas_Object *obj, const char *part)
{
	struct security_lock_type *locktype = (struct security_lock_type *)data;

	if (!strcmp(part, "elm.text")) {
		return strdup(locktype->text);
	}

	return NULL;
}

static Evas_Object *security_lock_type_content_get(void *data, Evas_Object *obj, const char *part)
{
	static Evas_Object *group = NULL;
	struct security_lock_type* locktype = (struct security_lock_type *)data;

	if (group == NULL) {
		group = elm_radio_add(obj);
		elm_radio_state_value_set(group, 0);
		elm_radio_value_set(group, 0);
	}

	if (!strcmp(part, "elm.swallow.icon")) {
		Evas_Object* radio = elm_radio_add(obj);
		elm_radio_state_value_set(radio, locktype->index);

		if (locktype->index == security_lock_type_selected) {
			elm_radio_value_set(radio, locktype->index);
		}

		elm_radio_group_add(radio, group);
		evas_object_propagate_events_set(radio, EINA_FALSE);
		evas_object_repeat_events_set(radio, EINA_TRUE);

		return radio;
	}

	return NULL;
}

static void security_lock_type_select_cb(void *data, Evas_Object *obj, void *event_info)
{
	struct security_lock_type* locktype = (struct security_lock_type *)data;

	elm_genlist_item_selected_set((Elm_Object_Item *)event_info, EINA_FALSE);
	elm_radio_value_set(obj, locktype->index);
 	security_lock_type_selected = locktype->index;
}

static Eina_Bool security_pop_cb(void *data, Elm_Object_Item *it)
{
	elm_object_signal_emit(ud.conform, "elm,state,indicator,overlap", "elm");

	return EINA_TRUE;
}

static void security_previous_view_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (security_passphrase_mode != PASSPHRASE_STATE_UNDEFINED) {
		security_passphrase_mode--;
	}
	elm_naviframe_item_pop(ud.nf);
}

static void security_password_setup_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;
	if (security_passphrase_mode < PASSPHRASE_STATE_VERIFY) {
		security_passphrase_mode++;
		create_password_setup_view(ad);
		return;
	}

	if (strcmp(security_password_setup_data, security_password_verify_data) != 0) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Password not matched");
		return;
	}

        ad->zone_password = security_password_setup_data;

	if (_send_zone_create_request(ad) != 0) {
		ui_app_exit();
	}

	elm_object_signal_emit(ud.conform, "elm,state,indicator,overlap", "elm");
	_create_setup_view(ad);
}

static void security_password_entry_unfocused_cb(void *data, Evas_Object *obj, void *event_info)
{
	char **password = (char **)data;
	*password = strdup(elm_entry_entry_get(obj));
}

static Evas_Object *security_password_setup_content_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp(part, "elm.swallow.content")) {
		Evas_Object *layout, *entry;

		layout =  _create_layout(obj, ud.edj_path, "security_layout");

		entry = _create_entry(obj);
		elm_object_part_text_set(layout, "title", "Enter password");
		elm_object_part_content_set(layout, "entry", entry);
		evas_object_smart_callback_add(entry, "unfocused", security_password_entry_unfocused_cb, data);

		return layout;
	}

	return NULL;
}

static void create_password_setup_view(appdata_s *ad)
{
	char** entry;
	const char* title;
	Elm_Object_Item *item;
	Elm_Genlist_Item_Class *itc;
	Evas_Object *genlist, *layout, *left_button, *right_button;

	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");
	genlist = elm_genlist_add(layout);
	elm_layout_content_set(layout, "content_layout", genlist);
	elm_object_style_set(genlist, "solid/default");
	elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);

	left_button = _create_button(layout, PREV_BUTTON, security_previous_view_cb, NULL);
	right_button = _create_button(layout, NEXT_BUTTON, security_password_setup_cb, ad);

	//elm_object_disabled_set(right_button, EINA_TRUE);
	_create_two_button_layout(layout, left_button, right_button);

	if (security_passphrase_mode == 1) {
		entry = &security_password_setup_data;
		title = "Setup Password";
	} else {
		entry = &security_password_verify_data;
		title = "Verify Password";
	}

	itc = _create_genlist_item_class("full", NULL, security_password_setup_content_get);
	item = _append_genlist_item(genlist, itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, (void **)entry);

	item = elm_naviframe_item_push(ud.nf, title, NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(item, EINA_TRUE, EINA_TRUE);
}

void _create_security_view(appdata_s *ad)
{
	int  index;
	Elm_Object_Item *item;
	Elm_Genlist_Item_Class *itc;
	Evas_Object *genlist, *layout, *left_button, *right_button;

	elm_object_signal_emit(ud.conform, "elm,state,indicator,nooverlap", "elm");

	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");
	genlist = elm_genlist_add(layout);
	elm_object_style_set(genlist, "solid/default");
	elm_layout_content_set(layout, "content_layout", genlist);
	elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);

	itc = _create_genlist_item_class("multiline", security_multiline_text_get, NULL);
	_append_genlist_item(genlist, itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, NULL);

	itc =  _create_genlist_item_class("group_index", security_group_text_get, NULL);
	_append_genlist_item(genlist, itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, security_group_text[0]);

	itc = _create_genlist_item_class("one_icon", security_lock_type_text_get, security_lock_type_content_get);
	for (index = 0; index < 2; index++) {
		item = elm_genlist_item_append(genlist,
									   itc,
									   &security_lock_types[index],
									   NULL,
									   ELM_GENLIST_ITEM_NONE,
									   security_lock_type_select_cb,
									   &security_lock_types[index]);
		if (index == 1) {
			elm_object_item_disabled_set(item, EINA_TRUE);
		}
	}

	/* Timeout list group*/
	itc = _create_genlist_item_class("group_index", security_group_text_get, NULL);
	_append_genlist_item(genlist, itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, security_group_text[1]);

	itc = _create_genlist_item_class("double_label", security_double_label_text_get, NULL);
	item = _append_genlist_item(genlist, itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, NULL);
	elm_object_item_disabled_set(item, EINA_TRUE); /* [TBD] enable timeout options */

	left_button = _create_button(layout, PREV_BUTTON, security_previous_view_cb, NULL);
	right_button = _create_button(layout, NEXT_BUTTON, security_password_setup_cb, ad);
	_create_two_button_layout(layout, left_button, right_button);

	item = elm_naviframe_item_push(ud.nf, "Krate Security", NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(item, EINA_TRUE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(item, security_pop_cb, NULL);
}
