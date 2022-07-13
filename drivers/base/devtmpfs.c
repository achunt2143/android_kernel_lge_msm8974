<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * devtmpfs - kernel-maintained tmpfs-based /dev
 *
 * Copyright (C) 2009, Kay Sievers <kay.sievers@vrfy.org>
 *
 * During bootup, before any driver core device is registered,
 * devtmpfs, a tmpfs-based filesystem is created. Every driver-core
 * device which requests a device node, will add a node in this
 * filesystem.
<<<<<<< HEAD
 * By default, all devices are named after the the name of the
 * device, owned by root and have a default mode of 0600. Subsystems
 * can overwrite the default setting if needed.
 */

=======
 * By default, all devices are named after the name of the device,
 * owned by root and have a default mode of 0600. Subsystems can
 * overwrite the default setting if needed.
 */

#define pr_fmt(fmt) "devtmpfs: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/mount.h>
#include <linux/device.h>
<<<<<<< HEAD
#include <linux/genhd.h>
=======
#include <linux/blkdev.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/shmem_fs.h>
#include <linux/ramfs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/kthread.h>
<<<<<<< HEAD

static struct task_struct *thread;

#if defined CONFIG_DEVTMPFS_MOUNT
static int mount_dev = 1;
#else
static int mount_dev;
#endif
=======
#include <linux/init_syscalls.h>
#include <uapi/linux/mount.h>
#include "base.h"

#ifdef CONFIG_DEVTMPFS_SAFE
#define DEVTMPFS_MFLAGS       (MS_SILENT | MS_NOEXEC | MS_NOSUID)
#else
#define DEVTMPFS_MFLAGS       (MS_SILENT)
#endif

static struct task_struct *thread;

