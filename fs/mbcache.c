<<<<<<< HEAD
/*
 * linux/fs/mbcache.c
 * (C) 2001-2002 Andreas Gruenbacher, <a.gruenbacher@computer.org>
 */

/*
 * Filesystem Meta Information Block Cache (mbcache)
 *
 * The mbcache caches blocks of block devices that need to be located
 * by their device/block number, as well as by other criteria (such
 * as the block's contents).
 *
 * There can only be one cache entry in a cache per device and block number.
 * Additional indexes need not be unique in this sense. The number of
 * additional indexes (=other criteria) can be hardwired at compile time
 * or specified at cache create time.
 *
 * Each cache entry is of fixed size. An entry may be `valid' or `invalid'
 * in the cache. A valid entry is in the main hash tables of the cache,
 * and may also be in the lru list. An invalid entry is not in any hashes
 * or lists.
 *
 * A valid cache entry is only in the lru list if no handles refer to it.
 * Invalid cache entries will be freed when the last handle to the cache
 * entry is released. Entries that cannot be freed immediately are put
 * back on the lru list.
 */

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/hash.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/mbcache.h>


#ifdef MB_CACHE_DEBUG
# define mb_debug(f...) do { \
		printk(KERN_DEBUG f); \
		printk("\n"); \
	} while (0)
#define mb_assert(c) do { if (!(c)) \
		printk(KERN_ERR "assertion " #c " failed\n"); \
	} while(0)
#else
# define mb_debug(f...) do { } while(0)
# define mb_assert(c) do { } while(0)
#endif
#define mb_error(f...) do { \
		printk(KERN_ERR f); \
		printk("\n"); \
	} while(0)

#define MB_CACHE_WRITER ((unsigned short)~0U >> 1)

static DECLARE_WAIT_QUEUE_HEAD(mb_cache_queue);
		
MODULE_AUTHOR("Andreas Gruenbacher <a.gruenbacher@computer.org>");
MODULE_DESCRIPTION("Meta block cache (for extended attributes)");
MODULE_LICENSE("GPL");

EXPORT_SYMBOL(mb_cache_create);
EXPORT_SYMBOL(mb_cache_shrink);
EXPORT_SYMBOL(mb_cache_destroy);
EXPORT_SYMBOL(mb_cache_entry_alloc);
EXPORT_SYMBOL(mb_cache_entry_insert);
EXPORT_SYMBOL(mb_cache_entry_release);
EXPORT_SYMBOL(mb_cache_entry_free);
EXPORT_SYMBOL(mb_cache_entry_get);
#if !defined(MB_CACHE_INDEXES_COUNT) || (MB_CACHE_INDEXES_COUNT > 0)
EXPORT_SYMBOL(mb_cache_entry_find_first);
EXPORT_SYMBOL(mb_cache_entry_find_next);
#endif

/*
 * Global data: list of all mbcache's, lru list, and a spinlock for
 * accessing cache data structures on SMP machines. The lru list is
 * global across all mbcaches.
 */

static LIST_HEAD(mb_cache_list);
static LIST_HEAD(mb_cache_lru_list);
static DEFINE_SPINLOCK(mb_cache_spinlock);

/*
 * What the mbcache registers as to get shrunk dynamically.
 */

static int mb_cache_shrink_fn(struct shrinker *shrink,
			      struct shrink_control *sc);

static struct shrinker mb_cache_shrinker = {
	.shrink = mb_cache_shrink_fn,
	.seeks = DEFAULT_SEEKS,
};

static inline int
__mb_cache_entry_is_hashed(struct mb_cache_entry *ce)
{
	return !list_empty(&ce->e_block_list);
}


static void
__mb_cache_entry_unhash(struct mb_cache_entry *ce)
{
	if (__mb_cache_entry_is_hashed(ce)) {
		list_del_init(&ce->e_block_list);
		list_del(&ce->e_index.o_list);
	}
}


static void
__mb_cache_entry_forget(struct mb_cache_entry *ce, gfp_t gfp_mask)
{
	struct mb_cache *cache = ce->e_cache;

	mb_assert(!(ce->e_used || ce->e_queued));
	kmem_cache_free(cache->c_entry_cache, ce);
	atomic_dec(&cache->c_entry_count);
}


