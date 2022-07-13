<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * fs/proc_namespace.c - handling of /proc/<pid>/{mounts,mountinfo,mountstats}
 *
 * In fact, that's a piece of procfs; it's *almost* isolated from
 * the rest of fs/proc, but has rather close relationships with
 * fs/namespace.c, thus here instead of fs/proc
 *
 */
#include <linux/mnt_namespace.h>
#include <linux/nsproxy.h>
#include <linux/security.h>
#include <linux/fs_struct.h>
<<<<<<< HEAD
=======
#include <linux/sched/task.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "proc/internal.h" /* only for get_proc_task() in ->open() */

#include "pnode.h"
#include "internal.h"

<<<<<<< HEAD
static unsigned mounts_poll(struct file *file, poll_table *wait)
{
	struct proc_mounts *p = proc_mounts(file->private_data);
	struct mnt_namespace *ns = p->ns;
	unsigned res = POLLIN | POLLRDNORM;

	poll_wait(file, &p->ns->poll, wait);

	br_read_lock(&vfsmount_lock);
	if (p->m.poll_event != ns->event) {
		p->m.poll_event = ns->event;
		res |= POLLERR | POLLPRI;
	}
	br_read_unlock(&vfsmount_lock);
=======
static __poll_t mounts_poll(struct file *file, poll_table *wait)
{
	struct seq_file *m = file->private_data;
	struct proc_mounts *p = m->private;
	struct mnt_namespace *ns = p->ns;
	__poll_t res = EPOLLIN | EPOLLRDNORM;
	int event;

	poll_wait(file, &p->ns->poll, wait);

	event = READ_ONCE(ns->event);
	if (m->poll_event != event) {
		m->poll_event = event;
		res |= EPOLLERR | EPOLLPRI;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return res;
}

<<<<<<< HEAD
struct proc_fs_info {
=======
struct proc_fs_opts {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int flag;
	const char *str;
};

static int show_sb_opts(struct seq_file *m, struct super_block *sb)
{
<<<<<<< HEAD
	static const struct proc_fs_info fs_info[] = {
		{ MS_SYNCHRONOUS, ",sync" },
		{ MS_DIRSYNC, ",dirsync" },
		{ MS_MANDLOCK, ",mand" },
		{ 0, NULL }
	};
	const struct proc_fs_info *fs_infop;

	for (fs_infop = fs_info; fs_infop->flag; fs_infop++) {
=======
	static const struct proc_fs_opts fs_opts[] = {
		{ SB_SYNCHRONOUS, ",sync" },
		{ SB_DIRSYNC, ",dirsync" },
		{ SB_MANDLOCK, ",mand" },
		{ SB_LAZYTIME, ",lazytime" },
		{ 0, NULL }
	};
	const struct proc_fs_opts *fs_infop;

	for (fs_infop = fs_opts; fs_infop->flag; fs_infop++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (sb->s_flags & fs_infop->flag)
			seq_puts(m, fs_infop->str);
	}

	return security_sb_show_options(m, sb);
}

static void show_mnt_opts(struct seq_file *m, struct vfsmount *mnt)
{
<<<<<<< HEAD
	static const struct proc_fs_info mnt_info[] = {
=======
	static const struct proc_fs_opts mnt_opts[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		{ MNT_NOSUID, ",nosuid" },
		{ MNT_NODEV, ",nodev" },
		{ MNT_NOEXEC, ",noexec" },
		{ MNT_NOATIME, ",noatime" },
		{ MNT_NODIRATIME, ",nodiratime" },
		{ MNT_RELATIME, ",relatime" },
<<<<<<< HEAD
		{ 0, NULL }
	};
	const struct proc_fs_info *fs_infop;

	for (fs_infop = mnt_info; fs_infop->flag; fs_infop++) {
		if (mnt->mnt_flags & fs_infop->flag)
			seq_puts(m, fs_infop->str);
	}
=======
		{ MNT_NOSYMFOLLOW, ",nosymfollow" },
		{ 0, NULL }
	};
	const struct proc_fs_opts *fs_infop;

	for (fs_infop = mnt_opts; fs_infop->flag; fs_infop++) {
		if (mnt->mnt_flags & fs_infop->flag)
			seq_puts(m, fs_infop->str);
	}

	if (is_idmapped_mnt(mnt))
		seq_puts(m, ",idmapped");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void mangle(struct seq_file *m, const char *s)
{
<<<<<<< HEAD
	seq_escape(m, s, " \t\n\\");
=======
	seq_escape(m, s, " \t\n\\#");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void show_type(struct seq_file *m, struct super_block *sb)
{
	mangle(m, sb->s_type->name);
<<<<<<< HEAD
	if (sb->s_subtype && sb->s_subtype[0]) {
=======
	if (sb->s_subtype) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		seq_putc(m, '.');
		mangle(m, sb->s_subtype);
	}
}

static int show_vfsmnt(struct seq_file *m, struct vfsmount *mnt)
{
<<<<<<< HEAD
	struct mount *r = real_mount(mnt);
	int err = 0;
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };
	struct super_block *sb = mnt_path.dentry->d_sb;
=======
	struct proc_mounts *p = m->private;
	struct mount *r = real_mount(mnt);
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };
	struct super_block *sb = mnt_path.dentry->d_sb;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sb->s_op->show_devname) {
		err = sb->s_op->show_devname(m, mnt_path.dentry);
		if (err)
			goto out;
	} else {
		mangle(m, r->mnt_devname ? r->mnt_devname : "none");
	}
	seq_putc(m, ' ');
<<<<<<< HEAD
	seq_path(m, &mnt_path, " \t\n\\");
=======
	/* mountpoints outside of chroot jail will give SEQ_SKIP on this */
	err = seq_path_root(m, &mnt_path, &p->root, " \t\n\\");
	if (err)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	seq_putc(m, ' ');
	show_type(m, sb);
	seq_puts(m, __mnt_is_readonly(mnt) ? " ro" : " rw");
	err = show_sb_opts(m, sb);
	if (err)
		goto out;
	show_mnt_opts(m, mnt);
<<<<<<< HEAD
	if (sb->s_op->show_options2)
			err = sb->s_op->show_options2(mnt, m, mnt_path.dentry);
	else if (sb->s_op->show_options)
=======
	if (sb->s_op->show_options)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = sb->s_op->show_options(m, mnt_path.dentry);
	seq_puts(m, " 0 0\n");
out:
	return err;
}

static int show_mountinfo(struct seq_file *m, struct vfsmount *mnt)
{
<<<<<<< HEAD
	struct proc_mounts *p = proc_mounts(m);
	struct mount *r = real_mount(mnt);
	struct super_block *sb = mnt->mnt_sb;
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };
	struct path root = p->root;
	int err = 0;

	seq_printf(m, "%i %i %u:%u ", r->mnt_id, r->mnt_parent->mnt_id,
		   MAJOR(sb->s_dev), MINOR(sb->s_dev));
	if (sb->s_op->show_path)
		err = sb->s_op->show_path(m, mnt->mnt_root);
	else
		seq_dentry(m, mnt->mnt_root, " \t\n\\");
=======
	struct proc_mounts *p = m->private;
	struct mount *r = real_mount(mnt);
	struct super_block *sb = mnt->mnt_sb;
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };
	int err;

	seq_printf(m, "%i %i %u:%u ", r->mnt_id, r->mnt_parent->mnt_id,
		   MAJOR(sb->s_dev), MINOR(sb->s_dev));
	err = show_path(m, mnt->mnt_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out;
	seq_putc(m, ' ');

	/* mountpoints outside of chroot jail will give SEQ_SKIP on this */
<<<<<<< HEAD
	err = seq_path_root(m, &mnt_path, &root, " \t\n\\");
=======
	err = seq_path_root(m, &mnt_path, &p->root, " \t\n\\");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out;

	seq_puts(m, mnt->mnt_flags & MNT_READONLY ? " ro" : " rw");
	show_mnt_opts(m, mnt);

	/* Tagged fields ("foo:X" or "bar") */
	if (IS_MNT_SHARED(r))
		seq_printf(m, " shared:%i", r->mnt_group_id);
	if (IS_MNT_SLAVE(r)) {
		int master = r->mnt_master->mnt_group_id;
		int dom = get_dominating_id(r, &p->root);
		seq_printf(m, " master:%i", master);
		if (dom && dom != master)
			seq_printf(m, " propagate_from:%i", dom);
	}
	if (IS_MNT_UNBINDABLE(r))
		seq_puts(m, " unbindable");

	/* Filesystem specific data */
	seq_puts(m, " - ");
	show_type(m, sb);
	seq_putc(m, ' ');
<<<<<<< HEAD
	if (sb->s_op->show_devname)
		err = sb->s_op->show_devname(m, mnt->mnt_root);
	else
		mangle(m, r->mnt_devname ? r->mnt_devname : "none");
	if (err)
		goto out;
	seq_puts(m, sb->s_flags & MS_RDONLY ? " ro" : " rw");
	err = show_sb_opts(m, sb);
	if (err)
		goto out;
	if (sb->s_op->show_options2) {
		err = sb->s_op->show_options2(mnt, m, mnt->mnt_root);
	} else if (sb->s_op->show_options)
=======
	if (sb->s_op->show_devname) {
		err = sb->s_op->show_devname(m, mnt->mnt_root);
		if (err)
			goto out;
	} else {
		mangle(m, r->mnt_devname ? r->mnt_devname : "none");
	}
	seq_puts(m, sb_rdonly(sb) ? " ro" : " rw");
	err = show_sb_opts(m, sb);
	if (err)
		goto out;
	if (sb->s_op->show_options)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = sb->s_op->show_options(m, mnt->mnt_root);
	seq_putc(m, '\n');
out:
	return err;
}

static int show_vfsstat(struct seq_file *m, struct vfsmount *mnt)
{
<<<<<<< HEAD
	struct mount *r = real_mount(mnt);
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };
	struct super_block *sb = mnt_path.dentry->d_sb;
	int err = 0;
=======
	struct proc_mounts *p = m->private;
	struct mount *r = real_mount(mnt);
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };
	struct super_block *sb = mnt_path.dentry->d_sb;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* device */
	if (sb->s_op->show_devname) {
		seq_puts(m, "device ");
		err = sb->s_op->show_devname(m, mnt_path.dentry);
<<<<<<< HEAD
=======
		if (err)
			goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (r->mnt_devname) {
			seq_puts(m, "device ");
			mangle(m, r->mnt_devname);
		} else
			seq_puts(m, "no device");
	}

	/* mount point */
	seq_puts(m, " mounted on ");
<<<<<<< HEAD
	seq_path(m, &mnt_path, " \t\n\\");
=======
	/* mountpoints outside of chroot jail will give SEQ_SKIP on this */
	err = seq_path_root(m, &mnt_path, &p->root, " \t\n\\");
	if (err)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	seq_putc(m, ' ');

	/* file system type */
	seq_puts(m, "with fstype ");
	show_type(m, sb);

	/* optional statistics */
	if (sb->s_op->show_stats) {
		seq_putc(m, ' ');
<<<<<<< HEAD
		if (!err)
			err = sb->s_op->show_stats(m, mnt_path.dentry);
	}

	seq_putc(m, '\n');
=======
		err = sb->s_op->show_stats(m, mnt_path.dentry);
	}

	seq_putc(m, '\n');
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int mounts_open_common(struct inode *inode, struct file *file,
			      int (*show)(struct seq_file *, struct vfsmount *))
{
	struct task_struct *task = get_proc_task(inode);
	struct nsproxy *nsp;
	struct mnt_namespace *ns = NULL;
	struct path root;
	struct proc_mounts *p;
<<<<<<< HEAD
=======
	struct seq_file *m;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret = -EINVAL;

	if (!task)
		goto err;

<<<<<<< HEAD
	rcu_read_lock();
	nsp = task_nsproxy(task);
	if (!nsp) {
		rcu_read_unlock();
=======
	task_lock(task);
	nsp = task->nsproxy;
	if (!nsp || !nsp->mnt_ns) {
		task_unlock(task);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_task_struct(task);
		goto err;
	}
	ns = nsp->mnt_ns;
<<<<<<< HEAD
	if (!ns) {
		rcu_read_unlock();
		put_task_struct(task);
		goto err;
	}
	get_mnt_ns(ns);
	rcu_read_unlock();
	task_lock(task);
=======
	get_mnt_ns(ns);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!task->fs) {
		task_unlock(task);
		put_task_struct(task);
		ret = -ENOENT;
		goto err_put_ns;
	}
	get_fs_root(task->fs, &root);
	task_unlock(task);
	put_task_struct(task);

<<<<<<< HEAD
	ret = -ENOMEM;
	p = kmalloc(sizeof(struct proc_mounts), GFP_KERNEL);
	if (!p)
		goto err_put_path;

	file->private_data = &p->m;
	ret = seq_open(file, &mounts_op);
	if (ret)
		goto err_free;

	p->ns = ns;
	p->root = root;
	p->m.poll_event = ns->event;
=======
	ret = seq_open_private(file, &mounts_op, sizeof(struct proc_mounts));
	if (ret)
		goto err_put_path;

	m = file->private_data;
	m->poll_event = ns->event;

	p = m->private;
	p->ns = ns;
	p->root = root;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p->show = show;

	return 0;

<<<<<<< HEAD
 err_free:
	kfree(p);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 err_put_path:
	path_put(&root);
 err_put_ns:
	put_mnt_ns(ns);
 err:
	return ret;
}

