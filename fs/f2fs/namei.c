<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * fs/f2fs/namei.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *             http://www.samsung.com/
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/fs.h>
#include <linux/f2fs_fs.h>
#include <linux/pagemap.h>
#include <linux/sched.h>
#include <linux/ctype.h>
<<<<<<< HEAD
#include <linux/dcache.h>
#include <linux/namei.h>

#include "f2fs.h"
#include "node.h"
=======
#include <linux/random.h>
#include <linux/dcache.h>
#include <linux/namei.h>
#include <linux/quotaops.h>

#include "f2fs.h"
#include "node.h"
#include "segment.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "xattr.h"
#include "acl.h"
#include <trace/events/f2fs.h>

<<<<<<< HEAD
static struct inode *f2fs_new_inode(struct inode *dir, umode_t mode)
=======
static inline bool is_extension_exist(const unsigned char *s, const char *sub,
						bool tmp_ext, bool tmp_dot)
{
	size_t slen = strlen(s);
	size_t sublen = strlen(sub);
	int i;

	if (sublen == 1 && *sub == '*')
		return true;

	/*
	 * filename format of multimedia file should be defined as:
	 * "filename + '.' + extension + (optional: '.' + temp extension)".
	 */
	if (slen < sublen + 2)
		return false;

	if (!tmp_ext) {
		/* file has no temp extension */
		if (s[slen - sublen - 1] != '.')
			return false;
		return !strncasecmp(s + slen - sublen, sub, sublen);
	}

	for (i = 1; i < slen - sublen; i++) {
		if (s[i] != '.')
			continue;
		if (!strncasecmp(s + i + 1, sub, sublen)) {
			if (!tmp_dot)
				return true;
			if (i == slen - sublen - 1 || s[i + 1 + sublen] == '.')
				return true;
		}
	}

	return false;
}

static inline bool is_temperature_extension(const unsigned char *s, const char *sub)
{
	return is_extension_exist(s, sub, true, false);
}

static inline bool is_compress_extension(const unsigned char *s, const char *sub)
{
	return is_extension_exist(s, sub, true, true);
}

int f2fs_update_extension_list(struct f2fs_sb_info *sbi, const char *name,
							bool hot, bool set)
{
	__u8 (*extlist)[F2FS_EXTENSION_LEN] = sbi->raw_super->extension_list;
	int cold_count = le32_to_cpu(sbi->raw_super->extension_count);
	int hot_count = sbi->raw_super->hot_ext_count;
	int total_count = cold_count + hot_count;
	int start, count;
	int i;

	if (set) {
		if (total_count == F2FS_MAX_EXTENSION)
			return -EINVAL;
	} else {
		if (!hot && !cold_count)
			return -EINVAL;
		if (hot && !hot_count)
			return -EINVAL;
	}

	if (hot) {
		start = cold_count;
		count = total_count;
	} else {
		start = 0;
		count = cold_count;
	}

	for (i = start; i < count; i++) {
		if (strcmp(name, extlist[i]))
			continue;

		if (set)
			return -EINVAL;

		memcpy(extlist[i], extlist[i + 1],
				F2FS_EXTENSION_LEN * (total_count - i - 1));
		memset(extlist[total_count - 1], 0, F2FS_EXTENSION_LEN);
		if (hot)
			sbi->raw_super->hot_ext_count = hot_count - 1;
		else
			sbi->raw_super->extension_count =
						cpu_to_le32(cold_count - 1);
		return 0;
	}

	if (!set)
		return -EINVAL;

	if (hot) {
		memcpy(extlist[count], name, strlen(name));
		sbi->raw_super->hot_ext_count = hot_count + 1;
	} else {
		char buf[F2FS_MAX_EXTENSION][F2FS_EXTENSION_LEN];

		memcpy(buf, &extlist[cold_count],
				F2FS_EXTENSION_LEN * hot_count);
		memset(extlist[cold_count], 0, F2FS_EXTENSION_LEN);
		memcpy(extlist[cold_count], name, strlen(name));
		memcpy(&extlist[cold_count + 1], buf,
				F2FS_EXTENSION_LEN * hot_count);
		sbi->raw_super->extension_count = cpu_to_le32(cold_count + 1);
	}
	return 0;
}

static void set_compress_new_inode(struct f2fs_sb_info *sbi, struct inode *dir,
				struct inode *inode, const unsigned char *name)
{
	__u8 (*extlist)[F2FS_EXTENSION_LEN] = sbi->raw_super->extension_list;
	unsigned char (*noext)[F2FS_EXTENSION_LEN] =
						F2FS_OPTION(sbi).noextensions;
	unsigned char (*ext)[F2FS_EXTENSION_LEN] = F2FS_OPTION(sbi).extensions;
	unsigned char ext_cnt = F2FS_OPTION(sbi).compress_ext_cnt;
	unsigned char noext_cnt = F2FS_OPTION(sbi).nocompress_ext_cnt;
	int i, cold_count, hot_count;

	if (!f2fs_sb_has_compression(sbi))
		return;

	if (S_ISDIR(inode->i_mode))
		goto inherit_comp;

	/* This name comes only from normal files. */
	if (!name)
		return;

	/* Don't compress hot files. */
	f2fs_down_read(&sbi->sb_lock);
	cold_count = le32_to_cpu(sbi->raw_super->extension_count);
	hot_count = sbi->raw_super->hot_ext_count;
	for (i = cold_count; i < cold_count + hot_count; i++)
		if (is_temperature_extension(name, extlist[i]))
			break;
	f2fs_up_read(&sbi->sb_lock);
	if (i < (cold_count + hot_count))
		return;

	/* Don't compress unallowed extension. */
	for (i = 0; i < noext_cnt; i++)
		if (is_compress_extension(name, noext[i]))
			return;

	/* Compress wanting extension. */
	for (i = 0; i < ext_cnt; i++) {
		if (is_compress_extension(name, ext[i])) {
			set_compress_context(inode);
			return;
		}
	}
inherit_comp:
	/* Inherit the {no-}compression flag in directory */
	if (F2FS_I(dir)->i_flags & F2FS_NOCOMP_FL) {
		F2FS_I(inode)->i_flags |= F2FS_NOCOMP_FL;
		f2fs_mark_inode_dirty_sync(inode, true);
	} else if (F2FS_I(dir)->i_flags & F2FS_COMPR_FL) {
		set_compress_context(inode);
	}
}

/*
 * Set file's temperature for hot/cold data separation
 */
static void set_file_temperature(struct f2fs_sb_info *sbi, struct inode *inode,
		const unsigned char *name)
{
	__u8 (*extlist)[F2FS_EXTENSION_LEN] = sbi->raw_super->extension_list;
	int i, cold_count, hot_count;

	f2fs_down_read(&sbi->sb_lock);
	cold_count = le32_to_cpu(sbi->raw_super->extension_count);
	hot_count = sbi->raw_super->hot_ext_count;
	for (i = 0; i < cold_count + hot_count; i++)
		if (is_temperature_extension(name, extlist[i]))
			break;
	f2fs_up_read(&sbi->sb_lock);

	if (i == cold_count + hot_count)
		return;

	if (i < cold_count)
		file_set_cold(inode);
	else
		file_set_hot(inode);
}

