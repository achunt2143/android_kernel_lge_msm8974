<<<<<<< HEAD
/*
 *  include/asm-s390/pgtable.h
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  S390 version
 *    Copyright IBM Corp. 1999, 2000
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Hartmut Penner (hp@de.ibm.com)
 *               Ulrich Weigand (weigand@de.ibm.com)
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 *  Derived from "include/asm-i386/pgtable.h"
 */

#ifndef _ASM_S390_PGTABLE_H
#define _ASM_S390_PGTABLE_H

<<<<<<< HEAD
/*
 * The Linux memory management assumes a three-level page table setup. For
 * s390 31 bit we "fold" the mid level into the top-level page table, so
 * that we physically have the same two-level page table as the s390 mmu
 * expects in 31 bit mode. For s390 64 bit we use three of the five levels
 * the hardware provides (region first and region second tables are not
 * used).
 *
 * The "pgd_xxx()" functions are trivial for a folded two-level
 * setup: the pgd is never bad, and a pmd always exists (as it's folded
 * into the pgd entry)
 *
 * This file contains the functions and defines necessary to modify and use
 * the S390 page table tree.
 */
#ifndef __ASSEMBLY__
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <asm/bug.h>
#include <asm/page.h>

extern pgd_t swapper_pg_dir[] __attribute__ ((aligned (4096)));
extern void paging_init(void);
extern void vmem_map_init(void);
extern void fault_init(void);
=======
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/page-flags.h>
#include <linux/radix-tree.h>
#include <linux/atomic.h>
#include <asm/sections.h>
#include <asm/ctlreg.h>
#include <asm/bug.h>
#include <asm/page.h>
#include <asm/uv.h>

extern pgd_t swapper_pg_dir[];
extern pgd_t invalid_pg_dir[];
extern void paging_init(void);
extern struct ctlreg s390_invalid_asce;

enum {
	PG_DIRECT_MAP_4K = 0,
	PG_DIRECT_MAP_1M,
	PG_DIRECT_MAP_2G,
	PG_DIRECT_MAP_MAX
};

extern atomic_long_t __bootdata_preserved(direct_pages_count[PG_DIRECT_MAP_MAX]);

