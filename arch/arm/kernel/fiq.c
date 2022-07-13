<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/kernel/fiq.c
 *
 *  Copyright (C) 1998 Russell King
 *  Copyright (C) 1998, 1999 Phil Blundell
 *
 *  FIQ support written by Philip Blundell <philb@gnu.org>, 1998.
 *
 *  FIQ support re-written by Russell King to be more generic
 *
 * We now properly support a method by which the FIQ handlers can
 * be stacked onto the vector.  We still do not support sharing
 * the FIQ vector itself.
 *
 * Operation is as follows:
 *  1. Owner A claims FIQ:
 *     - default_fiq relinquishes control.
 *  2. Owner A:
 *     - inserts code.
 *     - sets any registers,
 *     - enables FIQ.
 *  3. Owner B claims FIQ:
 *     - if owner A has a relinquish function.
 *       - disable FIQs.
 *       - saves any registers.
 *       - returns zero.
 *  4. Owner B:
 *     - inserts code.
 *     - sets any registers,
 *     - enables FIQ.
 *  5. Owner B releases FIQ:
 *     - Owner A is asked to reacquire FIQ:
 *	 - inserts code.
 *	 - restores saved registers.
 *	 - enables FIQ.
 *  6. Goto 3
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/irq.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/seq_file.h>

#include <asm/cacheflush.h>
#include <asm/cp15.h>
#include <asm/fiq.h>
<<<<<<< HEAD
=======
#include <asm/mach/irq.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/irq.h>
#include <asm/traps.h>

#define FIQ_OFFSET ({					\
		extern void *vector_fiq_offset;		\
		(unsigned)&vector_fiq_offset;		\
	})

<<<<<<< HEAD
static unsigned long no_fiq_insn;
=======
static unsigned long dfl_fiq_insn;
static struct pt_regs dfl_fiq_regs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Default reacquire function
 * - we always relinquish FIQ control
 * - we always reacquire FIQ control
 */
static int fiq_def_op(void *ref, int relinquish)
{
<<<<<<< HEAD
	if (!relinquish)
		set_fiq_handler(&no_fiq_insn, sizeof(no_fiq_insn));
=======
	if (!relinquish) {
		/* Restore default handler and registers */
		local_fiq_disable();
		set_fiq_regs(&dfl_fiq_regs);
		set_fiq_handler(&dfl_fiq_insn, sizeof(dfl_fiq_insn));
		local_fiq_enable();

		/* FIXME: notify irq controller to standard enable FIQs */
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static struct fiq_handler default_owner = {
	.name	= "default",
	.fiq_op = fiq_def_op,
};

static struct fiq_handler *current_fiq = &default_owner;

int show_fiq_list(struct seq_file *p, int prec)
{
	if (current_fiq != &default_owner)
		seq_printf(p, "%*s:              %s\n", prec, "FIQ",
			current_fiq->name);

	return 0;
}

void set_fiq_handler(void *start, unsigned int length)
{
	void *base = vectors_page;
	unsigned offset = FIQ_OFFSET;

	memcpy(base + offset, start, length);
	if (!cache_is_vipt_nonaliasing())
<<<<<<< HEAD
		flush_icache_range((unsigned long)base + offset, offset +
				   length);
=======
		flush_icache_range((unsigned long)base + offset,
				   (unsigned long)base + offset + length);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	flush_icache_range(0xffff0000 + offset, 0xffff0000 + offset + length);
}

int claim_fiq(struct fiq_handler *f)
{
	int ret = 0;

	if (current_fiq) {
		ret = -EBUSY;

		if (current_fiq->fiq_op != NULL)
			ret = current_fiq->fiq_op(current_fiq->dev_id, 1);
	}

	if (!ret) {
		f->next = current_fiq;
		current_fiq = f;
	}

	return ret;
}

void release_fiq(struct fiq_handler *f)
{
	if (current_fiq != f) {
<<<<<<< HEAD
		printk(KERN_ERR "%s FIQ trying to release %s FIQ\n",
=======
		pr_err("%s FIQ trying to release %s FIQ\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       f->name, current_fiq->name);
		dump_stack();
		return;
	}

	do
		current_fiq = current_fiq->next;
	while (current_fiq->fiq_op(current_fiq->dev_id, 0));
}

static int fiq_start;

void enable_fiq(int fiq)
{
	enable_irq(fiq + fiq_start);
}

void disable_fiq(int fiq)
{
	disable_irq(fiq + fiq_start);
}

<<<<<<< HEAD
void fiq_set_type(int fiq, unsigned int type)
{
	irq_set_irq_type(fiq + FIQ_START, type);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(set_fiq_handler);
EXPORT_SYMBOL(__set_fiq_regs);	/* defined in fiqasm.S */
EXPORT_SYMBOL(__get_fiq_regs);	/* defined in fiqasm.S */
EXPORT_SYMBOL(claim_fiq);
EXPORT_SYMBOL(release_fiq);
EXPORT_SYMBOL(enable_fiq);
EXPORT_SYMBOL(disable_fiq);
<<<<<<< HEAD
EXPORT_SYMBOL(fiq_set_type);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void __init init_FIQ(int start)
{
	unsigned offset = FIQ_OFFSET;
<<<<<<< HEAD
	no_fiq_insn = *(unsigned long *)(0xffff0000 + offset);
=======
	dfl_fiq_insn = *(unsigned long *)(0xffff0000 + offset);
	get_fiq_regs(&dfl_fiq_regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fiq_start = start;
}
