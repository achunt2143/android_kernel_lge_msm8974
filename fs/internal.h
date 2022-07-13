<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* fs/ internal definitions
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/lglock.h>

struct super_block;
struct file_system_type;
struct linux_binprm;
struct path;
struct mount;

/*
 * block_dev.c
 */
#ifdef CONFIG_BLOCK
extern void __init bdev_cache_init(void);

extern int __sync_blockdev(struct block_device *bdev, int wait);

=======
 */

struct super_block;
struct file_system_type;
struct iomap;
struct iomap_ops;
struct linux_binprm;
struct path;
struct mount;
struct shrink_control;
struct fs_context;
struct pipe_inode_info;
struct iov_iter;
struct mnt_idmap;

/*
 * block/bdev.c
 */
#ifdef CONFIG_BLOCK
extern void __init bdev_cache_init(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
static inline void bdev_cache_init(void)
{
}
<<<<<<< HEAD

static inline int __sync_blockdev(struct block_device *bdev, int wait)
{
	return 0;
}
#endif
=======
#endif /* CONFIG_BLOCK */

/*
 * buffer.c
 */
int __block_write_begin_int(struct folio *folio, loff_t pos, unsigned len,
		get_block_t *get_block, const struct iomap *iomap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * char_dev.c
 */
extern void __init chrdev_init(void);

/*
<<<<<<< HEAD
 * namespace.c
 */
extern int copy_mount_options(const void __user *, unsigned long *);
extern int copy_mount_string(const void __user *, char **);

extern struct vfsmount *lookup_mnt(struct path *);
extern int finish_automount(struct vfsmount *, struct path *);
=======
 * fs_context.c
 */
extern const struct fs_context_operations legacy_fs_context_ops;
extern int parse_monolithic_mount_data(struct fs_context *, void *);
extern void vfs_clean_context(struct fs_context *fc);
extern int finish_clean_context(struct fs_context *fc);

/*
 * namei.c
 */
extern int filename_lookup(int dfd, struct filename *name, unsigned flags,
			   struct path *path, struct path *root);
int do_rmdir(int dfd, struct filename *name);
int do_unlinkat(int dfd, struct filename *name);
int may_linkat(struct mnt_idmap *idmap, const struct path *link);
int do_renameat2(int olddfd, struct filename *oldname, int newdfd,
		 struct filename *newname, unsigned int flags);
int do_mkdirat(int dfd, struct filename *name, umode_t mode);
int do_symlinkat(struct filename *from, int newdfd, struct filename *to);
int do_linkat(int olddfd, struct filename *old, int newdfd,
			struct filename *new, int flags);

/*
 * namespace.c
 */
extern struct vfsmount *lookup_mnt(const struct path *);
extern int finish_automount(struct vfsmount *, const struct path *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int sb_prepare_remount_readonly(struct super_block *);

extern void __init mnt_init(void);

<<<<<<< HEAD
extern struct lglock vfsmount_lock;

=======
int mnt_get_write_access_file(struct file *file);
void mnt_put_write_access_file(struct file *file);

extern void dissolve_on_fput(struct vfsmount *);
extern bool may_mount(void);

int path_mount(const char *dev_name, struct path *path,
		const char *type_page, unsigned long flags, void *data_page);
int path_umount(struct path *path, int flags);

int show_path(struct seq_file *m, struct dentry *root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * fs_struct.c
 */
<<<<<<< HEAD
extern void chroot_fs_refs(struct path *, struct path *);
=======
extern void chroot_fs_refs(const struct path *, const struct path *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * file_table.c
 */
<<<<<<< HEAD
extern struct file *get_empty_filp(void);
=======
struct file *alloc_empty_file(int flags, const struct cred *cred);
struct file *alloc_empty_file_noaccount(int flags, const struct cred *cred);
struct file *alloc_empty_backing_file(int flags, const struct cred *cred);

static inline void file_put_write_access(struct file *file)
{
	put_write_access(file->f_inode);
	mnt_put_write_access(file->f_path.mnt);
	if (unlikely(file->f_mode & FMODE_BACKING))
		mnt_put_write_access(backing_file_user_path(file)->mnt);
}

static inline void put_file_access(struct file *file)
{
	if ((file->f_mode & (FMODE_READ | FMODE_WRITE)) == FMODE_READ) {
		i_readcount_dec(file->f_inode);
	} else if (file->f_mode & FMODE_WRITER) {
		file_put_write_access(file);
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * super.c
 */
<<<<<<< HEAD
extern int do_remount_sb(struct super_block *, int, void *, int);
extern int do_remount_sb2(struct vfsmount *, struct super_block *, int,
								void *, int);
extern bool grab_super_passive(struct super_block *sb);
extern struct dentry *mount_fs(struct file_system_type *,
			       int, const char *, struct vfsmount *, void *);
extern struct super_block *user_get_super(dev_t);
=======
extern int reconfigure_super(struct fs_context *);
extern bool super_trylock_shared(struct super_block *sb);
struct super_block *user_get_super(dev_t, bool excl);
void put_super(struct super_block *sb);
extern bool mount_capable(struct fs_context *);
int sb_init_dio_done_wq(struct super_block *sb);

/*
 * Prepare superblock for changing its read-only state (i.e., either remount
 * read-write superblock read-only or vice versa). After this function returns
 * mnt_is_readonly() will return true for any mount of the superblock if its
 * caller is able to observe any changes done by the remount. This holds until
 * sb_end_ro_state_change() is called.
 */
static inline void sb_start_ro_state_change(struct super_block *sb)
{
	WRITE_ONCE(sb->s_readonly_remount, 1);
	/*
	 * For RO->RW transition, the barrier pairs with the barrier in
	 * mnt_is_readonly() making sure if mnt_is_readonly() sees SB_RDONLY
	 * cleared, it will see s_readonly_remount set.
	 * For RW->RO transition, the barrier pairs with the barrier in
	 * mnt_get_write_access() before the mnt_is_readonly() check.
	 * The barrier makes sure if mnt_get_write_access() sees MNT_WRITE_HOLD
	 * already cleared, it will see s_readonly_remount set.
	 */
	smp_wmb();
}

/*
 * Ends section changing read-only state of the superblock. After this function
 * returns if mnt_is_readonly() returns false, the caller will be able to
 * observe all the changes remount did to the superblock.
 */
static inline void sb_end_ro_state_change(struct super_block *sb)
{
	/*
	 * This barrier provides release semantics that pairs with
	 * the smp_rmb() acquire semantics in mnt_is_readonly().
	 * This barrier pair ensure that when mnt_is_readonly() sees
	 * 0 for sb->s_readonly_remount, it will also see all the
	 * preceding flag changes that were made during the RO state
	 * change.
	 */
	smp_wmb();
	WRITE_ONCE(sb->s_readonly_remount, 0);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * open.c
 */
struct open_flags {
	int open_flag;
	umode_t mode;
	int acc_mode;
	int intent;
<<<<<<< HEAD
};
extern struct file *do_filp_open(int dfd, struct filename *pathname,
		const struct open_flags *op, int flags);
extern struct file *do_file_open_root(struct dentry *, struct vfsmount *,
		const char *, const struct open_flags *, int lookup_flags);

extern long do_handle_open(int mountdirfd,
			   struct file_handle __user *ufh, int open_flag);
extern int open_check_o_direct(struct file *f);
=======
	int lookup_flags;
};
extern struct file *do_filp_open(int dfd, struct filename *pathname,
		const struct open_flags *op);
extern struct file *do_file_open_root(const struct path *,
		const char *, const struct open_flags *);
extern struct open_how build_open_how(int flags, umode_t mode);
extern int build_open_flags(const struct open_how *how, struct open_flags *op);
struct file *file_close_fd_locked(struct files_struct *files, unsigned fd);

long do_ftruncate(struct file *file, loff_t length, int small);
long do_sys_ftruncate(unsigned int fd, loff_t length, int small);
int chmod_common(const struct path *path, umode_t mode);
int do_fchownat(int dfd, const char __user *filename, uid_t user, gid_t group,
		int flag);
int chown_common(const struct path *path, uid_t user, gid_t group);
extern int vfs_open(const struct path *, struct file *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * inode.c
 */
<<<<<<< HEAD
extern spinlock_t inode_sb_list_lock;
=======
extern long prune_icache_sb(struct super_block *sb, struct shrink_control *sc);
int dentry_needs_remove_privs(struct mnt_idmap *, struct dentry *dentry);
bool in_group_or_capable(struct mnt_idmap *idmap,
			 const struct inode *inode, vfsgid_t vfsgid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * fs-writeback.c
 */
<<<<<<< HEAD
extern void inode_wb_list_del(struct inode *inode);

extern int get_nr_dirty_inodes(void);
extern void evict_inodes(struct super_block *);
extern int invalidate_inodes(struct super_block *, bool);
=======
extern long get_nr_dirty_inodes(void);
void invalidate_inodes(struct super_block *sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * dcache.c
 */
<<<<<<< HEAD
extern struct dentry *__d_alloc(struct super_block *, const struct qstr *);
=======
extern int d_set_mounted(struct dentry *dentry);
extern long prune_dcache_sb(struct super_block *sb, struct shrink_control *sc);
extern struct dentry *d_alloc_cursor(struct dentry *);
extern struct dentry * d_alloc_pseudo(struct super_block *, const struct qstr *);
extern char *simple_dname(struct dentry *, char *, int);
extern void dput_to_list(struct dentry *, struct list_head *);
extern void shrink_dentry_list(struct list_head *);
extern void shrink_dcache_for_umount(struct super_block *);
extern struct dentry *__d_lookup(const struct dentry *, const struct qstr *);
extern struct dentry *__d_lookup_rcu(const struct dentry *parent,
				const struct qstr *name, unsigned *seq);
extern void d_genocide(struct dentry *);

/*
 * pipe.c
 */
extern const struct file_operations pipefifo_fops;

/*
 * fs_pin.c
 */
extern void group_pin_kill(struct hlist_head *p);
extern void mnt_pin_kill(struct mount *m);

/*
 * fs/nsfs.c
 */
extern const struct dentry_operations ns_dentry_operations;

/*
 * fs/stat.c:
 */

int getname_statx_lookup_flags(int flags);
int do_statx(int dfd, struct filename *filename, unsigned int flags,
	     unsigned int mask, struct statx __user *buffer);

/*
 * fs/splice.c:
 */
ssize_t splice_file_to_pipe(struct file *in,
			    struct pipe_inode_info *opipe,
			    loff_t *offset,
			    size_t len, unsigned int flags);

/*
 * fs/xattr.c:
 */
struct xattr_name {
	char name[XATTR_NAME_MAX + 1];
};

struct xattr_ctx {
	/* Value of attribute */
	union {
		const void __user *cvalue;
		void __user *value;
	};
	void *kvalue;
	size_t size;
	/* Attribute name */
	struct xattr_name *kname;
	unsigned int flags;
};


ssize_t do_getxattr(struct mnt_idmap *idmap,
		    struct dentry *d,
		    struct xattr_ctx *ctx);

int setxattr_copy(const char __user *name, struct xattr_ctx *ctx);
int do_setxattr(struct mnt_idmap *idmap, struct dentry *dentry,
		struct xattr_ctx *ctx);
int may_write_xattr(struct mnt_idmap *idmap, struct inode *inode);

#ifdef CONFIG_FS_POSIX_ACL
int do_set_acl(struct mnt_idmap *idmap, struct dentry *dentry,
	       const char *acl_name, const void *kvalue, size_t size);
ssize_t do_get_acl(struct mnt_idmap *idmap, struct dentry *dentry,
		   const char *acl_name, void *kvalue, size_t size);
#else
static inline int do_set_acl(struct mnt_idmap *idmap,
			     struct dentry *dentry, const char *acl_name,
			     const void *kvalue, size_t size)
{
	return -EOPNOTSUPP;
}
static inline ssize_t do_get_acl(struct mnt_idmap *idmap,
				 struct dentry *dentry, const char *acl_name,
				 void *kvalue, size_t size)
{
	return -EOPNOTSUPP;
}
#endif

ssize_t __kernel_write_iter(struct file *file, struct iov_iter *from, loff_t *pos);

/*
 * fs/attr.c
 */
struct mnt_idmap *alloc_mnt_idmap(struct user_namespace *mnt_userns);
struct mnt_idmap *mnt_idmap_get(struct mnt_idmap *idmap);
void mnt_idmap_put(struct mnt_idmap *idmap);
struct stashed_operations {
	void (*put_data)(void *data);
	int (*init_inode)(struct inode *inode, void *data);
};
int path_from_stashed(struct dentry **stashed, struct vfsmount *mnt, void *data,
		      struct path *path);
void stashed_dentry_prune(struct dentry *dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
