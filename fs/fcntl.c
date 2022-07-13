<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/fs/fcntl.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#include <linux/syscalls.h>
#include <linux/init.h>
#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/fs.h>
=======
#include <linux/sched/task.h>
#include <linux/fs.h>
#include <linux/filelock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/capability.h>
#include <linux/dnotify.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/pipe_fs_i.h>
#include <linux/security.h>
#include <linux/ptrace.h>
#include <linux/signal.h>
#include <linux/rcupdate.h>
#include <linux/pid_namespace.h>
<<<<<<< HEAD

#include <asm/poll.h>
#include <asm/siginfo.h>
#include <asm/uaccess.h>

void set_close_on_exec(unsigned int fd, int flag)
{
	struct files_struct *files = current->files;
	struct fdtable *fdt;
	spin_lock(&files->file_lock);
	fdt = files_fdtable(files);
	if (flag)
		__set_close_on_exec(fd, fdt);
	else
		__clear_close_on_exec(fd, fdt);
	spin_unlock(&files->file_lock);
}

static bool get_close_on_exec(unsigned int fd)
{
	struct files_struct *files = current->files;
	struct fdtable *fdt;
	bool res;
	rcu_read_lock();
	fdt = files_fdtable(files);
	res = close_on_exec(fd, fdt);
	rcu_read_unlock();
	return res;
}

SYSCALL_DEFINE3(dup3, unsigned int, oldfd, unsigned int, newfd, int, flags)
{
	int err = -EBADF;
	struct file * file, *tofree;
	struct files_struct * files = current->files;
	struct fdtable *fdt;

	if ((flags & ~O_CLOEXEC) != 0)
		return -EINVAL;

	if (unlikely(oldfd == newfd))
		return -EINVAL;

	spin_lock(&files->file_lock);
	err = expand_files(files, newfd);
	file = fcheck(oldfd);
	if (unlikely(!file))
		goto Ebadf;
	if (unlikely(err < 0)) {
		if (err == -EMFILE)
			goto Ebadf;
		goto out_unlock;
	}
	/*
	 * We need to detect attempts to do dup2() over allocated but still
	 * not finished descriptor.  NB: OpenBSD avoids that at the price of
	 * extra work in their equivalent of fget() - they insert struct
	 * file immediately after grabbing descriptor, mark it larval if
	 * more work (e.g. actual opening) is needed and make sure that
	 * fget() treats larval files as absent.  Potentially interesting,
	 * but while extra work in fget() is trivial, locking implications
	 * and amount of surgery on open()-related paths in VFS are not.
	 * FreeBSD fails with -EBADF in the same situation, NetBSD "solution"
	 * deadlocks in rather amusing ways, AFAICS.  All of that is out of
	 * scope of POSIX or SUS, since neither considers shared descriptor
	 * tables and this condition does not arise without those.
	 */
	err = -EBUSY;
	fdt = files_fdtable(files);
	tofree = fdt->fd[newfd];
	if (!tofree && fd_is_open(newfd, fdt))
		goto out_unlock;
	get_file(file);
	rcu_assign_pointer(fdt->fd[newfd], file);
	__set_open_fd(newfd, fdt);
	if (flags & O_CLOEXEC)
		__set_close_on_exec(newfd, fdt);
	else
		__clear_close_on_exec(newfd, fdt);
	spin_unlock(&files->file_lock);

	if (tofree)
		filp_close(tofree, files);

	return newfd;

Ebadf:
	err = -EBADF;
out_unlock:
	spin_unlock(&files->file_lock);
	return err;
}

SYSCALL_DEFINE2(dup2, unsigned int, oldfd, unsigned int, newfd)
{
	if (unlikely(newfd == oldfd)) { /* corner case */
		struct files_struct *files = current->files;
		int retval = oldfd;

		rcu_read_lock();
		if (!fcheck_files(files, oldfd))
			retval = -EBADF;
		rcu_read_unlock();
		return retval;
	}
	return sys_dup3(oldfd, newfd, 0);
}

SYSCALL_DEFINE1(dup, unsigned int, fildes)
{
	int ret = -EBADF;
	struct file *file = fget_raw(fildes);

	if (file) {
		ret = get_unused_fd();
		if (ret >= 0)
			fd_install(ret, file);
		else
			fput(file);
	}
	return ret;
}

#define SETFL_MASK (O_APPEND | O_NONBLOCK | O_NDELAY | O_DIRECT | O_NOATIME)

