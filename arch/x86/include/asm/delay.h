<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_DELAY_H
#define _ASM_X86_DELAY_H

#include <asm-generic/delay.h>
<<<<<<< HEAD

void use_tsc_delay(void);
=======
#include <linux/init.h>

void __init use_tsc_delay(void);
void __init use_tpause_delay(void);
void use_mwaitx_delay(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_DELAY_H */
