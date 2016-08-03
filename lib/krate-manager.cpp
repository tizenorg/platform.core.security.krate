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

#include "rmi/krate-manager.h"

namespace Krate {

KrateManager::KrateManager(KrateControlContext& ctx) :
	context(ctx)
{
}

KrateManager::~KrateManager()
{
}

int KrateManager::createKrate(const std::string& name, const std::string& setupWizAppid)
{
	try {
		return context->methodCall<int>("KrateManager::createKrate", name, setupWizAppid);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int KrateManager::removeKrate(const std::string& name)
{
	try {
		return context->methodCall<int>("KrateManager::removeKrate", name);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int KrateManager::lockKrate(const std::string& name)
{
	try {
		return context->methodCall<int>("KrateManager::lockKrate", name);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int KrateManager::unlockKrate(const std::string& name)
{
	try {
		return context->methodCall<int>("KrateManager::unlockKrate", name);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int KrateManager::getKrateState(const std::string& name)
{
	try {
		return context->methodCall<int>("KrateManager::getKrateState", name);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

std::vector<std::string> KrateManager::getKrateList(int state)
{
	std::vector<std::string> empty;
	try {
		return context->methodCall<std::vector<std::string>>("KrateManager::getKrateList", state);
	} catch (runtime::Exception& e) {
		return empty;
	}
}

int KrateManager::resetKratePassword(const std::string& name, const std::string& newPassword)
{
	try {
		return context->methodCall<int>("KrateManager::resetKratePassword", name, newPassword);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

} // namespace Krate
