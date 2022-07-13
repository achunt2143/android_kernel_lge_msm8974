<<<<<<< HEAD
/*
 * Copyright (C) 2006-2012 B.A.T.M.A.N. contributors:
 *
 * Simon Wunderlich, Marek Lindner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Simon Wunderlich, Marek Lindner
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _NET_BATMAN_ADV_HASH_H_
#define _NET_BATMAN_ADV_HASH_H_

<<<<<<< HEAD
#include <linux/list.h>

/* callback to a compare function.  should
 * compare 2 element datas for their keys,
 * return 0 if same and not 0 if not
 * same */
typedef int (*hashdata_compare_cb)(const struct hlist_node *, const void *);

/* the hashfunction, should return an index
 * based on the key in the data of the first
 * argument and the size the second */
typedef uint32_t (*hashdata_choose_cb)(const void *, uint32_t);
typedef void (*hashdata_free_cb)(struct hlist_node *, void *);

struct hashtable_t {
	struct hlist_head *table;   /* the hashtable itself with the buckets */
	spinlock_t *list_locks;     /* spinlock for each hash list entry */
	uint32_t size;		    /* size of hashtable */
};

/* allocates and clears the hash */
struct hashtable_t *hash_new(uint32_t size);

/* free only the hashtable and the hash itself. */
void hash_destroy(struct hashtable_t *hash);

/* remove the hash structure. if hashdata_free_cb != NULL, this function will be
 * called to remove the elements inside of the hash.  if you don't remove the
 * elements, memory might be leaked. */
static inline void hash_delete(struct hashtable_t *hash,
			       hashdata_free_cb free_cb, void *arg)
{
	struct hlist_head *head;
	struct hlist_node *node, *node_tmp;
	spinlock_t *list_lock; /* spinlock to protect write access */
	uint32_t i;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
		hlist_for_each_safe(node, node_tmp, head) {
			hlist_del_rcu(node);

			if (free_cb)
				free_cb(node, arg);
		}
		spin_unlock_bh(list_lock);
	}

	hash_destroy(hash);
}

/**
 *	hash_add - adds data to the hashtable
=======
#include "main.h"

#include <linux/atomic.h>
#include <linux/compiler.h>
#include <linux/list.h>
#include <linux/lockdep.h>
#include <linux/rculist.h>
#include <linux/spinlock.h>
#include <linux/stddef.h>
#include <linux/types.h>

/* callback to a compare function.  should compare 2 element data for their
 * keys
 *
 * Return: true if same and false if not same
 */
typedef bool (*batadv_hashdata_compare_cb)(const struct hlist_node *,
					   const void *);

/* the hashfunction
 *
 * Return: an index based on the key in the data of the first argument and the
 * size the second
 */
typedef u32 (*batadv_hashdata_choose_cb)(const void *, u32);
typedef void (*batadv_hashdata_free_cb)(struct hlist_node *, void *);

/**
 * struct batadv_hashtable - Wrapper of simple hlist based hashtable
 */
struct batadv_hashtable {
	/** @table: the hashtable itself with the buckets */
	struct hlist_head *table;

	/** @list_locks: spinlock for each hash list entry */
	spinlock_t *list_locks;

	/** @size: size of hashtable */
	u32 size;

	/** @generation: current (generation) sequence number */
	atomic_t generation;
};

/* allocates and clears the hash */
struct batadv_hashtable *batadv_hash_new(u32 size);

/* set class key for all locks */
void batadv_hash_set_lock_class(struct batadv_hashtable *hash,
				struct lock_class_key *key);

/* free only the hashtable and the hash itself. */
void batadv_hash_destroy(struct batadv_hashtable *hash);

/**
 *	batadv_hash_add() - adds data to the hashtable
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	@hash: storage hash table
 *	@compare: callback to determine if 2 hash elements are identical
 *	@choose: callback calculating the hash index
 *	@data: data passed to the aforementioned callbacks as argument
 *	@data_node: to be added element
 *
<<<<<<< HEAD
 *	Returns 0 on success, 1 if the element already is in the hash
 *	and -1 on error.
 */

static inline int hash_add(struct hashtable_t *hash,
			   hashdata_compare_cb compare,
			   hashdata_choose_cb choose,
			   const void *data, struct hlist_node *data_node)
{
	uint32_t index;
=======
 *	Return: 0 on success, 1 if the element already is in the hash
 *	and -1 on error.
 */
static inline int batadv_hash_add(struct batadv_hashtable *hash,
				  batadv_hashdata_compare_cb compare,
				  batadv_hashdata_choose_cb choose,
				  const void *data,
				  struct hlist_node *data_node)
{
	u32 index;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret = -1;
	struct hlist_head *head;
	struct hlist_node *node;
	spinlock_t *list_lock; /* spinlock to protect write access */

	if (!hash)
		goto out;

	index = choose(data, hash->size);
	head = &hash->table[index];
	list_lock = &hash->list_locks[index];

<<<<<<< HEAD
	rcu_read_lock();
	__hlist_for_each_rcu(node, head) {
=======
	spin_lock_bh(list_lock);

	hlist_for_each(node, head) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!compare(node, data))
			continue;

		ret = 1;
<<<<<<< HEAD
		goto err_unlock;
	}
	rcu_read_unlock();

	/* no duplicate found in list, add new element */
	spin_lock_bh(list_lock);
	hlist_add_head_rcu(data_node, head);
	spin_unlock_bh(list_lock);

	ret = 0;
	goto out;

err_unlock:
	rcu_read_unlock();
=======
		goto unlock;
	}

	/* no duplicate found in list, add new element */
	hlist_add_head_rcu(data_node, head);
	atomic_inc(&hash->generation);

	ret = 0;

unlock:
	spin_unlock_bh(list_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return ret;
}

<<<<<<< HEAD
/* removes data from hash, if found. returns pointer do data on success, so you
 * can remove the used structure yourself, or NULL on error .  data could be the
 * structure you use with just the key filled, we just need the key for
 * comparing. */
static inline void *hash_remove(struct hashtable_t *hash,
				hashdata_compare_cb compare,
				hashdata_choose_cb choose, void *data)
{
	uint32_t index;
=======
/**
 * batadv_hash_remove() - Removes data from hash, if found
 * @hash: hash table
 * @compare: callback to determine if 2 hash elements are identical
 * @choose: callback calculating the hash index
 * @data: data passed to the aforementioned callbacks as argument
 *
 * ata could be the structure you use with  just the key filled, we just need
 * the key for comparing.
 *
 * Return: returns pointer do data on success, so you can remove the used
 * structure yourself, or NULL on error
 */
static inline void *batadv_hash_remove(struct batadv_hashtable *hash,
				       batadv_hashdata_compare_cb compare,
				       batadv_hashdata_choose_cb choose,
				       void *data)
{
	u32 index;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct hlist_node *node;
	struct hlist_head *head;
	void *data_save = NULL;

	index = choose(data, hash->size);
	head = &hash->table[index];

	spin_lock_bh(&hash->list_locks[index]);
	hlist_for_each(node, head) {
		if (!compare(node, data))
			continue;

		data_save = node;
		hlist_del_rcu(node);
<<<<<<< HEAD
=======
		atomic_inc(&hash->generation);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	spin_unlock_bh(&hash->list_locks[index]);

	return data_save;
}

#endif /* _NET_BATMAN_ADV_HASH_H_ */
