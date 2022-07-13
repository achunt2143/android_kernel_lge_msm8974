<<<<<<< HEAD
/*
 * net/sched/cls_fw.c	Classifier mapping ipchains' fwmark to traffic class.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * net/sched/cls_fw.c	Classifier mapping ipchains' fwmark to traffic class.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 * Changes:
 * Karlis Peisenieks <karlis@mt.lv> : 990415 : fw_walk off by one
 * Karlis Peisenieks <karlis@mt.lv> : 990415 : fw_delete killed all the filter (and kernel).
 * Alex <alex@pilotsoft.com> : 2004xxyy: Added Action extension
<<<<<<< HEAD
 *
 * JHS: We should remove the CONFIG_NET_CLS_IND from here
 * eventually when the meta match extension is made available
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <net/netlink.h>
#include <net/act_api.h>
#include <net/pkt_cls.h>
<<<<<<< HEAD

#define HTSIZE (PAGE_SIZE/sizeof(struct fw_filter *))

struct fw_head {
	struct fw_filter *ht[HTSIZE];
	u32 mask;
};

struct fw_filter {
	struct fw_filter	*next;
	u32			id;
	struct tcf_result	res;
#ifdef CONFIG_NET_CLS_IND
	char			indev[IFNAMSIZ];
#endif /* CONFIG_NET_CLS_IND */
	struct tcf_exts		exts;
};

static const struct tcf_ext_map fw_ext_map = {
	.action = TCA_FW_ACT,
	.police = TCA_FW_POLICE
};

static inline int fw_hash(u32 handle)
{
	if (HTSIZE == 4096)
		return ((handle >> 24) & 0xFFF) ^
		       ((handle >> 12) & 0xFFF) ^
		       (handle & 0xFFF);
	else if (HTSIZE == 2048)
		return ((handle >> 22) & 0x7FF) ^
		       ((handle >> 11) & 0x7FF) ^
		       (handle & 0x7FF);
	else if (HTSIZE == 1024)
		return ((handle >> 20) & 0x3FF) ^
		       ((handle >> 10) & 0x3FF) ^
		       (handle & 0x3FF);
	else if (HTSIZE == 512)
		return (handle >> 27) ^
		       ((handle >> 18) & 0x1FF) ^
		       ((handle >> 9) & 0x1FF) ^
		       (handle & 0x1FF);
	else if (HTSIZE == 256) {
		u8 *t = (u8 *) &handle;
		return t[0] ^ t[1] ^ t[2] ^ t[3];
	} else
		return handle & (HTSIZE - 1);
}

