<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __LINUX_SMP_H
#define __LINUX_SMP_H

/*
 *	Generic SMP support
 *		Alan Cox. <alan@redhat.com>
 */

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/cpumask.h>
#include <linux/init.h>
<<<<<<< HEAD

extern void cpu_idle(void);

typedef void (*smp_call_func_t)(void *info);
struct call_single_data {
	struct list_head list;
	smp_call_func_t func;
	void *info;
	u16 flags;
	u16 priv;
};

=======
#include <linux/smp_types.h>

typedef void (*smp_call_func_t)(void *info);
typedef bool (*smp_cond_func_t)(int cpu, void *info);

/*
 * structure shares (partial) layout with struct irq_work
 */
struct __call_single_data {
	struct __call_single_node node;
	smp_call_func_t func;
	void *info;
};

#define CSD_INIT(_func, _info) \
	(struct __call_single_data){ .func = (_func), .info = (_info), }

/* Use __aligned() to avoid to use 2 cache lines for 1 csd */
typedef struct __call_single_data call_single_data_t
	__aligned(sizeof(struct __call_single_data));

#define INIT_CSD(_csd, _func, _info)		\
do {						\
	*(_csd) = CSD_INIT((_func), (_info));	\
} while (0)

/*
 * Enqueue a llist_node on the call_single_queue; be very careful, read
 * flush_smp_call_function_queue() in detail.
 */
extern void __smp_call_single_queue(int cpu, struct llist_node *node);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* total number of cpus in this system (may exceed NR_CPUS) */
extern unsigned int total_cpus;

int smp_call_function_single(int cpuid, smp_call_func_t func, void *info,
			     int wait);

<<<<<<< HEAD
#ifdef CONFIG_SMP

#include <linux/preempt.h>
#include <linux/kernel.h>
=======
void on_each_cpu_cond_mask(smp_cond_func_t cond_func, smp_call_func_t func,
			   void *info, bool wait, const struct cpumask *mask);

int smp_call_function_single_async(int cpu, call_single_data_t *csd);

/*
 * Cpus stopping functions in panic. All have default weak definitions.
 * Architecture-dependent code may override them.
 */
void __noreturn panic_smp_self_stop(void);
void __noreturn nmi_panic_self_stop(struct pt_regs *regs);
void crash_smp_send_stop(void);

/*
 * Call a function on all processors
 */
static inline void on_each_cpu(smp_call_func_t func, void *info, int wait)
{
	on_each_cpu_cond_mask(NULL, func, info, wait, cpu_online_mask);
}

/**
 * on_each_cpu_mask(): Run a function on processors specified by
 * cpumask, which may include the local processor.
 * @mask: The set of cpus to run on (only runs on online subset).
 * @func: The function to run. This must be fast and non-blocking.
 * @info: An arbitrary pointer to pass to the function.
 * @wait: If true, wait (atomically) until function has completed
 *        on other CPUs.
 *
 * If @wait is true, then returns once @func has returned.
 *
 * You must not call this function with disabled interrupts or from a
 * hardware interrupt handler or from a bottom half handler.  The
 * exception is that it may be used during early boot while
 * early_boot_irqs_disabled is set.
 */
static inline void on_each_cpu_mask(const struct cpumask *mask,
				    smp_call_func_t func, void *info, bool wait)
{
	on_each_cpu_cond_mask(NULL, func, info, wait, mask);
}

/*
 * Call a function on each processor for which the supplied function
 * cond_func returns a positive value. This may include the local
 * processor.  May be used during early boot while early_boot_irqs_disabled is
 * set. Use local_irq_save/restore() instead of local_irq_disable/enable().
 */
static inline void on_each_cpu_cond(smp_cond_func_t cond_func,
				    smp_call_func_t func, void *info, bool wait)
{
	on_each_cpu_cond_mask(cond_func, func, info, wait, cpu_online_mask);
}

/*
 * Architecture specific boot CPU setup.  Defined as empty weak function in
 * init/main.c. Architectures can override it.
 */
void smp_prepare_boot_cpu(void);

#ifdef CONFIG_SMP

#include <linux/preempt.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/compiler.h>
#include <linux/thread_info.h>
#include <asm/smp.h>

/*
 * main cross-CPU interfaces, handles INIT, TLB flush, STOP, etc.
 * (defined in asm header):
 */

/*
 * stops all CPUs but the current one:
 */
extern void smp_send_stop(void);

/*
 * sends a 'reschedule' event to another CPU:
 */