static inline void update_page_count(int level, long count)
{
	if (IS_ENABLED(CONFIG_PROC_FS))
		atomic_long_add(count, &direct_pages_count[level]);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The S390 doesn't have any external MMU info: the kernel page
 * tables contain all the necessary information.
 */
#define update_mmu_cache(vma, address, ptep)     do { } while (0)
<<<<<<< HEAD
=======
#define update_mmu_cache_range(vmf, vma, addr, ptep, nr) do { } while (0)
#define update_mmu_cache_pmd(vma, address, ptep) do { } while (0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * ZERO_PAGE is a global shared page that is always zero; used
 * for zero-mapped memory areas etc..
 */

extern unsigned long empty_zero_page;
extern unsigned long zero_page_mask;

#define ZERO_PAGE(vaddr) \
	(virt_to_page((void *)(empty_zero_page + \
	 (((unsigned long)(vaddr)) &zero_page_mask))))
<<<<<<< HEAD

#define is_zero_pfn is_zero_pfn
static inline int is_zero_pfn(unsigned long pfn)
{
	extern unsigned long zero_pfn;
	unsigned long offset_from_zero_pfn = pfn - zero_pfn;
	return offset_from_zero_pfn <= (zero_page_mask >> PAGE_SHIFT);
}

#define my_zero_pfn(addr)	page_to_pfn(ZERO_PAGE(addr))

/* TODO: s390 cannot support io_remap_pfn_range... */
#define io_remap_pfn_range(vma, vaddr, pfn, size, prot) 	       \
	remap_pfn_range(vma, vaddr, pfn, size, prot)

#endif /* !__ASSEMBLY__ */

/*
 * PMD_SHIFT determines the size of the area a second-level page
 * table can map
 * PGDIR_SHIFT determines what a third-level page table entry can map
 */
#ifndef __s390x__
# define PMD_SHIFT	20
# define PUD_SHIFT	20
# define PGDIR_SHIFT	20
#else /* __s390x__ */
# define PMD_SHIFT	20
# define PUD_SHIFT	31
# define PGDIR_SHIFT	42
#endif /* __s390x__ */

#define PMD_SIZE        (1UL << PMD_SHIFT)
#define PMD_MASK        (~(PMD_SIZE-1))
#define PUD_SIZE	(1UL << PUD_SHIFT)
#define PUD_MASK	(~(PUD_SIZE-1))
#define PGDIR_SIZE	(1UL << PGDIR_SHIFT)
#define PGDIR_MASK	(~(PGDIR_SIZE-1))

/*
 * entries per page directory level: the S390 is two-level, so
 * we don't really have any PMD directory physically.
 * for S390 segment-table entries are combined to one PGD
 * that leads to 1024 pte per pgd
 */
#define PTRS_PER_PTE	256
#ifndef __s390x__
#define PTRS_PER_PMD	1
#define PTRS_PER_PUD	1
#else /* __s390x__ */
#define PTRS_PER_PMD	2048
#define PTRS_PER_PUD	2048
#endif /* __s390x__ */
#define PTRS_PER_PGD	2048

#define FIRST_USER_ADDRESS  0

#define pte_ERROR(e) \
	printk("%s:%d: bad pte %p.\n", __FILE__, __LINE__, (void *) pte_val(e))
#define pmd_ERROR(e) \
	printk("%s:%d: bad pmd %p.\n", __FILE__, __LINE__, (void *) pmd_val(e))
#define pud_ERROR(e) \
	printk("%s:%d: bad pud %p.\n", __FILE__, __LINE__, (void *) pud_val(e))
#define pgd_ERROR(e) \
	printk("%s:%d: bad pgd %p.\n", __FILE__, __LINE__, (void *) pgd_val(e))

#ifndef __ASSEMBLY__
/*
 * The vmalloc area will always be on the topmost area of the kernel
 * mapping. We reserve 96MB (31bit) / 128GB (64bit) for vmalloc,
 * which should be enough for any sane case.
 * By putting vmalloc at the top, we maximise the gap between physical
 * memory and vmalloc to catch misplaced memory accesses. As a side
 * effect, this also makes sure that 64 bit module code cannot be used
 * as system call address.
 */
extern unsigned long VMALLOC_START;
extern unsigned long VMALLOC_END;
extern struct page *vmemmap;

#define VMEM_MAX_PHYS ((unsigned long) vmemmap)

/*
 * A 31 bit pagetable entry of S390 has following format:
 *  |   PFRA          |    |  OS  |
 * 0                   0IP0
 * 00000000001111111111222222222233
 * 01234567890123456789012345678901
 *
 * I Page-Invalid Bit:    Page is not available for address-translation
 * P Page-Protection Bit: Store access not possible for page
 *
 * A 31 bit segmenttable entry of S390 has following format:
 *  |   P-table origin      |  |PTL
 * 0                         IC
 * 00000000001111111111222222222233
 * 01234567890123456789012345678901
 *
 * I Segment-Invalid Bit:    Segment is not available for address-translation
 * C Common-Segment Bit:     Segment is not private (PoP 3-30)
 * PTL Page-Table-Length:    Page-table length (PTL+1*16 entries -> up to 256)
 *
 * The 31 bit segmenttable origin of S390 has following format:
 *
 *  |S-table origin   |     | STL |
 * X                   **GPS
 * 00000000001111111111222222222233
 * 01234567890123456789012345678901
 *
 * X Space-Switch event:
 * G Segment-Invalid Bit:     *
 * P Private-Space Bit:       Segment is not private (PoP 3-30)
 * S Storage-Alteration:
 * STL Segment-Table-Length:  Segment-table length (STL+1*16 entries -> up to 2048)
 *
=======
#define __HAVE_COLOR_ZERO_PAGE

/* TODO: s390 cannot support io_remap_pfn_range... */

#define pte_ERROR(e) \
	pr_err("%s:%d: bad pte %016lx.\n", __FILE__, __LINE__, pte_val(e))
#define pmd_ERROR(e) \
	pr_err("%s:%d: bad pmd %016lx.\n", __FILE__, __LINE__, pmd_val(e))
#define pud_ERROR(e) \
	pr_err("%s:%d: bad pud %016lx.\n", __FILE__, __LINE__, pud_val(e))
#define p4d_ERROR(e) \
	pr_err("%s:%d: bad p4d %016lx.\n", __FILE__, __LINE__, p4d_val(e))
#define pgd_ERROR(e) \
	pr_err("%s:%d: bad pgd %016lx.\n", __FILE__, __LINE__, pgd_val(e))

/*
 * The vmalloc and module area will always be on the topmost area of the
 * kernel mapping. 512GB are reserved for vmalloc by default.
 * At the top of the vmalloc area a 2GB area is reserved where modules
 * will reside. That makes sure that inter module branches always
 * happen without trampolines and in addition the placement within a
 * 2GB frame is branch prediction unit friendly.
 */
extern unsigned long __bootdata_preserved(VMALLOC_START);
extern unsigned long __bootdata_preserved(VMALLOC_END);
#define VMALLOC_DEFAULT_SIZE	((512UL << 30) - MODULES_LEN)
extern struct page *__bootdata_preserved(vmemmap);
extern unsigned long __bootdata_preserved(vmemmap_size);

extern unsigned long __bootdata_preserved(MODULES_VADDR);
extern unsigned long __bootdata_preserved(MODULES_END);
#define MODULES_VADDR	MODULES_VADDR
#define MODULES_END	MODULES_END
#define MODULES_LEN	(1UL << 31)

static inline int is_module_addr(void *addr)
{
	BUILD_BUG_ON(MODULES_LEN > (1UL << 31));
	if (addr < (void *)MODULES_VADDR)
		return 0;
	if (addr > (void *)MODULES_END)
		return 0;
	return 1;
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * A 64 bit pagetable entry of S390 has following format:
 * |			 PFRA			      |0IPC|  OS  |
 * 0000000000111111111122222222223333333333444444444455555555556666
 * 0123456789012345678901234567890123456789012345678901234567890123
 *
 * I Page-Invalid Bit:    Page is not available for address-translation
 * P Page-Protection Bit: Store access not possible for page
 * C Change-bit override: HW is not required to set change bit
 *
 * A 64 bit segmenttable entry of S390 has following format:
 * |        P-table origin                              |      TT
 * 0000000000111111111122222222223333333333444444444455555555556666
 * 0123456789012345678901234567890123456789012345678901234567890123
 *
 * I Segment-Invalid Bit:    Segment is not available for address-translation
 * C Common-Segment Bit:     Segment is not private (PoP 3-30)
 * P Page-Protection Bit: Store access not possible for page
 * TT Type 00
 *
 * A 64 bit region table entry of S390 has following format:
 * |        S-table origin                             |   TF  TTTL
 * 0000000000111111111122222222223333333333444444444455555555556666
 * 0123456789012345678901234567890123456789012345678901234567890123
 *
 * I Segment-Invalid Bit:    Segment is not available for address-translation
 * TT Type 01
 * TF
 * TL Table length
 *
 * The 64 bit regiontable origin of S390 has following format:
 * |      region table origon                          |       DTTL
 * 0000000000111111111122222222223333333333444444444455555555556666
 * 0123456789012345678901234567890123456789012345678901234567890123
 *
 * X Space-Switch event:
 * G Segment-Invalid Bit:  
 * P Private-Space Bit:    
 * S Storage-Alteration:
 * R Real space
 * TL Table-Length:
 *
 * A storage key has the following format:
 * | ACC |F|R|C|0|
 *  0   3 4 5 6 7
 * ACC: access key
 * F  : fetch protection bit
 * R  : referenced bit
 * C  : changed bit
 */

/* Hardware bits in the page table entry */
<<<<<<< HEAD
#define _PAGE_CO	0x100		/* HW Change-bit override */
#define _PAGE_RO	0x200		/* HW read-only bit  */
#define _PAGE_INVALID	0x400		/* HW invalid bit    */

/* Software bits in the page table entry */
#define _PAGE_SWT	0x001		/* SW pte type bit t */
#define _PAGE_SWX	0x002		/* SW pte type bit x */
#define _PAGE_SWC	0x004		/* SW pte changed bit (for KVM) */
#define _PAGE_SWR	0x008		/* SW pte referenced bit (for KVM) */
#define _PAGE_SPECIAL	0x010		/* SW associated with special page */
#define __HAVE_ARCH_PTE_SPECIAL

/* Set of bits not changed in pte_modify */
#define _PAGE_CHG_MASK	(PAGE_MASK | _PAGE_SPECIAL | _PAGE_SWC | _PAGE_SWR)

/* Six different types of pages. */
#define _PAGE_TYPE_EMPTY	0x400
#define _PAGE_TYPE_NONE		0x401
#define _PAGE_TYPE_SWAP		0x403
#define _PAGE_TYPE_FILE		0x601	/* bit 0x002 is used for offset !! */
#define _PAGE_TYPE_RO		0x200
#define _PAGE_TYPE_RW		0x000

/*
 * Only four types for huge pages, using the invalid bit and protection bit
 * of a segment table entry.
 */
#define _HPAGE_TYPE_EMPTY	0x020	/* _SEGMENT_ENTRY_INV */
#define _HPAGE_TYPE_NONE	0x220
#define _HPAGE_TYPE_RO		0x200	/* _SEGMENT_ENTRY_RO  */
#define _HPAGE_TYPE_RW		0x000

/*
 * PTE type bits are rather complicated. handle_pte_fault uses pte_present,
 * pte_none and pte_file to find out the pte type WITHOUT holding the page
 * table lock. ptep_clear_flush on the other hand uses ptep_clear_flush to
 * invalidate a given pte. ipte sets the hw invalid bit and clears all tlbs
 * for the page. The page table entry is set to _PAGE_TYPE_EMPTY afterwards.
 * This change is done while holding the lock, but the intermediate step
 * of a previously valid pte with the hw invalid bit set can be observed by
 * handle_pte_fault. That makes it necessary that all valid pte types with
 * the hw invalid bit set must be distinguishable from the four pte types
 * empty, none, swap and file.
 *
 *			irxt  ipte  irxt
 * _PAGE_TYPE_EMPTY	1000   ->   1000
 * _PAGE_TYPE_NONE	1001   ->   1001
 * _PAGE_TYPE_SWAP	1011   ->   1011
 * _PAGE_TYPE_FILE	11?1   ->   11?1
 * _PAGE_TYPE_RO	0100   ->   1100
 * _PAGE_TYPE_RW	0000   ->   1000
 *
 * pte_none is true for bits combinations 1000, 1010, 1100, 1110
 * pte_present is true for bits combinations 0000, 0010, 0100, 0110, 1001
 * pte_file is true for bits combinations 1101, 1111
 * swap pte is 1011 and 0001, 0011, 0101, 0111 are invalid.
 */

#ifndef __s390x__

/* Bits in the segment table address-space-control-element */
#define _ASCE_SPACE_SWITCH	0x80000000UL	/* space switch event	    */
#define _ASCE_ORIGIN_MASK	0x7ffff000UL	/* segment table origin	    */
#define _ASCE_PRIVATE_SPACE	0x100	/* private space control	    */
#define _ASCE_ALT_EVENT		0x80	/* storage alteration event control */
#define _ASCE_TABLE_LENGTH	0x7f	/* 128 x 64 entries = 8k	    */

/* Bits in the segment table entry */
#define _SEGMENT_ENTRY_ORIGIN	0x7fffffc0UL	/* page table origin	    */
#define _SEGMENT_ENTRY_RO	0x200	/* page protection bit		    */
#define _SEGMENT_ENTRY_INV	0x20	/* invalid segment table entry	    */
#define _SEGMENT_ENTRY_COMMON	0x10	/* common segment bit		    */
#define _SEGMENT_ENTRY_PTL	0x0f	/* page table length		    */

#define _SEGMENT_ENTRY		(_SEGMENT_ENTRY_PTL)
#define _SEGMENT_ENTRY_EMPTY	(_SEGMENT_ENTRY_INV)

/* Page status table bits for virtualization */
#define RCP_ACC_BITS	0xf0000000UL
#define RCP_FP_BIT	0x08000000UL
#define RCP_PCL_BIT	0x00800000UL
#define RCP_HR_BIT	0x00400000UL
#define RCP_HC_BIT	0x00200000UL
#define RCP_GR_BIT	0x00040000UL
#define RCP_GC_BIT	0x00020000UL

/* User dirty / referenced bit for KVM's migration feature */
#define KVM_UR_BIT	0x00008000UL
#define KVM_UC_BIT	0x00004000UL

#else /* __s390x__ */

/* Bits in the segment/region table address-space-control-element */
#define _ASCE_ORIGIN		~0xfffUL/* segment table origin		    */
=======
#define _PAGE_NOEXEC	0x100		/* HW no-execute bit  */
#define _PAGE_PROTECT	0x200		/* HW read-only bit  */
#define _PAGE_INVALID	0x400		/* HW invalid bit    */
#define _PAGE_LARGE	0x800		/* Bit to mark a large pte */

/* Software bits in the page table entry */
#define _PAGE_PRESENT	0x001		/* SW pte present bit */
#define _PAGE_YOUNG	0x004		/* SW pte young bit */
#define _PAGE_DIRTY	0x008		/* SW pte dirty bit */
#define _PAGE_READ	0x010		/* SW pte read bit */
#define _PAGE_WRITE	0x020		/* SW pte write bit */
#define _PAGE_SPECIAL	0x040		/* SW associated with special page */
#define _PAGE_UNUSED	0x080		/* SW bit for pgste usage state */

#ifdef CONFIG_MEM_SOFT_DIRTY
#define _PAGE_SOFT_DIRTY 0x002		/* SW pte soft dirty bit */
#else
#define _PAGE_SOFT_DIRTY 0x000
#endif

#define _PAGE_SW_BITS	0xffUL		/* All SW bits */

#define _PAGE_SWP_EXCLUSIVE _PAGE_LARGE	/* SW pte exclusive swap bit */

/* Set of bits not changed in pte_modify */
#define _PAGE_CHG_MASK		(PAGE_MASK | _PAGE_SPECIAL | _PAGE_DIRTY | \
				 _PAGE_YOUNG | _PAGE_SOFT_DIRTY)

/*
 * Mask of bits that must not be changed with RDP. Allow only _PAGE_PROTECT
 * HW bit and all SW bits.
 */
#define _PAGE_RDP_MASK		~(_PAGE_PROTECT | _PAGE_SW_BITS)

/*
 * handle_pte_fault uses pte_present and pte_none to find out the pte type
 * WITHOUT holding the page table lock. The _PAGE_PRESENT bit is used to
 * distinguish present from not-present ptes. It is changed only with the page
 * table lock held.
 *
 * The following table gives the different possible bit combinations for
 * the pte hardware and software bits in the last 12 bits of a pte
 * (. unassigned bit, x don't care, t swap type):
 *
 *				842100000000
 *				000084210000
 *				000000008421
 *				.IR.uswrdy.p
 * empty			.10.00000000
 * swap				.11..ttttt.0
 * prot-none, clean, old	.11.xx0000.1
 * prot-none, clean, young	.11.xx0001.1
 * prot-none, dirty, old	.11.xx0010.1
 * prot-none, dirty, young	.11.xx0011.1
 * read-only, clean, old	.11.xx0100.1
 * read-only, clean, young	.01.xx0101.1
 * read-only, dirty, old	.11.xx0110.1
 * read-only, dirty, young	.01.xx0111.1
 * read-write, clean, old	.11.xx1100.1
 * read-write, clean, young	.01.xx1101.1
 * read-write, dirty, old	.10.xx1110.1
 * read-write, dirty, young	.00.xx1111.1
 * HW-bits: R read-only, I invalid
 * SW-bits: p present, y young, d dirty, r read, w write, s special,
 *	    u unused, l large
 *
 * pte_none    is true for the bit pattern .10.00000000, pte == 0x400
 * pte_swap    is true for the bit pattern .11..ooooo.0, (pte & 0x201) == 0x200
 * pte_present is true for the bit pattern .xx.xxxxxx.1, (pte & 0x001) == 0x001
 */

/* Bits in the segment/region table address-space-control-element */
#define _ASCE_ORIGIN		~0xfffUL/* region/segment table origin	    */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define _ASCE_PRIVATE_SPACE	0x100	/* private space control	    */
#define _ASCE_ALT_EVENT		0x80	/* storage alteration event control */
#define _ASCE_SPACE_SWITCH	0x40	/* space switch event		    */
#define _ASCE_REAL_SPACE	0x20	/* real space control		    */
#define _ASCE_TYPE_MASK		0x0c	/* asce table type mask		    */
#define _ASCE_TYPE_REGION1	0x0c	/* region first table type	    */
#define _ASCE_TYPE_REGION2	0x08	/* region second table type	    */
#define _ASCE_TYPE_REGION3	0x04	/* region third table type	    */
#define _ASCE_TYPE_SEGMENT	0x00	/* segment table type		    */
#define _ASCE_TABLE_LENGTH	0x03	/* region table length		    */

/* Bits in the region table entry */
#define _REGION_ENTRY_ORIGIN	~0xfffUL/* region/segment table origin	    */
<<<<<<< HEAD
#define _REGION_ENTRY_INV	0x20	/* invalid region table entry	    */
#define _REGION_ENTRY_TYPE_MASK	0x0c	/* region/segment table type mask   */
=======
#define _REGION_ENTRY_PROTECT	0x200	/* region protection bit	    */
#define _REGION_ENTRY_NOEXEC	0x100	/* region no-execute bit	    */
#define _REGION_ENTRY_OFFSET	0xc0	/* region table offset		    */
#define _REGION_ENTRY_INVALID	0x20	/* invalid region table entry	    */
#define _REGION_ENTRY_TYPE_MASK	0x0c	/* region table type mask	    */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define _REGION_ENTRY_TYPE_R1	0x0c	/* region first table type	    */
#define _REGION_ENTRY_TYPE_R2	0x08	/* region second table type	    */
#define _REGION_ENTRY_TYPE_R3	0x04	/* region third table type	    */
#define _REGION_ENTRY_LENGTH	0x03	/* region third length		    */

#define _REGION1_ENTRY		(_REGION_ENTRY_TYPE_R1 | _REGION_ENTRY_LENGTH)
<<<<<<< HEAD
#define _REGION1_ENTRY_EMPTY	(_REGION_ENTRY_TYPE_R1 | _REGION_ENTRY_INV)
#define _REGION2_ENTRY		(_REGION_ENTRY_TYPE_R2 | _REGION_ENTRY_LENGTH)
#define _REGION2_ENTRY_EMPTY	(_REGION_ENTRY_TYPE_R2 | _REGION_ENTRY_INV)
#define _REGION3_ENTRY		(_REGION_ENTRY_TYPE_R3 | _REGION_ENTRY_LENGTH)
#define _REGION3_ENTRY_EMPTY	(_REGION_ENTRY_TYPE_R3 | _REGION_ENTRY_INV)

/* Bits in the segment table entry */
#define _SEGMENT_ENTRY_ORIGIN	~0x7ffUL/* segment table origin		    */
#define _SEGMENT_ENTRY_RO	0x200	/* page protection bit		    */
#define _SEGMENT_ENTRY_INV	0x20	/* invalid segment table entry	    */

#define _SEGMENT_ENTRY		(0)
#define _SEGMENT_ENTRY_EMPTY	(_SEGMENT_ENTRY_INV)

#define _SEGMENT_ENTRY_LARGE	0x400	/* STE-format control, large page   */
#define _SEGMENT_ENTRY_CO	0x100	/* change-recording override   */

/* Page status table bits for virtualization */
#define RCP_ACC_BITS	0xf000000000000000UL
#define RCP_FP_BIT	0x0800000000000000UL
#define RCP_PCL_BIT	0x0080000000000000UL
#define RCP_HR_BIT	0x0040000000000000UL
#define RCP_HC_BIT	0x0020000000000000UL
#define RCP_GR_BIT	0x0004000000000000UL
#define RCP_GC_BIT	0x0002000000000000UL

/* User dirty / referenced bit for KVM's migration feature */
#define KVM_UR_BIT	0x0000800000000000UL
#define KVM_UC_BIT	0x0000400000000000UL

#endif /* __s390x__ */
=======
#define _REGION1_ENTRY_EMPTY	(_REGION_ENTRY_TYPE_R1 | _REGION_ENTRY_INVALID)
#define _REGION2_ENTRY		(_REGION_ENTRY_TYPE_R2 | _REGION_ENTRY_LENGTH)
#define _REGION2_ENTRY_EMPTY	(_REGION_ENTRY_TYPE_R2 | _REGION_ENTRY_INVALID)
#define _REGION3_ENTRY		(_REGION_ENTRY_TYPE_R3 | _REGION_ENTRY_LENGTH)
#define _REGION3_ENTRY_EMPTY	(_REGION_ENTRY_TYPE_R3 | _REGION_ENTRY_INVALID)

#define _REGION3_ENTRY_ORIGIN_LARGE ~0x7fffffffUL /* large page address	     */
#define _REGION3_ENTRY_DIRTY	0x2000	/* SW region dirty bit */
#define _REGION3_ENTRY_YOUNG	0x1000	/* SW region young bit */
#define _REGION3_ENTRY_LARGE	0x0400	/* RTTE-format control, large page  */
#define _REGION3_ENTRY_READ	0x0002	/* SW region read bit */
#define _REGION3_ENTRY_WRITE	0x0001	/* SW region write bit */

#ifdef CONFIG_MEM_SOFT_DIRTY
#define _REGION3_ENTRY_SOFT_DIRTY 0x4000 /* SW region soft dirty bit */
#else
#define _REGION3_ENTRY_SOFT_DIRTY 0x0000 /* SW region soft dirty bit */
#endif

#define _REGION_ENTRY_BITS	 0xfffffffffffff22fUL

/* Bits in the segment table entry */
#define _SEGMENT_ENTRY_BITS			0xfffffffffffffe33UL
#define _SEGMENT_ENTRY_HARDWARE_BITS		0xfffffffffffffe30UL
#define _SEGMENT_ENTRY_HARDWARE_BITS_LARGE	0xfffffffffff00730UL
#define _SEGMENT_ENTRY_ORIGIN_LARGE ~0xfffffUL /* large page address	    */
#define _SEGMENT_ENTRY_ORIGIN	~0x7ffUL/* page table origin		    */
#define _SEGMENT_ENTRY_PROTECT	0x200	/* segment protection bit	    */
#define _SEGMENT_ENTRY_NOEXEC	0x100	/* segment no-execute bit	    */
#define _SEGMENT_ENTRY_INVALID	0x20	/* invalid segment table entry	    */
#define _SEGMENT_ENTRY_TYPE_MASK 0x0c	/* segment table type mask	    */

#define _SEGMENT_ENTRY		(0)
#define _SEGMENT_ENTRY_EMPTY	(_SEGMENT_ENTRY_INVALID)

#define _SEGMENT_ENTRY_DIRTY	0x2000	/* SW segment dirty bit */
#define _SEGMENT_ENTRY_YOUNG	0x1000	/* SW segment young bit */
#define _SEGMENT_ENTRY_LARGE	0x0400	/* STE-format control, large page */
#define _SEGMENT_ENTRY_WRITE	0x0002	/* SW segment write bit */
#define _SEGMENT_ENTRY_READ	0x0001	/* SW segment read bit */

#ifdef CONFIG_MEM_SOFT_DIRTY
#define _SEGMENT_ENTRY_SOFT_DIRTY 0x4000 /* SW segment soft dirty bit */
#else
#define _SEGMENT_ENTRY_SOFT_DIRTY 0x0000 /* SW segment soft dirty bit */
#endif

#define _CRST_ENTRIES	2048	/* number of region/segment table entries */
#define _PAGE_ENTRIES	256	/* number of page table entries	*/

#define _CRST_TABLE_SIZE (_CRST_ENTRIES * 8)
#define _PAGE_TABLE_SIZE (_PAGE_ENTRIES * 8)

#define _REGION1_SHIFT	53
#define _REGION2_SHIFT	42
#define _REGION3_SHIFT	31
#define _SEGMENT_SHIFT	20

#define _REGION1_INDEX	(0x7ffUL << _REGION1_SHIFT)
#define _REGION2_INDEX	(0x7ffUL << _REGION2_SHIFT)
#define _REGION3_INDEX	(0x7ffUL << _REGION3_SHIFT)
#define _SEGMENT_INDEX	(0x7ffUL << _SEGMENT_SHIFT)
#define _PAGE_INDEX	(0xffUL  << _PAGE_SHIFT)

#define _REGION1_SIZE	(1UL << _REGION1_SHIFT)
#define _REGION2_SIZE	(1UL << _REGION2_SHIFT)
#define _REGION3_SIZE	(1UL << _REGION3_SHIFT)
#define _SEGMENT_SIZE	(1UL << _SEGMENT_SHIFT)

#define _REGION1_MASK	(~(_REGION1_SIZE - 1))
#define _REGION2_MASK	(~(_REGION2_SIZE - 1))
#define _REGION3_MASK	(~(_REGION3_SIZE - 1))
#define _SEGMENT_MASK	(~(_SEGMENT_SIZE - 1))

#define PMD_SHIFT	_SEGMENT_SHIFT
#define PUD_SHIFT	_REGION3_SHIFT
#define P4D_SHIFT	_REGION2_SHIFT
#define PGDIR_SHIFT	_REGION1_SHIFT

#define PMD_SIZE	_SEGMENT_SIZE
#define PUD_SIZE	_REGION3_SIZE
#define P4D_SIZE	_REGION2_SIZE
#define PGDIR_SIZE	_REGION1_SIZE

#define PMD_MASK	_SEGMENT_MASK
#define PUD_MASK	_REGION3_MASK
#define P4D_MASK	_REGION2_MASK
#define PGDIR_MASK	_REGION1_MASK

#define PTRS_PER_PTE	_PAGE_ENTRIES
#define PTRS_PER_PMD	_CRST_ENTRIES
#define PTRS_PER_PUD	_CRST_ENTRIES
#define PTRS_PER_P4D	_CRST_ENTRIES
#define PTRS_PER_PGD	_CRST_ENTRIES

/*
 * Segment table and region3 table entry encoding
 * (R = read-only, I = invalid, y = young bit):
 *				dy..R...I...wr
 * prot-none, clean, old	00..1...1...00
 * prot-none, clean, young	01..1...1...00
 * prot-none, dirty, old	10..1...1...00
 * prot-none, dirty, young	11..1...1...00
 * read-only, clean, old	00..1...1...01
 * read-only, clean, young	01..1...0...01
 * read-only, dirty, old	10..1...1...01
 * read-only, dirty, young	11..1...0...01
 * read-write, clean, old	00..1...1...11
 * read-write, clean, young	01..1...0...11
 * read-write, dirty, old	10..0...1...11
 * read-write, dirty, young	11..0...0...11
 * The segment table origin is used to distinguish empty (origin==0) from
 * read-write, old segment table entries (origin!=0)
 * HW-bits: R read-only, I invalid
 * SW-bits: y young, d dirty, r read, w write
 */

/* Page status table bits for virtualization */
#define PGSTE_ACC_BITS	0xf000000000000000UL
#define PGSTE_FP_BIT	0x0800000000000000UL
#define PGSTE_PCL_BIT	0x0080000000000000UL
#define PGSTE_HR_BIT	0x0040000000000000UL
#define PGSTE_HC_BIT	0x0020000000000000UL
#define PGSTE_GR_BIT	0x0004000000000000UL
#define PGSTE_GC_BIT	0x0002000000000000UL
#define PGSTE_UC_BIT	0x0000800000000000UL	/* user dirty (migration) */
#define PGSTE_IN_BIT	0x0000400000000000UL	/* IPTE notify bit */
#define PGSTE_VSIE_BIT	0x0000200000000000UL	/* ref'd in a shadow table */

/* Guest Page State used for virtualization */
#define _PGSTE_GPS_ZERO			0x0000000080000000UL
#define _PGSTE_GPS_NODAT		0x0000000040000000UL
#define _PGSTE_GPS_USAGE_MASK		0x0000000003000000UL
#define _PGSTE_GPS_USAGE_STABLE		0x0000000000000000UL
#define _PGSTE_GPS_USAGE_UNUSED		0x0000000001000000UL
#define _PGSTE_GPS_USAGE_POT_VOLATILE	0x0000000002000000UL
#define _PGSTE_GPS_USAGE_VOLATILE	_PGSTE_GPS_USAGE_MASK
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * A user page table pointer has the space-switch-event bit, the
 * private-space-control bit and the storage-alteration-event-control
 * bit set. A kernel page table pointer doesn't need them.
 */
#define _ASCE_USER_BITS		(_ASCE_SPACE_SWITCH | _ASCE_PRIVATE_SPACE | \
				 _ASCE_ALT_EVENT)

