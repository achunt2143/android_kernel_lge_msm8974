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
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/slab.h>
#include <linux/blkdev.h>
#include <linux/writeback.h>
<<<<<<< HEAD
#include <linux/pagevec.h>
=======
#include <linux/sched/mm.h>
#include "messages.h"
#include "misc.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "ctree.h"
#include "transaction.h"
#include "btrfs_inode.h"
#include "extent_io.h"
<<<<<<< HEAD

static u64 entry_end(struct btrfs_ordered_extent *entry)
{
	if (entry->file_offset + entry->len < entry->file_offset)
		return (u64)-1;
	return entry->file_offset + entry->len;
=======
#include "disk-io.h"
#include "compression.h"
#include "delalloc-space.h"
#include "qgroup.h"
#include "subpage.h"
#include "file.h"

static struct kmem_cache *btrfs_ordered_extent_cache;

static u64 entry_end(struct btrfs_ordered_extent *entry)
{
	if (entry->file_offset + entry->num_bytes < entry->file_offset)
		return (u64)-1;
	return entry->file_offset + entry->num_bytes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* returns NULL if the insertion worked, or it returns the node it did find
 * in the tree
 */
static struct rb_node *tree_insert(struct rb_root *root, u64 file_offset,
				   struct rb_node *node)
{
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;
	struct btrfs_ordered_extent *entry;

	while (*p) {
		parent = *p;
		entry = rb_entry(parent, struct btrfs_ordered_extent, rb_node);

		if (file_offset < entry->file_offset)
			p = &(*p)->rb_left;
		else if (file_offset >= entry_end(entry))
			p = &(*p)->rb_right;
		else
			return parent;
	}

	rb_link_node(node, parent, p);
	rb_insert_color(node, root);
	return NULL;
}

<<<<<<< HEAD
static void ordered_data_tree_panic(struct inode *inode, int errno,
					       u64 offset)
{
	struct btrfs_fs_info *fs_info = btrfs_sb(inode->i_sb);
	btrfs_panic(fs_info, errno, "Inconsistency in ordered tree at offset "
		    "%llu\n", (unsigned long long)offset);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * look for a given offset in the tree, and if it can't be found return the
 * first lesser offset
 */
static struct rb_node *__tree_search(struct rb_root *root, u64 file_offset,
				     struct rb_node **prev_ret)
{
	struct rb_node *n = root->rb_node;
	struct rb_node *prev = NULL;
	struct rb_node *test;
	struct btrfs_ordered_extent *entry;
	struct btrfs_ordered_extent *prev_entry = NULL;

	while (n) {
		entry = rb_entry(n, struct btrfs_ordered_extent, rb_node);
		prev = n;
		prev_entry = entry;

		if (file_offset < entry->file_offset)
			n = n->rb_left;
		else if (file_offset >= entry_end(entry))
			n = n->rb_right;
		else
			return n;
	}
	if (!prev_ret)
		return NULL;

	while (prev && file_offset >= entry_end(prev_entry)) {
		test = rb_next(prev);
		if (!test)
			break;
		prev_entry = rb_entry(test, struct btrfs_ordered_extent,
				      rb_node);
		if (file_offset < entry_end(prev_entry))
			break;

		prev = test;
	}
	if (prev)
		prev_entry = rb_entry(prev, struct btrfs_ordered_extent,
				      rb_node);
	while (prev && file_offset < entry_end(prev_entry)) {
		test = rb_prev(prev);
		if (!test)
			break;
		prev_entry = rb_entry(test, struct btrfs_ordered_extent,
				      rb_node);
		prev = test;
	}
	*prev_ret = prev;
	return NULL;
}

<<<<<<< HEAD
/*
 * helper to check if a given offset is inside a given entry
 */
static int offset_in_entry(struct btrfs_ordered_extent *entry, u64 file_offset)
{
	if (file_offset < entry->file_offset ||
	    entry->file_offset + entry->len <= file_offset)
		return 0;
	return 1;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int range_overlaps(struct btrfs_ordered_extent *entry, u64 file_offset,
			  u64 len)
{
	if (file_offset + len <= entry->file_offset ||
<<<<<<< HEAD
	    entry->file_offset + entry->len <= file_offset)
=======
	    entry->file_offset + entry->num_bytes <= file_offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	return 1;
}

/*
 * look find the first ordered struct that has this offset, otherwise
 * the first one less than this offset
 */
<<<<<<< HEAD
static inline struct rb_node *tree_search(struct btrfs_ordered_inode_tree *tree,
					  u64 file_offset)
{
	struct rb_root *root = &tree->tree;
=======
static inline struct rb_node *ordered_tree_search(struct btrfs_inode *inode,
						  u64 file_offset)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rb_node *prev = NULL;
	struct rb_node *ret;
	struct btrfs_ordered_extent *entry;

<<<<<<< HEAD
	if (tree->last) {
		entry = rb_entry(tree->last, struct btrfs_ordered_extent,
				 rb_node);
		if (offset_in_entry(entry, file_offset))
			return tree->last;
	}
	ret = __tree_search(root, file_offset, &prev);
	if (!ret)
		ret = prev;
	if (ret)
		tree->last = ret;
	return ret;
}

/* allocate and add a new ordered_extent into the per-inode tree.
 * file_offset is the logical offset in the file
 *
 * start is the disk block number of an extent already reserved in the
 * extent allocation tree
 *
 * len is the length of the extent
 *
 * The tree is given a single reference on the ordered extent that was
 * inserted.
 */
static int __btrfs_add_ordered_extent(struct inode *inode, u64 file_offset,
				      u64 start, u64 len, u64 disk_len,
				      int type, int dio, int compress_type)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry;

	tree = &BTRFS_I(inode)->ordered_tree;
	entry = kzalloc(sizeof(*entry), GFP_NOFS);
	if (!entry)
		return -ENOMEM;

	entry->file_offset = file_offset;
	entry->start = start;
	entry->len = len;
	entry->disk_len = disk_len;
	entry->bytes_left = len;
	entry->inode = inode;
	entry->compress_type = compress_type;
	if (type != BTRFS_ORDERED_IO_DONE && type != BTRFS_ORDERED_COMPLETE)
		set_bit(type, &entry->flags);

	if (dio)
		set_bit(BTRFS_ORDERED_DIRECT, &entry->flags);

	/* one ref for the tree */
	atomic_set(&entry->refs, 1);
	init_waitqueue_head(&entry->wait);
	INIT_LIST_HEAD(&entry->list);
	INIT_LIST_HEAD(&entry->root_extent_list);

	trace_btrfs_ordered_extent_add(inode, entry);

	spin_lock(&tree->lock);
	node = tree_insert(&tree->tree, file_offset,
			   &entry->rb_node);
	if (node)
		ordered_data_tree_panic(inode, -EEXIST, file_offset);
	spin_unlock(&tree->lock);

	spin_lock(&BTRFS_I(inode)->root->fs_info->ordered_extent_lock);
	list_add_tail(&entry->root_extent_list,
		      &BTRFS_I(inode)->root->fs_info->ordered_extents);
	spin_unlock(&BTRFS_I(inode)->root->fs_info->ordered_extent_lock);

	return 0;
}

int btrfs_add_ordered_extent(struct inode *inode, u64 file_offset,
			     u64 start, u64 len, u64 disk_len, int type)
{
	return __btrfs_add_ordered_extent(inode, file_offset, start, len,
					  disk_len, type, 0,
					  BTRFS_COMPRESS_NONE);
}

int btrfs_add_ordered_extent_dio(struct inode *inode, u64 file_offset,
				 u64 start, u64 len, u64 disk_len, int type)
{
	return __btrfs_add_ordered_extent(inode, file_offset, start, len,
					  disk_len, type, 1,
					  BTRFS_COMPRESS_NONE);
}

int btrfs_add_ordered_extent_compress(struct inode *inode, u64 file_offset,
				      u64 start, u64 len, u64 disk_len,
				      int type, int compress_type)
{
	return __btrfs_add_ordered_extent(inode, file_offset, start, len,
					  disk_len, type, 0,
					  compress_type);
=======
	if (inode->ordered_tree_last) {
		entry = rb_entry(inode->ordered_tree_last, struct btrfs_ordered_extent,
				 rb_node);
		if (in_range(file_offset, entry->file_offset, entry->num_bytes))
			return inode->ordered_tree_last;
	}
	ret = __tree_search(&inode->ordered_tree, file_offset, &prev);
	if (!ret)
		ret = prev;
	if (ret)
		inode->ordered_tree_last = ret;
	return ret;
}

static struct btrfs_ordered_extent *alloc_ordered_extent(
			struct btrfs_inode *inode, u64 file_offset, u64 num_bytes,
			u64 ram_bytes, u64 disk_bytenr, u64 disk_num_bytes,
			u64 offset, unsigned long flags, int compress_type)
{
	struct btrfs_ordered_extent *entry;
	int ret;
	u64 qgroup_rsv = 0;

	if (flags &
	    ((1 << BTRFS_ORDERED_NOCOW) | (1 << BTRFS_ORDERED_PREALLOC))) {
		/* For nocow write, we can release the qgroup rsv right now */
		ret = btrfs_qgroup_free_data(inode, NULL, file_offset, num_bytes, &qgroup_rsv);
		if (ret < 0)
			return ERR_PTR(ret);
	} else {
		/*
		 * The ordered extent has reserved qgroup space, release now
		 * and pass the reserved number for qgroup_record to free.
		 */
		ret = btrfs_qgroup_release_data(inode, file_offset, num_bytes, &qgroup_rsv);
		if (ret < 0)
			return ERR_PTR(ret);
	}
	entry = kmem_cache_zalloc(btrfs_ordered_extent_cache, GFP_NOFS);
	if (!entry)
		return ERR_PTR(-ENOMEM);

	entry->file_offset = file_offset;
	entry->num_bytes = num_bytes;
	entry->ram_bytes = ram_bytes;
	entry->disk_bytenr = disk_bytenr;
	entry->disk_num_bytes = disk_num_bytes;
	entry->offset = offset;
	entry->bytes_left = num_bytes;
	entry->inode = igrab(&inode->vfs_inode);
	entry->compress_type = compress_type;
	entry->truncated_len = (u64)-1;
	entry->qgroup_rsv = qgroup_rsv;
	entry->flags = flags;
	refcount_set(&entry->refs, 1);
	init_waitqueue_head(&entry->wait);
	INIT_LIST_HEAD(&entry->list);
	INIT_LIST_HEAD(&entry->log_list);
	INIT_LIST_HEAD(&entry->root_extent_list);
	INIT_LIST_HEAD(&entry->work_list);
	INIT_LIST_HEAD(&entry->bioc_list);
	init_completion(&entry->completion);

	/*
	 * We don't need the count_max_extents here, we can assume that all of
	 * that work has been done at higher layers, so this is truly the
	 * smallest the extent is going to get.
	 */
	spin_lock(&inode->lock);
	btrfs_mod_outstanding_extents(inode, 1);
	spin_unlock(&inode->lock);

	return entry;
}

static void insert_ordered_extent(struct btrfs_ordered_extent *entry)
{
	struct btrfs_inode *inode = BTRFS_I(entry->inode);
	struct btrfs_root *root = inode->root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct rb_node *node;

	trace_btrfs_ordered_extent_add(inode, entry);

	percpu_counter_add_batch(&fs_info->ordered_bytes, entry->num_bytes,
				 fs_info->delalloc_batch);

	/* One ref for the tree. */
	refcount_inc(&entry->refs);

	spin_lock_irq(&inode->ordered_tree_lock);
	node = tree_insert(&inode->ordered_tree, entry->file_offset,
			   &entry->rb_node);
	if (node)
		btrfs_panic(fs_info, -EEXIST,
				"inconsistency in ordered tree at offset %llu",
				entry->file_offset);
	spin_unlock_irq(&inode->ordered_tree_lock);

	spin_lock(&root->ordered_extent_lock);
	list_add_tail(&entry->root_extent_list,
		      &root->ordered_extents);
	root->nr_ordered_extents++;
	if (root->nr_ordered_extents == 1) {
		spin_lock(&fs_info->ordered_root_lock);
		BUG_ON(!list_empty(&root->ordered_root));
		list_add_tail(&root->ordered_root, &fs_info->ordered_roots);
		spin_unlock(&fs_info->ordered_root_lock);
	}
	spin_unlock(&root->ordered_extent_lock);
}

/*
 * Add an ordered extent to the per-inode tree.
 *
 * @inode:           Inode that this extent is for.
 * @file_offset:     Logical offset in file where the extent starts.
 * @num_bytes:       Logical length of extent in file.
 * @ram_bytes:       Full length of unencoded data.
 * @disk_bytenr:     Offset of extent on disk.
 * @disk_num_bytes:  Size of extent on disk.
 * @offset:          Offset into unencoded data where file data starts.
 * @flags:           Flags specifying type of extent (1 << BTRFS_ORDERED_*).
 * @compress_type:   Compression algorithm used for data.
 *
 * Most of these parameters correspond to &struct btrfs_file_extent_item. The
 * tree is given a single reference on the ordered extent that was inserted, and
 * the returned pointer is given a second reference.
 *
 * Return: the new ordered extent or error pointer.
 */
struct btrfs_ordered_extent *btrfs_alloc_ordered_extent(
			struct btrfs_inode *inode, u64 file_offset,
			u64 num_bytes, u64 ram_bytes, u64 disk_bytenr,
			u64 disk_num_bytes, u64 offset, unsigned long flags,
			int compress_type)
{
	struct btrfs_ordered_extent *entry;

	ASSERT((flags & ~BTRFS_ORDERED_TYPE_FLAGS) == 0);

	entry = alloc_ordered_extent(inode, file_offset, num_bytes, ram_bytes,
				     disk_bytenr, disk_num_bytes, offset, flags,
				     compress_type);
	if (!IS_ERR(entry))
		insert_ordered_extent(entry);
	return entry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Add a struct btrfs_ordered_sum into the list of checksums to be inserted
 * when an ordered extent is finished.  If the list covers more than one
 * ordered extent, it is split across multiples.
 */
<<<<<<< HEAD
void btrfs_add_ordered_sum(struct inode *inode,
			   struct btrfs_ordered_extent *entry,
			   struct btrfs_ordered_sum *sum)
{
	struct btrfs_ordered_inode_tree *tree;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
	list_add_tail(&sum->list, &entry->list);
	spin_unlock(&tree->lock);
}

/*
 * this is used to account for finished IO across a given range
 * of the file.  The IO may span ordered extents.  If
 * a given ordered_extent is completely done, 1 is returned, otherwise
 * 0.
 *
 * test_and_set_bit on a flag in the struct btrfs_ordered_extent is used
 * to make sure this function only returns 1 once for a given ordered extent.
 *
 * file_offset is updated to one byte past the range that is recorded as
 * complete.  This allows you to walk forward in the file.
 */
int btrfs_dec_test_first_ordered_pending(struct inode *inode,
				   struct btrfs_ordered_extent **cached,
				   u64 *file_offset, u64 io_size)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	int ret;
	u64 dec_end;
	u64 dec_start;
	u64 to_dec;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
	node = tree_search(tree, *file_offset);
	if (!node) {
		ret = 1;
		goto out;
	}

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
	if (!offset_in_entry(entry, *file_offset)) {
		ret = 1;
		goto out;
	}

	dec_start = max(*file_offset, entry->file_offset);
	dec_end = min(*file_offset + io_size, entry->file_offset +
		      entry->len);
	*file_offset = dec_end;
	if (dec_start > dec_end) {
		printk(KERN_CRIT "bad ordering dec_start %llu end %llu\n",
		       (unsigned long long)dec_start,
		       (unsigned long long)dec_end);
	}
	to_dec = dec_end - dec_start;
	if (to_dec > entry->bytes_left) {
		printk(KERN_CRIT "bad ordered accounting left %llu size %llu\n",
		       (unsigned long long)entry->bytes_left,
		       (unsigned long long)to_dec);
	}
	entry->bytes_left -= to_dec;
	if (entry->bytes_left == 0)
		ret = test_and_set_bit(BTRFS_ORDERED_IO_DONE, &entry->flags);
	else
		ret = 1;
out:
	if (!ret && cached && entry) {
		*cached = entry;
		atomic_inc(&entry->refs);
	}
	spin_unlock(&tree->lock);
	return ret == 0;
}

/*
 * this is used to account for finished IO across a given range
 * of the file.  The IO should not span ordered extents.  If
 * a given ordered_extent is completely done, 1 is returned, otherwise
 * 0.
 *
 * test_and_set_bit on a flag in the struct btrfs_ordered_extent is used
 * to make sure this function only returns 1 once for a given ordered extent.
 */
int btrfs_dec_test_ordered_pending(struct inode *inode,
				   struct btrfs_ordered_extent **cached,
				   u64 file_offset, u64 io_size)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	int ret;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
	node = tree_search(tree, file_offset);
	if (!node) {
		ret = 1;
		goto out;
	}

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
	if (!offset_in_entry(entry, file_offset)) {
		ret = 1;
		goto out;
	}

	if (io_size > entry->bytes_left) {
		printk(KERN_CRIT "bad ordered accounting left %llu size %llu\n",
		       (unsigned long long)entry->bytes_left,
		       (unsigned long long)io_size);
	}
	entry->bytes_left -= io_size;
	if (entry->bytes_left == 0)
		ret = test_and_set_bit(BTRFS_ORDERED_IO_DONE, &entry->flags);
	else
		ret = 1;
out:
	if (!ret && cached && entry) {
		*cached = entry;
		atomic_inc(&entry->refs);
	}
	spin_unlock(&tree->lock);
	return ret == 0;
=======
void btrfs_add_ordered_sum(struct btrfs_ordered_extent *entry,
			   struct btrfs_ordered_sum *sum)
{
	struct btrfs_inode *inode = BTRFS_I(entry->inode);

	spin_lock_irq(&inode->ordered_tree_lock);
	list_add_tail(&sum->list, &entry->list);
	spin_unlock_irq(&inode->ordered_tree_lock);
}

static void finish_ordered_fn(struct btrfs_work *work)
{
	struct btrfs_ordered_extent *ordered_extent;

	ordered_extent = container_of(work, struct btrfs_ordered_extent, work);
	btrfs_finish_ordered_io(ordered_extent);
}

static bool can_finish_ordered_extent(struct btrfs_ordered_extent *ordered,
				      struct page *page, u64 file_offset,
				      u64 len, bool uptodate)
{
	struct btrfs_inode *inode = BTRFS_I(ordered->inode);
	struct btrfs_fs_info *fs_info = inode->root->fs_info;

	lockdep_assert_held(&inode->ordered_tree_lock);

	if (page) {
		ASSERT(page->mapping);
		ASSERT(page_offset(page) <= file_offset);
		ASSERT(file_offset + len <= page_offset(page) + PAGE_SIZE);

		/*
		 * Ordered (Private2) bit indicates whether we still have
		 * pending io unfinished for the ordered extent.
		 *
		 * If there's no such bit, we need to skip to next range.
		 */
		if (!btrfs_folio_test_ordered(fs_info, page_folio(page),
					      file_offset, len))
			return false;
		btrfs_folio_clear_ordered(fs_info, page_folio(page), file_offset, len);
	}

	/* Now we're fine to update the accounting. */
	if (WARN_ON_ONCE(len > ordered->bytes_left)) {
		btrfs_crit(fs_info,
"bad ordered extent accounting, root=%llu ino=%llu OE offset=%llu OE len=%llu to_dec=%llu left=%llu",
			   inode->root->root_key.objectid, btrfs_ino(inode),
			   ordered->file_offset, ordered->num_bytes,
			   len, ordered->bytes_left);
		ordered->bytes_left = 0;
	} else {
		ordered->bytes_left -= len;
	}

	if (!uptodate)
		set_bit(BTRFS_ORDERED_IOERR, &ordered->flags);

	if (ordered->bytes_left)
		return false;

	/*
	 * All the IO of the ordered extent is finished, we need to queue
	 * the finish_func to be executed.
	 */
	set_bit(BTRFS_ORDERED_IO_DONE, &ordered->flags);
	cond_wake_up(&ordered->wait);
	refcount_inc(&ordered->refs);
	trace_btrfs_ordered_extent_mark_finished(inode, ordered);
	return true;
}

static void btrfs_queue_ordered_fn(struct btrfs_ordered_extent *ordered)
{
	struct btrfs_inode *inode = BTRFS_I(ordered->inode);
	struct btrfs_fs_info *fs_info = inode->root->fs_info;
	struct btrfs_workqueue *wq = btrfs_is_free_space_inode(inode) ?
		fs_info->endio_freespace_worker : fs_info->endio_write_workers;

	btrfs_init_work(&ordered->work, finish_ordered_fn, NULL);
	btrfs_queue_work(wq, &ordered->work);
}

bool btrfs_finish_ordered_extent(struct btrfs_ordered_extent *ordered,
				 struct page *page, u64 file_offset, u64 len,
				 bool uptodate)
{
	struct btrfs_inode *inode = BTRFS_I(ordered->inode);
	unsigned long flags;
	bool ret;

	trace_btrfs_finish_ordered_extent(inode, file_offset, len, uptodate);

	spin_lock_irqsave(&inode->ordered_tree_lock, flags);
	ret = can_finish_ordered_extent(ordered, page, file_offset, len, uptodate);
	spin_unlock_irqrestore(&inode->ordered_tree_lock, flags);

	if (ret)
		btrfs_queue_ordered_fn(ordered);
	return ret;
}

/*
 * Mark all ordered extents io inside the specified range finished.
 *
 * @page:	 The involved page for the operation.
 *		 For uncompressed buffered IO, the page status also needs to be
 *		 updated to indicate whether the pending ordered io is finished.
 *		 Can be NULL for direct IO and compressed write.
 *		 For these cases, callers are ensured they won't execute the
 *		 endio function twice.
 *
 * This function is called for endio, thus the range must have ordered
 * extent(s) covering it.
 */
void btrfs_mark_ordered_io_finished(struct btrfs_inode *inode,
				    struct page *page, u64 file_offset,
				    u64 num_bytes, bool uptodate)
{
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	unsigned long flags;
	u64 cur = file_offset;

	trace_btrfs_writepage_end_io_hook(inode, file_offset,
					  file_offset + num_bytes - 1,
					  uptodate);

	spin_lock_irqsave(&inode->ordered_tree_lock, flags);
	while (cur < file_offset + num_bytes) {
		u64 entry_end;
		u64 end;
		u32 len;

		node = ordered_tree_search(inode, cur);
		/* No ordered extents at all */
		if (!node)
			break;

		entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
		entry_end = entry->file_offset + entry->num_bytes;
		/*
		 * |<-- OE --->|  |
		 *		  cur
		 * Go to next OE.
		 */
		if (cur >= entry_end) {
			node = rb_next(node);
			/* No more ordered extents, exit */
			if (!node)
				break;
			entry = rb_entry(node, struct btrfs_ordered_extent,
					 rb_node);

			/* Go to next ordered extent and continue */
			cur = entry->file_offset;
			continue;
		}
		/*
		 * |	|<--- OE --->|
		 * cur
		 * Go to the start of OE.
		 */
		if (cur < entry->file_offset) {
			cur = entry->file_offset;
			continue;
		}

		/*
		 * Now we are definitely inside one ordered extent.
		 *
		 * |<--- OE --->|
		 *	|
		 *	cur
		 */
		end = min(entry->file_offset + entry->num_bytes,
			  file_offset + num_bytes) - 1;
		ASSERT(end + 1 - cur < U32_MAX);
		len = end + 1 - cur;

		if (can_finish_ordered_extent(entry, page, cur, len, uptodate)) {
			spin_unlock_irqrestore(&inode->ordered_tree_lock, flags);
			btrfs_queue_ordered_fn(entry);
			spin_lock_irqsave(&inode->ordered_tree_lock, flags);
		}
		cur += len;
	}
	spin_unlock_irqrestore(&inode->ordered_tree_lock, flags);
}

/*
 * Finish IO for one ordered extent across a given range.  The range can only
 * contain one ordered extent.
 *
 * @cached:	 The cached ordered extent. If not NULL, we can skip the tree
 *               search and use the ordered extent directly.
 * 		 Will be also used to store the finished ordered extent.
 * @file_offset: File offset for the finished IO
 * @io_size:	 Length of the finish IO range
 *
 * Return true if the ordered extent is finished in the range, and update
 * @cached.
 * Return false otherwise.
 *
 * NOTE: The range can NOT cross multiple ordered extents.
 * Thus caller should ensure the range doesn't cross ordered extents.
 */
bool btrfs_dec_test_ordered_pending(struct btrfs_inode *inode,
				    struct btrfs_ordered_extent **cached,
				    u64 file_offset, u64 io_size)
{
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	unsigned long flags;
	bool finished = false;

	spin_lock_irqsave(&inode->ordered_tree_lock, flags);
	if (cached && *cached) {
		entry = *cached;
		goto have_entry;
	}

	node = ordered_tree_search(inode, file_offset);
	if (!node)
		goto out;

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
have_entry:
	if (!in_range(file_offset, entry->file_offset, entry->num_bytes))
		goto out;

	if (io_size > entry->bytes_left)
		btrfs_crit(inode->root->fs_info,
			   "bad ordered accounting left %llu size %llu",
		       entry->bytes_left, io_size);

	entry->bytes_left -= io_size;

	if (entry->bytes_left == 0) {
		/*
		 * Ensure only one caller can set the flag and finished_ret
		 * accordingly
		 */
		finished = !test_and_set_bit(BTRFS_ORDERED_IO_DONE, &entry->flags);
		/* test_and_set_bit implies a barrier */
		cond_wake_up_nomb(&entry->wait);
	}
out:
	if (finished && cached && entry) {
		*cached = entry;
		refcount_inc(&entry->refs);
		trace_btrfs_ordered_extent_dec_test_pending(inode, entry);
	}
	spin_unlock_irqrestore(&inode->ordered_tree_lock, flags);
	return finished;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * used to drop a reference on an ordered extent.  This will free
 * the extent if the last reference is dropped
 */
void btrfs_put_ordered_extent(struct btrfs_ordered_extent *entry)
{
	struct list_head *cur;
	struct btrfs_ordered_sum *sum;

<<<<<<< HEAD
	trace_btrfs_ordered_extent_put(entry->inode, entry);

	if (atomic_dec_and_test(&entry->refs)) {
=======
	trace_btrfs_ordered_extent_put(BTRFS_I(entry->inode), entry);

	if (refcount_dec_and_test(&entry->refs)) {
		ASSERT(list_empty(&entry->root_extent_list));
		ASSERT(list_empty(&entry->log_list));
		ASSERT(RB_EMPTY_NODE(&entry->rb_node));
		if (entry->inode)
			btrfs_add_delayed_iput(BTRFS_I(entry->inode));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		while (!list_empty(&entry->list)) {
			cur = entry->list.next;
			sum = list_entry(cur, struct btrfs_ordered_sum, list);
			list_del(&sum->list);
<<<<<<< HEAD
			kfree(sum);
		}
		kfree(entry);
=======
			kvfree(sum);
		}
		kmem_cache_free(btrfs_ordered_extent_cache, entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * remove an ordered extent from the tree.  No references are dropped
<<<<<<< HEAD
 * and you must wake_up entry->wait.  You must hold the tree lock
 * while you call this function.
 */
static void __btrfs_remove_ordered_extent(struct inode *inode,
					  struct btrfs_ordered_extent *entry)
{
	struct btrfs_ordered_inode_tree *tree;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct rb_node *node;

	tree = &BTRFS_I(inode)->ordered_tree;
	node = &entry->rb_node;
	rb_erase(node, &tree->tree);
	tree->last = NULL;
	set_bit(BTRFS_ORDERED_COMPLETE, &entry->flags);

	spin_lock(&root->fs_info->ordered_extent_lock);
	list_del_init(&entry->root_extent_list);

	trace_btrfs_ordered_extent_remove(inode, entry);

	/*
	 * we have no more ordered extents for this inode and
	 * no dirty pages.  We can safely remove it from the
	 * list of ordered extents
	 */
	if (RB_EMPTY_ROOT(&tree->tree) &&
	    !mapping_tagged(inode->i_mapping, PAGECACHE_TAG_DIRTY)) {
		list_del_init(&BTRFS_I(inode)->ordered_operations);
	}
	spin_unlock(&root->fs_info->ordered_extent_lock);
}

/*
 * remove an ordered extent from the tree.  No references are dropped
 * but any waiters are woken.
 */
void btrfs_remove_ordered_extent(struct inode *inode,
				 struct btrfs_ordered_extent *entry)
{
	struct btrfs_ordered_inode_tree *tree;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
	__btrfs_remove_ordered_extent(inode, entry);
	spin_unlock(&tree->lock);
	wake_up(&entry->wait);
=======
 * and waiters are woken up.
 */
void btrfs_remove_ordered_extent(struct btrfs_inode *btrfs_inode,
				 struct btrfs_ordered_extent *entry)
{
	struct btrfs_root *root = btrfs_inode->root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct rb_node *node;
	bool pending;
	bool freespace_inode;

	/*
	 * If this is a free space inode the thread has not acquired the ordered
	 * extents lockdep map.
	 */
	freespace_inode = btrfs_is_free_space_inode(btrfs_inode);

	btrfs_lockdep_acquire(fs_info, btrfs_trans_pending_ordered);
	/* This is paired with btrfs_alloc_ordered_extent. */
	spin_lock(&btrfs_inode->lock);
	btrfs_mod_outstanding_extents(btrfs_inode, -1);
	spin_unlock(&btrfs_inode->lock);
	if (root != fs_info->tree_root) {
		u64 release;

		if (test_bit(BTRFS_ORDERED_ENCODED, &entry->flags))
			release = entry->disk_num_bytes;
		else
			release = entry->num_bytes;
		btrfs_delalloc_release_metadata(btrfs_inode, release,
						test_bit(BTRFS_ORDERED_IOERR,
							 &entry->flags));
	}

	percpu_counter_add_batch(&fs_info->ordered_bytes, -entry->num_bytes,
				 fs_info->delalloc_batch);

	spin_lock_irq(&btrfs_inode->ordered_tree_lock);
	node = &entry->rb_node;
	rb_erase(node, &btrfs_inode->ordered_tree);
	RB_CLEAR_NODE(node);
	if (btrfs_inode->ordered_tree_last == node)
		btrfs_inode->ordered_tree_last = NULL;
	set_bit(BTRFS_ORDERED_COMPLETE, &entry->flags);
	pending = test_and_clear_bit(BTRFS_ORDERED_PENDING, &entry->flags);
	spin_unlock_irq(&btrfs_inode->ordered_tree_lock);

	/*
	 * The current running transaction is waiting on us, we need to let it
	 * know that we're complete and wake it up.
	 */
	if (pending) {
		struct btrfs_transaction *trans;

		/*
		 * The checks for trans are just a formality, it should be set,
		 * but if it isn't we don't want to deref/assert under the spin
		 * lock, so be nice and check if trans is set, but ASSERT() so
		 * if it isn't set a developer will notice.
		 */
		spin_lock(&fs_info->trans_lock);
		trans = fs_info->running_transaction;
		if (trans)
			refcount_inc(&trans->use_count);
		spin_unlock(&fs_info->trans_lock);

		ASSERT(trans || BTRFS_FS_ERROR(fs_info));
		if (trans) {
			if (atomic_dec_and_test(&trans->pending_ordered))
				wake_up(&trans->pending_wait);
			btrfs_put_transaction(trans);
		}
	}

	btrfs_lockdep_release(fs_info, btrfs_trans_pending_ordered);

	spin_lock(&root->ordered_extent_lock);
	list_del_init(&entry->root_extent_list);
	root->nr_ordered_extents--;

	trace_btrfs_ordered_extent_remove(btrfs_inode, entry);

	if (!root->nr_ordered_extents) {
		spin_lock(&fs_info->ordered_root_lock);
		BUG_ON(list_empty(&root->ordered_root));
		list_del_init(&root->ordered_root);
		spin_unlock(&fs_info->ordered_root_lock);
	}
	spin_unlock(&root->ordered_extent_lock);
	wake_up(&entry->wait);
	if (!freespace_inode)
		btrfs_lockdep_release(fs_info, btrfs_ordered_extent);
}

static void btrfs_run_ordered_extent_work(struct btrfs_work *work)
{
	struct btrfs_ordered_extent *ordered;

	ordered = container_of(work, struct btrfs_ordered_extent, flush_work);
	btrfs_start_ordered_extent(ordered);
	complete(&ordered->completion);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * wait for all the ordered extents in a root.  This is done when balancing
 * space between drives.
 */
<<<<<<< HEAD
void btrfs_wait_ordered_extents(struct btrfs_root *root,
				int nocow_only, int delay_iput)
{
	struct list_head splice;
	struct list_head *cur;
	struct btrfs_ordered_extent *ordered;
	struct inode *inode;

	INIT_LIST_HEAD(&splice);

	spin_lock(&root->fs_info->ordered_extent_lock);
	list_splice_init(&root->fs_info->ordered_extents, &splice);
	while (!list_empty(&splice)) {
		cur = splice.next;
		ordered = list_entry(cur, struct btrfs_ordered_extent,
				     root_extent_list);
		if (nocow_only &&
		    !test_bit(BTRFS_ORDERED_NOCOW, &ordered->flags) &&
		    !test_bit(BTRFS_ORDERED_PREALLOC, &ordered->flags)) {
			list_move(&ordered->root_extent_list,
				  &root->fs_info->ordered_extents);
			cond_resched_lock(&root->fs_info->ordered_extent_lock);
			continue;
		}

		list_del_init(&ordered->root_extent_list);
		atomic_inc(&ordered->refs);

		/*
		 * the inode may be getting freed (in sys_unlink path).
		 */
		inode = igrab(ordered->inode);

		spin_unlock(&root->fs_info->ordered_extent_lock);

		if (inode) {
			btrfs_start_ordered_extent(inode, ordered, 1);
			btrfs_put_ordered_extent(ordered);
			if (delay_iput)
				btrfs_add_delayed_iput(inode);
			else
				iput(inode);
		} else {
			btrfs_put_ordered_extent(ordered);
		}

		spin_lock(&root->fs_info->ordered_extent_lock);
	}
	spin_unlock(&root->fs_info->ordered_extent_lock);
}

/*
 * this is used during transaction commit to write all the inodes
 * added to the ordered operation list.  These files must be fully on
 * disk before the transaction commits.
 *
 * we have two modes here, one is to just start the IO via filemap_flush
 * and the other is to wait for all the io.  When we wait, we have an
 * extra check to make sure the ordered operation list really is empty
 * before we return
 */
void btrfs_run_ordered_operations(struct btrfs_root *root, int wait)
{
	struct btrfs_inode *btrfs_inode;
	struct inode *inode;
	struct list_head splice;

	INIT_LIST_HEAD(&splice);

	mutex_lock(&root->fs_info->ordered_operations_mutex);
	spin_lock(&root->fs_info->ordered_extent_lock);
again:
	list_splice_init(&root->fs_info->ordered_operations, &splice);

	while (!list_empty(&splice)) {
		btrfs_inode = list_entry(splice.next, struct btrfs_inode,
				   ordered_operations);

		inode = &btrfs_inode->vfs_inode;

		list_del_init(&btrfs_inode->ordered_operations);

		/*
		 * the inode may be getting freed (in sys_unlink path).
		 */
		inode = igrab(inode);

		if (!wait && inode) {
			list_add_tail(&BTRFS_I(inode)->ordered_operations,
			      &root->fs_info->ordered_operations);
		}
		spin_unlock(&root->fs_info->ordered_extent_lock);

		if (inode) {
			if (wait)
				btrfs_wait_ordered_range(inode, 0, (u64)-1);
			else
				filemap_flush(inode->i_mapping);
			btrfs_add_delayed_iput(inode);
		}

		cond_resched();
		spin_lock(&root->fs_info->ordered_extent_lock);
	}
	if (wait && !list_empty(&root->fs_info->ordered_operations))
		goto again;

	spin_unlock(&root->fs_info->ordered_extent_lock);
	mutex_unlock(&root->fs_info->ordered_operations_mutex);
}

/*
 * Used to start IO or wait for a given ordered extent to finish.
 *
 * If wait is one, this effectively waits on page writeback for all the pages
 * in the extent, and it waits on the io completion code to insert
 * metadata into the btree corresponding to the extent
 */
void btrfs_start_ordered_extent(struct inode *inode,
				       struct btrfs_ordered_extent *entry,
				       int wait)
{
	u64 start = entry->file_offset;
	u64 end = start + entry->len - 1;
=======
u64 btrfs_wait_ordered_extents(struct btrfs_root *root, u64 nr,
			       const u64 range_start, const u64 range_len)
{
	struct btrfs_fs_info *fs_info = root->fs_info;
	LIST_HEAD(splice);
	LIST_HEAD(skipped);
	LIST_HEAD(works);
	struct btrfs_ordered_extent *ordered, *next;
	u64 count = 0;
	const u64 range_end = range_start + range_len;

	mutex_lock(&root->ordered_extent_mutex);
	spin_lock(&root->ordered_extent_lock);
	list_splice_init(&root->ordered_extents, &splice);
	while (!list_empty(&splice) && nr) {
		ordered = list_first_entry(&splice, struct btrfs_ordered_extent,
					   root_extent_list);

		if (range_end <= ordered->disk_bytenr ||
		    ordered->disk_bytenr + ordered->disk_num_bytes <= range_start) {
			list_move_tail(&ordered->root_extent_list, &skipped);
			cond_resched_lock(&root->ordered_extent_lock);
			continue;
		}

		list_move_tail(&ordered->root_extent_list,
			       &root->ordered_extents);
		refcount_inc(&ordered->refs);
		spin_unlock(&root->ordered_extent_lock);

		btrfs_init_work(&ordered->flush_work,
				btrfs_run_ordered_extent_work, NULL);
		list_add_tail(&ordered->work_list, &works);
		btrfs_queue_work(fs_info->flush_workers, &ordered->flush_work);

		cond_resched();
		spin_lock(&root->ordered_extent_lock);
		if (nr != U64_MAX)
			nr--;
		count++;
	}
	list_splice_tail(&skipped, &root->ordered_extents);
	list_splice_tail(&splice, &root->ordered_extents);
	spin_unlock(&root->ordered_extent_lock);

	list_for_each_entry_safe(ordered, next, &works, work_list) {
		list_del_init(&ordered->work_list);
		wait_for_completion(&ordered->completion);
		btrfs_put_ordered_extent(ordered);
		cond_resched();
	}
	mutex_unlock(&root->ordered_extent_mutex);

	return count;
}

void btrfs_wait_ordered_roots(struct btrfs_fs_info *fs_info, u64 nr,
			     const u64 range_start, const u64 range_len)
{
	struct btrfs_root *root;
	LIST_HEAD(splice);
	u64 done;

	mutex_lock(&fs_info->ordered_operations_mutex);
	spin_lock(&fs_info->ordered_root_lock);
	list_splice_init(&fs_info->ordered_roots, &splice);
	while (!list_empty(&splice) && nr) {
		root = list_first_entry(&splice, struct btrfs_root,
					ordered_root);
		root = btrfs_grab_root(root);
		BUG_ON(!root);
		list_move_tail(&root->ordered_root,
			       &fs_info->ordered_roots);
		spin_unlock(&fs_info->ordered_root_lock);

		done = btrfs_wait_ordered_extents(root, nr,
						  range_start, range_len);
		btrfs_put_root(root);

		spin_lock(&fs_info->ordered_root_lock);
		if (nr != U64_MAX) {
			nr -= done;
		}
	}
	list_splice_tail(&splice, &fs_info->ordered_roots);
	spin_unlock(&fs_info->ordered_root_lock);
	mutex_unlock(&fs_info->ordered_operations_mutex);
}

/*
 * Start IO and wait for a given ordered extent to finish.
 *
 * Wait on page writeback for all the pages in the extent and the IO completion
 * code to insert metadata into the btree corresponding to the extent.
 */
void btrfs_start_ordered_extent(struct btrfs_ordered_extent *entry)
{
	u64 start = entry->file_offset;
	u64 end = start + entry->num_bytes - 1;
	struct btrfs_inode *inode = BTRFS_I(entry->inode);
	bool freespace_inode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	trace_btrfs_ordered_extent_start(inode, entry);

	/*
<<<<<<< HEAD
	 * pages in the range can be dirty, clean or writeback.  We
	 * start IO on any dirty ones so the wait doesn't stall waiting
	 * for pdflush to find them
	 */
	if (!test_bit(BTRFS_ORDERED_DIRECT, &entry->flags))
		filemap_fdatawrite_range(inode->i_mapping, start, end);
	if (wait) {
		wait_event(entry->wait, test_bit(BTRFS_ORDERED_COMPLETE,
						 &entry->flags));
	}
=======
	 * If this is a free space inode do not take the ordered extents lockdep
	 * map.
	 */
	freespace_inode = btrfs_is_free_space_inode(inode);

	/*
	 * pages in the range can be dirty, clean or writeback.  We
	 * start IO on any dirty ones so the wait doesn't stall waiting
	 * for the flusher thread to find them
	 */
	if (!test_bit(BTRFS_ORDERED_DIRECT, &entry->flags))
		filemap_fdatawrite_range(inode->vfs_inode.i_mapping, start, end);

	if (!freespace_inode)
		btrfs_might_wait_for_event(inode->root->fs_info, btrfs_ordered_extent);
	wait_event(entry->wait, test_bit(BTRFS_ORDERED_COMPLETE, &entry->flags));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Used to wait on ordered extents across a large range of bytes.
 */
<<<<<<< HEAD
void btrfs_wait_ordered_range(struct inode *inode, u64 start, u64 len)
{
	u64 end;
	u64 orig_end;
	struct btrfs_ordered_extent *ordered;
	int found;

	if (start + len < start) {
		orig_end = INT_LIMIT(loff_t);
	} else {
		orig_end = start + len - 1;
		if (orig_end > INT_LIMIT(loff_t))
			orig_end = INT_LIMIT(loff_t);
	}
again:
	/* start IO across the range first to instantiate any delalloc
	 * extents
	 */
	filemap_fdatawrite_range(inode->i_mapping, start, orig_end);

	/* The compression code will leave pages locked but return from
	 * writepage without setting the page writeback.  Starting again
	 * with WB_SYNC_ALL will end up waiting for the IO to actually start.
	 */
	filemap_fdatawrite_range(inode->i_mapping, start, orig_end);

	filemap_fdatawait_range(inode->i_mapping, start, orig_end);

	end = orig_end;
	found = 0;
	while (1) {
		ordered = btrfs_lookup_first_ordered_extent(inode, end);
=======
int btrfs_wait_ordered_range(struct inode *inode, u64 start, u64 len)
{
	int ret = 0;
	int ret_wb = 0;
	u64 end;
	u64 orig_end;
	struct btrfs_ordered_extent *ordered;

	if (start + len < start) {
		orig_end = OFFSET_MAX;
	} else {
		orig_end = start + len - 1;
		if (orig_end > OFFSET_MAX)
			orig_end = OFFSET_MAX;
	}

	/* start IO across the range first to instantiate any delalloc
	 * extents
	 */
	ret = btrfs_fdatawrite_range(inode, start, orig_end);
	if (ret)
		return ret;

	/*
	 * If we have a writeback error don't return immediately. Wait first
	 * for any ordered extents that haven't completed yet. This is to make
	 * sure no one can dirty the same page ranges and call writepages()
	 * before the ordered extents complete - to avoid failures (-EEXIST)
	 * when adding the new ordered extents to the ordered tree.
	 */
	ret_wb = filemap_fdatawait_range(inode->i_mapping, start, orig_end);

	end = orig_end;
	while (1) {
		ordered = btrfs_lookup_first_ordered_extent(BTRFS_I(inode), end);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!ordered)
			break;
		if (ordered->file_offset > orig_end) {
			btrfs_put_ordered_extent(ordered);
			break;
		}
<<<<<<< HEAD
		if (ordered->file_offset + ordered->len < start) {
			btrfs_put_ordered_extent(ordered);
			break;
		}
		found++;
		btrfs_start_ordered_extent(inode, ordered, 1);
		end = ordered->file_offset;
=======
		if (ordered->file_offset + ordered->num_bytes <= start) {
			btrfs_put_ordered_extent(ordered);
			break;
		}
		btrfs_start_ordered_extent(ordered);
		end = ordered->file_offset;
		/*
		 * If the ordered extent had an error save the error but don't
		 * exit without waiting first for all other ordered extents in
		 * the range to complete.
		 */
		if (test_bit(BTRFS_ORDERED_IOERR, &ordered->flags))
			ret = -EIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		btrfs_put_ordered_extent(ordered);
		if (end == 0 || end == start)
			break;
		end--;
	}
<<<<<<< HEAD
	if (found || test_range_bit(&BTRFS_I(inode)->io_tree, start, orig_end,
			   EXTENT_DELALLOC, 0, NULL)) {
		schedule_timeout(1);
		goto again;
	}
=======
	return ret_wb ? ret_wb : ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * find an ordered extent corresponding to file_offset.  return NULL if
 * nothing is found, otherwise take a reference on the extent and return it
 */
<<<<<<< HEAD
struct btrfs_ordered_extent *btrfs_lookup_ordered_extent(struct inode *inode,
							 u64 file_offset)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
	node = tree_search(tree, file_offset);
=======
struct btrfs_ordered_extent *btrfs_lookup_ordered_extent(struct btrfs_inode *inode,
							 u64 file_offset)
{
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	unsigned long flags;

	spin_lock_irqsave(&inode->ordered_tree_lock, flags);
	node = ordered_tree_search(inode, file_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!node)
		goto out;

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
<<<<<<< HEAD
	if (!offset_in_entry(entry, file_offset))
		entry = NULL;
	if (entry)
		atomic_inc(&entry->refs);
out:
	spin_unlock(&tree->lock);
=======
	if (!in_range(file_offset, entry->file_offset, entry->num_bytes))
		entry = NULL;
	if (entry) {
		refcount_inc(&entry->refs);
		trace_btrfs_ordered_extent_lookup(inode, entry);
	}
out:
	spin_unlock_irqrestore(&inode->ordered_tree_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return entry;
}

/* Since the DIO code tries to lock a wide area we need to look for any ordered
 * extents that exist in the range, rather than just the start of the range.
 */
<<<<<<< HEAD
struct btrfs_ordered_extent *btrfs_lookup_ordered_range(struct inode *inode,
							u64 file_offset,
							u64 len)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
	node = tree_search(tree, file_offset);
	if (!node) {
		node = tree_search(tree, file_offset + len);
=======
struct btrfs_ordered_extent *btrfs_lookup_ordered_range(
		struct btrfs_inode *inode, u64 file_offset, u64 len)
{
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

	spin_lock_irq(&inode->ordered_tree_lock);
	node = ordered_tree_search(inode, file_offset);
	if (!node) {
		node = ordered_tree_search(inode, file_offset + len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!node)
			goto out;
	}

	while (1) {
		entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
		if (range_overlaps(entry, file_offset, len))
			break;

		if (entry->file_offset >= file_offset + len) {
			entry = NULL;
			break;
		}
		entry = NULL;
		node = rb_next(node);
		if (!node)
			break;
	}
out:
<<<<<<< HEAD
	if (entry)
		atomic_inc(&entry->refs);
	spin_unlock(&tree->lock);
=======
	if (entry) {
		refcount_inc(&entry->refs);
		trace_btrfs_ordered_extent_lookup_range(inode, entry);
	}
	spin_unlock_irq(&inode->ordered_tree_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return entry;
}

/*
<<<<<<< HEAD
=======
 * Adds all ordered extents to the given list. The list ends up sorted by the
 * file_offset of the ordered extents.
 */
void btrfs_get_ordered_extents_for_logging(struct btrfs_inode *inode,
					   struct list_head *list)
{
	struct rb_node *n;

	ASSERT(inode_is_locked(&inode->vfs_inode));

	spin_lock_irq(&inode->ordered_tree_lock);
	for (n = rb_first(&inode->ordered_tree); n; n = rb_next(n)) {
		struct btrfs_ordered_extent *ordered;

		ordered = rb_entry(n, struct btrfs_ordered_extent, rb_node);

		if (test_bit(BTRFS_ORDERED_LOGGED, &ordered->flags))
			continue;

		ASSERT(list_empty(&ordered->log_list));
		list_add_tail(&ordered->log_list, list);
		refcount_inc(&ordered->refs);
		trace_btrfs_ordered_extent_lookup_for_logging(inode, ordered);
	}
	spin_unlock_irq(&inode->ordered_tree_lock);
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * lookup and return any extent before 'file_offset'.  NULL is returned
 * if none is found
 */
struct btrfs_ordered_extent *
<<<<<<< HEAD
btrfs_lookup_first_ordered_extent(struct inode *inode, u64 file_offset)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
	node = tree_search(tree, file_offset);
=======
btrfs_lookup_first_ordered_extent(struct btrfs_inode *inode, u64 file_offset)
{
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

	spin_lock_irq(&inode->ordered_tree_lock);
	node = ordered_tree_search(inode, file_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!node)
		goto out;

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
<<<<<<< HEAD
	atomic_inc(&entry->refs);
out:
	spin_unlock(&tree->lock);
=======
	refcount_inc(&entry->refs);
	trace_btrfs_ordered_extent_lookup_first(inode, entry);
out:
	spin_unlock_irq(&inode->ordered_tree_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return entry;
}

/*
<<<<<<< HEAD
 * After an extent is done, call this to conditionally update the on disk
 * i_size.  i_size is updated to cover any fully written part of the file.
 */
int btrfs_ordered_update_i_size(struct inode *inode, u64 offset,
				struct btrfs_ordered_extent *ordered)
{
	struct btrfs_ordered_inode_tree *tree = &BTRFS_I(inode)->ordered_tree;
	struct extent_io_tree *io_tree = &BTRFS_I(inode)->io_tree;
	u64 disk_i_size;
	u64 new_i_size;
	u64 i_size_test;
	u64 i_size = i_size_read(inode);
	struct rb_node *node;
	struct rb_node *prev = NULL;
	struct btrfs_ordered_extent *test;
	int ret = 1;

	if (ordered)
		offset = entry_end(ordered);
	else
		offset = ALIGN(offset, BTRFS_I(inode)->root->sectorsize);

	spin_lock(&tree->lock);
	disk_i_size = BTRFS_I(inode)->disk_i_size;

	/* truncate file */
	if (disk_i_size > i_size) {
		BTRFS_I(inode)->disk_i_size = i_size;
		ret = 0;
		goto out;
	}

	/*
	 * if the disk i_size is already at the inode->i_size, or
	 * this ordered extent is inside the disk i_size, we're done
	 */
	if (disk_i_size == i_size || offset <= disk_i_size) {
		goto out;
	}

	/*
	 * we can't update the disk_isize if there are delalloc bytes
	 * between disk_i_size and  this ordered extent
	 */
	if (test_range_bit(io_tree, disk_i_size, offset - 1,
			   EXTENT_DELALLOC, 0, NULL)) {
		goto out;
	}
	/*
	 * walk backward from this ordered extent to disk_i_size.
	 * if we find an ordered extent then we can't update disk i_size
	 * yet
	 */
	if (ordered) {
		node = rb_prev(&ordered->rb_node);
	} else {
		prev = tree_search(tree, offset);
		/*
		 * we insert file extents without involving ordered struct,
		 * so there should be no ordered struct cover this offset
		 */
		if (prev) {
			test = rb_entry(prev, struct btrfs_ordered_extent,
					rb_node);
			BUG_ON(offset_in_entry(test, offset));
		}
		node = prev;
	}
	while (node) {
		test = rb_entry(node, struct btrfs_ordered_extent, rb_node);
		if (test->file_offset + test->len <= disk_i_size)
			break;
		if (test->file_offset >= i_size)
			break;
		if (test->file_offset >= disk_i_size)
			goto out;
		node = rb_prev(node);
	}
	new_i_size = min_t(u64, offset, i_size);

	/*
	 * at this point, we know we can safely update i_size to at least
	 * the offset from this ordered extent.  But, we need to
	 * walk forward and see if ios from higher up in the file have
	 * finished.
	 */
	if (ordered) {
		node = rb_next(&ordered->rb_node);
	} else {
		if (prev)
			node = rb_next(prev);
		else
			node = rb_first(&tree->tree);
	}
	i_size_test = 0;
	if (node) {
		/*
		 * do we have an area where IO might have finished
		 * between our ordered extent and the next one.
		 */
		test = rb_entry(node, struct btrfs_ordered_extent, rb_node);
		if (test->file_offset > offset)
			i_size_test = test->file_offset;
	} else {
		i_size_test = i_size;
	}

	/*
	 * i_size_test is the end of a region after this ordered
	 * extent where there are no ordered extents.  As long as there
	 * are no delalloc bytes in this area, it is safe to update
	 * disk_i_size to the end of the region.
	 */
	if (i_size_test > offset &&
	    !test_range_bit(io_tree, offset, i_size_test - 1,
			    EXTENT_DELALLOC, 0, NULL)) {
		new_i_size = min_t(u64, i_size_test, i_size);
	}
	BTRFS_I(inode)->disk_i_size = new_i_size;
	ret = 0;
out:
	/*
	 * we need to remove the ordered extent with the tree lock held
	 * so that other people calling this function don't find our fully
	 * processed ordered entry and skip updating the i_size
	 */
	if (ordered)
		__btrfs_remove_ordered_extent(inode, ordered);
	spin_unlock(&tree->lock);
	if (ordered)
		wake_up(&ordered->wait);
	return ret;
}

/*
 * search the ordered extents for one corresponding to 'offset' and
 * try to find a checksum.  This is used because we allow pages to
 * be reclaimed before their checksum is actually put into the btree
 */
int btrfs_find_ordered_sum(struct inode *inode, u64 offset, u64 disk_bytenr,
			   u32 *sum)
{
	struct btrfs_ordered_sum *ordered_sum;
	struct btrfs_sector_sum *sector_sums;
	struct btrfs_ordered_extent *ordered;
	struct btrfs_ordered_inode_tree *tree = &BTRFS_I(inode)->ordered_tree;
	unsigned long num_sectors;
	unsigned long i;
	u32 sectorsize = BTRFS_I(inode)->root->sectorsize;
	int ret = 1;

	ordered = btrfs_lookup_ordered_extent(inode, offset);
	if (!ordered)
		return 1;

	spin_lock(&tree->lock);
	list_for_each_entry_reverse(ordered_sum, &ordered->list, list) {
		if (disk_bytenr >= ordered_sum->bytenr) {
			num_sectors = ordered_sum->len / sectorsize;
			sector_sums = ordered_sum->sums;
			for (i = 0; i < num_sectors; i++) {
				if (sector_sums[i].bytenr == disk_bytenr) {
					*sum = sector_sums[i].sum;
					ret = 0;
					goto out;
				}
			}
		}
	}
out:
	spin_unlock(&tree->lock);
	btrfs_put_ordered_extent(ordered);
	return ret;
}


/*
 * add a given inode to the list of inodes that must be fully on
 * disk before a transaction commit finishes.
 *
 * This basically gives us the ext3 style data=ordered mode, and it is mostly
 * used to make sure renamed files are fully on disk.
 *
 * It is a noop if the inode is already fully on disk.
 *
 * If trans is not null, we'll do a friendly check for a transaction that
 * is already flushing things and force the IO down ourselves.
 */
void btrfs_add_ordered_operation(struct btrfs_trans_handle *trans,
				 struct btrfs_root *root, struct inode *inode)
{
	u64 last_mod;

	last_mod = max(BTRFS_I(inode)->generation, BTRFS_I(inode)->last_trans);

	/*
	 * if this file hasn't been changed since the last transaction
	 * commit, we can safely return without doing anything
	 */
	if (last_mod < root->fs_info->last_trans_committed)
		return;

	/*
	 * the transaction is already committing.  Just start the IO and
	 * don't bother with all of this list nonsense
	 */
	if (trans && root->fs_info->running_transaction->blocked) {
		btrfs_wait_ordered_range(inode, 0, (u64)-1);
		return;
	}

	spin_lock(&root->fs_info->ordered_extent_lock);
	if (list_empty(&BTRFS_I(inode)->ordered_operations)) {
		list_add_tail(&BTRFS_I(inode)->ordered_operations,
			      &root->fs_info->ordered_operations);
	}
	spin_unlock(&root->fs_info->ordered_extent_lock);
=======
 * Lookup the first ordered extent that overlaps the range
 * [@file_offset, @file_offset + @len).
 *
 * The difference between this and btrfs_lookup_first_ordered_extent() is
 * that this one won't return any ordered extent that does not overlap the range.
 * And the difference against btrfs_lookup_ordered_extent() is, this function
 * ensures the first ordered extent gets returned.
 */
struct btrfs_ordered_extent *btrfs_lookup_first_ordered_range(
			struct btrfs_inode *inode, u64 file_offset, u64 len)
{
	struct rb_node *node;
	struct rb_node *cur;
	struct rb_node *prev;
	struct rb_node *next;
	struct btrfs_ordered_extent *entry = NULL;

	spin_lock_irq(&inode->ordered_tree_lock);
	node = inode->ordered_tree.rb_node;
	/*
	 * Here we don't want to use tree_search() which will use tree->last
	 * and screw up the search order.
	 * And __tree_search() can't return the adjacent ordered extents
	 * either, thus here we do our own search.
	 */
	while (node) {
		entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);

		if (file_offset < entry->file_offset) {
			node = node->rb_left;
		} else if (file_offset >= entry_end(entry)) {
			node = node->rb_right;
		} else {
			/*
			 * Direct hit, got an ordered extent that starts at
			 * @file_offset
			 */
			goto out;
		}
	}
	if (!entry) {
		/* Empty tree */
		goto out;
	}

	cur = &entry->rb_node;
	/* We got an entry around @file_offset, check adjacent entries */
	if (entry->file_offset < file_offset) {
		prev = cur;
		next = rb_next(cur);
	} else {
		prev = rb_prev(cur);
		next = cur;
	}
	if (prev) {
		entry = rb_entry(prev, struct btrfs_ordered_extent, rb_node);
		if (range_overlaps(entry, file_offset, len))
			goto out;
	}
	if (next) {
		entry = rb_entry(next, struct btrfs_ordered_extent, rb_node);
		if (range_overlaps(entry, file_offset, len))
			goto out;
	}
	/* No ordered extent in the range */
	entry = NULL;
out:
	if (entry) {
		refcount_inc(&entry->refs);
		trace_btrfs_ordered_extent_lookup_first_range(inode, entry);
	}

	spin_unlock_irq(&inode->ordered_tree_lock);
	return entry;
}

/*
 * Lock the passed range and ensures all pending ordered extents in it are run
 * to completion.
 *
 * @inode:        Inode whose ordered tree is to be searched
 * @start:        Beginning of range to flush
 * @end:          Last byte of range to lock
 * @cached_state: If passed, will return the extent state responsible for the
 *                locked range. It's the caller's responsibility to free the
 *                cached state.
 *
 * Always return with the given range locked, ensuring after it's called no
 * order extent can be pending.
 */
void btrfs_lock_and_flush_ordered_range(struct btrfs_inode *inode, u64 start,
					u64 end,
					struct extent_state **cached_state)
{
	struct btrfs_ordered_extent *ordered;
	struct extent_state *cache = NULL;
	struct extent_state **cachedp = &cache;

	if (cached_state)
		cachedp = cached_state;

	while (1) {
		lock_extent(&inode->io_tree, start, end, cachedp);
		ordered = btrfs_lookup_ordered_range(inode, start,
						     end - start + 1);
		if (!ordered) {
			/*
			 * If no external cached_state has been passed then
			 * decrement the extra ref taken for cachedp since we
			 * aren't exposing it outside of this function
			 */
			if (!cached_state)
				refcount_dec(&cache->refs);
			break;
		}
		unlock_extent(&inode->io_tree, start, end, cachedp);
		btrfs_start_ordered_extent(ordered);
		btrfs_put_ordered_extent(ordered);
	}
}

/*
 * Lock the passed range and ensure all pending ordered extents in it are run
 * to completion in nowait mode.
 *
 * Return true if btrfs_lock_ordered_range does not return any extents,
 * otherwise false.
 */
bool btrfs_try_lock_ordered_range(struct btrfs_inode *inode, u64 start, u64 end,
				  struct extent_state **cached_state)
{
	struct btrfs_ordered_extent *ordered;

	if (!try_lock_extent(&inode->io_tree, start, end, cached_state))
		return false;

	ordered = btrfs_lookup_ordered_range(inode, start, end - start + 1);
	if (!ordered)
		return true;

	btrfs_put_ordered_extent(ordered);
	unlock_extent(&inode->io_tree, start, end, cached_state);

	return false;
}

/* Split out a new ordered extent for this first @len bytes of @ordered. */
struct btrfs_ordered_extent *btrfs_split_ordered_extent(
			struct btrfs_ordered_extent *ordered, u64 len)
{
	struct btrfs_inode *inode = BTRFS_I(ordered->inode);
	struct btrfs_root *root = inode->root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	u64 file_offset = ordered->file_offset;
	u64 disk_bytenr = ordered->disk_bytenr;
	unsigned long flags = ordered->flags;
	struct btrfs_ordered_sum *sum, *tmpsum;
	struct btrfs_ordered_extent *new;
	struct rb_node *node;
	u64 offset = 0;

	trace_btrfs_ordered_extent_split(inode, ordered);

	ASSERT(!(flags & (1U << BTRFS_ORDERED_COMPRESSED)));

	/*
	 * The entire bio must be covered by the ordered extent, but we can't
	 * reduce the original extent to a zero length either.
	 */
	if (WARN_ON_ONCE(len >= ordered->num_bytes))
		return ERR_PTR(-EINVAL);
	/* We cannot split partially completed ordered extents. */
	if (ordered->bytes_left) {
		ASSERT(!(flags & ~BTRFS_ORDERED_TYPE_FLAGS));
		if (WARN_ON_ONCE(ordered->bytes_left != ordered->disk_num_bytes))
			return ERR_PTR(-EINVAL);
	}
	/* We cannot split a compressed ordered extent. */
	if (WARN_ON_ONCE(ordered->disk_num_bytes != ordered->num_bytes))
		return ERR_PTR(-EINVAL);

	new = alloc_ordered_extent(inode, file_offset, len, len, disk_bytenr,
				   len, 0, flags, ordered->compress_type);
	if (IS_ERR(new))
		return new;

	/* One ref for the tree. */
	refcount_inc(&new->refs);

	spin_lock_irq(&root->ordered_extent_lock);
	spin_lock(&inode->ordered_tree_lock);
	/* Remove from tree once */
	node = &ordered->rb_node;
	rb_erase(node, &inode->ordered_tree);
	RB_CLEAR_NODE(node);
	if (inode->ordered_tree_last == node)
		inode->ordered_tree_last = NULL;

	ordered->file_offset += len;
	ordered->disk_bytenr += len;
	ordered->num_bytes -= len;
	ordered->disk_num_bytes -= len;
	ordered->ram_bytes -= len;

	if (test_bit(BTRFS_ORDERED_IO_DONE, &ordered->flags)) {
		ASSERT(ordered->bytes_left == 0);
		new->bytes_left = 0;
	} else {
		ordered->bytes_left -= len;
	}

	if (test_bit(BTRFS_ORDERED_TRUNCATED, &ordered->flags)) {
		if (ordered->truncated_len > len) {
			ordered->truncated_len -= len;
		} else {
			new->truncated_len = ordered->truncated_len;
			ordered->truncated_len = 0;
		}
	}

	list_for_each_entry_safe(sum, tmpsum, &ordered->list, list) {
		if (offset == len)
			break;
		list_move_tail(&sum->list, &new->list);
		offset += sum->len;
	}

	/* Re-insert the node */
	node = tree_insert(&inode->ordered_tree, ordered->file_offset,
			   &ordered->rb_node);
	if (node)
		btrfs_panic(fs_info, -EEXIST,
			"zoned: inconsistency in ordered tree at offset %llu",
			ordered->file_offset);

	node = tree_insert(&inode->ordered_tree, new->file_offset, &new->rb_node);
	if (node)
		btrfs_panic(fs_info, -EEXIST,
			"zoned: inconsistency in ordered tree at offset %llu",
			new->file_offset);
	spin_unlock(&inode->ordered_tree_lock);

	list_add_tail(&new->root_extent_list, &root->ordered_extents);
	root->nr_ordered_extents++;
	spin_unlock_irq(&root->ordered_extent_lock);
	return new;
}

int __init ordered_data_init(void)
{
	btrfs_ordered_extent_cache = KMEM_CACHE(btrfs_ordered_extent, 0);
	if (!btrfs_ordered_extent_cache)
		return -ENOMEM;

	return 0;
}

void __cold ordered_data_exit(void)
{
	kmem_cache_destroy(btrfs_ordered_extent_cache);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
