<<<<<<< HEAD
#ifndef _ASM_S390_BUG_H
#define _ASM_S390_BUG_H

#include <linux/kernel.h>
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_S390_BUG_H
#define _ASM_S390_BUG_H

#include <linux/compiler.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_BUG

#ifdef CONFIG_DEBUG_BUGVERBOSE

#define __EMIT_BUG(x) do {					\
<<<<<<< HEAD
	asm volatile(						\
		"0:	j	0b+2\n"				\
		"1:\n"						\
		".section .rodata.str,\"aMS\",@progbits,1\n"	\
		"2:	.asciz	\""__FILE__"\"\n"		\
		".previous\n"					\
		".section __bug_table,\"a\"\n"			\
		"3:	.long	1b-3b,2b-3b\n"			\
		"	.short	%0,%1\n"			\
		"	.org	3b+%2\n"			\
=======
	asm_inline volatile(					\
		"0:	mc	0,0\n"				\
		".section .rodata.str,\"aMS\",@progbits,1\n"	\
		"1:	.asciz	\""__FILE__"\"\n"		\
		".previous\n"					\
		".section __bug_table,\"aw\"\n"			\
		"2:	.long	0b-.\n"				\
		"	.long	1b-.\n"				\
		"	.short	%0,%1\n"			\
		"	.org	2b+%2\n"			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		".previous\n"					\
		: : "i" (__LINE__),				\
		    "i" (x),					\
		    "i" (sizeof(struct bug_entry)));		\
} while (0)

#else /* CONFIG_DEBUG_BUGVERBOSE */

<<<<<<< HEAD
#define __EMIT_BUG(x) do {				\
	asm volatile(					\
		"0:	j	0b+2\n"			\
		"1:\n"					\
		".section __bug_table,\"a\"\n"		\
		"2:	.long	1b-2b\n"		\
		"	.short	%0\n"			\
		"	.org	2b+%1\n"		\
		".previous\n"				\
		: : "i" (x),				\
		    "i" (sizeof(struct bug_entry)));	\
=======
#define __EMIT_BUG(x) do {					\
	asm_inline volatile(					\
		"0:	mc	0,0\n"				\
		".section __bug_table,\"aw\"\n"			\
		"1:	.long	0b-.\n"				\
		"	.short	%0\n"				\
		"	.org	1b+%1\n"			\
		".previous\n"					\
		: : "i" (x),					\
		    "i" (sizeof(struct bug_entry)));		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} while (0)

#endif /* CONFIG_DEBUG_BUGVERBOSE */

#define BUG() do {					\
	__EMIT_BUG(0);					\
	unreachable();					\
} while (0)

<<<<<<< HEAD
#define __WARN_TAINT(taint) do {			\
	__EMIT_BUG(BUGFLAG_TAINT(taint));		\
=======
#define __WARN_FLAGS(flags) do {			\
	__EMIT_BUG(BUGFLAG_WARNING|(flags));		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} while (0)

#define WARN_ON(x) ({					\
	int __ret_warn_on = !!(x);			\
	if (__builtin_constant_p(__ret_warn_on)) {	\
		if (__ret_warn_on)			\
			__WARN();			\
	} else {					\
		if (unlikely(__ret_warn_on))		\
			__WARN();			\
	}						\
	unlikely(__ret_warn_on);			\
})

#define HAVE_ARCH_BUG
#define HAVE_ARCH_WARN_ON
#endif /* CONFIG_BUG */

#include <asm-generic/bug.h>

#endif /* _ASM_S390_BUG_H */