/*
 * Page protection definitions.
 */
<<<<<<< HEAD
#define PAGE_NONE	__pgprot(_PAGE_TYPE_NONE)
#define PAGE_RO		__pgprot(_PAGE_TYPE_RO)
#define PAGE_RW		__pgprot(_PAGE_TYPE_RW)

#define PAGE_KERNEL	PAGE_RW
#define PAGE_COPY	PAGE_RO
=======
#define PAGE_NONE	__pgprot(_PAGE_PRESENT | _PAGE_INVALID | _PAGE_PROTECT)
#define PAGE_RO		__pgprot(_PAGE_PRESENT | _PAGE_READ | \
				 _PAGE_NOEXEC  | _PAGE_INVALID | _PAGE_PROTECT)
#define PAGE_RX		__pgprot(_PAGE_PRESENT | _PAGE_READ | \
				 _PAGE_INVALID | _PAGE_PROTECT)
#define PAGE_RW		__pgprot(_PAGE_PRESENT | _PAGE_READ | _PAGE_WRITE | \
				 _PAGE_NOEXEC  | _PAGE_INVALID | _PAGE_PROTECT)
#define PAGE_RWX	__pgprot(_PAGE_PRESENT | _PAGE_READ | _PAGE_WRITE | \
				 _PAGE_INVALID | _PAGE_PROTECT)

#define PAGE_SHARED	__pgprot(_PAGE_PRESENT | _PAGE_READ | _PAGE_WRITE | \
				 _PAGE_YOUNG | _PAGE_DIRTY | _PAGE_NOEXEC)
#define PAGE_KERNEL	__pgprot(_PAGE_PRESENT | _PAGE_READ | _PAGE_WRITE | \
				 _PAGE_YOUNG | _PAGE_DIRTY | _PAGE_NOEXEC)
#define PAGE_KERNEL_RO	__pgprot(_PAGE_PRESENT | _PAGE_READ | _PAGE_YOUNG | \
				 _PAGE_PROTECT | _PAGE_NOEXEC)
#define PAGE_KERNEL_EXEC __pgprot(_PAGE_PRESENT | _PAGE_READ | _PAGE_WRITE | \
				  _PAGE_YOUNG |	_PAGE_DIRTY)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * On s390 the page table entry has an invalid bit and a read-only bit.
 * Read permission implies execute permission and write permission
 * implies read permission.
 */
         /*xwr*/
<<<<<<< HEAD
#define __P000	PAGE_NONE
#define __P001	PAGE_RO
#define __P010	PAGE_RO
#define __P011	PAGE_RO
#define __P100	PAGE_RO
#define __P101	PAGE_RO
#define __P110	PAGE_RO
#define __P111	PAGE_RO

#define __S000	PAGE_NONE
#define __S001	PAGE_RO
#define __S010	PAGE_RW
#define __S011	PAGE_RW
#define __S100	PAGE_RO
#define __S101	PAGE_RO
#define __S110	PAGE_RW
#define __S111	PAGE_RW

static inline int mm_exclusive(struct mm_struct *mm)
{
	return likely(mm == current->active_mm &&
		      atomic_read(&mm->context.attach_count) <= 1);
}
=======

/*
 * Segment entry (large page) protection definitions.
 */
#define SEGMENT_NONE	__pgprot(_SEGMENT_ENTRY_INVALID | \
				 _SEGMENT_ENTRY_PROTECT)
#define SEGMENT_RO	__pgprot(_SEGMENT_ENTRY_PROTECT | \
				 _SEGMENT_ENTRY_READ | \
				 _SEGMENT_ENTRY_NOEXEC)
#define SEGMENT_RX	__pgprot(_SEGMENT_ENTRY_PROTECT | \
				 _SEGMENT_ENTRY_READ)
#define SEGMENT_RW	__pgprot(_SEGMENT_ENTRY_READ | \
				 _SEGMENT_ENTRY_WRITE | \
				 _SEGMENT_ENTRY_NOEXEC)
#define SEGMENT_RWX	__pgprot(_SEGMENT_ENTRY_READ | \
				 _SEGMENT_ENTRY_WRITE)
#define SEGMENT_KERNEL	__pgprot(_SEGMENT_ENTRY |	\
				 _SEGMENT_ENTRY_LARGE |	\
				 _SEGMENT_ENTRY_READ |	\
				 _SEGMENT_ENTRY_WRITE | \
				 _SEGMENT_ENTRY_YOUNG | \
				 _SEGMENT_ENTRY_DIRTY | \
				 _SEGMENT_ENTRY_NOEXEC)
#define SEGMENT_KERNEL_RO __pgprot(_SEGMENT_ENTRY |	\
				 _SEGMENT_ENTRY_LARGE |	\
				 _SEGMENT_ENTRY_READ |	\
				 _SEGMENT_ENTRY_YOUNG |	\
				 _SEGMENT_ENTRY_PROTECT | \
				 _SEGMENT_ENTRY_NOEXEC)
#define SEGMENT_KERNEL_EXEC __pgprot(_SEGMENT_ENTRY |	\
				 _SEGMENT_ENTRY_LARGE |	\
				 _SEGMENT_ENTRY_READ |	\
				 _SEGMENT_ENTRY_WRITE | \
				 _SEGMENT_ENTRY_YOUNG |	\
				 _SEGMENT_ENTRY_DIRTY)

/*
 * Region3 entry (large page) protection definitions.
 */

#define REGION3_KERNEL	__pgprot(_REGION_ENTRY_TYPE_R3 | \
				 _REGION3_ENTRY_LARGE |	 \
				 _REGION3_ENTRY_READ |	 \
				 _REGION3_ENTRY_WRITE |	 \
				 _REGION3_ENTRY_YOUNG |	 \
				 _REGION3_ENTRY_DIRTY | \
				 _REGION_ENTRY_NOEXEC)
#define REGION3_KERNEL_RO __pgprot(_REGION_ENTRY_TYPE_R3 | \
				   _REGION3_ENTRY_LARGE |  \
				   _REGION3_ENTRY_READ |   \
				   _REGION3_ENTRY_YOUNG |  \
				   _REGION_ENTRY_PROTECT | \
				   _REGION_ENTRY_NOEXEC)
#define REGION3_KERNEL_EXEC __pgprot(_REGION_ENTRY_TYPE_R3 | \
				 _REGION3_ENTRY_LARGE |	 \
				 _REGION3_ENTRY_READ |	 \
				 _REGION3_ENTRY_WRITE |	 \
				 _REGION3_ENTRY_YOUNG |	 \
				 _REGION3_ENTRY_DIRTY)

static inline bool mm_p4d_folded(struct mm_struct *mm)
{
	return mm->context.asce_limit <= _REGION1_SIZE;
}
#define mm_p4d_folded(mm) mm_p4d_folded(mm)

static inline bool mm_pud_folded(struct mm_struct *mm)
{
	return mm->context.asce_limit <= _REGION2_SIZE;
}
#define mm_pud_folded(mm) mm_pud_folded(mm)

static inline bool mm_pmd_folded(struct mm_struct *mm)
{
	return mm->context.asce_limit <= _REGION3_SIZE;
}
#define mm_pmd_folded(mm) mm_pmd_folded(mm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int mm_has_pgste(struct mm_struct *mm)
{
#ifdef CONFIG_PGSTE
	if (unlikely(mm->context.has_pgste))
		return 1;
#endif
	return 0;
}
<<<<<<< HEAD
/*
 * pgd/pmd/pte query functions
 */
#ifndef __s390x__

static inline int pgd_present(pgd_t pgd) { return 1; }
static inline int pgd_none(pgd_t pgd)    { return 0; }
static inline int pgd_bad(pgd_t pgd)     { return 0; }

static inline int pud_present(pud_t pud) { return 1; }
static inline int pud_none(pud_t pud)	 { return 0; }
static inline int pud_bad(pud_t pud)	 { return 0; }

#else /* __s390x__ */

