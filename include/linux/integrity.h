<<<<<<< HEAD
/*
 * Copyright (C) 2009 IBM Corporation
 * Author: Mimi Zohar <zohar@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2009 IBM Corporation
 * Author: Mimi Zohar <zohar@us.ibm.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _LINUX_INTEGRITY_H
#define _LINUX_INTEGRITY_H

#include <linux/fs.h>

enum integrity_status {
	INTEGRITY_PASS = 0,
<<<<<<< HEAD
	INTEGRITY_FAIL,
=======
	INTEGRITY_PASS_IMMUTABLE,
	INTEGRITY_FAIL,
	INTEGRITY_FAIL_IMMUTABLE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INTEGRITY_NOLABEL,
	INTEGRITY_NOXATTRS,
	INTEGRITY_UNKNOWN,
};

<<<<<<< HEAD
/* List of EVM protected security xattrs */
#ifdef CONFIG_INTEGRITY
extern int integrity_inode_alloc(struct inode *inode);
extern void integrity_inode_free(struct inode *inode);

#else
static inline int integrity_inode_alloc(struct inode *inode)
{
	return 0;
}

static inline void integrity_inode_free(struct inode *inode)
{
	return;
}
#endif /* CONFIG_INTEGRITY_H */
=======
#ifdef CONFIG_INTEGRITY
extern void __init integrity_load_keys(void);

#else
static inline void integrity_load_keys(void)
{
}
#endif /* CONFIG_INTEGRITY */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_INTEGRITY_H */
