<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/fs/nfs/file.c
 *
 *  Copyright (C) 1992  Rick Sladkey
 *
 *  Changes Copyright (C) 1994 by Florian La Roche
 *   - Do not copy data too often around in the kernel.
 *   - In nfs_file_read the return value of kmalloc wasn't checked.
 *   - Put in a better version of read look-ahead buffering. Original idea
 *     and implementation by Wai S Kok elekokws@ee.nus.sg.
 *
 *  Expire cache on write to a file by Wai S Kok (Oct 1994).
 *
 *  Total rewrite of read side for new NFS buffer cache.. Linus.
 *
 *  nfs regular file handling functions
 */

<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/stat.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_mount.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
<<<<<<< HEAD
#include <linux/aio.h>
#include <linux/gfp.h>
#include <linux/swap.h>

#include <asm/uaccess.h>
=======
#include <linux/gfp.h>
#include <linux/swap.h>

#include <linux/uaccess.h>
#include <linux/filelock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "delegation.h"
#include "internal.h"
#include "iostat.h"
#include "fscache.h"
#include "pnfs.h"

<<<<<<< HEAD
=======
#include "nfstrace.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define NFSDBG_FACILITY		NFSDBG_FILE

static const struct vm_operations_struct nfs_file_vm_ops;

<<<<<<< HEAD
const struct inode_operations nfs_file_inode_operations = {
	.permission	= nfs_permission,
	.getattr	= nfs_getattr,
	.setattr	= nfs_setattr,
};

#ifdef CONFIG_NFS_V3
const struct inode_operations nfs3_file_inode_operations = {
	.permission	= nfs_permission,
	.getattr	= nfs_getattr,
	.setattr	= nfs_setattr,
	.listxattr	= nfs3_listxattr,
	.getxattr	= nfs3_getxattr,
	.setxattr	= nfs3_setxattr,
	.removexattr	= nfs3_removexattr,
};
#endif  /* CONFIG_NFS_v3 */

/* Hack for future NFS swap support */
#ifndef IS_SWAPFILE
# define IS_SWAPFILE(inode)	(0)
#endif

