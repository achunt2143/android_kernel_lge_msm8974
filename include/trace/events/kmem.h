<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#undef TRACE_SYSTEM
#define TRACE_SYSTEM kmem

#if !defined(_TRACE_KMEM_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_KMEM_H

#include <linux/types.h>
#include <linux/tracepoint.h>
<<<<<<< HEAD
#include "gfpflags.h"

DECLARE_EVENT_CLASS(kmem_alloc,

	TP_PROTO(unsigned long call_site,
		 const void *ptr,
		 size_t bytes_req,
		 size_t bytes_alloc,
		 gfp_t gfp_flags),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags),
=======
#include <trace/events/mmflags.h>

TRACE_EVENT(kmem_cache_alloc,

	TP_PROTO(unsigned long call_site,
		 const void *ptr,
		 struct kmem_cache *s,
		 gfp_t gfp_flags,
		 int node),

	TP_ARGS(call_site, ptr, s, gfp_flags, node),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_STRUCT__entry(
		__field(	unsigned long,	call_site	)
		__field(	const void *,	ptr		)
		__field(	size_t,		bytes_req	)
		__field(	size_t,		bytes_alloc	)
<<<<<<< HEAD
		__field(	gfp_t,		gfp_flags	)
=======
		__field(	unsigned long,	gfp_flags	)
		__field(	int,		node		)
		__field(	bool,		accounted	)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->call_site	= call_site;
		__entry->ptr		= ptr;
<<<<<<< HEAD
		__entry->bytes_req	= bytes_req;
		__entry->bytes_alloc	= bytes_alloc;
		__entry->gfp_flags	= gfp_flags;
	),

	TP_printk("call_site=%lx ptr=%p bytes_req=%zu bytes_alloc=%zu gfp_flags=%s",
		__entry->call_site,
		__entry->ptr,
		__entry->bytes_req,
		__entry->bytes_alloc,
		show_gfp_flags(__entry->gfp_flags))
);

DEFINE_EVENT(kmem_alloc, kmalloc,

	TP_PROTO(unsigned long call_site, const void *ptr,
		 size_t bytes_req, size_t bytes_alloc, gfp_t gfp_flags),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags)
);

DEFINE_EVENT(kmem_alloc, kmem_cache_alloc,

	TP_PROTO(unsigned long call_site, const void *ptr,
		 size_t bytes_req, size_t bytes_alloc, gfp_t gfp_flags),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags)
);

DECLARE_EVENT_CLASS(kmem_alloc_node,
=======
		__entry->bytes_req	= s->object_size;
		__entry->bytes_alloc	= s->size;
		__entry->gfp_flags	= (__force unsigned long)gfp_flags;
		__entry->node		= node;
		__entry->accounted	= IS_ENABLED(CONFIG_MEMCG_KMEM) ?
					  ((gfp_flags & __GFP_ACCOUNT) ||
					  (s->flags & SLAB_ACCOUNT)) : false;
	),

	TP_printk("call_site=%pS ptr=%p bytes_req=%zu bytes_alloc=%zu gfp_flags=%s node=%d accounted=%s",
		(void *)__entry->call_site,
		__entry->ptr,
		__entry->bytes_req,
		__entry->bytes_alloc,
		show_gfp_flags(__entry->gfp_flags),
		__entry->node,
		__entry->accounted ? "true" : "false")
);

TRACE_EVENT(kmalloc,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_PROTO(unsigned long call_site,
		 const void *ptr,
		 size_t bytes_req,
		 size_t bytes_alloc,
		 gfp_t gfp_flags,
		 int node),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags, node),

	TP_STRUCT__entry(
		__field(	unsigned long,	call_site	)
		__field(	const void *,	ptr		)
		__field(	size_t,		bytes_req	)
		__field(	size_t,		bytes_alloc	)
<<<<<<< HEAD
		__field(	gfp_t,		gfp_flags	)
=======
		__field(	unsigned long,	gfp_flags	)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(	int,		node		)
	),

	TP_fast_assign(
		__entry->call_site	= call_site;
		__entry->ptr		= ptr;
		__entry->bytes_req	= bytes_req;
		__entry->bytes_alloc	= bytes_alloc;
<<<<<<< HEAD
		__entry->gfp_flags	= gfp_flags;
		__entry->node		= node;
	),

	TP_printk("call_site=%lx ptr=%p bytes_req=%zu bytes_alloc=%zu gfp_flags=%s node=%d",
		__entry->call_site,
=======
		__entry->gfp_flags	= (__force unsigned long)gfp_flags;
		__entry->node		= node;
	),

	TP_printk("call_site=%pS ptr=%p bytes_req=%zu bytes_alloc=%zu gfp_flags=%s node=%d accounted=%s",
		(void *)__entry->call_site,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->ptr,
		__entry->bytes_req,
		__entry->bytes_alloc,
		show_gfp_flags(__entry->gfp_flags),
<<<<<<< HEAD
		__entry->node)
);

