<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_HIGHMEM_H
#define _LINUX_HIGHMEM_H

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/bug.h>
<<<<<<< HEAD
=======
#include <linux/cacheflush.h>
#include <linux/kmsan.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/hardirq.h>

<<<<<<< HEAD
#include <asm/cacheflush.h>
=======
#include "highmem-internal.h"

/**
 * kmap - Map a page for long term usage
 * @page:	Pointer to the page to be mapped
 *
 * Returns: The virtual address of the mapping
 *
 * Can only be invoked from preemptible task context because on 32bit
 * systems with CONFIG_HIGHMEM enabled this function might sleep.
 *
 * For systems with CONFIG_HIGHMEM=n and for pages in the low memory area
 * this returns the virtual address of the direct kernel mapping.
 *
 * The returned virtual address is globally visible and valid up to the
 * point where it is unmapped via kunmap(). The pointer can be handed to
 * other contexts.
 *
 * For highmem pages on 32bit systems this can be slow as the mapping space
 * is limited and protected by a global lock. In case that there is no
 * mapping slot available the function blocks until a slot is released via
 * kunmap().
 */
static inline void *kmap(struct page *page);

/**
 * kunmap - Unmap the virtual address mapped by kmap()
 * @page:	Pointer to the page which was mapped by kmap()
 *
 * Counterpart to kmap(). A NOOP for CONFIG_HIGHMEM=n and for mappings of
 * pages in the low memory area.
 */
static inline void kunmap(struct page *page);

/**
 * kmap_to_page - Get the page for a kmap'ed address
 * @addr:	The address to look up
 *
 * Returns: The page which is mapped to @addr.
 */
static inline struct page *kmap_to_page(void *addr);

/**
 * kmap_flush_unused - Flush all unused kmap mappings in order to
 *		       remove stray mappings
 */
static inline void kmap_flush_unused(void);

/**
 * kmap_local_page - Map a page for temporary usage
 * @page: Pointer to the page to be mapped
 *
 * Returns: The virtual address of the mapping
 *
 * Can be invoked from any context, including interrupts.
 *
 * Requires careful handling when nesting multiple mappings because the map
 * management is stack based. The unmap has to be in the reverse order of
 * the map operation:
 *
 * addr1 = kmap_local_page(page1);
 * addr2 = kmap_local_page(page2);
 * ...
 * kunmap_local(addr2);
 * kunmap_local(addr1);
 *
 * Unmapping addr1 before addr2 is invalid and causes malfunction.
 *
 * Contrary to kmap() mappings the mapping is only valid in the context of
 * the caller and cannot be handed to other contexts.
 *
 * On CONFIG_HIGHMEM=n kernels and for low memory pages this returns the
 * virtual address of the direct mapping. Only real highmem pages are
 * temporarily mapped.
 *
 * While kmap_local_page() is significantly faster than kmap() for the highmem
 * case it comes with restrictions about the pointer validity.
 *
 * On HIGHMEM enabled systems mapping a highmem page has the side effect of
 * disabling migration in order to keep the virtual address stable across
 * preemption. No caller of kmap_local_page() can rely on this side effect.
 */
static inline void *kmap_local_page(struct page *page);

/**
 * kmap_local_folio - Map a page in this folio for temporary usage
 * @folio: The folio containing the page.
 * @offset: The byte offset within the folio which identifies the page.
 *
 * Requires careful handling when nesting multiple mappings because the map
 * management is stack based. The unmap has to be in the reverse order of
 * the map operation::
 *
 *   addr1 = kmap_local_folio(folio1, offset1);
 *   addr2 = kmap_local_folio(folio2, offset2);
 *   ...
 *   kunmap_local(addr2);
 *   kunmap_local(addr1);
 *
 * Unmapping addr1 before addr2 is invalid and causes malfunction.
 *
 * Contrary to kmap() mappings the mapping is only valid in the context of
 * the caller and cannot be handed to other contexts.
 *
 * On CONFIG_HIGHMEM=n kernels and for low memory pages this returns the
 * virtual address of the direct mapping. Only real highmem pages are
 * temporarily mapped.
 *
 * While it is significantly faster than kmap() for the highmem case it
 * comes with restrictions about the pointer validity.
 *
 * On HIGHMEM enabled systems mapping a highmem page has the side effect of
 * disabling migration in order to keep the virtual address stable across
 * preemption. No caller of kmap_local_folio() can rely on this side effect.
 *
 * Context: Can be invoked from any context.
 * Return: The virtual address of @offset.
 */
