<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/include/asm/perf_event.h
 *
 *  Copyright (C) 2009 picoChip Designs Ltd, Jamie Iles
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ARM_PERF_EVENT_H__
#define __ARM_PERF_EVENT_H__

<<<<<<< HEAD
/* ARM perf PMU IDs for use by internal perf clients. */
enum arm_perf_pmu_ids {
	ARM_PERF_PMU_ID_XSCALE1	= 0,
	ARM_PERF_PMU_ID_XSCALE2,
	ARM_PERF_PMU_ID_V6,
	ARM_PERF_PMU_ID_V6MP,
	ARM_PERF_PMU_ID_CA8,
	ARM_PERF_PMU_ID_CA9,
	ARM_PERF_PMU_ID_CA5,
	ARM_PERF_PMU_ID_CA15,
	ARM_PERF_PMU_ID_L2X0,
	ARM_PERF_PMU_ID_CA7,
	ARM_PERF_PMU_ID_SCORPION,
	ARM_PERF_PMU_ID_SCORPIONMP,
	ARM_PERF_PMU_ID_SCORPIONMP_L2,
	ARM_PERF_PMU_ID_KRAIT,
	ARM_PERF_PMU_ID_KRAIT_L2,
	ARM_NUM_PMU_IDS,
};

extern enum arm_perf_pmu_ids
armpmu_get_pmu_id(void);
=======
#ifdef CONFIG_PERF_EVENTS
struct pt_regs;
extern unsigned long perf_instruction_pointer(struct pt_regs *regs);
extern unsigned long perf_misc_flags(struct pt_regs *regs);
#define perf_misc_flags(regs)	perf_misc_flags(regs)
#endif

#define perf_arch_fetch_caller_regs(regs, __ip) { \
	(regs)->ARM_pc = (__ip); \
	frame_pointer((regs)) = (unsigned long) __builtin_frame_address(0); \
	(regs)->ARM_sp = current_stack_pointer; \
	(regs)->ARM_cpsr = SVC_MODE; \
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ARM_PERF_EVENT_H__ */
