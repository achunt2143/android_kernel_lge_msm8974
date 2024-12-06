/*
 * zsmalloc memory allocator
 *
 * Copyright (C) 2011  Nitin Gupta
 * Copyright (C) 2012, 2013 Minchan Kim
 *
 * This code is released using a dual license strategy: BSD/GPL
 * You can choose the license that better fits your requirements.
 *
 * Released under the terms of 3-clause BSD License
 * Released under the terms of GNU General Public License Version 2.0
 */

<<<<<<< HEAD

/*
 * This allocator is designed for use with zcache and zram. Thus, the
 * allocator is supposed to work well under low memory conditions. In
 * particular, it never attempts higher order page allocation which is
 * very likely to fail under memory pressure. On the other hand, if we
 * just use single (0-order) pages, it would suffer from very high
 * fragmentation -- any object of size PAGE_SIZE/2 or larger would occupy
 * an entire page. This was one of the major issues with its predecessor
 * (xvmalloc).
 *
 * To overcome these issues, zsmalloc allocates a bunch of 0-order pages
 * and links them together using various 'struct page' fields. These linked
 * pages act as a single higher-order page i.e. an object can span 0-order
 * page boundaries. The code refers to these linked pages as a single entity
 * called zspage.
 *
=======
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Following is how we use various fields and flags of underlying
 * struct page(s) to form a zspage.
 *
 * Usage of struct page fields:
<<<<<<< HEAD
 *	page->first_page: points to the first component (0-order) page
 *	page->index (union with page->freelist): offset of the first object
 *		starting in this page. For the first page, this is
 *		always 0, so we use this field (aka freelist) to point
 *		to the first free object in zspage.
 *	page->lru: links together all component pages (except the first page)
 *		of a zspage
 *
 *	For _first_ page only:
 *
 *	page->private (union with page->first_page): refers to the
 *		component page after the first page
 *	page->freelist: points to the first free object in zspage.
 *		Free objects are linked together using in-place
 *		metadata.
 *	page->objects: maximum number of objects we can store in this
 *		zspage (class->zspage_order * PAGE_SIZE / class->size)
 *	page->lru: links together first pages of various zspages.
 *		Basically forming list of zspages in a fullness group.
 *	page->mapping: class index and fullness group of the zspage
 *
 * Usage of struct page flags:
 *	PG_private: identifies the first component page
 *	PG_private2: identifies the last component page
 *
 */

#ifdef CONFIG_ZSMALLOC_DEBUG
#define DEBUG
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/bitops.h>
#include <linux/errno.h>
#include <linux/highmem.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/tlbflush.h>
#include <asm/pgtable.h>
#include <linux/cpumask.h>
#include <linux/cpu.h>
#include <linux/vmalloc.h>
#include <linux/hardirq.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/zsmalloc.h>

/*
 * This must be power of 2 and greater than of equal to sizeof(link_free).
=======
 *	page->private: points to zspage
 *	page->index: links together all component pages of a zspage
 *		For the huge page, this is always 0, so we use this field
 *		to store handle.
 *	page->page_type: first object offset in a subpage of zspage
 *
 * Usage of struct page flags:
 *	PG_private: identifies the first component page
 *	PG_owner_priv_1: identifies the huge component page
 *
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

/*
 * lock ordering:
 *	page_lock
 *	pool->lock
 *	zspage->lock
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/bitops.h>
#include <linux/errno.h>
#include <linux/highmem.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/pgtable.h>
#include <asm/tlbflush.h>
#include <linux/cpumask.h>
#include <linux/cpu.h>
#include <linux/vmalloc.h>
#include <linux/preempt.h>
#include <linux/spinlock.h>
#include <linux/shrinker.h>
#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/zsmalloc.h>
#include <linux/zpool.h>
#include <linux/migrate.h>
#include <linux/wait.h>
#include <linux/pagemap.h>
#include <linux/fs.h>
#include <linux/local_lock.h>

#define ZSPAGE_MAGIC	0x58

/*
 * This must be power of 2 and greater than or equal to sizeof(link_free).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * These two conditions ensure that any 'struct link_free' itself doesn't
 * span more than 1 page which avoids complex case of mapping 2 pages simply
 * to restore link_free pointer values.
 */
#define ZS_ALIGN		8

<<<<<<< HEAD
/*
 * A single 'zspage' is composed of up to 2^N discontiguous 0-order (single)
 * pages. ZS_MAX_ZSPAGE_ORDER defines upper limit on N.
 */
#define ZS_MAX_ZSPAGE_ORDER 2
#define ZS_MAX_PAGES_PER_ZSPAGE (_AC(1, UL) << ZS_MAX_ZSPAGE_ORDER)

/*
 * Object location (<PFN>, <obj_idx>) is encoded as
 * as single (void *) handle value.
 *
 * Note that object index <obj_idx> is relative to system
 * page <PFN> it is stored in, so for each sub-page belonging
 * to a zspage, obj_idx starts with 0.
=======
#define ZS_HANDLE_SIZE (sizeof(unsigned long))

/*
 * Object location (<PFN>, <obj_idx>) is encoded as
 * a single (unsigned long) handle value.
 *
 * Note that object index <obj_idx> starts from 0.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This is made more complicated by various memory models and PAE.
 */

<<<<<<< HEAD
#ifndef MAX_PHYSMEM_BITS
#ifdef CONFIG_HIGHMEM64G
#define MAX_PHYSMEM_BITS 36
#else /* !CONFIG_HIGHMEM64G */
=======
#ifndef MAX_POSSIBLE_PHYSMEM_BITS
#ifdef MAX_PHYSMEM_BITS
#define MAX_POSSIBLE_PHYSMEM_BITS MAX_PHYSMEM_BITS
#else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * If this definition of MAX_PHYSMEM_BITS is used, OBJ_INDEX_BITS will just
 * be PAGE_SHIFT
 */
<<<<<<< HEAD
#define MAX_PHYSMEM_BITS BITS_PER_LONG
#endif
#endif
#define _PFN_BITS		(MAX_PHYSMEM_BITS - PAGE_SHIFT)
#define OBJ_INDEX_BITS	(BITS_PER_LONG - _PFN_BITS)
#define OBJ_INDEX_MASK	((_AC(1, UL) << OBJ_INDEX_BITS) - 1)

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
/* ZS_MIN_ALLOC_SIZE must be multiple of ZS_ALIGN */
#define ZS_MIN_ALLOC_SIZE \
	MAX(32, (ZS_MAX_PAGES_PER_ZSPAGE << PAGE_SHIFT >> OBJ_INDEX_BITS))
#define ZS_MAX_ALLOC_SIZE	PAGE_SIZE

/*
 * On systems with 4K page size, this gives 254 size classes! There is a
=======
#define MAX_POSSIBLE_PHYSMEM_BITS BITS_PER_LONG
#endif
#endif

#define _PFN_BITS		(MAX_POSSIBLE_PHYSMEM_BITS - PAGE_SHIFT)

/*
 * Head in allocated object should have OBJ_ALLOCATED_TAG
 * to identify the object was allocated or not.
 * It's okay to add the status bit in the least bit because
 * header keeps handle which is 4byte-aligned address so we
 * have room for two bit at least.
 */
#define OBJ_ALLOCATED_TAG 1

#define OBJ_TAG_BITS	1
#define OBJ_TAG_MASK	OBJ_ALLOCATED_TAG

#define OBJ_INDEX_BITS	(BITS_PER_LONG - _PFN_BITS)
#define OBJ_INDEX_MASK	((_AC(1, UL) << OBJ_INDEX_BITS) - 1)

#define HUGE_BITS	1
#define FULLNESS_BITS	4
#define CLASS_BITS	8
#define MAGIC_VAL_BITS	8

#define MAX(a, b) ((a) >= (b) ? (a) : (b))

#define ZS_MAX_PAGES_PER_ZSPAGE	(_AC(CONFIG_ZSMALLOC_CHAIN_SIZE, UL))

/* ZS_MIN_ALLOC_SIZE must be multiple of ZS_ALIGN */
#define ZS_MIN_ALLOC_SIZE \
	MAX(32, (ZS_MAX_PAGES_PER_ZSPAGE << PAGE_SHIFT >> OBJ_INDEX_BITS))
/* each chunk includes extra space to keep handle */
#define ZS_MAX_ALLOC_SIZE	PAGE_SIZE

/*
 * On systems with 4K page size, this gives 255 size classes! There is a
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * trader-off here:
 *  - Large number of size classes is potentially wasteful as free page are
 *    spread across these classes
 *  - Small number of size classes causes large internal fragmentation
 *  - Probably its better to use specific size classes (empirically
 *    determined). NOTE: all those class sizes must be set as multiple of
 *    ZS_ALIGN to make sure link_free itself never has to span 2 pages.
 *
 *  ZS_MIN_ALLOC_SIZE and ZS_SIZE_CLASS_DELTA must be multiple of ZS_ALIGN
 *  (reason above)
 */
<<<<<<< HEAD
#define ZS_SIZE_CLASS_DELTA	(PAGE_SIZE >> 8)
#define ZS_SIZE_CLASSES		((ZS_MAX_ALLOC_SIZE - ZS_MIN_ALLOC_SIZE) / \
					ZS_SIZE_CLASS_DELTA + 1)

/*
 * We do not maintain any list for completely empty or full pages
 */
enum fullness_group {
	ZS_ALMOST_FULL,
	ZS_ALMOST_EMPTY,
	_ZS_NR_FULLNESS_GROUPS,

	ZS_EMPTY,
	ZS_FULL
};

/*
 * We assign a page to ZS_ALMOST_EMPTY fullness group when:
 *	n <= N / f, where
 * n = number of allocated objects
 * N = total number of objects zspage can store
 * f = 1/fullness_threshold_frac
 *
 * Similarly, we assign zspage to:
 *	ZS_ALMOST_FULL	when n > N / f
 *	ZS_EMPTY	when n == 0
 *	ZS_FULL		when n == N
 *
 * (see: fix_fullness_group())
 */
static const int fullness_threshold_frac = 4;

struct size_class {
=======
#define ZS_SIZE_CLASS_DELTA	(PAGE_SIZE >> CLASS_BITS)
#define ZS_SIZE_CLASSES	(DIV_ROUND_UP(ZS_MAX_ALLOC_SIZE - ZS_MIN_ALLOC_SIZE, \
				      ZS_SIZE_CLASS_DELTA) + 1)

/*
 * Pages are distinguished by the ratio of used memory (that is the ratio
 * of ->inuse objects to all objects that page can store). For example,
 * INUSE_RATIO_10 means that the ratio of used objects is > 0% and <= 10%.
 *
 * The number of fullness groups is not random. It allows us to keep
 * difference between the least busy page in the group (minimum permitted
 * number of ->inuse objects) and the most busy page (maximum permitted
 * number of ->inuse objects) at a reasonable value.
 */
enum fullness_group {
	ZS_INUSE_RATIO_0,
	ZS_INUSE_RATIO_10,
	/* NOTE: 8 more fullness groups here */
	ZS_INUSE_RATIO_99       = 10,
	ZS_INUSE_RATIO_100,
	NR_FULLNESS_GROUPS,
};

enum class_stat_type {
	/* NOTE: stats for 12 fullness groups here: from inuse 0 to 100 */
	ZS_OBJS_ALLOCATED       = NR_FULLNESS_GROUPS,
	ZS_OBJS_INUSE,
	NR_CLASS_STAT_TYPES,
};

struct zs_size_stat {
	unsigned long objs[NR_CLASS_STAT_TYPES];
};

#ifdef CONFIG_ZSMALLOC_STAT
static struct dentry *zs_stat_root;
#endif

