<<<<<<< HEAD
/*
 *   Machine check handler
 *
 *    Copyright IBM Corp. 2000,2009
 *    Author(s): Ingo Adlung <adlung@de.ibm.com>,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Cornelia Huck <cornelia.huck@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *   Machine check handler
 *
 *    Copyright IBM Corp. 2000, 2009
 *    Author(s): Ingo Adlung <adlung@de.ibm.com>,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Cornelia Huck <cornelia.huck@de.ibm.com>,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel_stat.h>
#include <linux/init.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/hardirq.h>
#include <linux/time.h>
#include <linux/module.h>
#include <asm/lowcore.h>
#include <asm/smp.h>
#include <asm/etr.h>
#include <asm/cputime.h>
#include <asm/nmi.h>
#include <asm/crw.h>

struct mcck_struct {
	int kill_task;
	int channel_report;
	int warning;
	unsigned long long mcck_code;
=======
#include <linux/entry-common.h>
#include <linux/hardirq.h>
#include <linux/log2.h>
#include <linux/kprobes.h>
#include <linux/kmemleak.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/kvm_host.h>
#include <linux/export.h>
#include <asm/lowcore.h>
#include <asm/ctlreg.h>
#include <asm/fpu.h>
#include <asm/smp.h>
#include <asm/stp.h>
#include <asm/cputime.h>
#include <asm/nmi.h>
#include <asm/crw.h>
#include <asm/asm-offsets.h>
#include <asm/pai.h>

struct mcck_struct {
	unsigned int kill_task : 1;
	unsigned int channel_report : 1;
	unsigned int warning : 1;
	unsigned int stp_queue : 1;
	unsigned long mcck_code;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static DEFINE_PER_CPU(struct mcck_struct, cpu_mcck);

<<<<<<< HEAD
static void s390_handle_damage(char *msg)
{
	smp_send_stop();
	disabled_wait((unsigned long) __builtin_return_address(0));
	while (1);
}

/*
 * Main machine check handler function. Will be called with interrupts enabled
 * or disabled and machine checks enabled or disabled.
 */
