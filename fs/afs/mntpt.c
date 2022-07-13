<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* mountpoint management
 *
 * Copyright (C) 2002 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/gfp.h>
<<<<<<< HEAD
=======
#include <linux/fs_context.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "internal.h"


static struct dentry *afs_mntpt_lookup(struct inode *dir,
				       struct dentry *dentry,
				       unsigned int flags);
static int afs_mntpt_open(struct inode *inode, struct file *file);
static void afs_mntpt_expiry_timed_out(struct work_struct *work);

const struct file_operations afs_mntpt_file_operations = {
	.open		= afs_mntpt_open,
	.llseek		= noop_llseek,
};

const struct inode_operations afs_mntpt_inode_operations = {
	.lookup		= afs_mntpt_lookup,
	.readlink	= page_readlink,
	.getattr	= afs_getattr,
};

const struct inode_operations afs_autocell_inode_operations = {
	.getattr	= afs_getattr,
};

static LIST_HEAD(afs_vfsmounts);
static DECLARE_DELAYED_WORK(afs_mntpt_expiry_timer, afs_mntpt_expiry_timed_out);

static unsigned long afs_mntpt_expiry_timeout = 10 * 60;

<<<<<<< HEAD
/*
 * check a symbolic link to see whether it actually encodes a mountpoint
 * - sets the AFS_VNODE_MOUNTPOINT flag on the vnode appropriately
 */
int afs_mntpt_check_symlink(struct afs_vnode *vnode, struct key *key)
{
	struct page *page;
	size_t size;
	char *buf;
	int ret;

	_enter("{%x:%u,%u}",
	       vnode->fid.vid, vnode->fid.vnode, vnode->fid.unique);

	/* read the contents of the symlink into the pagecache */
	page = read_cache_page(AFS_VNODE_TO_I(vnode)->i_mapping, 0,
			       afs_page_filler, key);
	if (IS_ERR(page)) {
		ret = PTR_ERR(page);
		goto out;
	}

	ret = -EIO;
	if (PageError(page))
		goto out_free;

	buf = kmap(page);

	/* examine the symlink's contents */
	size = vnode->status.size;
	_debug("symlink to %*.*s", (int) size, (int) size, buf);

	if (size > 2 &&
	    (buf[0] == '%' || buf[0] == '#') &&
	    buf[size - 1] == '.'
	    ) {
		_debug("symlink is a mountpoint");
		spin_lock(&vnode->lock);
		set_bit(AFS_VNODE_MOUNTPOINT, &vnode->flags);
		vnode->vfs_inode.i_flags |= S_AUTOMOUNT;
		spin_unlock(&vnode->lock);
	}

	ret = 0;

	kunmap(page);
out_free:
	page_cache_release(page);
out:
	_leave(" = %d", ret);
	return ret;
}
=======
static const char afs_root_volume[] = "root.cell";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * no valid lookup procedure on this sort of dir
 */
