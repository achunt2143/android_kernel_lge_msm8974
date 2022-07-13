<<<<<<< HEAD
/*
 * Copyright (c) 2008, Intel Corporation.
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
 * Copyright (c) 2008, Intel Corporation.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author: Alexander Duyck <alexander.h.duyck@intel.com>
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>
<<<<<<< HEAD

=======
#include <net/pkt_cls.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct multiq_sched_data {
	u16 bands;
	u16 max_bands;
	u16 curband;
<<<<<<< HEAD
	struct tcf_proto *filter_list;
=======
	struct tcf_proto __rcu *filter_list;
	struct tcf_block *block;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct Qdisc **queues;
};


static struct Qdisc *
multiq_classify(struct sk_buff *skb, struct Qdisc *sch, int *qerr)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	u32 band;
	struct tcf_result res;
<<<<<<< HEAD
	int err;

	*qerr = NET_XMIT_SUCCESS | __NET_XMIT_BYPASS;
	err = tc_classify(skb, q->filter_list, &res);
=======
	struct tcf_proto *fl = rcu_dereference_bh(q->filter_list);
	int err;

	*qerr = NET_XMIT_SUCCESS | __NET_XMIT_BYPASS;
	err = tcf_classify(skb, NULL, fl, &res, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_NET_CLS_ACT
	switch (err) {
	case TC_ACT_STOLEN:
	case TC_ACT_QUEUED:
<<<<<<< HEAD
		*qerr = NET_XMIT_SUCCESS | __NET_XMIT_STOLEN;
=======
	case TC_ACT_TRAP:
		*qerr = NET_XMIT_SUCCESS | __NET_XMIT_STOLEN;
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TC_ACT_SHOT:
		return NULL;
	}
#endif
	band = skb_get_queue_mapping(skb);

	if (band >= q->bands)
		return q->queues[0];

	return q->queues[band];
}

static int
<<<<<<< HEAD
multiq_enqueue(struct sk_buff *skb, struct Qdisc *sch)
=======
multiq_enqueue(struct sk_buff *skb, struct Qdisc *sch,
	       struct sk_buff **to_free)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct Qdisc *qdisc;
	int ret;

	qdisc = multiq_classify(skb, sch, &ret);
#ifdef CONFIG_NET_CLS_ACT
	if (qdisc == NULL) {

		if (ret & __NET_XMIT_BYPASS)
<<<<<<< HEAD
			sch->qstats.drops++;
		kfree_skb(skb);
=======
			qdisc_qstats_drop(sch);
		__qdisc_drop(skb, to_free);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;
	}
#endif

<<<<<<< HEAD
	ret = qdisc_enqueue(skb, qdisc);
=======
	ret = qdisc_enqueue(skb, qdisc, to_free);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret == NET_XMIT_SUCCESS) {
		sch->q.qlen++;
		return NET_XMIT_SUCCESS;
	}
	if (net_xmit_drop_count(ret))
<<<<<<< HEAD
		sch->qstats.drops++;
=======
		qdisc_qstats_drop(sch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static struct sk_buff *multiq_dequeue(struct Qdisc *sch)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	struct Qdisc *qdisc;
	struct sk_buff *skb;
	int band;

	for (band = 0; band < q->bands; band++) {
		/* cycle through bands to ensure fairness */
		q->curband++;
		if (q->curband >= q->bands)
			q->curband = 0;

		/* Check that target subqueue is available before
		 * pulling an skb to avoid head-of-line blocking.
		 */
		if (!netif_xmit_stopped(
		    netdev_get_tx_queue(qdisc_dev(sch), q->curband))) {
			qdisc = q->queues[q->curband];
			skb = qdisc->dequeue(qdisc);
			if (skb) {
				qdisc_bstats_update(sch, skb);
				sch->q.qlen--;
				return skb;
			}
		}
	}
	return NULL;

}

static struct sk_buff *multiq_peek(struct Qdisc *sch)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	unsigned int curband = q->curband;
	struct Qdisc *qdisc;
	struct sk_buff *skb;
	int band;

	for (band = 0; band < q->bands; band++) {
		/* cycle through bands to ensure fairness */
		curband++;
		if (curband >= q->bands)
			curband = 0;

		/* Check that target subqueue is available before
		 * pulling an skb to avoid head-of-line blocking.
		 */
		if (!netif_xmit_stopped(
		    netdev_get_tx_queue(qdisc_dev(sch), curband))) {
			qdisc = q->queues[curband];
			skb = qdisc->ops->peek(qdisc);
			if (skb)
				return skb;
		}
	}
	return NULL;

}

<<<<<<< HEAD
static unsigned int multiq_drop(struct Qdisc *sch)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	int band;
	unsigned int len;
	struct Qdisc *qdisc;

	for (band = q->bands - 1; band >= 0; band--) {
		qdisc = q->queues[band];
		if (qdisc->ops->drop) {
			len = qdisc->ops->drop(qdisc);
			if (len != 0) {
				sch->q.qlen--;
				return len;
			}
		}
	}
	return 0;
}


