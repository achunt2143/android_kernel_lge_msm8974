/*
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 *
 * Ported the filesystem routines to 2.5.
 * 2003-02-10 Petr Baudis <pasky@ucw.cz>
 */

#include <linux/fs.h>
<<<<<<< HEAD
=======
#include <linux/magic.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/statfs.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
#include <linux/mount.h>
#include <linux/namei.h>
#include "hostfs.h"
#include "init.h"
#include "kern.h"
=======
#include <linux/writeback.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include "hostfs.h"
#include <init.h>
#include <kern.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct hostfs_inode_info {
	int fd;
	fmode_t mode;
	struct inode vfs_inode;
<<<<<<< HEAD
=======
	struct mutex open_mutex;
	dev_t dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static inline struct hostfs_inode_info *HOSTFS_I(struct inode *inode)
{
	return list_entry(inode, struct hostfs_inode_info, vfs_inode);
}

<<<<<<< HEAD
#define FILE_HOSTFS_I(file) HOSTFS_I((file)->f_path.dentry->d_inode)

static int hostfs_d_delete(const struct dentry *dentry)
{
	return 1;
}

static const struct dentry_operations hostfs_dentry_ops = {
	.d_delete		= hostfs_d_delete,
};
=======
#define FILE_HOSTFS_I(file) HOSTFS_I(file_inode(file))

static struct kmem_cache *hostfs_inode_cache;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Changed in hostfs_args before the kernel starts running */
static char *root_ino = "";
static int append = 0;

<<<<<<< HEAD
#define HOSTFS_SUPER_MAGIC 0x00c0ffee

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct inode_operations hostfs_iops;
static const struct inode_operations hostfs_dir_iops;
static const struct inode_operations hostfs_link_iops;

#ifndef MODULE
static int __init hostfs_args(char *options, int *add)
{
	char *ptr;

	ptr = strchr(options, ',');
	if (ptr != NULL)
		*ptr++ = '\0';
	if (*options != '\0')
		root_ino = options;

	options = ptr;
	while (options) {
		ptr = strchr(options, ',');
		if (ptr != NULL)
			*ptr++ = '\0';
		if (*options != '\0') {
			if (!strcmp(options, "append"))
				append = 1;
			else printf("hostfs_args - unsupported option - %s\n",
				    options);
		}
		options = ptr;
	}
	return 0;
}

__uml_setup("hostfs=", hostfs_args,
"hostfs=<root dir>,<flags>,...\n"
"    This is used to set hostfs parameters.  The root directory argument\n"
"    is used to confine all hostfs mounts to within the specified directory\n"
"    tree on the host.  If this isn't specified, then a user inside UML can\n"
"    mount anything on the host that's accessible to the user that's running\n"
"    it.\n"
"    The only flag currently supported is 'append', which specifies that all\n"
"    files opened by hostfs will be opened in append mode.\n\n"
);
#endif

