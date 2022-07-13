<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _SPARC_BUG_H
#define _SPARC_BUG_H

#ifdef CONFIG_BUG
#include <linux/compiler.h>

#ifdef CONFIG_DEBUG_BUGVERBOSE
<<<<<<< HEAD
extern void do_BUG(const char *file, int line);
#define BUG() do {					\
	do_BUG(__FILE__, __LINE__);			\
	__builtin_trap();				\
} while (0)
#else
#define BUG()		__builtin_trap()
=======
void do_BUG(const char *file, int line);
#define BUG() do {					\
	do_BUG(__FILE__, __LINE__);			\
	barrier_before_unreachable();			\
	__builtin_trap();				\
} while (0)
#else
#define BUG() do {					\
	barrier_before_unreachable();			\
	__builtin_trap();				\
} while (0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#define HAVE_ARCH_BUG
#endif

#include <asm-generic/bug.h>

struct pt_regs;
<<<<<<< HEAD
extern void die_if_kernel(char *str, struct pt_regs *regs) __attribute__ ((noreturn));
=======
void __noreturn die_if_kernel(char *str, struct pt_regs *regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
