<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Memory merging support.
 *
 * This code enables dynamic sharing of identical pages found in different
 * memory areas, even if they are not shared by fork()
 *
 * Copyright (C) 2008-2009 Red Hat, Inc.
 * Authors:
 *	Izik Eidus
 *	Andrea Arcangeli
 *	Chris Wright
 *	Hugh Dickins
<<<<<<< HEAD
 *
 * This work is licensed under the terms of the GNU GPL, version 2.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/errno.h>
#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/fs.h>
#include <linux/mman.h>
#include <linux/sched.h>
=======
#include <linux/mm_inline.h>
#include <linux/fs.h>
#include <linux/mman.h>
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/sched/coredump.h>
#include <linux/sched/cputime.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/rwsem.h>
#include <linux/pagemap.h>
#include <linux/rmap.h>
#include <linux/spinlock.h>
<<<<<<< HEAD
#include <linux/jhash.h>
=======
#include <linux/xxhash.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/rbtree.h>
#include <linux/memory.h>
#include <linux/mmu_notifier.h>
#include <linux/swap.h>
#include <linux/ksm.h>
<<<<<<< HEAD
#include <linux/hash.h>
#include <linux/freezer.h>
#include <linux/oom.h>

#include <asm/tlbflush.h>
#include "internal.h"

/*
=======
#include <linux/hashtable.h>
#include <linux/freezer.h>
#include <linux/oom.h>
#include <linux/numa.h>
#include <linux/pagewalk.h>

#include <asm/tlbflush.h>
#include "internal.h"
#include "mm_slot.h"

#define CREATE_TRACE_POINTS
#include <trace/events/ksm.h>

#ifdef CONFIG_NUMA
#define NUMA(x)		(x)
#define DO_NUMA(x)	do { (x); } while (0)
#else
#define NUMA(x)		(0)
#define DO_NUMA(x)	do { } while (0)
#endif

typedef u8 rmap_age_t;

/**
 * DOC: Overview
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * A few notes about the KSM scanning process,
 * to make it easier to understand the data structures below:
 *
 * In order to reduce excessive scanning, KSM sorts the memory pages by their
 * contents into a data structure that holds pointers to the pages' locations.
 *
 * Since the contents of the pages may change at any moment, KSM cannot just
 * insert the pages into a normal sorted tree and expect it to find anything.
 * Therefore KSM uses two data structures - the stable and the unstable tree.
 *
 * The stable tree holds pointers to all the merged pages (ksm pages), sorted
 * by their contents.  Because each such page is write-protected, searching on
 * this tree is fully assured to be working (except when pages are unmapped),
 * and therefore this tree is called the stable tree.
 *
<<<<<<< HEAD
=======
 * The stable tree node includes information required for reverse
 * mapping from a KSM page to virtual addresses that map this page.
 *
 * In order to avoid large latencies of the rmap walks on KSM pages,
 * KSM maintains two types of nodes in the stable tree:
 *
 * * the regular nodes that keep the reverse mapping structures in a
 *   linked list
 * * the "chains" that link nodes ("dups") that represent the same
 *   write protected memory content, but each "dup" corresponds to a
 *   different KSM page copy of that content
 *
 * Internally, the regular nodes, "dups" and "chains" are represented
 * using the same struct ksm_stable_node structure.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * In addition to the stable tree, KSM uses a second data structure called the
 * unstable tree: this tree holds pointers to pages which have been found to
 * be "unchanged for a period of time".  The unstable tree sorts these pages
 * by their contents, but since they are not write-protected, KSM cannot rely
 * upon the unstable tree to work correctly - the unstable tree is liable to
 * be corrupted as its contents are modified, and so it is called unstable.
 *
 * KSM solves this problem by several techniques:
 *
 * 1) The unstable tree is flushed every time KSM completes scanning all
 *    memory areas, and then the tree is rebuilt again from the beginning.
 * 2) KSM will only insert into the unstable tree, pages whose hash value
 *    has not changed since the previous scan of all memory areas.
 * 3) The unstable tree is a RedBlack Tree - so its balancing is based on the
 *    colors of the nodes and not on their contents, assuring that even when
 *    the tree gets "corrupted" it won't get out of balance, so scanning time
 *    remains the same (also, searching and inserting nodes in an rbtree uses
 *    the same algorithm, so we have no overhead when we flush and rebuild).
 * 4) KSM never flushes the stable tree, which means that even if it were to
 *    take 10 attempts to find a page in the unstable tree, once it is found,
 *    it is secured in the stable tree.  (When we scan a new page, we first
 *    compare it against the stable tree, and then against the unstable tree.)
<<<<<<< HEAD
 */

/**
 * struct mm_slot - ksm information per mm that is being scanned
 * @link: link to the mm_slots hash list
 * @mm_list: link into the mm_slots list, rooted in ksm_mm_head
 * @rmap_list: head for this mm_slot's singly-linked list of rmap_items
 * @mm: the mm that this information is valid for
 */
