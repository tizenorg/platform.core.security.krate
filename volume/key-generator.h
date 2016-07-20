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

#ifndef __VOLUME_MANAGER_KEY_GENERATOR_H__
#define __VOLUME_MANAGER_KEY_GENERATOR_H__

#include "ecryptfs.h"

#define SHA1_DIGEST_SIZE 20

typedef struct {
	unsigned int state[5];
	unsigned int count[2];
	unsigned char buffer[64];
} SHA1_CTX;

class KeyGenerator {
public:
	KeyGenerator() = delete;
	KeyGenerator(const KeyGenerator&) = delete;
	KeyGenerator(KeyGenerator&&) = delete;

	KeyGenerator& operator=(const KeyGenerator&) = delete;
	KeyGenerator& operator=(KeyGenerator&&) = delete;

	static std::string wrapKey(const std::string& decrypted, const std::string& salt, int len);
	static std::string generateKey(int len);
	static void generateToken(char* key, ecryptfs_payload** outToken);

private:
	static void sha1Init(SHA1_CTX* context);
	static void sha1Update(SHA1_CTX* context, const void* p, unsigned int len);
	static void sha1Final(unsigned char digsest[SHA1_DIGEST_SIZE], SHA1_CTX* context);
	static void sha1Transform(unsigned int state[5], const unsigned char buffer[64]);
	static void sha1Hmac(const unsigned char* key, int keyLen, const unsigned char* data, int dataLen, unsigned char out[SHA1_DIGEST_SIZE]);

	static int pbkdf2(const char* pass, int passLen, const unsigned char* salt, int saltLen, int iter, int keyLen, unsigned char* out);
	static std::string hexConvert(unsigned char* src, int srcLen);
};

#endif //!__VOLUME_MANAGER_KEY_GENERATOR_H__
