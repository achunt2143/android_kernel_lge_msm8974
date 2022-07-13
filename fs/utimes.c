<<<<<<< HEAD
#include <linux/compiler.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/linkage.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/sched.h>
#include <linux/stat.h>
#include <linux/utime.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>

#ifdef __ARCH_WANT_SYS_UTIME

/*
 * sys_utime() can be implemented in user-level using sys_utimes().
 * Is this for backwards compatibility?  If so, why not move it
 * into the appropriate arch directory (for those architectures that
 * need it).
 */

/* If times==NULL, set access and modification to current time,
 * must be owner or have write permission.
 * Else, update from *times, must be owner or super user.
 */
SYSCALL_DEFINE2(utime, char __user *, filename, struct utimbuf __user *, times)
{
	struct timespec tv[2];

	if (times) {
		if (get_user(tv[0].tv_sec, &times->actime) ||
		    get_user(tv[1].tv_sec, &times->modtime))
			return -EFAULT;
		tv[0].tv_nsec = 0;
		tv[1].tv_nsec = 0;
	}
	return do_utimes(AT_FDCWD, filename, times ? tv : NULL, 0);
}

#endif
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/file.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/utime.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#include <asm/unistd.h>
#include <linux/filelock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static bool nsec_valid(long nsec)
{
	if (nsec == UTIME_OMIT || nsec == UTIME_NOW)
		return true;

	return nsec >= 0 && nsec <= 999999999;
}

<<<<<<< HEAD
static int utimes_common(struct path *path, struct timespec *times)
=======
int vfs_utimes(const struct path *path, struct timespec64 *times)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int error;
	struct iattr newattrs;
	struct inode *inode = path->dentry->d_inode;
<<<<<<< HEAD
=======
	struct inode *delegated_inode = NULL;

	if (times) {
		if (!nsec_valid(times[0].tv_nsec) ||
		    !nsec_valid(times[1].tv_nsec))
			return -EINVAL;
		if (times[0].tv_nsec == UTIME_NOW &&
		    times[1].tv_nsec == UTIME_NOW)
			times = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	error = mnt_want_write(path->mnt);
	if (error)
		goto out;

<<<<<<< HEAD
	if (times && times[0].tv_nsec == UTIME_NOW &&
		     times[1].tv_nsec == UTIME_NOW)
		times = NULL;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	newattrs.ia_valid = ATTR_CTIME | ATTR_MTIME | ATTR_ATIME;
	if (times) {
		if (times[0].tv_nsec == UTIME_OMIT)
			newattrs.ia_valid &= ~ATTR_ATIME;
		else if (times[0].tv_nsec != UTIME_NOW) {
<<<<<<< HEAD
			newattrs.ia_atime.tv_sec = times[0].tv_sec;
			newattrs.ia_atime.tv_nsec = times[0].tv_nsec;
=======
			newattrs.ia_atime = times[0];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			newattrs.ia_valid |= ATTR_ATIME_SET;
		}

		if (times[1].tv_nsec == UTIME_OMIT)
			newattrs.ia_valid &= ~ATTR_MTIME;
		else if (times[1].tv_nsec != UTIME_NOW) {
<<<<<<< HEAD
			newattrs.ia_mtime.tv_sec = times[1].tv_sec;
			newattrs.ia_mtime.tv_nsec = times[1].tv_nsec;
			newattrs.ia_valid |= ATTR_MTIME_SET;
		}
		/*
		 * Tell inode_change_ok(), that this is an explicit time
=======
			newattrs.ia_mtime = times[1];
			newattrs.ia_valid |= ATTR_MTIME_SET;
		}
		/*
		 * Tell setattr_prepare(), that this is an explicit time
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * update, even if neither ATTR_ATIME_SET nor ATTR_MTIME_SET
		 * were used.
		 */
		newattrs.ia_valid |= ATTR_TIMES_SET;
	} else {
<<<<<<< HEAD
		/*
		 * If times is NULL (or both times are UTIME_NOW),
		 * then we need to check permissions, because
		 * inode_change_ok() won't do it.
		 */
		error = -EACCES;
                if (IS_IMMUTABLE(inode))
			goto mnt_drop_write_and_out;

		if (!inode_owner_or_capable(inode)) {
			error = inode_permission2(path->mnt, inode, MAY_WRITE);
			if (error)
				goto mnt_drop_write_and_out;
		}
	}
	mutex_lock(&inode->i_mutex);
	error = notify_change2(path->mnt, path->dentry, &newattrs);
	mutex_unlock(&inode->i_mutex);

mnt_drop_write_and_out:
=======
		newattrs.ia_valid |= ATTR_TOUCH;
	}
retry_deleg:
	inode_lock(inode);
	error = notify_change(mnt_idmap(path->mnt), path->dentry, &newattrs,
			      &delegated_inode);
	inode_unlock(inode);
	if (delegated_inode) {
		error = break_deleg_wait(&delegated_inode);
		if (!error)
			goto retry_deleg;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mnt_drop_write(path->mnt);
out:
	return error;
}

