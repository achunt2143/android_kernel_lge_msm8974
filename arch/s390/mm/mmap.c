<<<<<<< HEAD
/*
 *  linux/arch/s390/mm/mmap.c
 *
=======
// SPDX-License-Identifier: GPL-2.0+
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  flexible mmap layout support
 *
 * Copyright 2003-2004 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Started by Ingo Molnar <mingo@elte.hu>
 */

#include <linux/personality.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/compat.h>
#include <asm/pgalloc.h>
=======
 * Started by Ingo Molnar <mingo@elte.hu>
 */

#include <linux/elf-randomize.h>
#include <linux/personality.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/sched/signal.h>
#include <linux/sched/mm.h>
#include <linux/random.h>
#include <linux/compat.h>
#include <linux/security.h>
#include <asm/elf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static unsigned long stack_maxrandom_size(void)
{
	if (!(current->flags & PF_RANDOMIZE))
		return 0;
<<<<<<< HEAD
	if (current->personality & ADDR_NO_RANDOMIZE)
		return 0;
	return STACK_RND_MASK << PAGE_SHIFT;
}

/*
 * Top of mmap area (just below the process stack).
 *
 * Leave at least a ~32 MB hole.
 */
#define MIN_GAP (32*1024*1024)
#define MAX_GAP (STACK_TOP/6*5)

static inline int mmap_is_legacy(void)
{
	if (current->personality & ADDR_COMPAT_LAYOUT)
		return 1;
	if (rlimit(RLIMIT_STACK) == RLIM_INFINITY)
=======
	return STACK_RND_MASK << PAGE_SHIFT;
}

static inline int mmap_is_legacy(struct rlimit *rlim_stack)
{
	if (current->personality & ADDR_COMPAT_LAYOUT)
		return 1;
	if (rlim_stack->rlim_cur == RLIM_INFINITY)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	return sysctl_legacy_va_layout;
}

<<<<<<< HEAD
static unsigned long mmap_rnd(void)
{
	if (!(current->flags & PF_RANDOMIZE))
		return 0;
	/* 8MB randomization for mmap_base */
	return (get_random_int() & 0x7ffUL) << PAGE_SHIFT;
}

static inline unsigned long mmap_base(void)
{
	unsigned long gap = rlimit(RLIMIT_STACK);

	if (gap < MIN_GAP)
		gap = MIN_GAP;
	else if (gap > MAX_GAP)
		gap = MAX_GAP;
	gap &= PAGE_MASK;
	return STACK_TOP - stack_maxrandom_size() - mmap_rnd() - gap;
}

#ifndef CONFIG_64BIT

/*
 * This function, called very early during the creation of a new
 * process VM image, sets up which VM layout function to use:
 */
void arch_pick_mmap_layout(struct mm_struct *mm)
{
	/*
	 * Fall back to the standard layout if the personality
	 * bit is set, or if the expected stack growth is unlimited:
	 */
	if (mmap_is_legacy()) {
		mm->mmap_base = TASK_UNMAPPED_BASE;
		mm->get_unmapped_area = arch_get_unmapped_area;
		mm->unmap_area = arch_unmap_area;
	} else {
		mm->mmap_base = mmap_base();
		mm->get_unmapped_area = arch_get_unmapped_area_topdown;
		mm->unmap_area = arch_unmap_area_topdown;
	}
}

#else

int s390_mmap_check(unsigned long addr, unsigned long len)
{
	int rc;

	if (!is_compat_task() &&
	    len >= TASK_SIZE && TASK_SIZE < (1UL << 53)) {
		rc = crst_table_upgrade(current->mm, 1UL << 53);
		if (rc)
			return rc;
		update_mm(current->mm, current);
	}
	return 0;
}

