<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Kerberos-based RxRPC security
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

=======
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <crypto/skcipher.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
<<<<<<< HEAD
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <linux/ctype.h>
#include <linux/slab.h>
#include <net/sock.h>
#include <net/af_rxrpc.h>
#include <keys/rxrpc-type.h>
#define rxrpc_debug rxkad_debug
=======
#include <linux/scatterlist.h>
#include <linux/ctype.h>
#include <linux/slab.h>
#include <linux/key-type.h>
#include <net/sock.h>
#include <net/af_rxrpc.h>
#include <keys/rxrpc-type.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "ar-internal.h"

#define RXKAD_VERSION			2
#define MAXKRB5TICKETLEN		1024
#define RXKAD_TKT_TYPE_KERBEROS_V5	256
#define ANAME_SZ			40	/* size of authentication name */
#define INST_SZ				40	/* size of principal's instance */
#define REALM_SZ			40	/* size of principal's auth domain */
#define SNAME_SZ			40	/* size of service name */
<<<<<<< HEAD

unsigned rxrpc_debug;
module_param_named(debug, rxrpc_debug, uint, S_IWUSR | S_IRUGO);
MODULE_PARM_DESC(debug, "rxkad debugging mask");
=======
#define RXKAD_ALIGN			8
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct rxkad_level1_hdr {
	__be32	data_size;	/* true data size (excluding padding) */
};

struct rxkad_level2_hdr {
	__be32	data_size;	/* true data size (excluding padding) */
	__be32	checksum;	/* decrypted data checksum */
};

<<<<<<< HEAD
MODULE_DESCRIPTION("RxRPC network protocol type-2 security (Kerberos 4)");
MODULE_AUTHOR("Red Hat, Inc.");
MODULE_LICENSE("GPL");
=======
static int rxkad_prime_packet_security(struct rxrpc_connection *conn,
				       struct crypto_sync_skcipher *ci);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * this holds a pinned cipher so that keventd doesn't get called by the cipher
 * alloc routine, but since we have it to hand, we use it to decrypt RESPONSE
 * packets
 */
<<<<<<< HEAD
static struct crypto_blkcipher *rxkad_ci;
static DEFINE_MUTEX(rxkad_ci_mutex);

/*
 * initialise connection security
 */
