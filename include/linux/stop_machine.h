<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_STOP_MACHINE
#define _LINUX_STOP_MACHINE

#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/smp.h>
#include <linux/list.h>

/*
 * stop_cpu[s]() is simplistic per-cpu maximum priority cpu
 * monopolization mechanism.  The caller can specify a non-sleeping
 * function to be executed on a single or multiple cpus preempting all
 * other processes and monopolizing those cpus until it finishes.
 *
 * Resources for this mechanism are preallocated when a cpu is brought
 * up and requests are guaranteed to be served as long as the target
 * cpus are online.
 */
typedef int (*cpu_stop_fn_t)(void *arg);

#ifdef CONFIG_SMP

struct cpu_stop_work {
	struct list_head	list;		/* cpu_stopper->works */
	cpu_stop_fn_t		fn;
<<<<<<< HEAD
=======
	unsigned long		caller;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void			*arg;
	struct cpu_stop_done	*done;
};

int stop_one_cpu(unsigned int cpu, cpu_stop_fn_t fn, void *arg);
<<<<<<< HEAD
void stop_one_cpu_nowait(unsigned int cpu, cpu_stop_fn_t fn, void *arg,
			 struct cpu_stop_work *work_buf);
int stop_cpus(const struct cpumask *cpumask, cpu_stop_fn_t fn, void *arg);
int try_stop_cpus(const struct cpumask *cpumask, cpu_stop_fn_t fn, void *arg);
=======
int stop_two_cpus(unsigned int cpu1, unsigned int cpu2, cpu_stop_fn_t fn, void *arg);
bool stop_one_cpu_nowait(unsigned int cpu, cpu_stop_fn_t fn, void *arg,
			 struct cpu_stop_work *work_buf);
void stop_machine_park(int cpu);
void stop_machine_unpark(int cpu);
void stop_machine_yield(const struct cpumask *cpumask);

extern void print_stop_info(const char *log_lvl, struct task_struct *task);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#else	/* CONFIG_SMP */

#include <linux/workqueue.h>

struct cpu_stop_work {
	struct work_struct	work;
	cpu_stop_fn_t		fn;
	void			*arg;
};

static inline int stop_one_cpu(unsigned int cpu, cpu_stop_fn_t fn, void *arg)
{
	int ret = -ENOENT;
	preempt_disable();
	if (cpu == smp_processor_id())
		ret = fn(arg);
	preempt_enable();
	return ret;
}

static void stop_one_cpu_nowait_workfn(struct work_struct *work)
{
	struct cpu_stop_work *stwork =
		container_of(work, struct cpu_stop_work, work);
	preempt_disable();
	stwork->fn(stwork->arg);
	preempt_enable();
}

<<<<<<< HEAD
static inline void stop_one_cpu_nowait(unsigned int cpu,
=======
static inline bool stop_one_cpu_nowait(unsigned int cpu,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				       cpu_stop_fn_t fn, void *arg,
				       struct cpu_stop_work *work_buf)
{
	if (cpu == smp_processor_id()) {
		INIT_WORK(&work_buf->work, stop_one_cpu_nowait_workfn);
		work_buf->fn = fn;
		work_buf->arg = arg;
		schedule_work(&work_buf->work);
<<<<<<< HEAD
	}
}

static inline int stop_cpus(const struct cpumask *cpumask,
			    cpu_stop_fn_t fn, void *arg)
{
	if (cpumask_test_cpu(raw_smp_processor_id(), cpumask))
		return stop_one_cpu(raw_smp_processor_id(), fn, arg);
	return -ENOENT;
}

static inline int try_stop_cpus(const struct cpumask *cpumask,
				cpu_stop_fn_t fn, void *arg)
{
	return stop_cpus(cpumask, fn, arg);
}
=======
		return true;
	}

	return false;
}

static inline void print_stop_info(const char *log_lvl, struct task_struct *task) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* CONFIG_SMP */

/*
 * stop_machine "Bogolock": stop the entire machine, disable
 * interrupts.  This is a very heavy lock, which is equivalent to
 * grabbing every spinlock (and more).  So the "read" side to such a
 * lock is anything which disables preemption.
 */
