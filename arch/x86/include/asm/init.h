<<<<<<< HEAD
#ifndef _ASM_X86_INIT_32_H
#define _ASM_X86_INIT_32_H

#ifdef CONFIG_X86_32
extern void __init early_ioremap_page_table_range_init(void);
#endif

extern void __init zone_sizes_init(void);

extern unsigned long __init
kernel_physical_mapping_init(unsigned long start,
			     unsigned long end,
			     unsigned long page_size_mask);


extern unsigned long __initdata pgt_buf_start;
extern unsigned long __meminitdata pgt_buf_end;
extern unsigned long __meminitdata pgt_buf_top;

#endif /* _ASM_X86_INIT_32_H */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_INIT_H
#define _ASM_X86_INIT_H

#define __head	__section(".head.text")

struct x86_mapping_info {
	void *(*alloc_pgt_page)(void *); /* allocate buf for page table */
	void *context;			 /* context for alloc_pgt_page */
	unsigned long page_flag;	 /* page flag for PMD or PUD entry */
	unsigned long offset;		 /* ident mapping offset */
	bool direct_gbpages;		 /* PUD level 1GB page support */
	unsigned long kernpg_flag;	 /* kernel pagetable flag override */
};

int kernel_ident_mapping_init(struct x86_mapping_info *info, pgd_t *pgd_page,
				unsigned long pstart, unsigned long pend);

#endif /* _ASM_X86_INIT_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
