<<<<<<< HEAD

#include <asm/bootparam.h>


/* ResetVector
 */
	.section        .ResetVector.text, "ax"
	.global         _ResetVector
_ResetVector:
	_j reset
	.align 4
RomInitAddr:
	.word 0xd0001000
RomBootParam:
	.word _bootparam
reset:
	l32r    a0, RomInitAddr
	l32r	a2, RomBootParam
	movi	a3, 0
	movi	a4, 0
	jx      a0

	.align 4
	.section .bootstrap.data, "aw"

	.globl _bootparam
=======
/*
 * arch/xtensa/boot/boot-elf/bootstrap.S
 *
 * Low-level exception handling
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2004 - 2013 by Tensilica Inc.
 *
 * Chris Zankel <chris@zankel.net>
 * Marc Gauthier <marc@tensilica.com>
 * Piet Delaney <piet@tensilica.com>
 */

#include <asm/bootparam.h>
#include <asm/initialize_mmu.h>
#include <asm/vectors.h>
#include <linux/linkage.h>

	.section	.ResetVector.text, "ax"
	.global         _ResetVector
	.global         reset

_ResetVector:
	_j _SetupMMU

	.begin  no-absolute-literals
	.literal_position

#ifdef CONFIG_PARSE_BOOTPARAM
	.align 4
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
_bootparam:
	.short	BP_TAG_FIRST
	.short	4
	.long	BP_VERSION
	.short	BP_TAG_LAST
	.short	0
	.long	0
<<<<<<< HEAD
=======
#endif

	.align  4
_SetupMMU:
#if XCHAL_HAVE_WINDOWED
	movi	a0, 0
	wsr	a0, windowbase
	rsync
	movi	a0, 1
	wsr	a0, windowstart
	rsync
#endif
	movi	a0, 0x1F
	wsr	a0, ps
	rsync

#ifndef CONFIG_INITIALIZE_XTENSA_MMU_INSIDE_VMLINUX
	initialize_mmu
#endif

	rsil    a0, XCHAL_DEBUGLEVEL-1
	rsync
reset:
#if defined(CONFIG_INITIALIZE_XTENSA_MMU_INSIDE_VMLINUX) && \
	XCHAL_HAVE_PTP_MMU && XCHAL_HAVE_SPANNING_WAY
	movi	a0, CONFIG_KERNEL_LOAD_ADDRESS
#else
	movi	a0, KERNELOFFSET
#endif
#ifdef CONFIG_PARSE_BOOTPARAM
	movi	a2, _bootparam
#else
	movi	a2, 0
#endif
	movi	a3, 0
	movi	a4, 0
	jx      a0

	.end    no-absolute-literals
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