static void
__mb_cache_entry_release_unlock(struct mb_cache_entry *ce)
	__releases(mb_cache_spinlock)
{
	/* Wake up all processes queuing for this cache entry. */
	if (ce->e_queued)
		wake_up_all(&mb_cache_queue);
	if (ce->e_used >= MB_CACHE_WRITER)
		ce->e_used -= MB_CACHE_WRITER;
	ce->e_used--;
	if (!(ce->e_used || ce->e_queued)) {
		if (!__mb_cache_entry_is_hashed(ce))
			goto forget;
		mb_assert(list_empty(&ce->e_lru_list));
		list_add_tail(&ce->e_lru_list, &mb_cache_lru_list);
	}
	spin_unlock(&mb_cache_spinlock);
	return;
forget:
	spin_unlock(&mb_cache_spinlock);
	__mb_cache_entry_forget(ce, GFP_KERNEL);
}


/*
 * mb_cache_shrink_fn()  memory pressure callback
 *
 * This function is called by the kernel memory management when memory
 * gets low.
 *
 * @shrink: (ignored)
 * @sc: shrink_control passed from reclaim
 *
 * Returns the number of objects which are present in the cache.
 */
static int
mb_cache_shrink_fn(struct shrinker *shrink, struct shrink_control *sc)
{
	LIST_HEAD(free_list);
	struct mb_cache *cache;
	struct mb_cache_entry *entry, *tmp;
	int count = 0;
	int nr_to_scan = sc->nr_to_scan;
	gfp_t gfp_mask = sc->gfp_mask;

	mb_debug("trying to free %d entries", nr_to_scan);
	spin_lock(&mb_cache_spinlock);
	while (nr_to_scan-- && !list_empty(&mb_cache_lru_list)) {
		struct mb_cache_entry *ce =
			list_entry(mb_cache_lru_list.next,
				   struct mb_cache_entry, e_lru_list);
		list_move_tail(&ce->e_lru_list, &free_list);
		__mb_cache_entry_unhash(ce);
	}
	list_for_each_entry(cache, &mb_cache_list, c_cache_list) {
		mb_debug("cache %s (%d)", cache->c_name,
			  atomic_read(&cache->c_entry_count));
		count += atomic_read(&cache->c_entry_count);
	}
	spin_unlock(&mb_cache_spinlock);
	list_for_each_entry_safe(entry, tmp, &free_list, e_lru_list) {
		__mb_cache_entry_forget(entry, gfp_mask);
	}
	return (count / 100) * sysctl_vfs_cache_pressure;
}


/*
 * mb_cache_create()  create a new cache
 *
 * All entries in one cache are equal size. Cache entries may be from
 * multiple devices. If this is the first mbcache created, registers
 * the cache with kernel memory management. Returns NULL if no more
 * memory was available.
 *
 * @name: name of the cache (informal)
 * @bucket_bits: log2(number of hash buckets)
 */
struct mb_cache *
mb_cache_create(const char *name, int bucket_bits)
{
	int n, bucket_count = 1 << bucket_bits;
	struct mb_cache *cache = NULL;

	cache = kmalloc(sizeof(struct mb_cache), GFP_KERNEL);
	if (!cache)
		return NULL;
	cache->c_name = name;
	atomic_set(&cache->c_entry_count, 0);
	cache->c_bucket_bits = bucket_bits;
	cache->c_block_hash = kmalloc(bucket_count * sizeof(struct list_head),
	                              GFP_KERNEL);
	if (!cache->c_block_hash)
		goto fail;
	for (n=0; n<bucket_count; n++)
		INIT_LIST_HEAD(&cache->c_block_hash[n]);
	cache->c_index_hash = kmalloc(bucket_count * sizeof(struct list_head),
				      GFP_KERNEL);
	if (!cache->c_index_hash)
		goto fail;
	for (n=0; n<bucket_count; n++)
		INIT_LIST_HEAD(&cache->c_index_hash[n]);
	cache->c_entry_cache = kmem_cache_create(name,
		sizeof(struct mb_cache_entry), 0,
		SLAB_RECLAIM_ACCOUNT|SLAB_MEM_SPREAD, NULL);
	if (!cache->c_entry_cache)
		goto fail2;

	/*
	 * Set an upper limit on the number of cache entries so that the hash
	 * chains won't grow too long.
	 */
	cache->c_max_entries = bucket_count << 4;

	spin_lock(&mb_cache_spinlock);
	list_add(&cache->c_cache_list, &mb_cache_list);
	spin_unlock(&mb_cache_spinlock);
	return cache;

fail2:
	kfree(cache->c_index_hash);

fail:
	kfree(cache->c_block_hash);
	kfree(cache);
	return NULL;
}


/*
 * mb_cache_shrink()
 *
 * Removes all cache entries of a device from the cache. All cache entries
 * currently in use cannot be freed, and thus remain in the cache. All others
 * are freed.
 *
 * @bdev: which device's cache entries to shrink
 */
