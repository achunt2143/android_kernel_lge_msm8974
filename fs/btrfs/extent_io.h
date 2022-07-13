<<<<<<< HEAD
#ifndef __EXTENTIO__
#define __EXTENTIO__

#include <linux/rbtree.h>

/* bits for the extent state */
#define EXTENT_DIRTY 1
#define EXTENT_WRITEBACK (1 << 1)
#define EXTENT_UPTODATE (1 << 2)
#define EXTENT_LOCKED (1 << 3)
#define EXTENT_NEW (1 << 4)
#define EXTENT_DELALLOC (1 << 5)
#define EXTENT_DEFRAG (1 << 6)
#define EXTENT_DEFRAG_DONE (1 << 7)
#define EXTENT_BUFFER_FILLED (1 << 8)
#define EXTENT_BOUNDARY (1 << 9)
#define EXTENT_NODATASUM (1 << 10)
#define EXTENT_DO_ACCOUNTING (1 << 11)
#define EXTENT_FIRST_DELALLOC (1 << 12)
#define EXTENT_NEED_WAIT (1 << 13)
#define EXTENT_DAMAGED (1 << 14)
#define EXTENT_IOBITS (EXTENT_LOCKED | EXTENT_WRITEBACK)
#define EXTENT_CTLBITS (EXTENT_DO_ACCOUNTING | EXTENT_FIRST_DELALLOC)

/*
 * flags for bio submission. The high bits indicate the compression
 * type for this bio
 */
#define EXTENT_BIO_COMPRESSED 1
#define EXTENT_BIO_FLAG_SHIFT 16

/* these are bit numbers for test/set bit */
#define EXTENT_BUFFER_UPTODATE 0
#define EXTENT_BUFFER_BLOCKING 1
#define EXTENT_BUFFER_DIRTY 2
#define EXTENT_BUFFER_CORRUPT 3
#define EXTENT_BUFFER_READAHEAD 4	/* this got triggered by readahead */
#define EXTENT_BUFFER_TREE_REF 5
#define EXTENT_BUFFER_STALE 6
#define EXTENT_BUFFER_WRITEBACK 7
#define EXTENT_BUFFER_IOERR 8

/* these are flags for extent_clear_unlock_delalloc */
#define EXTENT_CLEAR_UNLOCK_PAGE 0x1
#define EXTENT_CLEAR_UNLOCK	 0x2
#define EXTENT_CLEAR_DELALLOC	 0x4
#define EXTENT_CLEAR_DIRTY	 0x8
#define EXTENT_SET_WRITEBACK	 0x10
#define EXTENT_END_WRITEBACK	 0x20
#define EXTENT_SET_PRIVATE2	 0x40
#define EXTENT_CLEAR_ACCOUNTING  0x80

/*
 * page->private values.  Every page that is controlled by the extent
 * map has page->private set to one.
 */
#define EXTENT_PAGE_PRIVATE 1
#define EXTENT_PAGE_PRIVATE_FIRST_PAGE 3

struct extent_state;
struct btrfs_root;

typedef	int (extent_submit_bio_hook_t)(struct inode *inode, int rw,
				       struct bio *bio, int mirror_num,
				       unsigned long bio_flags, u64 bio_offset);
struct extent_io_ops {
	int (*fill_delalloc)(struct inode *inode, struct page *locked_page,
			     u64 start, u64 end, int *page_started,
			     unsigned long *nr_written);
	int (*writepage_start_hook)(struct page *page, u64 start, u64 end);
	int (*writepage_io_hook)(struct page *page, u64 start, u64 end);
	extent_submit_bio_hook_t *submit_bio_hook;
	int (*merge_bio_hook)(struct page *page, unsigned long offset,
			      size_t size, struct bio *bio,
			      unsigned long bio_flags);
	int (*readpage_io_hook)(struct page *page, u64 start, u64 end);
	int (*readpage_io_failed_hook)(struct page *page, int failed_mirror);
	int (*writepage_io_failed_hook)(struct bio *bio, struct page *page,
					u64 start, u64 end,
				       struct extent_state *state);
	int (*readpage_end_io_hook)(struct page *page, u64 start, u64 end,
				    struct extent_state *state, int mirror);
	int (*writepage_end_io_hook)(struct page *page, u64 start, u64 end,
				      struct extent_state *state, int uptodate);
	void (*set_bit_hook)(struct inode *inode, struct extent_state *state,
			     int *bits);
	void (*clear_bit_hook)(struct inode *inode, struct extent_state *state,
			       int *bits);
	void (*merge_extent_hook)(struct inode *inode,
				  struct extent_state *new,
				  struct extent_state *other);
	void (*split_extent_hook)(struct inode *inode,
				  struct extent_state *orig, u64 split);
	int (*write_cache_pages_lock_hook)(struct page *page, void *data,
					   void (*flush_fn)(void *));
};

