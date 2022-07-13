<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* IEEE754 floating point arithmetic
 * single precision
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

ieee754sp ieee754sp_flong(s64 x)
=======
 */

#include "ieee754sp.h"

union ieee754sp ieee754sp_flong(s64 x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u64 xm;		/* <--- need 64-bit mantissa temp */
	int xe;
	int xs;

<<<<<<< HEAD
	CLEARCX;
=======
	ieee754_clearcx();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (x == 0)
		return ieee754sp_zero(0);
	if (x == 1 || x == -1)
		return ieee754sp_one(x < 0);
	if (x == 10 || x == -10)
		return ieee754sp_ten(x < 0);

	xs = (x < 0);
	if (xs) {
		if (x == (1ULL << 63))
			xm = (1ULL << 63);	/* max neg can't be safely negated */
		else
			xm = -x;
	} else {
		xm = x;
	}
<<<<<<< HEAD
	xe = SP_MBITS + 3;

	if (xm >> (SP_MBITS + 1 + 3)) {
		/* shunt out overflow bits
		 */
		while (xm >> (SP_MBITS + 1 + 3)) {
=======
	xe = SP_FBITS + 3;

	if (xm >> (SP_FBITS + 1 + 3)) {
		/* shunt out overflow bits
		 */
		while (xm >> (SP_FBITS + 1 + 3)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			SPXSRSX1();
		}
	} else {
		/* normalize in grs extended single precision */
<<<<<<< HEAD
		while ((xm >> (SP_MBITS + 3)) == 0) {
=======
		while ((xm >> (SP_FBITS + 3)) == 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			xm <<= 1;
			xe--;
		}
	}
<<<<<<< HEAD
	SPNORMRET1(xs, xe, xm, "sp_flong", x);
}


ieee754sp ieee754sp_fulong(u64 u)
{
	if ((s64) u < 0)
		return ieee754sp_add(ieee754sp_1e63(),
				     ieee754sp_flong(u & ~(1ULL << 63)));
	return ieee754sp_flong(u);
=======
	return ieee754sp_format(xs, xe, xm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
