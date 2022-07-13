<<<<<<< HEAD
/*
 * net/sched/cls_cgroup.c	Control Group Classifier
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * net/sched/cls_cgroup.c	Control Group Classifier
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors:	Thomas Graf <tgraf@suug.ch>
 */

#include <linux/module.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/types.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/cgroup.h>
=======
#include <linux/skbuff.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/rcupdate.h>
#include <net/rtnetlink.h>
#include <net/pkt_cls.h>
#include <net/sock.h>
#include <net/cls_cgroup.h>
<<<<<<< HEAD

static struct cgroup_subsys_state *cgrp_create(struct cgroup *cgrp);
static void cgrp_destroy(struct cgroup *cgrp);
static int cgrp_populate(struct cgroup_subsys *ss, struct cgroup *cgrp);

struct cgroup_subsys net_cls_subsys = {
	.name		= "net_cls",
	.create		= cgrp_create,
	.destroy	= cgrp_destroy,
	.populate	= cgrp_populate,
#ifdef CONFIG_NET_CLS_CGROUP
	.subsys_id	= net_cls_subsys_id,
#endif
	.module		= THIS_MODULE,
};


static inline struct cgroup_cls_state *cgrp_cls_state(struct cgroup *cgrp)
{
	return container_of(cgroup_subsys_state(cgrp, net_cls_subsys_id),
			    struct cgroup_cls_state, css);
}

static inline struct cgroup_cls_state *task_cls_state(struct task_struct *p)
{
	return container_of(task_subsys_state(p, net_cls_subsys_id),
			    struct cgroup_cls_state, css);
}

static struct cgroup_subsys_state *cgrp_create(struct cgroup *cgrp)
{
	struct cgroup_cls_state *cs;

	cs = kzalloc(sizeof(*cs), GFP_KERNEL);
	if (!cs)
		return ERR_PTR(-ENOMEM);

	if (cgrp->parent)
		cs->classid = cgrp_cls_state(cgrp->parent)->classid;

	return &cs->css;
}

static void cgrp_destroy(struct cgroup *cgrp)
{
	kfree(cgrp_cls_state(cgrp));
}

static u64 read_classid(struct cgroup *cgrp, struct cftype *cft)
{
	return cgrp_cls_state(cgrp)->classid;
}

static int write_classid(struct cgroup *cgrp, struct cftype *cft, u64 value)
{
	cgrp_cls_state(cgrp)->classid = (u32) value;
	return 0;
}

static struct cftype ss_files[] = {
	{
		.name = "classid",
		.read_u64 = read_classid,
		.write_u64 = write_classid,
	},
};

static int cgrp_populate(struct cgroup_subsys *ss, struct cgroup *cgrp)
{
	return cgroup_add_files(cgrp, ss, ss_files, ARRAY_SIZE(ss_files));
}
=======
#include <net/tc_wrapper.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct cls_cgroup_head {
	u32			handle;
	struct tcf_exts		exts;
	struct tcf_ematch_tree	ematches;
<<<<<<< HEAD
};

static int cls_cgroup_classify(struct sk_buff *skb, const struct tcf_proto *tp,
			       struct tcf_result *res)
{
	struct cls_cgroup_head *head = tp->root;
	u32 classid;

	rcu_read_lock();
	classid = task_cls_state(current)->classid;
	rcu_read_unlock();

	/*
	 * Due to the nature of the classifier it is required to ignore all
	 * packets originating from softirq context as accessing `current'
	 * would lead to false results.
	 *
	 * This test assumes that all callers of dev_queue_xmit() explicitely
	 * disable bh. Knowing this, it is possible to detect softirq based
	 * calls by looking at the number of nested bh disable calls because
	 * softirqs always disables bh.
	 */
	if (in_serving_softirq()) {
		/* If there is an sk_classid we'll use that. */
		if (!skb->sk)
			return -1;
		classid = skb->sk->sk_classid;
	}

	if (!classid)
		return -1;

=======
	struct tcf_proto	*tp;
	struct rcu_work		rwork;
};

TC_INDIRECT_SCOPE int cls_cgroup_classify(struct sk_buff *skb,
					  const struct tcf_proto *tp,
					  struct tcf_result *res)
{
	struct cls_cgroup_head *head = rcu_dereference_bh(tp->root);
	u32 classid = task_get_classid(skb);

	if (unlikely(!head))
		return -1;
	if (!classid)
		return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!tcf_em_tree_match(skb, &head->ematches, NULL))
		return -1;

	res->classid = classid;
	res->class = 0;
<<<<<<< HEAD
	return tcf_exts_exec(skb, &head->exts, res);
}

static unsigned long cls_cgroup_get(struct tcf_proto *tp, u32 handle)
{
	return 0UL;
}

