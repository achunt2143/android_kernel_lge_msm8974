<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Smp support for CHRP machines.
 *
 * Written by Cort Dougan (cort@cs.nmt.edu) borrowing a great
 * deal of code from the sparc and intel versions.
 *
 * Copyright (C) 1999 Cort Dougan <cort@cs.nmt.edu>
 *
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/spinlock.h>
=======
#include <linux/spinlock.h>
#include <linux/pgtable.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/ptrace.h>
#include <linux/atomic.h>
#include <asm/irq.h>
#include <asm/page.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
#include <asm/sections.h>
#include <asm/io.h>
#include <asm/prom.h>
=======
#include <asm/sections.h>
#include <asm/io.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/smp.h>
#include <asm/machdep.h>
#include <asm/mpic.h>
#include <asm/rtas.h>

<<<<<<< HEAD
static int __devinit smp_chrp_kick_cpu(int nr)
=======
static int smp_chrp_kick_cpu(int nr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	*(unsigned long *)KERNELBASE = nr;
	asm volatile("dcbf 0,%0"::"r"(KERNELBASE):"memory");

	return 0;
}

<<<<<<< HEAD
static void __devinit smp_chrp_setup_cpu(int cpu_nr)
=======
static void smp_chrp_setup_cpu(int cpu_nr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	mpic_setup_this_cpu();
}

/* CHRP with openpic */
struct smp_ops_t chrp_smp_ops = {
<<<<<<< HEAD
=======
	.cause_nmi_ipi = NULL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.message_pass = smp_mpic_message_pass,
	.probe = smp_mpic_probe,
	.kick_cpu = smp_chrp_kick_cpu,
	.setup_cpu = smp_chrp_setup_cpu,
	.give_timebase = rtas_give_timebase,
	.take_timebase = rtas_take_timebase,
};
