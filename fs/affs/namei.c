<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/fs/affs/namei.c
 *
 *  (c) 1996  Hans-Joachim Widmaier - Rewritten
 *
 *  (C) 1993  Ray Burr - Modified for Amiga FFS filesystem.
 *
 *  (C) 1991  Linus Torvalds - minix filesystem
 */

#include "affs.h"
<<<<<<< HEAD

typedef int (*toupper_t)(int);

static int	 affs_toupper(int ch);
static int	 affs_hash_dentry(const struct dentry *,
		const struct inode *, struct qstr *);
static int       affs_compare_dentry(const struct dentry *parent,
		const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name);
static int	 affs_intl_toupper(int ch);
static int	 affs_intl_hash_dentry(const struct dentry *,
		const struct inode *, struct qstr *);
static int       affs_intl_compare_dentry(const struct dentry *parent,
		const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name);

const struct dentry_operations affs_dentry_operations = {
	.d_hash		= affs_hash_dentry,
	.d_compare	= affs_compare_dentry,
};

const struct dentry_operations affs_intl_dentry_operations = {
	.d_hash		= affs_intl_hash_dentry,
	.d_compare	= affs_intl_compare_dentry,
};


=======
#include <linux/exportfs.h>

typedef int (*toupper_t)(int);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Simple toupper() for DOS\1 */

static int
affs_toupper(int ch)
{
	return ch >= 'a' && ch <= 'z' ? ch -= ('a' - 'A') : ch;
}

/* International toupper() for DOS\3 ("international") */

static int
affs_intl_toupper(int ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 0xE0
		&& ch <= 0xFE && ch != 0xF7) ?
		ch - ('a' - 'A') : ch;
}