struct extent_io_tree {
	struct rb_root state;
	struct radix_tree_root buffer;
	struct address_space *mapping;
	u64 dirty_bytes;
	int track_uptodate;
	spinlock_t lock;
	spinlock_t buffer_lock;
	struct extent_io_ops *ops;
};

struct extent_state {
	u64 start;
	u64 end; /* inclusive */
	struct rb_node rb_node;

	/* ADD NEW ELEMENTS AFTER THIS */
	struct extent_io_tree *tree;
	wait_queue_head_t wq;
	atomic_t refs;
	unsigned long state;

	/* for use by the FS */
	u64 private;

	struct list_head leak_list;
};

#define INLINE_EXTENT_BUFFER_PAGES 16
#define MAX_INLINE_EXTENT_BUFFER_SIZE (INLINE_EXTENT_BUFFER_PAGES * PAGE_CACHE_SIZE)
struct extent_buffer {
	u64 start;
	unsigned long len;
	unsigned long map_start;
	unsigned long map_len;
	unsigned long bflags;
	struct extent_io_tree *tree;
	spinlock_t refs_lock;
	atomic_t refs;
	atomic_t io_pages;
	int read_mirror;
	struct list_head leak_list;
	struct rcu_head rcu_head;
	pid_t lock_owner;

	/* count of read lock holders on the extent buffer */
	atomic_t write_locks;
	atomic_t read_locks;
	atomic_t blocking_writers;
	atomic_t blocking_readers;
	atomic_t spinning_readers;
	atomic_t spinning_writers;
	int lock_nested;

	/* protects write locks */
	rwlock_t lock;

	/* readers use lock_wq while they wait for the write
	 * lock holders to unlock
	 */
	wait_queue_head_t write_lock_wq;

	/* writers use read_lock_wq while they wait for readers
	 * to unlock
	 */
	wait_queue_head_t read_lock_wq;
	wait_queue_head_t lock_wq;
	struct page *inline_pages[INLINE_EXTENT_BUFFER_PAGES];
	struct page **pages;
};

static inline void extent_set_compress_type(unsigned long *bio_flags,
					    int compress_type)
{
	*bio_flags |= compress_type << EXTENT_BIO_FLAG_SHIFT;
}

static inline int extent_compress_type(unsigned long bio_flags)
{
	return bio_flags >> EXTENT_BIO_FLAG_SHIFT;
}

struct extent_map_tree;

typedef struct extent_map *(get_extent_t)(struct inode *inode,
					  struct page *page,
					  size_t pg_offset,
					  u64 start, u64 len,
					  int create);

void extent_io_tree_init(struct extent_io_tree *tree,
			 struct address_space *mapping);
int try_release_extent_mapping(struct extent_map_tree *map,
			       struct extent_io_tree *tree, struct page *page,
			       gfp_t mask);
int try_release_extent_buffer(struct page *page, gfp_t mask);
int try_release_extent_state(struct extent_map_tree *map,
			     struct extent_io_tree *tree, struct page *page,
			     gfp_t mask);
int lock_extent(struct extent_io_tree *tree, u64 start, u64 end);
int lock_extent_bits(struct extent_io_tree *tree, u64 start, u64 end,
		     int bits, struct extent_state **cached);
int unlock_extent(struct extent_io_tree *tree, u64 start, u64 end);
int unlock_extent_cached(struct extent_io_tree *tree, u64 start, u64 end,
			 struct extent_state **cached, gfp_t mask);
int try_lock_extent(struct extent_io_tree *tree, u64 start, u64 end);
int extent_read_full_page(struct extent_io_tree *tree, struct page *page,
			  get_extent_t *get_extent, int mirror_num);
