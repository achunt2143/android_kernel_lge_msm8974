<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * FIPS 200 support.
 *
 * Copyright (c) 2008 Neil Horman <nhorman@tuxdriver.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include "internal.h"
=======
 */

#include <linux/export.h>
#include <linux/fips.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysctl.h>
#include <linux/notifier.h>
#include <generated/utsrelease.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int fips_enabled;
EXPORT_SYMBOL_GPL(fips_enabled);

<<<<<<< HEAD
static int fips_error;

void set_fips_error(void)
{
	fips_error = 1;
}

int get_fips_error_state(void)
{
	return fips_error;
}
EXPORT_SYMBOL_GPL(get_fips_error_state);

static int cc_mode;

int get_cc_mode_state(void)
{
	return cc_mode;
}
EXPORT_SYMBOL_GPL(get_cc_mode_state);

void fips_init_proc()
{
	crypto_init_proc(&fips_error, &cc_mode);
}
=======
ATOMIC_NOTIFIER_HEAD(fips_fail_notif_chain);
EXPORT_SYMBOL_GPL(fips_fail_notif_chain);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Process kernel command-line parameter at boot time. fips=0 or fips=1 */
static int fips_enable(char *str)
{
	fips_enabled = !!simple_strtol(str, NULL, 0);
<<<<<<< HEAD
	cc_mode = !!simple_strtol(str, NULL, 0);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	printk(KERN_INFO "fips mode: %s\n",
		fips_enabled ? "enabled" : "disabled");
	return 1;
}

<<<<<<< HEAD
static int cc_mode_enable(char *str)
{
	fips_enabled = !!simple_strtol(str, NULL, 0);
	cc_mode = !!simple_strtol(str, NULL, 0);
	printk(KERN_INFO "cc_mode: %s\n",
		cc_mode ? "enabled" : "disabled");
	return 1;
}

__setup("fips=", fips_enable);
__setup("cc_mode=", cc_mode_enable);
=======
__setup("fips=", fips_enable);

#define FIPS_MODULE_NAME CONFIG_CRYPTO_FIPS_NAME
#ifdef CONFIG_CRYPTO_FIPS_CUSTOM_VERSION
#define FIPS_MODULE_VERSION CONFIG_CRYPTO_FIPS_VERSION
#else
#define FIPS_MODULE_VERSION UTS_RELEASE
#endif

static char fips_name[] = FIPS_MODULE_NAME;
static char fips_version[] = FIPS_MODULE_VERSION;

static struct ctl_table crypto_sysctl_table[] = {
	{
		.procname	= "fips_enabled",
		.data		= &fips_enabled,
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "fips_name",
		.data		= &fips_name,
		.maxlen		= 64,
		.mode		= 0444,
		.proc_handler	= proc_dostring
	},
	{
		.procname	= "fips_version",
		.data		= &fips_version,
		.maxlen		= 64,
		.mode		= 0444,
		.proc_handler	= proc_dostring
	},
	{}
};

static struct ctl_table_header *crypto_sysctls;

static void crypto_proc_fips_init(void)
{
	crypto_sysctls = register_sysctl("crypto", crypto_sysctl_table);
}

static void crypto_proc_fips_exit(void)
{
	unregister_sysctl_table(crypto_sysctls);
}

void fips_fail_notify(void)
{
	if (fips_enabled)
		atomic_notifier_call_chain(&fips_fail_notif_chain, 0, NULL);
}
EXPORT_SYMBOL_GPL(fips_fail_notify);

static int __init fips_init(void)
{
	crypto_proc_fips_init();
	return 0;
}

static void __exit fips_exit(void)
{
	crypto_proc_fips_exit();
}

subsys_initcall(fips_init);
module_exit(fips_exit);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