DEFINE_EVENT(kmem_alloc_node, kmalloc_node,

	TP_PROTO(unsigned long call_site, const void *ptr,
		 size_t bytes_req, size_t bytes_alloc,
		 gfp_t gfp_flags, int node),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags, node)
);

DEFINE_EVENT(kmem_alloc_node, kmem_cache_alloc_node,

	TP_PROTO(unsigned long call_site, const void *ptr,
		 size_t bytes_req, size_t bytes_alloc,
		 gfp_t gfp_flags, int node),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags, node)
);

DECLARE_EVENT_CLASS(kmem_free,
=======
		__entry->node,
		(IS_ENABLED(CONFIG_MEMCG_KMEM) &&
		 (__entry->gfp_flags & (__force unsigned long)__GFP_ACCOUNT)) ? "true" : "false")
);

TRACE_EVENT(kfree,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_PROTO(unsigned long call_site, const void *ptr),

	TP_ARGS(call_site, ptr),

	TP_STRUCT__entry(
		__field(	unsigned long,	call_site	)
		__field(	const void *,	ptr		)
	),

	TP_fast_assign(
		__entry->call_site	= call_site;
		__entry->ptr		= ptr;
	),

<<<<<<< HEAD
	TP_printk("call_site=%lx ptr=%p", __entry->call_site, __entry->ptr)
);

DEFINE_EVENT(kmem_free, kfree,

	TP_PROTO(unsigned long call_site, const void *ptr),

	TP_ARGS(call_site, ptr)
);

DEFINE_EVENT(kmem_free, kmem_cache_free,

	TP_PROTO(unsigned long call_site, const void *ptr),

	TP_ARGS(call_site, ptr)
=======
	TP_printk("call_site=%pS ptr=%p",
		  (void *)__entry->call_site, __entry->ptr)
);

TRACE_EVENT(kmem_cache_free,

	TP_PROTO(unsigned long call_site, const void *ptr, const struct kmem_cache *s),

	TP_ARGS(call_site, ptr, s),

	TP_STRUCT__entry(
		__field(	unsigned long,	call_site	)
		__field(	const void *,	ptr		)
		__string(	name,		s->name		)
	),

	TP_fast_assign(
		__entry->call_site	= call_site;
		__entry->ptr		= ptr;
		__assign_str(name, s->name);
	),

	TP_printk("call_site=%pS ptr=%p name=%s",
		  (void *)__entry->call_site, __entry->ptr, __get_str(name))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

TRACE_EVENT(mm_page_free,

	TP_PROTO(struct page *page, unsigned int order),

	TP_ARGS(page, order),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(	struct page *,	page		)
=======
		__field(	unsigned long,	pfn		)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(	unsigned int,	order		)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->page		= page;
		__entry->order		= order;
	),

	TP_printk("page=%p pfn=%lu order=%d",
			__entry->page,
			page_to_pfn(__entry->page),
=======
		__entry->pfn		= page_to_pfn(page);
		__entry->order		= order;
	),

	TP_printk("page=%p pfn=0x%lx order=%d",
			pfn_to_page(__entry->pfn),
			__entry->pfn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__entry->order)
);