static struct inode *f2fs_new_inode(struct mnt_idmap *idmap,
						struct inode *dir, umode_t mode,
						const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	nid_t ino;
	struct inode *inode;
	bool nid_free = false;
<<<<<<< HEAD
=======
	bool encrypt = false;
	int xattr_size = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	inode = new_inode(dir->i_sb);
	if (!inode)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	f2fs_lock_op(sbi);
	if (!alloc_nid(sbi, &ino)) {
		f2fs_unlock_op(sbi);
		err = -ENOSPC;
		goto fail;
	}
	f2fs_unlock_op(sbi);

	inode_init_owner(inode, dir, mode);

	inode->i_ino = ino;
	inode->i_blocks = 0;
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
	inode->i_generation = sbi->s_next_generation++;
=======
	if (!f2fs_alloc_nid(sbi, &ino)) {
		err = -ENOSPC;
		goto fail;
	}

	nid_free = true;

	inode_init_owner(idmap, inode, dir, mode);

	inode->i_ino = ino;
	inode->i_blocks = 0;
	simple_inode_init_ts(inode);
	F2FS_I(inode)->i_crtime = inode_get_mtime(inode);
	inode->i_generation = get_random_u32();

	if (S_ISDIR(inode->i_mode))
		F2FS_I(inode)->i_current_depth = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = insert_inode_locked(inode);
	if (err) {
		err = -EINVAL;
<<<<<<< HEAD
		nid_free = true;
		goto fail;
	}

	/* If the directory encrypted, then we should encrypt the inode. */
	if (f2fs_encrypted_inode(dir) && f2fs_may_encrypt(inode))
		f2fs_set_encrypted_inode(inode);

	if (test_opt(sbi, INLINE_DATA) && f2fs_may_inline_data(inode))
		set_inode_flag(F2FS_I(inode), FI_INLINE_DATA);
	if (f2fs_may_inline_dentry(inode))
		set_inode_flag(F2FS_I(inode), FI_INLINE_DENTRY);

	f2fs_init_extent_tree(inode, NULL);
=======
		goto fail;
	}

	if (f2fs_sb_has_project_quota(sbi) &&
		(F2FS_I(dir)->i_flags & F2FS_PROJINHERIT_FL))
		F2FS_I(inode)->i_projid = F2FS_I(dir)->i_projid;
	else
		F2FS_I(inode)->i_projid = make_kprojid(&init_user_ns,
							F2FS_DEF_PROJID);

	err = fscrypt_prepare_new_inode(dir, inode, &encrypt);
	if (err)
		goto fail_drop;

	err = f2fs_dquot_initialize(inode);
	if (err)
		goto fail_drop;

	set_inode_flag(inode, FI_NEW_INODE);

	if (encrypt)
		f2fs_set_encrypted_inode(inode);

	if (f2fs_sb_has_extra_attr(sbi)) {
		set_inode_flag(inode, FI_EXTRA_ATTR);
		F2FS_I(inode)->i_extra_isize = F2FS_TOTAL_EXTRA_ATTR_SIZE;
	}

	if (test_opt(sbi, INLINE_XATTR))
		set_inode_flag(inode, FI_INLINE_XATTR);

	if (f2fs_may_inline_dentry(inode))
		set_inode_flag(inode, FI_INLINE_DENTRY);

	if (f2fs_sb_has_flexible_inline_xattr(sbi)) {
		f2fs_bug_on(sbi, !f2fs_has_extra_attr(inode));
		if (f2fs_has_inline_xattr(inode))
			xattr_size = F2FS_OPTION(sbi).inline_xattr_size;
		/* Otherwise, will be 0 */
	} else if (f2fs_has_inline_xattr(inode) ||
				f2fs_has_inline_dentry(inode)) {
		xattr_size = DEFAULT_INLINE_XATTR_ADDRS;
	}
	F2FS_I(inode)->i_inline_xattr_size = xattr_size;

	F2FS_I(inode)->i_flags =
		f2fs_mask_flags(mode, F2FS_I(dir)->i_flags & F2FS_FL_INHERITED);

	if (S_ISDIR(inode->i_mode))
		F2FS_I(inode)->i_flags |= F2FS_INDEX_FL;

	if (F2FS_I(inode)->i_flags & F2FS_PROJINHERIT_FL)
		set_inode_flag(inode, FI_PROJ_INHERIT);

	/* Check compression first. */
	set_compress_new_inode(sbi, dir, inode, name);

	/* Should enable inline_data after compression set */
	if (test_opt(sbi, INLINE_DATA) && f2fs_may_inline_data(inode))
		set_inode_flag(inode, FI_INLINE_DATA);

	if (name && !test_opt(sbi, DISABLE_EXT_IDENTIFY))
		set_file_temperature(sbi, inode, name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	stat_inc_inline_xattr(inode);
	stat_inc_inline_inode(inode);
	stat_inc_inline_dir(inode);

<<<<<<< HEAD
	trace_f2fs_new_inode(inode, 0);
	mark_inode_dirty(inode);
=======
	f2fs_set_inode_flags(inode);

	f2fs_init_extent_tree(inode);

	trace_f2fs_new_inode(inode, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return inode;

fail:
	trace_f2fs_new_inode(inode, err);
	make_bad_inode(inode);
	if (nid_free)
<<<<<<< HEAD
		set_inode_flag(F2FS_I(inode), FI_FREE_NID);
=======
		set_inode_flag(inode, FI_FREE_NID);
	iput(inode);
	return ERR_PTR(err);
fail_drop:
	trace_f2fs_new_inode(inode, err);
	dquot_drop(inode);
	inode->i_flags |= S_NOQUOTA;
	if (nid_free)
		set_inode_flag(inode, FI_FREE_NID);
	clear_nlink(inode);
	unlock_new_inode(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iput(inode);
	return ERR_PTR(err);
}

<<<<<<< HEAD
static int is_multimedia_file(const unsigned char *s, const char *sub)
{
	size_t slen = strlen(s);
	size_t sublen = strlen(sub);

	/*
	 * filename format of multimedia file should be defined as:
	 * "filename + '.' + extension".
	 */
	if (slen < sublen + 2)
		return 0;

	if (s[slen - sublen - 1] != '.')
		return 0;

	return !strncasecmp(s + slen - sublen, sub, sublen);
}

/*
 * Set multimedia files as cold files for hot/cold data separation
 */
static inline void set_cold_files(struct f2fs_sb_info *sbi, struct inode *inode,
		const unsigned char *name)
{
	int i;
	__u8 (*extlist)[8] = sbi->raw_super->extension_list;

	int count = le32_to_cpu(sbi->raw_super->extension_count);
	for (i = 0; i < count; i++) {
		if (is_multimedia_file(name, extlist[i])) {
			file_set_cold(inode);
			break;
		}
	}
}

static int f2fs_create(struct inode *dir, struct dentry *dentry, umode_t mode,
		       bool excl)
=======
static int f2fs_create(struct mnt_idmap *idmap, struct inode *dir,
		       struct dentry *dentry, umode_t mode, bool excl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	struct inode *inode;
	nid_t ino = 0;
	int err;

<<<<<<< HEAD
	inode = f2fs_new_inode(dir, mode);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

	if (!test_opt(sbi, DISABLE_EXT_IDENTIFY))
		set_cold_files(sbi, inode, dentry->d_name.name);

=======
	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;
	if (!f2fs_is_checkpoint_ready(sbi))
		return -ENOSPC;

	err = f2fs_dquot_initialize(dir);
	if (err)
		return err;

	inode = f2fs_new_inode(idmap, dir, mode, dentry->d_name.name);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inode->i_op = &f2fs_file_inode_operations;
	inode->i_fop = &f2fs_file_operations;
	inode->i_mapping->a_ops = &f2fs_dblock_aops;
	ino = inode->i_ino;

<<<<<<< HEAD
	f2fs_balance_fs(sbi, true);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_lock_op(sbi);
	err = f2fs_add_link(dentry, inode);
	if (err)
		goto out;
	f2fs_unlock_op(sbi);

<<<<<<< HEAD
	alloc_nid_done(sbi, ino);

	d_instantiate(dentry, inode);
	unlock_new_inode(inode);

	if (IS_DIRSYNC(dir))
		f2fs_sync_fs(sbi->sb, 1);
	return 0;
out:
	handle_failed_inode(inode);
=======
	f2fs_alloc_nid_done(sbi, ino);

	d_instantiate_new(dentry, inode);

	if (IS_DIRSYNC(dir))
		f2fs_sync_fs(sbi->sb, 1);

	f2fs_balance_fs(sbi, true);
	return 0;
out:
	f2fs_handle_failed_inode(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int f2fs_link(struct dentry *old_dentry, struct inode *dir,
		struct dentry *dentry)
{
<<<<<<< HEAD
	struct inode *inode = old_dentry->d_inode;
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	int err;

	if (f2fs_encrypted_inode(dir) &&
			!fscrypt_has_permitted_context(dir, inode))
		return -EPERM;

	f2fs_balance_fs(sbi, true);

	inode->i_ctime = CURRENT_TIME;
	ihold(inode);

	set_inode_flag(F2FS_I(inode), FI_INC_LINK);
=======
	struct inode *inode = d_inode(old_dentry);
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	int err;

	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;
	if (!f2fs_is_checkpoint_ready(sbi))
		return -ENOSPC;

	err = fscrypt_prepare_link(old_dentry, dir, dentry);
	if (err)
		return err;

	if (is_inode_flag_set(dir, FI_PROJ_INHERIT) &&
			(!projid_eq(F2FS_I(dir)->i_projid,
			F2FS_I(old_dentry->d_inode)->i_projid)))
		return -EXDEV;

	err = f2fs_dquot_initialize(dir);
	if (err)
		return err;

	f2fs_balance_fs(sbi, true);

	inode_set_ctime_current(inode);
	ihold(inode);

	set_inode_flag(inode, FI_INC_LINK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_lock_op(sbi);
	err = f2fs_add_link(dentry, inode);
	if (err)
		goto out;
	f2fs_unlock_op(sbi);

	d_instantiate(dentry, inode);

	if (IS_DIRSYNC(dir))
		f2fs_sync_fs(sbi->sb, 1);
	return 0;
out:
<<<<<<< HEAD
	clear_inode_flag(F2FS_I(inode), FI_INC_LINK);
=======
	clear_inode_flag(inode, FI_INC_LINK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iput(inode);
	f2fs_unlock_op(sbi);
	return err;
}

struct dentry *f2fs_get_parent(struct dentry *child)
{
<<<<<<< HEAD
	struct qstr dotdot = {.len = 2, .name = ".."};
	unsigned long ino = f2fs_inode_by_name(child->d_inode, &dotdot);
	if (!ino)
		return ERR_PTR(-ENOENT);
	return d_obtain_alias(f2fs_iget(child->d_inode->i_sb, ino));
=======
	struct page *page;
	unsigned long ino = f2fs_inode_by_name(d_inode(child), &dotdot_name, &page);

	if (!ino) {
		if (IS_ERR(page))
			return ERR_CAST(page);
		return ERR_PTR(-ENOENT);
	}
	return d_obtain_alias(f2fs_iget(child->d_sb, ino));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __recover_dot_dentries(struct inode *dir, nid_t pino)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
<<<<<<< HEAD
	struct qstr dot = {.len = 1, .name = "."};
	struct qstr dotdot = {.len = 2, .name = ".."};
=======
	struct qstr dot = QSTR_INIT(".", 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct f2fs_dir_entry *de;
	struct page *page;
	int err = 0;

	if (f2fs_readonly(sbi->sb)) {
<<<<<<< HEAD
		f2fs_msg(sbi->sb, KERN_INFO,
			"skip recovering inline_dots inode (ino:%lu, pino:%u) "
			"in readonly mountpoint", dir->i_ino, pino);
		return 0;
	}

=======
		f2fs_info(sbi, "skip recovering inline_dots inode (ino:%lu, pino:%u) in readonly mountpoint",
			  dir->i_ino, pino);
		return 0;
	}

	if (!S_ISDIR(dir->i_mode)) {
		f2fs_err(sbi, "inconsistent inode status, skip recovering inline_dots inode (ino:%lu, i_mode:%u, pino:%u)",
			  dir->i_ino, dir->i_mode, pino);
		set_sbi_flag(sbi, SBI_NEED_FSCK);
		return -ENOTDIR;
	}

	err = f2fs_dquot_initialize(dir);
	if (err)
		return err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_balance_fs(sbi, true);

	f2fs_lock_op(sbi);

	de = f2fs_find_entry(dir, &dot, &page);
	if (de) {
<<<<<<< HEAD
		f2fs_dentry_kunmap(dir, page);
		f2fs_put_page(page, 0);
	} else {
		err = __f2fs_add_link(dir, &dot, NULL, dir->i_ino, S_IFDIR);
=======
		f2fs_put_page(page, 0);
	} else if (IS_ERR(page)) {
		err = PTR_ERR(page);
		goto out;
	} else {
		err = f2fs_do_add_link(dir, &dot, NULL, dir->i_ino, S_IFDIR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			goto out;
	}

<<<<<<< HEAD
	de = f2fs_find_entry(dir, &dotdot, &page);
	if (de) {
		f2fs_dentry_kunmap(dir, page);
		f2fs_put_page(page, 0);
	} else {
		err = __f2fs_add_link(dir, &dotdot, NULL, pino, S_IFDIR);
	}
out:
	if (!err) {
		clear_inode_flag(F2FS_I(dir), FI_INLINE_DOTS);
		mark_inode_dirty(dir);
	}
=======
	de = f2fs_find_entry(dir, &dotdot_name, &page);
	if (de)
		f2fs_put_page(page, 0);
	else if (IS_ERR(page))
		err = PTR_ERR(page);
	else
		err = f2fs_do_add_link(dir, &dotdot_name, NULL, pino, S_IFDIR);
out:
	if (!err)
		clear_inode_flag(dir, FI_INLINE_DOTS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	f2fs_unlock_op(sbi);
	return err;
}

static struct dentry *f2fs_lookup(struct inode *dir, struct dentry *dentry,
<<<<<<< HEAD
					unsigned int flags)
=======
		unsigned int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode = NULL;
	struct f2fs_dir_entry *de;
	struct page *page;
<<<<<<< HEAD
	nid_t ino;
	int err = 0;
	unsigned int root_ino = F2FS_ROOT_INO(F2FS_I_SB(dir));

	if (f2fs_encrypted_inode(dir)) {
		int res = fscrypt_get_encryption_info(dir);

		/*
		 * DCACHE_ENCRYPTED_WITH_KEY is set if the dentry is
		 * created while the directory was encrypted and we
		 * don't have access to the key.
		 */
		if (fscrypt_has_encryption_key(dir))
			fscrypt_set_encrypted_dentry(dentry);
		fscrypt_set_d_op(dentry);
		if (res && res != -ENOKEY)
			return ERR_PTR(res);
	}

	if (dentry->d_name.len > F2FS_NAME_LEN)
		return ERR_PTR(-ENAMETOOLONG);

	de = f2fs_find_entry(dir, &dentry->d_name, &page);
	if (!de)
		return d_splice_alias(inode, dentry);

	ino = le32_to_cpu(de->ino);
	f2fs_dentry_kunmap(dir, page);
	f2fs_put_page(page, 0);

	inode = f2fs_iget(dir->i_sb, ino);
	if (IS_ERR(inode))
		return ERR_CAST(inode);
=======
	struct dentry *new;
	nid_t ino = -1;
	int err = 0;
	unsigned int root_ino = F2FS_ROOT_INO(F2FS_I_SB(dir));
	struct f2fs_filename fname;

	trace_f2fs_lookup_start(dir, dentry, flags);

	if (dentry->d_name.len > F2FS_NAME_LEN) {
		err = -ENAMETOOLONG;
		goto out;
	}

	err = f2fs_prepare_lookup(dir, dentry, &fname);
	if (err == -ENOENT)
		goto out_splice;
	if (err)
		goto out;
	de = __f2fs_find_entry(dir, &fname, &page);
	f2fs_free_filename(&fname);

	if (!de) {
		if (IS_ERR(page)) {
			err = PTR_ERR(page);
			goto out;
		}
		err = -ENOENT;
		goto out_splice;
	}

	ino = le32_to_cpu(de->ino);
	f2fs_put_page(page, 0);

	inode = f2fs_iget(dir->i_sb, ino);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((dir->i_ino == root_ino) && f2fs_has_inline_dots(dir)) {
		err = __recover_dot_dentries(dir, root_ino);
		if (err)
<<<<<<< HEAD
			goto err_out;
=======
			goto out_iput;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (f2fs_has_inline_dots(inode)) {
		err = __recover_dot_dentries(inode, dir->i_ino);
		if (err)
<<<<<<< HEAD
			goto err_out;
	}
	if (!IS_ERR(inode) && f2fs_encrypted_inode(dir) &&
			(S_ISDIR(inode->i_mode) || S_ISLNK(inode->i_mode)) &&
			!fscrypt_has_permitted_context(dir, inode)) {
		bool nokey = f2fs_encrypted_inode(inode) &&
			!fscrypt_has_encryption_key(inode);
		err = nokey ? -ENOKEY : -EPERM;
		goto err_out;
	}
	return d_splice_alias(inode, dentry);

err_out:
	iput(inode);
=======
			goto out_iput;
	}
	if (IS_ENCRYPTED(dir) &&
	    (S_ISDIR(inode->i_mode) || S_ISLNK(inode->i_mode)) &&
	    !fscrypt_has_permitted_context(dir, inode)) {
		f2fs_warn(F2FS_I_SB(inode), "Inconsistent encryption contexts: %lu/%lu",
			  dir->i_ino, inode->i_ino);
		err = -EPERM;
		goto out_iput;
	}
out_splice:
#if IS_ENABLED(CONFIG_UNICODE)
	if (!inode && IS_CASEFOLDED(dir)) {
		/* Eventually we want to call d_add_ci(dentry, NULL)
		 * for negative dentries in the encoding case as
		 * well.  For now, prevent the negative dentry
		 * from being cached.
		 */
		trace_f2fs_lookup_end(dir, dentry, ino, err);
		return NULL;
	}
#endif
	new = d_splice_alias(inode, dentry);
	trace_f2fs_lookup_end(dir, !IS_ERR_OR_NULL(new) ? new : dentry,
				ino, IS_ERR(new) ? PTR_ERR(new) : err);
	return new;
out_iput:
	iput(inode);
out:
	trace_f2fs_lookup_end(dir, dentry, ino, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ERR_PTR(err);
}

static int f2fs_unlink(struct inode *dir, struct dentry *dentry)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
<<<<<<< HEAD
	struct inode *inode = dentry->d_inode;
	struct f2fs_dir_entry *de;
	struct page *page;
	int err = -ENOENT;

	trace_f2fs_unlink_enter(dir, dentry);

	de = f2fs_find_entry(dir, &dentry->d_name, &page);
	if (!de)
		goto fail;
=======
	struct inode *inode = d_inode(dentry);
	struct f2fs_dir_entry *de;
	struct page *page;
	int err;

	trace_f2fs_unlink_enter(dir, dentry);

	if (unlikely(f2fs_cp_error(sbi))) {
		err = -EIO;
		goto fail;
	}

	err = f2fs_dquot_initialize(dir);
	if (err)
		goto fail;
	err = f2fs_dquot_initialize(inode);
	if (err)
		goto fail;

	de = f2fs_find_entry(dir, &dentry->d_name, &page);
	if (!de) {
		if (IS_ERR(page))
			err = PTR_ERR(page);
		goto fail;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	f2fs_balance_fs(sbi, true);

	f2fs_lock_op(sbi);
<<<<<<< HEAD
	err = acquire_orphan_inode(sbi);
	if (err) {
		f2fs_unlock_op(sbi);
		f2fs_dentry_kunmap(dir, page);
=======
	err = f2fs_acquire_orphan_inode(sbi);
	if (err) {
		f2fs_unlock_op(sbi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		f2fs_put_page(page, 0);
		goto fail;
	}
	f2fs_delete_entry(de, page, dir, inode);
	f2fs_unlock_op(sbi);

<<<<<<< HEAD
	/* In order to evict this inode, we set it dirty */
	mark_inode_dirty(inode);

=======
#if IS_ENABLED(CONFIG_UNICODE)
	/* VFS negative dentries are incompatible with Encoding and
	 * Case-insensitiveness. Eventually we'll want avoid
	 * invalidating the dentries here, alongside with returning the
	 * negative dentries at f2fs_lookup(), when it is better
	 * supported by the VFS for the CI case.
	 */
	if (IS_CASEFOLDED(dir))
		d_invalidate(dentry);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_DIRSYNC(dir))
		f2fs_sync_fs(sbi->sb, 1);
fail:
	trace_f2fs_unlink_exit(inode, err);
	return err;
}

<<<<<<< HEAD
static void *f2fs_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	struct page *page;
	char *link;

	page = page_follow_link_light(dentry, nd);
	if (IS_ERR(page))
		return page;

	link = nd_get_link(nd);
	if (IS_ERR(link))
		return link;

	/* this is broken symlink case */
	if (*link == 0) {
		kunmap(page);
		page_cache_release(page);
		return ERR_PTR(-ENOENT);
	}
	return page;
}

static int f2fs_symlink(struct inode *dir, struct dentry *dentry,
					const char *symname)
=======
static const char *f2fs_get_link(struct dentry *dentry,
				 struct inode *inode,
				 struct delayed_call *done)
{
	const char *link = page_get_link(dentry, inode, done);

	if (!IS_ERR(link) && !*link) {
		/* this is broken symlink case */
		do_delayed_call(done);
		clear_delayed_call(done);
		link = ERR_PTR(-ENOENT);
	}
	return link;
}

static int f2fs_symlink(struct mnt_idmap *idmap, struct inode *dir,
			struct dentry *dentry, const char *symname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	struct inode *inode;
	size_t len = strlen(symname);
<<<<<<< HEAD
	struct fscrypt_str disk_link = FSTR_INIT((char *)symname, len + 1);
	struct fscrypt_symlink_data *sd = NULL;
	int err;

	if (f2fs_encrypted_inode(dir)) {
		err = fscrypt_get_encryption_info(dir);
		if (err)
			return err;

		if (!fscrypt_has_encryption_key(dir))
			return -EPERM;

		disk_link.len = (fscrypt_fname_encrypted_size(dir, len) +
				sizeof(struct fscrypt_symlink_data));
	}

	if (disk_link.len > dir->i_sb->s_blocksize)
		return -ENAMETOOLONG;

	inode = f2fs_new_inode(dir, S_IFLNK | S_IRWXUGO);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

	if (f2fs_encrypted_inode(inode))
		inode->i_op = &f2fs_encrypted_symlink_inode_operations;
	else
		inode->i_op = &f2fs_symlink_inode_operations;
	inode->i_mapping->a_ops = &f2fs_dblock_aops;

	f2fs_balance_fs(sbi, true);

	f2fs_lock_op(sbi);
	err = f2fs_add_link(dentry, inode);
	if (err)
		goto out;
	f2fs_unlock_op(sbi);
	alloc_nid_done(sbi, inode->i_ino);

	if (f2fs_encrypted_inode(inode)) {
		struct qstr istr = QSTR_INIT(symname, len);
		struct fscrypt_str ostr;

		sd = kzalloc(disk_link.len, GFP_NOFS);
		if (!sd) {
			err = -ENOMEM;
			goto err_out;
		}

		err = fscrypt_get_encryption_info(inode);
		if (err)
			goto err_out;

		if (!fscrypt_has_encryption_key(inode)) {
			err = -EPERM;
			goto err_out;
		}

		ostr.name = sd->encrypted_path;
		ostr.len = disk_link.len;
		err = fscrypt_fname_usr_to_disk(inode, &istr, &ostr);
		if (err < 0)
			goto err_out;

		sd->len = cpu_to_le16(ostr.len);
		disk_link.name = (char *)sd;
	}
=======
	struct fscrypt_str disk_link;
	int err;

	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;
	if (!f2fs_is_checkpoint_ready(sbi))
		return -ENOSPC;

	err = fscrypt_prepare_symlink(dir, symname, len, dir->i_sb->s_blocksize,
				      &disk_link);
	if (err)
		return err;

	err = f2fs_dquot_initialize(dir);
	if (err)
		return err;

	inode = f2fs_new_inode(idmap, dir, S_IFLNK | S_IRWXUGO, NULL);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

	if (IS_ENCRYPTED(inode))
		inode->i_op = &f2fs_encrypted_symlink_inode_operations;
	else
		inode->i_op = &f2fs_symlink_inode_operations;
	inode_nohighmem(inode);
	inode->i_mapping->a_ops = &f2fs_dblock_aops;

	f2fs_lock_op(sbi);
	err = f2fs_add_link(dentry, inode);
	if (err)
		goto out_f2fs_handle_failed_inode;
	f2fs_unlock_op(sbi);
	f2fs_alloc_nid_done(sbi, inode->i_ino);

	err = fscrypt_encrypt_symlink(inode, symname, len, &disk_link);
	if (err)
		goto err_out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = page_symlink(inode, disk_link.name, disk_link.len);

err_out:
<<<<<<< HEAD
	d_instantiate(dentry, inode);
	unlock_new_inode(inode);
=======
	d_instantiate_new(dentry, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Let's flush symlink data in order to avoid broken symlink as much as
	 * possible. Nevertheless, fsyncing is the best way, but there is no
	 * way to get a file descriptor in order to flush that.
	 *
	 * Note that, it needs to do dir->fsync to make this recoverable.
	 * If the symlink path is stored into inline_data, there is no
	 * performance regression.
	 */
	if (!err) {
		filemap_write_and_wait_range(inode->i_mapping, 0,
							disk_link.len - 1);

		if (IS_DIRSYNC(dir))
			f2fs_sync_fs(sbi->sb, 1);
	} else {
		f2fs_unlink(dir, dentry);
	}

<<<<<<< HEAD
	kfree(sd);
	return err;
out:
	handle_failed_inode(inode);
	return err;
}

static int f2fs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
=======
	f2fs_balance_fs(sbi, true);
	goto out_free_encrypted_link;

out_f2fs_handle_failed_inode:
	f2fs_handle_failed_inode(inode);
out_free_encrypted_link:
	if (disk_link.name != (unsigned char *)symname)
		kfree(disk_link.name);
	return err;
}

static int f2fs_mkdir(struct mnt_idmap *idmap, struct inode *dir,
		      struct dentry *dentry, umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	struct inode *inode;
	int err;

<<<<<<< HEAD
	inode = f2fs_new_inode(dir, S_IFDIR | mode);
=======
	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;

	err = f2fs_dquot_initialize(dir);
	if (err)
		return err;

	inode = f2fs_new_inode(idmap, dir, S_IFDIR | mode, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(inode))
		return PTR_ERR(inode);

	inode->i_op = &f2fs_dir_inode_operations;
	inode->i_fop = &f2fs_dir_operations;
	inode->i_mapping->a_ops = &f2fs_dblock_aops;
<<<<<<< HEAD
	mapping_set_gfp_mask(inode->i_mapping, GFP_F2FS_HIGH_ZERO);

	f2fs_balance_fs(sbi, true);

	set_inode_flag(F2FS_I(inode), FI_INC_LINK);
=======
	mapping_set_gfp_mask(inode->i_mapping, GFP_NOFS);

	set_inode_flag(inode, FI_INC_LINK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_lock_op(sbi);
	err = f2fs_add_link(dentry, inode);
	if (err)
		goto out_fail;
	f2fs_unlock_op(sbi);

<<<<<<< HEAD
	alloc_nid_done(sbi, inode->i_ino);

	d_instantiate(dentry, inode);
	unlock_new_inode(inode);

	if (IS_DIRSYNC(dir))
		f2fs_sync_fs(sbi->sb, 1);
	return 0;

out_fail:
	clear_inode_flag(F2FS_I(inode), FI_INC_LINK);
	handle_failed_inode(inode);
=======
	f2fs_alloc_nid_done(sbi, inode->i_ino);

	d_instantiate_new(dentry, inode);

	if (IS_DIRSYNC(dir))
		f2fs_sync_fs(sbi->sb, 1);

	f2fs_balance_fs(sbi, true);
	return 0;

out_fail:
	clear_inode_flag(inode, FI_INC_LINK);
	f2fs_handle_failed_inode(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int f2fs_rmdir(struct inode *dir, struct dentry *dentry)
{
<<<<<<< HEAD
	struct inode *inode = dentry->d_inode;
=======
	struct inode *inode = d_inode(dentry);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (f2fs_empty_dir(inode))
		return f2fs_unlink(dir, dentry);
	return -ENOTEMPTY;
}

<<<<<<< HEAD
static int f2fs_mknod(struct inode *dir, struct dentry *dentry,
				umode_t mode, dev_t rdev)
=======
static int f2fs_mknod(struct mnt_idmap *idmap, struct inode *dir,
		      struct dentry *dentry, umode_t mode, dev_t rdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	struct inode *inode;
	int err = 0;

<<<<<<< HEAD
	if (!new_valid_dev(rdev))
		return -EINVAL;

	inode = f2fs_new_inode(dir, mode);
=======
	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;
	if (!f2fs_is_checkpoint_ready(sbi))
		return -ENOSPC;

	err = f2fs_dquot_initialize(dir);
	if (err)
		return err;

	inode = f2fs_new_inode(idmap, dir, mode, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(inode))
		return PTR_ERR(inode);

	init_special_inode(inode, inode->i_mode, rdev);
	inode->i_op = &f2fs_special_inode_operations;

<<<<<<< HEAD
	f2fs_balance_fs(sbi, true);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_lock_op(sbi);
	err = f2fs_add_link(dentry, inode);
	if (err)
		goto out;
	f2fs_unlock_op(sbi);

<<<<<<< HEAD
	alloc_nid_done(sbi, inode->i_ino);

	d_instantiate(dentry, inode);
	unlock_new_inode(inode);

	if (IS_DIRSYNC(dir))
		f2fs_sync_fs(sbi->sb, 1);
	return 0;
out:
	handle_failed_inode(inode);
	return err;
}

static int f2fs_rename(struct inode *old_dir, struct dentry *old_dentry,
			struct inode *new_dir, struct dentry *new_dentry)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(old_dir);
	struct inode *old_inode = old_dentry->d_inode;
	struct inode *new_inode = new_dentry->d_inode;
	struct page *old_dir_page;
	struct page *old_page, *new_page;
	struct f2fs_dir_entry *old_dir_entry = NULL;
	struct f2fs_dir_entry *old_entry;
	struct f2fs_dir_entry *new_entry;
	bool is_old_inline = f2fs_has_inline_dentry(old_dir);
	int err = -ENOENT;

	if ((old_dir != new_dir) && f2fs_encrypted_inode(new_dir) &&
			!fscrypt_has_permitted_context(new_dir, old_inode)) {
		err = -EPERM;
		goto out;
	}

	old_entry = f2fs_find_entry(old_dir, &old_dentry->d_name, &old_page);
	if (!old_entry)
		goto out;

	if (S_ISDIR(old_inode->i_mode)) {
		err = -EIO;
		old_dir_entry = f2fs_parent_dir(old_inode, &old_dir_page);
		if (!old_dir_entry)
			goto out_old;
=======
	f2fs_alloc_nid_done(sbi, inode->i_ino);

	d_instantiate_new(dentry, inode);

	if (IS_DIRSYNC(dir))
		f2fs_sync_fs(sbi->sb, 1);

	f2fs_balance_fs(sbi, true);
	return 0;
out:
	f2fs_handle_failed_inode(inode);
	return err;
}

static int __f2fs_tmpfile(struct mnt_idmap *idmap, struct inode *dir,
			  struct file *file, umode_t mode, bool is_whiteout,
			  struct inode **new_inode, struct f2fs_filename *fname)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	struct inode *inode;
	int err;

	err = f2fs_dquot_initialize(dir);
	if (err)
		return err;

	inode = f2fs_new_inode(idmap, dir, mode, NULL);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

	if (is_whiteout) {
		init_special_inode(inode, inode->i_mode, WHITEOUT_DEV);
		inode->i_op = &f2fs_special_inode_operations;
	} else {
		inode->i_op = &f2fs_file_inode_operations;
		inode->i_fop = &f2fs_file_operations;
		inode->i_mapping->a_ops = &f2fs_dblock_aops;
	}

	f2fs_lock_op(sbi);
	err = f2fs_acquire_orphan_inode(sbi);
	if (err)
		goto out;

	err = f2fs_do_tmpfile(inode, dir, fname);
	if (err)
		goto release_out;

	/*
	 * add this non-linked tmpfile to orphan list, in this way we could
	 * remove all unused data of tmpfile after abnormal power-off.
	 */
	f2fs_add_orphan_inode(inode);
	f2fs_alloc_nid_done(sbi, inode->i_ino);

	if (is_whiteout) {
		f2fs_i_links_write(inode, false);

		spin_lock(&inode->i_lock);
		inode->i_state |= I_LINKABLE;
		spin_unlock(&inode->i_lock);
	} else {
		if (file)
			d_tmpfile(file, inode);
		else
			f2fs_i_links_write(inode, false);
	}
	/* link_count was changed by d_tmpfile as well. */
	f2fs_unlock_op(sbi);
	unlock_new_inode(inode);

	if (new_inode)
		*new_inode = inode;

	f2fs_balance_fs(sbi, true);
	return 0;

release_out:
	f2fs_release_orphan_inode(sbi);
out:
	f2fs_handle_failed_inode(inode);
	return err;
}

static int f2fs_tmpfile(struct mnt_idmap *idmap, struct inode *dir,
			struct file *file, umode_t mode)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	int err;

	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;
	if (!f2fs_is_checkpoint_ready(sbi))
		return -ENOSPC;

	err = __f2fs_tmpfile(idmap, dir, file, mode, false, NULL, NULL);

	return finish_open_simple(file, err);
}

static int f2fs_create_whiteout(struct mnt_idmap *idmap,
				struct inode *dir, struct inode **whiteout,
				struct f2fs_filename *fname)
{
	return __f2fs_tmpfile(idmap, dir, NULL, S_IFCHR | WHITEOUT_MODE,
						true, whiteout, fname);
}

int f2fs_get_tmpfile(struct mnt_idmap *idmap, struct inode *dir,
		     struct inode **new_inode)
{
	return __f2fs_tmpfile(idmap, dir, NULL, S_IFREG,
				false, new_inode, NULL);
}

static int f2fs_rename(struct mnt_idmap *idmap, struct inode *old_dir,
			struct dentry *old_dentry, struct inode *new_dir,
			struct dentry *new_dentry, unsigned int flags)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(old_dir);
	struct inode *old_inode = d_inode(old_dentry);
	struct inode *new_inode = d_inode(new_dentry);
	struct inode *whiteout = NULL;
	struct page *old_dir_page = NULL;
	struct page *old_page, *new_page = NULL;
	struct f2fs_dir_entry *old_dir_entry = NULL;
	struct f2fs_dir_entry *old_entry;
	struct f2fs_dir_entry *new_entry;
	bool old_is_dir = S_ISDIR(old_inode->i_mode);
	int err;

	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;
	if (!f2fs_is_checkpoint_ready(sbi))
		return -ENOSPC;

	if (is_inode_flag_set(new_dir, FI_PROJ_INHERIT) &&
			(!projid_eq(F2FS_I(new_dir)->i_projid,
			F2FS_I(old_dentry->d_inode)->i_projid)))
		return -EXDEV;

	/*
	 * If new_inode is null, the below renaming flow will
	 * add a link in old_dir which can convert inline_dir.
	 * After then, if we failed to get the entry due to other
	 * reasons like ENOMEM, we had to remove the new entry.
	 * Instead of adding such the error handling routine, let's
	 * simply convert first here.
	 */
	if (old_dir == new_dir && !new_inode) {
		err = f2fs_try_convert_inline_dir(old_dir, new_dentry);
		if (err)
			return err;
	}

	if (flags & RENAME_WHITEOUT) {
		struct f2fs_filename fname;

		err = f2fs_setup_filename(old_dir, &old_dentry->d_name,
							0, &fname);
		if (err)
			return err;

		err = f2fs_create_whiteout(idmap, old_dir, &whiteout, &fname);
		if (err)
			return err;
	}

	err = f2fs_dquot_initialize(old_dir);
	if (err)
		goto out;

	err = f2fs_dquot_initialize(new_dir);
	if (err)
		goto out;

	if (new_inode) {
		err = f2fs_dquot_initialize(new_inode);
		if (err)
			goto out;
	}

	err = -ENOENT;
	old_entry = f2fs_find_entry(old_dir, &old_dentry->d_name, &old_page);
	if (!old_entry) {
		if (IS_ERR(old_page))
			err = PTR_ERR(old_page);
		goto out;
	}

	if (old_is_dir && old_dir != new_dir) {
		old_dir_entry = f2fs_parent_dir(old_inode, &old_dir_page);
		if (!old_dir_entry) {
			if (IS_ERR(old_dir_page))
				err = PTR_ERR(old_dir_page);
			goto out_old;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (new_inode) {

		err = -ENOTEMPTY;
<<<<<<< HEAD
		if (old_dir_entry && !f2fs_empty_dir(new_inode))
=======
		if (old_is_dir && !f2fs_empty_dir(new_inode))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_dir;

		err = -ENOENT;
		new_entry = f2fs_find_entry(new_dir, &new_dentry->d_name,
						&new_page);
<<<<<<< HEAD
		if (!new_entry)
			goto out_dir;
=======
		if (!new_entry) {
			if (IS_ERR(new_page))
				err = PTR_ERR(new_page);
			goto out_dir;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		f2fs_balance_fs(sbi, true);

		f2fs_lock_op(sbi);

<<<<<<< HEAD
		err = acquire_orphan_inode(sbi);
		if (err)
			goto put_out_dir;

		err = update_dent_inode(old_inode, new_inode,
						&new_dentry->d_name);
		if (err) {
			release_orphan_inode(sbi);
			goto put_out_dir;
		}

		f2fs_set_link(new_dir, new_entry, new_page, old_inode);

		new_inode->i_ctime = CURRENT_TIME;
		down_write(&F2FS_I(new_inode)->i_sem);
		if (old_dir_entry)
			drop_nlink(new_inode);
		drop_nlink(new_inode);
		up_write(&F2FS_I(new_inode)->i_sem);

		mark_inode_dirty(new_inode);

		if (!new_inode->i_nlink)
			add_orphan_inode(sbi, new_inode->i_ino);
		else
			release_orphan_inode(sbi);

		update_inode_page(old_inode);
		update_inode_page(new_inode);
=======
		err = f2fs_acquire_orphan_inode(sbi);
		if (err)
			goto put_out_dir;

		f2fs_set_link(new_dir, new_entry, new_page, old_inode);
		new_page = NULL;

		inode_set_ctime_current(new_inode);
		f2fs_down_write(&F2FS_I(new_inode)->i_sem);
		if (old_is_dir)
			f2fs_i_links_write(new_inode, false);
		f2fs_i_links_write(new_inode, false);
		f2fs_up_write(&F2FS_I(new_inode)->i_sem);

		if (!new_inode->i_nlink)
			f2fs_add_orphan_inode(new_inode);
		else
			f2fs_release_orphan_inode(sbi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		f2fs_balance_fs(sbi, true);

		f2fs_lock_op(sbi);

		err = f2fs_add_link(new_dentry, old_inode);
		if (err) {
			f2fs_unlock_op(sbi);
			goto out_dir;
		}

<<<<<<< HEAD
		if (old_dir_entry) {
			inc_nlink(new_dir);
			update_inode_page(new_dir);
		}

		/*
		 * old entry and new entry can locate in the same inline
		 * dentry in inode, when attaching new entry in inline dentry,
		 * it could force inline dentry conversion, after that,
		 * old_entry and old_page will point to wrong address, in
		 * order to avoid this, let's do the check and update here.
		 */
		if (is_old_inline && !f2fs_has_inline_dentry(old_dir)) {
			f2fs_put_page(old_page, 0);
			old_page = NULL;

			old_entry = f2fs_find_entry(old_dir,
						&old_dentry->d_name, &old_page);
			if (!old_entry) {
				err = -EIO;
				f2fs_unlock_op(sbi);
				goto out_dir;
			}
		}
	}

	down_write(&F2FS_I(old_inode)->i_sem);
	file_lost_pino(old_inode);
	if (new_inode && file_enc_name(new_inode))
		file_set_enc_name(old_inode);
	up_write(&F2FS_I(old_inode)->i_sem);

	old_inode->i_ctime = CURRENT_TIME;
	mark_inode_dirty(old_inode);

	f2fs_delete_entry(old_entry, old_page, old_dir, NULL);

	if (old_dir_entry) {
		if (old_dir != new_dir) {
			f2fs_set_link(old_inode, old_dir_entry,
						old_dir_page, new_dir);
			update_inode_page(old_inode);
		} else {
			f2fs_dentry_kunmap(old_inode, old_dir_page);
			f2fs_put_page(old_dir_page, 0);
		}
		drop_nlink(old_dir);
		mark_inode_dirty(old_dir);
		update_inode_page(old_dir);
=======
		if (old_is_dir)
			f2fs_i_links_write(new_dir, true);
	}

	f2fs_down_write(&F2FS_I(old_inode)->i_sem);
	if (!old_is_dir || whiteout)
		file_lost_pino(old_inode);
	else
		/* adjust dir's i_pino to pass fsck check */
		f2fs_i_pino_write(old_inode, new_dir->i_ino);
	f2fs_up_write(&F2FS_I(old_inode)->i_sem);

	inode_set_ctime_current(old_inode);
	f2fs_mark_inode_dirty_sync(old_inode, false);

	f2fs_delete_entry(old_entry, old_page, old_dir, NULL);
	old_page = NULL;

	if (whiteout) {
		set_inode_flag(whiteout, FI_INC_LINK);
		err = f2fs_add_link(old_dentry, whiteout);
		if (err)
			goto put_out_dir;

		spin_lock(&whiteout->i_lock);
		whiteout->i_state &= ~I_LINKABLE;
		spin_unlock(&whiteout->i_lock);

		iput(whiteout);
	}

	if (old_dir_entry)
		f2fs_set_link(old_inode, old_dir_entry, old_dir_page, new_dir);
	if (old_is_dir)
		f2fs_i_links_write(old_dir, false);

	if (F2FS_OPTION(sbi).fsync_mode == FSYNC_MODE_STRICT) {
		f2fs_add_ino_entry(sbi, new_dir->i_ino, TRANS_DIR_INO);
		if (S_ISDIR(old_inode->i_mode))
			f2fs_add_ino_entry(sbi, old_inode->i_ino,
							TRANS_DIR_INO);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	f2fs_unlock_op(sbi);

	if (IS_DIRSYNC(old_dir) || IS_DIRSYNC(new_dir))
		f2fs_sync_fs(sbi->sb, 1);
<<<<<<< HEAD
=======

	f2fs_update_time(sbi, REQ_TIME);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

put_out_dir:
	f2fs_unlock_op(sbi);
<<<<<<< HEAD
	f2fs_dentry_kunmap(new_dir, new_page);
	f2fs_put_page(new_page, 0);
out_dir:
	if (old_dir_entry) {
		f2fs_dentry_kunmap(old_inode, old_dir_page);
		f2fs_put_page(old_dir_page, 0);
	}
out_old:
	f2fs_dentry_kunmap(old_dir, old_page);
=======
	f2fs_put_page(new_page, 0);
out_dir:
	if (old_dir_entry)
		f2fs_put_page(old_dir_page, 0);
out_old:
	f2fs_put_page(old_page, 0);
out:
	iput(whiteout);
	return err;
}

static int f2fs_cross_rename(struct inode *old_dir, struct dentry *old_dentry,
			     struct inode *new_dir, struct dentry *new_dentry)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(old_dir);
	struct inode *old_inode = d_inode(old_dentry);
	struct inode *new_inode = d_inode(new_dentry);
	struct page *old_dir_page, *new_dir_page;
	struct page *old_page, *new_page;
	struct f2fs_dir_entry *old_dir_entry = NULL, *new_dir_entry = NULL;
	struct f2fs_dir_entry *old_entry, *new_entry;
	int old_nlink = 0, new_nlink = 0;
	int err;

	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;
	if (!f2fs_is_checkpoint_ready(sbi))
		return -ENOSPC;

	if ((is_inode_flag_set(new_dir, FI_PROJ_INHERIT) &&
			!projid_eq(F2FS_I(new_dir)->i_projid,
			F2FS_I(old_dentry->d_inode)->i_projid)) ||
	    (is_inode_flag_set(new_dir, FI_PROJ_INHERIT) &&
			!projid_eq(F2FS_I(old_dir)->i_projid,
			F2FS_I(new_dentry->d_inode)->i_projid)))
		return -EXDEV;

	err = f2fs_dquot_initialize(old_dir);
	if (err)
		goto out;

	err = f2fs_dquot_initialize(new_dir);
	if (err)
		goto out;

	err = -ENOENT;
	old_entry = f2fs_find_entry(old_dir, &old_dentry->d_name, &old_page);
	if (!old_entry) {
		if (IS_ERR(old_page))
			err = PTR_ERR(old_page);
		goto out;
	}

	new_entry = f2fs_find_entry(new_dir, &new_dentry->d_name, &new_page);
	if (!new_entry) {
		if (IS_ERR(new_page))
			err = PTR_ERR(new_page);
		goto out_old;
	}

	/* prepare for updating ".." directory entry info later */
	if (old_dir != new_dir) {
		if (S_ISDIR(old_inode->i_mode)) {
			old_dir_entry = f2fs_parent_dir(old_inode,
							&old_dir_page);
			if (!old_dir_entry) {
				if (IS_ERR(old_dir_page))
					err = PTR_ERR(old_dir_page);
				goto out_new;
			}
		}

		if (S_ISDIR(new_inode->i_mode)) {
			new_dir_entry = f2fs_parent_dir(new_inode,
							&new_dir_page);
			if (!new_dir_entry) {
				if (IS_ERR(new_dir_page))
					err = PTR_ERR(new_dir_page);
				goto out_old_dir;
			}
		}
	}

	/*
	 * If cross rename between file and directory those are not
	 * in the same directory, we will inc nlink of file's parent
	 * later, so we should check upper boundary of its nlink.
	 */
	if ((!old_dir_entry || !new_dir_entry) &&
				old_dir_entry != new_dir_entry) {
		old_nlink = old_dir_entry ? -1 : 1;
		new_nlink = -old_nlink;
		err = -EMLINK;
		if ((old_nlink > 0 && old_dir->i_nlink >= F2FS_LINK_MAX) ||
			(new_nlink > 0 && new_dir->i_nlink >= F2FS_LINK_MAX))
			goto out_new_dir;
	}

	f2fs_balance_fs(sbi, true);

	f2fs_lock_op(sbi);

	/* update ".." directory entry info of old dentry */
	if (old_dir_entry)
		f2fs_set_link(old_inode, old_dir_entry, old_dir_page, new_dir);

	/* update ".." directory entry info of new dentry */
	if (new_dir_entry)
		f2fs_set_link(new_inode, new_dir_entry, new_dir_page, old_dir);

	/* update directory entry info of old dir inode */
	f2fs_set_link(old_dir, old_entry, old_page, new_inode);

	f2fs_down_write(&F2FS_I(old_inode)->i_sem);
	if (!old_dir_entry)
		file_lost_pino(old_inode);
	else
		/* adjust dir's i_pino to pass fsck check */
		f2fs_i_pino_write(old_inode, new_dir->i_ino);
	f2fs_up_write(&F2FS_I(old_inode)->i_sem);

	inode_set_ctime_current(old_dir);
	if (old_nlink) {
		f2fs_down_write(&F2FS_I(old_dir)->i_sem);
		f2fs_i_links_write(old_dir, old_nlink > 0);
		f2fs_up_write(&F2FS_I(old_dir)->i_sem);
	}
	f2fs_mark_inode_dirty_sync(old_dir, false);

	/* update directory entry info of new dir inode */
	f2fs_set_link(new_dir, new_entry, new_page, old_inode);

	f2fs_down_write(&F2FS_I(new_inode)->i_sem);
	if (!new_dir_entry)
		file_lost_pino(new_inode);
	else
		/* adjust dir's i_pino to pass fsck check */
		f2fs_i_pino_write(new_inode, old_dir->i_ino);
	f2fs_up_write(&F2FS_I(new_inode)->i_sem);

	inode_set_ctime_current(new_dir);
	if (new_nlink) {
		f2fs_down_write(&F2FS_I(new_dir)->i_sem);
		f2fs_i_links_write(new_dir, new_nlink > 0);
		f2fs_up_write(&F2FS_I(new_dir)->i_sem);
	}
	f2fs_mark_inode_dirty_sync(new_dir, false);

	if (F2FS_OPTION(sbi).fsync_mode == FSYNC_MODE_STRICT) {
		f2fs_add_ino_entry(sbi, old_dir->i_ino, TRANS_DIR_INO);
		f2fs_add_ino_entry(sbi, new_dir->i_ino, TRANS_DIR_INO);
	}

	f2fs_unlock_op(sbi);

	if (IS_DIRSYNC(old_dir) || IS_DIRSYNC(new_dir))
		f2fs_sync_fs(sbi->sb, 1);

	f2fs_update_time(sbi, REQ_TIME);
	return 0;
out_new_dir:
	if (new_dir_entry) {
		f2fs_put_page(new_dir_page, 0);
	}
out_old_dir:
	if (old_dir_entry) {
		f2fs_put_page(old_dir_page, 0);
	}
out_new:
	f2fs_put_page(new_page, 0);
out_old:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_put_page(old_page, 0);
out:
	return err;
}

<<<<<<< HEAD
static void *f2fs_encrypted_follow_link(struct dentry *dentry,
						struct nameidata *nd)
{
	struct page *cpage = NULL;
	char *caddr, *paddr = NULL;
	struct fscrypt_str cstr = FSTR_INIT(NULL, 0);
	struct fscrypt_str pstr = FSTR_INIT(NULL, 0);
	struct fscrypt_symlink_data *sd;
	struct inode *inode = dentry->d_inode;
	loff_t size = min_t(loff_t, i_size_read(inode), PAGE_SIZE - 1);
	u32 max_size = inode->i_sb->s_blocksize;
	int res;

	res = fscrypt_get_encryption_info(inode);
	if (res)
		return ERR_PTR(res);

	cpage = read_mapping_page(inode->i_mapping, 0, NULL);
	if (IS_ERR(cpage))
		return cpage;
	caddr = kmap(cpage);
	caddr[size] = 0;

	/* Symlink is encrypted */
	sd = (struct fscrypt_symlink_data *)caddr;
	cstr.name = sd->encrypted_path;
	cstr.len = le16_to_cpu(sd->len);

	/* this is broken symlink case */
	if (unlikely(cstr.len == 0)) {
		res = -ENOENT;
		goto errout;
	}

	if ((cstr.len + sizeof(struct fscrypt_symlink_data) - 1) > max_size) {
		/* Symlink data on the disk is corrupted */
		res = -EIO;
		goto errout;
	}
	res = fscrypt_fname_alloc_buffer(inode, cstr.len, &pstr);
	if (res)
		goto errout;

	res = fscrypt_fname_disk_to_usr(inode, 0, 0, &cstr, &pstr);
	if (res < 0)
		goto errout;

	/* this is broken symlink case */
	if (unlikely(pstr.name[0] == 0)) {
		res = -ENOENT;
		goto errout;
	}

	paddr = pstr.name;

	/* Null-terminate the name */
	paddr[res] = '\0';
	nd_set_link(nd, paddr);

	kunmap(cpage);
	page_cache_release(cpage);
	return NULL;
errout:
	fscrypt_fname_free_buffer(&pstr);
	kunmap(cpage);
	page_cache_release(cpage);
	return ERR_PTR(res);
}

void kfree_put_link(struct dentry *dentry, struct nameidata *nd,
		void *cookie)
{
	char *s = nd_get_link(nd);
	if (!IS_ERR(s))
		kfree(s);
}

const struct inode_operations f2fs_encrypted_symlink_inode_operations = {
	.readlink       = generic_readlink,
	.follow_link    = f2fs_encrypted_follow_link,
	.put_link       = kfree_put_link,
	.getattr	= f2fs_getattr,
	.setattr	= f2fs_setattr,
#ifdef CONFIG_F2FS_FS_XATTR
	.setxattr	= generic_setxattr,
	.getxattr	= generic_getxattr,
	.listxattr	= f2fs_listxattr,
	.removexattr	= generic_removexattr,
#endif
=======
static int f2fs_rename2(struct mnt_idmap *idmap,
			struct inode *old_dir, struct dentry *old_dentry,
			struct inode *new_dir, struct dentry *new_dentry,
			unsigned int flags)
{
	int err;

	if (flags & ~(RENAME_NOREPLACE | RENAME_EXCHANGE | RENAME_WHITEOUT))
		return -EINVAL;

	trace_f2fs_rename_start(old_dir, old_dentry, new_dir, new_dentry,
								flags);

	err = fscrypt_prepare_rename(old_dir, old_dentry, new_dir, new_dentry,
				     flags);
	if (err)
		return err;

	if (flags & RENAME_EXCHANGE)
		err = f2fs_cross_rename(old_dir, old_dentry,
					new_dir, new_dentry);
	else
	/*
	 * VFS has already handled the new dentry existence case,
	 * here, we just deal with "RENAME_NOREPLACE" as regular rename.
	 */
		err = f2fs_rename(idmap, old_dir, old_dentry,
					new_dir, new_dentry, flags);

	trace_f2fs_rename_end(old_dentry, new_dentry, flags, err);
	return err;
}

static const char *f2fs_encrypted_get_link(struct dentry *dentry,
					   struct inode *inode,
					   struct delayed_call *done)
{
	struct page *page;
	const char *target;

	if (!dentry)
		return ERR_PTR(-ECHILD);

	page = read_mapping_page(inode->i_mapping, 0, NULL);
	if (IS_ERR(page))
		return ERR_CAST(page);

	target = fscrypt_get_symlink(inode, page_address(page),
				     inode->i_sb->s_blocksize, done);
	put_page(page);
	return target;
}

static int f2fs_encrypted_symlink_getattr(struct mnt_idmap *idmap,
					  const struct path *path,
					  struct kstat *stat, u32 request_mask,
					  unsigned int query_flags)
{
	f2fs_getattr(idmap, path, stat, request_mask, query_flags);

	return fscrypt_symlink_getattr(path, stat);
}

const struct inode_operations f2fs_encrypted_symlink_inode_operations = {
	.get_link	= f2fs_encrypted_get_link,
	.getattr	= f2fs_encrypted_symlink_getattr,
	.setattr	= f2fs_setattr,
	.listxattr	= f2fs_listxattr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct inode_operations f2fs_dir_inode_operations = {
	.create		= f2fs_create,
	.lookup		= f2fs_lookup,
	.link		= f2fs_link,
	.unlink		= f2fs_unlink,
	.symlink	= f2fs_symlink,
	.mkdir		= f2fs_mkdir,
	.rmdir		= f2fs_rmdir,
	.mknod		= f2fs_mknod,
<<<<<<< HEAD
	.rename		= f2fs_rename,
	.getattr	= f2fs_getattr,
	.setattr	= f2fs_setattr,
	.get_acl	= f2fs_get_acl,
#ifdef CONFIG_F2FS_FS_XATTR
	.setxattr	= generic_setxattr,
	.getxattr	= generic_getxattr,
	.listxattr	= f2fs_listxattr,
	.removexattr	= generic_removexattr,
#endif
};

const struct inode_operations f2fs_symlink_inode_operations = {
	.readlink       = generic_readlink,
	.follow_link    = f2fs_follow_link,
	.put_link       = page_put_link,
	.getattr	= f2fs_getattr,
	.setattr	= f2fs_setattr,
#ifdef CONFIG_F2FS_FS_XATTR
	.setxattr	= generic_setxattr,
	.getxattr	= generic_getxattr,
	.listxattr	= f2fs_listxattr,
	.removexattr	= generic_removexattr,
#endif
=======
	.rename		= f2fs_rename2,
	.tmpfile	= f2fs_tmpfile,
	.getattr	= f2fs_getattr,
	.setattr	= f2fs_setattr,
	.get_inode_acl	= f2fs_get_acl,
	.set_acl	= f2fs_set_acl,
	.listxattr	= f2fs_listxattr,
	.fiemap		= f2fs_fiemap,
	.fileattr_get	= f2fs_fileattr_get,
	.fileattr_set	= f2fs_fileattr_set,
};

const struct inode_operations f2fs_symlink_inode_operations = {
	.get_link	= f2fs_get_link,
	.getattr	= f2fs_getattr,
	.setattr	= f2fs_setattr,
	.listxattr	= f2fs_listxattr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct inode_operations f2fs_special_inode_operations = {
	.getattr	= f2fs_getattr,
<<<<<<< HEAD
	.setattr        = f2fs_setattr,
	.get_acl	= f2fs_get_acl,
#ifdef CONFIG_F2FS_FS_XATTR
	.setxattr       = generic_setxattr,
	.getxattr       = generic_getxattr,
	.listxattr	= f2fs_listxattr,
	.removexattr    = generic_removexattr,
#endif
=======
	.setattr	= f2fs_setattr,
	.get_inode_acl	= f2fs_get_acl,
	.set_acl	= f2fs_set_acl,
	.listxattr	= f2fs_listxattr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
