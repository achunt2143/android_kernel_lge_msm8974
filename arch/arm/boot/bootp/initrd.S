<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.type	initrd_start,#object
	.globl	initrd_start
initrd_start:
	.incbin	INITRD
	.globl	initrd_end
initrd_end:
