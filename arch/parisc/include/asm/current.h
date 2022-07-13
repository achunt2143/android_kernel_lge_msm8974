<<<<<<< HEAD
#ifndef _PARISC_CURRENT_H
#define _PARISC_CURRENT_H

#include <linux/thread_info.h>

struct task_struct;

static inline struct task_struct * get_current(void)
{
	return current_thread_info()->task;
}
 
#define current get_current()

#endif /* !(_PARISC_CURRENT_H) */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_PARISC_CURRENT_H
#define _ASM_PARISC_CURRENT_H

#ifndef __ASSEMBLY__
struct task_struct;

static __always_inline struct task_struct *get_current(void)
{
	struct task_struct *ts;

	/* do not use mfctl() macro as it is marked volatile */
	asm( "mfctl %%cr30,%0" : "=r" (ts) );
	return ts;
}

#define current get_current()

#endif /* __ASSEMBLY__ */

#endif /* _ASM_PARISC_CURRENT_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