static struct dentry *afs_mntpt_lookup(struct inode *dir,
				       struct dentry *dentry,
				       unsigned int flags)
{
<<<<<<< HEAD
	_enter("%p,%p{%p{%s},%s}",
	       dir,
	       dentry,
	       dentry->d_parent,
	       dentry->d_parent ?
	       dentry->d_parent->d_name.name : (const unsigned char *) "",
	       dentry->d_name.name);

=======
	_enter("%p,%p{%pd2}", dir, dentry, dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ERR_PTR(-EREMOTE);
}

/*
 * no valid open procedure on this sort of dir
 */
static int afs_mntpt_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	_enter("%p,%p{%p{%s},%s}",
	       inode, file,
	       file->f_path.dentry->d_parent,
	       file->f_path.dentry->d_parent ?
	       file->f_path.dentry->d_parent->d_name.name :
	       (const unsigned char *) "",
	       file->f_path.dentry->d_name.name);

=======
	_enter("%p,%p{%pD2}", inode, file, file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -EREMOTE;
}

/*
<<<<<<< HEAD
=======
 * Set the parameters for the proposed superblock.
 */
static int afs_mntpt_set_params(struct fs_context *fc, struct dentry *mntpt)
{
	struct afs_fs_context *ctx = fc->fs_private;
	struct afs_super_info *src_as = AFS_FS_S(mntpt->d_sb);
	struct afs_vnode *vnode = AFS_FS_I(d_inode(mntpt));
	struct afs_cell *cell;
	const char *p;
	int ret;

	if (fc->net_ns != src_as->net_ns) {
		put_net(fc->net_ns);
		fc->net_ns = get_net(src_as->net_ns);
	}

	if (src_as->volume && src_as->volume->type == AFSVL_RWVOL) {
		ctx->type = AFSVL_RWVOL;
		ctx->force = true;
	}
	if (ctx->cell) {
		afs_unuse_cell(ctx->net, ctx->cell, afs_cell_trace_unuse_mntpt);
		ctx->cell = NULL;
	}
	if (test_bit(AFS_VNODE_PSEUDODIR, &vnode->flags)) {
		/* if the directory is a pseudo directory, use the d_name */
		unsigned size = mntpt->d_name.len;

		if (size < 2)
			return -ENOENT;

		p = mntpt->d_name.name;
		if (mntpt->d_name.name[0] == '.') {
			size--;
			p++;
			ctx->type = AFSVL_RWVOL;
			ctx->force = true;
		}
		if (size > AFS_MAXCELLNAME)
			return -ENAMETOOLONG;

		cell = afs_lookup_cell(ctx->net, p, size, NULL, false);
		if (IS_ERR(cell)) {
			pr_err("kAFS: unable to lookup cell '%pd'\n", mntpt);
			return PTR_ERR(cell);
		}
		ctx->cell = cell;

		ctx->volname = afs_root_volume;
		ctx->volnamesz = sizeof(afs_root_volume) - 1;
	} else {
		/* read the contents of the AFS special symlink */
		struct page *page;
		loff_t size = i_size_read(d_inode(mntpt));
		char *buf;

		if (src_as->cell)
			ctx->cell = afs_use_cell(src_as->cell, afs_cell_trace_use_mntpt);

		if (size < 2 || size > PAGE_SIZE - 1)
			return -EINVAL;

		page = read_mapping_page(d_inode(mntpt)->i_mapping, 0, NULL);
		if (IS_ERR(page))
			return PTR_ERR(page);

		buf = kmap(page);
		ret = -EINVAL;
		if (buf[size - 1] == '.')
			ret = vfs_parse_fs_string(fc, "source", buf, size - 1);
		kunmap(page);
		put_page(page);
		if (ret < 0)
			return ret;
	}

	return 0;
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * create a vfsmount to be automounted
 */
static struct vfsmount *afs_mntpt_do_automount(struct dentry *mntpt)
{
<<<<<<< HEAD
	struct afs_super_info *super;
	struct vfsmount *mnt;
	struct afs_vnode *vnode;
	struct page *page;
	char *devname, *options;
	bool rwpath = false;
	int ret;

	_enter("{%s}", mntpt->d_name.name);

	BUG_ON(!mntpt->d_inode);

	ret = -ENOMEM;
	devname = (char *) get_zeroed_page(GFP_KERNEL);
	if (!devname)
		goto error_no_devname;

	options = (char *) get_zeroed_page(GFP_KERNEL);
	if (!options)
		goto error_no_options;

	vnode = AFS_FS_I(mntpt->d_inode);
	if (test_bit(AFS_VNODE_PSEUDODIR, &vnode->flags)) {
		/* if the directory is a pseudo directory, use the d_name */
		static const char afs_root_cell[] = ":root.cell.";
		unsigned size = mntpt->d_name.len;

		ret = -ENOENT;
		if (size < 2 || size > AFS_MAXCELLNAME)
			goto error_no_page;

		if (mntpt->d_name.name[0] == '.') {
			devname[0] = '#';
			memcpy(devname + 1, mntpt->d_name.name, size - 1);
			memcpy(devname + size, afs_root_cell,
			       sizeof(afs_root_cell));
			rwpath = true;
		} else {
			devname[0] = '%';
			memcpy(devname + 1, mntpt->d_name.name, size);
			memcpy(devname + size + 1, afs_root_cell,
			       sizeof(afs_root_cell));
		}
	} else {
		/* read the contents of the AFS special symlink */
		loff_t size = i_size_read(mntpt->d_inode);
		char *buf;

		ret = -EINVAL;
		if (size > PAGE_SIZE - 1)
			goto error_no_page;

		page = read_mapping_page(mntpt->d_inode->i_mapping, 0, NULL);
		if (IS_ERR(page)) {
			ret = PTR_ERR(page);
			goto error_no_page;
		}

		ret = -EIO;
		if (PageError(page))
			goto error;

		buf = kmap_atomic(page);
		memcpy(devname, buf, size);
		kunmap_atomic(buf);
		page_cache_release(page);
		page = NULL;
	}

	/* work out what options we want */
	super = AFS_FS_S(mntpt->d_sb);
	memcpy(options, "cell=", 5);
	strcpy(options + 5, super->volume->cell->name);
	if (super->volume->type == AFSVL_RWVOL || rwpath)
		strcat(options, ",rwpath");

	/* try and do the mount */
	_debug("--- attempting mount %s -o %s ---", devname, options);
	mnt = vfs_kern_mount(&afs_fs_type, 0, devname, options);
	_debug("--- mount result %p ---", mnt);

	free_page((unsigned long) devname);
	free_page((unsigned long) options);
	_leave(" = %p", mnt);
	return mnt;

error:
	page_cache_release(page);
error_no_page:
	free_page((unsigned long) options);
error_no_options:
	free_page((unsigned long) devname);
error_no_devname:
	_leave(" = %d", ret);
	return ERR_PTR(ret);
=======
	struct fs_context *fc;
	struct vfsmount *mnt;
	int ret;

	BUG_ON(!d_inode(mntpt));

	fc = fs_context_for_submount(&afs_fs_type, mntpt);
	if (IS_ERR(fc))
		return ERR_CAST(fc);

	ret = afs_mntpt_set_params(fc, mntpt);
	if (!ret)
		mnt = fc_mount(fc);
	else
		mnt = ERR_PTR(ret);

	put_fs_context(fc);
	return mnt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * handle an automount point
 */
struct vfsmount *afs_d_automount(struct path *path)
{
	struct vfsmount *newmnt;

<<<<<<< HEAD
	_enter("{%s}", path->dentry->d_name.name);
=======
	_enter("{%pd}", path->dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	newmnt = afs_mntpt_do_automount(path->dentry);
	if (IS_ERR(newmnt))
		return newmnt;

	mntget(newmnt); /* prevent immediate expiration */
	mnt_set_expiry(newmnt, &afs_vfsmounts);
	queue_delayed_work(afs_wq, &afs_mntpt_expiry_timer,
			   afs_mntpt_expiry_timeout * HZ);
	_leave(" = %p", newmnt);
	return newmnt;
}

/*
 * handle mountpoint expiry timer going off
 */
static void afs_mntpt_expiry_timed_out(struct work_struct *work)
{
	_enter("");

	if (!list_empty(&afs_vfsmounts)) {
		mark_mounts_for_expiry(&afs_vfsmounts);
		queue_delayed_work(afs_wq, &afs_mntpt_expiry_timer,
				   afs_mntpt_expiry_timeout * HZ);
	}

	_leave("");
}

/*
 * kill the AFS mountpoint timer if it's still running
 */
void afs_mntpt_kill_timer(void)
{
	_enter("");

	ASSERT(list_empty(&afs_vfsmounts));
	cancel_delayed_work_sync(&afs_mntpt_expiry_timer);
}
