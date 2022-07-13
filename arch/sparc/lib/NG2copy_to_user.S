<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* NG2copy_to_user.S: Niagara-2 optimized copy to userspace.
 *
 * Copyright (C) 2007 David S. Miller (davem@davemloft.net)
 */

<<<<<<< HEAD
#define EX_ST(x)		\
98:	x;			\
	.section __ex_table,"a";\
	.align 4;		\
	.word 98b, __retl_one_asi;\
=======
#define EX_ST(x,y)		\
98:	x;			\
	.section __ex_table,"a";\
	.align 4;		\
	.word 98b, y;		\
	.text;			\
	.align 4;

#define EX_ST_FP(x,y)		\
98:	x;			\
	.section __ex_table,"a";\
	.align 4;		\
	.word 98b, y##_fp;	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.text;			\
	.align 4;

#ifndef ASI_AIUS
#define ASI_AIUS	0x11
#endif

#ifndef ASI_BLK_AIUS_4V
#define ASI_BLK_AIUS_4V	0x17
#endif

#ifndef ASI_BLK_INIT_QUAD_LDD_AIUS
#define ASI_BLK_INIT_QUAD_LDD_AIUS 0x23
#endif

#define FUNC_NAME		NG2copy_to_user
#define STORE(type,src,addr)	type##a src, [addr] ASI_AIUS
#define STORE_ASI		ASI_BLK_INIT_QUAD_LDD_AIUS
#define STORE_BLK(src,addr)	stda src, [addr] ASI_BLK_AIUS_4V
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

#include "NG2memcpy.S"
