<<<<<<< HEAD
/*
 *  SMP related functions
 *
 *    Copyright IBM Corp. 1999,2012
 *    Author(s): Denis Joseph Barrow,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *  SMP related functions
 *
 *    Copyright IBM Corp. 1999, 2012
 *    Author(s): Denis Joseph Barrow,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  based on other smp stuff by
 *    (c) 1995 Alan Cox, CymruNET Ltd  <alan@cymru.net>
 *    (c) 1998 Ingo Molnar
 *
 * The code outside of smp.c uses logical cpu numbers, only smp.c does
 * the translation of logical to physical cpu ids. All new code that
 * operates on physical cpu numbers needs to go into smp.c.
 */

#define KMSG_COMPONENT "cpu"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/workqueue.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/memblock.h>
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/spinlock.h>
#include <linux/kernel_stat.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/irqflags.h>
<<<<<<< HEAD
#include <linux/cpu.h>
#include <linux/slab.h>
#include <linux/crash_dump.h>
#include <asm/asm-offsets.h>
#include <asm/switch_to.h>
#include <asm/facility.h>
=======
#include <linux/irq_work.h>
#include <linux/cpu.h>
#include <linux/slab.h>
#include <linux/sched/hotplug.h>
#include <linux/sched/task_stack.h>
#include <linux/crash_dump.h>
#include <linux/kprobes.h>
#include <asm/access-regs.h>
#include <asm/asm-offsets.h>
#include <asm/ctlreg.h>
#include <asm/pfault.h>
#include <asm/diag.h>
#include <asm/facility.h>
#include <asm/fpu.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/ipl.h>
#include <asm/setup.h>
#include <asm/irq.h>
#include <asm/tlbflush.h>
<<<<<<< HEAD
#include <asm/timer.h>
#include <asm/lowcore.h>
#include <asm/sclp.h>
#include <asm/vdso.h>
#include <asm/debug.h>
#include <asm/os_info.h>
#include "entry.h"

enum {
	sigp_sense = 1,
	sigp_external_call = 2,
	sigp_emergency_signal = 3,
	sigp_start = 4,
	sigp_stop = 5,
	sigp_restart = 6,
	sigp_stop_and_store_status = 9,
	sigp_initial_cpu_reset = 11,
	sigp_cpu_reset = 12,
	sigp_set_prefix = 13,
	sigp_store_status_at_address = 14,
	sigp_store_extended_status_at_address = 15,
	sigp_set_architecture = 18,
	sigp_conditional_emergency_signal = 19,
	sigp_sense_running = 21,
};

enum {
	sigp_order_code_accepted = 0,
	sigp_status_stored = 1,
	sigp_busy = 2,
	sigp_not_operational = 3,
};

