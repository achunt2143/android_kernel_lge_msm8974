<<<<<<< HEAD
/*******************************************************************************
 * This file houses the main functions for the iSCSI CHAP support
 *
 * \u00a9 Copyright 2007-2011 RisingTide Systems LLC.
 *
 * Licensed to the Linux Foundation under the General Public License (GPL) version 2.
 *
 * Author: Nicholas A. Bellinger <nab@linux-iscsi.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 ******************************************************************************/

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/crypto.h>
#include <linux/err.h>
#include <linux/scatterlist.h>

#include "iscsi_target_core.h"
#include "iscsi_target_nego.h"
#include "iscsi_target_auth.h"

static int chap_string_to_hex(unsigned char *dst, unsigned char *src, int len)
{
	int j = DIV_ROUND_UP(len, 2), rc;

	rc = hex2bin(dst, src, j);
	if (rc < 0)
		pr_debug("CHAP string contains non hex digit symbols\n");

	dst[j] = '\0';
	return j;
}

static void chap_binaryhex_to_asciihex(char *dst, char *src, int src_len)
{
	int i;

	for (i = 0; i < src_len; i++) {
		sprintf(&dst[i*2], "%02x", (int) src[i] & 0xff);
	}
}

static void chap_set_random(char *data, int length)
{
	long r;
	unsigned n;

	while (length > 0) {
		get_random_bytes(&r, sizeof(long));
		r = r ^ (r >> 8);
		r = r ^ (r >> 4);
		n = r & 0x7;

		get_random_bytes(&r, sizeof(long));
		r = r ^ (r >> 8);
		r = r ^ (r >> 5);
		n = (n << 3) | (r & 0x7);

		get_random_bytes(&r, sizeof(long));
		r = r ^ (r >> 8);
		r = r ^ (r >> 5);
		n = (n << 2) | (r & 0x3);

		*data++ = n;
		length--;
	}
}