static unsigned long
s390_get_unmapped_area(struct file *filp, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	unsigned long area;
	int rc;

	area = arch_get_unmapped_area(filp, addr, len, pgoff, flags);
	if (!(area & ~PAGE_MASK))
		return area;
	if (area == -ENOMEM && !is_compat_task() && TASK_SIZE < (1UL << 53)) {
		/* Upgrade the page table to 4 levels and retry. */
		rc = crst_table_upgrade(mm, 1UL << 53);
		if (rc)
			return (unsigned long) rc;
		update_mm(mm, current);
		area = arch_get_unmapped_area(filp, addr, len, pgoff, flags);
	}
	return area;
}

static unsigned long
s390_get_unmapped_area_topdown(struct file *filp, const unsigned long addr,
			  const unsigned long len, const unsigned long pgoff,
			  const unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	unsigned long area;
	int rc;

	area = arch_get_unmapped_area_topdown(filp, addr, len, pgoff, flags);
	if (!(area & ~PAGE_MASK))
		return area;
	if (area == -ENOMEM && !is_compat_task() && TASK_SIZE < (1UL << 53)) {
		/* Upgrade the page table to 4 levels and retry. */
		rc = crst_table_upgrade(mm, 1UL << 53);
		if (rc)
			return (unsigned long) rc;
		update_mm(mm, current);
		area = arch_get_unmapped_area_topdown(filp, addr, len,
						      pgoff, flags);
	}
	return area;
}
=======
unsigned long arch_mmap_rnd(void)
{
	return (get_random_u32() & MMAP_RND_MASK) << PAGE_SHIFT;
}

static unsigned long mmap_base_legacy(unsigned long rnd)
{
	return TASK_UNMAPPED_BASE + rnd;
}

static inline unsigned long mmap_base(unsigned long rnd,
				      struct rlimit *rlim_stack)
{
	unsigned long gap = rlim_stack->rlim_cur;
	unsigned long pad = stack_maxrandom_size() + stack_guard_gap;
	unsigned long gap_min, gap_max;

	/* Values close to RLIM_INFINITY can overflow. */
	if (gap + pad > gap)
		gap += pad;

	/*
	 * Top of mmap area (just below the process stack).
	 * Leave at least a ~128 MB hole.
	 */
	gap_min = SZ_128M;
	gap_max = (STACK_TOP / 6) * 5;

	if (gap < gap_min)
		gap = gap_min;
	else if (gap > gap_max)
		gap = gap_max;

	return PAGE_ALIGN(STACK_TOP - gap - rnd);
}

static int get_align_mask(struct file *filp, unsigned long flags)
{
	if (!(current->flags & PF_RANDOMIZE))
		return 0;
	if (filp || (flags & MAP_SHARED))
		return MMAP_ALIGN_MASK << PAGE_SHIFT;
	return 0;
}

unsigned long arch_get_unmapped_area(struct file *filp, unsigned long addr,
				     unsigned long len, unsigned long pgoff,
				     unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	struct vm_unmapped_area_info info;

	if (len > TASK_SIZE - mmap_min_addr)
		return -ENOMEM;

	if (flags & MAP_FIXED)
		goto check_asce_limit;

	if (addr) {
		addr = PAGE_ALIGN(addr);
		vma = find_vma(mm, addr);
		if (TASK_SIZE - len >= addr && addr >= mmap_min_addr &&
		    (!vma || addr + len <= vm_start_gap(vma)))
			goto check_asce_limit;
	}

	info.flags = 0;
	info.length = len;
	info.low_limit = mm->mmap_base;
	info.high_limit = TASK_SIZE;
	info.align_mask = get_align_mask(filp, flags);
	info.align_offset = pgoff << PAGE_SHIFT;
	addr = vm_unmapped_area(&info);
	if (offset_in_page(addr))
		return addr;

check_asce_limit:
	return check_asce_limit(mm, addr, len);
}

