<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/fs/ext4/super.c
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/fs/minix/inode.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  Big-endian to little-endian byte-swapping/bitmaps by
 *        David S. Miller (davem@caip.rutgers.edu), 1995
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD
#include <linux/jbd2.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/blkdev.h>
=======
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/backing-dev.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/parser.h>
#include <linux/buffer_head.h>
#include <linux/exportfs.h>
#include <linux/vfs.h>
#include <linux/random.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/quotaops.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
#include <linux/proc_fs.h>
#include <linux/ctype.h>
#include <linux/log2.h>
#include <linux/crc16.h>
#include <linux/cleancache.h>
#include <asm/uaccess.h>

#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/ratelimit.h>

#include "ext4.h"
#include "ext4_extents.h"
=======
#include <linux/ctype.h>
#include <linux/log2.h>
#include <linux/crc16.h>
#include <linux/dax.h>
#include <linux/uaccess.h>
#include <linux/iversion.h>
#include <linux/unicode.h>
#include <linux/part_stat.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/fsnotify.h>
#include <linux/fs_context.h>
#include <linux/fs_parser.h>

#include "ext4.h"
#include "ext4_extents.h"	/* Needed for trace points definition */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "ext4_jbd2.h"
#include "xattr.h"
#include "acl.h"
#include "mballoc.h"
<<<<<<< HEAD
=======
#include "fsmap.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define CREATE_TRACE_POINTS
#include <trace/events/ext4.h>

<<<<<<< HEAD
static struct proc_dir_entry *ext4_proc_root;
static struct kset *ext4_kset;
static struct ext4_lazy_init *ext4_li_info;
static struct mutex ext4_li_mtx;
static struct ext4_features *ext4_feat;
=======
static struct ext4_lazy_init *ext4_li_info;
static DEFINE_MUTEX(ext4_li_mtx);
static struct ratelimit_state ext4_mount_msg_ratelimit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int ext4_load_journal(struct super_block *, struct ext4_super_block *,
			     unsigned long journal_devnum);
static int ext4_show_options(struct seq_file *seq, struct dentry *root);
<<<<<<< HEAD
static int ext4_commit_super(struct super_block *sb, int sync);
static void ext4_mark_recovery_complete(struct super_block *sb,
					struct ext4_super_block *es);
static void ext4_clear_journal_err(struct super_block *sb,
				   struct ext4_super_block *es);
static int ext4_sync_fs(struct super_block *sb, int wait);
static const char *ext4_decode_error(struct super_block *sb, int errno,
				     char nbuf[16]);
static int ext4_remount(struct super_block *sb, int *flags, char *data);
static int ext4_statfs(struct dentry *dentry, struct kstatfs *buf);
static int ext4_unfreeze(struct super_block *sb);
static void ext4_write_super(struct super_block *sb);
static int ext4_freeze(struct super_block *sb);
static struct dentry *ext4_mount(struct file_system_type *fs_type, int flags,
		       const char *dev_name, void *data);
static inline int ext2_feature_set_ok(struct super_block *sb);
static inline int ext3_feature_set_ok(struct super_block *sb);
static int ext4_feature_set_ok(struct super_block *sb, int readonly);
static void ext4_destroy_lazyinit_thread(void);
static void ext4_unregister_li_request(struct super_block *sb);
static void ext4_clear_request_list(void);

#if !defined(CONFIG_EXT2_FS) && !defined(CONFIG_EXT2_FS_MODULE) && defined(CONFIG_EXT4_USE_FOR_EXT23)
static struct file_system_type ext2_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "ext2",
	.mount		= ext4_mount,
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
};
MODULE_ALIAS_FS("ext2");
#define IS_EXT2_SB(sb) ((sb)->s_bdev->bd_holder == &ext2_fs_type)
=======
static void ext4_update_super(struct super_block *sb);
static int ext4_commit_super(struct super_block *sb);
static int ext4_mark_recovery_complete(struct super_block *sb,
					struct ext4_super_block *es);
static int ext4_clear_journal_err(struct super_block *sb,
				  struct ext4_super_block *es);
static int ext4_sync_fs(struct super_block *sb, int wait);
static int ext4_statfs(struct dentry *dentry, struct kstatfs *buf);
static int ext4_unfreeze(struct super_block *sb);
static int ext4_freeze(struct super_block *sb);
static inline int ext2_feature_set_ok(struct super_block *sb);
static inline int ext3_feature_set_ok(struct super_block *sb);
static void ext4_destroy_lazyinit_thread(void);
static void ext4_unregister_li_request(struct super_block *sb);
static void ext4_clear_request_list(void);
static struct inode *ext4_get_journal_inode(struct super_block *sb,
					    unsigned int journal_inum);
static int ext4_validate_options(struct fs_context *fc);
static int ext4_check_opt_consistency(struct fs_context *fc,
				      struct super_block *sb);
static void ext4_apply_options(struct fs_context *fc, struct super_block *sb);
static int ext4_parse_param(struct fs_context *fc, struct fs_parameter *param);
static int ext4_get_tree(struct fs_context *fc);
static int ext4_reconfigure(struct fs_context *fc);
static void ext4_fc_free(struct fs_context *fc);
static int ext4_init_fs_context(struct fs_context *fc);
static void ext4_kill_sb(struct super_block *sb);
static const struct fs_parameter_spec ext4_param_specs[];

/*
 * Lock ordering
 *
 * page fault path:
 * mmap_lock -> sb_start_pagefault -> invalidate_lock (r) -> transaction start
 *   -> page lock -> i_data_sem (rw)
 *
 * buffered write path:
 * sb_start_write -> i_mutex -> mmap_lock
 * sb_start_write -> i_mutex -> transaction start -> page lock ->
 *   i_data_sem (rw)
 *
 * truncate:
 * sb_start_write -> i_mutex -> invalidate_lock (w) -> i_mmap_rwsem (w) ->
 *   page lock
 * sb_start_write -> i_mutex -> invalidate_lock (w) -> transaction start ->
 *   i_data_sem (rw)
 *
 * direct IO:
 * sb_start_write -> i_mutex -> mmap_lock
 * sb_start_write -> i_mutex -> transaction start -> i_data_sem (rw)
 *
 * writepages:
 * transaction start -> page lock(s) -> i_data_sem (rw)
 */

static const struct fs_context_operations ext4_context_ops = {
	.parse_param	= ext4_parse_param,
	.get_tree	= ext4_get_tree,
	.reconfigure	= ext4_reconfigure,
	.free		= ext4_fc_free,
};


#if !defined(CONFIG_EXT2_FS) && !defined(CONFIG_EXT2_FS_MODULE) && defined(CONFIG_EXT4_USE_FOR_EXT2)
static struct file_system_type ext2_fs_type = {
	.owner			= THIS_MODULE,
	.name			= "ext2",
	.init_fs_context	= ext4_init_fs_context,
	.parameters		= ext4_param_specs,
	.kill_sb		= ext4_kill_sb,
	.fs_flags		= FS_REQUIRES_DEV,
};
MODULE_ALIAS_FS("ext2");
MODULE_ALIAS("ext2");
#define IS_EXT2_SB(sb) ((sb)->s_type == &ext2_fs_type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
#define IS_EXT2_SB(sb) (0)
#endif


<<<<<<< HEAD
#if !defined(CONFIG_EXT3_FS) && !defined(CONFIG_EXT3_FS_MODULE) && defined(CONFIG_EXT4_USE_FOR_EXT23)
static struct file_system_type ext3_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "ext3",
	.mount		= ext4_mount,
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
};
MODULE_ALIAS_FS("ext3");
#define IS_EXT3_SB(sb) ((sb)->s_bdev->bd_holder == &ext3_fs_type)
#else
#define IS_EXT3_SB(sb) (0)
#endif
=======
static struct file_system_type ext3_fs_type = {
	.owner			= THIS_MODULE,
	.name			= "ext3",
	.init_fs_context	= ext4_init_fs_context,
	.parameters		= ext4_param_specs,
	.kill_sb		= ext4_kill_sb,
	.fs_flags		= FS_REQUIRES_DEV,
};
MODULE_ALIAS_FS("ext3");
MODULE_ALIAS("ext3");
#define IS_EXT3_SB(sb) ((sb)->s_type == &ext3_fs_type)


static inline void __ext4_read_bh(struct buffer_head *bh, blk_opf_t op_flags,
				  bh_end_io_t *end_io)
{
	/*
	 * buffer's verified bit is no longer valid after reading from
	 * disk again due to write out error, clear it to make sure we
	 * recheck the buffer contents.
	 */
	clear_buffer_verified(bh);

	bh->b_end_io = end_io ? end_io : end_buffer_read_sync;
	get_bh(bh);
	submit_bh(REQ_OP_READ | op_flags, bh);
}

void ext4_read_bh_nowait(struct buffer_head *bh, blk_opf_t op_flags,
			 bh_end_io_t *end_io)
{
	BUG_ON(!buffer_locked(bh));

	if (ext4_buffer_uptodate(bh)) {
		unlock_buffer(bh);
		return;
	}
	__ext4_read_bh(bh, op_flags, end_io);
}

int ext4_read_bh(struct buffer_head *bh, blk_opf_t op_flags, bh_end_io_t *end_io)
{
	BUG_ON(!buffer_locked(bh));

	if (ext4_buffer_uptodate(bh)) {
		unlock_buffer(bh);
		return 0;
	}

	__ext4_read_bh(bh, op_flags, end_io);

	wait_on_buffer(bh);
	if (buffer_uptodate(bh))
		return 0;
	return -EIO;
}

int ext4_read_bh_lock(struct buffer_head *bh, blk_opf_t op_flags, bool wait)
{
	lock_buffer(bh);
	if (!wait) {
		ext4_read_bh_nowait(bh, op_flags, NULL);
		return 0;
	}
	return ext4_read_bh(bh, op_flags, NULL);
}

/*
 * This works like __bread_gfp() except it uses ERR_PTR for error
 * returns.  Currently with sb_bread it's impossible to distinguish
 * between ENOMEM and EIO situations (since both result in a NULL
 * return.
 */
static struct buffer_head *__ext4_sb_bread_gfp(struct super_block *sb,
					       sector_t block,
					       blk_opf_t op_flags, gfp_t gfp)
{
	struct buffer_head *bh;
	int ret;

	bh = sb_getblk_gfp(sb, block, gfp);
	if (bh == NULL)
		return ERR_PTR(-ENOMEM);
	if (ext4_buffer_uptodate(bh))
		return bh;

	ret = ext4_read_bh_lock(bh, REQ_META | op_flags, true);
	if (ret) {
		put_bh(bh);
		return ERR_PTR(ret);
	}
	return bh;
}

struct buffer_head *ext4_sb_bread(struct super_block *sb, sector_t block,
				   blk_opf_t op_flags)
{
	gfp_t gfp = mapping_gfp_constraint(sb->s_bdev->bd_inode->i_mapping,
			~__GFP_FS) | __GFP_MOVABLE;

	return __ext4_sb_bread_gfp(sb, block, op_flags, gfp);
}

struct buffer_head *ext4_sb_bread_unmovable(struct super_block *sb,
					    sector_t block)
{
	gfp_t gfp = mapping_gfp_constraint(sb->s_bdev->bd_inode->i_mapping,
			~__GFP_FS);

	return __ext4_sb_bread_gfp(sb, block, 0, gfp);
}

void ext4_sb_breadahead_unmovable(struct super_block *sb, sector_t block)
{
	struct buffer_head *bh = bdev_getblk(sb->s_bdev, block,
			sb->s_blocksize, GFP_NOWAIT | __GFP_NOWARN);

	if (likely(bh)) {
		if (trylock_buffer(bh))
			ext4_read_bh_nowait(bh, REQ_RAHEAD, NULL);
		brelse(bh);
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int ext4_verify_csum_type(struct super_block *sb,
				 struct ext4_super_block *es)
{
<<<<<<< HEAD
	if (!EXT4_HAS_RO_COMPAT_FEATURE(sb,
					EXT4_FEATURE_RO_COMPAT_METADATA_CSUM))
=======
	if (!ext4_has_feature_metadata_csum(sb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;

	return es->s_checksum_type == EXT4_CRC32C_CHKSUM;
}

<<<<<<< HEAD
static __le32 ext4_superblock_csum(struct super_block *sb,
				   struct ext4_super_block *es)
=======
__le32 ext4_superblock_csum(struct super_block *sb,
			    struct ext4_super_block *es)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	int offset = offsetof(struct ext4_super_block, s_checksum);
	__u32 csum;

	csum = ext4_chksum(sbi, ~0, (char *)es, offset);

	return cpu_to_le32(csum);
}

<<<<<<< HEAD
int ext4_superblock_csum_verify(struct super_block *sb,
				struct ext4_super_block *es)
{
	if (!EXT4_HAS_RO_COMPAT_FEATURE(sb,
				       EXT4_FEATURE_RO_COMPAT_METADATA_CSUM))
=======
static int ext4_superblock_csum_verify(struct super_block *sb,
				       struct ext4_super_block *es)
{
	if (!ext4_has_metadata_csum(sb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;

	return es->s_checksum == ext4_superblock_csum(sb, es);
}

<<<<<<< HEAD
void ext4_superblock_csum_set(struct super_block *sb,
			      struct ext4_super_block *es)
{
	if (!EXT4_HAS_RO_COMPAT_FEATURE(sb,
		EXT4_FEATURE_RO_COMPAT_METADATA_CSUM))
=======
void ext4_superblock_csum_set(struct super_block *sb)
{
	struct ext4_super_block *es = EXT4_SB(sb)->s_es;

	if (!ext4_has_metadata_csum(sb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	es->s_checksum = ext4_superblock_csum(sb, es);
}

<<<<<<< HEAD
void *ext4_kvmalloc(size_t size, gfp_t flags)
{
	void *ret;

	ret = kmalloc(size, flags);
	if (!ret)
		ret = __vmalloc(size, flags, PAGE_KERNEL);
	return ret;
}

void *ext4_kvzalloc(size_t size, gfp_t flags)
{
	void *ret;

	ret = kzalloc(size, flags);
	if (!ret)
		ret = __vmalloc(size, flags | __GFP_ZERO, PAGE_KERNEL);
	return ret;
}

void ext4_kvfree(void *ptr)
{
	if (is_vmalloc_addr(ptr))
		vfree(ptr);
	else
		kfree(ptr);

}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
ext4_fsblk_t ext4_block_bitmap(struct super_block *sb,
			       struct ext4_group_desc *bg)
{
	return le32_to_cpu(bg->bg_block_bitmap_lo) |
		(EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT ?
		 (ext4_fsblk_t)le32_to_cpu(bg->bg_block_bitmap_hi) << 32 : 0);
}

ext4_fsblk_t ext4_inode_bitmap(struct super_block *sb,
			       struct ext4_group_desc *bg)
{
	return le32_to_cpu(bg->bg_inode_bitmap_lo) |
		(EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT ?
		 (ext4_fsblk_t)le32_to_cpu(bg->bg_inode_bitmap_hi) << 32 : 0);
}

ext4_fsblk_t ext4_inode_table(struct super_block *sb,
			      struct ext4_group_desc *bg)
{
	return le32_to_cpu(bg->bg_inode_table_lo) |
		(EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT ?
		 (ext4_fsblk_t)le32_to_cpu(bg->bg_inode_table_hi) << 32 : 0);
}

__u32 ext4_free_group_clusters(struct super_block *sb,
			       struct ext4_group_desc *bg)
{
	return le16_to_cpu(bg->bg_free_blocks_count_lo) |
		(EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT ?
		 (__u32)le16_to_cpu(bg->bg_free_blocks_count_hi) << 16 : 0);
}

__u32 ext4_free_inodes_count(struct super_block *sb,
			      struct ext4_group_desc *bg)
{
	return le16_to_cpu(bg->bg_free_inodes_count_lo) |
		(EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT ?
		 (__u32)le16_to_cpu(bg->bg_free_inodes_count_hi) << 16 : 0);
}

__u32 ext4_used_dirs_count(struct super_block *sb,
			      struct ext4_group_desc *bg)
{
	return le16_to_cpu(bg->bg_used_dirs_count_lo) |
		(EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT ?
		 (__u32)le16_to_cpu(bg->bg_used_dirs_count_hi) << 16 : 0);
}

__u32 ext4_itable_unused_count(struct super_block *sb,
			      struct ext4_group_desc *bg)
{
	return le16_to_cpu(bg->bg_itable_unused_lo) |
		(EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT ?
		 (__u32)le16_to_cpu(bg->bg_itable_unused_hi) << 16 : 0);
}

void ext4_block_bitmap_set(struct super_block *sb,
			   struct ext4_group_desc *bg, ext4_fsblk_t blk)
{
	bg->bg_block_bitmap_lo = cpu_to_le32((u32)blk);
	if (EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT)
		bg->bg_block_bitmap_hi = cpu_to_le32(blk >> 32);
}

void ext4_inode_bitmap_set(struct super_block *sb,
			   struct ext4_group_desc *bg, ext4_fsblk_t blk)
{
	bg->bg_inode_bitmap_lo  = cpu_to_le32((u32)blk);
	if (EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT)
		bg->bg_inode_bitmap_hi = cpu_to_le32(blk >> 32);
}

void ext4_inode_table_set(struct super_block *sb,
			  struct ext4_group_desc *bg, ext4_fsblk_t blk)
{
	bg->bg_inode_table_lo = cpu_to_le32((u32)blk);
	if (EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT)
		bg->bg_inode_table_hi = cpu_to_le32(blk >> 32);
}

void ext4_free_group_clusters_set(struct super_block *sb,
				  struct ext4_group_desc *bg, __u32 count)
{
	bg->bg_free_blocks_count_lo = cpu_to_le16((__u16)count);
	if (EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT)
		bg->bg_free_blocks_count_hi = cpu_to_le16(count >> 16);
}

void ext4_free_inodes_set(struct super_block *sb,
			  struct ext4_group_desc *bg, __u32 count)
{
	bg->bg_free_inodes_count_lo = cpu_to_le16((__u16)count);
	if (EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT)
		bg->bg_free_inodes_count_hi = cpu_to_le16(count >> 16);
}

void ext4_used_dirs_set(struct super_block *sb,
			  struct ext4_group_desc *bg, __u32 count)
{
	bg->bg_used_dirs_count_lo = cpu_to_le16((__u16)count);
	if (EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT)
		bg->bg_used_dirs_count_hi = cpu_to_le16(count >> 16);
}

void ext4_itable_unused_set(struct super_block *sb,
			  struct ext4_group_desc *bg, __u32 count)
{
	bg->bg_itable_unused_lo = cpu_to_le16((__u16)count);
	if (EXT4_DESC_SIZE(sb) >= EXT4_MIN_DESC_SIZE_64BIT)
		bg->bg_itable_unused_hi = cpu_to_le16(count >> 16);
}

<<<<<<< HEAD

/* Just increment the non-pointer handle value */
static handle_t *ext4_get_nojournal(void)
{
	handle_t *handle = current->journal_info;
	unsigned long ref_cnt = (unsigned long)handle;

	BUG_ON(ref_cnt >= EXT4_NOJOURNAL_MAX_REF_COUNT);

	ref_cnt++;
	handle = (handle_t *)ref_cnt;

	current->journal_info = handle;
	return handle;
}


/* Decrement the non-pointer handle value */
static void ext4_put_nojournal(handle_t *handle)
{
	unsigned long ref_cnt = (unsigned long)handle;

	BUG_ON(ref_cnt == 0);

	ref_cnt--;
	handle = (handle_t *)ref_cnt;

	current->journal_info = handle;
}

/*
 * Wrappers for jbd2_journal_start/end.
 *
 * The only special thing we need to do here is to make sure that all
 * journal_end calls result in the superblock being marked dirty, so
 * that sync() will call the filesystem's write_super callback if
 * appropriate.
 *
 * To avoid j_barrier hold in userspace when a user calls freeze(),
 * ext4 prevents a new handle from being started by s_frozen, which
 * is in an upper layer.
 */
handle_t *ext4_journal_start_sb(struct super_block *sb, int nblocks)
{
	journal_t *journal;
	handle_t  *handle;

	trace_ext4_journal_start(sb, nblocks, _RET_IP_);
	if (sb->s_flags & MS_RDONLY)
		return ERR_PTR(-EROFS);

	journal = EXT4_SB(sb)->s_journal;
	handle = ext4_journal_current_handle();

	/*
	 * If a handle has been started, it should be allowed to
	 * finish, otherwise deadlock could happen between freeze
	 * and others(e.g. truncate) due to the restart of the
	 * journal handle if the filesystem is forzen and active
	 * handles are not stopped.
	 */
	if (!handle)
		vfs_check_frozen(sb, SB_FREEZE_TRANS);

	if (!journal)
		return ext4_get_nojournal();
	/*
	 * Special case here: if the journal has aborted behind our
	 * backs (eg. EIO in the commit thread), then we still need to
	 * take the FS itself readonly cleanly.
	 */
	if (is_journal_aborted(journal)) {
		ext4_abort(sb, "Detected aborted journal");
		return ERR_PTR(-EROFS);
	}
	return jbd2_journal_start(journal, nblocks);
}

/*
 * The only special thing we need to do here is to make sure that all
 * jbd2_journal_stop calls result in the superblock being marked dirty, so
 * that sync() will call the filesystem's write_super callback if
 * appropriate.
 */
int __ext4_journal_stop(const char *where, unsigned int line, handle_t *handle)
{
	struct super_block *sb;
	int err;
	int rc;

	if (!ext4_handle_valid(handle)) {
		ext4_put_nojournal(handle);
		return 0;
	}
	sb = handle->h_transaction->t_journal->j_private;
	err = handle->h_err;
	rc = jbd2_journal_stop(handle);

	if (!err)
		err = rc;
	if (err)
		__ext4_std_error(sb, where, line, err);
	return err;
}

void ext4_journal_abort_handle(const char *caller, unsigned int line,
			       const char *err_fn, struct buffer_head *bh,
			       handle_t *handle, int err)
{
	char nbuf[16];
	const char *errstr = ext4_decode_error(NULL, err, nbuf);

	BUG_ON(!ext4_handle_valid(handle));

	if (bh)
		BUFFER_TRACE(bh, "abort");

	if (!handle->h_err)
		handle->h_err = err;

	if (is_handle_aborted(handle))
		return;

	printk(KERN_ERR "EXT4-fs: %s:%d: aborting transaction: %s in %s\n",
	       caller, line, errstr, err_fn);

	jbd2_journal_abort_handle(handle);
}

static void __save_error_info(struct super_block *sb, const char *func,
			    unsigned int line)
{
	struct ext4_super_block *es = EXT4_SB(sb)->s_es;

	EXT4_SB(sb)->s_mount_state |= EXT4_ERROR_FS;
	es->s_state |= cpu_to_le16(EXT4_ERROR_FS);
	es->s_last_error_time = cpu_to_le32(get_seconds());
	strncpy(es->s_last_error_func, func, sizeof(es->s_last_error_func));
	es->s_last_error_line = cpu_to_le32(line);
	if (!es->s_first_error_time) {
		es->s_first_error_time = es->s_last_error_time;
		strncpy(es->s_first_error_func, func,
			sizeof(es->s_first_error_func));
		es->s_first_error_line = cpu_to_le32(line);
		es->s_first_error_ino = es->s_last_error_ino;
		es->s_first_error_block = es->s_last_error_block;
	}
	/*
	 * Start the daily error reporting function if it hasn't been
	 * started already
	 */
	if (!es->s_error_count)
		mod_timer(&EXT4_SB(sb)->s_err_report, jiffies + 24*60*60*HZ);
	es->s_error_count = cpu_to_le32(le32_to_cpu(es->s_error_count) + 1);
}

static void save_error_info(struct super_block *sb, const char *func,
			    unsigned int line)
{
	__save_error_info(sb, func, line);
	ext4_commit_super(sb, 1);
=======
static void __ext4_update_tstamp(__le32 *lo, __u8 *hi, time64_t now)
{
	now = clamp_val(now, 0, (1ull << 40) - 1);

	*lo = cpu_to_le32(lower_32_bits(now));
	*hi = upper_32_bits(now);
}

static time64_t __ext4_get_tstamp(__le32 *lo, __u8 *hi)
{
	return ((time64_t)(*hi) << 32) + le32_to_cpu(*lo);
}
#define ext4_update_tstamp(es, tstamp) \
	__ext4_update_tstamp(&(es)->tstamp, &(es)->tstamp ## _hi, \
			     ktime_get_real_seconds())
#define ext4_get_tstamp(es, tstamp) \
	__ext4_get_tstamp(&(es)->tstamp, &(es)->tstamp ## _hi)

#define EXT4_SB_REFRESH_INTERVAL_SEC (3600) /* seconds (1 hour) */
#define EXT4_SB_REFRESH_INTERVAL_KB (16384) /* kilobytes (16MB) */

/*
 * The ext4_maybe_update_superblock() function checks and updates the
 * superblock if needed.
 *
 * This function is designed to update the on-disk superblock only under
 * certain conditions to prevent excessive disk writes and unnecessary
 * waking of the disk from sleep. The superblock will be updated if:
 * 1. More than an hour has passed since the last superblock update, and
 * 2. More than 16MB have been written since the last superblock update.
 *
 * @sb: The superblock
 */
static void ext4_maybe_update_superblock(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
	journal_t *journal = sbi->s_journal;
	time64_t now;
	__u64 last_update;
	__u64 lifetime_write_kbytes;
	__u64 diff_size;

	if (sb_rdonly(sb) || !(sb->s_flags & SB_ACTIVE) ||
	    !journal || (journal->j_flags & JBD2_UNMOUNT))
		return;

	now = ktime_get_real_seconds();
	last_update = ext4_get_tstamp(es, s_wtime);

	if (likely(now - last_update < EXT4_SB_REFRESH_INTERVAL_SEC))
		return;

	lifetime_write_kbytes = sbi->s_kbytes_written +
		((part_stat_read(sb->s_bdev, sectors[STAT_WRITE]) -
		  sbi->s_sectors_written_start) >> 1);

	/* Get the number of kilobytes not written to disk to account
	 * for statistics and compare with a multiple of 16 MB. This
	 * is used to determine when the next superblock commit should
	 * occur (i.e. not more often than once per 16MB if there was
	 * less written in an hour).
	 */
	diff_size = lifetime_write_kbytes - le64_to_cpu(es->s_kbytes_written);

	if (diff_size > EXT4_SB_REFRESH_INTERVAL_KB)
		schedule_work(&EXT4_SB(sb)->s_sb_upd_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * The del_gendisk() function uninitializes the disk-specific data
 * structures, including the bdi structure, without telling anyone
 * else.  Once this happens, any attempt to call mark_buffer_dirty()
 * (for example, by ext4_commit_super), will cause a kernel OOPS.
 * This is a kludge to prevent these oops until we can put in a proper
 * hook in del_gendisk() to inform the VFS and file system layers.
 */
static int block_device_ejected(struct super_block *sb)
{
	struct inode *bd_inode = sb->s_bdev->bd_inode;
<<<<<<< HEAD
	struct backing_dev_info *bdi = bd_inode->i_mapping->backing_dev_info;
=======
	struct backing_dev_info *bdi = inode_to_bdi(bd_inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return bdi->dev == NULL;
}

static void ext4_journal_commit_callback(journal_t *journal, transaction_t *txn)
{
	struct super_block		*sb = journal->j_private;
	struct ext4_sb_info		*sbi = EXT4_SB(sb);
	int				error = is_journal_aborted(journal);
	struct ext4_journal_cb_entry	*jce;

	BUG_ON(txn->t_state == T_FINISHED);
<<<<<<< HEAD
=======

	ext4_process_freed_data(sb, txn->t_tid);
	ext4_maybe_update_superblock(sb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&sbi->s_md_lock);
	while (!list_empty(&txn->t_private_list)) {
		jce = list_entry(txn->t_private_list.next,
				 struct ext4_journal_cb_entry, jce_list);
		list_del_init(&jce->jce_list);
		spin_unlock(&sbi->s_md_lock);
		jce->jce_func(sb, jce, error);
		spin_lock(&sbi->s_md_lock);
	}
	spin_unlock(&sbi->s_md_lock);
}

<<<<<<< HEAD
=======
/*
 * This writepage callback for write_cache_pages()
 * takes care of a few cases after page cleaning.
 *
 * write_cache_pages() already checks for dirty pages
 * and calls clear_page_dirty_for_io(), which we want,
 * to write protect the pages.
 *
 * However, we may have to redirty a page (see below.)
 */
static int ext4_journalled_writepage_callback(struct folio *folio,
					      struct writeback_control *wbc,
					      void *data)
{
	transaction_t *transaction = (transaction_t *) data;
	struct buffer_head *bh, *head;
	struct journal_head *jh;

	bh = head = folio_buffers(folio);
	do {
		/*
		 * We have to redirty a page in these cases:
		 * 1) If buffer is dirty, it means the page was dirty because it
		 * contains a buffer that needs checkpointing. So the dirty bit
		 * needs to be preserved so that checkpointing writes the buffer
		 * properly.
		 * 2) If buffer is not part of the committing transaction
		 * (we may have just accidentally come across this buffer because
		 * inode range tracking is not exact) or if the currently running
		 * transaction already contains this buffer as well, dirty bit
		 * needs to be preserved so that the buffer gets writeprotected
		 * properly on running transaction's commit.
		 */
		jh = bh2jh(bh);
		if (buffer_dirty(bh) ||
		    (jh && (jh->b_transaction != transaction ||
			    jh->b_next_transaction))) {
			folio_redirty_for_writepage(wbc, folio);
			goto out;
		}
	} while ((bh = bh->b_this_page) != head);

out:
	return AOP_WRITEPAGE_ACTIVATE;
}

static int ext4_journalled_submit_inode_data_buffers(struct jbd2_inode *jinode)
{
	struct address_space *mapping = jinode->i_vfs_inode->i_mapping;
	struct writeback_control wbc = {
		.sync_mode =  WB_SYNC_ALL,
		.nr_to_write = LONG_MAX,
		.range_start = jinode->i_dirty_start,
		.range_end = jinode->i_dirty_end,
        };

	return write_cache_pages(mapping, &wbc,
				 ext4_journalled_writepage_callback,
				 jinode->i_transaction);
}

static int ext4_journal_submit_inode_data_buffers(struct jbd2_inode *jinode)
{
	int ret;

	if (ext4_should_journal_data(jinode->i_vfs_inode))
		ret = ext4_journalled_submit_inode_data_buffers(jinode);
	else
		ret = ext4_normal_submit_inode_data_buffers(jinode);
	return ret;
}

static int ext4_journal_finish_inode_data_buffers(struct jbd2_inode *jinode)
{
	int ret = 0;

	if (!ext4_should_journal_data(jinode->i_vfs_inode))
		ret = jbd2_journal_finish_inode_data_buffers(jinode);

	return ret;
}

static bool system_going_down(void)
{
	return system_state == SYSTEM_HALT || system_state == SYSTEM_POWER_OFF
		|| system_state == SYSTEM_RESTART;
}

struct ext4_err_translation {
	int code;
	int errno;
};

#define EXT4_ERR_TRANSLATE(err) { .code = EXT4_ERR_##err, .errno = err }

static struct ext4_err_translation err_translation[] = {
	EXT4_ERR_TRANSLATE(EIO),
	EXT4_ERR_TRANSLATE(ENOMEM),
	EXT4_ERR_TRANSLATE(EFSBADCRC),
	EXT4_ERR_TRANSLATE(EFSCORRUPTED),
	EXT4_ERR_TRANSLATE(ENOSPC),
	EXT4_ERR_TRANSLATE(ENOKEY),
	EXT4_ERR_TRANSLATE(EROFS),
	EXT4_ERR_TRANSLATE(EFBIG),
	EXT4_ERR_TRANSLATE(EEXIST),
	EXT4_ERR_TRANSLATE(ERANGE),
	EXT4_ERR_TRANSLATE(EOVERFLOW),
	EXT4_ERR_TRANSLATE(EBUSY),
	EXT4_ERR_TRANSLATE(ENOTDIR),
	EXT4_ERR_TRANSLATE(ENOTEMPTY),
	EXT4_ERR_TRANSLATE(ESHUTDOWN),
	EXT4_ERR_TRANSLATE(EFAULT),
};

static int ext4_errno_to_code(int errno)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(err_translation); i++)
		if (err_translation[i].errno == errno)
			return err_translation[i].code;
	return EXT4_ERR_UNKNOWN;
}

static void save_error_info(struct super_block *sb, int error,
			    __u32 ino, __u64 block,
			    const char *func, unsigned int line)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	/* We default to EFSCORRUPTED error... */
	if (error == 0)
		error = EFSCORRUPTED;

	spin_lock(&sbi->s_error_lock);
	sbi->s_add_error_count++;
	sbi->s_last_error_code = error;
	sbi->s_last_error_line = line;
	sbi->s_last_error_ino = ino;
	sbi->s_last_error_block = block;
	sbi->s_last_error_func = func;
	sbi->s_last_error_time = ktime_get_real_seconds();
	if (!sbi->s_first_error_time) {
		sbi->s_first_error_code = error;
		sbi->s_first_error_line = line;
		sbi->s_first_error_ino = ino;
		sbi->s_first_error_block = block;
		sbi->s_first_error_func = func;
		sbi->s_first_error_time = sbi->s_last_error_time;
	}
	spin_unlock(&sbi->s_error_lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Deal with the reporting of failure conditions on a filesystem such as
 * inconsistencies detected or read IO failures.
 *
 * On ext2, we can store the error state of the filesystem in the
 * superblock.  That is not possible on ext4, because we may have other
 * write ordering constraints on the superblock which prevent us from
 * writing it out straight away; and given that the journal is about to
 * be aborted, we can't rely on the current, or future, transactions to
 * write out the superblock safely.
 *
 * We'll just use the jbd2_journal_abort() error code to record an error in
 * the journal instead.  On recovery, the journal will complain about
 * that error until we've noted it down and cleared it.
<<<<<<< HEAD
 */

static void ext4_handle_error(struct super_block *sb)
{
	if (sb->s_flags & MS_RDONLY)
		return;

	if (!test_opt(sb, ERRORS_CONT)) {
		journal_t *journal = EXT4_SB(sb)->s_journal;

		EXT4_SB(sb)->s_mount_flags |= EXT4_MF_FS_ABORTED;
		if (journal)
			jbd2_journal_abort(journal, -EIO);
	}
	if (test_opt(sb, ERRORS_RO)) {
		ext4_msg(sb, KERN_CRIT, "Remounting filesystem read-only");
		sb->s_flags |= MS_RDONLY;
	}
	if (test_opt(sb, ERRORS_PANIC)) {
		if (EXT4_SB(sb)->s_journal &&
		  !(EXT4_SB(sb)->s_journal->j_flags & JBD2_REC_ERR))
			return;
		panic("EXT4-fs (device %s): panic forced after error\n",
			sb->s_id);
	}
}

void __ext4_error(struct super_block *sb, const char *function,
		  unsigned int line, const char *fmt, ...)
=======
 *
 * If force_ro is set, we unconditionally force the filesystem into an
 * ABORT|READONLY state, unless the error response on the fs has been set to
 * panic in which case we take the easy way out and panic immediately. This is
 * used to deal with unrecoverable failures such as journal IO errors or ENOMEM
 * at a critical moment in log management.
 */
static void ext4_handle_error(struct super_block *sb, bool force_ro, int error,
			      __u32 ino, __u64 block,
			      const char *func, unsigned int line)
{
	journal_t *journal = EXT4_SB(sb)->s_journal;
	bool continue_fs = !force_ro && test_opt(sb, ERRORS_CONT);

	EXT4_SB(sb)->s_mount_state |= EXT4_ERROR_FS;
	if (test_opt(sb, WARN_ON_ERROR))
		WARN_ON_ONCE(1);

	if (!continue_fs && !sb_rdonly(sb)) {
		set_bit(EXT4_FLAGS_SHUTDOWN, &EXT4_SB(sb)->s_ext4_flags);
		if (journal)
			jbd2_journal_abort(journal, -EIO);
	}

	if (!bdev_read_only(sb->s_bdev)) {
		save_error_info(sb, error, ino, block, func, line);
		/*
		 * In case the fs should keep running, we need to writeout
		 * superblock through the journal. Due to lock ordering
		 * constraints, it may not be safe to do it right here so we
		 * defer superblock flushing to a workqueue.
		 */
		if (continue_fs && journal)
			schedule_work(&EXT4_SB(sb)->s_sb_upd_work);
		else
			ext4_commit_super(sb);
	}

	/*
	 * We force ERRORS_RO behavior when system is rebooting. Otherwise we
	 * could panic during 'reboot -f' as the underlying device got already
	 * disabled.
	 */
	if (test_opt(sb, ERRORS_PANIC) && !system_going_down()) {
		panic("EXT4-fs (device %s): panic forced after error\n",
			sb->s_id);
	}

	if (sb_rdonly(sb) || continue_fs)
		return;

	ext4_msg(sb, KERN_CRIT, "Remounting filesystem read-only");
	/*
	 * Make sure updated value of ->s_mount_flags will be visible before
	 * ->s_flags update
	 */
	smp_wmb();
	sb->s_flags |= SB_RDONLY;
}

static void update_super_work(struct work_struct *work)
{
	struct ext4_sb_info *sbi = container_of(work, struct ext4_sb_info,
						s_sb_upd_work);
	journal_t *journal = sbi->s_journal;
	handle_t *handle;

	/*
	 * If the journal is still running, we have to write out superblock
	 * through the journal to avoid collisions of other journalled sb
	 * updates.
	 *
	 * We use directly jbd2 functions here to avoid recursing back into
	 * ext4 error handling code during handling of previous errors.
	 */
	if (!sb_rdonly(sbi->s_sb) && journal) {
		struct buffer_head *sbh = sbi->s_sbh;
		bool call_notify_err = false;

		handle = jbd2_journal_start(journal, 1);
		if (IS_ERR(handle))
			goto write_directly;
		if (jbd2_journal_get_write_access(handle, sbh)) {
			jbd2_journal_stop(handle);
			goto write_directly;
		}

		if (sbi->s_add_error_count > 0)
			call_notify_err = true;

		ext4_update_super(sbi->s_sb);
		if (buffer_write_io_error(sbh) || !buffer_uptodate(sbh)) {
			ext4_msg(sbi->s_sb, KERN_ERR, "previous I/O error to "
				 "superblock detected");
			clear_buffer_write_io_error(sbh);
			set_buffer_uptodate(sbh);
		}

		if (jbd2_journal_dirty_metadata(handle, sbh)) {
			jbd2_journal_stop(handle);
			goto write_directly;
		}
		jbd2_journal_stop(handle);

		if (call_notify_err)
			ext4_notify_error_sysfs(sbi);

		return;
	}
write_directly:
	/*
	 * Write through journal failed. Write sb directly to get error info
	 * out and hope for the best.
	 */
	ext4_commit_super(sbi->s_sb);
	ext4_notify_error_sysfs(sbi);
}

#define ext4_error_ratelimit(sb)					\
		___ratelimit(&(EXT4_SB(sb)->s_err_ratelimit_state),	\
			     "EXT4-fs error")

void __ext4_error(struct super_block *sb, const char *function,
		  unsigned int line, bool force_ro, int error, __u64 block,
		  const char *fmt, ...)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct va_format vaf;
	va_list args;

<<<<<<< HEAD
	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	printk(KERN_CRIT "EXT4-fs error (device %s): %s:%d: comm %s: %pV\n",
	       sb->s_id, function, line, current->comm, &vaf);
	va_end(args);
	save_error_info(sb, function, line);

	ext4_handle_error(sb);
}

void ext4_error_inode(struct inode *inode, const char *function,
		      unsigned int line, ext4_fsblk_t block,
		      const char *fmt, ...)
{
	va_list args;
	struct va_format vaf;
	struct ext4_super_block *es = EXT4_SB(inode->i_sb)->s_es;

	es->s_last_error_ino = cpu_to_le32(inode->i_ino);
	es->s_last_error_block = cpu_to_le64(block);
	save_error_info(inode->i_sb, function, line);
	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	if (block)
		printk(KERN_CRIT "EXT4-fs error (device %s): %s:%d: "
		       "inode #%lu: block %llu: comm %s: %pV\n",
		       inode->i_sb->s_id, function, line, inode->i_ino,
		       block, current->comm, &vaf);
	else
		printk(KERN_CRIT "EXT4-fs error (device %s): %s:%d: "
		       "inode #%lu: comm %s: %pV\n",
		       inode->i_sb->s_id, function, line, inode->i_ino,
		       current->comm, &vaf);
	va_end(args);

	ext4_handle_error(inode->i_sb);
}

void ext4_error_file(struct file *file, const char *function,
		     unsigned int line, ext4_fsblk_t block,
		     const char *fmt, ...)
{
	va_list args;
	struct va_format vaf;
	struct ext4_super_block *es;
	struct inode *inode = file->f_dentry->d_inode;
	char pathname[80], *path;

	es = EXT4_SB(inode->i_sb)->s_es;
	es->s_last_error_ino = cpu_to_le32(inode->i_ino);
	save_error_info(inode->i_sb, function, line);
	path = d_path(&(file->f_path), pathname, sizeof(pathname));
	if (IS_ERR(path))
		path = "(unknown)";
	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	if (block)
		printk(KERN_CRIT
		       "EXT4-fs error (device %s): %s:%d: inode #%lu: "
		       "block %llu: comm %s: path %s: %pV\n",
		       inode->i_sb->s_id, function, line, inode->i_ino,
		       block, current->comm, path, &vaf);
	else
		printk(KERN_CRIT
		       "EXT4-fs error (device %s): %s:%d: inode #%lu: "
		       "comm %s: path %s: %pV\n",
		       inode->i_sb->s_id, function, line, inode->i_ino,
		       current->comm, path, &vaf);
	va_end(args);

	ext4_handle_error(inode->i_sb);
}

static const char *ext4_decode_error(struct super_block *sb, int errno,
				     char nbuf[16])
=======
	if (unlikely(ext4_forced_shutdown(sb)))
		return;

	trace_ext4_error(sb, function, line);
	if (ext4_error_ratelimit(sb)) {
		va_start(args, fmt);
		vaf.fmt = fmt;
		vaf.va = &args;
		printk(KERN_CRIT
		       "EXT4-fs error (device %s): %s:%d: comm %s: %pV\n",
		       sb->s_id, function, line, current->comm, &vaf);
		va_end(args);
	}
	fsnotify_sb_error(sb, NULL, error ? error : EFSCORRUPTED);

	ext4_handle_error(sb, force_ro, error, 0, block, function, line);
}

void __ext4_error_inode(struct inode *inode, const char *function,
			unsigned int line, ext4_fsblk_t block, int error,
			const char *fmt, ...)
{
	va_list args;
	struct va_format vaf;

	if (unlikely(ext4_forced_shutdown(inode->i_sb)))
		return;

	trace_ext4_error(inode->i_sb, function, line);
	if (ext4_error_ratelimit(inode->i_sb)) {
		va_start(args, fmt);
		vaf.fmt = fmt;
		vaf.va = &args;
		if (block)
			printk(KERN_CRIT "EXT4-fs error (device %s): %s:%d: "
			       "inode #%lu: block %llu: comm %s: %pV\n",
			       inode->i_sb->s_id, function, line, inode->i_ino,
			       block, current->comm, &vaf);
		else
			printk(KERN_CRIT "EXT4-fs error (device %s): %s:%d: "
			       "inode #%lu: comm %s: %pV\n",
			       inode->i_sb->s_id, function, line, inode->i_ino,
			       current->comm, &vaf);
		va_end(args);
	}
	fsnotify_sb_error(inode->i_sb, inode, error ? error : EFSCORRUPTED);

	ext4_handle_error(inode->i_sb, false, error, inode->i_ino, block,
			  function, line);
}

void __ext4_error_file(struct file *file, const char *function,
		       unsigned int line, ext4_fsblk_t block,
		       const char *fmt, ...)
{
	va_list args;
	struct va_format vaf;
	struct inode *inode = file_inode(file);
	char pathname[80], *path;

	if (unlikely(ext4_forced_shutdown(inode->i_sb)))
		return;

	trace_ext4_error(inode->i_sb, function, line);
	if (ext4_error_ratelimit(inode->i_sb)) {
		path = file_path(file, pathname, sizeof(pathname));
		if (IS_ERR(path))
			path = "(unknown)";
		va_start(args, fmt);
		vaf.fmt = fmt;
		vaf.va = &args;
		if (block)
			printk(KERN_CRIT
			       "EXT4-fs error (device %s): %s:%d: inode #%lu: "
			       "block %llu: comm %s: path %s: %pV\n",
			       inode->i_sb->s_id, function, line, inode->i_ino,
			       block, current->comm, path, &vaf);
		else
			printk(KERN_CRIT
			       "EXT4-fs error (device %s): %s:%d: inode #%lu: "
			       "comm %s: path %s: %pV\n",
			       inode->i_sb->s_id, function, line, inode->i_ino,
			       current->comm, path, &vaf);
		va_end(args);
	}
	fsnotify_sb_error(inode->i_sb, inode, EFSCORRUPTED);

	ext4_handle_error(inode->i_sb, false, EFSCORRUPTED, inode->i_ino, block,
			  function, line);
}

const char *ext4_decode_error(struct super_block *sb, int errno,
			      char nbuf[16])
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *errstr = NULL;

	switch (errno) {
<<<<<<< HEAD
=======
	case -EFSCORRUPTED:
		errstr = "Corrupt filesystem";
		break;
	case -EFSBADCRC:
		errstr = "Filesystem failed CRC";
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case -EIO:
		errstr = "IO failure";
		break;
	case -ENOMEM:
		errstr = "Out of memory";
		break;
	case -EROFS:
		if (!sb || (EXT4_SB(sb)->s_journal &&
			    EXT4_SB(sb)->s_journal->j_flags & JBD2_ABORT))
			errstr = "Journal has aborted";
		else
			errstr = "Readonly filesystem";
		break;
	default:
		/* If the caller passed in an extra buffer for unknown
		 * errors, textualise them now.  Else we just return
		 * NULL. */
		if (nbuf) {
			/* Check for truncated error codes... */
			if (snprintf(nbuf, 16, "error %d", -errno) >= 0)
				errstr = nbuf;
		}
		break;
	}

	return errstr;
}

/* __ext4_std_error decodes expected errors from journaling functions
 * automatically and invokes the appropriate error response.  */

void __ext4_std_error(struct super_block *sb, const char *function,
		      unsigned int line, int errno)
{
	char nbuf[16];
	const char *errstr;

<<<<<<< HEAD
	/* Special case: if the error is EROFS, and we're not already
	 * inside a transaction, then there's really no point in logging
	 * an error. */
	if (errno == -EROFS && journal_current_handle() == NULL &&
	    (sb->s_flags & MS_RDONLY))
		return;

	errstr = ext4_decode_error(sb, errno, nbuf);
	printk(KERN_CRIT "EXT4-fs error (device %s) in %s:%d: %s\n",
	       sb->s_id, function, line, errstr);
	save_error_info(sb, function, line);

	ext4_handle_error(sb);
}

/*
 * ext4_abort is a much stronger failure handler than ext4_error.  The
 * abort function may be used to deal with unrecoverable failures such
 * as journal IO errors or ENOMEM at a critical moment in log management.
 *
 * We unconditionally force the filesystem into an ABORT|READONLY state,
 * unless the error response on the fs has been set to panic in which
 * case we take the easy way out and panic immediately.
 */

void __ext4_abort(struct super_block *sb, const char *function,
		unsigned int line, const char *fmt, ...)
{
	va_list args;

	save_error_info(sb, function, line);
	va_start(args, fmt);
	printk(KERN_CRIT "EXT4-fs error (device %s): %s:%d: ", sb->s_id,
	       function, line);
	vprintk(fmt, args);
	printk("\n");
	va_end(args);

	if ((sb->s_flags & MS_RDONLY) == 0) {
		ext4_msg(sb, KERN_CRIT, "Remounting filesystem read-only");
		sb->s_flags |= MS_RDONLY;
		EXT4_SB(sb)->s_mount_flags |= EXT4_MF_FS_ABORTED;
		if (EXT4_SB(sb)->s_journal)
			jbd2_journal_abort(EXT4_SB(sb)->s_journal, -EIO);
		save_error_info(sb, function, line);
	#ifdef CONFIG_MACH_LGE
	/* LGE_CHANGE
	 * put panic when ext4 partition is remounted as Read Only
	 * 2014-04-15, B2-BSP-FS@lge.com
	 */
	panic("EXT4-fs panic from previous error. remounted as RO \n");
	#endif

	}
	if (test_opt(sb, ERRORS_PANIC)) {
		if (EXT4_SB(sb)->s_journal &&
		  !(EXT4_SB(sb)->s_journal->j_flags & JBD2_REC_ERR))
			return;
		panic("EXT4-fs panic from previous error\n");
	}
}

void ext4_msg(struct super_block *sb, const char *prefix, const char *fmt, ...)
=======
	if (unlikely(ext4_forced_shutdown(sb)))
		return;

	/* Special case: if the error is EROFS, and we're not already
	 * inside a transaction, then there's really no point in logging
	 * an error. */
	if (errno == -EROFS && journal_current_handle() == NULL && sb_rdonly(sb))
		return;

	if (ext4_error_ratelimit(sb)) {
		errstr = ext4_decode_error(sb, errno, nbuf);
		printk(KERN_CRIT "EXT4-fs error (device %s) in %s:%d: %s\n",
		       sb->s_id, function, line, errstr);
	}
	fsnotify_sb_error(sb, NULL, errno ? errno : EFSCORRUPTED);

	ext4_handle_error(sb, false, -errno, 0, 0, function, line);
}

void __ext4_msg(struct super_block *sb,
		const char *prefix, const char *fmt, ...)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct va_format vaf;
	va_list args;

<<<<<<< HEAD
	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	printk_ratelimited("%sEXT4-fs (%s): %pV\n", prefix, sb->s_id, &vaf);
	va_end(args);
}

=======
	if (sb) {
		atomic_inc(&EXT4_SB(sb)->s_msg_count);
		if (!___ratelimit(&(EXT4_SB(sb)->s_msg_ratelimit_state),
				  "EXT4-fs"))
			return;
	}

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	if (sb)
		printk("%sEXT4-fs (%s): %pV\n", prefix, sb->s_id, &vaf);
	else
		printk("%sEXT4-fs: %pV\n", prefix, &vaf);
	va_end(args);
}

static int ext4_warning_ratelimit(struct super_block *sb)
{
	atomic_inc(&EXT4_SB(sb)->s_warning_count);
	return ___ratelimit(&(EXT4_SB(sb)->s_warning_ratelimit_state),
			    "EXT4-fs warning");
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void __ext4_warning(struct super_block *sb, const char *function,
		    unsigned int line, const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;

<<<<<<< HEAD
=======
	if (!ext4_warning_ratelimit(sb))
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	printk(KERN_WARNING "EXT4-fs warning (device %s): %s:%d: %pV\n",
	       sb->s_id, function, line, &vaf);
	va_end(args);
}

<<<<<<< HEAD
=======
void __ext4_warning_inode(const struct inode *inode, const char *function,
			  unsigned int line, const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;

	if (!ext4_warning_ratelimit(inode->i_sb))
		return;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	printk(KERN_WARNING "EXT4-fs warning (device %s): %s:%d: "
	       "inode #%lu: comm %s: %pV\n", inode->i_sb->s_id,
	       function, line, inode->i_ino, current->comm, &vaf);
	va_end(args);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void __ext4_grp_locked_error(const char *function, unsigned int line,
			     struct super_block *sb, ext4_group_t grp,
			     unsigned long ino, ext4_fsblk_t block,
			     const char *fmt, ...)
__releases(bitlock)
__acquires(bitlock)
{
	struct va_format vaf;
	va_list args;
<<<<<<< HEAD
	struct ext4_super_block *es = EXT4_SB(sb)->s_es;

	es->s_last_error_ino = cpu_to_le32(ino);
	es->s_last_error_block = cpu_to_le64(block);
	__save_error_info(sb, function, line);

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;
	printk(KERN_CRIT "EXT4-fs error (device %s): %s:%d: group %u, ",
	       sb->s_id, function, line, grp);
	if (ino)
		printk(KERN_CONT "inode %lu: ", ino);
	if (block)
		printk(KERN_CONT "block %llu:", (unsigned long long) block);
	printk(KERN_CONT "%pV\n", &vaf);
	va_end(args);

	if (test_opt(sb, ERRORS_CONT)) {
		ext4_commit_super(sb, 0);
		return;
	}

	ext4_unlock_group(sb, grp);
	ext4_handle_error(sb);
=======

	if (unlikely(ext4_forced_shutdown(sb)))
		return;

	trace_ext4_error(sb, function, line);
	if (ext4_error_ratelimit(sb)) {
		va_start(args, fmt);
		vaf.fmt = fmt;
		vaf.va = &args;
		printk(KERN_CRIT "EXT4-fs error (device %s): %s:%d: group %u, ",
		       sb->s_id, function, line, grp);
		if (ino)
			printk(KERN_CONT "inode %lu: ", ino);
		if (block)
			printk(KERN_CONT "block %llu:",
			       (unsigned long long) block);
		printk(KERN_CONT "%pV\n", &vaf);
		va_end(args);
	}

	if (test_opt(sb, ERRORS_CONT)) {
		if (test_opt(sb, WARN_ON_ERROR))
			WARN_ON_ONCE(1);
		EXT4_SB(sb)->s_mount_state |= EXT4_ERROR_FS;
		if (!bdev_read_only(sb->s_bdev)) {
			save_error_info(sb, EFSCORRUPTED, ino, block, function,
					line);
			schedule_work(&EXT4_SB(sb)->s_sb_upd_work);
		}
		return;
	}
	ext4_unlock_group(sb, grp);
	ext4_handle_error(sb, false, EFSCORRUPTED, ino, block, function, line);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * We only get here in the ERRORS_RO case; relocking the group
	 * may be dangerous, but nothing bad will happen since the
	 * filesystem will have already been marked read/only and the
	 * journal has been aborted.  We return 1 as a hint to callers
	 * who might what to use the return value from
	 * ext4_grp_locked_error() to distinguish between the
	 * ERRORS_CONT and ERRORS_RO case, and perhaps return more
	 * aggressively from the ext4 function in question, with a
	 * more appropriate error code.
	 */
	ext4_lock_group(sb, grp);
	return;
}

<<<<<<< HEAD
=======
void ext4_mark_group_bitmap_corrupted(struct super_block *sb,
				     ext4_group_t group,
				     unsigned int flags)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_group_info *grp = ext4_get_group_info(sb, group);
	struct ext4_group_desc *gdp = ext4_get_group_desc(sb, group, NULL);
	int ret;

	if (!grp || !gdp)
		return;
	if (flags & EXT4_GROUP_INFO_BBITMAP_CORRUPT) {
		ret = ext4_test_and_set_bit(EXT4_GROUP_INFO_BBITMAP_CORRUPT_BIT,
					    &grp->bb_state);
		if (!ret)
			percpu_counter_sub(&sbi->s_freeclusters_counter,
					   grp->bb_free);
	}

	if (flags & EXT4_GROUP_INFO_IBITMAP_CORRUPT) {
		ret = ext4_test_and_set_bit(EXT4_GROUP_INFO_IBITMAP_CORRUPT_BIT,
					    &grp->bb_state);
		if (!ret && gdp) {
			int count;

			count = ext4_free_inodes_count(sb, gdp);
			percpu_counter_sub(&sbi->s_freeinodes_counter,
					   count);
		}
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void ext4_update_dynamic_rev(struct super_block *sb)
{
	struct ext4_super_block *es = EXT4_SB(sb)->s_es;

	if (le32_to_cpu(es->s_rev_level) > EXT4_GOOD_OLD_REV)
		return;

	ext4_warning(sb,
		     "updating to rev %d because of new feature flag, "
		     "running e2fsck is recommended",
		     EXT4_DYNAMIC_REV);

	es->s_first_ino = cpu_to_le32(EXT4_GOOD_OLD_FIRST_INO);
	es->s_inode_size = cpu_to_le16(EXT4_GOOD_OLD_INODE_SIZE);
	es->s_rev_level = cpu_to_le32(EXT4_DYNAMIC_REV);
	/* leave es->s_feature_*compat flags alone */
	/* es->s_uuid will be set by e2fsck if empty */

	/*
	 * The rest of the superblock fields should be zero, and if not it
	 * means they are likely already in use, so leave them alone.  We
	 * can leave it up to e2fsck to clean up any inconsistencies there.
	 */
}

<<<<<<< HEAD
/*
 * Open the external journal device
 */
static struct block_device *ext4_blkdev_get(dev_t dev, struct super_block *sb)
{
	struct block_device *bdev;
	char b[BDEVNAME_SIZE];

	bdev = blkdev_get_by_dev(dev, FMODE_READ|FMODE_WRITE|FMODE_EXCL, sb);
	if (IS_ERR(bdev))
		goto fail;
	return bdev;

fail:
	ext4_msg(sb, KERN_ERR, "failed to open journal device %s: %ld",
			__bdevname(dev, b), PTR_ERR(bdev));
	return NULL;
}

/*
 * Release the journal device
 */
static int ext4_blkdev_put(struct block_device *bdev)
{
	return blkdev_put(bdev, FMODE_READ|FMODE_WRITE|FMODE_EXCL);
}

static int ext4_blkdev_remove(struct ext4_sb_info *sbi)
{
	struct block_device *bdev;
	int ret = -ENODEV;

	bdev = sbi->journal_bdev;
	if (bdev) {
		ret = ext4_blkdev_put(bdev);
		sbi->journal_bdev = NULL;
	}
	return ret;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct inode *orphan_list_entry(struct list_head *l)
{
	return &list_entry(l, struct ext4_inode_info, i_orphan)->vfs_inode;
}

static void dump_orphan_list(struct super_block *sb, struct ext4_sb_info *sbi)
{
	struct list_head *l;

	ext4_msg(sb, KERN_ERR, "sb orphan head is %d",
		 le32_to_cpu(sbi->s_es->s_last_orphan));

	printk(KERN_ERR "sb_info orphan list:\n");
	list_for_each(l, &sbi->s_orphan) {
		struct inode *inode = orphan_list_entry(l);
		printk(KERN_ERR "  "
		       "inode %s:%lu at %p: mode %o, nlink %d, next %d\n",
		       inode->i_sb->s_id, inode->i_ino, inode,
		       inode->i_mode, inode->i_nlink,
		       NEXT_ORPHAN(inode));
	}
}

<<<<<<< HEAD
static void ext4_put_super(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
	int i, err;

	ext4_unregister_li_request(sb);
	dquot_disable(sb, -1, DQUOT_USAGE_ENABLED | DQUOT_LIMITS_ENABLED);

	flush_workqueue(sbi->dio_unwritten_wq);
	destroy_workqueue(sbi->dio_unwritten_wq);

	lock_super(sb);
	if (sbi->s_journal) {
		err = jbd2_journal_destroy(sbi->s_journal);
		sbi->s_journal = NULL;
		if (err < 0)
			ext4_abort(sb, "Couldn't clean up the journal");
	}

	del_timer(&sbi->s_err_report);
	ext4_release_system_zone(sb);
	ext4_mb_release(sb);
	ext4_ext_release(sb);
	ext4_xattr_put_super(sb);

	if (!(sb->s_flags & MS_RDONLY)) {
		EXT4_CLEAR_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER);
		es->s_state = cpu_to_le16(sbi->s_mount_state);
	}
	if (sb->s_dirt || !(sb->s_flags & MS_RDONLY))
		ext4_commit_super(sb, 1);

	if (sbi->s_proc) {
		remove_proc_entry("options", sbi->s_proc);
		remove_proc_entry(sb->s_id, ext4_proc_root);
	}
	kobject_del(&sbi->s_kobj);

	for (i = 0; i < sbi->s_gdb_count; i++)
		brelse(sbi->s_group_desc[i]);
	ext4_kvfree(sbi->s_group_desc);
	ext4_kvfree(sbi->s_flex_groups);
=======
#ifdef CONFIG_QUOTA
static int ext4_quota_off(struct super_block *sb, int type);

static inline void ext4_quotas_off(struct super_block *sb, int type)
{
	BUG_ON(type > EXT4_MAXQUOTAS);

	/* Use our quota_off function to clear inode flags etc. */
	for (type--; type >= 0; type--)
		ext4_quota_off(sb, type);
}

/*
 * This is a helper function which is used in the mount/remount
 * codepaths (which holds s_umount) to fetch the quota file name.
 */
static inline char *get_qf_name(struct super_block *sb,
				struct ext4_sb_info *sbi,
				int type)
{
	return rcu_dereference_protected(sbi->s_qf_names[type],
					 lockdep_is_held(&sb->s_umount));
}
#else
static inline void ext4_quotas_off(struct super_block *sb, int type)
{
}
#endif

static int ext4_percpu_param_init(struct ext4_sb_info *sbi)
{
	ext4_fsblk_t block;
	int err;

	block = ext4_count_free_clusters(sbi->s_sb);
	ext4_free_blocks_count_set(sbi->s_es, EXT4_C2B(sbi, block));
	err = percpu_counter_init(&sbi->s_freeclusters_counter, block,
				  GFP_KERNEL);
	if (!err) {
		unsigned long freei = ext4_count_free_inodes(sbi->s_sb);
		sbi->s_es->s_free_inodes_count = cpu_to_le32(freei);
		err = percpu_counter_init(&sbi->s_freeinodes_counter, freei,
					  GFP_KERNEL);
	}
	if (!err)
		err = percpu_counter_init(&sbi->s_dirs_counter,
					  ext4_count_dirs(sbi->s_sb), GFP_KERNEL);
	if (!err)
		err = percpu_counter_init(&sbi->s_dirtyclusters_counter, 0,
					  GFP_KERNEL);
	if (!err)
		err = percpu_counter_init(&sbi->s_sra_exceeded_retry_limit, 0,
					  GFP_KERNEL);
	if (!err)
		err = percpu_init_rwsem(&sbi->s_writepages_rwsem);

	if (err)
		ext4_msg(sbi->s_sb, KERN_ERR, "insufficient memory");

	return err;
}

static void ext4_percpu_param_destroy(struct ext4_sb_info *sbi)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	percpu_counter_destroy(&sbi->s_freeclusters_counter);
	percpu_counter_destroy(&sbi->s_freeinodes_counter);
	percpu_counter_destroy(&sbi->s_dirs_counter);
	percpu_counter_destroy(&sbi->s_dirtyclusters_counter);
<<<<<<< HEAD
	brelse(sbi->s_sbh);
#ifdef CONFIG_QUOTA
	for (i = 0; i < MAXQUOTAS; i++)
		kfree(sbi->s_qf_names[i]);
=======
	percpu_counter_destroy(&sbi->s_sra_exceeded_retry_limit);
	percpu_free_rwsem(&sbi->s_writepages_rwsem);
}

static void ext4_group_desc_free(struct ext4_sb_info *sbi)
{
	struct buffer_head **group_desc;
	int i;

	rcu_read_lock();
	group_desc = rcu_dereference(sbi->s_group_desc);
	for (i = 0; i < sbi->s_gdb_count; i++)
		brelse(group_desc[i]);
	kvfree(group_desc);
	rcu_read_unlock();
}

static void ext4_flex_groups_free(struct ext4_sb_info *sbi)
{
	struct flex_groups **flex_groups;
	int i;

	rcu_read_lock();
	flex_groups = rcu_dereference(sbi->s_flex_groups);
	if (flex_groups) {
		for (i = 0; i < sbi->s_flex_groups_allocated; i++)
			kvfree(flex_groups[i]);
		kvfree(flex_groups);
	}
	rcu_read_unlock();
}

static void ext4_put_super(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
	int aborted = 0;
	int err;

	/*
	 * Unregister sysfs before destroying jbd2 journal.
	 * Since we could still access attr_journal_task attribute via sysfs
	 * path which could have sbi->s_journal->j_task as NULL
	 * Unregister sysfs before flush sbi->s_sb_upd_work.
	 * Since user may read /proc/fs/ext4/xx/mb_groups during umount, If
	 * read metadata verify failed then will queue error work.
	 * update_super_work will call start_this_handle may trigger
	 * BUG_ON.
	 */
	ext4_unregister_sysfs(sb);

	if (___ratelimit(&ext4_mount_msg_ratelimit, "EXT4-fs unmount"))
		ext4_msg(sb, KERN_INFO, "unmounting filesystem %pU.",
			 &sb->s_uuid);

	ext4_unregister_li_request(sb);
	ext4_quotas_off(sb, EXT4_MAXQUOTAS);

	flush_work(&sbi->s_sb_upd_work);
	destroy_workqueue(sbi->rsv_conversion_wq);
	ext4_release_orphan_info(sb);

	if (sbi->s_journal) {
		aborted = is_journal_aborted(sbi->s_journal);
		err = jbd2_journal_destroy(sbi->s_journal);
		sbi->s_journal = NULL;
		if ((err < 0) && !aborted) {
			ext4_abort(sb, -err, "Couldn't clean up the journal");
		}
	}

	ext4_es_unregister_shrinker(sbi);
	timer_shutdown_sync(&sbi->s_err_report);
	ext4_release_system_zone(sb);
	ext4_mb_release(sb);
	ext4_ext_release(sb);

	if (!sb_rdonly(sb) && !aborted) {
		ext4_clear_feature_journal_needs_recovery(sb);
		ext4_clear_feature_orphan_present(sb);
		es->s_state = cpu_to_le16(sbi->s_mount_state);
	}
	if (!sb_rdonly(sb))
		ext4_commit_super(sb);

	ext4_group_desc_free(sbi);
	ext4_flex_groups_free(sbi);
	ext4_percpu_param_destroy(sbi);
#ifdef CONFIG_QUOTA
	for (int i = 0; i < EXT4_MAXQUOTAS; i++)
		kfree(get_qf_name(sb, sbi, i));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	/* Debugging code just in case the in-memory inode orphan list
	 * isn't empty.  The on-disk one can be non-empty if we've
	 * detected an error and taken the fs readonly, but the
	 * in-memory list had better be clean by this point. */
	if (!list_empty(&sbi->s_orphan))
		dump_orphan_list(sb, sbi);
<<<<<<< HEAD
	J_ASSERT(list_empty(&sbi->s_orphan));

	sync_blockdev(sb->s_bdev);
	invalidate_bdev(sb->s_bdev);
	if (sbi->journal_bdev && sbi->journal_bdev != sb->s_bdev) {
=======
	ASSERT(list_empty(&sbi->s_orphan));

	sync_blockdev(sb->s_bdev);
	invalidate_bdev(sb->s_bdev);
	if (sbi->s_journal_bdev_file) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Invalidate the journal device's buffers.  We don't want them
		 * floating about in memory - the physical journal device may
		 * hotswapped, and it breaks the `ro-after' testing code.
		 */
<<<<<<< HEAD
		sync_blockdev(sbi->journal_bdev);
		invalidate_bdev(sbi->journal_bdev);
		ext4_blkdev_remove(sbi);
	}
	if (sbi->s_mmp_tsk)
		kthread_stop(sbi->s_mmp_tsk);
=======
		sync_blockdev(file_bdev(sbi->s_journal_bdev_file));
		invalidate_bdev(file_bdev(sbi->s_journal_bdev_file));
	}

	ext4_xattr_destroy_cache(sbi->s_ea_inode_cache);
	sbi->s_ea_inode_cache = NULL;

	ext4_xattr_destroy_cache(sbi->s_ea_block_cache);
	sbi->s_ea_block_cache = NULL;

	ext4_stop_mmpd(sbi);

	brelse(sbi->s_sbh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sb->s_fs_info = NULL;
	/*
	 * Now that we are completely done shutting down the
	 * superblock, we need to actually destroy the kobject.
	 */
<<<<<<< HEAD
	unlock_super(sb);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kobject_put(&sbi->s_kobj);
	wait_for_completion(&sbi->s_kobj_unregister);
	if (sbi->s_chksum_driver)
		crypto_free_shash(sbi->s_chksum_driver);
	kfree(sbi->s_blockgroup_lock);
<<<<<<< HEAD
=======
	fs_put_dax(sbi->s_daxdev, NULL);
	fscrypt_free_dummy_policy(&sbi->s_dummy_enc_policy);
#if IS_ENABLED(CONFIG_UNICODE)
	utf8_unload(sb->s_encoding);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(sbi);
}

static struct kmem_cache *ext4_inode_cachep;

/*
 * Called inside transaction, so use GFP_NOFS
 */
static struct inode *ext4_alloc_inode(struct super_block *sb)
{
	struct ext4_inode_info *ei;

<<<<<<< HEAD
	ei = kmem_cache_alloc(ext4_inode_cachep, GFP_NOFS);
	if (!ei)
		return NULL;

	ei->vfs_inode.i_version = 1;
	ei->vfs_inode.i_data.writeback_index = 0;
	memset(&ei->i_cached_extent, 0, sizeof(struct ext4_ext_cache));
	INIT_LIST_HEAD(&ei->i_prealloc_list);
	spin_lock_init(&ei->i_prealloc_lock);
	ei->i_reserved_data_blocks = 0;
	ei->i_reserved_meta_blocks = 0;
	ei->i_allocated_meta_blocks = 0;
	ei->i_da_metadata_calc_len = 0;
	ei->i_da_metadata_calc_last_lblock = 0;
	spin_lock_init(&(ei->i_block_reservation_lock));
#ifdef CONFIG_QUOTA
	ei->i_reserved_quota = 0;
#endif
	ei->jinode = NULL;
	INIT_LIST_HEAD(&ei->i_completed_io_list);
	spin_lock_init(&ei->i_completed_io_lock);
	ei->cur_aio_dio = NULL;
	ei->i_sync_tid = 0;
	ei->i_datasync_tid = 0;
	atomic_set(&ei->i_ioend_count, 0);
	atomic_set(&ei->i_aiodio_unwritten, 0);

=======
	ei = alloc_inode_sb(sb, ext4_inode_cachep, GFP_NOFS);
	if (!ei)
		return NULL;

	inode_set_iversion(&ei->vfs_inode, 1);
	ei->i_flags = 0;
	spin_lock_init(&ei->i_raw_lock);
	ei->i_prealloc_node = RB_ROOT;
	atomic_set(&ei->i_prealloc_active, 0);
	rwlock_init(&ei->i_prealloc_lock);
	ext4_es_init_tree(&ei->i_es_tree);
	rwlock_init(&ei->i_es_lock);
	INIT_LIST_HEAD(&ei->i_es_list);
	ei->i_es_all_nr = 0;
	ei->i_es_shk_nr = 0;
	ei->i_es_shrink_lblk = 0;
	ei->i_reserved_data_blocks = 0;
	spin_lock_init(&(ei->i_block_reservation_lock));
	ext4_init_pending_tree(&ei->i_pending_tree);
#ifdef CONFIG_QUOTA
	ei->i_reserved_quota = 0;
	memset(&ei->i_dquot, 0, sizeof(ei->i_dquot));
#endif
	ei->jinode = NULL;
	INIT_LIST_HEAD(&ei->i_rsv_conversion_list);
	spin_lock_init(&ei->i_completed_io_lock);
	ei->i_sync_tid = 0;
	ei->i_datasync_tid = 0;
	atomic_set(&ei->i_unwritten, 0);
	INIT_WORK(&ei->i_rsv_conversion_work, ext4_end_io_rsv_work);
	ext4_fc_init_inode(&ei->vfs_inode);
	mutex_init(&ei->i_fc_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return &ei->vfs_inode;
}

static int ext4_drop_inode(struct inode *inode)
{
	int drop = generic_drop_inode(inode);

<<<<<<< HEAD
=======
	if (!drop)
		drop = fscrypt_drop_inode(inode);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	trace_ext4_drop_inode(inode, drop);
	return drop;
}

<<<<<<< HEAD
static void ext4_i_callback(struct rcu_head *head)
{
	struct inode *inode = container_of(head, struct inode, i_rcu);
=======
static void ext4_free_in_core_inode(struct inode *inode)
{
	fscrypt_free_inode(inode);
	if (!list_empty(&(EXT4_I(inode)->i_fc_list))) {
		pr_warn("%s: inode %ld still in fc list",
			__func__, inode->i_ino);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kmem_cache_free(ext4_inode_cachep, EXT4_I(inode));
}

static void ext4_destroy_inode(struct inode *inode)
{
	if (!list_empty(&(EXT4_I(inode)->i_orphan))) {
		ext4_msg(inode->i_sb, KERN_ERR,
			 "Inode %lu (%p): orphan list check failed!",
			 inode->i_ino, EXT4_I(inode));
		print_hex_dump(KERN_INFO, "", DUMP_PREFIX_ADDRESS, 16, 4,
				EXT4_I(inode), sizeof(struct ext4_inode_info),
				true);
		dump_stack();
	}
<<<<<<< HEAD
	call_rcu(&inode->i_rcu, ext4_i_callback);
=======

	if (EXT4_I(inode)->i_reserved_data_blocks)
		ext4_msg(inode->i_sb, KERN_ERR,
			 "Inode %lu (%p): i_reserved_data_blocks (%u) not cleared!",
			 inode->i_ino, EXT4_I(inode),
			 EXT4_I(inode)->i_reserved_data_blocks);
}

static void ext4_shutdown(struct super_block *sb)
{
       ext4_force_shutdown(sb, EXT4_GOING_FLAGS_NOLOGFLUSH);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void init_once(void *foo)
{
<<<<<<< HEAD
	struct ext4_inode_info *ei = (struct ext4_inode_info *) foo;

	INIT_LIST_HEAD(&ei->i_orphan);
#ifdef CONFIG_EXT4_FS_XATTR
	init_rwsem(&ei->xattr_sem);
#endif
	init_rwsem(&ei->i_data_sem);
	inode_init_once(&ei->vfs_inode);
}

static int init_inodecache(void)
{
	ext4_inode_cachep = kmem_cache_create("ext4_inode_cache",
					     sizeof(struct ext4_inode_info),
					     0, (SLAB_RECLAIM_ACCOUNT|
						SLAB_MEM_SPREAD),
					     init_once);
=======
	struct ext4_inode_info *ei = foo;

	INIT_LIST_HEAD(&ei->i_orphan);
	init_rwsem(&ei->xattr_sem);
	init_rwsem(&ei->i_data_sem);
	inode_init_once(&ei->vfs_inode);
	ext4_fc_init_inode(&ei->vfs_inode);
}

static int __init init_inodecache(void)
{
	ext4_inode_cachep = kmem_cache_create_usercopy("ext4_inode_cache",
				sizeof(struct ext4_inode_info), 0,
				SLAB_RECLAIM_ACCOUNT | SLAB_ACCOUNT,
				offsetof(struct ext4_inode_info, i_data),
				sizeof_field(struct ext4_inode_info, i_data),
				init_once);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ext4_inode_cachep == NULL)
		return -ENOMEM;
	return 0;
}

static void destroy_inodecache(void)
{
	/*
	 * Make sure all delayed rcu free inodes are flushed before we
	 * destroy cache.
	 */
	rcu_barrier();
	kmem_cache_destroy(ext4_inode_cachep);
}

void ext4_clear_inode(struct inode *inode)
{
<<<<<<< HEAD
	invalidate_inode_buffers(inode);
	end_writeback(inode);
	dquot_drop(inode);
	ext4_discard_preallocations(inode);
=======
	ext4_fc_del(inode);
	invalidate_inode_buffers(inode);
	clear_inode(inode);
	ext4_discard_preallocations(inode);
	ext4_es_remove_extent(inode, 0, EXT_MAX_BLOCKS);
	dquot_drop(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (EXT4_I(inode)->jinode) {
		jbd2_journal_release_jbd_inode(EXT4_JOURNAL(inode),
					       EXT4_I(inode)->jinode);
		jbd2_free_inode(EXT4_I(inode)->jinode);
		EXT4_I(inode)->jinode = NULL;
	}
<<<<<<< HEAD
=======
	fscrypt_put_encryption_info(inode);
	fsverity_cleanup_inode(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct inode *ext4_nfs_get_inode(struct super_block *sb,
					u64 ino, u32 generation)
{
	struct inode *inode;

<<<<<<< HEAD
	if (ino < EXT4_FIRST_INO(sb) && ino != EXT4_ROOT_INO)
		return ERR_PTR(-ESTALE);
	if (ino > le32_to_cpu(EXT4_SB(sb)->s_es->s_inodes_count))
		return ERR_PTR(-ESTALE);

	/* iget isn't really right if the inode is currently unallocated!!
	 *
	 * ext4_read_inode will return a bad_inode if the inode had been
	 * deleted, so we should be safe.
	 *
	 * Currently we don't know the generation for parent directory, so
	 * a generation of 0 means "accept any"
	 */
	inode = ext4_iget_normal(sb, ino);
=======
	/*
	 * Currently we don't know the generation for parent directory, so
	 * a generation of 0 means "accept any"
	 */
	inode = ext4_iget(sb, ino, EXT4_IGET_HANDLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(inode))
		return ERR_CAST(inode);
	if (generation && inode->i_generation != generation) {
		iput(inode);
		return ERR_PTR(-ESTALE);
	}

	return inode;
}

static struct dentry *ext4_fh_to_dentry(struct super_block *sb, struct fid *fid,
					int fh_len, int fh_type)
{
	return generic_fh_to_dentry(sb, fid, fh_len, fh_type,
				    ext4_nfs_get_inode);
}

static struct dentry *ext4_fh_to_parent(struct super_block *sb, struct fid *fid,
					int fh_len, int fh_type)
{
	return generic_fh_to_parent(sb, fid, fh_len, fh_type,
				    ext4_nfs_get_inode);
}

<<<<<<< HEAD
/*
 * Try to release metadata pages (indirect blocks, directories) which are
 * mapped via the block device.  Since these pages could have journal heads
 * which would prevent try_to_free_buffers() from freeing them, we must use
 * jbd2 layer's try_to_free_buffers() function to release them.
 */
static int bdev_try_to_free_page(struct super_block *sb, struct page *page,
				 gfp_t wait)
{
	journal_t *journal = EXT4_SB(sb)->s_journal;

	WARN_ON(PageChecked(page));
	if (!page_has_buffers(page))
		return 0;
	if (journal)
		return jbd2_journal_try_to_free_buffers(journal, page,
							wait & ~__GFP_WAIT);
	return try_to_free_buffers(page);
}

#ifdef CONFIG_QUOTA
#define QTYPE2NAME(t) ((t) == USRQUOTA ? "user" : "group")
#define QTYPE2MOPT(on, t) ((t) == USRQUOTA?((on)##USRJQUOTA):((on)##GRPJQUOTA))
=======
static int ext4_nfs_commit_metadata(struct inode *inode)
{
	struct writeback_control wbc = {
		.sync_mode = WB_SYNC_ALL
	};

	trace_ext4_nfs_commit_metadata(inode);
	return ext4_write_inode(inode, &wbc);
}

#ifdef CONFIG_QUOTA
static const char * const quotatypes[] = INITQFNAMES;
#define QTYPE2NAME(t) (quotatypes[t])
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int ext4_write_dquot(struct dquot *dquot);
static int ext4_acquire_dquot(struct dquot *dquot);
static int ext4_release_dquot(struct dquot *dquot);
static int ext4_mark_dquot_dirty(struct dquot *dquot);
static int ext4_write_info(struct super_block *sb, int type);
static int ext4_quota_on(struct super_block *sb, int type, int format_id,
<<<<<<< HEAD
			 struct path *path);
static int ext4_quota_off(struct super_block *sb, int type);
static int ext4_quota_on_mount(struct super_block *sb, int type);
=======
			 const struct path *path);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t ext4_quota_read(struct super_block *sb, int type, char *data,
			       size_t len, loff_t off);
static ssize_t ext4_quota_write(struct super_block *sb, int type,
				const char *data, size_t len, loff_t off);
<<<<<<< HEAD

static const struct dquot_operations ext4_quota_operations = {
	.get_reserved_space = ext4_get_reserved_space,
	.write_dquot	= ext4_write_dquot,
	.acquire_dquot	= ext4_acquire_dquot,
	.release_dquot	= ext4_release_dquot,
	.mark_dirty	= ext4_mark_dquot_dirty,
	.write_info	= ext4_write_info,
	.alloc_dquot	= dquot_alloc,
	.destroy_dquot	= dquot_destroy,
=======
static int ext4_quota_enable(struct super_block *sb, int type, int format_id,
			     unsigned int flags);

static struct dquot __rcu **ext4_get_dquots(struct inode *inode)
{
	return EXT4_I(inode)->i_dquot;
}

static const struct dquot_operations ext4_quota_operations = {
	.get_reserved_space	= ext4_get_reserved_space,
	.write_dquot		= ext4_write_dquot,
	.acquire_dquot		= ext4_acquire_dquot,
	.release_dquot		= ext4_release_dquot,
	.mark_dirty		= ext4_mark_dquot_dirty,
	.write_info		= ext4_write_info,
	.alloc_dquot		= dquot_alloc,
	.destroy_dquot		= dquot_destroy,
	.get_projid		= ext4_get_projid,
	.get_inode_usage	= ext4_get_inode_usage,
	.get_next_id		= dquot_get_next_id,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct quotactl_ops ext4_qctl_operations = {
	.quota_on	= ext4_quota_on,
	.quota_off	= ext4_quota_off,
	.quota_sync	= dquot_quota_sync,
<<<<<<< HEAD
	.get_info	= dquot_get_dqinfo,
	.set_info	= dquot_set_dqinfo,
	.get_dqblk	= dquot_get_dqblk,
	.set_dqblk	= dquot_set_dqblk
=======
	.get_state	= dquot_get_state,
	.set_info	= dquot_set_dqinfo,
	.get_dqblk	= dquot_get_dqblk,
	.set_dqblk	= dquot_set_dqblk,
	.get_nextdqblk	= dquot_get_next_dqblk,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
#endif

static const struct super_operations ext4_sops = {
	.alloc_inode	= ext4_alloc_inode,
<<<<<<< HEAD
=======
	.free_inode	= ext4_free_in_core_inode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.destroy_inode	= ext4_destroy_inode,
	.write_inode	= ext4_write_inode,
	.dirty_inode	= ext4_dirty_inode,
	.drop_inode	= ext4_drop_inode,
	.evict_inode	= ext4_evict_inode,
	.put_super	= ext4_put_super,
	.sync_fs	= ext4_sync_fs,
	.freeze_fs	= ext4_freeze,
	.unfreeze_fs	= ext4_unfreeze,
	.statfs		= ext4_statfs,
<<<<<<< HEAD
	.remount_fs	= ext4_remount,
	.show_options	= ext4_show_options,
#ifdef CONFIG_QUOTA
	.quota_read	= ext4_quota_read,
	.quota_write	= ext4_quota_write,
#endif
	.bdev_try_to_free_page = bdev_try_to_free_page,
};

static const struct super_operations ext4_nojournal_sops = {
	.alloc_inode	= ext4_alloc_inode,
	.destroy_inode	= ext4_destroy_inode,
	.write_inode	= ext4_write_inode,
	.dirty_inode	= ext4_dirty_inode,
	.drop_inode	= ext4_drop_inode,
	.evict_inode	= ext4_evict_inode,
	.write_super	= ext4_write_super,
	.put_super	= ext4_put_super,
	.statfs		= ext4_statfs,
	.remount_fs	= ext4_remount,
	.show_options	= ext4_show_options,
#ifdef CONFIG_QUOTA
	.quota_read	= ext4_quota_read,
	.quota_write	= ext4_quota_write,
#endif
	.bdev_try_to_free_page = bdev_try_to_free_page,
};

static const struct export_operations ext4_export_ops = {
	.fh_to_dentry = ext4_fh_to_dentry,
	.fh_to_parent = ext4_fh_to_parent,
	.get_parent = ext4_get_parent,
=======
	.show_options	= ext4_show_options,
	.shutdown	= ext4_shutdown,
#ifdef CONFIG_QUOTA
	.quota_read	= ext4_quota_read,
	.quota_write	= ext4_quota_write,
	.get_dquots	= ext4_get_dquots,
#endif
};

static const struct export_operations ext4_export_ops = {
	.encode_fh = generic_encode_ino32_fh,
	.fh_to_dentry = ext4_fh_to_dentry,
	.fh_to_parent = ext4_fh_to_parent,
	.get_parent = ext4_get_parent,
	.commit_metadata = ext4_nfs_commit_metadata,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

enum {
	Opt_bsd_df, Opt_minix_df, Opt_grpid, Opt_nogrpid,
<<<<<<< HEAD
	Opt_resgid, Opt_resuid, Opt_sb, Opt_err_cont, Opt_err_panic, Opt_err_ro,
	Opt_nouid32, Opt_debug, Opt_removed,
	Opt_user_xattr, Opt_nouser_xattr, Opt_acl, Opt_noacl,
	Opt_auto_da_alloc, Opt_noauto_da_alloc, Opt_noload,
	Opt_commit, Opt_min_batch_time, Opt_max_batch_time,
	Opt_journal_dev, Opt_journal_checksum, Opt_journal_async_commit,
	Opt_abort, Opt_data_journal, Opt_data_ordered, Opt_data_writeback,
	Opt_data_err_abort, Opt_data_err_ignore,
	Opt_usrjquota, Opt_grpjquota, Opt_offusrjquota, Opt_offgrpjquota,
	Opt_jqfmt_vfsold, Opt_jqfmt_vfsv0, Opt_jqfmt_vfsv1, Opt_quota,
	Opt_noquota, Opt_barrier, Opt_nobarrier, Opt_err,
	Opt_usrquota, Opt_grpquota, Opt_i_version,
	Opt_stripe, Opt_delalloc, Opt_nodelalloc, Opt_mblk_io_submit,
=======
	Opt_resgid, Opt_resuid, Opt_sb,
	Opt_nouid32, Opt_debug, Opt_removed,
	Opt_user_xattr, Opt_acl,
	Opt_auto_da_alloc, Opt_noauto_da_alloc, Opt_noload,
	Opt_commit, Opt_min_batch_time, Opt_max_batch_time, Opt_journal_dev,
	Opt_journal_path, Opt_journal_checksum, Opt_journal_async_commit,
	Opt_abort, Opt_data_journal, Opt_data_ordered, Opt_data_writeback,
	Opt_data_err_abort, Opt_data_err_ignore, Opt_test_dummy_encryption,
	Opt_inlinecrypt,
	Opt_usrjquota, Opt_grpjquota, Opt_quota,
	Opt_noquota, Opt_barrier, Opt_nobarrier, Opt_err,
	Opt_usrquota, Opt_grpquota, Opt_prjquota,
	Opt_dax, Opt_dax_always, Opt_dax_inode, Opt_dax_never,
	Opt_stripe, Opt_delalloc, Opt_nodelalloc, Opt_warn_on_error,
	Opt_nowarn_on_error, Opt_mblk_io_submit, Opt_debug_want_extra_isize,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	Opt_nomblk_io_submit, Opt_block_validity, Opt_noblock_validity,
	Opt_inode_readahead_blks, Opt_journal_ioprio,
	Opt_dioread_nolock, Opt_dioread_lock,
	Opt_discard, Opt_nodiscard, Opt_init_itable, Opt_noinit_itable,
<<<<<<< HEAD
};

static const match_table_t tokens = {
	{Opt_bsd_df, "bsddf"},
	{Opt_minix_df, "minixdf"},
	{Opt_grpid, "grpid"},
	{Opt_grpid, "bsdgroups"},
	{Opt_nogrpid, "nogrpid"},
	{Opt_nogrpid, "sysvgroups"},
	{Opt_resgid, "resgid=%u"},
	{Opt_resuid, "resuid=%u"},
	{Opt_sb, "sb=%u"},
	{Opt_err_cont, "errors=continue"},
	{Opt_err_panic, "errors=panic"},
	{Opt_err_ro, "errors=remount-ro"},
	{Opt_nouid32, "nouid32"},
	{Opt_debug, "debug"},
	{Opt_removed, "oldalloc"},
	{Opt_removed, "orlov"},
	{Opt_user_xattr, "user_xattr"},
	{Opt_nouser_xattr, "nouser_xattr"},
	{Opt_acl, "acl"},
	{Opt_noacl, "noacl"},
	{Opt_noload, "norecovery"},
	{Opt_noload, "noload"},
	{Opt_removed, "nobh"},
	{Opt_removed, "bh"},
	{Opt_commit, "commit=%u"},
	{Opt_min_batch_time, "min_batch_time=%u"},
	{Opt_max_batch_time, "max_batch_time=%u"},
	{Opt_journal_dev, "journal_dev=%u"},
	{Opt_journal_checksum, "journal_checksum"},
	{Opt_journal_async_commit, "journal_async_commit"},
	{Opt_abort, "abort"},
	{Opt_data_journal, "data=journal"},
	{Opt_data_ordered, "data=ordered"},
	{Opt_data_writeback, "data=writeback"},
	{Opt_data_err_abort, "data_err=abort"},
	{Opt_data_err_ignore, "data_err=ignore"},
	{Opt_offusrjquota, "usrjquota="},
	{Opt_usrjquota, "usrjquota=%s"},
	{Opt_offgrpjquota, "grpjquota="},
	{Opt_grpjquota, "grpjquota=%s"},
	{Opt_jqfmt_vfsold, "jqfmt=vfsold"},
	{Opt_jqfmt_vfsv0, "jqfmt=vfsv0"},
	{Opt_jqfmt_vfsv1, "jqfmt=vfsv1"},
	{Opt_grpquota, "grpquota"},
	{Opt_noquota, "noquota"},
	{Opt_quota, "quota"},
	{Opt_usrquota, "usrquota"},
	{Opt_barrier, "barrier=%u"},
	{Opt_barrier, "barrier"},
	{Opt_nobarrier, "nobarrier"},
	{Opt_i_version, "i_version"},
	{Opt_stripe, "stripe=%u"},
	{Opt_delalloc, "delalloc"},
	{Opt_nodelalloc, "nodelalloc"},
	{Opt_mblk_io_submit, "mblk_io_submit"},
	{Opt_nomblk_io_submit, "nomblk_io_submit"},
	{Opt_block_validity, "block_validity"},
	{Opt_noblock_validity, "noblock_validity"},
	{Opt_inode_readahead_blks, "inode_readahead_blks=%u"},
	{Opt_journal_ioprio, "journal_ioprio=%u"},
	{Opt_auto_da_alloc, "auto_da_alloc=%u"},
	{Opt_auto_da_alloc, "auto_da_alloc"},
	{Opt_noauto_da_alloc, "noauto_da_alloc"},
	{Opt_dioread_nolock, "dioread_nolock"},
	{Opt_dioread_lock, "dioread_lock"},
	{Opt_discard, "discard"},
	{Opt_nodiscard, "nodiscard"},
	{Opt_init_itable, "init_itable=%u"},
	{Opt_init_itable, "init_itable"},
	{Opt_noinit_itable, "noinit_itable"},
	{Opt_removed, "check=none"},	/* mount option from ext2/3 */
	{Opt_removed, "nocheck"},	/* mount option from ext2/3 */
	{Opt_removed, "reservation"},	/* mount option from ext2/3 */
	{Opt_removed, "noreservation"}, /* mount option from ext2/3 */
	{Opt_removed, "journal=%u"},	/* mount option from ext2/3 */
	{Opt_err, NULL},
};

static ext4_fsblk_t get_sb_block(void **data)
{
	ext4_fsblk_t	sb_block;
	char		*options = (char *) *data;

	if (!options || strncmp(options, "sb=", 3) != 0)
		return 1;	/* Default location */

	options += 3;
	/* TODO: use simple_strtoll with >32bit ext4 */
	sb_block = simple_strtoul(options, &options, 0);
	if (*options && *options != ',') {
		printk(KERN_ERR "EXT4-fs: Invalid sb specification: %s\n",
		       (char *) *data);
		return 1;
	}
	if (*options == ',')
		options++;
	*data = (void *) options;

	return sb_block;
}

#define DEFAULT_JOURNAL_IOPRIO (IOPRIO_PRIO_VALUE(IOPRIO_CLASS_BE, 3))
static char deprecated_msg[] = "Mount option \"%s\" will be removed by %s\n"
	"Contact linux-ext4@vger.kernel.org if you think we should keep it.\n";

#ifdef CONFIG_QUOTA
static int set_qf_name(struct super_block *sb, int qtype, substring_t *args)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	char *qname;

	if (sb_any_quota_loaded(sb) &&
		!sbi->s_qf_names[qtype]) {
		ext4_msg(sb, KERN_ERR,
			"Cannot change journaled "
			"quota options when quota turned on");
		return -1;
	}
	qname = match_strdup(args);
	if (!qname) {
		ext4_msg(sb, KERN_ERR,
			"Not enough memory for storing quotafile name");
		return -1;
	}
	if (sbi->s_qf_names[qtype] &&
		strcmp(sbi->s_qf_names[qtype], qname)) {
		ext4_msg(sb, KERN_ERR,
			"%s quota file already specified", QTYPE2NAME(qtype));
		kfree(qname);
		return -1;
	}
	sbi->s_qf_names[qtype] = qname;
	if (strchr(sbi->s_qf_names[qtype], '/')) {
		ext4_msg(sb, KERN_ERR,
			"quotafile must be on filesystem root");
		kfree(sbi->s_qf_names[qtype]);
		sbi->s_qf_names[qtype] = NULL;
		return -1;
	}
	set_opt(sb, QUOTA);
	return 1;
}

static int clear_qf_name(struct super_block *sb, int qtype)
{

	struct ext4_sb_info *sbi = EXT4_SB(sb);

	if (sb_any_quota_loaded(sb) &&
		sbi->s_qf_names[qtype]) {
		ext4_msg(sb, KERN_ERR, "Cannot change journaled quota options"
			" when quota turned on");
		return -1;
	}
	/*
	 * The space will be released later when all options are confirmed
	 * to be correct
	 */
	sbi->s_qf_names[qtype] = NULL;
	return 1;
}
#endif
=======
	Opt_max_dir_size_kb, Opt_nojournal_checksum, Opt_nombcache,
	Opt_no_prefetch_block_bitmaps, Opt_mb_optimize_scan,
	Opt_errors, Opt_data, Opt_data_err, Opt_jqfmt, Opt_dax_type,
#ifdef CONFIG_EXT4_DEBUG
	Opt_fc_debug_max_replay, Opt_fc_debug_force
#endif
};

static const struct constant_table ext4_param_errors[] = {
	{"continue",	EXT4_MOUNT_ERRORS_CONT},
	{"panic",	EXT4_MOUNT_ERRORS_PANIC},
	{"remount-ro",	EXT4_MOUNT_ERRORS_RO},
	{}
};

static const struct constant_table ext4_param_data[] = {
	{"journal",	EXT4_MOUNT_JOURNAL_DATA},
	{"ordered",	EXT4_MOUNT_ORDERED_DATA},
	{"writeback",	EXT4_MOUNT_WRITEBACK_DATA},
	{}
};

static const struct constant_table ext4_param_data_err[] = {
	{"abort",	Opt_data_err_abort},
	{"ignore",	Opt_data_err_ignore},
	{}
};

static const struct constant_table ext4_param_jqfmt[] = {
	{"vfsold",	QFMT_VFS_OLD},
	{"vfsv0",	QFMT_VFS_V0},
	{"vfsv1",	QFMT_VFS_V1},
	{}
};

static const struct constant_table ext4_param_dax[] = {
	{"always",	Opt_dax_always},
	{"inode",	Opt_dax_inode},
	{"never",	Opt_dax_never},
	{}
};

/* String parameter that allows empty argument */
#define fsparam_string_empty(NAME, OPT) \
	__fsparam(fs_param_is_string, NAME, OPT, fs_param_can_be_empty, NULL)

/*
 * Mount option specification
 * We don't use fsparam_flag_no because of the way we set the
 * options and the way we show them in _ext4_show_options(). To
 * keep the changes to a minimum, let's keep the negative options
 * separate for now.
 */
static const struct fs_parameter_spec ext4_param_specs[] = {
	fsparam_flag	("bsddf",		Opt_bsd_df),
	fsparam_flag	("minixdf",		Opt_minix_df),
	fsparam_flag	("grpid",		Opt_grpid),
	fsparam_flag	("bsdgroups",		Opt_grpid),
	fsparam_flag	("nogrpid",		Opt_nogrpid),
	fsparam_flag	("sysvgroups",		Opt_nogrpid),
	fsparam_u32	("resgid",		Opt_resgid),
	fsparam_u32	("resuid",		Opt_resuid),
	fsparam_u32	("sb",			Opt_sb),
	fsparam_enum	("errors",		Opt_errors, ext4_param_errors),
	fsparam_flag	("nouid32",		Opt_nouid32),
	fsparam_flag	("debug",		Opt_debug),
	fsparam_flag	("oldalloc",		Opt_removed),
	fsparam_flag	("orlov",		Opt_removed),
	fsparam_flag	("user_xattr",		Opt_user_xattr),
	fsparam_flag	("acl",			Opt_acl),
	fsparam_flag	("norecovery",		Opt_noload),
	fsparam_flag	("noload",		Opt_noload),
	fsparam_flag	("bh",			Opt_removed),
	fsparam_flag	("nobh",		Opt_removed),
	fsparam_u32	("commit",		Opt_commit),
	fsparam_u32	("min_batch_time",	Opt_min_batch_time),
	fsparam_u32	("max_batch_time",	Opt_max_batch_time),
	fsparam_u32	("journal_dev",		Opt_journal_dev),
	fsparam_bdev	("journal_path",	Opt_journal_path),
	fsparam_flag	("journal_checksum",	Opt_journal_checksum),
	fsparam_flag	("nojournal_checksum",	Opt_nojournal_checksum),
	fsparam_flag	("journal_async_commit",Opt_journal_async_commit),
	fsparam_flag	("abort",		Opt_abort),
	fsparam_enum	("data",		Opt_data, ext4_param_data),
	fsparam_enum	("data_err",		Opt_data_err,
						ext4_param_data_err),
	fsparam_string_empty
			("usrjquota",		Opt_usrjquota),
	fsparam_string_empty
			("grpjquota",		Opt_grpjquota),
	fsparam_enum	("jqfmt",		Opt_jqfmt, ext4_param_jqfmt),
	fsparam_flag	("grpquota",		Opt_grpquota),
	fsparam_flag	("quota",		Opt_quota),
	fsparam_flag	("noquota",		Opt_noquota),
	fsparam_flag	("usrquota",		Opt_usrquota),
	fsparam_flag	("prjquota",		Opt_prjquota),
	fsparam_flag	("barrier",		Opt_barrier),
	fsparam_u32	("barrier",		Opt_barrier),
	fsparam_flag	("nobarrier",		Opt_nobarrier),
	fsparam_flag	("i_version",		Opt_removed),
	fsparam_flag	("dax",			Opt_dax),
	fsparam_enum	("dax",			Opt_dax_type, ext4_param_dax),
	fsparam_u32	("stripe",		Opt_stripe),
	fsparam_flag	("delalloc",		Opt_delalloc),
	fsparam_flag	("nodelalloc",		Opt_nodelalloc),
	fsparam_flag	("warn_on_error",	Opt_warn_on_error),
	fsparam_flag	("nowarn_on_error",	Opt_nowarn_on_error),
	fsparam_u32	("debug_want_extra_isize",
						Opt_debug_want_extra_isize),
	fsparam_flag	("mblk_io_submit",	Opt_removed),
	fsparam_flag	("nomblk_io_submit",	Opt_removed),
	fsparam_flag	("block_validity",	Opt_block_validity),
	fsparam_flag	("noblock_validity",	Opt_noblock_validity),
	fsparam_u32	("inode_readahead_blks",
						Opt_inode_readahead_blks),
	fsparam_u32	("journal_ioprio",	Opt_journal_ioprio),
	fsparam_u32	("auto_da_alloc",	Opt_auto_da_alloc),
	fsparam_flag	("auto_da_alloc",	Opt_auto_da_alloc),
	fsparam_flag	("noauto_da_alloc",	Opt_noauto_da_alloc),
	fsparam_flag	("dioread_nolock",	Opt_dioread_nolock),
	fsparam_flag	("nodioread_nolock",	Opt_dioread_lock),
	fsparam_flag	("dioread_lock",	Opt_dioread_lock),
	fsparam_flag	("discard",		Opt_discard),
	fsparam_flag	("nodiscard",		Opt_nodiscard),
	fsparam_u32	("init_itable",		Opt_init_itable),
	fsparam_flag	("init_itable",		Opt_init_itable),
	fsparam_flag	("noinit_itable",	Opt_noinit_itable),
#ifdef CONFIG_EXT4_DEBUG
	fsparam_flag	("fc_debug_force",	Opt_fc_debug_force),
	fsparam_u32	("fc_debug_max_replay",	Opt_fc_debug_max_replay),
#endif
	fsparam_u32	("max_dir_size_kb",	Opt_max_dir_size_kb),
	fsparam_flag	("test_dummy_encryption",
						Opt_test_dummy_encryption),
	fsparam_string	("test_dummy_encryption",
						Opt_test_dummy_encryption),
	fsparam_flag	("inlinecrypt",		Opt_inlinecrypt),
	fsparam_flag	("nombcache",		Opt_nombcache),
	fsparam_flag	("no_mbcache",		Opt_nombcache),	/* for backward compatibility */
	fsparam_flag	("prefetch_block_bitmaps",
						Opt_removed),
	fsparam_flag	("no_prefetch_block_bitmaps",
						Opt_no_prefetch_block_bitmaps),
	fsparam_s32	("mb_optimize_scan",	Opt_mb_optimize_scan),
	fsparam_string	("check",		Opt_removed),	/* mount option from ext2/3 */
	fsparam_flag	("nocheck",		Opt_removed),	/* mount option from ext2/3 */
	fsparam_flag	("reservation",		Opt_removed),	/* mount option from ext2/3 */
	fsparam_flag	("noreservation",	Opt_removed),	/* mount option from ext2/3 */
	fsparam_u32	("journal",		Opt_removed),	/* mount option from ext2/3 */
	{}
};

#define DEFAULT_JOURNAL_IOPRIO (IOPRIO_PRIO_VALUE(IOPRIO_CLASS_BE, 3))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MOPT_SET	0x0001
#define MOPT_CLEAR	0x0002
#define MOPT_NOSUPPORT	0x0004
#define MOPT_EXPLICIT	0x0008
<<<<<<< HEAD
#define MOPT_CLEAR_ERR	0x0010
#define MOPT_GTE0	0x0020
#ifdef CONFIG_QUOTA
#define MOPT_Q		0
#define MOPT_QFMT	0x0040
=======
#ifdef CONFIG_QUOTA
#define MOPT_Q		0
#define MOPT_QFMT	0x0010
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
#define MOPT_Q		MOPT_NOSUPPORT
#define MOPT_QFMT	MOPT_NOSUPPORT
#endif
<<<<<<< HEAD
#define MOPT_DATAJ	0x0080
=======
#define MOPT_NO_EXT2	0x0020
#define MOPT_NO_EXT3	0x0040
#define MOPT_EXT4_ONLY	(MOPT_NO_EXT2 | MOPT_NO_EXT3)
#define MOPT_SKIP	0x0080
#define	MOPT_2		0x0100
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct mount_opts {
	int	token;
	int	mount_opt;
	int	flags;
} ext4_mount_opts[] = {
	{Opt_minix_df, EXT4_MOUNT_MINIX_DF, MOPT_SET},
	{Opt_bsd_df, EXT4_MOUNT_MINIX_DF, MOPT_CLEAR},
	{Opt_grpid, EXT4_MOUNT_GRPID, MOPT_SET},
	{Opt_nogrpid, EXT4_MOUNT_GRPID, MOPT_CLEAR},
<<<<<<< HEAD
	{Opt_mblk_io_submit, EXT4_MOUNT_MBLK_IO_SUBMIT, MOPT_SET},
	{Opt_nomblk_io_submit, EXT4_MOUNT_MBLK_IO_SUBMIT, MOPT_CLEAR},
	{Opt_block_validity, EXT4_MOUNT_BLOCK_VALIDITY, MOPT_SET},
	{Opt_noblock_validity, EXT4_MOUNT_BLOCK_VALIDITY, MOPT_CLEAR},
	{Opt_dioread_nolock, EXT4_MOUNT_DIOREAD_NOLOCK, MOPT_SET},
	{Opt_dioread_lock, EXT4_MOUNT_DIOREAD_NOLOCK, MOPT_CLEAR},
	{Opt_discard, EXT4_MOUNT_DISCARD, MOPT_SET},
	{Opt_nodiscard, EXT4_MOUNT_DISCARD, MOPT_CLEAR},
	{Opt_delalloc, EXT4_MOUNT_DELALLOC, MOPT_SET | MOPT_EXPLICIT},
	{Opt_nodelalloc, EXT4_MOUNT_DELALLOC, MOPT_CLEAR | MOPT_EXPLICIT},
	{Opt_journal_checksum, EXT4_MOUNT_JOURNAL_CHECKSUM, MOPT_SET},
	{Opt_journal_async_commit, (EXT4_MOUNT_JOURNAL_ASYNC_COMMIT |
				    EXT4_MOUNT_JOURNAL_CHECKSUM), MOPT_SET},
	{Opt_noload, EXT4_MOUNT_NOLOAD, MOPT_SET},
	{Opt_err_panic, EXT4_MOUNT_ERRORS_PANIC, MOPT_SET | MOPT_CLEAR_ERR},
	{Opt_err_ro, EXT4_MOUNT_ERRORS_RO, MOPT_SET | MOPT_CLEAR_ERR},
	{Opt_err_cont, EXT4_MOUNT_ERRORS_CONT, MOPT_SET | MOPT_CLEAR_ERR},
	{Opt_data_err_abort, EXT4_MOUNT_DATA_ERR_ABORT, MOPT_SET},
	{Opt_data_err_ignore, EXT4_MOUNT_DATA_ERR_ABORT, MOPT_CLEAR},
=======
	{Opt_block_validity, EXT4_MOUNT_BLOCK_VALIDITY, MOPT_SET},
	{Opt_noblock_validity, EXT4_MOUNT_BLOCK_VALIDITY, MOPT_CLEAR},
	{Opt_dioread_nolock, EXT4_MOUNT_DIOREAD_NOLOCK,
	 MOPT_EXT4_ONLY | MOPT_SET},
	{Opt_dioread_lock, EXT4_MOUNT_DIOREAD_NOLOCK,
	 MOPT_EXT4_ONLY | MOPT_CLEAR},
	{Opt_discard, EXT4_MOUNT_DISCARD, MOPT_SET},
	{Opt_nodiscard, EXT4_MOUNT_DISCARD, MOPT_CLEAR},
	{Opt_delalloc, EXT4_MOUNT_DELALLOC,
	 MOPT_EXT4_ONLY | MOPT_SET | MOPT_EXPLICIT},
	{Opt_nodelalloc, EXT4_MOUNT_DELALLOC,
	 MOPT_EXT4_ONLY | MOPT_CLEAR},
	{Opt_warn_on_error, EXT4_MOUNT_WARN_ON_ERROR, MOPT_SET},
	{Opt_nowarn_on_error, EXT4_MOUNT_WARN_ON_ERROR, MOPT_CLEAR},
	{Opt_commit, 0, MOPT_NO_EXT2},
	{Opt_nojournal_checksum, EXT4_MOUNT_JOURNAL_CHECKSUM,
	 MOPT_EXT4_ONLY | MOPT_CLEAR},
	{Opt_journal_checksum, EXT4_MOUNT_JOURNAL_CHECKSUM,
	 MOPT_EXT4_ONLY | MOPT_SET | MOPT_EXPLICIT},
	{Opt_journal_async_commit, (EXT4_MOUNT_JOURNAL_ASYNC_COMMIT |
				    EXT4_MOUNT_JOURNAL_CHECKSUM),
	 MOPT_EXT4_ONLY | MOPT_SET | MOPT_EXPLICIT},
	{Opt_noload, EXT4_MOUNT_NOLOAD, MOPT_NO_EXT2 | MOPT_SET},
	{Opt_data_err, EXT4_MOUNT_DATA_ERR_ABORT, MOPT_NO_EXT2},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{Opt_barrier, EXT4_MOUNT_BARRIER, MOPT_SET},
	{Opt_nobarrier, EXT4_MOUNT_BARRIER, MOPT_CLEAR},
	{Opt_noauto_da_alloc, EXT4_MOUNT_NO_AUTO_DA_ALLOC, MOPT_SET},
	{Opt_auto_da_alloc, EXT4_MOUNT_NO_AUTO_DA_ALLOC, MOPT_CLEAR},
	{Opt_noinit_itable, EXT4_MOUNT_INIT_INODE_TABLE, MOPT_CLEAR},
<<<<<<< HEAD
	{Opt_commit, 0, MOPT_GTE0},
	{Opt_max_batch_time, 0, MOPT_GTE0},
	{Opt_min_batch_time, 0, MOPT_GTE0},
	{Opt_inode_readahead_blks, 0, MOPT_GTE0},
	{Opt_init_itable, 0, MOPT_GTE0},
	{Opt_stripe, 0, MOPT_GTE0},
	{Opt_data_journal, EXT4_MOUNT_JOURNAL_DATA, MOPT_DATAJ},
	{Opt_data_ordered, EXT4_MOUNT_ORDERED_DATA, MOPT_DATAJ},
	{Opt_data_writeback, EXT4_MOUNT_WRITEBACK_DATA, MOPT_DATAJ},
#ifdef CONFIG_EXT4_FS_XATTR
	{Opt_user_xattr, EXT4_MOUNT_XATTR_USER, MOPT_SET},
	{Opt_nouser_xattr, EXT4_MOUNT_XATTR_USER, MOPT_CLEAR},
#else
	{Opt_user_xattr, 0, MOPT_NOSUPPORT},
	{Opt_nouser_xattr, 0, MOPT_NOSUPPORT},
#endif
#ifdef CONFIG_EXT4_FS_POSIX_ACL
	{Opt_acl, EXT4_MOUNT_POSIX_ACL, MOPT_SET},
	{Opt_noacl, EXT4_MOUNT_POSIX_ACL, MOPT_CLEAR},
#else
	{Opt_acl, 0, MOPT_NOSUPPORT},
	{Opt_noacl, 0, MOPT_NOSUPPORT},
=======
	{Opt_dax_type, 0, MOPT_EXT4_ONLY},
	{Opt_journal_dev, 0, MOPT_NO_EXT2},
	{Opt_journal_path, 0, MOPT_NO_EXT2},
	{Opt_journal_ioprio, 0, MOPT_NO_EXT2},
	{Opt_data, 0, MOPT_NO_EXT2},
	{Opt_user_xattr, EXT4_MOUNT_XATTR_USER, MOPT_SET},
#ifdef CONFIG_EXT4_FS_POSIX_ACL
	{Opt_acl, EXT4_MOUNT_POSIX_ACL, MOPT_SET},
#else
	{Opt_acl, 0, MOPT_NOSUPPORT},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	{Opt_nouid32, EXT4_MOUNT_NO_UID32, MOPT_SET},
	{Opt_debug, EXT4_MOUNT_DEBUG, MOPT_SET},
	{Opt_quota, EXT4_MOUNT_QUOTA | EXT4_MOUNT_USRQUOTA, MOPT_SET | MOPT_Q},
	{Opt_usrquota, EXT4_MOUNT_QUOTA | EXT4_MOUNT_USRQUOTA,
							MOPT_SET | MOPT_Q},
	{Opt_grpquota, EXT4_MOUNT_QUOTA | EXT4_MOUNT_GRPQUOTA,
							MOPT_SET | MOPT_Q},
<<<<<<< HEAD
	{Opt_noquota, (EXT4_MOUNT_QUOTA | EXT4_MOUNT_USRQUOTA |
		       EXT4_MOUNT_GRPQUOTA), MOPT_CLEAR | MOPT_Q},
	{Opt_usrjquota, 0, MOPT_Q},
	{Opt_grpjquota, 0, MOPT_Q},
	{Opt_offusrjquota, 0, MOPT_Q},
	{Opt_offgrpjquota, 0, MOPT_Q},
	{Opt_jqfmt_vfsold, QFMT_VFS_OLD, MOPT_QFMT},
	{Opt_jqfmt_vfsv0, QFMT_VFS_V0, MOPT_QFMT},
	{Opt_jqfmt_vfsv1, QFMT_VFS_V1, MOPT_QFMT},
	{Opt_err, 0, 0}
};

static int handle_mount_opt(struct super_block *sb, char *opt, int token,
			    substring_t *args, unsigned long *journal_devnum,
			    unsigned int *journal_ioprio, int is_remount)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	const struct mount_opts *m;
	int arg = 0;

#ifdef CONFIG_QUOTA
	if (token == Opt_usrjquota)
		return set_qf_name(sb, USRQUOTA, &args[0]);
	else if (token == Opt_grpjquota)
		return set_qf_name(sb, GRPQUOTA, &args[0]);
	else if (token == Opt_offusrjquota)
		return clear_qf_name(sb, USRQUOTA);
	else if (token == Opt_offgrpjquota)
		return clear_qf_name(sb, GRPQUOTA);
#endif
	if (args->from && match_int(args, &arg))
		return -1;
	switch (token) {
	case Opt_noacl:
	case Opt_nouser_xattr:
		ext4_msg(sb, KERN_WARNING, deprecated_msg, opt, "3.5");
		break;
	case Opt_sb:
		return 1;	/* handled by get_sb_block() */
	case Opt_removed:
		ext4_msg(sb, KERN_WARNING,
			 "Ignoring removed %s option", opt);
		return 1;
	case Opt_resuid:
		sbi->s_resuid = arg;
		return 1;
	case Opt_resgid:
		sbi->s_resgid = arg;
		return 1;
	case Opt_abort:
		sbi->s_mount_flags |= EXT4_MF_FS_ABORTED;
		return 1;
	case Opt_i_version:
		sb->s_flags |= MS_I_VERSION;
		return 1;
	case Opt_journal_dev:
		if (is_remount) {
			ext4_msg(sb, KERN_ERR,
				 "Cannot specify journal on remount");
			return -1;
		}
		*journal_devnum = arg;
		return 1;
	case Opt_journal_ioprio:
		if (arg < 0 || arg > 7)
			return -1;
		*journal_ioprio = IOPRIO_PRIO_VALUE(IOPRIO_CLASS_BE, arg);
		return 1;
	}

	for (m = ext4_mount_opts; m->token != Opt_err; m++) {
		if (token != m->token)
			continue;
		if (args->from && (m->flags & MOPT_GTE0) && (arg < 0))
			return -1;
		if (m->flags & MOPT_EXPLICIT)
			set_opt2(sb, EXPLICIT_DELALLOC);
		if (m->flags & MOPT_CLEAR_ERR)
			clear_opt(sb, ERRORS_MASK);
		if (token == Opt_noquota && sb_any_quota_loaded(sb)) {
			ext4_msg(sb, KERN_ERR, "Cannot change quota "
				 "options when quota turned on");
			return -1;
		}

		if (m->flags & MOPT_NOSUPPORT) {
			ext4_msg(sb, KERN_ERR, "%s option not supported", opt);
		} else if (token == Opt_commit) {
			if (arg == 0)
				arg = JBD2_DEFAULT_MAX_COMMIT_AGE;
			sbi->s_commit_interval = HZ * arg;
		} else if (token == Opt_max_batch_time) {
			if (arg == 0)
				arg = EXT4_DEF_MAX_BATCH_TIME;
			sbi->s_max_batch_time = arg;
		} else if (token == Opt_min_batch_time) {
			sbi->s_min_batch_time = arg;
		} else if (token == Opt_inode_readahead_blks) {
			if (arg > (1 << 30))
				return -1;
			if (arg && !is_power_of_2(arg)) {
				ext4_msg(sb, KERN_ERR,
					 "EXT4-fs: inode_readahead_blks"
					 " must be a power of 2");
				return -1;
			}
			sbi->s_inode_readahead_blks = arg;
		} else if (token == Opt_init_itable) {
			set_opt(sb, INIT_INODE_TABLE);
			if (!args->from)
				arg = EXT4_DEF_LI_WAIT_MULT;
			sbi->s_li_wait_mult = arg;
		} else if (token == Opt_stripe) {
			sbi->s_stripe = arg;
		} else if (m->flags & MOPT_DATAJ) {
			if (is_remount) {
				if (!sbi->s_journal)
					ext4_msg(sb, KERN_WARNING, "Remounting file system with no journal so ignoring journalled data option");
				else if (test_opt(sb, DATA_FLAGS) !=
					 m->mount_opt) {
					ext4_msg(sb, KERN_ERR,
					 "Cannot change data mode on remount");
					return -1;
				}
			} else {
				clear_opt(sb, DATA_FLAGS);
				sbi->s_mount_opt |= m->mount_opt;
			}
#ifdef CONFIG_QUOTA
		} else if (m->flags & MOPT_QFMT) {
			if (sb_any_quota_loaded(sb) &&
			    sbi->s_jquota_fmt != m->mount_opt) {
				ext4_msg(sb, KERN_ERR, "Cannot "
					 "change journaled quota options "
					 "when quota turned on");
				return -1;
			}
			sbi->s_jquota_fmt = m->mount_opt;
#endif
		} else {
			if (!args->from)
				arg = 1;
			if (m->flags & MOPT_CLEAR)
				arg = !arg;
			else if (unlikely(!(m->flags & MOPT_SET))) {
				ext4_msg(sb, KERN_WARNING,
					 "buggy handling of option %s", opt);
				WARN_ON(1);
				return -1;
			}
			if (arg != 0)
				sbi->s_mount_opt |= m->mount_opt;
			else
				sbi->s_mount_opt &= ~m->mount_opt;
		}
		return 1;
	}
	ext4_msg(sb, KERN_ERR, "Unrecognized mount option \"%s\" "
		 "or missing value", opt);
	return -1;
}

static int parse_options(char *options, struct super_block *sb,
			 unsigned long *journal_devnum,
			 unsigned int *journal_ioprio,
			 int is_remount)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	char *p;
	substring_t args[MAX_OPT_ARGS];
	int token;

	if (!options)
		return 1;

	while ((p = strsep(&options, ",")) != NULL) {
		if (!*p)
			continue;
		/*
		 * Initialize args struct so we know whether arg was
		 * found; some options take optional arguments.
		 */
		args[0].to = args[0].from = 0;
		token = match_token(p, tokens, args);
		if (handle_mount_opt(sb, p, token, args, journal_devnum,
				     journal_ioprio, is_remount) < 0)
			return 0;
	}
#ifdef CONFIG_QUOTA
	if (sbi->s_qf_names[USRQUOTA] || sbi->s_qf_names[GRPQUOTA]) {
		if (test_opt(sb, USRQUOTA) && sbi->s_qf_names[USRQUOTA])
			clear_opt(sb, USRQUOTA);

		if (test_opt(sb, GRPQUOTA) && sbi->s_qf_names[GRPQUOTA])
			clear_opt(sb, GRPQUOTA);

		if (test_opt(sb, GRPQUOTA) || test_opt(sb, USRQUOTA)) {
			ext4_msg(sb, KERN_ERR, "old and new quota "
					"format mixing");
			return 0;
		}

		if (!sbi->s_jquota_fmt) {
			ext4_msg(sb, KERN_ERR, "journaled quota format "
					"not specified");
			return 0;
		}
	}
#endif
	if (test_opt(sb, DIOREAD_NOLOCK)) {
		int blocksize =
			BLOCK_SIZE << le32_to_cpu(sbi->s_es->s_log_block_size);

		if (blocksize < PAGE_CACHE_SIZE) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "dioread_nolock if block size != PAGE_SIZE");
			return 0;
		}
	}
=======
	{Opt_prjquota, EXT4_MOUNT_QUOTA | EXT4_MOUNT_PRJQUOTA,
							MOPT_SET | MOPT_Q},
	{Opt_noquota, (EXT4_MOUNT_QUOTA | EXT4_MOUNT_USRQUOTA |
		       EXT4_MOUNT_GRPQUOTA | EXT4_MOUNT_PRJQUOTA),
							MOPT_CLEAR | MOPT_Q},
	{Opt_usrjquota, 0, MOPT_Q},
	{Opt_grpjquota, 0, MOPT_Q},
	{Opt_jqfmt, 0, MOPT_QFMT},
	{Opt_nombcache, EXT4_MOUNT_NO_MBCACHE, MOPT_SET},
	{Opt_no_prefetch_block_bitmaps, EXT4_MOUNT_NO_PREFETCH_BLOCK_BITMAPS,
	 MOPT_SET},
#ifdef CONFIG_EXT4_DEBUG
	{Opt_fc_debug_force, EXT4_MOUNT2_JOURNAL_FAST_COMMIT,
	 MOPT_SET | MOPT_2 | MOPT_EXT4_ONLY},
#endif
	{Opt_abort, EXT4_MOUNT2_ABORT, MOPT_SET | MOPT_2},
	{Opt_err, 0, 0}
};

#if IS_ENABLED(CONFIG_UNICODE)
static const struct ext4_sb_encodings {
	__u16 magic;
	char *name;
	unsigned int version;
} ext4_sb_encoding_map[] = {
	{EXT4_ENC_UTF8_12_1, "utf8", UNICODE_AGE(12, 1, 0)},
};

static const struct ext4_sb_encodings *
ext4_sb_read_encoding(const struct ext4_super_block *es)
{
	__u16 magic = le16_to_cpu(es->s_encoding);
	int i;

	for (i = 0; i < ARRAY_SIZE(ext4_sb_encoding_map); i++)
		if (magic == ext4_sb_encoding_map[i].magic)
			return &ext4_sb_encoding_map[i];

	return NULL;
}
#endif

#define EXT4_SPEC_JQUOTA			(1 <<  0)
#define EXT4_SPEC_JQFMT				(1 <<  1)
#define EXT4_SPEC_DATAJ				(1 <<  2)
#define EXT4_SPEC_SB_BLOCK			(1 <<  3)
#define EXT4_SPEC_JOURNAL_DEV			(1 <<  4)
#define EXT4_SPEC_JOURNAL_IOPRIO		(1 <<  5)
#define EXT4_SPEC_s_want_extra_isize		(1 <<  7)
#define EXT4_SPEC_s_max_batch_time		(1 <<  8)
#define EXT4_SPEC_s_min_batch_time		(1 <<  9)
#define EXT4_SPEC_s_inode_readahead_blks	(1 << 10)
#define EXT4_SPEC_s_li_wait_mult		(1 << 11)
#define EXT4_SPEC_s_max_dir_size_kb		(1 << 12)
#define EXT4_SPEC_s_stripe			(1 << 13)
#define EXT4_SPEC_s_resuid			(1 << 14)
#define EXT4_SPEC_s_resgid			(1 << 15)
#define EXT4_SPEC_s_commit_interval		(1 << 16)
#define EXT4_SPEC_s_fc_debug_max_replay		(1 << 17)
#define EXT4_SPEC_s_sb_block			(1 << 18)
#define EXT4_SPEC_mb_optimize_scan		(1 << 19)

struct ext4_fs_context {
	char		*s_qf_names[EXT4_MAXQUOTAS];
	struct fscrypt_dummy_policy dummy_enc_policy;
	int		s_jquota_fmt;	/* Format of quota to use */
#ifdef CONFIG_EXT4_DEBUG
	int s_fc_debug_max_replay;
#endif
	unsigned short	qname_spec;
	unsigned long	vals_s_flags;	/* Bits to set in s_flags */
	unsigned long	mask_s_flags;	/* Bits changed in s_flags */
	unsigned long	journal_devnum;
	unsigned long	s_commit_interval;
	unsigned long	s_stripe;
	unsigned int	s_inode_readahead_blks;
	unsigned int	s_want_extra_isize;
	unsigned int	s_li_wait_mult;
	unsigned int	s_max_dir_size_kb;
	unsigned int	journal_ioprio;
	unsigned int	vals_s_mount_opt;
	unsigned int	mask_s_mount_opt;
	unsigned int	vals_s_mount_opt2;
	unsigned int	mask_s_mount_opt2;
	unsigned int	opt_flags;	/* MOPT flags */
	unsigned int	spec;
	u32		s_max_batch_time;
	u32		s_min_batch_time;
	kuid_t		s_resuid;
	kgid_t		s_resgid;
	ext4_fsblk_t	s_sb_block;
};

static void ext4_fc_free(struct fs_context *fc)
{
	struct ext4_fs_context *ctx = fc->fs_private;
	int i;

	if (!ctx)
		return;

	for (i = 0; i < EXT4_MAXQUOTAS; i++)
		kfree(ctx->s_qf_names[i]);

	fscrypt_free_dummy_policy(&ctx->dummy_enc_policy);
	kfree(ctx);
}

int ext4_init_fs_context(struct fs_context *fc)
{
	struct ext4_fs_context *ctx;

	ctx = kzalloc(sizeof(struct ext4_fs_context), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	fc->fs_private = ctx;
	fc->ops = &ext4_context_ops;

	return 0;
}

#ifdef CONFIG_QUOTA
/*
 * Note the name of the specified quota file.
 */
static int note_qf_name(struct fs_context *fc, int qtype,
		       struct fs_parameter *param)
{
	struct ext4_fs_context *ctx = fc->fs_private;
	char *qname;

	if (param->size < 1) {
		ext4_msg(NULL, KERN_ERR, "Missing quota name");
		return -EINVAL;
	}
	if (strchr(param->string, '/')) {
		ext4_msg(NULL, KERN_ERR,
			 "quotafile must be on filesystem root");
		return -EINVAL;
	}
	if (ctx->s_qf_names[qtype]) {
		if (strcmp(ctx->s_qf_names[qtype], param->string) != 0) {
			ext4_msg(NULL, KERN_ERR,
				 "%s quota file already specified",
				 QTYPE2NAME(qtype));
			return -EINVAL;
		}
		return 0;
	}

	qname = kmemdup_nul(param->string, param->size, GFP_KERNEL);
	if (!qname) {
		ext4_msg(NULL, KERN_ERR,
			 "Not enough memory for storing quotafile name");
		return -ENOMEM;
	}
	ctx->s_qf_names[qtype] = qname;
	ctx->qname_spec |= 1 << qtype;
	ctx->spec |= EXT4_SPEC_JQUOTA;
	return 0;
}

/*
 * Clear the name of the specified quota file.
 */
static int unnote_qf_name(struct fs_context *fc, int qtype)
{
	struct ext4_fs_context *ctx = fc->fs_private;

	if (ctx->s_qf_names[qtype])
		kfree(ctx->s_qf_names[qtype]);

	ctx->s_qf_names[qtype] = NULL;
	ctx->qname_spec |= 1 << qtype;
	ctx->spec |= EXT4_SPEC_JQUOTA;
	return 0;
}
#endif

static int ext4_parse_test_dummy_encryption(const struct fs_parameter *param,
					    struct ext4_fs_context *ctx)
{
	int err;

	if (!IS_ENABLED(CONFIG_FS_ENCRYPTION)) {
		ext4_msg(NULL, KERN_WARNING,
			 "test_dummy_encryption option not supported");
		return -EINVAL;
	}
	err = fscrypt_parse_test_dummy_encryption(param,
						  &ctx->dummy_enc_policy);
	if (err == -EINVAL) {
		ext4_msg(NULL, KERN_WARNING,
			 "Value of option \"%s\" is unrecognized", param->key);
	} else if (err == -EEXIST) {
		ext4_msg(NULL, KERN_WARNING,
			 "Conflicting test_dummy_encryption options");
		return -EINVAL;
	}
	return err;
}

#define EXT4_SET_CTX(name)						\
static inline void ctx_set_##name(struct ext4_fs_context *ctx,		\
				  unsigned long flag)			\
{									\
	ctx->mask_s_##name |= flag;					\
	ctx->vals_s_##name |= flag;					\
}

#define EXT4_CLEAR_CTX(name)						\
static inline void ctx_clear_##name(struct ext4_fs_context *ctx,	\
				    unsigned long flag)			\
{									\
	ctx->mask_s_##name |= flag;					\
	ctx->vals_s_##name &= ~flag;					\
}

#define EXT4_TEST_CTX(name)						\
static inline unsigned long						\
ctx_test_##name(struct ext4_fs_context *ctx, unsigned long flag)	\
{									\
	return (ctx->vals_s_##name & flag);				\
}

EXT4_SET_CTX(flags); /* set only */
EXT4_SET_CTX(mount_opt);
EXT4_CLEAR_CTX(mount_opt);
EXT4_TEST_CTX(mount_opt);
EXT4_SET_CTX(mount_opt2);
EXT4_CLEAR_CTX(mount_opt2);
EXT4_TEST_CTX(mount_opt2);

static int ext4_parse_param(struct fs_context *fc, struct fs_parameter *param)
{
	struct ext4_fs_context *ctx = fc->fs_private;
	struct fs_parse_result result;
	const struct mount_opts *m;
	int is_remount;
	kuid_t uid;
	kgid_t gid;
	int token;

	token = fs_parse(fc, ext4_param_specs, param, &result);
	if (token < 0)
		return token;
	is_remount = fc->purpose == FS_CONTEXT_FOR_RECONFIGURE;

	for (m = ext4_mount_opts; m->token != Opt_err; m++)
		if (token == m->token)
			break;

	ctx->opt_flags |= m->flags;

	if (m->flags & MOPT_EXPLICIT) {
		if (m->mount_opt & EXT4_MOUNT_DELALLOC) {
			ctx_set_mount_opt2(ctx, EXT4_MOUNT2_EXPLICIT_DELALLOC);
		} else if (m->mount_opt & EXT4_MOUNT_JOURNAL_CHECKSUM) {
			ctx_set_mount_opt2(ctx,
				       EXT4_MOUNT2_EXPLICIT_JOURNAL_CHECKSUM);
		} else
			return -EINVAL;
	}

	if (m->flags & MOPT_NOSUPPORT) {
		ext4_msg(NULL, KERN_ERR, "%s option not supported",
			 param->key);
		return 0;
	}

	switch (token) {
#ifdef CONFIG_QUOTA
	case Opt_usrjquota:
		if (!*param->string)
			return unnote_qf_name(fc, USRQUOTA);
		else
			return note_qf_name(fc, USRQUOTA, param);
	case Opt_grpjquota:
		if (!*param->string)
			return unnote_qf_name(fc, GRPQUOTA);
		else
			return note_qf_name(fc, GRPQUOTA, param);
#endif
	case Opt_sb:
		if (fc->purpose == FS_CONTEXT_FOR_RECONFIGURE) {
			ext4_msg(NULL, KERN_WARNING,
				 "Ignoring %s option on remount", param->key);
		} else {
			ctx->s_sb_block = result.uint_32;
			ctx->spec |= EXT4_SPEC_s_sb_block;
		}
		return 0;
	case Opt_removed:
		ext4_msg(NULL, KERN_WARNING, "Ignoring removed %s option",
			 param->key);
		return 0;
	case Opt_inlinecrypt:
#ifdef CONFIG_FS_ENCRYPTION_INLINE_CRYPT
		ctx_set_flags(ctx, SB_INLINECRYPT);
#else
		ext4_msg(NULL, KERN_ERR, "inline encryption not supported");
#endif
		return 0;
	case Opt_errors:
		ctx_clear_mount_opt(ctx, EXT4_MOUNT_ERRORS_MASK);
		ctx_set_mount_opt(ctx, result.uint_32);
		return 0;
#ifdef CONFIG_QUOTA
	case Opt_jqfmt:
		ctx->s_jquota_fmt = result.uint_32;
		ctx->spec |= EXT4_SPEC_JQFMT;
		return 0;
#endif
	case Opt_data:
		ctx_clear_mount_opt(ctx, EXT4_MOUNT_DATA_FLAGS);
		ctx_set_mount_opt(ctx, result.uint_32);
		ctx->spec |= EXT4_SPEC_DATAJ;
		return 0;
	case Opt_commit:
		if (result.uint_32 == 0)
			result.uint_32 = JBD2_DEFAULT_MAX_COMMIT_AGE;
		else if (result.uint_32 > INT_MAX / HZ) {
			ext4_msg(NULL, KERN_ERR,
				 "Invalid commit interval %d, "
				 "must be smaller than %d",
				 result.uint_32, INT_MAX / HZ);
			return -EINVAL;
		}
		ctx->s_commit_interval = HZ * result.uint_32;
		ctx->spec |= EXT4_SPEC_s_commit_interval;
		return 0;
	case Opt_debug_want_extra_isize:
		if ((result.uint_32 & 1) || (result.uint_32 < 4)) {
			ext4_msg(NULL, KERN_ERR,
				 "Invalid want_extra_isize %d", result.uint_32);
			return -EINVAL;
		}
		ctx->s_want_extra_isize = result.uint_32;
		ctx->spec |= EXT4_SPEC_s_want_extra_isize;
		return 0;
	case Opt_max_batch_time:
		ctx->s_max_batch_time = result.uint_32;
		ctx->spec |= EXT4_SPEC_s_max_batch_time;
		return 0;
	case Opt_min_batch_time:
		ctx->s_min_batch_time = result.uint_32;
		ctx->spec |= EXT4_SPEC_s_min_batch_time;
		return 0;
	case Opt_inode_readahead_blks:
		if (result.uint_32 &&
		    (result.uint_32 > (1 << 30) ||
		     !is_power_of_2(result.uint_32))) {
			ext4_msg(NULL, KERN_ERR,
				 "EXT4-fs: inode_readahead_blks must be "
				 "0 or a power of 2 smaller than 2^31");
			return -EINVAL;
		}
		ctx->s_inode_readahead_blks = result.uint_32;
		ctx->spec |= EXT4_SPEC_s_inode_readahead_blks;
		return 0;
	case Opt_init_itable:
		ctx_set_mount_opt(ctx, EXT4_MOUNT_INIT_INODE_TABLE);
		ctx->s_li_wait_mult = EXT4_DEF_LI_WAIT_MULT;
		if (param->type == fs_value_is_string)
			ctx->s_li_wait_mult = result.uint_32;
		ctx->spec |= EXT4_SPEC_s_li_wait_mult;
		return 0;
	case Opt_max_dir_size_kb:
		ctx->s_max_dir_size_kb = result.uint_32;
		ctx->spec |= EXT4_SPEC_s_max_dir_size_kb;
		return 0;
#ifdef CONFIG_EXT4_DEBUG
	case Opt_fc_debug_max_replay:
		ctx->s_fc_debug_max_replay = result.uint_32;
		ctx->spec |= EXT4_SPEC_s_fc_debug_max_replay;
		return 0;
#endif
	case Opt_stripe:
		ctx->s_stripe = result.uint_32;
		ctx->spec |= EXT4_SPEC_s_stripe;
		return 0;
	case Opt_resuid:
		uid = make_kuid(current_user_ns(), result.uint_32);
		if (!uid_valid(uid)) {
			ext4_msg(NULL, KERN_ERR, "Invalid uid value %d",
				 result.uint_32);
			return -EINVAL;
		}
		ctx->s_resuid = uid;
		ctx->spec |= EXT4_SPEC_s_resuid;
		return 0;
	case Opt_resgid:
		gid = make_kgid(current_user_ns(), result.uint_32);
		if (!gid_valid(gid)) {
			ext4_msg(NULL, KERN_ERR, "Invalid gid value %d",
				 result.uint_32);
			return -EINVAL;
		}
		ctx->s_resgid = gid;
		ctx->spec |= EXT4_SPEC_s_resgid;
		return 0;
	case Opt_journal_dev:
		if (is_remount) {
			ext4_msg(NULL, KERN_ERR,
				 "Cannot specify journal on remount");
			return -EINVAL;
		}
		ctx->journal_devnum = result.uint_32;
		ctx->spec |= EXT4_SPEC_JOURNAL_DEV;
		return 0;
	case Opt_journal_path:
	{
		struct inode *journal_inode;
		struct path path;
		int error;

		if (is_remount) {
			ext4_msg(NULL, KERN_ERR,
				 "Cannot specify journal on remount");
			return -EINVAL;
		}

		error = fs_lookup_param(fc, param, 1, LOOKUP_FOLLOW, &path);
		if (error) {
			ext4_msg(NULL, KERN_ERR, "error: could not find "
				 "journal device path");
			return -EINVAL;
		}

		journal_inode = d_inode(path.dentry);
		ctx->journal_devnum = new_encode_dev(journal_inode->i_rdev);
		ctx->spec |= EXT4_SPEC_JOURNAL_DEV;
		path_put(&path);
		return 0;
	}
	case Opt_journal_ioprio:
		if (result.uint_32 > 7) {
			ext4_msg(NULL, KERN_ERR, "Invalid journal IO priority"
				 " (must be 0-7)");
			return -EINVAL;
		}
		ctx->journal_ioprio =
			IOPRIO_PRIO_VALUE(IOPRIO_CLASS_BE, result.uint_32);
		ctx->spec |= EXT4_SPEC_JOURNAL_IOPRIO;
		return 0;
	case Opt_test_dummy_encryption:
		return ext4_parse_test_dummy_encryption(param, ctx);
	case Opt_dax:
	case Opt_dax_type:
#ifdef CONFIG_FS_DAX
	{
		int type = (token == Opt_dax) ?
			   Opt_dax : result.uint_32;

		switch (type) {
		case Opt_dax:
		case Opt_dax_always:
			ctx_set_mount_opt(ctx, EXT4_MOUNT_DAX_ALWAYS);
			ctx_clear_mount_opt2(ctx, EXT4_MOUNT2_DAX_NEVER);
			break;
		case Opt_dax_never:
			ctx_set_mount_opt2(ctx, EXT4_MOUNT2_DAX_NEVER);
			ctx_clear_mount_opt(ctx, EXT4_MOUNT_DAX_ALWAYS);
			break;
		case Opt_dax_inode:
			ctx_clear_mount_opt(ctx, EXT4_MOUNT_DAX_ALWAYS);
			ctx_clear_mount_opt2(ctx, EXT4_MOUNT2_DAX_NEVER);
			/* Strictly for printing options */
			ctx_set_mount_opt2(ctx, EXT4_MOUNT2_DAX_INODE);
			break;
		}
		return 0;
	}
#else
		ext4_msg(NULL, KERN_INFO, "dax option not supported");
		return -EINVAL;
#endif
	case Opt_data_err:
		if (result.uint_32 == Opt_data_err_abort)
			ctx_set_mount_opt(ctx, m->mount_opt);
		else if (result.uint_32 == Opt_data_err_ignore)
			ctx_clear_mount_opt(ctx, m->mount_opt);
		return 0;
	case Opt_mb_optimize_scan:
		if (result.int_32 == 1) {
			ctx_set_mount_opt2(ctx, EXT4_MOUNT2_MB_OPTIMIZE_SCAN);
			ctx->spec |= EXT4_SPEC_mb_optimize_scan;
		} else if (result.int_32 == 0) {
			ctx_clear_mount_opt2(ctx, EXT4_MOUNT2_MB_OPTIMIZE_SCAN);
			ctx->spec |= EXT4_SPEC_mb_optimize_scan;
		} else {
			ext4_msg(NULL, KERN_WARNING,
				 "mb_optimize_scan should be set to 0 or 1.");
			return -EINVAL;
		}
		return 0;
	}

	/*
	 * At this point we should only be getting options requiring MOPT_SET,
	 * or MOPT_CLEAR. Anything else is a bug
	 */
	if (m->token == Opt_err) {
		ext4_msg(NULL, KERN_WARNING, "buggy handling of option %s",
			 param->key);
		WARN_ON(1);
		return -EINVAL;
	}

	else {
		unsigned int set = 0;

		if ((param->type == fs_value_is_flag) ||
		    result.uint_32 > 0)
			set = 1;

		if (m->flags & MOPT_CLEAR)
			set = !set;
		else if (unlikely(!(m->flags & MOPT_SET))) {
			ext4_msg(NULL, KERN_WARNING,
				 "buggy handling of option %s",
				 param->key);
			WARN_ON(1);
			return -EINVAL;
		}
		if (m->flags & MOPT_2) {
			if (set != 0)
				ctx_set_mount_opt2(ctx, m->mount_opt);
			else
				ctx_clear_mount_opt2(ctx, m->mount_opt);
		} else {
			if (set != 0)
				ctx_set_mount_opt(ctx, m->mount_opt);
			else
				ctx_clear_mount_opt(ctx, m->mount_opt);
		}
	}

	return 0;
}

static int parse_options(struct fs_context *fc, char *options)
{
	struct fs_parameter param;
	int ret;
	char *key;

	if (!options)
		return 0;

	while ((key = strsep(&options, ",")) != NULL) {
		if (*key) {
			size_t v_len = 0;
			char *value = strchr(key, '=');

			param.type = fs_value_is_flag;
			param.string = NULL;

			if (value) {
				if (value == key)
					continue;

				*value++ = 0;
				v_len = strlen(value);
				param.string = kmemdup_nul(value, v_len,
							   GFP_KERNEL);
				if (!param.string)
					return -ENOMEM;
				param.type = fs_value_is_string;
			}

			param.key = key;
			param.size = v_len;

			ret = ext4_parse_param(fc, &param);
			if (param.string)
				kfree(param.string);
			if (ret < 0)
				return ret;
		}
	}

	ret = ext4_validate_options(fc);
	if (ret < 0)
		return ret;

	return 0;
}

static int parse_apply_sb_mount_options(struct super_block *sb,
					struct ext4_fs_context *m_ctx)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	char *s_mount_opts = NULL;
	struct ext4_fs_context *s_ctx = NULL;
	struct fs_context *fc = NULL;
	int ret = -ENOMEM;

	if (!sbi->s_es->s_mount_opts[0])
		return 0;

	s_mount_opts = kstrndup(sbi->s_es->s_mount_opts,
				sizeof(sbi->s_es->s_mount_opts),
				GFP_KERNEL);
	if (!s_mount_opts)
		return ret;

	fc = kzalloc(sizeof(struct fs_context), GFP_KERNEL);
	if (!fc)
		goto out_free;

	s_ctx = kzalloc(sizeof(struct ext4_fs_context), GFP_KERNEL);
	if (!s_ctx)
		goto out_free;

	fc->fs_private = s_ctx;
	fc->s_fs_info = sbi;

	ret = parse_options(fc, s_mount_opts);
	if (ret < 0)
		goto parse_failed;

	ret = ext4_check_opt_consistency(fc, sb);
	if (ret < 0) {
parse_failed:
		ext4_msg(sb, KERN_WARNING,
			 "failed to parse options in superblock: %s",
			 s_mount_opts);
		ret = 0;
		goto out_free;
	}

	if (s_ctx->spec & EXT4_SPEC_JOURNAL_DEV)
		m_ctx->journal_devnum = s_ctx->journal_devnum;
	if (s_ctx->spec & EXT4_SPEC_JOURNAL_IOPRIO)
		m_ctx->journal_ioprio = s_ctx->journal_ioprio;

	ext4_apply_options(fc, sb);
	ret = 0;

out_free:
	if (fc) {
		ext4_fc_free(fc);
		kfree(fc);
	}
	kfree(s_mount_opts);
	return ret;
}

static void ext4_apply_quota_options(struct fs_context *fc,
				     struct super_block *sb)
{
#ifdef CONFIG_QUOTA
	bool quota_feature = ext4_has_feature_quota(sb);
	struct ext4_fs_context *ctx = fc->fs_private;
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	char *qname;
	int i;

	if (quota_feature)
		return;

	if (ctx->spec & EXT4_SPEC_JQUOTA) {
		for (i = 0; i < EXT4_MAXQUOTAS; i++) {
			if (!(ctx->qname_spec & (1 << i)))
				continue;

			qname = ctx->s_qf_names[i]; /* May be NULL */
			if (qname)
				set_opt(sb, QUOTA);
			ctx->s_qf_names[i] = NULL;
			qname = rcu_replace_pointer(sbi->s_qf_names[i], qname,
						lockdep_is_held(&sb->s_umount));
			if (qname)
				kfree_rcu_mightsleep(qname);
		}
	}

	if (ctx->spec & EXT4_SPEC_JQFMT)
		sbi->s_jquota_fmt = ctx->s_jquota_fmt;
#endif
}

/*
 * Check quota settings consistency.
 */
static int ext4_check_quota_consistency(struct fs_context *fc,
					struct super_block *sb)
{
#ifdef CONFIG_QUOTA
	struct ext4_fs_context *ctx = fc->fs_private;
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	bool quota_feature = ext4_has_feature_quota(sb);
	bool quota_loaded = sb_any_quota_loaded(sb);
	bool usr_qf_name, grp_qf_name, usrquota, grpquota;
	int quota_flags, i;

	/*
	 * We do the test below only for project quotas. 'usrquota' and
	 * 'grpquota' mount options are allowed even without quota feature
	 * to support legacy quotas in quota files.
	 */
	if (ctx_test_mount_opt(ctx, EXT4_MOUNT_PRJQUOTA) &&
	    !ext4_has_feature_project(sb)) {
		ext4_msg(NULL, KERN_ERR, "Project quota feature not enabled. "
			 "Cannot enable project quota enforcement.");
		return -EINVAL;
	}

	quota_flags = EXT4_MOUNT_QUOTA | EXT4_MOUNT_USRQUOTA |
		      EXT4_MOUNT_GRPQUOTA | EXT4_MOUNT_PRJQUOTA;
	if (quota_loaded &&
	    ctx->mask_s_mount_opt & quota_flags &&
	    !ctx_test_mount_opt(ctx, quota_flags))
		goto err_quota_change;

	if (ctx->spec & EXT4_SPEC_JQUOTA) {

		for (i = 0; i < EXT4_MAXQUOTAS; i++) {
			if (!(ctx->qname_spec & (1 << i)))
				continue;

			if (quota_loaded &&
			    !!sbi->s_qf_names[i] != !!ctx->s_qf_names[i])
				goto err_jquota_change;

			if (sbi->s_qf_names[i] && ctx->s_qf_names[i] &&
			    strcmp(get_qf_name(sb, sbi, i),
				   ctx->s_qf_names[i]) != 0)
				goto err_jquota_specified;
		}

		if (quota_feature) {
			ext4_msg(NULL, KERN_INFO,
				 "Journaled quota options ignored when "
				 "QUOTA feature is enabled");
			return 0;
		}
	}

	if (ctx->spec & EXT4_SPEC_JQFMT) {
		if (sbi->s_jquota_fmt != ctx->s_jquota_fmt && quota_loaded)
			goto err_jquota_change;
		if (quota_feature) {
			ext4_msg(NULL, KERN_INFO, "Quota format mount options "
				 "ignored when QUOTA feature is enabled");
			return 0;
		}
	}

	/* Make sure we don't mix old and new quota format */
	usr_qf_name = (get_qf_name(sb, sbi, USRQUOTA) ||
		       ctx->s_qf_names[USRQUOTA]);
	grp_qf_name = (get_qf_name(sb, sbi, GRPQUOTA) ||
		       ctx->s_qf_names[GRPQUOTA]);

	usrquota = (ctx_test_mount_opt(ctx, EXT4_MOUNT_USRQUOTA) ||
		    test_opt(sb, USRQUOTA));

	grpquota = (ctx_test_mount_opt(ctx, EXT4_MOUNT_GRPQUOTA) ||
		    test_opt(sb, GRPQUOTA));

	if (usr_qf_name) {
		ctx_clear_mount_opt(ctx, EXT4_MOUNT_USRQUOTA);
		usrquota = false;
	}
	if (grp_qf_name) {
		ctx_clear_mount_opt(ctx, EXT4_MOUNT_GRPQUOTA);
		grpquota = false;
	}

	if (usr_qf_name || grp_qf_name) {
		if (usrquota || grpquota) {
			ext4_msg(NULL, KERN_ERR, "old and new quota "
				 "format mixing");
			return -EINVAL;
		}

		if (!(ctx->spec & EXT4_SPEC_JQFMT || sbi->s_jquota_fmt)) {
			ext4_msg(NULL, KERN_ERR, "journaled quota format "
				 "not specified");
			return -EINVAL;
		}
	}

	return 0;

err_quota_change:
	ext4_msg(NULL, KERN_ERR,
		 "Cannot change quota options when quota turned on");
	return -EINVAL;
err_jquota_change:
	ext4_msg(NULL, KERN_ERR, "Cannot change journaled quota "
		 "options when quota turned on");
	return -EINVAL;
err_jquota_specified:
	ext4_msg(NULL, KERN_ERR, "%s quota file already specified",
		 QTYPE2NAME(i));
	return -EINVAL;
#else
	return 0;
#endif
}

static int ext4_check_test_dummy_encryption(const struct fs_context *fc,
					    struct super_block *sb)
{
	const struct ext4_fs_context *ctx = fc->fs_private;
	const struct ext4_sb_info *sbi = EXT4_SB(sb);

	if (!fscrypt_is_dummy_policy_set(&ctx->dummy_enc_policy))
		return 0;

	if (!ext4_has_feature_encrypt(sb)) {
		ext4_msg(NULL, KERN_WARNING,
			 "test_dummy_encryption requires encrypt feature");
		return -EINVAL;
	}
	/*
	 * This mount option is just for testing, and it's not worthwhile to
	 * implement the extra complexity (e.g. RCU protection) that would be
	 * needed to allow it to be set or changed during remount.  We do allow
	 * it to be specified during remount, but only if there is no change.
	 */
	if (fc->purpose == FS_CONTEXT_FOR_RECONFIGURE) {
		if (fscrypt_dummy_policies_equal(&sbi->s_dummy_enc_policy,
						 &ctx->dummy_enc_policy))
			return 0;
		ext4_msg(NULL, KERN_WARNING,
			 "Can't set or change test_dummy_encryption on remount");
		return -EINVAL;
	}
	/* Also make sure s_mount_opts didn't contain a conflicting value. */
	if (fscrypt_is_dummy_policy_set(&sbi->s_dummy_enc_policy)) {
		if (fscrypt_dummy_policies_equal(&sbi->s_dummy_enc_policy,
						 &ctx->dummy_enc_policy))
			return 0;
		ext4_msg(NULL, KERN_WARNING,
			 "Conflicting test_dummy_encryption options");
		return -EINVAL;
	}
	return 0;
}

static void ext4_apply_test_dummy_encryption(struct ext4_fs_context *ctx,
					     struct super_block *sb)
{
	if (!fscrypt_is_dummy_policy_set(&ctx->dummy_enc_policy) ||
	    /* if already set, it was already verified to be the same */
	    fscrypt_is_dummy_policy_set(&EXT4_SB(sb)->s_dummy_enc_policy))
		return;
	EXT4_SB(sb)->s_dummy_enc_policy = ctx->dummy_enc_policy;
	memset(&ctx->dummy_enc_policy, 0, sizeof(ctx->dummy_enc_policy));
	ext4_msg(sb, KERN_WARNING, "Test dummy encryption mode enabled");
}

static int ext4_check_opt_consistency(struct fs_context *fc,
				      struct super_block *sb)
{
	struct ext4_fs_context *ctx = fc->fs_private;
	struct ext4_sb_info *sbi = fc->s_fs_info;
	int is_remount = fc->purpose == FS_CONTEXT_FOR_RECONFIGURE;
	int err;

	if ((ctx->opt_flags & MOPT_NO_EXT2) && IS_EXT2_SB(sb)) {
		ext4_msg(NULL, KERN_ERR,
			 "Mount option(s) incompatible with ext2");
		return -EINVAL;
	}
	if ((ctx->opt_flags & MOPT_NO_EXT3) && IS_EXT3_SB(sb)) {
		ext4_msg(NULL, KERN_ERR,
			 "Mount option(s) incompatible with ext3");
		return -EINVAL;
	}

	if (ctx->s_want_extra_isize >
	    (sbi->s_inode_size - EXT4_GOOD_OLD_INODE_SIZE)) {
		ext4_msg(NULL, KERN_ERR,
			 "Invalid want_extra_isize %d",
			 ctx->s_want_extra_isize);
		return -EINVAL;
	}

	err = ext4_check_test_dummy_encryption(fc, sb);
	if (err)
		return err;

	if ((ctx->spec & EXT4_SPEC_DATAJ) && is_remount) {
		if (!sbi->s_journal) {
			ext4_msg(NULL, KERN_WARNING,
				 "Remounting file system with no journal "
				 "so ignoring journalled data option");
			ctx_clear_mount_opt(ctx, EXT4_MOUNT_DATA_FLAGS);
		} else if (ctx_test_mount_opt(ctx, EXT4_MOUNT_DATA_FLAGS) !=
			   test_opt(sb, DATA_FLAGS)) {
			ext4_msg(NULL, KERN_ERR, "Cannot change data mode "
				 "on remount");
			return -EINVAL;
		}
	}

	if (is_remount) {
		if (ctx_test_mount_opt(ctx, EXT4_MOUNT_DAX_ALWAYS) &&
		    (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_JOURNAL_DATA)) {
			ext4_msg(NULL, KERN_ERR, "can't mount with "
				 "both data=journal and dax");
			return -EINVAL;
		}

		if (ctx_test_mount_opt(ctx, EXT4_MOUNT_DAX_ALWAYS) &&
		    (!(sbi->s_mount_opt & EXT4_MOUNT_DAX_ALWAYS) ||
		     (sbi->s_mount_opt2 & EXT4_MOUNT2_DAX_NEVER))) {
fail_dax_change_remount:
			ext4_msg(NULL, KERN_ERR, "can't change "
				 "dax mount option while remounting");
			return -EINVAL;
		} else if (ctx_test_mount_opt2(ctx, EXT4_MOUNT2_DAX_NEVER) &&
			 (!(sbi->s_mount_opt2 & EXT4_MOUNT2_DAX_NEVER) ||
			  (sbi->s_mount_opt & EXT4_MOUNT_DAX_ALWAYS))) {
			goto fail_dax_change_remount;
		} else if (ctx_test_mount_opt2(ctx, EXT4_MOUNT2_DAX_INODE) &&
			   ((sbi->s_mount_opt & EXT4_MOUNT_DAX_ALWAYS) ||
			    (sbi->s_mount_opt2 & EXT4_MOUNT2_DAX_NEVER) ||
			    !(sbi->s_mount_opt2 & EXT4_MOUNT2_DAX_INODE))) {
			goto fail_dax_change_remount;
		}
	}

	return ext4_check_quota_consistency(fc, sb);
}

static void ext4_apply_options(struct fs_context *fc, struct super_block *sb)
{
	struct ext4_fs_context *ctx = fc->fs_private;
	struct ext4_sb_info *sbi = fc->s_fs_info;

	sbi->s_mount_opt &= ~ctx->mask_s_mount_opt;
	sbi->s_mount_opt |= ctx->vals_s_mount_opt;
	sbi->s_mount_opt2 &= ~ctx->mask_s_mount_opt2;
	sbi->s_mount_opt2 |= ctx->vals_s_mount_opt2;
	sb->s_flags &= ~ctx->mask_s_flags;
	sb->s_flags |= ctx->vals_s_flags;

#define APPLY(X) ({ if (ctx->spec & EXT4_SPEC_##X) sbi->X = ctx->X; })
	APPLY(s_commit_interval);
	APPLY(s_stripe);
	APPLY(s_max_batch_time);
	APPLY(s_min_batch_time);
	APPLY(s_want_extra_isize);
	APPLY(s_inode_readahead_blks);
	APPLY(s_max_dir_size_kb);
	APPLY(s_li_wait_mult);
	APPLY(s_resgid);
	APPLY(s_resuid);

#ifdef CONFIG_EXT4_DEBUG
	APPLY(s_fc_debug_max_replay);
#endif

	ext4_apply_quota_options(fc, sb);
	ext4_apply_test_dummy_encryption(ctx, sb);
}


static int ext4_validate_options(struct fs_context *fc)
{
#ifdef CONFIG_QUOTA
	struct ext4_fs_context *ctx = fc->fs_private;
	char *usr_qf_name, *grp_qf_name;

	usr_qf_name = ctx->s_qf_names[USRQUOTA];
	grp_qf_name = ctx->s_qf_names[GRPQUOTA];

	if (usr_qf_name || grp_qf_name) {
		if (ctx_test_mount_opt(ctx, EXT4_MOUNT_USRQUOTA) && usr_qf_name)
			ctx_clear_mount_opt(ctx, EXT4_MOUNT_USRQUOTA);

		if (ctx_test_mount_opt(ctx, EXT4_MOUNT_GRPQUOTA) && grp_qf_name)
			ctx_clear_mount_opt(ctx, EXT4_MOUNT_GRPQUOTA);

		if (ctx_test_mount_opt(ctx, EXT4_MOUNT_USRQUOTA) ||
		    ctx_test_mount_opt(ctx, EXT4_MOUNT_GRPQUOTA)) {
			ext4_msg(NULL, KERN_ERR, "old and new quota "
				 "format mixing");
			return -EINVAL;
		}
	}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}

static inline void ext4_show_quota_options(struct seq_file *seq,
					   struct super_block *sb)
{
#if defined(CONFIG_QUOTA)
	struct ext4_sb_info *sbi = EXT4_SB(sb);
<<<<<<< HEAD
=======
	char *usr_qf_name, *grp_qf_name;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sbi->s_jquota_fmt) {
		char *fmtname = "";

		switch (sbi->s_jquota_fmt) {
		case QFMT_VFS_OLD:
			fmtname = "vfsold";
			break;
		case QFMT_VFS_V0:
			fmtname = "vfsv0";
			break;
		case QFMT_VFS_V1:
			fmtname = "vfsv1";
			break;
		}
		seq_printf(seq, ",jqfmt=%s", fmtname);
	}

<<<<<<< HEAD
	if (sbi->s_qf_names[USRQUOTA])
		seq_show_option(seq, "usrjquota", sbi->s_qf_names[USRQUOTA]);

	if (sbi->s_qf_names[GRPQUOTA])
		seq_show_option(seq, "grpjquota", sbi->s_qf_names[GRPQUOTA]);

	if (test_opt(sb, USRQUOTA))
		seq_puts(seq, ",usrquota");

	if (test_opt(sb, GRPQUOTA))
		seq_puts(seq, ",grpquota");
=======
	rcu_read_lock();
	usr_qf_name = rcu_dereference(sbi->s_qf_names[USRQUOTA]);
	grp_qf_name = rcu_dereference(sbi->s_qf_names[GRPQUOTA]);
	if (usr_qf_name)
		seq_show_option(seq, "usrjquota", usr_qf_name);
	if (grp_qf_name)
		seq_show_option(seq, "grpjquota", grp_qf_name);
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

static const char *token2str(int token)
{
<<<<<<< HEAD
	const struct match_token *t;

	for (t = tokens; t->token != Opt_err; t++)
		if (t->token == token && !strchr(t->pattern, '='))
			break;
	return t->pattern;
=======
	const struct fs_parameter_spec *spec;

	for (spec = ext4_param_specs; spec->name != NULL; spec++)
		if (spec->opt == token && !spec->type)
			break;
	return spec->name;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Show an option if
 *  - it's set to a non-default value OR
 *  - if the per-sb default is different from the global default
 */
static int _ext4_show_options(struct seq_file *seq, struct super_block *sb,
			      int nodefs)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
<<<<<<< HEAD
	int def_errors, def_mount_opt = nodefs ? 0 : sbi->s_def_mount_opt;
=======
	int def_errors;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const struct mount_opts *m;
	char sep = nodefs ? '\n' : ',';

#define SEQ_OPTS_PUTS(str) seq_printf(seq, "%c" str, sep)
#define SEQ_OPTS_PRINT(str, arg) seq_printf(seq, "%c" str, sep, arg)

	if (sbi->s_sb_block != 1)
		SEQ_OPTS_PRINT("sb=%llu", sbi->s_sb_block);

	for (m = ext4_mount_opts; m->token != Opt_err; m++) {
		int want_set = m->flags & MOPT_SET;
<<<<<<< HEAD
		if (((m->flags & (MOPT_SET|MOPT_CLEAR)) == 0) ||
		    (m->flags & MOPT_CLEAR_ERR))
			continue;
		if (!(m->mount_opt & (sbi->s_mount_opt ^ def_mount_opt)))
			continue; /* skip if same as the default */
		if ((want_set &&
		     (sbi->s_mount_opt & m->mount_opt) != m->mount_opt) ||
		    (!want_set && (sbi->s_mount_opt & m->mount_opt)))
			continue; /* select Opt_noFoo vs Opt_Foo */
		SEQ_OPTS_PRINT("%s", token2str(m->token));
	}

	if (nodefs || sbi->s_resuid != EXT4_DEF_RESUID ||
	    le16_to_cpu(es->s_def_resuid) != EXT4_DEF_RESUID)
		SEQ_OPTS_PRINT("resuid=%u", sbi->s_resuid);
	if (nodefs || sbi->s_resgid != EXT4_DEF_RESGID ||
	    le16_to_cpu(es->s_def_resgid) != EXT4_DEF_RESGID)
		SEQ_OPTS_PRINT("resgid=%u", sbi->s_resgid);
=======
		int opt_2 = m->flags & MOPT_2;
		unsigned int mount_opt, def_mount_opt;

		if (((m->flags & (MOPT_SET|MOPT_CLEAR)) == 0) ||
		    m->flags & MOPT_SKIP)
			continue;

		if (opt_2) {
			mount_opt = sbi->s_mount_opt2;
			def_mount_opt = sbi->s_def_mount_opt2;
		} else {
			mount_opt = sbi->s_mount_opt;
			def_mount_opt = sbi->s_def_mount_opt;
		}
		/* skip if same as the default */
		if (!nodefs && !(m->mount_opt & (mount_opt ^ def_mount_opt)))
			continue;
		/* select Opt_noFoo vs Opt_Foo */
		if ((want_set &&
		     (mount_opt & m->mount_opt) != m->mount_opt) ||
		    (!want_set && (mount_opt & m->mount_opt)))
			continue;
		SEQ_OPTS_PRINT("%s", token2str(m->token));
	}

	if (nodefs || !uid_eq(sbi->s_resuid, make_kuid(&init_user_ns, EXT4_DEF_RESUID)) ||
	    le16_to_cpu(es->s_def_resuid) != EXT4_DEF_RESUID)
		SEQ_OPTS_PRINT("resuid=%u",
				from_kuid_munged(&init_user_ns, sbi->s_resuid));
	if (nodefs || !gid_eq(sbi->s_resgid, make_kgid(&init_user_ns, EXT4_DEF_RESGID)) ||
	    le16_to_cpu(es->s_def_resgid) != EXT4_DEF_RESGID)
		SEQ_OPTS_PRINT("resgid=%u",
				from_kgid_munged(&init_user_ns, sbi->s_resgid));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	def_errors = nodefs ? -1 : le16_to_cpu(es->s_errors);
	if (test_opt(sb, ERRORS_RO) && def_errors != EXT4_ERRORS_RO)
		SEQ_OPTS_PUTS("errors=remount-ro");
	if (test_opt(sb, ERRORS_CONT) && def_errors != EXT4_ERRORS_CONTINUE)
		SEQ_OPTS_PUTS("errors=continue");
	if (test_opt(sb, ERRORS_PANIC) && def_errors != EXT4_ERRORS_PANIC)
		SEQ_OPTS_PUTS("errors=panic");
	if (nodefs || sbi->s_commit_interval != JBD2_DEFAULT_MAX_COMMIT_AGE*HZ)
		SEQ_OPTS_PRINT("commit=%lu", sbi->s_commit_interval / HZ);
	if (nodefs || sbi->s_min_batch_time != EXT4_DEF_MIN_BATCH_TIME)
		SEQ_OPTS_PRINT("min_batch_time=%u", sbi->s_min_batch_time);
	if (nodefs || sbi->s_max_batch_time != EXT4_DEF_MAX_BATCH_TIME)
		SEQ_OPTS_PRINT("max_batch_time=%u", sbi->s_max_batch_time);
<<<<<<< HEAD
	if (sb->s_flags & MS_I_VERSION)
		SEQ_OPTS_PUTS("i_version");
	if (nodefs || sbi->s_stripe)
		SEQ_OPTS_PRINT("stripe=%lu", sbi->s_stripe);
	if (EXT4_MOUNT_DATA_FLAGS & (sbi->s_mount_opt ^ def_mount_opt)) {
=======
	if (nodefs || sbi->s_stripe)
		SEQ_OPTS_PRINT("stripe=%lu", sbi->s_stripe);
	if (nodefs || EXT4_MOUNT_DATA_FLAGS &
			(sbi->s_mount_opt ^ sbi->s_def_mount_opt)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_JOURNAL_DATA)
			SEQ_OPTS_PUTS("data=journal");
		else if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_ORDERED_DATA)
			SEQ_OPTS_PUTS("data=ordered");
		else if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_WRITEBACK_DATA)
			SEQ_OPTS_PUTS("data=writeback");
	}
	if (nodefs ||
	    sbi->s_inode_readahead_blks != EXT4_DEF_INODE_READAHEAD_BLKS)
		SEQ_OPTS_PRINT("inode_readahead_blks=%u",
			       sbi->s_inode_readahead_blks);

<<<<<<< HEAD
	if (nodefs || (test_opt(sb, INIT_INODE_TABLE) &&
		       (sbi->s_li_wait_mult != EXT4_DEF_LI_WAIT_MULT)))
		SEQ_OPTS_PRINT("init_itable=%u", sbi->s_li_wait_mult);
=======
	if (test_opt(sb, INIT_INODE_TABLE) && (nodefs ||
		       (sbi->s_li_wait_mult != EXT4_DEF_LI_WAIT_MULT)))
		SEQ_OPTS_PRINT("init_itable=%u", sbi->s_li_wait_mult);
	if (nodefs || sbi->s_max_dir_size_kb)
		SEQ_OPTS_PRINT("max_dir_size_kb=%u", sbi->s_max_dir_size_kb);
	if (test_opt(sb, DATA_ERR_ABORT))
		SEQ_OPTS_PUTS("data_err=abort");

	fscrypt_show_test_dummy_encryption(seq, sep, sb);

	if (sb->s_flags & SB_INLINECRYPT)
		SEQ_OPTS_PUTS("inlinecrypt");

	if (test_opt(sb, DAX_ALWAYS)) {
		if (IS_EXT2_SB(sb))
			SEQ_OPTS_PUTS("dax");
		else
			SEQ_OPTS_PUTS("dax=always");
	} else if (test_opt2(sb, DAX_NEVER)) {
		SEQ_OPTS_PUTS("dax=never");
	} else if (test_opt2(sb, DAX_INODE)) {
		SEQ_OPTS_PUTS("dax=inode");
	}

	if (sbi->s_groups_count >= MB_DEFAULT_LINEAR_SCAN_THRESHOLD &&
			!test_opt2(sb, MB_OPTIMIZE_SCAN)) {
		SEQ_OPTS_PUTS("mb_optimize_scan=0");
	} else if (sbi->s_groups_count < MB_DEFAULT_LINEAR_SCAN_THRESHOLD &&
			test_opt2(sb, MB_OPTIMIZE_SCAN)) {
		SEQ_OPTS_PUTS("mb_optimize_scan=1");
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ext4_show_quota_options(seq, sb);
	return 0;
}

static int ext4_show_options(struct seq_file *seq, struct dentry *root)
{
	return _ext4_show_options(seq, root->d_sb, 0);
}

<<<<<<< HEAD
static int options_seq_show(struct seq_file *seq, void *offset)
=======
int ext4_seq_options_show(struct seq_file *seq, void *offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct super_block *sb = seq->private;
	int rc;

<<<<<<< HEAD
	seq_puts(seq, (sb->s_flags & MS_RDONLY) ? "ro" : "rw");
=======
	seq_puts(seq, sb_rdonly(sb) ? "ro" : "rw");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = _ext4_show_options(seq, sb, 1);
	seq_puts(seq, "\n");
	return rc;
}

<<<<<<< HEAD
static int options_open_fs(struct inode *inode, struct file *file)
{
	return single_open(file, options_seq_show, PDE(inode)->data);
}

static const struct file_operations ext4_seq_options_fops = {
	.owner = THIS_MODULE,
	.open = options_open_fs,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ext4_setup_super(struct super_block *sb, struct ext4_super_block *es,
			    int read_only)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
<<<<<<< HEAD
	int res = 0;
=======
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (le32_to_cpu(es->s_rev_level) > EXT4_MAX_SUPP_REV) {
		ext4_msg(sb, KERN_ERR, "revision level too high, "
			 "forcing read-only mode");
<<<<<<< HEAD
		res = MS_RDONLY;
=======
		err = -EROFS;
		goto done;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (read_only)
		goto done;
	if (!(sbi->s_mount_state & EXT4_VALID_FS))
		ext4_msg(sb, KERN_WARNING, "warning: mounting unchecked fs, "
			 "running e2fsck is recommended");
<<<<<<< HEAD
	else if ((sbi->s_mount_state & EXT4_ERROR_FS))
=======
	else if (sbi->s_mount_state & EXT4_ERROR_FS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ext4_msg(sb, KERN_WARNING,
			 "warning: mounting fs with errors, "
			 "running e2fsck is recommended");
	else if ((__s16) le16_to_cpu(es->s_max_mnt_count) > 0 &&
		 le16_to_cpu(es->s_mnt_count) >=
		 (unsigned short) (__s16) le16_to_cpu(es->s_max_mnt_count))
		ext4_msg(sb, KERN_WARNING,
			 "warning: maximal mount count reached, "
			 "running e2fsck is recommended");
	else if (le32_to_cpu(es->s_checkinterval) &&
<<<<<<< HEAD
		(le32_to_cpu(es->s_lastcheck) +
			le32_to_cpu(es->s_checkinterval) <= get_seconds()))
=======
		 (ext4_get_tstamp(es, s_lastcheck) +
		  le32_to_cpu(es->s_checkinterval) <= ktime_get_real_seconds()))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ext4_msg(sb, KERN_WARNING,
			 "warning: checktime reached, "
			 "running e2fsck is recommended");
	if (!sbi->s_journal)
		es->s_state &= cpu_to_le16(~EXT4_VALID_FS);
	if (!(__s16) le16_to_cpu(es->s_max_mnt_count))
		es->s_max_mnt_count = cpu_to_le16(EXT4_DFL_MAX_MNT_COUNT);
	le16_add_cpu(&es->s_mnt_count, 1);
<<<<<<< HEAD
	es->s_mtime = cpu_to_le32(get_seconds());
	ext4_update_dynamic_rev(sb);
	if (sbi->s_journal)
		EXT4_SET_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER);

	ext4_commit_super(sb, 1);
=======
	ext4_update_tstamp(es, s_mtime);
	if (sbi->s_journal) {
		ext4_set_feature_journal_needs_recovery(sb);
		if (ext4_has_feature_orphan_file(sb))
			ext4_set_feature_orphan_present(sb);
	}

	err = ext4_commit_super(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
done:
	if (test_opt(sb, DEBUG))
		printk(KERN_INFO "[EXT4 FS bs=%lu, gc=%u, "
				"bpg=%lu, ipg=%lu, mo=%04x, mo2=%04x]\n",
			sb->s_blocksize,
			sbi->s_groups_count,
			EXT4_BLOCKS_PER_GROUP(sb),
			EXT4_INODES_PER_GROUP(sb),
			sbi->s_mount_opt, sbi->s_mount_opt2);
<<<<<<< HEAD

	cleancache_init_fs(sb);
	return res;
=======
	return err;
}

int ext4_alloc_flex_bg_array(struct super_block *sb, ext4_group_t ngroup)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct flex_groups **old_groups, **new_groups;
	int size, i, j;

	if (!sbi->s_log_groups_per_flex)
		return 0;

	size = ext4_flex_group(sbi, ngroup - 1) + 1;
	if (size <= sbi->s_flex_groups_allocated)
		return 0;

	new_groups = kvzalloc(roundup_pow_of_two(size *
			      sizeof(*sbi->s_flex_groups)), GFP_KERNEL);
	if (!new_groups) {
		ext4_msg(sb, KERN_ERR,
			 "not enough memory for %d flex group pointers", size);
		return -ENOMEM;
	}
	for (i = sbi->s_flex_groups_allocated; i < size; i++) {
		new_groups[i] = kvzalloc(roundup_pow_of_two(
					 sizeof(struct flex_groups)),
					 GFP_KERNEL);
		if (!new_groups[i]) {
			for (j = sbi->s_flex_groups_allocated; j < i; j++)
				kvfree(new_groups[j]);
			kvfree(new_groups);
			ext4_msg(sb, KERN_ERR,
				 "not enough memory for %d flex groups", size);
			return -ENOMEM;
		}
	}
	rcu_read_lock();
	old_groups = rcu_dereference(sbi->s_flex_groups);
	if (old_groups)
		memcpy(new_groups, old_groups,
		       (sbi->s_flex_groups_allocated *
			sizeof(struct flex_groups *)));
	rcu_read_unlock();
	rcu_assign_pointer(sbi->s_flex_groups, new_groups);
	sbi->s_flex_groups_allocated = size;
	if (old_groups)
		ext4_kvfree_array_rcu(old_groups);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ext4_fill_flex_info(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_group_desc *gdp = NULL;
<<<<<<< HEAD
	ext4_group_t flex_group_count;
	ext4_group_t flex_group;
	unsigned int groups_per_flex = 0;
	size_t size;
	int i;
=======
	struct flex_groups *fg;
	ext4_group_t flex_group;
	int i, err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sbi->s_log_groups_per_flex = sbi->s_es->s_log_groups_per_flex;
	if (sbi->s_log_groups_per_flex < 1 || sbi->s_log_groups_per_flex > 31) {
		sbi->s_log_groups_per_flex = 0;
		return 1;
	}
<<<<<<< HEAD
	groups_per_flex = 1 << sbi->s_log_groups_per_flex;

	/* We allocate both existing and potentially added groups */
	flex_group_count = ((sbi->s_groups_count + groups_per_flex - 1) +
			((le16_to_cpu(sbi->s_es->s_reserved_gdt_blocks) + 1) <<
			      EXT4_DESC_PER_BLOCK_BITS(sb))) / groups_per_flex;
	size = flex_group_count * sizeof(struct flex_groups);
	sbi->s_flex_groups = ext4_kvzalloc(size, GFP_KERNEL);
	if (sbi->s_flex_groups == NULL) {
		ext4_msg(sb, KERN_ERR, "not enough memory for %u flex groups",
			 flex_group_count);
		goto failed;
	}
=======

	err = ext4_alloc_flex_bg_array(sb, sbi->s_groups_count);
	if (err)
		goto failed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < sbi->s_groups_count; i++) {
		gdp = ext4_get_group_desc(sb, i, NULL);

		flex_group = ext4_flex_group(sbi, i);
<<<<<<< HEAD
		atomic_add(ext4_free_inodes_count(sb, gdp),
			   &sbi->s_flex_groups[flex_group].free_inodes);
		atomic64_add(ext4_free_group_clusters(sb, gdp),
			     &sbi->s_flex_groups[flex_group].free_clusters);
		atomic_add(ext4_used_dirs_count(sb, gdp),
			   &sbi->s_flex_groups[flex_group].used_dirs);
=======
		fg = sbi_array_rcu_deref(sbi, s_flex_groups, flex_group);
		atomic_add(ext4_free_inodes_count(sb, gdp), &fg->free_inodes);
		atomic64_add(ext4_free_group_clusters(sb, gdp),
			     &fg->free_clusters);
		atomic_add(ext4_used_dirs_count(sb, gdp), &fg->used_dirs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 1;
failed:
	return 0;
}

<<<<<<< HEAD
static __le16 ext4_group_desc_csum(struct ext4_sb_info *sbi, __u32 block_group,
				   struct ext4_group_desc *gdp)
{
	int offset;
	__u16 crc = 0;
	__le32 le_group = cpu_to_le32(block_group);

	if ((sbi->s_es->s_feature_ro_compat &
	     cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM))) {
		/* Use new metadata_csum algorithm */
		__u16 old_csum;
		__u32 csum32;

		old_csum = gdp->bg_checksum;
		gdp->bg_checksum = 0;
		csum32 = ext4_chksum(sbi, sbi->s_csum_seed, (__u8 *)&le_group,
				     sizeof(le_group));
		csum32 = ext4_chksum(sbi, csum32, (__u8 *)gdp,
				     sbi->s_desc_size);
		gdp->bg_checksum = old_csum;
=======
static __le16 ext4_group_desc_csum(struct super_block *sb, __u32 block_group,
				   struct ext4_group_desc *gdp)
{
	int offset = offsetof(struct ext4_group_desc, bg_checksum);
	__u16 crc = 0;
	__le32 le_group = cpu_to_le32(block_group);
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	if (ext4_has_metadata_csum(sbi->s_sb)) {
		/* Use new metadata_csum algorithm */
		__u32 csum32;
		__u16 dummy_csum = 0;

		csum32 = ext4_chksum(sbi, sbi->s_csum_seed, (__u8 *)&le_group,
				     sizeof(le_group));
		csum32 = ext4_chksum(sbi, csum32, (__u8 *)gdp, offset);
		csum32 = ext4_chksum(sbi, csum32, (__u8 *)&dummy_csum,
				     sizeof(dummy_csum));
		offset += sizeof(dummy_csum);
		if (offset < sbi->s_desc_size)
			csum32 = ext4_chksum(sbi, csum32, (__u8 *)gdp + offset,
					     sbi->s_desc_size - offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		crc = csum32 & 0xFFFF;
		goto out;
	}

	/* old crc16 code */
<<<<<<< HEAD
	offset = offsetof(struct ext4_group_desc, bg_checksum);
=======
	if (!ext4_has_feature_gdt_csum(sb))
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	crc = crc16(~0, sbi->s_es->s_uuid, sizeof(sbi->s_es->s_uuid));
	crc = crc16(crc, (__u8 *)&le_group, sizeof(le_group));
	crc = crc16(crc, (__u8 *)gdp, offset);
	offset += sizeof(gdp->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
<<<<<<< HEAD
	if ((sbi->s_es->s_feature_incompat &
	     cpu_to_le32(EXT4_FEATURE_INCOMPAT_64BIT)) &&
	    offset < le16_to_cpu(sbi->s_es->s_desc_size))
		crc = crc16(crc, (__u8 *)gdp + offset,
			    le16_to_cpu(sbi->s_es->s_desc_size) -
				offset);
=======
	if (ext4_has_feature_64bit(sb) && offset < sbi->s_desc_size)
		crc = crc16(crc, (__u8 *)gdp + offset,
			    sbi->s_desc_size - offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	return cpu_to_le16(crc);
}

int ext4_group_desc_csum_verify(struct super_block *sb, __u32 block_group,
				struct ext4_group_desc *gdp)
{
	if (ext4_has_group_desc_csum(sb) &&
<<<<<<< HEAD
	    (gdp->bg_checksum != ext4_group_desc_csum(EXT4_SB(sb),
						      block_group, gdp)))
=======
	    (gdp->bg_checksum != ext4_group_desc_csum(sb, block_group, gdp)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	return 1;
}

void ext4_group_desc_csum_set(struct super_block *sb, __u32 block_group,
			      struct ext4_group_desc *gdp)
{
	if (!ext4_has_group_desc_csum(sb))
		return;
<<<<<<< HEAD
	gdp->bg_checksum = ext4_group_desc_csum(EXT4_SB(sb), block_group, gdp);
=======
	gdp->bg_checksum = ext4_group_desc_csum(sb, block_group, gdp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Called at mount-time, super-block is locked */
static int ext4_check_descriptors(struct super_block *sb,
				  ext4_fsblk_t sb_block,
				  ext4_group_t *first_not_zeroed)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	ext4_fsblk_t first_block = le32_to_cpu(sbi->s_es->s_first_data_block);
	ext4_fsblk_t last_block;
	ext4_fsblk_t last_bg_block = sb_block + ext4_bg_num_gdb(sb, 0);
	ext4_fsblk_t block_bitmap;
	ext4_fsblk_t inode_bitmap;
	ext4_fsblk_t inode_table;
	int flexbg_flag = 0;
	ext4_group_t i, grp = sbi->s_groups_count;

<<<<<<< HEAD
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_FLEX_BG))
=======
	if (ext4_has_feature_flex_bg(sb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		flexbg_flag = 1;

	ext4_debug("Checking group descriptors");

	for (i = 0; i < sbi->s_groups_count; i++) {
		struct ext4_group_desc *gdp = ext4_get_group_desc(sb, i, NULL);

		if (i == sbi->s_groups_count - 1 || flexbg_flag)
			last_block = ext4_blocks_count(sbi->s_es) - 1;
		else
			last_block = first_block +
				(EXT4_BLOCKS_PER_GROUP(sb) - 1);

		if ((grp == sbi->s_groups_count) &&
		   !(gdp->bg_flags & cpu_to_le16(EXT4_BG_INODE_ZEROED)))
			grp = i;

		block_bitmap = ext4_block_bitmap(sb, gdp);
		if (block_bitmap == sb_block) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
				 "Block bitmap for group %u overlaps "
				 "superblock", i);
<<<<<<< HEAD
=======
			if (!sb_rdonly(sb))
				return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		if (block_bitmap >= sb_block + 1 &&
		    block_bitmap <= last_bg_block) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
				 "Block bitmap for group %u overlaps "
				 "block group descriptors", i);
<<<<<<< HEAD
			if (!(sb->s_flags & MS_RDONLY))
=======
			if (!sb_rdonly(sb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return 0;
		}
		if (block_bitmap < first_block || block_bitmap > last_block) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
			       "Block bitmap for group %u not in group "
			       "(block %llu)!", i, block_bitmap);
			return 0;
		}
		inode_bitmap = ext4_inode_bitmap(sb, gdp);
		if (inode_bitmap == sb_block) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
				 "Inode bitmap for group %u overlaps "
				 "superblock", i);
<<<<<<< HEAD
=======
			if (!sb_rdonly(sb))
				return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		if (inode_bitmap >= sb_block + 1 &&
		    inode_bitmap <= last_bg_block) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
				 "Inode bitmap for group %u overlaps "
				 "block group descriptors", i);
<<<<<<< HEAD
			if (!(sb->s_flags & MS_RDONLY))
=======
			if (!sb_rdonly(sb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return 0;
		}
		if (inode_bitmap < first_block || inode_bitmap > last_block) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
			       "Inode bitmap for group %u not in group "
			       "(block %llu)!", i, inode_bitmap);
			return 0;
		}
		inode_table = ext4_inode_table(sb, gdp);
		if (inode_table == sb_block) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
				 "Inode table for group %u overlaps "
				 "superblock", i);
<<<<<<< HEAD
=======
			if (!sb_rdonly(sb))
				return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		if (inode_table >= sb_block + 1 &&
		    inode_table <= last_bg_block) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
				 "Inode table for group %u overlaps "
				 "block group descriptors", i);
<<<<<<< HEAD
			if (!(sb->s_flags & MS_RDONLY))
=======
			if (!sb_rdonly(sb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return 0;
		}
		if (inode_table < first_block ||
		    inode_table + sbi->s_itb_per_group - 1 > last_block) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
			       "Inode table for group %u not in group "
			       "(block %llu)!", i, inode_table);
			return 0;
		}
		ext4_lock_group(sb, i);
		if (!ext4_group_desc_csum_verify(sb, i, gdp)) {
			ext4_msg(sb, KERN_ERR, "ext4_check_descriptors: "
				 "Checksum for group %u failed (%u!=%u)",
<<<<<<< HEAD
				 i, le16_to_cpu(ext4_group_desc_csum(sbi, i,
				     gdp)), le16_to_cpu(gdp->bg_checksum));
			if (!(sb->s_flags & MS_RDONLY)) {
=======
				 i, le16_to_cpu(ext4_group_desc_csum(sb, i,
				     gdp)), le16_to_cpu(gdp->bg_checksum));
			if (!sb_rdonly(sb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				ext4_unlock_group(sb, i);
				return 0;
			}
		}
		ext4_unlock_group(sb, i);
		if (!flexbg_flag)
			first_block += EXT4_BLOCKS_PER_GROUP(sb);
	}
	if (NULL != first_not_zeroed)
		*first_not_zeroed = grp;
<<<<<<< HEAD

	ext4_free_blocks_count_set(sbi->s_es,
				   EXT4_C2B(sbi, ext4_count_free_clusters(sb)));
	sbi->s_es->s_free_inodes_count =cpu_to_le32(ext4_count_free_inodes(sb));
	return 1;
}

/* ext4_orphan_cleanup() walks a singly-linked list of inodes (starting at
 * the superblock) which were deleted from all directories, but held open by
 * a process at the time of a crash.  We walk the list and try to delete these
 * inodes at recovery time (only with a read-write filesystem).
 *
 * In order to keep the orphan inode chain consistent during traversal (in
 * case of crash during recovery), we link each inode into the superblock
 * orphan list_head and handle it the same way as an inode deletion during
 * normal operation (which journals the operations for us).
 *
 * We only do an iget() and an iput() on each inode, which is very safe if we
 * accidentally point at an in-use or already deleted inode.  The worst that
 * can happen in this case is that we get a "bit already cleared" message from
 * ext4_free_inode().  The only reason we would point at a wrong inode is if
 * e2fsck was run on this filesystem, and it must have already done the orphan
 * inode cleanup for us, so we can safely abort without any further action.
 */
static void ext4_orphan_cleanup(struct super_block *sb,
				struct ext4_super_block *es)
{
	unsigned int s_flags = sb->s_flags;
	int nr_orphans = 0, nr_truncates = 0;
#ifdef CONFIG_QUOTA
	int i;
#endif
	if (!es->s_last_orphan) {
		jbd_debug(4, "no orphan inodes to clean up\n");
		return;
	}

	if (bdev_read_only(sb->s_bdev)) {
		ext4_msg(sb, KERN_ERR, "write access "
			"unavailable, skipping orphan cleanup");
		return;
	}

	/* Check if feature set would not allow a r/w mount */
	if (!ext4_feature_set_ok(sb, 0)) {
		ext4_msg(sb, KERN_INFO, "Skipping orphan cleanup due to "
			 "unknown ROCOMPAT features");
		return;
	}

	if (EXT4_SB(sb)->s_mount_state & EXT4_ERROR_FS) {
		if (es->s_last_orphan)
			jbd_debug(1, "Errors on filesystem, "
				  "clearing orphan list.\n");
		es->s_last_orphan = 0;
		jbd_debug(1, "Skipping orphan recovery on fs with errors.\n");
		return;
	}

	if (s_flags & MS_RDONLY) {
		ext4_msg(sb, KERN_INFO, "orphan cleanup on readonly fs");
		sb->s_flags &= ~MS_RDONLY;
	}
#ifdef CONFIG_QUOTA
	/* Needed for iput() to work correctly and not trash data */
	sb->s_flags |= MS_ACTIVE;
	/* Turn on quotas so that they are updated correctly */
	for (i = 0; i < MAXQUOTAS; i++) {
		if (EXT4_SB(sb)->s_qf_names[i]) {
			int ret = ext4_quota_on_mount(sb, i);
			if (ret < 0)
				ext4_msg(sb, KERN_ERR,
					"Cannot turn on journaled "
					"quota: error %d", ret);
		}
	}
#endif

	while (es->s_last_orphan) {
		struct inode *inode;

		inode = ext4_orphan_get(sb, le32_to_cpu(es->s_last_orphan));
		if (IS_ERR(inode)) {
			es->s_last_orphan = 0;
			break;
		}

		list_add(&EXT4_I(inode)->i_orphan, &EXT4_SB(sb)->s_orphan);
		dquot_initialize(inode);
		if (inode->i_nlink) {
			ext4_msg(sb, KERN_DEBUG,
				"%s: truncating inode %lu to %lld bytes",
				__func__, inode->i_ino, inode->i_size);
			jbd_debug(2, "truncating inode %lu to %lld bytes\n",
				  inode->i_ino, inode->i_size);
			mutex_lock(&inode->i_mutex);
			ext4_truncate(inode);
			mutex_unlock(&inode->i_mutex);
			nr_truncates++;
		} else {
			ext4_msg(sb, KERN_DEBUG,
				"%s: deleting unreferenced inode %lu",
				__func__, inode->i_ino);
			jbd_debug(2, "deleting unreferenced inode %lu\n",
				  inode->i_ino);
			nr_orphans++;
		}
		iput(inode);  /* The delete magic happens here! */
	}

#define PLURAL(x) (x), ((x) == 1) ? "" : "s"

	if (nr_orphans)
		ext4_msg(sb, KERN_INFO, "%d orphan inode%s deleted",
		       PLURAL(nr_orphans));
	if (nr_truncates)
		ext4_msg(sb, KERN_INFO, "%d truncate%s cleaned up",
		       PLURAL(nr_truncates));
#ifdef CONFIG_QUOTA
	/* Turn quotas off */
	for (i = 0; i < MAXQUOTAS; i++) {
		if (sb_dqopt(sb)->files[i])
			dquot_quota_off(sb, i);
	}
#endif
	sb->s_flags = s_flags; /* Restore MS_RDONLY status */
}

=======
	return 1;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Maximal extent format file size.
 * Resulting logical blkno at s_maxbytes must fit in our on-disk
 * extent format containers, within a sector_t, and within i_blocks
 * in the vfs.  ext4 inode has 48 bits of i_block in fsblock units,
 * so that won't be a limiting factor.
 *
 * However there is other limiting factor. We do store extents in the form
 * of starting block and length, hence the resulting length of the extent
 * covering maximum file size must fit into on-disk format containers as
 * well. Given that length is always by 1 unit bigger than max unit (because
 * we count 0 as well) we have to lower the s_maxbytes by one fs block.
 *
 * Note, this does *not* consider any metadata overhead for vfs i_blocks.
 */
static loff_t ext4_max_size(int blkbits, int has_huge_files)
{
	loff_t res;
	loff_t upper_limit = MAX_LFS_FILESIZE;

<<<<<<< HEAD
	/* small i_blocks in vfs inode? */
	if (!has_huge_files || sizeof(blkcnt_t) < sizeof(u64)) {
		/*
		 * CONFIG_LBDAF is not enabled implies the inode
		 * i_block represent total blocks in 512 bytes
		 * 32 == size of vfs inode i_blocks * 8
		 */
=======
	BUILD_BUG_ON(sizeof(blkcnt_t) < sizeof(u64));

	if (!has_huge_files) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		upper_limit = (1LL << 32) - 1;

		/* total blocks in file system block size */
		upper_limit >>= (blkbits - 9);
		upper_limit <<= blkbits;
	}

	/*
	 * 32-bit extent-start container, ee_block. We lower the maxbytes
	 * by one fs block, so ee_len can cover the extent of maximum file
	 * size
	 */
	res = (1LL << 32) - 1;
	res <<= blkbits;

	/* Sanity check against vm- & vfs- imposed limits */
	if (res > upper_limit)
		res = upper_limit;

	return res;
}

/*
 * Maximal bitmap file size.  There is a direct, and {,double-,triple-}indirect
 * block limit, and also a limit of (2^48 - 1) 512-byte sectors in i_blocks.
 * We need to be 1 filesystem block less than the 2^48 sector limit.
 */
static loff_t ext4_max_bitmap_size(int bits, int has_huge_files)
{
<<<<<<< HEAD
	loff_t res = EXT4_NDIR_BLOCKS;
	int meta_blocks;
	loff_t upper_limit;
	/* This is calculated to be the largest file size for a dense, block
=======
	loff_t upper_limit, res = EXT4_NDIR_BLOCKS;
	int meta_blocks;
	unsigned int ppb = 1 << (bits - 2);

	/*
	 * This is calculated to be the largest file size for a dense, block
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * mapped file such that the file's total number of 512-byte sectors,
	 * including data and all indirect blocks, does not exceed (2^48 - 1).
	 *
	 * __u32 i_blocks_lo and _u16 i_blocks_high represent the total
	 * number of 512-byte sectors of the file.
	 */
<<<<<<< HEAD

	if (!has_huge_files || sizeof(blkcnt_t) < sizeof(u64)) {
		/*
		 * !has_huge_files or CONFIG_LBDAF not enabled implies that
		 * the inode i_block field represents total file blocks in
		 * 2^32 512-byte sectors == size of vfs inode i_blocks * 8
=======
	if (!has_huge_files) {
		/*
		 * !has_huge_files or implies that the inode i_block field
		 * represents total file blocks in 2^32 512-byte sectors ==
		 * size of vfs inode i_blocks * 8
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 */
		upper_limit = (1LL << 32) - 1;

		/* total blocks in file system block size */
		upper_limit >>= (bits - 9);

	} else {
		/*
		 * We use 48 bit ext4_inode i_blocks
		 * With EXT4_HUGE_FILE_FL set the i_blocks
		 * represent total number of blocks in
		 * file system block size
		 */
		upper_limit = (1LL << 48) - 1;

	}

<<<<<<< HEAD
	/* indirect blocks */
	meta_blocks = 1;
	/* double indirect blocks */
	meta_blocks += 1 + (1LL << (bits-2));
	/* tripple indirect blocks */
	meta_blocks += 1 + (1LL << (bits-2)) + (1LL << (2*(bits-2)));

	upper_limit -= meta_blocks;
	upper_limit <<= bits;

	res += 1LL << (bits-2);
	res += 1LL << (2*(bits-2));
	res += 1LL << (3*(bits-2));
	res <<= bits;
	if (res > upper_limit)
		res = upper_limit;

=======
	/* Compute how many blocks we can address by block tree */
	res += ppb;
	res += ppb * ppb;
	res += ((loff_t)ppb) * ppb * ppb;
	/* Compute how many metadata blocks are needed */
	meta_blocks = 1;
	meta_blocks += 1 + ppb;
	meta_blocks += 1 + ppb + ppb * ppb;
	/* Does block tree limit file size? */
	if (res + meta_blocks <= upper_limit)
		goto check_lfs;

	res = upper_limit;
	/* How many metadata blocks are needed for addressing upper_limit? */
	upper_limit -= EXT4_NDIR_BLOCKS;
	/* indirect blocks */
	meta_blocks = 1;
	upper_limit -= ppb;
	/* double indirect blocks */
	if (upper_limit < ppb * ppb) {
		meta_blocks += 1 + DIV_ROUND_UP_ULL(upper_limit, ppb);
		res -= meta_blocks;
		goto check_lfs;
	}
	meta_blocks += 1 + ppb;
	upper_limit -= ppb * ppb;
	/* tripple indirect blocks for the rest */
	meta_blocks += 1 + DIV_ROUND_UP_ULL(upper_limit, ppb) +
		DIV_ROUND_UP_ULL(upper_limit, ppb*ppb);
	res -= meta_blocks;
check_lfs:
	res <<= bits;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (res > MAX_LFS_FILESIZE)
		res = MAX_LFS_FILESIZE;

	return res;
}

static ext4_fsblk_t descriptor_loc(struct super_block *sb,
				   ext4_fsblk_t logical_sb_block, int nr)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	ext4_group_t bg, first_meta_bg;
	int has_super = 0;

	first_meta_bg = le32_to_cpu(sbi->s_es->s_first_meta_bg);

<<<<<<< HEAD
	if (!EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_META_BG) ||
	    nr < first_meta_bg)
=======
	if (!ext4_has_feature_meta_bg(sb) || nr < first_meta_bg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return logical_sb_block + nr + 1;
	bg = sbi->s_desc_per_block * nr;
	if (ext4_bg_has_super(sb, bg))
		has_super = 1;

<<<<<<< HEAD
=======
	/*
	 * If we have a meta_bg fs with 1k blocks, group 0's GDT is at
	 * block 2, not 1.  If s_first_data_block == 0 (bigalloc is enabled
	 * on modern mke2fs or blksize > 1k on older mke2fs) then we must
	 * compensate.
	 */
	if (sb->s_blocksize == 1024 && nr == 0 &&
	    le32_to_cpu(sbi->s_es->s_first_data_block) == 0)
		has_super++;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (has_super + ext4_group_first_block_no(sb, bg));
}

/**
 * ext4_get_stripe_size: Get the stripe size.
 * @sbi: In memory super block info
 *
 * If we have specified it via mount option, then
 * use the mount option value. If the value specified at mount time is
 * greater than the blocks per group use the super block value.
 * If the super block value is greater than blocks per group return 0.
 * Allocator needs it be less than blocks per group.
 *
 */
static unsigned long ext4_get_stripe_size(struct ext4_sb_info *sbi)
{
	unsigned long stride = le16_to_cpu(sbi->s_es->s_raid_stride);
	unsigned long stripe_width =
			le32_to_cpu(sbi->s_es->s_raid_stripe_width);
	int ret;

	if (sbi->s_stripe && sbi->s_stripe <= sbi->s_blocks_per_group)
		ret = sbi->s_stripe;
<<<<<<< HEAD
	else if (stripe_width <= sbi->s_blocks_per_group)
		ret = stripe_width;
	else if (stride <= sbi->s_blocks_per_group)
=======
	else if (stripe_width && stripe_width <= sbi->s_blocks_per_group)
		ret = stripe_width;
	else if (stride && stride <= sbi->s_blocks_per_group)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = stride;
	else
		ret = 0;

	/*
	 * If the stripe width is 1, this makes no sense and
	 * we set it to 0 to turn off stripe handling code.
	 */
	if (ret <= 1)
		ret = 0;

	return ret;
}

<<<<<<< HEAD
/* sysfs supprt */

struct ext4_attr {
	struct attribute attr;
	ssize_t (*show)(struct ext4_attr *, struct ext4_sb_info *, char *);
	ssize_t (*store)(struct ext4_attr *, struct ext4_sb_info *,
			 const char *, size_t);
	int offset;
};

static int parse_strtoul(const char *buf,
		unsigned long max, unsigned long *value)
{
	char *endp;

	*value = simple_strtoul(skip_spaces(buf), &endp, 0);
	endp = skip_spaces(endp);
	if (*endp || *value > max)
		return -EINVAL;

	return 0;
}

static ssize_t delayed_allocation_blocks_show(struct ext4_attr *a,
					      struct ext4_sb_info *sbi,
					      char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%llu\n",
		(s64) EXT4_C2B(sbi,
			percpu_counter_sum(&sbi->s_dirtyclusters_counter)));
}

static ssize_t session_write_kbytes_show(struct ext4_attr *a,
					 struct ext4_sb_info *sbi, char *buf)
{
	struct super_block *sb = sbi->s_buddy_cache->i_sb;

	if (!sb->s_bdev->bd_part)
		return snprintf(buf, PAGE_SIZE, "0\n");
	return snprintf(buf, PAGE_SIZE, "%lu\n",
			(part_stat_read(sb->s_bdev->bd_part, sectors[1]) -
			 sbi->s_sectors_written_start) >> 1);
}

static ssize_t lifetime_write_kbytes_show(struct ext4_attr *a,
					  struct ext4_sb_info *sbi, char *buf)
{
	struct super_block *sb = sbi->s_buddy_cache->i_sb;

	if (!sb->s_bdev->bd_part)
		return snprintf(buf, PAGE_SIZE, "0\n");
	return snprintf(buf, PAGE_SIZE, "%llu\n",
			(unsigned long long)(sbi->s_kbytes_written +
			((part_stat_read(sb->s_bdev->bd_part, sectors[1]) -
			  EXT4_SB(sb)->s_sectors_written_start) >> 1)));
}

static ssize_t inode_readahead_blks_store(struct ext4_attr *a,
					  struct ext4_sb_info *sbi,
					  const char *buf, size_t count)
{
	unsigned long t;

	if (parse_strtoul(buf, 0x40000000, &t))
		return -EINVAL;

	if (t && !is_power_of_2(t))
		return -EINVAL;

	sbi->s_inode_readahead_blks = t;
	return count;
}

static ssize_t sbi_ui_show(struct ext4_attr *a,
			   struct ext4_sb_info *sbi, char *buf)
{
	unsigned int *ui = (unsigned int *) (((char *) sbi) + a->offset);

	return snprintf(buf, PAGE_SIZE, "%u\n", *ui);
}

static ssize_t sbi_ui_store(struct ext4_attr *a,
			    struct ext4_sb_info *sbi,
			    const char *buf, size_t count)
{
	unsigned int *ui = (unsigned int *) (((char *) sbi) + a->offset);
	unsigned long t;

	if (parse_strtoul(buf, 0xffffffff, &t))
		return -EINVAL;
	*ui = t;
	return count;
}

#define EXT4_ATTR_OFFSET(_name,_mode,_show,_store,_elname) \
static struct ext4_attr ext4_attr_##_name = {			\
	.attr = {.name = __stringify(_name), .mode = _mode },	\
	.show	= _show,					\
	.store	= _store,					\
	.offset = offsetof(struct ext4_sb_info, _elname),	\
}
#define EXT4_ATTR(name, mode, show, store) \
static struct ext4_attr ext4_attr_##name = __ATTR(name, mode, show, store)

#define EXT4_INFO_ATTR(name) EXT4_ATTR(name, 0444, NULL, NULL)
#define EXT4_RO_ATTR(name) EXT4_ATTR(name, 0444, name##_show, NULL)
#define EXT4_RW_ATTR(name) EXT4_ATTR(name, 0644, name##_show, name##_store)
#define EXT4_RW_ATTR_SBI_UI(name, elname)	\
	EXT4_ATTR_OFFSET(name, 0644, sbi_ui_show, sbi_ui_store, elname)
#define ATTR_LIST(name) &ext4_attr_##name.attr

EXT4_RO_ATTR(delayed_allocation_blocks);
EXT4_RO_ATTR(session_write_kbytes);
EXT4_RO_ATTR(lifetime_write_kbytes);
EXT4_ATTR_OFFSET(inode_readahead_blks, 0644, sbi_ui_show,
		 inode_readahead_blks_store, s_inode_readahead_blks);
EXT4_RW_ATTR_SBI_UI(inode_goal, s_inode_goal);
EXT4_RW_ATTR_SBI_UI(mb_stats, s_mb_stats);
EXT4_RW_ATTR_SBI_UI(mb_max_to_scan, s_mb_max_to_scan);
EXT4_RW_ATTR_SBI_UI(mb_min_to_scan, s_mb_min_to_scan);
EXT4_RW_ATTR_SBI_UI(mb_order2_req, s_mb_order2_reqs);
EXT4_RW_ATTR_SBI_UI(mb_stream_req, s_mb_stream_request);
EXT4_RW_ATTR_SBI_UI(mb_group_prealloc, s_mb_group_prealloc);
EXT4_RW_ATTR_SBI_UI(max_writeback_mb_bump, s_max_writeback_mb_bump);

static struct attribute *ext4_attrs[] = {
	ATTR_LIST(delayed_allocation_blocks),
	ATTR_LIST(session_write_kbytes),
	ATTR_LIST(lifetime_write_kbytes),
	ATTR_LIST(inode_readahead_blks),
	ATTR_LIST(inode_goal),
	ATTR_LIST(mb_stats),
	ATTR_LIST(mb_max_to_scan),
	ATTR_LIST(mb_min_to_scan),
	ATTR_LIST(mb_order2_req),
	ATTR_LIST(mb_stream_req),
	ATTR_LIST(mb_group_prealloc),
	ATTR_LIST(max_writeback_mb_bump),
	NULL,
};

/* Features this copy of ext4 supports */
EXT4_INFO_ATTR(lazy_itable_init);
EXT4_INFO_ATTR(batched_discard);

static struct attribute *ext4_feat_attrs[] = {
	ATTR_LIST(lazy_itable_init),
	ATTR_LIST(batched_discard),
	NULL,
};

static ssize_t ext4_attr_show(struct kobject *kobj,
			      struct attribute *attr, char *buf)
{
	struct ext4_sb_info *sbi = container_of(kobj, struct ext4_sb_info,
						s_kobj);
	struct ext4_attr *a = container_of(attr, struct ext4_attr, attr);

	return a->show ? a->show(a, sbi, buf) : 0;
}

static ssize_t ext4_attr_store(struct kobject *kobj,
			       struct attribute *attr,
			       const char *buf, size_t len)
{
	struct ext4_sb_info *sbi = container_of(kobj, struct ext4_sb_info,
						s_kobj);
	struct ext4_attr *a = container_of(attr, struct ext4_attr, attr);

	return a->store ? a->store(a, sbi, buf, len) : 0;
}

static void ext4_sb_release(struct kobject *kobj)
{
	struct ext4_sb_info *sbi = container_of(kobj, struct ext4_sb_info,
						s_kobj);
	complete(&sbi->s_kobj_unregister);
}

static const struct sysfs_ops ext4_attr_ops = {
	.show	= ext4_attr_show,
	.store	= ext4_attr_store,
};

static struct kobj_type ext4_ktype = {
	.default_attrs	= ext4_attrs,
	.sysfs_ops	= &ext4_attr_ops,
	.release	= ext4_sb_release,
};

static void ext4_feat_release(struct kobject *kobj)
{
	complete(&ext4_feat->f_kobj_unregister);
}

static struct kobj_type ext4_feat_ktype = {
	.default_attrs	= ext4_feat_attrs,
	.sysfs_ops	= &ext4_attr_ops,
	.release	= ext4_feat_release,
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Check whether this filesystem can be mounted based on
 * the features present and the RDONLY/RDWR mount requested.
 * Returns 1 if this filesystem can be mounted as requested,
 * 0 if it cannot be.
 */
<<<<<<< HEAD
static int ext4_feature_set_ok(struct super_block *sb, int readonly)
{
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, ~EXT4_FEATURE_INCOMPAT_SUPP)) {
=======
int ext4_feature_set_ok(struct super_block *sb, int readonly)
{
	if (ext4_has_unknown_ext4_incompat_features(sb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ext4_msg(sb, KERN_ERR,
			"Couldn't mount because of "
			"unsupported optional features (%x)",
			(le32_to_cpu(EXT4_SB(sb)->s_es->s_feature_incompat) &
			~EXT4_FEATURE_INCOMPAT_SUPP));
		return 0;
	}

<<<<<<< HEAD
	if (readonly)
		return 1;

	/* Check that feature set is OK for a read-write mount */
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, ~EXT4_FEATURE_RO_COMPAT_SUPP)) {
=======
#if !IS_ENABLED(CONFIG_UNICODE)
	if (ext4_has_feature_casefold(sb)) {
		ext4_msg(sb, KERN_ERR,
			 "Filesystem with casefold feature cannot be "
			 "mounted without CONFIG_UNICODE");
		return 0;
	}
#endif

	if (readonly)
		return 1;

	if (ext4_has_feature_readonly(sb)) {
		ext4_msg(sb, KERN_INFO, "filesystem is read-only");
		sb->s_flags |= SB_RDONLY;
		return 1;
	}

	/* Check that feature set is OK for a read-write mount */
	if (ext4_has_unknown_ext4_ro_compat_features(sb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ext4_msg(sb, KERN_ERR, "couldn't mount RDWR because of "
			 "unsupported optional features (%x)",
			 (le32_to_cpu(EXT4_SB(sb)->s_es->s_feature_ro_compat) &
				~EXT4_FEATURE_RO_COMPAT_SUPP));
		return 0;
	}
<<<<<<< HEAD
	/*
	 * Large file size enabled file system can only be mounted
	 * read-write on 32-bit systems if kernel is built with CONFIG_LBDAF
	 */
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, EXT4_FEATURE_RO_COMPAT_HUGE_FILE)) {
		if (sizeof(blkcnt_t) < sizeof(u64)) {
			ext4_msg(sb, KERN_ERR, "Filesystem with huge files "
				 "cannot be mounted RDWR without "
				 "CONFIG_LBDAF");
			return 0;
		}
	}
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, EXT4_FEATURE_RO_COMPAT_BIGALLOC) &&
	    !EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_EXTENTS)) {
=======
	if (ext4_has_feature_bigalloc(sb) && !ext4_has_feature_extents(sb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ext4_msg(sb, KERN_ERR,
			 "Can't support bigalloc feature without "
			 "extents feature\n");
		return 0;
	}
<<<<<<< HEAD
=======

#if !IS_ENABLED(CONFIG_QUOTA) || !IS_ENABLED(CONFIG_QFMT_V2)
	if (!readonly && (ext4_has_feature_quota(sb) ||
			  ext4_has_feature_project(sb))) {
		ext4_msg(sb, KERN_ERR,
			 "The kernel was not built with CONFIG_QUOTA and CONFIG_QFMT_V2");
		return 0;
	}
#endif  /* CONFIG_QUOTA */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}

/*
 * This function is called once a day if we have errors logged
 * on the file system
 */
<<<<<<< HEAD
static void print_daily_error_info(unsigned long arg)
{
	struct super_block *sb = (struct super_block *) arg;
	struct ext4_sb_info *sbi;
	struct ext4_super_block *es;

	sbi = EXT4_SB(sb);
	es = sbi->s_es;
=======
static void print_daily_error_info(struct timer_list *t)
{
	struct ext4_sb_info *sbi = from_timer(sbi, t, s_err_report);
	struct super_block *sb = sbi->s_sb;
	struct ext4_super_block *es = sbi->s_es;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (es->s_error_count)
		/* fsck newer than v1.41.13 is needed to clean this condition. */
		ext4_msg(sb, KERN_NOTICE, "error count since last fsck: %u",
			 le32_to_cpu(es->s_error_count));
	if (es->s_first_error_time) {
<<<<<<< HEAD
		printk(KERN_NOTICE "EXT4-fs (%s): initial error at time %u: %.*s:%d",
		       sb->s_id, le32_to_cpu(es->s_first_error_time),
=======
		printk(KERN_NOTICE "EXT4-fs (%s): initial error at time %llu: %.*s:%d",
		       sb->s_id,
		       ext4_get_tstamp(es, s_first_error_time),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       (int) sizeof(es->s_first_error_func),
		       es->s_first_error_func,
		       le32_to_cpu(es->s_first_error_line));
		if (es->s_first_error_ino)
<<<<<<< HEAD
			printk(": inode %u",
			       le32_to_cpu(es->s_first_error_ino));
		if (es->s_first_error_block)
			printk(": block %llu", (unsigned long long)
			       le64_to_cpu(es->s_first_error_block));
		printk("\n");
	}
	if (es->s_last_error_time) {
		printk(KERN_NOTICE "EXT4-fs (%s): last error at time %u: %.*s:%d",
		       sb->s_id, le32_to_cpu(es->s_last_error_time),
=======
			printk(KERN_CONT ": inode %u",
			       le32_to_cpu(es->s_first_error_ino));
		if (es->s_first_error_block)
			printk(KERN_CONT ": block %llu", (unsigned long long)
			       le64_to_cpu(es->s_first_error_block));
		printk(KERN_CONT "\n");
	}
	if (es->s_last_error_time) {
		printk(KERN_NOTICE "EXT4-fs (%s): last error at time %llu: %.*s:%d",
		       sb->s_id,
		       ext4_get_tstamp(es, s_last_error_time),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       (int) sizeof(es->s_last_error_func),
		       es->s_last_error_func,
		       le32_to_cpu(es->s_last_error_line));
		if (es->s_last_error_ino)
<<<<<<< HEAD
			printk(": inode %u",
			       le32_to_cpu(es->s_last_error_ino));
		if (es->s_last_error_block)
			printk(": block %llu", (unsigned long long)
			       le64_to_cpu(es->s_last_error_block));
		printk("\n");
=======
			printk(KERN_CONT ": inode %u",
			       le32_to_cpu(es->s_last_error_ino));
		if (es->s_last_error_block)
			printk(KERN_CONT ": block %llu", (unsigned long long)
			       le64_to_cpu(es->s_last_error_block));
		printk(KERN_CONT "\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mod_timer(&sbi->s_err_report, jiffies + 24*60*60*HZ);  /* Once a day */
}

/* Find next suitable group and run ext4_init_inode_table */
static int ext4_run_li_request(struct ext4_li_request *elr)
{
	struct ext4_group_desc *gdp = NULL;
<<<<<<< HEAD
	ext4_group_t group, ngroups;
	struct super_block *sb;
	unsigned long timeout = 0;
	int ret = 0;

	sb = elr->lr_super;
	ngroups = EXT4_SB(sb)->s_groups_count;

	for (group = elr->lr_next_group; group < ngroups; group++) {
=======
	struct super_block *sb = elr->lr_super;
	ext4_group_t ngroups = EXT4_SB(sb)->s_groups_count;
	ext4_group_t group = elr->lr_next_group;
	unsigned int prefetch_ios = 0;
	int ret = 0;
	int nr = EXT4_SB(sb)->s_mb_prefetch;
	u64 start_time;

	if (elr->lr_mode == EXT4_LI_MODE_PREFETCH_BBITMAP) {
		elr->lr_next_group = ext4_mb_prefetch(sb, group, nr, &prefetch_ios);
		ext4_mb_prefetch_fini(sb, elr->lr_next_group, nr);
		trace_ext4_prefetch_bitmaps(sb, group, elr->lr_next_group, nr);
		if (group >= elr->lr_next_group) {
			ret = 1;
			if (elr->lr_first_not_zeroed != ngroups &&
			    !sb_rdonly(sb) && test_opt(sb, INIT_INODE_TABLE)) {
				elr->lr_next_group = elr->lr_first_not_zeroed;
				elr->lr_mode = EXT4_LI_MODE_ITABLE;
				ret = 0;
			}
		}
		return ret;
	}

	for (; group < ngroups; group++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		gdp = ext4_get_group_desc(sb, group, NULL);
		if (!gdp) {
			ret = 1;
			break;
		}

		if (!(gdp->bg_flags & cpu_to_le16(EXT4_BG_INODE_ZEROED)))
			break;
	}

<<<<<<< HEAD
	if (group == ngroups)
		ret = 1;

	if (!ret) {
		timeout = jiffies;
		ret = ext4_init_inode_table(sb, group,
					    elr->lr_timeout ? 0 : 1);
		if (elr->lr_timeout == 0) {
			timeout = (jiffies - timeout) *
				  elr->lr_sbi->s_li_wait_mult;
			elr->lr_timeout = timeout;
=======
	if (group >= ngroups)
		ret = 1;

	if (!ret) {
		start_time = ktime_get_real_ns();
		ret = ext4_init_inode_table(sb, group,
					    elr->lr_timeout ? 0 : 1);
		trace_ext4_lazy_itable_init(sb, group);
		if (elr->lr_timeout == 0) {
			elr->lr_timeout = nsecs_to_jiffies((ktime_get_real_ns() - start_time) *
				EXT4_SB(elr->lr_super)->s_li_wait_mult);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		elr->lr_next_sched = jiffies + elr->lr_timeout;
		elr->lr_next_group = group + 1;
	}
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * Remove lr_request from the list_request and free the
 * request structure. Should be called with li_list_mtx held
 */
static void ext4_remove_li_request(struct ext4_li_request *elr)
{
<<<<<<< HEAD
	struct ext4_sb_info *sbi;

	if (!elr)
		return;

	sbi = elr->lr_sbi;

	list_del(&elr->lr_request);
	sbi->s_li_request = NULL;
=======
	if (!elr)
		return;

	list_del(&elr->lr_request);
	EXT4_SB(elr->lr_super)->s_li_request = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(elr);
}

static void ext4_unregister_li_request(struct super_block *sb)
{
	mutex_lock(&ext4_li_mtx);
	if (!ext4_li_info) {
		mutex_unlock(&ext4_li_mtx);
		return;
	}

	mutex_lock(&ext4_li_info->li_list_mtx);
	ext4_remove_li_request(EXT4_SB(sb)->s_li_request);
	mutex_unlock(&ext4_li_info->li_list_mtx);
	mutex_unlock(&ext4_li_mtx);
}

static struct task_struct *ext4_lazyinit_task;

/*
 * This is the function where ext4lazyinit thread lives. It walks
 * through the request list searching for next scheduled filesystem.
 * When such a fs is found, run the lazy initialization request
 * (ext4_rn_li_request) and keep track of the time spend in this
 * function. Based on that time we compute next schedule time of
 * the request. When walking through the list is complete, compute
 * next waking time and put itself into sleep.
 */
static int ext4_lazyinit_thread(void *arg)
{
<<<<<<< HEAD
	struct ext4_lazy_init *eli = (struct ext4_lazy_init *)arg;
=======
	struct ext4_lazy_init *eli = arg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head *pos, *n;
	struct ext4_li_request *elr;
	unsigned long next_wakeup, cur;

	BUG_ON(NULL == eli);
<<<<<<< HEAD
=======
	set_freezable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

cont_thread:
	while (true) {
		next_wakeup = MAX_JIFFY_OFFSET;

		mutex_lock(&eli->li_list_mtx);
		if (list_empty(&eli->li_request_list)) {
			mutex_unlock(&eli->li_list_mtx);
			goto exit_thread;
		}
<<<<<<< HEAD

		list_for_each_safe(pos, n, &eli->li_request_list) {
			elr = list_entry(pos, struct ext4_li_request,
					 lr_request);

			if (time_after_eq(jiffies, elr->lr_next_sched)) {
				if (ext4_run_li_request(elr) != 0) {
					/* error, remove the lazy_init job */
					ext4_remove_li_request(elr);
					continue;
				}
			}

=======
		list_for_each_safe(pos, n, &eli->li_request_list) {
			int err = 0;
			int progress = 0;
			elr = list_entry(pos, struct ext4_li_request,
					 lr_request);

			if (time_before(jiffies, elr->lr_next_sched)) {
				if (time_before(elr->lr_next_sched, next_wakeup))
					next_wakeup = elr->lr_next_sched;
				continue;
			}
			if (down_read_trylock(&elr->lr_super->s_umount)) {
				if (sb_start_write_trylock(elr->lr_super)) {
					progress = 1;
					/*
					 * We hold sb->s_umount, sb can not
					 * be removed from the list, it is
					 * now safe to drop li_list_mtx
					 */
					mutex_unlock(&eli->li_list_mtx);
					err = ext4_run_li_request(elr);
					sb_end_write(elr->lr_super);
					mutex_lock(&eli->li_list_mtx);
					n = pos->next;
				}
				up_read((&elr->lr_super->s_umount));
			}
			/* error, remove the lazy_init job */
			if (err) {
				ext4_remove_li_request(elr);
				continue;
			}
			if (!progress) {
				elr->lr_next_sched = jiffies +
					get_random_u32_below(EXT4_DEF_LI_MAX_START_DELAY * HZ);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (time_before(elr->lr_next_sched, next_wakeup))
				next_wakeup = elr->lr_next_sched;
		}
		mutex_unlock(&eli->li_list_mtx);

		try_to_freeze();

		cur = jiffies;
		if ((time_after_eq(cur, next_wakeup)) ||
		    (MAX_JIFFY_OFFSET == next_wakeup)) {
			cond_resched();
			continue;
		}

		schedule_timeout_interruptible(next_wakeup - cur);

		if (kthread_should_stop()) {
			ext4_clear_request_list();
			goto exit_thread;
		}
	}

exit_thread:
	/*
	 * It looks like the request list is empty, but we need
	 * to check it under the li_list_mtx lock, to prevent any
	 * additions into it, and of course we should lock ext4_li_mtx
	 * to atomically free the list and ext4_li_info, because at
	 * this point another ext4 filesystem could be registering
	 * new one.
	 */
	mutex_lock(&ext4_li_mtx);
	mutex_lock(&eli->li_list_mtx);
	if (!list_empty(&eli->li_request_list)) {
		mutex_unlock(&eli->li_list_mtx);
		mutex_unlock(&ext4_li_mtx);
		goto cont_thread;
	}
	mutex_unlock(&eli->li_list_mtx);
	kfree(ext4_li_info);
	ext4_li_info = NULL;
	mutex_unlock(&ext4_li_mtx);

	return 0;
}

static void ext4_clear_request_list(void)
{
	struct list_head *pos, *n;
	struct ext4_li_request *elr;

	mutex_lock(&ext4_li_info->li_list_mtx);
	list_for_each_safe(pos, n, &ext4_li_info->li_request_list) {
		elr = list_entry(pos, struct ext4_li_request,
				 lr_request);
		ext4_remove_li_request(elr);
	}
	mutex_unlock(&ext4_li_info->li_list_mtx);
}

static int ext4_run_lazyinit_thread(void)
{
	ext4_lazyinit_task = kthread_run(ext4_lazyinit_thread,
					 ext4_li_info, "ext4lazyinit");
	if (IS_ERR(ext4_lazyinit_task)) {
		int err = PTR_ERR(ext4_lazyinit_task);
		ext4_clear_request_list();
		kfree(ext4_li_info);
		ext4_li_info = NULL;
		printk(KERN_CRIT "EXT4-fs: error %d creating inode table "
				 "initialization thread\n",
				 err);
		return err;
	}
	ext4_li_info->li_state |= EXT4_LAZYINIT_RUNNING;
	return 0;
}

/*
 * Check whether it make sense to run itable init. thread or not.
 * If there is at least one uninitialized inode table, return
 * corresponding group number, else the loop goes through all
 * groups and return total number of groups.
 */
static ext4_group_t ext4_has_uninit_itable(struct super_block *sb)
{
	ext4_group_t group, ngroups = EXT4_SB(sb)->s_groups_count;
	struct ext4_group_desc *gdp = NULL;

	if (!ext4_has_group_desc_csum(sb))
		return ngroups;

	for (group = 0; group < ngroups; group++) {
		gdp = ext4_get_group_desc(sb, group, NULL);
		if (!gdp)
			continue;

<<<<<<< HEAD
		if (gdp->bg_flags & cpu_to_le16(EXT4_BG_INODE_ZEROED))
			continue;
		if (group != 0)
			break;
		ext4_error(sb, "Inode table for bg 0 marked as "
			   "needing zeroing");
		if (sb->s_flags & MS_RDONLY)
			return ngroups;
=======
		if (!(gdp->bg_flags & cpu_to_le16(EXT4_BG_INODE_ZEROED)))
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return group;
}

static int ext4_li_info_new(void)
{
	struct ext4_lazy_init *eli = NULL;

	eli = kzalloc(sizeof(*eli), GFP_KERNEL);
	if (!eli)
		return -ENOMEM;

	INIT_LIST_HEAD(&eli->li_request_list);
	mutex_init(&eli->li_list_mtx);

	eli->li_state |= EXT4_LAZYINIT_QUIT;

	ext4_li_info = eli;

	return 0;
}

static struct ext4_li_request *ext4_li_request_new(struct super_block *sb,
					    ext4_group_t start)
{
<<<<<<< HEAD
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_li_request *elr;
	unsigned long rnd;
=======
	struct ext4_li_request *elr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	elr = kzalloc(sizeof(*elr), GFP_KERNEL);
	if (!elr)
		return NULL;

	elr->lr_super = sb;
<<<<<<< HEAD
	elr->lr_sbi = sbi;
	elr->lr_next_group = start;
=======
	elr->lr_first_not_zeroed = start;
	if (test_opt(sb, NO_PREFETCH_BLOCK_BITMAPS)) {
		elr->lr_mode = EXT4_LI_MODE_ITABLE;
		elr->lr_next_group = start;
	} else {
		elr->lr_mode = EXT4_LI_MODE_PREFETCH_BBITMAP;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Randomize first schedule time of the request to
	 * spread the inode table initialization requests
	 * better.
	 */
<<<<<<< HEAD
	get_random_bytes(&rnd, sizeof(rnd));
	elr->lr_next_sched = jiffies + (unsigned long)rnd %
			     (EXT4_DEF_LI_MAX_START_DELAY * HZ);

	return elr;
}

static int ext4_register_li_request(struct super_block *sb,
				    ext4_group_t first_not_zeroed)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_li_request *elr;
	ext4_group_t ngroups = EXT4_SB(sb)->s_groups_count;
	int ret = 0;

=======
	elr->lr_next_sched = jiffies + get_random_u32_below(EXT4_DEF_LI_MAX_START_DELAY * HZ);
	return elr;
}

int ext4_register_li_request(struct super_block *sb,
			     ext4_group_t first_not_zeroed)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_li_request *elr = NULL;
	ext4_group_t ngroups = sbi->s_groups_count;
	int ret = 0;

	mutex_lock(&ext4_li_mtx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (sbi->s_li_request != NULL) {
		/*
		 * Reset timeout so it can be computed again, because
		 * s_li_wait_mult might have changed.
		 */
		sbi->s_li_request->lr_timeout = 0;
<<<<<<< HEAD
		return 0;
	}

	if (first_not_zeroed == ngroups ||
	    (sb->s_flags & MS_RDONLY) ||
	    !test_opt(sb, INIT_INODE_TABLE))
		return 0;

	elr = ext4_li_request_new(sb, first_not_zeroed);
	if (!elr)
		return -ENOMEM;

	mutex_lock(&ext4_li_mtx);
=======
		goto out;
	}

	if (sb_rdonly(sb) ||
	    (test_opt(sb, NO_PREFETCH_BLOCK_BITMAPS) &&
	     (first_not_zeroed == ngroups || !test_opt(sb, INIT_INODE_TABLE))))
		goto out;

	elr = ext4_li_request_new(sb, first_not_zeroed);
	if (!elr) {
		ret = -ENOMEM;
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (NULL == ext4_li_info) {
		ret = ext4_li_info_new();
		if (ret)
			goto out;
	}

	mutex_lock(&ext4_li_info->li_list_mtx);
	list_add(&elr->lr_request, &ext4_li_info->li_request_list);
	mutex_unlock(&ext4_li_info->li_list_mtx);

	sbi->s_li_request = elr;
	/*
	 * set elr to NULL here since it has been inserted to
	 * the request_list and the removal and free of it is
	 * handled by ext4_clear_request_list from now on.
	 */
	elr = NULL;

	if (!(ext4_li_info->li_state & EXT4_LAZYINIT_RUNNING)) {
		ret = ext4_run_lazyinit_thread();
		if (ret)
			goto out;
	}
out:
	mutex_unlock(&ext4_li_mtx);
	if (ret)
		kfree(elr);
	return ret;
}

/*
 * We do not need to lock anything since this is called on
 * module unload.
 */
static void ext4_destroy_lazyinit_thread(void)
{
	/*
	 * If thread exited earlier
	 * there's nothing to be done.
	 */
	if (!ext4_li_info || !ext4_lazyinit_task)
		return;

	kthread_stop(ext4_lazyinit_task);
}

<<<<<<< HEAD
=======
static int set_journal_csum_feature_set(struct super_block *sb)
{
	int ret = 1;
	int compat, incompat;
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	if (ext4_has_metadata_csum(sb)) {
		/* journal checksum v3 */
		compat = 0;
		incompat = JBD2_FEATURE_INCOMPAT_CSUM_V3;
	} else {
		/* journal checksum v1 */
		compat = JBD2_FEATURE_COMPAT_CHECKSUM;
		incompat = 0;
	}

	jbd2_journal_clear_features(sbi->s_journal,
			JBD2_FEATURE_COMPAT_CHECKSUM, 0,
			JBD2_FEATURE_INCOMPAT_CSUM_V3 |
			JBD2_FEATURE_INCOMPAT_CSUM_V2);
	if (test_opt(sb, JOURNAL_ASYNC_COMMIT)) {
		ret = jbd2_journal_set_features(sbi->s_journal,
				compat, 0,
				JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT |
				incompat);
	} else if (test_opt(sb, JOURNAL_CHECKSUM)) {
		ret = jbd2_journal_set_features(sbi->s_journal,
				compat, 0,
				incompat);
		jbd2_journal_clear_features(sbi->s_journal, 0, 0,
				JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT);
	} else {
		jbd2_journal_clear_features(sbi->s_journal, 0, 0,
				JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT);
	}

	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Note: calculating the overhead so we can be compatible with
 * historical BSD practice is quite difficult in the face of
 * clusters/bigalloc.  This is because multiple metadata blocks from
 * different block group can end up in the same allocation cluster.
 * Calculating the exact overhead in the face of clustered allocation
 * requires either O(all block bitmaps) in memory or O(number of block
 * groups**2) in time.  We will still calculate the superblock for
 * older file systems --- and if we come across with a bigalloc file
 * system with zero in s_overhead_clusters the estimate will be close to
 * correct especially for very large cluster sizes --- but for newer
 * file systems, it's better to calculate this figure once at mkfs
 * time, and store it in the superblock.  If the superblock value is
 * present (even for non-bigalloc file systems), we will use it.
 */
static int count_overhead(struct super_block *sb, ext4_group_t grp,
			  char *buf)
{
	struct ext4_sb_info	*sbi = EXT4_SB(sb);
	struct ext4_group_desc	*gdp;
	ext4_fsblk_t		first_block, last_block, b;
	ext4_group_t		i, ngroups = ext4_get_groups_count(sb);
	int			s, j, count = 0;
<<<<<<< HEAD

	if (!EXT4_HAS_RO_COMPAT_FEATURE(sb, EXT4_FEATURE_RO_COMPAT_BIGALLOC))
		return (ext4_bg_has_super(sb, grp) + ext4_bg_num_gdb(sb, grp) +
=======
	int			has_super = ext4_bg_has_super(sb, grp);

	if (!ext4_has_feature_bigalloc(sb))
		return (has_super + ext4_bg_num_gdb(sb, grp) +
			(has_super ? le16_to_cpu(sbi->s_es->s_reserved_gdt_blocks) : 0) +
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			sbi->s_itb_per_group + 2);

	first_block = le32_to_cpu(sbi->s_es->s_first_data_block) +
		(grp * EXT4_BLOCKS_PER_GROUP(sb));
	last_block = first_block + EXT4_BLOCKS_PER_GROUP(sb) - 1;
	for (i = 0; i < ngroups; i++) {
		gdp = ext4_get_group_desc(sb, i, NULL);
		b = ext4_block_bitmap(sb, gdp);
		if (b >= first_block && b <= last_block) {
			ext4_set_bit(EXT4_B2C(sbi, b - first_block), buf);
			count++;
		}
		b = ext4_inode_bitmap(sb, gdp);
		if (b >= first_block && b <= last_block) {
			ext4_set_bit(EXT4_B2C(sbi, b - first_block), buf);
			count++;
		}
		b = ext4_inode_table(sb, gdp);
		if (b >= first_block && b + sbi->s_itb_per_group <= last_block)
			for (j = 0; j < sbi->s_itb_per_group; j++, b++) {
				int c = EXT4_B2C(sbi, b - first_block);
				ext4_set_bit(c, buf);
				count++;
			}
		if (i != grp)
			continue;
		s = 0;
		if (ext4_bg_has_super(sb, grp)) {
			ext4_set_bit(s++, buf);
			count++;
		}
<<<<<<< HEAD
		for (j = ext4_bg_num_gdb(sb, grp); j > 0; j--) {
			ext4_set_bit(EXT4_B2C(sbi, s++), buf);
			count++;
		}
=======
		j = ext4_bg_num_gdb(sb, grp);
		if (s + j > EXT4_BLOCKS_PER_GROUP(sb)) {
			ext4_error(sb, "Invalid number of block group "
				   "descriptor blocks: %d", j);
			j = EXT4_BLOCKS_PER_GROUP(sb) - s;
		}
		count += j;
		for (; j > 0; j--)
			ext4_set_bit(EXT4_B2C(sbi, s++), buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (!count)
		return 0;
	return EXT4_CLUSTERS_PER_GROUP(sb) -
		ext4_count_free(buf, EXT4_CLUSTERS_PER_GROUP(sb) / 8);
}

/*
 * Compute the overhead and stash it in sbi->s_overhead
 */
int ext4_calculate_overhead(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
<<<<<<< HEAD
	ext4_group_t i, ngroups = ext4_get_groups_count(sb);
	ext4_fsblk_t overhead = 0;
	char *buf = (char *) get_zeroed_page(GFP_KERNEL);

	memset(buf, 0, PAGE_SIZE);
=======
	struct inode *j_inode;
	unsigned int j_blocks, j_inum = le32_to_cpu(es->s_journal_inum);
	ext4_group_t i, ngroups = ext4_get_groups_count(sb);
	ext4_fsblk_t overhead = 0;
	char *buf = (char *) get_zeroed_page(GFP_NOFS);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!buf)
		return -ENOMEM;

	/*
	 * Compute the overhead (FS structures).  This is constant
	 * for a given filesystem unless the number of block groups
	 * changes so we cache the previous value until it does.
	 */

	/*
	 * All of the blocks before first_data_block are overhead
	 */
	overhead = EXT4_B2C(sbi, le32_to_cpu(es->s_first_data_block));

	/*
	 * Add the overhead found in each block group
	 */
	for (i = 0; i < ngroups; i++) {
		int blks;

		blks = count_overhead(sb, i, buf);
		overhead += blks;
		if (blks)
			memset(buf, 0, PAGE_SIZE);
		cond_resched();
	}
<<<<<<< HEAD
=======

	/*
	 * Add the internal journal blocks whether the journal has been
	 * loaded or not
	 */
	if (sbi->s_journal && !sbi->s_journal_bdev_file)
		overhead += EXT4_NUM_B2C(sbi, sbi->s_journal->j_total_len);
	else if (ext4_has_feature_journal(sb) && !sbi->s_journal && j_inum) {
		/* j_inum for internal journal is non-zero */
		j_inode = ext4_get_journal_inode(sb, j_inum);
		if (!IS_ERR(j_inode)) {
			j_blocks = j_inode->i_size >> sb->s_blocksize_bits;
			overhead += EXT4_NUM_B2C(sbi, j_blocks);
			iput(j_inode);
		} else {
			ext4_msg(sb, KERN_ERR, "can't get journal size");
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sbi->s_overhead = overhead;
	smp_wmb();
	free_page((unsigned long) buf);
	return 0;
}

<<<<<<< HEAD
static int ext4_fill_super(struct super_block *sb, void *data, int silent)
{
	char *orig_data = kstrdup(data, GFP_KERNEL);
	struct buffer_head *bh;
	struct ext4_super_block *es = NULL;
	struct ext4_sb_info *sbi;
	ext4_fsblk_t block;
	ext4_fsblk_t sb_block = get_sb_block(&data);
	ext4_fsblk_t logical_sb_block;
	unsigned long offset = 0;
	unsigned long journal_devnum = 0;
	unsigned long def_mount_opts;
	struct inode *root;
	char *cp;
	const char *descr;
	int ret = -ENOMEM;
	int blocksize, clustersize;
	unsigned int db_count;
	unsigned int i;
	int needs_recovery, has_huge_files, has_bigalloc;
	__u64 blocks_count;
	int err;
	unsigned int journal_ioprio = DEFAULT_JOURNAL_IOPRIO;
	ext4_group_t first_not_zeroed;

	sbi = kzalloc(sizeof(*sbi), GFP_KERNEL);
	if (!sbi)
		goto out_free_orig;

	sbi->s_blockgroup_lock =
		kzalloc(sizeof(struct blockgroup_lock), GFP_KERNEL);
	if (!sbi->s_blockgroup_lock) {
		kfree(sbi);
		goto out_free_orig;
	}
	sb->s_fs_info = sbi;
	sbi->s_mount_opt = 0;
	sbi->s_resuid = EXT4_DEF_RESUID;
	sbi->s_resgid = EXT4_DEF_RESGID;
	sbi->s_inode_readahead_blks = EXT4_DEF_INODE_READAHEAD_BLKS;
	sbi->s_sb_block = sb_block;
	if (sb->s_bdev->bd_part)
		sbi->s_sectors_written_start =
			part_stat_read(sb->s_bdev->bd_part, sectors[1]);

	/* Cleanup superblock name */
	for (cp = sb->s_id; (cp = strchr(cp, '/'));)
		*cp = '!';

	ret = -EINVAL;
	blocksize = sb_min_blocksize(sb, EXT4_MIN_BLOCK_SIZE);
	if (!blocksize) {
		ext4_msg(sb, KERN_ERR, "unable to set blocksize");
		goto out_fail;
	}

	/*
	 * The ext4 superblock will not be buffer aligned for other than 1kB
	 * block sizes.  We need to calculate the offset from buffer start.
	 */
	if (blocksize != EXT4_MIN_BLOCK_SIZE) {
		logical_sb_block = sb_block * EXT4_MIN_BLOCK_SIZE;
		offset = do_div(logical_sb_block, blocksize);
	} else {
		logical_sb_block = sb_block;
	}

	if (!(bh = sb_bread(sb, logical_sb_block))) {
		ext4_msg(sb, KERN_ERR, "unable to read superblock");
		goto out_fail;
	}
	/*
	 * Note: s_es must be initialized as soon as possible because
	 *       some ext4 macro-instructions depend on its value
	 */
	es = (struct ext4_super_block *) (((char *)bh->b_data) + offset);
	sbi->s_es = es;
	sb->s_magic = le16_to_cpu(es->s_magic);
	if (sb->s_magic != EXT4_SUPER_MAGIC)
		goto cantfind_ext4;
	sbi->s_kbytes_written = le64_to_cpu(es->s_kbytes_written);

	/* Warn if metadata_csum and gdt_csum are both set. */
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb,
				       EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) &&
	    EXT4_HAS_RO_COMPAT_FEATURE(sb, EXT4_FEATURE_RO_COMPAT_GDT_CSUM))
		ext4_warning(sb, KERN_INFO "metadata_csum and uninit_bg are "
			     "redundant flags; please run fsck.");

	/* Check for a known checksum algorithm */
	if (!ext4_verify_csum_type(sb, es)) {
		ext4_msg(sb, KERN_ERR, "VFS: Found ext4 filesystem with "
			 "unknown checksum algorithm.");
		silent = 1;
		goto cantfind_ext4;
	}

	/* Load the checksum driver */
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb,
				       EXT4_FEATURE_RO_COMPAT_METADATA_CSUM)) {
		sbi->s_chksum_driver = crypto_alloc_shash("crc32c", 0, 0);
		if (IS_ERR(sbi->s_chksum_driver)) {
			ext4_msg(sb, KERN_ERR, "Cannot load crc32c driver.");
			ret = PTR_ERR(sbi->s_chksum_driver);
			sbi->s_chksum_driver = NULL;
			goto failed_mount;
		}
	}

	/* Check superblock checksum */
	if (!ext4_superblock_csum_verify(sb, es)) {
		ext4_msg(sb, KERN_ERR, "VFS: Found ext4 filesystem with "
			 "invalid superblock checksum.  Run e2fsck?");
		silent = 1;
		goto cantfind_ext4;
	}

	/* Precompute checksum seed for all metadata */
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb,
			EXT4_FEATURE_RO_COMPAT_METADATA_CSUM))
		sbi->s_csum_seed = ext4_chksum(sbi, ~0, es->s_uuid,
					       sizeof(es->s_uuid));
=======
static void ext4_set_resv_clusters(struct super_block *sb)
{
	ext4_fsblk_t resv_clusters;
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	/*
	 * There's no need to reserve anything when we aren't using extents.
	 * The space estimates are exact, there are no unwritten extents,
	 * hole punching doesn't need new metadata... This is needed especially
	 * to keep ext2/3 backward compatibility.
	 */
	if (!ext4_has_feature_extents(sb))
		return;
	/*
	 * By default we reserve 2% or 4096 clusters, whichever is smaller.
	 * This should cover the situations where we can not afford to run
	 * out of space like for example punch hole, or converting
	 * unwritten extents in delalloc path. In most cases such
	 * allocation would require 1, or 2 blocks, higher numbers are
	 * very rare.
	 */
	resv_clusters = (ext4_blocks_count(sbi->s_es) >>
			 sbi->s_cluster_bits);

	do_div(resv_clusters, 50);
	resv_clusters = min_t(ext4_fsblk_t, resv_clusters, 4096);

	atomic64_set(&sbi->s_resv_clusters, resv_clusters);
}

static const char *ext4_quota_mode(struct super_block *sb)
{
#ifdef CONFIG_QUOTA
	if (!ext4_quota_capable(sb))
		return "none";

	if (EXT4_SB(sb)->s_journal && ext4_is_quota_journalled(sb))
		return "journalled";
	else
		return "writeback";
#else
	return "disabled";
#endif
}

static void ext4_setup_csum_trigger(struct super_block *sb,
				    enum ext4_journal_trigger_type type,
				    void (*trigger)(
					struct jbd2_buffer_trigger_type *type,
					struct buffer_head *bh,
					void *mapped_data,
					size_t size))
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	sbi->s_journal_triggers[type].sb = sb;
	sbi->s_journal_triggers[type].tr_triggers.t_frozen = trigger;
}

static void ext4_free_sbi(struct ext4_sb_info *sbi)
{
	if (!sbi)
		return;

	kfree(sbi->s_blockgroup_lock);
	fs_put_dax(sbi->s_daxdev, NULL);
	kfree(sbi);
}

static struct ext4_sb_info *ext4_alloc_sbi(struct super_block *sb)
{
	struct ext4_sb_info *sbi;

	sbi = kzalloc(sizeof(*sbi), GFP_KERNEL);
	if (!sbi)
		return NULL;

	sbi->s_daxdev = fs_dax_get_by_bdev(sb->s_bdev, &sbi->s_dax_part_off,
					   NULL, NULL);

	sbi->s_blockgroup_lock =
		kzalloc(sizeof(struct blockgroup_lock), GFP_KERNEL);

	if (!sbi->s_blockgroup_lock)
		goto err_out;

	sb->s_fs_info = sbi;
	sbi->s_sb = sb;
	return sbi;
err_out:
	fs_put_dax(sbi->s_daxdev, NULL);
	kfree(sbi);
	return NULL;
}

static void ext4_set_def_opts(struct super_block *sb,
			      struct ext4_super_block *es)
{
	unsigned long def_mount_opts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set defaults before we parse the mount options */
	def_mount_opts = le32_to_cpu(es->s_default_mount_opts);
	set_opt(sb, INIT_INODE_TABLE);
	if (def_mount_opts & EXT4_DEFM_DEBUG)
		set_opt(sb, DEBUG);
	if (def_mount_opts & EXT4_DEFM_BSDGROUPS)
		set_opt(sb, GRPID);
	if (def_mount_opts & EXT4_DEFM_UID16)
		set_opt(sb, NO_UID32);
	/* xattr user namespace & acls are now defaulted on */
<<<<<<< HEAD
#ifdef CONFIG_EXT4_FS_XATTR
	set_opt(sb, XATTR_USER);
#endif
#ifdef CONFIG_EXT4_FS_POSIX_ACL
	set_opt(sb, POSIX_ACL);
#endif
	set_opt(sb, MBLK_IO_SUBMIT);
=======
	set_opt(sb, XATTR_USER);
#ifdef CONFIG_EXT4_FS_POSIX_ACL
	set_opt(sb, POSIX_ACL);
#endif
	if (ext4_has_feature_fast_commit(sb))
		set_opt2(sb, JOURNAL_FAST_COMMIT);
	/* don't forget to enable journal_csum when metadata_csum is enabled. */
	if (ext4_has_metadata_csum(sb))
		set_opt(sb, JOURNAL_CHECKSUM);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((def_mount_opts & EXT4_DEFM_JMODE) == EXT4_DEFM_JMODE_DATA)
		set_opt(sb, JOURNAL_DATA);
	else if ((def_mount_opts & EXT4_DEFM_JMODE) == EXT4_DEFM_JMODE_ORDERED)
		set_opt(sb, ORDERED_DATA);
	else if ((def_mount_opts & EXT4_DEFM_JMODE) == EXT4_DEFM_JMODE_WBACK)
		set_opt(sb, WRITEBACK_DATA);

<<<<<<< HEAD
	if (le16_to_cpu(sbi->s_es->s_errors) == EXT4_ERRORS_PANIC)
		set_opt(sb, ERRORS_PANIC);
	else if (le16_to_cpu(sbi->s_es->s_errors) == EXT4_ERRORS_CONTINUE)
		set_opt(sb, ERRORS_CONT);
	else
		set_opt(sb, ERRORS_RO);
	if (def_mount_opts & EXT4_DEFM_BLOCK_VALIDITY)
		set_opt(sb, BLOCK_VALIDITY);
	if (def_mount_opts & EXT4_DEFM_DISCARD)
		set_opt(sb, DISCARD);

	sbi->s_resuid = le16_to_cpu(es->s_def_resuid);
	sbi->s_resgid = le16_to_cpu(es->s_def_resgid);
	sbi->s_commit_interval = JBD2_DEFAULT_MAX_COMMIT_AGE * HZ;
	sbi->s_min_batch_time = EXT4_DEF_MIN_BATCH_TIME;
	sbi->s_max_batch_time = EXT4_DEF_MAX_BATCH_TIME;

=======
	if (le16_to_cpu(es->s_errors) == EXT4_ERRORS_PANIC)
		set_opt(sb, ERRORS_PANIC);
	else if (le16_to_cpu(es->s_errors) == EXT4_ERRORS_CONTINUE)
		set_opt(sb, ERRORS_CONT);
	else
		set_opt(sb, ERRORS_RO);
	/* block_validity enabled by default; disable with noblock_validity */
	set_opt(sb, BLOCK_VALIDITY);
	if (def_mount_opts & EXT4_DEFM_DISCARD)
		set_opt(sb, DISCARD);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((def_mount_opts & EXT4_DEFM_NOBARRIER) == 0)
		set_opt(sb, BARRIER);

	/*
	 * enable delayed allocation by default
	 * Use -o nodelalloc to turn it off
	 */
<<<<<<< HEAD
	if (!IS_EXT3_SB(sb) &&
	    ((def_mount_opts & EXT4_DEFM_NODELALLOC) == 0))
		set_opt(sb, DELALLOC);

	/*
	 * set default s_li_wait_mult for lazyinit, for the case there is
	 * no mount option specified.
	 */
	sbi->s_li_wait_mult = EXT4_DEF_LI_WAIT_MULT;

	if (!parse_options((char *) sbi->s_es->s_mount_opts, sb,
			   &journal_devnum, &journal_ioprio, 0)) {
		ext4_msg(sb, KERN_WARNING,
			 "failed to parse options in superblock: %s",
			 sbi->s_es->s_mount_opts);
	}
	sbi->s_def_mount_opt = sbi->s_mount_opt;
	if (!parse_options((char *) data, sb, &journal_devnum,
			   &journal_ioprio, 0))
		goto failed_mount;

	if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_JOURNAL_DATA) {
		printk_once(KERN_WARNING "EXT4-fs: Warning: mounting "
			    "with data=journal disables delayed "
			    "allocation and O_DIRECT support!\n");
		if (test_opt2(sb, EXPLICIT_DELALLOC)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "both data=journal and delalloc");
			goto failed_mount;
		}
		if (test_opt(sb, DIOREAD_NOLOCK)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "both data=journal and dioread_nolock");
			goto failed_mount;
		}
		if (test_opt(sb, DELALLOC))
			clear_opt(sb, DELALLOC);
	}

	sb->s_flags = (sb->s_flags & ~MS_POSIXACL) |
		(test_opt(sb, POSIX_ACL) ? MS_POSIXACL : 0);

	if (le32_to_cpu(es->s_rev_level) == EXT4_GOOD_OLD_REV &&
	    (EXT4_HAS_COMPAT_FEATURE(sb, ~0U) ||
	     EXT4_HAS_RO_COMPAT_FEATURE(sb, ~0U) ||
	     EXT4_HAS_INCOMPAT_FEATURE(sb, ~0U)))
		ext4_msg(sb, KERN_WARNING,
		       "feature flags set on rev 0 fs, "
		       "running e2fsck is recommended");

	if (IS_EXT2_SB(sb)) {
		if (ext2_feature_set_ok(sb))
			ext4_msg(sb, KERN_INFO, "mounting ext2 file system "
				 "using the ext4 subsystem");
		else {
			ext4_msg(sb, KERN_ERR, "couldn't mount as ext2 due "
				 "to feature incompatibilities");
			goto failed_mount;
		}
	}

	if (IS_EXT3_SB(sb)) {
		if (ext3_feature_set_ok(sb))
			ext4_msg(sb, KERN_INFO, "mounting ext3 file system "
				 "using the ext4 subsystem");
		else {
			ext4_msg(sb, KERN_ERR, "couldn't mount as ext3 due "
				 "to feature incompatibilities");
			goto failed_mount;
		}
	}

	/*
	 * Check feature flags regardless of the revision level, since we
	 * previously didn't change the revision level when setting the flags,
	 * so there is a chance incompat flags are set on a rev 0 filesystem.
	 */
	if (!ext4_feature_set_ok(sb, (sb->s_flags & MS_RDONLY)))
		goto failed_mount;

	blocksize = BLOCK_SIZE << le32_to_cpu(es->s_log_block_size);
	if (blocksize < EXT4_MIN_BLOCK_SIZE ||
	    blocksize > EXT4_MAX_BLOCK_SIZE) {
		ext4_msg(sb, KERN_ERR,
		       "Unsupported filesystem blocksize %d", blocksize);
		goto failed_mount;
	}

	if (sb->s_blocksize != blocksize) {
		/* Validate the filesystem blocksize */
		if (!sb_set_blocksize(sb, blocksize)) {
			ext4_msg(sb, KERN_ERR, "bad block size %d",
					blocksize);
			goto failed_mount;
		}

		brelse(bh);
		logical_sb_block = sb_block * EXT4_MIN_BLOCK_SIZE;
		offset = do_div(logical_sb_block, blocksize);
		bh = sb_bread(sb, logical_sb_block);
		if (!bh) {
			ext4_msg(sb, KERN_ERR,
			       "Can't read superblock on 2nd try");
			goto failed_mount;
		}
		es = (struct ext4_super_block *)(((char *)bh->b_data) + offset);
		sbi->s_es = es;
		if (es->s_magic != cpu_to_le16(EXT4_SUPER_MAGIC)) {
			ext4_msg(sb, KERN_ERR,
			       "Magic mismatch, very weird!");
			goto failed_mount;
		}
	}

	has_huge_files = EXT4_HAS_RO_COMPAT_FEATURE(sb,
				EXT4_FEATURE_RO_COMPAT_HUGE_FILE);
	sbi->s_bitmap_maxbytes = ext4_max_bitmap_size(sb->s_blocksize_bits,
						      has_huge_files);
	sb->s_maxbytes = ext4_max_size(sb->s_blocksize_bits, has_huge_files);
=======
	if (!IS_EXT3_SB(sb) && !IS_EXT2_SB(sb) &&
	    ((def_mount_opts & EXT4_DEFM_NODELALLOC) == 0))
		set_opt(sb, DELALLOC);

	if (sb->s_blocksize <= PAGE_SIZE)
		set_opt(sb, DIOREAD_NOLOCK);
}

static int ext4_handle_clustersize(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
	int clustersize;

	/* Handle clustersize */
	clustersize = BLOCK_SIZE << le32_to_cpu(es->s_log_cluster_size);
	if (ext4_has_feature_bigalloc(sb)) {
		if (clustersize < sb->s_blocksize) {
			ext4_msg(sb, KERN_ERR,
				 "cluster size (%d) smaller than "
				 "block size (%lu)", clustersize, sb->s_blocksize);
			return -EINVAL;
		}
		sbi->s_cluster_bits = le32_to_cpu(es->s_log_cluster_size) -
			le32_to_cpu(es->s_log_block_size);
	} else {
		if (clustersize != sb->s_blocksize) {
			ext4_msg(sb, KERN_ERR,
				 "fragment/cluster size (%d) != "
				 "block size (%lu)", clustersize, sb->s_blocksize);
			return -EINVAL;
		}
		if (sbi->s_blocks_per_group > sb->s_blocksize * 8) {
			ext4_msg(sb, KERN_ERR,
				 "#blocks per group too big: %lu",
				 sbi->s_blocks_per_group);
			return -EINVAL;
		}
		sbi->s_cluster_bits = 0;
	}
	sbi->s_clusters_per_group = le32_to_cpu(es->s_clusters_per_group);
	if (sbi->s_clusters_per_group > sb->s_blocksize * 8) {
		ext4_msg(sb, KERN_ERR, "#clusters per group too big: %lu",
			 sbi->s_clusters_per_group);
		return -EINVAL;
	}
	if (sbi->s_blocks_per_group !=
	    (sbi->s_clusters_per_group * (clustersize / sb->s_blocksize))) {
		ext4_msg(sb, KERN_ERR,
			 "blocks per group (%lu) and clusters per group (%lu) inconsistent",
			 sbi->s_blocks_per_group, sbi->s_clusters_per_group);
		return -EINVAL;
	}
	sbi->s_cluster_ratio = clustersize / sb->s_blocksize;

	/* Do we have standard group size of clustersize * 8 blocks ? */
	if (sbi->s_blocks_per_group == clustersize << 3)
		set_opt2(sb, STD_GROUP_SIZE);

	return 0;
}

static void ext4_fast_commit_init(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	/* Initialize fast commit stuff */
	atomic_set(&sbi->s_fc_subtid, 0);
	INIT_LIST_HEAD(&sbi->s_fc_q[FC_Q_MAIN]);
	INIT_LIST_HEAD(&sbi->s_fc_q[FC_Q_STAGING]);
	INIT_LIST_HEAD(&sbi->s_fc_dentry_q[FC_Q_MAIN]);
	INIT_LIST_HEAD(&sbi->s_fc_dentry_q[FC_Q_STAGING]);
	sbi->s_fc_bytes = 0;
	ext4_clear_mount_flag(sb, EXT4_MF_FC_INELIGIBLE);
	sbi->s_fc_ineligible_tid = 0;
	spin_lock_init(&sbi->s_fc_lock);
	memset(&sbi->s_fc_stats, 0, sizeof(sbi->s_fc_stats));
	sbi->s_fc_replay_state.fc_regions = NULL;
	sbi->s_fc_replay_state.fc_regions_size = 0;
	sbi->s_fc_replay_state.fc_regions_used = 0;
	sbi->s_fc_replay_state.fc_regions_valid = 0;
	sbi->s_fc_replay_state.fc_modified_inodes = NULL;
	sbi->s_fc_replay_state.fc_modified_inodes_size = 0;
	sbi->s_fc_replay_state.fc_modified_inodes_used = 0;
}

static int ext4_inode_info_init(struct super_block *sb,
				struct ext4_super_block *es)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (le32_to_cpu(es->s_rev_level) == EXT4_GOOD_OLD_REV) {
		sbi->s_inode_size = EXT4_GOOD_OLD_INODE_SIZE;
		sbi->s_first_ino = EXT4_GOOD_OLD_FIRST_INO;
	} else {
		sbi->s_inode_size = le16_to_cpu(es->s_inode_size);
		sbi->s_first_ino = le32_to_cpu(es->s_first_ino);
		if (sbi->s_first_ino < EXT4_GOOD_OLD_FIRST_INO) {
			ext4_msg(sb, KERN_ERR, "invalid first ino: %u",
				 sbi->s_first_ino);
<<<<<<< HEAD
			goto failed_mount;
		}
		if ((sbi->s_inode_size < EXT4_GOOD_OLD_INODE_SIZE) ||
		    (!is_power_of_2(sbi->s_inode_size)) ||
		    (sbi->s_inode_size > blocksize)) {
			ext4_msg(sb, KERN_ERR,
			       "unsupported inode size: %d",
			       sbi->s_inode_size);
			goto failed_mount;
		}
		if (sbi->s_inode_size > EXT4_GOOD_OLD_INODE_SIZE)
			sb->s_time_gran = 1 << (EXT4_EPOCH_BITS - 2);
	}

	sbi->s_desc_size = le16_to_cpu(es->s_desc_size);
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_64BIT)) {
		if (sbi->s_desc_size < EXT4_MIN_DESC_SIZE_64BIT ||
		    sbi->s_desc_size > EXT4_MAX_DESC_SIZE ||
		    !is_power_of_2(sbi->s_desc_size)) {
			ext4_msg(sb, KERN_ERR,
			       "unsupported descriptor size %lu",
			       sbi->s_desc_size);
			goto failed_mount;
		}
	} else
		sbi->s_desc_size = EXT4_MIN_DESC_SIZE;

	sbi->s_blocks_per_group = le32_to_cpu(es->s_blocks_per_group);
	sbi->s_inodes_per_group = le32_to_cpu(es->s_inodes_per_group);
	if (EXT4_INODE_SIZE(sb) == 0 || EXT4_INODES_PER_GROUP(sb) == 0)
		goto cantfind_ext4;

	sbi->s_inodes_per_block = blocksize / EXT4_INODE_SIZE(sb);
	if (sbi->s_inodes_per_block == 0)
		goto cantfind_ext4;
	sbi->s_itb_per_group = sbi->s_inodes_per_group /
					sbi->s_inodes_per_block;
	sbi->s_desc_per_block = blocksize / EXT4_DESC_SIZE(sb);
	sbi->s_sbh = bh;
	sbi->s_mount_state = le16_to_cpu(es->s_state);
	sbi->s_addr_per_block_bits = ilog2(EXT4_ADDR_PER_BLOCK(sb));
	sbi->s_desc_per_block_bits = ilog2(EXT4_DESC_PER_BLOCK(sb));

	for (i = 0; i < 4; i++)
		sbi->s_hash_seed[i] = le32_to_cpu(es->s_hash_seed[i]);
	sbi->s_def_hash_version = es->s_def_hash_version;
	if (EXT4_HAS_COMPAT_FEATURE(sb, EXT4_FEATURE_COMPAT_DIR_INDEX)) {
		i = le32_to_cpu(es->s_flags);
		if (i & EXT2_FLAGS_UNSIGNED_HASH)
			sbi->s_hash_unsigned = 3;
		else if ((i & EXT2_FLAGS_SIGNED_HASH) == 0) {
#ifdef __CHAR_UNSIGNED__
			if (!(sb->s_flags & MS_RDONLY))
				es->s_flags |=
					cpu_to_le32(EXT2_FLAGS_UNSIGNED_HASH);
			sbi->s_hash_unsigned = 3;
#else
			if (!(sb->s_flags & MS_RDONLY))
				es->s_flags |=
					cpu_to_le32(EXT2_FLAGS_SIGNED_HASH);
#endif
		}
	}

	/* Handle clustersize */
	clustersize = BLOCK_SIZE << le32_to_cpu(es->s_log_cluster_size);
	has_bigalloc = EXT4_HAS_RO_COMPAT_FEATURE(sb,
				EXT4_FEATURE_RO_COMPAT_BIGALLOC);
	if (has_bigalloc) {
		if (clustersize < blocksize) {
			ext4_msg(sb, KERN_ERR,
				 "cluster size (%d) smaller than "
				 "block size (%d)", clustersize, blocksize);
			goto failed_mount;
		}
		sbi->s_cluster_bits = le32_to_cpu(es->s_log_cluster_size) -
			le32_to_cpu(es->s_log_block_size);
		sbi->s_clusters_per_group =
			le32_to_cpu(es->s_clusters_per_group);
		if (sbi->s_clusters_per_group > blocksize * 8) {
			ext4_msg(sb, KERN_ERR,
				 "#clusters per group too big: %lu",
				 sbi->s_clusters_per_group);
			goto failed_mount;
		}
		if (sbi->s_blocks_per_group !=
		    (sbi->s_clusters_per_group * (clustersize / blocksize))) {
			ext4_msg(sb, KERN_ERR, "blocks per group (%lu) and "
				 "clusters per group (%lu) inconsistent",
				 sbi->s_blocks_per_group,
				 sbi->s_clusters_per_group);
			goto failed_mount;
		}
	} else {
		if (clustersize != blocksize) {
			ext4_warning(sb, "fragment/cluster size (%d) != "
				     "block size (%d)", clustersize,
				     blocksize);
			clustersize = blocksize;
		}
		if (sbi->s_blocks_per_group > blocksize * 8) {
			ext4_msg(sb, KERN_ERR,
				 "#blocks per group too big: %lu",
				 sbi->s_blocks_per_group);
			goto failed_mount;
		}
		sbi->s_clusters_per_group = sbi->s_blocks_per_group;
		sbi->s_cluster_bits = 0;
	}
	sbi->s_cluster_ratio = clustersize / blocksize;

	if (sbi->s_inodes_per_group > blocksize * 8) {
		ext4_msg(sb, KERN_ERR,
		       "#inodes per group too big: %lu",
		       sbi->s_inodes_per_group);
		goto failed_mount;
	}

=======
			return -EINVAL;
		}
		if ((sbi->s_inode_size < EXT4_GOOD_OLD_INODE_SIZE) ||
		    (!is_power_of_2(sbi->s_inode_size)) ||
		    (sbi->s_inode_size > sb->s_blocksize)) {
			ext4_msg(sb, KERN_ERR,
			       "unsupported inode size: %d",
			       sbi->s_inode_size);
			ext4_msg(sb, KERN_ERR, "blocksize: %lu", sb->s_blocksize);
			return -EINVAL;
		}
		/*
		 * i_atime_extra is the last extra field available for
		 * [acm]times in struct ext4_inode. Checking for that
		 * field should suffice to ensure we have extra space
		 * for all three.
		 */
		if (sbi->s_inode_size >= offsetof(struct ext4_inode, i_atime_extra) +
			sizeof(((struct ext4_inode *)0)->i_atime_extra)) {
			sb->s_time_gran = 1;
			sb->s_time_max = EXT4_EXTRA_TIMESTAMP_MAX;
		} else {
			sb->s_time_gran = NSEC_PER_SEC;
			sb->s_time_max = EXT4_NON_EXTRA_TIMESTAMP_MAX;
		}
		sb->s_time_min = EXT4_TIMESTAMP_MIN;
	}

	if (sbi->s_inode_size > EXT4_GOOD_OLD_INODE_SIZE) {
		sbi->s_want_extra_isize = sizeof(struct ext4_inode) -
			EXT4_GOOD_OLD_INODE_SIZE;
		if (ext4_has_feature_extra_isize(sb)) {
			unsigned v, max = (sbi->s_inode_size -
					   EXT4_GOOD_OLD_INODE_SIZE);

			v = le16_to_cpu(es->s_want_extra_isize);
			if (v > max) {
				ext4_msg(sb, KERN_ERR,
					 "bad s_want_extra_isize: %d", v);
				return -EINVAL;
			}
			if (sbi->s_want_extra_isize < v)
				sbi->s_want_extra_isize = v;

			v = le16_to_cpu(es->s_min_extra_isize);
			if (v > max) {
				ext4_msg(sb, KERN_ERR,
					 "bad s_min_extra_isize: %d", v);
				return -EINVAL;
			}
			if (sbi->s_want_extra_isize < v)
				sbi->s_want_extra_isize = v;
		}
	}

	return 0;
}

#if IS_ENABLED(CONFIG_UNICODE)
static int ext4_encoding_init(struct super_block *sb, struct ext4_super_block *es)
{
	const struct ext4_sb_encodings *encoding_info;
	struct unicode_map *encoding;
	__u16 encoding_flags = le16_to_cpu(es->s_encoding_flags);

	if (!ext4_has_feature_casefold(sb) || sb->s_encoding)
		return 0;

	encoding_info = ext4_sb_read_encoding(es);
	if (!encoding_info) {
		ext4_msg(sb, KERN_ERR,
			"Encoding requested by superblock is unknown");
		return -EINVAL;
	}

	encoding = utf8_load(encoding_info->version);
	if (IS_ERR(encoding)) {
		ext4_msg(sb, KERN_ERR,
			"can't mount with superblock charset: %s-%u.%u.%u "
			"not supported by the kernel. flags: 0x%x.",
			encoding_info->name,
			unicode_major(encoding_info->version),
			unicode_minor(encoding_info->version),
			unicode_rev(encoding_info->version),
			encoding_flags);
		return -EINVAL;
	}
	ext4_msg(sb, KERN_INFO,"Using encoding defined by superblock: "
		"%s-%u.%u.%u with flags 0x%hx", encoding_info->name,
		unicode_major(encoding_info->version),
		unicode_minor(encoding_info->version),
		unicode_rev(encoding_info->version),
		encoding_flags);

	sb->s_encoding = encoding;
	sb->s_encoding_flags = encoding_flags;

	return 0;
}
#else
static inline int ext4_encoding_init(struct super_block *sb, struct ext4_super_block *es)
{
	return 0;
}
#endif

static int ext4_init_metadata_csum(struct super_block *sb, struct ext4_super_block *es)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	/* Warn if metadata_csum and gdt_csum are both set. */
	if (ext4_has_feature_metadata_csum(sb) &&
	    ext4_has_feature_gdt_csum(sb))
		ext4_warning(sb, "metadata_csum and uninit_bg are "
			     "redundant flags; please run fsck.");

	/* Check for a known checksum algorithm */
	if (!ext4_verify_csum_type(sb, es)) {
		ext4_msg(sb, KERN_ERR, "VFS: Found ext4 filesystem with "
			 "unknown checksum algorithm.");
		return -EINVAL;
	}
	ext4_setup_csum_trigger(sb, EXT4_JTR_ORPHAN_FILE,
				ext4_orphan_file_block_trigger);

	/* Load the checksum driver */
	sbi->s_chksum_driver = crypto_alloc_shash("crc32c", 0, 0);
	if (IS_ERR(sbi->s_chksum_driver)) {
		int ret = PTR_ERR(sbi->s_chksum_driver);
		ext4_msg(sb, KERN_ERR, "Cannot load crc32c driver.");
		sbi->s_chksum_driver = NULL;
		return ret;
	}

	/* Check superblock checksum */
	if (!ext4_superblock_csum_verify(sb, es)) {
		ext4_msg(sb, KERN_ERR, "VFS: Found ext4 filesystem with "
			 "invalid superblock checksum.  Run e2fsck?");
		return -EFSBADCRC;
	}

	/* Precompute checksum seed for all metadata */
	if (ext4_has_feature_csum_seed(sb))
		sbi->s_csum_seed = le32_to_cpu(es->s_checksum_seed);
	else if (ext4_has_metadata_csum(sb) || ext4_has_feature_ea_inode(sb))
		sbi->s_csum_seed = ext4_chksum(sbi, ~0, es->s_uuid,
					       sizeof(es->s_uuid));
	return 0;
}

static int ext4_check_feature_compatibility(struct super_block *sb,
					    struct ext4_super_block *es,
					    int silent)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	if (le32_to_cpu(es->s_rev_level) == EXT4_GOOD_OLD_REV &&
	    (ext4_has_compat_features(sb) ||
	     ext4_has_ro_compat_features(sb) ||
	     ext4_has_incompat_features(sb)))
		ext4_msg(sb, KERN_WARNING,
		       "feature flags set on rev 0 fs, "
		       "running e2fsck is recommended");

	if (es->s_creator_os == cpu_to_le32(EXT4_OS_HURD)) {
		set_opt2(sb, HURD_COMPAT);
		if (ext4_has_feature_64bit(sb)) {
			ext4_msg(sb, KERN_ERR,
				 "The Hurd can't support 64-bit file systems");
			return -EINVAL;
		}

		/*
		 * ea_inode feature uses l_i_version field which is not
		 * available in HURD_COMPAT mode.
		 */
		if (ext4_has_feature_ea_inode(sb)) {
			ext4_msg(sb, KERN_ERR,
				 "ea_inode feature is not supported for Hurd");
			return -EINVAL;
		}
	}

	if (IS_EXT2_SB(sb)) {
		if (ext2_feature_set_ok(sb))
			ext4_msg(sb, KERN_INFO, "mounting ext2 file system "
				 "using the ext4 subsystem");
		else {
			/*
			 * If we're probing be silent, if this looks like
			 * it's actually an ext[34] filesystem.
			 */
			if (silent && ext4_feature_set_ok(sb, sb_rdonly(sb)))
				return -EINVAL;
			ext4_msg(sb, KERN_ERR, "couldn't mount as ext2 due "
				 "to feature incompatibilities");
			return -EINVAL;
		}
	}

	if (IS_EXT3_SB(sb)) {
		if (ext3_feature_set_ok(sb))
			ext4_msg(sb, KERN_INFO, "mounting ext3 file system "
				 "using the ext4 subsystem");
		else {
			/*
			 * If we're probing be silent, if this looks like
			 * it's actually an ext4 filesystem.
			 */
			if (silent && ext4_feature_set_ok(sb, sb_rdonly(sb)))
				return -EINVAL;
			ext4_msg(sb, KERN_ERR, "couldn't mount as ext3 due "
				 "to feature incompatibilities");
			return -EINVAL;
		}
	}

	/*
	 * Check feature flags regardless of the revision level, since we
	 * previously didn't change the revision level when setting the flags,
	 * so there is a chance incompat flags are set on a rev 0 filesystem.
	 */
	if (!ext4_feature_set_ok(sb, (sb_rdonly(sb))))
		return -EINVAL;

	if (sbi->s_daxdev) {
		if (sb->s_blocksize == PAGE_SIZE)
			set_bit(EXT4_FLAGS_BDEV_IS_DAX, &sbi->s_ext4_flags);
		else
			ext4_msg(sb, KERN_ERR, "unsupported blocksize for DAX\n");
	}

	if (sbi->s_mount_opt & EXT4_MOUNT_DAX_ALWAYS) {
		if (ext4_has_feature_inline_data(sb)) {
			ext4_msg(sb, KERN_ERR, "Cannot use DAX on a filesystem"
					" that may contain inline data");
			return -EINVAL;
		}
		if (!test_bit(EXT4_FLAGS_BDEV_IS_DAX, &sbi->s_ext4_flags)) {
			ext4_msg(sb, KERN_ERR,
				"DAX unsupported by block device.");
			return -EINVAL;
		}
	}

	if (ext4_has_feature_encrypt(sb) && es->s_encryption_level) {
		ext4_msg(sb, KERN_ERR, "Unsupported encryption level %d",
			 es->s_encryption_level);
		return -EINVAL;
	}

	return 0;
}

static int ext4_check_geometry(struct super_block *sb,
			       struct ext4_super_block *es)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	__u64 blocks_count;
	int err;

	if (le16_to_cpu(sbi->s_es->s_reserved_gdt_blocks) > (sb->s_blocksize / 4)) {
		ext4_msg(sb, KERN_ERR,
			 "Number of reserved GDT blocks insanely large: %d",
			 le16_to_cpu(sbi->s_es->s_reserved_gdt_blocks));
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Test whether we have more sectors than will fit in sector_t,
	 * and whether the max offset is addressable by the page cache.
	 */
	err = generic_check_addressable(sb->s_blocksize_bits,
					ext4_blocks_count(es));
	if (err) {
		ext4_msg(sb, KERN_ERR, "filesystem"
			 " too large to mount safely on this system");
<<<<<<< HEAD
		if (sizeof(sector_t) < 8)
			ext4_msg(sb, KERN_WARNING, "CONFIG_LBDAF not enabled");
		ret = err;
		goto failed_mount;
	}

	if (EXT4_BLOCKS_PER_GROUP(sb) == 0)
		goto cantfind_ext4;

	/* check blocks count against device size */
	blocks_count = sb->s_bdev->bd_inode->i_size >> sb->s_blocksize_bits;
=======
		return err;
	}

	/* check blocks count against device size */
	blocks_count = sb_bdev_nr_blocks(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (blocks_count && ext4_blocks_count(es) > blocks_count) {
		ext4_msg(sb, KERN_WARNING, "bad geometry: block count %llu "
		       "exceeds size of device (%llu blocks)",
		       ext4_blocks_count(es), blocks_count);
<<<<<<< HEAD
		goto failed_mount;
=======
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * It makes no sense for the first data block to be beyond the end
	 * of the filesystem.
	 */
	if (le32_to_cpu(es->s_first_data_block) >= ext4_blocks_count(es)) {
		ext4_msg(sb, KERN_WARNING, "bad geometry: first data "
			 "block %u is beyond end of filesystem (%llu)",
			 le32_to_cpu(es->s_first_data_block),
			 ext4_blocks_count(es));
<<<<<<< HEAD
		goto failed_mount;
	}
=======
		return -EINVAL;
	}
	if ((es->s_first_data_block == 0) && (es->s_log_block_size == 0) &&
	    (sbi->s_cluster_ratio == 1)) {
		ext4_msg(sb, KERN_WARNING, "bad geometry: first data "
			 "block is 0 with a 1k block and cluster size");
		return -EINVAL;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	blocks_count = (ext4_blocks_count(es) -
			le32_to_cpu(es->s_first_data_block) +
			EXT4_BLOCKS_PER_GROUP(sb) - 1);
	do_div(blocks_count, EXT4_BLOCKS_PER_GROUP(sb));
	if (blocks_count > ((uint64_t)1<<32) - EXT4_DESC_PER_BLOCK(sb)) {
<<<<<<< HEAD
		ext4_msg(sb, KERN_WARNING, "groups count too large: %u "
		       "(block count %llu, first data block %u, "
		       "blocks per group %lu)", sbi->s_groups_count,
		       ext4_blocks_count(es),
		       le32_to_cpu(es->s_first_data_block),
		       EXT4_BLOCKS_PER_GROUP(sb));
		goto failed_mount;
=======
		ext4_msg(sb, KERN_WARNING, "groups count too large: %llu "
		       "(block count %llu, first data block %u, "
		       "blocks per group %lu)", blocks_count,
		       ext4_blocks_count(es),
		       le32_to_cpu(es->s_first_data_block),
		       EXT4_BLOCKS_PER_GROUP(sb));
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	sbi->s_groups_count = blocks_count;
	sbi->s_blockfile_groups = min_t(ext4_group_t, sbi->s_groups_count,
			(EXT4_MAX_BLOCK_FILE_PHYS / EXT4_BLOCKS_PER_GROUP(sb)));
<<<<<<< HEAD
	db_count = (sbi->s_groups_count + EXT4_DESC_PER_BLOCK(sb) - 1) /
		   EXT4_DESC_PER_BLOCK(sb);
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_FLEX_BG)) {
		if (le32_to_cpu(es->s_first_meta_bg) >= db_count) {
=======
	if (((u64)sbi->s_groups_count * sbi->s_inodes_per_group) !=
	    le32_to_cpu(es->s_inodes_count)) {
		ext4_msg(sb, KERN_ERR, "inodes count not valid: %u vs %llu",
			 le32_to_cpu(es->s_inodes_count),
			 ((u64)sbi->s_groups_count * sbi->s_inodes_per_group));
		return -EINVAL;
	}

	return 0;
}

static int ext4_group_desc_init(struct super_block *sb,
				struct ext4_super_block *es,
				ext4_fsblk_t logical_sb_block,
				ext4_group_t *first_not_zeroed)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	unsigned int db_count;
	ext4_fsblk_t block;
	int i;

	db_count = (sbi->s_groups_count + EXT4_DESC_PER_BLOCK(sb) - 1) /
		   EXT4_DESC_PER_BLOCK(sb);
	if (ext4_has_feature_meta_bg(sb)) {
		if (le32_to_cpu(es->s_first_meta_bg) > db_count) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ext4_msg(sb, KERN_WARNING,
				 "first meta block group too large: %u "
				 "(group descriptor block count %u)",
				 le32_to_cpu(es->s_first_meta_bg), db_count);
<<<<<<< HEAD
			goto failed_mount;
		}
	}
	sbi->s_group_desc = ext4_kvmalloc(db_count *
					  sizeof(struct buffer_head *),
					  GFP_KERNEL);
	if (sbi->s_group_desc == NULL) {
		ext4_msg(sb, KERN_ERR, "not enough memory");
		goto failed_mount;
	}

	if (ext4_proc_root)
		sbi->s_proc = proc_mkdir(sb->s_id, ext4_proc_root);

	if (sbi->s_proc)
		proc_create_data("options", S_IRUGO, sbi->s_proc,
				 &ext4_seq_options_fops, sb);

	bgl_lock_init(sbi->s_blockgroup_lock);

	for (i = 0; i < db_count; i++) {
		block = descriptor_loc(sb, logical_sb_block, i);
		sbi->s_group_desc[i] = sb_bread(sb, block);
		if (!sbi->s_group_desc[i]) {
			ext4_msg(sb, KERN_ERR,
			       "can't read group descriptor %d", i);
			db_count = i;
			goto failed_mount2;
		}
	}
	sbi->s_gdb_count = db_count;
	if (!ext4_check_descriptors(sb, logical_sb_block, &first_not_zeroed)) {
		ext4_msg(sb, KERN_ERR, "group descriptors corrupted!");
		goto failed_mount2;
	}
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_FLEX_BG))
		if (!ext4_fill_flex_info(sb)) {
			ext4_msg(sb, KERN_ERR,
			       "unable to initialize "
			       "flex_bg meta info!");
			goto failed_mount2;
		}

	get_random_bytes(&sbi->s_next_generation, sizeof(u32));
	spin_lock_init(&sbi->s_next_gen_lock);

	init_timer(&sbi->s_err_report);
	sbi->s_err_report.function = print_daily_error_info;
	sbi->s_err_report.data = (unsigned long) sb;

	err = percpu_counter_init(&sbi->s_freeclusters_counter,
			ext4_count_free_clusters(sb));
	if (!err) {
		err = percpu_counter_init(&sbi->s_freeinodes_counter,
				ext4_count_free_inodes(sb));
	}
	if (!err) {
		err = percpu_counter_init(&sbi->s_dirs_counter,
				ext4_count_dirs(sb));
	}
	if (!err) {
		err = percpu_counter_init(&sbi->s_dirtyclusters_counter, 0);
	}
	if (err) {
		ext4_msg(sb, KERN_ERR, "insufficient memory");
		goto failed_mount3;
	}

	sbi->s_stripe = ext4_get_stripe_size(sbi);
	sbi->s_max_writeback_mb_bump = 128;

	/*
	 * set up enough so that it can read an inode
	 */
	if (!test_opt(sb, NOLOAD) &&
	    EXT4_HAS_COMPAT_FEATURE(sb, EXT4_FEATURE_COMPAT_HAS_JOURNAL))
		sb->s_op = &ext4_sops;
	else
		sb->s_op = &ext4_nojournal_sops;
	sb->s_export_op = &ext4_export_ops;
	sb->s_xattr = ext4_xattr_handlers;
#ifdef CONFIG_QUOTA
	sb->s_qcop = &ext4_qctl_operations;
	sb->dq_op = &ext4_quota_operations;
#endif
	memcpy(sb->s_uuid, es->s_uuid, sizeof(es->s_uuid));

	INIT_LIST_HEAD(&sbi->s_orphan); /* unlinked but open files */
	mutex_init(&sbi->s_orphan_lock);
	sbi->s_resize_flags = 0;

	sb->s_root = NULL;

	needs_recovery = (es->s_last_orphan != 0 ||
			  EXT4_HAS_INCOMPAT_FEATURE(sb,
				    EXT4_FEATURE_INCOMPAT_RECOVER));

	if (EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_MMP) &&
	    !(sb->s_flags & MS_RDONLY))
		if (ext4_multi_mount_protect(sb, le64_to_cpu(es->s_mmp_block)))
			goto failed_mount3;

	/*
	 * The first inode we look at is the journal inode.  Don't try
	 * root first: it may be modified in the journal!
	 */
	if (!test_opt(sb, NOLOAD) &&
	    EXT4_HAS_COMPAT_FEATURE(sb, EXT4_FEATURE_COMPAT_HAS_JOURNAL)) {
		if (ext4_load_journal(sb, es, journal_devnum))
			goto failed_mount3;
	} else if (test_opt(sb, NOLOAD) && !(sb->s_flags & MS_RDONLY) &&
	      EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER)) {
		ext4_msg(sb, KERN_ERR, "required journal recovery "
		       "suppressed and not mounted read-only");
		goto failed_mount_wq;
	} else {
		clear_opt(sb, DATA_FLAGS);
		sbi->s_journal = NULL;
		needs_recovery = 0;
		goto no_journal;
	}

	if (ext4_blocks_count(es) > 0xffffffffULL &&
	    !jbd2_journal_set_features(EXT4_SB(sb)->s_journal, 0, 0,
				       JBD2_FEATURE_INCOMPAT_64BIT)) {
		ext4_msg(sb, KERN_ERR, "Failed to set 64-bit journal feature");
		goto failed_mount_wq;
	}

	if (test_opt(sb, JOURNAL_ASYNC_COMMIT)) {
		jbd2_journal_set_features(sbi->s_journal,
				JBD2_FEATURE_COMPAT_CHECKSUM, 0,
				JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT);
	} else if (test_opt(sb, JOURNAL_CHECKSUM)) {
		jbd2_journal_set_features(sbi->s_journal,
				JBD2_FEATURE_COMPAT_CHECKSUM, 0, 0);
		jbd2_journal_clear_features(sbi->s_journal, 0, 0,
				JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT);
	} else {
		jbd2_journal_clear_features(sbi->s_journal,
				JBD2_FEATURE_COMPAT_CHECKSUM, 0,
				JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT);
=======
			return -EINVAL;
		}
	}
	rcu_assign_pointer(sbi->s_group_desc,
			   kvmalloc_array(db_count,
					  sizeof(struct buffer_head *),
					  GFP_KERNEL));
	if (sbi->s_group_desc == NULL) {
		ext4_msg(sb, KERN_ERR, "not enough memory");
		return -ENOMEM;
	}

	bgl_lock_init(sbi->s_blockgroup_lock);

	/* Pre-read the descriptors into the buffer cache */
	for (i = 0; i < db_count; i++) {
		block = descriptor_loc(sb, logical_sb_block, i);
		ext4_sb_breadahead_unmovable(sb, block);
	}

	for (i = 0; i < db_count; i++) {
		struct buffer_head *bh;

		block = descriptor_loc(sb, logical_sb_block, i);
		bh = ext4_sb_bread_unmovable(sb, block);
		if (IS_ERR(bh)) {
			ext4_msg(sb, KERN_ERR,
			       "can't read group descriptor %d", i);
			sbi->s_gdb_count = i;
			return PTR_ERR(bh);
		}
		rcu_read_lock();
		rcu_dereference(sbi->s_group_desc)[i] = bh;
		rcu_read_unlock();
	}
	sbi->s_gdb_count = db_count;
	if (!ext4_check_descriptors(sb, logical_sb_block, first_not_zeroed)) {
		ext4_msg(sb, KERN_ERR, "group descriptors corrupted!");
		return -EFSCORRUPTED;
	}

	return 0;
}

static int ext4_load_and_init_journal(struct super_block *sb,
				      struct ext4_super_block *es,
				      struct ext4_fs_context *ctx)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	int err;

	err = ext4_load_journal(sb, es, ctx->journal_devnum);
	if (err)
		return err;

	if (ext4_has_feature_64bit(sb) &&
	    !jbd2_journal_set_features(EXT4_SB(sb)->s_journal, 0, 0,
				       JBD2_FEATURE_INCOMPAT_64BIT)) {
		ext4_msg(sb, KERN_ERR, "Failed to set 64-bit journal feature");
		goto out;
	}

	if (!set_journal_csum_feature_set(sb)) {
		ext4_msg(sb, KERN_ERR, "Failed to set journal checksum "
			 "feature set");
		goto out;
	}

	if (test_opt2(sb, JOURNAL_FAST_COMMIT) &&
		!jbd2_journal_set_features(EXT4_SB(sb)->s_journal, 0, 0,
					  JBD2_FEATURE_INCOMPAT_FAST_COMMIT)) {
		ext4_msg(sb, KERN_ERR,
			"Failed to set fast commit journal feature");
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* We have now updated the journal if required, so we can
	 * validate the data journaling mode. */
	switch (test_opt(sb, DATA_FLAGS)) {
	case 0:
		/* No mode set, assume a default based on the journal
		 * capabilities: ORDERED_DATA if the journal can
		 * cope, else JOURNAL_DATA
		 */
		if (jbd2_journal_check_available_features
<<<<<<< HEAD
		    (sbi->s_journal, 0, 0, JBD2_FEATURE_INCOMPAT_REVOKE))
			set_opt(sb, ORDERED_DATA);
		else
			set_opt(sb, JOURNAL_DATA);
=======
		    (sbi->s_journal, 0, 0, JBD2_FEATURE_INCOMPAT_REVOKE)) {
			set_opt(sb, ORDERED_DATA);
			sbi->s_def_mount_opt |= EXT4_MOUNT_ORDERED_DATA;
		} else {
			set_opt(sb, JOURNAL_DATA);
			sbi->s_def_mount_opt |= EXT4_MOUNT_JOURNAL_DATA;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case EXT4_MOUNT_ORDERED_DATA:
	case EXT4_MOUNT_WRITEBACK_DATA:
		if (!jbd2_journal_check_available_features
		    (sbi->s_journal, 0, 0, JBD2_FEATURE_INCOMPAT_REVOKE)) {
			ext4_msg(sb, KERN_ERR, "Journal does not support "
			       "requested data journaling mode");
<<<<<<< HEAD
			goto failed_mount_wq;
		}
	default:
		break;
	}
	set_task_ioprio(sbi->s_journal->j_task, journal_ioprio);

	sbi->s_journal->j_commit_callback = ext4_journal_commit_callback;

	/*
	 * The journal may have updated the bg summary counts, so we
	 * need to update the global counters.
	 */
	percpu_counter_set(&sbi->s_freeclusters_counter,
			   ext4_count_free_clusters(sb));
	percpu_counter_set(&sbi->s_freeinodes_counter,
			   ext4_count_free_inodes(sb));
	percpu_counter_set(&sbi->s_dirs_counter,
			   ext4_count_dirs(sb));
	percpu_counter_set(&sbi->s_dirtyclusters_counter, 0);

no_journal:
=======
			goto out;
		}
		break;
	default:
		break;
	}

	if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_ORDERED_DATA &&
	    test_opt(sb, JOURNAL_ASYNC_COMMIT)) {
		ext4_msg(sb, KERN_ERR, "can't mount with "
			"journal_async_commit in data=ordered mode");
		goto out;
	}

	set_task_ioprio(sbi->s_journal->j_task, ctx->journal_ioprio);

	sbi->s_journal->j_submit_inode_data_buffers =
		ext4_journal_submit_inode_data_buffers;
	sbi->s_journal->j_finish_inode_data_buffers =
		ext4_journal_finish_inode_data_buffers;

	return 0;

out:
	/* flush s_sb_upd_work before destroying the journal. */
	flush_work(&sbi->s_sb_upd_work);
	jbd2_journal_destroy(sbi->s_journal);
	sbi->s_journal = NULL;
	return -EINVAL;
}

static int ext4_check_journal_data_mode(struct super_block *sb)
{
	if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_JOURNAL_DATA) {
		printk_once(KERN_WARNING "EXT4-fs: Warning: mounting with "
			    "data=journal disables delayed allocation, "
			    "dioread_nolock, O_DIRECT and fast_commit support!\n");
		/* can't mount with both data=journal and dioread_nolock. */
		clear_opt(sb, DIOREAD_NOLOCK);
		clear_opt2(sb, JOURNAL_FAST_COMMIT);
		if (test_opt2(sb, EXPLICIT_DELALLOC)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "both data=journal and delalloc");
			return -EINVAL;
		}
		if (test_opt(sb, DAX_ALWAYS)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "both data=journal and dax");
			return -EINVAL;
		}
		if (ext4_has_feature_encrypt(sb)) {
			ext4_msg(sb, KERN_WARNING,
				 "encrypted files will use data=ordered "
				 "instead of data journaling mode");
		}
		if (test_opt(sb, DELALLOC))
			clear_opt(sb, DELALLOC);
	} else {
		sb->s_iflags |= SB_I_CGROUPWB;
	}

	return 0;
}

static int ext4_load_super(struct super_block *sb, ext4_fsblk_t *lsb,
			   int silent)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es;
	ext4_fsblk_t logical_sb_block;
	unsigned long offset = 0;
	struct buffer_head *bh;
	int ret = -EINVAL;
	int blocksize;

	blocksize = sb_min_blocksize(sb, EXT4_MIN_BLOCK_SIZE);
	if (!blocksize) {
		ext4_msg(sb, KERN_ERR, "unable to set blocksize");
		return -EINVAL;
	}

	/*
	 * The ext4 superblock will not be buffer aligned for other than 1kB
	 * block sizes.  We need to calculate the offset from buffer start.
	 */
	if (blocksize != EXT4_MIN_BLOCK_SIZE) {
		logical_sb_block = sbi->s_sb_block * EXT4_MIN_BLOCK_SIZE;
		offset = do_div(logical_sb_block, blocksize);
	} else {
		logical_sb_block = sbi->s_sb_block;
	}

	bh = ext4_sb_bread_unmovable(sb, logical_sb_block);
	if (IS_ERR(bh)) {
		ext4_msg(sb, KERN_ERR, "unable to read superblock");
		return PTR_ERR(bh);
	}
	/*
	 * Note: s_es must be initialized as soon as possible because
	 *       some ext4 macro-instructions depend on its value
	 */
	es = (struct ext4_super_block *) (bh->b_data + offset);
	sbi->s_es = es;
	sb->s_magic = le16_to_cpu(es->s_magic);
	if (sb->s_magic != EXT4_SUPER_MAGIC) {
		if (!silent)
			ext4_msg(sb, KERN_ERR, "VFS: Can't find ext4 filesystem");
		goto out;
	}

	if (le32_to_cpu(es->s_log_block_size) >
	    (EXT4_MAX_BLOCK_LOG_SIZE - EXT4_MIN_BLOCK_LOG_SIZE)) {
		ext4_msg(sb, KERN_ERR,
			 "Invalid log block size: %u",
			 le32_to_cpu(es->s_log_block_size));
		goto out;
	}
	if (le32_to_cpu(es->s_log_cluster_size) >
	    (EXT4_MAX_CLUSTER_LOG_SIZE - EXT4_MIN_BLOCK_LOG_SIZE)) {
		ext4_msg(sb, KERN_ERR,
			 "Invalid log cluster size: %u",
			 le32_to_cpu(es->s_log_cluster_size));
		goto out;
	}

	blocksize = EXT4_MIN_BLOCK_SIZE << le32_to_cpu(es->s_log_block_size);

	/*
	 * If the default block size is not the same as the real block size,
	 * we need to reload it.
	 */
	if (sb->s_blocksize == blocksize) {
		*lsb = logical_sb_block;
		sbi->s_sbh = bh;
		return 0;
	}

	/*
	 * bh must be released before kill_bdev(), otherwise
	 * it won't be freed and its page also. kill_bdev()
	 * is called by sb_set_blocksize().
	 */
	brelse(bh);
	/* Validate the filesystem blocksize */
	if (!sb_set_blocksize(sb, blocksize)) {
		ext4_msg(sb, KERN_ERR, "bad block size %d",
				blocksize);
		bh = NULL;
		goto out;
	}

	logical_sb_block = sbi->s_sb_block * EXT4_MIN_BLOCK_SIZE;
	offset = do_div(logical_sb_block, blocksize);
	bh = ext4_sb_bread_unmovable(sb, logical_sb_block);
	if (IS_ERR(bh)) {
		ext4_msg(sb, KERN_ERR, "Can't read superblock on 2nd try");
		ret = PTR_ERR(bh);
		bh = NULL;
		goto out;
	}
	es = (struct ext4_super_block *)(bh->b_data + offset);
	sbi->s_es = es;
	if (es->s_magic != cpu_to_le16(EXT4_SUPER_MAGIC)) {
		ext4_msg(sb, KERN_ERR, "Magic mismatch, very weird!");
		goto out;
	}
	*lsb = logical_sb_block;
	sbi->s_sbh = bh;
	return 0;
out:
	brelse(bh);
	return ret;
}

static void ext4_hash_info_init(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
	unsigned int i;

	for (i = 0; i < 4; i++)
		sbi->s_hash_seed[i] = le32_to_cpu(es->s_hash_seed[i]);

	sbi->s_def_hash_version = es->s_def_hash_version;
	if (ext4_has_feature_dir_index(sb)) {
		i = le32_to_cpu(es->s_flags);
		if (i & EXT2_FLAGS_UNSIGNED_HASH)
			sbi->s_hash_unsigned = 3;
		else if ((i & EXT2_FLAGS_SIGNED_HASH) == 0) {
#ifdef __CHAR_UNSIGNED__
			if (!sb_rdonly(sb))
				es->s_flags |=
					cpu_to_le32(EXT2_FLAGS_UNSIGNED_HASH);
			sbi->s_hash_unsigned = 3;
#else
			if (!sb_rdonly(sb))
				es->s_flags |=
					cpu_to_le32(EXT2_FLAGS_SIGNED_HASH);
#endif
		}
	}
}

static int ext4_block_group_meta_init(struct super_block *sb, int silent)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
	int has_huge_files;

	has_huge_files = ext4_has_feature_huge_file(sb);
	sbi->s_bitmap_maxbytes = ext4_max_bitmap_size(sb->s_blocksize_bits,
						      has_huge_files);
	sb->s_maxbytes = ext4_max_size(sb->s_blocksize_bits, has_huge_files);

	sbi->s_desc_size = le16_to_cpu(es->s_desc_size);
	if (ext4_has_feature_64bit(sb)) {
		if (sbi->s_desc_size < EXT4_MIN_DESC_SIZE_64BIT ||
		    sbi->s_desc_size > EXT4_MAX_DESC_SIZE ||
		    !is_power_of_2(sbi->s_desc_size)) {
			ext4_msg(sb, KERN_ERR,
			       "unsupported descriptor size %lu",
			       sbi->s_desc_size);
			return -EINVAL;
		}
	} else
		sbi->s_desc_size = EXT4_MIN_DESC_SIZE;

	sbi->s_blocks_per_group = le32_to_cpu(es->s_blocks_per_group);
	sbi->s_inodes_per_group = le32_to_cpu(es->s_inodes_per_group);

	sbi->s_inodes_per_block = sb->s_blocksize / EXT4_INODE_SIZE(sb);
	if (sbi->s_inodes_per_block == 0 || sbi->s_blocks_per_group == 0) {
		if (!silent)
			ext4_msg(sb, KERN_ERR, "VFS: Can't find ext4 filesystem");
		return -EINVAL;
	}
	if (sbi->s_inodes_per_group < sbi->s_inodes_per_block ||
	    sbi->s_inodes_per_group > sb->s_blocksize * 8) {
		ext4_msg(sb, KERN_ERR, "invalid inodes per group: %lu\n",
			 sbi->s_inodes_per_group);
		return -EINVAL;
	}
	sbi->s_itb_per_group = sbi->s_inodes_per_group /
					sbi->s_inodes_per_block;
	sbi->s_desc_per_block = sb->s_blocksize / EXT4_DESC_SIZE(sb);
	sbi->s_mount_state = le16_to_cpu(es->s_state) & ~EXT4_FC_REPLAY;
	sbi->s_addr_per_block_bits = ilog2(EXT4_ADDR_PER_BLOCK(sb));
	sbi->s_desc_per_block_bits = ilog2(EXT4_DESC_PER_BLOCK(sb));

	return 0;
}

static int __ext4_fill_super(struct fs_context *fc, struct super_block *sb)
{
	struct ext4_super_block *es = NULL;
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	ext4_fsblk_t logical_sb_block;
	struct inode *root;
	int needs_recovery;
	int err;
	ext4_group_t first_not_zeroed;
	struct ext4_fs_context *ctx = fc->fs_private;
	int silent = fc->sb_flags & SB_SILENT;

	/* Set defaults for the variables that will be set during parsing */
	if (!(ctx->spec & EXT4_SPEC_JOURNAL_IOPRIO))
		ctx->journal_ioprio = DEFAULT_JOURNAL_IOPRIO;

	sbi->s_inode_readahead_blks = EXT4_DEF_INODE_READAHEAD_BLKS;
	sbi->s_sectors_written_start =
		part_stat_read(sb->s_bdev, sectors[STAT_WRITE]);

	err = ext4_load_super(sb, &logical_sb_block, silent);
	if (err)
		goto out_fail;

	es = sbi->s_es;
	sbi->s_kbytes_written = le64_to_cpu(es->s_kbytes_written);

	err = ext4_init_metadata_csum(sb, es);
	if (err)
		goto failed_mount;

	ext4_set_def_opts(sb, es);

	sbi->s_resuid = make_kuid(&init_user_ns, le16_to_cpu(es->s_def_resuid));
	sbi->s_resgid = make_kgid(&init_user_ns, le16_to_cpu(es->s_def_resgid));
	sbi->s_commit_interval = JBD2_DEFAULT_MAX_COMMIT_AGE * HZ;
	sbi->s_min_batch_time = EXT4_DEF_MIN_BATCH_TIME;
	sbi->s_max_batch_time = EXT4_DEF_MAX_BATCH_TIME;

	/*
	 * set default s_li_wait_mult for lazyinit, for the case there is
	 * no mount option specified.
	 */
	sbi->s_li_wait_mult = EXT4_DEF_LI_WAIT_MULT;

	err = ext4_inode_info_init(sb, es);
	if (err)
		goto failed_mount;

	err = parse_apply_sb_mount_options(sb, ctx);
	if (err < 0)
		goto failed_mount;

	sbi->s_def_mount_opt = sbi->s_mount_opt;
	sbi->s_def_mount_opt2 = sbi->s_mount_opt2;

	err = ext4_check_opt_consistency(fc, sb);
	if (err < 0)
		goto failed_mount;

	ext4_apply_options(fc, sb);

	err = ext4_encoding_init(sb, es);
	if (err)
		goto failed_mount;

	err = ext4_check_journal_data_mode(sb);
	if (err)
		goto failed_mount;

	sb->s_flags = (sb->s_flags & ~SB_POSIXACL) |
		(test_opt(sb, POSIX_ACL) ? SB_POSIXACL : 0);

	/* i_version is always enabled now */
	sb->s_flags |= SB_I_VERSION;

	err = ext4_check_feature_compatibility(sb, es, silent);
	if (err)
		goto failed_mount;

	err = ext4_block_group_meta_init(sb, silent);
	if (err)
		goto failed_mount;

	ext4_hash_info_init(sb);

	err = ext4_handle_clustersize(sb);
	if (err)
		goto failed_mount;

	err = ext4_check_geometry(sb, es);
	if (err)
		goto failed_mount;

	timer_setup(&sbi->s_err_report, print_daily_error_info, 0);
	spin_lock_init(&sbi->s_error_lock);
	INIT_WORK(&sbi->s_sb_upd_work, update_super_work);

	err = ext4_group_desc_init(sb, es, logical_sb_block, &first_not_zeroed);
	if (err)
		goto failed_mount3;

	err = ext4_es_register_shrinker(sbi);
	if (err)
		goto failed_mount3;

	sbi->s_stripe = ext4_get_stripe_size(sbi);
	/*
	 * It's hard to get stripe aligned blocks if stripe is not aligned with
	 * cluster, just disable stripe and alert user to simpfy code and avoid
	 * stripe aligned allocation which will rarely successes.
	 */
	if (sbi->s_stripe > 0 && sbi->s_cluster_ratio > 1 &&
	    sbi->s_stripe % sbi->s_cluster_ratio != 0) {
		ext4_msg(sb, KERN_WARNING,
			 "stripe (%lu) is not aligned with cluster size (%u), "
			 "stripe is disabled",
			 sbi->s_stripe, sbi->s_cluster_ratio);
		sbi->s_stripe = 0;
	}
	sbi->s_extent_max_zeroout_kb = 32;

	/*
	 * set up enough so that it can read an inode
	 */
	sb->s_op = &ext4_sops;
	sb->s_export_op = &ext4_export_ops;
	sb->s_xattr = ext4_xattr_handlers;
#ifdef CONFIG_FS_ENCRYPTION
	sb->s_cop = &ext4_cryptops;
#endif
#ifdef CONFIG_FS_VERITY
	sb->s_vop = &ext4_verityops;
#endif
#ifdef CONFIG_QUOTA
	sb->dq_op = &ext4_quota_operations;
	if (ext4_has_feature_quota(sb))
		sb->s_qcop = &dquot_quotactl_sysfile_ops;
	else
		sb->s_qcop = &ext4_qctl_operations;
	sb->s_quota_types = QTYPE_MASK_USR | QTYPE_MASK_GRP | QTYPE_MASK_PRJ;
#endif
	super_set_uuid(sb, es->s_uuid, sizeof(es->s_uuid));

	INIT_LIST_HEAD(&sbi->s_orphan); /* unlinked but open files */
	mutex_init(&sbi->s_orphan_lock);

	ext4_fast_commit_init(sb);

	sb->s_root = NULL;

	needs_recovery = (es->s_last_orphan != 0 ||
			  ext4_has_feature_orphan_present(sb) ||
			  ext4_has_feature_journal_needs_recovery(sb));

	if (ext4_has_feature_mmp(sb) && !sb_rdonly(sb)) {
		err = ext4_multi_mount_protect(sb, le64_to_cpu(es->s_mmp_block));
		if (err)
			goto failed_mount3a;
	}

	err = -EINVAL;
	/*
	 * The first inode we look at is the journal inode.  Don't try
	 * root first: it may be modified in the journal!
	 */
	if (!test_opt(sb, NOLOAD) && ext4_has_feature_journal(sb)) {
		err = ext4_load_and_init_journal(sb, es, ctx);
		if (err)
			goto failed_mount3a;
	} else if (test_opt(sb, NOLOAD) && !sb_rdonly(sb) &&
		   ext4_has_feature_journal_needs_recovery(sb)) {
		ext4_msg(sb, KERN_ERR, "required journal recovery "
		       "suppressed and not mounted read-only");
		goto failed_mount3a;
	} else {
		/* Nojournal mode, all journal mount options are illegal */
		if (test_opt(sb, JOURNAL_ASYNC_COMMIT)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "journal_async_commit, fs mounted w/o journal");
			goto failed_mount3a;
		}

		if (test_opt2(sb, EXPLICIT_JOURNAL_CHECKSUM)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "journal_checksum, fs mounted w/o journal");
			goto failed_mount3a;
		}
		if (sbi->s_commit_interval != JBD2_DEFAULT_MAX_COMMIT_AGE*HZ) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "commit=%lu, fs mounted w/o journal",
				 sbi->s_commit_interval / HZ);
			goto failed_mount3a;
		}
		if (EXT4_MOUNT_DATA_FLAGS &
		    (sbi->s_mount_opt ^ sbi->s_def_mount_opt)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "data=, fs mounted w/o journal");
			goto failed_mount3a;
		}
		sbi->s_def_mount_opt &= ~EXT4_MOUNT_JOURNAL_CHECKSUM;
		clear_opt(sb, JOURNAL_CHECKSUM);
		clear_opt(sb, DATA_FLAGS);
		clear_opt2(sb, JOURNAL_FAST_COMMIT);
		sbi->s_journal = NULL;
		needs_recovery = 0;
	}

	if (!test_opt(sb, NO_MBCACHE)) {
		sbi->s_ea_block_cache = ext4_xattr_create_cache();
		if (!sbi->s_ea_block_cache) {
			ext4_msg(sb, KERN_ERR,
				 "Failed to create ea_block_cache");
			err = -EINVAL;
			goto failed_mount_wq;
		}

		if (ext4_has_feature_ea_inode(sb)) {
			sbi->s_ea_inode_cache = ext4_xattr_create_cache();
			if (!sbi->s_ea_inode_cache) {
				ext4_msg(sb, KERN_ERR,
					 "Failed to create ea_inode_cache");
				err = -EINVAL;
				goto failed_mount_wq;
			}
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Get the # of file system overhead blocks from the
	 * superblock if present.
	 */
<<<<<<< HEAD
	if (es->s_overhead_clusters)
		sbi->s_overhead = le32_to_cpu(es->s_overhead_clusters);
	else {
		ret = ext4_calculate_overhead(sb);
		if (ret)
=======
	sbi->s_overhead = le32_to_cpu(es->s_overhead_clusters);
	/* ignore the precalculated value if it is ridiculous */
	if (sbi->s_overhead > ext4_blocks_count(es))
		sbi->s_overhead = 0;
	/*
	 * If the bigalloc feature is not enabled recalculating the
	 * overhead doesn't take long, so we might as well just redo
	 * it to make sure we are using the correct value.
	 */
	if (!ext4_has_feature_bigalloc(sb))
		sbi->s_overhead = 0;
	if (sbi->s_overhead == 0) {
		err = ext4_calculate_overhead(sb);
		if (err)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto failed_mount_wq;
	}

	/*
	 * The maximum number of concurrent works can be high and
	 * concurrency isn't really necessary.  Limit it to 1.
	 */
<<<<<<< HEAD
	EXT4_SB(sb)->dio_unwritten_wq =
		alloc_workqueue("ext4-dio-unwritten", WQ_MEM_RECLAIM | WQ_UNBOUND, 1);
	if (!EXT4_SB(sb)->dio_unwritten_wq) {
		printk(KERN_ERR "EXT4-fs: failed to create DIO workqueue\n");
		goto failed_mount_wq;
=======
	EXT4_SB(sb)->rsv_conversion_wq =
		alloc_workqueue("ext4-rsv-conversion", WQ_MEM_RECLAIM | WQ_UNBOUND, 1);
	if (!EXT4_SB(sb)->rsv_conversion_wq) {
		printk(KERN_ERR "EXT4-fs: failed to create workqueue\n");
		err = -ENOMEM;
		goto failed_mount4;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * The jbd2_journal_load will have done any necessary log recovery,
	 * so we can safely mount the rest of the filesystem now.
	 */

<<<<<<< HEAD
	root = ext4_iget(sb, EXT4_ROOT_INO);
	if (IS_ERR(root)) {
		ext4_msg(sb, KERN_ERR, "get root inode failed");
		ret = PTR_ERR(root);
=======
	root = ext4_iget(sb, EXT4_ROOT_INO, EXT4_IGET_SPECIAL);
	if (IS_ERR(root)) {
		ext4_msg(sb, KERN_ERR, "get root inode failed");
		err = PTR_ERR(root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		root = NULL;
		goto failed_mount4;
	}
	if (!S_ISDIR(root->i_mode) || !root->i_blocks || !root->i_size) {
		ext4_msg(sb, KERN_ERR, "corrupt root inode, run e2fsck");
		iput(root);
<<<<<<< HEAD
		goto failed_mount4;
	}
	sb->s_root = d_make_root(root);
	if (!sb->s_root) {
		ext4_msg(sb, KERN_ERR, "get root dentry failed");
		ret = -ENOMEM;
		goto failed_mount4;
	}

	if (ext4_setup_super(sb, es, sb->s_flags & MS_RDONLY))
		sb->s_flags |= MS_RDONLY;

	/* determine the minimum size of new large inodes, if present */
	if (sbi->s_inode_size > EXT4_GOOD_OLD_INODE_SIZE) {
		sbi->s_want_extra_isize = sizeof(struct ext4_inode) -
						     EXT4_GOOD_OLD_INODE_SIZE;
		if (EXT4_HAS_RO_COMPAT_FEATURE(sb,
				       EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE)) {
			if (sbi->s_want_extra_isize <
			    le16_to_cpu(es->s_want_extra_isize))
				sbi->s_want_extra_isize =
					le16_to_cpu(es->s_want_extra_isize);
			if (sbi->s_want_extra_isize <
			    le16_to_cpu(es->s_min_extra_isize))
				sbi->s_want_extra_isize =
					le16_to_cpu(es->s_min_extra_isize);
		}
	}
	/* Check if enough inode space is available */
	if (EXT4_GOOD_OLD_INODE_SIZE + sbi->s_want_extra_isize >
							sbi->s_inode_size) {
		sbi->s_want_extra_isize = sizeof(struct ext4_inode) -
						       EXT4_GOOD_OLD_INODE_SIZE;
		ext4_msg(sb, KERN_INFO, "required extra inode space not"
			 "available");
	}

	err = ext4_setup_system_zone(sb);
	if (err) {
		ext4_msg(sb, KERN_ERR, "failed to initialize system "
			 "zone (%d)", err);
		goto failed_mount4a;
	}

	ext4_ext_init(sb);
	err = ext4_mb_init(sb, needs_recovery);
=======
		err = -EFSCORRUPTED;
		goto failed_mount4;
	}

	generic_set_sb_d_ops(sb);
	sb->s_root = d_make_root(root);
	if (!sb->s_root) {
		ext4_msg(sb, KERN_ERR, "get root dentry failed");
		err = -ENOMEM;
		goto failed_mount4;
	}

	err = ext4_setup_super(sb, es, sb_rdonly(sb));
	if (err == -EROFS) {
		sb->s_flags |= SB_RDONLY;
	} else if (err)
		goto failed_mount4a;

	ext4_set_resv_clusters(sb);

	if (test_opt(sb, BLOCK_VALIDITY)) {
		err = ext4_setup_system_zone(sb);
		if (err) {
			ext4_msg(sb, KERN_ERR, "failed to initialize system "
				 "zone (%d)", err);
			goto failed_mount4a;
		}
	}
	ext4_fc_replay_cleanup(sb);

	ext4_ext_init(sb);

	/*
	 * Enable optimize_scan if number of groups is > threshold. This can be
	 * turned off by passing "mb_optimize_scan=0". This can also be
	 * turned on forcefully by passing "mb_optimize_scan=1".
	 */
	if (!(ctx->spec & EXT4_SPEC_mb_optimize_scan)) {
		if (sbi->s_groups_count >= MB_DEFAULT_LINEAR_SCAN_THRESHOLD)
			set_opt2(sb, MB_OPTIMIZE_SCAN);
		else
			clear_opt2(sb, MB_OPTIMIZE_SCAN);
	}

	err = ext4_mb_init(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err) {
		ext4_msg(sb, KERN_ERR, "failed to initialize mballoc (%d)",
			 err);
		goto failed_mount5;
	}

<<<<<<< HEAD
=======
	/*
	 * We can only set up the journal commit callback once
	 * mballoc is initialized
	 */
	if (sbi->s_journal)
		sbi->s_journal->j_commit_callback =
			ext4_journal_commit_callback;

	err = ext4_percpu_param_init(sbi);
	if (err)
		goto failed_mount6;

	if (ext4_has_feature_flex_bg(sb))
		if (!ext4_fill_flex_info(sb)) {
			ext4_msg(sb, KERN_ERR,
			       "unable to initialize "
			       "flex_bg meta info!");
			err = -ENOMEM;
			goto failed_mount6;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ext4_register_li_request(sb, first_not_zeroed);
	if (err)
		goto failed_mount6;

<<<<<<< HEAD
	sbi->s_kobj.kset = ext4_kset;
	init_completion(&sbi->s_kobj_unregister);
	err = kobject_init_and_add(&sbi->s_kobj, &ext4_ktype, NULL,
				   "%s", sb->s_id);
	if (err)
		goto failed_mount7;

	EXT4_SB(sb)->s_mount_state |= EXT4_ORPHAN_FS;
	ext4_orphan_cleanup(sb, es);
	EXT4_SB(sb)->s_mount_state &= ~EXT4_ORPHAN_FS;
	if (needs_recovery) {
		ext4_msg(sb, KERN_INFO, "recovery complete");
		ext4_mark_recovery_complete(sb, es);
	}
	if (EXT4_SB(sb)->s_journal) {
=======
	err = ext4_register_sysfs(sb);
	if (err)
		goto failed_mount7;

	err = ext4_init_orphan_info(sb);
	if (err)
		goto failed_mount8;
#ifdef CONFIG_QUOTA
	/* Enable quota usage during mount. */
	if (ext4_has_feature_quota(sb) && !sb_rdonly(sb)) {
		err = ext4_enable_quotas(sb);
		if (err)
			goto failed_mount9;
	}
#endif  /* CONFIG_QUOTA */

	/*
	 * Save the original bdev mapping's wb_err value which could be
	 * used to detect the metadata async write error.
	 */
	spin_lock_init(&sbi->s_bdev_wb_lock);
	errseq_check_and_advance(&sb->s_bdev->bd_inode->i_mapping->wb_err,
				 &sbi->s_bdev_wb_err);
	EXT4_SB(sb)->s_mount_state |= EXT4_ORPHAN_FS;
	ext4_orphan_cleanup(sb, es);
	EXT4_SB(sb)->s_mount_state &= ~EXT4_ORPHAN_FS;
	/*
	 * Update the checksum after updating free space/inode counters and
	 * ext4_orphan_cleanup. Otherwise the superblock can have an incorrect
	 * checksum in the buffer cache until it is written out and
	 * e2fsprogs programs trying to open a file system immediately
	 * after it is mounted can fail.
	 */
	ext4_superblock_csum_set(sb);
	if (needs_recovery) {
		ext4_msg(sb, KERN_INFO, "recovery complete");
		err = ext4_mark_recovery_complete(sb, es);
		if (err)
			goto failed_mount10;
	}

	if (test_opt(sb, DISCARD) && !bdev_max_discard_sectors(sb->s_bdev))
		ext4_msg(sb, KERN_WARNING,
			 "mounting with \"discard\" option, but the device does not support discard");

	if (es->s_error_count)
		mod_timer(&sbi->s_err_report, jiffies + 300*HZ); /* 5 minutes */

	/* Enable message ratelimiting. Default is 10 messages per 5 secs. */
	ratelimit_state_init(&sbi->s_err_ratelimit_state, 5 * HZ, 10);
	ratelimit_state_init(&sbi->s_warning_ratelimit_state, 5 * HZ, 10);
	ratelimit_state_init(&sbi->s_msg_ratelimit_state, 5 * HZ, 10);
	atomic_set(&sbi->s_warning_count, 0);
	atomic_set(&sbi->s_msg_count, 0);

	return 0;

failed_mount10:
	ext4_quotas_off(sb, EXT4_MAXQUOTAS);
failed_mount9: __maybe_unused
	ext4_release_orphan_info(sb);
failed_mount8:
	ext4_unregister_sysfs(sb);
	kobject_put(&sbi->s_kobj);
failed_mount7:
	ext4_unregister_li_request(sb);
failed_mount6:
	ext4_mb_release(sb);
	ext4_flex_groups_free(sbi);
	ext4_percpu_param_destroy(sbi);
failed_mount5:
	ext4_ext_release(sb);
	ext4_release_system_zone(sb);
failed_mount4a:
	dput(sb->s_root);
	sb->s_root = NULL;
failed_mount4:
	ext4_msg(sb, KERN_ERR, "mount failed");
	if (EXT4_SB(sb)->rsv_conversion_wq)
		destroy_workqueue(EXT4_SB(sb)->rsv_conversion_wq);
failed_mount_wq:
	ext4_xattr_destroy_cache(sbi->s_ea_inode_cache);
	sbi->s_ea_inode_cache = NULL;

	ext4_xattr_destroy_cache(sbi->s_ea_block_cache);
	sbi->s_ea_block_cache = NULL;

	if (sbi->s_journal) {
		/* flush s_sb_upd_work before journal destroy. */
		flush_work(&sbi->s_sb_upd_work);
		jbd2_journal_destroy(sbi->s_journal);
		sbi->s_journal = NULL;
	}
failed_mount3a:
	ext4_es_unregister_shrinker(sbi);
failed_mount3:
	/* flush s_sb_upd_work before sbi destroy */
	flush_work(&sbi->s_sb_upd_work);
	del_timer_sync(&sbi->s_err_report);
	ext4_stop_mmpd(sbi);
	ext4_group_desc_free(sbi);
failed_mount:
	if (sbi->s_chksum_driver)
		crypto_free_shash(sbi->s_chksum_driver);

#if IS_ENABLED(CONFIG_UNICODE)
	utf8_unload(sb->s_encoding);
#endif

#ifdef CONFIG_QUOTA
	for (unsigned int i = 0; i < EXT4_MAXQUOTAS; i++)
		kfree(get_qf_name(sb, sbi, i));
#endif
	fscrypt_free_dummy_policy(&sbi->s_dummy_enc_policy);
	brelse(sbi->s_sbh);
	if (sbi->s_journal_bdev_file) {
		invalidate_bdev(file_bdev(sbi->s_journal_bdev_file));
		bdev_fput(sbi->s_journal_bdev_file);
	}
out_fail:
	invalidate_bdev(sb->s_bdev);
	sb->s_fs_info = NULL;
	return err;
}

static int ext4_fill_super(struct super_block *sb, struct fs_context *fc)
{
	struct ext4_fs_context *ctx = fc->fs_private;
	struct ext4_sb_info *sbi;
	const char *descr;
	int ret;

	sbi = ext4_alloc_sbi(sb);
	if (!sbi)
		return -ENOMEM;

	fc->s_fs_info = sbi;

	/* Cleanup superblock name */
	strreplace(sb->s_id, '/', '!');

	sbi->s_sb_block = 1;	/* Default super block location */
	if (ctx->spec & EXT4_SPEC_s_sb_block)
		sbi->s_sb_block = ctx->s_sb_block;

	ret = __ext4_fill_super(fc, sb);
	if (ret < 0)
		goto free_sbi;

	if (sbi->s_journal) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_JOURNAL_DATA)
			descr = " journalled data mode";
		else if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_ORDERED_DATA)
			descr = " ordered data mode";
		else
			descr = " writeback data mode";
	} else
		descr = "out journal";

<<<<<<< HEAD
	ext4_msg(sb, KERN_INFO, "mounted filesystem with%s. "
		 "Opts: %s%s%s", descr, sbi->s_es->s_mount_opts,
		 *sbi->s_es->s_mount_opts ? "; " : "", orig_data);

	if (es->s_error_count)
		mod_timer(&sbi->s_err_report, jiffies + 300*HZ); /* 5 minutes */

	kfree(orig_data);
	return 0;

cantfind_ext4:
	if (!silent)
		ext4_msg(sb, KERN_ERR, "VFS: Can't find ext4 filesystem");
#ifdef CONFIG_MACH_LGE
/* LGE_CHANGE
 * add return code if ext4 superblock is damaged
 * 2014-01-16, B2-BSP-FS@lge.com
 */
	ret = -ESUPER;
#endif
	goto failed_mount;

failed_mount7:
#ifdef CONFIG_MACH_LGE
	printk(KERN_ERR "EXT4-fs: failed_mount7\n");
#endif
	ext4_unregister_li_request(sb);
failed_mount6:
#ifdef CONFIG_MACH_LGE
	printk(KERN_ERR "EXT4-fs: failed_mount6\n");
#endif
	ext4_mb_release(sb);
failed_mount5:
#ifdef CONFIG_MACH_LGE
	printk(KERN_ERR "EXT4-fs: failed_mount5\n");
#endif
	ext4_ext_release(sb);
	ext4_release_system_zone(sb);
failed_mount4a:
#ifdef CONFIG_MACH_LGE
	printk(KERN_ERR "EXT4-fs: failed_mount4a\n");
#endif
	dput(sb->s_root);
	sb->s_root = NULL;
failed_mount4:
#ifdef CONFIG_MACH_LGE
	printk(KERN_ERR "EXT4-fs: failed_mount4\n");
#endif
	ext4_msg(sb, KERN_ERR, "mount failed");
	destroy_workqueue(EXT4_SB(sb)->dio_unwritten_wq);
failed_mount_wq:
#ifdef CONFIG_MACH_LGE
	printk(KERN_ERR "EXT4-fs: failed_mount_wq\n");
#endif
	if (sbi->s_journal) {
		jbd2_journal_destroy(sbi->s_journal);
		sbi->s_journal = NULL;
	}
failed_mount3:
#ifdef CONFIG_MACH_LGE
	printk(KERN_ERR "EXT4-fs: failed_mount3\n");
#endif
	del_timer(&sbi->s_err_report);
	if (sbi->s_flex_groups)
		ext4_kvfree(sbi->s_flex_groups);
	percpu_counter_destroy(&sbi->s_freeclusters_counter);
	percpu_counter_destroy(&sbi->s_freeinodes_counter);
	percpu_counter_destroy(&sbi->s_dirs_counter);
	percpu_counter_destroy(&sbi->s_dirtyclusters_counter);
	if (sbi->s_mmp_tsk)
		kthread_stop(sbi->s_mmp_tsk);
failed_mount2:
#ifdef CONFIG_MACH_LGE
	printk(KERN_ERR "EXT4-fs: failed_mount2\n");
	ret = -ESUPER;
#endif
	for (i = 0; i < db_count; i++)
		brelse(sbi->s_group_desc[i]);
	ext4_kvfree(sbi->s_group_desc);
failed_mount:
#ifdef CONFIG_MACH_LGE
	printk(KERN_ERR "EXT4-fs: failed_mount\n");
#endif
	if (sbi->s_chksum_driver)
		crypto_free_shash(sbi->s_chksum_driver);
	if (sbi->s_proc) {
		remove_proc_entry("options", sbi->s_proc);
		remove_proc_entry(sb->s_id, ext4_proc_root);
	}
#ifdef CONFIG_QUOTA
	for (i = 0; i < MAXQUOTAS; i++)
		kfree(sbi->s_qf_names[i]);
#endif
	ext4_blkdev_remove(sbi);
	brelse(bh);
out_fail:
	sb->s_fs_info = NULL;
	kfree(sbi->s_blockgroup_lock);
	kfree(sbi);
out_free_orig:
	kfree(orig_data);
	return ret;
}

=======
	if (___ratelimit(&ext4_mount_msg_ratelimit, "EXT4-fs mount"))
		ext4_msg(sb, KERN_INFO, "mounted filesystem %pU %s with%s. "
			 "Quota mode: %s.", &sb->s_uuid,
			 sb_rdonly(sb) ? "ro" : "r/w", descr,
			 ext4_quota_mode(sb));

	/* Update the s_overhead_clusters if necessary */
	ext4_update_overhead(sb, false);
	return 0;

free_sbi:
	ext4_free_sbi(sbi);
	fc->s_fs_info = NULL;
	return ret;
}

static int ext4_get_tree(struct fs_context *fc)
{
	return get_tree_bdev(fc, ext4_fill_super);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Setup any per-fs journal parameters now.  We'll do this both on
 * initial mount, once the journal has been initialised but before we've
 * done any recovery; and again on any subsequent remount.
 */
static void ext4_init_journal_params(struct super_block *sb, journal_t *journal)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	journal->j_commit_interval = sbi->s_commit_interval;
	journal->j_min_batch_time = sbi->s_min_batch_time;
	journal->j_max_batch_time = sbi->s_max_batch_time;
<<<<<<< HEAD
=======
	ext4_fc_init(sb, journal);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	write_lock(&journal->j_state_lock);
	if (test_opt(sb, BARRIER))
		journal->j_flags |= JBD2_BARRIER;
	else
		journal->j_flags &= ~JBD2_BARRIER;
	if (test_opt(sb, DATA_ERR_ABORT))
		journal->j_flags |= JBD2_ABORT_ON_SYNCDATA_ERR;
	else
		journal->j_flags &= ~JBD2_ABORT_ON_SYNCDATA_ERR;
<<<<<<< HEAD
	write_unlock(&journal->j_state_lock);
}

static journal_t *ext4_get_journal(struct super_block *sb,
				   unsigned int journal_inum)
{
	struct inode *journal_inode;
	journal_t *journal;

	BUG_ON(!EXT4_HAS_COMPAT_FEATURE(sb, EXT4_FEATURE_COMPAT_HAS_JOURNAL));

	/* First, test for the existence of a valid inode on disk.  Bad
	 * things happen if we iget() an unused inode, as the subsequent
	 * iput() will try to delete it. */

	journal_inode = ext4_iget(sb, journal_inum);
	if (IS_ERR(journal_inode)) {
		ext4_msg(sb, KERN_ERR, "no journal found");
		return NULL;
=======
	/*
	 * Always enable journal cycle record option, letting the journal
	 * records log transactions continuously between each mount.
	 */
	journal->j_flags |= JBD2_CYCLE_RECORD;
	write_unlock(&journal->j_state_lock);
}

static struct inode *ext4_get_journal_inode(struct super_block *sb,
					     unsigned int journal_inum)
{
	struct inode *journal_inode;

	/*
	 * Test for the existence of a valid inode on disk.  Bad things
	 * happen if we iget() an unused inode, as the subsequent iput()
	 * will try to delete it.
	 */
	journal_inode = ext4_iget(sb, journal_inum, EXT4_IGET_SPECIAL);
	if (IS_ERR(journal_inode)) {
		ext4_msg(sb, KERN_ERR, "no journal found");
		return ERR_CAST(journal_inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (!journal_inode->i_nlink) {
		make_bad_inode(journal_inode);
		iput(journal_inode);
		ext4_msg(sb, KERN_ERR, "journal inode is deleted");
<<<<<<< HEAD
		return NULL;
	}

	jbd_debug(2, "Journal inode found at %p: %lld bytes\n",
		  journal_inode, journal_inode->i_size);
	if (!S_ISREG(journal_inode->i_mode)) {
		ext4_msg(sb, KERN_ERR, "invalid journal inode");
		iput(journal_inode);
		return NULL;
	}

	journal = jbd2_journal_init_inode(journal_inode);
	if (!journal) {
		ext4_msg(sb, KERN_ERR, "Could not load journal inode");
		iput(journal_inode);
		return NULL;
	}
	journal->j_private = sb;
=======
		return ERR_PTR(-EFSCORRUPTED);
	}
	if (!S_ISREG(journal_inode->i_mode) || IS_ENCRYPTED(journal_inode)) {
		ext4_msg(sb, KERN_ERR, "invalid journal inode");
		iput(journal_inode);
		return ERR_PTR(-EFSCORRUPTED);
	}

	ext4_debug("Journal inode found at %p: %lld bytes\n",
		  journal_inode, journal_inode->i_size);
	return journal_inode;
}

static int ext4_journal_bmap(journal_t *journal, sector_t *block)
{
	struct ext4_map_blocks map;
	int ret;

	if (journal->j_inode == NULL)
		return 0;

	map.m_lblk = *block;
	map.m_len = 1;
	ret = ext4_map_blocks(NULL, journal->j_inode, &map, 0);
	if (ret <= 0) {
		ext4_msg(journal->j_inode->i_sb, KERN_CRIT,
			 "journal bmap failed: block %llu ret %d\n",
			 *block, ret);
		jbd2_journal_abort(journal, ret ? ret : -EIO);
		return ret;
	}
	*block = map.m_pblk;
	return 0;
}

static journal_t *ext4_open_inode_journal(struct super_block *sb,
					  unsigned int journal_inum)
{
	struct inode *journal_inode;
	journal_t *journal;

	journal_inode = ext4_get_journal_inode(sb, journal_inum);
	if (IS_ERR(journal_inode))
		return ERR_CAST(journal_inode);

	journal = jbd2_journal_init_inode(journal_inode);
	if (IS_ERR(journal)) {
		ext4_msg(sb, KERN_ERR, "Could not load journal inode");
		iput(journal_inode);
		return ERR_CAST(journal);
	}
	journal->j_private = sb;
	journal->j_bmap = ext4_journal_bmap;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ext4_init_journal_params(sb, journal);
	return journal;
}

<<<<<<< HEAD
static journal_t *ext4_get_dev_journal(struct super_block *sb,
				       dev_t j_dev)
{
	struct buffer_head *bh;
	journal_t *journal;
	ext4_fsblk_t start;
	ext4_fsblk_t len;
=======
static struct file *ext4_get_journal_blkdev(struct super_block *sb,
					dev_t j_dev, ext4_fsblk_t *j_start,
					ext4_fsblk_t *j_len)
{
	struct buffer_head *bh;
	struct block_device *bdev;
	struct file *bdev_file;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int hblock, blocksize;
	ext4_fsblk_t sb_block;
	unsigned long offset;
	struct ext4_super_block *es;
<<<<<<< HEAD
	struct block_device *bdev;

	BUG_ON(!EXT4_HAS_COMPAT_FEATURE(sb, EXT4_FEATURE_COMPAT_HAS_JOURNAL));

	bdev = ext4_blkdev_get(j_dev, sb);
	if (bdev == NULL)
		return NULL;

=======
	int errno;

	bdev_file = bdev_file_open_by_dev(j_dev,
		BLK_OPEN_READ | BLK_OPEN_WRITE | BLK_OPEN_RESTRICT_WRITES,
		sb, &fs_holder_ops);
	if (IS_ERR(bdev_file)) {
		ext4_msg(sb, KERN_ERR,
			 "failed to open journal device unknown-block(%u,%u) %ld",
			 MAJOR(j_dev), MINOR(j_dev), PTR_ERR(bdev_file));
		return bdev_file;
	}

	bdev = file_bdev(bdev_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	blocksize = sb->s_blocksize;
	hblock = bdev_logical_block_size(bdev);
	if (blocksize < hblock) {
		ext4_msg(sb, KERN_ERR,
			"blocksize too small for journal device");
<<<<<<< HEAD
=======
		errno = -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_bdev;
	}

	sb_block = EXT4_MIN_BLOCK_SIZE / blocksize;
	offset = EXT4_MIN_BLOCK_SIZE % blocksize;
	set_blocksize(bdev, blocksize);
<<<<<<< HEAD
	if (!(bh = __bread(bdev, sb_block, blocksize))) {
		ext4_msg(sb, KERN_ERR, "couldn't read superblock of "
		       "external journal");
		goto out_bdev;
	}

	es = (struct ext4_super_block *) (((char *)bh->b_data) + offset);
	if ((le16_to_cpu(es->s_magic) != EXT4_SUPER_MAGIC) ||
	    !(le32_to_cpu(es->s_feature_incompat) &
	      EXT4_FEATURE_INCOMPAT_JOURNAL_DEV)) {
		ext4_msg(sb, KERN_ERR, "external journal has "
					"bad superblock");
		brelse(bh);
		goto out_bdev;
=======
	bh = __bread(bdev, sb_block, blocksize);
	if (!bh) {
		ext4_msg(sb, KERN_ERR, "couldn't read superblock of "
		       "external journal");
		errno = -EINVAL;
		goto out_bdev;
	}

	es = (struct ext4_super_block *) (bh->b_data + offset);
	if ((le16_to_cpu(es->s_magic) != EXT4_SUPER_MAGIC) ||
	    !(le32_to_cpu(es->s_feature_incompat) &
	      EXT4_FEATURE_INCOMPAT_JOURNAL_DEV)) {
		ext4_msg(sb, KERN_ERR, "external journal has bad superblock");
		errno = -EFSCORRUPTED;
		goto out_bh;
	}

	if ((le32_to_cpu(es->s_feature_ro_compat) &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) &&
	    es->s_checksum != ext4_superblock_csum(sb, es)) {
		ext4_msg(sb, KERN_ERR, "external journal has corrupt superblock");
		errno = -EFSCORRUPTED;
		goto out_bh;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (memcmp(EXT4_SB(sb)->s_es->s_journal_uuid, es->s_uuid, 16)) {
		ext4_msg(sb, KERN_ERR, "journal UUID does not match");
<<<<<<< HEAD
		brelse(bh);
		goto out_bdev;
	}

	len = ext4_blocks_count(es);
	start = sb_block + 1;
	brelse(bh);	/* we're done with the superblock */

	journal = jbd2_journal_init_dev(bdev, sb->s_bdev,
					start, len, blocksize);
	if (!journal) {
		ext4_msg(sb, KERN_ERR, "failed to create device journal");
		goto out_bdev;
	}
	journal->j_private = sb;
	ll_rw_block(READ, 1, &journal->j_sb_buffer);
	wait_on_buffer(journal->j_sb_buffer);
	if (!buffer_uptodate(journal->j_sb_buffer)) {
		ext4_msg(sb, KERN_ERR, "I/O error on journal device");
		goto out_journal;
	}
=======
		errno = -EFSCORRUPTED;
		goto out_bh;
	}

	*j_start = sb_block + 1;
	*j_len = ext4_blocks_count(es);
	brelse(bh);
	return bdev_file;

out_bh:
	brelse(bh);
out_bdev:
	bdev_fput(bdev_file);
	return ERR_PTR(errno);
}

static journal_t *ext4_open_dev_journal(struct super_block *sb,
					dev_t j_dev)
{
	journal_t *journal;
	ext4_fsblk_t j_start;
	ext4_fsblk_t j_len;
	struct file *bdev_file;
	int errno = 0;

	bdev_file = ext4_get_journal_blkdev(sb, j_dev, &j_start, &j_len);
	if (IS_ERR(bdev_file))
		return ERR_CAST(bdev_file);

	journal = jbd2_journal_init_dev(file_bdev(bdev_file), sb->s_bdev, j_start,
					j_len, sb->s_blocksize);
	if (IS_ERR(journal)) {
		ext4_msg(sb, KERN_ERR, "failed to create device journal");
		errno = PTR_ERR(journal);
		goto out_bdev;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (be32_to_cpu(journal->j_superblock->s_nr_users) != 1) {
		ext4_msg(sb, KERN_ERR, "External journal has more than one "
					"user (unsupported) - %d",
			be32_to_cpu(journal->j_superblock->s_nr_users));
<<<<<<< HEAD
		goto out_journal;
	}
	EXT4_SB(sb)->journal_bdev = bdev;
=======
		errno = -EINVAL;
		goto out_journal;
	}
	journal->j_private = sb;
	EXT4_SB(sb)->s_journal_bdev_file = bdev_file;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ext4_init_journal_params(sb, journal);
	return journal;

out_journal:
	jbd2_journal_destroy(journal);
out_bdev:
<<<<<<< HEAD
	ext4_blkdev_put(bdev);
	return NULL;
=======
	bdev_fput(bdev_file);
	return ERR_PTR(errno);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ext4_load_journal(struct super_block *sb,
			     struct ext4_super_block *es,
			     unsigned long journal_devnum)
{
	journal_t *journal;
	unsigned int journal_inum = le32_to_cpu(es->s_journal_inum);
	dev_t journal_dev;
	int err = 0;
	int really_read_only;
<<<<<<< HEAD

	BUG_ON(!EXT4_HAS_COMPAT_FEATURE(sb, EXT4_FEATURE_COMPAT_HAS_JOURNAL));
=======
	int journal_dev_ro;

	if (WARN_ON_ONCE(!ext4_has_feature_journal(sb)))
		return -EFSCORRUPTED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (journal_devnum &&
	    journal_devnum != le32_to_cpu(es->s_journal_dev)) {
		ext4_msg(sb, KERN_INFO, "external journal device major/minor "
			"numbers have changed");
		journal_dev = new_decode_dev(journal_devnum);
	} else
		journal_dev = new_decode_dev(le32_to_cpu(es->s_journal_dev));

<<<<<<< HEAD
	really_read_only = bdev_read_only(sb->s_bdev);
=======
	if (journal_inum && journal_dev) {
		ext4_msg(sb, KERN_ERR,
			 "filesystem has both journal inode and journal device!");
		return -EINVAL;
	}

	if (journal_inum) {
		journal = ext4_open_inode_journal(sb, journal_inum);
		if (IS_ERR(journal))
			return PTR_ERR(journal);
	} else {
		journal = ext4_open_dev_journal(sb, journal_dev);
		if (IS_ERR(journal))
			return PTR_ERR(journal);
	}

	journal_dev_ro = bdev_read_only(journal->j_dev);
	really_read_only = bdev_read_only(sb->s_bdev) | journal_dev_ro;

	if (journal_dev_ro && !sb_rdonly(sb)) {
		ext4_msg(sb, KERN_ERR,
			 "journal device read-only, try mounting with '-o ro'");
		err = -EROFS;
		goto err_out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Are we loading a blank journal or performing recovery after a
	 * crash?  For recovery, we need to check in advance whether we
	 * can get read-write access to the device.
	 */
<<<<<<< HEAD
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER)) {
		if (sb->s_flags & MS_RDONLY) {
=======
	if (ext4_has_feature_journal_needs_recovery(sb)) {
		if (sb_rdonly(sb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ext4_msg(sb, KERN_INFO, "INFO: recovery "
					"required on readonly filesystem");
			if (really_read_only) {
				ext4_msg(sb, KERN_ERR, "write access "
<<<<<<< HEAD
					"unavailable, cannot proceed");
				return -EROFS;
=======
					"unavailable, cannot proceed "
					"(try mounting with noload)");
				err = -EROFS;
				goto err_out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			ext4_msg(sb, KERN_INFO, "write access will "
			       "be enabled during recovery");
		}
	}

<<<<<<< HEAD
	if (journal_inum && journal_dev) {
		ext4_msg(sb, KERN_ERR, "filesystem has both journal "
		       "and inode journals!");
		return -EINVAL;
	}

	if (journal_inum) {
		if (!(journal = ext4_get_journal(sb, journal_inum)))
			return -EINVAL;
	} else {
		if (!(journal = ext4_get_dev_journal(sb, journal_dev)))
			return -EINVAL;
	}

	if (!(journal->j_flags & JBD2_BARRIER))
		ext4_msg(sb, KERN_INFO, "barriers disabled");

	if (!EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER))
		err = jbd2_journal_wipe(journal, !really_read_only);
	if (!err) {
		char *save = kmalloc(EXT4_S_ERR_LEN, GFP_KERNEL);
=======
	if (!(journal->j_flags & JBD2_BARRIER))
		ext4_msg(sb, KERN_INFO, "barriers disabled");

	if (!ext4_has_feature_journal_needs_recovery(sb))
		err = jbd2_journal_wipe(journal, !really_read_only);
	if (!err) {
		char *save = kmalloc(EXT4_S_ERR_LEN, GFP_KERNEL);
		__le16 orig_state;
		bool changed = false;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (save)
			memcpy(save, ((char *) es) +
			       EXT4_S_ERR_START, EXT4_S_ERR_LEN);
		err = jbd2_journal_load(journal);
<<<<<<< HEAD
		if (save)
			memcpy(((char *) es) + EXT4_S_ERR_START,
			       save, EXT4_S_ERR_LEN);
		kfree(save);
=======
		if (save && memcmp(((char *) es) + EXT4_S_ERR_START,
				   save, EXT4_S_ERR_LEN)) {
			memcpy(((char *) es) + EXT4_S_ERR_START,
			       save, EXT4_S_ERR_LEN);
			changed = true;
		}
		kfree(save);
		orig_state = es->s_state;
		es->s_state |= cpu_to_le16(EXT4_SB(sb)->s_mount_state &
					   EXT4_ERROR_FS);
		if (orig_state != es->s_state)
			changed = true;
		/* Write out restored error information to the superblock */
		if (changed && !really_read_only) {
			int err2;
			err2 = ext4_commit_super(sb);
			err = err ? : err2;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (err) {
		ext4_msg(sb, KERN_ERR, "error loading journal");
<<<<<<< HEAD
=======
		goto err_out;
	}

	EXT4_SB(sb)->s_journal = journal;
	err = ext4_clear_journal_err(sb, es);
	if (err) {
		EXT4_SB(sb)->s_journal = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		jbd2_journal_destroy(journal);
		return err;
	}

<<<<<<< HEAD
	EXT4_SB(sb)->s_journal = journal;
	ext4_clear_journal_err(sb, es);

	if (!really_read_only && journal_devnum &&
	    journal_devnum != le32_to_cpu(es->s_journal_dev)) {
		es->s_journal_dev = cpu_to_le32(journal_devnum);

		/* Make sure we flush the recovery flag to disk. */
		ext4_commit_super(sb, 1);
	}

	return 0;
}

static int ext4_commit_super(struct super_block *sb, int sync)
{
	struct ext4_super_block *es = EXT4_SB(sb)->s_es;
	struct buffer_head *sbh = EXT4_SB(sb)->s_sbh;
	int error = 0;

	if (!sbh || block_device_ejected(sb))
		return error;
	if (buffer_write_io_error(sbh)) {
=======
	if (!really_read_only && journal_devnum &&
	    journal_devnum != le32_to_cpu(es->s_journal_dev)) {
		es->s_journal_dev = cpu_to_le32(journal_devnum);
		ext4_commit_super(sb);
	}
	if (!really_read_only && journal_inum &&
	    journal_inum != le32_to_cpu(es->s_journal_inum)) {
		es->s_journal_inum = cpu_to_le32(journal_inum);
		ext4_commit_super(sb);
	}

	return 0;

err_out:
	jbd2_journal_destroy(journal);
	return err;
}

/* Copy state of EXT4_SB(sb) into buffer for on-disk superblock */
static void ext4_update_super(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
	struct buffer_head *sbh = sbi->s_sbh;

	lock_buffer(sbh);
	/*
	 * If the file system is mounted read-only, don't update the
	 * superblock write time.  This avoids updating the superblock
	 * write time when we are mounting the root file system
	 * read/only but we need to replay the journal; at that point,
	 * for people who are east of GMT and who make their clock
	 * tick in localtime for Windows bug-for-bug compatibility,
	 * the clock is set in the future, and this will cause e2fsck
	 * to complain and force a full file system check.
	 */
	if (!sb_rdonly(sb))
		ext4_update_tstamp(es, s_wtime);
	es->s_kbytes_written =
		cpu_to_le64(sbi->s_kbytes_written +
		    ((part_stat_read(sb->s_bdev, sectors[STAT_WRITE]) -
		      sbi->s_sectors_written_start) >> 1));
	if (percpu_counter_initialized(&sbi->s_freeclusters_counter))
		ext4_free_blocks_count_set(es,
			EXT4_C2B(sbi, percpu_counter_sum_positive(
				&sbi->s_freeclusters_counter)));
	if (percpu_counter_initialized(&sbi->s_freeinodes_counter))
		es->s_free_inodes_count =
			cpu_to_le32(percpu_counter_sum_positive(
				&sbi->s_freeinodes_counter));
	/* Copy error information to the on-disk superblock */
	spin_lock(&sbi->s_error_lock);
	if (sbi->s_add_error_count > 0) {
		es->s_state |= cpu_to_le16(EXT4_ERROR_FS);
		if (!es->s_first_error_time && !es->s_first_error_time_hi) {
			__ext4_update_tstamp(&es->s_first_error_time,
					     &es->s_first_error_time_hi,
					     sbi->s_first_error_time);
			strncpy(es->s_first_error_func, sbi->s_first_error_func,
				sizeof(es->s_first_error_func));
			es->s_first_error_line =
				cpu_to_le32(sbi->s_first_error_line);
			es->s_first_error_ino =
				cpu_to_le32(sbi->s_first_error_ino);
			es->s_first_error_block =
				cpu_to_le64(sbi->s_first_error_block);
			es->s_first_error_errcode =
				ext4_errno_to_code(sbi->s_first_error_code);
		}
		__ext4_update_tstamp(&es->s_last_error_time,
				     &es->s_last_error_time_hi,
				     sbi->s_last_error_time);
		strncpy(es->s_last_error_func, sbi->s_last_error_func,
			sizeof(es->s_last_error_func));
		es->s_last_error_line = cpu_to_le32(sbi->s_last_error_line);
		es->s_last_error_ino = cpu_to_le32(sbi->s_last_error_ino);
		es->s_last_error_block = cpu_to_le64(sbi->s_last_error_block);
		es->s_last_error_errcode =
				ext4_errno_to_code(sbi->s_last_error_code);
		/*
		 * Start the daily error reporting function if it hasn't been
		 * started already
		 */
		if (!es->s_error_count)
			mod_timer(&sbi->s_err_report, jiffies + 24*60*60*HZ);
		le32_add_cpu(&es->s_error_count, sbi->s_add_error_count);
		sbi->s_add_error_count = 0;
	}
	spin_unlock(&sbi->s_error_lock);

	ext4_superblock_csum_set(sb);
	unlock_buffer(sbh);
}

static int ext4_commit_super(struct super_block *sb)
{
	struct buffer_head *sbh = EXT4_SB(sb)->s_sbh;

	if (!sbh)
		return -EINVAL;
	if (block_device_ejected(sb))
		return -ENODEV;

	ext4_update_super(sb);

	lock_buffer(sbh);
	/* Buffer got discarded which means block device got invalidated */
	if (!buffer_mapped(sbh)) {
		unlock_buffer(sbh);
		return -EIO;
	}

	if (buffer_write_io_error(sbh) || !buffer_uptodate(sbh)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Oh, dear.  A previous attempt to write the
		 * superblock failed.  This could happen because the
		 * USB device was yanked out.  Or it could happen to
		 * be a transient write error and maybe the block will
		 * be remapped.  Nothing we can do but to retry the
		 * write and hope for the best.
		 */
		ext4_msg(sb, KERN_ERR, "previous I/O error to "
		       "superblock detected");
		clear_buffer_write_io_error(sbh);
		set_buffer_uptodate(sbh);
	}
<<<<<<< HEAD
	/*
	 * If the file system is mounted read-only, don't update the
	 * superblock write time.  This avoids updating the superblock
	 * write time when we are mounting the root file system
	 * read/only but we need to replay the journal; at that point,
	 * for people who are east of GMT and who make their clock
	 * tick in localtime for Windows bug-for-bug compatibility,
	 * the clock is set in the future, and this will cause e2fsck
	 * to complain and force a full file system check.
	 */
	if (!(sb->s_flags & MS_RDONLY))
		es->s_wtime = cpu_to_le32(get_seconds());
	if (sb->s_bdev->bd_part)
		es->s_kbytes_written =
			cpu_to_le64(EXT4_SB(sb)->s_kbytes_written +
			    ((part_stat_read(sb->s_bdev->bd_part, sectors[1]) -
			      EXT4_SB(sb)->s_sectors_written_start) >> 1));
	else
		es->s_kbytes_written =
			cpu_to_le64(EXT4_SB(sb)->s_kbytes_written);
	ext4_free_blocks_count_set(es,
			EXT4_C2B(EXT4_SB(sb), percpu_counter_sum_positive(
				&EXT4_SB(sb)->s_freeclusters_counter)));
	es->s_free_inodes_count =
		cpu_to_le32(percpu_counter_sum_positive(
				&EXT4_SB(sb)->s_freeinodes_counter));
	sb->s_dirt = 0;
	BUFFER_TRACE(sbh, "marking dirty");
	ext4_superblock_csum_set(sb, es);
	mark_buffer_dirty(sbh);
	if (sync) {
		error = sync_dirty_buffer(sbh);
		if (error)
			return error;

		error = buffer_write_io_error(sbh);
		if (error) {
			ext4_msg(sb, KERN_ERR, "I/O error while writing "
			       "superblock");
			clear_buffer_write_io_error(sbh);
			set_buffer_uptodate(sbh);
		}
	}
	return error;
=======
	get_bh(sbh);
	/* Clear potential dirty bit if it was journalled update */
	clear_buffer_dirty(sbh);
	sbh->b_end_io = end_buffer_write_sync;
	submit_bh(REQ_OP_WRITE | REQ_SYNC |
		  (test_opt(sb, BARRIER) ? REQ_FUA : 0), sbh);
	wait_on_buffer(sbh);
	if (buffer_write_io_error(sbh)) {
		ext4_msg(sb, KERN_ERR, "I/O error while writing "
		       "superblock");
		clear_buffer_write_io_error(sbh);
		set_buffer_uptodate(sbh);
		return -EIO;
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Have we just finished recovery?  If so, and if we are mounting (or
 * remounting) the filesystem readonly, then we will end up with a
 * consistent fs on disk.  Record that fact.
 */
<<<<<<< HEAD
static void ext4_mark_recovery_complete(struct super_block *sb,
					struct ext4_super_block *es)
{
	journal_t *journal = EXT4_SB(sb)->s_journal;

	if (!EXT4_HAS_COMPAT_FEATURE(sb, EXT4_FEATURE_COMPAT_HAS_JOURNAL)) {
		BUG_ON(journal != NULL);
		return;
	}
	jbd2_journal_lock_updates(journal);
	if (jbd2_journal_flush(journal) < 0)
		goto out;

	if (EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER) &&
	    sb->s_flags & MS_RDONLY) {
		EXT4_CLEAR_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER);
		ext4_commit_super(sb, 1);
	}

out:
	jbd2_journal_unlock_updates(journal);
=======
static int ext4_mark_recovery_complete(struct super_block *sb,
				       struct ext4_super_block *es)
{
	int err;
	journal_t *journal = EXT4_SB(sb)->s_journal;

	if (!ext4_has_feature_journal(sb)) {
		if (journal != NULL) {
			ext4_error(sb, "Journal got removed while the fs was "
				   "mounted!");
			return -EFSCORRUPTED;
		}
		return 0;
	}
	jbd2_journal_lock_updates(journal);
	err = jbd2_journal_flush(journal, 0);
	if (err < 0)
		goto out;

	if (sb_rdonly(sb) && (ext4_has_feature_journal_needs_recovery(sb) ||
	    ext4_has_feature_orphan_present(sb))) {
		if (!ext4_orphan_file_empty(sb)) {
			ext4_error(sb, "Orphan file not empty on read-only fs.");
			err = -EFSCORRUPTED;
			goto out;
		}
		ext4_clear_feature_journal_needs_recovery(sb);
		ext4_clear_feature_orphan_present(sb);
		ext4_commit_super(sb);
	}
out:
	jbd2_journal_unlock_updates(journal);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * If we are mounting (or read-write remounting) a filesystem whose journal
 * has recorded an error from a previous lifetime, move that error to the
 * main filesystem now.
 */
<<<<<<< HEAD
static void ext4_clear_journal_err(struct super_block *sb,
=======
static int ext4_clear_journal_err(struct super_block *sb,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   struct ext4_super_block *es)
{
	journal_t *journal;
	int j_errno;
	const char *errstr;

<<<<<<< HEAD
	BUG_ON(!EXT4_HAS_COMPAT_FEATURE(sb, EXT4_FEATURE_COMPAT_HAS_JOURNAL));
=======
	if (!ext4_has_feature_journal(sb)) {
		ext4_error(sb, "Journal got removed while the fs was mounted!");
		return -EFSCORRUPTED;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	journal = EXT4_SB(sb)->s_journal;

	/*
	 * Now check for any error status which may have been recorded in the
	 * journal by a prior ext4_error() or ext4_abort()
	 */

	j_errno = jbd2_journal_errno(journal);
	if (j_errno) {
		char nbuf[16];

		errstr = ext4_decode_error(sb, j_errno, nbuf);
		ext4_warning(sb, "Filesystem error recorded "
			     "from previous mount: %s", errstr);
<<<<<<< HEAD
		ext4_warning(sb, "Marking fs in need of filesystem check.");

		EXT4_SB(sb)->s_mount_state |= EXT4_ERROR_FS;
		es->s_state |= cpu_to_le16(EXT4_ERROR_FS);
		ext4_commit_super(sb, 1);
=======

		EXT4_SB(sb)->s_mount_state |= EXT4_ERROR_FS;
		es->s_state |= cpu_to_le16(EXT4_ERROR_FS);
		j_errno = ext4_commit_super(sb);
		if (j_errno)
			return j_errno;
		ext4_warning(sb, "Marked fs in need of filesystem check.");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		jbd2_journal_clear_err(journal);
		jbd2_journal_update_sb_errno(journal);
	}
<<<<<<< HEAD
=======
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Force the running and committing transactions to commit,
 * and wait on the commit.
 */
int ext4_force_commit(struct super_block *sb)
{
<<<<<<< HEAD
	journal_t *journal;
	int ret = 0;

	if (sb->s_flags & MS_RDONLY)
		return 0;

	journal = EXT4_SB(sb)->s_journal;
	if (journal) {
		vfs_check_frozen(sb, SB_FREEZE_TRANS);
		ret = ext4_journal_force_commit(journal);
	}

	return ret;
}

static void ext4_write_super(struct super_block *sb)
{
	lock_super(sb);
	ext4_commit_super(sb, 1);
	unlock_super(sb);
=======
	return ext4_journal_force_commit(EXT4_SB(sb)->s_journal);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ext4_sync_fs(struct super_block *sb, int wait)
{
	int ret = 0;
	tid_t target;
<<<<<<< HEAD
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	trace_ext4_sync_fs(sb, wait);
	flush_workqueue(sbi->dio_unwritten_wq);
	if (jbd2_journal_start_commit(sbi->s_journal, &target)) {
		if (wait)
			jbd2_log_wait_commit(sbi->s_journal, target);
	}
=======
	bool needs_barrier = false;
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	if (unlikely(ext4_forced_shutdown(sb)))
		return 0;

	trace_ext4_sync_fs(sb, wait);
	flush_workqueue(sbi->rsv_conversion_wq);
	/*
	 * Writeback quota in non-journalled quota case - journalled quota has
	 * no dirty dquots
	 */
	dquot_writeback_dquots(sb, -1);
	/*
	 * Data writeback is possible w/o journal transaction, so barrier must
	 * being sent at the end of the function. But we can skip it if
	 * transaction_commit will do it for us.
	 */
	if (sbi->s_journal) {
		target = jbd2_get_latest_transaction(sbi->s_journal);
		if (wait && sbi->s_journal->j_flags & JBD2_BARRIER &&
		    !jbd2_trans_will_send_data_barrier(sbi->s_journal, target))
			needs_barrier = true;

		if (jbd2_journal_start_commit(sbi->s_journal, &target)) {
			if (wait)
				ret = jbd2_log_wait_commit(sbi->s_journal,
							   target);
		}
	} else if (wait && test_opt(sb, BARRIER))
		needs_barrier = true;
	if (needs_barrier) {
		int err;
		err = blkdev_issue_flush(sb->s_bdev);
		if (!ret)
			ret = err;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * LVM calls this function before a (read-only) snapshot is created.  This
 * gives us a chance to flush the journal completely and mark the fs clean.
 *
 * Note that only this function cannot bring a filesystem to be in a clean
<<<<<<< HEAD
 * state independently, because ext4 prevents a new handle from being started
 * by @sb->s_frozen, which stays in an upper layer.  It thus needs help from
 * the upper layer.
=======
 * state independently. It relies on upper layer to stop all data & metadata
 * modifications.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int ext4_freeze(struct super_block *sb)
{
	int error = 0;
<<<<<<< HEAD
	journal_t *journal;

	if (sb->s_flags & MS_RDONLY)
		return 0;

	journal = EXT4_SB(sb)->s_journal;

	/* Now we set up the journal barrier. */
	jbd2_journal_lock_updates(journal);

	/*
	 * Don't clear the needs_recovery flag if we failed to flush
	 * the journal.
	 */
	error = jbd2_journal_flush(journal);
	if (error < 0)
		goto out;

	/* Journal blocked and flushed, clear needs_recovery flag. */
	EXT4_CLEAR_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER);
	error = ext4_commit_super(sb, 1);
out:
	/* we rely on s_frozen to stop further updates */
	jbd2_journal_unlock_updates(EXT4_SB(sb)->s_journal);
=======
	journal_t *journal = EXT4_SB(sb)->s_journal;

	if (journal) {
		/* Now we set up the journal barrier. */
		jbd2_journal_lock_updates(journal);

		/*
		 * Don't clear the needs_recovery flag if we failed to
		 * flush the journal.
		 */
		error = jbd2_journal_flush(journal, 0);
		if (error < 0)
			goto out;

		/* Journal blocked and flushed, clear needs_recovery flag. */
		ext4_clear_feature_journal_needs_recovery(sb);
		if (ext4_orphan_file_empty(sb))
			ext4_clear_feature_orphan_present(sb);
	}

	error = ext4_commit_super(sb);
out:
	if (journal)
		/* we rely on upper layer to stop further updates */
		jbd2_journal_unlock_updates(journal);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/*
 * Called by LVM after the snapshot is done.  We need to reset the RECOVER
 * flag here, even though the filesystem is not technically dirty yet.
 */
static int ext4_unfreeze(struct super_block *sb)
{
<<<<<<< HEAD
	if (sb->s_flags & MS_RDONLY)
		return 0;

	lock_super(sb);
	/* Reset the needs_recovery flag before the fs is unlocked. */
	EXT4_SET_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER);
	ext4_commit_super(sb, 1);
	unlock_super(sb);
=======
	if (ext4_forced_shutdown(sb))
		return 0;

	if (EXT4_SB(sb)->s_journal) {
		/* Reset the needs_recovery flag before the fs is unlocked. */
		ext4_set_feature_journal_needs_recovery(sb);
		if (ext4_has_feature_orphan_file(sb))
			ext4_set_feature_orphan_present(sb);
	}

	ext4_commit_super(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Structure to save mount options for ext4_remount's benefit
 */
struct ext4_mount_options {
	unsigned long s_mount_opt;
	unsigned long s_mount_opt2;
<<<<<<< HEAD
	uid_t s_resuid;
	gid_t s_resgid;
=======
	kuid_t s_resuid;
	kgid_t s_resgid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long s_commit_interval;
	u32 s_min_batch_time, s_max_batch_time;
#ifdef CONFIG_QUOTA
	int s_jquota_fmt;
<<<<<<< HEAD
	char *s_qf_names[MAXQUOTAS];
#endif
};

static int ext4_remount(struct super_block *sb, int *flags, char *data)
{
=======
	char *s_qf_names[EXT4_MAXQUOTAS];
#endif
};

static int __ext4_remount(struct fs_context *fc, struct super_block *sb)
{
	struct ext4_fs_context *ctx = fc->fs_private;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ext4_super_block *es;
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	unsigned long old_sb_flags;
	struct ext4_mount_options old_opts;
<<<<<<< HEAD
	int enable_quota = 0;
	ext4_group_t g;
	unsigned int journal_ioprio = DEFAULT_JOURNAL_IOPRIO;
	int err = 0;
#ifdef CONFIG_QUOTA
	int i;
#endif
	char *orig_data = kstrdup(data, GFP_KERNEL);

	/* Store the original options */
	lock_super(sb);
=======
	ext4_group_t g;
	int err = 0;
	int alloc_ctx;
#ifdef CONFIG_QUOTA
	int enable_quota = 0;
	int i, j;
	char *to_free[EXT4_MAXQUOTAS];
#endif


	/* Store the original options */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	old_sb_flags = sb->s_flags;
	old_opts.s_mount_opt = sbi->s_mount_opt;
	old_opts.s_mount_opt2 = sbi->s_mount_opt2;
	old_opts.s_resuid = sbi->s_resuid;
	old_opts.s_resgid = sbi->s_resgid;
	old_opts.s_commit_interval = sbi->s_commit_interval;
	old_opts.s_min_batch_time = sbi->s_min_batch_time;
	old_opts.s_max_batch_time = sbi->s_max_batch_time;
#ifdef CONFIG_QUOTA
	old_opts.s_jquota_fmt = sbi->s_jquota_fmt;
<<<<<<< HEAD
	for (i = 0; i < MAXQUOTAS; i++)
		old_opts.s_qf_names[i] = sbi->s_qf_names[i];
#endif
	if (sbi->s_journal && sbi->s_journal->j_task->io_context)
		journal_ioprio = sbi->s_journal->j_task->io_context->ioprio;

	/*
	 * Allow the "check" option to be passed as a remount option.
	 */
	if (!parse_options(data, sb, NULL, &journal_ioprio, 1)) {
		err = -EINVAL;
		goto restore_opts;
=======
	for (i = 0; i < EXT4_MAXQUOTAS; i++)
		if (sbi->s_qf_names[i]) {
			char *qf_name = get_qf_name(sb, sbi, i);

			old_opts.s_qf_names[i] = kstrdup(qf_name, GFP_KERNEL);
			if (!old_opts.s_qf_names[i]) {
				for (j = 0; j < i; j++)
					kfree(old_opts.s_qf_names[j]);
				return -ENOMEM;
			}
		} else
			old_opts.s_qf_names[i] = NULL;
#endif
	if (!(ctx->spec & EXT4_SPEC_JOURNAL_IOPRIO)) {
		if (sbi->s_journal && sbi->s_journal->j_task->io_context)
			ctx->journal_ioprio =
				sbi->s_journal->j_task->io_context->ioprio;
		else
			ctx->journal_ioprio = DEFAULT_JOURNAL_IOPRIO;

	}

	/*
	 * Changing the DIOREAD_NOLOCK or DELALLOC mount options may cause
	 * two calls to ext4_should_dioread_nolock() to return inconsistent
	 * values, triggering WARN_ON in ext4_add_complete_io(). we grab
	 * here s_writepages_rwsem to avoid race between writepages ops and
	 * remount.
	 */
	alloc_ctx = ext4_writepages_down_write(sb);
	ext4_apply_options(fc, sb);
	ext4_writepages_up_write(sb, alloc_ctx);

	if ((old_opts.s_mount_opt & EXT4_MOUNT_JOURNAL_CHECKSUM) ^
	    test_opt(sb, JOURNAL_CHECKSUM)) {
		ext4_msg(sb, KERN_ERR, "changing journal_checksum "
			 "during remount not supported; ignoring");
		sbi->s_mount_opt ^= EXT4_MOUNT_JOURNAL_CHECKSUM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_JOURNAL_DATA) {
		if (test_opt2(sb, EXPLICIT_DELALLOC)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "both data=journal and delalloc");
			err = -EINVAL;
			goto restore_opts;
		}
		if (test_opt(sb, DIOREAD_NOLOCK)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "both data=journal and dioread_nolock");
			err = -EINVAL;
			goto restore_opts;
		}
<<<<<<< HEAD
	}

	if (sbi->s_mount_flags & EXT4_MF_FS_ABORTED)
		ext4_abort(sb, "Abort forced by user");

	sb->s_flags = (sb->s_flags & ~MS_POSIXACL) |
		(test_opt(sb, POSIX_ACL) ? MS_POSIXACL : 0);
=======
	} else if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_ORDERED_DATA) {
		if (test_opt(sb, JOURNAL_ASYNC_COMMIT)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				"journal_async_commit in data=ordered mode");
			err = -EINVAL;
			goto restore_opts;
		}
	}

	if ((sbi->s_mount_opt ^ old_opts.s_mount_opt) & EXT4_MOUNT_NO_MBCACHE) {
		ext4_msg(sb, KERN_ERR, "can't enable nombcache during remount");
		err = -EINVAL;
		goto restore_opts;
	}

	if (test_opt2(sb, ABORT))
		ext4_abort(sb, ESHUTDOWN, "Abort forced by user");

	sb->s_flags = (sb->s_flags & ~SB_POSIXACL) |
		(test_opt(sb, POSIX_ACL) ? SB_POSIXACL : 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	es = sbi->s_es;

	if (sbi->s_journal) {
		ext4_init_journal_params(sb, sbi->s_journal);
<<<<<<< HEAD
		set_task_ioprio(sbi->s_journal->j_task, journal_ioprio);
	}

	if ((*flags & MS_RDONLY) != (sb->s_flags & MS_RDONLY)) {
		if (sbi->s_mount_flags & EXT4_MF_FS_ABORTED) {
=======
		set_task_ioprio(sbi->s_journal->j_task, ctx->journal_ioprio);
	}

	/* Flush outstanding errors before changing fs state */
	flush_work(&sbi->s_sb_upd_work);

	if ((bool)(fc->sb_flags & SB_RDONLY) != sb_rdonly(sb)) {
		if (ext4_forced_shutdown(sb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EROFS;
			goto restore_opts;
		}

<<<<<<< HEAD
		if (*flags & MS_RDONLY) {
=======
		if (fc->sb_flags & SB_RDONLY) {
			err = sync_filesystem(sb);
			if (err < 0)
				goto restore_opts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = dquot_suspend(sb, -1);
			if (err < 0)
				goto restore_opts;

			/*
			 * First of all, the unconditional stuff we have to do
			 * to disable replay of the journal when we next remount
			 */
<<<<<<< HEAD
			sb->s_flags |= MS_RDONLY;
=======
			sb->s_flags |= SB_RDONLY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/*
			 * OK, test if we are remounting a valid rw partition
			 * readonly, and if so set the rdonly flag and then
			 * mark the partition as valid again.
			 */
			if (!(es->s_state & cpu_to_le16(EXT4_VALID_FS)) &&
			    (sbi->s_mount_state & EXT4_VALID_FS))
				es->s_state = cpu_to_le16(sbi->s_mount_state);

<<<<<<< HEAD
			if (sbi->s_journal)
				ext4_mark_recovery_complete(sb, es);
		} else {
			/* Make sure we can mount this feature set readwrite */
			if (!ext4_feature_set_ok(sb, 0)) {
=======
			if (sbi->s_journal) {
				/*
				 * We let remount-ro finish even if marking fs
				 * as clean failed...
				 */
				ext4_mark_recovery_complete(sb, es);
			}
		} else {
			/* Make sure we can mount this feature set readwrite */
			if (ext4_has_feature_readonly(sb) ||
			    !ext4_feature_set_ok(sb, 0)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				err = -EROFS;
				goto restore_opts;
			}
			/*
			 * Make sure the group descriptor checksums
			 * are sane.  If they aren't, refuse to remount r/w.
			 */
			for (g = 0; g < sbi->s_groups_count; g++) {
				struct ext4_group_desc *gdp =
					ext4_get_group_desc(sb, g, NULL);

				if (!ext4_group_desc_csum_verify(sb, g, gdp)) {
					ext4_msg(sb, KERN_ERR,
	       "ext4_remount: Checksum for group %u failed (%u!=%u)",
<<<<<<< HEAD
		g, le16_to_cpu(ext4_group_desc_csum(sbi, g, gdp)),
					       le16_to_cpu(gdp->bg_checksum));
					err = -EINVAL;
=======
		g, le16_to_cpu(ext4_group_desc_csum(sb, g, gdp)),
					       le16_to_cpu(gdp->bg_checksum));
					err = -EFSBADCRC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					goto restore_opts;
				}
			}

			/*
			 * If we have an unprocessed orphan list hanging
			 * around from a previously readonly bdev mount,
			 * require a full umount/remount for now.
			 */
<<<<<<< HEAD
			if (es->s_last_orphan) {
=======
			if (es->s_last_orphan || !ext4_orphan_file_empty(sb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				ext4_msg(sb, KERN_WARNING, "Couldn't "
				       "remount RDWR because of unprocessed "
				       "orphan inode list.  Please "
				       "umount/remount instead");
				err = -EINVAL;
				goto restore_opts;
			}

			/*
			 * Mounting a RDONLY partition read-write, so reread
			 * and store the current valid flag.  (It may have
			 * been changed by e2fsck since we originally mounted
			 * the partition.)
			 */
<<<<<<< HEAD
			if (sbi->s_journal)
				ext4_clear_journal_err(sb, es);
			sbi->s_mount_state = le16_to_cpu(es->s_state);
			if (!ext4_setup_super(sb, es, 0))
				sb->s_flags &= ~MS_RDONLY;
			if (EXT4_HAS_INCOMPAT_FEATURE(sb,
						     EXT4_FEATURE_INCOMPAT_MMP))
				if (ext4_multi_mount_protect(sb,
						le64_to_cpu(es->s_mmp_block))) {
					err = -EROFS;
					goto restore_opts;
				}
			enable_quota = 1;
=======
			if (sbi->s_journal) {
				err = ext4_clear_journal_err(sb, es);
				if (err)
					goto restore_opts;
			}
			sbi->s_mount_state = (le16_to_cpu(es->s_state) &
					      ~EXT4_FC_REPLAY);

			err = ext4_setup_super(sb, es, 0);
			if (err)
				goto restore_opts;

			sb->s_flags &= ~SB_RDONLY;
			if (ext4_has_feature_mmp(sb)) {
				err = ext4_multi_mount_protect(sb,
						le64_to_cpu(es->s_mmp_block));
				if (err)
					goto restore_opts;
			}
#ifdef CONFIG_QUOTA
			enable_quota = 1;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/*
<<<<<<< HEAD
	 * Reinitialize lazy itable initialization thread based on
	 * current settings
	 */
	if ((sb->s_flags & MS_RDONLY) || !test_opt(sb, INIT_INODE_TABLE))
=======
	 * Handle creation of system zone data early because it can fail.
	 * Releasing of existing data is done when we are sure remount will
	 * succeed.
	 */
	if (test_opt(sb, BLOCK_VALIDITY) && !sbi->s_system_blks) {
		err = ext4_setup_system_zone(sb);
		if (err)
			goto restore_opts;
	}

	if (sbi->s_journal == NULL && !(old_sb_flags & SB_RDONLY)) {
		err = ext4_commit_super(sb);
		if (err)
			goto restore_opts;
	}

#ifdef CONFIG_QUOTA
	if (enable_quota) {
		if (sb_any_quota_suspended(sb))
			dquot_resume(sb, -1);
		else if (ext4_has_feature_quota(sb)) {
			err = ext4_enable_quotas(sb);
			if (err)
				goto restore_opts;
		}
	}
	/* Release old quota file names */
	for (i = 0; i < EXT4_MAXQUOTAS; i++)
		kfree(old_opts.s_qf_names[i]);
#endif
	if (!test_opt(sb, BLOCK_VALIDITY) && sbi->s_system_blks)
		ext4_release_system_zone(sb);

	/*
	 * Reinitialize lazy itable initialization thread based on
	 * current settings
	 */
	if (sb_rdonly(sb) || !test_opt(sb, INIT_INODE_TABLE))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ext4_unregister_li_request(sb);
	else {
		ext4_group_t first_not_zeroed;
		first_not_zeroed = ext4_has_uninit_itable(sb);
		ext4_register_li_request(sb, first_not_zeroed);
	}

<<<<<<< HEAD
	ext4_setup_system_zone(sb);
	if (sbi->s_journal == NULL && !(old_sb_flags & MS_RDONLY))
		ext4_commit_super(sb, 1);

#ifdef CONFIG_QUOTA
	/* Release old quota file names */
	for (i = 0; i < MAXQUOTAS; i++)
		if (old_opts.s_qf_names[i] &&
		    old_opts.s_qf_names[i] != sbi->s_qf_names[i])
			kfree(old_opts.s_qf_names[i]);
#endif
	unlock_super(sb);
	if (enable_quota)
		dquot_resume(sb, -1);

	ext4_msg(sb, KERN_INFO, "re-mounted. Opts: %s", orig_data);
	kfree(orig_data);
	return 0;

restore_opts:
=======
	if (!ext4_has_feature_mmp(sb) || sb_rdonly(sb))
		ext4_stop_mmpd(sbi);

	return 0;

restore_opts:
	/*
	 * If there was a failing r/w to ro transition, we may need to
	 * re-enable quota
	 */
	if (sb_rdonly(sb) && !(old_sb_flags & SB_RDONLY) &&
	    sb_any_quota_suspended(sb))
		dquot_resume(sb, -1);

	alloc_ctx = ext4_writepages_down_write(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sb->s_flags = old_sb_flags;
	sbi->s_mount_opt = old_opts.s_mount_opt;
	sbi->s_mount_opt2 = old_opts.s_mount_opt2;
	sbi->s_resuid = old_opts.s_resuid;
	sbi->s_resgid = old_opts.s_resgid;
	sbi->s_commit_interval = old_opts.s_commit_interval;
	sbi->s_min_batch_time = old_opts.s_min_batch_time;
	sbi->s_max_batch_time = old_opts.s_max_batch_time;
<<<<<<< HEAD
#ifdef CONFIG_QUOTA
	sbi->s_jquota_fmt = old_opts.s_jquota_fmt;
	for (i = 0; i < MAXQUOTAS; i++) {
		if (sbi->s_qf_names[i] &&
		    old_opts.s_qf_names[i] != sbi->s_qf_names[i])
			kfree(sbi->s_qf_names[i]);
		sbi->s_qf_names[i] = old_opts.s_qf_names[i];
	}
#endif
	unlock_super(sb);
	kfree(orig_data);
	return err;
}

=======
	ext4_writepages_up_write(sb, alloc_ctx);

	if (!test_opt(sb, BLOCK_VALIDITY) && sbi->s_system_blks)
		ext4_release_system_zone(sb);
#ifdef CONFIG_QUOTA
	sbi->s_jquota_fmt = old_opts.s_jquota_fmt;
	for (i = 0; i < EXT4_MAXQUOTAS; i++) {
		to_free[i] = get_qf_name(sb, sbi, i);
		rcu_assign_pointer(sbi->s_qf_names[i], old_opts.s_qf_names[i]);
	}
	synchronize_rcu();
	for (i = 0; i < EXT4_MAXQUOTAS; i++)
		kfree(to_free[i]);
#endif
	if (!ext4_has_feature_mmp(sb) || sb_rdonly(sb))
		ext4_stop_mmpd(sbi);
	return err;
}

static int ext4_reconfigure(struct fs_context *fc)
{
	struct super_block *sb = fc->root->d_sb;
	int ret;

	fc->s_fs_info = EXT4_SB(sb);

	ret = ext4_check_opt_consistency(fc, sb);
	if (ret < 0)
		return ret;

	ret = __ext4_remount(fc, sb);
	if (ret < 0)
		return ret;

	ext4_msg(sb, KERN_INFO, "re-mounted %pU %s. Quota mode: %s.",
		 &sb->s_uuid, sb_rdonly(sb) ? "ro" : "r/w",
		 ext4_quota_mode(sb));

	return 0;
}

#ifdef CONFIG_QUOTA
static int ext4_statfs_project(struct super_block *sb,
			       kprojid_t projid, struct kstatfs *buf)
{
	struct kqid qid;
	struct dquot *dquot;
	u64 limit;
	u64 curblock;

	qid = make_kqid_projid(projid);
	dquot = dqget(sb, qid);
	if (IS_ERR(dquot))
		return PTR_ERR(dquot);
	spin_lock(&dquot->dq_dqb_lock);

	limit = min_not_zero(dquot->dq_dqb.dqb_bsoftlimit,
			     dquot->dq_dqb.dqb_bhardlimit);
	limit >>= sb->s_blocksize_bits;

	if (limit && buf->f_blocks > limit) {
		curblock = (dquot->dq_dqb.dqb_curspace +
			    dquot->dq_dqb.dqb_rsvspace) >> sb->s_blocksize_bits;
		buf->f_blocks = limit;
		buf->f_bfree = buf->f_bavail =
			(buf->f_blocks > curblock) ?
			 (buf->f_blocks - curblock) : 0;
	}

	limit = min_not_zero(dquot->dq_dqb.dqb_isoftlimit,
			     dquot->dq_dqb.dqb_ihardlimit);
	if (limit && buf->f_files > limit) {
		buf->f_files = limit;
		buf->f_ffree =
			(buf->f_files > dquot->dq_dqb.dqb_curinodes) ?
			 (buf->f_files - dquot->dq_dqb.dqb_curinodes) : 0;
	}

	spin_unlock(&dquot->dq_dqb_lock);
	dqput(dquot);
	return 0;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ext4_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	struct super_block *sb = dentry->d_sb;
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
<<<<<<< HEAD
	ext4_fsblk_t overhead = 0;
	u64 fsid;
	s64 bfree;
=======
	ext4_fsblk_t overhead = 0, resv_blocks;
	s64 bfree;
	resv_blocks = EXT4_C2B(sbi, atomic64_read(&sbi->s_resv_clusters));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!test_opt(sb, MINIX_DF))
		overhead = sbi->s_overhead;

	buf->f_type = EXT4_SUPER_MAGIC;
	buf->f_bsize = sb->s_blocksize;
<<<<<<< HEAD
	buf->f_blocks = ext4_blocks_count(es) - EXT4_C2B(sbi, sbi->s_overhead);
=======
	buf->f_blocks = ext4_blocks_count(es) - EXT4_C2B(sbi, overhead);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bfree = percpu_counter_sum_positive(&sbi->s_freeclusters_counter) -
		percpu_counter_sum_positive(&sbi->s_dirtyclusters_counter);
	/* prevent underflow in case that few free space is available */
	buf->f_bfree = EXT4_C2B(sbi, max_t(s64, bfree, 0));
<<<<<<< HEAD
	buf->f_bavail = buf->f_bfree - ext4_r_blocks_count(es);
	if (buf->f_bfree < ext4_r_blocks_count(es))
=======
	buf->f_bavail = buf->f_bfree -
			(ext4_r_blocks_count(es) + resv_blocks);
	if (buf->f_bfree < (ext4_r_blocks_count(es) + resv_blocks))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		buf->f_bavail = 0;
	buf->f_files = le32_to_cpu(es->s_inodes_count);
	buf->f_ffree = percpu_counter_sum_positive(&sbi->s_freeinodes_counter);
	buf->f_namelen = EXT4_NAME_LEN;
<<<<<<< HEAD
	fsid = le64_to_cpup((void *)es->s_uuid) ^
	       le64_to_cpup((void *)es->s_uuid + sizeof(u64));
	buf->f_fsid.val[0] = fsid & 0xFFFFFFFFUL;
	buf->f_fsid.val[1] = (fsid >> 32) & 0xFFFFFFFFUL;

	return 0;
}

/* Helper function for writing quotas on sync - we need to start transaction
 * before quota file is locked for write. Otherwise the are possible deadlocks:
 * Process 1                         Process 2
 * ext4_create()                     quota_sync()
 *   jbd2_journal_start()                  write_dquot()
 *   dquot_initialize()                         down(dqio_mutex)
 *     down(dqio_mutex)                    jbd2_journal_start()
 *
 */

#ifdef CONFIG_QUOTA

static inline struct inode *dquot_to_inode(struct dquot *dquot)
{
	return sb_dqopt(dquot->dq_sb)->files[dquot->dq_type];
=======
	buf->f_fsid = uuid_to_fsid(es->s_uuid);

#ifdef CONFIG_QUOTA
	if (ext4_test_inode_flag(dentry->d_inode, EXT4_INODE_PROJINHERIT) &&
	    sb_has_quota_limits_enabled(sb, PRJQUOTA))
		ext4_statfs_project(sb, EXT4_I(dentry->d_inode)->i_projid, buf);
#endif
	return 0;
}


#ifdef CONFIG_QUOTA

/*
 * Helper functions so that transaction is started before we acquire dqio_sem
 * to keep correct lock ordering of transaction > dqio_sem
 */
static inline struct inode *dquot_to_inode(struct dquot *dquot)
{
	return sb_dqopt(dquot->dq_sb)->files[dquot->dq_id.type];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ext4_write_dquot(struct dquot *dquot)
{
	int ret, err;
	handle_t *handle;
	struct inode *inode;

	inode = dquot_to_inode(dquot);
<<<<<<< HEAD
	handle = ext4_journal_start(inode,
=======
	handle = ext4_journal_start(inode, EXT4_HT_QUOTA,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    EXT4_QUOTA_TRANS_BLOCKS(dquot->dq_sb));
	if (IS_ERR(handle))
		return PTR_ERR(handle);
	ret = dquot_commit(dquot);
<<<<<<< HEAD
=======
	if (ret < 0)
		ext4_error_err(dquot->dq_sb, -ret,
			       "Failed to commit dquot type %d",
			       dquot->dq_id.type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ext4_journal_stop(handle);
	if (!ret)
		ret = err;
	return ret;
}

static int ext4_acquire_dquot(struct dquot *dquot)
{
	int ret, err;
	handle_t *handle;

<<<<<<< HEAD
	handle = ext4_journal_start(dquot_to_inode(dquot),
=======
	handle = ext4_journal_start(dquot_to_inode(dquot), EXT4_HT_QUOTA,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    EXT4_QUOTA_INIT_BLOCKS(dquot->dq_sb));
	if (IS_ERR(handle))
		return PTR_ERR(handle);
	ret = dquot_acquire(dquot);
<<<<<<< HEAD
=======
	if (ret < 0)
		ext4_error_err(dquot->dq_sb, -ret,
			      "Failed to acquire dquot type %d",
			      dquot->dq_id.type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ext4_journal_stop(handle);
	if (!ret)
		ret = err;
	return ret;
}

static int ext4_release_dquot(struct dquot *dquot)
{
	int ret, err;
	handle_t *handle;

<<<<<<< HEAD
	handle = ext4_journal_start(dquot_to_inode(dquot),
=======
	handle = ext4_journal_start(dquot_to_inode(dquot), EXT4_HT_QUOTA,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    EXT4_QUOTA_DEL_BLOCKS(dquot->dq_sb));
	if (IS_ERR(handle)) {
		/* Release dquot anyway to avoid endless cycle in dqput() */
		dquot_release(dquot);
		return PTR_ERR(handle);
	}
	ret = dquot_release(dquot);
<<<<<<< HEAD
=======
	if (ret < 0)
		ext4_error_err(dquot->dq_sb, -ret,
			       "Failed to release dquot type %d",
			       dquot->dq_id.type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ext4_journal_stop(handle);
	if (!ret)
		ret = err;
	return ret;
}

static int ext4_mark_dquot_dirty(struct dquot *dquot)
{
<<<<<<< HEAD
	/* Are we journaling quotas? */
	if (EXT4_SB(dquot->dq_sb)->s_qf_names[USRQUOTA] ||
	    EXT4_SB(dquot->dq_sb)->s_qf_names[GRPQUOTA]) {
=======
	struct super_block *sb = dquot->dq_sb;

	if (ext4_is_quota_journalled(sb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dquot_mark_dquot_dirty(dquot);
		return ext4_write_dquot(dquot);
	} else {
		return dquot_mark_dquot_dirty(dquot);
	}
}

static int ext4_write_info(struct super_block *sb, int type)
{
	int ret, err;
	handle_t *handle;

	/* Data block + inode block */
<<<<<<< HEAD
	handle = ext4_journal_start(sb->s_root->d_inode, 2);
=======
	handle = ext4_journal_start_sb(sb, EXT4_HT_QUOTA, 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(handle))
		return PTR_ERR(handle);
	ret = dquot_commit_info(sb, type);
	err = ext4_journal_stop(handle);
	if (!ret)
		ret = err;
	return ret;
}

<<<<<<< HEAD
/*
 * Turn on quotas during mount time - we need to find
 * the quota file and such...
 */
static int ext4_quota_on_mount(struct super_block *sb, int type)
{
	return dquot_quota_on_mount(sb, EXT4_SB(sb)->s_qf_names[type],
					EXT4_SB(sb)->s_jquota_fmt, type);
=======
static void lockdep_set_quota_inode(struct inode *inode, int subclass)
{
	struct ext4_inode_info *ei = EXT4_I(inode);

	/* The first argument of lockdep_set_subclass has to be
	 * *exactly* the same as the argument to init_rwsem() --- in
	 * this case, in init_once() --- or lockdep gets unhappy
	 * because the name of the lock is set using the
	 * stringification of the argument to init_rwsem().
	 */
	(void) ei;	/* shut up clang warning if !CONFIG_LOCKDEP */
	lockdep_set_subclass(&ei->i_data_sem, subclass);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Standard function to be called on quota_on
 */
static int ext4_quota_on(struct super_block *sb, int type, int format_id,
<<<<<<< HEAD
			 struct path *path)
=======
			 const struct path *path)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;

	if (!test_opt(sb, QUOTA))
		return -EINVAL;

	/* Quotafile not on the same filesystem? */
	if (path->dentry->d_sb != sb)
		return -EXDEV;
<<<<<<< HEAD
=======

	/* Quota already enabled for this file? */
	if (IS_NOQUOTA(d_inode(path->dentry)))
		return -EBUSY;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Journaling quota? */
	if (EXT4_SB(sb)->s_qf_names[type]) {
		/* Quotafile not in fs root? */
		if (path->dentry->d_parent != sb->s_root)
			ext4_msg(sb, KERN_WARNING,
				"Quota file not on filesystem root. "
				"Journaled quota will not work");
<<<<<<< HEAD
	}

	/*
	 * When we journal data on quota file, we have to flush journal to see
	 * all updates to the file when we bypass pagecache...
	 */
	if (EXT4_SB(sb)->s_journal &&
	    ext4_should_journal_data(path->dentry->d_inode)) {
		/*
		 * We don't need to lock updates but journal_flush() could
		 * otherwise be livelocked...
		 */
		jbd2_journal_lock_updates(EXT4_SB(sb)->s_journal);
		err = jbd2_journal_flush(EXT4_SB(sb)->s_journal);
		jbd2_journal_unlock_updates(EXT4_SB(sb)->s_journal);
		if (err)
			return err;
	}

	return dquot_quota_on(sb, type, format_id, path);
=======
		sb_dqopt(sb)->flags |= DQUOT_NOLIST_DIRTY;
	} else {
		/*
		 * Clear the flag just in case mount options changed since
		 * last time.
		 */
		sb_dqopt(sb)->flags &= ~DQUOT_NOLIST_DIRTY;
	}

	lockdep_set_quota_inode(path->dentry->d_inode, I_DATA_SEM_QUOTA);
	err = dquot_quota_on(sb, type, format_id, path);
	if (!err) {
		struct inode *inode = d_inode(path->dentry);
		handle_t *handle;

		/*
		 * Set inode flags to prevent userspace from messing with quota
		 * files. If this fails, we return success anyway since quotas
		 * are already enabled and this is not a hard failure.
		 */
		inode_lock(inode);
		handle = ext4_journal_start(inode, EXT4_HT_QUOTA, 1);
		if (IS_ERR(handle))
			goto unlock_inode;
		EXT4_I(inode)->i_flags |= EXT4_NOATIME_FL | EXT4_IMMUTABLE_FL;
		inode_set_flags(inode, S_NOATIME | S_IMMUTABLE,
				S_NOATIME | S_IMMUTABLE);
		err = ext4_mark_inode_dirty(handle, inode);
		ext4_journal_stop(handle);
	unlock_inode:
		inode_unlock(inode);
		if (err)
			dquot_quota_off(sb, type);
	}
	if (err)
		lockdep_set_quota_inode(path->dentry->d_inode,
					     I_DATA_SEM_NORMAL);
	return err;
}

static inline bool ext4_check_quota_inum(int type, unsigned long qf_inum)
{
	switch (type) {
	case USRQUOTA:
		return qf_inum == EXT4_USR_QUOTA_INO;
	case GRPQUOTA:
		return qf_inum == EXT4_GRP_QUOTA_INO;
	case PRJQUOTA:
		return qf_inum >= EXT4_GOOD_OLD_FIRST_INO;
	default:
		BUG();
	}
}

static int ext4_quota_enable(struct super_block *sb, int type, int format_id,
			     unsigned int flags)
{
	int err;
	struct inode *qf_inode;
	unsigned long qf_inums[EXT4_MAXQUOTAS] = {
		le32_to_cpu(EXT4_SB(sb)->s_es->s_usr_quota_inum),
		le32_to_cpu(EXT4_SB(sb)->s_es->s_grp_quota_inum),
		le32_to_cpu(EXT4_SB(sb)->s_es->s_prj_quota_inum)
	};

	BUG_ON(!ext4_has_feature_quota(sb));

	if (!qf_inums[type])
		return -EPERM;

	if (!ext4_check_quota_inum(type, qf_inums[type])) {
		ext4_error(sb, "Bad quota inum: %lu, type: %d",
				qf_inums[type], type);
		return -EUCLEAN;
	}

	qf_inode = ext4_iget(sb, qf_inums[type], EXT4_IGET_SPECIAL);
	if (IS_ERR(qf_inode)) {
		ext4_error(sb, "Bad quota inode: %lu, type: %d",
				qf_inums[type], type);
		return PTR_ERR(qf_inode);
	}

	/* Don't account quota for quota files to avoid recursion */
	qf_inode->i_flags |= S_NOQUOTA;
	lockdep_set_quota_inode(qf_inode, I_DATA_SEM_QUOTA);
	err = dquot_load_quota_inode(qf_inode, type, format_id, flags);
	if (err)
		lockdep_set_quota_inode(qf_inode, I_DATA_SEM_NORMAL);
	iput(qf_inode);

	return err;
}

/* Enable usage tracking for all quota types. */
int ext4_enable_quotas(struct super_block *sb)
{
	int type, err = 0;
	unsigned long qf_inums[EXT4_MAXQUOTAS] = {
		le32_to_cpu(EXT4_SB(sb)->s_es->s_usr_quota_inum),
		le32_to_cpu(EXT4_SB(sb)->s_es->s_grp_quota_inum),
		le32_to_cpu(EXT4_SB(sb)->s_es->s_prj_quota_inum)
	};
	bool quota_mopt[EXT4_MAXQUOTAS] = {
		test_opt(sb, USRQUOTA),
		test_opt(sb, GRPQUOTA),
		test_opt(sb, PRJQUOTA),
	};

	sb_dqopt(sb)->flags |= DQUOT_QUOTA_SYS_FILE | DQUOT_NOLIST_DIRTY;
	for (type = 0; type < EXT4_MAXQUOTAS; type++) {
		if (qf_inums[type]) {
			err = ext4_quota_enable(sb, type, QFMT_VFS_V1,
				DQUOT_USAGE_ENABLED |
				(quota_mopt[type] ? DQUOT_LIMITS_ENABLED : 0));
			if (err) {
				ext4_warning(sb,
					"Failed to enable quota tracking "
					"(type=%d, err=%d, ino=%lu). "
					"Please run e2fsck to fix.", type,
					err, qf_inums[type]);

				ext4_quotas_off(sb, type);
				return err;
			}
		}
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ext4_quota_off(struct super_block *sb, int type)
{
	struct inode *inode = sb_dqopt(sb)->files[type];
	handle_t *handle;
<<<<<<< HEAD
=======
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Force all delayed allocation blocks to be allocated.
	 * Caller already holds s_umount sem */
	if (test_opt(sb, DELALLOC))
		sync_filesystem(sb);

<<<<<<< HEAD
	if (!inode)
		goto out;

	/* Update modification times of quota files when userspace can
	 * start looking at them */
	handle = ext4_journal_start(inode, 1);
	if (IS_ERR(handle))
		goto out;
	inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	ext4_mark_inode_dirty(handle, inode);
	ext4_journal_stop(handle);

=======
	if (!inode || !igrab(inode))
		goto out;

	err = dquot_quota_off(sb, type);
	if (err || ext4_has_feature_quota(sb))
		goto out_put;
	/*
	 * When the filesystem was remounted read-only first, we cannot cleanup
	 * inode flags here. Bad luck but people should be using QUOTA feature
	 * these days anyway.
	 */
	if (sb_rdonly(sb))
		goto out_put;

	inode_lock(inode);
	/*
	 * Update modification times of quota files when userspace can
	 * start looking at them. If we fail, we return success anyway since
	 * this is not a hard failure and quotas are already disabled.
	 */
	handle = ext4_journal_start(inode, EXT4_HT_QUOTA, 1);
	if (IS_ERR(handle)) {
		err = PTR_ERR(handle);
		goto out_unlock;
	}
	EXT4_I(inode)->i_flags &= ~(EXT4_NOATIME_FL | EXT4_IMMUTABLE_FL);
	inode_set_flags(inode, 0, S_NOATIME | S_IMMUTABLE);
	inode_set_mtime_to_ts(inode, inode_set_ctime_current(inode));
	err = ext4_mark_inode_dirty(handle, inode);
	ext4_journal_stop(handle);
out_unlock:
	inode_unlock(inode);
out_put:
	lockdep_set_quota_inode(inode, I_DATA_SEM_NORMAL);
	iput(inode);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return dquot_quota_off(sb, type);
}

/* Read data from quotafile - avoid pagecache and such because we cannot afford
 * acquiring the locks... As quota files are never truncated and quota code
 * itself serializes the operations (and no one else should touch the files)
 * we don't have to be afraid of races */
static ssize_t ext4_quota_read(struct super_block *sb, int type, char *data,
			       size_t len, loff_t off)
{
	struct inode *inode = sb_dqopt(sb)->files[type];
	ext4_lblk_t blk = off >> EXT4_BLOCK_SIZE_BITS(sb);
<<<<<<< HEAD
	int err = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int offset = off & (sb->s_blocksize - 1);
	int tocopy;
	size_t toread;
	struct buffer_head *bh;
	loff_t i_size = i_size_read(inode);

	if (off > i_size)
		return 0;
	if (off+len > i_size)
		len = i_size-off;
	toread = len;
	while (toread > 0) {
<<<<<<< HEAD
		tocopy = sb->s_blocksize - offset < toread ?
				sb->s_blocksize - offset : toread;
		bh = ext4_bread(NULL, inode, blk, 0, &err);
		if (err)
			return err;
=======
		tocopy = min_t(unsigned long, sb->s_blocksize - offset, toread);
		bh = ext4_bread(NULL, inode, blk, 0);
		if (IS_ERR(bh))
			return PTR_ERR(bh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!bh)	/* A hole? */
			memset(data, 0, tocopy);
		else
			memcpy(data, bh->b_data+offset, tocopy);
		brelse(bh);
		offset = 0;
		toread -= tocopy;
		data += tocopy;
		blk++;
	}
	return len;
}

/* Write to quotafile (we know the transaction is already started and has
 * enough credits) */
static ssize_t ext4_quota_write(struct super_block *sb, int type,
				const char *data, size_t len, loff_t off)
{
	struct inode *inode = sb_dqopt(sb)->files[type];
	ext4_lblk_t blk = off >> EXT4_BLOCK_SIZE_BITS(sb);
<<<<<<< HEAD
	int err = 0;
	int offset = off & (sb->s_blocksize - 1);
	struct buffer_head *bh;
	handle_t *handle = journal_current_handle();

	if (EXT4_SB(sb)->s_journal && !handle) {
=======
	int err = 0, err2 = 0, offset = off & (sb->s_blocksize - 1);
	int retries = 0;
	struct buffer_head *bh;
	handle_t *handle = journal_current_handle();

	if (!handle) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ext4_msg(sb, KERN_WARNING, "Quota write (off=%llu, len=%llu)"
			" cancelled because transaction is not started",
			(unsigned long long)off, (unsigned long long)len);
		return -EIO;
	}
	/*
	 * Since we account only one data block in transaction credits,
	 * then it is impossible to cross a block boundary.
	 */
	if (sb->s_blocksize - offset < len) {
		ext4_msg(sb, KERN_WARNING, "Quota write (off=%llu, len=%llu)"
			" cancelled because not block aligned",
			(unsigned long long)off, (unsigned long long)len);
		return -EIO;
	}

<<<<<<< HEAD
	mutex_lock_nested(&inode->i_mutex, I_MUTEX_QUOTA);
	bh = ext4_bread(handle, inode, blk, 1, &err);
	if (!bh)
		goto out;
	err = ext4_journal_get_write_access(handle, bh);
	if (err) {
		brelse(bh);
		goto out;
=======
	do {
		bh = ext4_bread(handle, inode, blk,
				EXT4_GET_BLOCKS_CREATE |
				EXT4_GET_BLOCKS_METADATA_NOFAIL);
	} while (PTR_ERR(bh) == -ENOSPC &&
		 ext4_should_retry_alloc(inode->i_sb, &retries));
	if (IS_ERR(bh))
		return PTR_ERR(bh);
	if (!bh)
		goto out;
	BUFFER_TRACE(bh, "get write access");
	err = ext4_journal_get_write_access(handle, sb, bh, EXT4_JTR_NONE);
	if (err) {
		brelse(bh);
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	lock_buffer(bh);
	memcpy(bh->b_data+offset, data, len);
	flush_dcache_page(bh->b_page);
	unlock_buffer(bh);
	err = ext4_handle_dirty_metadata(handle, NULL, bh);
	brelse(bh);
out:
<<<<<<< HEAD
	if (err) {
		mutex_unlock(&inode->i_mutex);
		return err;
	}
	if (inode->i_size < off + len) {
		i_size_write(inode, off + len);
		EXT4_I(inode)->i_disksize = inode->i_size;
		ext4_mark_inode_dirty(handle, inode);
	}
	mutex_unlock(&inode->i_mutex);
	return len;
}

#endif

static struct dentry *ext4_mount(struct file_system_type *fs_type, int flags,
		       const char *dev_name, void *data)
{
	return mount_bdev(fs_type, flags, dev_name, data, ext4_fill_super);
}

#if !defined(CONFIG_EXT2_FS) && !defined(CONFIG_EXT2_FS_MODULE) && defined(CONFIG_EXT4_USE_FOR_EXT23)
=======
	if (inode->i_size < off + len) {
		i_size_write(inode, off + len);
		EXT4_I(inode)->i_disksize = inode->i_size;
		err2 = ext4_mark_inode_dirty(handle, inode);
		if (unlikely(err2 && !err))
			err = err2;
	}
	return err ? err : len;
}
#endif

#if !defined(CONFIG_EXT2_FS) && !defined(CONFIG_EXT2_FS_MODULE) && defined(CONFIG_EXT4_USE_FOR_EXT2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void register_as_ext2(void)
{
	int err = register_filesystem(&ext2_fs_type);
	if (err)
		printk(KERN_WARNING
		       "EXT4-fs: Unable to register as ext2 (%d)\n", err);
}

static inline void unregister_as_ext2(void)
{
	unregister_filesystem(&ext2_fs_type);
}

static inline int ext2_feature_set_ok(struct super_block *sb)
{
<<<<<<< HEAD
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, ~EXT2_FEATURE_INCOMPAT_SUPP))
		return 0;
	if (sb->s_flags & MS_RDONLY)
		return 1;
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, ~EXT2_FEATURE_RO_COMPAT_SUPP))
=======
	if (ext4_has_unknown_ext2_incompat_features(sb))
		return 0;
	if (sb_rdonly(sb))
		return 1;
	if (ext4_has_unknown_ext2_ro_compat_features(sb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	return 1;
}
#else
static inline void register_as_ext2(void) { }
static inline void unregister_as_ext2(void) { }
static inline int ext2_feature_set_ok(struct super_block *sb) { return 0; }
#endif

<<<<<<< HEAD
#if !defined(CONFIG_EXT3_FS) && !defined(CONFIG_EXT3_FS_MODULE) && defined(CONFIG_EXT4_USE_FOR_EXT23)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void register_as_ext3(void)
{
	int err = register_filesystem(&ext3_fs_type);
	if (err)
		printk(KERN_WARNING
		       "EXT4-fs: Unable to register as ext3 (%d)\n", err);
}

static inline void unregister_as_ext3(void)
{
	unregister_filesystem(&ext3_fs_type);
}

static inline int ext3_feature_set_ok(struct super_block *sb)
{
<<<<<<< HEAD
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, ~EXT3_FEATURE_INCOMPAT_SUPP))
		return 0;
	if (!EXT4_HAS_COMPAT_FEATURE(sb, EXT4_FEATURE_COMPAT_HAS_JOURNAL))
		return 0;
	if (sb->s_flags & MS_RDONLY)
		return 1;
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, ~EXT3_FEATURE_RO_COMPAT_SUPP))
		return 0;
	return 1;
}
#else
static inline void register_as_ext3(void) { }
static inline void unregister_as_ext3(void) { }
static inline int ext3_feature_set_ok(struct super_block *sb) { return 0; }
#endif

static struct file_system_type ext4_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "ext4",
	.mount		= ext4_mount,
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
};
MODULE_ALIAS_FS("ext4");

static int __init ext4_init_feat_adverts(void)
{
	struct ext4_features *ef;
	int ret = -ENOMEM;

	ef = kzalloc(sizeof(struct ext4_features), GFP_KERNEL);
	if (!ef)
		goto out;

	ef->f_kobj.kset = ext4_kset;
	init_completion(&ef->f_kobj_unregister);
	ret = kobject_init_and_add(&ef->f_kobj, &ext4_feat_ktype, NULL,
				   "features");
	if (ret) {
		kfree(ef);
		goto out;
	}

	ext4_feat = ef;
	ret = 0;
out:
	return ret;
}

static void ext4_exit_feat_adverts(void)
{
	kobject_put(&ext4_feat->f_kobj);
	wait_for_completion(&ext4_feat->f_kobj_unregister);
	kfree(ext4_feat);
}

/* Shared across all ext4 file systems */
wait_queue_head_t ext4__ioend_wq[EXT4_WQ_HASH_SZ];
struct mutex ext4__aio_mutex[EXT4_WQ_HASH_SZ];
=======
	if (ext4_has_unknown_ext3_incompat_features(sb))
		return 0;
	if (!ext4_has_feature_journal(sb))
		return 0;
	if (sb_rdonly(sb))
		return 1;
	if (ext4_has_unknown_ext3_ro_compat_features(sb))
		return 0;
	return 1;
}

static void ext4_kill_sb(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct file *bdev_file = sbi ? sbi->s_journal_bdev_file : NULL;

	kill_block_super(sb);

	if (bdev_file)
		bdev_fput(bdev_file);
}

static struct file_system_type ext4_fs_type = {
	.owner			= THIS_MODULE,
	.name			= "ext4",
	.init_fs_context	= ext4_init_fs_context,
	.parameters		= ext4_param_specs,
	.kill_sb		= ext4_kill_sb,
	.fs_flags		= FS_REQUIRES_DEV | FS_ALLOW_IDMAP,
};
MODULE_ALIAS_FS("ext4");

/* Shared across all ext4 file systems */
wait_queue_head_t ext4__ioend_wq[EXT4_WQ_HASH_SZ];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init ext4_init_fs(void)
{
	int i, err;

<<<<<<< HEAD
	ext4_li_info = NULL;
	mutex_init(&ext4_li_mtx);

	ext4_check_flag_values();

	for (i = 0; i < EXT4_WQ_HASH_SZ; i++) {
		mutex_init(&ext4__aio_mutex[i]);
		init_waitqueue_head(&ext4__ioend_wq[i]);
	}

	err = ext4_init_pageio();
	if (err)
		return err;
	err = ext4_init_system_zone();
	if (err)
		goto out6;
	ext4_kset = kset_create_and_add("ext4", NULL, fs_kobj);
	if (!ext4_kset)
		goto out5;
	ext4_proc_root = proc_mkdir("fs/ext4", NULL);

	err = ext4_init_feat_adverts();
	if (err)
		goto out4;

	err = ext4_init_mballoc();
	if (err)
		goto out3;

	err = ext4_init_xattr();
=======
	ratelimit_state_init(&ext4_mount_msg_ratelimit, 30 * HZ, 64);
	ext4_li_info = NULL;

	/* Build-time check for flags consistency */
	ext4_check_flag_values();

	for (i = 0; i < EXT4_WQ_HASH_SZ; i++)
		init_waitqueue_head(&ext4__ioend_wq[i]);

	err = ext4_init_es();
	if (err)
		return err;

	err = ext4_init_pending();
	if (err)
		goto out7;

	err = ext4_init_post_read_processing();
	if (err)
		goto out6;

	err = ext4_init_pageio();
	if (err)
		goto out5;

	err = ext4_init_system_zone();
	if (err)
		goto out4;

	err = ext4_init_sysfs();
	if (err)
		goto out3;

	err = ext4_init_mballoc();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out2;
	err = init_inodecache();
	if (err)
		goto out1;
<<<<<<< HEAD
=======

	err = ext4_fc_init_dentry_cache();
	if (err)
		goto out05;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	register_as_ext3();
	register_as_ext2();
	err = register_filesystem(&ext4_fs_type);
	if (err)
		goto out;

	return 0;
out:
	unregister_as_ext2();
	unregister_as_ext3();
<<<<<<< HEAD
	destroy_inodecache();
out1:
	ext4_exit_xattr();
out2:
	ext4_exit_mballoc();
out3:
	ext4_exit_feat_adverts();
out4:
	if (ext4_proc_root)
		remove_proc_entry("fs/ext4", NULL);
	kset_unregister(ext4_kset);
out5:
	ext4_exit_system_zone();
out6:
	ext4_exit_pageio();
=======
	ext4_fc_destroy_dentry_cache();
out05:
	destroy_inodecache();
out1:
	ext4_exit_mballoc();
out2:
	ext4_exit_sysfs();
out3:
	ext4_exit_system_zone();
out4:
	ext4_exit_pageio();
out5:
	ext4_exit_post_read_processing();
out6:
	ext4_exit_pending();
out7:
	ext4_exit_es();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static void __exit ext4_exit_fs(void)
{
	ext4_destroy_lazyinit_thread();
	unregister_as_ext2();
	unregister_as_ext3();
	unregister_filesystem(&ext4_fs_type);
<<<<<<< HEAD
	destroy_inodecache();
	ext4_exit_xattr();
	ext4_exit_mballoc();
	ext4_exit_feat_adverts();
	remove_proc_entry("fs/ext4", NULL);
	kset_unregister(ext4_kset);
	ext4_exit_system_zone();
	ext4_exit_pageio();
=======
	ext4_fc_destroy_dentry_cache();
	destroy_inodecache();
	ext4_exit_mballoc();
	ext4_exit_sysfs();
	ext4_exit_system_zone();
	ext4_exit_pageio();
	ext4_exit_post_read_processing();
	ext4_exit_es();
	ext4_exit_pending();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

MODULE_AUTHOR("Remy Card, Stephen Tweedie, Andrew Morton, Andreas Dilger, Theodore Ts'o and others");
MODULE_DESCRIPTION("Fourth Extended Filesystem");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_SOFTDEP("pre: crc32c");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_init(ext4_init_fs)
module_exit(ext4_exit_fs)
