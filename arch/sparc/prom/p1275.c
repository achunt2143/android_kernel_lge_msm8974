<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * p1275.c: Sun IEEE 1275 PROM low level interface routines
 *
 * Copyright (C) 1996,1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/string.h>
#include <linux/spinlock.h>
<<<<<<< HEAD
=======
#include <linux/irqflags.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/openprom.h>
#include <asm/oplib.h>
#include <asm/spitfire.h>
#include <asm/pstate.h>
#include <asm/ldc.h>

struct {
	long prom_callback;			/* 0x00 */
	void (*prom_cif_handler)(long *);	/* 0x08 */
<<<<<<< HEAD
	unsigned long prom_cif_stack;		/* 0x10 */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} p1275buf;

extern void prom_world(int);

extern void prom_cif_direct(unsigned long *args);
extern void prom_cif_callback(void);

/*
 * This provides SMP safety on the p1275buf.
 */
DEFINE_RAW_SPINLOCK(prom_entry_lock);

void p1275_cmd_direct(unsigned long *args)
{
	unsigned long flags;

<<<<<<< HEAD
	raw_local_save_flags(flags);
	raw_local_irq_restore((unsigned long)PIL_NMI);
=======
	local_save_flags(flags);
	local_irq_restore((unsigned long)PIL_NMI);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	raw_spin_lock(&prom_entry_lock);

	prom_world(1);
	prom_cif_direct(args);
	prom_world(0);

	raw_spin_unlock(&prom_entry_lock);
<<<<<<< HEAD
	raw_local_irq_restore(flags);
=======
	local_irq_restore(flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void prom_cif_init(void *cif_handler, void *cif_stack)
{
	p1275buf.prom_cif_handler = (void (*)(long *))cif_handler;
<<<<<<< HEAD
	p1275buf.prom_cif_stack = (unsigned long)cif_stack;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
