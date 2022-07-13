<<<<<<< HEAD
#ifdef __uClinux__
#include "pgtable_no.h"
#else
#include "pgtable_mm.h"
#endif
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __M68K_PGTABLE_H
#define __M68K_PGTABLE_H

#ifdef __uClinux__
#include <asm/pgtable_no.h>
#else
#include <asm/pgtable_mm.h>
#endif

#ifndef __ASSEMBLY__
extern void paging_init(void);
#endif

#endif /* __M68K_PGTABLE_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
