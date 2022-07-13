<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ecryptfs_format.h: helper functions for the encrypted key type
 *
 * Copyright (C) 2006 International Business Machines Corp.
 * Copyright (C) 2010 Politecnico di Torino, Italy
<<<<<<< HEAD
 *                    TORSEC group -- http://security.polito.it
=======
 *                    TORSEC group -- https://security.polito.it
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Authors:
 * Michael A. Halcrow <mahalcro@us.ibm.com>
 * Tyler Hicks <tyhicks@ou.edu>
 * Roberto Sassu <roberto.sassu@polito.it>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __KEYS_ECRYPTFS_H
#define __KEYS_ECRYPTFS_H

#include <linux/ecryptfs.h>

#define PGP_DIGEST_ALGO_SHA512   10

u8 *ecryptfs_get_auth_tok_key(struct ecryptfs_auth_tok *auth_tok);
void ecryptfs_get_versions(int *major, int *minor, int *file_version);
int ecryptfs_fill_auth_tok(struct ecryptfs_auth_tok *auth_tok,
			   const char *key_desc);

#endif /* __KEYS_ECRYPTFS_H */
