<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * fs/f2fs/xattr.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *             http://www.samsung.com/
 *
 * Portions of this code from linux/fs/ext2/xattr.c
 *
 * Copyright (C) 2001-2003 Andreas Gruenbacher <agruen@suse.de>
 *
 * Fix by Harrison Xing <harrison@mountainviewdata.com>.
 * Extended attributes for symlinks and special files added per
 *  suggestion of Luka Renko <luka.renko@hermes.si>.
 * xattr consolidation Copyright (c) 2004 James Morris <jmorris@redhat.com>,
 *  Red Hat Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/rwsem.h>
#include <linux/f2fs_fs.h>
#include <linux/security.h>
<<<<<<< HEAD
#include "f2fs.h"
#include "xattr.h"

static size_t f2fs_xattr_generic_list(struct dentry *dentry, char *list,
		size_t list_size, const char *name, size_t len, int type)
{
	struct f2fs_sb_info *sbi = F2FS_SB(dentry->d_sb);
	int total_len, prefix_len = 0;
	const char *prefix = NULL;

	switch (type) {
	case F2FS_XATTR_INDEX_USER:
		if (!test_opt(sbi, XATTR_USER))
			return -EOPNOTSUPP;
		prefix = XATTR_USER_PREFIX;
		prefix_len = XATTR_USER_PREFIX_LEN;
		break;
	case F2FS_XATTR_INDEX_TRUSTED:
		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;
		prefix = XATTR_TRUSTED_PREFIX;
		prefix_len = XATTR_TRUSTED_PREFIX_LEN;
		break;
	case F2FS_XATTR_INDEX_SECURITY:
		prefix = XATTR_SECURITY_PREFIX;
		prefix_len = XATTR_SECURITY_PREFIX_LEN;
		break;
	default:
		return -EINVAL;
	}

	total_len = prefix_len + len + 1;
	if (list && total_len <= list_size) {
		memcpy(list, prefix, prefix_len);
		memcpy(list + prefix_len, name, len);
		list[prefix_len + len] = '\0';
	}
	return total_len;
}

static int f2fs_xattr_generic_get(struct dentry *dentry, const char *name,
		void *buffer, size_t size, int type)
{
	struct f2fs_sb_info *sbi = F2FS_SB(dentry->d_sb);

	switch (type) {
	case F2FS_XATTR_INDEX_USER:
		if (!test_opt(sbi, XATTR_USER))
			return -EOPNOTSUPP;
		break;
	case F2FS_XATTR_INDEX_TRUSTED:
		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;
		break;
	case F2FS_XATTR_INDEX_SECURITY:
		break;
	default:
		return -EINVAL;
	}
	if (strcmp(name, "") == 0)
		return -EINVAL;
	return f2fs_getxattr(dentry->d_inode, type, name, buffer, size, NULL);
}

static int f2fs_xattr_generic_set(struct dentry *dentry, const char *name,
		const void *value, size_t size, int flags, int type)
{
	struct f2fs_sb_info *sbi = F2FS_SB(dentry->d_sb);

	switch (type) {
=======
#include <linux/posix_acl_xattr.h>
#include "f2fs.h"
#include "xattr.h"
#include "segment.h"

static void *xattr_alloc(struct f2fs_sb_info *sbi, int size, bool *is_inline)
{
	if (likely(size == sbi->inline_xattr_slab_size)) {
		*is_inline = true;
		return f2fs_kmem_cache_alloc(sbi->inline_xattr_slab,
					GFP_F2FS_ZERO, false, sbi);
	}
	*is_inline = false;
	return f2fs_kzalloc(sbi, size, GFP_NOFS);
}

static void xattr_free(struct f2fs_sb_info *sbi, void *xattr_addr,
							bool is_inline)
{
	if (is_inline)
		kmem_cache_free(sbi->inline_xattr_slab, xattr_addr);
	else
		kfree(xattr_addr);
}

static int f2fs_xattr_generic_get(const struct xattr_handler *handler,
		struct dentry *unused, struct inode *inode,
		const char *name, void *buffer, size_t size)
{
	struct f2fs_sb_info *sbi = F2FS_SB(inode->i_sb);

	switch (handler->flags) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case F2FS_XATTR_INDEX_USER:
		if (!test_opt(sbi, XATTR_USER))
			return -EOPNOTSUPP;
		break;
	case F2FS_XATTR_INDEX_TRUSTED:
<<<<<<< HEAD
		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;
		break;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case F2FS_XATTR_INDEX_SECURITY:
		break;
	default:
		return -EINVAL;
	}
<<<<<<< HEAD
	if (strcmp(name, "") == 0)
		return -EINVAL;

	return f2fs_setxattr(dentry->d_inode, type, name,
					value, size, NULL, flags);
}

static size_t f2fs_xattr_advise_list(struct dentry *dentry, char *list,
		size_t list_size, const char *name, size_t len, int type)
{
	const char *xname = F2FS_SYSTEM_ADVISE_PREFIX;
	size_t size;

	if (type != F2FS_XATTR_INDEX_ADVISE)
		return 0;

	size = strlen(xname) + 1;
	if (list && size <= list_size)
		memcpy(list, xname, size);
	return size;
}

static int f2fs_xattr_advise_get(struct dentry *dentry, const char *name,
		void *buffer, size_t size, int type)
{
	struct inode *inode = dentry->d_inode;

	if (strcmp(name, "") != 0)
		return -EINVAL;

=======
	return f2fs_getxattr(inode, handler->flags, name,
			     buffer, size, NULL);
}

static int f2fs_xattr_generic_set(const struct xattr_handler *handler,
		struct mnt_idmap *idmap,
		struct dentry *unused, struct inode *inode,
		const char *name, const void *value,
		size_t size, int flags)
{
	struct f2fs_sb_info *sbi = F2FS_SB(inode->i_sb);

	switch (handler->flags) {
	case F2FS_XATTR_INDEX_USER:
		if (!test_opt(sbi, XATTR_USER))
			return -EOPNOTSUPP;
		break;
	case F2FS_XATTR_INDEX_TRUSTED:
	case F2FS_XATTR_INDEX_SECURITY:
		break;
	default:
		return -EINVAL;
	}
	return f2fs_setxattr(inode, handler->flags, name,
					value, size, NULL, flags);
}

static bool f2fs_xattr_user_list(struct dentry *dentry)
{
	struct f2fs_sb_info *sbi = F2FS_SB(dentry->d_sb);

	return test_opt(sbi, XATTR_USER);
}

static bool f2fs_xattr_trusted_list(struct dentry *dentry)
{
	return capable(CAP_SYS_ADMIN);
}

static int f2fs_xattr_advise_get(const struct xattr_handler *handler,
		struct dentry *unused, struct inode *inode,
		const char *name, void *buffer, size_t size)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (buffer)
		*((char *)buffer) = F2FS_I(inode)->i_advise;
	return sizeof(char);
}

