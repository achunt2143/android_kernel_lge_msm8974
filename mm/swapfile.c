<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/mm/swapfile.c
 *
 *  Copyright (C) 1991, 1992, 1993, 1994  Linus Torvalds
 *  Swap reorganised 29.12.95, Stephen Tweedie
 */

<<<<<<< HEAD
#include <linux/mm.h>
=======
#include <linux/blkdev.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/hugetlb.h>
#include <linux/mman.h>
#include <linux/slab.h>
#include <linux/kernel_stat.h>
#include <linux/swap.h>
#include <linux/vmalloc.h>
#include <linux/pagemap.h>
#include <linux/namei.h>
#include <linux/shmem_fs.h>
<<<<<<< HEAD
#include <linux/blkdev.h>
=======
#include <linux/blk-cgroup.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/random.h>
#include <linux/writeback.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/ksm.h>
#include <linux/rmap.h>
#include <linux/security.h>
#include <linux/backing-dev.h>
#include <linux/mutex.h>
#include <linux/capability.h>
#include <linux/syscalls.h>
#include <linux/memcontrol.h>
#include <linux/poll.h>
#include <linux/oom.h>
<<<<<<< HEAD
#include <linux/export.h>

#include <asm/pgtable.h>
#include <asm/tlbflush.h>
#include <linux/swapops.h>
#include <linux/page_cgroup.h>
=======
#include <linux/swapfile.h>
#include <linux/export.h>
#include <linux/swap_slots.h>
#include <linux/sort.h>
#include <linux/completion.h>
#include <linux/suspend.h>
#include <linux/zswap.h>
#include <linux/plist.h>

#include <asm/tlbflush.h>
#include <linux/swapops.h>
#include <linux/swap_cgroup.h>
#include "internal.h"
#include "swap.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static bool swap_count_continued(struct swap_info_struct *, pgoff_t,
				 unsigned char);
static void free_swap_count_continuations(struct swap_info_struct *);
<<<<<<< HEAD
static sector_t map_swap_entry(swp_entry_t, struct block_device**);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_SPINLOCK(swap_lock);
static unsigned int nr_swapfiles;
atomic_long_t nr_swap_pages;
<<<<<<< HEAD
/* protected with swap_lock. reading in vm_swap_full() doesn't need lock */
long total_swap_pages;
static int least_priority;
static atomic_t highest_priority_index = ATOMIC_INIT(-1);
=======
/*
 * Some modules use swappable objects and may try to swap them out under
 * memory pressure (via the shrinker). Before doing so, they may wish to
 * check to see if any swap space is available.
 */
EXPORT_SYMBOL_GPL(nr_swap_pages);
/* protected with swap_lock. reading in vm_swap_full() doesn't need lock */
long total_swap_pages;
static int least_priority = -1;
unsigned long swapfile_maximum_size;
#ifdef CONFIG_MIGRATION
bool swap_migration_ad_supported;
#endif	/* CONFIG_MIGRATION */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const char Bad_file[] = "Bad swap file entry ";
static const char Unused_file[] = "Unused swap file entry ";
static const char Bad_offset[] = "Bad swap offset entry ";
static const char Unused_offset[] = "Unused swap offset entry ";

<<<<<<< HEAD
static struct swap_list_t swap_list = {-1, -1};
=======
/*
 * all active swap_info_structs
 * protected with swap_lock, and ordered by priority.
 */
static PLIST_HEAD(swap_active_head);

/*
 * all available (active, not full) swap_info_structs
 * protected with swap_avail_lock, ordered by priority.
 * This is used by folio_alloc_swap() instead of swap_active_head
 * because swap_active_head includes all swap_info_structs,
 * but folio_alloc_swap() doesn't need to look at full ones.
 * This uses its own lock instead of swap_lock because when a
 * swap_info_struct changes between not-full/full, it needs to
 * add/remove itself to/from this list, but the swap_info_struct->lock
 * is held and the locking order requires swap_lock to be taken
 * before any swap_info_struct->lock.
 */
