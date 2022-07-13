<<<<<<< HEAD
#ifndef _ASM_S390_MODULE_H
#define _ASM_S390_MODULE_H
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_S390_MODULE_H
#define _ASM_S390_MODULE_H

#include <asm-generic/module.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This file contains the s390 architecture specific module code.
 */

<<<<<<< HEAD
struct mod_arch_syminfo
{
=======
struct mod_arch_syminfo {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long got_offset;
	unsigned long plt_offset;
	int got_initialized;
	int plt_initialized;
};

<<<<<<< HEAD
struct mod_arch_specific
{
=======
struct mod_arch_specific {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Starting offset of got in the module core memory. */
	unsigned long got_offset;
	/* Starting offset of plt in the module core memory. */
	unsigned long plt_offset;
	/* Size of the got. */
	unsigned long got_size;
	/* Size of the plt. */
	unsigned long plt_size;
	/* Number of symbols in syminfo. */
	int nsyms;
	/* Additional symbol information (got and plt offsets). */
	struct mod_arch_syminfo *syminfo;
<<<<<<< HEAD
};

#ifdef __s390x__
#define ElfW(x) Elf64_ ## x
#define ELFW(x) ELF64_ ## x
#else
#define ElfW(x) Elf32_ ## x
#define ELFW(x) ELF32_ ## x
#endif

#define Elf_Addr ElfW(Addr)
#define Elf_Rela ElfW(Rela)
#define Elf_Shdr ElfW(Shdr)
#define Elf_Sym ElfW(Sym)
#define Elf_Ehdr ElfW(Ehdr)
#define ELF_R_SYM ELFW(R_SYM)
#define ELF_R_TYPE ELFW(R_TYPE)
=======
#ifdef CONFIG_FUNCTION_TRACER
	/* Start of memory reserved for ftrace hotpatch trampolines. */
	struct ftrace_hotpatch_trampoline *trampolines_start;
	/* End of memory reserved for ftrace hotpatch trampolines. */
	struct ftrace_hotpatch_trampoline *trampolines_end;
	/* Next unused ftrace hotpatch trampoline slot. */
	struct ftrace_hotpatch_trampoline *next_trampoline;
#endif /* CONFIG_FUNCTION_TRACER */
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_S390_MODULE_H */
