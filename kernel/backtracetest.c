<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Simple stack backtrace regression test module
 *
 * (C) Copyright 2008 Intel Corporation
 * Author: Arjan van de Ven <arjan@linux.intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/stacktrace.h>

static void backtrace_test_normal(void)
{
<<<<<<< HEAD
	printk("Testing a backtrace from process context.\n");
	printk("The following trace is a kernel self test and not a bug!\n");
=======
	pr_info("Testing a backtrace from process context.\n");
	pr_info("The following trace is a kernel self test and not a bug!\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dump_stack();
}

<<<<<<< HEAD
static DECLARE_COMPLETION(backtrace_work);

static void backtrace_test_irq_callback(unsigned long data)
{
	dump_stack();
	complete(&backtrace_work);
}

static DECLARE_TASKLET(backtrace_tasklet, &backtrace_test_irq_callback, 0);

static void backtrace_test_irq(void)
{
	printk("Testing a backtrace from irq context.\n");
	printk("The following trace is a kernel self test and not a bug!\n");

	init_completion(&backtrace_work);
	tasklet_schedule(&backtrace_tasklet);
	wait_for_completion(&backtrace_work);
=======
static void backtrace_test_bh_workfn(struct work_struct *work)
{
	dump_stack();
}

static DECLARE_WORK(backtrace_bh_work, &backtrace_test_bh_workfn);

static void backtrace_test_bh(void)
{
	pr_info("Testing a backtrace from BH context.\n");
	pr_info("The following trace is a kernel self test and not a bug!\n");

	queue_work(system_bh_wq, &backtrace_bh_work);
	flush_work(&backtrace_bh_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_STACKTRACE
static void backtrace_test_saved(void)
{
<<<<<<< HEAD
	struct stack_trace trace;
	unsigned long entries[8];

	printk("Testing a saved backtrace.\n");
	printk("The following trace is a kernel self test and not a bug!\n");

	trace.nr_entries = 0;
	trace.max_entries = ARRAY_SIZE(entries);
	trace.entries = entries;
	trace.skip = 0;

	save_stack_trace(&trace);
	print_stack_trace(&trace, 0);
=======
	unsigned long entries[8];
	unsigned int nr_entries;

	pr_info("Testing a saved backtrace.\n");
	pr_info("The following trace is a kernel self test and not a bug!\n");

	nr_entries = stack_trace_save(entries, ARRAY_SIZE(entries), 0);
	stack_trace_print(entries, nr_entries, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#else
static void backtrace_test_saved(void)
{
<<<<<<< HEAD
	printk("Saved backtrace test skipped.\n");
=======
	pr_info("Saved backtrace test skipped.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

static int backtrace_regression_test(void)
{
<<<<<<< HEAD
	printk("====[ backtrace testing ]===========\n");

	backtrace_test_normal();
	backtrace_test_irq();
	backtrace_test_saved();

	printk("====[ end of backtrace testing ]====\n");
=======
	pr_info("====[ backtrace testing ]===========\n");

	backtrace_test_normal();
	backtrace_test_bh();
	backtrace_test_saved();

	pr_info("====[ end of backtrace testing ]====\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void exitf(void)
{
}

module_init(backtrace_regression_test);
module_exit(exitf);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arjan van de Ven <arjan@linux.intel.com>");
