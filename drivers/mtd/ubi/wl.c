<<<<<<< HEAD
/*
 * @ubi: UBI device description object
 * Copyright (c) International Business Machines Corp., 2006
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) International Business Machines Corp., 2006
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors: Artem Bityutskiy (Битюцкий Артём), Thomas Gleixner
 */

/*
 * UBI wear-leveling sub-system.
 *
 * This sub-system is responsible for wear-leveling. It works in terms of
 * physical eraseblocks and erase counters and knows nothing about logical
 * eraseblocks, volumes, etc. From this sub-system's perspective all physical
 * eraseblocks are of two types - used and free. Used physical eraseblocks are
 * those that were "get" by the 'ubi_wl_get_peb()' function, and free physical
 * eraseblocks are those that were put by the 'ubi_wl_put_peb()' function.
 *
 * Physical eraseblocks returned by 'ubi_wl_get_peb()' have only erase counter
 * header. The rest of the physical eraseblock contains only %0xFF bytes.
 *
 * When physical eraseblocks are returned to the WL sub-system by means of the
 * 'ubi_wl_put_peb()' function, they are scheduled for erasure. The erasure is
 * done asynchronously in context of the per-UBI device background thread,
 * which is also managed by the WL sub-system.
 *
 * The wear-leveling is ensured by means of moving the contents of used
 * physical eraseblocks with low erase counter to free physical eraseblocks
 * with high erase counter.
 *
<<<<<<< HEAD
 * The 'ubi_wl_get_peb()' function accepts data type hints which help to pick
 * an "optimal" physical eraseblock. For example, when it is known that the
 * physical eraseblock will be "put" soon because it contains short-term data,
 * the WL sub-system may pick a free physical eraseblock with low erase
 * counter, and so forth.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * If the WL sub-system fails to erase a physical eraseblock, it marks it as
 * bad.
 *
 * This sub-system is also responsible for scrubbing. If a bit-flip is detected
 * in a physical eraseblock, it has to be moved. Technically this is the same
 * as moving it for wear-leveling reasons.
 *
 * As it was said, for the UBI sub-system all physical eraseblocks are either
 * "free" or "used". Free eraseblock are kept in the @wl->free RB-tree, while
 * used eraseblocks are kept in @wl->used, @wl->erroneous, or @wl->scrub
 * RB-trees, as well as (temporarily) in the @wl->pq queue.
 *
 * When the WL sub-system returns a physical eraseblock, the physical
 * eraseblock is protected from being moved for some "time". For this reason,
 * the physical eraseblock is not directly moved from the @wl->free tree to the
 * @wl->used tree. There is a protection queue in between where this
 * physical eraseblock is temporarily stored (@wl->pq).
 *
 * All this protection stuff is needed because:
 *  o we don't want to move physical eraseblocks just after we have given them
 *    to the user; instead, we first want to let users fill them up with data;
 *
 *  o there is a chance that the user will put the physical eraseblock very
<<<<<<< HEAD
 *    soon, so it makes sense not to move it for some time, but wait; this is
 *    especially important in case of "short term" physical eraseblocks.
=======
 *    soon, so it makes sense not to move it for some time, but wait.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Physical eraseblocks stay protected only for limited time. But the "time" is
 * measured in erase cycles in this case. This is implemented with help of the
 * protection queue. Eraseblocks are put to the tail of this queue when they
 * are returned by the 'ubi_wl_get_peb()', and eraseblocks are removed from the
 * head of the queue on each erase operation (for any eraseblock). So the
 * length of the queue defines how may (global) erase cycles PEBs are protected.
 *
 * To put it differently, each physical eraseblock has 2 main states: free and
 * used. The former state corresponds to the @wl->free tree. The latter state
 * is split up on several sub-states:
 * o the WL movement is allowed (@wl->used tree);
 * o the WL movement is disallowed (@wl->erroneous) because the PEB is
 *   erroneous - e.g., there was a read error;
 * o the WL movement is temporarily prohibited (@wl->pq queue);
 * o scrubbing is needed (@wl->scrub tree).
 *
 * Depending on the sub-state, wear-leveling entries of the used physical
 * eraseblocks may be kept in one of those structures.
 *
 * Note, in this implementation, we keep a small in-RAM object for each physical
 * eraseblock. This is surely not a scalable solution. But it appears to be good
 * enough for moderately large flashes and it is simple. In future, one may
 * re-work this sub-system and make it more scalable.
 *
 * At the moment this sub-system does not utilize the sequence number, which
 * was introduced relatively recently. But it would be wise to do this because
 * the sequence number of a logical eraseblock characterizes how old is it. For
 * example, when we move a PEB with low erase counter, and we need to pick the
 * target PEB, we pick a PEB with the highest EC if our PEB is "old" and we
 * pick target PEB with an average EC if our PEB is not very "old". This is a
 * room for future re-works of the WL sub-system.
 */

#include <linux/slab.h>
#include <linux/crc32.h>
#include <linux/freezer.h>
#include <linux/kthread.h>
#include "ubi.h"
<<<<<<< HEAD
=======
#include "wl.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Number of physical eraseblocks reserved for wear-leveling purposes */
#define WL_RESERVED_PEBS 1

/*
 * Maximum difference between two erase counters. If this threshold is
 * exceeded, the WL sub-system starts moving data from used physical
 * eraseblocks with low erase counter to free physical eraseblocks with high
 * erase counter.
 */
#define UBI_WL_THRESHOLD CONFIG_MTD_UBI_WL_THRESHOLD

/*
 * When a physical eraseblock is moved, the WL sub-system has to pick the target
 * physical eraseblock to move to. The simplest way would be just to pick the
 * one with the highest erase counter. But in certain workloads this could lead
 * to an unlimited wear of one or few physical eraseblock. Indeed, imagine a
 * situation when the picked physical eraseblock is constantly erased after the
 * data is written to it. So, we have a constant which limits the highest erase
 * counter of the free physical eraseblock to pick. Namely, the WL sub-system
 * does not pick eraseblocks with erase counter greater than the lowest erase
 * counter plus %WL_FREE_MAX_DIFF.
 */
#define WL_FREE_MAX_DIFF (2*UBI_WL_THRESHOLD)

/*
 * Maximum number of consecutive background thread failures which is enough to
 * switch to read-only mode.
 */
#define WL_MAX_FAILURES 32

<<<<<<< HEAD
/**
 * struct ubi_work - UBI work description data structure.
 * @list: a link in the list of pending works
 * @func: worker function
 * @e: physical eraseblock to erase
 * @torture: if the physical eraseblock has to be tortured
 *
 * The @func pointer points to the worker function. If the @cancel argument is
 * not zero, the worker has to free the resources and exit immediately. The
 * worker has to return zero in case of success and a negative error code in
 * case of failure.
 */
struct ubi_work {
	struct list_head list;
	int (*func)(struct ubi_device *ubi, struct ubi_work *wrk, int cancel);
	/* The below fields are only relevant to erasure works */
	struct ubi_wl_entry *e;
	int torture;
};

#ifdef CONFIG_MTD_UBI_DEBUG
static int paranoid_check_ec(struct ubi_device *ubi, int pnum, int ec);
static int paranoid_check_in_wl_tree(const struct ubi_device *ubi,
				     struct ubi_wl_entry *e,
				     struct rb_root *root);
static int paranoid_check_in_pq(const struct ubi_device *ubi,
				struct ubi_wl_entry *e);
#else
#define paranoid_check_ec(ubi, pnum, ec) 0
#define paranoid_check_in_wl_tree(ubi, e, root)
#define paranoid_check_in_pq(ubi, e) 0
#endif
=======
static int self_check_ec(struct ubi_device *ubi, int pnum, int ec);
static int self_check_in_wl_tree(const struct ubi_device *ubi,
				 struct ubi_wl_entry *e, struct rb_root *root);
static int self_check_in_pq(const struct ubi_device *ubi,
			    struct ubi_wl_entry *e);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * wl_tree_add - add a wear-leveling entry to a WL RB-tree.
 * @e: the wear-leveling entry to add
 * @root: the root of the tree
 *
 * Note, we use (erase counter, physical eraseblock number) pairs as keys in
 * the @ubi->used and @ubi->free RB-trees.
 */
static void wl_tree_add(struct ubi_wl_entry *e, struct rb_root *root)
{
	struct rb_node **p, *parent = NULL;

	p = &root->rb_node;
	while (*p) {
		struct ubi_wl_entry *e1;

		parent = *p;
		e1 = rb_entry(parent, struct ubi_wl_entry, u.rb);

		if (e->ec < e1->ec)
			p = &(*p)->rb_left;
		else if (e->ec > e1->ec)
			p = &(*p)->rb_right;
		else {
			ubi_assert(e->pnum != e1->pnum);
			if (e->pnum < e1->pnum)
				p = &(*p)->rb_left;
			else
				p = &(*p)->rb_right;
		}
	}

	rb_link_node(&e->u.rb, parent, p);
	rb_insert_color(&e->u.rb, root);
}

/**
<<<<<<< HEAD
 * do_work - do one pending work.
 * @ubi: UBI device description object
 *
 * This function returns zero in case of success and a negative error code in
 * case of failure.
 */
static int do_work(struct ubi_device *ubi)
=======
 * wl_entry_destroy - destroy a wear-leveling entry.
 * @ubi: UBI device description object
 * @e: the wear-leveling entry to add
 *
 * This function destroys a wear leveling entry and removes
 * the reference from the lookup table.
 */
static void wl_entry_destroy(struct ubi_device *ubi, struct ubi_wl_entry *e)
{
	ubi->lookuptbl[e->pnum] = NULL;
	kmem_cache_free(ubi_wl_entry_slab, e);
}

/**
 * do_work - do one pending work.
 * @ubi: UBI device description object
 * @executed: whether there is one work is executed
 *
 * This function returns zero in case of success and a negative error code in
 * case of failure. If @executed is not NULL and there is one work executed,
 * @executed is set as %1, otherwise @executed is set as %0.
 */
static int do_work(struct ubi_device *ubi, int *executed)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	struct ubi_work *wrk;

	cond_resched();

	/*
	 * @ubi->work_sem is used to synchronize with the workers. Workers take
	 * it in read mode, so many of them may be doing works at a time. But
	 * the queue flush code has to be sure the whole queue of works is
	 * done, and it takes the mutex in write mode.
	 */
	down_read(&ubi->work_sem);
	spin_lock(&ubi->wl_lock);
	if (list_empty(&ubi->works)) {
		spin_unlock(&ubi->wl_lock);
		up_read(&ubi->work_sem);
<<<<<<< HEAD
		return 0;
	}

=======
		if (executed)
			*executed = 0;
		return 0;
	}

	if (executed)
		*executed = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wrk = list_entry(ubi->works.next, struct ubi_work, list);
	list_del(&wrk->list);
	ubi->works_count -= 1;
	ubi_assert(ubi->works_count >= 0);
	spin_unlock(&ubi->wl_lock);

	/*
	 * Call the worker function. Do not touch the work structure
	 * after this call as it will have been freed or reused by that
	 * time by the worker function.
	 */
	err = wrk->func(ubi, wrk, 0);
	if (err)
<<<<<<< HEAD
		ubi_err("work failed with error code %d", err);
=======
		ubi_err(ubi, "work failed with error code %d", err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	up_read(&ubi->work_sem);

	return err;
}

/**
<<<<<<< HEAD
 * produce_free_peb - produce a free physical eraseblock.
 * @ubi: UBI device description object
 *
 * This function tries to make a free PEB by means of synchronous execution of
 * pending works. This may be needed if, for example the background thread is
 * disabled. Returns zero in case of success and a negative error code in case
 * of failure.
 */
