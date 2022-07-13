<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _M68KNOMMU_CACHEFLUSH_H
#define _M68KNOMMU_CACHEFLUSH_H

/*
 * (C) Copyright 2000-2010, Greg Ungerer <gerg@snapgear.com>
 */
#include <linux/mm.h>
#include <asm/mcfsim.h>

#define flush_cache_all()			__flush_cache_all()
<<<<<<< HEAD
#define flush_cache_mm(mm)			do { } while (0)
#define flush_cache_dup_mm(mm)			do { } while (0)
#define flush_cache_range(vma, start, end)	do { } while (0)
#define flush_cache_page(vma, vmaddr)		do { } while (0)
#define flush_dcache_range(start, len)		__flush_dcache_all()
#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 0
#define flush_dcache_page(page)			do { } while (0)
#define flush_dcache_mmap_lock(mapping)		do { } while (0)
#define flush_dcache_mmap_unlock(mapping)	do { } while (0)
#define flush_icache_range(start, len)		__flush_icache_all()
#define flush_icache_page(vma,pg)		do { } while (0)
#define flush_icache_user_range(vma,pg,adr,len)	do { } while (0)
#define flush_cache_vmap(start, end)		do { } while (0)
#define flush_cache_vunmap(start, end)		do { } while (0)

#define copy_to_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)
#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)

void mcf_cache_push(void);

=======
#define flush_dcache_range(start, len)		__flush_dcache_all()
#define flush_icache_range(start, len)		__flush_icache_all()

void mcf_cache_push(void);

static inline void __clear_cache_all(void)
{
#ifdef CACHE_INVALIDATE
	__asm__ __volatile__ (
		"movec	%0, %%CACR\n\t"
		"nop\n\t"
		: : "r" (CACHE_INVALIDATE) );
#endif
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void __flush_cache_all(void)
{
#ifdef CACHE_PUSH
	mcf_cache_push();
#endif
<<<<<<< HEAD
#ifdef CACHE_INVALIDATE
	__asm__ __volatile__ (
		"movel	%0, %%d0\n\t"
		"movec	%%d0, %%CACR\n\t"
		"nop\n\t"
		: : "i" (CACHE_INVALIDATE) : "d0" );
#endif
=======
	__clear_cache_all();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Some ColdFire parts implement separate instruction and data caches,
 * on those we should just flush the appropriate cache. If we don't need
 * to do any specific flushing then this will be optimized away.
 */
static inline void __flush_icache_all(void)
{
#ifdef CACHE_INVALIDATEI
	__asm__ __volatile__ (
<<<<<<< HEAD
		"movel	%0, %%d0\n\t"
		"movec	%%d0, %%CACR\n\t"
		"nop\n\t"
		: : "i" (CACHE_INVALIDATEI) : "d0" );
=======
		"movec	%0, %%CACR\n\t"
		"nop\n\t"
		: : "r" (CACHE_INVALIDATEI) );
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

static inline void __flush_dcache_all(void)
{
#ifdef CACHE_PUSH
	mcf_cache_push();
#endif
#ifdef CACHE_INVALIDATED
	__asm__ __volatile__ (
<<<<<<< HEAD
		"movel	%0, %%d0\n\t"
		"movec	%%d0, %%CACR\n\t"
		"nop\n\t"
		: : "i" (CACHE_INVALIDATED) : "d0" );
#else
	/* Flush the wrtite buffer */
	__asm__ __volatile__ ( "nop" );
#endif
}
=======
		"movec	%0, %%CACR\n\t"
		"nop\n\t"
		: : "r" (CACHE_INVALIDATED) );
#else
	/* Flush the write buffer */
	__asm__ __volatile__ ( "nop" );
#endif
}

/*
 * Push cache entries at supplied address. We want to write back any dirty
 * data and then invalidate the cache lines associated with this address.
 */
static inline void cache_push(unsigned long paddr, int len)
{
	__flush_cache_all();
}

/*
 * Clear cache entries at supplied address (that is don't write back any
 * dirty data).
 */
static inline void cache_clear(unsigned long paddr, int len)
{
	__clear_cache_all();
}

#include <asm-generic/cacheflush.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _M68KNOMMU_CACHEFLUSH_H */
