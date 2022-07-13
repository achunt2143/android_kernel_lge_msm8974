<<<<<<< HEAD
/*
 * net/sched/simp.c	Simple example of an action
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Jamal Hadi Salim (2005-8)
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * net/sched/act_simple.c	Simple example of an action
 *
 * Authors:	Jamal Hadi Salim (2005-8)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/rtnetlink.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>
<<<<<<< HEAD

#define TCA_ACT_SIMP 22
=======
#include <net/pkt_cls.h>
#include <net/tc_wrapper.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/tc_act/tc_defact.h>
#include <net/tc_act/tc_defact.h>

<<<<<<< HEAD
#define SIMP_TAB_MASK     7
static struct tcf_common *tcf_simp_ht[SIMP_TAB_MASK + 1];
static u32 simp_idx_gen;
static DEFINE_RWLOCK(simp_lock);

static struct tcf_hashinfo simp_hash_info = {
	.htab	=	tcf_simp_ht,
	.hmask	=	SIMP_TAB_MASK,
	.lock	=	&simp_lock,
};

#define SIMP_MAX_DATA	32
static int tcf_simp(struct sk_buff *skb, const struct tc_action *a,
		    struct tcf_result *res)
{
	struct tcf_defact *d = a->priv;

	spin_lock(&d->tcf_lock);
	d->tcf_tm.lastuse = jiffies;
=======
static struct tc_action_ops act_simp_ops;

#define SIMP_MAX_DATA	32
TC_INDIRECT_SCOPE int tcf_simp_act(struct sk_buff *skb,
				   const struct tc_action *a,
				   struct tcf_result *res)
{
	struct tcf_defact *d = to_defact(a);

	spin_lock(&d->tcf_lock);
	tcf_lastuse_update(&d->tcf_tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bstats_update(&d->tcf_bstats, skb);

	/* print policy string followed by _ then packet count
	 * Example if this was the 3rd packet and the string was "hello"
	 * then it would look like "hello_3" (without quotes)
	 */
<<<<<<< HEAD
	pr_info("simple: %s_%d\n",
	       (char *)d->tcfd_defdata, d->tcf_bstats.packets);
=======
	pr_info("simple: %s_%llu\n",
		(char *)d->tcfd_defdata,
		u64_stats_read(&d->tcf_bstats.packets));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&d->tcf_lock);
	return d->tcf_action;
}

<<<<<<< HEAD
static int tcf_simp_release(struct tcf_defact *d, int bind)
{
	int ret = 0;
	if (d) {
		if (bind)
			d->tcf_bindcnt--;
		d->tcf_refcnt--;
		if (d->tcf_bindcnt <= 0 && d->tcf_refcnt <= 0) {
			kfree(d->tcfd_defdata);
			tcf_hash_destroy(&d->common, &simp_hash_info);
			ret = 1;
		}
	}
	return ret;
}

static int alloc_defdata(struct tcf_defact *d, char *defdata)
=======
static void tcf_simp_release(struct tc_action *a)
{
	struct tcf_defact *d = to_defact(a);
	kfree(d->tcfd_defdata);
}

static int alloc_defdata(struct tcf_defact *d, const struct nlattr *defdata)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	d->tcfd_defdata = kzalloc(SIMP_MAX_DATA, GFP_KERNEL);
	if (unlikely(!d->tcfd_defdata))
		return -ENOMEM;
<<<<<<< HEAD
	strlcpy(d->tcfd_defdata, defdata, SIMP_MAX_DATA);
	return 0;
}

static void reset_policy(struct tcf_defact *d, char *defdata,
			 struct tc_defact *p)
{
	spin_lock_bh(&d->tcf_lock);
	d->tcf_action = p->action;
	memset(d->tcfd_defdata, 0, SIMP_MAX_DATA);
	strlcpy(d->tcfd_defdata, defdata, SIMP_MAX_DATA);
	spin_unlock_bh(&d->tcf_lock);
=======
	nla_strscpy(d->tcfd_defdata, defdata, SIMP_MAX_DATA);
	return 0;
}

static int reset_policy(struct tc_action *a, const struct nlattr *defdata,
			struct tc_defact *p, struct tcf_proto *tp,
			struct netlink_ext_ack *extack)
{
	struct tcf_chain *goto_ch = NULL;
	struct tcf_defact *d;
	int err;

