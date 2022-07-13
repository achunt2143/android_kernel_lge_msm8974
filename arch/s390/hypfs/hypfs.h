<<<<<<< HEAD
/*
 *  arch/s390/hypfs/hypfs.h
 *    Hypervisor filesystem for Linux on s390.
 *
 *    Copyright (C) IBM Corp. 2006
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *    Hypervisor filesystem for Linux on s390.
 *
 *    Copyright IBM Corp. 2006
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Michael Holzheu <holzheu@de.ibm.com>
 */

#ifndef _HYPFS_H_
#define _HYPFS_H_

#include <linux/fs.h>
#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/workqueue.h>
#include <linux/kref.h>
<<<<<<< HEAD
=======
#include <asm/hypfs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define REG_FILE_MODE    0440
#define UPDATE_FILE_MODE 0220
#define DIR_MODE         0550

<<<<<<< HEAD
extern struct dentry *hypfs_mkdir(struct super_block *sb, struct dentry *parent,
				  const char *name);

extern struct dentry *hypfs_create_u64(struct super_block *sb,
				       struct dentry *dir, const char *name,
				       __u64 value);

extern struct dentry *hypfs_create_str(struct super_block *sb,
				       struct dentry *dir, const char *name,
=======
extern struct dentry *hypfs_mkdir(struct dentry *parent, const char *name);

extern struct dentry *hypfs_create_u64(struct dentry *dir, const char *name,
				       __u64 value);

extern struct dentry *hypfs_create_str(struct dentry *dir, const char *name,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				       char *string);

/* LPAR Hypervisor */
extern int hypfs_diag_init(void);
extern void hypfs_diag_exit(void);
<<<<<<< HEAD
extern int hypfs_diag_create_files(struct super_block *sb, struct dentry *root);
=======
extern int hypfs_diag_create_files(struct dentry *root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* VM Hypervisor */
extern int hypfs_vm_init(void);
extern void hypfs_vm_exit(void);
<<<<<<< HEAD
extern int hypfs_vm_create_files(struct super_block *sb, struct dentry *root);
=======
extern int hypfs_vm_create_files(struct dentry *root);

/* VM diagnose 0c */
int hypfs_diag0c_init(void);
void hypfs_diag0c_exit(void);

/* Set Partition-Resource Parameter */
void hypfs_sprp_init(void);
void hypfs_sprp_exit(void);

int __hypfs_fs_init(void);

static inline int hypfs_fs_init(void)
{
	if (IS_ENABLED(CONFIG_S390_HYPFS_FS))
		return __hypfs_fs_init();
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* debugfs interface */
struct hypfs_dbfs_file;

struct hypfs_dbfs_data {
	void			*buf;
	void			*buf_free_ptr;
	size_t			size;
	struct hypfs_dbfs_file	*dbfs_file;
<<<<<<< HEAD
	struct kref		kref;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct hypfs_dbfs_file {
	const char	*name;
	int		(*data_create)(void **data, void **data_free_ptr,
				       size_t *size);
	void		(*data_free)(const void *buf_free_ptr);
<<<<<<< HEAD

	/* Private data for hypfs_dbfs.c */
	struct hypfs_dbfs_data	*data;
	struct delayed_work	data_free_work;
=======
	long		(*unlocked_ioctl) (struct file *, unsigned int,
					   unsigned long);

	/* Private data for hypfs_dbfs.c */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mutex		lock;
	struct dentry		*dentry;
};

<<<<<<< HEAD
extern int hypfs_dbfs_init(void);
extern void hypfs_dbfs_exit(void);
extern int hypfs_dbfs_create_file(struct hypfs_dbfs_file *df);
=======
extern void hypfs_dbfs_exit(void);
extern void hypfs_dbfs_create_file(struct hypfs_dbfs_file *df);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void hypfs_dbfs_remove_file(struct hypfs_dbfs_file *df);

#endif /* _HYPFS_H_ */