static inline void *kmap_local_folio(struct folio *folio, size_t offset);

/**
 * kmap_atomic - Atomically map a page for temporary usage - Deprecated!
 * @page:	Pointer to the page to be mapped
 *
 * Returns: The virtual address of the mapping
 *
 * In fact a wrapper around kmap_local_page() which also disables pagefaults
 * and, depending on PREEMPT_RT configuration, also CPU migration and
 * preemption. Therefore users should not count on the latter two side effects.
 *
 * Mappings should always be released by kunmap_atomic().
 *
 * Do not use in new code. Use kmap_local_page() instead.
 *
 * It is used in atomic context when code wants to access the contents of a
 * page that might be allocated from high memory (see __GFP_HIGHMEM), for
 * example a page in the pagecache.  The API has two functions, and they
 * can be used in a manner similar to the following::
 *
 *   // Find the page of interest.
 *   struct page *page = find_get_page(mapping, offset);
 *
 *   // Gain access to the contents of that page.
 *   void *vaddr = kmap_atomic(page);
 *
 *   // Do something to the contents of that page.
 *   memset(vaddr, 0, PAGE_SIZE);
 *
 *   // Unmap that page.
 *   kunmap_atomic(vaddr);
 *
 * Note that the kunmap_atomic() call takes the result of the kmap_atomic()
 * call, not the argument.
 *
 * If you need to map two pages because you want to copy from one page to
 * another you need to keep the kmap_atomic calls strictly nested, like:
 *
 * vaddr1 = kmap_atomic(page1);
 * vaddr2 = kmap_atomic(page2);
 *
 * memcpy(vaddr1, vaddr2, PAGE_SIZE);
 *
 * kunmap_atomic(vaddr2);
 * kunmap_atomic(vaddr1);
 */
static inline void *kmap_atomic(struct page *page);

