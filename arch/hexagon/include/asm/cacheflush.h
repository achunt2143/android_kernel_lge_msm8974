<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cache flush operations for the Hexagon architecture
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ASM_CACHEFLUSH_H
#define _ASM_CACHEFLUSH_H

<<<<<<< HEAD
#include <linux/cache.h>
#include <linux/mm.h>
#include <asm/string.h>
#include <asm-generic/cacheflush.h>
=======
#include <linux/mm_types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Cache flushing:
 *
 *  - flush_cache_all() flushes entire cache
 *  - flush_cache_mm(mm) flushes the specified mm context's cache lines
 *  - flush_cache_page(mm, vmaddr, pfn) flushes a single page
 *  - flush_cache_range(vma, start, end) flushes a range of pages
 *  - flush_icache_range(start, end) flush a range of instructions
 *  - flush_dcache_page(pg) flushes(wback&invalidates) a page for dcache
<<<<<<< HEAD
 *  - flush_icache_page(vma, pg) flushes(invalidates) a page for icache
=======
 *  - flush_icache_pages(vma, pg, nr) flushes(invalidates) nr pages for icache
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  Need to doublecheck which one is really needed for ptrace stuff to work.
 */
#define LINESIZE	32
#define LINEBITS	5

/*
 * Flush Dcache range through current map.
 */
extern void flush_dcache_range(unsigned long start, unsigned long end);
<<<<<<< HEAD
=======
#define flush_dcache_range flush_dcache_range
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Flush Icache range through current map.
 */
<<<<<<< HEAD
#undef flush_icache_range
extern void flush_icache_range(unsigned long start, unsigned long end);
=======
extern void flush_icache_range(unsigned long start, unsigned long end);
#define flush_icache_range flush_icache_range
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Memory-management related flushes are there to ensure in non-physically
 * indexed cache schemes that stale lines belonging to a given ASID aren't
 * in the cache to confuse things.  The prototype Hexagon Virtual Machine
 * only uses a single ASID for all user-mode maps, which should
 * mean that they aren't necessary.  A brute-force, flush-everything
 * implementation, with the name xxxxx_hexagon() is present in
 * arch/hexagon/mm/cache.c, but let's not wire it up until we know
 * it is needed.
 */
extern void flush_cache_all_hexagon(void);

/*
 * This may or may not ever have to be non-null, depending on the
 * virtual machine MMU.  For a native kernel, it's definitiely  a no-op
 *
 * This is also the place where deferred cache coherency stuff seems
 * to happen, classically...  but instead we do it like ia64 and
 * clean the cache when the PTE is set.
 *
 */
<<<<<<< HEAD
static inline void update_mmu_cache(struct vm_area_struct *vma,
					unsigned long address, pte_t *ptep)
=======
static inline void update_mmu_cache_range(struct vm_fault *vmf,
		struct vm_area_struct *vma, unsigned long address,
		pte_t *ptep, unsigned int nr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*  generic_ptrace_pokedata doesn't wind up here, does it?  */
}

<<<<<<< HEAD
#undef copy_to_user_page
static inline void copy_to_user_page(struct vm_area_struct *vma,
					     struct page *page,
					     unsigned long vaddr,
					     void *dst, void *src, int len)
{
	memcpy(dst, src, len);
	if (vma->vm_flags & VM_EXEC) {
		flush_icache_range((unsigned long) dst,
		(unsigned long) dst + len);
	}
}

=======
#define update_mmu_cache(vma, addr, ptep) \
	update_mmu_cache_range(NULL, vma, addr, ptep, 1)

void copy_to_user_page(struct vm_area_struct *vma, struct page *page,
		       unsigned long vaddr, void *dst, void *src, int len);
#define copy_to_user_page copy_to_user_page

#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void hexagon_inv_dcache_range(unsigned long start, unsigned long end);
extern void hexagon_clean_dcache_range(unsigned long start, unsigned long end);

<<<<<<< HEAD
=======
#include <asm-generic/cacheflush.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
