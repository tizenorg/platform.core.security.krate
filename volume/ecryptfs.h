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

#ifndef __ECRYPTFS_H__
#define __ECRYPTFS_H__

#include <sys/types.h>
#include <errno.h>

// this is versions of ecryptfs module
#define ECRYPTFS_MAJOR_VERSION 0x00
#define ECRYPTFS_MINOR_VERSION 0x04
#define ECRYPTFS_VERSION ((ECRYPTFS_MAJOR_VERSION << 8) | ECRYPTFS_MINOR_VERSION)

#define ECRYPTFS_MAX_PKI_NAME_BYTES             16
#define ECRYPTFS_SESSION_KEY_ENCRYPTION_KEY_SET 0x02

#define PGP_DIGEST_ALGO_SHA512  10

#define ECRYPTFS_FEK_CIPHER     "aes"
#define ECRYPTFS_MOUNT_DEVICE   "ecryptfs"

#define ECRYPTFS_MAX_OPTIONS    1024

#define ECRYPTFS_MAX_SIG_SIZE   8
#define ECRYPTFS_MAX_SIG_HEX (ECRYPTFS_MAX_SIG_SIZE*2)
#define ECRYPTFS_PASSWORD_SIG_SIZE ECRYPTFS_MAX_SIG_HEX

#define ECRYPTFS_MAX_KEY_SIZE               32
#define ECRYPTFS_MAX_KEY_HEX                (ECRYPTFS_MAX_KEY_SIZE * 2)

#define ECRYPTFS_MAX_ENCRYPTED_KEY_BYTES     512
#define ECRYPTFS_MAX_PKI_NAME_BYTES          16

#define ECRYPTFS_MAX_SALT_SIZE               4
#define ECRYPTFS_MAX_SALT_HEX                8

#define ECRYPTFS_PWD_PAYLOAD_TYPE            0 // password

struct ecryptfs_session_key {
#define ECRYPTFS_USERSPACE_SHOULD_TRY_TO_DECRYPT    0x00000001
#define ECRYPTFS_USERSPACE_SHOULD_TRY_TO_ENCRYPT    0x00000002
#define ECRYPTFS_CONTAINS_DECRYPTED_KEY             0x00000004
#define ECRYPTFS_CONTAINS_ENCRYPTED_KEY             0x00000008
	int32_t flags;
	int32_t encrypted_key_size;
	int32_t decrypted_key_size;
	u_int8_t encrypted_key[ECRYPTFS_MAX_ENCRYPTED_KEY_BYTES];
	u_int8_t decrypted_key[ECRYPTFS_MAX_KEY_HEX];
};

struct ecryptfs_password {
	int32_t password_bytes;
	int32_t hash_algo;
	int32_t hash_iterations;
	int32_t session_key_encryption_key_bytes;
#define ECRYPTFS_PERSISTENT_PASSWORD                0x01
#define ECRYPTFS_SESSION_KEY_ENCRYPTION_KEY_SET     0x02
	u_int32_t flags;
	/* Iterated-hash concatenation of salt and passphrase */
	u_int8_t session_key_encryption_key[ECRYPTFS_MAX_KEY_HEX];
	u_int8_t signature[ECRYPTFS_PASSWORD_SIG_SIZE + 1];
	/* Always in expanded hex */
	u_int8_t salt[ECRYPTFS_MAX_SALT_SIZE];
};

enum ecryptfs_token_types {
	ECRYPTFS_PASSWORD,
	ECRYPTFS_PRIVATE_KEY
};

struct ecryptfs_private_key {
	u_int32_t key_size;
	u_int32_t data_len;
	u_int8_t signature[ECRYPTFS_PASSWORD_SIG_SIZE + 1];
	char pki_type[ECRYPTFS_MAX_PKI_NAME_BYTES + 1];
	u_int8_t data[];
};

struct ecryptfs_auth_tok {
	u_int16_t version; /* 8-bit major and 8-bit minor */
	u_int16_t token_type;
#define ECRYPTFS_ENCRYPT_ONLY                       0x00000001
	u_int32_t flags;
	struct ecryptfs_session_key session_key;
	u_int8_t reserved[32];
	union {
		struct ecryptfs_password password;
		struct ecryptfs_private_key private_key;
	} token;
}  __attribute__((packed));

typedef struct ecryptfs_auth_tok ecryptfs_payload;
#endif
