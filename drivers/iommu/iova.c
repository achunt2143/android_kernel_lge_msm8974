<<<<<<< HEAD
/*
 * Copyright © 2006-2009, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright © 2006-2009, Intel Corporation.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author: Anil S Keshavamurthy <anil.s.keshavamurthy@intel.com>
 */

#include <linux/iova.h>
<<<<<<< HEAD

void
init_iova_domain(struct iova_domain *iovad, unsigned long pfn_32bit)
{
	spin_lock_init(&iovad->iova_rbtree_lock);
	iovad->rbroot = RB_ROOT;
	iovad->cached32_node = NULL;
	iovad->dma_32bit_pfn = pfn_32bit;
}

static struct rb_node *
__get_cached_rbnode(struct iova_domain *iovad, unsigned long *limit_pfn)
{
	if ((*limit_pfn != iovad->dma_32bit_pfn) ||
		(iovad->cached32_node == NULL))
		return rb_last(&iovad->rbroot);
	else {
		struct rb_node *prev_node = rb_prev(iovad->cached32_node);
		struct iova *curr_iova =
			container_of(iovad->cached32_node, struct iova, node);
		*limit_pfn = curr_iova->pfn_lo - 1;
		return prev_node;
	}
}

static void
__cached_rbnode_insert_update(struct iova_domain *iovad,
	unsigned long limit_pfn, struct iova *new)
{
	if (limit_pfn != iovad->dma_32bit_pfn)
		return;
	iovad->cached32_node = &new->node;
=======
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/bitops.h>
#include <linux/cpu.h>
#include <linux/workqueue.h>

/* The anchor node sits above the top of the usable address space */
#define IOVA_ANCHOR	~0UL

#define IOVA_RANGE_CACHE_MAX_SIZE 6	/* log of max cached IOVA range size (in pages) */

static bool iova_rcache_insert(struct iova_domain *iovad,
			       unsigned long pfn,
			       unsigned long size);
static unsigned long iova_rcache_get(struct iova_domain *iovad,
				     unsigned long size,
				     unsigned long limit_pfn);
static void free_iova_rcaches(struct iova_domain *iovad);
static void free_cpu_cached_iovas(unsigned int cpu, struct iova_domain *iovad);
static void free_global_cached_iovas(struct iova_domain *iovad);

static struct iova *to_iova(struct rb_node *node)
{
	return rb_entry(node, struct iova, node);
}

void
init_iova_domain(struct iova_domain *iovad, unsigned long granule,
	unsigned long start_pfn)
{
	/*
	 * IOVA granularity will normally be equal to the smallest
	 * supported IOMMU page size; both *must* be capable of
	 * representing individual CPU pages exactly.
	 */
	BUG_ON((granule > PAGE_SIZE) || !is_power_of_2(granule));

	spin_lock_init(&iovad->iova_rbtree_lock);
	iovad->rbroot = RB_ROOT;
	iovad->cached_node = &iovad->anchor.node;
	iovad->cached32_node = &iovad->anchor.node;
	iovad->granule = granule;
	iovad->start_pfn = start_pfn;
	iovad->dma_32bit_pfn = 1UL << (32 - iova_shift(iovad));
	iovad->max32_alloc_size = iovad->dma_32bit_pfn;
	iovad->anchor.pfn_lo = iovad->anchor.pfn_hi = IOVA_ANCHOR;
	rb_link_node(&iovad->anchor.node, NULL, &iovad->rbroot.rb_node);
	rb_insert_color(&iovad->anchor.node, &iovad->rbroot);
}
EXPORT_SYMBOL_GPL(init_iova_domain);

static struct rb_node *
__get_cached_rbnode(struct iova_domain *iovad, unsigned long limit_pfn)
{
	if (limit_pfn <= iovad->dma_32bit_pfn)
		return iovad->cached32_node;

	return iovad->cached_node;
}

static void
__cached_rbnode_insert_update(struct iova_domain *iovad, struct iova *new)
{
	if (new->pfn_hi < iovad->dma_32bit_pfn)
		iovad->cached32_node = &new->node;
	else
		iovad->cached_node = &new->node;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
__cached_rbnode_delete_update(struct iova_domain *iovad, struct iova *free)
{
	struct iova *cached_iova;
<<<<<<< HEAD
	struct rb_node *curr;

	if (!iovad->cached32_node)
		return;
	curr = iovad->cached32_node;
	cached_iova = container_of(curr, struct iova, node);

	if (free->pfn_lo >= cached_iova->pfn_lo) {
		struct rb_node *node = rb_next(&free->node);
		struct iova *iova = container_of(node, struct iova, node);

		/* only cache if it's below 32bit pfn */
		if (node && iova->pfn_lo < iovad->dma_32bit_pfn)
			iovad->cached32_node = node;
		else
			iovad->cached32_node = NULL;
	}
}

/* Computes the padding size required, to make the
 * the start address naturally aligned on its size
 */
static int
iova_get_pad_size(int size, unsigned int limit_pfn)
{
	unsigned int pad_size = 0;
	unsigned int order = ilog2(size);

	if (order)
		pad_size = (limit_pfn + 1) % (1 << order);

	return pad_size;
}

static int __alloc_and_insert_iova_range(struct iova_domain *iovad,
		unsigned long size, unsigned long limit_pfn,
			struct iova *new, bool size_aligned)
{
	struct rb_node *prev, *curr = NULL;
	unsigned long flags;
	unsigned long saved_pfn;
	unsigned int pad_size = 0;

	/* Walk the tree backwards */
	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
	saved_pfn = limit_pfn;
	curr = __get_cached_rbnode(iovad, &limit_pfn);
	prev = curr;
	while (curr) {
		struct iova *curr_iova = container_of(curr, struct iova, node);

		if (limit_pfn < curr_iova->pfn_lo)
			goto move_left;
		else if (limit_pfn < curr_iova->pfn_hi)
			goto adjust_limit_pfn;
		else {
			if (size_aligned)
				pad_size = iova_get_pad_size(size, limit_pfn);
			if ((curr_iova->pfn_hi + size + pad_size) <= limit_pfn)
				break;	/* found a free slot */
		}
adjust_limit_pfn:
		limit_pfn = curr_iova->pfn_lo - 1;
move_left:
		prev = curr;
		curr = rb_prev(curr);
	}

	if (!curr) {
		if (size_aligned)
			pad_size = iova_get_pad_size(size, limit_pfn);
		if ((IOVA_START_PFN + size + pad_size) > limit_pfn) {
			spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
			return -ENOMEM;
		}
	}

	/* pfn_lo will point to size aligned address if size_aligned is set */
	new->pfn_lo = limit_pfn - (size + pad_size) + 1;
	new->pfn_hi = new->pfn_lo + size - 1;

	/* Insert the new_iova into domain rbtree by holding writer lock */
	/* Add new node and rebalance tree. */
	{
		struct rb_node **entry, *parent = NULL;

		/* If we have 'prev', it's a valid place to start the
		   insertion. Otherwise, start from the root. */
		if (prev)
			entry = &prev;
		else
			entry = &iovad->rbroot.rb_node;

		/* Figure out where to put new node */
		while (*entry) {
			struct iova *this = container_of(*entry,
							struct iova, node);
			parent = *entry;

			if (new->pfn_lo < this->pfn_lo)
				entry = &((*entry)->rb_left);
			else if (new->pfn_lo > this->pfn_lo)
				entry = &((*entry)->rb_right);
			else
				BUG(); /* this should not happen */
		}

		/* Add new node and rebalance tree. */
		rb_link_node(&new->node, parent, entry);
		rb_insert_color(&new->node, &iovad->rbroot);
	}
	__cached_rbnode_insert_update(iovad, saved_pfn, new);

	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);


	return 0;
}

static void
iova_insert_rbtree(struct rb_root *root, struct iova *iova)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;
	/* Figure out where to put new node */
	while (*new) {
		struct iova *this = container_of(*new, struct iova, node);
=======

	cached_iova = to_iova(iovad->cached32_node);
	if (free == cached_iova ||
	    (free->pfn_hi < iovad->dma_32bit_pfn &&
	     free->pfn_lo >= cached_iova->pfn_lo))
		iovad->cached32_node = rb_next(&free->node);

	if (free->pfn_lo < iovad->dma_32bit_pfn)
		iovad->max32_alloc_size = iovad->dma_32bit_pfn;

	cached_iova = to_iova(iovad->cached_node);
	if (free->pfn_lo >= cached_iova->pfn_lo)
		iovad->cached_node = rb_next(&free->node);
}

static struct rb_node *iova_find_limit(struct iova_domain *iovad, unsigned long limit_pfn)
{
	struct rb_node *node, *next;
	/*
	 * Ideally what we'd like to judge here is whether limit_pfn is close
	 * enough to the highest-allocated IOVA that starting the allocation
	 * walk from the anchor node will be quicker than this initial work to
	 * find an exact starting point (especially if that ends up being the
	 * anchor node anyway). This is an incredibly crude approximation which
	 * only really helps the most likely case, but is at least trivially easy.
	 */
	if (limit_pfn > iovad->dma_32bit_pfn)
		return &iovad->anchor.node;

	node = iovad->rbroot.rb_node;
	while (to_iova(node)->pfn_hi < limit_pfn)
		node = node->rb_right;

search_left:
	while (node->rb_left && to_iova(node->rb_left)->pfn_lo >= limit_pfn)
		node = node->rb_left;

	if (!node->rb_left)
		return node;

	next = node->rb_left;
	while (next->rb_right) {
		next = next->rb_right;
		if (to_iova(next)->pfn_lo >= limit_pfn) {
			node = next;
			goto search_left;
		}
	}

	return node;
}

/* Insert the iova into domain rbtree by holding writer lock */
static void
iova_insert_rbtree(struct rb_root *root, struct iova *iova,
		   struct rb_node *start)
{
	struct rb_node **new, *parent = NULL;

	new = (start) ? &start : &(root->rb_node);
	/* Figure out where to put new node */
	while (*new) {
		struct iova *this = to_iova(*new);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		parent = *new;

		if (iova->pfn_lo < this->pfn_lo)
			new = &((*new)->rb_left);
		else if (iova->pfn_lo > this->pfn_lo)
			new = &((*new)->rb_right);
<<<<<<< HEAD
		else
			BUG(); /* this should not happen */
=======
		else {
			WARN_ON(1); /* this should not happen */
			return;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/* Add new node and rebalance tree. */
	rb_link_node(&iova->node, parent, new);
	rb_insert_color(&iova->node, root);
}

<<<<<<< HEAD
/**
 * alloc_iova - allocates an iova
 * @iovad - iova domain in question
 * @size - size of page frames to allocate
 * @limit_pfn - max limit address
 * @size_aligned - set if size_aligned address range is required
 * This function allocates an iova in the range limit_pfn to IOVA_START_PFN
 * looking from limit_pfn instead from IOVA_START_PFN. If the size_aligned
=======
static int __alloc_and_insert_iova_range(struct iova_domain *iovad,
		unsigned long size, unsigned long limit_pfn,
			struct iova *new, bool size_aligned)
{
	struct rb_node *curr, *prev;
	struct iova *curr_iova;
	unsigned long flags;
	unsigned long new_pfn, retry_pfn;
	unsigned long align_mask = ~0UL;
	unsigned long high_pfn = limit_pfn, low_pfn = iovad->start_pfn;

	if (size_aligned)
		align_mask <<= fls_long(size - 1);

	/* Walk the tree backwards */
	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
	if (limit_pfn <= iovad->dma_32bit_pfn &&
			size >= iovad->max32_alloc_size)
		goto iova32_full;

	curr = __get_cached_rbnode(iovad, limit_pfn);
	curr_iova = to_iova(curr);
	retry_pfn = curr_iova->pfn_hi;

retry:
	do {
		high_pfn = min(high_pfn, curr_iova->pfn_lo);
		new_pfn = (high_pfn - size) & align_mask;
		prev = curr;
		curr = rb_prev(curr);
		curr_iova = to_iova(curr);
	} while (curr && new_pfn <= curr_iova->pfn_hi && new_pfn >= low_pfn);

	if (high_pfn < size || new_pfn < low_pfn) {
		if (low_pfn == iovad->start_pfn && retry_pfn < limit_pfn) {
			high_pfn = limit_pfn;
			low_pfn = retry_pfn + 1;
			curr = iova_find_limit(iovad, limit_pfn);
			curr_iova = to_iova(curr);
			goto retry;
		}
		iovad->max32_alloc_size = size;
		goto iova32_full;
	}

	/* pfn_lo will point to size aligned address if size_aligned is set */
	new->pfn_lo = new_pfn;
	new->pfn_hi = new->pfn_lo + size - 1;

	/* If we have 'prev', it's a valid place to start the insertion. */
	iova_insert_rbtree(&iovad->rbroot, new, prev);
	__cached_rbnode_insert_update(iovad, new);

	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
	return 0;

iova32_full:
	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
	return -ENOMEM;
}

static struct kmem_cache *iova_cache;
static unsigned int iova_cache_users;
static DEFINE_MUTEX(iova_cache_mutex);

static struct iova *alloc_iova_mem(void)
{
	return kmem_cache_zalloc(iova_cache, GFP_ATOMIC | __GFP_NOWARN);
}

static void free_iova_mem(struct iova *iova)
{
	if (iova->pfn_lo != IOVA_ANCHOR)
		kmem_cache_free(iova_cache, iova);
}

/**
 * alloc_iova - allocates an iova
 * @iovad: - iova domain in question
 * @size: - size of page frames to allocate
 * @limit_pfn: - max limit address
 * @size_aligned: - set if size_aligned address range is required
 * This function allocates an iova in the range iovad->start_pfn to limit_pfn,
 * searching top-down from limit_pfn to iovad->start_pfn. If the size_aligned
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * flag is set then the allocated address iova->pfn_lo will be naturally
 * aligned on roundup_power_of_two(size).
 */
struct iova *
alloc_iova(struct iova_domain *iovad, unsigned long size,
	unsigned long limit_pfn,
	bool size_aligned)
{
	struct iova *new_iova;
	int ret;

	new_iova = alloc_iova_mem();
	if (!new_iova)
		return NULL;

<<<<<<< HEAD
	/* If size aligned is set then round the size to
	 * to next power of two.
	 */
	if (size_aligned)
		size = __roundup_pow_of_two(size);

	ret = __alloc_and_insert_iova_range(iovad, size, limit_pfn,
=======
	ret = __alloc_and_insert_iova_range(iovad, size, limit_pfn + 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			new_iova, size_aligned);

	if (ret) {
		free_iova_mem(new_iova);
		return NULL;
	}

	return new_iova;
}
<<<<<<< HEAD

/**
 * find_iova - find's an iova for a given pfn
 * @iovad - iova domain in question.
 * pfn - page frame number
 * This function finds and returns an iova belonging to the
 * given doamin which matches the given pfn.
=======
EXPORT_SYMBOL_GPL(alloc_iova);

static struct iova *
private_find_iova(struct iova_domain *iovad, unsigned long pfn)
{
	struct rb_node *node = iovad->rbroot.rb_node;

	assert_spin_locked(&iovad->iova_rbtree_lock);

	while (node) {
		struct iova *iova = to_iova(node);

		if (pfn < iova->pfn_lo)
			node = node->rb_left;
		else if (pfn > iova->pfn_hi)
			node = node->rb_right;
		else
			return iova;	/* pfn falls within iova's range */
	}

	return NULL;
}

static void remove_iova(struct iova_domain *iovad, struct iova *iova)
{
	assert_spin_locked(&iovad->iova_rbtree_lock);
	__cached_rbnode_delete_update(iovad, iova);
	rb_erase(&iova->node, &iovad->rbroot);
}

/**
 * find_iova - finds an iova for a given pfn
 * @iovad: - iova domain in question.
 * @pfn: - page frame number
 * This function finds and returns an iova belonging to the
 * given domain which matches the given pfn.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct iova *find_iova(struct iova_domain *iovad, unsigned long pfn)
{
	unsigned long flags;
<<<<<<< HEAD
	struct rb_node *node;

	/* Take the lock so that no other thread is manipulating the rbtree */
	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
	node = iovad->rbroot.rb_node;
	while (node) {
		struct iova *iova = container_of(node, struct iova, node);

		/* If pfn falls within iova's range, return iova */
		if ((pfn >= iova->pfn_lo) && (pfn <= iova->pfn_hi)) {
			spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
			/* We are not holding the lock while this iova
			 * is referenced by the caller as the same thread
			 * which called this function also calls __free_iova()
			 * and it is by desing that only one thread can possibly
			 * reference a particular iova and hence no conflict.
			 */
			return iova;
		}

		if (pfn < iova->pfn_lo)
			node = node->rb_left;
		else if (pfn > iova->pfn_lo)
			node = node->rb_right;
	}

	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
	return NULL;
}
=======
	struct iova *iova;

	/* Take the lock so that no other thread is manipulating the rbtree */
	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
	iova = private_find_iova(iovad, pfn);
	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
	return iova;
}
EXPORT_SYMBOL_GPL(find_iova);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * __free_iova - frees the given iova
 * @iovad: iova domain in question.
 * @iova: iova in question.
 * Frees the given iova belonging to the giving domain
 */
