<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/arch/sh/mm/extable.c
 *  Taken from:
 *   linux/arch/i386/mm/extable.c
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <asm/uaccess.h>
=======
#include <linux/extable.h>
#include <linux/uaccess.h>

#include <asm/ptrace.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int fixup_exception(struct pt_regs *regs)
{
	const struct exception_table_entry *fixup;

	fixup = search_exception_tables(regs->pc);
	if (fixup) {
		regs->pc = fixup->fixup;
		return 1;
	}

	return 0;
}