void s390_handle_mcck(void)
{
	unsigned long flags;
	struct mcck_struct mcck;
=======
static inline int nmi_needs_mcesa(void)
{
	return cpu_has_vx() || MACHINE_HAS_GS;
}

/*
 * The initial machine check extended save area for the boot CPU.
 * It will be replaced on the boot CPU reinit with an allocated
 * structure. The structure is required for machine check happening
 * early in the boot process.
 */
static struct mcesa boot_mcesa __aligned(MCESA_MAX_SIZE);

void __init nmi_alloc_mcesa_early(u64 *mcesad)
{
	if (!nmi_needs_mcesa())
		return;
	*mcesad = __pa(&boot_mcesa);
	if (MACHINE_HAS_GS)
		*mcesad |= ilog2(MCESA_MAX_SIZE);
}

int nmi_alloc_mcesa(u64 *mcesad)
{
	unsigned long size;
	void *origin;

	*mcesad = 0;
	if (!nmi_needs_mcesa())
		return 0;
	size = MACHINE_HAS_GS ? MCESA_MAX_SIZE : MCESA_MIN_SIZE;
	origin = kmalloc(size, GFP_KERNEL);
	if (!origin)
		return -ENOMEM;
	/* The pointer is stored with mcesa_bits ORed in */
	kmemleak_not_leak(origin);
	*mcesad = __pa(origin);
	if (MACHINE_HAS_GS)
		*mcesad |= ilog2(MCESA_MAX_SIZE);
	return 0;
}

void nmi_free_mcesa(u64 *mcesad)
{
	if (!nmi_needs_mcesa())
		return;
	kfree(__va(*mcesad & MCESA_ORIGIN_MASK));
}

static __always_inline char *nmi_puts(char *dest, const char *src)
{
	while (*src)
		*dest++ = *src++;
	*dest = 0;
	return dest;
}

static __always_inline char *u64_to_hex(char *dest, u64 val)
{
	int i, num;

	for (i = 1; i <= 16; i++) {
		num = (val >> (64 - 4 * i)) & 0xf;
		if (num >= 10)
			*dest++ = 'A' + num - 10;
		else
			*dest++ = '0' + num;
	}
	*dest = 0;
	return dest;
}

static notrace void s390_handle_damage(void)
{
	union ctlreg0 cr0, cr0_new;
	char message[100];
	psw_t psw_save;
	char *ptr;

	smp_emergency_stop();
	diag_amode31_ops.diag308_reset();
	ptr = nmi_puts(message, "System stopped due to unrecoverable machine check, code: 0x");
	u64_to_hex(ptr, S390_lowcore.mcck_interruption_code);

	/*
	 * Disable low address protection and make machine check new PSW a
	 * disabled wait PSW. Any additional machine check cannot be handled.
	 */
	local_ctl_store(0, &cr0.reg);
	cr0_new = cr0;
	cr0_new.lap = 0;
	local_ctl_load(0, &cr0_new.reg);
	psw_save = S390_lowcore.mcck_new_psw;
	psw_bits(S390_lowcore.mcck_new_psw).io = 0;
	psw_bits(S390_lowcore.mcck_new_psw).ext = 0;
	psw_bits(S390_lowcore.mcck_new_psw).wait = 1;
	sclp_emergency_printk(message);

	/*
	 * Restore machine check new PSW and control register 0 to original
	 * values. This makes possible system dump analysis easier.
	 */
	S390_lowcore.mcck_new_psw = psw_save;
	local_ctl_load(0, &cr0.reg);
	disabled_wait();
	while (1);
}
NOKPROBE_SYMBOL(s390_handle_damage);

/*
 * Main machine check handler function. Will be called with interrupts disabled
 * and machine checks enabled.
 */
void s390_handle_mcck(void)
{
	struct mcck_struct mcck;
	unsigned long mflags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Disable machine checks and get the current state of accumulated
	 * machine checks. Afterwards delete the old state and enable machine
	 * checks again.
	 */
<<<<<<< HEAD
	local_irq_save(flags);
	local_mcck_disable();
	mcck = __get_cpu_var(cpu_mcck);
	memset(&__get_cpu_var(cpu_mcck), 0, sizeof(struct mcck_struct));
	clear_thread_flag(TIF_MCCK_PENDING);
	local_mcck_enable();
	local_irq_restore(flags);
=======
	local_mcck_save(mflags);
	mcck = *this_cpu_ptr(&cpu_mcck);
	memset(this_cpu_ptr(&cpu_mcck), 0, sizeof(mcck));
	local_mcck_restore(mflags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mcck.channel_report)
		crw_handle_channel_report();
	/*
	 * A warning may remain for a prolonged period on the bare iron.
	 * (actually until the machine is powered off, or the problem is gone)
	 * So we just stop listening for the WARNING MCH and avoid continuously
	 * being interrupted.  One caveat is however, that we must do this per
	 * processor and cannot use the smp version of ctl_clear_bit().
	 * On VM we only get one interrupt per virtally presented machinecheck.
	 * Though one suffices, we may get one interrupt per (virtual) cpu.
	 */
	if (mcck.warning) {	/* WARNING pending ? */
		static int mchchk_wng_posted = 0;

		/* Use single cpu clear, as we cannot handle smp here. */
<<<<<<< HEAD
		__ctl_clear_bit(14, 24);	/* Disable WARNING MCH */
		if (xchg(&mchchk_wng_posted, 1) == 0)
			kill_cad_pid(SIGPWR, 1);
	}
	if (mcck.kill_task) {
		local_irq_enable();
		printk(KERN_EMERG "mcck: Terminating task because of machine "
		       "malfunction (code 0x%016llx).\n", mcck.mcck_code);
		printk(KERN_EMERG "mcck: task: %s, pid: %d.\n",
		       current->comm, current->pid);
		do_exit(SIGSEGV);
	}
}
EXPORT_SYMBOL_GPL(s390_handle_mcck);

/*
 * returns 0 if all registers could be validated
 * returns 1 otherwise
 */
static int notrace s390_revalidate_registers(struct mci *mci)
{
	int kill_task;
	u64 zero;
	void *fpt_save_area, *fpt_creg_save_area;

	kill_task = 0;
	zero = 0;

	if (!mci->gr) {
		/*
		 * General purpose registers couldn't be restored and have
		 * unknown contents. Process needs to be terminated.
		 */
		kill_task = 1;
	}
	if (!mci->fp) {
		/*
		 * Floating point registers can't be restored and
		 * therefore the process needs to be terminated.
		 */
		kill_task = 1;
	}
#ifndef CONFIG_64BIT
	asm volatile(
		"	ld	0,0(%0)\n"
		"	ld	2,8(%0)\n"
		"	ld	4,16(%0)\n"
		"	ld	6,24(%0)"
		: : "a" (&S390_lowcore.floating_pt_save_area));
#endif

	if (MACHINE_HAS_IEEE) {
#ifdef CONFIG_64BIT
		fpt_save_area = &S390_lowcore.floating_pt_save_area;
		fpt_creg_save_area = &S390_lowcore.fpt_creg_save_area;
#else
		fpt_save_area = (void *) S390_lowcore.extended_save_area_addr;
		fpt_creg_save_area = fpt_save_area + 128;
#endif
		if (!mci->fc) {
			/*
			 * Floating point control register can't be restored.
			 * Task will be terminated.
			 */
			asm volatile("lfpc 0(%0)" : : "a" (&zero), "m" (zero));
			kill_task = 1;

		} else
			asm volatile("lfpc 0(%0)" : : "a" (fpt_creg_save_area));

		asm volatile(
			"	ld	0,0(%0)\n"
			"	ld	1,8(%0)\n"
			"	ld	2,16(%0)\n"
			"	ld	3,24(%0)\n"
			"	ld	4,32(%0)\n"
			"	ld	5,40(%0)\n"
			"	ld	6,48(%0)\n"
			"	ld	7,56(%0)\n"
			"	ld	8,64(%0)\n"
			"	ld	9,72(%0)\n"
			"	ld	10,80(%0)\n"
			"	ld	11,88(%0)\n"
			"	ld	12,96(%0)\n"
			"	ld	13,104(%0)\n"
			"	ld	14,112(%0)\n"
			"	ld	15,120(%0)\n"
			: : "a" (fpt_save_area));
	}
	/* Revalidate access registers */
	asm volatile(
		"	lam	0,15,0(%0)"
		: : "a" (&S390_lowcore.access_regs_save_area));
	if (!mci->ar) {
		/*
		 * Access registers have unknown contents.
		 * Terminating task.
		 */
		kill_task = 1;
	}
	/* Revalidate control registers */
	if (!mci->cr) {
		/*
		 * Control registers have unknown contents.
		 * Can't recover and therefore stopping machine.
		 */
		s390_handle_damage("invalid control registers.");
	} else {
#ifdef CONFIG_64BIT
		asm volatile(
			"	lctlg	0,15,0(%0)"
			: : "a" (&S390_lowcore.cregs_save_area));
#else
		asm volatile(
			"	lctl	0,15,0(%0)"
			: : "a" (&S390_lowcore.cregs_save_area));
#endif
	}
	/*
	 * We don't even try to revalidate the TOD register, since we simply
	 * can't write something sensible into that register.
	 */
#ifdef CONFIG_64BIT
	/*
	 * See if we can revalidate the TOD programmable register with its
	 * old contents (should be zero) otherwise set it to zero.
	 */
	if (!mci->pr)
		asm volatile(
			"	sr	0,0\n"
			"	sckpf"
			: : : "0", "cc");
	else
		asm volatile(
			"	l	0,0(%0)\n"
			"	sckpf"
			: : "a" (&S390_lowcore.tod_progreg_save_area)
			: "0", "cc");
#endif
	/* Revalidate clock comparator register */
	if (S390_lowcore.clock_comparator == -1)
		set_clock_comparator(S390_lowcore.mcck_clock);
	else
		set_clock_comparator(S390_lowcore.clock_comparator);
	/* Check if old PSW is valid */
	if (!mci->wp)
		/*
		 * Can't tell if we come from user or kernel mode
		 * -> stopping machine.
		 */
		s390_handle_damage("old psw invalid.");

	if (!mci->ms || !mci->pm || !mci->ia)
		kill_task = 1;

	return kill_task;
}
=======
		local_ctl_clear_bit(14, CR14_WARNING_SUBMASK_BIT);
		if (xchg(&mchchk_wng_posted, 1) == 0)
			kill_cad_pid(SIGPWR, 1);
	}
	if (mcck.stp_queue)
		stp_queue_work();
	if (mcck.kill_task) {
		printk(KERN_EMERG "mcck: Terminating task because of machine "
		       "malfunction (code 0x%016lx).\n", mcck.mcck_code);
		printk(KERN_EMERG "mcck: task: %s, pid: %d.\n",
		       current->comm, current->pid);
		if (is_global_init(current))
			panic("mcck: Attempting to kill init!\n");
		do_send_sig_info(SIGKILL, SEND_SIG_PRIV, current, PIDTYPE_PID);
	}
}

