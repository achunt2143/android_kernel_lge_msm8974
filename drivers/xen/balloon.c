/******************************************************************************
 * Xen balloon driver - enables returning/claiming memory to/from Xen.
 *
 * Copyright (c) 2003, B Dragovic
 * Copyright (c) 2003-2004, M Williamson, K Fraser
 * Copyright (c) 2005 Dan M. Smith, IBM Corporation
 * Copyright (c) 2010 Daniel Kiper
 *
 * Memory hotplug support was written by Daniel Kiper. Work on
 * it was sponsored by Google under Google Summer of Code 2010
 * program. Jeremy Fitzhardinge from Citrix was the mentor for
 * this project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
=======
#define pr_fmt(fmt) "xen:" KBUILD_MODNAME ": " fmt

#include <linux/cpu.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/errno.h>
#include <linux/freezer.h>
#include <linux/kthread.h>
#include <linux/mm.h>
#include <linux/memblock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pagemap.h>
#include <linux/highmem.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/gfp.h>
#include <linux/notifier.h>
#include <linux/memory.h>
#include <linux/memory_hotplug.h>
<<<<<<< HEAD

#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm/tlb.h>
#include <asm/e820.h>
=======
#include <linux/percpu-defs.h>
#include <linux/slab.h>
#include <linux/sysctl.h>
#include <linux/moduleparam.h>
#include <linux/jiffies.h>

#include <asm/page.h>
#include <asm/tlb.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/xen/hypervisor.h>
#include <asm/xen/hypercall.h>

#include <xen/xen.h>
#include <xen/interface/xen.h>
#include <xen/interface/memory.h>
#include <xen/balloon.h>
#include <xen/features.h>
#include <xen/page.h>
<<<<<<< HEAD

/*
 * balloon_process() state:
 *
 * BP_DONE: done or nothing to do,
=======
#include <xen/mem-reservation.h>

#undef MODULE_PARAM_PREFIX
#define MODULE_PARAM_PREFIX "xen."

static uint __read_mostly balloon_boot_timeout = 180;
module_param(balloon_boot_timeout, uint, 0444);

#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
static int xen_hotplug_unpopulated;

static struct ctl_table balloon_table[] = {
	{
		.procname	= "hotplug_unpopulated",
		.data		= &xen_hotplug_unpopulated,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

#else
#define xen_hotplug_unpopulated 0
#endif

/*
 * Use one extent per PAGE_SIZE to avoid to break down the page into
 * multiple frame.
 */
#define EXTENT_ORDER (fls(XEN_PFN_PER_PAGE) - 1)

/*
 * balloon_thread() state:
 *
 * BP_DONE: done or nothing to do,
 * BP_WAIT: wait to be rescheduled,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * BP_EAGAIN: error, go to sleep,
 * BP_ECANCELED: error, balloon operation canceled.
 */

<<<<<<< HEAD
enum bp_state {
	BP_DONE,
	BP_EAGAIN,
	BP_ECANCELED
};

=======
static enum bp_state {
	BP_DONE,
	BP_WAIT,
	BP_EAGAIN,
	BP_ECANCELED
} balloon_state = BP_DONE;

