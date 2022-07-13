<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_POWERPC_MODULE_H
#define _ASM_POWERPC_MODULE_H
#ifdef __KERNEL__

<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/list.h>
#include <asm/bug.h>

=======
#include <linux/list.h>
#include <asm/bug.h>
#include <asm-generic/module.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef __powerpc64__
/*
 * Thanks to Paul M for explaining this.
 *
 * PPC can only do rel jumps += 32MB, and often the kernel and other
<<<<<<< HEAD
 * modules are furthur away than this.  So, we jump to a table of
=======
 * modules are further away than this.  So, we jump to a table of
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * trampolines attached to the module (the Procedure Linkage Table)
 * whenever that happens.
 */

struct ppc_plt_entry {
	/* 16 byte jump instruction sequence (4 instructions) */
	unsigned int jump[4];
};
#endif	/* __powerpc64__ */


struct mod_arch_specific {
#ifdef __powerpc64__
	unsigned int stubs_section;	/* Index of stubs section in module */
<<<<<<< HEAD
	unsigned int toc_section;	/* What section is the TOC? */
#ifdef CONFIG_DYNAMIC_FTRACE
	unsigned long toc;
	unsigned long tramp;
#endif

=======
#ifdef CONFIG_PPC_KERNEL_PCREL
	unsigned int got_section;	/* What section is the GOT? */
	unsigned int pcpu_section;	/* .data..percpu section */
#else
	unsigned int toc_section;	/* What section is the TOC? */
	bool toc_fixed;			/* Have we fixed up .TOC.? */
#endif

	/* For module function descriptor dereference */
	unsigned long start_opd;
	unsigned long end_opd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else /* powerpc64 */
	/* Indices of PLT sections within module. */
	unsigned int core_plt_section;
	unsigned int init_plt_section;
<<<<<<< HEAD
#ifdef CONFIG_DYNAMIC_FTRACE
	unsigned long tramp;
#endif
#endif /* powerpc64 */
=======
#endif /* powerpc64 */

#ifdef CONFIG_DYNAMIC_FTRACE
	unsigned long tramp;
	unsigned long tramp_regs;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* List of BUG addresses, source line numbers and filenames */
	struct list_head bug_list;
	struct bug_entry *bug_table;
	unsigned int num_bugs;
};

/*
 * Select ELF headers.
<<<<<<< HEAD
 * Make empty section for module_frob_arch_sections to expand.
 */

#ifdef __powerpc64__
#    define Elf_Shdr	Elf64_Shdr
#    define Elf_Sym	Elf64_Sym
#    define Elf_Ehdr	Elf64_Ehdr
#    ifdef MODULE
	asm(".section .stubs,\"ax\",@nobits; .align 3; .previous");
#    endif
#else
#    define Elf_Shdr	Elf32_Shdr
#    define Elf_Sym	Elf32_Sym
#    define Elf_Ehdr	Elf32_Ehdr
=======
 * Make empty sections for module_frob_arch_sections to expand.
 */

#ifdef __powerpc64__
#    ifdef MODULE
	asm(".section .stubs,\"ax\",@nobits; .align 3; .previous");
#        ifdef CONFIG_PPC_KERNEL_PCREL
	    asm(".section .mygot,\"a\",@nobits; .align 3; .previous");
#        endif
#    endif
#else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#    ifdef MODULE
	asm(".section .plt,\"ax\",@nobits; .align 3; .previous");
	asm(".section .init.plt,\"ax\",@nobits; .align 3; .previous");
#    endif	/* MODULE */
#endif

#ifdef CONFIG_DYNAMIC_FTRACE
<<<<<<< HEAD
#    ifdef MODULE
	asm(".section .ftrace.tramp,\"ax\",@nobits; .align 3; .previous");
#    endif	/* MODULE */
#endif


struct exception_table_entry;
void sort_ex_table(struct exception_table_entry *start,
		   struct exception_table_entry *finish);

#if defined(CONFIG_MODVERSIONS) && defined(CONFIG_PPC64)
#define ARCH_RELOCATES_KCRCTAB
#define reloc_start PHYSICAL_START
#endif
=======
int module_trampoline_target(struct module *mod, unsigned long trampoline,
			     unsigned long *target);
int module_finalize_ftrace(struct module *mod, const Elf_Shdr *sechdrs);
#else
static inline int module_finalize_ftrace(struct module *mod, const Elf_Shdr *sechdrs)
{
	return 0;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __KERNEL__ */
#endif	/* _ASM_POWERPC_MODULE_H */
