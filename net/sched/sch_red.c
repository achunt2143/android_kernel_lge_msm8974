<<<<<<< HEAD
/*
 * net/sched/sch_red.c	Random Early Detection queue.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * net/sched/sch_red.c	Random Early Detection queue.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 * Changes:
 * J Hadi Salim 980914:	computation fixes
 * Alexey Makarenko <makar@phoenix.kharkov.ua> 990814: qave on idle link was calculated incorrectly.
 * J Hadi Salim 980816:  ECN support
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <net/pkt_sched.h>
<<<<<<< HEAD
=======
#include <net/pkt_cls.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/inet_ecn.h>
#include <net/red.h>


/*	Parameters, settable by user:
	-----------------------------

	limit		- bytes (must be > qth_max + burst)

	Hard limit on queue length, should be chosen >qth_max
	to allow packet bursts. This parameter does not
	affect the algorithms behaviour and can be chosen
	arbitrarily high (well, less than ram size)
	Really, this limit will never be reached
	if RED works correctly.
 */

struct red_sched_data {
	u32			limit;		/* HARD maximal queue length */
<<<<<<< HEAD
	unsigned char		flags;
	struct timer_list	adapt_timer;
=======

	unsigned char		flags;
	/* Non-flags in tc_red_qopt.flags. */
	unsigned char		userbits;

	struct timer_list	adapt_timer;
	struct Qdisc		*sch;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct red_parms	parms;
	struct red_vars		vars;
	struct red_stats	stats;
	struct Qdisc		*qdisc;
<<<<<<< HEAD
};

=======
	struct tcf_qevent	qe_early_drop;
	struct tcf_qevent	qe_mark;
};

#define TC_RED_SUPPORTED_FLAGS (TC_RED_HISTORIC_FLAGS | TC_RED_NODROP)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int red_use_ecn(struct red_sched_data *q)
{
	return q->flags & TC_RED_ECN;
}

static inline int red_use_harddrop(struct red_sched_data *q)
{
	return q->flags & TC_RED_HARDDROP;
}

<<<<<<< HEAD
static int red_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct Qdisc *child = q->qdisc;
=======
static int red_use_nodrop(struct red_sched_data *q)
{
	return q->flags & TC_RED_NODROP;
}

