/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#ifndef __DPM_APP_INFO_INTERNAL_H__
#define __DPM_APP_INFO_INTERNAL_H__

#include <glib.h>

// pkgmgr-info : include/pkgmgrinfo_basic.h
typedef struct icon_x {
		char *text;
		char *lang;
		char *section;
		char *size;
		char *resolution;
		char *dpi;
} icon_x;

// pkgmgr-info : include/pkgmgrinfo_basic.h
typedef struct label_x {
		char *name;
		char *text;
		char *lang;
} label_x;

// pkgmgr-info : include/pkgmgrinfo_basic.h
typedef struct application_x {
	char *appid;    /*attr*/
	char *exec;     /*attr*/
	char *nodisplay;    /*attr, default: "false"*/
	char *multiple; /*attr, default: "false"*/
	char *taskmanage;       /*attr, default: "true"*/
	char *enabled;  /*attr, default: "true"*/
	char *type;     /*attr*/
	char *categories;       /*attr*/
	char *extraid;  /*attr*/
	char *hwacceleration;   /*attr, default: "default"*/
	char *screenreader;     /*attr, default: "use-system-setting"*/
	char *mainapp;  /*attr, default: "false"*/
	char *package;  /*set from package_x*/
	char *recentimage;      /*attr, default: "false"*/
	char *launchcondition;  /*attr, default: "false"*/
	char *indicatordisplay; /*attr, default: "true"*/
	char *portraitimg;      /*attr*/
	char *landscapeimg;     /*attr*/
	char *effectimage_type; /*attr, default: "image"*/
	char *guestmode_visibility;     /*attr, default: "true"*/
	char *component;    /*no xml part*/
	char *permission_type;  /*attr, default: "normal"*/
	char *component_type;   /*attr, default: "uiapp"*/
	char *preload;  /*no xml part*/
	char *submode;  /*attr, default: "false"*/
	char *submode_mainid;   /*attr, default: "false"*/
	char *process_pool;     /*attr, default: "false"*/
	char *installed_storage;
	char *autorestart;      /*attr, default: "false"*/
	char *onboot;   /*attr, default: "false"*/
	char *support_disable;  /*set from package_x*/
	char *ui_gadget;    /*attr, default: "false"*/
	char *launch_mode;      /*attr, default: "single"*/
	char *ambient_support;  /*attr, default: "false"*/
	char *alias_appid;      /*attr*/
	char *effective_appid;  /*attr*/
	char *package_type;     /*set from package_x*/
	char *tep_name; /*set from package_x*/
	char *zip_mount_file;   /*set from package_x*/
	char *root_path;    /*set from package_x*/
	char *api_version;      /*set from package_x*/
	char *for_all_users; /**< Flag that indicates if the package is available for everyone or for current user only, no xml part*/
	char *is_disabled; /**< Flag that indicates if the application is disabled or not, no xml part*/
	char *splash_screen_display; /*attr, default: "true"*/
	GList *label;   /*element*/
	GList *icon;    /*element*/
	GList *image;   /*element*/
	GList *category; /*element*/
	GList *metadata;    /*element*/
	GList *permission;      /*element*/
	GList *launchconditions;    /*element*/
	GList *notification;    /*element*/
	GList *datashare;       /*element*/
	GList *datacontrol; /*element*/
	GList *background_category; /*element*/
	GList *appcontrol; /*element*/
	GList *splashscreens; /*element*/
} application_x;

// pkgmgr-info : include/pkgmgrinfo_private.h
typedef struct _pkgmgr_appinfo_x {
	const char *package;
	char *locale;
	int app_component;
	application_x *app_info;
} pkgmgr_appinfo_x;

// app-manager : src/app_info.c
typedef struct app_info_s {
	char *app_id;
	pkgmgr_appinfo_x *pkg_app_info;
} app_info_s;


#endif //__DPM_APP_INFO_INTERNAL_H__
