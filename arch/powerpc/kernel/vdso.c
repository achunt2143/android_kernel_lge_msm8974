<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *    Copyright (C) 2004 Benjamin Herrenschmidt, IBM Corp.
 *			 <benh@kernel.crashing.org>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/stddef.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <linux/user.h>
#include <linux/elf.h>
#include <linux/security.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
#include <linux/memblock.h>

#include <asm/pgtable.h>
#include <asm/processor.h>
#include <asm/mmu.h>
#include <asm/mmu_context.h>
#include <asm/prom.h>
=======
#include <linux/memblock.h>
#include <linux/syscalls.h>
#include <linux/time_namespace.h>
#include <vdso/datapage.h>

#include <asm/syscall.h>
#include <asm/processor.h>
#include <asm/mmu.h>
#include <asm/mmu_context.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/machdep.h>
#include <asm/cputable.h>
#include <asm/sections.h>
#include <asm/firmware.h>
#include <asm/vdso.h>
#include <asm/vdso_datapage.h>
<<<<<<< HEAD

#include "setup.h"

#undef DEBUG

#ifdef DEBUG
#define DBG(fmt...) printk(fmt)
#else
#define DBG(fmt...)
#endif

/* Max supported size for symbol names */
#define MAX_SYMNAME	64
=======
#include <asm/setup.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* The alignment of the vDSO */
#define VDSO_ALIGNMENT	(1 << 16)

extern char vdso32_start, vdso32_end;
<<<<<<< HEAD
static void *vdso32_kbase = &vdso32_start;
static unsigned int vdso32_pages;
static struct page **vdso32_pagelist;
unsigned long vdso32_sigtramp;
unsigned long vdso32_rt_sigtramp;

#ifdef CONFIG_PPC64
extern char vdso64_start, vdso64_end;
static void *vdso64_kbase = &vdso64_start;
static unsigned int vdso64_pages;
static struct page **vdso64_pagelist;
unsigned long vdso64_rt_sigtramp;
#endif /* CONFIG_PPC64 */

static int vdso_ready;
=======
extern char vdso64_start, vdso64_end;

long sys_ni_syscall(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The vdso data page (aka. systemcfg for old ppc64 fans) is here.
 * Once the early boot kernel code no longer needs to muck around
 * with it, it will become dynamically allocated
 */
static union {
<<<<<<< HEAD
	struct vdso_data	data;
	u8			page[PAGE_SIZE];
} vdso_data_store __page_aligned_data;
struct vdso_data *vdso_data = &vdso_data_store.data;

/* Format of the patch table */
struct vdso_patch_def
{
	unsigned long	ftr_mask, ftr_value;
	const char	*gen_name;
	const char	*fix_name;
};

/* Table of functions to patch based on the CPU type/revision
 *
 * Currently, we only change sync_dicache to do nothing on processors
 * with a coherent icache
 */
static struct vdso_patch_def vdso_patches[] = {
	{
		CPU_FTR_COHERENT_ICACHE, CPU_FTR_COHERENT_ICACHE,
		"__kernel_sync_dicache", "__kernel_sync_dicache_p5"
	},
	{
		CPU_FTR_USE_TB, 0,
		"__kernel_gettimeofday", NULL
	},
	{
		CPU_FTR_USE_TB, 0,
		"__kernel_clock_gettime", NULL
	},
	{
		CPU_FTR_USE_TB, 0,
		"__kernel_clock_getres", NULL
	},
	{
		CPU_FTR_USE_TB, 0,
		"__kernel_get_tbfreq", NULL
	},
};

/*
 * Some infos carried around for each of them during parsing at
 * boot time.
 */
struct lib32_elfinfo
{
	Elf32_Ehdr	*hdr;		/* ptr to ELF */
	Elf32_Sym	*dynsym;	/* ptr to .dynsym section */
	unsigned long	dynsymsize;	/* size of .dynsym section */
	char		*dynstr;	/* ptr to .dynstr section */
	unsigned long	text;		/* offset of .text section in .so */
};

struct lib64_elfinfo
{
	Elf64_Ehdr	*hdr;
	Elf64_Sym	*dynsym;
	unsigned long	dynsymsize;
	char		*dynstr;
	unsigned long	text;
};


#ifdef __DEBUG
static void dump_one_vdso_page(struct page *pg, struct page *upg)
{
	printk("kpg: %p (c:%d,f:%08lx)", __va(page_to_pfn(pg) << PAGE_SHIFT),
	       page_count(pg),
	       pg->flags);
	if (upg && !IS_ERR(upg) /* && pg != upg*/) {
		printk(" upg: %p (c:%d,f:%08lx)", __va(page_to_pfn(upg)
						       << PAGE_SHIFT),
		       page_count(upg),
		       upg->flags);
	}
	printk("\n");
}

static void dump_vdso_pages(struct vm_area_struct * vma)
{
	int i;

	if (!vma || is_32bit_task()) {
		printk("vDSO32 @ %016lx:\n", (unsigned long)vdso32_kbase);
		for (i=0; i<vdso32_pages; i++) {
			struct page *pg = virt_to_page(vdso32_kbase +
						       i*PAGE_SIZE);
			struct page *upg = (vma && vma->vm_mm) ?
				follow_page(vma, vma->vm_start + i*PAGE_SIZE, 0)
				: NULL;
			dump_one_vdso_page(pg, upg);
		}
	}
	if (!vma || !is_32bit_task()) {
		printk("vDSO64 @ %016lx:\n", (unsigned long)vdso64_kbase);
		for (i=0; i<vdso64_pages; i++) {
			struct page *pg = virt_to_page(vdso64_kbase +
						       i*PAGE_SIZE);
			struct page *upg = (vma && vma->vm_mm) ?
				follow_page(vma, vma->vm_start + i*PAGE_SIZE, 0)
				: NULL;
			dump_one_vdso_page(pg, upg);
		}
	}
}
#endif /* DEBUG */
=======
	struct vdso_arch_data	data;
	u8			page[PAGE_SIZE];
} vdso_data_store __page_aligned_data;
struct vdso_arch_data *vdso_data = &vdso_data_store.data;