static void cls_cgroup_put(struct tcf_proto *tp, unsigned long f)
{
=======

	return tcf_exts_exec(skb, &head->exts, res);
}

static void *cls_cgroup_get(struct tcf_proto *tp, u32 handle)
{
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int cls_cgroup_init(struct tcf_proto *tp)
{
	return 0;
}

<<<<<<< HEAD
static const struct tcf_ext_map cgroup_ext_map = {
	.action = TCA_CGROUP_ACT,
	.police = TCA_CGROUP_POLICE,
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct nla_policy cgroup_policy[TCA_CGROUP_MAX + 1] = {
	[TCA_CGROUP_EMATCHES]	= { .type = NLA_NESTED },
};

<<<<<<< HEAD
static int cls_cgroup_change(struct tcf_proto *tp, unsigned long base,
			     u32 handle, struct nlattr **tca,
			     unsigned long *arg)
{
	struct nlattr *tb[TCA_CGROUP_MAX + 1];
	struct cls_cgroup_head *head = tp->root;
	struct tcf_ematch_tree t;
	struct tcf_exts e;
=======
static void __cls_cgroup_destroy(struct cls_cgroup_head *head)
{
	tcf_exts_destroy(&head->exts);
	tcf_em_tree_destroy(&head->ematches);
	tcf_exts_put_net(&head->exts);
	kfree(head);
}

static void cls_cgroup_destroy_work(struct work_struct *work)
{
	struct cls_cgroup_head *head = container_of(to_rcu_work(work),
						    struct cls_cgroup_head,
						    rwork);
	rtnl_lock();
	__cls_cgroup_destroy(head);
	rtnl_unlock();
}

static int cls_cgroup_change(struct net *net, struct sk_buff *in_skb,
			     struct tcf_proto *tp, unsigned long base,
			     u32 handle, struct nlattr **tca,
			     void **arg, u32 flags,
			     struct netlink_ext_ack *extack)
{
	struct nlattr *tb[TCA_CGROUP_MAX + 1];
	struct cls_cgroup_head *head = rtnl_dereference(tp->root);
	struct cls_cgroup_head *new;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	if (!tca[TCA_OPTIONS])
		return -EINVAL;

<<<<<<< HEAD
	if (head == NULL) {
		if (!handle)
			return -EINVAL;

		head = kzalloc(sizeof(*head), GFP_KERNEL);
		if (head == NULL)
			return -ENOBUFS;

		head->handle = handle;

		tcf_tree_lock(tp);
		tp->root = head;
		tcf_tree_unlock(tp);
	}

	if (handle != head->handle)
		return -ENOENT;

	err = nla_parse_nested(tb, TCA_CGROUP_MAX, tca[TCA_OPTIONS],
			       cgroup_policy);
	if (err < 0)
		return err;

	err = tcf_exts_validate(tp, tb, tca[TCA_RATE], &e, &cgroup_ext_map);
	if (err < 0)
		return err;

	err = tcf_em_tree_validate(tp, tb[TCA_CGROUP_EMATCHES], &t);
	if (err < 0)
		return err;

	tcf_exts_change(tp, &head->exts, &e);
	tcf_em_tree_change(tp, &head->ematches, &t);

	return 0;
}

static void cls_cgroup_destroy(struct tcf_proto *tp)
{
	struct cls_cgroup_head *head = tp->root;

	if (head) {
		tcf_exts_destroy(tp, &head->exts);
		tcf_em_tree_destroy(tp, &head->ematches);
		kfree(head);
	}
}

static int cls_cgroup_delete(struct tcf_proto *tp, unsigned long arg)
=======
	if (!head && !handle)
		return -EINVAL;

	if (head && handle != head->handle)
		return -ENOENT;

	new = kzalloc(sizeof(*head), GFP_KERNEL);
	if (!new)
		return -ENOBUFS;

	err = tcf_exts_init(&new->exts, net, TCA_CGROUP_ACT, TCA_CGROUP_POLICE);
	if (err < 0)
		goto errout;
	new->handle = handle;
	new->tp = tp;
	err = nla_parse_nested_deprecated(tb, TCA_CGROUP_MAX,
					  tca[TCA_OPTIONS], cgroup_policy,
					  NULL);
	if (err < 0)
		goto errout;

	err = tcf_exts_validate(net, tp, tb, tca[TCA_RATE], &new->exts, flags,
				extack);
	if (err < 0)
		goto errout;

	err = tcf_em_tree_validate(tp, tb[TCA_CGROUP_EMATCHES], &new->ematches);
	if (err < 0)
		goto errout;

	rcu_assign_pointer(tp->root, new);
	if (head) {
		tcf_exts_get_net(&head->exts);
		tcf_queue_work(&head->rwork, cls_cgroup_destroy_work);
	}
	return 0;
errout:
	tcf_exts_destroy(&new->exts);
	kfree(new);
	return err;
}

static void cls_cgroup_destroy(struct tcf_proto *tp, bool rtnl_held,
			       struct netlink_ext_ack *extack)
{
	struct cls_cgroup_head *head = rtnl_dereference(tp->root);

	/* Head can still be NULL due to cls_cgroup_init(). */
	if (head) {
		if (tcf_exts_get_net(&head->exts))
			tcf_queue_work(&head->rwork, cls_cgroup_destroy_work);
		else
			__cls_cgroup_destroy(head);
	}
}

static int cls_cgroup_delete(struct tcf_proto *tp, void *arg, bool *last,
			     bool rtnl_held, struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return -EOPNOTSUPP;
}

<<<<<<< HEAD
static void cls_cgroup_walk(struct tcf_proto *tp, struct tcf_walker *arg)
{
	struct cls_cgroup_head *head = tp->root;
=======
static void cls_cgroup_walk(struct tcf_proto *tp, struct tcf_walker *arg,
			    bool rtnl_held)
{
	struct cls_cgroup_head *head = rtnl_dereference(tp->root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (arg->count < arg->skip)
		goto skip;

<<<<<<< HEAD
	if (arg->fn(tp, (unsigned long) head, arg) < 0) {
=======
	if (!head)
		return;
	if (arg->fn(tp, head, arg) < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		arg->stop = 1;
		return;
	}
skip:
	arg->count++;
}

<<<<<<< HEAD
static int cls_cgroup_dump(struct tcf_proto *tp, unsigned long fh,
			   struct sk_buff *skb, struct tcmsg *t)
{
	struct cls_cgroup_head *head = tp->root;
	unsigned char *b = skb_tail_pointer(skb);
=======
static int cls_cgroup_dump(struct net *net, struct tcf_proto *tp, void *fh,
			   struct sk_buff *skb, struct tcmsg *t, bool rtnl_held)
{
	struct cls_cgroup_head *head = rtnl_dereference(tp->root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nlattr *nest;

	t->tcm_handle = head->handle;

<<<<<<< HEAD
	nest = nla_nest_start(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	if (tcf_exts_dump(skb, &head->exts, &cgroup_ext_map) < 0 ||
=======
	nest = nla_nest_start_noflag(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	if (tcf_exts_dump(skb, &head->exts) < 0 ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    tcf_em_tree_dump(skb, &head->ematches, TCA_CGROUP_EMATCHES) < 0)
		goto nla_put_failure;

	nla_nest_end(skb, nest);

<<<<<<< HEAD
	if (tcf_exts_dump_stats(skb, &head->exts, &cgroup_ext_map) < 0)
=======
	if (tcf_exts_dump_stats(skb, &head->exts) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto nla_put_failure;

	return skb->len;

nla_put_failure:
<<<<<<< HEAD
	nlmsg_trim(skb, b);
=======
	nla_nest_cancel(skb, nest);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -1;
}

static struct tcf_proto_ops cls_cgroup_ops __read_mostly = {
	.kind		=	"cgroup",
	.init		=	cls_cgroup_init,
	.change		=	cls_cgroup_change,
	.classify	=	cls_cgroup_classify,
	.destroy	=	cls_cgroup_destroy,
	.get		=	cls_cgroup_get,
<<<<<<< HEAD
	.put		=	cls_cgroup_put,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.delete		=	cls_cgroup_delete,
	.walk		=	cls_cgroup_walk,
	.dump		=	cls_cgroup_dump,
	.owner		=	THIS_MODULE,
};
<<<<<<< HEAD

static int __init init_cgroup_cls(void)
{
	int ret;

	ret = cgroup_load_subsys(&net_cls_subsys);
	if (ret)
		goto out;

#ifndef CONFIG_NET_CLS_CGROUP
	/* We can't use rcu_assign_pointer because this is an int. */
	smp_wmb();
	net_cls_subsys_id = net_cls_subsys.subsys_id;
#endif

	ret = register_tcf_proto_ops(&cls_cgroup_ops);
	if (ret)
		cgroup_unload_subsys(&net_cls_subsys);

out:
	return ret;
=======
MODULE_ALIAS_NET_CLS("cgroup");

static int __init init_cgroup_cls(void)
{
	return register_tcf_proto_ops(&cls_cgroup_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit exit_cgroup_cls(void)
{
	unregister_tcf_proto_ops(&cls_cgroup_ops);
<<<<<<< HEAD

#ifndef CONFIG_NET_CLS_CGROUP
	net_cls_subsys_id = -1;
	synchronize_rcu();
#endif

	cgroup_unload_subsys(&net_cls_subsys);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(init_cgroup_cls);
module_exit(exit_cgroup_cls);
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("TC cgroup classifier");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
