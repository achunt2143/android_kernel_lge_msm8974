<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __PARISC_MMU_CONTEXT_H
#define __PARISC_MMU_CONTEXT_H

#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/atomic.h>
<<<<<<< HEAD
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm-generic/mm_hooks.h>

static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk)
{
}

=======
#include <linux/spinlock.h>
#include <asm-generic/mm_hooks.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* on PA-RISC, we actually have enough contexts to justify an allocator
 * for them.  prumpf */

extern unsigned long alloc_sid(void);
extern void free_sid(unsigned long);

<<<<<<< HEAD
=======
#define init_new_context init_new_context
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int
init_new_context(struct task_struct *tsk, struct mm_struct *mm)
{
	BUG_ON(atomic_read(&mm->mm_users) != 1);

<<<<<<< HEAD
	mm->context = alloc_sid();
	return 0;
}

static inline void
destroy_context(struct mm_struct *mm)
{
	free_sid(mm->context);
	mm->context = 0;
=======
	mm->context.space_id = alloc_sid();
	return 0;
}

#define destroy_context destroy_context
static inline void
destroy_context(struct mm_struct *mm)
{
	free_sid(mm->context.space_id);
	mm->context.space_id = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline unsigned long __space_to_prot(mm_context_t context)
{
#if SPACEID_SHIFT == 0
<<<<<<< HEAD
	return context << 1;
#else
	return context >> (SPACEID_SHIFT - 1);
=======
	return context.space_id << 1;
#else
	return context.space_id >> (SPACEID_SHIFT - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

static inline void load_context(mm_context_t context)
{
<<<<<<< HEAD
	mtsp(context, 3);
	mtctl(__space_to_prot(context), 8);
}

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next, struct task_struct *tsk)
{

	if (prev != next) {
=======
	mtsp(context.space_id, SR_USER);
	mtctl(__space_to_prot(context), 8);
}

static inline void switch_mm_irqs_off(struct mm_struct *prev,
		struct mm_struct *next, struct task_struct *tsk)
{
	if (prev != next) {
#ifdef CONFIG_TLB_PTLOCK
		/* put physical address of page_table_lock in cr28 (tr4)
		   for TLB faults */
		spinlock_t *pgd_lock = &next->page_table_lock;
		mtctl(__pa(__ldcw_align(&pgd_lock->rlock.raw_lock)), 28);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mtctl(__pa(next->pgd), 25);
		load_context(next->context);
	}
}

<<<<<<< HEAD
#define deactivate_mm(tsk,mm)	do { } while (0)

=======
static inline void switch_mm(struct mm_struct *prev,
		struct mm_struct *next, struct task_struct *tsk)
{
	unsigned long flags;

	if (prev == next)
		return;

	local_irq_save(flags);
	switch_mm_irqs_off(prev, next, tsk);
	local_irq_restore(flags);
}
#define switch_mm_irqs_off switch_mm_irqs_off

#define activate_mm activate_mm
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void activate_mm(struct mm_struct *prev, struct mm_struct *next)
{
	/*
	 * Activate_mm is our one chance to allocate a space id
	 * for a new mm created in the exec path. There's also
	 * some lazy tlb stuff, which is currently dead code, but
	 * we only allocate a space id if one hasn't been allocated
	 * already, so we should be OK.
	 */

	BUG_ON(next == &init_mm); /* Should never happen */

<<<<<<< HEAD
	if (next->context == 0)
	    next->context = alloc_sid();

	switch_mm(prev,next,current);
}
=======
	if (next->context.space_id == 0)
		next->context.space_id = alloc_sid();

	switch_mm(prev,next,current);
}

#include <asm-generic/mmu_context.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
