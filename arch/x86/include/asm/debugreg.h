<<<<<<< HEAD
#ifndef _ASM_X86_DEBUGREG_H
#define _ASM_X86_DEBUGREG_H


/* Indicate the register numbers for a number of the specific
   debug registers.  Registers 0-3 contain the addresses we wish to trap on */
#define DR_FIRSTADDR 0        /* u_debugreg[DR_FIRSTADDR] */
#define DR_LASTADDR 3         /* u_debugreg[DR_LASTADDR]  */

#define DR_STATUS 6           /* u_debugreg[DR_STATUS]     */
#define DR_CONTROL 7          /* u_debugreg[DR_CONTROL] */

/* Define a few things for the status register.  We can use this to determine
   which debugging register was responsible for the trap.  The other bits
   are either reserved or not of interest to us. */

/* Define reserved bits in DR6 which are always set to 1 */
#define DR6_RESERVED	(0xFFFF0FF0)

#define DR_TRAP0	(0x1)		/* db0 */
#define DR_TRAP1	(0x2)		/* db1 */
#define DR_TRAP2	(0x4)		/* db2 */
#define DR_TRAP3	(0x8)		/* db3 */
#define DR_TRAP_BITS	(DR_TRAP0|DR_TRAP1|DR_TRAP2|DR_TRAP3)

#define DR_STEP		(0x4000)	/* single-step */
#define DR_SWITCH	(0x8000)	/* task switch */

/* Now define a bunch of things for manipulating the control register.
   The top two bytes of the control register consist of 4 fields of 4
   bits - each field corresponds to one of the four debug registers,
   and indicates what types of access we trap on, and how large the data
   field is that we are looking at */

#define DR_CONTROL_SHIFT 16 /* Skip this many bits in ctl register */
#define DR_CONTROL_SIZE 4   /* 4 control bits per register */

#define DR_RW_EXECUTE (0x0)   /* Settings for the access types to trap on */
#define DR_RW_WRITE (0x1)
#define DR_RW_READ (0x3)

#define DR_LEN_1 (0x0) /* Settings for data length to trap on */
#define DR_LEN_2 (0x4)
#define DR_LEN_4 (0xC)
#define DR_LEN_8 (0x8)

/* The low byte to the control register determine which registers are
   enabled.  There are 4 fields of two bits.  One bit is "local", meaning
   that the processor will reset the bit after a task switch and the other
   is global meaning that we have to explicitly reset the bit.  With linux,
   you can use either one, since we explicitly zero the register when we enter
   kernel mode. */

#define DR_LOCAL_ENABLE_SHIFT 0    /* Extra shift to the local enable bit */
#define DR_GLOBAL_ENABLE_SHIFT 1   /* Extra shift to the global enable bit */
#define DR_LOCAL_ENABLE (0x1)      /* Local enable for reg 0 */
#define DR_GLOBAL_ENABLE (0x2)     /* Global enable for reg 0 */
#define DR_ENABLE_SIZE 2           /* 2 enable bits per register */

#define DR_LOCAL_ENABLE_MASK (0x55)  /* Set  local bits for all 4 regs */
#define DR_GLOBAL_ENABLE_MASK (0xAA) /* Set global bits for all 4 regs */

/* The second byte to the control register has a few special things.
   We can slow the instruction pipeline for instructions coming via the
   gdt or the ldt if we want to.  I am not sure why this is an advantage */

#ifdef __i386__
#define DR_CONTROL_RESERVED (0xFC00) /* Reserved by Intel */
#else
#define DR_CONTROL_RESERVED (0xFFFFFFFF0000FC00UL) /* Reserved */
#endif

#define DR_LOCAL_SLOWDOWN (0x100)   /* Local slow the pipeline */
#define DR_GLOBAL_SLOWDOWN (0x200)  /* Global slow the pipeline */

/*
 * HW breakpoint additions
 */
#ifdef __KERNEL__

#include <linux/bug.h>

DECLARE_PER_CPU(unsigned long, cpu_dr7);

#ifndef CONFIG_PARAVIRT
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_DEBUGREG_H
#define _ASM_X86_DEBUGREG_H

#include <linux/bug.h>
#include <linux/percpu.h>
#include <uapi/asm/debugreg.h>

#include <asm/cpufeature.h>
#include <asm/msr.h>

DECLARE_PER_CPU(unsigned long, cpu_dr7);

#ifndef CONFIG_PARAVIRT_XXL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * These special macros can be used to get or set a debugging register
 */
#define get_debugreg(var, register)				\
	(var) = native_get_debugreg(register)
#define set_debugreg(value, register)				\
	native_set_debugreg(register, value)
#endif

<<<<<<< HEAD
static inline unsigned long native_get_debugreg(int regno)
=======
static __always_inline unsigned long native_get_debugreg(int regno)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long val = 0;	/* Damn you, gcc! */

	switch (regno) {
	case 0:
		asm("mov %%db0, %0" :"=r" (val));
		break;
	case 1:
		asm("mov %%db1, %0" :"=r" (val));
		break;
	case 2:
		asm("mov %%db2, %0" :"=r" (val));
		break;
	case 3:
		asm("mov %%db3, %0" :"=r" (val));
		break;
	case 6:
		asm("mov %%db6, %0" :"=r" (val));
		break;
	case 7:
<<<<<<< HEAD
		asm("mov %%db7, %0" :"=r" (val));
=======
		/*
		 * Apply __FORCE_ORDER to DR7 reads to forbid re-ordering them
		 * with other code.
		 *
		 * This is needed because a DR7 access can cause a #VC exception
		 * when running under SEV-ES. Taking a #VC exception is not a
		 * safe thing to do just anywhere in the entry code and
		 * re-ordering might place the access into an unsafe location.
		 *
		 * This happened in the NMI handler, where the DR7 read was
		 * re-ordered to happen before the call to sev_es_ist_enter(),
		 * causing stack recursion.
		 */
		asm volatile("mov %%db7, %0" : "=r" (val) : __FORCE_ORDER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		BUG();
	}
	return val;
}

