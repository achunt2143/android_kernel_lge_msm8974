<<<<<<< HEAD
/*
 * net/sched/sch_fifo.c	The simplest FIFO queue.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * net/sched/sch_fifo.c	The simplest FIFO queue.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <net/pkt_sched.h>
<<<<<<< HEAD

/* 1 band FIFO pseudo-"scheduler" */

static int bfifo_enqueue(struct sk_buff *skb, struct Qdisc *sch)
=======
#include <net/pkt_cls.h>

/* 1 band FIFO pseudo-"scheduler" */

static int bfifo_enqueue(struct sk_buff *skb, struct Qdisc *sch,
			 struct sk_buff **to_free)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (likely(sch->qstats.backlog + qdisc_pkt_len(skb) <= sch->limit))
		return qdisc_enqueue_tail(skb, sch);

<<<<<<< HEAD
	return qdisc_reshape_fail(skb, sch);
}

static int pfifo_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	if (likely(skb_queue_len(&sch->q) < sch->limit))
		return qdisc_enqueue_tail(skb, sch);

	return qdisc_reshape_fail(skb, sch);
}

static int pfifo_tail_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	if (likely(skb_queue_len(&sch->q) < sch->limit))
		return qdisc_enqueue_tail(skb, sch);

	/* queue full, remove one skb to fulfill the limit */
	__qdisc_queue_drop_head(sch, &sch->q);
	sch->qstats.drops++;
	qdisc_enqueue_tail(skb, sch);

	return NET_XMIT_CN;
}

static int fifo_init(struct Qdisc *sch, struct nlattr *opt)
=======
	return qdisc_drop(skb, sch, to_free);
}

static int pfifo_enqueue(struct sk_buff *skb, struct Qdisc *sch,
			 struct sk_buff **to_free)
{
	if (likely(sch->q.qlen < sch->limit))
		return qdisc_enqueue_tail(skb, sch);

	return qdisc_drop(skb, sch, to_free);
}

static int pfifo_tail_enqueue(struct sk_buff *skb, struct Qdisc *sch,
			      struct sk_buff **to_free)
{
	unsigned int prev_backlog;

	if (likely(sch->q.qlen < sch->limit))
		return qdisc_enqueue_tail(skb, sch);

	prev_backlog = sch->qstats.backlog;
	/* queue full, remove one skb to fulfill the limit */
	__qdisc_queue_drop_head(sch, &sch->q, to_free);
	qdisc_qstats_drop(sch);
	qdisc_enqueue_tail(skb, sch);

	qdisc_tree_reduce_backlog(sch, 0, prev_backlog - sch->qstats.backlog);
	return NET_XMIT_CN;
}

static void fifo_offload_init(struct Qdisc *sch)
{
	struct net_device *dev = qdisc_dev(sch);
	struct tc_fifo_qopt_offload qopt;

	if (!tc_can_offload(dev) || !dev->netdev_ops->ndo_setup_tc)
		return;

	qopt.command = TC_FIFO_REPLACE;
	qopt.handle = sch->handle;
	qopt.parent = sch->parent;
	dev->netdev_ops->ndo_setup_tc(dev, TC_SETUP_QDISC_FIFO, &qopt);
}

static void fifo_offload_destroy(struct Qdisc *sch)
{
	struct net_device *dev = qdisc_dev(sch);
	struct tc_fifo_qopt_offload qopt;

	if (!tc_can_offload(dev) || !dev->netdev_ops->ndo_setup_tc)
		return;

	qopt.command = TC_FIFO_DESTROY;
	qopt.handle = sch->handle;
	qopt.parent = sch->parent;
	dev->netdev_ops->ndo_setup_tc(dev, TC_SETUP_QDISC_FIFO, &qopt);
}

static int fifo_offload_dump(struct Qdisc *sch)
{
	struct tc_fifo_qopt_offload qopt;

	qopt.command = TC_FIFO_STATS;
	qopt.handle = sch->handle;
	qopt.parent = sch->parent;
	qopt.stats.bstats = &sch->bstats;
	qopt.stats.qstats = &sch->qstats;

	return qdisc_offload_dump_helper(sch, TC_SETUP_QDISC_FIFO, &qopt);
}

static int __fifo_init(struct Qdisc *sch, struct nlattr *opt,
		       struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	bool bypass;
	bool is_bfifo = sch->ops == &bfifo_qdisc_ops;

	if (opt == NULL) {
<<<<<<< HEAD
		u32 limit = qdisc_dev(sch)->tx_queue_len ? : 1;
=======
		u32 limit = qdisc_dev(sch)->tx_queue_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (is_bfifo)
			limit *= psched_mtu(qdisc_dev(sch));

		sch->limit = limit;
	} else {
		struct tc_fifo_qopt *ctl = nla_data(opt);

		if (nla_len(opt) < sizeof(*ctl))
			return -EINVAL;

		sch->limit = ctl->limit;
	}

	if (is_bfifo)
		bypass = sch->limit >= psched_mtu(qdisc_dev(sch));
	else
		bypass = sch->limit >= 1;

	if (bypass)
		sch->flags |= TCQ_F_CAN_BYPASS;
	else
		sch->flags &= ~TCQ_F_CAN_BYPASS;
<<<<<<< HEAD
	return 0;
}

static int fifo_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct tc_fifo_qopt opt = { .limit = sch->limit };

	NLA_PUT(skb, TCA_OPTIONS, sizeof(opt), &opt);
