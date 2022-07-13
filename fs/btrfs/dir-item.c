<<<<<<< HEAD
/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
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
 */

#include "ctree.h"
#include "disk-io.h"
#include "hash.h"
#include "transaction.h"
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 */

#include "messages.h"
#include "ctree.h"
#include "disk-io.h"
#include "transaction.h"
#include "accessors.h"
#include "dir-item.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * insert a name into a directory, doing overflow properly if there is a hash
 * collision.  data_size indicates how big the item inserted should be.  On
 * success a struct btrfs_dir_item pointer is returned, otherwise it is
 * an ERR_PTR.
 *
 * The name is not copied into the dir item, you have to do that yourself.
 */
static struct btrfs_dir_item *insert_with_overflow(struct btrfs_trans_handle
						   *trans,
						   struct btrfs_root *root,
						   struct btrfs_path *path,
						   struct btrfs_key *cpu_key,
						   u32 data_size,
						   const char *name,
						   int name_len)
{
<<<<<<< HEAD
	int ret;
	char *ptr;
	struct btrfs_item *item;
=======
	struct btrfs_fs_info *fs_info = root->fs_info;
	int ret;
	char *ptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct extent_buffer *leaf;

	ret = btrfs_insert_empty_item(trans, root, path, cpu_key, data_size);
	if (ret == -EEXIST) {
		struct btrfs_dir_item *di;
<<<<<<< HEAD
		di = btrfs_match_dir_item_name(root, path, name, name_len);
		if (di)
			return ERR_PTR(-EEXIST);
		btrfs_extend_item(trans, root, path, data_size);
=======
		di = btrfs_match_dir_item_name(fs_info, path, name, name_len);
		if (di)
			return ERR_PTR(-EEXIST);
		btrfs_extend_item(trans, path, data_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (ret < 0)
		return ERR_PTR(ret);
	WARN_ON(ret > 0);
	leaf = path->nodes[0];
<<<<<<< HEAD
	item = btrfs_item_nr(leaf, path->slots[0]);
	ptr = btrfs_item_ptr(leaf, path->slots[0], char);
	BUG_ON(data_size > btrfs_item_size(leaf, item));
	ptr += btrfs_item_size(leaf, item) - data_size;
=======
	ptr = btrfs_item_ptr(leaf, path->slots[0], char);
	ASSERT(data_size <= btrfs_item_size(leaf, path->slots[0]));
	ptr += btrfs_item_size(leaf, path->slots[0]) - data_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (struct btrfs_dir_item *)ptr;
}

/*
 * xattrs work a lot like directories, this inserts an xattr item
 * into the tree
 */
int btrfs_insert_xattr_item(struct btrfs_trans_handle *trans,
			    struct btrfs_root *root,
			    struct btrfs_path *path, u64 objectid,
			    const char *name, u16 name_len,
			    const void *data, u16 data_len)
{
	int ret = 0;
	struct btrfs_dir_item *dir_item;
	unsigned long name_ptr, data_ptr;
	struct btrfs_key key, location;
	struct btrfs_disk_key disk_key;
	struct extent_buffer *leaf;
	u32 data_size;

<<<<<<< HEAD
	BUG_ON(name_len + data_len > BTRFS_MAX_XATTR_SIZE(root));

	key.objectid = objectid;
	btrfs_set_key_type(&key, BTRFS_XATTR_ITEM_KEY);
=======
	if (name_len + data_len > BTRFS_MAX_XATTR_SIZE(root->fs_info))
		return -ENOSPC;

	key.objectid = objectid;
	key.type = BTRFS_XATTR_ITEM_KEY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	key.offset = btrfs_name_hash(name, name_len);

	data_size = sizeof(*dir_item) + name_len + data_len;
	dir_item = insert_with_overflow(trans, root, path, &key, data_size,
					name, name_len);
	if (IS_ERR(dir_item))
		return PTR_ERR(dir_item);
	memset(&location, 0, sizeof(location));

	leaf = path->nodes[0];
	btrfs_cpu_key_to_disk(&disk_key, &location);
	btrfs_set_dir_item_key(leaf, dir_item, &disk_key);
<<<<<<< HEAD
	btrfs_set_dir_type(leaf, dir_item, BTRFS_FT_XATTR);
=======
	btrfs_set_dir_flags(leaf, dir_item, BTRFS_FT_XATTR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	btrfs_set_dir_name_len(leaf, dir_item, name_len);
	btrfs_set_dir_transid(leaf, dir_item, trans->transid);
	btrfs_set_dir_data_len(leaf, dir_item, data_len);
	name_ptr = (unsigned long)(dir_item + 1);
	data_ptr = (unsigned long)((char *)name_ptr + name_len);

	write_extent_buffer(leaf, name, name_ptr, name_len);
	write_extent_buffer(leaf, data, data_ptr, data_len);
<<<<<<< HEAD
	btrfs_mark_buffer_dirty(path->nodes[0]);
=======
	btrfs_mark_buffer_dirty(trans, path->nodes[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/*
 * insert a directory item in the tree, doing all the magic for
 * both indexes. 'dir' indicates which objectid to insert it into,
 * 'location' is the key to stuff into the directory item, 'type' is the
 * type of the inode we're pointing to, and 'index' is the sequence number
 * to use for the second index (if one is created).
 * Will return 0 or -ENOMEM
 */
<<<<<<< HEAD
int btrfs_insert_dir_item(struct btrfs_trans_handle *trans, struct btrfs_root
			  *root, const char *name, int name_len,
			  struct inode *dir, struct btrfs_key *location,
			  u8 type, u64 index)
{
	int ret = 0;
	int ret2 = 0;
=======
int btrfs_insert_dir_item(struct btrfs_trans_handle *trans,
			  const struct fscrypt_str *name, struct btrfs_inode *dir,
			  struct btrfs_key *location, u8 type, u64 index)
{
	int ret = 0;
	int ret2 = 0;
	struct btrfs_root *root = dir->root;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct btrfs_path *path;
	struct btrfs_dir_item *dir_item;
	struct extent_buffer *leaf;
	unsigned long name_ptr;
	struct btrfs_key key;
	struct btrfs_disk_key disk_key;
	u32 data_size;

	key.objectid = btrfs_ino(dir);
<<<<<<< HEAD
	btrfs_set_key_type(&key, BTRFS_DIR_ITEM_KEY);
	key.offset = btrfs_name_hash(name, name_len);
=======
	key.type = BTRFS_DIR_ITEM_KEY;
	key.offset = btrfs_name_hash(name->name, name->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
<<<<<<< HEAD
	path->leave_spinning = 1;

	btrfs_cpu_key_to_disk(&disk_key, location);

	data_size = sizeof(*dir_item) + name_len;
	dir_item = insert_with_overflow(trans, root, path, &key, data_size,
					name, name_len);
=======

	btrfs_cpu_key_to_disk(&disk_key, location);

	data_size = sizeof(*dir_item) + name->len;
	dir_item = insert_with_overflow(trans, root, path, &key, data_size,
					name->name, name->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(dir_item)) {
		ret = PTR_ERR(dir_item);
		if (ret == -EEXIST)
			goto second_insert;
		goto out_free;
	}

<<<<<<< HEAD
	leaf = path->nodes[0];
	btrfs_set_dir_item_key(leaf, dir_item, &disk_key);
	btrfs_set_dir_type(leaf, dir_item, type);
	btrfs_set_dir_data_len(leaf, dir_item, 0);
	btrfs_set_dir_name_len(leaf, dir_item, name_len);
	btrfs_set_dir_transid(leaf, dir_item, trans->transid);
	name_ptr = (unsigned long)(dir_item + 1);

	write_extent_buffer(leaf, name, name_ptr, name_len);
	btrfs_mark_buffer_dirty(leaf);
=======
	if (IS_ENCRYPTED(&dir->vfs_inode))
		type |= BTRFS_FT_ENCRYPTED;

	leaf = path->nodes[0];
	btrfs_set_dir_item_key(leaf, dir_item, &disk_key);
	btrfs_set_dir_flags(leaf, dir_item, type);
	btrfs_set_dir_data_len(leaf, dir_item, 0);
	btrfs_set_dir_name_len(leaf, dir_item, name->len);
	btrfs_set_dir_transid(leaf, dir_item, trans->transid);
	name_ptr = (unsigned long)(dir_item + 1);

	write_extent_buffer(leaf, name->name, name_ptr, name->len);
	btrfs_mark_buffer_dirty(trans, leaf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

second_insert:
	/* FIXME, use some real flag for selecting the extra index */
	if (root == root->fs_info->tree_root) {
		ret = 0;
		goto out_free;
	}
	btrfs_release_path(path);

<<<<<<< HEAD
	ret2 = btrfs_insert_delayed_dir_index(trans, root, name, name_len, dir,
=======
	ret2 = btrfs_insert_delayed_dir_index(trans, name->name, name->len, dir,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					      &disk_key, type, index);
out_free:
	btrfs_free_path(path);
	if (ret)
		return ret;
	if (ret2)
		return ret2;
	return 0;
}

<<<<<<< HEAD
/*
 * lookup a directory item based on name.  'dir' is the objectid
 * we're searching in, and 'mod' tells us if you plan on deleting the
 * item (use mod < 0) or changing the options (use mod > 0)
=======
static struct btrfs_dir_item *btrfs_lookup_match_dir(
			struct btrfs_trans_handle *trans,
			struct btrfs_root *root, struct btrfs_path *path,
			struct btrfs_key *key, const char *name,
			int name_len, int mod)
{
	const int ins_len = (mod < 0 ? -1 : 0);
	const int cow = (mod != 0);
	int ret;

	ret = btrfs_search_slot(trans, root, key, path, ins_len, cow);
	if (ret < 0)
		return ERR_PTR(ret);
	if (ret > 0)
		return ERR_PTR(-ENOENT);

	return btrfs_match_dir_item_name(root->fs_info, path, name, name_len);
}

/*
 * Lookup for a directory item by name.
 *
 * @trans:	The transaction handle to use. Can be NULL if @mod is 0.
 * @root:	The root of the target tree.
 * @path:	Path to use for the search.
 * @dir:	The inode number (objectid) of the directory.
 * @name:	The name associated to the directory entry we are looking for.
 * @name_len:	The length of the name.
 * @mod:	Used to indicate if the tree search is meant for a read only
 *		lookup, for a modification lookup or for a deletion lookup, so
 *		its value should be 0, 1 or -1, respectively.
 *
 * Returns: NULL if the dir item does not exists, an error pointer if an error
 * happened, or a pointer to a dir item if a dir item exists for the given name.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct btrfs_dir_item *btrfs_lookup_dir_item(struct btrfs_trans_handle *trans,
					     struct btrfs_root *root,
					     struct btrfs_path *path, u64 dir,
<<<<<<< HEAD
					     const char *name, int name_len,
					     int mod)
{
	int ret;
	struct btrfs_key key;
	int ins_len = mod < 0 ? -1 : 0;
	int cow = mod != 0;

	key.objectid = dir;
	btrfs_set_key_type(&key, BTRFS_DIR_ITEM_KEY);

	key.offset = btrfs_name_hash(name, name_len);

	ret = btrfs_search_slot(trans, root, &key, path, ins_len, cow);
	if (ret < 0)
		return ERR_PTR(ret);
	if (ret > 0)
		return NULL;

	return btrfs_match_dir_item_name(root, path, name, name_len);
}

/*
 * lookup a directory item based on index.  'dir' is the objectid
 * we're searching in, and 'mod' tells us if you plan on deleting the
 * item (use mod < 0) or changing the options (use mod > 0)
 *
 * The name is used to make sure the index really points to the name you were
 * looking for.
=======
					     const struct fscrypt_str *name,
					     int mod)
{
	struct btrfs_key key;
	struct btrfs_dir_item *di;

	key.objectid = dir;
	key.type = BTRFS_DIR_ITEM_KEY;
	key.offset = btrfs_name_hash(name->name, name->len);

	di = btrfs_lookup_match_dir(trans, root, path, &key, name->name,
				    name->len, mod);
	if (IS_ERR(di) && PTR_ERR(di) == -ENOENT)
		return NULL;

	return di;
}

int btrfs_check_dir_item_collision(struct btrfs_root *root, u64 dir,
				   const struct fscrypt_str *name)
{
	int ret;
	struct btrfs_key key;
	struct btrfs_dir_item *di;
	int data_size;
	struct extent_buffer *leaf;
	int slot;
	struct btrfs_path *path;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = dir;
	key.type = BTRFS_DIR_ITEM_KEY;
	key.offset = btrfs_name_hash(name->name, name->len);

	di = btrfs_lookup_match_dir(NULL, root, path, &key, name->name,
				    name->len, 0);
	if (IS_ERR(di)) {
		ret = PTR_ERR(di);
		/* Nothing found, we're safe */
		if (ret == -ENOENT) {
			ret = 0;
			goto out;
		}

		if (ret < 0)
			goto out;
	}

	/* we found an item, look for our name in the item */
	if (di) {
		/* our exact name was found */
		ret = -EEXIST;
		goto out;
	}

	/* See if there is room in the item to insert this name. */
	data_size = sizeof(*di) + name->len;
	leaf = path->nodes[0];
	slot = path->slots[0];
	if (data_size + btrfs_item_size(leaf, slot) +
	    sizeof(struct btrfs_item) > BTRFS_LEAF_DATA_SIZE(root->fs_info)) {
		ret = -EOVERFLOW;
	} else {
		/* plenty of insertion room */
		ret = 0;
	}
out:
	btrfs_free_path(path);
	return ret;
}

/*
 * Lookup for a directory index item by name and index number.
 *
 * @trans:	The transaction handle to use. Can be NULL if @mod is 0.
 * @root:	The root of the target tree.
 * @path:	Path to use for the search.
 * @dir:	The inode number (objectid) of the directory.
 * @index:	The index number.
 * @name:	The name associated to the directory entry we are looking for.
 * @name_len:	The length of the name.
 * @mod:	Used to indicate if the tree search is meant for a read only
 *		lookup, for a modification lookup or for a deletion lookup, so
 *		its value should be 0, 1 or -1, respectively.
 *
 * Returns: NULL if the dir index item does not exists, an error pointer if an
 * error happened, or a pointer to a dir item if the dir index item exists and
 * matches the criteria (name and index number).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct btrfs_dir_item *
btrfs_lookup_dir_index_item(struct btrfs_trans_handle *trans,
			    struct btrfs_root *root,
			    struct btrfs_path *path, u64 dir,
<<<<<<< HEAD
			    u64 objectid, const char *name, int name_len,
			    int mod)
{
	int ret;
	struct btrfs_key key;
	int ins_len = mod < 0 ? -1 : 0;
	int cow = mod != 0;

	key.objectid = dir;
	btrfs_set_key_type(&key, BTRFS_DIR_INDEX_KEY);
	key.offset = objectid;

	ret = btrfs_search_slot(trans, root, &key, path, ins_len, cow);
	if (ret < 0)
		return ERR_PTR(ret);
	if (ret > 0)
		return ERR_PTR(-ENOENT);
	return btrfs_match_dir_item_name(root, path, name, name_len);
}

struct btrfs_dir_item *
btrfs_search_dir_index_item(struct btrfs_root *root,
			    struct btrfs_path *path, u64 dirid,
			    const char *name, int name_len)
{
	struct extent_buffer *leaf;
	struct btrfs_dir_item *di;
	struct btrfs_key key;
	u32 nritems;
=======
			    u64 index, const struct fscrypt_str *name, int mod)
{
	struct btrfs_dir_item *di;
	struct btrfs_key key;

	key.objectid = dir;
	key.type = BTRFS_DIR_INDEX_KEY;
	key.offset = index;

	di = btrfs_lookup_match_dir(trans, root, path, &key, name->name,
				    name->len, mod);
	if (di == ERR_PTR(-ENOENT))
		return NULL;

	return di;
}

struct btrfs_dir_item *
btrfs_search_dir_index_item(struct btrfs_root *root, struct btrfs_path *path,
			    u64 dirid, const struct fscrypt_str *name)
{
	struct btrfs_dir_item *di;
	struct btrfs_key key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	key.objectid = dirid;
	key.type = BTRFS_DIR_INDEX_KEY;
	key.offset = 0;

<<<<<<< HEAD
	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
	if (ret < 0)
		return ERR_PTR(ret);

	leaf = path->nodes[0];
	nritems = btrfs_header_nritems(leaf);

	while (1) {
		if (path->slots[0] >= nritems) {
			ret = btrfs_next_leaf(root, path);
			if (ret < 0)
				return ERR_PTR(ret);
			if (ret > 0)
				break;
			leaf = path->nodes[0];
			nritems = btrfs_header_nritems(leaf);
			continue;
		}

		btrfs_item_key_to_cpu(leaf, &key, path->slots[0]);
		if (key.objectid != dirid || key.type != BTRFS_DIR_INDEX_KEY)
			break;

		di = btrfs_match_dir_item_name(root, path, name, name_len);
		if (di)
			return di;

		path->slots[0]++;
	}
	return NULL;
=======
	btrfs_for_each_slot(root, &key, &key, path, ret) {
		if (key.objectid != dirid || key.type != BTRFS_DIR_INDEX_KEY)
			break;

		di = btrfs_match_dir_item_name(root->fs_info, path,
					       name->name, name->len);
		if (di)
			return di;
	}
	/* Adjust return code if the key was not found in the next leaf. */
	if (ret > 0)
		ret = 0;

	return ERR_PTR(ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct btrfs_dir_item *btrfs_lookup_xattr(struct btrfs_trans_handle *trans,
					  struct btrfs_root *root,
					  struct btrfs_path *path, u64 dir,
					  const char *name, u16 name_len,
					  int mod)
{
<<<<<<< HEAD
	int ret;
	struct btrfs_key key;
	int ins_len = mod < 0 ? -1 : 0;
	int cow = mod != 0;

	key.objectid = dir;
	btrfs_set_key_type(&key, BTRFS_XATTR_ITEM_KEY);
	key.offset = btrfs_name_hash(name, name_len);
	ret = btrfs_search_slot(trans, root, &key, path, ins_len, cow);
	if (ret < 0)
		return ERR_PTR(ret);
	if (ret > 0)
		return NULL;

	return btrfs_match_dir_item_name(root, path, name, name_len);
=======
	struct btrfs_key key;
	struct btrfs_dir_item *di;

	key.objectid = dir;
	key.type = BTRFS_XATTR_ITEM_KEY;
	key.offset = btrfs_name_hash(name, name_len);

	di = btrfs_lookup_match_dir(trans, root, path, &key, name, name_len, mod);
	if (IS_ERR(di) && PTR_ERR(di) == -ENOENT)
		return NULL;

	return di;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * helper function to look at the directory item pointed to by 'path'
 * this walks through all the entries in a dir item and finds one
 * for a specific name.
 */
<<<<<<< HEAD
struct btrfs_dir_item *btrfs_match_dir_item_name(struct btrfs_root *root,
			      struct btrfs_path *path,
			      const char *name, int name_len)
=======
struct btrfs_dir_item *btrfs_match_dir_item_name(struct btrfs_fs_info *fs_info,
						 struct btrfs_path *path,
						 const char *name, int name_len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct btrfs_dir_item *dir_item;
	unsigned long name_ptr;
	u32 total_len;
	u32 cur = 0;
	u32 this_len;
	struct extent_buffer *leaf;

	leaf = path->nodes[0];
	dir_item = btrfs_item_ptr(leaf, path->slots[0], struct btrfs_dir_item);
<<<<<<< HEAD
	if (verify_dir_item(root, leaf, dir_item))
		return NULL;

	total_len = btrfs_item_size_nr(leaf, path->slots[0]);
=======

	total_len = btrfs_item_size(leaf, path->slots[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (cur < total_len) {
		this_len = sizeof(*dir_item) +
			btrfs_dir_name_len(leaf, dir_item) +
			btrfs_dir_data_len(leaf, dir_item);
		name_ptr = (unsigned long)(dir_item + 1);

		if (btrfs_dir_name_len(leaf, dir_item) == name_len &&
		    memcmp_extent_buffer(leaf, name, name_ptr, name_len) == 0)
			return dir_item;

		cur += this_len;
		dir_item = (struct btrfs_dir_item *)((char *)dir_item +
						     this_len);
	}
	return NULL;
}

/*
 * given a pointer into a directory item, delete it.  This
 * handles items that have more than one entry in them.
 */
int btrfs_delete_one_dir_name(struct btrfs_trans_handle *trans,
			      struct btrfs_root *root,
			      struct btrfs_path *path,
			      struct btrfs_dir_item *di)
{

	struct extent_buffer *leaf;
	u32 sub_item_len;
	u32 item_len;
	int ret = 0;

	leaf = path->nodes[0];
	sub_item_len = sizeof(*di) + btrfs_dir_name_len(leaf, di) +
		btrfs_dir_data_len(leaf, di);
<<<<<<< HEAD
	item_len = btrfs_item_size_nr(leaf, path->slots[0]);
=======
	item_len = btrfs_item_size(leaf, path->slots[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (sub_item_len == item_len) {
		ret = btrfs_del_item(trans, root, path);
	} else {
		/* MARKER */
		unsigned long ptr = (unsigned long)di;
		unsigned long start;

		start = btrfs_item_ptr_offset(leaf, path->slots[0]);
		memmove_extent_buffer(leaf, ptr, ptr + sub_item_len,
			item_len - (ptr + sub_item_len - start));
<<<<<<< HEAD
		btrfs_truncate_item(trans, root, path,
				    item_len - sub_item_len, 1);
	}
	return ret;
}

int verify_dir_item(struct btrfs_root *root,
		    struct extent_buffer *leaf,
		    struct btrfs_dir_item *dir_item)
{
	u16 namelen = BTRFS_NAME_LEN;
	u8 type = btrfs_dir_type(leaf, dir_item);

	if (type >= BTRFS_FT_MAX) {
		printk(KERN_CRIT "btrfs: invalid dir item type: %d\n",
		       (int)type);
		return 1;
	}

	if (type == BTRFS_FT_XATTR)
		namelen = XATTR_NAME_MAX;

	if (btrfs_dir_name_len(leaf, dir_item) > namelen) {
		printk(KERN_CRIT "btrfs: invalid dir item name len: %u\n",
		       (unsigned)btrfs_dir_data_len(leaf, dir_item));
		return 1;
	}

	/* BTRFS_MAX_XATTR_SIZE is the same for all dir items */
	if (btrfs_dir_data_len(leaf, dir_item) > BTRFS_MAX_XATTR_SIZE(root)) {
		printk(KERN_CRIT "btrfs: invalid dir item data len: %u\n",
		       (unsigned)btrfs_dir_data_len(leaf, dir_item));
		return 1;
	}

	return 0;
}
=======
		btrfs_truncate_item(trans, path, item_len - sub_item_len, 1);
	}
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
