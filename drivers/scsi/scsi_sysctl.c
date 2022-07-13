<<<<<<< HEAD
/*
 * Copyright (C) 2003 Christoph Hellwig.
 *	Released under GPL v2.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2003 Christoph Hellwig.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sysctl.h>

#include "scsi_logging.h"
#include "scsi_priv.h"


<<<<<<< HEAD
static ctl_table scsi_table[] = {
=======
static struct ctl_table scsi_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ .procname	= "logging_level",
	  .data		= &scsi_logging_level,
	  .maxlen	= sizeof(scsi_logging_level),
	  .mode		= 0644,
	  .proc_handler	= proc_dointvec },
<<<<<<< HEAD
	{ }
};

static ctl_table scsi_dir_table[] = {
	{ .procname	= "scsi",
	  .mode		= 0555,
	  .child	= scsi_table },
	{ }
};

static ctl_table scsi_root_table[] = {
	{ .procname	= "dev",
	  .mode		= 0555,
	  .child	= scsi_dir_table },
	{ }
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct ctl_table_header *scsi_table_header;

int __init scsi_init_sysctl(void)
{
<<<<<<< HEAD
	scsi_table_header = register_sysctl_table(scsi_root_table);
=======
	scsi_table_header = register_sysctl("dev/scsi", scsi_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!scsi_table_header)
		return -ENOMEM;
	return 0;
}

void scsi_exit_sysctl(void)
{
	unregister_sysctl_table(scsi_table_header);
}
