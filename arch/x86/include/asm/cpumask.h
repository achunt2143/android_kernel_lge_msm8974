<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_CPUMASK_H
#define _ASM_X86_CPUMASK_H
#ifndef __ASSEMBLY__
#include <linux/cpumask.h>

<<<<<<< HEAD
extern cpumask_var_t cpu_callin_mask;
extern cpumask_var_t cpu_callout_mask;
extern cpumask_var_t cpu_initialized_mask;
extern cpumask_var_t cpu_sibling_setup_mask;

extern void setup_cpu_local_masks(void);

=======
extern void setup_cpu_local_masks(void);

/*
 * NMI and MCE exceptions need cpu_is_offline() _really_ early,
 * provide an arch_ special for them to avoid instrumentation.
 */
#if NR_CPUS > 1
static __always_inline bool arch_cpu_online(int cpu)
{
	return arch_test_bit(cpu, cpumask_bits(cpu_online_mask));
}

static __always_inline void arch_cpumask_clear_cpu(int cpu, struct cpumask *dstp)
{
	arch_clear_bit(cpumask_check(cpu), cpumask_bits(dstp));
}
#else
static __always_inline bool arch_cpu_online(int cpu)
{
	return cpu == 0;
}

static __always_inline void arch_cpumask_clear_cpu(int cpu, struct cpumask *dstp)
{
	return;
}
#endif

#define arch_cpu_is_offline(cpu)	unlikely(!arch_cpu_online(cpu))

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASSEMBLY__ */
#endif /* _ASM_X86_CPUMASK_H */
