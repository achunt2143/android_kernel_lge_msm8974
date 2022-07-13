<<<<<<< HEAD
/*
 * Copyright 2008, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2008, Jouni Malinen <j@w1.fi>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef AES_CMAC_H
#define AES_CMAC_H

#include <linux/crypto.h>
<<<<<<< HEAD

struct crypto_cipher * ieee80211_aes_cmac_key_setup(const u8 key[]);
void ieee80211_aes_cmac(struct crypto_cipher *tfm, const u8 *aad,
			const u8 *data, size_t data_len, u8 *mic);
void ieee80211_aes_cmac_key_free(struct crypto_cipher *tfm);
=======
#include <crypto/hash.h>

struct crypto_shash *ieee80211_aes_cmac_key_setup(const u8 key[],
						  size_t key_len);
void ieee80211_aes_cmac(struct crypto_shash *tfm, const u8 *aad,
			const u8 *data, size_t data_len, u8 *mic);
void ieee80211_aes_cmac_256(struct crypto_shash *tfm, const u8 *aad,
			    const u8 *data, size_t data_len, u8 *mic);
void ieee80211_aes_cmac_key_free(struct crypto_shash *tfm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* AES_CMAC_H */