static int mounts_release(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	struct proc_mounts *p = proc_mounts(file->private_data);
	path_put(&p->root);
	put_mnt_ns(p->ns);
	return seq_release(inode, file);
=======
	struct seq_file *m = file->private_data;
	struct proc_mounts *p = m->private;
	path_put(&p->root);
	put_mnt_ns(p->ns);
	return seq_release_private(inode, file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mounts_open(struct inode *inode, struct file *file)
{
	return mounts_open_common(inode, file, show_vfsmnt);
}

static int mountinfo_open(struct inode *inode, struct file *file)
{
	return mounts_open_common(inode, file, show_mountinfo);
}

static int mountstats_open(struct inode *inode, struct file *file)
{
	return mounts_open_common(inode, file, show_vfsstat);
}

const struct file_operations proc_mounts_operations = {
	.open		= mounts_open,
<<<<<<< HEAD
	.read		= seq_read,
=======
	.read_iter	= seq_read_iter,
	.splice_read	= copy_splice_read,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek		= seq_lseek,
	.release	= mounts_release,
	.poll		= mounts_poll,
};

const struct file_operations proc_mountinfo_operations = {
	.open		= mountinfo_open,
<<<<<<< HEAD
	.read		= seq_read,
=======
	.read_iter	= seq_read_iter,
	.splice_read	= copy_splice_read,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek		= seq_lseek,
	.release	= mounts_release,
	.poll		= mounts_poll,
};

const struct file_operations proc_mountstats_operations = {
	.open		= mountstats_open,
<<<<<<< HEAD
	.read		= seq_read,
=======
	.read_iter	= seq_read_iter,
	.splice_read	= copy_splice_read,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek		= seq_lseek,
	.release	= mounts_release,
};
