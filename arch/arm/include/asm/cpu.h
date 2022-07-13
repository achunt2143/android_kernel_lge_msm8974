<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  arch/arm/include/asm/cpu.h
 *
 *  Copyright (C) 2004-2005 ARM Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __ASM_ARM_CPU_H
#define __ASM_ARM_CPU_H

#include <linux/percpu.h>
#include <linux/cpu.h>

struct cpuinfo_arm {
	struct cpu	cpu;
<<<<<<< HEAD
#ifdef CONFIG_SMP
	struct task_struct *idle;
=======
	u32		cpuid;
#ifdef CONFIG_SMP
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int	loops_per_jiffy;
#endif
};

DECLARE_PER_CPU(struct cpuinfo_arm, cpu_data);

#endif