=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void
multiq_reset(struct Qdisc *sch)
{
	u16 band;
	struct multiq_sched_data *q = qdisc_priv(sch);

	for (band = 0; band < q->bands; band++)
		qdisc_reset(q->queues[band]);
<<<<<<< HEAD
	sch->q.qlen = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	q->curband = 0;
}

static void
multiq_destroy(struct Qdisc *sch)
{
	int band;
	struct multiq_sched_data *q = qdisc_priv(sch);

<<<<<<< HEAD
	tcf_destroy_chain(&q->filter_list);
	for (band = 0; band < q->bands; band++)
		qdisc_destroy(q->queues[band]);
=======
	tcf_block_put(q->block);
	for (band = 0; band < q->bands; band++)
		qdisc_put(q->queues[band]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree(q->queues);
}

<<<<<<< HEAD
static int multiq_tune(struct Qdisc *sch, struct nlattr *opt)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	struct tc_multiq_qopt *qopt;
	int i;
=======
static int multiq_tune(struct Qdisc *sch, struct nlattr *opt,
		       struct netlink_ext_ack *extack)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	struct tc_multiq_qopt *qopt;
	struct Qdisc **removed;
	int i, n_removed = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!netif_is_multiqueue(qdisc_dev(sch)))
		return -EOPNOTSUPP;
	if (nla_len(opt) < sizeof(*qopt))
		return -EINVAL;

	qopt = nla_data(opt);

	qopt->bands = qdisc_dev(sch)->real_num_tx_queues;

<<<<<<< HEAD
=======
	removed = kmalloc(sizeof(*removed) * (q->max_bands - q->bands),
			  GFP_KERNEL);
	if (!removed)
		return -ENOMEM;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sch_tree_lock(sch);
	q->bands = qopt->bands;
	for (i = q->bands; i < q->max_bands; i++) {
		if (q->queues[i] != &noop_qdisc) {
			struct Qdisc *child = q->queues[i];
<<<<<<< HEAD
			q->queues[i] = &noop_qdisc;
			qdisc_tree_decrease_qlen(child, child->q.qlen);
			qdisc_destroy(child);
=======

			q->queues[i] = &noop_qdisc;
			qdisc_purge_queue(child);
			removed[n_removed++] = child;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	sch_tree_unlock(sch);

<<<<<<< HEAD
=======
	for (i = 0; i < n_removed; i++)
		qdisc_put(removed[i]);
	kfree(removed);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < q->bands; i++) {
		if (q->queues[i] == &noop_qdisc) {
			struct Qdisc *child, *old;
			child = qdisc_create_dflt(sch->dev_queue,
						  &pfifo_qdisc_ops,
						  TC_H_MAKE(sch->handle,
<<<<<<< HEAD
							    i + 1));
=======
							    i + 1), extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (child) {
				sch_tree_lock(sch);
				old = q->queues[i];
				q->queues[i] = child;
<<<<<<< HEAD

				if (old != &noop_qdisc) {
					qdisc_tree_decrease_qlen(old,
								 old->q.qlen);
					qdisc_destroy(old);
				}
				sch_tree_unlock(sch);
=======
				if (child != &noop_qdisc)
					qdisc_hash_add(child, true);

				if (old != &noop_qdisc)
					qdisc_purge_queue(old);
				sch_tree_unlock(sch);
				qdisc_put(old);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}
	return 0;
}

<<<<<<< HEAD
static int multiq_init(struct Qdisc *sch, struct nlattr *opt)
=======
static int multiq_init(struct Qdisc *sch, struct nlattr *opt,
		       struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	int i, err;

	q->queues = NULL;

<<<<<<< HEAD
	if (opt == NULL)
		return -EINVAL;

=======
	if (!opt)
		return -EINVAL;

	err = tcf_block_get(&q->block, &q->filter_list, sch, extack);
	if (err)
		return err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	q->max_bands = qdisc_dev(sch)->num_tx_queues;

	q->queues = kcalloc(q->max_bands, sizeof(struct Qdisc *), GFP_KERNEL);
	if (!q->queues)
		return -ENOBUFS;
	for (i = 0; i < q->max_bands; i++)
		q->queues[i] = &noop_qdisc;

<<<<<<< HEAD
	err = multiq_tune(sch, opt);

	if (err)
		kfree(q->queues);

	return err;
=======
	return multiq_tune(sch, opt, extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int multiq_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	unsigned char *b = skb_tail_pointer(skb);
	struct tc_multiq_qopt opt;

	opt.bands = q->bands;
	opt.max_bands = q->max_bands;

<<<<<<< HEAD
	NLA_PUT(skb, TCA_OPTIONS, sizeof(opt), &opt);
=======
	if (nla_put(skb, TCA_OPTIONS, sizeof(opt), &opt))
		goto nla_put_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return skb->len;

nla_put_failure:
	nlmsg_trim(skb, b);
	return -1;
}

static int multiq_graft(struct Qdisc *sch, unsigned long arg, struct Qdisc *new,
<<<<<<< HEAD
		      struct Qdisc **old)
=======
			struct Qdisc **old, struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	unsigned long band = arg - 1;

	if (new == NULL)
		new = &noop_qdisc;

<<<<<<< HEAD
	sch_tree_lock(sch);
	*old = q->queues[band];
	q->queues[band] = new;
	qdisc_tree_decrease_qlen(*old, (*old)->q.qlen);
	qdisc_reset(*old);
	sch_tree_unlock(sch);

=======
	*old = qdisc_replace(sch, new, &q->queues[band]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static struct Qdisc *
multiq_leaf(struct Qdisc *sch, unsigned long arg)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	unsigned long band = arg - 1;

	return q->queues[band];
}

<<<<<<< HEAD
static unsigned long multiq_get(struct Qdisc *sch, u32 classid)
=======
static unsigned long multiq_find(struct Qdisc *sch, u32 classid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	unsigned long band = TC_H_MIN(classid);

	if (band - 1 >= q->bands)
		return 0;
	return band;
}

static unsigned long multiq_bind(struct Qdisc *sch, unsigned long parent,
				 u32 classid)
{
<<<<<<< HEAD
	return multiq_get(sch, classid);
}


static void multiq_put(struct Qdisc *q, unsigned long cl)
=======
	return multiq_find(sch, classid);
}


static void multiq_unbind(struct Qdisc *q, unsigned long cl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

static int multiq_dump_class(struct Qdisc *sch, unsigned long cl,
			     struct sk_buff *skb, struct tcmsg *tcm)
{
	struct multiq_sched_data *q = qdisc_priv(sch);

	tcm->tcm_handle |= TC_H_MIN(cl);
	tcm->tcm_info = q->queues[cl - 1]->handle;
	return 0;
}

static int multiq_dump_class_stats(struct Qdisc *sch, unsigned long cl,
				 struct gnet_dump *d)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	struct Qdisc *cl_q;

	cl_q = q->queues[cl - 1];
<<<<<<< HEAD
	cl_q->qstats.qlen = cl_q->q.qlen;
	if (gnet_stats_copy_basic(d, &cl_q->bstats) < 0 ||
	    gnet_stats_copy_queue(d, &cl_q->qstats) < 0)
=======
	if (gnet_stats_copy_basic(d, cl_q->cpu_bstats, &cl_q->bstats, true) < 0 ||
	    qdisc_qstats_copy(d, cl_q) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;

	return 0;
}

static void multiq_walk(struct Qdisc *sch, struct qdisc_walker *arg)
{
	struct multiq_sched_data *q = qdisc_priv(sch);
	int band;

	if (arg->stop)
		return;

	for (band = 0; band < q->bands; band++) {
<<<<<<< HEAD
		if (arg->count < arg->skip) {
			arg->count++;
			continue;
		}
		if (arg->fn(sch, band + 1, arg) < 0) {
			arg->stop = 1;
			break;
		}
		arg->count++;
	}
}

static struct tcf_proto **multiq_find_tcf(struct Qdisc *sch, unsigned long cl)
=======
		if (!tc_qdisc_stats_dump(sch, band + 1, arg))
			break;
	}
}

