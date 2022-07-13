<<<<<<< HEAD
/* Key to pathname encoder
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* Key to pathname encoder
 *
 * Copyright (C) 2021 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/slab.h>
#include "internal.h"

static const char cachefiles_charmap[64] =
	"0123456789"			/* 0 - 9 */
	"abcdefghijklmnopqrstuvwxyz"	/* 10 - 35 */
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"	/* 36 - 61 */
	"_-"				/* 62 - 63 */
	;

static const char cachefiles_filecharmap[256] = {
	/* we skip space and tab and control chars */
	[33 ... 46] = 1,		/* '!' -> '.' */
	/* we skip '/' as it's significant to pathwalk */
	[48 ... 127] = 1,		/* '0' -> '~' */
};

<<<<<<< HEAD
/*
 * turn the raw key into something cooked
 * - the raw key should include the length in the two bytes at the front
 * - the key may be up to 514 bytes in length (including the length word)
=======
static inline unsigned int how_many_hex_digits(unsigned int x)
{
	return x ? round_up(ilog2(x) + 1, 4) / 4 : 0;
}

/*
 * turn the raw key into something cooked
 * - the key may be up to NAME_MAX in length (including the length word)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *   - "base64" encode the strange keys, mapping 3 bytes of raw to four of
 *     cooked
 *   - need to cut the cooked key into 252 char lengths (189 raw bytes)
 */
