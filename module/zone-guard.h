/*
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

#ifndef __ZONE_GUARD_H__
#define __ZONE_GUARD_H__

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

#include <string>

#include "exception.h"

class ZoneGuard {
public:
    ZoneGuard(const std::string& name) :
        zoneName(name),
        semaphore(nullptr)
    {
        semaphore = ::sem_open(zoneName.c_str(), O_CREAT, 0700, 1);
        if (semaphore == nullptr) {
            throw runtime::Exception("Filed to create semaphore for zone guard");
        }
    }

    ~ZoneGuard()
    {
        if (semaphore == nullptr) {
            return;
        }

        ::sem_post(semaphore);
        ::sem_close(semaphore);
        ::sem_unlink(zoneName.c_str());
    }

    void wait()
    {
        while ((::sem_wait(semaphore) == -1) && (errno == EINTR));
    }

private:
    std::string zoneName;
    sem_t* semaphore;
};

#endif //!__ZONE_GUARD_H__
