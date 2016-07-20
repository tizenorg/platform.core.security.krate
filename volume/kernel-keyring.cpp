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

#include <unistd.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <linux/limits.h>

#include "kernel-keyring.h"

#ifndef KEYCTL_SEARCH
#define KEYCTL_SEARCH   10
#endif

KeySerial KernelKeyRing::add(const char *type, const char* description, const void* payload, size_t plen, KeySerial ringid)
{
	return ::syscall(__NR_add_key, type, description, payload, plen, ringid);
}

long KernelKeyRing::search(KeySerial ringid, const char* type, const char* description, KeySerial destringid)
{
	return ::syscall(__NR_keyctl, KEYCTL_SEARCH, ringid, type, description, destringid);
}

long KernelKeyRing::link(KeySerial keyid, KeySerial ringid)
{
	return ::syscall(__NR_keyctl, KEYCTL_LINK, keyid, ringid);
}

long KernelKeyRing::unlink(KeySerial keyid, KeySerial ringid)
{
	return ::syscall(__NR_keyctl, KEYCTL_UNLINK, keyid, ringid);
}
