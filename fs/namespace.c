<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/fs/namespace.c
 *
 * (C) Copyright Al Viro 2000, 2001
<<<<<<< HEAD
 *	Released under GPL v2.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Based on code from fs/super.c, copyright Linus Torvalds and others.
 * Heavily rewritten.
 */

#include <linux/syscalls.h>
#include <linux/export.h>
#include <linux/capability.h>
#include <linux/mnt_namespace.h>
#include <linux/user_namespace.h>
#include <linux/namei.h>
#include <linux/security.h>
<<<<<<< HEAD
#include <linux/idr.h>
#include <linux/acct.h>		/* acct_auto_close_mnt */
#include <linux/ramfs.h>	/* init_rootfs */
#include <linux/fs_struct.h>	/* get_fs_root et.al. */
#include <linux/fsnotify.h>	/* fsnotify_vfsmount_delete */
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include "pnode.h"
#include "internal.h"

#define HASH_SHIFT ilog2(PAGE_SIZE / sizeof(struct list_head))
#define HASH_SIZE (1UL << HASH_SHIFT)

static int event;
static DEFINE_IDA(mnt_id_ida);
static DEFINE_IDA(mnt_group_ida);
static DEFINE_SPINLOCK(mnt_id_lock);
static int mnt_id_start = 0;
static int mnt_group_start = 1;

static struct list_head *mount_hashtable __read_mostly;
static struct kmem_cache *mnt_cache __read_mostly;
static struct rw_semaphore namespace_sem;

/* /sys/fs */
struct kobject *fs_kobj;
=======
#include <linux/cred.h>
#include <linux/idr.h>
#include <linux/init.h>		/* init_rootfs */
#include <linux/fs_struct.h>	/* get_fs_root et.al. */
#include <linux/fsnotify.h>	/* fsnotify_vfsmount_delete */
#include <linux/file.h>
#include <linux/uaccess.h>
#include <linux/proc_ns.h>
#include <linux/magic.h>
#include <linux/memblock.h>
#include <linux/proc_fs.h>
#include <linux/task_work.h>
#include <linux/sched/task.h>
#include <uapi/linux/mount.h>
#include <linux/fs_context.h>
#include <linux/shmem_fs.h>
#include <linux/mnt_idmapping.h>
#include <linux/nospec.h>

#include "pnode.h"
#include "internal.h"

/* Maximum number of mounts in a mount namespace */
static unsigned int sysctl_mount_max __read_mostly = 100000;

static unsigned int m_hash_mask __ro_after_init;
static unsigned int m_hash_shift __ro_after_init;
static unsigned int mp_hash_mask __ro_after_init;
static unsigned int mp_hash_shift __ro_after_init;

static __initdata unsigned long mhash_entries;
static int __init set_mhash_entries(char *str)
{
	if (!str)
		return 0;
	mhash_entries = simple_strtoul(str, &str, 0);
	return 1;
}
__setup("mhash_entries=", set_mhash_entries);

static __initdata unsigned long mphash_entries;
static int __init set_mphash_entries(char *str)
{
	if (!str)
		return 0;
	mphash_entries = simple_strtoul(str, &str, 0);
	return 1;
}
__setup("mphash_entries=", set_mphash_entries);

static u64 event;
static DEFINE_IDA(mnt_id_ida);
static DEFINE_IDA(mnt_group_ida);

/* Don't allow confusion with old 32bit mount ID */
static atomic64_t mnt_id_ctr = ATOMIC64_INIT(1ULL << 32);

static struct hlist_head *mount_hashtable __ro_after_init;
static struct hlist_head *mountpoint_hashtable __ro_after_init;
static struct kmem_cache *mnt_cache __ro_after_init;
static DECLARE_RWSEM(namespace_sem);
static HLIST_HEAD(unmounted);	/* protected by namespace_sem */
static LIST_HEAD(ex_mountpoints); /* protected by namespace_sem */

struct mount_kattr {
	unsigned int attr_set;
	unsigned int attr_clr;
	unsigned int propagation;
	unsigned int lookup_flags;
	bool recurse;
	struct user_namespace *mnt_userns;
	struct mnt_idmap *mnt_idmap;
};

/* /sys/fs */
struct kobject *fs_kobj __ro_after_init;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL_GPL(fs_kobj);

/*
 * vfsmount lock may be taken for read to prevent changes to the
 * vfsmount hash, ie. during mountpoint lookups or walking back
 * up the tree.
 *
 * It should be taken for write in all cases where the vfsmount
 * tree or hash is modified or when a vfsmount structure is modified.
 */
<<<<<<< HEAD
DEFINE_BRLOCK(vfsmount_lock);

static inline unsigned long hash(struct vfsmount *mnt, struct dentry *dentry)
{
	unsigned long tmp = ((unsigned long)mnt / L1_CACHE_BYTES);
	tmp += ((unsigned long)dentry / L1_CACHE_BYTES);
	tmp = tmp + (tmp >> HASH_SHIFT);
	return tmp & (HASH_SIZE - 1);
}

#define MNT_WRITER_UNDERFLOW_LIMIT -(1<<16)

/*
 * allocation is serialized by namespace_sem, but we need the spinlock to
 * serialize with freeing.
 */
