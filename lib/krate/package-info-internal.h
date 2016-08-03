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

#ifndef __ZONE_PACKAGE_INFO_INTERNAL_H__
#define __ZONE_PACKAGE_INFO_INTERNAL_H__

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
typedef struct description_x {
		char *name;
		char *text;
		char *lang;
} description_x;

// pkgmgr-info : include/pkgmgrinfo_basic.h
typedef struct label_x {
		char *name;
		char *text;
		char *lang;
} label_x;

// pkgmgr-info : include/pkgmgrinfo_basic.h
typedef struct author_x {
		char *email;
		char *href;
		char *text;
		char *lang;
} author_x;

// pkgmgr-info : include/pkgmgrinfo_basic.h
typedef struct package_x {
	char *for_all_users;        /**< Flag that indicates if the package is available for everyone or for current user only, no xml part*/
	char *package;              /**< package name, attr*/
	char *version;              /**< package version, attr*/
	char *installlocation;      /**< package install location, attr, default: "internal-only"*/
	char *ns;                   /**<name space, attr*/
	char *removable;            /**< package removable flag, no xml part*/
	char *preload;              /**< package preload flag, no xml part*/
	char *readonly;             /**< package readonly flag, no xml part*/
	char *update;               /**< package update flag, no xml part*/
	char *appsetting;           /**< package app setting flag, attr, default: "false"*/
	char *system;               /**< package system flag, no xml part*/
	char *type;                 /**< package type, attr*/
	char *package_size;         /**< package size for external installation, attr*/
	char *installed_time;       /**< installed time after finishing of installation, no xml part*/
	char *installed_storage;    /**< package currently installed storage, no xml part*/
	char *storeclient_id;       /**< id of store client for installed package, attr*/
	char *mainapp_id;           /**< app id of main application, no xml part*/
	char *package_url;          /**< app id of main application, attr*/
	char *root_path;            /**< package root path, attr*/
	char *csc_path;             /**< package csc path, attr*/
	char *nodisplay_setting;    /**< package no display setting menu, attr, default: "false"*/
	char *support_disable;      /**< package support disable flag, attr, default: "false"*/
	char *api_version;          /**< minimum version of API package using, attr, default: patch_version trimmed version from tizen_full_version*/
	char *tep_name;             /*no xml part*/
	char *zip_mount_file;       /*no xml part*/
	char *backend_installer;    /**< package backend installer, attr*/
	GList *icon;                /**< package icon, element*/
	GList *label;               /**< package label, element*/
	GList *author;              /**< package author, element*/
	GList *description;         /**< package description, element*/
	GList *license;             /**< package license, no xml part*/
	GList *privileges;          /**< package privileges, element*/
	GList *application;         /**< package's application, element*/
	GList *compatibility;       /**< package compatibility, element*/
	GList *deviceprofile;       /**< package device profile, element*/
} package_x;

// pkgmgr-info : include/pkgmgrinfo_private.h
typedef struct _pkgmgr_pkginfo_x {
	uid_t uid;
	package_x *pkg_info;
	char *locale;
} pkgmgr_pkginfo_x;

// package-manager : src/package_info.c
typedef struct package_info_s {
	char *package;
	pkgmgr_pkginfo_x *pkgmgr_pkginfo;
} package_info_s;

#endif //__ZONE_PACKAGE_INFO_INTERNAL_H__
