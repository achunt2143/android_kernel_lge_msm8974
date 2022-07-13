<<<<<<< HEAD
#ifndef _PARISC_SECTIONS_H
#define _PARISC_SECTIONS_H

/* nothing to see, move along */
#include <asm-generic/sections.h>

#ifdef CONFIG_64BIT
#undef dereference_function_descriptor
void *dereference_function_descriptor(void *);
#endif
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PARISC_SECTIONS_H
#define _PARISC_SECTIONS_H

#ifdef CONFIG_HAVE_FUNCTION_DESCRIPTORS
#include <asm/elf.h>
typedef Elf64_Fdesc func_desc_t;
#endif

/* nothing to see, move along */
#include <asm-generic/sections.h>

extern char __alt_instructions[], __alt_instructions_end[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
