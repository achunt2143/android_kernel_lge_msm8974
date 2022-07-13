<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_POWERPC_BUG_H
#define _ASM_POWERPC_BUG_H
#ifdef __KERNEL__

#include <asm/asm-compat.h>

<<<<<<< HEAD
/*
 * Define an illegal instr to trap on the bug.
 * We don't use 0 because that marks the end of a function
 * in the ELF ABI.  That's "Boo Boo" in case you wonder...
 */
#define BUG_OPCODE .long 0x00b00b00  /* For asm */
#define BUG_ILLEGAL_INSTR "0x00b00b00" /* For BUG macro */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_BUG

#ifdef __ASSEMBLY__
#include <asm/asm-offsets.h>
#ifdef CONFIG_DEBUG_BUGVERBOSE
.macro EMIT_BUG_ENTRY addr,file,line,flags
<<<<<<< HEAD
	 .section __bug_table,"a"
5001:	 PPC_LONG \addr, 5002f
=======
	 .section __bug_table,"aw"
5001:	 .4byte \addr - .
	 .4byte 5002f - .
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 .short \line, \flags
	 .org 5001b+BUG_ENTRY_SIZE
	 .previous
	 .section .rodata,"a"
5002:	 .asciz "\file"
	 .previous
.endm
#else
.macro EMIT_BUG_ENTRY addr,file,line,flags
<<<<<<< HEAD
	 .section __bug_table,"a"
5001:	 PPC_LONG \addr
=======
	 .section __bug_table,"aw"
5001:	 .4byte \addr - .
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 .short \flags
	 .org 5001b+BUG_ENTRY_SIZE
	 .previous
.endm
#endif /* verbose */

#else /* !__ASSEMBLY__ */
/* _EMIT_BUG_ENTRY expects args %0,%1,%2,%3 to be FILE, LINE, flags and
   sizeof(struct bug_entry), respectively */
#ifdef CONFIG_DEBUG_BUGVERBOSE
#define _EMIT_BUG_ENTRY				\
<<<<<<< HEAD
	".section __bug_table,\"a\"\n"		\
	"2:\t" PPC_LONG "1b, %0\n"		\
	"\t.short %1, %2\n"			\
=======
	".section __bug_table,\"aw\"\n"		\
	"2:	.4byte 1b - .\n"		\
	"	.4byte %0 - .\n"		\
	"	.short %1, %2\n"		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	".org 2b+%3\n"				\
	".previous\n"
#else
#define _EMIT_BUG_ENTRY				\
<<<<<<< HEAD
	".section __bug_table,\"a\"\n"		\
	"2:\t" PPC_LONG "1b\n"			\
	"\t.short %2\n"				\
=======
	".section __bug_table,\"aw\"\n"		\
	"2:	.4byte 1b - .\n"		\
	"	.short %2\n"			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	".org 2b+%3\n"				\
	".previous\n"
#endif

<<<<<<< HEAD
=======
#define BUG_ENTRY(insn, flags, ...)			\
	__asm__ __volatile__(				\
		"1:	" insn "\n"			\
		_EMIT_BUG_ENTRY				\
		: : "i" (__FILE__), "i" (__LINE__),	\
		  "i" (flags),				\
		  "i" (sizeof(struct bug_entry)),	\
		  ##__VA_ARGS__)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * BUG_ON() and WARN_ON() do their best to cooperate with compile-time
 * optimisations. However depending on the complexity of the condition
 * some compiler versions may not produce optimal results.
 */

#define BUG() do {						\
<<<<<<< HEAD
	__asm__ __volatile__(					\
		"1:	twi 31,0,0\n"				\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__),		\
		    "i" (0), "i"  (sizeof(struct bug_entry)));	\
	unreachable();						\
} while (0)

=======
	BUG_ENTRY("twi 31, 0, 0", 0);				\
	unreachable();						\
} while (0)
#define HAVE_ARCH_BUG

#define __WARN_FLAGS(flags) BUG_ENTRY("twi 31, 0, 0", BUGFLAG_WARNING | (flags))

#ifdef CONFIG_PPC64
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define BUG_ON(x) do {						\
	if (__builtin_constant_p(x)) {				\
		if (x)						\
			BUG();					\
	} else {						\
<<<<<<< HEAD
		__asm__ __volatile__(				\
		"1:	"PPC_TLNEI"	%4,0\n"			\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__), "i" (0),	\
		  "i" (sizeof(struct bug_entry)),		\
		  "r" ((__force long)(x)));			\
	}							\
} while (0)

#define __WARN_TAINT(taint) do {				\
	__asm__ __volatile__(					\
		"1:	twi 31,0,0\n"				\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__),		\
		  "i" (BUGFLAG_TAINT(taint)),			\
		  "i" (sizeof(struct bug_entry)));		\
} while (0)

=======
		BUG_ENTRY(PPC_TLNEI " %4, 0", 0, "r" ((__force long)(x)));	\
	}							\
} while (0)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define WARN_ON(x) ({						\
	int __ret_warn_on = !!(x);				\
	if (__builtin_constant_p(__ret_warn_on)) {		\
		if (__ret_warn_on)				\
			__WARN();				\
	} else {						\
<<<<<<< HEAD
		__asm__ __volatile__(				\
		"1:	"PPC_TLNEI"	%4,0\n"			\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__),		\
		  "i" (BUGFLAG_TAINT(TAINT_WARN)),		\
		  "i" (sizeof(struct bug_entry)),		\
		  "r" (__ret_warn_on));				\
=======
		BUG_ENTRY(PPC_TLNEI " %4, 0",			\
			  BUGFLAG_WARNING | BUGFLAG_TAINT(TAINT_WARN),	\
			  "r" (__ret_warn_on));	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}							\
	unlikely(__ret_warn_on);				\
})

<<<<<<< HEAD
#define HAVE_ARCH_BUG
#define HAVE_ARCH_BUG_ON
#define HAVE_ARCH_WARN_ON
=======
#define HAVE_ARCH_BUG_ON
#define HAVE_ARCH_WARN_ON
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASSEMBLY __ */
#else
#ifdef __ASSEMBLY__
.macro EMIT_BUG_ENTRY addr,file,line,flags
.endm
#else /* !__ASSEMBLY__ */
#define _EMIT_BUG_ENTRY
#endif
#endif /* CONFIG_BUG */

<<<<<<< HEAD
=======
#define EMIT_WARN_ENTRY EMIT_BUG_ENTRY

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/bug.h>

#ifndef __ASSEMBLY__

struct pt_regs;
<<<<<<< HEAD
extern int do_page_fault(struct pt_regs *, unsigned long, unsigned long);
extern void bad_page_fault(struct pt_regs *, unsigned long, int);
extern void _exception(int, struct pt_regs *, int, unsigned long);
extern void die(const char *, struct pt_regs *, long);
extern void print_backtrace(unsigned long *);

=======
void hash__do_page_fault(struct pt_regs *);
void bad_page_fault(struct pt_regs *, int);
void emulate_single_step(struct pt_regs *regs);
extern void _exception(int, struct pt_regs *, int, unsigned long);
extern void _exception_pkey(struct pt_regs *, unsigned long, int);
extern void die(const char *, struct pt_regs *, long);
void die_mce(const char *str, struct pt_regs *regs, long err);
extern bool die_will_crash(void);
extern void panic_flush_kmsg_start(void);
extern void panic_flush_kmsg_end(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* !__ASSEMBLY__ */

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_BUG_H */
