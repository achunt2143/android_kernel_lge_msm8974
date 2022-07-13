<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* * This file is part of UBIFS.
 *
 * Copyright (C) 2006-2008 Nokia Corporation.
 * Copyright (C) 2006, 2007 University of Szeged, Hungary
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors: Artem Bityutskiy (Битюцкий Артём)
 *          Adrian Hunter
 *          Zoltan Sogor
 */

/*
 * This file implements directory operations.
 *
 * All FS operations in this file allocate budget before writing anything to the
 * media. If they fail to allocate it, the error is returned. The only
 * exceptions are 'ubifs_unlink()' and 'ubifs_rmdir()' which keep working even
 * if they unable to allocate the budget, because deletion %-ENOSPC failure is
 * not what users are usually ready to get. UBIFS budgeting subsystem has some
 * space reserved for these purposes.
 *
 * All operations in this file write all inodes which they change straight
 * away, instead of marking them dirty. For example, 'ubifs_link()' changes
 * @i_size of the parent inode and writes the parent inode together with the
 * target inode. This was done to simplify file-system recovery which would
 * otherwise be very difficult to do. The only exception is rename which marks
 * the re-named inode dirty (because its @i_ctime is updated) but does not
 * write it, but just marks it as dirty.
 */

#include "ubifs.h"

/**
 * inherit_flags - inherit flags of the parent inode.
 * @dir: parent inode
 * @mode: new inode mode flags
 *
 * This is a helper function for 'ubifs_new_inode()' which inherits flag of the
 * parent directory inode @dir. UBIFS inodes inherit the following flags:
 * o %UBIFS_COMPR_FL, which is useful to switch compression on/of on
 *   sub-directory basis;
 * o %UBIFS_SYNC_FL - useful for the same reasons;
 * o %UBIFS_DIRSYNC_FL - similar, but relevant only to directories.
 *
 * This function returns the inherited flags.
 */
static int inherit_flags(const struct inode *dir, umode_t mode)
{
	int flags;
	const struct ubifs_inode *ui = ubifs_inode(dir);

	if (!S_ISDIR(dir->i_mode))
		/*
		 * The parent is not a directory, which means that an extended
		 * attribute inode is being created. No flags.
		 */
		return 0;

	flags = ui->flags & (UBIFS_COMPR_FL | UBIFS_SYNC_FL | UBIFS_DIRSYNC_FL);
	if (!S_ISDIR(mode))
		/* The "DIRSYNC" flag only applies to directories */
		flags &= ~UBIFS_DIRSYNC_FL;
	return flags;
}

/**
 * ubifs_new_inode - allocate new UBIFS inode object.
 * @c: UBIFS file-system description object
 * @dir: parent directory inode
 * @mode: inode mode flags
<<<<<<< HEAD
=======
 * @is_xattr: whether the inode is xattr inode
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function finds an unused inode number, allocates new inode and
 * initializes it. Returns new inode in case of success and an error code in
 * case of failure.
 */