TRACE_EVENT(mm_page_free_batched,

<<<<<<< HEAD
	TP_PROTO(struct page *page, int cold),

	TP_ARGS(page, cold),

	TP_STRUCT__entry(
		__field(	struct page *,	page		)
		__field(	int,		cold		)
	),

	TP_fast_assign(
		__entry->page		= page;
		__entry->cold		= cold;
	),

	TP_printk("page=%p pfn=%lu order=0 cold=%d",
			__entry->page,
			page_to_pfn(__entry->page),
			__entry->cold)
=======
	TP_PROTO(struct page *page),

	TP_ARGS(page),

	TP_STRUCT__entry(
		__field(	unsigned long,	pfn		)
	),

	TP_fast_assign(
		__entry->pfn		= page_to_pfn(page);
	),

	TP_printk("page=%p pfn=0x%lx order=0",
			pfn_to_page(__entry->pfn),
			__entry->pfn)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

TRACE_EVENT(mm_page_alloc,

	TP_PROTO(struct page *page, unsigned int order,
			gfp_t gfp_flags, int migratetype),

	TP_ARGS(page, order, gfp_flags, migratetype),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(	struct page *,	page		)
		__field(	unsigned int,	order		)
		__field(	gfp_t,		gfp_flags	)
=======
		__field(	unsigned long,	pfn		)
		__field(	unsigned int,	order		)
		__field(	unsigned long,	gfp_flags	)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(	int,		migratetype	)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->page		= page;
		__entry->order		= order;
		__entry->gfp_flags	= gfp_flags;
		__entry->migratetype	= migratetype;
	),

	TP_printk("page=%p pfn=%lu order=%d migratetype=%d gfp_flags=%s",
		__entry->page,
		__entry->page ? page_to_pfn(__entry->page) : 0,
=======
		__entry->pfn		= page ? page_to_pfn(page) : -1UL;
		__entry->order		= order;
		__entry->gfp_flags	= (__force unsigned long)gfp_flags;
		__entry->migratetype	= migratetype;
	),

	TP_printk("page=%p pfn=0x%lx order=%d migratetype=%d gfp_flags=%s",
		__entry->pfn != -1UL ? pfn_to_page(__entry->pfn) : NULL,
		__entry->pfn != -1UL ? __entry->pfn : 0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->order,
		__entry->migratetype,
		show_gfp_flags(__entry->gfp_flags))
);

DECLARE_EVENT_CLASS(mm_page,

<<<<<<< HEAD
=======
	TP_PROTO(struct page *page, unsigned int order, int migratetype,
		 int percpu_refill),

	TP_ARGS(page, order, migratetype, percpu_refill),

	TP_STRUCT__entry(
		__field(	unsigned long,	pfn		)
		__field(	unsigned int,	order		)
		__field(	int,		migratetype	)
		__field(	int,		percpu_refill	)
	),

	TP_fast_assign(
		__entry->pfn		= page ? page_to_pfn(page) : -1UL;
		__entry->order		= order;
		__entry->migratetype	= migratetype;
		__entry->percpu_refill	= percpu_refill;
	),

	TP_printk("page=%p pfn=0x%lx order=%u migratetype=%d percpu_refill=%d",
		__entry->pfn != -1UL ? pfn_to_page(__entry->pfn) : NULL,
		__entry->pfn != -1UL ? __entry->pfn : 0,
		__entry->order,
		__entry->migratetype,
		__entry->percpu_refill)
);

DEFINE_EVENT(mm_page, mm_page_alloc_zone_locked,

	TP_PROTO(struct page *page, unsigned int order, int migratetype,
		 int percpu_refill),

	TP_ARGS(page, order, migratetype, percpu_refill)
);

TRACE_EVENT(mm_page_pcpu_drain,

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	TP_PROTO(struct page *page, unsigned int order, int migratetype),

	TP_ARGS(page, order, migratetype),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(	struct page *,	page		)
=======
		__field(	unsigned long,	pfn		)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(	unsigned int,	order		)
		__field(	int,		migratetype	)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->page		= page;
=======
		__entry->pfn		= page ? page_to_pfn(page) : -1UL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->order		= order;
		__entry->migratetype	= migratetype;
	),

<<<<<<< HEAD
	TP_printk("page=%p pfn=%lu order=%u migratetype=%d percpu_refill=%d",
		__entry->page,
		__entry->page ? page_to_pfn(__entry->page) : 0,
		__entry->order,
		__entry->migratetype,
		__entry->order == 0)
);

DEFINE_EVENT(mm_page, mm_page_alloc_zone_locked,

	TP_PROTO(struct page *page, unsigned int order, int migratetype),

	TP_ARGS(page, order, migratetype)
);