static inline int pgd_present(pgd_t pgd)
{
	if ((pgd_val(pgd) & _REGION_ENTRY_TYPE_MASK) < _REGION_ENTRY_TYPE_R2)
=======

static inline int mm_is_protected(struct mm_struct *mm)
{
#ifdef CONFIG_PGSTE
	if (unlikely(atomic_read(&mm->context.protected_count)))
		return 1;
#endif
	return 0;
}

static inline int mm_alloc_pgste(struct mm_struct *mm)
{
#ifdef CONFIG_PGSTE
	if (unlikely(mm->context.alloc_pgste))
		return 1;
#endif
	return 0;
}

static inline pte_t clear_pte_bit(pte_t pte, pgprot_t prot)
{
	return __pte(pte_val(pte) & ~pgprot_val(prot));
}

static inline pte_t set_pte_bit(pte_t pte, pgprot_t prot)
{
	return __pte(pte_val(pte) | pgprot_val(prot));
}

static inline pmd_t clear_pmd_bit(pmd_t pmd, pgprot_t prot)
{
	return __pmd(pmd_val(pmd) & ~pgprot_val(prot));
}

static inline pmd_t set_pmd_bit(pmd_t pmd, pgprot_t prot)
{
	return __pmd(pmd_val(pmd) | pgprot_val(prot));
}

static inline pud_t clear_pud_bit(pud_t pud, pgprot_t prot)
{
	return __pud(pud_val(pud) & ~pgprot_val(prot));
}

static inline pud_t set_pud_bit(pud_t pud, pgprot_t prot)
{
	return __pud(pud_val(pud) | pgprot_val(prot));
}

/*
 * In the case that a guest uses storage keys
 * faults should no longer be backed by zero pages
 */
#define mm_forbids_zeropage mm_has_pgste
static inline int mm_uses_skeys(struct mm_struct *mm)
{
#ifdef CONFIG_PGSTE
	if (mm->context.uses_skeys)
		return 1;
#endif
	return 0;
}

static inline void csp(unsigned int *ptr, unsigned int old, unsigned int new)
{
	union register_pair r1 = { .even = old, .odd = new, };
	unsigned long address = (unsigned long)ptr | 1;

	asm volatile(
		"	csp	%[r1],%[address]"
		: [r1] "+&d" (r1.pair), "+m" (*ptr)
		: [address] "d" (address)
		: "cc");
}

static inline void cspg(unsigned long *ptr, unsigned long old, unsigned long new)
{
	union register_pair r1 = { .even = old, .odd = new, };
	unsigned long address = (unsigned long)ptr | 1;

	asm volatile(
		"	cspg	%[r1],%[address]"
		: [r1] "+&d" (r1.pair), "+m" (*ptr)
		: [address] "d" (address)
		: "cc");
}

#define CRDTE_DTT_PAGE		0x00UL
#define CRDTE_DTT_SEGMENT	0x10UL
#define CRDTE_DTT_REGION3	0x14UL
#define CRDTE_DTT_REGION2	0x18UL
#define CRDTE_DTT_REGION1	0x1cUL

static inline void crdte(unsigned long old, unsigned long new,
			 unsigned long *table, unsigned long dtt,
			 unsigned long address, unsigned long asce)
{
	union register_pair r1 = { .even = old, .odd = new, };
	union register_pair r2 = { .even = __pa(table) | dtt, .odd = address, };

	asm volatile(".insn rrf,0xb98f0000,%[r1],%[r2],%[asce],0"
		     : [r1] "+&d" (r1.pair)
		     : [r2] "d" (r2.pair), [asce] "a" (asce)
		     : "memory", "cc");
}

/*
 * pgd/p4d/pud/pmd/pte query functions
 */
static inline int pgd_folded(pgd_t pgd)
{
	return (pgd_val(pgd) & _REGION_ENTRY_TYPE_MASK) < _REGION_ENTRY_TYPE_R1;
}

static inline int pgd_present(pgd_t pgd)
{
	if (pgd_folded(pgd))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	return (pgd_val(pgd) & _REGION_ENTRY_ORIGIN) != 0UL;
}

static inline int pgd_none(pgd_t pgd)
{
<<<<<<< HEAD
	if ((pgd_val(pgd) & _REGION_ENTRY_TYPE_MASK) < _REGION_ENTRY_TYPE_R2)
		return 0;
	return (pgd_val(pgd) & _REGION_ENTRY_INV) != 0UL;
=======
	if (pgd_folded(pgd))
		return 0;
	return (pgd_val(pgd) & _REGION_ENTRY_INVALID) != 0UL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int pgd_bad(pgd_t pgd)
{
<<<<<<< HEAD
	/*
	 * With dynamic page table levels the pgd can be a region table
	 * entry or a segment table entry. Check for the bit that are
	 * invalid for either table entry.
	 */
	unsigned long mask =
		~_SEGMENT_ENTRY_ORIGIN & ~_REGION_ENTRY_INV &
		~_REGION_ENTRY_TYPE_MASK & ~_REGION_ENTRY_LENGTH;
	return (pgd_val(pgd) & mask) != 0;
=======
	if ((pgd_val(pgd) & _REGION_ENTRY_TYPE_MASK) < _REGION_ENTRY_TYPE_R1)
		return 0;
	return (pgd_val(pgd) & ~_REGION_ENTRY_BITS) != 0;
}

static inline unsigned long pgd_pfn(pgd_t pgd)
{
	unsigned long origin_mask;

	origin_mask = _REGION_ENTRY_ORIGIN;
	return (pgd_val(pgd) & origin_mask) >> PAGE_SHIFT;
}

static inline int p4d_folded(p4d_t p4d)
{
	return (p4d_val(p4d) & _REGION_ENTRY_TYPE_MASK) < _REGION_ENTRY_TYPE_R2;
}

static inline int p4d_present(p4d_t p4d)
{
	if (p4d_folded(p4d))
		return 1;
	return (p4d_val(p4d) & _REGION_ENTRY_ORIGIN) != 0UL;
}

static inline int p4d_none(p4d_t p4d)
{
	if (p4d_folded(p4d))
		return 0;
	return p4d_val(p4d) == _REGION2_ENTRY_EMPTY;
}

static inline unsigned long p4d_pfn(p4d_t p4d)
{
	unsigned long origin_mask;

	origin_mask = _REGION_ENTRY_ORIGIN;
	return (p4d_val(p4d) & origin_mask) >> PAGE_SHIFT;
}

static inline int pud_folded(pud_t pud)
{
	return (pud_val(pud) & _REGION_ENTRY_TYPE_MASK) < _REGION_ENTRY_TYPE_R3;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int pud_present(pud_t pud)
{
<<<<<<< HEAD
	if ((pud_val(pud) & _REGION_ENTRY_TYPE_MASK) < _REGION_ENTRY_TYPE_R3)
=======
	if (pud_folded(pud))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	return (pud_val(pud) & _REGION_ENTRY_ORIGIN) != 0UL;
}

static inline int pud_none(pud_t pud)
{
<<<<<<< HEAD
	if ((pud_val(pud) & _REGION_ENTRY_TYPE_MASK) < _REGION_ENTRY_TYPE_R3)
		return 0;
	return (pud_val(pud) & _REGION_ENTRY_INV) != 0UL;
}

static inline int pud_bad(pud_t pud)
{
	/*
	 * With dynamic page table levels the pud can be a region table
	 * entry or a segment table entry. Check for the bit that are
	 * invalid for either table entry.
	 */
	unsigned long mask =
		~_SEGMENT_ENTRY_ORIGIN & ~_REGION_ENTRY_INV &
		~_REGION_ENTRY_TYPE_MASK & ~_REGION_ENTRY_LENGTH;
	return (pud_val(pud) & mask) != 0;
}

#endif /* __s390x__ */

static inline int pmd_present(pmd_t pmd)
{
	return (pmd_val(pmd) & _SEGMENT_ENTRY_ORIGIN) != 0UL;
}

static inline int pmd_none(pmd_t pmd)
{
	return (pmd_val(pmd) & _SEGMENT_ENTRY_INV) != 0UL;
=======
	if (pud_folded(pud))
		return 0;
	return pud_val(pud) == _REGION3_ENTRY_EMPTY;
}

#define pud_leaf pud_leaf
static inline bool pud_leaf(pud_t pud)
{
	if ((pud_val(pud) & _REGION_ENTRY_TYPE_MASK) != _REGION_ENTRY_TYPE_R3)
		return 0;
	return !!(pud_val(pud) & _REGION3_ENTRY_LARGE);
}

#define pmd_leaf pmd_leaf
static inline bool pmd_leaf(pmd_t pmd)
{
	return (pmd_val(pmd) & _SEGMENT_ENTRY_LARGE) != 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int pmd_bad(pmd_t pmd)
{
<<<<<<< HEAD
	unsigned long mask = ~_SEGMENT_ENTRY_ORIGIN & ~_SEGMENT_ENTRY_INV;
	return (pmd_val(pmd) & mask) != _SEGMENT_ENTRY;
}

static inline int pte_none(pte_t pte)
{
	return (pte_val(pte) & _PAGE_INVALID) && !(pte_val(pte) & _PAGE_SWT);
=======
	if ((pmd_val(pmd) & _SEGMENT_ENTRY_TYPE_MASK) > 0 || pmd_leaf(pmd))
		return 1;
	return (pmd_val(pmd) & ~_SEGMENT_ENTRY_BITS) != 0;
}

static inline int pud_bad(pud_t pud)
{
	unsigned long type = pud_val(pud) & _REGION_ENTRY_TYPE_MASK;

	if (type > _REGION_ENTRY_TYPE_R3 || pud_leaf(pud))
		return 1;
	if (type < _REGION_ENTRY_TYPE_R3)
		return 0;
	return (pud_val(pud) & ~_REGION_ENTRY_BITS) != 0;
}

static inline int p4d_bad(p4d_t p4d)
{
	unsigned long type = p4d_val(p4d) & _REGION_ENTRY_TYPE_MASK;

	if (type > _REGION_ENTRY_TYPE_R2)
		return 1;
	if (type < _REGION_ENTRY_TYPE_R2)
		return 0;
	return (p4d_val(p4d) & ~_REGION_ENTRY_BITS) != 0;
}

static inline int pmd_present(pmd_t pmd)
{
	return pmd_val(pmd) != _SEGMENT_ENTRY_EMPTY;
}

static inline int pmd_none(pmd_t pmd)
{
	return pmd_val(pmd) == _SEGMENT_ENTRY_EMPTY;
}

#define pmd_write pmd_write
static inline int pmd_write(pmd_t pmd)
{
	return (pmd_val(pmd) & _SEGMENT_ENTRY_WRITE) != 0;
}

#define pud_write pud_write
static inline int pud_write(pud_t pud)
{
	return (pud_val(pud) & _REGION3_ENTRY_WRITE) != 0;
}

#define pmd_dirty pmd_dirty
static inline int pmd_dirty(pmd_t pmd)
{
	return (pmd_val(pmd) & _SEGMENT_ENTRY_DIRTY) != 0;
}

#define pmd_young pmd_young
static inline int pmd_young(pmd_t pmd)
{
	return (pmd_val(pmd) & _SEGMENT_ENTRY_YOUNG) != 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int pte_present(pte_t pte)
{
<<<<<<< HEAD
	unsigned long mask = _PAGE_RO | _PAGE_INVALID | _PAGE_SWT | _PAGE_SWX;
	return (pte_val(pte) & mask) == _PAGE_TYPE_NONE ||
		(!(pte_val(pte) & _PAGE_INVALID) &&
		 !(pte_val(pte) & _PAGE_SWT));
}

static inline int pte_file(pte_t pte)
{
	unsigned long mask = _PAGE_RO | _PAGE_INVALID | _PAGE_SWT;
	return (pte_val(pte) & mask) == _PAGE_TYPE_FILE;
=======
	/* Bit pattern: (pte & 0x001) == 0x001 */
	return (pte_val(pte) & _PAGE_PRESENT) != 0;
}

static inline int pte_none(pte_t pte)
{
	/* Bit pattern: pte == 0x400 */
	return pte_val(pte) == _PAGE_INVALID;
}

static inline int pte_swap(pte_t pte)
{
	/* Bit pattern: (pte & 0x201) == 0x200 */
	return (pte_val(pte) & (_PAGE_PROTECT | _PAGE_PRESENT))
		== _PAGE_PROTECT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int pte_special(pte_t pte)
{
	return (pte_val(pte) & _PAGE_SPECIAL);
}

#define __HAVE_ARCH_PTE_SAME
static inline int pte_same(pte_t a, pte_t b)
{
	return pte_val(a) == pte_val(b);
}

<<<<<<< HEAD
static inline pgste_t pgste_get_lock(pte_t *ptep)
{
	unsigned long new = 0;
#ifdef CONFIG_PGSTE
	unsigned long old;

	preempt_disable();
	asm(
		"	lg	%0,%2\n"
		"0:	lgr	%1,%0\n"
		"	nihh	%0,0xff7f\n"	/* clear RCP_PCL_BIT in old */
		"	oihh	%1,0x0080\n"	/* set RCP_PCL_BIT in new */
		"	csg	%0,%1,%2\n"
		"	jl	0b\n"
		: "=&d" (old), "=&d" (new), "=Q" (ptep[PTRS_PER_PTE])
		: "Q" (ptep[PTRS_PER_PTE]) : "cc");
#endif
	return __pgste(new);
}

static inline void pgste_set_unlock(pte_t *ptep, pgste_t pgste)
{
#ifdef CONFIG_PGSTE
	asm(
		"	nihh	%1,0xff7f\n"	/* clear RCP_PCL_BIT */
		"	stg	%1,%0\n"
		: "=Q" (ptep[PTRS_PER_PTE])
		: "d" (pgste_val(pgste)), "Q" (ptep[PTRS_PER_PTE]) : "cc");
	preempt_enable();
#endif
}

static inline pgste_t pgste_update_all(pte_t *ptep, pgste_t pgste)
{
#ifdef CONFIG_PGSTE
	unsigned long address, bits;
	unsigned char skey;

	if (!pte_present(*ptep))
		return pgste;
	address = pte_val(*ptep) & PAGE_MASK;
	skey = page_get_storage_key(address);
	bits = skey & (_PAGE_CHANGED | _PAGE_REFERENCED);
	/* Clear page changed & referenced bit in the storage key */
	if (bits & _PAGE_CHANGED)
		page_set_storage_key(address, skey ^ bits, 1);
	else if (bits)
		page_reset_referenced(address);
	/* Transfer page changed & referenced bit to guest bits in pgste */
	pgste_val(pgste) |= bits << 48;		/* RCP_GR_BIT & RCP_GC_BIT */
	/* Get host changed & referenced bits from pgste */
	bits |= (pgste_val(pgste) & (RCP_HR_BIT | RCP_HC_BIT)) >> 52;
	/* Clear host bits in pgste. */
	pgste_val(pgste) &= ~(RCP_HR_BIT | RCP_HC_BIT);
	pgste_val(pgste) &= ~(RCP_ACC_BITS | RCP_FP_BIT);
	/* Copy page access key and fetch protection bit to pgste */
	pgste_val(pgste) |=
		(unsigned long) (skey & (_PAGE_ACC_BITS | _PAGE_FP_BIT)) << 56;
	/* Transfer changed and referenced to kvm user bits */
	pgste_val(pgste) |= bits << 45;		/* KVM_UR_BIT & KVM_UC_BIT */
	/* Transfer changed & referenced to pte sofware bits */
	pte_val(*ptep) |= bits << 1;		/* _PAGE_SWR & _PAGE_SWC */
#endif
	return pgste;

}

static inline pgste_t pgste_update_young(pte_t *ptep, pgste_t pgste)
{
#ifdef CONFIG_PGSTE
	int young;

	if (!pte_present(*ptep))
		return pgste;
	young = page_reset_referenced(pte_val(*ptep) & PAGE_MASK);
	/* Transfer page referenced bit to pte software bit (host view) */
	if (young || (pgste_val(pgste) & RCP_HR_BIT))
		pte_val(*ptep) |= _PAGE_SWR;
	/* Clear host referenced bit in pgste. */
	pgste_val(pgste) &= ~RCP_HR_BIT;
	/* Transfer page referenced bit to guest bit in pgste */
	pgste_val(pgste) |= (unsigned long) young << 50; /* set RCP_GR_BIT */
#endif
	return pgste;

}

static inline void pgste_set_pte(pte_t *ptep, pgste_t pgste, pte_t entry)
{
#ifdef CONFIG_PGSTE
	unsigned long address;
	unsigned long okey, nkey;

	if (!pte_present(entry))
		return;
	address = pte_val(entry) & PAGE_MASK;
	okey = nkey = page_get_storage_key(address);
	nkey &= ~(_PAGE_ACC_BITS | _PAGE_FP_BIT);
	/* Set page access key and fetch protection bit from pgste */
	nkey |= (pgste_val(pgste) & (RCP_ACC_BITS | RCP_FP_BIT)) >> 56;
	if (okey != nkey)
		page_set_storage_key(address, nkey, 1);
#endif
}

/**
 * struct gmap_struct - guest address space
 * @mm: pointer to the parent mm_struct
 * @table: pointer to the page directory
 * @asce: address space control element for gmap page table
 * @crst_list: list of all crst tables used in the guest address space
 */
struct gmap {
	struct list_head list;
	struct mm_struct *mm;
	unsigned long *table;
	unsigned long asce;
	struct list_head crst_list;
};

/**
 * struct gmap_rmap - reverse mapping for segment table entries
 * @next: pointer to the next gmap_rmap structure in the list
 * @entry: pointer to a segment table entry
 */
struct gmap_rmap {
	struct list_head list;
	unsigned long *entry;
};

/**
 * struct gmap_pgtable - gmap information attached to a page table
 * @vmaddr: address of the 1MB segment in the process virtual memory
 * @mapper: list of segment table entries maping a page table
 */
struct gmap_pgtable {
	unsigned long vmaddr;
	struct list_head mapper;
};

struct gmap *gmap_alloc(struct mm_struct *mm);
void gmap_free(struct gmap *gmap);
void gmap_enable(struct gmap *gmap);
void gmap_disable(struct gmap *gmap);
int gmap_map_segment(struct gmap *gmap, unsigned long from,
		     unsigned long to, unsigned long length);
int gmap_unmap_segment(struct gmap *gmap, unsigned long to, unsigned long len);
unsigned long __gmap_fault(unsigned long address, struct gmap *);
unsigned long gmap_fault(unsigned long address, struct gmap *);
void gmap_discard(unsigned long from, unsigned long to, struct gmap *);

/*
 * Certain architectures need to do special things when PTEs
 * within a page table are directly modified.  Thus, the following
 * hook is made available.
 */
static inline void set_pte_at(struct mm_struct *mm, unsigned long addr,
			      pte_t *ptep, pte_t entry)
{
	pgste_t pgste;

	if (mm_has_pgste(mm)) {
		pgste = pgste_get_lock(ptep);
		pgste_set_pte(ptep, pgste, entry);
		*ptep = entry;
		pgste_set_unlock(ptep, pgste);
	} else
		*ptep = entry;
=======
#ifdef CONFIG_NUMA_BALANCING
static inline int pte_protnone(pte_t pte)
{
	return pte_present(pte) && !(pte_val(pte) & _PAGE_READ);
}

static inline int pmd_protnone(pmd_t pmd)
{
	/* pmd_leaf(pmd) implies pmd_present(pmd) */
	return pmd_leaf(pmd) && !(pmd_val(pmd) & _SEGMENT_ENTRY_READ);
}
#endif

static inline int pte_swp_exclusive(pte_t pte)
{
	return pte_val(pte) & _PAGE_SWP_EXCLUSIVE;
}

static inline pte_t pte_swp_mkexclusive(pte_t pte)
{
	return set_pte_bit(pte, __pgprot(_PAGE_SWP_EXCLUSIVE));
}

static inline pte_t pte_swp_clear_exclusive(pte_t pte)
{
	return clear_pte_bit(pte, __pgprot(_PAGE_SWP_EXCLUSIVE));
}

static inline int pte_soft_dirty(pte_t pte)
{
	return pte_val(pte) & _PAGE_SOFT_DIRTY;
}
#define pte_swp_soft_dirty pte_soft_dirty

static inline pte_t pte_mksoft_dirty(pte_t pte)
{
	return set_pte_bit(pte, __pgprot(_PAGE_SOFT_DIRTY));
}
#define pte_swp_mksoft_dirty pte_mksoft_dirty

static inline pte_t pte_clear_soft_dirty(pte_t pte)
{
	return clear_pte_bit(pte, __pgprot(_PAGE_SOFT_DIRTY));
}
#define pte_swp_clear_soft_dirty pte_clear_soft_dirty

static inline int pmd_soft_dirty(pmd_t pmd)
{
	return pmd_val(pmd) & _SEGMENT_ENTRY_SOFT_DIRTY;
}

static inline pmd_t pmd_mksoft_dirty(pmd_t pmd)
{
	return set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_SOFT_DIRTY));
}

static inline pmd_t pmd_clear_soft_dirty(pmd_t pmd)
{
	return clear_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_SOFT_DIRTY));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * query functions pte_write/pte_dirty/pte_young only work if
 * pte_present() is true. Undefined behaviour if not..
 */
static inline int pte_write(pte_t pte)
{
<<<<<<< HEAD
	return (pte_val(pte) & _PAGE_RO) == 0;
=======
	return (pte_val(pte) & _PAGE_WRITE) != 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int pte_dirty(pte_t pte)
{
<<<<<<< HEAD
#ifdef CONFIG_PGSTE
	if (pte_val(pte) & _PAGE_SWC)
		return 1;
#endif
	return 0;
=======
	return (pte_val(pte) & _PAGE_DIRTY) != 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int pte_young(pte_t pte)
{
<<<<<<< HEAD
#ifdef CONFIG_PGSTE
	if (pte_val(pte) & _PAGE_SWR)
		return 1;
#endif
	return 0;
=======
	return (pte_val(pte) & _PAGE_YOUNG) != 0;
}

#define __HAVE_ARCH_PTE_UNUSED
static inline int pte_unused(pte_t pte)
{
	return pte_val(pte) & _PAGE_UNUSED;
}

/*
 * Extract the pgprot value from the given pte while at the same time making it
 * usable for kernel address space mappings where fault driven dirty and
 * young/old accounting is not supported, i.e _PAGE_PROTECT and _PAGE_INVALID
 * must not be set.
 */
static inline pgprot_t pte_pgprot(pte_t pte)
{
	unsigned long pte_flags = pte_val(pte) & _PAGE_CHG_MASK;

	if (pte_write(pte))
		pte_flags |= pgprot_val(PAGE_KERNEL);
	else
		pte_flags |= pgprot_val(PAGE_KERNEL_RO);
	pte_flags |= pte_val(pte) & mio_wb_bit_mask;

	return __pgprot(pte_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * pgd/pmd/pte modification functions
 */

<<<<<<< HEAD
static inline void pgd_clear(pgd_t *pgd)
{
#ifdef __s390x__
	if ((pgd_val(*pgd) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R2)
		pgd_val(*pgd) = _REGION2_ENTRY_EMPTY;
#endif
=======
static inline void set_pgd(pgd_t *pgdp, pgd_t pgd)
{
	WRITE_ONCE(*pgdp, pgd);
}

static inline void set_p4d(p4d_t *p4dp, p4d_t p4d)
{
	WRITE_ONCE(*p4dp, p4d);
}

static inline void set_pud(pud_t *pudp, pud_t pud)
{
	WRITE_ONCE(*pudp, pud);
}

static inline void set_pmd(pmd_t *pmdp, pmd_t pmd)
{
	WRITE_ONCE(*pmdp, pmd);
}

static inline void set_pte(pte_t *ptep, pte_t pte)
{
	WRITE_ONCE(*ptep, pte);
}

static inline void pgd_clear(pgd_t *pgd)
{
	if ((pgd_val(*pgd) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R1)
		set_pgd(pgd, __pgd(_REGION1_ENTRY_EMPTY));
}

static inline void p4d_clear(p4d_t *p4d)
{
	if ((p4d_val(*p4d) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R2)
		set_p4d(p4d, __p4d(_REGION2_ENTRY_EMPTY));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void pud_clear(pud_t *pud)
{
<<<<<<< HEAD
#ifdef __s390x__
	if ((pud_val(*pud) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R3)
		pud_val(*pud) = _REGION3_ENTRY_EMPTY;
#endif
=======
	if ((pud_val(*pud) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R3)
		set_pud(pud, __pud(_REGION3_ENTRY_EMPTY));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void pmd_clear(pmd_t *pmdp)
{
<<<<<<< HEAD
	pmd_val(*pmdp) = _SEGMENT_ENTRY_EMPTY;
=======
	set_pmd(pmdp, __pmd(_SEGMENT_ENTRY_EMPTY));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void pte_clear(struct mm_struct *mm, unsigned long addr, pte_t *ptep)
{
<<<<<<< HEAD
	pte_val(*ptep) = _PAGE_TYPE_EMPTY;
=======
	set_pte(ptep, __pte(_PAGE_INVALID));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * The following pte modification functions only work if
 * pte_present() is true. Undefined behaviour if not..
 */
static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
<<<<<<< HEAD
	pte_val(pte) &= _PAGE_CHG_MASK;
	pte_val(pte) |= pgprot_val(newprot);
=======
	pte = clear_pte_bit(pte, __pgprot(~_PAGE_CHG_MASK));
	pte = set_pte_bit(pte, newprot);
	/*
	 * newprot for PAGE_NONE, PAGE_RO, PAGE_RX, PAGE_RW and PAGE_RWX
	 * has the invalid bit set, clear it again for readable, young pages
	 */
	if ((pte_val(pte) & _PAGE_YOUNG) && (pte_val(pte) & _PAGE_READ))
		pte = clear_pte_bit(pte, __pgprot(_PAGE_INVALID));
	/*
	 * newprot for PAGE_RO, PAGE_RX, PAGE_RW and PAGE_RWX has the page
	 * protection bit set, clear it again for writable, dirty pages
	 */
	if ((pte_val(pte) & _PAGE_DIRTY) && (pte_val(pte) & _PAGE_WRITE))
		pte = clear_pte_bit(pte, __pgprot(_PAGE_PROTECT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return pte;
}

static inline pte_t pte_wrprotect(pte_t pte)
{
<<<<<<< HEAD
	/* Do not clobber _PAGE_TYPE_NONE pages!  */
	if (!(pte_val(pte) & _PAGE_INVALID))
		pte_val(pte) |= _PAGE_RO;
	return pte;
}

static inline pte_t pte_mkwrite(pte_t pte)
{
	pte_val(pte) &= ~_PAGE_RO;
=======
	pte = clear_pte_bit(pte, __pgprot(_PAGE_WRITE));
	return set_pte_bit(pte, __pgprot(_PAGE_PROTECT));
}

static inline pte_t pte_mkwrite_novma(pte_t pte)
{
	pte = set_pte_bit(pte, __pgprot(_PAGE_WRITE));
	if (pte_val(pte) & _PAGE_DIRTY)
		pte = clear_pte_bit(pte, __pgprot(_PAGE_PROTECT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return pte;
}

static inline pte_t pte_mkclean(pte_t pte)
{
<<<<<<< HEAD
#ifdef CONFIG_PGSTE
	pte_val(pte) &= ~_PAGE_SWC;
#endif
	return pte;
=======
	pte = clear_pte_bit(pte, __pgprot(_PAGE_DIRTY));
	return set_pte_bit(pte, __pgprot(_PAGE_PROTECT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline pte_t pte_mkdirty(pte_t pte)
{
<<<<<<< HEAD
=======
	pte = set_pte_bit(pte, __pgprot(_PAGE_DIRTY | _PAGE_SOFT_DIRTY));
	if (pte_val(pte) & _PAGE_WRITE)
		pte = clear_pte_bit(pte, __pgprot(_PAGE_PROTECT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return pte;
}

static inline pte_t pte_mkold(pte_t pte)
{
<<<<<<< HEAD
#ifdef CONFIG_PGSTE
	pte_val(pte) &= ~_PAGE_SWR;
#endif
	return pte;
=======
	pte = clear_pte_bit(pte, __pgprot(_PAGE_YOUNG));
	return set_pte_bit(pte, __pgprot(_PAGE_INVALID));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline pte_t pte_mkyoung(pte_t pte)
{
<<<<<<< HEAD
=======
	pte = set_pte_bit(pte, __pgprot(_PAGE_YOUNG));
	if (pte_val(pte) & _PAGE_READ)
		pte = clear_pte_bit(pte, __pgprot(_PAGE_INVALID));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return pte;
}

static inline pte_t pte_mkspecial(pte_t pte)
{
<<<<<<< HEAD
	pte_val(pte) |= _PAGE_SPECIAL;
	return pte;
=======
	return set_pte_bit(pte, __pgprot(_PAGE_SPECIAL));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_HUGETLB_PAGE
static inline pte_t pte_mkhuge(pte_t pte)
{
<<<<<<< HEAD
	/*
	 * PROT_NONE needs to be remapped from the pte type to the ste type.
	 * The HW invalid bit is also different for pte and ste. The pte
	 * invalid bit happens to be the same as the ste _SEGMENT_ENTRY_LARGE
	 * bit, so we don't have to clear it.
	 */
	if (pte_val(pte) & _PAGE_INVALID) {
		if (pte_val(pte) & _PAGE_SWT)
			pte_val(pte) |= _HPAGE_TYPE_NONE;
		pte_val(pte) |= _SEGMENT_ENTRY_INV;
	}
	/*
	 * Clear SW pte bits SWT and SWX, there are no SW bits in a segment
	 * table entry.
	 */
	pte_val(pte) &= ~(_PAGE_SWT | _PAGE_SWX);
	/*
	 * Also set the change-override bit because we don't need dirty bit
	 * tracking for hugetlbfs pages.
	 */
	pte_val(pte) |= (_SEGMENT_ENTRY_LARGE | _SEGMENT_ENTRY_CO);
	return pte;
}
#endif

/*
 * Get (and clear) the user dirty bit for a pte.
 */
static inline int ptep_test_and_clear_user_dirty(struct mm_struct *mm,
						 pte_t *ptep)
{
	pgste_t pgste;
	int dirty = 0;

	if (mm_has_pgste(mm)) {
		pgste = pgste_get_lock(ptep);
		pgste = pgste_update_all(ptep, pgste);
		dirty = !!(pgste_val(pgste) & KVM_UC_BIT);
		pgste_val(pgste) &= ~KVM_UC_BIT;
		pgste_set_unlock(ptep, pgste);
		return dirty;
	}
	return dirty;
}

/*
 * Get (and clear) the user referenced bit for a pte.
 */
static inline int ptep_test_and_clear_user_young(struct mm_struct *mm,
						 pte_t *ptep)
{
	pgste_t pgste;
	int young = 0;

	if (mm_has_pgste(mm)) {
		pgste = pgste_get_lock(ptep);
		pgste = pgste_update_young(ptep, pgste);
		young = !!(pgste_val(pgste) & KVM_UR_BIT);
		pgste_val(pgste) &= ~KVM_UR_BIT;
		pgste_set_unlock(ptep, pgste);
	}
	return young;
}

#define __HAVE_ARCH_PTEP_TEST_AND_CLEAR_YOUNG
static inline int ptep_test_and_clear_young(struct vm_area_struct *vma,
					    unsigned long addr, pte_t *ptep)
{
	pgste_t pgste;
	pte_t pte;

	if (mm_has_pgste(vma->vm_mm)) {
		pgste = pgste_get_lock(ptep);
		pgste = pgste_update_young(ptep, pgste);
		pte = *ptep;
		*ptep = pte_mkold(pte);
		pgste_set_unlock(ptep, pgste);
		return pte_young(pte);
	}
	return 0;
}

#define __HAVE_ARCH_PTEP_CLEAR_YOUNG_FLUSH
static inline int ptep_clear_flush_young(struct vm_area_struct *vma,
					 unsigned long address, pte_t *ptep)
{
	/* No need to flush TLB
	 * On s390 reference bits are in storage key and never in TLB
	 * With virtualization we handle the reference bit, without we
	 * we can simply return */
	return ptep_test_and_clear_young(vma, address, ptep);
}

static inline void __ptep_ipte(unsigned long address, pte_t *ptep)
{
	if (!(pte_val(*ptep) & _PAGE_INVALID)) {
#ifndef __s390x__
		/* pto must point to the start of the segment table */
		pte_t *pto = (pte_t *) (((unsigned long) ptep) & 0x7ffffc00);
#else
		/* ipte in zarch mode can do the math */
		pte_t *pto = ptep;
#endif
		asm volatile(
			"	ipte	%2,%3"
			: "=m" (*ptep) : "m" (*ptep),
			  "a" (pto), "a" (address));
	}
=======
	return set_pte_bit(pte, __pgprot(_PAGE_LARGE));
}
#endif

#define IPTE_GLOBAL	0
#define	IPTE_LOCAL	1

#define IPTE_NODAT	0x400
#define IPTE_GUEST_ASCE	0x800

static __always_inline void __ptep_rdp(unsigned long addr, pte_t *ptep,
				       unsigned long opt, unsigned long asce,
				       int local)
{
	unsigned long pto;

	pto = __pa(ptep) & ~(PTRS_PER_PTE * sizeof(pte_t) - 1);
	asm volatile(".insn rrf,0xb98b0000,%[r1],%[r2],%[asce],%[m4]"
		     : "+m" (*ptep)
		     : [r1] "a" (pto), [r2] "a" ((addr & PAGE_MASK) | opt),
		       [asce] "a" (asce), [m4] "i" (local));
}

static __always_inline void __ptep_ipte(unsigned long address, pte_t *ptep,
					unsigned long opt, unsigned long asce,
					int local)
{
	unsigned long pto = __pa(ptep);

	if (__builtin_constant_p(opt) && opt == 0) {
		/* Invalidation + TLB flush for the pte */
		asm volatile(
			"	ipte	%[r1],%[r2],0,%[m4]"
			: "+m" (*ptep) : [r1] "a" (pto), [r2] "a" (address),
			  [m4] "i" (local));
		return;
	}

	/* Invalidate ptes with options + TLB flush of the ptes */
	opt = opt | (asce & _ASCE_ORIGIN);
	asm volatile(
		"	ipte	%[r1],%[r2],%[r3],%[m4]"
		: [r2] "+a" (address), [r3] "+a" (opt)
		: [r1] "a" (pto), [m4] "i" (local) : "memory");
}

static __always_inline void __ptep_ipte_range(unsigned long address, int nr,
					      pte_t *ptep, int local)
{
	unsigned long pto = __pa(ptep);

	/* Invalidate a range of ptes + TLB flush of the ptes */
	do {
		asm volatile(
			"	ipte %[r1],%[r2],%[r3],%[m4]"
			: [r2] "+a" (address), [r3] "+a" (nr)
			: [r1] "a" (pto), [m4] "i" (local) : "memory");
	} while (nr != 255);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This is hard to understand. ptep_get_and_clear and ptep_clear_flush
 * both clear the TLB for the unmapped pte. The reason is that
 * ptep_get_and_clear is used in common code (e.g. change_pte_range)
 * to modify an active pte. The sequence is
 *   1) ptep_get_and_clear
 *   2) set_pte_at
 *   3) flush_tlb_range
 * On s390 the tlb needs to get flushed with the modification of the pte
 * if the pte is active. The only way how this can be implemented is to
 * have ptep_get_and_clear do the tlb flush. In exchange flush_tlb_range
 * is a nop.
 */
<<<<<<< HEAD
#define __HAVE_ARCH_PTEP_GET_AND_CLEAR
static inline pte_t ptep_get_and_clear(struct mm_struct *mm,
				       unsigned long address, pte_t *ptep)
{
	pgste_t pgste;
	pte_t pte;

	mm->context.flush_mm = 1;
	if (mm_has_pgste(mm))
		pgste = pgste_get_lock(ptep);

	pte = *ptep;
	if (!mm_exclusive(mm))
		__ptep_ipte(address, ptep);
	pte_val(*ptep) = _PAGE_TYPE_EMPTY;

	if (mm_has_pgste(mm)) {
		pgste = pgste_update_all(&pte, pgste);
		pgste_set_unlock(ptep, pgste);
	}
	return pte;
}

#define __HAVE_ARCH_PTEP_MODIFY_PROT_TRANSACTION
static inline pte_t ptep_modify_prot_start(struct mm_struct *mm,
					   unsigned long address,
					   pte_t *ptep)
{
	pte_t pte;

	mm->context.flush_mm = 1;
	if (mm_has_pgste(mm))
		pgste_get_lock(ptep);

	pte = *ptep;
	if (!mm_exclusive(mm))
		__ptep_ipte(address, ptep);
	return pte;
}

static inline void ptep_modify_prot_commit(struct mm_struct *mm,
					   unsigned long address,
					   pte_t *ptep, pte_t pte)
{
	*ptep = pte;
	if (mm_has_pgste(mm))
		pgste_set_unlock(ptep, *(pgste_t *)(ptep + PTRS_PER_PTE));
}

#define __HAVE_ARCH_PTEP_CLEAR_FLUSH
static inline pte_t ptep_clear_flush(struct vm_area_struct *vma,
				     unsigned long address, pte_t *ptep)
{
	pgste_t pgste;
	pte_t pte;

	if (mm_has_pgste(vma->vm_mm))
		pgste = pgste_get_lock(ptep);

	pte = *ptep;
	__ptep_ipte(address, ptep);
	pte_val(*ptep) = _PAGE_TYPE_EMPTY;

	if (mm_has_pgste(vma->vm_mm)) {
		pgste = pgste_update_all(&pte, pgste);
		pgste_set_unlock(ptep, pgste);
	}
	return pte;
=======
pte_t ptep_xchg_direct(struct mm_struct *, unsigned long, pte_t *, pte_t);
pte_t ptep_xchg_lazy(struct mm_struct *, unsigned long, pte_t *, pte_t);

#define __HAVE_ARCH_PTEP_TEST_AND_CLEAR_YOUNG
static inline int ptep_test_and_clear_young(struct vm_area_struct *vma,
					    unsigned long addr, pte_t *ptep)
{
	pte_t pte = *ptep;

	pte = ptep_xchg_direct(vma->vm_mm, addr, ptep, pte_mkold(pte));
	return pte_young(pte);
}

#define __HAVE_ARCH_PTEP_CLEAR_YOUNG_FLUSH
static inline int ptep_clear_flush_young(struct vm_area_struct *vma,
					 unsigned long address, pte_t *ptep)
{
	return ptep_test_and_clear_young(vma, address, ptep);
}

#define __HAVE_ARCH_PTEP_GET_AND_CLEAR
static inline pte_t ptep_get_and_clear(struct mm_struct *mm,
				       unsigned long addr, pte_t *ptep)
{
	pte_t res;

	res = ptep_xchg_lazy(mm, addr, ptep, __pte(_PAGE_INVALID));
	/* At this point the reference through the mapping is still present */
	if (mm_is_protected(mm) && pte_present(res))
		uv_convert_owned_from_secure(pte_val(res) & PAGE_MASK);
	return res;
}

#define __HAVE_ARCH_PTEP_MODIFY_PROT_TRANSACTION
pte_t ptep_modify_prot_start(struct vm_area_struct *, unsigned long, pte_t *);
void ptep_modify_prot_commit(struct vm_area_struct *, unsigned long,
			     pte_t *, pte_t, pte_t);

#define __HAVE_ARCH_PTEP_CLEAR_FLUSH
static inline pte_t ptep_clear_flush(struct vm_area_struct *vma,
				     unsigned long addr, pte_t *ptep)
{
	pte_t res;

	res = ptep_xchg_direct(vma->vm_mm, addr, ptep, __pte(_PAGE_INVALID));
	/* At this point the reference through the mapping is still present */
	if (mm_is_protected(vma->vm_mm) && pte_present(res))
		uv_convert_owned_from_secure(pte_val(res) & PAGE_MASK);
	return res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * The batched pte unmap code uses ptep_get_and_clear_full to clear the
 * ptes. Here an optimization is possible. tlb_gather_mmu flushes all
 * tlbs of an mm if it can guarantee that the ptes of the mm_struct
 * cannot be accessed while the batched unmap is running. In this case
 * full==1 and a simple pte_clear is enough. See tlb.h.
 */
#define __HAVE_ARCH_PTEP_GET_AND_CLEAR_FULL
static inline pte_t ptep_get_and_clear_full(struct mm_struct *mm,
<<<<<<< HEAD
					    unsigned long address,
					    pte_t *ptep, int full)
{
	pgste_t pgste;
	pte_t pte;

	if (mm_has_pgste(mm))
		pgste = pgste_get_lock(ptep);

	pte = *ptep;
	if (!full)
		__ptep_ipte(address, ptep);
	pte_val(*ptep) = _PAGE_TYPE_EMPTY;

	if (mm_has_pgste(mm)) {
		pgste = pgste_update_all(&pte, pgste);
		pgste_set_unlock(ptep, pgste);
	}
	return pte;
}

#define __HAVE_ARCH_PTEP_SET_WRPROTECT
static inline pte_t ptep_set_wrprotect(struct mm_struct *mm,
				       unsigned long address, pte_t *ptep)
{
	pgste_t pgste;
	pte_t pte = *ptep;

	if (pte_write(pte)) {
		mm->context.flush_mm = 1;
		if (mm_has_pgste(mm))
			pgste = pgste_get_lock(ptep);

		if (!mm_exclusive(mm))
			__ptep_ipte(address, ptep);
		*ptep = pte_wrprotect(pte);

		if (mm_has_pgste(mm))
			pgste_set_unlock(ptep, pgste);
	}
	return pte;
}

#define __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
static inline int ptep_set_access_flags(struct vm_area_struct *vma,
					unsigned long address, pte_t *ptep,
					pte_t entry, int dirty)
{
	pgste_t pgste;

	if (pte_same(*ptep, entry))
		return 0;
	if (mm_has_pgste(vma->vm_mm))
		pgste = pgste_get_lock(ptep);

	__ptep_ipte(address, ptep);
	*ptep = entry;

	if (mm_has_pgste(vma->vm_mm))
		pgste_set_unlock(ptep, pgste);
=======
					    unsigned long addr,
					    pte_t *ptep, int full)
{
	pte_t res;

	if (full) {
		res = *ptep;
		set_pte(ptep, __pte(_PAGE_INVALID));
	} else {
		res = ptep_xchg_lazy(mm, addr, ptep, __pte(_PAGE_INVALID));
	}
	/* Nothing to do */
	if (!mm_is_protected(mm) || !pte_present(res))
		return res;
	/*
	 * At this point the reference through the mapping is still present.
	 * The notifier should have destroyed all protected vCPUs at this
	 * point, so the destroy should be successful.
	 */
	if (full && !uv_destroy_owned_page(pte_val(res) & PAGE_MASK))
		return res;
	/*
	 * If something went wrong and the page could not be destroyed, or
	 * if this is not a mm teardown, the slower export is used as
	 * fallback instead.
	 */
	uv_convert_owned_from_secure(pte_val(res) & PAGE_MASK);
	return res;
}

#define __HAVE_ARCH_PTEP_SET_WRPROTECT
static inline void ptep_set_wrprotect(struct mm_struct *mm,
				      unsigned long addr, pte_t *ptep)
{
	pte_t pte = *ptep;

	if (pte_write(pte))
		ptep_xchg_lazy(mm, addr, ptep, pte_wrprotect(pte));
}

/*
 * Check if PTEs only differ in _PAGE_PROTECT HW bit, but also allow SW PTE
 * bits in the comparison. Those might change e.g. because of dirty and young
 * tracking.
 */
static inline int pte_allow_rdp(pte_t old, pte_t new)
{
	/*
	 * Only allow changes from RO to RW
	 */
	if (!(pte_val(old) & _PAGE_PROTECT) || pte_val(new) & _PAGE_PROTECT)
		return 0;

	return (pte_val(old) & _PAGE_RDP_MASK) == (pte_val(new) & _PAGE_RDP_MASK);
}

static inline void flush_tlb_fix_spurious_fault(struct vm_area_struct *vma,
						unsigned long address,
						pte_t *ptep)
{
	/*
	 * RDP might not have propagated the PTE protection reset to all CPUs,
	 * so there could be spurious TLB protection faults.
	 * NOTE: This will also be called when a racing pagetable update on
	 * another thread already installed the correct PTE. Both cases cannot
	 * really be distinguished.
	 * Therefore, only do the local TLB flush when RDP can be used, and the
	 * PTE does not have _PAGE_PROTECT set, to avoid unnecessary overhead.
	 * A local RDP can be used to do the flush.
	 */
	if (MACHINE_HAS_RDP && !(pte_val(*ptep) & _PAGE_PROTECT))
		__ptep_rdp(address, ptep, 0, 0, 1);
}
#define flush_tlb_fix_spurious_fault flush_tlb_fix_spurious_fault

void ptep_reset_dat_prot(struct mm_struct *mm, unsigned long addr, pte_t *ptep,
			 pte_t new);

#define __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
static inline int ptep_set_access_flags(struct vm_area_struct *vma,
					unsigned long addr, pte_t *ptep,
					pte_t entry, int dirty)
{
	if (pte_same(*ptep, entry))
		return 0;
	if (MACHINE_HAS_RDP && !mm_has_pgste(vma->vm_mm) && pte_allow_rdp(*ptep, entry))
		ptep_reset_dat_prot(vma->vm_mm, addr, ptep, entry);
	else
		ptep_xchg_direct(vma->vm_mm, addr, ptep, entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}

/*
<<<<<<< HEAD
=======
 * Additional functions to handle KVM guest page tables
 */
void ptep_set_pte_at(struct mm_struct *mm, unsigned long addr,
		     pte_t *ptep, pte_t entry);
void ptep_set_notify(struct mm_struct *mm, unsigned long addr, pte_t *ptep);
void ptep_notify(struct mm_struct *mm, unsigned long addr,
		 pte_t *ptep, unsigned long bits);
int ptep_force_prot(struct mm_struct *mm, unsigned long gaddr,
		    pte_t *ptep, int prot, unsigned long bit);
void ptep_zap_unused(struct mm_struct *mm, unsigned long addr,
		     pte_t *ptep , int reset);
void ptep_zap_key(struct mm_struct *mm, unsigned long addr, pte_t *ptep);
int ptep_shadow_pte(struct mm_struct *mm, unsigned long saddr,
		    pte_t *sptep, pte_t *tptep, pte_t pte);
void ptep_unshadow_pte(struct mm_struct *mm, unsigned long saddr, pte_t *ptep);

bool ptep_test_and_clear_uc(struct mm_struct *mm, unsigned long address,
			    pte_t *ptep);
int set_guest_storage_key(struct mm_struct *mm, unsigned long addr,
			  unsigned char key, bool nq);
int cond_set_guest_storage_key(struct mm_struct *mm, unsigned long addr,
			       unsigned char key, unsigned char *oldkey,
			       bool nq, bool mr, bool mc);
int reset_guest_reference_bit(struct mm_struct *mm, unsigned long addr);
int get_guest_storage_key(struct mm_struct *mm, unsigned long addr,
			  unsigned char *key);

int set_pgste_bits(struct mm_struct *mm, unsigned long addr,
				unsigned long bits, unsigned long value);
int get_pgste(struct mm_struct *mm, unsigned long hva, unsigned long *pgstep);
int pgste_perform_essa(struct mm_struct *mm, unsigned long hva, int orc,
			unsigned long *oldpte, unsigned long *oldpgste);
void gmap_pmdp_csp(struct mm_struct *mm, unsigned long vmaddr);
void gmap_pmdp_invalidate(struct mm_struct *mm, unsigned long vmaddr);
void gmap_pmdp_idte_local(struct mm_struct *mm, unsigned long vmaddr);
void gmap_pmdp_idte_global(struct mm_struct *mm, unsigned long vmaddr);

#define pgprot_writecombine	pgprot_writecombine
pgprot_t pgprot_writecombine(pgprot_t prot);

#define pgprot_writethrough	pgprot_writethrough
pgprot_t pgprot_writethrough(pgprot_t prot);

#define PFN_PTE_SHIFT		PAGE_SHIFT

/*
 * Set multiple PTEs to consecutive pages with a single call.  All PTEs
 * are within the same folio, PMD and VMA.
 */
static inline void set_ptes(struct mm_struct *mm, unsigned long addr,
			      pte_t *ptep, pte_t entry, unsigned int nr)
{
	if (pte_present(entry))
		entry = clear_pte_bit(entry, __pgprot(_PAGE_UNUSED));
	if (mm_has_pgste(mm)) {
		for (;;) {
			ptep_set_pte_at(mm, addr, ptep, entry);
			if (--nr == 0)
				break;
			ptep++;
			entry = __pte(pte_val(entry) + PAGE_SIZE);
			addr += PAGE_SIZE;
		}
	} else {
		for (;;) {
			set_pte(ptep, entry);
			if (--nr == 0)
				break;
			ptep++;
			entry = __pte(pte_val(entry) + PAGE_SIZE);
		}
	}
}
#define set_ptes set_ptes

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Conversion functions: convert a page and protection to a page entry,
 * and a page entry and page directory to the page they refer to.
 */
static inline pte_t mk_pte_phys(unsigned long physpage, pgprot_t pgprot)
{
	pte_t __pte;
<<<<<<< HEAD
	pte_val(__pte) = physpage + pgprot_val(pgprot);
	return __pte;
=======

	__pte = __pte(physpage | pgprot_val(pgprot));
	if (!MACHINE_HAS_NX)
		__pte = clear_pte_bit(__pte, __pgprot(_PAGE_NOEXEC));
	return pte_mkyoung(__pte);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline pte_t mk_pte(struct page *page, pgprot_t pgprot)
{
	unsigned long physpage = page_to_phys(page);
<<<<<<< HEAD

	return mk_pte_phys(physpage, pgprot);
}

#define pgd_index(address) (((address) >> PGDIR_SHIFT) & (PTRS_PER_PGD-1))
#define pud_index(address) (((address) >> PUD_SHIFT) & (PTRS_PER_PUD-1))
#define pmd_index(address) (((address) >> PMD_SHIFT) & (PTRS_PER_PMD-1))
#define pte_index(address) (((address) >> PAGE_SHIFT) & (PTRS_PER_PTE-1))

#define pgd_offset(mm, address) ((mm)->pgd + pgd_index(address))
#define pgd_offset_k(address) pgd_offset(&init_mm, address)

#ifndef __s390x__

#define pmd_deref(pmd) (pmd_val(pmd) & _SEGMENT_ENTRY_ORIGIN)
#define pud_deref(pmd) ({ BUG(); 0UL; })
#define pgd_deref(pmd) ({ BUG(); 0UL; })

#define pud_offset(pgd, address) ((pud_t *) pgd)
#define pmd_offset(pud, address) ((pmd_t *) pud + pmd_index(address))

#else /* __s390x__ */

#define pmd_deref(pmd) (pmd_val(pmd) & _SEGMENT_ENTRY_ORIGIN)
#define pud_deref(pud) (pud_val(pud) & _REGION_ENTRY_ORIGIN)
#define pgd_deref(pgd) (pgd_val(pgd) & _REGION_ENTRY_ORIGIN)

static inline pud_t *pud_offset(pgd_t *pgd, unsigned long address)
{
	pud_t *pud = (pud_t *) pgd;
	if ((pgd_val(*pgd) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R2)
		pud = (pud_t *) pgd_deref(*pgd);
	return pud  + pud_index(address);
}

static inline pmd_t *pmd_offset(pud_t *pud, unsigned long address)
{
	pmd_t *pmd = (pmd_t *) pud;
	if ((pud_val(*pud) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R3)
		pmd = (pmd_t *) pud_deref(*pud);
	return pmd + pmd_index(address);
}

#endif /* __s390x__ */

#define pfn_pte(pfn,pgprot) mk_pte_phys(__pa((pfn) << PAGE_SHIFT),(pgprot))
#define pte_pfn(x) (pte_val(x) >> PAGE_SHIFT)
#define pte_page(x) pfn_to_page(pte_pfn(x))

#define pmd_page(pmd) pfn_to_page(pmd_val(pmd) >> PAGE_SHIFT)

/* Find an entry in the lowest level page table.. */
#define pte_offset(pmd, addr) ((pte_t *) pmd_deref(*(pmd)) + pte_index(addr))
#define pte_offset_kernel(pmd, address) pte_offset(pmd,address)
#define pte_offset_map(pmd, address) pte_offset_kernel(pmd, address)
#define pte_unmap(pte) do { } while (0)

/*
 * 31 bit swap entry format:
 * A page-table entry has some bits we have to treat in a special way.
 * Bits 0, 20 and bit 23 have to be zero, otherwise an specification
 * exception will occur instead of a page translation exception. The
 * specifiation exception has the bad habit not to store necessary
 * information in the lowcore.
 * Bit 21 and bit 22 are the page invalid bit and the page protection
 * bit. We set both to indicate a swapped page.
 * Bit 30 and 31 are used to distinguish the different page types. For
 * a swapped page these bits need to be zero.
 * This leaves the bits 1-19 and bits 24-29 to store type and offset.
 * We use the 5 bits from 25-29 for the type and the 20 bits from 1-19
 * plus 24 for the offset.
 * 0|     offset        |0110|o|type |00|
 * 0 0000000001111111111 2222 2 22222 33
 * 0 1234567890123456789 0123 4 56789 01
 *
 * 64 bit swap entry format:
 * A page-table entry has some bits we have to treat in a special way.
 * Bits 52 and bit 55 have to be zero, otherwise an specification
 * exception will occur instead of a page translation exception. The
 * specifiation exception has the bad habit not to store necessary
 * information in the lowcore.
 * Bit 53 and bit 54 are the page invalid bit and the page protection
 * bit. We set both to indicate a swapped page.
 * Bit 62 and 63 are used to distinguish the different page types. For
 * a swapped page these bits need to be zero.
 * This leaves the bits 0-51 and bits 56-61 to store type and offset.
 * We use the 5 bits from 57-61 for the type and the 53 bits from 0-51
 * plus 56 for the offset.
 * |                      offset                        |0110|o|type |00|
 *  0000000000111111111122222222223333333333444444444455 5555 5 55566 66
 *  0123456789012345678901234567890123456789012345678901 2345 6 78901 23
 */
#ifndef __s390x__
#define __SWP_OFFSET_MASK (~0UL >> 12)
#else
#define __SWP_OFFSET_MASK (~0UL >> 11)
#endif
static inline pte_t mk_swap_pte(unsigned long type, unsigned long offset)
{
	pte_t pte;
	offset &= __SWP_OFFSET_MASK;
	pte_val(pte) = _PAGE_TYPE_SWAP | ((type & 0x1f) << 2) |
		((offset & 1UL) << 7) | ((offset & ~1UL) << 11);
	return pte;
}

#define __swp_type(entry)	(((entry).val >> 2) & 0x1f)
#define __swp_offset(entry)	(((entry).val >> 11) | (((entry).val >> 7) & 1))
#define __swp_entry(type,offset) ((swp_entry_t) { pte_val(mk_swap_pte((type),(offset))) })
=======
	pte_t __pte = mk_pte_phys(physpage, pgprot);

	if (pte_write(__pte) && PageDirty(page))
		__pte = pte_mkdirty(__pte);
	return __pte;
}

#define pgd_index(address) (((address) >> PGDIR_SHIFT) & (PTRS_PER_PGD-1))
#define p4d_index(address) (((address) >> P4D_SHIFT) & (PTRS_PER_P4D-1))
#define pud_index(address) (((address) >> PUD_SHIFT) & (PTRS_PER_PUD-1))
#define pmd_index(address) (((address) >> PMD_SHIFT) & (PTRS_PER_PMD-1))

#define p4d_deref(pud) ((unsigned long)__va(p4d_val(pud) & _REGION_ENTRY_ORIGIN))
#define pgd_deref(pgd) ((unsigned long)__va(pgd_val(pgd) & _REGION_ENTRY_ORIGIN))

static inline unsigned long pmd_deref(pmd_t pmd)
{
	unsigned long origin_mask;

	origin_mask = _SEGMENT_ENTRY_ORIGIN;
	if (pmd_leaf(pmd))
		origin_mask = _SEGMENT_ENTRY_ORIGIN_LARGE;
	return (unsigned long)__va(pmd_val(pmd) & origin_mask);
}

static inline unsigned long pmd_pfn(pmd_t pmd)
{
	return __pa(pmd_deref(pmd)) >> PAGE_SHIFT;
}

static inline unsigned long pud_deref(pud_t pud)
{
	unsigned long origin_mask;

	origin_mask = _REGION_ENTRY_ORIGIN;
	if (pud_leaf(pud))
		origin_mask = _REGION3_ENTRY_ORIGIN_LARGE;
	return (unsigned long)__va(pud_val(pud) & origin_mask);
}

static inline unsigned long pud_pfn(pud_t pud)
{
	return __pa(pud_deref(pud)) >> PAGE_SHIFT;
}

/*
 * The pgd_offset function *always* adds the index for the top-level
 * region/segment table. This is done to get a sequence like the
 * following to work:
 *	pgdp = pgd_offset(current->mm, addr);
 *	pgd = READ_ONCE(*pgdp);
 *	p4dp = p4d_offset(&pgd, addr);
 *	...
 * The subsequent p4d_offset, pud_offset and pmd_offset functions
 * only add an index if they dereferenced the pointer.
 */
static inline pgd_t *pgd_offset_raw(pgd_t *pgd, unsigned long address)
{
	unsigned long rste;
	unsigned int shift;

	/* Get the first entry of the top level table */
	rste = pgd_val(*pgd);
	/* Pick up the shift from the table type of the first entry */
	shift = ((rste & _REGION_ENTRY_TYPE_MASK) >> 2) * 11 + 20;
	return pgd + ((address >> shift) & (PTRS_PER_PGD - 1));
}

#define pgd_offset(mm, address) pgd_offset_raw(READ_ONCE((mm)->pgd), address)

static inline p4d_t *p4d_offset_lockless(pgd_t *pgdp, pgd_t pgd, unsigned long address)
{
	if ((pgd_val(pgd) & _REGION_ENTRY_TYPE_MASK) >= _REGION_ENTRY_TYPE_R1)
		return (p4d_t *) pgd_deref(pgd) + p4d_index(address);
	return (p4d_t *) pgdp;
}
#define p4d_offset_lockless p4d_offset_lockless

static inline p4d_t *p4d_offset(pgd_t *pgdp, unsigned long address)
{
	return p4d_offset_lockless(pgdp, *pgdp, address);
}

static inline pud_t *pud_offset_lockless(p4d_t *p4dp, p4d_t p4d, unsigned long address)
{
	if ((p4d_val(p4d) & _REGION_ENTRY_TYPE_MASK) >= _REGION_ENTRY_TYPE_R2)
		return (pud_t *) p4d_deref(p4d) + pud_index(address);
	return (pud_t *) p4dp;
}
#define pud_offset_lockless pud_offset_lockless

static inline pud_t *pud_offset(p4d_t *p4dp, unsigned long address)
{
	return pud_offset_lockless(p4dp, *p4dp, address);
}
#define pud_offset pud_offset

static inline pmd_t *pmd_offset_lockless(pud_t *pudp, pud_t pud, unsigned long address)
{
	if ((pud_val(pud) & _REGION_ENTRY_TYPE_MASK) >= _REGION_ENTRY_TYPE_R3)
		return (pmd_t *) pud_deref(pud) + pmd_index(address);
	return (pmd_t *) pudp;
}
#define pmd_offset_lockless pmd_offset_lockless

static inline pmd_t *pmd_offset(pud_t *pudp, unsigned long address)
{
	return pmd_offset_lockless(pudp, *pudp, address);
}
#define pmd_offset pmd_offset

static inline unsigned long pmd_page_vaddr(pmd_t pmd)
{
	return (unsigned long) pmd_deref(pmd);
}

static inline bool gup_fast_permitted(unsigned long start, unsigned long end)
{
	return end <= current->mm->context.asce_limit;
}
#define gup_fast_permitted gup_fast_permitted

#define pfn_pte(pfn, pgprot)	mk_pte_phys(((pfn) << PAGE_SHIFT), (pgprot))
#define pte_pfn(x) (pte_val(x) >> PAGE_SHIFT)
#define pte_page(x) pfn_to_page(pte_pfn(x))

#define pmd_page(pmd) pfn_to_page(pmd_pfn(pmd))
#define pud_page(pud) pfn_to_page(pud_pfn(pud))
#define p4d_page(p4d) pfn_to_page(p4d_pfn(p4d))
#define pgd_page(pgd) pfn_to_page(pgd_pfn(pgd))

static inline pmd_t pmd_wrprotect(pmd_t pmd)
{
	pmd = clear_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_WRITE));
	return set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_PROTECT));
}

static inline pmd_t pmd_mkwrite_novma(pmd_t pmd)
{
	pmd = set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_WRITE));
	if (pmd_val(pmd) & _SEGMENT_ENTRY_DIRTY)
		pmd = clear_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_PROTECT));
	return pmd;
}

static inline pmd_t pmd_mkclean(pmd_t pmd)
{
	pmd = clear_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_DIRTY));
	return set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_PROTECT));
}

static inline pmd_t pmd_mkdirty(pmd_t pmd)
{
	pmd = set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_DIRTY | _SEGMENT_ENTRY_SOFT_DIRTY));
	if (pmd_val(pmd) & _SEGMENT_ENTRY_WRITE)
		pmd = clear_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_PROTECT));
	return pmd;
}

static inline pud_t pud_wrprotect(pud_t pud)
{
	pud = clear_pud_bit(pud, __pgprot(_REGION3_ENTRY_WRITE));
	return set_pud_bit(pud, __pgprot(_REGION_ENTRY_PROTECT));
}

static inline pud_t pud_mkwrite(pud_t pud)
{
	pud = set_pud_bit(pud, __pgprot(_REGION3_ENTRY_WRITE));
	if (pud_val(pud) & _REGION3_ENTRY_DIRTY)
		pud = clear_pud_bit(pud, __pgprot(_REGION_ENTRY_PROTECT));
	return pud;
}

static inline pud_t pud_mkclean(pud_t pud)
{
	pud = clear_pud_bit(pud, __pgprot(_REGION3_ENTRY_DIRTY));
	return set_pud_bit(pud, __pgprot(_REGION_ENTRY_PROTECT));
}

static inline pud_t pud_mkdirty(pud_t pud)
{
	pud = set_pud_bit(pud, __pgprot(_REGION3_ENTRY_DIRTY | _REGION3_ENTRY_SOFT_DIRTY));
	if (pud_val(pud) & _REGION3_ENTRY_WRITE)
		pud = clear_pud_bit(pud, __pgprot(_REGION_ENTRY_PROTECT));
	return pud;
}

#if defined(CONFIG_TRANSPARENT_HUGEPAGE) || defined(CONFIG_HUGETLB_PAGE)
static inline unsigned long massage_pgprot_pmd(pgprot_t pgprot)
{
	/*
	 * pgprot is PAGE_NONE, PAGE_RO, PAGE_RX, PAGE_RW or PAGE_RWX
	 * (see __Pxxx / __Sxxx). Convert to segment table entry format.
	 */
	if (pgprot_val(pgprot) == pgprot_val(PAGE_NONE))
		return pgprot_val(SEGMENT_NONE);
	if (pgprot_val(pgprot) == pgprot_val(PAGE_RO))
		return pgprot_val(SEGMENT_RO);
	if (pgprot_val(pgprot) == pgprot_val(PAGE_RX))
		return pgprot_val(SEGMENT_RX);
	if (pgprot_val(pgprot) == pgprot_val(PAGE_RW))
		return pgprot_val(SEGMENT_RW);
	return pgprot_val(SEGMENT_RWX);
}

static inline pmd_t pmd_mkyoung(pmd_t pmd)
{
	pmd = set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_YOUNG));
	if (pmd_val(pmd) & _SEGMENT_ENTRY_READ)
		pmd = clear_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_INVALID));
	return pmd;
}

static inline pmd_t pmd_mkold(pmd_t pmd)
{
	pmd = clear_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_YOUNG));
	return set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_INVALID));
}

static inline pmd_t pmd_modify(pmd_t pmd, pgprot_t newprot)
{
	unsigned long mask;

	mask  = _SEGMENT_ENTRY_ORIGIN_LARGE;
	mask |= _SEGMENT_ENTRY_DIRTY;
	mask |= _SEGMENT_ENTRY_YOUNG;
	mask |=	_SEGMENT_ENTRY_LARGE;
	mask |= _SEGMENT_ENTRY_SOFT_DIRTY;
	pmd = __pmd(pmd_val(pmd) & mask);
	pmd = set_pmd_bit(pmd, __pgprot(massage_pgprot_pmd(newprot)));
	if (!(pmd_val(pmd) & _SEGMENT_ENTRY_DIRTY))
		pmd = set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_PROTECT));
	if (!(pmd_val(pmd) & _SEGMENT_ENTRY_YOUNG))
		pmd = set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_INVALID));
	return pmd;
}

static inline pmd_t mk_pmd_phys(unsigned long physpage, pgprot_t pgprot)
{
	return __pmd(physpage + massage_pgprot_pmd(pgprot));
}

#endif /* CONFIG_TRANSPARENT_HUGEPAGE || CONFIG_HUGETLB_PAGE */

static inline void __pmdp_csp(pmd_t *pmdp)
{
	csp((unsigned int *)pmdp + 1, pmd_val(*pmdp),
	    pmd_val(*pmdp) | _SEGMENT_ENTRY_INVALID);
}

#define IDTE_GLOBAL	0
#define IDTE_LOCAL	1

#define IDTE_PTOA	0x0800
#define IDTE_NODAT	0x1000
#define IDTE_GUEST_ASCE	0x2000

static __always_inline void __pmdp_idte(unsigned long addr, pmd_t *pmdp,
					unsigned long opt, unsigned long asce,
					int local)
{
	unsigned long sto;

	sto = __pa(pmdp) - pmd_index(addr) * sizeof(pmd_t);
	if (__builtin_constant_p(opt) && opt == 0) {
		/* flush without guest asce */
		asm volatile(
			"	idte	%[r1],0,%[r2],%[m4]"
			: "+m" (*pmdp)
			: [r1] "a" (sto), [r2] "a" ((addr & HPAGE_MASK)),
			  [m4] "i" (local)
			: "cc" );
	} else {
		/* flush with guest asce */
		asm volatile(
			"	idte	%[r1],%[r3],%[r2],%[m4]"
			: "+m" (*pmdp)
			: [r1] "a" (sto), [r2] "a" ((addr & HPAGE_MASK) | opt),
			  [r3] "a" (asce), [m4] "i" (local)
			: "cc" );
	}
}

static __always_inline void __pudp_idte(unsigned long addr, pud_t *pudp,
					unsigned long opt, unsigned long asce,
					int local)
{
	unsigned long r3o;

	r3o = __pa(pudp) - pud_index(addr) * sizeof(pud_t);
	r3o |= _ASCE_TYPE_REGION3;
	if (__builtin_constant_p(opt) && opt == 0) {
		/* flush without guest asce */
		asm volatile(
			"	idte	%[r1],0,%[r2],%[m4]"
			: "+m" (*pudp)
			: [r1] "a" (r3o), [r2] "a" ((addr & PUD_MASK)),
			  [m4] "i" (local)
			: "cc");
	} else {
		/* flush with guest asce */
		asm volatile(
			"	idte	%[r1],%[r3],%[r2],%[m4]"
			: "+m" (*pudp)
			: [r1] "a" (r3o), [r2] "a" ((addr & PUD_MASK) | opt),
			  [r3] "a" (asce), [m4] "i" (local)
			: "cc" );
	}
}

pmd_t pmdp_xchg_direct(struct mm_struct *, unsigned long, pmd_t *, pmd_t);
pmd_t pmdp_xchg_lazy(struct mm_struct *, unsigned long, pmd_t *, pmd_t);
pud_t pudp_xchg_direct(struct mm_struct *, unsigned long, pud_t *, pud_t);

#ifdef CONFIG_TRANSPARENT_HUGEPAGE

#define __HAVE_ARCH_PGTABLE_DEPOSIT
void pgtable_trans_huge_deposit(struct mm_struct *mm, pmd_t *pmdp,
				pgtable_t pgtable);

#define __HAVE_ARCH_PGTABLE_WITHDRAW
pgtable_t pgtable_trans_huge_withdraw(struct mm_struct *mm, pmd_t *pmdp);

#define  __HAVE_ARCH_PMDP_SET_ACCESS_FLAGS
static inline int pmdp_set_access_flags(struct vm_area_struct *vma,
					unsigned long addr, pmd_t *pmdp,
					pmd_t entry, int dirty)
{
	VM_BUG_ON(addr & ~HPAGE_MASK);

	entry = pmd_mkyoung(entry);
	if (dirty)
		entry = pmd_mkdirty(entry);
	if (pmd_val(*pmdp) == pmd_val(entry))
		return 0;
	pmdp_xchg_direct(vma->vm_mm, addr, pmdp, entry);
	return 1;
}

#define __HAVE_ARCH_PMDP_TEST_AND_CLEAR_YOUNG
static inline int pmdp_test_and_clear_young(struct vm_area_struct *vma,
					    unsigned long addr, pmd_t *pmdp)
{
	pmd_t pmd = *pmdp;

	pmd = pmdp_xchg_direct(vma->vm_mm, addr, pmdp, pmd_mkold(pmd));
	return pmd_young(pmd);
}

#define __HAVE_ARCH_PMDP_CLEAR_YOUNG_FLUSH
static inline int pmdp_clear_flush_young(struct vm_area_struct *vma,
					 unsigned long addr, pmd_t *pmdp)
{
	VM_BUG_ON(addr & ~HPAGE_MASK);
	return pmdp_test_and_clear_young(vma, addr, pmdp);
}

static inline void set_pmd_at(struct mm_struct *mm, unsigned long addr,
			      pmd_t *pmdp, pmd_t entry)
{
	if (!MACHINE_HAS_NX)
		entry = clear_pmd_bit(entry, __pgprot(_SEGMENT_ENTRY_NOEXEC));
	set_pmd(pmdp, entry);
}

static inline pmd_t pmd_mkhuge(pmd_t pmd)
{
	pmd = set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_LARGE));
	pmd = set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_YOUNG));
	return set_pmd_bit(pmd, __pgprot(_SEGMENT_ENTRY_PROTECT));
}