=======

	return 0;
}

static int fifo_init(struct Qdisc *sch, struct nlattr *opt,
		     struct netlink_ext_ack *extack)
{
	int err;

	err = __fifo_init(sch, opt, extack);
	if (err)
		return err;

	fifo_offload_init(sch);
	return 0;
}

static int fifo_hd_init(struct Qdisc *sch, struct nlattr *opt,
			struct netlink_ext_ack *extack)
{
	return __fifo_init(sch, opt, extack);
}

static void fifo_destroy(struct Qdisc *sch)
{
	fifo_offload_destroy(sch);
}

static int __fifo_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct tc_fifo_qopt opt = { .limit = sch->limit };

	if (nla_put(skb, TCA_OPTIONS, sizeof(opt), &opt))
		goto nla_put_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return skb->len;

nla_put_failure:
	return -1;
}

<<<<<<< HEAD
=======
static int fifo_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	int err;

	err = fifo_offload_dump(sch);
	if (err)
		return err;

	return __fifo_dump(sch, skb);
}

static int fifo_hd_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	return __fifo_dump(sch, skb);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct Qdisc_ops pfifo_qdisc_ops __read_mostly = {
	.id		=	"pfifo",
	.priv_size	=	0,
	.enqueue	=	pfifo_enqueue,
	.dequeue	=	qdisc_dequeue_head,
	.peek		=	qdisc_peek_head,
<<<<<<< HEAD
	.drop		=	qdisc_queue_drop,
	.init		=	fifo_init,
=======
	.init		=	fifo_init,
	.destroy	=	fifo_destroy,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.reset		=	qdisc_reset_queue,
	.change		=	fifo_init,
	.dump		=	fifo_dump,
	.owner		=	THIS_MODULE,
};
EXPORT_SYMBOL(pfifo_qdisc_ops);

struct Qdisc_ops bfifo_qdisc_ops __read_mostly = {
	.id		=	"bfifo",
	.priv_size	=	0,
	.enqueue	=	bfifo_enqueue,
	.dequeue	=	qdisc_dequeue_head,
	.peek		=	qdisc_peek_head,
<<<<<<< HEAD
	.drop		=	qdisc_queue_drop,
	.init		=	fifo_init,
=======
	.init		=	fifo_init,
	.destroy	=	fifo_destroy,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.reset		=	qdisc_reset_queue,
	.change		=	fifo_init,
	.dump		=	fifo_dump,
	.owner		=	THIS_MODULE,
};
EXPORT_SYMBOL(bfifo_qdisc_ops);

struct Qdisc_ops pfifo_head_drop_qdisc_ops __read_mostly = {
	.id		=	"pfifo_head_drop",
	.priv_size	=	0,
	.enqueue	=	pfifo_tail_enqueue,
	.dequeue	=	qdisc_dequeue_head,
	.peek		=	qdisc_peek_head,
<<<<<<< HEAD
	.drop		=	qdisc_queue_drop_head,
	.init		=	fifo_init,
	.reset		=	qdisc_reset_queue,
	.change		=	fifo_init,
	.dump		=	fifo_dump,
=======
	.init		=	fifo_hd_init,
	.reset		=	qdisc_reset_queue,
	.change		=	fifo_hd_init,
	.dump		=	fifo_hd_dump,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.owner		=	THIS_MODULE,
};

/* Pass size change message down to embedded FIFO */
int fifo_set_limit(struct Qdisc *q, unsigned int limit)
{
	struct nlattr *nla;
	int ret = -ENOMEM;

	/* Hack to avoid sending change message to non-FIFO */
	if (strncmp(q->ops->id + 1, "fifo", 4) != 0)
		return 0;

<<<<<<< HEAD
=======
	if (!q->ops->change)
		return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nla = kmalloc(nla_attr_size(sizeof(struct tc_fifo_qopt)), GFP_KERNEL);
	if (nla) {
		nla->nla_type = RTM_NEWQDISC;
		nla->nla_len = nla_attr_size(sizeof(struct tc_fifo_qopt));
		((struct tc_fifo_qopt *)nla_data(nla))->limit = limit;

<<<<<<< HEAD
		ret = q->ops->change(q, nla);
=======
		ret = q->ops->change(q, nla, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(nla);
	}
	return ret;
}
EXPORT_SYMBOL(fifo_set_limit);

struct Qdisc *fifo_create_dflt(struct Qdisc *sch, struct Qdisc_ops *ops,
<<<<<<< HEAD
			       unsigned int limit)
=======
			       unsigned int limit,
			       struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct Qdisc *q;
	int err = -ENOMEM;

<<<<<<< HEAD
	q = qdisc_create_dflt(sch->dev_queue, ops, TC_H_MAKE(sch->handle, 1));
	if (q) {
		err = fifo_set_limit(q, limit);
		if (err < 0) {
			qdisc_destroy(q);
=======
	q = qdisc_create_dflt(sch->dev_queue, ops, TC_H_MAKE(sch->handle, 1),
			      extack);
	if (q) {
		err = fifo_set_limit(q, limit);
		if (err < 0) {
			qdisc_put(q);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			q = NULL;
		}
	}

	return q ? : ERR_PTR(err);
}
EXPORT_SYMBOL(fifo_create_dflt);
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Single queue packet and byte based First In First Out(P/BFIFO) scheduler");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
