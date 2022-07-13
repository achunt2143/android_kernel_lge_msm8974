<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* GENcopy_to_user.S: Generic sparc64 copy to userspace.
 *
 * Copyright (C) 2007 David S. Miller (davem@davemloft.net)
 */

<<<<<<< HEAD
#define EX_ST(x)		\
98:	x;			\
	.section __ex_table,"a";\
	.align 4;		\
	.word 98b, __retl_one;	\
=======
#define EX_ST(x,y)		\
98:	x;			\
	.section __ex_table,"a";\
	.align 4;		\
	.word 98b, y;		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.text;			\
	.align 4;

#ifndef ASI_AIUS
#define ASI_AIUS	0x11
#endif

#define FUNC_NAME		GENcopy_to_user
#define STORE(type,src,addr)	type##a src, [addr] ASI_AIUS
#define EX_RETVAL(x)		0

#ifdef __KERNEL__
	/* Writing to %asi is _expensive_ so we hardcode it.
	 * Reading %asi to check for KERNEL_DS is comparatively
	 * cheap.
	 */
#define PREAMBLE					\
	rd		%asi, %g1;			\
	cmp		%g1, ASI_AIUS;			\
<<<<<<< HEAD
	bne,pn		%icc, ___copy_in_user;		\
=======
	bne,pn		%icc, raw_copy_in_user;		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 nop
#endif

#include "GENmemcpy.S"
