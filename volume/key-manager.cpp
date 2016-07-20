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

#include <string>

#include <klay/exception.h>
#include <ckmc/ckmc-manager.h>

#include "key-manager.h"


#define addAliasPrefix(alias)	\
	(ckmcIdSystem + ckmcIdSeperator + alias)

namespace {

const std::string ckmcIdSystem = ckmc_owner_id_system;
const std::string ckmcIdSeperator = ckmc_owner_id_separator;

} //namespace

bool KeyManager::isKeyExist(const std::string& keyName)
{
	int ret;
	ckmc_raw_buffer_s* keyData = NULL;
	std::string ckmAlias = addAliasPrefix(keyName);

	ret = ::ckmc_get_data(ckmAlias.c_str(), NULL, &keyData);
	::ckmc_buffer_free(keyData);

	return (ret != CKMC_ERROR_DB_ALIAS_UNKNOWN);
}

void KeyManager::addKey(const std::string& keyName, const std::string& data)
{
	int ret;
	const std::string ckmAlias = addAliasPrefix(keyName);
	ckmc_raw_buffer_s keyData;
	ckmc_policy_s keyPolicy;

	keyData.data = (unsigned char*)data.c_str();
	keyData.size = data.size();

	keyPolicy.password = NULL;
	keyPolicy.extractable = true;

	ret = ::ckmc_save_data(ckmAlias.c_str(), keyData, keyPolicy);
	if (ret != 0) {
		throw runtime::Exception("Failed to add key data");
	}
}

std::string KeyManager::getKey(const std::string& keyName)
{
	int ret;
	const std::string ckmAlias = addAliasPrefix(keyName);
	ckmc_raw_buffer_s* keyData;
	std::string result;

	ret = ::ckmc_get_data(ckmAlias.c_str(), NULL, &keyData);
	if (ret != CKMC_ERROR_NONE) {
		throw runtime::Exception("Failed to get key data");
	}

	result = std::string(reinterpret_cast<char*>(keyData->data), keyData->size);

	::ckmc_buffer_free(keyData);

	return result;
}

void KeyManager::removeKey(const std::string& keyName)
{
	const std::string ckmAlias = addAliasPrefix(keyName);

	if (::ckmc_remove_data(ckmAlias.c_str()) != 0) {
		throw runtime::Exception("Failed to remove key data");
	}
}
