<<<<<<< HEAD
/*
 *  include/asm-s390/pgalloc.h
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
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 *  Derived from "include/asm-i386/pgalloc.h"
 *    Copyright (C) 1994  Linus Torvalds
 */

#ifndef _S390_PGALLOC_H
#define _S390_PGALLOC_H

#include <linux/threads.h>
<<<<<<< HEAD
#include <linux/gfp.h>
#include <linux/mm.h>

unsigned long *crst_table_alloc(struct mm_struct *);
void crst_table_free(struct mm_struct *, unsigned long *);

unsigned long *page_table_alloc(struct mm_struct *, unsigned long);
void page_table_free(struct mm_struct *, unsigned long *);
void page_table_free_rcu(struct mmu_gather *, unsigned long *);

static inline void clear_table(unsigned long *s, unsigned long val, size_t n)
{
	typedef struct { char _[n]; } addrtype;

	*s = val;
	n = (n / 256) - 1;
	asm volatile(
#ifdef CONFIG_64BIT
		"	mvc	8(248,%0),0(%0)\n"
#else
		"	mvc	4(252,%0),0(%0)\n"
#endif
		"0:	mvc	256(256,%0),0(%0)\n"
		"	la	%0,256(%0)\n"
		"	brct	%1,0b\n"
		: "+a" (s), "+d" (n), "=m" (*(addrtype *) s)
		: "m" (*(addrtype *) s));
}

static inline void crst_table_init(unsigned long *crst, unsigned long entry)
{
	clear_table(crst, entry, sizeof(unsigned long)*2048);
}

#ifndef __s390x__

static inline unsigned long pgd_entry_type(struct mm_struct *mm)
{
	return _SEGMENT_ENTRY_EMPTY;
}

#define pud_alloc_one(mm,address)		({ BUG(); ((pud_t *)2); })
#define pud_free(mm, x)				do { } while (0)

#define pmd_alloc_one(mm,address)		({ BUG(); ((pmd_t *)2); })
#define pmd_free(mm, x)				do { } while (0)

#define pgd_populate(mm, pgd, pud)		BUG()
#define pud_populate(mm, pud, pmd)		BUG()

#else /* __s390x__ */

static inline unsigned long pgd_entry_type(struct mm_struct *mm)
{
	if (mm->context.asce_limit <= (1UL << 31))
		return _SEGMENT_ENTRY_EMPTY;
	if (mm->context.asce_limit <= (1UL << 42))
		return _REGION3_ENTRY_EMPTY;
	return _REGION2_ENTRY_EMPTY;
}

int crst_table_upgrade(struct mm_struct *, unsigned long limit);
void crst_table_downgrade(struct mm_struct *, unsigned long limit);
=======
#include <linux/string.h>
#include <linux/gfp.h>
#include <linux/mm.h>

#define CRST_ALLOC_ORDER 2

unsigned long *crst_table_alloc(struct mm_struct *);
void crst_table_free(struct mm_struct *, unsigned long *);

unsigned long *page_table_alloc(struct mm_struct *);
struct ptdesc *page_table_alloc_pgste(struct mm_struct *mm);
void page_table_free(struct mm_struct *, unsigned long *);
void page_table_free_pgste(struct ptdesc *ptdesc);
extern int page_table_allocate_pgste;

static inline void crst_table_init(unsigned long *crst, unsigned long entry)
{
	memset64((u64 *)crst, entry, _CRST_ENTRIES);
}

int crst_table_upgrade(struct mm_struct *mm, unsigned long limit);

static inline unsigned long check_asce_limit(struct mm_struct *mm, unsigned long addr,
					     unsigned long len)
{
	int rc;

	if (addr + len > mm->context.asce_limit &&
	    addr + len <= TASK_SIZE) {
		rc = crst_table_upgrade(mm, addr + len);
		if (rc)
			return (unsigned long) rc;
	}
	return addr;
}

static inline p4d_t *p4d_alloc_one(struct mm_struct *mm, unsigned long address)
{
	unsigned long *table = crst_table_alloc(mm);

	if (table)
		crst_table_init(table, _REGION2_ENTRY_EMPTY);
	return (p4d_t *) table;
}