<<<<<<< HEAD
#if defined(CONFIG_STOP_MACHINE) && defined(CONFIG_SMP)
=======
#if defined(CONFIG_SMP) || defined(CONFIG_HOTPLUG_CPU)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * stop_machine: freeze the machine on all CPUs and run this function
 * @fn: the function to run
 * @data: the data ptr for the @fn()
 * @cpus: the cpus to run the @fn() on (NULL = any online cpu)
 *
 * Description: This causes a thread to be scheduled on every cpu,
 * each of which disables interrupts.  The result is that no one is
 * holding a spinlock or inside any other preempt-disabled region when
 * @fn() runs.
 *
 * This can be thought of as a very heavy write lock, equivalent to
<<<<<<< HEAD
 * grabbing every spinlock in the kernel. */
int stop_machine(int (*fn)(void *), void *data, const struct cpumask *cpus);

/**
 * __stop_machine: freeze the machine on all CPUs and run this function
 * @fn: the function to run
 * @data: the data ptr for the @fn
 * @cpus: the cpus to run the @fn() on (NULL = any online cpu)
 *
 * Description: This is a special version of the above, which assumes cpus
 * won't come or go while it's being called.  Used by hotplug cpu.
 */
int __stop_machine(int (*fn)(void *), void *data, const struct cpumask *cpus);

int stop_machine_from_inactive_cpu(int (*fn)(void *), void *data,
				   const struct cpumask *cpus);

#else	 /* CONFIG_STOP_MACHINE && CONFIG_SMP */

static inline int __stop_machine(int (*fn)(void *), void *data,
				 const struct cpumask *cpus)
=======
 * grabbing every spinlock in the kernel.
 *
 * Protects against CPU hotplug.
 */
int stop_machine(cpu_stop_fn_t fn, void *data, const struct cpumask *cpus);

/**
 * stop_machine_cpuslocked: freeze the machine on all CPUs and run this function
 * @fn: the function to run
 * @data: the data ptr for the @fn()
 * @cpus: the cpus to run the @fn() on (NULL = any online cpu)
 *
 * Same as above. Must be called from with in a cpus_read_lock() protected
 * region. Avoids nested calls to cpus_read_lock().
 */
int stop_machine_cpuslocked(cpu_stop_fn_t fn, void *data, const struct cpumask *cpus);

/**
 * stop_core_cpuslocked: - stop all threads on just one core
 * @cpu: any cpu in the targeted core
 * @fn: the function to run
 * @data: the data ptr for @fn()
 *
 * Same as above, but instead of every CPU, only the logical CPUs of a
 * single core are affected.
 *
 * Context: Must be called from within a cpus_read_lock() protected region.
 *
 * Return: 0 if all executions of @fn returned 0, any non zero return
 * value if any returned non zero.
 */
int stop_core_cpuslocked(unsigned int cpu, cpu_stop_fn_t fn, void *data);

int stop_machine_from_inactive_cpu(cpu_stop_fn_t fn, void *data,
				   const struct cpumask *cpus);
#else	/* CONFIG_SMP || CONFIG_HOTPLUG_CPU */

static __always_inline int stop_machine_cpuslocked(cpu_stop_fn_t fn, void *data,
					  const struct cpumask *cpus)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;
	int ret;
	local_irq_save(flags);
	ret = fn(data);
	local_irq_restore(flags);
	return ret;
}

<<<<<<< HEAD
static inline int stop_machine(int (*fn)(void *), void *data,
			       const struct cpumask *cpus)
{
	return __stop_machine(fn, data, cpus);
}

static inline int stop_machine_from_inactive_cpu(int (*fn)(void *), void *data,
						 const struct cpumask *cpus)
{
	return __stop_machine(fn, data, cpus);
}

#endif	/* CONFIG_STOP_MACHINE && CONFIG_SMP */
=======
static __always_inline int
stop_machine(cpu_stop_fn_t fn, void *data, const struct cpumask *cpus)
{
	return stop_machine_cpuslocked(fn, data, cpus);
}

static __always_inline int
stop_machine_from_inactive_cpu(cpu_stop_fn_t fn, void *data,
			       const struct cpumask *cpus)
{
	return stop_machine(fn, data, cpus);
}

#endif	/* CONFIG_SMP || CONFIG_HOTPLUG_CPU */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _LINUX_STOP_MACHINE */
