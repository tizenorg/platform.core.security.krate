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

#include <pwd.h>
#include <shadow.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <regex>
#include <memory>

#include "user.h"
#include "group.h"
#include "shadow.h"
#include "filesystem.h"
#include "exception.h"

namespace runtime {

User::User(const User& user) :
	name(user.name), uid(user.uid), gid(user.gid)
{
}

User::User(const std::string& user)
{
	struct passwd pwd, *result;
	int bufsize;

	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1)
		bufsize = 16384;

	std::unique_ptr<char[]> buf(new char[bufsize]);

	::getpwnam_r(user.c_str(), &pwd, buf.get(), bufsize, &result);
	if (result == NULL) {
		throw runtime::Exception("User " + user + " doesn't exist");
	}

	name = result->pw_name;
	uid = result->pw_uid;
	gid = result->pw_gid;
}

User::User(const uid_t user)
{
	struct passwd pwd, *result;
	int bufsize;

	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1)
		bufsize = 16384;

	std::unique_ptr<char[]> buf(new char[bufsize]);
	::getpwuid_r(user, &pwd, buf.get(), bufsize, &result);
	if (result == NULL) {
		throw runtime::Exception("User " + std::to_string(user) + "doesn't exist");
	}

	name = result->pw_name;
	uid = result->pw_uid;
	gid = result->pw_gid;
}

User::User() :
	User(::getuid())
{
}

} // namespace runtime
