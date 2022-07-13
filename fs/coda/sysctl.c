<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Sysctl operations for Coda filesystem
 * Original version: (C) 1996 P. Braam and M. Callahan
 * Rewritten for Linux 2.1. (C) 1997 Carnegie Mellon University
 * 
 * Carnegie Mellon encourages users to contribute improvements to
 * the Coda project. Contact Peter Braam (coda@cs.cmu.edu).
 */

#include <linux/sysctl.h>

#include "coda_int.h"

<<<<<<< HEAD
#ifdef CONFIG_SYSCTL
static struct ctl_table_header *fs_table_header;

static ctl_table coda_table[] = {
=======
static struct ctl_table_header *fs_table_header;

static struct ctl_table coda_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.procname	= "timeout",
		.data		= &coda_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "hard",
		.data		= &coda_hard,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "fake_statfs",
		.data		= &coda_fake_statfs,
		.maxlen		= sizeof(int),
		.mode		= 0600,
		.proc_handler	= proc_dointvec
	},
<<<<<<< HEAD
	{}
};

static ctl_table fs_table[] = {
	{
		.procname	= "coda",
		.mode		= 0555,
		.child		= coda_table
	},
	{}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

void coda_sysctl_init(void)
{
	if ( !fs_table_header )
<<<<<<< HEAD
		fs_table_header = register_sysctl_table(fs_table);
=======
		fs_table_header = register_sysctl("coda", coda_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void coda_sysctl_clean(void)
{
	if ( fs_table_header ) {
		unregister_sysctl_table(fs_table_header);
		fs_table_header = NULL;
	}
}
<<<<<<< HEAD

#else
void coda_sysctl_init(void)
{
}

void coda_sysctl_clean(void)
{
}
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
