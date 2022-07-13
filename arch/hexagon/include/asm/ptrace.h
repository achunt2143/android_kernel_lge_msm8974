<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Ptrace definitions for the Hexagon architecture
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef _ASM_PTRACE_H
#define _ASM_PTRACE_H

#include <asm/registers.h>

#define instruction_pointer(regs) pt_elr(regs)
#define user_stack_pointer(regs) ((regs)->r29)

#define profile_pc(regs) instruction_pointer(regs)
=======
 */

#ifndef _ASM_HEXAGON_PTRACE_H
#define _ASM_HEXAGON_PTRACE_H

#include <uapi/asm/ptrace.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* kprobe-based event tracer support */
extern int regs_query_register_offset(const char *name);
extern const char *regs_query_register_name(unsigned int offset);

<<<<<<< HEAD
=======
#define current_pt_regs() \
	((struct pt_regs *) \
	 ((unsigned long)current_thread_info() + THREAD_SIZE) - 1)

#if CONFIG_HEXAGON_ARCH_VERSION >= 4
#define arch_has_single_step()	(1)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