<<<<<<< HEAD
=======
static int do_utimes_path(int dfd, const char __user *filename,
		struct timespec64 *times, int flags)
{
	struct path path;
	int lookup_flags = 0, error;

	if (flags & ~(AT_SYMLINK_NOFOLLOW | AT_EMPTY_PATH))
		return -EINVAL;

	if (!(flags & AT_SYMLINK_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
		lookup_flags |= LOOKUP_EMPTY;

retry:
	error = user_path_at(dfd, filename, lookup_flags, &path);
	if (error)
		return error;

	error = vfs_utimes(&path, times);
	path_put(&path);
	if (retry_estale(error, lookup_flags)) {
		lookup_flags |= LOOKUP_REVAL;
		goto retry;
	}

	return error;
}

static int do_utimes_fd(int fd, struct timespec64 *times, int flags)
{
	struct fd f;
	int error;

	if (flags)
		return -EINVAL;

	f = fdget(fd);
	if (!f.file)
		return -EBADF;
	error = vfs_utimes(&f.file->f_path, times);
	fdput(f);
	return error;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * do_utimes - change times on filename or file descriptor
 * @dfd: open file descriptor, -1 or AT_FDCWD
 * @filename: path name or NULL
 * @times: new times or NULL
 * @flags: zero or more flags (only AT_SYMLINK_NOFOLLOW for the moment)
 *
 * If filename is NULL and dfd refers to an open file, then operate on
 * the file.  Otherwise look up filename, possibly using dfd as a
 * starting point.
 *
 * If times==NULL, set access and modification to current time,
 * must be owner or have write permission.
 * Else, update from *times, must be owner or super user.
 */
<<<<<<< HEAD
long do_utimes(int dfd, const char __user *filename, struct timespec *times,
	       int flags)
{
	int error = -EINVAL;

	if (times && (!nsec_valid(times[0].tv_nsec) ||
		      !nsec_valid(times[1].tv_nsec))) {
		goto out;
	}

	if (flags & ~AT_SYMLINK_NOFOLLOW)
		goto out;

	if (filename == NULL && dfd != AT_FDCWD) {
		struct file *file;

		if (flags & AT_SYMLINK_NOFOLLOW)
			goto out;

		file = fget(dfd);
		error = -EBADF;
		if (!file)
			goto out;

		error = utimes_common(&file->f_path, times);
		fput(file);
	} else {
		struct path path;
		int lookup_flags = 0;

		if (!(flags & AT_SYMLINK_NOFOLLOW))
			lookup_flags |= LOOKUP_FOLLOW;

		error = user_path_at(dfd, filename, lookup_flags, &path);
		if (error)
			goto out;

		error = utimes_common(&path, times);
		path_put(&path);
	}

out:
	return error;
}

SYSCALL_DEFINE4(utimensat, int, dfd, const char __user *, filename,
		struct timespec __user *, utimes, int, flags)
{
	struct timespec tstimes[2];

	if (utimes) {
		if (copy_from_user(&tstimes, utimes, sizeof(tstimes)))
=======
long do_utimes(int dfd, const char __user *filename, struct timespec64 *times,
	       int flags)
{
	if (filename == NULL && dfd != AT_FDCWD)
		return do_utimes_fd(dfd, times, flags);
	return do_utimes_path(dfd, filename, times, flags);
}

SYSCALL_DEFINE4(utimensat, int, dfd, const char __user *, filename,
		struct __kernel_timespec __user *, utimes, int, flags)
{
	struct timespec64 tstimes[2];

	if (utimes) {
		if ((get_timespec64(&tstimes[0], &utimes[0]) ||
			get_timespec64(&tstimes[1], &utimes[1])))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;

		/* Nothing to do, we must not even check the path.  */
		if (tstimes[0].tv_nsec == UTIME_OMIT &&
		    tstimes[1].tv_nsec == UTIME_OMIT)
			return 0;
	}

	return do_utimes(dfd, filename, utimes ? tstimes : NULL, flags);
}

<<<<<<< HEAD
SYSCALL_DEFINE3(futimesat, int, dfd, const char __user *, filename,
		struct timeval __user *, utimes)
{
	struct timeval times[2];
	struct timespec tstimes[2];
=======
#ifdef __ARCH_WANT_SYS_UTIME
/*
 * futimesat(), utimes() and utime() are older versions of utimensat()
 * that are provided for compatibility with traditional C libraries.
 * On modern architectures, we always use libc wrappers around
 * utimensat() instead.
 */
static long do_futimesat(int dfd, const char __user *filename,
			 struct __kernel_old_timeval __user *utimes)
{
	struct __kernel_old_timeval times[2];
	struct timespec64 tstimes[2];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (utimes) {
		if (copy_from_user(&times, utimes, sizeof(times)))
			return -EFAULT;

		/* This test is needed to catch all invalid values.  If we
		   would test only in do_utimes we would miss those invalid
		   values truncated by the multiplication with 1000.  Note
		   that we also catch UTIME_{NOW,OMIT} here which are only
		   valid for utimensat.  */
		if (times[0].tv_usec >= 1000000 || times[0].tv_usec < 0 ||
		    times[1].tv_usec >= 1000000 || times[1].tv_usec < 0)
			return -EINVAL;

		tstimes[0].tv_sec = times[0].tv_sec;
		tstimes[0].tv_nsec = 1000 * times[0].tv_usec;
		tstimes[1].tv_sec = times[1].tv_sec;
		tstimes[1].tv_nsec = 1000 * times[1].tv_usec;
	}

	return do_utimes(dfd, filename, utimes ? tstimes : NULL, 0);
}

<<<<<<< HEAD
SYSCALL_DEFINE2(utimes, char __user *, filename,
		struct timeval __user *, utimes)
{
	return sys_futimesat(AT_FDCWD, filename, utimes);
}
=======

SYSCALL_DEFINE3(futimesat, int, dfd, const char __user *, filename,
		struct __kernel_old_timeval __user *, utimes)
{
	return do_futimesat(dfd, filename, utimes);
}

SYSCALL_DEFINE2(utimes, char __user *, filename,
		struct __kernel_old_timeval __user *, utimes)
{
	return do_futimesat(AT_FDCWD, filename, utimes);
}

SYSCALL_DEFINE2(utime, char __user *, filename, struct utimbuf __user *, times)
{
	struct timespec64 tv[2];

	if (times) {
		if (get_user(tv[0].tv_sec, &times->actime) ||
		    get_user(tv[1].tv_sec, &times->modtime))
			return -EFAULT;
		tv[0].tv_nsec = 0;
		tv[1].tv_nsec = 0;
	}
	return do_utimes(AT_FDCWD, filename, times ? tv : NULL, 0);
}
#endif

#ifdef CONFIG_COMPAT_32BIT_TIME
/*
 * Not all architectures have sys_utime, so implement this in terms
 * of sys_utimes.
 */
#ifdef __ARCH_WANT_SYS_UTIME32
SYSCALL_DEFINE2(utime32, const char __user *, filename,
		struct old_utimbuf32 __user *, t)
{
	struct timespec64 tv[2];

	if (t) {
		if (get_user(tv[0].tv_sec, &t->actime) ||
		    get_user(tv[1].tv_sec, &t->modtime))
			return -EFAULT;
		tv[0].tv_nsec = 0;
		tv[1].tv_nsec = 0;
	}
	return do_utimes(AT_FDCWD, filename, t ? tv : NULL, 0);
}
#endif

SYSCALL_DEFINE4(utimensat_time32, unsigned int, dfd, const char __user *, filename, struct old_timespec32 __user *, t, int, flags)
{
	struct timespec64 tv[2];

	if  (t) {
		if (get_old_timespec32(&tv[0], &t[0]) ||
		    get_old_timespec32(&tv[1], &t[1]))
			return -EFAULT;

		if (tv[0].tv_nsec == UTIME_OMIT && tv[1].tv_nsec == UTIME_OMIT)
			return 0;
	}
	return do_utimes(dfd, filename, t ? tv : NULL, flags);
}

#ifdef __ARCH_WANT_SYS_UTIME32
static long do_compat_futimesat(unsigned int dfd, const char __user *filename,
				struct old_timeval32 __user *t)
{
	struct timespec64 tv[2];

	if (t) {
		if (get_user(tv[0].tv_sec, &t[0].tv_sec) ||
		    get_user(tv[0].tv_nsec, &t[0].tv_usec) ||
		    get_user(tv[1].tv_sec, &t[1].tv_sec) ||
		    get_user(tv[1].tv_nsec, &t[1].tv_usec))
			return -EFAULT;
		if (tv[0].tv_nsec >= 1000000 || tv[0].tv_nsec < 0 ||
		    tv[1].tv_nsec >= 1000000 || tv[1].tv_nsec < 0)
			return -EINVAL;
		tv[0].tv_nsec *= 1000;
		tv[1].tv_nsec *= 1000;
	}
	return do_utimes(dfd, filename, t ? tv : NULL, 0);
}

SYSCALL_DEFINE3(futimesat_time32, unsigned int, dfd,
		       const char __user *, filename,
		       struct old_timeval32 __user *, t)
{
	return do_compat_futimesat(dfd, filename, t);
}

SYSCALL_DEFINE2(utimes_time32, const char __user *, filename, struct old_timeval32 __user *, t)
{
	return do_compat_futimesat(AT_FDCWD, filename, t);
}
#endif
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
