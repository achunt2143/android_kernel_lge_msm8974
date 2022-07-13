<<<<<<< HEAD
/*
 * Copyright (C) 2011 STRATO.  All rights reserved.
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

=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2011 STRATO.  All rights reserved.
 */

#include <linux/mm.h>
#include <linux/rbtree.h>
#include <trace/events/btrfs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "ctree.h"
#include "disk-io.h"
#include "backref.h"
#include "ulist.h"
#include "transaction.h"
#include "delayed-ref.h"
#include "locking.h"
<<<<<<< HEAD

/*
 * this structure records all encountered refs on the way up to the root
 */
struct __prelim_ref {
	struct list_head list;
	u64 root_id;
	struct btrfs_key key;
	int level;
	int count;
	u64 parent;
	u64 wanted_disk_byte;
};

static int __add_prelim_ref(struct list_head *head, u64 root_id,
			    struct btrfs_key *key, int level, u64 parent,
			    u64 wanted_disk_byte, int count)
{
	struct __prelim_ref *ref;

	/* in case we're adding delayed refs, we're holding the refs spinlock */
	ref = kmalloc(sizeof(*ref), GFP_ATOMIC);
=======
#include "misc.h"
#include "tree-mod-log.h"
#include "fs.h"
#include "accessors.h"
#include "extent-tree.h"
#include "relocation.h"
#include "tree-checker.h"

/* Just arbitrary numbers so we can be sure one of these happened. */
#define BACKREF_FOUND_SHARED     6
#define BACKREF_FOUND_NOT_SHARED 7

struct extent_inode_elem {
	u64 inum;
	u64 offset;
	u64 num_bytes;
	struct extent_inode_elem *next;
};

static int check_extent_in_eb(struct btrfs_backref_walk_ctx *ctx,
			      const struct btrfs_key *key,
			      const struct extent_buffer *eb,
			      const struct btrfs_file_extent_item *fi,
			      struct extent_inode_elem **eie)
{
	const u64 data_len = btrfs_file_extent_num_bytes(eb, fi);
	u64 offset = key->offset;
	struct extent_inode_elem *e;
	const u64 *root_ids;
	int root_count;
	bool cached;

	if (!ctx->ignore_extent_item_pos &&
	    !btrfs_file_extent_compression(eb, fi) &&
	    !btrfs_file_extent_encryption(eb, fi) &&
	    !btrfs_file_extent_other_encoding(eb, fi)) {
		u64 data_offset;

		data_offset = btrfs_file_extent_offset(eb, fi);

		if (ctx->extent_item_pos < data_offset ||
		    ctx->extent_item_pos >= data_offset + data_len)
			return 1;
		offset += ctx->extent_item_pos - data_offset;
	}

	if (!ctx->indirect_ref_iterator || !ctx->cache_lookup)
		goto add_inode_elem;

	cached = ctx->cache_lookup(eb->start, ctx->user_ctx, &root_ids,
				   &root_count);
	if (!cached)
		goto add_inode_elem;

	for (int i = 0; i < root_count; i++) {
		int ret;

		ret = ctx->indirect_ref_iterator(key->objectid, offset,
						 data_len, root_ids[i],
						 ctx->user_ctx);
		if (ret)
			return ret;
	}

add_inode_elem:
	e = kmalloc(sizeof(*e), GFP_NOFS);
	if (!e)
		return -ENOMEM;

	e->next = *eie;
	e->inum = key->objectid;
	e->offset = offset;
	e->num_bytes = data_len;
	*eie = e;

	return 0;
}

static void free_inode_elem_list(struct extent_inode_elem *eie)
{
	struct extent_inode_elem *eie_next;

	for (; eie; eie = eie_next) {
		eie_next = eie->next;
		kfree(eie);
	}
}

static int find_extent_in_eb(struct btrfs_backref_walk_ctx *ctx,
			     const struct extent_buffer *eb,
			     struct extent_inode_elem **eie)
{
	u64 disk_byte;
	struct btrfs_key key;
	struct btrfs_file_extent_item *fi;
	int slot;
	int nritems;
	int extent_type;
	int ret;

	/*
	 * from the shared data ref, we only have the leaf but we need
	 * the key. thus, we must look into all items and see that we
	 * find one (some) with a reference to our extent item.
	 */
	nritems = btrfs_header_nritems(eb);
	for (slot = 0; slot < nritems; ++slot) {
		btrfs_item_key_to_cpu(eb, &key, slot);
		if (key.type != BTRFS_EXTENT_DATA_KEY)
			continue;
		fi = btrfs_item_ptr(eb, slot, struct btrfs_file_extent_item);
		extent_type = btrfs_file_extent_type(eb, fi);
		if (extent_type == BTRFS_FILE_EXTENT_INLINE)
			continue;
		/* don't skip BTRFS_FILE_EXTENT_PREALLOC, we can handle that */
		disk_byte = btrfs_file_extent_disk_bytenr(eb, fi);
		if (disk_byte != ctx->bytenr)
			continue;

		ret = check_extent_in_eb(ctx, &key, eb, fi, eie);
		if (ret == BTRFS_ITERATE_EXTENT_INODES_STOP || ret < 0)
			return ret;
	}

	return 0;
}

struct preftree {
	struct rb_root_cached root;
	unsigned int count;
};

#define PREFTREE_INIT	{ .root = RB_ROOT_CACHED, .count = 0 }

struct preftrees {
	struct preftree direct;    /* BTRFS_SHARED_[DATA|BLOCK]_REF_KEY */
	struct preftree indirect;  /* BTRFS_[TREE_BLOCK|EXTENT_DATA]_REF_KEY */
	struct preftree indirect_missing_keys;
};

/*
 * Checks for a shared extent during backref search.
 *
 * The share_count tracks prelim_refs (direct and indirect) having a
 * ref->count >0:
 *  - incremented when a ref->count transitions to >0
 *  - decremented when a ref->count transitions to <1
 */
struct share_check {
	struct btrfs_backref_share_check_ctx *ctx;
	struct btrfs_root *root;
	u64 inum;
	u64 data_bytenr;
	u64 data_extent_gen;
	/*
	 * Counts number of inodes that refer to an extent (different inodes in
	 * the same root or different roots) that we could find. The sharedness
	 * check typically stops once this counter gets greater than 1, so it
	 * may not reflect the total number of inodes.
	 */
	int share_count;
	/*
	 * The number of times we found our inode refers to the data extent we
	 * are determining the sharedness. In other words, how many file extent
	 * items we could find for our inode that point to our target data
	 * extent. The value we get here after finishing the extent sharedness
	 * check may be smaller than reality, but if it ends up being greater
	 * than 1, then we know for sure the inode has multiple file extent
	 * items that point to our inode, and we can safely assume it's useful
	 * to cache the sharedness check result.
	 */
	int self_ref_count;
	bool have_delayed_delete_refs;
};

static inline int extent_is_shared(struct share_check *sc)
{
	return (sc && sc->share_count > 1) ? BACKREF_FOUND_SHARED : 0;
}

static struct kmem_cache *btrfs_prelim_ref_cache;

int __init btrfs_prelim_ref_init(void)
{
	btrfs_prelim_ref_cache = kmem_cache_create("btrfs_prelim_ref",
					sizeof(struct prelim_ref), 0, 0, NULL);
	if (!btrfs_prelim_ref_cache)
		return -ENOMEM;
	return 0;
}

void __cold btrfs_prelim_ref_exit(void)
{
	kmem_cache_destroy(btrfs_prelim_ref_cache);
}

static void free_pref(struct prelim_ref *ref)
{
	kmem_cache_free(btrfs_prelim_ref_cache, ref);
}

/*
 * Return 0 when both refs are for the same block (and can be merged).
 * A -1 return indicates ref1 is a 'lower' block than ref2, while 1
 * indicates a 'higher' block.
 */
static int prelim_ref_compare(struct prelim_ref *ref1,
			      struct prelim_ref *ref2)
{
	if (ref1->level < ref2->level)
		return -1;
	if (ref1->level > ref2->level)
		return 1;
	if (ref1->root_id < ref2->root_id)
		return -1;
	if (ref1->root_id > ref2->root_id)
		return 1;
	if (ref1->key_for_search.type < ref2->key_for_search.type)
		return -1;
	if (ref1->key_for_search.type > ref2->key_for_search.type)
		return 1;
	if (ref1->key_for_search.objectid < ref2->key_for_search.objectid)
		return -1;
	if (ref1->key_for_search.objectid > ref2->key_for_search.objectid)
		return 1;
	if (ref1->key_for_search.offset < ref2->key_for_search.offset)
		return -1;
	if (ref1->key_for_search.offset > ref2->key_for_search.offset)
		return 1;
	if (ref1->parent < ref2->parent)
		return -1;
	if (ref1->parent > ref2->parent)
		return 1;

	return 0;
}

static void update_share_count(struct share_check *sc, int oldcount,
			       int newcount, struct prelim_ref *newref)
{
	if ((!sc) || (oldcount == 0 && newcount < 1))
		return;

	if (oldcount > 0 && newcount < 1)
		sc->share_count--;
	else if (oldcount < 1 && newcount > 0)
		sc->share_count++;

	if (newref->root_id == sc->root->root_key.objectid &&
	    newref->wanted_disk_byte == sc->data_bytenr &&
	    newref->key_for_search.objectid == sc->inum)
		sc->self_ref_count += newref->count;
}

/*
 * Add @newref to the @root rbtree, merging identical refs.
 *
 * Callers should assume that newref has been freed after calling.
 */
static void prelim_ref_insert(const struct btrfs_fs_info *fs_info,
			      struct preftree *preftree,
			      struct prelim_ref *newref,
			      struct share_check *sc)
{
	struct rb_root_cached *root;
	struct rb_node **p;
	struct rb_node *parent = NULL;
	struct prelim_ref *ref;
	int result;
	bool leftmost = true;

	root = &preftree->root;
	p = &root->rb_root.rb_node;

	while (*p) {
		parent = *p;
		ref = rb_entry(parent, struct prelim_ref, rbnode);
		result = prelim_ref_compare(ref, newref);
		if (result < 0) {
			p = &(*p)->rb_left;
		} else if (result > 0) {
			p = &(*p)->rb_right;
			leftmost = false;
		} else {
			/* Identical refs, merge them and free @newref */
			struct extent_inode_elem *eie = ref->inode_list;

			while (eie && eie->next)
				eie = eie->next;

			if (!eie)
				ref->inode_list = newref->inode_list;
			else
				eie->next = newref->inode_list;
			trace_btrfs_prelim_ref_merge(fs_info, ref, newref,
						     preftree->count);
			/*
			 * A delayed ref can have newref->count < 0.
			 * The ref->count is updated to follow any
			 * BTRFS_[ADD|DROP]_DELAYED_REF actions.
			 */
			update_share_count(sc, ref->count,
					   ref->count + newref->count, newref);
			ref->count += newref->count;
			free_pref(newref);
			return;
		}
	}

	update_share_count(sc, 0, newref->count, newref);
	preftree->count++;
	trace_btrfs_prelim_ref_insert(fs_info, newref, NULL, preftree->count);
	rb_link_node(&newref->rbnode, parent, p);
	rb_insert_color_cached(&newref->rbnode, root, leftmost);
}

/*
 * Release the entire tree.  We don't care about internal consistency so
 * just free everything and then reset the tree root.
 */
static void prelim_release(struct preftree *preftree)
{
	struct prelim_ref *ref, *next_ref;

	rbtree_postorder_for_each_entry_safe(ref, next_ref,
					     &preftree->root.rb_root, rbnode) {
		free_inode_elem_list(ref->inode_list);
		free_pref(ref);
	}

	preftree->root = RB_ROOT_CACHED;
	preftree->count = 0;
}

/*
 * the rules for all callers of this function are:
 * - obtaining the parent is the goal
 * - if you add a key, you must know that it is a correct key
 * - if you cannot add the parent or a correct key, then we will look into the
 *   block later to set a correct key
 *
 * delayed refs
 * ============
 *        backref type | shared | indirect | shared | indirect
 * information         |   tree |     tree |   data |     data
 * --------------------+--------+----------+--------+----------
 *      parent logical |    y   |     -    |    -   |     -
 *      key to resolve |    -   |     y    |    y   |     y
 *  tree block logical |    -   |     -    |    -   |     -
 *  root for resolving |    y   |     y    |    y   |     y
 *
 * - column 1:       we've the parent -> done
 * - column 2, 3, 4: we use the key to find the parent
 *
 * on disk refs (inline or keyed)
 * ==============================
 *        backref type | shared | indirect | shared | indirect
 * information         |   tree |     tree |   data |     data
 * --------------------+--------+----------+--------+----------
 *      parent logical |    y   |     -    |    y   |     -
 *      key to resolve |    -   |     -    |    -   |     y
 *  tree block logical |    y   |     y    |    y   |     y
 *  root for resolving |    -   |     y    |    y   |     y
 *
 * - column 1, 3: we've the parent -> done
 * - column 2:    we take the first key from the block to find the parent
 *                (see add_missing_keys)
 * - column 4:    we use the key to find the parent
 *
 * additional information that's available but not required to find the parent
 * block might help in merging entries to gain some speed.
 */
static int add_prelim_ref(const struct btrfs_fs_info *fs_info,
			  struct preftree *preftree, u64 root_id,
			  const struct btrfs_key *key, int level, u64 parent,
			  u64 wanted_disk_byte, int count,
			  struct share_check *sc, gfp_t gfp_mask)
{
	struct prelim_ref *ref;

	if (root_id == BTRFS_DATA_RELOC_TREE_OBJECTID)
		return 0;

	ref = kmem_cache_alloc(btrfs_prelim_ref_cache, gfp_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ref)
		return -ENOMEM;

	ref->root_id = root_id;
	if (key)
<<<<<<< HEAD
		ref->key = *key;
	else
		memset(&ref->key, 0, sizeof(ref->key));

=======
		ref->key_for_search = *key;
	else
		memset(&ref->key_for_search, 0, sizeof(ref->key_for_search));

	ref->inode_list = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ref->level = level;
	ref->count = count;
	ref->parent = parent;
	ref->wanted_disk_byte = wanted_disk_byte;
<<<<<<< HEAD
	list_add_tail(&ref->list, head);

	return 0;
}

static int add_all_parents(struct btrfs_root *root, struct btrfs_path *path,
				struct ulist *parents,
				struct extent_buffer *eb, int level,
				u64 wanted_objectid, u64 wanted_disk_byte)
{
	int ret;
	int slot;
	struct btrfs_file_extent_item *fi;
	struct btrfs_key key;
	u64 disk_byte;

add_parent:
	ret = ulist_add(parents, eb->start, 0, GFP_NOFS);
	if (ret < 0)
		return ret;

	if (level != 0)
		return 0;

	/*
	 * if the current leaf is full with EXTENT_DATA items, we must
	 * check the next one if that holds a reference as well.
	 * ref->count cannot be used to skip this check.
	 * repeat this until we don't find any additional EXTENT_DATA items.
	 */
	while (1) {
		ret = btrfs_next_leaf(root, path);
		if (ret < 0)
			return ret;
		if (ret)
			return 0;

		eb = path->nodes[0];
		for (slot = 0; slot < btrfs_header_nritems(eb); ++slot) {
			btrfs_item_key_to_cpu(eb, &key, slot);
			if (key.objectid != wanted_objectid ||
			    key.type != BTRFS_EXTENT_DATA_KEY)
				return 0;
			fi = btrfs_item_ptr(eb, slot,
						struct btrfs_file_extent_item);
			disk_byte = btrfs_file_extent_disk_bytenr(eb, fi);
			if (disk_byte == wanted_disk_byte)
				goto add_parent;
		}
	}

	return 0;
=======
	prelim_ref_insert(fs_info, preftree, ref, sc);
	return extent_is_shared(sc);
}

/* direct refs use root == 0, key == NULL */
static int add_direct_ref(const struct btrfs_fs_info *fs_info,
			  struct preftrees *preftrees, int level, u64 parent,
			  u64 wanted_disk_byte, int count,
			  struct share_check *sc, gfp_t gfp_mask)
{
	return add_prelim_ref(fs_info, &preftrees->direct, 0, NULL, level,
			      parent, wanted_disk_byte, count, sc, gfp_mask);
}

/* indirect refs use parent == 0 */
static int add_indirect_ref(const struct btrfs_fs_info *fs_info,
			    struct preftrees *preftrees, u64 root_id,
			    const struct btrfs_key *key, int level,
			    u64 wanted_disk_byte, int count,
			    struct share_check *sc, gfp_t gfp_mask)
{
	struct preftree *tree = &preftrees->indirect;

	if (!key)
		tree = &preftrees->indirect_missing_keys;
	return add_prelim_ref(fs_info, tree, root_id, key, level, 0,
			      wanted_disk_byte, count, sc, gfp_mask);
}

static int is_shared_data_backref(struct preftrees *preftrees, u64 bytenr)
{
	struct rb_node **p = &preftrees->direct.root.rb_root.rb_node;
	struct rb_node *parent = NULL;
	struct prelim_ref *ref = NULL;
	struct prelim_ref target = {};
	int result;

	target.parent = bytenr;

	while (*p) {
		parent = *p;
		ref = rb_entry(parent, struct prelim_ref, rbnode);
		result = prelim_ref_compare(ref, &target);

		if (result < 0)
			p = &(*p)->rb_left;
		else if (result > 0)
			p = &(*p)->rb_right;
		else
			return 1;
	}
	return 0;
}

static int add_all_parents(struct btrfs_backref_walk_ctx *ctx,
			   struct btrfs_root *root, struct btrfs_path *path,
			   struct ulist *parents,
			   struct preftrees *preftrees, struct prelim_ref *ref,
			   int level)
{
	int ret = 0;
	int slot;
	struct extent_buffer *eb;
	struct btrfs_key key;
	struct btrfs_key *key_for_search = &ref->key_for_search;
	struct btrfs_file_extent_item *fi;
	struct extent_inode_elem *eie = NULL, *old = NULL;
	u64 disk_byte;
	u64 wanted_disk_byte = ref->wanted_disk_byte;
	u64 count = 0;
	u64 data_offset;
	u8 type;

	if (level != 0) {
		eb = path->nodes[level];
		ret = ulist_add(parents, eb->start, 0, GFP_NOFS);
		if (ret < 0)
			return ret;
		return 0;
	}

	/*
	 * 1. We normally enter this function with the path already pointing to
	 *    the first item to check. But sometimes, we may enter it with
	 *    slot == nritems.
	 * 2. We are searching for normal backref but bytenr of this leaf
	 *    matches shared data backref
	 * 3. The leaf owner is not equal to the root we are searching
	 *
	 * For these cases, go to the next leaf before we continue.
	 */
	eb = path->nodes[0];
	if (path->slots[0] >= btrfs_header_nritems(eb) ||
	    is_shared_data_backref(preftrees, eb->start) ||
	    ref->root_id != btrfs_header_owner(eb)) {
		if (ctx->time_seq == BTRFS_SEQ_LAST)
			ret = btrfs_next_leaf(root, path);
		else
			ret = btrfs_next_old_leaf(root, path, ctx->time_seq);
	}

	while (!ret && count < ref->count) {
		eb = path->nodes[0];
		slot = path->slots[0];

		btrfs_item_key_to_cpu(eb, &key, slot);

		if (key.objectid != key_for_search->objectid ||
		    key.type != BTRFS_EXTENT_DATA_KEY)
			break;

		/*
		 * We are searching for normal backref but bytenr of this leaf
		 * matches shared data backref, OR
		 * the leaf owner is not equal to the root we are searching for
		 */
		if (slot == 0 &&
		    (is_shared_data_backref(preftrees, eb->start) ||
		     ref->root_id != btrfs_header_owner(eb))) {
			if (ctx->time_seq == BTRFS_SEQ_LAST)
				ret = btrfs_next_leaf(root, path);
			else
				ret = btrfs_next_old_leaf(root, path, ctx->time_seq);
			continue;
		}
		fi = btrfs_item_ptr(eb, slot, struct btrfs_file_extent_item);
		type = btrfs_file_extent_type(eb, fi);
		if (type == BTRFS_FILE_EXTENT_INLINE)
			goto next;
		disk_byte = btrfs_file_extent_disk_bytenr(eb, fi);
		data_offset = btrfs_file_extent_offset(eb, fi);

		if (disk_byte == wanted_disk_byte) {
			eie = NULL;
			old = NULL;
			if (ref->key_for_search.offset == key.offset - data_offset)
				count++;
			else
				goto next;
			if (!ctx->skip_inode_ref_list) {
				ret = check_extent_in_eb(ctx, &key, eb, fi, &eie);
				if (ret == BTRFS_ITERATE_EXTENT_INODES_STOP ||
				    ret < 0)
					break;
			}
			if (ret > 0)
				goto next;
			ret = ulist_add_merge_ptr(parents, eb->start,
						  eie, (void **)&old, GFP_NOFS);
			if (ret < 0)
				break;
			if (!ret && !ctx->skip_inode_ref_list) {
				while (old->next)
					old = old->next;
				old->next = eie;
			}
			eie = NULL;
		}
next:
		if (ctx->time_seq == BTRFS_SEQ_LAST)
			ret = btrfs_next_item(root, path);
		else
			ret = btrfs_next_old_item(root, path, ctx->time_seq);
	}

	if (ret == BTRFS_ITERATE_EXTENT_INODES_STOP || ret < 0)
		free_inode_elem_list(eie);
	else if (ret > 0)
		ret = 0;

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * resolve an indirect backref in the form (root_id, key, level)
 * to a logical address
 */
<<<<<<< HEAD
static int __resolve_indirect_ref(struct btrfs_fs_info *fs_info,
					int search_commit_root,
					struct __prelim_ref *ref,
					struct ulist *parents)
{
	struct btrfs_path *path;
	struct btrfs_root *root;
	struct btrfs_key root_key;
	struct btrfs_key key = {0};
=======
static int resolve_indirect_ref(struct btrfs_backref_walk_ctx *ctx,
				struct btrfs_path *path,
				struct preftrees *preftrees,
				struct prelim_ref *ref, struct ulist *parents)
{
	struct btrfs_root *root;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct extent_buffer *eb;
	int ret = 0;
	int root_level;
	int level = ref->level;
<<<<<<< HEAD

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
	path->search_commit_root = !!search_commit_root;

	root_key.objectid = ref->root_id;
	root_key.type = BTRFS_ROOT_ITEM_KEY;
	root_key.offset = (u64)-1;
	root = btrfs_read_fs_root_no_name(fs_info, &root_key);
	if (IS_ERR(root)) {
		ret = PTR_ERR(root);
		goto out;
	}

	rcu_read_lock();
	root_level = btrfs_header_level(root->node);
	rcu_read_unlock();
=======
	struct btrfs_key search_key = ref->key_for_search;

	/*
	 * If we're search_commit_root we could possibly be holding locks on
	 * other tree nodes.  This happens when qgroups does backref walks when
	 * adding new delayed refs.  To deal with this we need to look in cache
	 * for the root, and if we don't find it then we need to search the
	 * tree_root's commit root, thus the btrfs_get_fs_root_commit_root usage
	 * here.
	 */
	if (path->search_commit_root)
		root = btrfs_get_fs_root_commit_root(ctx->fs_info, path, ref->root_id);
	else
		root = btrfs_get_fs_root(ctx->fs_info, ref->root_id, false);
	if (IS_ERR(root)) {
		ret = PTR_ERR(root);
		goto out_free;
	}

	if (!path->search_commit_root &&
	    test_bit(BTRFS_ROOT_DELETING, &root->state)) {
		ret = -ENOENT;
		goto out;
	}

	if (btrfs_is_testing(ctx->fs_info)) {
		ret = -ENOENT;
		goto out;
	}

	if (path->search_commit_root)
		root_level = btrfs_header_level(root->commit_root);
	else if (ctx->time_seq == BTRFS_SEQ_LAST)
		root_level = btrfs_header_level(root->node);
	else
		root_level = btrfs_old_root_level(root, ctx->time_seq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (root_level + 1 == level)
		goto out;

<<<<<<< HEAD
	path->lowest_level = level;
	ret = btrfs_search_slot(NULL, root, &ref->key, path, 0, 0);
	pr_debug("search slot in root %llu (level %d, ref count %d) returned "
		 "%d for key (%llu %u %llu)\n",
		 (unsigned long long)ref->root_id, level, ref->count, ret,
		 (unsigned long long)ref->key.objectid, ref->key.type,
		 (unsigned long long)ref->key.offset);
=======
	/*
	 * We can often find data backrefs with an offset that is too large
	 * (>= LLONG_MAX, maximum allowed file offset) due to underflows when
	 * subtracting a file's offset with the data offset of its
	 * corresponding extent data item. This can happen for example in the
	 * clone ioctl.
	 *
	 * So if we detect such case we set the search key's offset to zero to
	 * make sure we will find the matching file extent item at
	 * add_all_parents(), otherwise we will miss it because the offset
	 * taken form the backref is much larger then the offset of the file
	 * extent item. This can make us scan a very large number of file
	 * extent items, but at least it will not make us miss any.
	 *
	 * This is an ugly workaround for a behaviour that should have never
	 * existed, but it does and a fix for the clone ioctl would touch a lot
	 * of places, cause backwards incompatibility and would not fix the
	 * problem for extents cloned with older kernels.
	 */
	if (search_key.type == BTRFS_EXTENT_DATA_KEY &&
	    search_key.offset >= LLONG_MAX)
		search_key.offset = 0;
	path->lowest_level = level;
	if (ctx->time_seq == BTRFS_SEQ_LAST)
		ret = btrfs_search_slot(NULL, root, &search_key, path, 0, 0);
	else
		ret = btrfs_search_old_slot(root, &search_key, path, ctx->time_seq);

	btrfs_debug(ctx->fs_info,
		"search slot in root %llu (level %d, ref count %d) returned %d for key (%llu %u %llu)",
		 ref->root_id, level, ref->count, ret,
		 ref->key_for_search.objectid, ref->key_for_search.type,
		 ref->key_for_search.offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		goto out;

	eb = path->nodes[level];
<<<<<<< HEAD
	if (!eb) {
		WARN_ON(1);
		ret = 1;
		goto out;
	}

	if (level == 0) {
		if (ret == 1 && path->slots[0] >= btrfs_header_nritems(eb)) {
			ret = btrfs_next_leaf(root, path);
			if (ret)
				goto out;
			eb = path->nodes[0];
		}

		btrfs_item_key_to_cpu(eb, &key, path->slots[0]);
	}

	/* the last two parameters will only be used for level == 0 */
	ret = add_all_parents(root, path, parents, eb, level, key.objectid,
				ref->wanted_disk_byte);
out:
	btrfs_free_path(path);
	return ret;
}

/*
 * resolve all indirect backrefs from the list
 */
static int __resolve_indirect_refs(struct btrfs_fs_info *fs_info,
				   int search_commit_root,
				   struct list_head *head)
{
	int err;
	int ret = 0;
	struct __prelim_ref *ref;
	struct __prelim_ref *ref_safe;
	struct __prelim_ref *new_ref;
	struct ulist *parents;
	struct ulist_node *node;
=======
	while (!eb) {
		if (WARN_ON(!level)) {
			ret = 1;
			goto out;
		}
		level--;
		eb = path->nodes[level];
	}

	ret = add_all_parents(ctx, root, path, parents, preftrees, ref, level);
out:
	btrfs_put_root(root);
out_free:
	path->lowest_level = 0;
	btrfs_release_path(path);
	return ret;
}

static struct extent_inode_elem *
unode_aux_to_inode_list(struct ulist_node *node)
{
	if (!node)
		return NULL;
	return (struct extent_inode_elem *)(uintptr_t)node->aux;
}

static void free_leaf_list(struct ulist *ulist)
{
	struct ulist_node *node;
	struct ulist_iterator uiter;

	ULIST_ITER_INIT(&uiter);
	while ((node = ulist_next(ulist, &uiter)))
		free_inode_elem_list(unode_aux_to_inode_list(node));

	ulist_free(ulist);
}

/*
 * We maintain three separate rbtrees: one for direct refs, one for
 * indirect refs which have a key, and one for indirect refs which do not
 * have a key. Each tree does merge on insertion.
 *
 * Once all of the references are located, we iterate over the tree of
 * indirect refs with missing keys. An appropriate key is located and
 * the ref is moved onto the tree for indirect refs. After all missing
 * keys are thus located, we iterate over the indirect ref tree, resolve
 * each reference, and then insert the resolved reference onto the
 * direct tree (merging there too).
 *
 * New backrefs (i.e., for parent nodes) are added to the appropriate
 * rbtree as they are encountered. The new backrefs are subsequently
 * resolved as above.
 */
static int resolve_indirect_refs(struct btrfs_backref_walk_ctx *ctx,
				 struct btrfs_path *path,
				 struct preftrees *preftrees,
				 struct share_check *sc)
{
	int err;
	int ret = 0;
	struct ulist *parents;
	struct ulist_node *node;
	struct ulist_iterator uiter;
	struct rb_node *rnode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	parents = ulist_alloc(GFP_NOFS);
	if (!parents)
		return -ENOMEM;

	/*
<<<<<<< HEAD
	 * _safe allows us to insert directly after the current item without
	 * iterating over the newly inserted items.
	 * we're also allowed to re-assign ref during iteration.
	 */
	list_for_each_entry_safe(ref, ref_safe, head, list) {
		if (ref->parent)	/* already direct */
			continue;
		if (ref->count == 0)
			continue;
		err = __resolve_indirect_ref(fs_info, search_commit_root,
					     ref, parents);
		if (err) {
			if (ret == 0)
				ret = err;
			continue;
		}

		/* we put the first parent into the ref at hand */
		node = ulist_next(parents, NULL);
		ref->parent = node ? node->val : 0;

		/* additional parents require new refs being added here */
		while ((node = ulist_next(parents, node))) {
			new_ref = kmalloc(sizeof(*new_ref), GFP_NOFS);
			if (!new_ref) {
				ret = -ENOMEM;
				break;
			}
			memcpy(new_ref, ref, sizeof(*ref));
			new_ref->parent = node->val;
			list_add(&new_ref->list, &ref->list);
		}
		ulist_reinit(parents);
	}

	ulist_free(parents);
=======
	 * We could trade memory usage for performance here by iterating
	 * the tree, allocating new refs for each insertion, and then
	 * freeing the entire indirect tree when we're done.  In some test
	 * cases, the tree can grow quite large (~200k objects).
	 */
	while ((rnode = rb_first_cached(&preftrees->indirect.root))) {
		struct prelim_ref *ref;

		ref = rb_entry(rnode, struct prelim_ref, rbnode);
		if (WARN(ref->parent,
			 "BUG: direct ref found in indirect tree")) {
			ret = -EINVAL;
			goto out;
		}

		rb_erase_cached(&ref->rbnode, &preftrees->indirect.root);
		preftrees->indirect.count--;

		if (ref->count == 0) {
			free_pref(ref);
			continue;
		}

		if (sc && ref->root_id != sc->root->root_key.objectid) {
			free_pref(ref);
			ret = BACKREF_FOUND_SHARED;
			goto out;
		}
		err = resolve_indirect_ref(ctx, path, preftrees, ref, parents);
		/*
		 * we can only tolerate ENOENT,otherwise,we should catch error
		 * and return directly.
		 */
		if (err == -ENOENT) {
			prelim_ref_insert(ctx->fs_info, &preftrees->direct, ref,
					  NULL);
			continue;
		} else if (err) {
			free_pref(ref);
			ret = err;
			goto out;
		}

		/* we put the first parent into the ref at hand */
		ULIST_ITER_INIT(&uiter);
		node = ulist_next(parents, &uiter);
		ref->parent = node ? node->val : 0;
		ref->inode_list = unode_aux_to_inode_list(node);

		/* Add a prelim_ref(s) for any other parent(s). */
		while ((node = ulist_next(parents, &uiter))) {
			struct prelim_ref *new_ref;

			new_ref = kmem_cache_alloc(btrfs_prelim_ref_cache,
						   GFP_NOFS);
			if (!new_ref) {
				free_pref(ref);
				ret = -ENOMEM;
				goto out;
			}
			memcpy(new_ref, ref, sizeof(*ref));
			new_ref->parent = node->val;
			new_ref->inode_list = unode_aux_to_inode_list(node);
			prelim_ref_insert(ctx->fs_info, &preftrees->direct,
					  new_ref, NULL);
		}

		/*
		 * Now it's a direct ref, put it in the direct tree. We must
		 * do this last because the ref could be merged/freed here.
		 */
		prelim_ref_insert(ctx->fs_info, &preftrees->direct, ref, NULL);

		ulist_reinit(parents);
		cond_resched();
	}
out:
	/*
	 * We may have inode lists attached to refs in the parents ulist, so we
	 * must free them before freeing the ulist and its refs.
	 */
	free_leaf_list(parents);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
<<<<<<< HEAD
 * merge two lists of backrefs and adjust counts accordingly
 *
 * mode = 1: merge identical keys, if key is set
 * mode = 2: merge identical parents
 */
static int __merge_refs(struct list_head *head, int mode)
{
	struct list_head *pos1;

	list_for_each(pos1, head) {
		struct list_head *n2;
		struct list_head *pos2;
		struct __prelim_ref *ref1;

		ref1 = list_entry(pos1, struct __prelim_ref, list);

		if (mode == 1 && ref1->key.type == 0)
			continue;
		for (pos2 = pos1->next, n2 = pos2->next; pos2 != head;
		     pos2 = n2, n2 = pos2->next) {
			struct __prelim_ref *ref2;

			ref2 = list_entry(pos2, struct __prelim_ref, list);

			if (mode == 1) {
				if (memcmp(&ref1->key, &ref2->key,
					   sizeof(ref1->key)) ||
				    ref1->level != ref2->level ||
				    ref1->root_id != ref2->root_id)
					continue;
				ref1->count += ref2->count;
			} else {
				if (ref1->parent != ref2->parent)
					continue;
				ref1->count += ref2->count;
			}
			list_del(&ref2->list);
			kfree(ref2);
		}

=======
 * read tree blocks and add keys where required.
 */
static int add_missing_keys(struct btrfs_fs_info *fs_info,
			    struct preftrees *preftrees, bool lock)
{
	struct prelim_ref *ref;
	struct extent_buffer *eb;
	struct preftree *tree = &preftrees->indirect_missing_keys;
	struct rb_node *node;

	while ((node = rb_first_cached(&tree->root))) {
		struct btrfs_tree_parent_check check = { 0 };

		ref = rb_entry(node, struct prelim_ref, rbnode);
		rb_erase_cached(node, &tree->root);

		BUG_ON(ref->parent);	/* should not be a direct ref */
		BUG_ON(ref->key_for_search.type);
		BUG_ON(!ref->wanted_disk_byte);

		check.level = ref->level - 1;
		check.owner_root = ref->root_id;

		eb = read_tree_block(fs_info, ref->wanted_disk_byte, &check);
		if (IS_ERR(eb)) {
			free_pref(ref);
			return PTR_ERR(eb);
		}
		if (!extent_buffer_uptodate(eb)) {
			free_pref(ref);
			free_extent_buffer(eb);
			return -EIO;
		}

		if (lock)
			btrfs_tree_read_lock(eb);
		if (btrfs_header_level(eb) == 0)
			btrfs_item_key_to_cpu(eb, &ref->key_for_search, 0);
		else
			btrfs_node_key_to_cpu(eb, &ref->key_for_search, 0);
		if (lock)
			btrfs_tree_read_unlock(eb);
		free_extent_buffer(eb);
		prelim_ref_insert(fs_info, &preftrees->indirect, ref, NULL);
		cond_resched();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

/*
 * add all currently queued delayed refs from this head whose seq nr is
 * smaller or equal that seq to the list
 */
<<<<<<< HEAD
static int __add_delayed_refs(struct btrfs_delayed_ref_head *head, u64 seq,
			      struct btrfs_key *info_key,
			      struct list_head *prefs)
{
	struct btrfs_delayed_extent_op *extent_op = head->extent_op;
	struct rb_node *n = &head->node.rb_node;
	int sgn;
	int ret = 0;

	if (extent_op && extent_op->update_key)
		btrfs_disk_key_to_cpu(info_key, &extent_op->key);

	while ((n = rb_prev(n))) {
		struct btrfs_delayed_ref_node *node;
		node = rb_entry(n, struct btrfs_delayed_ref_node,
				rb_node);
		if (node->bytenr != head->node.bytenr)
			break;
		WARN_ON(node->is_head);

=======
static int add_delayed_refs(const struct btrfs_fs_info *fs_info,
			    struct btrfs_delayed_ref_head *head, u64 seq,
			    struct preftrees *preftrees, struct share_check *sc)
{
	struct btrfs_delayed_ref_node *node;
	struct btrfs_key key;
	struct rb_node *n;
	int count;
	int ret = 0;

	spin_lock(&head->lock);
	for (n = rb_first_cached(&head->ref_tree); n; n = rb_next(n)) {
		node = rb_entry(n, struct btrfs_delayed_ref_node,
				ref_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (node->seq > seq)
			continue;

		switch (node->action) {
		case BTRFS_ADD_DELAYED_EXTENT:
		case BTRFS_UPDATE_DELAYED_HEAD:
			WARN_ON(1);
			continue;
		case BTRFS_ADD_DELAYED_REF:
<<<<<<< HEAD
			sgn = 1;
			break;
		case BTRFS_DROP_DELAYED_REF:
			sgn = -1;
			break;
		default:
			BUG_ON(1);
		}
		switch (node->type) {
		case BTRFS_TREE_BLOCK_REF_KEY: {
			struct btrfs_delayed_tree_ref *ref;

			ref = btrfs_delayed_node_to_tree_ref(node);
			ret = __add_prelim_ref(prefs, ref->root, info_key,
					       ref->level + 1, 0, node->bytenr,
					       node->ref_mod * sgn);
			break;
		}
		case BTRFS_SHARED_BLOCK_REF_KEY: {
			struct btrfs_delayed_tree_ref *ref;

			ref = btrfs_delayed_node_to_tree_ref(node);
			ret = __add_prelim_ref(prefs, ref->root, info_key,
					       ref->level + 1, ref->parent,
					       node->bytenr,
					       node->ref_mod * sgn);
			break;
		}
		case BTRFS_EXTENT_DATA_REF_KEY: {
			struct btrfs_delayed_data_ref *ref;
			struct btrfs_key key;

=======
			count = node->ref_mod;
			break;
		case BTRFS_DROP_DELAYED_REF:
			count = node->ref_mod * -1;
			break;
		default:
			BUG();
		}
		switch (node->type) {
		case BTRFS_TREE_BLOCK_REF_KEY: {
			/* NORMAL INDIRECT METADATA backref */
			struct btrfs_delayed_tree_ref *ref;
			struct btrfs_key *key_ptr = NULL;

			if (head->extent_op && head->extent_op->update_key) {
				btrfs_disk_key_to_cpu(&key, &head->extent_op->key);
				key_ptr = &key;
			}

			ref = btrfs_delayed_node_to_tree_ref(node);
			ret = add_indirect_ref(fs_info, preftrees, ref->root,
					       key_ptr, ref->level + 1,
					       node->bytenr, count, sc,
					       GFP_ATOMIC);
			break;
		}
		case BTRFS_SHARED_BLOCK_REF_KEY: {
			/* SHARED DIRECT METADATA backref */
			struct btrfs_delayed_tree_ref *ref;

			ref = btrfs_delayed_node_to_tree_ref(node);

			ret = add_direct_ref(fs_info, preftrees, ref->level + 1,
					     ref->parent, node->bytenr, count,
					     sc, GFP_ATOMIC);
			break;
		}
		case BTRFS_EXTENT_DATA_REF_KEY: {
			/* NORMAL INDIRECT DATA backref */
			struct btrfs_delayed_data_ref *ref;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ref = btrfs_delayed_node_to_data_ref(node);

			key.objectid = ref->objectid;
			key.type = BTRFS_EXTENT_DATA_KEY;
			key.offset = ref->offset;
<<<<<<< HEAD
			ret = __add_prelim_ref(prefs, ref->root, &key, 0, 0,
					       node->bytenr,
					       node->ref_mod * sgn);
			break;
		}
		case BTRFS_SHARED_DATA_REF_KEY: {
			struct btrfs_delayed_data_ref *ref;
			struct btrfs_key key;

			ref = btrfs_delayed_node_to_data_ref(node);

			key.objectid = ref->objectid;
			key.type = BTRFS_EXTENT_DATA_KEY;
			key.offset = ref->offset;
			ret = __add_prelim_ref(prefs, ref->root, &key, 0,
					       ref->parent, node->bytenr,
					       node->ref_mod * sgn);
=======

			/*
			 * If we have a share check context and a reference for
			 * another inode, we can't exit immediately. This is
			 * because even if this is a BTRFS_ADD_DELAYED_REF
			 * reference we may find next a BTRFS_DROP_DELAYED_REF
			 * which cancels out this ADD reference.
			 *
			 * If this is a DROP reference and there was no previous
			 * ADD reference, then we need to signal that when we
			 * process references from the extent tree (through
			 * add_inline_refs() and add_keyed_refs()), we should
			 * not exit early if we find a reference for another
			 * inode, because one of the delayed DROP references
			 * may cancel that reference in the extent tree.
			 */
			if (sc && count < 0)
				sc->have_delayed_delete_refs = true;

			ret = add_indirect_ref(fs_info, preftrees, ref->root,
					       &key, 0, node->bytenr, count, sc,
					       GFP_ATOMIC);
			break;
		}
		case BTRFS_SHARED_DATA_REF_KEY: {
			/* SHARED DIRECT FULL backref */
			struct btrfs_delayed_data_ref *ref;

			ref = btrfs_delayed_node_to_data_ref(node);

			ret = add_direct_ref(fs_info, preftrees, 0, ref->parent,
					     node->bytenr, count, sc,
					     GFP_ATOMIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		default:
			WARN_ON(1);
		}
<<<<<<< HEAD
		BUG_ON(ret);
	}

	return 0;
=======
		/*
		 * We must ignore BACKREF_FOUND_SHARED until all delayed
		 * refs have been checked.
		 */
		if (ret && (ret != BACKREF_FOUND_SHARED))
			break;
	}
	if (!ret)
		ret = extent_is_shared(sc);

	spin_unlock(&head->lock);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * add all inline backrefs for bytenr to the list
<<<<<<< HEAD
 */
static int __add_inline_refs(struct btrfs_fs_info *fs_info,
			     struct btrfs_path *path, u64 bytenr,
			     struct btrfs_key *info_key, int *info_level,
			     struct list_head *prefs)
=======
 *
 * Returns 0 on success, <0 on error, or BACKREF_FOUND_SHARED.
 */
static int add_inline_refs(struct btrfs_backref_walk_ctx *ctx,
			   struct btrfs_path *path,
			   int *info_level, struct preftrees *preftrees,
			   struct share_check *sc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret = 0;
	int slot;
	struct extent_buffer *leaf;
	struct btrfs_key key;
<<<<<<< HEAD
=======
	struct btrfs_key found_key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long ptr;
	unsigned long end;
	struct btrfs_extent_item *ei;
	u64 flags;
	u64 item_size;

	/*
	 * enumerate all inline refs
	 */
	leaf = path->nodes[0];
<<<<<<< HEAD
	slot = path->slots[0] - 1;

	item_size = btrfs_item_size_nr(leaf, slot);
	BUG_ON(item_size < sizeof(*ei));

	ei = btrfs_item_ptr(leaf, slot, struct btrfs_extent_item);
	flags = btrfs_extent_flags(leaf, ei);
=======
	slot = path->slots[0];

	item_size = btrfs_item_size(leaf, slot);
	ei = btrfs_item_ptr(leaf, slot, struct btrfs_extent_item);

	if (ctx->check_extent_item) {
		ret = ctx->check_extent_item(ctx->bytenr, ei, leaf, ctx->user_ctx);
		if (ret)
			return ret;
	}

	flags = btrfs_extent_flags(leaf, ei);
	btrfs_item_key_to_cpu(leaf, &found_key, slot);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ptr = (unsigned long)(ei + 1);
	end = (unsigned long)ei + item_size;

<<<<<<< HEAD
	if (flags & BTRFS_EXTENT_FLAG_TREE_BLOCK) {
		struct btrfs_tree_block_info *info;
		struct btrfs_disk_key disk_key;

		info = (struct btrfs_tree_block_info *)ptr;
		*info_level = btrfs_tree_block_level(leaf, info);
		btrfs_tree_block_key(leaf, info, &disk_key);
		btrfs_disk_key_to_cpu(info_key, &disk_key);
		ptr += sizeof(struct btrfs_tree_block_info);
		BUG_ON(ptr > end);
=======
	if (found_key.type == BTRFS_EXTENT_ITEM_KEY &&
	    flags & BTRFS_EXTENT_FLAG_TREE_BLOCK) {
		struct btrfs_tree_block_info *info;

		info = (struct btrfs_tree_block_info *)ptr;
		*info_level = btrfs_tree_block_level(leaf, info);
		ptr += sizeof(struct btrfs_tree_block_info);
		BUG_ON(ptr > end);
	} else if (found_key.type == BTRFS_METADATA_ITEM_KEY) {
		*info_level = found_key.offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		BUG_ON(!(flags & BTRFS_EXTENT_FLAG_DATA));
	}

	while (ptr < end) {
		struct btrfs_extent_inline_ref *iref;
		u64 offset;
		int type;

		iref = (struct btrfs_extent_inline_ref *)ptr;
<<<<<<< HEAD
		type = btrfs_extent_inline_ref_type(leaf, iref);
=======
		type = btrfs_get_extent_inline_ref_type(leaf, iref,
							BTRFS_REF_TYPE_ANY);
		if (type == BTRFS_REF_TYPE_INVALID)
			return -EUCLEAN;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		offset = btrfs_extent_inline_ref_offset(leaf, iref);

		switch (type) {
		case BTRFS_SHARED_BLOCK_REF_KEY:
<<<<<<< HEAD
			ret = __add_prelim_ref(prefs, 0, info_key,
						*info_level + 1, offset,
						bytenr, 1);
=======
			ret = add_direct_ref(ctx->fs_info, preftrees,
					     *info_level + 1, offset,
					     ctx->bytenr, 1, NULL, GFP_NOFS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case BTRFS_SHARED_DATA_REF_KEY: {
			struct btrfs_shared_data_ref *sdref;
			int count;

			sdref = (struct btrfs_shared_data_ref *)(iref + 1);
			count = btrfs_shared_data_ref_count(leaf, sdref);
<<<<<<< HEAD
			ret = __add_prelim_ref(prefs, 0, NULL, 0, offset,
					       bytenr, count);
			break;
		}
		case BTRFS_TREE_BLOCK_REF_KEY:
			ret = __add_prelim_ref(prefs, offset, info_key,
					       *info_level + 1, 0, bytenr, 1);
=======

			ret = add_direct_ref(ctx->fs_info, preftrees, 0, offset,
					     ctx->bytenr, count, sc, GFP_NOFS);
			break;
		}
		case BTRFS_TREE_BLOCK_REF_KEY:
			ret = add_indirect_ref(ctx->fs_info, preftrees, offset,
					       NULL, *info_level + 1,
					       ctx->bytenr, 1, NULL, GFP_NOFS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case BTRFS_EXTENT_DATA_REF_KEY: {
			struct btrfs_extent_data_ref *dref;
			int count;
			u64 root;

			dref = (struct btrfs_extent_data_ref *)(&iref->offset);
			count = btrfs_extent_data_ref_count(leaf, dref);
			key.objectid = btrfs_extent_data_ref_objectid(leaf,
								      dref);
			key.type = BTRFS_EXTENT_DATA_KEY;
			key.offset = btrfs_extent_data_ref_offset(leaf, dref);
<<<<<<< HEAD
			root = btrfs_extent_data_ref_root(leaf, dref);
			ret = __add_prelim_ref(prefs, root, &key, 0, 0, bytenr,
						count);
			break;
		}
		default:
			WARN_ON(1);
		}
		BUG_ON(ret);
=======

			if (sc && key.objectid != sc->inum &&
			    !sc->have_delayed_delete_refs) {
				ret = BACKREF_FOUND_SHARED;
				break;
			}

			root = btrfs_extent_data_ref_root(leaf, dref);

			if (!ctx->skip_data_ref ||
			    !ctx->skip_data_ref(root, key.objectid, key.offset,
						ctx->user_ctx))
				ret = add_indirect_ref(ctx->fs_info, preftrees,
						       root, &key, 0, ctx->bytenr,
						       count, sc, GFP_NOFS);
			break;
		}
		case BTRFS_EXTENT_OWNER_REF_KEY:
			ASSERT(btrfs_fs_incompat(ctx->fs_info, SIMPLE_QUOTA));
			break;
		default:
			WARN_ON(1);
		}
		if (ret)
			return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ptr += btrfs_extent_inline_ref_size(type);
	}

	return 0;
}

/*
 * add all non-inline backrefs for bytenr to the list
<<<<<<< HEAD
 */
static int __add_keyed_refs(struct btrfs_fs_info *fs_info,
			    struct btrfs_path *path, u64 bytenr,
			    struct btrfs_key *info_key, int info_level,
			    struct list_head *prefs)
{
	struct btrfs_root *extent_root = fs_info->extent_root;
=======
 *
 * Returns 0 on success, <0 on error, or BACKREF_FOUND_SHARED.
 */
static int add_keyed_refs(struct btrfs_backref_walk_ctx *ctx,
			  struct btrfs_root *extent_root,
			  struct btrfs_path *path,
			  int info_level, struct preftrees *preftrees,
			  struct share_check *sc)
{
	struct btrfs_fs_info *fs_info = extent_root->fs_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;
	int slot;
	struct extent_buffer *leaf;
	struct btrfs_key key;

	while (1) {
		ret = btrfs_next_item(extent_root, path);
		if (ret < 0)
			break;
		if (ret) {
			ret = 0;
			break;
		}

		slot = path->slots[0];
		leaf = path->nodes[0];
		btrfs_item_key_to_cpu(leaf, &key, slot);

<<<<<<< HEAD
		if (key.objectid != bytenr)
=======
		if (key.objectid != ctx->bytenr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		if (key.type < BTRFS_TREE_BLOCK_REF_KEY)
			continue;
		if (key.type > BTRFS_SHARED_DATA_REF_KEY)
			break;

		switch (key.type) {
		case BTRFS_SHARED_BLOCK_REF_KEY:
<<<<<<< HEAD
			ret = __add_prelim_ref(prefs, 0, info_key,
						info_level + 1, key.offset,
						bytenr, 1);
			break;
		case BTRFS_SHARED_DATA_REF_KEY: {
=======
			/* SHARED DIRECT METADATA backref */
			ret = add_direct_ref(fs_info, preftrees,
					     info_level + 1, key.offset,
					     ctx->bytenr, 1, NULL, GFP_NOFS);
			break;
		case BTRFS_SHARED_DATA_REF_KEY: {
			/* SHARED DIRECT FULL backref */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			struct btrfs_shared_data_ref *sdref;
			int count;

			sdref = btrfs_item_ptr(leaf, slot,
					      struct btrfs_shared_data_ref);
			count = btrfs_shared_data_ref_count(leaf, sdref);
<<<<<<< HEAD
			ret = __add_prelim_ref(prefs, 0, NULL, 0, key.offset,
						bytenr, count);
			break;
		}
		case BTRFS_TREE_BLOCK_REF_KEY:
			ret = __add_prelim_ref(prefs, key.offset, info_key,
						info_level + 1, 0, bytenr, 1);
			break;
		case BTRFS_EXTENT_DATA_REF_KEY: {
=======
			ret = add_direct_ref(fs_info, preftrees, 0,
					     key.offset, ctx->bytenr, count,
					     sc, GFP_NOFS);
			break;
		}
		case BTRFS_TREE_BLOCK_REF_KEY:
			/* NORMAL INDIRECT METADATA backref */
			ret = add_indirect_ref(fs_info, preftrees, key.offset,
					       NULL, info_level + 1, ctx->bytenr,
					       1, NULL, GFP_NOFS);
			break;
		case BTRFS_EXTENT_DATA_REF_KEY: {
			/* NORMAL INDIRECT DATA backref */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			struct btrfs_extent_data_ref *dref;
			int count;
			u64 root;

			dref = btrfs_item_ptr(leaf, slot,
					      struct btrfs_extent_data_ref);
			count = btrfs_extent_data_ref_count(leaf, dref);
			key.objectid = btrfs_extent_data_ref_objectid(leaf,
								      dref);
			key.type = BTRFS_EXTENT_DATA_KEY;
			key.offset = btrfs_extent_data_ref_offset(leaf, dref);
<<<<<<< HEAD
			root = btrfs_extent_data_ref_root(leaf, dref);
			ret = __add_prelim_ref(prefs, root, &key, 0, 0,
						bytenr, count);
=======

			if (sc && key.objectid != sc->inum &&
			    !sc->have_delayed_delete_refs) {
				ret = BACKREF_FOUND_SHARED;
				break;
			}

			root = btrfs_extent_data_ref_root(leaf, dref);

			if (!ctx->skip_data_ref ||
			    !ctx->skip_data_ref(root, key.objectid, key.offset,
						ctx->user_ctx))
				ret = add_indirect_ref(fs_info, preftrees, root,
						       &key, 0, ctx->bytenr,
						       count, sc, GFP_NOFS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		default:
			WARN_ON(1);
		}
<<<<<<< HEAD
		BUG_ON(ret);
=======
		if (ret)
			return ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return ret;
}

/*
<<<<<<< HEAD
=======
 * The caller has joined a transaction or is holding a read lock on the
 * fs_info->commit_root_sem semaphore, so no need to worry about the root's last
 * snapshot field changing while updating or checking the cache.
 */
static bool lookup_backref_shared_cache(struct btrfs_backref_share_check_ctx *ctx,
					struct btrfs_root *root,
					u64 bytenr, int level, bool *is_shared)
{
	const struct btrfs_fs_info *fs_info = root->fs_info;
	struct btrfs_backref_shared_cache_entry *entry;

	if (!current->journal_info)
		lockdep_assert_held(&fs_info->commit_root_sem);

	if (!ctx->use_path_cache)
		return false;

	if (WARN_ON_ONCE(level >= BTRFS_MAX_LEVEL))
		return false;

	/*
	 * Level -1 is used for the data extent, which is not reliable to cache
	 * because its reference count can increase or decrease without us
	 * realizing. We cache results only for extent buffers that lead from
	 * the root node down to the leaf with the file extent item.
	 */
	ASSERT(level >= 0);

	entry = &ctx->path_cache_entries[level];

	/* Unused cache entry or being used for some other extent buffer. */
	if (entry->bytenr != bytenr)
		return false;

	/*
	 * We cached a false result, but the last snapshot generation of the
	 * root changed, so we now have a snapshot. Don't trust the result.
	 */
	if (!entry->is_shared &&
	    entry->gen != btrfs_root_last_snapshot(&root->root_item))
		return false;

	/*
	 * If we cached a true result and the last generation used for dropping
	 * a root changed, we can not trust the result, because the dropped root
	 * could be a snapshot sharing this extent buffer.
	 */
	if (entry->is_shared &&
	    entry->gen != btrfs_get_last_root_drop_gen(fs_info))
		return false;

	*is_shared = entry->is_shared;
	/*
	 * If the node at this level is shared, than all nodes below are also
	 * shared. Currently some of the nodes below may be marked as not shared
	 * because we have just switched from one leaf to another, and switched
	 * also other nodes above the leaf and below the current level, so mark
	 * them as shared.
	 */
	if (*is_shared) {
		for (int i = 0; i < level; i++) {
			ctx->path_cache_entries[i].is_shared = true;
			ctx->path_cache_entries[i].gen = entry->gen;
		}
	}

	return true;
}

/*
 * The caller has joined a transaction or is holding a read lock on the
 * fs_info->commit_root_sem semaphore, so no need to worry about the root's last
 * snapshot field changing while updating or checking the cache.
 */
static void store_backref_shared_cache(struct btrfs_backref_share_check_ctx *ctx,
				       struct btrfs_root *root,
				       u64 bytenr, int level, bool is_shared)
{
	const struct btrfs_fs_info *fs_info = root->fs_info;
	struct btrfs_backref_shared_cache_entry *entry;
	u64 gen;

	if (!current->journal_info)
		lockdep_assert_held(&fs_info->commit_root_sem);

	if (!ctx->use_path_cache)
		return;

	if (WARN_ON_ONCE(level >= BTRFS_MAX_LEVEL))
		return;

	/*
	 * Level -1 is used for the data extent, which is not reliable to cache
	 * because its reference count can increase or decrease without us
	 * realizing. We cache results only for extent buffers that lead from
	 * the root node down to the leaf with the file extent item.
	 */
	ASSERT(level >= 0);

	if (is_shared)
		gen = btrfs_get_last_root_drop_gen(fs_info);
	else
		gen = btrfs_root_last_snapshot(&root->root_item);

	entry = &ctx->path_cache_entries[level];
	entry->bytenr = bytenr;
	entry->is_shared = is_shared;
	entry->gen = gen;

	/*
	 * If we found an extent buffer is shared, set the cache result for all
	 * extent buffers below it to true. As nodes in the path are COWed,
	 * their sharedness is moved to their children, and if a leaf is COWed,
	 * then the sharedness of a data extent becomes direct, the refcount of
	 * data extent is increased in the extent item at the extent tree.
	 */
	if (is_shared) {
		for (int i = 0; i < level; i++) {
			entry = &ctx->path_cache_entries[i];
			entry->is_shared = is_shared;
			entry->gen = gen;
		}
	}
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * this adds all existing backrefs (inline backrefs, backrefs and delayed
 * refs) for the given bytenr to the refs list, merges duplicates and resolves
 * indirect refs to their parent bytenr.
 * When roots are found, they're added to the roots list
 *
<<<<<<< HEAD
 * FIXME some caching might speed things up
 */
static int find_parent_nodes(struct btrfs_trans_handle *trans,
			     struct btrfs_fs_info *fs_info, u64 bytenr,
			     u64 seq, struct ulist *refs, struct ulist *roots)
{
	struct btrfs_key key;
	struct btrfs_path *path;
	struct btrfs_key info_key = { 0 };
=======
 * @ctx:     Backref walking context object, must be not NULL.
 * @sc:      If !NULL, then immediately return BACKREF_FOUND_SHARED when a
 *           shared extent is detected.
 *
 * Otherwise this returns 0 for success and <0 for an error.
 *
 * FIXME some caching might speed things up
 */
static int find_parent_nodes(struct btrfs_backref_walk_ctx *ctx,
			     struct share_check *sc)
{
	struct btrfs_root *root = btrfs_extent_root(ctx->fs_info, ctx->bytenr);
	struct btrfs_key key;
	struct btrfs_path *path;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct btrfs_delayed_ref_root *delayed_refs = NULL;
	struct btrfs_delayed_ref_head *head;
	int info_level = 0;
	int ret;
<<<<<<< HEAD
	int search_commit_root = (trans == BTRFS_BACKREF_SEARCH_COMMIT_ROOT);
	struct list_head prefs_delayed;
	struct list_head prefs;
	struct __prelim_ref *ref;

	INIT_LIST_HEAD(&prefs);
	INIT_LIST_HEAD(&prefs_delayed);

	key.objectid = bytenr;
	key.type = BTRFS_EXTENT_ITEM_KEY;
	key.offset = (u64)-1;
=======
	struct prelim_ref *ref;
	struct rb_node *node;
	struct extent_inode_elem *eie = NULL;
	struct preftrees preftrees = {
		.direct = PREFTREE_INIT,
		.indirect = PREFTREE_INIT,
		.indirect_missing_keys = PREFTREE_INIT
	};

	/* Roots ulist is not needed when using a sharedness check context. */
	if (sc)
		ASSERT(ctx->roots == NULL);

	key.objectid = ctx->bytenr;
	key.offset = (u64)-1;
	if (btrfs_fs_incompat(ctx->fs_info, SKINNY_METADATA))
		key.type = BTRFS_METADATA_ITEM_KEY;
	else
		key.type = BTRFS_EXTENT_ITEM_KEY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
<<<<<<< HEAD
	path->search_commit_root = !!search_commit_root;

	/*
	 * grab both a lock on the path and a lock on the delayed ref head.
	 * We need both to get a consistent picture of how the refs look
	 * at a specified point in time
	 */
again:
	head = NULL;

	ret = btrfs_search_slot(trans, fs_info->extent_root, &key, path, 0, 0);
	if (ret < 0)
		goto out;
	BUG_ON(ret == 0);

	if (trans != BTRFS_BACKREF_SEARCH_COMMIT_ROOT) {
		/*
		 * look if there are updates for this ref queued and lock the
		 * head
		 */
		delayed_refs = &trans->transaction->delayed_refs;
		spin_lock(&delayed_refs->lock);
		head = btrfs_find_delayed_ref_head(trans, bytenr);
		if (head) {
			if (!mutex_trylock(&head->mutex)) {
				atomic_inc(&head->node.refs);
=======
	if (!ctx->trans) {
		path->search_commit_root = 1;
		path->skip_locking = 1;
	}

	if (ctx->time_seq == BTRFS_SEQ_LAST)
		path->skip_locking = 1;

again:
	head = NULL;

	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
	if (ret < 0)
		goto out;
	if (ret == 0) {
		/*
		 * Key with offset -1 found, there would have to exist an extent
		 * item with such offset, but this is out of the valid range.
		 */
		ret = -EUCLEAN;
		goto out;
	}

	if (ctx->trans && likely(ctx->trans->type != __TRANS_DUMMY) &&
	    ctx->time_seq != BTRFS_SEQ_LAST) {
		/*
		 * We have a specific time_seq we care about and trans which
		 * means we have the path lock, we need to grab the ref head and
		 * lock it so we have a consistent view of the refs at the given
		 * time.
		 */
		delayed_refs = &ctx->trans->transaction->delayed_refs;
		spin_lock(&delayed_refs->lock);
		head = btrfs_find_delayed_ref_head(delayed_refs, ctx->bytenr);
		if (head) {
			if (!mutex_trylock(&head->mutex)) {
				refcount_inc(&head->refs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				spin_unlock(&delayed_refs->lock);

				btrfs_release_path(path);

				/*
				 * Mutex was contended, block until it's
				 * released and try again
				 */
				mutex_lock(&head->mutex);
				mutex_unlock(&head->mutex);
<<<<<<< HEAD
				btrfs_put_delayed_ref(&head->node);
				goto again;
			}
			ret = __add_delayed_refs(head, seq, &info_key,
						 &prefs_delayed);
			if (ret) {
				spin_unlock(&delayed_refs->lock);
				goto out;
			}
		}
		spin_unlock(&delayed_refs->lock);
=======
				btrfs_put_delayed_ref_head(head);
				goto again;
			}
			spin_unlock(&delayed_refs->lock);
			ret = add_delayed_refs(ctx->fs_info, head, ctx->time_seq,
					       &preftrees, sc);
			mutex_unlock(&head->mutex);
			if (ret)
				goto out;
		} else {
			spin_unlock(&delayed_refs->lock);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (path->slots[0]) {
		struct extent_buffer *leaf;
		int slot;

<<<<<<< HEAD
		leaf = path->nodes[0];
		slot = path->slots[0] - 1;
		btrfs_item_key_to_cpu(leaf, &key, slot);
		if (key.objectid == bytenr &&
		    key.type == BTRFS_EXTENT_ITEM_KEY) {
			ret = __add_inline_refs(fs_info, path, bytenr,
						&info_key, &info_level, &prefs);
			if (ret)
				goto out;
			ret = __add_keyed_refs(fs_info, path, bytenr, &info_key,
					       info_level, &prefs);
=======
		path->slots[0]--;
		leaf = path->nodes[0];
		slot = path->slots[0];
		btrfs_item_key_to_cpu(leaf, &key, slot);
		if (key.objectid == ctx->bytenr &&
		    (key.type == BTRFS_EXTENT_ITEM_KEY ||
		     key.type == BTRFS_METADATA_ITEM_KEY)) {
			ret = add_inline_refs(ctx, path, &info_level,
					      &preftrees, sc);
			if (ret)
				goto out;
			ret = add_keyed_refs(ctx, root, path, info_level,
					     &preftrees, sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (ret)
				goto out;
		}
	}
<<<<<<< HEAD
	btrfs_release_path(path);

	/*
	 * when adding the delayed refs above, the info_key might not have
	 * been known yet. Go over the list and replace the missing keys
	 */
	list_for_each_entry(ref, &prefs_delayed, list) {
		if ((ref->key.offset | ref->key.type | ref->key.objectid) == 0)
			memcpy(&ref->key, &info_key, sizeof(ref->key));
	}
	list_splice_init(&prefs_delayed, &prefs);

	ret = __merge_refs(&prefs, 1);
	if (ret)
		goto out;

	ret = __resolve_indirect_refs(fs_info, search_commit_root, &prefs);
	if (ret)
		goto out;

	ret = __merge_refs(&prefs, 2);
	if (ret)
		goto out;

	while (!list_empty(&prefs)) {
		ref = list_first_entry(&prefs, struct __prelim_ref, list);
		list_del(&ref->list);
		if (ref->count < 0)
			WARN_ON(1);
		if (ref->count && ref->root_id && ref->parent == 0) {
			/* no parent == root of tree */
			ret = ulist_add(roots, ref->root_id, 0, GFP_NOFS);
			BUG_ON(ret < 0);
		}
		if (ref->count && ref->parent) {
			ret = ulist_add(refs, ref->parent, 0, GFP_NOFS);
			BUG_ON(ret < 0);
		}
		kfree(ref);
	}

out:
	if (head)
		mutex_unlock(&head->mutex);
	btrfs_free_path(path);
	while (!list_empty(&prefs)) {
		ref = list_first_entry(&prefs, struct __prelim_ref, list);
		list_del(&ref->list);
		kfree(ref);
	}
	while (!list_empty(&prefs_delayed)) {
		ref = list_first_entry(&prefs_delayed, struct __prelim_ref,
				       list);
		list_del(&ref->list);
		kfree(ref);
	}

=======

	/*
	 * If we have a share context and we reached here, it means the extent
	 * is not directly shared (no multiple reference items for it),
	 * otherwise we would have exited earlier with a return value of
	 * BACKREF_FOUND_SHARED after processing delayed references or while
	 * processing inline or keyed references from the extent tree.
	 * The extent may however be indirectly shared through shared subtrees
	 * as a result from creating snapshots, so we determine below what is
	 * its parent node, in case we are dealing with a metadata extent, or
	 * what's the leaf (or leaves), from a fs tree, that has a file extent
	 * item pointing to it in case we are dealing with a data extent.
	 */
	ASSERT(extent_is_shared(sc) == 0);

	/*
	 * If we are here for a data extent and we have a share_check structure
	 * it means the data extent is not directly shared (does not have
	 * multiple reference items), so we have to check if a path in the fs
	 * tree (going from the root node down to the leaf that has the file
	 * extent item pointing to the data extent) is shared, that is, if any
	 * of the extent buffers in the path is referenced by other trees.
	 */
	if (sc && ctx->bytenr == sc->data_bytenr) {
		/*
		 * If our data extent is from a generation more recent than the
		 * last generation used to snapshot the root, then we know that
		 * it can not be shared through subtrees, so we can skip
		 * resolving indirect references, there's no point in
		 * determining the extent buffers for the path from the fs tree
		 * root node down to the leaf that has the file extent item that
		 * points to the data extent.
		 */
		if (sc->data_extent_gen >
		    btrfs_root_last_snapshot(&sc->root->root_item)) {
			ret = BACKREF_FOUND_NOT_SHARED;
			goto out;
		}

		/*
		 * If we are only determining if a data extent is shared or not
		 * and the corresponding file extent item is located in the same
		 * leaf as the previous file extent item, we can skip resolving
		 * indirect references for a data extent, since the fs tree path
		 * is the same (same leaf, so same path). We skip as long as the
		 * cached result for the leaf is valid and only if there's only
		 * one file extent item pointing to the data extent, because in
		 * the case of multiple file extent items, they may be located
		 * in different leaves and therefore we have multiple paths.
		 */
		if (sc->ctx->curr_leaf_bytenr == sc->ctx->prev_leaf_bytenr &&
		    sc->self_ref_count == 1) {
			bool cached;
			bool is_shared;

			cached = lookup_backref_shared_cache(sc->ctx, sc->root,
						     sc->ctx->curr_leaf_bytenr,
						     0, &is_shared);
			if (cached) {
				if (is_shared)
					ret = BACKREF_FOUND_SHARED;
				else
					ret = BACKREF_FOUND_NOT_SHARED;
				goto out;
			}
		}
	}

	btrfs_release_path(path);

	ret = add_missing_keys(ctx->fs_info, &preftrees, path->skip_locking == 0);
	if (ret)
		goto out;

	WARN_ON(!RB_EMPTY_ROOT(&preftrees.indirect_missing_keys.root.rb_root));

	ret = resolve_indirect_refs(ctx, path, &preftrees, sc);
	if (ret)
		goto out;

	WARN_ON(!RB_EMPTY_ROOT(&preftrees.indirect.root.rb_root));

	/*
	 * This walks the tree of merged and resolved refs. Tree blocks are
	 * read in as needed. Unique entries are added to the ulist, and
	 * the list of found roots is updated.
	 *
	 * We release the entire tree in one go before returning.
	 */
	node = rb_first_cached(&preftrees.direct.root);
	while (node) {
		ref = rb_entry(node, struct prelim_ref, rbnode);
		node = rb_next(&ref->rbnode);
		/*
		 * ref->count < 0 can happen here if there are delayed
		 * refs with a node->action of BTRFS_DROP_DELAYED_REF.
		 * prelim_ref_insert() relies on this when merging
		 * identical refs to keep the overall count correct.
		 * prelim_ref_insert() will merge only those refs
		 * which compare identically.  Any refs having
		 * e.g. different offsets would not be merged,
		 * and would retain their original ref->count < 0.
		 */
		if (ctx->roots && ref->count && ref->root_id && ref->parent == 0) {
			/* no parent == root of tree */
			ret = ulist_add(ctx->roots, ref->root_id, 0, GFP_NOFS);
			if (ret < 0)
				goto out;
		}
		if (ref->count && ref->parent) {
			if (!ctx->skip_inode_ref_list && !ref->inode_list &&
			    ref->level == 0) {
				struct btrfs_tree_parent_check check = { 0 };
				struct extent_buffer *eb;

				check.level = ref->level;

				eb = read_tree_block(ctx->fs_info, ref->parent,
						     &check);
				if (IS_ERR(eb)) {
					ret = PTR_ERR(eb);
					goto out;
				}
				if (!extent_buffer_uptodate(eb)) {
					free_extent_buffer(eb);
					ret = -EIO;
					goto out;
				}

				if (!path->skip_locking)
					btrfs_tree_read_lock(eb);
				ret = find_extent_in_eb(ctx, eb, &eie);
				if (!path->skip_locking)
					btrfs_tree_read_unlock(eb);
				free_extent_buffer(eb);
				if (ret == BTRFS_ITERATE_EXTENT_INODES_STOP ||
				    ret < 0)
					goto out;
				ref->inode_list = eie;
				/*
				 * We transferred the list ownership to the ref,
				 * so set to NULL to avoid a double free in case
				 * an error happens after this.
				 */
				eie = NULL;
			}
			ret = ulist_add_merge_ptr(ctx->refs, ref->parent,
						  ref->inode_list,
						  (void **)&eie, GFP_NOFS);
			if (ret < 0)
				goto out;
			if (!ret && !ctx->skip_inode_ref_list) {
				/*
				 * We've recorded that parent, so we must extend
				 * its inode list here.
				 *
				 * However if there was corruption we may not
				 * have found an eie, return an error in this
				 * case.
				 */
				ASSERT(eie);
				if (!eie) {
					ret = -EUCLEAN;
					goto out;
				}
				while (eie->next)
					eie = eie->next;
				eie->next = ref->inode_list;
			}
			eie = NULL;
			/*
			 * We have transferred the inode list ownership from
			 * this ref to the ref we added to the 'refs' ulist.
			 * So set this ref's inode list to NULL to avoid
			 * use-after-free when our caller uses it or double
			 * frees in case an error happens before we return.
			 */
			ref->inode_list = NULL;
		}
		cond_resched();
	}

out:
	btrfs_free_path(path);

	prelim_release(&preftrees.direct);
	prelim_release(&preftrees.indirect);
	prelim_release(&preftrees.indirect_missing_keys);

	if (ret == BTRFS_ITERATE_EXTENT_INODES_STOP || ret < 0)
		free_inode_elem_list(eie);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
<<<<<<< HEAD
 * Finds all leafs with a reference to the specified combination of bytenr and
 * offset. key_list_head will point to a list of corresponding keys (caller must
 * free each list element). The leafs will be stored in the leafs ulist, which
 * must be freed with ulist_free.
 *
 * returns 0 on success, <0 on error
 */
static int btrfs_find_all_leafs(struct btrfs_trans_handle *trans,
				struct btrfs_fs_info *fs_info, u64 bytenr,
				u64 num_bytes, u64 seq, struct ulist **leafs)
{
	struct ulist *tmp;
	int ret;

	tmp = ulist_alloc(GFP_NOFS);
	if (!tmp)
		return -ENOMEM;
	*leafs = ulist_alloc(GFP_NOFS);
	if (!*leafs) {
		ulist_free(tmp);
		return -ENOMEM;
	}

	ret = find_parent_nodes(trans, fs_info, bytenr, seq, *leafs, tmp);
	ulist_free(tmp);

	if (ret < 0 && ret != -ENOENT) {
		ulist_free(*leafs);
=======
 * Finds all leaves with a reference to the specified combination of
 * @ctx->bytenr and @ctx->extent_item_pos. The bytenr of the found leaves are
 * added to the ulist at @ctx->refs, and that ulist is allocated by this
 * function. The caller should free the ulist with free_leaf_list() if
 * @ctx->ignore_extent_item_pos is false, otherwise a fimple ulist_free() is
 * enough.
 *
 * Returns 0 on success and < 0 on error. On error @ctx->refs is not allocated.
 */
int btrfs_find_all_leafs(struct btrfs_backref_walk_ctx *ctx)
{
	int ret;

	ASSERT(ctx->refs == NULL);

	ctx->refs = ulist_alloc(GFP_NOFS);
	if (!ctx->refs)
		return -ENOMEM;

	ret = find_parent_nodes(ctx, NULL);
	if (ret == BTRFS_ITERATE_EXTENT_INODES_STOP ||
	    (ret < 0 && ret != -ENOENT)) {
		free_leaf_list(ctx->refs);
		ctx->refs = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;
	}

	return 0;
}

/*
<<<<<<< HEAD
 * walk all backrefs for a given extent to find all roots that reference this
=======
 * Walk all backrefs for a given extent to find all roots that reference this
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * extent. Walking a backref means finding all extents that reference this
 * extent and in turn walk the backrefs of those, too. Naturally this is a
 * recursive process, but here it is implemented in an iterative fashion: We
 * find all referencing extents for the extent in question and put them on a
 * list. In turn, we find all referencing extents for those, further appending
 * to the list. The way we iterate the list allows adding more elements after
 * the current while iterating. The process stops when we reach the end of the
<<<<<<< HEAD
 * list. Found roots are added to the roots list.
 *
 * returns 0 on success, < 0 on error.
 */
int btrfs_find_all_roots(struct btrfs_trans_handle *trans,
				struct btrfs_fs_info *fs_info, u64 bytenr,
				u64 num_bytes, u64 seq, struct ulist **roots)
{
	struct ulist *tmp;
	struct ulist_node *node = NULL;
	int ret;

	tmp = ulist_alloc(GFP_NOFS);
	if (!tmp)
		return -ENOMEM;
	*roots = ulist_alloc(GFP_NOFS);
	if (!*roots) {
		ulist_free(tmp);
		return -ENOMEM;
	}

	while (1) {
		ret = find_parent_nodes(trans, fs_info, bytenr, seq,
					tmp, *roots);
		if (ret < 0 && ret != -ENOENT) {
			ulist_free(tmp);
			ulist_free(*roots);
			return ret;
		}
		node = ulist_next(tmp, node);
		if (!node)
			break;
		bytenr = node->val;
	}

	ulist_free(tmp);
	return 0;
}


static int __inode_info(u64 inum, u64 ioff, u8 key_type,
			struct btrfs_root *fs_root, struct btrfs_path *path,
			struct btrfs_key *found_key)
{
	int ret;
	struct btrfs_key key;
	struct extent_buffer *eb;

	key.type = key_type;
	key.objectid = inum;
	key.offset = ioff;

	ret = btrfs_search_slot(NULL, fs_root, &key, path, 0, 0);
	if (ret < 0)
		return ret;

	eb = path->nodes[0];
	if (ret && path->slots[0] >= btrfs_header_nritems(eb)) {
		ret = btrfs_next_leaf(fs_root, path);
		if (ret)
			return ret;
		eb = path->nodes[0];
	}

	btrfs_item_key_to_cpu(eb, found_key, path->slots[0]);
	if (found_key->type != key.type || found_key->objectid != key.objectid)
		return 1;

	return 0;
}

/*
 * this makes the path point to (inum INODE_ITEM ioff)
 */
int inode_item_info(u64 inum, u64 ioff, struct btrfs_root *fs_root,
			struct btrfs_path *path)
{
	struct btrfs_key key;
	return __inode_info(inum, ioff, BTRFS_INODE_ITEM_KEY, fs_root, path,
				&key);
}

static int inode_ref_info(u64 inum, u64 ioff, struct btrfs_root *fs_root,
				struct btrfs_path *path,
				struct btrfs_key *found_key)
{
	return __inode_info(inum, ioff, BTRFS_INODE_REF_KEY, fs_root, path,
				found_key);
}

/*
 * this iterates to turn a btrfs_inode_ref into a full filesystem path. elements
 * of the path are separated by '/' and the path is guaranteed to be
=======
 * list.
 *
 * Found roots are added to @ctx->roots, which is allocated by this function if
 * it points to NULL, in which case the caller is responsible for freeing it
 * after it's not needed anymore.
 * This function requires @ctx->refs to be NULL, as it uses it for allocating a
 * ulist to do temporary work, and frees it before returning.
 *
 * Returns 0 on success, < 0 on error.
 */
static int btrfs_find_all_roots_safe(struct btrfs_backref_walk_ctx *ctx)
{
	const u64 orig_bytenr = ctx->bytenr;
	const bool orig_skip_inode_ref_list = ctx->skip_inode_ref_list;
	bool roots_ulist_allocated = false;
	struct ulist_iterator uiter;
	int ret = 0;

	ASSERT(ctx->refs == NULL);

	ctx->refs = ulist_alloc(GFP_NOFS);
	if (!ctx->refs)
		return -ENOMEM;

	if (!ctx->roots) {
		ctx->roots = ulist_alloc(GFP_NOFS);
		if (!ctx->roots) {
			ulist_free(ctx->refs);
			ctx->refs = NULL;
			return -ENOMEM;
		}
		roots_ulist_allocated = true;
	}

	ctx->skip_inode_ref_list = true;

	ULIST_ITER_INIT(&uiter);
	while (1) {
		struct ulist_node *node;

		ret = find_parent_nodes(ctx, NULL);
		if (ret < 0 && ret != -ENOENT) {
			if (roots_ulist_allocated) {
				ulist_free(ctx->roots);
				ctx->roots = NULL;
			}
			break;
		}
		ret = 0;
		node = ulist_next(ctx->refs, &uiter);
		if (!node)
			break;
		ctx->bytenr = node->val;
		cond_resched();
	}

	ulist_free(ctx->refs);
	ctx->refs = NULL;
	ctx->bytenr = orig_bytenr;
	ctx->skip_inode_ref_list = orig_skip_inode_ref_list;

	return ret;
}

int btrfs_find_all_roots(struct btrfs_backref_walk_ctx *ctx,
			 bool skip_commit_root_sem)
{
	int ret;

	if (!ctx->trans && !skip_commit_root_sem)
		down_read(&ctx->fs_info->commit_root_sem);
	ret = btrfs_find_all_roots_safe(ctx);
	if (!ctx->trans && !skip_commit_root_sem)
		up_read(&ctx->fs_info->commit_root_sem);
	return ret;
}

struct btrfs_backref_share_check_ctx *btrfs_alloc_backref_share_check_ctx(void)
{
	struct btrfs_backref_share_check_ctx *ctx;

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return NULL;

	ulist_init(&ctx->refs);

	return ctx;
}

void btrfs_free_backref_share_ctx(struct btrfs_backref_share_check_ctx *ctx)
{
	if (!ctx)
		return;

	ulist_release(&ctx->refs);
	kfree(ctx);
}

/*
 * Check if a data extent is shared or not.
 *
 * @inode:       The inode whose extent we are checking.
 * @bytenr:      Logical bytenr of the extent we are checking.
 * @extent_gen:  Generation of the extent (file extent item) or 0 if it is
 *               not known.
 * @ctx:         A backref sharedness check context.
 *
 * btrfs_is_data_extent_shared uses the backref walking code but will short
 * circuit as soon as it finds a root or inode that doesn't match the
 * one passed in. This provides a significant performance benefit for
 * callers (such as fiemap) which want to know whether the extent is
 * shared but do not need a ref count.
 *
 * This attempts to attach to the running transaction in order to account for
 * delayed refs, but continues on even when no running transaction exists.
 *
 * Return: 0 if extent is not shared, 1 if it is shared, < 0 on error.
 */
int btrfs_is_data_extent_shared(struct btrfs_inode *inode, u64 bytenr,
				u64 extent_gen,
				struct btrfs_backref_share_check_ctx *ctx)
{
	struct btrfs_backref_walk_ctx walk_ctx = { 0 };
	struct btrfs_root *root = inode->root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct btrfs_trans_handle *trans;
	struct ulist_iterator uiter;
	struct ulist_node *node;
	struct btrfs_seq_list elem = BTRFS_SEQ_LIST_INIT(elem);
	int ret = 0;
	struct share_check shared = {
		.ctx = ctx,
		.root = root,
		.inum = btrfs_ino(inode),
		.data_bytenr = bytenr,
		.data_extent_gen = extent_gen,
		.share_count = 0,
		.self_ref_count = 0,
		.have_delayed_delete_refs = false,
	};
	int level;
	bool leaf_cached;
	bool leaf_is_shared;

	for (int i = 0; i < BTRFS_BACKREF_CTX_PREV_EXTENTS_SIZE; i++) {
		if (ctx->prev_extents_cache[i].bytenr == bytenr)
			return ctx->prev_extents_cache[i].is_shared;
	}

	ulist_init(&ctx->refs);

	trans = btrfs_join_transaction_nostart(root);
	if (IS_ERR(trans)) {
		if (PTR_ERR(trans) != -ENOENT && PTR_ERR(trans) != -EROFS) {
			ret = PTR_ERR(trans);
			goto out;
		}
		trans = NULL;
		down_read(&fs_info->commit_root_sem);
	} else {
		btrfs_get_tree_mod_seq(fs_info, &elem);
		walk_ctx.time_seq = elem.seq;
	}

	ctx->use_path_cache = true;

	/*
	 * We may have previously determined that the current leaf is shared.
	 * If it is, then we have a data extent that is shared due to a shared
	 * subtree (caused by snapshotting) and we don't need to check for data
	 * backrefs. If the leaf is not shared, then we must do backref walking
	 * to determine if the data extent is shared through reflinks.
	 */
	leaf_cached = lookup_backref_shared_cache(ctx, root,
						  ctx->curr_leaf_bytenr, 0,
						  &leaf_is_shared);
	if (leaf_cached && leaf_is_shared) {
		ret = 1;
		goto out_trans;
	}

	walk_ctx.skip_inode_ref_list = true;
	walk_ctx.trans = trans;
	walk_ctx.fs_info = fs_info;
	walk_ctx.refs = &ctx->refs;

	/* -1 means we are in the bytenr of the data extent. */
	level = -1;
	ULIST_ITER_INIT(&uiter);
	while (1) {
		const unsigned long prev_ref_count = ctx->refs.nnodes;

		walk_ctx.bytenr = bytenr;
		ret = find_parent_nodes(&walk_ctx, &shared);
		if (ret == BACKREF_FOUND_SHARED ||
		    ret == BACKREF_FOUND_NOT_SHARED) {
			/* If shared must return 1, otherwise return 0. */
			ret = (ret == BACKREF_FOUND_SHARED) ? 1 : 0;
			if (level >= 0)
				store_backref_shared_cache(ctx, root, bytenr,
							   level, ret == 1);
			break;
		}
		if (ret < 0 && ret != -ENOENT)
			break;
		ret = 0;

		/*
		 * More than one extent buffer (bytenr) may have been added to
		 * the ctx->refs ulist, in which case we have to check multiple
		 * tree paths in case the first one is not shared, so we can not
		 * use the path cache which is made for a single path. Multiple
		 * extent buffers at the current level happen when:
		 *
		 * 1) level -1, the data extent: If our data extent was not
		 *    directly shared (without multiple reference items), then
		 *    it might have a single reference item with a count > 1 for
		 *    the same offset, which means there are 2 (or more) file
		 *    extent items that point to the data extent - this happens
		 *    when a file extent item needs to be split and then one
		 *    item gets moved to another leaf due to a b+tree leaf split
		 *    when inserting some item. In this case the file extent
		 *    items may be located in different leaves and therefore
		 *    some of the leaves may be referenced through shared
		 *    subtrees while others are not. Since our extent buffer
		 *    cache only works for a single path (by far the most common
		 *    case and simpler to deal with), we can not use it if we
		 *    have multiple leaves (which implies multiple paths).
		 *
		 * 2) level >= 0, a tree node/leaf: We can have a mix of direct
		 *    and indirect references on a b+tree node/leaf, so we have
		 *    to check multiple paths, and the extent buffer (the
		 *    current bytenr) may be shared or not. One example is
		 *    during relocation as we may get a shared tree block ref
		 *    (direct ref) and a non-shared tree block ref (indirect
		 *    ref) for the same node/leaf.
		 */
		if ((ctx->refs.nnodes - prev_ref_count) > 1)
			ctx->use_path_cache = false;

		if (level >= 0)
			store_backref_shared_cache(ctx, root, bytenr,
						   level, false);
		node = ulist_next(&ctx->refs, &uiter);
		if (!node)
			break;
		bytenr = node->val;
		if (ctx->use_path_cache) {
			bool is_shared;
			bool cached;

			level++;
			cached = lookup_backref_shared_cache(ctx, root, bytenr,
							     level, &is_shared);
			if (cached) {
				ret = (is_shared ? 1 : 0);
				break;
			}
		}
		shared.share_count = 0;
		shared.have_delayed_delete_refs = false;
		cond_resched();
	}

	/*
	 * If the path cache is disabled, then it means at some tree level we
	 * got multiple parents due to a mix of direct and indirect backrefs or
	 * multiple leaves with file extent items pointing to the same data
	 * extent. We have to invalidate the cache and cache only the sharedness
	 * result for the levels where we got only one node/reference.
	 */
	if (!ctx->use_path_cache) {
		int i = 0;

		level--;
		if (ret >= 0 && level >= 0) {
			bytenr = ctx->path_cache_entries[level].bytenr;
			ctx->use_path_cache = true;
			store_backref_shared_cache(ctx, root, bytenr, level, ret);
			i = level + 1;
		}

		for ( ; i < BTRFS_MAX_LEVEL; i++)
			ctx->path_cache_entries[i].bytenr = 0;
	}

	/*
	 * Cache the sharedness result for the data extent if we know our inode
	 * has more than 1 file extent item that refers to the data extent.
	 */
	if (ret >= 0 && shared.self_ref_count > 1) {
		int slot = ctx->prev_extents_cache_slot;

		ctx->prev_extents_cache[slot].bytenr = shared.data_bytenr;
		ctx->prev_extents_cache[slot].is_shared = (ret == 1);

		slot = (slot + 1) % BTRFS_BACKREF_CTX_PREV_EXTENTS_SIZE;
		ctx->prev_extents_cache_slot = slot;
	}

out_trans:
	if (trans) {
		btrfs_put_tree_mod_seq(fs_info, &elem);
		btrfs_end_transaction(trans);
	} else {
		up_read(&fs_info->commit_root_sem);
	}
out:
	ulist_release(&ctx->refs);
	ctx->prev_leaf_bytenr = ctx->curr_leaf_bytenr;

	return ret;
}

int btrfs_find_one_extref(struct btrfs_root *root, u64 inode_objectid,
			  u64 start_off, struct btrfs_path *path,
			  struct btrfs_inode_extref **ret_extref,
			  u64 *found_off)
{
	int ret, slot;
	struct btrfs_key key;
	struct btrfs_key found_key;
	struct btrfs_inode_extref *extref;
	const struct extent_buffer *leaf;
	unsigned long ptr;

	key.objectid = inode_objectid;
	key.type = BTRFS_INODE_EXTREF_KEY;
	key.offset = start_off;

	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
	if (ret < 0)
		return ret;

	while (1) {
		leaf = path->nodes[0];
		slot = path->slots[0];
		if (slot >= btrfs_header_nritems(leaf)) {
			/*
			 * If the item at offset is not found,
			 * btrfs_search_slot will point us to the slot
			 * where it should be inserted. In our case
			 * that will be the slot directly before the
			 * next INODE_REF_KEY_V2 item. In the case
			 * that we're pointing to the last slot in a
			 * leaf, we must move one leaf over.
			 */
			ret = btrfs_next_leaf(root, path);
			if (ret) {
				if (ret >= 1)
					ret = -ENOENT;
				break;
			}
			continue;
		}

		btrfs_item_key_to_cpu(leaf, &found_key, slot);

		/*
		 * Check that we're still looking at an extended ref key for
		 * this particular objectid. If we have different
		 * objectid or type then there are no more to be found
		 * in the tree and we can exit.
		 */
		ret = -ENOENT;
		if (found_key.objectid != inode_objectid)
			break;
		if (found_key.type != BTRFS_INODE_EXTREF_KEY)
			break;

		ret = 0;
		ptr = btrfs_item_ptr_offset(leaf, path->slots[0]);
		extref = (struct btrfs_inode_extref *)ptr;
		*ret_extref = extref;
		if (found_off)
			*found_off = found_key.offset;
		break;
	}

	return ret;
}

/*
 * this iterates to turn a name (from iref/extref) into a full filesystem path.
 * Elements of the path are separated by '/' and the path is guaranteed to be
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * 0-terminated. the path is only given within the current file system.
 * Therefore, it never starts with a '/'. the caller is responsible to provide
 * "size" bytes in "dest". the dest buffer will be filled backwards. finally,
 * the start point of the resulting string is returned. this pointer is within
 * dest, normally.
 * in case the path buffer would overflow, the pointer is decremented further
 * as if output was written to the buffer, though no more output is actually
 * generated. that way, the caller can determine how much space would be
 * required for the path to fit into the buffer. in that case, the returned
 * value will be smaller than dest. callers must check this!
 */
<<<<<<< HEAD
static char *iref_to_path(struct btrfs_root *fs_root, struct btrfs_path *path,
				struct btrfs_inode_ref *iref,
				struct extent_buffer *eb_in, u64 parent,
				char *dest, u32 size)
{
	u32 len;
	int slot;
	u64 next_inum;
	int ret;
	s64 bytes_left = size - 1;
	struct extent_buffer *eb = eb_in;
	struct btrfs_key found_key;
	int leave_spinning = path->leave_spinning;
=======
char *btrfs_ref_to_path(struct btrfs_root *fs_root, struct btrfs_path *path,
			u32 name_len, unsigned long name_off,
			struct extent_buffer *eb_in, u64 parent,
			char *dest, u32 size)
{
	int slot;
	u64 next_inum;
	int ret;
	s64 bytes_left = ((s64)size) - 1;
	struct extent_buffer *eb = eb_in;
	struct btrfs_key found_key;
	struct btrfs_inode_ref *iref;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (bytes_left >= 0)
		dest[bytes_left] = '\0';

<<<<<<< HEAD
	path->leave_spinning = 1;
	while (1) {
		len = btrfs_inode_ref_name_len(eb, iref);
		bytes_left -= len;
		if (bytes_left >= 0)
			read_extent_buffer(eb, dest + bytes_left,
						(unsigned long)(iref + 1), len);
		if (eb != eb_in) {
			btrfs_tree_read_unlock_blocking(eb);
			free_extent_buffer(eb);
		}
		ret = inode_ref_info(parent, 0, fs_root, path, &found_key);
=======
	while (1) {
		bytes_left -= name_len;
		if (bytes_left >= 0)
			read_extent_buffer(eb, dest + bytes_left,
					   name_off, name_len);
		if (eb != eb_in) {
			if (!path->skip_locking)
				btrfs_tree_read_unlock(eb);
			free_extent_buffer(eb);
		}
		ret = btrfs_find_item(fs_root, path, parent, 0,
				BTRFS_INODE_REF_KEY, &found_key);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret > 0)
			ret = -ENOENT;
		if (ret)
			break;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		next_inum = found_key.offset;

		/* regular exit ahead */
		if (parent == next_inum)
			break;

		slot = path->slots[0];
		eb = path->nodes[0];
		/* make sure we can use eb after releasing the path */
		if (eb != eb_in) {
<<<<<<< HEAD
			atomic_inc(&eb->refs);
			btrfs_tree_read_lock(eb);
			btrfs_set_lock_blocking_rw(eb, BTRFS_READ_LOCK);
		}
		btrfs_release_path(path);

		iref = btrfs_item_ptr(eb, slot, struct btrfs_inode_ref);
=======
			path->nodes[0] = NULL;
			path->locks[0] = 0;
		}
		btrfs_release_path(path);
		iref = btrfs_item_ptr(eb, slot, struct btrfs_inode_ref);

		name_len = btrfs_inode_ref_name_len(eb, iref);
		name_off = (unsigned long)(iref + 1);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		parent = next_inum;
		--bytes_left;
		if (bytes_left >= 0)
			dest[bytes_left] = '/';
	}

	btrfs_release_path(path);
<<<<<<< HEAD
	path->leave_spinning = leave_spinning;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ret)
		return ERR_PTR(ret);

	return dest + bytes_left;
}

/*
 * this makes the path point to (logical EXTENT_ITEM *)
 * returns BTRFS_EXTENT_FLAG_DATA for data, BTRFS_EXTENT_FLAG_TREE_BLOCK for
 * tree blocks and <0 on error.
 */
int extent_from_logical(struct btrfs_fs_info *fs_info, u64 logical,
<<<<<<< HEAD
			struct btrfs_path *path, struct btrfs_key *found_key)
{
	int ret;
	u64 flags;
	u32 item_size;
	struct extent_buffer *eb;
	struct btrfs_extent_item *ei;
	struct btrfs_key key;

	key.type = BTRFS_EXTENT_ITEM_KEY;
	key.objectid = logical;
	key.offset = (u64)-1;

	ret = btrfs_search_slot(NULL, fs_info->extent_root, &key, path, 0, 0);
	if (ret < 0)
		return ret;
	ret = btrfs_previous_item(fs_info->extent_root, path,
					0, BTRFS_EXTENT_ITEM_KEY);
	if (ret < 0)
		return ret;

	btrfs_item_key_to_cpu(path->nodes[0], found_key, path->slots[0]);
	if (found_key->type != BTRFS_EXTENT_ITEM_KEY ||
	    found_key->objectid > logical ||
	    found_key->objectid + found_key->offset <= logical) {
		pr_debug("logical %llu is not within any extent\n",
			 (unsigned long long)logical);
=======
			struct btrfs_path *path, struct btrfs_key *found_key,
			u64 *flags_ret)
{
	struct btrfs_root *extent_root = btrfs_extent_root(fs_info, logical);
	int ret;
	u64 flags;
	u64 size = 0;
	u32 item_size;
	const struct extent_buffer *eb;
	struct btrfs_extent_item *ei;
	struct btrfs_key key;

	if (btrfs_fs_incompat(fs_info, SKINNY_METADATA))
		key.type = BTRFS_METADATA_ITEM_KEY;
	else
		key.type = BTRFS_EXTENT_ITEM_KEY;
	key.objectid = logical;
	key.offset = (u64)-1;

	ret = btrfs_search_slot(NULL, extent_root, &key, path, 0, 0);
	if (ret < 0)
		return ret;
	if (ret == 0) {
		/*
		 * Key with offset -1 found, there would have to exist an extent
		 * item with such offset, but this is out of the valid range.
		 */
		return -EUCLEAN;
	}

	ret = btrfs_previous_extent_item(extent_root, path, 0);
	if (ret) {
		if (ret > 0)
			ret = -ENOENT;
		return ret;
	}
	btrfs_item_key_to_cpu(path->nodes[0], found_key, path->slots[0]);
	if (found_key->type == BTRFS_METADATA_ITEM_KEY)
		size = fs_info->nodesize;
	else if (found_key->type == BTRFS_EXTENT_ITEM_KEY)
		size = found_key->offset;

	if (found_key->objectid > logical ||
	    found_key->objectid + size <= logical) {
		btrfs_debug(fs_info,
			"logical %llu is not within any extent", logical);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOENT;
	}

	eb = path->nodes[0];
<<<<<<< HEAD
	item_size = btrfs_item_size_nr(eb, path->slots[0]);
	BUG_ON(item_size < sizeof(*ei));
=======
	item_size = btrfs_item_size(eb, path->slots[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ei = btrfs_item_ptr(eb, path->slots[0], struct btrfs_extent_item);
	flags = btrfs_extent_flags(eb, ei);

<<<<<<< HEAD
	pr_debug("logical %llu is at position %llu within the extent (%llu "
		 "EXTENT_ITEM %llu) flags %#llx size %u\n",
		 (unsigned long long)logical,
		 (unsigned long long)(logical - found_key->objectid),
		 (unsigned long long)found_key->objectid,
		 (unsigned long long)found_key->offset,
		 (unsigned long long)flags, item_size);
	if (flags & BTRFS_EXTENT_FLAG_TREE_BLOCK)
		return BTRFS_EXTENT_FLAG_TREE_BLOCK;
	if (flags & BTRFS_EXTENT_FLAG_DATA)
		return BTRFS_EXTENT_FLAG_DATA;
=======
	btrfs_debug(fs_info,
		"logical %llu is at position %llu within the extent (%llu EXTENT_ITEM %llu) flags %#llx size %u",
		 logical, logical - found_key->objectid, found_key->objectid,
		 found_key->offset, flags, item_size);

	WARN_ON(!flags_ret);
	if (flags_ret) {
		if (flags & BTRFS_EXTENT_FLAG_TREE_BLOCK)
			*flags_ret = BTRFS_EXTENT_FLAG_TREE_BLOCK;
		else if (flags & BTRFS_EXTENT_FLAG_DATA)
			*flags_ret = BTRFS_EXTENT_FLAG_DATA;
		else
			BUG();
		return 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return -EIO;
}

/*
 * helper function to iterate extent inline refs. ptr must point to a 0 value
 * for the first call and may be modified. it is used to track state.
 * if more refs exist, 0 is returned and the next call to
<<<<<<< HEAD
 * __get_extent_inline_ref must pass the modified ptr parameter to get the
 * next ref. after the last ref was processed, 1 is returned.
 * returns <0 on error
 */
static int __get_extent_inline_ref(unsigned long *ptr, struct extent_buffer *eb,
				struct btrfs_extent_item *ei, u32 item_size,
				struct btrfs_extent_inline_ref **out_eiref,
				int *out_type)
=======
 * get_extent_inline_ref must pass the modified ptr parameter to get the
 * next ref. after the last ref was processed, 1 is returned.
 * returns <0 on error
 */
static int get_extent_inline_ref(unsigned long *ptr,
				 const struct extent_buffer *eb,
				 const struct btrfs_key *key,
				 const struct btrfs_extent_item *ei,
				 u32 item_size,
				 struct btrfs_extent_inline_ref **out_eiref,
				 int *out_type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long end;
	u64 flags;
	struct btrfs_tree_block_info *info;

	if (!*ptr) {
		/* first call */
		flags = btrfs_extent_flags(eb, ei);
		if (flags & BTRFS_EXTENT_FLAG_TREE_BLOCK) {
<<<<<<< HEAD
			info = (struct btrfs_tree_block_info *)(ei + 1);
			*out_eiref =
				(struct btrfs_extent_inline_ref *)(info + 1);
=======
			if (key->type == BTRFS_METADATA_ITEM_KEY) {
				/* a skinny metadata extent */
				*out_eiref =
				     (struct btrfs_extent_inline_ref *)(ei + 1);
			} else {
				WARN_ON(key->type != BTRFS_EXTENT_ITEM_KEY);
				info = (struct btrfs_tree_block_info *)(ei + 1);
				*out_eiref =
				   (struct btrfs_extent_inline_ref *)(info + 1);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			*out_eiref = (struct btrfs_extent_inline_ref *)(ei + 1);
		}
		*ptr = (unsigned long)*out_eiref;
		if ((unsigned long)(*ptr) >= (unsigned long)ei + item_size)
			return -ENOENT;
	}

	end = (unsigned long)ei + item_size;
<<<<<<< HEAD
	*out_eiref = (struct btrfs_extent_inline_ref *)*ptr;
	*out_type = btrfs_extent_inline_ref_type(eb, *out_eiref);
=======
	*out_eiref = (struct btrfs_extent_inline_ref *)(*ptr);
	*out_type = btrfs_get_extent_inline_ref_type(eb, *out_eiref,
						     BTRFS_REF_TYPE_ANY);
	if (*out_type == BTRFS_REF_TYPE_INVALID)
		return -EUCLEAN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*ptr += btrfs_extent_inline_ref_size(*out_type);
	WARN_ON(*ptr > end);
	if (*ptr == end)
		return 1; /* last */

	return 0;
}

/*
 * reads the tree block backref for an extent. tree level and root are returned
 * through out_level and out_root. ptr must point to a 0 value for the first
<<<<<<< HEAD
 * call and may be modified (see __get_extent_inline_ref comment).
=======
 * call and may be modified (see get_extent_inline_ref comment).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * returns 0 if data was provided, 1 if there was no more data to provide or
 * <0 on error.
 */
int tree_backref_for_extent(unsigned long *ptr, struct extent_buffer *eb,
<<<<<<< HEAD
				struct btrfs_extent_item *ei, u32 item_size,
				u64 *out_root, u8 *out_level)
{
	int ret;
	int type;
	struct btrfs_tree_block_info *info;
=======
			    struct btrfs_key *key, struct btrfs_extent_item *ei,
			    u32 item_size, u64 *out_root, u8 *out_level)
{
	int ret;
	int type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct btrfs_extent_inline_ref *eiref;

	if (*ptr == (unsigned long)-1)
		return 1;

	while (1) {
<<<<<<< HEAD
		ret = __get_extent_inline_ref(ptr, eb, ei, item_size,
						&eiref, &type);
=======
		ret = get_extent_inline_ref(ptr, eb, key, ei, item_size,
					      &eiref, &type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret < 0)
			return ret;

		if (type == BTRFS_TREE_BLOCK_REF_KEY ||
		    type == BTRFS_SHARED_BLOCK_REF_KEY)
			break;

		if (ret == 1)
			return 1;
	}

	/* we can treat both ref types equally here */
<<<<<<< HEAD
	info = (struct btrfs_tree_block_info *)(ei + 1);
	*out_root = btrfs_extent_inline_ref_offset(eb, eiref);
	*out_level = btrfs_tree_block_level(eb, info);
=======
	*out_root = btrfs_extent_inline_ref_offset(eb, eiref);

	if (key->type == BTRFS_EXTENT_ITEM_KEY) {
		struct btrfs_tree_block_info *info;

		info = (struct btrfs_tree_block_info *)(ei + 1);
		*out_level = btrfs_tree_block_level(eb, info);
	} else {
		ASSERT(key->type == BTRFS_METADATA_ITEM_KEY);
		*out_level = (u8)key->offset;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ret == 1)
		*ptr = (unsigned long)-1;

	return 0;
}

<<<<<<< HEAD
static int iterate_leaf_refs(struct btrfs_fs_info *fs_info, u64 logical,
				u64 orig_extent_item_objectid,
				u64 extent_item_pos, u64 root,
				iterate_extent_inodes_t *iterate, void *ctx)
{
	u64 disk_byte;
	struct btrfs_key key;
	struct btrfs_file_extent_item *fi;
	struct extent_buffer *eb;
	int slot;
	int nritems;
	int ret = 0;
	int extent_type;
	u64 data_offset;
	u64 data_len;

	eb = read_tree_block(fs_info->tree_root, logical,
				fs_info->tree_root->leafsize, 0);
	if (!eb)
		return -EIO;

	/*
	 * from the shared data ref, we only have the leaf but we need
	 * the key. thus, we must look into all items and see that we
	 * find one (some) with a reference to our extent item.
	 */
	nritems = btrfs_header_nritems(eb);
	for (slot = 0; slot < nritems; ++slot) {
		btrfs_item_key_to_cpu(eb, &key, slot);
		if (key.type != BTRFS_EXTENT_DATA_KEY)
			continue;
		fi = btrfs_item_ptr(eb, slot, struct btrfs_file_extent_item);
		extent_type = btrfs_file_extent_type(eb, fi);
		if (extent_type == BTRFS_FILE_EXTENT_INLINE)
			continue;
		/* don't skip BTRFS_FILE_EXTENT_PREALLOC, we can handle that */
		disk_byte = btrfs_file_extent_disk_bytenr(eb, fi);
		if (disk_byte != orig_extent_item_objectid)
			continue;

		data_offset = btrfs_file_extent_offset(eb, fi);
		data_len = btrfs_file_extent_num_bytes(eb, fi);

		if (extent_item_pos < data_offset ||
		    extent_item_pos >= data_offset + data_len)
			continue;

		pr_debug("ref for %llu resolved, key (%llu EXTEND_DATA %llu), "
				"root %llu\n", orig_extent_item_objectid,
				key.objectid, key.offset, root);
		ret = iterate(key.objectid,
				key.offset + (extent_item_pos - data_offset),
				root, ctx);
		if (ret) {
			pr_debug("stopping iteration because ret=%d\n", ret);
=======
static int iterate_leaf_refs(struct btrfs_fs_info *fs_info,
			     struct extent_inode_elem *inode_list,
			     u64 root, u64 extent_item_objectid,
			     iterate_extent_inodes_t *iterate, void *ctx)
{
	struct extent_inode_elem *eie;
	int ret = 0;

	for (eie = inode_list; eie; eie = eie->next) {
		btrfs_debug(fs_info,
			    "ref for %llu resolved, key (%llu EXTEND_DATA %llu), root %llu",
			    extent_item_objectid, eie->inum,
			    eie->offset, root);
		ret = iterate(eie->inum, eie->offset, eie->num_bytes, root, ctx);
		if (ret) {
			btrfs_debug(fs_info,
				    "stopping iteration for %llu due to ret=%d",
				    extent_item_objectid, ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}

<<<<<<< HEAD
	free_extent_buffer(eb);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * calls iterate() for every inode that references the extent identified by
 * the given parameters.
 * when the iterator function returns a non-zero value, iteration stops.
 */
<<<<<<< HEAD
int iterate_extent_inodes(struct btrfs_fs_info *fs_info,
				u64 extent_item_objectid, u64 extent_item_pos,
				int search_commit_root,
				iterate_extent_inodes_t *iterate, void *ctx)
{
	int ret;
	struct list_head data_refs = LIST_HEAD_INIT(data_refs);
	struct list_head shared_refs = LIST_HEAD_INIT(shared_refs);
	struct btrfs_trans_handle *trans;
	struct ulist *refs = NULL;
	struct ulist *roots = NULL;
	struct ulist_node *ref_node = NULL;
	struct ulist_node *root_node = NULL;
	struct seq_list seq_elem;
	struct btrfs_delayed_ref_root *delayed_refs = NULL;

	pr_debug("resolving all inodes for extent %llu\n",
			extent_item_objectid);

	if (search_commit_root) {
		trans = BTRFS_BACKREF_SEARCH_COMMIT_ROOT;
	} else {
		trans = btrfs_join_transaction(fs_info->extent_root);
		if (IS_ERR(trans))
			return PTR_ERR(trans);

		delayed_refs = &trans->transaction->delayed_refs;
		spin_lock(&delayed_refs->lock);
		btrfs_get_delayed_seq(delayed_refs, &seq_elem);
		spin_unlock(&delayed_refs->lock);
	}

	ret = btrfs_find_all_leafs(trans, fs_info, extent_item_objectid,
				   extent_item_pos, seq_elem.seq,
				   &refs);

	if (ret)
		goto out;

	while (!ret && (ref_node = ulist_next(refs, ref_node))) {
		ret = btrfs_find_all_roots(trans, fs_info, ref_node->val, -1,
						seq_elem.seq, &roots);
		if (ret)
			break;
		while (!ret && (root_node = ulist_next(roots, root_node))) {
			pr_debug("root %llu references leaf %llu\n",
					root_node->val, ref_node->val);
			ret = iterate_leaf_refs(fs_info, ref_node->val,
						extent_item_objectid,
						extent_item_pos, root_node->val,
						iterate, ctx);
		}
	}

	ulist_free(refs);
	ulist_free(roots);
out:
	if (!search_commit_root) {
		btrfs_put_delayed_seq(delayed_refs, &seq_elem);
		btrfs_end_transaction(trans, fs_info->extent_root);
	}

	return ret;
}

int iterate_inodes_from_logical(u64 logical, struct btrfs_fs_info *fs_info,
				struct btrfs_path *path,
				iterate_extent_inodes_t *iterate, void *ctx)
{
	int ret;
	u64 extent_item_pos;
	struct btrfs_key found_key;
	int search_commit_root = path->search_commit_root;

	ret = extent_from_logical(fs_info, logical, path,
					&found_key);
	btrfs_release_path(path);
	if (ret & BTRFS_EXTENT_FLAG_TREE_BLOCK)
		ret = -EINVAL;
	if (ret < 0)
		return ret;

	extent_item_pos = logical - found_key.objectid;
	ret = iterate_extent_inodes(fs_info, found_key.objectid,
					extent_item_pos, search_commit_root,
					iterate, ctx);

	return ret;
}

static int iterate_irefs(u64 inum, struct btrfs_root *fs_root,
				struct btrfs_path *path,
				iterate_irefs_t *iterate, void *ctx)
=======
int iterate_extent_inodes(struct btrfs_backref_walk_ctx *ctx,
			  bool search_commit_root,
			  iterate_extent_inodes_t *iterate, void *user_ctx)
{
	int ret;
	struct ulist *refs;
	struct ulist_node *ref_node;
	struct btrfs_seq_list seq_elem = BTRFS_SEQ_LIST_INIT(seq_elem);
	struct ulist_iterator ref_uiter;

	btrfs_debug(ctx->fs_info, "resolving all inodes for extent %llu",
		    ctx->bytenr);

	ASSERT(ctx->trans == NULL);
	ASSERT(ctx->roots == NULL);

	if (!search_commit_root) {
		struct btrfs_trans_handle *trans;

		trans = btrfs_attach_transaction(ctx->fs_info->tree_root);
		if (IS_ERR(trans)) {
			if (PTR_ERR(trans) != -ENOENT &&
			    PTR_ERR(trans) != -EROFS)
				return PTR_ERR(trans);
			trans = NULL;
		}
		ctx->trans = trans;
	}

	if (ctx->trans) {
		btrfs_get_tree_mod_seq(ctx->fs_info, &seq_elem);
		ctx->time_seq = seq_elem.seq;
	} else {
		down_read(&ctx->fs_info->commit_root_sem);
	}

	ret = btrfs_find_all_leafs(ctx);
	if (ret)
		goto out;
	refs = ctx->refs;
	ctx->refs = NULL;

	ULIST_ITER_INIT(&ref_uiter);
	while (!ret && (ref_node = ulist_next(refs, &ref_uiter))) {
		const u64 leaf_bytenr = ref_node->val;
		struct ulist_node *root_node;
		struct ulist_iterator root_uiter;
		struct extent_inode_elem *inode_list;

		inode_list = (struct extent_inode_elem *)(uintptr_t)ref_node->aux;

		if (ctx->cache_lookup) {
			const u64 *root_ids;
			int root_count;
			bool cached;

			cached = ctx->cache_lookup(leaf_bytenr, ctx->user_ctx,
						   &root_ids, &root_count);
			if (cached) {
				for (int i = 0; i < root_count; i++) {
					ret = iterate_leaf_refs(ctx->fs_info,
								inode_list,
								root_ids[i],
								leaf_bytenr,
								iterate,
								user_ctx);
					if (ret)
						break;
				}
				continue;
			}
		}

		if (!ctx->roots) {
			ctx->roots = ulist_alloc(GFP_NOFS);
			if (!ctx->roots) {
				ret = -ENOMEM;
				break;
			}
		}

		ctx->bytenr = leaf_bytenr;
		ret = btrfs_find_all_roots_safe(ctx);
		if (ret)
			break;

		if (ctx->cache_store)
			ctx->cache_store(leaf_bytenr, ctx->roots, ctx->user_ctx);

		ULIST_ITER_INIT(&root_uiter);
		while (!ret && (root_node = ulist_next(ctx->roots, &root_uiter))) {
			btrfs_debug(ctx->fs_info,
				    "root %llu references leaf %llu, data list %#llx",
				    root_node->val, ref_node->val,
				    ref_node->aux);
			ret = iterate_leaf_refs(ctx->fs_info, inode_list,
						root_node->val, ctx->bytenr,
						iterate, user_ctx);
		}
		ulist_reinit(ctx->roots);
	}

	free_leaf_list(refs);
out:
	if (ctx->trans) {
		btrfs_put_tree_mod_seq(ctx->fs_info, &seq_elem);
		btrfs_end_transaction(ctx->trans);
		ctx->trans = NULL;
	} else {
		up_read(&ctx->fs_info->commit_root_sem);
	}

	ulist_free(ctx->roots);
	ctx->roots = NULL;

	if (ret == BTRFS_ITERATE_EXTENT_INODES_STOP)
		ret = 0;

	return ret;
}

static int build_ino_list(u64 inum, u64 offset, u64 num_bytes, u64 root, void *ctx)
{
	struct btrfs_data_container *inodes = ctx;
	const size_t c = 3 * sizeof(u64);

	if (inodes->bytes_left >= c) {
		inodes->bytes_left -= c;
		inodes->val[inodes->elem_cnt] = inum;
		inodes->val[inodes->elem_cnt + 1] = offset;
		inodes->val[inodes->elem_cnt + 2] = root;
		inodes->elem_cnt += 3;
	} else {
		inodes->bytes_missing += c - inodes->bytes_left;
		inodes->bytes_left = 0;
		inodes->elem_missed += 3;
	}

	return 0;
}

int iterate_inodes_from_logical(u64 logical, struct btrfs_fs_info *fs_info,
				struct btrfs_path *path,
				void *ctx, bool ignore_offset)
{
	struct btrfs_backref_walk_ctx walk_ctx = { 0 };
	int ret;
	u64 flags = 0;
	struct btrfs_key found_key;
	int search_commit_root = path->search_commit_root;

	ret = extent_from_logical(fs_info, logical, path, &found_key, &flags);
	btrfs_release_path(path);
	if (ret < 0)
		return ret;
	if (flags & BTRFS_EXTENT_FLAG_TREE_BLOCK)
		return -EINVAL;

	walk_ctx.bytenr = found_key.objectid;
	if (ignore_offset)
		walk_ctx.ignore_extent_item_pos = true;
	else
		walk_ctx.extent_item_pos = logical - found_key.objectid;
	walk_ctx.fs_info = fs_info;

	return iterate_extent_inodes(&walk_ctx, search_commit_root,
				     build_ino_list, ctx);
}

static int inode_to_path(u64 inum, u32 name_len, unsigned long name_off,
			 struct extent_buffer *eb, struct inode_fs_paths *ipath);

static int iterate_inode_refs(u64 inum, struct inode_fs_paths *ipath)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret = 0;
	int slot;
	u32 cur;
	u32 len;
	u32 name_len;
	u64 parent = 0;
	int found = 0;
<<<<<<< HEAD
	struct extent_buffer *eb;
	struct btrfs_item *item;
=======
	struct btrfs_root *fs_root = ipath->fs_root;
	struct btrfs_path *path = ipath->btrfs_path;
	struct extent_buffer *eb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct btrfs_inode_ref *iref;
	struct btrfs_key found_key;

	while (!ret) {
<<<<<<< HEAD
		path->leave_spinning = 1;
		ret = inode_ref_info(inum, parent ? parent+1 : 0, fs_root, path,
					&found_key);
=======
		ret = btrfs_find_item(fs_root, path, inum,
				parent ? parent + 1 : 0, BTRFS_INODE_REF_KEY,
				&found_key);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret < 0)
			break;
		if (ret) {
			ret = found ? 0 : -ENOENT;
			break;
		}
		++found;

		parent = found_key.offset;
		slot = path->slots[0];
<<<<<<< HEAD
		eb = path->nodes[0];
		/* make sure we can use eb after releasing the path */
		atomic_inc(&eb->refs);
		btrfs_tree_read_lock(eb);
		btrfs_set_lock_blocking_rw(eb, BTRFS_READ_LOCK);
		btrfs_release_path(path);

		item = btrfs_item_nr(eb, slot);
		iref = btrfs_item_ptr(eb, slot, struct btrfs_inode_ref);

		for (cur = 0; cur < btrfs_item_size(eb, item); cur += len) {
			name_len = btrfs_inode_ref_name_len(eb, iref);
			/* path must be released before calling iterate()! */
			pr_debug("following ref at offset %u for inode %llu in "
				 "tree %llu\n", cur,
				 (unsigned long long)found_key.objectid,
				 (unsigned long long)fs_root->objectid);
			ret = iterate(parent, iref, eb, ctx);
=======
		eb = btrfs_clone_extent_buffer(path->nodes[0]);
		if (!eb) {
			ret = -ENOMEM;
			break;
		}
		btrfs_release_path(path);

		iref = btrfs_item_ptr(eb, slot, struct btrfs_inode_ref);

		for (cur = 0; cur < btrfs_item_size(eb, slot); cur += len) {
			name_len = btrfs_inode_ref_name_len(eb, iref);
			/* path must be released before calling iterate()! */
			btrfs_debug(fs_root->fs_info,
				"following ref at offset %u for inode %llu in tree %llu",
				cur, found_key.objectid,
				fs_root->root_key.objectid);
			ret = inode_to_path(parent, name_len,
				      (unsigned long)(iref + 1), eb, ipath);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (ret)
				break;
			len = sizeof(*iref) + name_len;
			iref = (struct btrfs_inode_ref *)((char *)iref + len);
		}
<<<<<<< HEAD
		btrfs_tree_read_unlock_blocking(eb);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		free_extent_buffer(eb);
	}

	btrfs_release_path(path);

	return ret;
}

<<<<<<< HEAD
=======
static int iterate_inode_extrefs(u64 inum, struct inode_fs_paths *ipath)
{
	int ret;
	int slot;
	u64 offset = 0;
	u64 parent;
	int found = 0;
	struct btrfs_root *fs_root = ipath->fs_root;
	struct btrfs_path *path = ipath->btrfs_path;
	struct extent_buffer *eb;
	struct btrfs_inode_extref *extref;
	u32 item_size;
	u32 cur_offset;
	unsigned long ptr;

	while (1) {
		ret = btrfs_find_one_extref(fs_root, inum, offset, path, &extref,
					    &offset);
		if (ret < 0)
			break;
		if (ret) {
			ret = found ? 0 : -ENOENT;
			break;
		}
		++found;

		slot = path->slots[0];
		eb = btrfs_clone_extent_buffer(path->nodes[0]);
		if (!eb) {
			ret = -ENOMEM;
			break;
		}
		btrfs_release_path(path);

		item_size = btrfs_item_size(eb, slot);
		ptr = btrfs_item_ptr_offset(eb, slot);
		cur_offset = 0;

		while (cur_offset < item_size) {
			u32 name_len;

			extref = (struct btrfs_inode_extref *)(ptr + cur_offset);
			parent = btrfs_inode_extref_parent(eb, extref);
			name_len = btrfs_inode_extref_name_len(eb, extref);
			ret = inode_to_path(parent, name_len,
				      (unsigned long)&extref->name, eb, ipath);
			if (ret)
				break;

			cur_offset += btrfs_inode_extref_name_len(eb, extref);
			cur_offset += sizeof(*extref);
		}
		free_extent_buffer(eb);

		offset++;
	}

	btrfs_release_path(path);

	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * returns 0 if the path could be dumped (probably truncated)
 * returns <0 in case of an error
 */
<<<<<<< HEAD
static int inode_to_path(u64 inum, struct btrfs_inode_ref *iref,
				struct extent_buffer *eb, void *ctx)
{
	struct inode_fs_paths *ipath = ctx;
=======
static int inode_to_path(u64 inum, u32 name_len, unsigned long name_off,
			 struct extent_buffer *eb, struct inode_fs_paths *ipath)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *fspath;
	char *fspath_min;
	int i = ipath->fspath->elem_cnt;
	const int s_ptr = sizeof(char *);
	u32 bytes_left;

	bytes_left = ipath->fspath->bytes_left > s_ptr ?
					ipath->fspath->bytes_left - s_ptr : 0;

	fspath_min = (char *)ipath->fspath->val + (i + 1) * s_ptr;
<<<<<<< HEAD
	fspath = iref_to_path(ipath->fs_root, ipath->btrfs_path, iref, eb,
				inum, fspath_min, bytes_left);
=======
	fspath = btrfs_ref_to_path(ipath->fs_root, ipath->btrfs_path, name_len,
				   name_off, eb, inum, fspath_min, bytes_left);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(fspath))
		return PTR_ERR(fspath);

	if (fspath > fspath_min) {
<<<<<<< HEAD
		pr_debug("path resolved: %s\n", fspath);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ipath->fspath->val[i] = (u64)(unsigned long)fspath;
		++ipath->fspath->elem_cnt;
		ipath->fspath->bytes_left = fspath - fspath_min;
	} else {
<<<<<<< HEAD
		pr_debug("missed path, not enough space. missing bytes: %lu, "
			 "constructed so far: %s\n",
			 (unsigned long)(fspath_min - fspath), fspath_min);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		++ipath->fspath->elem_missed;
		ipath->fspath->bytes_missing += fspath_min - fspath;
		ipath->fspath->bytes_left = 0;
	}

	return 0;
}

/*
 * this dumps all file system paths to the inode into the ipath struct, provided
 * is has been created large enough. each path is zero-terminated and accessed
 * from ipath->fspath->val[i].
 * when it returns, there are ipath->fspath->elem_cnt number of paths available
 * in ipath->fspath->val[]. when the allocated space wasn't sufficient, the
<<<<<<< HEAD
 * number of missed paths in recored in ipath->fspath->elem_missed, otherwise,
=======
 * number of missed paths is recorded in ipath->fspath->elem_missed, otherwise,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * it's zero. ipath->fspath->bytes_missing holds the number of bytes that would
 * have been needed to return all paths.
 */
int paths_from_inode(u64 inum, struct inode_fs_paths *ipath)
{
<<<<<<< HEAD
	return iterate_irefs(inum, ipath->fs_root, ipath->btrfs_path,
				inode_to_path, ipath);
=======
	int ret;
	int found_refs = 0;

	ret = iterate_inode_refs(inum, ipath);
	if (!ret)
		++found_refs;
	else if (ret != -ENOENT)
		return ret;

	ret = iterate_inode_extrefs(inum, ipath);
	if (ret == -ENOENT && found_refs)
		return 0;

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct btrfs_data_container *init_data_container(u32 total_bytes)
{
	struct btrfs_data_container *data;
	size_t alloc_bytes;

	alloc_bytes = max_t(size_t, total_bytes, sizeof(*data));
<<<<<<< HEAD
	data = kmalloc(alloc_bytes, GFP_NOFS);
	if (!data)
		return ERR_PTR(-ENOMEM);

	if (total_bytes >= sizeof(*data)) {
		data->bytes_left = total_bytes - sizeof(*data);
		data->bytes_missing = 0;
	} else {
		data->bytes_missing = sizeof(*data) - total_bytes;
		data->bytes_left = 0;
	}

	data->elem_cnt = 0;
	data->elem_missed = 0;
=======
	data = kvzalloc(alloc_bytes, GFP_KERNEL);
	if (!data)
		return ERR_PTR(-ENOMEM);

	if (total_bytes >= sizeof(*data))
		data->bytes_left = total_bytes - sizeof(*data);
	else
		data->bytes_missing = sizeof(*data) - total_bytes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return data;
}

/*
 * allocates space to return multiple file system paths for an inode.
 * total_bytes to allocate are passed, note that space usable for actual path
 * information will be total_bytes - sizeof(struct inode_fs_paths).
 * the returned pointer must be freed with free_ipath() in the end.
 */
struct inode_fs_paths *init_ipath(s32 total_bytes, struct btrfs_root *fs_root,
					struct btrfs_path *path)
{
	struct inode_fs_paths *ifp;
	struct btrfs_data_container *fspath;

	fspath = init_data_container(total_bytes);
	if (IS_ERR(fspath))
<<<<<<< HEAD
		return (void *)fspath;

	ifp = kmalloc(sizeof(*ifp), GFP_NOFS);
	if (!ifp) {
		kfree(fspath);
=======
		return ERR_CAST(fspath);

	ifp = kmalloc(sizeof(*ifp), GFP_KERNEL);
	if (!ifp) {
		kvfree(fspath);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ERR_PTR(-ENOMEM);
	}

	ifp->btrfs_path = path;
	ifp->fspath = fspath;
	ifp->fs_root = fs_root;

	return ifp;
}

void free_ipath(struct inode_fs_paths *ipath)
{
	if (!ipath)
		return;
<<<<<<< HEAD
	kfree(ipath->fspath);
	kfree(ipath);
}
=======
	kvfree(ipath->fspath);
	kfree(ipath);
}

struct btrfs_backref_iter *btrfs_backref_iter_alloc(struct btrfs_fs_info *fs_info)
{
	struct btrfs_backref_iter *ret;

	ret = kzalloc(sizeof(*ret), GFP_NOFS);
	if (!ret)
		return NULL;

	ret->path = btrfs_alloc_path();
	if (!ret->path) {
		kfree(ret);
		return NULL;
	}

	/* Current backref iterator only supports iteration in commit root */
	ret->path->search_commit_root = 1;
	ret->path->skip_locking = 1;
	ret->fs_info = fs_info;

	return ret;
}

static void btrfs_backref_iter_release(struct btrfs_backref_iter *iter)
{
	iter->bytenr = 0;
	iter->item_ptr = 0;
	iter->cur_ptr = 0;
	iter->end_ptr = 0;
	btrfs_release_path(iter->path);
	memset(&iter->cur_key, 0, sizeof(iter->cur_key));
}

int btrfs_backref_iter_start(struct btrfs_backref_iter *iter, u64 bytenr)
{
	struct btrfs_fs_info *fs_info = iter->fs_info;
	struct btrfs_root *extent_root = btrfs_extent_root(fs_info, bytenr);
	struct btrfs_path *path = iter->path;
	struct btrfs_extent_item *ei;
	struct btrfs_key key;
	int ret;

	key.objectid = bytenr;
	key.type = BTRFS_METADATA_ITEM_KEY;
	key.offset = (u64)-1;
	iter->bytenr = bytenr;

	ret = btrfs_search_slot(NULL, extent_root, &key, path, 0, 0);
	if (ret < 0)
		return ret;
	if (ret == 0) {
		/*
		 * Key with offset -1 found, there would have to exist an extent
		 * item with such offset, but this is out of the valid range.
		 */
		ret = -EUCLEAN;
		goto release;
	}
	if (path->slots[0] == 0) {
		WARN_ON(IS_ENABLED(CONFIG_BTRFS_DEBUG));
		ret = -EUCLEAN;
		goto release;
	}
	path->slots[0]--;

	btrfs_item_key_to_cpu(path->nodes[0], &key, path->slots[0]);
	if ((key.type != BTRFS_EXTENT_ITEM_KEY &&
	     key.type != BTRFS_METADATA_ITEM_KEY) || key.objectid != bytenr) {
		ret = -ENOENT;
		goto release;
	}
	memcpy(&iter->cur_key, &key, sizeof(key));
	iter->item_ptr = (u32)btrfs_item_ptr_offset(path->nodes[0],
						    path->slots[0]);
	iter->end_ptr = (u32)(iter->item_ptr +
			btrfs_item_size(path->nodes[0], path->slots[0]));
	ei = btrfs_item_ptr(path->nodes[0], path->slots[0],
			    struct btrfs_extent_item);

	/*
	 * Only support iteration on tree backref yet.
	 *
	 * This is an extra precaution for non skinny-metadata, where
	 * EXTENT_ITEM is also used for tree blocks, that we can only use
	 * extent flags to determine if it's a tree block.
	 */
	if (btrfs_extent_flags(path->nodes[0], ei) & BTRFS_EXTENT_FLAG_DATA) {
		ret = -ENOTSUPP;
		goto release;
	}
	iter->cur_ptr = (u32)(iter->item_ptr + sizeof(*ei));

	/* If there is no inline backref, go search for keyed backref */
	if (iter->cur_ptr >= iter->end_ptr) {
		ret = btrfs_next_item(extent_root, path);

		/* No inline nor keyed ref */
		if (ret > 0) {
			ret = -ENOENT;
			goto release;
		}
		if (ret < 0)
			goto release;

		btrfs_item_key_to_cpu(path->nodes[0], &iter->cur_key,
				path->slots[0]);
		if (iter->cur_key.objectid != bytenr ||
		    (iter->cur_key.type != BTRFS_SHARED_BLOCK_REF_KEY &&
		     iter->cur_key.type != BTRFS_TREE_BLOCK_REF_KEY)) {
			ret = -ENOENT;
			goto release;
		}
		iter->cur_ptr = (u32)btrfs_item_ptr_offset(path->nodes[0],
							   path->slots[0]);
		iter->item_ptr = iter->cur_ptr;
		iter->end_ptr = (u32)(iter->item_ptr + btrfs_item_size(
				      path->nodes[0], path->slots[0]));
	}

	return 0;
release:
	btrfs_backref_iter_release(iter);
	return ret;
}

static bool btrfs_backref_iter_is_inline_ref(struct btrfs_backref_iter *iter)
{
	if (iter->cur_key.type == BTRFS_EXTENT_ITEM_KEY ||
	    iter->cur_key.type == BTRFS_METADATA_ITEM_KEY)
		return true;
	return false;
}

/*
 * Go to the next backref item of current bytenr, can be either inlined or
 * keyed.
 *
 * Caller needs to check whether it's inline ref or not by iter->cur_key.
 *
 * Return 0 if we get next backref without problem.
 * Return >0 if there is no extra backref for this bytenr.
 * Return <0 if there is something wrong happened.
 */
int btrfs_backref_iter_next(struct btrfs_backref_iter *iter)
{
	struct extent_buffer *eb = iter->path->nodes[0];
	struct btrfs_root *extent_root;
	struct btrfs_path *path = iter->path;
	struct btrfs_extent_inline_ref *iref;
	int ret;
	u32 size;

	if (btrfs_backref_iter_is_inline_ref(iter)) {
		/* We're still inside the inline refs */
		ASSERT(iter->cur_ptr < iter->end_ptr);

		if (btrfs_backref_has_tree_block_info(iter)) {
			/* First tree block info */
			size = sizeof(struct btrfs_tree_block_info);
		} else {
			/* Use inline ref type to determine the size */
			int type;

			iref = (struct btrfs_extent_inline_ref *)
				((unsigned long)iter->cur_ptr);
			type = btrfs_extent_inline_ref_type(eb, iref);

			size = btrfs_extent_inline_ref_size(type);
		}
		iter->cur_ptr += size;
		if (iter->cur_ptr < iter->end_ptr)
			return 0;

		/* All inline items iterated, fall through */
	}

	/* We're at keyed items, there is no inline item, go to the next one */
	extent_root = btrfs_extent_root(iter->fs_info, iter->bytenr);
	ret = btrfs_next_item(extent_root, iter->path);
	if (ret)
		return ret;

	btrfs_item_key_to_cpu(path->nodes[0], &iter->cur_key, path->slots[0]);
	if (iter->cur_key.objectid != iter->bytenr ||
	    (iter->cur_key.type != BTRFS_TREE_BLOCK_REF_KEY &&
	     iter->cur_key.type != BTRFS_SHARED_BLOCK_REF_KEY))
		return 1;
	iter->item_ptr = (u32)btrfs_item_ptr_offset(path->nodes[0],
					path->slots[0]);
	iter->cur_ptr = iter->item_ptr;
	iter->end_ptr = iter->item_ptr + (u32)btrfs_item_size(path->nodes[0],
						path->slots[0]);
	return 0;
}

void btrfs_backref_init_cache(struct btrfs_fs_info *fs_info,
			      struct btrfs_backref_cache *cache, bool is_reloc)
{
	int i;

	cache->rb_root = RB_ROOT;
	for (i = 0; i < BTRFS_MAX_LEVEL; i++)
		INIT_LIST_HEAD(&cache->pending[i]);
	INIT_LIST_HEAD(&cache->changed);
	INIT_LIST_HEAD(&cache->detached);
	INIT_LIST_HEAD(&cache->leaves);
	INIT_LIST_HEAD(&cache->pending_edge);
	INIT_LIST_HEAD(&cache->useless_node);
	cache->fs_info = fs_info;
	cache->is_reloc = is_reloc;
}

struct btrfs_backref_node *btrfs_backref_alloc_node(
		struct btrfs_backref_cache *cache, u64 bytenr, int level)
{
	struct btrfs_backref_node *node;

	ASSERT(level >= 0 && level < BTRFS_MAX_LEVEL);
	node = kzalloc(sizeof(*node), GFP_NOFS);
	if (!node)
		return node;

	INIT_LIST_HEAD(&node->list);
	INIT_LIST_HEAD(&node->upper);
	INIT_LIST_HEAD(&node->lower);
	RB_CLEAR_NODE(&node->rb_node);
	cache->nr_nodes++;
	node->level = level;
	node->bytenr = bytenr;

	return node;
}

void btrfs_backref_free_node(struct btrfs_backref_cache *cache,
			     struct btrfs_backref_node *node)
{
	if (node) {
		ASSERT(list_empty(&node->list));
		ASSERT(list_empty(&node->lower));
		ASSERT(node->eb == NULL);
		cache->nr_nodes--;
		btrfs_put_root(node->root);
		kfree(node);
	}
}

struct btrfs_backref_edge *btrfs_backref_alloc_edge(
		struct btrfs_backref_cache *cache)
{
	struct btrfs_backref_edge *edge;

	edge = kzalloc(sizeof(*edge), GFP_NOFS);
	if (edge)
		cache->nr_edges++;
	return edge;
}

void btrfs_backref_free_edge(struct btrfs_backref_cache *cache,
			     struct btrfs_backref_edge *edge)
{
	if (edge) {
		cache->nr_edges--;
		kfree(edge);
	}
}

void btrfs_backref_unlock_node_buffer(struct btrfs_backref_node *node)
{
	if (node->locked) {
		btrfs_tree_unlock(node->eb);
		node->locked = 0;
	}
}

void btrfs_backref_drop_node_buffer(struct btrfs_backref_node *node)
{
	if (node->eb) {
		btrfs_backref_unlock_node_buffer(node);
		free_extent_buffer(node->eb);
		node->eb = NULL;
	}
}

/*
 * Drop the backref node from cache without cleaning up its children
 * edges.
 *
 * This can only be called on node without parent edges.
 * The children edges are still kept as is.
 */
void btrfs_backref_drop_node(struct btrfs_backref_cache *tree,
			     struct btrfs_backref_node *node)
{
	ASSERT(list_empty(&node->upper));

	btrfs_backref_drop_node_buffer(node);
	list_del_init(&node->list);
	list_del_init(&node->lower);
	if (!RB_EMPTY_NODE(&node->rb_node))
		rb_erase(&node->rb_node, &tree->rb_root);
	btrfs_backref_free_node(tree, node);
}

/*
 * Drop the backref node from cache, also cleaning up all its
 * upper edges and any uncached nodes in the path.
 *
 * This cleanup happens bottom up, thus the node should either
 * be the lowest node in the cache or a detached node.
 */
void btrfs_backref_cleanup_node(struct btrfs_backref_cache *cache,
				struct btrfs_backref_node *node)
{
	struct btrfs_backref_node *upper;
	struct btrfs_backref_edge *edge;

	if (!node)
		return;

	BUG_ON(!node->lowest && !node->detached);
	while (!list_empty(&node->upper)) {
		edge = list_entry(node->upper.next, struct btrfs_backref_edge,
				  list[LOWER]);
		upper = edge->node[UPPER];
		list_del(&edge->list[LOWER]);
		list_del(&edge->list[UPPER]);
		btrfs_backref_free_edge(cache, edge);

		/*
		 * Add the node to leaf node list if no other child block
		 * cached.
		 */
		if (list_empty(&upper->lower)) {
			list_add_tail(&upper->lower, &cache->leaves);
			upper->lowest = 1;
		}
	}

	btrfs_backref_drop_node(cache, node);
}

/*
 * Release all nodes/edges from current cache
 */
void btrfs_backref_release_cache(struct btrfs_backref_cache *cache)
{
	struct btrfs_backref_node *node;
	int i;

	while (!list_empty(&cache->detached)) {
		node = list_entry(cache->detached.next,
				  struct btrfs_backref_node, list);
		btrfs_backref_cleanup_node(cache, node);
	}

	while (!list_empty(&cache->leaves)) {
		node = list_entry(cache->leaves.next,
				  struct btrfs_backref_node, lower);
		btrfs_backref_cleanup_node(cache, node);
	}

	cache->last_trans = 0;

	for (i = 0; i < BTRFS_MAX_LEVEL; i++)
		ASSERT(list_empty(&cache->pending[i]));
	ASSERT(list_empty(&cache->pending_edge));
	ASSERT(list_empty(&cache->useless_node));
	ASSERT(list_empty(&cache->changed));
	ASSERT(list_empty(&cache->detached));
	ASSERT(RB_EMPTY_ROOT(&cache->rb_root));
	ASSERT(!cache->nr_nodes);
	ASSERT(!cache->nr_edges);
}

void btrfs_backref_link_edge(struct btrfs_backref_edge *edge,
			     struct btrfs_backref_node *lower,
			     struct btrfs_backref_node *upper,
			     int link_which)
{
	ASSERT(upper && lower && upper->level == lower->level + 1);
	edge->node[LOWER] = lower;
	edge->node[UPPER] = upper;
	if (link_which & LINK_LOWER)
		list_add_tail(&edge->list[LOWER], &lower->upper);
	if (link_which & LINK_UPPER)
		list_add_tail(&edge->list[UPPER], &upper->lower);
}
/*
 * Handle direct tree backref
 *
 * Direct tree backref means, the backref item shows its parent bytenr
 * directly. This is for SHARED_BLOCK_REF backref (keyed or inlined).
 *
 * @ref_key:	The converted backref key.
 *		For keyed backref, it's the item key.
 *		For inlined backref, objectid is the bytenr,
 *		type is btrfs_inline_ref_type, offset is
 *		btrfs_inline_ref_offset.
 */
static int handle_direct_tree_backref(struct btrfs_backref_cache *cache,
				      struct btrfs_key *ref_key,
				      struct btrfs_backref_node *cur)
{
	struct btrfs_backref_edge *edge;
	struct btrfs_backref_node *upper;
	struct rb_node *rb_node;

	ASSERT(ref_key->type == BTRFS_SHARED_BLOCK_REF_KEY);

	/* Only reloc root uses backref pointing to itself */
	if (ref_key->objectid == ref_key->offset) {
		struct btrfs_root *root;

		cur->is_reloc_root = 1;
		/* Only reloc backref cache cares about a specific root */
		if (cache->is_reloc) {
			root = find_reloc_root(cache->fs_info, cur->bytenr);
			if (!root)
				return -ENOENT;
			cur->root = root;
		} else {
			/*
			 * For generic purpose backref cache, reloc root node
			 * is useless.
			 */
			list_add(&cur->list, &cache->useless_node);
		}
		return 0;
	}

	edge = btrfs_backref_alloc_edge(cache);
	if (!edge)
		return -ENOMEM;

	rb_node = rb_simple_search(&cache->rb_root, ref_key->offset);
	if (!rb_node) {
		/* Parent node not yet cached */
		upper = btrfs_backref_alloc_node(cache, ref_key->offset,
					   cur->level + 1);
		if (!upper) {
			btrfs_backref_free_edge(cache, edge);
			return -ENOMEM;
		}

		/*
		 *  Backrefs for the upper level block isn't cached, add the
		 *  block to pending list
		 */
		list_add_tail(&edge->list[UPPER], &cache->pending_edge);
	} else {
		/* Parent node already cached */
		upper = rb_entry(rb_node, struct btrfs_backref_node, rb_node);
		ASSERT(upper->checked);
		INIT_LIST_HEAD(&edge->list[UPPER]);
	}
	btrfs_backref_link_edge(edge, cur, upper, LINK_LOWER);
	return 0;
}

/*
 * Handle indirect tree backref
 *
 * Indirect tree backref means, we only know which tree the node belongs to.
 * We still need to do a tree search to find out the parents. This is for
 * TREE_BLOCK_REF backref (keyed or inlined).
 *
 * @trans:	Transaction handle.
 * @ref_key:	The same as @ref_key in  handle_direct_tree_backref()
 * @tree_key:	The first key of this tree block.
 * @path:	A clean (released) path, to avoid allocating path every time
 *		the function get called.
 */
static int handle_indirect_tree_backref(struct btrfs_trans_handle *trans,
					struct btrfs_backref_cache *cache,
					struct btrfs_path *path,
					struct btrfs_key *ref_key,
					struct btrfs_key *tree_key,
					struct btrfs_backref_node *cur)
{
	struct btrfs_fs_info *fs_info = cache->fs_info;
	struct btrfs_backref_node *upper;
	struct btrfs_backref_node *lower;
	struct btrfs_backref_edge *edge;
	struct extent_buffer *eb;
	struct btrfs_root *root;
	struct rb_node *rb_node;
	int level;
	bool need_check = true;
	int ret;

	root = btrfs_get_fs_root(fs_info, ref_key->offset, false);
	if (IS_ERR(root))
		return PTR_ERR(root);
	if (!test_bit(BTRFS_ROOT_SHAREABLE, &root->state))
		cur->cowonly = 1;

	if (btrfs_root_level(&root->root_item) == cur->level) {
		/* Tree root */
		ASSERT(btrfs_root_bytenr(&root->root_item) == cur->bytenr);
		/*
		 * For reloc backref cache, we may ignore reloc root.  But for
		 * general purpose backref cache, we can't rely on
		 * btrfs_should_ignore_reloc_root() as it may conflict with
		 * current running relocation and lead to missing root.
		 *
		 * For general purpose backref cache, reloc root detection is
		 * completely relying on direct backref (key->offset is parent
		 * bytenr), thus only do such check for reloc cache.
		 */
		if (btrfs_should_ignore_reloc_root(root) && cache->is_reloc) {
			btrfs_put_root(root);
			list_add(&cur->list, &cache->useless_node);
		} else {
			cur->root = root;
		}
		return 0;
	}

	level = cur->level + 1;

	/* Search the tree to find parent blocks referring to the block */
	path->search_commit_root = 1;
	path->skip_locking = 1;
	path->lowest_level = level;
	ret = btrfs_search_slot(NULL, root, tree_key, path, 0, 0);
	path->lowest_level = 0;
	if (ret < 0) {
		btrfs_put_root(root);
		return ret;
	}
	if (ret > 0 && path->slots[level] > 0)
		path->slots[level]--;

	eb = path->nodes[level];
	if (btrfs_node_blockptr(eb, path->slots[level]) != cur->bytenr) {
		btrfs_err(fs_info,
"couldn't find block (%llu) (level %d) in tree (%llu) with key (%llu %u %llu)",
			  cur->bytenr, level - 1, root->root_key.objectid,
			  tree_key->objectid, tree_key->type, tree_key->offset);
		btrfs_put_root(root);
		ret = -ENOENT;
		goto out;
	}
	lower = cur;

	/* Add all nodes and edges in the path */
	for (; level < BTRFS_MAX_LEVEL; level++) {
		if (!path->nodes[level]) {
			ASSERT(btrfs_root_bytenr(&root->root_item) ==
			       lower->bytenr);
			/* Same as previous should_ignore_reloc_root() call */
			if (btrfs_should_ignore_reloc_root(root) &&
			    cache->is_reloc) {
				btrfs_put_root(root);
				list_add(&lower->list, &cache->useless_node);
			} else {
				lower->root = root;
			}
			break;
		}

		edge = btrfs_backref_alloc_edge(cache);
		if (!edge) {
			btrfs_put_root(root);
			ret = -ENOMEM;
			goto out;
		}

		eb = path->nodes[level];
		rb_node = rb_simple_search(&cache->rb_root, eb->start);
		if (!rb_node) {
			upper = btrfs_backref_alloc_node(cache, eb->start,
							 lower->level + 1);
			if (!upper) {
				btrfs_put_root(root);
				btrfs_backref_free_edge(cache, edge);
				ret = -ENOMEM;
				goto out;
			}
			upper->owner = btrfs_header_owner(eb);
			if (!test_bit(BTRFS_ROOT_SHAREABLE, &root->state))
				upper->cowonly = 1;

			/*
			 * If we know the block isn't shared we can avoid
			 * checking its backrefs.
			 */
			if (btrfs_block_can_be_shared(trans, root, eb))
				upper->checked = 0;
			else
				upper->checked = 1;

			/*
			 * Add the block to pending list if we need to check its
			 * backrefs, we only do this once while walking up a
			 * tree as we will catch anything else later on.
			 */
			if (!upper->checked && need_check) {
				need_check = false;
				list_add_tail(&edge->list[UPPER],
					      &cache->pending_edge);
			} else {
				if (upper->checked)
					need_check = true;
				INIT_LIST_HEAD(&edge->list[UPPER]);
			}
		} else {
			upper = rb_entry(rb_node, struct btrfs_backref_node,
					 rb_node);
			ASSERT(upper->checked);
			INIT_LIST_HEAD(&edge->list[UPPER]);
			if (!upper->owner)
				upper->owner = btrfs_header_owner(eb);
		}
		btrfs_backref_link_edge(edge, lower, upper, LINK_LOWER);

		if (rb_node) {
			btrfs_put_root(root);
			break;
		}
		lower = upper;
		upper = NULL;
	}
out:
	btrfs_release_path(path);
	return ret;
}

/*
 * Add backref node @cur into @cache.
 *
 * NOTE: Even if the function returned 0, @cur is not yet cached as its upper
 *	 links aren't yet bi-directional. Needs to finish such links.
 *	 Use btrfs_backref_finish_upper_links() to finish such linkage.
 *
 * @trans:	Transaction handle.
 * @path:	Released path for indirect tree backref lookup
 * @iter:	Released backref iter for extent tree search
 * @node_key:	The first key of the tree block
 */
int btrfs_backref_add_tree_node(struct btrfs_trans_handle *trans,
				struct btrfs_backref_cache *cache,
				struct btrfs_path *path,
				struct btrfs_backref_iter *iter,
				struct btrfs_key *node_key,
				struct btrfs_backref_node *cur)
{
	struct btrfs_backref_edge *edge;
	struct btrfs_backref_node *exist;
	int ret;

	ret = btrfs_backref_iter_start(iter, cur->bytenr);
	if (ret < 0)
		return ret;
	/*
	 * We skip the first btrfs_tree_block_info, as we don't use the key
	 * stored in it, but fetch it from the tree block
	 */
	if (btrfs_backref_has_tree_block_info(iter)) {
		ret = btrfs_backref_iter_next(iter);
		if (ret < 0)
			goto out;
		/* No extra backref? This means the tree block is corrupted */
		if (ret > 0) {
			ret = -EUCLEAN;
			goto out;
		}
	}
	WARN_ON(cur->checked);
	if (!list_empty(&cur->upper)) {
		/*
		 * The backref was added previously when processing backref of
		 * type BTRFS_TREE_BLOCK_REF_KEY
		 */
		ASSERT(list_is_singular(&cur->upper));
		edge = list_entry(cur->upper.next, struct btrfs_backref_edge,
				  list[LOWER]);
		ASSERT(list_empty(&edge->list[UPPER]));
		exist = edge->node[UPPER];
		/*
		 * Add the upper level block to pending list if we need check
		 * its backrefs
		 */
		if (!exist->checked)
			list_add_tail(&edge->list[UPPER], &cache->pending_edge);
	} else {
		exist = NULL;
	}

	for (; ret == 0; ret = btrfs_backref_iter_next(iter)) {
		struct extent_buffer *eb;
		struct btrfs_key key;
		int type;

		cond_resched();
		eb = iter->path->nodes[0];

		key.objectid = iter->bytenr;
		if (btrfs_backref_iter_is_inline_ref(iter)) {
			struct btrfs_extent_inline_ref *iref;

			/* Update key for inline backref */
			iref = (struct btrfs_extent_inline_ref *)
				((unsigned long)iter->cur_ptr);
			type = btrfs_get_extent_inline_ref_type(eb, iref,
							BTRFS_REF_TYPE_BLOCK);
			if (type == BTRFS_REF_TYPE_INVALID) {
				ret = -EUCLEAN;
				goto out;
			}
			key.type = type;
			key.offset = btrfs_extent_inline_ref_offset(eb, iref);
		} else {
			key.type = iter->cur_key.type;
			key.offset = iter->cur_key.offset;
		}

		/*
		 * Parent node found and matches current inline ref, no need to
		 * rebuild this node for this inline ref
		 */
		if (exist &&
		    ((key.type == BTRFS_TREE_BLOCK_REF_KEY &&
		      exist->owner == key.offset) ||
		     (key.type == BTRFS_SHARED_BLOCK_REF_KEY &&
		      exist->bytenr == key.offset))) {
			exist = NULL;
			continue;
		}

		/* SHARED_BLOCK_REF means key.offset is the parent bytenr */
		if (key.type == BTRFS_SHARED_BLOCK_REF_KEY) {
			ret = handle_direct_tree_backref(cache, &key, cur);
			if (ret < 0)
				goto out;
		} else if (key.type == BTRFS_TREE_BLOCK_REF_KEY) {
			/*
			 * key.type == BTRFS_TREE_BLOCK_REF_KEY, inline ref
			 * offset means the root objectid. We need to search
			 * the tree to get its parent bytenr.
			 */
			ret = handle_indirect_tree_backref(trans, cache, path,
							   &key, node_key, cur);
			if (ret < 0)
				goto out;
		}
		/*
		 * Unrecognized tree backref items (if it can pass tree-checker)
		 * would be ignored.
		 */
	}
	ret = 0;
	cur->checked = 1;
	WARN_ON(exist);
out:
	btrfs_backref_iter_release(iter);
	return ret;
}

/*
 * Finish the upwards linkage created by btrfs_backref_add_tree_node()
 */
int btrfs_backref_finish_upper_links(struct btrfs_backref_cache *cache,
				     struct btrfs_backref_node *start)
{
	struct list_head *useless_node = &cache->useless_node;
	struct btrfs_backref_edge *edge;
	struct rb_node *rb_node;
	LIST_HEAD(pending_edge);

	ASSERT(start->checked);

	/* Insert this node to cache if it's not COW-only */
	if (!start->cowonly) {
		rb_node = rb_simple_insert(&cache->rb_root, start->bytenr,
					   &start->rb_node);
		if (rb_node)
			btrfs_backref_panic(cache->fs_info, start->bytenr,
					    -EEXIST);
		list_add_tail(&start->lower, &cache->leaves);
	}

	/*
	 * Use breadth first search to iterate all related edges.
	 *
	 * The starting points are all the edges of this node
	 */
	list_for_each_entry(edge, &start->upper, list[LOWER])
		list_add_tail(&edge->list[UPPER], &pending_edge);

	while (!list_empty(&pending_edge)) {
		struct btrfs_backref_node *upper;
		struct btrfs_backref_node *lower;

		edge = list_first_entry(&pending_edge,
				struct btrfs_backref_edge, list[UPPER]);
		list_del_init(&edge->list[UPPER]);
		upper = edge->node[UPPER];
		lower = edge->node[LOWER];

		/* Parent is detached, no need to keep any edges */
		if (upper->detached) {
			list_del(&edge->list[LOWER]);
			btrfs_backref_free_edge(cache, edge);

			/* Lower node is orphan, queue for cleanup */
			if (list_empty(&lower->upper))
				list_add(&lower->list, useless_node);
			continue;
		}

		/*
		 * All new nodes added in current build_backref_tree() haven't
		 * been linked to the cache rb tree.
		 * So if we have upper->rb_node populated, this means a cache
		 * hit. We only need to link the edge, as @upper and all its
		 * parents have already been linked.
		 */
		if (!RB_EMPTY_NODE(&upper->rb_node)) {
			if (upper->lowest) {
				list_del_init(&upper->lower);
				upper->lowest = 0;
			}

			list_add_tail(&edge->list[UPPER], &upper->lower);
			continue;
		}

		/* Sanity check, we shouldn't have any unchecked nodes */
		if (!upper->checked) {
			ASSERT(0);
			return -EUCLEAN;
		}

		/* Sanity check, COW-only node has non-COW-only parent */
		if (start->cowonly != upper->cowonly) {
			ASSERT(0);
			return -EUCLEAN;
		}

		/* Only cache non-COW-only (subvolume trees) tree blocks */
		if (!upper->cowonly) {
			rb_node = rb_simple_insert(&cache->rb_root, upper->bytenr,
						   &upper->rb_node);
			if (rb_node) {
				btrfs_backref_panic(cache->fs_info,
						upper->bytenr, -EEXIST);
				return -EUCLEAN;
			}
		}

		list_add_tail(&edge->list[UPPER], &upper->lower);

		/*
		 * Also queue all the parent edges of this uncached node
		 * to finish the upper linkage
		 */
		list_for_each_entry(edge, &upper->upper, list[LOWER])
			list_add_tail(&edge->list[UPPER], &pending_edge);
	}
	return 0;
}

void btrfs_backref_error_cleanup(struct btrfs_backref_cache *cache,
				 struct btrfs_backref_node *node)
{
	struct btrfs_backref_node *lower;
	struct btrfs_backref_node *upper;
	struct btrfs_backref_edge *edge;

	while (!list_empty(&cache->useless_node)) {
		lower = list_first_entry(&cache->useless_node,
				   struct btrfs_backref_node, list);
		list_del_init(&lower->list);
	}
	while (!list_empty(&cache->pending_edge)) {
		edge = list_first_entry(&cache->pending_edge,
				struct btrfs_backref_edge, list[UPPER]);
		list_del(&edge->list[UPPER]);
		list_del(&edge->list[LOWER]);
		lower = edge->node[LOWER];
		upper = edge->node[UPPER];
		btrfs_backref_free_edge(cache, edge);

		/*
		 * Lower is no longer linked to any upper backref nodes and
		 * isn't in the cache, we can free it ourselves.
		 */
		if (list_empty(&lower->upper) &&
		    RB_EMPTY_NODE(&lower->rb_node))
			list_add(&lower->list, &cache->useless_node);

		if (!RB_EMPTY_NODE(&upper->rb_node))
			continue;

		/* Add this guy's upper edges to the list to process */
		list_for_each_entry(edge, &upper->upper, list[LOWER])
			list_add_tail(&edge->list[UPPER],
				      &cache->pending_edge);
		if (list_empty(&upper->upper))
			list_add(&upper->list, &cache->useless_node);
	}

	while (!list_empty(&cache->useless_node)) {
		lower = list_first_entry(&cache->useless_node,
				   struct btrfs_backref_node, list);
		list_del_init(&lower->list);
		if (lower == node)
			node = NULL;
		btrfs_backref_drop_node(cache, lower);
	}

	btrfs_backref_cleanup_node(cache, node);
	ASSERT(list_empty(&cache->useless_node) &&
	       list_empty(&cache->pending_edge));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