#define __HAVE_ARCH_PMDP_HUGE_GET_AND_CLEAR
static inline pmd_t pmdp_huge_get_and_clear(struct mm_struct *mm,
					    unsigned long addr, pmd_t *pmdp)
{
	return pmdp_xchg_direct(mm, addr, pmdp, __pmd(_SEGMENT_ENTRY_EMPTY));
}

#define __HAVE_ARCH_PMDP_HUGE_GET_AND_CLEAR_FULL
static inline pmd_t pmdp_huge_get_and_clear_full(struct vm_area_struct *vma,
						 unsigned long addr,
						 pmd_t *pmdp, int full)
{
	if (full) {
		pmd_t pmd = *pmdp;
		set_pmd(pmdp, __pmd(_SEGMENT_ENTRY_EMPTY));
		return pmd;
	}
	return pmdp_xchg_lazy(vma->vm_mm, addr, pmdp, __pmd(_SEGMENT_ENTRY_EMPTY));
}

#define __HAVE_ARCH_PMDP_HUGE_CLEAR_FLUSH
static inline pmd_t pmdp_huge_clear_flush(struct vm_area_struct *vma,
					  unsigned long addr, pmd_t *pmdp)
{
	return pmdp_huge_get_and_clear(vma->vm_mm, addr, pmdp);
}

