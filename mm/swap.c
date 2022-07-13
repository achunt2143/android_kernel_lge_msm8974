<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/mm/swap.c
 *
 *  Copyright (C) 1991, 1992, 1993, 1994  Linus Torvalds
 */

/*
 * This file contains the default values for the operation of the
 * Linux VM subsystem. Fine-tuning documentation can be found in
<<<<<<< HEAD
 * Documentation/sysctl/vm.txt.
=======
 * Documentation/admin-guide/sysctl/vm.rst.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Started 18.12.91
 * Swap aging added 23.2.95, Stephen Tweedie.
 * Buffermem limits added 12.3.98, Rik van Riel.
 */

#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/kernel_stat.h>
#include <linux/swap.h>
#include <linux/mman.h>
#include <linux/pagemap.h>
#include <linux/pagevec.h>
#include <linux/init.h>
#include <linux/export.h>
#include <linux/mm_inline.h>
#include <linux/percpu_counter.h>
<<<<<<< HEAD
=======
#include <linux/memremap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/percpu.h>
#include <linux/cpu.h>
#include <linux/notifier.h>
#include <linux/backing-dev.h>
#include <linux/memcontrol.h>
#include <linux/gfp.h>
<<<<<<< HEAD
#include <linux/hugetlb.h>

#include "internal.h"

/* How many pages do we try to swap or page in/out together? */
int page_cluster;

static DEFINE_PER_CPU(struct pagevec[NR_LRU_LISTS], lru_add_pvecs);
static DEFINE_PER_CPU(struct pagevec, lru_rotate_pvecs);
static DEFINE_PER_CPU(struct pagevec, lru_deactivate_pvecs);

/*
 * This path almost never happens for VM activity - pages are normally
 * freed via pagevecs.  But it gets used by networking.
 */
static void __page_cache_release(struct page *page)
{
	if (PageLRU(page)) {
		unsigned long flags;
		struct zone *zone = page_zone(page);

		spin_lock_irqsave(&zone->lru_lock, flags);
		VM_BUG_ON(!PageLRU(page));
		__ClearPageLRU(page);
		del_page_from_lru_list(zone, page, page_off_lru(page));
		spin_unlock_irqrestore(&zone->lru_lock, flags);
	}
}

static void __put_single_page(struct page *page)
{
	__page_cache_release(page);
	free_hot_cold_page(page, 0);
}

static void __put_compound_page(struct page *page)
{
	compound_page_dtor *dtor;

	if (!PageHuge(page))
		__page_cache_release(page);
	dtor = get_compound_page_dtor(page);
	(*dtor)(page);
}

static void put_compound_page(struct page *page)
{
	if (unlikely(PageTail(page))) {
		/* __split_huge_page_refcount can run under us */
		struct page *page_head = compound_trans_head(page);

		if (likely(page != page_head &&
			   get_page_unless_zero(page_head))) {
			unsigned long flags;

			 if (PageHeadHuge(page_head)) {
				if (likely(PageTail(page))) {
					/*
					 * __split_huge_page_refcount
					 * cannot race here.
					 */
					VM_BUG_ON(!PageHead(page_head));
					atomic_dec(&page->_mapcount);
					if (put_page_testzero(page_head))
						VM_BUG_ON(1);
					if (put_page_testzero(page_head))
						__put_compound_page(page_head);
					return;
				} else {
					/*
					 * __split_huge_page_refcount
					 * run before us, "page" was a
					 * THP tail. The split
					 * page_head has been freed
					 * and reallocated as slab or
					 * hugetlbfs page of smaller
					 * order (only possible if
					 * reallocated as slab on
					 * x86).
					 */
					goto skip_lock;
				}
			}
			/*
			 * page_head wasn't a dangling pointer but it
			 * may not be a head page anymore by the time
			 * we obtain the lock. That is ok as long as it
			 * can't be freed from under us.
			 */
			flags = compound_lock_irqsave(page_head);
			if (unlikely(!PageTail(page))) {
				/* __split_huge_page_refcount run before us */
				compound_unlock_irqrestore(page_head, flags);
				VM_BUG_ON(PageHead(page_head));
skip_lock:
				if (put_page_testzero(page_head)) {
					/*
					 * The head page may have been
					 * freed and reallocated as a
					 * compound page of smaller
					 * order and then freed again.
					 * All we know is that it
					 * cannot have become: a THP
					 * page, a compound page of
					 * higher order, a tail page.
					 * That is because we still
					 * hold the refcount of the
					 * split THP tail and
					 * page_head was the THP head
					 * before the split.
					 */
					if (PageHead(page_head))
						__put_compound_page(page_head);
					else
						__put_single_page(page_head);
				}
out_put_single:
				if (put_page_testzero(page))
					__put_single_page(page);
				return;
			}
			VM_BUG_ON(page_head != page->first_page);
			/*
			 * We can release the refcount taken by
			 * get_page_unless_zero() now that
			 * __split_huge_page_refcount() is blocked on
			 * the compound_lock.
			 */
			if (put_page_testzero(page_head))
				VM_BUG_ON(1);
			/* __split_huge_page_refcount will wait now */
			VM_BUG_ON(page_mapcount(page) <= 0);
			atomic_dec(&page->_mapcount);
			VM_BUG_ON(atomic_read(&page_head->_count) <= 0);
			VM_BUG_ON(atomic_read(&page->_count) != 0);
			compound_unlock_irqrestore(page_head, flags);
			if (put_page_testzero(page_head)) {
				if (PageHead(page_head))
					__put_compound_page(page_head);
				else
					__put_single_page(page_head);
			}
		} else {
			/* page_head is a dangling pointer */
			VM_BUG_ON(PageTail(page));
			goto out_put_single;
		}
	} else if (put_page_testzero(page)) {
		if (PageHead(page))
			__put_compound_page(page);
		else
			__put_single_page(page);
	}
}

void put_page(struct page *page)
{
	if (unlikely(PageCompound(page)))
		put_compound_page(page);
	else if (put_page_testzero(page))
		__put_single_page(page);
}
EXPORT_SYMBOL(put_page);

/*
 * This function is exported but must not be called by anything other
 * than get_page(). It implements the slow path of get_page().
 */
bool __get_page_tail(struct page *page)
{
	/*
	 * This takes care of get_page() if run on a tail page
	 * returned by one of the get_user_pages/follow_page variants.
	 * get_user_pages/follow_page itself doesn't need the compound
	 * lock because it runs __get_page_tail_foll() under the
	 * proper PT lock that already serializes against
	 * split_huge_page().
	 */
	unsigned long flags;
	bool got = false;
	struct page *page_head = compound_trans_head(page);

	if (likely(page != page_head && get_page_unless_zero(page_head))) {
		/* Ref to put_compound_page() comment. */
		if (PageHeadHuge(page_head)) {
			if (likely(PageTail(page))) {
				/*
				 * This is a hugetlbfs
				 * page. __split_huge_page_refcount
				 * cannot race here.
				 */
				VM_BUG_ON(!PageHead(page_head));
				__get_page_tail_foll(page, false);
				return true;
			} else {
				/*
				 * __split_huge_page_refcount run
				 * before us, "page" was a THP
				 * tail. The split page_head has been
				 * freed and reallocated as slab or
				 * hugetlbfs page of smaller order
				 * (only possible if reallocated as
				 * slab on x86).
				 */
				put_page(page_head);
				return false;
			}
		}
		/*
		 * page_head wasn't a dangling pointer but it
		 * may not be a head page anymore by the time
		 * we obtain the lock. That is ok as long as it
		 * can't be freed from under us.
		 */
		flags = compound_lock_irqsave(page_head);
		/* here __split_huge_page_refcount won't run anymore */
		if (likely(PageTail(page))) {
			__get_page_tail_foll(page, false);
			got = true;
		}
		compound_unlock_irqrestore(page_head, flags);
		if (unlikely(!got))
			put_page(page_head);
	}
	return got;
}
EXPORT_SYMBOL(__get_page_tail);
=======
#include <linux/uio.h>
#include <linux/hugetlb.h>
#include <linux/page_idle.h>
#include <linux/local_lock.h>
#include <linux/buffer_head.h>