enum {
	ec_schedule = 0,
	ec_call_function,
	ec_call_function_single,
	ec_stop_cpu,
=======
#include <asm/vtimer.h>
#include <asm/abs_lowcore.h>
#include <asm/sclp.h>
#include <asm/debug.h>
#include <asm/os_info.h>
#include <asm/sigp.h>
#include <asm/idle.h>
#include <asm/nmi.h>
#include <asm/stacktrace.h>
#include <asm/topology.h>
#include <asm/vdso.h>
#include <asm/maccess.h>
#include "entry.h"

enum {
	ec_schedule = 0,
	ec_call_function_single,
	ec_stop_cpu,
	ec_mcck_pending,
	ec_irq_work,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

enum {
	CPU_STATE_STANDBY,
	CPU_STATE_CONFIGURED,
};

<<<<<<< HEAD
struct pcpu {
	struct cpu cpu;
	struct task_struct *idle;	/* idle process for the cpu */
	struct _lowcore *lowcore;	/* lowcore page(s) for the cpu */
	unsigned long async_stack;	/* async stack for the cpu */
	unsigned long panic_stack;	/* panic stack for the cpu */
	unsigned long ec_mask;		/* bit mask for ec_xxx functions */
	int state;			/* physical cpu state */
	u32 status;			/* last status received via sigp */
	u16 address;			/* physical cpu address */
};

static u8 boot_cpu_type;
static u16 boot_cpu_address;
static struct pcpu pcpu_devices[NR_CPUS];

=======
static DEFINE_PER_CPU(struct cpu *, cpu_device);

struct pcpu {
	unsigned long ec_mask;		/* bit mask for ec_xxx functions */
	unsigned long ec_clk;		/* sigp timestamp for ec_xxx */
	signed char state;		/* physical cpu state */
	signed char polarization;	/* physical polarization */
	u16 address;			/* physical cpu address */
};

static u8 boot_core_type;
static struct pcpu pcpu_devices[NR_CPUS];

unsigned int smp_cpu_mt_shift;
EXPORT_SYMBOL(smp_cpu_mt_shift);

unsigned int smp_cpu_mtid;
EXPORT_SYMBOL(smp_cpu_mtid);

#ifdef CONFIG_CRASH_DUMP
__vector128 __initdata boot_cpu_vector_save_area[__NUM_VXRS];
#endif

static unsigned int smp_max_threads __initdata = -1U;
cpumask_t cpu_setup_mask;

static int __init early_nosmt(char *s)
{
	smp_max_threads = 1;
	return 0;
}
early_param("nosmt", early_nosmt);

static int __init early_smt(char *s)
{
	get_option(&s, &smp_max_threads);
	return 0;
}
early_param("smt", early_smt);

/*
 * The smp_cpu_state_mutex must be held when changing the state or polarization
 * member of a pcpu data structure within the pcpu_devices array.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
DEFINE_MUTEX(smp_cpu_state_mutex);

/*
 * Signal processor helper functions.
 */
<<<<<<< HEAD
static inline int __pcpu_sigp(u16 addr, u8 order, u32 parm, u32 *status)
{
	register unsigned int reg1 asm ("1") = parm;
	int cc;

	asm volatile(
		"	sigp	%1,%2,0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		: "=d" (cc), "+d" (reg1) : "d" (addr), "a" (order) : "cc");
	if (status && cc == 1)
		*status = reg1;
	return cc;
}

static inline int __pcpu_sigp_relax(u16 addr, u8 order, u32 parm, u32 *status)
=======
static inline int __pcpu_sigp_relax(u16 addr, u8 order, unsigned long parm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int cc;

	while (1) {
<<<<<<< HEAD
		cc = __pcpu_sigp(addr, order, parm, status);
		if (cc != sigp_busy)
=======
		cc = __pcpu_sigp(addr, order, parm, NULL);
		if (cc != SIGP_CC_BUSY)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return cc;
		cpu_relax();
	}
}

static int pcpu_sigp_retry(struct pcpu *pcpu, u8 order, u32 parm)
{
	int cc, retry;

	for (retry = 0; ; retry++) {
<<<<<<< HEAD
		cc = __pcpu_sigp(pcpu->address, order, parm, &pcpu->status);
		if (cc != sigp_busy)
=======
		cc = __pcpu_sigp(pcpu->address, order, parm, NULL);
		if (cc != SIGP_CC_BUSY)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		if (retry >= 3)
			udelay(10);
	}
	return cc;
}

static inline int pcpu_stopped(struct pcpu *pcpu)
{
<<<<<<< HEAD
	if (__pcpu_sigp(pcpu->address, sigp_sense,
			0, &pcpu->status) != sigp_status_stored)
		return 0;
	/* Check for stopped and check stop state */
	return !!(pcpu->status & 0x50);
=======
	u32 status;

	if (__pcpu_sigp(pcpu->address, SIGP_SENSE,
			0, &status) != SIGP_CC_STATUS_STORED)
		return 0;
	return !!(status & (SIGP_STATUS_CHECK_STOP|SIGP_STATUS_STOPPED));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int pcpu_running(struct pcpu *pcpu)
{
<<<<<<< HEAD
	if (__pcpu_sigp(pcpu->address, sigp_sense_running,
			0, &pcpu->status) != sigp_status_stored)
		return 1;
	/* Check for running status */
	return !(pcpu->status & 0x400);
=======
	if (__pcpu_sigp(pcpu->address, SIGP_SENSE_RUNNING,
			0, NULL) != SIGP_CC_STATUS_STORED)
		return 1;
	/* Status stored condition code is equivalent to cpu not running. */
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Find struct pcpu by cpu address.
 */
<<<<<<< HEAD
static struct pcpu *pcpu_find_address(const struct cpumask *mask, int address)
=======
static struct pcpu *pcpu_find_address(const struct cpumask *mask, u16 address)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int cpu;

	for_each_cpu(cpu, mask)
		if (pcpu_devices[cpu].address == address)
			return pcpu_devices + cpu;
	return NULL;
}

static void pcpu_ec_call(struct pcpu *pcpu, int ec_bit)
{
	int order;

<<<<<<< HEAD
	set_bit(ec_bit, &pcpu->ec_mask);
	order = pcpu_running(pcpu) ?
		sigp_external_call : sigp_emergency_signal;
	pcpu_sigp_retry(pcpu, order, 0);
}

static int __cpuinit pcpu_alloc_lowcore(struct pcpu *pcpu, int cpu)
{
	struct _lowcore *lc;

	if (pcpu != &pcpu_devices[0]) {
		pcpu->lowcore =	(struct _lowcore *)
			__get_free_pages(GFP_KERNEL | GFP_DMA, LC_ORDER);
		pcpu->async_stack = __get_free_pages(GFP_KERNEL, ASYNC_ORDER);
		pcpu->panic_stack = __get_free_page(GFP_KERNEL);
		if (!pcpu->lowcore || !pcpu->panic_stack || !pcpu->async_stack)
			goto out;
	}
	lc = pcpu->lowcore;
	memcpy(lc, &S390_lowcore, 512);
	memset((char *) lc + 512, 0, sizeof(*lc) - 512);
	lc->async_stack = pcpu->async_stack + ASYNC_SIZE;
	lc->panic_stack = pcpu->panic_stack + PAGE_SIZE;
	lc->cpu_nr = cpu;
#ifndef CONFIG_64BIT
	if (MACHINE_HAS_IEEE) {
		lc->extended_save_area_addr = get_zeroed_page(GFP_KERNEL);
		if (!lc->extended_save_area_addr)
			goto out;
	}
#else
	if (vdso_alloc_per_cpu(lc))
		goto out;
#endif
	lowcore_ptr[cpu] = lc;
	pcpu_sigp_retry(pcpu, sigp_set_prefix, (u32)(unsigned long) lc);
	return 0;
out:
	if (pcpu != &pcpu_devices[0]) {
		free_page(pcpu->panic_stack);
		free_pages(pcpu->async_stack, ASYNC_ORDER);
		free_pages((unsigned long) pcpu->lowcore, LC_ORDER);
	}
=======
	if (test_and_set_bit(ec_bit, &pcpu->ec_mask))
		return;
	order = pcpu_running(pcpu) ? SIGP_EXTERNAL_CALL : SIGP_EMERGENCY_SIGNAL;
	pcpu->ec_clk = get_tod_clock_fast();
	pcpu_sigp_retry(pcpu, order, 0);
}

static int pcpu_alloc_lowcore(struct pcpu *pcpu, int cpu)
{
	unsigned long async_stack, nodat_stack, mcck_stack;
	struct lowcore *lc;

	lc = (struct lowcore *) __get_free_pages(GFP_KERNEL | GFP_DMA, LC_ORDER);
	nodat_stack = __get_free_pages(GFP_KERNEL, THREAD_SIZE_ORDER);
	async_stack = stack_alloc();
	mcck_stack = stack_alloc();
	if (!lc || !nodat_stack || !async_stack || !mcck_stack)
		goto out;
	memcpy(lc, &S390_lowcore, 512);
	memset((char *) lc + 512, 0, sizeof(*lc) - 512);
	lc->async_stack = async_stack + STACK_INIT_OFFSET;
	lc->nodat_stack = nodat_stack + STACK_INIT_OFFSET;
	lc->mcck_stack = mcck_stack + STACK_INIT_OFFSET;
	lc->cpu_nr = cpu;
	lc->spinlock_lockval = arch_spin_lockval(cpu);
	lc->spinlock_index = 0;
	lc->return_lpswe = gen_lpswe(__LC_RETURN_PSW);
	lc->return_mcck_lpswe = gen_lpswe(__LC_RETURN_MCCK_PSW);
	lc->preempt_count = PREEMPT_DISABLED;
	if (nmi_alloc_mcesa(&lc->mcesad))
		goto out;
	if (abs_lowcore_map(cpu, lc, true))
		goto out_mcesa;
	lowcore_ptr[cpu] = lc;
	pcpu_sigp_retry(pcpu, SIGP_SET_PREFIX, __pa(lc));
	return 0;

out_mcesa:
	nmi_free_mcesa(&lc->mcesad);
out:
	stack_free(mcck_stack);
	stack_free(async_stack);
	free_pages(nodat_stack, THREAD_SIZE_ORDER);
	free_pages((unsigned long) lc, LC_ORDER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -ENOMEM;
}

static void pcpu_free_lowcore(struct pcpu *pcpu)
{
<<<<<<< HEAD
	pcpu_sigp_retry(pcpu, sigp_set_prefix, 0);
	lowcore_ptr[pcpu - pcpu_devices] = NULL;
#ifndef CONFIG_64BIT
	if (MACHINE_HAS_IEEE) {
		struct _lowcore *lc = pcpu->lowcore;

		free_page((unsigned long) lc->extended_save_area_addr);
		lc->extended_save_area_addr = 0;
	}
#else
	vdso_free_per_cpu(pcpu->lowcore);
#endif
	if (pcpu != &pcpu_devices[0]) {
		free_page(pcpu->panic_stack);
		free_pages(pcpu->async_stack, ASYNC_ORDER);
		free_pages((unsigned long) pcpu->lowcore, LC_ORDER);
	}
=======
	unsigned long async_stack, nodat_stack, mcck_stack;
	struct lowcore *lc;
	int cpu;

	cpu = pcpu - pcpu_devices;
	lc = lowcore_ptr[cpu];
	nodat_stack = lc->nodat_stack - STACK_INIT_OFFSET;
	async_stack = lc->async_stack - STACK_INIT_OFFSET;
	mcck_stack = lc->mcck_stack - STACK_INIT_OFFSET;
	pcpu_sigp_retry(pcpu, SIGP_SET_PREFIX, 0);
	lowcore_ptr[cpu] = NULL;
	abs_lowcore_unmap(cpu);
	nmi_free_mcesa(&lc->mcesad);
	stack_free(async_stack);
	stack_free(mcck_stack);
	free_pages(nodat_stack, THREAD_SIZE_ORDER);
	free_pages((unsigned long) lc, LC_ORDER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void pcpu_prepare_secondary(struct pcpu *pcpu, int cpu)
{
<<<<<<< HEAD
	struct _lowcore *lc = pcpu->lowcore;

	atomic_inc(&init_mm.context.attach_count);
	lc->cpu_nr = cpu;
	lc->percpu_offset = __per_cpu_offset[cpu];
	lc->kernel_asce = S390_lowcore.kernel_asce;
	lc->machine_flags = S390_lowcore.machine_flags;
	lc->ftrace_func = S390_lowcore.ftrace_func;
	lc->user_timer = lc->system_timer = lc->steal_timer = 0;
	__ctl_store(lc->cregs_save_area, 0, 15);
	save_access_regs((unsigned int *) lc->access_regs_save_area);
	memcpy(lc->stfle_fac_list, S390_lowcore.stfle_fac_list,
	       MAX_FACILITY_BIT/8);
=======
	struct lowcore *lc, *abs_lc;

	lc = lowcore_ptr[cpu];
	cpumask_set_cpu(cpu, &init_mm.context.cpu_attach_mask);
	cpumask_set_cpu(cpu, mm_cpumask(&init_mm));
	lc->cpu_nr = cpu;
	lc->restart_flags = RESTART_FLAG_CTLREGS;
	lc->spinlock_lockval = arch_spin_lockval(cpu);
	lc->spinlock_index = 0;
	lc->percpu_offset = __per_cpu_offset[cpu];
	lc->kernel_asce = S390_lowcore.kernel_asce;
	lc->user_asce = s390_invalid_asce;
	lc->machine_flags = S390_lowcore.machine_flags;
	lc->user_timer = lc->system_timer =
		lc->steal_timer = lc->avg_steal_timer = 0;
	abs_lc = get_abs_lowcore();
	memcpy(lc->cregs_save_area, abs_lc->cregs_save_area, sizeof(lc->cregs_save_area));
	put_abs_lowcore(abs_lc);
	lc->cregs_save_area[1] = lc->kernel_asce;
	lc->cregs_save_area[7] = lc->user_asce;
	save_access_regs((unsigned int *) lc->access_regs_save_area);
	arch_spin_lock_setup(cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void pcpu_attach_task(struct pcpu *pcpu, struct task_struct *tsk)
{
<<<<<<< HEAD
	struct _lowcore *lc = pcpu->lowcore;
	struct thread_info *ti = task_thread_info(tsk);

	lc->kernel_stack = (unsigned long) task_stack_page(tsk) + THREAD_SIZE;
	lc->thread_info = (unsigned long) task_thread_info(tsk);
	lc->current_task = (unsigned long) tsk;
	lc->user_timer = ti->user_timer;
	lc->system_timer = ti->system_timer;
=======
	struct lowcore *lc;
	int cpu;

	cpu = pcpu - pcpu_devices;
	lc = lowcore_ptr[cpu];
	lc->kernel_stack = (unsigned long)task_stack_page(tsk) + STACK_INIT_OFFSET;
	lc->current_task = (unsigned long)tsk;
	lc->lpp = LPP_MAGIC;
	lc->current_pid = tsk->pid;
	lc->user_timer = tsk->thread.user_timer;
	lc->guest_timer = tsk->thread.guest_timer;
	lc->system_timer = tsk->thread.system_timer;
	lc->hardirq_timer = tsk->thread.hardirq_timer;
	lc->softirq_timer = tsk->thread.softirq_timer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lc->steal_timer = 0;
}

static void pcpu_start_fn(struct pcpu *pcpu, void (*func)(void *), void *data)
{
<<<<<<< HEAD
	struct _lowcore *lc = pcpu->lowcore;

	lc->restart_stack = lc->kernel_stack;
	lc->restart_fn = (unsigned long) func;
	lc->restart_data = (unsigned long) data;
	lc->restart_source = -1UL;
	pcpu_sigp_retry(pcpu, sigp_restart, 0);
}

/*
 * Call function via PSW restart on pcpu and stop the current cpu.
 */
static void pcpu_delegate(struct pcpu *pcpu, void (*func)(void *),
			  void *data, unsigned long stack)
{
	struct _lowcore *lc = pcpu->lowcore;
	unsigned short this_cpu;

	__load_psw_mask(psw_kernel_bits);
	this_cpu = stap();
	if (pcpu->address == this_cpu)
		func(data);	/* should not return */
	/* Stop target cpu (if func returns this stops the current cpu). */
	pcpu_sigp_retry(pcpu, sigp_stop, 0);
	/* Restart func on the target cpu and stop the current cpu. */
	lc->restart_stack = stack;
	lc->restart_fn = (unsigned long) func;
	lc->restart_data = (unsigned long) data;
	lc->restart_source = (unsigned long) this_cpu;
	asm volatile(
		"0:	sigp	0,%0,6	# sigp restart to target cpu\n"
		"	brc	2,0b	# busy, try again\n"
		"1:	sigp	0,%1,5	# sigp stop to current cpu\n"
		"	brc	2,1b	# busy, try again\n"
		: : "d" (pcpu->address), "d" (this_cpu) : "0", "1", "cc");
=======
	struct lowcore *lc;
	int cpu;

	cpu = pcpu - pcpu_devices;
	lc = lowcore_ptr[cpu];
	lc->restart_stack = lc->kernel_stack;
	lc->restart_fn = (unsigned long) func;
	lc->restart_data = (unsigned long) data;
	lc->restart_source = -1U;
	pcpu_sigp_retry(pcpu, SIGP_RESTART, 0);
}

typedef void (pcpu_delegate_fn)(void *);

/*
 * Call function via PSW restart on pcpu and stop the current cpu.
 */
static void __pcpu_delegate(pcpu_delegate_fn *func, void *data)
{
	func(data);	/* should not return */
}

static void pcpu_delegate(struct pcpu *pcpu,
			  pcpu_delegate_fn *func,
			  void *data, unsigned long stack)
{
	struct lowcore *lc, *abs_lc;
	unsigned int source_cpu;

	lc = lowcore_ptr[pcpu - pcpu_devices];
	source_cpu = stap();

	if (pcpu->address == source_cpu) {
		call_on_stack(2, stack, void, __pcpu_delegate,
			      pcpu_delegate_fn *, func, void *, data);
	}
	/* Stop target cpu (if func returns this stops the current cpu). */
	pcpu_sigp_retry(pcpu, SIGP_STOP, 0);
	pcpu_sigp_retry(pcpu, SIGP_CPU_RESET, 0);
	/* Restart func on the target cpu and stop the current cpu. */
	if (lc) {
		lc->restart_stack = stack;
		lc->restart_fn = (unsigned long)func;
		lc->restart_data = (unsigned long)data;
		lc->restart_source = source_cpu;
	} else {
		abs_lc = get_abs_lowcore();
		abs_lc->restart_stack = stack;
		abs_lc->restart_fn = (unsigned long)func;
		abs_lc->restart_data = (unsigned long)data;
		abs_lc->restart_source = source_cpu;
		put_abs_lowcore(abs_lc);
	}
	asm volatile(
		"0:	sigp	0,%0,%2	# sigp restart to target cpu\n"
		"	brc	2,0b	# busy, try again\n"
		"1:	sigp	0,%1,%3	# sigp stop to current cpu\n"
		"	brc	2,1b	# busy, try again\n"
		: : "d" (pcpu->address), "d" (source_cpu),
		    "K" (SIGP_RESTART), "K" (SIGP_STOP)
		: "0", "1", "cc");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (;;) ;
}

/*
<<<<<<< HEAD
=======
 * Enable additional logical cpus for multi-threading.
 */
static int pcpu_set_smt(unsigned int mtid)
{
	int cc;

	if (smp_cpu_mtid == mtid)
		return 0;
	cc = __pcpu_sigp(0, SIGP_SET_MULTI_THREADING, mtid, NULL);
	if (cc == 0) {
		smp_cpu_mtid = mtid;
		smp_cpu_mt_shift = 0;
		while (smp_cpu_mtid >= (1U << smp_cpu_mt_shift))
			smp_cpu_mt_shift++;
		pcpu_devices[0].address = stap();
	}
	return cc;
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Call function on an online CPU.
 */
void smp_call_online_cpu(void (*func)(void *), void *data)
{
	struct pcpu *pcpu;

	/* Use the current cpu if it is online. */
	pcpu = pcpu_find_address(cpu_online_mask, stap());
	if (!pcpu)
		/* Use the first online cpu. */
		pcpu = pcpu_devices + cpumask_first(cpu_online_mask);
	pcpu_delegate(pcpu, func, data, (unsigned long) restart_stack);
}

/*
 * Call function on the ipl CPU.
 */
void smp_call_ipl_cpu(void (*func)(void *), void *data)
{
<<<<<<< HEAD
	pcpu_delegate(&pcpu_devices[0], func, data,
		      pcpu_devices->panic_stack + PAGE_SIZE);
=======
	struct lowcore *lc = lowcore_ptr[0];

	if (pcpu_devices[0].address == stap())
		lc = &S390_lowcore;

	pcpu_delegate(&pcpu_devices[0], func, data,
		      lc->nodat_stack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int smp_find_processor_id(u16 address)
{
	int cpu;

	for_each_present_cpu(cpu)
		if (pcpu_devices[cpu].address == address)
			return cpu;
	return -1;
}

<<<<<<< HEAD
int smp_vcpu_scheduled(int cpu)
{
	return pcpu_running(pcpu_devices + cpu);
}

void smp_yield(void)
{
	if (MACHINE_HAS_DIAG44)
		asm volatile("diag 0,0,0x44");
}

void smp_yield_cpu(int cpu)
{
	if (MACHINE_HAS_DIAG9C)
		asm volatile("diag %0,0,0x9c"
			     : : "d" (pcpu_devices[cpu].address));
	else if (MACHINE_HAS_DIAG44)
		asm volatile("diag 0,0,0x44");
}
=======
void schedule_mcck_handler(void)
{
	pcpu_ec_call(pcpu_devices + smp_processor_id(), ec_mcck_pending);
}

bool notrace arch_vcpu_is_preempted(int cpu)
{
	if (test_cpu_flag_of(CIF_ENABLED_WAIT, cpu))
		return false;
	if (pcpu_running(pcpu_devices + cpu))
		return false;
	return true;
}
EXPORT_SYMBOL(arch_vcpu_is_preempted);

void notrace smp_yield_cpu(int cpu)
{
	if (!MACHINE_HAS_DIAG9C)
		return;
	diag_stat_inc_norecursion(DIAG_STAT_X09C);
	asm volatile("diag %0,0,0x9c"
		     : : "d" (pcpu_devices[cpu].address));
}
EXPORT_SYMBOL_GPL(smp_yield_cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Send cpus emergency shutdown signal. This gives the cpus the
 * opportunity to complete outstanding interrupts.
 */
<<<<<<< HEAD
void smp_emergency_stop(cpumask_t *cpumask)
{
	u64 end;
	int cpu;

	end = get_clock() + (1000000UL << 12);
	for_each_cpu(cpu, cpumask) {
		struct pcpu *pcpu = pcpu_devices + cpu;
		set_bit(ec_stop_cpu, &pcpu->ec_mask);
		while (__pcpu_sigp(pcpu->address, sigp_emergency_signal,
				   0, NULL) == sigp_busy &&
		       get_clock() < end)
			cpu_relax();
	}
	while (get_clock() < end) {
		for_each_cpu(cpu, cpumask)
			if (pcpu_stopped(pcpu_devices + cpu))
				cpumask_clear_cpu(cpu, cpumask);
		if (cpumask_empty(cpumask))
			break;
		cpu_relax();
	}
}
=======
void notrace smp_emergency_stop(void)
{
	static arch_spinlock_t lock = __ARCH_SPIN_LOCK_UNLOCKED;
	static cpumask_t cpumask;
	u64 end;
	int cpu;

	arch_spin_lock(&lock);
	cpumask_copy(&cpumask, cpu_online_mask);
	cpumask_clear_cpu(smp_processor_id(), &cpumask);

	end = get_tod_clock() + (1000000UL << 12);
	for_each_cpu(cpu, &cpumask) {
		struct pcpu *pcpu = pcpu_devices + cpu;
		set_bit(ec_stop_cpu, &pcpu->ec_mask);
		while (__pcpu_sigp(pcpu->address, SIGP_EMERGENCY_SIGNAL,
				   0, NULL) == SIGP_CC_BUSY &&
		       get_tod_clock() < end)
			cpu_relax();
	}
	while (get_tod_clock() < end) {
		for_each_cpu(cpu, &cpumask)
			if (pcpu_stopped(pcpu_devices + cpu))
				cpumask_clear_cpu(cpu, &cpumask);
		if (cpumask_empty(&cpumask))
			break;
		cpu_relax();
	}
	arch_spin_unlock(&lock);
}
NOKPROBE_SYMBOL(smp_emergency_stop);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Stop all cpus but the current one.
 */
void smp_send_stop(void)
{
<<<<<<< HEAD
	cpumask_t cpumask;
	int cpu;

	/* Disable all interrupts/machine checks */
	__load_psw_mask(psw_kernel_bits | PSW_MASK_DAT);
	trace_hardirqs_off();

	debug_set_critical();
	cpumask_copy(&cpumask, cpu_online_mask);
	cpumask_clear_cpu(smp_processor_id(), &cpumask);

	if (oops_in_progress)
		smp_emergency_stop(&cpumask);

	/* stop all processors */
	for_each_cpu(cpu, &cpumask) {
		struct pcpu *pcpu = pcpu_devices + cpu;
		pcpu_sigp_retry(pcpu, sigp_stop, 0);
		while (!pcpu_stopped(pcpu))
=======
	int cpu;

	/* Disable all interrupts/machine checks */
	__load_psw_mask(PSW_KERNEL_BITS);
	trace_hardirqs_off();

	debug_set_critical();

	if (oops_in_progress)
		smp_emergency_stop();

	/* stop all processors */
	for_each_online_cpu(cpu) {
		if (cpu == smp_processor_id())
			continue;
		pcpu_sigp_retry(pcpu_devices + cpu, SIGP_STOP, 0);
		while (!pcpu_stopped(pcpu_devices + cpu))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			cpu_relax();
	}
}

/*
<<<<<<< HEAD
 * Stop the current cpu.
 */
void smp_stop_cpu(void)
{
	pcpu_sigp_retry(pcpu_devices + smp_processor_id(), sigp_stop, 0);
	for (;;) ;
}

/*
 * This is the main routine where commands issued by other
 * cpus are handled.
 */
static void do_ext_call_interrupt(struct ext_code ext_code,
				  unsigned int param32, unsigned long param64)
{
	unsigned long bits;
	int cpu;

	cpu = smp_processor_id();
	if (ext_code.code == 0x1202)
		kstat_cpu(cpu).irqs[EXTINT_EXC]++;
	else
		kstat_cpu(cpu).irqs[EXTINT_EMS]++;
	/*
	 * handle bit signal external calls
	 */
	bits = xchg(&pcpu_devices[cpu].ec_mask, 0);

	if (test_bit(ec_stop_cpu, &bits))
		smp_stop_cpu();

	if (test_bit(ec_schedule, &bits))
		scheduler_ipi();

	if (test_bit(ec_call_function, &bits))
		generic_smp_call_function_interrupt();

	if (test_bit(ec_call_function_single, &bits))
		generic_smp_call_function_single_interrupt();

=======
 * This is the main routine where commands issued by other
 * cpus are handled.
 */
static void smp_handle_ext_call(void)
{
	unsigned long bits;

	/* handle bit signal external calls */
	bits = xchg(&pcpu_devices[smp_processor_id()].ec_mask, 0);
	if (test_bit(ec_stop_cpu, &bits))
		smp_stop_cpu();
	if (test_bit(ec_schedule, &bits))
		scheduler_ipi();
	if (test_bit(ec_call_function_single, &bits))
		generic_smp_call_function_single_interrupt();
	if (test_bit(ec_mcck_pending, &bits))
		s390_handle_mcck();
	if (test_bit(ec_irq_work, &bits))
		irq_work_run();
}

static void do_ext_call_interrupt(struct ext_code ext_code,
				  unsigned int param32, unsigned long param64)
{
	inc_irq_stat(ext_code.code == 0x1202 ? IRQEXT_EXC : IRQEXT_EMS);
	smp_handle_ext_call();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
	int cpu;

	for_each_cpu(cpu, mask)
<<<<<<< HEAD
		pcpu_ec_call(pcpu_devices + cpu, ec_call_function);
=======
		pcpu_ec_call(pcpu_devices + cpu, ec_call_function_single);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void arch_send_call_function_single_ipi(int cpu)
{
	pcpu_ec_call(pcpu_devices + cpu, ec_call_function_single);
}

<<<<<<< HEAD
#ifndef CONFIG_64BIT
/*
 * this function sends a 'purge tlb' signal to another CPU.
 */
static void smp_ptlb_callback(void *info)
{
	__tlb_flush_local();
}

void smp_ptlb_all(void)
{
	on_each_cpu(smp_ptlb_callback, NULL, 1);
}
EXPORT_SYMBOL(smp_ptlb_all);
#endif /* ! CONFIG_64BIT */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * this function sends a 'reschedule' IPI to another CPU.
 * it goes straight through and wastes no time serializing
 * anything. Worst case is that we lose a reschedule ...
 */
<<<<<<< HEAD
void smp_send_reschedule(int cpu)
=======
void arch_smp_send_reschedule(int cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	pcpu_ec_call(pcpu_devices + cpu, ec_schedule);
}

<<<<<<< HEAD
/*
 * parameter area for the set/clear control bit callbacks
 */
struct ec_creg_mask_parms {
	unsigned long orval;
	unsigned long andval;
	int cr;
};

/*
 * callback for setting/clearing control bits
 */
static void smp_ctl_bit_callback(void *info)
{
	struct ec_creg_mask_parms *pp = info;
	unsigned long cregs[16];

	__ctl_store(cregs, 0, 15);
	cregs[pp->cr] = (cregs[pp->cr] & pp->andval) | pp->orval;
	__ctl_load(cregs, 0, 15);
}

/*
 * Set a bit in a control register of all cpus
 */
void smp_ctl_set_bit(int cr, int bit)
{
	struct ec_creg_mask_parms parms = { 1UL << bit, -1UL, cr };

	on_each_cpu(smp_ctl_bit_callback, &parms, 1);
}
EXPORT_SYMBOL(smp_ctl_set_bit);

/*
 * Clear a bit in a control register of all cpus
 */
void smp_ctl_clear_bit(int cr, int bit)
{
	struct ec_creg_mask_parms parms = { 0, ~(1UL << bit), cr };

	on_each_cpu(smp_ctl_bit_callback, &parms, 1);
}
EXPORT_SYMBOL(smp_ctl_clear_bit);

#if defined(CONFIG_ZFCPDUMP) || defined(CONFIG_CRASH_DUMP)

struct save_area *zfcpdump_save_areas[NR_CPUS + 1];
EXPORT_SYMBOL_GPL(zfcpdump_save_areas);

static void __init smp_get_save_area(int cpu, u16 address)
{
	void *lc = pcpu_devices[0].lowcore;
	struct save_area *save_area;

	if (is_kdump_kernel())
		return;
	if (!OLDMEM_BASE && (address == boot_cpu_address ||
			     ipl_info.type != IPL_TYPE_FCP_DUMP))
		return;
	if (cpu >= NR_CPUS) {
		pr_warning("CPU %i exceeds the maximum %i and is excluded "
			   "from the dump\n", cpu, NR_CPUS - 1);
		return;
	}
	save_area = kmalloc(sizeof(struct save_area), GFP_KERNEL);
	if (!save_area)
		panic("could not allocate memory for save area\n");
	zfcpdump_save_areas[cpu] = save_area;
#ifdef CONFIG_CRASH_DUMP
	if (address == boot_cpu_address) {
		/* Copy the registers of the boot cpu. */
		copy_oldmem_page(1, (void *) save_area, sizeof(*save_area),
				 SAVE_AREA_BASE - PAGE_SIZE, 0);
		return;
	}
#endif
	/* Get the registers of a non-boot cpu. */
	__pcpu_sigp_relax(address, sigp_stop_and_store_status, 0, NULL);
	memcpy_real(save_area, lc + SAVE_AREA_BASE, sizeof(*save_area));
}

int smp_store_status(int cpu)
{
	struct pcpu *pcpu;

	pcpu = pcpu_devices + cpu;
	if (__pcpu_sigp_relax(pcpu->address, sigp_stop_and_store_status,
			      0, NULL) != sigp_order_code_accepted)
=======
#ifdef CONFIG_IRQ_WORK
void arch_irq_work_raise(void)
{
	pcpu_ec_call(pcpu_devices + smp_processor_id(), ec_irq_work);
}
#endif

#ifdef CONFIG_CRASH_DUMP

int smp_store_status(int cpu)
{
	struct lowcore *lc;
	struct pcpu *pcpu;
	unsigned long pa;

	pcpu = pcpu_devices + cpu;
	lc = lowcore_ptr[cpu];
	pa = __pa(&lc->floating_pt_save_area);
	if (__pcpu_sigp_relax(pcpu->address, SIGP_STORE_STATUS_AT_ADDRESS,
			      pa) != SIGP_CC_ORDER_CODE_ACCEPTED)
		return -EIO;
	if (!cpu_has_vx() && !MACHINE_HAS_GS)
		return 0;
	pa = lc->mcesad & MCESA_ORIGIN_MASK;
	if (MACHINE_HAS_GS)
		pa |= lc->mcesad & MCESA_LC_MASK;
	if (__pcpu_sigp_relax(pcpu->address, SIGP_STORE_ADDITIONAL_STATUS,
			      pa) != SIGP_CC_ORDER_CODE_ACCEPTED)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EIO;
	return 0;
}

<<<<<<< HEAD
#else /* CONFIG_ZFCPDUMP || CONFIG_CRASH_DUMP */

static inline void smp_get_save_area(int cpu, u16 address) { }

#endif /* CONFIG_ZFCPDUMP || CONFIG_CRASH_DUMP */

static struct sclp_cpu_info *smp_get_cpu_info(void)
{
	static int use_sigp_detection;
	struct sclp_cpu_info *info;
	int address;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (info && (use_sigp_detection || sclp_get_cpu_info(info))) {
		use_sigp_detection = 1;
		for (address = 0; address <= MAX_CPU_ADDRESS; address++) {
			if (__pcpu_sigp_relax(address, sigp_sense, 0, NULL) ==
			    sigp_not_operational)
				continue;
			info->cpu[info->configured].address = address;
=======
/*
 * Collect CPU state of the previous, crashed system.
 * There are four cases:
 * 1) standard zfcp/nvme dump
 *    condition: OLDMEM_BASE == NULL && is_ipl_type_dump() == true
 *    The state for all CPUs except the boot CPU needs to be collected
 *    with sigp stop-and-store-status. The boot CPU state is located in
 *    the absolute lowcore of the memory stored in the HSA. The zcore code
 *    will copy the boot CPU state from the HSA.
 * 2) stand-alone kdump for SCSI/NVMe (zfcp/nvme dump with swapped memory)
 *    condition: OLDMEM_BASE != NULL && is_ipl_type_dump() == true
 *    The state for all CPUs except the boot CPU needs to be collected
 *    with sigp stop-and-store-status. The firmware or the boot-loader
 *    stored the registers of the boot CPU in the absolute lowcore in the
 *    memory of the old system.
 * 3) kdump and the old kernel did not store the CPU state,
 *    or stand-alone kdump for DASD
 *    condition: OLDMEM_BASE != NULL && !is_kdump_kernel()
 *    The state for all CPUs except the boot CPU needs to be collected
 *    with sigp stop-and-store-status. The kexec code or the boot-loader
 *    stored the registers of the boot CPU in the memory of the old system.
 * 4) kdump and the old kernel stored the CPU state
 *    condition: OLDMEM_BASE != NULL && is_kdump_kernel()
 *    This case does not exist for s390 anymore, setup_arch explicitly
 *    deactivates the elfcorehdr= kernel parameter
 */
static bool dump_available(void)
{
	return oldmem_data.start || is_ipl_type_dump();
}

void __init smp_save_dump_ipl_cpu(void)
{
	struct save_area *sa;
	void *regs;

	if (!dump_available())
		return;
	sa = save_area_alloc(true);
	regs = memblock_alloc(512, 8);
	if (!sa || !regs)
		panic("could not allocate memory for boot CPU save area\n");
	copy_oldmem_kernel(regs, __LC_FPREGS_SAVE_AREA, 512);
	save_area_add_regs(sa, regs);
	memblock_free(regs, 512);
	if (cpu_has_vx())
		save_area_add_vxrs(sa, boot_cpu_vector_save_area);
}

void __init smp_save_dump_secondary_cpus(void)
{
	int addr, boot_cpu_addr, max_cpu_addr;
	struct save_area *sa;
	void *page;

	if (!dump_available())
		return;
	/* Allocate a page as dumping area for the store status sigps */
	page = memblock_alloc_low(PAGE_SIZE, PAGE_SIZE);
	if (!page)
		panic("ERROR: Failed to allocate %lx bytes below %lx\n",
		      PAGE_SIZE, 1UL << 31);

	/* Set multi-threading state to the previous system. */
	pcpu_set_smt(sclp.mtid_prev);
	boot_cpu_addr = stap();
	max_cpu_addr = SCLP_MAX_CORES << sclp.mtid_prev;
	for (addr = 0; addr <= max_cpu_addr; addr++) {
		if (addr == boot_cpu_addr)
			continue;
		if (__pcpu_sigp_relax(addr, SIGP_SENSE, 0) ==
		    SIGP_CC_NOT_OPERATIONAL)
			continue;
		sa = save_area_alloc(false);
		if (!sa)
			panic("could not allocate memory for save area\n");
		__pcpu_sigp_relax(addr, SIGP_STORE_STATUS_AT_ADDRESS, __pa(page));
		save_area_add_regs(sa, page);
		if (cpu_has_vx()) {
			__pcpu_sigp_relax(addr, SIGP_STORE_ADDITIONAL_STATUS, __pa(page));
			save_area_add_vxrs(sa, page);
		}
	}
	memblock_free(page, PAGE_SIZE);
	diag_amode31_ops.diag308_reset();
	pcpu_set_smt(0);
}
#endif /* CONFIG_CRASH_DUMP */

void smp_cpu_set_polarization(int cpu, int val)
{
	pcpu_devices[cpu].polarization = val;
}

int smp_cpu_get_polarization(int cpu)
{
	return pcpu_devices[cpu].polarization;
}

int smp_cpu_get_cpu_address(int cpu)
{
	return pcpu_devices[cpu].address;
}

static void __ref smp_get_core_info(struct sclp_core_info *info, int early)
{
	static int use_sigp_detection;
	int address;

	if (use_sigp_detection || sclp_get_core_info(info, early)) {
		use_sigp_detection = 1;
		for (address = 0;
		     address < (SCLP_MAX_CORES << smp_cpu_mt_shift);
		     address += (1U << smp_cpu_mt_shift)) {
			if (__pcpu_sigp_relax(address, SIGP_SENSE, 0) ==
			    SIGP_CC_NOT_OPERATIONAL)
				continue;
			info->core[info->configured].core_id =
				address >> smp_cpu_mt_shift;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			info->configured++;
		}
		info->combined = info->configured;
	}
<<<<<<< HEAD
	return info;
}

static int __devinit smp_add_present_cpu(int cpu);

static int __devinit __smp_rescan_cpus(struct sclp_cpu_info *info,
				       int sysfs_add)
{
	struct pcpu *pcpu;
	cpumask_t avail;
	int cpu, nr, i;

	nr = 0;
	cpumask_xor(&avail, cpu_possible_mask, cpu_present_mask);
	cpu = cpumask_first(&avail);
	for (i = 0; (i < info->combined) && (cpu < nr_cpu_ids); i++) {
		if (info->has_cpu_type && info->cpu[i].type != boot_cpu_type)
			continue;
		if (pcpu_find_address(cpu_present_mask, info->cpu[i].address))
			continue;
		pcpu = pcpu_devices + cpu;
		pcpu->address = info->cpu[i].address;
		pcpu->state = (cpu >= info->configured) ?
			CPU_STATE_STANDBY : CPU_STATE_CONFIGURED;
		cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
		set_cpu_present(cpu, true);
		if (sysfs_add && smp_add_present_cpu(cpu) != 0)
			set_cpu_present(cpu, false);
		else
			nr++;
		cpu = cpumask_next(cpu, &avail);
=======
}

static int smp_add_present_cpu(int cpu);

static int smp_add_core(struct sclp_core_entry *core, cpumask_t *avail,
			bool configured, bool early)
{
	struct pcpu *pcpu;
	int cpu, nr, i;
	u16 address;

	nr = 0;
	if (sclp.has_core_type && core->type != boot_core_type)
		return nr;
	cpu = cpumask_first(avail);
	address = core->core_id << smp_cpu_mt_shift;
	for (i = 0; (i <= smp_cpu_mtid) && (cpu < nr_cpu_ids); i++) {
		if (pcpu_find_address(cpu_present_mask, address + i))
			continue;
		pcpu = pcpu_devices + cpu;
		pcpu->address = address + i;
		if (configured)
			pcpu->state = CPU_STATE_CONFIGURED;
		else
			pcpu->state = CPU_STATE_STANDBY;
		smp_cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
		set_cpu_present(cpu, true);
		if (!early && smp_add_present_cpu(cpu) != 0)
			set_cpu_present(cpu, false);
		else
			nr++;
		cpumask_clear_cpu(cpu, avail);
		cpu = cpumask_next(cpu, avail);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return nr;
}

<<<<<<< HEAD
static void __init smp_detect_cpus(void)
{
	unsigned int cpu, c_cpus, s_cpus;
	struct sclp_cpu_info *info;

	info = smp_get_cpu_info();
	if (!info)
		panic("smp_detect_cpus failed to allocate memory\n");
	if (info->has_cpu_type) {
		for (cpu = 0; cpu < info->combined; cpu++) {
			if (info->cpu[cpu].address != boot_cpu_address)
				continue;
			/* The boot cpu dictates the cpu type. */
			boot_cpu_type = info->cpu[cpu].type;
			break;
		}
	}
	c_cpus = s_cpus = 0;
	for (cpu = 0; cpu < info->combined; cpu++) {
		if (info->has_cpu_type && info->cpu[cpu].type != boot_cpu_type)
			continue;
		if (cpu < info->configured) {
			smp_get_save_area(c_cpus, info->cpu[cpu].address);
			c_cpus++;
		} else
			s_cpus++;
	}
	pr_info("%d configured CPUs, %d standby CPUs\n", c_cpus, s_cpus);
	get_online_cpus();
	__smp_rescan_cpus(info, 0);
	put_online_cpus();
	kfree(info);
=======
static int __smp_rescan_cpus(struct sclp_core_info *info, bool early)
{
	struct sclp_core_entry *core;
	static cpumask_t avail;
	bool configured;
	u16 core_id;
	int nr, i;

	cpus_read_lock();
	mutex_lock(&smp_cpu_state_mutex);
	nr = 0;
	cpumask_xor(&avail, cpu_possible_mask, cpu_present_mask);
	/*
	 * Add IPL core first (which got logical CPU number 0) to make sure
	 * that all SMT threads get subsequent logical CPU numbers.
	 */
	if (early) {
		core_id = pcpu_devices[0].address >> smp_cpu_mt_shift;
		for (i = 0; i < info->configured; i++) {
			core = &info->core[i];
			if (core->core_id == core_id) {
				nr += smp_add_core(core, &avail, true, early);
				break;
			}
		}
	}
	for (i = 0; i < info->combined; i++) {
		configured = i < info->configured;
		nr += smp_add_core(&info->core[i], &avail, configured, early);
	}
	mutex_unlock(&smp_cpu_state_mutex);
	cpus_read_unlock();
	return nr;
}

void __init smp_detect_cpus(void)
{
	unsigned int cpu, mtid, c_cpus, s_cpus;
	struct sclp_core_info *info;
	u16 address;

	/* Get CPU information */
	info = memblock_alloc(sizeof(*info), 8);
	if (!info)
		panic("%s: Failed to allocate %zu bytes align=0x%x\n",
		      __func__, sizeof(*info), 8);
	smp_get_core_info(info, 1);
	/* Find boot CPU type */
	if (sclp.has_core_type) {
		address = stap();
		for (cpu = 0; cpu < info->combined; cpu++)
			if (info->core[cpu].core_id == address) {
				/* The boot cpu dictates the cpu type. */
				boot_core_type = info->core[cpu].type;
				break;
			}
		if (cpu >= info->combined)
			panic("Could not find boot CPU type");
	}

	/* Set multi-threading state for the current system */
	mtid = boot_core_type ? sclp.mtid : sclp.mtid_cp;
	mtid = (mtid < smp_max_threads) ? mtid : smp_max_threads - 1;
	pcpu_set_smt(mtid);

	/* Print number of CPUs */
	c_cpus = s_cpus = 0;
	for (cpu = 0; cpu < info->combined; cpu++) {
		if (sclp.has_core_type &&
		    info->core[cpu].type != boot_core_type)
			continue;
		if (cpu < info->configured)
			c_cpus += smp_cpu_mtid + 1;
		else
			s_cpus += smp_cpu_mtid + 1;
	}
	pr_info("%d configured CPUs, %d standby CPUs\n", c_cpus, s_cpus);

	/* Add CPUs present at boot */
	__smp_rescan_cpus(info, true);
	memblock_free(info, sizeof(*info));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Activate a secondary processor.
 */
<<<<<<< HEAD
static void __cpuinit smp_start_secondary(void *cpuvoid)
{
	S390_lowcore.last_update_clock = get_clock();
	S390_lowcore.restart_stack = (unsigned long) restart_stack;
	S390_lowcore.restart_fn = (unsigned long) do_restart;
	S390_lowcore.restart_data = 0;
	S390_lowcore.restart_source = -1UL;
	restore_access_regs(S390_lowcore.access_regs_save_area);
	__ctl_load(S390_lowcore.cregs_save_area, 0, 15);
	__load_psw_mask(psw_kernel_bits | PSW_MASK_DAT);
	cpu_init();
	preempt_disable();
	init_cpu_timer();
	init_cpu_vtimer();
	pfault_init();
	notify_cpu_starting(smp_processor_id());
	ipi_call_lock();
	set_cpu_online(smp_processor_id(), true);
	ipi_call_unlock();
	local_irq_enable();
	/* cpu_idle will call schedule for us */
	cpu_idle();
}

struct create_idle {
	struct work_struct work;
	struct task_struct *idle;
	struct completion done;
	int cpu;
};

static void __cpuinit smp_fork_idle(struct work_struct *work)
{
	struct create_idle *c_idle;

	c_idle = container_of(work, struct create_idle, work);
	c_idle->idle = fork_idle(c_idle->cpu);
	complete(&c_idle->done);
}

/* Upping and downing of CPUs */
int __cpuinit __cpu_up(unsigned int cpu)
{
	struct create_idle c_idle;
	struct pcpu *pcpu;
	int rc;

	pcpu = pcpu_devices + cpu;
	if (pcpu->state != CPU_STATE_CONFIGURED)
		return -EIO;
	if (pcpu_sigp_retry(pcpu, sigp_initial_cpu_reset, 0) !=
	    sigp_order_code_accepted)
		return -EIO;
	if (!pcpu->idle) {
		c_idle.done = COMPLETION_INITIALIZER_ONSTACK(c_idle.done);
		INIT_WORK_ONSTACK(&c_idle.work, smp_fork_idle);
		c_idle.cpu = cpu;
		schedule_work(&c_idle.work);
		wait_for_completion(&c_idle.done);
		if (IS_ERR(c_idle.idle))
			return PTR_ERR(c_idle.idle);
		pcpu->idle = c_idle.idle;
	}
	init_idle(pcpu->idle, cpu);
	rc = pcpu_alloc_lowcore(pcpu, cpu);
	if (rc)
		return rc;
	pcpu_prepare_secondary(pcpu, cpu);
	pcpu_attach_task(pcpu, pcpu->idle);
	pcpu_start_fn(pcpu, smp_start_secondary, NULL);
	while (!cpu_online(cpu))
		cpu_relax();
	return 0;
}

static int __init setup_possible_cpus(char *s)
{
	int max, cpu;

	if (kstrtoint(s, 0, &max) < 0)
		return 0;
	init_cpu_possible(cpumask_of(0));
	for (cpu = 1; cpu < max && cpu < nr_cpu_ids; cpu++)
		set_cpu_possible(cpu, true);
	return 0;
}
early_param("possible_cpus", setup_possible_cpus);

#ifdef CONFIG_HOTPLUG_CPU

int __cpu_disable(void)
{
	unsigned long cregs[16];

	set_cpu_online(smp_processor_id(), false);
	/* Disable pseudo page faults on this cpu. */
	pfault_fini();
	/* Disable interrupt sources via control register. */
	__ctl_store(cregs, 0, 15);
	cregs[0]  &= ~0x0000ee70UL;	/* disable all external interrupts */
	cregs[6]  &= ~0xff000000UL;	/* disable all I/O interrupts */
	cregs[14] &= ~0x1f000000UL;	/* disable most machine checks */
	__ctl_load(cregs, 0, 15);
=======
static void smp_start_secondary(void *cpuvoid)
{
	int cpu = raw_smp_processor_id();

	S390_lowcore.last_update_clock = get_tod_clock();
	S390_lowcore.restart_stack = (unsigned long)restart_stack;
	S390_lowcore.restart_fn = (unsigned long)do_restart;
	S390_lowcore.restart_data = 0;
	S390_lowcore.restart_source = -1U;
	S390_lowcore.restart_flags = 0;
	restore_access_regs(S390_lowcore.access_regs_save_area);
	cpu_init();
	rcutree_report_cpu_starting(cpu);
	init_cpu_timer();
	vtime_init();
	vdso_getcpu_init();
	pfault_init();
	cpumask_set_cpu(cpu, &cpu_setup_mask);
	update_cpu_masks();
	notify_cpu_starting(cpu);
	if (topology_cpu_dedicated(cpu))
		set_cpu_flag(CIF_DEDICATED_CPU);
	else
		clear_cpu_flag(CIF_DEDICATED_CPU);
	set_cpu_online(cpu, true);
	inc_irq_stat(CPU_RST);
	local_irq_enable();
	cpu_startup_entry(CPUHP_AP_ONLINE_IDLE);
}

/* Upping and downing of CPUs */
int __cpu_up(unsigned int cpu, struct task_struct *tidle)
{
	struct pcpu *pcpu = pcpu_devices + cpu;
	int rc;

	if (pcpu->state != CPU_STATE_CONFIGURED)
		return -EIO;
	if (pcpu_sigp_retry(pcpu, SIGP_INITIAL_CPU_RESET, 0) !=
	    SIGP_CC_ORDER_CODE_ACCEPTED)
		return -EIO;

	rc = pcpu_alloc_lowcore(pcpu, cpu);
	if (rc)
		return rc;
	/*
	 * Make sure global control register contents do not change
	 * until new CPU has initialized control registers.
	 */
	system_ctlreg_lock();
	pcpu_prepare_secondary(pcpu, cpu);
	pcpu_attach_task(pcpu, tidle);
	pcpu_start_fn(pcpu, smp_start_secondary, NULL);
	/* Wait until cpu puts itself in the online & active maps */
	while (!cpu_online(cpu))
		cpu_relax();
	system_ctlreg_unlock();
	return 0;
}

static unsigned int setup_possible_cpus __initdata;

static int __init _setup_possible_cpus(char *s)
{
	get_option(&s, &setup_possible_cpus);
	return 0;
}
early_param("possible_cpus", _setup_possible_cpus);

int __cpu_disable(void)
{
	struct ctlreg cregs[16];
	int cpu;

	/* Handle possible pending IPIs */
	smp_handle_ext_call();
	cpu = smp_processor_id();
	set_cpu_online(cpu, false);
	cpumask_clear_cpu(cpu, &cpu_setup_mask);
	update_cpu_masks();
	/* Disable pseudo page faults on this cpu. */
	pfault_fini();
	/* Disable interrupt sources via control register. */
	__local_ctl_store(0, 15, cregs);
	cregs[0].val  &= ~0x0000ee70UL;	/* disable all external interrupts */
	cregs[6].val  &= ~0xff000000UL;	/* disable all I/O interrupts */
	cregs[14].val &= ~0x1f000000UL;	/* disable most machine checks */
	__local_ctl_load(0, 15, cregs);
	clear_cpu_flag(CIF_NOHZ_DELAY);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

void __cpu_die(unsigned int cpu)
{
	struct pcpu *pcpu;

	/* Wait until target cpu is down */
	pcpu = pcpu_devices + cpu;
	while (!pcpu_stopped(pcpu))
		cpu_relax();
	pcpu_free_lowcore(pcpu);
<<<<<<< HEAD
	atomic_dec(&init_mm.context.attach_count);
=======
	cpumask_clear_cpu(cpu, mm_cpumask(&init_mm));
	cpumask_clear_cpu(cpu, &init_mm.context.cpu_attach_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __noreturn cpu_die(void)
{
	idle_task_exit();
<<<<<<< HEAD
	pcpu_sigp_retry(pcpu_devices + smp_processor_id(), sigp_stop, 0);
	for (;;) ;
}

#endif /* CONFIG_HOTPLUG_CPU */

static void smp_call_os_info_init_fn(void)
{
	int (*init_fn)(void);
	unsigned long size;

	init_fn = os_info_old_entry(OS_INFO_INIT_FN, &size);
	if (!init_fn)
		return;
	init_fn();
=======
	pcpu_sigp_retry(pcpu_devices + smp_processor_id(), SIGP_STOP, 0);
	for (;;) ;
}

void __init smp_fill_possible_mask(void)
{
	unsigned int possible, sclp_max, cpu;

	sclp_max = max(sclp.mtid, sclp.mtid_cp) + 1;
	sclp_max = min(smp_max_threads, sclp_max);
	sclp_max = (sclp.max_cores * sclp_max) ?: nr_cpu_ids;
	possible = setup_possible_cpus ?: nr_cpu_ids;
	possible = min(possible, sclp_max);
	for (cpu = 0; cpu < possible && cpu < nr_cpu_ids; cpu++)
		set_cpu_possible(cpu, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init smp_prepare_cpus(unsigned int max_cpus)
{
<<<<<<< HEAD
	/* request the 0x1201 emergency signal external interrupt */
	if (register_external_interrupt(0x1201, do_ext_call_interrupt) != 0)
		panic("Couldn't request external interrupt 0x1201");
	/* request the 0x1202 external call external interrupt */
	if (register_external_interrupt(0x1202, do_ext_call_interrupt) != 0)
		panic("Couldn't request external interrupt 0x1202");
	smp_call_os_info_init_fn();
	smp_detect_cpus();
=======
	if (register_external_irq(EXT_IRQ_EMERGENCY_SIG, do_ext_call_interrupt))
		panic("Couldn't request external interrupt 0x1201");
	system_ctl_set_bit(0, 14);
	if (register_external_irq(EXT_IRQ_EXTERNAL_CALL, do_ext_call_interrupt))
		panic("Couldn't request external interrupt 0x1202");
	system_ctl_set_bit(0, 13);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init smp_prepare_boot_cpu(void)
{
	struct pcpu *pcpu = pcpu_devices;

<<<<<<< HEAD
	boot_cpu_address = stap();
	pcpu->idle = current;
	pcpu->state = CPU_STATE_CONFIGURED;
	pcpu->address = boot_cpu_address;
	pcpu->lowcore = (struct _lowcore *)(unsigned long) store_prefix();
	pcpu->async_stack = S390_lowcore.async_stack - ASYNC_SIZE;
	pcpu->panic_stack = S390_lowcore.panic_stack - PAGE_SIZE;
	S390_lowcore.percpu_offset = __per_cpu_offset[0];
	cpu_set_polarization(0, POLARIZATION_UNKNOWN);
	set_cpu_present(0, true);
	set_cpu_online(0, true);
}

void __init smp_cpus_done(unsigned int max_cpus)
{
=======
	WARN_ON(!cpu_present(0) || !cpu_online(0));
	pcpu->state = CPU_STATE_CONFIGURED;
	S390_lowcore.percpu_offset = __per_cpu_offset[0];
	smp_cpu_set_polarization(0, POLARIZATION_UNKNOWN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init smp_setup_processor_id(void)
{
<<<<<<< HEAD
	S390_lowcore.cpu_nr = 0;
=======
	pcpu_devices[0].address = stap();
	S390_lowcore.cpu_nr = 0;
	S390_lowcore.spinlock_lockval = arch_spin_lockval(0);
	S390_lowcore.spinlock_index = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * the frequency of the profiling timer can be changed
 * by writing a multiplier value into /proc/profile.
 *
 * usually you want to run this on all CPUs ;)
 */
int setup_profiling_timer(unsigned int multiplier)
{
	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t cpu_configure_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{
	ssize_t count;

	mutex_lock(&smp_cpu_state_mutex);
	count = sprintf(buf, "%d\n", pcpu_devices[dev->id].state);
	mutex_unlock(&smp_cpu_state_mutex);
	return count;
}

static ssize_t cpu_configure_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct pcpu *pcpu;
<<<<<<< HEAD
	int cpu, val, rc;
=======
	int cpu, val, rc, i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char delim;

	if (sscanf(buf, "%d %c", &val, &delim) != 1)
		return -EINVAL;
	if (val != 0 && val != 1)
		return -EINVAL;
<<<<<<< HEAD
	get_online_cpus();
	mutex_lock(&smp_cpu_state_mutex);
	rc = -EBUSY;
	/* disallow configuration changes of online cpus and cpu 0 */
	cpu = dev->id;
	if (cpu_online(cpu) || cpu == 0)
		goto out;
=======
	cpus_read_lock();
	mutex_lock(&smp_cpu_state_mutex);
	rc = -EBUSY;
	/* disallow configuration changes of online cpus */
	cpu = dev->id;
	cpu = smp_get_base_cpu(cpu);
	for (i = 0; i <= smp_cpu_mtid; i++)
		if (cpu_online(cpu + i))
			goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pcpu = pcpu_devices + cpu;
	rc = 0;
	switch (val) {
	case 0:
		if (pcpu->state != CPU_STATE_CONFIGURED)
			break;
<<<<<<< HEAD
		rc = sclp_cpu_deconfigure(pcpu->address);
		if (rc)
			break;
		pcpu->state = CPU_STATE_STANDBY;
		cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
=======
		rc = sclp_core_deconfigure(pcpu->address >> smp_cpu_mt_shift);
		if (rc)
			break;
		for (i = 0; i <= smp_cpu_mtid; i++) {
			if (cpu + i >= nr_cpu_ids || !cpu_present(cpu + i))
				continue;
			pcpu[i].state = CPU_STATE_STANDBY;
			smp_cpu_set_polarization(cpu + i,
						 POLARIZATION_UNKNOWN);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		topology_expect_change();
		break;
	case 1:
		if (pcpu->state != CPU_STATE_STANDBY)
			break;
<<<<<<< HEAD
		rc = sclp_cpu_configure(pcpu->address);
		if (rc)
			break;
		pcpu->state = CPU_STATE_CONFIGURED;
		cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
=======
		rc = sclp_core_configure(pcpu->address >> smp_cpu_mt_shift);
		if (rc)
			break;
		for (i = 0; i <= smp_cpu_mtid; i++) {
			if (cpu + i >= nr_cpu_ids || !cpu_present(cpu + i))
				continue;
			pcpu[i].state = CPU_STATE_CONFIGURED;
			smp_cpu_set_polarization(cpu + i,
						 POLARIZATION_UNKNOWN);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		topology_expect_change();
		break;
	default:
		break;
	}
out:
	mutex_unlock(&smp_cpu_state_mutex);
<<<<<<< HEAD
	put_online_cpus();
	return rc ? rc : count;
}
static DEVICE_ATTR(configure, 0644, cpu_configure_show, cpu_configure_store);
#endif /* CONFIG_HOTPLUG_CPU */
=======
	cpus_read_unlock();
	return rc ? rc : count;
}
static DEVICE_ATTR(configure, 0644, cpu_configure_show, cpu_configure_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t show_cpu_address(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", pcpu_devices[dev->id].address);
}
static DEVICE_ATTR(address, 0444, show_cpu_address, NULL);

static struct attribute *cpu_common_attrs[] = {
<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU
	&dev_attr_configure.attr,
#endif
=======
	&dev_attr_configure.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&dev_attr_address.attr,
	NULL,
};

static struct attribute_group cpu_common_attr_group = {
	.attrs = cpu_common_attrs,
};

<<<<<<< HEAD
static ssize_t show_capability(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	unsigned int capability;
	int rc;

	rc = get_cpu_capability(&capability);
	if (rc)
		return rc;
	return sprintf(buf, "%u\n", capability);
}
static DEVICE_ATTR(capability, 0444, show_capability, NULL);

static ssize_t show_idle_count(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct s390_idle_data *idle = &per_cpu(s390_idle, dev->id);
	unsigned long long idle_count;
	unsigned int sequence;

	do {
		sequence = ACCESS_ONCE(idle->sequence);
		idle_count = ACCESS_ONCE(idle->idle_count);
		if (ACCESS_ONCE(idle->idle_enter))
			idle_count++;
	} while ((sequence & 1) || (idle->sequence != sequence));
	return sprintf(buf, "%llu\n", idle_count);
}
static DEVICE_ATTR(idle_count, 0444, show_idle_count, NULL);

static ssize_t show_idle_time(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct s390_idle_data *idle = &per_cpu(s390_idle, dev->id);
	unsigned long long now, idle_time, idle_enter, idle_exit;
	unsigned int sequence;

	do {
		now = get_clock();
		sequence = ACCESS_ONCE(idle->sequence);
		idle_time = ACCESS_ONCE(idle->idle_time);
		idle_enter = ACCESS_ONCE(idle->idle_enter);
		idle_exit = ACCESS_ONCE(idle->idle_exit);
	} while ((sequence & 1) || (idle->sequence != sequence));
	idle_time += idle_enter ? ((idle_exit ? : now) - idle_enter) : 0;
	return sprintf(buf, "%llu\n", idle_time >> 12);
}
static DEVICE_ATTR(idle_time_us, 0444, show_idle_time, NULL);

static struct attribute *cpu_online_attrs[] = {
	&dev_attr_capability.attr,
=======
static struct attribute *cpu_online_attrs[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&dev_attr_idle_count.attr,
	&dev_attr_idle_time_us.attr,
	NULL,
};

static struct attribute_group cpu_online_attr_group = {
	.attrs = cpu_online_attrs,
};

<<<<<<< HEAD
static int __cpuinit smp_cpu_notify(struct notifier_block *self,
				    unsigned long action, void *hcpu)
{
	unsigned int cpu = (unsigned int)(long)hcpu;
	struct cpu *c = &pcpu_devices[cpu].cpu;
	struct device *s = &c->dev;
	int err = 0;

	switch (action) {
	case CPU_ONLINE:
	case CPU_ONLINE_FROZEN:
		err = sysfs_create_group(&s->kobj, &cpu_online_attr_group);
		break;
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
		sysfs_remove_group(&s->kobj, &cpu_online_attr_group);
		break;
	}
	return notifier_from_errno(err);
}

static struct notifier_block __cpuinitdata smp_cpu_nb = {
	.notifier_call = smp_cpu_notify,
};

static int __devinit smp_add_present_cpu(int cpu)
{
	struct cpu *c = &pcpu_devices[cpu].cpu;
	struct device *s = &c->dev;
	int rc;

	c->hotpluggable = 1;
=======
static int smp_cpu_online(unsigned int cpu)
{
	struct device *s = &per_cpu(cpu_device, cpu)->dev;

	return sysfs_create_group(&s->kobj, &cpu_online_attr_group);
}

static int smp_cpu_pre_down(unsigned int cpu)
{
	struct device *s = &per_cpu(cpu_device, cpu)->dev;

	sysfs_remove_group(&s->kobj, &cpu_online_attr_group);
	return 0;
}

static int smp_add_present_cpu(int cpu)
{
	struct device *s;
	struct cpu *c;
	int rc;

	c = kzalloc(sizeof(*c), GFP_KERNEL);
	if (!c)
		return -ENOMEM;
	per_cpu(cpu_device, cpu) = c;
	s = &c->dev;
	c->hotpluggable = !!cpu;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = register_cpu(c, cpu);
	if (rc)
		goto out;
	rc = sysfs_create_group(&s->kobj, &cpu_common_attr_group);
	if (rc)
		goto out_cpu;
<<<<<<< HEAD
	if (cpu_online(cpu)) {
		rc = sysfs_create_group(&s->kobj, &cpu_online_attr_group);
		if (rc)
			goto out_online;
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = topology_cpu_init(c);
	if (rc)
		goto out_topology;
	return 0;

out_topology:
<<<<<<< HEAD
	if (cpu_online(cpu))
		sysfs_remove_group(&s->kobj, &cpu_online_attr_group);
out_online:
	sysfs_remove_group(&s->kobj, &cpu_common_attr_group);
out_cpu:
#ifdef CONFIG_HOTPLUG_CPU
	unregister_cpu(c);
#endif
=======
	sysfs_remove_group(&s->kobj, &cpu_common_attr_group);
out_cpu:
	unregister_cpu(c);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return rc;
}

<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU

int __ref smp_rescan_cpus(void)
{
	struct sclp_cpu_info *info;
	int nr;

	info = smp_get_cpu_info();
	if (!info)
		return -ENOMEM;
	get_online_cpus();
	mutex_lock(&smp_cpu_state_mutex);
	nr = __smp_rescan_cpus(info, 1);
	mutex_unlock(&smp_cpu_state_mutex);
	put_online_cpus();
=======
int __ref smp_rescan_cpus(void)
{
	struct sclp_core_info *info;
	int nr;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;
	smp_get_core_info(info, 0);
	nr = __smp_rescan_cpus(info, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(info);
	if (nr)
		topology_schedule_update();
	return 0;
}

static ssize_t __ref rescan_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf,
				  size_t count)
{
	int rc;

<<<<<<< HEAD
	rc = smp_rescan_cpus();
	return rc ? rc : count;
}
static DEVICE_ATTR(rescan, 0200, NULL, rescan_store);
#endif /* CONFIG_HOTPLUG_CPU */

static int __init s390_smp_init(void)
{
	int cpu, rc;

	register_cpu_notifier(&smp_cpu_nb);
#ifdef CONFIG_HOTPLUG_CPU
	rc = device_create_file(cpu_subsys.dev_root, &dev_attr_rescan);
	if (rc)
		return rc;
#endif
	for_each_present_cpu(cpu) {
		rc = smp_add_present_cpu(cpu);
		if (rc)
			return rc;
	}
	return 0;
=======
	rc = lock_device_hotplug_sysfs();
	if (rc)
		return rc;
	rc = smp_rescan_cpus();
	unlock_device_hotplug();
	return rc ? rc : count;
}
static DEVICE_ATTR_WO(rescan);

static int __init s390_smp_init(void)
{
	struct device *dev_root;
	int cpu, rc = 0;

	dev_root = bus_get_dev_root(&cpu_subsys);
	if (dev_root) {
		rc = device_create_file(dev_root, &dev_attr_rescan);
		put_device(dev_root);
		if (rc)
			return rc;
	}

	for_each_present_cpu(cpu) {
		rc = smp_add_present_cpu(cpu);
		if (rc)
			goto out;
	}

	rc = cpuhp_setup_state(CPUHP_AP_ONLINE_DYN, "s390/smp:online",
			       smp_cpu_online, smp_cpu_pre_down);
	rc = rc <= 0 ? rc : 0;
out:
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
subsys_initcall(s390_smp_init);
