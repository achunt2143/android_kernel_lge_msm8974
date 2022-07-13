<<<<<<< HEAD
/*
 * net/sched/cls_u32.c	Ugly (or Universal) 32bit key Packet Classifier.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * net/sched/cls_u32.c	Ugly (or Universal) 32bit key Packet Classifier.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 *	The filters are packed to hash tables of key nodes
 *	with a set of 32bit key/mask pairs at every node.
 *	Nodes reference next level hash tables etc.
 *
 *	This scheme is the best universal classifier I managed to
 *	invent; it is not super-fast, but it is not slow (provided you
 *	program it correctly), and general enough.  And its relative
 *	speed grows as the number of rules becomes larger.
 *
 *	It seems that it represents the best middle point between
 *	speed and manageability both by human and by machine.
 *
 *	It is especially useful for link sharing combined with QoS;
 *	pure RSVP doesn't need such a general approach and can use
 *	much simpler (and faster) schemes, sort of cls_rsvp.c.
 *
<<<<<<< HEAD
 *	JHS: We should remove the CONFIG_NET_CLS_IND from here
 *	eventually when the meta match extension is made available
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	nfmark match added by Catalin(ux aka Dino) BOIE <catab at umbrella.ro>
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>
#include <net/netlink.h>
#include <net/act_api.h>
#include <net/pkt_cls.h>

struct tc_u_knode {
	struct tc_u_knode	*next;
	u32			handle;
	struct tc_u_hnode	*ht_up;
	struct tcf_exts		exts;
#ifdef CONFIG_NET_CLS_IND
	char                     indev[IFNAMSIZ];
#endif
	u8			fshift;
	struct tcf_result	res;
	struct tc_u_hnode	*ht_down;
#ifdef CONFIG_CLS_U32_PERF
	struct tc_u32_pcnt	*pf;
#endif
#ifdef CONFIG_CLS_U32_MARK
	struct tc_u32_mark	mark;
#endif
=======
#include <linux/percpu.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>
#include <linux/bitmap.h>
#include <linux/netdevice.h>
#include <linux/hash.h>
#include <net/netlink.h>
#include <net/act_api.h>
#include <net/pkt_cls.h>
#include <linux/idr.h>
#include <net/tc_wrapper.h>

struct tc_u_knode {
	struct tc_u_knode __rcu	*next;
	u32			handle;
	struct tc_u_hnode __rcu	*ht_up;
	struct tcf_exts		exts;
	int			ifindex;
	u8			fshift;
	struct tcf_result	res;
	struct tc_u_hnode __rcu	*ht_down;
#ifdef CONFIG_CLS_U32_PERF
	struct tc_u32_pcnt __percpu *pf;
#endif
	u32			flags;
	unsigned int		in_hw_count;
#ifdef CONFIG_CLS_U32_MARK
	u32			val;
	u32			mask;
	u32 __percpu		*pcpu_success;
#endif
	struct rcu_work		rwork;
	/* The 'sel' field MUST be the last field in structure to allow for
	 * tc_u32_keys allocated at end of structure.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tc_u32_sel	sel;
};

struct tc_u_hnode {
<<<<<<< HEAD
	struct tc_u_hnode	*next;
	u32			handle;
	u32			prio;
	struct tc_u_common	*tp_c;
	int			refcnt;
	unsigned int		divisor;
	struct tc_u_knode	*ht[1];
};

struct tc_u_common {
	struct tc_u_hnode	*hlist;
	struct Qdisc		*q;
	int			refcnt;
	u32			hgenerator;
};

static const struct tcf_ext_map u32_ext_map = {
	.action = TCA_U32_ACT,
	.police = TCA_U32_POLICE
=======
	struct tc_u_hnode __rcu	*next;
	u32			handle;
	u32			prio;
	refcount_t		refcnt;
	unsigned int		divisor;
	struct idr		handle_idr;
	bool			is_root;
	struct rcu_head		rcu;
	u32			flags;
	/* The 'ht' field MUST be the last field in structure to allow for
	 * more entries allocated at end of structure.
	 */
	struct tc_u_knode __rcu	*ht[];
};

struct tc_u_common {
	struct tc_u_hnode __rcu	*hlist;
	void			*ptr;
	refcount_t		refcnt;
	struct idr		handle_idr;
	struct hlist_node	hnode;
	long			knodes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static inline unsigned int u32_hash_fold(__be32 key,
					 const struct tc_u32_sel *sel,
					 u8 fshift)
{
	unsigned int h = ntohl(key & sel->hmask) >> fshift;

	return h;
}

<<<<<<< HEAD
static int u32_classify(struct sk_buff *skb, const struct tcf_proto *tp, struct tcf_result *res)
=======
TC_INDIRECT_SCOPE int u32_classify(struct sk_buff *skb,
				   const struct tcf_proto *tp,
				   struct tcf_result *res)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct {
		struct tc_u_knode *knode;
		unsigned int	  off;
	} stack[TC_U32_MAXDEPTH];

<<<<<<< HEAD
	struct tc_u_hnode *ht = (struct tc_u_hnode *)tp->root;
=======
	struct tc_u_hnode *ht = rcu_dereference_bh(tp->root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int off = skb_network_offset(skb);
	struct tc_u_knode *n;
	int sdepth = 0;
	int off2 = 0;
	int sel = 0;
#ifdef CONFIG_CLS_U32_PERF
	int j;
#endif
	int i, r;

next_ht:
<<<<<<< HEAD
	n = ht->ht[sel];
=======
	n = rcu_dereference_bh(ht->ht[sel]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

next_knode:
	if (n) {
		struct tc_u32_key *key = n->sel.keys;

#ifdef CONFIG_CLS_U32_PERF
<<<<<<< HEAD
		n->pf->rcnt += 1;
		j = 0;
#endif

#ifdef CONFIG_CLS_U32_MARK
		if ((skb->mark & n->mark.mask) != n->mark.val) {
			n = n->next;
			goto next_knode;
		} else {
			n->mark.success++;
=======
		__this_cpu_inc(n->pf->rcnt);
		j = 0;
#endif

		if (tc_skip_sw(n->flags)) {
			n = rcu_dereference_bh(n->next);
			goto next_knode;
		}

#ifdef CONFIG_CLS_U32_MARK
		if ((skb->mark & n->mask) != n->val) {
			n = rcu_dereference_bh(n->next);
			goto next_knode;
		} else {
			__this_cpu_inc(*n->pcpu_success);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
#endif

		for (i = n->sel.nkeys; i > 0; i--, key++) {
			int toff = off + key->off + (off2 & key->offmask);
			__be32 *data, hdata;

			if (skb_headroom(skb) + toff > INT_MAX)
				goto out;

			data = skb_header_pointer(skb, toff, 4, &hdata);
			if (!data)
				goto out;
			if ((*data ^ key->val) & key->mask) {
<<<<<<< HEAD
				n = n->next;
				goto next_knode;
			}
#ifdef CONFIG_CLS_U32_PERF
			n->pf->kcnts[j] += 1;
			j++;
#endif
		}
		if (n->ht_down == NULL) {
=======
				n = rcu_dereference_bh(n->next);
				goto next_knode;
			}
#ifdef CONFIG_CLS_U32_PERF
			__this_cpu_inc(n->pf->kcnts[j]);
			j++;
#endif
		}

		ht = rcu_dereference_bh(n->ht_down);
		if (!ht) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
check_terminal:
			if (n->sel.flags & TC_U32_TERMINAL) {

				*res = n->res;
<<<<<<< HEAD
#ifdef CONFIG_NET_CLS_IND
				if (!tcf_match_indev(skb, n->indev)) {
					n = n->next;
					goto next_knode;
				}
#endif
#ifdef CONFIG_CLS_U32_PERF
				n->pf->rhit += 1;
#endif
				r = tcf_exts_exec(skb, &n->exts, res);
				if (r < 0) {
					n = n->next;
=======
				if (!tcf_match_indev(skb, n->ifindex)) {
					n = rcu_dereference_bh(n->next);
					goto next_knode;
				}
#ifdef CONFIG_CLS_U32_PERF
				__this_cpu_inc(n->pf->rhit);
#endif
				r = tcf_exts_exec(skb, &n->exts, res);
				if (r < 0) {
					n = rcu_dereference_bh(n->next);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					goto next_knode;
				}

				return r;
			}
<<<<<<< HEAD
			n = n->next;
=======
			n = rcu_dereference_bh(n->next);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto next_knode;
		}

		/* PUSH */
		if (sdepth >= TC_U32_MAXDEPTH)
			goto deadloop;
		stack[sdepth].knode = n;
		stack[sdepth].off = off;
		sdepth++;

<<<<<<< HEAD
		ht = n->ht_down;
=======
		ht = rcu_dereference_bh(n->ht_down);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sel = 0;
		if (ht->divisor) {
			__be32 *data, hdata;

			data = skb_header_pointer(skb, off + n->sel.hoff, 4,
						  &hdata);
			if (!data)
				goto out;
			sel = ht->divisor & u32_hash_fold(*data, &n->sel,
							  n->fshift);
		}
		if (!(n->sel.flags & (TC_U32_VAROFFSET | TC_U32_OFFSET | TC_U32_EAT)))
			goto next_ht;

		if (n->sel.flags & (TC_U32_OFFSET | TC_U32_VAROFFSET)) {
			off2 = n->sel.off + 3;
			if (n->sel.flags & TC_U32_VAROFFSET) {
				__be16 *data, hdata;

				data = skb_header_pointer(skb,
							  off + n->sel.offoff,
							  2, &hdata);
				if (!data)
					goto out;
				off2 += ntohs(n->sel.offmask & *data) >>
					n->sel.offshift;
			}
			off2 &= ~3;
		}
		if (n->sel.flags & TC_U32_EAT) {
			off += off2;
			off2 = 0;
		}

		if (off < skb->len)
			goto next_ht;
	}

	/* POP */
	if (sdepth--) {
		n = stack[sdepth].knode;
<<<<<<< HEAD
		ht = n->ht_up;
=======
		ht = rcu_dereference_bh(n->ht_up);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		off = stack[sdepth].off;
		goto check_terminal;
	}
out:
	return -1;

deadloop:
<<<<<<< HEAD
	if (net_ratelimit())
		pr_warning("cls_u32: dead loop\n");
	return -1;
}

static struct tc_u_hnode *
u32_lookup_ht(struct tc_u_common *tp_c, u32 handle)
{
	struct tc_u_hnode *ht;

	for (ht = tp_c->hlist; ht; ht = ht->next)
=======
	net_warn_ratelimited("cls_u32: dead loop\n");
	return -1;
}

static struct tc_u_hnode *u32_lookup_ht(struct tc_u_common *tp_c, u32 handle)
{
	struct tc_u_hnode *ht;

	for (ht = rtnl_dereference(tp_c->hlist);
	     ht;
	     ht = rtnl_dereference(ht->next))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ht->handle == handle)
			break;

	return ht;
}

<<<<<<< HEAD
static struct tc_u_knode *
u32_lookup_key(struct tc_u_hnode *ht, u32 handle)
=======
static struct tc_u_knode *u32_lookup_key(struct tc_u_hnode *ht, u32 handle)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int sel;
	struct tc_u_knode *n = NULL;

	sel = TC_U32_HASH(handle);
	if (sel > ht->divisor)
		goto out;

<<<<<<< HEAD
	for (n = ht->ht[sel]; n; n = n->next)
=======
	for (n = rtnl_dereference(ht->ht[sel]);
	     n;
	     n = rtnl_dereference(n->next))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (n->handle == handle)
			break;
out:
	return n;
}


<<<<<<< HEAD
static unsigned long u32_get(struct tcf_proto *tp, u32 handle)
=======
static void *u32_get(struct tcf_proto *tp, u32 handle)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tc_u_hnode *ht;
	struct tc_u_common *tp_c = tp->data;