/* Main waiting point for xen-balloon thread. */
static DECLARE_WAIT_QUEUE_HEAD(balloon_thread_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_MUTEX(balloon_mutex);

struct balloon_stats balloon_stats;
EXPORT_SYMBOL_GPL(balloon_stats);

/* We increase/decrease in batches which fit in a page */
<<<<<<< HEAD
static unsigned long frame_list[PAGE_SIZE / sizeof(unsigned long)];

#ifdef CONFIG_HIGHMEM
#define inc_totalhigh_pages() (totalhigh_pages++)
#define dec_totalhigh_pages() (totalhigh_pages--)
#else
#define inc_totalhigh_pages() do {} while (0)
#define dec_totalhigh_pages() do {} while (0)
#endif

/* List of ballooned pages, threaded through the mem_map array. */
static LIST_HEAD(ballooned_pages);

/* Main work function, always executed in process context. */
static void balloon_process(struct work_struct *work);
static DECLARE_DELAYED_WORK(balloon_worker, balloon_process);
=======
static xen_pfn_t frame_list[PAGE_SIZE / sizeof(xen_pfn_t)];


/* List of ballooned pages, threaded through the mem_map array. */
static LIST_HEAD(ballooned_pages);
static DECLARE_WAIT_QUEUE_HEAD(balloon_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* When ballooning out (allocating memory to return to Xen) we don't really
   want the kernel to try too hard since that can trigger the oom killer. */
#define GFP_BALLOON \
	(GFP_HIGHUSER | __GFP_NOWARN | __GFP_NORETRY | __GFP_NOMEMALLOC)

<<<<<<< HEAD
static void scrub_page(struct page *page)
{
#ifdef CONFIG_XEN_SCRUB_PAGES
	clear_highpage(page);
#endif
}

/* balloon_append: add the given page to the balloon. */
static void __balloon_append(struct page *page)
{
=======
/* balloon_append: add the given page to the balloon. */
static void balloon_append(struct page *page)
{
	__SetPageOffline(page);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Lowmem is re-populated first, so highmem pages go at list tail. */
	if (PageHighMem(page)) {
		list_add_tail(&page->lru, &ballooned_pages);
		balloon_stats.balloon_high++;
	} else {
		list_add(&page->lru, &ballooned_pages);
		balloon_stats.balloon_low++;
	}
<<<<<<< HEAD
}

static void balloon_append(struct page *page)
{
	__balloon_append(page);
	if (PageHighMem(page))
		dec_totalhigh_pages();
	totalram_pages--;
}

/* balloon_retrieve: rescue a page from the balloon, if it is not empty. */
static struct page *balloon_retrieve(bool prefer_highmem)
=======
	wake_up(&balloon_wq);
}

/* balloon_retrieve: rescue a page from the balloon, if it is not empty. */
static struct page *balloon_retrieve(bool require_lowmem)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct page *page;

	if (list_empty(&ballooned_pages))
		return NULL;

<<<<<<< HEAD
	if (prefer_highmem)
		page = list_entry(ballooned_pages.prev, struct page, lru);
	else
		page = list_entry(ballooned_pages.next, struct page, lru);
	list_del(&page->lru);

	if (PageHighMem(page)) {
		balloon_stats.balloon_high--;
		inc_totalhigh_pages();
	} else
		balloon_stats.balloon_low--;

	totalram_pages++;

	return page;
}

static struct page *balloon_first_page(void)
{
	if (list_empty(&ballooned_pages))
		return NULL;
	return list_entry(ballooned_pages.next, struct page, lru);
}

=======
	page = list_entry(ballooned_pages.next, struct page, lru);
	if (require_lowmem && PageHighMem(page))
		return NULL;
	list_del(&page->lru);

	if (PageHighMem(page))
		balloon_stats.balloon_high--;
	else
		balloon_stats.balloon_low--;

	__ClearPageOffline(page);
	return page;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct page *balloon_next_page(struct page *page)
{
	struct list_head *next = page->lru.next;
	if (next == &ballooned_pages)
		return NULL;
	return list_entry(next, struct page, lru);
}

<<<<<<< HEAD
static enum bp_state update_schedule(enum bp_state state)
{
	if (state == BP_DONE) {
		balloon_stats.schedule_delay = 1;
		balloon_stats.retry_count = 1;
		return BP_DONE;
=======
static void update_schedule(void)
{
	if (balloon_state == BP_WAIT || balloon_state == BP_ECANCELED)
		return;

	if (balloon_state == BP_DONE) {
		balloon_stats.schedule_delay = 1;
		balloon_stats.retry_count = 1;
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	++balloon_stats.retry_count;

	if (balloon_stats.max_retry_count != RETRY_UNLIMITED &&
			balloon_stats.retry_count > balloon_stats.max_retry_count) {
		balloon_stats.schedule_delay = 1;
		balloon_stats.retry_count = 1;
<<<<<<< HEAD
		return BP_ECANCELED;
=======
		balloon_state = BP_ECANCELED;
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	balloon_stats.schedule_delay <<= 1;

	if (balloon_stats.schedule_delay > balloon_stats.max_schedule_delay)
		balloon_stats.schedule_delay = balloon_stats.max_schedule_delay;

<<<<<<< HEAD
	return BP_EAGAIN;
}

#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
static long current_credit(void)
{
	return balloon_stats.target_pages - balloon_stats.current_pages -
		balloon_stats.hotplug_pages;
}

static bool balloon_is_inflated(void)
{
	if (balloon_stats.balloon_low || balloon_stats.balloon_high ||
			balloon_stats.balloon_hotplug)
		return true;
	else
		return false;
}

/*
 * reserve_additional_memory() adds memory region of size >= credit above
 * max_pfn. New region is section aligned and size is modified to be multiple
 * of section size. Those features allow optimal use of address space and
 * establish proper alignment when this function is called first time after
 * boot (last section not fully populated at boot time contains unused memory
 * pages with PG_reserved bit not set; online_pages_range() does not allow page
 * onlining in whole range if first onlined page does not have PG_reserved
 * bit set). Real size of added memory is established at page onlining stage.
 */

static enum bp_state reserve_additional_memory(long credit)
{
	int nid, rc;
	u64 hotplug_start_paddr;
	unsigned long balloon_hotplug = credit;

	hotplug_start_paddr = PFN_PHYS(SECTION_ALIGN_UP(max_pfn));
	balloon_hotplug = round_up(balloon_hotplug, PAGES_PER_SECTION);
	nid = memory_add_physaddr_to_nid(hotplug_start_paddr);

	rc = add_memory(nid, hotplug_start_paddr, balloon_hotplug << PAGE_SHIFT);

	if (rc) {
		pr_info("xen_balloon: %s: add_memory() failed: %i\n", __func__, rc);
		return BP_EAGAIN;
	}

	balloon_hotplug -= credit;

	balloon_stats.hotplug_pages += credit;
	balloon_stats.balloon_hotplug = balloon_hotplug;

	return BP_DONE;
}

static void xen_online_page(struct page *page)
{
	__online_page_set_limits(page);

	mutex_lock(&balloon_mutex);

	__balloon_append(page);

	if (balloon_stats.hotplug_pages)
		--balloon_stats.hotplug_pages;
	else
		--balloon_stats.balloon_hotplug;

=======
	balloon_state = BP_EAGAIN;
}

#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
static void release_memory_resource(struct resource *resource)
{
	if (!resource)
		return;

	/*
	 * No need to reset region to identity mapped since we now
	 * know that no I/O can be in this region
	 */
	release_resource(resource);
	kfree(resource);
}

static struct resource *additional_memory_resource(phys_addr_t size)
{
	struct resource *res;
	int ret;

	res = kzalloc(sizeof(*res), GFP_KERNEL);
	if (!res)
		return NULL;

	res->name = "System RAM";
	res->flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;

	ret = allocate_resource(&iomem_resource, res,
				size, 0, -1,
				PAGES_PER_SECTION * PAGE_SIZE, NULL, NULL);
	if (ret < 0) {
		pr_err("Cannot allocate new System RAM resource\n");
		kfree(res);
		return NULL;
	}

	return res;
}

static enum bp_state reserve_additional_memory(void)
{
	long credit;
	struct resource *resource;
	int nid, rc;
	unsigned long balloon_hotplug;

	credit = balloon_stats.target_pages + balloon_stats.target_unpopulated
		- balloon_stats.total_pages;

	/*
	 * Already hotplugged enough pages?  Wait for them to be
	 * onlined.
	 */
	if (credit <= 0)
		return BP_WAIT;

	balloon_hotplug = round_up(credit, PAGES_PER_SECTION);

	resource = additional_memory_resource(balloon_hotplug * PAGE_SIZE);
	if (!resource)
		goto err;

	nid = memory_add_physaddr_to_nid(resource->start);

#ifdef CONFIG_XEN_HAVE_PVMMU
	/*
	 * We don't support PV MMU when Linux and Xen is using
	 * different page granularity.
	 */
	BUILD_BUG_ON(XEN_PAGE_SIZE != PAGE_SIZE);

        /*
         * add_memory() will build page tables for the new memory so
         * the p2m must contain invalid entries so the correct
         * non-present PTEs will be written.
         *
         * If a failure occurs, the original (identity) p2m entries
         * are not restored since this region is now known not to
         * conflict with any devices.
         */ 
	if (!xen_feature(XENFEAT_auto_translated_physmap)) {
		unsigned long pfn, i;

		pfn = PFN_DOWN(resource->start);
		for (i = 0; i < balloon_hotplug; i++) {
			if (!set_phys_to_machine(pfn + i, INVALID_P2M_ENTRY)) {
				pr_warn("set_phys_to_machine() failed, no memory added\n");
				goto err;
			}
                }
	}
#endif

	/*
	 * add_memory_resource() will call online_pages() which in its turn
	 * will call xen_online_page() callback causing deadlock if we don't
	 * release balloon_mutex here. Unlocking here is safe because the
	 * callers drop the mutex before trying again.
	 */
	mutex_unlock(&balloon_mutex);
	/* add_memory_resource() requires the device_hotplug lock */
	lock_device_hotplug();
	rc = add_memory_resource(nid, resource, MHP_MERGE_RESOURCE);
	unlock_device_hotplug();
	mutex_lock(&balloon_mutex);

	if (rc) {
		pr_warn("Cannot add additional memory (%i)\n", rc);
		goto err;
	}

	balloon_stats.total_pages += balloon_hotplug;

	return BP_WAIT;
  err:
	release_memory_resource(resource);
	return BP_ECANCELED;
}

static void xen_online_page(struct page *page, unsigned int order)
{
	unsigned long i, size = (1 << order);
	unsigned long start_pfn = page_to_pfn(page);
	struct page *p;

	pr_debug("Online %lu pages starting at pfn 0x%lx\n", size, start_pfn);
	mutex_lock(&balloon_mutex);
	for (i = 0; i < size; i++) {
		p = pfn_to_page(start_pfn + i);
		balloon_append(p);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&balloon_mutex);
}

static int xen_memory_notifier(struct notifier_block *nb, unsigned long val, void *v)
{
	if (val == MEM_ONLINE)
<<<<<<< HEAD
		schedule_delayed_work(&balloon_worker, 0);
=======
		wake_up(&balloon_thread_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NOTIFY_OK;
}

static struct notifier_block xen_memory_nb = {
	.notifier_call = xen_memory_notifier,
	.priority = 0
};
#else
<<<<<<< HEAD
static long current_credit(void)
{
	unsigned long target = balloon_stats.target_pages;

	target = min(target,
		     balloon_stats.current_pages +
		     balloon_stats.balloon_low +
		     balloon_stats.balloon_high);

	return target - balloon_stats.current_pages;
=======
static enum bp_state reserve_additional_memory(void)
{
	balloon_stats.target_pages = balloon_stats.current_pages +
				     balloon_stats.target_unpopulated;
	return BP_ECANCELED;
}
#endif /* CONFIG_XEN_BALLOON_MEMORY_HOTPLUG */

static long current_credit(void)
{
	return balloon_stats.target_pages - balloon_stats.current_pages;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static bool balloon_is_inflated(void)
{
<<<<<<< HEAD
	if (balloon_stats.balloon_low || balloon_stats.balloon_high)
		return true;
	else
		return false;
}

static enum bp_state reserve_additional_memory(long credit)
{
	balloon_stats.target_pages = balloon_stats.current_pages;
	return BP_DONE;
}
#endif /* CONFIG_XEN_BALLOON_MEMORY_HOTPLUG */

static enum bp_state increase_reservation(unsigned long nr_pages)
{
	int rc;
	unsigned long  pfn, i;
	struct page   *page;
	struct xen_memory_reservation reservation = {
		.address_bits = 0,
		.extent_order = 0,
		.domid        = DOMID_SELF
	};

#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
	if (!balloon_stats.balloon_low && !balloon_stats.balloon_high) {
		nr_pages = min(nr_pages, balloon_stats.balloon_hotplug);
		balloon_stats.hotplug_pages += nr_pages;
		balloon_stats.balloon_hotplug -= nr_pages;
		return BP_DONE;
	}
#endif
=======
	return balloon_stats.balloon_low || balloon_stats.balloon_high;
}

static enum bp_state increase_reservation(unsigned long nr_pages)
{
	int rc;
	unsigned long i;
	struct page   *page;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (nr_pages > ARRAY_SIZE(frame_list))
		nr_pages = ARRAY_SIZE(frame_list);

<<<<<<< HEAD
	page = balloon_first_page();
=======
	page = list_first_entry_or_null(&ballooned_pages, struct page, lru);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < nr_pages; i++) {
		if (!page) {
			nr_pages = i;
			break;
		}
<<<<<<< HEAD
		frame_list[i] = page_to_pfn(page);
		page = balloon_next_page(page);
	}

	set_xen_guest_handle(reservation.extent_start, frame_list);
	reservation.nr_extents = nr_pages;
	rc = HYPERVISOR_memory_op(XENMEM_populate_physmap, &reservation);
=======

		frame_list[i] = page_to_xen_pfn(page);
		page = balloon_next_page(page);
	}

	rc = xenmem_reservation_increase(nr_pages, frame_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc <= 0)
		return BP_EAGAIN;

	for (i = 0; i < rc; i++) {
		page = balloon_retrieve(false);
		BUG_ON(page == NULL);

<<<<<<< HEAD
		pfn = page_to_pfn(page);
		BUG_ON(!xen_feature(XENFEAT_auto_translated_physmap) &&
		       phys_to_machine_mapping_valid(pfn));

		set_phys_to_machine(pfn, frame_list[i]);

		/* Link back into the page tables if not highmem. */
		if (xen_pv_domain() && !PageHighMem(page)) {
			int ret;
			ret = HYPERVISOR_update_va_mapping(
				(unsigned long)__va(pfn << PAGE_SHIFT),
				mfn_pte(frame_list[i], PAGE_KERNEL),
				0);
			BUG_ON(ret);
		}

		/* Relinquish the page back to the allocator. */
		ClearPageReserved(page);
		init_page_count(page);
		__free_page(page);
=======
		xenmem_reservation_va_mapping_update(1, &page, &frame_list[i]);

		/* Relinquish the page back to the allocator. */
		free_reserved_page(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	balloon_stats.current_pages += rc;

	return BP_DONE;
}

static enum bp_state decrease_reservation(unsigned long nr_pages, gfp_t gfp)
{
	enum bp_state state = BP_DONE;
<<<<<<< HEAD
	unsigned long  pfn, i;
	struct page   *page;
	int ret;
	struct xen_memory_reservation reservation = {
		.address_bits = 0,
		.extent_order = 0,
		.domid        = DOMID_SELF
	};

#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
	if (balloon_stats.hotplug_pages) {
		nr_pages = min(nr_pages, balloon_stats.hotplug_pages);
		balloon_stats.hotplug_pages -= nr_pages;
		balloon_stats.balloon_hotplug += nr_pages;
		return BP_DONE;
	}
#endif
=======
	unsigned long i;
	struct page *page, *tmp;
	int ret;
	LIST_HEAD(pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (nr_pages > ARRAY_SIZE(frame_list))
		nr_pages = ARRAY_SIZE(frame_list);

	for (i = 0; i < nr_pages; i++) {
<<<<<<< HEAD
		if ((page = alloc_page(gfp)) == NULL) {
=======
		page = alloc_page(gfp);
		if (page == NULL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			nr_pages = i;
			state = BP_EAGAIN;
			break;
		}
<<<<<<< HEAD

		pfn = page_to_pfn(page);
		frame_list[i] = pfn_to_mfn(pfn);

		scrub_page(page);

		if (xen_pv_domain() && !PageHighMem(page)) {
			ret = HYPERVISOR_update_va_mapping(
				(unsigned long)__va(pfn << PAGE_SHIFT),
				__pte_ma(0), 0);
			BUG_ON(ret);
		}

	}

	/* Ensure that ballooned highmem pages don't have kmaps. */
	kmap_flush_unused();
	flush_tlb_all();

	/* No more mappings: invalidate P2M and add to balloon. */
	for (i = 0; i < nr_pages; i++) {
		pfn = mfn_to_pfn(frame_list[i]);
		__set_phys_to_machine(pfn, INVALID_P2M_ENTRY);
		balloon_append(pfn_to_page(pfn));
	}

	set_xen_guest_handle(reservation.extent_start, frame_list);
	reservation.nr_extents   = nr_pages;
	ret = HYPERVISOR_memory_op(XENMEM_decrease_reservation, &reservation);
=======
		adjust_managed_page_count(page, -1);
		xenmem_reservation_scrub_page(page);
		list_add(&page->lru, &pages);
	}

	/*
	 * Ensure that ballooned highmem pages don't have kmaps.
	 *
	 * Do this before changing the p2m as kmap_flush_unused()
	 * reads PTEs to obtain pages (and hence needs the original
	 * p2m entry).
	 */
	kmap_flush_unused();

	/*
	 * Setup the frame, update direct mapping, invalidate P2M,
	 * and add to balloon.
	 */
	i = 0;
	list_for_each_entry_safe(page, tmp, &pages, lru) {
		frame_list[i++] = xen_page_to_gfn(page);

		xenmem_reservation_va_mapping_reset(1, &page);

		list_del(&page->lru);

		balloon_append(page);
	}

	flush_tlb_all();

	ret = xenmem_reservation_decrease(nr_pages, frame_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	BUG_ON(ret != nr_pages);

	balloon_stats.current_pages -= nr_pages;

	return state;
}

/*
<<<<<<< HEAD
 * We avoid multiple worker processes conflicting via the balloon mutex.
=======
 * Stop waiting if either state is BP_DONE and ballooning action is
 * needed, or if the credit has changed while state is not BP_DONE.
 */
static bool balloon_thread_cond(long credit)
{
	if (balloon_state == BP_DONE)
		credit = 0;

	return current_credit() != credit || kthread_should_stop();
}

/*
 * As this is a kthread it is guaranteed to run as a single instance only.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * We may of course race updates of the target counts (which are protected
 * by the balloon lock), or with changes to the Xen hard limit, but we will
 * recover from these in time.
 */
<<<<<<< HEAD
static void balloon_process(struct work_struct *work)
{
	enum bp_state state = BP_DONE;
	long credit;

	mutex_lock(&balloon_mutex);

	do {
=======
static int balloon_thread(void *unused)
{
	long credit;
	unsigned long timeout;

	set_freezable();
	for (;;) {
		switch (balloon_state) {
		case BP_DONE:
		case BP_ECANCELED:
			timeout = 3600 * HZ;
			break;
		case BP_EAGAIN:
			timeout = balloon_stats.schedule_delay * HZ;
			break;
		case BP_WAIT:
			timeout = HZ;
			break;
		}

		credit = current_credit();

		wait_event_freezable_timeout(balloon_thread_wq,
			balloon_thread_cond(credit), timeout);

		if (kthread_should_stop())
			return 0;

		mutex_lock(&balloon_mutex);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		credit = current_credit();

		if (credit > 0) {
			if (balloon_is_inflated())
<<<<<<< HEAD
				state = increase_reservation(credit);
			else
				state = reserve_additional_memory(credit);
		}

		if (credit < 0)
			state = decrease_reservation(-credit, GFP_BALLOON);

		state = update_schedule(state);

#ifndef CONFIG_PREEMPT
		if (need_resched())
			schedule();
#endif
	} while (credit && state == BP_DONE);

	/* Schedule more work if there is some still to be done. */
	if (state == BP_EAGAIN)
		schedule_delayed_work(&balloon_worker, balloon_stats.schedule_delay * HZ);

	mutex_unlock(&balloon_mutex);
=======
				balloon_state = increase_reservation(credit);
			else
				balloon_state = reserve_additional_memory();
		}

		if (credit < 0) {
			long n_pages;

			n_pages = min(-credit, si_mem_available());
			balloon_state = decrease_reservation(n_pages,
							     GFP_BALLOON);
			if (balloon_state == BP_DONE && n_pages != -credit &&
			    n_pages < totalreserve_pages)
				balloon_state = BP_EAGAIN;
		}

		update_schedule();

		mutex_unlock(&balloon_mutex);

		cond_resched();
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Resets the Xen limit, sets new target, and kicks off processing. */
void balloon_set_new_target(unsigned long target)
{
	/* No need for lock. Not read-modify-write updates. */
	balloon_stats.target_pages = target;
<<<<<<< HEAD
	schedule_delayed_work(&balloon_worker, 0);
}
EXPORT_SYMBOL_GPL(balloon_set_new_target);

/**
 * alloc_xenballooned_pages - get pages that have been ballooned out
 * @nr_pages: Number of pages to get
 * @pages: pages returned
 * @highmem: allow highmem pages
 * @return 0 on success, error otherwise
 */
int alloc_xenballooned_pages(int nr_pages, struct page **pages, bool highmem)
{
	int pgno = 0;
	struct page *page;
	mutex_lock(&balloon_mutex);
	while (pgno < nr_pages) {
		page = balloon_retrieve(highmem);
		if (page && (highmem || !PageHighMem(page))) {
			pages[pgno++] = page;
		} else {
			enum bp_state st;
			if (page)
				balloon_append(page);
			st = decrease_reservation(nr_pages - pgno,
					highmem ? GFP_HIGHUSER : GFP_USER);
			if (st != BP_DONE)
=======
	wake_up(&balloon_thread_wq);
}
EXPORT_SYMBOL_GPL(balloon_set_new_target);

static int add_ballooned_pages(unsigned int nr_pages)
{
	enum bp_state st;

	if (xen_hotplug_unpopulated) {
		st = reserve_additional_memory();
		if (st != BP_ECANCELED) {
			int rc;

			mutex_unlock(&balloon_mutex);
			rc = wait_event_interruptible(balloon_wq,
				   !list_empty(&ballooned_pages));
			mutex_lock(&balloon_mutex);
			return rc ? -ENOMEM : 0;
		}
	}

	if (si_mem_available() < nr_pages)
		return -ENOMEM;

	st = decrease_reservation(nr_pages, GFP_USER);
	if (st != BP_DONE)
		return -ENOMEM;

	return 0;
}

/**
 * xen_alloc_ballooned_pages - get pages that have been ballooned out
 * @nr_pages: Number of pages to get
 * @pages: pages returned
 * @return 0 on success, error otherwise
 */
int xen_alloc_ballooned_pages(unsigned int nr_pages, struct page **pages)
{
	unsigned int pgno = 0;
	struct page *page;
	int ret;

	mutex_lock(&balloon_mutex);

	balloon_stats.target_unpopulated += nr_pages;

	while (pgno < nr_pages) {
		page = balloon_retrieve(true);
		if (page) {
			pages[pgno++] = page;
#ifdef CONFIG_XEN_HAVE_PVMMU
			/*
			 * We don't support PV MMU when Linux and Xen is using
			 * different page granularity.
			 */
			BUILD_BUG_ON(XEN_PAGE_SIZE != PAGE_SIZE);

			if (!xen_feature(XENFEAT_auto_translated_physmap)) {
				ret = xen_alloc_p2m_entry(page_to_pfn(page));
				if (ret < 0)
					goto out_undo;
			}
#endif
		} else {
			ret = add_ballooned_pages(nr_pages - pgno);
			if (ret < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto out_undo;
		}
	}
	mutex_unlock(&balloon_mutex);
	return 0;
 out_undo:
<<<<<<< HEAD
	while (pgno)
		balloon_append(pages[--pgno]);
	/* Free the memory back to the kernel soon */
	schedule_delayed_work(&balloon_worker, 0);
	mutex_unlock(&balloon_mutex);
	return -ENOMEM;
}
EXPORT_SYMBOL(alloc_xenballooned_pages);

/**
 * free_xenballooned_pages - return pages retrieved with get_ballooned_pages
 * @nr_pages: Number of pages
 * @pages: pages to return
 */
void free_xenballooned_pages(int nr_pages, struct page **pages)
{
	int i;
=======
	mutex_unlock(&balloon_mutex);
	xen_free_ballooned_pages(pgno, pages);
	/*
	 * NB: xen_free_ballooned_pages will only subtract pgno pages, but since
	 * target_unpopulated is incremented with nr_pages at the start we need
	 * to remove the remaining ones also, or accounting will be screwed.
	 */
	balloon_stats.target_unpopulated -= nr_pages - pgno;
	return ret;
}
EXPORT_SYMBOL(xen_alloc_ballooned_pages);

/**
 * xen_free_ballooned_pages - return pages retrieved with get_ballooned_pages
 * @nr_pages: Number of pages
 * @pages: pages to return
 */
void xen_free_ballooned_pages(unsigned int nr_pages, struct page **pages)
{
	unsigned int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&balloon_mutex);

	for (i = 0; i < nr_pages; i++) {
		if (pages[i])
			balloon_append(pages[i]);
	}

<<<<<<< HEAD
	/* The balloon may be too large now. Shrink it if needed. */
	if (current_credit())
		schedule_delayed_work(&balloon_worker, 0);

	mutex_unlock(&balloon_mutex);
}
EXPORT_SYMBOL(free_xenballooned_pages);

static void __init balloon_add_region(unsigned long start_pfn,
				      unsigned long pages)
{
	unsigned long pfn, extra_pfn_end;
	struct page *page;

	/*
	 * If the amount of usable memory has been limited (e.g., with
	 * the 'mem' command line parameter), don't add pages beyond
	 * this limit.
	 */
	extra_pfn_end = min(max_pfn, start_pfn + pages);

	for (pfn = start_pfn; pfn < extra_pfn_end; pfn++) {
		page = pfn_to_page(pfn);
		/* totalram_pages and totalhigh_pages do not
		   include the boot-time balloon extension, so
		   don't subtract from it. */
		__balloon_append(page);
=======
	balloon_stats.target_unpopulated -= nr_pages;

	/* The balloon may be too large now. Shrink it if needed. */
	if (current_credit())
		wake_up(&balloon_thread_wq);

	mutex_unlock(&balloon_mutex);
}
EXPORT_SYMBOL(xen_free_ballooned_pages);

static void __init balloon_add_regions(void)
{
	unsigned long start_pfn, pages;
	unsigned long pfn, extra_pfn_end;
	unsigned int i;

	for (i = 0; i < XEN_EXTRA_MEM_MAX_REGIONS; i++) {
		pages = xen_extra_mem[i].n_pfns;
		if (!pages)
			continue;

		start_pfn = xen_extra_mem[i].start_pfn;

		/*
		 * If the amount of usable memory has been limited (e.g., with
		 * the 'mem' command line parameter), don't add pages beyond
		 * this limit.
		 */
		extra_pfn_end = min(max_pfn, start_pfn + pages);

		for (pfn = start_pfn; pfn < extra_pfn_end; pfn++)
			balloon_append(pfn_to_page(pfn));

		balloon_stats.total_pages += extra_pfn_end - start_pfn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static int __init balloon_init(void)
{
<<<<<<< HEAD
	int i;
=======
	struct task_struct *task;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!xen_domain())
		return -ENODEV;

<<<<<<< HEAD
	pr_info("xen/balloon: Initialising balloon driver.\n");

	balloon_stats.current_pages = xen_pv_domain()
		? min(xen_start_info->nr_pages - xen_released_pages, max_pfn)
		: max_pfn;
	balloon_stats.target_pages  = balloon_stats.current_pages;
	balloon_stats.balloon_low   = 0;
	balloon_stats.balloon_high  = 0;
=======
	pr_info("Initialising balloon driver\n");

#ifdef CONFIG_XEN_PV
	balloon_stats.current_pages = xen_pv_domain()
		? min(xen_start_info->nr_pages - xen_released_pages, max_pfn)
		: get_num_physpages();
#else
	balloon_stats.current_pages = get_num_physpages();
#endif
	balloon_stats.target_pages  = balloon_stats.current_pages;
	balloon_stats.balloon_low   = 0;
	balloon_stats.balloon_high  = 0;
	balloon_stats.total_pages   = balloon_stats.current_pages;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	balloon_stats.schedule_delay = 1;
	balloon_stats.max_schedule_delay = 32;
	balloon_stats.retry_count = 1;
<<<<<<< HEAD
	balloon_stats.max_retry_count = RETRY_UNLIMITED;

#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
	balloon_stats.hotplug_pages = 0;
	balloon_stats.balloon_hotplug = 0;

	set_online_page_callback(&xen_online_page);
	register_memory_notifier(&xen_memory_nb);
#endif

	/*
	 * Initialize the balloon with pages from the extra memory
	 * regions (see arch/x86/xen/setup.c).
	 */
	for (i = 0; i < XEN_EXTRA_MEM_MAX_REGIONS; i++)
		if (xen_extra_mem[i].size)
			balloon_add_region(PFN_UP(xen_extra_mem[i].start),
					   PFN_DOWN(xen_extra_mem[i].size));

	return 0;
}

subsys_initcall(balloon_init);

MODULE_LICENSE("GPL");
=======
	balloon_stats.max_retry_count = 4;

#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
	set_online_page_callback(&xen_online_page);
	register_memory_notifier(&xen_memory_nb);
	register_sysctl_init("xen/balloon", balloon_table);
#endif

	balloon_add_regions();

	task = kthread_run(balloon_thread, NULL, "xen-balloon");
	if (IS_ERR(task)) {
		pr_err("xen-balloon thread could not be started, ballooning will not work!\n");
		return PTR_ERR(task);
	}

	/* Init the xen-balloon driver. */
	xen_balloon_init();

	return 0;
}
subsys_initcall(balloon_init);

static int __init balloon_wait_finish(void)
{
	long credit, last_credit = 0;
	unsigned long last_changed = 0;

	if (!xen_domain())
		return -ENODEV;

	/* PV guests don't need to wait. */
	if (xen_pv_domain() || !current_credit())
		return 0;

	pr_notice("Waiting for initial ballooning down having finished.\n");

	while ((credit = current_credit()) < 0) {
		if (credit != last_credit) {
			last_changed = jiffies;
			last_credit = credit;
		}
		if (balloon_state == BP_ECANCELED) {
			pr_warn_once("Initial ballooning failed, %ld pages need to be freed.\n",
				     -credit);
			if (time_is_before_eq_jiffies(last_changed + HZ * balloon_boot_timeout))
				panic("Initial ballooning failed!\n");
		}

		schedule_timeout_interruptible(HZ / 10);
	}

	pr_notice("Initial ballooning down finished.\n");

	return 0;
}
late_initcall_sync(balloon_wait_finish);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
