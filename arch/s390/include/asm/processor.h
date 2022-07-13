<<<<<<< HEAD
/*
 *  include/asm-s390/processor.h
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  S390 version
 *    Copyright IBM Corp. 1999
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Hartmut Penner (hp@de.ibm.com),
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 *  Derived from "include/asm-i386/processor.h"
 *    Copyright (C) 1994, Linus Torvalds
 */

#ifndef __ASM_S390_PROCESSOR_H
#define __ASM_S390_PROCESSOR_H

<<<<<<< HEAD
#include <linux/linkage.h>
#include <linux/irqflags.h>
=======
#include <linux/bits.h>

#define CIF_SIE			0	/* CPU needs SIE exit cleanup */
#define CIF_NOHZ_DELAY		2	/* delay HZ disable for a tick */
#define CIF_ENABLED_WAIT	5	/* in enabled wait state */
#define CIF_MCCK_GUEST		6	/* machine check happening in guest */
#define CIF_DEDICATED_CPU	7	/* this CPU is dedicated */

#define _CIF_SIE		BIT(CIF_SIE)
#define _CIF_NOHZ_DELAY		BIT(CIF_NOHZ_DELAY)
#define _CIF_ENABLED_WAIT	BIT(CIF_ENABLED_WAIT)
#define _CIF_MCCK_GUEST		BIT(CIF_MCCK_GUEST)
#define _CIF_DEDICATED_CPU	BIT(CIF_DEDICATED_CPU)

#define RESTART_FLAG_CTLREGS	_AC(1 << 0, U)

#ifndef __ASSEMBLY__

#include <linux/cpumask.h>
#include <linux/linkage.h>
#include <linux/irqflags.h>
#include <asm/fpu-types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/cpu.h>
#include <asm/page.h>
#include <asm/ptrace.h>
#include <asm/setup.h>
<<<<<<< HEAD

#ifdef __KERNEL__
/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
#define current_text_addr() ({ void *pc; asm("basr %0,0" : "=a" (pc)); pc; })
=======
#include <asm/runtime_instr.h>
#include <asm/irqflags.h>

typedef long (*sys_call_ptr_t)(struct pt_regs *regs);

static __always_inline void set_cpu_flag(int flag)
{
	S390_lowcore.cpu_flags |= (1UL << flag);
}

static __always_inline void clear_cpu_flag(int flag)
{
	S390_lowcore.cpu_flags &= ~(1UL << flag);
}

static __always_inline bool test_cpu_flag(int flag)
{
	return S390_lowcore.cpu_flags & (1UL << flag);
}

static __always_inline bool test_and_set_cpu_flag(int flag)
{
	if (test_cpu_flag(flag))
		return true;
	set_cpu_flag(flag);
	return false;
}

static __always_inline bool test_and_clear_cpu_flag(int flag)
{
	if (!test_cpu_flag(flag))
		return false;
	clear_cpu_flag(flag);
	return true;
}

/*
 * Test CIF flag of another CPU. The caller needs to ensure that
 * CPU hotplug can not happen, e.g. by disabling preemption.
 */
static __always_inline bool test_cpu_flag_of(int flag, int cpu)
{
	struct lowcore *lc = lowcore_ptr[cpu];

	return lc->cpu_flags & (1UL << flag);
}

#define arch_needs_cpu() test_cpu_flag(CIF_NOHZ_DELAY)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void get_cpu_id(struct cpuid *ptr)
{
	asm volatile("stidp %0" : "=Q" (*ptr));
}

<<<<<<< HEAD
extern void s390_adjust_jiffies(void);
extern int get_cpu_capability(unsigned int *);
extern const struct seq_operations cpuinfo_op;
extern int sysctl_ieee_emulation_warnings;
=======
void s390_adjust_jiffies(void);
void s390_update_cpu_mhz(void);
void cpu_detect_mhz_feature(void);

