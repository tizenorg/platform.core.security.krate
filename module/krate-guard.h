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

#ifndef __KRATE_GUARD_H__
#define __KRATE_GUARD_H__

#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>

#include <string>

#include <klay/exception.h>

class KrateGuard {
public:
	KrateGuard(const std::string& name) :
		krateName(name), semaphore(nullptr)
	{
		semaphore = ::sem_open(krateName.c_str(), O_CREAT, 0700, 1);
		if (semaphore == nullptr) {
			throw runtime::Exception("Filed to create semaphore for krate guard");
		}
	}

	~KrateGuard()
	{
		if (semaphore == nullptr) {
			return;
		}

		::sem_post(semaphore);
		::sem_close(semaphore);
		::sem_unlink(krateName.c_str());
	}

	void wait()
	{
		while ((::sem_wait(semaphore) == -1) && (errno == EINTR));
	}

private:
	std::string krateName;
	sem_t* semaphore;
};

#endif //!__KRATE_GUARD_H__