enum vvar_pages {
	VVAR_DATA_PAGE_OFFSET,
	VVAR_TIMENS_PAGE_OFFSET,
	VVAR_NR_PAGES,
};

static int vdso_mremap(const struct vm_special_mapping *sm, struct vm_area_struct *new_vma,
		       unsigned long text_size)
{
	unsigned long new_size = new_vma->vm_end - new_vma->vm_start;

	if (new_size != text_size)
		return -EINVAL;

	current->mm->context.vdso = (void __user *)new_vma->vm_start;

	return 0;
}

static int vdso32_mremap(const struct vm_special_mapping *sm, struct vm_area_struct *new_vma)
{
	return vdso_mremap(sm, new_vma, &vdso32_end - &vdso32_start);
}

static int vdso64_mremap(const struct vm_special_mapping *sm, struct vm_area_struct *new_vma)
{
	return vdso_mremap(sm, new_vma, &vdso64_end - &vdso64_start);
}

static vm_fault_t vvar_fault(const struct vm_special_mapping *sm,
			     struct vm_area_struct *vma, struct vm_fault *vmf);

static struct vm_special_mapping vvar_spec __ro_after_init = {
	.name = "[vvar]",
	.fault = vvar_fault,
};

static struct vm_special_mapping vdso32_spec __ro_after_init = {
	.name = "[vdso]",
	.mremap = vdso32_mremap,
};

static struct vm_special_mapping vdso64_spec __ro_after_init = {
	.name = "[vdso]",
	.mremap = vdso64_mremap,
};

#ifdef CONFIG_TIME_NS
struct vdso_data *arch_get_vdso_data(void *vvar_page)
{
	return ((struct vdso_arch_data *)vvar_page)->data;
}

/*
 * The vvar mapping contains data for a specific time namespace, so when a task
 * changes namespace we must unmap its vvar data for the old namespace.
 * Subsequent faults will map in data for the new namespace.
 *
 * For more details see timens_setup_vdso_data().
 */
int vdso_join_timens(struct task_struct *task, struct time_namespace *ns)
{
	struct mm_struct *mm = task->mm;
	VMA_ITERATOR(vmi, mm, 0);
	struct vm_area_struct *vma;

	mmap_read_lock(mm);
	for_each_vma(vmi, vma) {
		if (vma_is_special_mapping(vma, &vvar_spec))
			zap_vma_pages(vma);
	}
	mmap_read_unlock(mm);

	return 0;
}
#endif