static int red_enqueue(struct sk_buff *skb, struct Qdisc *sch,
		       struct sk_buff **to_free)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct Qdisc *child = q->qdisc;
	unsigned int len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	q->vars.qavg = red_calc_qavg(&q->parms,
				     &q->vars,
				     child->qstats.backlog);

	if (red_is_idling(&q->vars))
		red_end_of_idle_period(&q->vars);

	switch (red_action(&q->parms, &q->vars, q->vars.qavg)) {
	case RED_DONT_MARK:
		break;

	case RED_PROB_MARK:
<<<<<<< HEAD
		sch->qstats.overlimits++;
		if (!red_use_ecn(q) || !INET_ECN_set_ce(skb)) {
=======
		qdisc_qstats_overlimit(sch);
		if (!red_use_ecn(q)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			q->stats.prob_drop++;
			goto congestion_drop;
		}

<<<<<<< HEAD
		q->stats.prob_mark++;
		break;

	case RED_HARD_MARK:
		sch->qstats.overlimits++;
		if (red_use_harddrop(q) || !red_use_ecn(q) ||
		    !INET_ECN_set_ce(skb)) {
=======
		if (INET_ECN_set_ce(skb)) {
			q->stats.prob_mark++;
			skb = tcf_qevent_handle(&q->qe_mark, sch, skb, to_free, &ret);
			if (!skb)
				return NET_XMIT_CN | ret;
		} else if (!red_use_nodrop(q)) {
			q->stats.prob_drop++;
			goto congestion_drop;
		}

		/* Non-ECT packet in ECN nodrop mode: queue it. */
		break;

	case RED_HARD_MARK:
		qdisc_qstats_overlimit(sch);
		if (red_use_harddrop(q) || !red_use_ecn(q)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			q->stats.forced_drop++;
			goto congestion_drop;
		}

<<<<<<< HEAD
		q->stats.forced_mark++;
		break;
	}

	ret = qdisc_enqueue(skb, child);
	if (likely(ret == NET_XMIT_SUCCESS)) {
		sch->q.qlen++;
	} else if (net_xmit_drop_count(ret)) {
		q->stats.pdrop++;
		sch->qstats.drops++;
=======
		if (INET_ECN_set_ce(skb)) {
			q->stats.forced_mark++;
			skb = tcf_qevent_handle(&q->qe_mark, sch, skb, to_free, &ret);
			if (!skb)
				return NET_XMIT_CN | ret;
		} else if (!red_use_nodrop(q)) {
			q->stats.forced_drop++;
			goto congestion_drop;
		}

		/* Non-ECT packet in ECN nodrop mode: queue it. */
		break;
	}

	len = qdisc_pkt_len(skb);
	ret = qdisc_enqueue(skb, child, to_free);
	if (likely(ret == NET_XMIT_SUCCESS)) {
		sch->qstats.backlog += len;
		sch->q.qlen++;
	} else if (net_xmit_drop_count(ret)) {
		q->stats.pdrop++;
		qdisc_qstats_drop(sch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return ret;

congestion_drop:
<<<<<<< HEAD
	qdisc_drop(skb, sch);
=======
	skb = tcf_qevent_handle(&q->qe_early_drop, sch, skb, to_free, &ret);
	if (!skb)
		return NET_XMIT_CN | ret;

	qdisc_drop(skb, sch, to_free);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NET_XMIT_CN;
}

static struct sk_buff *red_dequeue(struct Qdisc *sch)
{
	struct sk_buff *skb;
	struct red_sched_data *q = qdisc_priv(sch);
	struct Qdisc *child = q->qdisc;

	skb = child->dequeue(child);
	if (skb) {
		qdisc_bstats_update(sch, skb);
<<<<<<< HEAD
=======
		qdisc_qstats_backlog_dec(sch, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sch->q.qlen--;
	} else {
		if (!red_is_idling(&q->vars))
			red_start_of_idle_period(&q->vars);
	}
	return skb;
}

static struct sk_buff *red_peek(struct Qdisc *sch)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct Qdisc *child = q->qdisc;

	return child->ops->peek(child);
}

<<<<<<< HEAD
static unsigned int red_drop(struct Qdisc *sch)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct Qdisc *child = q->qdisc;
	unsigned int len;

	if (child->ops->drop && (len = child->ops->drop(child)) > 0) {
		q->stats.other++;
		sch->qstats.drops++;
		sch->q.qlen--;
		return len;
	}

	if (!red_is_idling(&q->vars))
		red_start_of_idle_period(&q->vars);

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void red_reset(struct Qdisc *sch)
{
	struct red_sched_data *q = qdisc_priv(sch);

	qdisc_reset(q->qdisc);
<<<<<<< HEAD
	sch->q.qlen = 0;
	red_restart(&q->vars);
}

=======
	red_restart(&q->vars);
}

static int red_offload(struct Qdisc *sch, bool enable)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct net_device *dev = qdisc_dev(sch);
	struct tc_red_qopt_offload opt = {
		.handle = sch->handle,
		.parent = sch->parent,
	};

	if (!tc_can_offload(dev) || !dev->netdev_ops->ndo_setup_tc)
		return -EOPNOTSUPP;

	if (enable) {
		opt.command = TC_RED_REPLACE;
		opt.set.min = q->parms.qth_min >> q->parms.Wlog;
		opt.set.max = q->parms.qth_max >> q->parms.Wlog;
		opt.set.probability = q->parms.max_P;
		opt.set.limit = q->limit;
		opt.set.is_ecn = red_use_ecn(q);
		opt.set.is_harddrop = red_use_harddrop(q);
		opt.set.is_nodrop = red_use_nodrop(q);
		opt.set.qstats = &sch->qstats;
	} else {
		opt.command = TC_RED_DESTROY;
	}

	return dev->netdev_ops->ndo_setup_tc(dev, TC_SETUP_QDISC_RED, &opt);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void red_destroy(struct Qdisc *sch)
{
	struct red_sched_data *q = qdisc_priv(sch);

<<<<<<< HEAD
	del_timer_sync(&q->adapt_timer);
	qdisc_destroy(q->qdisc);
}

static const struct nla_policy red_policy[TCA_RED_MAX + 1] = {
	[TCA_RED_PARMS]	= { .len = sizeof(struct tc_red_qopt) },
	[TCA_RED_STAB]	= { .len = RED_STAB_SIZE },
	[TCA_RED_MAX_P] = { .type = NLA_U32 },
};

static int red_change(struct Qdisc *sch, struct nlattr *opt)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct nlattr *tb[TCA_RED_MAX + 1];
	struct tc_red_qopt *ctl;
	struct Qdisc *child = NULL;
	int err;
	u32 max_P;

	if (opt == NULL)
		return -EINVAL;

	err = nla_parse_nested(tb, TCA_RED_MAX, opt, red_policy);
	if (err < 0)
		return err;
=======
	tcf_qevent_destroy(&q->qe_mark, sch);
	tcf_qevent_destroy(&q->qe_early_drop, sch);
	del_timer_sync(&q->adapt_timer);
	red_offload(sch, false);
	qdisc_put(q->qdisc);
}

static const struct nla_policy red_policy[TCA_RED_MAX + 1] = {
	[TCA_RED_UNSPEC] = { .strict_start_type = TCA_RED_FLAGS },
	[TCA_RED_PARMS]	= { .len = sizeof(struct tc_red_qopt) },
	[TCA_RED_STAB]	= { .len = RED_STAB_SIZE },
	[TCA_RED_MAX_P] = { .type = NLA_U32 },
	[TCA_RED_FLAGS] = NLA_POLICY_BITFIELD32(TC_RED_SUPPORTED_FLAGS),
	[TCA_RED_EARLY_DROP_BLOCK] = { .type = NLA_U32 },
	[TCA_RED_MARK_BLOCK] = { .type = NLA_U32 },
};

static int __red_change(struct Qdisc *sch, struct nlattr **tb,
			struct netlink_ext_ack *extack)
{
	struct Qdisc *old_child = NULL, *child = NULL;
	struct red_sched_data *q = qdisc_priv(sch);
	struct nla_bitfield32 flags_bf;
	struct tc_red_qopt *ctl;
	unsigned char userbits;
	unsigned char flags;
	int err;
	u32 max_P;
	u8 *stab;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tb[TCA_RED_PARMS] == NULL ||
	    tb[TCA_RED_STAB] == NULL)
		return -EINVAL;

	max_P = tb[TCA_RED_MAX_P] ? nla_get_u32(tb[TCA_RED_MAX_P]) : 0;

	ctl = nla_data(tb[TCA_RED_PARMS]);
<<<<<<< HEAD

	if (ctl->limit > 0) {
		child = fifo_create_dflt(sch, &bfifo_qdisc_ops, ctl->limit);
		if (IS_ERR(child))
			return PTR_ERR(child);
	}

	sch_tree_lock(sch);
	q->flags = ctl->flags;
	q->limit = ctl->limit;
	if (child) {
		qdisc_tree_decrease_qlen(q->qdisc, q->qdisc->q.qlen);
		qdisc_destroy(q->qdisc);
=======
	stab = nla_data(tb[TCA_RED_STAB]);
	if (!red_check_params(ctl->qth_min, ctl->qth_max, ctl->Wlog,
			      ctl->Scell_log, stab))
		return -EINVAL;

	err = red_get_flags(ctl->flags, TC_RED_HISTORIC_FLAGS,
			    tb[TCA_RED_FLAGS], TC_RED_SUPPORTED_FLAGS,
			    &flags_bf, &userbits, extack);
	if (err)
		return err;

	if (ctl->limit > 0) {
		child = fifo_create_dflt(sch, &bfifo_qdisc_ops, ctl->limit,
					 extack);
		if (IS_ERR(child))
			return PTR_ERR(child);

		/* child is fifo, no need to check for noop_qdisc */
		qdisc_hash_add(child, true);
	}

	sch_tree_lock(sch);

	flags = (q->flags & ~flags_bf.selector) | flags_bf.value;
	err = red_validate_flags(flags, extack);
	if (err)
		goto unlock_out;

	q->flags = flags;
	q->userbits = userbits;
	q->limit = ctl->limit;
	if (child) {
		qdisc_tree_flush_backlog(q->qdisc);
		old_child = q->qdisc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		q->qdisc = child;
	}

	red_set_parms(&q->parms,
		      ctl->qth_min, ctl->qth_max, ctl->Wlog,
		      ctl->Plog, ctl->Scell_log,
<<<<<<< HEAD
		      nla_data(tb[TCA_RED_STAB]),
=======
		      stab,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		      max_P);
	red_set_vars(&q->vars);

	del_timer(&q->adapt_timer);
	if (ctl->flags & TC_RED_ADAPTATIVE)
		mod_timer(&q->adapt_timer, jiffies + HZ/2);

	if (!q->qdisc->q.qlen)
		red_start_of_idle_period(&q->vars);

	sch_tree_unlock(sch);
<<<<<<< HEAD
	return 0;
}

static inline void red_adaptative_timer(unsigned long arg)
{
	struct Qdisc *sch = (struct Qdisc *)arg;
	struct red_sched_data *q = qdisc_priv(sch);
	spinlock_t *root_lock = qdisc_lock(qdisc_root_sleeping(sch));

=======

	red_offload(sch, true);

	if (old_child)
		qdisc_put(old_child);
	return 0;

unlock_out:
	sch_tree_unlock(sch);
	if (child)
		qdisc_put(child);
	return err;
}

static inline void red_adaptative_timer(struct timer_list *t)
{
	struct red_sched_data *q = from_timer(q, t, adapt_timer);
	struct Qdisc *sch = q->sch;
	spinlock_t *root_lock;

	rcu_read_lock();
	root_lock = qdisc_lock(qdisc_root_sleeping(sch));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(root_lock);
	red_adaptative_algo(&q->parms, &q->vars);
	mod_timer(&q->adapt_timer, jiffies + HZ/2);
	spin_unlock(root_lock);
<<<<<<< HEAD
}

static int red_init(struct Qdisc *sch, struct nlattr *opt)
{
	struct red_sched_data *q = qdisc_priv(sch);

	q->qdisc = &noop_qdisc;
	setup_timer(&q->adapt_timer, red_adaptative_timer, (unsigned long)sch);
	return red_change(sch, opt);
=======
	rcu_read_unlock();
}

static int red_init(struct Qdisc *sch, struct nlattr *opt,
		    struct netlink_ext_ack *extack)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct nlattr *tb[TCA_RED_MAX + 1];
	int err;

	q->qdisc = &noop_qdisc;
	q->sch = sch;
	timer_setup(&q->adapt_timer, red_adaptative_timer, 0);

	if (!opt)
		return -EINVAL;

	err = nla_parse_nested_deprecated(tb, TCA_RED_MAX, opt, red_policy,
					  extack);
	if (err < 0)
		return err;

	err = __red_change(sch, tb, extack);
	if (err)
		return err;

	err = tcf_qevent_init(&q->qe_early_drop, sch,
			      FLOW_BLOCK_BINDER_TYPE_RED_EARLY_DROP,
			      tb[TCA_RED_EARLY_DROP_BLOCK], extack);
	if (err)
		return err;

	return tcf_qevent_init(&q->qe_mark, sch,
			       FLOW_BLOCK_BINDER_TYPE_RED_MARK,
			       tb[TCA_RED_MARK_BLOCK], extack);
}

