<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* delay.h: Linux delay routines on sparc64.
 *
 * Copyright (C) 1996, 2004, 2007 David S. Miller (davem@davemloft.net).
 */

#ifndef _SPARC64_DELAY_H
#define _SPARC64_DELAY_H

#ifndef __ASSEMBLY__

<<<<<<< HEAD
extern void __delay(unsigned long loops);
extern void udelay(unsigned long usecs);
=======
void __delay(unsigned long loops);
void udelay(unsigned long usecs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define mdelay(n)	udelay((n) * 1000)

#endif /* !__ASSEMBLY__ */

#endif /* _SPARC64_DELAY_H */
