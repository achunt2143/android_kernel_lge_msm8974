/*
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/module.h>
#include <linux/sched.h>
=======
#include <linux/cpu.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/debug_locks.h>

#include <asm/exceptions.h>
#include <asm/unwind.h>

void trap_init(void)
{
	__enable_hw_exceptions();
}

static unsigned long kstack_depth_to_print;	/* 0 == entire stack */

static int __init kstack_setup(char *s)
{
<<<<<<< HEAD
	return !strict_strtoul(s, 0, &kstack_depth_to_print);
}
__setup("kstack=", kstack_setup);

void show_stack(struct task_struct *task, unsigned long *sp)
=======
	return !kstrtoul(s, 0, &kstack_depth_to_print);
}
__setup("kstack=", kstack_setup);

void show_stack(struct task_struct *task, unsigned long *sp, const char *loglvl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long words_to_show;
	u32 fp = (u32) sp;

	if (fp == 0) {
		if (task) {
			fp = ((struct thread_info *)
				(task->stack))->cpu_context.r1;
		} else {
			/* Pick up caller of dump_stack() */
			fp = (u32)&sp - 8;
		}
	}

	words_to_show = (THREAD_SIZE - (fp & (THREAD_SIZE - 1))) >> 2;
	if (kstack_depth_to_print && (words_to_show > kstack_depth_to_print))
		words_to_show = kstack_depth_to_print;

<<<<<<< HEAD
	pr_info("Kernel Stack:\n");
=======
	printk("%sKernel Stack:\n", loglvl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Make the first line an 'odd' size if necessary to get
	 * remaining lines to start at an address multiple of 0x10
	 */
	if (fp & 0xF) {
		unsigned long line1_words = (0x10 - (fp & 0xF)) >> 2;
		if (line1_words < words_to_show) {
			print_hex_dump(KERN_INFO, "", DUMP_PREFIX_ADDRESS, 32,
				       4, (void *)fp, line1_words << 2, 0);
			fp += line1_words << 2;
			words_to_show -= line1_words;
		}
	}
<<<<<<< HEAD
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_ADDRESS, 32, 4, (void *)fp,
		       words_to_show << 2, 0);
	printk(KERN_INFO "\n\n");

	pr_info("Call Trace:\n");
	microblaze_unwind(task, NULL);
	pr_info("\n");
=======
	print_hex_dump(loglvl, "", DUMP_PREFIX_ADDRESS, 32, 4, (void *)fp,
		       words_to_show << 2, 0);
	printk("%s\n\nCall Trace:\n", loglvl);
	microblaze_unwind(task, NULL, loglvl);
	printk("%s\n", loglvl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!task)
		task = current;

	debug_show_held_locks(task);
}
<<<<<<< HEAD

void dump_stack(void)
{
	show_stack(NULL, NULL);
}
EXPORT_SYMBOL(dump_stack);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
