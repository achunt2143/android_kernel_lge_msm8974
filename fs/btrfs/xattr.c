<<<<<<< HEAD
/*
 * Copyright (C) 2007 Red Hat.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2007 Red Hat.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/rwsem.h>
#include <linux/xattr.h>
#include <linux/security.h>
<<<<<<< HEAD
#include "ctree.h"
=======
#include <linux/posix_acl_xattr.h>
#include <linux/iversion.h>
#include <linux/sched/mm.h>
#include "ctree.h"
#include "fs.h"
#include "messages.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "btrfs_inode.h"
#include "transaction.h"
#include "xattr.h"
#include "disk-io.h"
<<<<<<< HEAD


ssize_t __btrfs_getxattr(struct inode *inode, const char *name,
=======
#include "props.h"
#include "locking.h"
#include "accessors.h"
#include "dir-item.h"

int btrfs_getxattr(struct inode *inode, const char *name,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				void *buffer, size_t size)
{
	struct btrfs_dir_item *di;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	int ret = 0;
	unsigned long data_ptr;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	/* lookup the xattr by name */
<<<<<<< HEAD
	di = btrfs_lookup_xattr(NULL, root, path, btrfs_ino(inode), name,
				strlen(name), 0);
=======
	di = btrfs_lookup_xattr(NULL, root, path, btrfs_ino(BTRFS_I(inode)),
			name, strlen(name), 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!di) {
		ret = -ENODATA;
		goto out;
	} else if (IS_ERR(di)) {
		ret = PTR_ERR(di);
		goto out;
	}

	leaf = path->nodes[0];
	/* if size is 0, that means we want the size of the attr */
	if (!size) {
		ret = btrfs_dir_data_len(leaf, di);
		goto out;
	}

	/* now get the data out of our dir_item */
	if (btrfs_dir_data_len(leaf, di) > size) {
		ret = -ERANGE;
		goto out;
	}

	/*
	 * The way things are packed into the leaf is like this
	 * |struct btrfs_dir_item|name|data|
	 * where name is the xattr name, so security.foo, and data is the
	 * content of the xattr.  data_ptr points to the location in memory
	 * where the data starts in the in memory leaf
	 */
	data_ptr = (unsigned long)((char *)(di + 1) +
				   btrfs_dir_name_len(leaf, di));
	read_extent_buffer(leaf, buffer, data_ptr,
			   btrfs_dir_data_len(leaf, di));
	ret = btrfs_dir_data_len(leaf, di);

out:
	btrfs_free_path(path);
	return ret;
}

