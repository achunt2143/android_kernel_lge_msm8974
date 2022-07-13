<<<<<<< HEAD
/* Keyring handling
 *
 * Copyright (C) 2004-2005, 2008 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* Keyring handling
 *
 * Copyright (C) 2004-2005, 2008, 2013 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 */

#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/security.h>
#include <linux/seq_file.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <keys/keyring-type.h>
#include <linux/uaccess.h>
#include "internal.h"

#define rcu_dereference_locked_keyring(keyring)				\
	(rcu_dereference_protected(					\
		(keyring)->payload.subscriptions,			\
		rwsem_is_locked((struct rw_semaphore *)&(keyring)->sem)))

#define rcu_deref_link_locked(klist, index, keyring)			\
	(rcu_dereference_protected(					\
		(klist)->keys[index],					\
		rwsem_is_locked((struct rw_semaphore *)&(keyring)->sem)))

#define KEY_LINK_FIXQUOTA 1UL

=======
#include <linux/user_namespace.h>
#include <linux/nsproxy.h>
#include <keys/keyring-type.h>
#include <keys/user-type.h>
#include <linux/assoc_array_priv.h>
#include <linux/uaccess.h>
#include <net/net_namespace.h>
#include "internal.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * When plumbing the depths of the key tree, this sets a hard limit
 * set on how deep we're willing to go.
 */
#define KEYRING_SEARCH_MAX_DEPTH 6

/*
<<<<<<< HEAD
 * We keep all named keyrings in a hash to speed looking them up.
 */
#define KEYRING_NAME_HASH_SIZE	(1 << 5)

static struct list_head	keyring_name_hash[KEYRING_NAME_HASH_SIZE];
static DEFINE_RWLOCK(keyring_name_lock);

