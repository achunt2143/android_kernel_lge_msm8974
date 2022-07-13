<<<<<<< HEAD
#include <linux/linkage.h>
#include <asm/assembler.h>
#include <mach/hardware.h>
#include <mach/entry-macro.S>

	.text

=======
/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/linkage.h>
#include <asm/assembler.h>
#include <mach/hardware.h>

	.equ	ioc_base_high, IOC_BASE & 0xff000000
	.equ	ioc_base_low, IOC_BASE & 0x00ff0000

	.text
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.global	rpc_default_fiq_end
ENTRY(rpc_default_fiq_start)
	mov	r12, #ioc_base_high
	.if	ioc_base_low
	orr	r12, r12, #ioc_base_low
	.endif
	strb	r12, [r12, #0x38]	@ Disable FIQ register
	subs	pc, lr, #4
rpc_default_fiq_end:
