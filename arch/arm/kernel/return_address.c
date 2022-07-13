<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/kernel/return_address.c
 *
 * Copyright (C) 2009 Uwe Kleine-Koenig <u.kleine-koenig@pengutronix.de>
 * for Pengutronix
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#include <linux/export.h>
#include <linux/ftrace.h>

#if defined(CONFIG_FRAME_POINTER) && !defined(CONFIG_ARM_UNWIND)
=======
 */
#include <linux/export.h>
#include <linux/ftrace.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sched.h>

#include <asm/stacktrace.h>

struct return_address_data {
	unsigned int level;
	void *addr;
};

<<<<<<< HEAD
static int save_return_addr(struct stackframe *frame, void *d)
=======
static bool save_return_addr(void *d, unsigned long pc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct return_address_data *data = d;

	if (!data->level) {
<<<<<<< HEAD
		data->addr = (void *)frame->lr;

		return 1;
	} else {
		--data->level;
		return 0;
=======
		data->addr = (void *)pc;

		return false;
	} else {
		--data->level;
		return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

void *return_address(unsigned int level)
{
	struct return_address_data data;
	struct stackframe frame;
<<<<<<< HEAD
	register unsigned long current_sp asm ("sp");

	data.level = level + 1;

	frame.fp = (unsigned long)__builtin_frame_address(0);
	frame.sp = current_sp;
	frame.lr = (unsigned long)__builtin_return_address(0);
	frame.pc = (unsigned long)return_address;
=======

	data.level = level + 2;
	data.addr = NULL;

	frame.fp = (unsigned long)__builtin_frame_address(0);
	frame.sp = current_stack_pointer;
	frame.lr = (unsigned long)__builtin_return_address(0);
here:
	frame.pc = (unsigned long)&&here;
#ifdef CONFIG_KRETPROBES
	frame.kr_cur = NULL;
	frame.tsk = current;
#endif
	frame.ex_frame = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	walk_stackframe(&frame, save_return_addr, &data);

	if (!data.level)
		return data.addr;
	else
		return NULL;
}

<<<<<<< HEAD
#else /* if defined(CONFIG_FRAME_POINTER) && !defined(CONFIG_ARM_UNWIND) */

void *return_address(unsigned int level)
{
	return NULL;
}

#endif /* if defined(CONFIG_FRAME_POINTER) && !defined(CONFIG_ARM_UNWIND) / else */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL_GPL(return_address);