#include "internal.h"

#define CREATE_TRACE_POINTS
#include <trace/events/pagemap.h>

/* How many pages do we try to swap or page in/out together? As a power of 2 */
int page_cluster;
const int page_cluster_max = 31;

/* Protecting only lru_rotate.fbatch which requires disabling interrupts */
struct lru_rotate {
	local_lock_t lock;
	struct folio_batch fbatch;
};
static DEFINE_PER_CPU(struct lru_rotate, lru_rotate) = {
	.lock = INIT_LOCAL_LOCK(lock),
};

/*
 * The following folio batches are grouped together because they are protected
 * by disabling preemption (and interrupts remain enabled).
 */
struct cpu_fbatches {
	local_lock_t lock;
	struct folio_batch lru_add;
	struct folio_batch lru_deactivate_file;
	struct folio_batch lru_deactivate;
	struct folio_batch lru_lazyfree;
#ifdef CONFIG_SMP
	struct folio_batch activate;
#endif
};
static DEFINE_PER_CPU(struct cpu_fbatches, cpu_fbatches) = {
	.lock = INIT_LOCAL_LOCK(lock),
};

static void __page_cache_release(struct folio *folio, struct lruvec **lruvecp,
		unsigned long *flagsp)
{
	if (folio_test_lru(folio)) {
		folio_lruvec_relock_irqsave(folio, lruvecp, flagsp);
		lruvec_del_folio(*lruvecp, folio);
		__folio_clear_lru_flags(folio);
	}

	/*
	 * In rare cases, when truncation or holepunching raced with
	 * munlock after VM_LOCKED was cleared, Mlocked may still be
	 * found set here.  This does not indicate a problem, unless
	 * "unevictable_pgs_cleared" appears worryingly large.
	 */
	if (unlikely(folio_test_mlocked(folio))) {
		long nr_pages = folio_nr_pages(folio);

		__folio_clear_mlocked(folio);
		zone_stat_mod_folio(folio, NR_MLOCK, -nr_pages);
		count_vm_events(UNEVICTABLE_PGCLEARED, nr_pages);
	}
}

/*
 * This path almost never happens for VM activity - pages are normally freed
 * in batches.  But it gets used by networking - and for compound pages.
 */
static void page_cache_release(struct folio *folio)
{
	struct lruvec *lruvec = NULL;
	unsigned long flags;

	__page_cache_release(folio, &lruvec, &flags);
	if (lruvec)
		unlock_page_lruvec_irqrestore(lruvec, flags);
}

static void __folio_put_small(struct folio *folio)
{
	page_cache_release(folio);
	mem_cgroup_uncharge(folio);
	free_unref_page(&folio->page, 0);
}

static void __folio_put_large(struct folio *folio)
{
	/*
	 * __page_cache_release() is supposed to be called for thp, not for
	 * hugetlb. This is because hugetlb page does never have PageLRU set
	 * (it's never listed to any LRU lists) and no memcg routines should
	 * be called for hugetlb (it has a separate hugetlb_cgroup.)
	 */
	if (!folio_test_hugetlb(folio))
		page_cache_release(folio);
	destroy_large_folio(folio);
}

void __folio_put(struct folio *folio)
{
	if (unlikely(folio_is_zone_device(folio)))
		free_zone_device_page(&folio->page);
	else if (unlikely(folio_test_large(folio)))
		__folio_put_large(folio);
	else
		__folio_put_small(folio);
}
EXPORT_SYMBOL(__folio_put);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * put_pages_list() - release a list of pages
 * @pages: list of pages threaded on page->lru
 *
<<<<<<< HEAD
 * Release a list of pages which are strung together on page.lru.  Currently
 * used by read_cache_pages() and related error recovery code.
 */
void put_pages_list(struct list_head *pages)
{
	while (!list_empty(pages)) {
		struct page *victim;

		victim = list_entry(pages->prev, struct page, lru);
		list_del(&victim->lru);
		page_cache_release(victim);
	}
}
EXPORT_SYMBOL(put_pages_list);

static void pagevec_lru_move_fn(struct pagevec *pvec,
				void (*move_fn)(struct page *page, void *arg),
				void *arg)
{
	int i;
	struct zone *zone = NULL;
	unsigned long flags = 0;

	for (i = 0; i < pagevec_count(pvec); i++) {
		struct page *page = pvec->pages[i];
		struct zone *pagezone = page_zone(page);

		if (pagezone != zone) {
			if (zone)
				spin_unlock_irqrestore(&zone->lru_lock, flags);
			zone = pagezone;
			spin_lock_irqsave(&zone->lru_lock, flags);
		}

		(*move_fn)(page, arg);
	}
	if (zone)
		spin_unlock_irqrestore(&zone->lru_lock, flags);
	release_pages(pvec->pages, pvec->nr, pvec->cold);
	pagevec_reinit(pvec);
}

