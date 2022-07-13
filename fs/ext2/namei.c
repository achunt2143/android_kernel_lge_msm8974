<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/ext2/namei.c
 *
 * Rewrite to pagecache. Almost all code had been changed, so blame me
 * if the things go wrong. Please, send bug reports to
 * viro@parcelfarce.linux.theplanet.co.uk
 *
 * Stuff here is basically a glue between the VFS and generic UNIXish
 * filesystem that keeps everything in pagecache. All knowledge of the
 * directory layout is in fs/ext2/dir.c - it turned out to be easily separatable
 * and it's easier to debug that way. In principle we might want to
 * generalize that a bit and turn it into a library. Or not.
 *
 * The only non-static object here is ext2_dir_inode_operations.
 *
 * TODO: get rid of kmap() use, add readahead.
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/fs/minix/namei.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  Big-endian to little-endian byte-swapping/bitmaps by
 *        David S. Miller (davem@caip.rutgers.edu), 1995
 */

#include <linux/pagemap.h>
#include <linux/quotaops.h>
#include "ext2.h"
#include "xattr.h"
#include "acl.h"
<<<<<<< HEAD
#include "xip.h"
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int ext2_add_nondir(struct dentry *dentry, struct inode *inode)
{
	int err = ext2_add_link(dentry, inode);
	if (!err) {
<<<<<<< HEAD
		d_instantiate(dentry, inode);
		unlock_new_inode(inode);
		return 0;
	}
	inode_dec_link_count(inode);
	unlock_new_inode(inode);
	iput(inode);
=======
		d_instantiate_new(dentry, inode);
		return 0;
	}
	inode_dec_link_count(inode);
	discard_new_inode(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/*
 * Methods themselves.
 */

static struct dentry *ext2_lookup(struct inode * dir, struct dentry *dentry, unsigned int flags)
{
	struct inode * inode;
	ino_t ino;
<<<<<<< HEAD
=======
	int res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	
	if (dentry->d_name.len > EXT2_NAME_LEN)
		return ERR_PTR(-ENAMETOOLONG);

<<<<<<< HEAD
	ino = ext2_inode_by_name(dir, &dentry->d_name);
	inode = NULL;
	if (ino) {
=======
	res = ext2_inode_by_name(dir, &dentry->d_name, &ino);
	if (res) {
		if (res != -ENOENT)
			return ERR_PTR(res);
		inode = NULL;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		inode = ext2_iget(dir->i_sb, ino);
		if (inode == ERR_PTR(-ESTALE)) {
			ext2_error(dir->i_sb, __func__,
					"deleted inode referenced: %lu",
					(unsigned long) ino);
			return ERR_PTR(-EIO);
		}
	}
	return d_splice_alias(inode, dentry);
}

struct dentry *ext2_get_parent(struct dentry *child)
{
<<<<<<< HEAD
	struct qstr dotdot = QSTR_INIT("..", 2);
	unsigned long ino = ext2_inode_by_name(child->d_inode, &dotdot);
	if (!ino)
		return ERR_PTR(-ENOENT);
	return d_obtain_alias(ext2_iget(child->d_inode->i_sb, ino));
=======
	ino_t ino;
	int res;

	res = ext2_inode_by_name(d_inode(child), &dotdot_name, &ino);
	if (res)
		return ERR_PTR(res);

	return d_obtain_alias(ext2_iget(child->d_sb, ino));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} 

/*
 * By the time this is called, we already have created
 * the directory cache entry for the new file, but it
 * is so far negative - it has no inode.
 *
 * If the create succeeds, we fill in the inode information
 * with d_instantiate(). 
 */
<<<<<<< HEAD
static int ext2_create (struct inode * dir, struct dentry * dentry, umode_t mode, bool excl)
{
	struct inode *inode;

	dquot_initialize(dir);
=======
static int ext2_create (struct mnt_idmap * idmap,
			struct inode * dir, struct dentry * dentry,
			umode_t mode, bool excl)
{
	struct inode *inode;
	int err;

	err = dquot_initialize(dir);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode = ext2_new_inode(dir, mode, &dentry->d_name);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

<<<<<<< HEAD
	inode->i_op = &ext2_file_inode_operations;
	if (ext2_use_xip(inode->i_sb)) {
		inode->i_mapping->a_ops = &ext2_aops_xip;
		inode->i_fop = &ext2_xip_file_operations;
	} else if (test_opt(inode->i_sb, NOBH)) {
		inode->i_mapping->a_ops = &ext2_nobh_aops;
		inode->i_fop = &ext2_file_operations;
	} else {
		inode->i_mapping->a_ops = &ext2_aops;
		inode->i_fop = &ext2_file_operations;
	}
=======
	ext2_set_file_ops(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mark_inode_dirty(inode);
	return ext2_add_nondir(dentry, inode);
}

<<<<<<< HEAD
static int ext2_tmpfile(struct inode *dir, struct dentry *dentry, umode_t mode)
=======
static int ext2_tmpfile(struct mnt_idmap *idmap, struct inode *dir,
			struct file *file, umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode = ext2_new_inode(dir, mode, NULL);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

<<<<<<< HEAD
	inode->i_op = &ext2_file_inode_operations;
	if (ext2_use_xip(inode->i_sb)) {
		inode->i_mapping->a_ops = &ext2_aops_xip;
		inode->i_fop = &ext2_xip_file_operations;
	} else if (test_opt(inode->i_sb, NOBH)) {
		inode->i_mapping->a_ops = &ext2_nobh_aops;
		inode->i_fop = &ext2_file_operations;
	} else {
		inode->i_mapping->a_ops = &ext2_aops;
		inode->i_fop = &ext2_file_operations;
	}
	mark_inode_dirty(inode);
	d_tmpfile(dentry, inode);
	unlock_new_inode(inode);
	return 0;
}

static int ext2_mknod (struct inode * dir, struct dentry *dentry, umode_t mode, dev_t rdev)
=======
	ext2_set_file_ops(inode);
	mark_inode_dirty(inode);
	d_tmpfile(file, inode);
	unlock_new_inode(inode);
	return finish_open_simple(file, 0);
}

static int ext2_mknod (struct mnt_idmap * idmap, struct inode * dir,
	struct dentry *dentry, umode_t mode, dev_t rdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode * inode;
	int err;

<<<<<<< HEAD
	if (!new_valid_dev(rdev))
		return -EINVAL;

	dquot_initialize(dir);
=======
	err = dquot_initialize(dir);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode = ext2_new_inode (dir, mode, &dentry->d_name);
	err = PTR_ERR(inode);
	if (!IS_ERR(inode)) {
		init_special_inode(inode, inode->i_mode, rdev);
<<<<<<< HEAD
#ifdef CONFIG_EXT2_FS_XATTR
		inode->i_op = &ext2_special_inode_operations;
#endif
=======
		inode->i_op = &ext2_special_inode_operations;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mark_inode_dirty(inode);
		err = ext2_add_nondir(dentry, inode);
	}
	return err;
}

<<<<<<< HEAD
static int ext2_symlink (struct inode * dir, struct dentry * dentry,
	const char * symname)
=======
static int ext2_symlink (struct mnt_idmap * idmap, struct inode * dir,
	struct dentry * dentry, const char * symname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct super_block * sb = dir->i_sb;
	int err = -ENAMETOOLONG;
	unsigned l = strlen(symname)+1;
	struct inode * inode;

	if (l > sb->s_blocksize)
		goto out;

<<<<<<< HEAD
	dquot_initialize(dir);
=======
	err = dquot_initialize(dir);
	if (err)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode = ext2_new_inode (dir, S_IFLNK | S_IRWXUGO, &dentry->d_name);
	err = PTR_ERR(inode);
	if (IS_ERR(inode))
		goto out;

	if (l > sizeof (EXT2_I(inode)->i_data)) {
		/* slow symlink */
		inode->i_op = &ext2_symlink_inode_operations;
<<<<<<< HEAD
		if (test_opt(inode->i_sb, NOBH))
			inode->i_mapping->a_ops = &ext2_nobh_aops;
		else
			inode->i_mapping->a_ops = &ext2_aops;
=======
		inode_nohighmem(inode);
		inode->i_mapping->a_ops = &ext2_aops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = page_symlink(inode, symname, l);
		if (err)
			goto out_fail;
	} else {
		/* fast symlink */
		inode->i_op = &ext2_fast_symlink_inode_operations;
<<<<<<< HEAD
		memcpy((char*)(EXT2_I(inode)->i_data),symname,l);
=======
		inode->i_link = (char*)EXT2_I(inode)->i_data;
		memcpy(inode->i_link, symname, l);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		inode->i_size = l-1;
	}
	mark_inode_dirty(inode);

	err = ext2_add_nondir(dentry, inode);
out:
	return err;

out_fail:
	inode_dec_link_count(inode);
<<<<<<< HEAD
	unlock_new_inode(inode);
	iput (inode);
=======
	discard_new_inode(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto out;
}

static int ext2_link (struct dentry * old_dentry, struct inode * dir,
	struct dentry *dentry)
{
<<<<<<< HEAD
	struct inode *inode = old_dentry->d_inode;
	int err;

	dquot_initialize(dir);

	inode->i_ctime = CURRENT_TIME_SEC;
=======
	struct inode *inode = d_inode(old_dentry);
	int err;

	err = dquot_initialize(dir);
	if (err)
		return err;

	inode_set_ctime_current(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inode_inc_link_count(inode);
	ihold(inode);

	err = ext2_add_link(dentry, inode);
	if (!err) {
		d_instantiate(dentry, inode);
		return 0;
	}
	inode_dec_link_count(inode);
	iput(inode);
	return err;
}

<<<<<<< HEAD
static int ext2_mkdir(struct inode * dir, struct dentry * dentry, umode_t mode)
=======
static int ext2_mkdir(struct mnt_idmap * idmap,
	struct inode * dir, struct dentry * dentry, umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode * inode;
	int err;

<<<<<<< HEAD
	dquot_initialize(dir);
=======
	err = dquot_initialize(dir);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode_inc_link_count(dir);

	inode = ext2_new_inode(dir, S_IFDIR | mode, &dentry->d_name);
	err = PTR_ERR(inode);
	if (IS_ERR(inode))
		goto out_dir;

	inode->i_op = &ext2_dir_inode_operations;
	inode->i_fop = &ext2_dir_operations;
<<<<<<< HEAD
	if (test_opt(inode->i_sb, NOBH))
		inode->i_mapping->a_ops = &ext2_nobh_aops;
	else
		inode->i_mapping->a_ops = &ext2_aops;
=======
	inode->i_mapping->a_ops = &ext2_aops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode_inc_link_count(inode);

	err = ext2_make_empty(inode, dir);
	if (err)
		goto out_fail;

	err = ext2_add_link(dentry, inode);
	if (err)
		goto out_fail;

<<<<<<< HEAD
	d_instantiate(dentry, inode);
	unlock_new_inode(inode);
=======
	d_instantiate_new(dentry, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;

out_fail:
	inode_dec_link_count(inode);
	inode_dec_link_count(inode);
<<<<<<< HEAD
	unlock_new_inode(inode);
	iput(inode);
=======
	discard_new_inode(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_dir:
	inode_dec_link_count(dir);
	goto out;
}

<<<<<<< HEAD
static int ext2_unlink(struct inode * dir, struct dentry *dentry)
{
	struct inode * inode = dentry->d_inode;
	struct ext2_dir_entry_2 * de;
	struct page * page;
	int err = -ENOENT;

	dquot_initialize(dir);

	de = ext2_find_entry (dir, &dentry->d_name, &page);
	if (!de)
		goto out;

	err = ext2_delete_entry (de, page);
	if (err)
		goto out;

	inode->i_ctime = dir->i_ctime;
=======
static int ext2_unlink(struct inode *dir, struct dentry *dentry)
{
	struct inode *inode = d_inode(dentry);
	struct ext2_dir_entry_2 *de;
	struct folio *folio;
	int err;

	err = dquot_initialize(dir);
	if (err)
		goto out;

	de = ext2_find_entry(dir, &dentry->d_name, &folio);
	if (IS_ERR(de)) {
		err = PTR_ERR(de);
		goto out;
	}

	err = ext2_delete_entry(de, folio);
	folio_release_kmap(folio, de);
	if (err)
		goto out;

	inode_set_ctime_to_ts(inode, inode_get_ctime(dir));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inode_dec_link_count(inode);
	err = 0;
out:
	return err;
}

static int ext2_rmdir (struct inode * dir, struct dentry *dentry)
{
<<<<<<< HEAD
	struct inode * inode = dentry->d_inode;
=======
	struct inode * inode = d_inode(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = -ENOTEMPTY;

	if (ext2_empty_dir(inode)) {
		err = ext2_unlink(dir, dentry);
		if (!err) {
			inode->i_size = 0;
			inode_dec_link_count(inode);
			inode_dec_link_count(dir);
		}
	}
	return err;
}

<<<<<<< HEAD
static int ext2_rename (struct inode * old_dir, struct dentry * old_dentry,
	struct inode * new_dir,	struct dentry * new_dentry )
{
	struct inode * old_inode = old_dentry->d_inode;
	struct inode * new_inode = new_dentry->d_inode;
	struct page * dir_page = NULL;
	struct ext2_dir_entry_2 * dir_de = NULL;
	struct page * old_page;
	struct ext2_dir_entry_2 * old_de;
	int err = -ENOENT;

	dquot_initialize(old_dir);
	dquot_initialize(new_dir);

	old_de = ext2_find_entry (old_dir, &old_dentry->d_name, &old_page);
	if (!old_de)
		goto out;

	if (S_ISDIR(old_inode->i_mode)) {
		err = -EIO;
		dir_de = ext2_dotdot(old_inode, &dir_page);
=======
static int ext2_rename (struct mnt_idmap * idmap,
			struct inode * old_dir, struct dentry * old_dentry,
			struct inode * new_dir, struct dentry * new_dentry,
			unsigned int flags)
{
	struct inode * old_inode = d_inode(old_dentry);
	struct inode * new_inode = d_inode(new_dentry);
	struct folio *dir_folio = NULL;
	struct ext2_dir_entry_2 * dir_de = NULL;
	struct folio * old_folio;
	struct ext2_dir_entry_2 * old_de;
	bool old_is_dir = S_ISDIR(old_inode->i_mode);
	int err;

	if (flags & ~RENAME_NOREPLACE)
		return -EINVAL;

	err = dquot_initialize(old_dir);
	if (err)
		return err;

	err = dquot_initialize(new_dir);
	if (err)
		return err;

	old_de = ext2_find_entry(old_dir, &old_dentry->d_name, &old_folio);
	if (IS_ERR(old_de))
		return PTR_ERR(old_de);

	if (old_is_dir && old_dir != new_dir) {
		err = -EIO;
		dir_de = ext2_dotdot(old_inode, &dir_folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!dir_de)
			goto out_old;
	}

	if (new_inode) {
<<<<<<< HEAD
		struct page *new_page;
		struct ext2_dir_entry_2 *new_de;

		err = -ENOTEMPTY;
		if (dir_de && !ext2_empty_dir (new_inode))
			goto out_dir;

		err = -ENOENT;
		new_de = ext2_find_entry (new_dir, &new_dentry->d_name, &new_page);
		if (!new_de)
			goto out_dir;
		ext2_set_link(new_dir, new_de, new_page, old_inode, 1);
		new_inode->i_ctime = CURRENT_TIME_SEC;
		if (dir_de)
=======
		struct folio *new_folio;
		struct ext2_dir_entry_2 *new_de;

		err = -ENOTEMPTY;
		if (old_is_dir && !ext2_empty_dir(new_inode))
			goto out_dir;

		new_de = ext2_find_entry(new_dir, &new_dentry->d_name,
					 &new_folio);
		if (IS_ERR(new_de)) {
			err = PTR_ERR(new_de);
			goto out_dir;
		}
		err = ext2_set_link(new_dir, new_de, new_folio, old_inode, true);
		folio_release_kmap(new_folio, new_de);
		if (err)
			goto out_dir;
		inode_set_ctime_current(new_inode);
		if (old_is_dir)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			drop_nlink(new_inode);
		inode_dec_link_count(new_inode);
	} else {
		err = ext2_add_link(new_dentry, old_inode);
		if (err)
			goto out_dir;
<<<<<<< HEAD
		if (dir_de)
=======
		if (old_is_dir)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			inode_inc_link_count(new_dir);
	}

	/*
	 * Like most other Unix systems, set the ctime for inodes on a
 	 * rename.
	 */
<<<<<<< HEAD
	old_inode->i_ctime = CURRENT_TIME_SEC;
	mark_inode_dirty(old_inode);

	ext2_delete_entry (old_de, old_page);

	if (dir_de) {
		if (old_dir != new_dir)
			ext2_set_link(old_inode, dir_de, dir_page, new_dir, 0);
		else {
			kunmap(dir_page);
			page_cache_release(dir_page);
		}
		inode_dec_link_count(old_dir);
	}
	return 0;


out_dir:
	if (dir_de) {
		kunmap(dir_page);
		page_cache_release(dir_page);
	}
out_old:
	kunmap(old_page);
	page_cache_release(old_page);
out:
=======
	inode_set_ctime_current(old_inode);
	mark_inode_dirty(old_inode);

	err = ext2_delete_entry(old_de, old_folio);
	if (!err && old_is_dir) {
		if (old_dir != new_dir)
			err = ext2_set_link(old_inode, dir_de, dir_folio,
					    new_dir, false);

		inode_dec_link_count(old_dir);
	}
out_dir:
	if (dir_de)
		folio_release_kmap(dir_folio, dir_de);
out_old:
	folio_release_kmap(old_folio, old_de);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

const struct inode_operations ext2_dir_inode_operations = {
	.create		= ext2_create,
	.lookup		= ext2_lookup,
	.link		= ext2_link,
	.unlink		= ext2_unlink,
	.symlink	= ext2_symlink,
	.mkdir		= ext2_mkdir,
	.rmdir		= ext2_rmdir,
	.mknod		= ext2_mknod,
	.rename		= ext2_rename,
<<<<<<< HEAD
#ifdef CONFIG_EXT2_FS_XATTR
	.setxattr	= generic_setxattr,
	.getxattr	= generic_getxattr,
	.listxattr	= ext2_listxattr,
	.removexattr	= generic_removexattr,
#endif
	.setattr	= ext2_setattr,
	.get_acl	= ext2_get_acl,
	.tmpfile	= ext2_tmpfile,
};

const struct inode_operations ext2_special_inode_operations = {
#ifdef CONFIG_EXT2_FS_XATTR
	.setxattr	= generic_setxattr,
	.getxattr	= generic_getxattr,
	.listxattr	= ext2_listxattr,
	.removexattr	= generic_removexattr,
#endif
	.setattr	= ext2_setattr,
	.get_acl	= ext2_get_acl,
=======
	.listxattr	= ext2_listxattr,
	.getattr	= ext2_getattr,
	.setattr	= ext2_setattr,
	.get_inode_acl	= ext2_get_acl,
	.set_acl	= ext2_set_acl,
	.tmpfile	= ext2_tmpfile,
	.fileattr_get	= ext2_fileattr_get,
	.fileattr_set	= ext2_fileattr_set,
};

const struct inode_operations ext2_special_inode_operations = {
	.listxattr	= ext2_listxattr,
	.getattr	= ext2_getattr,
	.setattr	= ext2_setattr,
	.get_inode_acl	= ext2_get_acl,
	.set_acl	= ext2_set_acl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
