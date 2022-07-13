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
 */

#include "main.h"
#include "hash.h"

/* clears the hash */
static void hash_init(struct hashtable_t *hash)
{
	uint32_t i;

	for (i = 0 ; i < hash->size; i++) {
		INIT_HLIST_HEAD(&hash->table[i]);
		spin_lock_init(&hash->list_locks[i]);
	}
}

/* free only the hashtable and the hash itself. */
void hash_destroy(struct hashtable_t *hash)
=======
// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Simon Wunderlich, Marek Lindner
 */

#include "hash.h"
#include "main.h"

#include <linux/gfp.h>
#include <linux/lockdep.h>
#include <linux/slab.h>

/* clears the hash */
static void batadv_hash_init(struct batadv_hashtable *hash)
{
	u32 i;

	for (i = 0; i < hash->size; i++) {
		INIT_HLIST_HEAD(&hash->table[i]);
		spin_lock_init(&hash->list_locks[i]);
	}

	atomic_set(&hash->generation, 0);
}

/**
 * batadv_hash_destroy() - Free only the hashtable and the hash itself
 * @hash: hash object to destroy
 */
void batadv_hash_destroy(struct batadv_hashtable *hash)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	kfree(hash->list_locks);
	kfree(hash->table);
	kfree(hash);
}

<<<<<<< HEAD
/* allocates and clears the hash */
struct hashtable_t *hash_new(uint32_t size)
{
	struct hashtable_t *hash;
=======
/**
 * batadv_hash_new() - Allocates and clears the hashtable
 * @size: number of hash buckets to allocate
 *
 * Return: newly allocated hashtable, NULL on errors
 */
struct batadv_hashtable *batadv_hash_new(u32 size)
{
	struct batadv_hashtable *hash;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hash = kmalloc(sizeof(*hash), GFP_ATOMIC);
	if (!hash)
		return NULL;

<<<<<<< HEAD
	hash->table = kmalloc(sizeof(*hash->table) * size, GFP_ATOMIC);
	if (!hash->table)
		goto free_hash;

	hash->list_locks = kmalloc(sizeof(*hash->list_locks) * size,
				   GFP_ATOMIC);
=======
	hash->table = kmalloc_array(size, sizeof(*hash->table), GFP_ATOMIC);
	if (!hash->table)
		goto free_hash;

	hash->list_locks = kmalloc_array(size, sizeof(*hash->list_locks),
					 GFP_ATOMIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!hash->list_locks)
		goto free_table;

	hash->size = size;
<<<<<<< HEAD
	hash_init(hash);
=======
	batadv_hash_init(hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return hash;

free_table:
	kfree(hash->table);
free_hash:
	kfree(hash);
	return NULL;
}
<<<<<<< HEAD
=======

/**
 * batadv_hash_set_lock_class() - Set specific lockdep class for hash spinlocks
 * @hash: hash object to modify
 * @key: lockdep class key address
 */
void batadv_hash_set_lock_class(struct batadv_hashtable *hash,
				struct lock_class_key *key)
{
	u32 i;

	for (i = 0; i < hash->size; i++)
		lockdep_set_class(&hash->list_locks[i], key);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