static void pagevec_move_tail_fn(struct page *page, void *arg)
{
	int *pgmoved = arg;

	if (PageLRU(page) && !PageActive(page) && !PageUnevictable(page)) {
		enum lru_list lru = page_lru_base_type(page);
		struct lruvec *lruvec;

		lruvec = mem_cgroup_lru_move_lists(page_zone(page),
						   page, lru, lru);
		list_move_tail(&page->lru, &lruvec->lists[lru]);
		(*pgmoved)++;
=======
 * Release a list of pages which are strung together on page.lru.
 */
void put_pages_list(struct list_head *pages)
{
	struct folio_batch fbatch;
	struct folio *folio, *next;

	folio_batch_init(&fbatch);
	list_for_each_entry_safe(folio, next, pages, lru) {
		if (!folio_put_testzero(folio))
			continue;
		if (folio_test_large(folio)) {
			__folio_put_large(folio);
			continue;
		}
		/* LRU flag must be clear because it's passed using the lru */
		if (folio_batch_add(&fbatch, folio) > 0)
			continue;
		free_unref_folios(&fbatch);
	}

	if (fbatch.nr)
		free_unref_folios(&fbatch);
	INIT_LIST_HEAD(pages);
}
EXPORT_SYMBOL(put_pages_list);

typedef void (*move_fn_t)(struct lruvec *lruvec, struct folio *folio);

static void lru_add_fn(struct lruvec *lruvec, struct folio *folio)
{
	int was_unevictable = folio_test_clear_unevictable(folio);
	long nr_pages = folio_nr_pages(folio);

	VM_BUG_ON_FOLIO(folio_test_lru(folio), folio);

	/*
	 * Is an smp_mb__after_atomic() still required here, before
	 * folio_evictable() tests the mlocked flag, to rule out the possibility
	 * of stranding an evictable folio on an unevictable LRU?  I think
	 * not, because __munlock_folio() only clears the mlocked flag
	 * while the LRU lock is held.
	 *
	 * (That is not true of __page_cache_release(), and not necessarily
	 * true of folios_put(): but those only clear the mlocked flag after
	 * folio_put_testzero() has excluded any other users of the folio.)
	 */
	if (folio_evictable(folio)) {
		if (was_unevictable)
			__count_vm_events(UNEVICTABLE_PGRESCUED, nr_pages);
	} else {
		folio_clear_active(folio);
		folio_set_unevictable(folio);
		/*
		 * folio->mlock_count = !!folio_test_mlocked(folio)?
		 * But that leaves __mlock_folio() in doubt whether another
		 * actor has already counted the mlock or not.  Err on the
		 * safe side, underestimate, let page reclaim fix it, rather
		 * than leaving a page on the unevictable LRU indefinitely.
		 */
		folio->mlock_count = 0;
		if (!was_unevictable)
			__count_vm_events(UNEVICTABLE_PGCULLED, nr_pages);
	}

	lruvec_add_folio(lruvec, folio);
	trace_mm_lru_insertion(folio);
}

static void folio_batch_move_lru(struct folio_batch *fbatch, move_fn_t move_fn)
{
	int i;
	struct lruvec *lruvec = NULL;
	unsigned long flags = 0;

	for (i = 0; i < folio_batch_count(fbatch); i++) {
		struct folio *folio = fbatch->folios[i];

		/* block memcg migration while the folio moves between lru */
		if (move_fn != lru_add_fn && !folio_test_clear_lru(folio))
			continue;

		folio_lruvec_relock_irqsave(folio, &lruvec, &flags);
		move_fn(lruvec, folio);

		folio_set_lru(folio);
	}

	if (lruvec)
		unlock_page_lruvec_irqrestore(lruvec, flags);
	folios_put(fbatch);
}

static void folio_batch_add_and_move(struct folio_batch *fbatch,
		struct folio *folio, move_fn_t move_fn)
{
	if (folio_batch_add(fbatch, folio) && !folio_test_large(folio) &&
	    !lru_cache_disabled())
		return;
	folio_batch_move_lru(fbatch, move_fn);
}

static void lru_move_tail_fn(struct lruvec *lruvec, struct folio *folio)
{
	if (!folio_test_unevictable(folio)) {
		lruvec_del_folio(lruvec, folio);
		folio_clear_active(folio);
		lruvec_add_folio_tail(lruvec, folio);
		__count_vm_events(PGROTATED, folio_nr_pages(folio));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
<<<<<<< HEAD
 * pagevec_move_tail() must be called with IRQ disabled.
 * Otherwise this may cause nasty races.
 */
static void pagevec_move_tail(struct pagevec *pvec)
{
	int pgmoved = 0;

	pagevec_lru_move_fn(pvec, pagevec_move_tail_fn, &pgmoved);
	__count_vm_events(PGROTATED, pgmoved);
}

/*
 * Writeback is about to end against a page which has been marked for immediate
 * reclaim.  If it still appears to be reclaimable, move it to the tail of the
 * inactive list.
 */
void rotate_reclaimable_page(struct page *page)
{
	if (!PageLocked(page) && !PageDirty(page) && !PageActive(page) &&
	    !PageUnevictable(page) && PageLRU(page)) {
		struct pagevec *pvec;
		unsigned long flags;

		page_cache_get(page);
		local_irq_save(flags);
		pvec = &__get_cpu_var(lru_rotate_pvecs);
		if (!pagevec_add(pvec, page))
			pagevec_move_tail(pvec);
		local_irq_restore(flags);
	}
}

static void update_page_reclaim_stat(struct zone *zone, struct page *page,
				     int file, int rotated)
{
	struct zone_reclaim_stat *reclaim_stat = &zone->reclaim_stat;
	struct zone_reclaim_stat *memcg_reclaim_stat;

	memcg_reclaim_stat = mem_cgroup_get_reclaim_stat_from_page(page);

	reclaim_stat->recent_scanned[file]++;
	if (rotated)
		reclaim_stat->recent_rotated[file]++;

	if (!memcg_reclaim_stat)
		return;

	memcg_reclaim_stat->recent_scanned[file]++;
	if (rotated)
		memcg_reclaim_stat->recent_rotated[file]++;
}

static void __activate_page(struct page *page, void *arg)
{
	struct zone *zone = page_zone(page);

	if (PageLRU(page) && !PageActive(page) && !PageUnevictable(page)) {
		int file = page_is_file_cache(page);
		int lru = page_lru_base_type(page);
		del_page_from_lru_list(zone, page, lru);

		SetPageActive(page);
		lru += LRU_ACTIVE;
		add_page_to_lru_list(zone, page, lru);
		__count_vm_event(PGACTIVATE);

		update_page_reclaim_stat(zone, page, file, 1);
=======
 * Writeback is about to end against a folio which has been marked for
 * immediate reclaim.  If it still appears to be reclaimable, move it
 * to the tail of the inactive list.
 *
 * folio_rotate_reclaimable() must disable IRQs, to prevent nasty races.
 */
void folio_rotate_reclaimable(struct folio *folio)
{
	if (!folio_test_locked(folio) && !folio_test_dirty(folio) &&
	    !folio_test_unevictable(folio) && folio_test_lru(folio)) {
		struct folio_batch *fbatch;
		unsigned long flags;

		folio_get(folio);
		local_lock_irqsave(&lru_rotate.lock, flags);
		fbatch = this_cpu_ptr(&lru_rotate.fbatch);
		folio_batch_add_and_move(fbatch, folio, lru_move_tail_fn);
		local_unlock_irqrestore(&lru_rotate.lock, flags);
	}
}

void lru_note_cost(struct lruvec *lruvec, bool file,
		   unsigned int nr_io, unsigned int nr_rotated)
{
	unsigned long cost;

	/*
	 * Reflect the relative cost of incurring IO and spending CPU
	 * time on rotations. This doesn't attempt to make a precise
	 * comparison, it just says: if reloads are about comparable
	 * between the LRU lists, or rotations are overwhelmingly
	 * different between them, adjust scan balance for CPU work.
	 */
	cost = nr_io * SWAP_CLUSTER_MAX + nr_rotated;

	do {
		unsigned long lrusize;

		/*
		 * Hold lruvec->lru_lock is safe here, since
		 * 1) The pinned lruvec in reclaim, or
		 * 2) From a pre-LRU page during refault (which also holds the
		 *    rcu lock, so would be safe even if the page was on the LRU
		 *    and could move simultaneously to a new lruvec).
		 */
		spin_lock_irq(&lruvec->lru_lock);
		/* Record cost event */
		if (file)
			lruvec->file_cost += cost;
		else
			lruvec->anon_cost += cost;

		/*
		 * Decay previous events
		 *
		 * Because workloads change over time (and to avoid
		 * overflow) we keep these statistics as a floating
		 * average, which ends up weighing recent refaults
		 * more than old ones.
		 */
		lrusize = lruvec_page_state(lruvec, NR_INACTIVE_ANON) +
			  lruvec_page_state(lruvec, NR_ACTIVE_ANON) +
			  lruvec_page_state(lruvec, NR_INACTIVE_FILE) +
			  lruvec_page_state(lruvec, NR_ACTIVE_FILE);

		if (lruvec->file_cost + lruvec->anon_cost > lrusize / 4) {
			lruvec->file_cost /= 2;
			lruvec->anon_cost /= 2;
		}
		spin_unlock_irq(&lruvec->lru_lock);
	} while ((lruvec = parent_lruvec(lruvec)));
}

void lru_note_cost_refault(struct folio *folio)
{
	lru_note_cost(folio_lruvec(folio), folio_is_file_lru(folio),
		      folio_nr_pages(folio), 0);
}

static void folio_activate_fn(struct lruvec *lruvec, struct folio *folio)
{
	if (!folio_test_active(folio) && !folio_test_unevictable(folio)) {
		long nr_pages = folio_nr_pages(folio);

		lruvec_del_folio(lruvec, folio);
		folio_set_active(folio);
		lruvec_add_folio(lruvec, folio);
		trace_mm_lru_activate(folio);

		__count_vm_events(PGACTIVATE, nr_pages);
		__count_memcg_events(lruvec_memcg(lruvec), PGACTIVATE,
				     nr_pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

#ifdef CONFIG_SMP
<<<<<<< HEAD
static DEFINE_PER_CPU(struct pagevec, activate_page_pvecs);

static void activate_page_drain(int cpu)
{
	struct pagevec *pvec = &per_cpu(activate_page_pvecs, cpu);

	if (pagevec_count(pvec))
		pagevec_lru_move_fn(pvec, __activate_page, NULL);
}

void activate_page(struct page *page)
{
	if (PageLRU(page) && !PageActive(page) && !PageUnevictable(page)) {
		struct pagevec *pvec = &get_cpu_var(activate_page_pvecs);

		page_cache_get(page);
		if (!pagevec_add(pvec, page))
			pagevec_lru_move_fn(pvec, __activate_page, NULL);
		put_cpu_var(activate_page_pvecs);
=======
static void folio_activate_drain(int cpu)
{
	struct folio_batch *fbatch = &per_cpu(cpu_fbatches.activate, cpu);

	if (folio_batch_count(fbatch))
		folio_batch_move_lru(fbatch, folio_activate_fn);
}

void folio_activate(struct folio *folio)
{
	if (folio_test_lru(folio) && !folio_test_active(folio) &&
	    !folio_test_unevictable(folio)) {
		struct folio_batch *fbatch;

		folio_get(folio);
		local_lock(&cpu_fbatches.lock);
		fbatch = this_cpu_ptr(&cpu_fbatches.activate);
		folio_batch_add_and_move(fbatch, folio, folio_activate_fn);
		local_unlock(&cpu_fbatches.lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

#else
<<<<<<< HEAD
static inline void activate_page_drain(int cpu)
{
}

void activate_page(struct page *page)
{
	struct zone *zone = page_zone(page);

	spin_lock_irq(&zone->lru_lock);
	__activate_page(page, NULL);
	spin_unlock_irq(&zone->lru_lock);
}
#endif

=======
static inline void folio_activate_drain(int cpu)
{
}

void folio_activate(struct folio *folio)
{
	struct lruvec *lruvec;

	if (folio_test_clear_lru(folio)) {
		lruvec = folio_lruvec_lock_irq(folio);
		folio_activate_fn(lruvec, folio);
		unlock_page_lruvec_irq(lruvec);
		folio_set_lru(folio);
	}
}
#endif

static void __lru_cache_activate_folio(struct folio *folio)
{
	struct folio_batch *fbatch;
	int i;

	local_lock(&cpu_fbatches.lock);
	fbatch = this_cpu_ptr(&cpu_fbatches.lru_add);

	/*
	 * Search backwards on the optimistic assumption that the folio being
	 * activated has just been added to this batch. Note that only
	 * the local batch is examined as a !LRU folio could be in the
	 * process of being released, reclaimed, migrated or on a remote
	 * batch that is currently being drained. Furthermore, marking
	 * a remote batch's folio active potentially hits a race where
	 * a folio is marked active just after it is added to the inactive
	 * list causing accounting errors and BUG_ON checks to trigger.
	 */
	for (i = folio_batch_count(fbatch) - 1; i >= 0; i--) {
		struct folio *batch_folio = fbatch->folios[i];

		if (batch_folio == folio) {
			folio_set_active(folio);
			break;
		}
	}

	local_unlock(&cpu_fbatches.lock);
}

#ifdef CONFIG_LRU_GEN
static void folio_inc_refs(struct folio *folio)
{
	unsigned long new_flags, old_flags = READ_ONCE(folio->flags);

	if (folio_test_unevictable(folio))
		return;

	if (!folio_test_referenced(folio)) {
		folio_set_referenced(folio);
		return;
	}

	if (!folio_test_workingset(folio)) {
		folio_set_workingset(folio);
		return;
	}

	/* see the comment on MAX_NR_TIERS */
	do {
		new_flags = old_flags & LRU_REFS_MASK;
		if (new_flags == LRU_REFS_MASK)
			break;

		new_flags += BIT(LRU_REFS_PGOFF);
		new_flags |= old_flags & ~LRU_REFS_MASK;
	} while (!try_cmpxchg(&folio->flags, &old_flags, new_flags));
}
#else
static void folio_inc_refs(struct folio *folio)
{
}
#endif /* CONFIG_LRU_GEN */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Mark a page as having seen activity.
 *
 * inactive,unreferenced	->	inactive,referenced
 * inactive,referenced		->	active,unreferenced
 * active,unreferenced		->	active,referenced
<<<<<<< HEAD
 */
void mark_page_accessed(struct page *page)
{
	if (!PageActive(page) && !PageUnevictable(page) &&
			PageReferenced(page) && PageLRU(page)) {
		activate_page(page);
		ClearPageReferenced(page);
	} else if (!PageReferenced(page)) {
		SetPageReferenced(page);
	}
}
EXPORT_SYMBOL(mark_page_accessed);

void __lru_cache_add(struct page *page, enum lru_list lru)
{
	struct pagevec *pvec = &get_cpu_var(lru_add_pvecs)[lru];

	page_cache_get(page);
	if (!pagevec_add(pvec, page))
		__pagevec_lru_add(pvec, lru);
	put_cpu_var(lru_add_pvecs);
}
EXPORT_SYMBOL(__lru_cache_add);

/**
 * lru_cache_add_lru - add a page to a page list
 * @page: the page to be added to the LRU.
 * @lru: the LRU list to which the page is added.
 */
void lru_cache_add_lru(struct page *page, enum lru_list lru)
{
	if (PageActive(page)) {
		VM_BUG_ON(PageUnevictable(page));
		ClearPageActive(page);
	} else if (PageUnevictable(page)) {
		VM_BUG_ON(PageActive(page));
		ClearPageUnevictable(page);
	}

	VM_BUG_ON(PageLRU(page) || PageActive(page) || PageUnevictable(page));
	__lru_cache_add(page, lru);
}

/**
 * add_page_to_unevictable_list - add a page to the unevictable list
 * @page:  the page to be added to the unevictable list
 *
 * Add page directly to its zone's unevictable list.  To avoid races with
 * tasks that might be making the page evictable, through eg. munlock,
 * munmap or exit, while it's not on the lru, we want to add the page
 * while it's locked or otherwise "invisible" to other tasks.  This is
 * difficult to do when using the pagevec cache, so bypass that.
 */
void add_page_to_unevictable_list(struct page *page)
{
	struct zone *zone = page_zone(page);

	spin_lock_irq(&zone->lru_lock);
	SetPageUnevictable(page);
	SetPageLRU(page);
	add_page_to_lru_list(zone, page, LRU_UNEVICTABLE);
	spin_unlock_irq(&zone->lru_lock);
}

/*
 * If the page can not be invalidated, it is moved to the
=======
 *
 * When a newly allocated page is not yet visible, so safe for non-atomic ops,
 * __SetPageReferenced(page) may be substituted for mark_page_accessed(page).
 */
void folio_mark_accessed(struct folio *folio)
{
	if (lru_gen_enabled()) {
		folio_inc_refs(folio);
		return;
	}

	if (!folio_test_referenced(folio)) {
		folio_set_referenced(folio);
	} else if (folio_test_unevictable(folio)) {
		/*
		 * Unevictable pages are on the "LRU_UNEVICTABLE" list. But,
		 * this list is never rotated or maintained, so marking an
		 * unevictable page accessed has no effect.
		 */
	} else if (!folio_test_active(folio)) {
		/*
		 * If the folio is on the LRU, queue it for activation via
		 * cpu_fbatches.activate. Otherwise, assume the folio is in a
		 * folio_batch, mark it active and it'll be moved to the active
		 * LRU on the next drain.
		 */
		if (folio_test_lru(folio))
			folio_activate(folio);
		else
			__lru_cache_activate_folio(folio);
		folio_clear_referenced(folio);
		workingset_activation(folio);
	}
	if (folio_test_idle(folio))
		folio_clear_idle(folio);
}
EXPORT_SYMBOL(folio_mark_accessed);

/**
 * folio_add_lru - Add a folio to an LRU list.
 * @folio: The folio to be added to the LRU.
 *
 * Queue the folio for addition to the LRU. The decision on whether
 * to add the page to the [in]active [file|anon] list is deferred until the
 * folio_batch is drained. This gives a chance for the caller of folio_add_lru()
 * have the folio added to the active list using folio_mark_accessed().
 */
void folio_add_lru(struct folio *folio)
{
	struct folio_batch *fbatch;

	VM_BUG_ON_FOLIO(folio_test_active(folio) &&
			folio_test_unevictable(folio), folio);
	VM_BUG_ON_FOLIO(folio_test_lru(folio), folio);

	/* see the comment in lru_gen_add_folio() */
	if (lru_gen_enabled() && !folio_test_unevictable(folio) &&
	    lru_gen_in_fault() && !(current->flags & PF_MEMALLOC))
		folio_set_active(folio);

	folio_get(folio);
	local_lock(&cpu_fbatches.lock);
	fbatch = this_cpu_ptr(&cpu_fbatches.lru_add);
	folio_batch_add_and_move(fbatch, folio, lru_add_fn);
	local_unlock(&cpu_fbatches.lock);
}
EXPORT_SYMBOL(folio_add_lru);

/**
 * folio_add_lru_vma() - Add a folio to the appropate LRU list for this VMA.
 * @folio: The folio to be added to the LRU.
 * @vma: VMA in which the folio is mapped.
 *
 * If the VMA is mlocked, @folio is added to the unevictable list.
 * Otherwise, it is treated the same way as folio_add_lru().
 */
void folio_add_lru_vma(struct folio *folio, struct vm_area_struct *vma)
{
	VM_BUG_ON_FOLIO(folio_test_lru(folio), folio);

	if (unlikely((vma->vm_flags & (VM_LOCKED | VM_SPECIAL)) == VM_LOCKED))
		mlock_new_folio(folio);
	else
		folio_add_lru(folio);
}

/*
 * If the folio cannot be invalidated, it is moved to the
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * inactive list to speed up its reclaim.  It is moved to the
 * head of the list, rather than the tail, to give the flusher
 * threads some time to write it out, as this is much more
 * effective than the single-page writeout from reclaim.
 *
<<<<<<< HEAD
 * If the page isn't page_mapped and dirty/writeback, the page
 * could reclaim asap using PG_reclaim.
 *
 * 1. active, mapped page -> none
 * 2. active, dirty/writeback page -> inactive, head, PG_reclaim
 * 3. inactive, mapped page -> none
 * 4. inactive, dirty/writeback page -> inactive, head, PG_reclaim
 * 5. inactive, clean -> inactive, tail
 * 6. Others -> none
 *
 * In 4, why it moves inactive's head, the VM expects the page would
 * be write it out by flusher threads as this is much more effective
 * than the single-page writeout from reclaim.
 */
static void lru_deactivate_fn(struct page *page, void *arg)
{
	int lru, file;
	bool active;
	struct zone *zone = page_zone(page);

	if (!PageLRU(page))
		return;

	if (PageUnevictable(page))
		return;

	/* Some processes are using the page */
	if (page_mapped(page))
		return;

	active = PageActive(page);

	file = page_is_file_cache(page);
	lru = page_lru_base_type(page);
	del_page_from_lru_list(zone, page, lru + active);
	ClearPageActive(page);
	ClearPageReferenced(page);
	add_page_to_lru_list(zone, page, lru);

	if (PageWriteback(page) || PageDirty(page)) {
		/*
		 * PG_reclaim could be raced with end_page_writeback
		 * It can make readahead confusing.  But race window
		 * is _really_ small and  it's non-critical problem.
		 */
		SetPageReclaim(page);
	} else {
		struct lruvec *lruvec;
		/*
		 * The page's writeback ends up during pagevec
		 * We moves tha page into tail of inactive.
		 */
		lruvec = mem_cgroup_lru_move_lists(zone, page, lru, lru);
		list_move_tail(&page->lru, &lruvec->lists[lru]);
		__count_vm_event(PGROTATED);
	}

	if (active)
		__count_vm_event(PGDEACTIVATE);
	update_page_reclaim_stat(zone, page, file, 0);
}

/*
 * Drain pages out of the cpu's pagevecs.
=======
 * If the folio isn't mapped and dirty/writeback, the folio
 * could be reclaimed asap using the reclaim flag.
 *
 * 1. active, mapped folio -> none
 * 2. active, dirty/writeback folio -> inactive, head, reclaim
 * 3. inactive, mapped folio -> none
 * 4. inactive, dirty/writeback folio -> inactive, head, reclaim
 * 5. inactive, clean -> inactive, tail
 * 6. Others -> none
 *
 * In 4, it moves to the head of the inactive list so the folio is
 * written out by flusher threads as this is much more efficient
 * than the single-page writeout from reclaim.
 */
static void lru_deactivate_file_fn(struct lruvec *lruvec, struct folio *folio)
{
	bool active = folio_test_active(folio);
	long nr_pages = folio_nr_pages(folio);

	if (folio_test_unevictable(folio))
		return;

	/* Some processes are using the folio */
	if (folio_mapped(folio))
		return;

	lruvec_del_folio(lruvec, folio);
	folio_clear_active(folio);
	folio_clear_referenced(folio);

	if (folio_test_writeback(folio) || folio_test_dirty(folio)) {
		/*
		 * Setting the reclaim flag could race with
		 * folio_end_writeback() and confuse readahead.  But the
		 * race window is _really_ small and  it's not a critical
		 * problem.
		 */
		lruvec_add_folio(lruvec, folio);
		folio_set_reclaim(folio);
	} else {
		/*
		 * The folio's writeback ended while it was in the batch.
		 * We move that folio to the tail of the inactive list.
		 */
		lruvec_add_folio_tail(lruvec, folio);
		__count_vm_events(PGROTATED, nr_pages);
	}

	if (active) {
		__count_vm_events(PGDEACTIVATE, nr_pages);
		__count_memcg_events(lruvec_memcg(lruvec), PGDEACTIVATE,
				     nr_pages);
	}
}

static void lru_deactivate_fn(struct lruvec *lruvec, struct folio *folio)
{
	if (!folio_test_unevictable(folio) && (folio_test_active(folio) || lru_gen_enabled())) {
		long nr_pages = folio_nr_pages(folio);

		lruvec_del_folio(lruvec, folio);
		folio_clear_active(folio);
		folio_clear_referenced(folio);
		lruvec_add_folio(lruvec, folio);

		__count_vm_events(PGDEACTIVATE, nr_pages);
		__count_memcg_events(lruvec_memcg(lruvec), PGDEACTIVATE,
				     nr_pages);
	}
}

static void lru_lazyfree_fn(struct lruvec *lruvec, struct folio *folio)
{
	if (folio_test_anon(folio) && folio_test_swapbacked(folio) &&
	    !folio_test_swapcache(folio) && !folio_test_unevictable(folio)) {
		long nr_pages = folio_nr_pages(folio);

		lruvec_del_folio(lruvec, folio);
		folio_clear_active(folio);
		folio_clear_referenced(folio);
		/*
		 * Lazyfree folios are clean anonymous folios.  They have
		 * the swapbacked flag cleared, to distinguish them from normal
		 * anonymous folios
		 */
		folio_clear_swapbacked(folio);
		lruvec_add_folio(lruvec, folio);

		__count_vm_events(PGLAZYFREE, nr_pages);
		__count_memcg_events(lruvec_memcg(lruvec), PGLAZYFREE,
				     nr_pages);
	}
}

/*
 * Drain pages out of the cpu's folio_batch.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Either "cpu" is the current CPU, and preemption has already been
 * disabled; or "cpu" is being hot-unplugged, and is already dead.
 */
void lru_add_drain_cpu(int cpu)
{
<<<<<<< HEAD
	struct pagevec *pvecs = per_cpu(lru_add_pvecs, cpu);
	struct pagevec *pvec;
	int lru;

	for_each_lru(lru) {
		pvec = &pvecs[lru - LRU_BASE];
		if (pagevec_count(pvec))
			__pagevec_lru_add(pvec, lru);
	}

	pvec = &per_cpu(lru_rotate_pvecs, cpu);
	if (pagevec_count(pvec)) {
		unsigned long flags;

		/* No harm done if a racing interrupt already did this */
		local_irq_save(flags);
		pagevec_move_tail(pvec);
		local_irq_restore(flags);
	}

	pvec = &per_cpu(lru_deactivate_pvecs, cpu);
	if (pagevec_count(pvec))
		pagevec_lru_move_fn(pvec, lru_deactivate_fn, NULL);

	activate_page_drain(cpu);
}

/**
 * deactivate_page - forcefully deactivate a page
 * @page: page to deactivate
 *
 * This function hints the VM that @page is a good reclaim candidate,
 * for example if its invalidation fails due to the page being dirty
 * or under writeback.
 */
void deactivate_page(struct page *page)
{
	/*
	 * In a workload with many unevictable page such as mprotect, unevictable
	 * page deactivation for accelerating reclaim is pointless.
	 */
	if (PageUnevictable(page))
		return;

	if (likely(get_page_unless_zero(page))) {
		struct pagevec *pvec = &get_cpu_var(lru_deactivate_pvecs);

		if (!pagevec_add(pvec, page))
			pagevec_lru_move_fn(pvec, lru_deactivate_fn, NULL);
		put_cpu_var(lru_deactivate_pvecs);
=======
	struct cpu_fbatches *fbatches = &per_cpu(cpu_fbatches, cpu);
	struct folio_batch *fbatch = &fbatches->lru_add;

	if (folio_batch_count(fbatch))
		folio_batch_move_lru(fbatch, lru_add_fn);

	fbatch = &per_cpu(lru_rotate.fbatch, cpu);
	/* Disabling interrupts below acts as a compiler barrier. */
	if (data_race(folio_batch_count(fbatch))) {
		unsigned long flags;

		/* No harm done if a racing interrupt already did this */
		local_lock_irqsave(&lru_rotate.lock, flags);
		folio_batch_move_lru(fbatch, lru_move_tail_fn);
		local_unlock_irqrestore(&lru_rotate.lock, flags);
	}

	fbatch = &fbatches->lru_deactivate_file;
	if (folio_batch_count(fbatch))
		folio_batch_move_lru(fbatch, lru_deactivate_file_fn);

	fbatch = &fbatches->lru_deactivate;
	if (folio_batch_count(fbatch))
		folio_batch_move_lru(fbatch, lru_deactivate_fn);

	fbatch = &fbatches->lru_lazyfree;
	if (folio_batch_count(fbatch))
		folio_batch_move_lru(fbatch, lru_lazyfree_fn);

	folio_activate_drain(cpu);
}

/**
 * deactivate_file_folio() - Deactivate a file folio.
 * @folio: Folio to deactivate.
 *
 * This function hints to the VM that @folio is a good reclaim candidate,
 * for example if its invalidation fails due to the folio being dirty
 * or under writeback.
 *
 * Context: Caller holds a reference on the folio.
 */
void deactivate_file_folio(struct folio *folio)
{
	struct folio_batch *fbatch;

	/* Deactivating an unevictable folio will not accelerate reclaim */
	if (folio_test_unevictable(folio))
		return;

	folio_get(folio);
	local_lock(&cpu_fbatches.lock);
	fbatch = this_cpu_ptr(&cpu_fbatches.lru_deactivate_file);
	folio_batch_add_and_move(fbatch, folio, lru_deactivate_file_fn);
	local_unlock(&cpu_fbatches.lock);
}

/*
 * folio_deactivate - deactivate a folio
 * @folio: folio to deactivate
 *
 * folio_deactivate() moves @folio to the inactive list if @folio was on the
 * active list and was not unevictable. This is done to accelerate the
 * reclaim of @folio.
 */
void folio_deactivate(struct folio *folio)
{
	if (folio_test_lru(folio) && !folio_test_unevictable(folio) &&
	    (folio_test_active(folio) || lru_gen_enabled())) {
		struct folio_batch *fbatch;

		folio_get(folio);
		local_lock(&cpu_fbatches.lock);
		fbatch = this_cpu_ptr(&cpu_fbatches.lru_deactivate);
		folio_batch_add_and_move(fbatch, folio, lru_deactivate_fn);
		local_unlock(&cpu_fbatches.lock);
	}
}

/**
 * folio_mark_lazyfree - make an anon folio lazyfree
 * @folio: folio to deactivate
 *
 * folio_mark_lazyfree() moves @folio to the inactive file list.
 * This is done to accelerate the reclaim of @folio.
 */
void folio_mark_lazyfree(struct folio *folio)
{
	if (folio_test_lru(folio) && folio_test_anon(folio) &&
	    folio_test_swapbacked(folio) && !folio_test_swapcache(folio) &&
	    !folio_test_unevictable(folio)) {
		struct folio_batch *fbatch;

		folio_get(folio);
		local_lock(&cpu_fbatches.lock);
		fbatch = this_cpu_ptr(&cpu_fbatches.lru_lazyfree);
		folio_batch_add_and_move(fbatch, folio, lru_lazyfree_fn);
		local_unlock(&cpu_fbatches.lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

void lru_add_drain(void)
{
<<<<<<< HEAD
	lru_add_drain_cpu(get_cpu());
	put_cpu();
}

static void lru_add_drain_per_cpu(struct work_struct *dummy)
{
	lru_add_drain();
}

/*
 * Returns 0 for success
 */
int lru_add_drain_all(void)
{
	return schedule_on_each_cpu(lru_add_drain_per_cpu);
}

/*
 * Batched page_cache_release().  Decrement the reference count on all the
 * passed pages.  If it fell to zero then remove the page from the LRU and
 * free it.
 *
 * Avoid taking zone->lru_lock if possible, but if it is taken, retain it
 * for the remainder of the operation.
 *
 * The locking in this function is against shrink_inactive_list(): we recheck
 * the page count inside the lock to see whether shrink_inactive_list()
 * grabbed the page via the LRU.  If it did, give up: shrink_inactive_list()
 * will free it.
 */
void release_pages(struct page **pages, int nr, int cold)
{
	int i;
	LIST_HEAD(pages_to_free);
	struct zone *zone = NULL;
	unsigned long uninitialized_var(flags);

	for (i = 0; i < nr; i++) {
		struct page *page = pages[i];

		if (unlikely(PageCompound(page))) {
			if (zone) {
				spin_unlock_irqrestore(&zone->lru_lock, flags);
				zone = NULL;
			}
			put_compound_page(page);
			continue;
		}

		if (!put_page_testzero(page))
			continue;

		if (PageLRU(page)) {
			struct zone *pagezone = page_zone(page);

			if (pagezone != zone) {
				if (zone)
					spin_unlock_irqrestore(&zone->lru_lock,
									flags);
				zone = pagezone;
				spin_lock_irqsave(&zone->lru_lock, flags);
			}
			VM_BUG_ON(!PageLRU(page));
			__ClearPageLRU(page);
			del_page_from_lru_list(zone, page, page_off_lru(page));
		}

		list_add(&page->lru, &pages_to_free);
	}
	if (zone)
		spin_unlock_irqrestore(&zone->lru_lock, flags);

	free_hot_cold_page_list(&pages_to_free, cold);
=======
	local_lock(&cpu_fbatches.lock);
	lru_add_drain_cpu(smp_processor_id());
	local_unlock(&cpu_fbatches.lock);
	mlock_drain_local();
}

/*
 * It's called from per-cpu workqueue context in SMP case so
 * lru_add_drain_cpu and invalidate_bh_lrus_cpu should run on
 * the same cpu. It shouldn't be a problem in !SMP case since
 * the core is only one and the locks will disable preemption.
 */
static void lru_add_and_bh_lrus_drain(void)
{
	local_lock(&cpu_fbatches.lock);
	lru_add_drain_cpu(smp_processor_id());
	local_unlock(&cpu_fbatches.lock);
	invalidate_bh_lrus_cpu();
	mlock_drain_local();
}

void lru_add_drain_cpu_zone(struct zone *zone)
{
	local_lock(&cpu_fbatches.lock);
	lru_add_drain_cpu(smp_processor_id());
	drain_local_pages(zone);
	local_unlock(&cpu_fbatches.lock);
	mlock_drain_local();
}

#ifdef CONFIG_SMP

static DEFINE_PER_CPU(struct work_struct, lru_add_drain_work);

static void lru_add_drain_per_cpu(struct work_struct *dummy)
{
	lru_add_and_bh_lrus_drain();
}

static bool cpu_needs_drain(unsigned int cpu)
{
	struct cpu_fbatches *fbatches = &per_cpu(cpu_fbatches, cpu);

	/* Check these in order of likelihood that they're not zero */
	return folio_batch_count(&fbatches->lru_add) ||
		data_race(folio_batch_count(&per_cpu(lru_rotate.fbatch, cpu))) ||
		folio_batch_count(&fbatches->lru_deactivate_file) ||
		folio_batch_count(&fbatches->lru_deactivate) ||
		folio_batch_count(&fbatches->lru_lazyfree) ||
		folio_batch_count(&fbatches->activate) ||
		need_mlock_drain(cpu) ||
		has_bh_in_lru(cpu, NULL);
}

/*
 * Doesn't need any cpu hotplug locking because we do rely on per-cpu
 * kworkers being shut down before our page_alloc_cpu_dead callback is
 * executed on the offlined cpu.
 * Calling this function with cpu hotplug locks held can actually lead
 * to obscure indirect dependencies via WQ context.
 */
static inline void __lru_add_drain_all(bool force_all_cpus)
{
	/*
	 * lru_drain_gen - Global pages generation number
	 *
	 * (A) Definition: global lru_drain_gen = x implies that all generations
	 *     0 < n <= x are already *scheduled* for draining.
	 *
	 * This is an optimization for the highly-contended use case where a
	 * user space workload keeps constantly generating a flow of pages for
	 * each CPU.
	 */
	static unsigned int lru_drain_gen;
	static struct cpumask has_work;
	static DEFINE_MUTEX(lock);
	unsigned cpu, this_gen;

	/*
	 * Make sure nobody triggers this path before mm_percpu_wq is fully
	 * initialized.
	 */
	if (WARN_ON(!mm_percpu_wq))
		return;

	/*
	 * Guarantee folio_batch counter stores visible by this CPU
	 * are visible to other CPUs before loading the current drain
	 * generation.
	 */
	smp_mb();

	/*
	 * (B) Locally cache global LRU draining generation number
	 *
	 * The read barrier ensures that the counter is loaded before the mutex
	 * is taken. It pairs with smp_mb() inside the mutex critical section
	 * at (D).
	 */
	this_gen = smp_load_acquire(&lru_drain_gen);

	mutex_lock(&lock);

	/*
	 * (C) Exit the draining operation if a newer generation, from another
	 * lru_add_drain_all(), was already scheduled for draining. Check (A).
	 */
	if (unlikely(this_gen != lru_drain_gen && !force_all_cpus))
		goto done;

	/*
	 * (D) Increment global generation number
	 *
	 * Pairs with smp_load_acquire() at (B), outside of the critical
	 * section. Use a full memory barrier to guarantee that the
	 * new global drain generation number is stored before loading
	 * folio_batch counters.
	 *
	 * This pairing must be done here, before the for_each_online_cpu loop
	 * below which drains the page vectors.
	 *
	 * Let x, y, and z represent some system CPU numbers, where x < y < z.
	 * Assume CPU #z is in the middle of the for_each_online_cpu loop
	 * below and has already reached CPU #y's per-cpu data. CPU #x comes
	 * along, adds some pages to its per-cpu vectors, then calls
	 * lru_add_drain_all().
	 *
	 * If the paired barrier is done at any later step, e.g. after the
	 * loop, CPU #x will just exit at (C) and miss flushing out all of its
	 * added pages.
	 */
	WRITE_ONCE(lru_drain_gen, lru_drain_gen + 1);
	smp_mb();

	cpumask_clear(&has_work);
	for_each_online_cpu(cpu) {
		struct work_struct *work = &per_cpu(lru_add_drain_work, cpu);

		if (cpu_needs_drain(cpu)) {
			INIT_WORK(work, lru_add_drain_per_cpu);
			queue_work_on(cpu, mm_percpu_wq, work);
			__cpumask_set_cpu(cpu, &has_work);
		}
	}

	for_each_cpu(cpu, &has_work)
		flush_work(&per_cpu(lru_add_drain_work, cpu));

done:
	mutex_unlock(&lock);
}

void lru_add_drain_all(void)
{
	__lru_add_drain_all(false);
}
#else
void lru_add_drain_all(void)
{
	lru_add_drain();
}
#endif /* CONFIG_SMP */

atomic_t lru_disable_count = ATOMIC_INIT(0);

/*
 * lru_cache_disable() needs to be called before we start compiling
 * a list of pages to be migrated using isolate_lru_page().
 * It drains pages on LRU cache and then disable on all cpus until
 * lru_cache_enable is called.
 *
 * Must be paired with a call to lru_cache_enable().
 */
void lru_cache_disable(void)
{
	atomic_inc(&lru_disable_count);
	/*
	 * Readers of lru_disable_count are protected by either disabling
	 * preemption or rcu_read_lock:
	 *
	 * preempt_disable, local_irq_disable  [bh_lru_lock()]
	 * rcu_read_lock		       [rt_spin_lock CONFIG_PREEMPT_RT]
	 * preempt_disable		       [local_lock !CONFIG_PREEMPT_RT]
	 *
	 * Since v5.1 kernel, synchronize_rcu() is guaranteed to wait on
	 * preempt_disable() regions of code. So any CPU which sees
	 * lru_disable_count = 0 will have exited the critical
	 * section when synchronize_rcu() returns.
	 */
	synchronize_rcu_expedited();
#ifdef CONFIG_SMP
	__lru_add_drain_all(true);
#else
	lru_add_and_bh_lrus_drain();
#endif
}

/**
 * folios_put_refs - Reduce the reference count on a batch of folios.
 * @folios: The folios.
 * @refs: The number of refs to subtract from each folio.
 *
 * Like folio_put(), but for a batch of folios.  This is more efficient
 * than writing the loop yourself as it will optimise the locks which need
 * to be taken if the folios are freed.  The folios batch is returned
 * empty and ready to be reused for another batch; there is no need
 * to reinitialise it.  If @refs is NULL, we subtract one from each
 * folio refcount.
 *
 * Context: May be called in process or interrupt context, but not in NMI
 * context.  May be called while holding a spinlock.
 */
void folios_put_refs(struct folio_batch *folios, unsigned int *refs)
{
	int i, j;
	struct lruvec *lruvec = NULL;
	unsigned long flags = 0;

	for (i = 0, j = 0; i < folios->nr; i++) {
		struct folio *folio = folios->folios[i];
		unsigned int nr_refs = refs ? refs[i] : 1;

		if (is_huge_zero_page(&folio->page))
			continue;

		if (folio_is_zone_device(folio)) {
			if (lruvec) {
				unlock_page_lruvec_irqrestore(lruvec, flags);
				lruvec = NULL;
			}
			if (put_devmap_managed_page_refs(&folio->page, nr_refs))
				continue;
			if (folio_ref_sub_and_test(folio, nr_refs))
				free_zone_device_page(&folio->page);
			continue;
		}

		if (!folio_ref_sub_and_test(folio, nr_refs))
			continue;

		/* hugetlb has its own memcg */
		if (folio_test_hugetlb(folio)) {
			if (lruvec) {
				unlock_page_lruvec_irqrestore(lruvec, flags);
				lruvec = NULL;
			}
			free_huge_folio(folio);
			continue;
		}
		if (folio_test_large(folio) &&
		    folio_test_large_rmappable(folio))
			folio_undo_large_rmappable(folio);

		__page_cache_release(folio, &lruvec, &flags);

		if (j != i)
			folios->folios[j] = folio;
		j++;
	}
	if (lruvec)
		unlock_page_lruvec_irqrestore(lruvec, flags);
	if (!j) {
		folio_batch_reinit(folios);
		return;
	}

	folios->nr = j;
	mem_cgroup_uncharge_folios(folios);
	free_unref_folios(folios);
}
EXPORT_SYMBOL(folios_put_refs);

/**
 * release_pages - batched put_page()
 * @arg: array of pages to release
 * @nr: number of pages
 *
 * Decrement the reference count on all the pages in @arg.  If it
 * fell to zero, remove the page from the LRU and free it.
 *
 * Note that the argument can be an array of pages, encoded pages,
 * or folio pointers. We ignore any encoded bits, and turn any of
 * them into just a folio that gets free'd.
 */
void release_pages(release_pages_arg arg, int nr)
{
	struct folio_batch fbatch;
	int refs[PAGEVEC_SIZE];
	struct encoded_page **encoded = arg.encoded_pages;
	int i;

	folio_batch_init(&fbatch);
	for (i = 0; i < nr; i++) {
		/* Turn any of the argument types into a folio */
		struct folio *folio = page_folio(encoded_page_ptr(encoded[i]));

		/* Is our next entry actually "nr_pages" -> "nr_refs" ? */
		refs[fbatch.nr] = 1;
		if (unlikely(encoded_page_flags(encoded[i]) &
			     ENCODED_PAGE_BIT_NR_PAGES_NEXT))
			refs[fbatch.nr] = encoded_nr_pages(encoded[++i]);

		if (folio_batch_add(&fbatch, folio) > 0)
			continue;
		folios_put_refs(&fbatch, refs);
	}

	if (fbatch.nr)
		folios_put_refs(&fbatch, refs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(release_pages);

/*
<<<<<<< HEAD
 * The pages which we're about to release may be in the deferred lru-addition
 * queues.  That would prevent them from really being freed right now.  That's
 * OK from a correctness point of view but is inefficient - those pages may be
 * cache-warm and we want to give them back to the page allocator ASAP.
 *
 * So __pagevec_release() will drain those queues here.  __pagevec_lru_add()
 * and __pagevec_lru_add_active() call release_pages() directly to avoid
 * mutual recursion.
 */
void __pagevec_release(struct pagevec *pvec)
{
	lru_add_drain();
	release_pages(pvec->pages, pagevec_count(pvec), pvec->cold);
	pagevec_reinit(pvec);
}
EXPORT_SYMBOL(__pagevec_release);

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
/* used by __split_huge_page_refcount() */
void lru_add_page_tail(struct zone* zone,
		       struct page *page, struct page *page_tail)
{
	int uninitialized_var(active);
	enum lru_list lru;
	const int file = 0;

	VM_BUG_ON(!PageHead(page));
	VM_BUG_ON(PageCompound(page_tail));
	VM_BUG_ON(PageLRU(page_tail));
	VM_BUG_ON(NR_CPUS != 1 && !spin_is_locked(&zone->lru_lock));

	SetPageLRU(page_tail);

	if (page_evictable(page_tail, NULL)) {
		if (PageActive(page)) {
			SetPageActive(page_tail);
			active = 1;
			lru = LRU_ACTIVE_ANON;
		} else {
			active = 0;
			lru = LRU_INACTIVE_ANON;
		}
	} else {
		SetPageUnevictable(page_tail);
		lru = LRU_UNEVICTABLE;
	}

	if (likely(PageLRU(page)))
		list_add_tail(&page_tail->lru, &page->lru);
	else {
		struct list_head *list_head;
		/*
		 * Head page has not yet been counted, as an hpage,
		 * so we must account for each subpage individually.
		 *
		 * Use the standard add function to put page_tail on the list,
		 * but then correct its position so they all end up in order.
		 */
		add_page_to_lru_list(zone, page_tail, lru);
		list_head = page_tail->lru.prev;
		list_move_tail(&page_tail->lru, list_head);
	}

	if (!PageUnevictable(page))
		update_page_reclaim_stat(zone, page_tail, file, active);
}
#endif /* CONFIG_TRANSPARENT_HUGEPAGE */

static void __pagevec_lru_add_fn(struct page *page, void *arg)
{
	enum lru_list lru = (enum lru_list)arg;
	struct zone *zone = page_zone(page);
	int file = is_file_lru(lru);
	int active = is_active_lru(lru);

	VM_BUG_ON(PageActive(page));
	VM_BUG_ON(PageUnevictable(page));
	VM_BUG_ON(PageLRU(page));

	SetPageLRU(page);
	if (active)
		SetPageActive(page);
	add_page_to_lru_list(zone, page, lru);
	update_page_reclaim_stat(zone, page, file, active);
}

/*
 * Add the passed pages to the LRU, then drop the caller's refcount
 * on them.  Reinitialises the caller's pagevec.
 */
void __pagevec_lru_add(struct pagevec *pvec, enum lru_list lru)
{
	VM_BUG_ON(is_unevictable_lru(lru));

	pagevec_lru_move_fn(pvec, __pagevec_lru_add_fn, (void *)lru);
}
EXPORT_SYMBOL(__pagevec_lru_add);

/**
 * pagevec_lookup - gang pagecache lookup
 * @pvec:	Where the resulting pages are placed
 * @mapping:	The address_space to search
 * @start:	The starting page index
 * @nr_pages:	The maximum number of pages
 *
 * pagevec_lookup() will search for and return a group of up to @nr_pages pages
 * in the mapping.  The pages are placed in @pvec.  pagevec_lookup() takes a
 * reference against the pages in @pvec.
 *
 * The search returns a group of mapping-contiguous pages with ascending
 * indexes.  There may be holes in the indices due to not-present pages.
 *
 * pagevec_lookup() returns the number of pages which were found.
 */
unsigned pagevec_lookup(struct pagevec *pvec, struct address_space *mapping,
		pgoff_t start, unsigned nr_pages)
{
	pvec->nr = find_get_pages(mapping, start, nr_pages, pvec->pages);
	return pagevec_count(pvec);
}
EXPORT_SYMBOL(pagevec_lookup);

unsigned pagevec_lookup_tag(struct pagevec *pvec, struct address_space *mapping,
		pgoff_t *index, int tag, unsigned nr_pages)
{
	pvec->nr = find_get_pages_tag(mapping, index, tag,
					nr_pages, pvec->pages);
	return pagevec_count(pvec);
}
EXPORT_SYMBOL(pagevec_lookup_tag);
=======
 * The folios which we're about to release may be in the deferred lru-addition
 * queues.  That would prevent them from really being freed right now.  That's
 * OK from a correctness point of view but is inefficient - those folios may be
 * cache-warm and we want to give them back to the page allocator ASAP.
 *
 * So __folio_batch_release() will drain those queues here.
 * folio_batch_move_lru() calls folios_put() directly to avoid
 * mutual recursion.
 */
void __folio_batch_release(struct folio_batch *fbatch)
{
	if (!fbatch->percpu_pvec_drained) {
		lru_add_drain();
		fbatch->percpu_pvec_drained = true;
	}
	folios_put(fbatch);
}
EXPORT_SYMBOL(__folio_batch_release);

/**
 * folio_batch_remove_exceptionals() - Prune non-folios from a batch.
 * @fbatch: The batch to prune
 *
 * find_get_entries() fills a batch with both folios and shadow/swap/DAX
 * entries.  This function prunes all the non-folio entries from @fbatch
 * without leaving holes, so that it can be passed on to folio-only batch
 * operations.
 */
void folio_batch_remove_exceptionals(struct folio_batch *fbatch)
{
	unsigned int i, j;

	for (i = 0, j = 0; i < folio_batch_count(fbatch); i++) {
		struct folio *folio = fbatch->folios[i];
		if (!xa_is_value(folio))
			fbatch->folios[j++] = folio;
	}
	fbatch->nr = j;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Perform any setup for the swap system
 */
void __init swap_setup(void)
{
<<<<<<< HEAD
	unsigned long megs = totalram_pages >> (20 - PAGE_SHIFT);

#ifdef CONFIG_SWAP
	bdi_init(swapper_space.backing_dev_info);
#endif
=======
	unsigned long megs = totalram_pages() >> (20 - PAGE_SHIFT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Use a smaller cluster for small-memory machines */
	if (megs < 16)
		page_cluster = 2;
	else
		page_cluster = 3;
	/*
	 * Right now other parts of the system means that we
	 * _really_ don't want to cluster much more
	 */
}