void
mb_cache_shrink(struct block_device *bdev)
{
	LIST_HEAD(free_list);
	struct list_head *l, *ltmp;

	spin_lock(&mb_cache_spinlock);
	list_for_each_safe(l, ltmp, &mb_cache_lru_list) {
		struct mb_cache_entry *ce =
			list_entry(l, struct mb_cache_entry, e_lru_list);
		if (ce->e_bdev == bdev) {
			list_move_tail(&ce->e_lru_list, &free_list);
			__mb_cache_entry_unhash(ce);
		}
	}
	spin_unlock(&mb_cache_spinlock);
	list_for_each_safe(l, ltmp, &free_list) {
		__mb_cache_entry_forget(list_entry(l, struct mb_cache_entry,
						   e_lru_list), GFP_KERNEL);
	}
}


/*
 * mb_cache_destroy()
 *
 * Shrinks the cache to its minimum possible size (hopefully 0 entries),
 * and then destroys it. If this was the last mbcache, un-registers the
 * mbcache from kernel memory management.
 */
void
mb_cache_destroy(struct mb_cache *cache)
{
	LIST_HEAD(free_list);
	struct list_head *l, *ltmp;

	spin_lock(&mb_cache_spinlock);
	list_for_each_safe(l, ltmp, &mb_cache_lru_list) {
		struct mb_cache_entry *ce =
			list_entry(l, struct mb_cache_entry, e_lru_list);
		if (ce->e_cache == cache) {
			list_move_tail(&ce->e_lru_list, &free_list);
			__mb_cache_entry_unhash(ce);
		}
	}
	list_del(&cache->c_cache_list);
	spin_unlock(&mb_cache_spinlock);

	list_for_each_safe(l, ltmp, &free_list) {
		__mb_cache_entry_forget(list_entry(l, struct mb_cache_entry,
						   e_lru_list), GFP_KERNEL);
	}

	if (atomic_read(&cache->c_entry_count) > 0) {
		mb_error("cache %s: %d orphaned entries",
			  cache->c_name,
			  atomic_read(&cache->c_entry_count));
	}

	kmem_cache_destroy(cache->c_entry_cache);

	kfree(cache->c_index_hash);
	kfree(cache->c_block_hash);
	kfree(cache);
}

/*
 * mb_cache_entry_alloc()
 *
 * Allocates a new cache entry. The new entry will not be valid initially,
 * and thus cannot be looked up yet. It should be filled with data, and
 * then inserted into the cache using mb_cache_entry_insert(). Returns NULL
 * if no more memory was available.
 */
struct mb_cache_entry *
mb_cache_entry_alloc(struct mb_cache *cache, gfp_t gfp_flags)
{
	struct mb_cache_entry *ce = NULL;

	if (atomic_read(&cache->c_entry_count) >= cache->c_max_entries) {
		spin_lock(&mb_cache_spinlock);
		if (!list_empty(&mb_cache_lru_list)) {
			ce = list_entry(mb_cache_lru_list.next,
					struct mb_cache_entry, e_lru_list);
			list_del_init(&ce->e_lru_list);
			__mb_cache_entry_unhash(ce);
		}
		spin_unlock(&mb_cache_spinlock);
	}
	if (!ce) {
		ce = kmem_cache_alloc(cache->c_entry_cache, gfp_flags);
		if (!ce)
			return NULL;
		atomic_inc(&cache->c_entry_count);
		INIT_LIST_HEAD(&ce->e_lru_list);
		INIT_LIST_HEAD(&ce->e_block_list);
		ce->e_cache = cache;
		ce->e_queued = 0;
	}
	ce->e_used = 1 + MB_CACHE_WRITER;
	return ce;
}


/*
 * mb_cache_entry_insert()
 *
 * Inserts an entry that was allocated using mb_cache_entry_alloc() into
 * the cache. After this, the cache entry can be looked up, but is not yet
 * in the lru list as the caller still holds a handle to it. Returns 0 on
 * success, or -EBUSY if a cache entry for that device + inode exists
 * already (this may happen after a failed lookup, but when another process
 * has inserted the same cache entry in the meantime).
 *
 * @bdev: device the cache entry belongs to
 * @block: block number
 * @key: lookup key
 */