DEFINE_EVENT_PRINT(mm_page, mm_page_pcpu_drain,

	TP_PROTO(struct page *page, unsigned int order, int migratetype),

	TP_ARGS(page, order, migratetype),

	TP_printk("page=%p pfn=%lu order=%d migratetype=%d",
		__entry->page, page_to_pfn(__entry->page),
=======
	TP_printk("page=%p pfn=0x%lx order=%d migratetype=%d",
		pfn_to_page(__entry->pfn), __entry->pfn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->order, __entry->migratetype)
);

TRACE_EVENT(mm_page_alloc_extfrag,

	TP_PROTO(struct page *page,
<<<<<<< HEAD
			int alloc_order, int fallback_order,
			int alloc_migratetype, int fallback_migratetype),
=======
		int alloc_order, int fallback_order,
		int alloc_migratetype, int fallback_migratetype),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_ARGS(page,
		alloc_order, fallback_order,
		alloc_migratetype, fallback_migratetype),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(	struct page *,	page			)
=======
		__field(	unsigned long,	pfn			)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(	int,		alloc_order		)
		__field(	int,		fallback_order		)
		__field(	int,		alloc_migratetype	)
		__field(	int,		fallback_migratetype	)
<<<<<<< HEAD
	),

	TP_fast_assign(
		__entry->page			= page;
=======
		__field(	int,		change_ownership	)
	),

	TP_fast_assign(
		__entry->pfn			= page_to_pfn(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->alloc_order		= alloc_order;
		__entry->fallback_order		= fallback_order;
		__entry->alloc_migratetype	= alloc_migratetype;
		__entry->fallback_migratetype	= fallback_migratetype;
<<<<<<< HEAD
	),

	TP_printk("page=%p pfn=%lu alloc_order=%d fallback_order=%d pageblock_order=%d alloc_migratetype=%d fallback_migratetype=%d fragmenting=%d change_ownership=%d",
		__entry->page,
		page_to_pfn(__entry->page),
=======
		__entry->change_ownership	= (alloc_migratetype ==
					get_pageblock_migratetype(page));
	),

	TP_printk("page=%p pfn=0x%lx alloc_order=%d fallback_order=%d pageblock_order=%d alloc_migratetype=%d fallback_migratetype=%d fragmenting=%d change_ownership=%d",
		pfn_to_page(__entry->pfn),
		__entry->pfn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->alloc_order,
		__entry->fallback_order,
		pageblock_order,
		__entry->alloc_migratetype,
		__entry->fallback_migratetype,
		__entry->fallback_order < pageblock_order,
<<<<<<< HEAD
		__entry->alloc_migratetype == __entry->fallback_migratetype)
);


DECLARE_EVENT_CLASS(ion_alloc,

	TP_PROTO(const char *client_name,
		 const char *heap_name,
		 size_t len,
		 unsigned int mask,
		 unsigned int flags),

	TP_ARGS(client_name, heap_name, len, mask, flags),

	TP_STRUCT__entry(
		__array(char,		client_name, 64)
		__field(const char *,	heap_name)
		__field(size_t,		len)
		__field(unsigned int,	mask)
		__field(unsigned int,	flags)
	),

	TP_fast_assign(
		strlcpy(__entry->client_name, client_name, 64);
		__entry->heap_name	= heap_name;
		__entry->len		= len;
		__entry->mask		= mask;
		__entry->flags		= flags;
	),

	TP_printk("client_name=%s heap_name=%s len=%zu mask=0x%x flags=0x%x",
		__entry->client_name,
		__entry->heap_name,
		__entry->len,
		__entry->mask,
		__entry->flags)
);

DEFINE_EVENT(ion_alloc, ion_alloc_buffer_start,

	TP_PROTO(const char *client_name,
		 const char *heap_name,
		 size_t len,
		 unsigned int mask,
		 unsigned int flags),

	TP_ARGS(client_name, heap_name, len, mask, flags)
);

DEFINE_EVENT(ion_alloc, ion_alloc_buffer_end,

	TP_PROTO(const char *client_name,
		 const char *heap_name,
		 size_t len,
		 unsigned int mask,
		 unsigned int flags),

	TP_ARGS(client_name, heap_name, len, mask, flags)
);

