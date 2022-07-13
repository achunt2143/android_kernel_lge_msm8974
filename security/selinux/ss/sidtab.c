<<<<<<< HEAD
/*
 * Implementation of the SID table type.
 *
 * Author : Stephen Smalley, <sds@epoch.ncsc.mil>
 */
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include "flask.h"
#include "security.h"
#include "sidtab.h"

#define SIDTAB_HASH(sid) \
(sid & SIDTAB_HASH_MASK)

int sidtab_init(struct sidtab *s)
{
	int i;

	s->htable = kmalloc(sizeof(*(s->htable)) * SIDTAB_SIZE, GFP_ATOMIC);
	if (!s->htable)
		return -ENOMEM;
	for (i = 0; i < SIDTAB_SIZE; i++)
		s->htable[i] = NULL;
	s->nel = 0;
	s->next_sid = 1;
	s->shutdown = 0;
	spin_lock_init(&s->lock);
	return 0;
}

int sidtab_insert(struct sidtab *s, u32 sid, struct context *context)
{
	int hvalue, rc = 0;
	struct sidtab_node *prev, *cur, *newnode;

	if (!s) {
		rc = -ENOMEM;
		goto out;
	}

	hvalue = SIDTAB_HASH(sid);
	prev = NULL;
	cur = s->htable[hvalue];
	while (cur && sid > cur->sid) {
		prev = cur;
		cur = cur->next;
	}

	if (cur && sid == cur->sid) {
		rc = -EEXIST;
		goto out;
	}

	newnode = kmalloc(sizeof(*newnode), GFP_ATOMIC);
	if (newnode == NULL) {
		rc = -ENOMEM;
		goto out;
	}
	newnode->sid = sid;
	if (context_cpy(&newnode->context, context)) {
		kfree(newnode);
		rc = -ENOMEM;
		goto out;
	}

	if (prev) {
		newnode->next = prev->next;
		wmb();
		prev->next = newnode;
	} else {
		newnode->next = s->htable[hvalue];
		wmb();
		s->htable[hvalue] = newnode;
	}

	s->nel++;
	if (sid >= s->next_sid)
		s->next_sid = sid + 1;
out:
	return rc;
}

static struct context *sidtab_search_core(struct sidtab *s, u32 sid, int force)
{
	int hvalue;
	struct sidtab_node *cur;

	if (!s)
		return NULL;

	hvalue = SIDTAB_HASH(sid);
	cur = s->htable[hvalue];
	while (cur && sid > cur->sid)
		cur = cur->next;

	if (force && cur && sid == cur->sid && cur->context.len)
		return &cur->context;

	if (cur == NULL || sid != cur->sid || cur->context.len) {
		/* Remap invalid SIDs to the unlabeled SID. */
		sid = SECINITSID_UNLABELED;
		hvalue = SIDTAB_HASH(sid);
		cur = s->htable[hvalue];
		while (cur && sid > cur->sid)
			cur = cur->next;
		if (!cur || sid != cur->sid)
			return NULL;
	}

	return &cur->context;
}

struct context *sidtab_search(struct sidtab *s, u32 sid)
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Implementation of the SID table type.
 *
 * Original author: Stephen Smalley, <stephen.smalley.work@gmail.com>
 * Author: Ondrej Mosnacek, <omosnacek@gmail.com>
 *
 * Copyright (C) 2018 Red Hat, Inc.
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/rcupdate.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <asm/barrier.h>
#include "flask.h"
#include "security.h"
#include "sidtab.h"
#include "services.h"

struct sidtab_str_cache {
	struct rcu_head rcu_member;
	struct list_head lru_member;
	struct sidtab_entry *parent;
	u32 len;
	char str[] __counted_by(len);
};

#define index_to_sid(index) ((index) + SECINITSID_NUM + 1)
#define sid_to_index(sid)   ((sid) - (SECINITSID_NUM + 1))

int sidtab_init(struct sidtab *s)
{
	u32 i;

	memset(s->roots, 0, sizeof(s->roots));

	for (i = 0; i < SECINITSID_NUM; i++)
		s->isids[i].set = 0;

	s->frozen = false;
	s->count = 0;
	s->convert = NULL;
	hash_init(s->context_to_sid);

	spin_lock_init(&s->lock);

#if CONFIG_SECURITY_SELINUX_SID2STR_CACHE_SIZE > 0
	s->cache_free_slots = CONFIG_SECURITY_SELINUX_SID2STR_CACHE_SIZE;
	INIT_LIST_HEAD(&s->cache_lru_list);
	spin_lock_init(&s->cache_lock);
#endif

	return 0;
}

