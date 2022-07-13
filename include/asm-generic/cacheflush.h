<<<<<<< HEAD
#ifndef __ASM_CACHEFLUSH_H
#define __ASM_CACHEFLUSH_H

/* Keep includes the same across arches.  */
#include <linux/mm.h>
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_CACHEFLUSH_H
#define _ASM_GENERIC_CACHEFLUSH_H

#include <linux/instrumented.h>

struct mm_struct;
struct vm_area_struct;
struct page;
struct address_space;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The cache doesn't need to be flushed when TLB entries change when
 * the cache is mapped to physical memory, not virtual memory
 */
<<<<<<< HEAD
#define flush_cache_all()			do { } while (0)
#define flush_cache_mm(mm)			do { } while (0)
#define flush_cache_dup_mm(mm)			do { } while (0)
#define flush_cache_range(vma, start, end)	do { } while (0)
#define flush_cache_page(vma, vmaddr, pfn)	do { } while (0)
#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 0
#define flush_dcache_page(page)			do { } while (0)
#define flush_dcache_mmap_lock(mapping)		do { } while (0)
#define flush_dcache_mmap_unlock(mapping)	do { } while (0)
#define flush_icache_range(start, end)		do { } while (0)
#define flush_icache_page(vma,pg)		do { } while (0)
#define flush_icache_user_range(vma,pg,adr,len)	do { } while (0)
#define flush_cache_vmap(start, end)		do { } while (0)
#define flush_cache_vunmap(start, end)		do { } while (0)

#define copy_to_user_page(vma, page, vaddr, dst, src, len) \
	do { \
		memcpy(dst, src, len); \
		flush_icache_user_range(vma, page, vaddr, len); \
	} while (0)
#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)

#endif /* __ASM_CACHEFLUSH_H */
=======
#ifndef flush_cache_all
static inline void flush_cache_all(void)
{
}
#endif

#ifndef flush_cache_mm
static inline void flush_cache_mm(struct mm_struct *mm)
{
}
#endif

#ifndef flush_cache_dup_mm
static inline void flush_cache_dup_mm(struct mm_struct *mm)
{
}
#endif

#ifndef flush_cache_range
static inline void flush_cache_range(struct vm_area_struct *vma,
				     unsigned long start,
				     unsigned long end)
{
}
#endif

#ifndef flush_cache_page
static inline void flush_cache_page(struct vm_area_struct *vma,
				    unsigned long vmaddr,
				    unsigned long pfn)
{
}
#endif

#ifndef ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE
static inline void flush_dcache_page(struct page *page)
{
}

#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 0
#endif

#ifndef flush_dcache_mmap_lock
static inline void flush_dcache_mmap_lock(struct address_space *mapping)
{
}
#endif

#ifndef flush_dcache_mmap_unlock
static inline void flush_dcache_mmap_unlock(struct address_space *mapping)
{
}
#endif

#ifndef flush_icache_range
static inline void flush_icache_range(unsigned long start, unsigned long end)
{
}
#endif

#ifndef flush_icache_user_range
#define flush_icache_user_range flush_icache_range
#endif

#ifndef flush_icache_user_page
static inline void flush_icache_user_page(struct vm_area_struct *vma,
					   struct page *page,
					   unsigned long addr, int len)
{
}
#endif

#ifndef flush_cache_vmap
static inline void flush_cache_vmap(unsigned long start, unsigned long end)
{
}
#endif

#ifndef flush_cache_vmap_early
static inline void flush_cache_vmap_early(unsigned long start, unsigned long end)
{
}
#endif

#ifndef flush_cache_vunmap
static inline void flush_cache_vunmap(unsigned long start, unsigned long end)
{
}
#endif

#ifndef copy_to_user_page
#define copy_to_user_page(vma, page, vaddr, dst, src, len)	\
	do { \
		instrument_copy_to_user((void __user *)dst, src, len); \
		memcpy(dst, src, len); \
		flush_icache_user_page(vma, page, vaddr, len); \
	} while (0)
#endif


#ifndef copy_from_user_page
#define copy_from_user_page(vma, page, vaddr, dst, src, len)		  \
	do {								  \
		instrument_copy_from_user_before(dst, (void __user *)src, \
						 len);			  \
		memcpy(dst, src, len);					  \
		instrument_copy_from_user_after(dst, (void __user *)src, len, \
						0);			  \
	} while (0)
#endif

#endif /* _ASM_GENERIC_CACHEFLUSH_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
