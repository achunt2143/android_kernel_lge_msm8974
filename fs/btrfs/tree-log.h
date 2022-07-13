<<<<<<< HEAD
/*
 * Copyright (C) 2008 Oracle.  All rights reserved.
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

#ifndef __TREE_LOG_
#define __TREE_LOG_
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2008 Oracle.  All rights reserved.
 */

#ifndef BTRFS_TREE_LOG_H
#define BTRFS_TREE_LOG_H

#include <linux/list.h>
#include <linux/fs.h>
#include "messages.h"
#include "ctree.h"
#include "transaction.h"

struct inode;
struct dentry;
struct btrfs_ordered_extent;
struct btrfs_root;
struct btrfs_trans_handle;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* return value for btrfs_log_dentry_safe that means we don't need to log it at all */
#define BTRFS_NO_LOG_SYNC 256

<<<<<<< HEAD
int btrfs_sync_log(struct btrfs_trans_handle *trans,
		   struct btrfs_root *root);
=======
/*
 * We can't use the tree log for whatever reason, force a transaction commit.
 * We use a negative value because there are functions through the logging code
 * that need to return an error (< 0 value), false (0) or true (1). Any negative
 * value will do, as it will cause the log to be marked for a full sync.
 */
#define BTRFS_LOG_FORCE_COMMIT				(-(MAX_ERRNO + 1))

struct btrfs_log_ctx {
	int log_ret;
	int log_transid;
	bool log_new_dentries;
	bool logging_new_name;
	bool logging_new_delayed_dentries;
	/* Indicate if the inode being logged was logged before. */
	bool logged_before;
	struct inode *inode;
	struct list_head list;
	/* Only used for fast fsyncs. */
	struct list_head ordered_extents;
	struct list_head conflict_inodes;
	int num_conflict_inodes;
	bool logging_conflict_inodes;
	/*
	 * Used for fsyncs that need to copy items from the subvolume tree to
	 * the log tree (full sync flag set or copy everything flag set) to
	 * avoid allocating a temporary extent buffer while holding a lock on
	 * an extent buffer of the subvolume tree and under the log transaction.
	 * Also helps to avoid allocating and freeing a temporary extent buffer
	 * in case we need to process multiple leaves from the subvolume tree.
	 */
	struct extent_buffer *scratch_eb;
};

void btrfs_init_log_ctx(struct btrfs_log_ctx *ctx, struct inode *inode);
void btrfs_init_log_ctx_scratch_eb(struct btrfs_log_ctx *ctx);
void btrfs_release_log_ctx_extents(struct btrfs_log_ctx *ctx);

static inline void btrfs_set_log_full_commit(struct btrfs_trans_handle *trans)
{
	WRITE_ONCE(trans->fs_info->last_trans_log_full_commit, trans->transid);
}

static inline int btrfs_need_log_full_commit(struct btrfs_trans_handle *trans)
{
	return READ_ONCE(trans->fs_info->last_trans_log_full_commit) ==
		trans->transid;
}

int btrfs_sync_log(struct btrfs_trans_handle *trans,
		   struct btrfs_root *root, struct btrfs_log_ctx *ctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int btrfs_free_log(struct btrfs_trans_handle *trans, struct btrfs_root *root);
int btrfs_free_log_root_tree(struct btrfs_trans_handle *trans,
			     struct btrfs_fs_info *fs_info);
int btrfs_recover_log_trees(struct btrfs_root *tree_root);
int btrfs_log_dentry_safe(struct btrfs_trans_handle *trans,
<<<<<<< HEAD
			  struct btrfs_root *root, struct dentry *dentry);
int btrfs_del_dir_entries_in_log(struct btrfs_trans_handle *trans,
				 struct btrfs_root *root,
				 const char *name, int name_len,
				 struct inode *dir, u64 index);
int btrfs_del_inode_ref_in_log(struct btrfs_trans_handle *trans,
			       struct btrfs_root *root,
			       const char *name, int name_len,
			       struct inode *inode, u64 dirid);
void btrfs_end_log_trans(struct btrfs_root *root);
int btrfs_pin_log_trans(struct btrfs_root *root);
int btrfs_log_inode_parent(struct btrfs_trans_handle *trans,
		    struct btrfs_root *root, struct inode *inode,
		    struct dentry *parent, int exists_only);
void btrfs_record_unlink_dir(struct btrfs_trans_handle *trans,
			     struct inode *dir, struct inode *inode,
			     int for_rename);
int btrfs_log_new_name(struct btrfs_trans_handle *trans,
			struct inode *inode, struct inode *old_dir,
			struct dentry *parent);
=======
			  struct dentry *dentry,
			  struct btrfs_log_ctx *ctx);
void btrfs_del_dir_entries_in_log(struct btrfs_trans_handle *trans,
				  struct btrfs_root *root,
				  const struct fscrypt_str *name,
				  struct btrfs_inode *dir, u64 index);
void btrfs_del_inode_ref_in_log(struct btrfs_trans_handle *trans,
				struct btrfs_root *root,
				const struct fscrypt_str *name,
				struct btrfs_inode *inode, u64 dirid);
void btrfs_end_log_trans(struct btrfs_root *root);
void btrfs_pin_log_trans(struct btrfs_root *root);
void btrfs_record_unlink_dir(struct btrfs_trans_handle *trans,
			     struct btrfs_inode *dir, struct btrfs_inode *inode,
			     bool for_rename);
void btrfs_record_snapshot_destroy(struct btrfs_trans_handle *trans,
				   struct btrfs_inode *dir);
void btrfs_log_new_name(struct btrfs_trans_handle *trans,
			struct dentry *old_dentry, struct btrfs_inode *old_dir,
			u64 old_dir_index, struct dentry *parent);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