int
mb_cache_entry_insert(struct mb_cache_entry *ce, struct block_device *bdev,
		      sector_t block, unsigned int key)
{
	struct mb_cache *cache = ce->e_cache;
	unsigned int bucket;
	struct list_head *l;
	int error = -EBUSY;

	bucket = hash_long((unsigned long)bdev + (block & 0xffffffff), 
			   cache->c_bucket_bits);
	spin_lock(&mb_cache_spinlock);
	list_for_each_prev(l, &cache->c_block_hash[bucket]) {
		struct mb_cache_entry *ce =
			list_entry(l, struct mb_cache_entry, e_block_list);
		if (ce->e_bdev == bdev && ce->e_block == block)
			goto out;
	}
	__mb_cache_entry_unhash(ce);
	ce->e_bdev = bdev;
	ce->e_block = block;
	list_add(&ce->e_block_list, &cache->c_block_hash[bucket]);
	ce->e_index.o_key = key;
	bucket = hash_long(key, cache->c_bucket_bits);
	list_add(&ce->e_index.o_list, &cache->c_index_hash[bucket]);
	error = 0;
out:
	spin_unlock(&mb_cache_spinlock);
	return error;
}


/*
 * mb_cache_entry_release()
 *
 * Release a handle to a cache entry. When the last handle to a cache entry
 * is released it is either freed (if it is invalid) or otherwise inserted
 * in to the lru list.
 */
void
mb_cache_entry_release(struct mb_cache_entry *ce)
{
	spin_lock(&mb_cache_spinlock);
	__mb_cache_entry_release_unlock(ce);
}


/*
 * mb_cache_entry_free()
 *
 * This is equivalent to the sequence mb_cache_entry_takeout() --
 * mb_cache_entry_release().
 */
void
mb_cache_entry_free(struct mb_cache_entry *ce)
{
	spin_lock(&mb_cache_spinlock);
	mb_assert(list_empty(&ce->e_lru_list));
	__mb_cache_entry_unhash(ce);
	__mb_cache_entry_release_unlock(ce);
}


/*
 * mb_cache_entry_get()
 *
 * Get a cache entry  by device / block number. (There can only be one entry
 * in the cache per device and block.) Returns NULL if no such cache entry
 * exists. The returned cache entry is locked for exclusive access ("single
 * writer").
 */
struct mb_cache_entry *
mb_cache_entry_get(struct mb_cache *cache, struct block_device *bdev,
		   sector_t block)
{
	unsigned int bucket;
	struct list_head *l;
	struct mb_cache_entry *ce;

	bucket = hash_long((unsigned long)bdev + (block & 0xffffffff),
			   cache->c_bucket_bits);
	spin_lock(&mb_cache_spinlock);
	list_for_each(l, &cache->c_block_hash[bucket]) {
		ce = list_entry(l, struct mb_cache_entry, e_block_list);
		if (ce->e_bdev == bdev && ce->e_block == block) {
			DEFINE_WAIT(wait);

			if (!list_empty(&ce->e_lru_list))
				list_del_init(&ce->e_lru_list);

			while (ce->e_used > 0) {
				ce->e_queued++;
				prepare_to_wait(&mb_cache_queue, &wait,
						TASK_UNINTERRUPTIBLE);
				spin_unlock(&mb_cache_spinlock);
				schedule();
				spin_lock(&mb_cache_spinlock);
				ce->e_queued--;
			}
			finish_wait(&mb_cache_queue, &wait);
			ce->e_used += 1 + MB_CACHE_WRITER;

			if (!__mb_cache_entry_is_hashed(ce)) {
				__mb_cache_entry_release_unlock(ce);
				return NULL;
			}
			goto cleanup;
		}
	}
	ce = NULL;

cleanup:
	spin_unlock(&mb_cache_spinlock);
	return ce;
}

#if !defined(MB_CACHE_INDEXES_COUNT) || (MB_CACHE_INDEXES_COUNT > 0)

static struct mb_cache_entry *
__mb_cache_entry_find(struct list_head *l, struct list_head *head,
		      struct block_device *bdev, unsigned int key)
{
	while (l != head) {
		struct mb_cache_entry *ce =
			list_entry(l, struct mb_cache_entry, e_index.o_list);
		if (ce->e_bdev == bdev && ce->e_index.o_key == key) {
			DEFINE_WAIT(wait);

			if (!list_empty(&ce->e_lru_list))
				list_del_init(&ce->e_lru_list);

			/* Incrementing before holding the lock gives readers
			   priority over writers. */
			ce->e_used++;
			while (ce->e_used >= MB_CACHE_WRITER) {
				ce->e_queued++;
				prepare_to_wait(&mb_cache_queue, &wait,
						TASK_UNINTERRUPTIBLE);
				spin_unlock(&mb_cache_spinlock);
				schedule();
				spin_lock(&mb_cache_spinlock);
				ce->e_queued--;
			}
			finish_wait(&mb_cache_queue, &wait);

			if (!__mb_cache_entry_is_hashed(ce)) {
				__mb_cache_entry_release_unlock(ce);
				spin_lock(&mb_cache_spinlock);
				return ERR_PTR(-EAGAIN);
			}
			return ce;
		}
		l = l->next;
	}
	return NULL;
}