extern const struct seq_operations cpuinfo_op;
extern void execve_tail(void);
unsigned long vdso_size(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * User space process size: 2GB for 31 bit, 4TB or 8PT for 64 bit.
 */
<<<<<<< HEAD
#ifndef __s390x__

#define TASK_SIZE		(1UL << 31)
#define TASK_UNMAPPED_BASE	(1UL << 30)

#else /* __s390x__ */

#define TASK_SIZE_OF(tsk)	((tsk)->mm->context.asce_limit)
#define TASK_UNMAPPED_BASE	(test_thread_flag(TIF_31BIT) ? \
					(1UL << 30) : (1UL << 41))
#define TASK_SIZE		TASK_SIZE_OF(current)

#endif /* __s390x__ */

#ifdef __KERNEL__

#ifndef __s390x__
#define STACK_TOP		(1UL << 31)
#define STACK_TOP_MAX		(1UL << 31)
#else /* __s390x__ */
#define STACK_TOP		(1UL << (test_thread_flag(TIF_31BIT) ? 31:42))
#define STACK_TOP_MAX		(1UL << 42)
#endif /* __s390x__ */


#endif

#define HAVE_ARCH_PICK_MMAP_LAYOUT

typedef struct {
        __u32 ar4;
} mm_segment_t;
=======

#define TASK_SIZE		(test_thread_flag(TIF_31BIT) ? \
					_REGION3_SIZE : TASK_SIZE_MAX)
#define TASK_UNMAPPED_BASE	(test_thread_flag(TIF_31BIT) ? \
					(_REGION3_SIZE >> 1) : (_REGION2_SIZE >> 1))
#define TASK_SIZE_MAX		(-PAGE_SIZE)

#define VDSO_BASE		(STACK_TOP + PAGE_SIZE)
#define VDSO_LIMIT		(test_thread_flag(TIF_31BIT) ? _REGION3_SIZE : _REGION2_SIZE)
#define STACK_TOP		(VDSO_LIMIT - vdso_size() - PAGE_SIZE)
#define STACK_TOP_MAX		(_REGION2_SIZE - vdso_size() - PAGE_SIZE)

#define HAVE_ARCH_PICK_MMAP_LAYOUT

#define __stackleak_poison __stackleak_poison
static __always_inline void __stackleak_poison(unsigned long erase_low,
					       unsigned long erase_high,
					       unsigned long poison)
{
	unsigned long tmp, count;

	count = erase_high - erase_low;
	if (!count)
		return;
	asm volatile(
		"	cghi	%[count],8\n"
		"	je	2f\n"
		"	aghi	%[count],-(8+1)\n"
		"	srlg	%[tmp],%[count],8\n"
		"	ltgr	%[tmp],%[tmp]\n"
		"	jz	1f\n"
		"0:	stg	%[poison],0(%[addr])\n"
		"	mvc	8(256-8,%[addr]),0(%[addr])\n"
		"	la	%[addr],256(%[addr])\n"
		"	brctg	%[tmp],0b\n"
		"1:	stg	%[poison],0(%[addr])\n"
		"	larl	%[tmp],3f\n"
		"	ex	%[count],0(%[tmp])\n"
		"	j	4f\n"
		"2:	stg	%[poison],0(%[addr])\n"
		"	j	4f\n"
		"3:	mvc	8(1,%[addr]),0(%[addr])\n"
		"4:\n"
		: [addr] "+&a" (erase_low), [count] "+&d" (count), [tmp] "=&a" (tmp)
		: [poison] "d" (poison)
		: "memory", "cc"
		);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Thread structure
 */
struct thread_struct {
<<<<<<< HEAD
	s390_fp_regs fp_regs;
	unsigned int  acrs[NUM_ACRS];
        unsigned long ksp;              /* kernel stack pointer             */
	mm_segment_t mm_segment;
	unsigned long gmap_addr;	/* address of last gmap fault. */
	struct per_regs per_user;	/* User specified PER registers */
	struct per_event per_event;	/* Cause of the last PER trap */
        /* pfault_wait is used to block the process on a pfault event */
	unsigned long pfault_wait;
	struct list_head list;
};

typedef struct thread_struct thread_struct;

/*
 * Stack layout of a C stack frame.
 */
#ifndef __PACK_STACK
struct stack_frame {
	unsigned long back_chain;
	unsigned long empty1[5];
	unsigned long gprs[10];
	unsigned int  empty2[8];
};
#else
struct stack_frame {
	unsigned long empty1[5];
	unsigned int  empty2[8];
	unsigned long gprs[10];
	unsigned long back_chain;
};
#endif

=======
	unsigned int  acrs[NUM_ACRS];
	unsigned long ksp;			/* kernel stack pointer */
	unsigned long user_timer;		/* task cputime in user space */
	unsigned long guest_timer;		/* task cputime in kvm guest */
	unsigned long system_timer;		/* task cputime in kernel space */
	unsigned long hardirq_timer;		/* task cputime in hardirq context */
	unsigned long softirq_timer;		/* task cputime in softirq context */
	const sys_call_ptr_t *sys_call_table;	/* system call table address */
	unsigned long gmap_addr;		/* address of last gmap fault. */
	unsigned int gmap_write_flag;		/* gmap fault write indication */
	unsigned int gmap_int_code;		/* int code of last gmap fault */
	unsigned int gmap_pfault;		/* signal of a pending guest pfault */
	int ufpu_flags;				/* user fpu flags */
	int kfpu_flags;				/* kernel fpu flags */

	/* Per-thread information related to debugging */
	struct per_regs per_user;		/* User specified PER registers */
	struct per_event per_event;		/* Cause of the last PER trap */
	unsigned long per_flags;		/* Flags to control debug behavior */
	unsigned int system_call;		/* system call number in signal */
	unsigned long last_break;		/* last breaking-event-address. */
	/* pfault_wait is used to block the process on a pfault event */
	unsigned long pfault_wait;
	struct list_head list;
	/* cpu runtime instrumentation */
	struct runtime_instr_cb *ri_cb;
	struct gs_cb *gs_cb;			/* Current guarded storage cb */
	struct gs_cb *gs_bc_cb;			/* Broadcast guarded storage cb */
	struct pgm_tdb trap_tdb;		/* Transaction abort diagnose block */
	struct fpu ufpu;			/* User FP and VX register save area */
	struct fpu kfpu;			/* Kernel FP and VX register save area */
};

