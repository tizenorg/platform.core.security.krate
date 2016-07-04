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

 #ifndef __ZONE_BUILDER_H__
 #define __ZONE_BUILDER_H__

#include <string>
#include <vector>
#include <memory>

#include "xml/parser.h"
#include "xml/document.h"
#include "auth/user.h"

class ZoneBuilder {
public:
    ZoneBuilder(const runtime::User& user, const std::string& manifestPath);
    virtual ~ZoneBuilder();

    void containerize(bool create = true);

protected:
    void bindFilesystemNode(const std::string& source, const std::string& target,
                            const std::string& type, const std::string& options,
                            bool create = true);
private:
    std::string name;
    uid_t uid;
    gid_t gid;
    std::unique_ptr<xml::Document> manifest;
};

 #endif //!__ZONE_BUILDER_H__