static int red_change(struct Qdisc *sch, struct nlattr *opt,
		      struct netlink_ext_ack *extack)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct nlattr *tb[TCA_RED_MAX + 1];
	int err;

	err = nla_parse_nested_deprecated(tb, TCA_RED_MAX, opt, red_policy,
					  extack);
	if (err < 0)
		return err;

	err = tcf_qevent_validate_change(&q->qe_early_drop,
					 tb[TCA_RED_EARLY_DROP_BLOCK], extack);
	if (err)
		return err;

	err = tcf_qevent_validate_change(&q->qe_mark,
					 tb[TCA_RED_MARK_BLOCK], extack);
	if (err)
		return err;

	return __red_change(sch, tb, extack);
}

static int red_dump_offload_stats(struct Qdisc *sch)
{
	struct tc_red_qopt_offload hw_stats = {
		.command = TC_RED_STATS,
		.handle = sch->handle,
		.parent = sch->parent,
		{
			.stats.bstats = &sch->bstats,
			.stats.qstats = &sch->qstats,
		},
	};

	return qdisc_offload_dump_helper(sch, TC_SETUP_QDISC_RED, &hw_stats);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int red_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct nlattr *opts = NULL;
	struct tc_red_qopt opt = {
		.limit		= q->limit,
<<<<<<< HEAD
		.flags		= q->flags,
=======
		.flags		= (q->flags & TC_RED_HISTORIC_FLAGS) |
				  q->userbits,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.qth_min	= q->parms.qth_min >> q->parms.Wlog,
		.qth_max	= q->parms.qth_max >> q->parms.Wlog,
		.Wlog		= q->parms.Wlog,
		.Plog		= q->parms.Plog,
		.Scell_log	= q->parms.Scell_log,
	};
<<<<<<< HEAD

	sch->qstats.backlog = q->qdisc->qstats.backlog;
	opts = nla_nest_start(skb, TCA_OPTIONS);
	if (opts == NULL)
		goto nla_put_failure;
	NLA_PUT(skb, TCA_RED_PARMS, sizeof(opt), &opt);
	NLA_PUT_U32(skb, TCA_RED_MAX_P, q->parms.max_P);
=======
	int err;

	err = red_dump_offload_stats(sch);
	if (err)
		goto nla_put_failure;

	opts = nla_nest_start_noflag(skb, TCA_OPTIONS);
	if (opts == NULL)
		goto nla_put_failure;
	if (nla_put(skb, TCA_RED_PARMS, sizeof(opt), &opt) ||
	    nla_put_u32(skb, TCA_RED_MAX_P, q->parms.max_P) ||
	    nla_put_bitfield32(skb, TCA_RED_FLAGS,
			       q->flags, TC_RED_SUPPORTED_FLAGS) ||
	    tcf_qevent_dump(skb, TCA_RED_MARK_BLOCK, &q->qe_mark) ||
	    tcf_qevent_dump(skb, TCA_RED_EARLY_DROP_BLOCK, &q->qe_early_drop))
		goto nla_put_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return nla_nest_end(skb, opts);

nla_put_failure:
	nla_nest_cancel(skb, opts);
	return -EMSGSIZE;
}

