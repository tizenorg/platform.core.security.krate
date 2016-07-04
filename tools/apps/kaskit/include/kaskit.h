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
#ifndef __KASKIT_H__
#define __KASKIT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <app.h>
#include <app_common.h>
#include <bundle.h>
#include <dlog.h>
#include <Elementary.h>
#include <efl_extension.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "org.tizen.kaskit"

#if !defined(PACKAGE)
#define PACKAGE "org.tizen.kaskit"
#endif

typedef struct {
        Evas_Object* win;
        Evas_Object* conform;
        Evas_Object* layout;
        Evas_Object* panel;
        Evas_Object* scroller;
        Evas_Object* app_view;
        Eina_List* app_icon_list;
        char* edj_path;
} uidata_s;

void _create_kaskit_window();
void _set_kaskit_window_title(const char *title);

void _create_app_icon(const char* pkg_id, const char* app_id, const char* label, const char* icon, bool removable);
void _destroy_app_icon(const char* pkg_id);
void _update_app_icon_badge(const char* app_id, unsigned int count);

void _icon_clicked_cb(const char *app_id);
void _icon_uninstalled_cb(const char *app_id);

#endif /* __KASKIT_H__ */