	if (TC_U32_HTID(handle) == TC_U32_ROOT)
<<<<<<< HEAD
		ht = tp->root;
=======
		ht = rtnl_dereference(tp->root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		ht = u32_lookup_ht(tp_c, TC_U32_HTID(handle));

	if (!ht)
<<<<<<< HEAD
		return 0;

	if (TC_U32_KEY(handle) == 0)
		return (unsigned long)ht;

	return (unsigned long)u32_lookup_key(ht, handle);
}

static void u32_put(struct tcf_proto *tp, unsigned long f)
{
}

static u32 gen_new_htid(struct tc_u_common *tp_c)
{
	int i = 0x800;

	do {
		if (++tp_c->hgenerator == 0x7FF)
			tp_c->hgenerator = 1;
	} while (--i > 0 && u32_lookup_ht(tp_c, (tp_c->hgenerator|0x800)<<20));

	return i > 0 ? (tp_c->hgenerator|0x800)<<20 : 0;
=======
		return NULL;

	if (TC_U32_KEY(handle) == 0)
		return ht;

	return u32_lookup_key(ht, handle);
}

/* Protected by rtnl lock */
static u32 gen_new_htid(struct tc_u_common *tp_c, struct tc_u_hnode *ptr)
{
	int id = idr_alloc_cyclic(&tp_c->handle_idr, ptr, 1, 0x7FF, GFP_KERNEL);
	if (id < 0)
		return 0;
	return (id | 0x800U) << 20;
}

static struct hlist_head *tc_u_common_hash;

#define U32_HASH_SHIFT 10
#define U32_HASH_SIZE (1 << U32_HASH_SHIFT)

static void *tc_u_common_ptr(const struct tcf_proto *tp)
{
	struct tcf_block *block = tp->chain->block;

	/* The block sharing is currently supported only
	 * for classless qdiscs. In that case we use block
	 * for tc_u_common identification. In case the
	 * block is not shared, block->q is a valid pointer
	 * and we can use that. That works for classful qdiscs.
	 */
	if (tcf_block_shared(block))
		return block;
	else
		return block->q;
}

static struct hlist_head *tc_u_hash(void *key)
{
	return tc_u_common_hash + hash_ptr(key, U32_HASH_SHIFT);
}

static struct tc_u_common *tc_u_common_find(void *key)
{
	struct tc_u_common *tc;
	hlist_for_each_entry(tc, tc_u_hash(key), hnode) {
		if (tc->ptr == key)
			return tc;
	}
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int u32_init(struct tcf_proto *tp)
{
	struct tc_u_hnode *root_ht;
<<<<<<< HEAD
	struct tc_u_common *tp_c;

	tp_c = tp->q->u32_node;

	root_ht = kzalloc(sizeof(*root_ht), GFP_KERNEL);
	if (root_ht == NULL)
		return -ENOBUFS;

	root_ht->divisor = 0;
	root_ht->refcnt++;
	root_ht->handle = tp_c ? gen_new_htid(tp_c) : 0x80000000;
	root_ht->prio = tp->prio;
=======
	void *key = tc_u_common_ptr(tp);
	struct tc_u_common *tp_c = tc_u_common_find(key);

	root_ht = kzalloc(struct_size(root_ht, ht, 1), GFP_KERNEL);
	if (root_ht == NULL)
		return -ENOBUFS;

	refcount_set(&root_ht->refcnt, 1);
	root_ht->handle = tp_c ? gen_new_htid(tp_c, root_ht) : 0x80000000;
	root_ht->prio = tp->prio;
	root_ht->is_root = true;
	idr_init(&root_ht->handle_idr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tp_c == NULL) {
		tp_c = kzalloc(sizeof(*tp_c), GFP_KERNEL);
		if (tp_c == NULL) {
			kfree(root_ht);
			return -ENOBUFS;
		}
<<<<<<< HEAD
		tp_c->q = tp->q;
		tp->q->u32_node = tp_c;
	}

	tp_c->refcnt++;
	root_ht->next = tp_c->hlist;
	tp_c->hlist = root_ht;
	root_ht->tp_c = tp_c;

	tp->root = root_ht;
=======
		refcount_set(&tp_c->refcnt, 1);
		tp_c->ptr = key;
		INIT_HLIST_NODE(&tp_c->hnode);
		idr_init(&tp_c->handle_idr);

		hlist_add_head(&tp_c->hnode, tc_u_hash(key));
	} else {
		refcount_inc(&tp_c->refcnt);
	}

	RCU_INIT_POINTER(root_ht->next, tp_c->hlist);
	rcu_assign_pointer(tp_c->hlist, root_ht);