<<<<<<< HEAD
static int do_setxattr(struct btrfs_trans_handle *trans,
		       struct inode *inode, const char *name,
		       const void *value, size_t size, int flags)
{
	struct btrfs_dir_item *di;
	struct btrfs_root *root = BTRFS_I(inode)->root;
=======
int btrfs_setxattr(struct btrfs_trans_handle *trans, struct inode *inode,
		   const char *name, const void *value, size_t size, int flags)
{
	struct btrfs_dir_item *di = NULL;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_fs_info *fs_info = root->fs_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct btrfs_path *path;
	size_t name_len = strlen(name);
	int ret = 0;

<<<<<<< HEAD
	if (name_len + size > BTRFS_MAX_XATTR_SIZE(root))
=======
	ASSERT(trans);

	if (name_len + size > BTRFS_MAX_XATTR_SIZE(root->fs_info))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOSPC;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
<<<<<<< HEAD

	if (flags & XATTR_REPLACE) {
		di = btrfs_lookup_xattr(trans, root, path, btrfs_ino(inode), name,
					name_len, -1);
		if (IS_ERR(di)) {
			ret = PTR_ERR(di);
			goto out;
		} else if (!di) {
			ret = -ENODATA;
			goto out;
		}
		ret = btrfs_delete_one_dir_name(trans, root, path, di);
		if (ret)
			goto out;
		btrfs_release_path(path);

		/*
		 * remove the attribute
		 */
		if (!value)
			goto out;
	}

again:
	ret = btrfs_insert_xattr_item(trans, root, path, btrfs_ino(inode),
				      name, name_len, value, size);
	/*
	 * If we're setting an xattr to a new value but the new value is say
	 * exactly BTRFS_MAX_XATTR_SIZE, we could end up with EOVERFLOW getting
	 * back from split_leaf.  This is because it thinks we'll be extending
	 * the existing item size, but we're asking for enough space to add the
	 * item itself.  So if we get EOVERFLOW just set ret to EEXIST and let
	 * the rest of the function figure it out.
	 */
	if (ret == -EOVERFLOW)
		ret = -EEXIST;

	if (ret == -EEXIST) {
		if (flags & XATTR_CREATE)
			goto out;
		/*
		 * We can't use the path we already have since we won't have the
		 * proper locking for a delete, so release the path and
		 * re-lookup to delete the thing.
		 */
		btrfs_release_path(path);
		di = btrfs_lookup_xattr(trans, root, path, btrfs_ino(inode),
					name, name_len, -1);
		if (IS_ERR(di)) {
			ret = PTR_ERR(di);
			goto out;
		} else if (!di) {
			/* Shouldn't happen but just in case... */
			btrfs_release_path(path);
			goto again;
		}

		ret = btrfs_delete_one_dir_name(trans, root, path, di);
		if (ret)
			goto out;

		/*
		 * We have a value to set, so go back and try to insert it now.
		 */
		if (value) {
			btrfs_release_path(path);
			goto again;
		}
	}
out:
	btrfs_free_path(path);
=======
	path->skip_release_on_error = 1;

	if (!value) {
		di = btrfs_lookup_xattr(trans, root, path,
				btrfs_ino(BTRFS_I(inode)), name, name_len, -1);
		if (!di && (flags & XATTR_REPLACE))
			ret = -ENODATA;
		else if (IS_ERR(di))
			ret = PTR_ERR(di);
		else if (di)
			ret = btrfs_delete_one_dir_name(trans, root, path, di);
		goto out;
	}

	/*
	 * For a replace we can't just do the insert blindly.
	 * Do a lookup first (read-only btrfs_search_slot), and return if xattr
	 * doesn't exist. If it exists, fall down below to the insert/replace
	 * path - we can't race with a concurrent xattr delete, because the VFS
	 * locks the inode's i_mutex before calling setxattr or removexattr.
	 */
	if (flags & XATTR_REPLACE) {
		ASSERT(inode_is_locked(inode));
		di = btrfs_lookup_xattr(NULL, root, path,
				btrfs_ino(BTRFS_I(inode)), name, name_len, 0);
		if (!di)
			ret = -ENODATA;
		else if (IS_ERR(di))
			ret = PTR_ERR(di);
		if (ret)
			goto out;
		btrfs_release_path(path);
		di = NULL;
	}

	ret = btrfs_insert_xattr_item(trans, root, path, btrfs_ino(BTRFS_I(inode)),
				      name, name_len, value, size);
	if (ret == -EOVERFLOW) {
		/*
		 * We have an existing item in a leaf, split_leaf couldn't
		 * expand it. That item might have or not a dir_item that
		 * matches our target xattr, so lets check.
		 */
		ret = 0;
		btrfs_assert_tree_write_locked(path->nodes[0]);
		di = btrfs_match_dir_item_name(fs_info, path, name, name_len);
		if (!di && !(flags & XATTR_REPLACE)) {
			ret = -ENOSPC;
			goto out;
		}
	} else if (ret == -EEXIST) {
		ret = 0;
		di = btrfs_match_dir_item_name(fs_info, path, name, name_len);
		ASSERT(di); /* logic error */
	} else if (ret) {
		goto out;
	}

	if (di && (flags & XATTR_CREATE)) {
		ret = -EEXIST;
		goto out;
	}

	if (di) {
		/*
		 * We're doing a replace, and it must be atomic, that is, at
		 * any point in time we have either the old or the new xattr
		 * value in the tree. We don't want readers (getxattr and
		 * listxattrs) to miss a value, this is specially important
		 * for ACLs.
		 */
		const int slot = path->slots[0];
		struct extent_buffer *leaf = path->nodes[0];
		const u16 old_data_len = btrfs_dir_data_len(leaf, di);
		const u32 item_size = btrfs_item_size(leaf, slot);
		const u32 data_size = sizeof(*di) + name_len + size;
		unsigned long data_ptr;
		char *ptr;

		if (size > old_data_len) {
			if (btrfs_leaf_free_space(leaf) <
			    (size - old_data_len)) {
				ret = -ENOSPC;
				goto out;
			}
		}

		if (old_data_len + name_len + sizeof(*di) == item_size) {
			/* No other xattrs packed in the same leaf item. */
			if (size > old_data_len)
				btrfs_extend_item(trans, path, size - old_data_len);
			else if (size < old_data_len)
				btrfs_truncate_item(trans, path, data_size, 1);
		} else {
			/* There are other xattrs packed in the same item. */
			ret = btrfs_delete_one_dir_name(trans, root, path, di);
			if (ret)
				goto out;
			btrfs_extend_item(trans, path, data_size);
		}

		ptr = btrfs_item_ptr(leaf, slot, char);
		ptr += btrfs_item_size(leaf, slot) - data_size;
		di = (struct btrfs_dir_item *)ptr;
		btrfs_set_dir_data_len(leaf, di, size);
		data_ptr = ((unsigned long)(di + 1)) + name_len;
		write_extent_buffer(leaf, value, data_ptr, size);
		btrfs_mark_buffer_dirty(trans, leaf);
	} else {
		/*
		 * Insert, and we had space for the xattr, so path->slots[0] is
		 * where our xattr dir_item is and btrfs_insert_xattr_item()
		 * filled it.
		 */
	}
out:
	btrfs_free_path(path);
	if (!ret) {
		set_bit(BTRFS_INODE_COPY_EVERYTHING,
			&BTRFS_I(inode)->runtime_flags);
		clear_bit(BTRFS_INODE_NO_XATTRS, &BTRFS_I(inode)->runtime_flags);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * @value: "" makes the attribute to empty, NULL removes it
 */
<<<<<<< HEAD
int __btrfs_setxattr(struct btrfs_trans_handle *trans,
		     struct inode *inode, const char *name,
		     const void *value, size_t size, int flags)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	int ret;

	if (trans)
		return do_setxattr(trans, inode, name, value, size, flags);

	trans = btrfs_start_transaction(root, 2);
	if (IS_ERR(trans))
		return PTR_ERR(trans);

	ret = do_setxattr(trans, inode, name, value, size, flags);
	if (ret)
		goto out;

	inode->i_ctime = CURRENT_TIME;
	ret = btrfs_update_inode(trans, root, inode);
	BUG_ON(ret);
out:
	btrfs_end_transaction(trans, root);
=======
int btrfs_setxattr_trans(struct inode *inode, const char *name,
			 const void *value, size_t size, int flags)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_trans_handle *trans;
	const bool start_trans = (current->journal_info == NULL);
	int ret;

	if (start_trans) {
		/*
		 * 1 unit for inserting/updating/deleting the xattr
		 * 1 unit for the inode item update
		 */
		trans = btrfs_start_transaction(root, 2);
		if (IS_ERR(trans))
			return PTR_ERR(trans);
	} else {
		/*
		 * This can happen when smack is enabled and a directory is being
		 * created. It happens through d_instantiate_new(), which calls
		 * smack_d_instantiate(), which in turn calls __vfs_setxattr() to
		 * set the transmute xattr (XATTR_NAME_SMACKTRANSMUTE) on the
		 * inode. We have already reserved space for the xattr and inode
		 * update at btrfs_mkdir(), so just use the transaction handle.
		 * We don't join or start a transaction, as that will reset the
		 * block_rsv of the handle and trigger a warning for the start
		 * case.
		 */
		ASSERT(strncmp(name, XATTR_SECURITY_PREFIX,
			       XATTR_SECURITY_PREFIX_LEN) == 0);
		trans = current->journal_info;
	}

	ret = btrfs_setxattr(trans, inode, name, value, size, flags);
	if (ret)
		goto out;

	inode_inc_iversion(inode);
	inode_set_ctime_current(inode);
	ret = btrfs_update_inode(trans, BTRFS_I(inode));
	if (ret)
		btrfs_abort_transaction(trans, ret);
out:
	if (start_trans)
		btrfs_end_transaction(trans);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

ssize_t btrfs_listxattr(struct dentry *dentry, char *buffer, size_t size)
{
<<<<<<< HEAD
	struct btrfs_key key, found_key;
	struct inode *inode = dentry->d_inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_dir_item *di;
	int ret = 0, slot;
	size_t total_size = 0, size_left = size;
	unsigned long name_ptr;
	size_t name_len;
=======
	struct btrfs_key found_key;
	struct btrfs_key key;
	struct inode *inode = d_inode(dentry);
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_path *path;
	int iter_ret = 0;
	int ret = 0;
	size_t total_size = 0, size_left = size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * ok we want all objects associated with this id.
	 * NOTE: we set key.offset = 0; because we want to start with the
	 * first xattr that we find and walk forward
	 */
<<<<<<< HEAD
	key.objectid = btrfs_ino(inode);
	btrfs_set_key_type(&key, BTRFS_XATTR_ITEM_KEY);
=======
	key.objectid = btrfs_ino(BTRFS_I(inode));
	key.type = BTRFS_XATTR_ITEM_KEY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	key.offset = 0;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
<<<<<<< HEAD
	path->reada = 2;

	/* search for our xattrs */
	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
	if (ret < 0)
		goto err;

	while (1) {
		leaf = path->nodes[0];
		slot = path->slots[0];

		/* this is where we start walking through the path */
		if (slot >= btrfs_header_nritems(leaf)) {
			/*
			 * if we've reached the last slot in this leaf we need
			 * to go to the next leaf and reset everything
			 */
			ret = btrfs_next_leaf(root, path);
			if (ret < 0)
				goto err;
			else if (ret > 0)
				break;
			continue;
		}

		btrfs_item_key_to_cpu(leaf, &found_key, slot);

		/* check to make sure this item is what we want */
		if (found_key.objectid != key.objectid)
			break;
		if (btrfs_key_type(&found_key) != BTRFS_XATTR_ITEM_KEY)
			break;

		di = btrfs_item_ptr(leaf, slot, struct btrfs_dir_item);
		if (verify_dir_item(root, leaf, di))
			continue;

		name_len = btrfs_dir_name_len(leaf, di);
		total_size += name_len + 1;

		/* we are just looking for how big our buffer needs to be */
		if (!size)
			goto next;

		if (!buffer || (name_len + 1) > size_left) {
			ret = -ERANGE;
			goto err;
		}

		name_ptr = (unsigned long)(di + 1);
		read_extent_buffer(leaf, buffer, name_ptr, name_len);
		buffer[name_len] = '\0';

		size_left -= name_len + 1;
		buffer += name_len + 1;
next:
		path->slots[0]++;
	}
	ret = total_size;

err:
=======
	path->reada = READA_FORWARD;

	/* search for our xattrs */
	btrfs_for_each_slot(root, &key, &found_key, path, iter_ret) {
		struct extent_buffer *leaf;
		int slot;
		struct btrfs_dir_item *di;
		u32 item_size;
		u32 cur;

		leaf = path->nodes[0];
		slot = path->slots[0];

		/* check to make sure this item is what we want */
		if (found_key.objectid != key.objectid)
			break;
		if (found_key.type > BTRFS_XATTR_ITEM_KEY)
			break;
		if (found_key.type < BTRFS_XATTR_ITEM_KEY)
			continue;

		di = btrfs_item_ptr(leaf, slot, struct btrfs_dir_item);
		item_size = btrfs_item_size(leaf, slot);
		cur = 0;
		while (cur < item_size) {
			u16 name_len = btrfs_dir_name_len(leaf, di);
			u16 data_len = btrfs_dir_data_len(leaf, di);
			u32 this_len = sizeof(*di) + name_len + data_len;
			unsigned long name_ptr = (unsigned long)(di + 1);

			total_size += name_len + 1;
			/*
			 * We are just looking for how big our buffer needs to
			 * be.
			 */
			if (!size)
				goto next;

			if (!buffer || (name_len + 1) > size_left) {
			        iter_ret = -ERANGE;
				break;
			}

			read_extent_buffer(leaf, buffer, name_ptr, name_len);
			buffer[name_len] = '\0';

			size_left -= name_len + 1;
			buffer += name_len + 1;
next:
			cur += this_len;
			di = (struct btrfs_dir_item *)((char *)di + this_len);
		}
	}

	if (iter_ret < 0)
		ret = iter_ret;
	else
		ret = total_size;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	btrfs_free_path(path);

	return ret;
}

<<<<<<< HEAD
/*
 * List of handlers for synthetic system.* attributes.  All real ondisk
 * attributes are handled directly.
 */
const struct xattr_handler *btrfs_xattr_handlers[] = {
#ifdef CONFIG_BTRFS_FS_POSIX_ACL
	&btrfs_xattr_acl_access_handler,
	&btrfs_xattr_acl_default_handler,
#endif
	NULL,
};

/*
 * Check if the attribute is in a supported namespace.
 *
 * This is applied after the check for the synthetic attributes in the system
 * namespace.
 */
static int btrfs_is_valid_xattr(const char *name)
{
	int len = strlen(name);
	int prefixlen = 0;

	if (!strncmp(name, XATTR_SECURITY_PREFIX,
			XATTR_SECURITY_PREFIX_LEN))
		prefixlen = XATTR_SECURITY_PREFIX_LEN;
	else if (!strncmp(name, XATTR_SYSTEM_PREFIX, XATTR_SYSTEM_PREFIX_LEN))
		prefixlen = XATTR_SYSTEM_PREFIX_LEN;
	else if (!strncmp(name, XATTR_TRUSTED_PREFIX, XATTR_TRUSTED_PREFIX_LEN))
		prefixlen = XATTR_TRUSTED_PREFIX_LEN;
	else if (!strncmp(name, XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN))
		prefixlen = XATTR_USER_PREFIX_LEN;
	else
		return -EOPNOTSUPP;

	/*
	 * The name cannot consist of just prefix
	 */
	if (len <= prefixlen)
		return -EINVAL;

	return 0;
}

ssize_t btrfs_getxattr(struct dentry *dentry, const char *name,
		       void *buffer, size_t size)
{
	int ret;

	/*
	 * If this is a request for a synthetic attribute in the system.*
	 * namespace use the generic infrastructure to resolve a handler
	 * for it via sb->s_xattr.
	 */
	if (!strncmp(name, XATTR_SYSTEM_PREFIX, XATTR_SYSTEM_PREFIX_LEN))
		return generic_getxattr(dentry, name, buffer, size);

	ret = btrfs_is_valid_xattr(name);
	if (ret)
		return ret;
	return __btrfs_getxattr(dentry->d_inode, name, buffer, size);
}

int btrfs_setxattr(struct dentry *dentry, const char *name, const void *value,
		   size_t size, int flags)
{
	struct btrfs_root *root = BTRFS_I(dentry->d_inode)->root;
	int ret;

	/*
	 * The permission on security.* and system.* is not checked
	 * in permission().
	 */
	if (btrfs_root_readonly(root))
		return -EROFS;

	/*
	 * If this is a request for a synthetic attribute in the system.*
	 * namespace use the generic infrastructure to resolve a handler
	 * for it via sb->s_xattr.
	 */
	if (!strncmp(name, XATTR_SYSTEM_PREFIX, XATTR_SYSTEM_PREFIX_LEN))
		return generic_setxattr(dentry, name, value, size, flags);

	ret = btrfs_is_valid_xattr(name);
	if (ret)
		return ret;

	if (size == 0)
		value = "";  /* empty EA, do not remove */

	return __btrfs_setxattr(NULL, dentry->d_inode, name, value, size,
				flags);
}

int btrfs_removexattr(struct dentry *dentry, const char *name)
{
	struct btrfs_root *root = BTRFS_I(dentry->d_inode)->root;
	int ret;

	/*
	 * The permission on security.* and system.* is not checked
	 * in permission().
	 */
	if (btrfs_root_readonly(root))
		return -EROFS;

	/*
	 * If this is a request for a synthetic attribute in the system.*
	 * namespace use the generic infrastructure to resolve a handler
	 * for it via sb->s_xattr.
	 */
	if (!strncmp(name, XATTR_SYSTEM_PREFIX, XATTR_SYSTEM_PREFIX_LEN))
		return generic_removexattr(dentry, name);

	ret = btrfs_is_valid_xattr(name);
	if (ret)
		return ret;

	return __btrfs_setxattr(NULL, dentry->d_inode, name, NULL, 0,
				XATTR_REPLACE);
}

int btrfs_initxattrs(struct inode *inode, const struct xattr *xattr_array,
		     void *fs_info)
{
	const struct xattr *xattr;
	struct btrfs_trans_handle *trans = fs_info;
	char *name;
	int err = 0;

	for (xattr = xattr_array; xattr->name != NULL; xattr++) {
		name = kmalloc(XATTR_SECURITY_PREFIX_LEN +
			       strlen(xattr->name) + 1, GFP_NOFS);
=======
static int btrfs_xattr_handler_get(const struct xattr_handler *handler,
				   struct dentry *unused, struct inode *inode,
				   const char *name, void *buffer, size_t size)
{
	name = xattr_full_name(handler, name);
	return btrfs_getxattr(inode, name, buffer, size);
}

static int btrfs_xattr_handler_set(const struct xattr_handler *handler,
				   struct mnt_idmap *idmap,
				   struct dentry *unused, struct inode *inode,
				   const char *name, const void *buffer,
				   size_t size, int flags)
{
	if (btrfs_root_readonly(BTRFS_I(inode)->root))
		return -EROFS;

	name = xattr_full_name(handler, name);
	return btrfs_setxattr_trans(inode, name, buffer, size, flags);
}

static int btrfs_xattr_handler_get_security(const struct xattr_handler *handler,
					    struct dentry *unused,
					    struct inode *inode,
					    const char *name, void *buffer,
					    size_t size)
{
	int ret;
	bool is_cap = false;

	name = xattr_full_name(handler, name);

	/*
	 * security.capability doesn't cache the results, so calls into us
	 * constantly to see if there's a capability xattr.  Cache the result
	 * here in order to avoid wasting time doing lookups for xattrs we know
	 * don't exist.
	 */
	if (strcmp(name, XATTR_NAME_CAPS) == 0) {
		is_cap = true;
		if (test_bit(BTRFS_INODE_NO_CAP_XATTR, &BTRFS_I(inode)->runtime_flags))
			return -ENODATA;
	}

	ret = btrfs_getxattr(inode, name, buffer, size);
	if (ret == -ENODATA && is_cap)
		set_bit(BTRFS_INODE_NO_CAP_XATTR, &BTRFS_I(inode)->runtime_flags);
	return ret;
}

static int btrfs_xattr_handler_set_security(const struct xattr_handler *handler,
					    struct mnt_idmap *idmap,
					    struct dentry *unused,
					    struct inode *inode,
					    const char *name,
					    const void *buffer,
					    size_t size, int flags)
{
	if (btrfs_root_readonly(BTRFS_I(inode)->root))
		return -EROFS;

	name = xattr_full_name(handler, name);
	if (strcmp(name, XATTR_NAME_CAPS) == 0)
		clear_bit(BTRFS_INODE_NO_CAP_XATTR, &BTRFS_I(inode)->runtime_flags);

	return btrfs_setxattr_trans(inode, name, buffer, size, flags);
}

static int btrfs_xattr_handler_set_prop(const struct xattr_handler *handler,
					struct mnt_idmap *idmap,
					struct dentry *unused, struct inode *inode,
					const char *name, const void *value,
					size_t size, int flags)
{
	int ret;
	struct btrfs_trans_handle *trans;
	struct btrfs_root *root = BTRFS_I(inode)->root;

	name = xattr_full_name(handler, name);
	ret = btrfs_validate_prop(BTRFS_I(inode), name, value, size);
	if (ret)
		return ret;

	if (btrfs_ignore_prop(BTRFS_I(inode), name))
		return 0;

	trans = btrfs_start_transaction(root, 2);
	if (IS_ERR(trans))
		return PTR_ERR(trans);

	ret = btrfs_set_prop(trans, inode, name, value, size, flags);
	if (!ret) {
		inode_inc_iversion(inode);
		inode_set_ctime_current(inode);
		ret = btrfs_update_inode(trans, BTRFS_I(inode));
		if (ret)
			btrfs_abort_transaction(trans, ret);
	}

	btrfs_end_transaction(trans);

	return ret;
}

static const struct xattr_handler btrfs_security_xattr_handler = {
	.prefix = XATTR_SECURITY_PREFIX,
	.get = btrfs_xattr_handler_get_security,
	.set = btrfs_xattr_handler_set_security,
};

static const struct xattr_handler btrfs_trusted_xattr_handler = {
	.prefix = XATTR_TRUSTED_PREFIX,
	.get = btrfs_xattr_handler_get,
	.set = btrfs_xattr_handler_set,
};

static const struct xattr_handler btrfs_user_xattr_handler = {
	.prefix = XATTR_USER_PREFIX,
	.get = btrfs_xattr_handler_get,
	.set = btrfs_xattr_handler_set,
};

static const struct xattr_handler btrfs_btrfs_xattr_handler = {
	.prefix = XATTR_BTRFS_PREFIX,
	.get = btrfs_xattr_handler_get,
	.set = btrfs_xattr_handler_set_prop,
};

const struct xattr_handler * const btrfs_xattr_handlers[] = {
	&btrfs_security_xattr_handler,
	&btrfs_trusted_xattr_handler,
	&btrfs_user_xattr_handler,
	&btrfs_btrfs_xattr_handler,
	NULL,
};

static int btrfs_initxattrs(struct inode *inode,
			    const struct xattr *xattr_array, void *fs_private)
{
	struct btrfs_trans_handle *trans = fs_private;
	const struct xattr *xattr;
	unsigned int nofs_flag;
	char *name;
	int err = 0;

	/*
	 * We're holding a transaction handle, so use a NOFS memory allocation
	 * context to avoid deadlock if reclaim happens.
	 */
	nofs_flag = memalloc_nofs_save();
	for (xattr = xattr_array; xattr->name != NULL; xattr++) {
		name = kmalloc(XATTR_SECURITY_PREFIX_LEN +
			       strlen(xattr->name) + 1, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!name) {
			err = -ENOMEM;
			break;
		}
		strcpy(name, XATTR_SECURITY_PREFIX);
		strcpy(name + XATTR_SECURITY_PREFIX_LEN, xattr->name);
<<<<<<< HEAD
		err = __btrfs_setxattr(trans, inode, name,
				       xattr->value, xattr->value_len, 0);
=======

		if (strcmp(name, XATTR_NAME_CAPS) == 0)
			clear_bit(BTRFS_INODE_NO_CAP_XATTR, &BTRFS_I(inode)->runtime_flags);

		err = btrfs_setxattr(trans, inode, name, xattr->value,
				     xattr->value_len, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(name);
		if (err < 0)
			break;
	}
<<<<<<< HEAD
=======
	memalloc_nofs_restore(nofs_flag);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

int btrfs_xattr_security_init(struct btrfs_trans_handle *trans,
			      struct inode *inode, struct inode *dir,
			      const struct qstr *qstr)
{
	return security_inode_init_security(inode, dir, qstr,
					    &btrfs_initxattrs, trans);
}
