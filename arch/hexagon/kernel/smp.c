<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SMP support for Hexagon
 *
 * Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
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
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/err.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/percpu.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
#include <linux/cpu.h>
=======
#include <linux/sched/mm.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
#include <linux/cpu.h>
#include <linux/mm_types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/time.h>    /*  timer_interrupt  */
#include <asm/hexagon_vm.h>

#define BASE_IPI_IRQ 26

/*
 * cpu_possible_mask needs to be filled out prior to setup_per_cpu_areas
 * (which is prior to any of our smp_prepare_cpu crap), in order to set
 * up the...  per_cpu areas.
 */

struct ipi_data {
	unsigned long bits;
};

static DEFINE_PER_CPU(struct ipi_data, ipi_data);

static inline void __handle_ipi(unsigned long *ops, struct ipi_data *ipi,
				int cpu)
{
	unsigned long msg = 0;
	do {
		msg = find_next_bit(ops, BITS_PER_LONG, msg+1);

		switch (msg) {

		case IPI_TIMER:
			ipi_timer();
			break;

		case IPI_CALL_FUNC:
			generic_smp_call_function_interrupt();
			break;

<<<<<<< HEAD
		case IPI_CALL_FUNC_SINGLE:
			generic_smp_call_function_single_interrupt();
			break;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case IPI_CPU_STOP:
			/*
			 * call vmstop()
			 */
			__vmstop();
			break;

		case IPI_RESCHEDULE:
			scheduler_ipi();
			break;
		}
	} while (msg < BITS_PER_LONG);
}

/*  Used for IPI call from other CPU's to unmask int  */
void smp_vm_unmask_irq(void *info)
{
	__vmintop_locen((long) info);
}


/*
 * This is based on Alpha's IPI stuff.
 * Supposed to take (int, void*) as args now.
 * Specifically, first arg is irq, second is the irq_desc.
 */

<<<<<<< HEAD
irqreturn_t handle_ipi(int irq, void *desc)
=======
static irqreturn_t handle_ipi(int irq, void *desc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int cpu = smp_processor_id();
	struct ipi_data *ipi = &per_cpu(ipi_data, cpu);
	unsigned long ops;

	while ((ops = xchg(&ipi->bits, 0)) != 0)
		__handle_ipi(&ops, ipi, cpu);
	return IRQ_HANDLED;
}

void send_ipi(const struct cpumask *cpumask, enum ipi_message_type msg)
{
	unsigned long flags;
	unsigned long cpu;
	unsigned long retval;

	local_irq_save(flags);

	for_each_cpu(cpu, cpumask) {
		struct ipi_data *ipi = &per_cpu(ipi_data, cpu);

		set_bit(msg, &ipi->bits);
		/*  Possible barrier here  */
		retval = __vmintop_post(BASE_IPI_IRQ+cpu);

		if (retval != 0) {
			printk(KERN_ERR "interrupt %ld not configured?\n",
				BASE_IPI_IRQ+cpu);
		}
	}

	local_irq_restore(flags);
}

<<<<<<< HEAD
static struct irqaction ipi_intdesc = {
	.handler = handle_ipi,
	.flags = IRQF_TRIGGER_RISING,
	.name = "ipi_handler"
};