static inline unsigned keyring_hash(const char *desc)
{
	unsigned bucket = 0;

	for (; *desc; desc++)
		bucket += (unsigned char)*desc;

	return bucket & (KEYRING_NAME_HASH_SIZE - 1);
=======
 * We mark pointers we pass to the associative array with bit 1 set if
 * they're keyrings and clear otherwise.
 */
#define KEYRING_PTR_SUBTYPE	0x2UL

static inline bool keyring_ptr_is_keyring(const struct assoc_array_ptr *x)
{
	return (unsigned long)x & KEYRING_PTR_SUBTYPE;
}
static inline struct key *keyring_ptr_to_key(const struct assoc_array_ptr *x)
{
	void *object = assoc_array_ptr_to_leaf(x);
	return (struct key *)((unsigned long)object & ~KEYRING_PTR_SUBTYPE);
}
static inline void *keyring_key_to_ptr(struct key *key)
{
	if (key->type == &key_type_keyring)
		return (void *)((unsigned long)key | KEYRING_PTR_SUBTYPE);
	return key;
}

static DEFINE_RWLOCK(keyring_name_lock);

/*
 * Clean up the bits of user_namespace that belong to us.
 */
void key_free_user_ns(struct user_namespace *ns)
{
	write_lock(&keyring_name_lock);
	list_del_init(&ns->keyring_name_list);
	write_unlock(&keyring_name_lock);

	key_put(ns->user_keyring_register);
#ifdef CONFIG_PERSISTENT_KEYRINGS
	key_put(ns->persistent_keyring_register);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * The keyring key type definition.  Keyrings are simply keys of this type and
 * can be treated as ordinary keys in addition to having their own special
 * operations.
 */
<<<<<<< HEAD
static int keyring_instantiate(struct key *keyring,
			       const void *data, size_t datalen);
static int keyring_match(const struct key *keyring, const void *criterion);
=======
static int keyring_preparse(struct key_preparsed_payload *prep);
static void keyring_free_preparse(struct key_preparsed_payload *prep);
static int keyring_instantiate(struct key *keyring,
			       struct key_preparsed_payload *prep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void keyring_revoke(struct key *keyring);
static void keyring_destroy(struct key *keyring);
static void keyring_describe(const struct key *keyring, struct seq_file *m);
static long keyring_read(const struct key *keyring,
<<<<<<< HEAD
			 char __user *buffer, size_t buflen);

struct key_type key_type_keyring = {
	.name		= "keyring",
	.def_datalen	= sizeof(struct keyring_list),
	.instantiate	= keyring_instantiate,
	.match		= keyring_match,
=======
			 char *buffer, size_t buflen);

struct key_type key_type_keyring = {
	.name		= "keyring",
	.def_datalen	= 0,
	.preparse	= keyring_preparse,
	.free_preparse	= keyring_free_preparse,
	.instantiate	= keyring_instantiate,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.revoke		= keyring_revoke,
	.destroy	= keyring_destroy,
	.describe	= keyring_describe,
	.read		= keyring_read,
};
EXPORT_SYMBOL(key_type_keyring);

/*
 * Semaphore to serialise link/link calls to prevent two link calls in parallel
 * introducing a cycle.
 */
<<<<<<< HEAD
static DECLARE_RWSEM(keyring_serialise_link_sem);

/*
 * Publish the name of a keyring so that it can be found by name (if it has
 * one).
 */
static void keyring_publish_name(struct key *keyring)
{
	int bucket;

	if (keyring->description) {
		bucket = keyring_hash(keyring->description);

		write_lock(&keyring_name_lock);

		if (!keyring_name_hash[bucket].next)
			INIT_LIST_HEAD(&keyring_name_hash[bucket]);

		list_add_tail(&keyring->type_data.link,
			      &keyring_name_hash[bucket]);

=======
static DEFINE_MUTEX(keyring_serialise_link_lock);

/*
 * Publish the name of a keyring so that it can be found by name (if it has
 * one and it doesn't begin with a dot).
 */
static void keyring_publish_name(struct key *keyring)
{
	struct user_namespace *ns = current_user_ns();

	if (keyring->description &&
	    keyring->description[0] &&
	    keyring->description[0] != '.') {
		write_lock(&keyring_name_lock);
		list_add_tail(&keyring->name_link, &ns->keyring_name_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		write_unlock(&keyring_name_lock);
	}
}

/*
<<<<<<< HEAD
=======
 * Preparse a keyring payload
 */
static int keyring_preparse(struct key_preparsed_payload *prep)
{
	return prep->datalen != 0 ? -EINVAL : 0;
}

/*
 * Free a preparse of a user defined key payload
 */
static void keyring_free_preparse(struct key_preparsed_payload *prep)
{
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Initialise a keyring.
 *
 * Returns 0 on success, -EINVAL if given any data.
 */
static int keyring_instantiate(struct key *keyring,
<<<<<<< HEAD
			       const void *data, size_t datalen)
{
	int ret;

	ret = -EINVAL;
	if (datalen == 0) {
		/* make the keyring available by name if it has one */
		keyring_publish_name(keyring);
		ret = 0;
	}

	return ret;
}

/*
 * Match keyrings on their name
 */
static int keyring_match(const struct key *keyring, const void *description)
{
	return keyring->description &&
		strcmp(keyring->description, description) == 0;
}

/*
=======
			       struct key_preparsed_payload *prep)
{
	assoc_array_init(&keyring->keys);
	/* make the keyring available by name if it has one */
	keyring_publish_name(keyring);
	return 0;
}

/*
 * Multiply 64-bits by 32-bits to 96-bits and fold back to 64-bit.  Ideally we'd
 * fold the carry back too, but that requires inline asm.
 */
static u64 mult_64x32_and_fold(u64 x, u32 y)
{
	u64 hi = (u64)(u32)(x >> 32) * y;
	u64 lo = (u64)(u32)(x) * y;
	return lo + ((u64)(u32)hi << 32) + (u32)(hi >> 32);
}

/*
 * Hash a key type and description.
 */
static void hash_key_type_and_desc(struct keyring_index_key *index_key)
{
	const unsigned level_shift = ASSOC_ARRAY_LEVEL_STEP;
	const unsigned long fan_mask = ASSOC_ARRAY_FAN_MASK;
	const char *description = index_key->description;
	unsigned long hash, type;
	u32 piece;
	u64 acc;
	int n, desc_len = index_key->desc_len;

	type = (unsigned long)index_key->type;
	acc = mult_64x32_and_fold(type, desc_len + 13);
	acc = mult_64x32_and_fold(acc, 9207);
	piece = (unsigned long)index_key->domain_tag;
	acc = mult_64x32_and_fold(acc, piece);
	acc = mult_64x32_and_fold(acc, 9207);

	for (;;) {
		n = desc_len;
		if (n <= 0)
			break;
		if (n > 4)
			n = 4;
		piece = 0;
		memcpy(&piece, description, n);
		description += n;
		desc_len -= n;
		acc = mult_64x32_and_fold(acc, piece);
		acc = mult_64x32_and_fold(acc, 9207);
	}

	/* Fold the hash down to 32 bits if need be. */
	hash = acc;
	if (ASSOC_ARRAY_KEY_CHUNK_SIZE == 32)
		hash ^= acc >> 32;

	/* Squidge all the keyrings into a separate part of the tree to
	 * ordinary keys by making sure the lowest level segment in the hash is
	 * zero for keyrings and non-zero otherwise.
	 */
	if (index_key->type != &key_type_keyring && (hash & fan_mask) == 0)
		hash |= (hash >> (ASSOC_ARRAY_KEY_CHUNK_SIZE - level_shift)) | 1;
	else if (index_key->type == &key_type_keyring && (hash & fan_mask) != 0)
		hash = (hash + (hash << level_shift)) & ~fan_mask;
	index_key->hash = hash;
}

/*
 * Finalise an index key to include a part of the description actually in the
 * index key, to set the domain tag and to calculate the hash.
 */
void key_set_index_key(struct keyring_index_key *index_key)
{
	static struct key_tag default_domain_tag = { .usage = REFCOUNT_INIT(1), };
	size_t n = min_t(size_t, index_key->desc_len, sizeof(index_key->desc));

	memcpy(index_key->desc, index_key->description, n);

	if (!index_key->domain_tag) {
		if (index_key->type->flags & KEY_TYPE_NET_DOMAIN)
			index_key->domain_tag = current->nsproxy->net_ns->key_domain;
		else
			index_key->domain_tag = &default_domain_tag;
	}

	hash_key_type_and_desc(index_key);
}

/**
 * key_put_tag - Release a ref on a tag.
 * @tag: The tag to release.
 *
 * This releases a reference the given tag and returns true if that ref was the
 * last one.
 */
bool key_put_tag(struct key_tag *tag)
{
	if (refcount_dec_and_test(&tag->usage)) {
		kfree_rcu(tag, rcu);
		return true;
	}

	return false;
}

/**
 * key_remove_domain - Kill off a key domain and gc its keys
 * @domain_tag: The domain tag to release.
 *
 * This marks a domain tag as being dead and releases a ref on it.  If that
 * wasn't the last reference, the garbage collector is poked to try and delete
 * all keys that were in the domain.
 */
void key_remove_domain(struct key_tag *domain_tag)
{
	domain_tag->removed = true;
	if (!key_put_tag(domain_tag))
		key_schedule_gc_links();
}

/*
 * Build the next index key chunk.
 *
 * We return it one word-sized chunk at a time.
 */
static unsigned long keyring_get_key_chunk(const void *data, int level)
{
	const struct keyring_index_key *index_key = data;
	unsigned long chunk = 0;
	const u8 *d;
	int desc_len = index_key->desc_len, n = sizeof(chunk);

	level /= ASSOC_ARRAY_KEY_CHUNK_SIZE;
	switch (level) {
	case 0:
		return index_key->hash;
	case 1:
		return index_key->x;
	case 2:
		return (unsigned long)index_key->type;
	case 3:
		return (unsigned long)index_key->domain_tag;
	default:
		level -= 4;
		if (desc_len <= sizeof(index_key->desc))
			return 0;

		d = index_key->description + sizeof(index_key->desc);
		d += level * sizeof(long);
		desc_len -= sizeof(index_key->desc);
		if (desc_len > n)
			desc_len = n;
		do {
			chunk <<= 8;
			chunk |= *d++;
		} while (--desc_len > 0);
		return chunk;
	}
}

static unsigned long keyring_get_object_key_chunk(const void *object, int level)
{
	const struct key *key = keyring_ptr_to_key(object);
	return keyring_get_key_chunk(&key->index_key, level);
}

static bool keyring_compare_object(const void *object, const void *data)
{
	const struct keyring_index_key *index_key = data;
	const struct key *key = keyring_ptr_to_key(object);

	return key->index_key.type == index_key->type &&
		key->index_key.domain_tag == index_key->domain_tag &&
		key->index_key.desc_len == index_key->desc_len &&
		memcmp(key->index_key.description, index_key->description,
		       index_key->desc_len) == 0;
}

/*
 * Compare the index keys of a pair of objects and determine the bit position
 * at which they differ - if they differ.
 */
static int keyring_diff_objects(const void *object, const void *data)
{
	const struct key *key_a = keyring_ptr_to_key(object);
	const struct keyring_index_key *a = &key_a->index_key;
	const struct keyring_index_key *b = data;
	unsigned long seg_a, seg_b;
	int level, i;

	level = 0;
	seg_a = a->hash;
	seg_b = b->hash;
	if ((seg_a ^ seg_b) != 0)
		goto differ;
	level += ASSOC_ARRAY_KEY_CHUNK_SIZE / 8;

	/* The number of bits contributed by the hash is controlled by a
	 * constant in the assoc_array headers.  Everything else thereafter we
	 * can deal with as being machine word-size dependent.
	 */
	seg_a = a->x;
	seg_b = b->x;
	if ((seg_a ^ seg_b) != 0)
		goto differ;
	level += sizeof(unsigned long);

	/* The next bit may not work on big endian */
	seg_a = (unsigned long)a->type;
	seg_b = (unsigned long)b->type;
	if ((seg_a ^ seg_b) != 0)
		goto differ;
	level += sizeof(unsigned long);

	seg_a = (unsigned long)a->domain_tag;
	seg_b = (unsigned long)b->domain_tag;
	if ((seg_a ^ seg_b) != 0)
		goto differ;
	level += sizeof(unsigned long);

	i = sizeof(a->desc);
	if (a->desc_len <= i)
		goto same;

	for (; i < a->desc_len; i++) {
		seg_a = *(unsigned char *)(a->description + i);
		seg_b = *(unsigned char *)(b->description + i);
		if ((seg_a ^ seg_b) != 0)
			goto differ_plus_i;
	}

same:
	return -1;

differ_plus_i:
	level += i;
differ:
	i = level * 8 + __ffs(seg_a ^ seg_b);
	return i;
}

/*
 * Free an object after stripping the keyring flag off of the pointer.
 */
static void keyring_free_object(void *object)
{
	key_put(keyring_ptr_to_key(object));
}

/*
 * Operations for keyring management by the index-tree routines.
 */
static const struct assoc_array_ops keyring_assoc_array_ops = {
	.get_key_chunk		= keyring_get_key_chunk,
	.get_object_key_chunk	= keyring_get_object_key_chunk,
	.compare_object		= keyring_compare_object,
	.diff_objects		= keyring_diff_objects,
	.free_object		= keyring_free_object,
};

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Clean up a keyring when it is destroyed.  Unpublish its name if it had one
 * and dispose of its data.
 *
 * The garbage collector detects the final key_put(), removes the keyring from
 * the serial number tree and then does RCU synchronisation before coming here,
 * so we shouldn't need to worry about code poking around here with the RCU
 * readlock held by this time.
 */
static void keyring_destroy(struct key *keyring)
{
<<<<<<< HEAD
	struct keyring_list *klist;
	int loop;

	if (keyring->description) {
		write_lock(&keyring_name_lock);

		if (keyring->type_data.link.next != NULL &&
		    !list_empty(&keyring->type_data.link))
			list_del(&keyring->type_data.link);
=======
	if (keyring->description) {
		write_lock(&keyring_name_lock);

		if (keyring->name_link.next != NULL &&
		    !list_empty(&keyring->name_link))
			list_del(&keyring->name_link);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		write_unlock(&keyring_name_lock);
	}

<<<<<<< HEAD
	klist = rcu_access_pointer(keyring->payload.subscriptions);
	if (klist) {
		for (loop = klist->nkeys - 1; loop >= 0; loop--)
			key_put(rcu_access_pointer(klist->keys[loop]));
		kfree(klist);
	}
=======
	if (keyring->restrict_link) {
		struct key_restriction *keyres = keyring->restrict_link;

		key_put(keyres->key);
		kfree(keyres);
	}

	assoc_array_destroy(&keyring->keys, &keyring_assoc_array_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Describe a keyring for /proc.
 */
static void keyring_describe(const struct key *keyring, struct seq_file *m)
{
<<<<<<< HEAD
	struct keyring_list *klist;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (keyring->description)
		seq_puts(m, keyring->description);
	else
		seq_puts(m, "[anon]");

<<<<<<< HEAD
	if (key_is_instantiated(keyring)) {
		rcu_read_lock();
		klist = rcu_dereference(keyring->payload.subscriptions);
		if (klist)
			seq_printf(m, ": %u/%u", klist->nkeys, klist->maxkeys);
		else
			seq_puts(m, ": empty");
		rcu_read_unlock();
	}
}

/*
 * Read a list of key IDs from the keyring's contents in binary form
 *
 * The keyring's semaphore is read-locked by the caller.
 */
static long keyring_read(const struct key *keyring,
			 char __user *buffer, size_t buflen)
{
	struct keyring_list *klist;
	struct key *key;
	size_t qty, tmp;
	int loop, ret;

	ret = 0;
	klist = rcu_dereference_locked_keyring(keyring);
	if (klist) {
		/* calculate how much data we could return */
		qty = klist->nkeys * sizeof(key_serial_t);

		if (buffer && buflen > 0) {
			if (buflen > qty)
				buflen = qty;

			/* copy the IDs of the subscribed keys into the
			 * buffer */
			ret = -EFAULT;

			for (loop = 0; loop < klist->nkeys; loop++) {
				key = rcu_deref_link_locked(klist, loop,
							    keyring);

				tmp = sizeof(key_serial_t);
				if (tmp > buflen)
					tmp = buflen;

				if (copy_to_user(buffer,
						 &key->serial,
						 tmp) != 0)
					goto error;

				buflen -= tmp;
				if (buflen == 0)
					break;
				buffer += tmp;
			}
		}

		ret = qty;
	}

error:
=======
	if (key_is_positive(keyring)) {
		if (keyring->keys.nr_leaves_on_tree != 0)
			seq_printf(m, ": %lu", keyring->keys.nr_leaves_on_tree);
		else
			seq_puts(m, ": empty");
	}
}

struct keyring_read_iterator_context {
	size_t			buflen;
	size_t			count;
	key_serial_t		*buffer;
};

static int keyring_read_iterator(const void *object, void *data)
{
	struct keyring_read_iterator_context *ctx = data;
	const struct key *key = keyring_ptr_to_key(object);

	kenter("{%s,%d},,{%zu/%zu}",
	       key->type->name, key->serial, ctx->count, ctx->buflen);

	if (ctx->count >= ctx->buflen)
		return 1;

	*ctx->buffer++ = key->serial;
	ctx->count += sizeof(key->serial);
	return 0;
}

/*
 * Read a list of key IDs from the keyring's contents in binary form
 *
 * The keyring's semaphore is read-locked by the caller.  This prevents someone
 * from modifying it under us - which could cause us to read key IDs multiple
 * times.
 */
static long keyring_read(const struct key *keyring,
			 char *buffer, size_t buflen)
{
	struct keyring_read_iterator_context ctx;
	long ret;

	kenter("{%d},,%zu", key_serial(keyring), buflen);

	if (buflen & (sizeof(key_serial_t) - 1))
		return -EINVAL;

	/* Copy as many key IDs as fit into the buffer */
	if (buffer && buflen) {
		ctx.buffer = (key_serial_t *)buffer;
		ctx.buflen = buflen;
		ctx.count = 0;
		ret = assoc_array_iterate(&keyring->keys,
					  keyring_read_iterator, &ctx);
		if (ret < 0) {
			kleave(" = %ld [iterate]", ret);
			return ret;
		}
	}

	/* Return the size of the buffer needed */
	ret = keyring->keys.nr_leaves_on_tree * sizeof(key_serial_t);
	if (ret <= buflen)
		kleave("= %ld [ok]", ret);
	else
		kleave("= %ld [buffer too small]", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * Allocate a keyring and link into the destination keyring.
 */
<<<<<<< HEAD
struct key *keyring_alloc(const char *description, uid_t uid, gid_t gid,
			  const struct cred *cred, unsigned long flags,
=======
struct key *keyring_alloc(const char *description, kuid_t uid, kgid_t gid,
			  const struct cred *cred, key_perm_t perm,
			  unsigned long flags,
			  struct key_restriction *restrict_link,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  struct key *dest)
{
	struct key *keyring;
	int ret;

	keyring = key_alloc(&key_type_keyring, description,
<<<<<<< HEAD
			    uid, gid, cred,
			    (KEY_POS_ALL & ~KEY_POS_SETATTR) | KEY_USR_ALL,
			    flags);

=======
			    uid, gid, cred, perm, flags, restrict_link);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!IS_ERR(keyring)) {
		ret = key_instantiate_and_link(keyring, NULL, 0, dest, NULL);
		if (ret < 0) {
			key_put(keyring);
			keyring = ERR_PTR(ret);
		}
	}

	return keyring;
}
<<<<<<< HEAD

/**
 * keyring_search_aux - Search a keyring tree for a key matching some criteria
 * @keyring_ref: A pointer to the keyring with possession indicator.
 * @cred: The credentials to use for permissions checks.
 * @type: The type of key to search for.
 * @description: Parameter for @match.
 * @match: Function to rule on whether or not a key is the one required.
 * @no_state_check: Don't check if a matching key is bad
=======
EXPORT_SYMBOL(keyring_alloc);

/**
 * restrict_link_reject - Give -EPERM to restrict link
 * @keyring: The keyring being added to.
 * @type: The type of key being added.
 * @payload: The payload of the key intended to be added.
 * @restriction_key: Keys providing additional data for evaluating restriction.
 *
 * Reject the addition of any links to a keyring.  It can be overridden by
 * passing KEY_ALLOC_BYPASS_RESTRICTION to key_instantiate_and_link() when
 * adding a key to a keyring.
 *
 * This is meant to be stored in a key_restriction structure which is passed
 * in the restrict_link parameter to keyring_alloc().
 */
int restrict_link_reject(struct key *keyring,
			 const struct key_type *type,
			 const union key_payload *payload,
			 struct key *restriction_key)
{
	return -EPERM;
}

/*
 * By default, we keys found by getting an exact match on their descriptions.
 */
bool key_default_cmp(const struct key *key,
		     const struct key_match_data *match_data)
{
	return strcmp(key->description, match_data->raw_data) == 0;
}

/*
 * Iteration function to consider each key found.
 */
static int keyring_search_iterator(const void *object, void *iterator_data)
{
	struct keyring_search_context *ctx = iterator_data;
	const struct key *key = keyring_ptr_to_key(object);
	unsigned long kflags = READ_ONCE(key->flags);
	short state = READ_ONCE(key->state);

	kenter("{%d}", key->serial);

	/* ignore keys not of this type */
	if (key->type != ctx->index_key.type) {
		kleave(" = 0 [!type]");
		return 0;
	}

	/* skip invalidated, revoked and expired keys */
	if (ctx->flags & KEYRING_SEARCH_DO_STATE_CHECK) {
		time64_t expiry = READ_ONCE(key->expiry);

		if (kflags & ((1 << KEY_FLAG_INVALIDATED) |
			      (1 << KEY_FLAG_REVOKED))) {
			ctx->result = ERR_PTR(-EKEYREVOKED);
			kleave(" = %d [invrev]", ctx->skipped_ret);
			goto skipped;
		}

		if (expiry && ctx->now >= expiry) {
			if (!(ctx->flags & KEYRING_SEARCH_SKIP_EXPIRED))
				ctx->result = ERR_PTR(-EKEYEXPIRED);
			kleave(" = %d [expire]", ctx->skipped_ret);
			goto skipped;
		}
	}

	/* keys that don't match */
	if (!ctx->match_data.cmp(key, &ctx->match_data)) {
		kleave(" = 0 [!match]");
		return 0;
	}

	/* key must have search permissions */
	if (!(ctx->flags & KEYRING_SEARCH_NO_CHECK_PERM) &&
	    key_task_permission(make_key_ref(key, ctx->possessed),
				ctx->cred, KEY_NEED_SEARCH) < 0) {
		ctx->result = ERR_PTR(-EACCES);
		kleave(" = %d [!perm]", ctx->skipped_ret);
		goto skipped;
	}

	if (ctx->flags & KEYRING_SEARCH_DO_STATE_CHECK) {
		/* we set a different error code if we pass a negative key */
		if (state < 0) {
			ctx->result = ERR_PTR(state);
			kleave(" = %d [neg]", ctx->skipped_ret);
			goto skipped;
		}
	}

	/* Found */
	ctx->result = make_key_ref(key, ctx->possessed);
	kleave(" = 1 [found]");
	return 1;

skipped:
	return ctx->skipped_ret;
}

/*
 * Search inside a keyring for a key.  We can search by walking to it
 * directly based on its index-key or we can iterate over the entire
 * tree looking for it, based on the match function.
 */
static int search_keyring(struct key *keyring, struct keyring_search_context *ctx)
{
	if (ctx->match_data.lookup_type == KEYRING_SEARCH_LOOKUP_DIRECT) {
		const void *object;

		object = assoc_array_find(&keyring->keys,
					  &keyring_assoc_array_ops,
					  &ctx->index_key);
		return object ? ctx->iterator(object, ctx) : 0;
	}
	return assoc_array_iterate(&keyring->keys, ctx->iterator, ctx);
}

/*
 * Search a tree of keyrings that point to other keyrings up to the maximum
 * depth.
 */
static bool search_nested_keyrings(struct key *keyring,
				   struct keyring_search_context *ctx)
{
	struct {
		struct key *keyring;
		struct assoc_array_node *node;
		int slot;
	} stack[KEYRING_SEARCH_MAX_DEPTH];

	struct assoc_array_shortcut *shortcut;
	struct assoc_array_node *node;
	struct assoc_array_ptr *ptr;
	struct key *key;
	int sp = 0, slot;

	kenter("{%d},{%s,%s}",
	       keyring->serial,
	       ctx->index_key.type->name,
	       ctx->index_key.description);

#define STATE_CHECKS (KEYRING_SEARCH_NO_STATE_CHECK | KEYRING_SEARCH_DO_STATE_CHECK)
	BUG_ON((ctx->flags & STATE_CHECKS) == 0 ||
	       (ctx->flags & STATE_CHECKS) == STATE_CHECKS);

	if (ctx->index_key.description)
		key_set_index_key(&ctx->index_key);

	/* Check to see if this top-level keyring is what we are looking for
	 * and whether it is valid or not.
	 */
	if (ctx->match_data.lookup_type == KEYRING_SEARCH_LOOKUP_ITERATE ||
	    keyring_compare_object(keyring, &ctx->index_key)) {
		ctx->skipped_ret = 2;
		switch (ctx->iterator(keyring_key_to_ptr(keyring), ctx)) {
		case 1:
			goto found;
		case 2:
			return false;
		default:
			break;
		}
	}

	ctx->skipped_ret = 0;

	/* Start processing a new keyring */
descend_to_keyring:
	kdebug("descend to %d", keyring->serial);
	if (keyring->flags & ((1 << KEY_FLAG_INVALIDATED) |
			      (1 << KEY_FLAG_REVOKED)))
		goto not_this_keyring;

	/* Search through the keys in this keyring before its searching its
	 * subtrees.
	 */
	if (search_keyring(keyring, ctx))
		goto found;

	/* Then manually iterate through the keyrings nested in this one.
	 *
	 * Start from the root node of the index tree.  Because of the way the
	 * hash function has been set up, keyrings cluster on the leftmost
	 * branch of the root node (root slot 0) or in the root node itself.
	 * Non-keyrings avoid the leftmost branch of the root entirely (root
	 * slots 1-15).
	 */
	if (!(ctx->flags & KEYRING_SEARCH_RECURSE))
		goto not_this_keyring;

	ptr = READ_ONCE(keyring->keys.root);
	if (!ptr)
		goto not_this_keyring;

	if (assoc_array_ptr_is_shortcut(ptr)) {
		/* If the root is a shortcut, either the keyring only contains
		 * keyring pointers (everything clusters behind root slot 0) or
		 * doesn't contain any keyring pointers.
		 */
		shortcut = assoc_array_ptr_to_shortcut(ptr);
		if ((shortcut->index_key[0] & ASSOC_ARRAY_FAN_MASK) != 0)
			goto not_this_keyring;

		ptr = READ_ONCE(shortcut->next_node);
		node = assoc_array_ptr_to_node(ptr);
		goto begin_node;
	}

	node = assoc_array_ptr_to_node(ptr);
	ptr = node->slots[0];
	if (!assoc_array_ptr_is_meta(ptr))
		goto begin_node;

descend_to_node:
	/* Descend to a more distal node in this keyring's content tree and go
	 * through that.
	 */
	kdebug("descend");
	if (assoc_array_ptr_is_shortcut(ptr)) {
		shortcut = assoc_array_ptr_to_shortcut(ptr);
		ptr = READ_ONCE(shortcut->next_node);
		BUG_ON(!assoc_array_ptr_is_node(ptr));
	}
	node = assoc_array_ptr_to_node(ptr);

begin_node:
	kdebug("begin_node");
	slot = 0;
ascend_to_node:
	/* Go through the slots in a node */
	for (; slot < ASSOC_ARRAY_FAN_OUT; slot++) {
		ptr = READ_ONCE(node->slots[slot]);

		if (assoc_array_ptr_is_meta(ptr) && node->back_pointer)
			goto descend_to_node;

		if (!keyring_ptr_is_keyring(ptr))
			continue;

		key = keyring_ptr_to_key(ptr);

		if (sp >= KEYRING_SEARCH_MAX_DEPTH) {
			if (ctx->flags & KEYRING_SEARCH_DETECT_TOO_DEEP) {
				ctx->result = ERR_PTR(-ELOOP);
				return false;
			}
			goto not_this_keyring;
		}

		/* Search a nested keyring */
		if (!(ctx->flags & KEYRING_SEARCH_NO_CHECK_PERM) &&
		    key_task_permission(make_key_ref(key, ctx->possessed),
					ctx->cred, KEY_NEED_SEARCH) < 0)
			continue;

		/* stack the current position */
		stack[sp].keyring = keyring;
		stack[sp].node = node;
		stack[sp].slot = slot;
		sp++;

		/* begin again with the new keyring */
		keyring = key;
		goto descend_to_keyring;
	}

	/* We've dealt with all the slots in the current node, so now we need
	 * to ascend to the parent and continue processing there.
	 */
	ptr = READ_ONCE(node->back_pointer);
	slot = node->parent_slot;

	if (ptr && assoc_array_ptr_is_shortcut(ptr)) {
		shortcut = assoc_array_ptr_to_shortcut(ptr);
		ptr = READ_ONCE(shortcut->back_pointer);
		slot = shortcut->parent_slot;
	}
	if (!ptr)
		goto not_this_keyring;
	node = assoc_array_ptr_to_node(ptr);
	slot++;

	/* If we've ascended to the root (zero backpointer), we must have just
	 * finished processing the leftmost branch rather than the root slots -
	 * so there can't be any more keyrings for us to find.
	 */
	if (node->back_pointer) {
		kdebug("ascend %d", slot);
		goto ascend_to_node;
	}

	/* The keyring we're looking at was disqualified or didn't contain a
	 * matching key.
	 */
not_this_keyring:
	kdebug("not_this_keyring %d", sp);
	if (sp <= 0) {
		kleave(" = false");
		return false;
	}

	/* Resume the processing of a keyring higher up in the tree */
	sp--;
	keyring = stack[sp].keyring;
	node = stack[sp].node;
	slot = stack[sp].slot + 1;
	kdebug("ascend to %d [%d]", keyring->serial, slot);
	goto ascend_to_node;

	/* We found a viable match */
found:
	key = key_ref_to_ptr(ctx->result);
	key_check(key);
	if (!(ctx->flags & KEYRING_SEARCH_NO_UPDATE_TIME)) {
		key->last_used_at = ctx->now;
		keyring->last_used_at = ctx->now;
		while (sp > 0)
			stack[--sp].keyring->last_used_at = ctx->now;
	}
	kleave(" = true");
	return true;
}

/**
 * keyring_search_rcu - Search a keyring tree for a matching key under RCU
 * @keyring_ref: A pointer to the keyring with possession indicator.
 * @ctx: The keyring search context.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Search the supplied keyring tree for a key that matches the criteria given.
 * The root keyring and any linked keyrings must grant Search permission to the
 * caller to be searchable and keys can only be found if they too grant Search
 * to the caller. The possession flag on the root keyring pointer controls use
 * of the possessor bits in permissions checking of the entire tree.  In
 * addition, the LSM gets to forbid keyring searches and key matches.
 *
 * The search is performed as a breadth-then-depth search up to the prescribed
<<<<<<< HEAD
 * limit (KEYRING_SEARCH_MAX_DEPTH).
 *
 * Keys are matched to the type provided and are then filtered by the match
 * function, which is given the description to use in any way it sees fit.  The
 * match function may use any attributes of a key that it wishes to to
 * determine the match.  Normally the match function from the key type would be
 * used.
 *
 * RCU is used to prevent the keyring key lists from disappearing without the
 * need to take lots of locks.
=======
 * limit (KEYRING_SEARCH_MAX_DEPTH).  The caller must hold the RCU read lock to
 * prevent keyrings from being destroyed or rearranged whilst they are being
 * searched.
 *
 * Keys are matched to the type provided and are then filtered by the match
 * function, which is given the description to use in any way it sees fit.  The
 * match function may use any attributes of a key that it wishes to
 * determine the match.  Normally the match function from the key type would be
 * used.
 *
 * RCU can be used to prevent the keyring key lists from disappearing without
 * the need to take lots of locks.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns a pointer to the found key and increments the key usage count if
 * successful; -EAGAIN if no matching keys were found, or if expired or revoked
 * keys were found; -ENOKEY if only negative keys were found; -ENOTDIR if the
 * specified keyring wasn't a keyring.
 *
 * In the case of a successful return, the possession attribute from
 * @keyring_ref is propagated to the returned key reference.
 */
<<<<<<< HEAD
key_ref_t keyring_search_aux(key_ref_t keyring_ref,
			     const struct cred *cred,
			     struct key_type *type,
			     const void *description,
			     key_match_func_t match,
			     bool no_state_check)
{
	struct {
		struct keyring_list *keylist;
		int kix;
	} stack[KEYRING_SEARCH_MAX_DEPTH];

	struct keyring_list *keylist;
	struct timespec now;
	unsigned long possessed, kflags;
	struct key *keyring, *key;
	key_ref_t key_ref;
	long err;
	int sp, nkeys, kix;

	keyring = key_ref_to_ptr(keyring_ref);
	possessed = is_key_possessed(keyring_ref);
	key_check(keyring);

	/* top keyring must have search permission to begin the search */
	err = key_task_permission(keyring_ref, cred, KEY_SEARCH);
	if (err < 0) {
		key_ref = ERR_PTR(err);
		goto error;
	}

	key_ref = ERR_PTR(-ENOTDIR);
	if (keyring->type != &key_type_keyring)
		goto error;

	rcu_read_lock();

	now = current_kernel_time();
	err = -EAGAIN;
	sp = 0;

	/* firstly we should check to see if this top-level keyring is what we
	 * are looking for */
	key_ref = ERR_PTR(-EAGAIN);
	kflags = keyring->flags;
	if (keyring->type == type && match(keyring, description)) {
		key = keyring;
		if (no_state_check)
			goto found;

		/* check it isn't negative and hasn't expired or been
		 * revoked */
		if (kflags & (1 << KEY_FLAG_REVOKED))
			goto error_2;
		if (key->expiry && now.tv_sec >= key->expiry)
			goto error_2;
		key_ref = ERR_PTR(key->type_data.reject_error);
		if (kflags & (1 << KEY_FLAG_NEGATIVE))
			goto error_2;
		goto found;
	}

	/* otherwise, the top keyring must not be revoked, expired, or
	 * negatively instantiated if we are to search it */
	key_ref = ERR_PTR(-EAGAIN);
	if (kflags & ((1 << KEY_FLAG_INVALIDATED) |
		      (1 << KEY_FLAG_REVOKED) |
		      (1 << KEY_FLAG_NEGATIVE)) ||
	    (keyring->expiry && now.tv_sec >= keyring->expiry))
		goto error_2;

	/* start processing a new keyring */
descend:
	kflags = keyring->flags;
	if (kflags & ((1 << KEY_FLAG_INVALIDATED) |
		      (1 << KEY_FLAG_REVOKED)))
		goto not_this_keyring;

	keylist = rcu_dereference(keyring->payload.subscriptions);
	if (!keylist)
		goto not_this_keyring;

	/* iterate through the keys in this keyring first */
	nkeys = keylist->nkeys;
	smp_rmb();
	for (kix = 0; kix < nkeys; kix++) {
		key = rcu_dereference(keylist->keys[kix]);
		kflags = key->flags;

		/* ignore keys not of this type */
		if (key->type != type)
			continue;

		/* skip invalidated, revoked and expired keys */
		if (!no_state_check) {
			if (kflags & ((1 << KEY_FLAG_INVALIDATED) |
				      (1 << KEY_FLAG_REVOKED)))
				continue;

			if (key->expiry && now.tv_sec >= key->expiry)
				continue;
		}

		/* keys that don't match */
		if (!match(key, description))
			continue;

		/* key must have search permissions */
		if (key_task_permission(make_key_ref(key, possessed),
					cred, KEY_SEARCH) < 0)
			continue;

		if (no_state_check)
			goto found;

		/* we set a different error code if we pass a negative key */
		if (kflags & (1 << KEY_FLAG_NEGATIVE)) {
			err = key->type_data.reject_error;
			continue;
		}

		goto found;
	}

	/* search through the keyrings nested in this one */
	kix = 0;
ascend:
	nkeys = keylist->nkeys;
	smp_rmb();
	for (; kix < nkeys; kix++) {
		key = rcu_dereference(keylist->keys[kix]);
		if (key->type != &key_type_keyring)
			continue;

		/* recursively search nested keyrings
		 * - only search keyrings for which we have search permission
		 */
		if (sp >= KEYRING_SEARCH_MAX_DEPTH)
			continue;

		if (key_task_permission(make_key_ref(key, possessed),
					cred, KEY_SEARCH) < 0)
			continue;

		/* stack the current position */
		stack[sp].keylist = keylist;
		stack[sp].kix = kix;
		sp++;

		/* begin again with the new keyring */
		keyring = key;
		goto descend;
	}

	/* the keyring we're looking at was disqualified or didn't contain a
	 * matching key */
not_this_keyring:
	if (sp > 0) {
		/* resume the processing of a keyring higher up in the tree */
		sp--;
		keylist = stack[sp].keylist;
		kix = stack[sp].kix + 1;
		goto ascend;
	}

	key_ref = ERR_PTR(err);
	goto error_2;

	/* we found a viable match */
found:
	atomic_inc(&key->usage);
	key_check(key);
	key_ref = make_key_ref(key, possessed);
error_2:
	rcu_read_unlock();
error:
	return key_ref;
=======
key_ref_t keyring_search_rcu(key_ref_t keyring_ref,
			     struct keyring_search_context *ctx)
{
	struct key *keyring;
	long err;

	ctx->iterator = keyring_search_iterator;
	ctx->possessed = is_key_possessed(keyring_ref);
	ctx->result = ERR_PTR(-EAGAIN);

	keyring = key_ref_to_ptr(keyring_ref);
	key_check(keyring);

	if (keyring->type != &key_type_keyring)
		return ERR_PTR(-ENOTDIR);

	if (!(ctx->flags & KEYRING_SEARCH_NO_CHECK_PERM)) {
		err = key_task_permission(keyring_ref, ctx->cred, KEY_NEED_SEARCH);
		if (err < 0)
			return ERR_PTR(err);
	}

	ctx->now = ktime_get_real_seconds();
	if (search_nested_keyrings(keyring, ctx))
		__key_get(key_ref_to_ptr(ctx->result));
	return ctx->result;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * keyring_search - Search the supplied keyring tree for a matching key
 * @keyring: The root of the keyring tree to be searched.
 * @type: The type of keyring we want to find.
 * @description: The name of the keyring we want to find.
<<<<<<< HEAD
 *
 * As keyring_search_aux() above, but using the current task's credentials and
 * type's default matching function.
 */
key_ref_t keyring_search(key_ref_t keyring,
			 struct key_type *type,
			 const char *description)
{
	if (!type->match)
		return ERR_PTR(-ENOKEY);

	return keyring_search_aux(keyring, current->cred,
				  type, description, type->match, false);
}
EXPORT_SYMBOL(keyring_search);

/*
 * Search the given keyring only (no recursion).
 *
 * The caller must guarantee that the keyring is a keyring and that the
 * permission is granted to search the keyring as no check is made here.
 *
 * RCU is used to make it unnecessary to lock the keyring key list here.
 *
 * Returns a pointer to the found key with usage count incremented if
 * successful and returns -ENOKEY if not found.  Revoked keys and keys not
 * providing the requested permission are skipped over.
=======
 * @recurse: True to search the children of @keyring also
 *
 * As keyring_search_rcu() above, but using the current task's credentials and
 * type's default matching function and preferred search method.
 */
key_ref_t keyring_search(key_ref_t keyring,
			 struct key_type *type,
			 const char *description,
			 bool recurse)
{
	struct keyring_search_context ctx = {
		.index_key.type		= type,
		.index_key.description	= description,
		.index_key.desc_len	= strlen(description),
		.cred			= current_cred(),
		.match_data.cmp		= key_default_cmp,
		.match_data.raw_data	= description,
		.match_data.lookup_type	= KEYRING_SEARCH_LOOKUP_DIRECT,
		.flags			= KEYRING_SEARCH_DO_STATE_CHECK,
	};
	key_ref_t key;
	int ret;

	if (recurse)
		ctx.flags |= KEYRING_SEARCH_RECURSE;
	if (type->match_preparse) {
		ret = type->match_preparse(&ctx.match_data);
		if (ret < 0)
			return ERR_PTR(ret);
	}

	rcu_read_lock();
	key = keyring_search_rcu(keyring, &ctx);
	rcu_read_unlock();

	if (type->match_free)
		type->match_free(&ctx.match_data);
	return key;
}
EXPORT_SYMBOL(keyring_search);

static struct key_restriction *keyring_restriction_alloc(
	key_restrict_link_func_t check)
{
	struct key_restriction *keyres =
		kzalloc(sizeof(struct key_restriction), GFP_KERNEL);

	if (!keyres)
		return ERR_PTR(-ENOMEM);

	keyres->check = check;

	return keyres;
}

/*
 * Semaphore to serialise restriction setup to prevent reference count
 * cycles through restriction key pointers.
 */
static DECLARE_RWSEM(keyring_serialise_restrict_sem);

/*
 * Check for restriction cycles that would prevent keyring garbage collection.
 * keyring_serialise_restrict_sem must be held.
 */
static bool keyring_detect_restriction_cycle(const struct key *dest_keyring,
					     struct key_restriction *keyres)
{
	while (keyres && keyres->key &&
	       keyres->key->type == &key_type_keyring) {
		if (keyres->key == dest_keyring)
			return true;

		keyres = keyres->key->restrict_link;
	}

	return false;
}

/**
 * keyring_restrict - Look up and apply a restriction to a keyring
 * @keyring_ref: The keyring to be restricted
 * @type: The key type that will provide the restriction checker.
 * @restriction: The restriction options to apply to the keyring
 *
 * Look up a keyring and apply a restriction to it.  The restriction is managed
 * by the specific key type, but can be configured by the options specified in
 * the restriction string.
 */
int keyring_restrict(key_ref_t keyring_ref, const char *type,
		     const char *restriction)
{
	struct key *keyring;
	struct key_type *restrict_type = NULL;
	struct key_restriction *restrict_link;
	int ret = 0;

	keyring = key_ref_to_ptr(keyring_ref);
	key_check(keyring);

	if (keyring->type != &key_type_keyring)
		return -ENOTDIR;

	if (!type) {
		restrict_link = keyring_restriction_alloc(restrict_link_reject);
	} else {
		restrict_type = key_type_lookup(type);

		if (IS_ERR(restrict_type))
			return PTR_ERR(restrict_type);

		if (!restrict_type->lookup_restriction) {
			ret = -ENOENT;
			goto error;
		}

		restrict_link = restrict_type->lookup_restriction(restriction);
	}

	if (IS_ERR(restrict_link)) {
		ret = PTR_ERR(restrict_link);
		goto error;
	}

	down_write(&keyring->sem);
	down_write(&keyring_serialise_restrict_sem);

	if (keyring->restrict_link) {
		ret = -EEXIST;
	} else if (keyring_detect_restriction_cycle(keyring, restrict_link)) {
		ret = -EDEADLK;
	} else {
		keyring->restrict_link = restrict_link;
		notify_key(keyring, NOTIFY_KEY_SETATTR, 0);
	}

	up_write(&keyring_serialise_restrict_sem);
	up_write(&keyring->sem);

	if (ret < 0) {
		key_put(restrict_link->key);
		kfree(restrict_link);
	}

error:
	if (restrict_type)
		key_type_put(restrict_type);

	return ret;
}
EXPORT_SYMBOL(keyring_restrict);

/*
 * Search the given keyring for a key that might be updated.
 *
 * The caller must guarantee that the keyring is a keyring and that the
 * permission is granted to modify the keyring as no check is made here.  The
 * caller must also hold a lock on the keyring semaphore.
 *
 * Returns a pointer to the found key with usage count incremented if
 * successful and returns NULL if not found.  Revoked and invalidated keys are
 * skipped over.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * If successful, the possession indicator is propagated from the keyring ref
 * to the returned key reference.
 */
<<<<<<< HEAD
key_ref_t __keyring_search_one(key_ref_t keyring_ref,
			       const struct key_type *ktype,
			       const char *description,
			       key_perm_t perm)
{
	struct keyring_list *klist;
	unsigned long possessed;
	struct key *keyring, *key;
	int nkeys, loop;

	keyring = key_ref_to_ptr(keyring_ref);
	possessed = is_key_possessed(keyring_ref);

	rcu_read_lock();

	klist = rcu_dereference(keyring->payload.subscriptions);
	if (klist) {
		nkeys = klist->nkeys;
		smp_rmb();
		for (loop = 0; loop < nkeys ; loop++) {
			key = rcu_dereference(klist->keys[loop]);
			if (key->type == ktype &&
			    (!key->type->match ||
			     key->type->match(key, description)) &&
			    key_permission(make_key_ref(key, possessed),
					   perm) == 0 &&
			    !(key->flags & ((1 << KEY_FLAG_INVALIDATED) |
					    (1 << KEY_FLAG_REVOKED)))
			    )
				goto found;
		}
	}

	rcu_read_unlock();
	return ERR_PTR(-ENOKEY);

found:
	atomic_inc(&key->usage);
	rcu_read_unlock();
	return make_key_ref(key, possessed);
=======
key_ref_t find_key_to_update(key_ref_t keyring_ref,
			     const struct keyring_index_key *index_key)
{
	struct key *keyring, *key;
	const void *object;

	keyring = key_ref_to_ptr(keyring_ref);

	kenter("{%d},{%s,%s}",
	       keyring->serial, index_key->type->name, index_key->description);

	object = assoc_array_find(&keyring->keys, &keyring_assoc_array_ops,
				  index_key);

	if (object)
		goto found;

	kleave(" = NULL");
	return NULL;

found:
	key = keyring_ptr_to_key(object);
	if (key->flags & ((1 << KEY_FLAG_INVALIDATED) |
			  (1 << KEY_FLAG_REVOKED))) {
		kleave(" = NULL [x]");
		return NULL;
	}
	__key_get(key);
	kleave(" = {%d}", key->serial);
	return make_key_ref(key, is_key_possessed(keyring_ref));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Find a keyring with the specified name.
 *
<<<<<<< HEAD
 * All named keyrings in the current user namespace are searched, provided they
 * grant Search permission directly to the caller (unless this check is
 * skipped).  Keyrings whose usage points have reached zero or who have been
 * revoked are skipped.
=======
 * Only keyrings that have nonzero refcount, are not revoked, and are owned by a
 * user in the current user namespace are considered.  If @uid_keyring is %true,
 * the keyring additionally must have been allocated as a user or user session
 * keyring; otherwise, it must grant Search permission directly to the caller.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns a pointer to the keyring with the keyring's refcount having being
 * incremented on success.  -ENOKEY is returned if a key could not be found.
 */
<<<<<<< HEAD
struct key *find_keyring_by_name(const char *name, bool skip_perm_check)
{
	struct key *keyring;
	int bucket;
=======
struct key *find_keyring_by_name(const char *name, bool uid_keyring)
{
	struct user_namespace *ns = current_user_ns();
	struct key *keyring;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!name)
		return ERR_PTR(-EINVAL);

<<<<<<< HEAD
	bucket = keyring_hash(name);

	read_lock(&keyring_name_lock);

	if (keyring_name_hash[bucket].next) {
		/* search this hash bucket for a keyring with a matching name
		 * that's readable and that hasn't been revoked */
		list_for_each_entry(keyring,
				    &keyring_name_hash[bucket],
				    type_data.link
				    ) {
			if (keyring->user->user_ns != current_user_ns())
				continue;

			if (test_bit(KEY_FLAG_REVOKED, &keyring->flags))
				continue;

			if (strcmp(keyring->description, name) != 0)
				continue;

			if (!skip_perm_check &&
			    key_permission(make_key_ref(keyring, 0),
					   KEY_SEARCH) < 0)
				continue;

			/* we've got a match but we might end up racing with
			 * key_cleanup() if the keyring is currently 'dead'
			 * (ie. it has a zero usage count) */
			if (!atomic_inc_not_zero(&keyring->usage))
				continue;
			goto out;
		}
=======
	read_lock(&keyring_name_lock);

	/* Search this hash bucket for a keyring with a matching name that
	 * grants Search permission and that hasn't been revoked
	 */
	list_for_each_entry(keyring, &ns->keyring_name_list, name_link) {
		if (!kuid_has_mapping(ns, keyring->user->uid))
			continue;

		if (test_bit(KEY_FLAG_REVOKED, &keyring->flags))
			continue;

		if (strcmp(keyring->description, name) != 0)
			continue;

		if (uid_keyring) {
			if (!test_bit(KEY_FLAG_UID_KEYRING,
				      &keyring->flags))
				continue;
		} else {
			if (key_permission(make_key_ref(keyring, 0),
					   KEY_NEED_SEARCH) < 0)
				continue;
		}

		/* we've got a match but we might end up racing with
		 * key_cleanup() if the keyring is currently 'dead'
		 * (ie. it has a zero usage count) */
		if (!refcount_inc_not_zero(&keyring->usage))
			continue;
		keyring->last_used_at = ktime_get_real_seconds();
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	keyring = ERR_PTR(-ENOKEY);
out:
	read_unlock(&keyring_name_lock);
	return keyring;
}

<<<<<<< HEAD
/*
 * See if a cycle will will be created by inserting acyclic tree B in acyclic
=======
static int keyring_detect_cycle_iterator(const void *object,
					 void *iterator_data)
{
	struct keyring_search_context *ctx = iterator_data;
	const struct key *key = keyring_ptr_to_key(object);

	kenter("{%d}", key->serial);

	/* We might get a keyring with matching index-key that is nonetheless a
	 * different keyring. */
	if (key != ctx->match_data.raw_data)
		return 0;

	ctx->result = ERR_PTR(-EDEADLK);
	return 1;
}

/*
 * See if a cycle will be created by inserting acyclic tree B in acyclic
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * tree A at the topmost level (ie: as a direct child of A).
 *
 * Since we are adding B to A at the top level, checking for cycles should just
 * be a matter of seeing if node A is somewhere in tree B.
 */
static int keyring_detect_cycle(struct key *A, struct key *B)
{
<<<<<<< HEAD
	struct {
		struct keyring_list *keylist;
		int kix;
	} stack[KEYRING_SEARCH_MAX_DEPTH];

	struct keyring_list *keylist;
	struct key *subtree, *key;
	int sp, nkeys, kix, ret;

	rcu_read_lock();

	ret = -EDEADLK;
	if (A == B)
		goto cycle_detected;

	subtree = B;
	sp = 0;

	/* start processing a new keyring */
descend:
	if (test_bit(KEY_FLAG_REVOKED, &subtree->flags))
		goto not_this_keyring;

	keylist = rcu_dereference(subtree->payload.subscriptions);
	if (!keylist)
		goto not_this_keyring;
	kix = 0;

ascend:
	/* iterate through the remaining keys in this keyring */
	nkeys = keylist->nkeys;
	smp_rmb();
	for (; kix < nkeys; kix++) {
		key = rcu_dereference(keylist->keys[kix]);

		if (key == A)
			goto cycle_detected;

		/* recursively check nested keyrings */
		if (key->type == &key_type_keyring) {
			if (sp >= KEYRING_SEARCH_MAX_DEPTH)
				goto too_deep;

			/* stack the current position */
			stack[sp].keylist = keylist;
			stack[sp].kix = kix;
			sp++;

			/* begin again with the new keyring */
			subtree = key;
			goto descend;
		}
	}

	/* the keyring we're looking at was disqualified or didn't contain a
	 * matching key */
not_this_keyring:
	if (sp > 0) {
		/* resume the checking of a keyring higher up in the tree */
		sp--;
		keylist = stack[sp].keylist;
		kix = stack[sp].kix + 1;
		goto ascend;
	}

	ret = 0; /* no cycles detected */

error:
	rcu_read_unlock();
	return ret;

too_deep:
	ret = -ELOOP;
	goto error;

cycle_detected:
	ret = -EDEADLK;
	goto error;
}

/*
 * Dispose of a keyring list after the RCU grace period, freeing the unlinked
 * key
 */
static void keyring_unlink_rcu_disposal(struct rcu_head *rcu)
{
	struct keyring_list *klist =
		container_of(rcu, struct keyring_list, rcu);

	if (klist->delkey != USHRT_MAX)
		key_put(rcu_access_pointer(klist->keys[klist->delkey]));
	kfree(klist);
}

/*
 * Preallocate memory so that a key can be linked into to a keyring.
 */
int __key_link_begin(struct key *keyring, const struct key_type *type,
		     const char *description, unsigned long *_prealloc)
	__acquires(&keyring->sem)
{
	struct keyring_list *klist, *nklist;
	unsigned long prealloc;
	unsigned max;
	size_t size;
	int loop, ret;

	kenter("%d,%s,%s,", key_serial(keyring), type->name, description);

=======
	struct keyring_search_context ctx = {
		.index_key		= A->index_key,
		.match_data.raw_data	= A,
		.match_data.lookup_type = KEYRING_SEARCH_LOOKUP_DIRECT,
		.iterator		= keyring_detect_cycle_iterator,
		.flags			= (KEYRING_SEARCH_NO_STATE_CHECK |
					   KEYRING_SEARCH_NO_UPDATE_TIME |
					   KEYRING_SEARCH_NO_CHECK_PERM |
					   KEYRING_SEARCH_DETECT_TOO_DEEP |
					   KEYRING_SEARCH_RECURSE),
	};

	rcu_read_lock();
	search_nested_keyrings(B, &ctx);
	rcu_read_unlock();
	return PTR_ERR(ctx.result) == -EAGAIN ? 0 : PTR_ERR(ctx.result);
}

/*
 * Lock keyring for link.
 */
int __key_link_lock(struct key *keyring,
		    const struct keyring_index_key *index_key)
	__acquires(&keyring->sem)
	__acquires(&keyring_serialise_link_lock)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (keyring->type != &key_type_keyring)
		return -ENOTDIR;

	down_write(&keyring->sem);

<<<<<<< HEAD
	ret = -EKEYREVOKED;
	if (test_bit(KEY_FLAG_REVOKED, &keyring->flags))
		goto error_krsem;

	/* serialise link/link calls to prevent parallel calls causing a cycle
	 * when linking two keyring in opposite orders */
	if (type == &key_type_keyring)
		down_write(&keyring_serialise_link_sem);

	klist = rcu_dereference_locked_keyring(keyring);

	/* see if there's a matching key we can displace */
	if (klist && klist->nkeys > 0) {
		for (loop = klist->nkeys - 1; loop >= 0; loop--) {
			struct key *key = rcu_deref_link_locked(klist, loop,
								keyring);
			if (key->type == type &&
			    strcmp(key->description, description) == 0) {
				/* Found a match - we'll replace the link with
				 * one to the new key.  We record the slot
				 * position.
				 */
				klist->delkey = loop;
				prealloc = 0;
				goto done;
			}
		}
	}

	/* check that we aren't going to overrun the user's quota */
	ret = key_payload_reserve(keyring,
				  keyring->datalen + KEYQUOTA_LINK_BYTES);
	if (ret < 0)
		goto error_sem;

	if (klist && klist->nkeys < klist->maxkeys) {
		/* there's sufficient slack space to append directly */
		klist->delkey = klist->nkeys;
		prealloc = KEY_LINK_FIXQUOTA;
	} else {
		/* grow the key list */
		max = 4;
		if (klist)
			max += klist->maxkeys;

		ret = -ENFILE;
		if (max > USHRT_MAX - 1)
			goto error_quota;
		size = sizeof(*klist) + sizeof(struct key *) * max;
		if (size > PAGE_SIZE)
			goto error_quota;

		ret = -ENOMEM;
		nklist = kmalloc(size, GFP_KERNEL);
		if (!nklist)
			goto error_quota;

		nklist->maxkeys = max;
		if (klist) {
			memcpy(nklist->keys, klist->keys,
			       sizeof(struct key *) * klist->nkeys);
			nklist->delkey = klist->nkeys;
			nklist->nkeys = klist->nkeys + 1;
			klist->delkey = USHRT_MAX;
		} else {
			nklist->nkeys = 1;
			nklist->delkey = 0;
		}

		/* add the key into the new space */
		RCU_INIT_POINTER(nklist->keys[nklist->delkey], NULL);
		prealloc = (unsigned long)nklist | KEY_LINK_FIXQUOTA;
	}

done:
	*_prealloc = prealloc;
	kleave(" = 0");
	return 0;

error_quota:
	/* undo the quota changes */
	key_payload_reserve(keyring,
			    keyring->datalen - KEYQUOTA_LINK_BYTES);
error_sem:
	if (type == &key_type_keyring)
		up_write(&keyring_serialise_link_sem);
error_krsem:
	up_write(&keyring->sem);
=======
	/* Serialise link/link calls to prevent parallel calls causing a cycle
	 * when linking two keyring in opposite orders.
	 */
	if (index_key->type == &key_type_keyring)
		mutex_lock(&keyring_serialise_link_lock);

	return 0;
}

/*
 * Lock keyrings for move (link/unlink combination).
 */
int __key_move_lock(struct key *l_keyring, struct key *u_keyring,
		    const struct keyring_index_key *index_key)
	__acquires(&l_keyring->sem)
	__acquires(&u_keyring->sem)
	__acquires(&keyring_serialise_link_lock)
{
	if (l_keyring->type != &key_type_keyring ||
	    u_keyring->type != &key_type_keyring)
		return -ENOTDIR;

	/* We have to be very careful here to take the keyring locks in the
	 * right order, lest we open ourselves to deadlocking against another
	 * move operation.
	 */
	if (l_keyring < u_keyring) {
		down_write(&l_keyring->sem);
		down_write_nested(&u_keyring->sem, 1);
	} else {
		down_write(&u_keyring->sem);
		down_write_nested(&l_keyring->sem, 1);
	}

	/* Serialise link/link calls to prevent parallel calls causing a cycle
	 * when linking two keyring in opposite orders.
	 */
	if (index_key->type == &key_type_keyring)
		mutex_lock(&keyring_serialise_link_lock);

	return 0;
}

/*
 * Preallocate memory so that a key can be linked into to a keyring.
 */
int __key_link_begin(struct key *keyring,
		     const struct keyring_index_key *index_key,
		     struct assoc_array_edit **_edit)
{
	struct assoc_array_edit *edit;
	int ret;

	kenter("%d,%s,%s,",
	       keyring->serial, index_key->type->name, index_key->description);

	BUG_ON(index_key->desc_len == 0);
	BUG_ON(*_edit != NULL);

	*_edit = NULL;

	ret = -EKEYREVOKED;
	if (test_bit(KEY_FLAG_REVOKED, &keyring->flags))
		goto error;

	/* Create an edit script that will insert/replace the key in the
	 * keyring tree.
	 */
	edit = assoc_array_insert(&keyring->keys,
				  &keyring_assoc_array_ops,
				  index_key,
				  NULL);
	if (IS_ERR(edit)) {
		ret = PTR_ERR(edit);
		goto error;
	}

	/* If we're not replacing a link in-place then we're going to need some
	 * extra quota.
	 */
	if (!edit->dead_leaf) {
		ret = key_payload_reserve(keyring,
					  keyring->datalen + KEYQUOTA_LINK_BYTES);
		if (ret < 0)
			goto error_cancel;
	}

	*_edit = edit;
	kleave(" = 0");
	return 0;

error_cancel:
	assoc_array_cancel_edit(edit);
error:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kleave(" = %d", ret);
	return ret;
}

/*
 * Check already instantiated keys aren't going to be a problem.
 *
 * The caller must have called __key_link_begin(). Don't need to call this for
 * keys that were created since __key_link_begin() was called.
 */
int __key_link_check_live_key(struct key *keyring, struct key *key)
{
	if (key->type == &key_type_keyring)
		/* check that we aren't going to create a cycle by linking one
		 * keyring to another */
		return keyring_detect_cycle(keyring, key);
	return 0;
}

/*
 * Link a key into to a keyring.
 *
 * Must be called with __key_link_begin() having being called.  Discards any
 * already extant link to matching key if there is one, so that each keyring
 * holds at most one link to any given key of a particular type+description
 * combination.
 */
void __key_link(struct key *keyring, struct key *key,
<<<<<<< HEAD
		unsigned long *_prealloc)
{
	struct keyring_list *klist, *nklist;
	struct key *discard;

	nklist = (struct keyring_list *)(*_prealloc & ~KEY_LINK_FIXQUOTA);
	*_prealloc = 0;

	kenter("%d,%d,%p", keyring->serial, key->serial, nklist);

	klist = rcu_dereference_locked_keyring(keyring);

	atomic_inc(&key->usage);

	/* there's a matching key we can displace or an empty slot in a newly
	 * allocated list we can fill */
	if (nklist) {
		kdebug("reissue %hu/%hu/%hu",
		       nklist->delkey, nklist->nkeys, nklist->maxkeys);

		RCU_INIT_POINTER(nklist->keys[nklist->delkey], key);

		rcu_assign_pointer(keyring->payload.subscriptions, nklist);

		/* dispose of the old keyring list and, if there was one, the
		 * displaced key */
		if (klist) {
			kdebug("dispose %hu/%hu/%hu",
			       klist->delkey, klist->nkeys, klist->maxkeys);
			call_rcu(&klist->rcu, keyring_unlink_rcu_disposal);
		}
	} else if (klist->delkey < klist->nkeys) {
		kdebug("replace %hu/%hu/%hu",
		       klist->delkey, klist->nkeys, klist->maxkeys);

		discard = rcu_dereference_protected(
			klist->keys[klist->delkey],
			rwsem_is_locked(&keyring->sem));
		rcu_assign_pointer(klist->keys[klist->delkey], key);
		/* The garbage collector will take care of RCU
		 * synchronisation */
		key_put(discard);
	} else {
		/* there's sufficient slack space to append directly */
		kdebug("append %hu/%hu/%hu",
		       klist->delkey, klist->nkeys, klist->maxkeys);

		RCU_INIT_POINTER(klist->keys[klist->delkey], key);
		smp_wmb();
		klist->nkeys++;
	}
=======
		struct assoc_array_edit **_edit)
{
	__key_get(key);
	assoc_array_insert_set_object(*_edit, keyring_key_to_ptr(key));
	assoc_array_apply_edit(*_edit);
	*_edit = NULL;
	notify_key(keyring, NOTIFY_KEY_LINKED, key_serial(key));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Finish linking a key into to a keyring.
 *
 * Must be called with __key_link_begin() having being called.
 */
<<<<<<< HEAD
void __key_link_end(struct key *keyring, struct key_type *type,
		    unsigned long prealloc)
	__releases(&keyring->sem)
{
	BUG_ON(type == NULL);
	BUG_ON(type->name == NULL);
	kenter("%d,%s,%lx", keyring->serial, type->name, prealloc);

	if (type == &key_type_keyring)
		up_write(&keyring_serialise_link_sem);

	if (prealloc) {
		if (prealloc & KEY_LINK_FIXQUOTA)
			key_payload_reserve(keyring,
					    keyring->datalen -
					    KEYQUOTA_LINK_BYTES);
		kfree((struct keyring_list *)(prealloc & ~KEY_LINK_FIXQUOTA));
	}
	up_write(&keyring->sem);
=======
void __key_link_end(struct key *keyring,
		    const struct keyring_index_key *index_key,
		    struct assoc_array_edit *edit)
	__releases(&keyring->sem)
	__releases(&keyring_serialise_link_lock)
{
	BUG_ON(index_key->type == NULL);
	kenter("%d,%s,", keyring->serial, index_key->type->name);

	if (edit) {
		if (!edit->dead_leaf) {
			key_payload_reserve(keyring,
				keyring->datalen - KEYQUOTA_LINK_BYTES);
		}
		assoc_array_cancel_edit(edit);
	}
	up_write(&keyring->sem);

	if (index_key->type == &key_type_keyring)
		mutex_unlock(&keyring_serialise_link_lock);
}

/*
 * Check addition of keys to restricted keyrings.
 */
static int __key_link_check_restriction(struct key *keyring, struct key *key)
{
	if (!keyring->restrict_link || !keyring->restrict_link->check)
		return 0;
	return keyring->restrict_link->check(keyring, key->type, &key->payload,
					     keyring->restrict_link->key);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * key_link - Link a key to a keyring
 * @keyring: The keyring to make the link in.
 * @key: The key to link to.
 *
 * Make a link in a keyring to a key, such that the keyring holds a reference
 * on that key and the key can potentially be found by searching that keyring.
 *
 * This function will write-lock the keyring's semaphore and will consume some
 * of the user's key data quota to hold the link.
 *
 * Returns 0 if successful, -ENOTDIR if the keyring isn't a keyring,
 * -EKEYREVOKED if the keyring has been revoked, -ENFILE if the keyring is
 * full, -EDQUOT if there is insufficient key data quota remaining to add
 * another link or -ENOMEM if there's insufficient memory.
 *
 * It is assumed that the caller has checked that it is permitted for a link to
 * be made (the keyring should have Write permission and the key Link
 * permission).
 */
int key_link(struct key *keyring, struct key *key)
{
<<<<<<< HEAD
	unsigned long prealloc;
	int ret;

	key_check(keyring);
	key_check(key);

	ret = __key_link_begin(keyring, key->type, key->description, &prealloc);
	if (ret == 0) {
		ret = __key_link_check_live_key(keyring, key);
		if (ret == 0)
			__key_link(keyring, key, &prealloc);
		__key_link_end(keyring, key->type, prealloc);
	}

=======
	struct assoc_array_edit *edit = NULL;
	int ret;

	kenter("{%d,%d}", keyring->serial, refcount_read(&keyring->usage));

	key_check(keyring);
	key_check(key);

	ret = __key_link_lock(keyring, &key->index_key);
	if (ret < 0)
		goto error;

	ret = __key_link_begin(keyring, &key->index_key, &edit);
	if (ret < 0)
		goto error_end;

	kdebug("begun {%d,%d}", keyring->serial, refcount_read(&keyring->usage));
	ret = __key_link_check_restriction(keyring, key);
	if (ret == 0)
		ret = __key_link_check_live_key(keyring, key);
	if (ret == 0)
		__key_link(keyring, key, &edit);

error_end:
	__key_link_end(keyring, &key->index_key, edit);
error:
	kleave(" = %d {%d,%d}", ret, keyring->serial, refcount_read(&keyring->usage));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(key_link);

<<<<<<< HEAD
=======
/*
 * Lock a keyring for unlink.
 */
static int __key_unlink_lock(struct key *keyring)
	__acquires(&keyring->sem)
{
	if (keyring->type != &key_type_keyring)
		return -ENOTDIR;

	down_write(&keyring->sem);
	return 0;
}

/*
 * Begin the process of unlinking a key from a keyring.
 */
static int __key_unlink_begin(struct key *keyring, struct key *key,
			      struct assoc_array_edit **_edit)
{
	struct assoc_array_edit *edit;

	BUG_ON(*_edit != NULL);

	edit = assoc_array_delete(&keyring->keys, &keyring_assoc_array_ops,
				  &key->index_key);
	if (IS_ERR(edit))
		return PTR_ERR(edit);

	if (!edit)
		return -ENOENT;

	*_edit = edit;
	return 0;
}

/*
 * Apply an unlink change.
 */
static void __key_unlink(struct key *keyring, struct key *key,
			 struct assoc_array_edit **_edit)
{
	assoc_array_apply_edit(*_edit);
	notify_key(keyring, NOTIFY_KEY_UNLINKED, key_serial(key));
	*_edit = NULL;
	key_payload_reserve(keyring, keyring->datalen - KEYQUOTA_LINK_BYTES);
}

/*
 * Finish unlinking a key from to a keyring.
 */
static void __key_unlink_end(struct key *keyring,
			     struct key *key,
			     struct assoc_array_edit *edit)
	__releases(&keyring->sem)
{
	if (edit)
		assoc_array_cancel_edit(edit);
	up_write(&keyring->sem);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * key_unlink - Unlink the first link to a key from a keyring.
 * @keyring: The keyring to remove the link from.
 * @key: The key the link is to.
 *
 * Remove a link from a keyring to a key.
 *
 * This function will write-lock the keyring's semaphore.
 *
 * Returns 0 if successful, -ENOTDIR if the keyring isn't a keyring, -ENOENT if
 * the key isn't linked to by the keyring or -ENOMEM if there's insufficient
 * memory.
 *
 * It is assumed that the caller has checked that it is permitted for a link to
 * be removed (the keyring should have Write permission; no permissions are
 * required on the key).
 */
int key_unlink(struct key *keyring, struct key *key)
{
<<<<<<< HEAD
	struct keyring_list *klist, *nklist;
	int loop, ret;
=======
	struct assoc_array_edit *edit = NULL;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	key_check(keyring);
	key_check(key);

<<<<<<< HEAD
	ret = -ENOTDIR;
	if (keyring->type != &key_type_keyring)
		goto error;

	down_write(&keyring->sem);

	klist = rcu_dereference_locked_keyring(keyring);
	if (klist) {
		/* search the keyring for the key */
		for (loop = 0; loop < klist->nkeys; loop++)
			if (rcu_access_pointer(klist->keys[loop]) == key)
				goto key_is_present;
	}

	up_write(&keyring->sem);
	ret = -ENOENT;
	goto error;

key_is_present:
	/* we need to copy the key list for RCU purposes */
	nklist = kmalloc(sizeof(*klist) +
			 sizeof(struct key *) * klist->maxkeys,
			 GFP_KERNEL);
	if (!nklist)
		goto nomem;
	nklist->maxkeys = klist->maxkeys;
	nklist->nkeys = klist->nkeys - 1;

	if (loop > 0)
		memcpy(&nklist->keys[0],
		       &klist->keys[0],
		       loop * sizeof(struct key *));

	if (loop < nklist->nkeys)
		memcpy(&nklist->keys[loop],
		       &klist->keys[loop + 1],
		       (nklist->nkeys - loop) * sizeof(struct key *));

	/* adjust the user's quota */
	key_payload_reserve(keyring,
			    keyring->datalen - KEYQUOTA_LINK_BYTES);

	rcu_assign_pointer(keyring->payload.subscriptions, nklist);

	up_write(&keyring->sem);

	/* schedule for later cleanup */
	klist->delkey = loop;
	call_rcu(&klist->rcu, keyring_unlink_rcu_disposal);

	ret = 0;

error:
	return ret;
nomem:
	ret = -ENOMEM;
	up_write(&keyring->sem);
	goto error;
}
EXPORT_SYMBOL(key_unlink);

/*
 * Dispose of a keyring list after the RCU grace period, releasing the keys it
 * links to.
 */
static void keyring_clear_rcu_disposal(struct rcu_head *rcu)
{
	struct keyring_list *klist;
	int loop;

	klist = container_of(rcu, struct keyring_list, rcu);

	for (loop = klist->nkeys - 1; loop >= 0; loop--)
		key_put(rcu_access_pointer(klist->keys[loop]));

	kfree(klist);
}
=======
	ret = __key_unlink_lock(keyring);
	if (ret < 0)
		return ret;

	ret = __key_unlink_begin(keyring, key, &edit);
	if (ret == 0)
		__key_unlink(keyring, key, &edit);
	__key_unlink_end(keyring, key, edit);
	return ret;
}
EXPORT_SYMBOL(key_unlink);

/**
 * key_move - Move a key from one keyring to another
 * @key: The key to move
 * @from_keyring: The keyring to remove the link from.
 * @to_keyring: The keyring to make the link in.
 * @flags: Qualifying flags, such as KEYCTL_MOVE_EXCL.
 *
 * Make a link in @to_keyring to a key, such that the keyring holds a reference
 * on that key and the key can potentially be found by searching that keyring
 * whilst simultaneously removing a link to the key from @from_keyring.
 *
 * This function will write-lock both keyring's semaphores and will consume
 * some of the user's key data quota to hold the link on @to_keyring.
 *
 * Returns 0 if successful, -ENOTDIR if either keyring isn't a keyring,
 * -EKEYREVOKED if either keyring has been revoked, -ENFILE if the second
 * keyring is full, -EDQUOT if there is insufficient key data quota remaining
 * to add another link or -ENOMEM if there's insufficient memory.  If
 * KEYCTL_MOVE_EXCL is set, then -EEXIST will be returned if there's already a
 * matching key in @to_keyring.
 *
 * It is assumed that the caller has checked that it is permitted for a link to
 * be made (the keyring should have Write permission and the key Link
 * permission).
 */
int key_move(struct key *key,
	     struct key *from_keyring,
	     struct key *to_keyring,
	     unsigned int flags)
{
	struct assoc_array_edit *from_edit = NULL, *to_edit = NULL;
	int ret;

	kenter("%d,%d,%d", key->serial, from_keyring->serial, to_keyring->serial);

	if (from_keyring == to_keyring)
		return 0;

	key_check(key);
	key_check(from_keyring);
	key_check(to_keyring);

	ret = __key_move_lock(from_keyring, to_keyring, &key->index_key);
	if (ret < 0)
		goto out;
	ret = __key_unlink_begin(from_keyring, key, &from_edit);
	if (ret < 0)
		goto error;
	ret = __key_link_begin(to_keyring, &key->index_key, &to_edit);
	if (ret < 0)
		goto error;

	ret = -EEXIST;
	if (to_edit->dead_leaf && (flags & KEYCTL_MOVE_EXCL))
		goto error;

	ret = __key_link_check_restriction(to_keyring, key);
	if (ret < 0)
		goto error;
	ret = __key_link_check_live_key(to_keyring, key);
	if (ret < 0)
		goto error;

	__key_unlink(from_keyring, key, &from_edit);
	__key_link(to_keyring, key, &to_edit);
error:
	__key_link_end(to_keyring, &key->index_key, to_edit);
	__key_unlink_end(from_keyring, key, from_edit);
out:
	kleave(" = %d", ret);
	return ret;
}
EXPORT_SYMBOL(key_move);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * keyring_clear - Clear a keyring
 * @keyring: The keyring to clear.
 *
 * Clear the contents of the specified keyring.
 *
 * Returns 0 if successful or -ENOTDIR if the keyring isn't a keyring.
 */
int keyring_clear(struct key *keyring)
{
<<<<<<< HEAD
	struct keyring_list *klist;
	int ret;

	ret = -ENOTDIR;
	if (keyring->type == &key_type_keyring) {
		/* detach the pointer block with the locks held */
		down_write(&keyring->sem);

		klist = rcu_dereference_locked_keyring(keyring);
		if (klist) {
			/* adjust the quota */
			key_payload_reserve(keyring,
					    sizeof(struct keyring_list));

			rcu_assign_pointer(keyring->payload.subscriptions,
					   NULL);
		}

		up_write(&keyring->sem);

		/* free the keys after the locks have been dropped */
		if (klist)
			call_rcu(&klist->rcu, keyring_clear_rcu_disposal);

		ret = 0;
	}

=======
	struct assoc_array_edit *edit;
	int ret;

	if (keyring->type != &key_type_keyring)
		return -ENOTDIR;

	down_write(&keyring->sem);

	edit = assoc_array_clear(&keyring->keys, &keyring_assoc_array_ops);
	if (IS_ERR(edit)) {
		ret = PTR_ERR(edit);
	} else {
		if (edit)
			assoc_array_apply_edit(edit);
		notify_key(keyring, NOTIFY_KEY_CLEARED, 0);
		key_payload_reserve(keyring, 0);
		ret = 0;
	}

	up_write(&keyring->sem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(keyring_clear);

/*
 * Dispose of the links from a revoked keyring.
 *
 * This is called with the key sem write-locked.
 */
static void keyring_revoke(struct key *keyring)
{
<<<<<<< HEAD
	struct keyring_list *klist;

	klist = rcu_dereference_locked_keyring(keyring);

	/* adjust the quota */
	key_payload_reserve(keyring, 0);

	if (klist) {
		rcu_assign_pointer(keyring->payload.subscriptions, NULL);
		call_rcu(&klist->rcu, keyring_clear_rcu_disposal);
	}
}

/*
 * Collect garbage from the contents of a keyring, replacing the old list with
 * a new one with the pointers all shuffled down.
 *
 * Dead keys are classed as oned that are flagged as being dead or are revoked,
 * expired or negative keys that were revoked or expired before the specified
 * limit.
 */
void keyring_gc(struct key *keyring, time_t limit)
{
	struct keyring_list *klist, *new;
	struct key *key;
	int loop, keep, max;

	kenter("{%x,%s}", key_serial(keyring), keyring->description);

	down_write(&keyring->sem);

	klist = rcu_dereference_locked_keyring(keyring);
	if (!klist)
		goto no_klist;

	/* work out how many subscriptions we're keeping */
	keep = 0;
	for (loop = klist->nkeys - 1; loop >= 0; loop--)
		if (!key_is_dead(rcu_deref_link_locked(klist, loop, keyring),
				 limit))
			keep++;

	if (keep == klist->nkeys)
		goto just_return;

	/* allocate a new keyring payload */
	max = roundup(keep, 4);
	new = kmalloc(sizeof(struct keyring_list) + max * sizeof(struct key *),
		      GFP_KERNEL);
	if (!new)
		goto nomem;
	new->maxkeys = max;
	new->nkeys = 0;
	new->delkey = 0;

	/* install the live keys
	 * - must take care as expired keys may be updated back to life
	 */
	keep = 0;
	for (loop = klist->nkeys - 1; loop >= 0; loop--) {
		key = rcu_deref_link_locked(klist, loop, keyring);
		if (!key_is_dead(key, limit)) {
			if (keep >= max)
				goto discard_new;
			RCU_INIT_POINTER(new->keys[keep++], key_get(key));
		}
	}
	new->nkeys = keep;

	/* adjust the quota */
	key_payload_reserve(keyring,
			    sizeof(struct keyring_list) +
			    KEYQUOTA_LINK_BYTES * keep);

	if (keep == 0) {
		rcu_assign_pointer(keyring->payload.subscriptions, NULL);
		kfree(new);
	} else {
		rcu_assign_pointer(keyring->payload.subscriptions, new);
	}

	up_write(&keyring->sem);

	call_rcu(&klist->rcu, keyring_clear_rcu_disposal);
	kleave(" [yes]");
	return;

discard_new:
	new->nkeys = keep;
	keyring_clear_rcu_disposal(&new->rcu);
	up_write(&keyring->sem);
	kleave(" [discard]");
	return;

just_return:
	up_write(&keyring->sem);
	kleave(" [no dead]");
	return;

no_klist:
	up_write(&keyring->sem);
	kleave(" [no_klist]");
	return;

nomem:
	up_write(&keyring->sem);
	kleave(" [oom]");
=======
	struct assoc_array_edit *edit;

	edit = assoc_array_clear(&keyring->keys, &keyring_assoc_array_ops);
	if (!IS_ERR(edit)) {
		if (edit)
			assoc_array_apply_edit(edit);
		key_payload_reserve(keyring, 0);
	}
}

static bool keyring_gc_select_iterator(void *object, void *iterator_data)
{
	struct key *key = keyring_ptr_to_key(object);
	time64_t *limit = iterator_data;

	if (key_is_dead(key, *limit))
		return false;
	key_get(key);
	return true;
}

static int keyring_gc_check_iterator(const void *object, void *iterator_data)
{
	const struct key *key = keyring_ptr_to_key(object);
	time64_t *limit = iterator_data;

	key_check(key);
	return key_is_dead(key, *limit);
}

/*
 * Garbage collect pointers from a keyring.
 *
 * Not called with any locks held.  The keyring's key struct will not be
 * deallocated under us as only our caller may deallocate it.
 */
void keyring_gc(struct key *keyring, time64_t limit)
{
	int result;

	kenter("%x{%s}", keyring->serial, keyring->description ?: "");

	if (keyring->flags & ((1 << KEY_FLAG_INVALIDATED) |
			      (1 << KEY_FLAG_REVOKED)))
		goto dont_gc;

	/* scan the keyring looking for dead keys */
	rcu_read_lock();
	result = assoc_array_iterate(&keyring->keys,
				     keyring_gc_check_iterator, &limit);
	rcu_read_unlock();
	if (result == true)
		goto do_gc;

dont_gc:
	kleave(" [no gc]");
	return;

do_gc:
	down_write(&keyring->sem);
	assoc_array_gc(&keyring->keys, &keyring_assoc_array_ops,
		       keyring_gc_select_iterator, &limit);
	up_write(&keyring->sem);
	kleave(" [gc]");
}

/*
 * Garbage collect restriction pointers from a keyring.
 *
 * Keyring restrictions are associated with a key type, and must be cleaned
 * up if the key type is unregistered. The restriction is altered to always
 * reject additional keys so a keyring cannot be opened up by unregistering
 * a key type.
 *
 * Not called with any keyring locks held. The keyring's key struct will not
 * be deallocated under us as only our caller may deallocate it.
 *
 * The caller is required to hold key_types_sem and dead_type->sem. This is
 * fulfilled by key_gc_keytype() holding the locks on behalf of
 * key_garbage_collector(), which it invokes on a workqueue.
 */
void keyring_restriction_gc(struct key *keyring, struct key_type *dead_type)
{
	struct key_restriction *keyres;

	kenter("%x{%s}", keyring->serial, keyring->description ?: "");

	/*
	 * keyring->restrict_link is only assigned at key allocation time
	 * or with the key type locked, so the only values that could be
	 * concurrently assigned to keyring->restrict_link are for key
	 * types other than dead_type. Given this, it's ok to check
	 * the key type before acquiring keyring->sem.
	 */
	if (!dead_type || !keyring->restrict_link ||
	    keyring->restrict_link->keytype != dead_type) {
		kleave(" [no restriction gc]");
		return;
	}

	/* Lock the keyring to ensure that a link is not in progress */
	down_write(&keyring->sem);

	keyres = keyring->restrict_link;

	keyres->check = restrict_link_reject;

	key_put(keyres->key);
	keyres->key = NULL;
	keyres->keytype = NULL;

	up_write(&keyring->sem);

	kleave(" [restriction gc]");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