static int produce_free_peb(struct ubi_device *ubi)
{
	int err;

	spin_lock(&ubi->wl_lock);
	while (!ubi->free.rb_node) {
		spin_unlock(&ubi->wl_lock);

		dbg_wl("do one work synchronously");
		err = do_work(ubi);
		if (err)
			return err;

		spin_lock(&ubi->wl_lock);
	}
	spin_unlock(&ubi->wl_lock);

	return 0;
}

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * in_wl_tree - check if wear-leveling entry is present in a WL RB-tree.
 * @e: the wear-leveling entry to check
 * @root: the root of the tree
 *
 * This function returns non-zero if @e is in the @root RB-tree and zero if it
 * is not.
 */
static int in_wl_tree(struct ubi_wl_entry *e, struct rb_root *root)
{
	struct rb_node *p;

	p = root->rb_node;
	while (p) {
		struct ubi_wl_entry *e1;

		e1 = rb_entry(p, struct ubi_wl_entry, u.rb);

		if (e->pnum == e1->pnum) {
			ubi_assert(e == e1);
			return 1;
		}

		if (e->ec < e1->ec)
			p = p->rb_left;
		else if (e->ec > e1->ec)
			p = p->rb_right;
		else {
			ubi_assert(e->pnum != e1->pnum);
			if (e->pnum < e1->pnum)
				p = p->rb_left;
			else
				p = p->rb_right;
		}
	}

	return 0;
}

/**
<<<<<<< HEAD
=======
 * in_pq - check if a wear-leveling entry is present in the protection queue.
 * @ubi: UBI device description object
 * @e: the wear-leveling entry to check
 *
 * This function returns non-zero if @e is in the protection queue and zero
 * if it is not.
 */
static inline int in_pq(const struct ubi_device *ubi, struct ubi_wl_entry *e)
{
	struct ubi_wl_entry *p;
	int i;

	for (i = 0; i < UBI_PROT_QUEUE_LEN; ++i)
		list_for_each_entry(p, &ubi->pq[i], u.list)
			if (p == e)
				return 1;

	return 0;
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * prot_queue_add - add physical eraseblock to the protection queue.
 * @ubi: UBI device description object
 * @e: the physical eraseblock to add
 *
 * This function adds @e to the tail of the protection queue @ubi->pq, where
 * @e will stay for %UBI_PROT_QUEUE_LEN erase operations and will be
 * temporarily protected from the wear-leveling worker. Note, @wl->lock has to
 * be locked.
 */
static void prot_queue_add(struct ubi_device *ubi, struct ubi_wl_entry *e)
{
	int pq_tail = ubi->pq_head - 1;

	if (pq_tail < 0)
		pq_tail = UBI_PROT_QUEUE_LEN - 1;
	ubi_assert(pq_tail >= 0 && pq_tail < UBI_PROT_QUEUE_LEN);
	list_add_tail(&e->u.list, &ubi->pq[pq_tail]);
	dbg_wl("added PEB %d EC %d to the protection queue", e->pnum, e->ec);
}

/**
 * find_wl_entry - find wear-leveling entry closest to certain erase counter.
<<<<<<< HEAD
 * @root: the RB-tree where to look for
 * @diff: maximum possible difference from the smallest erase counter
=======
 * @ubi: UBI device description object
 * @root: the RB-tree where to look for
 * @diff: maximum possible difference from the smallest erase counter
 * @pick_max: pick PEB even its erase counter beyonds 'min_ec + @diff'
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function looks for a wear leveling entry with erase counter closest to
 * min + @diff, where min is the smallest erase counter.
 */
<<<<<<< HEAD
static struct ubi_wl_entry *find_wl_entry(struct rb_root *root, int diff)
=======
static struct ubi_wl_entry *find_wl_entry(struct ubi_device *ubi,
					  struct rb_root *root, int diff,
					  int pick_max)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rb_node *p;
	struct ubi_wl_entry *e;
	int max;

	e = rb_entry(rb_first(root), struct ubi_wl_entry, u.rb);
	max = e->ec + diff;

	p = root->rb_node;
	while (p) {
		struct ubi_wl_entry *e1;

		e1 = rb_entry(p, struct ubi_wl_entry, u.rb);
<<<<<<< HEAD
		if (e1->ec >= max)
			p = p->rb_left;
		else {
=======
		if (e1->ec >= max) {
			if (pick_max)
				e = e1;
			p = p->rb_left;
		} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			p = p->rb_right;
			e = e1;
		}
	}

	return e;
}

/**
<<<<<<< HEAD
 * ubi_wl_get_peb - get a physical eraseblock.
 * @ubi: UBI device description object
 * @dtype: type of data which will be stored in this physical eraseblock
 *
 * This function returns a physical eraseblock in case of success and a
 * negative error code in case of failure. Might sleep.
 */