/* Highmem related interfaces for management code */
static inline unsigned int nr_free_highpages(void);
static inline unsigned long totalhigh_pages(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef ARCH_HAS_FLUSH_ANON_PAGE
static inline void flush_anon_page(struct vm_area_struct *vma, struct page *page, unsigned long vmaddr)
{
}
#endif

<<<<<<< HEAD
#ifndef ARCH_HAS_FLUSH_KERNEL_DCACHE_PAGE
static inline void flush_kernel_dcache_page(struct page *page)
{
}
=======
#ifndef ARCH_IMPLEMENTS_FLUSH_KERNEL_VMAP_RANGE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void flush_kernel_vmap_range(void *vaddr, int size)
{
}
static inline void invalidate_kernel_vmap_range(void *vaddr, int size)
{
}
#endif

<<<<<<< HEAD
#include <asm/kmap_types.h>

#ifdef CONFIG_HIGHMEM
#include <asm/highmem.h>

/* declarations for linux/mm/highmem.c */
unsigned int nr_free_highpages(void);
extern unsigned long totalhigh_pages;

void kmap_flush_unused(void);

struct page *kmap_to_page(void *addr);

#ifdef CONFIG_ARCH_WANT_KMAP_ATOMIC_FLUSH
void kmap_atomic_flush_unused(void);
#else
static inline void kmap_atomic_flush_unused(void) { }
#endif

#else /* CONFIG_HIGHMEM */

static inline unsigned int nr_free_highpages(void) { return 0; }

static inline struct page *kmap_to_page(void *addr)
{
	return virt_to_page(addr);
}

#define totalhigh_pages 0UL

#ifndef ARCH_HAS_KMAP
static inline void *kmap(struct page *page)
{
	might_sleep();
	return page_address(page);
}

static inline void kunmap(struct page *page)
{
}

static inline void *kmap_atomic(struct page *page)
{
	pagefault_disable();
	return page_address(page);
}
#define kmap_atomic_prot(page, prot)	kmap_atomic(page)

static inline void __kunmap_atomic(void *addr)
{
	pagefault_enable();
}

#define kmap_atomic_pfn(pfn)	kmap_atomic(pfn_to_page(pfn))
#define kmap_atomic_to_page(ptr)	virt_to_page(ptr)

#define kmap_flush_unused()	do {} while(0)
#define kmap_atomic_flush_unused()	do {} while (0)
#endif

#endif /* CONFIG_HIGHMEM */

#if defined(CONFIG_HIGHMEM) || defined(CONFIG_X86_32)

DECLARE_PER_CPU(int, __kmap_atomic_idx);

static inline int kmap_atomic_idx_push(void)
{
	int idx = __this_cpu_inc_return(__kmap_atomic_idx) - 1;

#ifdef CONFIG_DEBUG_HIGHMEM
	WARN_ON_ONCE(in_irq() && !irqs_disabled());
	BUG_ON(idx > KM_TYPE_NR);
#endif
	return idx;
}

static inline int kmap_atomic_idx(void)
{
	return __this_cpu_read(__kmap_atomic_idx) - 1;
}

static inline void kmap_atomic_idx_pop(void)
{
#ifdef CONFIG_DEBUG_HIGHMEM
	int idx = __this_cpu_dec_return(__kmap_atomic_idx);

	BUG_ON(idx < 0);
#else
	__this_cpu_dec(__kmap_atomic_idx);
#endif
}

#endif

/*
 * NOTE:
 * kmap_atomic() and kunmap_atomic() with two arguments are deprecated.
 * We only keep them for backward compatibility, any usage of them
 * are now warned.
 */

#define PASTE(a, b) a ## b
#define PASTE2(a, b) PASTE(a, b)

#define NARG_(_2, _1, n, ...) n
#define NARG(...) NARG_(__VA_ARGS__, 2, 1, :)

static inline void __deprecated *kmap_atomic_deprecated(struct page *page,
							enum km_type km)
{
	return kmap_atomic(page);
}

#define kmap_atomic1(...) kmap_atomic(__VA_ARGS__)
#define kmap_atomic2(...) kmap_atomic_deprecated(__VA_ARGS__)
#define kmap_atomic(...) PASTE2(kmap_atomic, NARG(__VA_ARGS__)(__VA_ARGS__))

static inline void __deprecated __kunmap_atomic_deprecated(void *addr,
							enum km_type km)
{
	__kunmap_atomic(addr);
}

/*
 * Prevent people trying to call kunmap_atomic() as if it were kunmap()
 * kunmap_atomic() should get the return value of kmap_atomic, not the page.
 */
#define kunmap_atomic_deprecated(addr, km)                      \
do {                                                            \
	BUILD_BUG_ON(__same_type((addr), struct page *));       \
	__kunmap_atomic_deprecated(addr, km);                   \
} while (0)

#define kunmap_atomic_withcheck(addr)                           \
do {                                                            \
	BUILD_BUG_ON(__same_type((addr), struct page *));       \
	__kunmap_atomic(addr);                                  \
} while (0)

#define kunmap_atomic1(...) kunmap_atomic_withcheck(__VA_ARGS__)
#define kunmap_atomic2(...) kunmap_atomic_deprecated(__VA_ARGS__)
#define kunmap_atomic(...) PASTE2(kunmap_atomic, NARG(__VA_ARGS__)(__VA_ARGS__))
/**** End of C pre-processor tricks for deprecated macros ****/

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* when CONFIG_HIGHMEM is not set these will be plain clear/copy_page */
#ifndef clear_user_highpage
static inline void clear_user_highpage(struct page *page, unsigned long vaddr)
{
<<<<<<< HEAD
	void *addr = kmap_atomic(page);
	clear_user_page(addr, vaddr, page);
	kunmap_atomic(addr);
}
#endif

#ifndef __HAVE_ARCH_ALLOC_ZEROED_USER_HIGHPAGE
/**
 * __alloc_zeroed_user_highpage - Allocate a zeroed HIGHMEM page for a VMA with caller-specified movable GFP flags
 * @movableflags: The GFP flags related to the pages future ability to move like __GFP_MOVABLE
 * @vma: The VMA the page is to be allocated for
 * @vaddr: The virtual address the page will be inserted into
 *
 * This function will allocate a page for a VMA but the caller is expected
 * to specify via movableflags whether the page will be movable in the
 * future or not
 *
 * An architecture may override this function by defining
 * __HAVE_ARCH_ALLOC_ZEROED_USER_HIGHPAGE and providing their own
 * implementation.
 */
static inline struct page *
__alloc_zeroed_user_highpage(gfp_t movableflags,
			struct vm_area_struct *vma,
			unsigned long vaddr)
{
	struct page *page = alloc_page_vma(GFP_HIGHUSER | movableflags,
			vma, vaddr);

	if (page)
		clear_user_highpage(page, vaddr);

	return page;
}
#endif

/**
 * alloc_zeroed_user_highpage_movable - Allocate a zeroed HIGHMEM page for a VMA that the caller knows can move
 * @vma: The VMA the page is to be allocated for
 * @vaddr: The virtual address the page will be inserted into
 *
 * This function will allocate a page for a VMA that the caller knows will
 * be able to migrate in the future using move_pages() or reclaimed
 */
static inline struct page *
alloc_zeroed_user_highpage_movable(struct vm_area_struct *vma,
					unsigned long vaddr)
{
#ifndef CONFIG_CMA
	return __alloc_zeroed_user_highpage(__GFP_MOVABLE, vma, vaddr);
#else
	return __alloc_zeroed_user_highpage(__GFP_MOVABLE|__GFP_CMA, vma,
						vaddr);
#endif
}

#ifdef CONFIG_CMA
static inline struct page *
alloc_zeroed_user_highpage_movable_cma(struct vm_area_struct *vma,
						unsigned long vaddr)
{
	return __alloc_zeroed_user_highpage(__GFP_MOVABLE|__GFP_CMA, vma,
						vaddr);
=======
	void *addr = kmap_local_page(page);
	clear_user_page(addr, vaddr, page);
	kunmap_local(addr);
}
#endif

#ifndef vma_alloc_zeroed_movable_folio
/**
 * vma_alloc_zeroed_movable_folio - Allocate a zeroed page for a VMA.
 * @vma: The VMA the page is to be allocated for.
 * @vaddr: The virtual address the page will be inserted into.
 *
 * This function will allocate a page suitable for inserting into this
 * VMA at this virtual address.  It may be allocated from highmem or
 * the movable zone.  An architecture may provide its own implementation.
 *
 * Return: A folio containing one allocated and zeroed page or NULL if
 * we are out of memory.
 */
static inline
struct folio *vma_alloc_zeroed_movable_folio(struct vm_area_struct *vma,
				   unsigned long vaddr)
{
	struct folio *folio;

	folio = vma_alloc_folio(GFP_HIGHUSER_MOVABLE, 0, vma, vaddr, false);
	if (folio)
		clear_user_highpage(&folio->page, vaddr);

	return folio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

static inline void clear_highpage(struct page *page)
{
<<<<<<< HEAD
	void *kaddr = kmap_atomic(page);
	clear_page(kaddr);
	kunmap_atomic(kaddr);
}

static inline void zero_user_segments(struct page *page,
	unsigned start1, unsigned end1,
	unsigned start2, unsigned end2)
{
	void *kaddr = kmap_atomic(page);

	BUG_ON(end1 > PAGE_SIZE || end2 > PAGE_SIZE);
=======
	void *kaddr = kmap_local_page(page);
	clear_page(kaddr);
	kunmap_local(kaddr);
}

static inline void clear_highpage_kasan_tagged(struct page *page)
{
	void *kaddr = kmap_local_page(page);

	clear_page(kasan_reset_tag(kaddr));
	kunmap_local(kaddr);
}

#ifndef __HAVE_ARCH_TAG_CLEAR_HIGHPAGE

static inline void tag_clear_highpage(struct page *page)
{
}

#endif

/*
 * If we pass in a base or tail page, we can zero up to PAGE_SIZE.
 * If we pass in a head page, we can zero up to the size of the compound page.
 */
#ifdef CONFIG_HIGHMEM
void zero_user_segments(struct page *page, unsigned start1, unsigned end1,
		unsigned start2, unsigned end2);
#else
static inline void zero_user_segments(struct page *page,
		unsigned start1, unsigned end1,
		unsigned start2, unsigned end2)
{
	void *kaddr = kmap_local_page(page);
	unsigned int i;

	BUG_ON(end1 > page_size(page) || end2 > page_size(page));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (end1 > start1)
		memset(kaddr + start1, 0, end1 - start1);

	if (end2 > start2)
		memset(kaddr + start2, 0, end2 - start2);

<<<<<<< HEAD
	kunmap_atomic(kaddr);
	flush_dcache_page(page);
}
=======
	kunmap_local(kaddr);
	for (i = 0; i < compound_nr(page); i++)
		flush_dcache_page(page + i);
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void zero_user_segment(struct page *page,
	unsigned start, unsigned end)
{
	zero_user_segments(page, start, end, 0, 0);
}

static inline void zero_user(struct page *page,
	unsigned start, unsigned size)
{
	zero_user_segments(page, start, start + size, 0, 0);
}

<<<<<<< HEAD
static inline void __deprecated memclear_highpage_flush(struct page *page,
			unsigned int offset, unsigned int size)
{
	zero_user(page, offset, size);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __HAVE_ARCH_COPY_USER_HIGHPAGE

static inline void copy_user_highpage(struct page *to, struct page *from,
	unsigned long vaddr, struct vm_area_struct *vma)
{
	char *vfrom, *vto;

<<<<<<< HEAD
	vfrom = kmap_atomic(from);
	vto = kmap_atomic(to);
	copy_user_page(vto, vfrom, vaddr, to);
	kunmap_atomic(vto);
	kunmap_atomic(vfrom);
=======
	vfrom = kmap_local_page(from);
	vto = kmap_local_page(to);
	copy_user_page(vto, vfrom, vaddr, to);
	kmsan_unpoison_memory(page_address(to), PAGE_SIZE);
	kunmap_local(vto);
	kunmap_local(vfrom);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif

<<<<<<< HEAD
=======
#ifndef __HAVE_ARCH_COPY_HIGHPAGE

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void copy_highpage(struct page *to, struct page *from)
{
	char *vfrom, *vto;

<<<<<<< HEAD
	vfrom = kmap_atomic(from);
	vto = kmap_atomic(to);
	copy_page(vto, vfrom);
	kunmap_atomic(vto);
	kunmap_atomic(vfrom);
=======
	vfrom = kmap_local_page(from);
	vto = kmap_local_page(to);
	copy_page(vto, vfrom);
	kmsan_copy_page_meta(to, from);
	kunmap_local(vto);
	kunmap_local(vfrom);
}

#endif

#ifdef copy_mc_to_kernel
/*
 * If architecture supports machine check exception handling, define the
 * #MC versions of copy_user_highpage and copy_highpage. They copy a memory
 * page with #MC in source page (@from) handled, and return the number
 * of bytes not copied if there was a #MC, otherwise 0 for success.
 */
static inline int copy_mc_user_highpage(struct page *to, struct page *from,
					unsigned long vaddr, struct vm_area_struct *vma)
{
	unsigned long ret;
	char *vfrom, *vto;

	vfrom = kmap_local_page(from);
	vto = kmap_local_page(to);
	ret = copy_mc_to_kernel(vto, vfrom, PAGE_SIZE);
	if (!ret)
		kmsan_unpoison_memory(page_address(to), PAGE_SIZE);
	kunmap_local(vto);
	kunmap_local(vfrom);

	return ret;
}

static inline int copy_mc_highpage(struct page *to, struct page *from)
{
	unsigned long ret;
	char *vfrom, *vto;

	vfrom = kmap_local_page(from);
	vto = kmap_local_page(to);
	ret = copy_mc_to_kernel(vto, vfrom, PAGE_SIZE);
	if (!ret)
		kmsan_copy_page_meta(to, from);
	kunmap_local(vto);
	kunmap_local(vfrom);

	return ret;
}
#else
static inline int copy_mc_user_highpage(struct page *to, struct page *from,
					unsigned long vaddr, struct vm_area_struct *vma)
{
	copy_user_highpage(to, from, vaddr, vma);
	return 0;
}

static inline int copy_mc_highpage(struct page *to, struct page *from)
{
	copy_highpage(to, from);
	return 0;
}
#endif

static inline void memcpy_page(struct page *dst_page, size_t dst_off,
			       struct page *src_page, size_t src_off,
			       size_t len)
{
	char *dst = kmap_local_page(dst_page);
	char *src = kmap_local_page(src_page);

	VM_BUG_ON(dst_off + len > PAGE_SIZE || src_off + len > PAGE_SIZE);
	memcpy(dst + dst_off, src + src_off, len);
	kunmap_local(src);
	kunmap_local(dst);
}

static inline void memset_page(struct page *page, size_t offset, int val,
			       size_t len)
{
	char *addr = kmap_local_page(page);

	VM_BUG_ON(offset + len > PAGE_SIZE);
	memset(addr + offset, val, len);
	kunmap_local(addr);
}

static inline void memcpy_from_page(char *to, struct page *page,
				    size_t offset, size_t len)
{
	char *from = kmap_local_page(page);

	VM_BUG_ON(offset + len > PAGE_SIZE);
	memcpy(to, from + offset, len);
	kunmap_local(from);
}

static inline void memcpy_to_page(struct page *page, size_t offset,
				  const char *from, size_t len)
{
	char *to = kmap_local_page(page);

	VM_BUG_ON(offset + len > PAGE_SIZE);
	memcpy(to + offset, from, len);
	flush_dcache_page(page);
	kunmap_local(to);
}

static inline void memzero_page(struct page *page, size_t offset, size_t len)
{
	char *addr = kmap_local_page(page);

	VM_BUG_ON(offset + len > PAGE_SIZE);
	memset(addr + offset, 0, len);
	flush_dcache_page(page);
	kunmap_local(addr);
}

/**
 * memcpy_from_folio - Copy a range of bytes from a folio.
 * @to: The memory to copy to.
 * @folio: The folio to read from.
 * @offset: The first byte in the folio to read.
 * @len: The number of bytes to copy.
 */
static inline void memcpy_from_folio(char *to, struct folio *folio,
		size_t offset, size_t len)
{
	VM_BUG_ON(offset + len > folio_size(folio));

	do {
		const char *from = kmap_local_folio(folio, offset);
		size_t chunk = len;

		if (folio_test_highmem(folio) &&
		    chunk > PAGE_SIZE - offset_in_page(offset))
			chunk = PAGE_SIZE - offset_in_page(offset);
		memcpy(to, from, chunk);
		kunmap_local(from);

		to += chunk;
		offset += chunk;
		len -= chunk;
	} while (len > 0);
}

/**
 * memcpy_to_folio - Copy a range of bytes to a folio.
 * @folio: The folio to write to.
 * @offset: The first byte in the folio to store to.
 * @from: The memory to copy from.
 * @len: The number of bytes to copy.
 */
static inline void memcpy_to_folio(struct folio *folio, size_t offset,
		const char *from, size_t len)
{
	VM_BUG_ON(offset + len > folio_size(folio));

	do {
		char *to = kmap_local_folio(folio, offset);
		size_t chunk = len;

		if (folio_test_highmem(folio) &&
		    chunk > PAGE_SIZE - offset_in_page(offset))
			chunk = PAGE_SIZE - offset_in_page(offset);
		memcpy(to, from, chunk);
		kunmap_local(to);

		from += chunk;
		offset += chunk;
		len -= chunk;
	} while (len > 0);

	flush_dcache_folio(folio);
}

/**
 * folio_zero_tail - Zero the tail of a folio.
 * @folio: The folio to zero.
 * @offset: The byte offset in the folio to start zeroing at.
 * @kaddr: The address the folio is currently mapped to.
 *
 * If you have already used kmap_local_folio() to map a folio, written
 * some data to it and now need to zero the end of the folio (and flush
 * the dcache), you can use this function.  If you do not have the
 * folio kmapped (eg the folio has been partially populated by DMA),
 * use folio_zero_range() or folio_zero_segment() instead.
 *
 * Return: An address which can be passed to kunmap_local().
 */
static inline __must_check void *folio_zero_tail(struct folio *folio,
		size_t offset, void *kaddr)
{
	size_t len = folio_size(folio) - offset;

	if (folio_test_highmem(folio)) {
		size_t max = PAGE_SIZE - offset_in_page(offset);

		while (len > max) {
			memset(kaddr, 0, max);
			kunmap_local(kaddr);
			len -= max;
			offset += max;
			max = PAGE_SIZE;
			kaddr = kmap_local_folio(folio, offset);
		}
	}

	memset(kaddr, 0, len);
	flush_dcache_folio(folio);

	return kaddr;
}

/**
 * folio_fill_tail - Copy some data to a folio and pad with zeroes.
 * @folio: The destination folio.
 * @offset: The offset into @folio at which to start copying.
 * @from: The data to copy.
 * @len: How many bytes of data to copy.
 *
 * This function is most useful for filesystems which support inline data.
 * When they want to copy data from the inode into the page cache, this
 * function does everything for them.  It supports large folios even on
 * HIGHMEM configurations.
 */
static inline void folio_fill_tail(struct folio *folio, size_t offset,
		const char *from, size_t len)
{
	char *to = kmap_local_folio(folio, offset);

	VM_BUG_ON(offset + len > folio_size(folio));

	if (folio_test_highmem(folio)) {
		size_t max = PAGE_SIZE - offset_in_page(offset);

		while (len > max) {
			memcpy(to, from, max);
			kunmap_local(to);
			len -= max;
			from += max;
			offset += max;
			max = PAGE_SIZE;
			to = kmap_local_folio(folio, offset);
		}
	}

	memcpy(to, from, len);
	to = folio_zero_tail(folio, offset + len, to + len);
	kunmap_local(to);
}

/**
 * memcpy_from_file_folio - Copy some bytes from a file folio.
 * @to: The destination buffer.
 * @folio: The folio to copy from.
 * @pos: The position in the file.
 * @len: The maximum number of bytes to copy.
 *
 * Copy up to @len bytes from this folio.  This may be limited by PAGE_SIZE
 * if the folio comes from HIGHMEM, and by the size of the folio.
 *
 * Return: The number of bytes copied from the folio.
 */
static inline size_t memcpy_from_file_folio(char *to, struct folio *folio,
		loff_t pos, size_t len)
{
	size_t offset = offset_in_folio(folio, pos);
	char *from = kmap_local_folio(folio, offset);

	if (folio_test_highmem(folio)) {
		offset = offset_in_page(offset);
		len = min_t(size_t, len, PAGE_SIZE - offset);
	} else
		len = min(len, folio_size(folio) - offset);

	memcpy(to, from, len);
	kunmap_local(from);

	return len;
}

/**
 * folio_zero_segments() - Zero two byte ranges in a folio.
 * @folio: The folio to write to.
 * @start1: The first byte to zero.
 * @xend1: One more than the last byte in the first range.
 * @start2: The first byte to zero in the second range.
 * @xend2: One more than the last byte in the second range.
 */
static inline void folio_zero_segments(struct folio *folio,
		size_t start1, size_t xend1, size_t start2, size_t xend2)
{
	zero_user_segments(&folio->page, start1, xend1, start2, xend2);
}

/**
 * folio_zero_segment() - Zero a byte range in a folio.
 * @folio: The folio to write to.
 * @start: The first byte to zero.
 * @xend: One more than the last byte to zero.
 */
static inline void folio_zero_segment(struct folio *folio,
		size_t start, size_t xend)
{
	zero_user_segments(&folio->page, start, xend, 0, 0);
}

/**
 * folio_zero_range() - Zero a byte range in a folio.
 * @folio: The folio to write to.
 * @start: The first byte to zero.
 * @length: The number of bytes to zero.
 */
static inline void folio_zero_range(struct folio *folio,
		size_t start, size_t length)
{
	zero_user_segments(&folio->page, start, start + length, 0, 0);
}

/**
 * folio_release_kmap - Unmap a folio and drop a refcount.
 * @folio: The folio to release.
 * @addr: The address previously returned by a call to kmap_local_folio().
 *
 * It is common, eg in directory handling to kmap a folio.  This function
 * unmaps the folio and drops the refcount that was being held to keep the
 * folio alive while we accessed it.
 */
static inline void folio_release_kmap(struct folio *folio, void *addr)
{
	kunmap_local(addr);
	folio_put(folio);
}

static inline void unmap_and_put_page(struct page *page, void *addr)
{
	folio_release_kmap(page_folio(page), addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif /* _LINUX_HIGHMEM_H */
