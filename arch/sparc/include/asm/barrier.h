<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef ___ASM_SPARC_BARRIER_H
#define ___ASM_SPARC_BARRIER_H
#if defined(__sparc__) && defined(__arch64__)
#include <asm/barrier_64.h>
#else
#include <asm/barrier_32.h>
#endif
#endif
