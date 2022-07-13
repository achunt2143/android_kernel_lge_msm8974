<<<<<<< HEAD
/*
 *  linux/fs/9p/vfs_file.c
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This file contians vfs file ops for 9P2000.
 *
 *  Copyright (C) 2004 by Eric Van Hensbergen <ericvh@gmail.com>
 *  Copyright (C) 2002 by Ron Minnich <rminnich@lanl.gov>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
<<<<<<< HEAD
=======
#include <linux/filelock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/stat.h>
#include <linux/string.h>
<<<<<<< HEAD
#include <linux/inet.h>
#include <linux/list.h>
#include <linux/pagemap.h>
#include <linux/utsname.h>
#include <asm/uaccess.h>
#include <linux/idr.h>
=======
#include <linux/list.h>
#include <linux/pagemap.h>
#include <linux/utsname.h>
#include <linux/uaccess.h>
#include <linux/uio.h>
#include <linux/slab.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/9p/9p.h>
#include <net/9p/client.h>

#include "v9fs.h"
#include "v9fs_vfs.h"
#include "fid.h"
#include "cache.h"

<<<<<<< HEAD
static const struct vm_operations_struct v9fs_file_vm_ops;
=======
static const struct vm_operations_struct v9fs_mmap_file_vm_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * v9fs_file_open - open a file (or directory)
 * @inode: inode to be opened
 * @file: file being opened
 *
 */

