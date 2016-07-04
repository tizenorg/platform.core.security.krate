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
#ifndef __ZONE_KEYGUARD_H__
#define __ZONE_KEYGUARD_H__

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
#include <shortcut_manager.h>
#include <auth-passwd.h>
#include <zone/zone.h>
#include <zone/app-proxy.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "keyguard"

#if !defined(PACKAGE)
#define PACKAGE "org.tizen.keyguard"
#endif

#define KEYGUARD_PACKAGE "org.tizen.keyguard"
#define KASKIT_PACKAGE "org.tizen.kaskit"

void _create_keyguard_window(void);
void _launch_req_app(void);

bool _has_password(void);
bool _check_password(const char* password);
unsigned int _get_left_attempts(void);

#endif /* __ZONE_KEYGUARD_H__ */
