<<<<<<< HEAD
/*
 * 2002-10-18  written by Jim Houston jim.houston@ccur.com
 *	Copyright (C) 2002 by Concurrent Computer Corporation
 *	Distributed under the GNU GPL license version 2.
 *
 * Modified by George Anzinger to reuse immediately and to use
 * find bit instructions.  Also removed _irq on spinlocks.
 *
 * Modified by Nadia Derbey to make it RCU safe.
 *
 * Small id to pointer translation service.
 *
 * It uses a radix tree like structure as a sparse array indexed
 * by the id to obtain the pointer.  The bitmap makes allocating
 * a new id quick.
 *
 * You call it to allocate an id (an int) an associate with that id a
 * pointer or what ever, we treat it as a (void *).  You can pass this
 * id to a user for him to pass back at a later time.  You then pass
 * that id to this code and it returns your pointer.

 * You can release ids at any time. When all ids are released, most of
 * the memory is returned (we keep IDR_FREE_MAX) in a local pool so we
 * don't need to go to the memory "store" during an id allocate, just
 * so you don't need to be too concerned about locking and conflicts
 * with the slab allocator.
 */

#ifndef TEST                        // to test in user space...
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/export.h>
#endif
#include <linux/err.h>
#include <linux/string.h>
#include <linux/idr.h>
#include <linux/spinlock.h>

static struct kmem_cache *idr_layer_cache;
static DEFINE_SPINLOCK(simple_ida_lock);

/* the maximum ID which can be allocated given idr->layers */
static int idr_max(int layers)
{
	int bits = min_t(int, layers * IDR_BITS, MAX_ID_SHIFT);

	return (1 << bits) - 1;
}

static struct idr_layer *get_from_free_list(struct idr *idp)
{
	struct idr_layer *p;
	unsigned long flags;

	spin_lock_irqsave(&idp->lock, flags);
	if ((p = idp->id_free)) {
		idp->id_free = p->ary[0];
		idp->id_free_cnt--;
		p->ary[0] = NULL;
	}
	spin_unlock_irqrestore(&idp->lock, flags);
	return(p);
}

static void idr_layer_rcu_free(struct rcu_head *head)
{
	struct idr_layer *layer;

	layer = container_of(head, struct idr_layer, rcu_head);
	kmem_cache_free(idr_layer_cache, layer);
}

static inline void free_layer(struct idr_layer *p)
{
	call_rcu(&p->rcu_head, idr_layer_rcu_free);
}

/* only called when idp->lock is held */
static void __move_to_free_list(struct idr *idp, struct idr_layer *p)
{
	p->ary[0] = idp->id_free;
	idp->id_free = p;
	idp->id_free_cnt++;
}

static void move_to_free_list(struct idr *idp, struct idr_layer *p)
{
	unsigned long flags;

	/*
	 * Depends on the return element being zeroed.
	 */
	spin_lock_irqsave(&idp->lock, flags);
	__move_to_free_list(idp, p);
	spin_unlock_irqrestore(&idp->lock, flags);
}

static void idr_mark_full(struct idr_layer **pa, int id)
{
	struct idr_layer *p = pa[0];
	int l = 0;

	__set_bit(id & IDR_MASK, &p->bitmap);
	/*
	 * If this layer is full mark the bit in the layer above to
	 * show that this part of the radix tree is full.  This may
	 * complete the layer above and require walking up the radix
	 * tree.
	 */
	while (p->bitmap == IDR_FULL) {
		if (!(p = pa[++l]))
			break;
		id = id >> IDR_BITS;
		__set_bit((id & IDR_MASK), &p->bitmap);
	}
}

/**
 * idr_pre_get - reserve resources for idr allocation
 * @idp:	idr handle
 * @gfp_mask:	memory allocation flags
 *
 * This function should be called prior to calling the idr_get_new* functions.
 * It preallocates enough memory to satisfy the worst possible allocation. The
 * caller should pass in GFP_KERNEL if possible.  This of course requires that
 * no spinning locks be held.
 *
 * If the system is REALLY out of memory this function returns %0,
 * otherwise %1.
 */
int idr_pre_get(struct idr *idp, gfp_t gfp_mask)
{
	while (idp->id_free_cnt < IDR_FREE_MAX) {
		struct idr_layer *new;
		new = kmem_cache_zalloc(idr_layer_cache, gfp_mask);
		if (new == NULL)
			return (0);
		move_to_free_list(idp, new);
	}
	return 1;
}
EXPORT_SYMBOL(idr_pre_get);

static int sub_alloc(struct idr *idp, int *starting_id, struct idr_layer **pa)
{
	int n, m, sh;
	struct idr_layer *p, *new;
	int l, id, oid;
	unsigned long bm;

	id = *starting_id;
 restart:
	p = idp->top;
	l = idp->layers;
	pa[l--] = NULL;
	while (1) {
		/*
		 * We run around this while until we reach the leaf node...
		 */
		n = (id >> (IDR_BITS*l)) & IDR_MASK;
		bm = ~p->bitmap;
		m = find_next_bit(&bm, IDR_SIZE, n);
		if (m == IDR_SIZE) {
			/* no space available go back to previous layer. */
			l++;
			oid = id;
			id = (id | ((1 << (IDR_BITS * l)) - 1)) + 1;

			/* if already at the top layer, we need to grow */
			if (id >= 1 << (idp->layers * IDR_BITS)) {
				*starting_id = id;
				return IDR_NEED_TO_GROW;
			}
			p = pa[l];
			BUG_ON(!p);

			/* If we need to go up one layer, continue the
			 * loop; otherwise, restart from the top.
			 */
			sh = IDR_BITS * (l + 1);
			if (oid >> sh == id >> sh)
				continue;
			else
				goto restart;
		}
		if (m != n) {
			sh = IDR_BITS*l;
			id = ((id >> sh) ^ n ^ m) << sh;
		}
		if ((id >= MAX_ID_BIT) || (id < 0))
			return IDR_NOMORE_SPACE;
		if (l == 0)
			break;
		/*
		 * Create the layer below if it is missing.
		 */
		if (!p->ary[m]) {
			new = get_from_free_list(idp);
			if (!new)
				return -1;
			new->layer = l-1;
			rcu_assign_pointer(p->ary[m], new);
			p->count++;
		}
		pa[l--] = p;
		p = p->ary[m];
	}

	pa[l] = p;
	return id;
}

static int idr_get_empty_slot(struct idr *idp, int starting_id,
			      struct idr_layer **pa)
{
	struct idr_layer *p, *new;
	int layers, v, id;
	unsigned long flags;

