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

#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/types.h>

#include <cstring>
#include <string>
#include <vector>
#include <iostream>

#include <klay/error.h>
#include <klay/exception.h>
#include <klay/audit/logger.h>

#include "ecryptfs.h"
#include "kernel-keyring.h"
#include "key-manager.h"
#include "key-generator.h"

int generateKey(const std::string& keyName)
{
	if (KeyManager::isKeyExist(keyName)) {
		ERROR("Key already registered");
		return -1;
	}

	try {
		std::string pass = KeyGenerator::generateKey(ECRYPTFS_MAX_KEY_SIZE);
		std::string salt = KeyGenerator::generateKey(ECRYPTFS_MAX_SALT_SIZE);
		std::string wrappedKey = KeyGenerator::wrapKey(pass, salt, ECRYPTFS_MAX_KEY_SIZE);

		KeyManager::addKey(keyName, wrappedKey);
	} catch (runtime::Exception& e) {
		ERROR(e.what());
		return -1;
	}

	return 0;
}

ecryptfs_payload* generateToken(char* key)
{
	struct ecryptfs_password* tokenKey;

	unsigned char keyBuffer[ECRYPTFS_MAX_KEY_SIZE+1];

	ecryptfs_payload* authToken = (ecryptfs_payload *)::malloc(sizeof(ecryptfs_payload));
	if (authToken == NULL) {
		return NULL;
	}

	::memset(authToken, 0, sizeof(ecryptfs_payload));
	::strncpy((char*)keyBuffer, key, ECRYPTFS_MAX_KEY_SIZE);
	keyBuffer[ECRYPTFS_MAX_KEY_SIZE] = '\0';

	tokenKey = &authToken->token.password;

	authToken->version = ECRYPTFS_VERSION;
	authToken->token_type = ECRYPTFS_PWD_PAYLOAD_TYPE;
	tokenKey->session_key_encryption_key_bytes = ECRYPTFS_MAX_KEY_SIZE;
	tokenKey->flags = ECRYPTFS_SESSION_KEY_ENCRYPTION_KEY_SET;
	::memcpy(tokenKey->session_key_encryption_key, keyBuffer, ECRYPTFS_MAX_KEY_SIZE);
	::memcpy(tokenKey->signature, keyBuffer, ECRYPTFS_MAX_SIG_HEX);

	return authToken;
}

int mountEcryptfs(const std::string& src, const std::string& keyName)
{
	int rc;
	char ecryptfsOpts[1024];
	ecryptfs_payload* authTok = NULL;

	std::string key;
	try {
		key = KeyManager::getKey(keyName);
	} catch (runtime::Exception& e) {
		ERROR(e.what());
		return -1;
	}

	if (KernelKeyRing::link(KEY_SPEC_USER_KEYRING, KEY_SPEC_SESSION_KEYRING) != 0) {
		ERROR("Failed to link key");
		return -1;
	}

	if ((authTok = generateToken((char*)key.c_str())) == NULL) {
		ERROR("Failed to generate Token");
		return -1;
	}

	const char* signature = (const char*)authTok->token.password.signature;
	rc = KernelKeyRing::search(KEY_SPEC_USER_KEYRING,
							   "user",
							   signature,
							   0);
	if (rc == -1 && errno != ENOKEY) {
		ERROR("Failed to find key");
		return -1;
	}
	if (rc == -1) {
		rc = KernelKeyRing::add("user",
								signature,
								(void*)authTok,
								sizeof(ecryptfs_payload),
								KEY_SPEC_USER_KEYRING);
		if (rc == -1) {
			ERROR("Failed to add key");
			return -1;
		}
	}

	::snprintf(ecryptfsOpts, 1024,
			   "ecryptfs_passthrough,"
			   "ecryptfs_cipher=aes,"
			   "ecryptfs_key_bytes=%d,"
			   "ecryptfs_sig=%s,"
			   "smackfsroot=*,smackfsdef=*",
			   ECRYPTFS_MAX_KEY_SIZE, signature);

	rc = ::mount(src.c_str(), src.c_str(), "ecryptfs", MS_NODEV, ecryptfsOpts);
	if (rc != 0) {
		ERROR(runtime::GetSystemErrorMessage());
		return -1;
	}

	return 0;
}

int mountEcryptfsToAll()
{
	return 0;
}

void usage(const std::string& prog)
{
	std::cout << "Usage: " << prog << std::endl
			  << "-a            : Automount" << std::endl
			  << "-g name       : Generate key for zone" << std::endl
			  << "-m name       : Apply filesystem encrytion to zone" << std::endl;
}

int main(int argc, char* argv[])
{
	int opt, index, ret = -1;
	struct option options[] = {
		{"automount", no_argument, 0, 'a'},
		{"generate", required_argument, 0, 'g'},
		{"mount", required_argument, 0, 'm'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "ag:m:", options, &index)) != -1) {
		switch (opt) {
		case 'a':
			ret = mountEcryptfsToAll();
			break;
		case 'g':
			ret = generateKey(optarg);
			break;
		case 'm':
			ret = mountEcryptfs("/home/" + std::string(optarg), optarg);
			break;
		default:
			std::cerr << "unknown" << std::endl;
			usage(argv[0]);
			break;
		}
	}

	if (ret != 0) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