<<<<<<< HEAD
extern void smp_send_reschedule(int cpu);

=======
extern void arch_smp_send_reschedule(int cpu);
/*
 * scheduler_ipi() is inline so can't be passed as callback reason, but the
 * callsite IP should be sufficient for root-causing IPIs sent from here.
 */
#define smp_send_reschedule(cpu) ({		  \
	trace_ipi_send_cpu(cpu, _RET_IP_, NULL);  \
	arch_smp_send_reschedule(cpu);		  \
})
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Prepare machine for booting other CPUs.
 */
extern void smp_prepare_cpus(unsigned int max_cpus);

/*
 * Bring a CPU up
 */
<<<<<<< HEAD
extern int __cpu_up(unsigned int cpunum);
=======
extern int __cpu_up(unsigned int cpunum, struct task_struct *tidle);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Final polishing of CPUs
 */
extern void smp_cpus_done(unsigned int max_cpus);

/*
 * Call a function on all other processors
 */
<<<<<<< HEAD
int smp_call_function(smp_call_func_t func, void *info, int wait);
void smp_call_function_many(const struct cpumask *mask,
			    smp_call_func_t func, void *info, bool wait);

void __smp_call_function_single(int cpuid, struct call_single_data *data,
				int wait);

int smp_call_function_any(const struct cpumask *mask,
			  smp_call_func_t func, void *info, int wait);

/*
 * Generic and arch helpers
 */
#ifdef CONFIG_USE_GENERIC_SMP_HELPERS
void __init call_function_init(void);
void generic_smp_call_function_single_interrupt(void);
void generic_smp_call_function_interrupt(void);
void ipi_call_lock(void);
void ipi_call_unlock(void);
void ipi_call_lock_irq(void);
void ipi_call_unlock_irq(void);
#else
static inline void call_function_init(void) { }
#endif

/*
 * Call a function on all processors
 */
int on_each_cpu(smp_call_func_t func, void *info, int wait);

/*
 * Call a function on processors specified by mask, which might include
 * the local one.
 */
void on_each_cpu_mask(const struct cpumask *mask, smp_call_func_t func,
		void *info, bool wait);

/*
 * Call a function on each processor for which the supplied function
 * cond_func returns a positive value. This may include the local
 * processor.
 */
void on_each_cpu_cond(bool (*cond_func)(int cpu, void *info),
		smp_call_func_t func, void *info, bool wait,
		gfp_t gfp_flags);

/*
 * Mark the boot cpu "online" so that it can call console drivers in
 * printk() and can access its per-cpu storage.
 */
void smp_prepare_boot_cpu(void);
=======
void smp_call_function(smp_call_func_t func, void *info, int wait);
void smp_call_function_many(const struct cpumask *mask,
			    smp_call_func_t func, void *info, bool wait);

int smp_call_function_any(const struct cpumask *mask,
			  smp_call_func_t func, void *info, int wait);

void kick_all_cpus_sync(void);
void wake_up_all_idle_cpus(void);

/*
 * Generic and arch helpers
 */
void __init call_function_init(void);
void generic_smp_call_function_single_interrupt(void);
#define generic_smp_call_function_interrupt \
	generic_smp_call_function_single_interrupt
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern unsigned int setup_max_cpus;
extern void __init setup_nr_cpu_ids(void);
extern void __init smp_init(void);

<<<<<<< HEAD
=======
extern int __boot_cpu_id;