/*
 * mb_cache_entry_find_first()
 *
 * Find the first cache entry on a given device with a certain key in
 * an additional index. Additional matches can be found with
 * mb_cache_entry_find_next(). Returns NULL if no match was found. The
 * returned cache entry is locked for shared access ("multiple readers").
 *
 * @cache: the cache to search
 * @bdev: the device the cache entry should belong to
 * @key: the key in the index
 */
struct mb_cache_entry *
mb_cache_entry_find_first(struct mb_cache *cache, struct block_device *bdev,
			  unsigned int key)
{
	unsigned int bucket = hash_long(key, cache->c_bucket_bits);
	struct list_head *l;
	struct mb_cache_entry *ce;

	spin_lock(&mb_cache_spinlock);
	l = cache->c_index_hash[bucket].next;
	ce = __mb_cache_entry_find(l, &cache->c_index_hash[bucket], bdev, key);
	spin_unlock(&mb_cache_spinlock);
	return ce;
}


/*
 * mb_cache_entry_find_next()
 *
 * Find the next cache entry on a given device with a certain key in an
 * additional index. Returns NULL if no match could be found. The previous
 * entry is atomatically released, so that mb_cache_entry_find_next() can
 * be called like this:
 *
 * entry = mb_cache_entry_find_first();
 * while (entry) {
 * 	...
 *	entry = mb_cache_entry_find_next(entry, ...);
 * }
 *
 * @prev: The previous match
 * @bdev: the device the cache entry should belong to
 * @key: the key in the index
 */
struct mb_cache_entry *
mb_cache_entry_find_next(struct mb_cache_entry *prev,
			 struct block_device *bdev, unsigned int key)
{
	struct mb_cache *cache = prev->e_cache;
	unsigned int bucket = hash_long(key, cache->c_bucket_bits);
	struct list_head *l;
	struct mb_cache_entry *ce;

	spin_lock(&mb_cache_spinlock);
	l = prev->e_index.o_list.next;
	ce = __mb_cache_entry_find(l, &cache->c_index_hash[bucket], bdev, key);
	__mb_cache_entry_release_unlock(prev);
	return ce;
}

#endif  /* !defined(MB_CACHE_INDEXES_COUNT) || (MB_CACHE_INDEXES_COUNT > 0) */

static int __init init_mbcache(void)
{
	register_shrinker(&mb_cache_shrinker);
	return 0;
}

static void __exit exit_mbcache(void)
{
	unregister_shrinker(&mb_cache_shrinker);
}

module_init(init_mbcache)
module_exit(exit_mbcache)

=======
// SPDX-License-Identifier: GPL-2.0-only
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/list_bl.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/mbcache.h>

/*
 * Mbcache is a simple key-value store. Keys need not be unique, however
 * key-value pairs are expected to be unique (we use this fact in
 * mb_cache_entry_delete_or_get()).
 *
 * Ext2 and ext4 use this cache for deduplication of extended attribute blocks.
 * Ext4 also uses it for deduplication of xattr values stored in inodes.
 * They use hash of data as a key and provide a value that may represent a
 * block or inode number. That's why keys need not be unique (hash of different
 * data may be the same). However user provided value always uniquely
 * identifies a cache entry.
 *
 * We provide functions for creation and removal of entries, search by key,
 * and a special "delete entry with given key-value pair" operation. Fixed
 * size hash table is used for fast key lookups.
 */

struct mb_cache {
	/* Hash table of entries */
	struct hlist_bl_head	*c_hash;
	/* log2 of hash table size */
	int			c_bucket_bits;
	/* Maximum entries in cache to avoid degrading hash too much */
	unsigned long		c_max_entries;
	/* Protects c_list, c_entry_count */
	spinlock_t		c_list_lock;
	struct list_head	c_list;
	/* Number of entries in cache */
	unsigned long		c_entry_count;
	struct shrinker		*c_shrink;
	/* Work for shrinking when the cache has too many entries */
	struct work_struct	c_shrink_work;
};

static struct kmem_cache *mb_entry_cache;

static unsigned long mb_cache_shrink(struct mb_cache *cache,
				     unsigned long nr_to_scan);

