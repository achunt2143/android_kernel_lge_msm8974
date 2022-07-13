<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Structure dynamic extension infrastructure
 * Copyright (C) 2004 Rusty Russell IBM Corporation
 * Copyright (C) 2007 Netfilter Core Team <coreteam@netfilter.org>
 * Copyright (C) 2007 USAGI/WIDE Project <http://www.linux-ipv6.org>
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */
#include <linux/kernel.h>
=======
 */
#include <linux/kernel.h>
#include <linux/kmemleak.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/rcupdate.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <net/netfilter/nf_conntrack_extend.h>

<<<<<<< HEAD
static struct nf_ct_ext_type __rcu *nf_ct_ext_types[NF_CT_EXT_NUM];
static DEFINE_MUTEX(nf_ct_ext_type_mutex);

void __nf_ct_ext_destroy(struct nf_conn *ct)
{
	unsigned int i;
	struct nf_ct_ext_type *t;
	struct nf_ct_ext *ext = ct->ext;

	for (i = 0; i < NF_CT_EXT_NUM; i++) {
		if (!__nf_ct_ext_exist(ext, i))
			continue;

		rcu_read_lock();
		t = rcu_dereference(nf_ct_ext_types[i]);

		/* Here the nf_ct_ext_type might have been unregisterd.
		 * I.e., it has responsible to cleanup private
		 * area in all conntracks when it is unregisterd.
		 */
		if (t && t->destroy)
			t->destroy(ct);
		rcu_read_unlock();
	}
}
EXPORT_SYMBOL(__nf_ct_ext_destroy);

static void *
nf_ct_ext_create(struct nf_ct_ext **ext, enum nf_ct_ext_id id,
		 size_t var_alloc_len, gfp_t gfp)
{
	unsigned int off, len;
	struct nf_ct_ext_type *t;
	size_t alloc_size;

	rcu_read_lock();
	t = rcu_dereference(nf_ct_ext_types[id]);
	BUG_ON(t == NULL);
	off = ALIGN(sizeof(struct nf_ct_ext), t->align);
	len = off + t->len + var_alloc_len;
	alloc_size = t->alloc_size + var_alloc_len;
	rcu_read_unlock();

	*ext = kzalloc(alloc_size, gfp);
	if (!*ext)
		return NULL;

	(*ext)->offset[id] = off;
	(*ext)->len = len;

	return (void *)(*ext) + off;
}

