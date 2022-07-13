<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _PGTABLE_NOPUD_H
#define _PGTABLE_NOPUD_H

#ifndef __ASSEMBLY__

<<<<<<< HEAD
#define __PAGETABLE_PUD_FOLDED

/*
 * Having the pud type consist of a pgd gets the size right, and allows
 * us to conceptually access the pgd entry that this pud is folded into
 * without casting.
 */
typedef struct { pgd_t pgd; } pud_t;

#define PUD_SHIFT	PGDIR_SHIFT
=======
#include <asm-generic/pgtable-nop4d.h>

#define __PAGETABLE_PUD_FOLDED 1

/*
 * Having the pud type consist of a p4d gets the size right, and allows
 * us to conceptually access the p4d entry that this pud is folded into
 * without casting.
 */
typedef struct { p4d_t p4d; } pud_t;

#define PUD_SHIFT	P4D_SHIFT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PTRS_PER_PUD	1
#define PUD_SIZE  	(1UL << PUD_SHIFT)
#define PUD_MASK  	(~(PUD_SIZE-1))

/*
<<<<<<< HEAD
 * The "pgd_xxx()" functions here are trivial for a folded two-level
 * setup: the pud is never bad, and a pud always exists (as it's folded
 * into the pgd entry)
 */
static inline int pgd_none(pgd_t pgd)		{ return 0; }
static inline int pgd_bad(pgd_t pgd)		{ return 0; }
static inline int pgd_present(pgd_t pgd)	{ return 1; }
static inline void pgd_clear(pgd_t *pgd)	{ }
#define pud_ERROR(pud)				(pgd_ERROR((pud).pgd))

#define pgd_populate(mm, pgd, pud)		do { } while (0)
/*
 * (puds are folded into pgds so this doesn't get actually called,
 * but the define is needed for a generic inline function.)
 */
#define set_pgd(pgdptr, pgdval)			set_pud((pud_t *)(pgdptr), (pud_t) { pgdval })

static inline pud_t * pud_offset(pgd_t * pgd, unsigned long address)
{
	return (pud_t *)pgd;
}

#define pud_val(x)				(pgd_val((x).pgd))
#define __pud(x)				((pud_t) { __pgd(x) } )

#define pgd_page(pgd)				(pud_page((pud_t){ pgd }))
#define pgd_page_vaddr(pgd)			(pud_page_vaddr((pud_t){ pgd }))

/*
 * allocating and freeing a pud is trivial: the 1-entry pud is
 * inside the pgd, so has no extra memory associated with it.
 */
#define pud_alloc_one(mm, address)		NULL
#define pud_free(mm, x)				do { } while (0)
#define __pud_free_tlb(tlb, x, a)		do { } while (0)
=======
 * The "p4d_xxx()" functions here are trivial for a folded two-level
 * setup: the pud is never bad, and a pud always exists (as it's folded
 * into the p4d entry)
 */
static inline int p4d_none(p4d_t p4d)		{ return 0; }
static inline int p4d_bad(p4d_t p4d)		{ return 0; }
static inline int p4d_present(p4d_t p4d)	{ return 1; }
static inline void p4d_clear(p4d_t *p4d)	{ }
#define pud_ERROR(pud)				(p4d_ERROR((pud).p4d))

#define p4d_populate(mm, p4d, pud)		do { } while (0)
#define p4d_populate_safe(mm, p4d, pud)		do { } while (0)
/*
 * (puds are folded into p4ds so this doesn't get actually called,
 * but the define is needed for a generic inline function.)
 */
#define set_p4d(p4dptr, p4dval)	set_pud((pud_t *)(p4dptr), (pud_t) { p4dval })

static inline pud_t *pud_offset(p4d_t *p4d, unsigned long address)
{
	return (pud_t *)p4d;
}
#define pud_offset pud_offset

#define pud_val(x)				(p4d_val((x).p4d))
#define __pud(x)				((pud_t) { __p4d(x) })

#define p4d_page(p4d)				(pud_page((pud_t){ p4d }))
#define p4d_pgtable(p4d)			((pud_t *)(pud_pgtable((pud_t){ p4d })))

/*
 * allocating and freeing a pud is trivial: the 1-entry pud is
 * inside the p4d, so has no extra memory associated with it.
 */
#define pud_alloc_one(mm, address)		NULL
#define pud_free(mm, x)				do { } while (0)
#define pud_free_tlb(tlb, x, a)		        do { } while (0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#undef  pud_addr_end
#define pud_addr_end(addr, end)			(end)

#endif /* __ASSEMBLY__ */
#endif /* _PGTABLE_NOPUD_H */
