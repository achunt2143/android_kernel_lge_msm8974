<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* IEEE754 floating point arithmetic
 * double precision: common utilities
 */
/*
 * MIPS floating point support
 * Copyright (C) 1994-2000 Algorithmics Ltd.
<<<<<<< HEAD
 *
 * ########################################################################
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 */


#include "ieee754dp.h"

ieee754dp ieee754dp_fsp(ieee754sp x)
=======
 */

#include "ieee754sp.h"
#include "ieee754dp.h"

static inline union ieee754dp ieee754dp_nan_fsp(int xs, u64 xm)
{
	return builddp(xs, DP_EMAX + 1 + DP_EBIAS,
		       xm << (DP_FBITS - SP_FBITS));
}

union ieee754dp ieee754dp_fsp(union ieee754sp x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	COMPXSP;

	EXPLODEXSP;

<<<<<<< HEAD
	CLEARCX;
=======
	ieee754_clearcx();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	FLUSHXSP;

	switch (xc) {
	case IEEE754_CLASS_SNAN:
<<<<<<< HEAD
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754dp_nanxcpt(ieee754dp_indef(), "fsp");
	case IEEE754_CLASS_QNAN:
		return ieee754dp_nanxcpt(builddp(xs,
						 DP_EMAX + 1 + DP_EBIAS,
						 ((u64) xm
						  << (DP_MBITS -
						      SP_MBITS))), "fsp",
					 x);
	case IEEE754_CLASS_INF:
		return ieee754dp_inf(xs);
	case IEEE754_CLASS_ZERO:
		return ieee754dp_zero(xs);
	case IEEE754_CLASS_DNORM:
		/* normalize */
		while ((xm >> SP_MBITS) == 0) {
=======
		return ieee754dp_nanxcpt(ieee754dp_nan_fsp(xs, xm));

	case IEEE754_CLASS_QNAN:
		return ieee754dp_nan_fsp(xs, xm);

	case IEEE754_CLASS_INF:
		return ieee754dp_inf(xs);

	case IEEE754_CLASS_ZERO:
		return ieee754dp_zero(xs);

	case IEEE754_CLASS_DNORM:
		/* normalize */
		while ((xm >> SP_FBITS) == 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			xm <<= 1;
			xe--;
		}
		break;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IEEE754_CLASS_NORM:
		break;
	}

<<<<<<< HEAD
	/* CAN'T possibly overflow,underflow, or need rounding
=======
	/*
	 * Can't possibly overflow,underflow, or need rounding
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */

	/* drop the hidden bit */
	xm &= ~SP_HIDDEN_BIT;

	return builddp(xs, xe + DP_EBIAS,
<<<<<<< HEAD
		       (u64) xm << (DP_MBITS - SP_MBITS));
=======
		       (u64) xm << (DP_FBITS - SP_FBITS));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