<<<<<<< HEAD
static int f2fs_xattr_advise_set(struct dentry *dentry, const char *name,
		const void *value, size_t size, int flags, int type)
{
	struct inode *inode = dentry->d_inode;

	if (strcmp(name, "") != 0)
		return -EINVAL;
	if (!inode_owner_or_capable(inode))
=======
static int f2fs_xattr_advise_set(const struct xattr_handler *handler,
		struct mnt_idmap *idmap,
		struct dentry *unused, struct inode *inode,
		const char *name, const void *value,
		size_t size, int flags)
{
	unsigned char old_advise = F2FS_I(inode)->i_advise;
	unsigned char new_advise;

	if (!inode_owner_or_capable(&nop_mnt_idmap, inode))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;
	if (value == NULL)
		return -EINVAL;

<<<<<<< HEAD
	F2FS_I(inode)->i_advise |= *(char *)value;
	mark_inode_dirty(inode);
=======
	new_advise = *(char *)value;
	if (new_advise & ~FADVISE_MODIFIABLE_BITS)
		return -EINVAL;

	new_advise = new_advise & FADVISE_MODIFIABLE_BITS;
	new_advise |= old_advise & ~FADVISE_MODIFIABLE_BITS;

	F2FS_I(inode)->i_advise = new_advise;
	f2fs_mark_inode_dirty_sync(inode, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

#ifdef CONFIG_F2FS_FS_SECURITY
static int f2fs_initxattrs(struct inode *inode, const struct xattr *xattr_array,
		void *page)
{
	const struct xattr *xattr;
	int err = 0;

	for (xattr = xattr_array; xattr->name != NULL; xattr++) {
		err = f2fs_setxattr(inode, F2FS_XATTR_INDEX_SECURITY,
				xattr->name, xattr->value,
				xattr->value_len, (struct page *)page, 0);
		if (err < 0)
			break;
	}
	return err;
}

int f2fs_init_security(struct inode *inode, struct inode *dir,
				const struct qstr *qstr, struct page *ipage)
{
	return security_inode_init_security(inode, dir, qstr,
				&f2fs_initxattrs, ipage);
}
#endif

const struct xattr_handler f2fs_xattr_user_handler = {
	.prefix	= XATTR_USER_PREFIX,
	.flags	= F2FS_XATTR_INDEX_USER,
<<<<<<< HEAD
	.list	= f2fs_xattr_generic_list,
=======
	.list	= f2fs_xattr_user_list,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get	= f2fs_xattr_generic_get,
	.set	= f2fs_xattr_generic_set,
};

const struct xattr_handler f2fs_xattr_trusted_handler = {
	.prefix	= XATTR_TRUSTED_PREFIX,
	.flags	= F2FS_XATTR_INDEX_TRUSTED,
<<<<<<< HEAD
	.list	= f2fs_xattr_generic_list,
=======
	.list	= f2fs_xattr_trusted_list,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get	= f2fs_xattr_generic_get,
	.set	= f2fs_xattr_generic_set,
};

const struct xattr_handler f2fs_xattr_advise_handler = {
<<<<<<< HEAD
	.prefix = F2FS_SYSTEM_ADVISE_PREFIX,
	.flags	= F2FS_XATTR_INDEX_ADVISE,
	.list   = f2fs_xattr_advise_list,
	.get    = f2fs_xattr_advise_get,
	.set    = f2fs_xattr_advise_set,
=======
	.name	= F2FS_SYSTEM_ADVISE_NAME,
	.flags	= F2FS_XATTR_INDEX_ADVISE,
	.get	= f2fs_xattr_advise_get,
	.set	= f2fs_xattr_advise_set,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct xattr_handler f2fs_xattr_security_handler = {
	.prefix	= XATTR_SECURITY_PREFIX,
	.flags	= F2FS_XATTR_INDEX_SECURITY,
<<<<<<< HEAD
	.list	= f2fs_xattr_generic_list,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get	= f2fs_xattr_generic_get,
	.set	= f2fs_xattr_generic_set,
};

<<<<<<< HEAD
static const struct xattr_handler *f2fs_xattr_handler_map[] = {
	[F2FS_XATTR_INDEX_USER] = &f2fs_xattr_user_handler,
#ifdef CONFIG_F2FS_FS_POSIX_ACL
	[F2FS_XATTR_INDEX_POSIX_ACL_ACCESS] = &f2fs_xattr_acl_access_handler,
	[F2FS_XATTR_INDEX_POSIX_ACL_DEFAULT] = &f2fs_xattr_acl_default_handler,
=======
static const struct xattr_handler * const f2fs_xattr_handler_map[] = {
	[F2FS_XATTR_INDEX_USER] = &f2fs_xattr_user_handler,
#ifdef CONFIG_F2FS_FS_POSIX_ACL
	[F2FS_XATTR_INDEX_POSIX_ACL_ACCESS] = &nop_posix_acl_access,
	[F2FS_XATTR_INDEX_POSIX_ACL_DEFAULT] = &nop_posix_acl_default,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	[F2FS_XATTR_INDEX_TRUSTED] = &f2fs_xattr_trusted_handler,
#ifdef CONFIG_F2FS_FS_SECURITY
	[F2FS_XATTR_INDEX_SECURITY] = &f2fs_xattr_security_handler,
#endif
	[F2FS_XATTR_INDEX_ADVISE] = &f2fs_xattr_advise_handler,
};

<<<<<<< HEAD
const struct xattr_handler *f2fs_xattr_handlers[] = {
	&f2fs_xattr_user_handler,
#ifdef CONFIG_F2FS_FS_POSIX_ACL
	&f2fs_xattr_acl_access_handler,
	&f2fs_xattr_acl_default_handler,
#endif
=======
const struct xattr_handler * const f2fs_xattr_handlers[] = {
	&f2fs_xattr_user_handler,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&f2fs_xattr_trusted_handler,
#ifdef CONFIG_F2FS_FS_SECURITY
	&f2fs_xattr_security_handler,
#endif
	&f2fs_xattr_advise_handler,
	NULL,
};

<<<<<<< HEAD
static inline const struct xattr_handler *f2fs_xattr_handler(int index)
=======
static inline const char *f2fs_xattr_prefix(int index,
					    struct dentry *dentry)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct xattr_handler *handler = NULL;

	if (index > 0 && index < ARRAY_SIZE(f2fs_xattr_handler_map))
		handler = f2fs_xattr_handler_map[index];
<<<<<<< HEAD
	return handler;
}

static struct f2fs_xattr_entry *__find_xattr(void *base_addr, int index,
					size_t len, const char *name)
=======

	if (!xattr_handler_can_list(handler, dentry))
		return NULL;

	return xattr_prefix(handler);
}

static struct f2fs_xattr_entry *__find_xattr(void *base_addr,
				void *last_base_addr, void **last_addr,
				int index, size_t len, const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_xattr_entry *entry;

	list_for_each_xattr(entry, base_addr) {
<<<<<<< HEAD
=======
		if ((void *)(entry) + sizeof(__u32) > last_base_addr ||
			(void *)XATTR_NEXT_ENTRY(entry) > last_base_addr) {
			if (last_addr)
				*last_addr = entry;
			return NULL;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (entry->e_name_index != index)
			continue;
		if (entry->e_name_len != len)
			continue;
		if (!memcmp(entry->e_name, name, len))
			break;
	}
	return entry;
}

<<<<<<< HEAD
static void *read_all_xattrs(struct inode *inode, struct page *ipage)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct f2fs_xattr_header *header;
	size_t size = PAGE_SIZE, inline_size = 0;
	void *txattr_addr;

	inline_size = inline_xattr_size(inode);

	txattr_addr = kzalloc(inline_size + size, GFP_F2FS_ZERO);
	if (!txattr_addr)
		return NULL;

	/* read from inline xattr */
	if (inline_size) {
		struct page *page = NULL;
		void *inline_addr;

		if (ipage) {
			inline_addr = inline_xattr_addr(ipage);
		} else {
			page = get_node_page(sbi, inode->i_ino);
			if (IS_ERR(page))
				goto fail;
			inline_addr = inline_xattr_addr(page);
		}
		memcpy(txattr_addr, inline_addr, inline_size);
		f2fs_put_page(page, 1);
	}

	/* read from xattr node block */
	if (F2FS_I(inode)->i_xattr_nid) {
		struct page *xpage;
		void *xattr_addr;

		/* The inode already has an extended attribute block. */
		xpage = get_node_page(sbi, F2FS_I(inode)->i_xattr_nid);
		if (IS_ERR(xpage))
			goto fail;

		xattr_addr = page_address(xpage);
		memcpy(txattr_addr + inline_size, xattr_addr, PAGE_SIZE);
		f2fs_put_page(xpage, 1);
=======
static struct f2fs_xattr_entry *__find_inline_xattr(struct inode *inode,
				void *base_addr, void **last_addr, int index,
				size_t len, const char *name)
{
	struct f2fs_xattr_entry *entry;
	unsigned int inline_size = inline_xattr_size(inode);
	void *max_addr = base_addr + inline_size;

	entry = __find_xattr(base_addr, max_addr, last_addr, index, len, name);
	if (!entry)
		return NULL;

	/* inline xattr header or entry across max inline xattr size */
	if (IS_XATTR_LAST_ENTRY(entry) &&
		(void *)entry + sizeof(__u32) > max_addr) {
		*last_addr = entry;
		return NULL;
	}
	return entry;
}

static int read_inline_xattr(struct inode *inode, struct page *ipage,
							void *txattr_addr)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	unsigned int inline_size = inline_xattr_size(inode);
	struct page *page = NULL;
	void *inline_addr;

	if (ipage) {
		inline_addr = inline_xattr_addr(inode, ipage);
	} else {
		page = f2fs_get_node_page(sbi, inode->i_ino);
		if (IS_ERR(page))
			return PTR_ERR(page);

		inline_addr = inline_xattr_addr(inode, page);
	}
	memcpy(txattr_addr, inline_addr, inline_size);
	f2fs_put_page(page, 1);

	return 0;
}

static int read_xattr_block(struct inode *inode, void *txattr_addr)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	nid_t xnid = F2FS_I(inode)->i_xattr_nid;
	unsigned int inline_size = inline_xattr_size(inode);
	struct page *xpage;
	void *xattr_addr;

	/* The inode already has an extended attribute block. */
	xpage = f2fs_get_node_page(sbi, xnid);
	if (IS_ERR(xpage))
		return PTR_ERR(xpage);

	xattr_addr = page_address(xpage);
	memcpy(txattr_addr + inline_size, xattr_addr, VALID_XATTR_BLOCK_SIZE);
	f2fs_put_page(xpage, 1);

	return 0;
}

static int lookup_all_xattrs(struct inode *inode, struct page *ipage,
				unsigned int index, unsigned int len,
				const char *name, struct f2fs_xattr_entry **xe,
				void **base_addr, int *base_size,
				bool *is_inline)
{
	void *cur_addr, *txattr_addr, *last_txattr_addr;
	void *last_addr = NULL;
	nid_t xnid = F2FS_I(inode)->i_xattr_nid;
	unsigned int inline_size = inline_xattr_size(inode);
	int err;

	if (!xnid && !inline_size)
		return -ENODATA;

	*base_size = XATTR_SIZE(inode) + XATTR_PADDING_SIZE;
	txattr_addr = xattr_alloc(F2FS_I_SB(inode), *base_size, is_inline);
	if (!txattr_addr)
		return -ENOMEM;

	last_txattr_addr = (void *)txattr_addr + XATTR_SIZE(inode);

	/* read from inline xattr */
	if (inline_size) {
		err = read_inline_xattr(inode, ipage, txattr_addr);
		if (err)
			goto out;

		*xe = __find_inline_xattr(inode, txattr_addr, &last_addr,
						index, len, name);
		if (*xe) {
			*base_size = inline_size;
			goto check;
		}
	}

	/* read from xattr node block */
	if (xnid) {
		err = read_xattr_block(inode, txattr_addr);
		if (err)
			goto out;
	}

	if (last_addr)
		cur_addr = XATTR_HDR(last_addr) - 1;
	else
		cur_addr = txattr_addr;

	*xe = __find_xattr(cur_addr, last_txattr_addr, NULL, index, len, name);
	if (!*xe) {
		f2fs_err(F2FS_I_SB(inode), "lookup inode (%lu) has corrupted xattr",
								inode->i_ino);
		set_sbi_flag(F2FS_I_SB(inode), SBI_NEED_FSCK);
		err = -ENODATA;
		f2fs_handle_error(F2FS_I_SB(inode),
					ERROR_CORRUPTED_XATTR);
		goto out;
	}
check:
	if (IS_XATTR_LAST_ENTRY(*xe)) {
		err = -ENODATA;
		goto out;
	}

	*base_addr = txattr_addr;
	return 0;
out:
	xattr_free(F2FS_I_SB(inode), txattr_addr, *is_inline);
	return err;
}

static int read_all_xattrs(struct inode *inode, struct page *ipage,
							void **base_addr)
{
	struct f2fs_xattr_header *header;
	nid_t xnid = F2FS_I(inode)->i_xattr_nid;
	unsigned int size = VALID_XATTR_BLOCK_SIZE;
	unsigned int inline_size = inline_xattr_size(inode);
	void *txattr_addr;
	int err;

	txattr_addr = f2fs_kzalloc(F2FS_I_SB(inode),
			inline_size + size + XATTR_PADDING_SIZE, GFP_NOFS);
	if (!txattr_addr)
		return -ENOMEM;

	/* read from inline xattr */
	if (inline_size) {
		err = read_inline_xattr(inode, ipage, txattr_addr);
		if (err)
			goto fail;
	}

	/* read from xattr node block */
	if (xnid) {
		err = read_xattr_block(inode, txattr_addr);
		if (err)
			goto fail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	header = XATTR_HDR(txattr_addr);

	/* never been allocated xattrs */
	if (le32_to_cpu(header->h_magic) != F2FS_XATTR_MAGIC) {
		header->h_magic = cpu_to_le32(F2FS_XATTR_MAGIC);
		header->h_refcount = cpu_to_le32(1);
	}
<<<<<<< HEAD
	return txattr_addr;
fail:
	kzfree(txattr_addr);
	return NULL;
=======
	*base_addr = txattr_addr;
	return 0;
fail:
	kfree(txattr_addr);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int write_all_xattrs(struct inode *inode, __u32 hsize,
				void *txattr_addr, struct page *ipage)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
<<<<<<< HEAD
	size_t inline_size = 0;
	void *xattr_addr;
	struct page *xpage;
	nid_t new_nid = 0;
	int err;

	inline_size = inline_xattr_size(inode);

	if (hsize > inline_size && !F2FS_I(inode)->i_xattr_nid)
		if (!alloc_nid(sbi, &new_nid))
=======
	size_t inline_size = inline_xattr_size(inode);
	struct page *in_page = NULL;
	void *xattr_addr;
	void *inline_addr = NULL;
	struct page *xpage;
	nid_t new_nid = 0;
	int err = 0;

	if (hsize > inline_size && !F2FS_I(inode)->i_xattr_nid)
		if (!f2fs_alloc_nid(sbi, &new_nid))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ENOSPC;

	/* write to inline xattr */
	if (inline_size) {
<<<<<<< HEAD
		struct page *page = NULL;
		void *inline_addr;

		if (ipage) {
			inline_addr = inline_xattr_addr(ipage);
			f2fs_wait_on_page_writeback(ipage, NODE, true);
		} else {
			page = get_node_page(sbi, inode->i_ino);
			if (IS_ERR(page)) {
				alloc_nid_failed(sbi, new_nid);
				return PTR_ERR(page);
			}
			inline_addr = inline_xattr_addr(page);
			f2fs_wait_on_page_writeback(page, NODE, true);
		}
		memcpy(inline_addr, txattr_addr, inline_size);
		f2fs_put_page(page, 1);

		/* no need to use xattr node block */
		if (hsize <= inline_size) {
			err = truncate_xattr_node(inode, ipage);
			alloc_nid_failed(sbi, new_nid);
			return err;
=======
		if (ipage) {
			inline_addr = inline_xattr_addr(inode, ipage);
		} else {
			in_page = f2fs_get_node_page(sbi, inode->i_ino);
			if (IS_ERR(in_page)) {
				f2fs_alloc_nid_failed(sbi, new_nid);
				return PTR_ERR(in_page);
			}
			inline_addr = inline_xattr_addr(inode, in_page);
		}

		f2fs_wait_on_page_writeback(ipage ? ipage : in_page,
							NODE, true, true);
		/* no need to use xattr node block */
		if (hsize <= inline_size) {
			err = f2fs_truncate_xattr_node(inode);
			f2fs_alloc_nid_failed(sbi, new_nid);
			if (err) {
				f2fs_put_page(in_page, 1);
				return err;
			}
			memcpy(inline_addr, txattr_addr, inline_size);
			set_page_dirty(ipage ? ipage : in_page);
			goto in_page_out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/* write to xattr node block */
	if (F2FS_I(inode)->i_xattr_nid) {
<<<<<<< HEAD
		xpage = get_node_page(sbi, F2FS_I(inode)->i_xattr_nid);
		if (IS_ERR(xpage)) {
			alloc_nid_failed(sbi, new_nid);
			return PTR_ERR(xpage);
		}
		f2fs_bug_on(sbi, new_nid);
		f2fs_wait_on_page_writeback(xpage, NODE, true);
	} else {
		struct dnode_of_data dn;
		set_new_dnode(&dn, inode, NULL, NULL, new_nid);
		xpage = new_node_page(&dn, XATTR_NODE_OFFSET, ipage);
		if (IS_ERR(xpage)) {
			alloc_nid_failed(sbi, new_nid);
			return PTR_ERR(xpage);
		}
		alloc_nid_done(sbi, new_nid);
	}

	xattr_addr = page_address(xpage);
	memcpy(xattr_addr, txattr_addr + inline_size, PAGE_SIZE -
						sizeof(struct node_footer));
	set_page_dirty(xpage);
	f2fs_put_page(xpage, 1);

	/* need to checkpoint during fsync */
	F2FS_I(inode)->xattr_ver = cur_cp_version(F2FS_CKPT(sbi));
	return 0;
=======
		xpage = f2fs_get_node_page(sbi, F2FS_I(inode)->i_xattr_nid);
		if (IS_ERR(xpage)) {
			err = PTR_ERR(xpage);
			f2fs_alloc_nid_failed(sbi, new_nid);
			goto in_page_out;
		}
		f2fs_bug_on(sbi, new_nid);
		f2fs_wait_on_page_writeback(xpage, NODE, true, true);
	} else {
		struct dnode_of_data dn;

		set_new_dnode(&dn, inode, NULL, NULL, new_nid);
		xpage = f2fs_new_node_page(&dn, XATTR_NODE_OFFSET);
		if (IS_ERR(xpage)) {
			err = PTR_ERR(xpage);
			f2fs_alloc_nid_failed(sbi, new_nid);
			goto in_page_out;
		}
		f2fs_alloc_nid_done(sbi, new_nid);
	}
	xattr_addr = page_address(xpage);

	if (inline_size)
		memcpy(inline_addr, txattr_addr, inline_size);
	memcpy(xattr_addr, txattr_addr + inline_size, VALID_XATTR_BLOCK_SIZE);

	if (inline_size)
		set_page_dirty(ipage ? ipage : in_page);
	set_page_dirty(xpage);

	f2fs_put_page(xpage, 1);
in_page_out:
	f2fs_put_page(in_page, 1);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int f2fs_getxattr(struct inode *inode, int index, const char *name,
		void *buffer, size_t buffer_size, struct page *ipage)
{
<<<<<<< HEAD
	struct f2fs_xattr_entry *entry;
	void *base_addr;
	int error = 0;
	size_t size, len;
=======
	struct f2fs_xattr_entry *entry = NULL;
	int error;
	unsigned int size, len;
	void *base_addr = NULL;
	int base_size;
	bool is_inline;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (name == NULL)
		return -EINVAL;

	len = strlen(name);
	if (len > F2FS_NAME_LEN)
		return -ERANGE;

<<<<<<< HEAD
	base_addr = read_all_xattrs(inode, ipage);
	if (!base_addr)
		return -ENOMEM;

	entry = __find_xattr(base_addr, index, len, name);
	if (IS_XATTR_LAST_ENTRY(entry)) {
		error = -ENODATA;
		goto cleanup;
	}
=======
	if (!ipage)
		f2fs_down_read(&F2FS_I(inode)->i_xattr_sem);
	error = lookup_all_xattrs(inode, ipage, index, len, name,
				&entry, &base_addr, &base_size, &is_inline);
	if (!ipage)
		f2fs_up_read(&F2FS_I(inode)->i_xattr_sem);
	if (error)
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	size = le16_to_cpu(entry->e_value_size);

	if (buffer && size > buffer_size) {
		error = -ERANGE;
<<<<<<< HEAD
		goto cleanup;
=======
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (buffer) {
		char *pval = entry->e_name + entry->e_name_len;
<<<<<<< HEAD
		memcpy(buffer, pval, size);
	}
	error = size;

cleanup:
	kzfree(base_addr);
=======

		if (base_size - (pval - (char *)base_addr) < size) {
			error = -ERANGE;
			goto out;
		}
		memcpy(buffer, pval, size);
	}
	error = size;
out:
	xattr_free(F2FS_I_SB(inode), base_addr, is_inline);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

ssize_t f2fs_listxattr(struct dentry *dentry, char *buffer, size_t buffer_size)
{
<<<<<<< HEAD
	struct inode *inode = dentry->d_inode;
	struct f2fs_xattr_entry *entry;
	void *base_addr;
	int error = 0;
	size_t rest = buffer_size;

	base_addr = read_all_xattrs(inode, NULL);
	if (!base_addr)
		return -ENOMEM;

	list_for_each_xattr(entry, base_addr) {
		const struct xattr_handler *handler =
			f2fs_xattr_handler(entry->e_name_index);
		size_t size;

		if (!handler)
			continue;

		size = handler->list(dentry, buffer, rest, entry->e_name,
				entry->e_name_len, handler->flags);
		if (buffer && size > rest) {
			error = -ERANGE;
			goto cleanup;
		}

		if (buffer)
			buffer += size;
=======
	struct inode *inode = d_inode(dentry);
	struct f2fs_xattr_entry *entry;
	void *base_addr, *last_base_addr;
	int error;
	size_t rest = buffer_size;

	f2fs_down_read(&F2FS_I(inode)->i_xattr_sem);
	error = read_all_xattrs(inode, NULL, &base_addr);
	f2fs_up_read(&F2FS_I(inode)->i_xattr_sem);
	if (error)
		return error;

	last_base_addr = (void *)base_addr + XATTR_SIZE(inode);

	list_for_each_xattr(entry, base_addr) {
		const char *prefix;
		size_t prefix_len;
		size_t size;

		prefix = f2fs_xattr_prefix(entry->e_name_index, dentry);

		if ((void *)(entry) + sizeof(__u32) > last_base_addr ||
			(void *)XATTR_NEXT_ENTRY(entry) > last_base_addr) {
			f2fs_err(F2FS_I_SB(inode), "list inode (%lu) has corrupted xattr",
						inode->i_ino);
			set_sbi_flag(F2FS_I_SB(inode), SBI_NEED_FSCK);
			f2fs_handle_error(F2FS_I_SB(inode),
						ERROR_CORRUPTED_XATTR);
			break;
		}

		if (!prefix)
			continue;

		prefix_len = strlen(prefix);
		size = prefix_len + entry->e_name_len + 1;
		if (buffer) {
			if (size > rest) {
				error = -ERANGE;
				goto cleanup;
			}
			memcpy(buffer, prefix, prefix_len);
			buffer += prefix_len;
			memcpy(buffer, entry->e_name, entry->e_name_len);
			buffer += entry->e_name_len;
			*buffer++ = 0;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rest -= size;
	}
	error = buffer_size - rest;
cleanup:
<<<<<<< HEAD
	kzfree(base_addr);
	return error;
}

=======
	kfree(base_addr);
	return error;
}

static bool f2fs_xattr_value_same(struct f2fs_xattr_entry *entry,
					const void *value, size_t size)
{
	void *pval = entry->e_name + entry->e_name_len;

	return (le16_to_cpu(entry->e_value_size) == size) &&
					!memcmp(pval, value, size);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __f2fs_setxattr(struct inode *inode, int index,
			const char *name, const void *value, size_t size,
			struct page *ipage, int flags)
{
<<<<<<< HEAD
	struct f2fs_inode_info *fi = F2FS_I(inode);
	struct f2fs_xattr_entry *here, *last;
	void *base_addr;
	int found, newsize;
	size_t len;
	__u32 new_hsize;
	int error = -ENOMEM;
=======
	struct f2fs_xattr_entry *here, *last;
	void *base_addr, *last_base_addr;
	int found, newsize;
	size_t len;
	__u32 new_hsize;
	int error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (name == NULL)
		return -EINVAL;

	if (value == NULL)
		size = 0;

	len = strlen(name);

	if (len > F2FS_NAME_LEN)
		return -ERANGE;

	if (size > MAX_VALUE_LEN(inode))
		return -E2BIG;
<<<<<<< HEAD

	base_addr = read_all_xattrs(inode, ipage);
	if (!base_addr)
		goto exit;

	/* find entry with wanted name. */
	here = __find_xattr(base_addr, index, len, name);

	found = IS_XATTR_LAST_ENTRY(here) ? 0 : 1;

	if ((flags & XATTR_REPLACE) && !found) {
		error = -ENODATA;
		goto exit;
	} else if ((flags & XATTR_CREATE) && found) {
		error = -EEXIST;
		goto exit;
	}

	last = here;
	while (!IS_XATTR_LAST_ENTRY(last))
		last = XATTR_NEXT_ENTRY(last);
=======
retry:
	error = read_all_xattrs(inode, ipage, &base_addr);
	if (error)
		return error;

	last_base_addr = (void *)base_addr + XATTR_SIZE(inode);

	/* find entry with wanted name. */
	here = __find_xattr(base_addr, last_base_addr, NULL, index, len, name);
	if (!here) {
		if (!F2FS_I(inode)->i_xattr_nid) {
			error = f2fs_recover_xattr_data(inode, NULL);
			f2fs_notice(F2FS_I_SB(inode),
				"recover xattr in inode (%lu), error(%d)",
					inode->i_ino, error);
			if (!error) {
				kfree(base_addr);
				goto retry;
			}
		}
		f2fs_err(F2FS_I_SB(inode), "set inode (%lu) has corrupted xattr",
								inode->i_ino);
		set_sbi_flag(F2FS_I_SB(inode), SBI_NEED_FSCK);
		error = -EFSCORRUPTED;
		f2fs_handle_error(F2FS_I_SB(inode),
					ERROR_CORRUPTED_XATTR);
		goto exit;
	}

	found = IS_XATTR_LAST_ENTRY(here) ? 0 : 1;

	if (found) {
		if ((flags & XATTR_CREATE)) {
			error = -EEXIST;
			goto exit;
		}

		if (value && f2fs_xattr_value_same(here, value, size))
			goto same;
	} else if ((flags & XATTR_REPLACE)) {
		error = -ENODATA;
		goto exit;
	}

	last = here;
	while (!IS_XATTR_LAST_ENTRY(last)) {
		if ((void *)(last) + sizeof(__u32) > last_base_addr ||
			(void *)XATTR_NEXT_ENTRY(last) > last_base_addr) {
			f2fs_err(F2FS_I_SB(inode), "inode (%lu) has invalid last xattr entry, entry_size: %zu",
					inode->i_ino, ENTRY_SIZE(last));
			set_sbi_flag(F2FS_I_SB(inode), SBI_NEED_FSCK);
			error = -EFSCORRUPTED;
			f2fs_handle_error(F2FS_I_SB(inode),
						ERROR_CORRUPTED_XATTR);
			goto exit;
		}
		last = XATTR_NEXT_ENTRY(last);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	newsize = XATTR_ALIGN(sizeof(struct f2fs_xattr_entry) + len + size);

	/* 1. Check space */
	if (value) {
		int free;
		/*
		 * If value is NULL, it is remove operation.
		 * In case of update operation, we calculate free.
		 */
		free = MIN_OFFSET(inode) - ((char *)last - (char *)base_addr);
		if (found)
			free = free + ENTRY_SIZE(here);

		if (unlikely(free < newsize)) {
<<<<<<< HEAD
			error = -ENOSPC;
=======
			error = -E2BIG;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto exit;
		}
	}

	/* 2. Remove old entry */
	if (found) {
		/*
		 * If entry is found, remove old entry.
		 * If not found, remove operation is not needed.
		 */
		struct f2fs_xattr_entry *next = XATTR_NEXT_ENTRY(here);
		int oldsize = ENTRY_SIZE(here);

		memmove(here, next, (char *)last - (char *)next);
		last = (struct f2fs_xattr_entry *)((char *)last - oldsize);
		memset(last, 0, oldsize);
	}

	new_hsize = (char *)last - (char *)base_addr;

	/* 3. Write new entry */
	if (value) {
		char *pval;
		/*
		 * Before we come here, old entry is removed.
		 * We just write new entry.
		 */
<<<<<<< HEAD
		memset(last, 0, newsize);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		last->e_name_index = index;
		last->e_name_len = len;
		memcpy(last->e_name, name, len);
		pval = last->e_name + len;
		memcpy(pval, value, size);
		last->e_value_size = cpu_to_le16(size);
		new_hsize += newsize;
<<<<<<< HEAD
=======
		/*
		 * Explicitly add the null terminator.  The unused xattr space
		 * is supposed to always be zeroed, which would make this
		 * unnecessary, but don't depend on that.
		 */
		*(u32 *)((u8 *)last + newsize) = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	error = write_all_xattrs(inode, new_hsize, base_addr, ipage);
	if (error)
		goto exit;

<<<<<<< HEAD
	if (is_inode_flag_set(fi, FI_ACL_MODE)) {
		inode->i_mode = fi->i_acl_mode;
		inode->i_ctime = CURRENT_TIME;
		clear_inode_flag(fi, FI_ACL_MODE);
	}
	if (index == F2FS_XATTR_INDEX_ENCRYPTION &&
			!strcmp(name, F2FS_XATTR_NAME_ENCRYPTION_CONTEXT))
		f2fs_set_encrypted_inode(inode);

	if (ipage)
		update_inode(inode, ipage);
	else
		update_inode_page(inode);
exit:
	kzfree(base_addr);
=======
	if (index == F2FS_XATTR_INDEX_ENCRYPTION &&
			!strcmp(name, F2FS_XATTR_NAME_ENCRYPTION_CONTEXT))
		f2fs_set_encrypted_inode(inode);
	if (S_ISDIR(inode->i_mode))
		set_sbi_flag(F2FS_I_SB(inode), SBI_NEED_CP);

same:
	if (is_inode_flag_set(inode, FI_ACL_MODE)) {
		inode->i_mode = F2FS_I(inode)->i_acl_mode;
		clear_inode_flag(inode, FI_ACL_MODE);
	}

	inode_set_ctime_current(inode);
	f2fs_mark_inode_dirty_sync(inode, true);
exit:
	kfree(base_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

int f2fs_setxattr(struct inode *inode, int index, const char *name,
				const void *value, size_t size,
				struct page *ipage, int flags)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	int err;

<<<<<<< HEAD
	/* this case is only from init_inode_metadata */
=======
	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;
	if (!f2fs_is_checkpoint_ready(sbi))
		return -ENOSPC;

	err = f2fs_dquot_initialize(inode);
	if (err)
		return err;

	/* this case is only from f2fs_init_inode_metadata */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ipage)
		return __f2fs_setxattr(inode, index, name, value,
						size, ipage, flags);
	f2fs_balance_fs(sbi, true);

	f2fs_lock_op(sbi);
<<<<<<< HEAD
	/* protect xattr_ver */
	down_write(&F2FS_I(inode)->i_sem);
	err = __f2fs_setxattr(inode, index, name, value, size, ipage, flags);
	up_write(&F2FS_I(inode)->i_sem);
=======
	f2fs_down_write(&F2FS_I(inode)->i_xattr_sem);
	err = __f2fs_setxattr(inode, index, name, value, size, ipage, flags);
	f2fs_up_write(&F2FS_I(inode)->i_xattr_sem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_unlock_op(sbi);

	f2fs_update_time(sbi, REQ_TIME);
	return err;
}
<<<<<<< HEAD
=======

int f2fs_init_xattr_caches(struct f2fs_sb_info *sbi)
{
	dev_t dev = sbi->sb->s_bdev->bd_dev;
	char slab_name[32];

	sprintf(slab_name, "f2fs_xattr_entry-%u:%u", MAJOR(dev), MINOR(dev));

	sbi->inline_xattr_slab_size = F2FS_OPTION(sbi).inline_xattr_size *
					sizeof(__le32) + XATTR_PADDING_SIZE;

	sbi->inline_xattr_slab = f2fs_kmem_cache_create(slab_name,
					sbi->inline_xattr_slab_size);
	if (!sbi->inline_xattr_slab)
		return -ENOMEM;

	return 0;
}

void f2fs_destroy_xattr_caches(struct f2fs_sb_info *sbi)
{
	kmem_cache_destroy(sbi->inline_xattr_slab);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