static struct tcf_block *multiq_tcf_block(struct Qdisc *sch, unsigned long cl,
					  struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct multiq_sched_data *q = qdisc_priv(sch);

	if (cl)
		return NULL;
<<<<<<< HEAD
	return &q->filter_list;
=======
	return q->block;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct Qdisc_class_ops multiq_class_ops = {
	.graft		=	multiq_graft,
	.leaf		=	multiq_leaf,
<<<<<<< HEAD
	.get		=	multiq_get,
	.put		=	multiq_put,
	.walk		=	multiq_walk,
	.tcf_chain	=	multiq_find_tcf,
	.bind_tcf	=	multiq_bind,
	.unbind_tcf	=	multiq_put,
=======
	.find		=	multiq_find,
	.walk		=	multiq_walk,
	.tcf_block	=	multiq_tcf_block,
	.bind_tcf	=	multiq_bind,
	.unbind_tcf	=	multiq_unbind,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.dump		=	multiq_dump_class,
	.dump_stats	=	multiq_dump_class_stats,
};

static struct Qdisc_ops multiq_qdisc_ops __read_mostly = {
	.next		=	NULL,
	.cl_ops		=	&multiq_class_ops,
	.id		=	"multiq",
	.priv_size	=	sizeof(struct multiq_sched_data),
	.enqueue	=	multiq_enqueue,
	.dequeue	=	multiq_dequeue,
	.peek		=	multiq_peek,
<<<<<<< HEAD
	.drop		=	multiq_drop,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.init		=	multiq_init,
	.reset		=	multiq_reset,
	.destroy	=	multiq_destroy,
	.change		=	multiq_tune,
	.dump		=	multiq_dump,
	.owner		=	THIS_MODULE,
};
<<<<<<< HEAD
=======
MODULE_ALIAS_NET_SCH("multiq");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init multiq_module_init(void)
{
	return register_qdisc(&multiq_qdisc_ops);
}

static void __exit multiq_module_exit(void)
{
	unregister_qdisc(&multiq_qdisc_ops);
}

module_init(multiq_module_init)
module_exit(multiq_module_exit)

MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Multi queue to hardware queue mapping qdisc");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