static size_t huge_class_size;

struct size_class {
	struct list_head fullness_list[NR_FULLNESS_GROUPS];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Size of objects stored in this class. Must be multiple
	 * of ZS_ALIGN.
	 */
	int size;
<<<<<<< HEAD
	unsigned int index;

	/* Number of PAGE_SIZE sized pages to combine to form a 'zspage' */
	int pages_per_zspage;

	spinlock_t lock;

	/* stats */
	u64 pages_allocated;

	struct page *fullness_list[_ZS_NR_FULLNESS_GROUPS];
=======
	int objs_per_zspage;
	/* Number of PAGE_SIZE sized pages to combine to form a 'zspage' */
	int pages_per_zspage;

	unsigned int index;
	struct zs_size_stat stats;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Placed within free objects to form a singly linked list.
<<<<<<< HEAD
 * For every zspage, first_page->freelist gives head of this list.
=======
 * For every zspage, zspage->freeobj gives head of this list.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This must be power of 2 and less than or equal to ZS_ALIGN
 */
struct link_free {
<<<<<<< HEAD
	/* Handle of next free chunk (encodes <PFN, obj_idx>) */
	void *next;
};

struct zs_pool {
	struct size_class size_class[ZS_SIZE_CLASSES];

	gfp_t flags;	/* allocation flags used when growing pool */
};

/*
 * A zspage's class index and fullness group
 * are encoded in its (first)page->mapping
 */
#define CLASS_IDX_BITS	28
#define FULLNESS_BITS	4
#define CLASS_IDX_MASK	((1 << CLASS_IDX_BITS) - 1)
#define FULLNESS_MASK	((1 << FULLNESS_BITS) - 1)

/*
 * By default, zsmalloc uses a copy-based object mapping method to access
 * allocations that span two pages. However, if a particular architecture
 * performs VM mapping faster than copying, then it should be added here
 * so that USE_PGTABLE_MAPPING is defined. This causes zsmalloc to use
 * page table mapping rather than copying for object mapping.
 */
#if defined(CONFIG_ARM) && !defined(MODULE)
#define USE_PGTABLE_MAPPING
#endif

struct mapping_area {
#ifdef USE_PGTABLE_MAPPING
	struct vm_struct *vm; /* vm area for mapping object that span pages */
#else
	char *vm_buf; /* copy buffer for objects that span pages */
#endif
=======
	union {
		/*
		 * Free object index;
		 * It's valid for non-allocated object
		 */
		unsigned long next;
		/*
		 * Handle of allocated object.
		 */
		unsigned long handle;
	};
};

struct zs_pool {
	const char *name;

	struct size_class *size_class[ZS_SIZE_CLASSES];
	struct kmem_cache *handle_cachep;
	struct kmem_cache *zspage_cachep;

	atomic_long_t pages_allocated;

	struct zs_pool_stats stats;

	/* Compact classes */
	struct shrinker *shrinker;

#ifdef CONFIG_ZSMALLOC_STAT
	struct dentry *stat_dentry;
#endif
#ifdef CONFIG_COMPACTION
	struct work_struct free_work;
#endif
	spinlock_t lock;
	atomic_t compaction_in_progress;
};

struct zspage {
	struct {
		unsigned int huge:HUGE_BITS;
		unsigned int fullness:FULLNESS_BITS;
		unsigned int class:CLASS_BITS + 1;
		unsigned int magic:MAGIC_VAL_BITS;
	};
	unsigned int inuse;
	unsigned int freeobj;
	struct page *first_page;
	struct list_head list; /* fullness list */
	struct zs_pool *pool;
	rwlock_t lock;
};

struct mapping_area {
	local_lock_t lock;
	char *vm_buf; /* copy buffer for objects that span pages */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *vm_addr; /* address of kmap_atomic()'ed pages */
	enum zs_mapmode vm_mm; /* mapping mode */
};

<<<<<<< HEAD

/* per-cpu VM mapping areas for zspage accesses that cross page boundaries */
static DEFINE_PER_CPU(struct mapping_area, zs_map_area);

static int is_first_page(struct page *page)
=======
/* huge object: pages_per_zspage == 1 && maxobj_per_zspage == 1 */
static void SetZsHugePage(struct zspage *zspage)
{
	zspage->huge = 1;
}

static bool ZsHugePage(struct zspage *zspage)
{
	return zspage->huge;
}

static void migrate_lock_init(struct zspage *zspage);
static void migrate_read_lock(struct zspage *zspage);
static void migrate_read_unlock(struct zspage *zspage);
static void migrate_write_lock(struct zspage *zspage);
static void migrate_write_unlock(struct zspage *zspage);

#ifdef CONFIG_COMPACTION
static void kick_deferred_free(struct zs_pool *pool);
static void init_deferred_free(struct zs_pool *pool);
static void SetZsPageMovable(struct zs_pool *pool, struct zspage *zspage);
#else
static void kick_deferred_free(struct zs_pool *pool) {}
static void init_deferred_free(struct zs_pool *pool) {}
static void SetZsPageMovable(struct zs_pool *pool, struct zspage *zspage) {}
#endif

static int create_cache(struct zs_pool *pool)
{
	pool->handle_cachep = kmem_cache_create("zs_handle", ZS_HANDLE_SIZE,
					0, 0, NULL);
	if (!pool->handle_cachep)
		return 1;

	pool->zspage_cachep = kmem_cache_create("zspage", sizeof(struct zspage),
					0, 0, NULL);
	if (!pool->zspage_cachep) {
		kmem_cache_destroy(pool->handle_cachep);
		pool->handle_cachep = NULL;
		return 1;
	}

	return 0;
}

static void destroy_cache(struct zs_pool *pool)
{
	kmem_cache_destroy(pool->handle_cachep);
	kmem_cache_destroy(pool->zspage_cachep);
}

static unsigned long cache_alloc_handle(struct zs_pool *pool, gfp_t gfp)
{
	return (unsigned long)kmem_cache_alloc(pool->handle_cachep,
			gfp & ~(__GFP_HIGHMEM|__GFP_MOVABLE));
}

static void cache_free_handle(struct zs_pool *pool, unsigned long handle)
{
	kmem_cache_free(pool->handle_cachep, (void *)handle);
}

static struct zspage *cache_alloc_zspage(struct zs_pool *pool, gfp_t flags)
{
	return kmem_cache_zalloc(pool->zspage_cachep,
			flags & ~(__GFP_HIGHMEM|__GFP_MOVABLE));
}

static void cache_free_zspage(struct zs_pool *pool, struct zspage *zspage)
{
	kmem_cache_free(pool->zspage_cachep, zspage);
}

/* pool->lock(which owns the handle) synchronizes races */
static void record_obj(unsigned long handle, unsigned long obj)
{
	*(unsigned long *)handle = obj;
}

/* zpool driver */

#ifdef CONFIG_ZPOOL

static void *zs_zpool_create(const char *name, gfp_t gfp)
{
	/*
	 * Ignore global gfp flags: zs_malloc() may be invoked from
	 * different contexts and its caller must provide a valid
	 * gfp mask.
	 */
	return zs_create_pool(name);
}

static void zs_zpool_destroy(void *pool)
{
	zs_destroy_pool(pool);
}

static int zs_zpool_malloc(void *pool, size_t size, gfp_t gfp,
			unsigned long *handle)
{
	*handle = zs_malloc(pool, size, gfp);

	if (IS_ERR_VALUE(*handle))
		return PTR_ERR((void *)*handle);
	return 0;
}
static void zs_zpool_free(void *pool, unsigned long handle)
{
	zs_free(pool, handle);
}

static void *zs_zpool_map(void *pool, unsigned long handle,
			enum zpool_mapmode mm)
{
	enum zs_mapmode zs_mm;

	switch (mm) {
	case ZPOOL_MM_RO:
		zs_mm = ZS_MM_RO;
		break;
	case ZPOOL_MM_WO:
		zs_mm = ZS_MM_WO;
		break;
	case ZPOOL_MM_RW:
	default:
		zs_mm = ZS_MM_RW;
		break;
	}

	return zs_map_object(pool, handle, zs_mm);
}
static void zs_zpool_unmap(void *pool, unsigned long handle)
{
	zs_unmap_object(pool, handle);
}

static u64 zs_zpool_total_size(void *pool)
{
	return zs_get_total_pages(pool) << PAGE_SHIFT;
}

static struct zpool_driver zs_zpool_driver = {
	.type =			  "zsmalloc",
	.owner =		  THIS_MODULE,
	.create =		  zs_zpool_create,
	.destroy =		  zs_zpool_destroy,
	.malloc_support_movable = true,
	.malloc =		  zs_zpool_malloc,
	.free =			  zs_zpool_free,
	.map =			  zs_zpool_map,
	.unmap =		  zs_zpool_unmap,
	.total_size =		  zs_zpool_total_size,
};

MODULE_ALIAS("zpool-zsmalloc");
#endif /* CONFIG_ZPOOL */

/* per-cpu VM mapping areas for zspage accesses that cross page boundaries */
static DEFINE_PER_CPU(struct mapping_area, zs_map_area) = {
	.lock	= INIT_LOCAL_LOCK(lock),
};

static __maybe_unused int is_first_page(struct page *page)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return PagePrivate(page);
}

<<<<<<< HEAD
static int is_last_page(struct page *page)
{
	return PagePrivate2(page);
}

static void get_zspage_mapping(struct page *page, unsigned int *class_idx,
				enum fullness_group *fullness)
{
	unsigned long m;
	BUG_ON(!is_first_page(page));

	m = (unsigned long)page->mapping;
	*fullness = m & FULLNESS_MASK;
	*class_idx = (m >> FULLNESS_BITS) & CLASS_IDX_MASK;
}

static void set_zspage_mapping(struct page *page, unsigned int class_idx,
				enum fullness_group fullness)
{
	unsigned long m;
	BUG_ON(!is_first_page(page));

	m = ((class_idx & CLASS_IDX_MASK) << FULLNESS_BITS) |
			(fullness & FULLNESS_MASK);
	page->mapping = (struct address_space *)m;
}

=======
/* Protected by pool->lock */
static inline int get_zspage_inuse(struct zspage *zspage)
{
	return zspage->inuse;
}


static inline void mod_zspage_inuse(struct zspage *zspage, int val)
{
	zspage->inuse += val;
}

static inline struct page *get_first_page(struct zspage *zspage)
{
	struct page *first_page = zspage->first_page;

	VM_BUG_ON_PAGE(!is_first_page(first_page), first_page);
	return first_page;
}

static inline unsigned int get_first_obj_offset(struct page *page)
{
	return page->page_type;
}

static inline void set_first_obj_offset(struct page *page, unsigned int offset)
{
	page->page_type = offset;
}

static inline unsigned int get_freeobj(struct zspage *zspage)
{
	return zspage->freeobj;
}

static inline void set_freeobj(struct zspage *zspage, unsigned int obj)
{
	zspage->freeobj = obj;
}

static struct size_class *zspage_class(struct zs_pool *pool,
				       struct zspage *zspage)
{
	return pool->size_class[zspage->class];
}