	/* root_ht must be destroyed when tcf_proto is destroyed */
	rcu_assign_pointer(tp->root, root_ht);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tp->data = tp_c;
	return 0;
}

<<<<<<< HEAD
static int u32_destroy_key(struct tcf_proto *tp, struct tc_u_knode *n)
{
	tcf_unbind_filter(tp, &n->res);
	tcf_exts_destroy(tp, &n->exts);
	if (n->ht_down)
		n->ht_down->refcnt--;
#ifdef CONFIG_CLS_U32_PERF
	kfree(n->pf);
#endif
	kfree(n);
	return 0;
}

static int u32_delete_key(struct tcf_proto *tp, struct tc_u_knode* key)
{
	struct tc_u_knode **kp;
	struct tc_u_hnode *ht = key->ht_up;

	if (ht) {
		for (kp = &ht->ht[TC_U32_HASH(key->handle)]; *kp; kp = &(*kp)->next) {
			if (*kp == key) {
				tcf_tree_lock(tp);
				*kp = key->next;
				tcf_tree_unlock(tp);

				u32_destroy_key(tp, key);
=======
static void __u32_destroy_key(struct tc_u_knode *n)
{
	struct tc_u_hnode *ht = rtnl_dereference(n->ht_down);

	tcf_exts_destroy(&n->exts);
	if (ht && refcount_dec_and_test(&ht->refcnt))
		kfree(ht);
	kfree(n);
}

static void u32_destroy_key(struct tc_u_knode *n, bool free_pf)
{
	tcf_exts_put_net(&n->exts);
#ifdef CONFIG_CLS_U32_PERF
	if (free_pf)
		free_percpu(n->pf);
#endif
#ifdef CONFIG_CLS_U32_MARK
	if (free_pf)
		free_percpu(n->pcpu_success);
#endif
	__u32_destroy_key(n);
}

/* u32_delete_key_rcu should be called when free'ing a copied
 * version of a tc_u_knode obtained from u32_init_knode(). When
 * copies are obtained from u32_init_knode() the statistics are
 * shared between the old and new copies to allow readers to
 * continue to update the statistics during the copy. To support
 * this the u32_delete_key_rcu variant does not free the percpu
 * statistics.
 */
static void u32_delete_key_work(struct work_struct *work)
{
	struct tc_u_knode *key = container_of(to_rcu_work(work),
					      struct tc_u_knode,
					      rwork);
	rtnl_lock();
	u32_destroy_key(key, false);
	rtnl_unlock();
}

/* u32_delete_key_freepf_rcu is the rcu callback variant
 * that free's the entire structure including the statistics
 * percpu variables. Only use this if the key is not a copy
 * returned by u32_init_knode(). See u32_delete_key_rcu()
 * for the variant that should be used with keys return from
 * u32_init_knode()
 */
static void u32_delete_key_freepf_work(struct work_struct *work)
{
	struct tc_u_knode *key = container_of(to_rcu_work(work),
					      struct tc_u_knode,
					      rwork);
	rtnl_lock();
	u32_destroy_key(key, true);
	rtnl_unlock();
}

static int u32_delete_key(struct tcf_proto *tp, struct tc_u_knode *key)
{
	struct tc_u_common *tp_c = tp->data;
	struct tc_u_knode __rcu **kp;
	struct tc_u_knode *pkp;
	struct tc_u_hnode *ht = rtnl_dereference(key->ht_up);

	if (ht) {
		kp = &ht->ht[TC_U32_HASH(key->handle)];
		for (pkp = rtnl_dereference(*kp); pkp;
		     kp = &pkp->next, pkp = rtnl_dereference(*kp)) {
			if (pkp == key) {
				RCU_INIT_POINTER(*kp, key->next);
				tp_c->knodes--;

				tcf_unbind_filter(tp, &key->res);
				idr_remove(&ht->handle_idr, key->handle);
				tcf_exts_get_net(&key->exts);
				tcf_queue_work(&key->rwork, u32_delete_key_freepf_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return 0;
			}
		}
	}
	WARN_ON(1);
	return 0;
}

<<<<<<< HEAD
static void u32_clear_hnode(struct tcf_proto *tp, struct tc_u_hnode *ht)
{
=======
static void u32_clear_hw_hnode(struct tcf_proto *tp, struct tc_u_hnode *h,
			       struct netlink_ext_ack *extack)
{
	struct tcf_block *block = tp->chain->block;
	struct tc_cls_u32_offload cls_u32 = {};

	tc_cls_common_offload_init(&cls_u32.common, tp, h->flags, extack);
	cls_u32.command = TC_CLSU32_DELETE_HNODE;
	cls_u32.hnode.divisor = h->divisor;
	cls_u32.hnode.handle = h->handle;
	cls_u32.hnode.prio = h->prio;

	tc_setup_cb_call(block, TC_SETUP_CLSU32, &cls_u32, false, true);
}

static int u32_replace_hw_hnode(struct tcf_proto *tp, struct tc_u_hnode *h,
				u32 flags, struct netlink_ext_ack *extack)
{
	struct tcf_block *block = tp->chain->block;
	struct tc_cls_u32_offload cls_u32 = {};
	bool skip_sw = tc_skip_sw(flags);
	bool offloaded = false;
	int err;

	tc_cls_common_offload_init(&cls_u32.common, tp, flags, extack);
	cls_u32.command = TC_CLSU32_NEW_HNODE;
	cls_u32.hnode.divisor = h->divisor;
	cls_u32.hnode.handle = h->handle;
	cls_u32.hnode.prio = h->prio;

	err = tc_setup_cb_call(block, TC_SETUP_CLSU32, &cls_u32, skip_sw, true);
	if (err < 0) {
		u32_clear_hw_hnode(tp, h, NULL);
		return err;
	} else if (err > 0) {
		offloaded = true;
	}

	if (skip_sw && !offloaded)
		return -EINVAL;

	return 0;
}

static void u32_remove_hw_knode(struct tcf_proto *tp, struct tc_u_knode *n,
				struct netlink_ext_ack *extack)
{
	struct tcf_block *block = tp->chain->block;
	struct tc_cls_u32_offload cls_u32 = {};

	tc_cls_common_offload_init(&cls_u32.common, tp, n->flags, extack);
	cls_u32.command = TC_CLSU32_DELETE_KNODE;
	cls_u32.knode.handle = n->handle;

	tc_setup_cb_destroy(block, tp, TC_SETUP_CLSU32, &cls_u32, false,
			    &n->flags, &n->in_hw_count, true);
}

static int u32_replace_hw_knode(struct tcf_proto *tp, struct tc_u_knode *n,
				u32 flags, struct netlink_ext_ack *extack)
{
	struct tc_u_hnode *ht = rtnl_dereference(n->ht_down);
	struct tcf_block *block = tp->chain->block;
	struct tc_cls_u32_offload cls_u32 = {};
	bool skip_sw = tc_skip_sw(flags);
	int err;

	tc_cls_common_offload_init(&cls_u32.common, tp, flags, extack);
	cls_u32.command = TC_CLSU32_REPLACE_KNODE;
	cls_u32.knode.handle = n->handle;
	cls_u32.knode.fshift = n->fshift;
#ifdef CONFIG_CLS_U32_MARK
	cls_u32.knode.val = n->val;
	cls_u32.knode.mask = n->mask;
#else
	cls_u32.knode.val = 0;
	cls_u32.knode.mask = 0;
#endif
	cls_u32.knode.sel = &n->sel;
	cls_u32.knode.res = &n->res;
	cls_u32.knode.exts = &n->exts;
	if (n->ht_down)
		cls_u32.knode.link_handle = ht->handle;

	err = tc_setup_cb_add(block, tp, TC_SETUP_CLSU32, &cls_u32, skip_sw,
			      &n->flags, &n->in_hw_count, true);
	if (err) {
		u32_remove_hw_knode(tp, n, NULL);
		return err;
	}

	if (skip_sw && !(n->flags & TCA_CLS_FLAGS_IN_HW))
		return -EINVAL;

	return 0;
}

static void u32_clear_hnode(struct tcf_proto *tp, struct tc_u_hnode *ht,
			    struct netlink_ext_ack *extack)
{
	struct tc_u_common *tp_c = tp->data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tc_u_knode *n;
	unsigned int h;

	for (h = 0; h <= ht->divisor; h++) {
<<<<<<< HEAD
		while ((n = ht->ht[h]) != NULL) {
			ht->ht[h] = n->next;

			u32_destroy_key(tp, n);
=======
		while ((n = rtnl_dereference(ht->ht[h])) != NULL) {
			RCU_INIT_POINTER(ht->ht[h],
					 rtnl_dereference(n->next));
			tp_c->knodes--;
			tcf_unbind_filter(tp, &n->res);
			u32_remove_hw_knode(tp, n, extack);
			idr_remove(&ht->handle_idr, n->handle);
			if (tcf_exts_get_net(&n->exts))
				tcf_queue_work(&n->rwork, u32_delete_key_freepf_work);
			else
				u32_destroy_key(n, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

<<<<<<< HEAD
static int u32_destroy_hnode(struct tcf_proto *tp, struct tc_u_hnode *ht)
{
	struct tc_u_common *tp_c = tp->data;
	struct tc_u_hnode **hn;

	WARN_ON(ht->refcnt);

	u32_clear_hnode(tp, ht);

	for (hn = &tp_c->hlist; *hn; hn = &(*hn)->next) {
		if (*hn == ht) {
			*hn = ht->next;
			kfree(ht);
=======
static int u32_destroy_hnode(struct tcf_proto *tp, struct tc_u_hnode *ht,
			     struct netlink_ext_ack *extack)
{
	struct tc_u_common *tp_c = tp->data;
	struct tc_u_hnode __rcu **hn;
	struct tc_u_hnode *phn;

	u32_clear_hnode(tp, ht, extack);

	hn = &tp_c->hlist;
	for (phn = rtnl_dereference(*hn);
	     phn;
	     hn = &phn->next, phn = rtnl_dereference(*hn)) {
		if (phn == ht) {
			u32_clear_hw_hnode(tp, ht, extack);
			idr_destroy(&ht->handle_idr);
			idr_remove(&tp_c->handle_idr, ht->handle);
			RCU_INIT_POINTER(*hn, ht->next);
			kfree_rcu(ht, rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		}
	}

<<<<<<< HEAD
	WARN_ON(1);
	return -ENOENT;
}

static void u32_destroy(struct tcf_proto *tp)
{
	struct tc_u_common *tp_c = tp->data;
	struct tc_u_hnode *root_ht = tp->root;

	WARN_ON(root_ht == NULL);

	if (root_ht && --root_ht->refcnt == 0)
		u32_destroy_hnode(tp, root_ht);

	if (--tp_c->refcnt == 0) {
		struct tc_u_hnode *ht;

		tp->q->u32_node = NULL;

		for (ht = tp_c->hlist; ht; ht = ht->next) {
			ht->refcnt--;
			u32_clear_hnode(tp, ht);
		}

		while ((ht = tp_c->hlist) != NULL) {
			tp_c->hlist = ht->next;

			WARN_ON(ht->refcnt != 0);

			kfree(ht);
		}

=======
	return -ENOENT;
}

static void u32_destroy(struct tcf_proto *tp, bool rtnl_held,
			struct netlink_ext_ack *extack)
{
	struct tc_u_common *tp_c = tp->data;
	struct tc_u_hnode *root_ht = rtnl_dereference(tp->root);

	WARN_ON(root_ht == NULL);

	if (root_ht && refcount_dec_and_test(&root_ht->refcnt))
		u32_destroy_hnode(tp, root_ht, extack);

	if (refcount_dec_and_test(&tp_c->refcnt)) {
		struct tc_u_hnode *ht;

		hlist_del(&tp_c->hnode);

		while ((ht = rtnl_dereference(tp_c->hlist)) != NULL) {
			u32_clear_hnode(tp, ht, extack);
			RCU_INIT_POINTER(tp_c->hlist, ht->next);

			/* u32_destroy_key() will later free ht for us, if it's
			 * still referenced by some knode
			 */
			if (refcount_dec_and_test(&ht->refcnt))
				kfree_rcu(ht, rcu);
		}

		idr_destroy(&tp_c->handle_idr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(tp_c);
	}

	tp->data = NULL;
}

<<<<<<< HEAD
static int u32_delete(struct tcf_proto *tp, unsigned long arg)
{
	struct tc_u_hnode *ht = (struct tc_u_hnode *)arg;

	if (ht == NULL)
		return 0;

	if (TC_U32_KEY(ht->handle))
		return u32_delete_key(tp, (struct tc_u_knode *)ht);

	if (tp->root == ht)
		return -EINVAL;

	if (ht->refcnt == 1) {
		ht->refcnt--;
		u32_destroy_hnode(tp, ht);
	} else {
		return -EBUSY;
	}

	return 0;
}

static u32 gen_new_kid(struct tc_u_hnode *ht, u32 handle)
{
	struct tc_u_knode *n;
	unsigned int i = 0x7FF;

	for (n = ht->ht[TC_U32_HASH(handle)]; n; n = n->next)
		if (i < TC_U32_NODE(n->handle))
			i = TC_U32_NODE(n->handle);
	i++;

	return handle | (i > 0xFFF ? 0xFFF : i);
=======
static int u32_delete(struct tcf_proto *tp, void *arg, bool *last,
		      bool rtnl_held, struct netlink_ext_ack *extack)
{
	struct tc_u_hnode *ht = arg;
	struct tc_u_common *tp_c = tp->data;
	int ret = 0;

	if (TC_U32_KEY(ht->handle)) {
		u32_remove_hw_knode(tp, (struct tc_u_knode *)ht, extack);
		ret = u32_delete_key(tp, (struct tc_u_knode *)ht);
		goto out;
	}

	if (ht->is_root) {
		NL_SET_ERR_MSG_MOD(extack, "Not allowed to delete root node");
		return -EINVAL;
	}

	if (refcount_dec_if_one(&ht->refcnt)) {
		u32_destroy_hnode(tp, ht, extack);
	} else {
		NL_SET_ERR_MSG_MOD(extack, "Can not delete in-use filter");
		return -EBUSY;
	}

out:
	*last = refcount_read(&tp_c->refcnt) == 1 && tp_c->knodes == 0;
	return ret;
}

static u32 gen_new_kid(struct tc_u_hnode *ht, u32 htid)
{
	u32 index = htid | 0x800;
	u32 max = htid | 0xFFF;

	if (idr_alloc_u32(&ht->handle_idr, NULL, &index, max, GFP_KERNEL)) {
		index = htid + 1;
		if (idr_alloc_u32(&ht->handle_idr, NULL, &index, max,
				 GFP_KERNEL))
			index = max;
	}

	return index;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct nla_policy u32_policy[TCA_U32_MAX + 1] = {
	[TCA_U32_CLASSID]	= { .type = NLA_U32 },
	[TCA_U32_HASH]		= { .type = NLA_U32 },
	[TCA_U32_LINK]		= { .type = NLA_U32 },
	[TCA_U32_DIVISOR]	= { .type = NLA_U32 },
	[TCA_U32_SEL]		= { .len = sizeof(struct tc_u32_sel) },
	[TCA_U32_INDEV]		= { .type = NLA_STRING, .len = IFNAMSIZ },
	[TCA_U32_MARK]		= { .len = sizeof(struct tc_u32_mark) },
<<<<<<< HEAD
};

static int u32_set_parms(struct tcf_proto *tp, unsigned long base,
			 struct tc_u_hnode *ht,
			 struct tc_u_knode *n, struct nlattr **tb,
			 struct nlattr *est)
{
	int err;
	struct tcf_exts e;

	err = tcf_exts_validate(tp, tb, est, &e, &u32_ext_map);
	if (err < 0)
		return err;

	err = -EINVAL;
	if (tb[TCA_U32_LINK]) {
		u32 handle = nla_get_u32(tb[TCA_U32_LINK]);
		struct tc_u_hnode *ht_down = NULL, *ht_old;

		if (TC_U32_KEY(handle))
			goto errout;

		if (handle) {
			ht_down = u32_lookup_ht(ht->tp_c, handle);

			if (ht_down == NULL)
				goto errout;
			ht_down->refcnt++;
		}

		tcf_tree_lock(tp);
		ht_old = n->ht_down;
		n->ht_down = ht_down;
		tcf_tree_unlock(tp);

		if (ht_old)
			ht_old->refcnt--;
	}
=======
	[TCA_U32_FLAGS]		= { .type = NLA_U32 },
};

static void u32_unbind_filter(struct tcf_proto *tp, struct tc_u_knode *n,
			      struct nlattr **tb)
{
	if (tb[TCA_U32_CLASSID])
		tcf_unbind_filter(tp, &n->res);
}

static void u32_bind_filter(struct tcf_proto *tp, struct tc_u_knode *n,
			    unsigned long base, struct nlattr **tb)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (tb[TCA_U32_CLASSID]) {
		n->res.classid = nla_get_u32(tb[TCA_U32_CLASSID]);
		tcf_bind_filter(tp, &n->res, base);
	}
<<<<<<< HEAD

#ifdef CONFIG_NET_CLS_IND
	if (tb[TCA_U32_INDEV]) {
		err = tcf_change_indev(tp, n->indev, tb[TCA_U32_INDEV]);
		if (err < 0)
			goto errout;
	}
#endif
	tcf_exts_change(tp, &n->exts, &e);

	return 0;
errout:
	tcf_exts_destroy(tp, &e);
	return err;
}

static int u32_change(struct tcf_proto *tp, unsigned long base, u32 handle,
		      struct nlattr **tca,
		      unsigned long *arg)
=======
}

static int u32_set_parms(struct net *net, struct tcf_proto *tp,
			 struct tc_u_knode *n, struct nlattr **tb,
			 struct nlattr *est, u32 flags, u32 fl_flags,
			 struct netlink_ext_ack *extack)
{
	int err, ifindex = -1;

	err = tcf_exts_validate_ex(net, tp, tb, est, &n->exts, flags,
				   fl_flags, extack);
	if (err < 0)
		return err;

	if (tb[TCA_U32_INDEV]) {
		ifindex = tcf_change_indev(net, tb[TCA_U32_INDEV], extack);
		if (ifindex < 0)
			return -EINVAL;
	}

	if (tb[TCA_U32_LINK]) {
		u32 handle = nla_get_u32(tb[TCA_U32_LINK]);
		struct tc_u_hnode *ht_down = NULL, *ht_old;

		if (TC_U32_KEY(handle)) {
			NL_SET_ERR_MSG_MOD(extack, "u32 Link handle must be a hash table");
			return -EINVAL;
		}

		if (handle) {
			ht_down = u32_lookup_ht(tp->data, handle);

			if (!ht_down) {
				NL_SET_ERR_MSG_MOD(extack, "Link hash table not found");
				return -EINVAL;
			}
			if (ht_down->is_root) {
				NL_SET_ERR_MSG_MOD(extack, "Not linking to root node");
				return -EINVAL;
			}
			refcount_inc(&ht_down->refcnt);
		}

		ht_old = rtnl_dereference(n->ht_down);
		rcu_assign_pointer(n->ht_down, ht_down);

		if (ht_old)
			refcount_dec(&ht_old->refcnt);
	}

	if (ifindex >= 0)
		n->ifindex = ifindex;

	return 0;
}

static void u32_replace_knode(struct tcf_proto *tp, struct tc_u_common *tp_c,
			      struct tc_u_knode *n)
{
	struct tc_u_knode __rcu **ins;
	struct tc_u_knode *pins;
	struct tc_u_hnode *ht;

	if (TC_U32_HTID(n->handle) == TC_U32_ROOT)
		ht = rtnl_dereference(tp->root);
	else
		ht = u32_lookup_ht(tp_c, TC_U32_HTID(n->handle));

	ins = &ht->ht[TC_U32_HASH(n->handle)];

	/* The node must always exist for it to be replaced if this is not the
	 * case then something went very wrong elsewhere.
	 */
	for (pins = rtnl_dereference(*ins); ;
	     ins = &pins->next, pins = rtnl_dereference(*ins))
		if (pins->handle == n->handle)
			break;

	idr_replace(&ht->handle_idr, n, n->handle);
	RCU_INIT_POINTER(n->next, pins->next);
	rcu_assign_pointer(*ins, n);
}

static struct tc_u_knode *u32_init_knode(struct net *net, struct tcf_proto *tp,
					 struct tc_u_knode *n)
{
	struct tc_u_hnode *ht = rtnl_dereference(n->ht_down);
	struct tc_u32_sel *s = &n->sel;
	struct tc_u_knode *new;

	new = kzalloc(struct_size(new, sel.keys, s->nkeys), GFP_KERNEL);
	if (!new)
		return NULL;

	RCU_INIT_POINTER(new->next, n->next);
	new->handle = n->handle;
	RCU_INIT_POINTER(new->ht_up, n->ht_up);

	new->ifindex = n->ifindex;
	new->fshift = n->fshift;
	new->flags = n->flags;
	RCU_INIT_POINTER(new->ht_down, ht);

#ifdef CONFIG_CLS_U32_PERF
	/* Statistics may be incremented by readers during update
	 * so we must keep them in tact. When the node is later destroyed
	 * a special destroy call must be made to not free the pf memory.
	 */
	new->pf = n->pf;
#endif

#ifdef CONFIG_CLS_U32_MARK
	new->val = n->val;
	new->mask = n->mask;
	/* Similarly success statistics must be moved as pointers */
	new->pcpu_success = n->pcpu_success;
#endif
	memcpy(&new->sel, s, struct_size(s, keys, s->nkeys));

	if (tcf_exts_init(&new->exts, net, TCA_U32_ACT, TCA_U32_POLICE)) {
		kfree(new);
		return NULL;
	}

	/* bump reference count as long as we hold pointer to structure */
	if (ht)
		refcount_inc(&ht->refcnt);

	return new;
}

static int u32_change(struct net *net, struct sk_buff *in_skb,
		      struct tcf_proto *tp, unsigned long base, u32 handle,
		      struct nlattr **tca, void **arg, u32 flags,
		      struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tc_u_common *tp_c = tp->data;
	struct tc_u_hnode *ht;
	struct tc_u_knode *n;
	struct tc_u32_sel *s;
	struct nlattr *opt = tca[TCA_OPTIONS];
	struct nlattr *tb[TCA_U32_MAX + 1];
<<<<<<< HEAD
	u32 htid;
	int err;

	if (opt == NULL)
		return handle ? -EINVAL : 0;

	err = nla_parse_nested(tb, TCA_U32_MAX, opt, u32_policy);
	if (err < 0)
		return err;

	n = (struct tc_u_knode *)*arg;
	if (n) {
		if (TC_U32_KEY(n->handle) == 0)
			return -EINVAL;

		return u32_set_parms(tp, base, n->ht_up, n, tb, tca[TCA_RATE]);
=======
	u32 htid, userflags = 0;
	size_t sel_size;
	int err;

	if (!opt) {
		if (handle) {
			NL_SET_ERR_MSG_MOD(extack, "Filter handle requires options");
			return -EINVAL;
		} else {
			return 0;
		}
	}

	err = nla_parse_nested_deprecated(tb, TCA_U32_MAX, opt, u32_policy,
					  extack);
	if (err < 0)
		return err;

	if (tb[TCA_U32_FLAGS]) {
		userflags = nla_get_u32(tb[TCA_U32_FLAGS]);
		if (!tc_flags_valid(userflags)) {
			NL_SET_ERR_MSG_MOD(extack, "Invalid filter flags");
			return -EINVAL;
		}
	}

	n = *arg;
	if (n) {
		struct tc_u_knode *new;

		if (TC_U32_KEY(n->handle) == 0) {
			NL_SET_ERR_MSG_MOD(extack, "Key node id cannot be zero");
			return -EINVAL;
		}

		if ((n->flags ^ userflags) &
		    ~(TCA_CLS_FLAGS_IN_HW | TCA_CLS_FLAGS_NOT_IN_HW)) {
			NL_SET_ERR_MSG_MOD(extack, "Key node flags do not match passed flags");
			return -EINVAL;
		}

		new = u32_init_knode(net, tp, n);
		if (!new)
			return -ENOMEM;

		err = u32_set_parms(net, tp, new, tb, tca[TCA_RATE],
				    flags, new->flags, extack);

		if (err) {
			__u32_destroy_key(new);
			return err;
		}

		u32_bind_filter(tp, new, base, tb);

		err = u32_replace_hw_knode(tp, new, flags, extack);
		if (err) {
			u32_unbind_filter(tp, new, tb);

			if (tb[TCA_U32_LINK]) {
				struct tc_u_hnode *ht_old;

				ht_old = rtnl_dereference(n->ht_down);
				if (ht_old)
					refcount_inc(&ht_old->refcnt);
			}
			__u32_destroy_key(new);
			return err;
		}

		if (!tc_in_hw(new->flags))
			new->flags |= TCA_CLS_FLAGS_NOT_IN_HW;

		u32_replace_knode(tp, tp_c, new);
		tcf_unbind_filter(tp, &n->res);
		tcf_exts_get_net(&n->exts);
		tcf_queue_work(&n->rwork, u32_delete_key_work);
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (tb[TCA_U32_DIVISOR]) {
		unsigned int divisor = nla_get_u32(tb[TCA_U32_DIVISOR]);

<<<<<<< HEAD
		if (--divisor > 0x100)
			return -EINVAL;
		if (TC_U32_KEY(handle))
			return -EINVAL;
		if (handle == 0) {
			handle = gen_new_htid(tp->data);
			if (handle == 0)
				return -ENOMEM;
		}
		ht = kzalloc(sizeof(*ht) + divisor*sizeof(void *), GFP_KERNEL);
		if (ht == NULL)
			return -ENOBUFS;
		ht->tp_c = tp_c;
		ht->refcnt = 1;
		ht->divisor = divisor;
		ht->handle = handle;
		ht->prio = tp->prio;
		ht->next = tp_c->hlist;
		tp_c->hlist = ht;
		*arg = (unsigned long)ht;
=======
		if (!is_power_of_2(divisor)) {
			NL_SET_ERR_MSG_MOD(extack, "Divisor is not a power of 2");
			return -EINVAL;
		}
		if (divisor-- > 0x100) {
			NL_SET_ERR_MSG_MOD(extack, "Exceeded maximum 256 hash buckets");
			return -EINVAL;
		}
		if (TC_U32_KEY(handle)) {
			NL_SET_ERR_MSG_MOD(extack, "Divisor can only be used on a hash table");
			return -EINVAL;
		}
		ht = kzalloc(struct_size(ht, ht, divisor + 1), GFP_KERNEL);
		if (ht == NULL)
			return -ENOBUFS;
		if (handle == 0) {
			handle = gen_new_htid(tp->data, ht);
			if (handle == 0) {
				kfree(ht);
				return -ENOMEM;
			}
		} else {
			err = idr_alloc_u32(&tp_c->handle_idr, ht, &handle,
					    handle, GFP_KERNEL);
			if (err) {
				kfree(ht);
				return err;
			}
		}
		refcount_set(&ht->refcnt, 1);
		ht->divisor = divisor;
		ht->handle = handle;
		ht->prio = tp->prio;
		idr_init(&ht->handle_idr);
		ht->flags = userflags;

		err = u32_replace_hw_hnode(tp, ht, userflags, extack);
		if (err) {
			idr_remove(&tp_c->handle_idr, handle);
			kfree(ht);
			return err;
		}

		RCU_INIT_POINTER(ht->next, tp_c->hlist);
		rcu_assign_pointer(tp_c->hlist, ht);
		*arg = ht;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	if (tb[TCA_U32_HASH]) {
		htid = nla_get_u32(tb[TCA_U32_HASH]);
		if (TC_U32_HTID(htid) == TC_U32_ROOT) {
<<<<<<< HEAD
			ht = tp->root;
			htid = ht->handle;
		} else {
			ht = u32_lookup_ht(tp->data, TC_U32_HTID(htid));
			if (ht == NULL)
				return -EINVAL;
		}
	} else {
		ht = tp->root;
		htid = ht->handle;
	}

	if (ht->divisor < TC_U32_HASH(htid))
		return -EINVAL;

	if (handle) {
		if (TC_U32_HTID(handle) && TC_U32_HTID(handle^htid))
			return -EINVAL;
		handle = htid | TC_U32_NODE(handle);
	} else
		handle = gen_new_kid(ht, htid);

	if (tb[TCA_U32_SEL] == NULL)
		return -EINVAL;

	s = nla_data(tb[TCA_U32_SEL]);

	n = kzalloc(sizeof(*n) + s->nkeys*sizeof(struct tc_u32_key), GFP_KERNEL);
	if (n == NULL)
		return -ENOBUFS;

#ifdef CONFIG_CLS_U32_PERF
	n->pf = kzalloc(sizeof(struct tc_u32_pcnt) + s->nkeys*sizeof(u64), GFP_KERNEL);
	if (n->pf == NULL) {
		kfree(n);
		return -ENOBUFS;
	}
#endif

	memcpy(&n->sel, s, sizeof(*s) + s->nkeys*sizeof(struct tc_u32_key));
	n->ht_up = ht;
	n->handle = handle;
	n->fshift = s->hmask ? ffs(ntohl(s->hmask)) - 1 : 0;

#ifdef CONFIG_CLS_U32_MARK
=======
			ht = rtnl_dereference(tp->root);
			htid = ht->handle;
		} else {
			ht = u32_lookup_ht(tp->data, TC_U32_HTID(htid));
			if (!ht) {
				NL_SET_ERR_MSG_MOD(extack, "Specified hash table not found");
				return -EINVAL;
			}
		}
	} else {
		ht = rtnl_dereference(tp->root);
		htid = ht->handle;
	}

	if (ht->divisor < TC_U32_HASH(htid)) {
		NL_SET_ERR_MSG_MOD(extack, "Specified hash table buckets exceed configured value");
		return -EINVAL;
	}

	/* At this point, we need to derive the new handle that will be used to
	 * uniquely map the identity of this table match entry. The
	 * identity of the entry that we need to construct is 32 bits made of:
	 *     htid(12b):bucketid(8b):node/entryid(12b)
	 *
	 * At this point _we have the table(ht)_ in which we will insert this
	 * entry. We carry the table's id in variable "htid".
	 * Note that earlier code picked the ht selection either by a) the user
	 * providing the htid specified via TCA_U32_HASH attribute or b) when
	 * no such attribute is passed then the root ht, is default to at ID
	 * 0x[800][00][000]. Rule: the root table has a single bucket with ID 0.
	 * If OTOH the user passed us the htid, they may also pass a bucketid of
	 * choice. 0 is fine. For example a user htid is 0x[600][01][000] it is
	 * indicating hash bucketid of 1. Rule: the entry/node ID _cannot_ be
	 * passed via the htid, so even if it was non-zero it will be ignored.
	 *
	 * We may also have a handle, if the user passed one. The handle also
	 * carries the same addressing of htid(12b):bucketid(8b):node/entryid(12b).
	 * Rule: the bucketid on the handle is ignored even if one was passed;
	 * rather the value on "htid" is always assumed to be the bucketid.
	 */
	if (handle) {
		/* Rule: The htid from handle and tableid from htid must match */
		if (TC_U32_HTID(handle) && TC_U32_HTID(handle ^ htid)) {
			NL_SET_ERR_MSG_MOD(extack, "Handle specified hash table address mismatch");
			return -EINVAL;
		}
		/* Ok, so far we have a valid htid(12b):bucketid(8b) but we
		 * need to finalize the table entry identification with the last
		 * part - the node/entryid(12b)). Rule: Nodeid _cannot be 0_ for
		 * entries. Rule: nodeid of 0 is reserved only for tables(see
		 * earlier code which processes TC_U32_DIVISOR attribute).
		 * Rule: The nodeid can only be derived from the handle (and not
		 * htid).
		 * Rule: if the handle specified zero for the node id example
		 * 0x60000000, then pick a new nodeid from the pool of IDs
		 * this hash table has been allocating from.
		 * If OTOH it is specified (i.e for example the user passed a
		 * handle such as 0x60000123), then we use it generate our final
		 * handle which is used to uniquely identify the match entry.
		 */
		if (!TC_U32_NODE(handle)) {
			handle = gen_new_kid(ht, htid);
		} else {
			handle = htid | TC_U32_NODE(handle);
			err = idr_alloc_u32(&ht->handle_idr, NULL, &handle,
					    handle, GFP_KERNEL);
			if (err)
				return err;
		}
	} else {
		/* The user did not give us a handle; lets just generate one
		 * from the table's pool of nodeids.
		 */
		handle = gen_new_kid(ht, htid);
	}

	if (tb[TCA_U32_SEL] == NULL) {
		NL_SET_ERR_MSG_MOD(extack, "Selector not specified");
		err = -EINVAL;
		goto erridr;
	}

	s = nla_data(tb[TCA_U32_SEL]);
	sel_size = struct_size(s, keys, s->nkeys);
	if (nla_len(tb[TCA_U32_SEL]) < sel_size) {
		err = -EINVAL;
		goto erridr;
	}

	n = kzalloc(struct_size(n, sel.keys, s->nkeys), GFP_KERNEL);
	if (n == NULL) {
		err = -ENOBUFS;
		goto erridr;
	}

#ifdef CONFIG_CLS_U32_PERF
	n->pf = __alloc_percpu(struct_size(n->pf, kcnts, s->nkeys),
			       __alignof__(struct tc_u32_pcnt));
	if (!n->pf) {
		err = -ENOBUFS;
		goto errfree;
	}
#endif

	unsafe_memcpy(&n->sel, s, sel_size,
		      /* A composite flex-array structure destination,
		       * which was correctly sized with struct_size(),
		       * bounds-checked against nla_len(), and allocated
		       * above. */);
	RCU_INIT_POINTER(n->ht_up, ht);
	n->handle = handle;
	n->fshift = s->hmask ? ffs(ntohl(s->hmask)) - 1 : 0;
	n->flags = userflags;

	err = tcf_exts_init(&n->exts, net, TCA_U32_ACT, TCA_U32_POLICE);
	if (err < 0)
		goto errout;

#ifdef CONFIG_CLS_U32_MARK
	n->pcpu_success = alloc_percpu(u32);
	if (!n->pcpu_success) {
		err = -ENOMEM;
		goto errout;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (tb[TCA_U32_MARK]) {
		struct tc_u32_mark *mark;

		mark = nla_data(tb[TCA_U32_MARK]);
<<<<<<< HEAD
		memcpy(&n->mark, mark, sizeof(struct tc_u32_mark));
		n->mark.success = 0;
	}
#endif

	err = u32_set_parms(tp, base, ht, n, tb, tca[TCA_RATE]);
	if (err == 0) {
		struct tc_u_knode **ins;
		for (ins = &ht->ht[TC_U32_HASH(handle)]; *ins; ins = &(*ins)->next)
			if (TC_U32_NODE(handle) < TC_U32_NODE((*ins)->handle))
				break;

		n->next = *ins;
		tcf_tree_lock(tp);
		*ins = n;
		tcf_tree_unlock(tp);

		*arg = (unsigned long)n;
		return 0;
	}
#ifdef CONFIG_CLS_U32_PERF
	kfree(n->pf);
#endif
	kfree(n);
	return err;
}

static void u32_walk(struct tcf_proto *tp, struct tcf_walker *arg)
=======
		n->val = mark->val;
		n->mask = mark->mask;
	}
#endif

	err = u32_set_parms(net, tp, n, tb, tca[TCA_RATE],
			    flags, n->flags, extack);

	u32_bind_filter(tp, n, base, tb);

	if (err == 0) {
		struct tc_u_knode __rcu **ins;
		struct tc_u_knode *pins;

		err = u32_replace_hw_knode(tp, n, flags, extack);
		if (err)
			goto errunbind;

		if (!tc_in_hw(n->flags))
			n->flags |= TCA_CLS_FLAGS_NOT_IN_HW;

		ins = &ht->ht[TC_U32_HASH(handle)];
		for (pins = rtnl_dereference(*ins); pins;
		     ins = &pins->next, pins = rtnl_dereference(*ins))
			if (TC_U32_NODE(handle) < TC_U32_NODE(pins->handle))
				break;

		RCU_INIT_POINTER(n->next, pins);
		rcu_assign_pointer(*ins, n);
		tp_c->knodes++;
		*arg = n;
		return 0;
	}

errunbind:
	u32_unbind_filter(tp, n, tb);

#ifdef CONFIG_CLS_U32_MARK
	free_percpu(n->pcpu_success);
#endif

errout:
	tcf_exts_destroy(&n->exts);
#ifdef CONFIG_CLS_U32_PERF
errfree:
	free_percpu(n->pf);
#endif
	kfree(n);
erridr:
	idr_remove(&ht->handle_idr, handle);
	return err;
}

static void u32_walk(struct tcf_proto *tp, struct tcf_walker *arg,
		     bool rtnl_held)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tc_u_common *tp_c = tp->data;
	struct tc_u_hnode *ht;
	struct tc_u_knode *n;
	unsigned int h;

	if (arg->stop)
		return;

<<<<<<< HEAD
	for (ht = tp_c->hlist; ht; ht = ht->next) {
		if (ht->prio != tp->prio)
			continue;
		if (arg->count >= arg->skip) {
			if (arg->fn(tp, (unsigned long)ht, arg) < 0) {
				arg->stop = 1;
				return;
			}
		}
		arg->count++;
		for (h = 0; h <= ht->divisor; h++) {
			for (n = ht->ht[h]; n; n = n->next) {
				if (arg->count < arg->skip) {
					arg->count++;
					continue;
				}
				if (arg->fn(tp, (unsigned long)n, arg) < 0) {
					arg->stop = 1;
					return;
				}
				arg->count++;
=======
	for (ht = rtnl_dereference(tp_c->hlist);
	     ht;
	     ht = rtnl_dereference(ht->next)) {
		if (ht->prio != tp->prio)
			continue;

		if (!tc_cls_stats_dump(tp, arg, ht))
			return;

		for (h = 0; h <= ht->divisor; h++) {
			for (n = rtnl_dereference(ht->ht[h]);
			     n;
			     n = rtnl_dereference(n->next)) {
				if (!tc_cls_stats_dump(tp, arg, n))
					return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}
}

<<<<<<< HEAD
static int u32_dump(struct tcf_proto *tp, unsigned long fh,
		     struct sk_buff *skb, struct tcmsg *t)
{
	struct tc_u_knode *n = (struct tc_u_knode *)fh;
=======
static int u32_reoffload_hnode(struct tcf_proto *tp, struct tc_u_hnode *ht,
			       bool add, flow_setup_cb_t *cb, void *cb_priv,
			       struct netlink_ext_ack *extack)
{
	struct tc_cls_u32_offload cls_u32 = {};
	int err;

	tc_cls_common_offload_init(&cls_u32.common, tp, ht->flags, extack);
	cls_u32.command = add ? TC_CLSU32_NEW_HNODE : TC_CLSU32_DELETE_HNODE;
	cls_u32.hnode.divisor = ht->divisor;
	cls_u32.hnode.handle = ht->handle;
	cls_u32.hnode.prio = ht->prio;

	err = cb(TC_SETUP_CLSU32, &cls_u32, cb_priv);
	if (err && add && tc_skip_sw(ht->flags))
		return err;

	return 0;
}

static int u32_reoffload_knode(struct tcf_proto *tp, struct tc_u_knode *n,
			       bool add, flow_setup_cb_t *cb, void *cb_priv,
			       struct netlink_ext_ack *extack)
{
	struct tc_u_hnode *ht = rtnl_dereference(n->ht_down);
	struct tcf_block *block = tp->chain->block;
	struct tc_cls_u32_offload cls_u32 = {};

	tc_cls_common_offload_init(&cls_u32.common, tp, n->flags, extack);
	cls_u32.command = add ?
		TC_CLSU32_REPLACE_KNODE : TC_CLSU32_DELETE_KNODE;
	cls_u32.knode.handle = n->handle;

	if (add) {
		cls_u32.knode.fshift = n->fshift;
#ifdef CONFIG_CLS_U32_MARK
		cls_u32.knode.val = n->val;
		cls_u32.knode.mask = n->mask;
#else
		cls_u32.knode.val = 0;
		cls_u32.knode.mask = 0;
#endif
		cls_u32.knode.sel = &n->sel;
		cls_u32.knode.res = &n->res;
		cls_u32.knode.exts = &n->exts;
		if (n->ht_down)
			cls_u32.knode.link_handle = ht->handle;
	}

	return tc_setup_cb_reoffload(block, tp, add, cb, TC_SETUP_CLSU32,
				     &cls_u32, cb_priv, &n->flags,
				     &n->in_hw_count);
}

static int u32_reoffload(struct tcf_proto *tp, bool add, flow_setup_cb_t *cb,
			 void *cb_priv, struct netlink_ext_ack *extack)
{
	struct tc_u_common *tp_c = tp->data;
	struct tc_u_hnode *ht;
	struct tc_u_knode *n;
	unsigned int h;
	int err;

	for (ht = rtnl_dereference(tp_c->hlist);
	     ht;
	     ht = rtnl_dereference(ht->next)) {
		if (ht->prio != tp->prio)
			continue;

		/* When adding filters to a new dev, try to offload the
		 * hashtable first. When removing, do the filters before the
		 * hashtable.
		 */
		if (add && !tc_skip_hw(ht->flags)) {
			err = u32_reoffload_hnode(tp, ht, add, cb, cb_priv,
						  extack);
			if (err)
				return err;
		}

		for (h = 0; h <= ht->divisor; h++) {
			for (n = rtnl_dereference(ht->ht[h]);
			     n;
			     n = rtnl_dereference(n->next)) {
				if (tc_skip_hw(n->flags))
					continue;

				err = u32_reoffload_knode(tp, n, add, cb,
							  cb_priv, extack);
				if (err)
					return err;
			}
		}

		if (!add && !tc_skip_hw(ht->flags))
			u32_reoffload_hnode(tp, ht, add, cb, cb_priv, extack);
	}

	return 0;
}

static void u32_bind_class(void *fh, u32 classid, unsigned long cl, void *q,
			   unsigned long base)
{
	struct tc_u_knode *n = fh;

	tc_cls_bind_class(classid, cl, q, &n->res, base);
}

static int u32_dump(struct net *net, struct tcf_proto *tp, void *fh,
		    struct sk_buff *skb, struct tcmsg *t, bool rtnl_held)
{
	struct tc_u_knode *n = fh;
	struct tc_u_hnode *ht_up, *ht_down;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nlattr *nest;

	if (n == NULL)
		return skb->len;

	t->tcm_handle = n->handle;

<<<<<<< HEAD
	nest = nla_nest_start(skb, TCA_OPTIONS);
=======
	nest = nla_nest_start_noflag(skb, TCA_OPTIONS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (nest == NULL)
		goto nla_put_failure;

	if (TC_U32_KEY(n->handle) == 0) {
<<<<<<< HEAD
		struct tc_u_hnode *ht = (struct tc_u_hnode *)fh;
		u32 divisor = ht->divisor + 1;

		NLA_PUT_U32(skb, TCA_U32_DIVISOR, divisor);
	} else {
		NLA_PUT(skb, TCA_U32_SEL,
			sizeof(n->sel) + n->sel.nkeys*sizeof(struct tc_u32_key),
			&n->sel);
		if (n->ht_up) {
			u32 htid = n->handle & 0xFFFFF000;
			NLA_PUT_U32(skb, TCA_U32_HASH, htid);
		}
		if (n->res.classid)
			NLA_PUT_U32(skb, TCA_U32_CLASSID, n->res.classid);
		if (n->ht_down)
			NLA_PUT_U32(skb, TCA_U32_LINK, n->ht_down->handle);

#ifdef CONFIG_CLS_U32_MARK
		if (n->mark.val || n->mark.mask)
			NLA_PUT(skb, TCA_U32_MARK, sizeof(n->mark), &n->mark);
#endif

		if (tcf_exts_dump(skb, &n->exts, &u32_ext_map) < 0)
			goto nla_put_failure;

#ifdef CONFIG_NET_CLS_IND
		if (strlen(n->indev))
			NLA_PUT_STRING(skb, TCA_U32_INDEV, n->indev);
#endif
#ifdef CONFIG_CLS_U32_PERF
		NLA_PUT(skb, TCA_U32_PCNT,
		sizeof(struct tc_u32_pcnt) + n->sel.nkeys*sizeof(u64),
			n->pf);
=======
		struct tc_u_hnode *ht = fh;
		u32 divisor = ht->divisor + 1;

		if (nla_put_u32(skb, TCA_U32_DIVISOR, divisor))
			goto nla_put_failure;
	} else {
#ifdef CONFIG_CLS_U32_PERF
		struct tc_u32_pcnt *gpf;
		int cpu;
#endif

		if (nla_put(skb, TCA_U32_SEL, struct_size(&n->sel, keys, n->sel.nkeys),
			    &n->sel))
			goto nla_put_failure;

		ht_up = rtnl_dereference(n->ht_up);
		if (ht_up) {
			u32 htid = n->handle & 0xFFFFF000;
			if (nla_put_u32(skb, TCA_U32_HASH, htid))
				goto nla_put_failure;
		}
		if (n->res.classid &&
		    nla_put_u32(skb, TCA_U32_CLASSID, n->res.classid))
			goto nla_put_failure;

		ht_down = rtnl_dereference(n->ht_down);
		if (ht_down &&
		    nla_put_u32(skb, TCA_U32_LINK, ht_down->handle))
			goto nla_put_failure;

		if (n->flags && nla_put_u32(skb, TCA_U32_FLAGS, n->flags))
			goto nla_put_failure;

#ifdef CONFIG_CLS_U32_MARK
		if ((n->val || n->mask)) {
			struct tc_u32_mark mark = {.val = n->val,
						   .mask = n->mask,
						   .success = 0};
			int cpum;

			for_each_possible_cpu(cpum) {
				__u32 cnt = *per_cpu_ptr(n->pcpu_success, cpum);

				mark.success += cnt;
			}

			if (nla_put(skb, TCA_U32_MARK, sizeof(mark), &mark))
				goto nla_put_failure;
		}
#endif

		if (tcf_exts_dump(skb, &n->exts) < 0)
			goto nla_put_failure;

		if (n->ifindex) {
			struct net_device *dev;
			dev = __dev_get_by_index(net, n->ifindex);
			if (dev && nla_put_string(skb, TCA_U32_INDEV, dev->name))
				goto nla_put_failure;
		}
#ifdef CONFIG_CLS_U32_PERF
		gpf = kzalloc(struct_size(gpf, kcnts, n->sel.nkeys), GFP_KERNEL);
		if (!gpf)
			goto nla_put_failure;

		for_each_possible_cpu(cpu) {
			int i;
			struct tc_u32_pcnt *pf = per_cpu_ptr(n->pf, cpu);

			gpf->rcnt += pf->rcnt;
			gpf->rhit += pf->rhit;
			for (i = 0; i < n->sel.nkeys; i++)
				gpf->kcnts[i] += pf->kcnts[i];
		}

		if (nla_put_64bit(skb, TCA_U32_PCNT, struct_size(gpf, kcnts, n->sel.nkeys),
				  gpf, TCA_U32_PAD)) {
			kfree(gpf);
			goto nla_put_failure;
		}
		kfree(gpf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	}

	nla_nest_end(skb, nest);

	if (TC_U32_KEY(n->handle))
<<<<<<< HEAD
		if (tcf_exts_dump_stats(skb, &n->exts, &u32_ext_map) < 0)
=======
		if (tcf_exts_dump_stats(skb, &n->exts) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto nla_put_failure;
	return skb->len;

nla_put_failure:
	nla_nest_cancel(skb, nest);
	return -1;
}

static struct tcf_proto_ops cls_u32_ops __read_mostly = {
	.kind		=	"u32",
	.classify	=	u32_classify,
	.init		=	u32_init,
	.destroy	=	u32_destroy,
	.get		=	u32_get,
<<<<<<< HEAD
	.put		=	u32_put,
	.change		=	u32_change,
	.delete		=	u32_delete,
	.walk		=	u32_walk,
	.dump		=	u32_dump,
	.owner		=	THIS_MODULE,
};

static int __init init_u32(void)
{
=======
	.change		=	u32_change,
	.delete		=	u32_delete,
	.walk		=	u32_walk,
	.reoffload	=	u32_reoffload,
	.dump		=	u32_dump,
	.bind_class	=	u32_bind_class,
	.owner		=	THIS_MODULE,
};
MODULE_ALIAS_NET_CLS("u32");

static int __init init_u32(void)
{
	int i, ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pr_info("u32 classifier\n");
#ifdef CONFIG_CLS_U32_PERF
	pr_info("    Performance counters on\n");
#endif
<<<<<<< HEAD
#ifdef CONFIG_NET_CLS_IND
	pr_info("    input device check on\n");
#endif
#ifdef CONFIG_NET_CLS_ACT
	pr_info("    Actions configured\n");
#endif
	return register_tcf_proto_ops(&cls_u32_ops);
=======
	pr_info("    input device check on\n");
#ifdef CONFIG_NET_CLS_ACT
	pr_info("    Actions configured\n");
#endif
	tc_u_common_hash = kvmalloc_array(U32_HASH_SIZE,
					  sizeof(struct hlist_head),
					  GFP_KERNEL);
	if (!tc_u_common_hash)
		return -ENOMEM;

	for (i = 0; i < U32_HASH_SIZE; i++)
		INIT_HLIST_HEAD(&tc_u_common_hash[i]);

	ret = register_tcf_proto_ops(&cls_u32_ops);
	if (ret)
		kvfree(tc_u_common_hash);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit exit_u32(void)
{
	unregister_tcf_proto_ops(&cls_u32_ops);
<<<<<<< HEAD
=======
	kvfree(tc_u_common_hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(init_u32)
module_exit(exit_u32)
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Universal 32bit based TC Classifier");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