static void chap_gen_challenge(
	struct iscsi_conn *conn,
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*******************************************************************************
 * This file houses the main functions for the iSCSI CHAP support
 *
 * (c) Copyright 2007-2013 Datera, Inc.
 *
 * Author: Nicholas A. Bellinger <nab@linux-iscsi.org>
 *
 ******************************************************************************/

#include <crypto/hash.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/err.h>
#include <linux/random.h>
#include <linux/scatterlist.h>
#include <target/iscsi/iscsi_target_core.h>
#include "iscsi_target_nego.h"
#include "iscsi_target_auth.h"

static char *chap_get_digest_name(const int digest_type)
{
	switch (digest_type) {
	case CHAP_DIGEST_MD5:
		return "md5";
	case CHAP_DIGEST_SHA1:
		return "sha1";
	case CHAP_DIGEST_SHA256:
		return "sha256";
	case CHAP_DIGEST_SHA3_256:
		return "sha3-256";
	default:
		return NULL;
	}
}

static int chap_gen_challenge(
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int caller,
	char *c_str,
	unsigned int *c_len)
{
<<<<<<< HEAD
	unsigned char challenge_asciihex[CHAP_CHALLENGE_LENGTH * 2 + 1];
	struct iscsi_chap *chap = conn->auth_protocol;

	memset(challenge_asciihex, 0, CHAP_CHALLENGE_LENGTH * 2 + 1);

	chap_set_random(chap->challenge, CHAP_CHALLENGE_LENGTH);
	chap_binaryhex_to_asciihex(challenge_asciihex, chap->challenge,
				CHAP_CHALLENGE_LENGTH);
=======
	int ret;
	unsigned char *challenge_asciihex;
	struct iscsi_chap *chap = conn->auth_protocol;

	challenge_asciihex = kzalloc(chap->challenge_len * 2 + 1, GFP_KERNEL);
	if (!challenge_asciihex)
		return -ENOMEM;

	memset(chap->challenge, 0, MAX_CHAP_CHALLENGE_LEN);

	ret = get_random_bytes_wait(chap->challenge, chap->challenge_len);
	if (unlikely(ret))
		goto out;

	bin2hex(challenge_asciihex, chap->challenge,
				chap->challenge_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Set CHAP_C, and copy the generated challenge into c_str.
	 */
	*c_len += sprintf(c_str + *c_len, "CHAP_C=0x%s", challenge_asciihex);
	*c_len += 1;

	pr_debug("[%s] Sending CHAP_C=0x%s\n\n", (caller) ? "server" : "client",
			challenge_asciihex);
<<<<<<< HEAD
}


static struct iscsi_chap *chap_server_open(
	struct iscsi_conn *conn,
=======

out:
	kfree(challenge_asciihex);
	return ret;
}

static int chap_test_algorithm(const char *name)
{
	struct crypto_shash *tfm;

	tfm = crypto_alloc_shash(name, 0, 0);
	if (IS_ERR(tfm))
		return -1;

	crypto_free_shash(tfm);
	return 0;
}

static int chap_check_algorithm(const char *a_str)
{
	char *tmp, *orig, *token, *digest_name;
	long digest_type;
	int r = CHAP_DIGEST_UNKNOWN;

	tmp = kstrdup(a_str, GFP_KERNEL);
	if (!tmp) {
		pr_err("Memory allocation failed for CHAP_A temporary buffer\n");
		return CHAP_DIGEST_UNKNOWN;
	}
	orig = tmp;

	token = strsep(&tmp, "=");
	if (!token)
		goto out;

	if (strcmp(token, "CHAP_A")) {
		pr_err("Unable to locate CHAP_A key\n");
		goto out;
	}
	while (token) {
		token = strsep(&tmp, ",");
		if (!token)
			goto out;

		if (kstrtol(token, 10, &digest_type))
			continue;

		digest_name = chap_get_digest_name(digest_type);
		if (!digest_name)
			continue;

		pr_debug("Selected %s Algorithm\n", digest_name);
		if (chap_test_algorithm(digest_name) < 0) {
			pr_err("failed to allocate %s algo\n", digest_name);
		} else {
			r = digest_type;
			goto out;
		}
	}
out:
	kfree(orig);
	return r;
}

static void chap_close(struct iscsit_conn *conn)
{
	kfree(conn->auth_protocol);
	conn->auth_protocol = NULL;
}

static struct iscsi_chap *chap_server_open(
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_node_auth *auth,
	const char *a_str,
	char *aic_str,
	unsigned int *aic_len)
{
<<<<<<< HEAD
=======
	int digest_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_chap *chap;

	if (!(auth->naf_flags & NAF_USERID_SET) ||
	    !(auth->naf_flags & NAF_PASSWORD_SET)) {
		pr_err("CHAP user or password not set for"
				" Initiator ACL\n");
		return NULL;
	}

	conn->auth_protocol = kzalloc(sizeof(struct iscsi_chap), GFP_KERNEL);
	if (!conn->auth_protocol)
		return NULL;

	chap = conn->auth_protocol;
<<<<<<< HEAD
	/*
	 * We only support MD5 MDA presently.
	 */
	if (strncmp(a_str, "CHAP_A=5", 8)) {
		pr_err("CHAP_A is not MD5.\n");
		return NULL;
	}
	pr_debug("[server] Got CHAP_A=5\n");
	/*
	 * Send back CHAP_A set to MD5.
	 */
	*aic_len = sprintf(aic_str, "CHAP_A=5");
	*aic_len += 1;
	chap->digest_type = CHAP_DIGEST_MD5;
	pr_debug("[server] Sending CHAP_A=%d\n", chap->digest_type);
	/*
	 * Set Identifier.
	 */
	chap->id = ISCSI_TPG_C(conn)->tpg_chap_id++;
=======
	digest_type = chap_check_algorithm(a_str);
	switch (digest_type) {
	case CHAP_DIGEST_MD5:
		chap->digest_size = MD5_SIGNATURE_SIZE;
		break;
	case CHAP_DIGEST_SHA1:
		chap->digest_size = SHA1_SIGNATURE_SIZE;
		break;
	case CHAP_DIGEST_SHA256:
		chap->digest_size = SHA256_SIGNATURE_SIZE;
		break;
	case CHAP_DIGEST_SHA3_256:
		chap->digest_size = SHA3_256_SIGNATURE_SIZE;
		break;
	case CHAP_DIGEST_UNKNOWN:
	default:
		pr_err("Unsupported CHAP_A value\n");
		chap_close(conn);
		return NULL;
	}

	chap->digest_name = chap_get_digest_name(digest_type);

	/* Tie the challenge length to the digest size */
	chap->challenge_len = chap->digest_size;

	pr_debug("[server] Got CHAP_A=%d\n", digest_type);
	*aic_len = sprintf(aic_str, "CHAP_A=%d", digest_type);
	*aic_len += 1;
	pr_debug("[server] Sending CHAP_A=%d\n", digest_type);

	/*
	 * Set Identifier.
	 */
	chap->id = conn->tpg->tpg_chap_id++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*aic_len += sprintf(aic_str + *aic_len, "CHAP_I=%d", chap->id);
	*aic_len += 1;
	pr_debug("[server] Sending CHAP_I=%d\n", chap->id);
	/*
	 * Generate Challenge.
	 */
<<<<<<< HEAD
	chap_gen_challenge(conn, 1, aic_str, aic_len);
=======
	if (chap_gen_challenge(conn, 1, aic_str, aic_len) < 0) {
		chap_close(conn);
		return NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return chap;
}

<<<<<<< HEAD
static void chap_close(struct iscsi_conn *conn)
{
	kfree(conn->auth_protocol);
	conn->auth_protocol = NULL;
}

static int chap_server_compute_md5(
	struct iscsi_conn *conn,
=======
static const char base64_lookup_table[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int chap_base64_decode(u8 *dst, const char *src, size_t len)
{
	int i, bits = 0, ac = 0;
	const char *p;
	u8 *cp = dst;

	for (i = 0; i < len; i++) {
		if (src[i] == '=')
			return cp - dst;

		p = strchr(base64_lookup_table, src[i]);
		if (p == NULL || src[i] == 0)
			return -2;

		ac <<= 6;
		ac += (p - base64_lookup_table);
		bits += 6;
		if (bits >= 8) {
			*cp++ = (ac >> (bits - 8)) & 0xff;
			ac &= ~(BIT(16) - BIT(bits - 8));
			bits -= 8;
		}
	}
	if (ac)
		return -1;

	return cp - dst;
}

static int chap_server_compute_hash(
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_node_auth *auth,
	char *nr_in_ptr,
	char *nr_out_ptr,
	unsigned int *nr_out_len)
{
<<<<<<< HEAD
	char *endptr;
	unsigned long id;
	unsigned char id_as_uchar;
	unsigned char digest[MD5_SIGNATURE_SIZE];
	unsigned char type, response[MD5_SIGNATURE_SIZE * 2 + 2];
	unsigned char identifier[10], *challenge = NULL;
	unsigned char *challenge_binhex = NULL;
	unsigned char client_digest[MD5_SIGNATURE_SIZE];
	unsigned char server_digest[MD5_SIGNATURE_SIZE];
	unsigned char chap_n[MAX_CHAP_N_SIZE], chap_r[MAX_RESPONSE_LENGTH];
	size_t compare_len;
	struct iscsi_chap *chap = conn->auth_protocol;
	struct crypto_hash *tfm;
	struct hash_desc desc;
	struct scatterlist sg;
	int auth_ret = -1, ret, challenge_len;
=======
	unsigned long id;
	unsigned char id_as_uchar;
	unsigned char type;
	unsigned char identifier[10], *initiatorchg = NULL;
	unsigned char *initiatorchg_binhex = NULL;
	unsigned char *digest = NULL;
	unsigned char *response = NULL;
	unsigned char *client_digest = NULL;
	unsigned char *server_digest = NULL;
	unsigned char chap_n[MAX_CHAP_N_SIZE], chap_r[MAX_RESPONSE_LENGTH];
	size_t compare_len;
	struct iscsi_chap *chap = conn->auth_protocol;
	struct crypto_shash *tfm = NULL;
	struct shash_desc *desc = NULL;
	int auth_ret = -1, ret, initiatorchg_len;

	digest = kzalloc(chap->digest_size, GFP_KERNEL);
	if (!digest) {
		pr_err("Unable to allocate the digest buffer\n");
		goto out;
	}

	response = kzalloc(chap->digest_size * 2 + 2, GFP_KERNEL);
	if (!response) {
		pr_err("Unable to allocate the response buffer\n");
		goto out;
	}

	client_digest = kzalloc(chap->digest_size, GFP_KERNEL);
	if (!client_digest) {
		pr_err("Unable to allocate the client_digest buffer\n");
		goto out;
	}

	server_digest = kzalloc(chap->digest_size, GFP_KERNEL);
	if (!server_digest) {
		pr_err("Unable to allocate the server_digest buffer\n");
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memset(identifier, 0, 10);
	memset(chap_n, 0, MAX_CHAP_N_SIZE);
	memset(chap_r, 0, MAX_RESPONSE_LENGTH);
<<<<<<< HEAD
	memset(digest, 0, MD5_SIGNATURE_SIZE);
	memset(response, 0, MD5_SIGNATURE_SIZE * 2 + 2);
	memset(client_digest, 0, MD5_SIGNATURE_SIZE);
	memset(server_digest, 0, MD5_SIGNATURE_SIZE);

	challenge = kzalloc(CHAP_CHALLENGE_STR_LEN, GFP_KERNEL);
	if (!challenge) {
=======

	initiatorchg = kzalloc(CHAP_CHALLENGE_STR_LEN, GFP_KERNEL);
	if (!initiatorchg) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pr_err("Unable to allocate challenge buffer\n");
		goto out;
	}

<<<<<<< HEAD
	challenge_binhex = kzalloc(CHAP_CHALLENGE_STR_LEN, GFP_KERNEL);
	if (!challenge_binhex) {
		pr_err("Unable to allocate challenge_binhex buffer\n");
=======
	initiatorchg_binhex = kzalloc(CHAP_CHALLENGE_STR_LEN, GFP_KERNEL);
	if (!initiatorchg_binhex) {
		pr_err("Unable to allocate initiatorchg_binhex buffer\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}
	/*
	 * Extract CHAP_N.
	 */
	if (extract_param(nr_in_ptr, "CHAP_N", MAX_CHAP_N_SIZE, chap_n,
				&type) < 0) {
		pr_err("Could not find CHAP_N.\n");
		goto out;
	}
	if (type == HEX) {
		pr_err("Could not find CHAP_N.\n");
		goto out;
	}

	/* Include the terminating NULL in the compare */
	compare_len = strlen(auth->userid) + 1;
	if (strncmp(chap_n, auth->userid, compare_len) != 0) {
		pr_err("CHAP_N values do not match!\n");
		goto out;
	}
	pr_debug("[server] Got CHAP_N=%s\n", chap_n);
	/*
	 * Extract CHAP_R.
	 */
	if (extract_param(nr_in_ptr, "CHAP_R", MAX_RESPONSE_LENGTH, chap_r,
				&type) < 0) {
		pr_err("Could not find CHAP_R.\n");
		goto out;
	}
<<<<<<< HEAD
	if (type != HEX) {
		pr_err("Could not find CHAP_R.\n");
=======

	switch (type) {
	case HEX:
		if (strlen(chap_r) != chap->digest_size * 2) {
			pr_err("Malformed CHAP_R\n");
			goto out;
		}
		if (hex2bin(client_digest, chap_r, chap->digest_size) < 0) {
			pr_err("Malformed CHAP_R: invalid HEX\n");
			goto out;
		}
		break;
	case BASE64:
		if (chap_base64_decode(client_digest, chap_r, strlen(chap_r)) !=
		    chap->digest_size) {
			pr_err("Malformed CHAP_R: invalid BASE64\n");
			goto out;
		}
		break;
	default:
		pr_err("Could not find CHAP_R\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	pr_debug("[server] Got CHAP_R=%s\n", chap_r);
<<<<<<< HEAD
	chap_string_to_hex(client_digest, chap_r, strlen(chap_r));

	tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(tfm)) {
		pr_err("Unable to allocate struct crypto_hash\n");
		goto out;
	}
	desc.tfm = tfm;
	desc.flags = 0;

	ret = crypto_hash_init(&desc);
	if (ret < 0) {
		pr_err("crypto_hash_init() failed\n");
		crypto_free_hash(tfm);
		goto out;
	}

	sg_init_one(&sg, &chap->id, 1);
	ret = crypto_hash_update(&desc, &sg, 1);
	if (ret < 0) {
		pr_err("crypto_hash_update() failed for id\n");
		crypto_free_hash(tfm);
		goto out;
	}

	sg_init_one(&sg, &auth->password, strlen(auth->password));
	ret = crypto_hash_update(&desc, &sg, strlen(auth->password));
	if (ret < 0) {
		pr_err("crypto_hash_update() failed for password\n");
		crypto_free_hash(tfm);
		goto out;
	}

	sg_init_one(&sg, chap->challenge, CHAP_CHALLENGE_LENGTH);
	ret = crypto_hash_update(&desc, &sg, CHAP_CHALLENGE_LENGTH);
	if (ret < 0) {
		pr_err("crypto_hash_update() failed for challenge\n");
		crypto_free_hash(tfm);
		goto out;
	}

	ret = crypto_hash_final(&desc, server_digest);
	if (ret < 0) {
		pr_err("crypto_hash_final() failed for server digest\n");
		crypto_free_hash(tfm);
		goto out;
	}
	crypto_free_hash(tfm);

	chap_binaryhex_to_asciihex(response, server_digest, MD5_SIGNATURE_SIZE);
	pr_debug("[server] MD5 Server Digest: %s\n", response);

	if (memcmp(server_digest, client_digest, MD5_SIGNATURE_SIZE) != 0) {
		pr_debug("[server] MD5 Digests do not match!\n\n");
		goto out;
	} else
		pr_debug("[server] MD5 Digests match, CHAP connetication"
				" successful.\n\n");
=======

	tfm = crypto_alloc_shash(chap->digest_name, 0, 0);
	if (IS_ERR(tfm)) {
		tfm = NULL;
		pr_err("Unable to allocate struct crypto_shash\n");
		goto out;
	}

	desc = kmalloc(sizeof(*desc) + crypto_shash_descsize(tfm), GFP_KERNEL);
	if (!desc) {
		pr_err("Unable to allocate struct shash_desc\n");
		goto out;
	}

	desc->tfm = tfm;

	ret = crypto_shash_init(desc);
	if (ret < 0) {
		pr_err("crypto_shash_init() failed\n");
		goto out;
	}

	ret = crypto_shash_update(desc, &chap->id, 1);
	if (ret < 0) {
		pr_err("crypto_shash_update() failed for id\n");
		goto out;
	}

	ret = crypto_shash_update(desc, (char *)&auth->password,
				  strlen(auth->password));
	if (ret < 0) {
		pr_err("crypto_shash_update() failed for password\n");
		goto out;
	}

	ret = crypto_shash_finup(desc, chap->challenge,
				 chap->challenge_len, server_digest);
	if (ret < 0) {
		pr_err("crypto_shash_finup() failed for challenge\n");
		goto out;
	}

	bin2hex(response, server_digest, chap->digest_size);
	pr_debug("[server] %s Server Digest: %s\n",
		chap->digest_name, response);

	if (memcmp(server_digest, client_digest, chap->digest_size) != 0) {
		pr_debug("[server] %s Digests do not match!\n\n",
			chap->digest_name);
		goto out;
	} else
		pr_debug("[server] %s Digests match, CHAP connection"
				" successful.\n\n", chap->digest_name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * One way authentication has succeeded, return now if mutual
	 * authentication is not enabled.
	 */
	if (!auth->authenticate_target) {
<<<<<<< HEAD
		kfree(challenge);
		kfree(challenge_binhex);
		return 0;
=======
		auth_ret = 0;
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/*
	 * Get CHAP_I.
	 */
<<<<<<< HEAD
	if (extract_param(nr_in_ptr, "CHAP_I", 10, identifier, &type) < 0) {
=======
	ret = extract_param(nr_in_ptr, "CHAP_I", 10, identifier, &type);
	if (ret == -ENOENT) {
		pr_debug("Could not find CHAP_I. Initiator uses One way authentication.\n");
		auth_ret = 0;
		goto out;
	}
	if (ret < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pr_err("Could not find CHAP_I.\n");
		goto out;
	}

	if (type == HEX)
<<<<<<< HEAD
		id = simple_strtoul(&identifier[2], &endptr, 0);
	else
		id = simple_strtoul(identifier, &endptr, 0);
=======
		ret = kstrtoul(&identifier[2], 0, &id);
	else
		ret = kstrtoul(identifier, 0, &id);

	if (ret < 0) {
		pr_err("kstrtoul() failed for CHAP identifier: %d\n", ret);
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (id > 255) {
		pr_err("chap identifier: %lu greater than 255\n", id);
		goto out;
	}
	/*
	 * RFC 1994 says Identifier is no more than octet (8 bits).
	 */
	pr_debug("[server] Got CHAP_I=%lu\n", id);
	/*
	 * Get CHAP_C.
	 */
	if (extract_param(nr_in_ptr, "CHAP_C", CHAP_CHALLENGE_STR_LEN,
<<<<<<< HEAD
			challenge, &type) < 0) {
=======
			initiatorchg, &type) < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pr_err("Could not find CHAP_C.\n");
		goto out;
	}

<<<<<<< HEAD
	if (type != HEX) {
		pr_err("Could not find CHAP_C.\n");
		goto out;
	}
	pr_debug("[server] Got CHAP_C=%s\n", challenge);
	challenge_len = chap_string_to_hex(challenge_binhex, challenge,
				strlen(challenge));
	if (!challenge_len) {
		pr_err("Unable to convert incoming challenge\n");
		goto out;
	}
=======
	switch (type) {
	case HEX:
		initiatorchg_len = DIV_ROUND_UP(strlen(initiatorchg), 2);
		if (!initiatorchg_len) {
			pr_err("Unable to convert incoming challenge\n");
			goto out;
		}
		if (initiatorchg_len > 1024) {
			pr_err("CHAP_C exceeds maximum binary size of 1024 bytes\n");
			goto out;
		}

		if (hex2bin(initiatorchg_binhex, initiatorchg,
			    initiatorchg_len) < 0) {
			pr_err("Malformed CHAP_C: invalid HEX\n");
			goto out;
		}
		break;
	case BASE64:
		initiatorchg_len = chap_base64_decode(initiatorchg_binhex,
						      initiatorchg,
						      strlen(initiatorchg));
		if (initiatorchg_len < 0) {
			pr_err("Malformed CHAP_C: invalid BASE64\n");
			goto out;
		}
		if (!initiatorchg_len) {
			pr_err("Unable to convert incoming challenge\n");
			goto out;
		}
		if (initiatorchg_len > 1024) {
			pr_err("CHAP_C exceeds maximum binary size of 1024 bytes\n");
			goto out;
		}
		break;
	default:
		pr_err("Could not find CHAP_C.\n");
		goto out;
	}

	pr_debug("[server] Got CHAP_C=%s\n", initiatorchg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * During mutual authentication, the CHAP_C generated by the
	 * initiator must not match the original CHAP_C generated by
	 * the target.
	 */
<<<<<<< HEAD
	if (!memcmp(challenge_binhex, chap->challenge, CHAP_CHALLENGE_LENGTH)) {
=======
	if (initiatorchg_len == chap->challenge_len &&
				!memcmp(initiatorchg_binhex, chap->challenge,
				initiatorchg_len)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pr_err("initiator CHAP_C matches target CHAP_C, failing"
		       " login attempt\n");
		goto out;
	}
	/*
	 * Generate CHAP_N and CHAP_R for mutual authentication.
	 */
<<<<<<< HEAD
	tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(tfm)) {
		pr_err("Unable to allocate struct crypto_hash\n");
		goto out;
	}
	desc.tfm = tfm;
	desc.flags = 0;

	ret = crypto_hash_init(&desc);
	if (ret < 0) {
		pr_err("crypto_hash_init() failed\n");
		crypto_free_hash(tfm);
=======
	ret = crypto_shash_init(desc);
	if (ret < 0) {
		pr_err("crypto_shash_init() failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/* To handle both endiannesses */
	id_as_uchar = id;
<<<<<<< HEAD
	sg_init_one(&sg, &id_as_uchar, 1);
	ret = crypto_hash_update(&desc, &sg, 1);
	if (ret < 0) {
		pr_err("crypto_hash_update() failed for id\n");
		crypto_free_hash(tfm);
		goto out;
	}

	sg_init_one(&sg, auth->password_mutual,
				strlen(auth->password_mutual));
	ret = crypto_hash_update(&desc, &sg, strlen(auth->password_mutual));
	if (ret < 0) {
		pr_err("crypto_hash_update() failed for"
				" password_mutual\n");
		crypto_free_hash(tfm);
=======
	ret = crypto_shash_update(desc, &id_as_uchar, 1);
	if (ret < 0) {
		pr_err("crypto_shash_update() failed for id\n");
		goto out;
	}

	ret = crypto_shash_update(desc, auth->password_mutual,
				  strlen(auth->password_mutual));
	if (ret < 0) {
		pr_err("crypto_shash_update() failed for"
				" password_mutual\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}
	/*
	 * Convert received challenge to binary hex.
	 */
<<<<<<< HEAD
	sg_init_one(&sg, challenge_binhex, challenge_len);
	ret = crypto_hash_update(&desc, &sg, challenge_len);
	if (ret < 0) {
		pr_err("crypto_hash_update() failed for ma challenge\n");
		crypto_free_hash(tfm);
		goto out;
	}

	ret = crypto_hash_final(&desc, digest);
	if (ret < 0) {
		pr_err("crypto_hash_final() failed for ma digest\n");
		crypto_free_hash(tfm);
		goto out;
	}
	crypto_free_hash(tfm);
=======
	ret = crypto_shash_finup(desc, initiatorchg_binhex, initiatorchg_len,
				 digest);
	if (ret < 0) {
		pr_err("crypto_shash_finup() failed for ma challenge\n");
		goto out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Generate CHAP_N and CHAP_R.
	 */
	*nr_out_len = sprintf(nr_out_ptr, "CHAP_N=%s", auth->userid_mutual);
	*nr_out_len += 1;
	pr_debug("[server] Sending CHAP_N=%s\n", auth->userid_mutual);
	/*
	 * Convert response from binary hex to ascii hext.
	 */
<<<<<<< HEAD
	chap_binaryhex_to_asciihex(response, digest, MD5_SIGNATURE_SIZE);
=======
	bin2hex(response, digest, chap->digest_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*nr_out_len += sprintf(nr_out_ptr + *nr_out_len, "CHAP_R=0x%s",
			response);
	*nr_out_len += 1;
	pr_debug("[server] Sending CHAP_R=0x%s\n", response);
	auth_ret = 0;
out:
<<<<<<< HEAD
	kfree(challenge);
	kfree(challenge_binhex);
	return auth_ret;
}

static int chap_got_response(
	struct iscsi_conn *conn,
	struct iscsi_node_auth *auth,
	char *nr_in_ptr,
	char *nr_out_ptr,
	unsigned int *nr_out_len)
{
	struct iscsi_chap *chap = conn->auth_protocol;

	switch (chap->digest_type) {
	case CHAP_DIGEST_MD5:
		if (chap_server_compute_md5(conn, auth, nr_in_ptr,
				nr_out_ptr, nr_out_len) < 0)
			return -1;
		return 0;
	default:
		pr_err("Unknown CHAP digest type %d!\n",
				chap->digest_type);
		return -1;
	}
}

u32 chap_main_loop(
	struct iscsi_conn *conn,
=======
	kfree_sensitive(desc);
	if (tfm)
		crypto_free_shash(tfm);
	kfree(initiatorchg);
	kfree(initiatorchg_binhex);
	kfree(digest);
	kfree(response);
	kfree(server_digest);
	kfree(client_digest);
	return auth_ret;
}

u32 chap_main_loop(
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_node_auth *auth,
	char *in_text,
	char *out_text,
	int *in_len,
	int *out_len)
{
	struct iscsi_chap *chap = conn->auth_protocol;

	if (!chap) {
		chap = chap_server_open(conn, auth, in_text, out_text, out_len);
		if (!chap)
			return 2;
		chap->chap_state = CHAP_STAGE_SERVER_AIC;
		return 0;
	} else if (chap->chap_state == CHAP_STAGE_SERVER_AIC) {
		convert_null_to_semi(in_text, *in_len);
<<<<<<< HEAD
		if (chap_got_response(conn, auth, in_text, out_text,
=======
		if (chap_server_compute_hash(conn, auth, in_text, out_text,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				out_len) < 0) {
			chap_close(conn);
			return 2;
		}
		if (auth->authenticate_target)
			chap->chap_state = CHAP_STAGE_SERVER_NR;
		else
			*out_len = 0;
		chap_close(conn);
		return 1;
	}

	return 2;
}
