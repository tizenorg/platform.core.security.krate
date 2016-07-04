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
#include <efl_util.h>
#include "widget.h"

static void __win_delete_request_cb(void *data , Evas_Object *obj , void *event_info)
{
	ui_app_exit();
}

Evas_Object *_create_win(const char *package)
{
        Evas_Object *win;
        elm_app_base_scale_set(1.8);

	win = elm_win_add(NULL, package, ELM_WIN_NOTIFICATION);
	efl_util_set_notification_window_level(win, EFL_UTIL_NOTIFICATION_LEVEL_DEFAULT);
	/* [TBD] enable : eext_win_keygrab_set(win, "XF86Home"); */

	elm_win_conformant_set(win, EINA_FALSE);

	evas_object_smart_callback_add(win, "delete,request", __win_delete_request_cb, NULL);

	elm_win_indicator_mode_set(win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(win, ELM_WIN_INDICATOR_TRANSPARENT);
	eext_object_event_callback_add(win, EEXT_CALLBACK_BACK, __win_delete_request_cb, NULL);
	return win;
}

Evas_Object *_create_conformant(Evas_Object *parent)
{
	Evas_Object *conform = elm_conformant_add(parent);

	evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(parent, conform);
	elm_object_signal_emit(conform, "elm,state,indicator,overlap", "elm");
	evas_object_show(conform);

	return conform;
}

Evas_Object *_create_layout(Evas_Object *parent, char *file, const char *group)
{
	Evas_Object *layout = elm_layout_add(parent);

	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	if (file == NULL)
		elm_layout_theme_set(layout, "layout", "application", "default");
	else
		elm_layout_file_set(layout, file, group);

	evas_object_show(layout);

	return layout;
}

Evas_Object *_create_entry(Evas_Object *parent)
{
	Evas_Object *entry = elm_entry_add(parent);
	static Elm_Entry_Filter_Limit_Size limit_size = {
		.max_char_count = 20,
		.max_byte_count = 0,
	};

	evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_entry_password_set(entry, EINA_TRUE);
	elm_entry_single_line_set(entry, EINA_TRUE);
	elm_entry_markup_filter_append(entry, elm_entry_filter_limit_size, &limit_size);

	evas_object_show(entry);
	return entry;
}

Evas_Object *_create_button(Evas_Object *parent, const char *text)
{
	Evas_Object *btn = elm_button_add(parent);
	elm_object_style_set(btn, "popup");
	elm_object_text_set(btn, text);
	return btn;
}

static void __popup_timeout_cb(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_del(obj);
}

static void __block_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_del(obj);
}

static void __popup_btn_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *popup = (Evas_Object *)data;
	evas_object_del(popup);
}

Evas_Object *_create_popup(Evas_Object *parent, const char *title, const char *content, char *style)
{
	Evas_Object *popup = elm_popup_add(parent);

	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_style_set(popup, style);
	elm_object_text_set(popup, content);

	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, parent);
	if (!strcmp(style, "toast")) {
		elm_popup_timeout_set(popup, 3.0);
		evas_object_smart_callback_add(popup, "timeout", __popup_timeout_cb, NULL);
		evas_object_smart_callback_add(popup, "block,clicked", __block_clicked_cb, NULL);
	} else if (!strcmp(style, "default")) {
		elm_object_part_text_set(popup, "title,text", title);
		elm_object_item_part_text_translatable_set(popup, "title,text", EINA_TRUE);

		Evas_Object *btn = _create_button(popup, "OK");
		elm_object_part_content_set(popup, "button1", btn);
		evas_object_smart_callback_add(btn, "clicked", __popup_btn_clicked_cb, popup);
		eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, NULL);
	}

	return popup;
}