static int red_dump_stats(struct Qdisc *sch, struct gnet_dump *d)
{
	struct red_sched_data *q = qdisc_priv(sch);
<<<<<<< HEAD
	struct tc_red_xstats st = {
		.early	= q->stats.prob_drop + q->stats.forced_drop,
		.pdrop	= q->stats.pdrop,
		.other	= q->stats.other,
		.marked	= q->stats.prob_mark + q->stats.forced_mark,
	};
=======
	struct net_device *dev = qdisc_dev(sch);
	struct tc_red_xstats st = {0};

	if (sch->flags & TCQ_F_OFFLOADED) {
		struct tc_red_qopt_offload hw_stats_request = {
			.command = TC_RED_XSTATS,
			.handle = sch->handle,
			.parent = sch->parent,
			{
				.xstats = &q->stats,
			},
		};
		dev->netdev_ops->ndo_setup_tc(dev, TC_SETUP_QDISC_RED,
					      &hw_stats_request);
	}
	st.early = q->stats.prob_drop + q->stats.forced_drop;
	st.pdrop = q->stats.pdrop;
	st.marked = q->stats.prob_mark + q->stats.forced_mark;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return gnet_stats_copy_app(d, &st, sizeof(st));
}

static int red_dump_class(struct Qdisc *sch, unsigned long cl,
			  struct sk_buff *skb, struct tcmsg *tcm)
{
	struct red_sched_data *q = qdisc_priv(sch);