static int setfl(int fd, struct file * filp, unsigned long arg)
{
	struct inode * inode = filp->f_path.dentry->d_inode;
=======
#include <linux/user_namespace.h>
#include <linux/memfd.h>
#include <linux/compat.h>
#include <linux/mount.h>
#include <linux/rw_hint.h>

#include <linux/poll.h>
#include <asm/siginfo.h>
#include <linux/uaccess.h>

#define SETFL_MASK (O_APPEND | O_NONBLOCK | O_NDELAY | O_DIRECT | O_NOATIME)

static int setfl(int fd, struct file * filp, unsigned int arg)
{
	struct inode * inode = file_inode(filp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int error = 0;

	/*
	 * O_APPEND cannot be cleared if the file is marked as append-only
	 * and the file is open for write.
	 */
	if (((arg ^ filp->f_flags) & O_APPEND) && IS_APPEND(inode))
		return -EPERM;

	/* O_NOATIME can only be set by the owner or superuser */
	if ((arg & O_NOATIME) && !(filp->f_flags & O_NOATIME))
<<<<<<< HEAD
		if (!inode_owner_or_capable(inode))
=======
		if (!inode_owner_or_capable(file_mnt_idmap(filp), inode))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EPERM;

	/* required for strict SunOS emulation */
	if (O_NONBLOCK != O_NDELAY)
	       if (arg & O_NDELAY)
		   arg |= O_NONBLOCK;

<<<<<<< HEAD
	if (arg & O_DIRECT) {
		if (!filp->f_mapping || !filp->f_mapping->a_ops ||
			!filp->f_mapping->a_ops->direct_IO)
				return -EINVAL;
	}

	if (filp->f_op && filp->f_op->check_flags)
=======
	/* Pipe packetized mode is controlled by O_DIRECT flag */
	if (!S_ISFIFO(inode->i_mode) &&
	    (arg & O_DIRECT) &&
	    !(filp->f_mode & FMODE_CAN_ODIRECT))
		return -EINVAL;

	if (filp->f_op->check_flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		error = filp->f_op->check_flags(arg);
	if (error)
		return error;

	/*
	 * ->fasync() is responsible for setting the FASYNC bit.
	 */
<<<<<<< HEAD
	if (((arg ^ filp->f_flags) & FASYNC) && filp->f_op &&
			filp->f_op->fasync) {
=======
	if (((arg ^ filp->f_flags) & FASYNC) && filp->f_op->fasync) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		error = filp->f_op->fasync(fd, filp, (arg & FASYNC) != 0);
		if (error < 0)
			goto out;
		if (error > 0)
			error = 0;
	}
	spin_lock(&filp->f_lock);
	filp->f_flags = (arg & SETFL_MASK) | (filp->f_flags & ~SETFL_MASK);
<<<<<<< HEAD
=======
	filp->f_iocb_flags = iocb_flags(filp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&filp->f_lock);

 out:
	return error;
}

static void f_modown(struct file *filp, struct pid *pid, enum pid_type type,
                     int force)
{
	write_lock_irq(&filp->f_owner.lock);
	if (force || !filp->f_owner.pid) {
		put_pid(filp->f_owner.pid);
		filp->f_owner.pid = get_pid(pid);
		filp->f_owner.pid_type = type;

		if (pid) {
			const struct cred *cred = current_cred();
			filp->f_owner.uid = cred->uid;
			filp->f_owner.euid = cred->euid;
		}
	}
	write_unlock_irq(&filp->f_owner.lock);
}

<<<<<<< HEAD
int __f_setown(struct file *filp, struct pid *pid, enum pid_type type,
		int force)
{
	int err;

	err = security_file_set_fowner(filp);
	if (err)
		return err;

	f_modown(filp, pid, type, force);
	return 0;
}
EXPORT_SYMBOL(__f_setown);

int f_setown(struct file *filp, unsigned long arg, int force)
{
	enum pid_type type;
	struct pid *pid;
	int who = arg;
	int result;
	type = PIDTYPE_PID;
	if (who < 0) {
		type = PIDTYPE_PGID;
		who = -who;
	}
	rcu_read_lock();
	pid = find_vpid(who);
	result = __f_setown(filp, pid, type, force);
	rcu_read_unlock();
	return result;
=======
void __f_setown(struct file *filp, struct pid *pid, enum pid_type type,
		int force)
{
	security_file_set_fowner(filp);
	f_modown(filp, pid, type, force);
}
EXPORT_SYMBOL(__f_setown);

int f_setown(struct file *filp, int who, int force)
{
	enum pid_type type;
	struct pid *pid = NULL;
	int ret = 0;

	type = PIDTYPE_TGID;
	if (who < 0) {
		/* avoid overflow below */
		if (who == INT_MIN)
			return -EINVAL;

		type = PIDTYPE_PGID;
		who = -who;
	}

	rcu_read_lock();
	if (who) {
		pid = find_vpid(who);
		if (!pid)
			ret = -ESRCH;
	}

	if (!ret)
		__f_setown(filp, pid, type, force);
	rcu_read_unlock();

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(f_setown);

void f_delown(struct file *filp)
{
<<<<<<< HEAD
	f_modown(filp, NULL, PIDTYPE_PID, 1);
=======
	f_modown(filp, NULL, PIDTYPE_TGID, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

pid_t f_getown(struct file *filp)
{
<<<<<<< HEAD
	pid_t pid;
	read_lock(&filp->f_owner.lock);
	pid = pid_vnr(filp->f_owner.pid);
	if (filp->f_owner.pid_type == PIDTYPE_PGID)
		pid = -pid;
	read_unlock(&filp->f_owner.lock);
=======
	pid_t pid = 0;

	read_lock_irq(&filp->f_owner.lock);
	rcu_read_lock();
	if (pid_task(filp->f_owner.pid, filp->f_owner.pid_type)) {
		pid = pid_vnr(filp->f_owner.pid);
		if (filp->f_owner.pid_type == PIDTYPE_PGID)
			pid = -pid;
	}
	rcu_read_unlock();
	read_unlock_irq(&filp->f_owner.lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return pid;
}

static int f_setown_ex(struct file *filp, unsigned long arg)
{
<<<<<<< HEAD
	struct f_owner_ex * __user owner_p = (void * __user)arg;
=======
	struct f_owner_ex __user *owner_p = (void __user *)arg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct f_owner_ex owner;
	struct pid *pid;
	int type;
	int ret;

	ret = copy_from_user(&owner, owner_p, sizeof(owner));
	if (ret)
		return -EFAULT;

	switch (owner.type) {
	case F_OWNER_TID:
<<<<<<< HEAD
		type = PIDTYPE_MAX;
		break;

	case F_OWNER_PID:
		type = PIDTYPE_PID;
=======
		type = PIDTYPE_PID;
		break;

	case F_OWNER_PID:
		type = PIDTYPE_TGID;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case F_OWNER_PGRP:
		type = PIDTYPE_PGID;
		break;

	default:
		return -EINVAL;
	}

	rcu_read_lock();
	pid = find_vpid(owner.pid);
	if (owner.pid && !pid)
		ret = -ESRCH;
	else
<<<<<<< HEAD
		ret = __f_setown(filp, pid, type, 1);
=======
		 __f_setown(filp, pid, type, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();

	return ret;
}

static int f_getown_ex(struct file *filp, unsigned long arg)
{
<<<<<<< HEAD
	struct f_owner_ex * __user owner_p = (void * __user)arg;
	struct f_owner_ex owner;
	int ret = 0;

	read_lock(&filp->f_owner.lock);
	owner.pid = pid_vnr(filp->f_owner.pid);
	switch (filp->f_owner.pid_type) {
	case PIDTYPE_MAX:
		owner.type = F_OWNER_TID;
		break;

	case PIDTYPE_PID:
=======
	struct f_owner_ex __user *owner_p = (void __user *)arg;
	struct f_owner_ex owner = {};
	int ret = 0;

	read_lock_irq(&filp->f_owner.lock);
	rcu_read_lock();
	if (pid_task(filp->f_owner.pid, filp->f_owner.pid_type))
		owner.pid = pid_vnr(filp->f_owner.pid);
	rcu_read_unlock();
	switch (filp->f_owner.pid_type) {
	case PIDTYPE_PID:
		owner.type = F_OWNER_TID;
		break;

	case PIDTYPE_TGID:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		owner.type = F_OWNER_PID;
		break;

	case PIDTYPE_PGID:
		owner.type = F_OWNER_PGRP;
		break;

	default:
		WARN_ON(1);
		ret = -EINVAL;
		break;
	}
<<<<<<< HEAD
	read_unlock(&filp->f_owner.lock);
=======
	read_unlock_irq(&filp->f_owner.lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!ret) {
		ret = copy_to_user(owner_p, &owner, sizeof(owner));
		if (ret)
			ret = -EFAULT;
	}
	return ret;
}

<<<<<<< HEAD
static long do_fcntl(int fd, unsigned int cmd, unsigned long arg,
		struct file *filp)
{
=======
#ifdef CONFIG_CHECKPOINT_RESTORE
static int f_getowner_uids(struct file *filp, unsigned long arg)
{
	struct user_namespace *user_ns = current_user_ns();
	uid_t __user *dst = (void __user *)arg;
	uid_t src[2];
	int err;

	read_lock_irq(&filp->f_owner.lock);
	src[0] = from_kuid(user_ns, filp->f_owner.uid);
	src[1] = from_kuid(user_ns, filp->f_owner.euid);
	read_unlock_irq(&filp->f_owner.lock);

	err  = put_user(src[0], &dst[0]);
	err |= put_user(src[1], &dst[1]);

	return err;
}
#else
static int f_getowner_uids(struct file *filp, unsigned long arg)
{
	return -EINVAL;
}
#endif

static bool rw_hint_valid(u64 hint)
{
	BUILD_BUG_ON(WRITE_LIFE_NOT_SET != RWH_WRITE_LIFE_NOT_SET);
	BUILD_BUG_ON(WRITE_LIFE_NONE != RWH_WRITE_LIFE_NONE);
	BUILD_BUG_ON(WRITE_LIFE_SHORT != RWH_WRITE_LIFE_SHORT);
	BUILD_BUG_ON(WRITE_LIFE_MEDIUM != RWH_WRITE_LIFE_MEDIUM);
	BUILD_BUG_ON(WRITE_LIFE_LONG != RWH_WRITE_LIFE_LONG);
	BUILD_BUG_ON(WRITE_LIFE_EXTREME != RWH_WRITE_LIFE_EXTREME);

	switch (hint) {
	case RWH_WRITE_LIFE_NOT_SET:
	case RWH_WRITE_LIFE_NONE:
	case RWH_WRITE_LIFE_SHORT:
	case RWH_WRITE_LIFE_MEDIUM:
	case RWH_WRITE_LIFE_LONG:
	case RWH_WRITE_LIFE_EXTREME:
		return true;
	default:
		return false;
	}
}

static long fcntl_get_rw_hint(struct file *file, unsigned int cmd,
			      unsigned long arg)
{
	struct inode *inode = file_inode(file);
	u64 __user *argp = (u64 __user *)arg;
	u64 hint = READ_ONCE(inode->i_write_hint);

	if (copy_to_user(argp, &hint, sizeof(*argp)))
		return -EFAULT;
	return 0;
}

static long fcntl_set_rw_hint(struct file *file, unsigned int cmd,
			      unsigned long arg)
{
	struct inode *inode = file_inode(file);
	u64 __user *argp = (u64 __user *)arg;
	u64 hint;

	if (copy_from_user(&hint, argp, sizeof(hint)))
		return -EFAULT;
	if (!rw_hint_valid(hint))
		return -EINVAL;

	WRITE_ONCE(inode->i_write_hint, hint);

	/*
	 * file->f_mapping->host may differ from inode. As an example,
	 * blkdev_open() modifies file->f_mapping.
	 */
	if (file->f_mapping->host != inode)
		WRITE_ONCE(file->f_mapping->host->i_write_hint, hint);

	return 0;
}

static long do_fcntl(int fd, unsigned int cmd, unsigned long arg,
		struct file *filp)
{
	void __user *argp = (void __user *)arg;
	int argi = (int)arg;
	struct flock flock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	long err = -EINVAL;

	switch (cmd) {
	case F_DUPFD:
<<<<<<< HEAD
	case F_DUPFD_CLOEXEC:
		if (arg >= rlimit(RLIMIT_NOFILE))
			break;
		err = alloc_fd(arg, cmd == F_DUPFD_CLOEXEC ? O_CLOEXEC : 0);
		if (err >= 0) {
			get_file(filp);
			fd_install(err, filp);
		}
=======
		err = f_dupfd(argi, filp, 0);
		break;
	case F_DUPFD_CLOEXEC:
		err = f_dupfd(argi, filp, O_CLOEXEC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case F_GETFD:
		err = get_close_on_exec(fd) ? FD_CLOEXEC : 0;
		break;
	case F_SETFD:
		err = 0;
<<<<<<< HEAD
		set_close_on_exec(fd, arg & FD_CLOEXEC);
=======
		set_close_on_exec(fd, argi & FD_CLOEXEC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case F_GETFL:
		err = filp->f_flags;
		break;
	case F_SETFL:
<<<<<<< HEAD
		err = setfl(fd, filp, arg);
=======
		err = setfl(fd, filp, argi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
#if BITS_PER_LONG != 32
	/* 32-bit arches must use fcntl64() */
	case F_OFD_GETLK:
#endif
	case F_GETLK:
<<<<<<< HEAD
		err = fcntl_getlk(filp, cmd, (struct flock __user *) arg);
=======
		if (copy_from_user(&flock, argp, sizeof(flock)))
			return -EFAULT;
		err = fcntl_getlk(filp, cmd, &flock);
		if (!err && copy_to_user(argp, &flock, sizeof(flock)))
			return -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
#if BITS_PER_LONG != 32
	/* 32-bit arches must use fcntl64() */
	case F_OFD_SETLK:
	case F_OFD_SETLKW:
<<<<<<< HEAD
#endif
		/* Fallthrough */
	case F_SETLK:
	case F_SETLKW:
		err = fcntl_setlk(fd, filp, cmd, (struct flock __user *) arg);
=======
		fallthrough;
#endif
	case F_SETLK:
	case F_SETLKW:
		if (copy_from_user(&flock, argp, sizeof(flock)))
			return -EFAULT;
		err = fcntl_setlk(fd, filp, cmd, &flock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case F_GETOWN:
		/*
		 * XXX If f_owner is a process group, the
		 * negative return value will get converted
		 * into an error.  Oops.  If we keep the
		 * current syscall conventions, the only way
		 * to fix this will be in libc.
		 */
		err = f_getown(filp);
		force_successful_syscall_return();
		break;
	case F_SETOWN:
<<<<<<< HEAD
		err = f_setown(filp, arg, 1);
=======
		err = f_setown(filp, argi, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case F_GETOWN_EX:
		err = f_getown_ex(filp, arg);
		break;
	case F_SETOWN_EX:
		err = f_setown_ex(filp, arg);
		break;
<<<<<<< HEAD
=======
	case F_GETOWNER_UIDS:
		err = f_getowner_uids(filp, arg);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case F_GETSIG:
		err = filp->f_owner.signum;
		break;
	case F_SETSIG:
		/* arg == 0 restores default behaviour. */
<<<<<<< HEAD
		if (!valid_signal(arg)) {
			break;
		}
		err = 0;
		filp->f_owner.signum = arg;
=======
		if (!valid_signal(argi)) {
			break;
		}
		err = 0;
		filp->f_owner.signum = argi;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case F_GETLEASE:
		err = fcntl_getlease(filp);
		break;
	case F_SETLEASE:
<<<<<<< HEAD
		err = fcntl_setlease(fd, filp, arg);
		break;
	case F_NOTIFY:
		err = fcntl_dirnotify(fd, filp, arg);
		break;
	case F_SETPIPE_SZ:
	case F_GETPIPE_SZ:
		err = pipe_fcntl(filp, cmd, arg);
=======
		err = fcntl_setlease(fd, filp, argi);
		break;
	case F_NOTIFY:
		err = fcntl_dirnotify(fd, filp, argi);
		break;
	case F_SETPIPE_SZ:
	case F_GETPIPE_SZ:
		err = pipe_fcntl(filp, cmd, argi);
		break;
	case F_ADD_SEALS:
	case F_GET_SEALS:
		err = memfd_fcntl(filp, cmd, argi);
		break;
	case F_GET_RW_HINT:
		err = fcntl_get_rw_hint(filp, cmd, arg);
		break;
	case F_SET_RW_HINT:
		err = fcntl_set_rw_hint(filp, cmd, arg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		break;
	}
	return err;
}

static int check_fcntl_cmd(unsigned cmd)
{
	switch (cmd) {
	case F_DUPFD:
	case F_DUPFD_CLOEXEC:
	case F_GETFD:
	case F_SETFD:
	case F_GETFL:
		return 1;
	}
	return 0;
}

SYSCALL_DEFINE3(fcntl, unsigned int, fd, unsigned int, cmd, unsigned long, arg)
{	
<<<<<<< HEAD
	struct file *filp;
	long err = -EBADF;

	filp = fget_raw(fd);
	if (!filp)
		goto out;

	if (unlikely(filp->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd)) {
			fput(filp);
			goto out;
		}
	}

	err = security_file_fcntl(filp, cmd, arg);
	if (err) {
		fput(filp);
		return err;
	}

	err = do_fcntl(fd, cmd, arg, filp);

 	fput(filp);
=======
	struct fd f = fdget_raw(fd);
	long err = -EBADF;

	if (!f.file)
		goto out;

	if (unlikely(f.file->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd))
			goto out1;
	}

	err = security_file_fcntl(f.file, cmd, arg);
	if (!err)
		err = do_fcntl(fd, cmd, arg, f.file);

out1:
 	fdput(f);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;
}

#if BITS_PER_LONG == 32
SYSCALL_DEFINE3(fcntl64, unsigned int, fd, unsigned int, cmd,
		unsigned long, arg)
{	
<<<<<<< HEAD
	struct file * filp;
	long err;

	err = -EBADF;
	filp = fget_raw(fd);
	if (!filp)
		goto out;

	if (unlikely(filp->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd)) {
			fput(filp);
			goto out;
		}
	}

	err = security_file_fcntl(filp, cmd, arg);
	if (err) {
		fput(filp);
		return err;
	}
	err = -EBADF;
=======
	void __user *argp = (void __user *)arg;
	struct fd f = fdget_raw(fd);
	struct flock64 flock;
	long err = -EBADF;

	if (!f.file)
		goto out;

	if (unlikely(f.file->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd))
			goto out1;
	}

	err = security_file_fcntl(f.file, cmd, arg);
	if (err)
		goto out1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	
	switch (cmd) {
	case F_GETLK64:
	case F_OFD_GETLK:
<<<<<<< HEAD
		err = fcntl_getlk64(filp, cmd, (struct flock64 __user *) arg);
=======
		err = -EFAULT;
		if (copy_from_user(&flock, argp, sizeof(flock)))
			break;
		err = fcntl_getlk64(f.file, cmd, &flock);
		if (!err && copy_to_user(argp, &flock, sizeof(flock)))
			err = -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case F_SETLK64:
	case F_SETLKW64:
	case F_OFD_SETLK:
	case F_OFD_SETLKW:
<<<<<<< HEAD
		err = fcntl_setlk64(fd, filp, cmd,
				(struct flock64 __user *) arg);
		break;
	default:
		err = do_fcntl(fd, cmd, arg, filp);
		break;
	}
	fput(filp);
=======
		err = -EFAULT;
		if (copy_from_user(&flock, argp, sizeof(flock)))
			break;
		err = fcntl_setlk64(fd, f.file, cmd, &flock);
		break;
	default:
		err = do_fcntl(fd, cmd, arg, f.file);
		break;
	}
out1:
	fdput(f);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;
}
#endif

<<<<<<< HEAD
/* Table to convert sigio signal codes into poll band bitmaps */

static const long band_table[NSIGPOLL] = {
	POLLIN | POLLRDNORM,			/* POLL_IN */
	POLLOUT | POLLWRNORM | POLLWRBAND,	/* POLL_OUT */
	POLLIN | POLLRDNORM | POLLMSG,		/* POLL_MSG */
	POLLERR,				/* POLL_ERR */
	POLLPRI | POLLRDBAND,			/* POLL_PRI */
	POLLHUP | POLLERR			/* POLL_HUP */
=======
#ifdef CONFIG_COMPAT
/* careful - don't use anywhere else */
#define copy_flock_fields(dst, src)		\
	(dst)->l_type = (src)->l_type;		\
	(dst)->l_whence = (src)->l_whence;	\
	(dst)->l_start = (src)->l_start;	\
	(dst)->l_len = (src)->l_len;		\
	(dst)->l_pid = (src)->l_pid;

static int get_compat_flock(struct flock *kfl, const struct compat_flock __user *ufl)
{
	struct compat_flock fl;

	if (copy_from_user(&fl, ufl, sizeof(struct compat_flock)))
		return -EFAULT;
	copy_flock_fields(kfl, &fl);
	return 0;
}

static int get_compat_flock64(struct flock *kfl, const struct compat_flock64 __user *ufl)
{
	struct compat_flock64 fl;

	if (copy_from_user(&fl, ufl, sizeof(struct compat_flock64)))
		return -EFAULT;
	copy_flock_fields(kfl, &fl);
	return 0;
}

static int put_compat_flock(const struct flock *kfl, struct compat_flock __user *ufl)
{
	struct compat_flock fl;

	memset(&fl, 0, sizeof(struct compat_flock));
	copy_flock_fields(&fl, kfl);
	if (copy_to_user(ufl, &fl, sizeof(struct compat_flock)))
		return -EFAULT;
	return 0;
}

static int put_compat_flock64(const struct flock *kfl, struct compat_flock64 __user *ufl)
{
	struct compat_flock64 fl;

	BUILD_BUG_ON(sizeof(kfl->l_start) > sizeof(ufl->l_start));
	BUILD_BUG_ON(sizeof(kfl->l_len) > sizeof(ufl->l_len));

	memset(&fl, 0, sizeof(struct compat_flock64));
	copy_flock_fields(&fl, kfl);
	if (copy_to_user(ufl, &fl, sizeof(struct compat_flock64)))
		return -EFAULT;
	return 0;
}
#undef copy_flock_fields

static unsigned int
convert_fcntl_cmd(unsigned int cmd)
{
	switch (cmd) {
	case F_GETLK64:
		return F_GETLK;
	case F_SETLK64:
		return F_SETLK;
	case F_SETLKW64:
		return F_SETLKW;
	}

	return cmd;
}

/*
 * GETLK was successful and we need to return the data, but it needs to fit in
 * the compat structure.
 * l_start shouldn't be too big, unless the original start + end is greater than
 * COMPAT_OFF_T_MAX, in which case the app was asking for trouble, so we return
 * -EOVERFLOW in that case.  l_len could be too big, in which case we just
 * truncate it, and only allow the app to see that part of the conflicting lock
 * that might make sense to it anyway
 */
static int fixup_compat_flock(struct flock *flock)
{
	if (flock->l_start > COMPAT_OFF_T_MAX)
		return -EOVERFLOW;
	if (flock->l_len > COMPAT_OFF_T_MAX)
		flock->l_len = COMPAT_OFF_T_MAX;
	return 0;
}

static long do_compat_fcntl64(unsigned int fd, unsigned int cmd,
			     compat_ulong_t arg)
{
	struct fd f = fdget_raw(fd);
	struct flock flock;
	long err = -EBADF;

	if (!f.file)
		return err;

	if (unlikely(f.file->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd))
			goto out_put;
	}

	err = security_file_fcntl(f.file, cmd, arg);
	if (err)
		goto out_put;

	switch (cmd) {
	case F_GETLK:
		err = get_compat_flock(&flock, compat_ptr(arg));
		if (err)
			break;
		err = fcntl_getlk(f.file, convert_fcntl_cmd(cmd), &flock);
		if (err)
			break;
		err = fixup_compat_flock(&flock);
		if (!err)
			err = put_compat_flock(&flock, compat_ptr(arg));
		break;
	case F_GETLK64:
	case F_OFD_GETLK:
		err = get_compat_flock64(&flock, compat_ptr(arg));
		if (err)
			break;
		err = fcntl_getlk(f.file, convert_fcntl_cmd(cmd), &flock);
		if (!err)
			err = put_compat_flock64(&flock, compat_ptr(arg));
		break;
	case F_SETLK:
	case F_SETLKW:
		err = get_compat_flock(&flock, compat_ptr(arg));
		if (err)
			break;
		err = fcntl_setlk(fd, f.file, convert_fcntl_cmd(cmd), &flock);
		break;
	case F_SETLK64:
	case F_SETLKW64:
	case F_OFD_SETLK:
	case F_OFD_SETLKW:
		err = get_compat_flock64(&flock, compat_ptr(arg));
		if (err)
			break;
		err = fcntl_setlk(fd, f.file, convert_fcntl_cmd(cmd), &flock);
		break;
	default:
		err = do_fcntl(fd, cmd, arg, f.file);
		break;
	}
out_put:
	fdput(f);
	return err;
}

COMPAT_SYSCALL_DEFINE3(fcntl64, unsigned int, fd, unsigned int, cmd,
		       compat_ulong_t, arg)
{
	return do_compat_fcntl64(fd, cmd, arg);
}

COMPAT_SYSCALL_DEFINE3(fcntl, unsigned int, fd, unsigned int, cmd,
		       compat_ulong_t, arg)
{
	switch (cmd) {
	case F_GETLK64:
	case F_SETLK64:
	case F_SETLKW64:
	case F_OFD_GETLK:
	case F_OFD_SETLK:
	case F_OFD_SETLKW:
		return -EINVAL;
	}
	return do_compat_fcntl64(fd, cmd, arg);
}
#endif

/* Table to convert sigio signal codes into poll band bitmaps */

static const __poll_t band_table[NSIGPOLL] = {
	EPOLLIN | EPOLLRDNORM,			/* POLL_IN */
	EPOLLOUT | EPOLLWRNORM | EPOLLWRBAND,	/* POLL_OUT */
	EPOLLIN | EPOLLRDNORM | EPOLLMSG,		/* POLL_MSG */
	EPOLLERR,				/* POLL_ERR */
	EPOLLPRI | EPOLLRDBAND,			/* POLL_PRI */
	EPOLLHUP | EPOLLERR			/* POLL_HUP */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static inline int sigio_perm(struct task_struct *p,
                             struct fown_struct *fown, int sig)
{
	const struct cred *cred;
	int ret;

	rcu_read_lock();
	cred = __task_cred(p);
<<<<<<< HEAD
	ret = ((fown->euid == 0 ||
		fown->euid == cred->suid || fown->euid == cred->uid ||
		fown->uid  == cred->suid || fown->uid  == cred->uid) &&
=======
	ret = ((uid_eq(fown->euid, GLOBAL_ROOT_UID) ||
		uid_eq(fown->euid, cred->suid) || uid_eq(fown->euid, cred->uid) ||
		uid_eq(fown->uid,  cred->suid) || uid_eq(fown->uid,  cred->uid)) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       !security_file_send_sigiotask(p, fown, sig));
	rcu_read_unlock();
	return ret;
}

static void send_sigio_to_task(struct task_struct *p,
			       struct fown_struct *fown,
<<<<<<< HEAD
			       int fd, int reason, int group)
=======
			       int fd, int reason, enum pid_type type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * F_SETSIG can change ->signum lockless in parallel, make
	 * sure we read it once and use the same value throughout.
	 */
<<<<<<< HEAD
	int signum = ACCESS_ONCE(fown->signum);
=======
	int signum = READ_ONCE(fown->signum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!sigio_perm(p, fown, signum))
		return;

	switch (signum) {
<<<<<<< HEAD
		siginfo_t si;
		default:
=======
		default: {
			kernel_siginfo_t si;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Queue a rt signal with the appropriate fd as its
			   value.  We use SI_SIGIO as the source, not 
			   SI_KERNEL, since kernel signals always get 
			   delivered even if we can't queue.  Failure to
			   queue in this case _should_ be reported; we fall
			   back to SIGIO in that case. --sct */
<<<<<<< HEAD
			si.si_signo = signum;
			si.si_errno = 0;
		        si.si_code  = reason;
			/* Make sure we are called with one of the POLL_*
			   reasons, otherwise we could leak kernel stack into
			   userspace.  */
			BUG_ON((reason & __SI_MASK) != __SI_POLL);
			if (reason - POLL_IN >= NSIGPOLL)
				si.si_band  = ~0L;
			else
				si.si_band = band_table[reason - POLL_IN];
			si.si_fd    = fd;
			if (!do_send_sig_info(signum, &si, p, group))
				break;
		/* fall-through: fall back on the old plain SIGIO signal */
		case 0:
			do_send_sig_info(SIGIO, SEND_SIG_PRIV, p, group);
=======
			clear_siginfo(&si);
			si.si_signo = signum;
			si.si_errno = 0;
		        si.si_code  = reason;
			/*
			 * Posix definies POLL_IN and friends to be signal
			 * specific si_codes for SIG_POLL.  Linux extended
			 * these si_codes to other signals in a way that is
			 * ambiguous if other signals also have signal
			 * specific si_codes.  In that case use SI_SIGIO instead
			 * to remove the ambiguity.
			 */
			if ((signum != SIGPOLL) && sig_specific_sicodes(signum))
				si.si_code = SI_SIGIO;

			/* Make sure we are called with one of the POLL_*
			   reasons, otherwise we could leak kernel stack into
			   userspace.  */
			BUG_ON((reason < POLL_IN) || ((reason - POLL_IN) >= NSIGPOLL));
			if (reason - POLL_IN >= NSIGPOLL)
				si.si_band  = ~0L;
			else
				si.si_band = mangle_poll(band_table[reason - POLL_IN]);
			si.si_fd    = fd;
			if (!do_send_sig_info(signum, &si, p, type))
				break;
		}
			fallthrough;	/* fall back on the old plain SIGIO signal */
		case 0:
			do_send_sig_info(SIGIO, SEND_SIG_PRIV, p, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

void send_sigio(struct fown_struct *fown, int fd, int band)
{
	struct task_struct *p;
	enum pid_type type;
<<<<<<< HEAD
	struct pid *pid;
	int group = 1;
	
	read_lock(&fown->lock);

	type = fown->pid_type;
	if (type == PIDTYPE_MAX) {
		group = 0;
		type = PIDTYPE_PID;
	}

	pid = fown->pid;
	if (!pid)
		goto out_unlock_fown;
	
	read_lock(&tasklist_lock);
	do_each_pid_task(pid, type, p) {
		send_sigio_to_task(p, fown, fd, band, group);
	} while_each_pid_task(pid, type, p);
	read_unlock(&tasklist_lock);
 out_unlock_fown:
	read_unlock(&fown->lock);
}

static void send_sigurg_to_task(struct task_struct *p,
				struct fown_struct *fown, int group)
{
	if (sigio_perm(p, fown, SIGURG))
		do_send_sig_info(SIGURG, SEND_SIG_PRIV, p, group);
=======
	unsigned long flags;
	struct pid *pid;
	
	read_lock_irqsave(&fown->lock, flags);

	type = fown->pid_type;
	pid = fown->pid;
	if (!pid)
		goto out_unlock_fown;

	if (type <= PIDTYPE_TGID) {
		rcu_read_lock();
		p = pid_task(pid, PIDTYPE_PID);
		if (p)
			send_sigio_to_task(p, fown, fd, band, type);
		rcu_read_unlock();
	} else {
		read_lock(&tasklist_lock);
		do_each_pid_task(pid, type, p) {
			send_sigio_to_task(p, fown, fd, band, type);
		} while_each_pid_task(pid, type, p);
		read_unlock(&tasklist_lock);
	}
 out_unlock_fown:
	read_unlock_irqrestore(&fown->lock, flags);
}

static void send_sigurg_to_task(struct task_struct *p,
				struct fown_struct *fown, enum pid_type type)
{
	if (sigio_perm(p, fown, SIGURG))
		do_send_sig_info(SIGURG, SEND_SIG_PRIV, p, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int send_sigurg(struct fown_struct *fown)
{
	struct task_struct *p;
	enum pid_type type;
	struct pid *pid;
<<<<<<< HEAD
	int group = 1;
	int ret = 0;
	
	read_lock(&fown->lock);

	type = fown->pid_type;
	if (type == PIDTYPE_MAX) {
		group = 0;
		type = PIDTYPE_PID;
	}

=======
	unsigned long flags;
	int ret = 0;
	
	read_lock_irqsave(&fown->lock, flags);

	type = fown->pid_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pid = fown->pid;
	if (!pid)
		goto out_unlock_fown;

	ret = 1;
<<<<<<< HEAD
	
	read_lock(&tasklist_lock);
	do_each_pid_task(pid, type, p) {
		send_sigurg_to_task(p, fown, group);
	} while_each_pid_task(pid, type, p);
	read_unlock(&tasklist_lock);
 out_unlock_fown:
	read_unlock(&fown->lock);
=======

	if (type <= PIDTYPE_TGID) {
		rcu_read_lock();
		p = pid_task(pid, PIDTYPE_PID);
		if (p)
			send_sigurg_to_task(p, fown, type);
		rcu_read_unlock();
	} else {
		read_lock(&tasklist_lock);
		do_each_pid_task(pid, type, p) {
			send_sigurg_to_task(p, fown, type);
		} while_each_pid_task(pid, type, p);
		read_unlock(&tasklist_lock);
	}
 out_unlock_fown:
	read_unlock_irqrestore(&fown->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static DEFINE_SPINLOCK(fasync_lock);
<<<<<<< HEAD
static struct kmem_cache *fasync_cache __read_mostly;

static void fasync_free_rcu(struct rcu_head *head)
{
	kmem_cache_free(fasync_cache,
			container_of(head, struct fasync_struct, fa_rcu));
}
=======
static struct kmem_cache *fasync_cache __ro_after_init;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Remove a fasync entry. If successfully removed, return
 * positive and clear the FASYNC flag. If no entry exists,
 * do nothing and return 0.
 *
 * NOTE! It is very important that the FASYNC flag always
 * match the state "is the filp on a fasync list".
 *
 */
int fasync_remove_entry(struct file *filp, struct fasync_struct **fapp)
{
	struct fasync_struct *fa, **fp;
	int result = 0;

	spin_lock(&filp->f_lock);
	spin_lock(&fasync_lock);
	for (fp = fapp; (fa = *fp) != NULL; fp = &fa->fa_next) {
		if (fa->fa_file != filp)
			continue;

<<<<<<< HEAD
		spin_lock_irq(&fa->fa_lock);
		fa->fa_file = NULL;
		spin_unlock_irq(&fa->fa_lock);

		*fp = fa->fa_next;
		call_rcu(&fa->fa_rcu, fasync_free_rcu);
=======
		write_lock_irq(&fa->fa_lock);
		fa->fa_file = NULL;
		write_unlock_irq(&fa->fa_lock);

		*fp = fa->fa_next;
		kfree_rcu(fa, fa_rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		filp->f_flags &= ~FASYNC;
		result = 1;
		break;
	}
	spin_unlock(&fasync_lock);
	spin_unlock(&filp->f_lock);
	return result;
}

struct fasync_struct *fasync_alloc(void)
{
	return kmem_cache_alloc(fasync_cache, GFP_KERNEL);
}

/*
 * NOTE! This can be used only for unused fasync entries:
 * entries that actually got inserted on the fasync list
 * need to be released by rcu - see fasync_remove_entry.
 */
void fasync_free(struct fasync_struct *new)
{
	kmem_cache_free(fasync_cache, new);
}

/*
 * Insert a new entry into the fasync list.  Return the pointer to the
 * old one if we didn't use the new one.
 *
 * NOTE! It is very important that the FASYNC flag always
 * match the state "is the filp on a fasync list".
 */
struct fasync_struct *fasync_insert_entry(int fd, struct file *filp, struct fasync_struct **fapp, struct fasync_struct *new)
{
        struct fasync_struct *fa, **fp;

	spin_lock(&filp->f_lock);
	spin_lock(&fasync_lock);
	for (fp = fapp; (fa = *fp) != NULL; fp = &fa->fa_next) {
		if (fa->fa_file != filp)
			continue;

<<<<<<< HEAD
		spin_lock_irq(&fa->fa_lock);
		fa->fa_fd = fd;
		spin_unlock_irq(&fa->fa_lock);
		goto out;
	}

	spin_lock_init(&new->fa_lock);
=======
		write_lock_irq(&fa->fa_lock);
		fa->fa_fd = fd;
		write_unlock_irq(&fa->fa_lock);
		goto out;
	}

	rwlock_init(&new->fa_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	new->magic = FASYNC_MAGIC;
	new->fa_file = filp;
	new->fa_fd = fd;
	new->fa_next = *fapp;
	rcu_assign_pointer(*fapp, new);
	filp->f_flags |= FASYNC;

out:
	spin_unlock(&fasync_lock);
	spin_unlock(&filp->f_lock);
	return fa;
}

/*
 * Add a fasync entry. Return negative on error, positive if
 * added, and zero if did nothing but change an existing one.
 */
static int fasync_add_entry(int fd, struct file *filp, struct fasync_struct **fapp)
{
	struct fasync_struct *new;

	new = fasync_alloc();
	if (!new)
		return -ENOMEM;

	/*
	 * fasync_insert_entry() returns the old (update) entry if
	 * it existed.
	 *
	 * So free the (unused) new entry and return 0 to let the
	 * caller know that we didn't add any new fasync entries.
	 */
	if (fasync_insert_entry(fd, filp, fapp, new)) {
		fasync_free(new);
		return 0;
	}

	return 1;
}

/*
 * fasync_helper() is used by almost all character device drivers
 * to set up the fasync queue, and for regular files by the file
 * lease code. It returns negative on error, 0 if it did no changes
 * and positive if it added/deleted the entry.
 */
int fasync_helper(int fd, struct file * filp, int on, struct fasync_struct **fapp)
{
	if (!on)
		return fasync_remove_entry(filp, fapp);
	return fasync_add_entry(fd, filp, fapp);
}

EXPORT_SYMBOL(fasync_helper);

/*
 * rcu_read_lock() is held
 */
static void kill_fasync_rcu(struct fasync_struct *fa, int sig, int band)
{
	while (fa) {
		struct fown_struct *fown;
		unsigned long flags;

		if (fa->magic != FASYNC_MAGIC) {
			printk(KERN_ERR "kill_fasync: bad magic number in "
			       "fasync_struct!\n");
			return;
		}
<<<<<<< HEAD
		spin_lock_irqsave(&fa->fa_lock, flags);
=======
		read_lock_irqsave(&fa->fa_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (fa->fa_file) {
			fown = &fa->fa_file->f_owner;
			/* Don't send SIGURG to processes which have not set a
			   queued signum: SIGURG has its own default signalling
			   mechanism. */
			if (!(sig == SIGURG && fown->signum == 0))
				send_sigio(fown, fa->fa_fd, band);
		}
<<<<<<< HEAD
		spin_unlock_irqrestore(&fa->fa_lock, flags);
=======
		read_unlock_irqrestore(&fa->fa_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fa = rcu_dereference(fa->fa_next);
	}
}

void kill_fasync(struct fasync_struct **fp, int sig, int band)
{
	/* First a quick test without locking: usually
	 * the list is empty.
	 */
	if (*fp) {
		rcu_read_lock();
		kill_fasync_rcu(rcu_dereference(*fp), sig, band);
		rcu_read_unlock();
	}
}
EXPORT_SYMBOL(kill_fasync);

static int __init fcntl_init(void)
{
	/*
	 * Please add new bits here to ensure allocation uniqueness.
	 * Exceptions: O_NONBLOCK is a two bit define on parisc; O_NDELAY
	 * is defined as O_NONBLOCK on some platforms and not on others.
	 */
<<<<<<< HEAD
	BUILD_BUG_ON(20 - 1 /* for O_RDONLY being 0 */ != HWEIGHT32(
		O_RDONLY	| O_WRONLY	| O_RDWR	|
		O_CREAT		| O_EXCL	| O_NOCTTY	|
		O_TRUNC		| O_APPEND	| /* O_NONBLOCK	| */
		__O_SYNC	| O_DSYNC	| FASYNC	|
		O_DIRECT	| O_LARGEFILE	| O_DIRECTORY	|
		O_NOFOLLOW	| O_NOATIME	| O_CLOEXEC	|
		__FMODE_EXEC	| O_PATH	| __O_TMPFILE
		));

	fasync_cache = kmem_cache_create("fasync_cache",
		sizeof(struct fasync_struct), 0, SLAB_PANIC, NULL);
=======
	BUILD_BUG_ON(21 - 1 /* for O_RDONLY being 0 */ !=
		HWEIGHT32(
			(VALID_OPEN_FLAGS & ~(O_NONBLOCK | O_NDELAY)) |
			__FMODE_EXEC | __FMODE_NONOTIFY));

	fasync_cache = kmem_cache_create("fasync_cache",
					 sizeof(struct fasync_struct), 0,
					 SLAB_PANIC | SLAB_ACCOUNT, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

module_init(fcntl_init)