/*
 * zsmalloc divides the pool into various size classes where each
 * class maintains a list of zspages where each zspage is divided
 * into equal sized chunks. Each allocation falls into one of these
 * classes depending on its size. This function returns index of the
 * size class which has chunk size big enough to hold the given size.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int get_size_class_index(int size)
{
	int idx = 0;

	if (likely(size > ZS_MIN_ALLOC_SIZE))
		idx = DIV_ROUND_UP(size - ZS_MIN_ALLOC_SIZE,
				ZS_SIZE_CLASS_DELTA);

<<<<<<< HEAD
	return idx;
}

static enum fullness_group get_fullness_group(struct page *page)
{
	int inuse, max_objects;
	enum fullness_group fg;
	BUG_ON(!is_first_page(page));

	inuse = page->inuse;
	max_objects = page->objects;

	if (inuse == 0)
		fg = ZS_EMPTY;
	else if (inuse == max_objects)
		fg = ZS_FULL;
	else if (inuse <= max_objects / fullness_threshold_frac)
		fg = ZS_ALMOST_EMPTY;
	else
		fg = ZS_ALMOST_FULL;

	return fg;
}

static void insert_zspage(struct page *page, struct size_class *class,
				enum fullness_group fullness)
{
	struct page **head;

	BUG_ON(!is_first_page(page));

	if (fullness >= _ZS_NR_FULLNESS_GROUPS)
		return;

	head = &class->fullness_list[fullness];
	if (*head)
		list_add_tail(&page->lru, &(*head)->lru);

	*head = page;
}

static void remove_zspage(struct page *page, struct size_class *class,
				enum fullness_group fullness)
{
	struct page **head;

	BUG_ON(!is_first_page(page));

	if (fullness >= _ZS_NR_FULLNESS_GROUPS)
		return;

	head = &class->fullness_list[fullness];
	BUG_ON(!*head);
	if (list_empty(&(*head)->lru))
		*head = NULL;
	else if (*head == page)
		*head = (struct page *)list_entry((*head)->lru.next,
					struct page, lru);

	list_del_init(&page->lru);
}

static enum fullness_group fix_fullness_group(struct zs_pool *pool,
						struct page *page)
{
	int class_idx;
	struct size_class *class;
	enum fullness_group currfg, newfg;

	BUG_ON(!is_first_page(page));

	get_zspage_mapping(page, &class_idx, &currfg);
	newfg = get_fullness_group(page);
	if (newfg == currfg)
		goto out;

	class = &pool->size_class[class_idx];
	remove_zspage(page, class, currfg);
	insert_zspage(page, class, newfg);
	set_zspage_mapping(page, class_idx, newfg);

=======
	return min_t(int, ZS_SIZE_CLASSES - 1, idx);
}

static inline void class_stat_inc(struct size_class *class,
				int type, unsigned long cnt)
{
	class->stats.objs[type] += cnt;
}

static inline void class_stat_dec(struct size_class *class,
				int type, unsigned long cnt)
{
	class->stats.objs[type] -= cnt;
}

static inline unsigned long zs_stat_get(struct size_class *class, int type)
{
	return class->stats.objs[type];
}

#ifdef CONFIG_ZSMALLOC_STAT

static void __init zs_stat_init(void)
{
	if (!debugfs_initialized()) {
		pr_warn("debugfs not available, stat dir not created\n");
		return;
	}

	zs_stat_root = debugfs_create_dir("zsmalloc", NULL);
}

static void __exit zs_stat_exit(void)
{
	debugfs_remove_recursive(zs_stat_root);
}

static unsigned long zs_can_compact(struct size_class *class);

static int zs_stats_size_show(struct seq_file *s, void *v)
{
	int i, fg;
	struct zs_pool *pool = s->private;
	struct size_class *class;
	int objs_per_zspage;
	unsigned long obj_allocated, obj_used, pages_used, freeable;
	unsigned long total_objs = 0, total_used_objs = 0, total_pages = 0;
	unsigned long total_freeable = 0;
	unsigned long inuse_totals[NR_FULLNESS_GROUPS] = {0, };

	seq_printf(s, " %5s %5s %9s %9s %9s %9s %9s %9s %9s %9s %9s %9s %9s %13s %10s %10s %16s %8s\n",
			"class", "size", "10%", "20%", "30%", "40%",
			"50%", "60%", "70%", "80%", "90%", "99%", "100%",
			"obj_allocated", "obj_used", "pages_used",
			"pages_per_zspage", "freeable");

	for (i = 0; i < ZS_SIZE_CLASSES; i++) {

		class = pool->size_class[i];

		if (class->index != i)
			continue;

		spin_lock(&pool->lock);

		seq_printf(s, " %5u %5u ", i, class->size);
		for (fg = ZS_INUSE_RATIO_10; fg < NR_FULLNESS_GROUPS; fg++) {
			inuse_totals[fg] += zs_stat_get(class, fg);
			seq_printf(s, "%9lu ", zs_stat_get(class, fg));
		}

		obj_allocated = zs_stat_get(class, ZS_OBJS_ALLOCATED);
		obj_used = zs_stat_get(class, ZS_OBJS_INUSE);
		freeable = zs_can_compact(class);
		spin_unlock(&pool->lock);

		objs_per_zspage = class->objs_per_zspage;
		pages_used = obj_allocated / objs_per_zspage *
				class->pages_per_zspage;

		seq_printf(s, "%13lu %10lu %10lu %16d %8lu\n",
			   obj_allocated, obj_used, pages_used,
			   class->pages_per_zspage, freeable);

		total_objs += obj_allocated;
		total_used_objs += obj_used;
		total_pages += pages_used;
		total_freeable += freeable;
	}

	seq_puts(s, "\n");
	seq_printf(s, " %5s %5s ", "Total", "");

	for (fg = ZS_INUSE_RATIO_10; fg < NR_FULLNESS_GROUPS; fg++)
		seq_printf(s, "%9lu ", inuse_totals[fg]);

	seq_printf(s, "%13lu %10lu %10lu %16s %8lu\n",
		   total_objs, total_used_objs, total_pages, "",
		   total_freeable);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(zs_stats_size);

static void zs_pool_stat_create(struct zs_pool *pool, const char *name)
{
	if (!zs_stat_root) {
		pr_warn("no root stat dir, not creating <%s> stat dir\n", name);
		return;
	}

	pool->stat_dentry = debugfs_create_dir(name, zs_stat_root);

	debugfs_create_file("classes", S_IFREG | 0444, pool->stat_dentry, pool,
			    &zs_stats_size_fops);
}

static void zs_pool_stat_destroy(struct zs_pool *pool)
{
	debugfs_remove_recursive(pool->stat_dentry);
}

#else /* CONFIG_ZSMALLOC_STAT */
static void __init zs_stat_init(void)
{
}

static void __exit zs_stat_exit(void)
{
}

static inline void zs_pool_stat_create(struct zs_pool *pool, const char *name)
{
}

static inline void zs_pool_stat_destroy(struct zs_pool *pool)
{
}
#endif


/*
 * For each size class, zspages are divided into different groups
 * depending on their usage ratio. This function returns fullness
 * status of the given page.
 */
static int get_fullness_group(struct size_class *class, struct zspage *zspage)
{
	int inuse, objs_per_zspage, ratio;

	inuse = get_zspage_inuse(zspage);
	objs_per_zspage = class->objs_per_zspage;

	if (inuse == 0)
		return ZS_INUSE_RATIO_0;
	if (inuse == objs_per_zspage)
		return ZS_INUSE_RATIO_100;

	ratio = 100 * inuse / objs_per_zspage;
	/*
	 * Take integer division into consideration: a page with one inuse
	 * object out of 127 possible, will end up having 0 usage ratio,
	 * which is wrong as it belongs in ZS_INUSE_RATIO_10 fullness group.
	 */
	return ratio / 10 + 1;
}

/*
 * Each size class maintains various freelists and zspages are assigned
 * to one of these freelists based on the number of live objects they
 * have. This functions inserts the given zspage into the freelist
 * identified by <class, fullness_group>.
 */
static void insert_zspage(struct size_class *class,
				struct zspage *zspage,
				int fullness)
{
	class_stat_inc(class, fullness, 1);
	list_add(&zspage->list, &class->fullness_list[fullness]);
	zspage->fullness = fullness;
}

/*
 * This function removes the given zspage from the freelist identified
 * by <class, fullness_group>.
 */
static void remove_zspage(struct size_class *class, struct zspage *zspage)
{
	int fullness = zspage->fullness;

	VM_BUG_ON(list_empty(&class->fullness_list[fullness]));

	list_del_init(&zspage->list);
	class_stat_dec(class, fullness, 1);
}

/*
 * Each size class maintains zspages in different fullness groups depending
 * on the number of live objects they contain. When allocating or freeing
 * objects, the fullness status of the page can change, for instance, from
 * INUSE_RATIO_80 to INUSE_RATIO_70 when freeing an object. This function
 * checks if such a status change has occurred for the given page and
 * accordingly moves the page from the list of the old fullness group to that
 * of the new fullness group.
 */