<<<<<<< HEAD
struct inode *ubifs_new_inode(struct ubifs_info *c, const struct inode *dir,
			      umode_t mode)
{
	struct inode *inode;
	struct ubifs_inode *ui;
=======
struct inode *ubifs_new_inode(struct ubifs_info *c, struct inode *dir,
			      umode_t mode, bool is_xattr)
{
	int err;
	struct inode *inode;
	struct ubifs_inode *ui;
	bool encrypted = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode = new_inode(c->vfs_sb);
	ui = ubifs_inode(inode);
	if (!inode)
		return ERR_PTR(-ENOMEM);

	/*
	 * Set 'S_NOCMTIME' to prevent VFS form updating [mc]time of inodes and
	 * marking them dirty in file write path (see 'file_update_time()').
	 * UBIFS has to fully control "clean <-> dirty" transitions of inodes
	 * to make budgeting work.
	 */
	inode->i_flags |= S_NOCMTIME;

<<<<<<< HEAD
	inode_init_owner(inode, dir, mode);
	inode->i_mtime = inode->i_atime = inode->i_ctime =
			 ubifs_current_time(inode);
	inode->i_mapping->nrpages = 0;
	/* Disable readahead */
	inode->i_mapping->backing_dev_info = &c->bdi;
=======
	inode_init_owner(&nop_mnt_idmap, inode, dir, mode);
	simple_inode_init_ts(inode);
	inode->i_mapping->nrpages = 0;

	if (!is_xattr) {
		err = fscrypt_prepare_new_inode(dir, inode, &encrypted);
		if (err) {
			ubifs_err(c, "fscrypt_prepare_new_inode failed: %i", err);
			goto out_iput;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (mode & S_IFMT) {
	case S_IFREG:
		inode->i_mapping->a_ops = &ubifs_file_address_operations;
		inode->i_op = &ubifs_file_inode_operations;
		inode->i_fop = &ubifs_file_operations;
		break;
	case S_IFDIR:
		inode->i_op  = &ubifs_dir_inode_operations;
		inode->i_fop = &ubifs_dir_operations;
		inode->i_size = ui->ui_size = UBIFS_INO_NODE_SZ;
		break;
	case S_IFLNK:
		inode->i_op = &ubifs_symlink_inode_operations;
		break;
	case S_IFSOCK:
	case S_IFIFO:
	case S_IFBLK:
	case S_IFCHR:
		inode->i_op  = &ubifs_file_inode_operations;
		break;
	default:
		BUG();
	}

	ui->flags = inherit_flags(dir, mode);
	ubifs_set_inode_flags(inode);
	if (S_ISREG(mode))
		ui->compr_type = c->default_compr;
	else
		ui->compr_type = UBIFS_COMPR_NONE;
	ui->synced_i_size = 0;

	spin_lock(&c->cnt_lock);
	/* Inode number overflow is currently not supported */
	if (c->highest_inum >= INUM_WARN_WATERMARK) {
		if (c->highest_inum >= INUM_WATERMARK) {
			spin_unlock(&c->cnt_lock);
<<<<<<< HEAD
			ubifs_err("out of inode numbers");
			make_bad_inode(inode);
			iput(inode);
			return ERR_PTR(-EINVAL);
		}
		ubifs_warn("running out of inode numbers (current %lu, max %d)",
=======
			ubifs_err(c, "out of inode numbers");
			err = -EINVAL;
			goto out_iput;
		}
		ubifs_warn(c, "running out of inode numbers (current %lu, max %u)",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   (unsigned long)c->highest_inum, INUM_WATERMARK);
	}

	inode->i_ino = ++c->highest_inum;
	/*
	 * The creation sequence number remains with this inode for its
	 * lifetime. All nodes for this inode have a greater sequence number,
	 * and so it is possible to distinguish obsolete nodes belonging to a
	 * previous incarnation of the same inode number - for example, for the
	 * purpose of rebuilding the index.
	 */
	ui->creat_sqnum = ++c->max_sqnum;
	spin_unlock(&c->cnt_lock);
<<<<<<< HEAD
	return inode;
}

#ifdef CONFIG_UBIFS_FS_DEBUG

static int dbg_check_name(const struct ubifs_info *c,
			  const struct ubifs_dent_node *dent,
			  const struct qstr *nm)
{
	if (!dbg_is_chk_gen(c))
		return 0;
	if (le16_to_cpu(dent->nlen) != nm->len)
		return -EINVAL;
	if (memcmp(dent->name, nm->name, nm->len))
=======

	if (encrypted) {
		err = fscrypt_set_context(inode, NULL);
		if (err) {
			ubifs_err(c, "fscrypt_set_context failed: %i", err);
			goto out_iput;
		}
	}

	return inode;

out_iput:
	make_bad_inode(inode);
	iput(inode);
	return ERR_PTR(err);
}

static int dbg_check_name(const struct ubifs_info *c,
			  const struct ubifs_dent_node *dent,
			  const struct fscrypt_name *nm)
{
	if (!dbg_is_chk_gen(c))
		return 0;
	if (le16_to_cpu(dent->nlen) != fname_len(nm))
		return -EINVAL;
	if (memcmp(dent->name, fname_name(nm), fname_len(nm)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	return 0;
}

<<<<<<< HEAD
#else

#define dbg_check_name(c, dent, nm) 0

#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct dentry *ubifs_lookup(struct inode *dir, struct dentry *dentry,
				   unsigned int flags)
{
	int err;
	union ubifs_key key;
	struct inode *inode = NULL;
<<<<<<< HEAD
	struct ubifs_dent_node *dent;
	struct ubifs_info *c = dir->i_sb->s_fs_info;

	dbg_gen("'%.*s' in dir ino %lu",
		dentry->d_name.len, dentry->d_name.name, dir->i_ino);

	if (dentry->d_name.len > UBIFS_MAX_NLEN)
		return ERR_PTR(-ENAMETOOLONG);

	dent = kmalloc(UBIFS_MAX_DENT_NODE_SZ, GFP_NOFS);
	if (!dent)
		return ERR_PTR(-ENOMEM);

	dent_key_init(c, &key, dir->i_ino, &dentry->d_name);

	err = ubifs_tnc_lookup_nm(c, &key, dent, &dentry->d_name);
	if (err) {
		if (err == -ENOENT) {
			dbg_gen("not found");
			goto done;
		}
		goto out;
	}

	if (dbg_check_name(c, dent, &dentry->d_name)) {
		err = -EINVAL;
		goto out;
=======
	struct ubifs_dent_node *dent = NULL;
	struct ubifs_info *c = dir->i_sb->s_fs_info;
	struct fscrypt_name nm;

	dbg_gen("'%pd' in dir ino %lu", dentry, dir->i_ino);

	err = fscrypt_prepare_lookup(dir, dentry, &nm);
	if (err == -ENOENT)
		return d_splice_alias(NULL, dentry);
	if (err)
		return ERR_PTR(err);

	if (fname_len(&nm) > UBIFS_MAX_NLEN) {
		inode = ERR_PTR(-ENAMETOOLONG);
		goto done;
	}

	dent = kmalloc(UBIFS_MAX_DENT_NODE_SZ, GFP_NOFS);
	if (!dent) {
		inode = ERR_PTR(-ENOMEM);
		goto done;
	}

	if (fname_name(&nm) == NULL) {
		if (nm.hash & ~UBIFS_S_KEY_HASH_MASK)
			goto done; /* ENOENT */
		dent_key_init_hash(c, &key, dir->i_ino, nm.hash);
		err = ubifs_tnc_lookup_dh(c, &key, dent, nm.minor_hash);
	} else {
		dent_key_init(c, &key, dir->i_ino, &nm);
		err = ubifs_tnc_lookup_nm(c, &key, dent, &nm);
	}

	if (err) {
		if (err == -ENOENT)
			dbg_gen("not found");
		else
			inode = ERR_PTR(err);
		goto done;
	}

	if (dbg_check_name(c, dent, &nm)) {
		inode = ERR_PTR(-EINVAL);
		goto done;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	inode = ubifs_iget(dir->i_sb, le64_to_cpu(dent->inum));
	if (IS_ERR(inode)) {
		/*
		 * This should not happen. Probably the file-system needs
		 * checking.
		 */
		err = PTR_ERR(inode);
<<<<<<< HEAD
		ubifs_err("dead directory entry '%.*s', error %d",
			  dentry->d_name.len, dentry->d_name.name, err);
		ubifs_ro_mode(c, err);
		goto out;
=======
		ubifs_err(c, "dead directory entry '%pd', error %d",
			  dentry, err);
		ubifs_ro_mode(c, err);
		goto done;
	}

	if (IS_ENCRYPTED(dir) &&
	    (S_ISDIR(inode->i_mode) || S_ISLNK(inode->i_mode)) &&
	    !fscrypt_has_permitted_context(dir, inode)) {
		ubifs_warn(c, "Inconsistent encryption contexts: %lu/%lu",
			   dir->i_ino, inode->i_ino);
		iput(inode);
		inode = ERR_PTR(-EPERM);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

done:
	kfree(dent);
<<<<<<< HEAD
	/*
	 * Note, d_splice_alias() would be required instead if we supported
	 * NFS.
	 */
	d_add(dentry, inode);
	return NULL;

out:
	kfree(dent);
	return ERR_PTR(err);
}

static int ubifs_create(struct inode *dir, struct dentry *dentry, umode_t mode,
			bool excl)
{
	struct inode *inode;
	struct ubifs_info *c = dir->i_sb->s_fs_info;
	int err, sz_change = CALC_DENT_SIZE(dentry->d_name.len);
	struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1,
					.dirtied_ino = 1 };
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
=======
	fscrypt_free_filename(&nm);
	return d_splice_alias(inode, dentry);
}

static int ubifs_prepare_create(struct inode *dir, struct dentry *dentry,
				struct fscrypt_name *nm)
{
	if (fscrypt_is_nokey_name(dentry))
		return -ENOKEY;

	return fscrypt_setup_filename(dir, &dentry->d_name, 0, nm);
}

static int ubifs_create(struct mnt_idmap *idmap, struct inode *dir,
			struct dentry *dentry, umode_t mode, bool excl)
{
	struct inode *inode;
	struct ubifs_info *c = dir->i_sb->s_fs_info;
	struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1,
					.dirtied_ino = 1 };
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	struct fscrypt_name nm;
	int err, sz_change;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Budget request settings: new inode, new direntry, changing the
	 * parent directory inode.
	 */

<<<<<<< HEAD
	dbg_gen("dent '%.*s', mode %#hx in dir ino %lu",
		dentry->d_name.len, dentry->d_name.name, mode, dir->i_ino);
=======
	dbg_gen("dent '%pd', mode %#hx in dir ino %lu",
		dentry, mode, dir->i_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = ubifs_budget_space(c, &req);
	if (err)
		return err;

<<<<<<< HEAD
	inode = ubifs_new_inode(c, dir, mode);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_budg;
	}

	mutex_lock(&dir_ui->ui_mutex);
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
	dir->i_mtime = dir->i_ctime = inode->i_ctime;
	err = ubifs_jnl_update(c, dir, &dentry->d_name, inode, 0, 0);
=======
	err = ubifs_prepare_create(dir, dentry, &nm);
	if (err)
		goto out_budg;

	sz_change = CALC_DENT_SIZE(fname_len(&nm));

	inode = ubifs_new_inode(c, dir, mode, false);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_fname;
	}

	err = ubifs_init_security(dir, inode, &dentry->d_name);
	if (err)
		goto out_inode;

	mutex_lock(&dir_ui->ui_mutex);
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
	inode_set_mtime_to_ts(dir,
			      inode_set_ctime_to_ts(dir, inode_get_ctime(inode)));
	err = ubifs_jnl_update(c, dir, &nm, inode, 0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out_cancel;
	mutex_unlock(&dir_ui->ui_mutex);

	ubifs_release_budget(c, &req);
<<<<<<< HEAD
=======
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	insert_inode_hash(inode);
	d_instantiate(dentry, inode);
	return 0;

out_cancel:
	dir->i_size -= sz_change;
	dir_ui->ui_size = dir->i_size;
	mutex_unlock(&dir_ui->ui_mutex);
<<<<<<< HEAD
	make_bad_inode(inode);
	iput(inode);
out_budg:
	ubifs_release_budget(c, &req);
	ubifs_err("cannot create regular file, error %d", err);
=======
out_inode:
	make_bad_inode(inode);
	iput(inode);
out_fname:
	fscrypt_free_filename(&nm);
out_budg:
	ubifs_release_budget(c, &req);
	ubifs_err(c, "cannot create regular file, error %d", err);
	return err;
}

static struct inode *create_whiteout(struct inode *dir, struct dentry *dentry)
{
	int err;
	umode_t mode = S_IFCHR | WHITEOUT_MODE;
	struct inode *inode;
	struct ubifs_info *c = dir->i_sb->s_fs_info;

	/*
	 * Create an inode('nlink = 1') for whiteout without updating journal,
	 * let ubifs_jnl_rename() store it on flash to complete rename whiteout
	 * atomically.
	 */

	dbg_gen("dent '%pd', mode %#hx in dir ino %lu",
		dentry, mode, dir->i_ino);

	inode = ubifs_new_inode(c, dir, mode, false);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_free;
	}

	init_special_inode(inode, inode->i_mode, WHITEOUT_DEV);
	ubifs_assert(c, inode->i_op == &ubifs_file_inode_operations);

	err = ubifs_init_security(dir, inode, &dentry->d_name);
	if (err)
		goto out_inode;

	/* The dir size is updated by do_rename. */
	insert_inode_hash(inode);

	return inode;

out_inode:
	make_bad_inode(inode);
	iput(inode);
out_free:
	ubifs_err(c, "cannot create whiteout file, error %d", err);
	return ERR_PTR(err);
}

/**
 * lock_2_inodes - a wrapper for locking two UBIFS inodes.
 * @inode1: first inode
 * @inode2: second inode
 *
 * We do not implement any tricks to guarantee strict lock ordering, because
 * VFS has already done it for us on the @i_mutex. So this is just a simple
 * wrapper function.
 */
static void lock_2_inodes(struct inode *inode1, struct inode *inode2)
{
	mutex_lock_nested(&ubifs_inode(inode1)->ui_mutex, WB_MUTEX_1);
	mutex_lock_nested(&ubifs_inode(inode2)->ui_mutex, WB_MUTEX_2);
}

/**
 * unlock_2_inodes - a wrapper for unlocking two UBIFS inodes.
 * @inode1: first inode
 * @inode2: second inode
 */
static void unlock_2_inodes(struct inode *inode1, struct inode *inode2)
{
	mutex_unlock(&ubifs_inode(inode2)->ui_mutex);
	mutex_unlock(&ubifs_inode(inode1)->ui_mutex);
}

static int ubifs_tmpfile(struct mnt_idmap *idmap, struct inode *dir,
			 struct file *file, umode_t mode)
{
	struct dentry *dentry = file->f_path.dentry;
	struct inode *inode;
	struct ubifs_info *c = dir->i_sb->s_fs_info;
	struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1,
					.dirtied_ino = 1};
	struct ubifs_budget_req ino_req = { .dirtied_ino = 1 };
	struct ubifs_inode *ui;
	int err, instantiated = 0;
	struct fscrypt_name nm;

	/*
	 * Budget request settings: new inode, new direntry, changing the
	 * parent directory inode.
	 * Allocate budget separately for new dirtied inode, the budget will
	 * be released via writeback.
	 */

	dbg_gen("dent '%pd', mode %#hx in dir ino %lu",
		dentry, mode, dir->i_ino);

	err = fscrypt_setup_filename(dir, &dentry->d_name, 0, &nm);
	if (err)
		return err;

	err = ubifs_budget_space(c, &req);
	if (err) {
		fscrypt_free_filename(&nm);
		return err;
	}

	err = ubifs_budget_space(c, &ino_req);
	if (err) {
		ubifs_release_budget(c, &req);
		fscrypt_free_filename(&nm);
		return err;
	}

	inode = ubifs_new_inode(c, dir, mode, false);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_budg;
	}
	ui = ubifs_inode(inode);

	err = ubifs_init_security(dir, inode, &dentry->d_name);
	if (err)
		goto out_inode;

	mutex_lock(&ui->ui_mutex);
	insert_inode_hash(inode);
	d_tmpfile(file, inode);
	ubifs_assert(c, ui->dirty);

	instantiated = 1;
	mutex_unlock(&ui->ui_mutex);

	lock_2_inodes(dir, inode);
	err = ubifs_jnl_update(c, dir, &nm, inode, 1, 0);
	if (err)
		goto out_cancel;
	unlock_2_inodes(dir, inode);

	ubifs_release_budget(c, &req);
	fscrypt_free_filename(&nm);

	return finish_open_simple(file, 0);

out_cancel:
	unlock_2_inodes(dir, inode);
out_inode:
	make_bad_inode(inode);
	if (!instantiated)
		iput(inode);
out_budg:
	ubifs_release_budget(c, &req);
	if (!instantiated)
		ubifs_release_budget(c, &ino_req);
	fscrypt_free_filename(&nm);
	ubifs_err(c, "cannot create temporary file, error %d", err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/**
 * vfs_dent_type - get VFS directory entry type.
 * @type: UBIFS directory entry type
 *
 * This function converts UBIFS directory entry type into VFS directory entry
 * type.
 */
static unsigned int vfs_dent_type(uint8_t type)
{
	switch (type) {
	case UBIFS_ITYPE_REG:
		return DT_REG;
	case UBIFS_ITYPE_DIR:
		return DT_DIR;
	case UBIFS_ITYPE_LNK:
		return DT_LNK;
	case UBIFS_ITYPE_BLK:
		return DT_BLK;
	case UBIFS_ITYPE_CHR:
		return DT_CHR;
	case UBIFS_ITYPE_FIFO:
		return DT_FIFO;
	case UBIFS_ITYPE_SOCK:
		return DT_SOCK;
	default:
		BUG();
	}
	return 0;
}

/*
 * The classical Unix view for directory is that it is a linear array of
 * (name, inode number) entries. Linux/VFS assumes this model as well.
 * Particularly, 'readdir()' call wants us to return a directory entry offset
 * which later may be used to continue 'readdir()'ing the directory or to
 * 'seek()' to that specific direntry. Obviously UBIFS does not really fit this
 * model because directory entries are identified by keys, which may collide.
 *
 * UBIFS uses directory entry hash value for directory offsets, so
 * 'seekdir()'/'telldir()' may not always work because of possible key
 * collisions. But UBIFS guarantees that consecutive 'readdir()' calls work
 * properly by means of saving full directory entry name in the private field
 * of the file description object.
 *
 * This means that UBIFS cannot support NFS which requires full
 * 'seekdir()'/'telldir()' support.
 */
<<<<<<< HEAD
static int ubifs_readdir(struct file *file, void *dirent, filldir_t filldir)
{
	int err, over = 0;
	loff_t pos = file->f_pos;
	struct qstr nm;
	union ubifs_key key;
	struct ubifs_dent_node *dent;
	struct inode *dir = file->f_path.dentry->d_inode;
	struct ubifs_info *c = dir->i_sb->s_fs_info;

	dbg_gen("dir ino %lu, f_pos %#llx", dir->i_ino, pos);

	if (pos > UBIFS_S_KEY_HASH_MASK || pos == 2)
=======
static int ubifs_readdir(struct file *file, struct dir_context *ctx)
{
	int fstr_real_len = 0, err = 0;
	struct fscrypt_name nm;
	struct fscrypt_str fstr = {0};
	union ubifs_key key;
	struct ubifs_dent_node *dent;
	struct inode *dir = file_inode(file);
	struct ubifs_info *c = dir->i_sb->s_fs_info;
	bool encrypted = IS_ENCRYPTED(dir);

	dbg_gen("dir ino %lu, f_pos %#llx", dir->i_ino, ctx->pos);

	if (ctx->pos > UBIFS_S_KEY_HASH_MASK || ctx->pos == 2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * The directory was seek'ed to a senseless position or there
		 * are no more entries.
		 */
		return 0;

<<<<<<< HEAD
=======
	if (encrypted) {
		err = fscrypt_prepare_readdir(dir);
		if (err)
			return err;

		err = fscrypt_fname_alloc_buffer(UBIFS_MAX_NLEN, &fstr);
		if (err)
			return err;

		fstr_real_len = fstr.len;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (file->f_version == 0) {
		/*
		 * The file was seek'ed, which means that @file->private_data
		 * is now invalid. This may also be just the first
		 * 'ubifs_readdir()' invocation, in which case
		 * @file->private_data is NULL, and the below code is
		 * basically a no-op.
		 */
		kfree(file->private_data);
		file->private_data = NULL;
	}

	/*
	 * 'generic_file_llseek()' unconditionally sets @file->f_version to
	 * zero, and we use this for detecting whether the file was seek'ed.
	 */
	file->f_version = 1;

	/* File positions 0 and 1 correspond to "." and ".." */
<<<<<<< HEAD
	if (pos == 0) {
		ubifs_assert(!file->private_data);
		over = filldir(dirent, ".", 1, 0, dir->i_ino, DT_DIR);
		if (over)
			return 0;
		file->f_pos = pos = 1;
	}

	if (pos == 1) {
		ubifs_assert(!file->private_data);
		over = filldir(dirent, "..", 2, 1,
			       parent_ino(file->f_path.dentry), DT_DIR);
		if (over)
			return 0;

		/* Find the first entry in TNC and save it */
		lowest_dent_key(c, &key, dir->i_ino);
		nm.name = NULL;
=======
	if (ctx->pos < 2) {
		ubifs_assert(c, !file->private_data);
		if (!dir_emit_dots(file, ctx)) {
			if (encrypted)
				fscrypt_fname_free_buffer(&fstr);
			return 0;
		}

		/* Find the first entry in TNC and save it */
		lowest_dent_key(c, &key, dir->i_ino);
		fname_len(&nm) = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dent = ubifs_tnc_next_ent(c, &key, &nm);
		if (IS_ERR(dent)) {
			err = PTR_ERR(dent);
			goto out;
		}

<<<<<<< HEAD
		file->f_pos = pos = key_hash_flash(c, &dent->key);
=======
		ctx->pos = key_hash_flash(c, &dent->key);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		file->private_data = dent;
	}

	dent = file->private_data;
	if (!dent) {
		/*
		 * The directory was seek'ed to and is now readdir'ed.
<<<<<<< HEAD
		 * Find the entry corresponding to @pos or the closest one.
		 */
		dent_key_init_hash(c, &key, dir->i_ino, pos);
		nm.name = NULL;
=======
		 * Find the entry corresponding to @ctx->pos or the closest one.
		 */
		dent_key_init_hash(c, &key, dir->i_ino, ctx->pos);
		fname_len(&nm) = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dent = ubifs_tnc_next_ent(c, &key, &nm);
		if (IS_ERR(dent)) {
			err = PTR_ERR(dent);
			goto out;
		}
<<<<<<< HEAD
		file->f_pos = pos = key_hash_flash(c, &dent->key);
=======
		ctx->pos = key_hash_flash(c, &dent->key);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		file->private_data = dent;
	}

	while (1) {
<<<<<<< HEAD
		dbg_gen("feed '%s', ino %llu, new f_pos %#x",
			dent->name, (unsigned long long)le64_to_cpu(dent->inum),
			key_hash_flash(c, &dent->key));
		ubifs_assert(le64_to_cpu(dent->ch.sqnum) >
			     ubifs_inode(dir)->creat_sqnum);

		nm.len = le16_to_cpu(dent->nlen);
		over = filldir(dirent, dent->name, nm.len, pos,
			       le64_to_cpu(dent->inum),
			       vfs_dent_type(dent->type));
		if (over)
			return 0;

		/* Switch to the next entry */
		key_read(c, &dent->key, &key);
		nm.name = dent->name;
=======
		dbg_gen("ino %llu, new f_pos %#x",
			(unsigned long long)le64_to_cpu(dent->inum),
			key_hash_flash(c, &dent->key));
		ubifs_assert(c, le64_to_cpu(dent->ch.sqnum) >
			     ubifs_inode(dir)->creat_sqnum);

		fname_len(&nm) = le16_to_cpu(dent->nlen);
		fname_name(&nm) = dent->name;

		if (encrypted) {
			fstr.len = fstr_real_len;

			err = fscrypt_fname_disk_to_usr(dir, key_hash_flash(c,
							&dent->key),
							le32_to_cpu(dent->cookie),
							&nm.disk_name, &fstr);
			if (err)
				goto out;
		} else {
			fstr.len = fname_len(&nm);
			fstr.name = fname_name(&nm);
		}

		if (!dir_emit(ctx, fstr.name, fstr.len,
			       le64_to_cpu(dent->inum),
			       vfs_dent_type(dent->type))) {
			if (encrypted)
				fscrypt_fname_free_buffer(&fstr);
			return 0;
		}

		/* Switch to the next entry */
		key_read(c, &dent->key, &key);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dent = ubifs_tnc_next_ent(c, &key, &nm);
		if (IS_ERR(dent)) {
			err = PTR_ERR(dent);
			goto out;
		}

		kfree(file->private_data);
<<<<<<< HEAD
		file->f_pos = pos = key_hash_flash(c, &dent->key);
		file->private_data = dent;
		cond_resched();

		if (file->f_version == 0)
			/*
			 * The file was seek'ed meanwhile, lets return and start
			 * reading direntries from the new position on the next
			 * invocation.
			 */
			return 0;
	}

out:
	if (err != -ENOENT) {
		ubifs_err("cannot find next direntry, error %d", err);
		return err;
	}

	kfree(file->private_data);
	file->private_data = NULL;
	/* 2 is a special value indicating that there are no more direntries */
	file->f_pos = 2;
	return 0;
}

static loff_t ubifs_dir_llseek(struct file *file, loff_t offset, int origin)
{
	return generic_file_llseek(file, offset, origin);
=======
		ctx->pos = key_hash_flash(c, &dent->key);
		file->private_data = dent;
		cond_resched();
	}

out:
	kfree(file->private_data);
	file->private_data = NULL;

	if (encrypted)
		fscrypt_fname_free_buffer(&fstr);

	if (err != -ENOENT)
		ubifs_err(c, "cannot find next direntry, error %d", err);
	else
		/*
		 * -ENOENT is a non-fatal error in this context, the TNC uses
		 * it to indicate that the cursor moved past the current directory
		 * and readdir() has to stop.
		 */
		err = 0;


	/* 2 is a special value indicating that there are no more direntries */
	ctx->pos = 2;
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Free saved readdir() state when the directory is closed */
static int ubifs_dir_release(struct inode *dir, struct file *file)
{
	kfree(file->private_data);
	file->private_data = NULL;
	return 0;
}

<<<<<<< HEAD
/**
 * lock_2_inodes - a wrapper for locking two UBIFS inodes.
 * @inode1: first inode
 * @inode2: second inode
 *
 * We do not implement any tricks to guarantee strict lock ordering, because
 * VFS has already done it for us on the @i_mutex. So this is just a simple
 * wrapper function.
 */
static void lock_2_inodes(struct inode *inode1, struct inode *inode2)
{
	mutex_lock_nested(&ubifs_inode(inode1)->ui_mutex, WB_MUTEX_1);
	mutex_lock_nested(&ubifs_inode(inode2)->ui_mutex, WB_MUTEX_2);
}

/**
 * unlock_2_inodes - a wrapper for unlocking two UBIFS inodes.
 * @inode1: first inode
 * @inode2: second inode
 */
static void unlock_2_inodes(struct inode *inode1, struct inode *inode2)
{
	mutex_unlock(&ubifs_inode(inode2)->ui_mutex);
	mutex_unlock(&ubifs_inode(inode1)->ui_mutex);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ubifs_link(struct dentry *old_dentry, struct inode *dir,
		      struct dentry *dentry)
{
	struct ubifs_info *c = dir->i_sb->s_fs_info;
<<<<<<< HEAD
	struct inode *inode = old_dentry->d_inode;
	struct ubifs_inode *ui = ubifs_inode(inode);
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	int err, sz_change = CALC_DENT_SIZE(dentry->d_name.len);
	struct ubifs_budget_req req = { .new_dent = 1, .dirtied_ino = 2,
				.dirtied_ino_d = ALIGN(ui->data_len, 8) };
=======
	struct inode *inode = d_inode(old_dentry);
	struct ubifs_inode *ui = ubifs_inode(inode);
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	int err, sz_change;
	struct ubifs_budget_req req = { .new_dent = 1, .dirtied_ino = 2,
				.dirtied_ino_d = ALIGN(ui->data_len, 8) };
	struct fscrypt_name nm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Budget request settings: new direntry, changing the target inode,
	 * changing the parent inode.
	 */

<<<<<<< HEAD
	dbg_gen("dent '%.*s' to ino %lu (nlink %d) in dir ino %lu",
		dentry->d_name.len, dentry->d_name.name, inode->i_ino,
		inode->i_nlink, dir->i_ino);
	ubifs_assert(mutex_is_locked(&dir->i_mutex));
	ubifs_assert(mutex_is_locked(&inode->i_mutex));

	err = dbg_check_synced_i_size(c, inode);
	if (err)
		return err;

	err = ubifs_budget_space(c, &req);
	if (err)
		return err;

	lock_2_inodes(dir, inode);
	inc_nlink(inode);
	ihold(inode);
	inode->i_ctime = ubifs_current_time(inode);
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
	dir->i_mtime = dir->i_ctime = inode->i_ctime;
	err = ubifs_jnl_update(c, dir, &dentry->d_name, inode, 0, 0);
=======
	dbg_gen("dent '%pd' to ino %lu (nlink %d) in dir ino %lu",
		dentry, inode->i_ino,
		inode->i_nlink, dir->i_ino);
	ubifs_assert(c, inode_is_locked(dir));
	ubifs_assert(c, inode_is_locked(inode));

	err = fscrypt_prepare_link(old_dentry, dir, dentry);
	if (err)
		return err;

	err = fscrypt_setup_filename(dir, &dentry->d_name, 0, &nm);
	if (err)
		return err;

	sz_change = CALC_DENT_SIZE(fname_len(&nm));

	err = dbg_check_synced_i_size(c, inode);
	if (err)
		goto out_fname;

	err = ubifs_budget_space(c, &req);
	if (err)
		goto out_fname;

	lock_2_inodes(dir, inode);

	/* Handle O_TMPFILE corner case, it is allowed to link a O_TMPFILE. */
	if (inode->i_nlink == 0)
		ubifs_delete_orphan(c, inode->i_ino);

	inc_nlink(inode);
	ihold(inode);
	inode_set_ctime_current(inode);
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
	inode_set_mtime_to_ts(dir,
			      inode_set_ctime_to_ts(dir, inode_get_ctime(inode)));
	err = ubifs_jnl_update(c, dir, &nm, inode, 0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out_cancel;
	unlock_2_inodes(dir, inode);

	ubifs_release_budget(c, &req);
	d_instantiate(dentry, inode);
<<<<<<< HEAD
=======
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

out_cancel:
	dir->i_size -= sz_change;
	dir_ui->ui_size = dir->i_size;
	drop_nlink(inode);
<<<<<<< HEAD
	unlock_2_inodes(dir, inode);
	ubifs_release_budget(c, &req);
	iput(inode);
=======
	if (inode->i_nlink == 0)
		ubifs_add_orphan(c, inode->i_ino);
	unlock_2_inodes(dir, inode);
	ubifs_release_budget(c, &req);
	iput(inode);
out_fname:
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int ubifs_unlink(struct inode *dir, struct dentry *dentry)
{
	struct ubifs_info *c = dir->i_sb->s_fs_info;
<<<<<<< HEAD
	struct inode *inode = dentry->d_inode;
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	int sz_change = CALC_DENT_SIZE(dentry->d_name.len);
	int err, budgeted = 1;
	struct ubifs_budget_req req = { .mod_dent = 1, .dirtied_ino = 2 };
	unsigned int saved_nlink = inode->i_nlink;
=======
	struct inode *inode = d_inode(dentry);
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	int err, sz_change, budgeted = 1;
	struct ubifs_budget_req req = { .mod_dent = 1, .dirtied_ino = 2 };
	unsigned int saved_nlink = inode->i_nlink;
	struct fscrypt_name nm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Budget request settings: deletion direntry, deletion inode (+1 for
	 * @dirtied_ino), changing the parent directory inode. If budgeting
	 * fails, go ahead anyway because we have extra space reserved for
	 * deletions.
	 */

<<<<<<< HEAD
	dbg_gen("dent '%.*s' from ino %lu (nlink %d) in dir ino %lu",
		dentry->d_name.len, dentry->d_name.name, inode->i_ino,
		inode->i_nlink, dir->i_ino);
	ubifs_assert(mutex_is_locked(&dir->i_mutex));
	ubifs_assert(mutex_is_locked(&inode->i_mutex));
	err = dbg_check_synced_i_size(c, inode);
	if (err)
		return err;

	err = ubifs_budget_space(c, &req);
	if (err) {
		if (err != -ENOSPC)
			return err;
=======
	dbg_gen("dent '%pd' from ino %lu (nlink %d) in dir ino %lu",
		dentry, inode->i_ino,
		inode->i_nlink, dir->i_ino);

	err = fscrypt_setup_filename(dir, &dentry->d_name, 1, &nm);
	if (err)
		return err;

	err = ubifs_purge_xattrs(inode);
	if (err)
		return err;

	sz_change = CALC_DENT_SIZE(fname_len(&nm));

	ubifs_assert(c, inode_is_locked(dir));
	ubifs_assert(c, inode_is_locked(inode));
	err = dbg_check_synced_i_size(c, inode);
	if (err)
		goto out_fname;

	err = ubifs_budget_space(c, &req);
	if (err) {
		if (err != -ENOSPC)
			goto out_fname;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		budgeted = 0;
	}

	lock_2_inodes(dir, inode);
<<<<<<< HEAD
	inode->i_ctime = ubifs_current_time(dir);
	drop_nlink(inode);
	dir->i_size -= sz_change;
	dir_ui->ui_size = dir->i_size;
	dir->i_mtime = dir->i_ctime = inode->i_ctime;
	err = ubifs_jnl_update(c, dir, &dentry->d_name, inode, 1, 0);
=======
	inode_set_ctime_current(inode);
	drop_nlink(inode);
	dir->i_size -= sz_change;
	dir_ui->ui_size = dir->i_size;
	inode_set_mtime_to_ts(dir,
			      inode_set_ctime_to_ts(dir, inode_get_ctime(inode)));
	err = ubifs_jnl_update(c, dir, &nm, inode, 1, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out_cancel;
	unlock_2_inodes(dir, inode);

	if (budgeted)
		ubifs_release_budget(c, &req);
	else {
		/* We've deleted something - clean the "no space" flags */
		c->bi.nospace = c->bi.nospace_rp = 0;
		smp_wmb();
	}
<<<<<<< HEAD
=======
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

out_cancel:
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
	set_nlink(inode, saved_nlink);
	unlock_2_inodes(dir, inode);
	if (budgeted)
		ubifs_release_budget(c, &req);
<<<<<<< HEAD
=======
out_fname:
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/**
<<<<<<< HEAD
 * check_dir_empty - check if a directory is empty or not.
 * @c: UBIFS file-system description object
=======
 * ubifs_check_dir_empty - check if a directory is empty or not.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @dir: VFS inode object of the directory to check
 *
 * This function checks if directory @dir is empty. Returns zero if the
 * directory is empty, %-ENOTEMPTY if it is not, and other negative error codes
<<<<<<< HEAD
 * in case of of errors.
 */
static int check_dir_empty(struct ubifs_info *c, struct inode *dir)
{
	struct qstr nm = { .name = NULL };
=======
 * in case of errors.
 */
int ubifs_check_dir_empty(struct inode *dir)
{
	struct ubifs_info *c = dir->i_sb->s_fs_info;
	struct fscrypt_name nm = { 0 };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ubifs_dent_node *dent;
	union ubifs_key key;
	int err;

	lowest_dent_key(c, &key, dir->i_ino);
	dent = ubifs_tnc_next_ent(c, &key, &nm);
	if (IS_ERR(dent)) {
		err = PTR_ERR(dent);
		if (err == -ENOENT)
			err = 0;
	} else {
		kfree(dent);
		err = -ENOTEMPTY;
	}
	return err;
}

static int ubifs_rmdir(struct inode *dir, struct dentry *dentry)
{
	struct ubifs_info *c = dir->i_sb->s_fs_info;
<<<<<<< HEAD
	struct inode *inode = dentry->d_inode;
	int sz_change = CALC_DENT_SIZE(dentry->d_name.len);
	int err, budgeted = 1;
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	struct ubifs_budget_req req = { .mod_dent = 1, .dirtied_ino = 2 };
=======
	struct inode *inode = d_inode(dentry);
	int err, sz_change, budgeted = 1;
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	struct ubifs_budget_req req = { .mod_dent = 1, .dirtied_ino = 2 };
	struct fscrypt_name nm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Budget request settings: deletion direntry, deletion inode and
	 * changing the parent inode. If budgeting fails, go ahead anyway
	 * because we have extra space reserved for deletions.
	 */

<<<<<<< HEAD
	dbg_gen("directory '%.*s', ino %lu in dir ino %lu", dentry->d_name.len,
		dentry->d_name.name, inode->i_ino, dir->i_ino);
	ubifs_assert(mutex_is_locked(&dir->i_mutex));
	ubifs_assert(mutex_is_locked(&inode->i_mutex));
	err = check_dir_empty(c, dentry->d_inode);
	if (err)
		return err;

	err = ubifs_budget_space(c, &req);
	if (err) {
		if (err != -ENOSPC)
			return err;
=======
	dbg_gen("directory '%pd', ino %lu in dir ino %lu", dentry,
		inode->i_ino, dir->i_ino);
	ubifs_assert(c, inode_is_locked(dir));
	ubifs_assert(c, inode_is_locked(inode));
	err = ubifs_check_dir_empty(d_inode(dentry));
	if (err)
		return err;

	err = fscrypt_setup_filename(dir, &dentry->d_name, 1, &nm);
	if (err)
		return err;

	err = ubifs_purge_xattrs(inode);
	if (err)
		return err;

	sz_change = CALC_DENT_SIZE(fname_len(&nm));

	err = ubifs_budget_space(c, &req);
	if (err) {
		if (err != -ENOSPC)
			goto out_fname;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		budgeted = 0;
	}

	lock_2_inodes(dir, inode);
<<<<<<< HEAD
	inode->i_ctime = ubifs_current_time(dir);
=======
	inode_set_ctime_current(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_nlink(inode);
	drop_nlink(dir);
	dir->i_size -= sz_change;
	dir_ui->ui_size = dir->i_size;
<<<<<<< HEAD
	dir->i_mtime = dir->i_ctime = inode->i_ctime;
	err = ubifs_jnl_update(c, dir, &dentry->d_name, inode, 1, 0);
=======
	inode_set_mtime_to_ts(dir,
			      inode_set_ctime_to_ts(dir, inode_get_ctime(inode)));
	err = ubifs_jnl_update(c, dir, &nm, inode, 1, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out_cancel;
	unlock_2_inodes(dir, inode);

	if (budgeted)
		ubifs_release_budget(c, &req);
	else {
		/* We've deleted something - clean the "no space" flags */
		c->bi.nospace = c->bi.nospace_rp = 0;
		smp_wmb();
	}
<<<<<<< HEAD
=======
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

out_cancel:
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
	inc_nlink(dir);
	set_nlink(inode, 2);
	unlock_2_inodes(dir, inode);
	if (budgeted)
		ubifs_release_budget(c, &req);
<<<<<<< HEAD
	return err;
}

static int ubifs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
=======
out_fname:
	fscrypt_free_filename(&nm);
	return err;
}

static int ubifs_mkdir(struct mnt_idmap *idmap, struct inode *dir,
		       struct dentry *dentry, umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode;
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	struct ubifs_info *c = dir->i_sb->s_fs_info;
<<<<<<< HEAD
	int err, sz_change = CALC_DENT_SIZE(dentry->d_name.len);
	struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1 };
=======
	int err, sz_change;
	struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1,
					.dirtied_ino = 1};
	struct fscrypt_name nm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Budget request settings: new inode, new direntry and changing parent
	 * directory inode.
	 */

<<<<<<< HEAD
	dbg_gen("dent '%.*s', mode %#hx in dir ino %lu",
		dentry->d_name.len, dentry->d_name.name, mode, dir->i_ino);
=======
	dbg_gen("dent '%pd', mode %#hx in dir ino %lu",
		dentry, mode, dir->i_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = ubifs_budget_space(c, &req);
	if (err)
		return err;

<<<<<<< HEAD
	inode = ubifs_new_inode(c, dir, S_IFDIR | mode);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_budg;
	}

=======
	err = ubifs_prepare_create(dir, dentry, &nm);
	if (err)
		goto out_budg;

	sz_change = CALC_DENT_SIZE(fname_len(&nm));

	inode = ubifs_new_inode(c, dir, S_IFDIR | mode, false);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_fname;
	}

	err = ubifs_init_security(dir, inode, &dentry->d_name);
	if (err)
		goto out_inode;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&dir_ui->ui_mutex);
	insert_inode_hash(inode);
	inc_nlink(inode);
	inc_nlink(dir);
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
<<<<<<< HEAD
	dir->i_mtime = dir->i_ctime = inode->i_ctime;
	err = ubifs_jnl_update(c, dir, &dentry->d_name, inode, 0, 0);
	if (err) {
		ubifs_err("cannot create directory, error %d", err);
=======
	inode_set_mtime_to_ts(dir,
			      inode_set_ctime_to_ts(dir, inode_get_ctime(inode)));
	err = ubifs_jnl_update(c, dir, &nm, inode, 0, 0);
	if (err) {
		ubifs_err(c, "cannot create directory, error %d", err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_cancel;
	}
	mutex_unlock(&dir_ui->ui_mutex);

	ubifs_release_budget(c, &req);
	d_instantiate(dentry, inode);
<<<<<<< HEAD
=======
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

out_cancel:
	dir->i_size -= sz_change;
	dir_ui->ui_size = dir->i_size;
	drop_nlink(dir);
	mutex_unlock(&dir_ui->ui_mutex);
<<<<<<< HEAD
	make_bad_inode(inode);
	iput(inode);
=======
out_inode:
	make_bad_inode(inode);
	iput(inode);
out_fname:
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_budg:
	ubifs_release_budget(c, &req);
	return err;
}

<<<<<<< HEAD
static int ubifs_mknod(struct inode *dir, struct dentry *dentry,
		       umode_t mode, dev_t rdev)
=======
static int ubifs_mknod(struct mnt_idmap *idmap, struct inode *dir,
		       struct dentry *dentry, umode_t mode, dev_t rdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode;
	struct ubifs_inode *ui;
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	struct ubifs_info *c = dir->i_sb->s_fs_info;
	union ubifs_dev_desc *dev = NULL;
<<<<<<< HEAD
	int sz_change = CALC_DENT_SIZE(dentry->d_name.len);
	int err, devlen = 0;
	struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1,
					.new_ino_d = ALIGN(devlen, 8),
					.dirtied_ino = 1 };
=======
	int sz_change;
	int err, devlen = 0;
	struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1,
					.dirtied_ino = 1 };
	struct fscrypt_name nm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Budget request settings: new inode, new direntry and changing parent
	 * directory inode.
	 */

<<<<<<< HEAD
	dbg_gen("dent '%.*s' in dir ino %lu",
		dentry->d_name.len, dentry->d_name.name, dir->i_ino);

	if (!new_valid_dev(rdev))
		return -EINVAL;
=======
	dbg_gen("dent '%pd' in dir ino %lu", dentry, dir->i_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (S_ISBLK(mode) || S_ISCHR(mode)) {
		dev = kmalloc(sizeof(union ubifs_dev_desc), GFP_NOFS);
		if (!dev)
			return -ENOMEM;
		devlen = ubifs_encode_dev(dev, rdev);
	}

<<<<<<< HEAD
=======
	req.new_ino_d = ALIGN(devlen, 8);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ubifs_budget_space(c, &req);
	if (err) {
		kfree(dev);
		return err;
	}

<<<<<<< HEAD
	inode = ubifs_new_inode(c, dir, mode);
	if (IS_ERR(inode)) {
		kfree(dev);
		err = PTR_ERR(inode);
		goto out_budg;
=======
	err = ubifs_prepare_create(dir, dentry, &nm);
	if (err) {
		kfree(dev);
		goto out_budg;
	}

	sz_change = CALC_DENT_SIZE(fname_len(&nm));

	inode = ubifs_new_inode(c, dir, mode, false);
	if (IS_ERR(inode)) {
		kfree(dev);
		err = PTR_ERR(inode);
		goto out_fname;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	init_special_inode(inode, inode->i_mode, rdev);
	inode->i_size = ubifs_inode(inode)->ui_size = devlen;
	ui = ubifs_inode(inode);
	ui->data = dev;
	ui->data_len = devlen;

<<<<<<< HEAD
	mutex_lock(&dir_ui->ui_mutex);
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
	dir->i_mtime = dir->i_ctime = inode->i_ctime;
	err = ubifs_jnl_update(c, dir, &dentry->d_name, inode, 0, 0);
	if (err)
		goto out_cancel;
	mutex_unlock(&dir_ui->ui_mutex);

	ubifs_release_budget(c, &req);
	insert_inode_hash(inode);
	d_instantiate(dentry, inode);
	return 0;

out_cancel:
	dir->i_size -= sz_change;
	dir_ui->ui_size = dir->i_size;
	mutex_unlock(&dir_ui->ui_mutex);
	make_bad_inode(inode);
	iput(inode);
out_budg:
	ubifs_release_budget(c, &req);
	return err;
}

static int ubifs_symlink(struct inode *dir, struct dentry *dentry,
			 const char *symname)
{
	struct inode *inode;
	struct ubifs_inode *ui;
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	struct ubifs_info *c = dir->i_sb->s_fs_info;
	int err, len = strlen(symname);
	int sz_change = CALC_DENT_SIZE(dentry->d_name.len);
	struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1,
					.new_ino_d = ALIGN(len, 8),
					.dirtied_ino = 1 };

	/*
	 * Budget request settings: new inode, new direntry and changing parent
	 * directory inode.
	 */

	dbg_gen("dent '%.*s', target '%s' in dir ino %lu", dentry->d_name.len,
		dentry->d_name.name, symname, dir->i_ino);

	if (len > UBIFS_MAX_INO_DATA)
		return -ENAMETOOLONG;

	err = ubifs_budget_space(c, &req);
	if (err)
		return err;

	inode = ubifs_new_inode(c, dir, S_IFLNK | S_IRWXUGO);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_budg;
	}

	ui = ubifs_inode(inode);
	ui->data = kmalloc(len + 1, GFP_NOFS);
	if (!ui->data) {
		err = -ENOMEM;
		goto out_inode;
	}

	memcpy(ui->data, symname, len);
	((char *)ui->data)[len] = '\0';
	/*
	 * The terminating zero byte is not written to the flash media and it
	 * is put just to make later in-memory string processing simpler. Thus,
	 * data length is @len, not @len + %1.
	 */
	ui->data_len = len;
	inode->i_size = ubifs_inode(inode)->ui_size = len;
=======
	err = ubifs_init_security(dir, inode, &dentry->d_name);
	if (err)
		goto out_inode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&dir_ui->ui_mutex);
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
<<<<<<< HEAD
	dir->i_mtime = dir->i_ctime = inode->i_ctime;
	err = ubifs_jnl_update(c, dir, &dentry->d_name, inode, 0, 0);
=======
	inode_set_mtime_to_ts(dir,
			      inode_set_ctime_to_ts(dir, inode_get_ctime(inode)));
	err = ubifs_jnl_update(c, dir, &nm, inode, 0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out_cancel;
	mutex_unlock(&dir_ui->ui_mutex);

	ubifs_release_budget(c, &req);
	insert_inode_hash(inode);
	d_instantiate(dentry, inode);
<<<<<<< HEAD
=======
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

out_cancel:
	dir->i_size -= sz_change;
	dir_ui->ui_size = dir->i_size;
	mutex_unlock(&dir_ui->ui_mutex);
out_inode:
<<<<<<< HEAD
	make_bad_inode(inode);
	iput(inode);
=======
	/* Free inode->i_link before inode is marked as bad. */
	fscrypt_free_inode(inode);
	make_bad_inode(inode);
	iput(inode);
out_fname:
	fscrypt_free_filename(&nm);
out_budg:
	ubifs_release_budget(c, &req);
	return err;
}

static int ubifs_symlink(struct mnt_idmap *idmap, struct inode *dir,
			 struct dentry *dentry, const char *symname)
{
	struct inode *inode;
	struct ubifs_inode *ui;
	struct ubifs_inode *dir_ui = ubifs_inode(dir);
	struct ubifs_info *c = dir->i_sb->s_fs_info;
	int err, sz_change, len = strlen(symname);
	struct fscrypt_str disk_link;
	struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1,
					.dirtied_ino = 1 };
	struct fscrypt_name nm;

	dbg_gen("dent '%pd', target '%s' in dir ino %lu", dentry,
		symname, dir->i_ino);

	err = fscrypt_prepare_symlink(dir, symname, len, UBIFS_MAX_INO_DATA,
				      &disk_link);
	if (err)
		return err;

	/*
	 * Budget request settings: new inode, new direntry and changing parent
	 * directory inode.
	 */
	req.new_ino_d = ALIGN(disk_link.len - 1, 8);
	err = ubifs_budget_space(c, &req);
	if (err)
		return err;

	err = ubifs_prepare_create(dir, dentry, &nm);
	if (err)
		goto out_budg;

	sz_change = CALC_DENT_SIZE(fname_len(&nm));

	inode = ubifs_new_inode(c, dir, S_IFLNK | S_IRWXUGO, false);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_fname;
	}

	ui = ubifs_inode(inode);
	ui->data = kmalloc(disk_link.len, GFP_NOFS);
	if (!ui->data) {
		err = -ENOMEM;
		goto out_inode;
	}

	if (IS_ENCRYPTED(inode)) {
		disk_link.name = ui->data; /* encrypt directly into ui->data */
		err = fscrypt_encrypt_symlink(inode, symname, len, &disk_link);
		if (err)
			goto out_inode;
	} else {
		memcpy(ui->data, disk_link.name, disk_link.len);
		inode->i_link = ui->data;
	}

	/*
	 * The terminating zero byte is not written to the flash media and it
	 * is put just to make later in-memory string processing simpler. Thus,
	 * data length is @disk_link.len - 1, not @disk_link.len.
	 */
	ui->data_len = disk_link.len - 1;
	inode->i_size = ubifs_inode(inode)->ui_size = disk_link.len - 1;

	err = ubifs_init_security(dir, inode, &dentry->d_name);
	if (err)
		goto out_inode;

	mutex_lock(&dir_ui->ui_mutex);
	dir->i_size += sz_change;
	dir_ui->ui_size = dir->i_size;
	inode_set_mtime_to_ts(dir,
			      inode_set_ctime_to_ts(dir, inode_get_ctime(inode)));
	err = ubifs_jnl_update(c, dir, &nm, inode, 0, 0);
	if (err)
		goto out_cancel;
	mutex_unlock(&dir_ui->ui_mutex);

	insert_inode_hash(inode);
	d_instantiate(dentry, inode);
	err = 0;
	goto out_fname;

out_cancel:
	dir->i_size -= sz_change;
	dir_ui->ui_size = dir->i_size;
	mutex_unlock(&dir_ui->ui_mutex);
out_inode:
	/* Free inode->i_link before inode is marked as bad. */
	fscrypt_free_inode(inode);
	make_bad_inode(inode);
	iput(inode);
out_fname:
	fscrypt_free_filename(&nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_budg:
	ubifs_release_budget(c, &req);
	return err;
}

/**
<<<<<<< HEAD
 * lock_3_inodes - a wrapper for locking three UBIFS inodes.
 * @inode1: first inode
 * @inode2: second inode
 * @inode3: third inode
 *
 * This function is used for 'ubifs_rename()' and @inode1 may be the same as
 * @inode2 whereas @inode3 may be %NULL.
=======
 * lock_4_inodes - a wrapper for locking three UBIFS inodes.
 * @inode1: first inode
 * @inode2: second inode
 * @inode3: third inode
 * @inode4: fourth inode
 *
 * This function is used for 'ubifs_rename()' and @inode1 may be the same as
 * @inode2 whereas @inode3 and @inode4 may be %NULL.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * We do not implement any tricks to guarantee strict lock ordering, because
 * VFS has already done it for us on the @i_mutex. So this is just a simple
 * wrapper function.
 */
<<<<<<< HEAD
static void lock_3_inodes(struct inode *inode1, struct inode *inode2,
			  struct inode *inode3)
=======
static void lock_4_inodes(struct inode *inode1, struct inode *inode2,
			  struct inode *inode3, struct inode *inode4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	mutex_lock_nested(&ubifs_inode(inode1)->ui_mutex, WB_MUTEX_1);
	if (inode2 != inode1)
		mutex_lock_nested(&ubifs_inode(inode2)->ui_mutex, WB_MUTEX_2);
	if (inode3)
		mutex_lock_nested(&ubifs_inode(inode3)->ui_mutex, WB_MUTEX_3);
<<<<<<< HEAD
}

/**
 * unlock_3_inodes - a wrapper for unlocking three UBIFS inodes for rename.
 * @inode1: first inode
 * @inode2: second inode
 * @inode3: third inode
 */
static void unlock_3_inodes(struct inode *inode1, struct inode *inode2,
			    struct inode *inode3)
{
=======
	if (inode4)
		mutex_lock_nested(&ubifs_inode(inode4)->ui_mutex, WB_MUTEX_4);
}

/**
 * unlock_4_inodes - a wrapper for unlocking three UBIFS inodes for rename.
 * @inode1: first inode
 * @inode2: second inode
 * @inode3: third inode
 * @inode4: fourth inode
 */
static void unlock_4_inodes(struct inode *inode1, struct inode *inode2,
			    struct inode *inode3, struct inode *inode4)
{
	if (inode4)
		mutex_unlock(&ubifs_inode(inode4)->ui_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (inode3)
		mutex_unlock(&ubifs_inode(inode3)->ui_mutex);
	if (inode1 != inode2)
		mutex_unlock(&ubifs_inode(inode2)->ui_mutex);
	mutex_unlock(&ubifs_inode(inode1)->ui_mutex);
}

<<<<<<< HEAD
static int ubifs_rename(struct inode *old_dir, struct dentry *old_dentry,
			struct inode *new_dir, struct dentry *new_dentry)
{
	struct ubifs_info *c = old_dir->i_sb->s_fs_info;
	struct inode *old_inode = old_dentry->d_inode;
	struct inode *new_inode = new_dentry->d_inode;
	struct ubifs_inode *old_inode_ui = ubifs_inode(old_inode);
	int err, release, sync = 0, move = (new_dir != old_dir);
	int is_dir = S_ISDIR(old_inode->i_mode);
	int unlink = !!new_inode;
	int new_sz = CALC_DENT_SIZE(new_dentry->d_name.len);
	int old_sz = CALC_DENT_SIZE(old_dentry->d_name.len);
=======
static int do_rename(struct inode *old_dir, struct dentry *old_dentry,
		     struct inode *new_dir, struct dentry *new_dentry,
		     unsigned int flags)
{
	struct ubifs_info *c = old_dir->i_sb->s_fs_info;
	struct inode *old_inode = d_inode(old_dentry);
	struct inode *new_inode = d_inode(new_dentry);
	struct inode *whiteout = NULL;
	struct ubifs_inode *old_inode_ui = ubifs_inode(old_inode);
	struct ubifs_inode *whiteout_ui = NULL;
	int err, release, sync = 0, move = (new_dir != old_dir);
	int is_dir = S_ISDIR(old_inode->i_mode);
	int unlink = !!new_inode, new_sz, old_sz;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ubifs_budget_req req = { .new_dent = 1, .mod_dent = 1,
					.dirtied_ino = 3 };
	struct ubifs_budget_req ino_req = { .dirtied_ino = 1,
			.dirtied_ino_d = ALIGN(old_inode_ui->data_len, 8) };
<<<<<<< HEAD
	struct timespec time;
	unsigned int uninitialized_var(saved_nlink);

	/*
	 * Budget request settings: deletion direntry, new direntry, removing
	 * the old inode, and changing old and new parent directory inodes.
	 *
	 * However, this operation also marks the target inode as dirty and
	 * does not write it, so we allocate budget for the target inode
	 * separately.
	 */

	dbg_gen("dent '%.*s' ino %lu in dir ino %lu to dent '%.*s' in "
		"dir ino %lu", old_dentry->d_name.len, old_dentry->d_name.name,
		old_inode->i_ino, old_dir->i_ino, new_dentry->d_name.len,
		new_dentry->d_name.name, new_dir->i_ino);
	ubifs_assert(mutex_is_locked(&old_dir->i_mutex));
	ubifs_assert(mutex_is_locked(&new_dir->i_mutex));
	if (unlink)
		ubifs_assert(mutex_is_locked(&new_inode->i_mutex));


	if (unlink && is_dir) {
		err = check_dir_empty(c, new_inode);
=======
	struct ubifs_budget_req wht_req;
	unsigned int saved_nlink;
	struct fscrypt_name old_nm, new_nm;

	/*
	 * Budget request settings:
	 *   req: deletion direntry, new direntry, removing the old inode,
	 *   and changing old and new parent directory inodes.
	 *
	 *   wht_req: new whiteout inode for RENAME_WHITEOUT.
	 *
	 *   ino_req: marks the target inode as dirty and does not write it.
	 */

	dbg_gen("dent '%pd' ino %lu in dir ino %lu to dent '%pd' in dir ino %lu flags 0x%x",
		old_dentry, old_inode->i_ino, old_dir->i_ino,
		new_dentry, new_dir->i_ino, flags);

	if (unlink) {
		ubifs_assert(c, inode_is_locked(new_inode));

		/* Budget for old inode's data when its nlink > 1. */
		req.dirtied_ino_d = ALIGN(ubifs_inode(new_inode)->data_len, 8);
		err = ubifs_purge_xattrs(new_inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			return err;
	}

<<<<<<< HEAD
	err = ubifs_budget_space(c, &req);
	if (err)
		return err;
	err = ubifs_budget_space(c, &ino_req);
	if (err) {
=======
	if (unlink && is_dir) {
		err = ubifs_check_dir_empty(new_inode);
		if (err)
			return err;
	}

	err = fscrypt_setup_filename(old_dir, &old_dentry->d_name, 0, &old_nm);
	if (err)
		return err;

	err = fscrypt_setup_filename(new_dir, &new_dentry->d_name, 0, &new_nm);
	if (err) {
		fscrypt_free_filename(&old_nm);
		return err;
	}

	new_sz = CALC_DENT_SIZE(fname_len(&new_nm));
	old_sz = CALC_DENT_SIZE(fname_len(&old_nm));

	err = ubifs_budget_space(c, &req);
	if (err) {
		fscrypt_free_filename(&old_nm);
		fscrypt_free_filename(&new_nm);
		return err;
	}
	err = ubifs_budget_space(c, &ino_req);
	if (err) {
		fscrypt_free_filename(&old_nm);
		fscrypt_free_filename(&new_nm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ubifs_release_budget(c, &req);
		return err;
	}

<<<<<<< HEAD
	lock_3_inodes(old_dir, new_dir, new_inode);
=======
	if (flags & RENAME_WHITEOUT) {
		union ubifs_dev_desc *dev = NULL;

		dev = kmalloc(sizeof(union ubifs_dev_desc), GFP_NOFS);
		if (!dev) {
			err = -ENOMEM;
			goto out_release;
		}

		/*
		 * The whiteout inode without dentry is pinned in memory,
		 * umount won't happen during rename process because we
		 * got parent dentry.
		 */
		whiteout = create_whiteout(old_dir, old_dentry);
		if (IS_ERR(whiteout)) {
			err = PTR_ERR(whiteout);
			kfree(dev);
			goto out_release;
		}

		whiteout_ui = ubifs_inode(whiteout);
		whiteout_ui->data = dev;
		whiteout_ui->data_len = ubifs_encode_dev(dev, MKDEV(0, 0));
		ubifs_assert(c, !whiteout_ui->dirty);

		memset(&wht_req, 0, sizeof(struct ubifs_budget_req));
		wht_req.new_ino = 1;
		wht_req.new_ino_d = ALIGN(whiteout_ui->data_len, 8);
		/*
		 * To avoid deadlock between space budget (holds ui_mutex and
		 * waits wb work) and writeback work(waits ui_mutex), do space
		 * budget before ubifs inodes locked.
		 */
		err = ubifs_budget_space(c, &wht_req);
		if (err) {
			/*
			 * Whiteout inode can not be written on flash by
			 * ubifs_jnl_write_inode(), because it's neither
			 * dirty nor zero-nlink.
			 */
			iput(whiteout);
			goto out_release;
		}

		/* Add the old_dentry size to the old_dir size. */
		old_sz -= CALC_DENT_SIZE(fname_len(&old_nm));
	}

	lock_4_inodes(old_dir, new_dir, new_inode, whiteout);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Like most other Unix systems, set the @i_ctime for inodes on a
	 * rename.
	 */
<<<<<<< HEAD
	time = ubifs_current_time(old_dir);
	old_inode->i_ctime = time;
=======
	simple_rename_timestamp(old_dir, old_dentry, new_dir, new_dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We must adjust parent link count when renaming directories */
	if (is_dir) {
		if (move) {
			/*
			 * @old_dir loses a link because we are moving
			 * @old_inode to a different directory.
			 */
			drop_nlink(old_dir);
			/*
			 * @new_dir only gains a link if we are not also
			 * overwriting an existing directory.
			 */
			if (!unlink)
				inc_nlink(new_dir);
		} else {
			/*
			 * @old_inode is not moving to a different directory,
			 * but @old_dir still loses a link if we are
			 * overwriting an existing directory.
			 */
			if (unlink)
				drop_nlink(old_dir);
		}
	}

	old_dir->i_size -= old_sz;
	ubifs_inode(old_dir)->ui_size = old_dir->i_size;
<<<<<<< HEAD
	old_dir->i_mtime = old_dir->i_ctime = time;
	new_dir->i_mtime = new_dir->i_ctime = time;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * And finally, if we unlinked a direntry which happened to have the
	 * same name as the moved direntry, we have to decrement @i_nlink of
<<<<<<< HEAD
	 * the unlinked inode and change its ctime.
=======
	 * the unlinked inode.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	if (unlink) {
		/*
		 * Directories cannot have hard-links, so if this is a
		 * directory, just clear @i_nlink.
		 */
		saved_nlink = new_inode->i_nlink;
		if (is_dir)
			clear_nlink(new_inode);
		else
			drop_nlink(new_inode);
<<<<<<< HEAD
		new_inode->i_ctime = time;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		new_dir->i_size += new_sz;
		ubifs_inode(new_dir)->ui_size = new_dir->i_size;
	}

	/*
	 * Do not ask 'ubifs_jnl_rename()' to flush write-buffer if @old_inode
	 * is dirty, because this will be done later on at the end of
	 * 'ubifs_rename()'.
	 */
	if (IS_SYNC(old_inode)) {
		sync = IS_DIRSYNC(old_dir) || IS_DIRSYNC(new_dir);
		if (unlink && IS_SYNC(new_inode))
			sync = 1;
<<<<<<< HEAD
	}
	err = ubifs_jnl_rename(c, old_dir, old_dentry, new_dir, new_dentry,
			       sync);
	if (err)
		goto out_cancel;

	unlock_3_inodes(old_dir, new_dir, new_inode);
	ubifs_release_budget(c, &req);

=======
		/*
		 * S_SYNC flag of whiteout inherits from the old_dir, and we
		 * have already checked the old dir inode. So there is no need
		 * to check whiteout.
		 */
	}

	err = ubifs_jnl_rename(c, old_dir, old_inode, &old_nm, new_dir,
			       new_inode, &new_nm, whiteout, sync);
	if (err)
		goto out_cancel;

	unlock_4_inodes(old_dir, new_dir, new_inode, whiteout);
	ubifs_release_budget(c, &req);

	if (whiteout) {
		ubifs_release_budget(c, &wht_req);
		iput(whiteout);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&old_inode_ui->ui_mutex);
	release = old_inode_ui->dirty;
	mark_inode_dirty_sync(old_inode);
	mutex_unlock(&old_inode_ui->ui_mutex);

	if (release)
		ubifs_release_budget(c, &ino_req);
	if (IS_SYNC(old_inode))
<<<<<<< HEAD
		err = old_inode->i_sb->s_op->write_inode(old_inode, NULL);
	return err;
=======
		/*
		 * Rename finished here. Although old inode cannot be updated
		 * on flash, old ctime is not a big problem, don't return err
		 * code to userspace.
		 */
		old_inode->i_sb->s_op->write_inode(old_inode, NULL);

	fscrypt_free_filename(&old_nm);
	fscrypt_free_filename(&new_nm);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out_cancel:
	if (unlink) {
		set_nlink(new_inode, saved_nlink);
	} else {
		new_dir->i_size -= new_sz;
		ubifs_inode(new_dir)->ui_size = new_dir->i_size;
	}
	old_dir->i_size += old_sz;
	ubifs_inode(old_dir)->ui_size = old_dir->i_size;
	if (is_dir) {
		if (move) {
			inc_nlink(old_dir);
			if (!unlink)
				drop_nlink(new_dir);
		} else {
			if (unlink)
				inc_nlink(old_dir);
		}
	}
<<<<<<< HEAD
	unlock_3_inodes(old_dir, new_dir, new_inode);
	ubifs_release_budget(c, &ino_req);
	ubifs_release_budget(c, &req);
	return err;
}

int ubifs_getattr(struct vfsmount *mnt, struct dentry *dentry,
		  struct kstat *stat)
{
	loff_t size;
	struct inode *inode = dentry->d_inode;
	struct ubifs_inode *ui = ubifs_inode(inode);

	mutex_lock(&ui->ui_mutex);
	stat->dev = inode->i_sb->s_dev;
	stat->ino = inode->i_ino;
	stat->mode = inode->i_mode;
	stat->nlink = inode->i_nlink;
	stat->uid = inode->i_uid;
	stat->gid = inode->i_gid;
	stat->rdev = inode->i_rdev;
	stat->atime = inode->i_atime;
	stat->mtime = inode->i_mtime;
	stat->ctime = inode->i_ctime;
=======
	unlock_4_inodes(old_dir, new_dir, new_inode, whiteout);
	if (whiteout) {
		ubifs_release_budget(c, &wht_req);
		iput(whiteout);
	}
out_release:
	ubifs_release_budget(c, &ino_req);
	ubifs_release_budget(c, &req);
	fscrypt_free_filename(&old_nm);
	fscrypt_free_filename(&new_nm);
	return err;
}

static int ubifs_xrename(struct inode *old_dir, struct dentry *old_dentry,
			struct inode *new_dir, struct dentry *new_dentry)
{
	struct ubifs_info *c = old_dir->i_sb->s_fs_info;
	struct ubifs_budget_req req = { .new_dent = 1, .mod_dent = 1,
				.dirtied_ino = 2 };
	int sync = IS_DIRSYNC(old_dir) || IS_DIRSYNC(new_dir);
	struct inode *fst_inode = d_inode(old_dentry);
	struct inode *snd_inode = d_inode(new_dentry);
	int err;
	struct fscrypt_name fst_nm, snd_nm;

	ubifs_assert(c, fst_inode && snd_inode);

	/*
	 * Budget request settings: changing two direntries, changing the two
	 * parent directory inodes.
	 */

	dbg_gen("dent '%pd' ino %lu in dir ino %lu exchange dent '%pd' ino %lu in dir ino %lu",
		old_dentry, fst_inode->i_ino, old_dir->i_ino,
		new_dentry, snd_inode->i_ino, new_dir->i_ino);

	err = fscrypt_setup_filename(old_dir, &old_dentry->d_name, 0, &fst_nm);
	if (err)
		return err;

	err = fscrypt_setup_filename(new_dir, &new_dentry->d_name, 0, &snd_nm);
	if (err) {
		fscrypt_free_filename(&fst_nm);
		return err;
	}

	err = ubifs_budget_space(c, &req);
	if (err)
		goto out;

	lock_4_inodes(old_dir, new_dir, NULL, NULL);

	simple_rename_timestamp(old_dir, old_dentry, new_dir, new_dentry);

	if (old_dir != new_dir) {
		if (S_ISDIR(fst_inode->i_mode) && !S_ISDIR(snd_inode->i_mode)) {
			inc_nlink(new_dir);
			drop_nlink(old_dir);
		}
		else if (!S_ISDIR(fst_inode->i_mode) && S_ISDIR(snd_inode->i_mode)) {
			drop_nlink(new_dir);
			inc_nlink(old_dir);
		}
	}

	err = ubifs_jnl_xrename(c, old_dir, fst_inode, &fst_nm, new_dir,
				snd_inode, &snd_nm, sync);

	unlock_4_inodes(old_dir, new_dir, NULL, NULL);
	ubifs_release_budget(c, &req);

out:
	fscrypt_free_filename(&fst_nm);
	fscrypt_free_filename(&snd_nm);
	return err;
}

static int ubifs_rename(struct mnt_idmap *idmap,
			struct inode *old_dir, struct dentry *old_dentry,
			struct inode *new_dir, struct dentry *new_dentry,
			unsigned int flags)
{
	int err;
	struct ubifs_info *c = old_dir->i_sb->s_fs_info;

	if (flags & ~(RENAME_NOREPLACE | RENAME_WHITEOUT | RENAME_EXCHANGE))
		return -EINVAL;

	ubifs_assert(c, inode_is_locked(old_dir));
	ubifs_assert(c, inode_is_locked(new_dir));

	err = fscrypt_prepare_rename(old_dir, old_dentry, new_dir, new_dentry,
				     flags);
	if (err)
		return err;

	if (flags & RENAME_EXCHANGE)
		return ubifs_xrename(old_dir, old_dentry, new_dir, new_dentry);

	return do_rename(old_dir, old_dentry, new_dir, new_dentry, flags);
}

int ubifs_getattr(struct mnt_idmap *idmap, const struct path *path,
		  struct kstat *stat, u32 request_mask, unsigned int flags)
{
	loff_t size;
	struct inode *inode = d_inode(path->dentry);
	struct ubifs_inode *ui = ubifs_inode(inode);

	mutex_lock(&ui->ui_mutex);

	if (ui->flags & UBIFS_APPEND_FL)
		stat->attributes |= STATX_ATTR_APPEND;
	if (ui->flags & UBIFS_COMPR_FL)
		stat->attributes |= STATX_ATTR_COMPRESSED;
	if (ui->flags & UBIFS_CRYPT_FL)
		stat->attributes |= STATX_ATTR_ENCRYPTED;
	if (ui->flags & UBIFS_IMMUTABLE_FL)
		stat->attributes |= STATX_ATTR_IMMUTABLE;

	stat->attributes_mask |= (STATX_ATTR_APPEND |
				STATX_ATTR_COMPRESSED |
				STATX_ATTR_ENCRYPTED |
				STATX_ATTR_IMMUTABLE);

	generic_fillattr(&nop_mnt_idmap, request_mask, inode, stat);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	stat->blksize = UBIFS_BLOCK_SIZE;
	stat->size = ui->ui_size;

	/*
	 * Unfortunately, the 'stat()' system call was designed for block
	 * device based file systems, and it is not appropriate for UBIFS,
	 * because UBIFS does not have notion of "block". For example, it is
	 * difficult to tell how many block a directory takes - it actually
	 * takes less than 300 bytes, but we have to round it to block size,
	 * which introduces large mistake. This makes utilities like 'du' to
	 * report completely senseless numbers. This is the reason why UBIFS
	 * goes the same way as JFFS2 - it reports zero blocks for everything
	 * but regular files, which makes more sense than reporting completely
	 * wrong sizes.
	 */
	if (S_ISREG(inode->i_mode)) {
		size = ui->xattr_size;
		size += stat->size;
		size = ALIGN(size, UBIFS_BLOCK_SIZE);
		/*
		 * Note, user-space expects 512-byte blocks count irrespectively
		 * of what was reported in @stat->size.
		 */
		stat->blocks = size >> 9;
	} else
		stat->blocks = 0;
	mutex_unlock(&ui->ui_mutex);
	return 0;
}

const struct inode_operations ubifs_dir_inode_operations = {
	.lookup      = ubifs_lookup,
	.create      = ubifs_create,
	.link        = ubifs_link,
	.symlink     = ubifs_symlink,
	.unlink      = ubifs_unlink,
	.mkdir       = ubifs_mkdir,
	.rmdir       = ubifs_rmdir,
	.mknod       = ubifs_mknod,
	.rename      = ubifs_rename,
	.setattr     = ubifs_setattr,
	.getattr     = ubifs_getattr,
<<<<<<< HEAD
#ifdef CONFIG_UBIFS_FS_XATTR
	.setxattr    = ubifs_setxattr,
	.getxattr    = ubifs_getxattr,
	.listxattr   = ubifs_listxattr,
	.removexattr = ubifs_removexattr,
#endif
};

const struct file_operations ubifs_dir_operations = {
	.llseek         = ubifs_dir_llseek,
	.release        = ubifs_dir_release,
	.read           = generic_read_dir,
	.readdir        = ubifs_readdir,
=======
	.listxattr   = ubifs_listxattr,
	.update_time = ubifs_update_time,
	.tmpfile     = ubifs_tmpfile,
	.fileattr_get = ubifs_fileattr_get,
	.fileattr_set = ubifs_fileattr_set,
};

const struct file_operations ubifs_dir_operations = {
	.llseek         = generic_file_llseek,
	.release        = ubifs_dir_release,
	.read           = generic_read_dir,
	.iterate_shared = ubifs_readdir,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.fsync          = ubifs_fsync,
	.unlocked_ioctl = ubifs_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl   = ubifs_compat_ioctl,
#endif
};
