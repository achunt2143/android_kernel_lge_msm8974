<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* ----------------------------------------------------------------------- *
 *
 *   Copyright (C) 1991, 1992 Linus Torvalds
 *   Copyright 2007 rPath, Inc. - All Rights Reserved
 *
<<<<<<< HEAD
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ----------------------------------------------------------------------- */

#include <linux/linkage.h>

/*
 * Memory copy routines
 */

<<<<<<< HEAD
	.code16gcc
	.text

GLOBAL(memcpy)
=======
	.code16
	.text

SYM_FUNC_START_NOALIGN(memcpy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pushw	%si
	pushw	%di
	movw	%ax, %di
	movw	%dx, %si
	pushw	%cx
	shrw	$2, %cx
	rep; movsl
	popw	%cx
	andw	$3, %cx
	rep; movsb
	popw	%di
	popw	%si
<<<<<<< HEAD
	ret
ENDPROC(memcpy)

GLOBAL(memset)
=======
	retl
SYM_FUNC_END(memcpy)

SYM_FUNC_START_NOALIGN(memset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pushw	%di
	movw	%ax, %di
	movzbl	%dl, %eax
	imull	$0x01010101,%eax
	pushw	%cx
	shrw	$2, %cx
	rep; stosl
	popw	%cx
	andw	$3, %cx
	rep; stosb
	popw	%di
<<<<<<< HEAD
	ret
ENDPROC(memset)

GLOBAL(copy_from_fs)
	pushw	%ds
	pushw	%fs
	popw	%ds
	call	memcpy
	popw	%ds
	ret
ENDPROC(copy_from_fs)

GLOBAL(copy_to_fs)
	pushw	%es
	pushw	%fs
	popw	%es
	call	memcpy
	popw	%es
	ret
ENDPROC(copy_to_fs)

#if 0 /* Not currently used, but can be enabled as needed */
GLOBAL(copy_from_gs)
	pushw	%ds
	pushw	%gs
	popw	%ds
	call	memcpy
	popw	%ds
	ret
ENDPROC(copy_from_gs)

GLOBAL(copy_to_gs)
	pushw	%es
	pushw	%gs
	popw	%es
	call	memcpy
	popw	%es
	ret
ENDPROC(copy_to_gs)
#endif
=======
	retl
SYM_FUNC_END(memset)

SYM_FUNC_START_NOALIGN(copy_from_fs)
	pushw	%ds
	pushw	%fs
	popw	%ds
	calll	memcpy
	popw	%ds
	retl
SYM_FUNC_END(copy_from_fs)

SYM_FUNC_START_NOALIGN(copy_to_fs)
	pushw	%es
	pushw	%fs
	popw	%es
	calll	memcpy
	popw	%es
	retl
SYM_FUNC_END(copy_to_fs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