static inline struct hlist_bl_head *mb_cache_entry_head(struct mb_cache *cache,
							u32 key)
{
	return &cache->c_hash[hash_32(key, cache->c_bucket_bits)];
}

/*
 * Number of entries to reclaim synchronously when there are too many entries
 * in cache
 */
#define SYNC_SHRINK_BATCH 64

/*
 * mb_cache_entry_create - create entry in cache
 * @cache - cache where the entry should be created
 * @mask - gfp mask with which the entry should be allocated
 * @key - key of the entry
 * @value - value of the entry
 * @reusable - is the entry reusable by others?
 *
 * Creates entry in @cache with key @key and value @value. The function returns
 * -EBUSY if entry with the same key and value already exists in cache.
 * Otherwise 0 is returned.
 */
int mb_cache_entry_create(struct mb_cache *cache, gfp_t mask, u32 key,
			  u64 value, bool reusable)
{
	struct mb_cache_entry *entry, *dup;
	struct hlist_bl_node *dup_node;
	struct hlist_bl_head *head;

	/* Schedule background reclaim if there are too many entries */
	if (cache->c_entry_count >= cache->c_max_entries)
		schedule_work(&cache->c_shrink_work);
	/* Do some sync reclaim if background reclaim cannot keep up */
	if (cache->c_entry_count >= 2*cache->c_max_entries)
		mb_cache_shrink(cache, SYNC_SHRINK_BATCH);

	entry = kmem_cache_alloc(mb_entry_cache, mask);
	if (!entry)
		return -ENOMEM;

	INIT_LIST_HEAD(&entry->e_list);
	/*
	 * We create entry with two references. One reference is kept by the
	 * hash table, the other reference is used to protect us from
	 * mb_cache_entry_delete_or_get() until the entry is fully setup. This
	 * avoids nesting of cache->c_list_lock into hash table bit locks which
	 * is problematic for RT.
	 */
	atomic_set(&entry->e_refcnt, 2);
	entry->e_key = key;
	entry->e_value = value;
	entry->e_flags = 0;
	if (reusable)
		set_bit(MBE_REUSABLE_B, &entry->e_flags);
	head = mb_cache_entry_head(cache, key);
	hlist_bl_lock(head);
	hlist_bl_for_each_entry(dup, dup_node, head, e_hash_list) {
		if (dup->e_key == key && dup->e_value == value) {
			hlist_bl_unlock(head);
			kmem_cache_free(mb_entry_cache, entry);
			return -EBUSY;
		}
	}
	hlist_bl_add_head(&entry->e_hash_list, head);
	hlist_bl_unlock(head);
	spin_lock(&cache->c_list_lock);
	list_add_tail(&entry->e_list, &cache->c_list);
	cache->c_entry_count++;
	spin_unlock(&cache->c_list_lock);
	mb_cache_entry_put(cache, entry);

	return 0;
}
EXPORT_SYMBOL(mb_cache_entry_create);

void __mb_cache_entry_free(struct mb_cache *cache, struct mb_cache_entry *entry)
{
	struct hlist_bl_head *head;

	head = mb_cache_entry_head(cache, entry->e_key);
	hlist_bl_lock(head);
	hlist_bl_del(&entry->e_hash_list);
	hlist_bl_unlock(head);
	kmem_cache_free(mb_entry_cache, entry);
}
EXPORT_SYMBOL(__mb_cache_entry_free);

/*
 * mb_cache_entry_wait_unused - wait to be the last user of the entry
 *
 * @entry - entry to work on
 *
 * Wait to be the last user of the entry.
 */
void mb_cache_entry_wait_unused(struct mb_cache_entry *entry)
{
	wait_var_event(&entry->e_refcnt, atomic_read(&entry->e_refcnt) <= 2);
}
EXPORT_SYMBOL(mb_cache_entry_wait_unused);

static struct mb_cache_entry *__entry_find(struct mb_cache *cache,
					   struct mb_cache_entry *entry,
					   u32 key)
{
	struct mb_cache_entry *old_entry = entry;
	struct hlist_bl_node *node;
	struct hlist_bl_head *head;

	head = mb_cache_entry_head(cache, key);
	hlist_bl_lock(head);
	if (entry && !hlist_bl_unhashed(&entry->e_hash_list))
		node = entry->e_hash_list.next;
	else
		node = hlist_bl_first(head);
	while (node) {
		entry = hlist_bl_entry(node, struct mb_cache_entry,
				       e_hash_list);
		if (entry->e_key == key &&
		    test_bit(MBE_REUSABLE_B, &entry->e_flags) &&
		    atomic_inc_not_zero(&entry->e_refcnt))
			goto out;
		node = node->next;
	}
	entry = NULL;
out:
	hlist_bl_unlock(head);
	if (old_entry)
		mb_cache_entry_put(cache, old_entry);

	return entry;
}

