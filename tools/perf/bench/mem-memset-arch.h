<<<<<<< HEAD

#ifdef ARCH_X86_64

#define MEMSET_FN(fn, name, desc)		\
	extern void *fn(void *, int, size_t);
=======
/* SPDX-License-Identifier: GPL-2.0 */

#ifdef HAVE_ARCH_X86_64_SUPPORT

#define MEMSET_FN(fn, name, desc)		\
	void *fn(void *, int, size_t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "mem-memset-x86-64-asm-def.h"

#undef MEMSET_FN

#endif