	id = starting_id;
build_up:
	p = idp->top;
	layers = idp->layers;
	if (unlikely(!p)) {
		if (!(p = get_from_free_list(idp)))
			return -1;
		p->layer = 0;
		layers = 1;
	}
	/*
	 * Add a new layer to the top of the tree if the requested
	 * id is larger than the currently allocated space.
	 */
	while (id > idr_max(layers)) {
		layers++;
		if (!p->count) {
			/* special case: if the tree is currently empty,
			 * then we grow the tree by moving the top node
			 * upwards.
			 */
			p->layer++;
			continue;
		}
		if (!(new = get_from_free_list(idp))) {
			/*
			 * The allocation failed.  If we built part of
			 * the structure tear it down.
			 */
			spin_lock_irqsave(&idp->lock, flags);
			for (new = p; p && p != idp->top; new = p) {
				p = p->ary[0];
				new->ary[0] = NULL;
				new->bitmap = new->count = 0;
				__move_to_free_list(idp, new);
			}
			spin_unlock_irqrestore(&idp->lock, flags);
			return -1;
		}
		new->ary[0] = p;
		new->count = 1;
		new->layer = layers-1;
		if (p->bitmap == IDR_FULL)
			__set_bit(0, &new->bitmap);
		p = new;
	}
	rcu_assign_pointer(idp->top, p);
	idp->layers = layers;
	v = sub_alloc(idp, &id, pa);
	if (v == IDR_NEED_TO_GROW)
		goto build_up;
	return(v);
}

static int idr_get_new_above_int(struct idr *idp, void *ptr, int starting_id)
{
	struct idr_layer *pa[MAX_LEVEL + 1];
	int id;

	id = idr_get_empty_slot(idp, starting_id, pa);
	if (id >= 0) {
		/*
		 * Successfully found an empty slot.  Install the user
		 * pointer and mark the slot full.
		 */
		rcu_assign_pointer(pa[0]->ary[id & IDR_MASK],
				(struct idr_layer *)ptr);
		pa[0]->count++;
		idr_mark_full(pa, id);
	}

	return id;
}

/**
 * idr_get_new_above - allocate new idr entry above or equal to a start id
 * @idp: idr handle
 * @ptr: pointer you want associated with the id
 * @starting_id: id to start search at
 * @id: pointer to the allocated handle
 *
 * This is the allocate id function.  It should be called with any
 * required locks.
 *
 * If allocation from IDR's private freelist fails, idr_get_new_above() will
 * return %-EAGAIN.  The caller should retry the idr_pre_get() call to refill
 * IDR's preallocation and then retry the idr_get_new_above() call.
 *
 * If the idr is full idr_get_new_above() will return %-ENOSPC.
 *
 * @id returns a value in the range @starting_id ... %0x7fffffff
 */
int idr_get_new_above(struct idr *idp, void *ptr, int starting_id, int *id)
{
	int rv;

	rv = idr_get_new_above_int(idp, ptr, starting_id);
	/*
	 * This is a cheap hack until the IDR code can be fixed to
	 * return proper error values.
	 */
	if (rv < 0)
		return _idr_rc_to_errno(rv);
	*id = rv;
	return 0;
}
EXPORT_SYMBOL(idr_get_new_above);

/**
 * idr_get_new - allocate new idr entry
 * @idp: idr handle
 * @ptr: pointer you want associated with the id
 * @id: pointer to the allocated handle
 *
 * If allocation from IDR's private freelist fails, idr_get_new_above() will
 * return %-EAGAIN.  The caller should retry the idr_pre_get() call to refill
 * IDR's preallocation and then retry the idr_get_new_above() call.
 *
 * If the idr is full idr_get_new_above() will return %-ENOSPC.
 *
 * @id returns a value in the range %0 ... %0x7fffffff
 */
int idr_get_new(struct idr *idp, void *ptr, int *id)
{
	int rv;

	rv = idr_get_new_above_int(idp, ptr, 0);
	/*
	 * This is a cheap hack until the IDR code can be fixed to
	 * return proper error values.
	 */
	if (rv < 0)
		return _idr_rc_to_errno(rv);
	*id = rv;
	return 0;
}
EXPORT_SYMBOL(idr_get_new);

static void idr_remove_warning(int id)
{
	printk(KERN_WARNING
		"idr_remove called for id=%d which is not allocated.\n", id);
	dump_stack();
}

static void sub_remove(struct idr *idp, int shift, int id)
{
	struct idr_layer *p = idp->top;
	struct idr_layer **pa[MAX_LEVEL + 1];
	struct idr_layer ***paa = &pa[0];
	struct idr_layer *to_free;
	int n;

	*paa = NULL;
	*++paa = &idp->top;

	while ((shift > 0) && p) {
		n = (id >> shift) & IDR_MASK;
		__clear_bit(n, &p->bitmap);
		*++paa = &p->ary[n];
		p = p->ary[n];
		shift -= IDR_BITS;
	}
	n = id & IDR_MASK;
	if (likely(p != NULL && test_bit(n, &p->bitmap))){
		__clear_bit(n, &p->bitmap);
		rcu_assign_pointer(p->ary[n], NULL);
		to_free = NULL;
		while(*paa && ! --((**paa)->count)){
			if (to_free)
				free_layer(to_free);
			to_free = **paa;
			**paa-- = NULL;
		}
		if (!*paa)
			idp->layers = 0;
		if (to_free)
			free_layer(to_free);
	} else
		idr_remove_warning(id);
}

/**
 * idr_remove - remove the given id and free its slot
 * @idp: idr handle
 * @id: unique key
 */
void idr_remove(struct idr *idp, int id)
{
	struct idr_layer *p;
	struct idr_layer *to_free;

	/* Mask off upper bits we don't use for the search. */
	id &= MAX_ID_MASK;

	sub_remove(idp, (idp->layers - 1) * IDR_BITS, id);
	if (idp->top && idp->top->count == 1 && (idp->layers > 1) &&
	    idp->top->ary[0]) {
		/*
		 * Single child at leftmost slot: we can shrink the tree.
		 * This level is not needed anymore since when layers are
		 * inserted, they are inserted at the top of the existing
		 * tree.
		 */
		to_free = idp->top;
		p = idp->top->ary[0];
		rcu_assign_pointer(idp->top, p);
		--idp->layers;
		to_free->bitmap = to_free->count = 0;
		free_layer(to_free);
	}
	while (idp->id_free_cnt >= IDR_FREE_MAX) {
		p = get_from_free_list(idp);
		/*
		 * Note: we don't call the rcu callback here, since the only
		 * layers that fall into the freelist are those that have been
		 * preallocated.
		 */
		kmem_cache_free(idr_layer_cache, p);
	}
	return;
}
EXPORT_SYMBOL(idr_remove);

