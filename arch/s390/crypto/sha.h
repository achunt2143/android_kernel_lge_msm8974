<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API.
 *
 * s390 generic implementation of the SHA Secure Hash Algorithms.
 *
 * Copyright IBM Corp. 2007
 * Author(s): Jan Glauber (jang@de.ibm.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _CRYPTO_ARCH_S390_SHA_H
#define _CRYPTO_ARCH_S390_SHA_H

#include <linux/crypto.h>
<<<<<<< HEAD
#include <crypto/sha.h>

/* must be big enough for the largest SHA variant */
#define SHA_MAX_STATE_SIZE	16
#define SHA_MAX_BLOCK_SIZE      SHA512_BLOCK_SIZE

struct s390_sha_ctx {
	u64 count;              /* message length in bytes */
	u32 state[SHA_MAX_STATE_SIZE];
	u8 buf[2 * SHA_MAX_BLOCK_SIZE];
=======
#include <crypto/sha1.h>
#include <crypto/sha2.h>
#include <crypto/sha3.h>

/* must be big enough for the largest SHA variant */
#define SHA3_STATE_SIZE			200
#define CPACF_MAX_PARMBLOCK_SIZE	SHA3_STATE_SIZE
#define SHA_MAX_BLOCK_SIZE		SHA3_224_BLOCK_SIZE

struct s390_sha_ctx {
	u64 count;		/* message length in bytes */
	u32 state[CPACF_MAX_PARMBLOCK_SIZE / sizeof(u32)];
	u8 buf[SHA_MAX_BLOCK_SIZE];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int func;		/* KIMD function to use */
};

struct shash_desc;

int s390_sha_update(struct shash_desc *desc, const u8 *data, unsigned int len);
int s390_sha_final(struct shash_desc *desc, u8 *out);

#endif
