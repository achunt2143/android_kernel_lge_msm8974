<<<<<<< HEAD
#ifndef _MIPS_SPRAM_H
#define _MIPS_SPRAM_H

#ifdef CONFIG_CPU_MIPSR2
extern __init void spram_config(void);
#else
static inline void spram_config(void) { };
#endif /* CONFIG_CPU_MIPSR2 */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _MIPS_SPRAM_H
#define _MIPS_SPRAM_H

#if defined(CONFIG_MIPS_SPRAM)
extern void spram_config(void);
#else
static inline void spram_config(void) { }
#endif /* CONFIG_MIPS_SPRAM */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _MIPS_SPRAM_H */