static inline void p4d_free(struct mm_struct *mm, p4d_t *p4d)
{
	if (!mm_p4d_folded(mm))
		crst_table_free(mm, (unsigned long *) p4d);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline pud_t *pud_alloc_one(struct mm_struct *mm, unsigned long address)
{
	unsigned long *table = crst_table_alloc(mm);
	if (table)
		crst_table_init(table, _REGION3_ENTRY_EMPTY);
	return (pud_t *) table;
}
<<<<<<< HEAD
#define pud_free(mm, pud) crst_table_free(mm, (unsigned long *) pud)
=======

static inline void pud_free(struct mm_struct *mm, pud_t *pud)
{
	if (!mm_pud_folded(mm))
		crst_table_free(mm, (unsigned long *) pud);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long vmaddr)
{
	unsigned long *table = crst_table_alloc(mm);
<<<<<<< HEAD
	if (table)
		crst_table_init(table, _SEGMENT_ENTRY_EMPTY);
	return (pmd_t *) table;
}
#define pmd_free(mm, pmd) crst_table_free(mm, (unsigned long *) pmd)

static inline void pgd_populate(struct mm_struct *mm, pgd_t *pgd, pud_t *pud)
{
	pgd_val(*pgd) = _REGION2_ENTRY | __pa(pud);
=======

	if (!table)
		return NULL;
	crst_table_init(table, _SEGMENT_ENTRY_EMPTY);
	if (!pagetable_pmd_ctor(virt_to_ptdesc(table))) {
		crst_table_free(mm, table);
		return NULL;
	}
	return (pmd_t *) table;
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	if (mm_pmd_folded(mm))
		return;
	pagetable_pmd_dtor(virt_to_ptdesc(pmd));
	crst_table_free(mm, (unsigned long *) pmd);
}

static inline void pgd_populate(struct mm_struct *mm, pgd_t *pgd, p4d_t *p4d)
{
	set_pgd(pgd, __pgd(_REGION1_ENTRY | __pa(p4d)));
}

static inline void p4d_populate(struct mm_struct *mm, p4d_t *p4d, pud_t *pud)
{
	set_p4d(p4d, __p4d(_REGION2_ENTRY | __pa(pud)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void pud_populate(struct mm_struct *mm, pud_t *pud, pmd_t *pmd)
{
<<<<<<< HEAD
	pud_val(*pud) = _REGION3_ENTRY | __pa(pmd);
}

#endif /* __s390x__ */

static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	spin_lock_init(&mm->context.list_lock);
	INIT_LIST_HEAD(&mm->context.pgtable_list);
	INIT_LIST_HEAD(&mm->context.gmap_list);
	return (pgd_t *) crst_table_alloc(mm);
}
#define pgd_free(mm, pgd) crst_table_free(mm, (unsigned long *) pgd)
=======
	set_pud(pud, __pud(_REGION3_ENTRY | __pa(pmd)));
}

static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	return (pgd_t *) crst_table_alloc(mm);
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	crst_table_free(mm, (unsigned long *) pgd);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void pmd_populate(struct mm_struct *mm,
				pmd_t *pmd, pgtable_t pte)
{
<<<<<<< HEAD
	pmd_val(*pmd) = _SEGMENT_ENTRY + __pa(pte);
=======
	set_pmd(pmd, __pmd(_SEGMENT_ENTRY | __pa(pte)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define pmd_populate_kernel(mm, pmd, pte) pmd_populate(mm, pmd, pte)

<<<<<<< HEAD
#define pmd_pgtable(pmd) \
	(pgtable_t)(pmd_val(pmd) & -sizeof(pte_t)*PTRS_PER_PTE)

/*
 * page table entry allocation/free routines.
 */
#define pte_alloc_one_kernel(mm, vmaddr) ((pte_t *) page_table_alloc(mm, vmaddr))
#define pte_alloc_one(mm, vmaddr) ((pte_t *) page_table_alloc(mm, vmaddr))
=======
/*
 * page table entry allocation/free routines.
 */
#define pte_alloc_one_kernel(mm) ((pte_t *)page_table_alloc(mm))
#define pte_alloc_one(mm) ((pte_t *)page_table_alloc(mm))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define pte_free_kernel(mm, pte) page_table_free(mm, (unsigned long *) pte)
#define pte_free(mm, pte) page_table_free(mm, (unsigned long *) pte)

<<<<<<< HEAD
extern void rcu_table_freelist_finish(void);
=======
/* arch use pte_free_defer() implementation in arch/s390/mm/pgalloc.c */
#define pte_free_defer pte_free_defer
void pte_free_defer(struct mm_struct *mm, pgtable_t pgtable);

void vmem_map_init(void);
void *vmem_crst_alloc(unsigned long val);
pte_t *vmem_pte_alloc(void);

unsigned long base_asce_alloc(unsigned long addr, unsigned long num_pages);
void base_asce_free(unsigned long asce);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _S390_PGALLOC_H */