static int __initdata mount_dev = IS_ENABLED(CONFIG_DEVTMPFS_MOUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_SPINLOCK(req_lock);

static struct req {
	struct req *next;
	struct completion done;
	int err;
	const char *name;
	umode_t mode;	/* 0 => delete */
<<<<<<< HEAD
=======
	kuid_t uid;
	kgid_t gid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct device *dev;
} *requests;

static int __init mount_param(char *str)
{
	mount_dev = simple_strtoul(str, NULL, 0);
	return 1;
}
__setup("devtmpfs.mount=", mount_param);

<<<<<<< HEAD
static struct dentry *dev_mount(struct file_system_type *fs_type, int flags,
		      const char *dev_name, void *data)
{
#ifdef CONFIG_TMPFS
	return mount_single(fs_type, flags, data, shmem_fill_super);
#else
	return mount_single(fs_type, flags, data, ramfs_fill_super);
#endif
}

static struct file_system_type dev_fs_type = {
	.name = "devtmpfs",
	.mount = dev_mount,
	.kill_sb = kill_litter_super,
};

#ifdef CONFIG_BLOCK
static inline int is_blockdev(struct device *dev)
{
	return dev->class == &block_class;
}
#else
static inline int is_blockdev(struct device *dev) { return 0; }
#endif
=======
static struct vfsmount *mnt;

static struct dentry *public_dev_mount(struct file_system_type *fs_type, int flags,
		      const char *dev_name, void *data)
{
	struct super_block *s = mnt->mnt_sb;
	int err;

	atomic_inc(&s->s_active);
	down_write(&s->s_umount);
	err = reconfigure_single(s, flags, data);
	if (err < 0) {
		deactivate_locked_super(s);
		return ERR_PTR(err);
	}
	return dget(s->s_root);
}

static struct file_system_type internal_fs_type = {
	.name = "devtmpfs",
#ifdef CONFIG_TMPFS
	.init_fs_context = shmem_init_fs_context,
#else
	.init_fs_context = ramfs_init_fs_context,
#endif
	.kill_sb = kill_litter_super,
};

static struct file_system_type dev_fs_type = {
	.name = "devtmpfs",
	.mount = public_dev_mount,
};

static int devtmpfs_submit_req(struct req *req, const char *tmp)
{
	init_completion(&req->done);

	spin_lock(&req_lock);
	req->next = requests;
	requests = req;
	spin_unlock(&req_lock);

	wake_up_process(thread);
	wait_for_completion(&req->done);

	kfree(tmp);

	return req->err;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int devtmpfs_create_node(struct device *dev)
{
	const char *tmp = NULL;
	struct req req;

	if (!thread)
		return 0;

	req.mode = 0;
<<<<<<< HEAD
	req.name = device_get_devnode(dev, &req.mode, &tmp);
=======
	req.uid = GLOBAL_ROOT_UID;
	req.gid = GLOBAL_ROOT_GID;
	req.name = device_get_devnode(dev, &req.mode, &req.uid, &req.gid, &tmp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!req.name)
		return -ENOMEM;

	if (req.mode == 0)
		req.mode = 0600;
	if (is_blockdev(dev))
		req.mode |= S_IFBLK;
	else
		req.mode |= S_IFCHR;

	req.dev = dev;

<<<<<<< HEAD
	init_completion(&req.done);

	spin_lock(&req_lock);
	req.next = requests;
	requests = &req;
	spin_unlock(&req_lock);

	wake_up_process(thread);
	wait_for_completion(&req.done);

	kfree(tmp);

	return req.err;
=======
	return devtmpfs_submit_req(&req, tmp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int devtmpfs_delete_node(struct device *dev)
{
	const char *tmp = NULL;
	struct req req;

	if (!thread)
		return 0;

<<<<<<< HEAD
	req.name = device_get_devnode(dev, NULL, &tmp);
=======
	req.name = device_get_devnode(dev, NULL, NULL, NULL, &tmp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!req.name)
		return -ENOMEM;

	req.mode = 0;
	req.dev = dev;

<<<<<<< HEAD
	init_completion(&req.done);

	spin_lock(&req_lock);
	req.next = requests;
	requests = &req;
	spin_unlock(&req_lock);

	wake_up_process(thread);
	wait_for_completion(&req.done);

	kfree(tmp);
	return req.err;
=======
	return devtmpfs_submit_req(&req, tmp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int dev_mkdir(const char *name, umode_t mode)
{
	struct dentry *dentry;
	struct path path;
	int err;

<<<<<<< HEAD
	dentry = kern_path_create(AT_FDCWD, name, &path, 1);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

	err = vfs_mkdir(path.dentry->d_inode, dentry, mode);
	if (!err)
		/* mark as kernel-created inode */
		dentry->d_inode->i_private = &thread;
=======
	dentry = kern_path_create(AT_FDCWD, name, &path, LOOKUP_DIRECTORY);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

	err = vfs_mkdir(&nop_mnt_idmap, d_inode(path.dentry), dentry, mode);
	if (!err)
		/* mark as kernel-created inode */
		d_inode(dentry)->i_private = &thread;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	done_path_create(&path, dentry);
	return err;
}

static int create_path(const char *nodepath)
{
	char *path;
	char *s;
	int err = 0;

	/* parent directories do not exist, create them */
	path = kstrdup(nodepath, GFP_KERNEL);
	if (!path)
		return -ENOMEM;

	s = path;
	for (;;) {
		s = strchr(s, '/');
		if (!s)
			break;
		s[0] = '\0';
		err = dev_mkdir(path, 0755);
		if (err && err != -EEXIST)
			break;
		s[0] = '/';
		s++;
	}
	kfree(path);
	return err;
}

<<<<<<< HEAD
static int handle_create(const char *nodename, umode_t mode, struct device *dev)
=======
static int handle_create(const char *nodename, umode_t mode, kuid_t uid,
			 kgid_t gid, struct device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dentry *dentry;
	struct path path;
	int err;

	dentry = kern_path_create(AT_FDCWD, nodename, &path, 0);
	if (dentry == ERR_PTR(-ENOENT)) {
		create_path(nodename);
		dentry = kern_path_create(AT_FDCWD, nodename, &path, 0);
	}
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

<<<<<<< HEAD
	err = vfs_mknod(path.dentry->d_inode,
			dentry, mode, dev->devt);
	if (!err) {
		struct iattr newattrs;

		/* fixup possibly umasked mode */
		newattrs.ia_mode = mode;
		newattrs.ia_valid = ATTR_MODE;
		mutex_lock(&dentry->d_inode->i_mutex);
		notify_change(dentry, &newattrs);
		mutex_unlock(&dentry->d_inode->i_mutex);

		/* mark as kernel-created inode */
		dentry->d_inode->i_private = &thread;
=======
	err = vfs_mknod(&nop_mnt_idmap, d_inode(path.dentry), dentry, mode,
			dev->devt);
	if (!err) {
		struct iattr newattrs;

		newattrs.ia_mode = mode;
		newattrs.ia_uid = uid;
		newattrs.ia_gid = gid;
		newattrs.ia_valid = ATTR_MODE|ATTR_UID|ATTR_GID;
		inode_lock(d_inode(dentry));
		notify_change(&nop_mnt_idmap, dentry, &newattrs, NULL);
		inode_unlock(d_inode(dentry));

		/* mark as kernel-created inode */
		d_inode(dentry)->i_private = &thread;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	done_path_create(&path, dentry);
	return err;
}

static int dev_rmdir(const char *name)
{
	struct path parent;
	struct dentry *dentry;
	int err;

	dentry = kern_path_locked(name, &parent);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);
<<<<<<< HEAD
	if (dentry->d_inode) {
		if (dentry->d_inode->i_private == &thread)
			err = vfs_rmdir(parent.dentry->d_inode, dentry);
=======
	if (d_really_is_positive(dentry)) {
		if (d_inode(dentry)->i_private == &thread)
			err = vfs_rmdir(&nop_mnt_idmap, d_inode(parent.dentry),
					dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else
			err = -EPERM;
	} else {
		err = -ENOENT;
	}
	dput(dentry);
<<<<<<< HEAD
	mutex_unlock(&parent.dentry->d_inode->i_mutex);
=======
	inode_unlock(d_inode(parent.dentry));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	path_put(&parent);
	return err;
}

static int delete_path(const char *nodepath)
{
<<<<<<< HEAD
	const char *path;
=======
	char *path;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = 0;

	path = kstrdup(nodepath, GFP_KERNEL);
	if (!path)
		return -ENOMEM;

	for (;;) {
		char *base;

		base = strrchr(path, '/');
		if (!base)
			break;
		base[0] = '\0';
		err = dev_rmdir(path);
		if (err)
			break;
	}

	kfree(path);
	return err;
}

static int dev_mynode(struct device *dev, struct inode *inode, struct kstat *stat)
{
	/* did we create it */
	if (inode->i_private != &thread)
		return 0;

	/* does the dev_t match */
	if (is_blockdev(dev)) {
		if (!S_ISBLK(stat->mode))
			return 0;
	} else {
		if (!S_ISCHR(stat->mode))
			return 0;
	}
	if (stat->rdev != dev->devt)
		return 0;

	/* ours */
	return 1;
}

static int handle_remove(const char *nodename, struct device *dev)
{
	struct path parent;
	struct dentry *dentry;
<<<<<<< HEAD
	int deleted = 1;
=======
	int deleted = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	dentry = kern_path_locked(nodename, &parent);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

<<<<<<< HEAD
	if (dentry->d_inode) {
		struct kstat stat;
		err = vfs_getattr(parent.mnt, dentry, &stat);
		if (!err && dev_mynode(dev, dentry->d_inode, &stat)) {
=======
	if (d_really_is_positive(dentry)) {
		struct kstat stat;
		struct path p = {.mnt = parent.mnt, .dentry = dentry};
		err = vfs_getattr(&p, &stat, STATX_TYPE | STATX_MODE,
				  AT_STATX_SYNC_AS_STAT);
		if (!err && dev_mynode(dev, d_inode(dentry), &stat)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			struct iattr newattrs;
			/*
			 * before unlinking this node, reset permissions
			 * of possible references like hardlinks
			 */
<<<<<<< HEAD
			newattrs.ia_uid = 0;
			newattrs.ia_gid = 0;
			newattrs.ia_mode = stat.mode & ~0777;
			newattrs.ia_valid =
				ATTR_UID|ATTR_GID|ATTR_MODE;
			mutex_lock(&dentry->d_inode->i_mutex);
			notify_change(dentry, &newattrs);
			mutex_unlock(&dentry->d_inode->i_mutex);
			err = vfs_unlink(parent.dentry->d_inode, dentry);
=======
			newattrs.ia_uid = GLOBAL_ROOT_UID;
			newattrs.ia_gid = GLOBAL_ROOT_GID;
			newattrs.ia_mode = stat.mode & ~0777;
			newattrs.ia_valid =
				ATTR_UID|ATTR_GID|ATTR_MODE;
			inode_lock(d_inode(dentry));
			notify_change(&nop_mnt_idmap, dentry, &newattrs, NULL);
			inode_unlock(d_inode(dentry));
			err = vfs_unlink(&nop_mnt_idmap, d_inode(parent.dentry),
					 dentry, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (!err || err == -ENOENT)
				deleted = 1;
		}
	} else {
		err = -ENOENT;
	}
	dput(dentry);
<<<<<<< HEAD
	mutex_unlock(&parent.dentry->d_inode->i_mutex);
=======
	inode_unlock(d_inode(parent.dentry));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	path_put(&parent);
	if (deleted && strchr(nodename, '/'))
		delete_path(nodename);
	return err;
}

/*
 * If configured, or requested by the commandline, devtmpfs will be
 * auto-mounted after the kernel mounted the root filesystem.
 */
<<<<<<< HEAD
int devtmpfs_mount(const char *mntdir)
=======
int __init devtmpfs_mount(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;

	if (!mount_dev)
		return 0;

	if (!thread)
		return 0;

<<<<<<< HEAD
	err = sys_mount("devtmpfs", (char *)mntdir, "devtmpfs", MS_SILENT, NULL);
	if (err)
		printk(KERN_INFO "devtmpfs: error mounting %i\n", err);
	else
		printk(KERN_INFO "devtmpfs: mounted\n");
	return err;
}

static DECLARE_COMPLETION(setup_done);

static int handle(const char *name, umode_t mode, struct device *dev)
{
	if (mode)
		return handle_create(name, mode, dev);
=======
	err = init_mount("devtmpfs", "dev", "devtmpfs", DEVTMPFS_MFLAGS, NULL);
	if (err)
		pr_info("error mounting %d\n", err);
	else
		pr_info("mounted\n");
	return err;
}

static __initdata DECLARE_COMPLETION(setup_done);

static int handle(const char *name, umode_t mode, kuid_t uid, kgid_t gid,
		  struct device *dev)
{
	if (mode)
		return handle_create(name, mode, uid, gid, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		return handle_remove(name, dev);
}

<<<<<<< HEAD
static int devtmpfsd(void *p)
{
	char options[] = "mode=0755";
	int *err = p;
	*err = sys_unshare(CLONE_NEWNS);
	if (*err)
		goto out;
	*err = sys_mount("devtmpfs", "/", "devtmpfs", MS_SILENT, options);
	if (*err)
		goto out;
	sys_chdir("/.."); /* will traverse into overmounted root */
	sys_chroot(".");
	complete(&setup_done);
=======
static void __noreturn devtmpfs_work_loop(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (1) {
		spin_lock(&req_lock);
		while (requests) {
			struct req *req = requests;
			requests = NULL;
			spin_unlock(&req_lock);
			while (req) {
				struct req *next = req->next;
<<<<<<< HEAD
				req->err = handle(req->name, req->mode, req->dev);
=======
				req->err = handle(req->name, req->mode,
						  req->uid, req->gid, req->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				complete(&req->done);
				req = next;
			}
			spin_lock(&req_lock);
		}
		__set_current_state(TASK_INTERRUPTIBLE);
		spin_unlock(&req_lock);
		schedule();
	}
<<<<<<< HEAD
	return 0;
out:
	complete(&setup_done);
	return *err;
=======
}

static noinline int __init devtmpfs_setup(void *p)
{
	int err;

	err = ksys_unshare(CLONE_NEWNS);
	if (err)
		goto out;
	err = init_mount("devtmpfs", "/", "devtmpfs", DEVTMPFS_MFLAGS, NULL);
	if (err)
		goto out;
	init_chdir("/.."); /* will traverse into overmounted root */
	init_chroot(".");
out:
	*(int *)p = err;
	return err;
}

/*
 * The __ref is because devtmpfs_setup needs to be __init for the routines it
 * calls.  That call is done while devtmpfs_init, which is marked __init,
 * synchronously waits for it to complete.
 */
static int __ref devtmpfsd(void *p)
{
	int err = devtmpfs_setup(p);

	complete(&setup_done);
	if (err)
		return err;
	devtmpfs_work_loop();
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Create devtmpfs instance, driver-core devices will add their device
 * nodes here.
 */
int __init devtmpfs_init(void)
{
<<<<<<< HEAD
	int err = register_filesystem(&dev_fs_type);
	if (err) {
		printk(KERN_ERR "devtmpfs: unable to register devtmpfs "
		       "type %i\n", err);
=======
	char opts[] = "mode=0755";
	int err;

	mnt = vfs_kern_mount(&internal_fs_type, 0, "devtmpfs", opts);
	if (IS_ERR(mnt)) {
		pr_err("unable to create devtmpfs %ld\n", PTR_ERR(mnt));
		return PTR_ERR(mnt);
	}
	err = register_filesystem(&dev_fs_type);
	if (err) {
		pr_err("unable to register devtmpfs type %d\n", err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return err;
	}

	thread = kthread_run(devtmpfsd, &err, "kdevtmpfs");
	if (!IS_ERR(thread)) {
		wait_for_completion(&setup_done);
	} else {
		err = PTR_ERR(thread);
		thread = NULL;
	}

	if (err) {
<<<<<<< HEAD
		printk(KERN_ERR "devtmpfs: unable to create devtmpfs %i\n", err);
		unregister_filesystem(&dev_fs_type);
		return err;
	}

	printk(KERN_INFO "devtmpfs: initialized\n");
=======
		pr_err("unable to create devtmpfs %d\n", err);
		unregister_filesystem(&dev_fs_type);
		thread = NULL;
		return err;
	}

	pr_info("initialized\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