static int rxkad_init_connection_security(struct rxrpc_connection *conn)
{
	struct crypto_blkcipher *ci;
	struct rxrpc_key_token *token;
=======
static struct crypto_sync_skcipher *rxkad_ci;
static struct skcipher_request *rxkad_ci_req;
static DEFINE_MUTEX(rxkad_ci_mutex);

/*
 * Parse the information from a server key
 *
 * The data should be the 8-byte secret key.
 */
static int rxkad_preparse_server_key(struct key_preparsed_payload *prep)
{
	struct crypto_skcipher *ci;

	if (prep->datalen != 8)
		return -EINVAL;

	memcpy(&prep->payload.data[2], prep->data, 8);

	ci = crypto_alloc_skcipher("pcbc(des)", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(ci)) {
		_leave(" = %ld", PTR_ERR(ci));
		return PTR_ERR(ci);
	}

	if (crypto_skcipher_setkey(ci, prep->data, 8) < 0)
		BUG();

	prep->payload.data[0] = ci;
	_leave(" = 0");
	return 0;
}

static void rxkad_free_preparse_server_key(struct key_preparsed_payload *prep)
{

	if (prep->payload.data[0])
		crypto_free_skcipher(prep->payload.data[0]);
}

static void rxkad_destroy_server_key(struct key *key)
{
	if (key->payload.data[0]) {
		crypto_free_skcipher(key->payload.data[0]);
		key->payload.data[0] = NULL;
	}
}

/*
 * initialise connection security
 */
static int rxkad_init_connection_security(struct rxrpc_connection *conn,
					  struct rxrpc_key_token *token)
{
	struct crypto_sync_skcipher *ci;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	_enter("{%d},{%x}", conn->debug_id, key_serial(conn->key));

<<<<<<< HEAD
	token = conn->key->payload.data;
	conn->security_ix = token->security_index;

	ci = crypto_alloc_blkcipher("pcbc(fcrypt)", 0, CRYPTO_ALG_ASYNC);
=======
	conn->security_ix = token->security_index;

	ci = crypto_alloc_sync_skcipher("pcbc(fcrypt)", 0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(ci)) {
		_debug("no cipher");
		ret = PTR_ERR(ci);
		goto error;
	}

<<<<<<< HEAD
	if (crypto_blkcipher_setkey(ci, token->kad->session_key,
				    sizeof(token->kad->session_key)) < 0)
=======
	if (crypto_sync_skcipher_setkey(ci, token->kad->session_key,
				   sizeof(token->kad->session_key)) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		BUG();

	switch (conn->security_level) {
	case RXRPC_SECURITY_PLAIN:
<<<<<<< HEAD
		break;
	case RXRPC_SECURITY_AUTH:
		conn->size_align = 8;
		conn->security_size = sizeof(struct rxkad_level1_hdr);
		conn->header_size += sizeof(struct rxkad_level1_hdr);
		break;
	case RXRPC_SECURITY_ENCRYPT:
		conn->size_align = 8;
		conn->security_size = sizeof(struct rxkad_level2_hdr);
		conn->header_size += sizeof(struct rxkad_level2_hdr);
=======
	case RXRPC_SECURITY_AUTH:
	case RXRPC_SECURITY_ENCRYPT:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		ret = -EKEYREJECTED;
		goto error;
	}

<<<<<<< HEAD
	conn->cipher = ci;
	ret = 0;
=======
	ret = rxkad_prime_packet_security(conn, ci);
	if (ret < 0)
		goto error_ci;

	conn->rxkad.cipher = ci;
	return 0;

error_ci:
	crypto_free_sync_skcipher(ci);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
error:
	_leave(" = %d", ret);
	return ret;
}

/*
<<<<<<< HEAD
 * prime the encryption state with the invariant parts of a connection's
 * description
 */
static void rxkad_prime_packet_security(struct rxrpc_connection *conn)
{
	struct rxrpc_key_token *token;
	struct blkcipher_desc desc;
	struct scatterlist sg[2];
	struct rxrpc_crypt iv;
	struct {
		__be32 x[4];
	} tmpbuf __attribute__((aligned(16))); /* must all be in same page */
=======
 * Work out how much data we can put in a packet.
 */
static struct rxrpc_txbuf *rxkad_alloc_txbuf(struct rxrpc_call *call, size_t remain, gfp_t gfp)
{
	struct rxrpc_txbuf *txb;
	size_t shdr, space;

	remain = min(remain, 65535 - sizeof(struct rxrpc_wire_header));

	switch (call->conn->security_level) {
	default:
		space = min_t(size_t, remain, RXRPC_JUMBO_DATALEN);
		return rxrpc_alloc_data_txbuf(call, space, 1, gfp);
	case RXRPC_SECURITY_AUTH:
		shdr = sizeof(struct rxkad_level1_hdr);
		break;
	case RXRPC_SECURITY_ENCRYPT:
		shdr = sizeof(struct rxkad_level2_hdr);
		break;
	}

	space = min_t(size_t, round_down(RXRPC_JUMBO_DATALEN, RXKAD_ALIGN), remain + shdr);
	space = round_up(space, RXKAD_ALIGN);

	txb = rxrpc_alloc_data_txbuf(call, space, RXKAD_ALIGN, gfp);
	if (!txb)
		return NULL;

	txb->offset += shdr;
	txb->space -= shdr;
	return txb;
}

/*
 * prime the encryption state with the invariant parts of a connection's
 * description
 */
static int rxkad_prime_packet_security(struct rxrpc_connection *conn,
				       struct crypto_sync_skcipher *ci)
{
	struct skcipher_request *req;
	struct rxrpc_key_token *token;
	struct scatterlist sg;
	struct rxrpc_crypt iv;
	__be32 *tmpbuf;
	size_t tmpsize = 4 * sizeof(__be32);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	_enter("");

	if (!conn->key)
<<<<<<< HEAD
		return;

	token = conn->key->payload.data;
	memcpy(&iv, token->kad->session_key, sizeof(iv));

	desc.tfm = conn->cipher;
	desc.info = iv.x;
	desc.flags = 0;

	tmpbuf.x[0] = conn->epoch;
	tmpbuf.x[1] = conn->cid;
	tmpbuf.x[2] = 0;
	tmpbuf.x[3] = htonl(conn->security_ix);

	sg_init_one(&sg[0], &tmpbuf, sizeof(tmpbuf));
	sg_init_one(&sg[1], &tmpbuf, sizeof(tmpbuf));
	crypto_blkcipher_encrypt_iv(&desc, &sg[0], &sg[1], sizeof(tmpbuf));

	memcpy(&conn->csum_iv, &tmpbuf.x[2], sizeof(conn->csum_iv));
	ASSERTCMP(conn->csum_iv.n[0], ==, tmpbuf.x[2]);

	_leave("");
=======
		return 0;

	tmpbuf = kmalloc(tmpsize, GFP_KERNEL);
	if (!tmpbuf)
		return -ENOMEM;

	req = skcipher_request_alloc(&ci->base, GFP_NOFS);
	if (!req) {
		kfree(tmpbuf);
		return -ENOMEM;
	}

	token = conn->key->payload.data[0];
	memcpy(&iv, token->kad->session_key, sizeof(iv));

	tmpbuf[0] = htonl(conn->proto.epoch);
	tmpbuf[1] = htonl(conn->proto.cid);
	tmpbuf[2] = 0;
	tmpbuf[3] = htonl(conn->security_ix);

	sg_init_one(&sg, tmpbuf, tmpsize);
	skcipher_request_set_sync_tfm(req, ci);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, &sg, &sg, tmpsize, iv.x);
	crypto_skcipher_encrypt(req);
	skcipher_request_free(req);

	memcpy(&conn->rxkad.csum_iv, tmpbuf + 2, sizeof(conn->rxkad.csum_iv));
	kfree(tmpbuf);
	_leave(" = 0");
	return 0;
}

/*
 * Allocate and prepare the crypto request on a call.  For any particular call,
 * this is called serially for the packets, so no lock should be necessary.
 */
static struct skcipher_request *rxkad_get_call_crypto(struct rxrpc_call *call)
{
	struct crypto_skcipher *tfm = &call->conn->rxkad.cipher->base;

	return skcipher_request_alloc(tfm, GFP_NOFS);
}

/*
 * Clean up the crypto on a call.
 */
static void rxkad_free_call_crypto(struct rxrpc_call *call)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * partially encrypt a packet (level 1 security)
 */
static int rxkad_secure_packet_auth(const struct rxrpc_call *call,
<<<<<<< HEAD
				    struct sk_buff *skb,
				    u32 data_size,
				    void *sechdr)
{
	struct rxrpc_skb_priv *sp;
	struct blkcipher_desc desc;
	struct rxrpc_crypt iv;
	struct scatterlist sg[2];
	struct {
		struct rxkad_level1_hdr hdr;
		__be32	first;	/* first four bytes of data and padding */
	} tmpbuf __attribute__((aligned(8))); /* must all be in same page */
	u16 check;

	sp = rxrpc_skb(skb);

	_enter("");

	check = ntohl(sp->hdr.seq ^ sp->hdr.callNumber);
	data_size |= (u32) check << 16;

	tmpbuf.hdr.data_size = htonl(data_size);
	memcpy(&tmpbuf.first, sechdr + 4, sizeof(tmpbuf.first));

	/* start the encryption afresh */
	memset(&iv, 0, sizeof(iv));
	desc.tfm = call->conn->cipher;
	desc.info = iv.x;
	desc.flags = 0;

	sg_init_one(&sg[0], &tmpbuf, sizeof(tmpbuf));
	sg_init_one(&sg[1], &tmpbuf, sizeof(tmpbuf));
	crypto_blkcipher_encrypt_iv(&desc, &sg[0], &sg[1], sizeof(tmpbuf));

	memcpy(sechdr, &tmpbuf, sizeof(tmpbuf));
=======
				    struct rxrpc_txbuf *txb,
				    struct skcipher_request *req)
{
	struct rxrpc_wire_header *whdr = txb->kvec[0].iov_base;
	struct rxkad_level1_hdr *hdr = (void *)(whdr + 1);
	struct rxrpc_crypt iv;
	struct scatterlist sg;
	size_t pad;
	u16 check;

	_enter("");

	check = txb->seq ^ call->call_id;
	hdr->data_size = htonl((u32)check << 16 | txb->len);

	txb->len += sizeof(struct rxkad_level1_hdr);
	pad = txb->len;
	pad = RXKAD_ALIGN - pad;
	pad &= RXKAD_ALIGN - 1;
	if (pad) {
		memset(txb->kvec[0].iov_base + txb->offset, 0, pad);
		txb->len += pad;
	}

	/* start the encryption afresh */
	memset(&iv, 0, sizeof(iv));

	sg_init_one(&sg, hdr, 8);
	skcipher_request_set_sync_tfm(req, call->conn->rxkad.cipher);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, &sg, &sg, 8, iv.x);
	crypto_skcipher_encrypt(req);
	skcipher_request_zero(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	_leave(" = 0");
	return 0;
}

/*
 * wholly encrypt a packet (level 2 security)
 */
static int rxkad_secure_packet_encrypt(const struct rxrpc_call *call,
<<<<<<< HEAD
					struct sk_buff *skb,
					u32 data_size,
					void *sechdr)
{
	const struct rxrpc_key_token *token;
	struct rxkad_level2_hdr rxkhdr
		__attribute__((aligned(8))); /* must be all on one page */
	struct rxrpc_skb_priv *sp;
	struct blkcipher_desc desc;
	struct rxrpc_crypt iv;
	struct scatterlist sg[16];
	struct sk_buff *trailer;
	unsigned len;
	u16 check;
	int nsg;

	sp = rxrpc_skb(skb);

	_enter("");

	check = ntohl(sp->hdr.seq ^ sp->hdr.callNumber);

	rxkhdr.data_size = htonl(data_size | (u32) check << 16);
	rxkhdr.checksum = 0;

	/* encrypt from the session key */
	token = call->conn->key->payload.data;
	memcpy(&iv, token->kad->session_key, sizeof(iv));
	desc.tfm = call->conn->cipher;
	desc.info = iv.x;
	desc.flags = 0;

	sg_init_one(&sg[0], sechdr, sizeof(rxkhdr));
	sg_init_one(&sg[1], &rxkhdr, sizeof(rxkhdr));
	crypto_blkcipher_encrypt_iv(&desc, &sg[0], &sg[1], sizeof(rxkhdr));

	/* we want to encrypt the skbuff in-place */
	nsg = skb_cow_data(skb, 0, &trailer);
	if (nsg < 0 || nsg > 16)
		return -ENOMEM;

	len = data_size + call->conn->size_align - 1;
	len &= ~(call->conn->size_align - 1);

	sg_init_table(sg, nsg);
	skb_to_sgvec(skb, sg, 0, len);
	crypto_blkcipher_encrypt_iv(&desc, sg, sg, len);

	_leave(" = 0");
	return 0;
=======
				       struct rxrpc_txbuf *txb,
				       struct skcipher_request *req)
{
	const struct rxrpc_key_token *token;
	struct rxrpc_wire_header *whdr = txb->kvec[0].iov_base;
	struct rxkad_level2_hdr *rxkhdr = (void *)(whdr + 1);
	struct rxrpc_crypt iv;
	struct scatterlist sg;
	size_t pad;
	u16 check;
	int ret;

	_enter("");

	check = txb->seq ^ call->call_id;

	rxkhdr->data_size = htonl(txb->len | (u32)check << 16);
	rxkhdr->checksum = 0;

	txb->len += sizeof(struct rxkad_level2_hdr);
	pad = txb->len;
	pad = RXKAD_ALIGN - pad;
	pad &= RXKAD_ALIGN - 1;
	if (pad) {
		memset(txb->kvec[0].iov_base + txb->offset, 0, pad);
		txb->len += pad;
	}

	/* encrypt from the session key */
	token = call->conn->key->payload.data[0];
	memcpy(&iv, token->kad->session_key, sizeof(iv));

	sg_init_one(&sg, rxkhdr, txb->len);
	skcipher_request_set_sync_tfm(req, call->conn->rxkad.cipher);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, &sg, &sg, txb->len, iv.x);
	ret = crypto_skcipher_encrypt(req);
	skcipher_request_zero(req);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * checksum an RxRPC packet header
 */
<<<<<<< HEAD
static int rxkad_secure_packet(const struct rxrpc_call *call,
				struct sk_buff *skb,
				size_t data_size,
				void *sechdr)
{
	struct rxrpc_skb_priv *sp;
	struct blkcipher_desc desc;
	struct rxrpc_crypt iv;
	struct scatterlist sg[2];
	struct {
		__be32 x[2];
	} tmpbuf __attribute__((aligned(8))); /* must all be in same page */
	__be32 x;
	u32 y;
	int ret;

	sp = rxrpc_skb(skb);

	_enter("{%d{%x}},{#%u},%zu,",
	       call->debug_id, key_serial(call->conn->key), ntohl(sp->hdr.seq),
	       data_size);

	if (!call->conn->cipher)
=======
static int rxkad_secure_packet(struct rxrpc_call *call, struct rxrpc_txbuf *txb)
{
	struct skcipher_request	*req;
	struct rxrpc_crypt iv;
	struct scatterlist sg;
	union {
		__be32 buf[2];
	} crypto __aligned(8);
	u32 x, y;
	int ret;

	_enter("{%d{%x}},{#%u},%u,",
	       call->debug_id, key_serial(call->conn->key),
	       txb->seq, txb->len);

	if (!call->conn->rxkad.cipher)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	ret = key_validate(call->conn->key);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	/* continue encrypting from where we left off */
	memcpy(&iv, call->conn->csum_iv.x, sizeof(iv));
	desc.tfm = call->conn->cipher;
	desc.info = iv.x;
	desc.flags = 0;

	/* calculate the security checksum */
	x = htonl(call->channel << (32 - RXRPC_CIDSHIFT));
	x |= sp->hdr.seq & cpu_to_be32(0x3fffffff);
	tmpbuf.x[0] = sp->hdr.callNumber;
	tmpbuf.x[1] = x;

	sg_init_one(&sg[0], &tmpbuf, sizeof(tmpbuf));
	sg_init_one(&sg[1], &tmpbuf, sizeof(tmpbuf));
	crypto_blkcipher_encrypt_iv(&desc, &sg[0], &sg[1], sizeof(tmpbuf));

	y = ntohl(tmpbuf.x[1]);
	y = (y >> 16) & 0xffff;
	if (y == 0)
		y = 1; /* zero checksums are not permitted */
	sp->hdr.cksum = htons(y);
=======
	req = rxkad_get_call_crypto(call);
	if (!req)
		return -ENOMEM;

	/* continue encrypting from where we left off */
	memcpy(&iv, call->conn->rxkad.csum_iv.x, sizeof(iv));

	/* calculate the security checksum */
	x = (call->cid & RXRPC_CHANNELMASK) << (32 - RXRPC_CIDSHIFT);
	x |= txb->seq & 0x3fffffff;
	crypto.buf[0] = htonl(call->call_id);
	crypto.buf[1] = htonl(x);

	sg_init_one(&sg, crypto.buf, 8);
	skcipher_request_set_sync_tfm(req, call->conn->rxkad.cipher);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, &sg, &sg, 8, iv.x);
	crypto_skcipher_encrypt(req);
	skcipher_request_zero(req);

	y = ntohl(crypto.buf[1]);
	y = (y >> 16) & 0xffff;
	if (y == 0)
		y = 1; /* zero checksums are not permitted */
	txb->cksum = htons(y);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (call->conn->security_level) {
	case RXRPC_SECURITY_PLAIN:
		ret = 0;
		break;
	case RXRPC_SECURITY_AUTH:
<<<<<<< HEAD
		ret = rxkad_secure_packet_auth(call, skb, data_size, sechdr);
		break;
	case RXRPC_SECURITY_ENCRYPT:
		ret = rxkad_secure_packet_encrypt(call, skb, data_size,
						  sechdr);
=======
		ret = rxkad_secure_packet_auth(call, txb, req);
		break;
	case RXRPC_SECURITY_ENCRYPT:
		ret = rxkad_secure_packet_encrypt(call, txb, req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		ret = -EPERM;
		break;
	}

<<<<<<< HEAD
	_leave(" = %d [set %hx]", ret, y);
=======
	skcipher_request_free(req);
	_leave(" = %d [set %x]", ret, y);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * decrypt partial encryption on a packet (level 1 security)
 */
<<<<<<< HEAD
static int rxkad_verify_packet_auth(const struct rxrpc_call *call,
				    struct sk_buff *skb,
				    u32 *_abort_code)
{
	struct rxkad_level1_hdr sechdr;
	struct rxrpc_skb_priv *sp;
	struct blkcipher_desc desc;
	struct rxrpc_crypt iv;
	struct scatterlist sg[16];
	struct sk_buff *trailer;
	u32 data_size, buf;
	u16 check;
	int nsg;

	_enter("");

	sp = rxrpc_skb(skb);

	/* we want to decrypt the skbuff in-place */
	nsg = skb_cow_data(skb, 0, &trailer);
	if (nsg < 0 || nsg > 16)
		goto nomem;

	sg_init_table(sg, nsg);
	skb_to_sgvec(skb, sg, 0, 8);

	/* start the decryption afresh */
	memset(&iv, 0, sizeof(iv));
	desc.tfm = call->conn->cipher;
	desc.info = iv.x;
	desc.flags = 0;

	crypto_blkcipher_decrypt_iv(&desc, sg, sg, 8);

	/* remove the decrypted packet length */
	if (skb_copy_bits(skb, 0, &sechdr, sizeof(sechdr)) < 0)
		goto datalen_error;
	if (!skb_pull(skb, sizeof(sechdr)))
		BUG();
=======
static int rxkad_verify_packet_1(struct rxrpc_call *call, struct sk_buff *skb,
				 rxrpc_seq_t seq,
				 struct skcipher_request *req)
{
	struct rxkad_level1_hdr sechdr;
	struct rxrpc_skb_priv *sp = rxrpc_skb(skb);
	struct rxrpc_crypt iv;
	struct scatterlist sg[16];
	u32 data_size, buf;
	u16 check;
	int ret;

	_enter("");

	if (sp->len < 8)
		return rxrpc_abort_eproto(call, skb, RXKADSEALEDINCON,
					  rxkad_abort_1_short_header);

	/* Decrypt the skbuff in-place.  TODO: We really want to decrypt
	 * directly into the target buffer.
	 */
	sg_init_table(sg, ARRAY_SIZE(sg));
	ret = skb_to_sgvec(skb, sg, sp->offset, 8);
	if (unlikely(ret < 0))
		return ret;

	/* start the decryption afresh */
	memset(&iv, 0, sizeof(iv));

	skcipher_request_set_sync_tfm(req, call->conn->rxkad.cipher);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg, sg, 8, iv.x);
	crypto_skcipher_decrypt(req);
	skcipher_request_zero(req);

	/* Extract the decrypted packet length */
	if (skb_copy_bits(skb, sp->offset, &sechdr, sizeof(sechdr)) < 0)
		return rxrpc_abort_eproto(call, skb, RXKADDATALEN,
					  rxkad_abort_1_short_encdata);
	sp->offset += sizeof(sechdr);
	sp->len    -= sizeof(sechdr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	buf = ntohl(sechdr.data_size);
	data_size = buf & 0xffff;

	check = buf >> 16;
<<<<<<< HEAD
	check ^= ntohl(sp->hdr.seq ^ sp->hdr.callNumber);
	check &= 0xffff;
	if (check != 0) {
		*_abort_code = RXKADSEALEDINCON;
		goto protocol_error;
	}

	/* shorten the packet to remove the padding */
	if (data_size > skb->len)
		goto datalen_error;
	else if (data_size < skb->len)
		skb->len = data_size;

	_leave(" = 0 [dlen=%x]", data_size);
	return 0;

datalen_error:
	*_abort_code = RXKADDATALEN;
protocol_error:
	_leave(" = -EPROTO");
	return -EPROTO;

nomem:
	_leave(" = -ENOMEM");
	return -ENOMEM;
=======
	check ^= seq ^ call->call_id;
	check &= 0xffff;
	if (check != 0)
		return rxrpc_abort_eproto(call, skb, RXKADSEALEDINCON,
					  rxkad_abort_1_short_check);
	if (data_size > sp->len)
		return rxrpc_abort_eproto(call, skb, RXKADDATALEN,
					  rxkad_abort_1_short_data);
	sp->len = data_size;

	_leave(" = 0 [dlen=%x]", data_size);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * wholly decrypt a packet (level 2 security)
 */
<<<<<<< HEAD
static int rxkad_verify_packet_encrypt(const struct rxrpc_call *call,
				       struct sk_buff *skb,
				       u32 *_abort_code)
{
	const struct rxrpc_key_token *token;
	struct rxkad_level2_hdr sechdr;
	struct rxrpc_skb_priv *sp;
	struct blkcipher_desc desc;
	struct rxrpc_crypt iv;
	struct scatterlist _sg[4], *sg;
	struct sk_buff *trailer;
	u32 data_size, buf;
	u16 check;
	int nsg;

	_enter(",{%d}", skb->len);

	sp = rxrpc_skb(skb);

	/* we want to decrypt the skbuff in-place */
	nsg = skb_cow_data(skb, 0, &trailer);
	if (nsg < 0)
		goto nomem;

	sg = _sg;
	if (unlikely(nsg > 4)) {
		sg = kmalloc(sizeof(*sg) * nsg, GFP_NOIO);
		if (!sg)
			goto nomem;
	}

	sg_init_table(sg, nsg);
	skb_to_sgvec(skb, sg, 0, skb->len);

	/* decrypt from the session key */
	token = call->conn->key->payload.data;
	memcpy(&iv, token->kad->session_key, sizeof(iv));
	desc.tfm = call->conn->cipher;
	desc.info = iv.x;
	desc.flags = 0;

	crypto_blkcipher_decrypt_iv(&desc, sg, sg, skb->len);
	if (sg != _sg)
		kfree(sg);

	/* remove the decrypted packet length */
	if (skb_copy_bits(skb, 0, &sechdr, sizeof(sechdr)) < 0)
		goto datalen_error;
	if (!skb_pull(skb, sizeof(sechdr)))
		BUG();
=======
static int rxkad_verify_packet_2(struct rxrpc_call *call, struct sk_buff *skb,
				 rxrpc_seq_t seq,
				 struct skcipher_request *req)
{
	const struct rxrpc_key_token *token;
	struct rxkad_level2_hdr sechdr;
	struct rxrpc_skb_priv *sp = rxrpc_skb(skb);
	struct rxrpc_crypt iv;
	struct scatterlist _sg[4], *sg;
	u32 data_size, buf;
	u16 check;
	int nsg, ret;

	_enter(",{%d}", sp->len);

	if (sp->len < 8)
		return rxrpc_abort_eproto(call, skb, RXKADSEALEDINCON,
					  rxkad_abort_2_short_header);

	/* Decrypt the skbuff in-place.  TODO: We really want to decrypt
	 * directly into the target buffer.
	 */
	sg = _sg;
	nsg = skb_shinfo(skb)->nr_frags + 1;
	if (nsg <= 4) {
		nsg = 4;
	} else {
		sg = kmalloc_array(nsg, sizeof(*sg), GFP_NOIO);
		if (!sg)
			return -ENOMEM;
	}

	sg_init_table(sg, nsg);
	ret = skb_to_sgvec(skb, sg, sp->offset, sp->len);
	if (unlikely(ret < 0)) {
		if (sg != _sg)
			kfree(sg);
		return ret;
	}

	/* decrypt from the session key */
	token = call->conn->key->payload.data[0];
	memcpy(&iv, token->kad->session_key, sizeof(iv));

	skcipher_request_set_sync_tfm(req, call->conn->rxkad.cipher);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg, sg, sp->len, iv.x);
	crypto_skcipher_decrypt(req);
	skcipher_request_zero(req);
	if (sg != _sg)
		kfree(sg);

	/* Extract the decrypted packet length */
	if (skb_copy_bits(skb, sp->offset, &sechdr, sizeof(sechdr)) < 0)
		return rxrpc_abort_eproto(call, skb, RXKADDATALEN,
					  rxkad_abort_2_short_len);
	sp->offset += sizeof(sechdr);
	sp->len    -= sizeof(sechdr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	buf = ntohl(sechdr.data_size);
	data_size = buf & 0xffff;

	check = buf >> 16;
<<<<<<< HEAD
	check ^= ntohl(sp->hdr.seq ^ sp->hdr.callNumber);
	check &= 0xffff;
	if (check != 0) {
		*_abort_code = RXKADSEALEDINCON;
		goto protocol_error;
	}

	/* shorten the packet to remove the padding */
	if (data_size > skb->len)
		goto datalen_error;
	else if (data_size < skb->len)
		skb->len = data_size;

	_leave(" = 0 [dlen=%x]", data_size);
	return 0;

datalen_error:
	*_abort_code = RXKADDATALEN;
protocol_error:
	_leave(" = -EPROTO");
	return -EPROTO;

nomem:
	_leave(" = -ENOMEM");
	return -ENOMEM;
}

/*
 * verify the security on a received packet
 */
static int rxkad_verify_packet(const struct rxrpc_call *call,
			       struct sk_buff *skb,
			       u32 *_abort_code)
{
	struct blkcipher_desc desc;
	struct rxrpc_skb_priv *sp;
	struct rxrpc_crypt iv;
	struct scatterlist sg[2];
	struct {
		__be32 x[2];
	} tmpbuf __attribute__((aligned(8))); /* must all be in same page */
	__be32 x;
	__be16 cksum;
	u32 y;
	int ret;

	sp = rxrpc_skb(skb);

	_enter("{%d{%x}},{#%u}",
	       call->debug_id, key_serial(call->conn->key),
	       ntohl(sp->hdr.seq));

	if (!call->conn->cipher)
		return 0;

	if (sp->hdr.securityIndex != RXRPC_SECURITY_RXKAD) {
		*_abort_code = RXKADINCONSISTENCY;
		_leave(" = -EPROTO [not rxkad]");
		return -EPROTO;
	}

	/* continue encrypting from where we left off */
	memcpy(&iv, call->conn->csum_iv.x, sizeof(iv));
	desc.tfm = call->conn->cipher;
	desc.info = iv.x;
	desc.flags = 0;

	/* validate the security checksum */
	x = htonl(call->channel << (32 - RXRPC_CIDSHIFT));
	x |= sp->hdr.seq & cpu_to_be32(0x3fffffff);
	tmpbuf.x[0] = call->call_id;
	tmpbuf.x[1] = x;

	sg_init_one(&sg[0], &tmpbuf, sizeof(tmpbuf));
	sg_init_one(&sg[1], &tmpbuf, sizeof(tmpbuf));
	crypto_blkcipher_encrypt_iv(&desc, &sg[0], &sg[1], sizeof(tmpbuf));

	y = ntohl(tmpbuf.x[1]);
	y = (y >> 16) & 0xffff;
	if (y == 0)
		y = 1; /* zero checksums are not permitted */

	cksum = htons(y);
	if (sp->hdr.cksum != cksum) {
		*_abort_code = RXKADSEALEDINCON;
		_leave(" = -EPROTO [csum failed]");
		return -EPROTO;
=======
	check ^= seq ^ call->call_id;
	check &= 0xffff;
	if (check != 0)
		return rxrpc_abort_eproto(call, skb, RXKADSEALEDINCON,
					  rxkad_abort_2_short_check);

	if (data_size > sp->len)
		return rxrpc_abort_eproto(call, skb, RXKADDATALEN,
					  rxkad_abort_2_short_data);

	sp->len = data_size;
	_leave(" = 0 [dlen=%x]", data_size);
	return 0;
}

/*
 * Verify the security on a received packet and the subpackets therein.
 */
static int rxkad_verify_packet(struct rxrpc_call *call, struct sk_buff *skb)
{
	struct rxrpc_skb_priv *sp = rxrpc_skb(skb);
	struct skcipher_request	*req;
	struct rxrpc_crypt iv;
	struct scatterlist sg;
	union {
		__be32 buf[2];
	} crypto __aligned(8);
	rxrpc_seq_t seq = sp->hdr.seq;
	int ret;
	u16 cksum;
	u32 x, y;

	_enter("{%d{%x}},{#%u}",
	       call->debug_id, key_serial(call->conn->key), seq);

	if (!call->conn->rxkad.cipher)
		return 0;

	req = rxkad_get_call_crypto(call);
	if (!req)
		return -ENOMEM;

	/* continue encrypting from where we left off */
	memcpy(&iv, call->conn->rxkad.csum_iv.x, sizeof(iv));

	/* validate the security checksum */
	x = (call->cid & RXRPC_CHANNELMASK) << (32 - RXRPC_CIDSHIFT);
	x |= seq & 0x3fffffff;
	crypto.buf[0] = htonl(call->call_id);
	crypto.buf[1] = htonl(x);

	sg_init_one(&sg, crypto.buf, 8);
	skcipher_request_set_sync_tfm(req, call->conn->rxkad.cipher);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, &sg, &sg, 8, iv.x);
	crypto_skcipher_encrypt(req);
	skcipher_request_zero(req);

	y = ntohl(crypto.buf[1]);
	cksum = (y >> 16) & 0xffff;
	if (cksum == 0)
		cksum = 1; /* zero checksums are not permitted */

	if (cksum != sp->hdr.cksum) {
		ret = rxrpc_abort_eproto(call, skb, RXKADSEALEDINCON,
					 rxkad_abort_bad_checksum);
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	switch (call->conn->security_level) {
	case RXRPC_SECURITY_PLAIN:
		ret = 0;
		break;
	case RXRPC_SECURITY_AUTH:
<<<<<<< HEAD
		ret = rxkad_verify_packet_auth(call, skb, _abort_code);
		break;
	case RXRPC_SECURITY_ENCRYPT:
		ret = rxkad_verify_packet_encrypt(call, skb, _abort_code);
=======
		ret = rxkad_verify_packet_1(call, skb, seq, req);
		break;
	case RXRPC_SECURITY_ENCRYPT:
		ret = rxkad_verify_packet_2(call, skb, seq, req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		ret = -ENOANO;
		break;
	}

<<<<<<< HEAD
	_leave(" = %d", ret);
=======
out:
	skcipher_request_free(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * issue a challenge
 */
static int rxkad_issue_challenge(struct rxrpc_connection *conn)
{
	struct rxkad_challenge challenge;
<<<<<<< HEAD
	struct rxrpc_header hdr;
	struct msghdr msg;
	struct kvec iov[2];
	size_t len;
	int ret;

	_enter("{%d,%x}", conn->debug_id, key_serial(conn->key));

	ret = key_validate(conn->key);
	if (ret < 0)
		return ret;

	get_random_bytes(&conn->security_nonce, sizeof(conn->security_nonce));

	challenge.version	= htonl(2);
	challenge.nonce		= htonl(conn->security_nonce);
	challenge.min_level	= htonl(0);
	challenge.__padding	= 0;

	msg.msg_name	= &conn->trans->peer->srx.transport.sin;
	msg.msg_namelen	= sizeof(conn->trans->peer->srx.transport.sin);
=======
	struct rxrpc_wire_header whdr;
	struct msghdr msg;
	struct kvec iov[2];
	size_t len;
	u32 serial;
	int ret;

	_enter("{%d}", conn->debug_id);

	get_random_bytes(&conn->rxkad.nonce, sizeof(conn->rxkad.nonce));

	challenge.version	= htonl(2);
	challenge.nonce		= htonl(conn->rxkad.nonce);
	challenge.min_level	= htonl(0);
	challenge.__padding	= 0;

	msg.msg_name	= &conn->peer->srx.transport;
	msg.msg_namelen	= conn->peer->srx.transport_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	msg.msg_control	= NULL;
	msg.msg_controllen = 0;
	msg.msg_flags	= 0;

<<<<<<< HEAD
	hdr.epoch	= conn->epoch;
	hdr.cid		= conn->cid;
	hdr.callNumber	= 0;
	hdr.seq		= 0;
	hdr.type	= RXRPC_PACKET_TYPE_CHALLENGE;
	hdr.flags	= conn->out_clientflag;
	hdr.userStatus	= 0;
	hdr.securityIndex = conn->security_ix;
	hdr._rsvd	= 0;
	hdr.serviceId	= conn->service_id;

	iov[0].iov_base	= &hdr;
	iov[0].iov_len	= sizeof(hdr);
=======
	whdr.epoch	= htonl(conn->proto.epoch);
	whdr.cid	= htonl(conn->proto.cid);
	whdr.callNumber	= 0;
	whdr.seq	= 0;
	whdr.type	= RXRPC_PACKET_TYPE_CHALLENGE;
	whdr.flags	= conn->out_clientflag;
	whdr.userStatus	= 0;
	whdr.securityIndex = conn->security_ix;
	whdr._rsvd	= 0;
	whdr.serviceId	= htons(conn->service_id);

	iov[0].iov_base	= &whdr;
	iov[0].iov_len	= sizeof(whdr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iov[1].iov_base	= &challenge;
	iov[1].iov_len	= sizeof(challenge);

	len = iov[0].iov_len + iov[1].iov_len;

<<<<<<< HEAD
	hdr.serial = htonl(atomic_inc_return(&conn->serial));
	_proto("Tx CHALLENGE %%%u", ntohl(hdr.serial));

	ret = kernel_sendmsg(conn->trans->local->socket, &msg, iov, 2, len);
	if (ret < 0) {
		_debug("sendmsg failed: %d", ret);
		return -EAGAIN;
	}

=======
	serial = rxrpc_get_next_serial(conn);
	whdr.serial = htonl(serial);

	ret = kernel_sendmsg(conn->local->socket, &msg, iov, 2, len);
	if (ret < 0) {
		trace_rxrpc_tx_fail(conn->debug_id, serial, ret,
				    rxrpc_tx_point_rxkad_challenge);
		return -EAGAIN;
	}

	conn->peer->last_tx_at = ktime_get_seconds();
	trace_rxrpc_tx_packet(conn->debug_id, &whdr,
			      rxrpc_tx_point_rxkad_challenge);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	_leave(" = 0");
	return 0;
}

/*
 * send a Kerberos security response
 */
static int rxkad_send_response(struct rxrpc_connection *conn,
<<<<<<< HEAD
			       struct rxrpc_header *hdr,
			       struct rxkad_response *resp,
			       const struct rxkad_key *s2)
{
	struct msghdr msg;
	struct kvec iov[3];
	size_t len;
=======
			       struct rxrpc_host_header *hdr,
			       struct rxkad_response *resp,
			       const struct rxkad_key *s2)
{
	struct rxrpc_wire_header whdr;
	struct msghdr msg;
	struct kvec iov[3];
	size_t len;
	u32 serial;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	_enter("");

<<<<<<< HEAD
	msg.msg_name	= &conn->trans->peer->srx.transport.sin;
	msg.msg_namelen	= sizeof(conn->trans->peer->srx.transport.sin);
=======
	msg.msg_name	= &conn->peer->srx.transport;
	msg.msg_namelen	= conn->peer->srx.transport_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	msg.msg_control	= NULL;
	msg.msg_controllen = 0;
	msg.msg_flags	= 0;

<<<<<<< HEAD
	hdr->epoch	= conn->epoch;
	hdr->seq	= 0;
	hdr->type	= RXRPC_PACKET_TYPE_RESPONSE;
	hdr->flags	= conn->out_clientflag;
	hdr->userStatus	= 0;
	hdr->_rsvd	= 0;

	iov[0].iov_base	= hdr;
	iov[0].iov_len	= sizeof(*hdr);
	iov[1].iov_base	= resp;
	iov[1].iov_len	= sizeof(*resp);
	iov[2].iov_base	= (void *) s2->ticket;
=======
	memset(&whdr, 0, sizeof(whdr));
	whdr.epoch	= htonl(hdr->epoch);
	whdr.cid	= htonl(hdr->cid);
	whdr.type	= RXRPC_PACKET_TYPE_RESPONSE;
	whdr.flags	= conn->out_clientflag;
	whdr.securityIndex = hdr->securityIndex;
	whdr.serviceId	= htons(hdr->serviceId);

	iov[0].iov_base	= &whdr;
	iov[0].iov_len	= sizeof(whdr);
	iov[1].iov_base	= resp;
	iov[1].iov_len	= sizeof(*resp);
	iov[2].iov_base	= (void *)s2->ticket;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iov[2].iov_len	= s2->ticket_len;

	len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;

<<<<<<< HEAD
	hdr->serial = htonl(atomic_inc_return(&conn->serial));
	_proto("Tx RESPONSE %%%u", ntohl(hdr->serial));

	ret = kernel_sendmsg(conn->trans->local->socket, &msg, iov, 3, len);
	if (ret < 0) {
		_debug("sendmsg failed: %d", ret);
		return -EAGAIN;
	}

=======
	serial = rxrpc_get_next_serial(conn);
	whdr.serial = htonl(serial);

	rxrpc_local_dont_fragment(conn->local, false);
	ret = kernel_sendmsg(conn->local->socket, &msg, iov, 3, len);
	if (ret < 0) {
		trace_rxrpc_tx_fail(conn->debug_id, serial, ret,
				    rxrpc_tx_point_rxkad_response);
		return -EAGAIN;
	}

	conn->peer->last_tx_at = ktime_get_seconds();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	_leave(" = 0");
	return 0;
}

/*
 * calculate the response checksum
 */
static void rxkad_calc_response_checksum(struct rxkad_response *response)
{
	u32 csum = 1000003;
	int loop;
	u8 *p = (u8 *) response;

	for (loop = sizeof(*response); loop > 0; loop--)
		csum = csum * 0x10204081 + *p++;

	response->encrypted.checksum = htonl(csum);
}

/*
<<<<<<< HEAD
 * load a scatterlist with a potentially split-page buffer
 */
static void rxkad_sg_set_buf2(struct scatterlist sg[2],
			      void *buf, size_t buflen)
{
	int nsg = 1;

	sg_init_table(sg, 2);

	sg_set_buf(&sg[0], buf, buflen);
	if (sg[0].offset + buflen > PAGE_SIZE) {
		/* the buffer was split over two pages */
		sg[0].length = PAGE_SIZE - sg[0].offset;
		sg_set_buf(&sg[1], buf + sg[0].length, buflen - sg[0].length);
		nsg++;
	}

	sg_mark_end(&sg[nsg - 1]);

	ASSERTCMP(sg[0].length + sg[1].length, ==, buflen);
}

/*
 * encrypt the response packet
 */
static void rxkad_encrypt_response(struct rxrpc_connection *conn,
				   struct rxkad_response *resp,
				   const struct rxkad_key *s2)
{
	struct blkcipher_desc desc;
	struct rxrpc_crypt iv;
	struct scatterlist sg[2];

	/* continue encrypting from where we left off */
	memcpy(&iv, s2->session_key, sizeof(iv));
	desc.tfm = conn->cipher;
	desc.info = iv.x;
	desc.flags = 0;

	rxkad_sg_set_buf2(sg, &resp->encrypted, sizeof(resp->encrypted));
	crypto_blkcipher_encrypt_iv(&desc, sg, sg, sizeof(resp->encrypted));
=======
 * encrypt the response packet
 */
static int rxkad_encrypt_response(struct rxrpc_connection *conn,
				  struct rxkad_response *resp,
				  const struct rxkad_key *s2)
{
	struct skcipher_request *req;
	struct rxrpc_crypt iv;
	struct scatterlist sg[1];

	req = skcipher_request_alloc(&conn->rxkad.cipher->base, GFP_NOFS);
	if (!req)
		return -ENOMEM;

	/* continue encrypting from where we left off */
	memcpy(&iv, s2->session_key, sizeof(iv));

	sg_init_table(sg, 1);
	sg_set_buf(sg, &resp->encrypted, sizeof(resp->encrypted));
	skcipher_request_set_sync_tfm(req, conn->rxkad.cipher);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg, sg, sizeof(resp->encrypted), iv.x);
	crypto_skcipher_encrypt(req);
	skcipher_request_free(req);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * respond to a challenge packet
 */
static int rxkad_respond_to_challenge(struct rxrpc_connection *conn,
<<<<<<< HEAD
				      struct sk_buff *skb,
				      u32 *_abort_code)
{
	const struct rxrpc_key_token *token;
	struct rxkad_challenge challenge;
	struct rxkad_response resp
		__attribute__((aligned(8))); /* must be aligned for crypto */
	struct rxrpc_skb_priv *sp;
	u32 version, nonce, min_level, abort_code;
	int ret;

	_enter("{%d,%x}", conn->debug_id, key_serial(conn->key));

	if (!conn->key) {
		_leave(" = -EPROTO [no key]");
		return -EPROTO;
	}

	ret = key_validate(conn->key);
	if (ret < 0) {
		*_abort_code = RXKADEXPIRED;
		return ret;
	}

	abort_code = RXKADPACKETSHORT;
	sp = rxrpc_skb(skb);
	if (skb_copy_bits(skb, 0, &challenge, sizeof(challenge)) < 0)
		goto protocol_error;
=======
				      struct sk_buff *skb)
{
	const struct rxrpc_key_token *token;
	struct rxkad_challenge challenge;
	struct rxkad_response *resp;
	struct rxrpc_skb_priv *sp = rxrpc_skb(skb);
	u32 version, nonce, min_level;
	int ret = -EPROTO;

	_enter("{%d,%x}", conn->debug_id, key_serial(conn->key));

	if (!conn->key)
		return rxrpc_abort_conn(conn, skb, RX_PROTOCOL_ERROR, -EPROTO,
					rxkad_abort_chall_no_key);

	ret = key_validate(conn->key);
	if (ret < 0)
		return rxrpc_abort_conn(conn, skb, RXKADEXPIRED, ret,
					rxkad_abort_chall_key_expired);

	if (skb_copy_bits(skb, sizeof(struct rxrpc_wire_header),
			  &challenge, sizeof(challenge)) < 0)
		return rxrpc_abort_conn(conn, skb, RXKADPACKETSHORT, -EPROTO,
					rxkad_abort_chall_short);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	version = ntohl(challenge.version);
	nonce = ntohl(challenge.nonce);
	min_level = ntohl(challenge.min_level);

<<<<<<< HEAD
	_proto("Rx CHALLENGE %%%u { v=%u n=%u ml=%u }",
	       ntohl(sp->hdr.serial), version, nonce, min_level);

	abort_code = RXKADINCONSISTENCY;
	if (version != RXKAD_VERSION)
		goto protocol_error;

	abort_code = RXKADLEVELFAIL;
	if (conn->security_level < min_level)
		goto protocol_error;

	token = conn->key->payload.data;

	/* build the response packet */
	memset(&resp, 0, sizeof(resp));

	resp.version = RXKAD_VERSION;
	resp.encrypted.epoch = conn->epoch;
	resp.encrypted.cid = conn->cid;
	resp.encrypted.securityIndex = htonl(conn->security_ix);
	resp.encrypted.call_id[0] =
		(conn->channels[0] ? conn->channels[0]->call_id : 0);
	resp.encrypted.call_id[1] =
		(conn->channels[1] ? conn->channels[1]->call_id : 0);
	resp.encrypted.call_id[2] =
		(conn->channels[2] ? conn->channels[2]->call_id : 0);
	resp.encrypted.call_id[3] =
		(conn->channels[3] ? conn->channels[3]->call_id : 0);
	resp.encrypted.inc_nonce = htonl(nonce + 1);
	resp.encrypted.level = htonl(conn->security_level);
	resp.kvno = htonl(token->kad->kvno);
	resp.ticket_len = htonl(token->kad->ticket_len);

	/* calculate the response checksum and then do the encryption */
	rxkad_calc_response_checksum(&resp);
	rxkad_encrypt_response(conn, &resp, token->kad);
	return rxkad_send_response(conn, &sp->hdr, &resp, token->kad);

protocol_error:
	*_abort_code = abort_code;
	_leave(" = -EPROTO [%d]", abort_code);
	return -EPROTO;
=======
	trace_rxrpc_rx_challenge(conn, sp->hdr.serial, version, nonce, min_level);

	if (version != RXKAD_VERSION)
		return rxrpc_abort_conn(conn, skb, RXKADINCONSISTENCY, -EPROTO,
					rxkad_abort_chall_version);

	if (conn->security_level < min_level)
		return rxrpc_abort_conn(conn, skb, RXKADLEVELFAIL, -EACCES,
					rxkad_abort_chall_level);

	token = conn->key->payload.data[0];

	/* build the response packet */
	resp = kzalloc(sizeof(struct rxkad_response), GFP_NOFS);
	if (!resp)
		return -ENOMEM;

	resp->version			= htonl(RXKAD_VERSION);
	resp->encrypted.epoch		= htonl(conn->proto.epoch);
	resp->encrypted.cid		= htonl(conn->proto.cid);
	resp->encrypted.securityIndex	= htonl(conn->security_ix);
	resp->encrypted.inc_nonce	= htonl(nonce + 1);
	resp->encrypted.level		= htonl(conn->security_level);
	resp->kvno			= htonl(token->kad->kvno);
	resp->ticket_len		= htonl(token->kad->ticket_len);
	resp->encrypted.call_id[0]	= htonl(conn->channels[0].call_counter);
	resp->encrypted.call_id[1]	= htonl(conn->channels[1].call_counter);
	resp->encrypted.call_id[2]	= htonl(conn->channels[2].call_counter);
	resp->encrypted.call_id[3]	= htonl(conn->channels[3].call_counter);

	/* calculate the response checksum and then do the encryption */
	rxkad_calc_response_checksum(resp);
	ret = rxkad_encrypt_response(conn, resp, token->kad);
	if (ret == 0)
		ret = rxkad_send_response(conn, &sp->hdr, resp, token->kad);
	kfree(resp);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * decrypt the kerberos IV ticket in the response
 */
static int rxkad_decrypt_ticket(struct rxrpc_connection *conn,
<<<<<<< HEAD
				void *ticket, size_t ticket_len,
				struct rxrpc_crypt *_session_key,
				time_t *_expiry,
				u32 *_abort_code)
{
	struct blkcipher_desc desc;
	struct rxrpc_crypt iv, key;
	struct scatterlist sg[1];
	struct in_addr addr;
	unsigned life;
	time_t issue, now;
	bool little_endian;
	int ret;
	u8 *p, *q, *name, *end;

	_enter("{%d},{%x}", conn->debug_id, key_serial(conn->server_key));

	*_expiry = 0;

	ret = key_validate(conn->server_key);
	if (ret < 0) {
		switch (ret) {
		case -EKEYEXPIRED:
			*_abort_code = RXKADEXPIRED;
			goto error;
		default:
			*_abort_code = RXKADNOAUTH;
			goto error;
		}
	}

	ASSERT(conn->server_key->payload.data != NULL);
	ASSERTCMP((unsigned long) ticket & 7UL, ==, 0);

	memcpy(&iv, &conn->server_key->type_data, sizeof(iv));

	desc.tfm = conn->server_key->payload.data;
	desc.info = iv.x;
	desc.flags = 0;

	sg_init_one(&sg[0], ticket, ticket_len);
	crypto_blkcipher_decrypt_iv(&desc, sg, sg, ticket_len);
=======
				struct key *server_key,
				struct sk_buff *skb,
				void *ticket, size_t ticket_len,
				struct rxrpc_crypt *_session_key,
				time64_t *_expiry)
{
	struct skcipher_request *req;
	struct rxrpc_crypt iv, key;
	struct scatterlist sg[1];
	struct in_addr addr;
	unsigned int life;
	time64_t issue, now;
	bool little_endian;
	u8 *p, *q, *name, *end;

	_enter("{%d},{%x}", conn->debug_id, key_serial(server_key));

	*_expiry = 0;

	ASSERT(server_key->payload.data[0] != NULL);
	ASSERTCMP((unsigned long) ticket & 7UL, ==, 0);

	memcpy(&iv, &server_key->payload.data[2], sizeof(iv));

	req = skcipher_request_alloc(server_key->payload.data[0], GFP_NOFS);
	if (!req)
		return -ENOMEM;

	sg_init_one(&sg[0], ticket, ticket_len);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg, sg, ticket_len, iv.x);
	crypto_skcipher_decrypt(req);
	skcipher_request_free(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	p = ticket;
	end = p + ticket_len;

<<<<<<< HEAD
#define Z(size)						\
	({						\
		u8 *__str = p;				\
		q = memchr(p, 0, end - p);		\
		if (!q || q - p > (size))		\
			goto bad_ticket;		\
		for (; p < q; p++)			\
			if (!isprint(*p))		\
				goto bad_ticket;	\
		p++;					\
		__str;					\
=======
#define Z(field, fieldl)						\
	({								\
		u8 *__str = p;						\
		q = memchr(p, 0, end - p);				\
		if (!q || q - p > field##_SZ)				\
			return rxrpc_abort_conn(			\
				conn, skb, RXKADBADTICKET, -EPROTO,	\
				rxkad_abort_resp_tkt_##fieldl);		\
		for (; p < q; p++)					\
			if (!isprint(*p))				\
				return rxrpc_abort_conn(		\
					conn, skb, RXKADBADTICKET, -EPROTO, \
					rxkad_abort_resp_tkt_##fieldl);	\
		p++;							\
		__str;							\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	})

	/* extract the ticket flags */
	_debug("KIV FLAGS: %x", *p);
	little_endian = *p & 1;
	p++;

	/* extract the authentication name */
<<<<<<< HEAD
	name = Z(ANAME_SZ);
	_debug("KIV ANAME: %s", name);

	/* extract the principal's instance */
	name = Z(INST_SZ);
	_debug("KIV INST : %s", name);

	/* extract the principal's authentication domain */
	name = Z(REALM_SZ);
	_debug("KIV REALM: %s", name);

	if (end - p < 4 + 8 + 4 + 2)
		goto bad_ticket;
=======
	name = Z(ANAME, aname);
	_debug("KIV ANAME: %s", name);

	/* extract the principal's instance */
	name = Z(INST, inst);
	_debug("KIV INST : %s", name);

	/* extract the principal's authentication domain */
	name = Z(REALM, realm);
	_debug("KIV REALM: %s", name);

	if (end - p < 4 + 8 + 4 + 2)
		return rxrpc_abort_conn(conn, skb, RXKADBADTICKET, -EPROTO,
					rxkad_abort_resp_tkt_short);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* get the IPv4 address of the entity that requested the ticket */
	memcpy(&addr, p, sizeof(addr));
	p += 4;
	_debug("KIV ADDR : %pI4", &addr);

	/* get the session key from the ticket */
	memcpy(&key, p, sizeof(key));
	p += 8;
	_debug("KIV KEY  : %08x %08x", ntohl(key.n[0]), ntohl(key.n[1]));
	memcpy(_session_key, &key, sizeof(key));

	/* get the ticket's lifetime */
	life = *p++ * 5 * 60;
	_debug("KIV LIFE : %u", life);

	/* get the issue time of the ticket */
	if (little_endian) {
		__le32 stamp;
		memcpy(&stamp, p, 4);
<<<<<<< HEAD
		issue = le32_to_cpu(stamp);
	} else {
		__be32 stamp;
		memcpy(&stamp, p, 4);
		issue = be32_to_cpu(stamp);
	}
	p += 4;
	now = get_seconds();
	_debug("KIV ISSUE: %lx [%lx]", issue, now);

	/* check the ticket is in date */
	if (issue > now) {
		*_abort_code = RXKADNOAUTH;
		ret = -EKEYREJECTED;
		goto error;
	}

	if (issue < now - life) {
		*_abort_code = RXKADEXPIRED;
		ret = -EKEYEXPIRED;
		goto error;
	}
=======
		issue = rxrpc_u32_to_time64(le32_to_cpu(stamp));
	} else {
		__be32 stamp;
		memcpy(&stamp, p, 4);
		issue = rxrpc_u32_to_time64(be32_to_cpu(stamp));
	}
	p += 4;
	now = ktime_get_real_seconds();
	_debug("KIV ISSUE: %llx [%llx]", issue, now);

	/* check the ticket is in date */
	if (issue > now)
		return rxrpc_abort_conn(conn, skb, RXKADNOAUTH, -EKEYREJECTED,
					rxkad_abort_resp_tkt_future);
	if (issue < now - life)
		return rxrpc_abort_conn(conn, skb, RXKADEXPIRED, -EKEYEXPIRED,
					rxkad_abort_resp_tkt_expired);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*_expiry = issue + life;

	/* get the service name */
<<<<<<< HEAD
	name = Z(SNAME_SZ);
	_debug("KIV SNAME: %s", name);

	/* get the service instance name */
	name = Z(INST_SZ);
	_debug("KIV SINST: %s", name);

	ret = 0;
error:
	_leave(" = %d", ret);
	return ret;

bad_ticket:
	*_abort_code = RXKADBADTICKET;
	ret = -EBADMSG;
	goto error;
=======
	name = Z(SNAME, sname);
	_debug("KIV SNAME: %s", name);

	/* get the service instance name */
	name = Z(INST, sinst);
	_debug("KIV SINST: %s", name);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * decrypt the response packet
 */
static void rxkad_decrypt_response(struct rxrpc_connection *conn,
				   struct rxkad_response *resp,
				   const struct rxrpc_crypt *session_key)
{
<<<<<<< HEAD
	struct blkcipher_desc desc;
	struct scatterlist sg[2];
=======
	struct skcipher_request *req = rxkad_ci_req;
	struct scatterlist sg[1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rxrpc_crypt iv;

	_enter(",,%08x%08x",
	       ntohl(session_key->n[0]), ntohl(session_key->n[1]));

<<<<<<< HEAD
	ASSERT(rxkad_ci != NULL);

	mutex_lock(&rxkad_ci_mutex);
	if (crypto_blkcipher_setkey(rxkad_ci, session_key->x,
				    sizeof(*session_key)) < 0)
		BUG();

	memcpy(&iv, session_key, sizeof(iv));
	desc.tfm = rxkad_ci;
	desc.info = iv.x;
	desc.flags = 0;

	rxkad_sg_set_buf2(sg, &resp->encrypted, sizeof(resp->encrypted));
	crypto_blkcipher_decrypt_iv(&desc, sg, sg, sizeof(resp->encrypted));
=======
	mutex_lock(&rxkad_ci_mutex);
	if (crypto_sync_skcipher_setkey(rxkad_ci, session_key->x,
					sizeof(*session_key)) < 0)
		BUG();

	memcpy(&iv, session_key, sizeof(iv));

	sg_init_table(sg, 1);
	sg_set_buf(sg, &resp->encrypted, sizeof(resp->encrypted));
	skcipher_request_set_sync_tfm(req, rxkad_ci);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg, sg, sizeof(resp->encrypted), iv.x);
	crypto_skcipher_decrypt(req);
	skcipher_request_zero(req);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&rxkad_ci_mutex);

	_leave("");
}

/*
 * verify a response
 */
static int rxkad_verify_response(struct rxrpc_connection *conn,
<<<<<<< HEAD
				 struct sk_buff *skb,
				 u32 *_abort_code)
{
	struct rxkad_response response
		__attribute__((aligned(8))); /* must be aligned for crypto */
	struct rxrpc_skb_priv *sp;
	struct rxrpc_crypt session_key;
	time_t expiry;
	void *ticket;
	u32 abort_code, version, kvno, ticket_len, level;
	__be32 csum;
	int ret;

	_enter("{%d,%x}", conn->debug_id, key_serial(conn->server_key));

	abort_code = RXKADPACKETSHORT;
	if (skb_copy_bits(skb, 0, &response, sizeof(response)) < 0)
		goto protocol_error;
	if (!pskb_pull(skb, sizeof(response)))
		BUG();

	version = ntohl(response.version);
	ticket_len = ntohl(response.ticket_len);
	kvno = ntohl(response.kvno);
	sp = rxrpc_skb(skb);
	_proto("Rx RESPONSE %%%u { v=%u kv=%u tl=%u }",
	       ntohl(sp->hdr.serial), version, kvno, ticket_len);

	abort_code = RXKADINCONSISTENCY;
	if (version != RXKAD_VERSION)
		goto protocol_error;

	abort_code = RXKADTICKETLEN;
	if (ticket_len < 4 || ticket_len > MAXKRB5TICKETLEN)
		goto protocol_error;

	abort_code = RXKADUNKNOWNKEY;
	if (kvno >= RXKAD_TKT_TYPE_KERBEROS_V5)
		goto protocol_error;

	/* extract the kerberos ticket and decrypt and decode it */
	ticket = kmalloc(ticket_len, GFP_NOFS);
	if (!ticket)
		return -ENOMEM;

	abort_code = RXKADPACKETSHORT;
	if (skb_copy_bits(skb, 0, ticket, ticket_len) < 0)
		goto protocol_error_free;

	ret = rxkad_decrypt_ticket(conn, ticket, ticket_len, &session_key,
				   &expiry, &abort_code);
	if (ret < 0) {
		*_abort_code = abort_code;
		kfree(ticket);
		return ret;
	}

	/* use the session key from inside the ticket to decrypt the
	 * response */
	rxkad_decrypt_response(conn, &response, &session_key);

	abort_code = RXKADSEALEDINCON;
	if (response.encrypted.epoch != conn->epoch)
		goto protocol_error_free;
	if (response.encrypted.cid != conn->cid)
		goto protocol_error_free;
	if (ntohl(response.encrypted.securityIndex) != conn->security_ix)
		goto protocol_error_free;
	csum = response.encrypted.checksum;
	response.encrypted.checksum = 0;
	rxkad_calc_response_checksum(&response);
	if (response.encrypted.checksum != csum)
		goto protocol_error_free;

	if (ntohl(response.encrypted.call_id[0]) > INT_MAX ||
	    ntohl(response.encrypted.call_id[1]) > INT_MAX ||
	    ntohl(response.encrypted.call_id[2]) > INT_MAX ||
	    ntohl(response.encrypted.call_id[3]) > INT_MAX)
		goto protocol_error_free;

	abort_code = RXKADOUTOFSEQUENCE;
	if (response.encrypted.inc_nonce != htonl(conn->security_nonce + 1))
		goto protocol_error_free;

	abort_code = RXKADLEVELFAIL;
	level = ntohl(response.encrypted.level);
	if (level > RXRPC_SECURITY_ENCRYPT)
		goto protocol_error_free;
=======
				 struct sk_buff *skb)
{
	struct rxkad_response *response;
	struct rxrpc_skb_priv *sp = rxrpc_skb(skb);
	struct rxrpc_crypt session_key;
	struct key *server_key;
	time64_t expiry;
	void *ticket;
	u32 version, kvno, ticket_len, level;
	__be32 csum;
	int ret, i;

	_enter("{%d}", conn->debug_id);

	server_key = rxrpc_look_up_server_security(conn, skb, 0, 0);
	if (IS_ERR(server_key)) {
		ret = PTR_ERR(server_key);
		switch (ret) {
		case -ENOKEY:
			return rxrpc_abort_conn(conn, skb, RXKADUNKNOWNKEY, ret,
						rxkad_abort_resp_nokey);
		case -EKEYEXPIRED:
			return rxrpc_abort_conn(conn, skb, RXKADEXPIRED, ret,
						rxkad_abort_resp_key_expired);
		default:
			return rxrpc_abort_conn(conn, skb, RXKADNOAUTH, ret,
						rxkad_abort_resp_key_rejected);
		}
	}

	ret = -ENOMEM;
	response = kzalloc(sizeof(struct rxkad_response), GFP_NOFS);
	if (!response)
		goto temporary_error;

	if (skb_copy_bits(skb, sizeof(struct rxrpc_wire_header),
			  response, sizeof(*response)) < 0) {
		rxrpc_abort_conn(conn, skb, RXKADPACKETSHORT, -EPROTO,
				 rxkad_abort_resp_short);
		goto protocol_error;
	}

	version = ntohl(response->version);
	ticket_len = ntohl(response->ticket_len);
	kvno = ntohl(response->kvno);

	trace_rxrpc_rx_response(conn, sp->hdr.serial, version, kvno, ticket_len);

	if (version != RXKAD_VERSION) {
		rxrpc_abort_conn(conn, skb, RXKADINCONSISTENCY, -EPROTO,
				 rxkad_abort_resp_version);
		goto protocol_error;
	}

	if (ticket_len < 4 || ticket_len > MAXKRB5TICKETLEN) {
		rxrpc_abort_conn(conn, skb, RXKADTICKETLEN, -EPROTO,
				 rxkad_abort_resp_tkt_len);
		goto protocol_error;
	}

	if (kvno >= RXKAD_TKT_TYPE_KERBEROS_V5) {
		rxrpc_abort_conn(conn, skb, RXKADUNKNOWNKEY, -EPROTO,
				 rxkad_abort_resp_unknown_tkt);
		goto protocol_error;
	}

	/* extract the kerberos ticket and decrypt and decode it */
	ret = -ENOMEM;
	ticket = kmalloc(ticket_len, GFP_NOFS);
	if (!ticket)
		goto temporary_error_free_resp;

	if (skb_copy_bits(skb, sizeof(struct rxrpc_wire_header) + sizeof(*response),
			  ticket, ticket_len) < 0) {
		rxrpc_abort_conn(conn, skb, RXKADPACKETSHORT, -EPROTO,
				 rxkad_abort_resp_short_tkt);
		goto protocol_error;
	}

	ret = rxkad_decrypt_ticket(conn, server_key, skb, ticket, ticket_len,
				   &session_key, &expiry);
	if (ret < 0)
		goto temporary_error_free_ticket;

	/* use the session key from inside the ticket to decrypt the
	 * response */
	rxkad_decrypt_response(conn, response, &session_key);

	if (ntohl(response->encrypted.epoch) != conn->proto.epoch ||
	    ntohl(response->encrypted.cid) != conn->proto.cid ||
	    ntohl(response->encrypted.securityIndex) != conn->security_ix) {
		rxrpc_abort_conn(conn, skb, RXKADSEALEDINCON, -EPROTO,
				 rxkad_abort_resp_bad_param);
		goto protocol_error_free;
	}

	csum = response->encrypted.checksum;
	response->encrypted.checksum = 0;
	rxkad_calc_response_checksum(response);
	if (response->encrypted.checksum != csum) {
		rxrpc_abort_conn(conn, skb, RXKADSEALEDINCON, -EPROTO,
				 rxkad_abort_resp_bad_checksum);
		goto protocol_error_free;
	}

	for (i = 0; i < RXRPC_MAXCALLS; i++) {
		u32 call_id = ntohl(response->encrypted.call_id[i]);
		u32 counter = READ_ONCE(conn->channels[i].call_counter);

		if (call_id > INT_MAX) {
			rxrpc_abort_conn(conn, skb, RXKADSEALEDINCON, -EPROTO,
					 rxkad_abort_resp_bad_callid);
			goto protocol_error_free;
		}

		if (call_id < counter) {
			rxrpc_abort_conn(conn, skb, RXKADSEALEDINCON, -EPROTO,
					 rxkad_abort_resp_call_ctr);
			goto protocol_error_free;
		}

		if (call_id > counter) {
			if (conn->channels[i].call) {
				rxrpc_abort_conn(conn, skb, RXKADSEALEDINCON, -EPROTO,
						 rxkad_abort_resp_call_state);
				goto protocol_error_free;
			}
			conn->channels[i].call_counter = call_id;
		}
	}

	if (ntohl(response->encrypted.inc_nonce) != conn->rxkad.nonce + 1) {
		rxrpc_abort_conn(conn, skb, RXKADOUTOFSEQUENCE, -EPROTO,
				 rxkad_abort_resp_ooseq);
		goto protocol_error_free;
	}

	level = ntohl(response->encrypted.level);
	if (level > RXRPC_SECURITY_ENCRYPT) {
		rxrpc_abort_conn(conn, skb, RXKADLEVELFAIL, -EPROTO,
				 rxkad_abort_resp_level);
		goto protocol_error_free;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	conn->security_level = level;

	/* create a key to hold the security data and expiration time - after
	 * this the connection security can be handled in exactly the same way
	 * as for a client connection */
	ret = rxrpc_get_server_data_key(conn, &session_key, expiry, kvno);
<<<<<<< HEAD
	if (ret < 0) {
		kfree(ticket);
		return ret;
	}

	kfree(ticket);
=======
	if (ret < 0)
		goto temporary_error_free_ticket;

	kfree(ticket);
	kfree(response);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	_leave(" = 0");
	return 0;

protocol_error_free:
	kfree(ticket);
protocol_error:
<<<<<<< HEAD
	*_abort_code = abort_code;
	_leave(" = -EPROTO [%d]", abort_code);
	return -EPROTO;
=======
	kfree(response);
	key_put(server_key);
	return -EPROTO;

temporary_error_free_ticket:
	kfree(ticket);
temporary_error_free_resp:
	kfree(response);
temporary_error:
	/* Ignore the response packet if we got a temporary error such as
	 * ENOMEM.  We just want to send the challenge again.  Note that we
	 * also come out this way if the ticket decryption fails.
	 */
	key_put(server_key);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * clear the connection security
 */
static void rxkad_clear(struct rxrpc_connection *conn)
{
	_enter("");

<<<<<<< HEAD
	if (conn->cipher)
		crypto_free_blkcipher(conn->cipher);
=======
	if (conn->rxkad.cipher)
		crypto_free_sync_skcipher(conn->rxkad.cipher);
}

/*
 * Initialise the rxkad security service.
 */
static int rxkad_init(void)
{
	struct crypto_sync_skcipher *tfm;
	struct skcipher_request *req;

	/* pin the cipher we need so that the crypto layer doesn't invoke
	 * keventd to go get it */
	tfm = crypto_alloc_sync_skcipher("pcbc(fcrypt)", 0, 0);
	if (IS_ERR(tfm))
		return PTR_ERR(tfm);

	req = skcipher_request_alloc(&tfm->base, GFP_KERNEL);
	if (!req)
		goto nomem_tfm;

	rxkad_ci_req = req;
	rxkad_ci = tfm;
	return 0;

nomem_tfm:
	crypto_free_sync_skcipher(tfm);
	return -ENOMEM;
}

/*
 * Clean up the rxkad security service.
 */
static void rxkad_exit(void)
{
	crypto_free_sync_skcipher(rxkad_ci);
	skcipher_request_free(rxkad_ci_req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * RxRPC Kerberos-based security
 */
<<<<<<< HEAD
static struct rxrpc_security rxkad = {
	.owner				= THIS_MODULE,
	.name				= "rxkad",
	.security_index			= RXRPC_SECURITY_RXKAD,
	.init_connection_security	= rxkad_init_connection_security,
	.prime_packet_security		= rxkad_prime_packet_security,
	.secure_packet			= rxkad_secure_packet,
	.verify_packet			= rxkad_verify_packet,
=======
const struct rxrpc_security rxkad = {
	.name				= "rxkad",
	.security_index			= RXRPC_SECURITY_RXKAD,
	.no_key_abort			= RXKADUNKNOWNKEY,
	.init				= rxkad_init,
	.exit				= rxkad_exit,
	.preparse_server_key		= rxkad_preparse_server_key,
	.free_preparse_server_key	= rxkad_free_preparse_server_key,
	.destroy_server_key		= rxkad_destroy_server_key,
	.init_connection_security	= rxkad_init_connection_security,
	.alloc_txbuf			= rxkad_alloc_txbuf,
	.secure_packet			= rxkad_secure_packet,
	.verify_packet			= rxkad_verify_packet,
	.free_call_crypto		= rxkad_free_call_crypto,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.issue_challenge		= rxkad_issue_challenge,
	.respond_to_challenge		= rxkad_respond_to_challenge,
	.verify_response		= rxkad_verify_response,
	.clear				= rxkad_clear,
};
<<<<<<< HEAD

static __init int rxkad_init(void)
{
	_enter("");

	/* pin the cipher we need so that the crypto layer doesn't invoke
	 * keventd to go get it */
	rxkad_ci = crypto_alloc_blkcipher("pcbc(fcrypt)", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(rxkad_ci))
		return PTR_ERR(rxkad_ci);

	return rxrpc_register_security(&rxkad);
}

module_init(rxkad_init);

static __exit void rxkad_exit(void)
{
	_enter("");

	rxrpc_unregister_security(&rxkad);
	crypto_free_blkcipher(rxkad_ci);
}

module_exit(rxkad_exit);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
