<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
**  Copyright (C) 2004-2007 Red Hat, Inc.  All rights reserved.
**
<<<<<<< HEAD
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
**
*******************************************************************************
******************************************************************************/

<<<<<<< HEAD
=======
#include <linux/module.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "dlm_internal.h"
#include "lockspace.h"
#include "lock.h"
#include "user.h"
#include "memory.h"
#include "config.h"
<<<<<<< HEAD
=======
#include "midcomms.h"

#define CREATE_TRACE_POINTS
#include <trace/events/dlm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init init_dlm(void)
{
	int error;

	error = dlm_memory_init();
	if (error)
		goto out;

<<<<<<< HEAD
=======
	dlm_midcomms_init();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = dlm_lockspace_init();
	if (error)
		goto out_mem;

	error = dlm_config_init();
	if (error)
		goto out_lockspace;

<<<<<<< HEAD
	error = dlm_register_debugfs();
	if (error)
		goto out_config;
=======
	dlm_register_debugfs();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	error = dlm_user_init();
	if (error)
		goto out_debug;

<<<<<<< HEAD
	error = dlm_netlink_init();
	if (error)
		goto out_user;

	error = dlm_plock_init();
	if (error)
		goto out_netlink;
=======
	error = dlm_plock_init();
	if (error)
		goto out_user;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	printk("DLM installed\n");

	return 0;

<<<<<<< HEAD
 out_netlink:
	dlm_netlink_exit();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 out_user:
	dlm_user_exit();
 out_debug:
	dlm_unregister_debugfs();
<<<<<<< HEAD
 out_config:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dlm_config_exit();
 out_lockspace:
	dlm_lockspace_exit();
 out_mem:
<<<<<<< HEAD
=======
	dlm_midcomms_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dlm_memory_exit();
 out:
	return error;
}

static void __exit exit_dlm(void)
{
	dlm_plock_exit();
<<<<<<< HEAD
	dlm_netlink_exit();
	dlm_user_exit();
	dlm_config_exit();
	dlm_memory_exit();
	dlm_lockspace_exit();
	dlm_unregister_debugfs();
=======
	dlm_user_exit();
	dlm_config_exit();
	dlm_lockspace_exit();
	dlm_midcomms_exit();
	dlm_unregister_debugfs();
	dlm_memory_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(init_dlm);
module_exit(exit_dlm);

MODULE_DESCRIPTION("Distributed Lock Manager");
MODULE_AUTHOR("Red Hat, Inc.");
MODULE_LICENSE("GPL");

EXPORT_SYMBOL_GPL(dlm_new_lockspace);
EXPORT_SYMBOL_GPL(dlm_release_lockspace);
EXPORT_SYMBOL_GPL(dlm_lock);
EXPORT_SYMBOL_GPL(dlm_unlock);

