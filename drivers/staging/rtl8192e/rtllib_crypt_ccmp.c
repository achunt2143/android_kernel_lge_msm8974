<<<<<<< HEAD
/*
 * Host AP crypt: host-based CCMP encryption implementation for Host AP driver
 *
 * Copyright (c) 2003-2004, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. See README and COPYING for
 * more details.
=======
// SPDX-License-Identifier: GPL-2.0
/* Host AP crypt: host-based CCMP encryption implementation for Host AP driver
 *
 * Copyright (c) 2003-2004, Jouni Malinen <jkmaline@cc.hut.fi>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/string.h>
#include <linux/wireless.h>
#include "rtllib.h"

#include <linux/crypto.h>
<<<<<<< HEAD
=======
#include <crypto/aead.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/scatterlist.h>

#define AES_BLOCK_LEN 16
#define CCMP_HDR_LEN 8
#define CCMP_MIC_LEN 8
#define CCMP_TK_LEN 16
#define CCMP_PN_LEN 6

struct rtllib_ccmp_data {
	u8 key[CCMP_TK_LEN];
	int key_set;

	u8 tx_pn[CCMP_PN_LEN];
	u8 rx_pn[CCMP_PN_LEN];

<<<<<<< HEAD
	u32 dot11RSNAStatsCCMPFormatErrors;
	u32 dot11RSNAStatsCCMPReplays;
	u32 dot11RSNAStatsCCMPDecryptErrors;

	int key_idx;

	struct crypto_tfm *tfm;

	/* scratch buffers for virt_to_page() (crypto API) */
	u8 tx_b0[AES_BLOCK_LEN], tx_b[AES_BLOCK_LEN],
		tx_e[AES_BLOCK_LEN], tx_s0[AES_BLOCK_LEN];
	u8 rx_b0[AES_BLOCK_LEN], rx_b[AES_BLOCK_LEN], rx_a[AES_BLOCK_LEN];
};

static void rtllib_ccmp_aes_encrypt(struct crypto_tfm *tfm,
			     const u8 pt[16], u8 ct[16])
{
	crypto_cipher_encrypt_one((void *)tfm, ct, pt);
}

