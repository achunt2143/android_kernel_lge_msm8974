<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Key permission checking
 *
 * Copyright (C) 2005 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
=======
 */

#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/security.h>
#include "internal.h"

/**
 * key_task_permission - Check a key can be used
 * @key_ref: The key to check.
 * @cred: The credentials to use.
<<<<<<< HEAD
 * @perm: The permissions to check for.
=======
 * @need_perm: The permission required.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Check to see whether permission is granted to use a key in the desired way,
 * but permit the security modules to override.
 *
 * The caller must hold either a ref on cred or must hold the RCU readlock.
 *
 * Returns 0 if successful, -EACCES if access is denied based on the
 * permissions bits or the LSM check.
 */
int key_task_permission(const key_ref_t key_ref, const struct cred *cred,
<<<<<<< HEAD
			key_perm_t perm)
{
	struct key *key;
	key_perm_t kperm;
	int ret;

	key = key_ref_to_ptr(key_ref);

	if (key->user->user_ns != cred->user->user_ns)
		goto use_other_perms;

	/* use the second 8-bits of permissions for keys the caller owns */
	if (key->uid == cred->fsuid) {
=======
			enum key_need_perm need_perm)
{
	struct key *key;
	key_perm_t kperm, mask;
	int ret;

	switch (need_perm) {
	default:
		WARN_ON(1);
		return -EACCES;
	case KEY_NEED_UNLINK:
	case KEY_SYSADMIN_OVERRIDE:
	case KEY_AUTHTOKEN_OVERRIDE:
	case KEY_DEFER_PERM_CHECK:
		goto lsm;

	case KEY_NEED_VIEW:	mask = KEY_OTH_VIEW;	break;
	case KEY_NEED_READ:	mask = KEY_OTH_READ;	break;
	case KEY_NEED_WRITE:	mask = KEY_OTH_WRITE;	break;
	case KEY_NEED_SEARCH:	mask = KEY_OTH_SEARCH;	break;
	case KEY_NEED_LINK:	mask = KEY_OTH_LINK;	break;
	case KEY_NEED_SETATTR:	mask = KEY_OTH_SETATTR;	break;
	}

	key = key_ref_to_ptr(key_ref);

	/* use the second 8-bits of permissions for keys the caller owns */
	if (uid_eq(key->uid, cred->fsuid)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kperm = key->perm >> 16;
		goto use_these_perms;
	}

	/* use the third 8-bits of permissions for keys the caller has a group
	 * membership in common with */
<<<<<<< HEAD
	if (key->gid != -1 && key->perm & KEY_GRP_ALL) {
		if (key->gid == cred->fsgid) {
=======
	if (gid_valid(key->gid) && key->perm & KEY_GRP_ALL) {
		if (gid_eq(key->gid, cred->fsgid)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			kperm = key->perm >> 8;
			goto use_these_perms;
		}

		ret = groups_search(cred->group_info, key->gid);
		if (ret) {
			kperm = key->perm >> 8;
			goto use_these_perms;
		}
	}

<<<<<<< HEAD
use_other_perms:

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* otherwise use the least-significant 8-bits */
	kperm = key->perm;

use_these_perms:

	/* use the top 8-bits of permissions for keys the caller possesses
	 * - possessor permissions are additive with other permissions
	 */
	if (is_key_possessed(key_ref))
		kperm |= key->perm >> 24;

<<<<<<< HEAD
	kperm = kperm & perm & KEY_ALL;

	if (kperm != perm)
		return -EACCES;

	/* let LSM be the final arbiter */
	return security_key_permission(key_ref, cred, perm);
=======
	if ((kperm & mask) != mask)
		return -EACCES;

	/* let LSM be the final arbiter */
lsm:
	return security_key_permission(key_ref, cred, need_perm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(key_task_permission);

/**
 * key_validate - Validate a key.
 * @key: The key to be validated.
 *
 * Check that a key is valid, returning 0 if the key is okay, -ENOKEY if the
 * key is invalidated, -EKEYREVOKED if the key's type has been removed or if
 * the key has been revoked or -EKEYEXPIRED if the key has expired.
 */
<<<<<<< HEAD
int key_validate(struct key *key)
{
	struct timespec now;
	unsigned long flags = key->flags;
	int ret = 0;

	if (key) {
		ret = -ENOKEY;
		if (flags & (1 << KEY_FLAG_INVALIDATED))
			goto error;

		/* check it's still accessible */
		ret = -EKEYREVOKED;
		if (flags & ((1 << KEY_FLAG_REVOKED) |
			     (1 << KEY_FLAG_DEAD)))
			goto error;

		/* check it hasn't expired */
		ret = 0;
		if (key->expiry) {
			now = current_kernel_time();
			if (now.tv_sec >= key->expiry)
				ret = -EKEYEXPIRED;
		}
	}

error:
	return ret;
=======
int key_validate(const struct key *key)
{
	unsigned long flags = READ_ONCE(key->flags);
	time64_t expiry = READ_ONCE(key->expiry);

	if (flags & (1 << KEY_FLAG_INVALIDATED))
		return -ENOKEY;

	/* check it's still accessible */
	if (flags & ((1 << KEY_FLAG_REVOKED) |
		     (1 << KEY_FLAG_DEAD)))
		return -EKEYREVOKED;

	/* check it hasn't expired */
	if (expiry) {
		if (ktime_get_real_seconds() >= expiry)
			return -EKEYEXPIRED;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(key_validate);