static char *__dentry_name(struct dentry *dentry, char *name)
{
	char *p = dentry_path_raw(dentry, name, PATH_MAX);
	char *root;
	size_t len;

	root = dentry->d_sb->s_fs_info;
	len = strlen(root);
	if (IS_ERR(p)) {
		__putname(name);
		return NULL;
	}
<<<<<<< HEAD
	strlcpy(name, root, PATH_MAX);
=======

	/*
	 * This function relies on the fact that dentry_path_raw() will place
	 * the path name at the end of the provided buffer.
	 */
	BUG_ON(p + strlen(p) + 1 != name + PATH_MAX);

	strscpy(name, root, PATH_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (len > p - name) {
		__putname(name);
		return NULL;
	}
<<<<<<< HEAD
	if (p > name + len) {
		char *s = name + len;
		while ((*s++ = *p++) != '\0')
			;
	}
=======

	if (p > name + len)
		strcpy(name + len, p);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return name;
}

static char *dentry_name(struct dentry *dentry)
{
	char *name = __getname();
	if (!name)
		return NULL;

<<<<<<< HEAD
	return __dentry_name(dentry, name); /* will unlock */
=======
	return __dentry_name(dentry, name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static char *inode_name(struct inode *ino)
{
	struct dentry *dentry;
	char *name;

	dentry = d_find_alias(ino);
	if (!dentry)
		return NULL;

	name = dentry_name(dentry);

	dput(dentry);

	return name;
}

static char *follow_link(char *link)
{
<<<<<<< HEAD
	int len, n;
	char *name, *resolved, *end;

	len = 64;
	while (1) {
		n = -ENOMEM;
		name = kmalloc(len, GFP_KERNEL);
		if (name == NULL)
			goto out;

		n = hostfs_do_readlink(link, name, len);
		if (n < len)
			break;
		len *= 2;
		kfree(name);
	}
	if (n < 0)
		goto out_free;
=======
	char *name, *resolved, *end;
	int n;

	name = kmalloc(PATH_MAX, GFP_KERNEL);
	if (!name) {
		n = -ENOMEM;
		goto out_free;
	}

	n = hostfs_do_readlink(link, name, PATH_MAX);
	if (n < 0)
		goto out_free;
	else if (n == PATH_MAX) {
		n = -E2BIG;
		goto out_free;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (*name == '/')
		return name;

	end = strrchr(link, '/');
	if (end == NULL)
		return name;

	*(end + 1) = '\0';
<<<<<<< HEAD
	len = strlen(link) + strlen(name) + 1;

	resolved = kmalloc(len, GFP_KERNEL);
=======

	resolved = kasprintf(GFP_KERNEL, "%s%s", link, name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (resolved == NULL) {
		n = -ENOMEM;
		goto out_free;
	}

<<<<<<< HEAD
	sprintf(resolved, "%s%s", link, name);
	kfree(name);
	kfree(link);
=======
	kfree(name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return resolved;

 out_free:
	kfree(name);
<<<<<<< HEAD
 out:
	return ERR_PTR(n);
}

static struct inode *hostfs_iget(struct super_block *sb)
{
	struct inode *inode = new_inode(sb);
	if (!inode)
		return ERR_PTR(-ENOMEM);
	return inode;
}

int hostfs_statfs(struct dentry *dentry, struct kstatfs *sf)
=======
	return ERR_PTR(n);
}

static int hostfs_statfs(struct dentry *dentry, struct kstatfs *sf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * do_statfs uses struct statfs64 internally, but the linux kernel
	 * struct statfs still has 32-bit versions for most of these fields,
	 * so we convert them here
	 */
	int err;
	long long f_blocks;
	long long f_bfree;
	long long f_bavail;
	long long f_files;
	long long f_ffree;

	err = do_statfs(dentry->d_sb->s_fs_info,
			&sf->f_bsize, &f_blocks, &f_bfree, &f_bavail, &f_files,
			&f_ffree, &sf->f_fsid, sizeof(sf->f_fsid),
			&sf->f_namelen);
	if (err)
		return err;
	sf->f_blocks = f_blocks;
	sf->f_bfree = f_bfree;
	sf->f_bavail = f_bavail;
	sf->f_files = f_files;
	sf->f_ffree = f_ffree;
	sf->f_type = HOSTFS_SUPER_MAGIC;
	return 0;
}

static struct inode *hostfs_alloc_inode(struct super_block *sb)
{
	struct hostfs_inode_info *hi;

<<<<<<< HEAD
	hi = kzalloc(sizeof(*hi), GFP_KERNEL);
	if (hi == NULL)
		return NULL;
	hi->fd = -1;
	inode_init_once(&hi->vfs_inode);
=======
	hi = alloc_inode_sb(sb, hostfs_inode_cache, GFP_KERNEL_ACCOUNT);
	if (hi == NULL)
		return NULL;
	hi->fd = -1;
	hi->mode = 0;
	hi->dev = 0;
	inode_init_once(&hi->vfs_inode);
	mutex_init(&hi->open_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return &hi->vfs_inode;
}

static void hostfs_evict_inode(struct inode *inode)
{
<<<<<<< HEAD
	truncate_inode_pages(&inode->i_data, 0);
	end_writeback(inode);
	if (HOSTFS_I(inode)->fd != -1) {
		close_file(&HOSTFS_I(inode)->fd);
		HOSTFS_I(inode)->fd = -1;
	}
}

static void hostfs_i_callback(struct rcu_head *head)
{
	struct inode *inode = container_of(head, struct inode, i_rcu);
	kfree(HOSTFS_I(inode));
}

static void hostfs_destroy_inode(struct inode *inode)
{
	call_rcu(&inode->i_rcu, hostfs_i_callback);
=======
	truncate_inode_pages_final(&inode->i_data);
	clear_inode(inode);
	if (HOSTFS_I(inode)->fd != -1) {
		close_file(&HOSTFS_I(inode)->fd);
		HOSTFS_I(inode)->fd = -1;
		HOSTFS_I(inode)->dev = 0;
	}
}

static void hostfs_free_inode(struct inode *inode)
{
	kmem_cache_free(hostfs_inode_cache, HOSTFS_I(inode));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int hostfs_show_options(struct seq_file *seq, struct dentry *root)
{
	const char *root_path = root->d_sb->s_fs_info;
	size_t offset = strlen(root_ino) + 1;

	if (strlen(root_path) > offset)
		seq_show_option(seq, root_path + offset, NULL);

<<<<<<< HEAD
=======
	if (append)
		seq_puts(seq, ",append");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct super_operations hostfs_sbops = {
	.alloc_inode	= hostfs_alloc_inode,
<<<<<<< HEAD
	.destroy_inode	= hostfs_destroy_inode,
=======
	.free_inode	= hostfs_free_inode,
	.drop_inode	= generic_delete_inode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.evict_inode	= hostfs_evict_inode,
	.statfs		= hostfs_statfs,
	.show_options	= hostfs_show_options,
};

<<<<<<< HEAD
int hostfs_readdir(struct file *file, void *ent, filldir_t filldir)
=======
static int hostfs_readdir(struct file *file, struct dir_context *ctx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	void *dir;
	char *name;
	unsigned long long next, ino;
	int error, len;
	unsigned int type;

	name = dentry_name(file->f_path.dentry);
	if (name == NULL)
		return -ENOMEM;
	dir = open_dir(name, &error);
	__putname(name);
	if (dir == NULL)
		return -error;
<<<<<<< HEAD
	next = file->f_pos;
	while ((name = read_dir(dir, &next, &ino, &len, &type)) != NULL) {
		error = (*filldir)(ent, name, len, file->f_pos,
				   ino, type);
		if (error) break;
		file->f_pos = next;
=======
	next = ctx->pos;
	seek_dir(dir, next);
	while ((name = read_dir(dir, &next, &ino, &len, &type)) != NULL) {
		if (!dir_emit(ctx, name, len, ino, type))
			break;
		ctx->pos = next;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	close_dir(dir);
	return 0;
}

<<<<<<< HEAD
int hostfs_file_open(struct inode *ino, struct file *file)
{
	static DEFINE_MUTEX(open_mutex);
	char *name;
	fmode_t mode = 0;
	int err;
	int r = 0, w = 0, fd;
=======
static int hostfs_open(struct inode *ino, struct file *file)
{
	char *name;
	fmode_t mode;
	int err;
	int r, w, fd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mode = file->f_mode & (FMODE_READ | FMODE_WRITE);
	if ((mode & HOSTFS_I(ino)->mode) == mode)
		return 0;

	mode |= HOSTFS_I(ino)->mode;

retry:
<<<<<<< HEAD
	if (mode & FMODE_READ)
		r = 1;
	if (mode & FMODE_WRITE)
		w = 1;
	if (w)
		r = 1;

	name = dentry_name(file->f_path.dentry);
=======
	r = w = 0;

	if (mode & FMODE_READ)
		r = 1;
	if (mode & FMODE_WRITE)
		r = w = 1;

	name = dentry_name(file_dentry(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (name == NULL)
		return -ENOMEM;

	fd = open_file(name, r, w, append);
	__putname(name);
	if (fd < 0)
		return fd;

<<<<<<< HEAD
	mutex_lock(&open_mutex);
	/* somebody else had handled it first? */
	if ((mode & HOSTFS_I(ino)->mode) == mode) {
		mutex_unlock(&open_mutex);
=======
	mutex_lock(&HOSTFS_I(ino)->open_mutex);
	/* somebody else had handled it first? */
	if ((mode & HOSTFS_I(ino)->mode) == mode) {
		mutex_unlock(&HOSTFS_I(ino)->open_mutex);
		close_file(&fd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	if ((mode | HOSTFS_I(ino)->mode) != mode) {
		mode |= HOSTFS_I(ino)->mode;
<<<<<<< HEAD
		mutex_unlock(&open_mutex);
=======
		mutex_unlock(&HOSTFS_I(ino)->open_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		close_file(&fd);
		goto retry;
	}
	if (HOSTFS_I(ino)->fd == -1) {
		HOSTFS_I(ino)->fd = fd;
	} else {
		err = replace_file(fd, HOSTFS_I(ino)->fd);
		close_file(&fd);
		if (err < 0) {
<<<<<<< HEAD
			mutex_unlock(&open_mutex);
=======
			mutex_unlock(&HOSTFS_I(ino)->open_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return err;
		}
	}
	HOSTFS_I(ino)->mode = mode;
<<<<<<< HEAD
	mutex_unlock(&open_mutex);
=======
	mutex_unlock(&HOSTFS_I(ino)->open_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
int hostfs_fsync(struct file *file, loff_t start, loff_t end, int datasync)
=======
static int hostfs_file_release(struct inode *inode, struct file *file)
{
	filemap_write_and_wait(inode->i_mapping);

	return 0;
}

static int hostfs_fsync(struct file *file, loff_t start, loff_t end,
			int datasync)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode = file->f_mapping->host;
	int ret;

<<<<<<< HEAD
	ret = filemap_write_and_wait_range(inode->i_mapping, start, end);
	if (ret)
		return ret;

	mutex_lock(&inode->i_mutex);
	ret = fsync_file(HOSTFS_I(inode)->fd, datasync);
	mutex_unlock(&inode->i_mutex);
=======
	ret = file_write_and_wait_range(file, start, end);
	if (ret)
		return ret;

	inode_lock(inode);
	ret = fsync_file(HOSTFS_I(inode)->fd, datasync);
	inode_unlock(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static const struct file_operations hostfs_file_fops = {
	.llseek		= generic_file_llseek,
<<<<<<< HEAD
	.read		= do_sync_read,
	.splice_read	= generic_file_splice_read,
	.aio_read	= generic_file_aio_read,
	.aio_write	= generic_file_aio_write,
	.write		= do_sync_write,
	.mmap		= generic_file_mmap,
	.open		= hostfs_file_open,
	.release	= NULL,
=======
	.splice_read	= filemap_splice_read,
	.splice_write	= iter_file_splice_write,
	.read_iter	= generic_file_read_iter,
	.write_iter	= generic_file_write_iter,
	.mmap		= generic_file_mmap,
	.open		= hostfs_open,
	.release	= hostfs_file_release,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.fsync		= hostfs_fsync,
};

static const struct file_operations hostfs_dir_fops = {
	.llseek		= generic_file_llseek,
<<<<<<< HEAD
	.readdir	= hostfs_readdir,
	.read		= generic_read_dir,
};

int hostfs_writepage(struct page *page, struct writeback_control *wbc)
=======
	.iterate_shared	= hostfs_readdir,
	.read		= generic_read_dir,
	.open		= hostfs_open,
	.fsync		= hostfs_fsync,
};

static int hostfs_writepage(struct page *page, struct writeback_control *wbc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct address_space *mapping = page->mapping;
	struct inode *inode = mapping->host;
	char *buffer;
<<<<<<< HEAD
	unsigned long long base;
	int count = PAGE_CACHE_SIZE;
	int end_index = inode->i_size >> PAGE_CACHE_SHIFT;
	int err;

	if (page->index >= end_index)
		count = inode->i_size & (PAGE_CACHE_SIZE-1);

	buffer = kmap(page);
	base = ((unsigned long long) page->index) << PAGE_CACHE_SHIFT;

	err = write_file(HOSTFS_I(inode)->fd, &base, buffer, count);
	if (err != count) {
		ClearPageUptodate(page);
=======
	loff_t base = page_offset(page);
	int count = PAGE_SIZE;
	int end_index = inode->i_size >> PAGE_SHIFT;
	int err;

	if (page->index >= end_index)
		count = inode->i_size & (PAGE_SIZE-1);

	buffer = kmap_local_page(page);

	err = write_file(HOSTFS_I(inode)->fd, &base, buffer, count);
	if (err != count) {
		if (err >= 0)
			err = -EIO;
		mapping_set_error(mapping, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	if (base > inode->i_size)
		inode->i_size = base;

<<<<<<< HEAD
	if (PageError(page))
		ClearPageError(page);
	err = 0;

 out:
	kunmap(page);

	unlock_page(page);
	return err;
}

int hostfs_readpage(struct file *file, struct page *page)
{
	char *buffer;
	long long start;
	int err = 0;

	start = (long long) page->index << PAGE_CACHE_SHIFT;
	buffer = kmap(page);
	err = read_file(FILE_HOSTFS_I(file)->fd, &start, buffer,
			PAGE_CACHE_SIZE);
	if (err < 0)
		goto out;

	memset(&buffer[err], 0, PAGE_CACHE_SIZE - err);

	flush_dcache_page(page);
	SetPageUptodate(page);
	if (PageError(page)) ClearPageError(page);
	err = 0;
 out:
	kunmap(page);
	unlock_page(page);
	return err;
}

int hostfs_write_begin(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned flags,
			struct page **pagep, void **fsdata)
{
	pgoff_t index = pos >> PAGE_CACHE_SHIFT;

	*pagep = grab_cache_page_write_begin(mapping, index, flags);
=======
	err = 0;

 out:
	kunmap_local(buffer);
	unlock_page(page);

	return err;
}

static int hostfs_read_folio(struct file *file, struct folio *folio)
{
	struct page *page = &folio->page;
	char *buffer;
	loff_t start = page_offset(page);
	int bytes_read, ret = 0;

	buffer = kmap_local_page(page);
	bytes_read = read_file(FILE_HOSTFS_I(file)->fd, &start, buffer,
			PAGE_SIZE);
	if (bytes_read < 0) {
		ClearPageUptodate(page);
		SetPageError(page);
		ret = bytes_read;
		goto out;
	}

	memset(buffer + bytes_read, 0, PAGE_SIZE - bytes_read);

	ClearPageError(page);
	SetPageUptodate(page);

 out:
	flush_dcache_page(page);
	kunmap_local(buffer);
	unlock_page(page);

	return ret;
}

static int hostfs_write_begin(struct file *file, struct address_space *mapping,
			      loff_t pos, unsigned len,
			      struct page **pagep, void **fsdata)
{
	pgoff_t index = pos >> PAGE_SHIFT;

	*pagep = grab_cache_page_write_begin(mapping, index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!*pagep)
		return -ENOMEM;
	return 0;
}

<<<<<<< HEAD
int hostfs_write_end(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned copied,
			struct page *page, void *fsdata)
{
	struct inode *inode = mapping->host;
	void *buffer;
	unsigned from = pos & (PAGE_CACHE_SIZE - 1);
	int err;

	buffer = kmap(page);
	err = write_file(FILE_HOSTFS_I(file)->fd, &pos, buffer + from, copied);
	kunmap(page);

	if (!PageUptodate(page) && err == PAGE_CACHE_SIZE)
=======
static int hostfs_write_end(struct file *file, struct address_space *mapping,
			    loff_t pos, unsigned len, unsigned copied,
			    struct page *page, void *fsdata)
{
	struct inode *inode = mapping->host;
	void *buffer;
	unsigned from = pos & (PAGE_SIZE - 1);
	int err;

	buffer = kmap_local_page(page);
	err = write_file(FILE_HOSTFS_I(file)->fd, &pos, buffer + from, copied);
	kunmap_local(buffer);

	if (!PageUptodate(page) && err == PAGE_SIZE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		SetPageUptodate(page);

	/*
	 * If err > 0, write_file has added err to pos, so we are comparing
	 * i_size against the last byte written.
	 */
	if (err > 0 && (pos > inode->i_size))
		inode->i_size = pos;
	unlock_page(page);
<<<<<<< HEAD
	page_cache_release(page);
=======
	put_page(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

static const struct address_space_operations hostfs_aops = {
	.writepage 	= hostfs_writepage,
<<<<<<< HEAD
	.readpage	= hostfs_readpage,
	.set_page_dirty = __set_page_dirty_nobuffers,
=======
	.read_folio	= hostfs_read_folio,
	.dirty_folio	= filemap_dirty_folio,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.write_begin	= hostfs_write_begin,
	.write_end	= hostfs_write_end,
};

<<<<<<< HEAD
static int read_name(struct inode *ino, char *name)
{
	dev_t rdev;
	struct hostfs_stat st;
	int err = stat_file(name, &st, -1);
	if (err)
		return err;

	/* Reencode maj and min with the kernel encoding.*/
	rdev = MKDEV(st.maj, st.min);

	switch (st.mode & S_IFMT) {
=======
static int hostfs_inode_update(struct inode *ino, const struct hostfs_stat *st)
{
	set_nlink(ino, st->nlink);
	i_uid_write(ino, st->uid);
	i_gid_write(ino, st->gid);
	inode_set_atime_to_ts(ino, (struct timespec64){
			st->atime.tv_sec,
			st->atime.tv_nsec,
		});
	inode_set_mtime_to_ts(ino, (struct timespec64){
			st->mtime.tv_sec,
			st->mtime.tv_nsec,
		});
	inode_set_ctime(ino, st->ctime.tv_sec, st->ctime.tv_nsec);
	ino->i_size = st->size;
	ino->i_blocks = st->blocks;
	return 0;
}

static int hostfs_inode_set(struct inode *ino, void *data)
{
	struct hostfs_stat *st = data;
	dev_t rdev;

	/* Reencode maj and min with the kernel encoding.*/
	rdev = MKDEV(st->maj, st->min);

	switch (st->mode & S_IFMT) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case S_IFLNK:
		ino->i_op = &hostfs_link_iops;
		break;
	case S_IFDIR:
		ino->i_op = &hostfs_dir_iops;
		ino->i_fop = &hostfs_dir_fops;
		break;
	case S_IFCHR:
	case S_IFBLK:
	case S_IFIFO:
	case S_IFSOCK:
<<<<<<< HEAD
		init_special_inode(ino, st.mode & S_IFMT, rdev);
		ino->i_op = &hostfs_iops;
		break;

	default:
		ino->i_op = &hostfs_iops;
		ino->i_fop = &hostfs_file_fops;
		ino->i_mapping->a_ops = &hostfs_aops;
	}

	ino->i_ino = st.ino;
	ino->i_mode = st.mode;
	set_nlink(ino, st.nlink);
	ino->i_uid = st.uid;
	ino->i_gid = st.gid;
	ino->i_atime = st.atime;
	ino->i_mtime = st.mtime;
	ino->i_ctime = st.ctime;
	ino->i_size = st.size;
	ino->i_blocks = st.blocks;
	return 0;
}

int hostfs_create(struct inode *dir, struct dentry *dentry, umode_t mode,
		  bool excl)
{
	struct inode *inode;
	char *name;
	int error, fd;

	inode = hostfs_iget(dir->i_sb);
	if (IS_ERR(inode)) {
		error = PTR_ERR(inode);
		goto out;
	}

	error = -ENOMEM;
	name = dentry_name(dentry);
	if (name == NULL)
		goto out_put;

	fd = file_create(name,
			 mode & S_IRUSR, mode & S_IWUSR, mode & S_IXUSR,
			 mode & S_IRGRP, mode & S_IWGRP, mode & S_IXGRP,
			 mode & S_IROTH, mode & S_IWOTH, mode & S_IXOTH);
	if (fd < 0)
		error = fd;
	else
		error = read_name(inode, name);

	__putname(name);
	if (error)
		goto out_put;
=======
		init_special_inode(ino, st->mode & S_IFMT, rdev);
		ino->i_op = &hostfs_iops;
		break;
	case S_IFREG:
		ino->i_op = &hostfs_iops;
		ino->i_fop = &hostfs_file_fops;
		ino->i_mapping->a_ops = &hostfs_aops;
		break;
	default:
		return -EIO;
	}

	HOSTFS_I(ino)->dev = st->dev;
	ino->i_ino = st->ino;
	ino->i_mode = st->mode;
	return hostfs_inode_update(ino, st);
}

static int hostfs_inode_test(struct inode *inode, void *data)
{
	const struct hostfs_stat *st = data;

	return inode->i_ino == st->ino && HOSTFS_I(inode)->dev == st->dev;
}

static struct inode *hostfs_iget(struct super_block *sb, char *name)
{
	struct inode *inode;
	struct hostfs_stat st;
	int err = stat_file(name, &st, -1);

	if (err)
		return ERR_PTR(err);

	inode = iget5_locked(sb, st.ino, hostfs_inode_test, hostfs_inode_set,
			     &st);
	if (!inode)
		return ERR_PTR(-ENOMEM);

	if (inode->i_state & I_NEW) {
		unlock_new_inode(inode);
	} else {
		spin_lock(&inode->i_lock);
		hostfs_inode_update(inode, &st);
		spin_unlock(&inode->i_lock);
	}

	return inode;
}

static int hostfs_create(struct mnt_idmap *idmap, struct inode *dir,
			 struct dentry *dentry, umode_t mode, bool excl)
{
	struct inode *inode;
	char *name;
	int fd;

	name = dentry_name(dentry);
	if (name == NULL)
		return -ENOMEM;

	fd = file_create(name, mode & 0777);
	if (fd < 0) {
		__putname(name);
		return fd;
	}

	inode = hostfs_iget(dir->i_sb, name);
	__putname(name);
	if (IS_ERR(inode))
		return PTR_ERR(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	HOSTFS_I(inode)->fd = fd;
	HOSTFS_I(inode)->mode = FMODE_READ | FMODE_WRITE;
	d_instantiate(dentry, inode);
	return 0;
<<<<<<< HEAD

 out_put:
	iput(inode);
 out:
	return error;
}

struct dentry *hostfs_lookup(struct inode *ino, struct dentry *dentry,
			     unsigned int flags)
{
	struct inode *inode;
	char *name;
	int err;

	inode = hostfs_iget(ino->i_sb);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out;
	}

	err = -ENOMEM;
	name = dentry_name(dentry);
	if (name == NULL)
		goto out_put;

	err = read_name(inode, name);

	__putname(name);
	if (err == -ENOENT) {
		iput(inode);
		inode = NULL;
	}
	else if (err)
		goto out_put;

	d_add(dentry, inode);
	return NULL;

 out_put:
	iput(inode);
 out:
	return ERR_PTR(err);
}

int hostfs_link(struct dentry *to, struct inode *ino, struct dentry *from)
=======
}

static struct dentry *hostfs_lookup(struct inode *ino, struct dentry *dentry,
				    unsigned int flags)
{
	struct inode *inode = NULL;
	char *name;

	name = dentry_name(dentry);
	if (name == NULL)
		return ERR_PTR(-ENOMEM);

	inode = hostfs_iget(ino->i_sb, name);
	__putname(name);
	if (inode == ERR_PTR(-ENOENT))
		inode = NULL;

	return d_splice_alias(inode, dentry);
}

static int hostfs_link(struct dentry *to, struct inode *ino,
		       struct dentry *from)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *from_name, *to_name;
	int err;

	if ((from_name = dentry_name(from)) == NULL)
		return -ENOMEM;
	to_name = dentry_name(to);
	if (to_name == NULL) {
		__putname(from_name);
		return -ENOMEM;
	}
	err = link_file(to_name, from_name);
	__putname(from_name);
	__putname(to_name);
	return err;
}

<<<<<<< HEAD
int hostfs_unlink(struct inode *ino, struct dentry *dentry)
=======
static int hostfs_unlink(struct inode *ino, struct dentry *dentry)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *file;
	int err;

	if (append)
		return -EPERM;

	if ((file = dentry_name(dentry)) == NULL)
		return -ENOMEM;

	err = unlink_file(file);
	__putname(file);
	return err;
}

<<<<<<< HEAD
int hostfs_symlink(struct inode *ino, struct dentry *dentry, const char *to)
=======
static int hostfs_symlink(struct mnt_idmap *idmap, struct inode *ino,
			  struct dentry *dentry, const char *to)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *file;
	int err;

	if ((file = dentry_name(dentry)) == NULL)
		return -ENOMEM;
	err = make_symlink(file, to);
	__putname(file);
	return err;
}

<<<<<<< HEAD
int hostfs_mkdir(struct inode *ino, struct dentry *dentry, umode_t mode)
=======
static int hostfs_mkdir(struct mnt_idmap *idmap, struct inode *ino,
			struct dentry *dentry, umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *file;
	int err;

	if ((file = dentry_name(dentry)) == NULL)
		return -ENOMEM;
	err = do_mkdir(file, mode);
	__putname(file);
	return err;
}

<<<<<<< HEAD
int hostfs_rmdir(struct inode *ino, struct dentry *dentry)
=======
static int hostfs_rmdir(struct inode *ino, struct dentry *dentry)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *file;
	int err;

	if ((file = dentry_name(dentry)) == NULL)
		return -ENOMEM;
<<<<<<< HEAD
	err = do_rmdir(file);
=======
	err = hostfs_do_rmdir(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__putname(file);
	return err;
}

<<<<<<< HEAD
static int hostfs_mknod(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev)
=======
static int hostfs_mknod(struct mnt_idmap *idmap, struct inode *dir,
			struct dentry *dentry, umode_t mode, dev_t dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode;
	char *name;
	int err;

<<<<<<< HEAD
	inode = hostfs_iget(dir->i_sb);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out;
	}

	err = -ENOMEM;
	name = dentry_name(dentry);
	if (name == NULL)
		goto out_put;

	init_special_inode(inode, mode, dev);
	err = do_mknod(name, mode, MAJOR(dev), MINOR(dev));
	if (!err)
		goto out_free;

	err = read_name(inode, name);
	__putname(name);
	if (err)
		goto out_put;
	if (err)
		goto out_put;

	d_instantiate(dentry, inode);
	return 0;

 out_free:
	__putname(name);
 out_put:
	iput(inode);
 out:
	return err;
}

int hostfs_rename(struct inode *from_ino, struct dentry *from,
		  struct inode *to_ino, struct dentry *to)
{
	char *from_name, *to_name;
	int err;

	if ((from_name = dentry_name(from)) == NULL)
		return -ENOMEM;
	if ((to_name = dentry_name(to)) == NULL) {
		__putname(from_name);
		return -ENOMEM;
	}
	err = rename_file(from_name, to_name);
	__putname(from_name);
	__putname(to_name);
	return err;
}

int hostfs_permission(struct inode *ino, int desired)
=======
	name = dentry_name(dentry);
	if (name == NULL)
		return -ENOMEM;

	err = do_mknod(name, mode, MAJOR(dev), MINOR(dev));
	if (err) {
		__putname(name);
		return err;
	}

	inode = hostfs_iget(dir->i_sb, name);
	__putname(name);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

	d_instantiate(dentry, inode);
	return 0;
}

static int hostfs_rename2(struct mnt_idmap *idmap,
			  struct inode *old_dir, struct dentry *old_dentry,
			  struct inode *new_dir, struct dentry *new_dentry,
			  unsigned int flags)
{
	char *old_name, *new_name;
	int err;

	if (flags & ~(RENAME_NOREPLACE | RENAME_EXCHANGE))
		return -EINVAL;

	old_name = dentry_name(old_dentry);
	if (old_name == NULL)
		return -ENOMEM;
	new_name = dentry_name(new_dentry);
	if (new_name == NULL) {
		__putname(old_name);
		return -ENOMEM;
	}
	if (!flags)
		err = rename_file(old_name, new_name);
	else
		err = rename2_file(old_name, new_name, flags);

	__putname(old_name);
	__putname(new_name);
	return err;
}

static int hostfs_permission(struct mnt_idmap *idmap,
			     struct inode *ino, int desired)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *name;
	int r = 0, w = 0, x = 0, err;

	if (desired & MAY_NOT_BLOCK)
		return -ECHILD;

	if (desired & MAY_READ) r = 1;
	if (desired & MAY_WRITE) w = 1;
	if (desired & MAY_EXEC) x = 1;
	name = inode_name(ino);
	if (name == NULL)
		return -ENOMEM;

	if (S_ISCHR(ino->i_mode) || S_ISBLK(ino->i_mode) ||
	    S_ISFIFO(ino->i_mode) || S_ISSOCK(ino->i_mode))
		err = 0;
	else
		err = access_file(name, r, w, x);
	__putname(name);
	if (!err)
<<<<<<< HEAD
		err = generic_permission(ino, desired);
	return err;
}

int hostfs_setattr(struct dentry *dentry, struct iattr *attr)
{
	struct inode *inode = dentry->d_inode;
=======
		err = generic_permission(&nop_mnt_idmap, ino, desired);
	return err;
}

static int hostfs_setattr(struct mnt_idmap *idmap,
			  struct dentry *dentry, struct iattr *attr)
{
	struct inode *inode = d_inode(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct hostfs_iattr attrs;
	char *name;
	int err;

	int fd = HOSTFS_I(inode)->fd;

<<<<<<< HEAD
	err = inode_change_ok(inode, attr);
=======
	err = setattr_prepare(&nop_mnt_idmap, dentry, attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return err;

	if (append)
		attr->ia_valid &= ~ATTR_SIZE;

	attrs.ia_valid = 0;
	if (attr->ia_valid & ATTR_MODE) {
		attrs.ia_valid |= HOSTFS_ATTR_MODE;
		attrs.ia_mode = attr->ia_mode;
	}
	if (attr->ia_valid & ATTR_UID) {
		attrs.ia_valid |= HOSTFS_ATTR_UID;
<<<<<<< HEAD
		attrs.ia_uid = attr->ia_uid;
	}
	if (attr->ia_valid & ATTR_GID) {
		attrs.ia_valid |= HOSTFS_ATTR_GID;
		attrs.ia_gid = attr->ia_gid;
=======
		attrs.ia_uid = from_kuid(&init_user_ns, attr->ia_uid);
	}
	if (attr->ia_valid & ATTR_GID) {
		attrs.ia_valid |= HOSTFS_ATTR_GID;
		attrs.ia_gid = from_kgid(&init_user_ns, attr->ia_gid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (attr->ia_valid & ATTR_SIZE) {
		attrs.ia_valid |= HOSTFS_ATTR_SIZE;
		attrs.ia_size = attr->ia_size;
	}
	if (attr->ia_valid & ATTR_ATIME) {
		attrs.ia_valid |= HOSTFS_ATTR_ATIME;
<<<<<<< HEAD
		attrs.ia_atime = attr->ia_atime;
	}
	if (attr->ia_valid & ATTR_MTIME) {
		attrs.ia_valid |= HOSTFS_ATTR_MTIME;
		attrs.ia_mtime = attr->ia_mtime;
	}
	if (attr->ia_valid & ATTR_CTIME) {
		attrs.ia_valid |= HOSTFS_ATTR_CTIME;
		attrs.ia_ctime = attr->ia_ctime;
=======
		attrs.ia_atime = (struct hostfs_timespec)
			{ attr->ia_atime.tv_sec, attr->ia_atime.tv_nsec };
	}
	if (attr->ia_valid & ATTR_MTIME) {
		attrs.ia_valid |= HOSTFS_ATTR_MTIME;
		attrs.ia_mtime = (struct hostfs_timespec)
			{ attr->ia_mtime.tv_sec, attr->ia_mtime.tv_nsec };
	}
	if (attr->ia_valid & ATTR_CTIME) {
		attrs.ia_valid |= HOSTFS_ATTR_CTIME;
		attrs.ia_ctime = (struct hostfs_timespec)
			{ attr->ia_ctime.tv_sec, attr->ia_ctime.tv_nsec };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (attr->ia_valid & ATTR_ATIME_SET) {
		attrs.ia_valid |= HOSTFS_ATTR_ATIME_SET;
	}
	if (attr->ia_valid & ATTR_MTIME_SET) {
		attrs.ia_valid |= HOSTFS_ATTR_MTIME_SET;
	}
	name = dentry_name(dentry);
	if (name == NULL)
		return -ENOMEM;
	err = set_attr(name, &attrs, fd);
	__putname(name);
	if (err)
		return err;

	if ((attr->ia_valid & ATTR_SIZE) &&
<<<<<<< HEAD
	    attr->ia_size != i_size_read(inode)) {
		int error;

		error = vmtruncate(inode, attr->ia_size);
		if (err)
			return err;
	}

	setattr_copy(inode, attr);
=======
	    attr->ia_size != i_size_read(inode))
		truncate_setsize(inode, attr->ia_size);

	setattr_copy(&nop_mnt_idmap, inode, attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mark_inode_dirty(inode);
	return 0;
}

static const struct inode_operations hostfs_iops = {
<<<<<<< HEAD
	.create		= hostfs_create,
	.link		= hostfs_link,
	.unlink		= hostfs_unlink,
	.symlink	= hostfs_symlink,
	.mkdir		= hostfs_mkdir,
	.rmdir		= hostfs_rmdir,
	.mknod		= hostfs_mknod,
	.rename		= hostfs_rename,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.permission	= hostfs_permission,
	.setattr	= hostfs_setattr,
};

static const struct inode_operations hostfs_dir_iops = {
	.create		= hostfs_create,
	.lookup		= hostfs_lookup,
	.link		= hostfs_link,
	.unlink		= hostfs_unlink,
	.symlink	= hostfs_symlink,
	.mkdir		= hostfs_mkdir,
	.rmdir		= hostfs_rmdir,
	.mknod		= hostfs_mknod,
<<<<<<< HEAD
	.rename		= hostfs_rename,
=======
	.rename		= hostfs_rename2,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.permission	= hostfs_permission,
	.setattr	= hostfs_setattr,
};

<<<<<<< HEAD
static void *hostfs_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	char *link = __getname();
=======
static const char *hostfs_get_link(struct dentry *dentry,
				   struct inode *inode,
				   struct delayed_call *done)
{
	char *link;
	if (!dentry)
		return ERR_PTR(-ECHILD);
	link = kmalloc(PATH_MAX, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (link) {
		char *path = dentry_name(dentry);
		int err = -ENOMEM;
		if (path) {
			err = hostfs_do_readlink(path, link, PATH_MAX);
			if (err == PATH_MAX)
				err = -E2BIG;
			__putname(path);
		}
		if (err < 0) {
<<<<<<< HEAD
			__putname(link);
			link = ERR_PTR(err);
		}
	} else {
		link = ERR_PTR(-ENOMEM);
	}

	nd_set_link(nd, link);
	return NULL;
}

static void hostfs_put_link(struct dentry *dentry, struct nameidata *nd, void *cookie)
{
	char *s = nd_get_link(nd);
	if (!IS_ERR(s))
		__putname(s);
}

static const struct inode_operations hostfs_link_iops = {
	.readlink	= generic_readlink,
	.follow_link	= hostfs_follow_link,
	.put_link	= hostfs_put_link,
=======
			kfree(link);
			return ERR_PTR(err);
		}
	} else {
		return ERR_PTR(-ENOMEM);
	}

	set_delayed_call(done, kfree_link, link);
	return link;
}

static const struct inode_operations hostfs_link_iops = {
	.get_link	= hostfs_get_link,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int hostfs_fill_sb_common(struct super_block *sb, void *d, int silent)
{
	struct inode *root_inode;
	char *host_root_path, *req_root = d;
	int err;

	sb->s_blocksize = 1024;
	sb->s_blocksize_bits = 10;
	sb->s_magic = HOSTFS_SUPER_MAGIC;
	sb->s_op = &hostfs_sbops;
<<<<<<< HEAD
	sb->s_d_op = &hostfs_dentry_ops;
	sb->s_maxbytes = MAX_LFS_FILESIZE;

	/* NULL is printed as <NULL> by sprintf: avoid that. */
	if (req_root == NULL)
		req_root = "";

	err = -ENOMEM;
	sb->s_fs_info = host_root_path =
		kmalloc(strlen(root_ino) + strlen(req_root) + 2, GFP_KERNEL);
	if (host_root_path == NULL)
		goto out;

	sprintf(host_root_path, "%s/%s", root_ino, req_root);

	root_inode = new_inode(sb);
	if (!root_inode)
		goto out;

	err = read_name(root_inode, host_root_path);
	if (err)
		goto out_put;

	if (S_ISLNK(root_inode->i_mode)) {
		char *name = follow_link(host_root_path);
		if (IS_ERR(name))
			err = PTR_ERR(name);
		else
			err = read_name(root_inode, name);
		kfree(name);
		if (err)
			goto out_put;
	}

	err = -ENOMEM;
	sb->s_root = d_make_root(root_inode);
	if (sb->s_root == NULL)
		goto out;

	return 0;

out_put:
	iput(root_inode);
out:
	return err;
=======
	sb->s_d_op = &simple_dentry_operations;
	sb->s_maxbytes = MAX_LFS_FILESIZE;
	err = super_setup_bdi(sb);
	if (err)
		return err;

	/* NULL is printed as '(null)' by printf(): avoid that. */
	if (req_root == NULL)
		req_root = "";

	sb->s_fs_info = host_root_path =
		kasprintf(GFP_KERNEL, "%s/%s", root_ino, req_root);
	if (host_root_path == NULL)
		return -ENOMEM;

	root_inode = hostfs_iget(sb, host_root_path);
	if (IS_ERR(root_inode))
		return PTR_ERR(root_inode);

	if (S_ISLNK(root_inode->i_mode)) {
		char *name;

		iput(root_inode);
		name = follow_link(host_root_path);
		if (IS_ERR(name))
			return PTR_ERR(name);

		root_inode = hostfs_iget(sb, name);
		kfree(name);
		if (IS_ERR(root_inode))
			return PTR_ERR(root_inode);
	}

	sb->s_root = d_make_root(root_inode);
	if (sb->s_root == NULL)
		return -ENOMEM;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct dentry *hostfs_read_sb(struct file_system_type *type,
			  int flags, const char *dev_name,
			  void *data)
{
	return mount_nodev(type, flags, data, hostfs_fill_sb_common);
}

static void hostfs_kill_sb(struct super_block *s)
{
	kill_anon_super(s);
	kfree(s->s_fs_info);
}

static struct file_system_type hostfs_type = {
	.owner 		= THIS_MODULE,
	.name 		= "hostfs",
	.mount	 	= hostfs_read_sb,
	.kill_sb	= hostfs_kill_sb,
	.fs_flags 	= 0,
};
<<<<<<< HEAD

static int __init init_hostfs(void)
{
=======
MODULE_ALIAS_FS("hostfs");

static int __init init_hostfs(void)
{
	hostfs_inode_cache = KMEM_CACHE(hostfs_inode_info, 0);
	if (!hostfs_inode_cache)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return register_filesystem(&hostfs_type);
}

static void __exit exit_hostfs(void)
{
	unregister_filesystem(&hostfs_type);
<<<<<<< HEAD
=======
	kmem_cache_destroy(hostfs_inode_cache);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(init_hostfs)
module_exit(exit_hostfs)
MODULE_LICENSE("GPL");
