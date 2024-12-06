<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_SH_SMP_OPS_H
#define __ASM_SH_SMP_OPS_H

struct plat_smp_ops {
	void (*smp_setup)(void);
	unsigned int (*smp_processor_id)(void);
	void (*prepare_cpus)(unsigned int max_cpus);
	void (*start_cpu)(unsigned int cpu, unsigned long entry_point);
	void (*send_ipi)(unsigned int cpu, unsigned int message);
	int (*cpu_disable)(unsigned int cpu);
	void (*cpu_die)(unsigned int cpu);
	void (*play_dead)(void);
};

extern struct plat_smp_ops *mp_ops;
extern struct plat_smp_ops shx3_smp_ops;

#ifdef CONFIG_SMP

static inline void plat_smp_setup(void)
{
	BUG_ON(!mp_ops);
	mp_ops->smp_setup();
}

<<<<<<< HEAD
static inline void play_dead(void)
{
	mp_ops->play_dead();
=======
static inline void __noreturn play_dead(void)
{
	mp_ops->play_dead();
	BUG();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

extern void register_smp_ops(struct plat_smp_ops *ops);

#else

static inline void plat_smp_setup(void)
{
	/* UP, nothing to do ... */
}

static inline void register_smp_ops(struct plat_smp_ops *ops)
{
}

<<<<<<< HEAD
static inline void play_dead(void)
=======
static inline void __noreturn play_dead(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	BUG();
}

#endif /* CONFIG_SMP */

#endif /* __ASM_SH_SMP_OPS_H */