static int nfs_check_flags(int flags)
=======
int nfs_check_flags(int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if ((flags & (O_APPEND | O_DIRECT)) == (O_APPEND | O_DIRECT))
		return -EINVAL;

	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_check_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Open file
 */
static int
nfs_file_open(struct inode *inode, struct file *filp)
{
	int res;

<<<<<<< HEAD
	dprintk("NFS: open file(%s/%s)\n",
			filp->f_path.dentry->d_parent->d_name.name,
			filp->f_path.dentry->d_name.name);
=======
	dprintk("NFS: open file(%pD2)\n", filp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	nfs_inc_stats(inode, NFSIOS_VFSOPEN);
	res = nfs_check_flags(filp->f_flags);
	if (res)
		return res;

	res = nfs_open(inode, filp);
<<<<<<< HEAD
	return res;
}

static int
nfs_file_release(struct inode *inode, struct file *filp)
{
	dprintk("NFS: release(%s/%s)\n",
			filp->f_path.dentry->d_parent->d_name.name,
			filp->f_path.dentry->d_name.name);

	nfs_inc_stats(inode, NFSIOS_VFSRELEASE);
	return nfs_release(inode, filp);
}

/**
 * nfs_revalidate_size - Revalidate the file size
 * @inode - pointer to inode struct
 * @file - pointer to struct file
=======
	if (res == 0)
		filp->f_mode |= FMODE_CAN_ODIRECT;
	return res;
}

int
nfs_file_release(struct inode *inode, struct file *filp)
{
	dprintk("NFS: release(%pD2)\n", filp);

	nfs_inc_stats(inode, NFSIOS_VFSRELEASE);
	nfs_file_clear_open_context(filp);
	nfs_fscache_release_file(inode, filp);
	return 0;
}
EXPORT_SYMBOL_GPL(nfs_file_release);

/**
 * nfs_revalidate_file_size - Revalidate the file size
 * @inode: pointer to inode struct
 * @filp: pointer to struct file
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Revalidates the file length. This is basically a wrapper around
 * nfs_revalidate_inode() that takes into account the fact that we may
 * have cached writes (in which case we don't care about the server's
 * idea of what the file length is), or O_DIRECT (in which case we
 * shouldn't trust the cache).
 */
static int nfs_revalidate_file_size(struct inode *inode, struct file *filp)
{
	struct nfs_server *server = NFS_SERVER(inode);
<<<<<<< HEAD
	struct nfs_inode *nfsi = NFS_I(inode);

	if (nfs_have_delegated_attributes(inode))
		goto out_noreval;

	if (filp->f_flags & O_DIRECT)
		goto force_reval;
	if (nfsi->cache_validity & NFS_INO_REVAL_PAGECACHE)
		goto force_reval;
	if (nfs_attribute_timeout(inode))
		goto force_reval;
out_noreval:
=======

	if (filp->f_flags & O_DIRECT)
		goto force_reval;
	if (nfs_check_cache_invalid(inode, NFS_INO_INVALID_SIZE))
		goto force_reval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
force_reval:
	return __nfs_revalidate_inode(server, inode);
}

<<<<<<< HEAD
static loff_t nfs_file_llseek(struct file *filp, loff_t offset, int origin)
{
	dprintk("NFS: llseek file(%s/%s, %lld, %d)\n",
			filp->f_path.dentry->d_parent->d_name.name,
			filp->f_path.dentry->d_name.name,
			offset, origin);

	/*
	 * origin == SEEK_END || SEEK_DATA || SEEK_HOLE => we must revalidate
	 * the cached file length
	 */
	if (origin != SEEK_SET && origin != SEEK_CUR) {
=======
loff_t nfs_file_llseek(struct file *filp, loff_t offset, int whence)
{
	dprintk("NFS: llseek file(%pD2, %lld, %d)\n",
			filp, offset, whence);

	/*
	 * whence == SEEK_END || SEEK_DATA || SEEK_HOLE => we must revalidate
	 * the cached file length
	 */
	if (whence != SEEK_SET && whence != SEEK_CUR) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct inode *inode = filp->f_mapping->host;

		int retval = nfs_revalidate_file_size(inode, filp);
		if (retval < 0)
			return (loff_t)retval;
	}

<<<<<<< HEAD
	return generic_file_llseek(filp, offset, origin);
}
=======
	return generic_file_llseek(filp, offset, whence);
}
EXPORT_SYMBOL_GPL(nfs_file_llseek);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Flush all dirty pages, and check for write errors.
 */
static int
nfs_file_flush(struct file *file, fl_owner_t id)
{
<<<<<<< HEAD
	struct dentry	*dentry = file->f_path.dentry;
	struct inode	*inode = dentry->d_inode;

	dprintk("NFS: flush(%s/%s)\n",
			dentry->d_parent->d_name.name,
			dentry->d_name.name);
=======
	struct inode	*inode = file_inode(file);
	errseq_t since;

	dprintk("NFS: flush(%pD2)\n", file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	nfs_inc_stats(inode, NFSIOS_VFSFLUSH);
	if ((file->f_mode & FMODE_WRITE) == 0)
		return 0;

	/* Flush writes to the server and return any errors */
<<<<<<< HEAD
	return vfs_fsync(file, 0);
}

static ssize_t
nfs_file_read(struct kiocb *iocb, const struct iovec *iov,
		unsigned long nr_segs, loff_t pos)
{
	struct dentry * dentry = iocb->ki_filp->f_path.dentry;
	struct inode * inode = dentry->d_inode;
	ssize_t result;

	if (iocb->ki_filp->f_flags & O_DIRECT)
		return nfs_file_direct_read(iocb, iov, nr_segs, pos);

	dprintk("NFS: read(%s/%s, %lu@%lu)\n",
		dentry->d_parent->d_name.name, dentry->d_name.name,
		(unsigned long) iov_length(iov, nr_segs), (unsigned long) pos);

	result = nfs_revalidate_mapping(inode, iocb->ki_filp->f_mapping);
	if (!result) {
		result = generic_file_aio_read(iocb, iov, nr_segs, pos);
		if (result > 0)
			nfs_add_stats(inode, NFSIOS_NORMALREADBYTES, result);
	}
	return result;
}

static ssize_t
nfs_file_splice_read(struct file *filp, loff_t *ppos,
		     struct pipe_inode_info *pipe, size_t count,
		     unsigned int flags)
{
	struct dentry *dentry = filp->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	ssize_t res;

	dprintk("NFS: splice_read(%s/%s, %lu@%Lu)\n",
		dentry->d_parent->d_name.name, dentry->d_name.name,
		(unsigned long) count, (unsigned long long) *ppos);

	res = nfs_revalidate_mapping(inode, filp->f_mapping);
	if (!res) {
		res = generic_file_splice_read(filp, ppos, pipe, count, flags);
		if (res > 0)
			nfs_add_stats(inode, NFSIOS_NORMALREADBYTES, res);
	}
	return res;
}

static int
nfs_file_mmap(struct file * file, struct vm_area_struct * vma)
{
	struct dentry *dentry = file->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	int	status;

	dprintk("NFS: mmap(%s/%s)\n",
		dentry->d_parent->d_name.name, dentry->d_name.name);
=======
	since = filemap_sample_wb_err(file->f_mapping);
	nfs_wb_all(inode);
	return filemap_check_wb_err(file->f_mapping, since);
}

ssize_t
nfs_file_read(struct kiocb *iocb, struct iov_iter *to)
{
	struct inode *inode = file_inode(iocb->ki_filp);
	ssize_t result;

	if (iocb->ki_flags & IOCB_DIRECT)
		return nfs_file_direct_read(iocb, to, false);

	dprintk("NFS: read(%pD2, %zu@%lu)\n",
		iocb->ki_filp,
		iov_iter_count(to), (unsigned long) iocb->ki_pos);

	nfs_start_io_read(inode);
	result = nfs_revalidate_mapping(inode, iocb->ki_filp->f_mapping);
	if (!result) {
		result = generic_file_read_iter(iocb, to);
		if (result > 0)
			nfs_add_stats(inode, NFSIOS_NORMALREADBYTES, result);
	}
	nfs_end_io_read(inode);
	return result;
}
EXPORT_SYMBOL_GPL(nfs_file_read);

ssize_t
nfs_file_splice_read(struct file *in, loff_t *ppos, struct pipe_inode_info *pipe,
		     size_t len, unsigned int flags)
{
	struct inode *inode = file_inode(in);
	ssize_t result;

	dprintk("NFS: splice_read(%pD2, %zu@%llu)\n", in, len, *ppos);

	nfs_start_io_read(inode);
	result = nfs_revalidate_mapping(inode, in->f_mapping);
	if (!result) {
		result = filemap_splice_read(in, ppos, pipe, len, flags);
		if (result > 0)
			nfs_add_stats(inode, NFSIOS_NORMALREADBYTES, result);
	}
	nfs_end_io_read(inode);
	return result;
}
EXPORT_SYMBOL_GPL(nfs_file_splice_read);

int
nfs_file_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct inode *inode = file_inode(file);
	int	status;

	dprintk("NFS: mmap(%pD2)\n", file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Note: generic_file_mmap() returns ENOSYS on nommu systems
	 *       so we call that before revalidating the mapping
	 */
	status = generic_file_mmap(file, vma);
	if (!status) {
		vma->vm_ops = &nfs_file_vm_ops;
		status = nfs_revalidate_mapping(inode, file->f_mapping);
	}
	return status;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_file_mmap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Flush any dirty pages for this process, and check for write errors.
 * The return status from this call provides a reliable indication of
 * whether any write errors occurred for this process.
<<<<<<< HEAD
 *
 * Notice that it clears the NFS_CONTEXT_ERROR_WRITE before synching to
 * disk, but it retrieves and clears ctx->error after synching, despite
 * the two being set at the same time in nfs_context_set_write_error().
 * This is because the former is used to notify the _next_ call to
 * nfs_file_write() that a write error occurred, and hence cause it to
 * fall back to doing a synchronous write.
 */
static int
nfs_file_fsync(struct file *file, loff_t start, loff_t end, int datasync)
{
	struct dentry *dentry = file->f_path.dentry;
	struct nfs_open_context *ctx = nfs_file_open_context(file);
	struct inode *inode = dentry->d_inode;
	int have_error, status;
	int ret = 0;

	dprintk("NFS: fsync file(%s/%s) datasync %d\n",
			dentry->d_parent->d_name.name, dentry->d_name.name,
			datasync);

	ret = filemap_write_and_wait_range(inode->i_mapping, start, end);
	mutex_lock(&inode->i_mutex);

	nfs_inc_stats(inode, NFSIOS_VFSFSYNC);
	have_error = test_and_clear_bit(NFS_CONTEXT_ERROR_WRITE, &ctx->flags);
	status = nfs_commit_inode(inode, FLUSH_SYNC);
	if (status >= 0 && ret < 0)
		status = ret;
	have_error |= test_bit(NFS_CONTEXT_ERROR_WRITE, &ctx->flags);
	if (have_error)
		ret = xchg(&ctx->error, 0);
	if (!ret && status < 0)
		ret = status;
	if (!ret && !datasync)
		/* application has asked for meta-data sync */
		ret = pnfs_layoutcommit_inode(inode, true);
	mutex_unlock(&inode->i_mutex);
	return ret;
}

=======
 */
static int
nfs_file_fsync_commit(struct file *file, int datasync)
{
	struct inode *inode = file_inode(file);
	int ret, ret2;

	dprintk("NFS: fsync file(%pD2) datasync %d\n", file, datasync);

	nfs_inc_stats(inode, NFSIOS_VFSFSYNC);
	ret = nfs_commit_inode(inode, FLUSH_SYNC);
	ret2 = file_check_and_advance_wb_err(file);
	if (ret2 < 0)
		return ret2;
	return ret;
}

int
nfs_file_fsync(struct file *file, loff_t start, loff_t end, int datasync)
{
	struct inode *inode = file_inode(file);
	struct nfs_inode *nfsi = NFS_I(inode);
	long save_nredirtied = atomic_long_read(&nfsi->redirtied_pages);
	long nredirtied;
	int ret;

	trace_nfs_fsync_enter(inode);

	for (;;) {
		ret = file_write_and_wait_range(file, start, end);
		if (ret != 0)
			break;
		ret = nfs_file_fsync_commit(file, datasync);
		if (ret != 0)
			break;
		ret = pnfs_sync_inode(inode, !!datasync);
		if (ret != 0)
			break;
		nredirtied = atomic_long_read(&nfsi->redirtied_pages);
		if (nredirtied == save_nredirtied)
			break;
		save_nredirtied = nredirtied;
	}

	trace_nfs_fsync_exit(inode, ret);
	return ret;
}
EXPORT_SYMBOL_GPL(nfs_file_fsync);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Decide whether a read/modify/write cycle may be more efficient
 * then a modify/write/read cycle when writing to a page in the
 * page cache.
 *
<<<<<<< HEAD
=======
 * Some pNFS layout drivers can only read/write at a certain block
 * granularity like all block devices and therefore we must perform
 * read/modify/write whenever a page hasn't read yet and the data
 * to be written there is not aligned to a block boundary and/or
 * smaller than the block size.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * The modify/write/read cycle may occur if a page is read before
 * being completely filled by the writer.  In this situation, the
 * page must be completely written to stable storage on the server
 * before it can be refilled by reading in the page from the server.
 * This can lead to expensive, small, FILE_SYNC mode writes being
 * done.
 *
 * It may be more efficient to read the page first if the file is
 * open for reading in addition to writing, the page is not marked
 * as Uptodate, it is not dirty or waiting to be committed,
 * indicating that it was previously allocated and then modified,
 * that there were valid bytes of data in that range of the file,
 * and that the new data won't completely replace the old data in
 * that range of the file.
 */
<<<<<<< HEAD
static int nfs_want_read_modify_write(struct file *file, struct page *page,
			loff_t pos, unsigned len)
{
	unsigned int pglen = nfs_page_length(page);
	unsigned int offset = pos & (PAGE_CACHE_SIZE - 1);
	unsigned int end = offset + len;

	if ((file->f_mode & FMODE_READ) &&	/* open for read? */
	    !PageUptodate(page) &&		/* Uptodate? */
	    !PagePrivate(page) &&		/* i/o request already? */
	    pglen &&				/* valid bytes of file? */
	    (end < pglen || offset))		/* replace all valid bytes? */
		return 1;
	return 0;
=======
static bool nfs_folio_is_full_write(struct folio *folio, loff_t pos,
				    unsigned int len)
{
	unsigned int pglen = nfs_folio_length(folio);
	unsigned int offset = offset_in_folio(folio, pos);
	unsigned int end = offset + len;

	return !pglen || (end >= pglen && !offset);
}

static bool nfs_want_read_modify_write(struct file *file, struct folio *folio,
				       loff_t pos, unsigned int len)
{
	/*
	 * Up-to-date pages, those with ongoing or full-page write
	 * don't need read/modify/write
	 */
	if (folio_test_uptodate(folio) || folio_test_private(folio) ||
	    nfs_folio_is_full_write(folio, pos, len))
		return false;

	if (pnfs_ld_read_whole_page(file_inode(file)))
		return true;
	/* Open for reading too? */
	if (file->f_mode & FMODE_READ)
		return true;
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This does the "real" work of the write. We must allocate and lock the
 * page to be sent back to the generic routine, which then copies the
 * data from user space.
 *
 * If the writer ends up delaying the write, the writer needs to
 * increment the page use counts until he is done with the page.
 */
static int nfs_write_begin(struct file *file, struct address_space *mapping,
<<<<<<< HEAD
			loff_t pos, unsigned len, unsigned flags,
			struct page **pagep, void **fsdata)
{
	int ret;
	pgoff_t index = pos >> PAGE_CACHE_SHIFT;
	struct page *page;
	int once_thru = 0;

	dfprintk(PAGECACHE, "NFS: write_begin(%s/%s(%ld), %u@%lld)\n",
		file->f_path.dentry->d_parent->d_name.name,
		file->f_path.dentry->d_name.name,
		mapping->host->i_ino, len, (long long) pos);

start:
	/*
	 * Prevent starvation issues if someone is doing a consistency
	 * sync-to-disk
	 */
	ret = wait_on_bit(&NFS_I(mapping->host)->flags, NFS_INO_FLUSHING,
			nfs_wait_bit_killable, TASK_KILLABLE);
	if (ret)
		return ret;

	page = grab_cache_page_write_begin(mapping, index, flags);
	if (!page)
		return -ENOMEM;
	*pagep = page;

	ret = nfs_flush_incompatible(file, page);
	if (ret) {
		unlock_page(page);
		page_cache_release(page);
	} else if (!once_thru &&
		   nfs_want_read_modify_write(file, page, pos, len)) {
		once_thru = 1;
		ret = nfs_readpage(file, page);
		page_cache_release(page);
=======
			   loff_t pos, unsigned len, struct page **pagep,
			   void **fsdata)
{
	struct folio *folio;
	int once_thru = 0;
	int ret;

	dfprintk(PAGECACHE, "NFS: write_begin(%pD2(%lu), %u@%lld)\n",
		file, mapping->host->i_ino, len, (long long) pos);

start:
	folio = __filemap_get_folio(mapping, pos >> PAGE_SHIFT, FGP_WRITEBEGIN,
				    mapping_gfp_mask(mapping));
	if (IS_ERR(folio))
		return PTR_ERR(folio);
	*pagep = &folio->page;

	ret = nfs_flush_incompatible(file, folio);
	if (ret) {
		folio_unlock(folio);
		folio_put(folio);
	} else if (!once_thru &&
		   nfs_want_read_modify_write(file, folio, pos, len)) {
		once_thru = 1;
		ret = nfs_read_folio(file, folio);
		folio_put(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!ret)
			goto start;
	}
	return ret;
}

static int nfs_write_end(struct file *file, struct address_space *mapping,
<<<<<<< HEAD
			loff_t pos, unsigned len, unsigned copied,
			struct page *page, void *fsdata)
{
	unsigned offset = pos & (PAGE_CACHE_SIZE - 1);
	int status;

	dfprintk(PAGECACHE, "NFS: write_end(%s/%s(%ld), %u@%lld)\n",
		file->f_path.dentry->d_parent->d_name.name,
		file->f_path.dentry->d_name.name,
		mapping->host->i_ino, len, (long long) pos);
=======
			 loff_t pos, unsigned len, unsigned copied,
			 struct page *page, void *fsdata)
{
	struct nfs_open_context *ctx = nfs_file_open_context(file);
	struct folio *folio = page_folio(page);
	unsigned offset = offset_in_folio(folio, pos);
	int status;

	dfprintk(PAGECACHE, "NFS: write_end(%pD2(%lu), %u@%lld)\n",
		file, mapping->host->i_ino, len, (long long) pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Zero any uninitialised parts of the page, and then mark the page
	 * as up to date if it turns out that we're extending the file.
	 */
<<<<<<< HEAD
	if (!PageUptodate(page)) {
		unsigned pglen = nfs_page_length(page);
		unsigned end = offset + len;

		if (pglen == 0) {
			zero_user_segments(page, 0, offset,
					end, PAGE_CACHE_SIZE);
			SetPageUptodate(page);
		} else if (end >= pglen) {
			zero_user_segment(page, end, PAGE_CACHE_SIZE);
			if (offset == 0)
				SetPageUptodate(page);
		} else
			zero_user_segment(page, pglen, PAGE_CACHE_SIZE);
	}

	status = nfs_updatepage(file, page, offset, copied);

	unlock_page(page);
	page_cache_release(page);

	if (status < 0)
		return status;
=======
	if (!folio_test_uptodate(folio)) {
		size_t fsize = folio_size(folio);
		unsigned pglen = nfs_folio_length(folio);
		unsigned end = offset + copied;

		if (pglen == 0) {
			folio_zero_segments(folio, 0, offset, end, fsize);
			folio_mark_uptodate(folio);
		} else if (end >= pglen) {
			folio_zero_segment(folio, end, fsize);
			if (offset == 0)
				folio_mark_uptodate(folio);
		} else
			folio_zero_segment(folio, pglen, fsize);
	}

	status = nfs_update_folio(file, folio, offset, copied);

	folio_unlock(folio);
	folio_put(folio);

	if (status < 0)
		return status;
	NFS_I(mapping->host)->write_io += copied;

	if (nfs_ctx_key_to_expire(ctx, mapping->host))
		nfs_wb_all(mapping->host);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return copied;
}

/*
 * Partially or wholly invalidate a page
 * - Release the private state associated with a page if undergoing complete
 *   page invalidation
 * - Called if either PG_private or PG_fscache is set on the page
 * - Caller holds page lock
 */
<<<<<<< HEAD
static void nfs_invalidate_page(struct page *page, unsigned long offset)
{
	dfprintk(PAGECACHE, "NFS: invalidate_page(%p, %lu)\n", page, offset);

	if (offset != 0)
		return;
	/* Cancel any unstarted writes on this page */
	nfs_wb_page_cancel(page->mapping->host, page);

	nfs_fscache_invalidate_page(page, page->mapping->host);
}

/*
 * Attempt to release the private state associated with a page
 * - Called if either PG_private or PG_fscache is set on the page
 * - Caller holds page lock
 * - Return true (may release page) or false (may not)
 */
static int nfs_release_page(struct page *page, gfp_t gfp)
{
	struct address_space *mapping = page->mapping;

	dfprintk(PAGECACHE, "NFS: release_page(%p)\n", page);

	/* Only do I/O if gfp is a superset of GFP_KERNEL, and we're not
	 * doing this memory reclaim for a fs-related allocation.
	 */
	if (mapping && (gfp & GFP_KERNEL) == GFP_KERNEL &&
	    !(current->flags & PF_FSTRANS)) {
		int how = FLUSH_SYNC;

		/* Don't let kswapd deadlock waiting for OOM RPC calls */
		if (current_is_kswapd())
			how = 0;
		nfs_commit_inode(mapping->host, how);
	}
	/* If PagePrivate() is set, then the page is not freeable */
	if (PagePrivate(page))
		return 0;
	return nfs_fscache_release_page(page, gfp);
=======
static void nfs_invalidate_folio(struct folio *folio, size_t offset,
				size_t length)
{
	struct inode *inode = folio_file_mapping(folio)->host;
	dfprintk(PAGECACHE, "NFS: invalidate_folio(%lu, %zu, %zu)\n",
		 folio->index, offset, length);

	if (offset != 0 || length < folio_size(folio))
		return;
	/* Cancel any unstarted writes on this page */
	nfs_wb_folio_cancel(inode, folio);
	folio_wait_fscache(folio);
	trace_nfs_invalidate_folio(inode, folio);
}

/*
 * Attempt to release the private state associated with a folio
 * - Called if either private or fscache flags are set on the folio
 * - Caller holds folio lock
 * - Return true (may release folio) or false (may not)
 */
static bool nfs_release_folio(struct folio *folio, gfp_t gfp)
{
	dfprintk(PAGECACHE, "NFS: release_folio(%p)\n", folio);

	/* If the private flag is set, then the folio is not freeable */
	if (folio_test_private(folio)) {
		if ((current_gfp_context(gfp) & GFP_KERNEL) != GFP_KERNEL ||
		    current_is_kswapd())
			return false;
		if (nfs_wb_folio(folio_file_mapping(folio)->host, folio) < 0)
			return false;
	}
	return nfs_fscache_release_folio(folio, gfp);
}

static void nfs_check_dirty_writeback(struct folio *folio,
				bool *dirty, bool *writeback)
{
	struct nfs_inode *nfsi;
	struct address_space *mapping = folio->mapping;

	/*
	 * Check if an unstable folio is currently being committed and
	 * if so, have the VM treat it as if the folio is under writeback
	 * so it will not block due to folios that will shortly be freeable.
	 */
	nfsi = NFS_I(mapping->host);
	if (atomic_read(&nfsi->commit_info.rpcs_out)) {
		*writeback = true;
		return;
	}

	/*
	 * If the private flag is set, then the folio is not freeable
	 * and as the inode is not being committed, it's not going to
	 * be cleaned in the near future so treat it as dirty
	 */
	if (folio_test_private(folio))
		*dirty = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Attempt to clear the private state associated with a page when an error
 * occurs that requires the cached contents of an inode to be written back or
 * destroyed
 * - Called if either PG_private or fscache is set on the page
 * - Caller holds page lock
 * - Return 0 if successful, -error otherwise
 */
<<<<<<< HEAD
static int nfs_launder_page(struct page *page)
{
	struct inode *inode = page->mapping->host;
	struct nfs_inode *nfsi = NFS_I(inode);

	dfprintk(PAGECACHE, "NFS: launder_page(%ld, %llu)\n",
		inode->i_ino, (long long)page_offset(page));

	nfs_fscache_wait_on_page_write(nfsi, page);
	return nfs_wb_page(inode, page);
}

const struct address_space_operations nfs_file_aops = {
	.readpage = nfs_readpage,
	.readpages = nfs_readpages,
	.set_page_dirty = __set_page_dirty_nobuffers,
	.writepage = nfs_writepage,
	.writepages = nfs_writepages,
	.write_begin = nfs_write_begin,
	.write_end = nfs_write_end,
	.invalidatepage = nfs_invalidate_page,
	.releasepage = nfs_release_page,
	.direct_IO = nfs_direct_IO,
	.migratepage = nfs_migrate_page,
	.launder_page = nfs_launder_page,
	.error_remove_page = generic_error_remove_page,
=======
static int nfs_launder_folio(struct folio *folio)
{
	struct inode *inode = folio->mapping->host;
	int ret;

	dfprintk(PAGECACHE, "NFS: launder_folio(%ld, %llu)\n",
		inode->i_ino, folio_pos(folio));

	folio_wait_fscache(folio);
	ret = nfs_wb_folio(inode, folio);
	trace_nfs_launder_folio_done(inode, folio, ret);
	return ret;
}

static int nfs_swap_activate(struct swap_info_struct *sis, struct file *file,
						sector_t *span)
{
	unsigned long blocks;
	long long isize;
	int ret;
	struct inode *inode = file_inode(file);
	struct rpc_clnt *clnt = NFS_CLIENT(inode);
	struct nfs_client *cl = NFS_SERVER(inode)->nfs_client;

	spin_lock(&inode->i_lock);
	blocks = inode->i_blocks;
	isize = inode->i_size;
	spin_unlock(&inode->i_lock);
	if (blocks*512 < isize) {
		pr_warn("swap activate: swapfile has holes\n");
		return -EINVAL;
	}

	ret = rpc_clnt_swap_activate(clnt);
	if (ret)
		return ret;
	ret = add_swap_extent(sis, 0, sis->max, 0);
	if (ret < 0) {
		rpc_clnt_swap_deactivate(clnt);
		return ret;
	}

	*span = sis->pages;

	if (cl->rpc_ops->enable_swap)
		cl->rpc_ops->enable_swap(inode);

	sis->flags |= SWP_FS_OPS;
	return ret;
}

static void nfs_swap_deactivate(struct file *file)
{
	struct inode *inode = file_inode(file);
	struct rpc_clnt *clnt = NFS_CLIENT(inode);
	struct nfs_client *cl = NFS_SERVER(inode)->nfs_client;

	rpc_clnt_swap_deactivate(clnt);
	if (cl->rpc_ops->disable_swap)
		cl->rpc_ops->disable_swap(file_inode(file));
}

const struct address_space_operations nfs_file_aops = {
	.read_folio = nfs_read_folio,
	.readahead = nfs_readahead,
	.dirty_folio = filemap_dirty_folio,
	.writepages = nfs_writepages,
	.write_begin = nfs_write_begin,
	.write_end = nfs_write_end,
	.invalidate_folio = nfs_invalidate_folio,
	.release_folio = nfs_release_folio,
	.migrate_folio = nfs_migrate_folio,
	.launder_folio = nfs_launder_folio,
	.is_dirty_writeback = nfs_check_dirty_writeback,
	.error_remove_folio = generic_error_remove_folio,
	.swap_activate = nfs_swap_activate,
	.swap_deactivate = nfs_swap_deactivate,
	.swap_rw = nfs_swap_rw,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Notification that a PTE pointing to an NFS page is about to be made
 * writable, implying that someone is about to modify the page through a
 * shared-writable mapping
 */
<<<<<<< HEAD
static int nfs_vm_page_mkwrite(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct page *page = vmf->page;
	struct file *filp = vma->vm_file;
	struct dentry *dentry = filp->f_path.dentry;
	unsigned pagelen;
	int ret = VM_FAULT_NOPAGE;
	struct address_space *mapping;

	dfprintk(PAGECACHE, "NFS: vm_page_mkwrite(%s/%s(%ld), offset %lld)\n",
		dentry->d_parent->d_name.name, dentry->d_name.name,
		filp->f_mapping->host->i_ino,
		(long long)page_offset(page));

	/* make sure the cache has finished storing the page */
	nfs_fscache_wait_on_page_write(NFS_I(dentry->d_inode), page);

	lock_page(page);
	mapping = page->mapping;
	if (mapping != dentry->d_inode->i_mapping)
		goto out_unlock;

	wait_on_page_writeback(page);

	pagelen = nfs_page_length(page);
=======
static vm_fault_t nfs_vm_page_mkwrite(struct vm_fault *vmf)
{
	struct file *filp = vmf->vma->vm_file;
	struct inode *inode = file_inode(filp);
	unsigned pagelen;
	vm_fault_t ret = VM_FAULT_NOPAGE;
	struct address_space *mapping;
	struct folio *folio = page_folio(vmf->page);

	dfprintk(PAGECACHE, "NFS: vm_page_mkwrite(%pD2(%lu), offset %lld)\n",
		 filp, filp->f_mapping->host->i_ino,
		 (long long)folio_file_pos(folio));

	sb_start_pagefault(inode->i_sb);

	/* make sure the cache has finished storing the page */
	if (folio_test_fscache(folio) &&
	    folio_wait_fscache_killable(folio) < 0) {
		ret = VM_FAULT_RETRY;
		goto out;
	}

	wait_on_bit_action(&NFS_I(inode)->flags, NFS_INO_INVALIDATING,
			   nfs_wait_bit_killable,
			   TASK_KILLABLE|TASK_FREEZABLE_UNSAFE);

	folio_lock(folio);
	mapping = folio_file_mapping(folio);
	if (mapping != inode->i_mapping)
		goto out_unlock;

	folio_wait_writeback(folio);

	pagelen = nfs_folio_length(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pagelen == 0)
		goto out_unlock;

	ret = VM_FAULT_LOCKED;
<<<<<<< HEAD
	if (nfs_flush_incompatible(filp, page) == 0 &&
	    nfs_updatepage(filp, page, 0, pagelen) == 0)
=======
	if (nfs_flush_incompatible(filp, folio) == 0 &&
	    nfs_update_folio(filp, folio, 0, pagelen) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	ret = VM_FAULT_SIGBUS;
out_unlock:
<<<<<<< HEAD
	unlock_page(page);
out:
=======
	folio_unlock(folio);
out:
	sb_end_pagefault(inode->i_sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static const struct vm_operations_struct nfs_file_vm_ops = {
	.fault = filemap_fault,
<<<<<<< HEAD
	.page_mkwrite = nfs_vm_page_mkwrite,
};

static int nfs_need_sync_write(struct file *filp, struct inode *inode)
{
	struct nfs_open_context *ctx;

	if (IS_SYNC(inode) || (filp->f_flags & O_DSYNC))
		return 1;
	ctx = nfs_file_open_context(filp);
	if (test_bit(NFS_CONTEXT_ERROR_WRITE, &ctx->flags))
		return 1;
	return 0;
}

static ssize_t nfs_file_write(struct kiocb *iocb, const struct iovec *iov,
				unsigned long nr_segs, loff_t pos)
{
	struct dentry * dentry = iocb->ki_filp->f_path.dentry;
	struct inode * inode = dentry->d_inode;
	unsigned long written = 0;
	ssize_t result;
	size_t count = iov_length(iov, nr_segs);

	if (iocb->ki_filp->f_flags & O_DIRECT)
		return nfs_file_direct_write(iocb, iov, nr_segs, pos);

	dprintk("NFS: write(%s/%s, %lu@%Ld)\n",
		dentry->d_parent->d_name.name, dentry->d_name.name,
		(unsigned long) count, (long long) pos);

	result = -EBUSY;
=======
	.map_pages = filemap_map_pages,
	.page_mkwrite = nfs_vm_page_mkwrite,
};

ssize_t nfs_file_write(struct kiocb *iocb, struct iov_iter *from)
{
	struct file *file = iocb->ki_filp;
	struct inode *inode = file_inode(file);
	unsigned int mntflags = NFS_SERVER(inode)->flags;
	ssize_t result, written;
	errseq_t since;
	int error;

	result = nfs_key_timeout_notify(file, inode);
	if (result)
		return result;

	if (iocb->ki_flags & IOCB_DIRECT)
		return nfs_file_direct_write(iocb, from, false);

	dprintk("NFS: write(%pD2, %zu@%Ld)\n",
		file, iov_iter_count(from), (long long) iocb->ki_pos);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_SWAPFILE(inode))
		goto out_swapfile;
	/*
	 * O_APPEND implies that we must revalidate the file length.
	 */
<<<<<<< HEAD
	if (iocb->ki_filp->f_flags & O_APPEND) {
		result = nfs_revalidate_file_size(inode, iocb->ki_filp);
		if (result)
			goto out;
	}

	result = count;
	if (!count)
		goto out;

	result = generic_file_aio_write(iocb, iov, nr_segs, pos);
	if (result > 0)
		written = result;

	/* Return error values for O_DSYNC and IS_SYNC() */
	if (result >= 0 && nfs_need_sync_write(iocb->ki_filp, inode)) {
		int err = vfs_fsync(iocb->ki_filp, 0);
		if (err < 0)
			result = err;
	}
	if (result > 0)
		nfs_add_stats(inode, NFSIOS_NORMALWRITTENBYTES, written);
out:
=======
	if (iocb->ki_flags & IOCB_APPEND || iocb->ki_pos > i_size_read(inode)) {
		result = nfs_revalidate_file_size(inode, file);
		if (result)
			return result;
	}

	nfs_clear_invalid_mapping(file->f_mapping);

	since = filemap_sample_wb_err(file->f_mapping);
	nfs_start_io_write(inode);
	result = generic_write_checks(iocb, from);
	if (result > 0)
		result = generic_perform_write(iocb, from);
	nfs_end_io_write(inode);
	if (result <= 0)
		goto out;

	written = result;
	nfs_add_stats(inode, NFSIOS_NORMALWRITTENBYTES, written);

	if (mntflags & NFS_MOUNT_WRITE_EAGER) {
		result = filemap_fdatawrite_range(file->f_mapping,
						  iocb->ki_pos - written,
						  iocb->ki_pos - 1);
		if (result < 0)
			goto out;
	}
	if (mntflags & NFS_MOUNT_WRITE_WAIT) {
		filemap_fdatawait_range(file->f_mapping,
					iocb->ki_pos - written,
					iocb->ki_pos - 1);
	}
	result = generic_write_sync(iocb, written);
	if (result < 0)
		return result;

out:
	/* Return error values */
	error = filemap_check_wb_err(file->f_mapping, since);
	switch (error) {
	default:
		break;
	case -EDQUOT:
	case -EFBIG:
	case -ENOSPC:
		nfs_wb_all(inode);
		error = file_check_and_advance_wb_err(file);
		if (error < 0)
			result = error;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return result;

out_swapfile:
	printk(KERN_INFO "NFS: attempt to write to active swap file!\n");
<<<<<<< HEAD
	goto out;
}

static ssize_t nfs_file_splice_write(struct pipe_inode_info *pipe,
				     struct file *filp, loff_t *ppos,
				     size_t count, unsigned int flags)
{
	struct dentry *dentry = filp->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	unsigned long written = 0;
	ssize_t ret;

	dprintk("NFS splice_write(%s/%s, %lu@%llu)\n",
		dentry->d_parent->d_name.name, dentry->d_name.name,
		(unsigned long) count, (unsigned long long) *ppos);

	/*
	 * The combination of splice and an O_APPEND destination is disallowed.
	 */

	ret = generic_file_splice_write(pipe, filp, ppos, count, flags);
	if (ret > 0)
		written = ret;

	if (ret >= 0 && nfs_need_sync_write(filp, inode)) {
		int err = vfs_fsync(filp, 0);
		if (err < 0)
			ret = err;
	}
	if (ret > 0)
		nfs_add_stats(inode, NFSIOS_NORMALWRITTENBYTES, written);
	return ret;
}
=======
	return -ETXTBSY;
}
EXPORT_SYMBOL_GPL(nfs_file_write);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int
do_getlk(struct file *filp, int cmd, struct file_lock *fl, int is_local)
{
	struct inode *inode = filp->f_mapping->host;
	int status = 0;
<<<<<<< HEAD
	unsigned int saved_type = fl->fl_type;

	/* Try local locking first */
	posix_test_lock(filp, fl);
	if (fl->fl_type != F_UNLCK) {
		/* found a conflict */
		goto out;
	}
	fl->fl_type = saved_type;

	if (nfs_have_delegation(inode, FMODE_READ))
=======
	unsigned int saved_type = fl->c.flc_type;

	/* Try local locking first */
	posix_test_lock(filp, fl);
	if (fl->c.flc_type != F_UNLCK) {
		/* found a conflict */
		goto out;
	}
	fl->c.flc_type = saved_type;

	if (NFS_PROTO(inode)->have_delegation(inode, FMODE_READ))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_noconflict;

	if (is_local)
		goto out_noconflict;

	status = NFS_PROTO(inode)->lock(filp, cmd, fl);
out:
	return status;
out_noconflict:
<<<<<<< HEAD
	fl->fl_type = F_UNLCK;
	goto out;
}

static int do_vfs_lock(struct file *file, struct file_lock *fl)
{
	int res = 0;
	switch (fl->fl_flags & (FL_POSIX|FL_FLOCK)) {
		case FL_POSIX:
			res = posix_lock_file_wait(file, fl);
			break;
		case FL_FLOCK:
			res = flock_lock_file_wait(file, fl);
			break;
		default:
			BUG();
	}
	return res;
}

=======
	fl->c.flc_type = F_UNLCK;
	goto out;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int
do_unlk(struct file *filp, int cmd, struct file_lock *fl, int is_local)
{
	struct inode *inode = filp->f_mapping->host;
<<<<<<< HEAD
=======
	struct nfs_lock_context *l_ctx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int status;

	/*
	 * Flush all pending writes before doing anything
	 * with locks..
	 */
<<<<<<< HEAD
	nfs_sync_mapping(filp->f_mapping);

	/* NOTE: special case
	 * 	If we're signalled while cleaning up locks on process exit, we
	 * 	still need to complete the unlock.
	 */
=======
	nfs_wb_all(inode);

	l_ctx = nfs_get_lock_context(nfs_file_open_context(filp));
	if (!IS_ERR(l_ctx)) {
		status = nfs_iocounter_wait(l_ctx);
		nfs_put_lock_context(l_ctx);
		/*  NOTE: special case
		 * 	If we're signalled while cleaning up locks on process exit, we
		 * 	still need to complete the unlock.
		 */
		if (status < 0 && !(fl->c.flc_flags & FL_CLOSE))
			return status;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Use local locking if mounted with "-onolock" or with appropriate
	 * "-olocal_lock="
	 */
	if (!is_local)
		status = NFS_PROTO(inode)->lock(filp, cmd, fl);
	else
<<<<<<< HEAD
		status = do_vfs_lock(filp, fl);
=======
		status = locks_lock_file_wait(filp, fl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return status;
}

static int
<<<<<<< HEAD
is_time_granular(struct timespec *ts) {
	return ((ts->tv_sec == 0) && (ts->tv_nsec <= 1000));
}

static int
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
do_setlk(struct file *filp, int cmd, struct file_lock *fl, int is_local)
{
	struct inode *inode = filp->f_mapping->host;
	int status;

	/*
	 * Flush all pending writes before doing anything
	 * with locks..
	 */
	status = nfs_sync_mapping(filp->f_mapping);
	if (status != 0)
		goto out;

	/*
	 * Use local locking if mounted with "-onolock" or with appropriate
	 * "-olocal_lock="
	 */
	if (!is_local)
		status = NFS_PROTO(inode)->lock(filp, cmd, fl);
	else
<<<<<<< HEAD
		status = do_vfs_lock(filp, fl);
=======
		status = locks_lock_file_wait(filp, fl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (status < 0)
		goto out;

	/*
<<<<<<< HEAD
	 * Revalidate the cache if the server has time stamps granular
	 * enough to detect subsecond changes.  Otherwise, clear the
	 * cache to prevent missing any changes.
=======
	 * Invalidate cache to prevent missing any changes.  If
	 * the file is mapped, clear the page cache as well so
	 * those mappings will be loaded.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *
	 * This makes locking act as a cache coherency point.
	 */
	nfs_sync_mapping(filp->f_mapping);
<<<<<<< HEAD
	if (!nfs_have_delegation(inode, FMODE_READ)) {
		if (is_time_granular(&NFS_SERVER(inode)->time_delta))
			__nfs_revalidate_inode(NFS_SERVER(inode), inode);
		else
			nfs_zap_caches(inode);
=======
	if (!NFS_PROTO(inode)->have_delegation(inode, FMODE_READ)) {
		nfs_zap_caches(inode);
		if (mapping_mapped(filp->f_mapping))
			nfs_revalidate_mapping(inode, filp->f_mapping);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
out:
	return status;
}

/*
 * Lock a (portion of) a file
 */
<<<<<<< HEAD
static int nfs_lock(struct file *filp, int cmd, struct file_lock *fl)
=======
int nfs_lock(struct file *filp, int cmd, struct file_lock *fl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode = filp->f_mapping->host;
	int ret = -ENOLCK;
	int is_local = 0;

<<<<<<< HEAD
	dprintk("NFS: lock(%s/%s, t=%x, fl=%x, r=%lld:%lld)\n",
			filp->f_path.dentry->d_parent->d_name.name,
			filp->f_path.dentry->d_name.name,
			fl->fl_type, fl->fl_flags,
=======
	dprintk("NFS: lock(%pD2, t=%x, fl=%x, r=%lld:%lld)\n",
			filp, fl->c.flc_type, fl->c.flc_flags,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			(long long)fl->fl_start, (long long)fl->fl_end);

	nfs_inc_stats(inode, NFSIOS_VFSLOCK);

<<<<<<< HEAD
	/* No mandatory locks over NFS */
	if (__mandatory_lock(inode) && fl->fl_type != F_UNLCK)
		goto out_err;
=======
	if (fl->c.flc_flags & FL_RECLAIM)
		return -ENOGRACE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (NFS_SERVER(inode)->flags & NFS_MOUNT_LOCAL_FCNTL)
		is_local = 1;

	if (NFS_PROTO(inode)->lock_check_bounds != NULL) {
		ret = NFS_PROTO(inode)->lock_check_bounds(fl);
		if (ret < 0)
			goto out_err;
	}

	if (IS_GETLK(cmd))
		ret = do_getlk(filp, cmd, fl, is_local);
<<<<<<< HEAD
	else if (fl->fl_type == F_UNLCK)
=======
	else if (lock_is_unlock(fl))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = do_unlk(filp, cmd, fl, is_local);
	else
		ret = do_setlk(filp, cmd, fl, is_local);
out_err:
	return ret;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Lock a (portion of) a file
 */
<<<<<<< HEAD
static int nfs_flock(struct file *filp, int cmd, struct file_lock *fl)
=======
int nfs_flock(struct file *filp, int cmd, struct file_lock *fl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode = filp->f_mapping->host;
	int is_local = 0;

<<<<<<< HEAD
	dprintk("NFS: flock(%s/%s, t=%x, fl=%x)\n",
			filp->f_path.dentry->d_parent->d_name.name,
			filp->f_path.dentry->d_name.name,
			fl->fl_type, fl->fl_flags);

	if (!(fl->fl_flags & FL_FLOCK))
=======
	dprintk("NFS: flock(%pD2, t=%x, fl=%x)\n",
			filp, fl->c.flc_type, fl->c.flc_flags);

	if (!(fl->c.flc_flags & FL_FLOCK))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOLCK;

	if (NFS_SERVER(inode)->flags & NFS_MOUNT_LOCAL_FLOCK)
		is_local = 1;

	/* We're simulating flock() locks using posix locks on the server */
<<<<<<< HEAD
	fl->fl_owner = (fl_owner_t)filp;
	fl->fl_start = 0;
	fl->fl_end = OFFSET_MAX;

	if (fl->fl_type == F_UNLCK)
		return do_unlk(filp, cmd, fl, is_local);
	return do_setlk(filp, cmd, fl, is_local);
}

/*
 * There is no protocol support for leases, so we have no way to implement
 * them correctly in the face of opens by other clients.
 */
static int nfs_setlease(struct file *file, long arg, struct file_lock **fl)
{
	dprintk("NFS: setlease(%s/%s, arg=%ld)\n",
			file->f_path.dentry->d_parent->d_name.name,
			file->f_path.dentry->d_name.name, arg);
	return -EINVAL;
}

const struct file_operations nfs_file_operations = {
	.llseek		= nfs_file_llseek,
	.read		= do_sync_read,
	.write		= do_sync_write,
	.aio_read	= nfs_file_read,
	.aio_write	= nfs_file_write,
=======
	if (lock_is_unlock(fl))
		return do_unlk(filp, cmd, fl, is_local);
	return do_setlk(filp, cmd, fl, is_local);
}
EXPORT_SYMBOL_GPL(nfs_flock);

const struct file_operations nfs_file_operations = {
	.llseek		= nfs_file_llseek,
	.read_iter	= nfs_file_read,
	.write_iter	= nfs_file_write,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.mmap		= nfs_file_mmap,
	.open		= nfs_file_open,
	.flush		= nfs_file_flush,
	.release	= nfs_file_release,
	.fsync		= nfs_file_fsync,
	.lock		= nfs_lock,
	.flock		= nfs_flock,
	.splice_read	= nfs_file_splice_read,
<<<<<<< HEAD
	.splice_write	= nfs_file_splice_write,
	.check_flags	= nfs_check_flags,
	.setlease	= nfs_setlease,
};

#ifdef CONFIG_NFS_V4
static int
nfs4_file_open(struct inode *inode, struct file *filp)
{
	struct nfs_open_context *ctx;
	struct dentry *dentry = filp->f_path.dentry;
	struct dentry *parent = NULL;
	struct inode *dir;
	unsigned openflags = filp->f_flags;
	struct iattr attr;
	int err;

	BUG_ON(inode != dentry->d_inode);
	/*
	 * If no cached dentry exists or if it's negative, NFSv4 handled the
	 * opens in ->lookup() or ->create().
	 *
	 * We only get this far for a cached positive dentry.  We skipped
	 * revalidation, so handle it here by dropping the dentry and returning
	 * -EOPENSTALE.  The VFS will retry the lookup/create/open.
	 */

	dprintk("NFS: open file(%s/%s)\n",
		dentry->d_parent->d_name.name,
		dentry->d_name.name);

	if ((openflags & O_ACCMODE) == 3)
		openflags--;

	/* We can't create new files here */
	openflags &= ~(O_CREAT|O_EXCL);

	parent = dget_parent(dentry);
	dir = parent->d_inode;

	ctx = alloc_nfs_open_context(filp->f_path.dentry, filp->f_mode);
	err = PTR_ERR(ctx);
	if (IS_ERR(ctx))
		goto out;

	attr.ia_valid = ATTR_OPEN;
	if (openflags & O_TRUNC) {
		attr.ia_valid |= ATTR_SIZE;
		attr.ia_size = 0;
		nfs_wb_all(inode);
	}

	inode = NFS_PROTO(dir)->open_context(dir, ctx, openflags, &attr);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		switch (err) {
		case -EPERM:
		case -EACCES:
		case -EDQUOT:
		case -ENOSPC:
		case -EROFS:
			goto out_put_ctx;
		default:
			goto out_drop;
		}
	}
	iput(inode);
	if (inode != dentry->d_inode)
		goto out_drop;

	nfs_set_verifier(dentry, nfs_save_change_attribute(dir));
	nfs_file_set_open_context(filp, ctx);
	err = 0;

out_put_ctx:
	put_nfs_open_context(ctx);
out:
	dput(parent);
	return err;

out_drop:
	d_drop(dentry);
	err = -EOPENSTALE;
	goto out_put_ctx;
}

const struct file_operations nfs4_file_operations = {
	.llseek		= nfs_file_llseek,
	.read		= do_sync_read,
	.write		= do_sync_write,
	.aio_read	= nfs_file_read,
	.aio_write	= nfs_file_write,
	.mmap		= nfs_file_mmap,
	.open		= nfs4_file_open,
	.flush		= nfs_file_flush,
	.release	= nfs_file_release,
	.fsync		= nfs_file_fsync,
	.lock		= nfs_lock,
	.flock		= nfs_flock,
	.splice_read	= nfs_file_splice_read,
	.splice_write	= nfs_file_splice_write,
	.check_flags	= nfs_check_flags,
	.setlease	= nfs_setlease,
};
#endif /* CONFIG_NFS_V4 */
=======
	.splice_write	= iter_file_splice_write,
	.check_flags	= nfs_check_flags,
	.setlease	= simple_nosetlease,
};
EXPORT_SYMBOL_GPL(nfs_file_operations);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
