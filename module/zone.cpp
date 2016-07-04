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

#include <security/pam_ext.h>
#include <security/pam_modules.h>
#include <syslog.h>

#include <string>
#include <vector>

#include "zone-guard.h"
#include "zone-builder.h"
#include "session.h"

#include "exception.h"
#include "filesystem.h"
#include "xml/parser.h"
#include "xml/document.h"

#define ZONE_MANIFEST_DIR CONF_PATH "/zone/"

std::string buildZoneManifestPath(const std::string& name)
{
    return ZONE_MANIFEST_DIR + name + ".xml";
}

std::string getZoneName(pam_handle_t* handle)
{
    const void* retItem;
    int error = ::pam_get_item(handle, PAM_USER, &retItem);
    if (error != PAM_SUCCESS) {
        throw runtime::Exception("Failed to get user");
    }

    return static_cast<const char*>(retItem);
}

void openZoneSession(const std::string& name)
{
    auto sessionBuilder = [](const runtime::User& user) {
        ZoneBuilder builder(user, buildZoneManifestPath(user.getName()));
        builder.containerize();
    };

    createSession(runtime::User(name), sessionBuilder);
}

void closeZoneSession(const std::string& name)
{
    destroySession(runtime::User(name));
}

extern "C" {
PAM_EXTERN  __attribute__((visibility("default")))
int pam_sm_open_session(pam_handle_t* pamh, int flags, int argc, const char* argv[])
{
    try {
        std::string name = getZoneName(pamh);
        ZoneGuard zoneGuard(name);
        zoneGuard.wait();

        openZoneSession(name);
    } catch (runtime::Exception& e) {
        ::pam_syslog(pamh, LOG_ERR, "%s", e.what());
        return PAM_SESSION_ERR;
    }

    return PAM_SUCCESS;
}

PAM_EXTERN  __attribute__((visibility("default")))
int pam_sm_close_session(pam_handle_t* pamh, int flags, int argc, const char* argv[])
{
    try {
        std::string name = getZoneName(pamh);
        ZoneGuard zoneGuard(name);
        zoneGuard.wait();

        closeZoneSession(name);
    } catch (runtime::Exception& e) {
        ::pam_syslog(pamh, LOG_ERR, "%s", e.what());
        return PAM_SESSION_ERR;
    }

    return PAM_SUCCESS;
}

#ifdef PAM_MODULE_ENTRY
PAM_MODULE_ENTRY("pam_zone");
#endif

}