static u32 context_to_sid(struct sidtab *s, struct context *context, u32 hash)
{
	struct sidtab_entry *entry;
	u32 sid = 0;

	rcu_read_lock();
	hash_for_each_possible_rcu(s->context_to_sid, entry, list, hash) {
		if (entry->hash != hash)
			continue;
		if (context_cmp(&entry->context, context)) {
			sid = entry->sid;
			break;
		}
	}
	rcu_read_unlock();
	return sid;
}

int sidtab_set_initial(struct sidtab *s, u32 sid, struct context *context)
{
	struct sidtab_isid_entry *isid;
	u32 hash;
	int rc;

	if (sid == 0 || sid > SECINITSID_NUM)
		return -EINVAL;

	isid = &s->isids[sid - 1];

	rc = context_cpy(&isid->entry.context, context);
	if (rc)
		return rc;

#if CONFIG_SECURITY_SELINUX_SID2STR_CACHE_SIZE > 0
	isid->entry.cache = NULL;
#endif
	isid->set = 1;

	hash = context_compute_hash(context);

	/*
	 * Multiple initial sids may map to the same context. Check that this
	 * context is not already represented in the context_to_sid hashtable
	 * to avoid duplicate entries and long linked lists upon hash
	 * collision.
	 */
	if (!context_to_sid(s, context, hash)) {
		isid->entry.sid = sid;
		isid->entry.hash = hash;
		hash_add(s->context_to_sid, &isid->entry.list, hash);
	}

	return 0;
}

int sidtab_hash_stats(struct sidtab *sidtab, char *page)
{
	int i;
	int chain_len = 0;
	int slots_used = 0;
	int entries = 0;
	int max_chain_len = 0;
	int cur_bucket = 0;
	struct sidtab_entry *entry;

	rcu_read_lock();
	hash_for_each_rcu(sidtab->context_to_sid, i, entry, list) {
		entries++;
		if (i == cur_bucket) {
			chain_len++;
			if (chain_len == 1)
				slots_used++;
		} else {
			cur_bucket = i;
			if (chain_len > max_chain_len)
				max_chain_len = chain_len;
			chain_len = 0;
		}
	}
	rcu_read_unlock();

	if (chain_len > max_chain_len)
		max_chain_len = chain_len;

	return scnprintf(page, PAGE_SIZE,
			 "entries: %d\nbuckets used: %d/%d\n"
			 "longest chain: %d\n",
			 entries, slots_used, SIDTAB_HASH_BUCKETS,
			 max_chain_len);
}

static u32 sidtab_level_from_count(u32 count)
{
	u32 capacity = SIDTAB_LEAF_ENTRIES;
	u32 level = 0;

	while (count > capacity) {
		capacity <<= SIDTAB_INNER_SHIFT;
		++level;
	}
	return level;
}

static int sidtab_alloc_roots(struct sidtab *s, u32 level)
{
	u32 l;

	if (!s->roots[0].ptr_leaf) {
		s->roots[0].ptr_leaf =
			kzalloc(SIDTAB_NODE_ALLOC_SIZE, GFP_ATOMIC);
		if (!s->roots[0].ptr_leaf)
			return -ENOMEM;
	}
	for (l = 1; l <= level; ++l)
		if (!s->roots[l].ptr_inner) {
			s->roots[l].ptr_inner =
				kzalloc(SIDTAB_NODE_ALLOC_SIZE, GFP_ATOMIC);
			if (!s->roots[l].ptr_inner)
				return -ENOMEM;
			s->roots[l].ptr_inner->entries[0] = s->roots[l - 1];
		}
	return 0;
}

static struct sidtab_entry *sidtab_do_lookup(struct sidtab *s, u32 index,
					     int alloc)
{
	union sidtab_entry_inner *entry;
	u32 level, capacity_shift, leaf_index = index / SIDTAB_LEAF_ENTRIES;

	/* find the level of the subtree we need */
	level = sidtab_level_from_count(index + 1);
	capacity_shift = level * SIDTAB_INNER_SHIFT;

	/* allocate roots if needed */
	if (alloc && sidtab_alloc_roots(s, level) != 0)
		return NULL;

	/* lookup inside the subtree */
	entry = &s->roots[level];
	while (level != 0) {
		capacity_shift -= SIDTAB_INNER_SHIFT;
		--level;

		entry = &entry->ptr_inner->entries[leaf_index >> capacity_shift];
		leaf_index &= ((u32)1 << capacity_shift) - 1;

		if (!entry->ptr_inner) {
			if (alloc)
				entry->ptr_inner = kzalloc(
					SIDTAB_NODE_ALLOC_SIZE, GFP_ATOMIC);
			if (!entry->ptr_inner)
				return NULL;
		}
	}
	if (!entry->ptr_leaf) {
		if (alloc)
			entry->ptr_leaf =
				kzalloc(SIDTAB_NODE_ALLOC_SIZE, GFP_ATOMIC);
		if (!entry->ptr_leaf)
			return NULL;
	}
	return &entry->ptr_leaf->entries[index % SIDTAB_LEAF_ENTRIES];
}

