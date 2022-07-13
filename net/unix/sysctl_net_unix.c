<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * NET4:	Sysctl interface to net af_unix subsystem.
 *
 * Authors:	Mike Shaver.
<<<<<<< HEAD
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/sysctl.h>

#include <net/af_unix.h>

<<<<<<< HEAD
static ctl_table unix_table[] = {
=======
static struct ctl_table unix_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.procname	= "max_dgram_qlen",
		.data		= &init_net.unx.sysctl_max_dgram_qlen,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{ }
};

<<<<<<< HEAD
static struct ctl_path unix_path[] = {
	{ .procname = "net", },
	{ .procname = "unix", },
	{ },
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __net_init unix_sysctl_register(struct net *net)
{
	struct ctl_table *table;

<<<<<<< HEAD
	table = kmemdup(unix_table, sizeof(unix_table), GFP_KERNEL);
	if (table == NULL)
		goto err_alloc;

	table[0].data = &net->unx.sysctl_max_dgram_qlen;
	net->unx.ctl = register_net_sysctl_table(net, unix_path, table);
=======
	if (net_eq(net, &init_net)) {
		table = unix_table;
	} else {
		table = kmemdup(unix_table, sizeof(unix_table), GFP_KERNEL);
		if (!table)
			goto err_alloc;

		table[0].data = &net->unx.sysctl_max_dgram_qlen;
	}

	net->unx.ctl = register_net_sysctl_sz(net, "net/unix", table,
					      ARRAY_SIZE(unix_table));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (net->unx.ctl == NULL)
		goto err_reg;

	return 0;

err_reg:
<<<<<<< HEAD
	kfree(table);
=======
	if (!net_eq(net, &init_net))
		kfree(table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_alloc:
	return -ENOMEM;
}

void unix_sysctl_unregister(struct net *net)
{
	struct ctl_table *table;

	table = net->unx.ctl->ctl_table_arg;
<<<<<<< HEAD
	unregister_sysctl_table(net->unx.ctl);
	kfree(table);
=======
	unregister_net_sysctl_table(net->unx.ctl);
	if (!net_eq(net, &init_net))
		kfree(table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
