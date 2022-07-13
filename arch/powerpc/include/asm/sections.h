<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_POWERPC_SECTIONS_H
#define _ASM_POWERPC_SECTIONS_H
#ifdef __KERNEL__

#include <linux/elf.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
#include <asm-generic/sections.h>

#ifdef __powerpc64__

extern char __end_interrupts[];

static inline int in_kernel_text(unsigned long addr)
{
	if (addr >= (unsigned long)_stext && addr < (unsigned long)__init_end)
		return 1;

	return 0;
=======

#ifdef CONFIG_HAVE_FUNCTION_DESCRIPTORS
typedef struct func_desc func_desc_t;
#endif

#include <asm-generic/sections.h>

extern char __head_end[];
extern char __srwx_boundary[];
extern char __exittext_begin[], __exittext_end[];

/* Patch sites */
extern s32 patch__call_flush_branch_caches1;
extern s32 patch__call_flush_branch_caches2;
extern s32 patch__call_flush_branch_caches3;
extern s32 patch__flush_count_cache_return;
extern s32 patch__flush_link_stack_return;
extern s32 patch__call_kvm_flush_link_stack;
extern s32 patch__call_kvm_flush_link_stack_p9;
extern s32 patch__memset_nocache, patch__memcpy_nocache;

extern long flush_branch_caches;
extern long kvm_flush_link_stack;

#ifdef __powerpc64__

extern char __start_interrupts[];
extern char __end_interrupts[];

#ifdef CONFIG_PPC_POWERNV
extern char start_real_trampolines[];
extern char end_real_trampolines[];
extern char start_virt_trampolines[];
extern char end_virt_trampolines[];
#endif

/*
 * This assumes the kernel is never compiled -mcmodel=small or
 * the total .toc is always less than 64k.
 */
static inline unsigned long kernel_toc_addr(void)
{
#ifdef CONFIG_PPC_KERNEL_PCREL
	BUILD_BUG();
	return -1UL;
#else
	unsigned long toc_ptr;

	asm volatile("mr %0, 2" : "=r" (toc_ptr));
	return toc_ptr;
#endif
}

static inline int overlaps_interrupt_vector_text(unsigned long start,
							unsigned long end)
{
	unsigned long real_start, real_end;
	real_start = __start_interrupts - _stext;
	real_end = __end_interrupts - _stext;

	return start < (unsigned long)__va(real_end) &&
		(unsigned long)__va(real_start) < end;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int overlaps_kernel_text(unsigned long start, unsigned long end)
{
	return start < (unsigned long)__init_end &&
		(unsigned long)_stext < end;
}

<<<<<<< HEAD
#undef dereference_function_descriptor
static inline void *dereference_function_descriptor(void *ptr)
{
	struct ppc64_opd_entry *desc = ptr;
	void *p;

	if (!probe_kernel_address(&desc->funcaddr, p))
		ptr = p;
	return ptr;
}

=======
#else
static inline unsigned long kernel_toc_addr(void) { BUILD_BUG(); return -1UL; }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* __KERNEL__ */
#endif	/* _ASM_POWERPC_SECTIONS_H */