DECLARE_EVENT_CLASS(ion_alloc_error,

	TP_PROTO(const char *client_name,
		 const char *heap_name,
		 size_t len,
		 unsigned int mask,
		 unsigned int flags,
		 long error),

	TP_ARGS(client_name, heap_name, len, mask, flags, error),

	TP_STRUCT__entry(
		__field(const char *,	client_name)
		__field(const char *,	heap_name)
		__field(size_t,		len)
		__field(unsigned int,	mask)
		__field(unsigned int,	flags)
		__field(long,		error)
	),

	TP_fast_assign(
		__entry->client_name	= client_name;
		__entry->heap_name	= heap_name;
		__entry->len		= len;
		__entry->mask		= mask;
		__entry->flags		= flags;
		__entry->error		= error;
	),

	TP_printk(
	"client_name=%s heap_name=%s len=%zu mask=0x%x flags=0x%x error=%ld",
		__entry->client_name,
		__entry->heap_name,
		__entry->len,
		__entry->mask,
		__entry->flags,
		__entry->error)
);


DEFINE_EVENT(ion_alloc_error, ion_alloc_buffer_fallback,

	TP_PROTO(const char *client_name,
		 const char *heap_name,
		 size_t len,
		 unsigned int mask,
		 unsigned int flags,
		 long error),

	TP_ARGS(client_name, heap_name, len, mask, flags, error)
);

DEFINE_EVENT(ion_alloc_error, ion_alloc_buffer_fail,

	TP_PROTO(const char *client_name,
		 const char *heap_name,
		 size_t len,
		 unsigned int mask,
		 unsigned int flags,
		 long error),

	TP_ARGS(client_name, heap_name, len, mask, flags, error)
);


DECLARE_EVENT_CLASS(alloc_retry,

	TP_PROTO(int tries),

	TP_ARGS(tries),

	TP_STRUCT__entry(
		__field(int, tries)
	),

	TP_fast_assign(
		__entry->tries = tries;
	),

	TP_printk("tries=%d",
		__entry->tries)
);

DEFINE_EVENT(alloc_retry, ion_cp_alloc_retry,

	TP_PROTO(int tries),

	TP_ARGS(tries)
);

DEFINE_EVENT(alloc_retry, migrate_retry,

	TP_PROTO(int tries),

	TP_ARGS(tries)
);

DEFINE_EVENT(alloc_retry, dma_alloc_contiguous_retry,

	TP_PROTO(int tries),

	TP_ARGS(tries)
);

DECLARE_EVENT_CLASS(migrate_pages,

	TP_PROTO(int mode),

	TP_ARGS(mode),

	TP_STRUCT__entry(
		__field(int, mode)
	),

	TP_fast_assign(
		__entry->mode = mode;
	),

	TP_printk("mode=%d",
		__entry->mode)
);

DEFINE_EVENT(migrate_pages, migrate_pages_start,

	TP_PROTO(int mode),

	TP_ARGS(mode)
);

DEFINE_EVENT(migrate_pages, migrate_pages_end,

	TP_PROTO(int mode),

	TP_ARGS(mode)
);

DECLARE_EVENT_CLASS(ion_alloc_pages,

	TP_PROTO(gfp_t gfp_flags,
		unsigned int order),

	TP_ARGS(gfp_flags, order),

	TP_STRUCT__entry(
		__field(gfp_t, gfp_flags)
		__field(unsigned int, order)
		),

	TP_fast_assign(
		__entry->gfp_flags = gfp_flags;
		__entry->order = order;
		),

	TP_printk("gfp_flags=%s order=%d",
		show_gfp_flags(__entry->gfp_flags),
		__entry->order)
	);

DEFINE_EVENT(ion_alloc_pages, alloc_pages_iommu_start,
	TP_PROTO(gfp_t gfp_flags,
		unsigned int order),

	TP_ARGS(gfp_flags, order)
	);

DEFINE_EVENT(ion_alloc_pages, alloc_pages_iommu_end,
	TP_PROTO(gfp_t gfp_flags,
		unsigned int order),

	TP_ARGS(gfp_flags, order)
	);

DEFINE_EVENT(ion_alloc_pages, alloc_pages_iommu_fail,
	TP_PROTO(gfp_t gfp_flags,
		unsigned int order),

	TP_ARGS(gfp_flags, order)
	);

DEFINE_EVENT(ion_alloc_pages, alloc_pages_sys_start,
	TP_PROTO(gfp_t gfp_flags,
		unsigned int order),

	TP_ARGS(gfp_flags, order)
	);

DEFINE_EVENT(ion_alloc_pages, alloc_pages_sys_end,
	TP_PROTO(gfp_t gfp_flags,
		unsigned int order),

	TP_ARGS(gfp_flags, order)
	);

DEFINE_EVENT(ion_alloc_pages, alloc_pages_sys_fail,
	TP_PROTO(gfp_t gfp_flags,
		unsigned int order),

	TP_ARGS(gfp_flags, order)

	);