void *__nf_ct_ext_add_length(struct nf_conn *ct, enum nf_ct_ext_id id,
			     size_t var_alloc_len, gfp_t gfp)
{
	struct nf_ct_ext *old, *new;
	int i, newlen, newoff;
	struct nf_ct_ext_type *t;

	/* Conntrack must not be confirmed to avoid races on reallocation. */
	NF_CT_ASSERT(!nf_ct_is_confirmed(ct));

	old = ct->ext;
	if (!old)
		return nf_ct_ext_create(&ct->ext, id, var_alloc_len, gfp);

	if (__nf_ct_ext_exist(old, id))
		return NULL;

	rcu_read_lock();
	t = rcu_dereference(nf_ct_ext_types[id]);
	BUG_ON(t == NULL);

	newoff = ALIGN(old->len, t->align);
	newlen = newoff + t->len + var_alloc_len;
	rcu_read_unlock();

	new = __krealloc(old, newlen, gfp);
	if (!new)
		return NULL;

	if (new != old) {
		for (i = 0; i < NF_CT_EXT_NUM; i++) {
			if (!__nf_ct_ext_exist(old, i))
				continue;

			rcu_read_lock();
			t = rcu_dereference(nf_ct_ext_types[i]);
			if (t && t->move)
				t->move((void *)new + new->offset[i],
					(void *)old + old->offset[i]);
			rcu_read_unlock();
		}
		kfree_rcu(old, rcu);
		ct->ext = new;
=======
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_acct.h>
#include <net/netfilter/nf_conntrack_seqadj.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_zones.h>
#include <net/netfilter/nf_conntrack_timestamp.h>
#include <net/netfilter/nf_conntrack_timeout.h>
#include <net/netfilter/nf_conntrack_labels.h>
#include <net/netfilter/nf_conntrack_synproxy.h>
#include <net/netfilter/nf_conntrack_act_ct.h>
#include <net/netfilter/nf_nat.h>

#define NF_CT_EXT_PREALLOC	128u /* conntrack events are on by default */

atomic_t nf_conntrack_ext_genid __read_mostly = ATOMIC_INIT(1);

static const u8 nf_ct_ext_type_len[NF_CT_EXT_NUM] = {
	[NF_CT_EXT_HELPER] = sizeof(struct nf_conn_help),
#if IS_ENABLED(CONFIG_NF_NAT)
	[NF_CT_EXT_NAT] = sizeof(struct nf_conn_nat),
#endif
	[NF_CT_EXT_SEQADJ] = sizeof(struct nf_conn_seqadj),
	[NF_CT_EXT_ACCT] = sizeof(struct nf_conn_acct),
#ifdef CONFIG_NF_CONNTRACK_EVENTS
	[NF_CT_EXT_ECACHE] = sizeof(struct nf_conntrack_ecache),
#endif
#ifdef CONFIG_NF_CONNTRACK_TIMESTAMP
	[NF_CT_EXT_TSTAMP] = sizeof(struct nf_conn_tstamp),
#endif
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
	[NF_CT_EXT_TIMEOUT] = sizeof(struct nf_conn_timeout),
#endif
#ifdef CONFIG_NF_CONNTRACK_LABELS
	[NF_CT_EXT_LABELS] = sizeof(struct nf_conn_labels),
#endif
#if IS_ENABLED(CONFIG_NETFILTER_SYNPROXY)
	[NF_CT_EXT_SYNPROXY] = sizeof(struct nf_conn_synproxy),
#endif
#if IS_ENABLED(CONFIG_NET_ACT_CT)
	[NF_CT_EXT_ACT_CT] = sizeof(struct nf_conn_act_ct_ext),
#endif
};

static __always_inline unsigned int total_extension_size(void)
{
	/* remember to add new extensions below */
	BUILD_BUG_ON(NF_CT_EXT_NUM > 10);

	return sizeof(struct nf_ct_ext) +
	       sizeof(struct nf_conn_help)
#if IS_ENABLED(CONFIG_NF_NAT)
		+ sizeof(struct nf_conn_nat)
#endif
		+ sizeof(struct nf_conn_seqadj)
		+ sizeof(struct nf_conn_acct)
#ifdef CONFIG_NF_CONNTRACK_EVENTS
		+ sizeof(struct nf_conntrack_ecache)
#endif
#ifdef CONFIG_NF_CONNTRACK_TIMESTAMP
		+ sizeof(struct nf_conn_tstamp)
#endif
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
		+ sizeof(struct nf_conn_timeout)
#endif
#ifdef CONFIG_NF_CONNTRACK_LABELS
		+ sizeof(struct nf_conn_labels)
#endif
#if IS_ENABLED(CONFIG_NETFILTER_SYNPROXY)
		+ sizeof(struct nf_conn_synproxy)
#endif
#if IS_ENABLED(CONFIG_NET_ACT_CT)
		+ sizeof(struct nf_conn_act_ct_ext)
#endif
	;
}

void *nf_ct_ext_add(struct nf_conn *ct, enum nf_ct_ext_id id, gfp_t gfp)
{
	unsigned int newlen, newoff, oldlen, alloc;
	struct nf_ct_ext *new;

	/* Conntrack must not be confirmed to avoid races on reallocation. */
	WARN_ON(nf_ct_is_confirmed(ct));

	/* struct nf_ct_ext uses u8 to store offsets/size */
	BUILD_BUG_ON(total_extension_size() > 255u);

	if (ct->ext) {
		const struct nf_ct_ext *old = ct->ext;

		if (__nf_ct_ext_exist(old, id))
			return NULL;
		oldlen = old->len;
	} else {
		oldlen = sizeof(*new);
	}

	newoff = ALIGN(oldlen, __alignof__(struct nf_ct_ext));
	newlen = newoff + nf_ct_ext_type_len[id];

	alloc = max(newlen, NF_CT_EXT_PREALLOC);
	new = krealloc(ct->ext, alloc, gfp);
	if (!new)
		return NULL;

	if (!ct->ext) {
		memset(new->offset, 0, sizeof(new->offset));
		new->gen_id = atomic_read(&nf_conntrack_ext_genid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	new->offset[id] = newoff;
	new->len = newlen;
	memset((void *)new + newoff, 0, newlen - newoff);
<<<<<<< HEAD
	return (void *)new + newoff;
}
EXPORT_SYMBOL(__nf_ct_ext_add_length);

static void update_alloc_size(struct nf_ct_ext_type *type)
{
	int i, j;
	struct nf_ct_ext_type *t1, *t2;
	enum nf_ct_ext_id min = 0, max = NF_CT_EXT_NUM - 1;

	/* unnecessary to update all types */
	if ((type->flags & NF_CT_EXT_F_PREALLOC) == 0) {
		min = type->id;
		max = type->id;
	}

	/* This assumes that extended areas in conntrack for the types
	   whose NF_CT_EXT_F_PREALLOC bit set are allocated in order */
	for (i = min; i <= max; i++) {
		t1 = rcu_dereference_protected(nf_ct_ext_types[i],
				lockdep_is_held(&nf_ct_ext_type_mutex));
		if (!t1)
			continue;

		t1->alloc_size = ALIGN(sizeof(struct nf_ct_ext), t1->align) +
				 t1->len;
		for (j = 0; j < NF_CT_EXT_NUM; j++) {
			t2 = rcu_dereference_protected(nf_ct_ext_types[j],
				lockdep_is_held(&nf_ct_ext_type_mutex));
			if (t2 == NULL || t2 == t1 ||
			    (t2->flags & NF_CT_EXT_F_PREALLOC) == 0)
				continue;

			t1->alloc_size = ALIGN(t1->alloc_size, t2->align)
					 + t2->len;
		}
	}
}

/* This MUST be called in process context. */
int nf_ct_extend_register(struct nf_ct_ext_type *type)
{
	int ret = 0;

	mutex_lock(&nf_ct_ext_type_mutex);
	if (nf_ct_ext_types[type->id]) {
		ret = -EBUSY;
		goto out;
	}

	/* This ensures that nf_ct_ext_create() can allocate enough area
	   before updating alloc_size */
	type->alloc_size = ALIGN(sizeof(struct nf_ct_ext), type->align)
			   + type->len;
	rcu_assign_pointer(nf_ct_ext_types[type->id], type);
	update_alloc_size(type);
out:
	mutex_unlock(&nf_ct_ext_type_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(nf_ct_extend_register);

/* This MUST be called in process context. */
void nf_ct_extend_unregister(struct nf_ct_ext_type *type)
{
	mutex_lock(&nf_ct_ext_type_mutex);
	RCU_INIT_POINTER(nf_ct_ext_types[type->id], NULL);
	update_alloc_size(type);
	mutex_unlock(&nf_ct_ext_type_mutex);
	rcu_barrier(); /* Wait for completion of call_rcu()'s */
}
EXPORT_SYMBOL_GPL(nf_ct_extend_unregister);
=======

	ct->ext = new;
	return (void *)new + newoff;
}
EXPORT_SYMBOL(nf_ct_ext_add);

/* Use nf_ct_ext_find wrapper. This is only useful for unconfirmed entries. */
void *__nf_ct_ext_find(const struct nf_ct_ext *ext, u8 id)
{
	unsigned int gen_id = atomic_read(&nf_conntrack_ext_genid);
	unsigned int this_id = READ_ONCE(ext->gen_id);

	if (!__nf_ct_ext_exist(ext, id))
		return NULL;

	if (this_id == 0 || ext->gen_id == gen_id)
		return (void *)ext + ext->offset[id];

	return NULL;
}
EXPORT_SYMBOL(__nf_ct_ext_find);

void nf_ct_ext_bump_genid(void)
{
	unsigned int value = atomic_inc_return(&nf_conntrack_ext_genid);

	if (value == UINT_MAX)
		atomic_set(&nf_conntrack_ext_genid, 1);

	msleep(HZ);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