	err = tcf_action_check_ctrlact(p->action, tp, &goto_ch, extack);
	if (err < 0)
		return err;
	d = to_defact(a);
	spin_lock_bh(&d->tcf_lock);
	goto_ch = tcf_action_set_ctrlact(a, p->action, goto_ch);
	memset(d->tcfd_defdata, 0, SIMP_MAX_DATA);
	nla_strscpy(d->tcfd_defdata, defdata, SIMP_MAX_DATA);
	spin_unlock_bh(&d->tcf_lock);
	if (goto_ch)
		tcf_chain_put_by_act(goto_ch);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct nla_policy simple_policy[TCA_DEF_MAX + 1] = {
	[TCA_DEF_PARMS]	= { .len = sizeof(struct tc_defact) },
	[TCA_DEF_DATA]	= { .type = NLA_STRING, .len = SIMP_MAX_DATA },
};

<<<<<<< HEAD
static int tcf_simp_init(struct nlattr *nla, struct nlattr *est,
			 struct tc_action *a, int ovr, int bind)
{
	struct nlattr *tb[TCA_DEF_MAX + 1];
	struct tc_defact *parm;
	struct tcf_defact *d;
	struct tcf_common *pc;
	char *defdata;
	int ret = 0, err;
=======
static int tcf_simp_init(struct net *net, struct nlattr *nla,
			 struct nlattr *est, struct tc_action **a,
			 struct tcf_proto *tp, u32 flags,
			 struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, act_simp_ops.net_id);
	bool bind = flags & TCA_ACT_FLAGS_BIND;
	struct nlattr *tb[TCA_DEF_MAX + 1];
	struct tcf_chain *goto_ch = NULL;
	struct tc_defact *parm;
	struct tcf_defact *d;
	bool exists = false;
	int ret = 0, err;
	u32 index;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (nla == NULL)
		return -EINVAL;

<<<<<<< HEAD
	err = nla_parse_nested(tb, TCA_DEF_MAX, nla, simple_policy);
=======
	err = nla_parse_nested_deprecated(tb, TCA_DEF_MAX, nla, simple_policy,
					  NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		return err;

	if (tb[TCA_DEF_PARMS] == NULL)
		return -EINVAL;

<<<<<<< HEAD
	if (tb[TCA_DEF_DATA] == NULL)
		return -EINVAL;

	parm = nla_data(tb[TCA_DEF_PARMS]);
	defdata = nla_data(tb[TCA_DEF_DATA]);

	pc = tcf_hash_check(parm->index, a, bind, &simp_hash_info);
	if (!pc) {
		pc = tcf_hash_create(parm->index, est, a, sizeof(*d), bind,
				     &simp_idx_gen, &simp_hash_info);
		if (IS_ERR(pc))
			return PTR_ERR(pc);

		d = to_defact(pc);
		ret = alloc_defdata(d, defdata);
		if (ret < 0) {
			kfree(pc);
			return ret;
		}
		d->tcf_action = parm->action;
		ret = ACT_P_CREATED;
	} else {
		d = to_defact(pc);
		if (!ovr) {
			tcf_simp_release(d, bind);
			return -EEXIST;
		}
		reset_policy(d, defdata, parm);
	}

	if (ret == ACT_P_CREATED)
		tcf_hash_insert(pc, &simp_hash_info);
	return ret;
}

static int tcf_simp_cleanup(struct tc_action *a, int bind)
{
	struct tcf_defact *d = a->priv;

	if (d)
		return tcf_simp_release(d, bind);
	return 0;
=======
	parm = nla_data(tb[TCA_DEF_PARMS]);
	index = parm->index;
	err = tcf_idr_check_alloc(tn, &index, a, bind);
	if (err < 0)
		return err;
	exists = err;
	if (exists && bind)
		return ACT_P_BOUND;

	if (tb[TCA_DEF_DATA] == NULL) {
		if (exists)
			tcf_idr_release(*a, bind);
		else
			tcf_idr_cleanup(tn, index);
		return -EINVAL;
	}

	if (!exists) {
		ret = tcf_idr_create(tn, index, est, a,
				     &act_simp_ops, bind, false, flags);
		if (ret) {
			tcf_idr_cleanup(tn, index);
			return ret;
		}

		d = to_defact(*a);
		err = tcf_action_check_ctrlact(parm->action, tp, &goto_ch,
					       extack);
		if (err < 0)
			goto release_idr;

		err = alloc_defdata(d, tb[TCA_DEF_DATA]);
		if (err < 0)
			goto put_chain;

		tcf_action_set_ctrlact(*a, parm->action, goto_ch);
		ret = ACT_P_CREATED;
	} else {
		if (!(flags & TCA_ACT_FLAGS_REPLACE)) {
			err = -EEXIST;
			goto release_idr;
		}

		err = reset_policy(*a, tb[TCA_DEF_DATA], parm, tp, extack);
		if (err)
			goto release_idr;
	}

	return ret;
put_chain:
	if (goto_ch)
		tcf_chain_put_by_act(goto_ch);
release_idr:
	tcf_idr_release(*a, bind);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tcf_simp_dump(struct sk_buff *skb, struct tc_action *a,
			 int bind, int ref)
{
	unsigned char *b = skb_tail_pointer(skb);
<<<<<<< HEAD
	struct tcf_defact *d = a->priv;
	struct tc_defact opt = {
		.index   = d->tcf_index,
		.refcnt  = d->tcf_refcnt - ref,
		.bindcnt = d->tcf_bindcnt - bind,
		.action  = d->tcf_action,
	};
	struct tcf_t t;

	NLA_PUT(skb, TCA_DEF_PARMS, sizeof(opt), &opt);
	NLA_PUT_STRING(skb, TCA_DEF_DATA, d->tcfd_defdata);
	t.install = jiffies_to_clock_t(jiffies - d->tcf_tm.install);
	t.lastuse = jiffies_to_clock_t(jiffies - d->tcf_tm.lastuse);
	t.expires = jiffies_to_clock_t(d->tcf_tm.expires);
	NLA_PUT(skb, TCA_DEF_TM, sizeof(t), &t);
	return skb->len;

nla_put_failure:
=======
	struct tcf_defact *d = to_defact(a);
	struct tc_defact opt = {
		.index   = d->tcf_index,
		.refcnt  = refcount_read(&d->tcf_refcnt) - ref,
		.bindcnt = atomic_read(&d->tcf_bindcnt) - bind,
	};
	struct tcf_t t;

	spin_lock_bh(&d->tcf_lock);
	opt.action = d->tcf_action;
	if (nla_put(skb, TCA_DEF_PARMS, sizeof(opt), &opt) ||
	    nla_put_string(skb, TCA_DEF_DATA, d->tcfd_defdata))
		goto nla_put_failure;

	tcf_tm_dump(&t, &d->tcf_tm);
	if (nla_put_64bit(skb, TCA_DEF_TM, sizeof(t), &t, TCA_DEF_PAD))
		goto nla_put_failure;
	spin_unlock_bh(&d->tcf_lock);

	return skb->len;

nla_put_failure:
	spin_unlock_bh(&d->tcf_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nlmsg_trim(skb, b);
	return -1;
}

static struct tc_action_ops act_simp_ops = {
	.kind		=	"simple",
<<<<<<< HEAD
	.hinfo		=	&simp_hash_info,
	.type		=	TCA_ACT_SIMP,
	.capab		=	TCA_CAP_NONE,
	.owner		=	THIS_MODULE,
	.act		=	tcf_simp,
	.dump		=	tcf_simp_dump,
	.cleanup	=	tcf_simp_cleanup,
	.init		=	tcf_simp_init,
	.walk		=	tcf_generic_walker,
=======
	.id		=	TCA_ID_SIMP,
	.owner		=	THIS_MODULE,
	.act		=	tcf_simp_act,
	.dump		=	tcf_simp_dump,
	.cleanup	=	tcf_simp_release,
	.init		=	tcf_simp_init,
	.size		=	sizeof(struct tcf_defact),
};
MODULE_ALIAS_NET_ACT("simple");

static __net_init int simp_init_net(struct net *net)
{
	struct tc_action_net *tn = net_generic(net, act_simp_ops.net_id);

	return tc_action_net_init(net, tn, &act_simp_ops);
}

static void __net_exit simp_exit_net(struct list_head *net_list)
{
	tc_action_net_exit(net_list, act_simp_ops.net_id);
}

static struct pernet_operations simp_net_ops = {
	.init = simp_init_net,
	.exit_batch = simp_exit_net,
	.id   = &act_simp_ops.net_id,
	.size = sizeof(struct tc_action_net),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

MODULE_AUTHOR("Jamal Hadi Salim(2005)");
MODULE_DESCRIPTION("Simple example action");
MODULE_LICENSE("GPL");

static int __init simp_init_module(void)
{
<<<<<<< HEAD
	int ret = tcf_register_action(&act_simp_ops);
=======
	int ret = tcf_register_action(&act_simp_ops, &simp_net_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ret)
		pr_info("Simple TC action Loaded\n");
	return ret;
}

static void __exit simp_cleanup_module(void)
{
<<<<<<< HEAD
	tcf_unregister_action(&act_simp_ops);
=======
	tcf_unregister_action(&act_simp_ops, &simp_net_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(simp_init_module);
module_exit(simp_cleanup_module);
