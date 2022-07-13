<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
** SMP Support
**
** Copyright (C) 1999 Walt Drummond <drummond@valinux.com>
** Copyright (C) 1999 David Mosberger-Tang <davidm@hpl.hp.com>
** Copyright (C) 2001,2004 Grant Grundler <grundler@parisc-linux.org>
** 
** Lots of stuff stolen from arch/alpha/kernel/smp.c
** ...and then parisc stole from arch/ia64/kernel/smp.c. Thanks David! :^)
**
** Thanks to John Curry and Ullas Ponnadi. I learned a lot from their work.
** -grant (1/12/2001)
**
<<<<<<< HEAD
**	This program is free software; you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**      the Free Software Foundation; either version 2 of the License, or
**      (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
*/
#include <linux/types.h>
#include <linux/spinlock.h>

#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/sched.h>
=======
#include <linux/sched/mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/smp.h>
#include <linux/kernel_stat.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/bitops.h>
#include <linux/ftrace.h>
#include <linux/cpu.h>
<<<<<<< HEAD
=======
#include <linux/kgdb.h>
#include <linux/sched/hotplug.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/atomic.h>
#include <asm/current.h>
#include <asm/delay.h>
#include <asm/tlbflush.h>

#include <asm/io.h>
#include <asm/irq.h>		/* for CPU_IRQ_REGION and friends */
#include <asm/mmu_context.h>
#include <asm/page.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
#include <asm/pgalloc.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/processor.h>
#include <asm/ptrace.h>
#include <asm/unistd.h>
#include <asm/cacheflush.h>

#undef DEBUG_SMP
#ifdef DEBUG_SMP
static int smp_debug_lvl = 0;
#define smp_debug(lvl, printargs...)		\
		if (lvl >= smp_debug_lvl)	\
			printk(printargs);
#else
#define smp_debug(lvl, ...)	do { } while(0)
#endif /* DEBUG_SMP */

volatile struct task_struct *smp_init_current_idle_task;

/* track which CPU is booting */
<<<<<<< HEAD
static volatile int cpu_now_booting __cpuinitdata;

static int parisc_max_cpus __cpuinitdata = 1;
=======
static volatile int cpu_now_booting;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_PER_CPU(spinlock_t, ipi_lock);

enum ipi_message_type {
	IPI_NOP=0,
	IPI_RESCHEDULE=1,
	IPI_CALL_FUNC,
<<<<<<< HEAD
	IPI_CALL_FUNC_SINGLE,
	IPI_CPU_START,
	IPI_CPU_STOP,
	IPI_CPU_TEST
=======
	IPI_CPU_START,
	IPI_CPU_STOP,
	IPI_CPU_TEST,
#ifdef CONFIG_KGDB
	IPI_ENTER_KGDB,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};


/********** SMP inter processor interrupt and communication routines */

#undef PER_CPU_IRQ_REGION
#ifdef PER_CPU_IRQ_REGION
/* XXX REVISIT Ignore for now.
**    *May* need this "hook" to register IPI handler
**    once we have perCPU ExtIntr switch tables.
*/
static void
ipi_init(int cpuid)
{
#error verify IRQ_OFFSET(IPI_IRQ) is ipi_interrupt() in new IRQ region

	if(cpu_online(cpuid) )
	{
		switch_to_idle_task(current);
	}

	return;
}
#endif


/*
** Yoink this CPU from the runnable list... 
**
*/
static void
halt_processor(void) 
{
	/* REVISIT : redirect I/O Interrupts to another CPU? */
	/* REVISIT : does PM *know* this CPU isn't available? */
	set_cpu_online(smp_processor_id(), false);
	local_irq_disable();
<<<<<<< HEAD
=======
	__pdc_cpu_rendezvous();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (;;)
		;
}


irqreturn_t __irq_entry
ipi_interrupt(int irq, void *dev_id) 
{
	int this_cpu = smp_processor_id();
	struct cpuinfo_parisc *p = &per_cpu(cpu_data, this_cpu);
	unsigned long ops;
	unsigned long flags;

<<<<<<< HEAD
	/* Count this now; we may make a call that never returns. */
	p->ipi_count++;

	mb();	/* Order interrupt and bit testing. */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (;;) {
		spinlock_t *lock = &per_cpu(ipi_lock, this_cpu);
		spin_lock_irqsave(lock, flags);
		ops = p->pending_ipi;
		p->pending_ipi = 0;
		spin_unlock_irqrestore(lock, flags);

		mb(); /* Order bit clearing and data access. */

		if (!ops)
		    break;

		while (ops) {
			unsigned long which = ffz(~ops);

			ops &= ~(1 << which);

			switch (which) {
			case IPI_NOP:
				smp_debug(100, KERN_DEBUG "CPU%d IPI_NOP\n", this_cpu);
				break;
				
			case IPI_RESCHEDULE:
				smp_debug(100, KERN_DEBUG "CPU%d IPI_RESCHEDULE\n", this_cpu);
<<<<<<< HEAD
=======
				inc_irq_stat(irq_resched_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				scheduler_ipi();
				break;

			case IPI_CALL_FUNC:
				smp_debug(100, KERN_DEBUG "CPU%d IPI_CALL_FUNC\n", this_cpu);
<<<<<<< HEAD
				generic_smp_call_function_interrupt();
				break;

			case IPI_CALL_FUNC_SINGLE:
				smp_debug(100, KERN_DEBUG "CPU%d IPI_CALL_FUNC_SINGLE\n", this_cpu);
				generic_smp_call_function_single_interrupt();
				break;

=======
				inc_irq_stat(irq_call_count);
				generic_smp_call_function_interrupt();
				break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			case IPI_CPU_START:
				smp_debug(100, KERN_DEBUG "CPU%d IPI_CPU_START\n", this_cpu);
				break;

			case IPI_CPU_STOP:
				smp_debug(100, KERN_DEBUG "CPU%d IPI_CPU_STOP\n", this_cpu);
				halt_processor();
				break;

			case IPI_CPU_TEST:
				smp_debug(100, KERN_DEBUG "CPU%d is alive!\n", this_cpu);
				break;
<<<<<<< HEAD

=======
#ifdef CONFIG_KGDB
			case IPI_ENTER_KGDB:
				smp_debug(100, KERN_DEBUG "CPU%d ENTER_KGDB\n", this_cpu);
				kgdb_nmicallback(raw_smp_processor_id(), get_irq_regs());
				break;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			default:
				printk(KERN_CRIT "Unknown IPI num on CPU%d: %lu\n",
					this_cpu, which);
				return IRQ_NONE;
			} /* Switch */
<<<<<<< HEAD
		/* let in any pending interrupts */
		local_irq_enable();
		local_irq_disable();
=======

			/* before doing more, let in any pending interrupts */
			if (ops) {
				local_irq_enable();
				local_irq_disable();
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} /* while (ops) */
	}
	return IRQ_HANDLED;
}


static inline void
ipi_send(int cpu, enum ipi_message_type op)
{
	struct cpuinfo_parisc *p = &per_cpu(cpu_data, cpu);
	spinlock_t *lock = &per_cpu(ipi_lock, cpu);
	unsigned long flags;

	spin_lock_irqsave(lock, flags);
	p->pending_ipi |= 1 << op;
	gsc_writel(IPI_IRQ - CPU_IRQ_BASE, p->hpa);
	spin_unlock_irqrestore(lock, flags);
}

static void
send_IPI_mask(const struct cpumask *mask, enum ipi_message_type op)
{
	int cpu;

	for_each_cpu(cpu, mask)
		ipi_send(cpu, op);
}

static inline void
send_IPI_single(int dest_cpu, enum ipi_message_type op)
{
	BUG_ON(dest_cpu == NO_PROC_ID);

	ipi_send(dest_cpu, op);
}

static inline void
send_IPI_allbutself(enum ipi_message_type op)
{
	int i;
<<<<<<< HEAD
	
=======

	preempt_disable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for_each_online_cpu(i) {
		if (i != smp_processor_id())
			send_IPI_single(i, op);
	}
<<<<<<< HEAD
}

=======
	preempt_enable();
}

#ifdef CONFIG_KGDB
void kgdb_roundup_cpus(void)
{
	send_IPI_allbutself(IPI_ENTER_KGDB);
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

inline void 
smp_send_stop(void)	{ send_IPI_allbutself(IPI_CPU_STOP); }

<<<<<<< HEAD
static inline void
smp_send_start(void)	{ send_IPI_allbutself(IPI_CPU_START); }

void 
smp_send_reschedule(int cpu) { send_IPI_single(cpu, IPI_RESCHEDULE); }
=======
void
arch_smp_send_reschedule(int cpu) { send_IPI_single(cpu, IPI_RESCHEDULE); }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void
smp_send_all_nop(void)
{
	send_IPI_allbutself(IPI_NOP);
}

void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
	send_IPI_mask(mask, IPI_CALL_FUNC);
}

void arch_send_call_function_single_ipi(int cpu)
{
<<<<<<< HEAD
	send_IPI_single(cpu, IPI_CALL_FUNC_SINGLE);
}

/*
 * Flush all other CPU's tlb and then mine.  Do this with on_each_cpu()
 * as we want to ensure all TLB's flushed before proceeding.
 */

void
smp_flush_tlb_all(void)
{
	on_each_cpu(flush_tlb_all_local, NULL, 1);
=======
	send_IPI_single(cpu, IPI_CALL_FUNC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Called by secondaries to update state and initialize CPU registers.
 */
<<<<<<< HEAD
static void __init
smp_cpu_init(int cpunum)
{
	extern int init_per_cpu(int);  /* arch/parisc/kernel/processor.c */
	extern void init_IRQ(void);    /* arch/parisc/kernel/irq.c */
	extern void start_cpu_itimer(void); /* arch/parisc/kernel/time.c */

	/* Set modes and Enable floating point coprocessor */
	(void) init_per_cpu(cpunum);
=======
static void
smp_cpu_init(int cpunum)
{
	/* Set modes and Enable floating point coprocessor */
	init_per_cpu(cpunum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	disable_sr_hashing();

	mb();

	/* Well, support 2.4 linux scheme as well. */
	if (cpu_online(cpunum))	{
		extern void machine_halt(void); /* arch/parisc.../process.c */

		printk(KERN_CRIT "CPU#%d already initialized!\n", cpunum);
		machine_halt();
	}

	notify_cpu_starting(cpunum);

<<<<<<< HEAD
	ipi_call_lock();
	set_cpu_online(cpunum, true);
	ipi_call_unlock();

	/* Initialise the idle task for this CPU */
	atomic_inc(&init_mm.mm_count);
=======
	set_cpu_online(cpunum, true);

	/* Initialise the idle task for this CPU */
	mmgrab(&init_mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	current->active_mm = &init_mm;
	BUG_ON(current->mm);
	enter_lazy_tlb(&init_mm, current);

	init_IRQ();   /* make sure no IRQs are enabled or pending */
	start_cpu_itimer();
}


/*
 * Slaves start using C here. Indirectly called from smp_slave_stext.
 * Do what start_kernel() and main() do for boot strap processor (aka monarch)
 */
<<<<<<< HEAD
void __init smp_callin(void)
{
	int slave_id = cpu_now_booting;

	smp_cpu_init(slave_id);
	preempt_disable();
=======
void smp_callin(unsigned long pdce_proc)
{
	int slave_id = cpu_now_booting;

#ifdef CONFIG_64BIT
	WARN_ON(((unsigned long)(PAGE0->mem_pdc_hi) << 32
			| PAGE0->mem_pdc) != pdce_proc);
#endif

	smp_cpu_init(slave_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	flush_cache_all_local(); /* start with known state */
	flush_tlb_all_local(NULL);

	local_irq_enable();  /* Interrupts have been off until now */

<<<<<<< HEAD
	cpu_idle();      /* Wait for timer to schedule some work */
=======
	cpu_startup_entry(CPUHP_AP_ONLINE_IDLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* NOTREACHED */
	panic("smp_callin() AAAAaaaaahhhh....\n");
}

/*
 * Bring one cpu online.
 */
<<<<<<< HEAD
int __cpuinit smp_boot_one_cpu(int cpuid)
{
	const struct cpuinfo_parisc *p = &per_cpu(cpu_data, cpuid);
	struct task_struct *idle;
	long timeout;

	/* 
	 * Create an idle task for this CPU.  Note the address wed* give 
	 * to kernel_thread is irrelevant -- it's going to start
	 * where OS_BOOT_RENDEVZ vector in SAL says to start.  But
	 * this gets all the other task-y sort of data structures set
	 * up like we wish.   We need to pull the just created idle task 
	 * off the run queue and stuff it into the init_tasks[] array.  
	 * Sheesh . . .
	 */

	idle = fork_idle(cpuid);
	if (IS_ERR(idle))
		panic("SMP: fork failed for CPU:%d", cpuid);

	task_thread_info(idle)->cpu = cpuid;
=======
static int smp_boot_one_cpu(int cpuid, struct task_struct *idle)
{
	const struct cpuinfo_parisc *p = &per_cpu(cpu_data, cpuid);
	long timeout;

#ifdef CONFIG_HOTPLUG_CPU
	int i;

	/* reset irq statistics for this CPU */
	memset(&per_cpu(irq_stat, cpuid), 0, sizeof(irq_cpustat_t));
	for (i = 0; i < NR_IRQS; i++) {
		struct irq_desc *desc = irq_to_desc(i);

		if (desc && desc->kstat_irqs)
			*per_cpu_ptr(desc->kstat_irqs, cpuid) = 0;
	}
#endif

	/* wait until last booting CPU has started. */
	while (cpu_now_booting)
		;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Let _start know what logical CPU we're booting
	** (offset into init_tasks[],cpu_data[])
	*/
	cpu_now_booting = cpuid;

	/* 
	** boot strap code needs to know the task address since
	** it also contains the process stack.
	*/
	smp_init_current_idle_task = idle ;
	mb();

	printk(KERN_INFO "Releasing cpu %d now, hpa=%lx\n", cpuid, p->hpa);

	/*
	** This gets PDC to release the CPU from a very tight loop.
	**
	** From the PA-RISC 2.0 Firmware Architecture Reference Specification:
	** "The MEM_RENDEZ vector specifies the location of OS_RENDEZ which 
	** is executed after receiving the rendezvous signal (an interrupt to 
	** EIR{0}). MEM_RENDEZ is valid only when it is nonzero and the 
	** contents of memory are valid."
	*/
	gsc_writel(TIMER_IRQ - CPU_IRQ_BASE, p->hpa);
	mb();

	/* 
	 * OK, wait a bit for that CPU to finish staggering about. 
	 * Slave will set a bit when it reaches smp_cpu_init().
	 * Once the "monarch CPU" sees the bit change, it can move on.
	 */
	for (timeout = 0; timeout < 10000; timeout++) {
		if(cpu_online(cpuid)) {
			/* Which implies Slave has started up */
			cpu_now_booting = 0;
<<<<<<< HEAD
			smp_init_current_idle_task = NULL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto alive ;
		}
		udelay(100);
		barrier();
	}
<<<<<<< HEAD

	put_task_struct(idle);
	idle = NULL;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	printk(KERN_CRIT "SMP: CPU:%d is stuck.\n", cpuid);
	return -1;

alive:
	/* Remember the Slave data */
	smp_debug(100, KERN_DEBUG "SMP: CPU:%d came alive after %ld _us\n",
		cpuid, timeout * 100);
	return 0;
}

void __init smp_prepare_boot_cpu(void)
{
<<<<<<< HEAD
	int bootstrap_processor = per_cpu(cpu_data, 0).cpuid;

	/* Setup BSP mappings */
	printk(KERN_INFO "SMP: bootstrap CPU ID is %d\n", bootstrap_processor);

	set_cpu_online(bootstrap_processor, true);
	set_cpu_present(bootstrap_processor, true);
=======
	pr_info("SMP: bootstrap CPU ID is 0\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}



/*
** inventory.c:do_inventory() hasn't yet been run and thus we
** don't 'discover' the additional CPUs until later.
*/
void __init smp_prepare_cpus(unsigned int max_cpus)
{
	int cpu;

	for_each_possible_cpu(cpu)
		spin_lock_init(&per_cpu(ipi_lock, cpu));

	init_cpu_present(cpumask_of(0));
<<<<<<< HEAD

	parisc_max_cpus = max_cpus;
	if (!max_cpus)
		printk(KERN_INFO "SMP mode deactivated.\n");
}


void smp_cpus_done(unsigned int cpu_max)
{
	return;
}


int __cpuinit __cpu_up(unsigned int cpu)
{
	if (cpu != 0 && cpu < parisc_max_cpus)
		smp_boot_one_cpu(cpu);

	return cpu_online(cpu) ? 0 : -ENOSYS;
}

#ifdef CONFIG_PROC_FS
int __init
setup_profiling_timer(unsigned int multiplier)
{
	return -EINVAL;
}
#endif
=======
}


void __init smp_cpus_done(unsigned int cpu_max)
{
}


int __cpu_up(unsigned int cpu, struct task_struct *tidle)
{
	if (cpu_online(cpu))
		return 0;

	if (num_online_cpus() < nr_cpu_ids &&
		num_online_cpus() < setup_max_cpus &&
		smp_boot_one_cpu(cpu, tidle))
		return -EIO;

	return cpu_online(cpu) ? 0 : -EIO;
}

/*
 * __cpu_disable runs on the processor to be shutdown.
 */
int __cpu_disable(void)
{
#ifdef CONFIG_HOTPLUG_CPU
	unsigned int cpu = smp_processor_id();

	remove_cpu_topology(cpu);

	/*
	 * Take this CPU offline.  Once we clear this, we can't return,
	 * and we must not schedule until we're ready to give up the cpu.
	 */
	set_cpu_online(cpu, false);

	/* Find a new timesync master */
	if (cpu == time_keeper_id) {
		time_keeper_id = cpumask_first(cpu_online_mask);
		pr_info("CPU %d is now promoted to time-keeper master\n", time_keeper_id);
	}

	disable_percpu_irq(IPI_IRQ);

	irq_migrate_all_off_this_cpu();

	flush_cache_all_local();
	flush_tlb_all_local(NULL);

	/* disable all irqs, including timer irq */
	local_irq_disable();

	/* wait for next timer irq ... */
	mdelay(1000/HZ+100);

	/* ... and then clear all pending external irqs */
	set_eiem(0);
	mtctl(~0UL, CR_EIRR);
	mfctl(CR_EIRR);
	mtctl(0, CR_EIRR);
#endif
	return 0;
}

/*
 * called on the thread which is asking for a CPU to be shutdown -
 * waits until shutdown has completed, or it is timed out.
 */
void __cpu_die(unsigned int cpu)
{
	pdc_cpu_rendezvous_lock();
}

void arch_cpuhp_cleanup_dead_cpu(unsigned int cpu)
{
	pr_info("CPU%u: is shutting down\n", cpu);

	/* set task's state to interruptible sleep */
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout((IS_ENABLED(CONFIG_64BIT) ? 8:2) * HZ);

	pdc_cpu_rendezvous_unlock();
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