/* Flag to disable transactions. */
#define PER_FLAG_NO_TE			1UL
/* Flag to enable random transaction aborts. */
#define PER_FLAG_TE_ABORT_RAND		2UL
/* Flag to specify random transaction abort mode:
 * - abort each transaction at a random instruction before TEND if set.
 * - abort random transactions at a random instruction if cleared.
 */
#define PER_FLAG_TE_ABORT_RAND_TEND	4UL

typedef struct thread_struct thread_struct;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ARCH_MIN_TASKALIGN	8

#define INIT_THREAD {							\
	.ksp = sizeof(init_stack) + (unsigned long) &init_stack,	\
<<<<<<< HEAD
=======
	.last_break = 1,						\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Do necessary setup to start up a new thread.
 */
#define start_thread(regs, new_psw, new_stackp) do {			\
<<<<<<< HEAD
	regs->psw.mask	= psw_user_bits | PSW_MASK_EA | PSW_MASK_BA;	\
	regs->psw.addr	= new_psw | PSW_ADDR_AMODE;			\
	regs->gprs[15]	= new_stackp;					\
} while (0)

#define start_thread31(regs, new_psw, new_stackp) do {			\
	regs->psw.mask	= psw_user_bits | PSW_MASK_BA;			\
	regs->psw.addr	= new_psw | PSW_ADDR_AMODE;			\
	regs->gprs[15]	= new_stackp;					\
	__tlb_flush_mm(current->mm);					\
	crst_table_downgrade(current->mm, 1UL << 31);			\
	update_mm(current->mm, current);				\
} while (0)

/* Forward declaration, a strange C thing */
struct task_struct;
struct mm_struct;
struct seq_file;

/* Free all resources held by a thread. */
extern void release_thread(struct task_struct *);
extern int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags);

/* Prepare to copy thread state - unlazy all lazy status */
#define prepare_to_copy(tsk)	do { } while (0)

/*
 * Return saved PC of a blocked thread.
 */
extern unsigned long thread_saved_pc(struct task_struct *t);

extern void show_code(struct pt_regs *regs);

