<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* x86-specific clocksource additions */

#ifndef _ASM_X86_CLOCKSOURCE_H
#define _ASM_X86_CLOCKSOURCE_H

<<<<<<< HEAD
#ifdef CONFIG_X86_64

#define VCLOCK_NONE 0  /* No vDSO clock available.	*/
#define VCLOCK_TSC  1  /* vDSO should use vread_tsc.	*/
#define VCLOCK_HPET 2  /* vDSO should use vread_hpet.	*/

struct arch_clocksource_data {
	int vclock_mode;
};

#endif /* CONFIG_X86_64 */
=======
#include <asm/vdso/clocksource.h>

extern unsigned int vclocks_used;

static inline bool vclock_was_used(int vclock)
{
	return READ_ONCE(vclocks_used) & (1U << vclock);
}

static inline void vclocks_set_used(unsigned int which)
{
	WRITE_ONCE(vclocks_used, READ_ONCE(vclocks_used) | (1 << which));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_CLOCKSOURCE_H */