	tcm->tcm_handle |= TC_H_MIN(1);
	tcm->tcm_info = q->qdisc->handle;
	return 0;
}

<<<<<<< HEAD
static int red_graft(struct Qdisc *sch, unsigned long arg, struct Qdisc *new,
		     struct Qdisc **old)
=======
static void red_graft_offload(struct Qdisc *sch,
			      struct Qdisc *new, struct Qdisc *old,
			      struct netlink_ext_ack *extack)
{
	struct tc_red_qopt_offload graft_offload = {
		.handle		= sch->handle,
		.parent		= sch->parent,
		.child_handle	= new->handle,
		.command	= TC_RED_GRAFT,
	};

	qdisc_offload_graft_helper(qdisc_dev(sch), sch, new, old,
				   TC_SETUP_QDISC_RED, &graft_offload, extack);
}

static int red_graft(struct Qdisc *sch, unsigned long arg, struct Qdisc *new,
		     struct Qdisc **old, struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct red_sched_data *q = qdisc_priv(sch);

	if (new == NULL)
		new = &noop_qdisc;

<<<<<<< HEAD
	sch_tree_lock(sch);
	*old = q->qdisc;
	q->qdisc = new;
	qdisc_tree_decrease_qlen(*old, (*old)->q.qlen);
	qdisc_reset(*old);
	sch_tree_unlock(sch);
=======
	*old = qdisc_replace(sch, new, &q->qdisc);

	red_graft_offload(sch, new, *old, extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static struct Qdisc *red_leaf(struct Qdisc *sch, unsigned long arg)
{
	struct red_sched_data *q = qdisc_priv(sch);
	return q->qdisc;
}

<<<<<<< HEAD
static unsigned long red_get(struct Qdisc *sch, u32 classid)
=======
static unsigned long red_find(struct Qdisc *sch, u32 classid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 1;
}

<<<<<<< HEAD
static void red_put(struct Qdisc *sch, unsigned long arg)
{
}

static void red_walk(struct Qdisc *sch, struct qdisc_walker *walker)
{
	if (!walker->stop) {
		if (walker->count >= walker->skip)
			if (walker->fn(sch, 1, walker) < 0) {
				walker->stop = 1;
				return;
			}
		walker->count++;
=======
static void red_walk(struct Qdisc *sch, struct qdisc_walker *walker)
{
	if (!walker->stop) {
		tc_qdisc_stats_dump(sch, 1, walker);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static const struct Qdisc_class_ops red_class_ops = {
	.graft		=	red_graft,
	.leaf		=	red_leaf,
<<<<<<< HEAD
	.get		=	red_get,
	.put		=	red_put,
=======
	.find		=	red_find,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.walk		=	red_walk,
	.dump		=	red_dump_class,
};

static struct Qdisc_ops red_qdisc_ops __read_mostly = {
	.id		=	"red",
	.priv_size	=	sizeof(struct red_sched_data),
	.cl_ops		=	&red_class_ops,
	.enqueue	=	red_enqueue,
	.dequeue	=	red_dequeue,
	.peek		=	red_peek,
<<<<<<< HEAD
	.drop		=	red_drop,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.init		=	red_init,
	.reset		=	red_reset,
	.destroy	=	red_destroy,
	.change		=	red_change,
	.dump		=	red_dump,
	.dump_stats	=	red_dump_stats,
	.owner		=	THIS_MODULE,
};
<<<<<<< HEAD
=======
MODULE_ALIAS_NET_SCH("red");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init red_module_init(void)
{
	return register_qdisc(&red_qdisc_ops);
}

static void __exit red_module_exit(void)
{
	unregister_qdisc(&red_qdisc_ops);
}

module_init(red_module_init)
module_exit(red_module_exit)

MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Random Early Detection qdisc");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
