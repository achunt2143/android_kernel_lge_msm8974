<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Performance event callchain support - SuperH architecture code
 *
 * Copyright (C) 2009  Paul Mundt
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/perf_event.h>
#include <linux/percpu.h>
#include <asm/unwinder.h>
#include <asm/ptrace.h>

<<<<<<< HEAD
static int callchain_stack(void *data, char *name)
{
	return 0;
}

static void callchain_address(void *data, unsigned long addr, int reliable)
{
	struct perf_callchain_entry *entry = data;
=======
static void callchain_address(void *data, unsigned long addr, int reliable)
{
	struct perf_callchain_entry_ctx *entry = data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (reliable)
		perf_callchain_store(entry, addr);
}

static const struct stacktrace_ops callchain_ops = {
<<<<<<< HEAD
	.stack		= callchain_stack,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.address	= callchain_address,
};

void
<<<<<<< HEAD
perf_callchain_kernel(struct perf_callchain_entry *entry, struct pt_regs *regs)
=======
perf_callchain_kernel(struct perf_callchain_entry_ctx *entry, struct pt_regs *regs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	perf_callchain_store(entry, regs->pc);

	unwind_stack(NULL, regs, NULL, &callchain_ops, entry);
}
