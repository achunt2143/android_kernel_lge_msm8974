<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Count register synchronisation.
 *
 * All CPUs will have their count registers synchronised to the CPU0 next time
 * value. This can cause a small timewarp for CPU0. All other CPU's should
 * not have done anything significant (but they may have had interrupts
 * enabled briefly - prom_smp_finish() should not be responsible for enabling
 * interrupts...)
<<<<<<< HEAD
 *
 * FIXME: broken for SMTC
 */

#include <linux/kernel.h>
#include <linux/init.h>
=======
 */

#include <linux/kernel.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/irqflags.h>
#include <linux/cpumask.h>

#include <asm/r4k-timer.h>
#include <linux/atomic.h>
#include <asm/barrier.h>
#include <asm/mipsregs.h>

<<<<<<< HEAD
static atomic_t __cpuinitdata count_start_flag = ATOMIC_INIT(0);
static atomic_t __cpuinitdata count_count_start = ATOMIC_INIT(0);
static atomic_t __cpuinitdata count_count_stop = ATOMIC_INIT(0);
static atomic_t __cpuinitdata count_reference = ATOMIC_INIT(0);

#define COUNTON	100
#define NR_LOOPS 5

void __cpuinit synchronise_count_master(void)
{
	int i;
	unsigned long flags;
	unsigned int initcount;
	int nslaves;

#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 * SMTC needs to synchronise per VPE, not per CPU
	 * ignore for now
	 */
	return;
#endif

	printk(KERN_INFO "Synchronize counters across %u CPUs: ",
	       num_online_cpus());
=======
static unsigned int initcount = 0;
static atomic_t count_count_start = ATOMIC_INIT(0);
static atomic_t count_count_stop = ATOMIC_INIT(0);

#define COUNTON 100
#define NR_LOOPS 3

void synchronise_count_master(int cpu)
{
	int i;
	unsigned long flags;

	pr_info("Synchronize counters for CPU %u: ", cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	local_irq_save(flags);

	/*
<<<<<<< HEAD
	 * Notify the slaves that it's time to start
	 */
	atomic_set(&count_reference, read_c0_count());
	atomic_set(&count_start_flag, 1);
	smp_wmb();

	/* Count will be initialised to current timer for all CPU's */
	initcount = read_c0_count();

	/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * We loop a few times to get a primed instruction cache,
	 * then the last pass is more or less synchronised and
	 * the master and slaves each set their cycle counters to a known
	 * value all at once. This reduces the chance of having random offsets
	 * between the processors, and guarantees that the maximum
	 * delay between the cycle counters is never bigger than
	 * the latency of information-passing (cachelines) between
	 * two CPUs.
	 */

<<<<<<< HEAD
	nslaves = num_online_cpus()-1;
	for (i = 0; i < NR_LOOPS; i++) {
		/* slaves loop on '!= ncpus' */
		while (atomic_read(&count_count_start) != nslaves)
=======
	for (i = 0; i < NR_LOOPS; i++) {
		/* slaves loop on '!= 2' */
		while (atomic_read(&count_count_start) != 1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mb();
		atomic_set(&count_count_stop, 0);
		smp_wmb();

<<<<<<< HEAD
		/* this lets the slaves write their count register */
		atomic_inc(&count_count_start);

=======
		/* Let the slave writes its count register */
		atomic_inc(&count_count_start);

		/* Count will be initialised to current timer */
		if (i == 1)
			initcount = read_c0_count();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Everyone initialises count in the last loop:
		 */
		if (i == NR_LOOPS-1)
			write_c0_count(initcount);

		/*
<<<<<<< HEAD
		 * Wait for all slaves to leave the synchronization point:
		 */
		while (atomic_read(&count_count_stop) != nslaves)
=======
		 * Wait for slave to leave the synchronization point:
		 */
		while (atomic_read(&count_count_stop) != 1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mb();
		atomic_set(&count_count_start, 0);
		smp_wmb();
		atomic_inc(&count_count_stop);
	}
	/* Arrange for an interrupt in a short while */
	write_c0_compare(read_c0_count() + COUNTON);

	local_irq_restore(flags);

	/*
	 * i386 code reported the skew here, but the
	 * count registers were almost certainly out of sync
	 * so no point in alarming people
	 */
<<<<<<< HEAD
	printk("done.\n");
}

void __cpuinit synchronise_count_slave(void)
{
	int i;
	unsigned long flags;
	unsigned int initcount;
	int ncpus;

#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 * SMTC needs to synchronise per VPE, not per CPU
	 * ignore for now
	 */
	return;
#endif
=======
	pr_cont("done.\n");
}

void synchronise_count_slave(int cpu)
{
	int i;
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	local_irq_save(flags);

	/*
	 * Not every cpu is online at the time this gets called,
	 * so we first wait for the master to say everyone is ready
	 */

<<<<<<< HEAD
	while (!atomic_read(&count_start_flag))
		mb();

	/* Count will be initialised to next expire for all CPU's */
	initcount = atomic_read(&count_reference);

	ncpus = num_online_cpus();
	for (i = 0; i < NR_LOOPS; i++) {
		atomic_inc(&count_count_start);
		while (atomic_read(&count_count_start) != ncpus)
=======
	for (i = 0; i < NR_LOOPS; i++) {
		atomic_inc(&count_count_start);
		while (atomic_read(&count_count_start) != 2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mb();

		/*
		 * Everyone initialises count in the last loop:
		 */
		if (i == NR_LOOPS-1)
			write_c0_count(initcount);

		atomic_inc(&count_count_stop);
<<<<<<< HEAD
		while (atomic_read(&count_count_stop) != ncpus)
=======
		while (atomic_read(&count_count_stop) != 2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mb();
	}
	/* Arrange for an interrupt in a short while */
	write_c0_compare(read_c0_count() + COUNTON);

	local_irq_restore(flags);
}
#undef NR_LOOPS