void
__free_iova(struct iova_domain *iovad, struct iova *iova)
{
	unsigned long flags;

	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
<<<<<<< HEAD
	__cached_rbnode_delete_update(iovad, iova);
	rb_erase(&iova->node, &iovad->rbroot);
	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
	free_iova_mem(iova);
}
=======
	remove_iova(iovad, iova);
	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
	free_iova_mem(iova);
}
EXPORT_SYMBOL_GPL(__free_iova);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * free_iova - finds and frees the iova for a given pfn
 * @iovad: - iova domain in question.
 * @pfn: - pfn that is allocated previously
 * This functions finds an iova for a given pfn and then
 * frees the iova from that domain.
 */
void
free_iova(struct iova_domain *iovad, unsigned long pfn)
{
<<<<<<< HEAD
	struct iova *iova = find_iova(iovad, pfn);
	if (iova)
		__free_iova(iovad, iova);

}

/**
 * put_iova_domain - destroys the iova doamin
=======
	unsigned long flags;
	struct iova *iova;

	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
	iova = private_find_iova(iovad, pfn);
	if (!iova) {
		spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
		return;
	}
	remove_iova(iovad, iova);
	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
	free_iova_mem(iova);
}
EXPORT_SYMBOL_GPL(free_iova);

/**
 * alloc_iova_fast - allocates an iova from rcache
 * @iovad: - iova domain in question
 * @size: - size of page frames to allocate
 * @limit_pfn: - max limit address
 * @flush_rcache: - set to flush rcache on regular allocation failure
 * This function tries to satisfy an iova allocation from the rcache,
 * and falls back to regular allocation on failure. If regular allocation
 * fails too and the flush_rcache flag is set then the rcache will be flushed.
*/
unsigned long
alloc_iova_fast(struct iova_domain *iovad, unsigned long size,
		unsigned long limit_pfn, bool flush_rcache)
{
	unsigned long iova_pfn;
	struct iova *new_iova;

	/*
	 * Freeing non-power-of-two-sized allocations back into the IOVA caches
	 * will come back to bite us badly, so we have to waste a bit of space
	 * rounding up anything cacheable to make sure that can't happen. The
	 * order of the unadjusted size will still match upon freeing.
	 */
	if (size < (1 << (IOVA_RANGE_CACHE_MAX_SIZE - 1)))
		size = roundup_pow_of_two(size);

	iova_pfn = iova_rcache_get(iovad, size, limit_pfn + 1);
	if (iova_pfn)
		return iova_pfn;

retry:
	new_iova = alloc_iova(iovad, size, limit_pfn, true);
	if (!new_iova) {
		unsigned int cpu;

		if (!flush_rcache)
			return 0;

		/* Try replenishing IOVAs by flushing rcache. */
		flush_rcache = false;
		for_each_online_cpu(cpu)
			free_cpu_cached_iovas(cpu, iovad);
		free_global_cached_iovas(iovad);
		goto retry;
	}

	return new_iova->pfn_lo;
}
EXPORT_SYMBOL_GPL(alloc_iova_fast);

