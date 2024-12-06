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

int ieee754sp_cmp(ieee754sp x, ieee754sp y, int cmp, int sig)
{
=======
 */

#include "ieee754sp.h"

int ieee754sp_cmp(union ieee754sp x, union ieee754sp y, int cmp, int sig)
{
	int vx;
	int vy;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	COMPXSP;
	COMPYSP;

	EXPLODEXSP;
	EXPLODEYSP;
	FLUSHXSP;
	FLUSHYSP;
<<<<<<< HEAD
	CLEARCX;	/* Even clear inexact flag here */

	if (ieee754sp_isnan(x) || ieee754sp_isnan(y)) {
		if (sig || xc == IEEE754_CLASS_SNAN || yc == IEEE754_CLASS_SNAN)
			SETCX(IEEE754_INVALID_OPERATION);
		if (cmp & IEEE754_CUN)
			return 1;
		if (cmp & (IEEE754_CLT | IEEE754_CGT)) {
			if (sig && SETANDTESTCX(IEEE754_INVALID_OPERATION))
				return ieee754si_xcpt(0, "fcmpf", x);
		}
		return 0;
	} else {
		int vx = x.bits;
		int vy = y.bits;
=======
	ieee754_clearcx();	/* Even clear inexact flag here */

	if (ieee754_class_nan(xc) || ieee754_class_nan(yc)) {
		if (sig ||
		    xc == IEEE754_CLASS_SNAN || yc == IEEE754_CLASS_SNAN)
			ieee754_setcx(IEEE754_INVALID_OPERATION);
		return (cmp & IEEE754_CUN) != 0;
	} else {
		vx = x.bits;
		vy = y.bits;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (vx < 0)
			vx = -vx ^ SP_SIGN_BIT;
		if (vy < 0)
			vy = -vy ^ SP_SIGN_BIT;

		if (vx < vy)
			return (cmp & IEEE754_CLT) != 0;
		else if (vx == vy)
			return (cmp & IEEE754_CEQ) != 0;
		else
			return (cmp & IEEE754_CGT) != 0;
	}
}
