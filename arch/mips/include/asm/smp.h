/*
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file "COPYING" in the main directory of this
 * archive for more details.
 *
 * Copyright (C) 2000 - 2001 by Kanoj Sarcar (kanoj@sgi.com)
 * Copyright (C) 2000 - 2001 by Silicon Graphics, Inc.
 * Copyright (C) 2000, 2001, 2002 Ralf Baechle
 * Copyright (C) 2000, 2001 Broadcom Corporation
 */
#ifndef __ASM_SMP_H
#define __ASM_SMP_H

<<<<<<< HEAD
#include <linux/bitops.h>
#include <linux/linkage.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/cpumask.h>

#include <linux/atomic.h>
=======
#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/threads.h>
#include <linux/cpumask.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/smp-ops.h>

extern int smp_num_siblings;
extern cpumask_t cpu_sibling_map[];
<<<<<<< HEAD

#define raw_smp_processor_id() (current_thread_info()->cpu)

/* Map from cpu id to sequential logical cpu number.  This will only
   not be idempotent when cpus failed to come on-line.  */
extern int __cpu_number_map[NR_CPUS];
=======
extern cpumask_t cpu_core_map[];
extern cpumask_t cpu_foreign_map[];

static inline int raw_smp_processor_id(void)
{
#if defined(__VDSO__)
	extern int vdso_smp_processor_id(void)
		__compiletime_error("VDSO should not call smp_processor_id()");
	return vdso_smp_processor_id();
#else
	return current_thread_info()->cpu;
#endif
}
#define raw_smp_processor_id raw_smp_processor_id

/* Map from cpu id to sequential logical cpu number.  This will only
   not be idempotent when cpus failed to come on-line.	*/
extern int __cpu_number_map[CONFIG_MIPS_NR_CPU_NR_MAP];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define cpu_number_map(cpu)  __cpu_number_map[cpu]

/* The reverse map from sequential logical cpu number to cpu id.  */
extern int __cpu_logical_map[NR_CPUS];
#define cpu_logical_map(cpu)  __cpu_logical_map[cpu]

#define NO_PROC_ID	(-1)

<<<<<<< HEAD
#define SMP_RESCHEDULE_YOURSELF	0x1	/* XXX braindead */
#define SMP_CALL_FUNCTION	0x2
/* Octeon - Tell another core to flush its icache */
#define SMP_ICACHE_FLUSH	0x4

extern volatile cpumask_t cpu_callin_map;

extern void asmlinkage smp_bootstrap(void);
=======
#define SMP_RESCHEDULE_YOURSELF 0x1	/* XXX braindead */
#define SMP_CALL_FUNCTION	0x2
/* Octeon - Tell another core to flush its icache */
#define SMP_ICACHE_FLUSH	0x4
#define SMP_ASK_C0COUNT		0x8

/* Mask of CPUs which are currently definitely operating coherently */
extern cpumask_t cpu_coherent_mask;

extern unsigned int smp_max_threads __initdata;

extern asmlinkage void smp_bootstrap(void);

extern void calculate_cpu_foreign_map(void);

asmlinkage void start_secondary(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * this function sends a 'reschedule' IPI to another CPU.
 * it goes straight through and wastes no time serializing
 * anything. Worst case is that we lose a reschedule ...
 */
<<<<<<< HEAD
static inline void smp_send_reschedule(int cpu)
{
	extern struct plat_smp_ops *mp_ops;	/* private */
=======
static inline void arch_smp_send_reschedule(int cpu)
{
	extern const struct plat_smp_ops *mp_ops;	/* private */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mp_ops->send_ipi_single(cpu, SMP_RESCHEDULE_YOURSELF);
}

#ifdef CONFIG_HOTPLUG_CPU
static inline int __cpu_disable(void)
{
<<<<<<< HEAD
	extern struct plat_smp_ops *mp_ops;     /* private */
=======
	extern const struct plat_smp_ops *mp_ops;	/* private */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return mp_ops->cpu_disable();
}

static inline void __cpu_die(unsigned int cpu)
{
<<<<<<< HEAD
	extern struct plat_smp_ops *mp_ops;     /* private */
=======
	extern const struct plat_smp_ops *mp_ops;	/* private */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mp_ops->cpu_die(cpu);
}

<<<<<<< HEAD
extern void play_dead(void);
#endif

extern asmlinkage void smp_call_function_interrupt(void);

static inline void arch_send_call_function_single_ipi(int cpu)
{
	extern struct plat_smp_ops *mp_ops;     /* private */

	mp_ops->send_ipi_mask(&cpumask_of_cpu(cpu), SMP_CALL_FUNCTION);
=======
extern void __noreturn play_dead(void);
#endif

#ifdef CONFIG_KEXEC_CORE
static inline void kexec_nonboot_cpu(void)
{
	extern const struct plat_smp_ops *mp_ops;	/* private */

	return mp_ops->kexec_nonboot_cpu();
}

static inline void *kexec_nonboot_cpu_func(void)
{
	extern const struct plat_smp_ops *mp_ops;	/* private */

	return mp_ops->kexec_nonboot_cpu;
}
#endif

/*
 * This function will set up the necessary IPIs for Linux to communicate
 * with the CPUs in mask.
 * Return 0 on success.
 */
int mips_smp_ipi_allocate(const struct cpumask *mask);

/*
 * This function will free up IPIs allocated with mips_smp_ipi_allocate to the
 * CPUs in mask, which must be a subset of the IPIs that have been configured.
 * Return 0 on success.
 */
int mips_smp_ipi_free(const struct cpumask *mask);

static inline void arch_send_call_function_single_ipi(int cpu)
{
	extern const struct plat_smp_ops *mp_ops;	/* private */

	mp_ops->send_ipi_single(cpu, SMP_CALL_FUNCTION);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
<<<<<<< HEAD
	extern struct plat_smp_ops *mp_ops;     /* private */
=======
	extern const struct plat_smp_ops *mp_ops;	/* private */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mp_ops->send_ipi_mask(mask, SMP_CALL_FUNCTION);
}

#endif /* __ASM_SMP_H */