static int fw_classify(struct sk_buff *skb, const struct tcf_proto *tp,
			  struct tcf_result *res)
{
	struct fw_head *head = (struct fw_head *)tp->root;
=======
#include <net/sch_generic.h>
#include <net/tc_wrapper.h>

#define HTSIZE 256

struct fw_head {
	u32			mask;
	struct fw_filter __rcu	*ht[HTSIZE];
	struct rcu_head		rcu;
};

struct fw_filter {
	struct fw_filter __rcu	*next;
	u32			id;
	struct tcf_result	res;
	int			ifindex;
	struct tcf_exts		exts;
	struct tcf_proto	*tp;
	struct rcu_work		rwork;
};

static u32 fw_hash(u32 handle)
{
	handle ^= (handle >> 16);
	handle ^= (handle >> 8);
	return handle % HTSIZE;
}

TC_INDIRECT_SCOPE int fw_classify(struct sk_buff *skb,
				  const struct tcf_proto *tp,
				  struct tcf_result *res)
{
	struct fw_head *head = rcu_dereference_bh(tp->root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct fw_filter *f;
	int r;
	u32 id = skb->mark;

	if (head != NULL) {
		id &= head->mask;
<<<<<<< HEAD
		for (f = head->ht[fw_hash(id)]; f; f = f->next) {
			if (f->id == id) {
				*res = f->res;
#ifdef CONFIG_NET_CLS_IND
				if (!tcf_match_indev(skb, f->indev))
					continue;
#endif /* CONFIG_NET_CLS_IND */
=======

		for (f = rcu_dereference_bh(head->ht[fw_hash(id)]); f;
		     f = rcu_dereference_bh(f->next)) {
			if (f->id == id) {
				*res = f->res;
				if (!tcf_match_indev(skb, f->ifindex))
					continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				r = tcf_exts_exec(skb, &f->exts, res);
				if (r < 0)
					continue;

				return r;
			}
		}
	} else {
<<<<<<< HEAD
		/* old method */
		if (id && (TC_H_MAJ(id) == 0 ||
			   !(TC_H_MAJ(id ^ tp->q->handle)))) {
=======
		struct Qdisc *q = tcf_block_q(tp->chain->block);

		/* Old method: classify the packet using its skb mark. */
		if (id && (TC_H_MAJ(id) == 0 ||
			   !(TC_H_MAJ(id ^ q->handle)))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			res->classid = id;
			res->class = 0;
			return 0;
		}
	}

	return -1;
}

<<<<<<< HEAD
static unsigned long fw_get(struct tcf_proto *tp, u32 handle)
{
	struct fw_head *head = (struct fw_head *)tp->root;
	struct fw_filter *f;

	if (head == NULL)
		return 0;

	for (f = head->ht[fw_hash(handle)]; f; f = f->next) {
		if (f->id == handle)
			return (unsigned long)f;
	}
	return 0;
}

static void fw_put(struct tcf_proto *tp, unsigned long f)
{
=======
static void *fw_get(struct tcf_proto *tp, u32 handle)
{
	struct fw_head *head = rtnl_dereference(tp->root);
	struct fw_filter *f;

	if (head == NULL)
		return NULL;

	f = rtnl_dereference(head->ht[fw_hash(handle)]);
	for (; f; f = rtnl_dereference(f->next)) {
		if (f->id == handle)
			return f;
	}
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fw_init(struct tcf_proto *tp)
{
<<<<<<< HEAD
	return 0;
}

static void fw_delete_filter(struct tcf_proto *tp, struct fw_filter *f)
{
	tcf_unbind_filter(tp, &f->res);
	tcf_exts_destroy(tp, &f->exts);
	kfree(f);
}

static void fw_destroy(struct tcf_proto *tp)
{
	struct fw_head *head = tp->root;
=======
	/* We don't allocate fw_head here, because in the old method
	 * we don't need it at all.
	 */
	return 0;
}

static void __fw_delete_filter(struct fw_filter *f)
{
	tcf_exts_destroy(&f->exts);
	tcf_exts_put_net(&f->exts);
	kfree(f);
}

static void fw_delete_filter_work(struct work_struct *work)
{
	struct fw_filter *f = container_of(to_rcu_work(work),
					   struct fw_filter,
					   rwork);
	rtnl_lock();
	__fw_delete_filter(f);
	rtnl_unlock();
}

static void fw_destroy(struct tcf_proto *tp, bool rtnl_held,
		       struct netlink_ext_ack *extack)
{
	struct fw_head *head = rtnl_dereference(tp->root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct fw_filter *f;
	int h;

	if (head == NULL)
		return;

	for (h = 0; h < HTSIZE; h++) {
<<<<<<< HEAD
		while ((f = head->ht[h]) != NULL) {
			head->ht[h] = f->next;
			fw_delete_filter(tp, f);
		}
	}
	kfree(head);
}

static int fw_delete(struct tcf_proto *tp, unsigned long arg)
{
	struct fw_head *head = (struct fw_head *)tp->root;
	struct fw_filter *f = (struct fw_filter *)arg;
	struct fw_filter **fp;
=======
		while ((f = rtnl_dereference(head->ht[h])) != NULL) {
			RCU_INIT_POINTER(head->ht[h],
					 rtnl_dereference(f->next));
			tcf_unbind_filter(tp, &f->res);
			if (tcf_exts_get_net(&f->exts))
				tcf_queue_work(&f->rwork, fw_delete_filter_work);
			else
				__fw_delete_filter(f);
		}
	}
	kfree_rcu(head, rcu);
}

static int fw_delete(struct tcf_proto *tp, void *arg, bool *last,
		     bool rtnl_held, struct netlink_ext_ack *extack)
{
	struct fw_head *head = rtnl_dereference(tp->root);
	struct fw_filter *f = arg;
	struct fw_filter __rcu **fp;
	struct fw_filter *pfp;
	int ret = -EINVAL;
	int h;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (head == NULL || f == NULL)
		goto out;

<<<<<<< HEAD
	for (fp = &head->ht[fw_hash(f->id)]; *fp; fp = &(*fp)->next) {
		if (*fp == f) {
			tcf_tree_lock(tp);
			*fp = f->next;
			tcf_tree_unlock(tp);
			fw_delete_filter(tp, f);
			return 0;
		}
	}
out:
	return -EINVAL;
=======
	fp = &head->ht[fw_hash(f->id)];

	for (pfp = rtnl_dereference(*fp); pfp;
	     fp = &pfp->next, pfp = rtnl_dereference(*fp)) {
		if (pfp == f) {
			RCU_INIT_POINTER(*fp, rtnl_dereference(f->next));
			tcf_unbind_filter(tp, &f->res);
			tcf_exts_get_net(&f->exts);
			tcf_queue_work(&f->rwork, fw_delete_filter_work);
			ret = 0;
			break;
		}
	}

	*last = true;
	for (h = 0; h < HTSIZE; h++) {
		if (rcu_access_pointer(head->ht[h])) {
			*last = false;
			break;
		}
	}

out:
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct nla_policy fw_policy[TCA_FW_MAX + 1] = {
	[TCA_FW_CLASSID]	= { .type = NLA_U32 },
	[TCA_FW_INDEV]		= { .type = NLA_STRING, .len = IFNAMSIZ },
	[TCA_FW_MASK]		= { .type = NLA_U32 },
};

<<<<<<< HEAD
static int
fw_change_attrs(struct tcf_proto *tp, struct fw_filter *f,
	struct nlattr **tb, struct nlattr **tca, unsigned long base)
{
	struct fw_head *head = (struct fw_head *)tp->root;
	struct tcf_exts e;
	u32 mask;
	int err;

	err = tcf_exts_validate(tp, tb, tca[TCA_RATE], &e, &fw_ext_map);
	if (err < 0)
		return err;

	err = -EINVAL;
=======
static int fw_set_parms(struct net *net, struct tcf_proto *tp,
			struct fw_filter *f, struct nlattr **tb,
			struct nlattr **tca, unsigned long base, u32 flags,
			struct netlink_ext_ack *extack)
{
	struct fw_head *head = rtnl_dereference(tp->root);
	u32 mask;
	int err;

	err = tcf_exts_validate(net, tp, tb, tca[TCA_RATE], &f->exts, flags,
				extack);
	if (err < 0)
		return err;

	if (tb[TCA_FW_INDEV]) {
		int ret;
		ret = tcf_change_indev(net, tb[TCA_FW_INDEV], extack);
		if (ret < 0)
			return ret;
		f->ifindex = ret;
	}

	err = -EINVAL;
	if (tb[TCA_FW_MASK]) {
		mask = nla_get_u32(tb[TCA_FW_MASK]);
		if (mask != head->mask)
			return err;
	} else if (head->mask != 0xFFFFFFFF)
		return err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (tb[TCA_FW_CLASSID]) {
		f->res.classid = nla_get_u32(tb[TCA_FW_CLASSID]);
		tcf_bind_filter(tp, &f->res, base);
	}

<<<<<<< HEAD
#ifdef CONFIG_NET_CLS_IND
	if (tb[TCA_FW_INDEV]) {
		err = tcf_change_indev(tp, f->indev, tb[TCA_FW_INDEV]);
		if (err < 0)
			goto errout;
	}
#endif /* CONFIG_NET_CLS_IND */

	if (tb[TCA_FW_MASK]) {
		mask = nla_get_u32(tb[TCA_FW_MASK]);
		if (mask != head->mask)
			goto errout;
	} else if (head->mask != 0xFFFFFFFF)
		goto errout;

	tcf_exts_change(tp, &f->exts, &e);

	return 0;
errout:
	tcf_exts_destroy(tp, &e);
	return err;
}

static int fw_change(struct tcf_proto *tp, unsigned long base,
		     u32 handle,
		     struct nlattr **tca,
		     unsigned long *arg)
{
	struct fw_head *head = (struct fw_head *)tp->root;
	struct fw_filter *f = (struct fw_filter *) *arg;
=======
	return 0;
}

static int fw_change(struct net *net, struct sk_buff *in_skb,
		     struct tcf_proto *tp, unsigned long base,
		     u32 handle, struct nlattr **tca, void **arg,
		     u32 flags, struct netlink_ext_ack *extack)
{
	struct fw_head *head = rtnl_dereference(tp->root);
	struct fw_filter *f = *arg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nlattr *opt = tca[TCA_OPTIONS];
	struct nlattr *tb[TCA_FW_MAX + 1];
	int err;

	if (!opt)
<<<<<<< HEAD
		return handle ? -EINVAL : 0;

	err = nla_parse_nested(tb, TCA_FW_MAX, opt, fw_policy);
	if (err < 0)
		return err;

	if (f != NULL) {
		if (f->id != handle && handle)
			return -EINVAL;
		return fw_change_attrs(tp, f, tb, tca, base);
=======
		return handle ? -EINVAL : 0; /* Succeed if it is old method. */

	err = nla_parse_nested_deprecated(tb, TCA_FW_MAX, opt, fw_policy,
					  NULL);
	if (err < 0)
		return err;

	if (f) {
		struct fw_filter *pfp, *fnew;
		struct fw_filter __rcu **fp;

		if (f->id != handle && handle)
			return -EINVAL;

		fnew = kzalloc(sizeof(struct fw_filter), GFP_KERNEL);
		if (!fnew)
			return -ENOBUFS;

		fnew->id = f->id;
		fnew->ifindex = f->ifindex;
		fnew->tp = f->tp;

		err = tcf_exts_init(&fnew->exts, net, TCA_FW_ACT,
				    TCA_FW_POLICE);
		if (err < 0) {
			kfree(fnew);
			return err;
		}

		err = fw_set_parms(net, tp, fnew, tb, tca, base, flags, extack);
		if (err < 0) {
			tcf_exts_destroy(&fnew->exts);
			kfree(fnew);
			return err;
		}

		fp = &head->ht[fw_hash(fnew->id)];
		for (pfp = rtnl_dereference(*fp); pfp;
		     fp = &pfp->next, pfp = rtnl_dereference(*fp))
			if (pfp == f)
				break;

		RCU_INIT_POINTER(fnew->next, rtnl_dereference(pfp->next));
		rcu_assign_pointer(*fp, fnew);
		tcf_unbind_filter(tp, &f->res);
		tcf_exts_get_net(&f->exts);
		tcf_queue_work(&f->rwork, fw_delete_filter_work);

		*arg = fnew;
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!handle)
		return -EINVAL;

<<<<<<< HEAD
	if (head == NULL) {
=======
	if (!head) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		u32 mask = 0xFFFFFFFF;
		if (tb[TCA_FW_MASK])
			mask = nla_get_u32(tb[TCA_FW_MASK]);

<<<<<<< HEAD
		head = kzalloc(sizeof(struct fw_head), GFP_KERNEL);
		if (head == NULL)
			return -ENOBUFS;
		head->mask = mask;

		tcf_tree_lock(tp);
		tp->root = head;
		tcf_tree_unlock(tp);
=======
		head = kzalloc(sizeof(*head), GFP_KERNEL);
		if (!head)
			return -ENOBUFS;
		head->mask = mask;

		rcu_assign_pointer(tp->root, head);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	f = kzalloc(sizeof(struct fw_filter), GFP_KERNEL);
	if (f == NULL)
		return -ENOBUFS;

<<<<<<< HEAD
	f->id = handle;

	err = fw_change_attrs(tp, f, tb, tca, base);
	if (err < 0)
		goto errout;

	f->next = head->ht[fw_hash(handle)];
	tcf_tree_lock(tp);
	head->ht[fw_hash(handle)] = f;
	tcf_tree_unlock(tp);

	*arg = (unsigned long)f;
	return 0;

errout:
=======
	err = tcf_exts_init(&f->exts, net, TCA_FW_ACT, TCA_FW_POLICE);
	if (err < 0)
		goto errout;
	f->id = handle;
	f->tp = tp;

	err = fw_set_parms(net, tp, f, tb, tca, base, flags, extack);
	if (err < 0)
		goto errout;

	RCU_INIT_POINTER(f->next, head->ht[fw_hash(handle)]);
	rcu_assign_pointer(head->ht[fw_hash(handle)], f);

	*arg = f;
	return 0;

errout:
	tcf_exts_destroy(&f->exts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(f);
	return err;
}

<<<<<<< HEAD
static void fw_walk(struct tcf_proto *tp, struct tcf_walker *arg)
{
	struct fw_head *head = (struct fw_head *)tp->root;
=======
static void fw_walk(struct tcf_proto *tp, struct tcf_walker *arg,
		    bool rtnl_held)
{
	struct fw_head *head = rtnl_dereference(tp->root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int h;

	if (head == NULL)
		arg->stop = 1;

	if (arg->stop)
		return;

	for (h = 0; h < HTSIZE; h++) {
		struct fw_filter *f;

<<<<<<< HEAD
		for (f = head->ht[h]; f; f = f->next) {
			if (arg->count < arg->skip) {
				arg->count++;
				continue;
			}
			if (arg->fn(tp, (unsigned long)f, arg) < 0) {
				arg->stop = 1;
				return;
			}
			arg->count++;
=======
		for (f = rtnl_dereference(head->ht[h]); f;
		     f = rtnl_dereference(f->next)) {
			if (!tc_cls_stats_dump(tp, arg, f))
				return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

<<<<<<< HEAD
static int fw_dump(struct tcf_proto *tp, unsigned long fh,
		   struct sk_buff *skb, struct tcmsg *t)
{
	struct fw_head *head = (struct fw_head *)tp->root;
	struct fw_filter *f = (struct fw_filter *)fh;
	unsigned char *b = skb_tail_pointer(skb);
=======
static int fw_dump(struct net *net, struct tcf_proto *tp, void *fh,
		   struct sk_buff *skb, struct tcmsg *t, bool rtnl_held)
{
	struct fw_head *head = rtnl_dereference(tp->root);
	struct fw_filter *f = fh;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nlattr *nest;

	if (f == NULL)
		return skb->len;

	t->tcm_handle = f->id;

<<<<<<< HEAD
	if (!f->res.classid && !tcf_exts_is_available(&f->exts))
		return skb->len;

	nest = nla_nest_start(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	if (f->res.classid)
		NLA_PUT_U32(skb, TCA_FW_CLASSID, f->res.classid);
#ifdef CONFIG_NET_CLS_IND
	if (strlen(f->indev))
		NLA_PUT_STRING(skb, TCA_FW_INDEV, f->indev);
#endif /* CONFIG_NET_CLS_IND */
	if (head->mask != 0xFFFFFFFF)
		NLA_PUT_U32(skb, TCA_FW_MASK, head->mask);

	if (tcf_exts_dump(skb, &f->exts, &fw_ext_map) < 0)
=======
	if (!f->res.classid && !tcf_exts_has_actions(&f->exts))
		return skb->len;

	nest = nla_nest_start_noflag(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	if (f->res.classid &&
	    nla_put_u32(skb, TCA_FW_CLASSID, f->res.classid))
		goto nla_put_failure;
	if (f->ifindex) {
		struct net_device *dev;
		dev = __dev_get_by_index(net, f->ifindex);
		if (dev && nla_put_string(skb, TCA_FW_INDEV, dev->name))
			goto nla_put_failure;
	}
	if (head->mask != 0xFFFFFFFF &&
	    nla_put_u32(skb, TCA_FW_MASK, head->mask))
		goto nla_put_failure;

	if (tcf_exts_dump(skb, &f->exts) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto nla_put_failure;

	nla_nest_end(skb, nest);

<<<<<<< HEAD
	if (tcf_exts_dump_stats(skb, &f->exts, &fw_ext_map) < 0)
=======
	if (tcf_exts_dump_stats(skb, &f->exts) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto nla_put_failure;

	return skb->len;

nla_put_failure:
<<<<<<< HEAD
	nlmsg_trim(skb, b);
	return -1;
}

=======
	nla_nest_cancel(skb, nest);
	return -1;
}

static void fw_bind_class(void *fh, u32 classid, unsigned long cl, void *q,
			  unsigned long base)
{
	struct fw_filter *f = fh;

	tc_cls_bind_class(classid, cl, q, &f->res, base);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct tcf_proto_ops cls_fw_ops __read_mostly = {
	.kind		=	"fw",
	.classify	=	fw_classify,
	.init		=	fw_init,
	.destroy	=	fw_destroy,
	.get		=	fw_get,
<<<<<<< HEAD
	.put		=	fw_put,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.change		=	fw_change,
	.delete		=	fw_delete,
	.walk		=	fw_walk,
	.dump		=	fw_dump,
<<<<<<< HEAD
	.owner		=	THIS_MODULE,
};
=======
	.bind_class	=	fw_bind_class,
	.owner		=	THIS_MODULE,
};
MODULE_ALIAS_NET_CLS("fw");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init init_fw(void)
{
	return register_tcf_proto_ops(&cls_fw_ops);
}

static void __exit exit_fw(void)
{
	unregister_tcf_proto_ops(&cls_fw_ops);
}

module_init(init_fw)
module_exit(exit_fw)
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("SKB mark based TC classifier");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
