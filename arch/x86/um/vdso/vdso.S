<<<<<<< HEAD
#include <linux/init.h>

__INITDATA

	.globl vdso_start, vdso_end
vdso_start:
	.incbin "arch/x86/um/vdso/vdso.so"
vdso_end:
=======
/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/init.h>
#include <linux/linkage.h>

__INITDATA

SYM_DATA_START(vdso_start)
	.incbin "arch/x86/um/vdso/vdso.so"
SYM_DATA_END_LABEL(vdso_start, SYM_L_GLOBAL, vdso_end)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

__FINIT
