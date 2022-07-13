<<<<<<< HEAD
#ifndef ___ASM_SPARC_PAGE_H
#define ___ASM_SPARC_PAGE_H
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ___ASM_SPARC_PAGE_H
#define ___ASM_SPARC_PAGE_H

#define page_to_phys(page)	(page_to_pfn(page) << PAGE_SHIFT)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if defined(__sparc__) && defined(__arch64__)
#include <asm/page_64.h>
#else
#include <asm/page_32.h>
#endif
#endif