=======
	u32 dot11rsna_stats_ccmp_format_errors;
	u32 dot11rsna_stats_ccmp_replays;
	u32 dot11rsna_stats_ccmp_decrypt_errors;

	int key_idx;

	struct crypto_aead *tfm;

	/* scratch buffers for virt_to_page() (crypto API) */
	u8 tx_aad[2 * AES_BLOCK_LEN];
	u8 rx_aad[2 * AES_BLOCK_LEN];
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void *rtllib_ccmp_init(int key_idx)
{
	struct rtllib_ccmp_data *priv;

	priv = kzalloc(sizeof(*priv), GFP_ATOMIC);
<<<<<<< HEAD
	if (priv == NULL)
		goto fail;
	priv->key_idx = key_idx;

	priv->tfm = (void *)crypto_alloc_cipher("aes", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(priv->tfm)) {
		printk(KERN_DEBUG "rtllib_crypt_ccmp: could not allocate "
		       "crypto API aes\n");
=======
	if (!priv)
		goto fail;
	priv->key_idx = key_idx;

	priv->tfm = crypto_alloc_aead("ccm(aes)", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(priv->tfm)) {
		pr_debug("Could not allocate crypto API aes\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		priv->tfm = NULL;
		goto fail;
	}
	return priv;

fail:
	if (priv) {
		if (priv->tfm)
<<<<<<< HEAD
			crypto_free_cipher((void *)priv->tfm);
=======
			crypto_free_aead(priv->tfm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(priv);
	}

	return NULL;
}

<<<<<<< HEAD

static void rtllib_ccmp_deinit(void *priv)
{
	struct rtllib_ccmp_data *_priv = priv;
	if (_priv && _priv->tfm)
		crypto_free_cipher((void *)_priv->tfm);
	kfree(priv);
}


static inline void xor_block(u8 *b, u8 *a, size_t len)
{
	int i;
	for (i = 0; i < len; i++)
		b[i] ^= a[i];
}



static void ccmp_init_blocks(struct crypto_tfm *tfm,
			     struct rtllib_hdr_4addr *hdr,
			     u8 *pn, size_t dlen, u8 *b0, u8 *auth,
			     u8 *s0)
=======
static void rtllib_ccmp_deinit(void *priv)
{
	struct rtllib_ccmp_data *_priv = priv;

	if (_priv && _priv->tfm)
		crypto_free_aead(_priv->tfm);
	kfree(priv);
}

static int ccmp_init_iv_and_aad(struct ieee80211_hdr *hdr,
				u8 *pn, u8 *iv, u8 *aad)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u8 *pos, qc = 0;
	size_t aad_len;
	u16 fc;
	int a4_included, qc_included;
<<<<<<< HEAD
	u8 aad[2 * AES_BLOCK_LEN];

	fc = le16_to_cpu(hdr->frame_ctl);
	a4_included = ((fc & (RTLLIB_FCTL_TODS | RTLLIB_FCTL_FROMDS)) ==
		       (RTLLIB_FCTL_TODS | RTLLIB_FCTL_FROMDS));
	/*
	qc_included = ((WLAN_FC_GET_TYPE(fc) == RTLLIB_FTYPE_DATA) &&
		       (WLAN_FC_GET_STYPE(fc) & 0x08));
	*/
=======

	fc = le16_to_cpu(hdr->frame_control);
	a4_included = ieee80211_has_a4(hdr->frame_control);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	qc_included = ((WLAN_FC_GET_TYPE(fc) == RTLLIB_FTYPE_DATA) &&
		       (WLAN_FC_GET_STYPE(fc) & 0x80));
	aad_len = 22;
	if (a4_included)
		aad_len += 6;
	if (qc_included) {
<<<<<<< HEAD
		pos = (u8 *) &hdr->addr4;
=======
		pos = (u8 *)&hdr->addr4;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (a4_included)
			pos += 6;
		qc = *pos & 0x0f;
		aad_len += 2;
	}
<<<<<<< HEAD
	/* CCM Initial Block:
	 * Flag (Include authentication header, M=3 (8-octet MIC),
	 *       L=1 (2-octet Dlen))
	 * Nonce: 0x00 | A2 | PN
	 * Dlen */
	b0[0] = 0x59;
	b0[1] = qc;
	memcpy(b0 + 2, hdr->addr2, ETH_ALEN);
	memcpy(b0 + 8, pn, CCMP_PN_LEN);
	b0[14] = (dlen >> 8) & 0xff;
	b0[15] = dlen & 0xff;
=======
	/* In CCM, the initial vectors (IV) used for CTR mode encryption and CBC
	 * mode authentication are not allowed to collide, yet both are derived
	 * from the same vector. We only set L := 1 here to indicate that the
	 * data size can be represented in (L+1) bytes. The CCM layer will take
	 * care of storing the data length in the top (L+1) bytes and setting
	 * and clearing the other bits as is required to derive the two IVs.
	 */
	iv[0] = 0x1;

	/* Nonce: QC | A2 | PN */
	iv[1] = qc;
	memcpy(iv + 2, hdr->addr2, ETH_ALEN);
	memcpy(iv + 8, pn, CCMP_PN_LEN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* AAD:
	 * FC with bits 4..6 and 11..13 masked to zero; 14 is always one
	 * A1 | A2 | A3
	 * SC with bits 4..15 (seq#) masked to zero
	 * A4 (if present)
	 * QC (if present)
	 */
<<<<<<< HEAD
	pos = (u8 *) hdr;
	aad[0] = 0; /* aad_len >> 8 */
	aad[1] = aad_len & 0xff;
	aad[2] = pos[0] & 0x8f;
	aad[3] = pos[1] & 0xc7;
	memcpy(aad + 4, hdr->addr1, 3 * ETH_ALEN);
	pos = (u8 *) &hdr->seq_ctl;
	aad[22] = pos[0] & 0x0f;
	aad[23] = 0; /* all bits masked */
	memset(aad + 24, 0, 8);
	if (a4_included)
		memcpy(aad + 24, hdr->addr4, ETH_ALEN);
	if (qc_included) {
		aad[a4_included ? 30 : 24] = qc;
		/* rest of QC masked */
	}

	/* Start with the first block and AAD */
	rtllib_ccmp_aes_encrypt(tfm, b0, auth);
	xor_block(auth, aad, AES_BLOCK_LEN);
	rtllib_ccmp_aes_encrypt(tfm, auth, auth);
	xor_block(auth, &aad[AES_BLOCK_LEN], AES_BLOCK_LEN);
	rtllib_ccmp_aes_encrypt(tfm, auth, auth);
	b0[0] &= 0x07;
	b0[14] = b0[15] = 0;
	rtllib_ccmp_aes_encrypt(tfm, b0, s0);
}



static int rtllib_ccmp_encrypt(struct sk_buff *skb, int hdr_len, void *priv)
{
	struct rtllib_ccmp_data *key = priv;
	int data_len, i;
	u8 *pos;
	struct rtllib_hdr_4addr *hdr;
=======
	pos = (u8 *)hdr;
	aad[0] = pos[0] & 0x8f;
	aad[1] = pos[1] & 0xc7;
	memcpy(&aad[2], &hdr->addr1, ETH_ALEN);
	memcpy(&aad[8], &hdr->addr2, ETH_ALEN);
	memcpy(&aad[14], &hdr->addr3, ETH_ALEN);
	pos = (u8 *)&hdr->seq_ctrl;
	aad[20] = pos[0] & 0x0f;
	aad[21] = 0; /* all bits masked */
	memset(aad + 22, 0, 8);
	if (a4_included)
		memcpy(aad + 22, hdr->addr4, ETH_ALEN);
	if (qc_included) {
		aad[a4_included ? 28 : 22] = qc;
		/* rest of QC masked */
	}

	return aad_len;
}

static int rtllib_ccmp_encrypt(struct sk_buff *skb, int hdr_len, void *priv)
{
	struct rtllib_ccmp_data *key = priv;
	int i;
	u8 *pos;
	struct ieee80211_hdr *hdr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct cb_desc *tcb_desc = (struct cb_desc *)(skb->cb +
				    MAX_DEV_ADDR_SIZE);
	if (skb_headroom(skb) < CCMP_HDR_LEN ||
	    skb_tailroom(skb) < CCMP_MIC_LEN ||
	    skb->len < hdr_len)
		return -1;

<<<<<<< HEAD
	data_len = skb->len - hdr_len;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pos = skb_push(skb, CCMP_HDR_LEN);
	memmove(pos, pos + CCMP_HDR_LEN, hdr_len);
	pos += hdr_len;

	i = CCMP_PN_LEN - 1;
	while (i >= 0) {
		key->tx_pn[i]++;
		if (key->tx_pn[i] != 0)
			break;
		i--;
	}

	*pos++ = key->tx_pn[5];
	*pos++ = key->tx_pn[4];
	*pos++ = 0;
	*pos++ = (key->key_idx << 6) | (1 << 5) /* Ext IV included */;
	*pos++ = key->tx_pn[3];
	*pos++ = key->tx_pn[2];
	*pos++ = key->tx_pn[1];
	*pos++ = key->tx_pn[0];

<<<<<<< HEAD

	hdr = (struct rtllib_hdr_4addr *) skb->data;
	if (!tcb_desc->bHwSec) {
		int blocks, last, len;
		u8 *mic;
		u8 *b0 = key->tx_b0;
		u8 *b = key->tx_b;
		u8 *e = key->tx_e;
		u8 *s0 = key->tx_s0;

		mic = skb_put(skb, CCMP_MIC_LEN);

		ccmp_init_blocks(key->tfm, hdr, key->tx_pn, data_len,
				 b0, b, s0);

		blocks = (data_len + AES_BLOCK_LEN - 1) / AES_BLOCK_LEN;
		last = data_len % AES_BLOCK_LEN;

		for (i = 1; i <= blocks; i++) {
			len = (i == blocks && last) ? last : AES_BLOCK_LEN;
			/* Authentication */
			xor_block(b, pos, len);
			rtllib_ccmp_aes_encrypt(key->tfm, b, b);
			/* Encryption, with counter */
			b0[14] = (i >> 8) & 0xff;
			b0[15] = i & 0xff;
			rtllib_ccmp_aes_encrypt(key->tfm, b0, e);
			xor_block(pos, e, len);
			pos += len;
		}

		for (i = 0; i < CCMP_MIC_LEN; i++)
			mic[i] = b[i] ^ s0[i];
	}
	return 0;
}


=======
	hdr = (struct ieee80211_hdr *)skb->data;
	if (!tcb_desc->bHwSec) {
		struct aead_request *req;
		struct scatterlist sg[2];
		u8 *aad = key->tx_aad;
		u8 iv[AES_BLOCK_LEN];
		int aad_len, ret;
		int data_len = skb->len - hdr_len - CCMP_HDR_LEN;

		req = aead_request_alloc(key->tfm, GFP_ATOMIC);
		if (!req)
			return -ENOMEM;

		aad_len = ccmp_init_iv_and_aad(hdr, key->tx_pn, iv, aad);

		skb_put(skb, CCMP_MIC_LEN);
		sg_init_table(sg, 2);
		sg_set_buf(&sg[0], aad, aad_len);
		sg_set_buf(&sg[1], skb->data + hdr_len + CCMP_HDR_LEN,
			   data_len + CCMP_MIC_LEN);

		aead_request_set_callback(req, 0, NULL, NULL);
		aead_request_set_ad(req, aad_len);
		aead_request_set_crypt(req, sg, sg, data_len, iv);

		ret = crypto_aead_encrypt(req);
		aead_request_free(req);

		return ret;
	}

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int rtllib_ccmp_decrypt(struct sk_buff *skb, int hdr_len, void *priv)
{
	struct rtllib_ccmp_data *key = priv;
	u8 keyidx, *pos;
<<<<<<< HEAD
	struct rtllib_hdr_4addr *hdr;
=======
	struct ieee80211_hdr *hdr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct cb_desc *tcb_desc = (struct cb_desc *)(skb->cb +
				    MAX_DEV_ADDR_SIZE);
	u8 pn[6];

	if (skb->len < hdr_len + CCMP_HDR_LEN + CCMP_MIC_LEN) {
<<<<<<< HEAD
		key->dot11RSNAStatsCCMPFormatErrors++;
		return -1;
	}

	hdr = (struct rtllib_hdr_4addr *) skb->data;
=======
		key->dot11rsna_stats_ccmp_format_errors++;
		return -1;
	}

	hdr = (struct ieee80211_hdr *)skb->data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pos = skb->data + hdr_len;
	keyidx = pos[3];
	if (!(keyidx & (1 << 5))) {
		if (net_ratelimit()) {
<<<<<<< HEAD
			printk(KERN_DEBUG "CCMP: received packet without ExtIV"
			       " flag from %pM\n", hdr->addr2);
		}
		key->dot11RSNAStatsCCMPFormatErrors++;
=======
			pr_debug("CCMP: received packet without ExtIV flag from %pM\n",
				 hdr->addr2);
		}
		key->dot11rsna_stats_ccmp_format_errors++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -2;
	}
	keyidx >>= 6;
	if (key->key_idx != keyidx) {
<<<<<<< HEAD
		printk(KERN_DEBUG "CCMP: RX tkey->key_idx=%d frame "
		       "keyidx=%d priv=%p\n", key->key_idx, keyidx, priv);
=======
		pr_debug("CCMP: RX tkey->key_idx=%d frame keyidx=%d priv=%p\n",
			 key->key_idx, keyidx, priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -6;
	}
	if (!key->key_set) {
		if (net_ratelimit()) {
<<<<<<< HEAD
			printk(KERN_DEBUG "CCMP: received packet from %pM"
			       " with keyid=%d that does not have a configured"
			       " key\n", hdr->addr2, keyidx);
=======
			pr_debug("CCMP: received packet from %pM with keyid=%d that does not have a configured key\n",
				 hdr->addr2, keyidx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		return -3;
	}

	pn[0] = pos[7];
	pn[1] = pos[6];
	pn[2] = pos[5];
	pn[3] = pos[4];
	pn[4] = pos[1];
	pn[5] = pos[0];
	pos += 8;
	if (memcmp(pn, key->rx_pn, CCMP_PN_LEN) <= 0) {
<<<<<<< HEAD
		key->dot11RSNAStatsCCMPReplays++;
		return -4;
	}
	if (!tcb_desc->bHwSec) {
		size_t data_len = skb->len - hdr_len - CCMP_HDR_LEN -
				  CCMP_MIC_LEN;
		u8 *mic = skb->data + skb->len - CCMP_MIC_LEN;
		u8 *b0 = key->rx_b0;
		u8 *b = key->rx_b;
		u8 *a = key->rx_a;
		int i, blocks, last, len;


		ccmp_init_blocks(key->tfm, hdr, pn, data_len, b0, a, b);
		xor_block(mic, b, CCMP_MIC_LEN);

		blocks = (data_len + AES_BLOCK_LEN - 1) / AES_BLOCK_LEN;
		last = data_len % AES_BLOCK_LEN;

		for (i = 1; i <= blocks; i++) {
			len = (i == blocks && last) ? last : AES_BLOCK_LEN;
			/* Decrypt, with counter */
			b0[14] = (i >> 8) & 0xff;
			b0[15] = i & 0xff;
			rtllib_ccmp_aes_encrypt(key->tfm, b0, b);
			xor_block(pos, b, len);
			/* Authentication */
			xor_block(a, pos, len);
			rtllib_ccmp_aes_encrypt(key->tfm, a, a);
			pos += len;
		}

		if (memcmp(mic, a, CCMP_MIC_LEN) != 0) {
			if (net_ratelimit()) {
				printk(KERN_DEBUG "CCMP: decrypt failed: STA="
				" %pM\n", hdr->addr2);
			}
			key->dot11RSNAStatsCCMPDecryptErrors++;
=======
		key->dot11rsna_stats_ccmp_replays++;
		return -4;
	}
	if (!tcb_desc->bHwSec) {
		size_t data_len = skb->len - hdr_len - CCMP_HDR_LEN;
		struct aead_request *req;
		struct scatterlist sg[2];
		u8 *aad = key->rx_aad;
		u8 iv[AES_BLOCK_LEN];
		int aad_len, ret;

		req = aead_request_alloc(key->tfm, GFP_ATOMIC);
		if (!req)
			return -ENOMEM;

		aad_len = ccmp_init_iv_and_aad(hdr, pn, iv, aad);

		sg_init_table(sg, 2);
		sg_set_buf(&sg[0], aad, aad_len);
		sg_set_buf(&sg[1], pos, data_len);

		aead_request_set_callback(req, 0, NULL, NULL);
		aead_request_set_ad(req, aad_len);
		aead_request_set_crypt(req, sg, sg, data_len, iv);

		ret = crypto_aead_decrypt(req);
		aead_request_free(req);

		if (ret) {
			if (net_ratelimit()) {
				pr_debug("CCMP: decrypt failed: STA= %pM\n",
					 hdr->addr2);
			}
			key->dot11rsna_stats_ccmp_decrypt_errors++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -5;
		}

		memcpy(key->rx_pn, pn, CCMP_PN_LEN);
	}
	/* Remove hdr and MIC */
	memmove(skb->data + CCMP_HDR_LEN, skb->data, hdr_len);
	skb_pull(skb, CCMP_HDR_LEN);
	skb_trim(skb, skb->len - CCMP_MIC_LEN);

	return keyidx;
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int rtllib_ccmp_set_key(void *key, int len, u8 *seq, void *priv)
{
	struct rtllib_ccmp_data *data = priv;
	int keyidx;
<<<<<<< HEAD
	struct crypto_tfm *tfm = data->tfm;
=======
	struct crypto_aead *tfm = data->tfm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	keyidx = data->key_idx;
	memset(data, 0, sizeof(*data));
	data->key_idx = keyidx;
	data->tfm = tfm;
	if (len == CCMP_TK_LEN) {
		memcpy(data->key, key, CCMP_TK_LEN);
		data->key_set = 1;
		if (seq) {
			data->rx_pn[0] = seq[5];
			data->rx_pn[1] = seq[4];
			data->rx_pn[2] = seq[3];
			data->rx_pn[3] = seq[2];
			data->rx_pn[4] = seq[1];
			data->rx_pn[5] = seq[0];
		}
<<<<<<< HEAD
		crypto_cipher_setkey((void *)data->tfm, data->key, CCMP_TK_LEN);
	} else if (len == 0)
		data->key_set = 0;
	else
		return -1;
=======
		if (crypto_aead_setauthsize(data->tfm, CCMP_MIC_LEN) ||
		    crypto_aead_setkey(data->tfm, data->key, CCMP_TK_LEN))
			return -1;
	} else if (len == 0) {
		data->key_set = 0;
	} else {
		return -1;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int rtllib_ccmp_get_key(void *key, int len, u8 *seq, void *priv)
{
	struct rtllib_ccmp_data *data = priv;

	if (len < CCMP_TK_LEN)
		return -1;

	if (!data->key_set)
		return 0;
	memcpy(key, data->key, CCMP_TK_LEN);

	if (seq) {
		seq[0] = data->tx_pn[5];
		seq[1] = data->tx_pn[4];
		seq[2] = data->tx_pn[3];
		seq[3] = data->tx_pn[2];
		seq[4] = data->tx_pn[1];
		seq[5] = data->tx_pn[0];
	}

	return CCMP_TK_LEN;
}

<<<<<<< HEAD

static char *rtllib_ccmp_print_stats(char *p, void *priv)
{
	struct rtllib_ccmp_data *ccmp = priv;
	p += sprintf(p, "key[%d] alg=CCMP key_set=%d "
		     "tx_pn=%pM rx_pn=%pM "
		     "format_errors=%d replays=%d decrypt_errors=%d\n",
		     ccmp->key_idx, ccmp->key_set,
		     ccmp->tx_pn, ccmp->rx_pn,
		     ccmp->dot11RSNAStatsCCMPFormatErrors,
		     ccmp->dot11RSNAStatsCCMPReplays,
		     ccmp->dot11RSNAStatsCCMPDecryptErrors);

	return p;
=======
static void rtllib_ccmp_print_stats(struct seq_file *m, void *priv)
{
	struct rtllib_ccmp_data *ccmp = priv;

	seq_printf(m,
		   "key[%d] alg=CCMP key_set=%d tx_pn=%pM rx_pn=%pM format_errors=%d replays=%d decrypt_errors=%d\n",
		   ccmp->key_idx, ccmp->key_set,
		   ccmp->tx_pn, ccmp->rx_pn,
		   ccmp->dot11rsna_stats_ccmp_format_errors,
		   ccmp->dot11rsna_stats_ccmp_replays,
		   ccmp->dot11rsna_stats_ccmp_decrypt_errors);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct lib80211_crypto_ops rtllib_crypt_ccmp = {
	.name			= "R-CCMP",
	.init			= rtllib_ccmp_init,
	.deinit			= rtllib_ccmp_deinit,
	.encrypt_mpdu		= rtllib_ccmp_encrypt,
	.decrypt_mpdu		= rtllib_ccmp_decrypt,
	.encrypt_msdu		= NULL,
	.decrypt_msdu		= NULL,
	.set_key		= rtllib_ccmp_set_key,
	.get_key		= rtllib_ccmp_get_key,
	.print_stats		= rtllib_ccmp_print_stats,
	.extra_mpdu_prefix_len = CCMP_HDR_LEN,
	.extra_mpdu_postfix_len = CCMP_MIC_LEN,
	.owner			= THIS_MODULE,
};

<<<<<<< HEAD

int __init rtllib_crypto_ccmp_init(void)
=======
static int __init rtllib_crypto_ccmp_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return lib80211_register_crypto_ops(&rtllib_crypt_ccmp);
}

<<<<<<< HEAD

void __exit rtllib_crypto_ccmp_exit(void)
=======
static void __exit rtllib_crypto_ccmp_exit(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	lib80211_unregister_crypto_ops(&rtllib_crypt_ccmp);
}

module_init(rtllib_crypto_ccmp_init);
module_exit(rtllib_crypto_ccmp_exit);

MODULE_LICENSE("GPL");