static inline int get_boot_cpu_id(void)
{
	return __boot_cpu_id;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else /* !SMP */

static inline void smp_send_stop(void) { }

/*
 *	These macros fold the SMP functionality into a single CPU system
 */
#define raw_smp_processor_id()			0
<<<<<<< HEAD
static inline int up_smp_call_function(smp_call_func_t func, void *info)
{
	return 0;
}
#define smp_call_function(func, info, wait) \
			(up_smp_call_function(func, info))
#define on_each_cpu(func,info,wait)		\
	({					\
		local_irq_disable();		\
		func(info);			\
		local_irq_enable();		\
		0;				\
	})
/*
 * Note we still need to test the mask even for UP
 * because we actually can get an empty mask from
 * code that on SMP might call us without the local
 * CPU in the mask.
 */
#define on_each_cpu_mask(mask, func, info, wait) \
	do {						\
		if (cpumask_test_cpu(0, (mask))) {	\
			local_irq_disable();		\
			(func)(info);			\
			local_irq_enable();		\
		}					\
	} while (0)
/*
 * Preemption is disabled here to make sure the cond_func is called under the
 * same condtions in UP and SMP.
 */
#define on_each_cpu_cond(cond_func, func, info, wait, gfp_flags)\
	do {							\
		void *__info = (info);				\
		preempt_disable();				\
		if ((cond_func)(0, __info)) {			\
			local_irq_disable();			\
			(func)(__info);				\
			local_irq_enable();			\
		}						\
		preempt_enable();				\
	} while (0)

static inline void smp_send_reschedule(int cpu) { }
#define num_booting_cpus()			1
#define smp_prepare_boot_cpu()			do {} while (0)
=======
static inline void up_smp_call_function(smp_call_func_t func, void *info)
{
}
#define smp_call_function(func, info, wait) \
			(up_smp_call_function(func, info))

static inline void smp_send_reschedule(int cpu) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define smp_call_function_many(mask, func, info, wait) \
			(up_smp_call_function(func, info))
static inline void call_function_init(void) { }

static inline int
smp_call_function_any(const struct cpumask *mask, smp_call_func_t func,
		      void *info, int wait)
{
	return smp_call_function_single(0, func, info, wait);
}

<<<<<<< HEAD
#endif /* !SMP */

/*
 * smp_processor_id(): get the current CPU ID.
 *
 * if DEBUG_PREEMPT is enabled then we check whether it is
 * used in a preemption-safe way. (smp_processor_id() is safe
 * if it's used in a preemption-off critical section, or in
 * a thread that is bound to the current CPU.)
 *
 * NOTE: raw_smp_processor_id() is for internal use only
 * (smp_processor_id() is the preferred variant), but in rare
 * instances it might also be used to turn off false positives
 * (i.e. smp_processor_id() use that the debugging code reports but
 * which use for some reason is legal). Don't use this to hack around
 * the warning message, as your code might not work under PREEMPT.
 */
=======
static inline void kick_all_cpus_sync(void) {  }
static inline void wake_up_all_idle_cpus(void) {  }

#define setup_max_cpus 0

#ifdef CONFIG_UP_LATE_INIT
extern void __init up_late_init(void);
static inline void smp_init(void) { up_late_init(); }
#else
static inline void smp_init(void) { }
#endif

static inline int get_boot_cpu_id(void)
{
	return 0;
}

#endif /* !SMP */

/**
 * raw_processor_id() - get the current (unstable) CPU id
 *
 * For then you know what you are doing and need an unstable
 * CPU id.
 */

/**
 * smp_processor_id() - get the current (stable) CPU id
 *
 * This is the normal accessor to the CPU id and should be used
 * whenever possible.
 *
 * The CPU id is stable when:
 *
 *  - IRQs are disabled;
 *  - preemption is disabled;
 *  - the task is CPU affine.
 *
 * When CONFIG_DEBUG_PREEMPT; we verify these assumption and WARN
 * when smp_processor_id() is used when the CPU id is not stable.
 */

/*
 * Allow the architecture to differentiate between a stable and unstable read.
 * For example, x86 uses an IRQ-safe asm-volatile read for the unstable but a
 * regular asm read for the stable.
 */
#ifndef __smp_processor_id
#define __smp_processor_id() raw_smp_processor_id()
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_DEBUG_PREEMPT
  extern unsigned int debug_smp_processor_id(void);
# define smp_processor_id() debug_smp_processor_id()
#else
<<<<<<< HEAD
# define smp_processor_id() raw_smp_processor_id()
#endif

#define get_cpu()		({ preempt_disable(); smp_processor_id(); })
=======
# define smp_processor_id() __smp_processor_id()
#endif

#define get_cpu()		({ preempt_disable(); __smp_processor_id(); })
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define put_cpu()		preempt_enable()

/*
 * Callback to arch code if there's nosmp or maxcpus=0 on the
 * boot command line:
 */
extern void arch_disable_smp_support(void);

<<<<<<< HEAD
void smp_setup_processor_id(void);

=======
extern void arch_thaw_secondary_cpus_begin(void);
extern void arch_thaw_secondary_cpus_end(void);

void smp_setup_processor_id(void);

int smp_call_on_cpu(unsigned int cpu, int (*func)(void *), void *par,
		    bool phys);

/* SMP core functions */
int smpcfd_prepare_cpu(unsigned int cpu);
int smpcfd_dead_cpu(unsigned int cpu);
int smpcfd_dying_cpu(unsigned int cpu);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __LINUX_SMP_H */