#define __HAVE_ARCH_PMDP_INVALIDATE
static inline pmd_t pmdp_invalidate(struct vm_area_struct *vma,
				   unsigned long addr, pmd_t *pmdp)
{
	pmd_t pmd = __pmd(pmd_val(*pmdp) | _SEGMENT_ENTRY_INVALID);

	return pmdp_xchg_direct(vma->vm_mm, addr, pmdp, pmd);
}

#define __HAVE_ARCH_PMDP_SET_WRPROTECT
static inline void pmdp_set_wrprotect(struct mm_struct *mm,
				      unsigned long addr, pmd_t *pmdp)
{
	pmd_t pmd = *pmdp;

	if (pmd_write(pmd))
		pmd = pmdp_xchg_lazy(mm, addr, pmdp, pmd_wrprotect(pmd));
}

static inline pmd_t pmdp_collapse_flush(struct vm_area_struct *vma,
					unsigned long address,
					pmd_t *pmdp)
{
	return pmdp_huge_get_and_clear(vma->vm_mm, address, pmdp);
}
#define pmdp_collapse_flush pmdp_collapse_flush

#define pfn_pmd(pfn, pgprot)	mk_pmd_phys(((pfn) << PAGE_SHIFT), (pgprot))
#define mk_pmd(page, pgprot)	pfn_pmd(page_to_pfn(page), (pgprot))

