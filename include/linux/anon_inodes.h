<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  include/linux/anon_inodes.h
 *
 *  Copyright (C) 2007  Davide Libenzi <davidel@xmailserver.org>
 *
 */

#ifndef _LINUX_ANON_INODES_H
#define _LINUX_ANON_INODES_H

struct file_operations;
<<<<<<< HEAD
=======
struct inode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct file *anon_inode_getfile(const char *name,
				const struct file_operations *fops,
				void *priv, int flags);
<<<<<<< HEAD
int anon_inode_getfd(const char *name, const struct file_operations *fops,
		     void *priv, int flags);
=======
struct file *anon_inode_create_getfile(const char *name,
				       const struct file_operations *fops,
				       void *priv, int flags,
				       const struct inode *context_inode);
int anon_inode_getfd(const char *name, const struct file_operations *fops,
		     void *priv, int flags);
int anon_inode_create_getfd(const char *name,
			    const struct file_operations *fops,
			    void *priv, int flags,
			    const struct inode *context_inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _LINUX_ANON_INODES_H */

