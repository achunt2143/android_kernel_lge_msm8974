<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* U3copy_to_user.S: UltraSparc-III optimized copy to userspace.
 *
 * Copyright (C) 1999, 2000, 2004 David S. Miller (davem@redhat.com)
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

#define FUNC_NAME		U3copy_to_user
#define STORE(type,src,addr)	type##a src, [addr] ASI_AIUS
#define STORE_BLK(src,addr)	stda src, [addr] ASI_BLK_AIUS
#define EX_RETVAL(x)		0

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
	 nop;						\

#include "U3memcpy.S"