static inline toupper_t
affs_get_toupper(struct super_block *sb)
{
<<<<<<< HEAD
	return AFFS_SB(sb)->s_flags & SF_INTL ? affs_intl_toupper : affs_toupper;
=======
	return affs_test_opt(AFFS_SB(sb)->s_flags, SF_INTL) ?
	       affs_intl_toupper : affs_toupper;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Note: the dentry argument is the parent dentry.
 */
static inline int
<<<<<<< HEAD
__affs_hash_dentry(struct qstr *qstr, toupper_t toupper)
{
	const u8 *name = qstr->name;
	unsigned long hash;
	int i;

	i = affs_check_name(qstr->name, qstr->len);
	if (i)
		return i;

	hash = init_name_hash();
	i = min(qstr->len, 30u);
	for (; i > 0; name++, i--)
		hash = partial_name_hash(toupper(*name), hash);
=======
__affs_hash_dentry(const struct dentry *dentry, struct qstr *qstr, toupper_t fn, bool notruncate)
{
	const u8 *name = qstr->name;
	unsigned long hash;
	int retval;
	u32 len;

	retval = affs_check_name(qstr->name, qstr->len, notruncate);
	if (retval)
		return retval;

	hash = init_name_hash(dentry);
	len = min(qstr->len, AFFSNAMEMAX);
	for (; len > 0; name++, len--)
		hash = partial_name_hash(fn(*name), hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	qstr->hash = end_name_hash(hash);

	return 0;
}

static int
<<<<<<< HEAD
affs_hash_dentry(const struct dentry *dentry, const struct inode *inode,
		struct qstr *qstr)
{
	return __affs_hash_dentry(qstr, affs_toupper);
}
static int
affs_intl_hash_dentry(const struct dentry *dentry, const struct inode *inode,
		struct qstr *qstr)
{
	return __affs_hash_dentry(qstr, affs_intl_toupper);
}

static inline int __affs_compare_dentry(unsigned int len,
		const char *str, const struct qstr *name, toupper_t toupper)
=======
affs_hash_dentry(const struct dentry *dentry, struct qstr *qstr)
{
	return __affs_hash_dentry(dentry, qstr, affs_toupper,
				  affs_nofilenametruncate(dentry));

}

static int
affs_intl_hash_dentry(const struct dentry *dentry, struct qstr *qstr)
{
	return __affs_hash_dentry(dentry, qstr, affs_intl_toupper,
				  affs_nofilenametruncate(dentry));

}

static inline int __affs_compare_dentry(unsigned int len,
		const char *str, const struct qstr *name, toupper_t fn,
		bool notruncate)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const u8 *aname = str;
	const u8 *bname = name->name;

	/*
	 * 'str' is the name of an already existing dentry, so the name
	 * must be valid. 'name' must be validated first.
	 */

<<<<<<< HEAD
	if (affs_check_name(name->name, name->len))
=======
	if (affs_check_name(name->name, name->len, notruncate))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;

	/*
	 * If the names are longer than the allowed 30 chars,
	 * the excess is ignored, so their length may differ.
	 */
<<<<<<< HEAD
	if (len >= 30) {
		if (name->len < 30)
			return 1;
		len = 30;
=======
	if (len >= AFFSNAMEMAX) {
		if (name->len < AFFSNAMEMAX)
			return 1;
		len = AFFSNAMEMAX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (len != name->len)
		return 1;

	for (; len > 0; len--)
<<<<<<< HEAD
		if (toupper(*aname++) != toupper(*bname++))
=======
		if (fn(*aname++) != fn(*bname++))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 1;

	return 0;
}

static int
<<<<<<< HEAD
affs_compare_dentry(const struct dentry *parent, const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name)
{
	return __affs_compare_dentry(len, str, name, affs_toupper);
}
static int
affs_intl_compare_dentry(const struct dentry *parent,const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name)
{
	return __affs_compare_dentry(len, str, name, affs_intl_toupper);
=======
affs_compare_dentry(const struct dentry *dentry,
		unsigned int len, const char *str, const struct qstr *name)
{

	return __affs_compare_dentry(len, str, name, affs_toupper,
				     affs_nofilenametruncate(dentry));
}

static int
affs_intl_compare_dentry(const struct dentry *dentry,
		unsigned int len, const char *str, const struct qstr *name)
{
	return __affs_compare_dentry(len, str, name, affs_intl_toupper,
				     affs_nofilenametruncate(dentry));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * NOTE! unlike strncmp, affs_match returns 1 for success, 0 for failure.
 */

static inline int
<<<<<<< HEAD
affs_match(struct dentry *dentry, const u8 *name2, toupper_t toupper)
=======
affs_match(struct dentry *dentry, const u8 *name2, toupper_t fn)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const u8 *name = dentry->d_name.name;
	int len = dentry->d_name.len;

<<<<<<< HEAD
	if (len >= 30) {
		if (*name2 < 30)
			return 0;
		len = 30;
=======
	if (len >= AFFSNAMEMAX) {
		if (*name2 < AFFSNAMEMAX)
			return 0;
		len = AFFSNAMEMAX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (len != *name2)
		return 0;

	for (name2++; len > 0; len--)
<<<<<<< HEAD
		if (toupper(*name++) != toupper(*name2++))
=======
		if (fn(*name++) != fn(*name2++))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
	return 1;
}

int
affs_hash_name(struct super_block *sb, const u8 *name, unsigned int len)
{
<<<<<<< HEAD
	toupper_t toupper = affs_get_toupper(sb);
	int hash;

	hash = len = min(len, 30u);
	for (; len > 0; len--)
		hash = (hash * 13 + toupper(*name++)) & 0x7ff;
=======
	toupper_t fn = affs_get_toupper(sb);
	u32 hash;

	hash = len = min(len, AFFSNAMEMAX);
	for (; len > 0; len--)
		hash = (hash * 13 + fn(*name++)) & 0x7ff;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return hash % AFFS_SB(sb)->s_hashsize;
}

static struct buffer_head *
affs_find_entry(struct inode *dir, struct dentry *dentry)
{
	struct super_block *sb = dir->i_sb;
	struct buffer_head *bh;
<<<<<<< HEAD
	toupper_t toupper = affs_get_toupper(sb);
	u32 key;

	pr_debug("AFFS: find_entry(\"%.*s\")\n", (int)dentry->d_name.len, dentry->d_name.name);
=======
	toupper_t fn = affs_get_toupper(sb);
	u32 key;

	pr_debug("%s(\"%pd\")\n", __func__, dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bh = affs_bread(sb, dir->i_ino);
	if (!bh)
		return ERR_PTR(-EIO);

	key = be32_to_cpu(AFFS_HEAD(bh)->table[affs_hash_name(sb, dentry->d_name.name, dentry->d_name.len)]);

	for (;;) {
		affs_brelse(bh);
		if (key == 0)
			return NULL;
		bh = affs_bread(sb, key);
		if (!bh)
			return ERR_PTR(-EIO);
<<<<<<< HEAD
		if (affs_match(dentry, AFFS_TAIL(sb, bh)->name, toupper))
=======
		if (affs_match(dentry, AFFS_TAIL(sb, bh)->name, fn))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return bh;
		key = be32_to_cpu(AFFS_TAIL(sb, bh)->hash_chain);
	}
}

struct dentry *
affs_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags)
{
	struct super_block *sb = dir->i_sb;
	struct buffer_head *bh;
	struct inode *inode = NULL;
<<<<<<< HEAD

	pr_debug("AFFS: lookup(\"%.*s\")\n",(int)dentry->d_name.len,dentry->d_name.name);

	affs_lock_dir(dir);
	bh = affs_find_entry(dir, dentry);
	affs_unlock_dir(dir);
	if (IS_ERR(bh))
		return ERR_CAST(bh);
=======
	struct dentry *res;

	pr_debug("%s(\"%pd\")\n", __func__, dentry);

	affs_lock_dir(dir);
	bh = affs_find_entry(dir, dentry);
	if (IS_ERR(bh)) {
		affs_unlock_dir(dir);
		return ERR_CAST(bh);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (bh) {
		u32 ino = bh->b_blocknr;

		/* store the real header ino in d_fsdata for faster lookups */
		dentry->d_fsdata = (void *)(long)ino;
		switch (be32_to_cpu(AFFS_TAIL(sb, bh)->stype)) {
		//link to dirs disabled
		//case ST_LINKDIR:
		case ST_LINKFILE:
			ino = be32_to_cpu(AFFS_TAIL(sb, bh)->original);
		}
		affs_brelse(bh);
		inode = affs_iget(sb, ino);
<<<<<<< HEAD
		if (IS_ERR(inode))
			return ERR_CAST(inode);
	}
	d_add(dentry, inode);
	return NULL;
=======
	}
	res = d_splice_alias(inode, dentry);
	if (!IS_ERR_OR_NULL(res))
		res->d_fsdata = dentry->d_fsdata;
	affs_unlock_dir(dir);
	return res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
affs_unlink(struct inode *dir, struct dentry *dentry)
{
<<<<<<< HEAD
	pr_debug("AFFS: unlink(dir=%d, %lu \"%.*s\")\n", (u32)dir->i_ino,
		 dentry->d_inode->i_ino,
		 (int)dentry->d_name.len, dentry->d_name.name);
=======
	pr_debug("%s(dir=%lu, %lu \"%pd\")\n", __func__, dir->i_ino,
		 d_inode(dentry)->i_ino, dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return affs_remove_header(dentry);
}

int
<<<<<<< HEAD
affs_create(struct inode *dir, struct dentry *dentry, umode_t mode, bool excl)
=======
affs_create(struct mnt_idmap *idmap, struct inode *dir,
	    struct dentry *dentry, umode_t mode, bool excl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct super_block *sb = dir->i_sb;
	struct inode	*inode;
	int		 error;

<<<<<<< HEAD
	pr_debug("AFFS: create(%lu,\"%.*s\",0%ho)\n",dir->i_ino,(int)dentry->d_name.len,
		 dentry->d_name.name,mode);
=======
	pr_debug("%s(%lu,\"%pd\",0%ho)\n",
		 __func__, dir->i_ino, dentry, mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode = affs_new_inode(dir);
	if (!inode)
		return -ENOSPC;

	inode->i_mode = mode;
<<<<<<< HEAD
	mode_to_prot(inode);
=======
	affs_mode_to_prot(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mark_inode_dirty(inode);

	inode->i_op = &affs_file_inode_operations;
	inode->i_fop = &affs_file_operations;
<<<<<<< HEAD
	inode->i_mapping->a_ops = (AFFS_SB(sb)->s_flags & SF_OFS) ? &affs_aops_ofs : &affs_aops;
=======
	inode->i_mapping->a_ops = affs_test_opt(AFFS_SB(sb)->s_flags, SF_OFS) ?
				  &affs_aops_ofs : &affs_aops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = affs_add_entry(dir, inode, dentry, ST_FILE);
	if (error) {
		clear_nlink(inode);
		iput(inode);
		return error;
	}
	return 0;
}

int
<<<<<<< HEAD
affs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
=======
affs_mkdir(struct mnt_idmap *idmap, struct inode *dir,
	   struct dentry *dentry, umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode		*inode;
	int			 error;

<<<<<<< HEAD
	pr_debug("AFFS: mkdir(%lu,\"%.*s\",0%ho)\n",dir->i_ino,
		 (int)dentry->d_name.len,dentry->d_name.name,mode);
=======
	pr_debug("%s(%lu,\"%pd\",0%ho)\n",
		 __func__, dir->i_ino, dentry, mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode = affs_new_inode(dir);
	if (!inode)
		return -ENOSPC;

	inode->i_mode = S_IFDIR | mode;
<<<<<<< HEAD
	mode_to_prot(inode);
=======
	affs_mode_to_prot(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode->i_op = &affs_dir_inode_operations;
	inode->i_fop = &affs_dir_operations;

	error = affs_add_entry(dir, inode, dentry, ST_USERDIR);
	if (error) {
		clear_nlink(inode);
		mark_inode_dirty(inode);
		iput(inode);
		return error;
	}
	return 0;
}

int
affs_rmdir(struct inode *dir, struct dentry *dentry)
{
<<<<<<< HEAD
	pr_debug("AFFS: rmdir(dir=%u, %lu \"%.*s\")\n", (u32)dir->i_ino,
		 dentry->d_inode->i_ino,
		 (int)dentry->d_name.len, dentry->d_name.name);
=======
	pr_debug("%s(dir=%lu, %lu \"%pd\")\n", __func__, dir->i_ino,
		 d_inode(dentry)->i_ino, dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return affs_remove_header(dentry);
}

int
<<<<<<< HEAD
affs_symlink(struct inode *dir, struct dentry *dentry, const char *symname)
=======
affs_symlink(struct mnt_idmap *idmap, struct inode *dir,
	     struct dentry *dentry, const char *symname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct super_block	*sb = dir->i_sb;
	struct buffer_head	*bh;
	struct inode		*inode;
	char			*p;
	int			 i, maxlen, error;
	char			 c, lc;

<<<<<<< HEAD
	pr_debug("AFFS: symlink(%lu,\"%.*s\" -> \"%s\")\n",dir->i_ino,
		 (int)dentry->d_name.len,dentry->d_name.name,symname);
=======
	pr_debug("%s(%lu,\"%pd\" -> \"%s\")\n",
		 __func__, dir->i_ino, dentry, symname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	maxlen = AFFS_SB(sb)->s_hashsize * sizeof(u32) - 1;
	inode  = affs_new_inode(dir);
	if (!inode)
		return -ENOSPC;

	inode->i_op = &affs_symlink_inode_operations;
<<<<<<< HEAD
	inode->i_data.a_ops = &affs_symlink_aops;
	inode->i_mode = S_IFLNK | 0777;
	mode_to_prot(inode);
=======
	inode_nohighmem(inode);
	inode->i_data.a_ops = &affs_symlink_aops;
	inode->i_mode = S_IFLNK | 0777;
	affs_mode_to_prot(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	error = -EIO;
	bh = affs_bread(sb, inode->i_ino);
	if (!bh)
		goto err;
	i  = 0;
	p  = (char *)AFFS_HEAD(bh)->table;
	lc = '/';
	if (*symname == '/') {
		struct affs_sb_info *sbi = AFFS_SB(sb);
		while (*symname == '/')
			symname++;
		spin_lock(&sbi->symlink_lock);
		while (sbi->s_volume[i])	/* Cannot overflow */
			*p++ = sbi->s_volume[i++];
		spin_unlock(&sbi->symlink_lock);
	}
	while (i < maxlen && (c = *symname++)) {
		if (c == '.' && lc == '/' && *symname == '.' && symname[1] == '/') {
			*p++ = '/';
			i++;
			symname += 2;
			lc = '/';
		} else if (c == '.' && lc == '/' && *symname == '/') {
			symname++;
			lc = '/';
		} else {
			*p++ = c;
			lc   = c;
			i++;
		}
		if (lc == '/')
			while (*symname == '/')
				symname++;
	}
	*p = 0;
<<<<<<< HEAD
=======
	inode->i_size = i + 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mark_buffer_dirty_inode(bh, inode);
	affs_brelse(bh);
	mark_inode_dirty(inode);

	error = affs_add_entry(dir, inode, dentry, ST_SOFTLINK);
	if (error)
		goto err;

	return 0;

err:
	clear_nlink(inode);
	mark_inode_dirty(inode);
	iput(inode);
	return error;
}

int
affs_link(struct dentry *old_dentry, struct inode *dir, struct dentry *dentry)
{
<<<<<<< HEAD
	struct inode *inode = old_dentry->d_inode;

	pr_debug("AFFS: link(%u, %u, \"%.*s\")\n", (u32)inode->i_ino, (u32)dir->i_ino,
		 (int)dentry->d_name.len,dentry->d_name.name);
=======
	struct inode *inode = d_inode(old_dentry);

	pr_debug("%s(%lu, %lu, \"%pd\")\n", __func__, inode->i_ino, dir->i_ino,
		 dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return affs_add_entry(dir, inode, dentry, ST_LINKFILE);
}

<<<<<<< HEAD
int
=======
static int
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
affs_rename(struct inode *old_dir, struct dentry *old_dentry,
	    struct inode *new_dir, struct dentry *new_dentry)
{
	struct super_block *sb = old_dir->i_sb;
	struct buffer_head *bh = NULL;
	int retval;

<<<<<<< HEAD
	pr_debug("AFFS: rename(old=%u,\"%*s\" to new=%u,\"%*s\")\n",
		 (u32)old_dir->i_ino, (int)old_dentry->d_name.len, old_dentry->d_name.name,
		 (u32)new_dir->i_ino, (int)new_dentry->d_name.len, new_dentry->d_name.name);

	retval = affs_check_name(new_dentry->d_name.name,new_dentry->d_name.len);
=======
	retval = affs_check_name(new_dentry->d_name.name,
				 new_dentry->d_name.len,
				 affs_nofilenametruncate(old_dentry));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (retval)
		return retval;

	/* Unlink destination if it already exists */
<<<<<<< HEAD
	if (new_dentry->d_inode) {
=======
	if (d_really_is_positive(new_dentry)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		retval = affs_remove_header(new_dentry);
		if (retval)
			return retval;
	}

<<<<<<< HEAD
	bh = affs_bread(sb, old_dentry->d_inode->i_ino);
=======
	bh = affs_bread(sb, d_inode(old_dentry)->i_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!bh)
		return -EIO;

	/* Remove header from its parent directory. */
	affs_lock_dir(old_dir);
	retval = affs_remove_hash(old_dir, bh);
	affs_unlock_dir(old_dir);
	if (retval)
		goto done;

	/* And insert it into the new directory with the new name. */
	affs_copy_name(AFFS_TAIL(sb, bh)->name, new_dentry);
	affs_fix_checksum(sb, bh);
	affs_lock_dir(new_dir);
	retval = affs_insert_hash(new_dir, bh);
	affs_unlock_dir(new_dir);
	/* TODO: move it back to old_dir, if error? */

done:
	mark_buffer_dirty_inode(bh, retval ? old_dir : new_dir);
	affs_brelse(bh);
	return retval;
}
<<<<<<< HEAD
=======

static int
affs_xrename(struct inode *old_dir, struct dentry *old_dentry,
	     struct inode *new_dir, struct dentry *new_dentry)
{

	struct super_block *sb = old_dir->i_sb;
	struct buffer_head *bh_old = NULL;
	struct buffer_head *bh_new = NULL;
	int retval;

	bh_old = affs_bread(sb, d_inode(old_dentry)->i_ino);
	if (!bh_old)
		return -EIO;

	bh_new = affs_bread(sb, d_inode(new_dentry)->i_ino);
	if (!bh_new) {
		affs_brelse(bh_old);
		return -EIO;
	}

	/* Remove old header from its parent directory. */
	affs_lock_dir(old_dir);
	retval = affs_remove_hash(old_dir, bh_old);
	affs_unlock_dir(old_dir);
	if (retval)
		goto done;

	/* Remove new header from its parent directory. */
	affs_lock_dir(new_dir);
	retval = affs_remove_hash(new_dir, bh_new);
	affs_unlock_dir(new_dir);
	if (retval)
		goto done;

	/* Insert old into the new directory with the new name. */
	affs_copy_name(AFFS_TAIL(sb, bh_old)->name, new_dentry);
	affs_fix_checksum(sb, bh_old);
	affs_lock_dir(new_dir);
	retval = affs_insert_hash(new_dir, bh_old);
	affs_unlock_dir(new_dir);

	/* Insert new into the old directory with the old name. */
	affs_copy_name(AFFS_TAIL(sb, bh_new)->name, old_dentry);
	affs_fix_checksum(sb, bh_new);
	affs_lock_dir(old_dir);
	retval = affs_insert_hash(old_dir, bh_new);
	affs_unlock_dir(old_dir);
done:
	mark_buffer_dirty_inode(bh_old, new_dir);
	mark_buffer_dirty_inode(bh_new, old_dir);
	affs_brelse(bh_old);
	affs_brelse(bh_new);
	return retval;
}

int affs_rename2(struct mnt_idmap *idmap, struct inode *old_dir,
		 struct dentry *old_dentry, struct inode *new_dir,
		 struct dentry *new_dentry, unsigned int flags)
{

	if (flags & ~(RENAME_NOREPLACE | RENAME_EXCHANGE))
		return -EINVAL;

	pr_debug("%s(old=%lu,\"%pd\" to new=%lu,\"%pd\")\n", __func__,
		 old_dir->i_ino, old_dentry, new_dir->i_ino, new_dentry);

	if (flags & RENAME_EXCHANGE)
		return affs_xrename(old_dir, old_dentry, new_dir, new_dentry);

	return affs_rename(old_dir, old_dentry, new_dir, new_dentry);
}

static struct dentry *affs_get_parent(struct dentry *child)
{
	struct inode *parent;
	struct buffer_head *bh;

	bh = affs_bread(child->d_sb, d_inode(child)->i_ino);
	if (!bh)
		return ERR_PTR(-EIO);

	parent = affs_iget(child->d_sb,
			   be32_to_cpu(AFFS_TAIL(child->d_sb, bh)->parent));
	brelse(bh);
	return d_obtain_alias(parent);
}

static struct inode *affs_nfs_get_inode(struct super_block *sb, u64 ino,
					u32 generation)
{
	struct inode *inode;

	if (!affs_validblock(sb, ino))
		return ERR_PTR(-ESTALE);

	inode = affs_iget(sb, ino);
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	return inode;
}

static struct dentry *affs_fh_to_dentry(struct super_block *sb, struct fid *fid,
					int fh_len, int fh_type)
{
	return generic_fh_to_dentry(sb, fid, fh_len, fh_type,
				    affs_nfs_get_inode);
}

static struct dentry *affs_fh_to_parent(struct super_block *sb, struct fid *fid,
					int fh_len, int fh_type)
{
	return generic_fh_to_parent(sb, fid, fh_len, fh_type,
				    affs_nfs_get_inode);
}

const struct export_operations affs_export_ops = {
	.encode_fh = generic_encode_ino32_fh,
	.fh_to_dentry = affs_fh_to_dentry,
	.fh_to_parent = affs_fh_to_parent,
	.get_parent = affs_get_parent,
};

const struct dentry_operations affs_dentry_operations = {
	.d_hash		= affs_hash_dentry,
	.d_compare	= affs_compare_dentry,
};

const struct dentry_operations affs_intl_dentry_operations = {
	.d_hash		= affs_intl_hash_dentry,
	.d_compare	= affs_intl_compare_dentry,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