unsigned long arch_get_unmapped_area_topdown(struct file *filp, unsigned long addr,
					     unsigned long len, unsigned long pgoff,
					     unsigned long flags)
{
	struct vm_area_struct *vma;
	struct mm_struct *mm = current->mm;
	struct vm_unmapped_area_info info;

	/* requested length too big for entire address space */
	if (len > TASK_SIZE - mmap_min_addr)
		return -ENOMEM;

	if (flags & MAP_FIXED)
		goto check_asce_limit;

	/* requesting a specific address */
	if (addr) {
		addr = PAGE_ALIGN(addr);
		vma = find_vma(mm, addr);
		if (TASK_SIZE - len >= addr && addr >= mmap_min_addr &&
				(!vma || addr + len <= vm_start_gap(vma)))
			goto check_asce_limit;
	}

	info.flags = VM_UNMAPPED_AREA_TOPDOWN;
	info.length = len;
	info.low_limit = PAGE_SIZE;
	info.high_limit = mm->mmap_base;
	info.align_mask = get_align_mask(filp, flags);
	info.align_offset = pgoff << PAGE_SHIFT;
	addr = vm_unmapped_area(&info);

	/*
	 * A failed mmap() very likely causes application failure,
	 * so fall back to the bottom-up function here. This scenario
	 * can happen with large stack limits and large mmap()
	 * allocations.
	 */
	if (offset_in_page(addr)) {
		VM_BUG_ON(addr != -ENOMEM);
		info.flags = 0;
		info.low_limit = TASK_UNMAPPED_BASE;
		info.high_limit = TASK_SIZE;
		addr = vm_unmapped_area(&info);
		if (offset_in_page(addr))
			return addr;
	}

check_asce_limit:
	return check_asce_limit(mm, addr, len);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This function, called very early during the creation of a new
 * process VM image, sets up which VM layout function to use:
 */
<<<<<<< HEAD
void arch_pick_mmap_layout(struct mm_struct *mm)
{
=======
void arch_pick_mmap_layout(struct mm_struct *mm, struct rlimit *rlim_stack)
{
	unsigned long random_factor = 0UL;

	if (current->flags & PF_RANDOMIZE)
		random_factor = arch_mmap_rnd();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Fall back to the standard layout if the personality
	 * bit is set, or if the expected stack growth is unlimited:
	 */
<<<<<<< HEAD
	if (mmap_is_legacy()) {
		mm->mmap_base = TASK_UNMAPPED_BASE;
		mm->get_unmapped_area = s390_get_unmapped_area;
		mm->unmap_area = arch_unmap_area;
	} else {
		mm->mmap_base = mmap_base();
		mm->get_unmapped_area = s390_get_unmapped_area_topdown;
		mm->unmap_area = arch_unmap_area_topdown;
	}
}

#endif
=======
	if (mmap_is_legacy(rlim_stack)) {
		mm->mmap_base = mmap_base_legacy(random_factor);
		mm->get_unmapped_area = arch_get_unmapped_area;
	} else {
		mm->mmap_base = mmap_base(random_factor, rlim_stack);
		mm->get_unmapped_area = arch_get_unmapped_area_topdown;
	}
}

static const pgprot_t protection_map[16] = {
	[VM_NONE]					= PAGE_NONE,
	[VM_READ]					= PAGE_RO,
	[VM_WRITE]					= PAGE_RO,
	[VM_WRITE | VM_READ]				= PAGE_RO,
	[VM_EXEC]					= PAGE_RX,
	[VM_EXEC | VM_READ]				= PAGE_RX,
	[VM_EXEC | VM_WRITE]				= PAGE_RX,
	[VM_EXEC | VM_WRITE | VM_READ]			= PAGE_RX,
	[VM_SHARED]					= PAGE_NONE,
	[VM_SHARED | VM_READ]				= PAGE_RO,
	[VM_SHARED | VM_WRITE]				= PAGE_RW,
	[VM_SHARED | VM_WRITE | VM_READ]		= PAGE_RW,
	[VM_SHARED | VM_EXEC]				= PAGE_RX,
	[VM_SHARED | VM_EXEC | VM_READ]			= PAGE_RX,
	[VM_SHARED | VM_EXEC | VM_WRITE]		= PAGE_RWX,
	[VM_SHARED | VM_EXEC | VM_WRITE | VM_READ]	= PAGE_RWX
};
DECLARE_VM_GET_PAGE_PROT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
