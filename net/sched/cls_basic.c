<<<<<<< HEAD
/*
 * net/sched/cls_basic.c	Basic Packet Classifier.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * net/sched/cls_basic.c	Basic Packet Classifier.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors:	Thomas Graf <tgraf@suug.ch>
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>
<<<<<<< HEAD
#include <net/netlink.h>
#include <net/act_api.h>
#include <net/pkt_cls.h>

struct basic_head {
	u32			hgenerator;
	struct list_head	flist;
=======
#include <linux/idr.h>
#include <linux/percpu.h>
#include <net/netlink.h>
#include <net/act_api.h>
#include <net/pkt_cls.h>
#include <net/tc_wrapper.h>

struct basic_head {
	struct list_head	flist;
	struct idr		handle_idr;
	struct rcu_head		rcu;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct basic_filter {
	u32			handle;
	struct tcf_exts		exts;
	struct tcf_ematch_tree	ematches;
	struct tcf_result	res;
<<<<<<< HEAD
	struct list_head	link;
};

static const struct tcf_ext_map basic_ext_map = {
	.action = TCA_BASIC_ACT,
	.police = TCA_BASIC_POLICE
};

static int basic_classify(struct sk_buff *skb, const struct tcf_proto *tp,
			  struct tcf_result *res)
{
	int r;
	struct basic_head *head = (struct basic_head *) tp->root;
	struct basic_filter *f;

	list_for_each_entry(f, &head->flist, link) {
		if (!tcf_em_tree_match(skb, &f->ematches, NULL))
			continue;
=======
	struct tcf_proto	*tp;
	struct list_head	link;
	struct tc_basic_pcnt __percpu *pf;
	struct rcu_work		rwork;
};

TC_INDIRECT_SCOPE int basic_classify(struct sk_buff *skb,
				     const struct tcf_proto *tp,
				     struct tcf_result *res)
{
	int r;
	struct basic_head *head = rcu_dereference_bh(tp->root);
	struct basic_filter *f;

	list_for_each_entry_rcu(f, &head->flist, link) {
		__this_cpu_inc(f->pf->rcnt);
		if (!tcf_em_tree_match(skb, &f->ematches, NULL))
			continue;
		__this_cpu_inc(f->pf->rhit);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*res = f->res;
		r = tcf_exts_exec(skb, &f->exts, res);
		if (r < 0)
			continue;
		return r;
	}
	return -1;
}

<<<<<<< HEAD
static unsigned long basic_get(struct tcf_proto *tp, u32 handle)
{
	unsigned long l = 0UL;
	struct basic_head *head = (struct basic_head *) tp->root;
	struct basic_filter *f;

	if (head == NULL)
		return 0UL;

	list_for_each_entry(f, &head->flist, link)
		if (f->handle == handle)
			l = (unsigned long) f;

	return l;
}

static void basic_put(struct tcf_proto *tp, unsigned long f)
{
=======
static void *basic_get(struct tcf_proto *tp, u32 handle)
{
	struct basic_head *head = rtnl_dereference(tp->root);
	struct basic_filter *f;

	list_for_each_entry(f, &head->flist, link) {
		if (f->handle == handle) {
			return f;
		}
	}

	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int basic_init(struct tcf_proto *tp)
{
	struct basic_head *head;

	head = kzalloc(sizeof(*head), GFP_KERNEL);
	if (head == NULL)
		return -ENOBUFS;
	INIT_LIST_HEAD(&head->flist);
<<<<<<< HEAD
	tp->root = head;
	return 0;
}

static void basic_delete_filter(struct tcf_proto *tp, struct basic_filter *f)
{
	tcf_unbind_filter(tp, &f->res);
	tcf_exts_destroy(tp, &f->exts);
	tcf_em_tree_destroy(tp, &f->ematches);
	kfree(f);
}

static void basic_destroy(struct tcf_proto *tp)
{
	struct basic_head *head = tp->root;
	struct basic_filter *f, *n;

	list_for_each_entry_safe(f, n, &head->flist, link) {
		list_del(&f->link);
		basic_delete_filter(tp, f);
	}
	kfree(head);
}

static int basic_delete(struct tcf_proto *tp, unsigned long arg)
{
	struct basic_head *head = (struct basic_head *) tp->root;
	struct basic_filter *t, *f = (struct basic_filter *) arg;

	list_for_each_entry(t, &head->flist, link)
		if (t == f) {
			tcf_tree_lock(tp);
			list_del(&t->link);
			tcf_tree_unlock(tp);
			basic_delete_filter(tp, t);
			return 0;
		}

	return -ENOENT;
=======
	idr_init(&head->handle_idr);
	rcu_assign_pointer(tp->root, head);
	return 0;
}

static void __basic_delete_filter(struct basic_filter *f)
{
	tcf_exts_destroy(&f->exts);
	tcf_em_tree_destroy(&f->ematches);
	tcf_exts_put_net(&f->exts);
	free_percpu(f->pf);
	kfree(f);
}

static void basic_delete_filter_work(struct work_struct *work)
{
	struct basic_filter *f = container_of(to_rcu_work(work),
					      struct basic_filter,
					      rwork);
	rtnl_lock();
	__basic_delete_filter(f);
	rtnl_unlock();
}

static void basic_destroy(struct tcf_proto *tp, bool rtnl_held,
			  struct netlink_ext_ack *extack)
{
	struct basic_head *head = rtnl_dereference(tp->root);
	struct basic_filter *f, *n;

	list_for_each_entry_safe(f, n, &head->flist, link) {
		list_del_rcu(&f->link);
		tcf_unbind_filter(tp, &f->res);
		idr_remove(&head->handle_idr, f->handle);
		if (tcf_exts_get_net(&f->exts))
			tcf_queue_work(&f->rwork, basic_delete_filter_work);
		else
			__basic_delete_filter(f);
	}
	idr_destroy(&head->handle_idr);
	kfree_rcu(head, rcu);
}

static int basic_delete(struct tcf_proto *tp, void *arg, bool *last,
			bool rtnl_held, struct netlink_ext_ack *extack)
{
	struct basic_head *head = rtnl_dereference(tp->root);
	struct basic_filter *f = arg;

	list_del_rcu(&f->link);
	tcf_unbind_filter(tp, &f->res);
	idr_remove(&head->handle_idr, f->handle);
	tcf_exts_get_net(&f->exts);
	tcf_queue_work(&f->rwork, basic_delete_filter_work);
	*last = list_empty(&head->flist);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct nla_policy basic_policy[TCA_BASIC_MAX + 1] = {
	[TCA_BASIC_CLASSID]	= { .type = NLA_U32 },
	[TCA_BASIC_EMATCHES]	= { .type = NLA_NESTED },
};

<<<<<<< HEAD
static int basic_set_parms(struct tcf_proto *tp, struct basic_filter *f,
			   unsigned long base, struct nlattr **tb,
			   struct nlattr *est)
{
	int err = -EINVAL;
	struct tcf_exts e;
	struct tcf_ematch_tree t;

	err = tcf_exts_validate(tp, tb, est, &e, &basic_ext_map);
	if (err < 0)
		return err;

	err = tcf_em_tree_validate(tp, tb[TCA_BASIC_EMATCHES], &t);
	if (err < 0)
		goto errout;
=======
static int basic_set_parms(struct net *net, struct tcf_proto *tp,
			   struct basic_filter *f, unsigned long base,
			   struct nlattr **tb,
			   struct nlattr *est, u32 flags,
			   struct netlink_ext_ack *extack)
{
	int err;

	err = tcf_exts_validate(net, tp, tb, est, &f->exts, flags, extack);
	if (err < 0)
		return err;

	err = tcf_em_tree_validate(tp, tb[TCA_BASIC_EMATCHES], &f->ematches);
	if (err < 0)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tb[TCA_BASIC_CLASSID]) {
		f->res.classid = nla_get_u32(tb[TCA_BASIC_CLASSID]);
		tcf_bind_filter(tp, &f->res, base);
	}

<<<<<<< HEAD
	tcf_exts_change(tp, &f->exts, &e);
	tcf_em_tree_change(tp, &f->ematches, &t);

	return 0;
errout:
	tcf_exts_destroy(tp, &e);
	return err;
}

static int basic_change(struct tcf_proto *tp, unsigned long base, u32 handle,
			struct nlattr **tca, unsigned long *arg)
{
	int err;
	struct basic_head *head = (struct basic_head *) tp->root;
	struct nlattr *tb[TCA_BASIC_MAX + 1];
	struct basic_filter *f = (struct basic_filter *) *arg;
=======
	f->tp = tp;
	return 0;
}

static int basic_change(struct net *net, struct sk_buff *in_skb,
			struct tcf_proto *tp, unsigned long base, u32 handle,
			struct nlattr **tca, void **arg,
			u32 flags, struct netlink_ext_ack *extack)
{
	int err;
	struct basic_head *head = rtnl_dereference(tp->root);
	struct nlattr *tb[TCA_BASIC_MAX + 1];
	struct basic_filter *fold = (struct basic_filter *) *arg;
	struct basic_filter *fnew;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tca[TCA_OPTIONS] == NULL)
		return -EINVAL;

<<<<<<< HEAD
	err = nla_parse_nested(tb, TCA_BASIC_MAX, tca[TCA_OPTIONS],
			       basic_policy);
	if (err < 0)
		return err;

	if (f != NULL) {
		if (handle && f->handle != handle)
			return -EINVAL;
		return basic_set_parms(tp, f, base, tb, tca[TCA_RATE]);
	}

	err = -ENOBUFS;
	f = kzalloc(sizeof(*f), GFP_KERNEL);
	if (f == NULL)
		goto errout;

	err = -EINVAL;
	if (handle)
		f->handle = handle;
	else {
		unsigned int i = 0x80000000;
		do {
			if (++head->hgenerator == 0x7FFFFFFF)
				head->hgenerator = 1;
		} while (--i > 0 && basic_get(tp, head->hgenerator));

		if (i <= 0) {
			pr_err("Insufficient number of handles\n");
			goto errout;
		}

		f->handle = head->hgenerator;
	}

	err = basic_set_parms(tp, f, base, tb, tca[TCA_RATE]);
	if (err < 0)
		goto errout;

	tcf_tree_lock(tp);
	list_add(&f->link, &head->flist);
	tcf_tree_unlock(tp);
	*arg = (unsigned long) f;

	return 0;
errout:
	if (*arg == 0UL && f)
		kfree(f);

	return err;
}

static void basic_walk(struct tcf_proto *tp, struct tcf_walker *arg)
{
	struct basic_head *head = (struct basic_head *) tp->root;
	struct basic_filter *f;

	list_for_each_entry(f, &head->flist, link) {
		if (arg->count < arg->skip)
			goto skip;

		if (arg->fn(tp, (unsigned long) f, arg) < 0) {
			arg->stop = 1;
			break;
		}
skip:
		arg->count++;
	}
}

static int basic_dump(struct tcf_proto *tp, unsigned long fh,
		      struct sk_buff *skb, struct tcmsg *t)
{
	struct basic_filter *f = (struct basic_filter *) fh;
	struct nlattr *nest;
=======
	err = nla_parse_nested_deprecated(tb, TCA_BASIC_MAX, tca[TCA_OPTIONS],
					  basic_policy, NULL);
	if (err < 0)
		return err;

	if (fold != NULL) {
		if (handle && fold->handle != handle)
			return -EINVAL;
	}

	fnew = kzalloc(sizeof(*fnew), GFP_KERNEL);
	if (!fnew)
		return -ENOBUFS;

	err = tcf_exts_init(&fnew->exts, net, TCA_BASIC_ACT, TCA_BASIC_POLICE);
	if (err < 0)
		goto errout;

	if (!handle) {
		handle = 1;
		err = idr_alloc_u32(&head->handle_idr, fnew, &handle,
				    INT_MAX, GFP_KERNEL);
	} else if (!fold) {
		err = idr_alloc_u32(&head->handle_idr, fnew, &handle,
				    handle, GFP_KERNEL);
	}
	if (err)
		goto errout;
	fnew->handle = handle;
	fnew->pf = alloc_percpu(struct tc_basic_pcnt);
	if (!fnew->pf) {
		err = -ENOMEM;
		goto errout;
	}

	err = basic_set_parms(net, tp, fnew, base, tb, tca[TCA_RATE], flags,
			      extack);
	if (err < 0) {
		if (!fold)
			idr_remove(&head->handle_idr, fnew->handle);
		goto errout;
	}

	*arg = fnew;

	if (fold) {
		idr_replace(&head->handle_idr, fnew, fnew->handle);
		list_replace_rcu(&fold->link, &fnew->link);
		tcf_unbind_filter(tp, &fold->res);
		tcf_exts_get_net(&fold->exts);
		tcf_queue_work(&fold->rwork, basic_delete_filter_work);
	} else {
		list_add_rcu(&fnew->link, &head->flist);
	}

	return 0;
errout:
	free_percpu(fnew->pf);
	tcf_exts_destroy(&fnew->exts);
	kfree(fnew);
	return err;
}

static void basic_walk(struct tcf_proto *tp, struct tcf_walker *arg,
		       bool rtnl_held)
{
	struct basic_head *head = rtnl_dereference(tp->root);
	struct basic_filter *f;

	list_for_each_entry(f, &head->flist, link) {
		if (!tc_cls_stats_dump(tp, arg, f))
			break;
	}
}

static void basic_bind_class(void *fh, u32 classid, unsigned long cl, void *q,
			     unsigned long base)
{
	struct basic_filter *f = fh;

	tc_cls_bind_class(classid, cl, q, &f->res, base);
}

static int basic_dump(struct net *net, struct tcf_proto *tp, void *fh,
		      struct sk_buff *skb, struct tcmsg *t, bool rtnl_held)
{
	struct tc_basic_pcnt gpf = {};
	struct basic_filter *f = fh;
	struct nlattr *nest;
	int cpu;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (f == NULL)
		return skb->len;

	t->tcm_handle = f->handle;

<<<<<<< HEAD
	nest = nla_nest_start(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	if (f->res.classid)
		NLA_PUT_U32(skb, TCA_BASIC_CLASSID, f->res.classid);

	if (tcf_exts_dump(skb, &f->exts, &basic_ext_map) < 0 ||
=======
	nest = nla_nest_start_noflag(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	if (f->res.classid &&
	    nla_put_u32(skb, TCA_BASIC_CLASSID, f->res.classid))
		goto nla_put_failure;

	for_each_possible_cpu(cpu) {
		struct tc_basic_pcnt *pf = per_cpu_ptr(f->pf, cpu);

		gpf.rcnt += pf->rcnt;
		gpf.rhit += pf->rhit;
	}

	if (nla_put_64bit(skb, TCA_BASIC_PCNT,
			  sizeof(struct tc_basic_pcnt),
			  &gpf, TCA_BASIC_PAD))
		goto nla_put_failure;

	if (tcf_exts_dump(skb, &f->exts) < 0 ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    tcf_em_tree_dump(skb, &f->ematches, TCA_BASIC_EMATCHES) < 0)
		goto nla_put_failure;

	nla_nest_end(skb, nest);

<<<<<<< HEAD
	if (tcf_exts_dump_stats(skb, &f->exts, &basic_ext_map) < 0)
=======
	if (tcf_exts_dump_stats(skb, &f->exts) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto nla_put_failure;

	return skb->len;

nla_put_failure:
	nla_nest_cancel(skb, nest);
	return -1;
}

static struct tcf_proto_ops cls_basic_ops __read_mostly = {
	.kind		=	"basic",
	.classify	=	basic_classify,
	.init		=	basic_init,
	.destroy	=	basic_destroy,
	.get		=	basic_get,
<<<<<<< HEAD
	.put		=	basic_put,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.change		=	basic_change,
	.delete		=	basic_delete,
	.walk		=	basic_walk,
	.dump		=	basic_dump,
<<<<<<< HEAD
	.owner		=	THIS_MODULE,
};
=======
	.bind_class	=	basic_bind_class,
	.owner		=	THIS_MODULE,
};
MODULE_ALIAS_NET_CLS("basic");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init init_basic(void)
{
	return register_tcf_proto_ops(&cls_basic_ops);
}

static void __exit exit_basic(void)
{
	unregister_tcf_proto_ops(&cls_basic_ops);
}

module_init(init_basic)
module_exit(exit_basic)
<<<<<<< HEAD
MODULE_LICENSE("GPL");

=======
MODULE_DESCRIPTION("TC basic classifier");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