static int fix_fullness_group(struct size_class *class, struct zspage *zspage)
{
	int newfg;

	newfg = get_fullness_group(class, zspage);
	if (newfg == zspage->fullness)
		goto out;

	remove_zspage(class, zspage);
	insert_zspage(class, zspage, newfg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return newfg;
}

<<<<<<< HEAD
/*
 * We have to decide on how many pages to link together
 * to form a zspage for each size class. This is important
 * to reduce wastage due to unusable space left at end of
 * each zspage which is given as:
 *	wastage = Zp - Zp % size_class
 * where Zp = zspage size = k * PAGE_SIZE where k = 1, 2, ...
 *
 * For example, for size class of 3/8 * PAGE_SIZE, we should
 * link together 3 PAGE_SIZE sized pages to form a zspage
 * since then we can perfectly fit in 8 such objects.
 */
static int get_pages_per_zspage(int class_size)
{
	int i, max_usedpc = 0;
	/* zspage order which gives maximum used size per KB */
	int max_usedpc_order = 1;

	for (i = 1; i <= ZS_MAX_PAGES_PER_ZSPAGE; i++) {
		int zspage_size;
		int waste, usedpc;

		zspage_size = i * PAGE_SIZE;
		waste = zspage_size % class_size;
		usedpc = (zspage_size - waste) * 100 / zspage_size;

		if (usedpc > max_usedpc) {
			max_usedpc = usedpc;
			max_usedpc_order = i;
		}
	}

	return max_usedpc_order;
}

/*
 * A single 'zspage' is composed of many system pages which are
 * linked together using fields in struct page. This function finds
 * the first/head page, given any component page of a zspage.
 */
static struct page *get_first_page(struct page *page)
{
	if (is_first_page(page))
		return page;
	else
		return page->first_page;
=======
static struct zspage *get_zspage(struct page *page)
{
	struct zspage *zspage = (struct zspage *)page_private(page);

	BUG_ON(zspage->magic != ZSPAGE_MAGIC);
	return zspage;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct page *get_next_page(struct page *page)
{
<<<<<<< HEAD
	struct page *next;

	if (is_last_page(page))
		next = NULL;
	else if (is_first_page(page))
		next = (struct page *)page_private(page);
	else
		next = list_entry(page->lru.next, struct page, lru);

	return next;
}

/*
 * Encode <page, obj_idx> as a single handle value.
 * On hardware platforms with physical memory starting at 0x0 the pfn
 * could be 0 so we ensure that the handle will never be 0 by adjusting the
 * encoded obj_idx value before encoding.
 */
static void *obj_location_to_handle(struct page *page, unsigned long obj_idx)
{
	unsigned long handle;

	if (!page) {
		BUG_ON(obj_idx);
		return NULL;
	}

	handle = page_to_pfn(page) << OBJ_INDEX_BITS;
	handle |= ((obj_idx + 1) & OBJ_INDEX_MASK);

	return (void *)handle;
}

/*
 * Decode <page, obj_idx> pair from the given object handle. We adjust the
 * decoded obj_idx back to its original value since it was adjusted in
 * obj_location_to_handle().
 */
static void obj_handle_to_location(unsigned long handle, struct page **page,
				unsigned long *obj_idx)
{
	*page = pfn_to_page(handle >> OBJ_INDEX_BITS);
	*obj_idx = (handle & OBJ_INDEX_MASK) - 1;
}

static unsigned long obj_idx_to_offset(struct page *page,
				unsigned long obj_idx, int class_size)
{
	unsigned long off = 0;

	if (!is_first_page(page))
		off = page->index;

	return off + obj_idx * class_size;
=======
	struct zspage *zspage = get_zspage(page);

	if (unlikely(ZsHugePage(zspage)))
		return NULL;

	return (struct page *)page->index;
}

/**
 * obj_to_location - get (<page>, <obj_idx>) from encoded object value
 * @obj: the encoded object value
 * @page: page object resides in zspage
 * @obj_idx: object index
 */
static void obj_to_location(unsigned long obj, struct page **page,
				unsigned int *obj_idx)
{
	*page = pfn_to_page(obj >> OBJ_INDEX_BITS);
	*obj_idx = (obj & OBJ_INDEX_MASK);
}

static void obj_to_page(unsigned long obj, struct page **page)
{
	*page = pfn_to_page(obj >> OBJ_INDEX_BITS);
}

/**
 * location_to_obj - get obj value encoded from (<page>, <obj_idx>)
 * @page: page object resides in zspage
 * @obj_idx: object index
 */
static unsigned long location_to_obj(struct page *page, unsigned int obj_idx)
{
	unsigned long obj;

	obj = page_to_pfn(page) << OBJ_INDEX_BITS;
	obj |= obj_idx & OBJ_INDEX_MASK;

	return obj;
}

static unsigned long handle_to_obj(unsigned long handle)
{
	return *(unsigned long *)handle;
}

static inline bool obj_allocated(struct page *page, void *obj,
				 unsigned long *phandle)
{
	unsigned long handle;
	struct zspage *zspage = get_zspage(page);

	if (unlikely(ZsHugePage(zspage))) {
		VM_BUG_ON_PAGE(!is_first_page(page), page);
		handle = page->index;
	} else
		handle = *(unsigned long *)obj;

	if (!(handle & OBJ_ALLOCATED_TAG))
		return false;

	/* Clear all tags before returning the handle */
	*phandle = handle & ~OBJ_TAG_MASK;
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void reset_page(struct page *page)
{
<<<<<<< HEAD
	clear_bit(PG_private, &page->flags);
	clear_bit(PG_private_2, &page->flags);
	set_page_private(page, 0);
	page->mapping = NULL;
	page->freelist = NULL;
	reset_page_mapcount(page);
}

static void free_zspage(struct page *first_page)
{
	struct page *nextp, *tmp, *head_extra;

	BUG_ON(!is_first_page(first_page));
	BUG_ON(first_page->inuse);

	head_extra = (struct page *)page_private(first_page);

	reset_page(first_page);
	__free_page(first_page);

	/* zspage with only 1 system page */
	if (!head_extra)
		return;

	list_for_each_entry_safe(nextp, tmp, &head_extra->lru, lru) {
		list_del(&nextp->lru);
		reset_page(nextp);
		__free_page(nextp);
	}
	reset_page(head_extra);
	__free_page(head_extra);
}

/* Initialize a newly allocated zspage */
static void init_zspage(struct page *first_page, struct size_class *class)
{
	unsigned long off = 0;
	struct page *page = first_page;

	BUG_ON(!is_first_page(first_page));
	while (page) {
		struct page *next_page;
		struct link_free *link;
		unsigned int i, objs_on_page;

		/*
		 * page->index stores offset of first object starting
		 * in the page. For the first page, this is always 0,
		 * so we use first_page->index (aka ->freelist) to store
		 * head of corresponding zspage's freelist.
		 */
		if (page != first_page)
			page->index = off;

		link = (struct link_free *)kmap_atomic(page) +
						off / sizeof(*link);
		objs_on_page = (PAGE_SIZE - off) / class->size;

		for (i = 1; i <= objs_on_page; i++) {
			off += class->size;
			if (off < PAGE_SIZE) {
				link->next = obj_location_to_handle(page, i);
				link += class->size / sizeof(*link);
			}
=======
	__ClearPageMovable(page);
	ClearPagePrivate(page);
	set_page_private(page, 0);
	page_mapcount_reset(page);
	page->index = 0;
}

static int trylock_zspage(struct zspage *zspage)
{
	struct page *cursor, *fail;

	for (cursor = get_first_page(zspage); cursor != NULL; cursor =
					get_next_page(cursor)) {
		if (!trylock_page(cursor)) {
			fail = cursor;
			goto unlock;
		}
	}

	return 1;
unlock:
	for (cursor = get_first_page(zspage); cursor != fail; cursor =
					get_next_page(cursor))
		unlock_page(cursor);

	return 0;
}

static void __free_zspage(struct zs_pool *pool, struct size_class *class,
				struct zspage *zspage)
{
	struct page *page, *next;

	assert_spin_locked(&pool->lock);

	VM_BUG_ON(get_zspage_inuse(zspage));
	VM_BUG_ON(zspage->fullness != ZS_INUSE_RATIO_0);

	next = page = get_first_page(zspage);
	do {
		VM_BUG_ON_PAGE(!PageLocked(page), page);
		next = get_next_page(page);
		reset_page(page);
		unlock_page(page);
		dec_zone_page_state(page, NR_ZSPAGES);
		put_page(page);
		page = next;
	} while (page != NULL);

	cache_free_zspage(pool, zspage);

	class_stat_dec(class, ZS_OBJS_ALLOCATED, class->objs_per_zspage);
	atomic_long_sub(class->pages_per_zspage, &pool->pages_allocated);
}

static void free_zspage(struct zs_pool *pool, struct size_class *class,
				struct zspage *zspage)
{
	VM_BUG_ON(get_zspage_inuse(zspage));
	VM_BUG_ON(list_empty(&zspage->list));

	/*
	 * Since zs_free couldn't be sleepable, this function cannot call
	 * lock_page. The page locks trylock_zspage got will be released
	 * by __free_zspage.
	 */
	if (!trylock_zspage(zspage)) {
		kick_deferred_free(pool);
		return;
	}

	remove_zspage(class, zspage);
	__free_zspage(pool, class, zspage);
}

/* Initialize a newly allocated zspage */
static void init_zspage(struct size_class *class, struct zspage *zspage)
{
	unsigned int freeobj = 1;
	unsigned long off = 0;
	struct page *page = get_first_page(zspage);

	while (page) {
		struct page *next_page;
		struct link_free *link;
		void *vaddr;

		set_first_obj_offset(page, off);

		vaddr = kmap_atomic(page);
		link = (struct link_free *)vaddr + off / sizeof(*link);

		while ((off += class->size) < PAGE_SIZE) {
			link->next = freeobj++ << OBJ_TAG_BITS;
			link += class->size / sizeof(*link);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		/*
		 * We now come to the last (full or partial) object on this
		 * page, which must point to the first object on the next
		 * page (if present)
		 */
		next_page = get_next_page(page);
<<<<<<< HEAD
		link->next = obj_location_to_handle(next_page, 0);
		kunmap_atomic(link);
		page = next_page;
		off = (off + class->size) % PAGE_SIZE;
=======
		if (next_page) {
			link->next = freeobj++ << OBJ_TAG_BITS;
		} else {
			/*
			 * Reset OBJ_TAG_BITS bit to last link to tell
			 * whether it's allocated object or not.
			 */
			link->next = -1UL << OBJ_TAG_BITS;
		}
		kunmap_atomic(vaddr);
		page = next_page;
		off %= PAGE_SIZE;
	}

	set_freeobj(zspage, 0);
}

static void create_page_chain(struct size_class *class, struct zspage *zspage,
				struct page *pages[])
{
	int i;
	struct page *page;
	struct page *prev_page = NULL;
	int nr_pages = class->pages_per_zspage;

	/*
	 * Allocate individual pages and link them together as:
	 * 1. all pages are linked together using page->index
	 * 2. each sub-page point to zspage using page->private
	 *
	 * we set PG_private to identify the first page (i.e. no other sub-page
	 * has this flag set).
	 */
	for (i = 0; i < nr_pages; i++) {
		page = pages[i];
		set_page_private(page, (unsigned long)zspage);
		page->index = 0;
		if (i == 0) {
			zspage->first_page = page;
			SetPagePrivate(page);
			if (unlikely(class->objs_per_zspage == 1 &&
					class->pages_per_zspage == 1))
				SetZsHugePage(zspage);
		} else {
			prev_page->index = (unsigned long)page;
		}
		prev_page = page;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * Allocate a zspage for the given size class
 */
<<<<<<< HEAD
static struct page *alloc_zspage(struct size_class *class, gfp_t flags)
{
	int i, error;
	struct page *first_page = NULL, *uninitialized_var(prev_page);

	/*
	 * Allocate individual pages and link them together as:
	 * 1. first page->private = first sub-page
	 * 2. all sub-pages are linked together using page->lru
	 * 3. each sub-page is linked to the first page using page->first_page
	 *
	 * For each size class, First/Head pages are linked together using
	 * page->lru. Also, we set PG_private to identify the first page
	 * (i.e. no other sub-page has this flag set) and PG_private_2 to
	 * identify the last page.
	 */
	error = -ENOMEM;
	for (i = 0; i < class->pages_per_zspage; i++) {
		struct page *page;

		page = alloc_page(flags);
		if (!page)
			goto cleanup;

		INIT_LIST_HEAD(&page->lru);
		if (i == 0) {	/* first page */
			SetPagePrivate(page);
			set_page_private(page, 0);
			first_page = page;
			first_page->inuse = 0;
		}
		if (i == 1)
			set_page_private(first_page, (unsigned long)page);
		if (i >= 1)
			page->first_page = first_page;
		if (i >= 2)
			list_add(&page->lru, &prev_page->lru);
		if (i == class->pages_per_zspage - 1)	/* last page */
			SetPagePrivate2(page);
		prev_page = page;
	}

	init_zspage(first_page, class);

	first_page->freelist = obj_location_to_handle(first_page, 0);
	/* Maximum number of objects we can store in this zspage */
	first_page->objects = class->pages_per_zspage * PAGE_SIZE / class->size;

	error = 0; /* Success */

cleanup:
	if (unlikely(error) && first_page) {
		free_zspage(first_page);
		first_page = NULL;
	}

	return first_page;
}

static struct page *find_get_zspage(struct size_class *class)
{
	int i;
	struct page *page;

	for (i = 0; i < _ZS_NR_FULLNESS_GROUPS; i++) {
		page = class->fullness_list[i];
		if (page)
			break;
	}

	return page;
}

#ifdef USE_PGTABLE_MAPPING
static inline int __zs_cpu_up(struct mapping_area *area)
{
	/*
	 * Make sure we don't leak memory if a cpu UP notification
	 * and zs_init() race and both call zs_cpu_up() on the same cpu
	 */
	if (area->vm)
		return 0;
	area->vm = alloc_vm_area(PAGE_SIZE * 2, NULL);
	if (!area->vm)
		return -ENOMEM;
	return 0;
}

static inline void __zs_cpu_down(struct mapping_area *area)
{
	if (area->vm)
		free_vm_area(area->vm);
	area->vm = NULL;
}

static inline void *__zs_map_object(struct mapping_area *area,
				struct page *pages[2], int off, int size)
{
	BUG_ON(map_vm_area(area->vm, PAGE_KERNEL, &pages));
	area->vm_addr = area->vm->addr;
	return area->vm_addr + off;
}

static inline void __zs_unmap_object(struct mapping_area *area,
				struct page *pages[2], int off, int size)
{
	unsigned long addr = (unsigned long)area->vm_addr;

	unmap_kernel_range(addr, PAGE_SIZE * 2);
}

#else /* USE_PGTABLE_MAPPING */

=======
static struct zspage *alloc_zspage(struct zs_pool *pool,
					struct size_class *class,
					gfp_t gfp)
{
	int i;
	struct page *pages[ZS_MAX_PAGES_PER_ZSPAGE];
	struct zspage *zspage = cache_alloc_zspage(pool, gfp);

	if (!zspage)
		return NULL;

	zspage->magic = ZSPAGE_MAGIC;
	migrate_lock_init(zspage);

	for (i = 0; i < class->pages_per_zspage; i++) {
		struct page *page;

		page = alloc_page(gfp);
		if (!page) {
			while (--i >= 0) {
				dec_zone_page_state(pages[i], NR_ZSPAGES);
				__free_page(pages[i]);
			}
			cache_free_zspage(pool, zspage);
			return NULL;
		}

		inc_zone_page_state(page, NR_ZSPAGES);
		pages[i] = page;
	}

	create_page_chain(class, zspage, pages);
	init_zspage(class, zspage);
	zspage->pool = pool;
	zspage->class = class->index;

	return zspage;
}

static struct zspage *find_get_zspage(struct size_class *class)
{
	int i;
	struct zspage *zspage;

	for (i = ZS_INUSE_RATIO_99; i >= ZS_INUSE_RATIO_0; i--) {
		zspage = list_first_entry_or_null(&class->fullness_list[i],
						  struct zspage, list);
		if (zspage)
			break;
	}

	return zspage;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int __zs_cpu_up(struct mapping_area *area)
{
	/*
	 * Make sure we don't leak memory if a cpu UP notification
	 * and zs_init() race and both call zs_cpu_up() on the same cpu
	 */
	if (area->vm_buf)
		return 0;
<<<<<<< HEAD
	area->vm_buf = (char *)__get_free_page(GFP_KERNEL);
=======
	area->vm_buf = kmalloc(ZS_MAX_ALLOC_SIZE, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!area->vm_buf)
		return -ENOMEM;
	return 0;
}

static inline void __zs_cpu_down(struct mapping_area *area)
{
<<<<<<< HEAD
	if (area->vm_buf)
		free_page((unsigned long)area->vm_buf);
=======
	kfree(area->vm_buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	area->vm_buf = NULL;
}

static void *__zs_map_object(struct mapping_area *area,
			struct page *pages[2], int off, int size)
{
	int sizes[2];
	void *addr;
	char *buf = area->vm_buf;

	/* disable page faults to match kmap_atomic() return conditions */
	pagefault_disable();

	/* no read fastpath */
	if (area->vm_mm == ZS_MM_WO)
		goto out;

	sizes[0] = PAGE_SIZE - off;
	sizes[1] = size - sizes[0];

	/* copy object to per-cpu buffer */
	addr = kmap_atomic(pages[0]);
	memcpy(buf, addr + off, sizes[0]);
	kunmap_atomic(addr);
	addr = kmap_atomic(pages[1]);
	memcpy(buf + sizes[0], addr, sizes[1]);
	kunmap_atomic(addr);
out:
	return area->vm_buf;
}

static void __zs_unmap_object(struct mapping_area *area,
			struct page *pages[2], int off, int size)
{
	int sizes[2];
	void *addr;
<<<<<<< HEAD
	char *buf = area->vm_buf;
=======
	char *buf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* no write fastpath */
	if (area->vm_mm == ZS_MM_RO)
		goto out;

<<<<<<< HEAD
=======
	buf = area->vm_buf;
	buf = buf + ZS_HANDLE_SIZE;
	size -= ZS_HANDLE_SIZE;
	off += ZS_HANDLE_SIZE;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sizes[0] = PAGE_SIZE - off;
	sizes[1] = size - sizes[0];

	/* copy per-cpu buffer to object */
	addr = kmap_atomic(pages[0]);
	memcpy(addr + off, buf, sizes[0]);
	kunmap_atomic(addr);
	addr = kmap_atomic(pages[1]);
	memcpy(addr, buf + sizes[0], sizes[1]);
	kunmap_atomic(addr);

out:
	/* enable page faults to match kunmap_atomic() return conditions */
	pagefault_enable();
}

<<<<<<< HEAD
#endif /* USE_PGTABLE_MAPPING */

static int zs_cpu_notifier(struct notifier_block *nb, unsigned long action,
				void *pcpu)
{
	int ret, cpu = (long)pcpu;
	struct mapping_area *area;

	switch (action) {
	case CPU_UP_PREPARE:
		area = &per_cpu(zs_map_area, cpu);
		ret = __zs_cpu_up(area);
		if (ret)
			return notifier_from_errno(ret);
		break;
	case CPU_DEAD:
	case CPU_UP_CANCELED:
		area = &per_cpu(zs_map_area, cpu);
		__zs_cpu_down(area);
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block zs_cpu_nb = {
	.notifier_call = zs_cpu_notifier
};

static void zs_exit(void)
{
	int cpu;

	for_each_online_cpu(cpu)
		zs_cpu_notifier(NULL, CPU_DEAD, (void *)(long)cpu);
	unregister_cpu_notifier(&zs_cpu_nb);
}

static int zs_init(void)
{
	int cpu, ret;

	register_cpu_notifier(&zs_cpu_nb);
	for_each_online_cpu(cpu) {
		ret = zs_cpu_notifier(NULL, CPU_UP_PREPARE, (void *)(long)cpu);
		if (notifier_to_errno(ret)) {
			goto fail;
		}
	}
	return 0;
fail:
	zs_exit();
	return notifier_to_errno(ret);
}

/**
 * zs_create_pool - Creates an allocation pool to work from.
 * @flags: allocation flags used to allocate pool metadata
 *
 * This function must be called before anything when using
 * the zsmalloc allocator.
 *
 * On success, a pointer to the newly created pool is returned,
 * otherwise NULL.
 */
struct zs_pool *zs_create_pool(gfp_t flags)
{
	int i, ovhd_size;
	struct zs_pool *pool;

	ovhd_size = roundup(sizeof(*pool), PAGE_SIZE);
	pool = kzalloc(ovhd_size, GFP_KERNEL);
	if (!pool)
		return NULL;

	for (i = 0; i < ZS_SIZE_CLASSES; i++) {
		int size;
		struct size_class *class;

		size = ZS_MIN_ALLOC_SIZE + i * ZS_SIZE_CLASS_DELTA;
		if (size > ZS_MAX_ALLOC_SIZE)
			size = ZS_MAX_ALLOC_SIZE;

		class = &pool->size_class[i];
		class->size = size;
		class->index = i;
		spin_lock_init(&class->lock);
		class->pages_per_zspage = get_pages_per_zspage(size);

	}

	pool->flags = flags;

	return pool;
}
EXPORT_SYMBOL_GPL(zs_create_pool);

void zs_destroy_pool(struct zs_pool *pool)
{
	int i;

	for (i = 0; i < ZS_SIZE_CLASSES; i++) {
		int fg;
		struct size_class *class = &pool->size_class[i];

		for (fg = 0; fg < _ZS_NR_FULLNESS_GROUPS; fg++) {
			if (class->fullness_list[fg]) {
				pr_info("Freeing non-empty class with size %db, fullness group %d\n",
					class->size, fg);
			}
		}
	}
	kfree(pool);
}
EXPORT_SYMBOL_GPL(zs_destroy_pool);

/**
 * zs_malloc - Allocate block of given size from pool.
 * @pool: pool to allocate from
 * @size: size of block to allocate
 *
 * On success, handle to the allocated object is returned,
 * otherwise 0.
 * Allocation requests with size > ZS_MAX_ALLOC_SIZE will fail.
 */
unsigned long zs_malloc(struct zs_pool *pool, size_t size)
{
	unsigned long obj;
	struct link_free *link;
	int class_idx;
	struct size_class *class;

	struct page *first_page, *m_page;
	unsigned long m_objidx, m_offset;

	if (unlikely(!size || size > ZS_MAX_ALLOC_SIZE))
		return 0;

	class_idx = get_size_class_index(size);
	class = &pool->size_class[class_idx];
	BUG_ON(class_idx != class->index);

	spin_lock(&class->lock);
	first_page = find_get_zspage(class);

	if (!first_page) {
		spin_unlock(&class->lock);
		first_page = alloc_zspage(class, pool->flags);
		if (unlikely(!first_page))
			return 0;

		set_zspage_mapping(first_page, class->index, ZS_EMPTY);
		spin_lock(&class->lock);
		class->pages_allocated += class->pages_per_zspage;
	}

	obj = (unsigned long)first_page->freelist;
	obj_handle_to_location(obj, &m_page, &m_objidx);
	m_offset = obj_idx_to_offset(m_page, m_objidx, class->size);

	link = (struct link_free *)kmap_atomic(m_page) +
					m_offset / sizeof(*link);
	first_page->freelist = link->next;
	memset(link, POISON_INUSE, sizeof(*link));
	kunmap_atomic(link);

	first_page->inuse++;
	/* Now move the zspage to another fullness group, if required */
	fix_fullness_group(pool, first_page);
	spin_unlock(&class->lock);

	return obj;
}
EXPORT_SYMBOL_GPL(zs_malloc);

void zs_free(struct zs_pool *pool, unsigned long obj)
{
	struct link_free *link;
	struct page *first_page, *f_page;
	unsigned long f_objidx, f_offset;

	int class_idx;
	struct size_class *class;
	enum fullness_group fullness;

	if (unlikely(!obj))
		return;

	obj_handle_to_location(obj, &f_page, &f_objidx);
	first_page = get_first_page(f_page);

	get_zspage_mapping(first_page, &class_idx, &fullness);
	class = &pool->size_class[class_idx];
	f_offset = obj_idx_to_offset(f_page, f_objidx, class->size);

	spin_lock(&class->lock);

	/* Insert this object in containing zspage's freelist */
	link = (struct link_free *)((unsigned char *)kmap_atomic(f_page)
							+ f_offset);
	link->next = first_page->freelist;
	kunmap_atomic(link);
	first_page->freelist = (void *)obj;

	first_page->inuse--;
	fullness = fix_fullness_group(pool, first_page);

	if (fullness == ZS_EMPTY)
		class->pages_allocated -= class->pages_per_zspage;

	spin_unlock(&class->lock);

	if (fullness == ZS_EMPTY)
		free_zspage(first_page);
}
EXPORT_SYMBOL_GPL(zs_free);
=======
static int zs_cpu_prepare(unsigned int cpu)
{
	struct mapping_area *area;

	area = &per_cpu(zs_map_area, cpu);
	return __zs_cpu_up(area);
}

static int zs_cpu_dead(unsigned int cpu)
{
	struct mapping_area *area;

	area = &per_cpu(zs_map_area, cpu);
	__zs_cpu_down(area);
	return 0;
}

static bool can_merge(struct size_class *prev, int pages_per_zspage,
					int objs_per_zspage)
{
	if (prev->pages_per_zspage == pages_per_zspage &&
		prev->objs_per_zspage == objs_per_zspage)
		return true;

	return false;
}

static bool zspage_full(struct size_class *class, struct zspage *zspage)
{
	return get_zspage_inuse(zspage) == class->objs_per_zspage;
}

static bool zspage_empty(struct zspage *zspage)
{
	return get_zspage_inuse(zspage) == 0;
}

/**
 * zs_lookup_class_index() - Returns index of the zsmalloc &size_class
 * that hold objects of the provided size.
 * @pool: zsmalloc pool to use
 * @size: object size
 *
 * Context: Any context.
 *
 * Return: the index of the zsmalloc &size_class that hold objects of the
 * provided size.
 */
unsigned int zs_lookup_class_index(struct zs_pool *pool, unsigned int size)
{
	struct size_class *class;

	class = pool->size_class[get_size_class_index(size)];

	return class->index;
}
EXPORT_SYMBOL_GPL(zs_lookup_class_index);

unsigned long zs_get_total_pages(struct zs_pool *pool)
{
	return atomic_long_read(&pool->pages_allocated);
}
EXPORT_SYMBOL_GPL(zs_get_total_pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * zs_map_object - get address of allocated object from handle.
 * @pool: pool from which the object was allocated
 * @handle: handle returned from zs_malloc
<<<<<<< HEAD
=======
 * @mm: mapping mode to use
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Before using an object allocated from zs_malloc, it must be mapped using
 * this function. When done with the object, it must be unmapped using
 * zs_unmap_object.
 *
 * Only one object can be mapped per cpu at a time. There is no protection
 * against nested mappings.
 *
 * This function returns with preemption and page faults disabled.
 */
void *zs_map_object(struct zs_pool *pool, unsigned long handle,
			enum zs_mapmode mm)
{
<<<<<<< HEAD
	struct page *page;
	unsigned long obj_idx, off;

	unsigned int class_idx;
	enum fullness_group fg;
	struct size_class *class;
	struct mapping_area *area;
	struct page *pages[2];

	BUG_ON(!handle);
=======
	struct zspage *zspage;
	struct page *page;
	unsigned long obj, off;
	unsigned int obj_idx;

	struct size_class *class;
	struct mapping_area *area;
	struct page *pages[2];
	void *ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Because we use per-cpu mapping areas shared among the
	 * pools/users, we can't allow mapping in interrupt context
	 * because it can corrupt another users mappings.
	 */
	BUG_ON(in_interrupt());

<<<<<<< HEAD
	obj_handle_to_location(handle, &page, &obj_idx);
	get_zspage_mapping(get_first_page(page), &class_idx, &fg);
	class = &pool->size_class[class_idx];
	off = obj_idx_to_offset(page, obj_idx, class->size);

	area = &get_cpu_var(zs_map_area);
=======
	/* It guarantees it can get zspage from handle safely */
	spin_lock(&pool->lock);
	obj = handle_to_obj(handle);
	obj_to_location(obj, &page, &obj_idx);
	zspage = get_zspage(page);

	/*
	 * migration cannot move any zpages in this zspage. Here, pool->lock
	 * is too heavy since callers would take some time until they calls
	 * zs_unmap_object API so delegate the locking from class to zspage
	 * which is smaller granularity.
	 */
	migrate_read_lock(zspage);
	spin_unlock(&pool->lock);

	class = zspage_class(pool, zspage);
	off = offset_in_page(class->size * obj_idx);

	local_lock(&zs_map_area.lock);
	area = this_cpu_ptr(&zs_map_area);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	area->vm_mm = mm;
	if (off + class->size <= PAGE_SIZE) {
		/* this object is contained entirely within a page */
		area->vm_addr = kmap_atomic(page);
<<<<<<< HEAD
		return area->vm_addr + off;
=======
		ret = area->vm_addr + off;
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* this object spans two pages */
	pages[0] = page;
	pages[1] = get_next_page(page);
	BUG_ON(!pages[1]);

<<<<<<< HEAD
	return __zs_map_object(area, pages, off, class->size);
=======
	ret = __zs_map_object(area, pages, off, class->size);
out:
	if (likely(!ZsHugePage(zspage)))
		ret += ZS_HANDLE_SIZE;

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(zs_map_object);

void zs_unmap_object(struct zs_pool *pool, unsigned long handle)
{
<<<<<<< HEAD
	struct page *page;
	unsigned long obj_idx, off;

	unsigned int class_idx;
	enum fullness_group fg;
	struct size_class *class;
	struct mapping_area *area;

	BUG_ON(!handle);

	obj_handle_to_location(handle, &page, &obj_idx);
	get_zspage_mapping(get_first_page(page), &class_idx, &fg);
	class = &pool->size_class[class_idx];
	off = obj_idx_to_offset(page, obj_idx, class->size);

	area = &__get_cpu_var(zs_map_area);
=======
	struct zspage *zspage;
	struct page *page;
	unsigned long obj, off;
	unsigned int obj_idx;

	struct size_class *class;
	struct mapping_area *area;

	obj = handle_to_obj(handle);
	obj_to_location(obj, &page, &obj_idx);
	zspage = get_zspage(page);
	class = zspage_class(pool, zspage);
	off = offset_in_page(class->size * obj_idx);

	area = this_cpu_ptr(&zs_map_area);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (off + class->size <= PAGE_SIZE)
		kunmap_atomic(area->vm_addr);
	else {
		struct page *pages[2];

		pages[0] = page;
		pages[1] = get_next_page(page);
		BUG_ON(!pages[1]);

		__zs_unmap_object(area, pages, off, class->size);
	}
<<<<<<< HEAD
	put_cpu_var(zs_map_area);
}
EXPORT_SYMBOL_GPL(zs_unmap_object);

u64 zs_get_total_size_bytes(struct zs_pool *pool)
{
	int i;
	u64 npages = 0;

	for (i = 0; i < ZS_SIZE_CLASSES; i++)
		npages += pool->size_class[i].pages_allocated;

	return npages << PAGE_SHIFT;
}
EXPORT_SYMBOL_GPL(zs_get_total_size_bytes);
=======
	local_unlock(&zs_map_area.lock);

	migrate_read_unlock(zspage);
}
EXPORT_SYMBOL_GPL(zs_unmap_object);

/**
 * zs_huge_class_size() - Returns the size (in bytes) of the first huge
 *                        zsmalloc &size_class.
 * @pool: zsmalloc pool to use
 *
 * The function returns the size of the first huge class - any object of equal
 * or bigger size will be stored in zspage consisting of a single physical
 * page.
 *
 * Context: Any context.
 *
 * Return: the size (in bytes) of the first huge zsmalloc &size_class.
 */
size_t zs_huge_class_size(struct zs_pool *pool)
{
	return huge_class_size;
}
EXPORT_SYMBOL_GPL(zs_huge_class_size);

static unsigned long obj_malloc(struct zs_pool *pool,
				struct zspage *zspage, unsigned long handle)
{
	int i, nr_page, offset;
	unsigned long obj;
	struct link_free *link;
	struct size_class *class;

	struct page *m_page;
	unsigned long m_offset;
	void *vaddr;

	class = pool->size_class[zspage->class];
	handle |= OBJ_ALLOCATED_TAG;
	obj = get_freeobj(zspage);

	offset = obj * class->size;
	nr_page = offset >> PAGE_SHIFT;
	m_offset = offset_in_page(offset);
	m_page = get_first_page(zspage);

	for (i = 0; i < nr_page; i++)
		m_page = get_next_page(m_page);

	vaddr = kmap_atomic(m_page);
	link = (struct link_free *)vaddr + m_offset / sizeof(*link);
	set_freeobj(zspage, link->next >> OBJ_TAG_BITS);
	if (likely(!ZsHugePage(zspage)))
		/* record handle in the header of allocated chunk */
		link->handle = handle;
	else
		/* record handle to page->index */
		zspage->first_page->index = handle;

	kunmap_atomic(vaddr);
	mod_zspage_inuse(zspage, 1);

	obj = location_to_obj(m_page, obj);

	return obj;
}


/**
 * zs_malloc - Allocate block of given size from pool.
 * @pool: pool to allocate from
 * @size: size of block to allocate
 * @gfp: gfp flags when allocating object
 *
 * On success, handle to the allocated object is returned,
 * otherwise an ERR_PTR().
 * Allocation requests with size > ZS_MAX_ALLOC_SIZE will fail.
 */
unsigned long zs_malloc(struct zs_pool *pool, size_t size, gfp_t gfp)
{
	unsigned long handle, obj;
	struct size_class *class;
	int newfg;
	struct zspage *zspage;

	if (unlikely(!size))
		return (unsigned long)ERR_PTR(-EINVAL);

	if (unlikely(size > ZS_MAX_ALLOC_SIZE))
		return (unsigned long)ERR_PTR(-ENOSPC);

	handle = cache_alloc_handle(pool, gfp);
	if (!handle)
		return (unsigned long)ERR_PTR(-ENOMEM);

	/* extra space in chunk to keep the handle */
	size += ZS_HANDLE_SIZE;
	class = pool->size_class[get_size_class_index(size)];

	/* pool->lock effectively protects the zpage migration */
	spin_lock(&pool->lock);
	zspage = find_get_zspage(class);
	if (likely(zspage)) {
		obj = obj_malloc(pool, zspage, handle);
		/* Now move the zspage to another fullness group, if required */
		fix_fullness_group(class, zspage);
		record_obj(handle, obj);
		class_stat_inc(class, ZS_OBJS_INUSE, 1);

		goto out;
	}

	spin_unlock(&pool->lock);

	zspage = alloc_zspage(pool, class, gfp);
	if (!zspage) {
		cache_free_handle(pool, handle);
		return (unsigned long)ERR_PTR(-ENOMEM);
	}

	spin_lock(&pool->lock);
	obj = obj_malloc(pool, zspage, handle);
	newfg = get_fullness_group(class, zspage);
	insert_zspage(class, zspage, newfg);
	record_obj(handle, obj);
	atomic_long_add(class->pages_per_zspage, &pool->pages_allocated);
	class_stat_inc(class, ZS_OBJS_ALLOCATED, class->objs_per_zspage);
	class_stat_inc(class, ZS_OBJS_INUSE, 1);

	/* We completely set up zspage so mark them as movable */
	SetZsPageMovable(pool, zspage);
out:
	spin_unlock(&pool->lock);

	return handle;
}
EXPORT_SYMBOL_GPL(zs_malloc);

static void obj_free(int class_size, unsigned long obj)
{
	struct link_free *link;
	struct zspage *zspage;
	struct page *f_page;
	unsigned long f_offset;
	unsigned int f_objidx;
	void *vaddr;

	obj_to_location(obj, &f_page, &f_objidx);
	f_offset = offset_in_page(class_size * f_objidx);
	zspage = get_zspage(f_page);

	vaddr = kmap_atomic(f_page);
	link = (struct link_free *)(vaddr + f_offset);

	/* Insert this object in containing zspage's freelist */
	if (likely(!ZsHugePage(zspage)))
		link->next = get_freeobj(zspage) << OBJ_TAG_BITS;
	else
		f_page->index = 0;
	set_freeobj(zspage, f_objidx);

	kunmap_atomic(vaddr);
	mod_zspage_inuse(zspage, -1);
}

void zs_free(struct zs_pool *pool, unsigned long handle)
{
	struct zspage *zspage;
	struct page *f_page;
	unsigned long obj;
	struct size_class *class;
	int fullness;

	if (IS_ERR_OR_NULL((void *)handle))
		return;

	/*
	 * The pool->lock protects the race with zpage's migration
	 * so it's safe to get the page from handle.
	 */
	spin_lock(&pool->lock);
	obj = handle_to_obj(handle);
	obj_to_page(obj, &f_page);
	zspage = get_zspage(f_page);
	class = zspage_class(pool, zspage);

	class_stat_dec(class, ZS_OBJS_INUSE, 1);
	obj_free(class->size, obj);

	fullness = fix_fullness_group(class, zspage);
	if (fullness == ZS_INUSE_RATIO_0)
		free_zspage(pool, class, zspage);

	spin_unlock(&pool->lock);
	cache_free_handle(pool, handle);
}
EXPORT_SYMBOL_GPL(zs_free);

static void zs_object_copy(struct size_class *class, unsigned long dst,
				unsigned long src)
{
	struct page *s_page, *d_page;
	unsigned int s_objidx, d_objidx;
	unsigned long s_off, d_off;
	void *s_addr, *d_addr;
	int s_size, d_size, size;
	int written = 0;

	s_size = d_size = class->size;

	obj_to_location(src, &s_page, &s_objidx);
	obj_to_location(dst, &d_page, &d_objidx);

	s_off = offset_in_page(class->size * s_objidx);
	d_off = offset_in_page(class->size * d_objidx);

	if (s_off + class->size > PAGE_SIZE)
		s_size = PAGE_SIZE - s_off;

	if (d_off + class->size > PAGE_SIZE)
		d_size = PAGE_SIZE - d_off;

	s_addr = kmap_atomic(s_page);
	d_addr = kmap_atomic(d_page);

	while (1) {
		size = min(s_size, d_size);
		memcpy(d_addr + d_off, s_addr + s_off, size);
		written += size;

		if (written == class->size)
			break;

		s_off += size;
		s_size -= size;
		d_off += size;
		d_size -= size;

		/*
		 * Calling kunmap_atomic(d_addr) is necessary. kunmap_atomic()
		 * calls must occurs in reverse order of calls to kmap_atomic().
		 * So, to call kunmap_atomic(s_addr) we should first call
		 * kunmap_atomic(d_addr). For more details see
		 * Documentation/mm/highmem.rst.
		 */
		if (s_off >= PAGE_SIZE) {
			kunmap_atomic(d_addr);
			kunmap_atomic(s_addr);
			s_page = get_next_page(s_page);
			s_addr = kmap_atomic(s_page);
			d_addr = kmap_atomic(d_page);
			s_size = class->size - written;
			s_off = 0;
		}

		if (d_off >= PAGE_SIZE) {
			kunmap_atomic(d_addr);
			d_page = get_next_page(d_page);
			d_addr = kmap_atomic(d_page);
			d_size = class->size - written;
			d_off = 0;
		}
	}

	kunmap_atomic(d_addr);
	kunmap_atomic(s_addr);
}

/*
 * Find alloced object in zspage from index object and
 * return handle.
 */
static unsigned long find_alloced_obj(struct size_class *class,
				      struct page *page, int *obj_idx)
{
	unsigned int offset;
	int index = *obj_idx;
	unsigned long handle = 0;
	void *addr = kmap_atomic(page);

	offset = get_first_obj_offset(page);
	offset += class->size * index;

	while (offset < PAGE_SIZE) {
		if (obj_allocated(page, addr + offset, &handle))
			break;

		offset += class->size;
		index++;
	}

	kunmap_atomic(addr);

	*obj_idx = index;

	return handle;
}

static void migrate_zspage(struct zs_pool *pool, struct zspage *src_zspage,
			   struct zspage *dst_zspage)
{
	unsigned long used_obj, free_obj;
	unsigned long handle;
	int obj_idx = 0;
	struct page *s_page = get_first_page(src_zspage);
	struct size_class *class = pool->size_class[src_zspage->class];

	while (1) {
		handle = find_alloced_obj(class, s_page, &obj_idx);
		if (!handle) {
			s_page = get_next_page(s_page);
			if (!s_page)
				break;
			obj_idx = 0;
			continue;
		}

		used_obj = handle_to_obj(handle);
		free_obj = obj_malloc(pool, dst_zspage, handle);
		zs_object_copy(class, free_obj, used_obj);
		obj_idx++;
		record_obj(handle, free_obj);
		obj_free(class->size, used_obj);

		/* Stop if there is no more space */
		if (zspage_full(class, dst_zspage))
			break;

		/* Stop if there are no more objects to migrate */
		if (zspage_empty(src_zspage))
			break;
	}
}

static struct zspage *isolate_src_zspage(struct size_class *class)
{
	struct zspage *zspage;
	int fg;

	for (fg = ZS_INUSE_RATIO_10; fg <= ZS_INUSE_RATIO_99; fg++) {
		zspage = list_first_entry_or_null(&class->fullness_list[fg],
						  struct zspage, list);
		if (zspage) {
			remove_zspage(class, zspage);
			return zspage;
		}
	}

	return zspage;
}

static struct zspage *isolate_dst_zspage(struct size_class *class)
{
	struct zspage *zspage;
	int fg;

	for (fg = ZS_INUSE_RATIO_99; fg >= ZS_INUSE_RATIO_10; fg--) {
		zspage = list_first_entry_or_null(&class->fullness_list[fg],
						  struct zspage, list);
		if (zspage) {
			remove_zspage(class, zspage);
			return zspage;
		}
	}

	return zspage;
}

/*
 * putback_zspage - add @zspage into right class's fullness list
 * @class: destination class
 * @zspage: target page
 *
 * Return @zspage's fullness status
 */
static int putback_zspage(struct size_class *class, struct zspage *zspage)
{
	int fullness;

	fullness = get_fullness_group(class, zspage);
	insert_zspage(class, zspage, fullness);

	return fullness;
}

#ifdef CONFIG_COMPACTION
/*
 * To prevent zspage destroy during migration, zspage freeing should
 * hold locks of all pages in the zspage.
 */
static void lock_zspage(struct zspage *zspage)
{
	struct page *curr_page, *page;

	/*
	 * Pages we haven't locked yet can be migrated off the list while we're
	 * trying to lock them, so we need to be careful and only attempt to
	 * lock each page under migrate_read_lock(). Otherwise, the page we lock
	 * may no longer belong to the zspage. This means that we may wait for
	 * the wrong page to unlock, so we must take a reference to the page
	 * prior to waiting for it to unlock outside migrate_read_lock().
	 */
	while (1) {
		migrate_read_lock(zspage);
		page = get_first_page(zspage);
		if (trylock_page(page))
			break;
		get_page(page);
		migrate_read_unlock(zspage);
		wait_on_page_locked(page);
		put_page(page);
	}

	curr_page = page;
	while ((page = get_next_page(curr_page))) {
		if (trylock_page(page)) {
			curr_page = page;
		} else {
			get_page(page);
			migrate_read_unlock(zspage);
			wait_on_page_locked(page);
			put_page(page);
			migrate_read_lock(zspage);
		}
	}
	migrate_read_unlock(zspage);
}
#endif /* CONFIG_COMPACTION */

static void migrate_lock_init(struct zspage *zspage)
{
	rwlock_init(&zspage->lock);
}

static void migrate_read_lock(struct zspage *zspage) __acquires(&zspage->lock)
{
	read_lock(&zspage->lock);
}

static void migrate_read_unlock(struct zspage *zspage) __releases(&zspage->lock)
{
	read_unlock(&zspage->lock);
}

static void migrate_write_lock(struct zspage *zspage)
{
	write_lock(&zspage->lock);
}

static void migrate_write_unlock(struct zspage *zspage)
{
	write_unlock(&zspage->lock);
}

#ifdef CONFIG_COMPACTION

static const struct movable_operations zsmalloc_mops;

static void replace_sub_page(struct size_class *class, struct zspage *zspage,
				struct page *newpage, struct page *oldpage)
{
	struct page *page;
	struct page *pages[ZS_MAX_PAGES_PER_ZSPAGE] = {NULL, };
	int idx = 0;

	page = get_first_page(zspage);
	do {
		if (page == oldpage)
			pages[idx] = newpage;
		else
			pages[idx] = page;
		idx++;
	} while ((page = get_next_page(page)) != NULL);

	create_page_chain(class, zspage, pages);
	set_first_obj_offset(newpage, get_first_obj_offset(oldpage));
	if (unlikely(ZsHugePage(zspage)))
		newpage->index = oldpage->index;
	__SetPageMovable(newpage, &zsmalloc_mops);
}

static bool zs_page_isolate(struct page *page, isolate_mode_t mode)
{
	/*
	 * Page is locked so zspage couldn't be destroyed. For detail, look at
	 * lock_zspage in free_zspage.
	 */
	VM_BUG_ON_PAGE(PageIsolated(page), page);

	return true;
}

static int zs_page_migrate(struct page *newpage, struct page *page,
		enum migrate_mode mode)
{
	struct zs_pool *pool;
	struct size_class *class;
	struct zspage *zspage;
	struct page *dummy;
	void *s_addr, *d_addr, *addr;
	unsigned int offset;
	unsigned long handle;
	unsigned long old_obj, new_obj;
	unsigned int obj_idx;

	/*
	 * We cannot support the _NO_COPY case here, because copy needs to
	 * happen under the zs lock, which does not work with
	 * MIGRATE_SYNC_NO_COPY workflow.
	 */
	if (mode == MIGRATE_SYNC_NO_COPY)
		return -EINVAL;

	VM_BUG_ON_PAGE(!PageIsolated(page), page);

	/* The page is locked, so this pointer must remain valid */
	zspage = get_zspage(page);
	pool = zspage->pool;

	/*
	 * The pool's lock protects the race between zpage migration
	 * and zs_free.
	 */
	spin_lock(&pool->lock);
	class = zspage_class(pool, zspage);

	/* the migrate_write_lock protects zpage access via zs_map_object */
	migrate_write_lock(zspage);

	offset = get_first_obj_offset(page);
	s_addr = kmap_atomic(page);

	/*
	 * Here, any user cannot access all objects in the zspage so let's move.
	 */
	d_addr = kmap_atomic(newpage);
	copy_page(d_addr, s_addr);
	kunmap_atomic(d_addr);

	for (addr = s_addr + offset; addr < s_addr + PAGE_SIZE;
					addr += class->size) {
		if (obj_allocated(page, addr, &handle)) {

			old_obj = handle_to_obj(handle);
			obj_to_location(old_obj, &dummy, &obj_idx);
			new_obj = (unsigned long)location_to_obj(newpage,
								obj_idx);
			record_obj(handle, new_obj);
		}
	}
	kunmap_atomic(s_addr);

	replace_sub_page(class, zspage, newpage, page);
	/*
	 * Since we complete the data copy and set up new zspage structure,
	 * it's okay to release the pool's lock.
	 */
	spin_unlock(&pool->lock);
	migrate_write_unlock(zspage);

	get_page(newpage);
	if (page_zone(newpage) != page_zone(page)) {
		dec_zone_page_state(page, NR_ZSPAGES);
		inc_zone_page_state(newpage, NR_ZSPAGES);
	}

	reset_page(page);
	put_page(page);

	return MIGRATEPAGE_SUCCESS;
}

static void zs_page_putback(struct page *page)
{
	VM_BUG_ON_PAGE(!PageIsolated(page), page);
}

static const struct movable_operations zsmalloc_mops = {
	.isolate_page = zs_page_isolate,
	.migrate_page = zs_page_migrate,
	.putback_page = zs_page_putback,
};

/*
 * Caller should hold page_lock of all pages in the zspage
 * In here, we cannot use zspage meta data.
 */
static void async_free_zspage(struct work_struct *work)
{
	int i;
	struct size_class *class;
	struct zspage *zspage, *tmp;
	LIST_HEAD(free_pages);
	struct zs_pool *pool = container_of(work, struct zs_pool,
					free_work);

	for (i = 0; i < ZS_SIZE_CLASSES; i++) {
		class = pool->size_class[i];
		if (class->index != i)
			continue;

		spin_lock(&pool->lock);
		list_splice_init(&class->fullness_list[ZS_INUSE_RATIO_0],
				 &free_pages);
		spin_unlock(&pool->lock);
	}

	list_for_each_entry_safe(zspage, tmp, &free_pages, list) {
		list_del(&zspage->list);
		lock_zspage(zspage);

		spin_lock(&pool->lock);
		class = zspage_class(pool, zspage);
		__free_zspage(pool, class, zspage);
		spin_unlock(&pool->lock);
	}
};

static void kick_deferred_free(struct zs_pool *pool)
{
	schedule_work(&pool->free_work);
}

static void zs_flush_migration(struct zs_pool *pool)
{
	flush_work(&pool->free_work);
}

static void init_deferred_free(struct zs_pool *pool)
{
	INIT_WORK(&pool->free_work, async_free_zspage);
}

static void SetZsPageMovable(struct zs_pool *pool, struct zspage *zspage)
{
	struct page *page = get_first_page(zspage);

	do {
		WARN_ON(!trylock_page(page));
		__SetPageMovable(page, &zsmalloc_mops);
		unlock_page(page);
	} while ((page = get_next_page(page)) != NULL);
}
#else
static inline void zs_flush_migration(struct zs_pool *pool) { }
#endif

/*
 *
 * Based on the number of unused allocated objects calculate
 * and return the number of pages that we can free.
 */
static unsigned long zs_can_compact(struct size_class *class)
{
	unsigned long obj_wasted;
	unsigned long obj_allocated = zs_stat_get(class, ZS_OBJS_ALLOCATED);
	unsigned long obj_used = zs_stat_get(class, ZS_OBJS_INUSE);

	if (obj_allocated <= obj_used)
		return 0;

	obj_wasted = obj_allocated - obj_used;
	obj_wasted /= class->objs_per_zspage;

	return obj_wasted * class->pages_per_zspage;
}

static unsigned long __zs_compact(struct zs_pool *pool,
				  struct size_class *class)
{
	struct zspage *src_zspage = NULL;
	struct zspage *dst_zspage = NULL;
	unsigned long pages_freed = 0;

	/*
	 * protect the race between zpage migration and zs_free
	 * as well as zpage allocation/free
	 */
	spin_lock(&pool->lock);
	while (zs_can_compact(class)) {
		int fg;

		if (!dst_zspage) {
			dst_zspage = isolate_dst_zspage(class);
			if (!dst_zspage)
				break;
		}

		src_zspage = isolate_src_zspage(class);
		if (!src_zspage)
			break;

		migrate_write_lock(src_zspage);
		migrate_zspage(pool, src_zspage, dst_zspage);
		migrate_write_unlock(src_zspage);

		fg = putback_zspage(class, src_zspage);
		if (fg == ZS_INUSE_RATIO_0) {
			free_zspage(pool, class, src_zspage);
			pages_freed += class->pages_per_zspage;
		}
		src_zspage = NULL;

		if (get_fullness_group(class, dst_zspage) == ZS_INUSE_RATIO_100
		    || spin_is_contended(&pool->lock)) {
			putback_zspage(class, dst_zspage);
			dst_zspage = NULL;

			spin_unlock(&pool->lock);
			cond_resched();
			spin_lock(&pool->lock);
		}
	}

	if (src_zspage)
		putback_zspage(class, src_zspage);

	if (dst_zspage)
		putback_zspage(class, dst_zspage);

	spin_unlock(&pool->lock);

	return pages_freed;
}

unsigned long zs_compact(struct zs_pool *pool)
{
	int i;
	struct size_class *class;
	unsigned long pages_freed = 0;

	/*
	 * Pool compaction is performed under pool->lock so it is basically
	 * single-threaded. Having more than one thread in __zs_compact()
	 * will increase pool->lock contention, which will impact other
	 * zsmalloc operations that need pool->lock.
	 */
	if (atomic_xchg(&pool->compaction_in_progress, 1))
		return 0;

	for (i = ZS_SIZE_CLASSES - 1; i >= 0; i--) {
		class = pool->size_class[i];
		if (class->index != i)
			continue;
		pages_freed += __zs_compact(pool, class);
	}
	atomic_long_add(pages_freed, &pool->stats.pages_compacted);
	atomic_set(&pool->compaction_in_progress, 0);

	return pages_freed;
}
EXPORT_SYMBOL_GPL(zs_compact);

void zs_pool_stats(struct zs_pool *pool, struct zs_pool_stats *stats)
{
	memcpy(stats, &pool->stats, sizeof(struct zs_pool_stats));
}
EXPORT_SYMBOL_GPL(zs_pool_stats);

static unsigned long zs_shrinker_scan(struct shrinker *shrinker,
		struct shrink_control *sc)
{
	unsigned long pages_freed;
	struct zs_pool *pool = shrinker->private_data;

	/*
	 * Compact classes and calculate compaction delta.
	 * Can run concurrently with a manually triggered
	 * (by user) compaction.
	 */
	pages_freed = zs_compact(pool);

	return pages_freed ? pages_freed : SHRINK_STOP;
}

static unsigned long zs_shrinker_count(struct shrinker *shrinker,
		struct shrink_control *sc)
{
	int i;
	struct size_class *class;
	unsigned long pages_to_free = 0;
	struct zs_pool *pool = shrinker->private_data;

	for (i = ZS_SIZE_CLASSES - 1; i >= 0; i--) {
		class = pool->size_class[i];
		if (class->index != i)
			continue;

		pages_to_free += zs_can_compact(class);
	}

	return pages_to_free;
}

static void zs_unregister_shrinker(struct zs_pool *pool)
{
	shrinker_free(pool->shrinker);
}

static int zs_register_shrinker(struct zs_pool *pool)
{
	pool->shrinker = shrinker_alloc(0, "mm-zspool:%s", pool->name);
	if (!pool->shrinker)
		return -ENOMEM;

	pool->shrinker->scan_objects = zs_shrinker_scan;
	pool->shrinker->count_objects = zs_shrinker_count;
	pool->shrinker->batch = 0;
	pool->shrinker->private_data = pool;

	shrinker_register(pool->shrinker);

	return 0;
}

static int calculate_zspage_chain_size(int class_size)
{
	int i, min_waste = INT_MAX;
	int chain_size = 1;

	if (is_power_of_2(class_size))
		return chain_size;

	for (i = 1; i <= ZS_MAX_PAGES_PER_ZSPAGE; i++) {
		int waste;

		waste = (i * PAGE_SIZE) % class_size;
		if (waste < min_waste) {
			min_waste = waste;
			chain_size = i;
		}
	}

	return chain_size;
}

/**
 * zs_create_pool - Creates an allocation pool to work from.
 * @name: pool name to be created
 *
 * This function must be called before anything when using
 * the zsmalloc allocator.
 *
 * On success, a pointer to the newly created pool is returned,
 * otherwise NULL.
 */
struct zs_pool *zs_create_pool(const char *name)
{
	int i;
	struct zs_pool *pool;
	struct size_class *prev_class = NULL;

	pool = kzalloc(sizeof(*pool), GFP_KERNEL);
	if (!pool)
		return NULL;

	init_deferred_free(pool);
	spin_lock_init(&pool->lock);
	atomic_set(&pool->compaction_in_progress, 0);

	pool->name = kstrdup(name, GFP_KERNEL);
	if (!pool->name)
		goto err;

	if (create_cache(pool))
		goto err;

	/*
	 * Iterate reversely, because, size of size_class that we want to use
	 * for merging should be larger or equal to current size.
	 */
	for (i = ZS_SIZE_CLASSES - 1; i >= 0; i--) {
		int size;
		int pages_per_zspage;
		int objs_per_zspage;
		struct size_class *class;
		int fullness;

		size = ZS_MIN_ALLOC_SIZE + i * ZS_SIZE_CLASS_DELTA;
		if (size > ZS_MAX_ALLOC_SIZE)
			size = ZS_MAX_ALLOC_SIZE;
		pages_per_zspage = calculate_zspage_chain_size(size);
		objs_per_zspage = pages_per_zspage * PAGE_SIZE / size;

		/*
		 * We iterate from biggest down to smallest classes,
		 * so huge_class_size holds the size of the first huge
		 * class. Any object bigger than or equal to that will
		 * endup in the huge class.
		 */
		if (pages_per_zspage != 1 && objs_per_zspage != 1 &&
				!huge_class_size) {
			huge_class_size = size;
			/*
			 * The object uses ZS_HANDLE_SIZE bytes to store the
			 * handle. We need to subtract it, because zs_malloc()
			 * unconditionally adds handle size before it performs
			 * size class search - so object may be smaller than
			 * huge class size, yet it still can end up in the huge
			 * class because it grows by ZS_HANDLE_SIZE extra bytes
			 * right before class lookup.
			 */
			huge_class_size -= (ZS_HANDLE_SIZE - 1);
		}

		/*
		 * size_class is used for normal zsmalloc operation such
		 * as alloc/free for that size. Although it is natural that we
		 * have one size_class for each size, there is a chance that we
		 * can get more memory utilization if we use one size_class for
		 * many different sizes whose size_class have same
		 * characteristics. So, we makes size_class point to
		 * previous size_class if possible.
		 */
		if (prev_class) {
			if (can_merge(prev_class, pages_per_zspage, objs_per_zspage)) {
				pool->size_class[i] = prev_class;
				continue;
			}
		}

		class = kzalloc(sizeof(struct size_class), GFP_KERNEL);
		if (!class)
			goto err;

		class->size = size;
		class->index = i;
		class->pages_per_zspage = pages_per_zspage;
		class->objs_per_zspage = objs_per_zspage;
		pool->size_class[i] = class;

		fullness = ZS_INUSE_RATIO_0;
		while (fullness < NR_FULLNESS_GROUPS) {
			INIT_LIST_HEAD(&class->fullness_list[fullness]);
			fullness++;
		}

		prev_class = class;
	}

	/* debug only, don't abort if it fails */
	zs_pool_stat_create(pool, name);

	/*
	 * Not critical since shrinker is only used to trigger internal
	 * defragmentation of the pool which is pretty optional thing.  If
	 * registration fails we still can use the pool normally and user can
	 * trigger compaction manually. Thus, ignore return code.
	 */
	zs_register_shrinker(pool);

	return pool;

err:
	zs_destroy_pool(pool);
	return NULL;
}
EXPORT_SYMBOL_GPL(zs_create_pool);

void zs_destroy_pool(struct zs_pool *pool)
{
	int i;

	zs_unregister_shrinker(pool);
	zs_flush_migration(pool);
	zs_pool_stat_destroy(pool);

	for (i = 0; i < ZS_SIZE_CLASSES; i++) {
		int fg;
		struct size_class *class = pool->size_class[i];

		if (!class)
			continue;

		if (class->index != i)
			continue;

		for (fg = ZS_INUSE_RATIO_0; fg < NR_FULLNESS_GROUPS; fg++) {
			if (list_empty(&class->fullness_list[fg]))
				continue;

			pr_err("Class-%d fullness group %d is not empty\n",
			       class->size, fg);
		}
		kfree(class);
	}

	destroy_cache(pool);
	kfree(pool->name);
	kfree(pool);
}
EXPORT_SYMBOL_GPL(zs_destroy_pool);

static int __init zs_init(void)
{
	int ret;

	ret = cpuhp_setup_state(CPUHP_MM_ZS_PREPARE, "mm/zsmalloc:prepare",
				zs_cpu_prepare, zs_cpu_dead);
	if (ret)
		goto out;

#ifdef CONFIG_ZPOOL
	zpool_register_driver(&zs_zpool_driver);
#endif

	zs_stat_init();

	return 0;

out:
	return ret;
}

static void __exit zs_exit(void)
{
#ifdef CONFIG_ZPOOL
	zpool_unregister_driver(&zs_zpool_driver);
#endif
	cpuhp_remove_state(CPUHP_MM_ZS_PREPARE);

	zs_stat_exit();
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

module_init(zs_init);
module_exit(zs_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Nitin Gupta <ngupta@vflare.org>");
