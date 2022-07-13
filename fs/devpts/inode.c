<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* -*- linux-c -*- --------------------------------------------------------- *
 *
 * linux/fs/devpts/inode.c
 *
 *  Copyright 1998-2004 H. Peter Anvin -- All Rights Reserved
 *
<<<<<<< HEAD
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * ------------------------------------------------------------------------- */

=======
 * ------------------------------------------------------------------------- */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/namei.h>
#include <linux/slab.h>
#include <linux/mount.h>
#include <linux/tty.h>
#include <linux/mutex.h>
#include <linux/magic.h>
#include <linux/idr.h>
#include <linux/devpts_fs.h>
#include <linux/parser.h>
#include <linux/fsnotify.h>
#include <linux/seq_file.h>

#define DEVPTS_DEFAULT_MODE 0600
/*
 * ptmx is a new node in /dev/pts and will be unused in legacy (single-
 * instance) mode. To prevent surprises in user space, set permissions of
 * ptmx to 0. Use 'chmod' or remount with '-o ptmxmode' to set meaningful
 * permissions.
 */
#define DEVPTS_DEFAULT_PTMX_MODE 0000
#define PTMX_MINOR	2

/*
 * sysctl support for setting limits on the number of Unix98 ptys allocated.
 * Otherwise one can eat up all kernel memory by opening /dev/ptmx repeatedly.
 */
