<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _X86_IRQFLAGS_H_
#define _X86_IRQFLAGS_H_

#include <asm/processor-flags.h>

#ifndef __ASSEMBLY__
<<<<<<< HEAD
=======

#include <asm/nospec-branch.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Interrupt control:
 */

<<<<<<< HEAD
static inline unsigned long native_save_fl(void)
=======
/* Declaration required for gcc < 4.9 to prevent -Werror=missing-prototypes */
extern inline unsigned long native_save_fl(void);
extern __always_inline unsigned long native_save_fl(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;

	/*
	 * "=rm" is safe here, because "pop" adjusts the stack before
	 * it evaluates its effective address -- this is part of the
	 * documented behavior of the "pop" instruction.
	 */
	asm volatile("# __raw_save_flags\n\t"
		     "pushf ; pop %0"
		     : "=rm" (flags)
		     : /* no input */
		     : "memory");

	return flags;
}

<<<<<<< HEAD
static inline void native_restore_fl(unsigned long flags)
{
	asm volatile("push %0 ; popf"
		     : /* no output */
		     :"g" (flags)
		     :"memory", "cc");
}

static inline void native_irq_disable(void)
=======
static __always_inline void native_irq_disable(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	asm volatile("cli": : :"memory");
}

<<<<<<< HEAD
static inline void native_irq_enable(void)
=======
static __always_inline void native_irq_enable(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	asm volatile("sti": : :"memory");
}

<<<<<<< HEAD
static inline void native_safe_halt(void)
{
	asm volatile("sti; hlt": : :"memory");
}

static inline void native_halt(void)
{
=======
static __always_inline void native_safe_halt(void)
{
	mds_idle_clear_cpu_buffers();
	asm volatile("sti; hlt": : :"memory");
}

static __always_inline void native_halt(void)
{
	mds_idle_clear_cpu_buffers();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	asm volatile("hlt": : :"memory");
}

#endif

<<<<<<< HEAD
#ifdef CONFIG_PARAVIRT
=======
#ifdef CONFIG_PARAVIRT_XXL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/paravirt.h>
#else
#ifndef __ASSEMBLY__
#include <linux/types.h>

<<<<<<< HEAD
static inline notrace unsigned long arch_local_save_flags(void)
=======
static __always_inline unsigned long arch_local_save_flags(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return native_save_fl();
}

<<<<<<< HEAD
static inline notrace void arch_local_irq_restore(unsigned long flags)
{
	native_restore_fl(flags);
}

static inline notrace void arch_local_irq_disable(void)
=======
static __always_inline void arch_local_irq_disable(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	native_irq_disable();
}

<<<<<<< HEAD
static inline notrace void arch_local_irq_enable(void)
=======
static __always_inline void arch_local_irq_enable(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	native_irq_enable();
}

/*
 * Used in the idle loop; sti takes one instruction cycle
 * to complete:
 */
<<<<<<< HEAD
static inline void arch_safe_halt(void)
=======
static __always_inline void arch_safe_halt(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	native_safe_halt();
}

/*
 * Used when interrupts are already enabled or to
 * shutdown the processor:
 */
<<<<<<< HEAD
static inline void halt(void)
=======
static __always_inline void halt(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	native_halt();
}

/*
 * For spinlocks, etc:
 */
<<<<<<< HEAD
static inline notrace unsigned long arch_local_irq_save(void)
=======
static __always_inline unsigned long arch_local_irq_save(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags = arch_local_save_flags();
	arch_local_irq_disable();
	return flags;
}
#else

<<<<<<< HEAD
#define ENABLE_INTERRUPTS(x)	sti
#define DISABLE_INTERRUPTS(x)	cli

#ifdef CONFIG_X86_64
#define SWAPGS	swapgs
/*
 * Currently paravirt can't handle swapgs nicely when we
 * don't have a stack we can rely on (such as a user space
 * stack).  So we either find a way around these or just fault
 * and emulate if a guest tries to call swapgs directly.
 *
 * Either way, this is a good way to document that we don't
 * have a reliable stack. x86_64 only.
 */
#define SWAPGS_UNSAFE_STACK	swapgs

#define PARAVIRT_ADJUST_EXCEPTION_FRAME	/*  */

#define INTERRUPT_RETURN	jmp native_iret
#define USERGS_SYSRET64				\
	swapgs;					\
	sysretq;
#define USERGS_SYSRET32				\
	swapgs;					\
	sysretl
#define ENABLE_INTERRUPTS_SYSEXIT32		\
	swapgs;					\
	sti;					\
	sysexit

#else
#define INTERRUPT_RETURN		iret
#define ENABLE_INTERRUPTS_SYSEXIT	sti; sysexit
#define GET_CR0_INTO_EAX		movl %cr0, %eax
#endif


#endif /* __ASSEMBLY__ */
#endif /* CONFIG_PARAVIRT */

#ifndef __ASSEMBLY__
static inline int arch_irqs_disabled_flags(unsigned long flags)
=======
#ifdef CONFIG_X86_64
#ifdef CONFIG_DEBUG_ENTRY
#define SAVE_FLAGS		pushfq; popq %rax
#endif

#endif

#endif /* __ASSEMBLY__ */
#endif /* CONFIG_PARAVIRT_XXL */

#ifndef __ASSEMBLY__
static __always_inline int arch_irqs_disabled_flags(unsigned long flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return !(flags & X86_EFLAGS_IF);
}

<<<<<<< HEAD
static inline int arch_irqs_disabled(void)
=======
static __always_inline int arch_irqs_disabled(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags = arch_local_save_flags();

	return arch_irqs_disabled_flags(flags);
}

<<<<<<< HEAD
#else

#ifdef CONFIG_X86_64
#define ARCH_LOCKDEP_SYS_EXIT		call lockdep_sys_exit_thunk
#define ARCH_LOCKDEP_SYS_EXIT_IRQ	\
	TRACE_IRQS_ON; \
	sti; \
	SAVE_REST; \
	LOCKDEP_SYS_EXIT; \
	RESTORE_REST; \
	cli; \
	TRACE_IRQS_OFF;

#else
#define ARCH_LOCKDEP_SYS_EXIT			\
	pushl %eax;				\
	pushl %ecx;				\
	pushl %edx;				\
	call lockdep_sys_exit;			\
	popl %edx;				\
	popl %ecx;				\
	popl %eax;

#define ARCH_LOCKDEP_SYS_EXIT_IRQ
#endif

#ifdef CONFIG_TRACE_IRQFLAGS
#  define TRACE_IRQS_ON		call trace_hardirqs_on_thunk;
#  define TRACE_IRQS_OFF	call trace_hardirqs_off_thunk;
#else
#  define TRACE_IRQS_ON
#  define TRACE_IRQS_OFF
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
#  define LOCKDEP_SYS_EXIT	ARCH_LOCKDEP_SYS_EXIT
#  define LOCKDEP_SYS_EXIT_IRQ	ARCH_LOCKDEP_SYS_EXIT_IRQ
# else
#  define LOCKDEP_SYS_EXIT
#  define LOCKDEP_SYS_EXIT_IRQ
# endif

#endif /* __ASSEMBLY__ */
=======
static __always_inline void arch_local_irq_restore(unsigned long flags)
{
	if (!arch_irqs_disabled_flags(flags))
		arch_local_irq_enable();
}
#endif /* !__ASSEMBLY__ */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
