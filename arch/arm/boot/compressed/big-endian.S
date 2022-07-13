<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/boot/compressed/big-endian.S
 *
 *  Switch CPU into big endian mode.
 *  Author: Nicolas Pitre
 */

<<<<<<< HEAD
	.section ".start", #alloc, #execinstr
=======
	.section ".start", "ax"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mrc	p15, 0, r0, c1, c0, 0	@ read control reg
	orr	r0, r0, #(1 << 7)	@ enable big endian mode
	mcr	p15, 0, r0, c1, c0, 0	@ write control reg