unsigned long get_wchan(struct task_struct *p);
=======
	regs->psw.mask	= PSW_USER_BITS | PSW_MASK_EA | PSW_MASK_BA;	\
	regs->psw.addr	= new_psw;					\
	regs->gprs[15]	= new_stackp;					\
	execve_tail();							\
} while (0)

#define start_thread31(regs, new_psw, new_stackp) do {			\
	regs->psw.mask	= PSW_USER_BITS | PSW_MASK_BA;			\
	regs->psw.addr	= new_psw;					\
	regs->gprs[15]	= new_stackp;					\
	execve_tail();							\
} while (0)

struct task_struct;
struct mm_struct;
struct seq_file;
struct pt_regs;

void show_registers(struct pt_regs *regs);
void show_cacheinfo(struct seq_file *m);

/* Free guarded storage control block */
void guarded_storage_release(struct task_struct *tsk);
void gs_load_bc_cb(struct pt_regs *regs);

unsigned long __get_wchan(struct task_struct *p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define task_pt_regs(tsk) ((struct pt_regs *) \
        (task_stack_page(tsk) + THREAD_SIZE) - 1)
#define KSTK_EIP(tsk)	(task_pt_regs(tsk)->psw.addr)
#define KSTK_ESP(tsk)	(task_pt_regs(tsk)->gprs[15])

<<<<<<< HEAD
static inline unsigned short stap(void)
{
	unsigned short cpu_address;

	asm volatile("stap %0" : "=m" (cpu_address));
	return cpu_address;
}

/*
 * Give up the time slice of the virtual PU.
 */
static inline void cpu_relax(void)
{
	if (MACHINE_HAS_DIAG44)
		asm volatile("diag 0,0,68");
	barrier();
=======
/* Has task runtime instrumentation enabled ? */
#define is_ri_task(tsk) (!!(tsk)->thread.ri_cb)

/* avoid using global register due to gcc bug in versions < 8.4 */
#define current_stack_pointer (__current_stack_pointer())

static __always_inline unsigned long __current_stack_pointer(void)
{
	unsigned long sp;

	asm volatile("lgr %0,15" : "=d" (sp));
	return sp;
}

static __always_inline bool on_thread_stack(void)
{
	unsigned long ksp = S390_lowcore.kernel_stack;

	return !((ksp ^ current_stack_pointer) & ~(THREAD_SIZE - 1));
}

static __always_inline unsigned short stap(void)
{
	unsigned short cpu_address;

	asm volatile("stap %0" : "=Q" (cpu_address));
	return cpu_address;
}

#define cpu_relax() barrier()

#define ECAG_CACHE_ATTRIBUTE	0
#define ECAG_CPU_ATTRIBUTE	1

static inline unsigned long __ecag(unsigned int asi, unsigned char parm)
{
	unsigned long val;

	asm volatile("ecag %0,0,0(%1)" : "=d" (val) : "a" (asi << 8 | parm));
	return val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void psw_set_key(unsigned int key)
{
	asm volatile("spka 0(%0)" : : "d" (key));
}

/*
 * Set PSW to specified value.
 */
static inline void __load_psw(psw_t psw)
{
<<<<<<< HEAD
#ifndef __s390x__
	asm volatile("lpsw  %0" : : "Q" (psw) : "cc");
#else
	asm volatile("lpswe %0" : : "Q" (psw) : "cc");
#endif
=======
	asm volatile("lpswe %0" : : "Q" (psw) : "cc");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Set PSW mask to specified value, while leaving the
 * PSW addr pointing to the next instruction.
 */
<<<<<<< HEAD
static inline void __load_psw_mask (unsigned long mask)
=======
static __always_inline void __load_psw_mask(unsigned long mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long addr;
	psw_t psw;

	psw.mask = mask;

<<<<<<< HEAD
#ifndef __s390x__
	asm volatile(
		"	basr	%0,0\n"
		"0:	ahi	%0,1f-0b\n"
		"	st	%0,%O1+4(%R1)\n"
		"	lpsw	%1\n"
		"1:"
		: "=&d" (addr), "=Q" (psw) : "Q" (psw) : "memory", "cc");
#else /* __s390x__ */
	asm volatile(
		"	larl	%0,1f\n"
		"	stg	%0,%O1+8(%R1)\n"
		"	lpswe	%1\n"
		"1:"
		: "=&d" (addr), "=Q" (psw) : "Q" (psw) : "memory", "cc");
#endif /* __s390x__ */
=======
	asm volatile(
		"	larl	%0,1f\n"
		"	stg	%0,%1\n"
		"	lpswe	%2\n"
		"1:"
		: "=&d" (addr), "=Q" (psw.addr) : "Q" (psw) : "memory", "cc");
}

/*
 * Extract current PSW mask
 */
static inline unsigned long __extract_psw(void)
{
	unsigned int reg1, reg2;

	asm volatile("epsw %0,%1" : "=d" (reg1), "=a" (reg2));
	return (((unsigned long) reg1) << 32) | ((unsigned long) reg2);
}

static inline unsigned long __local_mcck_save(void)
{
	unsigned long mask = __extract_psw();

	__load_psw_mask(mask & ~PSW_MASK_MCHECK);
	return mask & PSW_MASK_MCHECK;
}

#define local_mcck_save(mflags)			\
do {						\
	typecheck(unsigned long, mflags);	\
	mflags = __local_mcck_save();		\
} while (0)

static inline void local_mcck_restore(unsigned long mflags)
{
	unsigned long mask = __extract_psw();

	mask &= ~PSW_MASK_MCHECK;
	__load_psw_mask(mask | mflags);
}

static inline void local_mcck_disable(void)
{
	__local_mcck_save();
}

static inline void local_mcck_enable(void)
{
	__load_psw_mask(__extract_psw() | PSW_MASK_MCHECK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Rewind PSW instruction address by specified number of bytes.
 */
static inline unsigned long __rewind_psw(psw_t psw, unsigned long ilc)
{
<<<<<<< HEAD
#ifndef __s390x__
	if (psw.addr & PSW_ADDR_AMODE)
		/* 31 bit mode */
		return (psw.addr - ilc) | PSW_ADDR_AMODE;
	/* 24 bit mode */
	return (psw.addr - ilc) & ((1UL << 24) - 1);
#else
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long mask;

	mask = (psw.mask & PSW_MASK_EA) ? -1UL :
	       (psw.mask & PSW_MASK_BA) ? (1UL << 31) - 1 :
					  (1UL << 24) - 1;
	return (psw.addr - ilc) & mask;
<<<<<<< HEAD
#endif
}
 
/*
 * Function to drop a processor into disabled wait state
 */
static inline void __noreturn disabled_wait(unsigned long code)
{
        unsigned long ctl_buf;
        psw_t dw_psw;

	dw_psw.mask = PSW_MASK_BASE | PSW_MASK_WAIT | PSW_MASK_BA | PSW_MASK_EA;
        dw_psw.addr = code;
        /* 
         * Store status and then load disabled wait psw,
         * the processor is dead afterwards
         */
#ifndef __s390x__
	asm volatile(
		"	stctl	0,0,0(%2)\n"
		"	ni	0(%2),0xef\n"	/* switch off protection */
		"	lctl	0,0,0(%2)\n"
		"	stpt	0xd8\n"		/* store timer */
		"	stckc	0xe0\n"		/* store clock comparator */
		"	stpx	0x108\n"	/* store prefix register */
		"	stam	0,15,0x120\n"	/* store access registers */
		"	std	0,0x160\n"	/* store f0 */
		"	std	2,0x168\n"	/* store f2 */
		"	std	4,0x170\n"	/* store f4 */
		"	std	6,0x178\n"	/* store f6 */
		"	stm	0,15,0x180\n"	/* store general registers */
		"	stctl	0,15,0x1c0\n"	/* store control registers */
		"	oi	0x1c0,0x10\n"	/* fake protection bit */
		"	lpsw	0(%1)"
		: "=m" (ctl_buf)
		: "a" (&dw_psw), "a" (&ctl_buf), "m" (dw_psw) : "cc");
#else /* __s390x__ */
	asm volatile(
		"	stctg	0,0,0(%2)\n"
		"	ni	4(%2),0xef\n"	/* switch off protection */
		"	lctlg	0,0,0(%2)\n"
		"	lghi	1,0x1000\n"
		"	stpt	0x328(1)\n"	/* store timer */
		"	stckc	0x330(1)\n"	/* store clock comparator */
		"	stpx	0x318(1)\n"	/* store prefix register */
		"	stam	0,15,0x340(1)\n"/* store access registers */
		"	stfpc	0x31c(1)\n"	/* store fpu control */
		"	std	0,0x200(1)\n"	/* store f0 */
		"	std	1,0x208(1)\n"	/* store f1 */
		"	std	2,0x210(1)\n"	/* store f2 */
		"	std	3,0x218(1)\n"	/* store f3 */
		"	std	4,0x220(1)\n"	/* store f4 */
		"	std	5,0x228(1)\n"	/* store f5 */
		"	std	6,0x230(1)\n"	/* store f6 */
		"	std	7,0x238(1)\n"	/* store f7 */
		"	std	8,0x240(1)\n"	/* store f8 */
		"	std	9,0x248(1)\n"	/* store f9 */
		"	std	10,0x250(1)\n"	/* store f10 */
		"	std	11,0x258(1)\n"	/* store f11 */
		"	std	12,0x260(1)\n"	/* store f12 */
		"	std	13,0x268(1)\n"	/* store f13 */
		"	std	14,0x270(1)\n"	/* store f14 */
		"	std	15,0x278(1)\n"	/* store f15 */
		"	stmg	0,15,0x280(1)\n"/* store general registers */
		"	stctg	0,15,0x380(1)\n"/* store control registers */
		"	oi	0x384(1),0x10\n"/* fake protection bit */
		"	lpswe	0(%1)"
		: "=m" (ctl_buf)
		: "a" (&dw_psw), "a" (&ctl_buf), "m" (dw_psw) : "cc", "0", "1");
#endif /* __s390x__ */
	while (1);
}

/*
 * Use to set psw mask except for the first byte which
 * won't be changed by this function.
 */
static inline void
__set_psw_mask(unsigned long mask)
{
	__load_psw_mask(mask | (arch_local_save_flags() & ~(-1UL >> 8)));
}

#define local_mcck_enable() \
	__set_psw_mask(psw_kernel_bits | PSW_MASK_DAT | PSW_MASK_MCHECK)
#define local_mcck_disable() \
	__set_psw_mask(psw_kernel_bits | PSW_MASK_DAT)

/*
 * Basic Machine Check/Program Check Handler.
 */

extern void s390_base_mcck_handler(void);
extern void s390_base_pgm_handler(void);
extern void s390_base_ext_handler(void);

extern void (*s390_base_mcck_handler_fn)(void);
extern void (*s390_base_pgm_handler_fn)(void);
extern void (*s390_base_ext_handler_fn)(void);

#define ARCH_LOW_ADDRESS_LIMIT	0x7fffffffUL

#endif

/*
 * Helper macro for exception table entries
 */
#ifndef __s390x__
#define EX_TABLE(_fault,_target)			\
	".section __ex_table,\"a\"\n"			\
	"	.align 4\n"				\
	"	.long  " #_fault "," #_target "\n"	\
	".previous\n"
#else
#define EX_TABLE(_fault,_target)			\
	".section __ex_table,\"a\"\n"			\
	"	.align 8\n"				\
	"	.quad  " #_fault "," #_target "\n"	\
	".previous\n"
#endif

#endif                                 /* __ASM_S390_PROCESSOR_H           */
=======
}

/*
 * Function to drop a processor into disabled wait state
 */
static __always_inline void __noreturn disabled_wait(void)
{
	psw_t psw;

	psw.mask = PSW_MASK_BASE | PSW_MASK_WAIT | PSW_MASK_BA | PSW_MASK_EA;
	psw.addr = _THIS_IP_;
	__load_psw(psw);
	while (1);
}

#define ARCH_LOW_ADDRESS_LIMIT	0x7fffffffUL

static __always_inline bool regs_irqs_disabled(struct pt_regs *regs)
{
	return arch_irqs_disabled_flags(regs->psw.mask);
}

#endif /* __ASSEMBLY__ */

#endif /* __ASM_S390_PROCESSOR_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