int v9fs_file_open(struct inode *inode, struct file *file)
{
	int err;
<<<<<<< HEAD
	struct v9fs_inode *v9inode;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct v9fs_session_info *v9ses;
	struct p9_fid *fid;
	int omode;

	p9_debug(P9_DEBUG_VFS, "inode: %p file: %p\n", inode, file);
<<<<<<< HEAD
	v9inode = V9FS_I(inode);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	v9ses = v9fs_inode2v9ses(inode);
	if (v9fs_proto_dotl(v9ses))
		omode = v9fs_open_to_dotl_flags(file->f_flags);
	else
		omode = v9fs_uflags2omode(file->f_flags,
					v9fs_proto_dotu(v9ses));
	fid = file->private_data;
	if (!fid) {
<<<<<<< HEAD
		fid = v9fs_fid_clone(file->f_path.dentry);
		if (IS_ERR(fid))
			return PTR_ERR(fid);

		err = p9_client_open(fid, omode);
		if (err < 0) {
			p9_client_clunk(fid);
			return err;
		}
		if (file->f_flags & O_TRUNC) {
			i_size_write(inode, 0);
			inode->i_blocks = 0;
=======
		fid = v9fs_fid_clone(file_dentry(file));
		if (IS_ERR(fid))
			return PTR_ERR(fid);

		if ((v9ses->cache & CACHE_WRITEBACK) && (omode & P9_OWRITE)) {
			int writeback_omode = (omode & ~P9_OWRITE) | P9_ORDWR;

			p9_debug(P9_DEBUG_CACHE, "write-only file with writeback enabled, try opening O_RDWR\n");
			err = p9_client_open(fid, writeback_omode);
			if (err < 0) {
				p9_debug(P9_DEBUG_CACHE, "could not open O_RDWR, disabling caches\n");
				err = p9_client_open(fid, omode);
				fid->mode |= P9L_DIRECT;
			}
		} else {
			err = p9_client_open(fid, omode);
		}
		if (err < 0) {
			p9_fid_put(fid);
			return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		if ((file->f_flags & O_APPEND) &&
			(!v9fs_proto_dotu(v9ses) && !v9fs_proto_dotl(v9ses)))
			generic_file_llseek(file, 0, SEEK_END);
<<<<<<< HEAD
	}

	file->private_data = fid;
	mutex_lock(&v9inode->v_mutex);
	if (v9ses->cache && !v9inode->writeback_fid &&
	    ((file->f_flags & O_ACCMODE) != O_RDONLY)) {
		/*
		 * clone a fid and add it to writeback_fid
		 * we do it during open time instead of
		 * page dirty time via write_begin/page_mkwrite
		 * because we want write after unlink usecase
		 * to work.
		 */
		fid = v9fs_writeback_fid(file->f_path.dentry);
		if (IS_ERR(fid)) {
			err = PTR_ERR(fid);
			mutex_unlock(&v9inode->v_mutex);
			goto out_error;
		}
		v9inode->writeback_fid = (void *) fid;
	}
	mutex_unlock(&v9inode->v_mutex);
#ifdef CONFIG_9P_FSCACHE
	if (v9ses->cache)
		v9fs_cache_inode_set_cookie(inode, file);
#endif
	return 0;
out_error:
	p9_client_clunk(file->private_data);
	file->private_data = NULL;
	return err;
=======

		file->private_data = fid;
	}

#ifdef CONFIG_9P_FSCACHE
	if (v9ses->cache & CACHE_FSCACHE)
		fscache_use_cookie(v9fs_inode_cookie(V9FS_I(inode)),
				   file->f_mode & FMODE_WRITE);
#endif
	v9fs_fid_add_modes(fid, v9ses->flags, v9ses->cache, file->f_flags);
	v9fs_open_fid_add(inode, &fid);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * v9fs_file_lock - lock a file (or directory)
 * @filp: file to be locked
 * @cmd: lock command
 * @fl: file lock structure
 *
 * Bugs: this looks like a local only lock, we should extend into 9P
 *       by using open exclusive
 */

static int v9fs_file_lock(struct file *filp, int cmd, struct file_lock *fl)
{
<<<<<<< HEAD
	int res = 0;
	struct inode *inode = filp->f_path.dentry->d_inode;

	p9_debug(P9_DEBUG_VFS, "filp: %p lock: %p\n", filp, fl);

	/* No mandatory locks */
	if (__mandatory_lock(inode) && fl->fl_type != F_UNLCK)
		return -ENOLCK;

	if ((IS_SETLK(cmd) || IS_SETLKW(cmd)) && fl->fl_type != F_UNLCK) {
=======
	struct inode *inode = file_inode(filp);

	p9_debug(P9_DEBUG_VFS, "filp: %p lock: %p\n", filp, fl);

	if ((IS_SETLK(cmd) || IS_SETLKW(cmd)) && fl->c.flc_type != F_UNLCK) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		filemap_write_and_wait(inode->i_mapping);
		invalidate_mapping_pages(&inode->i_data, 0, -1);
	}

<<<<<<< HEAD
	return res;
=======
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int v9fs_file_do_lock(struct file *filp, int cmd, struct file_lock *fl)
{
	struct p9_flock flock;
	struct p9_fid *fid;
<<<<<<< HEAD
	uint8_t status;
	int res = 0;
	unsigned char fl_type;
=======
	uint8_t status = P9_LOCK_ERROR;
	int res = 0;
	struct v9fs_session_info *v9ses;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	fid = filp->private_data;
	BUG_ON(fid == NULL);

<<<<<<< HEAD
	if ((fl->fl_flags & FL_POSIX) != FL_POSIX)
		BUG();

	res = posix_lock_file_wait(filp, fl);
=======
	BUG_ON((fl->c.flc_flags & FL_POSIX) != FL_POSIX);

	res = locks_lock_file_wait(filp, fl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (res < 0)
		goto out;

	/* convert posix lock to p9 tlock args */
	memset(&flock, 0, sizeof(flock));
	/* map the lock type */
<<<<<<< HEAD
	switch (fl->fl_type) {
=======
	switch (fl->c.flc_type) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case F_RDLCK:
		flock.type = P9_LOCK_TYPE_RDLCK;
		break;
	case F_WRLCK:
		flock.type = P9_LOCK_TYPE_WRLCK;
		break;
	case F_UNLCK:
		flock.type = P9_LOCK_TYPE_UNLCK;
		break;
	}
	flock.start = fl->fl_start;
	if (fl->fl_end == OFFSET_MAX)
		flock.length = 0;
	else
		flock.length = fl->fl_end - fl->fl_start + 1;
<<<<<<< HEAD
	flock.proc_id = fl->fl_pid;
	flock.client_id = utsname()->nodename;
	if (IS_SETLKW(cmd))
		flock.flags = P9_LOCK_FLAGS_BLOCK;

=======
	flock.proc_id = fl->c.flc_pid;
	flock.client_id = fid->clnt->name;
	if (IS_SETLKW(cmd))
		flock.flags = P9_LOCK_FLAGS_BLOCK;

	v9ses = v9fs_inode2v9ses(file_inode(filp));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * if its a blocked request and we get P9_LOCK_BLOCKED as the status
	 * for lock request, keep on trying
	 */
	for (;;) {
		res = p9_client_lock_dotl(fid, &flock, &status);
		if (res < 0)
<<<<<<< HEAD
			break;
=======
			goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (status != P9_LOCK_BLOCKED)
			break;
		if (status == P9_LOCK_BLOCKED && !IS_SETLKW(cmd))
			break;
<<<<<<< HEAD
		if (schedule_timeout_interruptible(P9_LOCK_TIMEOUT) != 0)
			break;
=======
		if (schedule_timeout_interruptible(v9ses->session_lock_timeout)
				!= 0)
			break;
		/*
		 * p9_client_lock_dotl overwrites flock.client_id with the
		 * server message, free and reuse the client name
		 */
		if (flock.client_id != fid->clnt->name) {
			kfree(flock.client_id);
			flock.client_id = fid->clnt->name;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* map 9p status to VFS status */
	switch (status) {
	case P9_LOCK_SUCCESS:
		res = 0;
		break;
	case P9_LOCK_BLOCKED:
		res = -EAGAIN;
		break;
<<<<<<< HEAD
=======
	default:
		WARN_ONCE(1, "unknown lock status code: %d\n", status);
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case P9_LOCK_ERROR:
	case P9_LOCK_GRACE:
		res = -ENOLCK;
		break;
<<<<<<< HEAD
	default:
		BUG();
	}

=======
	}

out_unlock:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * incase server returned error for lock request, revert
	 * it locally
	 */
<<<<<<< HEAD
	if (res < 0 && fl->fl_type != F_UNLCK) {
		fl_type = fl->fl_type;
		fl->fl_type = F_UNLCK;
		res = posix_lock_file_wait(filp, fl);
		fl->fl_type = fl_type;
	}
=======
	if (res < 0 && fl->c.flc_type != F_UNLCK) {
		unsigned char type = fl->c.flc_type;

		fl->c.flc_type = F_UNLCK;
		/* Even if this fails we want to return the remote error */
		locks_lock_file_wait(filp, fl);
		fl->c.flc_type = type;
	}
	if (flock.client_id != fid->clnt->name)
		kfree(flock.client_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return res;
}

static int v9fs_file_getlock(struct file *filp, struct file_lock *fl)
{
	struct p9_getlock glock;
	struct p9_fid *fid;
	int res = 0;

	fid = filp->private_data;
	BUG_ON(fid == NULL);

	posix_test_lock(filp, fl);
	/*
	 * if we have a conflicting lock locally, no need to validate
	 * with server
	 */
<<<<<<< HEAD
	if (fl->fl_type != F_UNLCK)
=======
	if (fl->c.flc_type != F_UNLCK)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return res;

	/* convert posix lock to p9 tgetlock args */
	memset(&glock, 0, sizeof(glock));
	glock.type  = P9_LOCK_TYPE_UNLCK;
	glock.start = fl->fl_start;
	if (fl->fl_end == OFFSET_MAX)
		glock.length = 0;
	else
		glock.length = fl->fl_end - fl->fl_start + 1;
<<<<<<< HEAD
	glock.proc_id = fl->fl_pid;
	glock.client_id = utsname()->nodename;

	res = p9_client_getlock_dotl(fid, &glock);
	if (res < 0)
		return res;
	/* map 9p lock type to os lock type */
	switch (glock.type) {
	case P9_LOCK_TYPE_RDLCK:
		fl->fl_type = F_RDLCK;
		break;
	case P9_LOCK_TYPE_WRLCK:
		fl->fl_type = F_WRLCK;
		break;
	case P9_LOCK_TYPE_UNLCK:
		fl->fl_type = F_UNLCK;
=======
	glock.proc_id = fl->c.flc_pid;
	glock.client_id = fid->clnt->name;

	res = p9_client_getlock_dotl(fid, &glock);
	if (res < 0)
		goto out;
	/* map 9p lock type to os lock type */
	switch (glock.type) {
	case P9_LOCK_TYPE_RDLCK:
		fl->c.flc_type = F_RDLCK;
		break;
	case P9_LOCK_TYPE_WRLCK:
		fl->c.flc_type = F_WRLCK;
		break;
	case P9_LOCK_TYPE_UNLCK:
		fl->c.flc_type = F_UNLCK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	if (glock.type != P9_LOCK_TYPE_UNLCK) {
		fl->fl_start = glock.start;
		if (glock.length == 0)
			fl->fl_end = OFFSET_MAX;
		else
			fl->fl_end = glock.start + glock.length - 1;
<<<<<<< HEAD
		fl->fl_pid = glock.proc_id;
	}
=======
		fl->c.flc_pid = -glock.proc_id;
	}
out:
	if (glock.client_id != fid->clnt->name)
		kfree(glock.client_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return res;
}

/**
 * v9fs_file_lock_dotl - lock a file (or directory)
 * @filp: file to be locked
 * @cmd: lock command
 * @fl: file lock structure
 *
 */

static int v9fs_file_lock_dotl(struct file *filp, int cmd, struct file_lock *fl)
{
<<<<<<< HEAD
	struct inode *inode = filp->f_path.dentry->d_inode;
	int ret = -ENOLCK;

	p9_debug(P9_DEBUG_VFS, "filp: %p cmd:%d lock: %p name: %s\n",
		 filp, cmd, fl, filp->f_path.dentry->d_name.name);

	/* No mandatory locks */
	if (__mandatory_lock(inode) && fl->fl_type != F_UNLCK)
		goto out_err;

	if ((IS_SETLK(cmd) || IS_SETLKW(cmd)) && fl->fl_type != F_UNLCK) {
=======
	struct inode *inode = file_inode(filp);
	int ret = -ENOLCK;

	p9_debug(P9_DEBUG_VFS, "filp: %p cmd:%d lock: %p name: %pD\n",
		 filp, cmd, fl, filp);

	if ((IS_SETLK(cmd) || IS_SETLKW(cmd)) && fl->c.flc_type != F_UNLCK) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		filemap_write_and_wait(inode->i_mapping);
		invalidate_mapping_pages(&inode->i_data, 0, -1);
	}

	if (IS_SETLK(cmd) || IS_SETLKW(cmd))
		ret = v9fs_file_do_lock(filp, cmd, fl);
	else if (IS_GETLK(cmd))
		ret = v9fs_file_getlock(filp, fl);
	else
		ret = -EINVAL;
<<<<<<< HEAD
out_err:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/**
 * v9fs_file_flock_dotl - lock a file
 * @filp: file to be locked
 * @cmd: lock command
 * @fl: file lock structure
 *
 */

static int v9fs_file_flock_dotl(struct file *filp, int cmd,
	struct file_lock *fl)
{
<<<<<<< HEAD
	struct inode *inode = filp->f_path.dentry->d_inode;
	int ret = -ENOLCK;

	p9_debug(P9_DEBUG_VFS, "filp: %p cmd:%d lock: %p name: %s\n",
		 filp, cmd, fl, filp->f_path.dentry->d_name.name);

	/* No mandatory locks */
	if (__mandatory_lock(inode) && fl->fl_type != F_UNLCK)
		goto out_err;

	if (!(fl->fl_flags & FL_FLOCK))
		goto out_err;

	if ((IS_SETLK(cmd) || IS_SETLKW(cmd)) && fl->fl_type != F_UNLCK) {
=======
	struct inode *inode = file_inode(filp);
	int ret = -ENOLCK;

	p9_debug(P9_DEBUG_VFS, "filp: %p cmd:%d lock: %p name: %pD\n",
		 filp, cmd, fl, filp);

	if (!(fl->c.flc_flags & FL_FLOCK))
		goto out_err;

	if ((IS_SETLK(cmd) || IS_SETLKW(cmd)) && fl->c.flc_type != F_UNLCK) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		filemap_write_and_wait(inode->i_mapping);
		invalidate_mapping_pages(&inode->i_data, 0, -1);
	}
	/* Convert flock to posix lock */
<<<<<<< HEAD
	fl->fl_owner = (fl_owner_t)filp;
	fl->fl_start = 0;
	fl->fl_end = OFFSET_MAX;
	fl->fl_flags |= FL_POSIX;
	fl->fl_flags ^= FL_FLOCK;
=======
	fl->c.flc_flags |= FL_POSIX;
	fl->c.flc_flags ^= FL_FLOCK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (IS_SETLK(cmd) | IS_SETLKW(cmd))
		ret = v9fs_file_do_lock(filp, cmd, fl);
	else
		ret = -EINVAL;
out_err:
	return ret;
}

/**
<<<<<<< HEAD
 * v9fs_fid_readn - read from a fid
 * @fid: fid to read
 * @data: data buffer to read data into
 * @udata: user data buffer to read data into
 * @count: size of buffer
 * @offset: offset at which to read data
 *
 */
ssize_t
v9fs_fid_readn(struct p9_fid *fid, char *data, char __user *udata, u32 count,
	       u64 offset)
{
	int n, total, size;

	p9_debug(P9_DEBUG_VFS, "fid %d offset %llu count %d\n",
		 fid->fid, (long long unsigned)offset, count);
	n = 0;
	total = 0;
	size = fid->iounit ? fid->iounit : fid->clnt->msize - P9_IOHDRSZ;
	do {
		n = p9_client_read(fid, data, udata, offset, count);
		if (n <= 0)
			break;

		if (data)
			data += n;
		if (udata)
			udata += n;

		offset += n;
		count -= n;
		total += n;
	} while (count > 0 && n == size);

	if (n < 0)
		total = n;

	return total;
}

/**
 * v9fs_file_readn - read from a file
 * @filp: file pointer to read
 * @data: data buffer to read data into
 * @udata: user data buffer to read data into
 * @count: size of buffer
 * @offset: offset at which to read data
 *
 */
ssize_t
v9fs_file_readn(struct file *filp, char *data, char __user *udata, u32 count,
	       u64 offset)
{
	return v9fs_fid_readn(filp->private_data, data, udata, count, offset);
}

/**
 * v9fs_file_read - read from a file
 * @filp: file pointer to read
 * @udata: user data buffer to read data into
 * @count: size of buffer
 * @offset: offset at which to read data
 *
 */

static ssize_t
v9fs_file_read(struct file *filp, char __user *udata, size_t count,
	       loff_t * offset)
{
	int ret;
	struct p9_fid *fid;
	size_t size;

	p9_debug(P9_DEBUG_VFS, "count %zu offset %lld\n", count, *offset);
	fid = filp->private_data;

	size = fid->iounit ? fid->iounit : fid->clnt->msize - P9_IOHDRSZ;
	if (count > size)
		ret = v9fs_file_readn(filp, NULL, udata, count, *offset);
	else
		ret = p9_client_read(fid, NULL, udata, *offset, count);

	if (ret > 0)
		*offset += ret;

	return ret;
}

ssize_t
v9fs_file_write_internal(struct inode *inode, struct p9_fid *fid,
			 const char __user *data, size_t count,
			 loff_t *offset, int invalidate)
{
	int n;
	loff_t i_size;
	size_t total = 0;
	struct p9_client *clnt;
	loff_t origin = *offset;
	unsigned long pg_start, pg_end;

	p9_debug(P9_DEBUG_VFS, "data %p count %d offset %x\n",
		 data, (int)count, (int)*offset);

	clnt = fid->clnt;
	do {
		n = p9_client_write(fid, NULL, data+total, origin+total, count);
		if (n <= 0)
			break;
		count -= n;
		total += n;
	} while (count > 0);

	if (invalidate && (total > 0)) {
		pg_start = origin >> PAGE_CACHE_SHIFT;
		pg_end = (origin + total - 1) >> PAGE_CACHE_SHIFT;
		if (inode->i_mapping && inode->i_mapping->nrpages)
			invalidate_inode_pages2_range(inode->i_mapping,
						      pg_start, pg_end);
		*offset += total;
		i_size = i_size_read(inode);
		if (*offset > i_size) {
			inode_add_bytes(inode, *offset - i_size);
			i_size_write(inode, *offset);
		}
	}
	if (n < 0)
		return n;

	return total;
}

/**
 * v9fs_file_write - write to a file
 * @filp: file pointer to write
 * @data: data buffer to write data from
 * @count: size of buffer
 * @offset: offset at which to write data
 *
 */
static ssize_t
v9fs_file_write(struct file *filp, const char __user * data,
		size_t count, loff_t *offset)
{
	ssize_t retval = 0;
	loff_t origin = *offset;


	retval = generic_write_checks(filp, &origin, &count, 0);
	if (retval)
		goto out;

	retval = -EINVAL;
	if ((ssize_t) count < 0)
		goto out;
	retval = 0;
	if (!count)
		goto out;

	retval = v9fs_file_write_internal(filp->f_path.dentry->d_inode,
					filp->private_data,
					data, count, &origin, 1);
	/* update offset on successful write */
	if (retval > 0)
		*offset = origin;
out:
	return retval;
}

=======
 * v9fs_file_read_iter - read from a file
 * @iocb: The operation parameters
 * @to: The buffer to read into
 *
 */
static ssize_t
v9fs_file_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct p9_fid *fid = iocb->ki_filp->private_data;

	p9_debug(P9_DEBUG_VFS, "fid %d count %zu offset %lld\n",
		 fid->fid, iov_iter_count(to), iocb->ki_pos);

	if (fid->mode & P9L_DIRECT)
		return netfs_unbuffered_read_iter(iocb, to);

	p9_debug(P9_DEBUG_VFS, "(cached)\n");
	return netfs_file_read_iter(iocb, to);
}

/*
 * v9fs_file_splice_read - splice-read from a file
 * @in: The 9p file to read from
 * @ppos: Where to find/update the file position
 * @pipe: The pipe to splice into
 * @len: The maximum amount of data to splice
 * @flags: SPLICE_F_* flags
 */
static ssize_t v9fs_file_splice_read(struct file *in, loff_t *ppos,
				     struct pipe_inode_info *pipe,
				     size_t len, unsigned int flags)
{
	struct p9_fid *fid = in->private_data;

	p9_debug(P9_DEBUG_VFS, "fid %d count %zu offset %lld\n",
		 fid->fid, len, *ppos);

	if (fid->mode & P9L_DIRECT)
		return copy_splice_read(in, ppos, pipe, len, flags);
	return filemap_splice_read(in, ppos, pipe, len, flags);
}

/**
 * v9fs_file_write_iter - write to a file
 * @iocb: The operation parameters
 * @from: The data to write
 *
 */
static ssize_t
v9fs_file_write_iter(struct kiocb *iocb, struct iov_iter *from)
{
	struct file *file = iocb->ki_filp;
	struct p9_fid *fid = file->private_data;

	p9_debug(P9_DEBUG_VFS, "fid %d\n", fid->fid);

	if (fid->mode & (P9L_DIRECT | P9L_NOWRITECACHE))
		return netfs_unbuffered_write_iter(iocb, from);

	p9_debug(P9_DEBUG_CACHE, "(cached)\n");
	return netfs_file_write_iter(iocb, from);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int v9fs_file_fsync(struct file *filp, loff_t start, loff_t end,
			   int datasync)
{
	struct p9_fid *fid;
	struct inode *inode = filp->f_mapping->host;
	struct p9_wstat wstat;
	int retval;

<<<<<<< HEAD
	retval = filemap_write_and_wait_range(inode->i_mapping, start, end);
	if (retval)
		return retval;

	mutex_lock(&inode->i_mutex);
=======
	retval = file_write_and_wait_range(filp, start, end);
	if (retval)
		return retval;

	inode_lock(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p9_debug(P9_DEBUG_VFS, "filp %p datasync %x\n", filp, datasync);

	fid = filp->private_data;
	v9fs_blank_wstat(&wstat);

	retval = p9_client_wstat(fid, &wstat);
<<<<<<< HEAD
	mutex_unlock(&inode->i_mutex);
=======
	inode_unlock(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return retval;
}

int v9fs_file_fsync_dotl(struct file *filp, loff_t start, loff_t end,
			 int datasync)
{
	struct p9_fid *fid;
	struct inode *inode = filp->f_mapping->host;
	int retval;

<<<<<<< HEAD
	retval = filemap_write_and_wait_range(inode->i_mapping, start, end);
	if (retval)
		return retval;

	mutex_lock(&inode->i_mutex);
=======
	retval = file_write_and_wait_range(filp, start, end);
	if (retval)
		return retval;

	inode_lock(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p9_debug(P9_DEBUG_VFS, "filp %p datasync %x\n", filp, datasync);

	fid = filp->private_data;

	retval = p9_client_fsync(fid, datasync);
<<<<<<< HEAD
	mutex_unlock(&inode->i_mutex);
=======
	inode_unlock(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return retval;
}

static int
<<<<<<< HEAD
v9fs_file_mmap(struct file *file, struct vm_area_struct *vma)
{
	int retval;

	retval = generic_file_mmap(file, vma);
	if (!retval)
		vma->vm_ops = &v9fs_file_vm_ops;

	return retval;
}

static int
v9fs_vm_page_mkwrite(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct v9fs_inode *v9inode;
	struct page *page = vmf->page;
	struct file *filp = vma->vm_file;
	struct inode *inode = filp->f_path.dentry->d_inode;


	p9_debug(P9_DEBUG_VFS, "page %p fid %lx\n",
		 page, (unsigned long)filp->private_data);

	v9inode = V9FS_I(inode);
	/* make sure the cache has finished storing the page */
	v9fs_fscache_wait_on_page_write(inode, page);
	BUG_ON(!v9inode->writeback_fid);
	lock_page(page);
	if (page->mapping != inode->i_mapping)
		goto out_unlock;

	return VM_FAULT_LOCKED;
out_unlock:
	unlock_page(page);
	return VM_FAULT_NOPAGE;
}

static ssize_t
v9fs_direct_read(struct file *filp, char __user *udata, size_t count,
		 loff_t *offsetp)
{
	loff_t size, offset;
	struct inode *inode;
	struct address_space *mapping;

	offset = *offsetp;
	mapping = filp->f_mapping;
	inode = mapping->host;
	if (!count)
		return 0;
	size = i_size_read(inode);
	if (offset < size)
		filemap_write_and_wait_range(mapping, offset,
					     offset + count - 1);

	return v9fs_file_read(filp, udata, count, offsetp);
}

/**
 * v9fs_cached_file_read - read from a file
 * @filp: file pointer to read
 * @udata: user data buffer to read data into
 * @count: size of buffer
 * @offset: offset at which to read data
 *
 */
static ssize_t
v9fs_cached_file_read(struct file *filp, char __user *data, size_t count,
		      loff_t *offset)
{
	if (filp->f_flags & O_DIRECT)
		return v9fs_direct_read(filp, data, count, offset);
	return do_sync_read(filp, data, count, offset);
}

static ssize_t
v9fs_direct_write(struct file *filp, const char __user * data,
		  size_t count, loff_t *offsetp)
{
	loff_t offset;
	ssize_t retval;
	struct inode *inode;
	struct address_space *mapping;

	offset = *offsetp;
	mapping = filp->f_mapping;
	inode = mapping->host;
	if (!count)
		return 0;

	mutex_lock(&inode->i_mutex);
	retval = filemap_write_and_wait_range(mapping, offset,
					      offset + count - 1);
	if (retval)
		goto err_out;
	/*
	 * After a write we want buffered reads to be sure to go to disk to get
	 * the new data.  We invalidate clean cached page from the region we're
	 * about to write.  We do this *before* the write so that if we fail
	 * here we fall back to buffered write
	 */
	if (mapping->nrpages) {
		pgoff_t pg_start = offset >> PAGE_CACHE_SHIFT;
		pgoff_t pg_end   = (offset + count - 1) >> PAGE_CACHE_SHIFT;

		retval = invalidate_inode_pages2_range(mapping,
							pg_start, pg_end);
		/*
		 * If a page can not be invalidated, fall back
		 * to buffered write.
		 */
		if (retval) {
			if (retval == -EBUSY)
				goto buff_write;
			goto err_out;
		}
	}
	retval = v9fs_file_write(filp, data, count, offsetp);
err_out:
	mutex_unlock(&inode->i_mutex);
	return retval;

buff_write:
	mutex_unlock(&inode->i_mutex);
	return do_sync_write(filp, data, count, offsetp);
}

/**
 * v9fs_cached_file_write - write to a file
 * @filp: file pointer to write
 * @data: data buffer to write data from
 * @count: size of buffer
 * @offset: offset at which to write data
 *
 */
static ssize_t
v9fs_cached_file_write(struct file *filp, const char __user * data,
		       size_t count, loff_t *offset)
{

	if (filp->f_flags & O_DIRECT)
		return v9fs_direct_write(filp, data, count, offset);
	return do_sync_write(filp, data, count, offset);
}

static const struct vm_operations_struct v9fs_file_vm_ops = {
	.fault = filemap_fault,
	.page_mkwrite = v9fs_vm_page_mkwrite,
};


const struct file_operations v9fs_cached_file_operations = {
	.llseek = generic_file_llseek,
	.read = v9fs_cached_file_read,
	.write = v9fs_cached_file_write,
	.aio_read = generic_file_aio_read,
	.aio_write = generic_file_aio_write,
	.open = v9fs_file_open,
	.release = v9fs_dir_release,
	.lock = v9fs_file_lock,
	.mmap = v9fs_file_mmap,
	.fsync = v9fs_file_fsync,
};

const struct file_operations v9fs_cached_file_operations_dotl = {
	.llseek = generic_file_llseek,
	.read = v9fs_cached_file_read,
	.write = v9fs_cached_file_write,
	.aio_read = generic_file_aio_read,
	.aio_write = generic_file_aio_write,
	.open = v9fs_file_open,
	.release = v9fs_dir_release,
	.lock = v9fs_file_lock_dotl,
	.flock = v9fs_file_flock_dotl,
	.mmap = v9fs_file_mmap,
	.fsync = v9fs_file_fsync_dotl,
};

const struct file_operations v9fs_file_operations = {
	.llseek = generic_file_llseek,
	.read = v9fs_file_read,
	.write = v9fs_file_write,
=======
v9fs_file_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int retval;
	struct inode *inode = file_inode(filp);
	struct v9fs_session_info *v9ses = v9fs_inode2v9ses(inode);

	p9_debug(P9_DEBUG_MMAP, "filp :%p\n", filp);

	if (!(v9ses->cache & CACHE_WRITEBACK)) {
		p9_debug(P9_DEBUG_CACHE, "(read-only mmap mode)");
		return generic_file_readonly_mmap(filp, vma);
	}

	retval = generic_file_mmap(filp, vma);
	if (!retval)
		vma->vm_ops = &v9fs_mmap_file_vm_ops;

	return retval;
}

static vm_fault_t
v9fs_vm_page_mkwrite(struct vm_fault *vmf)
{
	return netfs_page_mkwrite(vmf, NULL);
}

static void v9fs_mmap_vm_close(struct vm_area_struct *vma)
{
	struct inode *inode;

	struct writeback_control wbc = {
		.nr_to_write = LONG_MAX,
		.sync_mode = WB_SYNC_ALL,
		.range_start = (loff_t)vma->vm_pgoff * PAGE_SIZE,
		 /* absolute end, byte at end included */
		.range_end = (loff_t)vma->vm_pgoff * PAGE_SIZE +
			(vma->vm_end - vma->vm_start - 1),
	};

	if (!(vma->vm_flags & VM_SHARED))
		return;

	p9_debug(P9_DEBUG_VFS, "9p VMA close, %p, flushing", vma);

	inode = file_inode(vma->vm_file);
	filemap_fdatawrite_wbc(inode->i_mapping, &wbc);
}

static const struct vm_operations_struct v9fs_mmap_file_vm_ops = {
	.close = v9fs_mmap_vm_close,
	.fault = filemap_fault,
	.map_pages = filemap_map_pages,
	.page_mkwrite = v9fs_vm_page_mkwrite,
};

const struct file_operations v9fs_file_operations = {
	.llseek = generic_file_llseek,
	.read_iter = v9fs_file_read_iter,
	.write_iter = v9fs_file_write_iter,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open = v9fs_file_open,
	.release = v9fs_dir_release,
	.lock = v9fs_file_lock,
	.mmap = generic_file_readonly_mmap,
<<<<<<< HEAD
	.fsync = v9fs_file_fsync,
=======
	.splice_read = v9fs_file_splice_read,
	.splice_write = iter_file_splice_write,
	.fsync = v9fs_file_fsync,
	.setlease = simple_nosetlease,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct file_operations v9fs_file_operations_dotl = {
	.llseek = generic_file_llseek,
<<<<<<< HEAD
	.read = v9fs_file_read,
	.write = v9fs_file_write,
=======
	.read_iter = v9fs_file_read_iter,
	.write_iter = v9fs_file_write_iter,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open = v9fs_file_open,
	.release = v9fs_dir_release,
	.lock = v9fs_file_lock_dotl,
	.flock = v9fs_file_flock_dotl,
<<<<<<< HEAD
	.mmap = generic_file_readonly_mmap,
	.fsync = v9fs_file_fsync_dotl,
=======
	.mmap = v9fs_file_mmap,
	.splice_read = v9fs_file_splice_read,
	.splice_write = iter_file_splice_write,
	.fsync = v9fs_file_fsync_dotl,
	.setlease = simple_nosetlease,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
