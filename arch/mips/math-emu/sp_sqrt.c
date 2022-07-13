<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* IEEE754 floating point arithmetic
 * single precision square root
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


#include "ieee754sp.h"

ieee754sp ieee754sp_sqrt(ieee754sp x)
=======
 */

#include "ieee754sp.h"

union ieee754sp ieee754sp_sqrt(union ieee754sp x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ix, s, q, m, t, i;
	unsigned int r;
	COMPXSP;

	/* take care of Inf and NaN */

	EXPLODEXSP;
<<<<<<< HEAD
	CLEARCX;
=======
	ieee754_clearcx();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	FLUSHXSP;

	/* x == INF or NAN? */
	switch (xc) {
<<<<<<< HEAD
	case IEEE754_CLASS_QNAN:
		/* sqrt(Nan) = Nan */
		return ieee754sp_nanxcpt(x, "sqrt");
	case IEEE754_CLASS_SNAN:
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_nanxcpt(ieee754sp_indef(), "sqrt");
	case IEEE754_CLASS_ZERO:
		/* sqrt(0) = 0 */
		return x;
	case IEEE754_CLASS_INF:
		if (xs) {
			/* sqrt(-Inf) = Nan */
			SETCX(IEEE754_INVALID_OPERATION);
			return ieee754sp_nanxcpt(ieee754sp_indef(), "sqrt");
		}
		/* sqrt(+Inf) = Inf */
		return x;
=======
	case IEEE754_CLASS_SNAN:
		return ieee754sp_nanxcpt(x);

	case IEEE754_CLASS_QNAN:
		/* sqrt(Nan) = Nan */
		return x;

	case IEEE754_CLASS_ZERO:
		/* sqrt(0) = 0 */
		return x;

	case IEEE754_CLASS_INF:
		if (xs) {
			/* sqrt(-Inf) = Nan */
			ieee754_setcx(IEEE754_INVALID_OPERATION);
			return ieee754sp_indef();
		}
		/* sqrt(+Inf) = Inf */
		return x;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IEEE754_CLASS_DNORM:
	case IEEE754_CLASS_NORM:
		if (xs) {
			/* sqrt(-x) = Nan */
<<<<<<< HEAD
			SETCX(IEEE754_INVALID_OPERATION);
			return ieee754sp_nanxcpt(ieee754sp_indef(), "sqrt");
=======
			ieee754_setcx(IEEE754_INVALID_OPERATION);
			return ieee754sp_indef();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;
	}

	ix = x.bits;

	/* normalize x */
	m = (ix >> 23);
	if (m == 0) {		/* subnormal x */
		for (i = 0; (ix & 0x00800000) == 0; i++)
			ix <<= 1;
		m -= i - 1;
	}
	m -= 127;		/* unbias exponent */
	ix = (ix & 0x007fffff) | 0x00800000;
	if (m & 1)		/* odd m, double x to make it even */
		ix += ix;
	m >>= 1;		/* m = [m/2] */

	/* generate sqrt(x) bit by bit */
	ix += ix;
<<<<<<< HEAD
	q = s = 0;		/* q = sqrt(x) */
=======
	s = 0;
	q = 0;			/* q = sqrt(x) */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	r = 0x01000000;		/* r = moving bit from right to left */

	while (r != 0) {
		t = s + r;
		if (t <= ix) {
			s = t + r;
			ix -= t;
			q += r;
		}
		ix += ix;
		r >>= 1;
	}

	if (ix != 0) {
<<<<<<< HEAD
		SETCX(IEEE754_INEXACT);
		switch (ieee754_csr.rm) {
		case IEEE754_RP:
			q += 2;
			break;
		case IEEE754_RN:
=======
		ieee754_setcx(IEEE754_INEXACT);
		switch (ieee754_csr.rm) {
		case FPU_CSR_RU:
			q += 2;
			break;
		case FPU_CSR_RN:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			q += (q & 1);
			break;
		}
	}
	ix = (q >> 1) + 0x3f000000;
	ix += (m << 23);
	x.bits = ix;
	return x;
}
