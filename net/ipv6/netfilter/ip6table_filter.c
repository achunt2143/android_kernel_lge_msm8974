<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This is the 1999 rewrite of IP Firewalling, aiming for kernel 2.3.x.
 *
 * Copyright (C) 1999 Paul `Rusty' Russell & Michael J. Neuling
 * Copyright (C) 2000-2004 Netfilter Core Team <coreteam@netfilter.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("ip6tables filter table");

#define FILTER_VALID_HOOKS ((1 << NF_INET_LOCAL_IN) | \
			    (1 << NF_INET_FORWARD) | \
			    (1 << NF_INET_LOCAL_OUT))

static const struct xt_table packet_filter = {
	.name		= "filter",
	.valid_hooks	= FILTER_VALID_HOOKS,
	.me		= THIS_MODULE,
	.af		= NFPROTO_IPV6,
	.priority	= NF_IP6_PRI_FILTER,
};

<<<<<<< HEAD
/* The work comes in here from netfilter.c. */
static unsigned int
ip6table_filter_hook(unsigned int hook, struct sk_buff *skb,
		     const struct net_device *in, const struct net_device *out,
		     int (*okfn)(struct sk_buff *))
{
	const struct net *net = dev_net((in != NULL) ? in : out);

	return ip6t_do_table(skb, hook, in, out, net->ipv6.ip6table_filter);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct nf_hook_ops *filter_ops __read_mostly;

/* Default to forward because I got too much mail already. */
static bool forward = true;
module_param(forward, bool, 0000);

<<<<<<< HEAD
static int __net_init ip6table_filter_net_init(struct net *net)
{
	struct ip6t_replace *repl;
=======
static int ip6table_filter_table_init(struct net *net)
{
	struct ip6t_replace *repl;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	repl = ip6t_alloc_initial_table(&packet_filter);
	if (repl == NULL)
		return -ENOMEM;
	/* Entry 1 is the FORWARD hook */
	((struct ip6t_standard *)repl->entries)[1].target.verdict =
		forward ? -NF_ACCEPT - 1 : -NF_DROP - 1;

<<<<<<< HEAD
	net->ipv6.ip6table_filter =
		ip6t_register_table(net, &packet_filter, repl);
	kfree(repl);
	if (IS_ERR(net->ipv6.ip6table_filter))
		return PTR_ERR(net->ipv6.ip6table_filter);
	return 0;
}

static void __net_exit ip6table_filter_net_exit(struct net *net)
{
	ip6t_unregister_table(net, net->ipv6.ip6table_filter);
=======
	err = ip6t_register_table(net, &packet_filter, repl, filter_ops);
	kfree(repl);
	return err;
}

static int __net_init ip6table_filter_net_init(struct net *net)
{
	if (!forward)
		return ip6table_filter_table_init(net);

	return 0;
}

static void __net_exit ip6table_filter_net_pre_exit(struct net *net)
{
	ip6t_unregister_table_pre_exit(net, "filter");
}

static void __net_exit ip6table_filter_net_exit(struct net *net)
{
	ip6t_unregister_table_exit(net, "filter");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations ip6table_filter_net_ops = {
	.init = ip6table_filter_net_init,
<<<<<<< HEAD
=======
	.pre_exit = ip6table_filter_net_pre_exit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.exit = ip6table_filter_net_exit,
};

static int __init ip6table_filter_init(void)
{
<<<<<<< HEAD
	int ret;

	ret = register_pernet_subsys(&ip6table_filter_net_ops);
	if (ret < 0)
		return ret;

	/* Register hooks */
	filter_ops = xt_hook_link(&packet_filter, ip6table_filter_hook);
	if (IS_ERR(filter_ops)) {
		ret = PTR_ERR(filter_ops);
		goto cleanup_table;
	}

	return ret;

 cleanup_table:
	unregister_pernet_subsys(&ip6table_filter_net_ops);
=======
	int ret = xt_register_template(&packet_filter,
					ip6table_filter_table_init);

	if (ret < 0)
		return ret;

	filter_ops = xt_hook_ops_alloc(&packet_filter, ip6t_do_table);
	if (IS_ERR(filter_ops)) {
		xt_unregister_template(&packet_filter);
		return PTR_ERR(filter_ops);
	}

	ret = register_pernet_subsys(&ip6table_filter_net_ops);
	if (ret < 0) {
		xt_unregister_template(&packet_filter);
		kfree(filter_ops);
		return ret;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static void __exit ip6table_filter_fini(void)
{
<<<<<<< HEAD
	xt_hook_unlink(&packet_filter, filter_ops);
	unregister_pernet_subsys(&ip6table_filter_net_ops);
=======
	unregister_pernet_subsys(&ip6table_filter_net_ops);
	xt_unregister_template(&packet_filter);
	kfree(filter_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(ip6table_filter_init);
module_exit(ip6table_filter_fini);