static struct sidtab_entry *sidtab_lookup(struct sidtab *s, u32 index)
{
	/* read entries only after reading count */
	u32 count = smp_load_acquire(&s->count);

	if (index >= count)
		return NULL;

	return sidtab_do_lookup(s, index, 0);
}

static struct sidtab_entry *sidtab_lookup_initial(struct sidtab *s, u32 sid)
{
	return s->isids[sid - 1].set ? &s->isids[sid - 1].entry : NULL;
}

static struct sidtab_entry *sidtab_search_core(struct sidtab *s, u32 sid,
					       int force)
{
	if (sid != 0) {
		struct sidtab_entry *entry;

		if (sid > SECINITSID_NUM)
			entry = sidtab_lookup(s, sid_to_index(sid));
		else
			entry = sidtab_lookup_initial(s, sid);
		if (entry && (!entry->context.len || force))
			return entry;
	}

	return sidtab_lookup_initial(s, SECINITSID_UNLABELED);
}

struct sidtab_entry *sidtab_search_entry(struct sidtab *s, u32 sid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return sidtab_search_core(s, sid, 0);
}

<<<<<<< HEAD
struct context *sidtab_search_force(struct sidtab *s, u32 sid)
=======
struct sidtab_entry *sidtab_search_entry_force(struct sidtab *s, u32 sid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return sidtab_search_core(s, sid, 1);
}

<<<<<<< HEAD
int sidtab_map(struct sidtab *s,
	       int (*apply) (u32 sid,
			     struct context *context,
			     void *args),
	       void *args)
{
	int i, rc = 0;
	struct sidtab_node *cur;

	if (!s)
		goto out;

	for (i = 0; i < SIDTAB_SIZE; i++) {
		cur = s->htable[i];
		while (cur) {
			rc = apply(cur->sid, &cur->context, args);
			if (rc)
				goto out;
			cur = cur->next;
		}
	}
out:
	return rc;
}

static void sidtab_update_cache(struct sidtab *s, struct sidtab_node *n, int loc)
{
	BUG_ON(loc >= SIDTAB_CACHE_LEN);

	while (loc > 0) {
		s->cache[loc] = s->cache[loc - 1];
		loc--;
	}
	s->cache[0] = n;
}

static inline u32 sidtab_search_context(struct sidtab *s,
						  struct context *context)
{
	int i;
	struct sidtab_node *cur;

