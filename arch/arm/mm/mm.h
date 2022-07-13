<<<<<<< HEAD
#ifdef CONFIG_MMU
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifdef CONFIG_MMU
#include <linux/list.h>
#include <linux/vmalloc.h>
#include <linux/pgtable.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* the upper-most page table pointer */
extern pmd_t *top_pmd;

<<<<<<< HEAD
=======
extern int icache_size;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * 0xffff8000 to 0xffffffff is reserved for any ARM architecture
 * specific hacks for copying pages efficiently, while 0xffff4000
 * is reserved for VIPT aliasing flushing by generic code.
 *
 * Note that we don't allow VIPT aliasing caches with SMP.
 */
#define COPYPAGE_MINICACHE	0xffff8000
#define COPYPAGE_V6_FROM	0xffff8000
#define COPYPAGE_V6_TO		0xffffc000
/* PFN alias flushing, for VIPT caches */
#define FLUSH_ALIAS_START	0xffff4000

static inline void set_top_pte(unsigned long va, pte_t pte)
{
	pte_t *ptep = pte_offset_kernel(top_pmd, va);
	set_pte_ext(ptep, pte, 0);
	local_flush_tlb_kernel_page(va);
}

static inline pte_t get_top_pte(unsigned long va)
{
	pte_t *ptep = pte_offset_kernel(top_pmd, va);
	return *ptep;
}

<<<<<<< HEAD
static inline pmd_t *pmd_off_k(unsigned long virt)
{
	return pmd_offset(pud_offset(pgd_offset_k(virt), virt), virt);
}

struct mem_type {
	pteval_t prot_pte;
=======
struct mem_type {
	pteval_t prot_pte;
	pteval_t prot_pte_s2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pmdval_t prot_l1;
	pmdval_t prot_sect;
	unsigned int domain;
};

const struct mem_type *get_mem_type(unsigned int type);

<<<<<<< HEAD
extern void __flush_dcache_page(struct address_space *mapping, struct page *page);
=======
void __flush_dcache_folio(struct address_space *mapping, struct folio *folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * ARM specific vm_struct->flags bits.
 */

/* (super)section-mapped I/O regions used by ioremap()/iounmap() */
#define VM_ARM_SECTION_MAPPING	0x80000000

/* permanent static mappings from iotable_init() */
#define VM_ARM_STATIC_MAPPING	0x40000000

/* empty mapping */
#define VM_ARM_EMPTY_MAPPING	0x20000000

/* mapping type (attributes) for permanent static mappings */
#define VM_ARM_MTYPE(mt)		((mt) << 20)
#define VM_ARM_MTYPE_MASK	(0x1f << 20)

<<<<<<< HEAD
/* consistent regions used by dma_alloc_attrs() */
#define VM_ARM_DMA_CONSISTENT	0x20000000
=======

struct static_vm {
	struct vm_struct vm;
	struct list_head list;
};

extern struct list_head static_vmlist;
extern struct static_vm *find_static_vm_vaddr(void *vaddr);
extern __init void add_static_vm_early(struct static_vm *svm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif

#ifdef CONFIG_ZONE_DMA
extern phys_addr_t arm_dma_limit;
<<<<<<< HEAD
#else
#define arm_dma_limit ((phys_addr_t)~0)
=======
extern unsigned long arm_dma_pfn_limit;
#else
#define arm_dma_limit ((phys_addr_t)~0)
#define arm_dma_pfn_limit (~0ul >> PAGE_SHIFT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

extern phys_addr_t arm_lowmem_limit;

void __init bootmem_init(void);
void arm_mm_memblock_reserve(void);
<<<<<<< HEAD
void dma_contiguous_remap(void);
=======
#ifdef CONFIG_CMA_AREAS
void dma_contiguous_remap(void);
#else
static inline void dma_contiguous_remap(void) { }
#endif

unsigned long __clear_cr(unsigned long mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