static vm_fault_t vvar_fault(const struct vm_special_mapping *sm,
			     struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct page *timens_page = find_timens_vvar_page(vma);
	unsigned long pfn;

	switch (vmf->pgoff) {
	case VVAR_DATA_PAGE_OFFSET:
		if (timens_page)
			pfn = page_to_pfn(timens_page);
		else
			pfn = virt_to_pfn(vdso_data);
		break;
#ifdef CONFIG_TIME_NS
	case VVAR_TIMENS_PAGE_OFFSET:
		/*
		 * If a task belongs to a time namespace then a namespace
		 * specific VVAR is mapped with the VVAR_DATA_PAGE_OFFSET and
		 * the real VVAR page is mapped with the VVAR_TIMENS_PAGE_OFFSET
		 * offset.
		 * See also the comment near timens_setup_vdso_data().
		 */
		if (!timens_page)
			return VM_FAULT_SIGBUS;
		pfn = virt_to_pfn(vdso_data);
		break;
#endif /* CONFIG_TIME_NS */
	default:
		return VM_FAULT_SIGBUS;
	}

	return vmf_insert_pfn(vma, vmf->address, pfn);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is called from binfmt_elf, we create the special vma for the
 * vDSO and insert it into the mm struct tree
 */
<<<<<<< HEAD
int arch_setup_additional_pages(struct linux_binprm *bprm, int uses_interp)
{
	struct mm_struct *mm = current->mm;
	struct page **vdso_pagelist;
	unsigned long vdso_pages;
	unsigned long vdso_base;
	int rc;

	if (!vdso_ready)
		return 0;

#ifdef CONFIG_PPC64
	if (is_32bit_task()) {
		vdso_pagelist = vdso32_pagelist;
		vdso_pages = vdso32_pages;
		vdso_base = VDSO32_MBASE;
	} else {
		vdso_pagelist = vdso64_pagelist;
		vdso_pages = vdso64_pages;
		/*
		 * On 64bit we don't have a preferred map address. This
		 * allows get_unmapped_area to find an area near other mmaps
		 * and most likely share a SLB entry.
		 */
		vdso_base = 0;
	}
#else
	vdso_pagelist = vdso32_pagelist;
	vdso_pages = vdso32_pages;
	vdso_base = VDSO32_MBASE;
#endif

	current->mm->context.vdso_base = 0;

	/* vDSO has a problem and was disabled, just don't "enable" it for the
	 * process
	 */
	if (vdso_pages == 0)
		return 0;
	/* Add a page to the vdso size for the data page */
	vdso_pages ++;

	/*
	 * pick a base address for the vDSO in process space. We try to put it
	 * at vdso_base which is the "natural" base for it, but we might fail
	 * and end up putting it elsewhere.
	 * Add enough to the size so that the result can be aligned.
	 */
	down_write(&mm->mmap_sem);
	vdso_base = get_unmapped_area(NULL, vdso_base,
				      (vdso_pages << PAGE_SHIFT) +
				      ((VDSO_ALIGNMENT - 1) & PAGE_MASK),
				      0, 0);
	if (IS_ERR_VALUE(vdso_base)) {
		rc = vdso_base;
		goto fail_mmapsem;
	}
=======
static int __arch_setup_additional_pages(struct linux_binprm *bprm, int uses_interp)
{
	unsigned long vdso_size, vdso_base, mappings_size;
	struct vm_special_mapping *vdso_spec;
	unsigned long vvar_size = VVAR_NR_PAGES * PAGE_SIZE;
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;

	if (is_32bit_task()) {
		vdso_spec = &vdso32_spec;
		vdso_size = &vdso32_end - &vdso32_start;
	} else {
		vdso_spec = &vdso64_spec;
		vdso_size = &vdso64_end - &vdso64_start;
	}

	mappings_size = vdso_size + vvar_size;
	mappings_size += (VDSO_ALIGNMENT - 1) & PAGE_MASK;

	/*
	 * Pick a base address for the vDSO in process space.
	 * Add enough to the size so that the result can be aligned.
	 */
	vdso_base = get_unmapped_area(NULL, 0, mappings_size, 0, 0);
	if (IS_ERR_VALUE(vdso_base))
		return vdso_base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Add required alignment. */
	vdso_base = ALIGN(vdso_base, VDSO_ALIGNMENT);

	/*
	 * Put vDSO base into mm struct. We need to do this before calling
	 * install_special_mapping or the perf counter mmap tracking code
<<<<<<< HEAD
	 * will fail to recognise it as a vDSO (since arch_vma_name fails).
	 */
	current->mm->context.vdso_base = vdso_base;
=======
	 * will fail to recognise it as a vDSO.
	 */
	mm->context.vdso = (void __user *)vdso_base + vvar_size;

	vma = _install_special_mapping(mm, vdso_base, vvar_size,
				       VM_READ | VM_MAYREAD | VM_IO |
				       VM_DONTDUMP | VM_PFNMAP, &vvar_spec);
	if (IS_ERR(vma))
		return PTR_ERR(vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * our vma flags don't have VM_WRITE so by default, the process isn't
	 * allowed to write those pages.
	 * gdb can break that with ptrace interface, and thus trigger COW on
	 * those pages but it's then your responsibility to never do that on
	 * the "data" page of the vDSO or you'll stop getting kernel updates
	 * and your nice userland gettimeofday will be totally dead.
	 * It's fine to use that for setting breakpoints in the vDSO code
	 * pages though.
	 */
<<<<<<< HEAD
	rc = install_special_mapping(mm, vdso_base, vdso_pages << PAGE_SHIFT,
				     VM_READ|VM_EXEC|
				     VM_MAYREAD|VM_MAYWRITE|VM_MAYEXEC,
				     vdso_pagelist);
	if (rc) {
		current->mm->context.vdso_base = 0;
		goto fail_mmapsem;
	}

	up_write(&mm->mmap_sem);
	return 0;

 fail_mmapsem:
	up_write(&mm->mmap_sem);
	return rc;
}

const char *arch_vma_name(struct vm_area_struct *vma)
{
	if (vma->vm_mm && vma->vm_start == vma->vm_mm->context.vdso_base)
		return "[vdso]";
	return NULL;
}



static void * __init find_section32(Elf32_Ehdr *ehdr, const char *secname,
				  unsigned long *size)
{
	Elf32_Shdr *sechdrs;
	unsigned int i;
	char *secnames;

	/* Grab section headers and strings so we can tell who is who */
	sechdrs = (void *)ehdr + ehdr->e_shoff;
	secnames = (void *)ehdr + sechdrs[ehdr->e_shstrndx].sh_offset;

	/* Find the section they want */
	for (i = 1; i < ehdr->e_shnum; i++) {
		if (strcmp(secnames+sechdrs[i].sh_name, secname) == 0) {
			if (size)
				*size = sechdrs[i].sh_size;
			return (void *)ehdr + sechdrs[i].sh_offset;
		}
	}
	*size = 0;
	return NULL;
}

static Elf32_Sym * __init find_symbol32(struct lib32_elfinfo *lib,
					const char *symname)
{
	unsigned int i;
	char name[MAX_SYMNAME], *c;

	for (i = 0; i < (lib->dynsymsize / sizeof(Elf32_Sym)); i++) {
		if (lib->dynsym[i].st_name == 0)
			continue;
		strlcpy(name, lib->dynstr + lib->dynsym[i].st_name,
			MAX_SYMNAME);
		c = strchr(name, '@');
		if (c)
			*c = 0;
		if (strcmp(symname, name) == 0)
			return &lib->dynsym[i];
	}
	return NULL;
}

/* Note that we assume the section is .text and the symbol is relative to
 * the library base
 */
static unsigned long __init find_function32(struct lib32_elfinfo *lib,
					    const char *symname)
{
	Elf32_Sym *sym = find_symbol32(lib, symname);

	if (sym == NULL) {
		printk(KERN_WARNING "vDSO32: function %s not found !\n",
		       symname);
		return 0;
	}
	return sym->st_value - VDSO32_LBASE;
}

static int __init vdso_do_func_patch32(struct lib32_elfinfo *v32,
				       struct lib64_elfinfo *v64,
				       const char *orig, const char *fix)
{
	Elf32_Sym *sym32_gen, *sym32_fix;

	sym32_gen = find_symbol32(v32, orig);
	if (sym32_gen == NULL) {
		printk(KERN_ERR "vDSO32: Can't find symbol %s !\n", orig);
		return -1;
	}
	if (fix == NULL) {
		sym32_gen->st_name = 0;
		return 0;
	}
	sym32_fix = find_symbol32(v32, fix);
	if (sym32_fix == NULL) {
		printk(KERN_ERR "vDSO32: Can't find symbol %s !\n", fix);
		return -1;
	}
	sym32_gen->st_value = sym32_fix->st_value;
	sym32_gen->st_size = sym32_fix->st_size;
	sym32_gen->st_info = sym32_fix->st_info;
	sym32_gen->st_other = sym32_fix->st_other;
	sym32_gen->st_shndx = sym32_fix->st_shndx;

	return 0;
}


#ifdef CONFIG_PPC64

static void * __init find_section64(Elf64_Ehdr *ehdr, const char *secname,
				  unsigned long *size)
{
	Elf64_Shdr *sechdrs;
	unsigned int i;
	char *secnames;

	/* Grab section headers and strings so we can tell who is who */
	sechdrs = (void *)ehdr + ehdr->e_shoff;
	secnames = (void *)ehdr + sechdrs[ehdr->e_shstrndx].sh_offset;

	/* Find the section they want */
	for (i = 1; i < ehdr->e_shnum; i++) {
		if (strcmp(secnames+sechdrs[i].sh_name, secname) == 0) {
			if (size)
				*size = sechdrs[i].sh_size;
			return (void *)ehdr + sechdrs[i].sh_offset;
		}
	}
	if (size)
		*size = 0;
	return NULL;
}

static Elf64_Sym * __init find_symbol64(struct lib64_elfinfo *lib,
					const char *symname)
{
	unsigned int i;
	char name[MAX_SYMNAME], *c;

	for (i = 0; i < (lib->dynsymsize / sizeof(Elf64_Sym)); i++) {
		if (lib->dynsym[i].st_name == 0)
			continue;
		strlcpy(name, lib->dynstr + lib->dynsym[i].st_name,
			MAX_SYMNAME);
		c = strchr(name, '@');
		if (c)
			*c = 0;
		if (strcmp(symname, name) == 0)
			return &lib->dynsym[i];
	}
	return NULL;
}

/* Note that we assume the section is .text and the symbol is relative to
 * the library base
 */
static unsigned long __init find_function64(struct lib64_elfinfo *lib,
					    const char *symname)
{
	Elf64_Sym *sym = find_symbol64(lib, symname);

	if (sym == NULL) {
		printk(KERN_WARNING "vDSO64: function %s not found !\n",
		       symname);
		return 0;
	}
#ifdef VDS64_HAS_DESCRIPTORS
	return *((u64 *)(vdso64_kbase + sym->st_value - VDSO64_LBASE)) -
		VDSO64_LBASE;
#else
	return sym->st_value - VDSO64_LBASE;
#endif
}

static int __init vdso_do_func_patch64(struct lib32_elfinfo *v32,
				       struct lib64_elfinfo *v64,
				       const char *orig, const char *fix)
{
	Elf64_Sym *sym64_gen, *sym64_fix;

	sym64_gen = find_symbol64(v64, orig);
	if (sym64_gen == NULL) {
		printk(KERN_ERR "vDSO64: Can't find symbol %s !\n", orig);
		return -1;
	}
	if (fix == NULL) {
		sym64_gen->st_name = 0;
		return 0;
	}
	sym64_fix = find_symbol64(v64, fix);
	if (sym64_fix == NULL) {
		printk(KERN_ERR "vDSO64: Can't find symbol %s !\n", fix);
		return -1;
	}
	sym64_gen->st_value = sym64_fix->st_value;
	sym64_gen->st_size = sym64_fix->st_size;
	sym64_gen->st_info = sym64_fix->st_info;
	sym64_gen->st_other = sym64_fix->st_other;
	sym64_gen->st_shndx = sym64_fix->st_shndx;

	return 0;
}

#endif /* CONFIG_PPC64 */


static __init int vdso_do_find_sections(struct lib32_elfinfo *v32,
					struct lib64_elfinfo *v64)
{
	void *sect;

	/*
	 * Locate symbol tables & text section
	 */

	v32->dynsym = find_section32(v32->hdr, ".dynsym", &v32->dynsymsize);
	v32->dynstr = find_section32(v32->hdr, ".dynstr", NULL);
	if (v32->dynsym == NULL || v32->dynstr == NULL) {
		printk(KERN_ERR "vDSO32: required symbol section not found\n");
		return -1;
	}
	sect = find_section32(v32->hdr, ".text", NULL);
	if (sect == NULL) {
		printk(KERN_ERR "vDSO32: the .text section was not found\n");
		return -1;
	}
	v32->text = sect - vdso32_kbase;

#ifdef CONFIG_PPC64
	v64->dynsym = find_section64(v64->hdr, ".dynsym", &v64->dynsymsize);
	v64->dynstr = find_section64(v64->hdr, ".dynstr", NULL);
	if (v64->dynsym == NULL || v64->dynstr == NULL) {
		printk(KERN_ERR "vDSO64: required symbol section not found\n");
		return -1;
	}
	sect = find_section64(v64->hdr, ".text", NULL);
	if (sect == NULL) {
		printk(KERN_ERR "vDSO64: the .text section was not found\n");
		return -1;
	}
	v64->text = sect - vdso64_kbase;
#endif /* CONFIG_PPC64 */

	return 0;
}

static __init void vdso_setup_trampolines(struct lib32_elfinfo *v32,
					  struct lib64_elfinfo *v64)
{
	/*
	 * Find signal trampolines
	 */

#ifdef CONFIG_PPC64
	vdso64_rt_sigtramp = find_function64(v64, "__kernel_sigtramp_rt64");
#endif
	vdso32_sigtramp	   = find_function32(v32, "__kernel_sigtramp32");
	vdso32_rt_sigtramp = find_function32(v32, "__kernel_sigtramp_rt32");
}

static __init int vdso_fixup_datapage(struct lib32_elfinfo *v32,
				       struct lib64_elfinfo *v64)
{
	Elf32_Sym *sym32;
#ifdef CONFIG_PPC64
	Elf64_Sym *sym64;

       	sym64 = find_symbol64(v64, "__kernel_datapage_offset");
	if (sym64 == NULL) {
		printk(KERN_ERR "vDSO64: Can't find symbol "
		       "__kernel_datapage_offset !\n");
		return -1;
	}
	*((int *)(vdso64_kbase + sym64->st_value - VDSO64_LBASE)) =
		(vdso64_pages << PAGE_SHIFT) -
		(sym64->st_value - VDSO64_LBASE);
#endif /* CONFIG_PPC64 */

	sym32 = find_symbol32(v32, "__kernel_datapage_offset");
	if (sym32 == NULL) {
		printk(KERN_ERR "vDSO32: Can't find symbol "
		       "__kernel_datapage_offset !\n");
		return -1;
	}
	*((int *)(vdso32_kbase + (sym32->st_value - VDSO32_LBASE))) =
		(vdso32_pages << PAGE_SHIFT) -
		(sym32->st_value - VDSO32_LBASE);

	return 0;
}


static __init int vdso_fixup_features(struct lib32_elfinfo *v32,
				      struct lib64_elfinfo *v64)
{
	void *start32;
	unsigned long size32;

#ifdef CONFIG_PPC64
	void *start64;
	unsigned long size64;

	start64 = find_section64(v64->hdr, "__ftr_fixup", &size64);
	if (start64)
		do_feature_fixups(cur_cpu_spec->cpu_features,
				  start64, start64 + size64);

	start64 = find_section64(v64->hdr, "__mmu_ftr_fixup", &size64);
	if (start64)
		do_feature_fixups(cur_cpu_spec->mmu_features,
				  start64, start64 + size64);

	start64 = find_section64(v64->hdr, "__fw_ftr_fixup", &size64);
	if (start64)
		do_feature_fixups(powerpc_firmware_features,
				  start64, start64 + size64);

	start64 = find_section64(v64->hdr, "__lwsync_fixup", &size64);
	if (start64)
		do_lwsync_fixups(cur_cpu_spec->cpu_features,
				 start64, start64 + size64);
#endif /* CONFIG_PPC64 */

	start32 = find_section32(v32->hdr, "__ftr_fixup", &size32);
	if (start32)
		do_feature_fixups(cur_cpu_spec->cpu_features,
				  start32, start32 + size32);

	start32 = find_section32(v32->hdr, "__mmu_ftr_fixup", &size32);
	if (start32)
		do_feature_fixups(cur_cpu_spec->mmu_features,
				  start32, start32 + size32);

#ifdef CONFIG_PPC64
	start32 = find_section32(v32->hdr, "__fw_ftr_fixup", &size32);
	if (start32)
		do_feature_fixups(powerpc_firmware_features,
				  start32, start32 + size32);
#endif /* CONFIG_PPC64 */

	start32 = find_section32(v32->hdr, "__lwsync_fixup", &size32);
	if (start32)
		do_lwsync_fixups(cur_cpu_spec->cpu_features,
				 start32, start32 + size32);

	return 0;
}

static __init int vdso_fixup_alt_funcs(struct lib32_elfinfo *v32,
				       struct lib64_elfinfo *v64)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(vdso_patches); i++) {
		struct vdso_patch_def *patch = &vdso_patches[i];
		int match = (cur_cpu_spec->cpu_features & patch->ftr_mask)
			== patch->ftr_value;
		if (!match)
			continue;

		DBG("replacing %s with %s...\n", patch->gen_name,
		    patch->fix_name ? "NONE" : patch->fix_name);

		/*
		 * Patch the 32 bits and 64 bits symbols. Note that we do not
		 * patch the "." symbol on 64 bits.
		 * It would be easy to do, but doesn't seem to be necessary,
		 * patching the OPD symbol is enough.
		 */
		vdso_do_func_patch32(v32, v64, patch->gen_name,
				     patch->fix_name);
#ifdef CONFIG_PPC64
		vdso_do_func_patch64(v32, v64, patch->gen_name,
				     patch->fix_name);
#endif /* CONFIG_PPC64 */
	}

	return 0;
}


static __init int vdso_setup(void)
{
	struct lib32_elfinfo	v32;
	struct lib64_elfinfo	v64;

	v32.hdr = vdso32_kbase;
#ifdef CONFIG_PPC64
	v64.hdr = vdso64_kbase;
#endif
	if (vdso_do_find_sections(&v32, &v64))
		return -1;

	if (vdso_fixup_datapage(&v32, &v64))
		return -1;

	if (vdso_fixup_features(&v32, &v64))
		return -1;

	if (vdso_fixup_alt_funcs(&v32, &v64))
		return -1;

	vdso_setup_trampolines(&v32, &v64);

	return 0;
}

=======
	vma = _install_special_mapping(mm, vdso_base + vvar_size, vdso_size,
				       VM_READ | VM_EXEC | VM_MAYREAD |
				       VM_MAYWRITE | VM_MAYEXEC, vdso_spec);
	if (IS_ERR(vma))
		do_munmap(mm, vdso_base, vvar_size, NULL);

	return PTR_ERR_OR_ZERO(vma);
}