	for (i = 0; i < SIDTAB_SIZE; i++) {
		cur = s->htable[i];
		while (cur) {
			if (context_cmp(&cur->context, context)) {
				sidtab_update_cache(s, cur, SIDTAB_CACHE_LEN - 1);
				return cur->sid;
			}
			cur = cur->next;
		}
=======
int sidtab_context_to_sid(struct sidtab *s, struct context *context, u32 *sid)
{
	unsigned long flags;
	u32 count, hash = context_compute_hash(context);
	struct sidtab_convert_params *convert;
	struct sidtab_entry *dst, *dst_convert;
	int rc;

	*sid = context_to_sid(s, context, hash);
	if (*sid)
		return 0;

	/* lock-free search failed: lock, re-search, and insert if not found */
	spin_lock_irqsave(&s->lock, flags);

	rc = 0;
	*sid = context_to_sid(s, context, hash);
	if (*sid)
		goto out_unlock;

	if (unlikely(s->frozen)) {
		/*
		 * This sidtab is now frozen - tell the caller to abort and
		 * get the new one.
		 */
		rc = -ESTALE;
		goto out_unlock;
	}

	count = s->count;

	/* bail out if we already reached max entries */
	rc = -EOVERFLOW;
	if (count >= SIDTAB_MAX)
		goto out_unlock;

	/* insert context into new entry */
	rc = -ENOMEM;
	dst = sidtab_do_lookup(s, count, 1);
	if (!dst)
		goto out_unlock;

	dst->sid = index_to_sid(count);
	dst->hash = hash;

	rc = context_cpy(&dst->context, context);
	if (rc)
		goto out_unlock;

	/*
	 * if we are building a new sidtab, we need to convert the context
	 * and insert it there as well
	 */
	convert = s->convert;
	if (convert) {
		struct sidtab *target = convert->target;

		rc = -ENOMEM;
		dst_convert = sidtab_do_lookup(target, count, 1);
		if (!dst_convert) {
			context_destroy(&dst->context);
			goto out_unlock;
		}

		rc = services_convert_context(convert->args, context,
					      &dst_convert->context,
					      GFP_ATOMIC);
		if (rc) {
			context_destroy(&dst->context);
			goto out_unlock;
		}
		dst_convert->sid = index_to_sid(count);
		dst_convert->hash = context_compute_hash(&dst_convert->context);
		target->count = count + 1;

		hash_add_rcu(target->context_to_sid, &dst_convert->list,
			     dst_convert->hash);
	}

	if (context->len)
		pr_info("SELinux:  Context %s is not valid (left unmapped).\n",
			context->str);

	*sid = index_to_sid(count);

	/* write entries before updating count */
	smp_store_release(&s->count, count + 1);
	hash_add_rcu(s->context_to_sid, &dst->list, dst->hash);

	rc = 0;
out_unlock:
	spin_unlock_irqrestore(&s->lock, flags);
	return rc;
}

static void sidtab_convert_hashtable(struct sidtab *s, u32 count)
{
	struct sidtab_entry *entry;
	u32 i;

	for (i = 0; i < count; i++) {
		entry = sidtab_do_lookup(s, i, 0);
		entry->sid = index_to_sid(i);
		entry->hash = context_compute_hash(&entry->context);

		hash_add_rcu(s->context_to_sid, &entry->list, entry->hash);
	}
}

static int sidtab_convert_tree(union sidtab_entry_inner *edst,
			       union sidtab_entry_inner *esrc, u32 *pos,
			       u32 count, u32 level,
			       struct sidtab_convert_params *convert)
{
	int rc;
	u32 i;

	if (level != 0) {
		if (!edst->ptr_inner) {
			edst->ptr_inner =
				kzalloc(SIDTAB_NODE_ALLOC_SIZE, GFP_KERNEL);
			if (!edst->ptr_inner)
				return -ENOMEM;
		}
		i = 0;
		while (i < SIDTAB_INNER_ENTRIES && *pos < count) {
			rc = sidtab_convert_tree(&edst->ptr_inner->entries[i],
						 &esrc->ptr_inner->entries[i],
						 pos, count, level - 1,
						 convert);
			if (rc)
				return rc;
			i++;
		}
	} else {
		if (!edst->ptr_leaf) {
			edst->ptr_leaf =
				kzalloc(SIDTAB_NODE_ALLOC_SIZE, GFP_KERNEL);
			if (!edst->ptr_leaf)
				return -ENOMEM;
		}
		i = 0;
		while (i < SIDTAB_LEAF_ENTRIES && *pos < count) {
			rc = services_convert_context(
				convert->args,
				&esrc->ptr_leaf->entries[i].context,
				&edst->ptr_leaf->entries[i].context,
				GFP_KERNEL);
			if (rc)
				return rc;
			(*pos)++;
			i++;
		}
		cond_resched();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
static inline u32 sidtab_search_cache(struct sidtab *s, struct context *context)
{
	int i;
	struct sidtab_node *node;

	for (i = 0; i < SIDTAB_CACHE_LEN; i++) {
		node = s->cache[i];
		if (unlikely(!node))
			return 0;
		if (context_cmp(&node->context, context)) {
			sidtab_update_cache(s, node, i);
			return node->sid;
		}
	}
	return 0;
}

int sidtab_context_to_sid(struct sidtab *s,
			  struct context *context,
			  u32 *out_sid)
{
	u32 sid;
	int ret = 0;
	unsigned long flags;

	*out_sid = SECSID_NULL;

	sid  = sidtab_search_cache(s, context);
	if (!sid)
		sid = sidtab_search_context(s, context);
	if (!sid) {
		spin_lock_irqsave(&s->lock, flags);
		/* Rescan now that we hold the lock. */
		sid = sidtab_search_context(s, context);
		if (sid)
			goto unlock_out;
		/* No SID exists for the context.  Allocate a new one. */
		if (s->next_sid == UINT_MAX || s->shutdown) {
			ret = -ENOMEM;
			goto unlock_out;
		}
		sid = s->next_sid++;
		if (context->len)
			printk(KERN_INFO
		       "SELinux:  Context %s is not valid (left unmapped).\n",
			       context->str);
		ret = sidtab_insert(s, sid, context);
		if (ret)
			s->next_sid--;
unlock_out:
		spin_unlock_irqrestore(&s->lock, flags);
	}

	if (ret)
		return ret;

	*out_sid = sid;
	return 0;
}

void sidtab_hash_eval(struct sidtab *h, char *tag)
{
	int i, chain_len, slots_used, max_chain_len;
	struct sidtab_node *cur;

	slots_used = 0;
	max_chain_len = 0;
	for (i = 0; i < SIDTAB_SIZE; i++) {
		cur = h->htable[i];
		if (cur) {
			slots_used++;
			chain_len = 0;
			while (cur) {
				chain_len++;
				cur = cur->next;
			}

			if (chain_len > max_chain_len)
				max_chain_len = chain_len;
		}
	}

	printk(KERN_DEBUG "%s:  %d entries and %d/%d buckets used, longest "
	       "chain length %d\n", tag, h->nel, slots_used, SIDTAB_SIZE,
	       max_chain_len);
=======
int sidtab_convert(struct sidtab *s, struct sidtab_convert_params *params)
{
	unsigned long flags;
	u32 count, level, pos;
	int rc;

	spin_lock_irqsave(&s->lock, flags);

	/* concurrent policy loads are not allowed */
	if (s->convert) {
		spin_unlock_irqrestore(&s->lock, flags);
		return -EBUSY;
	}

	count = s->count;
	level = sidtab_level_from_count(count);

	/* allocate last leaf in the new sidtab (to avoid race with
	 * live convert)
	 */
	rc = sidtab_do_lookup(params->target, count - 1, 1) ? 0 : -ENOMEM;
	if (rc) {
		spin_unlock_irqrestore(&s->lock, flags);
		return rc;
	}

	/* set count in case no new entries are added during conversion */
	params->target->count = count;

	/* enable live convert of new entries */
	s->convert = params;

	/* we can safely convert the tree outside the lock */
	spin_unlock_irqrestore(&s->lock, flags);

	pr_info("SELinux:  Converting %u SID table entries...\n", count);

	/* convert all entries not covered by live convert */
	pos = 0;
	rc = sidtab_convert_tree(&params->target->roots[level],
				 &s->roots[level], &pos, count, level, params);
	if (rc) {
		/* we need to keep the old table - disable live convert */
		spin_lock_irqsave(&s->lock, flags);
		s->convert = NULL;
		spin_unlock_irqrestore(&s->lock, flags);
		return rc;
	}
	/*
	 * The hashtable can also be modified in sidtab_context_to_sid()
	 * so we must re-acquire the lock here.
	 */
	spin_lock_irqsave(&s->lock, flags);
	sidtab_convert_hashtable(params->target, count);
	spin_unlock_irqrestore(&s->lock, flags);

	return 0;
}

void sidtab_cancel_convert(struct sidtab *s)
{
	unsigned long flags;

	/* cancelling policy load - disable live convert of sidtab */
	spin_lock_irqsave(&s->lock, flags);
	s->convert = NULL;
	spin_unlock_irqrestore(&s->lock, flags);
}

void sidtab_freeze_begin(struct sidtab *s, unsigned long *flags)
	__acquires(&s->lock)
{
	spin_lock_irqsave(&s->lock, *flags);
	s->frozen = true;
	s->convert = NULL;
}
void sidtab_freeze_end(struct sidtab *s, unsigned long *flags)
	__releases(&s->lock)
{
	spin_unlock_irqrestore(&s->lock, *flags);
}

static void sidtab_destroy_entry(struct sidtab_entry *entry)
{
	context_destroy(&entry->context);
#if CONFIG_SECURITY_SELINUX_SID2STR_CACHE_SIZE > 0
	kfree(rcu_dereference_raw(entry->cache));
#endif
}

static void sidtab_destroy_tree(union sidtab_entry_inner entry, u32 level)
{
	u32 i;

	if (level != 0) {
		struct sidtab_node_inner *node = entry.ptr_inner;

		if (!node)
			return;

		for (i = 0; i < SIDTAB_INNER_ENTRIES; i++)
			sidtab_destroy_tree(node->entries[i], level - 1);
		kfree(node);
	} else {
		struct sidtab_node_leaf *node = entry.ptr_leaf;

		if (!node)
			return;

		for (i = 0; i < SIDTAB_LEAF_ENTRIES; i++)
			sidtab_destroy_entry(&node->entries[i]);
		kfree(node);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void sidtab_destroy(struct sidtab *s)
{
<<<<<<< HEAD
	int i;
	struct sidtab_node *cur, *temp;

	if (!s)
		return;

	for (i = 0; i < SIDTAB_SIZE; i++) {
		cur = s->htable[i];
		while (cur) {
			temp = cur;
			cur = cur->next;
			context_destroy(&temp->context);
			kfree(temp);
		}
		s->htable[i] = NULL;
	}
	kfree(s->htable);
	s->htable = NULL;
	s->nel = 0;
	s->next_sid = 1;
}

void sidtab_set(struct sidtab *dst, struct sidtab *src)
{
	unsigned long flags;
	int i;

	spin_lock_irqsave(&src->lock, flags);
	dst->htable = src->htable;
	dst->nel = src->nel;
	dst->next_sid = src->next_sid;
	dst->shutdown = 0;
	for (i = 0; i < SIDTAB_CACHE_LEN; i++)
		dst->cache[i] = NULL;
	spin_unlock_irqrestore(&src->lock, flags);
}

void sidtab_shutdown(struct sidtab *s)
{
	unsigned long flags;

	spin_lock_irqsave(&s->lock, flags);
	s->shutdown = 1;
	spin_unlock_irqrestore(&s->lock, flags);
}
=======
	u32 i, level;

	for (i = 0; i < SECINITSID_NUM; i++)
		if (s->isids[i].set)
			sidtab_destroy_entry(&s->isids[i].entry);

	level = SIDTAB_MAX_LEVEL;
	while (level && !s->roots[level].ptr_inner)
		--level;

	sidtab_destroy_tree(s->roots[level], level);
	/*
	 * The context_to_sid hashtable's objects are all shared
	 * with the isids array and context tree, and so don't need
	 * to be cleaned up here.
	 */
}

#if CONFIG_SECURITY_SELINUX_SID2STR_CACHE_SIZE > 0

void sidtab_sid2str_put(struct sidtab *s, struct sidtab_entry *entry,
			const char *str, u32 str_len)
{
	struct sidtab_str_cache *cache, *victim = NULL;
	unsigned long flags;

	/* do not cache invalid contexts */
	if (entry->context.len)
		return;

	spin_lock_irqsave(&s->cache_lock, flags);

	cache = rcu_dereference_protected(entry->cache,
					  lockdep_is_held(&s->cache_lock));
	if (cache) {
		/* entry in cache - just bump to the head of LRU list */
		list_move(&cache->lru_member, &s->cache_lru_list);
		goto out_unlock;
	}

	cache = kmalloc(struct_size(cache, str, str_len), GFP_ATOMIC);
	if (!cache)
		goto out_unlock;

	if (s->cache_free_slots == 0) {
		/* pop a cache entry from the tail and free it */
		victim = container_of(s->cache_lru_list.prev,
				      struct sidtab_str_cache, lru_member);
		list_del(&victim->lru_member);
		rcu_assign_pointer(victim->parent->cache, NULL);
	} else {
		s->cache_free_slots--;
	}
	cache->parent = entry;
	cache->len = str_len;
	memcpy(cache->str, str, str_len);
	list_add(&cache->lru_member, &s->cache_lru_list);

	rcu_assign_pointer(entry->cache, cache);

out_unlock:
	spin_unlock_irqrestore(&s->cache_lock, flags);
	kfree_rcu(victim, rcu_member);
}

int sidtab_sid2str_get(struct sidtab *s, struct sidtab_entry *entry, char **out,
		       u32 *out_len)
{
	struct sidtab_str_cache *cache;
	int rc = 0;

	if (entry->context.len)
		return -ENOENT; /* do not cache invalid contexts */

	rcu_read_lock();

	cache = rcu_dereference(entry->cache);
	if (!cache) {
		rc = -ENOENT;
	} else {
		*out_len = cache->len;
		if (out) {
			*out = kmemdup(cache->str, cache->len, GFP_ATOMIC);
			if (!*out)
				rc = -ENOMEM;
		}
	}

	rcu_read_unlock();

	if (!rc && out)
		sidtab_sid2str_put(s, entry, *out, *out_len);
	return rc;
}

#endif /* CONFIG_SECURITY_SELINUX_SID2STR_CACHE_SIZE > 0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
