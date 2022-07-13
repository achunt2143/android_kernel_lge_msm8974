<<<<<<< HEAD
#include <linux/types.h>
#include <net/net_namespace.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <linux/netfilter/xt_connlabel.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>

struct nf_conn_labels {
	u8 words;
	unsigned long bits[];
};

static inline struct nf_conn_labels *nf_ct_labels_find(const struct nf_conn *ct)
{
#ifdef CONFIG_NF_CONNTRACK_LABELS
	return nf_ct_ext_find(ct, NF_CT_EXT_LABELS);
=======
/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _NF_CONNTRACK_LABELS_H
#define _NF_CONNTRACK_LABELS_H

#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <linux/types.h>
#include <net/net_namespace.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <uapi/linux/netfilter/xt_connlabel.h>

#define NF_CT_LABELS_MAX_SIZE ((XT_CONNLABEL_MAXBIT + 1) / BITS_PER_BYTE)

struct nf_conn_labels {
	unsigned long bits[NF_CT_LABELS_MAX_SIZE / sizeof(long)];
};

/* Can't use nf_ct_ext_find(), flow dissector cannot use symbols
 * exported by nf_conntrack module.
 */
static inline struct nf_conn_labels *nf_ct_labels_find(const struct nf_conn *ct)
{
#ifdef CONFIG_NF_CONNTRACK_LABELS
	struct nf_ct_ext *ext = ct->ext;

	if (!ext || !__nf_ct_ext_exist(ext, NF_CT_EXT_LABELS))
		return NULL;

	return (void *)ct->ext + ct->ext->offset[NF_CT_EXT_LABELS];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	return NULL;
#endif
}

static inline struct nf_conn_labels *nf_ct_labels_ext_add(struct nf_conn *ct)
{
#ifdef CONFIG_NF_CONNTRACK_LABELS
<<<<<<< HEAD
	struct nf_conn_labels *cl_ext;
	struct net *net = nf_ct_net(ct);
	u8 words;

	words = ACCESS_ONCE(net->ct.label_words);
	if (words == 0 || WARN_ON_ONCE(words > 8))
		return NULL;

	cl_ext = nf_ct_ext_add_length(ct, NF_CT_EXT_LABELS,
				      words * sizeof(long), GFP_ATOMIC);
	if (cl_ext != NULL)
		cl_ext->words = words;

	return cl_ext;
=======
	struct net *net = nf_ct_net(ct);

	if (atomic_read(&net->ct.labels_used) == 0)
		return NULL;

	return nf_ct_ext_add(ct, NF_CT_EXT_LABELS, GFP_ATOMIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	return NULL;
#endif
}

<<<<<<< HEAD
bool nf_connlabel_match(const struct nf_conn *ct, u16 bit);
int nf_connlabel_set(struct nf_conn *ct, u16 bit);

#ifdef CONFIG_NF_CONNTRACK_LABELS
int nf_conntrack_labels_init(struct net *net);
void nf_conntrack_labels_fini(struct net *net);
#else
static inline int nf_conntrack_labels_init(struct net *n) { return 0; }
static inline void nf_conntrack_labels_fini(struct net *net) {}
#endif
=======
int nf_connlabels_replace(struct nf_conn *ct,
			  const u32 *data, const u32 *mask, unsigned int words);

#ifdef CONFIG_NF_CONNTRACK_LABELS
int nf_connlabels_get(struct net *net, unsigned int bit);
void nf_connlabels_put(struct net *net);
#else
static inline int nf_connlabels_get(struct net *net, unsigned int bit) { return 0; }
static inline void nf_connlabels_put(struct net *net) {}
#endif

#endif /* _NF_CONNTRACK_LABELS_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
