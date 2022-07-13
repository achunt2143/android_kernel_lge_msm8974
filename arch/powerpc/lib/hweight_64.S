<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Copyright (C) IBM Corporation, 2010
 *
 * Author: Anton Blanchard <anton@au.ibm.com>
 */
<<<<<<< HEAD
#include <asm/processor.h>
#include <asm/ppc_asm.h>
=======
#include <linux/export.h>
#include <asm/processor.h>
#include <asm/ppc_asm.h>
#include <asm/feature-fixups.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Note: This code relies on -mminimal-toc */

_GLOBAL(__arch_hweight8)
BEGIN_FTR_SECTION
<<<<<<< HEAD
	b .__sw_hweight8
	nop
	nop
FTR_SECTION_ELSE
	PPC_POPCNTB(r3,r3)
	clrldi	r3,r3,64-8
	blr
ALT_FTR_SECTION_END_IFCLR(CPU_FTR_POPCNTB)

_GLOBAL(__arch_hweight16)
BEGIN_FTR_SECTION
	b .__sw_hweight16
=======
	b CFUNC(__sw_hweight8)
	nop
	nop
FTR_SECTION_ELSE
	PPC_POPCNTB(R3,R3)
	clrldi	r3,r3,64-8
	blr
ALT_FTR_SECTION_END_IFCLR(CPU_FTR_POPCNTB)
EXPORT_SYMBOL(__arch_hweight8)

_GLOBAL(__arch_hweight16)
BEGIN_FTR_SECTION
	b CFUNC(__sw_hweight16)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nop
	nop
	nop
	nop
FTR_SECTION_ELSE
  BEGIN_FTR_SECTION_NESTED(50)
<<<<<<< HEAD
	PPC_POPCNTB(r3,r3)
=======
	PPC_POPCNTB(R3,R3)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	srdi	r4,r3,8
	add	r3,r4,r3
	clrldi	r3,r3,64-8
	blr
  FTR_SECTION_ELSE_NESTED(50)
	clrlwi  r3,r3,16
<<<<<<< HEAD
	PPC_POPCNTW(r3,r3)
=======
	PPC_POPCNTW(R3,R3)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clrldi	r3,r3,64-8
	blr
  ALT_FTR_SECTION_END_NESTED_IFCLR(CPU_FTR_POPCNTD, 50)
ALT_FTR_SECTION_END_IFCLR(CPU_FTR_POPCNTB)
<<<<<<< HEAD

_GLOBAL(__arch_hweight32)
BEGIN_FTR_SECTION
	b .__sw_hweight32
=======
EXPORT_SYMBOL(__arch_hweight16)

_GLOBAL(__arch_hweight32)
BEGIN_FTR_SECTION
	b CFUNC(__sw_hweight32)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nop
	nop
	nop
	nop
	nop
	nop
FTR_SECTION_ELSE
  BEGIN_FTR_SECTION_NESTED(51)
<<<<<<< HEAD
	PPC_POPCNTB(r3,r3)
=======
	PPC_POPCNTB(R3,R3)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	srdi	r4,r3,16
	add	r3,r4,r3
	srdi	r4,r3,8
	add	r3,r4,r3
	clrldi	r3,r3,64-8
	blr
  FTR_SECTION_ELSE_NESTED(51)
<<<<<<< HEAD
	PPC_POPCNTW(r3,r3)
=======
	PPC_POPCNTW(R3,R3)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clrldi	r3,r3,64-8
	blr
  ALT_FTR_SECTION_END_NESTED_IFCLR(CPU_FTR_POPCNTD, 51)
ALT_FTR_SECTION_END_IFCLR(CPU_FTR_POPCNTB)
<<<<<<< HEAD

_GLOBAL(__arch_hweight64)
BEGIN_FTR_SECTION
	b .__sw_hweight64
=======
EXPORT_SYMBOL(__arch_hweight32)

_GLOBAL(__arch_hweight64)
BEGIN_FTR_SECTION
	b CFUNC(__sw_hweight64)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
FTR_SECTION_ELSE
  BEGIN_FTR_SECTION_NESTED(52)
<<<<<<< HEAD
	PPC_POPCNTB(r3,r3)
=======
	PPC_POPCNTB(R3,R3)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	srdi	r4,r3,32
	add	r3,r4,r3
	srdi	r4,r3,16
	add	r3,r4,r3
	srdi	r4,r3,8
	add	r3,r4,r3
	clrldi	r3,r3,64-8
	blr
  FTR_SECTION_ELSE_NESTED(52)
<<<<<<< HEAD
	PPC_POPCNTD(r3,r3)
=======
	PPC_POPCNTD(R3,R3)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clrldi	r3,r3,64-8
	blr
  ALT_FTR_SECTION_END_NESTED_IFCLR(CPU_FTR_POPCNTD, 52)
ALT_FTR_SECTION_END_IFCLR(CPU_FTR_POPCNTB)
<<<<<<< HEAD
=======
EXPORT_SYMBOL(__arch_hweight64)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
