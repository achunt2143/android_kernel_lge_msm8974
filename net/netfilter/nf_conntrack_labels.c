<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * test/set flag bits stored in conntrack extension area.
 *
 * (C) 2013 Astaro GmbH & Co KG
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/ctype.h>
#include <linux/export.h>
#include <linux/jhash.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/slab.h>
=======
 */

#include <linux/export.h>
#include <linux/types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_labels.h>

<<<<<<< HEAD
static unsigned int label_bits(const struct nf_conn_labels *l)
{
	unsigned int longs = l->words;
	return longs * BITS_PER_LONG;
}

bool nf_connlabel_match(const struct nf_conn *ct, u16 bit)
{
	struct nf_conn_labels *labels = nf_ct_labels_find(ct);

	if (!labels)
		return false;

	return bit < label_bits(labels) && test_bit(bit, labels->bits);
}
EXPORT_SYMBOL_GPL(nf_connlabel_match);

int nf_connlabel_set(struct nf_conn *ct, u16 bit)
{
	struct nf_conn_labels *labels = nf_ct_labels_find(ct);

	if (!labels || bit >= label_bits(labels))
		return -ENOSPC;

	if (test_bit(bit, labels->bits))
		return 0;

	if (test_and_set_bit(bit, labels->bits))
		return 0;

	return 0;
}
EXPORT_SYMBOL_GPL(nf_connlabel_set);

static struct nf_ct_ext_type labels_extend __read_mostly = {
	.len    = sizeof(struct nf_conn_labels),
	.align  = __alignof__(struct nf_conn_labels),
	.id     = NF_CT_EXT_LABELS,
};

int nf_conntrack_labels_init(struct net *net)
{
	if (net_eq(net, &init_net))
		return nf_ct_extend_register(&labels_extend);
	return 0;
}

void nf_conntrack_labels_fini(struct net *net)
{
	if (net_eq(net, &init_net))
		nf_ct_extend_unregister(&labels_extend);
}
=======
static int replace_u32(u32 *address, u32 mask, u32 new)
{
	u32 old, tmp;

	do {
		old = *address;
		tmp = (old & mask) ^ new;
		if (old == tmp)
			return 0;
	} while (cmpxchg(address, old, tmp) != old);

	return 1;
}

int nf_connlabels_replace(struct nf_conn *ct,
			  const u32 *data,
			  const u32 *mask, unsigned int words32)
{
	struct nf_conn_labels *labels;
	unsigned int size, i;
	int changed = 0;
	u32 *dst;

	labels = nf_ct_labels_find(ct);
	if (!labels)
		return -ENOSPC;

	size = sizeof(labels->bits);
	if (size < (words32 * sizeof(u32)))
		words32 = size / sizeof(u32);

	dst = (u32 *) labels->bits;
	for (i = 0; i < words32; i++)
		changed |= replace_u32(&dst[i], mask ? ~mask[i] : 0, data[i]);

	size /= sizeof(u32);
	for (i = words32; i < size; i++) /* pad */
		replace_u32(&dst[i], 0, 0);

	if (changed)
		nf_conntrack_event_cache(IPCT_LABEL, ct);
	return 0;
}
EXPORT_SYMBOL_GPL(nf_connlabels_replace);

int nf_connlabels_get(struct net *net, unsigned int bits)
{
	int v;

	if (BIT_WORD(bits) >= NF_CT_LABELS_MAX_SIZE / sizeof(long))
		return -ERANGE;

	BUILD_BUG_ON(NF_CT_LABELS_MAX_SIZE / sizeof(long) >= U8_MAX);

	v = atomic_inc_return_relaxed(&net->ct.labels_used);
	WARN_ON_ONCE(v <= 0);

	return 0;
}
EXPORT_SYMBOL_GPL(nf_connlabels_get);

void nf_connlabels_put(struct net *net)
{
	int v = atomic_dec_return_relaxed(&net->ct.labels_used);

	WARN_ON_ONCE(v < 0);
}
EXPORT_SYMBOL_GPL(nf_connlabels_put);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
