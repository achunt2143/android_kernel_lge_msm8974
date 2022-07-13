<<<<<<< HEAD
#ifndef _FS_CEPH_DEBUGFS_H
#define _FS_CEPH_DEBUGFS_H

#include "ceph_debug.h"
#include "types.h"

#define CEPH_DEFINE_SHOW_FUNC(name)					\
static int name##_open(struct inode *inode, struct file *file)		\
{									\
	struct seq_file *sf;						\
	int ret;							\
									\
	ret = single_open(file, name, NULL);				\
	sf = file->private_data;					\
	sf->private = inode->i_private;					\
	return ret;							\
}									\
									\
static const struct file_operations name##_fops = {			\
	.open		= name##_open,					\
	.read		= seq_read,					\
	.llseek		= seq_lseek,					\
	.release	= single_release,				\
};

/* debugfs.c */
extern int ceph_debugfs_init(void);
extern void ceph_debugfs_cleanup(void);
extern int ceph_debugfs_client_init(struct ceph_client *client);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _FS_CEPH_DEBUGFS_H
#define _FS_CEPH_DEBUGFS_H

#include <linux/ceph/types.h>

/* debugfs.c */
extern void ceph_debugfs_init(void);
extern void ceph_debugfs_cleanup(void);
extern void ceph_debugfs_client_init(struct ceph_client *client);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void ceph_debugfs_client_cleanup(struct ceph_client *client);

#endif