int ubi_wl_get_peb(struct ubi_device *ubi, int dtype)
{
	int err;
	struct ubi_wl_entry *e, *first, *last;

	ubi_assert(dtype == UBI_LONGTERM || dtype == UBI_SHORTTERM ||
		   dtype == UBI_UNKNOWN);

retry:
	spin_lock(&ubi->wl_lock);
	if (!ubi->free.rb_node) {
		if (ubi->works_count == 0) {
			ubi_assert(list_empty(&ubi->works));
			ubi_err("no free eraseblocks");
			spin_unlock(&ubi->wl_lock);
			return -ENOSPC;
		}
		spin_unlock(&ubi->wl_lock);

		err = produce_free_peb(ubi);
		if (err < 0)
			return err;
		goto retry;
	}

	switch (dtype) {
	case UBI_LONGTERM:
		/*
		 * For long term data we pick a physical eraseblock with high
		 * erase counter. But the highest erase counter we can pick is
		 * bounded by the the lowest erase counter plus
		 * %WL_FREE_MAX_DIFF.
		 */
		e = find_wl_entry(&ubi->free, WL_FREE_MAX_DIFF);
		break;
	case UBI_UNKNOWN:
		/*
		 * For unknown data we pick a physical eraseblock with medium
		 * erase counter. But we by no means can pick a physical
		 * eraseblock with erase counter greater or equivalent than the
		 * lowest erase counter plus %WL_FREE_MAX_DIFF/2.
		 */
		first = rb_entry(rb_first(&ubi->free), struct ubi_wl_entry,
					u.rb);
		last = rb_entry(rb_last(&ubi->free), struct ubi_wl_entry, u.rb);

		if (last->ec - first->ec < WL_FREE_MAX_DIFF)
			e = rb_entry(ubi->free.rb_node,
					struct ubi_wl_entry, u.rb);
		else
			e = find_wl_entry(&ubi->free, WL_FREE_MAX_DIFF/2);
		break;
	case UBI_SHORTTERM:
		/*
		 * For short term data we pick a physical eraseblock with the
		 * lowest erase counter as we expect it will be erased soon.
		 */
		e = rb_entry(rb_first(&ubi->free), struct ubi_wl_entry, u.rb);
		break;
	default:
		BUG();
	}

	paranoid_check_in_wl_tree(ubi, e, &ubi->free);
=======
 * find_mean_wl_entry - find wear-leveling entry with medium erase counter.
 * @ubi: UBI device description object
 * @root: the RB-tree where to look for
 *
 * This function looks for a wear leveling entry with medium erase counter,
 * but not greater or equivalent than the lowest erase counter plus
 * %WL_FREE_MAX_DIFF/2.
 */
static struct ubi_wl_entry *find_mean_wl_entry(struct ubi_device *ubi,
					       struct rb_root *root)
{
	struct ubi_wl_entry *e, *first, *last;

	first = rb_entry(rb_first(root), struct ubi_wl_entry, u.rb);
	last = rb_entry(rb_last(root), struct ubi_wl_entry, u.rb);

	if (last->ec - first->ec < WL_FREE_MAX_DIFF) {
		e = rb_entry(root->rb_node, struct ubi_wl_entry, u.rb);

		/*
		 * If no fastmap has been written and fm_anchor is not
		 * reserved and this WL entry can be used as anchor PEB
		 * hold it back and return the second best WL entry such
		 * that fastmap can use the anchor PEB later.
		 */
		e = may_reserve_for_fm(ubi, e, root);
	} else
		e = find_wl_entry(ubi, root, WL_FREE_MAX_DIFF/2, 0);

	return e;
}

/**
 * wl_get_wle - get a mean wl entry to be used by ubi_wl_get_peb() or
 * refill_wl_user_pool().
 * @ubi: UBI device description object
 *
 * This function returns a wear leveling entry in case of success and
 * NULL in case of failure.
 */
static struct ubi_wl_entry *wl_get_wle(struct ubi_device *ubi)
{
	struct ubi_wl_entry *e;

	e = find_mean_wl_entry(ubi, &ubi->free);
	if (!e) {
		ubi_err(ubi, "no free eraseblocks");
		return NULL;
	}

	self_check_in_wl_tree(ubi, e, &ubi->free);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Move the physical eraseblock to the protection queue where it will
	 * be protected from being moved for some time.
	 */
	rb_erase(&e->u.rb, &ubi->free);
<<<<<<< HEAD
	dbg_wl("PEB %d EC %d", e->pnum, e->ec);
	prot_queue_add(ubi, e);
	spin_unlock(&ubi->wl_lock);

	err = ubi_dbg_check_all_ff(ubi, e->pnum, ubi->vid_hdr_aloffset,
				   ubi->peb_size - ubi->vid_hdr_aloffset);
	if (err) {
		ubi_err("new PEB %d does not contain all 0xFF bytes", e->pnum);
		return err;
	}

	return e->pnum;
=======
	ubi->free_count--;
	dbg_wl("PEB %d EC %d", e->pnum, e->ec);

	return e;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * prot_queue_del - remove a physical eraseblock from the protection queue.
 * @ubi: UBI device description object
 * @pnum: the physical eraseblock to remove
 *
 * This function deletes PEB @pnum from the protection queue and returns zero
 * in case of success and %-ENODEV if the PEB was not found.
 */
static int prot_queue_del(struct ubi_device *ubi, int pnum)
{
	struct ubi_wl_entry *e;

	e = ubi->lookuptbl[pnum];
	if (!e)
		return -ENODEV;

<<<<<<< HEAD
	if (paranoid_check_in_pq(ubi, e))
=======
	if (self_check_in_pq(ubi, e))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENODEV;

	list_del(&e->u.list);
	dbg_wl("deleted PEB %d from the protection queue", e->pnum);
	return 0;
}

/**
<<<<<<< HEAD
 * sync_erase - synchronously erase a physical eraseblock.
 * @ubi: UBI device description object
 * @e: the the physical eraseblock to erase
=======
 * ubi_sync_erase - synchronously erase a physical eraseblock.
 * @ubi: UBI device description object
 * @e: the physical eraseblock to erase
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @torture: if the physical eraseblock has to be tortured
 *
 * This function returns zero in case of success and a negative error code in
 * case of failure.
 */
<<<<<<< HEAD
static int sync_erase(struct ubi_device *ubi, struct ubi_wl_entry *e,
		      int torture)
=======
int ubi_sync_erase(struct ubi_device *ubi, struct ubi_wl_entry *e, int torture)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	struct ubi_ec_hdr *ec_hdr;
	unsigned long long ec = e->ec;

	dbg_wl("erase PEB %d, old EC %llu", e->pnum, ec);

<<<<<<< HEAD
	err = paranoid_check_ec(ubi, e->pnum, e->ec);
=======
	err = self_check_ec(ubi, e->pnum, e->ec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return -EINVAL;

	ec_hdr = kzalloc(ubi->ec_hdr_alsize, GFP_NOFS);
	if (!ec_hdr)
		return -ENOMEM;

	err = ubi_io_sync_erase(ubi, e->pnum, torture);
	if (err < 0)
		goto out_free;

	ec += err;
	if (ec > UBI_MAX_ERASECOUNTER) {
		/*
		 * Erase counter overflow. Upgrade UBI and use 64-bit
		 * erase counters internally.
		 */
<<<<<<< HEAD
		ubi_err("erase counter overflow at PEB %d, EC %llu",
=======
		ubi_err(ubi, "erase counter overflow at PEB %d, EC %llu",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			e->pnum, ec);
		err = -EINVAL;
		goto out_free;
	}

	dbg_wl("erased PEB %d, new EC %llu", e->pnum, ec);

	ec_hdr->ec = cpu_to_be64(ec);

	err = ubi_io_write_ec_hdr(ubi, e->pnum, ec_hdr);
	if (err)
		goto out_free;

	e->ec = ec;
	spin_lock(&ubi->wl_lock);
	if (e->ec > ubi->max_ec)
		ubi->max_ec = e->ec;
	spin_unlock(&ubi->wl_lock);

out_free:
	kfree(ec_hdr);
	return err;
}

/**
 * serve_prot_queue - check if it is time to stop protecting PEBs.
 * @ubi: UBI device description object
 *
 * This function is called after each erase operation and removes PEBs from the
 * tail of the protection queue. These PEBs have been protected for long enough
 * and should be moved to the used tree.
 */
static void serve_prot_queue(struct ubi_device *ubi)
{
	struct ubi_wl_entry *e, *tmp;
	int count;

	/*
	 * There may be several protected physical eraseblock to remove,
	 * process them all.
	 */
repeat:
	count = 0;
	spin_lock(&ubi->wl_lock);
	list_for_each_entry_safe(e, tmp, &ubi->pq[ubi->pq_head], u.list) {
		dbg_wl("PEB %d EC %d protection over, move to used tree",
			e->pnum, e->ec);

		list_del(&e->u.list);
		wl_tree_add(e, &ubi->used);
		if (count++ > 32) {
			/*
			 * Let's be nice and avoid holding the spinlock for
			 * too long.
			 */
			spin_unlock(&ubi->wl_lock);
			cond_resched();
			goto repeat;
		}
	}

	ubi->pq_head += 1;
	if (ubi->pq_head == UBI_PROT_QUEUE_LEN)
		ubi->pq_head = 0;
	ubi_assert(ubi->pq_head >= 0 && ubi->pq_head < UBI_PROT_QUEUE_LEN);
	spin_unlock(&ubi->wl_lock);
}

/**
<<<<<<< HEAD
 * schedule_ubi_work - schedule a work.
=======
 * __schedule_ubi_work - schedule a work.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @ubi: UBI device description object
 * @wrk: the work to schedule
 *
 * This function adds a work defined by @wrk to the tail of the pending works
<<<<<<< HEAD
 * list.
 */
static void schedule_ubi_work(struct ubi_device *ubi, struct ubi_work *wrk)
=======
 * list. Can only be used if ubi->work_sem is already held in read mode!
 */
static void __schedule_ubi_work(struct ubi_device *ubi, struct ubi_work *wrk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	spin_lock(&ubi->wl_lock);
	list_add_tail(&wrk->list, &ubi->works);
	ubi_assert(ubi->works_count >= 0);
	ubi->works_count += 1;
	if (ubi->thread_enabled && !ubi_dbg_is_bgt_disabled(ubi))
		wake_up_process(ubi->bgt_thread);
	spin_unlock(&ubi->wl_lock);
}

<<<<<<< HEAD
static int erase_worker(struct ubi_device *ubi, struct ubi_work *wl_wrk,
			int cancel);
=======
/**
 * schedule_ubi_work - schedule a work.
 * @ubi: UBI device description object
 * @wrk: the work to schedule
 *
 * This function adds a work defined by @wrk to the tail of the pending works
 * list.
 */
static void schedule_ubi_work(struct ubi_device *ubi, struct ubi_work *wrk)
{
	down_read(&ubi->work_sem);
	__schedule_ubi_work(ubi, wrk);
	up_read(&ubi->work_sem);
}

static int erase_worker(struct ubi_device *ubi, struct ubi_work *wl_wrk,
			int shutdown);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * schedule_erase - schedule an erase work.
 * @ubi: UBI device description object
 * @e: the WL entry of the physical eraseblock to erase
<<<<<<< HEAD
 * @torture: if the physical eraseblock has to be tortured
=======
 * @vol_id: the volume ID that last used this PEB
 * @lnum: the last used logical eraseblock number for the PEB
 * @torture: if the physical eraseblock has to be tortured
 * @nested: denotes whether the work_sem is already held
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function returns zero in case of success and a %-ENOMEM in case of
 * failure.
 */
static int schedule_erase(struct ubi_device *ubi, struct ubi_wl_entry *e,
<<<<<<< HEAD
			  int torture)
{
	struct ubi_work *wl_wrk;

=======
			  int vol_id, int lnum, int torture, bool nested)
{
	struct ubi_work *wl_wrk;

	ubi_assert(e);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dbg_wl("schedule erasure of PEB %d, EC %d, torture %d",
	       e->pnum, e->ec, torture);

	wl_wrk = kmalloc(sizeof(struct ubi_work), GFP_NOFS);
	if (!wl_wrk)
		return -ENOMEM;

	wl_wrk->func = &erase_worker;
	wl_wrk->e = e;
<<<<<<< HEAD
	wl_wrk->torture = torture;

	schedule_ubi_work(ubi, wl_wrk);
	return 0;
}

=======
	wl_wrk->vol_id = vol_id;
	wl_wrk->lnum = lnum;
	wl_wrk->torture = torture;

	if (nested)
		__schedule_ubi_work(ubi, wl_wrk);
	else
		schedule_ubi_work(ubi, wl_wrk);
	return 0;
}

static int __erase_worker(struct ubi_device *ubi, struct ubi_work *wl_wrk);
/**
 * do_sync_erase - run the erase worker synchronously.
 * @ubi: UBI device description object
 * @e: the WL entry of the physical eraseblock to erase
 * @vol_id: the volume ID that last used this PEB
 * @lnum: the last used logical eraseblock number for the PEB
 * @torture: if the physical eraseblock has to be tortured
 *
 */
static int do_sync_erase(struct ubi_device *ubi, struct ubi_wl_entry *e,
			 int vol_id, int lnum, int torture)
{
	struct ubi_work wl_wrk;

	dbg_wl("sync erase of PEB %i", e->pnum);

	wl_wrk.e = e;
	wl_wrk.vol_id = vol_id;
	wl_wrk.lnum = lnum;
	wl_wrk.torture = torture;

	return __erase_worker(ubi, &wl_wrk);
}

static int ensure_wear_leveling(struct ubi_device *ubi, int nested);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * wear_leveling_worker - wear-leveling worker function.
 * @ubi: UBI device description object
 * @wrk: the work object
<<<<<<< HEAD
 * @cancel: non-zero if the worker has to free memory and exit
=======
 * @shutdown: non-zero if the worker has to free memory and exit
 * because the WL-subsystem is shutting down
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function copies a more worn out physical eraseblock to a less worn out
 * one. Returns zero in case of success and a negative error code in case of
 * failure.
 */
static int wear_leveling_worker(struct ubi_device *ubi, struct ubi_work *wrk,
<<<<<<< HEAD
				int cancel)
{
	int err, scrubbing = 0, torture = 0, protect = 0, erroneous = 0;
	int vol_id = -1, lnum = -1;
	struct ubi_wl_entry *e1, *e2;
	struct ubi_vid_hdr *vid_hdr;

	kfree(wrk);
	if (cancel)
		return 0;

	vid_hdr = ubi_zalloc_vid_hdr(ubi, GFP_NOFS);
	if (!vid_hdr)
		return -ENOMEM;

=======
				int shutdown)
{
	int err, scrubbing = 0, torture = 0, protect = 0, erroneous = 0;
	int erase = 0, keep = 0, vol_id = -1, lnum = -1;
	struct ubi_wl_entry *e1, *e2;
	struct ubi_vid_io_buf *vidb;
	struct ubi_vid_hdr *vid_hdr;
	int dst_leb_clean = 0;

	kfree(wrk);
	if (shutdown)
		return 0;

	vidb = ubi_alloc_vid_buf(ubi, GFP_NOFS);
	if (!vidb)
		return -ENOMEM;

	vid_hdr = ubi_get_vid_hdr(vidb);

	down_read(&ubi->fm_eba_sem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&ubi->move_mutex);
	spin_lock(&ubi->wl_lock);
	ubi_assert(!ubi->move_from && !ubi->move_to);
	ubi_assert(!ubi->move_to_put);

<<<<<<< HEAD
	if (!ubi->free.rb_node ||
=======
#ifdef CONFIG_MTD_UBI_FASTMAP
	if (!next_peb_for_wl(ubi) ||
#else
	if (!ubi->free.rb_node ||
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    (!ubi->used.rb_node && !ubi->scrub.rb_node)) {
		/*
		 * No free physical eraseblocks? Well, they must be waiting in
		 * the queue to be erased. Cancel movement - it will be
		 * triggered again when a free physical eraseblock appears.
		 *
		 * No used physical eraseblocks? They must be temporarily
		 * protected from being moved. They will be moved to the
		 * @ubi->used tree later and the wear-leveling will be
		 * triggered again.
		 */
		dbg_wl("cancel WL, a list is empty: free %d, used %d",
		       !ubi->free.rb_node, !ubi->used.rb_node);
		goto out_cancel;
	}

<<<<<<< HEAD
	if (!ubi->scrub.rb_node) {
=======
#ifdef CONFIG_MTD_UBI_FASTMAP
	e1 = find_anchor_wl_entry(&ubi->used);
	if (e1 && ubi->fm_anchor &&
	    (ubi->fm_anchor->ec - e1->ec >= UBI_WL_THRESHOLD)) {
		ubi->fm_do_produce_anchor = 1;
		/*
		 * fm_anchor is no longer considered a good anchor.
		 * NULL assignment also prevents multiple wear level checks
		 * of this PEB.
		 */
		wl_tree_add(ubi->fm_anchor, &ubi->free);
		ubi->fm_anchor = NULL;
		ubi->free_count++;
	}

	if (ubi->fm_do_produce_anchor) {
		if (!e1)
			goto out_cancel;
		e2 = get_peb_for_wl(ubi);
		if (!e2)
			goto out_cancel;

		self_check_in_wl_tree(ubi, e1, &ubi->used);
		rb_erase(&e1->u.rb, &ubi->used);
		dbg_wl("anchor-move PEB %d to PEB %d", e1->pnum, e2->pnum);
		ubi->fm_do_produce_anchor = 0;
	} else if (!ubi->scrub.rb_node) {
#else
	if (!ubi->scrub.rb_node) {
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Now pick the least worn-out used physical eraseblock and a
		 * highly worn-out free physical eraseblock. If the erase
		 * counters differ much enough, start wear-leveling.
		 */
		e1 = rb_entry(rb_first(&ubi->used), struct ubi_wl_entry, u.rb);
<<<<<<< HEAD
		e2 = find_wl_entry(&ubi->free, WL_FREE_MAX_DIFF);
=======
		e2 = get_peb_for_wl(ubi);
		if (!e2)
			goto out_cancel;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!(e2->ec - e1->ec >= UBI_WL_THRESHOLD)) {
			dbg_wl("no WL needed: min used EC %d, max free EC %d",
			       e1->ec, e2->ec);

			/* Give the unused PEB back */
			wl_tree_add(e2, &ubi->free);
<<<<<<< HEAD
			goto out_cancel;
		}
		paranoid_check_in_wl_tree(ubi, e1, &ubi->used);
=======
			ubi->free_count++;
			goto out_cancel;
		}
		self_check_in_wl_tree(ubi, e1, &ubi->used);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rb_erase(&e1->u.rb, &ubi->used);
		dbg_wl("move PEB %d EC %d to PEB %d EC %d",
		       e1->pnum, e1->ec, e2->pnum, e2->ec);
	} else {
		/* Perform scrubbing */
		scrubbing = 1;
		e1 = rb_entry(rb_first(&ubi->scrub), struct ubi_wl_entry, u.rb);
<<<<<<< HEAD
		e2 = find_wl_entry(&ubi->free, WL_FREE_MAX_DIFF);
		paranoid_check_in_wl_tree(ubi, e1, &ubi->scrub);
=======
		e2 = get_peb_for_wl(ubi);
		if (!e2)
			goto out_cancel;

		self_check_in_wl_tree(ubi, e1, &ubi->scrub);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rb_erase(&e1->u.rb, &ubi->scrub);
		dbg_wl("scrub PEB %d to PEB %d", e1->pnum, e2->pnum);
	}

<<<<<<< HEAD
	paranoid_check_in_wl_tree(ubi, e2, &ubi->free);
	rb_erase(&e2->u.rb, &ubi->free);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ubi->move_from = e1;
	ubi->move_to = e2;
	spin_unlock(&ubi->wl_lock);

	/*
	 * Now we are going to copy physical eraseblock @e1->pnum to @e2->pnum.
	 * We so far do not know which logical eraseblock our physical
	 * eraseblock (@e1) belongs to. We have to read the volume identifier
	 * header first.
	 *
	 * Note, we are protected from this PEB being unmapped and erased. The
	 * 'ubi_wl_put_peb()' would wait for moving to be finished if the PEB
	 * which is being moved was unmapped.
	 */

<<<<<<< HEAD
	err = ubi_io_read_vid_hdr(ubi, e1->pnum, vid_hdr, 0);
	if (err && err != UBI_IO_BITFLIPS) {
=======
	err = ubi_io_read_vid_hdr(ubi, e1->pnum, vidb, 0);
	if (err && err != UBI_IO_BITFLIPS) {
		dst_leb_clean = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err == UBI_IO_FF) {
			/*
			 * We are trying to move PEB without a VID header. UBI
			 * always write VID headers shortly after the PEB was
			 * given, so we have a situation when it has not yet
			 * had a chance to write it, because it was preempted.
			 * So add this PEB to the protection queue so far,
			 * because presumably more data will be written there
			 * (including the missing VID header), and then we'll
			 * move it.
			 */
			dbg_wl("PEB %d has no VID header", e1->pnum);
			protect = 1;
			goto out_not_moved;
		} else if (err == UBI_IO_FF_BITFLIPS) {
			/*
			 * The same situation as %UBI_IO_FF, but bit-flips were
			 * detected. It is better to schedule this PEB for
			 * scrubbing.
			 */
			dbg_wl("PEB %d has no VID header but has bit-flips",
			       e1->pnum);
			scrubbing = 1;
			goto out_not_moved;
<<<<<<< HEAD
		}

		ubi_err("error %d while reading VID header from PEB %d",
=======
		} else if (ubi->fast_attach && err == UBI_IO_BAD_HDR_EBADMSG) {
			/*
			 * While a full scan would detect interrupted erasures
			 * at attach time we can face them here when attached from
			 * Fastmap.
			 */
			dbg_wl("PEB %d has ECC errors, maybe from an interrupted erasure",
			       e1->pnum);
			erase = 1;
			goto out_not_moved;
		}

		ubi_err(ubi, "error %d while reading VID header from PEB %d",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err, e1->pnum);
		goto out_error;
	}

	vol_id = be32_to_cpu(vid_hdr->vol_id);
	lnum = be32_to_cpu(vid_hdr->lnum);

<<<<<<< HEAD
	err = ubi_eba_copy_leb(ubi, e1->pnum, e2->pnum, vid_hdr);
=======
	err = ubi_eba_copy_leb(ubi, e1->pnum, e2->pnum, vidb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err) {
		if (err == MOVE_CANCEL_RACE) {
			/*
			 * The LEB has not been moved because the volume is
			 * being deleted or the PEB has been put meanwhile. We
			 * should prevent this PEB from being selected for
			 * wear-leveling movement again, so put it to the
			 * protection queue.
			 */
			protect = 1;
<<<<<<< HEAD
=======
			dst_leb_clean = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_not_moved;
		}
		if (err == MOVE_RETRY) {
			scrubbing = 1;
<<<<<<< HEAD
=======
			dst_leb_clean = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_not_moved;
		}
		if (err == MOVE_TARGET_BITFLIPS || err == MOVE_TARGET_WR_ERR ||
		    err == MOVE_TARGET_RD_ERR) {
			/*
			 * Target PEB had bit-flips or write error - torture it.
			 */
			torture = 1;
<<<<<<< HEAD
=======
			keep = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_not_moved;
		}

		if (err == MOVE_SOURCE_RD_ERR) {
			/*
			 * An error happened while reading the source PEB. Do
			 * not switch to R/O mode in this case, and give the
			 * upper layers a possibility to recover from this,
			 * e.g. by unmapping corresponding LEB. Instead, just
			 * put this PEB to the @ubi->erroneous list to prevent
			 * UBI from trying to move it over and over again.
			 */
			if (ubi->erroneous_peb_count > ubi->max_erroneous) {
<<<<<<< HEAD
				ubi_err("too many erroneous eraseblocks (%d)",
					ubi->erroneous_peb_count);
				goto out_error;
			}
=======
				ubi_err(ubi, "too many erroneous eraseblocks (%d)",
					ubi->erroneous_peb_count);
				goto out_error;
			}
			dst_leb_clean = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			erroneous = 1;
			goto out_not_moved;
		}

		if (err < 0)
			goto out_error;

		ubi_assert(0);
	}

	/* The PEB has been successfully moved */
	if (scrubbing)
<<<<<<< HEAD
		ubi_msg("scrubbed PEB %d (LEB %d:%d), data moved to PEB %d",
			e1->pnum, vol_id, lnum, e2->pnum);
	ubi_free_vid_hdr(ubi, vid_hdr);
=======
		ubi_msg(ubi, "scrubbed PEB %d (LEB %d:%d), data moved to PEB %d",
			e1->pnum, vol_id, lnum, e2->pnum);
	ubi_free_vid_buf(vidb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&ubi->wl_lock);
	if (!ubi->move_to_put) {
		wl_tree_add(e2, &ubi->used);
		e2 = NULL;
	}
	ubi->move_from = ubi->move_to = NULL;
	ubi->move_to_put = ubi->wl_scheduled = 0;
	spin_unlock(&ubi->wl_lock);

<<<<<<< HEAD
	err = schedule_erase(ubi, e1, 0);
	if (err) {
		kmem_cache_free(ubi_wl_entry_slab, e1);
		if (e2)
			kmem_cache_free(ubi_wl_entry_slab, e2);
=======
	err = do_sync_erase(ubi, e1, vol_id, lnum, 0);
	if (err) {
		if (e2) {
			spin_lock(&ubi->wl_lock);
			wl_entry_destroy(ubi, e2);
			spin_unlock(&ubi->wl_lock);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_ro;
	}

	if (e2) {
		/*
		 * Well, the target PEB was put meanwhile, schedule it for
		 * erasure.
		 */
		dbg_wl("PEB %d (LEB %d:%d) was put meanwhile, erase",
		       e2->pnum, vol_id, lnum);
<<<<<<< HEAD
		err = schedule_erase(ubi, e2, 0);
		if (err) {
			kmem_cache_free(ubi_wl_entry_slab, e2);
			goto out_ro;
		}
=======
		err = do_sync_erase(ubi, e2, vol_id, lnum, 0);
		if (err)
			goto out_ro;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	dbg_wl("done");
	mutex_unlock(&ubi->move_mutex);
<<<<<<< HEAD
=======
	up_read(&ubi->fm_eba_sem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

	/*
	 * For some reasons the LEB was not moved, might be an error, might be
	 * something else. @e1 was not changed, so return it back. @e2 might
	 * have been changed, schedule it for erasure.
	 */
out_not_moved:
	if (vol_id != -1)
		dbg_wl("cancel moving PEB %d (LEB %d:%d) to PEB %d (%d)",
		       e1->pnum, vol_id, lnum, e2->pnum, err);
	else
		dbg_wl("cancel moving PEB %d to PEB %d (%d)",
		       e1->pnum, e2->pnum, err);
	spin_lock(&ubi->wl_lock);
	if (protect)
		prot_queue_add(ubi, e1);
	else if (erroneous) {
		wl_tree_add(e1, &ubi->erroneous);
		ubi->erroneous_peb_count += 1;
	} else if (scrubbing)
		wl_tree_add(e1, &ubi->scrub);
<<<<<<< HEAD
	else
		wl_tree_add(e1, &ubi->used);
=======
	else if (keep)
		wl_tree_add(e1, &ubi->used);
	if (dst_leb_clean) {
		wl_tree_add(e2, &ubi->free);
		ubi->free_count++;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ubi_assert(!ubi->move_to_put);
	ubi->move_from = ubi->move_to = NULL;
	ubi->wl_scheduled = 0;
	spin_unlock(&ubi->wl_lock);

<<<<<<< HEAD
	ubi_free_vid_hdr(ubi, vid_hdr);
	err = schedule_erase(ubi, e2, torture);
	if (err) {
		kmem_cache_free(ubi_wl_entry_slab, e2);
		goto out_ro;
	}
	mutex_unlock(&ubi->move_mutex);
=======
	ubi_free_vid_buf(vidb);
	if (dst_leb_clean) {
		ensure_wear_leveling(ubi, 1);
	} else {
		err = do_sync_erase(ubi, e2, vol_id, lnum, torture);
		if (err)
			goto out_ro;
	}

	if (erase) {
		err = do_sync_erase(ubi, e1, vol_id, lnum, 1);
		if (err)
			goto out_ro;
	}

	mutex_unlock(&ubi->move_mutex);
	up_read(&ubi->fm_eba_sem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

out_error:
	if (vol_id != -1)
<<<<<<< HEAD
		ubi_err("error %d while moving PEB %d to PEB %d",
			err, e1->pnum, e2->pnum);
	else
		ubi_err("error %d while moving PEB %d (LEB %d:%d) to PEB %d",
=======
		ubi_err(ubi, "error %d while moving PEB %d to PEB %d",
			err, e1->pnum, e2->pnum);
	else
		ubi_err(ubi, "error %d while moving PEB %d (LEB %d:%d) to PEB %d",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err, e1->pnum, vol_id, lnum, e2->pnum);
	spin_lock(&ubi->wl_lock);
	ubi->move_from = ubi->move_to = NULL;
	ubi->move_to_put = ubi->wl_scheduled = 0;
<<<<<<< HEAD
	spin_unlock(&ubi->wl_lock);

	ubi_free_vid_hdr(ubi, vid_hdr);
	kmem_cache_free(ubi_wl_entry_slab, e1);
	kmem_cache_free(ubi_wl_entry_slab, e2);
=======
	wl_entry_destroy(ubi, e1);
	wl_entry_destroy(ubi, e2);
	spin_unlock(&ubi->wl_lock);

	ubi_free_vid_buf(vidb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out_ro:
	ubi_ro_mode(ubi);
	mutex_unlock(&ubi->move_mutex);
<<<<<<< HEAD
=======
	up_read(&ubi->fm_eba_sem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ubi_assert(err != 0);
	return err < 0 ? err : -EIO;

out_cancel:
	ubi->wl_scheduled = 0;
	spin_unlock(&ubi->wl_lock);
	mutex_unlock(&ubi->move_mutex);
<<<<<<< HEAD
	ubi_free_vid_hdr(ubi, vid_hdr);
=======
	up_read(&ubi->fm_eba_sem);
	ubi_free_vid_buf(vidb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
 * ensure_wear_leveling - schedule wear-leveling if it is needed.
 * @ubi: UBI device description object
<<<<<<< HEAD
=======
 * @nested: set to non-zero if this function is called from UBI worker
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function checks if it is time to start wear-leveling and schedules it
 * if yes. This function returns zero in case of success and a negative error
 * code in case of failure.
 */
<<<<<<< HEAD
static int ensure_wear_leveling(struct ubi_device *ubi)
{
	int err = 0;
	struct ubi_wl_entry *e1;
	struct ubi_wl_entry *e2;
=======
static int ensure_wear_leveling(struct ubi_device *ubi, int nested)
{
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ubi_work *wrk;

	spin_lock(&ubi->wl_lock);
	if (ubi->wl_scheduled)
		/* Wear-leveling is already in the work queue */
		goto out_unlock;

	/*
	 * If the ubi->scrub tree is not empty, scrubbing is needed, and the
<<<<<<< HEAD
	 * the WL worker has to be scheduled anyway.
	 */
	if (!ubi->scrub.rb_node) {
=======
	 * WL worker has to be scheduled anyway.
	 */
	if (!ubi->scrub.rb_node) {
#ifdef CONFIG_MTD_UBI_FASTMAP
		if (!need_wear_leveling(ubi))
			goto out_unlock;
#else
		struct ubi_wl_entry *e1;
		struct ubi_wl_entry *e2;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!ubi->used.rb_node || !ubi->free.rb_node)
			/* No physical eraseblocks - no deal */
			goto out_unlock;

		/*
		 * We schedule wear-leveling only if the difference between the
		 * lowest erase counter of used physical eraseblocks and a high
		 * erase counter of free physical eraseblocks is greater than
		 * %UBI_WL_THRESHOLD.
		 */
		e1 = rb_entry(rb_first(&ubi->used), struct ubi_wl_entry, u.rb);
<<<<<<< HEAD
		e2 = find_wl_entry(&ubi->free, WL_FREE_MAX_DIFF);

		if (!(e2->ec - e1->ec >= UBI_WL_THRESHOLD))
			goto out_unlock;
=======
		e2 = find_wl_entry(ubi, &ubi->free, WL_FREE_MAX_DIFF, 0);

		if (!(e2->ec - e1->ec >= UBI_WL_THRESHOLD))
			goto out_unlock;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dbg_wl("schedule wear-leveling");
	} else
		dbg_wl("schedule scrubbing");

	ubi->wl_scheduled = 1;
	spin_unlock(&ubi->wl_lock);

	wrk = kmalloc(sizeof(struct ubi_work), GFP_NOFS);
	if (!wrk) {
		err = -ENOMEM;
		goto out_cancel;
	}

	wrk->func = &wear_leveling_worker;
<<<<<<< HEAD
	schedule_ubi_work(ubi, wrk);
=======
	if (nested)
		__schedule_ubi_work(ubi, wrk);
	else
		schedule_ubi_work(ubi, wrk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;

out_cancel:
	spin_lock(&ubi->wl_lock);
	ubi->wl_scheduled = 0;
out_unlock:
	spin_unlock(&ubi->wl_lock);
	return err;
}

/**
<<<<<<< HEAD
 * erase_worker - physical eraseblock erase worker function.
 * @ubi: UBI device description object
 * @wl_wrk: the work object
 * @cancel: non-zero if the worker has to free memory and exit
=======
 * __erase_worker - physical eraseblock erase worker function.
 * @ubi: UBI device description object
 * @wl_wrk: the work object
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function erases a physical eraseblock and perform torture testing if
 * needed. It also takes care about marking the physical eraseblock bad if
 * needed. Returns zero in case of success and a negative error code in case of
 * failure.
 */
<<<<<<< HEAD
static int erase_worker(struct ubi_device *ubi, struct ubi_work *wl_wrk,
			int cancel)
{
	struct ubi_wl_entry *e = wl_wrk->e;
	int pnum = e->pnum, err, need;

	if (cancel) {
		dbg_wl("cancel erasure of PEB %d EC %d", pnum, e->ec);
		kfree(wl_wrk);
		kmem_cache_free(ubi_wl_entry_slab, e);
		return 0;
	}

	dbg_wl("erase PEB %d EC %d", pnum, e->ec);

	err = sync_erase(ubi, e, wl_wrk->torture);
	if (!err) {
		/* Fine, we've erased it successfully */
		kfree(wl_wrk);

		spin_lock(&ubi->wl_lock);
		wl_tree_add(e, &ubi->free);
=======
static int __erase_worker(struct ubi_device *ubi, struct ubi_work *wl_wrk)
{
	struct ubi_wl_entry *e = wl_wrk->e;
	int pnum = e->pnum;
	int vol_id = wl_wrk->vol_id;
	int lnum = wl_wrk->lnum;
	int err, available_consumed = 0;

	dbg_wl("erase PEB %d EC %d LEB %d:%d",
	       pnum, e->ec, wl_wrk->vol_id, wl_wrk->lnum);

	err = ubi_sync_erase(ubi, e, wl_wrk->torture);
	if (!err) {
		spin_lock(&ubi->wl_lock);

		if (!ubi->fm_disabled && !ubi->fm_anchor &&
		    e->pnum < UBI_FM_MAX_START) {
			/*
			 * Abort anchor production, if needed it will be
			 * enabled again in the wear leveling started below.
			 */
			ubi->fm_anchor = e;
			ubi->fm_do_produce_anchor = 0;
		} else {
			wl_tree_add(e, &ubi->free);
			ubi->free_count++;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock(&ubi->wl_lock);

		/*
		 * One more erase operation has happened, take care about
		 * protected physical eraseblocks.
		 */
		serve_prot_queue(ubi);

		/* And take care about wear-leveling */
<<<<<<< HEAD
		err = ensure_wear_leveling(ubi);
		return err;
	}

	ubi_err("failed to erase PEB %d, error %d", pnum, err);
	kfree(wl_wrk);
=======
		err = ensure_wear_leveling(ubi, 1);
		return err;
	}

	ubi_err(ubi, "failed to erase PEB %d, error %d", pnum, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (err == -EINTR || err == -ENOMEM || err == -EAGAIN ||
	    err == -EBUSY) {
		int err1;

		/* Re-schedule the LEB for erasure */
<<<<<<< HEAD
		err1 = schedule_erase(ubi, e, 0);
		if (err1) {
=======
		err1 = schedule_erase(ubi, e, vol_id, lnum, 0, true);
		if (err1) {
			spin_lock(&ubi->wl_lock);
			wl_entry_destroy(ubi, e);
			spin_unlock(&ubi->wl_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = err1;
			goto out_ro;
		}
		return err;
	}

<<<<<<< HEAD
	kmem_cache_free(ubi_wl_entry_slab, e);
=======
	spin_lock(&ubi->wl_lock);
	wl_entry_destroy(ubi, e);
	spin_unlock(&ubi->wl_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err != -EIO)
		/*
		 * If this is not %-EIO, we have no idea what to do. Scheduling
		 * this physical eraseblock for erasure again would cause
		 * errors again and again. Well, lets switch to R/O mode.
		 */
		goto out_ro;

	/* It is %-EIO, the PEB went bad */

	if (!ubi->bad_allowed) {
<<<<<<< HEAD
		ubi_err("bad physical eraseblock %d detected", pnum);
=======
		ubi_err(ubi, "bad physical eraseblock %d detected", pnum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_ro;
	}

	spin_lock(&ubi->volumes_lock);
<<<<<<< HEAD
	need = ubi->beb_rsvd_level - ubi->beb_rsvd_pebs + 1;
	if (need > 0) {
		need = ubi->avail_pebs >= need ? need : ubi->avail_pebs;
		ubi->avail_pebs -= need;
		ubi->rsvd_pebs += need;
		ubi->beb_rsvd_pebs += need;
		if (need > 0)
			ubi_msg("reserve more %d PEBs", need);
	}

	if (ubi->beb_rsvd_pebs == 0) {
		spin_unlock(&ubi->volumes_lock);
		ubi_err("no reserved physical eraseblocks");
		goto out_ro;
	}
	spin_unlock(&ubi->volumes_lock);

	ubi_msg("mark PEB %d as bad", pnum);
=======
	if (ubi->beb_rsvd_pebs == 0) {
		if (ubi->avail_pebs == 0) {
			spin_unlock(&ubi->volumes_lock);
			ubi_err(ubi, "no reserved/available physical eraseblocks");
			goto out_ro;
		}
		ubi->avail_pebs -= 1;
		available_consumed = 1;
	}
	spin_unlock(&ubi->volumes_lock);

	ubi_msg(ubi, "mark PEB %d as bad", pnum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ubi_io_mark_bad(ubi, pnum);
	if (err)
		goto out_ro;

	spin_lock(&ubi->volumes_lock);
<<<<<<< HEAD
	ubi->beb_rsvd_pebs -= 1;
	ubi->bad_peb_count += 1;
	ubi->good_peb_count -= 1;
	ubi_calculate_reserved(ubi);
	if (ubi->beb_rsvd_pebs)
		ubi_msg("%d PEBs left in the reserve", ubi->beb_rsvd_pebs);
	else
		ubi_warn("last PEB from the reserved pool was used");
=======
	if (ubi->beb_rsvd_pebs > 0) {
		if (available_consumed) {
			/*
			 * The amount of reserved PEBs increased since we last
			 * checked.
			 */
			ubi->avail_pebs += 1;
			available_consumed = 0;
		}
		ubi->beb_rsvd_pebs -= 1;
	}
	ubi->bad_peb_count += 1;
	ubi->good_peb_count -= 1;
	ubi_calculate_reserved(ubi);
	if (available_consumed)
		ubi_warn(ubi, "no PEBs in the reserved pool, used an available PEB");
	else if (ubi->beb_rsvd_pebs)
		ubi_msg(ubi, "%d PEBs left in the reserve",
			ubi->beb_rsvd_pebs);
	else
		ubi_warn(ubi, "last PEB from the reserve was used");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&ubi->volumes_lock);

	return err;

out_ro:
<<<<<<< HEAD
=======
	if (available_consumed) {
		spin_lock(&ubi->volumes_lock);
		ubi->avail_pebs += 1;
		spin_unlock(&ubi->volumes_lock);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ubi_ro_mode(ubi);
	return err;
}

<<<<<<< HEAD
/**
 * ubi_wl_put_peb - return a PEB to the wear-leveling sub-system.
 * @ubi: UBI device description object
=======
static int erase_worker(struct ubi_device *ubi, struct ubi_work *wl_wrk,
			  int shutdown)
{
	int ret;

	if (shutdown) {
		struct ubi_wl_entry *e = wl_wrk->e;

		dbg_wl("cancel erasure of PEB %d EC %d", e->pnum, e->ec);
		kfree(wl_wrk);
		wl_entry_destroy(ubi, e);
		return 0;
	}

	ret = __erase_worker(ubi, wl_wrk);
	kfree(wl_wrk);
	return ret;
}

/**
 * ubi_wl_put_peb - return a PEB to the wear-leveling sub-system.
 * @ubi: UBI device description object
 * @vol_id: the volume ID that last used this PEB
 * @lnum: the last used logical eraseblock number for the PEB
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @pnum: physical eraseblock to return
 * @torture: if this physical eraseblock has to be tortured
 *
 * This function is called to return physical eraseblock @pnum to the pool of
 * free physical eraseblocks. The @torture flag has to be set if an I/O error
 * occurred to this @pnum and it has to be tested. This function returns zero
 * in case of success, and a negative error code in case of failure.
 */
<<<<<<< HEAD
int ubi_wl_put_peb(struct ubi_device *ubi, int pnum, int torture)
=======
int ubi_wl_put_peb(struct ubi_device *ubi, int vol_id, int lnum,
		   int pnum, int torture)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	struct ubi_wl_entry *e;

	dbg_wl("PEB %d", pnum);
	ubi_assert(pnum >= 0);
	ubi_assert(pnum < ubi->peb_count);

<<<<<<< HEAD
retry:
	spin_lock(&ubi->wl_lock);
	e = ubi->lookuptbl[pnum];
=======
	down_read(&ubi->fm_protect);

retry:
	spin_lock(&ubi->wl_lock);
	e = ubi->lookuptbl[pnum];
	if (!e) {
		/*
		 * This wl entry has been removed for some errors by other
		 * process (eg. wear leveling worker), corresponding process
		 * (except __erase_worker, which cannot concurrent with
		 * ubi_wl_put_peb) will set ubi ro_mode at the same time,
		 * just ignore this wl entry.
		 */
		spin_unlock(&ubi->wl_lock);
		up_read(&ubi->fm_protect);
		return 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (e == ubi->move_from) {
		/*
		 * User is putting the physical eraseblock which was selected to
		 * be moved. It will be scheduled for erasure in the
		 * wear-leveling worker.
		 */
		dbg_wl("PEB %d is being moved, wait", pnum);
		spin_unlock(&ubi->wl_lock);

		/* Wait for the WL worker by taking the @ubi->move_mutex */
		mutex_lock(&ubi->move_mutex);
		mutex_unlock(&ubi->move_mutex);
		goto retry;
	} else if (e == ubi->move_to) {
		/*
		 * User is putting the physical eraseblock which was selected
		 * as the target the data is moved to. It may happen if the EBA
		 * sub-system already re-mapped the LEB in 'ubi_eba_copy_leb()'
		 * but the WL sub-system has not put the PEB to the "used" tree
		 * yet, but it is about to do this. So we just set a flag which
		 * will tell the WL worker that the PEB is not needed anymore
		 * and should be scheduled for erasure.
		 */
		dbg_wl("PEB %d is the target of data moving", pnum);
		ubi_assert(!ubi->move_to_put);
		ubi->move_to_put = 1;
		spin_unlock(&ubi->wl_lock);
<<<<<<< HEAD
		return 0;
	} else {
		if (in_wl_tree(e, &ubi->used)) {
			paranoid_check_in_wl_tree(ubi, e, &ubi->used);
			rb_erase(&e->u.rb, &ubi->used);
		} else if (in_wl_tree(e, &ubi->scrub)) {
			paranoid_check_in_wl_tree(ubi, e, &ubi->scrub);
			rb_erase(&e->u.rb, &ubi->scrub);
		} else if (in_wl_tree(e, &ubi->erroneous)) {
			paranoid_check_in_wl_tree(ubi, e, &ubi->erroneous);
=======
		up_read(&ubi->fm_protect);
		return 0;
	} else {
		if (in_wl_tree(e, &ubi->used)) {
			self_check_in_wl_tree(ubi, e, &ubi->used);
			rb_erase(&e->u.rb, &ubi->used);
		} else if (in_wl_tree(e, &ubi->scrub)) {
			self_check_in_wl_tree(ubi, e, &ubi->scrub);
			rb_erase(&e->u.rb, &ubi->scrub);
		} else if (in_wl_tree(e, &ubi->erroneous)) {
			self_check_in_wl_tree(ubi, e, &ubi->erroneous);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rb_erase(&e->u.rb, &ubi->erroneous);
			ubi->erroneous_peb_count -= 1;
			ubi_assert(ubi->erroneous_peb_count >= 0);
			/* Erroneous PEBs should be tortured */
			torture = 1;
		} else {
			err = prot_queue_del(ubi, e->pnum);
			if (err) {
<<<<<<< HEAD
				ubi_err("PEB %d not found", pnum);
				ubi_ro_mode(ubi);
				spin_unlock(&ubi->wl_lock);
=======
				ubi_err(ubi, "PEB %d not found", pnum);
				ubi_ro_mode(ubi);
				spin_unlock(&ubi->wl_lock);
				up_read(&ubi->fm_protect);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return err;
			}
		}
	}
	spin_unlock(&ubi->wl_lock);

<<<<<<< HEAD
	err = schedule_erase(ubi, e, torture);
=======
	err = schedule_erase(ubi, e, vol_id, lnum, torture, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err) {
		spin_lock(&ubi->wl_lock);
		wl_tree_add(e, &ubi->used);
		spin_unlock(&ubi->wl_lock);
	}

<<<<<<< HEAD
=======
	up_read(&ubi->fm_protect);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/**
 * ubi_wl_scrub_peb - schedule a physical eraseblock for scrubbing.
 * @ubi: UBI device description object
 * @pnum: the physical eraseblock to schedule
 *
 * If a bit-flip in a physical eraseblock is detected, this physical eraseblock
 * needs scrubbing. This function schedules a physical eraseblock for
 * scrubbing which is done in background. This function returns zero in case of
 * success and a negative error code in case of failure.
 */
int ubi_wl_scrub_peb(struct ubi_device *ubi, int pnum)
{
	struct ubi_wl_entry *e;

<<<<<<< HEAD
	dbg_msg("schedule PEB %d for scrubbing", pnum);
=======
	ubi_msg(ubi, "schedule PEB %d for scrubbing", pnum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

retry:
	spin_lock(&ubi->wl_lock);
	e = ubi->lookuptbl[pnum];
	if (e == ubi->move_from || in_wl_tree(e, &ubi->scrub) ||
				   in_wl_tree(e, &ubi->erroneous)) {
		spin_unlock(&ubi->wl_lock);
		return 0;
	}

	if (e == ubi->move_to) {
		/*
		 * This physical eraseblock was used to move data to. The data
		 * was moved but the PEB was not yet inserted to the proper
		 * tree. We should just wait a little and let the WL worker
		 * proceed.
		 */
		spin_unlock(&ubi->wl_lock);
		dbg_wl("the PEB %d is not in proper tree, retry", pnum);
		yield();
		goto retry;
	}

	if (in_wl_tree(e, &ubi->used)) {
<<<<<<< HEAD
		paranoid_check_in_wl_tree(ubi, e, &ubi->used);
=======
		self_check_in_wl_tree(ubi, e, &ubi->used);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rb_erase(&e->u.rb, &ubi->used);
	} else {
		int err;

		err = prot_queue_del(ubi, e->pnum);
		if (err) {
<<<<<<< HEAD
			ubi_err("PEB %d not found", pnum);
=======
			ubi_err(ubi, "PEB %d not found", pnum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ubi_ro_mode(ubi);
			spin_unlock(&ubi->wl_lock);
			return err;
		}
	}

	wl_tree_add(e, &ubi->scrub);
	spin_unlock(&ubi->wl_lock);

	/*
	 * Technically scrubbing is the same as wear-leveling, so it is done
	 * by the WL worker.
	 */
<<<<<<< HEAD
	return ensure_wear_leveling(ubi);
=======
	return ensure_wear_leveling(ubi, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ubi_wl_flush - flush all pending works.
 * @ubi: UBI device description object
<<<<<<< HEAD
 *
 * This function returns zero in case of success and a negative error code in
 * case of failure.
 */
int ubi_wl_flush(struct ubi_device *ubi)
{
	int err;
=======
 * @vol_id: the volume id to flush for
 * @lnum: the logical eraseblock number to flush for
 *
 * This function executes all pending works for a particular volume id /
 * logical eraseblock number pair. If either value is set to %UBI_ALL, then it
 * acts as a wildcard for all of the corresponding volume numbers or logical
 * eraseblock numbers. It returns zero in case of success and a negative error
 * code in case of failure.
 */
int ubi_wl_flush(struct ubi_device *ubi, int vol_id, int lnum)
{
	int err = 0;
	int found = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Erase while the pending works queue is not empty, but not more than
	 * the number of currently pending works.
	 */
<<<<<<< HEAD
	dbg_wl("flush (%d pending works)", ubi->works_count);
	while (ubi->works_count) {
		err = do_work(ubi);
		if (err)
			return err;
=======
	dbg_wl("flush pending work for LEB %d:%d (%d pending works)",
	       vol_id, lnum, ubi->works_count);

	while (found) {
		struct ubi_work *wrk, *tmp;
		found = 0;

		down_read(&ubi->work_sem);
		spin_lock(&ubi->wl_lock);
		list_for_each_entry_safe(wrk, tmp, &ubi->works, list) {
			if ((vol_id == UBI_ALL || wrk->vol_id == vol_id) &&
			    (lnum == UBI_ALL || wrk->lnum == lnum)) {
				list_del(&wrk->list);
				ubi->works_count -= 1;
				ubi_assert(ubi->works_count >= 0);
				spin_unlock(&ubi->wl_lock);

				err = wrk->func(ubi, wrk, 0);
				if (err) {
					up_read(&ubi->work_sem);
					return err;
				}

				spin_lock(&ubi->wl_lock);
				found = 1;
				break;
			}
		}
		spin_unlock(&ubi->wl_lock);
		up_read(&ubi->work_sem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Make sure all the works which have been done in parallel are
	 * finished.
	 */
	down_write(&ubi->work_sem);
	up_write(&ubi->work_sem);

<<<<<<< HEAD
	/*
	 * And in case last was the WL worker and it canceled the LEB
	 * movement, flush again.
	 */
	while (ubi->works_count) {
		dbg_wl("flush more (%d pending works)", ubi->works_count);
		err = do_work(ubi);
		if (err)
			return err;
	}

	return 0;
=======
	return err;
}

static bool scrub_possible(struct ubi_device *ubi, struct ubi_wl_entry *e)
{
	if (in_wl_tree(e, &ubi->scrub))
		return false;
	else if (in_wl_tree(e, &ubi->erroneous))
		return false;
	else if (ubi->move_from == e)
		return false;
	else if (ubi->move_to == e)
		return false;

	return true;
}

/**
 * ubi_bitflip_check - Check an eraseblock for bitflips and scrub it if needed.
 * @ubi: UBI device description object
 * @pnum: the physical eraseblock to schedule
 * @force: don't read the block, assume bitflips happened and take action.
 *
 * This function reads the given eraseblock and checks if bitflips occured.
 * In case of bitflips, the eraseblock is scheduled for scrubbing.
 * If scrubbing is forced with @force, the eraseblock is not read,
 * but scheduled for scrubbing right away.
 *
 * Returns:
 * %EINVAL, PEB is out of range
 * %ENOENT, PEB is no longer used by UBI
 * %EBUSY, PEB cannot be checked now or a check is currently running on it
 * %EAGAIN, bit flips happened but scrubbing is currently not possible
 * %EUCLEAN, bit flips happened and PEB is scheduled for scrubbing
 * %0, no bit flips detected
 */
int ubi_bitflip_check(struct ubi_device *ubi, int pnum, int force)
{
	int err = 0;
	struct ubi_wl_entry *e;

	if (pnum < 0 || pnum >= ubi->peb_count) {
		err = -EINVAL;
		goto out;
	}

	/*
	 * Pause all parallel work, otherwise it can happen that the
	 * erase worker frees a wl entry under us.
	 */
	down_write(&ubi->work_sem);

	/*
	 * Make sure that the wl entry does not change state while
	 * inspecting it.
	 */
	spin_lock(&ubi->wl_lock);
	e = ubi->lookuptbl[pnum];
	if (!e) {
		spin_unlock(&ubi->wl_lock);
		err = -ENOENT;
		goto out_resume;
	}

	/*
	 * Does it make sense to check this PEB?
	 */
	if (!scrub_possible(ubi, e)) {
		spin_unlock(&ubi->wl_lock);
		err = -EBUSY;
		goto out_resume;
	}
	spin_unlock(&ubi->wl_lock);

	if (!force) {
		mutex_lock(&ubi->buf_mutex);
		err = ubi_io_read(ubi, ubi->peb_buf, pnum, 0, ubi->peb_size);
		mutex_unlock(&ubi->buf_mutex);
	}

	if (force || err == UBI_IO_BITFLIPS) {
		/*
		 * Okay, bit flip happened, let's figure out what we can do.
		 */
		spin_lock(&ubi->wl_lock);

		/*
		 * Recheck. We released wl_lock, UBI might have killed the
		 * wl entry under us.
		 */
		e = ubi->lookuptbl[pnum];
		if (!e) {
			spin_unlock(&ubi->wl_lock);
			err = -ENOENT;
			goto out_resume;
		}

		/*
		 * Need to re-check state
		 */
		if (!scrub_possible(ubi, e)) {
			spin_unlock(&ubi->wl_lock);
			err = -EBUSY;
			goto out_resume;
		}

		if (in_pq(ubi, e)) {
			prot_queue_del(ubi, e->pnum);
			wl_tree_add(e, &ubi->scrub);
			spin_unlock(&ubi->wl_lock);

			err = ensure_wear_leveling(ubi, 1);
		} else if (in_wl_tree(e, &ubi->used)) {
			rb_erase(&e->u.rb, &ubi->used);
			wl_tree_add(e, &ubi->scrub);
			spin_unlock(&ubi->wl_lock);

			err = ensure_wear_leveling(ubi, 1);
		} else if (in_wl_tree(e, &ubi->free)) {
			rb_erase(&e->u.rb, &ubi->free);
			ubi->free_count--;
			spin_unlock(&ubi->wl_lock);

			/*
			 * This PEB is empty we can schedule it for
			 * erasure right away. No wear leveling needed.
			 */
			err = schedule_erase(ubi, e, UBI_UNKNOWN, UBI_UNKNOWN,
					     force ? 0 : 1, true);
		} else {
			spin_unlock(&ubi->wl_lock);
			err = -EAGAIN;
		}

		if (!err && !force)
			err = -EUCLEAN;
	} else {
		err = 0;
	}

out_resume:
	up_write(&ubi->work_sem);
out:

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * tree_destroy - destroy an RB-tree.
<<<<<<< HEAD
 * @root: the root of the tree to destroy
 */
static void tree_destroy(struct rb_root *root)
=======
 * @ubi: UBI device description object
 * @root: the root of the tree to destroy
 */
static void tree_destroy(struct ubi_device *ubi, struct rb_root *root)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rb_node *rb;
	struct ubi_wl_entry *e;

	rb = root->rb_node;
	while (rb) {
		if (rb->rb_left)
			rb = rb->rb_left;
		else if (rb->rb_right)
			rb = rb->rb_right;
		else {
			e = rb_entry(rb, struct ubi_wl_entry, u.rb);

			rb = rb_parent(rb);
			if (rb) {
				if (rb->rb_left == &e->u.rb)
					rb->rb_left = NULL;
				else
					rb->rb_right = NULL;
			}

<<<<<<< HEAD
			kmem_cache_free(ubi_wl_entry_slab, e);
=======
			wl_entry_destroy(ubi, e);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

/**
 * ubi_thread - UBI background thread.
 * @u: the UBI device description object pointer
 */
int ubi_thread(void *u)
{
	int failures = 0;
	struct ubi_device *ubi = u;

<<<<<<< HEAD
	ubi_msg("background thread \"%s\" started, PID %d",
=======
	ubi_msg(ubi, "background thread \"%s\" started, PID %d",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ubi->bgt_name, task_pid_nr(current));

	set_freezable();
	for (;;) {
		int err;

		if (kthread_should_stop())
			break;

		if (try_to_freeze())
			continue;

		spin_lock(&ubi->wl_lock);
		if (list_empty(&ubi->works) || ubi->ro_mode ||
		    !ubi->thread_enabled || ubi_dbg_is_bgt_disabled(ubi)) {
			set_current_state(TASK_INTERRUPTIBLE);
			spin_unlock(&ubi->wl_lock);
<<<<<<< HEAD
=======

			/*
			 * Check kthread_should_stop() after we set the task
			 * state to guarantee that we either see the stop bit
			 * and exit or the task state is reset to runnable such
			 * that it's not scheduled out indefinitely and detects
			 * the stop bit at kthread_should_stop().
			 */
			if (kthread_should_stop()) {
				set_current_state(TASK_RUNNING);
				break;
			}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			schedule();
			continue;
		}
		spin_unlock(&ubi->wl_lock);

<<<<<<< HEAD
		err = do_work(ubi);
		if (err) {
			ubi_err("%s: work failed with error code %d",
=======
		err = do_work(ubi, NULL);
		if (err) {
			ubi_err(ubi, "%s: work failed with error code %d",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				ubi->bgt_name, err);
			if (failures++ > WL_MAX_FAILURES) {
				/*
				 * Too many failures, disable the thread and
				 * switch to read-only mode.
				 */
<<<<<<< HEAD
				ubi_msg("%s: %d consecutive failures",
=======
				ubi_msg(ubi, "%s: %d consecutive failures",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					ubi->bgt_name, WL_MAX_FAILURES);
				ubi_ro_mode(ubi);
				ubi->thread_enabled = 0;
				continue;
			}
		} else
			failures = 0;

		cond_resched();
	}

	dbg_wl("background thread \"%s\" is killed", ubi->bgt_name);
<<<<<<< HEAD
=======
	ubi->thread_enabled = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
<<<<<<< HEAD
 * cancel_pending - cancel all pending works.
 * @ubi: UBI device description object
 */
static void cancel_pending(struct ubi_device *ubi)
=======
 * shutdown_work - shutdown all pending works.
 * @ubi: UBI device description object
 */
static void shutdown_work(struct ubi_device *ubi)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	while (!list_empty(&ubi->works)) {
		struct ubi_work *wrk;

		wrk = list_entry(ubi->works.next, struct ubi_work, list);
		list_del(&wrk->list);
		wrk->func(ubi, wrk, 1);
		ubi->works_count -= 1;
		ubi_assert(ubi->works_count >= 0);
	}
}

/**
<<<<<<< HEAD
 * ubi_wl_init_scan - initialize the WL sub-system using scanning information.
 * @ubi: UBI device description object
 * @si: scanning information
=======
 * erase_aeb - erase a PEB given in UBI attach info PEB
 * @ubi: UBI device description object
 * @aeb: UBI attach info PEB
 * @sync: If true, erase synchronously. Otherwise schedule for erasure
 */
static int erase_aeb(struct ubi_device *ubi, struct ubi_ainf_peb *aeb, bool sync)
{
	struct ubi_wl_entry *e;
	int err;

	e = kmem_cache_alloc(ubi_wl_entry_slab, GFP_KERNEL);
	if (!e)
		return -ENOMEM;

	e->pnum = aeb->pnum;
	e->ec = aeb->ec;
	ubi->lookuptbl[e->pnum] = e;

	if (sync) {
		err = ubi_sync_erase(ubi, e, false);
		if (err)
			goto out_free;

		wl_tree_add(e, &ubi->free);
		ubi->free_count++;
	} else {
		err = schedule_erase(ubi, e, aeb->vol_id, aeb->lnum, 0, false);
		if (err)
			goto out_free;
	}

	return 0;

out_free:
	wl_entry_destroy(ubi, e);

	return err;
}

/**
 * ubi_wl_init - initialize the WL sub-system using attaching information.
 * @ubi: UBI device description object
 * @ai: attaching information
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function returns zero in case of success, and a negative error code in
 * case of failure.
 */
<<<<<<< HEAD
int ubi_wl_init_scan(struct ubi_device *ubi, struct ubi_scan_info *si)
{
	int err, i;
	struct rb_node *rb1, *rb2;
	struct ubi_scan_volume *sv;
	struct ubi_scan_leb *seb, *tmp;
=======
int ubi_wl_init(struct ubi_device *ubi, struct ubi_attach_info *ai)
{
	int err, i, reserved_pebs, found_pebs = 0;
	struct rb_node *rb1, *rb2;
	struct ubi_ainf_volume *av;
	struct ubi_ainf_peb *aeb, *tmp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ubi_wl_entry *e;

	ubi->used = ubi->erroneous = ubi->free = ubi->scrub = RB_ROOT;
	spin_lock_init(&ubi->wl_lock);
	mutex_init(&ubi->move_mutex);
	init_rwsem(&ubi->work_sem);
<<<<<<< HEAD
	ubi->max_ec = si->max_ec;
=======
	ubi->max_ec = ai->max_ec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_LIST_HEAD(&ubi->works);

	sprintf(ubi->bgt_name, UBI_BGT_NAME_PATTERN, ubi->ubi_num);

	err = -ENOMEM;
<<<<<<< HEAD
	ubi->lookuptbl = kzalloc(ubi->peb_count * sizeof(void *), GFP_KERNEL);
=======
	ubi->lookuptbl = kcalloc(ubi->peb_count, sizeof(void *), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ubi->lookuptbl)
		return err;

	for (i = 0; i < UBI_PROT_QUEUE_LEN; i++)
		INIT_LIST_HEAD(&ubi->pq[i]);
	ubi->pq_head = 0;

<<<<<<< HEAD
	list_for_each_entry_safe(seb, tmp, &si->erase, u.list) {
		cond_resched();

		e = kmem_cache_alloc(ubi_wl_entry_slab, GFP_KERNEL);
		if (!e)
			goto out_free;

		e->pnum = seb->pnum;
		e->ec = seb->ec;
		ubi->lookuptbl[e->pnum] = e;
		if (schedule_erase(ubi, e, 0)) {
			kmem_cache_free(ubi_wl_entry_slab, e);
			goto out_free;
		}
	}

	list_for_each_entry(seb, &si->free, u.list) {
		cond_resched();

		e = kmem_cache_alloc(ubi_wl_entry_slab, GFP_KERNEL);
		if (!e)
			goto out_free;

		e->pnum = seb->pnum;
		e->ec = seb->ec;
		ubi_assert(e->ec >= 0);
		wl_tree_add(e, &ubi->free);
		ubi->lookuptbl[e->pnum] = e;
	}

	ubi_rb_for_each_entry(rb1, sv, &si->volumes, rb) {
		ubi_rb_for_each_entry(rb2, seb, &sv->root, u.rb) {
			cond_resched();

			e = kmem_cache_alloc(ubi_wl_entry_slab, GFP_KERNEL);
			if (!e)
				goto out_free;

			e->pnum = seb->pnum;
			e->ec = seb->ec;
			ubi->lookuptbl[e->pnum] = e;
			if (!seb->scrub) {
=======
	ubi->free_count = 0;
	list_for_each_entry_safe(aeb, tmp, &ai->erase, u.list) {
		cond_resched();

		err = erase_aeb(ubi, aeb, false);
		if (err)
			goto out_free;

		found_pebs++;
	}

	list_for_each_entry(aeb, &ai->free, u.list) {
		cond_resched();

		e = kmem_cache_alloc(ubi_wl_entry_slab, GFP_KERNEL);
		if (!e) {
			err = -ENOMEM;
			goto out_free;
		}

		e->pnum = aeb->pnum;
		e->ec = aeb->ec;
		ubi_assert(e->ec >= 0);

		wl_tree_add(e, &ubi->free);
		ubi->free_count++;

		ubi->lookuptbl[e->pnum] = e;

		found_pebs++;
	}

	ubi_rb_for_each_entry(rb1, av, &ai->volumes, rb) {
		ubi_rb_for_each_entry(rb2, aeb, &av->root, u.rb) {
			cond_resched();

			e = kmem_cache_alloc(ubi_wl_entry_slab, GFP_KERNEL);
			if (!e) {
				err = -ENOMEM;
				goto out_free;
			}

			e->pnum = aeb->pnum;
			e->ec = aeb->ec;
			ubi->lookuptbl[e->pnum] = e;

			if (!aeb->scrub) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				dbg_wl("add PEB %d EC %d to the used tree",
				       e->pnum, e->ec);
				wl_tree_add(e, &ubi->used);
			} else {
				dbg_wl("add PEB %d EC %d to the scrub tree",
				       e->pnum, e->ec);
				wl_tree_add(e, &ubi->scrub);
			}
<<<<<<< HEAD
		}
	}

	if (ubi->avail_pebs < WL_RESERVED_PEBS) {
		ubi_err("no enough physical eraseblocks (%d, need %d)",
			ubi->avail_pebs, WL_RESERVED_PEBS);
		if (ubi->corr_peb_count)
			ubi_err("%d PEBs are corrupted and not used",
=======

			found_pebs++;
		}
	}

	list_for_each_entry(aeb, &ai->fastmap, u.list) {
		cond_resched();

		e = ubi_find_fm_block(ubi, aeb->pnum);

		if (e) {
			ubi_assert(!ubi->lookuptbl[e->pnum]);
			ubi->lookuptbl[e->pnum] = e;
		} else {
			bool sync = false;

			/*
			 * Usually old Fastmap PEBs are scheduled for erasure
			 * and we don't have to care about them but if we face
			 * an power cut before scheduling them we need to
			 * take care of them here.
			 */
			if (ubi->lookuptbl[aeb->pnum])
				continue;

			/*
			 * The fastmap update code might not find a free PEB for
			 * writing the fastmap anchor to and then reuses the
			 * current fastmap anchor PEB. When this PEB gets erased
			 * and a power cut happens before it is written again we
			 * must make sure that the fastmap attach code doesn't
			 * find any outdated fastmap anchors, hence we erase the
			 * outdated fastmap anchor PEBs synchronously here.
			 */
			if (aeb->vol_id == UBI_FM_SB_VOLUME_ID)
				sync = true;

			err = erase_aeb(ubi, aeb, sync);
			if (err)
				goto out_free;
		}

		found_pebs++;
	}

	dbg_wl("found %i PEBs", found_pebs);

	ubi_assert(ubi->good_peb_count == found_pebs);

	reserved_pebs = WL_RESERVED_PEBS;
	ubi_fastmap_init(ubi, &reserved_pebs);

	if (ubi->avail_pebs < reserved_pebs) {
		ubi_err(ubi, "no enough physical eraseblocks (%d, need %d)",
			ubi->avail_pebs, reserved_pebs);
		if (ubi->corr_peb_count)
			ubi_err(ubi, "%d PEBs are corrupted and not used",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				ubi->corr_peb_count);
		err = -ENOSPC;
		goto out_free;
	}
<<<<<<< HEAD
	ubi->avail_pebs -= WL_RESERVED_PEBS;
	ubi->rsvd_pebs += WL_RESERVED_PEBS;

	/* Schedule wear-leveling if needed */
	err = ensure_wear_leveling(ubi);
	if (err)
		goto out_free;

	return 0;

out_free:
	cancel_pending(ubi);
	tree_destroy(&ubi->used);
	tree_destroy(&ubi->free);
	tree_destroy(&ubi->scrub);
=======
	ubi->avail_pebs -= reserved_pebs;
	ubi->rsvd_pebs += reserved_pebs;

	/* Schedule wear-leveling if needed */
	err = ensure_wear_leveling(ubi, 0);
	if (err)
		goto out_free;

#ifdef CONFIG_MTD_UBI_FASTMAP
	if (!ubi->ro_mode && !ubi->fm_disabled)
		ubi_ensure_anchor_pebs(ubi);
#endif
	return 0;

out_free:
	shutdown_work(ubi);
	tree_destroy(ubi, &ubi->used);
	tree_destroy(ubi, &ubi->free);
	tree_destroy(ubi, &ubi->scrub);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(ubi->lookuptbl);
	return err;
}

/**
 * protection_queue_destroy - destroy the protection queue.
 * @ubi: UBI device description object
 */
static void protection_queue_destroy(struct ubi_device *ubi)
{
	int i;
	struct ubi_wl_entry *e, *tmp;

	for (i = 0; i < UBI_PROT_QUEUE_LEN; ++i) {
		list_for_each_entry_safe(e, tmp, &ubi->pq[i], u.list) {
			list_del(&e->u.list);
<<<<<<< HEAD
			kmem_cache_free(ubi_wl_entry_slab, e);
=======
			wl_entry_destroy(ubi, e);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

/**
 * ubi_wl_close - close the wear-leveling sub-system.
 * @ubi: UBI device description object
 */
void ubi_wl_close(struct ubi_device *ubi)
{
	dbg_wl("close the WL sub-system");
<<<<<<< HEAD
	cancel_pending(ubi);
	protection_queue_destroy(ubi);
	tree_destroy(&ubi->used);
	tree_destroy(&ubi->erroneous);
	tree_destroy(&ubi->free);
	tree_destroy(&ubi->scrub);
	kfree(ubi->lookuptbl);
}

#ifdef CONFIG_MTD_UBI_DEBUG

/**
 * paranoid_check_ec - make sure that the erase counter of a PEB is correct.
=======
	ubi_fastmap_close(ubi);
	shutdown_work(ubi);
	protection_queue_destroy(ubi);
	tree_destroy(ubi, &ubi->used);
	tree_destroy(ubi, &ubi->erroneous);
	tree_destroy(ubi, &ubi->free);
	tree_destroy(ubi, &ubi->scrub);
	kfree(ubi->lookuptbl);
}

/**
 * self_check_ec - make sure that the erase counter of a PEB is correct.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @ubi: UBI device description object
 * @pnum: the physical eraseblock number to check
 * @ec: the erase counter to check
 *
 * This function returns zero if the erase counter of physical eraseblock @pnum
 * is equivalent to @ec, and a negative error code if not or if an error
 * occurred.
 */
<<<<<<< HEAD
static int paranoid_check_ec(struct ubi_device *ubi, int pnum, int ec)
=======
static int self_check_ec(struct ubi_device *ubi, int pnum, int ec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	long long read_ec;
	struct ubi_ec_hdr *ec_hdr;

<<<<<<< HEAD
	if (!ubi->dbg->chk_gen)
=======
	if (!ubi_dbg_chk_gen(ubi))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	ec_hdr = kzalloc(ubi->ec_hdr_alsize, GFP_NOFS);
	if (!ec_hdr)
		return -ENOMEM;

	err = ubi_io_read_ec_hdr(ubi, pnum, ec_hdr, 0);
	if (err && err != UBI_IO_BITFLIPS) {
		/* The header does not have to exist */
		err = 0;
		goto out_free;
	}

	read_ec = be64_to_cpu(ec_hdr->ec);
<<<<<<< HEAD
	if (ec != read_ec) {
		ubi_err("paranoid check failed for PEB %d", pnum);
		ubi_err("read EC is %lld, should be %d", read_ec, ec);
		ubi_dbg_dump_stack();
=======
	if (ec != read_ec && read_ec - ec > 1) {
		ubi_err(ubi, "self-check failed for PEB %d", pnum);
		ubi_err(ubi, "read EC is %lld, should be %d", read_ec, ec);
		dump_stack();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = 1;
	} else
		err = 0;

out_free:
	kfree(ec_hdr);
	return err;
}

/**
<<<<<<< HEAD
 * paranoid_check_in_wl_tree - check that wear-leveling entry is in WL RB-tree.
=======
 * self_check_in_wl_tree - check that wear-leveling entry is in WL RB-tree.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @ubi: UBI device description object
 * @e: the wear-leveling entry to check
 * @root: the root of the tree
 *
 * This function returns zero if @e is in the @root RB-tree and %-EINVAL if it
 * is not.
 */
<<<<<<< HEAD
static int paranoid_check_in_wl_tree(const struct ubi_device *ubi,
				     struct ubi_wl_entry *e,
				     struct rb_root *root)
{
	if (!ubi->dbg->chk_gen)
=======
static int self_check_in_wl_tree(const struct ubi_device *ubi,
				 struct ubi_wl_entry *e, struct rb_root *root)
{
	if (!ubi_dbg_chk_gen(ubi))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	if (in_wl_tree(e, root))
		return 0;

<<<<<<< HEAD
	ubi_err("paranoid check failed for PEB %d, EC %d, RB-tree %p ",
		e->pnum, e->ec, root);
	ubi_dbg_dump_stack();
=======
	ubi_err(ubi, "self-check failed for PEB %d, EC %d, RB-tree %p ",
		e->pnum, e->ec, root);
	dump_stack();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -EINVAL;
}

/**
<<<<<<< HEAD
 * paranoid_check_in_pq - check if wear-leveling entry is in the protection
=======
 * self_check_in_pq - check if wear-leveling entry is in the protection
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *                        queue.
 * @ubi: UBI device description object
 * @e: the wear-leveling entry to check
 *
 * This function returns zero if @e is in @ubi->pq and %-EINVAL if it is not.
 */
<<<<<<< HEAD
static int paranoid_check_in_pq(const struct ubi_device *ubi,
				struct ubi_wl_entry *e)
{
	struct ubi_wl_entry *p;
	int i;

	if (!ubi->dbg->chk_gen)
		return 0;

	for (i = 0; i < UBI_PROT_QUEUE_LEN; ++i)
		list_for_each_entry(p, &ubi->pq[i], u.list)
			if (p == e)
				return 0;

	ubi_err("paranoid check failed for PEB %d, EC %d, Protect queue",
		e->pnum, e->ec);
	ubi_dbg_dump_stack();
	return -EINVAL;
}

#endif /* CONFIG_MTD_UBI_DEBUG */
=======
static int self_check_in_pq(const struct ubi_device *ubi,
			    struct ubi_wl_entry *e)
{
	if (!ubi_dbg_chk_gen(ubi))
		return 0;

	if (in_pq(ubi, e))
		return 0;

	ubi_err(ubi, "self-check failed for PEB %d, EC %d, Protect queue",
		e->pnum, e->ec);
	dump_stack();
	return -EINVAL;
}
#ifndef CONFIG_MTD_UBI_FASTMAP
static struct ubi_wl_entry *get_peb_for_wl(struct ubi_device *ubi)
{
	struct ubi_wl_entry *e;

	e = find_wl_entry(ubi, &ubi->free, WL_FREE_MAX_DIFF, 0);
	self_check_in_wl_tree(ubi, e, &ubi->free);
	ubi->free_count--;
	ubi_assert(ubi->free_count >= 0);
	rb_erase(&e->u.rb, &ubi->free);

	return e;
}

/**
 * produce_free_peb - produce a free physical eraseblock.
 * @ubi: UBI device description object
 *
 * This function tries to make a free PEB by means of synchronous execution of
 * pending works. This may be needed if, for example the background thread is
 * disabled. Returns zero in case of success and a negative error code in case
 * of failure.
 */
static int produce_free_peb(struct ubi_device *ubi)
{
	int err;

	while (!ubi->free.rb_node && ubi->works_count) {
		spin_unlock(&ubi->wl_lock);

		dbg_wl("do one work synchronously");
		err = do_work(ubi, NULL);

		spin_lock(&ubi->wl_lock);
		if (err)
			return err;
	}

	return 0;
}

/**
 * ubi_wl_get_peb - get a physical eraseblock.
 * @ubi: UBI device description object
 *
 * This function returns a physical eraseblock in case of success and a
 * negative error code in case of failure.
 * Returns with ubi->fm_eba_sem held in read mode!
 */
int ubi_wl_get_peb(struct ubi_device *ubi)
{
	int err;
	struct ubi_wl_entry *e;

retry:
	down_read(&ubi->fm_eba_sem);
	spin_lock(&ubi->wl_lock);
	if (!ubi->free.rb_node) {
		if (ubi->works_count == 0) {
			ubi_err(ubi, "no free eraseblocks");
			ubi_assert(list_empty(&ubi->works));
			spin_unlock(&ubi->wl_lock);
			return -ENOSPC;
		}

		err = produce_free_peb(ubi);
		if (err < 0) {
			spin_unlock(&ubi->wl_lock);
			return err;
		}
		spin_unlock(&ubi->wl_lock);
		up_read(&ubi->fm_eba_sem);
		goto retry;

	}
	e = wl_get_wle(ubi);
	prot_queue_add(ubi, e);
	spin_unlock(&ubi->wl_lock);

	err = ubi_self_check_all_ff(ubi, e->pnum, ubi->vid_hdr_aloffset,
				    ubi->peb_size - ubi->vid_hdr_aloffset);
	if (err) {
		ubi_err(ubi, "new PEB %d does not contain all 0xFF bytes", e->pnum);
		return err;
	}

	return e->pnum;
}
#else
#include "fastmap-wl.c"
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
