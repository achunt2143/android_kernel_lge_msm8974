<<<<<<< HEAD
#ifndef ___ASM_SPARC_AUXIO_H
#define ___ASM_SPARC_AUXIO_H
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ___ASM_SPARC_AUXIO_H
#define ___ASM_SPARC_AUXIO_H

#ifndef __ASSEMBLY__

extern void __iomem *auxio_register;

#endif /* ifndef __ASSEMBLY__ */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if defined(__sparc__) && defined(__arch64__)
#include <asm/auxio_64.h>
#else
#include <asm/auxio_32.h>
#endif
#endif