static inline int pmd_trans_huge(pmd_t pmd)
{
	return pmd_val(pmd) & _SEGMENT_ENTRY_LARGE;
}

#define has_transparent_hugepage has_transparent_hugepage
static inline int has_transparent_hugepage(void)
{
	return MACHINE_HAS_EDAT1 ? 1 : 0;
}
#endif /* CONFIG_TRANSPARENT_HUGEPAGE */

/*
 * 64 bit swap entry format:
 * A page-table entry has some bits we have to treat in a special way.
 * Bits 54 and 63 are used to indicate the page type. Bit 53 marks the pte
 * as invalid.
 * A swap pte is indicated by bit pattern (pte & 0x201) == 0x200
 * |			  offset			|E11XX|type |S0|
 * |0000000000111111111122222222223333333333444444444455|55555|55566|66|
 * |0123456789012345678901234567890123456789012345678901|23456|78901|23|
 *
 * Bits 0-51 store the offset.
 * Bit 52 (E) is used to remember PG_anon_exclusive.
 * Bits 57-61 store the type.
 * Bit 62 (S) is used for softdirty tracking.
 * Bits 55 and 56 (X) are unused.
 */

#define __SWP_OFFSET_MASK	((1UL << 52) - 1)
#define __SWP_OFFSET_SHIFT	12
#define __SWP_TYPE_MASK		((1UL << 5) - 1)
#define __SWP_TYPE_SHIFT	2

