<<<<<<< HEAD
/*
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2006 Atmark Techno, Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ASM_MICROBLAZE_SWITCH_TO_H
#define _ASM_MICROBLAZE_SWITCH_TO_H

struct task_struct;
struct thread_info;

extern struct task_struct *_switch_to(struct thread_info *prev,
					struct thread_info *next);

#define switch_to(prev, next, last)					\
	do {								\
		(last) = _switch_to(task_thread_info(prev),		\
					task_thread_info(next));	\
	} while (0)

#endif /* _ASM_MICROBLAZE_SWITCH_TO_H */