int arch_setup_additional_pages(struct linux_binprm *bprm, int uses_interp)
{
	struct mm_struct *mm = current->mm;
	int rc;

	mm->context.vdso = NULL;

	if (mmap_write_lock_killable(mm))
		return -EINTR;

	rc = __arch_setup_additional_pages(bprm, uses_interp);
	if (rc)
		mm->context.vdso = NULL;

	mmap_write_unlock(mm);
	return rc;
}

#define VDSO_DO_FIXUPS(type, value, bits, sec) do {					\
	void *__start = (void *)VDSO##bits##_SYMBOL(&vdso##bits##_start, sec##_start);	\
	void *__end = (void *)VDSO##bits##_SYMBOL(&vdso##bits##_start, sec##_end);	\
											\
	do_##type##_fixups((value), __start, __end);					\
} while (0)

static void __init vdso_fixup_features(void)
{
#ifdef CONFIG_PPC64
	VDSO_DO_FIXUPS(feature, cur_cpu_spec->cpu_features, 64, ftr_fixup);
	VDSO_DO_FIXUPS(feature, cur_cpu_spec->mmu_features, 64, mmu_ftr_fixup);
	VDSO_DO_FIXUPS(feature, powerpc_firmware_features, 64, fw_ftr_fixup);
	VDSO_DO_FIXUPS(lwsync, cur_cpu_spec->cpu_features, 64, lwsync_fixup);
#endif /* CONFIG_PPC64 */

#ifdef CONFIG_VDSO32
	VDSO_DO_FIXUPS(feature, cur_cpu_spec->cpu_features, 32, ftr_fixup);
	VDSO_DO_FIXUPS(feature, cur_cpu_spec->mmu_features, 32, mmu_ftr_fixup);
#ifdef CONFIG_PPC64
	VDSO_DO_FIXUPS(feature, powerpc_firmware_features, 32, fw_ftr_fixup);
#endif /* CONFIG_PPC64 */
	VDSO_DO_FIXUPS(lwsync, cur_cpu_spec->cpu_features, 32, lwsync_fixup);
#endif
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Called from setup_arch to initialize the bitmap of available
 * syscalls in the systemcfg page
 */
static void __init vdso_setup_syscall_map(void)
{
	unsigned int i;
<<<<<<< HEAD
	extern unsigned long *sys_call_table;
	extern unsigned long sys_ni_syscall;


	for (i = 0; i < __NR_syscalls; i++) {
#ifdef CONFIG_PPC64
		if (sys_call_table[i*2] != sys_ni_syscall)
			vdso_data->syscall_map_64[i >> 5] |=
				0x80000000UL >> (i & 0x1f);
		if (sys_call_table[i*2+1] != sys_ni_syscall)
			vdso_data->syscall_map_32[i >> 5] |=
				0x80000000UL >> (i & 0x1f);
#else /* CONFIG_PPC64 */
		if (sys_call_table[i] != sys_ni_syscall)
			vdso_data->syscall_map_32[i >> 5] |=
				0x80000000UL >> (i & 0x1f);
#endif /* CONFIG_PPC64 */
	}
}


static int __init vdso_init(void)
{
	int i;

=======

	for (i = 0; i < NR_syscalls; i++) {
		if (sys_call_table[i] != (void *)&sys_ni_syscall)
			vdso_data->syscall_map[i >> 5] |= 0x80000000UL >> (i & 0x1f);
		if (IS_ENABLED(CONFIG_COMPAT) &&
		    compat_sys_call_table[i] != (void *)&sys_ni_syscall)
			vdso_data->compat_syscall_map[i >> 5] |= 0x80000000UL >> (i & 0x1f);
	}
}

#ifdef CONFIG_PPC64
int vdso_getcpu_init(void)
{
	unsigned long cpu, node, val;

	/*
	 * SPRG_VDSO contains the CPU in the bottom 16 bits and the NUMA node
	 * in the next 16 bits.  The VDSO uses this to implement getcpu().
	 */
	cpu = get_cpu();
	WARN_ON_ONCE(cpu > 0xffff);

	node = cpu_to_node(cpu);
	WARN_ON_ONCE(node > 0xffff);

	val = (cpu & 0xffff) | ((node & 0xffff) << 16);
	mtspr(SPRN_SPRG_VDSO_WRITE, val);
	get_paca()->sprg_vdso = val;

	put_cpu();

	return 0;
}
/* We need to call this before SMP init */
early_initcall(vdso_getcpu_init);
#endif

static struct page ** __init vdso_setup_pages(void *start, void *end)
{
	int i;
	struct page **pagelist;
	int pages = (end - start) >> PAGE_SHIFT;

	pagelist = kcalloc(pages + 1, sizeof(struct page *), GFP_KERNEL);
	if (!pagelist)
		panic("%s: Cannot allocate page list for VDSO", __func__);

	for (i = 0; i < pages; i++)
		pagelist[i] = virt_to_page(start + i * PAGE_SIZE);

	return pagelist;
}

static int __init vdso_init(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PPC64
	/*
	 * Fill up the "systemcfg" stuff for backward compatibility
	 */
	strcpy((char *)vdso_data->eye_catcher, "SYSTEMCFG:PPC64");
	vdso_data->version.major = SYSTEMCFG_MAJOR;
	vdso_data->version.minor = SYSTEMCFG_MINOR;
	vdso_data->processor = mfspr(SPRN_PVR);
	/*
	 * Fake the old platform number for pSeries and add
	 * in LPAR bit if necessary
	 */
	vdso_data->platform = 0x100;
	if (firmware_has_feature(FW_FEATURE_LPAR))
		vdso_data->platform |= 1;
	vdso_data->physicalMemorySize = memblock_phys_mem_size();
<<<<<<< HEAD
	vdso_data->dcache_size = ppc64_caches.dsize;
	vdso_data->dcache_line_size = ppc64_caches.dline_size;
	vdso_data->icache_size = ppc64_caches.isize;
	vdso_data->icache_line_size = ppc64_caches.iline_size;

	/* XXXOJN: Blocks should be added to ppc64_caches and used instead */
	vdso_data->dcache_block_size = ppc64_caches.dline_size;
	vdso_data->icache_block_size = ppc64_caches.iline_size;
	vdso_data->dcache_log_block_size = ppc64_caches.log_dline_size;
	vdso_data->icache_log_block_size = ppc64_caches.log_iline_size;

	/*
	 * Calculate the size of the 64 bits vDSO
	 */
	vdso64_pages = (&vdso64_end - &vdso64_start) >> PAGE_SHIFT;
	DBG("vdso64_kbase: %p, 0x%x pages\n", vdso64_kbase, vdso64_pages);
#else
	vdso_data->dcache_block_size = L1_CACHE_BYTES;
	vdso_data->dcache_log_block_size = L1_CACHE_SHIFT;
	vdso_data->icache_block_size = L1_CACHE_BYTES;
	vdso_data->icache_log_block_size = L1_CACHE_SHIFT;
#endif /* CONFIG_PPC64 */


	/*
	 * Calculate the size of the 32 bits vDSO
	 */
	vdso32_pages = (&vdso32_end - &vdso32_start) >> PAGE_SHIFT;
	DBG("vdso32_kbase: %p, 0x%x pages\n", vdso32_kbase, vdso32_pages);


	/*
	 * Setup the syscall map in the vDOS
	 */
	vdso_setup_syscall_map();

	/*
	 * Initialize the vDSO images in memory, that is do necessary
	 * fixups of vDSO symbols, locate trampolines, etc...
	 */
	if (vdso_setup()) {
		printk(KERN_ERR "vDSO setup failure, not enabled !\n");
		vdso32_pages = 0;
#ifdef CONFIG_PPC64
		vdso64_pages = 0;
#endif
		return 0;
	}

	/* Make sure pages are in the correct state */
	vdso32_pagelist = kzalloc(sizeof(struct page *) * (vdso32_pages + 2),
				  GFP_KERNEL);
	BUG_ON(vdso32_pagelist == NULL);
	for (i = 0; i < vdso32_pages; i++) {
		struct page *pg = virt_to_page(vdso32_kbase + i*PAGE_SIZE);
		ClearPageReserved(pg);
		get_page(pg);
		vdso32_pagelist[i] = pg;
	}
	vdso32_pagelist[i++] = virt_to_page(vdso_data);
	vdso32_pagelist[i] = NULL;

#ifdef CONFIG_PPC64
	vdso64_pagelist = kzalloc(sizeof(struct page *) * (vdso64_pages + 2),
				  GFP_KERNEL);
	BUG_ON(vdso64_pagelist == NULL);
	for (i = 0; i < vdso64_pages; i++) {
		struct page *pg = virt_to_page(vdso64_kbase + i*PAGE_SIZE);
		ClearPageReserved(pg);
		get_page(pg);
		vdso64_pagelist[i] = pg;
	}
	vdso64_pagelist[i++] = virt_to_page(vdso_data);
	vdso64_pagelist[i] = NULL;
#endif /* CONFIG_PPC64 */

	get_page(virt_to_page(vdso_data));

	smp_wmb();
	vdso_ready = 1;
=======
	vdso_data->dcache_size = ppc64_caches.l1d.size;
	vdso_data->dcache_line_size = ppc64_caches.l1d.line_size;
	vdso_data->icache_size = ppc64_caches.l1i.size;
	vdso_data->icache_line_size = ppc64_caches.l1i.line_size;
	vdso_data->dcache_block_size = ppc64_caches.l1d.block_size;
	vdso_data->icache_block_size = ppc64_caches.l1i.block_size;
	vdso_data->dcache_log_block_size = ppc64_caches.l1d.log_block_size;
	vdso_data->icache_log_block_size = ppc64_caches.l1i.log_block_size;
#endif /* CONFIG_PPC64 */

	vdso_setup_syscall_map();

	vdso_fixup_features();

	if (IS_ENABLED(CONFIG_VDSO32))
		vdso32_spec.pages = vdso_setup_pages(&vdso32_start, &vdso32_end);

	if (IS_ENABLED(CONFIG_PPC64))
		vdso64_spec.pages = vdso_setup_pages(&vdso64_start, &vdso64_end);

	smp_wmb();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
arch_initcall(vdso_init);
<<<<<<< HEAD

int in_gate_area_no_mm(unsigned long addr)
{
	return 0;
}

int in_gate_area(struct mm_struct *mm, unsigned long addr)
{
	return 0;
}

struct vm_area_struct *get_gate_vma(struct mm_struct *mm)
{
	return NULL;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
