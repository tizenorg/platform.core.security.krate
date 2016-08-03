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

#include <syslog.h>
#include <security/pam_ext.h>
#include <security/pam_modules.h>

#include <string>
#include <vector>

#include "session.h"
#include "krate-guard.h"
#include "krate-builder.h"

#include <klay/exception.h>
#include <klay/filesystem.h>
#include <klay/xml/parser.h>
#include <klay/xml/document.h>

std::string buildKrateManifestPath(const std::string& name)
{
	return CONF_PATH "/" + name + ".xml";
}

std::string getKrateName(pam_handle_t* handle)
{
	const void* retItem;
	int error = ::pam_get_item(handle, PAM_USER, &retItem);
	if (error != PAM_SUCCESS) {
		throw runtime::Exception("Failed to get user");
	}

	return static_cast<const char*>(retItem);
}

void openKrateSession(const std::string& name)
{
	auto sessionBuilder = [](const runtime::User& user) {
		KrateBuilder builder(user, buildKrateManifestPath(user.getName()));
		builder.containerize();
	};

	createSession(runtime::User(name), sessionBuilder);
}

void closeKrateSession(const std::string& name)
{
	destroySession(runtime::User(name));
}

extern "C" {
PAM_EXTERN  __attribute__((visibility("default")))
int pam_sm_open_session(pam_handle_t* pamh, int flags, int argc, const char* argv[])
{
	try {
		std::string name = getKrateName(pamh);
		KrateGuard krateGuard(name);
		krateGuard.wait();

		openKrateSession(name);
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
		std::string name = getKrateName(pamh);
		KrateGuard krateGuard(name);
		krateGuard.wait();

		closeKrateSession(name);
	} catch (runtime::Exception& e) {
		::pam_syslog(pamh, LOG_ERR, "%s", e.what());
		return PAM_SESSION_ERR;
	}

	return PAM_SUCCESS;
}

#ifdef PAM_MODULE_ENTRY
PAM_MODULE_ENTRY("pam_krate");
#endif

}
