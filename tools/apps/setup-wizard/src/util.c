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
#include <notification.h>
#include "zone-setup.h"

#define ZONE_METADATA_PATH "data/ZoneManifest.xml"

static char *__get_zone_metadata(void)
{
	FILE *fp = NULL;
	char *res_path = NULL;
	char *metadata = NULL;
	char metadata_path[PATH_MAX] = "\0";
	long fsize = 0;
	int ret = -1;

	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get resource path");
		return NULL;
	}
	snprintf(metadata_path, PATH_MAX, "%s%s", res_path, ZONE_METADATA_PATH);
	free(res_path);

	fp = fopen(metadata_path, "r");
	if (fp != NULL) {
		if (fseek(fp, 0, SEEK_END) == -1)
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to fseek");

		fsize = ftell(fp);
		if (fsize == -1) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to get file size");
			fclose(fp);
			return NULL;
		}

		metadata = malloc(fsize + 1);
		if (metadata == NULL) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to allocate memory");
			fclose(fp);
			return NULL;
		}
		memset(metadata, 0, fsize + 1);
		if (fseek(fp, 0, SEEK_SET) == -1) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to fseek");
			fclose(fp);
			free(metadata);
			return NULL;
		}

		ret = fread(metadata, fsize, 1, fp);
		if (ret < 0) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to read metadata file");
			fclose(fp);
			free(metadata);
			return NULL;
		}
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to open metadata file");
		return NULL;
	}

	fclose(fp);

	return metadata;
}

int _send_zone_create_request(appdata_s *ad)
{
	int ret;
	char *metadata = NULL;

	metadata = __get_zone_metadata();
	if (metadata == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get zone metadata");
		return -1;
	}

	ret = zone_manager_create_zone(ad->zone_manager, ad->zone_name, metadata);
	if (ret != ZONE_ERROR_NONE) {
		return -1;
	}

	return 0;
}

int _send_zone_remove_request(appdata_s *ad)
{
	int ret;

	ret = zone_manager_destroy_zone(ad->zone_manager, ad->zone_name);
	if (ret != ZONE_ERROR_NONE) {
		return -1;
	}

	return 0;
}

static int __set_notification(notification_h noti_handle, app_control_h app_control)
{
	int ret = 0;
        char *mode = NULL;
        char *noti_text[2][2] = {
                {NOTI_CREATE_ZONE, NOTI_BODY_CREATE_ZONE},
                {NOTI_REMOVE_ZONE, NOTI_BODY_REMOVE_ZONE}
        };
        char **text = NULL;

        if (app_control_get_extra_data(app_control, "mode", &mode) != APP_CONTROL_ERROR_NONE)
                return -1;

        if (!strcmp(mode, "create"))
                text = noti_text[0];
        else
                text = noti_text[1];

        ret = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_TITLE, text[0], NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
        if (ret != NOTIFICATION_ERROR_NONE)
                return -1;

        ret = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_CONTENT, text[1], NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
        if (ret != NOTIFICATION_ERROR_NONE)
                return -1;

        ret = notification_set_display_applist(noti_handle, NOTIFICATION_DISPLAY_APP_ALL);
        if (ret != NOTIFICATION_ERROR_NONE)
                return -1;

        ret = notification_set_image(noti_handle, NOTIFICATION_IMAGE_TYPE_THUMBNAIL, DPM_SYSPOPUP_ICON_PATH);
        if (ret != NOTIFICATION_ERROR_NONE)
                return -1;

        ret = notification_set_launch_option(noti_handle, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, app_control);
        if (ret != NOTIFICATION_ERROR_NONE)
                return -1;

        return ret;
}

void _create_notification(app_control_h app_control)
{
	notification_h noti_handle = NULL;
        int ret = 0;

        noti_handle = notification_create(NOTIFICATION_TYPE_NOTI);

        ret = __set_notification(noti_handle, app_control);
        if (ret != NOTIFICATION_ERROR_NONE) {
                notification_free(noti_handle);
                app_control_destroy(app_control);
                return;
        }

        notification_post(noti_handle);
        notification_free(noti_handle);
        app_control_destroy(app_control);

        return;
}
