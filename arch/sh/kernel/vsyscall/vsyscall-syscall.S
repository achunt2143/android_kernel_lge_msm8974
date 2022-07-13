<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>

__INITDATA

	.globl vsyscall_trapa_start, vsyscall_trapa_end
vsyscall_trapa_start:
	.incbin "arch/sh/kernel/vsyscall/vsyscall-trapa.so"
vsyscall_trapa_end:

__FINIT