static struct plist_head *swap_avail_heads;
static DEFINE_SPINLOCK(swap_avail_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct swap_info_struct *swap_info[MAX_SWAPFILES];

static DEFINE_MUTEX(swapon_mutex);

static DECLARE_WAIT_QUEUE_HEAD(proc_poll_wait);
/* Activity counter to indicate that a swapon or swapoff has occurred */
static atomic_t proc_poll_event = ATOMIC_INIT(0);

<<<<<<< HEAD
static inline unsigned char swap_count(unsigned char ent)
{
	return ent & ~SWAP_HAS_CACHE;	/* may include SWAP_HAS_CONT flag */
}

bool is_swap_fast(swp_entry_t entry)
{
	struct swap_info_struct *p;
	unsigned long type;

	if (non_swap_entry(entry))
		return false;

	type = swp_type(entry);
	if (type >= nr_swapfiles)
		return false;

	p = swap_info[type];

	if (p->flags & SWP_FAST)
		return true;

	return false;
}

/* returns 1 if swap entry is freed */
static int
__try_to_reclaim_swap(struct swap_info_struct *si, unsigned long offset)
{
	swp_entry_t entry = swp_entry(si->type, offset);
	struct page *page;
	int ret = 0;

	page = find_get_page(&swapper_space, entry.val);
	if (!page)
		return 0;
	/*
	 * This function is called from scan_swap_map() and it's called
	 * by vmscan.c at reclaiming pages. So, we hold a lock on a page, here.
	 * We have to use trylock for avoiding deadlock. This is a special
	 * case and you should use try_to_free_swap() with explicit lock_page()
	 * in usual operations.
	 */
	if (trylock_page(page)) {
		ret = try_to_free_swap(page);
		unlock_page(page);
	}
	page_cache_release(page);
	return ret;
}

=======
atomic_t nr_rotate_swap = ATOMIC_INIT(0);

static struct swap_info_struct *swap_type_to_swap_info(int type)
{
	if (type >= MAX_SWAPFILES)
		return NULL;

	return READ_ONCE(swap_info[type]); /* rcu_dereference() */
}

static inline unsigned char swap_count(unsigned char ent)
{
	return ent & ~SWAP_HAS_CACHE;	/* may include COUNT_CONTINUED flag */
}

/* Reclaim the swap entry anyway if possible */
#define TTRS_ANYWAY		0x1
/*
 * Reclaim the swap entry if there are no more mappings of the
 * corresponding page
 */
#define TTRS_UNMAPPED		0x2
/* Reclaim the swap entry if swap is getting full*/
#define TTRS_FULL		0x4

/* returns 1 if swap entry is freed */
static int __try_to_reclaim_swap(struct swap_info_struct *si,
				 unsigned long offset, unsigned long flags)
{
	swp_entry_t entry = swp_entry(si->type, offset);
	struct folio *folio;
	int ret = 0;

	folio = filemap_get_folio(swap_address_space(entry), offset);
	if (IS_ERR(folio))
		return 0;
	/*
	 * When this function is called from scan_swap_map_slots() and it's
	 * called by vmscan.c at reclaiming folios. So we hold a folio lock
	 * here. We have to use trylock for avoiding deadlock. This is a special
	 * case and you should use folio_free_swap() with explicit folio_lock()
	 * in usual operations.
	 */
	if (folio_trylock(folio)) {
		if ((flags & TTRS_ANYWAY) ||
		    ((flags & TTRS_UNMAPPED) && !folio_mapped(folio)) ||
		    ((flags & TTRS_FULL) && mem_cgroup_swap_full(folio)))
			ret = folio_free_swap(folio);
		folio_unlock(folio);
	}
	folio_put(folio);
	return ret;
}

static inline struct swap_extent *first_se(struct swap_info_struct *sis)
{
	struct rb_node *rb = rb_first(&sis->swap_extent_root);
	return rb_entry(rb, struct swap_extent, rb_node);
}

static inline struct swap_extent *next_se(struct swap_extent *se)
{
	struct rb_node *rb = rb_next(&se->rb_node);
	return rb ? rb_entry(rb, struct swap_extent, rb_node) : NULL;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * swapon tell device that all the old swap contents can be discarded,
 * to allow the swap device to optimize its wear-levelling.
 */
static int discard_swap(struct swap_info_struct *si)
{
	struct swap_extent *se;
	sector_t start_block;
	sector_t nr_blocks;
	int err = 0;

	/* Do not discard the swap header page! */
<<<<<<< HEAD
	se = &si->first_swap_extent;
=======
	se = first_se(si);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	start_block = (se->start_block + 1) << (PAGE_SHIFT - 9);
	nr_blocks = ((sector_t)se->nr_pages - 1) << (PAGE_SHIFT - 9);
	if (nr_blocks) {
		err = blkdev_issue_discard(si->bdev, start_block,
<<<<<<< HEAD
				nr_blocks, GFP_KERNEL, 0);
=======
				nr_blocks, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			return err;
		cond_resched();
	}

<<<<<<< HEAD
	list_for_each_entry(se, &si->first_swap_extent.list, list) {
=======
	for (se = next_se(se); se; se = next_se(se)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		start_block = se->start_block << (PAGE_SHIFT - 9);
		nr_blocks = (sector_t)se->nr_pages << (PAGE_SHIFT - 9);

		err = blkdev_issue_discard(si->bdev, start_block,
<<<<<<< HEAD
				nr_blocks, GFP_KERNEL, 0);
=======
				nr_blocks, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			break;

		cond_resched();
	}
	return err;		/* That will often be -EOPNOTSUPP */
}

<<<<<<< HEAD
=======
static struct swap_extent *
offset_to_swap_extent(struct swap_info_struct *sis, unsigned long offset)
{
	struct swap_extent *se;
	struct rb_node *rb;

	rb = sis->swap_extent_root.rb_node;
	while (rb) {
		se = rb_entry(rb, struct swap_extent, rb_node);
		if (offset < se->start_page)
			rb = rb->rb_left;
		else if (offset >= se->start_page + se->nr_pages)
			rb = rb->rb_right;
		else
			return se;
	}
	/* It *must* be present */
	BUG();
}

sector_t swap_folio_sector(struct folio *folio)
{
	struct swap_info_struct *sis = swp_swap_info(folio->swap);
	struct swap_extent *se;
	sector_t sector;
	pgoff_t offset;

	offset = swp_offset(folio->swap);
	se = offset_to_swap_extent(sis, offset);
	sector = se->start_block + (offset - se->start_page);
	return sector << (PAGE_SHIFT - 9);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * swap allocation tell device that a cluster of swap can now be discarded,
 * to allow the swap device to optimize its wear-levelling.
 */
static void discard_swap_cluster(struct swap_info_struct *si,
				 pgoff_t start_page, pgoff_t nr_pages)
{
<<<<<<< HEAD
	struct swap_extent *se = si->curr_swap_extent;
	int found_extent = 0;

	while (nr_pages) {
		struct list_head *lh;

		if (se->start_page <= start_page &&
		    start_page < se->start_page + se->nr_pages) {
			pgoff_t offset = start_page - se->start_page;
			sector_t start_block = se->start_block + offset;
			sector_t nr_blocks = se->nr_pages - offset;

			if (nr_blocks > nr_pages)
				nr_blocks = nr_pages;
			start_page += nr_blocks;
			nr_pages -= nr_blocks;

			if (!found_extent++)
				si->curr_swap_extent = se;

			start_block <<= PAGE_SHIFT - 9;
			nr_blocks <<= PAGE_SHIFT - 9;
			if (blkdev_issue_discard(si->bdev, start_block,
				    nr_blocks, GFP_NOIO, 0))
				break;
		}

		lh = se->list.next;
		se = list_entry(lh, struct swap_extent, list);
	}
}

static int wait_for_discard(void *word)
{
	schedule();
	return 0;
}

#define SWAPFILE_CLUSTER	256
#define LATENCY_LIMIT		256

static unsigned long scan_swap_map(struct swap_info_struct *si,
				   unsigned char usage)
{
=======
	struct swap_extent *se = offset_to_swap_extent(si, start_page);

	while (nr_pages) {
		pgoff_t offset = start_page - se->start_page;
		sector_t start_block = se->start_block + offset;
		sector_t nr_blocks = se->nr_pages - offset;

		if (nr_blocks > nr_pages)
			nr_blocks = nr_pages;
		start_page += nr_blocks;
		nr_pages -= nr_blocks;

		start_block <<= PAGE_SHIFT - 9;
		nr_blocks <<= PAGE_SHIFT - 9;
		if (blkdev_issue_discard(si->bdev, start_block,
					nr_blocks, GFP_NOIO))
			break;

		se = next_se(se);
	}
}

#ifdef CONFIG_THP_SWAP
#define SWAPFILE_CLUSTER	HPAGE_PMD_NR

#define swap_entry_size(size)	(size)
#else
#define SWAPFILE_CLUSTER	256

/*
 * Define swap_entry_size() as constant to let compiler to optimize
 * out some code if !CONFIG_THP_SWAP
 */
#define swap_entry_size(size)	1
#endif
#define LATENCY_LIMIT		256

static inline void cluster_set_flag(struct swap_cluster_info *info,
	unsigned int flag)
{
	info->flags = flag;
}

static inline unsigned int cluster_count(struct swap_cluster_info *info)
{
	return info->data;
}

static inline void cluster_set_count(struct swap_cluster_info *info,
				     unsigned int c)
{
	info->data = c;
}

static inline void cluster_set_count_flag(struct swap_cluster_info *info,
					 unsigned int c, unsigned int f)
{
	info->flags = f;
	info->data = c;
}

static inline unsigned int cluster_next(struct swap_cluster_info *info)
{
	return info->data;
}

static inline void cluster_set_next(struct swap_cluster_info *info,
				    unsigned int n)
{
	info->data = n;
}

static inline void cluster_set_next_flag(struct swap_cluster_info *info,
					 unsigned int n, unsigned int f)
{
	info->flags = f;
	info->data = n;
}

static inline bool cluster_is_free(struct swap_cluster_info *info)
{
	return info->flags & CLUSTER_FLAG_FREE;
}

static inline bool cluster_is_null(struct swap_cluster_info *info)
{
	return info->flags & CLUSTER_FLAG_NEXT_NULL;
}

static inline void cluster_set_null(struct swap_cluster_info *info)
{
	info->flags = CLUSTER_FLAG_NEXT_NULL;
	info->data = 0;
}

static inline bool cluster_is_huge(struct swap_cluster_info *info)
{
	if (IS_ENABLED(CONFIG_THP_SWAP))
		return info->flags & CLUSTER_FLAG_HUGE;
	return false;
}

static inline void cluster_clear_huge(struct swap_cluster_info *info)
{
	info->flags &= ~CLUSTER_FLAG_HUGE;
}

static inline struct swap_cluster_info *lock_cluster(struct swap_info_struct *si,
						     unsigned long offset)
{
	struct swap_cluster_info *ci;

	ci = si->cluster_info;
	if (ci) {
		ci += offset / SWAPFILE_CLUSTER;
		spin_lock(&ci->lock);
	}
	return ci;
}

static inline void unlock_cluster(struct swap_cluster_info *ci)
{
	if (ci)
		spin_unlock(&ci->lock);
}

/*
 * Determine the locking method in use for this device.  Return
 * swap_cluster_info if SSD-style cluster-based locking is in place.
 */
static inline struct swap_cluster_info *lock_cluster_or_swap_info(
		struct swap_info_struct *si, unsigned long offset)
{
	struct swap_cluster_info *ci;

	/* Try to use fine-grained SSD-style locking if available: */
	ci = lock_cluster(si, offset);
	/* Otherwise, fall back to traditional, coarse locking: */
	if (!ci)
		spin_lock(&si->lock);

	return ci;
}

static inline void unlock_cluster_or_swap_info(struct swap_info_struct *si,
					       struct swap_cluster_info *ci)
{
	if (ci)
		unlock_cluster(ci);
	else
		spin_unlock(&si->lock);
}

static inline bool cluster_list_empty(struct swap_cluster_list *list)
{
	return cluster_is_null(&list->head);
}

static inline unsigned int cluster_list_first(struct swap_cluster_list *list)
{
	return cluster_next(&list->head);
}

static void cluster_list_init(struct swap_cluster_list *list)
{
	cluster_set_null(&list->head);
	cluster_set_null(&list->tail);
}

static void cluster_list_add_tail(struct swap_cluster_list *list,
				  struct swap_cluster_info *ci,
				  unsigned int idx)
{
	if (cluster_list_empty(list)) {
		cluster_set_next_flag(&list->head, idx, 0);
		cluster_set_next_flag(&list->tail, idx, 0);
	} else {
		struct swap_cluster_info *ci_tail;
		unsigned int tail = cluster_next(&list->tail);

		/*
		 * Nested cluster lock, but both cluster locks are
		 * only acquired when we held swap_info_struct->lock
		 */
		ci_tail = ci + tail;
		spin_lock_nested(&ci_tail->lock, SINGLE_DEPTH_NESTING);
		cluster_set_next(ci_tail, idx);
		spin_unlock(&ci_tail->lock);
		cluster_set_next_flag(&list->tail, idx, 0);
	}
}

static unsigned int cluster_list_del_first(struct swap_cluster_list *list,
					   struct swap_cluster_info *ci)
{
	unsigned int idx;

	idx = cluster_next(&list->head);
	if (cluster_next(&list->tail) == idx) {
		cluster_set_null(&list->head);
		cluster_set_null(&list->tail);
	} else
		cluster_set_next_flag(&list->head,
				      cluster_next(&ci[idx]), 0);

	return idx;
}

/* Add a cluster to discard list and schedule it to do discard */
static void swap_cluster_schedule_discard(struct swap_info_struct *si,
		unsigned int idx)
{
	/*
	 * If scan_swap_map_slots() can't find a free cluster, it will check
	 * si->swap_map directly. To make sure the discarding cluster isn't
	 * taken by scan_swap_map_slots(), mark the swap entries bad (occupied).
	 * It will be cleared after discard
	 */
	memset(si->swap_map + idx * SWAPFILE_CLUSTER,
			SWAP_MAP_BAD, SWAPFILE_CLUSTER);

	cluster_list_add_tail(&si->discard_clusters, si->cluster_info, idx);

	schedule_work(&si->discard_work);
}

static void __free_cluster(struct swap_info_struct *si, unsigned long idx)
{
	struct swap_cluster_info *ci = si->cluster_info;

	cluster_set_flag(ci + idx, CLUSTER_FLAG_FREE);
	cluster_list_add_tail(&si->free_clusters, ci, idx);
}

/*
 * Doing discard actually. After a cluster discard is finished, the cluster
 * will be added to free cluster list. caller should hold si->lock.
*/
static void swap_do_scheduled_discard(struct swap_info_struct *si)
{
	struct swap_cluster_info *info, *ci;
	unsigned int idx;

	info = si->cluster_info;

	while (!cluster_list_empty(&si->discard_clusters)) {
		idx = cluster_list_del_first(&si->discard_clusters, info);
		spin_unlock(&si->lock);

		discard_swap_cluster(si, idx * SWAPFILE_CLUSTER,
				SWAPFILE_CLUSTER);

		spin_lock(&si->lock);
		ci = lock_cluster(si, idx * SWAPFILE_CLUSTER);
		__free_cluster(si, idx);
		memset(si->swap_map + idx * SWAPFILE_CLUSTER,
				0, SWAPFILE_CLUSTER);
		unlock_cluster(ci);
	}
}

static void swap_discard_work(struct work_struct *work)
{
	struct swap_info_struct *si;

	si = container_of(work, struct swap_info_struct, discard_work);

	spin_lock(&si->lock);
	swap_do_scheduled_discard(si);
	spin_unlock(&si->lock);
}

static void swap_users_ref_free(struct percpu_ref *ref)
{
	struct swap_info_struct *si;

	si = container_of(ref, struct swap_info_struct, users);
	complete(&si->comp);
}

static void alloc_cluster(struct swap_info_struct *si, unsigned long idx)
{
	struct swap_cluster_info *ci = si->cluster_info;

	VM_BUG_ON(cluster_list_first(&si->free_clusters) != idx);
	cluster_list_del_first(&si->free_clusters, ci);
	cluster_set_count_flag(ci + idx, 0, 0);
}

static void free_cluster(struct swap_info_struct *si, unsigned long idx)
{
	struct swap_cluster_info *ci = si->cluster_info + idx;

	VM_BUG_ON(cluster_count(ci) != 0);
	/*
	 * If the swap is discardable, prepare discard the cluster
	 * instead of free it immediately. The cluster will be freed
	 * after discard.
	 */
	if ((si->flags & (SWP_WRITEOK | SWP_PAGE_DISCARD)) ==
	    (SWP_WRITEOK | SWP_PAGE_DISCARD)) {
		swap_cluster_schedule_discard(si, idx);
		return;
	}

	__free_cluster(si, idx);
}

/*
 * The cluster corresponding to page_nr will be used. The cluster will be
 * removed from free cluster list and its usage counter will be increased.
 */
static void inc_cluster_info_page(struct swap_info_struct *p,
	struct swap_cluster_info *cluster_info, unsigned long page_nr)
{
	unsigned long idx = page_nr / SWAPFILE_CLUSTER;

	if (!cluster_info)
		return;
	if (cluster_is_free(&cluster_info[idx]))
		alloc_cluster(p, idx);

	VM_BUG_ON(cluster_count(&cluster_info[idx]) >= SWAPFILE_CLUSTER);
	cluster_set_count(&cluster_info[idx],
		cluster_count(&cluster_info[idx]) + 1);
}

/*
 * The cluster corresponding to page_nr decreases one usage. If the usage
 * counter becomes 0, which means no page in the cluster is in using, we can
 * optionally discard the cluster and add it to free cluster list.
 */
static void dec_cluster_info_page(struct swap_info_struct *p,
	struct swap_cluster_info *cluster_info, unsigned long page_nr)
{
	unsigned long idx = page_nr / SWAPFILE_CLUSTER;

	if (!cluster_info)
		return;

	VM_BUG_ON(cluster_count(&cluster_info[idx]) == 0);
	cluster_set_count(&cluster_info[idx],
		cluster_count(&cluster_info[idx]) - 1);

	if (cluster_count(&cluster_info[idx]) == 0)
		free_cluster(p, idx);
}

/*
 * It's possible scan_swap_map_slots() uses a free cluster in the middle of free
 * cluster list. Avoiding such abuse to avoid list corruption.
 */
static bool
scan_swap_map_ssd_cluster_conflict(struct swap_info_struct *si,
	unsigned long offset)
{
	struct percpu_cluster *percpu_cluster;
	bool conflict;

	offset /= SWAPFILE_CLUSTER;
	conflict = !cluster_list_empty(&si->free_clusters) &&
		offset != cluster_list_first(&si->free_clusters) &&
		cluster_is_free(&si->cluster_info[offset]);

	if (!conflict)
		return false;

	percpu_cluster = this_cpu_ptr(si->percpu_cluster);
	cluster_set_null(&percpu_cluster->index);
	return true;
}

/*
 * Try to get a swap entry from current cpu's swap entry pool (a cluster). This
 * might involve allocating a new cluster for current CPU too.
 */
static bool scan_swap_map_try_ssd_cluster(struct swap_info_struct *si,
	unsigned long *offset, unsigned long *scan_base)
{
	struct percpu_cluster *cluster;
	struct swap_cluster_info *ci;
	unsigned long tmp, max;

new_cluster:
	cluster = this_cpu_ptr(si->percpu_cluster);
	if (cluster_is_null(&cluster->index)) {
		if (!cluster_list_empty(&si->free_clusters)) {
			cluster->index = si->free_clusters.head;
			cluster->next = cluster_next(&cluster->index) *
					SWAPFILE_CLUSTER;
		} else if (!cluster_list_empty(&si->discard_clusters)) {
			/*
			 * we don't have free cluster but have some clusters in
			 * discarding, do discard now and reclaim them, then
			 * reread cluster_next_cpu since we dropped si->lock
			 */
			swap_do_scheduled_discard(si);
			*scan_base = this_cpu_read(*si->cluster_next_cpu);
			*offset = *scan_base;
			goto new_cluster;
		} else
			return false;
	}

	/*
	 * Other CPUs can use our cluster if they can't find a free cluster,
	 * check if there is still free entry in the cluster
	 */
	tmp = cluster->next;
	max = min_t(unsigned long, si->max,
		    (cluster_next(&cluster->index) + 1) * SWAPFILE_CLUSTER);
	if (tmp < max) {
		ci = lock_cluster(si, tmp);
		while (tmp < max) {
			if (!si->swap_map[tmp])
				break;
			tmp++;
		}
		unlock_cluster(ci);
	}
	if (tmp >= max) {
		cluster_set_null(&cluster->index);
		goto new_cluster;
	}
	cluster->next = tmp + 1;
	*offset = tmp;
	*scan_base = tmp;
	return true;
}

static void __del_from_avail_list(struct swap_info_struct *p)
{
	int nid;

	assert_spin_locked(&p->lock);
	for_each_node(nid)
		plist_del(&p->avail_lists[nid], &swap_avail_heads[nid]);
}

static void del_from_avail_list(struct swap_info_struct *p)
{
	spin_lock(&swap_avail_lock);
	__del_from_avail_list(p);
	spin_unlock(&swap_avail_lock);
}

static void swap_range_alloc(struct swap_info_struct *si, unsigned long offset,
			     unsigned int nr_entries)
{
	unsigned int end = offset + nr_entries - 1;

	if (offset == si->lowest_bit)
		si->lowest_bit += nr_entries;
	if (end == si->highest_bit)
		WRITE_ONCE(si->highest_bit, si->highest_bit - nr_entries);
	WRITE_ONCE(si->inuse_pages, si->inuse_pages + nr_entries);
	if (si->inuse_pages == si->pages) {
		si->lowest_bit = si->max;
		si->highest_bit = 0;
		del_from_avail_list(si);
	}
}

static void add_to_avail_list(struct swap_info_struct *p)
{
	int nid;

	spin_lock(&swap_avail_lock);
	for_each_node(nid)
		plist_add(&p->avail_lists[nid], &swap_avail_heads[nid]);
	spin_unlock(&swap_avail_lock);
}

static void swap_range_free(struct swap_info_struct *si, unsigned long offset,
			    unsigned int nr_entries)
{
	unsigned long begin = offset;
	unsigned long end = offset + nr_entries - 1;
	void (*swap_slot_free_notify)(struct block_device *, unsigned long);

	if (offset < si->lowest_bit)
		si->lowest_bit = offset;
	if (end > si->highest_bit) {
		bool was_full = !si->highest_bit;

		WRITE_ONCE(si->highest_bit, end);
		if (was_full && (si->flags & SWP_WRITEOK))
			add_to_avail_list(si);
	}
	if (si->flags & SWP_BLKDEV)
		swap_slot_free_notify =
			si->bdev->bd_disk->fops->swap_slot_free_notify;
	else
		swap_slot_free_notify = NULL;
	while (offset <= end) {
		arch_swap_invalidate_page(si->type, offset);
		if (swap_slot_free_notify)
			swap_slot_free_notify(si->bdev, offset);
		offset++;
	}
	clear_shadow_from_swap_cache(si->type, begin, end);

	/*
	 * Make sure that try_to_unuse() observes si->inuse_pages reaching 0
	 * only after the above cleanups are done.
	 */
	smp_wmb();
	atomic_long_add(nr_entries, &nr_swap_pages);
	WRITE_ONCE(si->inuse_pages, si->inuse_pages - nr_entries);
}

static void set_cluster_next(struct swap_info_struct *si, unsigned long next)
{
	unsigned long prev;

	if (!(si->flags & SWP_SOLIDSTATE)) {
		si->cluster_next = next;
		return;
	}

	prev = this_cpu_read(*si->cluster_next_cpu);
	/*
	 * Cross the swap address space size aligned trunk, choose
	 * another trunk randomly to avoid lock contention on swap
	 * address space if possible.
	 */
	if ((prev >> SWAP_ADDRESS_SPACE_SHIFT) !=
	    (next >> SWAP_ADDRESS_SPACE_SHIFT)) {
		/* No free swap slots available */
		if (si->highest_bit <= si->lowest_bit)
			return;
		next = get_random_u32_inclusive(si->lowest_bit, si->highest_bit);
		next = ALIGN_DOWN(next, SWAP_ADDRESS_SPACE_PAGES);
		next = max_t(unsigned int, next, si->lowest_bit);
	}
	this_cpu_write(*si->cluster_next_cpu, next);
}

static bool swap_offset_available_and_locked(struct swap_info_struct *si,
					     unsigned long offset)
{
	if (data_race(!si->swap_map[offset])) {
		spin_lock(&si->lock);
		return true;
	}

	if (vm_swap_full() && READ_ONCE(si->swap_map[offset]) == SWAP_HAS_CACHE) {
		spin_lock(&si->lock);
		return true;
	}

	return false;
}

static int scan_swap_map_slots(struct swap_info_struct *si,
			       unsigned char usage, int nr,
			       swp_entry_t slots[])
{
	struct swap_cluster_info *ci;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long offset;
	unsigned long scan_base;
	unsigned long last_in_cluster = 0;
	int latency_ration = LATENCY_LIMIT;
<<<<<<< HEAD
	int found_free_cluster = 0;
=======
	int n_ret = 0;
	bool scanned_many = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * We try to cluster swap pages by allocating them sequentially
	 * in swap.  Once we've allocated SWAPFILE_CLUSTER pages this
	 * way, however, we resort to first-free allocation, starting
	 * a new cluster.  This prevents us from scattering swap pages
	 * all over the entire swap partition, so that we reduce
	 * overall disk seek times between swap pages.  -- sct
	 * But we do now try to find an empty cluster.  -Andrea
	 * And we let swap pages go all over an SSD partition.  Hugh
	 */

	si->flags += SWP_SCANNING;
<<<<<<< HEAD
	scan_base = offset = si->cluster_next;

	if (unlikely(!si->cluster_nr--)) {
=======
	/*
	 * Use percpu scan base for SSD to reduce lock contention on
	 * cluster and swap cache.  For HDD, sequential access is more
	 * important.
	 */
	if (si->flags & SWP_SOLIDSTATE)
		scan_base = this_cpu_read(*si->cluster_next_cpu);
	else
		scan_base = si->cluster_next;
	offset = scan_base;

	/* SSD algorithm */
	if (si->cluster_info) {
		if (!scan_swap_map_try_ssd_cluster(si, &offset, &scan_base))
			goto scan;
	} else if (unlikely(!si->cluster_nr--)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (si->pages - si->inuse_pages < SWAPFILE_CLUSTER) {
			si->cluster_nr = SWAPFILE_CLUSTER - 1;
			goto checks;
		}
<<<<<<< HEAD
		if (si->flags & SWP_DISCARDABLE) {
			/*
			 * Start range check on racing allocations, in case
			 * they overlap the cluster we eventually decide on
			 * (we scan without swap_lock to allow preemption).
			 * It's hardly conceivable that cluster_nr could be
			 * wrapped during our scan, but don't depend on it.
			 */
			if (si->lowest_alloc)
				goto checks;
			si->lowest_alloc = si->max;
			si->highest_alloc = 0;
		}
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock(&si->lock);

		/*
		 * If seek is expensive, start searching for new cluster from
		 * start of partition, to minimize the span of allocated swap.
<<<<<<< HEAD
		 * But if seek is cheap, search from our current position, so
		 * that swap is allocated from all over the partition: if the
		 * Flash Translation Layer only remaps within limited zones,
		 * we don't want to wear out the first zone too quickly.
		 */
		if (!(si->flags & SWP_SOLIDSTATE))
			scan_base = offset = si->lowest_bit;
=======
		 * If seek is cheap, that is the SWP_SOLIDSTATE si->cluster_info
		 * case, just handled by scan_swap_map_try_ssd_cluster() above.
		 */
		scan_base = offset = si->lowest_bit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		last_in_cluster = offset + SWAPFILE_CLUSTER - 1;

		/* Locate the first empty (unaligned) cluster */
		for (; last_in_cluster <= si->highest_bit; offset++) {
			if (si->swap_map[offset])
				last_in_cluster = offset + SWAPFILE_CLUSTER;
			else if (offset == last_in_cluster) {
				spin_lock(&si->lock);
				offset -= SWAPFILE_CLUSTER - 1;
				si->cluster_next = offset;
				si->cluster_nr = SWAPFILE_CLUSTER - 1;
<<<<<<< HEAD
				found_free_cluster = 1;
				goto checks;
			}
			if (unlikely(--latency_ration < 0)) {
				cond_resched();
				latency_ration = LATENCY_LIMIT;
			}
		}

		offset = si->lowest_bit;
		last_in_cluster = offset + SWAPFILE_CLUSTER - 1;

		/* Locate the first empty (unaligned) cluster */
		for (; last_in_cluster < scan_base; offset++) {
			if (si->swap_map[offset])
				last_in_cluster = offset + SWAPFILE_CLUSTER;
			else if (offset == last_in_cluster) {
				spin_lock(&si->lock);
				offset -= SWAPFILE_CLUSTER - 1;
				si->cluster_next = offset;
				si->cluster_nr = SWAPFILE_CLUSTER - 1;
				found_free_cluster = 1;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto checks;
			}
			if (unlikely(--latency_ration < 0)) {
				cond_resched();
				latency_ration = LATENCY_LIMIT;
			}
		}

		offset = scan_base;
		spin_lock(&si->lock);
		si->cluster_nr = SWAPFILE_CLUSTER - 1;
<<<<<<< HEAD
		si->lowest_alloc = 0;
	}

checks:
=======
	}

checks:
	if (si->cluster_info) {
		while (scan_swap_map_ssd_cluster_conflict(si, offset)) {
		/* take a break if we already got some slots */
			if (n_ret)
				goto done;
			if (!scan_swap_map_try_ssd_cluster(si, &offset,
							&scan_base))
				goto scan;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(si->flags & SWP_WRITEOK))
		goto no_page;
	if (!si->highest_bit)
		goto no_page;
	if (offset > si->highest_bit)
		scan_base = offset = si->lowest_bit;

<<<<<<< HEAD
	/* reuse swap entry of cache-only swap if not busy. */
	if (vm_swap_full(si) && si->swap_map[offset] == SWAP_HAS_CACHE) {
		int swap_was_freed;
		spin_unlock(&si->lock);
		swap_was_freed = __try_to_reclaim_swap(si, offset);
=======
	ci = lock_cluster(si, offset);
	/* reuse swap entry of cache-only swap if not busy. */
	if (vm_swap_full() && si->swap_map[offset] == SWAP_HAS_CACHE) {
		int swap_was_freed;
		unlock_cluster(ci);
		spin_unlock(&si->lock);
		swap_was_freed = __try_to_reclaim_swap(si, offset, TTRS_ANYWAY);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_lock(&si->lock);
		/* entry was freed successfully, try to use this again */
		if (swap_was_freed)
			goto checks;
		goto scan; /* check next one */
	}

<<<<<<< HEAD
	if (si->swap_map[offset])
		goto scan;

	if (offset == si->lowest_bit)
		si->lowest_bit++;
	if (offset == si->highest_bit)
		si->highest_bit--;
	si->inuse_pages++;
	if (si->inuse_pages == si->pages) {
		si->lowest_bit = si->max;
		si->highest_bit = 0;
	}
	si->swap_map[offset] = usage;
	si->cluster_next = offset + 1;
	si->flags -= SWP_SCANNING;

	if (si->lowest_alloc) {
		/*
		 * Only set when SWP_DISCARDABLE, and there's a scan
		 * for a free cluster in progress or just completed.
		 */
		if (found_free_cluster) {
			/*
			 * To optimize wear-levelling, discard the
			 * old data of the cluster, taking care not to
			 * discard any of its pages that have already
			 * been allocated by racing tasks (offset has
			 * already stepped over any at the beginning).
			 */
			if (offset < si->highest_alloc &&
			    si->lowest_alloc <= last_in_cluster)
				last_in_cluster = si->lowest_alloc - 1;
			si->flags |= SWP_DISCARDING;
			spin_unlock(&si->lock);

			if (offset < last_in_cluster)
				discard_swap_cluster(si, offset,
					last_in_cluster - offset + 1);

			spin_lock(&si->lock);
			si->lowest_alloc = 0;
			si->flags &= ~SWP_DISCARDING;

			smp_mb();	/* wake_up_bit advises this */
			wake_up_bit(&si->flags, ilog2(SWP_DISCARDING));

		} else if (si->flags & SWP_DISCARDING) {
			/*
			 * Delay using pages allocated by racing tasks
			 * until the whole discard has been issued. We
			 * could defer that delay until swap_writepage,
			 * but it's easier to keep this self-contained.
			 */
			spin_unlock(&si->lock);
			wait_on_bit(&si->flags, ilog2(SWP_DISCARDING),
				wait_for_discard, TASK_UNINTERRUPTIBLE);
			spin_lock(&si->lock);
		} else {
			/*
			 * Note pages allocated by racing tasks while
			 * scan for a free cluster is in progress, so
			 * that its final discard can exclude them.
			 */
			if (offset < si->lowest_alloc)
				si->lowest_alloc = offset;
			if (offset > si->highest_alloc)
				si->highest_alloc = offset;
		}
	}
	return offset;

scan:
	spin_unlock(&si->lock);
	while (++offset <= si->highest_bit) {
		if (!si->swap_map[offset]) {
			spin_lock(&si->lock);
			goto checks;
		}
		if (vm_swap_full(si) &&
			si->swap_map[offset] == SWAP_HAS_CACHE) {
			spin_lock(&si->lock);
			goto checks;
		}
		if (unlikely(--latency_ration < 0)) {
			cond_resched();
			latency_ration = LATENCY_LIMIT;
		}
	}
	offset = si->lowest_bit;
	while (++offset < scan_base) {
		if (!si->swap_map[offset]) {
			spin_lock(&si->lock);
			goto checks;
		}
		if (vm_swap_full(si) &&
			si->swap_map[offset] == SWAP_HAS_CACHE) {
			spin_lock(&si->lock);
			goto checks;
		}
		if (unlikely(--latency_ration < 0)) {
			cond_resched();
			latency_ration = LATENCY_LIMIT;
		}
=======
	if (si->swap_map[offset]) {
		unlock_cluster(ci);
		if (!n_ret)
			goto scan;
		else
			goto done;
	}
	WRITE_ONCE(si->swap_map[offset], usage);
	inc_cluster_info_page(si, si->cluster_info, offset);
	unlock_cluster(ci);

	swap_range_alloc(si, offset, 1);
	slots[n_ret++] = swp_entry(si->type, offset);

	/* got enough slots or reach max slots? */
	if ((n_ret == nr) || (offset >= si->highest_bit))
		goto done;

	/* search for next available slot */

	/* time to take a break? */
	if (unlikely(--latency_ration < 0)) {
		if (n_ret)
			goto done;
		spin_unlock(&si->lock);
		cond_resched();
		spin_lock(&si->lock);
		latency_ration = LATENCY_LIMIT;
	}

	/* try to get more slots in cluster */
	if (si->cluster_info) {
		if (scan_swap_map_try_ssd_cluster(si, &offset, &scan_base))
			goto checks;
	} else if (si->cluster_nr && !si->swap_map[++offset]) {
		/* non-ssd case, still more slots in cluster? */
		--si->cluster_nr;
		goto checks;
	}

	/*
	 * Even if there's no free clusters available (fragmented),
	 * try to scan a little more quickly with lock held unless we
	 * have scanned too many slots already.
	 */
	if (!scanned_many) {
		unsigned long scan_limit;

		if (offset < scan_base)
			scan_limit = scan_base;
		else
			scan_limit = si->highest_bit;
		for (; offset <= scan_limit && --latency_ration > 0;
		     offset++) {
			if (!si->swap_map[offset])
				goto checks;
		}
	}

done:
	set_cluster_next(si, offset + 1);
	si->flags -= SWP_SCANNING;
	return n_ret;

scan:
	spin_unlock(&si->lock);
	while (++offset <= READ_ONCE(si->highest_bit)) {
		if (unlikely(--latency_ration < 0)) {
			cond_resched();
			latency_ration = LATENCY_LIMIT;
			scanned_many = true;
		}
		if (swap_offset_available_and_locked(si, offset))
			goto checks;
	}
	offset = si->lowest_bit;
	while (offset < scan_base) {
		if (unlikely(--latency_ration < 0)) {
			cond_resched();
			latency_ration = LATENCY_LIMIT;
			scanned_many = true;
		}
		if (swap_offset_available_and_locked(si, offset))
			goto checks;
		offset++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	spin_lock(&si->lock);

no_page:
	si->flags -= SWP_SCANNING;
<<<<<<< HEAD
	return 0;
}

swp_entry_t get_swap_page(void)
{
	struct swap_info_struct *si;
	pgoff_t offset;
	int type, next;
	int wrapped = 0;
	int hp_index;

	spin_lock(&swap_lock);
	if (atomic_long_read(&nr_swap_pages) <= 0)
		goto noswap;
	atomic_long_dec(&nr_swap_pages);

	for (type = swap_list.next; type >= 0 && wrapped < 2; type = next) {
		hp_index = atomic_xchg(&highest_priority_index, -1);
		/*
		 * highest_priority_index records current highest priority swap
		 * type which just frees swap entries. If its priority is
		 * higher than that of swap_list.next swap type, we use it.  It
		 * isn't protected by swap_lock, so it can be an invalid value
		 * if the corresponding swap type is swapoff. We double check
		 * the flags here. It's even possible the swap type is swapoff
		 * and swapon again and its priority is changed. In such rare
		 * case, low prority swap type might be used, but eventually
		 * high priority swap will be used after several rounds of
		 * swap.
		 */
		if (hp_index != -1 && hp_index != type &&
		    swap_info[type]->prio < swap_info[hp_index]->prio &&
		    (swap_info[hp_index]->flags & SWP_WRITEOK)) {
			type = hp_index;
			swap_list.next = type;
		}

		si = swap_info[type];
		next = si->next;
		if (next < 0 ||
		    (!wrapped && si->prio != swap_info[next]->prio)) {
			next = swap_list.head;
			wrapped++;
		}

		spin_lock(&si->lock);
		if (!si->highest_bit) {
			spin_unlock(&si->lock);
			continue;
		}
		if (!(si->flags & SWP_WRITEOK)) {
			spin_unlock(&si->lock);
			continue;
		}

		swap_list.next = next;

		spin_unlock(&swap_lock);
		/* This is called for allocating swap entry for cache */
		offset = scan_swap_map(si, SWAP_HAS_CACHE);
		spin_unlock(&si->lock);
		if (offset)
			return swp_entry(type, offset);
		spin_lock(&swap_lock);
		next = swap_list.next;
	}

	atomic_long_inc(&nr_swap_pages);
noswap:
	spin_unlock(&swap_lock);
	return (swp_entry_t) {0};
}

/* The only caller of this function is now susupend routine */
swp_entry_t get_swap_page_of_type(int type)
{
	struct swap_info_struct *si;
	pgoff_t offset;

	si = swap_info[type];
	spin_lock(&si->lock);
	if (si && (si->flags & SWP_WRITEOK)) {
		atomic_long_dec(&nr_swap_pages);
		/* This is called for allocating swap entry, not cache */
		offset = scan_swap_map(si, 1);
		if (offset) {
			spin_unlock(&si->lock);
			return swp_entry(type, offset);
		}
		atomic_long_inc(&nr_swap_pages);
	}
	spin_unlock(&si->lock);
	return (swp_entry_t) {0};
}

static struct swap_info_struct *swap_info_get(swp_entry_t entry)
{
	struct swap_info_struct *p;
	unsigned long offset, type;

	if (!entry.val)
		goto out;
	type = swp_type(entry);
	if (type >= nr_swapfiles)
		goto bad_nofile;
	p = swap_info[type];
	if (!(p->flags & SWP_USED))
=======
	return n_ret;
}

static int swap_alloc_cluster(struct swap_info_struct *si, swp_entry_t *slot)
{
	unsigned long idx;
	struct swap_cluster_info *ci;
	unsigned long offset;

	/*
	 * Should not even be attempting cluster allocations when huge
	 * page swap is disabled.  Warn and fail the allocation.
	 */
	if (!IS_ENABLED(CONFIG_THP_SWAP)) {
		VM_WARN_ON_ONCE(1);
		return 0;
	}

	if (cluster_list_empty(&si->free_clusters))
		return 0;

	idx = cluster_list_first(&si->free_clusters);
	offset = idx * SWAPFILE_CLUSTER;
	ci = lock_cluster(si, offset);
	alloc_cluster(si, idx);
	cluster_set_count_flag(ci, SWAPFILE_CLUSTER, CLUSTER_FLAG_HUGE);

	memset(si->swap_map + offset, SWAP_HAS_CACHE, SWAPFILE_CLUSTER);
	unlock_cluster(ci);
	swap_range_alloc(si, offset, SWAPFILE_CLUSTER);
	*slot = swp_entry(si->type, offset);

	return 1;
}

static void swap_free_cluster(struct swap_info_struct *si, unsigned long idx)
{
	unsigned long offset = idx * SWAPFILE_CLUSTER;
	struct swap_cluster_info *ci;

	ci = lock_cluster(si, offset);
	memset(si->swap_map + offset, 0, SWAPFILE_CLUSTER);
	cluster_set_count_flag(ci, 0, 0);
	free_cluster(si, idx);
	unlock_cluster(ci);
	swap_range_free(si, offset, SWAPFILE_CLUSTER);
}

int get_swap_pages(int n_goal, swp_entry_t swp_entries[], int entry_size)
{
	unsigned long size = swap_entry_size(entry_size);
	struct swap_info_struct *si, *next;
	long avail_pgs;
	int n_ret = 0;
	int node;

	/* Only single cluster request supported */
	WARN_ON_ONCE(n_goal > 1 && size == SWAPFILE_CLUSTER);

	spin_lock(&swap_avail_lock);

	avail_pgs = atomic_long_read(&nr_swap_pages) / size;
	if (avail_pgs <= 0) {
		spin_unlock(&swap_avail_lock);
		goto noswap;
	}

	n_goal = min3((long)n_goal, (long)SWAP_BATCH, avail_pgs);

	atomic_long_sub(n_goal * size, &nr_swap_pages);

start_over:
	node = numa_node_id();
	plist_for_each_entry_safe(si, next, &swap_avail_heads[node], avail_lists[node]) {
		/* requeue si to after same-priority siblings */
		plist_requeue(&si->avail_lists[node], &swap_avail_heads[node]);
		spin_unlock(&swap_avail_lock);
		spin_lock(&si->lock);
		if (!si->highest_bit || !(si->flags & SWP_WRITEOK)) {
			spin_lock(&swap_avail_lock);
			if (plist_node_empty(&si->avail_lists[node])) {
				spin_unlock(&si->lock);
				goto nextsi;
			}
			WARN(!si->highest_bit,
			     "swap_info %d in list but !highest_bit\n",
			     si->type);
			WARN(!(si->flags & SWP_WRITEOK),
			     "swap_info %d in list but !SWP_WRITEOK\n",
			     si->type);
			__del_from_avail_list(si);
			spin_unlock(&si->lock);
			goto nextsi;
		}
		if (size == SWAPFILE_CLUSTER) {
			if (si->flags & SWP_BLKDEV)
				n_ret = swap_alloc_cluster(si, swp_entries);
		} else
			n_ret = scan_swap_map_slots(si, SWAP_HAS_CACHE,
						    n_goal, swp_entries);
		spin_unlock(&si->lock);
		if (n_ret || size == SWAPFILE_CLUSTER)
			goto check_out;
		cond_resched();

		spin_lock(&swap_avail_lock);
nextsi:
		/*
		 * if we got here, it's likely that si was almost full before,
		 * and since scan_swap_map_slots() can drop the si->lock,
		 * multiple callers probably all tried to get a page from the
		 * same si and it filled up before we could get one; or, the si
		 * filled up between us dropping swap_avail_lock and taking
		 * si->lock. Since we dropped the swap_avail_lock, the
		 * swap_avail_head list may have been modified; so if next is
		 * still in the swap_avail_head list then try it, otherwise
		 * start over if we have not gotten any slots.
		 */
		if (plist_node_empty(&next->avail_lists[node]))
			goto start_over;
	}

	spin_unlock(&swap_avail_lock);

check_out:
	if (n_ret < n_goal)
		atomic_long_add((long)(n_goal - n_ret) * size,
				&nr_swap_pages);
noswap:
	return n_ret;
}

static struct swap_info_struct *_swap_info_get(swp_entry_t entry)
{
	struct swap_info_struct *p;
	unsigned long offset;

	if (!entry.val)
		goto out;
	p = swp_swap_info(entry);
	if (!p)
		goto bad_nofile;
	if (data_race(!(p->flags & SWP_USED)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto bad_device;
	offset = swp_offset(entry);
	if (offset >= p->max)
		goto bad_offset;
<<<<<<< HEAD
	if (!p->swap_map[offset])
		goto bad_free;
	spin_lock(&p->lock);
	return p;

bad_free:
	printk(KERN_ERR "swap_free: %s%08lx\n", Unused_offset, entry.val);
	goto out;
bad_offset:
	printk(KERN_ERR "swap_free: %s%08lx\n", Bad_offset, entry.val);
	goto out;
bad_device:
	printk(KERN_ERR "swap_free: %s%08lx\n", Unused_file, entry.val);
	goto out;
bad_nofile:
	printk(KERN_ERR "swap_free: %s%08lx\n", Bad_file, entry.val);
=======
	if (data_race(!p->swap_map[swp_offset(entry)]))
		goto bad_free;
	return p;

bad_free:
	pr_err("%s: %s%08lx\n", __func__, Unused_offset, entry.val);
	goto out;
bad_offset:
	pr_err("%s: %s%08lx\n", __func__, Bad_offset, entry.val);
	goto out;
bad_device:
	pr_err("%s: %s%08lx\n", __func__, Unused_file, entry.val);
	goto out;
bad_nofile:
	pr_err("%s: %s%08lx\n", __func__, Bad_file, entry.val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return NULL;
}

<<<<<<< HEAD
/*
 * This swap type frees swap entry, check if it is the highest priority swap
 * type which just frees swap entry. get_swap_page() uses
 * highest_priority_index to search highest priority swap type. The
 * swap_info_struct.lock can't protect us if there are multiple swap types
 * active, so we use atomic_cmpxchg.
 */
static void set_highest_priority_index(int type)
{
	int old_hp_index, new_hp_index;

	do {
		old_hp_index = atomic_read(&highest_priority_index);
		if (old_hp_index != -1 &&
			swap_info[old_hp_index]->prio >= swap_info[type]->prio)
			break;
		new_hp_index = type;
	} while (atomic_cmpxchg(&highest_priority_index,
		old_hp_index, new_hp_index) != old_hp_index);
}

static unsigned char swap_entry_free(struct swap_info_struct *p,
				     swp_entry_t entry, unsigned char usage)
{
	unsigned long offset = swp_offset(entry);
=======
static struct swap_info_struct *swap_info_get_cont(swp_entry_t entry,
					struct swap_info_struct *q)
{
	struct swap_info_struct *p;

	p = _swap_info_get(entry);

	if (p != q) {
		if (q != NULL)
			spin_unlock(&q->lock);
		if (p != NULL)
			spin_lock(&p->lock);
	}
	return p;
}

static unsigned char __swap_entry_free_locked(struct swap_info_struct *p,
					      unsigned long offset,
					      unsigned char usage)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned char count;
	unsigned char has_cache;

	count = p->swap_map[offset];
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	has_cache = count & SWAP_HAS_CACHE;
	count &= ~SWAP_HAS_CACHE;

	if (usage == SWAP_HAS_CACHE) {
		VM_BUG_ON(!has_cache);
		has_cache = 0;
	} else if (count == SWAP_MAP_SHMEM) {
		/*
		 * Or we could insist on shmem.c using a special
		 * swap_shmem_free() and free_shmem_swap_and_cache()...
		 */
		count = 0;
	} else if ((count & ~COUNT_CONTINUED) <= SWAP_MAP_MAX) {
		if (count == COUNT_CONTINUED) {
			if (swap_count_continued(p, offset, count))
				count = SWAP_MAP_MAX | COUNT_CONTINUED;
			else
				count = SWAP_MAP_MAX;
		} else
			count--;
	}

<<<<<<< HEAD
	if (!count)
		mem_cgroup_uncharge_swap(entry);

	usage = count | has_cache;
	p->swap_map[offset] = usage;

	/* free if no reference */
	if (!usage) {
		struct gendisk *disk = p->bdev->bd_disk;
		if (offset < p->lowest_bit)
			p->lowest_bit = offset;
		if (offset > p->highest_bit)
			p->highest_bit = offset;
		set_highest_priority_index(p->type);
		atomic_long_inc(&nr_swap_pages);
		p->inuse_pages--;
		if ((p->flags & SWP_BLKDEV) &&
				disk->fops->swap_slot_free_notify)
			disk->fops->swap_slot_free_notify(p->bdev, offset);
	}
=======
	usage = count | has_cache;
	if (usage)
		WRITE_ONCE(p->swap_map[offset], usage);
	else
		WRITE_ONCE(p->swap_map[offset], SWAP_HAS_CACHE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return usage;
}

/*
<<<<<<< HEAD
 * Caller has made sure that the swapdevice corresponding to entry
=======
 * When we get a swap entry, if there aren't some other ways to
 * prevent swapoff, such as the folio in swap cache is locked, page
 * table lock is held, etc., the swap entry may become invalid because
 * of swapoff.  Then, we need to enclose all swap related functions
 * with get_swap_device() and put_swap_device(), unless the swap
 * functions call get/put_swap_device() by themselves.
 *
 * Note that when only holding the PTL, swapoff might succeed immediately
 * after freeing a swap entry. Therefore, immediately after
 * __swap_entry_free(), the swap info might become stale and should not
 * be touched without a prior get_swap_device().
 *
 * Check whether swap entry is valid in the swap device.  If so,
 * return pointer to swap_info_struct, and keep the swap entry valid
 * via preventing the swap device from being swapoff, until
 * put_swap_device() is called.  Otherwise return NULL.
 *
 * Notice that swapoff or swapoff+swapon can still happen before the
 * percpu_ref_tryget_live() in get_swap_device() or after the
 * percpu_ref_put() in put_swap_device() if there isn't any other way
 * to prevent swapoff.  The caller must be prepared for that.  For
 * example, the following situation is possible.
 *
 *   CPU1				CPU2
 *   do_swap_page()
 *     ...				swapoff+swapon
 *     __read_swap_cache_async()
 *       swapcache_prepare()
 *         __swap_duplicate()
 *           // check swap_map
 *     // verify PTE not changed
 *
 * In __swap_duplicate(), the swap_map need to be checked before
 * changing partly because the specified swap entry may be for another
 * swap device which has been swapoff.  And in do_swap_page(), after
 * the page is read from the swap device, the PTE is verified not
 * changed with the page table locked to check whether the swap device
 * has been swapoff or swapoff+swapon.
 */
struct swap_info_struct *get_swap_device(swp_entry_t entry)
{
	struct swap_info_struct *si;
	unsigned long offset;

	if (!entry.val)
		goto out;
	si = swp_swap_info(entry);
	if (!si)
		goto bad_nofile;
	if (!percpu_ref_tryget_live(&si->users))
		goto out;
	/*
	 * Guarantee the si->users are checked before accessing other
	 * fields of swap_info_struct.
	 *
	 * Paired with the spin_unlock() after setup_swap_info() in
	 * enable_swap_info().
	 */
	smp_rmb();
	offset = swp_offset(entry);
	if (offset >= si->max)
		goto put_out;

	return si;
bad_nofile:
	pr_err("%s: %s%08lx\n", __func__, Bad_file, entry.val);
out:
	return NULL;
put_out:
	pr_err("%s: %s%08lx\n", __func__, Bad_offset, entry.val);
	percpu_ref_put(&si->users);
	return NULL;
}

static unsigned char __swap_entry_free(struct swap_info_struct *p,
				       swp_entry_t entry)
{
	struct swap_cluster_info *ci;
	unsigned long offset = swp_offset(entry);
	unsigned char usage;

	ci = lock_cluster_or_swap_info(p, offset);
	usage = __swap_entry_free_locked(p, offset, 1);
	unlock_cluster_or_swap_info(p, ci);
	if (!usage)
		free_swap_slot(entry);

	return usage;
}

static void swap_entry_free(struct swap_info_struct *p, swp_entry_t entry)
{
	struct swap_cluster_info *ci;
	unsigned long offset = swp_offset(entry);
	unsigned char count;

	ci = lock_cluster(p, offset);
	count = p->swap_map[offset];
	VM_BUG_ON(count != SWAP_HAS_CACHE);
	p->swap_map[offset] = 0;
	dec_cluster_info_page(p, p->cluster_info, offset);
	unlock_cluster(ci);

	mem_cgroup_uncharge_swap(entry, 1);
	swap_range_free(p, offset, 1);
}

/*
 * Caller has made sure that the swap device corresponding to entry
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * is still around or has not been recycled.
 */
void swap_free(swp_entry_t entry)
{
	struct swap_info_struct *p;

<<<<<<< HEAD
	p = swap_info_get(entry);
	if (p) {
		swap_entry_free(p, entry, 1);
		spin_unlock(&p->lock);
	}
=======
	p = _swap_info_get(entry);
	if (p)
		__swap_entry_free(p, entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Called after dropping swapcache to decrease refcnt to swap entries.
 */
<<<<<<< HEAD
void swapcache_free(swp_entry_t entry, struct page *page)
{
	struct swap_info_struct *p;
	unsigned char count;

	p = swap_info_get(entry);
	if (p) {
		count = swap_entry_free(p, entry, SWAP_HAS_CACHE);
		if (page)
			mem_cgroup_uncharge_swapcache(page, entry, count != 0);
		spin_unlock(&p->lock);
	}
}

/*
 * How many references to page are currently swapped out?
 * This does not give an exact answer when swap count is continued,
 * but does include the high COUNT_CONTINUED flag to allow for that.
 */
int page_swapcount(struct page *page)
{
	int count = 0;
	struct swap_info_struct *p;
	swp_entry_t entry;

	entry.val = page_private(page);
	p = swap_info_get(entry);
	if (p) {
		count = swap_count(p->swap_map[swp_offset(entry)]);
		spin_unlock(&p->lock);
	}
=======
void put_swap_folio(struct folio *folio, swp_entry_t entry)
{
	unsigned long offset = swp_offset(entry);
	unsigned long idx = offset / SWAPFILE_CLUSTER;
	struct swap_cluster_info *ci;
	struct swap_info_struct *si;
	unsigned char *map;
	unsigned int i, free_entries = 0;
	unsigned char val;
	int size = swap_entry_size(folio_nr_pages(folio));

	si = _swap_info_get(entry);
	if (!si)
		return;

	ci = lock_cluster_or_swap_info(si, offset);
	if (size == SWAPFILE_CLUSTER) {
		VM_BUG_ON(!cluster_is_huge(ci));
		map = si->swap_map + offset;
		for (i = 0; i < SWAPFILE_CLUSTER; i++) {
			val = map[i];
			VM_BUG_ON(!(val & SWAP_HAS_CACHE));
			if (val == SWAP_HAS_CACHE)
				free_entries++;
		}
		cluster_clear_huge(ci);
		if (free_entries == SWAPFILE_CLUSTER) {
			unlock_cluster_or_swap_info(si, ci);
			spin_lock(&si->lock);
			mem_cgroup_uncharge_swap(entry, SWAPFILE_CLUSTER);
			swap_free_cluster(si, idx);
			spin_unlock(&si->lock);
			return;
		}
	}
	for (i = 0; i < size; i++, entry.val++) {
		if (!__swap_entry_free_locked(si, offset + i, SWAP_HAS_CACHE)) {
			unlock_cluster_or_swap_info(si, ci);
			free_swap_slot(entry);
			if (i == size - 1)
				return;
			lock_cluster_or_swap_info(si, offset);
		}
	}
	unlock_cluster_or_swap_info(si, ci);
}

#ifdef CONFIG_THP_SWAP
int split_swap_cluster(swp_entry_t entry)
{
	struct swap_info_struct *si;
	struct swap_cluster_info *ci;
	unsigned long offset = swp_offset(entry);

	si = _swap_info_get(entry);
	if (!si)
		return -EBUSY;
	ci = lock_cluster(si, offset);
	cluster_clear_huge(ci);
	unlock_cluster(ci);
	return 0;
}
#endif

static int swp_entry_cmp(const void *ent1, const void *ent2)
{
	const swp_entry_t *e1 = ent1, *e2 = ent2;

	return (int)swp_type(*e1) - (int)swp_type(*e2);
}

void swapcache_free_entries(swp_entry_t *entries, int n)
{
	struct swap_info_struct *p, *prev;
	int i;

	if (n <= 0)
		return;

	prev = NULL;
	p = NULL;

	/*
	 * Sort swap entries by swap device, so each lock is only taken once.
	 * nr_swapfiles isn't absolutely correct, but the overhead of sort() is
	 * so low that it isn't necessary to optimize further.
	 */
	if (nr_swapfiles > 1)
		sort(entries, n, sizeof(entries[0]), swp_entry_cmp, NULL);
	for (i = 0; i < n; ++i) {
		p = swap_info_get_cont(entries[i], prev);
		if (p)
			swap_entry_free(p, entries[i]);
		prev = p;
	}
	if (p)
		spin_unlock(&p->lock);
}

int __swap_count(swp_entry_t entry)
{
	struct swap_info_struct *si = swp_swap_info(entry);
	pgoff_t offset = swp_offset(entry);

	return swap_count(si->swap_map[offset]);
}

/*
 * How many references to @entry are currently swapped out?
 * This does not give an exact answer when swap count is continued,
 * but does include the high COUNT_CONTINUED flag to allow for that.
 */
int swap_swapcount(struct swap_info_struct *si, swp_entry_t entry)
{
	pgoff_t offset = swp_offset(entry);
	struct swap_cluster_info *ci;
	int count;

	ci = lock_cluster_or_swap_info(si, offset);
	count = swap_count(si->swap_map[offset]);
	unlock_cluster_or_swap_info(si, ci);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return count;
}

/*
<<<<<<< HEAD
 * We can write to an anon page without COW if there are no other references
 * to it.  And as a side-effect, free up its swap: because the old content
 * on disk will never be read, and seeking back there to write new content
 * later would only waste time away from clustering.
 */
int reuse_swap_page(struct page *page)
{
	int count;

	VM_BUG_ON(!PageLocked(page));
	if (unlikely(PageKsm(page)))
		return 0;
	count = page_mapcount(page);
	if (count <= 1 && PageSwapCache(page)) {
		count += page_swapcount(page);
		if (count == 1 && !PageWriteback(page)) {
			delete_from_swap_cache(page);
			SetPageDirty(page);
		}
	}
	return count <= 1;
}

/*
 * If swap is getting full, or if there are no more mappings of this page,
 * then try_to_free_swap is called to free its swap space.
 */
int try_to_free_swap(struct page *page)
{
	VM_BUG_ON(!PageLocked(page));

	if (!PageSwapCache(page))
		return 0;
	if (PageWriteback(page))
		return 0;
	if (page_swapcount(page))
		return 0;

	/*
	 * Once hibernation has begun to create its image of memory,
	 * there's a danger that one of the calls to try_to_free_swap()
	 * - most probably a call from __try_to_reclaim_swap() while
	 * hibernation is allocating its own swap pages for the image,
	 * but conceivably even a call from memory reclaim - will free
	 * the swap from a page which has already been recorded in the
	 * image as a clean swapcache page, and then reuse its swap for
	 * another page of the image.  On waking from hibernation, the
	 * original page might be freed under memory pressure, then
	 * later read back in from swap, now with the wrong data.
	 *
	 * Hibration suspends storage while it is writing the image
	 * to disk so check that here.
	 */
	if (pm_suspended_storage())
		return 0;

	delete_from_swap_cache(page);
	SetPageDirty(page);
	return 1;
=======
 * How many references to @entry are currently swapped out?
 * This considers COUNT_CONTINUED so it returns exact answer.
 */
int swp_swapcount(swp_entry_t entry)
{
	int count, tmp_count, n;
	struct swap_info_struct *p;
	struct swap_cluster_info *ci;
	struct page *page;
	pgoff_t offset;
	unsigned char *map;

	p = _swap_info_get(entry);
	if (!p)
		return 0;

	offset = swp_offset(entry);

	ci = lock_cluster_or_swap_info(p, offset);

	count = swap_count(p->swap_map[offset]);
	if (!(count & COUNT_CONTINUED))
		goto out;

	count &= ~COUNT_CONTINUED;
	n = SWAP_MAP_MAX + 1;

	page = vmalloc_to_page(p->swap_map + offset);
	offset &= ~PAGE_MASK;
	VM_BUG_ON(page_private(page) != SWP_CONTINUED);

	do {
		page = list_next_entry(page, lru);
		map = kmap_local_page(page);
		tmp_count = map[offset];
		kunmap_local(map);

		count += (tmp_count & ~COUNT_CONTINUED) * n;
		n *= (SWAP_CONT_MAX + 1);
	} while (tmp_count & COUNT_CONTINUED);
out:
	unlock_cluster_or_swap_info(p, ci);
	return count;
}

static bool swap_page_trans_huge_swapped(struct swap_info_struct *si,
					 swp_entry_t entry)
{
	struct swap_cluster_info *ci;
	unsigned char *map = si->swap_map;
	unsigned long roffset = swp_offset(entry);
	unsigned long offset = round_down(roffset, SWAPFILE_CLUSTER);
	int i;
	bool ret = false;

	ci = lock_cluster_or_swap_info(si, offset);
	if (!ci || !cluster_is_huge(ci)) {
		if (swap_count(map[roffset]))
			ret = true;
		goto unlock_out;
	}
	for (i = 0; i < SWAPFILE_CLUSTER; i++) {
		if (swap_count(map[offset + i])) {
			ret = true;
			break;
		}
	}
unlock_out:
	unlock_cluster_or_swap_info(si, ci);
	return ret;
}

static bool folio_swapped(struct folio *folio)
{
	swp_entry_t entry = folio->swap;
	struct swap_info_struct *si = _swap_info_get(entry);

	if (!si)
		return false;

	if (!IS_ENABLED(CONFIG_THP_SWAP) || likely(!folio_test_large(folio)))
		return swap_swapcount(si, entry) != 0;

	return swap_page_trans_huge_swapped(si, entry);
}

/**
 * folio_free_swap() - Free the swap space used for this folio.
 * @folio: The folio to remove.
 *
 * If swap is getting full, or if there are no more mappings of this folio,
 * then call folio_free_swap to free its swap space.
 *
 * Return: true if we were able to release the swap space.
 */
bool folio_free_swap(struct folio *folio)
{
	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);

	if (!folio_test_swapcache(folio))
		return false;
	if (folio_test_writeback(folio))
		return false;
	if (folio_swapped(folio))
		return false;

	/*
	 * Once hibernation has begun to create its image of memory,
	 * there's a danger that one of the calls to folio_free_swap()
	 * - most probably a call from __try_to_reclaim_swap() while
	 * hibernation is allocating its own swap pages for the image,
	 * but conceivably even a call from memory reclaim - will free
	 * the swap from a folio which has already been recorded in the
	 * image as a clean swapcache folio, and then reuse its swap for
	 * another page of the image.  On waking from hibernation, the
	 * original folio might be freed under memory pressure, then
	 * later read back in from swap, now with the wrong data.
	 *
	 * Hibernation suspends storage while it is writing the image
	 * to disk so check that here.
	 */
	if (pm_suspended_storage())
		return false;

	delete_from_swap_cache(folio);
	folio_set_dirty(folio);
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Free the swap entry like above, but also try to
 * free the page cache entry if it is the last user.
 */
int free_swap_and_cache(swp_entry_t entry)
{
	struct swap_info_struct *p;
<<<<<<< HEAD
	struct page *page = NULL;
=======
	unsigned char count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (non_swap_entry(entry))
		return 1;

<<<<<<< HEAD
	p = swap_info_get(entry);
	if (p) {
		if (swap_entry_free(p, entry, 1) == SWAP_HAS_CACHE) {
			page = find_get_page(&swapper_space, entry.val);
			if (page && !trylock_page(page)) {
				page_cache_release(page);
				page = NULL;
			}
		}
		spin_unlock(&p->lock);
	}
	if (page) {
		/*
		 * Not mapped elsewhere, or swap space full? Free it!
		 * Also recheck PageSwapCache now page is locked (above).
		 */
		if (PageSwapCache(page) && !PageWriteback(page) &&
				(!page_mapped(page) ||
				vm_swap_full(page_swap_info(page)))) {
			delete_from_swap_cache(page);
			SetPageDirty(page);
		}
		unlock_page(page);
		page_cache_release(page);
=======
	p = get_swap_device(entry);
	if (p) {
		if (WARN_ON(data_race(!p->swap_map[swp_offset(entry)]))) {
			put_swap_device(p);
			return 0;
		}

		count = __swap_entry_free(p, entry);
		if (count == SWAP_HAS_CACHE &&
		    !swap_page_trans_huge_swapped(p, entry))
			__try_to_reclaim_swap(p, swp_offset(entry),
					      TTRS_UNMAPPED | TTRS_FULL);
		put_swap_device(p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return p != NULL;
}

<<<<<<< HEAD
#ifdef CONFIG_CGROUP_MEM_RES_CTLR
/**
 * mem_cgroup_count_swap_user - count the user of a swap entry
 * @ent: the swap entry to be checked
 * @pagep: the pointer for the swap cache page of the entry to be stored
 *
 * Returns the number of the user of the swap entry. The number is valid only
 * for swaps of anonymous pages.
 * If the entry is found on swap cache, the page is stored to pagep with
 * refcount of it being incremented.
 */
int mem_cgroup_count_swap_user(swp_entry_t ent, struct page **pagep)
{
	struct page *page;
	struct swap_info_struct *p;
	int count = 0;

	page = find_get_page(&swapper_space, ent.val);
	if (page)
		count += page_mapcount(page);
	p = swap_info_get(ent);
	if (p) {
		count += swap_count(p->swap_map[swp_offset(ent)]);
		spin_unlock(&swap_lock);
	}

	*pagep = page;
	return count;
}
#endif

#ifdef CONFIG_HIBERNATION
=======
#ifdef CONFIG_HIBERNATION

swp_entry_t get_swap_page_of_type(int type)
{
	struct swap_info_struct *si = swap_type_to_swap_info(type);
	swp_entry_t entry = {0};

	if (!si)
		goto fail;

	/* This is called for allocating swap entry, not cache */
	spin_lock(&si->lock);
	if ((si->flags & SWP_WRITEOK) && scan_swap_map_slots(si, 1, 1, &entry))
		atomic_long_dec(&nr_swap_pages);
	spin_unlock(&si->lock);
fail:
	return entry;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Find the swap type that corresponds to given device (if any).
 *
 * @offset - number of the PAGE_SIZE-sized block of the device, starting
 * from 0, in which the swap header is expected to be located.
 *
 * This is needed for the suspend to disk (aka swsusp).
 */
<<<<<<< HEAD
int swap_type_of(dev_t device, sector_t offset, struct block_device **bdev_p)
{
	struct block_device *bdev = NULL;
	int type;

	if (device)
		bdev = bdget(device);
=======
int swap_type_of(dev_t device, sector_t offset)
{
	int type;

	if (!device)
		return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&swap_lock);
	for (type = 0; type < nr_swapfiles; type++) {
		struct swap_info_struct *sis = swap_info[type];

		if (!(sis->flags & SWP_WRITEOK))
			continue;

<<<<<<< HEAD
		if (!bdev) {
			if (bdev_p)
				*bdev_p = bdgrab(sis->bdev);

			spin_unlock(&swap_lock);
			return type;
		}
		if (bdev == sis->bdev) {
			struct swap_extent *se = &sis->first_swap_extent;

			if (se->start_block == offset) {
				if (bdev_p)
					*bdev_p = bdgrab(sis->bdev);

				spin_unlock(&swap_lock);
				bdput(bdev);
=======
		if (device == sis->bdev->bd_dev) {
			struct swap_extent *se = first_se(sis);

			if (se->start_block == offset) {
				spin_unlock(&swap_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return type;
			}
		}
	}
	spin_unlock(&swap_lock);
<<<<<<< HEAD
	if (bdev)
		bdput(bdev);

=======
	return -ENODEV;
}

int find_first_swap(dev_t *device)
{
	int type;

	spin_lock(&swap_lock);
	for (type = 0; type < nr_swapfiles; type++) {
		struct swap_info_struct *sis = swap_info[type];

		if (!(sis->flags & SWP_WRITEOK))
			continue;
		*device = sis->bdev->bd_dev;
		spin_unlock(&swap_lock);
		return type;
	}
	spin_unlock(&swap_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -ENODEV;
}

/*
 * Get the (PAGE_SIZE) block corresponding to given offset on the swapdev
 * corresponding to given index in swap_info (swap type).
 */
sector_t swapdev_block(int type, pgoff_t offset)
{
<<<<<<< HEAD
	struct block_device *bdev;

	if ((unsigned int)type >= nr_swapfiles)
		return 0;
	if (!(swap_info[type]->flags & SWP_WRITEOK))
		return 0;
	return map_swap_entry(swp_entry(type, offset), &bdev);
=======
	struct swap_info_struct *si = swap_type_to_swap_info(type);
	struct swap_extent *se;

	if (!si || !(si->flags & SWP_WRITEOK))
		return 0;
	se = offset_to_swap_extent(si, offset);
	return se->start_block + (offset - se->start_page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Return either the total number of swap pages of given type, or the number
 * of free pages of that type (depending on @free)
 *
 * This is needed for software suspend
 */
unsigned int count_swap_pages(int type, int free)
{
	unsigned int n = 0;

	spin_lock(&swap_lock);
	if ((unsigned int)type < nr_swapfiles) {
		struct swap_info_struct *sis = swap_info[type];

		spin_lock(&sis->lock);
		if (sis->flags & SWP_WRITEOK) {
			n = sis->pages;
			if (free)
				n -= sis->inuse_pages;
		}
		spin_unlock(&sis->lock);
	}
	spin_unlock(&swap_lock);
	return n;
}
#endif /* CONFIG_HIBERNATION */

<<<<<<< HEAD
=======
static inline int pte_same_as_swp(pte_t pte, pte_t swp_pte)
{
	return pte_same(pte_swp_clear_flags(pte), swp_pte);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * No need to decide whether this PTE shares the swap entry with others,
 * just let do_wp_page work it out if a write is requested later - to
 * force COW, vm_page_prot omits write permission from any private vma.
 */
static int unuse_pte(struct vm_area_struct *vma, pmd_t *pmd,
<<<<<<< HEAD
		unsigned long addr, swp_entry_t entry, struct page *page)
{
	struct mem_cgroup *memcg;
	spinlock_t *ptl;
	pte_t *pte;
	int ret = 1;

	if (mem_cgroup_try_charge_swapin(vma->vm_mm, page,
					 GFP_KERNEL, &memcg)) {
		ret = -ENOMEM;
		goto out_nolock;
	}

	pte = pte_offset_map_lock(vma->vm_mm, pmd, addr, &ptl);
	if (unlikely(!pte_same(*pte, swp_entry_to_pte(entry)))) {
		if (ret > 0)
			mem_cgroup_cancel_charge_swapin(memcg);
=======
		unsigned long addr, swp_entry_t entry, struct folio *folio)
{
	struct page *page;
	struct folio *swapcache;
	spinlock_t *ptl;
	pte_t *pte, new_pte, old_pte;
	bool hwpoisoned = false;
	int ret = 1;

	swapcache = folio;
	folio = ksm_might_need_to_copy(folio, vma, addr);
	if (unlikely(!folio))
		return -ENOMEM;
	else if (unlikely(folio == ERR_PTR(-EHWPOISON))) {
		hwpoisoned = true;
		folio = swapcache;
	}

	page = folio_file_page(folio, swp_offset(entry));
	if (PageHWPoison(page))
		hwpoisoned = true;

	pte = pte_offset_map_lock(vma->vm_mm, pmd, addr, &ptl);
	if (unlikely(!pte || !pte_same_as_swp(ptep_get(pte),
						swp_entry_to_pte(entry)))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = 0;
		goto out;
	}

<<<<<<< HEAD
	dec_mm_counter(vma->vm_mm, MM_SWAPENTS);
	inc_mm_counter(vma->vm_mm, MM_ANONPAGES);
	get_page(page);
	set_pte_at(vma->vm_mm, addr, pte,
		   pte_mkold(mk_pte(page, vma->vm_page_prot)));
	page_add_anon_rmap(page, vma, addr);
	mem_cgroup_commit_charge_swapin(page, memcg);
	swap_free(entry);
	/*
	 * Move the page to the active list so it is not
	 * immediately swapped out again after swapon.
	 */
	activate_page(page);
out:
	pte_unmap_unlock(pte, ptl);
out_nolock:
=======
	old_pte = ptep_get(pte);

	if (unlikely(hwpoisoned || !folio_test_uptodate(folio))) {
		swp_entry_t swp_entry;

		dec_mm_counter(vma->vm_mm, MM_SWAPENTS);
		if (hwpoisoned) {
			swp_entry = make_hwpoison_entry(page);
		} else {
			swp_entry = make_poisoned_swp_entry();
		}
		new_pte = swp_entry_to_pte(swp_entry);
		ret = 0;
		goto setpte;
	}

	/*
	 * Some architectures may have to restore extra metadata to the page
	 * when reading from swap. This metadata may be indexed by swap entry
	 * so this must be called before swap_free().
	 */
	arch_swap_restore(entry, folio);

	dec_mm_counter(vma->vm_mm, MM_SWAPENTS);
	inc_mm_counter(vma->vm_mm, MM_ANONPAGES);
	folio_get(folio);
	if (folio == swapcache) {
		rmap_t rmap_flags = RMAP_NONE;

		/*
		 * See do_swap_page(): writeback would be problematic.
		 * However, we do a folio_wait_writeback() just before this
		 * call and have the folio locked.
		 */
		VM_BUG_ON_FOLIO(folio_test_writeback(folio), folio);
		if (pte_swp_exclusive(old_pte))
			rmap_flags |= RMAP_EXCLUSIVE;

		folio_add_anon_rmap_pte(folio, page, vma, addr, rmap_flags);
	} else { /* ksm created a completely new copy */
		folio_add_new_anon_rmap(folio, vma, addr);
		folio_add_lru_vma(folio, vma);
	}
	new_pte = pte_mkold(mk_pte(page, vma->vm_page_prot));
	if (pte_swp_soft_dirty(old_pte))
		new_pte = pte_mksoft_dirty(new_pte);
	if (pte_swp_uffd_wp(old_pte))
		new_pte = pte_mkuffd_wp(new_pte);
setpte:
	set_pte_at(vma->vm_mm, addr, pte, new_pte);
	swap_free(entry);
out:
	if (pte)
		pte_unmap_unlock(pte, ptl);
	if (folio != swapcache) {
		folio_unlock(folio);
		folio_put(folio);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int unuse_pte_range(struct vm_area_struct *vma, pmd_t *pmd,
<<<<<<< HEAD
				unsigned long addr, unsigned long end,
				swp_entry_t entry, struct page *page)
{
	pte_t swp_pte = swp_entry_to_pte(entry);
	pte_t *pte;
	int ret = 0;

	/*
	 * We don't actually need pte lock while scanning for swp_pte: since
	 * we hold page lock and mmap_sem, swp_pte cannot be inserted into the
	 * page table while we're scanning; though it could get zapped, and on
	 * some architectures (e.g. x86_32 with PAE) we might catch a glimpse
	 * of unmatched parts which look like swp_pte, so unuse_pte must
	 * recheck under pte lock.  Scanning without pte lock lets it be
	 * preemptible whenever CONFIG_PREEMPT but not CONFIG_HIGHPTE.
	 */
	pte = pte_offset_map(pmd, addr);
	do {
		/*
		 * swapoff spends a _lot_ of time in this loop!
		 * Test inline before going to call unuse_pte.
		 */
		if (unlikely(pte_same(*pte, swp_pte))) {
			pte_unmap(pte);
			ret = unuse_pte(vma, pmd, addr, entry, page);
			if (ret)
				goto out;
			pte = pte_offset_map(pmd, addr);
		}
	} while (pte++, addr += PAGE_SIZE, addr != end);
	pte_unmap(pte - 1);
out:
	return ret;
=======
			unsigned long addr, unsigned long end,
			unsigned int type)
{
	pte_t *pte = NULL;
	struct swap_info_struct *si;

	si = swap_info[type];
	do {
		struct folio *folio;
		unsigned long offset;
		unsigned char swp_count;
		swp_entry_t entry;
		int ret;
		pte_t ptent;

		if (!pte++) {
			pte = pte_offset_map(pmd, addr);
			if (!pte)
				break;
		}

		ptent = ptep_get_lockless(pte);

		if (!is_swap_pte(ptent))
			continue;

		entry = pte_to_swp_entry(ptent);
		if (swp_type(entry) != type)
			continue;

		offset = swp_offset(entry);
		pte_unmap(pte);
		pte = NULL;

		folio = swap_cache_get_folio(entry, vma, addr);
		if (!folio) {
			struct page *page;
			struct vm_fault vmf = {
				.vma = vma,
				.address = addr,
				.real_address = addr,
				.pmd = pmd,
			};

			page = swapin_readahead(entry, GFP_HIGHUSER_MOVABLE,
						&vmf);
			if (page)
				folio = page_folio(page);
		}
		if (!folio) {
			swp_count = READ_ONCE(si->swap_map[offset]);
			if (swp_count == 0 || swp_count == SWAP_MAP_BAD)
				continue;
			return -ENOMEM;
		}

		folio_lock(folio);
		folio_wait_writeback(folio);
		ret = unuse_pte(vma, pmd, addr, entry, folio);
		if (ret < 0) {
			folio_unlock(folio);
			folio_put(folio);
			return ret;
		}

		folio_free_swap(folio);
		folio_unlock(folio);
		folio_put(folio);
	} while (addr += PAGE_SIZE, addr != end);

	if (pte)
		pte_unmap(pte);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int unuse_pmd_range(struct vm_area_struct *vma, pud_t *pud,
				unsigned long addr, unsigned long end,
<<<<<<< HEAD
				swp_entry_t entry, struct page *page)
=======
				unsigned int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	pmd_t *pmd;
	unsigned long next;
	int ret;

	pmd = pmd_offset(pud, addr);
	do {
<<<<<<< HEAD
		next = pmd_addr_end(addr, end);
		if (pmd_none_or_trans_huge_or_clear_bad(pmd))
			continue;
		ret = unuse_pte_range(vma, pmd, addr, next, entry, page);
=======
		cond_resched();
		next = pmd_addr_end(addr, end);
		ret = unuse_pte_range(vma, pmd, addr, next, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			return ret;
	} while (pmd++, addr = next, addr != end);
	return 0;
}

<<<<<<< HEAD
static inline int unuse_pud_range(struct vm_area_struct *vma, pgd_t *pgd,
				unsigned long addr, unsigned long end,
				swp_entry_t entry, struct page *page)
=======
static inline int unuse_pud_range(struct vm_area_struct *vma, p4d_t *p4d,
				unsigned long addr, unsigned long end,
				unsigned int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	pud_t *pud;
	unsigned long next;
	int ret;

<<<<<<< HEAD
	pud = pud_offset(pgd, addr);
=======
	pud = pud_offset(p4d, addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	do {
		next = pud_addr_end(addr, end);
		if (pud_none_or_clear_bad(pud))
			continue;
<<<<<<< HEAD
		ret = unuse_pmd_range(vma, pud, addr, next, entry, page);
=======
		ret = unuse_pmd_range(vma, pud, addr, next, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			return ret;
	} while (pud++, addr = next, addr != end);
	return 0;
}

<<<<<<< HEAD
static int unuse_vma(struct vm_area_struct *vma,
				swp_entry_t entry, struct page *page)
=======
static inline int unuse_p4d_range(struct vm_area_struct *vma, pgd_t *pgd,
				unsigned long addr, unsigned long end,
				unsigned int type)
{
	p4d_t *p4d;
	unsigned long next;
	int ret;

	p4d = p4d_offset(pgd, addr);
	do {
		next = p4d_addr_end(addr, end);
		if (p4d_none_or_clear_bad(p4d))
			continue;
		ret = unuse_pud_range(vma, p4d, addr, next, type);
		if (ret)
			return ret;
	} while (p4d++, addr = next, addr != end);
	return 0;
}

static int unuse_vma(struct vm_area_struct *vma, unsigned int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	pgd_t *pgd;
	unsigned long addr, end, next;
	int ret;

<<<<<<< HEAD
	if (page_anon_vma(page)) {
		addr = page_address_in_vma(page, vma);
		if (addr == -EFAULT)
			return 0;
		else
			end = addr + PAGE_SIZE;
	} else {
		addr = vma->vm_start;
		end = vma->vm_end;
	}
=======
	addr = vma->vm_start;
	end = vma->vm_end;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pgd = pgd_offset(vma->vm_mm, addr);
	do {
		next = pgd_addr_end(addr, end);
		if (pgd_none_or_clear_bad(pgd))
			continue;
<<<<<<< HEAD
		ret = unuse_pud_range(vma, pgd, addr, next, entry, page);
=======
		ret = unuse_p4d_range(vma, pgd, addr, next, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			return ret;
	} while (pgd++, addr = next, addr != end);
	return 0;
}

<<<<<<< HEAD
static int unuse_mm(struct mm_struct *mm,
				swp_entry_t entry, struct page *page)
{
	struct vm_area_struct *vma;
	int ret = 0;

	if (!down_read_trylock(&mm->mmap_sem)) {
		/*
		 * Activate page so shrink_inactive_list is unlikely to unmap
		 * its ptes while lock is dropped, so swapoff can make progress.
		 */
		activate_page(page);
		unlock_page(page);
		down_read(&mm->mmap_sem);
		lock_page(page);
	}
	for (vma = mm->mmap; vma; vma = vma->vm_next) {
		if (vma->anon_vma && (ret = unuse_vma(vma, entry, page)))
			break;
	}
	up_read(&mm->mmap_sem);
	return (ret < 0)? ret: 0;
=======
static int unuse_mm(struct mm_struct *mm, unsigned int type)
{
	struct vm_area_struct *vma;
	int ret = 0;
	VMA_ITERATOR(vmi, mm, 0);

	mmap_read_lock(mm);
	for_each_vma(vmi, vma) {
		if (vma->anon_vma) {
			ret = unuse_vma(vma, type);
			if (ret)
				break;
		}

		cond_resched();
	}
	mmap_read_unlock(mm);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Scan swap_map from current position to next entry still in use.
<<<<<<< HEAD
 * Recycle to start on reaching the end, returning 0 when empty.
=======
 * Return 0 if there are no inuse entries after prev till end of
 * the map.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static unsigned int find_next_to_unuse(struct swap_info_struct *si,
					unsigned int prev)
{
<<<<<<< HEAD
	unsigned int max = si->max;
	unsigned int i = prev;
=======
	unsigned int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned char count;

	/*
	 * No need for swap_lock here: we're just looking
	 * for whether an entry is in use, not modifying it; false
	 * hits are okay, and sys_swapoff() has already prevented new
	 * allocations from this area (while holding swap_lock).
	 */
<<<<<<< HEAD
	for (;;) {
		if (++i >= max) {
			if (!prev) {
				i = 0;
				break;
			}
			/*
			 * No entries in use at top of swap_map,
			 * loop back to start and recheck there.
			 */
			max = prev + 1;
			prev = 0;
			i = 1;
		}
		count = si->swap_map[i];
		if (count && swap_count(count) != SWAP_MAP_BAD)
			break;
	}
	return i;
}

/*
 * We completely avoid races by reading each swap page in advance,
 * and then search for the process using it.  All the necessary
 * page table adjustments can then be made atomically.
 */
static int try_to_unuse(unsigned int type)
{
	struct swap_info_struct *si = swap_info[type];
	struct mm_struct *start_mm;
	unsigned char *swap_map;
	unsigned char swcount;
	struct page *page;
	swp_entry_t entry;
	unsigned int i = 0;
	int retval = 0;

	/*
	 * When searching mms for an entry, a good strategy is to
	 * start at the first mm we freed the previous entry from
	 * (though actually we don't notice whether we or coincidence
	 * freed the entry).  Initialize this start_mm with a hold.
	 *
	 * A simpler strategy would be to start at the last mm we
	 * freed the previous entry from; but that would take less
	 * advantage of mmlist ordering, which clusters forked mms
	 * together, child after parent.  If we race with dup_mmap(), we
	 * prefer to resolve parent before child, lest we miss entries
	 * duplicated after we scanned child: using last mm would invert
	 * that.
	 */
	start_mm = &init_mm;
	atomic_inc(&init_mm.mm_users);

	/*
	 * Keep on scanning until all entries have gone.  Usually,
	 * one pass through swap_map is enough, but not necessarily:
	 * there are races when an instance of an entry might be missed.
	 */
	while ((i = find_next_to_unuse(si, i)) != 0) {
		if (signal_pending(current)) {
			retval = -EINTR;
			break;
		}

		/*
		 * Get a page for the entry, using the existing swap
		 * cache page if there is one.  Otherwise, get a clean
		 * page and read the swap into it.
		 */
		swap_map = &si->swap_map[i];
		entry = swp_entry(type, i);
		page = read_swap_cache_async(entry,
					GFP_HIGHUSER_MOVABLE, NULL, 0);
		if (!page) {
			/*
			 * Either swap_duplicate() failed because entry
			 * has been freed independently, and will not be
			 * reused since sys_swapoff() already disabled
			 * allocation from here, or alloc_page() failed.
			 */
			if (!*swap_map)
				continue;
			retval = -ENOMEM;
			break;
		}

		/*
		 * Don't hold on to start_mm if it looks like exiting.
		 */
		if (atomic_read(&start_mm->mm_users) == 1) {
			mmput(start_mm);
			start_mm = &init_mm;
			atomic_inc(&init_mm.mm_users);
		}

		/*
		 * Wait for and lock page.  When do_swap_page races with
		 * try_to_unuse, do_swap_page can handle the fault much
		 * faster than try_to_unuse can locate the entry.  This
		 * apparently redundant "wait_on_page_locked" lets try_to_unuse
		 * defer to do_swap_page in such a case - in some tests,
		 * do_swap_page and try_to_unuse repeatedly compete.
		 */
		wait_on_page_locked(page);
		wait_on_page_writeback(page);
		lock_page(page);
		wait_on_page_writeback(page);

		/*
		 * Remove all references to entry.
		 */
		swcount = *swap_map;
		if (swap_count(swcount) == SWAP_MAP_SHMEM) {
			retval = shmem_unuse(entry, page);
			/* page has already been unlocked and released */
			if (retval < 0)
				break;
			continue;
		}
		if (swap_count(swcount) && start_mm != &init_mm)
			retval = unuse_mm(start_mm, entry, page);

		if (swap_count(*swap_map)) {
			int set_start_mm = (*swap_map >= swcount);
			struct list_head *p = &start_mm->mmlist;
			struct mm_struct *new_start_mm = start_mm;
			struct mm_struct *prev_mm = start_mm;
			struct mm_struct *mm;

			atomic_inc(&new_start_mm->mm_users);
			atomic_inc(&prev_mm->mm_users);
			spin_lock(&mmlist_lock);
			while (swap_count(*swap_map) && !retval &&
					(p = p->next) != &start_mm->mmlist) {
				mm = list_entry(p, struct mm_struct, mmlist);
				if (!atomic_inc_not_zero(&mm->mm_users))
					continue;
				spin_unlock(&mmlist_lock);
				mmput(prev_mm);
				prev_mm = mm;

				cond_resched();

				swcount = *swap_map;
				if (!swap_count(swcount)) /* any usage ? */
					;
				else if (mm == &init_mm)
					set_start_mm = 1;
				else
					retval = unuse_mm(mm, entry, page);

				if (set_start_mm && *swap_map < swcount) {
					mmput(new_start_mm);
					atomic_inc(&mm->mm_users);
					new_start_mm = mm;
					set_start_mm = 0;
				}
				spin_lock(&mmlist_lock);
			}
			spin_unlock(&mmlist_lock);
			mmput(prev_mm);
			mmput(start_mm);
			start_mm = new_start_mm;
		}
		if (retval) {
			unlock_page(page);
			page_cache_release(page);
			break;
		}

		/*
		 * If a reference remains (rare), we would like to leave
		 * the page in the swap cache; but try_to_unmap could
		 * then re-duplicate the entry once we drop page lock,
		 * so we might loop indefinitely; also, that page could
		 * not be swapped out to other storage meanwhile.  So:
		 * delete from cache even if there's another reference,
		 * after ensuring that the data has been saved to disk -
		 * since if the reference remains (rarer), it will be
		 * read from disk into another page.  Splitting into two
		 * pages would be incorrect if swap supported "shared
		 * private" pages, but they are handled by tmpfs files.
		 *
		 * Given how unuse_vma() targets one particular offset
		 * in an anon_vma, once the anon_vma has been determined,
		 * this splitting happens to be just what is needed to
		 * handle where KSM pages have been swapped out: re-reading
		 * is unnecessarily slow, but we can fix that later on.
		 */
		if (swap_count(*swap_map) &&
		     PageDirty(page) && PageSwapCache(page)) {
			struct writeback_control wbc = {
				.sync_mode = WB_SYNC_NONE,
			};

			swap_writepage(page, &wbc);
			lock_page(page);
			wait_on_page_writeback(page);
		}

		/*
		 * It is conceivable that a racing task removed this page from
		 * swap cache just before we acquired the page lock at the top,
		 * or while we dropped it in unuse_mm().  The page might even
		 * be back in swap cache on another swap area: that we must not
		 * delete, since it may not have been written out to swap yet.
		 */
		if (PageSwapCache(page) &&
		    likely(page_private(page) == entry.val))
			delete_from_swap_cache(page);

		/*
		 * So we could skip searching mms once swap count went
		 * to 1, we did not mark any present ptes as dirty: must
		 * mark page dirty so shrink_page_list will preserve it.
		 */
		SetPageDirty(page);
		unlock_page(page);
		page_cache_release(page);

		/*
=======
	for (i = prev + 1; i < si->max; i++) {
		count = READ_ONCE(si->swap_map[i]);
		if (count && swap_count(count) != SWAP_MAP_BAD)
			break;
		if ((i % LATENCY_LIMIT) == 0)
			cond_resched();
	}

	if (i == si->max)
		i = 0;

	return i;
}

static int try_to_unuse(unsigned int type)
{
	struct mm_struct *prev_mm;
	struct mm_struct *mm;
	struct list_head *p;
	int retval = 0;
	struct swap_info_struct *si = swap_info[type];
	struct folio *folio;
	swp_entry_t entry;
	unsigned int i;

	if (!READ_ONCE(si->inuse_pages))
		goto success;

retry:
	retval = shmem_unuse(type);
	if (retval)
		return retval;

	prev_mm = &init_mm;
	mmget(prev_mm);

	spin_lock(&mmlist_lock);
	p = &init_mm.mmlist;
	while (READ_ONCE(si->inuse_pages) &&
	       !signal_pending(current) &&
	       (p = p->next) != &init_mm.mmlist) {

		mm = list_entry(p, struct mm_struct, mmlist);
		if (!mmget_not_zero(mm))
			continue;
		spin_unlock(&mmlist_lock);
		mmput(prev_mm);
		prev_mm = mm;
		retval = unuse_mm(mm, type);
		if (retval) {
			mmput(prev_mm);
			return retval;
		}

		/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * Make sure that we aren't completely killing
		 * interactive performance.
		 */
		cond_resched();
<<<<<<< HEAD
	}

	mmput(start_mm);
	return retval;
=======
		spin_lock(&mmlist_lock);
	}
	spin_unlock(&mmlist_lock);

	mmput(prev_mm);

	i = 0;
	while (READ_ONCE(si->inuse_pages) &&
	       !signal_pending(current) &&
	       (i = find_next_to_unuse(si, i)) != 0) {

		entry = swp_entry(type, i);
		folio = filemap_get_folio(swap_address_space(entry), i);
		if (IS_ERR(folio))
			continue;

		/*
		 * It is conceivable that a racing task removed this folio from
		 * swap cache just before we acquired the page lock. The folio
		 * might even be back in swap cache on another swap area. But
		 * that is okay, folio_free_swap() only removes stale folios.
		 */
		folio_lock(folio);
		folio_wait_writeback(folio);
		folio_free_swap(folio);
		folio_unlock(folio);
		folio_put(folio);
	}

	/*
	 * Lets check again to see if there are still swap entries in the map.
	 * If yes, we would need to do retry the unuse logic again.
	 * Under global memory pressure, swap entries can be reinserted back
	 * into process space after the mmlist loop above passes over them.
	 *
	 * Limit the number of retries? No: when mmget_not_zero()
	 * above fails, that mm is likely to be freeing swap from
	 * exit_mmap(), which proceeds at its own independent pace;
	 * and even shmem_writepage() could have been preempted after
	 * folio_alloc_swap(), temporarily hiding that swap.  It's easy
	 * and robust (though cpu-intensive) just to keep retrying.
	 */
	if (READ_ONCE(si->inuse_pages)) {
		if (!signal_pending(current))
			goto retry;
		return -EINTR;
	}

success:
	/*
	 * Make sure that further cleanups after try_to_unuse() returns happen
	 * after swap_range_free() reduces si->inuse_pages to 0.
	 */
	smp_mb();
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * After a successful try_to_unuse, if no swap is now in use, we know
 * we can empty the mmlist.  swap_lock must be held on entry and exit.
 * Note that mmlist_lock nests inside swap_lock, and an mm must be
 * added to the mmlist just after page_duplicate - before would be racy.
 */
static void drain_mmlist(void)
{
	struct list_head *p, *next;
	unsigned int type;

	for (type = 0; type < nr_swapfiles; type++)
		if (swap_info[type]->inuse_pages)
			return;
	spin_lock(&mmlist_lock);
	list_for_each_safe(p, next, &init_mm.mmlist)
		list_del_init(p);
	spin_unlock(&mmlist_lock);
}

/*
<<<<<<< HEAD
 * Use this swapdev's extent info to locate the (PAGE_SIZE) block which
 * corresponds to page offset for the specified swap entry.
 * Note that the type of this function is sector_t, but it returns page offset
 * into the bdev, not sector offset.
 */
static sector_t map_swap_entry(swp_entry_t entry, struct block_device **bdev)
{
	struct swap_info_struct *sis;
	struct swap_extent *start_se;
	struct swap_extent *se;
	pgoff_t offset;

	sis = swap_info[swp_type(entry)];
	*bdev = sis->bdev;

	offset = swp_offset(entry);
	start_se = sis->curr_swap_extent;
	se = start_se;

	for ( ; ; ) {
		struct list_head *lh;

		if (se->start_page <= offset &&
				offset < (se->start_page + se->nr_pages)) {
			return se->start_block + (offset - se->start_page);
		}
		lh = se->list.next;
		se = list_entry(lh, struct swap_extent, list);
		sis->curr_swap_extent = se;
		BUG_ON(se == start_se);		/* It *must* be present */
	}
}

/*
 * Returns the page offset into bdev for the specified page's swap entry.
 */
sector_t map_swap_page(struct page *page, struct block_device **bdev)
{
	swp_entry_t entry;
	entry.val = page_private(page);
	return map_swap_entry(entry, bdev);
}

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Free all of a swapdev's extent information
 */
static void destroy_swap_extents(struct swap_info_struct *sis)
{
<<<<<<< HEAD
	while (!list_empty(&sis->first_swap_extent.list)) {
		struct swap_extent *se;

		se = list_entry(sis->first_swap_extent.list.next,
				struct swap_extent, list);
		list_del(&se->list);
		kfree(se);
	}
=======
	while (!RB_EMPTY_ROOT(&sis->swap_extent_root)) {
		struct rb_node *rb = sis->swap_extent_root.rb_node;
		struct swap_extent *se = rb_entry(rb, struct swap_extent, rb_node);

		rb_erase(rb, &sis->swap_extent_root);
		kfree(se);
	}

	if (sis->flags & SWP_ACTIVATED) {
		struct file *swap_file = sis->swap_file;
		struct address_space *mapping = swap_file->f_mapping;

		sis->flags &= ~SWP_ACTIVATED;
		if (mapping->a_ops->swap_deactivate)
			mapping->a_ops->swap_deactivate(swap_file);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Add a block range (and the corresponding page range) into this swapdev's
<<<<<<< HEAD
 * extent list.  The extent list is kept sorted in page order.
 *
 * This function rather assumes that it is called in ascending page order.
 */
static int
add_swap_extent(struct swap_info_struct *sis, unsigned long start_page,
		unsigned long nr_pages, sector_t start_block)
{
	struct swap_extent *se;
	struct swap_extent *new_se;
	struct list_head *lh;

	if (start_page == 0) {
		se = &sis->first_swap_extent;
		sis->curr_swap_extent = se;
		se->start_page = 0;
		se->nr_pages = nr_pages;
		se->start_block = start_block;
		return 1;
	} else {
		lh = sis->first_swap_extent.list.prev;	/* Highest extent */
		se = list_entry(lh, struct swap_extent, list);
=======
 * extent tree.
 *
 * This function rather assumes that it is called in ascending page order.
 */
int
add_swap_extent(struct swap_info_struct *sis, unsigned long start_page,
		unsigned long nr_pages, sector_t start_block)
{
	struct rb_node **link = &sis->swap_extent_root.rb_node, *parent = NULL;
	struct swap_extent *se;
	struct swap_extent *new_se;

	/*
	 * place the new node at the right most since the
	 * function is called in ascending page order.
	 */
	while (*link) {
		parent = *link;
		link = &parent->rb_right;
	}

	if (parent) {
		se = rb_entry(parent, struct swap_extent, rb_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		BUG_ON(se->start_page + se->nr_pages != start_page);
		if (se->start_block + se->nr_pages == start_block) {
			/* Merge it */
			se->nr_pages += nr_pages;
			return 0;
		}
	}

<<<<<<< HEAD
	/*
	 * No merge.  Insert a new extent, preserving ordering.
	 */
=======
	/* No merge, insert a new extent. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	new_se = kmalloc(sizeof(*se), GFP_KERNEL);
	if (new_se == NULL)
		return -ENOMEM;
	new_se->start_page = start_page;
	new_se->nr_pages = nr_pages;
	new_se->start_block = start_block;

<<<<<<< HEAD
	list_add_tail(&new_se->list, &sis->first_swap_extent.list);
	return 1;
}

/*
 * A `swap extent' is a simple thing which maps a contiguous range of pages
 * onto a contiguous range of disk blocks.  An ordered list of swap extents
 * is built at swapon time and is then used at swap_writepage/swap_readpage
=======
	rb_link_node(&new_se->rb_node, parent, link);
	rb_insert_color(&new_se->rb_node, &sis->swap_extent_root);
	return 1;
}
EXPORT_SYMBOL_GPL(add_swap_extent);

/*
 * A `swap extent' is a simple thing which maps a contiguous range of pages
 * onto a contiguous range of disk blocks.  A rbtree of swap extents is
 * built at swapon time and is then used at swap_writepage/swap_read_folio
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * time for locating where on disk a page belongs.
 *
 * If the swapfile is an S_ISBLK block device, a single extent is installed.
 * This is done so that the main operating code can treat S_ISBLK and S_ISREG
 * swap files identically.
 *
 * Whether the swapdev is an S_ISREG file or an S_ISBLK blockdev, the swap
<<<<<<< HEAD
 * extent list operates in PAGE_SIZE disk blocks.  Both S_ISREG and S_ISBLK
 * swapfiles are handled *identically* after swapon time.
 *
 * For S_ISREG swapfiles, setup_swap_extents() will walk all the file's blocks
 * and will parse them into an ordered extent list, in PAGE_SIZE chunks.  If
 * some stray blocks are found which do not fall within the PAGE_SIZE alignment
 * requirements, they are simply tossed out - we will never use those blocks
 * for swapping.
 *
 * For S_ISREG swapfiles we set S_SWAPFILE across the life of the swapon.  This
 * prevents root from shooting her foot off by ftruncating an in-use swapfile,
 * which will scribble on the fs.
 *
 * The amount of disk space which a single swap extent represents varies.
 * Typically it is in the 1-4 megabyte range.  So we can have hundreds of
 * extents in the list.  To avoid much list walking, we cache the previous
 * search location in `curr_swap_extent', and start new searches from there.
 * This is extremely effective.  The average number of iterations in
 * map_swap_page() has been measured at about 0.3 per page.  - akpm.
 */
static int setup_swap_extents(struct swap_info_struct *sis, sector_t *span)
{
	struct inode *inode;
	unsigned blocks_per_page;
	unsigned long page_no;
	unsigned blkbits;
	sector_t probe_block;
	sector_t last_block;
	sector_t lowest_block = -1;
	sector_t highest_block = 0;
	int nr_extents = 0;
	int ret;

	inode = sis->swap_file->f_mapping->host;
	if (S_ISBLK(inode->i_mode)) {
		ret = add_swap_extent(sis, 0, sis->max, 0);
		*span = sis->pages;
		goto out;
	}

	blkbits = inode->i_blkbits;
	blocks_per_page = PAGE_SIZE >> blkbits;

	/*
	 * Map all the blocks into the extent list.  This code doesn't try
	 * to be very smart.
	 */
	probe_block = 0;
	page_no = 0;
	last_block = i_size_read(inode) >> blkbits;
	while ((probe_block + blocks_per_page) <= last_block &&
			page_no < sis->max) {
		unsigned block_in_page;
		sector_t first_block;

		first_block = bmap(inode, probe_block);
		if (first_block == 0)
			goto bad_bmap;

		/*
		 * It must be PAGE_SIZE aligned on-disk
		 */
		if (first_block & (blocks_per_page - 1)) {
			probe_block++;
			goto reprobe;
		}

		for (block_in_page = 1; block_in_page < blocks_per_page;
					block_in_page++) {
			sector_t block;

			block = bmap(inode, probe_block + block_in_page);
			if (block == 0)
				goto bad_bmap;
			if (block != first_block + block_in_page) {
				/* Discontiguity */
				probe_block++;
				goto reprobe;
			}
		}

		first_block >>= (PAGE_SHIFT - blkbits);
		if (page_no) {	/* exclude the header page */
			if (first_block < lowest_block)
				lowest_block = first_block;
			if (first_block > highest_block)
				highest_block = first_block;
		}

		/*
		 * We found a PAGE_SIZE-length, PAGE_SIZE-aligned run of blocks
		 */
		ret = add_swap_extent(sis, page_no, 1, first_block);
		if (ret < 0)
			goto out;
		nr_extents += ret;
		page_no++;
		probe_block += blocks_per_page;
reprobe:
		continue;
	}
	ret = nr_extents;
	*span = 1 + highest_block - lowest_block;
	if (page_no == 0)
		page_no = 1;	/* force Empty message */
	sis->max = page_no;
	sis->pages = page_no - 1;
	sis->highest_bit = page_no - 1;
out:
	return ret;
bad_bmap:
	printk(KERN_ERR "swapon: swapfile has holes\n");
	ret = -EINVAL;
	goto out;
}

static void enable_swap_info(struct swap_info_struct *p, int prio,
				unsigned char *swap_map)
{
	int i, prev;

	spin_lock(&swap_lock);
=======
 * extent rbtree operates in PAGE_SIZE disk blocks.  Both S_ISREG and S_ISBLK
 * swapfiles are handled *identically* after swapon time.
 *
 * For S_ISREG swapfiles, setup_swap_extents() will walk all the file's blocks
 * and will parse them into a rbtree, in PAGE_SIZE chunks.  If some stray
 * blocks are found which do not fall within the PAGE_SIZE alignment
 * requirements, they are simply tossed out - we will never use those blocks
 * for swapping.
 *
 * For all swap devices we set S_SWAPFILE across the life of the swapon.  This
 * prevents users from writing to the swap device, which will corrupt memory.
 *
 * The amount of disk space which a single swap extent represents varies.
 * Typically it is in the 1-4 megabyte range.  So we can have hundreds of
 * extents in the rbtree. - akpm.
 */
static int setup_swap_extents(struct swap_info_struct *sis, sector_t *span)
{
	struct file *swap_file = sis->swap_file;
	struct address_space *mapping = swap_file->f_mapping;
	struct inode *inode = mapping->host;
	int ret;

	if (S_ISBLK(inode->i_mode)) {
		ret = add_swap_extent(sis, 0, sis->max, 0);
		*span = sis->pages;
		return ret;
	}

	if (mapping->a_ops->swap_activate) {
		ret = mapping->a_ops->swap_activate(sis, swap_file, span);
		if (ret < 0)
			return ret;
		sis->flags |= SWP_ACTIVATED;
		if ((sis->flags & SWP_FS_OPS) &&
		    sio_pool_init() != 0) {
			destroy_swap_extents(sis);
			return -ENOMEM;
		}
		return ret;
	}

	return generic_swapfile_activate(sis, swap_file, span);
}

static int swap_node(struct swap_info_struct *p)
{
	struct block_device *bdev;

	if (p->bdev)
		bdev = p->bdev;
	else
		bdev = p->swap_file->f_inode->i_sb->s_bdev;

	return bdev ? bdev->bd_disk->node_id : NUMA_NO_NODE;
}

static void setup_swap_info(struct swap_info_struct *p, int prio,
			    unsigned char *swap_map,
			    struct swap_cluster_info *cluster_info)
{
	int i;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (prio >= 0)
		p->prio = prio;
	else
		p->prio = --least_priority;
<<<<<<< HEAD
	p->swap_map = swap_map;
=======
	/*
	 * the plist prio is negated because plist ordering is
	 * low-to-high, while swap ordering is high-to-low
	 */
	p->list.prio = -p->prio;
	for_each_node(i) {
		if (p->prio >= 0)
			p->avail_lists[i].prio = -p->prio;
		else {
			if (swap_node(p) == i)
				p->avail_lists[i].prio = 1;
			else
				p->avail_lists[i].prio = -p->prio;
		}
	}
	p->swap_map = swap_map;
	p->cluster_info = cluster_info;
}

static void _enable_swap_info(struct swap_info_struct *p)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p->flags |= SWP_WRITEOK;
	atomic_long_add(p->pages, &nr_swap_pages);
	total_swap_pages += p->pages;

<<<<<<< HEAD
	/* insert swap space into swap_list: */
	prev = -1;
	for (i = swap_list.head; i >= 0; i = swap_info[i]->next) {
		if (p->prio >= swap_info[i]->prio)
			break;
		prev = i;
	}
	p->next = i;
	if (prev < 0)
		swap_list.head = swap_list.next = p->type;
	else
		swap_info[prev]->next = p->type;
	spin_unlock(&swap_lock);
=======
	assert_spin_locked(&swap_lock);
	/*
	 * both lists are plists, and thus priority ordered.
	 * swap_active_head needs to be priority ordered for swapoff(),
	 * which on removal of any swap_info_struct with an auto-assigned
	 * (i.e. negative) priority increments the auto-assigned priority
	 * of any lower-priority swap_info_structs.
	 * swap_avail_head needs to be priority ordered for folio_alloc_swap(),
	 * which allocates swap pages from the highest available priority
	 * swap_info_struct.
	 */
	plist_add(&p->list, &swap_active_head);

	/* add to available list iff swap device is not full */
	if (p->highest_bit)
		add_to_avail_list(p);
}

static void enable_swap_info(struct swap_info_struct *p, int prio,
				unsigned char *swap_map,
				struct swap_cluster_info *cluster_info)
{
	spin_lock(&swap_lock);
	spin_lock(&p->lock);
	setup_swap_info(p, prio, swap_map, cluster_info);
	spin_unlock(&p->lock);
	spin_unlock(&swap_lock);
	/*
	 * Finished initializing swap device, now it's safe to reference it.
	 */
	percpu_ref_resurrect(&p->users);
	spin_lock(&swap_lock);
	spin_lock(&p->lock);
	_enable_swap_info(p);
	spin_unlock(&p->lock);
	spin_unlock(&swap_lock);
}

static void reinsert_swap_info(struct swap_info_struct *p)
{
	spin_lock(&swap_lock);
	spin_lock(&p->lock);
	setup_swap_info(p, p->prio, p->swap_map, p->cluster_info);
	_enable_swap_info(p);
	spin_unlock(&p->lock);
	spin_unlock(&swap_lock);
}

bool has_usable_swap(void)
{
	bool ret = true;

	spin_lock(&swap_lock);
	if (plist_head_empty(&swap_active_head))
		ret = false;
	spin_unlock(&swap_lock);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE1(swapoff, const char __user *, specialfile)
{
	struct swap_info_struct *p = NULL;
	unsigned char *swap_map;
<<<<<<< HEAD
=======
	struct swap_cluster_info *cluster_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct file *swap_file, *victim;
	struct address_space *mapping;
	struct inode *inode;
	struct filename *pathname;
<<<<<<< HEAD
	int oom_score_adj;
	int i, type, prev;
	int err;
=======
	int err, found = 0;
	unsigned int old_block_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	BUG_ON(!current->mm);

	pathname = getname(specialfile);
<<<<<<< HEAD
	err = PTR_ERR(pathname);
	if (IS_ERR(pathname))
		goto out;
=======
	if (IS_ERR(pathname))
		return PTR_ERR(pathname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	victim = file_open_name(pathname, O_RDWR|O_LARGEFILE, 0);
	err = PTR_ERR(victim);
	if (IS_ERR(victim))
		goto out;

	mapping = victim->f_mapping;
<<<<<<< HEAD
	prev = -1;
	spin_lock(&swap_lock);
	for (type = swap_list.head; type >= 0; type = swap_info[type]->next) {
		p = swap_info[type];
		if (p->flags & SWP_WRITEOK) {
			if (p->swap_file->f_mapping == mapping)
				break;
		}
		prev = type;
	}
	if (type < 0) {
=======
	spin_lock(&swap_lock);
	plist_for_each_entry(p, &swap_active_head, list) {
		if (p->flags & SWP_WRITEOK) {
			if (p->swap_file->f_mapping == mapping) {
				found = 1;
				break;
			}
		}
	}
	if (!found) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EINVAL;
		spin_unlock(&swap_lock);
		goto out_dput;
	}
	if (!security_vm_enough_memory_mm(current->mm, p->pages))
		vm_unacct_memory(p->pages);
	else {
		err = -ENOMEM;
		spin_unlock(&swap_lock);
		goto out_dput;
	}
<<<<<<< HEAD
	if (prev < 0)
		swap_list.head = p->next;
	else
		swap_info[prev]->next = p->next;
	if (type == swap_list.next) {
		/* just pick something that's safe... */
		swap_list.next = swap_list.head;
	}
	spin_lock(&p->lock);
	if (p->prio < 0) {
		for (i = p->next; i >= 0; i = swap_info[i]->next)
			swap_info[i]->prio = p->prio--;
		least_priority++;
	}
=======
	spin_lock(&p->lock);
	del_from_avail_list(p);
	if (p->prio < 0) {
		struct swap_info_struct *si = p;
		int nid;

		plist_for_each_entry_continue(si, &swap_active_head, list) {
			si->prio++;
			si->list.prio--;
			for_each_node(nid) {
				if (si->avail_lists[nid].prio != 1)
					si->avail_lists[nid].prio--;
			}
		}
		least_priority++;
	}
	plist_del(&p->list, &swap_active_head);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_long_sub(p->pages, &nr_swap_pages);
	total_swap_pages -= p->pages;
	p->flags &= ~SWP_WRITEOK;
	spin_unlock(&p->lock);
	spin_unlock(&swap_lock);

<<<<<<< HEAD
	oom_score_adj = test_set_oom_score_adj(OOM_SCORE_ADJ_MAX);
	err = try_to_unuse(type);
	compare_swap_oom_score_adj(OOM_SCORE_ADJ_MAX, oom_score_adj);

	if (err) {
		/*
		 * reading p->prio and p->swap_map outside the lock is
		 * safe here because only sys_swapon and sys_swapoff
		 * change them, and there can be no other sys_swapon or
		 * sys_swapoff for this swap_info_struct at this point.
		 */
		/* re-insert swap space back into swap_list */
		enable_swap_info(p, p->prio, p->swap_map);
		goto out_dput;
	}

=======
	disable_swap_slots_cache_lock();

	set_current_oom_origin();
	err = try_to_unuse(p->type);
	clear_current_oom_origin();

	if (err) {
		/* re-insert swap space back into swap_list */
		reinsert_swap_info(p);
		reenable_swap_slots_cache_unlock();
		goto out_dput;
	}

	reenable_swap_slots_cache_unlock();

	/*
	 * Wait for swap operations protected by get/put_swap_device()
	 * to complete.
	 *
	 * We need synchronize_rcu() here to protect the accessing to
	 * the swap cache data structure.
	 */
	percpu_ref_kill(&p->users);
	synchronize_rcu();
	wait_for_completion(&p->comp);

	flush_work(&p->discard_work);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	destroy_swap_extents(p);
	if (p->flags & SWP_CONTINUED)
		free_swap_count_continuations(p);

<<<<<<< HEAD
=======
	if (!p->bdev || !bdev_nonrot(p->bdev))
		atomic_dec(&nr_rotate_swap);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&swapon_mutex);
	spin_lock(&swap_lock);
	spin_lock(&p->lock);
	drain_mmlist();

<<<<<<< HEAD
	/* wait for anyone still in scan_swap_map */
=======
	/* wait for anyone still in scan_swap_map_slots */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p->highest_bit = 0;		/* cuts scans short */
	while (p->flags >= SWP_SCANNING) {
		spin_unlock(&p->lock);
		spin_unlock(&swap_lock);
		schedule_timeout_uninterruptible(1);
		spin_lock(&swap_lock);
		spin_lock(&p->lock);
	}

	swap_file = p->swap_file;
<<<<<<< HEAD
=======
	old_block_size = p->old_block_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p->swap_file = NULL;
	p->max = 0;
	swap_map = p->swap_map;
	p->swap_map = NULL;
<<<<<<< HEAD
	p->flags = 0;
	spin_unlock(&p->lock);
	spin_unlock(&swap_lock);
	mutex_unlock(&swapon_mutex);
	vfree(swap_map);
	/* Destroy swap account informatin */
	swap_cgroup_swapoff(type);

	inode = mapping->host;
	if (S_ISBLK(inode->i_mode)) {
		struct block_device *bdev = I_BDEV(inode);
		set_blocksize(bdev, p->old_block_size);
		blkdev_put(bdev, FMODE_READ | FMODE_WRITE | FMODE_EXCL);
	} else {
		mutex_lock(&inode->i_mutex);
		inode->i_flags &= ~S_SWAPFILE;
		mutex_unlock(&inode->i_mutex);
	}
	filp_close(swap_file, NULL);
=======
	cluster_info = p->cluster_info;
	p->cluster_info = NULL;
	spin_unlock(&p->lock);
	spin_unlock(&swap_lock);
	arch_swap_invalidate_area(p->type);
	zswap_swapoff(p->type);
	mutex_unlock(&swapon_mutex);
	free_percpu(p->percpu_cluster);
	p->percpu_cluster = NULL;
	free_percpu(p->cluster_next_cpu);
	p->cluster_next_cpu = NULL;
	vfree(swap_map);
	kvfree(cluster_info);
	/* Destroy swap account information */
	swap_cgroup_swapoff(p->type);
	exit_swap_address_space(p->type);

	inode = mapping->host;
	if (p->bdev_file) {
		set_blocksize(p->bdev, old_block_size);
		fput(p->bdev_file);
		p->bdev_file = NULL;
	}

	inode_lock(inode);
	inode->i_flags &= ~S_SWAPFILE;
	inode_unlock(inode);
	filp_close(swap_file, NULL);

	/*
	 * Clear the SWP_USED flag after all resources are freed so that swapon
	 * can reuse this swap_info in alloc_swap_info() safely.  It is ok to
	 * not hold p->lock after we cleared its SWP_WRITEOK.
	 */
	spin_lock(&swap_lock);
	p->flags = 0;
	spin_unlock(&swap_lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = 0;
	atomic_inc(&proc_poll_event);
	wake_up_interruptible(&proc_poll_wait);

out_dput:
	filp_close(victim, NULL);
out:
<<<<<<< HEAD
=======
	putname(pathname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

#ifdef CONFIG_PROC_FS
<<<<<<< HEAD
static unsigned swaps_poll(struct file *file, poll_table *wait)
=======
static __poll_t swaps_poll(struct file *file, poll_table *wait)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct seq_file *seq = file->private_data;

	poll_wait(file, &proc_poll_wait, wait);

	if (seq->poll_event != atomic_read(&proc_poll_event)) {
		seq->poll_event = atomic_read(&proc_poll_event);
<<<<<<< HEAD
		return POLLIN | POLLRDNORM | POLLERR | POLLPRI;
	}

	return POLLIN | POLLRDNORM;
=======
		return EPOLLIN | EPOLLRDNORM | EPOLLERR | EPOLLPRI;
	}

	return EPOLLIN | EPOLLRDNORM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* iterator */
static void *swap_start(struct seq_file *swap, loff_t *pos)
{
	struct swap_info_struct *si;
	int type;
	loff_t l = *pos;

	mutex_lock(&swapon_mutex);

	if (!l)
		return SEQ_START_TOKEN;

<<<<<<< HEAD
	for (type = 0; type < nr_swapfiles; type++) {
		smp_rmb();	/* read nr_swapfiles before swap_info[type] */
		si = swap_info[type];
=======
	for (type = 0; (si = swap_type_to_swap_info(type)); type++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!(si->flags & SWP_USED) || !si->swap_map)
			continue;
		if (!--l)
			return si;
	}

	return NULL;
}

static void *swap_next(struct seq_file *swap, void *v, loff_t *pos)
{
	struct swap_info_struct *si = v;
	int type;

	if (v == SEQ_START_TOKEN)
		type = 0;
	else
		type = si->type + 1;

<<<<<<< HEAD
	for (; type < nr_swapfiles; type++) {
		smp_rmb();	/* read nr_swapfiles before swap_info[type] */
		si = swap_info[type];
		if (!(si->flags & SWP_USED) || !si->swap_map)
			continue;
		++*pos;
=======
	++(*pos);
	for (; (si = swap_type_to_swap_info(type)); type++) {
		if (!(si->flags & SWP_USED) || !si->swap_map)
			continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return si;
	}

	return NULL;
}

static void swap_stop(struct seq_file *swap, void *v)
{
	mutex_unlock(&swapon_mutex);
}

static int swap_show(struct seq_file *swap, void *v)
{
	struct swap_info_struct *si = v;
	struct file *file;
	int len;
<<<<<<< HEAD

	if (si == SEQ_START_TOKEN) {
		seq_puts(swap,"Filename\t\t\t\tType\t\tSize\tUsed\tPriority\n");
		return 0;
	}

	file = si->swap_file;
	len = seq_path(swap, &file->f_path, " \t\n\\");
	seq_printf(swap, "%*s%s\t%u\t%u\t%d\n",
			len < 40 ? 40 - len : 1, " ",
			S_ISBLK(file->f_path.dentry->d_inode->i_mode) ?
				"partition" : "file\t",
			si->pages << (PAGE_SHIFT - 10),
			si->inuse_pages << (PAGE_SHIFT - 10),
=======
	unsigned long bytes, inuse;

	if (si == SEQ_START_TOKEN) {
		seq_puts(swap, "Filename\t\t\t\tType\t\tSize\t\tUsed\t\tPriority\n");
		return 0;
	}

	bytes = K(si->pages);
	inuse = K(READ_ONCE(si->inuse_pages));

	file = si->swap_file;
	len = seq_file_path(swap, file, " \t\n\\");
	seq_printf(swap, "%*s%s\t%lu\t%s%lu\t%s%d\n",
			len < 40 ? 40 - len : 1, " ",
			S_ISBLK(file_inode(file)->i_mode) ?
				"partition" : "file\t",
			bytes, bytes < 10000000 ? "\t" : "",
			inuse, inuse < 10000000 ? "\t" : "",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			si->prio);
	return 0;
}

static const struct seq_operations swaps_op = {
	.start =	swap_start,
	.next =		swap_next,
	.stop =		swap_stop,
	.show =		swap_show
};

static int swaps_open(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	int ret;

	ret = seq_open(file, &swaps_op);
	if (ret)
		return ret;

	seq = file->private_data;
	seq->poll_event = atomic_read(&proc_poll_event);
	return 0;
}

<<<<<<< HEAD
static const struct file_operations proc_swaps_operations = {
	.open		= swaps_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
	.poll		= swaps_poll,
=======
static const struct proc_ops swaps_proc_ops = {
	.proc_flags	= PROC_ENTRY_PERMANENT,
	.proc_open	= swaps_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= seq_release,
	.proc_poll	= swaps_poll,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init procswaps_init(void)
{
<<<<<<< HEAD
	proc_create("swaps", 0, NULL, &proc_swaps_operations);
=======
	proc_create("swaps", 0, NULL, &swaps_proc_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
__initcall(procswaps_init);
#endif /* CONFIG_PROC_FS */

#ifdef MAX_SWAPFILES_CHECK
static int __init max_swapfiles_check(void)
{
	MAX_SWAPFILES_CHECK();
	return 0;
}
late_initcall(max_swapfiles_check);
#endif

static struct swap_info_struct *alloc_swap_info(void)
{
	struct swap_info_struct *p;
<<<<<<< HEAD
	unsigned int type;

	p = kzalloc(sizeof(*p), GFP_KERNEL);
	if (!p)
		return ERR_PTR(-ENOMEM);

=======
	struct swap_info_struct *defer = NULL;
	unsigned int type;
	int i;

	p = kvzalloc(struct_size(p, avail_lists, nr_node_ids), GFP_KERNEL);
	if (!p)
		return ERR_PTR(-ENOMEM);

	if (percpu_ref_init(&p->users, swap_users_ref_free,
			    PERCPU_REF_INIT_DEAD, GFP_KERNEL)) {
		kvfree(p);
		return ERR_PTR(-ENOMEM);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&swap_lock);
	for (type = 0; type < nr_swapfiles; type++) {
		if (!(swap_info[type]->flags & SWP_USED))
			break;
	}
	if (type >= MAX_SWAPFILES) {
		spin_unlock(&swap_lock);
<<<<<<< HEAD
		kfree(p);
=======
		percpu_ref_exit(&p->users);
		kvfree(p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ERR_PTR(-EPERM);
	}
	if (type >= nr_swapfiles) {
		p->type = type;
<<<<<<< HEAD
		swap_info[type] = p;
		/*
		 * Write swap_info[type] before nr_swapfiles, in case a
		 * racing procfs swap_start() or swap_next() is reading them.
		 * (We never shrink nr_swapfiles, we never free this entry.)
		 */
		smp_wmb();
		nr_swapfiles++;
	} else {
		kfree(p);
=======
		/*
		 * Publish the swap_info_struct after initializing it.
		 * Note that kvzalloc() above zeroes all its fields.
		 */
		smp_store_release(&swap_info[type], p); /* rcu_assign_pointer() */
		nr_swapfiles++;
	} else {
		defer = p;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		p = swap_info[type];
		/*
		 * Do not memset this entry: a racing procfs swap_next()
		 * would be relying on p->type to remain valid.
		 */
	}
<<<<<<< HEAD
	INIT_LIST_HEAD(&p->first_swap_extent.list);
	p->flags = SWP_USED;
	p->next = -1;
	spin_unlock(&swap_lock);
	spin_lock_init(&p->lock);
=======
	p->swap_extent_root = RB_ROOT;
	plist_node_init(&p->list, 0);
	for_each_node(i)
		plist_node_init(&p->avail_lists[i], 0);
	p->flags = SWP_USED;
	spin_unlock(&swap_lock);
	if (defer) {
		percpu_ref_exit(&defer->users);
		kvfree(defer);
	}
	spin_lock_init(&p->lock);
	spin_lock_init(&p->cont_lock);
	init_completion(&p->comp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return p;
}

static int claim_swapfile(struct swap_info_struct *p, struct inode *inode)
{
	int error;

	if (S_ISBLK(inode->i_mode)) {
<<<<<<< HEAD
		p->bdev = bdgrab(I_BDEV(inode));
		error = blkdev_get(p->bdev,
				   FMODE_READ | FMODE_WRITE | FMODE_EXCL,
				   sys_swapon);
		if (error < 0) {
			p->bdev = NULL;
			return -EINVAL;
		}
=======
		p->bdev_file = bdev_file_open_by_dev(inode->i_rdev,
				BLK_OPEN_READ | BLK_OPEN_WRITE, p, NULL);
		if (IS_ERR(p->bdev_file)) {
			error = PTR_ERR(p->bdev_file);
			p->bdev_file = NULL;
			return error;
		}
		p->bdev = file_bdev(p->bdev_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		p->old_block_size = block_size(p->bdev);
		error = set_blocksize(p->bdev, PAGE_SIZE);
		if (error < 0)
			return error;
<<<<<<< HEAD
		p->flags |= SWP_BLKDEV;
	} else if (S_ISREG(inode->i_mode)) {
		p->bdev = inode->i_sb->s_bdev;
		mutex_lock(&inode->i_mutex);
		if (IS_SWAPFILE(inode))
			return -EBUSY;
	} else
		return -EINVAL;
=======
		/*
		 * Zoned block devices contain zones that have a sequential
		 * write only restriction.  Hence zoned block devices are not
		 * suitable for swapping.  Disallow them here.
		 */
		if (bdev_is_zoned(p->bdev))
			return -EINVAL;
		p->flags |= SWP_BLKDEV;
	} else if (S_ISREG(inode->i_mode)) {
		p->bdev = inode->i_sb->s_bdev;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
=======

/*
 * Find out how many pages are allowed for a single swap device. There
 * are two limiting factors:
 * 1) the number of bits for the swap offset in the swp_entry_t type, and
 * 2) the number of bits in the swap pte, as defined by the different
 * architectures.
 *
 * In order to find the largest possible bit mask, a swap entry with
 * swap type 0 and swap offset ~0UL is created, encoded to a swap pte,
 * decoded to a swp_entry_t again, and finally the swap offset is
 * extracted.
 *
 * This will mask all the bits from the initial ~0UL mask that can't
 * be encoded in either the swp_entry_t or the architecture definition
 * of a swap pte.
 */
unsigned long generic_max_swapfile_size(void)
{
	return swp_offset(pte_to_swp_entry(
			swp_entry_to_pte(swp_entry(0, ~0UL)))) + 1;
}

/* Can be overridden by an architecture for additional checks. */
__weak unsigned long arch_max_swapfile_size(void)
{
	return generic_max_swapfile_size();
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned long read_swap_header(struct swap_info_struct *p,
					union swap_header *swap_header,
					struct inode *inode)
{
	int i;
	unsigned long maxpages;
	unsigned long swapfilepages;
<<<<<<< HEAD

	if (memcmp("SWAPSPACE2", swap_header->magic.magic, 10)) {
		printk(KERN_ERR "Unable to find swap-space signature\n");
		return 0;
	}

	/* swap partition endianess hack... */
=======
	unsigned long last_page;

	if (memcmp("SWAPSPACE2", swap_header->magic.magic, 10)) {
		pr_err("Unable to find swap-space signature\n");
		return 0;
	}

	/* swap partition endianness hack... */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (swab32(swap_header->info.version) == 1) {
		swab32s(&swap_header->info.version);
		swab32s(&swap_header->info.last_page);
		swab32s(&swap_header->info.nr_badpages);
<<<<<<< HEAD
=======
		if (swap_header->info.nr_badpages > MAX_SWAP_BADPAGES)
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (i = 0; i < swap_header->info.nr_badpages; i++)
			swab32s(&swap_header->info.badpages[i]);
	}
	/* Check the swap header's sub-version */
	if (swap_header->info.version != 1) {
<<<<<<< HEAD
		printk(KERN_WARNING
		       "Unable to handle swap header version %d\n",
		       swap_header->info.version);
=======
		pr_warn("Unable to handle swap header version %d\n",
			swap_header->info.version);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	p->lowest_bit  = 1;
	p->cluster_next = 1;
	p->cluster_nr = 0;

<<<<<<< HEAD
	/*
	 * Find out how many pages are allowed for a single swap
	 * device. There are two limiting factors: 1) the number
	 * of bits for the swap offset in the swp_entry_t type, and
	 * 2) the number of bits in the swap pte as defined by the
	 * different architectures. In order to find the
	 * largest possible bit mask, a swap entry with swap type 0
	 * and swap offset ~0UL is created, encoded to a swap pte,
	 * decoded to a swp_entry_t again, and finally the swap
	 * offset is extracted. This will mask all the bits from
	 * the initial ~0UL mask that can't be encoded in either
	 * the swp_entry_t or the architecture definition of a
	 * swap pte.
	 */
	maxpages = swp_offset(pte_to_swp_entry(
			swp_entry_to_pte(swp_entry(0, ~0UL)))) + 1;
	if (maxpages > swap_header->info.last_page) {
		maxpages = swap_header->info.last_page + 1;
=======
	maxpages = swapfile_maximum_size;
	last_page = swap_header->info.last_page;
	if (!last_page) {
		pr_warn("Empty swap-file\n");
		return 0;
	}
	if (last_page > maxpages) {
		pr_warn("Truncating oversized swap area, only using %luk out of %luk\n",
			K(maxpages), K(last_page));
	}
	if (maxpages > last_page) {
		maxpages = last_page + 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* p->max is an unsigned int: don't overflow it */
		if ((unsigned int)maxpages == 0)
			maxpages = UINT_MAX;
	}
	p->highest_bit = maxpages - 1;

	if (!maxpages)
		return 0;
	swapfilepages = i_size_read(inode) >> PAGE_SHIFT;
	if (swapfilepages && maxpages > swapfilepages) {
<<<<<<< HEAD
		printk(KERN_WARNING
		       "Swap area shorter than signature indicates\n");
=======
		pr_warn("Swap area shorter than signature indicates\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	if (swap_header->info.nr_badpages && S_ISREG(inode->i_mode))
		return 0;
	if (swap_header->info.nr_badpages > MAX_SWAP_BADPAGES)
		return 0;

	return maxpages;
}

<<<<<<< HEAD
static int setup_swap_map_and_extents(struct swap_info_struct *p,
					union swap_header *swap_header,
					unsigned char *swap_map,
					unsigned long maxpages,
					sector_t *span)
{
	int i;
	unsigned int nr_good_pages;
	int nr_extents;

	nr_good_pages = maxpages - 1;	/* omit header page */

=======
#define SWAP_CLUSTER_INFO_COLS						\
	DIV_ROUND_UP(L1_CACHE_BYTES, sizeof(struct swap_cluster_info))
#define SWAP_CLUSTER_SPACE_COLS						\
	DIV_ROUND_UP(SWAP_ADDRESS_SPACE_PAGES, SWAPFILE_CLUSTER)
#define SWAP_CLUSTER_COLS						\
	max_t(unsigned int, SWAP_CLUSTER_INFO_COLS, SWAP_CLUSTER_SPACE_COLS)

static int setup_swap_map_and_extents(struct swap_info_struct *p,
					union swap_header *swap_header,
					unsigned char *swap_map,
					struct swap_cluster_info *cluster_info,
					unsigned long maxpages,
					sector_t *span)
{
	unsigned int j, k;
	unsigned int nr_good_pages;
	int nr_extents;
	unsigned long nr_clusters = DIV_ROUND_UP(maxpages, SWAPFILE_CLUSTER);
	unsigned long col = p->cluster_next / SWAPFILE_CLUSTER % SWAP_CLUSTER_COLS;
	unsigned long i, idx;

	nr_good_pages = maxpages - 1;	/* omit header page */

	cluster_list_init(&p->free_clusters);
	cluster_list_init(&p->discard_clusters);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < swap_header->info.nr_badpages; i++) {
		unsigned int page_nr = swap_header->info.badpages[i];
		if (page_nr == 0 || page_nr > swap_header->info.last_page)
			return -EINVAL;
		if (page_nr < maxpages) {
			swap_map[page_nr] = SWAP_MAP_BAD;
			nr_good_pages--;
<<<<<<< HEAD
		}
	}

	if (nr_good_pages) {
		swap_map[0] = SWAP_MAP_BAD;
=======
			/*
			 * Haven't marked the cluster free yet, no list
			 * operation involved
			 */
			inc_cluster_info_page(p, cluster_info, page_nr);
		}
	}

	/* Haven't marked the cluster free yet, no list operation involved */
	for (i = maxpages; i < round_up(maxpages, SWAPFILE_CLUSTER); i++)
		inc_cluster_info_page(p, cluster_info, i);

	if (nr_good_pages) {
		swap_map[0] = SWAP_MAP_BAD;
		/*
		 * Not mark the cluster free yet, no list
		 * operation involved
		 */
		inc_cluster_info_page(p, cluster_info, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		p->max = maxpages;
		p->pages = nr_good_pages;
		nr_extents = setup_swap_extents(p, span);
		if (nr_extents < 0)
			return nr_extents;
		nr_good_pages = p->pages;
	}
	if (!nr_good_pages) {
<<<<<<< HEAD
		printk(KERN_WARNING "Empty swap-file\n");
		return -EINVAL;
	}

=======
		pr_warn("Empty swap-file\n");
		return -EINVAL;
	}

	if (!cluster_info)
		return nr_extents;


	/*
	 * Reduce false cache line sharing between cluster_info and
	 * sharing same address space.
	 */
	for (k = 0; k < SWAP_CLUSTER_COLS; k++) {
		j = (k + col) % SWAP_CLUSTER_COLS;
		for (i = 0; i < DIV_ROUND_UP(nr_clusters, SWAP_CLUSTER_COLS); i++) {
			idx = i * SWAP_CLUSTER_COLS + j;
			if (idx >= nr_clusters)
				continue;
			if (cluster_count(&cluster_info[idx]))
				continue;
			cluster_set_flag(&cluster_info[idx], CLUSTER_FLAG_FREE);
			cluster_list_add_tail(&p->free_clusters, cluster_info,
					      idx);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return nr_extents;
}

SYSCALL_DEFINE2(swapon, const char __user *, specialfile, int, swap_flags)
{
	struct swap_info_struct *p;
	struct filename *name;
	struct file *swap_file = NULL;
	struct address_space *mapping;
<<<<<<< HEAD
	int i;
=======
	struct dentry *dentry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int prio;
	int error;
	union swap_header *swap_header;
	int nr_extents;
	sector_t span;
	unsigned long maxpages;
	unsigned char *swap_map = NULL;
<<<<<<< HEAD
	struct page *page = NULL;
	struct inode *inode = NULL;
=======
	struct swap_cluster_info *cluster_info = NULL;
	struct page *page = NULL;
	struct inode *inode = NULL;
	bool inced_nr_rotate_swap = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (swap_flags & ~SWAP_FLAGS_VALID)
		return -EINVAL;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

<<<<<<< HEAD
=======
	if (!swap_avail_heads)
		return -ENOMEM;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p = alloc_swap_info();
	if (IS_ERR(p))
		return PTR_ERR(p);

<<<<<<< HEAD
=======
	INIT_WORK(&p->discard_work, swap_discard_work);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	name = getname(specialfile);
	if (IS_ERR(name)) {
		error = PTR_ERR(name);
		name = NULL;
		goto bad_swap;
	}
	swap_file = file_open_name(name, O_RDWR|O_LARGEFILE, 0);
	if (IS_ERR(swap_file)) {
		error = PTR_ERR(swap_file);
		swap_file = NULL;
		goto bad_swap;
	}

	p->swap_file = swap_file;
	mapping = swap_file->f_mapping;
<<<<<<< HEAD

	for (i = 0; i < nr_swapfiles; i++) {
		struct swap_info_struct *q = swap_info[i];

		if (q == p || !q->swap_file)
			continue;
		if (mapping == q->swap_file->f_mapping) {
			error = -EBUSY;
			goto bad_swap;
		}
	}

	inode = mapping->host;
	/* If S_ISREG(inode->i_mode) will do mutex_lock(&inode->i_mutex); */
=======
	dentry = swap_file->f_path.dentry;
	inode = mapping->host;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = claim_swapfile(p, inode);
	if (unlikely(error))
		goto bad_swap;

<<<<<<< HEAD
	/*
	 * Read the swap header.
	 */
	if (!mapping->a_ops->readpage) {
		error = -EINVAL;
		goto bad_swap;
=======
	inode_lock(inode);
	if (d_unlinked(dentry) || cant_mount(dentry)) {
		error = -ENOENT;
		goto bad_swap_unlock_inode;
	}
	if (IS_SWAPFILE(inode)) {
		error = -EBUSY;
		goto bad_swap_unlock_inode;
	}

	/*
	 * Read the swap header.
	 */
	if (!mapping->a_ops->read_folio) {
		error = -EINVAL;
		goto bad_swap_unlock_inode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	page = read_mapping_page(mapping, 0, swap_file);
	if (IS_ERR(page)) {
		error = PTR_ERR(page);
<<<<<<< HEAD
		goto bad_swap;
=======
		goto bad_swap_unlock_inode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	swap_header = kmap(page);

	maxpages = read_swap_header(p, swap_header, inode);
	if (unlikely(!maxpages)) {
		error = -EINVAL;
<<<<<<< HEAD
		goto bad_swap;
=======
		goto bad_swap_unlock_inode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* OK, set up the swap map and apply the bad block list */
	swap_map = vzalloc(maxpages);
	if (!swap_map) {
		error = -ENOMEM;
<<<<<<< HEAD
		goto bad_swap;
=======
		goto bad_swap_unlock_inode;
	}

	if (p->bdev && bdev_stable_writes(p->bdev))
		p->flags |= SWP_STABLE_WRITES;

	if (p->bdev && bdev_synchronous(p->bdev))
		p->flags |= SWP_SYNCHRONOUS_IO;

	if (p->bdev && bdev_nonrot(p->bdev)) {
		int cpu;
		unsigned long ci, nr_cluster;

		p->flags |= SWP_SOLIDSTATE;
		p->cluster_next_cpu = alloc_percpu(unsigned int);
		if (!p->cluster_next_cpu) {
			error = -ENOMEM;
			goto bad_swap_unlock_inode;
		}
		/*
		 * select a random position to start with to help wear leveling
		 * SSD
		 */
		for_each_possible_cpu(cpu) {
			per_cpu(*p->cluster_next_cpu, cpu) =
				get_random_u32_inclusive(1, p->highest_bit);
		}
		nr_cluster = DIV_ROUND_UP(maxpages, SWAPFILE_CLUSTER);

		cluster_info = kvcalloc(nr_cluster, sizeof(*cluster_info),
					GFP_KERNEL);
		if (!cluster_info) {
			error = -ENOMEM;
			goto bad_swap_unlock_inode;
		}

		for (ci = 0; ci < nr_cluster; ci++)
			spin_lock_init(&((cluster_info + ci)->lock));

		p->percpu_cluster = alloc_percpu(struct percpu_cluster);
		if (!p->percpu_cluster) {
			error = -ENOMEM;
			goto bad_swap_unlock_inode;
		}
		for_each_possible_cpu(cpu) {
			struct percpu_cluster *cluster;
			cluster = per_cpu_ptr(p->percpu_cluster, cpu);
			cluster_set_null(&cluster->index);
		}
	} else {
		atomic_inc(&nr_rotate_swap);
		inced_nr_rotate_swap = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	error = swap_cgroup_swapon(p->type, maxpages);
	if (error)
<<<<<<< HEAD
		goto bad_swap;

	nr_extents = setup_swap_map_and_extents(p, swap_header, swap_map,
		maxpages, &span);
	if (unlikely(nr_extents < 0)) {
		error = nr_extents;
		goto bad_swap;
	}

	if (p->bdev) {
		if (blk_queue_nonrot(bdev_get_queue(p->bdev))) {
			p->flags |= SWP_SOLIDSTATE;
			p->cluster_next = 1 + (random32() % p->highest_bit);
		}
		if ((swap_flags & SWAP_FLAG_DISCARD) && discard_swap(p) == 0)
			p->flags |= SWP_DISCARDABLE;
	}

	if (p->bdev && blk_queue_fast(bdev_get_queue(p->bdev)))
		p->flags |= SWP_FAST;
=======
		goto bad_swap_unlock_inode;

	nr_extents = setup_swap_map_and_extents(p, swap_header, swap_map,
		cluster_info, maxpages, &span);
	if (unlikely(nr_extents < 0)) {
		error = nr_extents;
		goto bad_swap_unlock_inode;
	}

	if ((swap_flags & SWAP_FLAG_DISCARD) &&
	    p->bdev && bdev_max_discard_sectors(p->bdev)) {
		/*
		 * When discard is enabled for swap with no particular
		 * policy flagged, we set all swap discard flags here in
		 * order to sustain backward compatibility with older
		 * swapon(8) releases.
		 */
		p->flags |= (SWP_DISCARDABLE | SWP_AREA_DISCARD |
			     SWP_PAGE_DISCARD);

		/*
		 * By flagging sys_swapon, a sysadmin can tell us to
		 * either do single-time area discards only, or to just
		 * perform discards for released swap page-clusters.
		 * Now it's time to adjust the p->flags accordingly.
		 */
		if (swap_flags & SWAP_FLAG_DISCARD_ONCE)
			p->flags &= ~SWP_PAGE_DISCARD;
		else if (swap_flags & SWAP_FLAG_DISCARD_PAGES)
			p->flags &= ~SWP_AREA_DISCARD;

		/* issue a swapon-time discard if it's still required */
		if (p->flags & SWP_AREA_DISCARD) {
			int err = discard_swap(p);
			if (unlikely(err))
				pr_err("swapon: discard_swap(%p): %d\n",
					p, err);
		}
	}

	error = init_swap_address_space(p->type, maxpages);
	if (error)
		goto bad_swap_unlock_inode;

	error = zswap_swapon(p->type, maxpages);
	if (error)
		goto free_swap_address_space;

	/*
	 * Flush any pending IO and dirty mappings before we start using this
	 * swap device.
	 */
	inode->i_flags |= S_SWAPFILE;
	error = inode_drain_writes(inode);
	if (error) {
		inode->i_flags &= ~S_SWAPFILE;
		goto free_swap_zswap;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&swapon_mutex);
	prio = -1;
	if (swap_flags & SWAP_FLAG_PREFER)
		prio =
		  (swap_flags & SWAP_FLAG_PRIO_MASK) >> SWAP_FLAG_PRIO_SHIFT;
<<<<<<< HEAD
	enable_swap_info(p, prio, swap_map);

	printk(KERN_INFO "Adding %uk swap on %s.  "
			"Priority:%d extents:%d across:%lluk %s%s\n",
		p->pages<<(PAGE_SHIFT-10), name->name, p->prio,
		nr_extents, (unsigned long long)span<<(PAGE_SHIFT-10),
		(p->flags & SWP_SOLIDSTATE) ? "SS" : "",
		(p->flags & SWP_DISCARDABLE) ? "D" : "");
=======
	enable_swap_info(p, prio, swap_map, cluster_info);

	pr_info("Adding %uk swap on %s.  Priority:%d extents:%d across:%lluk %s%s%s%s\n",
		K(p->pages), name->name, p->prio, nr_extents,
		K((unsigned long long)span),
		(p->flags & SWP_SOLIDSTATE) ? "SS" : "",
		(p->flags & SWP_DISCARDABLE) ? "D" : "",
		(p->flags & SWP_AREA_DISCARD) ? "s" : "",
		(p->flags & SWP_PAGE_DISCARD) ? "c" : "");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_unlock(&swapon_mutex);
	atomic_inc(&proc_poll_event);
	wake_up_interruptible(&proc_poll_wait);

<<<<<<< HEAD
	if (S_ISREG(inode->i_mode))
		inode->i_flags |= S_SWAPFILE;
	error = 0;
	goto out;
bad_swap:
	if (inode && S_ISBLK(inode->i_mode) && p->bdev) {
		set_blocksize(p->bdev, p->old_block_size);
		blkdev_put(p->bdev, FMODE_READ | FMODE_WRITE | FMODE_EXCL);
	}
=======
	error = 0;
	goto out;
free_swap_zswap:
	zswap_swapoff(p->type);
free_swap_address_space:
	exit_swap_address_space(p->type);
bad_swap_unlock_inode:
	inode_unlock(inode);
bad_swap:
	free_percpu(p->percpu_cluster);
	p->percpu_cluster = NULL;
	free_percpu(p->cluster_next_cpu);
	p->cluster_next_cpu = NULL;
	if (p->bdev_file) {
		set_blocksize(p->bdev, p->old_block_size);
		fput(p->bdev_file);
		p->bdev_file = NULL;
	}
	inode = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	destroy_swap_extents(p);
	swap_cgroup_swapoff(p->type);
	spin_lock(&swap_lock);
	p->swap_file = NULL;
	p->flags = 0;
	spin_unlock(&swap_lock);
	vfree(swap_map);
<<<<<<< HEAD
	if (swap_file) {
		if (inode && S_ISREG(inode->i_mode)) {
			mutex_unlock(&inode->i_mutex);
			inode = NULL;
		}
		filp_close(swap_file, NULL);
	}
out:
	if (page && !IS_ERR(page)) {
		kunmap(page);
		page_cache_release(page);
	}
	if (name)
		putname(name);
	if (inode && S_ISREG(inode->i_mode))
		mutex_unlock(&inode->i_mutex);
=======
	kvfree(cluster_info);
	if (inced_nr_rotate_swap)
		atomic_dec(&nr_rotate_swap);
	if (swap_file)
		filp_close(swap_file, NULL);
out:
	if (page && !IS_ERR(page)) {
		kunmap(page);
		put_page(page);
	}
	if (name)
		putname(name);
	if (inode)
		inode_unlock(inode);
	if (!error)
		enable_swap_slots_cache();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

void si_swapinfo(struct sysinfo *val)
{
	unsigned int type;
	unsigned long nr_to_be_unused = 0;

	spin_lock(&swap_lock);
	for (type = 0; type < nr_swapfiles; type++) {
		struct swap_info_struct *si = swap_info[type];

		if ((si->flags & SWP_USED) && !(si->flags & SWP_WRITEOK))
<<<<<<< HEAD
			nr_to_be_unused += si->inuse_pages;
=======
			nr_to_be_unused += READ_ONCE(si->inuse_pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	val->freeswap = atomic_long_read(&nr_swap_pages) + nr_to_be_unused;
	val->totalswap = total_swap_pages + nr_to_be_unused;
	spin_unlock(&swap_lock);
}

/*
 * Verify that a swap entry is valid and increment its swap map count.
 *
 * Returns error code in following case.
 * - success -> 0
 * - swp_entry is invalid -> EINVAL
 * - swp_entry is migration entry -> EINVAL
 * - swap-cache reference is requested but there is already one. -> EEXIST
 * - swap-cache reference is requested but the entry is not used. -> ENOENT
 * - swap-mapped reference requested but needs continued swap count. -> ENOMEM
 */
static int __swap_duplicate(swp_entry_t entry, unsigned char usage)
{
	struct swap_info_struct *p;
<<<<<<< HEAD
	unsigned long offset, type;
	unsigned char count;
	unsigned char has_cache;
	int err = -EINVAL;

	if (non_swap_entry(entry))
		goto out;

	type = swp_type(entry);
	if (type >= nr_swapfiles)
		goto bad_file;
	p = swap_info[type];
	offset = swp_offset(entry);

	spin_lock(&p->lock);
	if (unlikely(offset >= p->max))
		goto unlock_out;

	count = p->swap_map[offset];
=======
	struct swap_cluster_info *ci;
	unsigned long offset;
	unsigned char count;
	unsigned char has_cache;
	int err;

	p = swp_swap_info(entry);

	offset = swp_offset(entry);
	ci = lock_cluster_or_swap_info(p, offset);

	count = p->swap_map[offset];

	/*
	 * swapin_readahead() doesn't check if a swap entry is valid, so the
	 * swap entry could be SWAP_MAP_BAD. Check here with lock held.
	 */
	if (unlikely(swap_count(count) == SWAP_MAP_BAD)) {
		err = -ENOENT;
		goto unlock_out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	has_cache = count & SWAP_HAS_CACHE;
	count &= ~SWAP_HAS_CACHE;
	err = 0;

	if (usage == SWAP_HAS_CACHE) {

		/* set SWAP_HAS_CACHE if there is no cache and entry is used */
		if (!has_cache && count)
			has_cache = SWAP_HAS_CACHE;
		else if (has_cache)		/* someone else added cache */
			err = -EEXIST;
		else				/* no users remaining */
			err = -ENOENT;

	} else if (count || has_cache) {

		if ((count & ~COUNT_CONTINUED) < SWAP_MAP_MAX)
			count += usage;
		else if ((count & ~COUNT_CONTINUED) > SWAP_MAP_MAX)
			err = -EINVAL;
		else if (swap_count_continued(p, offset, count))
			count = COUNT_CONTINUED;
		else
			err = -ENOMEM;
	} else
		err = -ENOENT;			/* unused swap entry */

<<<<<<< HEAD
	p->swap_map[offset] = count | has_cache;

unlock_out:
	spin_unlock(&p->lock);
out:
	return err;

bad_file:
	printk(KERN_ERR "swap_dup: %s%08lx\n", Bad_file, entry.val);
	goto out;
=======
	if (!err)
		WRITE_ONCE(p->swap_map[offset], count | has_cache);

unlock_out:
	unlock_cluster_or_swap_info(p, ci);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Help swapoff by noting that swap entry belongs to shmem/tmpfs
 * (in which case its reference count is never incremented).
 */
void swap_shmem_alloc(swp_entry_t entry)
{
	__swap_duplicate(entry, SWAP_MAP_SHMEM);
}

/*
 * Increase reference count of swap entry by 1.
 * Returns 0 for success, or -ENOMEM if a swap_count_continuation is required
 * but could not be atomically allocated.  Returns 0, just as if it succeeded,
 * if __swap_duplicate() fails for another reason (-EINVAL or -ENOENT), which
 * might occur if a page table entry has got corrupted.
 */
int swap_duplicate(swp_entry_t entry)
{
	int err = 0;

	while (!err && __swap_duplicate(entry, 1) == -ENOMEM)
		err = add_swap_count_continuation(entry, GFP_ATOMIC);
	return err;
}

/*
 * @entry: swap entry for which we allocate swap cache.
 *
 * Called when allocating swap cache for existing swap entry,
 * This can return error codes. Returns 0 at success.
<<<<<<< HEAD
 * -EBUSY means there is a swap cache.
=======
 * -EEXIST means there is a swap cache.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Note: return code is different from swap_duplicate().
 */
int swapcache_prepare(swp_entry_t entry)
{
	return __swap_duplicate(entry, SWAP_HAS_CACHE);
}

<<<<<<< HEAD
struct swap_info_struct *page_swap_info(struct page *page)
{
	swp_entry_t swap = { .val = page_private(page) };
	BUG_ON(!PageSwapCache(page));
	return swap_info[swp_type(swap)];
}

/*
 * out-of-line __page_file_ methods to avoid include hell.
 */
struct address_space *__page_file_mapping(struct page *page)
{
	VM_BUG_ON(!PageSwapCache(page));
	return page_swap_info(page)->swap_file->f_mapping;
}
EXPORT_SYMBOL_GPL(__page_file_mapping);

pgoff_t __page_file_index(struct page *page)
{
	swp_entry_t swap = { .val = page_private(page) };
	VM_BUG_ON(!PageSwapCache(page));
=======
void swapcache_clear(struct swap_info_struct *si, swp_entry_t entry)
{
	struct swap_cluster_info *ci;
	unsigned long offset = swp_offset(entry);
	unsigned char usage;

	ci = lock_cluster_or_swap_info(si, offset);
	usage = __swap_entry_free_locked(si, offset, SWAP_HAS_CACHE);
	unlock_cluster_or_swap_info(si, ci);
	if (!usage)
		free_swap_slot(entry);
}

struct swap_info_struct *swp_swap_info(swp_entry_t entry)
{
	return swap_type_to_swap_info(swp_type(entry));
}

/*
 * out-of-line methods to avoid include hell.
 */
struct address_space *swapcache_mapping(struct folio *folio)
{
	return swp_swap_info(folio->swap)->swap_file->f_mapping;
}
EXPORT_SYMBOL_GPL(swapcache_mapping);

pgoff_t __page_file_index(struct page *page)
{
	swp_entry_t swap = page_swap_entry(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return swp_offset(swap);
}
EXPORT_SYMBOL_GPL(__page_file_index);

/*
 * add_swap_count_continuation - called when a swap count is duplicated
 * beyond SWAP_MAP_MAX, it allocates a new page and links that to the entry's
 * page of the original vmalloc'ed swap_map, to hold the continuation count
 * (for that entry and for its neighbouring PAGE_SIZE swap entries).  Called
 * again when count is duplicated beyond SWAP_MAP_MAX * SWAP_CONT_MAX, etc.
 *
 * These continuation pages are seldom referenced: the common paths all work
 * on the original swap_map, only referring to a continuation page when the
 * low "digit" of a count is incremented or decremented through SWAP_MAP_MAX.
 *
 * add_swap_count_continuation(, GFP_ATOMIC) can be called while holding
 * page table locks; if it fails, add_swap_count_continuation(, GFP_KERNEL)
 * can be called after dropping locks.
 */
int add_swap_count_continuation(swp_entry_t entry, gfp_t gfp_mask)
{
	struct swap_info_struct *si;
<<<<<<< HEAD
=======
	struct swap_cluster_info *ci;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct page *head;
	struct page *page;
	struct page *list_page;
	pgoff_t offset;
	unsigned char count;
<<<<<<< HEAD
=======
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * When debugging, it's easier to use __GFP_ZERO here; but it's better
	 * for latency not to zero a page while GFP_ATOMIC and holding locks.
	 */
	page = alloc_page(gfp_mask | __GFP_HIGHMEM);

<<<<<<< HEAD
	si = swap_info_get(entry);
	if (!si) {
		/*
		 * An acceptable race has occurred since the failing
		 * __swap_duplicate(): the swap entry has been freed,
		 * perhaps even the whole swap_map cleared for swapoff.
		 */
		goto outer;
	}

	offset = swp_offset(entry);
	count = si->swap_map[offset] & ~SWAP_HAS_CACHE;
=======
	si = get_swap_device(entry);
	if (!si) {
		/*
		 * An acceptable race has occurred since the failing
		 * __swap_duplicate(): the swap device may be swapoff
		 */
		goto outer;
	}
	spin_lock(&si->lock);

	offset = swp_offset(entry);

	ci = lock_cluster(si, offset);

	count = swap_count(si->swap_map[offset]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((count & ~COUNT_CONTINUED) != SWAP_MAP_MAX) {
		/*
		 * The higher the swap count, the more likely it is that tasks
		 * will race to add swap count continuation: we need to avoid
		 * over-provisioning.
		 */
		goto out;
	}

	if (!page) {
<<<<<<< HEAD
		spin_unlock(&si->lock);
		return -ENOMEM;
	}

	/*
	 * We are fortunate that although vmalloc_to_page uses pte_offset_map,
	 * no architecture is using highmem pages for kernel pagetables: so it
	 * will not corrupt the GFP_ATOMIC caller's atomic pagetable kmaps.
	 */
	head = vmalloc_to_page(si->swap_map + offset);
	offset &= ~PAGE_MASK;

=======
		ret = -ENOMEM;
		goto out;
	}

	head = vmalloc_to_page(si->swap_map + offset);
	offset &= ~PAGE_MASK;

	spin_lock(&si->cont_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Page allocation does not initialize the page's lru field,
	 * but it does always reset its private field.
	 */
	if (!page_private(head)) {
		BUG_ON(count & COUNT_CONTINUED);
		INIT_LIST_HEAD(&head->lru);
		set_page_private(head, SWP_CONTINUED);
		si->flags |= SWP_CONTINUED;
	}

	list_for_each_entry(list_page, &head->lru, lru) {
		unsigned char *map;

		/*
		 * If the previous map said no continuation, but we've found
		 * a continuation page, free our allocation and use this one.
		 */
		if (!(count & COUNT_CONTINUED))
<<<<<<< HEAD
			goto out;

		map = kmap_atomic(list_page) + offset;
		count = *map;
		kunmap_atomic(map);
=======
			goto out_unlock_cont;

		map = kmap_local_page(list_page) + offset;
		count = *map;
		kunmap_local(map);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * If this continuation count now has some space in it,
		 * free our allocation and use this one.
		 */
		if ((count & ~COUNT_CONTINUED) != SWAP_CONT_MAX)
<<<<<<< HEAD
			goto out;
=======
			goto out_unlock_cont;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	list_add_tail(&page->lru, &head->lru);
	page = NULL;			/* now it's attached, don't free it */
<<<<<<< HEAD
out:
	spin_unlock(&si->lock);
outer:
	if (page)
		__free_page(page);
	return 0;
=======
out_unlock_cont:
	spin_unlock(&si->cont_lock);
out:
	unlock_cluster(ci);
	spin_unlock(&si->lock);
	put_swap_device(si);
outer:
	if (page)
		__free_page(page);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * swap_count_continued - when the original swap_map count is incremented
 * from SWAP_MAP_MAX, check if there is already a continuation page to carry
 * into, carry if so, or else fail until a new continuation page is allocated;
 * when the original swap_map count is decremented from 0 with continuation,
 * borrow from the continuation and report whether it still holds more.
<<<<<<< HEAD
 * Called while __swap_duplicate() or swap_entry_free() holds swap_lock.
=======
 * Called while __swap_duplicate() or swap_entry_free() holds swap or cluster
 * lock.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static bool swap_count_continued(struct swap_info_struct *si,
				 pgoff_t offset, unsigned char count)
{
	struct page *head;
	struct page *page;
	unsigned char *map;
<<<<<<< HEAD
=======
	bool ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	head = vmalloc_to_page(si->swap_map + offset);
	if (page_private(head) != SWP_CONTINUED) {
		BUG_ON(count & COUNT_CONTINUED);
		return false;		/* need to add count continuation */
	}

<<<<<<< HEAD
	offset &= ~PAGE_MASK;
	page = list_entry(head->lru.next, struct page, lru);
	map = kmap_atomic(page) + offset;
=======
	spin_lock(&si->cont_lock);
	offset &= ~PAGE_MASK;
	page = list_next_entry(head, lru);
	map = kmap_local_page(page) + offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (count == SWAP_MAP_MAX)	/* initial increment from swap_map */
		goto init_map;		/* jump over SWAP_CONT_MAX checks */

	if (count == (SWAP_MAP_MAX | COUNT_CONTINUED)) { /* incrementing */
		/*
		 * Think of how you add 1 to 999
		 */
		while (*map == (SWAP_CONT_MAX | COUNT_CONTINUED)) {
<<<<<<< HEAD
			kunmap_atomic(map);
			page = list_entry(page->lru.next, struct page, lru);
			BUG_ON(page == head);
			map = kmap_atomic(page) + offset;
		}
		if (*map == SWAP_CONT_MAX) {
			kunmap_atomic(map);
			page = list_entry(page->lru.next, struct page, lru);
			if (page == head)
				return false;	/* add count continuation */
			map = kmap_atomic(page) + offset;
init_map:		*map = 0;		/* we didn't zero the page */
		}
		*map += 1;
		kunmap_atomic(map);
		page = list_entry(page->lru.prev, struct page, lru);
		while (page != head) {
			map = kmap_atomic(page) + offset;
			*map = COUNT_CONTINUED;
			kunmap_atomic(map);
			page = list_entry(page->lru.prev, struct page, lru);
		}
		return true;			/* incremented */
=======
			kunmap_local(map);
			page = list_next_entry(page, lru);
			BUG_ON(page == head);
			map = kmap_local_page(page) + offset;
		}
		if (*map == SWAP_CONT_MAX) {
			kunmap_local(map);
			page = list_next_entry(page, lru);
			if (page == head) {
				ret = false;	/* add count continuation */
				goto out;
			}
			map = kmap_local_page(page) + offset;
init_map:		*map = 0;		/* we didn't zero the page */
		}
		*map += 1;
		kunmap_local(map);
		while ((page = list_prev_entry(page, lru)) != head) {
			map = kmap_local_page(page) + offset;
			*map = COUNT_CONTINUED;
			kunmap_local(map);
		}
		ret = true;			/* incremented */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	} else {				/* decrementing */
		/*
		 * Think of how you subtract 1 from 1000
		 */
		BUG_ON(count != COUNT_CONTINUED);
		while (*map == COUNT_CONTINUED) {
<<<<<<< HEAD
			kunmap_atomic(map);
			page = list_entry(page->lru.next, struct page, lru);
			BUG_ON(page == head);
			map = kmap_atomic(page) + offset;
=======
			kunmap_local(map);
			page = list_next_entry(page, lru);
			BUG_ON(page == head);
			map = kmap_local_page(page) + offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		BUG_ON(*map == 0);
		*map -= 1;
		if (*map == 0)
			count = 0;
<<<<<<< HEAD
		kunmap_atomic(map);
		page = list_entry(page->lru.prev, struct page, lru);
		while (page != head) {
			map = kmap_atomic(page) + offset;
			*map = SWAP_CONT_MAX | count;
			count = COUNT_CONTINUED;
			kunmap_atomic(map);
			page = list_entry(page->lru.prev, struct page, lru);
		}
		return count == COUNT_CONTINUED;
	}
=======
		kunmap_local(map);
		while ((page = list_prev_entry(page, lru)) != head) {
			map = kmap_local_page(page) + offset;
			*map = SWAP_CONT_MAX | count;
			count = COUNT_CONTINUED;
			kunmap_local(map);
		}
		ret = count == COUNT_CONTINUED;
	}
out:
	spin_unlock(&si->cont_lock);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * free_swap_count_continuations - swapoff free all the continuation pages
 * appended to the swap_map, after swap_map is quiesced, before vfree'ing it.
 */
static void free_swap_count_continuations(struct swap_info_struct *si)
{
	pgoff_t offset;

	for (offset = 0; offset < si->max; offset += PAGE_SIZE) {
		struct page *head;
		head = vmalloc_to_page(si->swap_map + offset);
		if (page_private(head)) {
<<<<<<< HEAD
			struct list_head *this, *next;
			list_for_each_safe(this, next, &head->lru) {
				struct page *page;
				page = list_entry(this, struct page, lru);
				list_del(this);
=======
			struct page *page, *next;

			list_for_each_entry_safe(page, next, &head->lru, lru) {
				list_del(&page->lru);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				__free_page(page);
			}
		}
	}
}
<<<<<<< HEAD
=======

#if defined(CONFIG_MEMCG) && defined(CONFIG_BLK_CGROUP)
void __folio_throttle_swaprate(struct folio *folio, gfp_t gfp)
{
	struct swap_info_struct *si, *next;
	int nid = folio_nid(folio);

	if (!(gfp & __GFP_IO))
		return;

	if (!blk_cgroup_congested())
		return;

	/*
	 * We've already scheduled a throttle, avoid taking the global swap
	 * lock.
	 */
	if (current->throttle_disk)
		return;

	spin_lock(&swap_avail_lock);
	plist_for_each_entry_safe(si, next, &swap_avail_heads[nid],
				  avail_lists[nid]) {
		if (si->bdev) {
			blkcg_schedule_throttle(si->bdev->bd_disk, true);
			break;
		}
	}
	spin_unlock(&swap_avail_lock);
}
#endif

static int __init swapfile_init(void)
{
	int nid;

	swap_avail_heads = kmalloc_array(nr_node_ids, sizeof(struct plist_head),
					 GFP_KERNEL);
	if (!swap_avail_heads) {
		pr_emerg("Not enough memory for swap heads, swap is disabled\n");
		return -ENOMEM;
	}

	for_each_node(nid)
		plist_head_init(&swap_avail_heads[nid]);

	swapfile_maximum_size = arch_max_swapfile_size();

#ifdef CONFIG_MIGRATION
	if (swapfile_maximum_size >= (1UL << SWP_MIG_TOTAL_BITS))
		swap_migration_ad_supported = true;
#endif	/* CONFIG_MIGRATION */

	return 0;
}
subsys_initcall(swapfile_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
