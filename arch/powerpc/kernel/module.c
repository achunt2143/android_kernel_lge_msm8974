<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*  Kernel module help for powerpc.
    Copyright (C) 2001, 2003 Rusty Russell IBM Corporation.
    Copyright (C) 2008 Freescale Semiconductor, Inc.

<<<<<<< HEAD
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
*/
#include <linux/elf.h>
#include <linux/moduleloader.h>
#include <linux/err.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD
#include <linux/bug.h>
#include <asm/module.h>
#include <asm/uaccess.h>
#include <asm/firmware.h>
#include <linux/sort.h>

#include "setup.h"

LIST_HEAD(module_bug_list);
=======
#include <linux/mm.h>
#include <linux/bug.h>
#include <asm/module.h>
#include <linux/uaccess.h>
#include <asm/firmware.h>
#include <linux/sort.h>
#include <asm/setup.h>
#include <asm/sections.h>

static LIST_HEAD(module_bug_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const Elf_Shdr *find_section(const Elf_Ehdr *hdr,
				    const Elf_Shdr *sechdrs,
				    const char *name)
{
	char *secstrings;
	unsigned int i;

	secstrings = (char *)hdr + sechdrs[hdr->e_shstrndx].sh_offset;
	for (i = 1; i < hdr->e_shnum; i++)
		if (strcmp(secstrings+sechdrs[i].sh_name, name) == 0)
			return &sechdrs[i];
	return NULL;
}

int module_finalize(const Elf_Ehdr *hdr,
		const Elf_Shdr *sechdrs, struct module *me)
{
	const Elf_Shdr *sect;
<<<<<<< HEAD
=======
	int rc;

	rc = module_finalize_ftrace(me, sechdrs);
	if (rc)
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Apply feature fixups */
	sect = find_section(hdr, sechdrs, "__ftr_fixup");
	if (sect != NULL)
		do_feature_fixups(cur_cpu_spec->cpu_features,
				  (void *)sect->sh_addr,
				  (void *)sect->sh_addr + sect->sh_size);

	sect = find_section(hdr, sechdrs, "__mmu_ftr_fixup");
	if (sect != NULL)
		do_feature_fixups(cur_cpu_spec->mmu_features,
				  (void *)sect->sh_addr,
				  (void *)sect->sh_addr + sect->sh_size);

#ifdef CONFIG_PPC64
	sect = find_section(hdr, sechdrs, "__fw_ftr_fixup");
	if (sect != NULL)
		do_feature_fixups(powerpc_firmware_features,
				  (void *)sect->sh_addr,
				  (void *)sect->sh_addr + sect->sh_size);
<<<<<<< HEAD
#endif
=======
#endif /* CONFIG_PPC64 */

#ifdef CONFIG_PPC64_ELF_ABI_V1
	sect = find_section(hdr, sechdrs, ".opd");
	if (sect != NULL) {
		me->arch.start_opd = sect->sh_addr;
		me->arch.end_opd = sect->sh_addr + sect->sh_size;
	}
#endif /* CONFIG_PPC64_ELF_ABI_V1 */

#ifdef CONFIG_PPC_BARRIER_NOSPEC
	sect = find_section(hdr, sechdrs, "__spec_barrier_fixup");
	if (sect != NULL)
		do_barrier_nospec_fixups_range(barrier_nospec_enabled,
				  (void *)sect->sh_addr,
				  (void *)sect->sh_addr + sect->sh_size);
#endif /* CONFIG_PPC_BARRIER_NOSPEC */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sect = find_section(hdr, sechdrs, "__lwsync_fixup");
	if (sect != NULL)
		do_lwsync_fixups(cur_cpu_spec->cpu_features,
				 (void *)sect->sh_addr,
				 (void *)sect->sh_addr + sect->sh_size);

	return 0;
}
<<<<<<< HEAD
=======

static __always_inline void *
__module_alloc(unsigned long size, unsigned long start, unsigned long end, bool nowarn)
{
	pgprot_t prot = strict_module_rwx_enabled() ? PAGE_KERNEL : PAGE_KERNEL_EXEC;
	gfp_t gfp = GFP_KERNEL | (nowarn ? __GFP_NOWARN : 0);

	/*
	 * Don't do huge page allocations for modules yet until more testing
	 * is done. STRICT_MODULE_RWX may require extra work to support this
	 * too.
	 */
	return __vmalloc_node_range(size, 1, start, end, gfp, prot,
				    VM_FLUSH_RESET_PERMS,
				    NUMA_NO_NODE, __builtin_return_address(0));
}

void *module_alloc(unsigned long size)
{
#ifdef MODULES_VADDR
	unsigned long limit = (unsigned long)_etext - SZ_32M;
	void *ptr = NULL;

	BUILD_BUG_ON(TASK_SIZE > MODULES_VADDR);

	/* First try within 32M limit from _etext to avoid branch trampolines */
	if (MODULES_VADDR < PAGE_OFFSET && MODULES_END > limit)
		ptr = __module_alloc(size, limit, MODULES_END, true);

	if (!ptr)
		ptr = __module_alloc(size, MODULES_VADDR, MODULES_END, false);

	return ptr;
#else
	return __module_alloc(size, VMALLOC_START, VMALLOC_END, false);
#endif
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