int __init extent_io_init(void);
void extent_io_exit(void);

u64 count_range_bits(struct extent_io_tree *tree,
		     u64 *start, u64 search_end,
		     u64 max_bytes, unsigned long bits, int contig);

void free_extent_state(struct extent_state *state);
int test_range_bit(struct extent_io_tree *tree, u64 start, u64 end,
		   int bits, int filled, struct extent_state *cached_state);
int clear_extent_bits(struct extent_io_tree *tree, u64 start, u64 end,
		      int bits, gfp_t mask);
int clear_extent_bit(struct extent_io_tree *tree, u64 start, u64 end,
		     int bits, int wake, int delete, struct extent_state **cached,
		     gfp_t mask);
int set_extent_bits(struct extent_io_tree *tree, u64 start, u64 end,
		    int bits, gfp_t mask);
int set_extent_bit(struct extent_io_tree *tree, u64 start, u64 end,
		   int bits, u64 *failed_start,
		   struct extent_state **cached_state, gfp_t mask);
int set_extent_uptodate(struct extent_io_tree *tree, u64 start, u64 end,
			struct extent_state **cached_state, gfp_t mask);
int set_extent_new(struct extent_io_tree *tree, u64 start, u64 end,
		   gfp_t mask);
int set_extent_dirty(struct extent_io_tree *tree, u64 start, u64 end,
		     gfp_t mask);
int clear_extent_dirty(struct extent_io_tree *tree, u64 start, u64 end,
		       gfp_t mask);
int convert_extent_bit(struct extent_io_tree *tree, u64 start, u64 end,
		       int bits, int clear_bits, gfp_t mask);
int set_extent_delalloc(struct extent_io_tree *tree, u64 start, u64 end,
			struct extent_state **cached_state, gfp_t mask);
int find_first_extent_bit(struct extent_io_tree *tree, u64 start,
			  u64 *start_ret, u64 *end_ret, int bits);
struct extent_state *find_first_extent_bit_state(struct extent_io_tree *tree,
						 u64 start, int bits);
int extent_invalidatepage(struct extent_io_tree *tree,
			  struct page *page, unsigned long offset);
int extent_write_full_page(struct extent_io_tree *tree, struct page *page,
			  get_extent_t *get_extent,
			  struct writeback_control *wbc);
int extent_write_locked_range(struct extent_io_tree *tree, struct inode *inode,
			      u64 start, u64 end, get_extent_t *get_extent,
			      int mode);
int extent_writepages(struct extent_io_tree *tree,
		      struct address_space *mapping,
		      get_extent_t *get_extent,
		      struct writeback_control *wbc);
int btree_write_cache_pages(struct address_space *mapping,
			    struct writeback_control *wbc);
int extent_readpages(struct extent_io_tree *tree,
		     struct address_space *mapping,
		     struct list_head *pages, unsigned nr_pages,
		     get_extent_t get_extent);
int extent_fiemap(struct inode *inode, struct fiemap_extent_info *fieinfo,
		__u64 start, __u64 len, get_extent_t *get_extent);
int set_state_private(struct extent_io_tree *tree, u64 start, u64 private);
int get_state_private(struct extent_io_tree *tree, u64 start, u64 *private);
void set_page_extent_mapped(struct page *page);

struct extent_buffer *alloc_extent_buffer(struct extent_io_tree *tree,
					  u64 start, unsigned long len);
struct extent_buffer *find_extent_buffer(struct extent_io_tree *tree,
					 u64 start, unsigned long len);
=======
/* SPDX-License-Identifier: GPL-2.0 */

#ifndef BTRFS_EXTENT_IO_H
#define BTRFS_EXTENT_IO_H

#include <linux/rbtree.h>
#include <linux/refcount.h>
#include <linux/fiemap.h>
#include <linux/btrfs_tree.h>
#include <linux/spinlock.h>
#include <linux/atomic.h>
#include <linux/rwsem.h>
#include <linux/list.h>
#include <linux/slab.h>
#include "compression.h"
#include "messages.h"
#include "ulist.h"
#include "misc.h"

