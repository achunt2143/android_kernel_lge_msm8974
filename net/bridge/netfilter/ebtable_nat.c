<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  ebtable_nat
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *
 *  April, 2002
 *
 */

#include <linux/netfilter_bridge/ebtables.h>
<<<<<<< HEAD
#include <linux/module.h>

#define NAT_VALID_HOOKS ((1 << NF_BR_PRE_ROUTING) | (1 << NF_BR_LOCAL_OUT) | \
   (1 << NF_BR_POST_ROUTING))

static struct ebt_entries initial_chains[] =
{
=======
#include <uapi/linux/netfilter_bridge.h>
#include <linux/module.h>

#define NAT_VALID_HOOKS ((1 << NF_BR_PRE_ROUTING) | (1 << NF_BR_LOCAL_OUT) | \
			 (1 << NF_BR_POST_ROUTING))

static struct ebt_entries initial_chains[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.name	= "PREROUTING",
		.policy	= EBT_ACCEPT,
	},
	{
		.name	= "OUTPUT",
		.policy	= EBT_ACCEPT,
	},
	{
		.name	= "POSTROUTING",
		.policy	= EBT_ACCEPT,
	}
};

<<<<<<< HEAD
static struct ebt_replace_kernel initial_table =
{
=======
static struct ebt_replace_kernel initial_table = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name		= "nat",
	.valid_hooks	= NAT_VALID_HOOKS,
	.entries_size	= 3 * sizeof(struct ebt_entries),
	.hook_entry	= {
		[NF_BR_PRE_ROUTING]	= &initial_chains[0],
		[NF_BR_LOCAL_OUT]	= &initial_chains[1],
		[NF_BR_POST_ROUTING]	= &initial_chains[2],
	},
	.entries	= (char *)initial_chains,
};

<<<<<<< HEAD
static int check(const struct ebt_table_info *info, unsigned int valid_hooks)
{
	if (valid_hooks & ~NAT_VALID_HOOKS)
		return -EINVAL;
	return 0;
}

static struct ebt_table frame_nat =
{
	.name		= "nat",
	.table		= &initial_table,
	.valid_hooks	= NAT_VALID_HOOKS,
	.check		= check,
	.me		= THIS_MODULE,
};

static unsigned int
ebt_nat_in(unsigned int hook, struct sk_buff *skb, const struct net_device *in
   , const struct net_device *out, int (*okfn)(struct sk_buff *))
{
	return ebt_do_table(hook, skb, in, out, dev_net(in)->xt.frame_nat);
}

static unsigned int
ebt_nat_out(unsigned int hook, struct sk_buff *skb, const struct net_device *in
   , const struct net_device *out, int (*okfn)(struct sk_buff *))
{
	return ebt_do_table(hook, skb, in, out, dev_net(out)->xt.frame_nat);
}

static struct nf_hook_ops ebt_ops_nat[] __read_mostly = {
	{
		.hook		= ebt_nat_out,
		.owner		= THIS_MODULE,
=======
static const struct ebt_table frame_nat = {
	.name		= "nat",
	.table		= &initial_table,
	.valid_hooks	= NAT_VALID_HOOKS,
	.me		= THIS_MODULE,
};

static const struct nf_hook_ops ebt_ops_nat[] = {
	{
		.hook		= ebt_do_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pf		= NFPROTO_BRIDGE,
		.hooknum	= NF_BR_LOCAL_OUT,
		.priority	= NF_BR_PRI_NAT_DST_OTHER,
	},
	{
<<<<<<< HEAD
		.hook		= ebt_nat_out,
		.owner		= THIS_MODULE,
=======
		.hook		= ebt_do_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pf		= NFPROTO_BRIDGE,
		.hooknum	= NF_BR_POST_ROUTING,
		.priority	= NF_BR_PRI_NAT_SRC,
	},
	{
<<<<<<< HEAD
		.hook		= ebt_nat_in,
		.owner		= THIS_MODULE,
=======
		.hook		= ebt_do_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pf		= NFPROTO_BRIDGE,
		.hooknum	= NF_BR_PRE_ROUTING,
		.priority	= NF_BR_PRI_NAT_DST_BRIDGED,
	},
};

<<<<<<< HEAD
static int __net_init frame_nat_net_init(struct net *net)
{
	net->xt.frame_nat = ebt_register_table(net, &frame_nat);
	if (IS_ERR(net->xt.frame_nat))
		return PTR_ERR(net->xt.frame_nat);
	return 0;
=======
static int frame_nat_table_init(struct net *net)
{
	return ebt_register_table(net, &frame_nat, ebt_ops_nat);
}

static void __net_exit frame_nat_net_pre_exit(struct net *net)
{
	ebt_unregister_table_pre_exit(net, "nat");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __net_exit frame_nat_net_exit(struct net *net)
{
<<<<<<< HEAD
	ebt_unregister_table(net, net->xt.frame_nat);
}

static struct pernet_operations frame_nat_net_ops = {
	.init = frame_nat_net_init,
	.exit = frame_nat_net_exit,
=======
	ebt_unregister_table(net, "nat");
}

static struct pernet_operations frame_nat_net_ops = {
	.exit = frame_nat_net_exit,
	.pre_exit = frame_nat_net_pre_exit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init ebtable_nat_init(void)
{
<<<<<<< HEAD
	int ret;

	ret = register_pernet_subsys(&frame_nat_net_ops);
	if (ret < 0)
		return ret;
	ret = nf_register_hooks(ebt_ops_nat, ARRAY_SIZE(ebt_ops_nat));
	if (ret < 0)
		unregister_pernet_subsys(&frame_nat_net_ops);
=======
	int ret = ebt_register_template(&frame_nat, frame_nat_table_init);

	if (ret)
		return ret;

	ret = register_pernet_subsys(&frame_nat_net_ops);
	if (ret) {
		ebt_unregister_template(&frame_nat);
		return ret;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static void __exit ebtable_nat_fini(void)
{
<<<<<<< HEAD
	nf_unregister_hooks(ebt_ops_nat, ARRAY_SIZE(ebt_ops_nat));
	unregister_pernet_subsys(&frame_nat_net_ops);
=======
	unregister_pernet_subsys(&frame_nat_net_ops);
	ebt_unregister_template(&frame_nat);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(ebtable_nat_init);
module_exit(ebtable_nat_fini);
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("ebtables legacy stateless nat table");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
