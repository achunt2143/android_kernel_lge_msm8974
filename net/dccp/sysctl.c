<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  net/dccp/sysctl.c
 *
 *  An implementation of the DCCP protocol
 *  Arnaldo Carvalho de Melo <acme@mandriva.com>
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License v2
 *	as published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/mm.h>
#include <linux/sysctl.h>
#include "dccp.h"
#include "feat.h"

#ifndef CONFIG_SYSCTL
#error This file should not be compiled without CONFIG_SYSCTL defined
#endif

/* Boundary values */
<<<<<<< HEAD
static int		zero     = 0,
			u8_max   = 0xFF;
=======
static int		u8_max   = 0xFF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned long	seqw_min = DCCPF_SEQ_WMIN,
			seqw_max = 0xFFFFFFFF;		/* maximum on 32 bit */

static struct ctl_table dccp_default_table[] = {
	{
		.procname	= "seq_window",
		.data		= &sysctl_dccp_sequence_window,
		.maxlen		= sizeof(sysctl_dccp_sequence_window),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
		.extra1		= &seqw_min,		/* RFC 4340, 7.5.2 */
		.extra2		= &seqw_max,
	},
	{
		.procname	= "rx_ccid",
		.data		= &sysctl_dccp_rx_ccid,
		.maxlen		= sizeof(sysctl_dccp_rx_ccid),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
=======
		.extra1		= SYSCTL_ZERO,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.extra2		= &u8_max,		/* RFC 4340, 10. */
	},
	{
		.procname	= "tx_ccid",
		.data		= &sysctl_dccp_tx_ccid,
		.maxlen		= sizeof(sysctl_dccp_tx_ccid),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
=======
		.extra1		= SYSCTL_ZERO,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.extra2		= &u8_max,		/* RFC 4340, 10. */
	},
	{
		.procname	= "request_retries",
		.data		= &sysctl_dccp_request_retries,
		.maxlen		= sizeof(sysctl_dccp_request_retries),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
=======
		.extra1		= SYSCTL_ONE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.extra2		= &u8_max,
	},
	{
		.procname	= "retries1",
		.data		= &sysctl_dccp_retries1,
		.maxlen		= sizeof(sysctl_dccp_retries1),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
=======
		.extra1		= SYSCTL_ZERO,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.extra2		= &u8_max,
	},
	{
		.procname	= "retries2",
		.data		= &sysctl_dccp_retries2,
		.maxlen		= sizeof(sysctl_dccp_retries2),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
=======
		.extra1		= SYSCTL_ZERO,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.extra2		= &u8_max,
	},
	{
		.procname	= "tx_qlen",
		.data		= &sysctl_dccp_tx_qlen,
		.maxlen		= sizeof(sysctl_dccp_tx_qlen),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
=======
		.extra1		= SYSCTL_ZERO,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname	= "sync_ratelimit",
		.data		= &sysctl_dccp_sync_ratelimit,
		.maxlen		= sizeof(sysctl_dccp_sync_ratelimit),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_ms_jiffies,
	},

	{ }
};

<<<<<<< HEAD
static struct ctl_path dccp_path[] = {
	{ .procname = "net", },
	{ .procname = "dccp", },
	{ .procname = "default", },
	{ }
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct ctl_table_header *dccp_table_header;

int __init dccp_sysctl_init(void)
{
<<<<<<< HEAD
	dccp_table_header = register_sysctl_paths(dccp_path,
=======
	dccp_table_header = register_net_sysctl(&init_net, "net/dccp/default",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			dccp_default_table);

	return dccp_table_header != NULL ? 0 : -ENOMEM;
}

void dccp_sysctl_exit(void)
{
	if (dccp_table_header != NULL) {
<<<<<<< HEAD
		unregister_sysctl_table(dccp_table_header);
=======
		unregister_net_sysctl_table(dccp_table_header);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dccp_table_header = NULL;
	}
}
