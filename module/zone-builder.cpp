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
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <sys/mount.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <stdio.h>

#include "zone-builder.h"

#include "exception.h"
#include "filesystem.h"

ZoneBuilder::ZoneBuilder(const runtime::User& user, const std::string& manifestPath) :
    name(user.getName()), uid(user.getUid()), gid(user.getGid())
{
    manifest.reset(xml::Parser::parseFile(manifestPath));
}

ZoneBuilder::~ZoneBuilder()
{
}

void ZoneBuilder::bindFilesystemNode(const std::string& source, const std::string& target,
                                     const std::string& type, const std::string& options,
                                     bool create)
{
    if (create) {
        runtime::File dir(target);
        if (!dir.exists()) {
            dir.makeDirectory(true, uid, gid);
        }
    }

    runtime::Mount::mountEntry(source, target, type, options);
}

void ZoneBuilder::containerize(bool create)
{
    int nsFlags = CLONE_NEWIPC | CLONE_NEWNS;

    if (::unshare(nsFlags)) {
        throw runtime::Exception("Failed to unshare namespace");
    }

    if (::mount(NULL, "/", NULL, MS_SLAVE | MS_REC, NULL) == -1) {
        throw runtime::Exception("Failed to mount root filesystem");
    }

    xml::Node::NodeList entries = manifest->evaluate("/manifest/filesystem/entry");
    for (const xml::Node& entry : entries) {
        bindFilesystemNode(entry.getProp("source"), entry.getProp("target"),
                           entry.getProp("type"), entry.getProp("options"));
    }

    bindFilesystemNode("/home/" + name,
                       "/home/" + name + "/.zone/" + name,
                       "none", "rw,bind");

    bindFilesystemNode("/home/" + name + "/.zone", "/home",
                       "none", "rw,rbind");
}
