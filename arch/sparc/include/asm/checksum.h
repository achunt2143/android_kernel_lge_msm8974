<<<<<<< HEAD
#ifndef ___ASM_SPARC_CHECKSUM_H
#define ___ASM_SPARC_CHECKSUM_H
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ___ASM_SPARC_CHECKSUM_H
#define ___ASM_SPARC_CHECKSUM_H
#define _HAVE_ARCH_CSUM_AND_COPY
#define _HAVE_ARCH_COPY_AND_CSUM_FROM_USER
#define HAVE_CSUM_COPY_USER
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if defined(__sparc__) && defined(__arch64__)
#include <asm/checksum_64.h>
#else
#include <asm/checksum_32.h>
#endif
#endif