struct page;
struct file;
struct folio;
struct inode;
struct fiemap_extent_info;
struct readahead_control;
struct address_space;
struct writeback_control;
struct extent_io_tree;
struct extent_map_tree;
struct btrfs_block_group;
struct btrfs_fs_info;
struct btrfs_inode;
struct btrfs_root;
struct btrfs_trans_handle;
struct btrfs_tree_parent_check;

enum {
	EXTENT_BUFFER_UPTODATE,
	EXTENT_BUFFER_DIRTY,
	EXTENT_BUFFER_CORRUPT,
	/* this got triggered by readahead */
	EXTENT_BUFFER_READAHEAD,
	EXTENT_BUFFER_TREE_REF,
	EXTENT_BUFFER_STALE,
	EXTENT_BUFFER_WRITEBACK,
	/* read IO error */
	EXTENT_BUFFER_READ_ERR,
	EXTENT_BUFFER_UNMAPPED,
	EXTENT_BUFFER_IN_TREE,
	/* write IO error */
	EXTENT_BUFFER_WRITE_ERR,
	/* Indicate the extent buffer is written zeroed out (for zoned) */
	EXTENT_BUFFER_ZONED_ZEROOUT,
	/* Indicate that extent buffer pages a being read */
	EXTENT_BUFFER_READING,
};

/* these are flags for __process_pages_contig */
enum {
	ENUM_BIT(PAGE_UNLOCK),
	/* Page starts writeback, clear dirty bit and set writeback bit */
	ENUM_BIT(PAGE_START_WRITEBACK),
	ENUM_BIT(PAGE_END_WRITEBACK),
	ENUM_BIT(PAGE_SET_ORDERED),
};

/*
 * Folio private values.  Every page that is controlled by the extent map has
 * folio private set to this value.
 */
#define EXTENT_FOLIO_PRIVATE			1

/*
 * The extent buffer bitmap operations are done with byte granularity instead of
 * word granularity for two reasons:
 * 1. The bitmaps must be little-endian on disk.
 * 2. Bitmap items are not guaranteed to be aligned to a word and therefore a
 *    single word in a bitmap may straddle two pages in the extent buffer.
 */
#define BIT_BYTE(nr) ((nr) / BITS_PER_BYTE)
#define BYTE_MASK ((1 << BITS_PER_BYTE) - 1)
#define BITMAP_FIRST_BYTE_MASK(start) \
	((BYTE_MASK << ((start) & (BITS_PER_BYTE - 1))) & BYTE_MASK)
#define BITMAP_LAST_BYTE_MASK(nbits) \
	(BYTE_MASK >> (-(nbits) & (BITS_PER_BYTE - 1)))


int __init extent_buffer_init_cachep(void);
void __cold extent_buffer_free_cachep(void);

#define INLINE_EXTENT_BUFFER_PAGES     (BTRFS_MAX_METADATA_BLOCKSIZE / PAGE_SIZE)
struct extent_buffer {
	u64 start;
	u32 len;
	u32 folio_size;
	unsigned long bflags;
	struct btrfs_fs_info *fs_info;

	/*
	 * The address where the eb can be accessed without any cross-page handling.
	 * This can be NULL if not possible.
	 */
	void *addr;

	spinlock_t refs_lock;
	atomic_t refs;
	int read_mirror;
	/* >= 0 if eb belongs to a log tree, -1 otherwise */
	s8 log_index;
	u8 folio_shift;
	struct rcu_head rcu_head;

	struct rw_semaphore lock;

	/*
	 * Pointers to all the folios of the extent buffer.
	 *
	 * For now the folio is always order 0 (aka, a single page).
	 */
	struct folio *folios[INLINE_EXTENT_BUFFER_PAGES];
#ifdef CONFIG_BTRFS_DEBUG
	struct list_head leak_list;
	pid_t lock_owner;
#endif
};

struct btrfs_eb_write_context {
	struct writeback_control *wbc;
	struct extent_buffer *eb;
	/* Block group @eb resides in. Only used for zoned mode. */
	struct btrfs_block_group *zoned_bg;
};

static inline unsigned long offset_in_eb_folio(const struct extent_buffer *eb,
					       u64 start)
{
	ASSERT(eb->folio_size);
	return start & (eb->folio_size - 1);
}

