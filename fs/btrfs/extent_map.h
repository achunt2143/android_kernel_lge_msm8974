<<<<<<< HEAD
#ifndef __EXTENTMAP__
#define __EXTENTMAP__

#include <linux/rbtree.h>

#define EXTENT_MAP_LAST_BYTE (u64)-4
#define EXTENT_MAP_HOLE (u64)-3
#define EXTENT_MAP_INLINE (u64)-2
#define EXTENT_MAP_DELALLOC (u64)-1

/* bits for the flags field */
#define EXTENT_FLAG_PINNED 0 /* this entry not yet on disk, don't free it */
#define EXTENT_FLAG_COMPRESSED 1
#define EXTENT_FLAG_VACANCY 2 /* no file extent item found */
#define EXTENT_FLAG_PREALLOC 3 /* pre-allocated extent */

=======
/* SPDX-License-Identifier: GPL-2.0 */

#ifndef BTRFS_EXTENT_MAP_H
#define BTRFS_EXTENT_MAP_H

#include <linux/compiler_types.h>
#include <linux/rwlock_types.h>
#include <linux/rbtree.h>
#include <linux/list.h>
#include <linux/refcount.h>
#include "misc.h"
#include "extent_map.h"
#include "compression.h"

struct btrfs_inode;
struct btrfs_fs_info;

#define EXTENT_MAP_LAST_BYTE ((u64)-4)
#define EXTENT_MAP_HOLE ((u64)-3)
#define EXTENT_MAP_INLINE ((u64)-2)

/* bits for the extent_map::flags field */
enum {
	/* this entry not yet on disk, don't free it */
	ENUM_BIT(EXTENT_FLAG_PINNED),
	ENUM_BIT(EXTENT_FLAG_COMPRESS_ZLIB),
	ENUM_BIT(EXTENT_FLAG_COMPRESS_LZO),
	ENUM_BIT(EXTENT_FLAG_COMPRESS_ZSTD),
	/* pre-allocated extent */
	ENUM_BIT(EXTENT_FLAG_PREALLOC),
	/* Logging this extent */
	ENUM_BIT(EXTENT_FLAG_LOGGING),
	/* Filling in a preallocated extent */
	ENUM_BIT(EXTENT_FLAG_FILLING),
	/* This em is merged from two or more physically adjacent ems */
	ENUM_BIT(EXTENT_FLAG_MERGED),
};

/*
 * Keep this structure as compact as possible, as we can have really large
 * amounts of allocated extent maps at any time.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct extent_map {
	struct rb_node rb_node;

	/* all of these are in bytes */
	u64 start;
	u64 len;
<<<<<<< HEAD
	u64 orig_start;
	u64 block_start;
	u64 block_len;
	unsigned long flags;
	struct block_device *bdev;
	atomic_t refs;
	unsigned int in_tree;
	unsigned int compress_type;
};

struct extent_map_tree {
	struct rb_root map;
	rwlock_t lock;
};

static inline u64 extent_map_end(struct extent_map *em)
=======
	u64 mod_start;
	u64 mod_len;
	u64 orig_start;
	u64 orig_block_len;
	u64 ram_bytes;
	u64 block_start;
	u64 block_len;

	/*
	 * Generation of the extent map, for merged em it's the highest
	 * generation of all merged ems.
	 * For non-merged extents, it's from btrfs_file_extent_item::generation.
	 */
	u64 generation;
	u32 flags;
	refcount_t refs;
	struct list_head list;
};

struct extent_map_tree {
	struct rb_root_cached map;
	struct list_head modified_extents;
	rwlock_t lock;
};

struct btrfs_inode;

static inline void extent_map_set_compression(struct extent_map *em,
					      enum btrfs_compression_type type)
{
	if (type == BTRFS_COMPRESS_ZLIB)
		em->flags |= EXTENT_FLAG_COMPRESS_ZLIB;
	else if (type == BTRFS_COMPRESS_LZO)
		em->flags |= EXTENT_FLAG_COMPRESS_LZO;
	else if (type == BTRFS_COMPRESS_ZSTD)
		em->flags |= EXTENT_FLAG_COMPRESS_ZSTD;
}

static inline enum btrfs_compression_type extent_map_compression(const struct extent_map *em)
{
	if (em->flags & EXTENT_FLAG_COMPRESS_ZLIB)
		return BTRFS_COMPRESS_ZLIB;

	if (em->flags & EXTENT_FLAG_COMPRESS_LZO)
		return BTRFS_COMPRESS_LZO;

	if (em->flags & EXTENT_FLAG_COMPRESS_ZSTD)
		return BTRFS_COMPRESS_ZSTD;

	return BTRFS_COMPRESS_NONE;
}

/*
 * More efficient way to determine if extent is compressed, instead of using
 * 'extent_map_compression() != BTRFS_COMPRESS_NONE'.
 */
static inline bool extent_map_is_compressed(const struct extent_map *em)
{
	return (em->flags & (EXTENT_FLAG_COMPRESS_ZLIB |
			     EXTENT_FLAG_COMPRESS_LZO |
			     EXTENT_FLAG_COMPRESS_ZSTD)) != 0;
}

static inline int extent_map_in_tree(const struct extent_map *em)
{
	return !RB_EMPTY_NODE(&em->rb_node);
}

static inline u64 extent_map_end(const struct extent_map *em)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (em->start + em->len < em->start)
		return (u64)-1;
	return em->start + em->len;
}

<<<<<<< HEAD
static inline u64 extent_map_block_end(struct extent_map *em)
{
	if (em->block_start + em->block_len < em->block_start)
		return (u64)-1;
	return em->block_start + em->block_len;
}

void extent_map_tree_init(struct extent_map_tree *tree);
struct extent_map *lookup_extent_mapping(struct extent_map_tree *tree,
					 u64 start, u64 len);
int add_extent_mapping(struct extent_map_tree *tree,
		       struct extent_map *em);
int remove_extent_mapping(struct extent_map_tree *tree, struct extent_map *em);
=======
void extent_map_tree_init(struct extent_map_tree *tree);
struct extent_map *lookup_extent_mapping(struct extent_map_tree *tree,
					 u64 start, u64 len);
void remove_extent_mapping(struct extent_map_tree *tree, struct extent_map *em);
int split_extent_map(struct btrfs_inode *inode, u64 start, u64 len, u64 pre,
		     u64 new_logical);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct extent_map *alloc_extent_map(void);
void free_extent_map(struct extent_map *em);
int __init extent_map_init(void);
<<<<<<< HEAD
void extent_map_exit(void);
int unpin_extent_cache(struct extent_map_tree *tree, u64 start, u64 len);
struct extent_map *search_extent_mapping(struct extent_map_tree *tree,
					 u64 start, u64 len);
=======
void __cold extent_map_exit(void);
int unpin_extent_cache(struct btrfs_inode *inode, u64 start, u64 len, u64 gen);
void clear_em_logging(struct extent_map_tree *tree, struct extent_map *em);
struct extent_map *search_extent_mapping(struct extent_map_tree *tree,
					 u64 start, u64 len);
int btrfs_add_extent_mapping(struct btrfs_fs_info *fs_info,
			     struct extent_map_tree *em_tree,
			     struct extent_map **em_in, u64 start, u64 len);
void btrfs_drop_extent_map_range(struct btrfs_inode *inode,
				 u64 start, u64 end,
				 bool skip_pinned);
int btrfs_replace_extent_map_range(struct btrfs_inode *inode,
				   struct extent_map *new_em,
				   bool modified);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
