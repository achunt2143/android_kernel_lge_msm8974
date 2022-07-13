/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994, 1995, 1996, 1999, 2000 by Ralf Baechle
 * Copyright (C) 1999, 2000 by Silicon Graphics
 * Copyright (C) 2002  Maciej W. Rozycki
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <asm/traps.h>
#include <asm/uaccess.h>
=======
#include <linux/sched/debug.h>
#include <linux/sched/signal.h>
#include <asm/traps.h>
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/addrspace.h>
#include <asm/ptrace.h>
#include <asm/tlbdebug.h>

<<<<<<< HEAD
=======
#include "ip32-common.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ip32_be_handler(struct pt_regs *regs, int is_fixup)
{
	int data = regs->cp0_cause & 4;

	if (is_fixup)
		return MIPS_BE_FIXUP;

	printk("Got %cbe at 0x%lx\n", data ? 'd' : 'i', regs->cp0_epc);
	show_regs(regs);
	dump_tlb_all();
	while(1);
<<<<<<< HEAD
	force_sig(SIGBUS, current);
=======
	force_sig(SIGBUS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init ip32_be_init(void)
{
<<<<<<< HEAD
	board_be_handler = ip32_be_handler;
=======
	mips_set_be_handler(ip32_be_handler);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