/**
 * nmi_registers_valid - verify if registers are valid
 * @mci: machine check interruption code
 *
 * Inspect a machine check interruption code and verify if all required
 * registers are valid. For some registers the corresponding validity bit is
 * ignored and the registers are set to the expected value.
 * Returns true if all registers are valid, otherwise false.
 */
static bool notrace nmi_registers_valid(union mci mci)
{
	union ctlreg2 cr2;

	/*
	 * The getcpu vdso syscall reads the CPU number from the programmable
	 * field of the TOD clock. Disregard the TOD programmable register
	 * validity bit and load the CPU number into the TOD programmable field
	 * unconditionally.
	 */
	set_tod_programmable_field(raw_smp_processor_id());
	/*
	 * Set the clock comparator register to the next expected value.
	 */
	set_clock_comparator(S390_lowcore.clock_comparator);
	if (!mci.gr || !mci.fp || !mci.fc)
		return false;
	/*
	 * The vector validity must only be checked if not running a
	 * KVM guest. For KVM guests the machine check is forwarded by
	 * KVM and it is the responsibility of the guest to take
	 * appropriate actions. The host vector or FPU values have been
	 * saved by KVM and will be restored by KVM.
	 */
	if (!mci.vr && !test_cpu_flag(CIF_MCCK_GUEST))
		return false;
	if (!mci.ar)
		return false;
	/*
	 * Two cases for guarded storage registers:
	 * - machine check in kernel or userspace
	 * - machine check while running SIE (KVM guest)
	 * For kernel or userspace the userspace values of guarded storage
	 * control can not be recreated, the process must be terminated.
	 * For SIE the guest values of guarded storage can not be recreated.
	 * This is either due to a bug or due to GS being disabled in the
	 * guest. The guest will be notified by KVM code and the guests machine
	 * check handling must take care of this. The host values are saved by
	 * KVM and are not affected.
	 */
	cr2.reg = S390_lowcore.cregs_save_area[2];
	if (cr2.gse && !mci.gs && !test_cpu_flag(CIF_MCCK_GUEST))
		return false;
	if (!mci.ms || !mci.pm || !mci.ia)
		return false;
	return true;
}
NOKPROBE_SYMBOL(nmi_registers_valid);

