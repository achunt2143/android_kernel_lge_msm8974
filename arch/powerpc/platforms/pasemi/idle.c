<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2006-2007 PA Semi, Inc
 *
 * Maintained by: Olof Johansson <olof@lixom.net>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#undef DEBUG

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/irq.h>

#include <asm/machdep.h>
#include <asm/reg.h>
#include <asm/smp.h>

#include "pasemi.h"

struct sleep_mode {
	char *name;
	void (*entry)(void);
};

static struct sleep_mode modes[] = {
	{ .name = "spin", .entry = &idle_spin },
	{ .name = "doze", .entry = &idle_doze },
};

static int current_mode = 0;

static int pasemi_system_reset_exception(struct pt_regs *regs)
{
	/* If we were woken up from power savings, we need to return
	 * to the calling function, since nip is not saved across
	 * all modes.
	 */

	if (regs->msr & SRR1_WAKEMASK)
<<<<<<< HEAD
		regs->nip = regs->link;

	switch (regs->msr & SRR1_WAKEMASK) {
	case SRR1_WAKEEE:
		do_IRQ(regs);
		break;
	case SRR1_WAKEDEC:
		timer_interrupt(regs);
=======
		regs_set_return_ip(regs, regs->link);

	switch (regs->msr & SRR1_WAKEMASK) {
	case SRR1_WAKEDEC:
		set_dec(1);
		break;
	case SRR1_WAKEEE:
		/*
		 * Handle these when interrupts get re-enabled and we take
		 * them as regular exceptions. We are in an NMI context
		 * and can't handle these here.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		/* do system reset */
		return 0;
	}

	/* Set higher astate since we come out of power savings at 0 */
	restore_astate(hard_smp_processor_id());

	/* everything handled */
<<<<<<< HEAD
	regs->msr |= MSR_RI;
=======
	regs_set_recoverable(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}

static int __init pasemi_idle_init(void)
{
#ifndef CONFIG_PPC_PASEMI_CPUFREQ
<<<<<<< HEAD
	printk(KERN_WARNING "No cpufreq driver, powersavings modes disabled\n");
=======
	pr_warn("No cpufreq driver, powersavings modes disabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	current_mode = 0;
#endif

	ppc_md.system_reset_exception = pasemi_system_reset_exception;
	ppc_md.power_save = modes[current_mode].entry;
<<<<<<< HEAD
	printk(KERN_INFO "Using PA6T idle loop (%s)\n", modes[current_mode].name);
=======
	pr_info("Using PA6T idle loop (%s)\n", modes[current_mode].name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
machine_late_initcall(pasemi, pasemi_idle_init);

static int __init idle_param(char *p)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(modes); i++) {
		if (!strcmp(modes[i].name, p)) {
			current_mode = i;
			break;
		}
	}
	return 0;
}

early_param("idle", idle_param);