/*
 * Get the correct offset inside the page of extent buffer.
 *
 * @eb:		target extent buffer
 * @start:	offset inside the extent buffer
 *
 * Will handle both sectorsize == PAGE_SIZE and sectorsize < PAGE_SIZE cases.
 */
static inline size_t get_eb_offset_in_folio(const struct extent_buffer *eb,
					    unsigned long offset)
{
	/*
	 * 1) sectorsize == PAGE_SIZE and nodesize >= PAGE_SIZE case
	 *    1.1) One large folio covering the whole eb
	 *	   The eb->start is aligned to folio size, thus adding it
	 *	   won't cause any difference.
	 *    1.2) Several page sized folios
	 *	   The eb->start is aligned to folio (page) size, thus
	 *	   adding it won't cause any difference.
	 *
	 * 2) sectorsize < PAGE_SIZE and nodesize < PAGE_SIZE case
	 *    In this case there would only be one page sized folio, and there
	 *    may be several different extent buffers in the page/folio.
	 *    We need to add eb->start to properly access the offset inside
	 *    that eb.
	 */
	return offset_in_folio(eb->folios[0], offset + eb->start);
}

static inline unsigned long get_eb_folio_index(const struct extent_buffer *eb,
					       unsigned long offset)
{
	/*
	 * 1) sectorsize == PAGE_SIZE and nodesize >= PAGE_SIZE case
	 *    1.1) One large folio covering the whole eb.
	 *	   the folio_shift would be large enough to always make us
	 *	   return 0 as index.
	 *    1.2) Several page sized folios
	 *         The folio_shift would be PAGE_SHIFT, giving us the correct
	 *         index.
	 *
	 * 2) sectorsize < PAGE_SIZE and nodesize < PAGE_SIZE case
	 *    The folio would only be page sized, and always give us 0 as index.
	 */
	return offset >> eb->folio_shift;
}

/*
 * Structure to record how many bytes and which ranges are set/cleared
 */
struct extent_changeset {
	/* How many bytes are set/cleared in this operation */
	u64 bytes_changed;

	/* Changed ranges */
	struct ulist range_changed;
};

static inline void extent_changeset_init(struct extent_changeset *changeset)
{
	changeset->bytes_changed = 0;
	ulist_init(&changeset->range_changed);
}

static inline struct extent_changeset *extent_changeset_alloc(void)
{
	struct extent_changeset *ret;

	ret = kmalloc(sizeof(*ret), GFP_KERNEL);
	if (!ret)
		return NULL;

	extent_changeset_init(ret);
	return ret;
}

static inline void extent_changeset_release(struct extent_changeset *changeset)
{
	if (!changeset)
		return;
	changeset->bytes_changed = 0;
	ulist_release(&changeset->range_changed);
}

static inline void extent_changeset_free(struct extent_changeset *changeset)
{
	if (!changeset)
		return;
	extent_changeset_release(changeset);
	kfree(changeset);
}

int try_release_extent_mapping(struct page *page, gfp_t mask);
int try_release_extent_buffer(struct page *page);

int btrfs_read_folio(struct file *file, struct folio *folio);
void extent_write_locked_range(struct inode *inode, struct page *locked_page,
			       u64 start, u64 end, struct writeback_control *wbc,
			       bool pages_dirty);
int extent_writepages(struct address_space *mapping,
		      struct writeback_control *wbc);
int btree_write_cache_pages(struct address_space *mapping,
			    struct writeback_control *wbc);
void extent_readahead(struct readahead_control *rac);
int extent_fiemap(struct btrfs_inode *inode, struct fiemap_extent_info *fieinfo,
		  u64 start, u64 len);
int set_folio_extent_mapped(struct folio *folio);
int set_page_extent_mapped(struct page *page);
void clear_page_extent_mapped(struct page *page);

struct extent_buffer *alloc_extent_buffer(struct btrfs_fs_info *fs_info,
					  u64 start, u64 owner_root, int level);
struct extent_buffer *__alloc_dummy_extent_buffer(struct btrfs_fs_info *fs_info,
						  u64 start, unsigned long len);
struct extent_buffer *alloc_dummy_extent_buffer(struct btrfs_fs_info *fs_info,
						u64 start);