struct mm_slot {
	struct hlist_node link;
	struct list_head mm_list;
	struct rmap_item *rmap_list;
	struct mm_struct *mm;
=======
 *
 * If the merge_across_nodes tunable is unset, then KSM maintains multiple
 * stable trees and multiple unstable trees: one of each for each NUMA node.
 */

/**
 * struct ksm_mm_slot - ksm information per mm that is being scanned
 * @slot: hash lookup from mm to mm_slot
 * @rmap_list: head for this mm_slot's singly-linked list of rmap_items
 */
struct ksm_mm_slot {
	struct mm_slot slot;
	struct ksm_rmap_item *rmap_list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * struct ksm_scan - cursor for scanning
 * @mm_slot: the current mm_slot we are scanning
 * @address: the next address inside that to be scanned
 * @rmap_list: link to the next rmap to be scanned in the rmap_list
 * @seqnr: count of completed full scans (needed when removing unstable node)
 *
 * There is only the one ksm_scan instance of this cursor structure.
 */
struct ksm_scan {
<<<<<<< HEAD
	struct mm_slot *mm_slot;
	unsigned long address;
	struct rmap_item **rmap_list;
=======
	struct ksm_mm_slot *mm_slot;
	unsigned long address;
	struct ksm_rmap_item **rmap_list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long seqnr;
};

/**
<<<<<<< HEAD
 * struct stable_node - node of the stable rbtree
 * @node: rb node of this ksm page in the stable tree
 * @hlist: hlist head of rmap_items using this ksm page
 * @kpfn: page frame number of this ksm page
 */
struct stable_node {
	struct rb_node node;
	struct hlist_head hlist;
	unsigned long kpfn;
};

/**
 * struct rmap_item - reverse mapping item for virtual addresses
 * @rmap_list: next rmap_item in mm_slot's singly-linked rmap_list
 * @anon_vma: pointer to anon_vma for this mm,address, when in stable tree
=======
 * struct ksm_stable_node - node of the stable rbtree
 * @node: rb node of this ksm page in the stable tree
 * @head: (overlaying parent) &migrate_nodes indicates temporarily on that list
 * @hlist_dup: linked into the stable_node->hlist with a stable_node chain
 * @list: linked into migrate_nodes, pending placement in the proper node tree
 * @hlist: hlist head of rmap_items using this ksm page
 * @kpfn: page frame number of this ksm page (perhaps temporarily on wrong nid)
 * @chain_prune_time: time of the last full garbage collection
 * @rmap_hlist_len: number of rmap_item entries in hlist or STABLE_NODE_CHAIN
 * @nid: NUMA node id of stable tree in which linked (may not match kpfn)
 */
struct ksm_stable_node {
	union {
		struct rb_node node;	/* when node of stable tree */
		struct {		/* when listed for migration */
			struct list_head *head;
			struct {
				struct hlist_node hlist_dup;
				struct list_head list;
			};
		};
	};
	struct hlist_head hlist;
	union {
		unsigned long kpfn;
		unsigned long chain_prune_time;
	};
	/*
	 * STABLE_NODE_CHAIN can be any negative number in
	 * rmap_hlist_len negative range, but better not -1 to be able
	 * to reliably detect underflows.
	 */
#define STABLE_NODE_CHAIN -1024
	int rmap_hlist_len;
#ifdef CONFIG_NUMA
	int nid;
#endif
};

/**
 * struct ksm_rmap_item - reverse mapping item for virtual addresses
 * @rmap_list: next rmap_item in mm_slot's singly-linked rmap_list
 * @anon_vma: pointer to anon_vma for this mm,address, when in stable tree
 * @nid: NUMA node id of unstable tree in which linked (may not match page)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @mm: the memory structure this rmap_item is pointing into
 * @address: the virtual address this rmap_item tracks (+ flags in low bits)
 * @oldchecksum: previous checksum of the page at that virtual address
 * @node: rb node of this rmap_item in the unstable tree
 * @head: pointer to stable_node heading this list in the stable tree
 * @hlist: link into hlist of rmap_items hanging off that stable_node
<<<<<<< HEAD
 */
struct rmap_item {
	struct rmap_item *rmap_list;
	struct anon_vma *anon_vma;	/* when stable */
	struct mm_struct *mm;
	unsigned long address;		/* + low bits used for flags below */
	unsigned int oldchecksum;	/* when unstable */
	union {
		struct rb_node node;	/* when node of unstable tree */
		struct {		/* when listed from stable tree */
			struct stable_node *head;
=======
 * @age: number of scan iterations since creation
 * @remaining_skips: how many scans to skip
 */
struct ksm_rmap_item {
	struct ksm_rmap_item *rmap_list;
	union {
		struct anon_vma *anon_vma;	/* when stable */
#ifdef CONFIG_NUMA
		int nid;		/* when node of unstable tree */
#endif
	};
	struct mm_struct *mm;
	unsigned long address;		/* + low bits used for flags below */
	unsigned int oldchecksum;	/* when unstable */
	rmap_age_t age;
	rmap_age_t remaining_skips;
	union {
		struct rb_node node;	/* when node of unstable tree */
		struct {		/* when listed from stable tree */
			struct ksm_stable_node *head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			struct hlist_node hlist;
		};
	};
};

#define SEQNR_MASK	0x0ff	/* low bits of unstable tree seqnr */
#define UNSTABLE_FLAG	0x100	/* is a node of the unstable tree */
#define STABLE_FLAG	0x200	/* is listed from the stable tree */

/* The stable and unstable tree heads */
<<<<<<< HEAD
static struct rb_root root_stable_tree = RB_ROOT;
static struct rb_root root_unstable_tree = RB_ROOT;

#define MM_SLOTS_HASH_SHIFT 10
#define MM_SLOTS_HASH_HEADS (1 << MM_SLOTS_HASH_SHIFT)
static struct hlist_head mm_slots_hash[MM_SLOTS_HASH_HEADS];

static struct mm_slot ksm_mm_head = {
	.mm_list = LIST_HEAD_INIT(ksm_mm_head.mm_list),
=======
static struct rb_root one_stable_tree[1] = { RB_ROOT };
static struct rb_root one_unstable_tree[1] = { RB_ROOT };
static struct rb_root *root_stable_tree = one_stable_tree;
static struct rb_root *root_unstable_tree = one_unstable_tree;

/* Recently migrated nodes of stable tree, pending proper placement */
static LIST_HEAD(migrate_nodes);
#define STABLE_NODE_DUP_HEAD ((struct list_head *)&migrate_nodes.prev)

#define MM_SLOTS_HASH_BITS 10
static DEFINE_HASHTABLE(mm_slots_hash, MM_SLOTS_HASH_BITS);

static struct ksm_mm_slot ksm_mm_head = {
	.slot.mm_node = LIST_HEAD_INIT(ksm_mm_head.slot.mm_node),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
static struct ksm_scan ksm_scan = {
	.mm_slot = &ksm_mm_head,
};

static struct kmem_cache *rmap_item_cache;
static struct kmem_cache *stable_node_cache;
static struct kmem_cache *mm_slot_cache;

<<<<<<< HEAD
=======
/* Default number of pages to scan per batch */
#define DEFAULT_PAGES_TO_SCAN 100

/* The number of pages scanned */
static unsigned long ksm_pages_scanned;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* The number of nodes in the stable tree */
static unsigned long ksm_pages_shared;

/* The number of page slots additionally sharing those nodes */
static unsigned long ksm_pages_sharing;

/* The number of nodes in the unstable tree */
static unsigned long ksm_pages_unshared;

/* The number of rmap_items in use: to calculate pages_volatile */
static unsigned long ksm_rmap_items;

<<<<<<< HEAD
/* Number of pages ksmd should scan in one batch */
static unsigned int ksm_thread_pages_to_scan = 100;
=======
/* The number of stable_node chains */
static unsigned long ksm_stable_node_chains;

/* The number of stable_node dups linked to the stable_node chains */
static unsigned long ksm_stable_node_dups;

/* Delay in pruning stale stable_node_dups in the stable_node_chains */
static unsigned int ksm_stable_node_chains_prune_millisecs = 2000;

/* Maximum number of page slots sharing a stable node */
static int ksm_max_page_sharing = 256;

/* Number of pages ksmd should scan in one batch */
static unsigned int ksm_thread_pages_to_scan = DEFAULT_PAGES_TO_SCAN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Milliseconds ksmd should sleep between batches */
static unsigned int ksm_thread_sleep_millisecs = 20;

<<<<<<< HEAD
/* Boolean to indicate whether to use deferred timer or not */
static bool use_deferred_timer;
=======
/* Checksum of an empty (zeroed) page */
static unsigned int zero_checksum __read_mostly;

/* Whether to merge empty (zeroed) pages with actual zero pages */
static bool ksm_use_zero_pages __read_mostly;

/* Skip pages that couldn't be de-duplicated previously */
/* Default to true at least temporarily, for testing */
static bool ksm_smart_scan = true;

/* The number of zero pages which is placed by KSM */
unsigned long ksm_zero_pages;

/* The number of pages that have been skipped due to "smart scanning" */
static unsigned long ksm_pages_skipped;

/* Don't scan more than max pages per batch. */
static unsigned long ksm_advisor_max_pages_to_scan = 30000;

/* Min CPU for scanning pages per scan */
#define KSM_ADVISOR_MIN_CPU 10

/* Max CPU for scanning pages per scan */
static unsigned int ksm_advisor_max_cpu =  70;

/* Target scan time in seconds to analyze all KSM candidate pages. */
static unsigned long ksm_advisor_target_scan_time = 200;

/* Exponentially weighted moving average. */
#define EWMA_WEIGHT 30

/**
 * struct advisor_ctx - metadata for KSM advisor
 * @start_scan: start time of the current scan
 * @scan_time: scan time of previous scan
 * @change: change in percent to pages_to_scan parameter
 * @cpu_time: cpu time consumed by the ksmd thread in the previous scan
 */
struct advisor_ctx {
	ktime_t start_scan;
	unsigned long scan_time;
	unsigned long change;
	unsigned long long cpu_time;
};
static struct advisor_ctx advisor_ctx;

/* Define different advisor's */
enum ksm_advisor_type {
	KSM_ADVISOR_NONE,
	KSM_ADVISOR_SCAN_TIME,
};
static enum ksm_advisor_type ksm_advisor;

#ifdef CONFIG_SYSFS
/*
 * Only called through the sysfs control interface:
 */

/* At least scan this many pages per batch. */
static unsigned long ksm_advisor_min_pages_to_scan = 500;

static void set_advisor_defaults(void)
{
	if (ksm_advisor == KSM_ADVISOR_NONE) {
		ksm_thread_pages_to_scan = DEFAULT_PAGES_TO_SCAN;
	} else if (ksm_advisor == KSM_ADVISOR_SCAN_TIME) {
		advisor_ctx = (const struct advisor_ctx){ 0 };
		ksm_thread_pages_to_scan = ksm_advisor_min_pages_to_scan;
	}
}
#endif /* CONFIG_SYSFS */

static inline void advisor_start_scan(void)
{
	if (ksm_advisor == KSM_ADVISOR_SCAN_TIME)
		advisor_ctx.start_scan = ktime_get();
}

/*
 * Use previous scan time if available, otherwise use current scan time as an
 * approximation for the previous scan time.
 */
static inline unsigned long prev_scan_time(struct advisor_ctx *ctx,
					   unsigned long scan_time)
{
	return ctx->scan_time ? ctx->scan_time : scan_time;
}

/* Calculate exponential weighted moving average */
static unsigned long ewma(unsigned long prev, unsigned long curr)
{
	return ((100 - EWMA_WEIGHT) * prev + EWMA_WEIGHT * curr) / 100;
}

/*
 * The scan time advisor is based on the current scan rate and the target
 * scan rate.
 *
 *      new_pages_to_scan = pages_to_scan * (scan_time / target_scan_time)
 *
 * To avoid perturbations it calculates a change factor of previous changes.
 * A new change factor is calculated for each iteration and it uses an
 * exponentially weighted moving average. The new pages_to_scan value is
 * multiplied with that change factor:
 *
 *      new_pages_to_scan *= change facor
 *
 * The new_pages_to_scan value is limited by the cpu min and max values. It
 * calculates the cpu percent for the last scan and calculates the new
 * estimated cpu percent cost for the next scan. That value is capped by the
 * cpu min and max setting.
 *
 * In addition the new pages_to_scan value is capped by the max and min
 * limits.
 */
static void scan_time_advisor(void)
{
	unsigned int cpu_percent;
	unsigned long cpu_time;
	unsigned long cpu_time_diff;
	unsigned long cpu_time_diff_ms;
	unsigned long pages;
	unsigned long per_page_cost;
	unsigned long factor;
	unsigned long change;
	unsigned long last_scan_time;
	unsigned long scan_time;

	/* Convert scan time to seconds */
	scan_time = div_s64(ktime_ms_delta(ktime_get(), advisor_ctx.start_scan),
			    MSEC_PER_SEC);
	scan_time = scan_time ? scan_time : 1;

	/* Calculate CPU consumption of ksmd background thread */
	cpu_time = task_sched_runtime(current);
	cpu_time_diff = cpu_time - advisor_ctx.cpu_time;
	cpu_time_diff_ms = cpu_time_diff / 1000 / 1000;

	cpu_percent = (cpu_time_diff_ms * 100) / (scan_time * 1000);
	cpu_percent = cpu_percent ? cpu_percent : 1;
	last_scan_time = prev_scan_time(&advisor_ctx, scan_time);

	/* Calculate scan time as percentage of target scan time */
	factor = ksm_advisor_target_scan_time * 100 / scan_time;
	factor = factor ? factor : 1;

	/*
	 * Calculate scan time as percentage of last scan time and use
	 * exponentially weighted average to smooth it
	 */
	change = scan_time * 100 / last_scan_time;
	change = change ? change : 1;
	change = ewma(advisor_ctx.change, change);

	/* Calculate new scan rate based on target scan rate. */
	pages = ksm_thread_pages_to_scan * 100 / factor;
	/* Update pages_to_scan by weighted change percentage. */
	pages = pages * change / 100;

	/* Cap new pages_to_scan value */
	per_page_cost = ksm_thread_pages_to_scan / cpu_percent;
	per_page_cost = per_page_cost ? per_page_cost : 1;

	pages = min(pages, per_page_cost * ksm_advisor_max_cpu);
	pages = max(pages, per_page_cost * KSM_ADVISOR_MIN_CPU);
	pages = min(pages, ksm_advisor_max_pages_to_scan);

	/* Update advisor context */
	advisor_ctx.change = change;
	advisor_ctx.scan_time = scan_time;
	advisor_ctx.cpu_time = cpu_time;

	ksm_thread_pages_to_scan = pages;
	trace_ksm_advisor(scan_time, pages, cpu_percent);
}

static void advisor_stop_scan(void)
{
	if (ksm_advisor == KSM_ADVISOR_SCAN_TIME)
		scan_time_advisor();
}

#ifdef CONFIG_NUMA
/* Zeroed when merging across nodes is not allowed */
static unsigned int ksm_merge_across_nodes = 1;
static int ksm_nr_node_ids = 1;
#else
#define ksm_merge_across_nodes	1U
#define ksm_nr_node_ids		1
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define KSM_RUN_STOP	0
#define KSM_RUN_MERGE	1
#define KSM_RUN_UNMERGE	2
<<<<<<< HEAD
static unsigned int ksm_run = KSM_RUN_STOP;

static DECLARE_WAIT_QUEUE_HEAD(ksm_thread_wait);
static DEFINE_MUTEX(ksm_thread_mutex);
static DEFINE_SPINLOCK(ksm_mmlist_lock);

#define KSM_KMEM_CACHE(__struct, __flags) kmem_cache_create("ksm_"#__struct,\
=======
#define KSM_RUN_OFFLINE	4
static unsigned long ksm_run = KSM_RUN_STOP;
static void wait_while_offlining(void);

static DECLARE_WAIT_QUEUE_HEAD(ksm_thread_wait);
static DECLARE_WAIT_QUEUE_HEAD(ksm_iter_wait);
static DEFINE_MUTEX(ksm_thread_mutex);
static DEFINE_SPINLOCK(ksm_mmlist_lock);

#define KSM_KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sizeof(struct __struct), __alignof__(struct __struct),\
		(__flags), NULL)

static int __init ksm_slab_init(void)
{
<<<<<<< HEAD
	rmap_item_cache = KSM_KMEM_CACHE(rmap_item, 0);
	if (!rmap_item_cache)
		goto out;

	stable_node_cache = KSM_KMEM_CACHE(stable_node, 0);
	if (!stable_node_cache)
		goto out_free1;

	mm_slot_cache = KSM_KMEM_CACHE(mm_slot, 0);
=======
	rmap_item_cache = KSM_KMEM_CACHE(ksm_rmap_item, 0);
	if (!rmap_item_cache)
		goto out;

	stable_node_cache = KSM_KMEM_CACHE(ksm_stable_node, 0);
	if (!stable_node_cache)
		goto out_free1;

	mm_slot_cache = KSM_KMEM_CACHE(ksm_mm_slot, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!mm_slot_cache)
		goto out_free2;

	return 0;

out_free2:
	kmem_cache_destroy(stable_node_cache);
out_free1:
	kmem_cache_destroy(rmap_item_cache);
out:
	return -ENOMEM;
}

static void __init ksm_slab_free(void)
{
	kmem_cache_destroy(mm_slot_cache);
	kmem_cache_destroy(stable_node_cache);
	kmem_cache_destroy(rmap_item_cache);
	mm_slot_cache = NULL;
}

<<<<<<< HEAD
static inline struct rmap_item *alloc_rmap_item(void)
{
	struct rmap_item *rmap_item;

	rmap_item = kmem_cache_zalloc(rmap_item_cache, GFP_KERNEL);
=======
static __always_inline bool is_stable_node_chain(struct ksm_stable_node *chain)
{
	return chain->rmap_hlist_len == STABLE_NODE_CHAIN;
}

static __always_inline bool is_stable_node_dup(struct ksm_stable_node *dup)
{
	return dup->head == STABLE_NODE_DUP_HEAD;
}

static inline void stable_node_chain_add_dup(struct ksm_stable_node *dup,
					     struct ksm_stable_node *chain)
{
	VM_BUG_ON(is_stable_node_dup(dup));
	dup->head = STABLE_NODE_DUP_HEAD;
	VM_BUG_ON(!is_stable_node_chain(chain));
	hlist_add_head(&dup->hlist_dup, &chain->hlist);
	ksm_stable_node_dups++;
}

static inline void __stable_node_dup_del(struct ksm_stable_node *dup)
{
	VM_BUG_ON(!is_stable_node_dup(dup));
	hlist_del(&dup->hlist_dup);
	ksm_stable_node_dups--;
}

static inline void stable_node_dup_del(struct ksm_stable_node *dup)
{
	VM_BUG_ON(is_stable_node_chain(dup));
	if (is_stable_node_dup(dup))
		__stable_node_dup_del(dup);
	else
		rb_erase(&dup->node, root_stable_tree + NUMA(dup->nid));
#ifdef CONFIG_DEBUG_VM
	dup->head = NULL;
#endif
}

static inline struct ksm_rmap_item *alloc_rmap_item(void)
{
	struct ksm_rmap_item *rmap_item;

	rmap_item = kmem_cache_zalloc(rmap_item_cache, GFP_KERNEL |
						__GFP_NORETRY | __GFP_NOWARN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rmap_item)
		ksm_rmap_items++;
	return rmap_item;
}

<<<<<<< HEAD
static inline void free_rmap_item(struct rmap_item *rmap_item)
{
	ksm_rmap_items--;
=======
static inline void free_rmap_item(struct ksm_rmap_item *rmap_item)
{
	ksm_rmap_items--;
	rmap_item->mm->ksm_rmap_items--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rmap_item->mm = NULL;	/* debug safety */
	kmem_cache_free(rmap_item_cache, rmap_item);
}

<<<<<<< HEAD
static inline struct stable_node *alloc_stable_node(void)
{
	return kmem_cache_alloc(stable_node_cache, GFP_KERNEL);
}

static inline void free_stable_node(struct stable_node *stable_node)
{
	kmem_cache_free(stable_node_cache, stable_node);
}

static inline struct mm_slot *alloc_mm_slot(void)
{
	if (!mm_slot_cache)	/* initialization failed */
		return NULL;
	return kmem_cache_zalloc(mm_slot_cache, GFP_KERNEL);
}

static inline void free_mm_slot(struct mm_slot *mm_slot)
{
	kmem_cache_free(mm_slot_cache, mm_slot);
}

static struct mm_slot *get_mm_slot(struct mm_struct *mm)
{
	struct mm_slot *mm_slot;
	struct hlist_head *bucket;
	struct hlist_node *node;

	bucket = &mm_slots_hash[hash_ptr(mm, MM_SLOTS_HASH_SHIFT)];
	hlist_for_each_entry(mm_slot, node, bucket, link) {
		if (mm == mm_slot->mm)
			return mm_slot;
	}
	return NULL;
}

static void insert_to_mm_slots_hash(struct mm_struct *mm,
				    struct mm_slot *mm_slot)
{
	struct hlist_head *bucket;

	bucket = &mm_slots_hash[hash_ptr(mm, MM_SLOTS_HASH_SHIFT)];
	mm_slot->mm = mm;
	hlist_add_head(&mm_slot->link, bucket);
}

static inline int in_stable_tree(struct rmap_item *rmap_item)
{
	return rmap_item->address & STABLE_FLAG;
}

/*
 * ksmd, and unmerge_and_remove_all_rmap_items(), must not touch an mm's
 * page tables after it has passed through ksm_exit() - which, if necessary,
 * takes mmap_sem briefly to serialize against them.  ksm_exit() does not set
=======
static inline struct ksm_stable_node *alloc_stable_node(void)
{
	/*
	 * The allocation can take too long with GFP_KERNEL when memory is under
	 * pressure, which may lead to hung task warnings.  Adding __GFP_HIGH
	 * grants access to memory reserves, helping to avoid this problem.
	 */
	return kmem_cache_alloc(stable_node_cache, GFP_KERNEL | __GFP_HIGH);
}

static inline void free_stable_node(struct ksm_stable_node *stable_node)
{
	VM_BUG_ON(stable_node->rmap_hlist_len &&
		  !is_stable_node_chain(stable_node));
	kmem_cache_free(stable_node_cache, stable_node);
}

/*
 * ksmd, and unmerge_and_remove_all_rmap_items(), must not touch an mm's
 * page tables after it has passed through ksm_exit() - which, if necessary,
 * takes mmap_lock briefly to serialize against them.  ksm_exit() does not set
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * a special flag: they can just back out as soon as mm_users goes to zero.
 * ksm_test_exit() is used throughout to make this test for exit: in some
 * places for correctness, in some places just to avoid unnecessary work.
 */
static inline bool ksm_test_exit(struct mm_struct *mm)
{
	return atomic_read(&mm->mm_users) == 0;
}

<<<<<<< HEAD
/*
 * We use break_ksm to break COW on a ksm page: it's a stripped down
 *
 *	if (get_user_pages(current, mm, addr, 1, 1, 1, &page, NULL) == 1)
 *		put_page(page);
 *
 * but taking great care only to touch a ksm page, in a VM_MERGEABLE vma,
 * in case the application has unmapped and remapped mm,addr meanwhile.
 * Could a ksm page appear anywhere else?  Actually yes, in a VM_PFNMAP
 * mmap of /dev/mem or /dev/kmem, where we would not want to touch it.
 */
static int break_ksm(struct vm_area_struct *vma, unsigned long addr)
{
	struct page *page;
	int ret = 0;

	do {
		cond_resched();
		page = follow_page(vma, addr, FOLL_GET);
		if (IS_ERR_OR_NULL(page))
			break;
		if (PageKsm(page))
			ret = handle_mm_fault(vma->vm_mm, vma, addr,
							FAULT_FLAG_WRITE);
		else
			ret = VM_FAULT_WRITE;
		put_page(page);
	} while (!(ret & (VM_FAULT_WRITE | VM_FAULT_SIGBUS | VM_FAULT_SIGSEGV | VM_FAULT_OOM)));
	/*
	 * We must loop because handle_mm_fault() may back out if there's
	 * any difficulty e.g. if pte accessed bit gets updated concurrently.
	 *
	 * VM_FAULT_WRITE is what we have been hoping for: it indicates that
	 * COW has been broken, even if the vma does not permit VM_WRITE;
	 * but note that a concurrent fault might break PageKsm for us.
=======
static int break_ksm_pmd_entry(pmd_t *pmd, unsigned long addr, unsigned long next,
			struct mm_walk *walk)
{
	struct page *page = NULL;
	spinlock_t *ptl;
	pte_t *pte;
	pte_t ptent;
	int ret;

	pte = pte_offset_map_lock(walk->mm, pmd, addr, &ptl);
	if (!pte)
		return 0;
	ptent = ptep_get(pte);
	if (pte_present(ptent)) {
		page = vm_normal_page(walk->vma, addr, ptent);
	} else if (!pte_none(ptent)) {
		swp_entry_t entry = pte_to_swp_entry(ptent);

		/*
		 * As KSM pages remain KSM pages until freed, no need to wait
		 * here for migration to end.
		 */
		if (is_migration_entry(entry))
			page = pfn_swap_entry_to_page(entry);
	}
	/* return 1 if the page is an normal ksm page or KSM-placed zero page */
	ret = (page && PageKsm(page)) || is_ksm_zero_pte(ptent);
	pte_unmap_unlock(pte, ptl);
	return ret;
}

static const struct mm_walk_ops break_ksm_ops = {
	.pmd_entry = break_ksm_pmd_entry,
	.walk_lock = PGWALK_RDLOCK,
};

static const struct mm_walk_ops break_ksm_lock_vma_ops = {
	.pmd_entry = break_ksm_pmd_entry,
	.walk_lock = PGWALK_WRLOCK,
};

/*
 * We use break_ksm to break COW on a ksm page by triggering unsharing,
 * such that the ksm page will get replaced by an exclusive anonymous page.
 *
 * We take great care only to touch a ksm page, in a VM_MERGEABLE vma,
 * in case the application has unmapped and remapped mm,addr meanwhile.
 * Could a ksm page appear anywhere else?  Actually yes, in a VM_PFNMAP
 * mmap of /dev/mem, where we would not want to touch it.
 *
 * FAULT_FLAG_REMOTE/FOLL_REMOTE are because we do this outside the context
 * of the process that owns 'vma'.  We also do not want to enforce
 * protection keys here anyway.
 */
static int break_ksm(struct vm_area_struct *vma, unsigned long addr, bool lock_vma)
{
	vm_fault_t ret = 0;
	const struct mm_walk_ops *ops = lock_vma ?
				&break_ksm_lock_vma_ops : &break_ksm_ops;

	do {
		int ksm_page;

		cond_resched();
		ksm_page = walk_page_range_vma(vma, addr, addr + 1, ops, NULL);
		if (WARN_ON_ONCE(ksm_page < 0))
			return ksm_page;
		if (!ksm_page)
			return 0;
		ret = handle_mm_fault(vma, addr,
				      FAULT_FLAG_UNSHARE | FAULT_FLAG_REMOTE,
				      NULL);
	} while (!(ret & (VM_FAULT_SIGBUS | VM_FAULT_SIGSEGV | VM_FAULT_OOM)));
	/*
	 * We must loop until we no longer find a KSM page because
	 * handle_mm_fault() may back out if there's any difficulty e.g. if
	 * pte accessed bit gets updated concurrently.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *
	 * VM_FAULT_SIGBUS could occur if we race with truncation of the
	 * backing file, which also invalidates anonymous pages: that's
	 * okay, that truncation will have unmapped the PageKsm for us.
	 *
	 * VM_FAULT_OOM: at the time of writing (late July 2009), setting
	 * aside mem_cgroup limits, VM_FAULT_OOM would only be set if the
	 * current task has TIF_MEMDIE set, and will be OOM killed on return
	 * to user; and ksmd, having no mm, would never be chosen for that.
	 *
	 * But if the mm is in a limited mem_cgroup, then the fault may fail
	 * with VM_FAULT_OOM even if the current task is not TIF_MEMDIE; and
	 * even ksmd can fail in this way - though it's usually breaking ksm
	 * just to undo a merge it made a moment before, so unlikely to oom.
	 *
	 * That's a pity: we might therefore have more kernel pages allocated
	 * than we're counting as nodes in the stable tree; but ksm_do_scan
	 * will retry to break_cow on each pass, so should recover the page
	 * in due course.  The important thing is to not let VM_MERGEABLE
	 * be cleared while any such pages might remain in the area.
	 */
	return (ret & VM_FAULT_OOM) ? -ENOMEM : 0;
}

<<<<<<< HEAD
=======
static bool vma_ksm_compatible(struct vm_area_struct *vma)
{
	if (vma->vm_flags & (VM_SHARED  | VM_MAYSHARE   | VM_PFNMAP  |
			     VM_IO      | VM_DONTEXPAND | VM_HUGETLB |
			     VM_MIXEDMAP))
		return false;		/* just ignore the advice */

	if (vma_is_dax(vma))
		return false;

#ifdef VM_SAO
	if (vma->vm_flags & VM_SAO)
		return false;
#endif
#ifdef VM_SPARC_ADI
	if (vma->vm_flags & VM_SPARC_ADI)
		return false;
#endif

	return true;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct vm_area_struct *find_mergeable_vma(struct mm_struct *mm,
		unsigned long addr)
{
	struct vm_area_struct *vma;
	if (ksm_test_exit(mm))
		return NULL;
<<<<<<< HEAD
	vma = find_vma(mm, addr);
	if (!vma || vma->vm_start > addr)
		return NULL;
	if (!(vma->vm_flags & VM_MERGEABLE) || !vma->anon_vma)
=======
	vma = vma_lookup(mm, addr);
	if (!vma || !(vma->vm_flags & VM_MERGEABLE) || !vma->anon_vma)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;
	return vma;
}

<<<<<<< HEAD
static void break_cow(struct rmap_item *rmap_item)
=======
static void break_cow(struct ksm_rmap_item *rmap_item)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mm_struct *mm = rmap_item->mm;
	unsigned long addr = rmap_item->address;
	struct vm_area_struct *vma;

	/*
	 * It is not an accident that whenever we want to break COW
	 * to undo, we also need to drop a reference to the anon_vma.
	 */
	put_anon_vma(rmap_item->anon_vma);

<<<<<<< HEAD
	down_read(&mm->mmap_sem);
	vma = find_mergeable_vma(mm, addr);
	if (vma)
		break_ksm(vma, addr);
	up_read(&mm->mmap_sem);
}

static struct page *page_trans_compound_anon(struct page *page)
{
	if (PageTransCompound(page)) {
		struct page *head = compound_trans_head(page);
		/*
		 * head may actually be splitted and freed from under
		 * us but it's ok here.
		 */
		if (PageAnon(head))
			return head;
	}
	return NULL;
}

static struct page *get_mergeable_page(struct rmap_item *rmap_item)
=======
	mmap_read_lock(mm);
	vma = find_mergeable_vma(mm, addr);
	if (vma)
		break_ksm(vma, addr, false);
	mmap_read_unlock(mm);
}

static struct page *get_mergeable_page(struct ksm_rmap_item *rmap_item)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mm_struct *mm = rmap_item->mm;
	unsigned long addr = rmap_item->address;
	struct vm_area_struct *vma;
	struct page *page;

<<<<<<< HEAD
	down_read(&mm->mmap_sem);
=======
	mmap_read_lock(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vma = find_mergeable_vma(mm, addr);
	if (!vma)
		goto out;

	page = follow_page(vma, addr, FOLL_GET);
	if (IS_ERR_OR_NULL(page))
		goto out;
<<<<<<< HEAD
	if (PageAnon(page) || page_trans_compound_anon(page)) {
		flush_anon_page(vma, page, addr);
		flush_dcache_page(page);
	} else {
		put_page(page);
out:		page = NULL;
	}
	up_read(&mm->mmap_sem);
	return page;
}

static void remove_node_from_stable_tree(struct stable_node *stable_node)
{
	struct rmap_item *rmap_item;
	struct hlist_node *hlist;

	hlist_for_each_entry(rmap_item, hlist, &stable_node->hlist, hlist) {
		if (rmap_item->hlist.next)
			ksm_pages_sharing--;
		else
			ksm_pages_shared--;
=======
	if (is_zone_device_page(page))
		goto out_putpage;
	if (PageAnon(page)) {
		flush_anon_page(vma, page, addr);
		flush_dcache_page(page);
	} else {
out_putpage:
		put_page(page);
out:
		page = NULL;
	}
	mmap_read_unlock(mm);
	return page;
}

/*
 * This helper is used for getting right index into array of tree roots.
 * When merge_across_nodes knob is set to 1, there are only two rb-trees for
 * stable and unstable pages from all nodes with roots in index 0. Otherwise,
 * every node has its own stable and unstable tree.
 */
static inline int get_kpfn_nid(unsigned long kpfn)
{
	return ksm_merge_across_nodes ? 0 : NUMA(pfn_to_nid(kpfn));
}

static struct ksm_stable_node *alloc_stable_node_chain(struct ksm_stable_node *dup,
						   struct rb_root *root)
{
	struct ksm_stable_node *chain = alloc_stable_node();
	VM_BUG_ON(is_stable_node_chain(dup));
	if (likely(chain)) {
		INIT_HLIST_HEAD(&chain->hlist);
		chain->chain_prune_time = jiffies;
		chain->rmap_hlist_len = STABLE_NODE_CHAIN;
#if defined (CONFIG_DEBUG_VM) && defined(CONFIG_NUMA)
		chain->nid = NUMA_NO_NODE; /* debug */
#endif
		ksm_stable_node_chains++;

		/*
		 * Put the stable node chain in the first dimension of
		 * the stable tree and at the same time remove the old
		 * stable node.
		 */
		rb_replace_node(&dup->node, &chain->node, root);

		/*
		 * Move the old stable node to the second dimension
		 * queued in the hlist_dup. The invariant is that all
		 * dup stable_nodes in the chain->hlist point to pages
		 * that are write protected and have the exact same
		 * content.
		 */
		stable_node_chain_add_dup(dup, chain);
	}
	return chain;
}

static inline void free_stable_node_chain(struct ksm_stable_node *chain,
					  struct rb_root *root)
{
	rb_erase(&chain->node, root);
	free_stable_node(chain);
	ksm_stable_node_chains--;
}

static void remove_node_from_stable_tree(struct ksm_stable_node *stable_node)
{
	struct ksm_rmap_item *rmap_item;

	/* check it's not STABLE_NODE_CHAIN or negative */
	BUG_ON(stable_node->rmap_hlist_len < 0);

	hlist_for_each_entry(rmap_item, &stable_node->hlist, hlist) {
		if (rmap_item->hlist.next) {
			ksm_pages_sharing--;
			trace_ksm_remove_rmap_item(stable_node->kpfn, rmap_item, rmap_item->mm);
		} else {
			ksm_pages_shared--;
		}

		rmap_item->mm->ksm_merging_pages--;

		VM_BUG_ON(stable_node->rmap_hlist_len <= 0);
		stable_node->rmap_hlist_len--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_anon_vma(rmap_item->anon_vma);
		rmap_item->address &= PAGE_MASK;
		cond_resched();
	}

<<<<<<< HEAD
	rb_erase(&stable_node->node, &root_stable_tree);
	free_stable_node(stable_node);
}

=======
	/*
	 * We need the second aligned pointer of the migrate_nodes
	 * list_head to stay clear from the rb_parent_color union
	 * (aligned and different than any node) and also different
	 * from &migrate_nodes. This will verify that future list.h changes
	 * don't break STABLE_NODE_DUP_HEAD. Only recent gcc can handle it.
	 */
	BUILD_BUG_ON(STABLE_NODE_DUP_HEAD <= &migrate_nodes);
	BUILD_BUG_ON(STABLE_NODE_DUP_HEAD >= &migrate_nodes + 1);

	trace_ksm_remove_ksm_page(stable_node->kpfn);
	if (stable_node->head == &migrate_nodes)
		list_del(&stable_node->list);
	else
		stable_node_dup_del(stable_node);
	free_stable_node(stable_node);
}

enum get_ksm_page_flags {
	GET_KSM_PAGE_NOLOCK,
	GET_KSM_PAGE_LOCK,
	GET_KSM_PAGE_TRYLOCK
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * get_ksm_page: checks if the page indicated by the stable node
 * is still its ksm page, despite having held no reference to it.
 * In which case we can trust the content of the page, and it
 * returns the gotten page; but if the page has now been zapped,
 * remove the stale node from the stable tree and return NULL.
 * But beware, the stable node's page might be being migrated.
 *
 * You would expect the stable_node to hold a reference to the ksm page.
 * But if it increments the page's count, swapping out has to wait for
 * ksmd to come around again before it can free the page, which may take
 * seconds or even minutes: much too unresponsive.  So instead we use a
 * "keyhole reference": access to the ksm page from the stable node peeps
 * out through its keyhole to see if that page still holds the right key,
 * pointing back to this stable node.  This relies on freeing a PageAnon
 * page to reset its page->mapping to NULL, and relies on no other use of
 * a page to put something that might look like our key in page->mapping.
 * is on its way to being freed; but it is an anomaly to bear in mind.
 */
<<<<<<< HEAD
static struct page *get_ksm_page(struct stable_node *stable_node, bool locked)
=======
static struct page *get_ksm_page(struct ksm_stable_node *stable_node,
				 enum get_ksm_page_flags flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct page *page;
	void *expected_mapping;
	unsigned long kpfn;

<<<<<<< HEAD
	expected_mapping = (void *)stable_node +
				(PAGE_MAPPING_ANON | PAGE_MAPPING_KSM);
again:
	kpfn = ACCESS_ONCE(stable_node->kpfn);
	page = pfn_to_page(kpfn);

	/*
	 * page is computed from kpfn, so on most architectures reading
	 * page->mapping is naturally ordered after reading node->kpfn,
	 * but on Alpha we need to be more careful.
	 */
	smp_read_barrier_depends();
	if (ACCESS_ONCE(page->mapping) != expected_mapping)
=======
	expected_mapping = (void *)((unsigned long)stable_node |
					PAGE_MAPPING_KSM);
again:
	kpfn = READ_ONCE(stable_node->kpfn); /* Address dependency. */
	page = pfn_to_page(kpfn);
	if (READ_ONCE(page->mapping) != expected_mapping)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto stale;

	/*
	 * We cannot do anything with the page while its refcount is 0.
	 * Usually 0 means free, or tail of a higher-order page: in which
	 * case this node is no longer referenced, and should be freed;
<<<<<<< HEAD
	 * however, it might mean that the page is under page_freeze_refs().
	 * The __remove_mapping() case is easy, again the node is now stale;
	 * but if page is swapcache in migrate_page_move_mapping(), it might
	 * still be our page, in which case it's essential to keep the node.
=======
	 * however, it might mean that the page is under page_ref_freeze().
	 * The __remove_mapping() case is easy, again the node is now stale;
	 * the same is in reuse_ksm_page() case; but if page is swapcache
	 * in folio_migrate_mapping(), it might still be our page,
	 * in which case it's essential to keep the node.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	while (!get_page_unless_zero(page)) {
		/*
		 * Another check for page->mapping != expected_mapping would
		 * work here too.  We have chosen the !PageSwapCache test to
		 * optimize the common case, when the page is or is about to
		 * be freed: PageSwapCache is cleared (under spin_lock_irq)
<<<<<<< HEAD
		 * in the freeze_refs section of __remove_mapping(); but Anon
=======
		 * in the ref_freeze section of __remove_mapping(); but Anon
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * page->mapping reset to NULL later, in free_pages_prepare().
		 */
		if (!PageSwapCache(page))
			goto stale;
		cpu_relax();
	}

<<<<<<< HEAD
	if (ACCESS_ONCE(page->mapping) != expected_mapping) {
=======
	if (READ_ONCE(page->mapping) != expected_mapping) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_page(page);
		goto stale;
	}

<<<<<<< HEAD
	if (locked) {
		lock_page(page);
		if (ACCESS_ONCE(page->mapping) != expected_mapping) {
=======
	if (flags == GET_KSM_PAGE_TRYLOCK) {
		if (!trylock_page(page)) {
			put_page(page);
			return ERR_PTR(-EBUSY);
		}
	} else if (flags == GET_KSM_PAGE_LOCK)
		lock_page(page);

	if (flags != GET_KSM_PAGE_NOLOCK) {
		if (READ_ONCE(page->mapping) != expected_mapping) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			unlock_page(page);
			put_page(page);
			goto stale;
		}
	}
	return page;

stale:
	/*
	 * We come here from above when page->mapping or !PageSwapCache
	 * suggests that the node is stale; but it might be under migration.
<<<<<<< HEAD
	 * We need smp_rmb(), matching the smp_wmb() in ksm_migrate_page(),
	 * before checking whether node->kpfn has been changed.
	 */
	smp_rmb();
	if (ACCESS_ONCE(stable_node->kpfn) != kpfn)
=======
	 * We need smp_rmb(), matching the smp_wmb() in folio_migrate_ksm(),
	 * before checking whether node->kpfn has been changed.
	 */
	smp_rmb();
	if (READ_ONCE(stable_node->kpfn) != kpfn)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto again;
	remove_node_from_stable_tree(stable_node);
	return NULL;
}

/*
 * Removing rmap_item from stable or unstable tree.
 * This function will clean the information from the stable/unstable tree.
 */
<<<<<<< HEAD
static void remove_rmap_item_from_tree(struct rmap_item *rmap_item)
{
	if (rmap_item->address & STABLE_FLAG) {
		struct stable_node *stable_node;
		struct page *page;

		stable_node = rmap_item->head;
		page = get_ksm_page(stable_node, true);
=======
static void remove_rmap_item_from_tree(struct ksm_rmap_item *rmap_item)
{
	if (rmap_item->address & STABLE_FLAG) {
		struct ksm_stable_node *stable_node;
		struct page *page;

		stable_node = rmap_item->head;
		page = get_ksm_page(stable_node, GET_KSM_PAGE_LOCK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!page)
			goto out;

		hlist_del(&rmap_item->hlist);
		unlock_page(page);
		put_page(page);

<<<<<<< HEAD
		if (stable_node->hlist.first)
=======
		if (!hlist_empty(&stable_node->hlist))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ksm_pages_sharing--;
		else
			ksm_pages_shared--;

<<<<<<< HEAD
		put_anon_vma(rmap_item->anon_vma);
=======
		rmap_item->mm->ksm_merging_pages--;

		VM_BUG_ON(stable_node->rmap_hlist_len <= 0);
		stable_node->rmap_hlist_len--;

		put_anon_vma(rmap_item->anon_vma);
		rmap_item->head = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rmap_item->address &= PAGE_MASK;

	} else if (rmap_item->address & UNSTABLE_FLAG) {
		unsigned char age;
		/*
		 * Usually ksmd can and must skip the rb_erase, because
		 * root_unstable_tree was already reset to RB_ROOT.
		 * But be careful when an mm is exiting: do the rb_erase
		 * if this rmap_item was inserted by this scan, rather
		 * than left over from before.
		 */
		age = (unsigned char)(ksm_scan.seqnr - rmap_item->address);
		BUG_ON(age > 1);
		if (!age)
<<<<<<< HEAD
			rb_erase(&rmap_item->node, &root_unstable_tree);

=======
			rb_erase(&rmap_item->node,
				 root_unstable_tree + NUMA(rmap_item->nid));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ksm_pages_unshared--;
		rmap_item->address &= PAGE_MASK;
	}
out:
	cond_resched();		/* we're called from many long loops */
}

<<<<<<< HEAD
static void remove_trailing_rmap_items(struct mm_slot *mm_slot,
				       struct rmap_item **rmap_list)
{
	while (*rmap_list) {
		struct rmap_item *rmap_item = *rmap_list;
=======
static void remove_trailing_rmap_items(struct ksm_rmap_item **rmap_list)
{
	while (*rmap_list) {
		struct ksm_rmap_item *rmap_item = *rmap_list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*rmap_list = rmap_item->rmap_list;
		remove_rmap_item_from_tree(rmap_item);
		free_rmap_item(rmap_item);
	}
}

/*
<<<<<<< HEAD
 * Though it's very tempting to unmerge in_stable_tree(rmap_item)s rather
 * than check every pte of a given vma, the locking doesn't quite work for
 * that - an rmap_item is assigned to the stable tree after inserting ksm
 * page and upping mmap_sem.  Nor does it fit with the way we skip dup'ing
=======
 * Though it's very tempting to unmerge rmap_items from stable tree rather
 * than check every pte of a given vma, the locking doesn't quite work for
 * that - an rmap_item is assigned to the stable tree after inserting ksm
 * page and upping mmap_lock.  Nor does it fit with the way we skip dup'ing
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * rmap_items from parent to child at fork time (so as not to waste time
 * if exit comes before the next scan reaches it).
 *
 * Similarly, although we'd like to remove rmap_items (so updating counts
 * and freeing memory) when unmerging an area, it's easier to leave that
 * to the next pass of ksmd - consider, for example, how ksmd might be
 * in cmp_and_merge_page on one of the rmap_items we would be removing.
 */
static int unmerge_ksm_pages(struct vm_area_struct *vma,
<<<<<<< HEAD
			     unsigned long start, unsigned long end)
=======
			     unsigned long start, unsigned long end, bool lock_vma)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long addr;
	int err = 0;

	for (addr = start; addr < end && !err; addr += PAGE_SIZE) {
		if (ksm_test_exit(vma->vm_mm))
			break;
		if (signal_pending(current))
			err = -ERESTARTSYS;
		else
<<<<<<< HEAD
			err = break_ksm(vma, addr);
=======
			err = break_ksm(vma, addr, lock_vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return err;
}

<<<<<<< HEAD
=======
static inline struct ksm_stable_node *folio_stable_node(struct folio *folio)
{
	return folio_test_ksm(folio) ? folio_raw_mapping(folio) : NULL;
}

static inline struct ksm_stable_node *page_stable_node(struct page *page)
{
	return folio_stable_node(page_folio(page));
}

static inline void set_page_stable_node(struct page *page,
					struct ksm_stable_node *stable_node)
{
	VM_BUG_ON_PAGE(PageAnon(page) && PageAnonExclusive(page), page);
	page->mapping = (void *)((unsigned long)stable_node | PAGE_MAPPING_KSM);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SYSFS
/*
 * Only called through the sysfs control interface:
 */
<<<<<<< HEAD
static int unmerge_and_remove_all_rmap_items(void)
{
	struct mm_slot *mm_slot;
=======
static int remove_stable_node(struct ksm_stable_node *stable_node)
{
	struct page *page;
	int err;

	page = get_ksm_page(stable_node, GET_KSM_PAGE_LOCK);
	if (!page) {
		/*
		 * get_ksm_page did remove_node_from_stable_tree itself.
		 */
		return 0;
	}

	/*
	 * Page could be still mapped if this races with __mmput() running in
	 * between ksm_exit() and exit_mmap(). Just refuse to let
	 * merge_across_nodes/max_page_sharing be switched.
	 */
	err = -EBUSY;
	if (!page_mapped(page)) {
		/*
		 * The stable node did not yet appear stale to get_ksm_page(),
		 * since that allows for an unmapped ksm page to be recognized
		 * right up until it is freed; but the node is safe to remove.
		 * This page might be in an LRU cache waiting to be freed,
		 * or it might be PageSwapCache (perhaps under writeback),
		 * or it might have been removed from swapcache a moment ago.
		 */
		set_page_stable_node(page, NULL);
		remove_node_from_stable_tree(stable_node);
		err = 0;
	}

	unlock_page(page);
	put_page(page);
	return err;
}

static int remove_stable_node_chain(struct ksm_stable_node *stable_node,
				    struct rb_root *root)
{
	struct ksm_stable_node *dup;
	struct hlist_node *hlist_safe;

	if (!is_stable_node_chain(stable_node)) {
		VM_BUG_ON(is_stable_node_dup(stable_node));
		if (remove_stable_node(stable_node))
			return true;
		else
			return false;
	}

	hlist_for_each_entry_safe(dup, hlist_safe,
				  &stable_node->hlist, hlist_dup) {
		VM_BUG_ON(!is_stable_node_dup(dup));
		if (remove_stable_node(dup))
			return true;
	}
	BUG_ON(!hlist_empty(&stable_node->hlist));
	free_stable_node_chain(stable_node, root);
	return false;
}

static int remove_all_stable_nodes(void)
{
	struct ksm_stable_node *stable_node, *next;
	int nid;
	int err = 0;

	for (nid = 0; nid < ksm_nr_node_ids; nid++) {
		while (root_stable_tree[nid].rb_node) {
			stable_node = rb_entry(root_stable_tree[nid].rb_node,
						struct ksm_stable_node, node);
			if (remove_stable_node_chain(stable_node,
						     root_stable_tree + nid)) {
				err = -EBUSY;
				break;	/* proceed to next nid */
			}
			cond_resched();
		}
	}
	list_for_each_entry_safe(stable_node, next, &migrate_nodes, list) {
		if (remove_stable_node(stable_node))
			err = -EBUSY;
		cond_resched();
	}
	return err;
}

static int unmerge_and_remove_all_rmap_items(void)
{
	struct ksm_mm_slot *mm_slot;
	struct mm_slot *slot;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	int err = 0;

	spin_lock(&ksm_mmlist_lock);
<<<<<<< HEAD
	ksm_scan.mm_slot = list_entry(ksm_mm_head.mm_list.next,
						struct mm_slot, mm_list);
	spin_unlock(&ksm_mmlist_lock);

	for (mm_slot = ksm_scan.mm_slot;
			mm_slot != &ksm_mm_head; mm_slot = ksm_scan.mm_slot) {
		mm = mm_slot->mm;
		down_read(&mm->mmap_sem);
		for (vma = mm->mmap; vma; vma = vma->vm_next) {
			if (ksm_test_exit(mm))
				break;
			if (!(vma->vm_flags & VM_MERGEABLE) || !vma->anon_vma)
				continue;
			err = unmerge_ksm_pages(vma,
						vma->vm_start, vma->vm_end);
=======
	slot = list_entry(ksm_mm_head.slot.mm_node.next,
			  struct mm_slot, mm_node);
	ksm_scan.mm_slot = mm_slot_entry(slot, struct ksm_mm_slot, slot);
	spin_unlock(&ksm_mmlist_lock);

	for (mm_slot = ksm_scan.mm_slot; mm_slot != &ksm_mm_head;
	     mm_slot = ksm_scan.mm_slot) {
		VMA_ITERATOR(vmi, mm_slot->slot.mm, 0);

		mm = mm_slot->slot.mm;
		mmap_read_lock(mm);

		/*
		 * Exit right away if mm is exiting to avoid lockdep issue in
		 * the maple tree
		 */
		if (ksm_test_exit(mm))
			goto mm_exiting;

		for_each_vma(vmi, vma) {
			if (!(vma->vm_flags & VM_MERGEABLE) || !vma->anon_vma)
				continue;
			err = unmerge_ksm_pages(vma,
						vma->vm_start, vma->vm_end, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err)
				goto error;
		}

<<<<<<< HEAD
		remove_trailing_rmap_items(mm_slot, &mm_slot->rmap_list);

		spin_lock(&ksm_mmlist_lock);
		ksm_scan.mm_slot = list_entry(mm_slot->mm_list.next,
						struct mm_slot, mm_list);
		if (ksm_test_exit(mm)) {
			hlist_del(&mm_slot->link);
			list_del(&mm_slot->mm_list);
			spin_unlock(&ksm_mmlist_lock);

			free_mm_slot(mm_slot);
			clear_bit(MMF_VM_MERGEABLE, &mm->flags);
			up_read(&mm->mmap_sem);
			mmdrop(mm);
		} else {
			spin_unlock(&ksm_mmlist_lock);
			up_read(&mm->mmap_sem);
		}
	}

=======
mm_exiting:
		remove_trailing_rmap_items(&mm_slot->rmap_list);
		mmap_read_unlock(mm);

		spin_lock(&ksm_mmlist_lock);
		slot = list_entry(mm_slot->slot.mm_node.next,
				  struct mm_slot, mm_node);
		ksm_scan.mm_slot = mm_slot_entry(slot, struct ksm_mm_slot, slot);
		if (ksm_test_exit(mm)) {
			hash_del(&mm_slot->slot.hash);
			list_del(&mm_slot->slot.mm_node);
			spin_unlock(&ksm_mmlist_lock);

			mm_slot_free(mm_slot_cache, mm_slot);
			clear_bit(MMF_VM_MERGEABLE, &mm->flags);
			clear_bit(MMF_VM_MERGE_ANY, &mm->flags);
			mmdrop(mm);
		} else
			spin_unlock(&ksm_mmlist_lock);
	}

	/* Clean up stable nodes, but don't worry if some are still busy */
	remove_all_stable_nodes();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ksm_scan.seqnr = 0;
	return 0;

error:
<<<<<<< HEAD
	up_read(&mm->mmap_sem);
=======
	mmap_read_unlock(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&ksm_mmlist_lock);
	ksm_scan.mm_slot = &ksm_mm_head;
	spin_unlock(&ksm_mmlist_lock);
	return err;
}
#endif /* CONFIG_SYSFS */

static u32 calc_checksum(struct page *page)
{
	u32 checksum;
<<<<<<< HEAD
	void *addr = kmap_atomic(page);
	checksum = jhash2(addr, PAGE_SIZE / 4, 17);
	kunmap_atomic(addr);
	return checksum;
}

static int memcmp_pages(struct page *page1, struct page *page2)
{
	char *addr1, *addr2;
	int ret;

	addr1 = kmap_atomic(page1);
	addr2 = kmap_atomic(page2);
#ifdef __HAVE_ARCH_PAGECMP
	ret = cmp_page(addr1, addr2);
#else
	ret = memcmp(addr1, addr2, PAGE_SIZE);
#endif
	kunmap_atomic(addr2);
	kunmap_atomic(addr1);
	return ret;
}

static inline int pages_identical(struct page *page1, struct page *page2)
{
	return !memcmp_pages(page1, page2);
}

=======
	void *addr = kmap_local_page(page);
	checksum = xxhash(addr, PAGE_SIZE, 0);
	kunmap_local(addr);
	return checksum;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int write_protect_page(struct vm_area_struct *vma, struct page *page,
			      pte_t *orig_pte)
{
	struct mm_struct *mm = vma->vm_mm;
<<<<<<< HEAD
	unsigned long addr;
	pte_t *ptep;
	spinlock_t *ptl;
	int swapped;
	int err = -EFAULT;

	addr = page_address_in_vma(page, vma);
	if (addr == -EFAULT)
		goto out;

	BUG_ON(PageTransCompound(page));
	ptep = page_check_address(page, mm, addr, &ptl, 0);
	if (!ptep)
		goto out;

	if (pte_write(*ptep) || pte_dirty(*ptep)) {
		pte_t entry;

		swapped = PageSwapCache(page);
		flush_cache_page(vma, addr, page_to_pfn(page));
		/*
		 * Ok this is tricky, when get_user_pages_fast() run it doesn't
		 * take any lock, therefore the check that we are going to make
		 * with the pagecount against the mapcount is racey and
=======
	DEFINE_PAGE_VMA_WALK(pvmw, page, vma, 0, 0);
	int swapped;
	int err = -EFAULT;
	struct mmu_notifier_range range;
	bool anon_exclusive;
	pte_t entry;

	pvmw.address = page_address_in_vma(page, vma);
	if (pvmw.address == -EFAULT)
		goto out;

	BUG_ON(PageTransCompound(page));

	mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, mm, pvmw.address,
				pvmw.address + PAGE_SIZE);
	mmu_notifier_invalidate_range_start(&range);

	if (!page_vma_mapped_walk(&pvmw))
		goto out_mn;
	if (WARN_ONCE(!pvmw.pte, "Unexpected PMD mapping?"))
		goto out_unlock;

	anon_exclusive = PageAnonExclusive(page);
	entry = ptep_get(pvmw.pte);
	if (pte_write(entry) || pte_dirty(entry) ||
	    anon_exclusive || mm_tlb_flush_pending(mm)) {
		swapped = PageSwapCache(page);
		flush_cache_page(vma, pvmw.address, page_to_pfn(page));
		/*
		 * Ok this is tricky, when get_user_pages_fast() run it doesn't
		 * take any lock, therefore the check that we are going to make
		 * with the pagecount against the mapcount is racy and
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * O_DIRECT can happen right after the check.
		 * So we clear the pte and flush the tlb before the check
		 * this assure us that no O_DIRECT can happen after the check
		 * or in the middle of the check.
<<<<<<< HEAD
		 */
		entry = ptep_clear_flush(vma, addr, ptep);
=======
		 *
		 * No need to notify as we are downgrading page table to read
		 * only not changing it to point to a new page.
		 *
		 * See Documentation/mm/mmu_notifier.rst
		 */
		entry = ptep_clear_flush(vma, pvmw.address, pvmw.pte);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Check that no O_DIRECT or similar I/O is in progress on the
		 * page
		 */
		if (page_mapcount(page) + 1 + swapped != page_count(page)) {
<<<<<<< HEAD
			set_pte_at(mm, addr, ptep, entry);
			goto out_unlock;
		}
		if (pte_dirty(entry))
			set_page_dirty(page);
		entry = pte_mkclean(pte_wrprotect(entry));
		set_pte_at_notify(mm, addr, ptep, entry);
	}
	*orig_pte = *ptep;
	err = 0;

out_unlock:
	pte_unmap_unlock(ptep, ptl);
=======
			set_pte_at(mm, pvmw.address, pvmw.pte, entry);
			goto out_unlock;
		}

		/* See folio_try_share_anon_rmap_pte(): clear PTE first. */
		if (anon_exclusive &&
		    folio_try_share_anon_rmap_pte(page_folio(page), page)) {
			set_pte_at(mm, pvmw.address, pvmw.pte, entry);
			goto out_unlock;
		}

		if (pte_dirty(entry))
			set_page_dirty(page);
		entry = pte_mkclean(entry);

		if (pte_write(entry))
			entry = pte_wrprotect(entry);

		set_pte_at_notify(mm, pvmw.address, pvmw.pte, entry);
	}
	*orig_pte = entry;
	err = 0;

out_unlock:
	page_vma_mapped_walk_done(&pvmw);
out_mn:
	mmu_notifier_invalidate_range_end(&range);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;
}

/**
 * replace_page - replace page in vma by new ksm page
 * @vma:      vma that holds the pte pointing to page
 * @page:     the page we are replacing by kpage
 * @kpage:    the ksm page we replace page by
 * @orig_pte: the original value of the pte
 *
 * Returns 0 on success, -EFAULT on failure.
 */
static int replace_page(struct vm_area_struct *vma, struct page *page,
			struct page *kpage, pte_t orig_pte)
{
<<<<<<< HEAD
	struct mm_struct *mm = vma->vm_mm;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *ptep;
	spinlock_t *ptl;
	unsigned long addr;
	int err = -EFAULT;
=======
	struct folio *kfolio = page_folio(kpage);
	struct mm_struct *mm = vma->vm_mm;
	struct folio *folio;
	pmd_t *pmd;
	pmd_t pmde;
	pte_t *ptep;
	pte_t newpte;
	spinlock_t *ptl;
	unsigned long addr;
	int err = -EFAULT;
	struct mmu_notifier_range range;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	addr = page_address_in_vma(page, vma);
	if (addr == -EFAULT)
		goto out;

<<<<<<< HEAD
	pgd = pgd_offset(mm, addr);
	if (!pgd_present(*pgd))
		goto out;

	pud = pud_offset(pgd, addr);
	if (!pud_present(*pud))
		goto out;

	pmd = pmd_offset(pud, addr);
	BUG_ON(pmd_trans_huge(*pmd));
	if (!pmd_present(*pmd))
		goto out;

	ptep = pte_offset_map_lock(mm, pmd, addr, &ptl);
	if (!pte_same(*ptep, orig_pte)) {
		pte_unmap_unlock(ptep, ptl);
		goto out;
	}

	get_page(kpage);
	page_add_anon_rmap(kpage, vma, addr);

	flush_cache_page(vma, addr, pte_pfn(*ptep));
	ptep_clear_flush(vma, addr, ptep);
	set_pte_at_notify(mm, addr, ptep, mk_pte(kpage, vma->vm_page_prot));

	page_remove_rmap(page);
	if (!page_mapped(page))
		try_to_free_swap(page);
	put_page(page);

	pte_unmap_unlock(ptep, ptl);
	err = 0;
=======
	pmd = mm_find_pmd(mm, addr);
	if (!pmd)
		goto out;
	/*
	 * Some THP functions use the sequence pmdp_huge_clear_flush(), set_pmd_at()
	 * without holding anon_vma lock for write.  So when looking for a
	 * genuine pmde (in which to find pte), test present and !THP together.
	 */
	pmde = pmdp_get_lockless(pmd);
	if (!pmd_present(pmde) || pmd_trans_huge(pmde))
		goto out;

	mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, mm, addr,
				addr + PAGE_SIZE);
	mmu_notifier_invalidate_range_start(&range);

	ptep = pte_offset_map_lock(mm, pmd, addr, &ptl);
	if (!ptep)
		goto out_mn;
	if (!pte_same(ptep_get(ptep), orig_pte)) {
		pte_unmap_unlock(ptep, ptl);
		goto out_mn;
	}
	VM_BUG_ON_PAGE(PageAnonExclusive(page), page);
	VM_BUG_ON_FOLIO(folio_test_anon(kfolio) && PageAnonExclusive(kpage),
			kfolio);

	/*
	 * No need to check ksm_use_zero_pages here: we can only have a
	 * zero_page here if ksm_use_zero_pages was enabled already.
	 */
	if (!is_zero_pfn(page_to_pfn(kpage))) {
		folio_get(kfolio);
		folio_add_anon_rmap_pte(kfolio, kpage, vma, addr, RMAP_NONE);
		newpte = mk_pte(kpage, vma->vm_page_prot);
	} else {
		/*
		 * Use pte_mkdirty to mark the zero page mapped by KSM, and then
		 * we can easily track all KSM-placed zero pages by checking if
		 * the dirty bit in zero page's PTE is set.
		 */
		newpte = pte_mkdirty(pte_mkspecial(pfn_pte(page_to_pfn(kpage), vma->vm_page_prot)));
		ksm_zero_pages++;
		mm->ksm_zero_pages++;
		/*
		 * We're replacing an anonymous page with a zero page, which is
		 * not anonymous. We need to do proper accounting otherwise we
		 * will get wrong values in /proc, and a BUG message in dmesg
		 * when tearing down the mm.
		 */
		dec_mm_counter(mm, MM_ANONPAGES);
	}

	flush_cache_page(vma, addr, pte_pfn(ptep_get(ptep)));
	/*
	 * No need to notify as we are replacing a read only page with another
	 * read only page with the same content.
	 *
	 * See Documentation/mm/mmu_notifier.rst
	 */
	ptep_clear_flush(vma, addr, ptep);
	set_pte_at_notify(mm, addr, ptep, newpte);

	folio = page_folio(page);
	folio_remove_rmap_pte(folio, page, vma);
	if (!folio_mapped(folio))
		folio_free_swap(folio);
	folio_put(folio);

	pte_unmap_unlock(ptep, ptl);
	err = 0;
out_mn:
	mmu_notifier_invalidate_range_end(&range);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;
}

<<<<<<< HEAD
static int page_trans_compound_anon_split(struct page *page)
{
	int ret = 0;
	struct page *transhuge_head = page_trans_compound_anon(page);
	if (transhuge_head) {
		/* Get the reference on the head to split it. */
		if (get_page_unless_zero(transhuge_head)) {
			/*
			 * Recheck we got the reference while the head
			 * was still anonymous.
			 */
			if (PageAnon(transhuge_head))
				ret = split_huge_page(transhuge_head);
			else
				/*
				 * Retry later if split_huge_page run
				 * from under us.
				 */
				ret = 1;
			put_page(transhuge_head);
		} else
			/* Retry later if split_huge_page run from under us. */
			ret = 1;
	}
	return ret;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * try_to_merge_one_page - take two pages and merge them into one
 * @vma: the vma that holds the pte pointing to page
 * @page: the PageAnon page that we want to replace with kpage
 * @kpage: the PageKsm page that we want to map instead of page,
 *         or NULL the first time when we want to use page as kpage.
 *
 * This function returns 0 if the pages were merged, -EFAULT otherwise.
 */
static int try_to_merge_one_page(struct vm_area_struct *vma,
				 struct page *page, struct page *kpage)
{
	pte_t orig_pte = __pte(0);
	int err = -EFAULT;

	if (page == kpage)			/* ksm page forked */
		return 0;

<<<<<<< HEAD
	if (!(vma->vm_flags & VM_MERGEABLE))
		goto out;
	if (PageTransCompound(page) && page_trans_compound_anon_split(page))
		goto out;
	BUG_ON(PageTransCompound(page));
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!PageAnon(page))
		goto out;

	/*
	 * We need the page lock to read a stable PageSwapCache in
	 * write_protect_page().  We use trylock_page() instead of
	 * lock_page() because we don't want to wait here - we
	 * prefer to continue scanning and merging different pages,
	 * then come back to this page when it is unlocked.
	 */
	if (!trylock_page(page))
		goto out;
<<<<<<< HEAD
=======

	if (PageTransCompound(page)) {
		if (split_huge_page(page))
			goto out_unlock;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * If this anonymous page is mapped only here, its pte may need
	 * to be write-protected.  If it's mapped elsewhere, all of its
	 * ptes are necessarily already write-protected.  But in either
	 * case, we need to lock and check page_count is not raised.
	 */
	if (write_protect_page(vma, page, &orig_pte) == 0) {
		if (!kpage) {
			/*
			 * While we hold page lock, upgrade page from
			 * PageAnon+anon_vma to PageKsm+NULL stable_node:
			 * stable_tree_insert() will update stable_node.
			 */
			set_page_stable_node(page, NULL);
			mark_page_accessed(page);
<<<<<<< HEAD
=======
			/*
			 * Page reclaim just frees a clean page with no dirty
			 * ptes: make sure that the ksm page would be swapped.
			 */
			if (!PageDirty(page))
				SetPageDirty(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = 0;
		} else if (pages_identical(page, kpage))
			err = replace_page(vma, page, kpage, orig_pte);
	}

<<<<<<< HEAD
	if ((vma->vm_flags & VM_LOCKED) && kpage && !err) {
		munlock_vma_page(page);
		if (!PageMlocked(kpage)) {
			unlock_page(page);
			lock_page(kpage);
			mlock_vma_page(kpage);
			page = kpage;		/* for final unlock */
		}
	}

=======
out_unlock:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unlock_page(page);
out:
	return err;
}

/*
 * try_to_merge_with_ksm_page - like try_to_merge_two_pages,
 * but no new kernel page is allocated: kpage must already be a ksm page.
 *
 * This function returns 0 if the pages were merged, -EFAULT otherwise.
 */
<<<<<<< HEAD
static int try_to_merge_with_ksm_page(struct rmap_item *rmap_item,
=======
static int try_to_merge_with_ksm_page(struct ksm_rmap_item *rmap_item,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				      struct page *page, struct page *kpage)
{
	struct mm_struct *mm = rmap_item->mm;
	struct vm_area_struct *vma;
	int err = -EFAULT;

<<<<<<< HEAD
	down_read(&mm->mmap_sem);
	if (ksm_test_exit(mm))
		goto out;
	vma = find_vma(mm, rmap_item->address);
	if (!vma || vma->vm_start > rmap_item->address)
=======
	mmap_read_lock(mm);
	vma = find_mergeable_vma(mm, rmap_item->address);
	if (!vma)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	err = try_to_merge_one_page(vma, page, kpage);
	if (err)
		goto out;

<<<<<<< HEAD
	/* Must get reference to anon_vma while still holding mmap_sem */
	rmap_item->anon_vma = vma->anon_vma;
	get_anon_vma(vma->anon_vma);
out:
	up_read(&mm->mmap_sem);
=======
	/* Unstable nid is in union with stable anon_vma: remove first */
	remove_rmap_item_from_tree(rmap_item);

	/* Must get reference to anon_vma while still holding mmap_lock */
	rmap_item->anon_vma = vma->anon_vma;
	get_anon_vma(vma->anon_vma);
out:
	mmap_read_unlock(mm);
	trace_ksm_merge_with_ksm_page(kpage, page_to_pfn(kpage ? kpage : page),
				rmap_item, mm, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/*
 * try_to_merge_two_pages - take two identical pages and prepare them
 * to be merged into one page.
 *
 * This function returns the kpage if we successfully merged two identical
 * pages into one ksm page, NULL otherwise.
 *
 * Note that this function upgrades page to ksm page: if one of the pages
 * is already a ksm page, try_to_merge_with_ksm_page should be used.
 */
<<<<<<< HEAD
static struct page *try_to_merge_two_pages(struct rmap_item *rmap_item,
					   struct page *page,
					   struct rmap_item *tree_rmap_item,
=======
static struct page *try_to_merge_two_pages(struct ksm_rmap_item *rmap_item,
					   struct page *page,
					   struct ksm_rmap_item *tree_rmap_item,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					   struct page *tree_page)
{
	int err;

	err = try_to_merge_with_ksm_page(rmap_item, page, NULL);
	if (!err) {
		err = try_to_merge_with_ksm_page(tree_rmap_item,
							tree_page, page);
		/*
		 * If that fails, we have a ksm page with only one pte
		 * pointing to it: so break it.
		 */
		if (err)
			break_cow(rmap_item);
	}
	return err ? NULL : page;
}

<<<<<<< HEAD
=======
static __always_inline
bool __is_page_sharing_candidate(struct ksm_stable_node *stable_node, int offset)
{
	VM_BUG_ON(stable_node->rmap_hlist_len < 0);
	/*
	 * Check that at least one mapping still exists, otherwise
	 * there's no much point to merge and share with this
	 * stable_node, as the underlying tree_page of the other
	 * sharer is going to be freed soon.
	 */
	return stable_node->rmap_hlist_len &&
		stable_node->rmap_hlist_len + offset < ksm_max_page_sharing;
}

static __always_inline
bool is_page_sharing_candidate(struct ksm_stable_node *stable_node)
{
	return __is_page_sharing_candidate(stable_node, 0);
}

static struct page *stable_node_dup(struct ksm_stable_node **_stable_node_dup,
				    struct ksm_stable_node **_stable_node,
				    struct rb_root *root,
				    bool prune_stale_stable_nodes)
{
	struct ksm_stable_node *dup, *found = NULL, *stable_node = *_stable_node;
	struct hlist_node *hlist_safe;
	struct page *_tree_page, *tree_page = NULL;
	int nr = 0;
	int found_rmap_hlist_len;

	if (!prune_stale_stable_nodes ||
	    time_before(jiffies, stable_node->chain_prune_time +
			msecs_to_jiffies(
				ksm_stable_node_chains_prune_millisecs)))
		prune_stale_stable_nodes = false;
	else
		stable_node->chain_prune_time = jiffies;

	hlist_for_each_entry_safe(dup, hlist_safe,
				  &stable_node->hlist, hlist_dup) {
		cond_resched();
		/*
		 * We must walk all stable_node_dup to prune the stale
		 * stable nodes during lookup.
		 *
		 * get_ksm_page can drop the nodes from the
		 * stable_node->hlist if they point to freed pages
		 * (that's why we do a _safe walk). The "dup"
		 * stable_node parameter itself will be freed from
		 * under us if it returns NULL.
		 */
		_tree_page = get_ksm_page(dup, GET_KSM_PAGE_NOLOCK);
		if (!_tree_page)
			continue;
		nr += 1;
		if (is_page_sharing_candidate(dup)) {
			if (!found ||
			    dup->rmap_hlist_len > found_rmap_hlist_len) {
				if (found)
					put_page(tree_page);
				found = dup;
				found_rmap_hlist_len = found->rmap_hlist_len;
				tree_page = _tree_page;

				/* skip put_page for found dup */
				if (!prune_stale_stable_nodes)
					break;
				continue;
			}
		}
		put_page(_tree_page);
	}

	if (found) {
		/*
		 * nr is counting all dups in the chain only if
		 * prune_stale_stable_nodes is true, otherwise we may
		 * break the loop at nr == 1 even if there are
		 * multiple entries.
		 */
		if (prune_stale_stable_nodes && nr == 1) {
			/*
			 * If there's not just one entry it would
			 * corrupt memory, better BUG_ON. In KSM
			 * context with no lock held it's not even
			 * fatal.
			 */
			BUG_ON(stable_node->hlist.first->next);

			/*
			 * There's just one entry and it is below the
			 * deduplication limit so drop the chain.
			 */
			rb_replace_node(&stable_node->node, &found->node,
					root);
			free_stable_node(stable_node);
			ksm_stable_node_chains--;
			ksm_stable_node_dups--;
			/*
			 * NOTE: the caller depends on the stable_node
			 * to be equal to stable_node_dup if the chain
			 * was collapsed.
			 */
			*_stable_node = found;
			/*
			 * Just for robustness, as stable_node is
			 * otherwise left as a stable pointer, the
			 * compiler shall optimize it away at build
			 * time.
			 */
			stable_node = NULL;
		} else if (stable_node->hlist.first != &found->hlist_dup &&
			   __is_page_sharing_candidate(found, 1)) {
			/*
			 * If the found stable_node dup can accept one
			 * more future merge (in addition to the one
			 * that is underway) and is not at the head of
			 * the chain, put it there so next search will
			 * be quicker in the !prune_stale_stable_nodes
			 * case.
			 *
			 * NOTE: it would be inaccurate to use nr > 1
			 * instead of checking the hlist.first pointer
			 * directly, because in the
			 * prune_stale_stable_nodes case "nr" isn't
			 * the position of the found dup in the chain,
			 * but the total number of dups in the chain.
			 */
			hlist_del(&found->hlist_dup);
			hlist_add_head(&found->hlist_dup,
				       &stable_node->hlist);
		}
	}

	*_stable_node_dup = found;
	return tree_page;
}

static struct ksm_stable_node *stable_node_dup_any(struct ksm_stable_node *stable_node,
					       struct rb_root *root)
{
	if (!is_stable_node_chain(stable_node))
		return stable_node;
	if (hlist_empty(&stable_node->hlist)) {
		free_stable_node_chain(stable_node, root);
		return NULL;
	}
	return hlist_entry(stable_node->hlist.first,
			   typeof(*stable_node), hlist_dup);
}

/*
 * Like for get_ksm_page, this function can free the *_stable_node and
 * *_stable_node_dup if the returned tree_page is NULL.
 *
 * It can also free and overwrite *_stable_node with the found
 * stable_node_dup if the chain is collapsed (in which case
 * *_stable_node will be equal to *_stable_node_dup like if the chain
 * never existed). It's up to the caller to verify tree_page is not
 * NULL before dereferencing *_stable_node or *_stable_node_dup.
 *
 * *_stable_node_dup is really a second output parameter of this
 * function and will be overwritten in all cases, the caller doesn't
 * need to initialize it.
 */
static struct page *__stable_node_chain(struct ksm_stable_node **_stable_node_dup,
					struct ksm_stable_node **_stable_node,
					struct rb_root *root,
					bool prune_stale_stable_nodes)
{
	struct ksm_stable_node *stable_node = *_stable_node;
	if (!is_stable_node_chain(stable_node)) {
		if (is_page_sharing_candidate(stable_node)) {
			*_stable_node_dup = stable_node;
			return get_ksm_page(stable_node, GET_KSM_PAGE_NOLOCK);
		}
		/*
		 * _stable_node_dup set to NULL means the stable_node
		 * reached the ksm_max_page_sharing limit.
		 */
		*_stable_node_dup = NULL;
		return NULL;
	}
	return stable_node_dup(_stable_node_dup, _stable_node, root,
			       prune_stale_stable_nodes);
}

static __always_inline struct page *chain_prune(struct ksm_stable_node **s_n_d,
						struct ksm_stable_node **s_n,
						struct rb_root *root)
{
	return __stable_node_chain(s_n_d, s_n, root, true);
}

static __always_inline struct page *chain(struct ksm_stable_node **s_n_d,
					  struct ksm_stable_node *s_n,
					  struct rb_root *root)
{
	struct ksm_stable_node *old_stable_node = s_n;
	struct page *tree_page;

	tree_page = __stable_node_chain(s_n_d, &s_n, root, false);
	/* not pruning dups so s_n cannot have changed */
	VM_BUG_ON(s_n != old_stable_node);
	return tree_page;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * stable_tree_search - search for page inside the stable tree
 *
 * This function checks if there is a page inside the stable tree
 * with identical content to the page that we are scanning right now.
 *
 * This function returns the stable tree node of identical content if found,
 * NULL otherwise.
 */
static struct page *stable_tree_search(struct page *page)
{
<<<<<<< HEAD
	struct rb_node *node = root_stable_tree.rb_node;
	struct stable_node *stable_node;

	stable_node = page_stable_node(page);
	if (stable_node) {			/* ksm page forked */
=======
	int nid;
	struct rb_root *root;
	struct rb_node **new;
	struct rb_node *parent;
	struct ksm_stable_node *stable_node, *stable_node_dup, *stable_node_any;
	struct ksm_stable_node *page_node;

	page_node = page_stable_node(page);
	if (page_node && page_node->head != &migrate_nodes) {
		/* ksm page forked */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		get_page(page);
		return page;
	}

<<<<<<< HEAD
	while (node) {
=======
	nid = get_kpfn_nid(page_to_pfn(page));
	root = root_stable_tree + nid;
again:
	new = &root->rb_node;
	parent = NULL;

	while (*new) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct page *tree_page;
		int ret;

		cond_resched();
<<<<<<< HEAD
		stable_node = rb_entry(node, struct stable_node, node);
		tree_page = get_ksm_page(stable_node, false);
		if (!tree_page)
			return NULL;
=======
		stable_node = rb_entry(*new, struct ksm_stable_node, node);
		stable_node_any = NULL;
		tree_page = chain_prune(&stable_node_dup, &stable_node,	root);
		/*
		 * NOTE: stable_node may have been freed by
		 * chain_prune() if the returned stable_node_dup is
		 * not NULL. stable_node_dup may have been inserted in
		 * the rbtree instead as a regular stable_node (in
		 * order to collapse the stable_node chain if a single
		 * stable_node dup was found in it). In such case the
		 * stable_node is overwritten by the callee to point
		 * to the stable_node_dup that was collapsed in the
		 * stable rbtree and stable_node will be equal to
		 * stable_node_dup like if the chain never existed.
		 */
		if (!stable_node_dup) {
			/*
			 * Either all stable_node dups were full in
			 * this stable_node chain, or this chain was
			 * empty and should be rb_erased.
			 */
			stable_node_any = stable_node_dup_any(stable_node,
							      root);
			if (!stable_node_any) {
				/* rb_erase just run */
				goto again;
			}
			/*
			 * Take any of the stable_node dups page of
			 * this stable_node chain to let the tree walk
			 * continue. All KSM pages belonging to the
			 * stable_node dups in a stable_node chain
			 * have the same content and they're
			 * write protected at all times. Any will work
			 * fine to continue the walk.
			 */
			tree_page = get_ksm_page(stable_node_any,
						 GET_KSM_PAGE_NOLOCK);
		}
		VM_BUG_ON(!stable_node_dup ^ !!stable_node_any);
		if (!tree_page) {
			/*
			 * If we walked over a stale stable_node,
			 * get_ksm_page() will call rb_erase() and it
			 * may rebalance the tree from under us. So
			 * restart the search from scratch. Returning
			 * NULL would be safe too, but we'd generate
			 * false negative insertions just because some
			 * stable_node was stale.
			 */
			goto again;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ret = memcmp_pages(page, tree_page);
		put_page(tree_page);

<<<<<<< HEAD
		if (ret < 0)
			node = node->rb_left;
		else if (ret > 0)
			node = node->rb_right;
		else {
=======
		parent = *new;
		if (ret < 0)
			new = &parent->rb_left;
		else if (ret > 0)
			new = &parent->rb_right;
		else {
			if (page_node) {
				VM_BUG_ON(page_node->head != &migrate_nodes);
				/*
				 * Test if the migrated page should be merged
				 * into a stable node dup. If the mapcount is
				 * 1 we can migrate it with another KSM page
				 * without adding it to the chain.
				 */
				if (page_mapcount(page) > 1)
					goto chain_append;
			}

			if (!stable_node_dup) {
				/*
				 * If the stable_node is a chain and
				 * we got a payload match in memcmp
				 * but we cannot merge the scanned
				 * page in any of the existing
				 * stable_node dups because they're
				 * all full, we need to wait the
				 * scanned page to find itself a match
				 * in the unstable tree to create a
				 * brand new KSM page to add later to
				 * the dups of this stable_node.
				 */
				return NULL;
			}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Lock and unlock the stable_node's page (which
			 * might already have been migrated) so that page
			 * migration is sure to notice its raised count.
			 * It would be more elegant to return stable_node
			 * than kpage, but that involves more changes.
			 */
<<<<<<< HEAD
			tree_page = get_ksm_page(stable_node, true);
			if (tree_page)
				unlock_page(tree_page);
=======
			tree_page = get_ksm_page(stable_node_dup,
						 GET_KSM_PAGE_TRYLOCK);

			if (PTR_ERR(tree_page) == -EBUSY)
				return ERR_PTR(-EBUSY);

			if (unlikely(!tree_page))
				/*
				 * The tree may have been rebalanced,
				 * so re-evaluate parent and new.
				 */
				goto again;
			unlock_page(tree_page);

			if (get_kpfn_nid(stable_node_dup->kpfn) !=
			    NUMA(stable_node_dup->nid)) {
				put_page(tree_page);
				goto replace;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return tree_page;
		}
	}

<<<<<<< HEAD
	return NULL;
}

/*
 * stable_tree_insert - insert rmap_item pointing to new ksm page
=======
	if (!page_node)
		return NULL;

	list_del(&page_node->list);
	DO_NUMA(page_node->nid = nid);
	rb_link_node(&page_node->node, parent, new);
	rb_insert_color(&page_node->node, root);
out:
	if (is_page_sharing_candidate(page_node)) {
		get_page(page);
		return page;
	} else
		return NULL;

replace:
	/*
	 * If stable_node was a chain and chain_prune collapsed it,
	 * stable_node has been updated to be the new regular
	 * stable_node. A collapse of the chain is indistinguishable
	 * from the case there was no chain in the stable
	 * rbtree. Otherwise stable_node is the chain and
	 * stable_node_dup is the dup to replace.
	 */
	if (stable_node_dup == stable_node) {
		VM_BUG_ON(is_stable_node_chain(stable_node_dup));
		VM_BUG_ON(is_stable_node_dup(stable_node_dup));
		/* there is no chain */
		if (page_node) {
			VM_BUG_ON(page_node->head != &migrate_nodes);
			list_del(&page_node->list);
			DO_NUMA(page_node->nid = nid);
			rb_replace_node(&stable_node_dup->node,
					&page_node->node,
					root);
			if (is_page_sharing_candidate(page_node))
				get_page(page);
			else
				page = NULL;
		} else {
			rb_erase(&stable_node_dup->node, root);
			page = NULL;
		}
	} else {
		VM_BUG_ON(!is_stable_node_chain(stable_node));
		__stable_node_dup_del(stable_node_dup);
		if (page_node) {
			VM_BUG_ON(page_node->head != &migrate_nodes);
			list_del(&page_node->list);
			DO_NUMA(page_node->nid = nid);
			stable_node_chain_add_dup(page_node, stable_node);
			if (is_page_sharing_candidate(page_node))
				get_page(page);
			else
				page = NULL;
		} else {
			page = NULL;
		}
	}
	stable_node_dup->head = &migrate_nodes;
	list_add(&stable_node_dup->list, stable_node_dup->head);
	return page;

chain_append:
	/* stable_node_dup could be null if it reached the limit */
	if (!stable_node_dup)
		stable_node_dup = stable_node_any;
	/*
	 * If stable_node was a chain and chain_prune collapsed it,
	 * stable_node has been updated to be the new regular
	 * stable_node. A collapse of the chain is indistinguishable
	 * from the case there was no chain in the stable
	 * rbtree. Otherwise stable_node is the chain and
	 * stable_node_dup is the dup to replace.
	 */
	if (stable_node_dup == stable_node) {
		VM_BUG_ON(is_stable_node_dup(stable_node_dup));
		/* chain is missing so create it */
		stable_node = alloc_stable_node_chain(stable_node_dup,
						      root);
		if (!stable_node)
			return NULL;
	}
	/*
	 * Add this stable_node dup that was
	 * migrated to the stable_node chain
	 * of the current nid for this page
	 * content.
	 */
	VM_BUG_ON(!is_stable_node_dup(stable_node_dup));
	VM_BUG_ON(page_node->head != &migrate_nodes);
	list_del(&page_node->list);
	DO_NUMA(page_node->nid = nid);
	stable_node_chain_add_dup(page_node, stable_node);
	goto out;
}

/*
 * stable_tree_insert - insert stable tree node pointing to new ksm page
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * into the stable tree.
 *
 * This function returns the stable tree node just allocated on success,
 * NULL otherwise.
 */
<<<<<<< HEAD
static struct stable_node *stable_tree_insert(struct page *kpage)
{
	struct rb_node **new = &root_stable_tree.rb_node;
	struct rb_node *parent = NULL;
	struct stable_node *stable_node;
=======
static struct ksm_stable_node *stable_tree_insert(struct page *kpage)
{
	int nid;
	unsigned long kpfn;
	struct rb_root *root;
	struct rb_node **new;
	struct rb_node *parent;
	struct ksm_stable_node *stable_node, *stable_node_dup, *stable_node_any;
	bool need_chain = false;

	kpfn = page_to_pfn(kpage);
	nid = get_kpfn_nid(kpfn);
	root = root_stable_tree + nid;
again:
	parent = NULL;
	new = &root->rb_node;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (*new) {
		struct page *tree_page;
		int ret;

		cond_resched();
<<<<<<< HEAD
		stable_node = rb_entry(*new, struct stable_node, node);
		tree_page = get_ksm_page(stable_node, false);
		if (!tree_page)
			return NULL;
=======
		stable_node = rb_entry(*new, struct ksm_stable_node, node);
		stable_node_any = NULL;
		tree_page = chain(&stable_node_dup, stable_node, root);
		if (!stable_node_dup) {
			/*
			 * Either all stable_node dups were full in
			 * this stable_node chain, or this chain was
			 * empty and should be rb_erased.
			 */
			stable_node_any = stable_node_dup_any(stable_node,
							      root);
			if (!stable_node_any) {
				/* rb_erase just run */
				goto again;
			}
			/*
			 * Take any of the stable_node dups page of
			 * this stable_node chain to let the tree walk
			 * continue. All KSM pages belonging to the
			 * stable_node dups in a stable_node chain
			 * have the same content and they're
			 * write protected at all times. Any will work
			 * fine to continue the walk.
			 */
			tree_page = get_ksm_page(stable_node_any,
						 GET_KSM_PAGE_NOLOCK);
		}
		VM_BUG_ON(!stable_node_dup ^ !!stable_node_any);
		if (!tree_page) {
			/*
			 * If we walked over a stale stable_node,
			 * get_ksm_page() will call rb_erase() and it
			 * may rebalance the tree from under us. So
			 * restart the search from scratch. Returning
			 * NULL would be safe too, but we'd generate
			 * false negative insertions just because some
			 * stable_node was stale.
			 */
			goto again;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ret = memcmp_pages(kpage, tree_page);
		put_page(tree_page);

		parent = *new;
		if (ret < 0)
			new = &parent->rb_left;
		else if (ret > 0)
			new = &parent->rb_right;
		else {
<<<<<<< HEAD
			/*
			 * It is not a bug that stable_tree_search() didn't
			 * find this node: because at that time our page was
			 * not yet write-protected, so may have changed since.
			 */
			return NULL;
		}
	}

	stable_node = alloc_stable_node();
	if (!stable_node)
		return NULL;

	rb_link_node(&stable_node->node, parent, new);
	rb_insert_color(&stable_node->node, &root_stable_tree);

	INIT_HLIST_HEAD(&stable_node->hlist);

	stable_node->kpfn = page_to_pfn(kpage);
	set_page_stable_node(kpage, stable_node);

	return stable_node;
=======
			need_chain = true;
			break;
		}
	}

	stable_node_dup = alloc_stable_node();
	if (!stable_node_dup)
		return NULL;

	INIT_HLIST_HEAD(&stable_node_dup->hlist);
	stable_node_dup->kpfn = kpfn;
	set_page_stable_node(kpage, stable_node_dup);
	stable_node_dup->rmap_hlist_len = 0;
	DO_NUMA(stable_node_dup->nid = nid);
	if (!need_chain) {
		rb_link_node(&stable_node_dup->node, parent, new);
		rb_insert_color(&stable_node_dup->node, root);
	} else {
		if (!is_stable_node_chain(stable_node)) {
			struct ksm_stable_node *orig = stable_node;
			/* chain is missing so create it */
			stable_node = alloc_stable_node_chain(orig, root);
			if (!stable_node) {
				free_stable_node(stable_node_dup);
				return NULL;
			}
		}
		stable_node_chain_add_dup(stable_node_dup, stable_node);
	}

	return stable_node_dup;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * unstable_tree_search_insert - search for identical page,
 * else insert rmap_item into the unstable tree.
 *
 * This function searches for a page in the unstable tree identical to the
 * page currently being scanned; and if no identical page is found in the
 * tree, we insert rmap_item as a new object into the unstable tree.
 *
 * This function returns pointer to rmap_item found to be identical
 * to the currently scanned page, NULL otherwise.
 *
 * This function does both searching and inserting, because they share
 * the same walking algorithm in an rbtree.
 */
static
<<<<<<< HEAD
struct rmap_item *unstable_tree_search_insert(struct rmap_item *rmap_item,
					      struct page *page,
					      struct page **tree_pagep)

{
	struct rb_node **new = &root_unstable_tree.rb_node;
	struct rb_node *parent = NULL;

	while (*new) {
		struct rmap_item *tree_rmap_item;
=======
struct ksm_rmap_item *unstable_tree_search_insert(struct ksm_rmap_item *rmap_item,
					      struct page *page,
					      struct page **tree_pagep)
{
	struct rb_node **new;
	struct rb_root *root;
	struct rb_node *parent = NULL;
	int nid;

	nid = get_kpfn_nid(page_to_pfn(page));
	root = root_unstable_tree + nid;
	new = &root->rb_node;

	while (*new) {
		struct ksm_rmap_item *tree_rmap_item;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct page *tree_page;
		int ret;

		cond_resched();
<<<<<<< HEAD
		tree_rmap_item = rb_entry(*new, struct rmap_item, node);
		tree_page = get_mergeable_page(tree_rmap_item);
		if (IS_ERR_OR_NULL(tree_page))
=======
		tree_rmap_item = rb_entry(*new, struct ksm_rmap_item, node);
		tree_page = get_mergeable_page(tree_rmap_item);
		if (!tree_page)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return NULL;

		/*
		 * Don't substitute a ksm page for a forked page.
		 */
		if (page == tree_page) {
			put_page(tree_page);
			return NULL;
		}

		ret = memcmp_pages(page, tree_page);

		parent = *new;
		if (ret < 0) {
			put_page(tree_page);
			new = &parent->rb_left;
		} else if (ret > 0) {
			put_page(tree_page);
			new = &parent->rb_right;
<<<<<<< HEAD
=======
		} else if (!ksm_merge_across_nodes &&
			   page_to_nid(tree_page) != nid) {
			/*
			 * If tree_page has been migrated to another NUMA node,
			 * it will be flushed out and put in the right unstable
			 * tree next time: only merge with it when across_nodes.
			 */
			put_page(tree_page);
			return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			*tree_pagep = tree_page;
			return tree_rmap_item;
		}
	}

	rmap_item->address |= UNSTABLE_FLAG;
	rmap_item->address |= (ksm_scan.seqnr & SEQNR_MASK);
<<<<<<< HEAD
	rb_link_node(&rmap_item->node, parent, new);
	rb_insert_color(&rmap_item->node, &root_unstable_tree);
=======
	DO_NUMA(rmap_item->nid = nid);
	rb_link_node(&rmap_item->node, parent, new);
	rb_insert_color(&rmap_item->node, root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ksm_pages_unshared++;
	return NULL;
}

/*
 * stable_tree_append - add another rmap_item to the linked list of
 * rmap_items hanging off a given node of the stable tree, all sharing
 * the same ksm page.
 */
<<<<<<< HEAD
static void stable_tree_append(struct rmap_item *rmap_item,
			       struct stable_node *stable_node)
{
=======
static void stable_tree_append(struct ksm_rmap_item *rmap_item,
			       struct ksm_stable_node *stable_node,
			       bool max_page_sharing_bypass)
{
	/*
	 * rmap won't find this mapping if we don't insert the
	 * rmap_item in the right stable_node
	 * duplicate. page_migration could break later if rmap breaks,
	 * so we can as well crash here. We really need to check for
	 * rmap_hlist_len == STABLE_NODE_CHAIN, but we can as well check
	 * for other negative values as an underflow if detected here
	 * for the first time (and not when decreasing rmap_hlist_len)
	 * would be sign of memory corruption in the stable_node.
	 */
	BUG_ON(stable_node->rmap_hlist_len < 0);

	stable_node->rmap_hlist_len++;
	if (!max_page_sharing_bypass)
		/* possibly non fatal but unexpected overflow, only warn */
		WARN_ON_ONCE(stable_node->rmap_hlist_len >
			     ksm_max_page_sharing);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rmap_item->head = stable_node;
	rmap_item->address |= STABLE_FLAG;
	hlist_add_head(&rmap_item->hlist, &stable_node->hlist);

	if (rmap_item->hlist.next)
		ksm_pages_sharing++;
	else
		ksm_pages_shared++;
<<<<<<< HEAD
=======

	rmap_item->mm->ksm_merging_pages++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * cmp_and_merge_page - first see if page can be merged into the stable tree;
 * if not, compare checksum to previous and if it's the same, see if page can
 * be inserted into the unstable tree, or merged with a page already there and
 * both transferred to the stable tree.
 *
 * @page: the page that we are searching identical page to.
 * @rmap_item: the reverse mapping into the virtual address of this page
 */
<<<<<<< HEAD
static void cmp_and_merge_page(struct page *page, struct rmap_item *rmap_item)
{
	struct rmap_item *tree_rmap_item;
	struct page *tree_page = NULL;
	struct stable_node *stable_node;
	struct page *kpage;
	unsigned int checksum;
	int err;

	remove_rmap_item_from_tree(rmap_item);

	/* We first start with searching the page inside the stable tree */
	kpage = stable_tree_search(page);
	if (kpage) {
=======
static void cmp_and_merge_page(struct page *page, struct ksm_rmap_item *rmap_item)
{
	struct mm_struct *mm = rmap_item->mm;
	struct ksm_rmap_item *tree_rmap_item;
	struct page *tree_page = NULL;
	struct ksm_stable_node *stable_node;
	struct page *kpage;
	unsigned int checksum;
	int err;
	bool max_page_sharing_bypass = false;

	stable_node = page_stable_node(page);
	if (stable_node) {
		if (stable_node->head != &migrate_nodes &&
		    get_kpfn_nid(READ_ONCE(stable_node->kpfn)) !=
		    NUMA(stable_node->nid)) {
			stable_node_dup_del(stable_node);
			stable_node->head = &migrate_nodes;
			list_add(&stable_node->list, stable_node->head);
		}
		if (stable_node->head != &migrate_nodes &&
		    rmap_item->head == stable_node)
			return;
		/*
		 * If it's a KSM fork, allow it to go over the sharing limit
		 * without warnings.
		 */
		if (!is_page_sharing_candidate(stable_node))
			max_page_sharing_bypass = true;
	}

	/* We first start with searching the page inside the stable tree */
	kpage = stable_tree_search(page);
	if (kpage == page && rmap_item->head == stable_node) {
		put_page(kpage);
		return;
	}

	remove_rmap_item_from_tree(rmap_item);

	if (kpage) {
		if (PTR_ERR(kpage) == -EBUSY)
			return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = try_to_merge_with_ksm_page(rmap_item, page, kpage);
		if (!err) {
			/*
			 * The page was successfully merged:
			 * add its rmap_item to the stable tree.
			 */
			lock_page(kpage);
<<<<<<< HEAD
			stable_tree_append(rmap_item, page_stable_node(kpage));
=======
			stable_tree_append(rmap_item, page_stable_node(kpage),
					   max_page_sharing_bypass);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			unlock_page(kpage);
		}
		put_page(kpage);
		return;
	}

	/*
	 * If the hash value of the page has changed from the last time
	 * we calculated it, this page is changing frequently: therefore we
	 * don't want to insert it in the unstable tree, and we don't want
	 * to waste our time searching for something identical to it there.
	 */
	checksum = calc_checksum(page);
	if (rmap_item->oldchecksum != checksum) {
		rmap_item->oldchecksum = checksum;
		return;
	}

<<<<<<< HEAD
	tree_rmap_item =
		unstable_tree_search_insert(rmap_item, page, &tree_page);
	if (tree_rmap_item) {
		kpage = try_to_merge_two_pages(rmap_item, page,
						tree_rmap_item, tree_page);
		put_page(tree_page);
		/*
		 * As soon as we merge this page, we want to remove the
		 * rmap_item of the page we have merged with from the unstable
		 * tree, and insert it instead as new node in the stable tree.
		 */
		if (kpage) {
			remove_rmap_item_from_tree(tree_rmap_item);

			lock_page(kpage);
			stable_node = stable_tree_insert(kpage);
			if (stable_node) {
				stable_tree_append(tree_rmap_item, stable_node);
				stable_tree_append(rmap_item, stable_node);
=======
	/*
	 * Same checksum as an empty page. We attempt to merge it with the
	 * appropriate zero page if the user enabled this via sysfs.
	 */
	if (ksm_use_zero_pages && (checksum == zero_checksum)) {
		struct vm_area_struct *vma;

		mmap_read_lock(mm);
		vma = find_mergeable_vma(mm, rmap_item->address);
		if (vma) {
			err = try_to_merge_one_page(vma, page,
					ZERO_PAGE(rmap_item->address));
			trace_ksm_merge_one_page(
				page_to_pfn(ZERO_PAGE(rmap_item->address)),
				rmap_item, mm, err);
		} else {
			/*
			 * If the vma is out of date, we do not need to
			 * continue.
			 */
			err = 0;
		}
		mmap_read_unlock(mm);
		/*
		 * In case of failure, the page was not really empty, so we
		 * need to continue. Otherwise we're done.
		 */
		if (!err)
			return;
	}
	tree_rmap_item =
		unstable_tree_search_insert(rmap_item, page, &tree_page);
	if (tree_rmap_item) {
		bool split;

		kpage = try_to_merge_two_pages(rmap_item, page,
						tree_rmap_item, tree_page);
		/*
		 * If both pages we tried to merge belong to the same compound
		 * page, then we actually ended up increasing the reference
		 * count of the same compound page twice, and split_huge_page
		 * failed.
		 * Here we set a flag if that happened, and we use it later to
		 * try split_huge_page again. Since we call put_page right
		 * afterwards, the reference count will be correct and
		 * split_huge_page should succeed.
		 */
		split = PageTransCompound(page)
			&& compound_head(page) == compound_head(tree_page);
		put_page(tree_page);
		if (kpage) {
			/*
			 * The pages were successfully merged: insert new
			 * node in the stable tree and add both rmap_items.
			 */
			lock_page(kpage);
			stable_node = stable_tree_insert(kpage);
			if (stable_node) {
				stable_tree_append(tree_rmap_item, stable_node,
						   false);
				stable_tree_append(rmap_item, stable_node,
						   false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			unlock_page(kpage);

			/*
			 * If we fail to insert the page into the stable tree,
			 * we will have 2 virtual addresses that are pointing
			 * to a ksm page left outside the stable tree,
			 * in which case we need to break_cow on both.
			 */
			if (!stable_node) {
				break_cow(tree_rmap_item);
				break_cow(rmap_item);
			}
<<<<<<< HEAD
=======
		} else if (split) {
			/*
			 * We are here if we tried to merge two pages and
			 * failed because they both belonged to the same
			 * compound page. We will split the page now, but no
			 * merging will take place.
			 * We do not want to add the cost of a full lock; if
			 * the page is locked, it is better to skip it and
			 * perhaps try again later.
			 */
			if (!trylock_page(page))
				return;
			split_huge_page(page);
			unlock_page(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

<<<<<<< HEAD
static struct rmap_item *get_next_rmap_item(struct mm_slot *mm_slot,
					    struct rmap_item **rmap_list,
					    unsigned long addr)
{
	struct rmap_item *rmap_item;
=======
static struct ksm_rmap_item *get_next_rmap_item(struct ksm_mm_slot *mm_slot,
					    struct ksm_rmap_item **rmap_list,
					    unsigned long addr)
{
	struct ksm_rmap_item *rmap_item;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (*rmap_list) {
		rmap_item = *rmap_list;
		if ((rmap_item->address & PAGE_MASK) == addr)
			return rmap_item;
		if (rmap_item->address > addr)
			break;
		*rmap_list = rmap_item->rmap_list;
		remove_rmap_item_from_tree(rmap_item);
		free_rmap_item(rmap_item);
	}

	rmap_item = alloc_rmap_item();
	if (rmap_item) {
		/* It has already been zeroed */
<<<<<<< HEAD
		rmap_item->mm = mm_slot->mm;
=======
		rmap_item->mm = mm_slot->slot.mm;
		rmap_item->mm->ksm_rmap_items++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rmap_item->address = addr;
		rmap_item->rmap_list = *rmap_list;
		*rmap_list = rmap_item;
	}
	return rmap_item;
}

<<<<<<< HEAD
static struct rmap_item *scan_get_next_rmap_item(struct page **page)
{
	struct mm_struct *mm;
	struct mm_slot *slot;
	struct vm_area_struct *vma;
	struct rmap_item *rmap_item;

	if (list_empty(&ksm_mm_head.mm_list))
		return NULL;

	slot = ksm_scan.mm_slot;
	if (slot == &ksm_mm_head) {
		/*
		 * A number of pages can hang around indefinitely on per-cpu
		 * pagevecs, raised page count preventing write_protect_page
=======
/*
 * Calculate skip age for the ksm page age. The age determines how often
 * de-duplicating has already been tried unsuccessfully. If the age is
 * smaller, the scanning of this page is skipped for less scans.
 *
 * @age: rmap_item age of page
 */
static unsigned int skip_age(rmap_age_t age)
{
	if (age <= 3)
		return 1;
	if (age <= 5)
		return 2;
	if (age <= 8)
		return 4;

	return 8;
}

/*
 * Determines if a page should be skipped for the current scan.
 *
 * @page: page to check
 * @rmap_item: associated rmap_item of page
 */
static bool should_skip_rmap_item(struct page *page,
				  struct ksm_rmap_item *rmap_item)
{
	rmap_age_t age;

	if (!ksm_smart_scan)
		return false;

	/*
	 * Never skip pages that are already KSM; pages cmp_and_merge_page()
	 * will essentially ignore them, but we still have to process them
	 * properly.
	 */
	if (PageKsm(page))
		return false;

	age = rmap_item->age;
	if (age != U8_MAX)
		rmap_item->age++;

	/*
	 * Smaller ages are not skipped, they need to get a chance to go
	 * through the different phases of the KSM merging.
	 */
	if (age < 3)
		return false;

	/*
	 * Are we still allowed to skip? If not, then don't skip it
	 * and determine how much more often we are allowed to skip next.
	 */
	if (!rmap_item->remaining_skips) {
		rmap_item->remaining_skips = skip_age(age);
		return false;
	}

	/* Skip this page */
	ksm_pages_skipped++;
	rmap_item->remaining_skips--;
	remove_rmap_item_from_tree(rmap_item);
	return true;
}

static struct ksm_rmap_item *scan_get_next_rmap_item(struct page **page)
{
	struct mm_struct *mm;
	struct ksm_mm_slot *mm_slot;
	struct mm_slot *slot;
	struct vm_area_struct *vma;
	struct ksm_rmap_item *rmap_item;
	struct vma_iterator vmi;
	int nid;

	if (list_empty(&ksm_mm_head.slot.mm_node))
		return NULL;

	mm_slot = ksm_scan.mm_slot;
	if (mm_slot == &ksm_mm_head) {
		advisor_start_scan();
		trace_ksm_start_scan(ksm_scan.seqnr, ksm_rmap_items);

		/*
		 * A number of pages can hang around indefinitely in per-cpu
		 * LRU cache, raised page count preventing write_protect_page
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * from merging them.  Though it doesn't really matter much,
		 * it is puzzling to see some stuck in pages_volatile until
		 * other activity jostles them out, and they also prevented
		 * LTP's KSM test from succeeding deterministically; so drain
		 * them here (here rather than on entry to ksm_do_scan(),
		 * so we don't IPI too often when pages_to_scan is set low).
		 */
		lru_add_drain_all();

<<<<<<< HEAD
		root_unstable_tree = RB_ROOT;

		spin_lock(&ksm_mmlist_lock);
		slot = list_entry(slot->mm_list.next, struct mm_slot, mm_list);
		ksm_scan.mm_slot = slot;
=======
		/*
		 * Whereas stale stable_nodes on the stable_tree itself
		 * get pruned in the regular course of stable_tree_search(),
		 * those moved out to the migrate_nodes list can accumulate:
		 * so prune them once before each full scan.
		 */
		if (!ksm_merge_across_nodes) {
			struct ksm_stable_node *stable_node, *next;
			struct page *page;

			list_for_each_entry_safe(stable_node, next,
						 &migrate_nodes, list) {
				page = get_ksm_page(stable_node,
						    GET_KSM_PAGE_NOLOCK);
				if (page)
					put_page(page);
				cond_resched();
			}
		}

		for (nid = 0; nid < ksm_nr_node_ids; nid++)
			root_unstable_tree[nid] = RB_ROOT;

		spin_lock(&ksm_mmlist_lock);
		slot = list_entry(mm_slot->slot.mm_node.next,
				  struct mm_slot, mm_node);
		mm_slot = mm_slot_entry(slot, struct ksm_mm_slot, slot);
		ksm_scan.mm_slot = mm_slot;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock(&ksm_mmlist_lock);
		/*
		 * Although we tested list_empty() above, a racing __ksm_exit
		 * of the last mm on the list may have removed it since then.
		 */
<<<<<<< HEAD
		if (slot == &ksm_mm_head)
			return NULL;
next_mm:
		ksm_scan.address = 0;
		ksm_scan.rmap_list = &slot->rmap_list;
	}

	mm = slot->mm;
	down_read(&mm->mmap_sem);
	if (ksm_test_exit(mm))
		vma = NULL;
	else
		vma = find_vma(mm, ksm_scan.address);

	for (; vma; vma = vma->vm_next) {
=======
		if (mm_slot == &ksm_mm_head)
			return NULL;
next_mm:
		ksm_scan.address = 0;
		ksm_scan.rmap_list = &mm_slot->rmap_list;
	}

	slot = &mm_slot->slot;
	mm = slot->mm;
	vma_iter_init(&vmi, mm, ksm_scan.address);

	mmap_read_lock(mm);
	if (ksm_test_exit(mm))
		goto no_vmas;

	for_each_vma(vmi, vma) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!(vma->vm_flags & VM_MERGEABLE))
			continue;
		if (ksm_scan.address < vma->vm_start)
			ksm_scan.address = vma->vm_start;
		if (!vma->anon_vma)
			ksm_scan.address = vma->vm_end;

		while (ksm_scan.address < vma->vm_end) {
			if (ksm_test_exit(mm))
				break;
			*page = follow_page(vma, ksm_scan.address, FOLL_GET);
			if (IS_ERR_OR_NULL(*page)) {
				ksm_scan.address += PAGE_SIZE;
				cond_resched();
				continue;
			}
<<<<<<< HEAD
			if (PageAnon(*page) ||
			    page_trans_compound_anon(*page)) {
				flush_anon_page(vma, *page, ksm_scan.address);
				flush_dcache_page(*page);
				rmap_item = get_next_rmap_item(slot,
=======
			if (is_zone_device_page(*page))
				goto next_page;
			if (PageAnon(*page)) {
				flush_anon_page(vma, *page, ksm_scan.address);
				flush_dcache_page(*page);
				rmap_item = get_next_rmap_item(mm_slot,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					ksm_scan.rmap_list, ksm_scan.address);
				if (rmap_item) {
					ksm_scan.rmap_list =
							&rmap_item->rmap_list;
<<<<<<< HEAD
					ksm_scan.address += PAGE_SIZE;
				} else
					put_page(*page);
				up_read(&mm->mmap_sem);
				return rmap_item;
			}
=======

					if (should_skip_rmap_item(*page, rmap_item))
						goto next_page;

					ksm_scan.address += PAGE_SIZE;
				} else
					put_page(*page);
				mmap_read_unlock(mm);
				return rmap_item;
			}
next_page:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			put_page(*page);
			ksm_scan.address += PAGE_SIZE;
			cond_resched();
		}
	}

	if (ksm_test_exit(mm)) {
<<<<<<< HEAD
		ksm_scan.address = 0;
		ksm_scan.rmap_list = &slot->rmap_list;
=======
no_vmas:
		ksm_scan.address = 0;
		ksm_scan.rmap_list = &mm_slot->rmap_list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/*
	 * Nuke all the rmap_items that are above this current rmap:
	 * because there were no VM_MERGEABLE vmas with such addresses.
	 */
<<<<<<< HEAD
	remove_trailing_rmap_items(slot, ksm_scan.rmap_list);

	spin_lock(&ksm_mmlist_lock);
	ksm_scan.mm_slot = list_entry(slot->mm_list.next,
						struct mm_slot, mm_list);
	if (ksm_scan.address == 0) {
		/*
		 * We've completed a full scan of all vmas, holding mmap_sem
=======
	remove_trailing_rmap_items(ksm_scan.rmap_list);

	spin_lock(&ksm_mmlist_lock);
	slot = list_entry(mm_slot->slot.mm_node.next,
			  struct mm_slot, mm_node);
	ksm_scan.mm_slot = mm_slot_entry(slot, struct ksm_mm_slot, slot);
	if (ksm_scan.address == 0) {
		/*
		 * We've completed a full scan of all vmas, holding mmap_lock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * throughout, and found no VM_MERGEABLE: so do the same as
		 * __ksm_exit does to remove this mm from all our lists now.
		 * This applies either when cleaning up after __ksm_exit
		 * (but beware: we can reach here even before __ksm_exit),
		 * or when all VM_MERGEABLE areas have been unmapped (and
<<<<<<< HEAD
		 * mmap_sem then protects against race with MADV_MERGEABLE).
		 */
		hlist_del(&slot->link);
		list_del(&slot->mm_list);
		spin_unlock(&ksm_mmlist_lock);

		free_mm_slot(slot);
		clear_bit(MMF_VM_MERGEABLE, &mm->flags);
		up_read(&mm->mmap_sem);
		mmdrop(mm);
	} else {
		spin_unlock(&ksm_mmlist_lock);
		up_read(&mm->mmap_sem);
	}

	/* Repeat until we've completed scanning the whole list */
	slot = ksm_scan.mm_slot;
	if (slot != &ksm_mm_head)
		goto next_mm;

=======
		 * mmap_lock then protects against race with MADV_MERGEABLE).
		 */
		hash_del(&mm_slot->slot.hash);
		list_del(&mm_slot->slot.mm_node);
		spin_unlock(&ksm_mmlist_lock);

		mm_slot_free(mm_slot_cache, mm_slot);
		clear_bit(MMF_VM_MERGEABLE, &mm->flags);
		clear_bit(MMF_VM_MERGE_ANY, &mm->flags);
		mmap_read_unlock(mm);
		mmdrop(mm);
	} else {
		mmap_read_unlock(mm);
		/*
		 * mmap_read_unlock(mm) first because after
		 * spin_unlock(&ksm_mmlist_lock) run, the "mm" may
		 * already have been freed under us by __ksm_exit()
		 * because the "mm_slot" is still hashed and
		 * ksm_scan.mm_slot doesn't point to it anymore.
		 */
		spin_unlock(&ksm_mmlist_lock);
	}

	/* Repeat until we've completed scanning the whole list */
	mm_slot = ksm_scan.mm_slot;
	if (mm_slot != &ksm_mm_head)
		goto next_mm;

	advisor_stop_scan();

	trace_ksm_stop_scan(ksm_scan.seqnr, ksm_rmap_items);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ksm_scan.seqnr++;
	return NULL;
}

/**
 * ksm_do_scan  - the ksm scanner main worker function.
<<<<<<< HEAD
 * @scan_npages - number of pages we want to scan before we return.
 */
static void ksm_do_scan(unsigned int scan_npages)
{
	struct rmap_item *rmap_item;
	struct page *uninitialized_var(page);

	while (scan_npages-- && likely(!freezing(current))) {
=======
 * @scan_npages:  number of pages we want to scan before we return.
 */
static void ksm_do_scan(unsigned int scan_npages)
{
	struct ksm_rmap_item *rmap_item;
	struct page *page;
	unsigned int npages = scan_npages;

	while (npages-- && likely(!freezing(current))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cond_resched();
		rmap_item = scan_get_next_rmap_item(&page);
		if (!rmap_item)
			return;
<<<<<<< HEAD
		if (!PageKsm(page) || !in_stable_tree(rmap_item))
			cmp_and_merge_page(page, rmap_item);
		put_page(page);
	}
}

static void process_timeout(unsigned long __data)
{
	wake_up_process((struct task_struct *)__data);
}

static signed long __sched deferred_schedule_timeout(signed long timeout)
{
	struct timer_list timer;
	unsigned long expire;

	__set_current_state(TASK_INTERRUPTIBLE);
	if (timeout < 0) {
		pr_err("schedule_timeout: wrong timeout value %lx\n",
							timeout);
		__set_current_state(TASK_RUNNING);
		goto out;
	}

	expire = timeout + jiffies;

	setup_deferrable_timer_on_stack(&timer, process_timeout,
			(unsigned long)current);
	mod_timer(&timer, expire);
	schedule();
	del_singleshot_timer_sync(&timer);

	/* Remove the timer from the object tracker */
	destroy_timer_on_stack(&timer);

	timeout = expire - jiffies;

out:
	return timeout < 0 ? 0 : timeout;
=======
		cmp_and_merge_page(page, rmap_item);
		put_page(page);
	}

	ksm_pages_scanned += scan_npages - npages;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ksmd_should_run(void)
{
<<<<<<< HEAD
	return (ksm_run & KSM_RUN_MERGE) && !list_empty(&ksm_mm_head.mm_list);
=======
	return (ksm_run & KSM_RUN_MERGE) && !list_empty(&ksm_mm_head.slot.mm_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ksm_scan_thread(void *nothing)
{
<<<<<<< HEAD
=======
	unsigned int sleep_ms;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_freezable();
	set_user_nice(current, 5);

	while (!kthread_should_stop()) {
		mutex_lock(&ksm_thread_mutex);
<<<<<<< HEAD
=======
		wait_while_offlining();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ksmd_should_run())
			ksm_do_scan(ksm_thread_pages_to_scan);
		mutex_unlock(&ksm_thread_mutex);

<<<<<<< HEAD
		try_to_freeze();

		if (ksmd_should_run()) {
			if (use_deferred_timer)
				deferred_schedule_timeout(
				msecs_to_jiffies(ksm_thread_sleep_millisecs));
			else
				schedule_timeout_interruptible(
				msecs_to_jiffies(ksm_thread_sleep_millisecs));
=======
		if (ksmd_should_run()) {
			sleep_ms = READ_ONCE(ksm_thread_sleep_millisecs);
			wait_event_freezable_timeout(ksm_iter_wait,
				sleep_ms != READ_ONCE(ksm_thread_sleep_millisecs),
				msecs_to_jiffies(sleep_ms));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			wait_event_freezable(ksm_thread_wait,
				ksmd_should_run() || kthread_should_stop());
		}
	}
	return 0;
}

<<<<<<< HEAD
=======
static void __ksm_add_vma(struct vm_area_struct *vma)
{
	unsigned long vm_flags = vma->vm_flags;

	if (vm_flags & VM_MERGEABLE)
		return;

	if (vma_ksm_compatible(vma))
		vm_flags_set(vma, VM_MERGEABLE);
}

static int __ksm_del_vma(struct vm_area_struct *vma)
{
	int err;

	if (!(vma->vm_flags & VM_MERGEABLE))
		return 0;

	if (vma->anon_vma) {
		err = unmerge_ksm_pages(vma, vma->vm_start, vma->vm_end, true);
		if (err)
			return err;
	}

	vm_flags_clear(vma, VM_MERGEABLE);
	return 0;
}
/**
 * ksm_add_vma - Mark vma as mergeable if compatible
 *
 * @vma:  Pointer to vma
 */
void ksm_add_vma(struct vm_area_struct *vma)
{
	struct mm_struct *mm = vma->vm_mm;

	if (test_bit(MMF_VM_MERGE_ANY, &mm->flags))
		__ksm_add_vma(vma);
}

static void ksm_add_vmas(struct mm_struct *mm)
{
	struct vm_area_struct *vma;

	VMA_ITERATOR(vmi, mm, 0);
	for_each_vma(vmi, vma)
		__ksm_add_vma(vma);
}

static int ksm_del_vmas(struct mm_struct *mm)
{
	struct vm_area_struct *vma;
	int err;

	VMA_ITERATOR(vmi, mm, 0);
	for_each_vma(vmi, vma) {
		err = __ksm_del_vma(vma);
		if (err)
			return err;
	}
	return 0;
}

/**
 * ksm_enable_merge_any - Add mm to mm ksm list and enable merging on all
 *                        compatible VMA's
 *
 * @mm:  Pointer to mm
 *
 * Returns 0 on success, otherwise error code
 */
int ksm_enable_merge_any(struct mm_struct *mm)
{
	int err;

	if (test_bit(MMF_VM_MERGE_ANY, &mm->flags))
		return 0;

	if (!test_bit(MMF_VM_MERGEABLE, &mm->flags)) {
		err = __ksm_enter(mm);
		if (err)
			return err;
	}

	set_bit(MMF_VM_MERGE_ANY, &mm->flags);
	ksm_add_vmas(mm);

	return 0;
}

/**
 * ksm_disable_merge_any - Disable merging on all compatible VMA's of the mm,
 *			   previously enabled via ksm_enable_merge_any().
 *
 * Disabling merging implies unmerging any merged pages, like setting
 * MADV_UNMERGEABLE would. If unmerging fails, the whole operation fails and
 * merging on all compatible VMA's remains enabled.
 *
 * @mm: Pointer to mm
 *
 * Returns 0 on success, otherwise error code
 */
int ksm_disable_merge_any(struct mm_struct *mm)
{
	int err;

	if (!test_bit(MMF_VM_MERGE_ANY, &mm->flags))
		return 0;

	err = ksm_del_vmas(mm);
	if (err) {
		ksm_add_vmas(mm);
		return err;
	}

	clear_bit(MMF_VM_MERGE_ANY, &mm->flags);
	return 0;
}

int ksm_disable(struct mm_struct *mm)
{
	mmap_assert_write_locked(mm);

	if (!test_bit(MMF_VM_MERGEABLE, &mm->flags))
		return 0;
	if (test_bit(MMF_VM_MERGE_ANY, &mm->flags))
		return ksm_disable_merge_any(mm);
	return ksm_del_vmas(mm);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int ksm_madvise(struct vm_area_struct *vma, unsigned long start,
		unsigned long end, int advice, unsigned long *vm_flags)
{
	struct mm_struct *mm = vma->vm_mm;
	int err;

	switch (advice) {
	case MADV_MERGEABLE:
<<<<<<< HEAD
		/*
		 * Be somewhat over-protective for now!
		 */
		if (*vm_flags & (VM_MERGEABLE | VM_SHARED  | VM_MAYSHARE   |
				 VM_PFNMAP    | VM_IO      | VM_DONTEXPAND |
				 VM_RESERVED  | VM_HUGETLB | VM_INSERTPAGE |
				 VM_NONLINEAR | VM_MIXEDMAP | VM_SAO))
			return 0;		/* just ignore the advice */
=======
		if (vma->vm_flags & VM_MERGEABLE)
			return 0;
		if (!vma_ksm_compatible(vma))
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!test_bit(MMF_VM_MERGEABLE, &mm->flags)) {
			err = __ksm_enter(mm);
			if (err)
				return err;
		}

		*vm_flags |= VM_MERGEABLE;
		break;

	case MADV_UNMERGEABLE:
		if (!(*vm_flags & VM_MERGEABLE))
			return 0;		/* just ignore the advice */

		if (vma->anon_vma) {
<<<<<<< HEAD
			err = unmerge_ksm_pages(vma, start, end);
=======
			err = unmerge_ksm_pages(vma, start, end, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err)
				return err;
		}

		*vm_flags &= ~VM_MERGEABLE;
		break;
	}

	return 0;
}
<<<<<<< HEAD

int __ksm_enter(struct mm_struct *mm)
{
	struct mm_slot *mm_slot;
	int needs_wakeup;

	mm_slot = alloc_mm_slot();
	if (!mm_slot)
		return -ENOMEM;

	/* Check ksm_run too?  Would need tighter locking */
	needs_wakeup = list_empty(&ksm_mm_head.mm_list);

	spin_lock(&ksm_mmlist_lock);
	insert_to_mm_slots_hash(mm, mm_slot);
	/*
	 * Insert just behind the scanning cursor, to let the area settle
	 * down a little; when fork is followed by immediate exec, we don't
	 * want ksmd to waste time setting up and tearing down an rmap_list.
	 */
	list_add_tail(&mm_slot->mm_list, &ksm_scan.mm_slot->mm_list);
	spin_unlock(&ksm_mmlist_lock);

	set_bit(MMF_VM_MERGEABLE, &mm->flags);
	atomic_inc(&mm->mm_count);
=======
EXPORT_SYMBOL_GPL(ksm_madvise);

int __ksm_enter(struct mm_struct *mm)
{
	struct ksm_mm_slot *mm_slot;
	struct mm_slot *slot;
	int needs_wakeup;

	mm_slot = mm_slot_alloc(mm_slot_cache);
	if (!mm_slot)
		return -ENOMEM;

	slot = &mm_slot->slot;

	/* Check ksm_run too?  Would need tighter locking */
	needs_wakeup = list_empty(&ksm_mm_head.slot.mm_node);

	spin_lock(&ksm_mmlist_lock);
	mm_slot_insert(mm_slots_hash, mm, slot);
	/*
	 * When KSM_RUN_MERGE (or KSM_RUN_STOP),
	 * insert just behind the scanning cursor, to let the area settle
	 * down a little; when fork is followed by immediate exec, we don't
	 * want ksmd to waste time setting up and tearing down an rmap_list.
	 *
	 * But when KSM_RUN_UNMERGE, it's important to insert ahead of its
	 * scanning cursor, otherwise KSM pages in newly forked mms will be
	 * missed: then we might as well insert at the end of the list.
	 */
	if (ksm_run & KSM_RUN_UNMERGE)
		list_add_tail(&slot->mm_node, &ksm_mm_head.slot.mm_node);
	else
		list_add_tail(&slot->mm_node, &ksm_scan.mm_slot->slot.mm_node);
	spin_unlock(&ksm_mmlist_lock);

	set_bit(MMF_VM_MERGEABLE, &mm->flags);
	mmgrab(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (needs_wakeup)
		wake_up_interruptible(&ksm_thread_wait);

<<<<<<< HEAD
=======
	trace_ksm_enter(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

void __ksm_exit(struct mm_struct *mm)
{
<<<<<<< HEAD
	struct mm_slot *mm_slot;
=======
	struct ksm_mm_slot *mm_slot;
	struct mm_slot *slot;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int easy_to_free = 0;

	/*
	 * This process is exiting: if it's straightforward (as is the
	 * case when ksmd was never running), free mm_slot immediately.
	 * But if it's at the cursor or has rmap_items linked to it, use
<<<<<<< HEAD
	 * mmap_sem to synchronize with any break_cows before pagetables
=======
	 * mmap_lock to synchronize with any break_cows before pagetables
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * are freed, and leave the mm_slot on the list for ksmd to free.
	 * Beware: ksm may already have noticed it exiting and freed the slot.
	 */

	spin_lock(&ksm_mmlist_lock);
<<<<<<< HEAD
	mm_slot = get_mm_slot(mm);
	if (mm_slot && ksm_scan.mm_slot != mm_slot) {
		if (!mm_slot->rmap_list) {
			hlist_del(&mm_slot->link);
			list_del(&mm_slot->mm_list);
			easy_to_free = 1;
		} else {
			list_move(&mm_slot->mm_list,
				  &ksm_scan.mm_slot->mm_list);
=======
	slot = mm_slot_lookup(mm_slots_hash, mm);
	mm_slot = mm_slot_entry(slot, struct ksm_mm_slot, slot);
	if (mm_slot && ksm_scan.mm_slot != mm_slot) {
		if (!mm_slot->rmap_list) {
			hash_del(&slot->hash);
			list_del(&slot->mm_node);
			easy_to_free = 1;
		} else {
			list_move(&slot->mm_node,
				  &ksm_scan.mm_slot->slot.mm_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	spin_unlock(&ksm_mmlist_lock);

	if (easy_to_free) {
<<<<<<< HEAD
		free_mm_slot(mm_slot);
		clear_bit(MMF_VM_MERGEABLE, &mm->flags);
		mmdrop(mm);
	} else if (mm_slot) {
		down_write(&mm->mmap_sem);
		up_write(&mm->mmap_sem);
	}
}

struct page *ksm_does_need_to_copy(struct page *page,
			struct vm_area_struct *vma, unsigned long address)
{
	struct page *new_page;

	new_page = alloc_page_vma(GFP_HIGHUSER_MOVABLE, vma, address);
	if (new_page) {
		copy_user_highpage(new_page, page, address, vma);

		SetPageDirty(new_page);
		__SetPageUptodate(new_page);
		SetPageSwapBacked(new_page);
		__set_page_locked(new_page);

		if (page_evictable(new_page, vma))
			lru_cache_add_lru(new_page, LRU_ACTIVE_ANON);
		else
			add_page_to_unevictable_list(new_page);
	}

	return new_page;
}

int page_referenced_ksm(struct page *page, struct mem_cgroup *memcg,
			unsigned long *vm_flags)
{
	struct stable_node *stable_node;
	struct rmap_item *rmap_item;
	struct hlist_node *hlist;
	unsigned int mapcount = page_mapcount(page);
	int referenced = 0;
	int search_new_forks = 0;

	VM_BUG_ON(!PageKsm(page));
	VM_BUG_ON(!PageLocked(page));

	stable_node = page_stable_node(page);
	if (!stable_node)
		return 0;
again:
	hlist_for_each_entry(rmap_item, hlist, &stable_node->hlist, hlist) {
		struct anon_vma *anon_vma = rmap_item->anon_vma;
		struct anon_vma_chain *vmac;
		struct vm_area_struct *vma;

		anon_vma_lock(anon_vma);
		list_for_each_entry(vmac, &anon_vma->head, same_anon_vma) {
			vma = vmac->vma;
			if (rmap_item->address < vma->vm_start ||
			    rmap_item->address >= vma->vm_end)
				continue;
			/*
			 * Initially we examine only the vma which covers this
			 * rmap_item; but later, if there is still work to do,
			 * we examine covering vmas in other mms: in case they
			 * were forked from the original since ksmd passed.
			 */
			if ((rmap_item->mm == vma->vm_mm) == search_new_forks)
				continue;

			if (memcg && !mm_match_cgroup(vma->vm_mm, memcg))
				continue;

			referenced += page_referenced_one(page, vma,
				rmap_item->address, &mapcount, vm_flags);
			if (!search_new_forks || !mapcount)
				break;
		}
		anon_vma_unlock(anon_vma);
		if (!mapcount)
			goto out;
	}
	if (!search_new_forks++)
		goto again;
out:
	return referenced;
}

int try_to_unmap_ksm(struct page *page, enum ttu_flags flags)
{
	struct stable_node *stable_node;
	struct hlist_node *hlist;
	struct rmap_item *rmap_item;
	int ret = SWAP_AGAIN;
	int search_new_forks = 0;

	VM_BUG_ON(!PageKsm(page));
	VM_BUG_ON(!PageLocked(page));

	stable_node = page_stable_node(page);
	if (!stable_node)
		return SWAP_FAIL;
again:
	hlist_for_each_entry(rmap_item, hlist, &stable_node->hlist, hlist) {
=======
		mm_slot_free(mm_slot_cache, mm_slot);
		clear_bit(MMF_VM_MERGE_ANY, &mm->flags);
		clear_bit(MMF_VM_MERGEABLE, &mm->flags);
		mmdrop(mm);
	} else if (mm_slot) {
		mmap_write_lock(mm);
		mmap_write_unlock(mm);
	}

	trace_ksm_exit(mm);
}

struct folio *ksm_might_need_to_copy(struct folio *folio,
			struct vm_area_struct *vma, unsigned long addr)
{
	struct page *page = folio_page(folio, 0);
	struct anon_vma *anon_vma = folio_anon_vma(folio);
	struct folio *new_folio;

	if (folio_test_large(folio))
		return folio;

	if (folio_test_ksm(folio)) {
		if (folio_stable_node(folio) &&
		    !(ksm_run & KSM_RUN_UNMERGE))
			return folio;	/* no need to copy it */
	} else if (!anon_vma) {
		return folio;		/* no need to copy it */
	} else if (folio->index == linear_page_index(vma, addr) &&
			anon_vma->root == vma->anon_vma->root) {
		return folio;		/* still no need to copy it */
	}
	if (PageHWPoison(page))
		return ERR_PTR(-EHWPOISON);
	if (!folio_test_uptodate(folio))
		return folio;		/* let do_swap_page report the error */

	new_folio = vma_alloc_folio(GFP_HIGHUSER_MOVABLE, 0, vma, addr, false);
	if (new_folio &&
	    mem_cgroup_charge(new_folio, vma->vm_mm, GFP_KERNEL)) {
		folio_put(new_folio);
		new_folio = NULL;
	}
	if (new_folio) {
		if (copy_mc_user_highpage(folio_page(new_folio, 0), page,
								addr, vma)) {
			folio_put(new_folio);
			memory_failure_queue(folio_pfn(folio), 0);
			return ERR_PTR(-EHWPOISON);
		}
		folio_set_dirty(new_folio);
		__folio_mark_uptodate(new_folio);
		__folio_set_locked(new_folio);
#ifdef CONFIG_SWAP
		count_vm_event(KSM_SWPIN_COPY);
#endif
	}

	return new_folio;
}

void rmap_walk_ksm(struct folio *folio, struct rmap_walk_control *rwc)
{
	struct ksm_stable_node *stable_node;
	struct ksm_rmap_item *rmap_item;
	int search_new_forks = 0;

	VM_BUG_ON_FOLIO(!folio_test_ksm(folio), folio);

	/*
	 * Rely on the page lock to protect against concurrent modifications
	 * to that page's node of the stable tree.
	 */
	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);

	stable_node = folio_stable_node(folio);
	if (!stable_node)
		return;
again:
	hlist_for_each_entry(rmap_item, &stable_node->hlist, hlist) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct anon_vma *anon_vma = rmap_item->anon_vma;
		struct anon_vma_chain *vmac;
		struct vm_area_struct *vma;

<<<<<<< HEAD
		anon_vma_lock(anon_vma);
		list_for_each_entry(vmac, &anon_vma->head, same_anon_vma) {
			vma = vmac->vma;
			if (rmap_item->address < vma->vm_start ||
			    rmap_item->address >= vma->vm_end)
=======
		cond_resched();
		if (!anon_vma_trylock_read(anon_vma)) {
			if (rwc->try_lock) {
				rwc->contended = true;
				return;
			}
			anon_vma_lock_read(anon_vma);
		}
		anon_vma_interval_tree_foreach(vmac, &anon_vma->rb_root,
					       0, ULONG_MAX) {
			unsigned long addr;

			cond_resched();
			vma = vmac->vma;

			/* Ignore the stable/unstable/sqnr flags */
			addr = rmap_item->address & PAGE_MASK;

			if (addr < vma->vm_start || addr >= vma->vm_end)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				continue;
			/*
			 * Initially we examine only the vma which covers this
			 * rmap_item; but later, if there is still work to do,
			 * we examine covering vmas in other mms: in case they
			 * were forked from the original since ksmd passed.
			 */
			if ((rmap_item->mm == vma->vm_mm) == search_new_forks)
				continue;

<<<<<<< HEAD
			ret = try_to_unmap_one(page, vma,
					rmap_item->address, flags);
			if (ret != SWAP_AGAIN || !page_mapped(page)) {
				anon_vma_unlock(anon_vma);
				goto out;
			}
		}
		anon_vma_unlock(anon_vma);
	}
	if (!search_new_forks++)
		goto again;
out:
	return ret;
}

#ifdef CONFIG_MIGRATION
int rmap_walk_ksm(struct page *page, int (*rmap_one)(struct page *,
		  struct vm_area_struct *, unsigned long, void *), void *arg)
{
	struct stable_node *stable_node;
	struct hlist_node *hlist;
	struct rmap_item *rmap_item;
	int ret = SWAP_AGAIN;
	int search_new_forks = 0;

	VM_BUG_ON(!PageKsm(page));
	VM_BUG_ON(!PageLocked(page));

	stable_node = page_stable_node(page);
	if (!stable_node)
		return ret;
again:
	hlist_for_each_entry(rmap_item, hlist, &stable_node->hlist, hlist) {
		struct anon_vma *anon_vma = rmap_item->anon_vma;
		struct anon_vma_chain *vmac;
		struct vm_area_struct *vma;

		anon_vma_lock(anon_vma);
		list_for_each_entry(vmac, &anon_vma->head, same_anon_vma) {
			vma = vmac->vma;
			if (rmap_item->address < vma->vm_start ||
			    rmap_item->address >= vma->vm_end)
				continue;
			/*
			 * Initially we examine only the vma which covers this
			 * rmap_item; but later, if there is still work to do,
			 * we examine covering vmas in other mms: in case they
			 * were forked from the original since ksmd passed.
			 */
			if ((rmap_item->mm == vma->vm_mm) == search_new_forks)
				continue;

			ret = rmap_one(page, vma, rmap_item->address, arg);
			if (ret != SWAP_AGAIN) {
				anon_vma_unlock(anon_vma);
				goto out;
			}
		}
		anon_vma_unlock(anon_vma);
	}
	if (!search_new_forks++)
		goto again;
out:
	return ret;
}

void ksm_migrate_page(struct page *newpage, struct page *oldpage)
{
	struct stable_node *stable_node;

	VM_BUG_ON(!PageLocked(oldpage));
	VM_BUG_ON(!PageLocked(newpage));
	VM_BUG_ON(newpage->mapping != oldpage->mapping);

	stable_node = page_stable_node(newpage);
	if (stable_node) {
		VM_BUG_ON(stable_node->kpfn != page_to_pfn(oldpage));
		stable_node->kpfn = page_to_pfn(newpage);
		/*
		 * newpage->mapping was set in advance; now we need smp_wmb()
		 * to make sure that the new stable_node->kpfn is visible
		 * to get_ksm_page() before it can see that oldpage->mapping
		 * has gone stale (or that PageSwapCache has been cleared).
		 */
		smp_wmb();
		set_page_stable_node(oldpage, NULL);
=======
			if (rwc->invalid_vma && rwc->invalid_vma(vma, rwc->arg))
				continue;

			if (!rwc->rmap_one(folio, vma, addr, rwc->arg)) {
				anon_vma_unlock_read(anon_vma);
				return;
			}
			if (rwc->done && rwc->done(folio)) {
				anon_vma_unlock_read(anon_vma);
				return;
			}
		}
		anon_vma_unlock_read(anon_vma);
	}
	if (!search_new_forks++)
		goto again;
}

#ifdef CONFIG_MEMORY_FAILURE
/*
 * Collect processes when the error hit an ksm page.
 */
void collect_procs_ksm(struct page *page, struct list_head *to_kill,
		       int force_early)
{
	struct ksm_stable_node *stable_node;
	struct ksm_rmap_item *rmap_item;
	struct folio *folio = page_folio(page);
	struct vm_area_struct *vma;
	struct task_struct *tsk;

	stable_node = folio_stable_node(folio);
	if (!stable_node)
		return;
	hlist_for_each_entry(rmap_item, &stable_node->hlist, hlist) {
		struct anon_vma *av = rmap_item->anon_vma;

		anon_vma_lock_read(av);
		rcu_read_lock();
		for_each_process(tsk) {
			struct anon_vma_chain *vmac;
			unsigned long addr;
			struct task_struct *t =
				task_early_kill(tsk, force_early);
			if (!t)
				continue;
			anon_vma_interval_tree_foreach(vmac, &av->rb_root, 0,
						       ULONG_MAX)
			{
				vma = vmac->vma;
				if (vma->vm_mm == t->mm) {
					addr = rmap_item->address & PAGE_MASK;
					add_to_kill_ksm(t, page, vma, to_kill,
							addr);
				}
			}
		}
		rcu_read_unlock();
		anon_vma_unlock_read(av);
	}
}
#endif

#ifdef CONFIG_MIGRATION
void folio_migrate_ksm(struct folio *newfolio, struct folio *folio)
{
	struct ksm_stable_node *stable_node;

	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);
	VM_BUG_ON_FOLIO(!folio_test_locked(newfolio), newfolio);
	VM_BUG_ON_FOLIO(newfolio->mapping != folio->mapping, newfolio);

	stable_node = folio_stable_node(folio);
	if (stable_node) {
		VM_BUG_ON_FOLIO(stable_node->kpfn != folio_pfn(folio), folio);
		stable_node->kpfn = folio_pfn(newfolio);
		/*
		 * newfolio->mapping was set in advance; now we need smp_wmb()
		 * to make sure that the new stable_node->kpfn is visible
		 * to get_ksm_page() before it can see that folio->mapping
		 * has gone stale (or that folio_test_swapcache has been cleared).
		 */
		smp_wmb();
		set_page_stable_node(&folio->page, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
#endif /* CONFIG_MIGRATION */

#ifdef CONFIG_MEMORY_HOTREMOVE
<<<<<<< HEAD
static struct stable_node *ksm_check_stable_tree(unsigned long start_pfn,
						 unsigned long end_pfn)
{
	struct rb_node *node;

	for (node = rb_first(&root_stable_tree); node; node = rb_next(node)) {
		struct stable_node *stable_node;

		stable_node = rb_entry(node, struct stable_node, node);
		if (stable_node->kpfn >= start_pfn &&
		    stable_node->kpfn < end_pfn)
			return stable_node;
	}
	return NULL;
=======
static void wait_while_offlining(void)
{
	while (ksm_run & KSM_RUN_OFFLINE) {
		mutex_unlock(&ksm_thread_mutex);
		wait_on_bit(&ksm_run, ilog2(KSM_RUN_OFFLINE),
			    TASK_UNINTERRUPTIBLE);
		mutex_lock(&ksm_thread_mutex);
	}
}

static bool stable_node_dup_remove_range(struct ksm_stable_node *stable_node,
					 unsigned long start_pfn,
					 unsigned long end_pfn)
{
	if (stable_node->kpfn >= start_pfn &&
	    stable_node->kpfn < end_pfn) {
		/*
		 * Don't get_ksm_page, page has already gone:
		 * which is why we keep kpfn instead of page*
		 */
		remove_node_from_stable_tree(stable_node);
		return true;
	}
	return false;
}

static bool stable_node_chain_remove_range(struct ksm_stable_node *stable_node,
					   unsigned long start_pfn,
					   unsigned long end_pfn,
					   struct rb_root *root)
{
	struct ksm_stable_node *dup;
	struct hlist_node *hlist_safe;

	if (!is_stable_node_chain(stable_node)) {
		VM_BUG_ON(is_stable_node_dup(stable_node));
		return stable_node_dup_remove_range(stable_node, start_pfn,
						    end_pfn);
	}

	hlist_for_each_entry_safe(dup, hlist_safe,
				  &stable_node->hlist, hlist_dup) {
		VM_BUG_ON(!is_stable_node_dup(dup));
		stable_node_dup_remove_range(dup, start_pfn, end_pfn);
	}
	if (hlist_empty(&stable_node->hlist)) {
		free_stable_node_chain(stable_node, root);
		return true; /* notify caller that tree was rebalanced */
	} else
		return false;
}

static void ksm_check_stable_tree(unsigned long start_pfn,
				  unsigned long end_pfn)
{
	struct ksm_stable_node *stable_node, *next;
	struct rb_node *node;
	int nid;

	for (nid = 0; nid < ksm_nr_node_ids; nid++) {
		node = rb_first(root_stable_tree + nid);
		while (node) {
			stable_node = rb_entry(node, struct ksm_stable_node, node);
			if (stable_node_chain_remove_range(stable_node,
							   start_pfn, end_pfn,
							   root_stable_tree +
							   nid))
				node = rb_first(root_stable_tree + nid);
			else
				node = rb_next(node);
			cond_resched();
		}
	}
	list_for_each_entry_safe(stable_node, next, &migrate_nodes, list) {
		if (stable_node->kpfn >= start_pfn &&
		    stable_node->kpfn < end_pfn)
			remove_node_from_stable_tree(stable_node);
		cond_resched();
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ksm_memory_callback(struct notifier_block *self,
			       unsigned long action, void *arg)
{
	struct memory_notify *mn = arg;
<<<<<<< HEAD
	struct stable_node *stable_node;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (action) {
	case MEM_GOING_OFFLINE:
		/*
<<<<<<< HEAD
		 * Keep it very simple for now: just lock out ksmd and
		 * MADV_UNMERGEABLE while any memory is going offline.
		 * mutex_lock_nested() is necessary because lockdep was alarmed
		 * that here we take ksm_thread_mutex inside notifier chain
		 * mutex, and later take notifier chain mutex inside
		 * ksm_thread_mutex to unlock it.   But that's safe because both
		 * are inside mem_hotplug_mutex.
		 */
		mutex_lock_nested(&ksm_thread_mutex, SINGLE_DEPTH_NESTING);
=======
		 * Prevent ksm_do_scan(), unmerge_and_remove_all_rmap_items()
		 * and remove_all_stable_nodes() while memory is going offline:
		 * it is unsafe for them to touch the stable tree at this time.
		 * But unmerge_ksm_pages(), rmap lookups and other entry points
		 * which do not need the ksm_thread_mutex are all safe.
		 */
		mutex_lock(&ksm_thread_mutex);
		ksm_run |= KSM_RUN_OFFLINE;
		mutex_unlock(&ksm_thread_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case MEM_OFFLINE:
		/*
		 * Most of the work is done by page migration; but there might
		 * be a few stable_nodes left over, still pointing to struct
<<<<<<< HEAD
		 * pages which have been offlined: prune those from the tree.
		 */
		while ((stable_node = ksm_check_stable_tree(mn->start_pfn,
					mn->start_pfn + mn->nr_pages)) != NULL)
			remove_node_from_stable_tree(stable_node);
		/* fallthrough */

	case MEM_CANCEL_OFFLINE:
		mutex_unlock(&ksm_thread_mutex);
=======
		 * pages which have been offlined: prune those from the tree,
		 * otherwise get_ksm_page() might later try to access a
		 * non-existent struct page.
		 */
		ksm_check_stable_tree(mn->start_pfn,
				      mn->start_pfn + mn->nr_pages);
		fallthrough;
	case MEM_CANCEL_OFFLINE:
		mutex_lock(&ksm_thread_mutex);
		ksm_run &= ~KSM_RUN_OFFLINE;
		mutex_unlock(&ksm_thread_mutex);

		smp_mb();	/* wake_up_bit advises this */
		wake_up_bit(&ksm_run, ilog2(KSM_RUN_OFFLINE));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	return NOTIFY_OK;
}
<<<<<<< HEAD
#endif /* CONFIG_MEMORY_HOTREMOVE */

=======
#else
static void wait_while_offlining(void)
{
}
#endif /* CONFIG_MEMORY_HOTREMOVE */

#ifdef CONFIG_PROC_FS
long ksm_process_profit(struct mm_struct *mm)
{
	return (long)(mm->ksm_merging_pages + mm->ksm_zero_pages) * PAGE_SIZE -
		mm->ksm_rmap_items * sizeof(struct ksm_rmap_item);
}
#endif /* CONFIG_PROC_FS */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SYSFS
/*
 * This all compiles without CONFIG_SYSFS, but is a waste of space.
 */

#define KSM_ATTR_RO(_name) \
	static struct kobj_attribute _name##_attr = __ATTR_RO(_name)
#define KSM_ATTR(_name) \
<<<<<<< HEAD
	static struct kobj_attribute _name##_attr = \
		__ATTR(_name, 0644, _name##_show, _name##_store)
=======
	static struct kobj_attribute _name##_attr = __ATTR_RW(_name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t sleep_millisecs_show(struct kobject *kobj,
				    struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", ksm_thread_sleep_millisecs);
=======
	return sysfs_emit(buf, "%u\n", ksm_thread_sleep_millisecs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t sleep_millisecs_store(struct kobject *kobj,
				     struct kobj_attribute *attr,
				     const char *buf, size_t count)
{
<<<<<<< HEAD
	unsigned long msecs;
	int err;

	err = strict_strtoul(buf, 10, &msecs);
	if (err || msecs > UINT_MAX)
		return -EINVAL;

	ksm_thread_sleep_millisecs = msecs;
=======
	unsigned int msecs;
	int err;

	err = kstrtouint(buf, 10, &msecs);
	if (err)
		return -EINVAL;

	ksm_thread_sleep_millisecs = msecs;
	wake_up_interruptible(&ksm_iter_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return count;
}
KSM_ATTR(sleep_millisecs);

static ssize_t pages_to_scan_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", ksm_thread_pages_to_scan);
=======
	return sysfs_emit(buf, "%u\n", ksm_thread_pages_to_scan);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t pages_to_scan_store(struct kobject *kobj,
				   struct kobj_attribute *attr,
				   const char *buf, size_t count)
{
<<<<<<< HEAD
	int err;
	unsigned long nr_pages;

	err = strict_strtoul(buf, 10, &nr_pages);
	if (err || nr_pages > UINT_MAX)
=======
	unsigned int nr_pages;
	int err;

	if (ksm_advisor != KSM_ADVISOR_NONE)
		return -EINVAL;

	err = kstrtouint(buf, 10, &nr_pages);
	if (err)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	ksm_thread_pages_to_scan = nr_pages;

	return count;
}
KSM_ATTR(pages_to_scan);

static ssize_t run_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", ksm_run);
=======
	return sysfs_emit(buf, "%lu\n", ksm_run);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t run_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
<<<<<<< HEAD
	int err;
	unsigned long flags;

	err = strict_strtoul(buf, 10, &flags);
	if (err || flags > UINT_MAX)
=======
	unsigned int flags;
	int err;

	err = kstrtouint(buf, 10, &flags);
	if (err)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	if (flags > KSM_RUN_UNMERGE)
		return -EINVAL;

	/*
	 * KSM_RUN_MERGE sets ksmd running, and 0 stops it running.
	 * KSM_RUN_UNMERGE stops it running and unmerges all rmap_items,
	 * breaking COW to free the pages_shared (but leaves mm_slots
	 * on the list for when ksmd may be set running again).
	 */

	mutex_lock(&ksm_thread_mutex);
<<<<<<< HEAD
	if (ksm_run != flags) {
		ksm_run = flags;
		if (flags & KSM_RUN_UNMERGE) {
			int oom_score_adj;

			oom_score_adj = test_set_oom_score_adj(OOM_SCORE_ADJ_MAX);
			err = unmerge_and_remove_all_rmap_items();
			compare_swap_oom_score_adj(OOM_SCORE_ADJ_MAX,
								oom_score_adj);
=======
	wait_while_offlining();
	if (ksm_run != flags) {
		ksm_run = flags;
		if (flags & KSM_RUN_UNMERGE) {
			set_current_oom_origin();
			err = unmerge_and_remove_all_rmap_items();
			clear_current_oom_origin();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err) {
				ksm_run = KSM_RUN_STOP;
				count = err;
			}
		}
	}
	mutex_unlock(&ksm_thread_mutex);

	if (flags & KSM_RUN_MERGE)
		wake_up_interruptible(&ksm_thread_wait);

	return count;
}
KSM_ATTR(run);

<<<<<<< HEAD
static ssize_t deferred_timer_show(struct kobject *kobj,
				    struct kobj_attribute *attr, char *buf)
{
	return snprintf(buf, 8, "%d\n", use_deferred_timer);
}

static ssize_t deferred_timer_store(struct kobject *kobj,
				     struct kobj_attribute *attr,
				     const char *buf, size_t count)
{
	unsigned long enable;
	int err;

	err = kstrtoul(buf, 10, &enable);
	use_deferred_timer = enable;

	return count;
}
KSM_ATTR(deferred_timer);
=======
#ifdef CONFIG_NUMA
static ssize_t merge_across_nodes_show(struct kobject *kobj,
				       struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%u\n", ksm_merge_across_nodes);
}

static ssize_t merge_across_nodes_store(struct kobject *kobj,
				   struct kobj_attribute *attr,
				   const char *buf, size_t count)
{
	int err;
	unsigned long knob;

	err = kstrtoul(buf, 10, &knob);
	if (err)
		return err;
	if (knob > 1)
		return -EINVAL;

	mutex_lock(&ksm_thread_mutex);
	wait_while_offlining();
	if (ksm_merge_across_nodes != knob) {
		if (ksm_pages_shared || remove_all_stable_nodes())
			err = -EBUSY;
		else if (root_stable_tree == one_stable_tree) {
			struct rb_root *buf;
			/*
			 * This is the first time that we switch away from the
			 * default of merging across nodes: must now allocate
			 * a buffer to hold as many roots as may be needed.
			 * Allocate stable and unstable together:
			 * MAXSMP NODES_SHIFT 10 will use 16kB.
			 */
			buf = kcalloc(nr_node_ids + nr_node_ids, sizeof(*buf),
				      GFP_KERNEL);
			/* Let us assume that RB_ROOT is NULL is zero */
			if (!buf)
				err = -ENOMEM;
			else {
				root_stable_tree = buf;
				root_unstable_tree = buf + nr_node_ids;
				/* Stable tree is empty but not the unstable */
				root_unstable_tree[0] = one_unstable_tree[0];
			}
		}
		if (!err) {
			ksm_merge_across_nodes = knob;
			ksm_nr_node_ids = knob ? 1 : nr_node_ids;
		}
	}
	mutex_unlock(&ksm_thread_mutex);

	return err ? err : count;
}
KSM_ATTR(merge_across_nodes);
#endif

static ssize_t use_zero_pages_show(struct kobject *kobj,
				   struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%u\n", ksm_use_zero_pages);
}
static ssize_t use_zero_pages_store(struct kobject *kobj,
				   struct kobj_attribute *attr,
				   const char *buf, size_t count)
{
	int err;
	bool value;

	err = kstrtobool(buf, &value);
	if (err)
		return -EINVAL;

	ksm_use_zero_pages = value;

	return count;
}
KSM_ATTR(use_zero_pages);

static ssize_t max_page_sharing_show(struct kobject *kobj,
				     struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%u\n", ksm_max_page_sharing);
}

static ssize_t max_page_sharing_store(struct kobject *kobj,
				      struct kobj_attribute *attr,
				      const char *buf, size_t count)
{
	int err;
	int knob;

	err = kstrtoint(buf, 10, &knob);
	if (err)
		return err;
	/*
	 * When a KSM page is created it is shared by 2 mappings. This
	 * being a signed comparison, it implicitly verifies it's not
	 * negative.
	 */
	if (knob < 2)
		return -EINVAL;

	if (READ_ONCE(ksm_max_page_sharing) == knob)
		return count;

	mutex_lock(&ksm_thread_mutex);
	wait_while_offlining();
	if (ksm_max_page_sharing != knob) {
		if (ksm_pages_shared || remove_all_stable_nodes())
			err = -EBUSY;
		else
			ksm_max_page_sharing = knob;
	}
	mutex_unlock(&ksm_thread_mutex);

	return err ? err : count;
}
KSM_ATTR(max_page_sharing);

static ssize_t pages_scanned_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lu\n", ksm_pages_scanned);
}
KSM_ATTR_RO(pages_scanned);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t pages_shared_show(struct kobject *kobj,
				 struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%lu\n", ksm_pages_shared);
=======
	return sysfs_emit(buf, "%lu\n", ksm_pages_shared);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
KSM_ATTR_RO(pages_shared);

static ssize_t pages_sharing_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%lu\n", ksm_pages_sharing);
=======
	return sysfs_emit(buf, "%lu\n", ksm_pages_sharing);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
KSM_ATTR_RO(pages_sharing);

static ssize_t pages_unshared_show(struct kobject *kobj,
				   struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%lu\n", ksm_pages_unshared);
=======
	return sysfs_emit(buf, "%lu\n", ksm_pages_unshared);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
KSM_ATTR_RO(pages_unshared);

static ssize_t pages_volatile_show(struct kobject *kobj,
				   struct kobj_attribute *attr, char *buf)
{
	long ksm_pages_volatile;

	ksm_pages_volatile = ksm_rmap_items - ksm_pages_shared
				- ksm_pages_sharing - ksm_pages_unshared;
	/*
	 * It was not worth any locking to calculate that statistic,
	 * but it might therefore sometimes be negative: conceal that.
	 */
	if (ksm_pages_volatile < 0)
		ksm_pages_volatile = 0;
<<<<<<< HEAD
	return sprintf(buf, "%ld\n", ksm_pages_volatile);
}
KSM_ATTR_RO(pages_volatile);

static ssize_t full_scans_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%lu\n", ksm_scan.seqnr);
}
KSM_ATTR_RO(full_scans);

=======
	return sysfs_emit(buf, "%ld\n", ksm_pages_volatile);
}
KSM_ATTR_RO(pages_volatile);

static ssize_t pages_skipped_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lu\n", ksm_pages_skipped);
}
KSM_ATTR_RO(pages_skipped);

static ssize_t ksm_zero_pages_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%ld\n", ksm_zero_pages);
}
KSM_ATTR_RO(ksm_zero_pages);

static ssize_t general_profit_show(struct kobject *kobj,
				   struct kobj_attribute *attr, char *buf)
{
	long general_profit;

	general_profit = (ksm_pages_sharing + ksm_zero_pages) * PAGE_SIZE -
				ksm_rmap_items * sizeof(struct ksm_rmap_item);

	return sysfs_emit(buf, "%ld\n", general_profit);
}
KSM_ATTR_RO(general_profit);

static ssize_t stable_node_dups_show(struct kobject *kobj,
				     struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lu\n", ksm_stable_node_dups);
}
KSM_ATTR_RO(stable_node_dups);

static ssize_t stable_node_chains_show(struct kobject *kobj,
				       struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lu\n", ksm_stable_node_chains);
}
KSM_ATTR_RO(stable_node_chains);

static ssize_t
stable_node_chains_prune_millisecs_show(struct kobject *kobj,
					struct kobj_attribute *attr,
					char *buf)
{
	return sysfs_emit(buf, "%u\n", ksm_stable_node_chains_prune_millisecs);
}

static ssize_t
stable_node_chains_prune_millisecs_store(struct kobject *kobj,
					 struct kobj_attribute *attr,
					 const char *buf, size_t count)
{
	unsigned int msecs;
	int err;

	err = kstrtouint(buf, 10, &msecs);
	if (err)
		return -EINVAL;

	ksm_stable_node_chains_prune_millisecs = msecs;

	return count;
}
KSM_ATTR(stable_node_chains_prune_millisecs);

static ssize_t full_scans_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lu\n", ksm_scan.seqnr);
}
KSM_ATTR_RO(full_scans);

static ssize_t smart_scan_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%u\n", ksm_smart_scan);
}

static ssize_t smart_scan_store(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int err;
	bool value;

	err = kstrtobool(buf, &value);
	if (err)
		return -EINVAL;

	ksm_smart_scan = value;
	return count;
}
KSM_ATTR(smart_scan);

static ssize_t advisor_mode_show(struct kobject *kobj,
				 struct kobj_attribute *attr, char *buf)
{
	const char *output;

	if (ksm_advisor == KSM_ADVISOR_NONE)
		output = "[none] scan-time";
	else if (ksm_advisor == KSM_ADVISOR_SCAN_TIME)
		output = "none [scan-time]";

	return sysfs_emit(buf, "%s\n", output);
}

static ssize_t advisor_mode_store(struct kobject *kobj,
				  struct kobj_attribute *attr, const char *buf,
				  size_t count)
{
	enum ksm_advisor_type curr_advisor = ksm_advisor;

	if (sysfs_streq("scan-time", buf))
		ksm_advisor = KSM_ADVISOR_SCAN_TIME;
	else if (sysfs_streq("none", buf))
		ksm_advisor = KSM_ADVISOR_NONE;
	else
		return -EINVAL;

	/* Set advisor default values */
	if (curr_advisor != ksm_advisor)
		set_advisor_defaults();

	return count;
}
KSM_ATTR(advisor_mode);

static ssize_t advisor_max_cpu_show(struct kobject *kobj,
				    struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%u\n", ksm_advisor_max_cpu);
}

static ssize_t advisor_max_cpu_store(struct kobject *kobj,
				     struct kobj_attribute *attr,
				     const char *buf, size_t count)
{
	int err;
	unsigned long value;

	err = kstrtoul(buf, 10, &value);
	if (err)
		return -EINVAL;

	ksm_advisor_max_cpu = value;
	return count;
}
KSM_ATTR(advisor_max_cpu);

static ssize_t advisor_min_pages_to_scan_show(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lu\n", ksm_advisor_min_pages_to_scan);
}

static ssize_t advisor_min_pages_to_scan_store(struct kobject *kobj,
					struct kobj_attribute *attr,
					const char *buf, size_t count)
{
	int err;
	unsigned long value;

	err = kstrtoul(buf, 10, &value);
	if (err)
		return -EINVAL;

	ksm_advisor_min_pages_to_scan = value;
	return count;
}
KSM_ATTR(advisor_min_pages_to_scan);

static ssize_t advisor_max_pages_to_scan_show(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lu\n", ksm_advisor_max_pages_to_scan);
}

static ssize_t advisor_max_pages_to_scan_store(struct kobject *kobj,
					struct kobj_attribute *attr,
					const char *buf, size_t count)
{
	int err;
	unsigned long value;

	err = kstrtoul(buf, 10, &value);
	if (err)
		return -EINVAL;

	ksm_advisor_max_pages_to_scan = value;
	return count;
}
KSM_ATTR(advisor_max_pages_to_scan);

static ssize_t advisor_target_scan_time_show(struct kobject *kobj,
					     struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lu\n", ksm_advisor_target_scan_time);
}

static ssize_t advisor_target_scan_time_store(struct kobject *kobj,
					      struct kobj_attribute *attr,
					      const char *buf, size_t count)
{
	int err;
	unsigned long value;

	err = kstrtoul(buf, 10, &value);
	if (err)
		return -EINVAL;
	if (value < 1)
		return -EINVAL;

	ksm_advisor_target_scan_time = value;
	return count;
}
KSM_ATTR(advisor_target_scan_time);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct attribute *ksm_attrs[] = {
	&sleep_millisecs_attr.attr,
	&pages_to_scan_attr.attr,
	&run_attr.attr,
<<<<<<< HEAD
=======
	&pages_scanned_attr.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&pages_shared_attr.attr,
	&pages_sharing_attr.attr,
	&pages_unshared_attr.attr,
	&pages_volatile_attr.attr,
<<<<<<< HEAD
	&full_scans_attr.attr,
	&deferred_timer_attr.attr,
	NULL,
};

static struct attribute_group ksm_attr_group = {
=======
	&pages_skipped_attr.attr,
	&ksm_zero_pages_attr.attr,
	&full_scans_attr.attr,
#ifdef CONFIG_NUMA
	&merge_across_nodes_attr.attr,
#endif
	&max_page_sharing_attr.attr,
	&stable_node_chains_attr.attr,
	&stable_node_dups_attr.attr,
	&stable_node_chains_prune_millisecs_attr.attr,
	&use_zero_pages_attr.attr,
	&general_profit_attr.attr,
	&smart_scan_attr.attr,
	&advisor_mode_attr.attr,
	&advisor_max_cpu_attr.attr,
	&advisor_min_pages_to_scan_attr.attr,
	&advisor_max_pages_to_scan_attr.attr,
	&advisor_target_scan_time_attr.attr,
	NULL,
};

static const struct attribute_group ksm_attr_group = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.attrs = ksm_attrs,
	.name = "ksm",
};
#endif /* CONFIG_SYSFS */

static int __init ksm_init(void)
{
	struct task_struct *ksm_thread;
	int err;

<<<<<<< HEAD
=======
	/* The correct value depends on page size and endianness */
	zero_checksum = calc_checksum(ZERO_PAGE(0));
	/* Default to false for backwards compatibility */
	ksm_use_zero_pages = false;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ksm_slab_init();
	if (err)
		goto out;

	ksm_thread = kthread_run(ksm_scan_thread, NULL, "ksmd");
	if (IS_ERR(ksm_thread)) {
<<<<<<< HEAD
		printk(KERN_ERR "ksm: creating kthread failed\n");
=======
		pr_err("ksm: creating kthread failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = PTR_ERR(ksm_thread);
		goto out_free;
	}

#ifdef CONFIG_SYSFS
	err = sysfs_create_group(mm_kobj, &ksm_attr_group);
	if (err) {
<<<<<<< HEAD
		printk(KERN_ERR "ksm: register sysfs failed\n");
=======
		pr_err("ksm: register sysfs failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kthread_stop(ksm_thread);
		goto out_free;
	}
#else
	ksm_run = KSM_RUN_MERGE;	/* no way for user to start it */

#endif /* CONFIG_SYSFS */

#ifdef CONFIG_MEMORY_HOTREMOVE
<<<<<<< HEAD
	/*
	 * Choose a high priority since the callback takes ksm_thread_mutex:
	 * later callbacks could only be taking locks which nest within that.
	 */
	hotplug_memory_notifier(ksm_memory_callback, 100);
=======
	/* There is no significance to this priority 100 */
	hotplug_memory_notifier(ksm_memory_callback, KSM_CALLBACK_PRI);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	return 0;

out_free:
	ksm_slab_free();
out:
	return err;
}
<<<<<<< HEAD
module_init(ksm_init)
=======
subsys_initcall(ksm_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
