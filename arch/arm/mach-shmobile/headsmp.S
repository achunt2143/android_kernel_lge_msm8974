<<<<<<< HEAD
/*
=======
/* SPDX-License-Identifier: GPL-2.0
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * SMP support for R-Mobile / SH-Mobile
 *
 * Copyright (C) 2010  Magnus Damm
 * Copyright (C) 2010  Takashi Yoshii
 *
 * Based on vexpress, Copyright (c) 2003 ARM Limited, All Rights Reserved
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/memory.h>

	__CPUINIT

/* Cache invalidation nicked from arch/arm/mach-imx/head-v7.S, thanks!
 *
 * The secondary kernel init calls v7_flush_dcache_all before it enables
 * the L1; however, the L1 comes out of reset in an undefined state, so
 * the clean + invalidate performed by v7_flush_dcache_all causes a bunch
 * of cache lines with uninitialized data and uninitialized tags to get
 * written out to memory, which does really unpleasant things to the main
 * processor.  We fix this by performing an invalidate, rather than a
 * clean + invalidate, before jumping into the kernel.
 *
 * This funciton is cloned from arch/arm/mach-tegra/headsmp.S, and needs
 * to be called for both secondary cores startup and primary core resume
 * procedures.  Ideally, it should be moved into arch/arm/mm/cache-v7.S.
 */
ENTRY(v7_invalidate_l1)
	mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate I cache
	mcr	p15, 2, r0, c0, c0, 0
	mrc	p15, 1, r0, c0, c0, 0

	ldr	r1, =0x7fff
	and	r2, r1, r0, lsr #13

	ldr	r1, =0x3ff

	and	r3, r1, r0, lsr #3	@ NumWays - 1
	add	r2, r2, #1		@ NumSets

	and	r0, r0, #0x7
	add	r0, r0, #4	@ SetShift

	clz	r1, r3		@ WayShift
	add	r4, r3, #1	@ NumWays
1:	sub	r2, r2, #1	@ NumSets--
	mov	r3, r4		@ Temp = NumWays
2:	subs	r3, r3, #1	@ Temp--
	mov	r5, r3, lsl r1
	mov	r6, r2, lsl r0
	orr	r5, r5, r6	@ Reg = (Temp<<WayShift)|(NumSets<<SetShift)
	mcr	p15, 0, r5, c7, c6, 2
	bgt	2b
	cmp	r2, #0
	bgt	1b
	dsb
	isb
	mov	pc, lr
ENDPROC(v7_invalidate_l1)

ENTRY(shmobile_invalidate_start)
	bl	v7_invalidate_l1
	b	secondary_startup
ENDPROC(shmobile_invalidate_start)
=======
 */
#include <linux/init.h>
#include <linux/linkage.h>
#include <linux/threads.h>
#include <asm/assembler.h>
#include <asm/page.h>

#define SCTLR_MMU	0x01
#define BOOTROM_ADDRESS	0xE6340000
#define RWTCSRA_ADDRESS 0xE6020004
#define RWTCSRA_WOVF	0x10
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Reset vector for secondary CPUs.
 * This will be mapped at address 0 by SBAR register.
 * We need _long_ jump to the physical address.
 */
<<<<<<< HEAD
	.align  12
ENTRY(shmobile_secondary_vector)
	ldr     pc, 1f
1:	.long   shmobile_invalidate_start - PAGE_OFFSET + PLAT_PHYS_OFFSET
ENDPROC(shmobile_secondary_vector)
=======
	.arm
	.align  12
ENTRY(shmobile_boot_vector)
	ldr     r1, 1f
	bx	r1

ENDPROC(shmobile_boot_vector)

	.align	2
	.globl	shmobile_boot_fn
shmobile_boot_fn:
1:	.space	4
	.globl	shmobile_boot_size
shmobile_boot_size:
	.long	. - shmobile_boot_vector

#ifdef CONFIG_ARCH_RCAR_GEN2
/*
 * Reset vector for R-Car Gen2 and RZ/G1 secondary CPUs.
 * This will be mapped at address 0 by SBAR register.
 */
ENTRY(shmobile_boot_vector_gen2)
	mrc	p15, 0, r0, c0, c0, 5		@ r0 = MPIDR
	ldr	r1, shmobile_boot_cpu_gen2
	cmp	r0, r1
	bne	shmobile_smp_continue_gen2

	mrc	p15, 0, r1, c1, c0, 0		@ r1 = SCTLR
	and	r0, r1, #SCTLR_MMU
	cmp	r0, #SCTLR_MMU
	beq	shmobile_smp_continue_gen2

	ldr	r0, rwtcsra
	mov	r1, #0
	ldrb	r1, [r0]
	and	r0, r1, #RWTCSRA_WOVF
	cmp	r0, #RWTCSRA_WOVF
	bne	shmobile_smp_continue_gen2

	ldr	r0, bootrom
	bx	r0

shmobile_smp_continue_gen2:
	ldr     r1, shmobile_boot_fn_gen2
	bx	r1

ENDPROC(shmobile_boot_vector_gen2)

	.align	4
rwtcsra:
	.word	RWTCSRA_ADDRESS
bootrom:
	.word	BOOTROM_ADDRESS
	.globl	shmobile_boot_cpu_gen2
shmobile_boot_cpu_gen2:
	.word	0x00000000

	.align	2
	.globl	shmobile_boot_fn_gen2
shmobile_boot_fn_gen2:
	.space	4
	.globl	shmobile_boot_size_gen2
shmobile_boot_size_gen2:
	.long	. - shmobile_boot_vector_gen2
#endif /* CONFIG_ARCH_RCAR_GEN2 */

/*
 * Per-CPU SMP boot function/argument selection code based on MPIDR
 */

ENTRY(shmobile_smp_boot)
	mrc	p15, 0, r1, c0, c0, 5		@ r1 = MPIDR
	and	r0, r1, #0xffffff		@ MPIDR_HWID_BITMASK
						@ r0 = cpu_logical_map() value
	mov	r1, #0				@ r1 = CPU index
	adr	r2, 1f
	ldmia	r2, {r5, r6, r7}
	add	r5, r5, r2			@ array of per-cpu mpidr values
	add	r6, r6, r2			@ array of per-cpu functions
	add	r7, r7, r2			@ array of per-cpu arguments

shmobile_smp_boot_find_mpidr:
	ldr	r8, [r5, r1, lsl #2]
	cmp	r8, r0
	bne	shmobile_smp_boot_next

	ldr	r9, [r6, r1, lsl #2]
	cmp	r9, #0
	bne	shmobile_smp_boot_found

shmobile_smp_boot_next:
	add	r1, r1, #1
	cmp	r1, #NR_CPUS
	blo	shmobile_smp_boot_find_mpidr

	b	shmobile_smp_sleep

shmobile_smp_boot_found:
	ldr	r0, [r7, r1, lsl #2]
	ret	r9
ENDPROC(shmobile_smp_boot)

ENTRY(shmobile_smp_sleep)
	wfi
	b	shmobile_smp_boot
ENDPROC(shmobile_smp_sleep)

	.align	2
1:	.long	shmobile_smp_mpidr - .
	.long	shmobile_smp_fn - 1b
	.long	shmobile_smp_arg - 1b

	.bss
	.globl	shmobile_smp_mpidr
shmobile_smp_mpidr:
	.space	NR_CPUS * 4
	.globl	shmobile_smp_fn
shmobile_smp_fn:
	.space	NR_CPUS * 4
	.globl	shmobile_smp_arg
shmobile_smp_arg:
	.space	NR_CPUS * 4
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
