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
#ifndef __ZONE_SETUP_H__
#define __ZONE_SETUP_H__

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
#include <zone/zone.h>
#include <zone/app-proxy.h>

#include "setup-text.h"

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "zone-setup-wizard"

#if !defined(PACKAGE)
#define PACKAGE "org.tizen.zone-setup-wizard"
#endif

#define KEYGUARD_PACKAGE "org.tizen.keyguard"
#define KASKIT_PACKAGE "org.tizen.kaskit"

#define DPM_SYSPOPUP_ICON_PATH "/usr/share/icons/default/small/org.tizen.dpm-syspopup.png"

typedef struct {
	char *mode;
	char *zone_name;
        char *zone_password;

	zone_manager_h zone_manager;
	int zone_event_cb_id;
	bool request_done;

	app_control_h app_control;
} appdata_s;

typedef struct {
	Evas_Object *conform;
	Evas_Object *nf;
	char *edj_path;
} uidata_s;

void _create_base_window(appdata_s *data);
void _create_security_view(appdata_s *data);
void _create_setup_view(appdata_s *data);
void _create_two_button_layout(Evas_Object *parent, Evas_Object *left_button, Evas_Object *right_button);

int _send_zone_create_request(appdata_s *ad);
int _send_zone_remove_request(appdata_s *ad);
void _create_notification(app_control_h app_control);

#endif /* __ZONE_SETUP_H__ */