/*
 * mb_cache_entry_find_first - find the first reusable entry with the given key
 * @cache: cache where we should search
 * @key: key to look for
 *
 * Search in @cache for a reusable entry with key @key. Grabs reference to the
 * first reusable entry found and returns the entry.
 */
struct mb_cache_entry *mb_cache_entry_find_first(struct mb_cache *cache,
						 u32 key)
{
	return __entry_find(cache, NULL, key);
}
EXPORT_SYMBOL(mb_cache_entry_find_first);

/*
 * mb_cache_entry_find_next - find next reusable entry with the same key
 * @cache: cache where we should search
 * @entry: entry to start search from
 *
 * Finds next reusable entry in the hash chain which has the same key as @entry.
 * If @entry is unhashed (which can happen when deletion of entry races with the
 * search), finds the first reusable entry in the hash chain. The function drops
 * reference to @entry and returns with a reference to the found entry.
 */
struct mb_cache_entry *mb_cache_entry_find_next(struct mb_cache *cache,
						struct mb_cache_entry *entry)
{
	return __entry_find(cache, entry, entry->e_key);
}
EXPORT_SYMBOL(mb_cache_entry_find_next);

/*
 * mb_cache_entry_get - get a cache entry by value (and key)
 * @cache - cache we work with
 * @key - key
 * @value - value
 */
struct mb_cache_entry *mb_cache_entry_get(struct mb_cache *cache, u32 key,
					  u64 value)
{
	struct hlist_bl_node *node;
	struct hlist_bl_head *head;
	struct mb_cache_entry *entry;

	head = mb_cache_entry_head(cache, key);
	hlist_bl_lock(head);
	hlist_bl_for_each_entry(entry, node, head, e_hash_list) {
		if (entry->e_key == key && entry->e_value == value &&
		    atomic_inc_not_zero(&entry->e_refcnt))
			goto out;
	}
	entry = NULL;
out:
	hlist_bl_unlock(head);
	return entry;
}
EXPORT_SYMBOL(mb_cache_entry_get);

/* mb_cache_entry_delete_or_get - remove a cache entry if it has no users
 * @cache - cache we work with
 * @key - key
 * @value - value
 *
 * Remove entry from cache @cache with key @key and value @value. The removal
 * happens only if the entry is unused. The function returns NULL in case the
 * entry was successfully removed or there's no entry in cache. Otherwise the
 * function grabs reference of the entry that we failed to delete because it
 * still has users and return it.
 */
struct mb_cache_entry *mb_cache_entry_delete_or_get(struct mb_cache *cache,
						    u32 key, u64 value)
{
	struct mb_cache_entry *entry;

	entry = mb_cache_entry_get(cache, key, value);
	if (!entry)
		return NULL;

	/*
	 * Drop the ref we got from mb_cache_entry_get() and the initial hash
	 * ref if we are the last user
	 */
	if (atomic_cmpxchg(&entry->e_refcnt, 2, 0) != 2)
		return entry;

	spin_lock(&cache->c_list_lock);
	if (!list_empty(&entry->e_list))
		list_del_init(&entry->e_list);
	cache->c_entry_count--;
	spin_unlock(&cache->c_list_lock);
	__mb_cache_entry_free(cache, entry);
	return NULL;
}
EXPORT_SYMBOL(mb_cache_entry_delete_or_get);

/* mb_cache_entry_touch - cache entry got used
 * @cache - cache the entry belongs to
 * @entry - entry that got used
 *
 * Marks entry as used to give hit higher chances of surviving in cache.
 */
void mb_cache_entry_touch(struct mb_cache *cache,
			  struct mb_cache_entry *entry)
{
	set_bit(MBE_REFERENCED_B, &entry->e_flags);
}
EXPORT_SYMBOL(mb_cache_entry_touch);

static unsigned long mb_cache_count(struct shrinker *shrink,
				    struct shrink_control *sc)
{
	struct mb_cache *cache = shrink->private_data;

	return cache->c_entry_count;
}

