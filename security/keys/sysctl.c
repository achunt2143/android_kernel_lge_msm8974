<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Key management controls
 *
 * Copyright (C) 2008 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/key.h>
#include <linux/sysctl.h>
#include "internal.h"

<<<<<<< HEAD
static const int zero, one = 1, max = INT_MAX;

ctl_table key_sysctls[] = {
=======
static struct ctl_table key_sysctls[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.procname = "maxkeys",
		.data = &key_quota_maxkeys,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1 = (void *) &one,
		.extra2 = (void *) &max,
=======
		.extra1 = (void *) SYSCTL_ONE,
		.extra2 = (void *) SYSCTL_INT_MAX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname = "maxbytes",
		.data = &key_quota_maxbytes,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1 = (void *) &one,
		.extra2 = (void *) &max,
=======
		.extra1 = (void *) SYSCTL_ONE,
		.extra2 = (void *) SYSCTL_INT_MAX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname = "root_maxkeys",
		.data = &key_quota_root_maxkeys,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1 = (void *) &one,
		.extra2 = (void *) &max,
=======
		.extra1 = (void *) SYSCTL_ONE,
		.extra2 = (void *) SYSCTL_INT_MAX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname = "root_maxbytes",
		.data = &key_quota_root_maxbytes,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1 = (void *) &one,
		.extra2 = (void *) &max,
=======
		.extra1 = (void *) SYSCTL_ONE,
		.extra2 = (void *) SYSCTL_INT_MAX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname = "gc_delay",
		.data = &key_gc_delay,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1 = (void *) &zero,
		.extra2 = (void *) &max,
	},
	{ }
};
=======
		.extra1 = (void *) SYSCTL_ZERO,
		.extra2 = (void *) SYSCTL_INT_MAX,
	},
#ifdef CONFIG_PERSISTENT_KEYRINGS
	{
		.procname = "persistent_keyring_expiry",
		.data = &persistent_keyring_expiry,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
		.extra1 = (void *) SYSCTL_ZERO,
		.extra2 = (void *) SYSCTL_INT_MAX,
	},
#endif
	{ }
};

static int __init init_security_keys_sysctls(void)
{
	register_sysctl_init("kernel/keys", key_sysctls);
	return 0;
}
early_initcall(init_security_keys_sysctls);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
