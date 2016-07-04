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

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "session.h"
#include "exception.h"
#include "filesystem.h"

bool isValidSessionLeader(pid_t pid)
{
    runtime::File proc("/proc/" + std::to_string(pid));
    return proc.exists();
}

void createSession(const runtime::User& user, const SessionBuilder& sessionBuilder)
{
    runtime::File file("/var/run/zone/" + user.getName());
    if (file.exists()) {
        if (isValidSessionLeader(getSessionLeader(user))) {
            throw runtime::Exception("Session already opened");
        }
        file.remove();
    } else {
        file.makeBaseDirectory();
    }

    sessionBuilder(user);

    file.create(0600);
    file.lock();
    file.chown(user.getUid(), user.getGid());
    pid_t pid = ::getpid();
    file.write(&pid, sizeof(pid_t));
    file.unlock();
}

pid_t getSessionLeader(const runtime::User& user)
{
    runtime::File file("/var/run/zone/" + user.getName(), O_RDONLY);
    file.lock();
    pid_t pid = -1;
    file.read(&pid, sizeof(pid_t));
    file.unlock();

    return pid;
}

void destroySession(const runtime::User& user)
{
    std::string path = "/var/run/zone/" + user.getName();
    ::unlink(path.c_str());
}
