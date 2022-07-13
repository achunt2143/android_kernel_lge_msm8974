<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* NGcopy_from_user.S: Niagara optimized copy from userspace.
 *
 * Copyright (C) 2006, 2007 David S. Miller (davem@davemloft.net)
 */

<<<<<<< HEAD
#define EX_LD(x)		\
98:	x;			\
	.section __ex_table,"a";\
	.align 4;		\
	.word 98b, __ret_one_asi;\
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

#define FUNC_NAME		NGcopy_from_user
#define LOAD(type,addr,dest)	type##a [addr] ASI_AIUS, dest
#define LOAD_TWIN(addr_reg,dest0,dest1)	\
	ldda [addr_reg] ASI_BLK_INIT_QUAD_LDD_AIUS, dest0
#define EX_RETVAL(x)		%g0

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

#include "NGmemcpy.S"