struct extent_buffer *btrfs_clone_extent_buffer(const struct extent_buffer *src);
struct extent_buffer *find_extent_buffer(struct btrfs_fs_info *fs_info,
					 u64 start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void free_extent_buffer(struct extent_buffer *eb);
void free_extent_buffer_stale(struct extent_buffer *eb);
#define WAIT_NONE	0
#define WAIT_COMPLETE	1
#define WAIT_PAGE_LOCK	2
<<<<<<< HEAD
int read_extent_buffer_pages(struct extent_io_tree *tree,
			     struct extent_buffer *eb, u64 start, int wait,
			     get_extent_t *get_extent, int mirror_num);
unsigned long num_extent_pages(u64 start, u64 len);
struct page *extent_buffer_page(struct extent_buffer *eb, unsigned long i);

static inline void extent_buffer_get(struct extent_buffer *eb)
{
	atomic_inc(&eb->refs);
}

int memcmp_extent_buffer(struct extent_buffer *eb, const void *ptrv,
			  unsigned long start,
			  unsigned long len);
void read_extent_buffer(struct extent_buffer *eb, void *dst,
			unsigned long start,
			unsigned long len);
void write_extent_buffer(struct extent_buffer *eb, const void *src,
			 unsigned long start, unsigned long len);
void copy_extent_buffer(struct extent_buffer *dst, struct extent_buffer *src,
			unsigned long dst_offset, unsigned long src_offset,
			unsigned long len);
void memcpy_extent_buffer(struct extent_buffer *dst, unsigned long dst_offset,
			   unsigned long src_offset, unsigned long len);
void memmove_extent_buffer(struct extent_buffer *dst, unsigned long dst_offset,
			   unsigned long src_offset, unsigned long len);
void memset_extent_buffer(struct extent_buffer *eb, char c,
			  unsigned long start, unsigned long len);
void wait_extent_bit(struct extent_io_tree *tree, u64 start, u64 end, int bits);
void clear_extent_buffer_dirty(struct extent_buffer *eb);
int set_extent_buffer_dirty(struct extent_buffer *eb);
int set_extent_buffer_uptodate(struct extent_buffer *eb);
int clear_extent_buffer_uptodate(struct extent_buffer *eb);
int extent_buffer_uptodate(struct extent_buffer *eb);
int map_private_extent_buffer(struct extent_buffer *eb, unsigned long offset,
		      unsigned long min_len, char **map,
		      unsigned long *map_start,
		      unsigned long *map_len);
int extent_range_uptodate(struct extent_io_tree *tree,
			  u64 start, u64 end);
int extent_range_clear_dirty_for_io(struct inode *inode, u64 start, u64 end);
int extent_range_redirty_for_io(struct inode *inode, u64 start, u64 end);
int extent_clear_unlock_delalloc(struct inode *inode,
				struct extent_io_tree *tree,
				u64 start, u64 end, struct page *locked_page,
				unsigned long op);
struct bio *
btrfs_bio_alloc(struct block_device *bdev, u64 first_sector, int nr_vecs,
		gfp_t gfp_flags);

struct btrfs_mapping_tree;

int repair_io_failure(struct btrfs_mapping_tree *map_tree, u64 start,
			u64 length, u64 logical, struct page *page,
			int mirror_num);
int end_extent_writepage(struct page *page, int err, u64 start, u64 end);
int repair_eb_io_failure(struct btrfs_root *root, struct extent_buffer *eb,
			 int mirror_num);
=======
int read_extent_buffer_pages(struct extent_buffer *eb, int wait, int mirror_num,
			     struct btrfs_tree_parent_check *parent_check);
void wait_on_extent_buffer_writeback(struct extent_buffer *eb);
void btrfs_readahead_tree_block(struct btrfs_fs_info *fs_info,
				u64 bytenr, u64 owner_root, u64 gen, int level);
void btrfs_readahead_node_child(struct extent_buffer *node, int slot);

static inline int num_extent_pages(const struct extent_buffer *eb)
{
	/*
	 * For sectorsize == PAGE_SIZE case, since nodesize is always aligned to
	 * sectorsize, it's just eb->len >> PAGE_SHIFT.
	 *
	 * For sectorsize < PAGE_SIZE case, we could have nodesize < PAGE_SIZE,
	 * thus have to ensure we get at least one page.
	 */
	return (eb->len >> PAGE_SHIFT) ?: 1;
}

/*
 * This can only be determined at runtime by checking eb::folios[0].
 *
 * As we can have either one large folio covering the whole eb
 * (either nodesize <= PAGE_SIZE, or high order folio), or multiple
 * single-paged folios.
 */
static inline int num_extent_folios(const struct extent_buffer *eb)
{
	if (folio_order(eb->folios[0]))
		return 1;
	return num_extent_pages(eb);
}

static inline int extent_buffer_uptodate(const struct extent_buffer *eb)
{
	return test_bit(EXTENT_BUFFER_UPTODATE, &eb->bflags);
}

int memcmp_extent_buffer(const struct extent_buffer *eb, const void *ptrv,
			 unsigned long start, unsigned long len);
void read_extent_buffer(const struct extent_buffer *eb, void *dst,
			unsigned long start,
			unsigned long len);
int read_extent_buffer_to_user_nofault(const struct extent_buffer *eb,
				       void __user *dst, unsigned long start,
				       unsigned long len);
void write_extent_buffer(const struct extent_buffer *eb, const void *src,
			 unsigned long start, unsigned long len);

static inline void write_extent_buffer_chunk_tree_uuid(
		const struct extent_buffer *eb, const void *chunk_tree_uuid)
{
	write_extent_buffer(eb, chunk_tree_uuid,
			    offsetof(struct btrfs_header, chunk_tree_uuid),
			    BTRFS_FSID_SIZE);
}

static inline void write_extent_buffer_fsid(const struct extent_buffer *eb,
					    const void *fsid)
{
	write_extent_buffer(eb, fsid, offsetof(struct btrfs_header, fsid),
			    BTRFS_FSID_SIZE);
}

void copy_extent_buffer_full(const struct extent_buffer *dst,
			     const struct extent_buffer *src);
void copy_extent_buffer(const struct extent_buffer *dst,
			const struct extent_buffer *src,
			unsigned long dst_offset, unsigned long src_offset,
			unsigned long len);
void memcpy_extent_buffer(const struct extent_buffer *dst,
			  unsigned long dst_offset, unsigned long src_offset,
			  unsigned long len);
void memmove_extent_buffer(const struct extent_buffer *dst,
			   unsigned long dst_offset, unsigned long src_offset,
			   unsigned long len);
void memzero_extent_buffer(const struct extent_buffer *eb, unsigned long start,
			   unsigned long len);
int extent_buffer_test_bit(const struct extent_buffer *eb, unsigned long start,
			   unsigned long pos);
void extent_buffer_bitmap_set(const struct extent_buffer *eb, unsigned long start,
			      unsigned long pos, unsigned long len);
void extent_buffer_bitmap_clear(const struct extent_buffer *eb,
				unsigned long start, unsigned long pos,
				unsigned long len);
void set_extent_buffer_dirty(struct extent_buffer *eb);
void set_extent_buffer_uptodate(struct extent_buffer *eb);
void clear_extent_buffer_uptodate(struct extent_buffer *eb);
void extent_range_clear_dirty_for_io(struct inode *inode, u64 start, u64 end);
void extent_clear_unlock_delalloc(struct btrfs_inode *inode, u64 start, u64 end,
				  struct page *locked_page,
				  u32 bits_to_clear, unsigned long page_ops);
int extent_invalidate_folio(struct extent_io_tree *tree,
			    struct folio *folio, size_t offset);
void btrfs_clear_buffer_dirty(struct btrfs_trans_handle *trans,
			      struct extent_buffer *buf);

int btrfs_alloc_page_array(unsigned int nr_pages, struct page **page_array,
			   gfp_t extra_gfp);

#ifdef CONFIG_BTRFS_FS_RUN_SANITY_TESTS
bool find_lock_delalloc_range(struct inode *inode,
			     struct page *locked_page, u64 *start,
			     u64 *end);
#endif
struct extent_buffer *alloc_test_extent_buffer(struct btrfs_fs_info *fs_info,
					       u64 start);

#ifdef CONFIG_BTRFS_DEBUG
void btrfs_extent_buffer_leak_debug_check(struct btrfs_fs_info *fs_info);
#else
#define btrfs_extent_buffer_leak_debug_check(fs_info)	do {} while (0)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