/**
 * free_iova_fast - free iova pfn range into rcache
 * @iovad: - iova domain in question.
 * @pfn: - pfn that is allocated previously
 * @size: - # of pages in range
 * This functions frees an iova range by trying to put it into the rcache,
 * falling back to regular iova deallocation via free_iova() if this fails.
 */
void
free_iova_fast(struct iova_domain *iovad, unsigned long pfn, unsigned long size)
{
	if (iova_rcache_insert(iovad, pfn, size))
		return;

	free_iova(iovad, pfn);
}
EXPORT_SYMBOL_GPL(free_iova_fast);

static void iova_domain_free_rcaches(struct iova_domain *iovad)
{
	cpuhp_state_remove_instance_nocalls(CPUHP_IOMMU_IOVA_DEAD,
					    &iovad->cpuhp_dead);
	free_iova_rcaches(iovad);
}

/**
 * put_iova_domain - destroys the iova domain
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @iovad: - iova domain in question.
 * All the iova's in that domain are destroyed.
 */
void put_iova_domain(struct iova_domain *iovad)
{
<<<<<<< HEAD
	struct rb_node *node;
	unsigned long flags;

	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
	node = rb_first(&iovad->rbroot);
	while (node) {
		struct iova *iova = container_of(node, struct iova, node);
		rb_erase(node, &iovad->rbroot);
		free_iova_mem(iova);
		node = rb_first(&iovad->rbroot);
	}
	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
}
=======
	struct iova *iova, *tmp;

	if (iovad->rcaches)
		iova_domain_free_rcaches(iovad);

	rbtree_postorder_for_each_entry_safe(iova, tmp, &iovad->rbroot, node)
		free_iova_mem(iova);
}
EXPORT_SYMBOL_GPL(put_iova_domain);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int
__is_range_overlap(struct rb_node *node,
	unsigned long pfn_lo, unsigned long pfn_hi)
{
<<<<<<< HEAD
	struct iova *iova = container_of(node, struct iova, node);
=======
	struct iova *iova = to_iova(node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((pfn_lo <= iova->pfn_hi) && (pfn_hi >= iova->pfn_lo))
		return 1;
	return 0;
}

<<<<<<< HEAD
=======
static inline struct iova *
alloc_and_init_iova(unsigned long pfn_lo, unsigned long pfn_hi)
{
	struct iova *iova;

	iova = alloc_iova_mem();
	if (iova) {
		iova->pfn_lo = pfn_lo;
		iova->pfn_hi = pfn_hi;
	}

	return iova;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct iova *
__insert_new_range(struct iova_domain *iovad,
	unsigned long pfn_lo, unsigned long pfn_hi)
{
	struct iova *iova;

<<<<<<< HEAD
	iova = alloc_iova_mem();
	if (!iova)
		return iova;

	iova->pfn_hi = pfn_hi;
	iova->pfn_lo = pfn_lo;
	iova_insert_rbtree(&iovad->rbroot, iova);
=======
	iova = alloc_and_init_iova(pfn_lo, pfn_hi);
	if (iova)
		iova_insert_rbtree(&iovad->rbroot, iova, NULL);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return iova;
}

static void
__adjust_overlap_range(struct iova *iova,
	unsigned long *pfn_lo, unsigned long *pfn_hi)
{
	if (*pfn_lo < iova->pfn_lo)
		iova->pfn_lo = *pfn_lo;
	if (*pfn_hi > iova->pfn_hi)
		*pfn_lo = iova->pfn_hi + 1;
}

/**
 * reserve_iova - reserves an iova in the given range
 * @iovad: - iova domain pointer
 * @pfn_lo: - lower page frame address
 * @pfn_hi:- higher pfn adderss
 * This function allocates reserves the address range from pfn_lo to pfn_hi so
 * that this address is not dished out as part of alloc_iova.
 */
struct iova *
reserve_iova(struct iova_domain *iovad,
	unsigned long pfn_lo, unsigned long pfn_hi)
{
	struct rb_node *node;
	unsigned long flags;
	struct iova *iova;
	unsigned int overlap = 0;

<<<<<<< HEAD
	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
	for (node = rb_first(&iovad->rbroot); node; node = rb_next(node)) {
		if (__is_range_overlap(node, pfn_lo, pfn_hi)) {
			iova = container_of(node, struct iova, node);
=======
	/* Don't allow nonsensical pfns */
	if (WARN_ON((pfn_hi | pfn_lo) > (ULLONG_MAX >> iova_shift(iovad))))
		return NULL;

	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
	for (node = rb_first(&iovad->rbroot); node; node = rb_next(node)) {
		if (__is_range_overlap(node, pfn_lo, pfn_hi)) {
			iova = to_iova(node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__adjust_overlap_range(iova, &pfn_lo, &pfn_hi);
			if ((pfn_lo >= iova->pfn_lo) &&
				(pfn_hi <= iova->pfn_hi))
				goto finish;
			overlap = 1;

		} else if (overlap)
				break;
	}

	/* We are here either because this is the first reserver node
	 * or need to insert remaining non overlap addr range
	 */
	iova = __insert_new_range(iovad, pfn_lo, pfn_hi);
finish:

	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
	return iova;
}
<<<<<<< HEAD

/**
 * copy_reserved_iova - copies the reserved between domains
 * @from: - source doamin from where to copy
 * @to: - destination domin where to copy
 * This function copies reserved iova's from one doamin to
 * other.
 */
void
copy_reserved_iova(struct iova_domain *from, struct iova_domain *to)
{
	unsigned long flags;
	struct rb_node *node;

	spin_lock_irqsave(&from->iova_rbtree_lock, flags);
	for (node = rb_first(&from->rbroot); node; node = rb_next(node)) {
		struct iova *iova = container_of(node, struct iova, node);
		struct iova *new_iova;
		new_iova = reserve_iova(to, iova->pfn_lo, iova->pfn_hi);
		if (!new_iova)
			printk(KERN_ERR "Reserve iova range %lx@%lx failed\n",
				iova->pfn_lo, iova->pfn_lo);
	}
	spin_unlock_irqrestore(&from->iova_rbtree_lock, flags);
}
=======
EXPORT_SYMBOL_GPL(reserve_iova);

/*
 * Magazine caches for IOVA ranges.  For an introduction to magazines,
 * see the USENIX 2001 paper "Magazines and Vmem: Extending the Slab
 * Allocator to Many CPUs and Arbitrary Resources" by Bonwick and Adams.
 * For simplicity, we use a static magazine size and don't implement the
 * dynamic size tuning described in the paper.
 */

/*
 * As kmalloc's buffer size is fixed to power of 2, 127 is chosen to
 * assure size of 'iova_magazine' to be 1024 bytes, so that no memory
 * will be wasted. Since only full magazines are inserted into the depot,
 * we don't need to waste PFN capacity on a separate list head either.
 */
#define IOVA_MAG_SIZE 127

#define IOVA_DEPOT_DELAY msecs_to_jiffies(100)

struct iova_magazine {
	union {
		unsigned long size;
		struct iova_magazine *next;
	};
	unsigned long pfns[IOVA_MAG_SIZE];
};
static_assert(!(sizeof(struct iova_magazine) & (sizeof(struct iova_magazine) - 1)));

struct iova_cpu_rcache {
	spinlock_t lock;
	struct iova_magazine *loaded;
	struct iova_magazine *prev;
};

struct iova_rcache {
	spinlock_t lock;
	unsigned int depot_size;
	struct iova_magazine *depot;
	struct iova_cpu_rcache __percpu *cpu_rcaches;
	struct iova_domain *iovad;
	struct delayed_work work;
};

static struct kmem_cache *iova_magazine_cache;

unsigned long iova_rcache_range(void)
{
	return PAGE_SIZE << (IOVA_RANGE_CACHE_MAX_SIZE - 1);
}

static struct iova_magazine *iova_magazine_alloc(gfp_t flags)
{
	struct iova_magazine *mag;

	mag = kmem_cache_alloc(iova_magazine_cache, flags);
	if (mag)
		mag->size = 0;

	return mag;
}

static void iova_magazine_free(struct iova_magazine *mag)
{
	kmem_cache_free(iova_magazine_cache, mag);
}

static void
iova_magazine_free_pfns(struct iova_magazine *mag, struct iova_domain *iovad)
{
	unsigned long flags;
	int i;

	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);

	for (i = 0 ; i < mag->size; ++i) {
		struct iova *iova = private_find_iova(iovad, mag->pfns[i]);

		if (WARN_ON(!iova))
			continue;

		remove_iova(iovad, iova);
		free_iova_mem(iova);
	}

	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);

	mag->size = 0;
}

static bool iova_magazine_full(struct iova_magazine *mag)
{
	return mag->size == IOVA_MAG_SIZE;
}

static bool iova_magazine_empty(struct iova_magazine *mag)
{
	return mag->size == 0;
}

static unsigned long iova_magazine_pop(struct iova_magazine *mag,
				       unsigned long limit_pfn)
{
	int i;
	unsigned long pfn;

	/* Only fall back to the rbtree if we have no suitable pfns at all */
	for (i = mag->size - 1; mag->pfns[i] > limit_pfn; i--)
		if (i == 0)
			return 0;

	/* Swap it to pop it */
	pfn = mag->pfns[i];
	mag->pfns[i] = mag->pfns[--mag->size];

	return pfn;
}

static void iova_magazine_push(struct iova_magazine *mag, unsigned long pfn)
{
	mag->pfns[mag->size++] = pfn;
}

static struct iova_magazine *iova_depot_pop(struct iova_rcache *rcache)
{
	struct iova_magazine *mag = rcache->depot;

	rcache->depot = mag->next;
	mag->size = IOVA_MAG_SIZE;
	rcache->depot_size--;
	return mag;
}

static void iova_depot_push(struct iova_rcache *rcache, struct iova_magazine *mag)
{
	mag->next = rcache->depot;
	rcache->depot = mag;
	rcache->depot_size++;
}

static void iova_depot_work_func(struct work_struct *work)
{
	struct iova_rcache *rcache = container_of(work, typeof(*rcache), work.work);
	struct iova_magazine *mag = NULL;
	unsigned long flags;

	spin_lock_irqsave(&rcache->lock, flags);
	if (rcache->depot_size > num_online_cpus())
		mag = iova_depot_pop(rcache);
	spin_unlock_irqrestore(&rcache->lock, flags);

	if (mag) {
		iova_magazine_free_pfns(mag, rcache->iovad);
		iova_magazine_free(mag);
		schedule_delayed_work(&rcache->work, IOVA_DEPOT_DELAY);
	}
}

int iova_domain_init_rcaches(struct iova_domain *iovad)
{
	unsigned int cpu;
	int i, ret;

	iovad->rcaches = kcalloc(IOVA_RANGE_CACHE_MAX_SIZE,
				 sizeof(struct iova_rcache),
				 GFP_KERNEL);
	if (!iovad->rcaches)
		return -ENOMEM;

	for (i = 0; i < IOVA_RANGE_CACHE_MAX_SIZE; ++i) {
		struct iova_cpu_rcache *cpu_rcache;
		struct iova_rcache *rcache;

		rcache = &iovad->rcaches[i];
		spin_lock_init(&rcache->lock);
		rcache->iovad = iovad;
		INIT_DELAYED_WORK(&rcache->work, iova_depot_work_func);
		rcache->cpu_rcaches = __alloc_percpu(sizeof(*cpu_rcache),
						     cache_line_size());
		if (!rcache->cpu_rcaches) {
			ret = -ENOMEM;
			goto out_err;
		}
		for_each_possible_cpu(cpu) {
			cpu_rcache = per_cpu_ptr(rcache->cpu_rcaches, cpu);

			spin_lock_init(&cpu_rcache->lock);
			cpu_rcache->loaded = iova_magazine_alloc(GFP_KERNEL);
			cpu_rcache->prev = iova_magazine_alloc(GFP_KERNEL);
			if (!cpu_rcache->loaded || !cpu_rcache->prev) {
				ret = -ENOMEM;
				goto out_err;
			}
		}
	}

	ret = cpuhp_state_add_instance_nocalls(CPUHP_IOMMU_IOVA_DEAD,
					       &iovad->cpuhp_dead);
	if (ret)
		goto out_err;
	return 0;

out_err:
	free_iova_rcaches(iovad);
	return ret;
}
EXPORT_SYMBOL_GPL(iova_domain_init_rcaches);

/*
 * Try inserting IOVA range starting with 'iova_pfn' into 'rcache', and
 * return true on success.  Can fail if rcache is full and we can't free
 * space, and free_iova() (our only caller) will then return the IOVA
 * range to the rbtree instead.
 */
static bool __iova_rcache_insert(struct iova_domain *iovad,
				 struct iova_rcache *rcache,
				 unsigned long iova_pfn)
{
	struct iova_cpu_rcache *cpu_rcache;
	bool can_insert = false;
	unsigned long flags;

	cpu_rcache = raw_cpu_ptr(rcache->cpu_rcaches);
	spin_lock_irqsave(&cpu_rcache->lock, flags);

	if (!iova_magazine_full(cpu_rcache->loaded)) {
		can_insert = true;
	} else if (!iova_magazine_full(cpu_rcache->prev)) {
		swap(cpu_rcache->prev, cpu_rcache->loaded);
		can_insert = true;
	} else {
		struct iova_magazine *new_mag = iova_magazine_alloc(GFP_ATOMIC);

		if (new_mag) {
			spin_lock(&rcache->lock);
			iova_depot_push(rcache, cpu_rcache->loaded);
			spin_unlock(&rcache->lock);
			schedule_delayed_work(&rcache->work, IOVA_DEPOT_DELAY);

			cpu_rcache->loaded = new_mag;
			can_insert = true;
		}
	}

	if (can_insert)
		iova_magazine_push(cpu_rcache->loaded, iova_pfn);

	spin_unlock_irqrestore(&cpu_rcache->lock, flags);

	return can_insert;
}

static bool iova_rcache_insert(struct iova_domain *iovad, unsigned long pfn,
			       unsigned long size)
{
	unsigned int log_size = order_base_2(size);

	if (log_size >= IOVA_RANGE_CACHE_MAX_SIZE)
		return false;

	return __iova_rcache_insert(iovad, &iovad->rcaches[log_size], pfn);
}

/*
 * Caller wants to allocate a new IOVA range from 'rcache'.  If we can
 * satisfy the request, return a matching non-NULL range and remove
 * it from the 'rcache'.
 */
static unsigned long __iova_rcache_get(struct iova_rcache *rcache,
				       unsigned long limit_pfn)
{
	struct iova_cpu_rcache *cpu_rcache;
	unsigned long iova_pfn = 0;
	bool has_pfn = false;
	unsigned long flags;

	cpu_rcache = raw_cpu_ptr(rcache->cpu_rcaches);
	spin_lock_irqsave(&cpu_rcache->lock, flags);

	if (!iova_magazine_empty(cpu_rcache->loaded)) {
		has_pfn = true;
	} else if (!iova_magazine_empty(cpu_rcache->prev)) {
		swap(cpu_rcache->prev, cpu_rcache->loaded);
		has_pfn = true;
	} else {
		spin_lock(&rcache->lock);
		if (rcache->depot) {
			iova_magazine_free(cpu_rcache->loaded);
			cpu_rcache->loaded = iova_depot_pop(rcache);
			has_pfn = true;
		}
		spin_unlock(&rcache->lock);
	}

	if (has_pfn)
		iova_pfn = iova_magazine_pop(cpu_rcache->loaded, limit_pfn);

	spin_unlock_irqrestore(&cpu_rcache->lock, flags);

	return iova_pfn;
}

/*
 * Try to satisfy IOVA allocation range from rcache.  Fail if requested
 * size is too big or the DMA limit we are given isn't satisfied by the
 * top element in the magazine.
 */
static unsigned long iova_rcache_get(struct iova_domain *iovad,
				     unsigned long size,
				     unsigned long limit_pfn)
{
	unsigned int log_size = order_base_2(size);

	if (log_size >= IOVA_RANGE_CACHE_MAX_SIZE)
		return 0;

	return __iova_rcache_get(&iovad->rcaches[log_size], limit_pfn - size);
}

/*
 * free rcache data structures.
 */
static void free_iova_rcaches(struct iova_domain *iovad)
{
	struct iova_rcache *rcache;
	struct iova_cpu_rcache *cpu_rcache;
	unsigned int cpu;

	for (int i = 0; i < IOVA_RANGE_CACHE_MAX_SIZE; ++i) {
		rcache = &iovad->rcaches[i];
		if (!rcache->cpu_rcaches)
			break;
		for_each_possible_cpu(cpu) {
			cpu_rcache = per_cpu_ptr(rcache->cpu_rcaches, cpu);
			iova_magazine_free(cpu_rcache->loaded);
			iova_magazine_free(cpu_rcache->prev);
		}
		free_percpu(rcache->cpu_rcaches);
		cancel_delayed_work_sync(&rcache->work);
		while (rcache->depot)
			iova_magazine_free(iova_depot_pop(rcache));
	}

	kfree(iovad->rcaches);
	iovad->rcaches = NULL;
}

/*
 * free all the IOVA ranges cached by a cpu (used when cpu is unplugged)
 */
static void free_cpu_cached_iovas(unsigned int cpu, struct iova_domain *iovad)
{
	struct iova_cpu_rcache *cpu_rcache;
	struct iova_rcache *rcache;
	unsigned long flags;
	int i;

	for (i = 0; i < IOVA_RANGE_CACHE_MAX_SIZE; ++i) {
		rcache = &iovad->rcaches[i];
		cpu_rcache = per_cpu_ptr(rcache->cpu_rcaches, cpu);
		spin_lock_irqsave(&cpu_rcache->lock, flags);
		iova_magazine_free_pfns(cpu_rcache->loaded, iovad);
		iova_magazine_free_pfns(cpu_rcache->prev, iovad);
		spin_unlock_irqrestore(&cpu_rcache->lock, flags);
	}
}

/*
 * free all the IOVA ranges of global cache
 */
static void free_global_cached_iovas(struct iova_domain *iovad)
{
	struct iova_rcache *rcache;
	unsigned long flags;

	for (int i = 0; i < IOVA_RANGE_CACHE_MAX_SIZE; ++i) {
		rcache = &iovad->rcaches[i];
		spin_lock_irqsave(&rcache->lock, flags);
		while (rcache->depot) {
			struct iova_magazine *mag = iova_depot_pop(rcache);

			iova_magazine_free_pfns(mag, iovad);
			iova_magazine_free(mag);
		}
		spin_unlock_irqrestore(&rcache->lock, flags);
	}
}

static int iova_cpuhp_dead(unsigned int cpu, struct hlist_node *node)
{
	struct iova_domain *iovad;

	iovad = hlist_entry_safe(node, struct iova_domain, cpuhp_dead);

	free_cpu_cached_iovas(cpu, iovad);
	return 0;
}

int iova_cache_get(void)
{
	int err = -ENOMEM;

	mutex_lock(&iova_cache_mutex);
	if (!iova_cache_users) {
		iova_cache = kmem_cache_create("iommu_iova", sizeof(struct iova), 0,
					       SLAB_HWCACHE_ALIGN, NULL);
		if (!iova_cache)
			goto out_err;

		iova_magazine_cache = kmem_cache_create("iommu_iova_magazine",
							sizeof(struct iova_magazine),
							0, SLAB_HWCACHE_ALIGN, NULL);
		if (!iova_magazine_cache)
			goto out_err;

		err = cpuhp_setup_state_multi(CPUHP_IOMMU_IOVA_DEAD, "iommu/iova:dead",
					      NULL, iova_cpuhp_dead);
		if (err) {
			pr_err("IOVA: Couldn't register cpuhp handler: %pe\n", ERR_PTR(err));
			goto out_err;
		}
	}

	iova_cache_users++;
	mutex_unlock(&iova_cache_mutex);

	return 0;

out_err:
	kmem_cache_destroy(iova_cache);
	kmem_cache_destroy(iova_magazine_cache);
	mutex_unlock(&iova_cache_mutex);
	return err;
}
EXPORT_SYMBOL_GPL(iova_cache_get);

void iova_cache_put(void)
{
	mutex_lock(&iova_cache_mutex);
	if (WARN_ON(!iova_cache_users)) {
		mutex_unlock(&iova_cache_mutex);
		return;
	}
	iova_cache_users--;
	if (!iova_cache_users) {
		cpuhp_remove_multi_state(CPUHP_IOMMU_IOVA_DEAD);
		kmem_cache_destroy(iova_cache);
		kmem_cache_destroy(iova_magazine_cache);
	}
	mutex_unlock(&iova_cache_mutex);
}
EXPORT_SYMBOL_GPL(iova_cache_put);

MODULE_AUTHOR("Anil S Keshavamurthy <anil.s.keshavamurthy@intel.com>");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
