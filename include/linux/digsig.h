<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2011 Nokia Corporation
 * Copyright (C) 2011 Intel Corporation
 *
 * Author:
 * Dmitry Kasatkin <dmitry.kasatkin@nokia.com>
 *                 <dmitry.kasatkin@intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _DIGSIG_H
#define _DIGSIG_H

#include <linux/key.h>

enum pubkey_algo {
	PUBKEY_ALGO_RSA,
	PUBKEY_ALGO_MAX,
};

enum digest_algo {
	DIGEST_ALGO_SHA1,
	DIGEST_ALGO_SHA256,
	DIGEST_ALGO_MAX
};

struct pubkey_hdr {
	uint8_t		version;	/* key format version */
	uint32_t	timestamp;	/* key made, always 0 for now */
	uint8_t		algo;
	uint8_t		nmpi;
<<<<<<< HEAD
	char		mpi[0];
=======
	char		mpi[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} __packed;

struct signature_hdr {
	uint8_t		version;	/* signature format version */
	uint32_t	timestamp;	/* signature made */
	uint8_t		algo;
	uint8_t		hash;
	uint8_t		keyid[8];
	uint8_t		nmpi;
<<<<<<< HEAD
	char		mpi[0];
=======
	char		mpi[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} __packed;

#if defined(CONFIG_SIGNATURE) || defined(CONFIG_SIGNATURE_MODULE)

int digsig_verify(struct key *keyring, const char *sig, int siglen,
					const char *digest, int digestlen);

#else

static inline int digsig_verify(struct key *keyring, const char *sig,
				int siglen, const char *digest, int digestlen)
{
	return -EOPNOTSUPP;
}

#endif /* CONFIG_SIGNATURE */

#endif /* _DIGSIG_H */
