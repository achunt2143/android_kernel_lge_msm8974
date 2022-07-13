<<<<<<< HEAD
/*
 * 'raw' table, which is the very first hooked in at PRE_ROUTING and LOCAL_OUT .
 *
 * Copyright (C) 2003 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 */
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * 'raw' table, which is the very first hooked in at PRE_ROUTING and LOCAL_OUT .
 *
 * Copyright (C) 2003 Jozsef Kadlecsik <kadlec@netfilter.org>
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/slab.h>
#include <net/ip.h>

#define RAW_VALID_HOOKS ((1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_OUT))

<<<<<<< HEAD
=======
static bool raw_before_defrag __read_mostly;
MODULE_PARM_DESC(raw_before_defrag, "Enable raw table before defrag");
module_param(raw_before_defrag, bool, 0000);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct xt_table packet_raw = {
	.name = "raw",
	.valid_hooks =  RAW_VALID_HOOKS,
	.me = THIS_MODULE,
	.af = NFPROTO_IPV4,
	.priority = NF_IP_PRI_RAW,
};

<<<<<<< HEAD
/* The work comes in here from netfilter.c. */
static unsigned int
iptable_raw_hook(unsigned int hook, struct sk_buff *skb,
		 const struct net_device *in, const struct net_device *out,
		 int (*okfn)(struct sk_buff *))
{
	const struct net *net;

	if (hook == NF_INET_LOCAL_OUT && 
	    (skb->len < sizeof(struct iphdr) ||
	     ip_hdrlen(skb) < sizeof(struct iphdr)))
		/* root is playing with raw sockets. */
		return NF_ACCEPT;

	net = dev_net((in != NULL) ? in : out);
	return ipt_do_table(skb, hook, in, out, net->ipv4.iptable_raw);
}

static struct nf_hook_ops *rawtable_ops __read_mostly;

static int __net_init iptable_raw_net_init(struct net *net)
{
	struct ipt_replace *repl;

	repl = ipt_alloc_initial_table(&packet_raw);
	if (repl == NULL)
		return -ENOMEM;
	net->ipv4.iptable_raw =
		ipt_register_table(net, &packet_raw, repl);
	kfree(repl);
	if (IS_ERR(net->ipv4.iptable_raw))
		return PTR_ERR(net->ipv4.iptable_raw);
	return 0;
=======
static const struct xt_table packet_raw_before_defrag = {
	.name = "raw",
	.valid_hooks =  RAW_VALID_HOOKS,
	.me = THIS_MODULE,
	.af = NFPROTO_IPV4,
	.priority = NF_IP_PRI_RAW_BEFORE_DEFRAG,
};

static struct nf_hook_ops *rawtable_ops __read_mostly;

static int iptable_raw_table_init(struct net *net)
{
	struct ipt_replace *repl;
	const struct xt_table *table = &packet_raw;
	int ret;

	if (raw_before_defrag)
		table = &packet_raw_before_defrag;

	repl = ipt_alloc_initial_table(table);
	if (repl == NULL)
		return -ENOMEM;
	ret = ipt_register_table(net, table, repl, rawtable_ops);
	kfree(repl);
	return ret;
}

static void __net_exit iptable_raw_net_pre_exit(struct net *net)
{
	ipt_unregister_table_pre_exit(net, "raw");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __net_exit iptable_raw_net_exit(struct net *net)
{
<<<<<<< HEAD
	ipt_unregister_table(net, net->ipv4.iptable_raw);
}

static struct pernet_operations iptable_raw_net_ops = {
	.init = iptable_raw_net_init,
=======
	ipt_unregister_table_exit(net, "raw");
}

static struct pernet_operations iptable_raw_net_ops = {
	.pre_exit = iptable_raw_net_pre_exit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.exit = iptable_raw_net_exit,
};

static int __init iptable_raw_init(void)
{
	int ret;
<<<<<<< HEAD

	ret = register_pernet_subsys(&iptable_raw_net_ops);
	if (ret < 0)
		return ret;

	/* Register hooks */
	rawtable_ops = xt_hook_link(&packet_raw, iptable_raw_hook);
	if (IS_ERR(rawtable_ops)) {
		ret = PTR_ERR(rawtable_ops);
		goto cleanup_table;
	}

	return ret;

 cleanup_table:
	unregister_pernet_subsys(&iptable_raw_net_ops);
=======
	const struct xt_table *table = &packet_raw;

	if (raw_before_defrag) {
		table = &packet_raw_before_defrag;

		pr_info("Enabling raw table before defrag\n");
	}

	ret = xt_register_template(table,
				   iptable_raw_table_init);
	if (ret < 0)
		return ret;

	rawtable_ops = xt_hook_ops_alloc(table, ipt_do_table);
	if (IS_ERR(rawtable_ops)) {
		xt_unregister_template(table);
		return PTR_ERR(rawtable_ops);
	}

	ret = register_pernet_subsys(&iptable_raw_net_ops);
	if (ret < 0) {
		xt_unregister_template(table);
		kfree(rawtable_ops);
		return ret;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static void __exit iptable_raw_fini(void)
{
<<<<<<< HEAD
	xt_hook_unlink(&packet_raw, rawtable_ops);
	unregister_pernet_subsys(&iptable_raw_net_ops);
=======
	unregister_pernet_subsys(&iptable_raw_net_ops);
	kfree(rawtable_ops);
	xt_unregister_template(&packet_raw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(iptable_raw_init);
module_exit(iptable_raw_fini);
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("iptables legacy raw table");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