<<<<<<< HEAD
static inline void native_set_debugreg(int regno, unsigned long value)
=======
static __always_inline void native_set_debugreg(int regno, unsigned long value)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	switch (regno) {
	case 0:
		asm("mov %0, %%db0"	::"r" (value));
		break;
	case 1:
		asm("mov %0, %%db1"	::"r" (value));
		break;
	case 2:
		asm("mov %0, %%db2"	::"r" (value));
		break;
	case 3:
		asm("mov %0, %%db3"	::"r" (value));
		break;
	case 6:
		asm("mov %0, %%db6"	::"r" (value));
		break;
	case 7:
<<<<<<< HEAD
		asm("mov %0, %%db7"	::"r" (value));
=======
		/*
		 * Apply __FORCE_ORDER to DR7 writes to forbid re-ordering them
		 * with other code.
		 *
		 * While is didn't happen with a DR7 write (see the DR7 read
		 * comment above which explains where it happened), add the
		 * __FORCE_ORDER here too to avoid similar problems in the
		 * future.
		 */
		asm volatile("mov %0, %%db7"	::"r" (value), __FORCE_ORDER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		BUG();
	}
}

static inline void hw_breakpoint_disable(void)
{
	/* Zero the control register for HW Breakpoint */
	set_debugreg(0UL, 7);

	/* Zero-out the individual HW breakpoint address registers */
	set_debugreg(0UL, 0);
	set_debugreg(0UL, 1);
	set_debugreg(0UL, 2);
	set_debugreg(0UL, 3);
}

<<<<<<< HEAD
static inline int hw_breakpoint_active(void)
=======
static __always_inline bool hw_breakpoint_active(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return __this_cpu_read(cpu_dr7) & DR_GLOBAL_ENABLE_MASK;
}

<<<<<<< HEAD
extern void aout_dump_debugregs(struct user *dump);

extern void hw_breakpoint_restore(void);

#ifdef CONFIG_X86_64
DECLARE_PER_CPU(int, debug_stack_usage);
static inline void debug_stack_usage_inc(void)
{
	__get_cpu_var(debug_stack_usage)++;
}
static inline void debug_stack_usage_dec(void)
{
	__get_cpu_var(debug_stack_usage)--;
}
int is_debug_stack(unsigned long addr);
void debug_stack_set_zero(void);
void debug_stack_reset(void);
#else /* !X86_64 */
static inline int is_debug_stack(unsigned long addr) { return 0; }
static inline void debug_stack_set_zero(void) { }
static inline void debug_stack_reset(void) { }
static inline void debug_stack_usage_inc(void) { }
static inline void debug_stack_usage_dec(void) { }
#endif /* X86_64 */


#endif	/* __KERNEL__ */
=======
extern void hw_breakpoint_restore(void);

static __always_inline unsigned long local_db_save(void)
{
	unsigned long dr7;

	if (static_cpu_has(X86_FEATURE_HYPERVISOR) && !hw_breakpoint_active())
		return 0;

	get_debugreg(dr7, 7);
	dr7 &= ~0x400; /* architecturally set bit */
	if (dr7)
		set_debugreg(0, 7);
	/*
	 * Ensure the compiler doesn't lower the above statements into
	 * the critical section; disabling breakpoints late would not
	 * be good.
	 */
	barrier();

	return dr7;
}

static __always_inline void local_db_restore(unsigned long dr7)
{
	/*
	 * Ensure the compiler doesn't raise this statement into
	 * the critical section; enabling breakpoints early would
	 * not be good.
	 */
	barrier();
	if (dr7)
		set_debugreg(dr7, 7);
}

#ifdef CONFIG_CPU_SUP_AMD
extern void amd_set_dr_addr_mask(unsigned long mask, unsigned int dr);
extern unsigned long amd_get_dr_addr_mask(unsigned int dr);
#else
static inline void amd_set_dr_addr_mask(unsigned long mask, unsigned int dr) { }
static inline unsigned long amd_get_dr_addr_mask(unsigned int dr)
{
	return 0;
}
#endif

static inline unsigned long get_debugctlmsr(void)
{
	unsigned long debugctlmsr = 0;

#ifndef CONFIG_X86_DEBUGCTLMSR
	if (boot_cpu_data.x86 < 6)
		return 0;
#endif
	rdmsrl(MSR_IA32_DEBUGCTLMSR, debugctlmsr);

	return debugctlmsr;
}

static inline void update_debugctlmsr(unsigned long debugctlmsr)
{
#ifndef CONFIG_X86_DEBUGCTLMSR
	if (boot_cpu_data.x86 < 6)
		return;
#endif
	wrmsrl(MSR_IA32_DEBUGCTLMSR, debugctlmsr);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_DEBUGREG_H */