DECLARE_EVENT_CLASS(smmu_map,

	TP_PROTO(unsigned int va,
		phys_addr_t pa,
		unsigned int chunk_size,
		size_t len),

	TP_ARGS(va, pa, chunk_size, len),

	TP_STRUCT__entry(
		__field(unsigned int, va)
		__field(phys_addr_t, pa)
		__field(unsigned int, chunk_size)
		__field(size_t, len)
		),

	TP_fast_assign(
		__entry->va = va;
		__entry->pa = pa;
		__entry->chunk_size = chunk_size;
		__entry->len = len;
		),

	TP_printk("v_addr=%p p_addr=%pa chunk_size=0x%x len=%zu",
		(void *)__entry->va,
		&__entry->pa,
		__entry->chunk_size,
		__entry->len)
	);

DEFINE_EVENT(smmu_map, iommu_map_range,
	TP_PROTO(unsigned int va,
		phys_addr_t pa,
		unsigned int chunk_size,
		size_t len),

	TP_ARGS(va, pa, chunk_size, len)
	);

=======
		__entry->change_ownership)
);

TRACE_EVENT(mm_alloc_contig_migrate_range_info,

	TP_PROTO(unsigned long start,
		 unsigned long end,
		 unsigned long nr_migrated,
		 unsigned long nr_reclaimed,
		 unsigned long nr_mapped,
		 int migratetype),

	TP_ARGS(start, end, nr_migrated, nr_reclaimed, nr_mapped, migratetype),

	TP_STRUCT__entry(
		__field(unsigned long, start)
		__field(unsigned long, end)
		__field(unsigned long, nr_migrated)
		__field(unsigned long, nr_reclaimed)
		__field(unsigned long, nr_mapped)
		__field(int, migratetype)
	),

	TP_fast_assign(
		__entry->start = start;
		__entry->end = end;
		__entry->nr_migrated = nr_migrated;
		__entry->nr_reclaimed = nr_reclaimed;
		__entry->nr_mapped = nr_mapped;
		__entry->migratetype = migratetype;
	),

	TP_printk("start=0x%lx end=0x%lx migratetype=%d nr_migrated=%lu nr_reclaimed=%lu nr_mapped=%lu",
		  __entry->start,
		  __entry->end,
		  __entry->migratetype,
		  __entry->nr_migrated,
		  __entry->nr_reclaimed,
		  __entry->nr_mapped)
);

/*
 * Required for uniquely and securely identifying mm in rss_stat tracepoint.
 */
#ifndef __PTR_TO_HASHVAL
static unsigned int __maybe_unused mm_ptr_to_hash(const void *ptr)
{
	int ret;
	unsigned long hashval;

	ret = ptr_to_hashval(ptr, &hashval);
	if (ret)
		return 0;

	/* The hashed value is only 32-bit */
	return (unsigned int)hashval;
}
#define __PTR_TO_HASHVAL
#endif

#define TRACE_MM_PAGES		\
	EM(MM_FILEPAGES)	\
	EM(MM_ANONPAGES)	\
	EM(MM_SWAPENTS)		\
	EMe(MM_SHMEMPAGES)

#undef EM
#undef EMe

#define EM(a)	TRACE_DEFINE_ENUM(a);
#define EMe(a)	TRACE_DEFINE_ENUM(a);

TRACE_MM_PAGES

#undef EM
#undef EMe

#define EM(a)	{ a, #a },
#define EMe(a)	{ a, #a }

TRACE_EVENT(rss_stat,

	TP_PROTO(struct mm_struct *mm,
		int member),

	TP_ARGS(mm, member),

	TP_STRUCT__entry(
		__field(unsigned int, mm_id)
		__field(unsigned int, curr)
		__field(int, member)
		__field(long, size)
	),

	TP_fast_assign(
		__entry->mm_id = mm_ptr_to_hash(mm);
		__entry->curr = !!(current->mm == mm);
		__entry->member = member;
		__entry->size = (percpu_counter_sum_positive(&mm->rss_stat[member])
							    << PAGE_SHIFT);
	),

	TP_printk("mm_id=%u curr=%d type=%s size=%ldB",
		__entry->mm_id,
		__entry->curr,
		__print_symbolic(__entry->member, TRACE_MM_PAGES),
		__entry->size)
	);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _TRACE_KMEM_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