/**
 * idr_remove_all - remove all ids from the given idr tree
 * @idp: idr handle
 *
 * idr_destroy() only frees up unused, cached idp_layers, but this
 * function will remove all id mappings and leave all idp_layers
 * unused.
 *
 * A typical clean-up sequence for objects stored in an idr tree will
 * use idr_for_each() to free all objects, if necessay, then
 * idr_remove_all() to remove all ids, and idr_destroy() to free
 * up the cached idr_layers.
 */
void idr_remove_all(struct idr *idp)
{
	int n, id, max;
	int bt_mask;
	struct idr_layer *p;
	struct idr_layer *pa[MAX_LEVEL + 1];
	struct idr_layer **paa = &pa[0];

	n = idp->layers * IDR_BITS;
	p = idp->top;
	rcu_assign_pointer(idp->top, NULL);
	max = idr_max(idp->layers);

	id = 0;
	while (id >= 0 && id <= max) {
		while (n > IDR_BITS && p) {
			n -= IDR_BITS;
			*paa++ = p;
			p = p->ary[(id >> n) & IDR_MASK];
		}

		bt_mask = id;
		id += 1 << n;
		/* Get the highest bit that the above add changed from 0->1. */
		while (n < fls(id ^ bt_mask)) {
			if (p)
				free_layer(p);
			n += IDR_BITS;
			p = *--paa;
		}
	}
	idp->layers = 0;
}
EXPORT_SYMBOL(idr_remove_all);

/**
 * idr_destroy - release all cached layers within an idr tree
 * @idp: idr handle
 */
void idr_destroy(struct idr *idp)
{
	while (idp->id_free_cnt) {
		struct idr_layer *p = get_from_free_list(idp);
		kmem_cache_free(idr_layer_cache, p);
	}
}
EXPORT_SYMBOL(idr_destroy);

/**
 * idr_find - return pointer for given id
 * @idp: idr handle
 * @id: lookup key
 *
 * Return the pointer given the id it has been registered with.  A %NULL
 * return indicates that @id is not valid or you passed %NULL in
 * idr_get_new().
 *
 * This function can be called under rcu_read_lock(), given that the leaf
 * pointers lifetimes are correctly managed.
 */
void *idr_find(struct idr *idp, int id)
{
	int n;
	struct idr_layer *p;

	p = rcu_dereference_raw(idp->top);
	if (!p)
		return NULL;
	n = (p->layer+1) * IDR_BITS;

	/* Mask off upper bits we don't use for the search. */
	id &= MAX_ID_MASK;

	if (id > idr_max(p->layer + 1))
		return NULL;
	BUG_ON(n == 0);

	while (n > 0 && p) {
		n -= IDR_BITS;
		BUG_ON(n != p->layer*IDR_BITS);
		p = rcu_dereference_raw(p->ary[(id >> n) & IDR_MASK]);
	}
	return((void *)p);
}
EXPORT_SYMBOL(idr_find);

/**
 * idr_for_each - iterate through all stored pointers
 * @idp: idr handle
 * @fn: function to be called for each pointer
 * @data: data passed back to callback function
 *
 * Iterate over the pointers registered with the given idr.  The
 * callback function will be called for each pointer currently
 * registered, passing the id, the pointer and the data pointer passed
 * to this function.  It is not safe to modify the idr tree while in
 * the callback, so functions such as idr_get_new and idr_remove are
 * not allowed.
 *
 * We check the return of @fn each time. If it returns anything other
 * than %0, we break out and return that value.
 *
 * The caller must serialize idr_for_each() vs idr_get_new() and idr_remove().
 */
