<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *
 * arch/xtensa/platform-iss/setup.c
 *
 * Platform specific initialization.
 *
 * Authors: Chris Zankel <chris@zankel.net>
 *          Joe Taylor <joe@tensilica.com>
 *
 * Copyright 2001 - 2005 Tensilica Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/reboot.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/major.h>
#include <linux/blkdev.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/stringify.h>
#include <linux/notifier.h>

#include <asm/platform.h>
#include <asm/bootparam.h>


void __init platform_init(bp_tag_t* bootparam)
{

}

void platform_halt(void)
{
	printk (" ** Called platform_halt(), looping forever! **\n");
	while (1);
}

void platform_power_off(void)
{
	printk (" ** Called platform_power_off(), looping forever! **\n");
	while (1);
}
void platform_restart(void)
{
	/* Flush and reset the mmu, simulate a processor reset, and
	 * jump to the reset vector. */

	__asm__ __volatile__("movi	a2, 15\n\t"
			     "wsr	a2, " __stringify(ICOUNTLEVEL) "\n\t"
			     "movi	a2, 0\n\t"
			     "wsr	a2, " __stringify(ICOUNT) "\n\t"
			     "wsr	a2, " __stringify(IBREAKENABLE) "\n\t"
			     "wsr	a2, " __stringify(LCOUNT) "\n\t"
			     "movi	a2, 0x1f\n\t"
			     "wsr	a2, " __stringify(PS) "\n\t"
			     "isync\n\t"
			     "jx	%0\n\t"
			     :
			     : "a" (XCHAL_RESET_VECTOR_VADDR)
			     : "a2");

	/* control never gets here */
}

extern void iss_net_poll(void);

const char twirl[]="|/-\\|/-\\";

void platform_heartbeat(void)
{
#if 0
	static int i = 0, j = 0;

	if (--i < 0) {
		i = 99;
		printk("\r%c\r", twirl[j++]);
		if (j == 8)
			j = 0;
	}
#endif
}


=======
 * Copyright 2017 Cadence Design Systems Inc.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/panic_notifier.h>
#include <linux/printk.h>
#include <linux/reboot.h>
#include <linux/string.h>

#include <asm/platform.h>
#include <asm/setup.h>

#include <platform/simcall.h>


static int iss_power_off(struct sys_off_data *unused)
{
	pr_info(" ** Called platform_power_off() **\n");
	simc_exit(0);
	return NOTIFY_DONE;
}

static int iss_restart(struct notifier_block *this,
		       unsigned long event, void *ptr)
{
	/* Flush and reset the mmu, simulate a processor reset, and
	 * jump to the reset vector. */
	cpu_reset();

	return NOTIFY_DONE;
}

static struct notifier_block iss_restart_block = {
	.notifier_call = iss_restart,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int
iss_panic_event(struct notifier_block *this, unsigned long event, void *ptr)
{
<<<<<<< HEAD
	__asm__ __volatile__("movi a2, -1; simcall\n");
=======
	simc_exit(1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NOTIFY_DONE;
}

static struct notifier_block iss_panic_block = {
<<<<<<< HEAD
	iss_panic_event,
	NULL,
	0
=======
	.notifier_call = iss_panic_event,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

void __init platform_setup(char **p_cmdline)
{
<<<<<<< HEAD
	atomic_notifier_chain_register(&panic_notifier_list, &iss_panic_block);
=======
	static void *argv[COMMAND_LINE_SIZE / sizeof(void *)] __initdata;
	static char cmdline[COMMAND_LINE_SIZE] __initdata;
	int argc = simc_argc();
	int argv_size = simc_argv_size();

	if (argc > 1) {
		if (argv_size > sizeof(argv)) {
			pr_err("%s: command line too long: argv_size = %d\n",
			       __func__, argv_size);
		} else {
			int i;

			cmdline[0] = 0;
			simc_argv((void *)argv);

			for (i = 1; i < argc; ++i) {
				if (i > 1)
					strcat(cmdline, " ");
				strcat(cmdline, argv[i]);
			}
			*p_cmdline = cmdline;
		}
	}

	atomic_notifier_chain_register(&panic_notifier_list, &iss_panic_block);
	register_restart_handler(&iss_restart_block);
	register_sys_off_handler(SYS_OFF_MODE_POWER_OFF,
				 SYS_OFF_PRIO_PLATFORM,
				 iss_power_off, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
