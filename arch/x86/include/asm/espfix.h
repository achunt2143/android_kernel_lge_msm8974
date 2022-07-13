<<<<<<< HEAD
#ifndef _ASM_X86_ESPFIX_H
#define _ASM_X86_ESPFIX_H

#ifdef CONFIG_X86_64
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_ESPFIX_H
#define _ASM_X86_ESPFIX_H

#ifdef CONFIG_X86_ESPFIX64
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/percpu.h>

DECLARE_PER_CPU_READ_MOSTLY(unsigned long, espfix_stack);
DECLARE_PER_CPU_READ_MOSTLY(unsigned long, espfix_waddr);

extern void init_espfix_bsp(void);
<<<<<<< HEAD
extern void init_espfix_ap(void);

#endif /* CONFIG_X86_64 */
=======
extern void init_espfix_ap(int cpu);
#else
static inline void init_espfix_ap(int cpu) { }
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_ESPFIX_H */
