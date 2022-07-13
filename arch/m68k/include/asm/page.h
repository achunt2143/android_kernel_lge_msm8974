<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _M68K_PAGE_H
#define _M68K_PAGE_H

#include <linux/const.h>
#include <asm/setup.h>
#include <asm/page_offset.h>

/* PAGE_SHIFT determines the page size */
<<<<<<< HEAD
#if defined(CONFIG_SUN3) || defined(CONFIG_COLDFIRE)
#define PAGE_SHIFT	13
#else
#define PAGE_SHIFT	12
#endif
=======
#define PAGE_SHIFT	CONFIG_PAGE_SHIFT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PAGE_SIZE	(_AC(1, UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))
#define PAGE_OFFSET	(PAGE_OFFSET_RAW)

#ifndef __ASSEMBLY__

/*
 * These are used to make use of C type-checking..
 */
<<<<<<< HEAD
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long pmd[16]; } pmd_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;
typedef struct page *pgtable_t;

#define pte_val(x)	((x).pte)
#define pmd_val(x)	((&x)->pmd[0])
=======
#if !defined(CONFIG_MMU) || CONFIG_PGTABLE_LEVELS == 3
typedef struct { unsigned long pmd; } pmd_t;
#define pmd_val(x)	((&x)->pmd)
#define __pmd(x)	((pmd_t) { (x) } )
#endif

typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;

#if defined(CONFIG_SUN3)
/*
 * Sun3 still uses the asm-generic/pgalloc.h code and thus needs this
 * definition. It would be possible to unify Sun3 and ColdFire pgalloc and have
 * all of m68k use the same type.
 */
typedef struct page *pgtable_t;
#else
typedef pte_t *pgtable_t;
#endif

#define pte_val(x)	((x).pte)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x)	((pte_t) { (x) } )
<<<<<<< HEAD
#define __pmd(x)	((pmd_t) { (x) } )
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define __pgd(x)	((pgd_t) { (x) } )
#define __pgprot(x)	((pgprot_t) { (x) } )

extern unsigned long _rambase;
extern unsigned long _ramstart;
extern unsigned long _ramend;

#endif /* !__ASSEMBLY__ */

#ifdef CONFIG_MMU
<<<<<<< HEAD
#include "page_mm.h"
#else
#include "page_no.h"
#endif

#include <asm-generic/getorder.h>
=======
#include <asm/page_mm.h>
#else
#include <asm/page_no.h>
#endif

#include <asm-generic/getorder.h>
#include <asm-generic/memory_model.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _M68K_PAGE_H */
