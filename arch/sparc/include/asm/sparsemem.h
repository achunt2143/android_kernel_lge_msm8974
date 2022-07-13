<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _SPARC64_SPARSEMEM_H
#define _SPARC64_SPARSEMEM_H

#ifdef __KERNEL__

<<<<<<< HEAD
#define SECTION_SIZE_BITS       30
#define MAX_PHYSADDR_BITS       42
#define MAX_PHYSMEM_BITS        42
=======
#include <asm/page.h>

#define SECTION_SIZE_BITS       30
#define MAX_PHYSMEM_BITS        MAX_PHYS_ADDRESS_BITS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* !(__KERNEL__) */

#endif /* !(_SPARC64_SPARSEMEM_H) */
