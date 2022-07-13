<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _SPARC64_TLB_H
#define _SPARC64_TLB_H

#include <linux/swap.h>
#include <linux/pagemap.h>
<<<<<<< HEAD
#include <asm/pgalloc.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/tlbflush.h>
#include <asm/mmu_context.h>

#ifdef CONFIG_SMP
<<<<<<< HEAD
extern void smp_flush_tlb_pending(struct mm_struct *,
=======
void smp_flush_tlb_pending(struct mm_struct *,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  unsigned long, unsigned long *);
#endif

#ifdef CONFIG_SMP
<<<<<<< HEAD
extern void smp_flush_tlb_mm(struct mm_struct *mm);
=======
void smp_flush_tlb_mm(struct mm_struct *mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define do_flush_tlb_mm(mm) smp_flush_tlb_mm(mm)
#else
#define do_flush_tlb_mm(mm) __flush_tlb_mm(CTX_HWBITS(mm->context), SECONDARY_CONTEXT)
#endif

<<<<<<< HEAD
extern void __flush_tlb_pending(unsigned long, unsigned long, unsigned long *);
extern void flush_tlb_pending(void);

#define tlb_start_vma(tlb, vma) do { } while (0)
#define tlb_end_vma(tlb, vma)	do { } while (0)
#define __tlb_remove_tlb_entry(tlb, ptep, address) do { } while (0)
#define tlb_flush(tlb)	flush_tlb_pending()

=======
void __flush_tlb_pending(unsigned long, unsigned long, unsigned long *);
void flush_tlb_pending(void);

#define tlb_flush(tlb)	flush_tlb_pending()

/*
 * SPARC64's hardware TLB fill does not use the Linux page-tables
 * and therefore we don't need a TLBI when freeing page-table pages.
 */

#ifdef CONFIG_MMU_GATHER_RCU_TABLE_FREE
#define tlb_needs_table_invalidate()	(false)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/tlb.h>

#endif /* _SPARC64_TLB_H */