void __init smp_prepare_boot_cpu(void)
{
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * interrupts should already be disabled from the VM
 * SP should already be correct; need to set THREADINFO_REG
 * to point to current thread info
 */

<<<<<<< HEAD
void __cpuinit start_secondary(void)
{
	unsigned int cpu;
	unsigned long thread_ptr;
=======
static void start_secondary(void)
{
	unsigned long thread_ptr;
	unsigned int cpu, irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*  Calculate thread_info pointer from stack pointer  */
	__asm__ __volatile__(
		"%0 = SP;\n"
		: "=r" (thread_ptr)
	);

	thread_ptr = thread_ptr & ~(THREAD_SIZE-1);

	__asm__ __volatile__(
		QUOTED_THREADINFO_REG " = %0;\n"
		:
		: "r" (thread_ptr)
	);

	/*  Set the memory struct  */
<<<<<<< HEAD
	atomic_inc(&init_mm.mm_count);
=======
	mmgrab(&init_mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	current->active_mm = &init_mm;

	cpu = smp_processor_id();

<<<<<<< HEAD
	setup_irq(BASE_IPI_IRQ + cpu, &ipi_intdesc);
=======
	irq = BASE_IPI_IRQ + cpu;
	if (request_irq(irq, handle_ipi, IRQF_TRIGGER_RISING, "ipi_handler",
			NULL))
		pr_err("Failed to request irq %u (ipi_handler)\n", irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*  Register the clock_event dummy  */
	setup_percpu_clockdev();

	printk(KERN_INFO "%s cpu %d\n", __func__, current_thread_info()->cpu);

	notify_cpu_starting(cpu);

<<<<<<< HEAD
	ipi_call_lock();
	set_cpu_online(cpu, true);
	ipi_call_unlock();

	local_irq_enable();

	cpu_idle();
=======
	set_cpu_online(cpu, true);

	local_irq_enable();

	cpu_startup_entry(CPUHP_AP_ONLINE_IDLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/*
 * called once for each present cpu
 * apparently starts up the CPU and then
 * maintains control until "cpu_online(cpu)" is set.
 */

<<<<<<< HEAD
int __cpuinit __cpu_up(unsigned int cpu)
{
	struct task_struct *idle;
	struct thread_info *thread;
	void *stack_start;

	/*  Create new init task for the CPU  */
	idle = fork_idle(cpu);
	if (IS_ERR(idle))
		panic(KERN_ERR "fork_idle failed\n");

	thread = (struct thread_info *)idle->stack;
=======
int __cpu_up(unsigned int cpu, struct task_struct *idle)
{
	struct thread_info *thread = (struct thread_info *)idle->stack;
	void *stack_start;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	thread->cpu = cpu;

	/*  Boot to the head.  */
	stack_start =  ((void *) thread) + THREAD_SIZE;
	__vmstart(start_secondary, stack_start);

	while (!cpu_online(cpu))
		barrier();

	return 0;
}

void __init smp_cpus_done(unsigned int max_cpus)
{
}

void __init smp_prepare_cpus(unsigned int max_cpus)
{
<<<<<<< HEAD
	int i;
=======
	int i, irq = BASE_IPI_IRQ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * should eventually have some sort of machine
	 * descriptor that has this stuff
	 */

	/*  Right now, let's just fake it. */
	for (i = 0; i < max_cpus; i++)
		set_cpu_present(i, true);

	/*  Also need to register the interrupts for IPI  */
<<<<<<< HEAD
	if (max_cpus > 1)
		setup_irq(BASE_IPI_IRQ, &ipi_intdesc);
}

void smp_send_reschedule(int cpu)
=======
	if (max_cpus > 1) {
		if (request_irq(irq, handle_ipi, IRQF_TRIGGER_RISING,
				"ipi_handler", NULL))
			pr_err("Failed to request irq %d (ipi_handler)\n", irq);
	}
}

void arch_smp_send_reschedule(int cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	send_ipi(cpumask_of(cpu), IPI_RESCHEDULE);
}

void smp_send_stop(void)
{
	struct cpumask targets;
	cpumask_copy(&targets, cpu_online_mask);
	cpumask_clear_cpu(smp_processor_id(), &targets);
	send_ipi(&targets, IPI_CPU_STOP);
}

void arch_send_call_function_single_ipi(int cpu)
{
<<<<<<< HEAD
	send_ipi(cpumask_of(cpu), IPI_CALL_FUNC_SINGLE);
=======
	send_ipi(cpumask_of(cpu), IPI_CALL_FUNC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
	send_ipi(mask, IPI_CALL_FUNC);
}

<<<<<<< HEAD
int setup_profiling_timer(unsigned int multiplier)
{
	return -EINVAL;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void smp_start_cpus(void)
{
	int i;

	for (i = 0; i < NR_CPUS; i++)
		set_cpu_possible(i, true);
}
