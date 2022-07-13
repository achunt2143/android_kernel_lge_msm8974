<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_FS_STACK_H
#define _LINUX_FS_STACK_H

/* This file defines generic functions used primarily by stackable
 * filesystems; none of these functions require i_mutex to be held.
 */

#include <linux/fs.h>

/* externs for fs/stack.c */
extern void fsstack_copy_attr_all(struct inode *dest, const struct inode *src);
extern void fsstack_copy_inode_size(struct inode *dst, struct inode *src);

/* inlines */
static inline void fsstack_copy_attr_atime(struct inode *dest,
					   const struct inode *src)
{
<<<<<<< HEAD
	dest->i_atime = src->i_atime;
=======
	inode_set_atime_to_ts(dest, inode_get_atime(src));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void fsstack_copy_attr_times(struct inode *dest,
					   const struct inode *src)
{
<<<<<<< HEAD
	dest->i_atime = src->i_atime;
	dest->i_mtime = src->i_mtime;
	dest->i_ctime = src->i_ctime;
=======
	inode_set_atime_to_ts(dest, inode_get_atime(src));
	inode_set_mtime_to_ts(dest, inode_get_mtime(src));
	inode_set_ctime_to_ts(dest, inode_get_ctime(src));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif /* _LINUX_FS_STACK_H */