/* Shrink number of entries in cache */
static unsigned long mb_cache_shrink(struct mb_cache *cache,
				     unsigned long nr_to_scan)
{
	struct mb_cache_entry *entry;
	unsigned long shrunk = 0;

	spin_lock(&cache->c_list_lock);
	while (nr_to_scan-- && !list_empty(&cache->c_list)) {
		entry = list_first_entry(&cache->c_list,
					 struct mb_cache_entry, e_list);
		/* Drop initial hash reference if there is no user */
		if (test_bit(MBE_REFERENCED_B, &entry->e_flags) ||
		    atomic_cmpxchg(&entry->e_refcnt, 1, 0) != 1) {
			clear_bit(MBE_REFERENCED_B, &entry->e_flags);
			list_move_tail(&entry->e_list, &cache->c_list);
			continue;
		}
		list_del_init(&entry->e_list);
		cache->c_entry_count--;
		spin_unlock(&cache->c_list_lock);
		__mb_cache_entry_free(cache, entry);
		shrunk++;
		cond_resched();
		spin_lock(&cache->c_list_lock);
	}
	spin_unlock(&cache->c_list_lock);

	return shrunk;
}

static unsigned long mb_cache_scan(struct shrinker *shrink,
				   struct shrink_control *sc)
{
	struct mb_cache *cache = shrink->private_data;
	return mb_cache_shrink(cache, sc->nr_to_scan);
}

/* We shrink 1/X of the cache when we have too many entries in it */
#define SHRINK_DIVISOR 16

static void mb_cache_shrink_worker(struct work_struct *work)
{
	struct mb_cache *cache = container_of(work, struct mb_cache,
					      c_shrink_work);
	mb_cache_shrink(cache, cache->c_max_entries / SHRINK_DIVISOR);
}

/*
 * mb_cache_create - create cache
 * @bucket_bits: log2 of the hash table size
 *
 * Create cache for keys with 2^bucket_bits hash entries.
 */
struct mb_cache *mb_cache_create(int bucket_bits)
{
	struct mb_cache *cache;
	unsigned long bucket_count = 1UL << bucket_bits;
	unsigned long i;

	cache = kzalloc(sizeof(struct mb_cache), GFP_KERNEL);
	if (!cache)
		goto err_out;
	cache->c_bucket_bits = bucket_bits;
	cache->c_max_entries = bucket_count << 4;
	INIT_LIST_HEAD(&cache->c_list);
	spin_lock_init(&cache->c_list_lock);
	cache->c_hash = kmalloc_array(bucket_count,
				      sizeof(struct hlist_bl_head),
				      GFP_KERNEL);
	if (!cache->c_hash) {
		kfree(cache);
		goto err_out;
	}
	for (i = 0; i < bucket_count; i++)
		INIT_HLIST_BL_HEAD(&cache->c_hash[i]);

	cache->c_shrink = shrinker_alloc(0, "mbcache-shrinker");
	if (!cache->c_shrink) {
		kfree(cache->c_hash);
		kfree(cache);
		goto err_out;
	}

	cache->c_shrink->count_objects = mb_cache_count;
	cache->c_shrink->scan_objects = mb_cache_scan;
	cache->c_shrink->private_data = cache;

	shrinker_register(cache->c_shrink);

	INIT_WORK(&cache->c_shrink_work, mb_cache_shrink_worker);

	return cache;

err_out:
	return NULL;
}
EXPORT_SYMBOL(mb_cache_create);

/*
 * mb_cache_destroy - destroy cache
 * @cache: the cache to destroy
 *
 * Free all entries in cache and cache itself. Caller must make sure nobody
 * (except shrinker) can reach @cache when calling this.
 */
void mb_cache_destroy(struct mb_cache *cache)
{
	struct mb_cache_entry *entry, *next;

	shrinker_free(cache->c_shrink);

	/*
	 * We don't bother with any locking. Cache must not be used at this
	 * point.
	 */
	list_for_each_entry_safe(entry, next, &cache->c_list, e_list) {
		list_del(&entry->e_list);
		WARN_ON(atomic_read(&entry->e_refcnt) != 1);
		mb_cache_entry_put(cache, entry);
	}
	kfree(cache->c_hash);
	kfree(cache);
}
EXPORT_SYMBOL(mb_cache_destroy);

static int __init mbcache_init(void)
{
	mb_entry_cache = KMEM_CACHE(mb_cache_entry, SLAB_RECLAIM_ACCOUNT);
	if (!mb_entry_cache)
		return -ENOMEM;
	return 0;
}

static void __exit mbcache_exit(void)
{
	kmem_cache_destroy(mb_entry_cache);
}

module_init(mbcache_init)
module_exit(mbcache_exit)

MODULE_AUTHOR("Jan Kara <jack@suse.cz>");
MODULE_DESCRIPTION("Meta block cache (for extended attributes)");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