int idr_for_each(struct idr *idp,
		 int (*fn)(int id, void *p, void *data), void *data)
{
	int n, id, max, error = 0;
	struct idr_layer *p;
	struct idr_layer *pa[MAX_LEVEL + 1];
	struct idr_layer **paa = &pa[0];

	n = idp->layers * IDR_BITS;
	p = rcu_dereference_raw(idp->top);
	max = idr_max(idp->layers);

	id = 0;
	while (id >= 0 && id <= max) {
		while (n > 0 && p) {
			n -= IDR_BITS;
			*paa++ = p;
			p = rcu_dereference_raw(p->ary[(id >> n) & IDR_MASK]);
		}

		if (p) {
			error = fn(id, (void *)p, data);
			if (error)
				break;
		}

		id += 1 << n;
		while (n < fls(id)) {
			n += IDR_BITS;
			p = *--paa;
		}
	}

	return error;
=======
// SPDX-License-Identifier: GPL-2.0-only
#include <linux/bitmap.h>
#include <linux/bug.h>
#include <linux/export.h>
#include <linux/idr.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/xarray.h>

/**
 * idr_alloc_u32() - Allocate an ID.
 * @idr: IDR handle.
 * @ptr: Pointer to be associated with the new ID.
 * @nextid: Pointer to an ID.
 * @max: The maximum ID to allocate (inclusive).
 * @gfp: Memory allocation flags.
 *
 * Allocates an unused ID in the range specified by @nextid and @max.
 * Note that @max is inclusive whereas the @end parameter to idr_alloc()
 * is exclusive.  The new ID is assigned to @nextid before the pointer
 * is inserted into the IDR, so if @nextid points into the object pointed
 * to by @ptr, a concurrent lookup will not find an uninitialised ID.
 *
 * The caller should provide their own locking to ensure that two
 * concurrent modifications to the IDR are not possible.  Read-only
 * accesses to the IDR may be done under the RCU read lock or may
 * exclude simultaneous writers.
 *
 * Return: 0 if an ID was allocated, -ENOMEM if memory allocation failed,
 * or -ENOSPC if no free IDs could be found.  If an error occurred,
 * @nextid is unchanged.
 */
int idr_alloc_u32(struct idr *idr, void *ptr, u32 *nextid,
			unsigned long max, gfp_t gfp)
{
	struct radix_tree_iter iter;
	void __rcu **slot;
	unsigned int base = idr->idr_base;
	unsigned int id = *nextid;

	if (WARN_ON_ONCE(!(idr->idr_rt.xa_flags & ROOT_IS_IDR)))
		idr->idr_rt.xa_flags |= IDR_RT_MARKER;

	id = (id < base) ? 0 : id - base;
	radix_tree_iter_init(&iter, id);
	slot = idr_get_free(&idr->idr_rt, &iter, gfp, max - base);
	if (IS_ERR(slot))
		return PTR_ERR(slot);

	*nextid = iter.index + base;
	/* there is a memory barrier inside radix_tree_iter_replace() */
	radix_tree_iter_replace(&idr->idr_rt, &iter, slot, ptr);
	radix_tree_iter_tag_clear(&idr->idr_rt, &iter, IDR_FREE);

	return 0;
}
EXPORT_SYMBOL_GPL(idr_alloc_u32);

/**
 * idr_alloc() - Allocate an ID.
 * @idr: IDR handle.
 * @ptr: Pointer to be associated with the new ID.
 * @start: The minimum ID (inclusive).
 * @end: The maximum ID (exclusive).
 * @gfp: Memory allocation flags.
 *
 * Allocates an unused ID in the range specified by @start and @end.  If
 * @end is <= 0, it is treated as one larger than %INT_MAX.  This allows
 * callers to use @start + N as @end as long as N is within integer range.
 *
 * The caller should provide their own locking to ensure that two
 * concurrent modifications to the IDR are not possible.  Read-only
 * accesses to the IDR may be done under the RCU read lock or may
 * exclude simultaneous writers.
 *
 * Return: The newly allocated ID, -ENOMEM if memory allocation failed,
 * or -ENOSPC if no free IDs could be found.
 */
int idr_alloc(struct idr *idr, void *ptr, int start, int end, gfp_t gfp)
{
	u32 id = start;
	int ret;

	if (WARN_ON_ONCE(start < 0))
		return -EINVAL;

	ret = idr_alloc_u32(idr, ptr, &id, end > 0 ? end - 1 : INT_MAX, gfp);
	if (ret)
		return ret;

	return id;
}
EXPORT_SYMBOL_GPL(idr_alloc);

/**
 * idr_alloc_cyclic() - Allocate an ID cyclically.
 * @idr: IDR handle.
 * @ptr: Pointer to be associated with the new ID.
 * @start: The minimum ID (inclusive).
 * @end: The maximum ID (exclusive).
 * @gfp: Memory allocation flags.
 *
 * Allocates an unused ID in the range specified by @start and @end.  If
 * @end is <= 0, it is treated as one larger than %INT_MAX.  This allows
 * callers to use @start + N as @end as long as N is within integer range.
 * The search for an unused ID will start at the last ID allocated and will
 * wrap around to @start if no free IDs are found before reaching @end.
 *
 * The caller should provide their own locking to ensure that two
 * concurrent modifications to the IDR are not possible.  Read-only
 * accesses to the IDR may be done under the RCU read lock or may
 * exclude simultaneous writers.
 *
 * Return: The newly allocated ID, -ENOMEM if memory allocation failed,
 * or -ENOSPC if no free IDs could be found.
 */
int idr_alloc_cyclic(struct idr *idr, void *ptr, int start, int end, gfp_t gfp)
{
	u32 id = idr->idr_next;
	int err, max = end > 0 ? end - 1 : INT_MAX;

	if ((int)id < start)
		id = start;

	err = idr_alloc_u32(idr, ptr, &id, max, gfp);
	if ((err == -ENOSPC) && (id > start)) {
		id = start;
		err = idr_alloc_u32(idr, ptr, &id, max, gfp);
	}
	if (err)
		return err;

	idr->idr_next = id + 1;
	return id;
}
EXPORT_SYMBOL(idr_alloc_cyclic);

/**
 * idr_remove() - Remove an ID from the IDR.
 * @idr: IDR handle.
 * @id: Pointer ID.
 *
 * Removes this ID from the IDR.  If the ID was not previously in the IDR,
 * this function returns %NULL.
 *
 * Since this function modifies the IDR, the caller should provide their
 * own locking to ensure that concurrent modification of the same IDR is
 * not possible.
 *
 * Return: The pointer formerly associated with this ID.
 */
void *idr_remove(struct idr *idr, unsigned long id)
{
	return radix_tree_delete_item(&idr->idr_rt, id - idr->idr_base, NULL);
}
EXPORT_SYMBOL_GPL(idr_remove);

/**
 * idr_find() - Return pointer for given ID.
 * @idr: IDR handle.
 * @id: Pointer ID.
 *
 * Looks up the pointer associated with this ID.  A %NULL pointer may
 * indicate that @id is not allocated or that the %NULL pointer was
 * associated with this ID.
 *
 * This function can be called under rcu_read_lock(), given that the leaf
 * pointers lifetimes are correctly managed.
 *
 * Return: The pointer associated with this ID.
 */
void *idr_find(const struct idr *idr, unsigned long id)
{
	return radix_tree_lookup(&idr->idr_rt, id - idr->idr_base);
}
EXPORT_SYMBOL_GPL(idr_find);

/**
 * idr_for_each() - Iterate through all stored pointers.
 * @idr: IDR handle.
 * @fn: Function to be called for each pointer.
 * @data: Data passed to callback function.
 *
 * The callback function will be called for each entry in @idr, passing
 * the ID, the entry and @data.
 *
 * If @fn returns anything other than %0, the iteration stops and that
 * value is returned from this function.
 *
 * idr_for_each() can be called concurrently with idr_alloc() and
 * idr_remove() if protected by RCU.  Newly added entries may not be
 * seen and deleted entries may be seen, but adding and removing entries
 * will not cause other entries to be skipped, nor spurious ones to be seen.
 */
int idr_for_each(const struct idr *idr,
		int (*fn)(int id, void *p, void *data), void *data)
{
	struct radix_tree_iter iter;
	void __rcu **slot;
	int base = idr->idr_base;

	radix_tree_for_each_slot(slot, &idr->idr_rt, &iter, 0) {
		int ret;
		unsigned long id = iter.index + base;

		if (WARN_ON_ONCE(id > INT_MAX))
			break;
		ret = fn(id, rcu_dereference_raw(*slot), data);
		if (ret)
			return ret;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(idr_for_each);

/**
<<<<<<< HEAD
 * idr_get_next - lookup next object of id to given id.
 * @idp: idr handle
 * @nextidp:  pointer to lookup key
 *
 * Returns pointer to registered object with id, which is next number to
 * given id. After being looked up, *@nextidp will be updated for the next
 * iteration.
 *
 * This function can be called under rcu_read_lock(), given that the leaf
 * pointers lifetimes are correctly managed.
 */
void *idr_get_next(struct idr *idp, int *nextidp)
{
	struct idr_layer *p, *pa[MAX_LEVEL + 1];
	struct idr_layer **paa = &pa[0];
	int id = *nextidp;
	int n, max;

	/* find first ent */
	p = rcu_dereference_raw(idp->top);
	if (!p)
		return NULL;
	n = (p->layer + 1) * IDR_BITS;
	max = idr_max(p->layer + 1);

	while (id >= 0 && id <= max) {
		while (n > 0 && p) {
			n -= IDR_BITS;
			*paa++ = p;
			p = rcu_dereference_raw(p->ary[(id >> n) & IDR_MASK]);
		}

		if (p) {
			*nextidp = id;
			return p;
		}

		/*
		 * Proceed to the next layer at the current level.  Unlike
		 * idr_for_each(), @id isn't guaranteed to be aligned to
		 * layer boundary at this point and adding 1 << n may
		 * incorrectly skip IDs.  Make sure we jump to the
		 * beginning of the next layer using round_up().
		 */
		id = round_up(id + 1, 1 << n);
		while (n < fls(id)) {
			n += IDR_BITS;
			p = *--paa;
		}
	}
	return NULL;
}
EXPORT_SYMBOL(idr_get_next);


/**
 * idr_replace - replace pointer for given id
 * @idp: idr handle
 * @ptr: pointer you want associated with the id
 * @id: lookup key
 *
 * Replace the pointer registered with an id and return the old value.
 * A %-ENOENT return indicates that @id was not found.
 * A %-EINVAL return indicates that @id was not within valid constraints.
 *
 * The caller must serialize with writers.
 */
void *idr_replace(struct idr *idp, void *ptr, int id)
{
	int n;
	struct idr_layer *p, *old_p;

	p = idp->top;
	if (!p)
		return ERR_PTR(-EINVAL);

	n = (p->layer+1) * IDR_BITS;

	id &= MAX_ID_MASK;

	if (id >= (1 << n))
		return ERR_PTR(-EINVAL);

	n -= IDR_BITS;
	while ((n > 0) && p) {
		p = p->ary[(id >> n) & IDR_MASK];
		n -= IDR_BITS;
	}

	n = id & IDR_MASK;
	if (unlikely(p == NULL || !test_bit(n, &p->bitmap)))
		return ERR_PTR(-ENOENT);

	old_p = p->ary[n];
	rcu_assign_pointer(p->ary[n], ptr);

	return old_p;
}
EXPORT_SYMBOL(idr_replace);

void __init idr_init_cache(void)
{
	idr_layer_cache = kmem_cache_create("idr_layer_cache",
				sizeof(struct idr_layer), 0, SLAB_PANIC, NULL);
}

/**
 * idr_init - initialize idr handle
 * @idp:	idr handle
 *
 * This function is use to set up the handle (@idp) that you will pass
 * to the rest of the functions.
 */
void idr_init(struct idr *idp)
{
	memset(idp, 0, sizeof(struct idr));
	spin_lock_init(&idp->lock);
}
EXPORT_SYMBOL(idr_init);


/**
 * DOC: IDA description
 * IDA - IDR based ID allocator
 *
 * This is id allocator without id -> pointer translation.  Memory
 * usage is much lower than full blown idr because each id only
 * occupies a bit.  ida uses a custom leaf node which contains
 * IDA_BITMAP_BITS slots.
 *
 * 2007-04-25  written by Tejun Heo <htejun@gmail.com>
 */

static void free_bitmap(struct ida *ida, struct ida_bitmap *bitmap)
{
	unsigned long flags;

	if (!ida->free_bitmap) {
		spin_lock_irqsave(&ida->idr.lock, flags);
		if (!ida->free_bitmap) {
			ida->free_bitmap = bitmap;
			bitmap = NULL;
		}
		spin_unlock_irqrestore(&ida->idr.lock, flags);
	}

	kfree(bitmap);
}

/**
 * ida_pre_get - reserve resources for ida allocation
 * @ida:	ida handle
 * @gfp_mask:	memory allocation flag
 *
 * This function should be called prior to locking and calling the
 * following function.  It preallocates enough memory to satisfy the
 * worst possible allocation.
 *
 * If the system is REALLY out of memory this function returns %0,
 * otherwise %1.
 */
int ida_pre_get(struct ida *ida, gfp_t gfp_mask)
{
	/* allocate idr_layers */
	if (!idr_pre_get(&ida->idr, gfp_mask))
		return 0;

	/* allocate free_bitmap */
	if (!ida->free_bitmap) {
		struct ida_bitmap *bitmap;

		bitmap = kmalloc(sizeof(struct ida_bitmap), gfp_mask);
		if (!bitmap)
			return 0;

		free_bitmap(ida, bitmap);
	}

	return 1;
}
EXPORT_SYMBOL(ida_pre_get);

/**
 * ida_get_new_above - allocate new ID above or equal to a start id
 * @ida:	ida handle
 * @starting_id: id to start search at
 * @p_id:	pointer to the allocated handle
 *
 * Allocate new ID above or equal to @starting_id.  It should be called
 * with any required locks.
 *
 * If memory is required, it will return %-EAGAIN, you should unlock
 * and go back to the ida_pre_get() call.  If the ida is full, it will
 * return %-ENOSPC.
 *
 * @p_id returns a value in the range @starting_id ... %0x7fffffff.
 */
int ida_get_new_above(struct ida *ida, int starting_id, int *p_id)
{
	struct idr_layer *pa[MAX_LEVEL + 1];
	struct ida_bitmap *bitmap;
	unsigned long flags;
	int idr_id = starting_id / IDA_BITMAP_BITS;
	int offset = starting_id % IDA_BITMAP_BITS;
	int t, id;

 restart:
	/* get vacant slot */
	t = idr_get_empty_slot(&ida->idr, idr_id, pa);
	if (t < 0)
		return _idr_rc_to_errno(t);

	if (t * IDA_BITMAP_BITS >= MAX_ID_BIT)
		return -ENOSPC;

	if (t != idr_id)
		offset = 0;
	idr_id = t;

	/* if bitmap isn't there, create a new one */
	bitmap = (void *)pa[0]->ary[idr_id & IDR_MASK];
	if (!bitmap) {
		spin_lock_irqsave(&ida->idr.lock, flags);
		bitmap = ida->free_bitmap;
		ida->free_bitmap = NULL;
		spin_unlock_irqrestore(&ida->idr.lock, flags);

		if (!bitmap)
			return -EAGAIN;

		memset(bitmap, 0, sizeof(struct ida_bitmap));
		rcu_assign_pointer(pa[0]->ary[idr_id & IDR_MASK],
				(void *)bitmap);
		pa[0]->count++;
	}

	/* lookup for empty slot */
	t = find_next_zero_bit(bitmap->bitmap, IDA_BITMAP_BITS, offset);
	if (t == IDA_BITMAP_BITS) {
		/* no empty slot after offset, continue to the next chunk */
		idr_id++;
		offset = 0;
		goto restart;
	}

	id = idr_id * IDA_BITMAP_BITS + t;
	if (id >= MAX_ID_BIT)
		return -ENOSPC;

	__set_bit(t, bitmap->bitmap);
	if (++bitmap->nr_busy == IDA_BITMAP_BITS)
		idr_mark_full(pa, idr_id);

	*p_id = id;

	/* Each leaf node can handle nearly a thousand slots and the
	 * whole idea of ida is to have small memory foot print.
	 * Throw away extra resources one by one after each successful
	 * allocation.
	 */
	if (ida->idr.id_free_cnt || ida->free_bitmap) {
		struct idr_layer *p = get_from_free_list(&ida->idr);
		if (p)
			kmem_cache_free(idr_layer_cache, p);
	}

	return 0;
}
EXPORT_SYMBOL(ida_get_new_above);

/**
 * ida_get_new - allocate new ID
 * @ida:	idr handle
 * @p_id:	pointer to the allocated handle
 *
 * Allocate new ID.  It should be called with any required locks.
 *
 * If memory is required, it will return %-EAGAIN, you should unlock
 * and go back to the idr_pre_get() call.  If the idr is full, it will
 * return %-ENOSPC.
 *
 * @p_id returns a value in the range %0 ... %0x7fffffff.
 */
int ida_get_new(struct ida *ida, int *p_id)
{
	return ida_get_new_above(ida, 0, p_id);
}
EXPORT_SYMBOL(ida_get_new);

/**
 * ida_remove - remove the given ID
 * @ida:	ida handle
 * @id:		ID to free
 */
void ida_remove(struct ida *ida, int id)
{
	struct idr_layer *p = ida->idr.top;
	int shift = (ida->idr.layers - 1) * IDR_BITS;
	int idr_id = id / IDA_BITMAP_BITS;
	int offset = id % IDA_BITMAP_BITS;
	int n;
	struct ida_bitmap *bitmap;

	/* clear full bits while looking up the leaf idr_layer */
	while ((shift > 0) && p) {
		n = (idr_id >> shift) & IDR_MASK;
		__clear_bit(n, &p->bitmap);
		p = p->ary[n];
		shift -= IDR_BITS;
	}

	if (p == NULL)
		goto err;

	n = idr_id & IDR_MASK;
	__clear_bit(n, &p->bitmap);

	bitmap = (void *)p->ary[n];
	if (!test_bit(offset, bitmap->bitmap))
		goto err;

	/* update bitmap and remove it if empty */
	__clear_bit(offset, bitmap->bitmap);
	if (--bitmap->nr_busy == 0) {
		__set_bit(n, &p->bitmap);	/* to please idr_remove() */
		idr_remove(&ida->idr, idr_id);
		free_bitmap(ida, bitmap);
	}

	return;

 err:
	printk(KERN_WARNING
	       "ida_remove called for id=%d which is not allocated.\n", id);
}
EXPORT_SYMBOL(ida_remove);

/**
 * ida_destroy - release all cached layers within an ida tree
 * @ida:		ida handle
 */
void ida_destroy(struct ida *ida)
{
	idr_destroy(&ida->idr);
	kfree(ida->free_bitmap);
}
EXPORT_SYMBOL(ida_destroy);

/**
 * ida_simple_get - get a new id.
 * @ida: the (initialized) ida.
 * @start: the minimum id (inclusive, < 0x8000000)
 * @end: the maximum id (exclusive, < 0x8000000 or 0)
 * @gfp_mask: memory allocation flags
 *
 * Allocates an id in the range start <= id < end, or returns -ENOSPC.
 * On memory allocation failure, returns -ENOMEM.
 *
 * Use ida_simple_remove() to get rid of an id.
 */
int ida_simple_get(struct ida *ida, unsigned int start, unsigned int end,
		   gfp_t gfp_mask)
{
	int ret, id;
	unsigned int max;
	unsigned long flags;

	BUG_ON((int)start < 0);
	BUG_ON((int)end < 0);

	if (end == 0)
		max = 0x80000000;
	else {
		BUG_ON(end < start);
		max = end - 1;
	}

again:
	if (!ida_pre_get(ida, gfp_mask))
		return -ENOMEM;

	spin_lock_irqsave(&simple_ida_lock, flags);
	ret = ida_get_new_above(ida, start, &id);
	if (!ret) {
		if (id > max) {
			ida_remove(ida, id);
			ret = -ENOSPC;
		} else {
			ret = id;
		}
	}
	spin_unlock_irqrestore(&simple_ida_lock, flags);

	if (unlikely(ret == -EAGAIN))
		goto again;

	return ret;
}
EXPORT_SYMBOL(ida_simple_get);

/**
 * ida_simple_remove - remove an allocated id.
 * @ida: the (initialized) ida.
 * @id: the id returned by ida_simple_get.
 */
void ida_simple_remove(struct ida *ida, unsigned int id)
{
	unsigned long flags;

	BUG_ON((int)id < 0);
	spin_lock_irqsave(&simple_ida_lock, flags);
	ida_remove(ida, id);
	spin_unlock_irqrestore(&simple_ida_lock, flags);
}
EXPORT_SYMBOL(ida_simple_remove);

/**
 * ida_init - initialize ida handle
 * @ida:	ida handle
 *
 * This function is use to set up the handle (@ida) that you will pass
 * to the rest of the functions.
 */
void ida_init(struct ida *ida)
{
	memset(ida, 0, sizeof(struct ida));
	idr_init(&ida->idr);

}
EXPORT_SYMBOL(ida_init);
=======
 * idr_get_next_ul() - Find next populated entry.
 * @idr: IDR handle.
 * @nextid: Pointer to an ID.
 *
 * Returns the next populated entry in the tree with an ID greater than
 * or equal to the value pointed to by @nextid.  On exit, @nextid is updated
 * to the ID of the found value.  To use in a loop, the value pointed to by
 * nextid must be incremented by the user.
 */
void *idr_get_next_ul(struct idr *idr, unsigned long *nextid)
{
	struct radix_tree_iter iter;
	void __rcu **slot;
	void *entry = NULL;
	unsigned long base = idr->idr_base;
	unsigned long id = *nextid;

	id = (id < base) ? 0 : id - base;
	radix_tree_for_each_slot(slot, &idr->idr_rt, &iter, id) {
		entry = rcu_dereference_raw(*slot);
		if (!entry)
			continue;
		if (!xa_is_internal(entry))
			break;
		if (slot != &idr->idr_rt.xa_head && !xa_is_retry(entry))
			break;
		slot = radix_tree_iter_retry(&iter);
	}
	if (!slot)
		return NULL;

	*nextid = iter.index + base;
	return entry;
}
EXPORT_SYMBOL(idr_get_next_ul);

/**
 * idr_get_next() - Find next populated entry.
 * @idr: IDR handle.
 * @nextid: Pointer to an ID.
 *
 * Returns the next populated entry in the tree with an ID greater than
 * or equal to the value pointed to by @nextid.  On exit, @nextid is updated
 * to the ID of the found value.  To use in a loop, the value pointed to by
 * nextid must be incremented by the user.
 */
void *idr_get_next(struct idr *idr, int *nextid)
{
	unsigned long id = *nextid;
	void *entry = idr_get_next_ul(idr, &id);

	if (WARN_ON_ONCE(id > INT_MAX))
		return NULL;
	*nextid = id;
	return entry;
}
EXPORT_SYMBOL(idr_get_next);

/**
 * idr_replace() - replace pointer for given ID.
 * @idr: IDR handle.
 * @ptr: New pointer to associate with the ID.
 * @id: ID to change.
 *
 * Replace the pointer registered with an ID and return the old value.
 * This function can be called under the RCU read lock concurrently with
 * idr_alloc() and idr_remove() (as long as the ID being removed is not
 * the one being replaced!).
 *
 * Returns: the old value on success.  %-ENOENT indicates that @id was not
 * found.  %-EINVAL indicates that @ptr was not valid.
 */
void *idr_replace(struct idr *idr, void *ptr, unsigned long id)
{
	struct radix_tree_node *node;
	void __rcu **slot = NULL;
	void *entry;

	id -= idr->idr_base;

	entry = __radix_tree_lookup(&idr->idr_rt, id, &node, &slot);
	if (!slot || radix_tree_tag_get(&idr->idr_rt, id, IDR_FREE))
		return ERR_PTR(-ENOENT);

	__radix_tree_replace(&idr->idr_rt, node, slot, ptr);

	return entry;
}
EXPORT_SYMBOL(idr_replace);

/**
 * DOC: IDA description
 *
 * The IDA is an ID allocator which does not provide the ability to
 * associate an ID with a pointer.  As such, it only needs to store one
 * bit per ID, and so is more space efficient than an IDR.  To use an IDA,
 * define it using DEFINE_IDA() (or embed a &struct ida in a data structure,
 * then initialise it using ida_init()).  To allocate a new ID, call
 * ida_alloc(), ida_alloc_min(), ida_alloc_max() or ida_alloc_range().
 * To free an ID, call ida_free().
 *
 * ida_destroy() can be used to dispose of an IDA without needing to
 * free the individual IDs in it.  You can use ida_is_empty() to find
 * out whether the IDA has any IDs currently allocated.
 *
 * The IDA handles its own locking.  It is safe to call any of the IDA
 * functions without synchronisation in your code.
 *
 * IDs are currently limited to the range [0-INT_MAX].  If this is an awkward
 * limitation, it should be quite straightforward to raise the maximum.
 */

/*
 * Developer's notes:
 *
 * The IDA uses the functionality provided by the XArray to store bitmaps in
 * each entry.  The XA_FREE_MARK is only cleared when all bits in the bitmap
 * have been set.
 *
 * I considered telling the XArray that each slot is an order-10 node
 * and indexing by bit number, but the XArray can't allow a single multi-index
 * entry in the head, which would significantly increase memory consumption
 * for the IDA.  So instead we divide the index by the number of bits in the
 * leaf bitmap before doing a radix tree lookup.
 *
 * As an optimisation, if there are only a few low bits set in any given
 * leaf, instead of allocating a 128-byte bitmap, we store the bits
 * as a value entry.  Value entries never have the XA_FREE_MARK cleared
 * because we can always convert them into a bitmap entry.
 *
 * It would be possible to optimise further; once we've run out of a
 * single 128-byte bitmap, we currently switch to a 576-byte node, put
 * the 128-byte bitmap in the first entry and then start allocating extra
 * 128-byte entries.  We could instead use the 512 bytes of the node's
 * data as a bitmap before moving to that scheme.  I do not believe this
 * is a worthwhile optimisation; Rasmus Villemoes surveyed the current
 * users of the IDA and almost none of them use more than 1024 entries.
 * Those that do use more than the 8192 IDs that the 512 bytes would
 * provide.
 *
 * The IDA always uses a lock to alloc/free.  If we add a 'test_bit'
 * equivalent, it will still need locking.  Going to RCU lookup would require
 * using RCU to free bitmaps, and that's not trivial without embedding an
 * RCU head in the bitmap, which adds a 2-pointer overhead to each 128-byte
 * bitmap, which is excessive.
 */

/**
 * ida_alloc_range() - Allocate an unused ID.
 * @ida: IDA handle.
 * @min: Lowest ID to allocate.
 * @max: Highest ID to allocate.
 * @gfp: Memory allocation flags.
 *
 * Allocate an ID between @min and @max, inclusive.  The allocated ID will
 * not exceed %INT_MAX, even if @max is larger.
 *
 * Context: Any context. It is safe to call this function without
 * locking in your code.
 * Return: The allocated ID, or %-ENOMEM if memory could not be allocated,
 * or %-ENOSPC if there are no free IDs.
 */
int ida_alloc_range(struct ida *ida, unsigned int min, unsigned int max,
			gfp_t gfp)
{
	XA_STATE(xas, &ida->xa, min / IDA_BITMAP_BITS);
	unsigned bit = min % IDA_BITMAP_BITS;
	unsigned long flags;
	struct ida_bitmap *bitmap, *alloc = NULL;

	if ((int)min < 0)
		return -ENOSPC;

	if ((int)max < 0)
		max = INT_MAX;

retry:
	xas_lock_irqsave(&xas, flags);
next:
	bitmap = xas_find_marked(&xas, max / IDA_BITMAP_BITS, XA_FREE_MARK);
	if (xas.xa_index > min / IDA_BITMAP_BITS)
		bit = 0;
	if (xas.xa_index * IDA_BITMAP_BITS + bit > max)
		goto nospc;

	if (xa_is_value(bitmap)) {
		unsigned long tmp = xa_to_value(bitmap);

		if (bit < BITS_PER_XA_VALUE) {
			bit = find_next_zero_bit(&tmp, BITS_PER_XA_VALUE, bit);
			if (xas.xa_index * IDA_BITMAP_BITS + bit > max)
				goto nospc;
			if (bit < BITS_PER_XA_VALUE) {
				tmp |= 1UL << bit;
				xas_store(&xas, xa_mk_value(tmp));
				goto out;
			}
		}
		bitmap = alloc;
		if (!bitmap)
			bitmap = kzalloc(sizeof(*bitmap), GFP_NOWAIT);
		if (!bitmap)
			goto alloc;
		bitmap->bitmap[0] = tmp;
		xas_store(&xas, bitmap);
		if (xas_error(&xas)) {
			bitmap->bitmap[0] = 0;
			goto out;
		}
	}

	if (bitmap) {
		bit = find_next_zero_bit(bitmap->bitmap, IDA_BITMAP_BITS, bit);
		if (xas.xa_index * IDA_BITMAP_BITS + bit > max)
			goto nospc;
		if (bit == IDA_BITMAP_BITS)
			goto next;

		__set_bit(bit, bitmap->bitmap);
		if (bitmap_full(bitmap->bitmap, IDA_BITMAP_BITS))
			xas_clear_mark(&xas, XA_FREE_MARK);
	} else {
		if (bit < BITS_PER_XA_VALUE) {
			bitmap = xa_mk_value(1UL << bit);
		} else {
			bitmap = alloc;
			if (!bitmap)
				bitmap = kzalloc(sizeof(*bitmap), GFP_NOWAIT);
			if (!bitmap)
				goto alloc;
			__set_bit(bit, bitmap->bitmap);
		}
		xas_store(&xas, bitmap);
	}
out:
	xas_unlock_irqrestore(&xas, flags);
	if (xas_nomem(&xas, gfp)) {
		xas.xa_index = min / IDA_BITMAP_BITS;
		bit = min % IDA_BITMAP_BITS;
		goto retry;
	}
	if (bitmap != alloc)
		kfree(alloc);
	if (xas_error(&xas))
		return xas_error(&xas);
	return xas.xa_index * IDA_BITMAP_BITS + bit;
alloc:
	xas_unlock_irqrestore(&xas, flags);
	alloc = kzalloc(sizeof(*bitmap), gfp);
	if (!alloc)
		return -ENOMEM;
	xas_set(&xas, min / IDA_BITMAP_BITS);
	bit = min % IDA_BITMAP_BITS;
	goto retry;
nospc:
	xas_unlock_irqrestore(&xas, flags);
	kfree(alloc);
	return -ENOSPC;
}
EXPORT_SYMBOL(ida_alloc_range);

/**
 * ida_free() - Release an allocated ID.
 * @ida: IDA handle.
 * @id: Previously allocated ID.
 *
 * Context: Any context. It is safe to call this function without
 * locking in your code.
 */
void ida_free(struct ida *ida, unsigned int id)
{
	XA_STATE(xas, &ida->xa, id / IDA_BITMAP_BITS);
	unsigned bit = id % IDA_BITMAP_BITS;
	struct ida_bitmap *bitmap;
	unsigned long flags;

	if ((int)id < 0)
		return;

	xas_lock_irqsave(&xas, flags);
	bitmap = xas_load(&xas);

	if (xa_is_value(bitmap)) {
		unsigned long v = xa_to_value(bitmap);
		if (bit >= BITS_PER_XA_VALUE)
			goto err;
		if (!(v & (1UL << bit)))
			goto err;
		v &= ~(1UL << bit);
		if (!v)
			goto delete;
		xas_store(&xas, xa_mk_value(v));
	} else {
		if (!bitmap || !test_bit(bit, bitmap->bitmap))
			goto err;
		__clear_bit(bit, bitmap->bitmap);
		xas_set_mark(&xas, XA_FREE_MARK);
		if (bitmap_empty(bitmap->bitmap, IDA_BITMAP_BITS)) {
			kfree(bitmap);
delete:
			xas_store(&xas, NULL);
		}
	}
	xas_unlock_irqrestore(&xas, flags);
	return;
 err:
	xas_unlock_irqrestore(&xas, flags);
	WARN(1, "ida_free called for id=%d which is not allocated.\n", id);
}
EXPORT_SYMBOL(ida_free);

/**
 * ida_destroy() - Free all IDs.
 * @ida: IDA handle.
 *
 * Calling this function frees all IDs and releases all resources used
 * by an IDA.  When this call returns, the IDA is empty and can be reused
 * or freed.  If the IDA is already empty, there is no need to call this
 * function.
 *
 * Context: Any context. It is safe to call this function without
 * locking in your code.
 */
void ida_destroy(struct ida *ida)
{
	XA_STATE(xas, &ida->xa, 0);
	struct ida_bitmap *bitmap;
	unsigned long flags;

	xas_lock_irqsave(&xas, flags);
	xas_for_each(&xas, bitmap, ULONG_MAX) {
		if (!xa_is_value(bitmap))
			kfree(bitmap);
		xas_store(&xas, NULL);
	}
	xas_unlock_irqrestore(&xas, flags);
}
EXPORT_SYMBOL(ida_destroy);

#ifndef __KERNEL__
extern void xa_dump_index(unsigned long index, unsigned int shift);
#define IDA_CHUNK_SHIFT		ilog2(IDA_BITMAP_BITS)

static void ida_dump_entry(void *entry, unsigned long index)
{
	unsigned long i;

	if (!entry)
		return;

	if (xa_is_node(entry)) {
		struct xa_node *node = xa_to_node(entry);
		unsigned int shift = node->shift + IDA_CHUNK_SHIFT +
			XA_CHUNK_SHIFT;

		xa_dump_index(index * IDA_BITMAP_BITS, shift);
		xa_dump_node(node);
		for (i = 0; i < XA_CHUNK_SIZE; i++)
			ida_dump_entry(node->slots[i],
					index | (i << node->shift));
	} else if (xa_is_value(entry)) {
		xa_dump_index(index * IDA_BITMAP_BITS, ilog2(BITS_PER_LONG));
		pr_cont("value: data %lx [%px]\n", xa_to_value(entry), entry);
	} else {
		struct ida_bitmap *bitmap = entry;

		xa_dump_index(index * IDA_BITMAP_BITS, IDA_CHUNK_SHIFT);
		pr_cont("bitmap: %p data", bitmap);
		for (i = 0; i < IDA_BITMAP_LONGS; i++)
			pr_cont(" %lx", bitmap->bitmap[i]);
		pr_cont("\n");
	}
}

static void ida_dump(struct ida *ida)
{
	struct xarray *xa = &ida->xa;
	pr_debug("ida: %p node %p free %d\n", ida, xa->xa_head,
				xa->xa_flags >> ROOT_TAG_SHIFT);
	ida_dump_entry(xa->xa_head, 0);
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