static inline pte_t mk_swap_pte(unsigned long type, unsigned long offset)
{
	unsigned long pteval;

	pteval = _PAGE_INVALID | _PAGE_PROTECT;
	pteval |= (offset & __SWP_OFFSET_MASK) << __SWP_OFFSET_SHIFT;
	pteval |= (type & __SWP_TYPE_MASK) << __SWP_TYPE_SHIFT;
	return __pte(pteval);
}

static inline unsigned long __swp_type(swp_entry_t entry)
{
	return (entry.val >> __SWP_TYPE_SHIFT) & __SWP_TYPE_MASK;
}

static inline unsigned long __swp_offset(swp_entry_t entry)
{
	return (entry.val >> __SWP_OFFSET_SHIFT) & __SWP_OFFSET_MASK;
}

static inline swp_entry_t __swp_entry(unsigned long type, unsigned long offset)
{
	return (swp_entry_t) { pte_val(mk_swap_pte(type, offset)) };
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define __pte_to_swp_entry(pte)	((swp_entry_t) { pte_val(pte) })
#define __swp_entry_to_pte(x)	((pte_t) { (x).val })

<<<<<<< HEAD
#ifndef __s390x__
# define PTE_FILE_MAX_BITS	26
#else /* __s390x__ */
# define PTE_FILE_MAX_BITS	59
#endif /* __s390x__ */

#define pte_to_pgoff(__pte) \
	((((__pte).pte >> 12) << 7) + (((__pte).pte >> 1) & 0x7f))

#define pgoff_to_pte(__off) \
	((pte_t) { ((((__off) & 0x7f) << 1) + (((__off) >> 7) << 12)) \
		   | _PAGE_TYPE_FILE })

#endif /* !__ASSEMBLY__ */

#define kern_addr_valid(addr)   (1)

extern int vmem_add_mapping(unsigned long start, unsigned long size);
extern int vmem_remove_mapping(unsigned long start, unsigned long size);
extern int s390_enable_sie(void);

/*
 * No page table caches to initialise
 */
#define pgtable_cache_init()	do { } while (0)

#include <asm-generic/pgtable.h>
=======
extern int vmem_add_mapping(unsigned long start, unsigned long size);
extern void vmem_remove_mapping(unsigned long start, unsigned long size);
extern int __vmem_map_4k_page(unsigned long addr, unsigned long phys, pgprot_t prot, bool alloc);
extern int vmem_map_4k_page(unsigned long addr, unsigned long phys, pgprot_t prot);
extern void vmem_unmap_4k_page(unsigned long addr);
extern pte_t *vmem_get_alloc_pte(unsigned long addr, bool alloc);
extern int s390_enable_sie(void);
extern int s390_enable_skey(void);
extern void s390_reset_cmma(struct mm_struct *mm);

/* s390 has a private copy of get unmapped area to deal with cache synonyms */
#define HAVE_ARCH_UNMAPPED_AREA
#define HAVE_ARCH_UNMAPPED_AREA_TOPDOWN

#define pmd_pgtable(pmd) \
	((pgtable_t)__va(pmd_val(pmd) & -sizeof(pte_t)*PTRS_PER_PTE))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _S390_PAGE_H */
