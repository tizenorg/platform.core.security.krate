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

#ifndef WIDGET_H_
#define WIDGET_H_

#include <app.h>
#include <dlog.h>
#include <Elementary.h>
#include <efl_extension.h>

Evas_Object *_create_win(const char *package);
Evas_Object *_create_conformant(Evas_Object *parent);
Evas_Object *_create_layout(Evas_Object *parent, char *file, const char *group);
Evas_Object *_create_button(Evas_Object *parent, const char *text, Evas_Smart_Cb callback, void *user_data);
Evas_Object *_create_textblock(Evas_Object *parent, const char *text, char *style);
Evas_Object *_create_progressbar(Evas_Object *parent, const char *style);
Evas_Object *_create_entry(Evas_Object *parent);

Elm_Genlist_Item_Class *_create_genlist_item_class(char *style, Elm_Genlist_Item_Text_Get_Cb text_func, Elm_Genlist_Item_Content_Get_Cb content_func);
Elm_Object_Item *_append_genlist_item(Evas_Object *genlist, Elm_Genlist_Item_Class *itc, Elm_Object_Select_Mode select_mode, void* data);

#endif /* WIDGET_H_ */
