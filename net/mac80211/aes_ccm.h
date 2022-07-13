<<<<<<< HEAD
/*
 * Copyright 2003-2004, Instant802 Networks, Inc.
 * Copyright 2006, Devicescape Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2003-2004, Instant802 Networks, Inc.
 * Copyright 2006, Devicescape Software, Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef AES_CCM_H
#define AES_CCM_H

<<<<<<< HEAD
#include <linux/crypto.h>

struct crypto_cipher *ieee80211_aes_key_setup_encrypt(const u8 key[]);
void ieee80211_aes_ccm_encrypt(struct crypto_cipher *tfm, u8 *scratch,
			       u8 *data, size_t data_len,
			       u8 *cdata, u8 *mic);
int ieee80211_aes_ccm_decrypt(struct crypto_cipher *tfm, u8 *scratch,
			      u8 *cdata, size_t data_len,
			      u8 *mic, u8 *data);
void ieee80211_aes_key_free(struct crypto_cipher *tfm);
=======
#include "aead_api.h"

#define CCM_AAD_LEN	32

static inline struct crypto_aead *
ieee80211_aes_key_setup_encrypt(const u8 key[], size_t key_len, size_t mic_len)
{
	return aead_key_setup_encrypt("ccm(aes)", key, key_len, mic_len);
}

static inline int
ieee80211_aes_ccm_encrypt(struct crypto_aead *tfm,
			  u8 *b_0, u8 *aad, u8 *data,
			  size_t data_len, u8 *mic)
{
	return aead_encrypt(tfm, b_0, aad + 2,
			    be16_to_cpup((__be16 *)aad),
			    data, data_len, mic);
}

static inline int
ieee80211_aes_ccm_decrypt(struct crypto_aead *tfm,
			  u8 *b_0, u8 *aad, u8 *data,
			  size_t data_len, u8 *mic)
{
	return aead_decrypt(tfm, b_0, aad + 2,
			    be16_to_cpup((__be16 *)aad),
			    data, data_len, mic);
}

static inline void ieee80211_aes_key_free(struct crypto_aead *tfm)
{
	return aead_key_free(tfm);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* AES_CCM_H */
