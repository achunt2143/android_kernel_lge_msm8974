<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * "Indirect" DCR access
 *
 * Copyright (c) 2004 Eugene Surovegin <ebs@ebshome.net>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under  the terms of  the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the License, or (at your
 * option) any later version.
 */

=======
 */

#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/ppc_asm.h>
#include <asm/processor.h>
#include <asm/bug.h>

#define DCR_ACCESS_PROLOG(table) \
<<<<<<< HEAD
	cmpli	cr0,r3,1024;	 \
=======
	cmplwi	cr0,r3,1024;	 \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rlwinm  r3,r3,4,18,27;   \
	lis     r5,table@h;      \
	ori     r5,r5,table@l;   \
	add     r3,r3,r5;        \
	bge-	1f;		 \
	mtctr   r3;              \
	bctr;			 \
1:	trap;			 \
	EMIT_BUG_ENTRY 1b,__FILE__,__LINE__,0;	\
	blr

_GLOBAL(__mfdcr)
	DCR_ACCESS_PROLOG(__mfdcr_table)
<<<<<<< HEAD

_GLOBAL(__mtdcr)
	DCR_ACCESS_PROLOG(__mtdcr_table)
=======
EXPORT_SYMBOL(__mfdcr)

_GLOBAL(__mtdcr)
	DCR_ACCESS_PROLOG(__mtdcr_table)
EXPORT_SYMBOL(__mtdcr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

__mfdcr_table:
	mfdcr  r3,0; blr
__mtdcr_table:
	mtdcr  0,r4; blr

dcr     = 1
        .rept   1023
	mfdcr   r3,dcr; blr
	mtdcr   dcr,r4; blr
	dcr     = dcr + 1
	.endr
