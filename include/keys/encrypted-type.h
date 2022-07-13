<<<<<<< HEAD
/*
 * Copyright (C) 2010 IBM Corporation
 * Copyright (C) 2010 Politecnico di Torino, Italy
 *                    TORSEC group -- http://security.polito.it
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2010 IBM Corporation
 * Copyright (C) 2010 Politecnico di Torino, Italy
 *                    TORSEC group -- https://security.polito.it
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Authors:
 * Mimi Zohar <zohar@us.ibm.com>
 * Roberto Sassu <roberto.sassu@polito.it>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _KEYS_ENCRYPTED_TYPE_H
#define _KEYS_ENCRYPTED_TYPE_H

#include <linux/key.h>
#include <linux/rcupdate.h>

struct encrypted_key_payload {
	struct rcu_head rcu;
	char *format;		/* datablob: format */
	char *master_desc;	/* datablob: master key name */
	char *datalen;		/* datablob: decrypted key length */
	u8 *iv;			/* datablob: iv */
	u8 *encrypted_data;	/* datablob: encrypted data */
	unsigned short datablob_len;	/* length of datablob */
	unsigned short decrypted_datalen;	/* decrypted data length */
	unsigned short payload_datalen;		/* payload data length */
	unsigned short encrypted_key_format;	/* encrypted key format */
	u8 *decrypted_data;	/* decrypted data */
<<<<<<< HEAD
	u8 payload_data[0];	/* payload data + datablob + hmac */
=======
	u8 payload_data[];	/* payload data + datablob + hmac */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

extern struct key_type key_type_encrypted;

#endif /* _KEYS_ENCRYPTED_TYPE_H */