/*
 * Backup the guest's machine check info to its description block
 */
static void notrace s390_backup_mcck_info(struct pt_regs *regs)
{
	struct mcck_volatile_info *mcck_backup;
	struct sie_page *sie_page;

	/* r14 contains the sie block, which was set in sie64a */
	struct kvm_s390_sie_block *sie_block = phys_to_virt(regs->gprs[14]);

	if (sie_block == NULL)
		/* Something's seriously wrong, stop system. */
		s390_handle_damage();

	sie_page = container_of(sie_block, struct sie_page, sie_block);
	mcck_backup = &sie_page->mcck_info;
	mcck_backup->mcic = S390_lowcore.mcck_interruption_code &
				~(MCCK_CODE_CP | MCCK_CODE_EXT_DAMAGE);
	mcck_backup->ext_damage_code = S390_lowcore.external_damage_code;
	mcck_backup->failing_storage_address
			= S390_lowcore.failing_storage_address;
}
NOKPROBE_SYMBOL(s390_backup_mcck_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MAX_IPD_COUNT	29
#define MAX_IPD_TIME	(5 * 60 * USEC_PER_SEC) /* 5 minutes */

#define ED_STP_ISLAND	6	/* External damage STP island check */
#define ED_STP_SYNC	7	/* External damage STP sync check */
<<<<<<< HEAD
#define ED_ETR_SYNC	12	/* External damage ETR sync check */
#define ED_ETR_SWITCH	13	/* External damage ETR switch to local */
=======

#define MCCK_CODE_NO_GUEST	(MCCK_CODE_CP | MCCK_CODE_EXT_DAMAGE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * machine check handler.
 */
void notrace s390_do_machine_check(struct pt_regs *regs)
{
	static int ipd_count;
	static DEFINE_SPINLOCK(ipd_lock);
	static unsigned long long last_ipd;
	struct mcck_struct *mcck;
	unsigned long long tmp;
<<<<<<< HEAD
	struct mci *mci;
	int umode;

	nmi_enter();
	kstat_cpu(smp_processor_id()).irqs[NMI_NMI]++;
	mci = (struct mci *) &S390_lowcore.mcck_interruption_code;
	mcck = &__get_cpu_var(cpu_mcck);
	umode = user_mode(regs);

	if (mci->sd) {
		/* System damage -> stopping machine */
		s390_handle_damage("received system damage machine check.");
	}
	if (mci->pd) {
		if (mci->b) {
			/* Processing backup -> verify if we can survive this */
			u64 z_mcic, o_mcic, t_mcic;
#ifdef CONFIG_64BIT
=======
	irqentry_state_t irq_state;
	union mci mci;
	unsigned long mcck_dam_code;
	int mcck_pending = 0;

	irq_state = irqentry_nmi_enter(regs);

	if (user_mode(regs))
		update_timer_mcck();
	inc_irq_stat(NMI_NMI);
	mci.val = S390_lowcore.mcck_interruption_code;
	mcck = this_cpu_ptr(&cpu_mcck);

	/*
	 * Reinject the instruction processing damages' machine checks
	 * including Delayed Access Exception into the guest
	 * instead of damaging the host if they happen in the guest.
	 */
	if (mci.pd && !test_cpu_flag(CIF_MCCK_GUEST)) {
		if (mci.b) {
			/* Processing backup -> verify if we can survive this */
			u64 z_mcic, o_mcic, t_mcic;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			z_mcic = (1ULL<<63 | 1ULL<<59 | 1ULL<<29);
			o_mcic = (1ULL<<43 | 1ULL<<42 | 1ULL<<41 | 1ULL<<40 |
				  1ULL<<36 | 1ULL<<35 | 1ULL<<34 | 1ULL<<32 |
				  1ULL<<30 | 1ULL<<21 | 1ULL<<20 | 1ULL<<17 |
				  1ULL<<16);
<<<<<<< HEAD
#else
			z_mcic = (1ULL<<63 | 1ULL<<59 | 1ULL<<57 | 1ULL<<50 |
				  1ULL<<29);
			o_mcic = (1ULL<<43 | 1ULL<<42 | 1ULL<<41 | 1ULL<<40 |
				  1ULL<<36 | 1ULL<<35 | 1ULL<<34 | 1ULL<<32 |
				  1ULL<<30 | 1ULL<<20 | 1ULL<<17 | 1ULL<<16);
#endif
			t_mcic = *(u64 *)mci;

			if (((t_mcic & z_mcic) != 0) ||
			    ((t_mcic & o_mcic) != o_mcic)) {
				s390_handle_damage("processing backup machine "
						   "check with damage.");
=======
			t_mcic = mci.val;

			if (((t_mcic & z_mcic) != 0) ||
			    ((t_mcic & o_mcic) != o_mcic)) {
				s390_handle_damage();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}

			/*
			 * Nullifying exigent condition, therefore we might
			 * retry this instruction.
			 */
			spin_lock(&ipd_lock);
<<<<<<< HEAD
			tmp = get_clock();
=======
			tmp = get_tod_clock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (((tmp - last_ipd) >> 12) < MAX_IPD_TIME)
				ipd_count++;
			else
				ipd_count = 1;
			last_ipd = tmp;
			if (ipd_count == MAX_IPD_COUNT)
<<<<<<< HEAD
				s390_handle_damage("too many ipd retries.");
			spin_unlock(&ipd_lock);
		} else {
			/* Processing damage -> stopping machine */
			s390_handle_damage("received instruction processing "
					   "damage machine check.");
		}
	}
	if (s390_revalidate_registers(mci)) {
		if (umode) {
			/*
			 * Couldn't restore all register contents while in
			 * user mode -> mark task for termination.
			 */
			mcck->kill_task = 1;
			mcck->mcck_code = *(unsigned long long *) mci;
			set_thread_flag(TIF_MCCK_PENDING);
		} else {
			/*
			 * Couldn't restore all register contents while in
			 * kernel mode -> stopping machine.
			 */
			s390_handle_damage("unable to revalidate registers.");
		}
	}
	if (mci->cd) {
		/* Timing facility damage */
		s390_handle_damage("TOD clock damaged");
	}
	if (mci->ed && mci->ec) {
		/* External damage */
		if (S390_lowcore.external_damage_code & (1U << ED_ETR_SYNC))
			etr_sync_check();
		if (S390_lowcore.external_damage_code & (1U << ED_ETR_SWITCH))
			etr_switch_to_local();
		if (S390_lowcore.external_damage_code & (1U << ED_STP_SYNC))
			stp_sync_check();
		if (S390_lowcore.external_damage_code & (1U << ED_STP_ISLAND))
			stp_island_check();
	}
	if (mci->se)
		/* Storage error uncorrected */
		s390_handle_damage("received storage error uncorrected "
				   "machine check.");
	if (mci->ke)
		/* Storage key-error uncorrected */
		s390_handle_damage("received storage key-error uncorrected "
				   "machine check.");
	if (mci->ds && mci->fa)
		/* Storage degradation */
		s390_handle_damage("received storage degradation machine "
				   "check.");
	if (mci->cp) {
		/* Channel report word pending */
		mcck->channel_report = 1;
		set_thread_flag(TIF_MCCK_PENDING);
	}
	if (mci->w) {
		/* Warning pending */
		mcck->warning = 1;
		set_thread_flag(TIF_MCCK_PENDING);
	}
	nmi_exit();
}

static int __init machine_check_init(void)
{
	ctl_set_bit(14, 25);	/* enable external damage MCH */
	ctl_set_bit(14, 27);	/* enable system recovery MCH */
	ctl_set_bit(14, 24);	/* enable warning MCH */
	return 0;
}
arch_initcall(machine_check_init);
=======
				s390_handle_damage();
			spin_unlock(&ipd_lock);
		} else {
			/* Processing damage -> stopping machine */
			s390_handle_damage();
		}
	}
	if (!nmi_registers_valid(mci)) {
		if (!user_mode(regs))
			s390_handle_damage();
		/*
		 * Couldn't restore all register contents for the
		 * user space process -> mark task for termination.
		 */
		mcck->kill_task = 1;
		mcck->mcck_code = mci.val;
		mcck_pending = 1;
	}

	/*
	 * Backup the machine check's info if it happens when the guest
	 * is running.
	 */
	if (test_cpu_flag(CIF_MCCK_GUEST))
		s390_backup_mcck_info(regs);

	if (mci.cd) {
		/* Timing facility damage */
		s390_handle_damage();
	}
	if (mci.ed && mci.ec) {
		/* External damage */
		if (S390_lowcore.external_damage_code & (1U << ED_STP_SYNC))
			mcck->stp_queue |= stp_sync_check();
		if (S390_lowcore.external_damage_code & (1U << ED_STP_ISLAND))
			mcck->stp_queue |= stp_island_check();
		mcck_pending = 1;
	}
	/*
	 * Reinject storage related machine checks into the guest if they
	 * happen when the guest is running.
	 */
	if (!test_cpu_flag(CIF_MCCK_GUEST)) {
		/* Storage error uncorrected */
		if (mci.se)
			s390_handle_damage();
		/* Storage key-error uncorrected */
		if (mci.ke)
			s390_handle_damage();
		/* Storage degradation */
		if (mci.ds && mci.fa)
			s390_handle_damage();
	}
	if (mci.cp) {
		/* Channel report word pending */
		mcck->channel_report = 1;
		mcck_pending = 1;
	}
	if (mci.w) {
		/* Warning pending */
		mcck->warning = 1;
		mcck_pending = 1;
	}

	/*
	 * If there are only Channel Report Pending and External Damage
	 * machine checks, they will not be reinjected into the guest
	 * because they refer to host conditions only.
	 */
	mcck_dam_code = (mci.val & MCIC_SUBCLASS_MASK);
	if (test_cpu_flag(CIF_MCCK_GUEST) &&
	(mcck_dam_code & MCCK_CODE_NO_GUEST) != mcck_dam_code) {
		/* Set exit reason code for host's later handling */
		*((long *)(regs->gprs[15] + __SF_SIE_REASON)) = -EINTR;
	}
	clear_cpu_flag(CIF_MCCK_GUEST);

	if (mcck_pending)
		schedule_mcck_handler();

	irqentry_nmi_exit(regs, irq_state);
}
NOKPROBE_SYMBOL(s390_do_machine_check);

static int __init machine_check_init(void)
{
	system_ctl_set_bit(14, CR14_EXTERNAL_DAMAGE_SUBMASK_BIT);
	system_ctl_set_bit(14, CR14_RECOVERY_SUBMASK_BIT);
	system_ctl_set_bit(14, CR14_WARNING_SUBMASK_BIT);
	return 0;
}
early_initcall(machine_check_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
