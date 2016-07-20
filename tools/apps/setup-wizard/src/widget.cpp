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
#include "widget.h"

void __win_delete_request_cb(void *data , Evas_Object *obj , void *event_info)
{
	ui_app_exit();
}

Evas_Object *_create_button(Evas_Object *parent, const char *text, Evas_Smart_Cb callback, void *user_data)
{
	Evas_Object *btn = elm_button_add(parent);

	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(btn, text);
	elm_object_style_set(btn, "bottom");
	evas_object_smart_callback_add(btn, "clicked", callback, user_data);

	evas_object_show(btn);

	return btn;
}

Evas_Object *_create_textblock(Evas_Object *parent, const char *text, char *style)
{
	Evas_Object *txt = evas_object_textblock_add(parent);
	Evas_Textblock_Style *text_st = NULL;

	if (style != NULL) {
		text_st = evas_textblock_style_new();
		evas_textblock_style_set(text_st, style);
		evas_object_textblock_style_set(txt, text_st);
	}

	evas_object_textblock_text_markup_set(txt, text);
	evas_object_show(txt);

	if (text_st != NULL)
		evas_textblock_style_free(text_st);

	return txt;
}

Evas_Object *_create_progressbar(Evas_Object *parent, const char *style)
{
	Evas_Object *progressbar = elm_progressbar_add(parent);

	if (style != NULL)
		elm_object_style_set(progressbar, style);

	evas_object_size_hint_align_set(progressbar, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(progressbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_progressbar_pulse(progressbar, EINA_TRUE);
	elm_progressbar_pulse_set(progressbar, EINA_TRUE);
	evas_object_show(progressbar);

	return progressbar;
}

Elm_Genlist_Item_Class *_create_genlist_item_class(char *style, Elm_Gen_Item_Text_Get_Cb text_func, Elm_Gen_Item_Content_Get_Cb content_func)
{
	Elm_Genlist_Item_Class *itc;

	itc = elm_genlist_item_class_new();
	itc->item_style = style;
	itc->func.text_get = text_func;
	itc->func.content_get = content_func;

	return itc;
}

Elm_Object_Item *_append_genlist_item(Evas_Object *genlist, Elm_Genlist_Item_Class *itc, Elm_Object_Select_Mode select_mode, void* data)
{
	Elm_Object_Item *gl_item;

	gl_item = elm_genlist_item_append(genlist, itc, data, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);

	elm_genlist_item_select_mode_set(gl_item, select_mode);
	elm_genlist_item_class_free(itc);

	return gl_item;
}

Evas_Object *_create_entry(Evas_Object *parent)
{
	Evas_Object *entry = elm_entry_add(parent);
	static Elm_Entry_Filter_Limit_Size limit_size = {
		.max_char_count = 20,
		.max_byte_count = 0
	};

	evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_entry_password_set(entry, EINA_TRUE);
	elm_entry_single_line_set(entry, EINA_TRUE);
	elm_entry_markup_filter_append(entry, elm_entry_filter_limit_size, &limit_size);

	evas_object_show(entry);
	return entry;
}

Evas_Object *_create_popup(Evas_Object *parent, const char *title, const char *body)
{
	Evas_Object *popup = elm_popup_add(parent);

	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_style_set(popup, "default");
	elm_object_part_text_set(popup, "title,text", title);
	elm_object_item_part_text_translatable_set(popup, "title,text", EINA_TRUE);
	elm_object_text_set(popup, body);

	return popup;
}
