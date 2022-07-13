<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.globl	kernel_start
kernel_start:
	.incbin	"arch/arm/boot/zImage"
	.globl	kernel_end
kernel_end:
	.align	2
