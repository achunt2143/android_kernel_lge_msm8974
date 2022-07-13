/*
 * arch/xtensa/kernel/traps.c
 *
 * Exception handling.
 *
 * Derived from code with the following copyrights:
 * Copyright (C) 1994 - 1999 by Ralf Baechle
 * Modified for R3000 by Paul M. Antoine, 1995, 1996
 * Complete output from die() by Ulf Carlsson, 1998
 * Copyright (C) 1999 Silicon Graphics, Inc.
 *
 * Essentially rewritten for the Xtensa architecture port.
 *
<<<<<<< HEAD
 * Copyright (C) 2001 - 2005 Tensilica Inc.
=======
 * Copyright (C) 2001 - 2013 Tensilica Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Joe Taylor	<joe@tensilica.com, joetylr@yahoo.com>
 * Chris Zankel	<chris@zankel.net>
 * Marc Gauthier<marc@tensilica.com, marc@alumni.uwaterloo.ca>
 * Kevin Chea
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/sched.h>
=======
#include <linux/cpu.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/stringify.h>
#include <linux/kallsyms.h>
#include <linux/delay.h>
#include <linux/hardirq.h>
<<<<<<< HEAD

#include <asm/ptrace.h>
#include <asm/timex.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <asm/processor.h>

#ifdef CONFIG_KGDB
extern int gdb_enter;
extern int return_from_debug_flag;
#endif
=======
#include <linux/ratelimit.h>
#include <linux/pgtable.h>

#include <asm/stacktrace.h>
#include <asm/ptrace.h>
#include <asm/timex.h>
#include <linux/uaccess.h>
#include <asm/processor.h>
#include <asm/traps.h>
#include <asm/hw_breakpoint.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Machine specific interrupt handlers
 */

<<<<<<< HEAD
extern void kernel_exception(void);
extern void user_exception(void);

extern void fast_syscall_kernel(void);
extern void fast_syscall_user(void);
extern void fast_alloca(void);
extern void fast_unaligned(void);
extern void fast_second_level_miss(void);
extern void fast_store_prohibited(void);
extern void fast_coprocessor(void);

extern void do_illegal_instruction (struct pt_regs*);
extern void do_interrupt (struct pt_regs*);
extern void do_unaligned_user (struct pt_regs*);
extern void do_multihit (struct pt_regs*, unsigned long);
extern void do_page_fault (struct pt_regs*, unsigned long);
extern void do_debug (struct pt_regs*);
extern void system_call (struct pt_regs*);
=======
static void do_illegal_instruction(struct pt_regs *regs);
static void do_div0(struct pt_regs *regs);
static void do_interrupt(struct pt_regs *regs);
#if XTENSA_FAKE_NMI
static void do_nmi(struct pt_regs *regs);
#endif
#ifdef CONFIG_XTENSA_LOAD_STORE
static void do_load_store(struct pt_regs *regs);
#endif
static void do_unaligned_user(struct pt_regs *regs);
static void do_multihit(struct pt_regs *regs);
#if XTENSA_HAVE_COPROCESSORS
static void do_coprocessor(struct pt_regs *regs);
#endif
static void do_debug(struct pt_regs *regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The vector table must be preceded by a save area (which
 * implies it must be in RAM, unless one places RAM immediately
 * before a ROM and puts the vector at the start of the ROM (!))
 */

#define KRNL		0x01
#define USER		0x02

#define COPROCESSOR(x)							\
<<<<<<< HEAD
{ EXCCAUSE_COPROCESSOR ## x ## _DISABLED, USER, fast_coprocessor }
=======
{ EXCCAUSE_COPROCESSOR ## x ## _DISABLED, USER|KRNL, fast_coprocessor },\
{ EXCCAUSE_COPROCESSOR ## x ## _DISABLED, 0, do_coprocessor }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

typedef struct {
	int cause;
	int fast;
	void* handler;
} dispatch_init_table_t;

static dispatch_init_table_t __initdata dispatch_init_table[] = {

<<<<<<< HEAD
{ EXCCAUSE_ILLEGAL_INSTRUCTION,	0,	   do_illegal_instruction},
{ EXCCAUSE_SYSTEM_CALL,		KRNL,	   fast_syscall_kernel },
{ EXCCAUSE_SYSTEM_CALL,		USER,	   fast_syscall_user },
{ EXCCAUSE_SYSTEM_CALL,		0,	   system_call },
/* EXCCAUSE_INSTRUCTION_FETCH unhandled */
/* EXCCAUSE_LOAD_STORE_ERROR unhandled*/
{ EXCCAUSE_LEVEL1_INTERRUPT,	0,	   do_interrupt },
{ EXCCAUSE_ALLOCA,		USER|KRNL, fast_alloca },
/* EXCCAUSE_INTEGER_DIVIDE_BY_ZERO unhandled */
/* EXCCAUSE_PRIVILEGED unhandled */
#if XCHAL_UNALIGNED_LOAD_EXCEPTION || XCHAL_UNALIGNED_STORE_EXCEPTION
#ifdef CONFIG_UNALIGNED_USER
{ EXCCAUSE_UNALIGNED,		USER,	   fast_unaligned },
#else
{ EXCCAUSE_UNALIGNED,		0,	   do_unaligned_user },
#endif
{ EXCCAUSE_UNALIGNED,		KRNL,	   fast_unaligned },
#endif
#ifdef CONFIG_MMU
{ EXCCAUSE_ITLB_MISS,		0,	   do_page_fault },
{ EXCCAUSE_ITLB_MISS,		USER|KRNL, fast_second_level_miss},
{ EXCCAUSE_ITLB_MULTIHIT,		0,	   do_multihit },
{ EXCCAUSE_ITLB_PRIVILEGE,	0,	   do_page_fault },
/* EXCCAUSE_SIZE_RESTRICTION unhandled */
{ EXCCAUSE_FETCH_CACHE_ATTRIBUTE,	0,	   do_page_fault },
{ EXCCAUSE_DTLB_MISS,		USER|KRNL, fast_second_level_miss},
{ EXCCAUSE_DTLB_MISS,		0,	   do_page_fault },
{ EXCCAUSE_DTLB_MULTIHIT,		0,	   do_multihit },
{ EXCCAUSE_DTLB_PRIVILEGE,	0,	   do_page_fault },
/* EXCCAUSE_DTLB_SIZE_RESTRICTION unhandled */
{ EXCCAUSE_STORE_CACHE_ATTRIBUTE,	USER|KRNL, fast_store_prohibited },
{ EXCCAUSE_STORE_CACHE_ATTRIBUTE,	0,	   do_page_fault },
{ EXCCAUSE_LOAD_CACHE_ATTRIBUTE,	0,	   do_page_fault },
#endif /* CONFIG_MMU */
=======
#ifdef CONFIG_USER_ABI_CALL0_PROBE
{ EXCCAUSE_ILLEGAL_INSTRUCTION,	USER,	   fast_illegal_instruction_user },
#endif
{ EXCCAUSE_ILLEGAL_INSTRUCTION,	0,	   do_illegal_instruction},
{ EXCCAUSE_SYSTEM_CALL,		USER,	   fast_syscall_user },
{ EXCCAUSE_SYSTEM_CALL,		0,	   system_call },
/* EXCCAUSE_INSTRUCTION_FETCH unhandled */
#ifdef CONFIG_XTENSA_LOAD_STORE
{ EXCCAUSE_LOAD_STORE_ERROR,	USER|KRNL, fast_load_store },
{ EXCCAUSE_LOAD_STORE_ERROR,	0,	   do_load_store },
#endif
{ EXCCAUSE_LEVEL1_INTERRUPT,	0,	   do_interrupt },
#ifdef SUPPORT_WINDOWED
{ EXCCAUSE_ALLOCA,		USER|KRNL, fast_alloca },
#endif
{ EXCCAUSE_INTEGER_DIVIDE_BY_ZERO, 0,	   do_div0 },
/* EXCCAUSE_PRIVILEGED unhandled */
#if XCHAL_UNALIGNED_LOAD_EXCEPTION || XCHAL_UNALIGNED_STORE_EXCEPTION || \
		IS_ENABLED(CONFIG_XTENSA_LOAD_STORE)
#ifdef CONFIG_XTENSA_UNALIGNED_USER
{ EXCCAUSE_UNALIGNED,		USER,	   fast_unaligned },
#endif
{ EXCCAUSE_UNALIGNED,		KRNL,	   fast_unaligned },
#endif
{ EXCCAUSE_UNALIGNED,		0,	   do_unaligned_user },
#ifdef CONFIG_MMU
{ EXCCAUSE_ITLB_MISS,			0,	   do_page_fault },
{ EXCCAUSE_ITLB_MISS,			USER|KRNL, fast_second_level_miss},
{ EXCCAUSE_DTLB_MISS,			USER|KRNL, fast_second_level_miss},
{ EXCCAUSE_DTLB_MISS,			0,	   do_page_fault },
{ EXCCAUSE_STORE_CACHE_ATTRIBUTE,	USER|KRNL, fast_store_prohibited },
#endif /* CONFIG_MMU */
#ifdef CONFIG_PFAULT
{ EXCCAUSE_ITLB_MULTIHIT,		0,	   do_multihit },
{ EXCCAUSE_ITLB_PRIVILEGE,		0,	   do_page_fault },
{ EXCCAUSE_FETCH_CACHE_ATTRIBUTE,	0,	   do_page_fault },
{ EXCCAUSE_DTLB_MULTIHIT,		0,	   do_multihit },
{ EXCCAUSE_DTLB_PRIVILEGE,		0,	   do_page_fault },
{ EXCCAUSE_STORE_CACHE_ATTRIBUTE,	0,	   do_page_fault },
{ EXCCAUSE_LOAD_CACHE_ATTRIBUTE,	0,	   do_page_fault },
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* XCCHAL_EXCCAUSE_FLOATING_POINT unhandled */
#if XTENSA_HAVE_COPROCESSOR(0)
COPROCESSOR(0),
#endif
#if XTENSA_HAVE_COPROCESSOR(1)
COPROCESSOR(1),
#endif
#if XTENSA_HAVE_COPROCESSOR(2)
COPROCESSOR(2),
#endif
#if XTENSA_HAVE_COPROCESSOR(3)
COPROCESSOR(3),
#endif
#if XTENSA_HAVE_COPROCESSOR(4)
COPROCESSOR(4),
#endif
#if XTENSA_HAVE_COPROCESSOR(5)
COPROCESSOR(5),
#endif
#if XTENSA_HAVE_COPROCESSOR(6)
COPROCESSOR(6),
#endif
#if XTENSA_HAVE_COPROCESSOR(7)
COPROCESSOR(7),
#endif
<<<<<<< HEAD
=======
#if XTENSA_FAKE_NMI
{ EXCCAUSE_MAPPED_NMI,			0,		do_nmi },
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{ EXCCAUSE_MAPPED_DEBUG,		0,		do_debug },
{ -1, -1, 0 }

};

/* The exception table <exc_table> serves two functions:
 * 1. it contains three dispatch tables (fast_user, fast_kernel, default-c)
 * 2. it is a temporary memory buffer for the exception handlers.
 */

<<<<<<< HEAD
unsigned long exc_table[EXC_TABLE_SIZE/4];
=======
DEFINE_PER_CPU(struct exc_table, exc_table);
DEFINE_PER_CPU(struct debug_table, debug_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void die(const char*, struct pt_regs*, long);

static inline void
__die_if_kernel(const char *str, struct pt_regs *regs, long err)
{
	if (!user_mode(regs))
		die(str, regs, err);
}

<<<<<<< HEAD
=======
#ifdef CONFIG_PRINT_USER_CODE_ON_UNHANDLED_EXCEPTION
static inline void dump_user_code(struct pt_regs *regs)
{
	char buf[32];

	if (copy_from_user(buf, (void __user *)(regs->pc & -16), sizeof(buf)) == 0) {
		print_hex_dump(KERN_INFO, " ", DUMP_PREFIX_NONE,
			       32, 1, buf, sizeof(buf), false);

	}
}
#else
static inline void dump_user_code(struct pt_regs *regs)
{
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Unhandled Exceptions. Kill user task or panic if in kernel space.
 */

<<<<<<< HEAD
void do_unhandled(struct pt_regs *regs, unsigned long exccause)
{
	__die_if_kernel("Caught unhandled exception - should not happen",
	    		regs, SIGKILL);

	/* If in user mode, send SIGILL signal to current process */
	printk("Caught unhandled exception in '%s' "
	       "(pid = %d, pc = %#010lx) - should not happen\n"
	       "\tEXCCAUSE is %ld\n",
	       current->comm, task_pid_nr(current), regs->pc, exccause);
	force_sig(SIGILL, current);
=======
void do_unhandled(struct pt_regs *regs)
{
	__die_if_kernel("Caught unhandled exception - should not happen",
			regs, SIGKILL);

	/* If in user mode, send SIGILL signal to current process */
	pr_info_ratelimited("Caught unhandled exception in '%s' "
			    "(pid = %d, pc = %#010lx) - should not happen\n"
			    "\tEXCCAUSE is %ld\n",
			    current->comm, task_pid_nr(current), regs->pc,
			    regs->exccause);
	dump_user_code(regs);
	force_sig(SIGILL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Multi-hit exception. This if fatal!
 */

<<<<<<< HEAD
void do_multihit(struct pt_regs *regs, unsigned long exccause)
=======
static void do_multihit(struct pt_regs *regs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	die("Caught multihit exception", regs, SIGKILL);
}

/*
<<<<<<< HEAD
 * Level-1 interrupt.
 * We currently have no priority encoding.
 */

unsigned long ignored_level1_interrupts;
extern void do_IRQ(int, struct pt_regs *);

void do_interrupt (struct pt_regs *regs)
{
	unsigned long intread = get_sr (INTREAD);
	unsigned long intenable = get_sr (INTENABLE);
	int i, mask;

	/* Handle all interrupts (no priorities).
	 * (Clear the interrupt before processing, in case it's
	 *  edge-triggered or software-generated)
	 */

	for (i=0, mask = 1; i < XCHAL_NUM_INTERRUPTS; i++, mask <<= 1) {
		if (mask & (intread & intenable)) {
			set_sr (mask, INTCLEAR);
			do_IRQ (i,regs);
		}
	}
=======
 * IRQ handler.
 */

#if XTENSA_FAKE_NMI

#define IS_POW2(v) (((v) & ((v) - 1)) == 0)

#if !(PROFILING_INTLEVEL == XCHAL_EXCM_LEVEL && \
      IS_POW2(XTENSA_INTLEVEL_MASK(PROFILING_INTLEVEL)))
#warning "Fake NMI is requested for PMM, but there are other IRQs at or above its level."
#warning "Fake NMI will be used, but there will be a bugcheck if one of those IRQs fire."

static inline void check_valid_nmi(void)
{
	unsigned intread = xtensa_get_sr(interrupt);
	unsigned intenable = xtensa_get_sr(intenable);

	BUG_ON(intread & intenable &
	       ~(XTENSA_INTLEVEL_ANDBELOW_MASK(PROFILING_INTLEVEL) ^
		 XTENSA_INTLEVEL_MASK(PROFILING_INTLEVEL) ^
		 BIT(XCHAL_PROFILING_INTERRUPT)));
}

#else

static inline void check_valid_nmi(void)
{
}

#endif

irqreturn_t xtensa_pmu_irq_handler(int irq, void *dev_id);

DEFINE_PER_CPU(unsigned long, nmi_count);

static void do_nmi(struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);

	nmi_enter();
	++*this_cpu_ptr(&nmi_count);
	check_valid_nmi();
	xtensa_pmu_irq_handler(0, NULL);
	nmi_exit();
	set_irq_regs(old_regs);
}
#endif

static void do_interrupt(struct pt_regs *regs)
{
	static const unsigned int_level_mask[] = {
		0,
		XCHAL_INTLEVEL1_MASK,
		XCHAL_INTLEVEL2_MASK,
		XCHAL_INTLEVEL3_MASK,
		XCHAL_INTLEVEL4_MASK,
		XCHAL_INTLEVEL5_MASK,
		XCHAL_INTLEVEL6_MASK,
		XCHAL_INTLEVEL7_MASK,
	};
	struct pt_regs *old_regs = set_irq_regs(regs);
	unsigned unhandled = ~0u;

	irq_enter();

	for (;;) {
		unsigned intread = xtensa_get_sr(interrupt);
		unsigned intenable = xtensa_get_sr(intenable);
		unsigned int_at_level = intread & intenable;
		unsigned level;

		for (level = LOCKLEVEL; level > 0; --level) {
			if (int_at_level & int_level_mask[level]) {
				int_at_level &= int_level_mask[level];
				if (int_at_level & unhandled)
					int_at_level &= unhandled;
				else
					unhandled |= int_level_mask[level];
				break;
			}
		}

		if (level == 0)
			break;

		/* clear lowest pending irq in the unhandled mask */
		unhandled ^= (int_at_level & -int_at_level);
		do_IRQ(__ffs(int_at_level), regs);
	}

	irq_exit();
	set_irq_regs(old_regs);
}

static bool check_div0(struct pt_regs *regs)
{
	static const u8 pattern[] = {'D', 'I', 'V', '0'};
	const u8 *p;
	u8 buf[5];

	if (user_mode(regs)) {
		if (copy_from_user(buf, (void __user *)regs->pc + 2, 5))
			return false;
		p = buf;
	} else {
		p = (const u8 *)regs->pc + 2;
	}

	return memcmp(p, pattern, sizeof(pattern)) == 0 ||
		memcmp(p + 1, pattern, sizeof(pattern)) == 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Illegal instruction. Fatal if in kernel space.
 */

<<<<<<< HEAD
void
do_illegal_instruction(struct pt_regs *regs)
{
=======
static void do_illegal_instruction(struct pt_regs *regs)
{
#ifdef CONFIG_USER_ABI_CALL0_PROBE
	/*
	 * When call0 application encounters an illegal instruction fast
	 * exception handler will attempt to set PS.WOE and retry failing
	 * instruction.
	 * If we get here we know that that instruction is also illegal
	 * with PS.WOE set, so it's not related to the windowed option
	 * hence PS.WOE may be cleared.
	 */
	if (regs->pc == current_thread_info()->ps_woe_fix_addr)
		regs->ps &= ~PS_WOE_MASK;
#endif
	if (check_div0(regs)) {
		do_div0(regs);
		return;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__die_if_kernel("Illegal instruction in kernel", regs, SIGKILL);

	/* If in user mode, send SIGILL signal to current process. */

<<<<<<< HEAD
	printk("Illegal Instruction in '%s' (pid = %d, pc = %#010lx)\n",
	    current->comm, task_pid_nr(current), regs->pc);
	force_sig(SIGILL, current);
}

=======
	pr_info_ratelimited("Illegal Instruction in '%s' (pid = %d, pc = %#010lx)\n",
			    current->comm, task_pid_nr(current), regs->pc);
	force_sig(SIGILL);
}

static void do_div0(struct pt_regs *regs)
{
	__die_if_kernel("Unhandled division by 0 in kernel", regs, SIGKILL);
	force_sig_fault(SIGFPE, FPE_INTDIV, (void __user *)regs->pc);
}

#ifdef CONFIG_XTENSA_LOAD_STORE
static void do_load_store(struct pt_regs *regs)
{
	__die_if_kernel("Unhandled load/store exception in kernel",
			regs, SIGKILL);

	pr_info_ratelimited("Load/store error to %08lx in '%s' (pid = %d, pc = %#010lx)\n",
			    regs->excvaddr, current->comm,
			    task_pid_nr(current), regs->pc);
	force_sig_fault(SIGBUS, BUS_ADRERR, (void *)regs->excvaddr);
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Handle unaligned memory accesses from user space. Kill task.
 *
 * If CONFIG_UNALIGNED_USER is not set, we don't allow unaligned memory
 * accesses causes from user space.
 */

<<<<<<< HEAD
#if XCHAL_UNALIGNED_LOAD_EXCEPTION || XCHAL_UNALIGNED_STORE_EXCEPTION
#ifndef CONFIG_UNALIGNED_USER
void
do_unaligned_user (struct pt_regs *regs)
{
	siginfo_t info;

	__die_if_kernel("Unhandled unaligned exception in kernel",
	    		regs, SIGKILL);

	current->thread.bad_vaddr = regs->excvaddr;
	current->thread.error_code = -3;
	printk("Unaligned memory access to %08lx in '%s' "
	       "(pid = %d, pc = %#010lx)\n",
	       regs->excvaddr, current->comm, task_pid_nr(current), regs->pc);
	info.si_signo = SIGBUS;
	info.si_errno = 0;
	info.si_code = BUS_ADRALN;
	info.si_addr = (void *) regs->excvaddr;
	force_sig_info(SIGSEGV, &info, current);

}
#endif
#endif

void
do_debug(struct pt_regs *regs)
{
#ifdef CONFIG_KGDB
	/* If remote debugging is configured AND enabled, we give control to
	 * kgdb.  Otherwise, we fall through, perhaps giving control to the
	 * native debugger.
	 */

	if (gdb_enter) {
		extern void gdb_handle_exception(struct pt_regs *);
		gdb_handle_exception(regs);
		return_from_debug_flag = 1;
		return;
	}
#endif

=======
static void do_unaligned_user(struct pt_regs *regs)
{
	__die_if_kernel("Unhandled unaligned exception in kernel",
			regs, SIGKILL);

	pr_info_ratelimited("Unaligned memory access to %08lx in '%s' "
			    "(pid = %d, pc = %#010lx)\n",
			    regs->excvaddr, current->comm,
			    task_pid_nr(current), regs->pc);
	force_sig_fault(SIGBUS, BUS_ADRALN, (void *) regs->excvaddr);
}

#if XTENSA_HAVE_COPROCESSORS
static void do_coprocessor(struct pt_regs *regs)
{
	coprocessor_flush_release_all(current_thread_info());
}
#endif

/* Handle debug events.
 * When CONFIG_HAVE_HW_BREAKPOINT is on this handler is called with
 * preemption disabled to avoid rescheduling and keep mapping of hardware
 * breakpoint structures to debug registers intact, so that
 * DEBUGCAUSE.DBNUM could be used in case of data breakpoint hit.
 */
static void do_debug(struct pt_regs *regs)
{
#ifdef CONFIG_HAVE_HW_BREAKPOINT
	int ret = check_hw_breakpoint(regs);

	preempt_enable();
	if (ret == 0)
		return;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__die_if_kernel("Breakpoint in kernel", regs, SIGKILL);

	/* If in user mode, send SIGTRAP signal to current process */

<<<<<<< HEAD
	force_sig(SIGTRAP, current);
}


=======
	force_sig(SIGTRAP);
}


#define set_handler(type, cause, handler)				\
	do {								\
		unsigned int cpu;					\
									\
		for_each_possible_cpu(cpu)				\
			per_cpu(exc_table, cpu).type[cause] = (handler);\
	} while (0)

/* Set exception C handler - for temporary use when probing exceptions */

xtensa_exception_handler *
__init trap_set_handler(int cause, xtensa_exception_handler *handler)
{
	void *previous = per_cpu(exc_table, 0).default_handler[cause];

	set_handler(default_handler, cause, handler);
	return previous;
}


static void trap_init_excsave(void)
{
	xtensa_set_sr(this_cpu_ptr(&exc_table), excsave1);
}

static void trap_init_debug(void)
{
	unsigned long debugsave = (unsigned long)this_cpu_ptr(&debug_table);

	this_cpu_ptr(&debug_table)->debug_exception = debug_exception;
	__asm__ __volatile__("wsr %0, excsave" __stringify(XCHAL_DEBUGLEVEL)
			     :: "a"(debugsave));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Initialize dispatch tables.
 *
 * The exception vectors are stored compressed the __init section in the
 * dispatch_init_table. This function initializes the following three tables
 * from that compressed table:
 * - fast user		first dispatch table for user exceptions
 * - fast kernel	first dispatch table for kernel exceptions
 * - default C-handler	C-handler called by the default fast handler.
 *
 * See vectors.S for more details.
 */

<<<<<<< HEAD
#define set_handler(idx,handler) (exc_table[idx] = (unsigned long) (handler))

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void __init trap_init(void)
{
	int i;

	/* Setup default vectors. */

<<<<<<< HEAD
	for(i = 0; i < 64; i++) {
		set_handler(EXC_TABLE_FAST_USER/4   + i, user_exception);
		set_handler(EXC_TABLE_FAST_KERNEL/4 + i, kernel_exception);
		set_handler(EXC_TABLE_DEFAULT/4 + i, do_unhandled);
=======
	for (i = 0; i < EXCCAUSE_N; i++) {
		set_handler(fast_user_handler, i, user_exception);
		set_handler(fast_kernel_handler, i, kernel_exception);
		set_handler(default_handler, i, do_unhandled);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Setup specific handlers. */

	for(i = 0; dispatch_init_table[i].cause >= 0; i++) {
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int fast = dispatch_init_table[i].fast;
		int cause = dispatch_init_table[i].cause;
		void *handler = dispatch_init_table[i].handler;

		if (fast == 0)
<<<<<<< HEAD
			set_handler (EXC_TABLE_DEFAULT/4 + cause, handler);
		if (fast && fast & USER)
			set_handler (EXC_TABLE_FAST_USER/4 + cause, handler);
		if (fast && fast & KRNL)
			set_handler (EXC_TABLE_FAST_KERNEL/4 + cause, handler);
	}

	/* Initialize EXCSAVE_1 to hold the address of the exception table. */

	i = (unsigned long)exc_table;
	__asm__ __volatile__("wsr  %0, "__stringify(EXCSAVE_1)"\n" : : "a" (i));
}

=======
			set_handler(default_handler, cause, handler);
		if ((fast & USER) != 0)
			set_handler(fast_user_handler, cause, handler);
		if ((fast & KRNL) != 0)
			set_handler(fast_kernel_handler, cause, handler);
	}

	/* Initialize EXCSAVE_1 to hold the address of the exception table. */
	trap_init_excsave();
	trap_init_debug();
}

#ifdef CONFIG_SMP
void secondary_trap_init(void)
{
	trap_init_excsave();
	trap_init_debug();
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This function dumps the current valid window frame and other base registers.
 */

void show_regs(struct pt_regs * regs)
{
<<<<<<< HEAD
	int i, wmask;

	wmask = regs->wmask & ~1;

	for (i = 0; i < 16; i++) {
		if ((i % 8) == 0)
			printk(KERN_INFO "a%02d:", i);
		printk(KERN_CONT " %08lx", regs->areg[i]);
	}
	printk(KERN_CONT "\n");

	printk("pc: %08lx, ps: %08lx, depc: %08lx, excvaddr: %08lx\n",
	       regs->pc, regs->ps, regs->depc, regs->excvaddr);
	printk("lbeg: %08lx, lend: %08lx lcount: %08lx, sar: %08lx\n",
	       regs->lbeg, regs->lend, regs->lcount, regs->sar);
	if (user_mode(regs))
		printk("wb: %08lx, ws: %08lx, wmask: %08lx, syscall: %ld\n",
		       regs->windowbase, regs->windowstart, regs->wmask,
		       regs->syscall);
}

static __always_inline unsigned long *stack_pointer(struct task_struct *task)
{
	unsigned long *sp;

	if (!task || task == current)
		__asm__ __volatile__ ("mov %0, a1\n" : "=a"(sp));
	else
		sp = (unsigned long *)task->thread.sp;

	return sp;
}

static inline void spill_registers(void)
{
	unsigned int a0, ps;

	__asm__ __volatile__ (
		"movi	a14," __stringify (PS_EXCM_BIT) " | 1\n\t"
		"mov	a12, a0\n\t"
		"rsr	a13," __stringify(SAR) "\n\t"
		"xsr	a14," __stringify(PS) "\n\t"
		"movi	a0, _spill_registers\n\t"
		"rsync\n\t"
		"callx0 a0\n\t"
		"mov	a0, a12\n\t"
		"wsr	a13," __stringify(SAR) "\n\t"
		"wsr	a14," __stringify(PS) "\n\t"
		:: "a" (&a0), "a" (&ps)
		: "a2", "a3", "a4", "a7", "a11", "a12", "a13", "a14", "a15", "memory");
}

void show_trace(struct task_struct *task, unsigned long *sp)
{
	unsigned long a0, a1, pc;
	unsigned long sp_start, sp_end;

	if (sp)
		a1 = (unsigned long)sp;
	else
		a1 = (unsigned long)stack_pointer(task);

	sp_start = a1 & ~(THREAD_SIZE-1);
	sp_end = sp_start + THREAD_SIZE;

	printk("Call Trace:");
#ifdef CONFIG_KALLSYMS
	printk("\n");
#endif
	spill_registers();

	while (a1 > sp_start && a1 < sp_end) {
		sp = (unsigned long*)a1;

		a0 = *(sp - 4);
		a1 = *(sp - 3);

		if (a1 <= (unsigned long) sp)
			break;

		pc = MAKE_PC_FROM_RA(a0, a1);

		if (kernel_text_address(pc)) {
			printk(" [<%08lx>] ", pc);
			print_symbol("%s\n", pc);
		}
	}
	printk("\n");
}

/*
 * This routine abuses get_user()/put_user() to reference pointers
 * with at least a bit of error checking ...
 */

static int kstack_depth_to_print = 24;

void show_stack(struct task_struct *task, unsigned long *sp)
{
	int i = 0;
	unsigned long *stack;

	if (!sp)
		sp = stack_pointer(task);
 	stack = sp;

	printk("\nStack: ");

	for (i = 0; i < kstack_depth_to_print; i++) {
		if (kstack_end(sp))
			break;
		if (i && ((i % 8) == 0))
			printk("\n       ");
		printk("%08lx ", *sp++);
	}
	printk("\n");
	show_trace(task, stack);
}

void dump_stack(void)
{
	show_stack(current, NULL);
}

EXPORT_SYMBOL(dump_stack);


void show_code(unsigned int *pc)
{
	long i;

	printk("\nCode:");

	for(i = -3 ; i < 6 ; i++) {
		unsigned long insn;
		if (__get_user(insn, pc + i)) {
			printk(" (Bad address in pc)\n");
			break;
		}
		printk("%c%08lx%c",(i?' ':'<'),insn,(i?' ':'>'));
	}
=======
	int i;

	show_regs_print_info(KERN_DEFAULT);

	for (i = 0; i < 16; i++) {
		if ((i % 8) == 0)
			pr_info("a%02d:", i);
		pr_cont(" %08lx", regs->areg[i]);
	}
	pr_cont("\n");
	pr_info("pc: %08lx, ps: %08lx, depc: %08lx, excvaddr: %08lx\n",
		regs->pc, regs->ps, regs->depc, regs->excvaddr);
	pr_info("lbeg: %08lx, lend: %08lx lcount: %08lx, sar: %08lx\n",
		regs->lbeg, regs->lend, regs->lcount, regs->sar);
	if (user_mode(regs))
		pr_cont("wb: %08lx, ws: %08lx, wmask: %08lx, syscall: %ld\n",
			regs->windowbase, regs->windowstart, regs->wmask,
			regs->syscall);
}

static int show_trace_cb(struct stackframe *frame, void *data)
{
	const char *loglvl = data;

	if (kernel_text_address(frame->pc))
		printk("%s [<%08lx>] %pB\n",
			loglvl, frame->pc, (void *)frame->pc);
	return 0;
}

static void show_trace(struct task_struct *task, unsigned long *sp,
		       const char *loglvl)
{
	if (!sp)
		sp = stack_pointer(task);

	printk("%sCall Trace:\n", loglvl);
	walk_stackframe(sp, show_trace_cb, (void *)loglvl);
}

#define STACK_DUMP_ENTRY_SIZE 4
#define STACK_DUMP_LINE_SIZE 16
static size_t kstack_depth_to_print = CONFIG_PRINT_STACK_DEPTH;

struct stack_fragment
{
	size_t len;
	size_t off;
	u8 *sp;
	const char *loglvl;
};

static int show_stack_fragment_cb(struct stackframe *frame, void *data)
{
	struct stack_fragment *sf = data;

	while (sf->off < sf->len) {
		u8 line[STACK_DUMP_LINE_SIZE];
		size_t line_len = sf->len - sf->off > STACK_DUMP_LINE_SIZE ?
			STACK_DUMP_LINE_SIZE : sf->len - sf->off;
		bool arrow = sf->off == 0;

		if (frame && frame->sp == (unsigned long)(sf->sp + sf->off))
			arrow = true;

		__memcpy(line, sf->sp + sf->off, line_len);
		print_hex_dump(sf->loglvl, arrow ? "> " : "  ", DUMP_PREFIX_NONE,
			       STACK_DUMP_LINE_SIZE, STACK_DUMP_ENTRY_SIZE,
			       line, line_len, false);
		sf->off += STACK_DUMP_LINE_SIZE;
		if (arrow)
			return 0;
	}
	return 1;
}

void show_stack(struct task_struct *task, unsigned long *sp, const char *loglvl)
{
	struct stack_fragment sf;

	if (!sp)
		sp = stack_pointer(task);

	sf.len = min((-(size_t)sp) & (THREAD_SIZE - STACK_DUMP_ENTRY_SIZE),
		     kstack_depth_to_print * STACK_DUMP_ENTRY_SIZE);
	sf.off = 0;
	sf.sp = (u8 *)sp;
	sf.loglvl = loglvl;

	printk("%sStack:\n", loglvl);
	walk_stackframe(sp, show_stack_fragment_cb, &sf);
	while (sf.off < sf.len)
		show_stack_fragment_cb(NULL, &sf);
	show_trace(task, sp, loglvl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

DEFINE_SPINLOCK(die_lock);

<<<<<<< HEAD
void die(const char * str, struct pt_regs * regs, long err)
{
	static int die_counter;
	int nl = 0;
=======
void __noreturn die(const char * str, struct pt_regs * regs, long err)
{
	static int die_counter;
	const char *pr = "";

	if (IS_ENABLED(CONFIG_PREEMPTION))
		pr = IS_ENABLED(CONFIG_PREEMPT_RT) ? " PREEMPT_RT" : " PREEMPT";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	console_verbose();
	spin_lock_irq(&die_lock);

<<<<<<< HEAD
	printk("%s: sig: %ld [#%d]\n", str, err, ++die_counter);
#ifdef CONFIG_PREEMPT
	printk("PREEMPT ");
	nl = 1;
#endif
	if (nl)
		printk("\n");
	show_regs(regs);
	if (!user_mode(regs))
		show_stack(NULL, (unsigned long*)regs->areg[1]);

	add_taint(TAINT_DIE);
=======
	pr_info("%s: sig: %ld [#%d]%s\n", str, err, ++die_counter, pr);
	show_regs(regs);
	if (!user_mode(regs))
		show_stack(NULL, (unsigned long *)regs->areg[1], KERN_INFO);

	add_taint(TAINT_DIE, LOCKDEP_NOW_UNRELIABLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irq(&die_lock);

	if (in_interrupt())
		panic("Fatal exception in interrupt");

	if (panic_on_oops)
		panic("Fatal exception");

<<<<<<< HEAD
	do_exit(err);
}


=======
	make_task_dead(err);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