static int mnt_alloc_id(struct mount *mnt)
{
	int res;

retry:
	ida_pre_get(&mnt_id_ida, GFP_KERNEL);
	spin_lock(&mnt_id_lock);
	res = ida_get_new_above(&mnt_id_ida, mnt_id_start, &mnt->mnt_id);
	if (!res)
		mnt_id_start = mnt->mnt_id + 1;
	spin_unlock(&mnt_id_lock);
	if (res == -EAGAIN)
		goto retry;

	return res;
=======
__cacheline_aligned_in_smp DEFINE_SEQLOCK(mount_lock);

static inline void lock_mount_hash(void)
{
	write_seqlock(&mount_lock);
}

static inline void unlock_mount_hash(void)
{
	write_sequnlock(&mount_lock);
}

static inline struct hlist_head *m_hash(struct vfsmount *mnt, struct dentry *dentry)
{
	unsigned long tmp = ((unsigned long)mnt / L1_CACHE_BYTES);
	tmp += ((unsigned long)dentry / L1_CACHE_BYTES);
	tmp = tmp + (tmp >> m_hash_shift);
	return &mount_hashtable[tmp & m_hash_mask];
}

static inline struct hlist_head *mp_hash(struct dentry *dentry)
{
	unsigned long tmp = ((unsigned long)dentry / L1_CACHE_BYTES);
	tmp = tmp + (tmp >> mp_hash_shift);
	return &mountpoint_hashtable[tmp & mp_hash_mask];
}

static int mnt_alloc_id(struct mount *mnt)
{
	int res = ida_alloc(&mnt_id_ida, GFP_KERNEL);

	if (res < 0)
		return res;
	mnt->mnt_id = res;
	mnt->mnt_id_unique = atomic64_inc_return(&mnt_id_ctr);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void mnt_free_id(struct mount *mnt)
{
<<<<<<< HEAD
	int id = mnt->mnt_id;
	spin_lock(&mnt_id_lock);
	ida_remove(&mnt_id_ida, id);
	if (mnt_id_start > id)
		mnt_id_start = id;
	spin_unlock(&mnt_id_lock);
=======
	ida_free(&mnt_id_ida, mnt->mnt_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Allocate a new peer group ID
<<<<<<< HEAD
 *
 * mnt_group_ida is protected by namespace_sem
 */
static int mnt_alloc_group_id(struct mount *mnt)
{
	int res;

	if (!ida_pre_get(&mnt_group_ida, GFP_KERNEL))
		return -ENOMEM;

	res = ida_get_new_above(&mnt_group_ida,
				mnt_group_start,
				&mnt->mnt_group_id);
	if (!res)
		mnt_group_start = mnt->mnt_group_id + 1;

	return res;
=======
 */
static int mnt_alloc_group_id(struct mount *mnt)
{
	int res = ida_alloc_min(&mnt_group_ida, 1, GFP_KERNEL);

	if (res < 0)
		return res;
	mnt->mnt_group_id = res;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Release a peer group ID
 */
void mnt_release_group_id(struct mount *mnt)
{
<<<<<<< HEAD
	int id = mnt->mnt_group_id;
	ida_remove(&mnt_group_ida, id);
	if (mnt_group_start > id)
		mnt_group_start = id;
=======
	ida_free(&mnt_group_ida, mnt->mnt_group_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mnt->mnt_group_id = 0;
}

/*
 * vfsmount lock must be held for read
 */
static inline void mnt_add_count(struct mount *mnt, int n)
{
#ifdef CONFIG_SMP
	this_cpu_add(mnt->mnt_pcp->mnt_count, n);
#else
	preempt_disable();
	mnt->mnt_count += n;
	preempt_enable();
#endif
}

/*
 * vfsmount lock must be held for write
 */
<<<<<<< HEAD
unsigned int mnt_get_count(struct mount *mnt)
{
#ifdef CONFIG_SMP
	unsigned int count = 0;
=======
int mnt_get_count(struct mount *mnt)
{
#ifdef CONFIG_SMP
	int count = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int cpu;

	for_each_possible_cpu(cpu) {
		count += per_cpu_ptr(mnt->mnt_pcp, cpu)->mnt_count;
	}

	return count;
#else
	return mnt->mnt_count;
#endif
}

static struct mount *alloc_vfsmnt(const char *name)
{
	struct mount *mnt = kmem_cache_zalloc(mnt_cache, GFP_KERNEL);
	if (mnt) {
		int err;

		err = mnt_alloc_id(mnt);
		if (err)
			goto out_free_cache;

		if (name) {
<<<<<<< HEAD
			mnt->mnt_devname = kstrdup(name, GFP_KERNEL);
=======
			mnt->mnt_devname = kstrdup_const(name,
							 GFP_KERNEL_ACCOUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (!mnt->mnt_devname)
				goto out_free_id;
		}

#ifdef CONFIG_SMP
		mnt->mnt_pcp = alloc_percpu(struct mnt_pcp);
		if (!mnt->mnt_pcp)
			goto out_free_devname;

		this_cpu_add(mnt->mnt_pcp->mnt_count, 1);
#else
		mnt->mnt_count = 1;
		mnt->mnt_writers = 0;
#endif

<<<<<<< HEAD
		INIT_LIST_HEAD(&mnt->mnt_hash);
=======
		INIT_HLIST_NODE(&mnt->mnt_hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		INIT_LIST_HEAD(&mnt->mnt_child);
		INIT_LIST_HEAD(&mnt->mnt_mounts);
		INIT_LIST_HEAD(&mnt->mnt_list);
		INIT_LIST_HEAD(&mnt->mnt_expire);
		INIT_LIST_HEAD(&mnt->mnt_share);
		INIT_LIST_HEAD(&mnt->mnt_slave_list);
		INIT_LIST_HEAD(&mnt->mnt_slave);
<<<<<<< HEAD
#ifdef CONFIG_FSNOTIFY
		INIT_HLIST_HEAD(&mnt->mnt_fsnotify_marks);
#endif
=======
		INIT_HLIST_NODE(&mnt->mnt_mp_list);
		INIT_LIST_HEAD(&mnt->mnt_umounting);
		INIT_HLIST_HEAD(&mnt->mnt_stuck_children);
		mnt->mnt.mnt_idmap = &nop_mnt_idmap;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return mnt;

#ifdef CONFIG_SMP
out_free_devname:
<<<<<<< HEAD
	kfree(mnt->mnt_devname);
=======
	kfree_const(mnt->mnt_devname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
out_free_id:
	mnt_free_id(mnt);
out_free_cache:
	kmem_cache_free(mnt_cache, mnt);
	return NULL;
}

/*
 * Most r/o checks on a fs are for operations that take
 * discrete amounts of time, like a write() or unlink().
 * We must keep track of when those operations start
 * (for permission checks) and when they end, so that
 * we can determine when writes are able to occur to
 * a filesystem.
 */
/*
 * __mnt_is_readonly: check whether a mount is read-only
 * @mnt: the mount to check for its write status
 *
 * This shouldn't be used directly ouside of the VFS.
 * It does not guarantee that the filesystem will stay
 * r/w, just that it is right *now*.  This can not and
 * should not be used in place of IS_RDONLY(inode).
 * mnt_want/drop_write() will _keep_ the filesystem
 * r/w.
 */
<<<<<<< HEAD
int __mnt_is_readonly(struct vfsmount *mnt)
{
	if (mnt->mnt_flags & MNT_READONLY)
		return 1;
	if (mnt->mnt_sb->s_flags & MS_RDONLY)
		return 1;
	return 0;
=======
bool __mnt_is_readonly(struct vfsmount *mnt)
{
	return (mnt->mnt_flags & MNT_READONLY) || sb_rdonly(mnt->mnt_sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(__mnt_is_readonly);

static inline void mnt_inc_writers(struct mount *mnt)
{
#ifdef CONFIG_SMP
	this_cpu_inc(mnt->mnt_pcp->mnt_writers);
#else
	mnt->mnt_writers++;
#endif
}

static inline void mnt_dec_writers(struct mount *mnt)
{
#ifdef CONFIG_SMP
	this_cpu_dec(mnt->mnt_pcp->mnt_writers);
#else
	mnt->mnt_writers--;
#endif
}

static unsigned int mnt_get_writers(struct mount *mnt)
{
#ifdef CONFIG_SMP
	unsigned int count = 0;
	int cpu;

	for_each_possible_cpu(cpu) {
		count += per_cpu_ptr(mnt->mnt_pcp, cpu)->mnt_writers;
	}

	return count;
#else
	return mnt->mnt_writers;
#endif
}

static int mnt_is_readonly(struct vfsmount *mnt)
{
<<<<<<< HEAD
	if (mnt->mnt_sb->s_readonly_remount)
		return 1;
	/* Order wrt setting s_flags/s_readonly_remount in do_remount() */
=======
	if (READ_ONCE(mnt->mnt_sb->s_readonly_remount))
		return 1;
	/*
	 * The barrier pairs with the barrier in sb_start_ro_state_change()
	 * making sure if we don't see s_readonly_remount set yet, we also will
	 * not see any superblock / mount flag changes done by remount.
	 * It also pairs with the barrier in sb_end_ro_state_change()
	 * assuring that if we see s_readonly_remount already cleared, we will
	 * see the values of superblock / mount flags updated by remount.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	smp_rmb();
	return __mnt_is_readonly(mnt);
}

/*
<<<<<<< HEAD
 * Most r/o checks on a fs are for operations that take
 * discrete amounts of time, like a write() or unlink().
 * We must keep track of when those operations start
 * (for permission checks) and when they end, so that
 * we can determine when writes are able to occur to
 * a filesystem.
 */
/**
 * mnt_want_write - get write access to a mount
 * @m: the mount on which to take a write
 *
 * This tells the low-level filesystem that a write is
 * about to be performed to it, and makes sure that
 * writes are allowed before returning success.  When
 * the write operation is finished, mnt_drop_write()
 * must be called.  This is effectively a refcount.
 */
int mnt_want_write(struct vfsmount *m)
=======
 * Most r/o & frozen checks on a fs are for operations that take discrete
 * amounts of time, like a write() or unlink().  We must keep track of when
 * those operations start (for permission checks) and when they end, so that we
 * can determine when writes are able to occur to a filesystem.
 */
/**
 * mnt_get_write_access - get write access to a mount without freeze protection
 * @m: the mount on which to take a write
 *
 * This tells the low-level filesystem that a write is about to be performed to
 * it, and makes sure that writes are allowed (mnt it read-write) before
 * returning success. This operation does not protect against filesystem being
 * frozen. When the write operation is finished, mnt_put_write_access() must be
 * called. This is effectively a refcount.
 */
int mnt_get_write_access(struct vfsmount *m)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mount *mnt = real_mount(m);
	int ret = 0;

	preempt_disable();
	mnt_inc_writers(mnt);
	/*
	 * The store to mnt_inc_writers must be visible before we pass
	 * MNT_WRITE_HOLD loop below, so that the slowpath can see our
	 * incremented count after it has set MNT_WRITE_HOLD.
	 */
	smp_mb();
<<<<<<< HEAD
	while (mnt->mnt.mnt_flags & MNT_WRITE_HOLD)
		cpu_relax();
	/*
	 * After the slowpath clears MNT_WRITE_HOLD, mnt_is_readonly will
	 * be set to match its requirements. So we must not load that until
	 * MNT_WRITE_HOLD is cleared.
=======
	might_lock(&mount_lock.lock);
	while (READ_ONCE(mnt->mnt.mnt_flags) & MNT_WRITE_HOLD) {
		if (!IS_ENABLED(CONFIG_PREEMPT_RT)) {
			cpu_relax();
		} else {
			/*
			 * This prevents priority inversion, if the task
			 * setting MNT_WRITE_HOLD got preempted on a remote
			 * CPU, and it prevents life lock if the task setting
			 * MNT_WRITE_HOLD has a lower priority and is bound to
			 * the same CPU as the task that is spinning here.
			 */
			preempt_enable();
			lock_mount_hash();
			unlock_mount_hash();
			preempt_disable();
		}
	}
	/*
	 * The barrier pairs with the barrier sb_start_ro_state_change() making
	 * sure that if we see MNT_WRITE_HOLD cleared, we will also see
	 * s_readonly_remount set (or even SB_RDONLY / MNT_READONLY flags) in
	 * mnt_is_readonly() and bail in case we are racing with remount
	 * read-only.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	smp_rmb();
	if (mnt_is_readonly(m)) {
		mnt_dec_writers(mnt);
		ret = -EROFS;
	}
	preempt_enable();
<<<<<<< HEAD
=======

	return ret;
}
EXPORT_SYMBOL_GPL(mnt_get_write_access);

/**
 * mnt_want_write - get write access to a mount
 * @m: the mount on which to take a write
 *
 * This tells the low-level filesystem that a write is about to be performed to
 * it, and makes sure that writes are allowed (mount is read-write, filesystem
 * is not frozen) before returning success.  When the write operation is
 * finished, mnt_drop_write() must be called.  This is effectively a refcount.
 */
int mnt_want_write(struct vfsmount *m)
{
	int ret;

	sb_start_write(m->mnt_sb);
	ret = mnt_get_write_access(m);
	if (ret)
		sb_end_write(m->mnt_sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL_GPL(mnt_want_write);

/**
<<<<<<< HEAD
 * mnt_clone_write - get write access to a mount
 * @mnt: the mount on which to take a write
 *
 * This is effectively like mnt_want_write, except
 * it must only be used to take an extra write reference
 * on a mountpoint that we already know has a write reference
 * on it. This allows some optimisation.
 *
 * After finished, mnt_drop_write must be called as usual to
 * drop the reference.
 */
int mnt_clone_write(struct vfsmount *mnt)
{
	/* superblock may be r/o */
	if (__mnt_is_readonly(mnt))
		return -EROFS;
	preempt_disable();
	mnt_inc_writers(real_mount(mnt));
	preempt_enable();
	return 0;
}
EXPORT_SYMBOL_GPL(mnt_clone_write);
=======
 * mnt_get_write_access_file - get write access to a file's mount
 * @file: the file who's mount on which to take a write
 *
 * This is like mnt_get_write_access, but if @file is already open for write it
 * skips incrementing mnt_writers (since the open file already has a reference)
 * and instead only does the check for emergency r/o remounts.  This must be
 * paired with mnt_put_write_access_file.
 */
int mnt_get_write_access_file(struct file *file)
{
	if (file->f_mode & FMODE_WRITER) {
		/*
		 * Superblock may have become readonly while there are still
		 * writable fd's, e.g. due to a fs error with errors=remount-ro
		 */
		if (__mnt_is_readonly(file->f_path.mnt))
			return -EROFS;
		return 0;
	}
	return mnt_get_write_access(file->f_path.mnt);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * mnt_want_write_file - get write access to a file's mount
 * @file: the file who's mount on which to take a write
 *
<<<<<<< HEAD
 * This is like mnt_want_write, but it takes a file and can
 * do some optimisations if the file is open for write already
 */
int mnt_want_write_file(struct file *file)
{
	struct inode *inode = file->f_dentry->d_inode;
	if (!(file->f_mode & FMODE_WRITE) || special_file(inode->i_mode))
		return mnt_want_write(file->f_path.mnt);
	else
		return mnt_clone_write(file->f_path.mnt);
=======
 * This is like mnt_want_write, but if the file is already open for writing it
 * skips incrementing mnt_writers (since the open file already has a reference)
 * and instead only does the freeze protection and the check for emergency r/o
 * remounts.  This must be paired with mnt_drop_write_file.
 */
int mnt_want_write_file(struct file *file)
{
	int ret;

	sb_start_write(file_inode(file)->i_sb);
	ret = mnt_get_write_access_file(file);
	if (ret)
		sb_end_write(file_inode(file)->i_sb);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(mnt_want_write_file);

/**
<<<<<<< HEAD
 * mnt_drop_write - give up write access to a mount
=======
 * mnt_put_write_access - give up write access to a mount
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @mnt: the mount on which to give up write access
 *
 * Tells the low-level filesystem that we are done
 * performing writes to it.  Must be matched with
<<<<<<< HEAD
 * mnt_want_write() call above.
 */
void mnt_drop_write(struct vfsmount *mnt)
=======
 * mnt_get_write_access() call above.
 */
void mnt_put_write_access(struct vfsmount *mnt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	preempt_disable();
	mnt_dec_writers(real_mount(mnt));
	preempt_enable();
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(mnt_drop_write);

void mnt_drop_write_file(struct file *file)
{
	mnt_drop_write(file->f_path.mnt);
}
EXPORT_SYMBOL(mnt_drop_write_file);

static int mnt_make_readonly(struct mount *mnt)
{
	int ret = 0;

	br_write_lock(&vfsmount_lock);
=======
EXPORT_SYMBOL_GPL(mnt_put_write_access);

/**
 * mnt_drop_write - give up write access to a mount
 * @mnt: the mount on which to give up write access
 *
 * Tells the low-level filesystem that we are done performing writes to it and
 * also allows filesystem to be frozen again.  Must be matched with
 * mnt_want_write() call above.
 */
void mnt_drop_write(struct vfsmount *mnt)
{
	mnt_put_write_access(mnt);
	sb_end_write(mnt->mnt_sb);
}
EXPORT_SYMBOL_GPL(mnt_drop_write);

void mnt_put_write_access_file(struct file *file)
{
	if (!(file->f_mode & FMODE_WRITER))
		mnt_put_write_access(file->f_path.mnt);
}

void mnt_drop_write_file(struct file *file)
{
	mnt_put_write_access_file(file);
	sb_end_write(file_inode(file)->i_sb);
}
EXPORT_SYMBOL(mnt_drop_write_file);

/**
 * mnt_hold_writers - prevent write access to the given mount
 * @mnt: mnt to prevent write access to
 *
 * Prevents write access to @mnt if there are no active writers for @mnt.
 * This function needs to be called and return successfully before changing
 * properties of @mnt that need to remain stable for callers with write access
 * to @mnt.
 *
 * After this functions has been called successfully callers must pair it with
 * a call to mnt_unhold_writers() in order to stop preventing write access to
 * @mnt.
 *
 * Context: This function expects lock_mount_hash() to be held serializing
 *          setting MNT_WRITE_HOLD.
 * Return: On success 0 is returned.
 *	   On error, -EBUSY is returned.
 */
static inline int mnt_hold_writers(struct mount *mnt)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mnt->mnt.mnt_flags |= MNT_WRITE_HOLD;
	/*
	 * After storing MNT_WRITE_HOLD, we'll read the counters. This store
	 * should be visible before we do.
	 */
	smp_mb();

	/*
	 * With writers on hold, if this value is zero, then there are
	 * definitely no active writers (although held writers may subsequently
	 * increment the count, they'll have to wait, and decrement it after
	 * seeing MNT_READONLY).
	 *
	 * It is OK to have counter incremented on one CPU and decremented on
	 * another: the sum will add up correctly. The danger would be when we
	 * sum up each counter, if we read a counter before it is incremented,
	 * but then read another CPU's count which it has been subsequently
	 * decremented from -- we would see more decrements than we should.
	 * MNT_WRITE_HOLD protects against this scenario, because
	 * mnt_want_write first increments count, then smp_mb, then spins on
	 * MNT_WRITE_HOLD, so it can't be decremented by another CPU while
	 * we're counting up here.
	 */
	if (mnt_get_writers(mnt) > 0)
<<<<<<< HEAD
		ret = -EBUSY;
	else
		mnt->mnt.mnt_flags |= MNT_READONLY;
=======
		return -EBUSY;

	return 0;
}

/**
 * mnt_unhold_writers - stop preventing write access to the given mount
 * @mnt: mnt to stop preventing write access to
 *
 * Stop preventing write access to @mnt allowing callers to gain write access
 * to @mnt again.
 *
 * This function can only be called after a successful call to
 * mnt_hold_writers().
 *
 * Context: This function expects lock_mount_hash() to be held.
 */
static inline void mnt_unhold_writers(struct mount *mnt)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * MNT_READONLY must become visible before ~MNT_WRITE_HOLD, so writers
	 * that become unheld will see MNT_READONLY.
	 */
	smp_wmb();
	mnt->mnt.mnt_flags &= ~MNT_WRITE_HOLD;
<<<<<<< HEAD
	br_write_unlock(&vfsmount_lock);
	return ret;
}

static void __mnt_unmake_readonly(struct mount *mnt)
{
	br_write_lock(&vfsmount_lock);
	mnt->mnt.mnt_flags &= ~MNT_READONLY;
	br_write_unlock(&vfsmount_lock);
=======
}

static int mnt_make_readonly(struct mount *mnt)
{
	int ret;

	ret = mnt_hold_writers(mnt);
	if (!ret)
		mnt->mnt.mnt_flags |= MNT_READONLY;
	mnt_unhold_writers(mnt);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int sb_prepare_remount_readonly(struct super_block *sb)
{
	struct mount *mnt;
	int err = 0;

	/* Racy optimization.  Recheck the counter under MNT_WRITE_HOLD */
	if (atomic_long_read(&sb->s_remove_count))
		return -EBUSY;

<<<<<<< HEAD
	br_write_lock(&vfsmount_lock);
	list_for_each_entry(mnt, &sb->s_mounts, mnt_instance) {
		if (!(mnt->mnt.mnt_flags & MNT_READONLY)) {
			mnt->mnt.mnt_flags |= MNT_WRITE_HOLD;
			smp_mb();
			if (mnt_get_writers(mnt) > 0) {
				err = -EBUSY;
				break;
			}
=======
	lock_mount_hash();
	list_for_each_entry(mnt, &sb->s_mounts, mnt_instance) {
		if (!(mnt->mnt.mnt_flags & MNT_READONLY)) {
			err = mnt_hold_writers(mnt);
			if (err)
				break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	if (!err && atomic_long_read(&sb->s_remove_count))
		err = -EBUSY;

<<<<<<< HEAD
	if (!err) {
		sb->s_readonly_remount = 1;
		smp_wmb();
	}
=======
	if (!err)
		sb_start_ro_state_change(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry(mnt, &sb->s_mounts, mnt_instance) {
		if (mnt->mnt.mnt_flags & MNT_WRITE_HOLD)
			mnt->mnt.mnt_flags &= ~MNT_WRITE_HOLD;
	}
<<<<<<< HEAD
	br_write_unlock(&vfsmount_lock);
=======
	unlock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

static void free_vfsmnt(struct mount *mnt)
{
<<<<<<< HEAD
	kfree(mnt->mnt.data);
	kfree(mnt->mnt_devname);
	mnt_free_id(mnt);
=======
	mnt_idmap_put(mnt_idmap(&mnt->mnt));
	kfree_const(mnt->mnt_devname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SMP
	free_percpu(mnt->mnt_pcp);
#endif
	kmem_cache_free(mnt_cache, mnt);
}

<<<<<<< HEAD
/*
 * find the first or last mount at @dentry on vfsmount @mnt depending on
 * @dir. If @dir is set return the first mount else return the last mount.
 * vfsmount_lock must be held for read or write.
 */
struct mount *__lookup_mnt(struct vfsmount *mnt, struct dentry *dentry,
			      int dir)
{
	struct list_head *head = mount_hashtable + hash(mnt, dentry);
	struct list_head *tmp = head;
	struct mount *p, *found = NULL;

	for (;;) {
		tmp = dir ? tmp->next : tmp->prev;
		p = NULL;
		if (tmp == head)
			break;
		p = list_entry(tmp, struct mount, mnt_hash);
		if (&p->mnt_parent->mnt == mnt && p->mnt_mountpoint == dentry) {
			found = p;
			break;
		}
	}
	return found;
=======
static void delayed_free_vfsmnt(struct rcu_head *head)
{
	free_vfsmnt(container_of(head, struct mount, mnt_rcu));
}

/* call under rcu_read_lock */
int __legitimize_mnt(struct vfsmount *bastard, unsigned seq)
{
	struct mount *mnt;
	if (read_seqretry(&mount_lock, seq))
		return 1;
	if (bastard == NULL)
		return 0;
	mnt = real_mount(bastard);
	mnt_add_count(mnt, 1);
	smp_mb();			// see mntput_no_expire()
	if (likely(!read_seqretry(&mount_lock, seq)))
		return 0;
	if (bastard->mnt_flags & MNT_SYNC_UMOUNT) {
		mnt_add_count(mnt, -1);
		return 1;
	}
	lock_mount_hash();
	if (unlikely(bastard->mnt_flags & MNT_DOOMED)) {
		mnt_add_count(mnt, -1);
		unlock_mount_hash();
		return 1;
	}
	unlock_mount_hash();
	/* caller will mntput() */
	return -1;
}

/* call under rcu_read_lock */
static bool legitimize_mnt(struct vfsmount *bastard, unsigned seq)
{
	int res = __legitimize_mnt(bastard, seq);
	if (likely(!res))
		return true;
	if (unlikely(res < 0)) {
		rcu_read_unlock();
		mntput(bastard);
		rcu_read_lock();
	}
	return false;
}

/**
 * __lookup_mnt - find first child mount
 * @mnt:	parent mount
 * @dentry:	mountpoint
 *
 * If @mnt has a child mount @c mounted @dentry find and return it.
 *
 * Note that the child mount @c need not be unique. There are cases
 * where shadow mounts are created. For example, during mount
 * propagation when a source mount @mnt whose root got overmounted by a
 * mount @o after path lookup but before @namespace_sem could be
 * acquired gets copied and propagated. So @mnt gets copied including
 * @o. When @mnt is propagated to a destination mount @d that already
 * has another mount @n mounted at the same mountpoint then the source
 * mount @mnt will be tucked beneath @n, i.e., @n will be mounted on
 * @mnt and @mnt mounted on @d. Now both @n and @o are mounted at @mnt
 * on @dentry.
 *
 * Return: The first child of @mnt mounted @dentry or NULL.
 */
struct mount *__lookup_mnt(struct vfsmount *mnt, struct dentry *dentry)
{
	struct hlist_head *head = m_hash(mnt, dentry);
	struct mount *p;

	hlist_for_each_entry_rcu(p, head, mnt_hash)
		if (&p->mnt_parent->mnt == mnt && p->mnt_mountpoint == dentry)
			return p;
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * lookup_mnt - Return the first child mount mounted at path
 *
 * "First" means first mounted chronologically.  If you create the
 * following mounts:
 *
 * mount /dev/sda1 /mnt
 * mount /dev/sda2 /mnt
 * mount /dev/sda3 /mnt
 *
 * Then lookup_mnt() on the base /mnt dentry in the root mount will
 * return successively the root dentry and vfsmount of /dev/sda1, then
 * /dev/sda2, then /dev/sda3, then NULL.
 *
 * lookup_mnt takes a reference to the found vfsmount.
 */
<<<<<<< HEAD
struct vfsmount *lookup_mnt(struct path *path)
{
	struct mount *child_mnt;

	br_read_lock(&vfsmount_lock);
	child_mnt = __lookup_mnt(path->mnt, path->dentry, 1);
	if (child_mnt) {
		mnt_add_count(child_mnt, 1);
		br_read_unlock(&vfsmount_lock);
		return &child_mnt->mnt;
	} else {
		br_read_unlock(&vfsmount_lock);
		return NULL;
	}
=======
struct vfsmount *lookup_mnt(const struct path *path)
{
	struct mount *child_mnt;
	struct vfsmount *m;
	unsigned seq;

	rcu_read_lock();
	do {
		seq = read_seqbegin(&mount_lock);
		child_mnt = __lookup_mnt(path->mnt, path->dentry);
		m = child_mnt ? &child_mnt->mnt : NULL;
	} while (!legitimize_mnt(m, seq));
	rcu_read_unlock();
	return m;
}

/*
 * __is_local_mountpoint - Test to see if dentry is a mountpoint in the
 *                         current mount namespace.
 *
 * The common case is dentries are not mountpoints at all and that
 * test is handled inline.  For the slow case when we are actually
 * dealing with a mountpoint of some kind, walk through all of the
 * mounts in the current mount namespace and test to see if the dentry
 * is a mountpoint.
 *
 * The mount_hashtable is not usable in the context because we
 * need to identify all mounts that may be in the current mount
 * namespace not just a mount that happens to have some specified
 * parent mount.
 */
bool __is_local_mountpoint(struct dentry *dentry)
{
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	struct mount *mnt, *n;
	bool is_covered = false;

	down_read(&namespace_sem);
	rbtree_postorder_for_each_entry_safe(mnt, n, &ns->mounts, mnt_node) {
		is_covered = (mnt->mnt_mountpoint == dentry);
		if (is_covered)
			break;
	}
	up_read(&namespace_sem);

	return is_covered;
}

static struct mountpoint *lookup_mountpoint(struct dentry *dentry)
{
	struct hlist_head *chain = mp_hash(dentry);
	struct mountpoint *mp;

	hlist_for_each_entry(mp, chain, m_hash) {
		if (mp->m_dentry == dentry) {
			mp->m_count++;
			return mp;
		}
	}
	return NULL;
}

static struct mountpoint *get_mountpoint(struct dentry *dentry)
{
	struct mountpoint *mp, *new = NULL;
	int ret;

	if (d_mountpoint(dentry)) {
		/* might be worth a WARN_ON() */
		if (d_unlinked(dentry))
			return ERR_PTR(-ENOENT);
mountpoint:
		read_seqlock_excl(&mount_lock);
		mp = lookup_mountpoint(dentry);
		read_sequnlock_excl(&mount_lock);
		if (mp)
			goto done;
	}

	if (!new)
		new = kmalloc(sizeof(struct mountpoint), GFP_KERNEL);
	if (!new)
		return ERR_PTR(-ENOMEM);


	/* Exactly one processes may set d_mounted */
	ret = d_set_mounted(dentry);

	/* Someone else set d_mounted? */
	if (ret == -EBUSY)
		goto mountpoint;

	/* The dentry is not available as a mountpoint? */
	mp = ERR_PTR(ret);
	if (ret)
		goto done;

	/* Add the new mountpoint to the hash table */
	read_seqlock_excl(&mount_lock);
	new->m_dentry = dget(dentry);
	new->m_count = 1;
	hlist_add_head(&new->m_hash, mp_hash(dentry));
	INIT_HLIST_HEAD(&new->m_list);
	read_sequnlock_excl(&mount_lock);

	mp = new;
	new = NULL;
done:
	kfree(new);
	return mp;
}

/*
 * vfsmount lock must be held.  Additionally, the caller is responsible
 * for serializing calls for given disposal list.
 */
static void __put_mountpoint(struct mountpoint *mp, struct list_head *list)
{
	if (!--mp->m_count) {
		struct dentry *dentry = mp->m_dentry;
		BUG_ON(!hlist_empty(&mp->m_list));
		spin_lock(&dentry->d_lock);
		dentry->d_flags &= ~DCACHE_MOUNTED;
		spin_unlock(&dentry->d_lock);
		dput_to_list(dentry, list);
		hlist_del(&mp->m_hash);
		kfree(mp);
	}
}

/* called with namespace_lock and vfsmount lock */
static void put_mountpoint(struct mountpoint *mp)
{
	__put_mountpoint(mp, &ex_mountpoints);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int check_mnt(struct mount *mnt)
{
	return mnt->mnt_ns == current->nsproxy->mnt_ns;
}

/*
 * vfsmount lock must be held for write
 */
static void touch_mnt_namespace(struct mnt_namespace *ns)
{
	if (ns) {
		ns->event = ++event;
		wake_up_interruptible(&ns->poll);
	}
}

/*
 * vfsmount lock must be held for write
 */
static void __touch_mnt_namespace(struct mnt_namespace *ns)
{
	if (ns && ns->event != event) {
		ns->event = event;
		wake_up_interruptible(&ns->poll);
	}
}

/*
<<<<<<< HEAD
 * Clear dentry's mounted state if it has no remaining mounts.
 * vfsmount_lock must be held for write.
 */
static void dentry_reset_mounted(struct dentry *dentry)
{
	unsigned u;

	for (u = 0; u < HASH_SIZE; u++) {
		struct mount *p;

		list_for_each_entry(p, &mount_hashtable[u], mnt_hash) {
			if (p->mnt_mountpoint == dentry)
				return;
		}
	}
	spin_lock(&dentry->d_lock);
	dentry->d_flags &= ~DCACHE_MOUNTED;
	spin_unlock(&dentry->d_lock);
}

/*
 * vfsmount lock must be held for write
 */
static void detach_mnt(struct mount *mnt, struct path *old_path)
{
	old_path->dentry = mnt->mnt_mountpoint;
	old_path->mnt = &mnt->mnt_parent->mnt;
	mnt->mnt_parent = mnt;
	mnt->mnt_mountpoint = mnt->mnt.mnt_root;
	list_del_init(&mnt->mnt_child);
	list_del_init(&mnt->mnt_hash);
	dentry_reset_mounted(old_path->dentry);
=======
 * vfsmount lock must be held for write
 */
static struct mountpoint *unhash_mnt(struct mount *mnt)
{
	struct mountpoint *mp;
	mnt->mnt_parent = mnt;
	mnt->mnt_mountpoint = mnt->mnt.mnt_root;
	list_del_init(&mnt->mnt_child);
	hlist_del_init_rcu(&mnt->mnt_hash);
	hlist_del_init(&mnt->mnt_mp_list);
	mp = mnt->mnt_mp;
	mnt->mnt_mp = NULL;
	return mp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * vfsmount lock must be held for write
 */
<<<<<<< HEAD
void mnt_set_mountpoint(struct mount *mnt, struct dentry *dentry,
			struct mount *child_mnt)
{
	mnt_add_count(mnt, 1);	/* essentially, that's mntget */
	child_mnt->mnt_mountpoint = dget(dentry);
	child_mnt->mnt_parent = mnt;
	spin_lock(&dentry->d_lock);
	dentry->d_flags |= DCACHE_MOUNTED;
	spin_unlock(&dentry->d_lock);
}

/*
 * vfsmount lock must be held for write
 */
static void attach_mnt(struct mount *mnt, struct path *path)
{
	mnt_set_mountpoint(real_mount(path->mnt), path->dentry, mnt);
	list_add_tail(&mnt->mnt_hash, mount_hashtable +
			hash(path->mnt, path->dentry));
	list_add_tail(&mnt->mnt_child, &real_mount(path->mnt)->mnt_mounts);
=======
static void umount_mnt(struct mount *mnt)
{
	put_mountpoint(unhash_mnt(mnt));
}

/*
 * vfsmount lock must be held for write
 */
void mnt_set_mountpoint(struct mount *mnt,
			struct mountpoint *mp,
			struct mount *child_mnt)
{
	mp->m_count++;
	mnt_add_count(mnt, 1);	/* essentially, that's mntget */
	child_mnt->mnt_mountpoint = mp->m_dentry;
	child_mnt->mnt_parent = mnt;
	child_mnt->mnt_mp = mp;
	hlist_add_head(&child_mnt->mnt_mp_list, &mp->m_list);
}

/**
 * mnt_set_mountpoint_beneath - mount a mount beneath another one
 *
 * @new_parent: the source mount
 * @top_mnt:    the mount beneath which @new_parent is mounted
 * @new_mp:     the new mountpoint of @top_mnt on @new_parent
 *
 * Remove @top_mnt from its current mountpoint @top_mnt->mnt_mp and
 * parent @top_mnt->mnt_parent and mount it on top of @new_parent at
 * @new_mp. And mount @new_parent on the old parent and old
 * mountpoint of @top_mnt.
 *
 * Context: This function expects namespace_lock() and lock_mount_hash()
 *          to have been acquired in that order.
 */
static void mnt_set_mountpoint_beneath(struct mount *new_parent,
				       struct mount *top_mnt,
				       struct mountpoint *new_mp)
{
	struct mount *old_top_parent = top_mnt->mnt_parent;
	struct mountpoint *old_top_mp = top_mnt->mnt_mp;

	mnt_set_mountpoint(old_top_parent, old_top_mp, new_parent);
	mnt_change_mountpoint(new_parent, new_mp, top_mnt);
}


static void __attach_mnt(struct mount *mnt, struct mount *parent)
{
	hlist_add_head_rcu(&mnt->mnt_hash,
			   m_hash(&parent->mnt, mnt->mnt_mountpoint));
	list_add_tail(&mnt->mnt_child, &parent->mnt_mounts);
}

/**
 * attach_mnt - mount a mount, attach to @mount_hashtable and parent's
 *              list of child mounts
 * @parent:  the parent
 * @mnt:     the new mount
 * @mp:      the new mountpoint
 * @beneath: whether to mount @mnt beneath or on top of @parent
 *
 * If @beneath is false, mount @mnt at @mp on @parent. Then attach @mnt
 * to @parent's child mount list and to @mount_hashtable.
 *
 * If @beneath is true, remove @mnt from its current parent and
 * mountpoint and mount it on @mp on @parent, and mount @parent on the
 * old parent and old mountpoint of @mnt. Finally, attach @parent to
 * @mnt_hashtable and @parent->mnt_parent->mnt_mounts.
 *
 * Note, when __attach_mnt() is called @mnt->mnt_parent already points
 * to the correct parent.
 *
 * Context: This function expects namespace_lock() and lock_mount_hash()
 *          to have been acquired in that order.
 */
static void attach_mnt(struct mount *mnt, struct mount *parent,
		       struct mountpoint *mp, bool beneath)
{
	if (beneath)
		mnt_set_mountpoint_beneath(mnt, parent, mp);
	else
		mnt_set_mountpoint(parent, mp, mnt);
	/*
	 * Note, @mnt->mnt_parent has to be used. If @mnt was mounted
	 * beneath @parent then @mnt will need to be attached to
	 * @parent's old parent, not @parent. IOW, @mnt->mnt_parent
	 * isn't the same mount as @parent.
	 */
	__attach_mnt(mnt, mnt->mnt_parent);
}

void mnt_change_mountpoint(struct mount *parent, struct mountpoint *mp, struct mount *mnt)
{
	struct mountpoint *old_mp = mnt->mnt_mp;
	struct mount *old_parent = mnt->mnt_parent;

	list_del_init(&mnt->mnt_child);
	hlist_del_init(&mnt->mnt_mp_list);
	hlist_del_init_rcu(&mnt->mnt_hash);

	attach_mnt(mnt, parent, mp, false);

	put_mountpoint(old_mp);
	mnt_add_count(old_parent, -1);
}

static inline struct mount *node_to_mount(struct rb_node *node)
{
	return node ? rb_entry(node, struct mount, mnt_node) : NULL;
}

static void mnt_add_to_ns(struct mnt_namespace *ns, struct mount *mnt)
{
	struct rb_node **link = &ns->mounts.rb_node;
	struct rb_node *parent = NULL;

	WARN_ON(mnt->mnt.mnt_flags & MNT_ONRB);
	mnt->mnt_ns = ns;
	while (*link) {
		parent = *link;
		if (mnt->mnt_id_unique < node_to_mount(parent)->mnt_id_unique)
			link = &parent->rb_left;
		else
			link = &parent->rb_right;
	}
	rb_link_node(&mnt->mnt_node, parent, link);
	rb_insert_color(&mnt->mnt_node, &ns->mounts);
	mnt->mnt.mnt_flags |= MNT_ONRB;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * vfsmount lock must be held for write
 */
static void commit_tree(struct mount *mnt)
{
	struct mount *parent = mnt->mnt_parent;
	struct mount *m;
	LIST_HEAD(head);
	struct mnt_namespace *n = parent->mnt_ns;

	BUG_ON(parent == mnt);

	list_add_tail(&head, &mnt->mnt_list);
<<<<<<< HEAD
	list_for_each_entry(m, &head, mnt_list)
		m->mnt_ns = n;

	list_splice(&head, n->list.prev);

	list_add_tail(&mnt->mnt_hash, mount_hashtable +
				hash(&parent->mnt, mnt->mnt_mountpoint));
	list_add_tail(&mnt->mnt_child, &parent->mnt_mounts);
=======
	while (!list_empty(&head)) {
		m = list_first_entry(&head, typeof(*m), mnt_list);
		list_del(&m->mnt_list);

		mnt_add_to_ns(n, m);
	}
	n->nr_mounts += n->pending_mounts;
	n->pending_mounts = 0;

	__attach_mnt(mnt, parent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	touch_mnt_namespace(n);
}

static struct mount *next_mnt(struct mount *p, struct mount *root)
{
	struct list_head *next = p->mnt_mounts.next;
	if (next == &p->mnt_mounts) {
		while (1) {
			if (p == root)
				return NULL;
			next = p->mnt_child.next;
			if (next != &p->mnt_parent->mnt_mounts)
				break;
			p = p->mnt_parent;
		}
	}
	return list_entry(next, struct mount, mnt_child);
}

static struct mount *skip_mnt_tree(struct mount *p)
{
	struct list_head *prev = p->mnt_mounts.prev;
	while (prev != &p->mnt_mounts) {
		p = list_entry(prev, struct mount, mnt_child);
		prev = p->mnt_mounts.prev;
	}
	return p;
}

<<<<<<< HEAD
struct vfsmount *
vfs_kern_mount(struct file_system_type *type, int flags, const char *name, void *data)
{
	struct mount *mnt;
	struct dentry *root;

	if (!type)
		return ERR_PTR(-ENODEV);

	mnt = alloc_vfsmnt(name);
	if (!mnt)
		return ERR_PTR(-ENOMEM);

	mnt->mnt.data = NULL;
	if (type->alloc_mnt_data) {
		mnt->mnt.data = type->alloc_mnt_data();
		if (!mnt->mnt.data) {
			mnt_free_id(mnt);
			free_vfsmnt(mnt);
			return ERR_PTR(-ENOMEM);
		}
	}
	if (flags & MS_KERNMOUNT)
		mnt->mnt.mnt_flags = MNT_INTERNAL;

	root = mount_fs(type, flags, name, &mnt->mnt, data);
	if (IS_ERR(root)) {
		kfree(mnt->mnt.data);
		free_vfsmnt(mnt);
		return ERR_CAST(root);
	}

	mnt->mnt.mnt_root = root;
	mnt->mnt.mnt_sb = root->d_sb;
	mnt->mnt_mountpoint = mnt->mnt.mnt_root;
	mnt->mnt_parent = mnt;
	br_write_lock(&vfsmount_lock);
	list_add_tail(&mnt->mnt_instance, &root->d_sb->s_mounts);
	br_write_unlock(&vfsmount_lock);
	return &mnt->mnt;
}
EXPORT_SYMBOL_GPL(vfs_kern_mount);

=======
/**
 * vfs_create_mount - Create a mount for a configured superblock
 * @fc: The configuration context with the superblock attached
 *
 * Create a mount to an already configured superblock.  If necessary, the
 * caller should invoke vfs_get_tree() before calling this.
 *
 * Note that this does not attach the mount to anything.
 */
struct vfsmount *vfs_create_mount(struct fs_context *fc)
{
	struct mount *mnt;

	if (!fc->root)
		return ERR_PTR(-EINVAL);

	mnt = alloc_vfsmnt(fc->source ?: "none");
	if (!mnt)
		return ERR_PTR(-ENOMEM);

	if (fc->sb_flags & SB_KERNMOUNT)
		mnt->mnt.mnt_flags = MNT_INTERNAL;

	atomic_inc(&fc->root->d_sb->s_active);
	mnt->mnt.mnt_sb		= fc->root->d_sb;
	mnt->mnt.mnt_root	= dget(fc->root);
	mnt->mnt_mountpoint	= mnt->mnt.mnt_root;
	mnt->mnt_parent		= mnt;

	lock_mount_hash();
	list_add_tail(&mnt->mnt_instance, &mnt->mnt.mnt_sb->s_mounts);
	unlock_mount_hash();
	return &mnt->mnt;
}
EXPORT_SYMBOL(vfs_create_mount);

struct vfsmount *fc_mount(struct fs_context *fc)
{
	int err = vfs_get_tree(fc);
	if (!err) {
		up_write(&fc->root->d_sb->s_umount);
		return vfs_create_mount(fc);
	}
	return ERR_PTR(err);
}
EXPORT_SYMBOL(fc_mount);

struct vfsmount *vfs_kern_mount(struct file_system_type *type,
				int flags, const char *name,
				void *data)
{
	struct fs_context *fc;
	struct vfsmount *mnt;
	int ret = 0;

	if (!type)
		return ERR_PTR(-EINVAL);

	fc = fs_context_for_mount(type, flags);
	if (IS_ERR(fc))
		return ERR_CAST(fc);

	if (name)
		ret = vfs_parse_fs_string(fc, "source",
					  name, strlen(name));
	if (!ret)
		ret = parse_monolithic_mount_data(fc, data);
	if (!ret)
		mnt = fc_mount(fc);
	else
		mnt = ERR_PTR(ret);

	put_fs_context(fc);
	return mnt;
}
EXPORT_SYMBOL_GPL(vfs_kern_mount);

struct vfsmount *
vfs_submount(const struct dentry *mountpoint, struct file_system_type *type,
	     const char *name, void *data)
{
	/* Until it is worked out how to pass the user namespace
	 * through from the parent mount to the submount don't support
	 * unprivileged mounts with submounts.
	 */
	if (mountpoint->d_sb->s_user_ns != &init_user_ns)
		return ERR_PTR(-EPERM);

	return vfs_kern_mount(type, SB_SUBMOUNT, name, data);
}
EXPORT_SYMBOL_GPL(vfs_submount);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct mount *clone_mnt(struct mount *old, struct dentry *root,
					int flag)
{
	struct super_block *sb = old->mnt.mnt_sb;
	struct mount *mnt;
	int err;

	mnt = alloc_vfsmnt(old->mnt_devname);
	if (!mnt)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	if (sb->s_op->clone_mnt_data) {
		mnt->mnt.data = sb->s_op->clone_mnt_data(old->mnt.data);
		if (!mnt->mnt.data) {
			goto out_free;
		}
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (flag & (CL_SLAVE | CL_PRIVATE | CL_SHARED_TO_SLAVE))
		mnt->mnt_group_id = 0; /* not a peer of original */
	else
		mnt->mnt_group_id = old->mnt_group_id;

	if ((flag & CL_MAKE_SHARED) && !mnt->mnt_group_id) {
		err = mnt_alloc_group_id(mnt);
		if (err)
			goto out_free;
	}

<<<<<<< HEAD
	mnt->mnt.mnt_flags = old->mnt.mnt_flags & ~(MNT_WRITE_HOLD|MNT_MARKED);
	atomic_inc(&sb->s_active);
=======
	mnt->mnt.mnt_flags = old->mnt.mnt_flags;
	mnt->mnt.mnt_flags &= ~(MNT_WRITE_HOLD|MNT_MARKED|MNT_INTERNAL|MNT_ONRB);

	atomic_inc(&sb->s_active);
	mnt->mnt.mnt_idmap = mnt_idmap_get(mnt_idmap(&old->mnt));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mnt->mnt.mnt_sb = sb;
	mnt->mnt.mnt_root = dget(root);
	mnt->mnt_mountpoint = mnt->mnt.mnt_root;
	mnt->mnt_parent = mnt;
<<<<<<< HEAD
	br_write_lock(&vfsmount_lock);
	list_add_tail(&mnt->mnt_instance, &sb->s_mounts);
	br_write_unlock(&vfsmount_lock);
=======
	lock_mount_hash();
	list_add_tail(&mnt->mnt_instance, &sb->s_mounts);
	unlock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((flag & CL_SLAVE) ||
	    ((flag & CL_SHARED_TO_SLAVE) && IS_MNT_SHARED(old))) {
		list_add(&mnt->mnt_slave, &old->mnt_slave_list);
		mnt->mnt_master = old;
		CLEAR_MNT_SHARED(mnt);
	} else if (!(flag & CL_PRIVATE)) {
		if ((flag & CL_MAKE_SHARED) || IS_MNT_SHARED(old))
			list_add(&mnt->mnt_share, &old->mnt_share);
		if (IS_MNT_SLAVE(old))
			list_add(&mnt->mnt_slave, &old->mnt_slave);
		mnt->mnt_master = old->mnt_master;
<<<<<<< HEAD
=======
	} else {
		CLEAR_MNT_SHARED(mnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (flag & CL_MAKE_SHARED)
		set_mnt_shared(mnt);

	/* stick the duplicate mount on the same expiry list
	 * as the original if that was on one */
	if (flag & CL_EXPIRE) {
		if (!list_empty(&old->mnt_expire))
			list_add(&mnt->mnt_expire, &old->mnt_expire);
	}

	return mnt;

 out_free:
<<<<<<< HEAD
	kfree(mnt->mnt.data);
=======
	mnt_free_id(mnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	free_vfsmnt(mnt);
	return ERR_PTR(err);
}

<<<<<<< HEAD
static inline void mntfree(struct mount *mnt)
{
	struct vfsmount *m = &mnt->mnt;
	struct super_block *sb = m->mnt_sb;

	/*
	 * This probably indicates that somebody messed
	 * up a mnt_want/drop_write() pair.  If this
	 * happens, the filesystem was probably unable
	 * to make r/w->r/o transitions.
	 */
	/*
=======
static void cleanup_mnt(struct mount *mnt)
{
	struct hlist_node *p;
	struct mount *m;
	/*
	 * The warning here probably indicates that somebody messed
	 * up a mnt_want/drop_write() pair.  If this happens, the
	 * filesystem was probably unable to make r/w->r/o transitions.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * The locking used to deal with mnt_count decrement provides barriers,
	 * so mnt_get_writers() below is safe.
	 */
	WARN_ON(mnt_get_writers(mnt));
<<<<<<< HEAD
	fsnotify_vfsmount_delete(m);
	dput(m->mnt_root);
	free_vfsmnt(mnt);
	deactivate_super(sb);
}

static void mntput_no_expire(struct mount *mnt)
{
put_again:
#ifdef CONFIG_SMP
	br_read_lock(&vfsmount_lock);
	if (likely(mnt->mnt_ns)) {
		/* shouldn't be the last one */
		mnt_add_count(mnt, -1);
		br_read_unlock(&vfsmount_lock);
		return;
	}
	br_read_unlock(&vfsmount_lock);

	br_write_lock(&vfsmount_lock);
	mnt_add_count(mnt, -1);
	if (mnt_get_count(mnt)) {
		br_write_unlock(&vfsmount_lock);
		return;
	}
#else
	mnt_add_count(mnt, -1);
	if (likely(mnt_get_count(mnt)))
		return;
	br_write_lock(&vfsmount_lock);
#endif
	if (unlikely(mnt->mnt_pinned)) {
		mnt_add_count(mnt, mnt->mnt_pinned + 1);
		mnt->mnt_pinned = 0;
		br_write_unlock(&vfsmount_lock);
		acct_auto_close_mnt(&mnt->mnt);
		goto put_again;
	}

	list_del(&mnt->mnt_instance);
	br_write_unlock(&vfsmount_lock);
	mntfree(mnt);
=======
	if (unlikely(mnt->mnt_pins.first))
		mnt_pin_kill(mnt);
	hlist_for_each_entry_safe(m, p, &mnt->mnt_stuck_children, mnt_umount) {
		hlist_del(&m->mnt_umount);
		mntput(&m->mnt);
	}
	fsnotify_vfsmount_delete(&mnt->mnt);
	dput(mnt->mnt.mnt_root);
	deactivate_super(mnt->mnt.mnt_sb);
	mnt_free_id(mnt);
	call_rcu(&mnt->mnt_rcu, delayed_free_vfsmnt);
}

static void __cleanup_mnt(struct rcu_head *head)
{
	cleanup_mnt(container_of(head, struct mount, mnt_rcu));
}

static LLIST_HEAD(delayed_mntput_list);
static void delayed_mntput(struct work_struct *unused)
{
	struct llist_node *node = llist_del_all(&delayed_mntput_list);
	struct mount *m, *t;

	llist_for_each_entry_safe(m, t, node, mnt_llist)
		cleanup_mnt(m);
}
static DECLARE_DELAYED_WORK(delayed_mntput_work, delayed_mntput);

static void mntput_no_expire(struct mount *mnt)
{
	LIST_HEAD(list);
	int count;

	rcu_read_lock();
	if (likely(READ_ONCE(mnt->mnt_ns))) {
		/*
		 * Since we don't do lock_mount_hash() here,
		 * ->mnt_ns can change under us.  However, if it's
		 * non-NULL, then there's a reference that won't
		 * be dropped until after an RCU delay done after
		 * turning ->mnt_ns NULL.  So if we observe it
		 * non-NULL under rcu_read_lock(), the reference
		 * we are dropping is not the final one.
		 */
		mnt_add_count(mnt, -1);
		rcu_read_unlock();
		return;
	}
	lock_mount_hash();
	/*
	 * make sure that if __legitimize_mnt() has not seen us grab
	 * mount_lock, we'll see their refcount increment here.
	 */
	smp_mb();
	mnt_add_count(mnt, -1);
	count = mnt_get_count(mnt);
	if (count != 0) {
		WARN_ON(count < 0);
		rcu_read_unlock();
		unlock_mount_hash();
		return;
	}
	if (unlikely(mnt->mnt.mnt_flags & MNT_DOOMED)) {
		rcu_read_unlock();
		unlock_mount_hash();
		return;
	}
	mnt->mnt.mnt_flags |= MNT_DOOMED;
	rcu_read_unlock();

	list_del(&mnt->mnt_instance);

	if (unlikely(!list_empty(&mnt->mnt_mounts))) {
		struct mount *p, *tmp;
		list_for_each_entry_safe(p, tmp, &mnt->mnt_mounts,  mnt_child) {
			__put_mountpoint(unhash_mnt(p), &list);
			hlist_add_head(&p->mnt_umount, &mnt->mnt_stuck_children);
		}
	}
	unlock_mount_hash();
	shrink_dentry_list(&list);

	if (likely(!(mnt->mnt.mnt_flags & MNT_INTERNAL))) {
		struct task_struct *task = current;
		if (likely(!(task->flags & PF_KTHREAD))) {
			init_task_work(&mnt->mnt_rcu, __cleanup_mnt);
			if (!task_work_add(task, &mnt->mnt_rcu, TWA_RESUME))
				return;
		}
		if (llist_add(&mnt->mnt_llist, &delayed_mntput_list))
			schedule_delayed_work(&delayed_mntput_work, 1);
		return;
	}
	cleanup_mnt(mnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void mntput(struct vfsmount *mnt)
{
	if (mnt) {
		struct mount *m = real_mount(mnt);
<<<<<<< HEAD
		/* avoid cacheline pingpong, hope gcc doesn't get "smart" */
		if (unlikely(m->mnt_expiry_mark))
			m->mnt_expiry_mark = 0;
=======
		/* avoid cacheline pingpong */
		if (unlikely(m->mnt_expiry_mark))
			WRITE_ONCE(m->mnt_expiry_mark, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mntput_no_expire(m);
	}
}
EXPORT_SYMBOL(mntput);

struct vfsmount *mntget(struct vfsmount *mnt)
{
	if (mnt)
		mnt_add_count(real_mount(mnt), 1);
	return mnt;
}
EXPORT_SYMBOL(mntget);

<<<<<<< HEAD
void mnt_pin(struct vfsmount *mnt)
{
	br_write_lock(&vfsmount_lock);
	real_mount(mnt)->mnt_pinned++;
	br_write_unlock(&vfsmount_lock);
}
EXPORT_SYMBOL(mnt_pin);

void mnt_unpin(struct vfsmount *m)
{
	struct mount *mnt = real_mount(m);
	br_write_lock(&vfsmount_lock);
	if (mnt->mnt_pinned) {
		mnt_add_count(mnt, 1);
		mnt->mnt_pinned--;
	}
	br_write_unlock(&vfsmount_lock);
}
EXPORT_SYMBOL(mnt_unpin);

static inline void mangle(struct seq_file *m, const char *s)
{
	seq_escape(m, s, " \t\n\\");
}

/*
 * Simple .show_options callback for filesystems which don't want to
 * implement more complex mount option showing.
 *
 * See also save_mount_options().
 */
int generic_show_options(struct seq_file *m, struct dentry *root)
{
	const char *options;

	rcu_read_lock();
	options = rcu_dereference(root->d_sb->s_options);

	if (options != NULL && options[0]) {
		seq_putc(m, ',');
		mangle(m, options);
	}
	rcu_read_unlock();

	return 0;
}
EXPORT_SYMBOL(generic_show_options);

/*
 * If filesystem uses generic_show_options(), this function should be
 * called from the fill_super() callback.
 *
 * The .remount_fs callback usually needs to be handled in a special
 * way, to make sure, that previous options are not overwritten if the
 * remount fails.
 *
 * Also note, that if the filesystem's .remount_fs function doesn't
 * reset all options to their default value, but changes only newly
 * given options, then the displayed options will not reflect reality
 * any more.
 */
void save_mount_options(struct super_block *sb, char *options)
{
	BUG_ON(sb->s_options);
	rcu_assign_pointer(sb->s_options, kstrdup(options, GFP_KERNEL));
}
EXPORT_SYMBOL(save_mount_options);

void replace_mount_options(struct super_block *sb, char *options)
{
	char *old = sb->s_options;
	rcu_assign_pointer(sb->s_options, options);
	if (old) {
		synchronize_rcu();
		kfree(old);
	}
}
EXPORT_SYMBOL(replace_mount_options);

#ifdef CONFIG_PROC_FS
/* iterator; we want it to have access to namespace_sem, thus here... */
static void *m_start(struct seq_file *m, loff_t *pos)
{
	struct proc_mounts *p = proc_mounts(m);

	down_read(&namespace_sem);
	return seq_list_start(&p->ns->list, *pos);
=======
/*
 * Make a mount point inaccessible to new lookups.
 * Because there may still be current users, the caller MUST WAIT
 * for an RCU grace period before destroying the mount point.
 */
void mnt_make_shortterm(struct vfsmount *mnt)
{
	if (mnt)
		real_mount(mnt)->mnt_ns = NULL;
}

/**
 * path_is_mountpoint() - Check if path is a mount in the current namespace.
 * @path: path to check
 *
 *  d_mountpoint() can only be used reliably to establish if a dentry is
 *  not mounted in any namespace and that common case is handled inline.
 *  d_mountpoint() isn't aware of the possibility there may be multiple
 *  mounts using a given dentry in a different namespace. This function
 *  checks if the passed in path is a mountpoint rather than the dentry
 *  alone.
 */
bool path_is_mountpoint(const struct path *path)
{
	unsigned seq;
	bool res;

	if (!d_mountpoint(path->dentry))
		return false;

	rcu_read_lock();
	do {
		seq = read_seqbegin(&mount_lock);
		res = __path_is_mountpoint(path);
	} while (read_seqretry(&mount_lock, seq));
	rcu_read_unlock();

	return res;
}
EXPORT_SYMBOL(path_is_mountpoint);

struct vfsmount *mnt_clone_internal(const struct path *path)
{
	struct mount *p;
	p = clone_mnt(real_mount(path->mnt), path->dentry, CL_PRIVATE);
	if (IS_ERR(p))
		return ERR_CAST(p);
	p->mnt.mnt_flags |= MNT_INTERNAL;
	return &p->mnt;
}

/*
 * Returns the mount which either has the specified mnt_id, or has the next
 * smallest id afer the specified one.
 */
static struct mount *mnt_find_id_at(struct mnt_namespace *ns, u64 mnt_id)
{
	struct rb_node *node = ns->mounts.rb_node;
	struct mount *ret = NULL;

	while (node) {
		struct mount *m = node_to_mount(node);

		if (mnt_id <= m->mnt_id_unique) {
			ret = node_to_mount(node);
			if (mnt_id == m->mnt_id_unique)
				break;
			node = node->rb_left;
		} else {
			node = node->rb_right;
		}
	}
	return ret;
}

#ifdef CONFIG_PROC_FS

/* iterator; we want it to have access to namespace_sem, thus here... */
static void *m_start(struct seq_file *m, loff_t *pos)
{
	struct proc_mounts *p = m->private;

	down_read(&namespace_sem);

	return mnt_find_id_at(p->ns, *pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void *m_next(struct seq_file *m, void *v, loff_t *pos)
{
<<<<<<< HEAD
	struct proc_mounts *p = proc_mounts(m);

	return seq_list_next(v, &p->ns->list, pos);
=======
	struct mount *next = NULL, *mnt = v;
	struct rb_node *node = rb_next(&mnt->mnt_node);

	++*pos;
	if (node) {
		next = node_to_mount(node);
		*pos = next->mnt_id_unique;
	}
	return next;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void m_stop(struct seq_file *m, void *v)
{
	up_read(&namespace_sem);
}

static int m_show(struct seq_file *m, void *v)
{
<<<<<<< HEAD
	struct proc_mounts *p = proc_mounts(m);
	struct mount *r = list_entry(v, struct mount, mnt_list);
=======
	struct proc_mounts *p = m->private;
	struct mount *r = v;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return p->show(m, &r->mnt);
}

const struct seq_operations mounts_op = {
	.start	= m_start,
	.next	= m_next,
	.stop	= m_stop,
	.show	= m_show,
};
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif  /* CONFIG_PROC_FS */

/**
 * may_umount_tree - check if a mount tree is busy
<<<<<<< HEAD
 * @mnt: root of mount tree
=======
 * @m: root of mount tree
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This is called to check if a tree of mounts has any
 * open files, pwds, chroots or sub mounts that are
 * busy.
 */
int may_umount_tree(struct vfsmount *m)
{
	struct mount *mnt = real_mount(m);
	int actual_refs = 0;
	int minimum_refs = 0;
	struct mount *p;
	BUG_ON(!m);

	/* write lock needed for mnt_get_count */
<<<<<<< HEAD
	br_write_lock(&vfsmount_lock);
=======
	lock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (p = mnt; p; p = next_mnt(p, mnt)) {
		actual_refs += mnt_get_count(p);
		minimum_refs += 2;
	}
<<<<<<< HEAD
	br_write_unlock(&vfsmount_lock);
=======
	unlock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (actual_refs > minimum_refs)
		return 0;

	return 1;
}

EXPORT_SYMBOL(may_umount_tree);

/**
 * may_umount - check if a mount point is busy
 * @mnt: root of mount
 *
 * This is called to check if a mount point has any
 * open files, pwds, chroots or sub mounts. If the
 * mount has sub mounts this will return busy
 * regardless of whether the sub mounts are busy.
 *
 * Doesn't take quota and stuff into account. IOW, in some cases it will
 * give false negatives. The main reason why it's here is that we need
 * a non-destructive way to look for easily umountable filesystems.
 */
int may_umount(struct vfsmount *mnt)
{
	int ret = 1;
	down_read(&namespace_sem);
<<<<<<< HEAD
	br_write_lock(&vfsmount_lock);
	if (propagate_mount_busy(real_mount(mnt), 2))
		ret = 0;
	br_write_unlock(&vfsmount_lock);
=======
	lock_mount_hash();
	if (propagate_mount_busy(real_mount(mnt), 2))
		ret = 0;
	unlock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	up_read(&namespace_sem);
	return ret;
}

EXPORT_SYMBOL(may_umount);

<<<<<<< HEAD
void release_mounts(struct list_head *head)
{
	struct mount *mnt;
	while (!list_empty(head)) {
		mnt = list_first_entry(head, struct mount, mnt_hash);
		list_del_init(&mnt->mnt_hash);
		if (mnt_has_parent(mnt)) {
			struct dentry *dentry;
			struct mount *m;

			br_write_lock(&vfsmount_lock);
			dentry = mnt->mnt_mountpoint;
			m = mnt->mnt_parent;
			mnt->mnt_mountpoint = mnt->mnt.mnt_root;
			mnt->mnt_parent = mnt;
			m->mnt_ghosts--;
			br_write_unlock(&vfsmount_lock);
			dput(dentry);
			mntput(&m->mnt);
		}
		mntput(&mnt->mnt);
	}
}

/*
 * vfsmount lock must be held for write
 * namespace_sem must be held for write
 */
void umount_tree(struct mount *mnt, int propagate, struct list_head *kill)
=======
static void namespace_unlock(void)
{
	struct hlist_head head;
	struct hlist_node *p;
	struct mount *m;
	LIST_HEAD(list);

	hlist_move_list(&unmounted, &head);
	list_splice_init(&ex_mountpoints, &list);

	up_write(&namespace_sem);

	shrink_dentry_list(&list);

	if (likely(hlist_empty(&head)))
		return;

	synchronize_rcu_expedited();

	hlist_for_each_entry_safe(m, p, &head, mnt_umount) {
		hlist_del(&m->mnt_umount);
		mntput(&m->mnt);
	}
}

static inline void namespace_lock(void)
{
	down_write(&namespace_sem);
}

enum umount_tree_flags {
	UMOUNT_SYNC = 1,
	UMOUNT_PROPAGATE = 2,
	UMOUNT_CONNECTED = 4,
};

static bool disconnect_mount(struct mount *mnt, enum umount_tree_flags how)
{
	/* Leaving mounts connected is only valid for lazy umounts */
	if (how & UMOUNT_SYNC)
		return true;

	/* A mount without a parent has nothing to be connected to */
	if (!mnt_has_parent(mnt))
		return true;

	/* Because the reference counting rules change when mounts are
	 * unmounted and connected, umounted mounts may not be
	 * connected to mounted mounts.
	 */
	if (!(mnt->mnt_parent->mnt.mnt_flags & MNT_UMOUNT))
		return true;

	/* Has it been requested that the mount remain connected? */
	if (how & UMOUNT_CONNECTED)
		return false;

	/* Is the mount locked such that it needs to remain connected? */
	if (IS_MNT_LOCKED(mnt))
		return false;

	/* By default disconnect the mount */
	return true;
}

/*
 * mount_lock must be held
 * namespace_sem must be held for write
 */
static void umount_tree(struct mount *mnt, enum umount_tree_flags how)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	LIST_HEAD(tmp_list);
	struct mount *p;

<<<<<<< HEAD
	for (p = mnt; p; p = next_mnt(p, mnt))
		list_move(&p->mnt_hash, &tmp_list);

	list_for_each_entry(p, &tmp_list, mnt_hash)
		list_del_init(&p->mnt_child);

	if (propagate)
		propagate_umount(&tmp_list);

	list_for_each_entry(p, &tmp_list, mnt_hash) {
		list_del_init(&p->mnt_expire);
		list_del_init(&p->mnt_list);
		__touch_mnt_namespace(p->mnt_ns);
		p->mnt_ns = NULL;
		if (mnt_has_parent(p)) {
			p->mnt_parent->mnt_ghosts++;
			dentry_reset_mounted(p->mnt_mountpoint);
		}
		change_mnt_propagation(p, MS_PRIVATE);
	}
	list_splice(&tmp_list, kill);
}

static void shrink_submounts(struct mount *mnt, struct list_head *umounts);
=======
	if (how & UMOUNT_PROPAGATE)
		propagate_mount_unlock(mnt);

	/* Gather the mounts to umount */
	for (p = mnt; p; p = next_mnt(p, mnt)) {
		p->mnt.mnt_flags |= MNT_UMOUNT;
		if (p->mnt.mnt_flags & MNT_ONRB)
			move_from_ns(p, &tmp_list);
		else
			list_move(&p->mnt_list, &tmp_list);
	}

	/* Hide the mounts from mnt_mounts */
	list_for_each_entry(p, &tmp_list, mnt_list) {
		list_del_init(&p->mnt_child);
	}

	/* Add propogated mounts to the tmp_list */
	if (how & UMOUNT_PROPAGATE)
		propagate_umount(&tmp_list);

	while (!list_empty(&tmp_list)) {
		struct mnt_namespace *ns;
		bool disconnect;
		p = list_first_entry(&tmp_list, struct mount, mnt_list);
		list_del_init(&p->mnt_expire);
		list_del_init(&p->mnt_list);
		ns = p->mnt_ns;
		if (ns) {
			ns->nr_mounts--;
			__touch_mnt_namespace(ns);
		}
		p->mnt_ns = NULL;
		if (how & UMOUNT_SYNC)
			p->mnt.mnt_flags |= MNT_SYNC_UMOUNT;

		disconnect = disconnect_mount(p, how);
		if (mnt_has_parent(p)) {
			mnt_add_count(p->mnt_parent, -1);
			if (!disconnect) {
				/* Don't forget about p */
				list_add_tail(&p->mnt_child, &p->mnt_parent->mnt_mounts);
			} else {
				umount_mnt(p);
			}
		}
		change_mnt_propagation(p, MS_PRIVATE);
		if (disconnect)
			hlist_add_head(&p->mnt_umount, &unmounted);
	}
}

static void shrink_submounts(struct mount *mnt);

static int do_umount_root(struct super_block *sb)
{
	int ret = 0;

	down_write(&sb->s_umount);
	if (!sb_rdonly(sb)) {
		struct fs_context *fc;

		fc = fs_context_for_reconfigure(sb->s_root, SB_RDONLY,
						SB_RDONLY);
		if (IS_ERR(fc)) {
			ret = PTR_ERR(fc);
		} else {
			ret = parse_monolithic_mount_data(fc, NULL);
			if (!ret)
				ret = reconfigure_super(fc);
			put_fs_context(fc);
		}
	}
	up_write(&sb->s_umount);
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int do_umount(struct mount *mnt, int flags)
{
	struct super_block *sb = mnt->mnt.mnt_sb;
	int retval;
<<<<<<< HEAD
	LIST_HEAD(umount_list);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	retval = security_sb_umount(&mnt->mnt, flags);
	if (retval)
		return retval;

	/*
	 * Allow userspace to request a mountpoint be expired rather than
	 * unmounting unconditionally. Unmount only happens if:
	 *  (1) the mark is already set (the mark is cleared by mntput())
	 *  (2) the usage count == 1 [parent vfsmount] + 1 [sys_umount]
	 */
	if (flags & MNT_EXPIRE) {
		if (&mnt->mnt == current->fs->root.mnt ||
		    flags & (MNT_FORCE | MNT_DETACH))
			return -EINVAL;

		/*
		 * probably don't strictly need the lock here if we examined
		 * all race cases, but it's a slowpath.
		 */
<<<<<<< HEAD
		br_write_lock(&vfsmount_lock);
		if (mnt_get_count(mnt) != 2) {
			br_write_unlock(&vfsmount_lock);
			return -EBUSY;
		}
		br_write_unlock(&vfsmount_lock);
=======
		lock_mount_hash();
		if (mnt_get_count(mnt) != 2) {
			unlock_mount_hash();
			return -EBUSY;
		}
		unlock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!xchg(&mnt->mnt_expiry_mark, 1))
			return -EAGAIN;
	}

	/*
	 * If we may have to abort operations to get out of this
	 * mount, and they will themselves hold resources we must
	 * allow the fs to do things. In the Unix tradition of
	 * 'Gee thats tricky lets do it in userspace' the umount_begin
	 * might fail to complete on the first run through as other tasks
	 * must return, and the like. Thats for the mount program to worry
	 * about for the moment.
	 */

	if (flags & MNT_FORCE && sb->s_op->umount_begin) {
		sb->s_op->umount_begin(sb);
	}

	/*
	 * No sense to grab the lock for this test, but test itself looks
	 * somewhat bogus. Suggestions for better replacement?
	 * Ho-hum... In principle, we might treat that as umount + switch
	 * to rootfs. GC would eventually take care of the old vfsmount.
	 * Actually it makes sense, especially if rootfs would contain a
	 * /reboot - static binary that would close all descriptors and
	 * call reboot(9). Then init(8) could umount root and exec /reboot.
	 */
	if (&mnt->mnt == current->fs->root.mnt && !(flags & MNT_DETACH)) {
		/*
		 * Special case for "unmounting" root ...
		 * we just try to remount it readonly.
		 */
<<<<<<< HEAD
		down_write(&sb->s_umount);
		if (!(sb->s_flags & MS_RDONLY))
			retval = do_remount_sb(sb, MS_RDONLY, NULL, 0);
		up_write(&sb->s_umount);
		return retval;
	}

	down_write(&namespace_sem);
	br_write_lock(&vfsmount_lock);
	event++;

	if (!(flags & MNT_DETACH))
		shrink_submounts(mnt, &umount_list);

	retval = -EBUSY;
	if (flags & MNT_DETACH || !propagate_mount_busy(mnt, 2)) {
		if (!list_empty(&mnt->mnt_list))
			umount_tree(mnt, 1, &umount_list);
		retval = 0;
	}
	br_write_unlock(&vfsmount_lock);
	up_write(&namespace_sem);
	release_mounts(&umount_list);
=======
		if (!ns_capable(sb->s_user_ns, CAP_SYS_ADMIN))
			return -EPERM;
		return do_umount_root(sb);
	}

	namespace_lock();
	lock_mount_hash();

	/* Recheck MNT_LOCKED with the locks held */
	retval = -EINVAL;
	if (mnt->mnt.mnt_flags & MNT_LOCKED)
		goto out;

	event++;
	if (flags & MNT_DETACH) {
		if (mnt->mnt.mnt_flags & MNT_ONRB ||
		    !list_empty(&mnt->mnt_list))
			umount_tree(mnt, UMOUNT_PROPAGATE);
		retval = 0;
	} else {
		shrink_submounts(mnt);
		retval = -EBUSY;
		if (!propagate_mount_busy(mnt, 2)) {
			if (mnt->mnt.mnt_flags & MNT_ONRB ||
			    !list_empty(&mnt->mnt_list))
				umount_tree(mnt, UMOUNT_PROPAGATE|UMOUNT_SYNC);
			retval = 0;
		}
	}
out:
	unlock_mount_hash();
	namespace_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

/*
<<<<<<< HEAD
 * Now umount can handle mount points as well as block devices.
 * This is important for filesystems which use unnamed block devices.
 *
 * We now support a flag for forced unmount like the other 'big iron'
 * unixes. Our API is identical to OSF/1 to avoid making a mess of AMD
 */

SYSCALL_DEFINE2(umount, char __user *, name, int, flags)
{
	struct path path;
	struct mount *mnt;
	int retval;
	int lookup_flags = 0;

=======
 * __detach_mounts - lazily unmount all mounts on the specified dentry
 *
 * During unlink, rmdir, and d_drop it is possible to loose the path
 * to an existing mountpoint, and wind up leaking the mount.
 * detach_mounts allows lazily unmounting those mounts instead of
 * leaking them.
 *
 * The caller may hold dentry->d_inode->i_mutex.
 */
void __detach_mounts(struct dentry *dentry)
{
	struct mountpoint *mp;
	struct mount *mnt;

	namespace_lock();
	lock_mount_hash();
	mp = lookup_mountpoint(dentry);
	if (!mp)
		goto out_unlock;

	event++;
	while (!hlist_empty(&mp->m_list)) {
		mnt = hlist_entry(mp->m_list.first, struct mount, mnt_mp_list);
		if (mnt->mnt.mnt_flags & MNT_UMOUNT) {
			umount_mnt(mnt);
			hlist_add_head(&mnt->mnt_umount, &unmounted);
		}
		else umount_tree(mnt, UMOUNT_CONNECTED);
	}
	put_mountpoint(mp);
out_unlock:
	unlock_mount_hash();
	namespace_unlock();
}

/*
 * Is the caller allowed to modify his namespace?
 */
bool may_mount(void)
{
	return ns_capable(current->nsproxy->mnt_ns->user_ns, CAP_SYS_ADMIN);
}

/**
 * path_mounted - check whether path is mounted
 * @path: path to check
 *
 * Determine whether @path refers to the root of a mount.
 *
 * Return: true if @path is the root of a mount, false if not.
 */
static inline bool path_mounted(const struct path *path)
{
	return path->mnt->mnt_root == path->dentry;
}

static void warn_mandlock(void)
{
	pr_warn_once("=======================================================\n"
		     "WARNING: The mand mount option has been deprecated and\n"
		     "         and is ignored by this kernel. Remove the mand\n"
		     "         option from the mount to silence this warning.\n"
		     "=======================================================\n");
}

static int can_umount(const struct path *path, int flags)
{
	struct mount *mnt = real_mount(path->mnt);

	if (!may_mount())
		return -EPERM;
	if (!path_mounted(path))
		return -EINVAL;
	if (!check_mnt(mnt))
		return -EINVAL;
	if (mnt->mnt.mnt_flags & MNT_LOCKED) /* Check optimistically */
		return -EINVAL;
	if (flags & MNT_FORCE && !capable(CAP_SYS_ADMIN))
		return -EPERM;
	return 0;
}

// caller is responsible for flags being sane
int path_umount(struct path *path, int flags)
{
	struct mount *mnt = real_mount(path->mnt);
	int ret;

	ret = can_umount(path, flags);
	if (!ret)
		ret = do_umount(mnt, flags);

	/* we mustn't call path_put() as that would clear mnt_expiry_mark */
	dput(path->dentry);
	mntput_no_expire(mnt);
	return ret;
}

static int ksys_umount(char __user *name, int flags)
{
	int lookup_flags = LOOKUP_MOUNTPOINT;
	struct path path;
	int ret;

	// basic validity checks done first
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (flags & ~(MNT_FORCE | MNT_DETACH | MNT_EXPIRE | UMOUNT_NOFOLLOW))
		return -EINVAL;

	if (!(flags & UMOUNT_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;
<<<<<<< HEAD

	retval = user_path_at(AT_FDCWD, name, lookup_flags, &path);
	if (retval)
		goto out;
	mnt = real_mount(path.mnt);
	retval = -EINVAL;
	if (path.dentry != path.mnt->mnt_root)
		goto dput_and_out;
	if (!check_mnt(mnt))
		goto dput_and_out;

	retval = -EPERM;
	if (!ns_capable(mnt->mnt_ns->user_ns, CAP_SYS_ADMIN))
		goto dput_and_out;

	retval = do_umount(mnt, flags);
dput_and_out:
	/* we mustn't call path_put() as that would clear mnt_expiry_mark */
	dput(path.dentry);
	mntput_no_expire(mnt);
out:
	return retval;
=======
	ret = user_path_at(AT_FDCWD, name, lookup_flags, &path);
	if (ret)
		return ret;
	return path_umount(&path, flags);
}

SYSCALL_DEFINE2(umount, char __user *, name, int, flags)
{
	return ksys_umount(name, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef __ARCH_WANT_SYS_OLDUMOUNT

/*
 *	The 2.0 compatible umount. No flags.
 */
SYSCALL_DEFINE1(oldumount, char __user *, name)
{
<<<<<<< HEAD
	return sys_umount(name, 0);
=======
	return ksys_umount(name, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif

<<<<<<< HEAD
static int mount_is_safe(struct path *path)
{
	if (ns_capable(real_mount(path->mnt)->mnt_ns->user_ns, CAP_SYS_ADMIN))
		return 0;
	return -EPERM;
#ifdef notyet
	if (S_ISLNK(path->dentry->d_inode->i_mode))
		return -EPERM;
	if (path->dentry->d_inode->i_mode & S_ISVTX) {
		if (current_uid() != path->dentry->d_inode->i_uid)
			return -EPERM;
	}
	if (inode_permission(path->dentry->d_inode, MAY_WRITE))
		return -EPERM;
	return 0;
#endif
}

static bool mnt_ns_loop(struct path *path)
=======
static bool is_mnt_ns_file(struct dentry *dentry)
{
	/* Is this a proxy for a mount namespace? */
	return dentry->d_op == &ns_dentry_operations &&
	       dentry->d_fsdata == &mntns_operations;
}

static struct mnt_namespace *to_mnt_ns(struct ns_common *ns)
{
	return container_of(ns, struct mnt_namespace, ns);
}

struct ns_common *from_mnt_ns(struct mnt_namespace *mnt)
{
	return &mnt->ns;
}

static bool mnt_ns_loop(struct dentry *dentry)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Could bind mounting the mount namespace inode cause a
	 * mount namespace loop?
	 */
<<<<<<< HEAD
	struct inode *inode = path->dentry->d_inode;
	struct proc_inode *ei;
	struct mnt_namespace *mnt_ns;

	if (!proc_ns_inode(inode))
		return false;

	ei = PROC_I(inode);
	if (ei->ns_ops != &mntns_operations)
		return false;

	mnt_ns = ei->ns;
=======
	struct mnt_namespace *mnt_ns;
	if (!is_mnt_ns_file(dentry))
		return false;

	mnt_ns = to_mnt_ns(get_proc_ns(dentry->d_inode));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return current->nsproxy->mnt_ns->seq >= mnt_ns->seq;
}

struct mount *copy_tree(struct mount *mnt, struct dentry *dentry,
					int flag)
{
<<<<<<< HEAD
	struct mount *res, *p, *q, *r;
	struct path path;

	if (!(flag & CL_COPY_ALL) && IS_MNT_UNBINDABLE(mnt))
=======
	struct mount *res, *p, *q, *r, *parent;

	if (!(flag & CL_COPY_UNBINDABLE) && IS_MNT_UNBINDABLE(mnt))
		return ERR_PTR(-EINVAL);

	if (!(flag & CL_COPY_MNT_NS_FILE) && is_mnt_ns_file(dentry))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ERR_PTR(-EINVAL);

	res = q = clone_mnt(mnt, dentry, flag);
	if (IS_ERR(q))
		return q;

	q->mnt_mountpoint = mnt->mnt_mountpoint;

	p = mnt;
	list_for_each_entry(r, &mnt->mnt_mounts, mnt_child) {
		struct mount *s;
		if (!is_subdir(r->mnt_mountpoint, dentry))
			continue;

		for (s = r; s; s = next_mnt(s, r)) {
<<<<<<< HEAD
			if (!(flag & CL_COPY_ALL) && IS_MNT_UNBINDABLE(s)) {
=======
			if (!(flag & CL_COPY_UNBINDABLE) &&
			    IS_MNT_UNBINDABLE(s)) {
				if (s->mnt.mnt_flags & MNT_LOCKED) {
					/* Both unbindable and locked. */
					q = ERR_PTR(-EPERM);
					goto out;
				} else {
					s = skip_mnt_tree(s);
					continue;
				}
			}
			if (!(flag & CL_COPY_MNT_NS_FILE) &&
			    is_mnt_ns_file(s->mnt.mnt_root)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				s = skip_mnt_tree(s);
				continue;
			}
			while (p != s->mnt_parent) {
				p = p->mnt_parent;
				q = q->mnt_parent;
			}
			p = s;
<<<<<<< HEAD
			path.mnt = &q->mnt;
			path.dentry = p->mnt_mountpoint;
			q = clone_mnt(p, p->mnt.mnt_root, flag);
			if (IS_ERR(q))
				goto out;
			br_write_lock(&vfsmount_lock);
			list_add_tail(&q->mnt_list, &res->mnt_list);
			attach_mnt(q, &path);
			br_write_unlock(&vfsmount_lock);
=======
			parent = q;
			q = clone_mnt(p, p->mnt.mnt_root, flag);
			if (IS_ERR(q))
				goto out;
			lock_mount_hash();
			list_add_tail(&q->mnt_list, &res->mnt_list);
			attach_mnt(q, parent, p->mnt_mp, false);
			unlock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	return res;
out:
	if (res) {
<<<<<<< HEAD
		LIST_HEAD(umount_list);
		br_write_lock(&vfsmount_lock);
		umount_tree(res, 0, &umount_list);
		br_write_unlock(&vfsmount_lock);
		release_mounts(&umount_list);
=======
		lock_mount_hash();
		umount_tree(res, UMOUNT_SYNC);
		unlock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return q;
}

/* Caller should check returned pointer for errors */

<<<<<<< HEAD
struct vfsmount *collect_mounts(struct path *path)
{
	struct mount *tree;
	down_write(&namespace_sem);
=======
struct vfsmount *collect_mounts(const struct path *path)
{
	struct mount *tree;
	namespace_lock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!check_mnt(real_mount(path->mnt)))
		tree = ERR_PTR(-EINVAL);
	else
		tree = copy_tree(real_mount(path->mnt), path->dentry,
				 CL_COPY_ALL | CL_PRIVATE);
<<<<<<< HEAD
	up_write(&namespace_sem);
	if (IS_ERR(tree))
		return NULL;
	return &tree->mnt;
}

void drop_collected_mounts(struct vfsmount *mnt)
{
	LIST_HEAD(umount_list);
	down_write(&namespace_sem);
	br_write_lock(&vfsmount_lock);
	umount_tree(real_mount(mnt), 0, &umount_list);
	br_write_unlock(&vfsmount_lock);
	up_write(&namespace_sem);
	release_mounts(&umount_list);
}

=======
	namespace_unlock();
	if (IS_ERR(tree))
		return ERR_CAST(tree);
	return &tree->mnt;
}

static void free_mnt_ns(struct mnt_namespace *);
static struct mnt_namespace *alloc_mnt_ns(struct user_namespace *, bool);

void dissolve_on_fput(struct vfsmount *mnt)
{
	struct mnt_namespace *ns;
	namespace_lock();
	lock_mount_hash();
	ns = real_mount(mnt)->mnt_ns;
	if (ns) {
		if (is_anon_ns(ns))
			umount_tree(real_mount(mnt), UMOUNT_CONNECTED);
		else
			ns = NULL;
	}
	unlock_mount_hash();
	namespace_unlock();
	if (ns)
		free_mnt_ns(ns);
}

void drop_collected_mounts(struct vfsmount *mnt)
{
	namespace_lock();
	lock_mount_hash();
	umount_tree(real_mount(mnt), 0);
	unlock_mount_hash();
	namespace_unlock();
}

static bool has_locked_children(struct mount *mnt, struct dentry *dentry)
{
	struct mount *child;

	list_for_each_entry(child, &mnt->mnt_mounts, mnt_child) {
		if (!is_subdir(child->mnt_mountpoint, dentry))
			continue;

		if (child->mnt.mnt_flags & MNT_LOCKED)
			return true;
	}
	return false;
}

/**
 * clone_private_mount - create a private clone of a path
 * @path: path to clone
 *
 * This creates a new vfsmount, which will be the clone of @path.  The new mount
 * will not be attached anywhere in the namespace and will be private (i.e.
 * changes to the originating mount won't be propagated into this).
 *
 * Release with mntput().
 */
struct vfsmount *clone_private_mount(const struct path *path)
{
	struct mount *old_mnt = real_mount(path->mnt);
	struct mount *new_mnt;

	down_read(&namespace_sem);
	if (IS_MNT_UNBINDABLE(old_mnt))
		goto invalid;

	if (!check_mnt(old_mnt))
		goto invalid;

	if (has_locked_children(old_mnt, path->dentry))
		goto invalid;

	new_mnt = clone_mnt(old_mnt, path->dentry, CL_PRIVATE);
	up_read(&namespace_sem);

	if (IS_ERR(new_mnt))
		return ERR_CAST(new_mnt);

	/* Longterm mount to be removed by kern_unmount*() */
	new_mnt->mnt_ns = MNT_NS_INTERNAL;

	return &new_mnt->mnt;

invalid:
	up_read(&namespace_sem);
	return ERR_PTR(-EINVAL);
}
EXPORT_SYMBOL_GPL(clone_private_mount);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int iterate_mounts(int (*f)(struct vfsmount *, void *), void *arg,
		   struct vfsmount *root)
{
	struct mount *mnt;
	int res = f(root, arg);
	if (res)
		return res;
	list_for_each_entry(mnt, &real_mount(root)->mnt_list, mnt_list) {
		res = f(&mnt->mnt, arg);
		if (res)
			return res;
	}
	return 0;
}

<<<<<<< HEAD
=======
static void lock_mnt_tree(struct mount *mnt)
{
	struct mount *p;

	for (p = mnt; p; p = next_mnt(p, mnt)) {
		int flags = p->mnt.mnt_flags;
		/* Don't allow unprivileged users to change mount flags */
		flags |= MNT_LOCK_ATIME;

		if (flags & MNT_READONLY)
			flags |= MNT_LOCK_READONLY;

		if (flags & MNT_NODEV)
			flags |= MNT_LOCK_NODEV;

		if (flags & MNT_NOSUID)
			flags |= MNT_LOCK_NOSUID;

		if (flags & MNT_NOEXEC)
			flags |= MNT_LOCK_NOEXEC;
		/* Don't allow unprivileged users to reveal what is under a mount */
		if (list_empty(&p->mnt_expire))
			flags |= MNT_LOCKED;
		p->mnt.mnt_flags = flags;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void cleanup_group_ids(struct mount *mnt, struct mount *end)
{
	struct mount *p;

	for (p = mnt; p != end; p = next_mnt(p, mnt)) {
		if (p->mnt_group_id && !IS_MNT_SHARED(p))
			mnt_release_group_id(p);
	}
}

static int invent_group_ids(struct mount *mnt, bool recurse)
{
	struct mount *p;

	for (p = mnt; p; p = recurse ? next_mnt(p, mnt) : NULL) {
		if (!p->mnt_group_id && !IS_MNT_SHARED(p)) {
			int err = mnt_alloc_group_id(p);
			if (err) {
				cleanup_group_ids(mnt, p);
				return err;
			}
		}
	}

	return 0;
}

<<<<<<< HEAD
/*
 *  @source_mnt : mount tree to be attached
 *  @nd         : place the mount tree @source_mnt is attached
 *  @parent_nd  : if non-null, detach the source_mnt from its parent and
 *  		   store the parent mount and mountpoint dentry.
 *  		   (done when source_mnt is moved)
=======
int count_mounts(struct mnt_namespace *ns, struct mount *mnt)
{
	unsigned int max = READ_ONCE(sysctl_mount_max);
	unsigned int mounts = 0;
	struct mount *p;

	if (ns->nr_mounts >= max)
		return -ENOSPC;
	max -= ns->nr_mounts;
	if (ns->pending_mounts >= max)
		return -ENOSPC;
	max -= ns->pending_mounts;

	for (p = mnt; p; p = next_mnt(p, mnt))
		mounts++;

	if (mounts > max)
		return -ENOSPC;

	ns->pending_mounts += mounts;
	return 0;
}

enum mnt_tree_flags_t {
	MNT_TREE_MOVE = BIT(0),
	MNT_TREE_BENEATH = BIT(1),
};

/**
 * attach_recursive_mnt - attach a source mount tree
 * @source_mnt: mount tree to be attached
 * @top_mnt:    mount that @source_mnt will be mounted on or mounted beneath
 * @dest_mp:    the mountpoint @source_mnt will be mounted at
 * @flags:      modify how @source_mnt is supposed to be attached
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  NOTE: in the table below explains the semantics when a source mount
 *  of a given type is attached to a destination mount of a given type.
 * ---------------------------------------------------------------------------
 * |         BIND MOUNT OPERATION                                            |
 * |**************************************************************************
 * | source-->| shared        |       private  |       slave    | unbindable |
 * | dest     |               |                |                |            |
 * |   |      |               |                |                |            |
 * |   v      |               |                |                |            |
 * |**************************************************************************
 * |  shared  | shared (++)   |     shared (+) |     shared(+++)|  invalid   |
 * |          |               |                |                |            |
 * |non-shared| shared (+)    |      private   |      slave (*) |  invalid   |
 * ***************************************************************************
 * A bind operation clones the source mount and mounts the clone on the
 * destination mount.
 *
 * (++)  the cloned mount is propagated to all the mounts in the propagation
 * 	 tree of the destination mount and the cloned mount is added to
 * 	 the peer group of the source mount.
 * (+)   the cloned mount is created under the destination mount and is marked
 *       as shared. The cloned mount is added to the peer group of the source
 *       mount.
 * (+++) the mount is propagated to all the mounts in the propagation tree
 *       of the destination mount and the cloned mount is made slave
 *       of the same master as that of the source mount. The cloned mount
 *       is marked as 'shared and slave'.
 * (*)   the cloned mount is made a slave of the same master as that of the
 * 	 source mount.
 *
 * ---------------------------------------------------------------------------
 * |         		MOVE MOUNT OPERATION                                 |
 * |**************************************************************************
 * | source-->| shared        |       private  |       slave    | unbindable |
 * | dest     |               |                |                |            |
 * |   |      |               |                |                |            |
 * |   v      |               |                |                |            |
 * |**************************************************************************
 * |  shared  | shared (+)    |     shared (+) |    shared(+++) |  invalid   |
 * |          |               |                |                |            |
 * |non-shared| shared (+*)   |      private   |    slave (*)   | unbindable |
 * ***************************************************************************
 *
 * (+)  the mount is moved to the destination. And is then propagated to
 * 	all the mounts in the propagation tree of the destination mount.
 * (+*)  the mount is moved to the destination.
 * (+++)  the mount is moved to the destination and is then propagated to
 * 	all the mounts belonging to the destination mount's propagation tree.
 * 	the mount is marked as 'shared and slave'.
 * (*)	the mount continues to be a slave at the new location.
 *
 * if the source mount is a tree, the operations explained above is
 * applied to each mount in the tree.
 * Must be called without spinlocks held, since this function can sleep
 * in allocations.
<<<<<<< HEAD
 */
static int attach_recursive_mnt(struct mount *source_mnt,
			struct path *path, struct path *parent_path)
{
	LIST_HEAD(tree_list);
	struct mount *dest_mnt = real_mount(path->mnt);
	struct dentry *dest_dentry = path->dentry;
	struct mount *child, *p;
	int err;
=======
 *
 * Context: The function expects namespace_lock() to be held.
 * Return: If @source_mnt was successfully attached 0 is returned.
 *         Otherwise a negative error code is returned.
 */
static int attach_recursive_mnt(struct mount *source_mnt,
				struct mount *top_mnt,
				struct mountpoint *dest_mp,
				enum mnt_tree_flags_t flags)
{
	struct user_namespace *user_ns = current->nsproxy->mnt_ns->user_ns;
	HLIST_HEAD(tree_list);
	struct mnt_namespace *ns = top_mnt->mnt_ns;
	struct mountpoint *smp;
	struct mount *child, *dest_mnt, *p;
	struct hlist_node *n;
	int err = 0;
	bool moving = flags & MNT_TREE_MOVE, beneath = flags & MNT_TREE_BENEATH;

	/*
	 * Preallocate a mountpoint in case the new mounts need to be
	 * mounted beneath mounts on the same mountpoint.
	 */
	smp = get_mountpoint(source_mnt->mnt.mnt_root);
	if (IS_ERR(smp))
		return PTR_ERR(smp);

	/* Is there space to add these mounts to the mount namespace? */
	if (!moving) {
		err = count_mounts(ns, source_mnt);
		if (err)
			goto out;
	}

	if (beneath)
		dest_mnt = top_mnt->mnt_parent;
	else
		dest_mnt = top_mnt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (IS_MNT_SHARED(dest_mnt)) {
		err = invent_group_ids(source_mnt, true);
		if (err)
			goto out;
<<<<<<< HEAD
		err = propagate_mnt(dest_mnt, dest_dentry, source_mnt, &tree_list);
		br_write_lock(&vfsmount_lock);
		if (err)
			goto out_cleanup_ids;
		for (p = source_mnt; p; p = next_mnt(p, source_mnt))
			set_mnt_shared(p);
	} else {
		br_write_lock(&vfsmount_lock);
	}
	if (parent_path) {
		detach_mnt(source_mnt, parent_path);
		attach_mnt(source_mnt, path);
		touch_mnt_namespace(source_mnt->mnt_ns);
	} else {
		mnt_set_mountpoint(dest_mnt, dest_dentry, source_mnt);
		commit_tree(source_mnt);
	}

	list_for_each_entry_safe(child, p, &tree_list, mnt_hash) {
		list_del_init(&child->mnt_hash);
		commit_tree(child);
	}
	br_write_unlock(&vfsmount_lock);
=======
		err = propagate_mnt(dest_mnt, dest_mp, source_mnt, &tree_list);
	}
	lock_mount_hash();
	if (err)
		goto out_cleanup_ids;

	if (IS_MNT_SHARED(dest_mnt)) {
		for (p = source_mnt; p; p = next_mnt(p, source_mnt))
			set_mnt_shared(p);
	}

	if (moving) {
		if (beneath)
			dest_mp = smp;
		unhash_mnt(source_mnt);
		attach_mnt(source_mnt, top_mnt, dest_mp, beneath);
		touch_mnt_namespace(source_mnt->mnt_ns);
	} else {
		if (source_mnt->mnt_ns) {
			LIST_HEAD(head);

			/* move from anon - the caller will destroy */
			for (p = source_mnt; p; p = next_mnt(p, source_mnt))
				move_from_ns(p, &head);
			list_del_init(&head);
		}
		if (beneath)
			mnt_set_mountpoint_beneath(source_mnt, top_mnt, smp);
		else
			mnt_set_mountpoint(dest_mnt, dest_mp, source_mnt);
		commit_tree(source_mnt);
	}

	hlist_for_each_entry_safe(child, n, &tree_list, mnt_hash) {
		struct mount *q;
		hlist_del_init(&child->mnt_hash);
		q = __lookup_mnt(&child->mnt_parent->mnt,
				 child->mnt_mountpoint);
		if (q)
			mnt_change_mountpoint(child, smp, q);
		/* Notice when we are propagating across user namespaces */
		if (child->mnt_parent->mnt_ns->user_ns != user_ns)
			lock_mnt_tree(child);
		child->mnt.mnt_flags &= ~MNT_LOCKED;
		commit_tree(child);
	}
	put_mountpoint(smp);
	unlock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 out_cleanup_ids:
<<<<<<< HEAD
	while (!list_empty(&tree_list)) {
		child = list_first_entry(&tree_list, struct mount, mnt_hash);
		umount_tree(child, 0, &tree_list);
	}
	br_write_unlock(&vfsmount_lock);
	cleanup_group_ids(source_mnt, NULL);
 out:
	return err;
}

static int lock_mount(struct path *path)
{
	struct vfsmount *mnt;
retry:
	mutex_lock(&path->dentry->d_inode->i_mutex);
	if (unlikely(cant_mount(path->dentry))) {
		mutex_unlock(&path->dentry->d_inode->i_mutex);
		return -ENOENT;
	}
	down_write(&namespace_sem);
	mnt = lookup_mnt(path);
	if (likely(!mnt))
		return 0;
	up_write(&namespace_sem);
	mutex_unlock(&path->dentry->d_inode->i_mutex);
	path_put(path);
	path->mnt = mnt;
	path->dentry = dget(mnt->mnt_root);
	goto retry;
}

static void unlock_mount(struct path *path)
{
	up_write(&namespace_sem);
	mutex_unlock(&path->dentry->d_inode->i_mutex);
}

static int graft_tree(struct mount *mnt, struct path *path)
{
	if (mnt->mnt.mnt_sb->s_flags & MS_NOUSER)
		return -EINVAL;

	if (S_ISDIR(path->dentry->d_inode->i_mode) !=
	      S_ISDIR(mnt->mnt.mnt_root->d_inode->i_mode))
		return -ENOTDIR;

	if (d_unlinked(path->dentry))
		return -ENOENT;

	return attach_recursive_mnt(mnt, path, NULL);
=======
	while (!hlist_empty(&tree_list)) {
		child = hlist_entry(tree_list.first, struct mount, mnt_hash);
		child->mnt_parent->mnt_ns->pending_mounts = 0;
		umount_tree(child, UMOUNT_SYNC);
	}
	unlock_mount_hash();
	cleanup_group_ids(source_mnt, NULL);
 out:
	ns->pending_mounts = 0;

	read_seqlock_excl(&mount_lock);
	put_mountpoint(smp);
	read_sequnlock_excl(&mount_lock);

	return err;
}

/**
 * do_lock_mount - lock mount and mountpoint
 * @path:    target path
 * @beneath: whether the intention is to mount beneath @path
 *
 * Follow the mount stack on @path until the top mount @mnt is found. If
 * the initial @path->{mnt,dentry} is a mountpoint lookup the first
 * mount stacked on top of it. Then simply follow @{mnt,mnt->mnt_root}
 * until nothing is stacked on top of it anymore.
 *
 * Acquire the inode_lock() on the top mount's ->mnt_root to protect
 * against concurrent removal of the new mountpoint from another mount
 * namespace.
 *
 * If @beneath is requested, acquire inode_lock() on @mnt's mountpoint
 * @mp on @mnt->mnt_parent must be acquired. This protects against a
 * concurrent unlink of @mp->mnt_dentry from another mount namespace
 * where @mnt doesn't have a child mount mounted @mp. A concurrent
 * removal of @mnt->mnt_root doesn't matter as nothing will be mounted
 * on top of it for @beneath.
 *
 * In addition, @beneath needs to make sure that @mnt hasn't been
 * unmounted or moved from its current mountpoint in between dropping
 * @mount_lock and acquiring @namespace_sem. For the !@beneath case @mnt
 * being unmounted would be detected later by e.g., calling
 * check_mnt(mnt) in the function it's called from. For the @beneath
 * case however, it's useful to detect it directly in do_lock_mount().
 * If @mnt hasn't been unmounted then @mnt->mnt_mountpoint still points
 * to @mnt->mnt_mp->m_dentry. But if @mnt has been unmounted it will
 * point to @mnt->mnt_root and @mnt->mnt_mp will be NULL.
 *
 * Return: Either the target mountpoint on the top mount or the top
 *         mount's mountpoint.
 */
static struct mountpoint *do_lock_mount(struct path *path, bool beneath)
{
	struct vfsmount *mnt = path->mnt;
	struct dentry *dentry;
	struct mountpoint *mp = ERR_PTR(-ENOENT);

	for (;;) {
		struct mount *m;

		if (beneath) {
			m = real_mount(mnt);
			read_seqlock_excl(&mount_lock);
			dentry = dget(m->mnt_mountpoint);
			read_sequnlock_excl(&mount_lock);
		} else {
			dentry = path->dentry;
		}

		inode_lock(dentry->d_inode);
		if (unlikely(cant_mount(dentry))) {
			inode_unlock(dentry->d_inode);
			goto out;
		}

		namespace_lock();

		if (beneath && (!is_mounted(mnt) || m->mnt_mountpoint != dentry)) {
			namespace_unlock();
			inode_unlock(dentry->d_inode);
			goto out;
		}

		mnt = lookup_mnt(path);
		if (likely(!mnt))
			break;

		namespace_unlock();
		inode_unlock(dentry->d_inode);
		if (beneath)
			dput(dentry);
		path_put(path);
		path->mnt = mnt;
		path->dentry = dget(mnt->mnt_root);
	}

	mp = get_mountpoint(dentry);
	if (IS_ERR(mp)) {
		namespace_unlock();
		inode_unlock(dentry->d_inode);
	}

out:
	if (beneath)
		dput(dentry);

	return mp;
}

static inline struct mountpoint *lock_mount(struct path *path)
{
	return do_lock_mount(path, false);
}

static void unlock_mount(struct mountpoint *where)
{
	struct dentry *dentry = where->m_dentry;

	read_seqlock_excl(&mount_lock);
	put_mountpoint(where);
	read_sequnlock_excl(&mount_lock);

	namespace_unlock();
	inode_unlock(dentry->d_inode);
}

static int graft_tree(struct mount *mnt, struct mount *p, struct mountpoint *mp)
{
	if (mnt->mnt.mnt_sb->s_flags & SB_NOUSER)
		return -EINVAL;

	if (d_is_dir(mp->m_dentry) !=
	      d_is_dir(mnt->mnt.mnt_root))
		return -ENOTDIR;

	return attach_recursive_mnt(mnt, p, mp, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Sanity check the flags to change_mnt_propagation.
 */

<<<<<<< HEAD
static int flags_to_propagation_type(int flags)
{
	int type = flags & ~(MS_REC | MS_SILENT);
=======
static int flags_to_propagation_type(int ms_flags)
{
	int type = ms_flags & ~(MS_REC | MS_SILENT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Fail if any non-propagation flags are set */
	if (type & ~(MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE))
		return 0;
	/* Only one propagation flag should be set */
	if (!is_power_of_2(type))
		return 0;
	return type;
}

/*
 * recursively change the type of the mountpoint.
 */
<<<<<<< HEAD
static int do_change_type(struct path *path, int flag)
{
	struct mount *m;
	struct mount *mnt = real_mount(path->mnt);
	int recurse = flag & MS_REC;
	int type;
	int err = 0;

	if (!ns_capable(mnt->mnt_ns->user_ns, CAP_SYS_ADMIN))
		return -EPERM;

	if (path->dentry != path->mnt->mnt_root)
		return -EINVAL;

	type = flags_to_propagation_type(flag);
	if (!type)
		return -EINVAL;

	down_write(&namespace_sem);
=======
static int do_change_type(struct path *path, int ms_flags)
{
	struct mount *m;
	struct mount *mnt = real_mount(path->mnt);
	int recurse = ms_flags & MS_REC;
	int type;
	int err = 0;

	if (!path_mounted(path))
		return -EINVAL;

	type = flags_to_propagation_type(ms_flags);
	if (!type)
		return -EINVAL;

	namespace_lock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (type == MS_SHARED) {
		err = invent_group_ids(mnt, recurse);
		if (err)
			goto out_unlock;
	}

<<<<<<< HEAD
	br_write_lock(&vfsmount_lock);
	for (m = mnt; m; m = (recurse ? next_mnt(m, mnt) : NULL))
		change_mnt_propagation(m, type);
	br_write_unlock(&vfsmount_lock);

 out_unlock:
	up_write(&namespace_sem);
	return err;
}

=======
	lock_mount_hash();
	for (m = mnt; m; m = (recurse ? next_mnt(m, mnt) : NULL))
		change_mnt_propagation(m, type);
	unlock_mount_hash();

 out_unlock:
	namespace_unlock();
	return err;
}

static struct mount *__do_loopback(struct path *old_path, int recurse)
{
	struct mount *mnt = ERR_PTR(-EINVAL), *old = real_mount(old_path->mnt);

	if (IS_MNT_UNBINDABLE(old))
		return mnt;

	if (!check_mnt(old) && old_path->dentry->d_op != &ns_dentry_operations)
		return mnt;

	if (!recurse && has_locked_children(old, old_path->dentry))
		return mnt;

	if (recurse)
		mnt = copy_tree(old, old_path->dentry, CL_COPY_MNT_NS_FILE);
	else
		mnt = clone_mnt(old, old_path->dentry, 0);

	if (!IS_ERR(mnt))
		mnt->mnt.mnt_flags &= ~MNT_LOCKED;

	return mnt;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * do loopback mount.
 */
static int do_loopback(struct path *path, const char *old_name,
				int recurse)
{
<<<<<<< HEAD
	LIST_HEAD(umount_list);
	struct path old_path;
	struct mount *mnt = NULL, *old;
	int err = mount_is_safe(path);
	if (err)
		return err;
=======
	struct path old_path;
	struct mount *mnt = NULL, *parent;
	struct mountpoint *mp;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!old_name || !*old_name)
		return -EINVAL;
	err = kern_path(old_name, LOOKUP_FOLLOW|LOOKUP_AUTOMOUNT, &old_path);
	if (err)
		return err;

	err = -EINVAL;
<<<<<<< HEAD
	if (mnt_ns_loop(&old_path))
		goto out; 

	err = lock_mount(path);
	if (err)
		goto out;

	old = real_mount(old_path.mnt);

	err = -EINVAL;
	if (IS_MNT_UNBINDABLE(old))
		goto out2;

	if (!check_mnt(real_mount(path->mnt)) || !check_mnt(old))
		goto out2;

	if (recurse)
		mnt = copy_tree(old, old_path.dentry, 0);
	else
		mnt = clone_mnt(old, old_path.dentry, 0);

	if (IS_ERR(mnt)) {
		err = PTR_ERR(mnt);
		goto out;
	}

	err = graft_tree(mnt, path);
	if (err) {
		br_write_lock(&vfsmount_lock);
		umount_tree(mnt, 0, &umount_list);
		br_write_unlock(&vfsmount_lock);
	}
out2:
	unlock_mount(path);
	release_mounts(&umount_list);
=======
	if (mnt_ns_loop(old_path.dentry))
		goto out;

	mp = lock_mount(path);
	if (IS_ERR(mp)) {
		err = PTR_ERR(mp);
		goto out;
	}

	parent = real_mount(path->mnt);
	if (!check_mnt(parent))
		goto out2;

	mnt = __do_loopback(&old_path, recurse);
	if (IS_ERR(mnt)) {
		err = PTR_ERR(mnt);
		goto out2;
	}

	err = graft_tree(mnt, parent, mp);
	if (err) {
		lock_mount_hash();
		umount_tree(mnt, UMOUNT_SYNC);
		unlock_mount_hash();
	}
out2:
	unlock_mount(mp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	path_put(&old_path);
	return err;
}

<<<<<<< HEAD
static int change_mount_flags(struct vfsmount *mnt, int ms_flags)
{
	int error = 0;
	int readonly_request = 0;

	if (ms_flags & MS_RDONLY)
		readonly_request = 1;
	if (readonly_request == __mnt_is_readonly(mnt))
		return 0;

	if (readonly_request)
		error = mnt_make_readonly(real_mount(mnt));
	else
		__mnt_unmake_readonly(real_mount(mnt));
	return error;
=======
static struct file *open_detached_copy(struct path *path, bool recursive)
{
	struct user_namespace *user_ns = current->nsproxy->mnt_ns->user_ns;
	struct mnt_namespace *ns = alloc_mnt_ns(user_ns, true);
	struct mount *mnt, *p;
	struct file *file;

	if (IS_ERR(ns))
		return ERR_CAST(ns);

	namespace_lock();
	mnt = __do_loopback(path, recursive);
	if (IS_ERR(mnt)) {
		namespace_unlock();
		free_mnt_ns(ns);
		return ERR_CAST(mnt);
	}

	lock_mount_hash();
	for (p = mnt; p; p = next_mnt(p, mnt)) {
		mnt_add_to_ns(ns, p);
		ns->nr_mounts++;
	}
	ns->root = mnt;
	mntget(&mnt->mnt);
	unlock_mount_hash();
	namespace_unlock();

	mntput(path->mnt);
	path->mnt = &mnt->mnt;
	file = dentry_open(path, O_PATH, current_cred());
	if (IS_ERR(file))
		dissolve_on_fput(path->mnt);
	else
		file->f_mode |= FMODE_NEED_UNMOUNT;
	return file;
}

SYSCALL_DEFINE3(open_tree, int, dfd, const char __user *, filename, unsigned, flags)
{
	struct file *file;
	struct path path;
	int lookup_flags = LOOKUP_AUTOMOUNT | LOOKUP_FOLLOW;
	bool detached = flags & OPEN_TREE_CLONE;
	int error;
	int fd;

	BUILD_BUG_ON(OPEN_TREE_CLOEXEC != O_CLOEXEC);

	if (flags & ~(AT_EMPTY_PATH | AT_NO_AUTOMOUNT | AT_RECURSIVE |
		      AT_SYMLINK_NOFOLLOW | OPEN_TREE_CLONE |
		      OPEN_TREE_CLOEXEC))
		return -EINVAL;

	if ((flags & (AT_RECURSIVE | OPEN_TREE_CLONE)) == AT_RECURSIVE)
		return -EINVAL;

	if (flags & AT_NO_AUTOMOUNT)
		lookup_flags &= ~LOOKUP_AUTOMOUNT;
	if (flags & AT_SYMLINK_NOFOLLOW)
		lookup_flags &= ~LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
		lookup_flags |= LOOKUP_EMPTY;

	if (detached && !may_mount())
		return -EPERM;

	fd = get_unused_fd_flags(flags & O_CLOEXEC);
	if (fd < 0)
		return fd;

	error = user_path_at(dfd, filename, lookup_flags, &path);
	if (unlikely(error)) {
		file = ERR_PTR(error);
	} else {
		if (detached)
			file = open_detached_copy(&path, flags & AT_RECURSIVE);
		else
			file = dentry_open(&path, O_PATH, current_cred());
		path_put(&path);
	}
	if (IS_ERR(file)) {
		put_unused_fd(fd);
		return PTR_ERR(file);
	}
	fd_install(fd, file);
	return fd;
}

/*
 * Don't allow locked mount flags to be cleared.
 *
 * No locks need to be held here while testing the various MNT_LOCK
 * flags because those flags can never be cleared once they are set.
 */
static bool can_change_locked_flags(struct mount *mnt, unsigned int mnt_flags)
{
	unsigned int fl = mnt->mnt.mnt_flags;

	if ((fl & MNT_LOCK_READONLY) &&
	    !(mnt_flags & MNT_READONLY))
		return false;

	if ((fl & MNT_LOCK_NODEV) &&
	    !(mnt_flags & MNT_NODEV))
		return false;

	if ((fl & MNT_LOCK_NOSUID) &&
	    !(mnt_flags & MNT_NOSUID))
		return false;

	if ((fl & MNT_LOCK_NOEXEC) &&
	    !(mnt_flags & MNT_NOEXEC))
		return false;

	if ((fl & MNT_LOCK_ATIME) &&
	    ((fl & MNT_ATIME_MASK) != (mnt_flags & MNT_ATIME_MASK)))
		return false;

	return true;
}

static int change_mount_ro_state(struct mount *mnt, unsigned int mnt_flags)
{
	bool readonly_request = (mnt_flags & MNT_READONLY);

	if (readonly_request == __mnt_is_readonly(&mnt->mnt))
		return 0;

	if (readonly_request)
		return mnt_make_readonly(mnt);

	mnt->mnt.mnt_flags &= ~MNT_READONLY;
	return 0;
}

static void set_mount_attributes(struct mount *mnt, unsigned int mnt_flags)
{
	mnt_flags |= mnt->mnt.mnt_flags & ~MNT_USER_SETTABLE_MASK;
	mnt->mnt.mnt_flags = mnt_flags;
	touch_mnt_namespace(mnt->mnt_ns);
}

static void mnt_warn_timestamp_expiry(struct path *mountpoint, struct vfsmount *mnt)
{
	struct super_block *sb = mnt->mnt_sb;

	if (!__mnt_is_readonly(mnt) &&
	   (!(sb->s_iflags & SB_I_TS_EXPIRY_WARNED)) &&
	   (ktime_get_real_seconds() + TIME_UPTIME_SEC_MAX > sb->s_time_max)) {
		char *buf = (char *)__get_free_page(GFP_KERNEL);
		char *mntpath = buf ? d_path(mountpoint, buf, PAGE_SIZE) : ERR_PTR(-ENOMEM);

		pr_warn("%s filesystem being %s at %s supports timestamps until %ptTd (0x%llx)\n",
			sb->s_type->name,
			is_mounted(mnt) ? "remounted" : "mounted",
			mntpath, &sb->s_time_max,
			(unsigned long long)sb->s_time_max);

		free_page((unsigned long)buf);
		sb->s_iflags |= SB_I_TS_EXPIRY_WARNED;
	}
}

/*
 * Handle reconfiguration of the mountpoint only without alteration of the
 * superblock it refers to.  This is triggered by specifying MS_REMOUNT|MS_BIND
 * to mount(2).
 */
static int do_reconfigure_mnt(struct path *path, unsigned int mnt_flags)
{
	struct super_block *sb = path->mnt->mnt_sb;
	struct mount *mnt = real_mount(path->mnt);
	int ret;

	if (!check_mnt(mnt))
		return -EINVAL;

	if (!path_mounted(path))
		return -EINVAL;

	if (!can_change_locked_flags(mnt, mnt_flags))
		return -EPERM;

	/*
	 * We're only checking whether the superblock is read-only not
	 * changing it, so only take down_read(&sb->s_umount).
	 */
	down_read(&sb->s_umount);
	lock_mount_hash();
	ret = change_mount_ro_state(mnt, mnt_flags);
	if (ret == 0)
		set_mount_attributes(mnt, mnt_flags);
	unlock_mount_hash();
	up_read(&sb->s_umount);

	mnt_warn_timestamp_expiry(path, &mnt->mnt);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * change filesystem flags. dir should be a physical root of filesystem.
 * If you've mounted a non-root directory somewhere and want to do remount
 * on it - tough luck.
 */
<<<<<<< HEAD
static int do_remount(struct path *path, int flags, int mnt_flags,
		      void *data)
=======
static int do_remount(struct path *path, int ms_flags, int sb_flags,
		      int mnt_flags, void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	struct super_block *sb = path->mnt->mnt_sb;
	struct mount *mnt = real_mount(path->mnt);
<<<<<<< HEAD
	LIST_HEAD(umounts);

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
=======
	struct fs_context *fc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!check_mnt(mnt))
		return -EINVAL;

<<<<<<< HEAD
	if (path->dentry != path->mnt->mnt_root)
		return -EINVAL;

	err = security_sb_remount(sb, data);
	if (err)
		return err;

	down_write(&sb->s_umount);
	if (flags & MS_BIND)
		err = change_mount_flags(path->mnt, flags);
	else {
		err = do_remount_sb2(path->mnt, sb, flags, data, 0);
		down_write(&namespace_sem);
		br_write_lock(&vfsmount_lock);
		propagate_remount(mnt);
		br_write_unlock(&vfsmount_lock);
		up_write(&namespace_sem);
		release_mounts(&umounts);
	}
	if (!err) {
		br_write_lock(&vfsmount_lock);
		mnt_flags |= mnt->mnt.mnt_flags & ~MNT_USER_SETTABLE_MASK;
		mnt->mnt.mnt_flags = mnt_flags;
		br_write_unlock(&vfsmount_lock);
	}
	up_write(&sb->s_umount);
	if (!err) {
		br_write_lock(&vfsmount_lock);
		touch_mnt_namespace(mnt->mnt_ns);
		br_write_unlock(&vfsmount_lock);
	}
=======
	if (!path_mounted(path))
		return -EINVAL;

	if (!can_change_locked_flags(mnt, mnt_flags))
		return -EPERM;

	fc = fs_context_for_reconfigure(path->dentry, sb_flags, MS_RMT_MASK);
	if (IS_ERR(fc))
		return PTR_ERR(fc);

	/*
	 * Indicate to the filesystem that the remount request is coming
	 * from the legacy mount system call.
	 */
	fc->oldapi = true;

	err = parse_monolithic_mount_data(fc, data);
	if (!err) {
		down_write(&sb->s_umount);
		err = -EPERM;
		if (ns_capable(sb->s_user_ns, CAP_SYS_ADMIN)) {
			err = reconfigure_super(fc);
			if (!err) {
				lock_mount_hash();
				set_mount_attributes(mnt, mnt_flags);
				unlock_mount_hash();
			}
		}
		up_write(&sb->s_umount);
	}

	mnt_warn_timestamp_expiry(path, &mnt->mnt);

	put_fs_context(fc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static inline int tree_contains_unbindable(struct mount *mnt)
{
	struct mount *p;
	for (p = mnt; p; p = next_mnt(p, mnt)) {
		if (IS_MNT_UNBINDABLE(p))
			return 1;
	}
	return 0;
}

<<<<<<< HEAD
static int do_move_mount(struct path *path, const char *old_name)
{
	struct path old_path, parent_path;
	struct mount *p;
	struct mount *old;
	int err = 0;
	if (!ns_capable(real_mount(path->mnt)->mnt_ns->user_ns, CAP_SYS_ADMIN))
		return -EPERM;
	if (!old_name || !*old_name)
		return -EINVAL;
	err = kern_path(old_name, LOOKUP_FOLLOW, &old_path);
	if (err)
		return err;

	err = lock_mount(path);
	if (err < 0)
		goto out;

	old = real_mount(old_path.mnt);
	p = real_mount(path->mnt);

	err = -EINVAL;
	if (!check_mnt(p) || !check_mnt(old))
		goto out1;

	if (d_unlinked(path->dentry))
		goto out1;

	err = -EINVAL;
	if (old_path.dentry != old_path.mnt->mnt_root)
		goto out1;

	if (!mnt_has_parent(old))
		goto out1;

	if (S_ISDIR(path->dentry->d_inode->i_mode) !=
	      S_ISDIR(old_path.dentry->d_inode->i_mode))
		goto out1;
	/*
	 * Don't move a mount residing in a shared parent.
	 */
	if (IS_MNT_SHARED(old->mnt_parent))
		goto out1;
=======
/*
 * Check that there aren't references to earlier/same mount namespaces in the
 * specified subtree.  Such references can act as pins for mount namespaces
 * that aren't checked by the mount-cycle checking code, thereby allowing
 * cycles to be made.
 */
static bool check_for_nsfs_mounts(struct mount *subtree)
{
	struct mount *p;
	bool ret = false;

	lock_mount_hash();
	for (p = subtree; p; p = next_mnt(p, subtree))
		if (mnt_ns_loop(p->mnt.mnt_root))
			goto out;

	ret = true;
out:
	unlock_mount_hash();
	return ret;
}

static int do_set_group(struct path *from_path, struct path *to_path)
{
	struct mount *from, *to;
	int err;

	from = real_mount(from_path->mnt);
	to = real_mount(to_path->mnt);

	namespace_lock();

	err = -EINVAL;
	/* To and From must be mounted */
	if (!is_mounted(&from->mnt))
		goto out;
	if (!is_mounted(&to->mnt))
		goto out;

	err = -EPERM;
	/* We should be allowed to modify mount namespaces of both mounts */
	if (!ns_capable(from->mnt_ns->user_ns, CAP_SYS_ADMIN))
		goto out;
	if (!ns_capable(to->mnt_ns->user_ns, CAP_SYS_ADMIN))
		goto out;

	err = -EINVAL;
	/* To and From paths should be mount roots */
	if (!path_mounted(from_path))
		goto out;
	if (!path_mounted(to_path))
		goto out;

	/* Setting sharing groups is only allowed across same superblock */
	if (from->mnt.mnt_sb != to->mnt.mnt_sb)
		goto out;

	/* From mount root should be wider than To mount root */
	if (!is_subdir(to->mnt.mnt_root, from->mnt.mnt_root))
		goto out;

	/* From mount should not have locked children in place of To's root */
	if (has_locked_children(from, to->mnt.mnt_root))
		goto out;

	/* Setting sharing groups is only allowed on private mounts */
	if (IS_MNT_SHARED(to) || IS_MNT_SLAVE(to))
		goto out;

	/* From should not be private */
	if (!IS_MNT_SHARED(from) && !IS_MNT_SLAVE(from))
		goto out;

	if (IS_MNT_SLAVE(from)) {
		struct mount *m = from->mnt_master;

		list_add(&to->mnt_slave, &m->mnt_slave_list);
		to->mnt_master = m;
	}

	if (IS_MNT_SHARED(from)) {
		to->mnt_group_id = from->mnt_group_id;
		list_add(&to->mnt_share, &from->mnt_share);
		lock_mount_hash();
		set_mnt_shared(to);
		unlock_mount_hash();
	}

	err = 0;
out:
	namespace_unlock();
	return err;
}

/**
 * path_overmounted - check if path is overmounted
 * @path: path to check
 *
 * Check if path is overmounted, i.e., if there's a mount on top of
 * @path->mnt with @path->dentry as mountpoint.
 *
 * Context: This function expects namespace_lock() to be held.
 * Return: If path is overmounted true is returned, false if not.
 */
static inline bool path_overmounted(const struct path *path)
{
	rcu_read_lock();
	if (unlikely(__lookup_mnt(path->mnt, path->dentry))) {
		rcu_read_unlock();
		return true;
	}
	rcu_read_unlock();
	return false;
}

/**
 * can_move_mount_beneath - check that we can mount beneath the top mount
 * @from: mount to mount beneath
 * @to:   mount under which to mount
 * @mp:   mountpoint of @to
 *
 * - Make sure that @to->dentry is actually the root of a mount under
 *   which we can mount another mount.
 * - Make sure that nothing can be mounted beneath the caller's current
 *   root or the rootfs of the namespace.
 * - Make sure that the caller can unmount the topmost mount ensuring
 *   that the caller could reveal the underlying mountpoint.
 * - Ensure that nothing has been mounted on top of @from before we
 *   grabbed @namespace_sem to avoid creating pointless shadow mounts.
 * - Prevent mounting beneath a mount if the propagation relationship
 *   between the source mount, parent mount, and top mount would lead to
 *   nonsensical mount trees.
 *
 * Context: This function expects namespace_lock() to be held.
 * Return: On success 0, and on error a negative error code is returned.
 */
static int can_move_mount_beneath(const struct path *from,
				  const struct path *to,
				  const struct mountpoint *mp)
{
	struct mount *mnt_from = real_mount(from->mnt),
		     *mnt_to = real_mount(to->mnt),
		     *parent_mnt_to = mnt_to->mnt_parent;

	if (!mnt_has_parent(mnt_to))
		return -EINVAL;

	if (!path_mounted(to))
		return -EINVAL;

	if (IS_MNT_LOCKED(mnt_to))
		return -EINVAL;

	/* Avoid creating shadow mounts during mount propagation. */
	if (path_overmounted(from))
		return -EINVAL;

	/*
	 * Mounting beneath the rootfs only makes sense when the
	 * semantics of pivot_root(".", ".") are used.
	 */
	if (&mnt_to->mnt == current->fs->root.mnt)
		return -EINVAL;
	if (parent_mnt_to == current->nsproxy->mnt_ns->root)
		return -EINVAL;

	for (struct mount *p = mnt_from; mnt_has_parent(p); p = p->mnt_parent)
		if (p == mnt_to)
			return -EINVAL;

	/*
	 * If the parent mount propagates to the child mount this would
	 * mean mounting @mnt_from on @mnt_to->mnt_parent and then
	 * propagating a copy @c of @mnt_from on top of @mnt_to. This
	 * defeats the whole purpose of mounting beneath another mount.
	 */
	if (propagation_would_overmount(parent_mnt_to, mnt_to, mp))
		return -EINVAL;

	/*
	 * If @mnt_to->mnt_parent propagates to @mnt_from this would
	 * mean propagating a copy @c of @mnt_from on top of @mnt_from.
	 * Afterwards @mnt_from would be mounted on top of
	 * @mnt_to->mnt_parent and @mnt_to would be unmounted from
	 * @mnt->mnt_parent and remounted on @mnt_from. But since @c is
	 * already mounted on @mnt_from, @mnt_to would ultimately be
	 * remounted on top of @c. Afterwards, @mnt_from would be
	 * covered by a copy @c of @mnt_from and @c would be covered by
	 * @mnt_from itself. This defeats the whole purpose of mounting
	 * @mnt_from beneath @mnt_to.
	 */
	if (propagation_would_overmount(parent_mnt_to, mnt_from, mp))
		return -EINVAL;

	return 0;
}

static int do_move_mount(struct path *old_path, struct path *new_path,
			 bool beneath)
{
	struct mnt_namespace *ns;
	struct mount *p;
	struct mount *old;
	struct mount *parent;
	struct mountpoint *mp, *old_mp;
	int err;
	bool attached;
	enum mnt_tree_flags_t flags = 0;

	mp = do_lock_mount(new_path, beneath);
	if (IS_ERR(mp))
		return PTR_ERR(mp);

	old = real_mount(old_path->mnt);
	p = real_mount(new_path->mnt);
	parent = old->mnt_parent;
	attached = mnt_has_parent(old);
	if (attached)
		flags |= MNT_TREE_MOVE;
	old_mp = old->mnt_mp;
	ns = old->mnt_ns;

	err = -EINVAL;
	/* The mountpoint must be in our namespace. */
	if (!check_mnt(p))
		goto out;

	/* The thing moved must be mounted... */
	if (!is_mounted(&old->mnt))
		goto out;

	/* ... and either ours or the root of anon namespace */
	if (!(attached ? check_mnt(old) : is_anon_ns(ns)))
		goto out;

	if (old->mnt.mnt_flags & MNT_LOCKED)
		goto out;

	if (!path_mounted(old_path))
		goto out;

	if (d_is_dir(new_path->dentry) !=
	    d_is_dir(old_path->dentry))
		goto out;
	/*
	 * Don't move a mount residing in a shared parent.
	 */
	if (attached && IS_MNT_SHARED(parent))
		goto out;

	if (beneath) {
		err = can_move_mount_beneath(old_path, new_path, mp);
		if (err)
			goto out;

		err = -EINVAL;
		p = p->mnt_parent;
		flags |= MNT_TREE_BENEATH;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Don't move a mount tree containing unbindable mounts to a destination
	 * mount which is shared.
	 */
	if (IS_MNT_SHARED(p) && tree_contains_unbindable(old))
<<<<<<< HEAD
		goto out1;
	err = -ELOOP;
	for (; mnt_has_parent(p); p = p->mnt_parent)
		if (p == old)
			goto out1;

	err = attach_recursive_mnt(old, path, &parent_path);
	if (err)
		goto out1;
=======
		goto out;
	err = -ELOOP;
	if (!check_for_nsfs_mounts(old))
		goto out;
	for (; mnt_has_parent(p); p = p->mnt_parent)
		if (p == old)
			goto out;

	err = attach_recursive_mnt(old, real_mount(new_path->mnt), mp, flags);
	if (err)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* if the mount is moved, it should no longer be expire
	 * automatically */
	list_del_init(&old->mnt_expire);
<<<<<<< HEAD
out1:
	unlock_mount(path);
out:
	if (!err)
		path_put(&parent_path);
	path_put(&old_path);
	return err;
}

static struct vfsmount *fs_set_subtype(struct vfsmount *mnt, const char *fstype)
{
	int err;
	const char *subtype = strchr(fstype, '.');
	if (subtype) {
		subtype++;
		err = -EINVAL;
		if (!subtype[0])
			goto err;
	} else
		subtype = "";

	mnt->mnt_sb->s_subtype = kstrdup(subtype, GFP_KERNEL);
	err = -ENOMEM;
	if (!mnt->mnt_sb->s_subtype)
		goto err;
	return mnt;

 err:
	mntput(mnt);
	return ERR_PTR(err);
=======
	if (attached)
		put_mountpoint(old_mp);
out:
	unlock_mount(mp);
	if (!err) {
		if (attached)
			mntput_no_expire(parent);
		else
			free_mnt_ns(ns);
	}
	return err;
}

static int do_move_mount_old(struct path *path, const char *old_name)
{
	struct path old_path;
	int err;

	if (!old_name || !*old_name)
		return -EINVAL;

	err = kern_path(old_name, LOOKUP_FOLLOW, &old_path);
	if (err)
		return err;

	err = do_move_mount(&old_path, path, false);
	path_put(&old_path);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * add a mount into a namespace's mount tree
 */
<<<<<<< HEAD
static int do_add_mount(struct mount *newmnt, struct path *path, int mnt_flags)
{
	int err;

	mnt_flags &= ~MNT_INTERNAL_FLAGS;

	err = lock_mount(path);
	if (err)
		return err;

	err = -EINVAL;
	if (unlikely(!check_mnt(real_mount(path->mnt)))) {
		/* that's acceptable only for automounts done in private ns */
		if (!(mnt_flags & MNT_SHRINKABLE))
			goto unlock;
		/* ... and for those we'd better have mountpoint still alive */
		if (!real_mount(path->mnt)->mnt_ns)
			goto unlock;
	}

	/* Refuse the same filesystem on the same mount point */
	err = -EBUSY;
	if (path->mnt->mnt_sb == newmnt->mnt.mnt_sb &&
	    path->mnt->mnt_root == path->dentry)
		goto unlock;

	err = -EINVAL;
	if (S_ISLNK(newmnt->mnt.mnt_root->d_inode->i_mode))
		goto unlock;

	newmnt->mnt.mnt_flags = mnt_flags;
	err = graft_tree(newmnt, path);

unlock:
	unlock_mount(path);
	return err;
=======
static int do_add_mount(struct mount *newmnt, struct mountpoint *mp,
			const struct path *path, int mnt_flags)
{
	struct mount *parent = real_mount(path->mnt);

	mnt_flags &= ~MNT_INTERNAL_FLAGS;

	if (unlikely(!check_mnt(parent))) {
		/* that's acceptable only for automounts done in private ns */
		if (!(mnt_flags & MNT_SHRINKABLE))
			return -EINVAL;
		/* ... and for those we'd better have mountpoint still alive */
		if (!parent->mnt_ns)
			return -EINVAL;
	}

	/* Refuse the same filesystem on the same mount point */
	if (path->mnt->mnt_sb == newmnt->mnt.mnt_sb && path_mounted(path))
		return -EBUSY;

	if (d_is_symlink(newmnt->mnt.mnt_root))
		return -EINVAL;

	newmnt->mnt.mnt_flags = mnt_flags;
	return graft_tree(newmnt, parent, mp);
}

static bool mount_too_revealing(const struct super_block *sb, int *new_mnt_flags);

/*
 * Create a new mount using a superblock configuration and request it
 * be added to the namespace tree.
 */
static int do_new_mount_fc(struct fs_context *fc, struct path *mountpoint,
			   unsigned int mnt_flags)
{
	struct vfsmount *mnt;
	struct mountpoint *mp;
	struct super_block *sb = fc->root->d_sb;
	int error;

	error = security_sb_kern_mount(sb);
	if (!error && mount_too_revealing(sb, &mnt_flags))
		error = -EPERM;

	if (unlikely(error)) {
		fc_drop_locked(fc);
		return error;
	}

	up_write(&sb->s_umount);

	mnt = vfs_create_mount(fc);
	if (IS_ERR(mnt))
		return PTR_ERR(mnt);

	mnt_warn_timestamp_expiry(mountpoint, mnt);

	mp = lock_mount(mountpoint);
	if (IS_ERR(mp)) {
		mntput(mnt);
		return PTR_ERR(mp);
	}
	error = do_add_mount(real_mount(mnt), mp, mountpoint, mnt_flags);
	unlock_mount(mp);
	if (error < 0)
		mntput(mnt);
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * create a new mount for userspace and request it to be added into the
 * namespace's tree
 */
<<<<<<< HEAD
static int do_new_mount(struct path *path, const char *fstype, int flags,
			int mnt_flags, const char *name, void *data)
{
	struct file_system_type *type;
	struct user_namespace *user_ns;
	struct vfsmount *mnt;
	int err;
=======
static int do_new_mount(struct path *path, const char *fstype, int sb_flags,
			int mnt_flags, const char *name, void *data)
{
	struct file_system_type *type;
	struct fs_context *fc;
	const char *subtype = NULL;
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!fstype)
		return -EINVAL;

<<<<<<< HEAD
	/* we need capabilities... */
	user_ns = real_mount(path->mnt)->mnt_ns->user_ns;
	if (!ns_capable(user_ns, CAP_SYS_ADMIN))
		return -EPERM;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	type = get_fs_type(fstype);
	if (!type)
		return -ENODEV;

<<<<<<< HEAD
	if (user_ns != &init_user_ns) {
		if (!(type->fs_flags & FS_USERNS_MOUNT)) {
			put_filesystem(type);
			return -EPERM;
		}
		/* Only in special cases allow devices from mounts
		 * created outside the initial user namespace.
		 */
		if (!(type->fs_flags & FS_USERNS_DEV_MOUNT)) {
			flags |= MS_NODEV;
			mnt_flags |= MNT_NODEV;
		}
	}

	mnt = vfs_kern_mount(type, flags, name, data);
	if (!IS_ERR(mnt) && (type->fs_flags & FS_HAS_SUBTYPE) &&
	    !mnt->mnt_sb->s_subtype)
		mnt = fs_set_subtype(mnt, fstype);

	put_filesystem(type);
	if (IS_ERR(mnt))
		return PTR_ERR(mnt);

	err = do_add_mount(real_mount(mnt), path, mnt_flags);
	if (err)
		mntput(mnt);
	return err;
}

int finish_automount(struct vfsmount *m, struct path *path)
{
	struct mount *mnt = real_mount(m);
	int err;
=======
	if (type->fs_flags & FS_HAS_SUBTYPE) {
		subtype = strchr(fstype, '.');
		if (subtype) {
			subtype++;
			if (!*subtype) {
				put_filesystem(type);
				return -EINVAL;
			}
		}
	}

	fc = fs_context_for_mount(type, sb_flags);
	put_filesystem(type);
	if (IS_ERR(fc))
		return PTR_ERR(fc);

	/*
	 * Indicate to the filesystem that the mount request is coming
	 * from the legacy mount system call.
	 */
	fc->oldapi = true;

	if (subtype)
		err = vfs_parse_fs_string(fc, "subtype",
					  subtype, strlen(subtype));
	if (!err && name)
		err = vfs_parse_fs_string(fc, "source", name, strlen(name));
	if (!err)
		err = parse_monolithic_mount_data(fc, data);
	if (!err && !mount_capable(fc))
		err = -EPERM;
	if (!err)
		err = vfs_get_tree(fc);
	if (!err)
		err = do_new_mount_fc(fc, path, mnt_flags);

	put_fs_context(fc);
	return err;
}

int finish_automount(struct vfsmount *m, const struct path *path)
{
	struct dentry *dentry = path->dentry;
	struct mountpoint *mp;
	struct mount *mnt;
	int err;

	if (!m)
		return 0;
	if (IS_ERR(m))
		return PTR_ERR(m);

	mnt = real_mount(m);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* The new mount record should have at least 2 refs to prevent it being
	 * expired before we get a chance to add it
	 */
	BUG_ON(mnt_get_count(mnt) < 2);

	if (m->mnt_sb == path->mnt->mnt_sb &&
<<<<<<< HEAD
	    m->mnt_root == path->dentry) {
		err = -ELOOP;
		goto fail;
	}

	err = do_add_mount(mnt, path, path->mnt->mnt_flags | MNT_SHRINKABLE);
	if (!err)
		return 0;
fail:
	/* remove m from any expiration list it may be on */
	if (!list_empty(&mnt->mnt_expire)) {
		down_write(&namespace_sem);
		br_write_lock(&vfsmount_lock);
		list_del_init(&mnt->mnt_expire);
		br_write_unlock(&vfsmount_lock);
		up_write(&namespace_sem);
=======
	    m->mnt_root == dentry) {
		err = -ELOOP;
		goto discard;
	}

	/*
	 * we don't want to use lock_mount() - in this case finding something
	 * that overmounts our mountpoint to be means "quitely drop what we've
	 * got", not "try to mount it on top".
	 */
	inode_lock(dentry->d_inode);
	namespace_lock();
	if (unlikely(cant_mount(dentry))) {
		err = -ENOENT;
		goto discard_locked;
	}
	if (path_overmounted(path)) {
		err = 0;
		goto discard_locked;
	}
	mp = get_mountpoint(dentry);
	if (IS_ERR(mp)) {
		err = PTR_ERR(mp);
		goto discard_locked;
	}

	err = do_add_mount(mnt, mp, path, path->mnt->mnt_flags | MNT_SHRINKABLE);
	unlock_mount(mp);
	if (unlikely(err))
		goto discard;
	mntput(m);
	return 0;

discard_locked:
	namespace_unlock();
	inode_unlock(dentry->d_inode);
discard:
	/* remove m from any expiration list it may be on */
	if (!list_empty(&mnt->mnt_expire)) {
		namespace_lock();
		list_del_init(&mnt->mnt_expire);
		namespace_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mntput(m);
	mntput(m);
	return err;
}

/**
 * mnt_set_expiry - Put a mount on an expiration list
 * @mnt: The mount to list.
 * @expiry_list: The list to add the mount to.
 */
void mnt_set_expiry(struct vfsmount *mnt, struct list_head *expiry_list)
{
<<<<<<< HEAD
	down_write(&namespace_sem);
	br_write_lock(&vfsmount_lock);

	list_add_tail(&real_mount(mnt)->mnt_expire, expiry_list);

	br_write_unlock(&vfsmount_lock);
	up_write(&namespace_sem);
=======
	namespace_lock();

	list_add_tail(&real_mount(mnt)->mnt_expire, expiry_list);

	namespace_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(mnt_set_expiry);

/*
 * process a list of expirable mountpoints with the intent of discarding any
 * mountpoints that aren't in use and haven't been touched since last we came
 * here
 */
void mark_mounts_for_expiry(struct list_head *mounts)
{
	struct mount *mnt, *next;
	LIST_HEAD(graveyard);
<<<<<<< HEAD
	LIST_HEAD(umounts);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (list_empty(mounts))
		return;

<<<<<<< HEAD
	down_write(&namespace_sem);
	br_write_lock(&vfsmount_lock);
=======
	namespace_lock();
	lock_mount_hash();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* extract from the expiration list every vfsmount that matches the
	 * following criteria:
	 * - only referenced by its parent vfsmount
	 * - still marked for expiry (marked on the last call here; marks are
	 *   cleared by mntput())
	 */
	list_for_each_entry_safe(mnt, next, mounts, mnt_expire) {
		if (!xchg(&mnt->mnt_expiry_mark, 1) ||
			propagate_mount_busy(mnt, 1))
			continue;
		list_move(&mnt->mnt_expire, &graveyard);
	}
	while (!list_empty(&graveyard)) {
		mnt = list_first_entry(&graveyard, struct mount, mnt_expire);
		touch_mnt_namespace(mnt->mnt_ns);
<<<<<<< HEAD
		umount_tree(mnt, 1, &umounts);
	}
	br_write_unlock(&vfsmount_lock);
	up_write(&namespace_sem);

	release_mounts(&umounts);
=======
		umount_tree(mnt, UMOUNT_PROPAGATE|UMOUNT_SYNC);
	}
	unlock_mount_hash();
	namespace_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL_GPL(mark_mounts_for_expiry);

/*
 * Ripoff of 'select_parent()'
 *
 * search the list of submounts for a given mountpoint, and move any
 * shrinkable submounts to the 'graveyard' list.
 */
static int select_submounts(struct mount *parent, struct list_head *graveyard)
{
	struct mount *this_parent = parent;
	struct list_head *next;
	int found = 0;

repeat:
	next = this_parent->mnt_mounts.next;
resume:
	while (next != &this_parent->mnt_mounts) {
		struct list_head *tmp = next;
		struct mount *mnt = list_entry(tmp, struct mount, mnt_child);

		next = tmp->next;
		if (!(mnt->mnt.mnt_flags & MNT_SHRINKABLE))
			continue;
		/*
		 * Descend a level if the d_mounts list is non-empty.
		 */
		if (!list_empty(&mnt->mnt_mounts)) {
			this_parent = mnt;
			goto repeat;
		}

		if (!propagate_mount_busy(mnt, 1)) {
			list_move_tail(&mnt->mnt_expire, graveyard);
			found++;
		}
	}
	/*
	 * All done at this level ... ascend and resume the search
	 */
	if (this_parent != parent) {
		next = this_parent->mnt_child.next;
		this_parent = this_parent->mnt_parent;
		goto resume;
	}
	return found;
}

/*
 * process a list of expirable mountpoints with the intent of discarding any
 * submounts of a specific parent mountpoint
 *
<<<<<<< HEAD
 * vfsmount_lock must be held for write
 */
static void shrink_submounts(struct mount *mnt, struct list_head *umounts)
=======
 * mount_lock must be held for write
 */
static void shrink_submounts(struct mount *mnt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	LIST_HEAD(graveyard);
	struct mount *m;

	/* extract submounts of 'mountpoint' from the expiration list */
	while (select_submounts(mnt, &graveyard)) {
		while (!list_empty(&graveyard)) {
			m = list_first_entry(&graveyard, struct mount,
						mnt_expire);
			touch_mnt_namespace(m->mnt_ns);
<<<<<<< HEAD
			umount_tree(m, 1, umounts);
=======
			umount_tree(m, UMOUNT_PROPAGATE|UMOUNT_SYNC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

<<<<<<< HEAD
/*
 * Some copy_from_user() implementations do not return the exact number of
 * bytes remaining to copy on a fault.  But copy_mount_options() requires that.
 * Note that this function differs from copy_from_user() in that it will oops
 * on bad values of `to', rather than returning a short copy.
 */
static long exact_copy_from_user(void *to, const void __user * from,
				 unsigned long n)
{
	char *t = to;
	const char __user *f = from;
	char c;

	if (!access_ok(VERIFY_READ, from, n))
		return n;

	while (n) {
		if (__get_user(c, f)) {
			memset(t, 0, n);
			break;
		}
		*t++ = c;
		f++;
		n--;
	}
	return n;
}

int copy_mount_options(const void __user * data, unsigned long *where)
{
	int i;
	unsigned long page;
	unsigned long size;

	*where = 0;
	if (!data)
		return 0;

	if (!(page = __get_free_page(GFP_KERNEL)))
		return -ENOMEM;

	/* We only care that *some* data at the address the user
	 * gave us is valid.  Just in case, we'll zero
	 * the remainder of the page.
	 */
	/* copy_from_user cannot cross TASK_SIZE ! */
	size = TASK_SIZE - (unsigned long)data;
	if (size > PAGE_SIZE)
		size = PAGE_SIZE;

	i = size - exact_copy_from_user((void *)page, data, size);
	if (!i) {
		free_page(page);
		return -EFAULT;
	}
	if (i != PAGE_SIZE)
		memset((char *)page + i, 0, PAGE_SIZE - i);
	*where = page;
	return 0;
}

int copy_mount_string(const void __user *data, char **where)
{
	char *tmp;

	if (!data) {
		*where = NULL;
		return 0;
	}

	tmp = strndup_user(data, PAGE_SIZE);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);

	*where = tmp;
	return 0;
=======
static void *copy_mount_options(const void __user * data)
{
	char *copy;
	unsigned left, offset;

	if (!data)
		return NULL;

	copy = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!copy)
		return ERR_PTR(-ENOMEM);

	left = copy_from_user(copy, data, PAGE_SIZE);

	/*
	 * Not all architectures have an exact copy_from_user(). Resort to
	 * byte at a time.
	 */
	offset = PAGE_SIZE - left;
	while (left) {
		char c;
		if (get_user(c, (const char __user *)data + offset))
			break;
		copy[offset] = c;
		left--;
		offset++;
	}

	if (left == PAGE_SIZE) {
		kfree(copy);
		return ERR_PTR(-EFAULT);
	}

	return copy;
}

static char *copy_mount_string(const void __user *data)
{
	return data ? strndup_user(data, PATH_MAX) : NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Flags is a 32-bit value that allows up to 31 non-fs dependent flags to
 * be given to the mount() call (ie: read-only, no-dev, no-suid etc).
 *
 * data is a (void *) that can point to any structure up to
 * PAGE_SIZE-1 bytes, which can contain arbitrary fs-dependent
 * information (or be NULL).
 *
 * Pre-0.97 versions of mount() didn't have a flags word.
 * When the flags word was introduced its top half was required
 * to have the magic value 0xC0ED, and this remained so until 2.4.0-test9.
 * Therefore, if this magic number is present, it carries no information
 * and must be discarded.
 */
<<<<<<< HEAD
long do_mount(const char *dev_name, const char *dir_name,
		const char *type_page, unsigned long flags, void *data_page)
{
	struct path path;
	int retval = 0;
	int mnt_flags = 0;
=======
int path_mount(const char *dev_name, struct path *path,
		const char *type_page, unsigned long flags, void *data_page)
{
	unsigned int mnt_flags = 0, sb_flags;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Discard magic */
	if ((flags & MS_MGC_MSK) == MS_MGC_VAL)
		flags &= ~MS_MGC_MSK;

	/* Basic sanity checks */
<<<<<<< HEAD

	if (!dir_name || !*dir_name || !memchr(dir_name, 0, PAGE_SIZE))
		return -EINVAL;

	if (data_page)
		((char *)data_page)[PAGE_SIZE - 1] = 0;

	/* ... and get the mountpoint */
	retval = kern_path(dir_name, LOOKUP_FOLLOW, &path);
	if (retval)
		return retval;

	retval = security_sb_mount(dev_name, &path,
				   type_page, flags, data_page);
	if (retval)
		goto dput_out;
=======
	if (data_page)
		((char *)data_page)[PAGE_SIZE - 1] = 0;

	if (flags & MS_NOUSER)
		return -EINVAL;

	ret = security_sb_mount(dev_name, path, type_page, flags, data_page);
	if (ret)
		return ret;
	if (!may_mount())
		return -EPERM;
	if (flags & SB_MANDLOCK)
		warn_mandlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Default to relatime unless overriden */
	if (!(flags & MS_NOATIME))
		mnt_flags |= MNT_RELATIME;

	/* Separate the per-mountpoint flags */
	if (flags & MS_NOSUID)
		mnt_flags |= MNT_NOSUID;
	if (flags & MS_NODEV)
		mnt_flags |= MNT_NODEV;
	if (flags & MS_NOEXEC)
		mnt_flags |= MNT_NOEXEC;
	if (flags & MS_NOATIME)
		mnt_flags |= MNT_NOATIME;
	if (flags & MS_NODIRATIME)
		mnt_flags |= MNT_NODIRATIME;
	if (flags & MS_STRICTATIME)
		mnt_flags &= ~(MNT_RELATIME | MNT_NOATIME);
	if (flags & MS_RDONLY)
		mnt_flags |= MNT_READONLY;
<<<<<<< HEAD

	flags &= ~(MS_NOSUID | MS_NOEXEC | MS_NODEV | MS_ACTIVE | MS_BORN |
		   MS_NOATIME | MS_NODIRATIME | MS_RELATIME| MS_KERNMOUNT |
		   MS_STRICTATIME);

	if (flags & MS_REMOUNT)
		retval = do_remount(&path, flags & ~MS_REMOUNT, mnt_flags,
				    data_page);
	else if (flags & MS_BIND)
		retval = do_loopback(&path, dev_name, flags & MS_REC);
	else if (flags & (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE))
		retval = do_change_type(&path, flags);
	else if (flags & MS_MOVE)
		retval = do_move_mount(&path, dev_name);
	else
		retval = do_new_mount(&path, type_page, flags, mnt_flags,
				      dev_name, data_page);
dput_out:
	path_put(&path);
	return retval;
=======
	if (flags & MS_NOSYMFOLLOW)
		mnt_flags |= MNT_NOSYMFOLLOW;

	/* The default atime for remount is preservation */
	if ((flags & MS_REMOUNT) &&
	    ((flags & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME |
		       MS_STRICTATIME)) == 0)) {
		mnt_flags &= ~MNT_ATIME_MASK;
		mnt_flags |= path->mnt->mnt_flags & MNT_ATIME_MASK;
	}

	sb_flags = flags & (SB_RDONLY |
			    SB_SYNCHRONOUS |
			    SB_MANDLOCK |
			    SB_DIRSYNC |
			    SB_SILENT |
			    SB_POSIXACL |
			    SB_LAZYTIME |
			    SB_I_VERSION);

	if ((flags & (MS_REMOUNT | MS_BIND)) == (MS_REMOUNT | MS_BIND))
		return do_reconfigure_mnt(path, mnt_flags);
	if (flags & MS_REMOUNT)
		return do_remount(path, flags, sb_flags, mnt_flags, data_page);
	if (flags & MS_BIND)
		return do_loopback(path, dev_name, flags & MS_REC);
	if (flags & (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE))
		return do_change_type(path, flags);
	if (flags & MS_MOVE)
		return do_move_mount_old(path, dev_name);

	return do_new_mount(path, type_page, sb_flags, mnt_flags, dev_name,
			    data_page);
}

long do_mount(const char *dev_name, const char __user *dir_name,
		const char *type_page, unsigned long flags, void *data_page)
{
	struct path path;
	int ret;

	ret = user_path_at(AT_FDCWD, dir_name, LOOKUP_FOLLOW, &path);
	if (ret)
		return ret;
	ret = path_mount(dev_name, &path, type_page, flags, data_page);
	path_put(&path);
	return ret;
}

static struct ucounts *inc_mnt_namespaces(struct user_namespace *ns)
{
	return inc_ucount(ns, current_euid(), UCOUNT_MNT_NAMESPACES);
}

static void dec_mnt_namespaces(struct ucounts *ucounts)
{
	dec_ucount(ucounts, UCOUNT_MNT_NAMESPACES);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void free_mnt_ns(struct mnt_namespace *ns)
{
<<<<<<< HEAD
	proc_free_inum(ns->proc_inum);
=======
	if (!is_anon_ns(ns))
		ns_free_inum(&ns->ns);
	dec_mnt_namespaces(ns->ucounts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	put_user_ns(ns->user_ns);
	kfree(ns);
}

/*
 * Assign a sequence number so we can detect when we attempt to bind
 * mount a reference to an older mount namespace into the current
 * mount namespace, preventing reference counting loops.  A 64bit
 * number incrementing at 10Ghz will take 12,427 years to wrap which
 * is effectively never, so we can ignore the possibility.
 */
static atomic64_t mnt_ns_seq = ATOMIC64_INIT(1);

<<<<<<< HEAD
static struct mnt_namespace *alloc_mnt_ns(struct user_namespace *user_ns)
{
	struct mnt_namespace *new_ns;
	int ret;

	new_ns = kmalloc(sizeof(struct mnt_namespace), GFP_KERNEL);
	if (!new_ns)
		return ERR_PTR(-ENOMEM);
	ret = proc_alloc_inum(&new_ns->proc_inum);
	if (ret) {
		kfree(new_ns);
		return ERR_PTR(ret);
	}
	new_ns->seq = atomic64_add_return(1, &mnt_ns_seq);
	atomic_set(&new_ns->count, 1);
	new_ns->root = NULL;
	INIT_LIST_HEAD(&new_ns->list);
	init_waitqueue_head(&new_ns->poll);
	new_ns->event = 0;
	new_ns->user_ns = get_user_ns(user_ns);
	return new_ns;
}

/*
 * Allocate a new namespace structure and populate it with contents
 * copied from the namespace of the passed in task structure.
 */
static struct mnt_namespace *dup_mnt_ns(struct mnt_namespace *mnt_ns,
		struct user_namespace *user_ns, struct fs_struct *fs)
=======
static struct mnt_namespace *alloc_mnt_ns(struct user_namespace *user_ns, bool anon)
{
	struct mnt_namespace *new_ns;
	struct ucounts *ucounts;
	int ret;

	ucounts = inc_mnt_namespaces(user_ns);
	if (!ucounts)
		return ERR_PTR(-ENOSPC);

	new_ns = kzalloc(sizeof(struct mnt_namespace), GFP_KERNEL_ACCOUNT);
	if (!new_ns) {
		dec_mnt_namespaces(ucounts);
		return ERR_PTR(-ENOMEM);
	}
	if (!anon) {
		ret = ns_alloc_inum(&new_ns->ns);
		if (ret) {
			kfree(new_ns);
			dec_mnt_namespaces(ucounts);
			return ERR_PTR(ret);
		}
	}
	new_ns->ns.ops = &mntns_operations;
	if (!anon)
		new_ns->seq = atomic64_add_return(1, &mnt_ns_seq);
	refcount_set(&new_ns->ns.count, 1);
	new_ns->mounts = RB_ROOT;
	init_waitqueue_head(&new_ns->poll);
	new_ns->user_ns = get_user_ns(user_ns);
	new_ns->ucounts = ucounts;
	return new_ns;
}

__latent_entropy
struct mnt_namespace *copy_mnt_ns(unsigned long flags, struct mnt_namespace *ns,
		struct user_namespace *user_ns, struct fs_struct *new_fs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mnt_namespace *new_ns;
	struct vfsmount *rootmnt = NULL, *pwdmnt = NULL;
	struct mount *p, *q;
<<<<<<< HEAD
	struct mount *old = mnt_ns->root;
	struct mount *new;
	int copy_flags;

	new_ns = alloc_mnt_ns(user_ns);
	if (IS_ERR(new_ns))
		return new_ns;

	down_write(&namespace_sem);
	/* First pass: copy the tree topology */
	copy_flags = CL_COPY_ALL | CL_EXPIRE;
	if (user_ns != mnt_ns->user_ns)
		copy_flags |= CL_SHARED_TO_SLAVE;
	new = copy_tree(old, old->mnt.mnt_root, copy_flags);
	if (IS_ERR(new)) {
		up_write(&namespace_sem);
		free_mnt_ns(new_ns);
		return ERR_CAST(new);
	}
	new_ns->root = new;
	br_write_lock(&vfsmount_lock);
	list_add_tail(&new_ns->list, &new->mnt_list);
	br_write_unlock(&vfsmount_lock);
=======
	struct mount *old;
	struct mount *new;
	int copy_flags;

	BUG_ON(!ns);

	if (likely(!(flags & CLONE_NEWNS))) {
		get_mnt_ns(ns);
		return ns;
	}

	old = ns->root;

	new_ns = alloc_mnt_ns(user_ns, false);
	if (IS_ERR(new_ns))
		return new_ns;

	namespace_lock();
	/* First pass: copy the tree topology */
	copy_flags = CL_COPY_UNBINDABLE | CL_EXPIRE;
	if (user_ns != ns->user_ns)
		copy_flags |= CL_SHARED_TO_SLAVE;
	new = copy_tree(old, old->mnt.mnt_root, copy_flags);
	if (IS_ERR(new)) {
		namespace_unlock();
		free_mnt_ns(new_ns);
		return ERR_CAST(new);
	}
	if (user_ns != ns->user_ns) {
		lock_mount_hash();
		lock_mnt_tree(new);
		unlock_mount_hash();
	}
	new_ns->root = new;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Second pass: switch the tsk->fs->* elements and mark new vfsmounts
	 * as belonging to new namespace.  We have already acquired a private
	 * fs_struct, so tsk->fs->lock is not needed.
	 */
	p = old;
	q = new;
	while (p) {
<<<<<<< HEAD
		q->mnt_ns = new_ns;
		if (fs) {
			if (&p->mnt == fs->root.mnt) {
				fs->root.mnt = mntget(&q->mnt);
				rootmnt = &p->mnt;
			}
			if (&p->mnt == fs->pwd.mnt) {
				fs->pwd.mnt = mntget(&q->mnt);
=======
		mnt_add_to_ns(new_ns, q);
		new_ns->nr_mounts++;
		if (new_fs) {
			if (&p->mnt == new_fs->root.mnt) {
				new_fs->root.mnt = mntget(&q->mnt);
				rootmnt = &p->mnt;
			}
			if (&p->mnt == new_fs->pwd.mnt) {
				new_fs->pwd.mnt = mntget(&q->mnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				pwdmnt = &p->mnt;
			}
		}
		p = next_mnt(p, old);
		q = next_mnt(q, new);
<<<<<<< HEAD
	}
	up_write(&namespace_sem);
=======
		if (!q)
			break;
		// an mntns binding we'd skipped?
		while (p->mnt.mnt_root != q->mnt.mnt_root)
			p = next_mnt(skip_mnt_tree(p), old);
	}
	namespace_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (rootmnt)
		mntput(rootmnt);
	if (pwdmnt)
		mntput(pwdmnt);

	return new_ns;
}

<<<<<<< HEAD
struct mnt_namespace *copy_mnt_ns(unsigned long flags, struct mnt_namespace *ns,
		struct user_namespace *user_ns, struct fs_struct *new_fs)
{
	struct mnt_namespace *new_ns;

	BUG_ON(!ns);
	get_mnt_ns(ns);

	if (!(flags & CLONE_NEWNS))
		return ns;

	new_ns = dup_mnt_ns(ns, user_ns, new_fs);

	put_mnt_ns(ns);
	return new_ns;
}

/**
 * create_mnt_ns - creates a private namespace and adds a root filesystem
 * @mnt: pointer to the new root filesystem mountpoint
 */
static struct mnt_namespace *create_mnt_ns(struct vfsmount *m)
{
	struct mnt_namespace *new_ns = alloc_mnt_ns(&init_user_ns);
	if (!IS_ERR(new_ns)) {
		struct mount *mnt = real_mount(m);
		mnt->mnt_ns = new_ns;
		new_ns->root = mnt;
		list_add(&new_ns->list, &mnt->mnt_list);
	} else {
		mntput(m);
	}
	return new_ns;
}

struct dentry *mount_subtree(struct vfsmount *mnt, const char *name)
{
=======
struct dentry *mount_subtree(struct vfsmount *m, const char *name)
{
	struct mount *mnt = real_mount(m);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mnt_namespace *ns;
	struct super_block *s;
	struct path path;
	int err;

<<<<<<< HEAD
	ns = create_mnt_ns(mnt);
	if (IS_ERR(ns))
		return ERR_CAST(ns);

	err = vfs_path_lookup(mnt->mnt_root, mnt,
=======
	ns = alloc_mnt_ns(&init_user_ns, true);
	if (IS_ERR(ns)) {
		mntput(m);
		return ERR_CAST(ns);
	}
	ns->root = mnt;
	ns->nr_mounts++;
	mnt_add_to_ns(ns, mnt);

	err = vfs_path_lookup(m->mnt_root, m,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			name, LOOKUP_FOLLOW|LOOKUP_AUTOMOUNT, &path);

	put_mnt_ns(ns);

	if (err)
		return ERR_PTR(err);

	/* trade a vfsmount reference for active sb one */
	s = path.mnt->mnt_sb;
	atomic_inc(&s->s_active);
	mntput(path.mnt);
	/* lock the sucker */
	down_write(&s->s_umount);
	/* ... and return the root of (sub)tree on it */
	return path.dentry;
}
EXPORT_SYMBOL(mount_subtree);

SYSCALL_DEFINE5(mount, char __user *, dev_name, char __user *, dir_name,
		char __user *, type, unsigned long, flags, void __user *, data)
{
	int ret;
	char *kernel_type;
<<<<<<< HEAD
	struct filename *kernel_dir;
	char *kernel_dev;
	unsigned long data_page;

	ret = copy_mount_string(type, &kernel_type);
	if (ret < 0)
		goto out_type;

	kernel_dir = getname(dir_name);
	if (IS_ERR(kernel_dir)) {
		ret = PTR_ERR(kernel_dir);
		goto out_dir;
	}

	ret = copy_mount_string(dev_name, &kernel_dev);
	if (ret < 0)
		goto out_dev;

	ret = copy_mount_options(data, &data_page);
	if (ret < 0)
		goto out_data;

	ret = do_mount(kernel_dev, kernel_dir->name, kernel_type, flags,
		(void *) data_page);

	free_page(data_page);
out_data:
	kfree(kernel_dev);
out_dev:
	putname(kernel_dir);
out_dir:
=======
	char *kernel_dev;
	void *options;

	kernel_type = copy_mount_string(type);
	ret = PTR_ERR(kernel_type);
	if (IS_ERR(kernel_type))
		goto out_type;

	kernel_dev = copy_mount_string(dev_name);
	ret = PTR_ERR(kernel_dev);
	if (IS_ERR(kernel_dev))
		goto out_dev;

	options = copy_mount_options(data);
	ret = PTR_ERR(options);
	if (IS_ERR(options))
		goto out_data;

	ret = do_mount(kernel_dev, dir_name, kernel_type, flags, options);

	kfree(options);
out_data:
	kfree(kernel_dev);
out_dev:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(kernel_type);
out_type:
	return ret;
}

<<<<<<< HEAD
/*
 * Return true if path is reachable from root
 *
 * namespace_sem or vfsmount_lock is held
=======
#define FSMOUNT_VALID_FLAGS                                                    \
	(MOUNT_ATTR_RDONLY | MOUNT_ATTR_NOSUID | MOUNT_ATTR_NODEV |            \
	 MOUNT_ATTR_NOEXEC | MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME |       \
	 MOUNT_ATTR_NOSYMFOLLOW)

#define MOUNT_SETATTR_VALID_FLAGS (FSMOUNT_VALID_FLAGS | MOUNT_ATTR_IDMAP)

#define MOUNT_SETATTR_PROPAGATION_FLAGS \
	(MS_UNBINDABLE | MS_PRIVATE | MS_SLAVE | MS_SHARED)

static unsigned int attr_flags_to_mnt_flags(u64 attr_flags)
{
	unsigned int mnt_flags = 0;

	if (attr_flags & MOUNT_ATTR_RDONLY)
		mnt_flags |= MNT_READONLY;
	if (attr_flags & MOUNT_ATTR_NOSUID)
		mnt_flags |= MNT_NOSUID;
	if (attr_flags & MOUNT_ATTR_NODEV)
		mnt_flags |= MNT_NODEV;
	if (attr_flags & MOUNT_ATTR_NOEXEC)
		mnt_flags |= MNT_NOEXEC;
	if (attr_flags & MOUNT_ATTR_NODIRATIME)
		mnt_flags |= MNT_NODIRATIME;
	if (attr_flags & MOUNT_ATTR_NOSYMFOLLOW)
		mnt_flags |= MNT_NOSYMFOLLOW;

	return mnt_flags;
}

/*
 * Create a kernel mount representation for a new, prepared superblock
 * (specified by fs_fd) and attach to an open_tree-like file descriptor.
 */
SYSCALL_DEFINE3(fsmount, int, fs_fd, unsigned int, flags,
		unsigned int, attr_flags)
{
	struct mnt_namespace *ns;
	struct fs_context *fc;
	struct file *file;
	struct path newmount;
	struct mount *mnt;
	struct fd f;
	unsigned int mnt_flags = 0;
	long ret;

	if (!may_mount())
		return -EPERM;

	if ((flags & ~(FSMOUNT_CLOEXEC)) != 0)
		return -EINVAL;

	if (attr_flags & ~FSMOUNT_VALID_FLAGS)
		return -EINVAL;

	mnt_flags = attr_flags_to_mnt_flags(attr_flags);

	switch (attr_flags & MOUNT_ATTR__ATIME) {
	case MOUNT_ATTR_STRICTATIME:
		break;
	case MOUNT_ATTR_NOATIME:
		mnt_flags |= MNT_NOATIME;
		break;
	case MOUNT_ATTR_RELATIME:
		mnt_flags |= MNT_RELATIME;
		break;
	default:
		return -EINVAL;
	}

	f = fdget(fs_fd);
	if (!f.file)
		return -EBADF;

	ret = -EINVAL;
	if (f.file->f_op != &fscontext_fops)
		goto err_fsfd;

	fc = f.file->private_data;

	ret = mutex_lock_interruptible(&fc->uapi_mutex);
	if (ret < 0)
		goto err_fsfd;

	/* There must be a valid superblock or we can't mount it */
	ret = -EINVAL;
	if (!fc->root)
		goto err_unlock;

	ret = -EPERM;
	if (mount_too_revealing(fc->root->d_sb, &mnt_flags)) {
		pr_warn("VFS: Mount too revealing\n");
		goto err_unlock;
	}

	ret = -EBUSY;
	if (fc->phase != FS_CONTEXT_AWAITING_MOUNT)
		goto err_unlock;

	if (fc->sb_flags & SB_MANDLOCK)
		warn_mandlock();

	newmount.mnt = vfs_create_mount(fc);
	if (IS_ERR(newmount.mnt)) {
		ret = PTR_ERR(newmount.mnt);
		goto err_unlock;
	}
	newmount.dentry = dget(fc->root);
	newmount.mnt->mnt_flags = mnt_flags;

	/* We've done the mount bit - now move the file context into more or
	 * less the same state as if we'd done an fspick().  We don't want to
	 * do any memory allocation or anything like that at this point as we
	 * don't want to have to handle any errors incurred.
	 */
	vfs_clean_context(fc);

	ns = alloc_mnt_ns(current->nsproxy->mnt_ns->user_ns, true);
	if (IS_ERR(ns)) {
		ret = PTR_ERR(ns);
		goto err_path;
	}
	mnt = real_mount(newmount.mnt);
	ns->root = mnt;
	ns->nr_mounts = 1;
	mnt_add_to_ns(ns, mnt);
	mntget(newmount.mnt);

	/* Attach to an apparent O_PATH fd with a note that we need to unmount
	 * it, not just simply put it.
	 */
	file = dentry_open(&newmount, O_PATH, fc->cred);
	if (IS_ERR(file)) {
		dissolve_on_fput(newmount.mnt);
		ret = PTR_ERR(file);
		goto err_path;
	}
	file->f_mode |= FMODE_NEED_UNMOUNT;

	ret = get_unused_fd_flags((flags & FSMOUNT_CLOEXEC) ? O_CLOEXEC : 0);
	if (ret >= 0)
		fd_install(ret, file);
	else
		fput(file);

err_path:
	path_put(&newmount);
err_unlock:
	mutex_unlock(&fc->uapi_mutex);
err_fsfd:
	fdput(f);
	return ret;
}

/*
 * Move a mount from one place to another.  In combination with
 * fsopen()/fsmount() this is used to install a new mount and in combination
 * with open_tree(OPEN_TREE_CLONE [| AT_RECURSIVE]) it can be used to copy
 * a mount subtree.
 *
 * Note the flags value is a combination of MOVE_MOUNT_* flags.
 */
SYSCALL_DEFINE5(move_mount,
		int, from_dfd, const char __user *, from_pathname,
		int, to_dfd, const char __user *, to_pathname,
		unsigned int, flags)
{
	struct path from_path, to_path;
	unsigned int lflags;
	int ret = 0;

	if (!may_mount())
		return -EPERM;

	if (flags & ~MOVE_MOUNT__MASK)
		return -EINVAL;

	if ((flags & (MOVE_MOUNT_BENEATH | MOVE_MOUNT_SET_GROUP)) ==
	    (MOVE_MOUNT_BENEATH | MOVE_MOUNT_SET_GROUP))
		return -EINVAL;

	/* If someone gives a pathname, they aren't permitted to move
	 * from an fd that requires unmount as we can't get at the flag
	 * to clear it afterwards.
	 */
	lflags = 0;
	if (flags & MOVE_MOUNT_F_SYMLINKS)	lflags |= LOOKUP_FOLLOW;
	if (flags & MOVE_MOUNT_F_AUTOMOUNTS)	lflags |= LOOKUP_AUTOMOUNT;
	if (flags & MOVE_MOUNT_F_EMPTY_PATH)	lflags |= LOOKUP_EMPTY;

	ret = user_path_at(from_dfd, from_pathname, lflags, &from_path);
	if (ret < 0)
		return ret;

	lflags = 0;
	if (flags & MOVE_MOUNT_T_SYMLINKS)	lflags |= LOOKUP_FOLLOW;
	if (flags & MOVE_MOUNT_T_AUTOMOUNTS)	lflags |= LOOKUP_AUTOMOUNT;
	if (flags & MOVE_MOUNT_T_EMPTY_PATH)	lflags |= LOOKUP_EMPTY;

	ret = user_path_at(to_dfd, to_pathname, lflags, &to_path);
	if (ret < 0)
		goto out_from;

	ret = security_move_mount(&from_path, &to_path);
	if (ret < 0)
		goto out_to;

	if (flags & MOVE_MOUNT_SET_GROUP)
		ret = do_set_group(&from_path, &to_path);
	else
		ret = do_move_mount(&from_path, &to_path,
				    (flags & MOVE_MOUNT_BENEATH));

out_to:
	path_put(&to_path);
out_from:
	path_put(&from_path);
	return ret;
}

/*
 * Return true if path is reachable from root
 *
 * namespace_sem or mount_lock is held
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
bool is_path_reachable(struct mount *mnt, struct dentry *dentry,
			 const struct path *root)
{
	while (&mnt->mnt != root->mnt && mnt_has_parent(mnt)) {
		dentry = mnt->mnt_mountpoint;
		mnt = mnt->mnt_parent;
	}
	return &mnt->mnt == root->mnt && is_subdir(dentry, root->dentry);
}

<<<<<<< HEAD
int path_is_under(struct path *path1, struct path *path2)
{
	int res;
	br_read_lock(&vfsmount_lock);
	res = is_path_reachable(real_mount(path1->mnt), path1->dentry, path2);
	br_read_unlock(&vfsmount_lock);
=======
bool path_is_under(const struct path *path1, const struct path *path2)
{
	bool res;
	read_seqlock_excl(&mount_lock);
	res = is_path_reachable(real_mount(path1->mnt), path1->dentry, path2);
	read_sequnlock_excl(&mount_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return res;
}
EXPORT_SYMBOL(path_is_under);

/*
 * pivot_root Semantics:
 * Moves the root file system of the current process to the directory put_old,
 * makes new_root as the new root file system of the current process, and sets
 * root/cwd of all processes which had them on the current root to new_root.
 *
 * Restrictions:
 * The new_root and put_old must be directories, and  must not be on the
 * same file  system as the current process root. The put_old  must  be
 * underneath new_root,  i.e. adding a non-zero number of /.. to the string
 * pointed to by put_old must yield the same directory as new_root. No other
 * file system may be mounted on put_old. After all, new_root is a mountpoint.
 *
 * Also, the current root cannot be on the 'rootfs' (initial ramfs) filesystem.
<<<<<<< HEAD
 * See Documentation/filesystems/ramfs-rootfs-initramfs.txt for alternatives
=======
 * See Documentation/filesystems/ramfs-rootfs-initramfs.rst for alternatives
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * in this situation.
 *
 * Notes:
 *  - we don't move root/cwd if they are not at the root (reason: if something
 *    cared enough to change them, it's probably wrong to force them elsewhere)
 *  - it's okay to pick a root that isn't the root of a file system, e.g.
 *    /nfs/my_root where /nfs is the mount point. It must be a mountpoint,
 *    though, so you may need to say mount --bind /nfs/my_root /nfs/my_root
 *    first.
 */
SYSCALL_DEFINE2(pivot_root, const char __user *, new_root,
		const char __user *, put_old)
{
<<<<<<< HEAD
	struct path new, old, parent_path, root_parent, root;
	struct mount *new_mnt, *root_mnt;
	int error;

	if (!ns_capable(current->nsproxy->mnt_ns->user_ns, CAP_SYS_ADMIN))
		return -EPERM;

	error = user_path_dir(new_root, &new);
	if (error)
		goto out0;

	error = user_path_dir(put_old, &old);
=======
	struct path new, old, root;
	struct mount *new_mnt, *root_mnt, *old_mnt, *root_parent, *ex_parent;
	struct mountpoint *old_mp, *root_mp;
	int error;

	if (!may_mount())
		return -EPERM;

	error = user_path_at(AT_FDCWD, new_root,
			     LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &new);
	if (error)
		goto out0;

	error = user_path_at(AT_FDCWD, put_old,
			     LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &old);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		goto out1;

	error = security_sb_pivotroot(&old, &new);
	if (error)
		goto out2;

	get_fs_root(current->fs, &root);
<<<<<<< HEAD
	error = lock_mount(&old);
	if (error)
=======
	old_mp = lock_mount(&old);
	error = PTR_ERR(old_mp);
	if (IS_ERR(old_mp))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out3;

	error = -EINVAL;
	new_mnt = real_mount(new.mnt);
	root_mnt = real_mount(root.mnt);
<<<<<<< HEAD
	if (IS_MNT_SHARED(real_mount(old.mnt)) ||
		IS_MNT_SHARED(new_mnt->mnt_parent) ||
		IS_MNT_SHARED(root_mnt->mnt_parent))
		goto out4;
	if (!check_mnt(root_mnt) || !check_mnt(new_mnt))
		goto out4;
	error = -ENOENT;
	if (d_unlinked(new.dentry))
		goto out4;
	if (d_unlinked(old.dentry))
		goto out4;
	error = -EBUSY;
	if (new.mnt == root.mnt ||
	    old.mnt == root.mnt)
		goto out4; /* loop, on the same file system  */
	error = -EINVAL;
	if (root.mnt->mnt_root != root.dentry)
		goto out4; /* not a mountpoint */
	if (!mnt_has_parent(root_mnt))
		goto out4; /* not attached */
	if (new.mnt->mnt_root != new.dentry)
=======
	old_mnt = real_mount(old.mnt);
	ex_parent = new_mnt->mnt_parent;
	root_parent = root_mnt->mnt_parent;
	if (IS_MNT_SHARED(old_mnt) ||
		IS_MNT_SHARED(ex_parent) ||
		IS_MNT_SHARED(root_parent))
		goto out4;
	if (!check_mnt(root_mnt) || !check_mnt(new_mnt))
		goto out4;
	if (new_mnt->mnt.mnt_flags & MNT_LOCKED)
		goto out4;
	error = -ENOENT;
	if (d_unlinked(new.dentry))
		goto out4;
	error = -EBUSY;
	if (new_mnt == root_mnt || old_mnt == root_mnt)
		goto out4; /* loop, on the same file system  */
	error = -EINVAL;
	if (!path_mounted(&root))
		goto out4; /* not a mountpoint */
	if (!mnt_has_parent(root_mnt))
		goto out4; /* not attached */
	if (!path_mounted(&new))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out4; /* not a mountpoint */
	if (!mnt_has_parent(new_mnt))
		goto out4; /* not attached */
	/* make sure we can reach put_old from new_root */
<<<<<<< HEAD
	if (!is_path_reachable(real_mount(old.mnt), old.dentry, &new))
=======
	if (!is_path_reachable(old_mnt, old.dentry, &new))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out4;
	/* make certain new is below the root */
	if (!is_path_reachable(new_mnt, new.dentry, &root))
		goto out4;
<<<<<<< HEAD
	br_write_lock(&vfsmount_lock);
	detach_mnt(new_mnt, &parent_path);
	detach_mnt(root_mnt, &root_parent);
	/* mount old root on put_old */
	attach_mnt(root_mnt, &old);
	/* mount new_root on / */
	attach_mnt(new_mnt, &root_parent);
	touch_mnt_namespace(current->nsproxy->mnt_ns);
	br_write_unlock(&vfsmount_lock);
	chroot_fs_refs(&root, &new);
	error = 0;
out4:
	unlock_mount(&old);
	if (!error) {
		path_put(&root_parent);
		path_put(&parent_path);
	}
=======
	lock_mount_hash();
	umount_mnt(new_mnt);
	root_mp = unhash_mnt(root_mnt);  /* we'll need its mountpoint */
	if (root_mnt->mnt.mnt_flags & MNT_LOCKED) {
		new_mnt->mnt.mnt_flags |= MNT_LOCKED;
		root_mnt->mnt.mnt_flags &= ~MNT_LOCKED;
	}
	/* mount old root on put_old */
	attach_mnt(root_mnt, old_mnt, old_mp, false);
	/* mount new_root on / */
	attach_mnt(new_mnt, root_parent, root_mp, false);
	mnt_add_count(root_parent, -1);
	touch_mnt_namespace(current->nsproxy->mnt_ns);
	/* A moved mount should not expire automatically */
	list_del_init(&new_mnt->mnt_expire);
	put_mountpoint(root_mp);
	unlock_mount_hash();
	chroot_fs_refs(&root, &new);
	error = 0;
out4:
	unlock_mount(old_mp);
	if (!error)
		mntput_no_expire(ex_parent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out3:
	path_put(&root);
out2:
	path_put(&old);
out1:
	path_put(&new);
out0:
	return error;
}

<<<<<<< HEAD
static void __init init_mount_tree(void)
{
	struct vfsmount *mnt;
	struct mnt_namespace *ns;
	struct path root;
	struct file_system_type *type;

	type = get_fs_type("rootfs");
	if (!type)
		panic("Can't find rootfs type");
	mnt = vfs_kern_mount(type, 0, "rootfs", NULL);
	put_filesystem(type);
	if (IS_ERR(mnt))
		panic("Can't create rootfs");

	ns = create_mnt_ns(mnt);
	if (IS_ERR(ns))
		panic("Can't allocate initial namespace");

=======
static unsigned int recalc_flags(struct mount_kattr *kattr, struct mount *mnt)
{
	unsigned int flags = mnt->mnt.mnt_flags;

	/*  flags to clear */
	flags &= ~kattr->attr_clr;
	/* flags to raise */
	flags |= kattr->attr_set;

	return flags;
}

static int can_idmap_mount(const struct mount_kattr *kattr, struct mount *mnt)
{
	struct vfsmount *m = &mnt->mnt;
	struct user_namespace *fs_userns = m->mnt_sb->s_user_ns;

	if (!kattr->mnt_idmap)
		return 0;

	/*
	 * Creating an idmapped mount with the filesystem wide idmapping
	 * doesn't make sense so block that. We don't allow mushy semantics.
	 */
	if (kattr->mnt_userns == m->mnt_sb->s_user_ns)
		return -EINVAL;

	/*
	 * Once a mount has been idmapped we don't allow it to change its
	 * mapping. It makes things simpler and callers can just create
	 * another bind-mount they can idmap if they want to.
	 */
	if (is_idmapped_mnt(m))
		return -EPERM;

	/* The underlying filesystem doesn't support idmapped mounts yet. */
	if (!(m->mnt_sb->s_type->fs_flags & FS_ALLOW_IDMAP))
		return -EINVAL;

	/* We're not controlling the superblock. */
	if (!ns_capable(fs_userns, CAP_SYS_ADMIN))
		return -EPERM;

	/* Mount has already been visible in the filesystem hierarchy. */
	if (!is_anon_ns(mnt->mnt_ns))
		return -EINVAL;

	return 0;
}

/**
 * mnt_allow_writers() - check whether the attribute change allows writers
 * @kattr: the new mount attributes
 * @mnt: the mount to which @kattr will be applied
 *
 * Check whether thew new mount attributes in @kattr allow concurrent writers.
 *
 * Return: true if writers need to be held, false if not
 */
static inline bool mnt_allow_writers(const struct mount_kattr *kattr,
				     const struct mount *mnt)
{
	return (!(kattr->attr_set & MNT_READONLY) ||
		(mnt->mnt.mnt_flags & MNT_READONLY)) &&
	       !kattr->mnt_idmap;
}

static int mount_setattr_prepare(struct mount_kattr *kattr, struct mount *mnt)
{
	struct mount *m;
	int err;

	for (m = mnt; m; m = next_mnt(m, mnt)) {
		if (!can_change_locked_flags(m, recalc_flags(kattr, m))) {
			err = -EPERM;
			break;
		}

		err = can_idmap_mount(kattr, m);
		if (err)
			break;

		if (!mnt_allow_writers(kattr, m)) {
			err = mnt_hold_writers(m);
			if (err)
				break;
		}

		if (!kattr->recurse)
			return 0;
	}

	if (err) {
		struct mount *p;

		/*
		 * If we had to call mnt_hold_writers() MNT_WRITE_HOLD will
		 * be set in @mnt_flags. The loop unsets MNT_WRITE_HOLD for all
		 * mounts and needs to take care to include the first mount.
		 */
		for (p = mnt; p; p = next_mnt(p, mnt)) {
			/* If we had to hold writers unblock them. */
			if (p->mnt.mnt_flags & MNT_WRITE_HOLD)
				mnt_unhold_writers(p);

			/*
			 * We're done once the first mount we changed got
			 * MNT_WRITE_HOLD unset.
			 */
			if (p == m)
				break;
		}
	}
	return err;
}

static void do_idmap_mount(const struct mount_kattr *kattr, struct mount *mnt)
{
	if (!kattr->mnt_idmap)
		return;

	/*
	 * Pairs with smp_load_acquire() in mnt_idmap().
	 *
	 * Since we only allow a mount to change the idmapping once and
	 * verified this in can_idmap_mount() we know that the mount has
	 * @nop_mnt_idmap attached to it. So there's no need to drop any
	 * references.
	 */
	smp_store_release(&mnt->mnt.mnt_idmap, mnt_idmap_get(kattr->mnt_idmap));
}

static void mount_setattr_commit(struct mount_kattr *kattr, struct mount *mnt)
{
	struct mount *m;

	for (m = mnt; m; m = next_mnt(m, mnt)) {
		unsigned int flags;

		do_idmap_mount(kattr, m);
		flags = recalc_flags(kattr, m);
		WRITE_ONCE(m->mnt.mnt_flags, flags);

		/* If we had to hold writers unblock them. */
		if (m->mnt.mnt_flags & MNT_WRITE_HOLD)
			mnt_unhold_writers(m);

		if (kattr->propagation)
			change_mnt_propagation(m, kattr->propagation);
		if (!kattr->recurse)
			break;
	}
	touch_mnt_namespace(mnt->mnt_ns);
}

static int do_mount_setattr(struct path *path, struct mount_kattr *kattr)
{
	struct mount *mnt = real_mount(path->mnt);
	int err = 0;

	if (!path_mounted(path))
		return -EINVAL;

	if (kattr->mnt_userns) {
		struct mnt_idmap *mnt_idmap;

		mnt_idmap = alloc_mnt_idmap(kattr->mnt_userns);
		if (IS_ERR(mnt_idmap))
			return PTR_ERR(mnt_idmap);
		kattr->mnt_idmap = mnt_idmap;
	}

	if (kattr->propagation) {
		/*
		 * Only take namespace_lock() if we're actually changing
		 * propagation.
		 */
		namespace_lock();
		if (kattr->propagation == MS_SHARED) {
			err = invent_group_ids(mnt, kattr->recurse);
			if (err) {
				namespace_unlock();
				return err;
			}
		}
	}

	err = -EINVAL;
	lock_mount_hash();

	/* Ensure that this isn't anything purely vfs internal. */
	if (!is_mounted(&mnt->mnt))
		goto out;

	/*
	 * If this is an attached mount make sure it's located in the callers
	 * mount namespace. If it's not don't let the caller interact with it.
	 *
	 * If this mount doesn't have a parent it's most often simply a
	 * detached mount with an anonymous mount namespace. IOW, something
	 * that's simply not attached yet. But there are apparently also users
	 * that do change mount properties on the rootfs itself. That obviously
	 * neither has a parent nor is it a detached mount so we cannot
	 * unconditionally check for detached mounts.
	 */
	if ((mnt_has_parent(mnt) || !is_anon_ns(mnt->mnt_ns)) && !check_mnt(mnt))
		goto out;

	/*
	 * First, we get the mount tree in a shape where we can change mount
	 * properties without failure. If we succeeded to do so we commit all
	 * changes and if we failed we clean up.
	 */
	err = mount_setattr_prepare(kattr, mnt);
	if (!err)
		mount_setattr_commit(kattr, mnt);

out:
	unlock_mount_hash();

	if (kattr->propagation) {
		if (err)
			cleanup_group_ids(mnt, NULL);
		namespace_unlock();
	}

	return err;
}

static int build_mount_idmapped(const struct mount_attr *attr, size_t usize,
				struct mount_kattr *kattr, unsigned int flags)
{
	int err = 0;
	struct ns_common *ns;
	struct user_namespace *mnt_userns;
	struct fd f;

	if (!((attr->attr_set | attr->attr_clr) & MOUNT_ATTR_IDMAP))
		return 0;

	/*
	 * We currently do not support clearing an idmapped mount. If this ever
	 * is a use-case we can revisit this but for now let's keep it simple
	 * and not allow it.
	 */
	if (attr->attr_clr & MOUNT_ATTR_IDMAP)
		return -EINVAL;

	if (attr->userns_fd > INT_MAX)
		return -EINVAL;

	f = fdget(attr->userns_fd);
	if (!f.file)
		return -EBADF;

	if (!proc_ns_file(f.file)) {
		err = -EINVAL;
		goto out_fput;
	}

	ns = get_proc_ns(file_inode(f.file));
	if (ns->ops->type != CLONE_NEWUSER) {
		err = -EINVAL;
		goto out_fput;
	}

	/*
	 * The initial idmapping cannot be used to create an idmapped
	 * mount. We use the initial idmapping as an indicator of a mount
	 * that is not idmapped. It can simply be passed into helpers that
	 * are aware of idmapped mounts as a convenient shortcut. A user
	 * can just create a dedicated identity mapping to achieve the same
	 * result.
	 */
	mnt_userns = container_of(ns, struct user_namespace, ns);
	if (mnt_userns == &init_user_ns) {
		err = -EPERM;
		goto out_fput;
	}

	/* We're not controlling the target namespace. */
	if (!ns_capable(mnt_userns, CAP_SYS_ADMIN)) {
		err = -EPERM;
		goto out_fput;
	}

	kattr->mnt_userns = get_user_ns(mnt_userns);

out_fput:
	fdput(f);
	return err;
}

static int build_mount_kattr(const struct mount_attr *attr, size_t usize,
			     struct mount_kattr *kattr, unsigned int flags)
{
	unsigned int lookup_flags = LOOKUP_AUTOMOUNT | LOOKUP_FOLLOW;

	if (flags & AT_NO_AUTOMOUNT)
		lookup_flags &= ~LOOKUP_AUTOMOUNT;
	if (flags & AT_SYMLINK_NOFOLLOW)
		lookup_flags &= ~LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
		lookup_flags |= LOOKUP_EMPTY;

	*kattr = (struct mount_kattr) {
		.lookup_flags	= lookup_flags,
		.recurse	= !!(flags & AT_RECURSIVE),
	};

	if (attr->propagation & ~MOUNT_SETATTR_PROPAGATION_FLAGS)
		return -EINVAL;
	if (hweight32(attr->propagation & MOUNT_SETATTR_PROPAGATION_FLAGS) > 1)
		return -EINVAL;
	kattr->propagation = attr->propagation;

	if ((attr->attr_set | attr->attr_clr) & ~MOUNT_SETATTR_VALID_FLAGS)
		return -EINVAL;

	kattr->attr_set = attr_flags_to_mnt_flags(attr->attr_set);
	kattr->attr_clr = attr_flags_to_mnt_flags(attr->attr_clr);

	/*
	 * Since the MOUNT_ATTR_<atime> values are an enum, not a bitmap,
	 * users wanting to transition to a different atime setting cannot
	 * simply specify the atime setting in @attr_set, but must also
	 * specify MOUNT_ATTR__ATIME in the @attr_clr field.
	 * So ensure that MOUNT_ATTR__ATIME can't be partially set in
	 * @attr_clr and that @attr_set can't have any atime bits set if
	 * MOUNT_ATTR__ATIME isn't set in @attr_clr.
	 */
	if (attr->attr_clr & MOUNT_ATTR__ATIME) {
		if ((attr->attr_clr & MOUNT_ATTR__ATIME) != MOUNT_ATTR__ATIME)
			return -EINVAL;

		/*
		 * Clear all previous time settings as they are mutually
		 * exclusive.
		 */
		kattr->attr_clr |= MNT_RELATIME | MNT_NOATIME;
		switch (attr->attr_set & MOUNT_ATTR__ATIME) {
		case MOUNT_ATTR_RELATIME:
			kattr->attr_set |= MNT_RELATIME;
			break;
		case MOUNT_ATTR_NOATIME:
			kattr->attr_set |= MNT_NOATIME;
			break;
		case MOUNT_ATTR_STRICTATIME:
			break;
		default:
			return -EINVAL;
		}
	} else {
		if (attr->attr_set & MOUNT_ATTR__ATIME)
			return -EINVAL;
	}

	return build_mount_idmapped(attr, usize, kattr, flags);
}

static void finish_mount_kattr(struct mount_kattr *kattr)
{
	put_user_ns(kattr->mnt_userns);
	kattr->mnt_userns = NULL;

	if (kattr->mnt_idmap)
		mnt_idmap_put(kattr->mnt_idmap);
}

SYSCALL_DEFINE5(mount_setattr, int, dfd, const char __user *, path,
		unsigned int, flags, struct mount_attr __user *, uattr,
		size_t, usize)
{
	int err;
	struct path target;
	struct mount_attr attr;
	struct mount_kattr kattr;

	BUILD_BUG_ON(sizeof(struct mount_attr) != MOUNT_ATTR_SIZE_VER0);

	if (flags & ~(AT_EMPTY_PATH |
		      AT_RECURSIVE |
		      AT_SYMLINK_NOFOLLOW |
		      AT_NO_AUTOMOUNT))
		return -EINVAL;

	if (unlikely(usize > PAGE_SIZE))
		return -E2BIG;
	if (unlikely(usize < MOUNT_ATTR_SIZE_VER0))
		return -EINVAL;

	if (!may_mount())
		return -EPERM;

	err = copy_struct_from_user(&attr, sizeof(attr), uattr, usize);
	if (err)
		return err;

	/* Don't bother walking through the mounts if this is a nop. */
	if (attr.attr_set == 0 &&
	    attr.attr_clr == 0 &&
	    attr.propagation == 0)
		return 0;

	err = build_mount_kattr(&attr, usize, &kattr, flags);
	if (err)
		return err;

	err = user_path_at(dfd, path, kattr.lookup_flags, &target);
	if (!err) {
		err = do_mount_setattr(&target, &kattr);
		path_put(&target);
	}
	finish_mount_kattr(&kattr);
	return err;
}

int show_path(struct seq_file *m, struct dentry *root)
{
	if (root->d_sb->s_op->show_path)
		return root->d_sb->s_op->show_path(m, root);

	seq_dentry(m, root, " \t\n\\");
	return 0;
}

static struct vfsmount *lookup_mnt_in_ns(u64 id, struct mnt_namespace *ns)
{
	struct mount *mnt = mnt_find_id_at(ns, id);

	if (!mnt || mnt->mnt_id_unique != id)
		return NULL;

	return &mnt->mnt;
}

struct kstatmount {
	struct statmount __user *buf;
	size_t bufsize;
	struct vfsmount *mnt;
	u64 mask;
	struct path root;
	struct statmount sm;
	struct seq_file seq;
};

static u64 mnt_to_attr_flags(struct vfsmount *mnt)
{
	unsigned int mnt_flags = READ_ONCE(mnt->mnt_flags);
	u64 attr_flags = 0;

	if (mnt_flags & MNT_READONLY)
		attr_flags |= MOUNT_ATTR_RDONLY;
	if (mnt_flags & MNT_NOSUID)
		attr_flags |= MOUNT_ATTR_NOSUID;
	if (mnt_flags & MNT_NODEV)
		attr_flags |= MOUNT_ATTR_NODEV;
	if (mnt_flags & MNT_NOEXEC)
		attr_flags |= MOUNT_ATTR_NOEXEC;
	if (mnt_flags & MNT_NODIRATIME)
		attr_flags |= MOUNT_ATTR_NODIRATIME;
	if (mnt_flags & MNT_NOSYMFOLLOW)
		attr_flags |= MOUNT_ATTR_NOSYMFOLLOW;

	if (mnt_flags & MNT_NOATIME)
		attr_flags |= MOUNT_ATTR_NOATIME;
	else if (mnt_flags & MNT_RELATIME)
		attr_flags |= MOUNT_ATTR_RELATIME;
	else
		attr_flags |= MOUNT_ATTR_STRICTATIME;

	if (is_idmapped_mnt(mnt))
		attr_flags |= MOUNT_ATTR_IDMAP;

	return attr_flags;
}

static u64 mnt_to_propagation_flags(struct mount *m)
{
	u64 propagation = 0;

	if (IS_MNT_SHARED(m))
		propagation |= MS_SHARED;
	if (IS_MNT_SLAVE(m))
		propagation |= MS_SLAVE;
	if (IS_MNT_UNBINDABLE(m))
		propagation |= MS_UNBINDABLE;
	if (!propagation)
		propagation |= MS_PRIVATE;

	return propagation;
}

static void statmount_sb_basic(struct kstatmount *s)
{
	struct super_block *sb = s->mnt->mnt_sb;

	s->sm.mask |= STATMOUNT_SB_BASIC;
	s->sm.sb_dev_major = MAJOR(sb->s_dev);
	s->sm.sb_dev_minor = MINOR(sb->s_dev);
	s->sm.sb_magic = sb->s_magic;
	s->sm.sb_flags = sb->s_flags & (SB_RDONLY|SB_SYNCHRONOUS|SB_DIRSYNC|SB_LAZYTIME);
}

static void statmount_mnt_basic(struct kstatmount *s)
{
	struct mount *m = real_mount(s->mnt);

	s->sm.mask |= STATMOUNT_MNT_BASIC;
	s->sm.mnt_id = m->mnt_id_unique;
	s->sm.mnt_parent_id = m->mnt_parent->mnt_id_unique;
	s->sm.mnt_id_old = m->mnt_id;
	s->sm.mnt_parent_id_old = m->mnt_parent->mnt_id;
	s->sm.mnt_attr = mnt_to_attr_flags(&m->mnt);
	s->sm.mnt_propagation = mnt_to_propagation_flags(m);
	s->sm.mnt_peer_group = IS_MNT_SHARED(m) ? m->mnt_group_id : 0;
	s->sm.mnt_master = IS_MNT_SLAVE(m) ? m->mnt_master->mnt_group_id : 0;
}

static void statmount_propagate_from(struct kstatmount *s)
{
	struct mount *m = real_mount(s->mnt);

	s->sm.mask |= STATMOUNT_PROPAGATE_FROM;
	if (IS_MNT_SLAVE(m))
		s->sm.propagate_from = get_dominating_id(m, &current->fs->root);
}

static int statmount_mnt_root(struct kstatmount *s, struct seq_file *seq)
{
	int ret;
	size_t start = seq->count;

	ret = show_path(seq, s->mnt->mnt_root);
	if (ret)
		return ret;

	if (unlikely(seq_has_overflowed(seq)))
		return -EAGAIN;

	/*
         * Unescape the result. It would be better if supplied string was not
         * escaped in the first place, but that's a pretty invasive change.
         */
	seq->buf[seq->count] = '\0';
	seq->count = start;
	seq_commit(seq, string_unescape_inplace(seq->buf + start, UNESCAPE_OCTAL));
	return 0;
}

static int statmount_mnt_point(struct kstatmount *s, struct seq_file *seq)
{
	struct vfsmount *mnt = s->mnt;
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };
	int err;

	err = seq_path_root(seq, &mnt_path, &s->root, "");
	return err == SEQ_SKIP ? 0 : err;
}

static int statmount_fs_type(struct kstatmount *s, struct seq_file *seq)
{
	struct super_block *sb = s->mnt->mnt_sb;

	seq_puts(seq, sb->s_type->name);
	return 0;
}

static int statmount_string(struct kstatmount *s, u64 flag)
{
	int ret;
	size_t kbufsize;
	struct seq_file *seq = &s->seq;
	struct statmount *sm = &s->sm;

	switch (flag) {
	case STATMOUNT_FS_TYPE:
		sm->fs_type = seq->count;
		ret = statmount_fs_type(s, seq);
		break;
	case STATMOUNT_MNT_ROOT:
		sm->mnt_root = seq->count;
		ret = statmount_mnt_root(s, seq);
		break;
	case STATMOUNT_MNT_POINT:
		sm->mnt_point = seq->count;
		ret = statmount_mnt_point(s, seq);
		break;
	default:
		WARN_ON_ONCE(true);
		return -EINVAL;
	}

	if (unlikely(check_add_overflow(sizeof(*sm), seq->count, &kbufsize)))
		return -EOVERFLOW;
	if (kbufsize >= s->bufsize)
		return -EOVERFLOW;

	/* signal a retry */
	if (unlikely(seq_has_overflowed(seq)))
		return -EAGAIN;

	if (ret)
		return ret;

	seq->buf[seq->count++] = '\0';
	sm->mask |= flag;
	return 0;
}

static int copy_statmount_to_user(struct kstatmount *s)
{
	struct statmount *sm = &s->sm;
	struct seq_file *seq = &s->seq;
	char __user *str = ((char __user *)s->buf) + sizeof(*sm);
	size_t copysize = min_t(size_t, s->bufsize, sizeof(*sm));

	if (seq->count && copy_to_user(str, seq->buf, seq->count))
		return -EFAULT;

	/* Return the number of bytes copied to the buffer */
	sm->size = copysize + seq->count;
	if (copy_to_user(s->buf, sm, copysize))
		return -EFAULT;

	return 0;
}

static int do_statmount(struct kstatmount *s)
{
	struct mount *m = real_mount(s->mnt);
	int err;

	/*
	 * Don't trigger audit denials. We just want to determine what
	 * mounts to show users.
	 */
	if (!is_path_reachable(m, m->mnt.mnt_root, &s->root) &&
	    !ns_capable_noaudit(&init_user_ns, CAP_SYS_ADMIN))
		return -EPERM;

	err = security_sb_statfs(s->mnt->mnt_root);
	if (err)
		return err;

	if (s->mask & STATMOUNT_SB_BASIC)
		statmount_sb_basic(s);

	if (s->mask & STATMOUNT_MNT_BASIC)
		statmount_mnt_basic(s);

	if (s->mask & STATMOUNT_PROPAGATE_FROM)
		statmount_propagate_from(s);

	if (s->mask & STATMOUNT_FS_TYPE)
		err = statmount_string(s, STATMOUNT_FS_TYPE);

	if (!err && s->mask & STATMOUNT_MNT_ROOT)
		err = statmount_string(s, STATMOUNT_MNT_ROOT);

	if (!err && s->mask & STATMOUNT_MNT_POINT)
		err = statmount_string(s, STATMOUNT_MNT_POINT);

	if (err)
		return err;

	return 0;
}

static inline bool retry_statmount(const long ret, size_t *seq_size)
{
	if (likely(ret != -EAGAIN))
		return false;
	if (unlikely(check_mul_overflow(*seq_size, 2, seq_size)))
		return false;
	if (unlikely(*seq_size > MAX_RW_COUNT))
		return false;
	return true;
}

static int prepare_kstatmount(struct kstatmount *ks, struct mnt_id_req *kreq,
			      struct statmount __user *buf, size_t bufsize,
			      size_t seq_size)
{
	if (!access_ok(buf, bufsize))
		return -EFAULT;

	memset(ks, 0, sizeof(*ks));
	ks->mask = kreq->param;
	ks->buf = buf;
	ks->bufsize = bufsize;
	ks->seq.size = seq_size;
	ks->seq.buf = kvmalloc(seq_size, GFP_KERNEL_ACCOUNT);
	if (!ks->seq.buf)
		return -ENOMEM;
	return 0;
}

static int copy_mnt_id_req(const struct mnt_id_req __user *req,
			   struct mnt_id_req *kreq)
{
	int ret;
	size_t usize;

	BUILD_BUG_ON(sizeof(struct mnt_id_req) != MNT_ID_REQ_SIZE_VER0);

	ret = get_user(usize, &req->size);
	if (ret)
		return -EFAULT;
	if (unlikely(usize > PAGE_SIZE))
		return -E2BIG;
	if (unlikely(usize < MNT_ID_REQ_SIZE_VER0))
		return -EINVAL;
	memset(kreq, 0, sizeof(*kreq));
	ret = copy_struct_from_user(kreq, sizeof(*kreq), req, usize);
	if (ret)
		return ret;
	if (kreq->spare != 0)
		return -EINVAL;
	return 0;
}

SYSCALL_DEFINE4(statmount, const struct mnt_id_req __user *, req,
		struct statmount __user *, buf, size_t, bufsize,
		unsigned int, flags)
{
	struct vfsmount *mnt;
	struct mnt_id_req kreq;
	struct kstatmount ks;
	/* We currently support retrieval of 3 strings. */
	size_t seq_size = 3 * PATH_MAX;
	int ret;

	if (flags)
		return -EINVAL;

	ret = copy_mnt_id_req(req, &kreq);
	if (ret)
		return ret;

retry:
	ret = prepare_kstatmount(&ks, &kreq, buf, bufsize, seq_size);
	if (ret)
		return ret;

	down_read(&namespace_sem);
	mnt = lookup_mnt_in_ns(kreq.mnt_id, current->nsproxy->mnt_ns);
	if (!mnt) {
		up_read(&namespace_sem);
		kvfree(ks.seq.buf);
		return -ENOENT;
	}

	ks.mnt = mnt;
	get_fs_root(current->fs, &ks.root);
	ret = do_statmount(&ks);
	path_put(&ks.root);
	up_read(&namespace_sem);

	if (!ret)
		ret = copy_statmount_to_user(&ks);
	kvfree(ks.seq.buf);
	if (retry_statmount(ret, &seq_size))
		goto retry;
	return ret;
}

static struct mount *listmnt_next(struct mount *curr)
{
	return node_to_mount(rb_next(&curr->mnt_node));
}

static ssize_t do_listmount(struct mount *first, struct path *orig,
			    u64 mnt_parent_id, u64 __user *mnt_ids,
			    size_t nr_mnt_ids, const struct path *root)
{
	struct mount *r;
	ssize_t ret;

	/*
	 * Don't trigger audit denials. We just want to determine what
	 * mounts to show users.
	 */
	if (!is_path_reachable(real_mount(orig->mnt), orig->dentry, root) &&
	    !ns_capable_noaudit(&init_user_ns, CAP_SYS_ADMIN))
		return -EPERM;

	ret = security_sb_statfs(orig->dentry);
	if (ret)
		return ret;

	for (ret = 0, r = first; r && nr_mnt_ids; r = listmnt_next(r)) {
		if (r->mnt_id_unique == mnt_parent_id)
			continue;
		if (!is_path_reachable(r, r->mnt.mnt_root, orig))
			continue;
		if (put_user(r->mnt_id_unique, mnt_ids))
			return -EFAULT;
		mnt_ids++;
		nr_mnt_ids--;
		ret++;
	}
	return ret;
}

SYSCALL_DEFINE4(listmount, const struct mnt_id_req __user *, req, u64 __user *,
		mnt_ids, size_t, nr_mnt_ids, unsigned int, flags)
{
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	struct mnt_id_req kreq;
	struct mount *first;
	struct path root, orig;
	u64 mnt_parent_id, last_mnt_id;
	const size_t maxcount = (size_t)-1 >> 3;
	ssize_t ret;

	if (flags)
		return -EINVAL;

	if (unlikely(nr_mnt_ids > maxcount))
		return -EFAULT;

	if (!access_ok(mnt_ids, nr_mnt_ids * sizeof(*mnt_ids)))
		return -EFAULT;

	ret = copy_mnt_id_req(req, &kreq);
	if (ret)
		return ret;
	mnt_parent_id = kreq.mnt_id;
	last_mnt_id = kreq.param;

	down_read(&namespace_sem);
	get_fs_root(current->fs, &root);
	if (mnt_parent_id == LSMT_ROOT) {
		orig = root;
	} else {
		ret = -ENOENT;
		orig.mnt = lookup_mnt_in_ns(mnt_parent_id, ns);
		if (!orig.mnt)
			goto err;
		orig.dentry = orig.mnt->mnt_root;
	}
	if (!last_mnt_id)
		first = node_to_mount(rb_first(&ns->mounts));
	else
		first = mnt_find_id_at(ns, last_mnt_id + 1);

	ret = do_listmount(first, &orig, mnt_parent_id, mnt_ids, nr_mnt_ids, &root);
err:
	path_put(&root);
	up_read(&namespace_sem);
	return ret;
}


static void __init init_mount_tree(void)
{
	struct vfsmount *mnt;
	struct mount *m;
	struct mnt_namespace *ns;
	struct path root;

	mnt = vfs_kern_mount(&rootfs_fs_type, 0, "rootfs", NULL);
	if (IS_ERR(mnt))
		panic("Can't create rootfs");

	ns = alloc_mnt_ns(&init_user_ns, false);
	if (IS_ERR(ns))
		panic("Can't allocate initial namespace");
	m = real_mount(mnt);
	ns->root = m;
	ns->nr_mounts = 1;
	mnt_add_to_ns(ns, m);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	init_task.nsproxy->mnt_ns = ns;
	get_mnt_ns(ns);

	root.mnt = mnt;
	root.dentry = mnt->mnt_root;
<<<<<<< HEAD
=======
	mnt->mnt_flags |= MNT_LOCKED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	set_fs_pwd(current->fs, &root);
	set_fs_root(current->fs, &root);
}

void __init mnt_init(void)
{
<<<<<<< HEAD
	unsigned u;
	int err;

	init_rwsem(&namespace_sem);

	mnt_cache = kmem_cache_create("mnt_cache", sizeof(struct mount),
			0, SLAB_HWCACHE_ALIGN | SLAB_PANIC, NULL);

	mount_hashtable = (struct list_head *)__get_free_page(GFP_ATOMIC);

	if (!mount_hashtable)
		panic("Failed to allocate mount hash table\n");

	printk(KERN_INFO "Mount-cache hash table entries: %lu\n", HASH_SIZE);

	for (u = 0; u < HASH_SIZE; u++)
		INIT_LIST_HEAD(&mount_hashtable[u]);

	br_lock_init(&vfsmount_lock);
=======
	int err;

	mnt_cache = kmem_cache_create("mnt_cache", sizeof(struct mount),
			0, SLAB_HWCACHE_ALIGN|SLAB_PANIC|SLAB_ACCOUNT, NULL);

	mount_hashtable = alloc_large_system_hash("Mount-cache",
				sizeof(struct hlist_head),
				mhash_entries, 19,
				HASH_ZERO,
				&m_hash_shift, &m_hash_mask, 0, 0);
	mountpoint_hashtable = alloc_large_system_hash("Mountpoint-cache",
				sizeof(struct hlist_head),
				mphash_entries, 19,
				HASH_ZERO,
				&mp_hash_shift, &mp_hash_mask, 0, 0);

	if (!mount_hashtable || !mountpoint_hashtable)
		panic("Failed to allocate mount hash table\n");

	kernfs_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = sysfs_init();
	if (err)
		printk(KERN_WARNING "%s: sysfs_init error: %d\n",
			__func__, err);
	fs_kobj = kobject_create_and_add("fs", NULL);
	if (!fs_kobj)
		printk(KERN_WARNING "%s: kobj create error\n", __func__);
<<<<<<< HEAD
=======
	shmem_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	init_rootfs();
	init_mount_tree();
}

void put_mnt_ns(struct mnt_namespace *ns)
{
<<<<<<< HEAD
	LIST_HEAD(umount_list);

	if (!atomic_dec_and_test(&ns->count))
		return;
	down_write(&namespace_sem);
	br_write_lock(&vfsmount_lock);
	umount_tree(ns->root, 0, &umount_list);
	br_write_unlock(&vfsmount_lock);
	up_write(&namespace_sem);
	release_mounts(&umount_list);
	free_mnt_ns(ns);
}

struct vfsmount *kern_mount_data(struct file_system_type *type, void *data)
{
	struct vfsmount *mnt;
	mnt = vfs_kern_mount(type, MS_KERNMOUNT, type->name, data);
=======
	if (!refcount_dec_and_test(&ns->ns.count))
		return;
	drop_collected_mounts(&ns->root->mnt);
	free_mnt_ns(ns);
}

struct vfsmount *kern_mount(struct file_system_type *type)
{
	struct vfsmount *mnt;
	mnt = vfs_kern_mount(type, SB_KERNMOUNT, type->name, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!IS_ERR(mnt)) {
		/*
		 * it is a longterm mount, don't release mnt until
		 * we unmount before file sys is unregistered
		*/
		real_mount(mnt)->mnt_ns = MNT_NS_INTERNAL;
	}
	return mnt;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(kern_mount_data);
=======
EXPORT_SYMBOL_GPL(kern_mount);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void kern_unmount(struct vfsmount *mnt)
{
	/* release long term mount so mount point can be released */
<<<<<<< HEAD
	if (!IS_ERR_OR_NULL(mnt)) {
		br_write_lock(&vfsmount_lock);
		real_mount(mnt)->mnt_ns = NULL;
		br_write_unlock(&vfsmount_lock);
=======
	if (!IS_ERR(mnt)) {
		mnt_make_shortterm(mnt);
		synchronize_rcu();	/* yecchhh... */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mntput(mnt);
	}
}
EXPORT_SYMBOL(kern_unmount);

<<<<<<< HEAD
=======
void kern_unmount_array(struct vfsmount *mnt[], unsigned int num)
{
	unsigned int i;

	for (i = 0; i < num; i++)
		mnt_make_shortterm(mnt[i]);
	synchronize_rcu_expedited();
	for (i = 0; i < num; i++)
		mntput(mnt[i]);
}
EXPORT_SYMBOL(kern_unmount_array);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
bool our_mnt(struct vfsmount *mnt)
{
	return check_mnt(real_mount(mnt));
}

<<<<<<< HEAD
static void *mntns_get(struct task_struct *task)
{
	struct mnt_namespace *ns = NULL;
	struct nsproxy *nsproxy;

	rcu_read_lock();
	nsproxy = task_nsproxy(task);
	if (nsproxy) {
		ns = nsproxy->mnt_ns;
		get_mnt_ns(ns);
	}
	rcu_read_unlock();
=======
bool current_chrooted(void)
{
	/* Does the current process have a non-standard root */
	struct path ns_root;
	struct path fs_root;
	bool chrooted;

	/* Find the namespace root */
	ns_root.mnt = &current->nsproxy->mnt_ns->root->mnt;
	ns_root.dentry = ns_root.mnt->mnt_root;
	path_get(&ns_root);
	while (d_mountpoint(ns_root.dentry) && follow_down_one(&ns_root))
		;

	get_fs_root(current->fs, &fs_root);

	chrooted = !path_equal(&fs_root, &ns_root);

	path_put(&fs_root);
	path_put(&ns_root);

	return chrooted;
}

static bool mnt_already_visible(struct mnt_namespace *ns,
				const struct super_block *sb,
				int *new_mnt_flags)
{
	int new_flags = *new_mnt_flags;
	struct mount *mnt, *n;
	bool visible = false;

	down_read(&namespace_sem);
	rbtree_postorder_for_each_entry_safe(mnt, n, &ns->mounts, mnt_node) {
		struct mount *child;
		int mnt_flags;

		if (mnt->mnt.mnt_sb->s_type != sb->s_type)
			continue;

		/* This mount is not fully visible if it's root directory
		 * is not the root directory of the filesystem.
		 */
		if (mnt->mnt.mnt_root != mnt->mnt.mnt_sb->s_root)
			continue;

		/* A local view of the mount flags */
		mnt_flags = mnt->mnt.mnt_flags;

		/* Don't miss readonly hidden in the superblock flags */
		if (sb_rdonly(mnt->mnt.mnt_sb))
			mnt_flags |= MNT_LOCK_READONLY;

		/* Verify the mount flags are equal to or more permissive
		 * than the proposed new mount.
		 */
		if ((mnt_flags & MNT_LOCK_READONLY) &&
		    !(new_flags & MNT_READONLY))
			continue;
		if ((mnt_flags & MNT_LOCK_ATIME) &&
		    ((mnt_flags & MNT_ATIME_MASK) != (new_flags & MNT_ATIME_MASK)))
			continue;

		/* This mount is not fully visible if there are any
		 * locked child mounts that cover anything except for
		 * empty directories.
		 */
		list_for_each_entry(child, &mnt->mnt_mounts, mnt_child) {
			struct inode *inode = child->mnt_mountpoint->d_inode;
			/* Only worry about locked mounts */
			if (!(child->mnt.mnt_flags & MNT_LOCKED))
				continue;
			/* Is the directory permanetly empty? */
			if (!is_empty_dir_inode(inode))
				goto next;
		}
		/* Preserve the locked attributes */
		*new_mnt_flags |= mnt_flags & (MNT_LOCK_READONLY | \
					       MNT_LOCK_ATIME);
		visible = true;
		goto found;
	next:	;
	}
found:
	up_read(&namespace_sem);
	return visible;
}

static bool mount_too_revealing(const struct super_block *sb, int *new_mnt_flags)
{
	const unsigned long required_iflags = SB_I_NOEXEC | SB_I_NODEV;
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	unsigned long s_iflags;

	if (ns->user_ns == &init_user_ns)
		return false;

	/* Can this filesystem be too revealing? */
	s_iflags = sb->s_iflags;
	if (!(s_iflags & SB_I_USERNS_VISIBLE))
		return false;

	if ((s_iflags & required_iflags) != required_iflags) {
		WARN_ONCE(1, "Expected s_iflags to contain 0x%lx\n",
			  required_iflags);
		return true;
	}

	return !mnt_already_visible(ns, sb, new_mnt_flags);
}

bool mnt_may_suid(struct vfsmount *mnt)
{
	/*
	 * Foreign mounts (accessed via fchdir or through /proc
	 * symlinks) are always treated as if they are nosuid.  This
	 * prevents namespaces from trusting potentially unsafe
	 * suid/sgid bits, file caps, or security labels that originate
	 * in other namespaces.
	 */
	return !(mnt->mnt_flags & MNT_NOSUID) && check_mnt(real_mount(mnt)) &&
	       current_in_userns(mnt->mnt_sb->s_user_ns);
}

static struct ns_common *mntns_get(struct task_struct *task)
{
	struct ns_common *ns = NULL;
	struct nsproxy *nsproxy;

	task_lock(task);
	nsproxy = task->nsproxy;
	if (nsproxy) {
		ns = &nsproxy->mnt_ns->ns;
		get_mnt_ns(to_mnt_ns(ns));
	}
	task_unlock(task);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ns;
}

<<<<<<< HEAD
static void mntns_put(void *ns)
{
	put_mnt_ns(ns);
}

static int mntns_install(struct nsproxy *nsproxy, void *ns)
{
	struct fs_struct *fs = current->fs;
	struct mnt_namespace *mnt_ns = ns;
	struct path root;

	if (!ns_capable(mnt_ns->user_ns, CAP_SYS_ADMIN) ||
	    !nsown_capable(CAP_SYS_CHROOT))
=======
static void mntns_put(struct ns_common *ns)
{
	put_mnt_ns(to_mnt_ns(ns));
}

static int mntns_install(struct nsset *nsset, struct ns_common *ns)
{
	struct nsproxy *nsproxy = nsset->nsproxy;
	struct fs_struct *fs = nsset->fs;
	struct mnt_namespace *mnt_ns = to_mnt_ns(ns), *old_mnt_ns;
	struct user_namespace *user_ns = nsset->cred->user_ns;
	struct path root;
	int err;

	if (!ns_capable(mnt_ns->user_ns, CAP_SYS_ADMIN) ||
	    !ns_capable(user_ns, CAP_SYS_CHROOT) ||
	    !ns_capable(user_ns, CAP_SYS_ADMIN))
		return -EPERM;

	if (is_anon_ns(mnt_ns))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	if (fs->users != 1)
		return -EINVAL;

	get_mnt_ns(mnt_ns);
<<<<<<< HEAD
	put_mnt_ns(nsproxy->mnt_ns);
	nsproxy->mnt_ns = mnt_ns;

	/* Find the root */
	root.mnt    = &mnt_ns->root->mnt;
	root.dentry = mnt_ns->root->mnt.mnt_root;
	path_get(&root);
	while(d_mountpoint(root.dentry) && follow_down_one(&root))
		;
=======
	old_mnt_ns = nsproxy->mnt_ns;
	nsproxy->mnt_ns = mnt_ns;

	/* Find the root */
	err = vfs_path_lookup(mnt_ns->root->mnt.mnt_root, &mnt_ns->root->mnt,
				"/", LOOKUP_DOWN, &root);
	if (err) {
		/* revert to old namespace */
		nsproxy->mnt_ns = old_mnt_ns;
		put_mnt_ns(mnt_ns);
		return err;
	}

	put_mnt_ns(old_mnt_ns);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Update the pwd and root */
	set_fs_pwd(fs, &root);
	set_fs_root(fs, &root);

	path_put(&root);
	return 0;
}

<<<<<<< HEAD
static unsigned int mntns_inum(void *ns)
{
	struct mnt_namespace *mnt_ns = ns;
	return mnt_ns->proc_inum;
=======
static struct user_namespace *mntns_owner(struct ns_common *ns)
{
	return to_mnt_ns(ns)->user_ns;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

const struct proc_ns_operations mntns_operations = {
	.name		= "mnt",
	.type		= CLONE_NEWNS,
	.get		= mntns_get,
	.put		= mntns_put,
	.install	= mntns_install,
<<<<<<< HEAD
	.inum		= mntns_inum,
};
=======
	.owner		= mntns_owner,
};

#ifdef CONFIG_SYSCTL
static struct ctl_table fs_namespace_sysctls[] = {
	{
		.procname	= "mount-max",
		.data		= &sysctl_mount_max,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ONE,
	},
};

static int __init init_fs_namespace_sysctls(void)
{
	register_sysctl_init("fs", fs_namespace_sysctls);
	return 0;
}
fs_initcall(init_fs_namespace_sysctls);

#endif /* CONFIG_SYSCTL */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
