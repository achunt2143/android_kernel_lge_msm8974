<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* GENcopy_from_user.S: Generic sparc64 copy from userspace.
 *
 * Copyright (C) 2007 David S. Miller (davem@davemloft.net)
 */

<<<<<<< HEAD
#define EX_LD(x)		\
98:	x;			\
	.section __ex_table,"a";\
	.align 4;		\
	.word 98b, __retl_one;	\
=======
#define EX_LD(x,y)		\
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

#define FUNC_NAME		GENcopy_from_user
#define LOAD(type,addr,dest)	type##a [addr] ASI_AIUS, dest
#define EX_RETVAL(x)		0

#ifdef __KERNEL__
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