static int pty_limit = NR_UNIX98_PTY_DEFAULT;
static int pty_reserve = NR_UNIX98_PTY_RESERVE;
static int pty_limit_min;
static int pty_limit_max = INT_MAX;
<<<<<<< HEAD
static int pty_count;
=======
static atomic_t pty_count = ATOMIC_INIT(0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct ctl_table pty_table[] = {
	{
		.procname	= "max",
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.data		= &pty_limit,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &pty_limit_min,
		.extra2		= &pty_limit_max,
	}, {
		.procname	= "reserve",
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.data		= &pty_reserve,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &pty_limit_min,
		.extra2		= &pty_limit_max,
	}, {
		.procname	= "nr",
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.data		= &pty_count,
		.proc_handler	= proc_dointvec,
	},
<<<<<<< HEAD
	{}
};

static struct ctl_table pty_kern_table[] = {
	{
		.procname	= "pty",
		.mode		= 0555,
		.child		= pty_table,
	},
	{}
};

static struct ctl_table pty_root_table[] = {
	{
		.procname	= "kernel",
		.mode		= 0555,
		.child		= pty_kern_table,
	},
	{}
};

static DEFINE_MUTEX(allocated_ptys_lock);

static struct vfsmount *devpts_mnt;

struct pts_mount_opts {
	int setuid;
	int setgid;
	uid_t   uid;
	gid_t   gid;
	umode_t mode;
	umode_t ptmxmode;
	int newinstance;
=======
};

struct pts_mount_opts {
	int setuid;
	int setgid;
	kuid_t   uid;
	kgid_t   gid;
	umode_t mode;
	umode_t ptmxmode;
	int reserve;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int max;
};

enum {
	Opt_uid, Opt_gid, Opt_mode, Opt_ptmxmode, Opt_newinstance,  Opt_max,
	Opt_err
};

static const match_table_t tokens = {
	{Opt_uid, "uid=%u"},
	{Opt_gid, "gid=%u"},
	{Opt_mode, "mode=%o"},
<<<<<<< HEAD
#ifdef CONFIG_DEVPTS_MULTIPLE_INSTANCES
	{Opt_ptmxmode, "ptmxmode=%o"},
	{Opt_newinstance, "newinstance"},
	{Opt_max, "max=%d"},
#endif
=======
	{Opt_ptmxmode, "ptmxmode=%o"},
	{Opt_newinstance, "newinstance"},
	{Opt_max, "max=%d"},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{Opt_err, NULL}
};

struct pts_fs_info {
	struct ida allocated_ptys;
	struct pts_mount_opts mount_opts;
<<<<<<< HEAD
=======
	struct super_block *sb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct dentry *ptmx_dentry;
};

static inline struct pts_fs_info *DEVPTS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

<<<<<<< HEAD
static inline struct super_block *pts_sb_from_inode(struct inode *inode)
{
#ifdef CONFIG_DEVPTS_MULTIPLE_INSTANCES
	if (inode->i_sb->s_magic == DEVPTS_SUPER_MAGIC)
		return inode->i_sb;
#endif
	return devpts_mnt->mnt_sb;
=======
static int devpts_ptmx_path(struct path *path)
{
	struct super_block *sb;
	int err;

	/* Is a devpts filesystem at "pts" in the same directory? */
	err = path_pts(path);
	if (err)
		return err;

	/* Is the path the root of a devpts filesystem? */
	sb = path->mnt->mnt_sb;
	if ((sb->s_magic != DEVPTS_SUPER_MAGIC) ||
	    (path->mnt->mnt_root != sb->s_root))
		return -ENODEV;

	return 0;
}

/*
 * Try to find a suitable devpts filesystem. We support the following
 * scenarios:
 * - The ptmx device node is located in the same directory as the devpts
 *   mount where the pts device nodes are located.
 *   This is e.g. the case when calling open on the /dev/pts/ptmx device
 *   node when the devpts filesystem is mounted at /dev/pts.
 * - The ptmx device node is located outside the devpts filesystem mount
 *   where the pts device nodes are located. For example, the ptmx device
 *   is a symlink, separate device node, or bind-mount.
 *   A supported scenario is bind-mounting /dev/pts/ptmx to /dev/ptmx and
 *   then calling open on /dev/ptmx. In this case a suitable pts
 *   subdirectory can be found in the common parent directory /dev of the
 *   devpts mount and the ptmx bind-mount, after resolving the /dev/ptmx
 *   bind-mount.
 *   If no suitable pts subdirectory can be found this function will fail.
 *   This is e.g. the case when bind-mounting /dev/pts/ptmx to /ptmx.
 */
struct vfsmount *devpts_mntget(struct file *filp, struct pts_fs_info *fsi)
{
	struct path path;
	int err = 0;

	path = filp->f_path;
	path_get(&path);

	/* Walk upward while the start point is a bind mount of
	 * a single file.
	 */
	while (path.mnt->mnt_root == path.dentry)
		if (follow_up(&path) == 0)
			break;

	/* devpts_ptmx_path() finds a devpts fs or returns an error. */
	if ((path.mnt->mnt_sb->s_magic != DEVPTS_SUPER_MAGIC) ||
	    (DEVPTS_SB(path.mnt->mnt_sb) != fsi))
		err = devpts_ptmx_path(&path);
	dput(path.dentry);
	if (!err) {
		if (DEVPTS_SB(path.mnt->mnt_sb) == fsi)
			return path.mnt;

		err = -ENODEV;
	}

	mntput(path.mnt);
	return ERR_PTR(err);
}

struct pts_fs_info *devpts_acquire(struct file *filp)
{
	struct pts_fs_info *result;
	struct path path;
	struct super_block *sb;

	path = filp->f_path;
	path_get(&path);

	/* Has the devpts filesystem already been found? */
	if (path.mnt->mnt_sb->s_magic != DEVPTS_SUPER_MAGIC) {
		int err;

		err = devpts_ptmx_path(&path);
		if (err) {
			result = ERR_PTR(err);
			goto out;
		}
	}

	/*
	 * pty code needs to hold extra references in case of last /dev/tty close
	 */
	sb = path.mnt->mnt_sb;
	atomic_inc(&sb->s_active);
	result = DEVPTS_SB(sb);

out:
	path_put(&path);
	return result;
}

void devpts_release(struct pts_fs_info *fsi)
{
	deactivate_super(fsi->sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define PARSE_MOUNT	0
#define PARSE_REMOUNT	1

/*
 * parse_mount_options():
<<<<<<< HEAD
 * 	Set @opts to mount options specified in @data. If an option is not
 * 	specified in @data, set it to its default value. The exception is
 * 	'newinstance' option which can only be set/cleared on a mount (i.e.
 * 	cannot be changed during remount).
=======
 *	Set @opts to mount options specified in @data. If an option is not
 *	specified in @data, set it to its default value.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Note: @data may be NULL (in which case all options are set to default).
 */
static int parse_mount_options(char *data, int op, struct pts_mount_opts *opts)
{
	char *p;
<<<<<<< HEAD

	opts->setuid  = 0;
	opts->setgid  = 0;
	opts->uid     = 0;
	opts->gid     = 0;
=======
	kuid_t uid;
	kgid_t gid;

	opts->setuid  = 0;
	opts->setgid  = 0;
	opts->uid     = GLOBAL_ROOT_UID;
	opts->gid     = GLOBAL_ROOT_GID;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	opts->mode    = DEVPTS_DEFAULT_MODE;
	opts->ptmxmode = DEVPTS_DEFAULT_PTMX_MODE;
	opts->max     = NR_UNIX98_PTY_MAX;

<<<<<<< HEAD
	/* newinstance makes sense only on initial mount */
	if (op == PARSE_MOUNT)
		opts->newinstance = 0;
=======
	/* Only allow instances mounted from the initial mount
	 * namespace to tap the reserve pool of ptys.
	 */
	if (op == PARSE_MOUNT)
		opts->reserve =
			(current->nsproxy->mnt_ns == init_task.nsproxy->mnt_ns);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while ((p = strsep(&data, ",")) != NULL) {
		substring_t args[MAX_OPT_ARGS];
		int token;
		int option;

		if (!*p)
			continue;

		token = match_token(p, tokens, args);
		switch (token) {
		case Opt_uid:
			if (match_int(&args[0], &option))
				return -EINVAL;
<<<<<<< HEAD
			opts->uid = option;
=======
			uid = make_kuid(current_user_ns(), option);
			if (!uid_valid(uid))
				return -EINVAL;
			opts->uid = uid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			opts->setuid = 1;
			break;
		case Opt_gid:
			if (match_int(&args[0], &option))
				return -EINVAL;
<<<<<<< HEAD
			opts->gid = option;
=======
			gid = make_kgid(current_user_ns(), option);
			if (!gid_valid(gid))
				return -EINVAL;
			opts->gid = gid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			opts->setgid = 1;
			break;
		case Opt_mode:
			if (match_octal(&args[0], &option))
				return -EINVAL;
			opts->mode = option & S_IALLUGO;
			break;
<<<<<<< HEAD
#ifdef CONFIG_DEVPTS_MULTIPLE_INSTANCES
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case Opt_ptmxmode:
			if (match_octal(&args[0], &option))
				return -EINVAL;
			opts->ptmxmode = option & S_IALLUGO;
			break;
		case Opt_newinstance:
<<<<<<< HEAD
			/* newinstance makes sense only on initial mount */
			if (op == PARSE_MOUNT)
				opts->newinstance = 1;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case Opt_max:
			if (match_int(&args[0], &option) ||
			    option < 0 || option > NR_UNIX98_PTY_MAX)
				return -EINVAL;
			opts->max = option;
			break;
<<<<<<< HEAD
#endif
		default:
			printk(KERN_ERR "devpts: called with bogus options\n");
=======
		default:
			pr_err("called with bogus options\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
	}

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_DEVPTS_MULTIPLE_INSTANCES
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int mknod_ptmx(struct super_block *sb)
{
	int mode;
	int rc = -ENOMEM;
	struct dentry *dentry;
	struct inode *inode;
	struct dentry *root = sb->s_root;
	struct pts_fs_info *fsi = DEVPTS_SB(sb);
	struct pts_mount_opts *opts = &fsi->mount_opts;
<<<<<<< HEAD

	mutex_lock(&root->d_inode->i_mutex);
=======
	kuid_t ptmx_uid = current_fsuid();
	kgid_t ptmx_gid = current_fsgid();

	inode_lock(d_inode(root));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If we have already created ptmx node, return */
	if (fsi->ptmx_dentry) {
		rc = 0;
		goto out;
	}

	dentry = d_alloc_name(root, "ptmx");
	if (!dentry) {
<<<<<<< HEAD
		printk(KERN_NOTICE "Unable to alloc dentry for ptmx node\n");
=======
		pr_err("Unable to alloc dentry for ptmx node\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/*
	 * Create a new 'ptmx' node in this mount of devpts.
	 */
	inode = new_inode(sb);
	if (!inode) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to alloc inode for ptmx node\n");
=======
		pr_err("Unable to alloc inode for ptmx node\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dput(dentry);
		goto out;
	}

	inode->i_ino = 2;
<<<<<<< HEAD
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;

	mode = S_IFCHR|opts->ptmxmode;
	init_special_inode(inode, mode, MKDEV(TTYAUX_MAJOR, 2));
=======
	simple_inode_init_ts(inode);

	mode = S_IFCHR|opts->ptmxmode;
	init_special_inode(inode, mode, MKDEV(TTYAUX_MAJOR, 2));
	inode->i_uid = ptmx_uid;
	inode->i_gid = ptmx_gid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	d_add(dentry, inode);

	fsi->ptmx_dentry = dentry;
	rc = 0;
out:
<<<<<<< HEAD
	mutex_unlock(&root->d_inode->i_mutex);
=======
	inode_unlock(d_inode(root));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static void update_ptmx_mode(struct pts_fs_info *fsi)
{
	struct inode *inode;
	if (fsi->ptmx_dentry) {
<<<<<<< HEAD
		inode = fsi->ptmx_dentry->d_inode;
		inode->i_mode = S_IFCHR|fsi->mount_opts.ptmxmode;
	}
}
#else
static inline void update_ptmx_mode(struct pts_fs_info *fsi)
{
       return;
}
#endif
=======
		inode = d_inode(fsi->ptmx_dentry);
		inode->i_mode = S_IFCHR|fsi->mount_opts.ptmxmode;
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int devpts_remount(struct super_block *sb, int *flags, char *data)
{
	int err;
	struct pts_fs_info *fsi = DEVPTS_SB(sb);
	struct pts_mount_opts *opts = &fsi->mount_opts;

	err = parse_mount_options(data, PARSE_REMOUNT, opts);

	/*
	 * parse_mount_options() restores options to default values
	 * before parsing and may have changed ptmxmode. So, update the
	 * mode in the inode too. Bogus options don't fail the remount,
	 * so do this even on error return.
	 */
	update_ptmx_mode(fsi);

	return err;
}

static int devpts_show_options(struct seq_file *seq, struct dentry *root)
{
	struct pts_fs_info *fsi = DEVPTS_SB(root->d_sb);
	struct pts_mount_opts *opts = &fsi->mount_opts;

	if (opts->setuid)
<<<<<<< HEAD
		seq_printf(seq, ",uid=%u", opts->uid);
	if (opts->setgid)
		seq_printf(seq, ",gid=%u", opts->gid);
	seq_printf(seq, ",mode=%03o", opts->mode);
#ifdef CONFIG_DEVPTS_MULTIPLE_INSTANCES
	seq_printf(seq, ",ptmxmode=%03o", opts->ptmxmode);
	if (opts->max < NR_UNIX98_PTY_MAX)
		seq_printf(seq, ",max=%d", opts->max);
#endif
=======
		seq_printf(seq, ",uid=%u",
			   from_kuid_munged(&init_user_ns, opts->uid));
	if (opts->setgid)
		seq_printf(seq, ",gid=%u",
			   from_kgid_munged(&init_user_ns, opts->gid));
	seq_printf(seq, ",mode=%03o", opts->mode);
	seq_printf(seq, ",ptmxmode=%03o", opts->ptmxmode);
	if (opts->max < NR_UNIX98_PTY_MAX)
		seq_printf(seq, ",max=%d", opts->max);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct super_operations devpts_sops = {
	.statfs		= simple_statfs,
	.remount_fs	= devpts_remount,
	.show_options	= devpts_show_options,
};

<<<<<<< HEAD
static void *new_pts_fs_info(void)
=======
static void *new_pts_fs_info(struct super_block *sb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pts_fs_info *fsi;

	fsi = kzalloc(sizeof(struct pts_fs_info), GFP_KERNEL);
	if (!fsi)
		return NULL;

	ida_init(&fsi->allocated_ptys);
	fsi->mount_opts.mode = DEVPTS_DEFAULT_MODE;
	fsi->mount_opts.ptmxmode = DEVPTS_DEFAULT_PTMX_MODE;
<<<<<<< HEAD
=======
	fsi->sb = sb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return fsi;
}

static int
devpts_fill_super(struct super_block *s, void *data, int silent)
{
	struct inode *inode;
<<<<<<< HEAD

=======
	int error;

	s->s_iflags &= ~SB_I_NODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	s->s_blocksize = 1024;
	s->s_blocksize_bits = 10;
	s->s_magic = DEVPTS_SUPER_MAGIC;
	s->s_op = &devpts_sops;
<<<<<<< HEAD
	s->s_time_gran = 1;

	s->s_fs_info = new_pts_fs_info();
	if (!s->s_fs_info)
		goto fail;

=======
	s->s_d_op = &simple_dentry_operations;
	s->s_time_gran = 1;

	error = -ENOMEM;
	s->s_fs_info = new_pts_fs_info(s);
	if (!s->s_fs_info)
		goto fail;

	error = parse_mount_options(data, PARSE_MOUNT, &DEVPTS_SB(s)->mount_opts);
	if (error)
		goto fail;

	error = -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inode = new_inode(s);
	if (!inode)
		goto fail;
	inode->i_ino = 1;
<<<<<<< HEAD
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
=======
	simple_inode_init_ts(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inode->i_mode = S_IFDIR | S_IRUGO | S_IXUGO | S_IWUSR;
	inode->i_op = &simple_dir_inode_operations;
	inode->i_fop = &simple_dir_operations;
	set_nlink(inode, 2);

	s->s_root = d_make_root(inode);
<<<<<<< HEAD
	if (s->s_root)
		return 0;

	printk(KERN_ERR "devpts: get root dentry failed\n");

fail:
	return -ENOMEM;
}

#ifdef CONFIG_DEVPTS_MULTIPLE_INSTANCES
static int compare_init_pts_sb(struct super_block *s, void *p)
{
	if (devpts_mnt)
		return devpts_mnt->mnt_sb == s;
	return 0;
=======
	if (!s->s_root) {
		pr_err("get root dentry failed\n");
		goto fail;
	}

	error = mknod_ptmx(s);
	if (error)
		goto fail_dput;

	return 0;
fail_dput:
	dput(s->s_root);
	s->s_root = NULL;
fail:
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * devpts_mount()
 *
<<<<<<< HEAD
 *     If the '-o newinstance' mount option was specified, mount a new
 *     (private) instance of devpts.  PTYs created in this instance are
 *     independent of the PTYs in other devpts instances.
 *
 *     If the '-o newinstance' option was not specified, mount/remount the
 *     initial kernel mount of devpts.  This type of mount gives the
 *     legacy, single-instance semantics.
 *
 *     The 'newinstance' option is needed to support multiple namespace
 *     semantics in devpts while preserving backward compatibility of the
 *     current 'single-namespace' semantics. i.e all mounts of devpts
 *     without the 'newinstance' mount option should bind to the initial
 *     kernel mount, like mount_single().
 *
 *     Mounts with 'newinstance' option create a new, private namespace.
 *
 *     NOTE:
 *
 *     For single-mount semantics, devpts cannot use mount_single(),
 *     because mount_single()/sget() find and use the super-block from
 *     the most recent mount of devpts. But that recent mount may be a
 *     'newinstance' mount and mount_single() would pick the newinstance
 *     super-block instead of the initial super-block.
=======
 *     Mount a new (private) instance of devpts.  PTYs created in this
 *     instance are independent of the PTYs in other devpts instances.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static struct dentry *devpts_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
<<<<<<< HEAD
	int error;
	struct pts_mount_opts opts;
	struct super_block *s;

	error = parse_mount_options(data, PARSE_MOUNT, &opts);
	if (error)
		return ERR_PTR(error);

	if (opts.newinstance)
		s = sget(fs_type, NULL, set_anon_super, NULL);
	else
		s = sget(fs_type, compare_init_pts_sb, set_anon_super, NULL);

	if (IS_ERR(s))
		return ERR_CAST(s);

	if (!s->s_root) {
		s->s_flags = flags;
		error = devpts_fill_super(s, data, flags & MS_SILENT ? 1 : 0);
		if (error)
			goto out_undo_sget;
		s->s_flags |= MS_ACTIVE;
	}

	memcpy(&(DEVPTS_SB(s))->mount_opts, &opts, sizeof(opts));

	error = mknod_ptmx(s);
	if (error)
		goto out_undo_sget;

	return dget(s->s_root);

out_undo_sget:
	deactivate_locked_super(s);
	return ERR_PTR(error);
}

#else
/*
 * This supports only the legacy single-instance semantics (no
 * multiple-instance semantics)
 */
static struct dentry *devpts_mount(struct file_system_type *fs_type, int flags,
		const char *dev_name, void *data)
{
	return mount_single(fs_type, flags, data, devpts_fill_super);
}
#endif

=======
	return mount_nodev(fs_type, flags, data, devpts_fill_super);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void devpts_kill_sb(struct super_block *sb)
{
	struct pts_fs_info *fsi = DEVPTS_SB(sb);

<<<<<<< HEAD
	ida_destroy(&fsi->allocated_ptys);
=======
	if (fsi)
		ida_destroy(&fsi->allocated_ptys);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(fsi);
	kill_litter_super(sb);
}

static struct file_system_type devpts_fs_type = {
	.name		= "devpts",
	.mount		= devpts_mount,
	.kill_sb	= devpts_kill_sb,
<<<<<<< HEAD
};
MODULE_ALIAS_FS("devpts");
=======
	.fs_flags	= FS_USERNS_MOUNT,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The normal naming convention is simply /dev/pts/<number>; this conforms
 * to the System V naming convention
 */

<<<<<<< HEAD
int devpts_new_index(struct inode *ptmx_inode)
{
	struct super_block *sb = pts_sb_from_inode(ptmx_inode);
	struct pts_fs_info *fsi = DEVPTS_SB(sb);
	int index;
	int ida_ret;

retry:
	if (!ida_pre_get(&fsi->allocated_ptys, GFP_KERNEL))
		return -ENOMEM;

	mutex_lock(&allocated_ptys_lock);
	if (pty_count >= pty_limit -
			(fsi->mount_opts.newinstance ? pty_reserve : 0)) {
		mutex_unlock(&allocated_ptys_lock);
		return -ENOSPC;
	}

	ida_ret = ida_get_new(&fsi->allocated_ptys, &index);
	if (ida_ret < 0) {
		mutex_unlock(&allocated_ptys_lock);
		if (ida_ret == -EAGAIN)
			goto retry;
		return -EIO;
	}

	if (index >= fsi->mount_opts.max) {
		ida_remove(&fsi->allocated_ptys, index);
		mutex_unlock(&allocated_ptys_lock);
		return -ENOSPC;
	}
	pty_count++;
	mutex_unlock(&allocated_ptys_lock);
	return index;
}

void devpts_kill_index(struct inode *ptmx_inode, int idx)
{
	struct super_block *sb = pts_sb_from_inode(ptmx_inode);
	struct pts_fs_info *fsi = DEVPTS_SB(sb);

	mutex_lock(&allocated_ptys_lock);
	ida_remove(&fsi->allocated_ptys, idx);
	pty_count--;
	mutex_unlock(&allocated_ptys_lock);
}

int devpts_pty_new(struct inode *ptmx_inode, struct tty_struct *tty)
{
	/* tty layer puts index from devpts_new_index() in here */
	int number = tty->index;
	struct tty_driver *driver = tty->driver;
	dev_t device = MKDEV(driver->major, driver->minor_start+number);
	struct dentry *dentry;
	struct super_block *sb = pts_sb_from_inode(ptmx_inode);
	struct inode *inode = new_inode(sb);
	struct dentry *root = sb->s_root;
	struct pts_fs_info *fsi = DEVPTS_SB(sb);
	struct pts_mount_opts *opts = &fsi->mount_opts;
	int ret = 0;
	char s[12];

	/* We're supposed to be given the slave end of a pty */
	BUG_ON(driver->type != TTY_DRIVER_TYPE_PTY);
	BUG_ON(driver->subtype != PTY_TYPE_SLAVE);

	if (!inode)
		return -ENOMEM;

	inode->i_ino = number + 3;
	inode->i_uid = opts->setuid ? opts->uid : current_fsuid();
	inode->i_gid = opts->setgid ? opts->gid : current_fsgid();
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
	init_special_inode(inode, S_IFCHR|opts->mode, device);
	inode->i_private = tty;
	tty->driver_data = inode;

	sprintf(s, "%d", number);

	mutex_lock(&root->d_inode->i_mutex);

	dentry = d_alloc_name(root, s);
	if (dentry) {
		d_add(dentry, inode);
		fsnotify_create(root->d_inode, dentry);
	} else {
		iput(inode);
		ret = -ENOMEM;
	}

	mutex_unlock(&root->d_inode->i_mutex);

	return ret;
}

struct tty_struct *devpts_get_tty(struct inode *pts_inode, int number)
{
	struct dentry *dentry;
	struct tty_struct *tty;

	BUG_ON(pts_inode->i_rdev == MKDEV(TTYAUX_MAJOR, PTMX_MINOR));

	/* Ensure dentry has not been deleted by devpts_pty_kill() */
	dentry = d_find_alias(pts_inode);
	if (!dentry)
		return NULL;

	tty = NULL;
	if (pts_inode->i_sb->s_magic == DEVPTS_SUPER_MAGIC)
		tty = (struct tty_struct *)pts_inode->i_private;

	dput(dentry);

	return tty;
}

void devpts_pty_kill(struct tty_struct *tty)
{
	struct inode *inode = tty->driver_data;
	struct super_block *sb = pts_sb_from_inode(inode);
	struct dentry *root = sb->s_root;
	struct dentry *dentry;

	BUG_ON(inode->i_rdev == MKDEV(TTYAUX_MAJOR, PTMX_MINOR));

	mutex_lock(&root->d_inode->i_mutex);

	dentry = d_find_alias(inode);

	drop_nlink(inode);
	d_delete(dentry);
	dput(dentry);	/* d_alloc_name() in devpts_pty_new() */
	dput(dentry);		/* d_find_alias above */

	mutex_unlock(&root->d_inode->i_mutex);
=======
int devpts_new_index(struct pts_fs_info *fsi)
{
	int index = -ENOSPC;

	if (atomic_inc_return(&pty_count) >= (pty_limit -
			  (fsi->mount_opts.reserve ? 0 : pty_reserve)))
		goto out;

	index = ida_alloc_max(&fsi->allocated_ptys, fsi->mount_opts.max - 1,
			GFP_KERNEL);

out:
	if (index < 0)
		atomic_dec(&pty_count);
	return index;
}

void devpts_kill_index(struct pts_fs_info *fsi, int idx)
{
	ida_free(&fsi->allocated_ptys, idx);
	atomic_dec(&pty_count);
}

/**
 * devpts_pty_new -- create a new inode in /dev/pts/
 * @fsi: Filesystem info for this instance.
 * @index: used as a name of the node
 * @priv: what's given back by devpts_get_priv
 *
 * The dentry for the created inode is returned.
 * Remove it from /dev/pts/ with devpts_pty_kill().
 */
struct dentry *devpts_pty_new(struct pts_fs_info *fsi, int index, void *priv)
{
	struct dentry *dentry;
	struct super_block *sb = fsi->sb;
	struct inode *inode;
	struct dentry *root;
	struct pts_mount_opts *opts;
	char s[12];

	root = sb->s_root;
	opts = &fsi->mount_opts;

	inode = new_inode(sb);
	if (!inode)
		return ERR_PTR(-ENOMEM);

	inode->i_ino = index + 3;
	inode->i_uid = opts->setuid ? opts->uid : current_fsuid();
	inode->i_gid = opts->setgid ? opts->gid : current_fsgid();
	simple_inode_init_ts(inode);
	init_special_inode(inode, S_IFCHR|opts->mode, MKDEV(UNIX98_PTY_SLAVE_MAJOR, index));

	sprintf(s, "%d", index);

	dentry = d_alloc_name(root, s);
	if (dentry) {
		dentry->d_fsdata = priv;
		d_add(dentry, inode);
		fsnotify_create(d_inode(root), dentry);
	} else {
		iput(inode);
		dentry = ERR_PTR(-ENOMEM);
	}

	return dentry;
}

/**
 * devpts_get_priv -- get private data for a slave
 * @dentry: dentry of the slave
 *
 * Returns whatever was passed as priv in devpts_pty_new for a given inode.
 */
void *devpts_get_priv(struct dentry *dentry)
{
	if (dentry->d_sb->s_magic != DEVPTS_SUPER_MAGIC)
		return NULL;
	return dentry->d_fsdata;
}

/**
 * devpts_pty_kill -- remove inode form /dev/pts/
 * @dentry: dentry of the slave to be removed
 *
 * This is an inverse operation of devpts_pty_new.
 */
void devpts_pty_kill(struct dentry *dentry)
{
	WARN_ON_ONCE(dentry->d_sb->s_magic != DEVPTS_SUPER_MAGIC);

	dentry->d_fsdata = NULL;
	drop_nlink(dentry->d_inode);
	d_drop(dentry);
	fsnotify_unlink(d_inode(dentry->d_parent), dentry);
	dput(dentry);	/* d_alloc_name() in devpts_pty_new() */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init init_devpts_fs(void)
{
	int err = register_filesystem(&devpts_fs_type);
<<<<<<< HEAD
	struct ctl_table_header *table;

	if (!err) {
		table = register_sysctl_table(pty_root_table);
		devpts_mnt = kern_mount(&devpts_fs_type);
		if (IS_ERR(devpts_mnt)) {
			err = PTR_ERR(devpts_mnt);
			unregister_filesystem(&devpts_fs_type);
			unregister_sysctl_table(table);
		}
=======
	if (!err) {
		register_sysctl("kernel/pty", pty_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return err;
}
module_init(init_devpts_fs)