<<<<<<< HEAD
char *cachefiles_cook_key(const u8 *raw, int keylen, uint8_t type)
{
	unsigned char csum, ch;
	unsigned int acc;
	char *key;
	int loop, len, max, seg, mark, print;

	_enter(",%d", keylen);

	BUG_ON(keylen < 2 || keylen > 514);

	csum = raw[0] + raw[1];
	print = 1;
	for (loop = 2; loop < keylen; loop++) {
		ch = raw[loop];
		csum += ch;
		print &= cachefiles_filecharmap[ch];
	}

	if (print) {
		/* if the path is usable ASCII, then we render it directly */
		max = keylen - 2;
		max += 2;	/* two base64'd length chars on the front */
		max += 5;	/* @checksum/M */
		max += 3 * 2;	/* maximum number of segment dividers (".../M")
				 * is ((514 + 251) / 252) = 3
				 */
		max += 1;	/* NUL on end */
	} else {
		/* calculate the maximum length of the cooked key */
		keylen = (keylen + 2) / 3;

		max = keylen * 4;
		max += 5;	/* @checksum/M */
		max += 3 * 2;	/* maximum number of segment dividers (".../M")
				 * is ((514 + 188) / 189) = 3
				 */
		max += 1;	/* NUL on end */
	}

	max += 1;	/* 2nd NUL on end */

	_debug("max: %d", max);

	key = kmalloc(max, GFP_KERNEL);
	if (!key)
		return NULL;

	len = 0;

	/* build the cooked key */
	sprintf(key, "@%02x%c+", (unsigned) csum, 0);
	len = 5;
	mark = len - 1;

	if (print) {
		acc = *(uint16_t *) raw;
		raw += 2;

		key[len + 1] = cachefiles_charmap[acc & 63];
		acc >>= 6;
		key[len] = cachefiles_charmap[acc & 63];
		len += 2;

		seg = 250;
		for (loop = keylen; loop > 0; loop--) {
			if (seg <= 0) {
				key[len++] = '\0';
				mark = len;
				key[len++] = '+';
				seg = 252;
			}

			key[len++] = *raw++;
			ASSERT(len < max);
		}

		switch (type) {
		case FSCACHE_COOKIE_TYPE_INDEX:		type = 'I';	break;
		case FSCACHE_COOKIE_TYPE_DATAFILE:	type = 'D';	break;
		default:				type = 'S';	break;
		}
	} else {
		seg = 252;
		for (loop = keylen; loop > 0; loop--) {
			if (seg <= 0) {
				key[len++] = '\0';
				mark = len;
				key[len++] = '+';
				seg = 252;
			}

			acc = *raw++;
			acc |= *raw++ << 8;
			acc |= *raw++ << 16;

			_debug("acc: %06x", acc);

			key[len++] = cachefiles_charmap[acc & 63];
			acc >>= 6;
			key[len++] = cachefiles_charmap[acc & 63];
			acc >>= 6;
			key[len++] = cachefiles_charmap[acc & 63];
			acc >>= 6;
			key[len++] = cachefiles_charmap[acc & 63];

			ASSERT(len < max);
		}

		switch (type) {
		case FSCACHE_COOKIE_TYPE_INDEX:		type = 'J';	break;
		case FSCACHE_COOKIE_TYPE_DATAFILE:	type = 'E';	break;
		default:				type = 'T';	break;
		}
	}

	key[mark] = type;
	key[len++] = 0;
	key[len] = 0;

	_leave(" = %p %d", key, len);
	return key;
=======
bool cachefiles_cook_key(struct cachefiles_object *object)
{
	const u8 *key = fscache_get_key(object->cookie), *kend;
	unsigned char ch;
	unsigned int acc, i, n, nle, nbe, keylen = object->cookie->key_len;
	unsigned int b64len, len, print, pad;
	char *name, sep;

	_enter(",%u,%*phN", keylen, keylen, key);

	BUG_ON(keylen > NAME_MAX - 3);

	print = 1;
	for (i = 0; i < keylen; i++) {
		ch = key[i];
		print &= cachefiles_filecharmap[ch];
	}

	/* If the path is usable ASCII, then we render it directly */
	if (print) {
		len = 1 + keylen;
		name = kmalloc(len + 1, GFP_KERNEL);
		if (!name)
			return false;

		name[0] = 'D'; /* Data object type, string encoding */
		memcpy(name + 1, key, keylen);
		goto success;
	}

	/* See if it makes sense to encode it as "hex,hex,hex" for each 32-bit
	 * chunk.  We rely on the key having been padded out to a whole number
	 * of 32-bit words.
	 */
	n = round_up(keylen, 4);
	nbe = nle = 0;
	for (i = 0; i < n; i += 4) {
		u32 be = be32_to_cpu(*(__be32 *)(key + i));
		u32 le = le32_to_cpu(*(__le32 *)(key + i));

		nbe += 1 + how_many_hex_digits(be);
		nle += 1 + how_many_hex_digits(le);
	}

	b64len = DIV_ROUND_UP(keylen, 3);
	pad = b64len * 3 - keylen;
	b64len = 2 + b64len * 4; /* Length if we base64-encode it */
	_debug("len=%u nbe=%u nle=%u b64=%u", keylen, nbe, nle, b64len);
	if (nbe < b64len || nle < b64len) {
		unsigned int nlen = min(nbe, nle) + 1;
		name = kmalloc(nlen, GFP_KERNEL);
		if (!name)
			return false;
		sep = (nbe <= nle) ? 'S' : 'T'; /* Encoding indicator */
		len = 0;
		for (i = 0; i < n; i += 4) {
			u32 x;
			if (nbe <= nle)
				x = be32_to_cpu(*(__be32 *)(key + i));
			else
				x = le32_to_cpu(*(__le32 *)(key + i));
			name[len++] = sep;
			if (x != 0)
				len += snprintf(name + len, nlen - len, "%x", x);
			sep = ',';
		}
		goto success;
	}

	/* We need to base64-encode it */
	name = kmalloc(b64len + 1, GFP_KERNEL);
	if (!name)
		return false;

	name[0] = 'E';
	name[1] = '0' + pad;
	len = 2;
	kend = key + keylen;
	do {
		acc  = *key++;
		if (key < kend) {
			acc |= *key++ << 8;
			if (key < kend)
				acc |= *key++ << 16;
		}

		name[len++] = cachefiles_charmap[acc & 63];
		acc >>= 6;
		name[len++] = cachefiles_charmap[acc & 63];
		acc >>= 6;
		name[len++] = cachefiles_charmap[acc & 63];
		acc >>= 6;
		name[len++] = cachefiles_charmap[acc & 63];
	} while (key < kend);

success:
	name[len] = 0;
	object->d_name = name;
	object->d_name_len = len;
	_leave(" = %s", object->d_name);
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
