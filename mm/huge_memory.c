<<<<<<< HEAD
/*
 *  Copyright (C) 2009  Red Hat, Inc.
 *
 *  This work is licensed under the terms of the GNU GPL, version 2. See
 *  the COPYING file in the top-level directory.
 */

#include <linux/mm.h>
#include <linux/sched.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 2009  Red Hat, Inc.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/sched/coredump.h>
#include <linux/sched/numa_balancing.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/highmem.h>
#include <linux/hugetlb.h>
#include <linux/mmu_notifier.h>
#include <linux/rmap.h>
#include <linux/swap.h>
<<<<<<< HEAD
#include <linux/mm_inline.h>
#include <linux/kthread.h>
#include <linux/khugepaged.h>
#include <linux/freezer.h>
#include <linux/mman.h>
#include <asm/tlb.h>
#include <asm/pgalloc.h>
#include "internal.h"

/*
 * By default transparent hugepage support is enabled for all mappings
 * and khugepaged scans all mappings. Defrag is only invoked by
 * khugepaged hugepage allocations and by page faults inside
 * MADV_HUGEPAGE regions to avoid the risk of slowing down short lived
 * allocations.
=======
#include <linux/shrinker.h>
#include <linux/mm_inline.h>
#include <linux/swapops.h>
#include <linux/backing-dev.h>
#include <linux/dax.h>
#include <linux/khugepaged.h>
#include <linux/freezer.h>
#include <linux/pfn_t.h>
#include <linux/mman.h>
#include <linux/memremap.h>
#include <linux/pagemap.h>
#include <linux/debugfs.h>
#include <linux/migrate.h>
#include <linux/hashtable.h>
#include <linux/userfaultfd_k.h>
#include <linux/page_idle.h>
#include <linux/shmem_fs.h>
#include <linux/oom.h>
#include <linux/numa.h>
#include <linux/page_owner.h>
#include <linux/sched/sysctl.h>
#include <linux/memory-tiers.h>
#include <linux/compat.h>

#include <asm/tlb.h>
#include <asm/pgalloc.h>
#include "internal.h"
#include "swap.h"

#define CREATE_TRACE_POINTS
#include <trace/events/thp.h>

/*
 * By default, transparent hugepage support is disabled in order to avoid
 * risking an increased memory footprint for applications that are not
 * guaranteed to benefit from it. When transparent hugepage support is
 * enabled, it is for all mappings, and khugepaged scans all mappings.
 * Defrag is invoked by khugepaged hugepage allocations and by page faults
 * for all hugepage allocations.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
unsigned long transparent_hugepage_flags __read_mostly =
#ifdef CONFIG_TRANSPARENT_HUGEPAGE_ALWAYS
	(1<<TRANSPARENT_HUGEPAGE_FLAG)|
#endif
#ifdef CONFIG_TRANSPARENT_HUGEPAGE_MADVISE
	(1<<TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG)|
#endif
<<<<<<< HEAD
	(1<<TRANSPARENT_HUGEPAGE_DEFRAG_FLAG)|
	(1<<TRANSPARENT_HUGEPAGE_DEFRAG_KHUGEPAGED_FLAG);

/* default scan 8*512 pte (or vmas) every 30 second */
static unsigned int khugepaged_pages_to_scan __read_mostly = HPAGE_PMD_NR*8;
static unsigned int khugepaged_pages_collapsed;
static unsigned int khugepaged_full_scans;
static unsigned int khugepaged_scan_sleep_millisecs __read_mostly = 10000;
/* during fragmentation poll the hugepage allocator once every minute */
static unsigned int khugepaged_alloc_sleep_millisecs __read_mostly = 60000;
static struct task_struct *khugepaged_thread __read_mostly;
static DEFINE_MUTEX(khugepaged_mutex);
static DEFINE_SPINLOCK(khugepaged_mm_lock);
static DECLARE_WAIT_QUEUE_HEAD(khugepaged_wait);
/*
 * default collapse hugepages if there is at least one pte mapped like
 * it would have happened if the vma was large enough during page
 * fault.
 */
static unsigned int khugepaged_max_ptes_none __read_mostly = HPAGE_PMD_NR-1;

static int khugepaged(void *none);
static int mm_slots_hash_init(void);
static int khugepaged_slab_init(void);
static void khugepaged_slab_free(void);

#define MM_SLOTS_HASH_HEADS 1024
static struct hlist_head *mm_slots_hash __read_mostly;
static struct kmem_cache *mm_slot_cache __read_mostly;

/**
 * struct mm_slot - hash lookup from mm to mm_slot
 * @hash: hash collision list
 * @mm_node: khugepaged scan list headed in khugepaged_scan.mm_head
 * @mm: the mm that this information is valid for
 */
struct mm_slot {
	struct hlist_node hash;
	struct list_head mm_node;
	struct mm_struct *mm;
};

/**
 * struct khugepaged_scan - cursor for scanning
 * @mm_head: the head of the mm list to scan
 * @mm_slot: the current mm_slot we are scanning
 * @address: the next address inside that to be scanned
 *
 * There is only the one khugepaged_scan instance of this cursor structure.
 */
struct khugepaged_scan {
	struct list_head mm_head;
	struct mm_slot *mm_slot;
	unsigned long address;
};
static struct khugepaged_scan khugepaged_scan = {
	.mm_head = LIST_HEAD_INIT(khugepaged_scan.mm_head),
};


static int set_recommended_min_free_kbytes(void)
{
	struct zone *zone;
	int nr_zones = 0;
	unsigned long recommended_min;
	extern int min_free_kbytes;

	if (!test_bit(TRANSPARENT_HUGEPAGE_FLAG,
		      &transparent_hugepage_flags) &&
	    !test_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG,
		      &transparent_hugepage_flags))
		return 0;

	for_each_populated_zone(zone)
		nr_zones++;

	/* Make sure at least 2 hugepages are free for MIGRATE_RESERVE */
	recommended_min = pageblock_nr_pages * nr_zones * 2;

	/*
	 * Make sure that on average at least two pageblocks are almost free
	 * of another type, one for a migratetype to fall back to and a
	 * second to avoid subsequent fallbacks of other types There are 3
	 * MIGRATE_TYPES we care about.
	 */
	recommended_min += pageblock_nr_pages * nr_zones *
			   MIGRATE_PCPTYPES * MIGRATE_PCPTYPES;

	/* don't ever allow to reserve more than 5% of the lowmem */
	recommended_min = min(recommended_min,
			      (unsigned long) nr_free_buffer_pages() / 20);
	recommended_min <<= (PAGE_SHIFT-10);

	if (recommended_min > min_free_kbytes)
		min_free_kbytes = recommended_min;
	setup_per_zone_wmarks();
	return 0;
}
late_initcall(set_recommended_min_free_kbytes);

static int start_khugepaged(void)
{
	int err = 0;
	if (khugepaged_enabled()) {
		int wakeup;
		if (unlikely(!mm_slot_cache || !mm_slots_hash)) {
			err = -ENOMEM;
			goto out;
		}
		mutex_lock(&khugepaged_mutex);
		if (!khugepaged_thread)
			khugepaged_thread = kthread_run(khugepaged, NULL,
							"khugepaged");
		if (unlikely(IS_ERR(khugepaged_thread))) {
			printk(KERN_ERR
			       "khugepaged: kthread_run(khugepaged) failed\n");
			err = PTR_ERR(khugepaged_thread);
			khugepaged_thread = NULL;
		}
		wakeup = !list_empty(&khugepaged_scan.mm_head);
		mutex_unlock(&khugepaged_mutex);
		if (wakeup)
			wake_up_interruptible(&khugepaged_wait);

		set_recommended_min_free_kbytes();
	} else
		/* wakeup to exit */
		wake_up_interruptible(&khugepaged_wait);
out:
	return err;
}

#ifdef CONFIG_SYSFS

static ssize_t double_flag_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf,
				enum transparent_hugepage_flag enabled,
				enum transparent_hugepage_flag req_madv)
{
	if (test_bit(enabled, &transparent_hugepage_flags)) {
		VM_BUG_ON(test_bit(req_madv, &transparent_hugepage_flags));
		return sprintf(buf, "[always] madvise never\n");
	} else if (test_bit(req_madv, &transparent_hugepage_flags))
		return sprintf(buf, "always [madvise] never\n");
	else
		return sprintf(buf, "always madvise [never]\n");
}
static ssize_t double_flag_store(struct kobject *kobj,
				 struct kobj_attribute *attr,
				 const char *buf, size_t count,
				 enum transparent_hugepage_flag enabled,
				 enum transparent_hugepage_flag req_madv)
{
	if (!memcmp("always", buf,
		    min(sizeof("always")-1, count))) {
		set_bit(enabled, &transparent_hugepage_flags);
		clear_bit(req_madv, &transparent_hugepage_flags);
	} else if (!memcmp("madvise", buf,
			   min(sizeof("madvise")-1, count))) {
		clear_bit(enabled, &transparent_hugepage_flags);
		set_bit(req_madv, &transparent_hugepage_flags);
	} else if (!memcmp("never", buf,
			   min(sizeof("never")-1, count))) {
		clear_bit(enabled, &transparent_hugepage_flags);
		clear_bit(req_madv, &transparent_hugepage_flags);
	} else
		return -EINVAL;

	return count;
}

static ssize_t enabled_show(struct kobject *kobj,
			    struct kobj_attribute *attr, char *buf)
{
	return double_flag_show(kobj, attr, buf,
				TRANSPARENT_HUGEPAGE_FLAG,
				TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG);
}
=======
	(1<<TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG)|
	(1<<TRANSPARENT_HUGEPAGE_DEFRAG_KHUGEPAGED_FLAG)|
	(1<<TRANSPARENT_HUGEPAGE_USE_ZERO_PAGE_FLAG);

static struct shrinker *deferred_split_shrinker;
static unsigned long deferred_split_count(struct shrinker *shrink,
					  struct shrink_control *sc);
static unsigned long deferred_split_scan(struct shrinker *shrink,
					 struct shrink_control *sc);

static atomic_t huge_zero_refcount;
struct page *huge_zero_page __read_mostly;
unsigned long huge_zero_pfn __read_mostly = ~0UL;
unsigned long huge_anon_orders_always __read_mostly;
unsigned long huge_anon_orders_madvise __read_mostly;
unsigned long huge_anon_orders_inherit __read_mostly;

unsigned long __thp_vma_allowable_orders(struct vm_area_struct *vma,
					 unsigned long vm_flags, bool smaps,
					 bool in_pf, bool enforce_sysfs,
					 unsigned long orders)
{
	/* Check the intersection of requested and supported orders. */
	orders &= vma_is_anonymous(vma) ?
			THP_ORDERS_ALL_ANON : THP_ORDERS_ALL_FILE;
	if (!orders)
		return 0;

	if (!vma->vm_mm)		/* vdso */
		return 0;

	/*
	 * Explicitly disabled through madvise or prctl, or some
	 * architectures may disable THP for some mappings, for
	 * example, s390 kvm.
	 * */
	if ((vm_flags & VM_NOHUGEPAGE) ||
	    test_bit(MMF_DISABLE_THP, &vma->vm_mm->flags))
		return 0;
	/*
	 * If the hardware/firmware marked hugepage support disabled.
	 */
	if (transparent_hugepage_flags & (1 << TRANSPARENT_HUGEPAGE_UNSUPPORTED))
		return 0;

	/* khugepaged doesn't collapse DAX vma, but page fault is fine. */
	if (vma_is_dax(vma))
		return in_pf ? orders : 0;

	/*
	 * khugepaged special VMA and hugetlb VMA.
	 * Must be checked after dax since some dax mappings may have
	 * VM_MIXEDMAP set.
	 */
	if (!in_pf && !smaps && (vm_flags & VM_NO_KHUGEPAGED))
		return 0;

	/*
	 * Check alignment for file vma and size for both file and anon vma by
	 * filtering out the unsuitable orders.
	 *
	 * Skip the check for page fault. Huge fault does the check in fault
	 * handlers.
	 */
	if (!in_pf) {
		int order = highest_order(orders);
		unsigned long addr;

		while (orders) {
			addr = vma->vm_end - (PAGE_SIZE << order);
			if (thp_vma_suitable_order(vma, addr, order))
				break;
			order = next_order(&orders, order);
		}

		if (!orders)
			return 0;
	}

	/*
	 * Enabled via shmem mount options or sysfs settings.
	 * Must be done before hugepage flags check since shmem has its
	 * own flags.
	 */
	if (!in_pf && shmem_file(vma->vm_file))
		return shmem_is_huge(file_inode(vma->vm_file), vma->vm_pgoff,
				     !enforce_sysfs, vma->vm_mm, vm_flags)
			? orders : 0;

	if (!vma_is_anonymous(vma)) {
		/*
		 * Enforce sysfs THP requirements as necessary. Anonymous vmas
		 * were already handled in thp_vma_allowable_orders().
		 */
		if (enforce_sysfs &&
		    (!hugepage_global_enabled() || (!(vm_flags & VM_HUGEPAGE) &&
						    !hugepage_global_always())))
			return 0;

		/*
		 * Trust that ->huge_fault() handlers know what they are doing
		 * in fault path.
		 */
		if (((in_pf || smaps)) && vma->vm_ops->huge_fault)
			return orders;
		/* Only regular file is valid in collapse path */
		if (((!in_pf || smaps)) && file_thp_enabled(vma))
			return orders;
		return 0;
	}

	if (vma_is_temporary_stack(vma))
		return 0;

	/*
	 * THPeligible bit of smaps should show 1 for proper VMAs even
	 * though anon_vma is not initialized yet.
	 *
	 * Allow page fault since anon_vma may be not initialized until
	 * the first page fault.
	 */
	if (!vma->anon_vma)
		return (smaps || in_pf) ? orders : 0;

	return orders;
}

static bool get_huge_zero_page(void)
{
	struct page *zero_page;
retry:
	if (likely(atomic_inc_not_zero(&huge_zero_refcount)))
		return true;

	zero_page = alloc_pages((GFP_TRANSHUGE | __GFP_ZERO) & ~__GFP_MOVABLE,
			HPAGE_PMD_ORDER);
	if (!zero_page) {
		count_vm_event(THP_ZERO_PAGE_ALLOC_FAILED);
		return false;
	}
	preempt_disable();
	if (cmpxchg(&huge_zero_page, NULL, zero_page)) {
		preempt_enable();
		__free_pages(zero_page, compound_order(zero_page));
		goto retry;
	}
	WRITE_ONCE(huge_zero_pfn, page_to_pfn(zero_page));

	/* We take additional reference here. It will be put back by shrinker */
	atomic_set(&huge_zero_refcount, 2);
	preempt_enable();
	count_vm_event(THP_ZERO_PAGE_ALLOC);
	return true;
}

static void put_huge_zero_page(void)
{
	/*
	 * Counter should never go to zero here. Only shrinker can put
	 * last reference.
	 */
	BUG_ON(atomic_dec_and_test(&huge_zero_refcount));
}

struct page *mm_get_huge_zero_page(struct mm_struct *mm)
{
	if (test_bit(MMF_HUGE_ZERO_PAGE, &mm->flags))
		return READ_ONCE(huge_zero_page);

	if (!get_huge_zero_page())
		return NULL;

	if (test_and_set_bit(MMF_HUGE_ZERO_PAGE, &mm->flags))
		put_huge_zero_page();

	return READ_ONCE(huge_zero_page);
}

void mm_put_huge_zero_page(struct mm_struct *mm)
{
	if (test_bit(MMF_HUGE_ZERO_PAGE, &mm->flags))
		put_huge_zero_page();
}

static unsigned long shrink_huge_zero_page_count(struct shrinker *shrink,
					struct shrink_control *sc)
{
	/* we can free zero page only if last reference remains */
	return atomic_read(&huge_zero_refcount) == 1 ? HPAGE_PMD_NR : 0;
}

static unsigned long shrink_huge_zero_page_scan(struct shrinker *shrink,
				       struct shrink_control *sc)
{
	if (atomic_cmpxchg(&huge_zero_refcount, 1, 0) == 1) {
		struct page *zero_page = xchg(&huge_zero_page, NULL);
		BUG_ON(zero_page == NULL);
		WRITE_ONCE(huge_zero_pfn, ~0UL);
		__free_pages(zero_page, compound_order(zero_page));
		return HPAGE_PMD_NR;
	}

	return 0;
}

static struct shrinker *huge_zero_page_shrinker;

#ifdef CONFIG_SYSFS
static ssize_t enabled_show(struct kobject *kobj,
			    struct kobj_attribute *attr, char *buf)
{
	const char *output;

	if (test_bit(TRANSPARENT_HUGEPAGE_FLAG, &transparent_hugepage_flags))
		output = "[always] madvise never";
	else if (test_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG,
			  &transparent_hugepage_flags))
		output = "always [madvise] never";
	else
		output = "always madvise [never]";

	return sysfs_emit(buf, "%s\n", output);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t enabled_store(struct kobject *kobj,
			     struct kobj_attribute *attr,
			     const char *buf, size_t count)
{
<<<<<<< HEAD
	ssize_t ret;

	ret = double_flag_store(kobj, attr, buf, count,
				TRANSPARENT_HUGEPAGE_FLAG,
				TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG);

	if (ret > 0) {
		int err = start_khugepaged();
		if (err)
			ret = err;
	}

	if (ret > 0 &&
	    (test_bit(TRANSPARENT_HUGEPAGE_FLAG,
		      &transparent_hugepage_flags) ||
	     test_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG,
		      &transparent_hugepage_flags)))
		set_recommended_min_free_kbytes();

	return ret;
}
static struct kobj_attribute enabled_attr =
	__ATTR(enabled, 0644, enabled_show, enabled_store);

static ssize_t single_flag_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf,
				enum transparent_hugepage_flag flag)
{
	return sprintf(buf, "%d\n",
		       !!test_bit(flag, &transparent_hugepage_flags));
}

static ssize_t single_flag_store(struct kobject *kobj,
=======
	ssize_t ret = count;

	if (sysfs_streq(buf, "always")) {
		clear_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG, &transparent_hugepage_flags);
		set_bit(TRANSPARENT_HUGEPAGE_FLAG, &transparent_hugepage_flags);
	} else if (sysfs_streq(buf, "madvise")) {
		clear_bit(TRANSPARENT_HUGEPAGE_FLAG, &transparent_hugepage_flags);
		set_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG, &transparent_hugepage_flags);
	} else if (sysfs_streq(buf, "never")) {
		clear_bit(TRANSPARENT_HUGEPAGE_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG, &transparent_hugepage_flags);
	} else
		ret = -EINVAL;

	if (ret > 0) {
		int err = start_stop_khugepaged();
		if (err)
			ret = err;
	}
	return ret;
}

static struct kobj_attribute enabled_attr = __ATTR_RW(enabled);

ssize_t single_hugepage_flag_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf,
				  enum transparent_hugepage_flag flag)
{
	return sysfs_emit(buf, "%d\n",
			  !!test_bit(flag, &transparent_hugepage_flags));
}

ssize_t single_hugepage_flag_store(struct kobject *kobj,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 struct kobj_attribute *attr,
				 const char *buf, size_t count,
				 enum transparent_hugepage_flag flag)
{
	unsigned long value;
	int ret;

	ret = kstrtoul(buf, 10, &value);
	if (ret < 0)
		return ret;
	if (value > 1)
		return -EINVAL;

	if (value)
		set_bit(flag, &transparent_hugepage_flags);
	else
		clear_bit(flag, &transparent_hugepage_flags);

	return count;
}

<<<<<<< HEAD
/*
 * Currently defrag only disables __GFP_NOWAIT for allocation. A blind
 * __GFP_REPEAT is too aggressive, it's never worth swapping tons of
 * memory just to allocate one more hugepage.
 */
static ssize_t defrag_show(struct kobject *kobj,
			   struct kobj_attribute *attr, char *buf)
{
	return double_flag_show(kobj, attr, buf,
				TRANSPARENT_HUGEPAGE_DEFRAG_FLAG,
				TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG);
}
=======
static ssize_t defrag_show(struct kobject *kobj,
			   struct kobj_attribute *attr, char *buf)
{
	const char *output;

	if (test_bit(TRANSPARENT_HUGEPAGE_DEFRAG_DIRECT_FLAG,
		     &transparent_hugepage_flags))
		output = "[always] defer defer+madvise madvise never";
	else if (test_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_FLAG,
			  &transparent_hugepage_flags))
		output = "always [defer] defer+madvise madvise never";
	else if (test_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_OR_MADV_FLAG,
			  &transparent_hugepage_flags))
		output = "always defer [defer+madvise] madvise never";
	else if (test_bit(TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG,
			  &transparent_hugepage_flags))
		output = "always defer defer+madvise [madvise] never";
	else
		output = "always defer defer+madvise madvise [never]";

	return sysfs_emit(buf, "%s\n", output);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t defrag_store(struct kobject *kobj,
			    struct kobj_attribute *attr,
			    const char *buf, size_t count)
{
<<<<<<< HEAD
	return double_flag_store(kobj, attr, buf, count,
				 TRANSPARENT_HUGEPAGE_DEFRAG_FLAG,
				 TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG);
}
static struct kobj_attribute defrag_attr =
	__ATTR(defrag, 0644, defrag_show, defrag_store);

#ifdef CONFIG_DEBUG_VM
static ssize_t debug_cow_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	return single_flag_show(kobj, attr, buf,
				TRANSPARENT_HUGEPAGE_DEBUG_COW_FLAG);
}
static ssize_t debug_cow_store(struct kobject *kobj,
			       struct kobj_attribute *attr,
			       const char *buf, size_t count)
{
	return single_flag_store(kobj, attr, buf, count,
				 TRANSPARENT_HUGEPAGE_DEBUG_COW_FLAG);
}
static struct kobj_attribute debug_cow_attr =
	__ATTR(debug_cow, 0644, debug_cow_show, debug_cow_store);
#endif /* CONFIG_DEBUG_VM */
=======
	if (sysfs_streq(buf, "always")) {
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_OR_MADV_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG, &transparent_hugepage_flags);
		set_bit(TRANSPARENT_HUGEPAGE_DEFRAG_DIRECT_FLAG, &transparent_hugepage_flags);
	} else if (sysfs_streq(buf, "defer+madvise")) {
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_DIRECT_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG, &transparent_hugepage_flags);
		set_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_OR_MADV_FLAG, &transparent_hugepage_flags);
	} else if (sysfs_streq(buf, "defer")) {
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_DIRECT_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_OR_MADV_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG, &transparent_hugepage_flags);
		set_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_FLAG, &transparent_hugepage_flags);
	} else if (sysfs_streq(buf, "madvise")) {
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_DIRECT_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_OR_MADV_FLAG, &transparent_hugepage_flags);
		set_bit(TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG, &transparent_hugepage_flags);
	} else if (sysfs_streq(buf, "never")) {
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_DIRECT_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_OR_MADV_FLAG, &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG, &transparent_hugepage_flags);
	} else
		return -EINVAL;

	return count;
}
static struct kobj_attribute defrag_attr = __ATTR_RW(defrag);

static ssize_t use_zero_page_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	return single_hugepage_flag_show(kobj, attr, buf,
					 TRANSPARENT_HUGEPAGE_USE_ZERO_PAGE_FLAG);
}
static ssize_t use_zero_page_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	return single_hugepage_flag_store(kobj, attr, buf, count,
				 TRANSPARENT_HUGEPAGE_USE_ZERO_PAGE_FLAG);
}
static struct kobj_attribute use_zero_page_attr = __ATTR_RW(use_zero_page);

static ssize_t hpage_pmd_size_show(struct kobject *kobj,
				   struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lu\n", HPAGE_PMD_SIZE);
}
static struct kobj_attribute hpage_pmd_size_attr =
	__ATTR_RO(hpage_pmd_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *hugepage_attr[] = {
	&enabled_attr.attr,
	&defrag_attr.attr,
<<<<<<< HEAD
#ifdef CONFIG_DEBUG_VM
	&debug_cow_attr.attr,
=======
	&use_zero_page_attr.attr,
	&hpage_pmd_size_attr.attr,
#ifdef CONFIG_SHMEM
	&shmem_enabled_attr.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	NULL,
};

<<<<<<< HEAD
static struct attribute_group hugepage_attr_group = {
	.attrs = hugepage_attr,
};

static ssize_t scan_sleep_millisecs_show(struct kobject *kobj,
					 struct kobj_attribute *attr,
					 char *buf)
{
	return sprintf(buf, "%u\n", khugepaged_scan_sleep_millisecs);
}

static ssize_t scan_sleep_millisecs_store(struct kobject *kobj,
					  struct kobj_attribute *attr,
					  const char *buf, size_t count)
{
	unsigned long msecs;
	int err;

	err = strict_strtoul(buf, 10, &msecs);
	if (err || msecs > UINT_MAX)
		return -EINVAL;

	khugepaged_scan_sleep_millisecs = msecs;
	wake_up_interruptible(&khugepaged_wait);

	return count;
}
static struct kobj_attribute scan_sleep_millisecs_attr =
	__ATTR(scan_sleep_millisecs, 0644, scan_sleep_millisecs_show,
	       scan_sleep_millisecs_store);

static ssize_t alloc_sleep_millisecs_show(struct kobject *kobj,
					  struct kobj_attribute *attr,
					  char *buf)
{
	return sprintf(buf, "%u\n", khugepaged_alloc_sleep_millisecs);
}

static ssize_t alloc_sleep_millisecs_store(struct kobject *kobj,
					   struct kobj_attribute *attr,
					   const char *buf, size_t count)
{
	unsigned long msecs;
	int err;

	err = strict_strtoul(buf, 10, &msecs);
	if (err || msecs > UINT_MAX)
		return -EINVAL;

	khugepaged_alloc_sleep_millisecs = msecs;
	wake_up_interruptible(&khugepaged_wait);

	return count;
}
static struct kobj_attribute alloc_sleep_millisecs_attr =
	__ATTR(alloc_sleep_millisecs, 0644, alloc_sleep_millisecs_show,
	       alloc_sleep_millisecs_store);

static ssize_t pages_to_scan_show(struct kobject *kobj,
				  struct kobj_attribute *attr,
				  char *buf)
{
	return sprintf(buf, "%u\n", khugepaged_pages_to_scan);
}
static ssize_t pages_to_scan_store(struct kobject *kobj,
				   struct kobj_attribute *attr,
				   const char *buf, size_t count)
{
	int err;
	unsigned long pages;

	err = strict_strtoul(buf, 10, &pages);
	if (err || !pages || pages > UINT_MAX)
		return -EINVAL;

	khugepaged_pages_to_scan = pages;

	return count;
}
static struct kobj_attribute pages_to_scan_attr =
	__ATTR(pages_to_scan, 0644, pages_to_scan_show,
	       pages_to_scan_store);

static ssize_t pages_collapsed_show(struct kobject *kobj,
				    struct kobj_attribute *attr,
				    char *buf)
{
	return sprintf(buf, "%u\n", khugepaged_pages_collapsed);
}
static struct kobj_attribute pages_collapsed_attr =
	__ATTR_RO(pages_collapsed);

static ssize_t full_scans_show(struct kobject *kobj,
			       struct kobj_attribute *attr,
			       char *buf)
{
	return sprintf(buf, "%u\n", khugepaged_full_scans);
}
static struct kobj_attribute full_scans_attr =
	__ATTR_RO(full_scans);

static ssize_t khugepaged_defrag_show(struct kobject *kobj,
				      struct kobj_attribute *attr, char *buf)
{
	return single_flag_show(kobj, attr, buf,
				TRANSPARENT_HUGEPAGE_DEFRAG_KHUGEPAGED_FLAG);
}
static ssize_t khugepaged_defrag_store(struct kobject *kobj,
				       struct kobj_attribute *attr,
				       const char *buf, size_t count)
{
	return single_flag_store(kobj, attr, buf, count,
				 TRANSPARENT_HUGEPAGE_DEFRAG_KHUGEPAGED_FLAG);
}
static struct kobj_attribute khugepaged_defrag_attr =
	__ATTR(defrag, 0644, khugepaged_defrag_show,
	       khugepaged_defrag_store);

/*
 * max_ptes_none controls if khugepaged should collapse hugepages over
 * any unmapped ptes in turn potentially increasing the memory
 * footprint of the vmas. When max_ptes_none is 0 khugepaged will not
 * reduce the available free memory in the system as it
 * runs. Increasing max_ptes_none will instead potentially reduce the
 * free memory in the system during the khugepaged scan.
 */
static ssize_t khugepaged_max_ptes_none_show(struct kobject *kobj,
					     struct kobj_attribute *attr,
					     char *buf)
{
	return sprintf(buf, "%u\n", khugepaged_max_ptes_none);
}
static ssize_t khugepaged_max_ptes_none_store(struct kobject *kobj,
					      struct kobj_attribute *attr,
					      const char *buf, size_t count)
{
	int err;
	unsigned long max_ptes_none;

	err = strict_strtoul(buf, 10, &max_ptes_none);
	if (err || max_ptes_none > HPAGE_PMD_NR-1)
		return -EINVAL;

	khugepaged_max_ptes_none = max_ptes_none;

	return count;
}
static struct kobj_attribute khugepaged_max_ptes_none_attr =
	__ATTR(max_ptes_none, 0644, khugepaged_max_ptes_none_show,
	       khugepaged_max_ptes_none_store);

static struct attribute *khugepaged_attr[] = {
	&khugepaged_defrag_attr.attr,
	&khugepaged_max_ptes_none_attr.attr,
	&pages_to_scan_attr.attr,
	&pages_collapsed_attr.attr,
	&full_scans_attr.attr,
	&scan_sleep_millisecs_attr.attr,
	&alloc_sleep_millisecs_attr.attr,
	NULL,
};

static struct attribute_group khugepaged_attr_group = {
	.attrs = khugepaged_attr,
	.name = "khugepaged",
};

static int __init hugepage_init_sysfs(struct kobject **hugepage_kobj)
{
	int err;

	*hugepage_kobj = kobject_create_and_add("transparent_hugepage", mm_kobj);
	if (unlikely(!*hugepage_kobj)) {
		printk(KERN_ERR "hugepage: failed kobject create\n");
=======
static const struct attribute_group hugepage_attr_group = {
	.attrs = hugepage_attr,
};

static void hugepage_exit_sysfs(struct kobject *hugepage_kobj);
static void thpsize_release(struct kobject *kobj);
static DEFINE_SPINLOCK(huge_anon_orders_lock);
static LIST_HEAD(thpsize_list);

struct thpsize {
	struct kobject kobj;
	struct list_head node;
	int order;
};

#define to_thpsize(kobj) container_of(kobj, struct thpsize, kobj)

static ssize_t thpsize_enabled_show(struct kobject *kobj,
				    struct kobj_attribute *attr, char *buf)
{
	int order = to_thpsize(kobj)->order;
	const char *output;

	if (test_bit(order, &huge_anon_orders_always))
		output = "[always] inherit madvise never";
	else if (test_bit(order, &huge_anon_orders_inherit))
		output = "always [inherit] madvise never";
	else if (test_bit(order, &huge_anon_orders_madvise))
		output = "always inherit [madvise] never";
	else
		output = "always inherit madvise [never]";

	return sysfs_emit(buf, "%s\n", output);
}

static ssize_t thpsize_enabled_store(struct kobject *kobj,
				     struct kobj_attribute *attr,
				     const char *buf, size_t count)
{
	int order = to_thpsize(kobj)->order;
	ssize_t ret = count;

	if (sysfs_streq(buf, "always")) {
		spin_lock(&huge_anon_orders_lock);
		clear_bit(order, &huge_anon_orders_inherit);
		clear_bit(order, &huge_anon_orders_madvise);
		set_bit(order, &huge_anon_orders_always);
		spin_unlock(&huge_anon_orders_lock);
	} else if (sysfs_streq(buf, "inherit")) {
		spin_lock(&huge_anon_orders_lock);
		clear_bit(order, &huge_anon_orders_always);
		clear_bit(order, &huge_anon_orders_madvise);
		set_bit(order, &huge_anon_orders_inherit);
		spin_unlock(&huge_anon_orders_lock);
	} else if (sysfs_streq(buf, "madvise")) {
		spin_lock(&huge_anon_orders_lock);
		clear_bit(order, &huge_anon_orders_always);
		clear_bit(order, &huge_anon_orders_inherit);
		set_bit(order, &huge_anon_orders_madvise);
		spin_unlock(&huge_anon_orders_lock);
	} else if (sysfs_streq(buf, "never")) {
		spin_lock(&huge_anon_orders_lock);
		clear_bit(order, &huge_anon_orders_always);
		clear_bit(order, &huge_anon_orders_inherit);
		clear_bit(order, &huge_anon_orders_madvise);
		spin_unlock(&huge_anon_orders_lock);
	} else
		ret = -EINVAL;

	return ret;
}

static struct kobj_attribute thpsize_enabled_attr =
	__ATTR(enabled, 0644, thpsize_enabled_show, thpsize_enabled_store);

static struct attribute *thpsize_attrs[] = {
	&thpsize_enabled_attr.attr,
	NULL,
};

static const struct attribute_group thpsize_attr_group = {
	.attrs = thpsize_attrs,
};

static const struct kobj_type thpsize_ktype = {
	.release = &thpsize_release,
	.sysfs_ops = &kobj_sysfs_ops,
};

static struct thpsize *thpsize_create(int order, struct kobject *parent)
{
	unsigned long size = (PAGE_SIZE << order) / SZ_1K;
	struct thpsize *thpsize;
	int ret;

	thpsize = kzalloc(sizeof(*thpsize), GFP_KERNEL);
	if (!thpsize)
		return ERR_PTR(-ENOMEM);

	ret = kobject_init_and_add(&thpsize->kobj, &thpsize_ktype, parent,
				   "hugepages-%lukB", size);
	if (ret) {
		kfree(thpsize);
		return ERR_PTR(ret);
	}

	ret = sysfs_create_group(&thpsize->kobj, &thpsize_attr_group);
	if (ret) {
		kobject_put(&thpsize->kobj);
		return ERR_PTR(ret);
	}

	thpsize->order = order;
	return thpsize;
}

static void thpsize_release(struct kobject *kobj)
{
	kfree(to_thpsize(kobj));
}

static int __init hugepage_init_sysfs(struct kobject **hugepage_kobj)
{
	int err;
	struct thpsize *thpsize;
	unsigned long orders;
	int order;

	/*
	 * Default to setting PMD-sized THP to inherit the global setting and
	 * disable all other sizes. powerpc's PMD_ORDER isn't a compile-time
	 * constant so we have to do this here.
	 */
	huge_anon_orders_inherit = BIT(PMD_ORDER);

	*hugepage_kobj = kobject_create_and_add("transparent_hugepage", mm_kobj);
	if (unlikely(!*hugepage_kobj)) {
		pr_err("failed to create transparent hugepage kobject\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;
	}

	err = sysfs_create_group(*hugepage_kobj, &hugepage_attr_group);
	if (err) {
<<<<<<< HEAD
		printk(KERN_ERR "hugepage: failed register hugeage group\n");
=======
		pr_err("failed to register transparent hugepage group\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto delete_obj;
	}

	err = sysfs_create_group(*hugepage_kobj, &khugepaged_attr_group);
	if (err) {
<<<<<<< HEAD
		printk(KERN_ERR "hugepage: failed register hugeage group\n");
		goto remove_hp_group;
	}

	return 0;

=======
		pr_err("failed to register transparent hugepage group\n");
		goto remove_hp_group;
	}

	orders = THP_ORDERS_ALL_ANON;
	order = highest_order(orders);
	while (orders) {
		thpsize = thpsize_create(order, *hugepage_kobj);
		if (IS_ERR(thpsize)) {
			pr_err("failed to create thpsize for order %d\n", order);
			err = PTR_ERR(thpsize);
			goto remove_all;
		}
		list_add(&thpsize->node, &thpsize_list);
		order = next_order(&orders, order);
	}

	return 0;

remove_all:
	hugepage_exit_sysfs(*hugepage_kobj);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
remove_hp_group:
	sysfs_remove_group(*hugepage_kobj, &hugepage_attr_group);
delete_obj:
	kobject_put(*hugepage_kobj);
	return err;
}

static void __init hugepage_exit_sysfs(struct kobject *hugepage_kobj)
{
<<<<<<< HEAD
=======
	struct thpsize *thpsize, *tmp;

	list_for_each_entry_safe(thpsize, tmp, &thpsize_list, node) {
		list_del(&thpsize->node);
		kobject_put(&thpsize->kobj);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sysfs_remove_group(hugepage_kobj, &khugepaged_attr_group);
	sysfs_remove_group(hugepage_kobj, &hugepage_attr_group);
	kobject_put(hugepage_kobj);
}
#else
static inline int hugepage_init_sysfs(struct kobject **hugepage_kobj)
{
	return 0;
}

static inline void hugepage_exit_sysfs(struct kobject *hugepage_kobj)
{
}
#endif /* CONFIG_SYSFS */

<<<<<<< HEAD
=======
static int __init thp_shrinker_init(void)
{
	huge_zero_page_shrinker = shrinker_alloc(0, "thp-zero");
	if (!huge_zero_page_shrinker)
		return -ENOMEM;

	deferred_split_shrinker = shrinker_alloc(SHRINKER_NUMA_AWARE |
						 SHRINKER_MEMCG_AWARE |
						 SHRINKER_NONSLAB,
						 "thp-deferred_split");
	if (!deferred_split_shrinker) {
		shrinker_free(huge_zero_page_shrinker);
		return -ENOMEM;
	}

	huge_zero_page_shrinker->count_objects = shrink_huge_zero_page_count;
	huge_zero_page_shrinker->scan_objects = shrink_huge_zero_page_scan;
	shrinker_register(huge_zero_page_shrinker);

	deferred_split_shrinker->count_objects = deferred_split_count;
	deferred_split_shrinker->scan_objects = deferred_split_scan;
	shrinker_register(deferred_split_shrinker);

	return 0;
}

static void __init thp_shrinker_exit(void)
{
	shrinker_free(huge_zero_page_shrinker);
	shrinker_free(deferred_split_shrinker);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init hugepage_init(void)
{
	int err;
	struct kobject *hugepage_kobj;

	if (!has_transparent_hugepage()) {
<<<<<<< HEAD
		transparent_hugepage_flags = 0;
		return -EINVAL;
	}

	err = hugepage_init_sysfs(&hugepage_kobj);
	if (err)
		return err;

	err = khugepaged_slab_init();
	if (err)
		goto out;

	err = mm_slots_hash_init();
	if (err) {
		khugepaged_slab_free();
		goto out;
	}
=======
		transparent_hugepage_flags = 1 << TRANSPARENT_HUGEPAGE_UNSUPPORTED;
		return -EINVAL;
	}

	/*
	 * hugepages can't be allocated by the buddy allocator
	 */
	MAYBE_BUILD_BUG_ON(HPAGE_PMD_ORDER > MAX_PAGE_ORDER);
	/*
	 * we use page->mapping and page->index in second tail page
	 * as list_head: assuming THP order >= 2
	 */
	MAYBE_BUILD_BUG_ON(HPAGE_PMD_ORDER < 2);

	err = hugepage_init_sysfs(&hugepage_kobj);
	if (err)
		goto err_sysfs;

	err = khugepaged_init();
	if (err)
		goto err_slab;

	err = thp_shrinker_init();
	if (err)
		goto err_shrinker;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * By default disable transparent hugepages on smaller systems,
	 * where the extra memory used could hurt more than TLB overhead
	 * is likely to save.  The admin can still enable it through /sys.
	 */
<<<<<<< HEAD
	if (totalram_pages < (512 << (20 - PAGE_SHIFT)))
		transparent_hugepage_flags = 0;

	start_khugepaged();

	set_recommended_min_free_kbytes();

	return 0;
out:
	hugepage_exit_sysfs(hugepage_kobj);
	return err;
}
module_init(hugepage_init)
=======
	if (totalram_pages() < (512 << (20 - PAGE_SHIFT))) {
		transparent_hugepage_flags = 0;
		return 0;
	}

	err = start_stop_khugepaged();
	if (err)
		goto err_khugepaged;

	return 0;
err_khugepaged:
	thp_shrinker_exit();
err_shrinker:
	khugepaged_destroy();
err_slab:
	hugepage_exit_sysfs(hugepage_kobj);
err_sysfs:
	return err;
}
subsys_initcall(hugepage_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init setup_transparent_hugepage(char *str)
{
	int ret = 0;
	if (!str)
		goto out;
	if (!strcmp(str, "always")) {
		set_bit(TRANSPARENT_HUGEPAGE_FLAG,
			&transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG,
			  &transparent_hugepage_flags);
		ret = 1;
	} else if (!strcmp(str, "madvise")) {
		clear_bit(TRANSPARENT_HUGEPAGE_FLAG,
			  &transparent_hugepage_flags);
		set_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG,
			&transparent_hugepage_flags);
		ret = 1;
	} else if (!strcmp(str, "never")) {
		clear_bit(TRANSPARENT_HUGEPAGE_FLAG,
			  &transparent_hugepage_flags);
		clear_bit(TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG,
			  &transparent_hugepage_flags);
		ret = 1;
	}
out:
	if (!ret)
<<<<<<< HEAD
		printk(KERN_WARNING
		       "transparent_hugepage= cannot parse, ignored\n");
=======
		pr_warn("transparent_hugepage= cannot parse, ignored\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
__setup("transparent_hugepage=", setup_transparent_hugepage);

<<<<<<< HEAD
static void prepare_pmd_huge_pte(pgtable_t pgtable,
				 struct mm_struct *mm)
{
	assert_spin_locked(&mm->page_table_lock);

	/* FIFO */
	if (!mm->pmd_huge_pte)
		INIT_LIST_HEAD(&pgtable->lru);
	else
		list_add(&pgtable->lru, &mm->pmd_huge_pte->lru);
	mm->pmd_huge_pte = pgtable;
}

static inline pmd_t maybe_pmd_mkwrite(pmd_t pmd, struct vm_area_struct *vma)
{
	if (likely(vma->vm_flags & VM_WRITE))
		pmd = pmd_mkwrite(pmd);
	return pmd;
}

static int __do_huge_pmd_anonymous_page(struct mm_struct *mm,
					struct vm_area_struct *vma,
					unsigned long haddr, pmd_t *pmd,
					struct page *page)
{
	int ret = 0;
	pgtable_t pgtable;

	VM_BUG_ON(!PageCompound(page));
	pgtable = pte_alloc_one(mm, haddr);
	if (unlikely(!pgtable)) {
		mem_cgroup_uncharge_page(page);
		put_page(page);
		return VM_FAULT_OOM;
	}

	clear_huge_page(page, haddr, HPAGE_PMD_NR);
	__SetPageUptodate(page);

	spin_lock(&mm->page_table_lock);
	if (unlikely(!pmd_none(*pmd))) {
		spin_unlock(&mm->page_table_lock);
		mem_cgroup_uncharge_page(page);
		put_page(page);
		pte_free(mm, pgtable);
	} else {
		pmd_t entry;
		entry = mk_pmd(page, vma->vm_page_prot);
		entry = maybe_pmd_mkwrite(pmd_mkdirty(entry), vma);
		entry = pmd_mkhuge(entry);
		/*
		 * The spinlocking to take the lru_lock inside
		 * page_add_new_anon_rmap() acts as a full memory
		 * barrier to be sure clear_huge_page writes become
		 * visible after the set_pmd_at() write.
		 */
		page_add_new_anon_rmap(page, vma, haddr);
		set_pmd_at(mm, haddr, pmd, entry);
		prepare_pmd_huge_pte(pgtable, mm);
		add_mm_counter(mm, MM_ANONPAGES, HPAGE_PMD_NR);
		mm->nr_ptes++;
		spin_unlock(&mm->page_table_lock);
	}

	return ret;
}

static inline gfp_t alloc_hugepage_gfpmask(int defrag, gfp_t extra_gfp)
{
	return (GFP_TRANSHUGE & ~(defrag ? 0 : __GFP_WAIT)) | extra_gfp;
}

static inline struct page *alloc_hugepage_vma(int defrag,
					      struct vm_area_struct *vma,
					      unsigned long haddr, int nd,
					      gfp_t extra_gfp)
{
	return alloc_pages_vma(alloc_hugepage_gfpmask(defrag, extra_gfp),
			       HPAGE_PMD_ORDER, vma, haddr, nd);
}

#ifndef CONFIG_NUMA
static inline struct page *alloc_hugepage(int defrag)
{
	return alloc_pages(alloc_hugepage_gfpmask(defrag, 0),
			   HPAGE_PMD_ORDER);
}
#endif

int do_huge_pmd_anonymous_page(struct mm_struct *mm, struct vm_area_struct *vma,
			       unsigned long address, pmd_t *pmd,
			       unsigned int flags)
{
	struct page *page;
	unsigned long haddr = address & HPAGE_PMD_MASK;
	pte_t *pte;

	if (haddr >= vma->vm_start && haddr + HPAGE_PMD_SIZE <= vma->vm_end) {
		if (unlikely(anon_vma_prepare(vma)))
			return VM_FAULT_OOM;
		if (unlikely(khugepaged_enter(vma, vma->vm_flags)))
			return VM_FAULT_OOM;
		page = alloc_hugepage_vma(transparent_hugepage_defrag(vma),
					  vma, haddr, numa_node_id(), 0);
		if (unlikely(!page)) {
			count_vm_event(THP_FAULT_FALLBACK);
			goto out;
		}
		count_vm_event(THP_FAULT_ALLOC);
		if (unlikely(mem_cgroup_newpage_charge(page, mm, GFP_KERNEL))) {
			put_page(page);
			goto out;
		}

		return __do_huge_pmd_anonymous_page(mm, vma, haddr, pmd, page);
	}
out:
	/*
	 * Use __pte_alloc instead of pte_alloc_map, because we can't
	 * run pte_offset_map on the pmd, if an huge pmd could
	 * materialize from under us from a different thread.
	 */
	if (unlikely(__pte_alloc(mm, vma, pmd, address)))
		return VM_FAULT_OOM;
	/* if an huge pmd materialized from under us just retry later */
	if (unlikely(pmd_trans_huge(*pmd)))
		return 0;
	/*
	 * A regular pmd is established and it can't morph into a huge pmd
	 * from under us anymore at this point because we hold the mmap_sem
	 * read mode and khugepaged takes it in write mode. So now it's
	 * safe to run pte_offset_map().
	 */
	pte = pte_offset_map(pmd, address);
	return handle_pte_fault(mm, vma, address, pte, pmd, flags);
=======
pmd_t maybe_pmd_mkwrite(pmd_t pmd, struct vm_area_struct *vma)
{
	if (likely(vma->vm_flags & VM_WRITE))
		pmd = pmd_mkwrite(pmd, vma);
	return pmd;
}

#ifdef CONFIG_MEMCG
static inline
struct deferred_split *get_deferred_split_queue(struct folio *folio)
{
	struct mem_cgroup *memcg = folio_memcg(folio);
	struct pglist_data *pgdat = NODE_DATA(folio_nid(folio));

	if (memcg)
		return &memcg->deferred_split_queue;
	else
		return &pgdat->deferred_split_queue;
}
#else
static inline
struct deferred_split *get_deferred_split_queue(struct folio *folio)
{
	struct pglist_data *pgdat = NODE_DATA(folio_nid(folio));

	return &pgdat->deferred_split_queue;
}
#endif

void folio_prep_large_rmappable(struct folio *folio)
{
	if (!folio || !folio_test_large(folio))
		return;
	if (folio_order(folio) > 1)
		INIT_LIST_HEAD(&folio->_deferred_list);
	folio_set_large_rmappable(folio);
}

static inline bool is_transparent_hugepage(struct folio *folio)
{
	if (!folio_test_large(folio))
		return false;

	return is_huge_zero_page(&folio->page) ||
		folio_test_large_rmappable(folio);
}

static unsigned long __thp_get_unmapped_area(struct file *filp,
		unsigned long addr, unsigned long len,
		loff_t off, unsigned long flags, unsigned long size)
{
	loff_t off_end = off + len;
	loff_t off_align = round_up(off, size);
	unsigned long len_pad, ret, off_sub;

	if (IS_ENABLED(CONFIG_32BIT) || in_compat_syscall())
		return 0;

	if (off_end <= off_align || (off_end - off_align) < size)
		return 0;

	len_pad = len + size;
	if (len_pad < len || (off + len_pad) < off)
		return 0;

	ret = current->mm->get_unmapped_area(filp, addr, len_pad,
					      off >> PAGE_SHIFT, flags);

	/*
	 * The failure might be due to length padding. The caller will retry
	 * without the padding.
	 */
	if (IS_ERR_VALUE(ret))
		return 0;

	/*
	 * Do not try to align to THP boundary if allocation at the address
	 * hint succeeds.
	 */
	if (ret == addr)
		return addr;

	off_sub = (off - ret) & (size - 1);

	if (current->mm->get_unmapped_area == arch_get_unmapped_area_topdown &&
	    !off_sub)
		return ret + size;

	ret += off_sub;
	return ret;
}

unsigned long thp_get_unmapped_area(struct file *filp, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	unsigned long ret;
	loff_t off = (loff_t)pgoff << PAGE_SHIFT;

	ret = __thp_get_unmapped_area(filp, addr, len, off, flags, PMD_SIZE);
	if (ret)
		return ret;

	return current->mm->get_unmapped_area(filp, addr, len, pgoff, flags);
}
EXPORT_SYMBOL_GPL(thp_get_unmapped_area);

static vm_fault_t __do_huge_pmd_anonymous_page(struct vm_fault *vmf,
			struct page *page, gfp_t gfp)
{
	struct vm_area_struct *vma = vmf->vma;
	struct folio *folio = page_folio(page);
	pgtable_t pgtable;
	unsigned long haddr = vmf->address & HPAGE_PMD_MASK;
	vm_fault_t ret = 0;

	VM_BUG_ON_FOLIO(!folio_test_large(folio), folio);

	if (mem_cgroup_charge(folio, vma->vm_mm, gfp)) {
		folio_put(folio);
		count_vm_event(THP_FAULT_FALLBACK);
		count_vm_event(THP_FAULT_FALLBACK_CHARGE);
		return VM_FAULT_FALLBACK;
	}
	folio_throttle_swaprate(folio, gfp);

	pgtable = pte_alloc_one(vma->vm_mm);
	if (unlikely(!pgtable)) {
		ret = VM_FAULT_OOM;
		goto release;
	}

	clear_huge_page(page, vmf->address, HPAGE_PMD_NR);
	/*
	 * The memory barrier inside __folio_mark_uptodate makes sure that
	 * clear_huge_page writes become visible before the set_pmd_at()
	 * write.
	 */
	__folio_mark_uptodate(folio);

	vmf->ptl = pmd_lock(vma->vm_mm, vmf->pmd);
	if (unlikely(!pmd_none(*vmf->pmd))) {
		goto unlock_release;
	} else {
		pmd_t entry;

		ret = check_stable_address_space(vma->vm_mm);
		if (ret)
			goto unlock_release;

		/* Deliver the page fault to userland */
		if (userfaultfd_missing(vma)) {
			spin_unlock(vmf->ptl);
			folio_put(folio);
			pte_free(vma->vm_mm, pgtable);
			ret = handle_userfault(vmf, VM_UFFD_MISSING);
			VM_BUG_ON(ret & VM_FAULT_FALLBACK);
			return ret;
		}

		entry = mk_huge_pmd(page, vma->vm_page_prot);
		entry = maybe_pmd_mkwrite(pmd_mkdirty(entry), vma);
		folio_add_new_anon_rmap(folio, vma, haddr);
		folio_add_lru_vma(folio, vma);
		pgtable_trans_huge_deposit(vma->vm_mm, vmf->pmd, pgtable);
		set_pmd_at(vma->vm_mm, haddr, vmf->pmd, entry);
		update_mmu_cache_pmd(vma, vmf->address, vmf->pmd);
		add_mm_counter(vma->vm_mm, MM_ANONPAGES, HPAGE_PMD_NR);
		mm_inc_nr_ptes(vma->vm_mm);
		spin_unlock(vmf->ptl);
		count_vm_event(THP_FAULT_ALLOC);
		count_memcg_event_mm(vma->vm_mm, THP_FAULT_ALLOC);
	}

	return 0;
unlock_release:
	spin_unlock(vmf->ptl);
release:
	if (pgtable)
		pte_free(vma->vm_mm, pgtable);
	folio_put(folio);
	return ret;

}

/*
 * always: directly stall for all thp allocations
 * defer: wake kswapd and fail if not immediately available
 * defer+madvise: wake kswapd and directly stall for MADV_HUGEPAGE, otherwise
 *		  fail if not immediately available
 * madvise: directly stall for MADV_HUGEPAGE, otherwise fail if not immediately
 *	    available
 * never: never stall for any thp allocation
 */
gfp_t vma_thp_gfp_mask(struct vm_area_struct *vma)
{
	const bool vma_madvised = vma && (vma->vm_flags & VM_HUGEPAGE);

	/* Always do synchronous compaction */
	if (test_bit(TRANSPARENT_HUGEPAGE_DEFRAG_DIRECT_FLAG, &transparent_hugepage_flags))
		return GFP_TRANSHUGE | (vma_madvised ? 0 : __GFP_NORETRY);

	/* Kick kcompactd and fail quickly */
	if (test_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_FLAG, &transparent_hugepage_flags))
		return GFP_TRANSHUGE_LIGHT | __GFP_KSWAPD_RECLAIM;

	/* Synchronous compaction if madvised, otherwise kick kcompactd */
	if (test_bit(TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_OR_MADV_FLAG, &transparent_hugepage_flags))
		return GFP_TRANSHUGE_LIGHT |
			(vma_madvised ? __GFP_DIRECT_RECLAIM :
					__GFP_KSWAPD_RECLAIM);

	/* Only do synchronous compaction if madvised */
	if (test_bit(TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG, &transparent_hugepage_flags))
		return GFP_TRANSHUGE_LIGHT |
		       (vma_madvised ? __GFP_DIRECT_RECLAIM : 0);

	return GFP_TRANSHUGE_LIGHT;
}

/* Caller must hold page table lock. */
static void set_huge_zero_page(pgtable_t pgtable, struct mm_struct *mm,
		struct vm_area_struct *vma, unsigned long haddr, pmd_t *pmd,
		struct page *zero_page)
{
	pmd_t entry;
	if (!pmd_none(*pmd))
		return;
	entry = mk_pmd(zero_page, vma->vm_page_prot);
	entry = pmd_mkhuge(entry);
	pgtable_trans_huge_deposit(mm, pmd, pgtable);
	set_pmd_at(mm, haddr, pmd, entry);
	mm_inc_nr_ptes(mm);
}

vm_fault_t do_huge_pmd_anonymous_page(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	gfp_t gfp;
	struct folio *folio;
	unsigned long haddr = vmf->address & HPAGE_PMD_MASK;

	if (!thp_vma_suitable_order(vma, haddr, PMD_ORDER))
		return VM_FAULT_FALLBACK;
	if (unlikely(anon_vma_prepare(vma)))
		return VM_FAULT_OOM;
	khugepaged_enter_vma(vma, vma->vm_flags);

	if (!(vmf->flags & FAULT_FLAG_WRITE) &&
			!mm_forbids_zeropage(vma->vm_mm) &&
			transparent_hugepage_use_zero_page()) {
		pgtable_t pgtable;
		struct page *zero_page;
		vm_fault_t ret;
		pgtable = pte_alloc_one(vma->vm_mm);
		if (unlikely(!pgtable))
			return VM_FAULT_OOM;
		zero_page = mm_get_huge_zero_page(vma->vm_mm);
		if (unlikely(!zero_page)) {
			pte_free(vma->vm_mm, pgtable);
			count_vm_event(THP_FAULT_FALLBACK);
			return VM_FAULT_FALLBACK;
		}
		vmf->ptl = pmd_lock(vma->vm_mm, vmf->pmd);
		ret = 0;
		if (pmd_none(*vmf->pmd)) {
			ret = check_stable_address_space(vma->vm_mm);
			if (ret) {
				spin_unlock(vmf->ptl);
				pte_free(vma->vm_mm, pgtable);
			} else if (userfaultfd_missing(vma)) {
				spin_unlock(vmf->ptl);
				pte_free(vma->vm_mm, pgtable);
				ret = handle_userfault(vmf, VM_UFFD_MISSING);
				VM_BUG_ON(ret & VM_FAULT_FALLBACK);
			} else {
				set_huge_zero_page(pgtable, vma->vm_mm, vma,
						   haddr, vmf->pmd, zero_page);
				update_mmu_cache_pmd(vma, vmf->address, vmf->pmd);
				spin_unlock(vmf->ptl);
			}
		} else {
			spin_unlock(vmf->ptl);
			pte_free(vma->vm_mm, pgtable);
		}
		return ret;
	}
	gfp = vma_thp_gfp_mask(vma);
	folio = vma_alloc_folio(gfp, HPAGE_PMD_ORDER, vma, haddr, true);
	if (unlikely(!folio)) {
		count_vm_event(THP_FAULT_FALLBACK);
		return VM_FAULT_FALLBACK;
	}
	return __do_huge_pmd_anonymous_page(vmf, &folio->page, gfp);
}

static void insert_pfn_pmd(struct vm_area_struct *vma, unsigned long addr,
		pmd_t *pmd, pfn_t pfn, pgprot_t prot, bool write,
		pgtable_t pgtable)
{
	struct mm_struct *mm = vma->vm_mm;
	pmd_t entry;
	spinlock_t *ptl;

	ptl = pmd_lock(mm, pmd);
	if (!pmd_none(*pmd)) {
		if (write) {
			if (pmd_pfn(*pmd) != pfn_t_to_pfn(pfn)) {
				WARN_ON_ONCE(!is_huge_zero_pmd(*pmd));
				goto out_unlock;
			}
			entry = pmd_mkyoung(*pmd);
			entry = maybe_pmd_mkwrite(pmd_mkdirty(entry), vma);
			if (pmdp_set_access_flags(vma, addr, pmd, entry, 1))
				update_mmu_cache_pmd(vma, addr, pmd);
		}

		goto out_unlock;
	}

	entry = pmd_mkhuge(pfn_t_pmd(pfn, prot));
	if (pfn_t_devmap(pfn))
		entry = pmd_mkdevmap(entry);
	if (write) {
		entry = pmd_mkyoung(pmd_mkdirty(entry));
		entry = maybe_pmd_mkwrite(entry, vma);
	}

	if (pgtable) {
		pgtable_trans_huge_deposit(mm, pmd, pgtable);
		mm_inc_nr_ptes(mm);
		pgtable = NULL;
	}

	set_pmd_at(mm, addr, pmd, entry);
	update_mmu_cache_pmd(vma, addr, pmd);

out_unlock:
	spin_unlock(ptl);
	if (pgtable)
		pte_free(mm, pgtable);
}

/**
 * vmf_insert_pfn_pmd - insert a pmd size pfn
 * @vmf: Structure describing the fault
 * @pfn: pfn to insert
 * @write: whether it's a write fault
 *
 * Insert a pmd size pfn. See vmf_insert_pfn() for additional info.
 *
 * Return: vm_fault_t value.
 */
vm_fault_t vmf_insert_pfn_pmd(struct vm_fault *vmf, pfn_t pfn, bool write)
{
	unsigned long addr = vmf->address & PMD_MASK;
	struct vm_area_struct *vma = vmf->vma;
	pgprot_t pgprot = vma->vm_page_prot;
	pgtable_t pgtable = NULL;

	/*
	 * If we had pmd_special, we could avoid all these restrictions,
	 * but we need to be consistent with PTEs and architectures that
	 * can't support a 'special' bit.
	 */
	BUG_ON(!(vma->vm_flags & (VM_PFNMAP|VM_MIXEDMAP)) &&
			!pfn_t_devmap(pfn));
	BUG_ON((vma->vm_flags & (VM_PFNMAP|VM_MIXEDMAP)) ==
						(VM_PFNMAP|VM_MIXEDMAP));
	BUG_ON((vma->vm_flags & VM_PFNMAP) && is_cow_mapping(vma->vm_flags));

	if (addr < vma->vm_start || addr >= vma->vm_end)
		return VM_FAULT_SIGBUS;

	if (arch_needs_pgtable_deposit()) {
		pgtable = pte_alloc_one(vma->vm_mm);
		if (!pgtable)
			return VM_FAULT_OOM;
	}

	track_pfn_insert(vma, &pgprot, pfn);

	insert_pfn_pmd(vma, addr, vmf->pmd, pfn, pgprot, write, pgtable);
	return VM_FAULT_NOPAGE;
}
EXPORT_SYMBOL_GPL(vmf_insert_pfn_pmd);

#ifdef CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD
static pud_t maybe_pud_mkwrite(pud_t pud, struct vm_area_struct *vma)
{
	if (likely(vma->vm_flags & VM_WRITE))
		pud = pud_mkwrite(pud);
	return pud;
}

static void insert_pfn_pud(struct vm_area_struct *vma, unsigned long addr,
		pud_t *pud, pfn_t pfn, bool write)
{
	struct mm_struct *mm = vma->vm_mm;
	pgprot_t prot = vma->vm_page_prot;
	pud_t entry;
	spinlock_t *ptl;

	ptl = pud_lock(mm, pud);
	if (!pud_none(*pud)) {
		if (write) {
			if (pud_pfn(*pud) != pfn_t_to_pfn(pfn)) {
				WARN_ON_ONCE(!is_huge_zero_pud(*pud));
				goto out_unlock;
			}
			entry = pud_mkyoung(*pud);
			entry = maybe_pud_mkwrite(pud_mkdirty(entry), vma);
			if (pudp_set_access_flags(vma, addr, pud, entry, 1))
				update_mmu_cache_pud(vma, addr, pud);
		}
		goto out_unlock;
	}

	entry = pud_mkhuge(pfn_t_pud(pfn, prot));
	if (pfn_t_devmap(pfn))
		entry = pud_mkdevmap(entry);
	if (write) {
		entry = pud_mkyoung(pud_mkdirty(entry));
		entry = maybe_pud_mkwrite(entry, vma);
	}
	set_pud_at(mm, addr, pud, entry);
	update_mmu_cache_pud(vma, addr, pud);

out_unlock:
	spin_unlock(ptl);
}

/**
 * vmf_insert_pfn_pud - insert a pud size pfn
 * @vmf: Structure describing the fault
 * @pfn: pfn to insert
 * @write: whether it's a write fault
 *
 * Insert a pud size pfn. See vmf_insert_pfn() for additional info.
 *
 * Return: vm_fault_t value.
 */
vm_fault_t vmf_insert_pfn_pud(struct vm_fault *vmf, pfn_t pfn, bool write)
{
	unsigned long addr = vmf->address & PUD_MASK;
	struct vm_area_struct *vma = vmf->vma;
	pgprot_t pgprot = vma->vm_page_prot;

	/*
	 * If we had pud_special, we could avoid all these restrictions,
	 * but we need to be consistent with PTEs and architectures that
	 * can't support a 'special' bit.
	 */
	BUG_ON(!(vma->vm_flags & (VM_PFNMAP|VM_MIXEDMAP)) &&
			!pfn_t_devmap(pfn));
	BUG_ON((vma->vm_flags & (VM_PFNMAP|VM_MIXEDMAP)) ==
						(VM_PFNMAP|VM_MIXEDMAP));
	BUG_ON((vma->vm_flags & VM_PFNMAP) && is_cow_mapping(vma->vm_flags));

	if (addr < vma->vm_start || addr >= vma->vm_end)
		return VM_FAULT_SIGBUS;

	track_pfn_insert(vma, &pgprot, pfn);

	insert_pfn_pud(vma, addr, vmf->pud, pfn, write);
	return VM_FAULT_NOPAGE;
}
EXPORT_SYMBOL_GPL(vmf_insert_pfn_pud);
#endif /* CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD */

static void touch_pmd(struct vm_area_struct *vma, unsigned long addr,
		      pmd_t *pmd, bool write)
{
	pmd_t _pmd;

	_pmd = pmd_mkyoung(*pmd);
	if (write)
		_pmd = pmd_mkdirty(_pmd);
	if (pmdp_set_access_flags(vma, addr & HPAGE_PMD_MASK,
				  pmd, _pmd, write))
		update_mmu_cache_pmd(vma, addr, pmd);
}

struct page *follow_devmap_pmd(struct vm_area_struct *vma, unsigned long addr,
		pmd_t *pmd, int flags, struct dev_pagemap **pgmap)
{
	unsigned long pfn = pmd_pfn(*pmd);
	struct mm_struct *mm = vma->vm_mm;
	struct page *page;
	int ret;

	assert_spin_locked(pmd_lockptr(mm, pmd));

	if (flags & FOLL_WRITE && !pmd_write(*pmd))
		return NULL;

	if (pmd_present(*pmd) && pmd_devmap(*pmd))
		/* pass */;
	else
		return NULL;

	if (flags & FOLL_TOUCH)
		touch_pmd(vma, addr, pmd, flags & FOLL_WRITE);

	/*
	 * device mapped pages can only be returned if the
	 * caller will manage the page reference count.
	 */
	if (!(flags & (FOLL_GET | FOLL_PIN)))
		return ERR_PTR(-EEXIST);

	pfn += (addr & ~PMD_MASK) >> PAGE_SHIFT;
	*pgmap = get_dev_pagemap(pfn, *pgmap);
	if (!*pgmap)
		return ERR_PTR(-EFAULT);
	page = pfn_to_page(pfn);
	ret = try_grab_page(page, flags);
	if (ret)
		page = ERR_PTR(ret);

	return page;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int copy_huge_pmd(struct mm_struct *dst_mm, struct mm_struct *src_mm,
		  pmd_t *dst_pmd, pmd_t *src_pmd, unsigned long addr,
<<<<<<< HEAD
		  struct vm_area_struct *vma)
{
	struct page *src_page;
	pmd_t pmd;
	pgtable_t pgtable;
	int ret;

	ret = -ENOMEM;
	pgtable = pte_alloc_one(dst_mm, addr);
	if (unlikely(!pgtable))
		goto out;

	spin_lock(&dst_mm->page_table_lock);
	spin_lock_nested(&src_mm->page_table_lock, SINGLE_DEPTH_NESTING);

	ret = -EAGAIN;
	pmd = *src_pmd;
=======
		  struct vm_area_struct *dst_vma, struct vm_area_struct *src_vma)
{
	spinlock_t *dst_ptl, *src_ptl;
	struct page *src_page;
	struct folio *src_folio;
	pmd_t pmd;
	pgtable_t pgtable = NULL;
	int ret = -ENOMEM;

	/* Skip if can be re-fill on fault */
	if (!vma_is_anonymous(dst_vma))
		return 0;

	pgtable = pte_alloc_one(dst_mm);
	if (unlikely(!pgtable))
		goto out;

	dst_ptl = pmd_lock(dst_mm, dst_pmd);
	src_ptl = pmd_lockptr(src_mm, src_pmd);
	spin_lock_nested(src_ptl, SINGLE_DEPTH_NESTING);

	ret = -EAGAIN;
	pmd = *src_pmd;

#ifdef CONFIG_ARCH_ENABLE_THP_MIGRATION
	if (unlikely(is_swap_pmd(pmd))) {
		swp_entry_t entry = pmd_to_swp_entry(pmd);

		VM_BUG_ON(!is_pmd_migration_entry(pmd));
		if (!is_readable_migration_entry(entry)) {
			entry = make_readable_migration_entry(
							swp_offset(entry));
			pmd = swp_entry_to_pmd(entry);
			if (pmd_swp_soft_dirty(*src_pmd))
				pmd = pmd_swp_mksoft_dirty(pmd);
			if (pmd_swp_uffd_wp(*src_pmd))
				pmd = pmd_swp_mkuffd_wp(pmd);
			set_pmd_at(src_mm, addr, src_pmd, pmd);
		}
		add_mm_counter(dst_mm, MM_ANONPAGES, HPAGE_PMD_NR);
		mm_inc_nr_ptes(dst_mm);
		pgtable_trans_huge_deposit(dst_mm, dst_pmd, pgtable);
		if (!userfaultfd_wp(dst_vma))
			pmd = pmd_swp_clear_uffd_wp(pmd);
		set_pmd_at(dst_mm, addr, dst_pmd, pmd);
		ret = 0;
		goto out_unlock;
	}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(!pmd_trans_huge(pmd))) {
		pte_free(dst_mm, pgtable);
		goto out_unlock;
	}
<<<<<<< HEAD
	if (unlikely(pmd_trans_splitting(pmd))) {
		/* split huge page running from under us */
		spin_unlock(&src_mm->page_table_lock);
		spin_unlock(&dst_mm->page_table_lock);
		pte_free(dst_mm, pgtable);

		wait_split_huge_page(vma->anon_vma, src_pmd); /* src_vma */
		goto out;
	}
	src_page = pmd_page(pmd);
	VM_BUG_ON(!PageHead(src_page));
	get_page(src_page);
	page_dup_rmap(src_page);
	add_mm_counter(dst_mm, MM_ANONPAGES, HPAGE_PMD_NR);

	pmdp_set_wrprotect(src_mm, addr, src_pmd);
	pmd = pmd_mkold(pmd_wrprotect(pmd));
	set_pmd_at(dst_mm, addr, dst_pmd, pmd);
	prepare_pmd_huge_pte(pgtable, dst_mm);
	dst_mm->nr_ptes++;

	ret = 0;
out_unlock:
	spin_unlock(&src_mm->page_table_lock);
	spin_unlock(&dst_mm->page_table_lock);
=======
	/*
	 * When page table lock is held, the huge zero pmd should not be
	 * under splitting since we don't split the page itself, only pmd to
	 * a page table.
	 */
	if (is_huge_zero_pmd(pmd)) {
		/*
		 * get_huge_zero_page() will never allocate a new page here,
		 * since we already have a zero page to copy. It just takes a
		 * reference.
		 */
		mm_get_huge_zero_page(dst_mm);
		goto out_zero_page;
	}

	src_page = pmd_page(pmd);
	VM_BUG_ON_PAGE(!PageHead(src_page), src_page);
	src_folio = page_folio(src_page);

	folio_get(src_folio);
	if (unlikely(folio_try_dup_anon_rmap_pmd(src_folio, src_page, src_vma))) {
		/* Page maybe pinned: split and retry the fault on PTEs. */
		folio_put(src_folio);
		pte_free(dst_mm, pgtable);
		spin_unlock(src_ptl);
		spin_unlock(dst_ptl);
		__split_huge_pmd(src_vma, src_pmd, addr, false, NULL);
		return -EAGAIN;
	}
	add_mm_counter(dst_mm, MM_ANONPAGES, HPAGE_PMD_NR);
out_zero_page:
	mm_inc_nr_ptes(dst_mm);
	pgtable_trans_huge_deposit(dst_mm, dst_pmd, pgtable);
	pmdp_set_wrprotect(src_mm, addr, src_pmd);
	if (!userfaultfd_wp(dst_vma))
		pmd = pmd_clear_uffd_wp(pmd);
	pmd = pmd_mkold(pmd_wrprotect(pmd));
	set_pmd_at(dst_mm, addr, dst_pmd, pmd);

	ret = 0;
out_unlock:
	spin_unlock(src_ptl);
	spin_unlock(dst_ptl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return ret;
}

<<<<<<< HEAD
/* no "address" argument so destroys page coloring of some arch */
pgtable_t get_pmd_huge_pte(struct mm_struct *mm)
{
	pgtable_t pgtable;

	assert_spin_locked(&mm->page_table_lock);

	/* FIFO */
	pgtable = mm->pmd_huge_pte;
	if (list_empty(&pgtable->lru))
		mm->pmd_huge_pte = NULL;
	else {
		mm->pmd_huge_pte = list_entry(pgtable->lru.next,
					      struct page, lru);
		list_del(&pgtable->lru);
	}
	return pgtable;
}

static int do_huge_pmd_wp_page_fallback(struct mm_struct *mm,
					struct vm_area_struct *vma,
					unsigned long address,
					pmd_t *pmd, pmd_t orig_pmd,
					struct page *page,
					unsigned long haddr)
{
	pgtable_t pgtable;
	pmd_t _pmd;
	int ret = 0, i;
	struct page **pages;

	pages = kmalloc(sizeof(struct page *) * HPAGE_PMD_NR,
			GFP_KERNEL);
	if (unlikely(!pages)) {
		ret |= VM_FAULT_OOM;
		goto out;
	}

	for (i = 0; i < HPAGE_PMD_NR; i++) {
		pages[i] = alloc_page_vma_node(GFP_HIGHUSER_MOVABLE |
					       __GFP_OTHER_NODE,
					       vma, address, page_to_nid(page));
		if (unlikely(!pages[i] ||
			     mem_cgroup_newpage_charge(pages[i], mm,
						       GFP_KERNEL))) {
			if (pages[i])
				put_page(pages[i]);
			mem_cgroup_uncharge_start();
			while (--i >= 0) {
				mem_cgroup_uncharge_page(pages[i]);
				put_page(pages[i]);
			}
			mem_cgroup_uncharge_end();
			kfree(pages);
			ret |= VM_FAULT_OOM;
			goto out;
		}
	}

	for (i = 0; i < HPAGE_PMD_NR; i++) {
		copy_user_highpage(pages[i], page + i,
				   haddr + PAGE_SIZE * i, vma);
		__SetPageUptodate(pages[i]);
		cond_resched();
	}

	spin_lock(&mm->page_table_lock);
	if (unlikely(!pmd_same(*pmd, orig_pmd)))
		goto out_free_pages;
	VM_BUG_ON(!PageHead(page));

	pmdp_clear_flush_notify(vma, haddr, pmd);
	/* leave pmd empty until pte is filled */

	pgtable = get_pmd_huge_pte(mm);
	pmd_populate(mm, &_pmd, pgtable);

	for (i = 0; i < HPAGE_PMD_NR; i++, haddr += PAGE_SIZE) {
		pte_t *pte, entry;
		entry = mk_pte(pages[i], vma->vm_page_prot);
		entry = maybe_mkwrite(pte_mkdirty(entry), vma);
		page_add_new_anon_rmap(pages[i], vma, haddr);
		pte = pte_offset_map(&_pmd, haddr);
		VM_BUG_ON(!pte_none(*pte));
		set_pte_at(mm, haddr, pte, entry);
		pte_unmap(pte);
	}
	kfree(pages);

	smp_wmb(); /* make pte visible before pmd */
	pmd_populate(mm, pmd, pgtable);
	page_remove_rmap(page);
	spin_unlock(&mm->page_table_lock);

	ret |= VM_FAULT_WRITE;
	put_page(page);

out:
	return ret;

out_free_pages:
	spin_unlock(&mm->page_table_lock);
	mem_cgroup_uncharge_start();
	for (i = 0; i < HPAGE_PMD_NR; i++) {
		mem_cgroup_uncharge_page(pages[i]);
		put_page(pages[i]);
	}
	mem_cgroup_uncharge_end();
	kfree(pages);
	goto out;
}

int do_huge_pmd_wp_page(struct mm_struct *mm, struct vm_area_struct *vma,
			unsigned long address, pmd_t *pmd, pmd_t orig_pmd)
{
	int ret = 0;
	struct page *page, *new_page;
	unsigned long haddr;

	VM_BUG_ON(!vma->anon_vma);
	spin_lock(&mm->page_table_lock);
	if (unlikely(!pmd_same(*pmd, orig_pmd)))
		goto out_unlock;

	page = pmd_page(orig_pmd);
	VM_BUG_ON(!PageCompound(page) || !PageHead(page));
	haddr = address & HPAGE_PMD_MASK;
	if (page_mapcount(page) == 1) {
		pmd_t entry;
		entry = pmd_mkyoung(orig_pmd);
		entry = maybe_pmd_mkwrite(pmd_mkdirty(entry), vma);
		if (pmdp_set_access_flags(vma, haddr, pmd, entry,  1))
			update_mmu_cache(vma, address, entry);
		ret |= VM_FAULT_WRITE;
		goto out_unlock;
	}
	get_page(page);
	spin_unlock(&mm->page_table_lock);

	if (transparent_hugepage_enabled(vma) &&
	    !transparent_hugepage_debug_cow())
		new_page = alloc_hugepage_vma(transparent_hugepage_defrag(vma),
					      vma, haddr, numa_node_id(), 0);
	else
		new_page = NULL;

	if (unlikely(!new_page)) {
		count_vm_event(THP_FAULT_FALLBACK);
		ret = do_huge_pmd_wp_page_fallback(mm, vma, address,
						   pmd, orig_pmd, page, haddr);
		if (ret & VM_FAULT_OOM)
			split_huge_page(page);
		put_page(page);
		goto out;
	}
	count_vm_event(THP_FAULT_ALLOC);

	if (unlikely(mem_cgroup_newpage_charge(new_page, mm, GFP_KERNEL))) {
		put_page(new_page);
		split_huge_page(page);
		put_page(page);
		ret |= VM_FAULT_OOM;
		goto out;
	}

	copy_user_huge_page(new_page, page, haddr, vma, HPAGE_PMD_NR);
	__SetPageUptodate(new_page);

	spin_lock(&mm->page_table_lock);
	put_page(page);
	if (unlikely(!pmd_same(*pmd, orig_pmd))) {
		mem_cgroup_uncharge_page(new_page);
		put_page(new_page);
	} else {
		pmd_t entry;
		VM_BUG_ON(!PageHead(page));
		entry = mk_pmd(new_page, vma->vm_page_prot);
		entry = maybe_pmd_mkwrite(pmd_mkdirty(entry), vma);
		entry = pmd_mkhuge(entry);
		pmdp_clear_flush_notify(vma, haddr, pmd);
		page_add_new_anon_rmap(new_page, vma, haddr);
		set_pmd_at(mm, haddr, pmd, entry);
		update_mmu_cache(vma, address, entry);
		page_remove_rmap(page);
		put_page(page);
		ret |= VM_FAULT_WRITE;
	}
out_unlock:
	spin_unlock(&mm->page_table_lock);
out:
	return ret;
}

struct page *follow_trans_huge_pmd(struct mm_struct *mm,
=======
#ifdef CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD
static void touch_pud(struct vm_area_struct *vma, unsigned long addr,
		      pud_t *pud, bool write)
{
	pud_t _pud;

	_pud = pud_mkyoung(*pud);
	if (write)
		_pud = pud_mkdirty(_pud);
	if (pudp_set_access_flags(vma, addr & HPAGE_PUD_MASK,
				  pud, _pud, write))
		update_mmu_cache_pud(vma, addr, pud);
}

struct page *follow_devmap_pud(struct vm_area_struct *vma, unsigned long addr,
		pud_t *pud, int flags, struct dev_pagemap **pgmap)
{
	unsigned long pfn = pud_pfn(*pud);
	struct mm_struct *mm = vma->vm_mm;
	struct page *page;
	int ret;

	assert_spin_locked(pud_lockptr(mm, pud));

	if (flags & FOLL_WRITE && !pud_write(*pud))
		return NULL;

	if (pud_present(*pud) && pud_devmap(*pud))
		/* pass */;
	else
		return NULL;

	if (flags & FOLL_TOUCH)
		touch_pud(vma, addr, pud, flags & FOLL_WRITE);

	/*
	 * device mapped pages can only be returned if the
	 * caller will manage the page reference count.
	 *
	 * At least one of FOLL_GET | FOLL_PIN must be set, so assert that here:
	 */
	if (!(flags & (FOLL_GET | FOLL_PIN)))
		return ERR_PTR(-EEXIST);

	pfn += (addr & ~PUD_MASK) >> PAGE_SHIFT;
	*pgmap = get_dev_pagemap(pfn, *pgmap);
	if (!*pgmap)
		return ERR_PTR(-EFAULT);
	page = pfn_to_page(pfn);

	ret = try_grab_page(page, flags);
	if (ret)
		page = ERR_PTR(ret);

	return page;
}

int copy_huge_pud(struct mm_struct *dst_mm, struct mm_struct *src_mm,
		  pud_t *dst_pud, pud_t *src_pud, unsigned long addr,
		  struct vm_area_struct *vma)
{
	spinlock_t *dst_ptl, *src_ptl;
	pud_t pud;
	int ret;

	dst_ptl = pud_lock(dst_mm, dst_pud);
	src_ptl = pud_lockptr(src_mm, src_pud);
	spin_lock_nested(src_ptl, SINGLE_DEPTH_NESTING);

	ret = -EAGAIN;
	pud = *src_pud;
	if (unlikely(!pud_trans_huge(pud) && !pud_devmap(pud)))
		goto out_unlock;

	/*
	 * When page table lock is held, the huge zero pud should not be
	 * under splitting since we don't split the page itself, only pud to
	 * a page table.
	 */
	if (is_huge_zero_pud(pud)) {
		/* No huge zero pud yet */
	}

	/*
	 * TODO: once we support anonymous pages, use
	 * folio_try_dup_anon_rmap_*() and split if duplicating fails.
	 */
	pudp_set_wrprotect(src_mm, addr, src_pud);
	pud = pud_mkold(pud_wrprotect(pud));
	set_pud_at(dst_mm, addr, dst_pud, pud);

	ret = 0;
out_unlock:
	spin_unlock(src_ptl);
	spin_unlock(dst_ptl);
	return ret;
}

void huge_pud_set_accessed(struct vm_fault *vmf, pud_t orig_pud)
{
	bool write = vmf->flags & FAULT_FLAG_WRITE;

	vmf->ptl = pud_lock(vmf->vma->vm_mm, vmf->pud);
	if (unlikely(!pud_same(*vmf->pud, orig_pud)))
		goto unlock;

	touch_pud(vmf->vma, vmf->address, vmf->pud, write);
unlock:
	spin_unlock(vmf->ptl);
}
#endif /* CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD */

void huge_pmd_set_accessed(struct vm_fault *vmf)
{
	bool write = vmf->flags & FAULT_FLAG_WRITE;

	vmf->ptl = pmd_lock(vmf->vma->vm_mm, vmf->pmd);
	if (unlikely(!pmd_same(*vmf->pmd, vmf->orig_pmd)))
		goto unlock;

	touch_pmd(vmf->vma, vmf->address, vmf->pmd, write);

unlock:
	spin_unlock(vmf->ptl);
}

vm_fault_t do_huge_pmd_wp_page(struct vm_fault *vmf)
{
	const bool unshare = vmf->flags & FAULT_FLAG_UNSHARE;
	struct vm_area_struct *vma = vmf->vma;
	struct folio *folio;
	struct page *page;
	unsigned long haddr = vmf->address & HPAGE_PMD_MASK;
	pmd_t orig_pmd = vmf->orig_pmd;

	vmf->ptl = pmd_lockptr(vma->vm_mm, vmf->pmd);
	VM_BUG_ON_VMA(!vma->anon_vma, vma);

	if (is_huge_zero_pmd(orig_pmd))
		goto fallback;

	spin_lock(vmf->ptl);

	if (unlikely(!pmd_same(*vmf->pmd, orig_pmd))) {
		spin_unlock(vmf->ptl);
		return 0;
	}

	page = pmd_page(orig_pmd);
	folio = page_folio(page);
	VM_BUG_ON_PAGE(!PageHead(page), page);

	/* Early check when only holding the PT lock. */
	if (PageAnonExclusive(page))
		goto reuse;

	if (!folio_trylock(folio)) {
		folio_get(folio);
		spin_unlock(vmf->ptl);
		folio_lock(folio);
		spin_lock(vmf->ptl);
		if (unlikely(!pmd_same(*vmf->pmd, orig_pmd))) {
			spin_unlock(vmf->ptl);
			folio_unlock(folio);
			folio_put(folio);
			return 0;
		}
		folio_put(folio);
	}

	/* Recheck after temporarily dropping the PT lock. */
	if (PageAnonExclusive(page)) {
		folio_unlock(folio);
		goto reuse;
	}

	/*
	 * See do_wp_page(): we can only reuse the folio exclusively if
	 * there are no additional references. Note that we always drain
	 * the LRU cache immediately after adding a THP.
	 */
	if (folio_ref_count(folio) >
			1 + folio_test_swapcache(folio) * folio_nr_pages(folio))
		goto unlock_fallback;
	if (folio_test_swapcache(folio))
		folio_free_swap(folio);
	if (folio_ref_count(folio) == 1) {
		pmd_t entry;

		folio_move_anon_rmap(folio, vma);
		SetPageAnonExclusive(page);
		folio_unlock(folio);
reuse:
		if (unlikely(unshare)) {
			spin_unlock(vmf->ptl);
			return 0;
		}
		entry = pmd_mkyoung(orig_pmd);
		entry = maybe_pmd_mkwrite(pmd_mkdirty(entry), vma);
		if (pmdp_set_access_flags(vma, haddr, vmf->pmd, entry, 1))
			update_mmu_cache_pmd(vma, vmf->address, vmf->pmd);
		spin_unlock(vmf->ptl);
		return 0;
	}

unlock_fallback:
	folio_unlock(folio);
	spin_unlock(vmf->ptl);
fallback:
	__split_huge_pmd(vma, vmf->pmd, vmf->address, false, NULL);
	return VM_FAULT_FALLBACK;
}

static inline bool can_change_pmd_writable(struct vm_area_struct *vma,
					   unsigned long addr, pmd_t pmd)
{
	struct page *page;

	if (WARN_ON_ONCE(!(vma->vm_flags & VM_WRITE)))
		return false;

	/* Don't touch entries that are not even readable (NUMA hinting). */
	if (pmd_protnone(pmd))
		return false;

	/* Do we need write faults for softdirty tracking? */
	if (vma_soft_dirty_enabled(vma) && !pmd_soft_dirty(pmd))
		return false;

	/* Do we need write faults for uffd-wp tracking? */
	if (userfaultfd_huge_pmd_wp(vma, pmd))
		return false;

	if (!(vma->vm_flags & VM_SHARED)) {
		/* See can_change_pte_writable(). */
		page = vm_normal_page_pmd(vma, addr, pmd);
		return page && PageAnon(page) && PageAnonExclusive(page);
	}

	/* See can_change_pte_writable(). */
	return pmd_dirty(pmd);
}

/* FOLL_FORCE can write to even unwritable PMDs in COW mappings. */
static inline bool can_follow_write_pmd(pmd_t pmd, struct page *page,
					struct vm_area_struct *vma,
					unsigned int flags)
{
	/* If the pmd is writable, we can write to the page. */
	if (pmd_write(pmd))
		return true;

	/* Maybe FOLL_FORCE is set to override it? */
	if (!(flags & FOLL_FORCE))
		return false;

	/* But FOLL_FORCE has no effect on shared mappings */
	if (vma->vm_flags & (VM_MAYSHARE | VM_SHARED))
		return false;

	/* ... or read-only private ones */
	if (!(vma->vm_flags & VM_MAYWRITE))
		return false;

	/* ... or already writable ones that just need to take a write fault */
	if (vma->vm_flags & VM_WRITE)
		return false;

	/*
	 * See can_change_pte_writable(): we broke COW and could map the page
	 * writable if we have an exclusive anonymous page ...
	 */
	if (!page || !PageAnon(page) || !PageAnonExclusive(page))
		return false;

	/* ... and a write-fault isn't required for other reasons. */
	if (vma_soft_dirty_enabled(vma) && !pmd_soft_dirty(pmd))
		return false;
	return !userfaultfd_huge_pmd_wp(vma, pmd);
}

struct page *follow_trans_huge_pmd(struct vm_area_struct *vma,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   unsigned long addr,
				   pmd_t *pmd,
				   unsigned int flags)
{
<<<<<<< HEAD
	struct page *page = NULL;

	assert_spin_locked(&mm->page_table_lock);

	if (flags & FOLL_WRITE && !pmd_write(*pmd))
		goto out;

	page = pmd_page(*pmd);
	VM_BUG_ON(!PageHead(page));
	if (flags & FOLL_TOUCH) {
		pmd_t _pmd;
		/*
		 * We should set the dirty bit only for FOLL_WRITE but
		 * for now the dirty bit in the pmd is meaningless.
		 * And if the dirty bit will become meaningful and
		 * we'll only set it with FOLL_WRITE, an atomic
		 * set_bit will be required on the pmd to set the
		 * young bit, instead of the current set_pmd_at.
		 */
		_pmd = pmd_mkyoung(pmd_mkdirty(*pmd));
		set_pmd_at(mm, addr & HPAGE_PMD_MASK, pmd, _pmd);
	}
	page += (addr & ~HPAGE_PMD_MASK) >> PAGE_SHIFT;
	VM_BUG_ON(!PageCompound(page));
	if (flags & FOLL_GET)
		get_page_foll(page);

out:
	return page;
=======
	struct mm_struct *mm = vma->vm_mm;
	struct page *page;
	int ret;

	assert_spin_locked(pmd_lockptr(mm, pmd));

	page = pmd_page(*pmd);
	VM_BUG_ON_PAGE(!PageHead(page) && !is_zone_device_page(page), page);

	if ((flags & FOLL_WRITE) &&
	    !can_follow_write_pmd(*pmd, page, vma, flags))
		return NULL;

	/* Avoid dumping huge zero page */
	if ((flags & FOLL_DUMP) && is_huge_zero_pmd(*pmd))
		return ERR_PTR(-EFAULT);

	if (pmd_protnone(*pmd) && !gup_can_follow_protnone(vma, flags))
		return NULL;

	if (!pmd_write(*pmd) && gup_must_unshare(vma, flags, page))
		return ERR_PTR(-EMLINK);

	VM_BUG_ON_PAGE((flags & FOLL_PIN) && PageAnon(page) &&
			!PageAnonExclusive(page), page);

	ret = try_grab_page(page, flags);
	if (ret)
		return ERR_PTR(ret);

	if (flags & FOLL_TOUCH)
		touch_pmd(vma, addr, pmd, flags & FOLL_WRITE);

	page += (addr & ~HPAGE_PMD_MASK) >> PAGE_SHIFT;
	VM_BUG_ON_PAGE(!PageCompound(page) && !is_zone_device_page(page), page);

	return page;
}

/* NUMA hinting page fault entry point for trans huge pmds */
vm_fault_t do_huge_pmd_numa_page(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	pmd_t oldpmd = vmf->orig_pmd;
	pmd_t pmd;
	struct folio *folio;
	unsigned long haddr = vmf->address & HPAGE_PMD_MASK;
	int nid = NUMA_NO_NODE;
	int target_nid, last_cpupid = (-1 & LAST_CPUPID_MASK);
	bool migrated = false, writable = false;
	int flags = 0;

	vmf->ptl = pmd_lock(vma->vm_mm, vmf->pmd);
	if (unlikely(!pmd_same(oldpmd, *vmf->pmd))) {
		spin_unlock(vmf->ptl);
		goto out;
	}

	pmd = pmd_modify(oldpmd, vma->vm_page_prot);

	/*
	 * Detect now whether the PMD could be writable; this information
	 * is only valid while holding the PT lock.
	 */
	writable = pmd_write(pmd);
	if (!writable && vma_wants_manual_pte_write_upgrade(vma) &&
	    can_change_pmd_writable(vma, vmf->address, pmd))
		writable = true;

	folio = vm_normal_folio_pmd(vma, haddr, pmd);
	if (!folio)
		goto out_map;

	/* See similar comment in do_numa_page for explanation */
	if (!writable)
		flags |= TNF_NO_GROUP;

	nid = folio_nid(folio);
	/*
	 * For memory tiering mode, cpupid of slow memory page is used
	 * to record page access time.  So use default value.
	 */
	if (node_is_toptier(nid))
		last_cpupid = folio_last_cpupid(folio);
	target_nid = numa_migrate_prep(folio, vma, haddr, nid, &flags);
	if (target_nid == NUMA_NO_NODE) {
		folio_put(folio);
		goto out_map;
	}

	spin_unlock(vmf->ptl);
	writable = false;

	migrated = migrate_misplaced_folio(folio, vma, target_nid);
	if (migrated) {
		flags |= TNF_MIGRATED;
		nid = target_nid;
	} else {
		flags |= TNF_MIGRATE_FAIL;
		vmf->ptl = pmd_lock(vma->vm_mm, vmf->pmd);
		if (unlikely(!pmd_same(oldpmd, *vmf->pmd))) {
			spin_unlock(vmf->ptl);
			goto out;
		}
		goto out_map;
	}

out:
	if (nid != NUMA_NO_NODE)
		task_numa_fault(last_cpupid, nid, HPAGE_PMD_NR, flags);

	return 0;

out_map:
	/* Restore the PMD */
	pmd = pmd_modify(oldpmd, vma->vm_page_prot);
	pmd = pmd_mkyoung(pmd);
	if (writable)
		pmd = pmd_mkwrite(pmd, vma);
	set_pmd_at(vma->vm_mm, haddr, vmf->pmd, pmd);
	update_mmu_cache_pmd(vma, vmf->address, vmf->pmd);
	spin_unlock(vmf->ptl);
	goto out;
}

/*
 * Return true if we do MADV_FREE successfully on entire pmd page.
 * Otherwise, return false.
 */
bool madvise_free_huge_pmd(struct mmu_gather *tlb, struct vm_area_struct *vma,
		pmd_t *pmd, unsigned long addr, unsigned long next)
{
	spinlock_t *ptl;
	pmd_t orig_pmd;
	struct folio *folio;
	struct mm_struct *mm = tlb->mm;
	bool ret = false;

	tlb_change_page_size(tlb, HPAGE_PMD_SIZE);

	ptl = pmd_trans_huge_lock(pmd, vma);
	if (!ptl)
		goto out_unlocked;

	orig_pmd = *pmd;
	if (is_huge_zero_pmd(orig_pmd))
		goto out;

	if (unlikely(!pmd_present(orig_pmd))) {
		VM_BUG_ON(thp_migration_supported() &&
				  !is_pmd_migration_entry(orig_pmd));
		goto out;
	}

	folio = pfn_folio(pmd_pfn(orig_pmd));
	/*
	 * If other processes are mapping this folio, we couldn't discard
	 * the folio unless they all do MADV_FREE so let's skip the folio.
	 */
	if (folio_estimated_sharers(folio) != 1)
		goto out;

	if (!folio_trylock(folio))
		goto out;

	/*
	 * If user want to discard part-pages of THP, split it so MADV_FREE
	 * will deactivate only them.
	 */
	if (next - addr != HPAGE_PMD_SIZE) {
		folio_get(folio);
		spin_unlock(ptl);
		split_folio(folio);
		folio_unlock(folio);
		folio_put(folio);
		goto out_unlocked;
	}

	if (folio_test_dirty(folio))
		folio_clear_dirty(folio);
	folio_unlock(folio);

	if (pmd_young(orig_pmd) || pmd_dirty(orig_pmd)) {
		pmdp_invalidate(vma, addr, pmd);
		orig_pmd = pmd_mkold(orig_pmd);
		orig_pmd = pmd_mkclean(orig_pmd);

		set_pmd_at(mm, addr, pmd, orig_pmd);
		tlb_remove_pmd_tlb_entry(tlb, pmd, addr);
	}

	folio_mark_lazyfree(folio);
	ret = true;
out:
	spin_unlock(ptl);
out_unlocked:
	return ret;
}

static inline void zap_deposited_table(struct mm_struct *mm, pmd_t *pmd)
{
	pgtable_t pgtable;

	pgtable = pgtable_trans_huge_withdraw(mm, pmd);
	pte_free(mm, pgtable);
	mm_dec_nr_ptes(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int zap_huge_pmd(struct mmu_gather *tlb, struct vm_area_struct *vma,
		 pmd_t *pmd, unsigned long addr)
{
<<<<<<< HEAD
	int ret = 0;

	if (__pmd_trans_huge_lock(pmd, vma) == 1) {
		struct page *page;
		pgtable_t pgtable;
		pgtable = get_pmd_huge_pte(tlb->mm);
		page = pmd_page(*pmd);
		pmd_clear(pmd);
		tlb_remove_pmd_tlb_entry(tlb, pmd, addr);
		page_remove_rmap(page);
		VM_BUG_ON(page_mapcount(page) < 0);
		add_mm_counter(tlb->mm, MM_ANONPAGES, -HPAGE_PMD_NR);
		VM_BUG_ON(!PageHead(page));
		tlb->mm->nr_ptes--;
		spin_unlock(&tlb->mm->page_table_lock);
		tlb_remove_page(tlb, page);
		pte_free(tlb->mm, pgtable);
		ret = 1;
	}
	return ret;
}

int mincore_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
		unsigned long addr, unsigned long end,
		unsigned char *vec)
{
	int ret = 0;

	if (__pmd_trans_huge_lock(pmd, vma) == 1) {
		/*
		 * All logical pages in the range are present
		 * if backed by a huge page.
		 */
		spin_unlock(&vma->vm_mm->page_table_lock);
		memset(vec, 1, (end - addr) >> PAGE_SHIFT);
		ret = 1;
	}

	return ret;
}

int move_huge_pmd(struct vm_area_struct *vma, struct vm_area_struct *new_vma,
		  unsigned long old_addr,
		  unsigned long new_addr, unsigned long old_end,
		  pmd_t *old_pmd, pmd_t *new_pmd)
{
	int ret = 0;
	pmd_t pmd;

	struct mm_struct *mm = vma->vm_mm;

	if ((old_addr & ~HPAGE_PMD_MASK) ||
	    (new_addr & ~HPAGE_PMD_MASK) ||
	    old_end - old_addr < HPAGE_PMD_SIZE ||
	    (new_vma->vm_flags & VM_NOHUGEPAGE))
		goto out;

	/*
	 * The destination pmd shouldn't be established, free_pgtables()
	 * should have release it.
	 */
	if (WARN_ON(!pmd_none(*new_pmd))) {
		VM_BUG_ON(pmd_trans_huge(*new_pmd));
		goto out;
	}

	ret = __pmd_trans_huge_lock(old_pmd, vma);
	if (ret == 1) {
		pmd = pmdp_get_and_clear(mm, old_addr, old_pmd);
		VM_BUG_ON(!pmd_none(*new_pmd));
		set_pmd_at(mm, new_addr, new_pmd, pmd);
		spin_unlock(&mm->page_table_lock);
	}
out:
	return ret;
}

int change_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
		unsigned long addr, pgprot_t newprot)
{
	struct mm_struct *mm = vma->vm_mm;
	int ret = 0;

	if (__pmd_trans_huge_lock(pmd, vma) == 1) {
		pmd_t entry;
		entry = pmdp_get_and_clear(mm, addr, pmd);
		entry = pmd_modify(entry, newprot);
		set_pmd_at(mm, addr, pmd, entry);
		spin_unlock(&vma->vm_mm->page_table_lock);
		ret = 1;
	}

	return ret;
}

/*
 * Returns 1 if a given pmd maps a stable (not under splitting) thp.
 * Returns -1 if it maps a thp under splitting. Returns 0 otherwise.
 *
 * Note that if it returns 1, this routine returns without unlocking page
 * table locks. So callers must unlock them.
 */
int __pmd_trans_huge_lock(pmd_t *pmd, struct vm_area_struct *vma)
{
	spin_lock(&vma->vm_mm->page_table_lock);
	if (likely(pmd_trans_huge(*pmd))) {
		if (unlikely(pmd_trans_splitting(*pmd))) {
			spin_unlock(&vma->vm_mm->page_table_lock);
			wait_split_huge_page(vma->anon_vma, pmd);
			return -1;
		} else {
			/* Thp mapped by 'pmd' is stable, so we can
			 * handle it as it is. */
			return 1;
		}
	}
	spin_unlock(&vma->vm_mm->page_table_lock);
	return 0;
}

pmd_t *page_check_address_pmd(struct page *page,
			      struct mm_struct *mm,
			      unsigned long address,
			      enum page_check_address_pmd_flag flag)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd, *ret = NULL;

	if (address & ~HPAGE_PMD_MASK)
		goto out;

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		goto out;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		goto out;

	pmd = pmd_offset(pud, address);
	if (pmd_none(*pmd))
		goto out;
	if (pmd_page(*pmd) != page)
		goto out;
	/*
	 * split_vma() may create temporary aliased mappings. There is
	 * no risk as long as all huge pmd are found and have their
	 * splitting bit set before __split_huge_page_refcount
	 * runs. Finding the same huge pmd more than once during the
	 * same rmap walk is not a problem.
	 */
	if (flag == PAGE_CHECK_ADDRESS_PMD_NOTSPLITTING_FLAG &&
	    pmd_trans_splitting(*pmd))
		goto out;
	if (pmd_trans_huge(*pmd)) {
		VM_BUG_ON(flag == PAGE_CHECK_ADDRESS_PMD_SPLITTING_FLAG &&
			  !pmd_trans_splitting(*pmd));
		ret = pmd;
	}
out:
	return ret;
}

static int __split_huge_page_splitting(struct page *page,
				       struct vm_area_struct *vma,
				       unsigned long address)
{
	struct mm_struct *mm = vma->vm_mm;
	pmd_t *pmd;
	int ret = 0;

	spin_lock(&mm->page_table_lock);
	pmd = page_check_address_pmd(page, mm, address,
				     PAGE_CHECK_ADDRESS_PMD_NOTSPLITTING_FLAG);
	if (pmd) {
		/*
		 * We can't temporarily set the pmd to null in order
		 * to split it, the pmd must remain marked huge at all
		 * times or the VM won't take the pmd_trans_huge paths
		 * and it won't wait on the anon_vma->root->mutex to
		 * serialize against split_huge_page*.
		 */
		pmdp_splitting_flush_notify(vma, address, pmd);
		ret = 1;
	}
	spin_unlock(&mm->page_table_lock);

	return ret;
}

static void __split_huge_page_refcount(struct page *page)
{
	int i;
	struct zone *zone = page_zone(page);
	int tail_count = 0;

	/* prevent PageLRU to go away from under us, and freeze lru stats */
	spin_lock_irq(&zone->lru_lock);
	compound_lock(page);
	/* complete memcg works before add pages to LRU */
	mem_cgroup_split_huge_fixup(page);

	for (i = HPAGE_PMD_NR - 1; i >= 1; i--) {
		struct page *page_tail = page + i;

		/* tail_page->_mapcount cannot change */
		BUG_ON(page_mapcount(page_tail) < 0);
		tail_count += page_mapcount(page_tail);
		/* check for overflow */
		BUG_ON(tail_count < 0);
		BUG_ON(atomic_read(&page_tail->_count) != 0);
		/*
		 * tail_page->_count is zero and not changing from
		 * under us. But get_page_unless_zero() may be running
		 * from under us on the tail_page. If we used
		 * atomic_set() below instead of atomic_add(), we
		 * would then run atomic_set() concurrently with
		 * get_page_unless_zero(), and atomic_set() is
		 * implemented in C not using locked ops. spin_unlock
		 * on x86 sometime uses locked ops because of PPro
		 * errata 66, 92, so unless somebody can guarantee
		 * atomic_set() here would be safe on all archs (and
		 * not only on x86), it's safer to use atomic_add().
		 */
		atomic_add(page_mapcount(page) + page_mapcount(page_tail) + 1,
			   &page_tail->_count);

		/* after clearing PageTail the gup refcount can be released */
		smp_mb();

		/*
		 * retain hwpoison flag of the poisoned tail page:
		 *   fix for the unsuitable process killed on Guest Machine(KVM)
		 *   by the memory-failure.
		 */
		page_tail->flags &= ~PAGE_FLAGS_CHECK_AT_PREP | __PG_HWPOISON;
		page_tail->flags |= (page->flags &
				     ((1L << PG_referenced) |
				      (1L << PG_swapbacked) |
				      (1L << PG_mlocked) |
				      (1L << PG_uptodate)));
		page_tail->flags |= (1L << PG_dirty);

		/* clear PageTail before overwriting first_page */
		smp_wmb();

		/*
		 * __split_huge_page_splitting() already set the
		 * splitting bit in all pmd that could map this
		 * hugepage, that will ensure no CPU can alter the
		 * mapcount on the head page. The mapcount is only
		 * accounted in the head page and it has to be
		 * transferred to all tail pages in the below code. So
		 * for this code to be safe, the split the mapcount
		 * can't change. But that doesn't mean userland can't
		 * keep changing and reading the page contents while
		 * we transfer the mapcount, so the pmd splitting
		 * status is achieved setting a reserved bit in the
		 * pmd, not by clearing the present bit.
		*/
		page_tail->_mapcount = page->_mapcount;

		BUG_ON(page_tail->mapping);
		page_tail->mapping = page->mapping;

		page_tail->index = page->index + i;

		BUG_ON(!PageAnon(page_tail));
		BUG_ON(!PageUptodate(page_tail));
		BUG_ON(!PageDirty(page_tail));
		BUG_ON(!PageSwapBacked(page_tail));


		lru_add_page_tail(zone, page, page_tail);
	}
	atomic_sub(tail_count, &page->_count);
	BUG_ON(atomic_read(&page->_count) <= 0);

	__dec_zone_page_state(page, NR_ANON_TRANSPARENT_HUGEPAGES);
	__mod_zone_page_state(zone, NR_ANON_PAGES, HPAGE_PMD_NR);

	ClearPageCompound(page);
	compound_unlock(page);
	spin_unlock_irq(&zone->lru_lock);

	for (i = 1; i < HPAGE_PMD_NR; i++) {
		struct page *page_tail = page + i;
		BUG_ON(page_count(page_tail) <= 0);
		/*
		 * Tail pages may be freed if there wasn't any mapping
=======
	pmd_t orig_pmd;
	spinlock_t *ptl;

	tlb_change_page_size(tlb, HPAGE_PMD_SIZE);

	ptl = __pmd_trans_huge_lock(pmd, vma);
	if (!ptl)
		return 0;
	/*
	 * For architectures like ppc64 we look at deposited pgtable
	 * when calling pmdp_huge_get_and_clear. So do the
	 * pgtable_trans_huge_withdraw after finishing pmdp related
	 * operations.
	 */
	orig_pmd = pmdp_huge_get_and_clear_full(vma, addr, pmd,
						tlb->fullmm);
	arch_check_zapped_pmd(vma, orig_pmd);
	tlb_remove_pmd_tlb_entry(tlb, pmd, addr);
	if (vma_is_special_huge(vma)) {
		if (arch_needs_pgtable_deposit())
			zap_deposited_table(tlb->mm, pmd);
		spin_unlock(ptl);
	} else if (is_huge_zero_pmd(orig_pmd)) {
		zap_deposited_table(tlb->mm, pmd);
		spin_unlock(ptl);
	} else {
		struct folio *folio = NULL;
		int flush_needed = 1;

		if (pmd_present(orig_pmd)) {
			struct page *page = pmd_page(orig_pmd);

			folio = page_folio(page);
			folio_remove_rmap_pmd(folio, page, vma);
			VM_BUG_ON_PAGE(page_mapcount(page) < 0, page);
			VM_BUG_ON_PAGE(!PageHead(page), page);
		} else if (thp_migration_supported()) {
			swp_entry_t entry;

			VM_BUG_ON(!is_pmd_migration_entry(orig_pmd));
			entry = pmd_to_swp_entry(orig_pmd);
			folio = pfn_swap_entry_folio(entry);
			flush_needed = 0;
		} else
			WARN_ONCE(1, "Non present huge pmd without pmd migration enabled!");

		if (folio_test_anon(folio)) {
			zap_deposited_table(tlb->mm, pmd);
			add_mm_counter(tlb->mm, MM_ANONPAGES, -HPAGE_PMD_NR);
		} else {
			if (arch_needs_pgtable_deposit())
				zap_deposited_table(tlb->mm, pmd);
			add_mm_counter(tlb->mm, mm_counter_file(folio),
				       -HPAGE_PMD_NR);
		}

		spin_unlock(ptl);
		if (flush_needed)
			tlb_remove_page_size(tlb, &folio->page, HPAGE_PMD_SIZE);
	}
	return 1;
}

#ifndef pmd_move_must_withdraw
static inline int pmd_move_must_withdraw(spinlock_t *new_pmd_ptl,
					 spinlock_t *old_pmd_ptl,
					 struct vm_area_struct *vma)
{
	/*
	 * With split pmd lock we also need to move preallocated
	 * PTE page table if new_pmd is on different PMD page table.
	 *
	 * We also don't deposit and withdraw tables for file pages.
	 */
	return (new_pmd_ptl != old_pmd_ptl) && vma_is_anonymous(vma);
}
#endif

static pmd_t move_soft_dirty_pmd(pmd_t pmd)
{
#ifdef CONFIG_MEM_SOFT_DIRTY
	if (unlikely(is_pmd_migration_entry(pmd)))
		pmd = pmd_swp_mksoft_dirty(pmd);
	else if (pmd_present(pmd))
		pmd = pmd_mksoft_dirty(pmd);
#endif
	return pmd;
}

bool move_huge_pmd(struct vm_area_struct *vma, unsigned long old_addr,
		  unsigned long new_addr, pmd_t *old_pmd, pmd_t *new_pmd)
{
	spinlock_t *old_ptl, *new_ptl;
	pmd_t pmd;
	struct mm_struct *mm = vma->vm_mm;
	bool force_flush = false;

	/*
	 * The destination pmd shouldn't be established, free_pgtables()
	 * should have released it; but move_page_tables() might have already
	 * inserted a page table, if racing against shmem/file collapse.
	 */
	if (!pmd_none(*new_pmd)) {
		VM_BUG_ON(pmd_trans_huge(*new_pmd));
		return false;
	}

	/*
	 * We don't have to worry about the ordering of src and dst
	 * ptlocks because exclusive mmap_lock prevents deadlock.
	 */
	old_ptl = __pmd_trans_huge_lock(old_pmd, vma);
	if (old_ptl) {
		new_ptl = pmd_lockptr(mm, new_pmd);
		if (new_ptl != old_ptl)
			spin_lock_nested(new_ptl, SINGLE_DEPTH_NESTING);
		pmd = pmdp_huge_get_and_clear(mm, old_addr, old_pmd);
		if (pmd_present(pmd))
			force_flush = true;
		VM_BUG_ON(!pmd_none(*new_pmd));

		if (pmd_move_must_withdraw(new_ptl, old_ptl, vma)) {
			pgtable_t pgtable;
			pgtable = pgtable_trans_huge_withdraw(mm, old_pmd);
			pgtable_trans_huge_deposit(mm, new_pmd, pgtable);
		}
		pmd = move_soft_dirty_pmd(pmd);
		set_pmd_at(mm, new_addr, new_pmd, pmd);
		if (force_flush)
			flush_pmd_tlb_range(vma, old_addr, old_addr + PMD_SIZE);
		if (new_ptl != old_ptl)
			spin_unlock(new_ptl);
		spin_unlock(old_ptl);
		return true;
	}
	return false;
}

/*
 * Returns
 *  - 0 if PMD could not be locked
 *  - 1 if PMD was locked but protections unchanged and TLB flush unnecessary
 *      or if prot_numa but THP migration is not supported
 *  - HPAGE_PMD_NR if protections changed and TLB flush necessary
 */
int change_huge_pmd(struct mmu_gather *tlb, struct vm_area_struct *vma,
		    pmd_t *pmd, unsigned long addr, pgprot_t newprot,
		    unsigned long cp_flags)
{
	struct mm_struct *mm = vma->vm_mm;
	spinlock_t *ptl;
	pmd_t oldpmd, entry;
	bool prot_numa = cp_flags & MM_CP_PROT_NUMA;
	bool uffd_wp = cp_flags & MM_CP_UFFD_WP;
	bool uffd_wp_resolve = cp_flags & MM_CP_UFFD_WP_RESOLVE;
	int ret = 1;

	tlb_change_page_size(tlb, HPAGE_PMD_SIZE);

	if (prot_numa && !thp_migration_supported())
		return 1;

	ptl = __pmd_trans_huge_lock(pmd, vma);
	if (!ptl)
		return 0;

#ifdef CONFIG_ARCH_ENABLE_THP_MIGRATION
	if (is_swap_pmd(*pmd)) {
		swp_entry_t entry = pmd_to_swp_entry(*pmd);
		struct folio *folio = pfn_swap_entry_folio(entry);
		pmd_t newpmd;

		VM_BUG_ON(!is_pmd_migration_entry(*pmd));
		if (is_writable_migration_entry(entry)) {
			/*
			 * A protection check is difficult so
			 * just be safe and disable write
			 */
			if (folio_test_anon(folio))
				entry = make_readable_exclusive_migration_entry(swp_offset(entry));
			else
				entry = make_readable_migration_entry(swp_offset(entry));
			newpmd = swp_entry_to_pmd(entry);
			if (pmd_swp_soft_dirty(*pmd))
				newpmd = pmd_swp_mksoft_dirty(newpmd);
		} else {
			newpmd = *pmd;
		}

		if (uffd_wp)
			newpmd = pmd_swp_mkuffd_wp(newpmd);
		else if (uffd_wp_resolve)
			newpmd = pmd_swp_clear_uffd_wp(newpmd);
		if (!pmd_same(*pmd, newpmd))
			set_pmd_at(mm, addr, pmd, newpmd);
		goto unlock;
	}
#endif

	if (prot_numa) {
		struct folio *folio;
		bool toptier;
		/*
		 * Avoid trapping faults against the zero page. The read-only
		 * data is likely to be read-cached on the local CPU and
		 * local/remote hits to the zero page are not interesting.
		 */
		if (is_huge_zero_pmd(*pmd))
			goto unlock;

		if (pmd_protnone(*pmd))
			goto unlock;

		folio = page_folio(pmd_page(*pmd));
		toptier = node_is_toptier(folio_nid(folio));
		/*
		 * Skip scanning top tier node if normal numa
		 * balancing is disabled
		 */
		if (!(sysctl_numa_balancing_mode & NUMA_BALANCING_NORMAL) &&
		    toptier)
			goto unlock;

		if (sysctl_numa_balancing_mode & NUMA_BALANCING_MEMORY_TIERING &&
		    !toptier)
			folio_xchg_access_time(folio,
					       jiffies_to_msecs(jiffies));
	}
	/*
	 * In case prot_numa, we are under mmap_read_lock(mm). It's critical
	 * to not clear pmd intermittently to avoid race with MADV_DONTNEED
	 * which is also under mmap_read_lock(mm):
	 *
	 *	CPU0:				CPU1:
	 *				change_huge_pmd(prot_numa=1)
	 *				 pmdp_huge_get_and_clear_notify()
	 * madvise_dontneed()
	 *  zap_pmd_range()
	 *   pmd_trans_huge(*pmd) == 0 (without ptl)
	 *   // skip the pmd
	 *				 set_pmd_at();
	 *				 // pmd is re-established
	 *
	 * The race makes MADV_DONTNEED miss the huge pmd and don't clear it
	 * which may break userspace.
	 *
	 * pmdp_invalidate_ad() is required to make sure we don't miss
	 * dirty/young flags set by hardware.
	 */
	oldpmd = pmdp_invalidate_ad(vma, addr, pmd);

	entry = pmd_modify(oldpmd, newprot);
	if (uffd_wp)
		entry = pmd_mkuffd_wp(entry);
	else if (uffd_wp_resolve)
		/*
		 * Leave the write bit to be handled by PF interrupt
		 * handler, then things like COW could be properly
		 * handled.
		 */
		entry = pmd_clear_uffd_wp(entry);

	/* See change_pte_range(). */
	if ((cp_flags & MM_CP_TRY_CHANGE_WRITABLE) && !pmd_write(entry) &&
	    can_change_pmd_writable(vma, addr, entry))
		entry = pmd_mkwrite(entry, vma);

	ret = HPAGE_PMD_NR;
	set_pmd_at(mm, addr, pmd, entry);

	if (huge_pmd_needs_flush(oldpmd, entry))
		tlb_flush_pmd_range(tlb, addr, HPAGE_PMD_SIZE);
unlock:
	spin_unlock(ptl);
	return ret;
}

#ifdef CONFIG_USERFAULTFD
/*
 * The PT lock for src_pmd and dst_vma/src_vma (for reading) are locked by
 * the caller, but it must return after releasing the page_table_lock.
 * Just move the page from src_pmd to dst_pmd if possible.
 * Return zero if succeeded in moving the page, -EAGAIN if it needs to be
 * repeated by the caller, or other errors in case of failure.
 */
int move_pages_huge_pmd(struct mm_struct *mm, pmd_t *dst_pmd, pmd_t *src_pmd, pmd_t dst_pmdval,
			struct vm_area_struct *dst_vma, struct vm_area_struct *src_vma,
			unsigned long dst_addr, unsigned long src_addr)
{
	pmd_t _dst_pmd, src_pmdval;
	struct page *src_page;
	struct folio *src_folio;
	struct anon_vma *src_anon_vma;
	spinlock_t *src_ptl, *dst_ptl;
	pgtable_t src_pgtable;
	struct mmu_notifier_range range;
	int err = 0;

	src_pmdval = *src_pmd;
	src_ptl = pmd_lockptr(mm, src_pmd);

	lockdep_assert_held(src_ptl);
	vma_assert_locked(src_vma);
	vma_assert_locked(dst_vma);

	/* Sanity checks before the operation */
	if (WARN_ON_ONCE(!pmd_none(dst_pmdval)) || WARN_ON_ONCE(src_addr & ~HPAGE_PMD_MASK) ||
	    WARN_ON_ONCE(dst_addr & ~HPAGE_PMD_MASK)) {
		spin_unlock(src_ptl);
		return -EINVAL;
	}

	if (!pmd_trans_huge(src_pmdval)) {
		spin_unlock(src_ptl);
		if (is_pmd_migration_entry(src_pmdval)) {
			pmd_migration_entry_wait(mm, &src_pmdval);
			return -EAGAIN;
		}
		return -ENOENT;
	}

	src_page = pmd_page(src_pmdval);

	if (!is_huge_zero_pmd(src_pmdval)) {
		if (unlikely(!PageAnonExclusive(src_page))) {
			spin_unlock(src_ptl);
			return -EBUSY;
		}

		src_folio = page_folio(src_page);
		folio_get(src_folio);
	} else
		src_folio = NULL;

	spin_unlock(src_ptl);

	flush_cache_range(src_vma, src_addr, src_addr + HPAGE_PMD_SIZE);
	mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, mm, src_addr,
				src_addr + HPAGE_PMD_SIZE);
	mmu_notifier_invalidate_range_start(&range);

	if (src_folio) {
		folio_lock(src_folio);

		/*
		 * split_huge_page walks the anon_vma chain without the page
		 * lock. Serialize against it with the anon_vma lock, the page
		 * lock is not enough.
		 */
		src_anon_vma = folio_get_anon_vma(src_folio);
		if (!src_anon_vma) {
			err = -EAGAIN;
			goto unlock_folio;
		}
		anon_vma_lock_write(src_anon_vma);
	} else
		src_anon_vma = NULL;

	dst_ptl = pmd_lockptr(mm, dst_pmd);
	double_pt_lock(src_ptl, dst_ptl);
	if (unlikely(!pmd_same(*src_pmd, src_pmdval) ||
		     !pmd_same(*dst_pmd, dst_pmdval))) {
		err = -EAGAIN;
		goto unlock_ptls;
	}
	if (src_folio) {
		if (folio_maybe_dma_pinned(src_folio) ||
		    !PageAnonExclusive(&src_folio->page)) {
			err = -EBUSY;
			goto unlock_ptls;
		}

		if (WARN_ON_ONCE(!folio_test_head(src_folio)) ||
		    WARN_ON_ONCE(!folio_test_anon(src_folio))) {
			err = -EBUSY;
			goto unlock_ptls;
		}

		src_pmdval = pmdp_huge_clear_flush(src_vma, src_addr, src_pmd);
		/* Folio got pinned from under us. Put it back and fail the move. */
		if (folio_maybe_dma_pinned(src_folio)) {
			set_pmd_at(mm, src_addr, src_pmd, src_pmdval);
			err = -EBUSY;
			goto unlock_ptls;
		}

		folio_move_anon_rmap(src_folio, dst_vma);
		WRITE_ONCE(src_folio->index, linear_page_index(dst_vma, dst_addr));

		_dst_pmd = mk_huge_pmd(&src_folio->page, dst_vma->vm_page_prot);
		/* Follow mremap() behavior and treat the entry dirty after the move */
		_dst_pmd = pmd_mkwrite(pmd_mkdirty(_dst_pmd), dst_vma);
	} else {
		src_pmdval = pmdp_huge_clear_flush(src_vma, src_addr, src_pmd);
		_dst_pmd = mk_huge_pmd(src_page, dst_vma->vm_page_prot);
	}
	set_pmd_at(mm, dst_addr, dst_pmd, _dst_pmd);

	src_pgtable = pgtable_trans_huge_withdraw(mm, src_pmd);
	pgtable_trans_huge_deposit(mm, dst_pmd, src_pgtable);
unlock_ptls:
	double_pt_unlock(src_ptl, dst_ptl);
	if (src_anon_vma) {
		anon_vma_unlock_write(src_anon_vma);
		put_anon_vma(src_anon_vma);
	}
unlock_folio:
	/* unblock rmap walks */
	if (src_folio)
		folio_unlock(src_folio);
	mmu_notifier_invalidate_range_end(&range);
	if (src_folio)
		folio_put(src_folio);
	return err;
}
#endif /* CONFIG_USERFAULTFD */

/*
 * Returns page table lock pointer if a given pmd maps a thp, NULL otherwise.
 *
 * Note that if it returns page table lock pointer, this routine returns without
 * unlocking page table lock. So callers must unlock it.
 */
spinlock_t *__pmd_trans_huge_lock(pmd_t *pmd, struct vm_area_struct *vma)
{
	spinlock_t *ptl;
	ptl = pmd_lock(vma->vm_mm, pmd);
	if (likely(is_swap_pmd(*pmd) || pmd_trans_huge(*pmd) ||
			pmd_devmap(*pmd)))
		return ptl;
	spin_unlock(ptl);
	return NULL;
}

/*
 * Returns page table lock pointer if a given pud maps a thp, NULL otherwise.
 *
 * Note that if it returns page table lock pointer, this routine returns without
 * unlocking page table lock. So callers must unlock it.
 */
spinlock_t *__pud_trans_huge_lock(pud_t *pud, struct vm_area_struct *vma)
{
	spinlock_t *ptl;

	ptl = pud_lock(vma->vm_mm, pud);
	if (likely(pud_trans_huge(*pud) || pud_devmap(*pud)))
		return ptl;
	spin_unlock(ptl);
	return NULL;
}

#ifdef CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD
int zap_huge_pud(struct mmu_gather *tlb, struct vm_area_struct *vma,
		 pud_t *pud, unsigned long addr)
{
	spinlock_t *ptl;

	ptl = __pud_trans_huge_lock(pud, vma);
	if (!ptl)
		return 0;

	pudp_huge_get_and_clear_full(vma, addr, pud, tlb->fullmm);
	tlb_remove_pud_tlb_entry(tlb, pud, addr);
	if (vma_is_special_huge(vma)) {
		spin_unlock(ptl);
		/* No zero page support yet */
	} else {
		/* No support for anonymous PUD pages yet */
		BUG();
	}
	return 1;
}

static void __split_huge_pud_locked(struct vm_area_struct *vma, pud_t *pud,
		unsigned long haddr)
{
	VM_BUG_ON(haddr & ~HPAGE_PUD_MASK);
	VM_BUG_ON_VMA(vma->vm_start > haddr, vma);
	VM_BUG_ON_VMA(vma->vm_end < haddr + HPAGE_PUD_SIZE, vma);
	VM_BUG_ON(!pud_trans_huge(*pud) && !pud_devmap(*pud));

	count_vm_event(THP_SPLIT_PUD);

	pudp_huge_clear_flush(vma, haddr, pud);
}

void __split_huge_pud(struct vm_area_struct *vma, pud_t *pud,
		unsigned long address)
{
	spinlock_t *ptl;
	struct mmu_notifier_range range;

	mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, vma->vm_mm,
				address & HPAGE_PUD_MASK,
				(address & HPAGE_PUD_MASK) + HPAGE_PUD_SIZE);
	mmu_notifier_invalidate_range_start(&range);
	ptl = pud_lock(vma->vm_mm, pud);
	if (unlikely(!pud_trans_huge(*pud) && !pud_devmap(*pud)))
		goto out;
	__split_huge_pud_locked(vma, pud, range.start);

out:
	spin_unlock(ptl);
	mmu_notifier_invalidate_range_end(&range);
}
#endif /* CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD */

static void __split_huge_zero_page_pmd(struct vm_area_struct *vma,
		unsigned long haddr, pmd_t *pmd)
{
	struct mm_struct *mm = vma->vm_mm;
	pgtable_t pgtable;
	pmd_t _pmd, old_pmd;
	unsigned long addr;
	pte_t *pte;
	int i;

	/*
	 * Leave pmd empty until pte is filled note that it is fine to delay
	 * notification until mmu_notifier_invalidate_range_end() as we are
	 * replacing a zero pmd write protected page with a zero pte write
	 * protected page.
	 *
	 * See Documentation/mm/mmu_notifier.rst
	 */
	old_pmd = pmdp_huge_clear_flush(vma, haddr, pmd);

	pgtable = pgtable_trans_huge_withdraw(mm, pmd);
	pmd_populate(mm, &_pmd, pgtable);

	pte = pte_offset_map(&_pmd, haddr);
	VM_BUG_ON(!pte);
	for (i = 0, addr = haddr; i < HPAGE_PMD_NR; i++, addr += PAGE_SIZE) {
		pte_t entry;

		entry = pfn_pte(my_zero_pfn(addr), vma->vm_page_prot);
		entry = pte_mkspecial(entry);
		if (pmd_uffd_wp(old_pmd))
			entry = pte_mkuffd_wp(entry);
		VM_BUG_ON(!pte_none(ptep_get(pte)));
		set_pte_at(mm, addr, pte, entry);
		pte++;
	}
	pte_unmap(pte - 1);
	smp_wmb(); /* make pte visible before pmd */
	pmd_populate(mm, pmd, pgtable);
}

static void __split_huge_pmd_locked(struct vm_area_struct *vma, pmd_t *pmd,
		unsigned long haddr, bool freeze)
{
	struct mm_struct *mm = vma->vm_mm;
	struct folio *folio;
	struct page *page;
	pgtable_t pgtable;
	pmd_t old_pmd, _pmd;
	bool young, write, soft_dirty, pmd_migration = false, uffd_wp = false;
	bool anon_exclusive = false, dirty = false;
	unsigned long addr;
	pte_t *pte;
	int i;

	VM_BUG_ON(haddr & ~HPAGE_PMD_MASK);
	VM_BUG_ON_VMA(vma->vm_start > haddr, vma);
	VM_BUG_ON_VMA(vma->vm_end < haddr + HPAGE_PMD_SIZE, vma);
	VM_BUG_ON(!is_pmd_migration_entry(*pmd) && !pmd_trans_huge(*pmd)
				&& !pmd_devmap(*pmd));

	count_vm_event(THP_SPLIT_PMD);

	if (!vma_is_anonymous(vma)) {
		old_pmd = pmdp_huge_clear_flush(vma, haddr, pmd);
		/*
		 * We are going to unmap this huge page. So
		 * just go ahead and zap it
		 */
		if (arch_needs_pgtable_deposit())
			zap_deposited_table(mm, pmd);
		if (vma_is_special_huge(vma))
			return;
		if (unlikely(is_pmd_migration_entry(old_pmd))) {
			swp_entry_t entry;

			entry = pmd_to_swp_entry(old_pmd);
			folio = pfn_swap_entry_folio(entry);
		} else {
			page = pmd_page(old_pmd);
			folio = page_folio(page);
			if (!folio_test_dirty(folio) && pmd_dirty(old_pmd))
				folio_mark_dirty(folio);
			if (!folio_test_referenced(folio) && pmd_young(old_pmd))
				folio_set_referenced(folio);
			folio_remove_rmap_pmd(folio, page, vma);
			folio_put(folio);
		}
		add_mm_counter(mm, mm_counter_file(folio), -HPAGE_PMD_NR);
		return;
	}

	if (is_huge_zero_pmd(*pmd)) {
		/*
		 * FIXME: Do we want to invalidate secondary mmu by calling
		 * mmu_notifier_arch_invalidate_secondary_tlbs() see comments below
		 * inside __split_huge_pmd() ?
		 *
		 * We are going from a zero huge page write protected to zero
		 * small page also write protected so it does not seems useful
		 * to invalidate secondary mmu at this time.
		 */
		return __split_huge_zero_page_pmd(vma, haddr, pmd);
	}

	/*
	 * Up to this point the pmd is present and huge and userland has the
	 * whole access to the hugepage during the split (which happens in
	 * place). If we overwrite the pmd with the not-huge version pointing
	 * to the pte here (which of course we could if all CPUs were bug
	 * free), userland could trigger a small page size TLB miss on the
	 * small sized TLB while the hugepage TLB entry is still established in
	 * the huge TLB. Some CPU doesn't like that.
	 * See http://support.amd.com/TechDocs/41322_10h_Rev_Gd.pdf, Erratum
	 * 383 on page 105. Intel should be safe but is also warns that it's
	 * only safe if the permission and cache attributes of the two entries
	 * loaded in the two TLB is identical (which should be the case here).
	 * But it is generally safer to never allow small and huge TLB entries
	 * for the same virtual address to be loaded simultaneously. So instead
	 * of doing "pmd_populate(); flush_pmd_tlb_range();" we first mark the
	 * current pmd notpresent (atomically because here the pmd_trans_huge
	 * must remain set at all times on the pmd until the split is complete
	 * for this pmd), then we flush the SMP TLB and finally we write the
	 * non-huge version of the pmd entry with pmd_populate.
	 */
	old_pmd = pmdp_invalidate(vma, haddr, pmd);

	pmd_migration = is_pmd_migration_entry(old_pmd);
	if (unlikely(pmd_migration)) {
		swp_entry_t entry;

		entry = pmd_to_swp_entry(old_pmd);
		page = pfn_swap_entry_to_page(entry);
		write = is_writable_migration_entry(entry);
		if (PageAnon(page))
			anon_exclusive = is_readable_exclusive_migration_entry(entry);
		young = is_migration_entry_young(entry);
		dirty = is_migration_entry_dirty(entry);
		soft_dirty = pmd_swp_soft_dirty(old_pmd);
		uffd_wp = pmd_swp_uffd_wp(old_pmd);
	} else {
		page = pmd_page(old_pmd);
		folio = page_folio(page);
		if (pmd_dirty(old_pmd)) {
			dirty = true;
			folio_set_dirty(folio);
		}
		write = pmd_write(old_pmd);
		young = pmd_young(old_pmd);
		soft_dirty = pmd_soft_dirty(old_pmd);
		uffd_wp = pmd_uffd_wp(old_pmd);

		VM_WARN_ON_FOLIO(!folio_ref_count(folio), folio);
		VM_WARN_ON_FOLIO(!folio_test_anon(folio), folio);

		/*
		 * Without "freeze", we'll simply split the PMD, propagating the
		 * PageAnonExclusive() flag for each PTE by setting it for
		 * each subpage -- no need to (temporarily) clear.
		 *
		 * With "freeze" we want to replace mapped pages by
		 * migration entries right away. This is only possible if we
		 * managed to clear PageAnonExclusive() -- see
		 * set_pmd_migration_entry().
		 *
		 * In case we cannot clear PageAnonExclusive(), split the PMD
		 * only and let try_to_migrate_one() fail later.
		 *
		 * See folio_try_share_anon_rmap_pmd(): invalidate PMD first.
		 */
		anon_exclusive = PageAnonExclusive(page);
		if (freeze && anon_exclusive &&
		    folio_try_share_anon_rmap_pmd(folio, page))
			freeze = false;
		if (!freeze) {
			rmap_t rmap_flags = RMAP_NONE;

			folio_ref_add(folio, HPAGE_PMD_NR - 1);
			if (anon_exclusive)
				rmap_flags |= RMAP_EXCLUSIVE;
			folio_add_anon_rmap_ptes(folio, page, HPAGE_PMD_NR,
						 vma, haddr, rmap_flags);
		}
	}

	/*
	 * Withdraw the table only after we mark the pmd entry invalid.
	 * This's critical for some architectures (Power).
	 */
	pgtable = pgtable_trans_huge_withdraw(mm, pmd);
	pmd_populate(mm, &_pmd, pgtable);

	pte = pte_offset_map(&_pmd, haddr);
	VM_BUG_ON(!pte);

	/*
	 * Note that NUMA hinting access restrictions are not transferred to
	 * avoid any possibility of altering permissions across VMAs.
	 */
	if (freeze || pmd_migration) {
		for (i = 0, addr = haddr; i < HPAGE_PMD_NR; i++, addr += PAGE_SIZE) {
			pte_t entry;
			swp_entry_t swp_entry;

			if (write)
				swp_entry = make_writable_migration_entry(
							page_to_pfn(page + i));
			else if (anon_exclusive)
				swp_entry = make_readable_exclusive_migration_entry(
							page_to_pfn(page + i));
			else
				swp_entry = make_readable_migration_entry(
							page_to_pfn(page + i));
			if (young)
				swp_entry = make_migration_entry_young(swp_entry);
			if (dirty)
				swp_entry = make_migration_entry_dirty(swp_entry);
			entry = swp_entry_to_pte(swp_entry);
			if (soft_dirty)
				entry = pte_swp_mksoft_dirty(entry);
			if (uffd_wp)
				entry = pte_swp_mkuffd_wp(entry);

			VM_WARN_ON(!pte_none(ptep_get(pte + i)));
			set_pte_at(mm, addr, pte + i, entry);
		}
	} else {
		pte_t entry;

		entry = mk_pte(page, READ_ONCE(vma->vm_page_prot));
		if (write)
			entry = pte_mkwrite(entry, vma);
		if (!young)
			entry = pte_mkold(entry);
		/* NOTE: this may set soft-dirty too on some archs */
		if (dirty)
			entry = pte_mkdirty(entry);
		if (soft_dirty)
			entry = pte_mksoft_dirty(entry);
		if (uffd_wp)
			entry = pte_mkuffd_wp(entry);

		for (i = 0; i < HPAGE_PMD_NR; i++)
			VM_WARN_ON(!pte_none(ptep_get(pte + i)));

		set_ptes(mm, haddr, pte, entry, HPAGE_PMD_NR);
	}
	pte_unmap(pte);

	if (!pmd_migration)
		folio_remove_rmap_pmd(folio, page, vma);
	if (freeze)
		put_page(page);

	smp_wmb(); /* make pte visible before pmd */
	pmd_populate(mm, pmd, pgtable);
}

void __split_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
		unsigned long address, bool freeze, struct folio *folio)
{
	spinlock_t *ptl;
	struct mmu_notifier_range range;

	mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, vma->vm_mm,
				address & HPAGE_PMD_MASK,
				(address & HPAGE_PMD_MASK) + HPAGE_PMD_SIZE);
	mmu_notifier_invalidate_range_start(&range);
	ptl = pmd_lock(vma->vm_mm, pmd);

	/*
	 * If caller asks to setup a migration entry, we need a folio to check
	 * pmd against. Otherwise we can end up replacing wrong folio.
	 */
	VM_BUG_ON(freeze && !folio);
	VM_WARN_ON_ONCE(folio && !folio_test_locked(folio));

	if (pmd_trans_huge(*pmd) || pmd_devmap(*pmd) ||
	    is_pmd_migration_entry(*pmd)) {
		/*
		 * It's safe to call pmd_page when folio is set because it's
		 * guaranteed that pmd is present.
		 */
		if (folio && folio != page_folio(pmd_page(*pmd)))
			goto out;
		__split_huge_pmd_locked(vma, pmd, range.start, freeze);
	}

out:
	spin_unlock(ptl);
	mmu_notifier_invalidate_range_end(&range);
}

void split_huge_pmd_address(struct vm_area_struct *vma, unsigned long address,
		bool freeze, struct folio *folio)
{
	pmd_t *pmd = mm_find_pmd(vma->vm_mm, address);

	if (!pmd)
		return;

	__split_huge_pmd(vma, pmd, address, freeze, folio);
}

static inline void split_huge_pmd_if_needed(struct vm_area_struct *vma, unsigned long address)
{
	/*
	 * If the new address isn't hpage aligned and it could previously
	 * contain an hugepage: check if we need to split an huge pmd.
	 */
	if (!IS_ALIGNED(address, HPAGE_PMD_SIZE) &&
	    range_in_vma(vma, ALIGN_DOWN(address, HPAGE_PMD_SIZE),
			 ALIGN(address, HPAGE_PMD_SIZE)))
		split_huge_pmd_address(vma, address, false, NULL);
}

void vma_adjust_trans_huge(struct vm_area_struct *vma,
			     unsigned long start,
			     unsigned long end,
			     long adjust_next)
{
	/* Check if we need to split start first. */
	split_huge_pmd_if_needed(vma, start);

	/* Check if we need to split end next. */
	split_huge_pmd_if_needed(vma, end);

	/*
	 * If we're also updating the next vma vm_start,
	 * check if we need to split it.
	 */
	if (adjust_next > 0) {
		struct vm_area_struct *next = find_vma(vma->vm_mm, vma->vm_end);
		unsigned long nstart = next->vm_start;
		nstart += adjust_next;
		split_huge_pmd_if_needed(next, nstart);
	}
}

static void unmap_folio(struct folio *folio)
{
	enum ttu_flags ttu_flags = TTU_RMAP_LOCKED | TTU_SYNC |
		TTU_BATCH_FLUSH;

	VM_BUG_ON_FOLIO(!folio_test_large(folio), folio);

	if (folio_test_pmd_mappable(folio))
		ttu_flags |= TTU_SPLIT_HUGE_PMD;

	/*
	 * Anon pages need migration entries to preserve them, but file
	 * pages can simply be left unmapped, then faulted back on demand.
	 * If that is ever changed (perhaps for mlock), update remap_page().
	 */
	if (folio_test_anon(folio))
		try_to_migrate(folio, ttu_flags);
	else
		try_to_unmap(folio, ttu_flags | TTU_IGNORE_MLOCK);

	try_to_unmap_flush();
}

static void remap_page(struct folio *folio, unsigned long nr)
{
	int i = 0;

	/* If unmap_folio() uses try_to_migrate() on file, remove this check */
	if (!folio_test_anon(folio))
		return;
	for (;;) {
		remove_migration_ptes(folio, folio, true);
		i += folio_nr_pages(folio);
		if (i >= nr)
			break;
		folio = folio_next(folio);
	}
}

static void lru_add_page_tail(struct page *head, struct page *tail,
		struct lruvec *lruvec, struct list_head *list)
{
	VM_BUG_ON_PAGE(!PageHead(head), head);
	VM_BUG_ON_PAGE(PageLRU(tail), head);
	lockdep_assert_held(&lruvec->lru_lock);

	if (list) {
		/* page reclaim is reclaiming a huge page */
		VM_WARN_ON(PageLRU(head));
		get_page(tail);
		list_add_tail(&tail->lru, list);
	} else {
		/* head is still on lru (and we have it frozen) */
		VM_WARN_ON(!PageLRU(head));
		if (PageUnevictable(tail))
			tail->mlock_count = 0;
		else
			list_add_tail(&tail->lru, &head->lru);
		SetPageLRU(tail);
	}
}

static void __split_huge_page_tail(struct folio *folio, int tail,
		struct lruvec *lruvec, struct list_head *list,
		unsigned int new_order)
{
	struct page *head = &folio->page;
	struct page *page_tail = head + tail;
	/*
	 * Careful: new_folio is not a "real" folio before we cleared PageTail.
	 * Don't pass it around before clear_compound_head().
	 */
	struct folio *new_folio = (struct folio *)page_tail;

	VM_BUG_ON_PAGE(atomic_read(&page_tail->_mapcount) != -1, page_tail);

	/*
	 * Clone page flags before unfreezing refcount.
	 *
	 * After successful get_page_unless_zero() might follow flags change,
	 * for example lock_page() which set PG_waiters.
	 *
	 * Note that for mapped sub-pages of an anonymous THP,
	 * PG_anon_exclusive has been cleared in unmap_folio() and is stored in
	 * the migration entry instead from where remap_page() will restore it.
	 * We can still have PG_anon_exclusive set on effectively unmapped and
	 * unreferenced sub-pages of an anonymous THP: we can simply drop
	 * PG_anon_exclusive (-> PG_mappedtodisk) for these here.
	 */
	page_tail->flags &= ~PAGE_FLAGS_CHECK_AT_PREP;
	page_tail->flags |= (head->flags &
			((1L << PG_referenced) |
			 (1L << PG_swapbacked) |
			 (1L << PG_swapcache) |
			 (1L << PG_mlocked) |
			 (1L << PG_uptodate) |
			 (1L << PG_active) |
			 (1L << PG_workingset) |
			 (1L << PG_locked) |
			 (1L << PG_unevictable) |
#ifdef CONFIG_ARCH_USES_PG_ARCH_X
			 (1L << PG_arch_2) |
			 (1L << PG_arch_3) |
#endif
			 (1L << PG_dirty) |
			 LRU_GEN_MASK | LRU_REFS_MASK));

	/* ->mapping in first and second tail page is replaced by other uses */
	VM_BUG_ON_PAGE(tail > 2 && page_tail->mapping != TAIL_MAPPING,
			page_tail);
	page_tail->mapping = head->mapping;
	page_tail->index = head->index + tail;

	/*
	 * page->private should not be set in tail pages. Fix up and warn once
	 * if private is unexpectedly set.
	 */
	if (unlikely(page_tail->private)) {
		VM_WARN_ON_ONCE_PAGE(true, page_tail);
		page_tail->private = 0;
	}
	if (folio_test_swapcache(folio))
		new_folio->swap.val = folio->swap.val + tail;

	/* Page flags must be visible before we make the page non-compound. */
	smp_wmb();

	/*
	 * Clear PageTail before unfreezing page refcount.
	 *
	 * After successful get_page_unless_zero() might follow put_page()
	 * which needs correct compound_head().
	 */
	clear_compound_head(page_tail);
	if (new_order) {
		prep_compound_page(page_tail, new_order);
		folio_prep_large_rmappable(new_folio);
	}

	/* Finally unfreeze refcount. Additional reference from page cache. */
	page_ref_unfreeze(page_tail,
		1 + ((!folio_test_anon(folio) || folio_test_swapcache(folio)) ?
			     folio_nr_pages(new_folio) : 0));

	if (folio_test_young(folio))
		folio_set_young(new_folio);
	if (folio_test_idle(folio))
		folio_set_idle(new_folio);

	folio_xchg_last_cpupid(new_folio, folio_last_cpupid(folio));

	/*
	 * always add to the tail because some iterators expect new
	 * pages to show after the currently processed elements - e.g.
	 * migrate_pages
	 */
	lru_add_page_tail(head, page_tail, lruvec, list);
}

static void __split_huge_page(struct page *page, struct list_head *list,
		pgoff_t end, unsigned int new_order)
{
	struct folio *folio = page_folio(page);
	struct page *head = &folio->page;
	struct lruvec *lruvec;
	struct address_space *swap_cache = NULL;
	unsigned long offset = 0;
	int i, nr_dropped = 0;
	unsigned int new_nr = 1 << new_order;
	int order = folio_order(folio);
	unsigned int nr = 1 << order;

	/* complete memcg works before add pages to LRU */
	split_page_memcg(head, order, new_order);

	if (folio_test_anon(folio) && folio_test_swapcache(folio)) {
		offset = swp_offset(folio->swap);
		swap_cache = swap_address_space(folio->swap);
		xa_lock(&swap_cache->i_pages);
	}

	/* lock lru list/PageCompound, ref frozen by page_ref_freeze */
	lruvec = folio_lruvec_lock(folio);

	ClearPageHasHWPoisoned(head);

	for (i = nr - new_nr; i >= new_nr; i -= new_nr) {
		__split_huge_page_tail(folio, i, lruvec, list, new_order);
		/* Some pages can be beyond EOF: drop them from page cache */
		if (head[i].index >= end) {
			struct folio *tail = page_folio(head + i);

			if (shmem_mapping(folio->mapping))
				nr_dropped++;
			else if (folio_test_clear_dirty(tail))
				folio_account_cleaned(tail,
					inode_to_wb(folio->mapping->host));
			__filemap_remove_folio(tail, NULL);
			folio_put(tail);
		} else if (!PageAnon(page)) {
			__xa_store(&folio->mapping->i_pages, head[i].index,
					head + i, 0);
		} else if (swap_cache) {
			__xa_store(&swap_cache->i_pages, offset + i,
					head + i, 0);
		}
	}

	if (!new_order)
		ClearPageCompound(head);
	else {
		struct folio *new_folio = (struct folio *)head;

		folio_set_order(new_folio, new_order);
	}
	unlock_page_lruvec(lruvec);
	/* Caller disabled irqs, so they are still disabled here */

	split_page_owner(head, order, new_order);

	/* See comment in __split_huge_page_tail() */
	if (folio_test_anon(folio)) {
		/* Additional pin to swap cache */
		if (folio_test_swapcache(folio)) {
			folio_ref_add(folio, 1 + new_nr);
			xa_unlock(&swap_cache->i_pages);
		} else {
			folio_ref_inc(folio);
		}
	} else {
		/* Additional pin to page cache */
		folio_ref_add(folio, 1 + new_nr);
		xa_unlock(&folio->mapping->i_pages);
	}
	local_irq_enable();

	if (nr_dropped)
		shmem_uncharge(folio->mapping->host, nr_dropped);
	remap_page(folio, nr);

	if (folio_test_swapcache(folio))
		split_swap_cluster(folio->swap);

	/*
	 * set page to its compound_head when split to non order-0 pages, so
	 * we can skip unlocking it below, since PG_locked is transferred to
	 * the compound_head of the page and the caller will unlock it.
	 */
	if (new_order)
		page = compound_head(page);

	for (i = 0; i < nr; i += new_nr) {
		struct page *subpage = head + i;
		struct folio *new_folio = page_folio(subpage);
		if (subpage == page)
			continue;
		folio_unlock(new_folio);

		/*
		 * Subpages may be freed if there wasn't any mapping
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * like if add_to_swap() is running on a lru page that
		 * had its mapping zapped. And freeing these pages
		 * requires taking the lru_lock so we do the put_page
		 * of the tail pages after the split is complete.
		 */
<<<<<<< HEAD
		put_page(page_tail);
	}

	/*
	 * Only the head page (now become a regular page) is required
	 * to be pinned by the caller.
	 */
	BUG_ON(page_count(page) <= 0);
}

static int __split_huge_page_map(struct page *page,
				 struct vm_area_struct *vma,
				 unsigned long address)
{
	struct mm_struct *mm = vma->vm_mm;
	pmd_t *pmd, _pmd;
	int ret = 0, i;
	pgtable_t pgtable;
	unsigned long haddr;

	spin_lock(&mm->page_table_lock);
	pmd = page_check_address_pmd(page, mm, address,
				     PAGE_CHECK_ADDRESS_PMD_SPLITTING_FLAG);
	if (pmd) {
		pgtable = get_pmd_huge_pte(mm);
		pmd_populate(mm, &_pmd, pgtable);

		for (i = 0, haddr = address; i < HPAGE_PMD_NR;
		     i++, haddr += PAGE_SIZE) {
			pte_t *pte, entry;
			BUG_ON(PageCompound(page+i));
			entry = mk_pte(page + i, vma->vm_page_prot);
			entry = maybe_mkwrite(pte_mkdirty(entry), vma);
			if (!pmd_write(*pmd))
				entry = pte_wrprotect(entry);
			else
				BUG_ON(page_mapcount(page) != 1);
			if (!pmd_young(*pmd))
				entry = pte_mkold(entry);
			pte = pte_offset_map(&_pmd, haddr);
			BUG_ON(!pte_none(*pte));
			set_pte_at(mm, haddr, pte, entry);
			pte_unmap(pte);
		}

		smp_wmb(); /* make pte visible before pmd */
		/*
		 * Up to this point the pmd is present and huge and
		 * userland has the whole access to the hugepage
		 * during the split (which happens in place). If we
		 * overwrite the pmd with the not-huge version
		 * pointing to the pte here (which of course we could
		 * if all CPUs were bug free), userland could trigger
		 * a small page size TLB miss on the small sized TLB
		 * while the hugepage TLB entry is still established
		 * in the huge TLB. Some CPU doesn't like that. See
		 * http://support.amd.com/us/Processor_TechDocs/41322.pdf,
		 * Erratum 383 on page 93. Intel should be safe but is
		 * also warns that it's only safe if the permission
		 * and cache attributes of the two entries loaded in
		 * the two TLB is identical (which should be the case
		 * here). But it is generally safer to never allow
		 * small and huge TLB entries for the same virtual
		 * address to be loaded simultaneously. So instead of
		 * doing "pmd_populate(); flush_tlb_range();" we first
		 * mark the current pmd notpresent (atomically because
		 * here the pmd_trans_huge and pmd_trans_splitting
		 * must remain set at all times on the pmd until the
		 * split is complete for this pmd), then we flush the
		 * SMP TLB and finally we write the non-huge version
		 * of the pmd entry with pmd_populate.
		 */
		set_pmd_at(mm, address, pmd, pmd_mknotpresent(*pmd));
		flush_tlb_range(vma, address, address + HPAGE_PMD_SIZE);
		pmd_populate(mm, pmd, pgtable);
		ret = 1;
	}
	spin_unlock(&mm->page_table_lock);

	return ret;
}

/* must be called with anon_vma->root->mutex hold */
static void __split_huge_page(struct page *page,
			      struct anon_vma *anon_vma)
{
	int mapcount, mapcount2;
	struct anon_vma_chain *avc;

	BUG_ON(!PageHead(page));
	BUG_ON(PageTail(page));

	mapcount = 0;
	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long addr = vma_address(page, vma);
		BUG_ON(is_vma_temporary_stack(vma));
		if (addr == -EFAULT)
			continue;
		mapcount += __split_huge_page_splitting(page, vma, addr);
	}
	/*
	 * It is critical that new vmas are added to the tail of the
	 * anon_vma list. This guarantes that if copy_huge_pmd() runs
	 * and establishes a child pmd before
	 * __split_huge_page_splitting() freezes the parent pmd (so if
	 * we fail to prevent copy_huge_pmd() from running until the
	 * whole __split_huge_page() is complete), we will still see
	 * the newly established pmd of the child later during the
	 * walk, to be able to set it as pmd_trans_splitting too.
	 */
	if (mapcount != page_mapcount(page))
		printk(KERN_ERR "mapcount %d page_mapcount %d\n",
		       mapcount, page_mapcount(page));
	BUG_ON(mapcount != page_mapcount(page));

	__split_huge_page_refcount(page);

	mapcount2 = 0;
	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long addr = vma_address(page, vma);
		BUG_ON(is_vma_temporary_stack(vma));
		if (addr == -EFAULT)
			continue;
		mapcount2 += __split_huge_page_map(page, vma, addr);
	}
	if (mapcount != mapcount2)
		printk(KERN_ERR "mapcount %d mapcount2 %d page_mapcount %d\n",
		       mapcount, mapcount2, page_mapcount(page));
	BUG_ON(mapcount != mapcount2);
}

int split_huge_page(struct page *page)
{
	struct anon_vma *anon_vma;
	int ret = 1;

	BUG_ON(!PageAnon(page));
	anon_vma = page_lock_anon_vma(page);
	if (!anon_vma)
		goto out;
	ret = 0;
	if (!PageCompound(page))
		goto out_unlock;

	BUG_ON(!PageSwapBacked(page));
	__split_huge_page(page, anon_vma);
	count_vm_event(THP_SPLIT);

	BUG_ON(PageCompound(page));
out_unlock:
	page_unlock_anon_vma(anon_vma);
out:
	return ret;
}

#define VM_NO_THP (VM_SPECIAL|VM_INSERTPAGE|VM_MIXEDMAP|VM_SAO| \
		   VM_HUGETLB|VM_SHARED|VM_MAYSHARE)

int hugepage_madvise(struct vm_area_struct *vma,
		     unsigned long *vm_flags, int advice)
{
	switch (advice) {
	case MADV_HUGEPAGE:
		/*
		 * Be somewhat over-protective like KSM for now!
		 */
		if (*vm_flags & (VM_HUGEPAGE | VM_NO_THP))
			return -EINVAL;
		*vm_flags &= ~VM_NOHUGEPAGE;
		*vm_flags |= VM_HUGEPAGE;
		/*
		 * If the vma become good for khugepaged to scan,
		 * register it here without waiting a page fault that
		 * may not happen any time soon.
		 */
		if (unlikely(khugepaged_enter_vma_merge(vma, *vm_flags)))
			return -ENOMEM;
		break;
	case MADV_NOHUGEPAGE:
		/*
		 * Be somewhat over-protective like KSM for now!
		 */
		if (*vm_flags & (VM_NOHUGEPAGE | VM_NO_THP))
			return -EINVAL;
		*vm_flags &= ~VM_HUGEPAGE;
		*vm_flags |= VM_NOHUGEPAGE;
		/*
		 * Setting VM_NOHUGEPAGE will prevent khugepaged from scanning
		 * this vma even if we leave the mm registered in khugepaged if
		 * it got registered before VM_NOHUGEPAGE was set.
		 */
		break;
	}

	return 0;
}

static int __init khugepaged_slab_init(void)
{
	mm_slot_cache = kmem_cache_create("khugepaged_mm_slot",
					  sizeof(struct mm_slot),
					  __alignof__(struct mm_slot), 0, NULL);
	if (!mm_slot_cache)
		return -ENOMEM;

	return 0;
}

static void __init khugepaged_slab_free(void)
{
	kmem_cache_destroy(mm_slot_cache);
	mm_slot_cache = NULL;
}

static inline struct mm_slot *alloc_mm_slot(void)
{
	if (!mm_slot_cache)	/* initialization failed */
		return NULL;
	return kmem_cache_zalloc(mm_slot_cache, GFP_KERNEL);
}

static inline void free_mm_slot(struct mm_slot *mm_slot)
{
	kmem_cache_free(mm_slot_cache, mm_slot);
}

static int __init mm_slots_hash_init(void)
{
	mm_slots_hash = kzalloc(MM_SLOTS_HASH_HEADS * sizeof(struct hlist_head),
				GFP_KERNEL);
	if (!mm_slots_hash)
		return -ENOMEM;
	return 0;
}

#if 0
static void __init mm_slots_hash_free(void)
{
	kfree(mm_slots_hash);
	mm_slots_hash = NULL;
}
#endif

static struct mm_slot *get_mm_slot(struct mm_struct *mm)
{
	struct mm_slot *mm_slot;
	struct hlist_head *bucket;
	struct hlist_node *node;

	bucket = &mm_slots_hash[((unsigned long)mm / sizeof(struct mm_struct))
				% MM_SLOTS_HASH_HEADS];
	hlist_for_each_entry(mm_slot, node, bucket, hash) {
		if (mm == mm_slot->mm)
			return mm_slot;
	}
	return NULL;
}

static void insert_to_mm_slots_hash(struct mm_struct *mm,
				    struct mm_slot *mm_slot)
{
	struct hlist_head *bucket;

	bucket = &mm_slots_hash[((unsigned long)mm / sizeof(struct mm_struct))
				% MM_SLOTS_HASH_HEADS];
	mm_slot->mm = mm;
	hlist_add_head(&mm_slot->hash, bucket);
}

static inline int khugepaged_test_exit(struct mm_struct *mm)
{
	return atomic_read(&mm->mm_users) == 0;
}

int __khugepaged_enter(struct mm_struct *mm)
{
	struct mm_slot *mm_slot;
	int wakeup;

	mm_slot = alloc_mm_slot();
	if (!mm_slot)
		return -ENOMEM;

	/* __khugepaged_exit() must not run from under us */
	VM_BUG_ON(khugepaged_test_exit(mm));
	if (unlikely(test_and_set_bit(MMF_VM_HUGEPAGE, &mm->flags))) {
		free_mm_slot(mm_slot);
		return 0;
	}

	spin_lock(&khugepaged_mm_lock);
	insert_to_mm_slots_hash(mm, mm_slot);
	/*
	 * Insert just behind the scanning cursor, to let the area settle
	 * down a little.
	 */
	wakeup = list_empty(&khugepaged_scan.mm_head);
	list_add_tail(&mm_slot->mm_node, &khugepaged_scan.mm_head);
	spin_unlock(&khugepaged_mm_lock);

	atomic_inc(&mm->mm_count);
	if (wakeup)
		wake_up_interruptible(&khugepaged_wait);

	return 0;
}

int khugepaged_enter_vma_merge(struct vm_area_struct *vma,
			       unsigned long vm_flags)
{
	unsigned long hstart, hend;
	if (!vma->anon_vma)
		/*
		 * Not yet faulted in so we will register later in the
		 * page fault if needed.
		 */
		return 0;
	if (vma->vm_ops)
		/* khugepaged not yet working on file or special mappings */
		return 0;
	/*
	 * If is_pfn_mapping() is true is_learn_pfn_mapping() must be
	 * true too, verify it here.
	 */
	VM_BUG_ON(is_linear_pfn_mapping(vma) || vm_flags & VM_NO_THP);
	hstart = (vma->vm_start + ~HPAGE_PMD_MASK) & HPAGE_PMD_MASK;
	hend = vma->vm_end & HPAGE_PMD_MASK;
	if (hstart < hend)
		return khugepaged_enter(vma, vm_flags);
	return 0;
}

void __khugepaged_exit(struct mm_struct *mm)
{
	struct mm_slot *mm_slot;
	int free = 0;

	spin_lock(&khugepaged_mm_lock);
	mm_slot = get_mm_slot(mm);
	if (mm_slot && khugepaged_scan.mm_slot != mm_slot) {
		hlist_del(&mm_slot->hash);
		list_del(&mm_slot->mm_node);
		free = 1;
	}
	spin_unlock(&khugepaged_mm_lock);

	if (free) {
		clear_bit(MMF_VM_HUGEPAGE, &mm->flags);
		free_mm_slot(mm_slot);
		mmdrop(mm);
	} else if (mm_slot) {
		/*
		 * This is required to serialize against
		 * khugepaged_test_exit() (which is guaranteed to run
		 * under mmap sem read mode). Stop here (after we
		 * return all pagetables will be destroyed) until
		 * khugepaged has finished working on the pagetables
		 * under the mmap_sem.
		 */
		down_write(&mm->mmap_sem);
		up_write(&mm->mmap_sem);
	}
}

static void release_pte_page(struct page *page)
{
	/* 0 stands for page_is_file_cache(page) == false */
	dec_zone_page_state(page, NR_ISOLATED_ANON + 0);
	unlock_page(page);
	putback_lru_page(page);
}

static void release_pte_pages(pte_t *pte, pte_t *_pte)
{
	while (--_pte >= pte) {
		pte_t pteval = *_pte;
		if (!pte_none(pteval))
			release_pte_page(pte_page(pteval));
	}
}

static void release_all_pte_pages(pte_t *pte)
{
	release_pte_pages(pte, pte + HPAGE_PMD_NR);
}

static int __collapse_huge_page_isolate(struct vm_area_struct *vma,
					unsigned long address,
					pte_t *pte)
{
	struct page *page;
	pte_t *_pte;
	int referenced = 0, isolated = 0, none = 0;
	for (_pte = pte; _pte < pte+HPAGE_PMD_NR;
	     _pte++, address += PAGE_SIZE) {
		pte_t pteval = *_pte;
		if (pte_none(pteval)) {
			if (++none <= khugepaged_max_ptes_none)
				continue;
			else {
				release_pte_pages(pte, _pte);
				goto out;
			}
		}
		if (!pte_present(pteval) || !pte_write(pteval)) {
			release_pte_pages(pte, _pte);
			goto out;
		}
		page = vm_normal_page(vma, address, pteval);
		if (unlikely(!page)) {
			release_pte_pages(pte, _pte);
			goto out;
		}
		VM_BUG_ON(PageCompound(page));
		BUG_ON(!PageAnon(page));
		VM_BUG_ON(!PageSwapBacked(page));

		/* cannot use mapcount: can't collapse if there's a gup pin */
		if (page_count(page) != 1) {
			release_pte_pages(pte, _pte);
			goto out;
		}
		/*
		 * We can do it before isolate_lru_page because the
		 * page can't be freed from under us. NOTE: PG_lock
		 * is needed to serialize against split_huge_page
		 * when invoked from the VM.
		 */
		if (!trylock_page(page)) {
			release_pte_pages(pte, _pte);
			goto out;
		}
		/*
		 * Isolate the page to avoid collapsing an hugepage
		 * currently in use by the VM.
		 */
		if (isolate_lru_page(page)) {
			unlock_page(page);
			release_pte_pages(pte, _pte);
			goto out;
		}
		/* 0 stands for page_is_file_cache(page) == false */
		inc_zone_page_state(page, NR_ISOLATED_ANON + 0);
		VM_BUG_ON(!PageLocked(page));
		VM_BUG_ON(PageLRU(page));

		/* If there is no mapped pte young don't collapse the page */
		if (pte_young(pteval) || PageReferenced(page) ||
		    mmu_notifier_test_young(vma->vm_mm, address))
			referenced = 1;
	}
	if (unlikely(!referenced))
		release_all_pte_pages(pte);
	else
		isolated = 1;
out:
	return isolated;
}

static void __collapse_huge_page_copy(pte_t *pte, struct page *page,
				      struct vm_area_struct *vma,
				      unsigned long address,
				      spinlock_t *ptl)
{
	pte_t *_pte;
	for (_pte = pte; _pte < pte+HPAGE_PMD_NR; _pte++) {
		pte_t pteval = *_pte;
		struct page *src_page;

		if (pte_none(pteval)) {
			clear_user_highpage(page, address);
			add_mm_counter(vma->vm_mm, MM_ANONPAGES, 1);
		} else {
			src_page = pte_page(pteval);
			copy_user_highpage(page, src_page, address, vma);
			VM_BUG_ON(page_mapcount(src_page) != 1);
			VM_BUG_ON(page_count(src_page) != 2);
			release_pte_page(src_page);
			/*
			 * ptl mostly unnecessary, but preempt has to
			 * be disabled to update the per-cpu stats
			 * inside page_remove_rmap().
			 */
			spin_lock(ptl);
			/*
			 * paravirt calls inside pte_clear here are
			 * superfluous.
			 */
			pte_clear(vma->vm_mm, address, _pte);
			page_remove_rmap(src_page);
			spin_unlock(ptl);
			free_page_and_swap_cache(src_page);
		}

		address += PAGE_SIZE;
		page++;
	}
}

static void collapse_huge_page(struct mm_struct *mm,
			       unsigned long address,
			       struct page **hpage,
			       struct vm_area_struct *vma,
			       int node)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd, _pmd;
	pte_t *pte;
	pgtable_t pgtable;
	struct page *new_page;
	spinlock_t *ptl;
	int isolated;
	unsigned long hstart, hend;

	VM_BUG_ON(address & ~HPAGE_PMD_MASK);
#ifndef CONFIG_NUMA
	up_read(&mm->mmap_sem);
	VM_BUG_ON(!*hpage);
	new_page = *hpage;
#else
	VM_BUG_ON(*hpage);
	/*
	 * Allocate the page while the vma is still valid and under
	 * the mmap_sem read mode so there is no memory allocation
	 * later when we take the mmap_sem in write mode. This is more
	 * friendly behavior (OTOH it may actually hide bugs) to
	 * filesystems in userland with daemons allocating memory in
	 * the userland I/O paths.  Allocating memory with the
	 * mmap_sem in read mode is good idea also to allow greater
	 * scalability.
	 */
	new_page = alloc_hugepage_vma(khugepaged_defrag(), vma, address,
				      node, __GFP_OTHER_NODE);

	/*
	 * After allocating the hugepage, release the mmap_sem read lock in
	 * preparation for taking it in write mode.
	 */
	up_read(&mm->mmap_sem);
	if (unlikely(!new_page)) {
		count_vm_event(THP_COLLAPSE_ALLOC_FAILED);
		*hpage = ERR_PTR(-ENOMEM);
		return;
	}
#endif

	count_vm_event(THP_COLLAPSE_ALLOC);
	if (unlikely(mem_cgroup_newpage_charge(new_page, mm, GFP_KERNEL))) {
#ifdef CONFIG_NUMA
		put_page(new_page);
#endif
		return;
	}

	/*
	 * Prevent all access to pagetables with the exception of
	 * gup_fast later hanlded by the ptep_clear_flush and the VM
	 * handled by the anon_vma lock + PG_lock.
	 */
	down_write(&mm->mmap_sem);
	if (unlikely(khugepaged_test_exit(mm)))
		goto out;

	vma = find_vma(mm, address);
	if (!vma)
		goto out;
	hstart = (vma->vm_start + ~HPAGE_PMD_MASK) & HPAGE_PMD_MASK;
	hend = vma->vm_end & HPAGE_PMD_MASK;
	if (address < hstart || address + HPAGE_PMD_SIZE > hend)
		goto out;

	if ((!(vma->vm_flags & VM_HUGEPAGE) && !khugepaged_always()) ||
	    (vma->vm_flags & VM_NOHUGEPAGE))
		goto out;

	if (!vma->anon_vma || vma->vm_ops)
		goto out;
	if (is_vma_temporary_stack(vma))
		goto out;
	/*
	 * If is_pfn_mapping() is true is_learn_pfn_mapping() must be
	 * true too, verify it here.
	 */
	VM_BUG_ON(is_linear_pfn_mapping(vma) || vma->vm_flags & VM_NO_THP);

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		goto out;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		goto out;

	pmd = pmd_offset(pud, address);
	/* pmd can't go away or become huge under us */
	if (!pmd_present(*pmd) || pmd_trans_huge(*pmd))
		goto out;

	anon_vma_lock(vma->anon_vma);

	pte = pte_offset_map(pmd, address);
	ptl = pte_lockptr(mm, pmd);

	spin_lock(&mm->page_table_lock); /* probably unnecessary */
	/*
	 * After this gup_fast can't run anymore. This also removes
	 * any huge TLB entry from the CPU so we won't allow
	 * huge and small TLB entries for the same virtual address
	 * to avoid the risk of CPU bugs in that area.
	 */
	_pmd = pmdp_clear_flush_notify(vma, address, pmd);
	spin_unlock(&mm->page_table_lock);

	spin_lock(ptl);
	isolated = __collapse_huge_page_isolate(vma, address, pte);
	spin_unlock(ptl);

	if (unlikely(!isolated)) {
		pte_unmap(pte);
		spin_lock(&mm->page_table_lock);
		BUG_ON(!pmd_none(*pmd));
		/*
		 * We can only use set_pmd_at when establishing
		 * hugepmds and never for establishing regular pmds that
		 * points to regular pagetables. Use pmd_populate for that
		 */
		pmd_populate(mm, pmd, pmd_pgtable(_pmd));
		spin_unlock(&mm->page_table_lock);
		anon_vma_unlock(vma->anon_vma);
		goto out;
	}

	/*
	 * All pages are isolated and locked so anon_vma rmap
	 * can't run anymore.
	 */
	anon_vma_unlock(vma->anon_vma);

	__collapse_huge_page_copy(pte, new_page, vma, address, ptl);
	pte_unmap(pte);
	__SetPageUptodate(new_page);
	pgtable = pmd_pgtable(_pmd);
	VM_BUG_ON(page_count(pgtable) != 1);
	VM_BUG_ON(page_mapcount(pgtable) != 0);

	_pmd = mk_pmd(new_page, vma->vm_page_prot);
	_pmd = maybe_pmd_mkwrite(pmd_mkdirty(_pmd), vma);
	_pmd = pmd_mkhuge(_pmd);

	/*
	 * spin_lock() below is not the equivalent of smp_wmb(), so
	 * this is needed to avoid the copy_huge_page writes to become
	 * visible after the set_pmd_at() write.
	 */
	smp_wmb();

	spin_lock(&mm->page_table_lock);
	BUG_ON(!pmd_none(*pmd));
	page_add_new_anon_rmap(new_page, vma, address);
	set_pmd_at(mm, address, pmd, _pmd);
	update_mmu_cache(vma, address, _pmd);
	prepare_pmd_huge_pte(pgtable, mm);
	spin_unlock(&mm->page_table_lock);

#ifndef CONFIG_NUMA
	*hpage = NULL;
#endif
	khugepaged_pages_collapsed++;
out_up_write:
	up_write(&mm->mmap_sem);
	return;

out:
	mem_cgroup_uncharge_page(new_page);
#ifdef CONFIG_NUMA
	put_page(new_page);
#endif
	goto out_up_write;
}

static int khugepaged_scan_pmd(struct mm_struct *mm,
			       struct vm_area_struct *vma,
			       unsigned long address,
			       struct page **hpage)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte, *_pte;
	int ret = 0, referenced = 0, none = 0;
	struct page *page;
	unsigned long _address;
	spinlock_t *ptl;
	int node = -1;

	VM_BUG_ON(address & ~HPAGE_PMD_MASK);

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		goto out;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		goto out;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd) || pmd_trans_huge(*pmd))
		goto out;

	pte = pte_offset_map_lock(mm, pmd, address, &ptl);
	for (_address = address, _pte = pte; _pte < pte+HPAGE_PMD_NR;
	     _pte++, _address += PAGE_SIZE) {
		pte_t pteval = *_pte;
		if (pte_none(pteval)) {
			if (++none <= khugepaged_max_ptes_none)
				continue;
			else
				goto out_unmap;
		}
		if (!pte_present(pteval) || !pte_write(pteval))
			goto out_unmap;
		page = vm_normal_page(vma, _address, pteval);
		if (unlikely(!page))
			goto out_unmap;
		/*
		 * Chose the node of the first page. This could
		 * be more sophisticated and look at more pages,
		 * but isn't for now.
		 */
		if (node == -1)
			node = page_to_nid(page);
		VM_BUG_ON(PageCompound(page));
		if (!PageLRU(page) || PageLocked(page) || !PageAnon(page))
			goto out_unmap;
		/* cannot use mapcount: can't collapse if there's a gup pin */
		if (page_count(page) != 1)
			goto out_unmap;
		if (pte_young(pteval) || PageReferenced(page) ||
		    mmu_notifier_test_young(vma->vm_mm, address))
			referenced = 1;
	}
	if (referenced)
		ret = 1;
out_unmap:
	pte_unmap_unlock(pte, ptl);
	if (ret)
		/* collapse_huge_page will return with the mmap_sem released */
		collapse_huge_page(mm, address, hpage, vma, node);
out:
	return ret;
}

static void collect_mm_slot(struct mm_slot *mm_slot)
{
	struct mm_struct *mm = mm_slot->mm;

	VM_BUG_ON(NR_CPUS != 1 && !spin_is_locked(&khugepaged_mm_lock));

	if (khugepaged_test_exit(mm)) {
		/* free mm_slot */
		hlist_del(&mm_slot->hash);
		list_del(&mm_slot->mm_node);

		/*
		 * Not strictly needed because the mm exited already.
		 *
		 * clear_bit(MMF_VM_HUGEPAGE, &mm->flags);
		 */

		/* khugepaged_mm_lock actually not necessary for the below */
		free_mm_slot(mm_slot);
		mmdrop(mm);
	}
}

static unsigned int khugepaged_scan_mm_slot(unsigned int pages,
					    struct page **hpage)
	__releases(&khugepaged_mm_lock)
	__acquires(&khugepaged_mm_lock)
{
	struct mm_slot *mm_slot;
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	int progress = 0;

	VM_BUG_ON(!pages);
	VM_BUG_ON(NR_CPUS != 1 && !spin_is_locked(&khugepaged_mm_lock));

	if (khugepaged_scan.mm_slot)
		mm_slot = khugepaged_scan.mm_slot;
	else {
		mm_slot = list_entry(khugepaged_scan.mm_head.next,
				     struct mm_slot, mm_node);
		khugepaged_scan.address = 0;
		khugepaged_scan.mm_slot = mm_slot;
	}
	spin_unlock(&khugepaged_mm_lock);

	mm = mm_slot->mm;
	down_read(&mm->mmap_sem);
	if (unlikely(khugepaged_test_exit(mm)))
		vma = NULL;
	else
		vma = find_vma(mm, khugepaged_scan.address);

	progress++;
	for (; vma; vma = vma->vm_next) {
		unsigned long hstart, hend;

		cond_resched();
		if (unlikely(khugepaged_test_exit(mm))) {
			progress++;
			break;
		}

		if ((!(vma->vm_flags & VM_HUGEPAGE) &&
		     !khugepaged_always()) ||
		    (vma->vm_flags & VM_NOHUGEPAGE)) {
		skip:
			progress++;
			continue;
		}
		if (!vma->anon_vma || vma->vm_ops)
			goto skip;
		if (is_vma_temporary_stack(vma))
			goto skip;
		/*
		 * If is_pfn_mapping() is true is_learn_pfn_mapping()
		 * must be true too, verify it here.
		 */
		VM_BUG_ON(is_linear_pfn_mapping(vma) ||
			  vma->vm_flags & VM_NO_THP);

		hstart = (vma->vm_start + ~HPAGE_PMD_MASK) & HPAGE_PMD_MASK;
		hend = vma->vm_end & HPAGE_PMD_MASK;
		if (hstart >= hend)
			goto skip;
		if (khugepaged_scan.address > hend)
			goto skip;
		if (khugepaged_scan.address < hstart)
			khugepaged_scan.address = hstart;
		VM_BUG_ON(khugepaged_scan.address & ~HPAGE_PMD_MASK);

		while (khugepaged_scan.address < hend) {
			int ret;
			cond_resched();
			if (unlikely(khugepaged_test_exit(mm)))
				goto breakouterloop;

			VM_BUG_ON(khugepaged_scan.address < hstart ||
				  khugepaged_scan.address + HPAGE_PMD_SIZE >
				  hend);
			ret = khugepaged_scan_pmd(mm, vma,
						  khugepaged_scan.address,
						  hpage);
			/* move to next address */
			khugepaged_scan.address += HPAGE_PMD_SIZE;
			progress += HPAGE_PMD_NR;
			if (ret)
				/* we released mmap_sem so break loop */
				goto breakouterloop_mmap_sem;
			if (progress >= pages)
				goto breakouterloop;
		}
	}
breakouterloop:
	up_read(&mm->mmap_sem); /* exit_mmap will destroy ptes after this */
breakouterloop_mmap_sem:

	spin_lock(&khugepaged_mm_lock);
	VM_BUG_ON(khugepaged_scan.mm_slot != mm_slot);
	/*
	 * Release the current mm_slot if this mm is about to die, or
	 * if we scanned all vmas of this mm.
	 */
	if (khugepaged_test_exit(mm) || !vma) {
		/*
		 * Make sure that if mm_users is reaching zero while
		 * khugepaged runs here, khugepaged_exit will find
		 * mm_slot not pointing to the exiting mm.
		 */
		if (mm_slot->mm_node.next != &khugepaged_scan.mm_head) {
			khugepaged_scan.mm_slot = list_entry(
				mm_slot->mm_node.next,
				struct mm_slot, mm_node);
			khugepaged_scan.address = 0;
		} else {
			khugepaged_scan.mm_slot = NULL;
			khugepaged_full_scans++;
		}

		collect_mm_slot(mm_slot);
	}

	return progress;
}

static int khugepaged_has_work(void)
{
	return !list_empty(&khugepaged_scan.mm_head) &&
		khugepaged_enabled();
}

static int khugepaged_wait_event(void)
{
	return !list_empty(&khugepaged_scan.mm_head) ||
		!khugepaged_enabled();
}

static void khugepaged_do_scan(struct page **hpage)
{
	unsigned int progress = 0, pass_through_head = 0;
	unsigned int pages = khugepaged_pages_to_scan;

	barrier(); /* write khugepaged_pages_to_scan to local stack */

	while (progress < pages) {
		cond_resched();

#ifndef CONFIG_NUMA
		if (!*hpage) {
			*hpage = alloc_hugepage(khugepaged_defrag());
			if (unlikely(!*hpage)) {
				count_vm_event(THP_COLLAPSE_ALLOC_FAILED);
				break;
			}
			count_vm_event(THP_COLLAPSE_ALLOC);
		}
#else
		if (IS_ERR(*hpage))
			break;
#endif

		if (unlikely(kthread_should_stop() || freezing(current)))
			break;

		spin_lock(&khugepaged_mm_lock);
		if (!khugepaged_scan.mm_slot)
			pass_through_head++;
		if (khugepaged_has_work() &&
		    pass_through_head < 2)
			progress += khugepaged_scan_mm_slot(pages - progress,
							    hpage);
		else
			progress = pages;
		spin_unlock(&khugepaged_mm_lock);
	}
}

static void khugepaged_alloc_sleep(void)
{
	wait_event_freezable_timeout(khugepaged_wait, false,
			msecs_to_jiffies(khugepaged_alloc_sleep_millisecs));
}

#ifndef CONFIG_NUMA
static struct page *khugepaged_alloc_hugepage(void)
{
	struct page *hpage;

	do {
		hpage = alloc_hugepage(khugepaged_defrag());
		if (!hpage) {
			count_vm_event(THP_COLLAPSE_ALLOC_FAILED);
			khugepaged_alloc_sleep();
		} else
			count_vm_event(THP_COLLAPSE_ALLOC);
	} while (unlikely(!hpage) &&
		 likely(khugepaged_enabled()));
	return hpage;
}
#endif

static void khugepaged_loop(void)
{
	struct page *hpage;

#ifdef CONFIG_NUMA
	hpage = NULL;
#endif
	while (likely(khugepaged_enabled())) {
#ifndef CONFIG_NUMA
		hpage = khugepaged_alloc_hugepage();
		if (unlikely(!hpage))
			break;
#else
		if (IS_ERR(hpage)) {
			khugepaged_alloc_sleep();
			hpage = NULL;
		}
#endif

		khugepaged_do_scan(&hpage);
#ifndef CONFIG_NUMA
		if (hpage)
			put_page(hpage);
#endif
		try_to_freeze();
		if (unlikely(kthread_should_stop()))
			break;
		if (khugepaged_has_work()) {
			if (!khugepaged_scan_sleep_millisecs)
				continue;
			wait_event_freezable_timeout(khugepaged_wait, false,
			    msecs_to_jiffies(khugepaged_scan_sleep_millisecs));
		} else if (khugepaged_enabled())
			wait_event_freezable(khugepaged_wait,
					     khugepaged_wait_event());
	}
}

static int khugepaged(void *none)
{
	struct mm_slot *mm_slot;

	set_freezable();
	set_user_nice(current, 19);

	/* serialize with start_khugepaged() */
	mutex_lock(&khugepaged_mutex);

	for (;;) {
		mutex_unlock(&khugepaged_mutex);
		VM_BUG_ON(khugepaged_thread != current);
		khugepaged_loop();
		VM_BUG_ON(khugepaged_thread != current);

		mutex_lock(&khugepaged_mutex);
		if (!khugepaged_enabled())
			break;
		if (unlikely(kthread_should_stop()))
			break;
	}

	spin_lock(&khugepaged_mm_lock);
	mm_slot = khugepaged_scan.mm_slot;
	khugepaged_scan.mm_slot = NULL;
	if (mm_slot)
		collect_mm_slot(mm_slot);
	spin_unlock(&khugepaged_mm_lock);

	khugepaged_thread = NULL;
	mutex_unlock(&khugepaged_mutex);
=======
		free_page_and_swap_cache(subpage);
	}
}

/* Racy check whether the huge page can be split */
bool can_split_folio(struct folio *folio, int *pextra_pins)
{
	int extra_pins;

	/* Additional pins from page cache */
	if (folio_test_anon(folio))
		extra_pins = folio_test_swapcache(folio) ?
				folio_nr_pages(folio) : 0;
	else
		extra_pins = folio_nr_pages(folio);
	if (pextra_pins)
		*pextra_pins = extra_pins;
	return folio_mapcount(folio) == folio_ref_count(folio) - extra_pins - 1;
}

/*
 * This function splits huge page into pages in @new_order. @page can point to
 * any subpage of huge page to split. Split doesn't change the position of
 * @page.
 *
 * NOTE: order-1 anonymous folio is not supported because _deferred_list,
 * which is used by partially mapped folios, is stored in subpage 2 and an
 * order-1 folio only has subpage 0 and 1. File-backed order-1 folios are OK,
 * since they do not use _deferred_list.
 *
 * Only caller must hold pin on the @page, otherwise split fails with -EBUSY.
 * The huge page must be locked.
 *
 * If @list is null, tail pages will be added to LRU list, otherwise, to @list.
 *
 * Pages in new_order will inherit mapping, flags, and so on from the hugepage.
 *
 * GUP pin and PG_locked transferred to @page or the compound page @page belongs
 * to. Rest subpages can be freed if they are not mapped.
 *
 * Returns 0 if the hugepage is split successfully.
 * Returns -EBUSY if the page is pinned or if anon_vma disappeared from under
 * us.
 */
int split_huge_page_to_list_to_order(struct page *page, struct list_head *list,
				     unsigned int new_order)
{
	struct folio *folio = page_folio(page);
	struct deferred_split *ds_queue = get_deferred_split_queue(folio);
	/* reset xarray order to new order after split */
	XA_STATE_ORDER(xas, &folio->mapping->i_pages, folio->index, new_order);
	struct anon_vma *anon_vma = NULL;
	struct address_space *mapping = NULL;
	int extra_pins, ret;
	pgoff_t end;
	bool is_hzp;

	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);
	VM_BUG_ON_FOLIO(!folio_test_large(folio), folio);

	if (new_order >= folio_order(folio))
		return -EINVAL;

	/* Cannot split anonymous THP to order-1 */
	if (new_order == 1 && folio_test_anon(folio)) {
		VM_WARN_ONCE(1, "Cannot split to order-1 folio");
		return -EINVAL;
	}

	if (new_order) {
		/* Only swapping a whole PMD-mapped folio is supported */
		if (folio_test_swapcache(folio))
			return -EINVAL;
		/* Split shmem folio to non-zero order not supported */
		if (shmem_mapping(folio->mapping)) {
			VM_WARN_ONCE(1,
				"Cannot split shmem folio to non-0 order");
			return -EINVAL;
		}
		/* No split if the file system does not support large folio */
		if (!mapping_large_folio_support(folio->mapping)) {
			VM_WARN_ONCE(1,
				"Cannot split file folio to non-0 order");
			return -EINVAL;
		}
	}


	is_hzp = is_huge_zero_page(&folio->page);
	if (is_hzp) {
		pr_warn_ratelimited("Called split_huge_page for huge zero page\n");
		return -EBUSY;
	}

	if (folio_test_writeback(folio))
		return -EBUSY;

	if (folio_test_anon(folio)) {
		/*
		 * The caller does not necessarily hold an mmap_lock that would
		 * prevent the anon_vma disappearing so we first we take a
		 * reference to it and then lock the anon_vma for write. This
		 * is similar to folio_lock_anon_vma_read except the write lock
		 * is taken to serialise against parallel split or collapse
		 * operations.
		 */
		anon_vma = folio_get_anon_vma(folio);
		if (!anon_vma) {
			ret = -EBUSY;
			goto out;
		}
		end = -1;
		mapping = NULL;
		anon_vma_lock_write(anon_vma);
	} else {
		gfp_t gfp;

		mapping = folio->mapping;

		/* Truncated ? */
		if (!mapping) {
			ret = -EBUSY;
			goto out;
		}

		gfp = current_gfp_context(mapping_gfp_mask(mapping) &
							GFP_RECLAIM_MASK);

		if (!filemap_release_folio(folio, gfp)) {
			ret = -EBUSY;
			goto out;
		}

		xas_split_alloc(&xas, folio, folio_order(folio), gfp);
		if (xas_error(&xas)) {
			ret = xas_error(&xas);
			goto out;
		}

		anon_vma = NULL;
		i_mmap_lock_read(mapping);

		/*
		 *__split_huge_page() may need to trim off pages beyond EOF:
		 * but on 32-bit, i_size_read() takes an irq-unsafe seqlock,
		 * which cannot be nested inside the page tree lock. So note
		 * end now: i_size itself may be changed at any moment, but
		 * folio lock is good enough to serialize the trimming.
		 */
		end = DIV_ROUND_UP(i_size_read(mapping->host), PAGE_SIZE);
		if (shmem_mapping(mapping))
			end = shmem_fallocend(mapping->host, end);
	}

	/*
	 * Racy check if we can split the page, before unmap_folio() will
	 * split PMDs
	 */
	if (!can_split_folio(folio, &extra_pins)) {
		ret = -EAGAIN;
		goto out_unlock;
	}

	unmap_folio(folio);

	/* block interrupt reentry in xa_lock and spinlock */
	local_irq_disable();
	if (mapping) {
		/*
		 * Check if the folio is present in page cache.
		 * We assume all tail are present too, if folio is there.
		 */
		xas_lock(&xas);
		xas_reset(&xas);
		if (xas_load(&xas) != folio)
			goto fail;
	}

	/* Prevent deferred_split_scan() touching ->_refcount */
	spin_lock(&ds_queue->split_queue_lock);
	if (folio_ref_freeze(folio, 1 + extra_pins)) {
		if (folio_order(folio) > 1 &&
		    !list_empty(&folio->_deferred_list)) {
			ds_queue->split_queue_len--;
			/*
			 * Reinitialize page_deferred_list after removing the
			 * page from the split_queue, otherwise a subsequent
			 * split will see list corruption when checking the
			 * page_deferred_list.
			 */
			list_del_init(&folio->_deferred_list);
		}
		spin_unlock(&ds_queue->split_queue_lock);
		if (mapping) {
			int nr = folio_nr_pages(folio);

			xas_split(&xas, folio, folio_order(folio));
			if (folio_test_pmd_mappable(folio) &&
			    new_order < HPAGE_PMD_ORDER) {
				if (folio_test_swapbacked(folio)) {
					__lruvec_stat_mod_folio(folio,
							NR_SHMEM_THPS, -nr);
				} else {
					__lruvec_stat_mod_folio(folio,
							NR_FILE_THPS, -nr);
					filemap_nr_thps_dec(mapping);
				}
			}
		}

		__split_huge_page(page, list, end, new_order);
		ret = 0;
	} else {
		spin_unlock(&ds_queue->split_queue_lock);
fail:
		if (mapping)
			xas_unlock(&xas);
		local_irq_enable();
		remap_page(folio, folio_nr_pages(folio));
		ret = -EAGAIN;
	}

out_unlock:
	if (anon_vma) {
		anon_vma_unlock_write(anon_vma);
		put_anon_vma(anon_vma);
	}
	if (mapping)
		i_mmap_unlock_read(mapping);
out:
	xas_destroy(&xas);
	count_vm_event(!ret ? THP_SPLIT_PAGE : THP_SPLIT_PAGE_FAILED);
	return ret;
}

void folio_undo_large_rmappable(struct folio *folio)
{
	struct deferred_split *ds_queue;
	unsigned long flags;

	if (folio_order(folio) <= 1)
		return;

	/*
	 * At this point, there is no one trying to add the folio to
	 * deferred_list. If folio is not in deferred_list, it's safe
	 * to check without acquiring the split_queue_lock.
	 */
	if (data_race(list_empty(&folio->_deferred_list)))
		return;

	ds_queue = get_deferred_split_queue(folio);
	spin_lock_irqsave(&ds_queue->split_queue_lock, flags);
	if (!list_empty(&folio->_deferred_list)) {
		ds_queue->split_queue_len--;
		list_del_init(&folio->_deferred_list);
	}
	spin_unlock_irqrestore(&ds_queue->split_queue_lock, flags);
}

void deferred_split_folio(struct folio *folio)
{
	struct deferred_split *ds_queue = get_deferred_split_queue(folio);
#ifdef CONFIG_MEMCG
	struct mem_cgroup *memcg = folio_memcg(folio);
#endif
	unsigned long flags;

	/*
	 * Order 1 folios have no space for a deferred list, but we also
	 * won't waste much memory by not adding them to the deferred list.
	 */
	if (folio_order(folio) <= 1)
		return;

	/*
	 * The try_to_unmap() in page reclaim path might reach here too,
	 * this may cause a race condition to corrupt deferred split queue.
	 * And, if page reclaim is already handling the same folio, it is
	 * unnecessary to handle it again in shrinker.
	 *
	 * Check the swapcache flag to determine if the folio is being
	 * handled by page reclaim since THP swap would add the folio into
	 * swap cache before calling try_to_unmap().
	 */
	if (folio_test_swapcache(folio))
		return;

	if (!list_empty(&folio->_deferred_list))
		return;

	spin_lock_irqsave(&ds_queue->split_queue_lock, flags);
	if (list_empty(&folio->_deferred_list)) {
		count_vm_event(THP_DEFERRED_SPLIT_PAGE);
		list_add_tail(&folio->_deferred_list, &ds_queue->split_queue);
		ds_queue->split_queue_len++;
#ifdef CONFIG_MEMCG
		if (memcg)
			set_shrinker_bit(memcg, folio_nid(folio),
					 deferred_split_shrinker->id);
#endif
	}
	spin_unlock_irqrestore(&ds_queue->split_queue_lock, flags);
}

static unsigned long deferred_split_count(struct shrinker *shrink,
		struct shrink_control *sc)
{
	struct pglist_data *pgdata = NODE_DATA(sc->nid);
	struct deferred_split *ds_queue = &pgdata->deferred_split_queue;

#ifdef CONFIG_MEMCG
	if (sc->memcg)
		ds_queue = &sc->memcg->deferred_split_queue;
#endif
	return READ_ONCE(ds_queue->split_queue_len);
}

static unsigned long deferred_split_scan(struct shrinker *shrink,
		struct shrink_control *sc)
{
	struct pglist_data *pgdata = NODE_DATA(sc->nid);
	struct deferred_split *ds_queue = &pgdata->deferred_split_queue;
	unsigned long flags;
	LIST_HEAD(list);
	struct folio *folio, *next;
	int split = 0;

#ifdef CONFIG_MEMCG
	if (sc->memcg)
		ds_queue = &sc->memcg->deferred_split_queue;
#endif

	spin_lock_irqsave(&ds_queue->split_queue_lock, flags);
	/* Take pin on all head pages to avoid freeing them under us */
	list_for_each_entry_safe(folio, next, &ds_queue->split_queue,
							_deferred_list) {
		if (folio_try_get(folio)) {
			list_move(&folio->_deferred_list, &list);
		} else {
			/* We lost race with folio_put() */
			list_del_init(&folio->_deferred_list);
			ds_queue->split_queue_len--;
		}
		if (!--sc->nr_to_scan)
			break;
	}
	spin_unlock_irqrestore(&ds_queue->split_queue_lock, flags);

	list_for_each_entry_safe(folio, next, &list, _deferred_list) {
		if (!folio_trylock(folio))
			goto next;
		/* split_huge_page() removes page from list on success */
		if (!split_folio(folio))
			split++;
		folio_unlock(folio);
next:
		folio_put(folio);
	}

	spin_lock_irqsave(&ds_queue->split_queue_lock, flags);
	list_splice_tail(&list, &ds_queue->split_queue);
	spin_unlock_irqrestore(&ds_queue->split_queue_lock, flags);

	/*
	 * Stop shrinker if we didn't split any page, but the queue is empty.
	 * This can happen if pages were freed under us.
	 */
	if (!split && list_empty(&ds_queue->split_queue))
		return SHRINK_STOP;
	return split;
}

#ifdef CONFIG_DEBUG_FS
static void split_huge_pages_all(void)
{
	struct zone *zone;
	struct page *page;
	struct folio *folio;
	unsigned long pfn, max_zone_pfn;
	unsigned long total = 0, split = 0;

	pr_debug("Split all THPs\n");
	for_each_zone(zone) {
		if (!managed_zone(zone))
			continue;
		max_zone_pfn = zone_end_pfn(zone);
		for (pfn = zone->zone_start_pfn; pfn < max_zone_pfn; pfn++) {
			int nr_pages;

			page = pfn_to_online_page(pfn);
			if (!page || PageTail(page))
				continue;
			folio = page_folio(page);
			if (!folio_try_get(folio))
				continue;

			if (unlikely(page_folio(page) != folio))
				goto next;

			if (zone != folio_zone(folio))
				goto next;

			if (!folio_test_large(folio)
				|| folio_test_hugetlb(folio)
				|| !folio_test_lru(folio))
				goto next;

			total++;
			folio_lock(folio);
			nr_pages = folio_nr_pages(folio);
			if (!split_folio(folio))
				split++;
			pfn += nr_pages - 1;
			folio_unlock(folio);
next:
			folio_put(folio);
			cond_resched();
		}
	}

	pr_debug("%lu of %lu THP split\n", split, total);
}

static inline bool vma_not_suitable_for_thp_split(struct vm_area_struct *vma)
{
	return vma_is_special_huge(vma) || (vma->vm_flags & VM_IO) ||
		    is_vm_hugetlb_page(vma);
}

static int split_huge_pages_pid(int pid, unsigned long vaddr_start,
				unsigned long vaddr_end, unsigned int new_order)
{
	int ret = 0;
	struct task_struct *task;
	struct mm_struct *mm;
	unsigned long total = 0, split = 0;
	unsigned long addr;

	vaddr_start &= PAGE_MASK;
	vaddr_end &= PAGE_MASK;

	/* Find the task_struct from pid */
	rcu_read_lock();
	task = find_task_by_vpid(pid);
	if (!task) {
		rcu_read_unlock();
		ret = -ESRCH;
		goto out;
	}
	get_task_struct(task);
	rcu_read_unlock();

	/* Find the mm_struct */
	mm = get_task_mm(task);
	put_task_struct(task);

	if (!mm) {
		ret = -EINVAL;
		goto out;
	}

	pr_debug("Split huge pages in pid: %d, vaddr: [0x%lx - 0x%lx]\n",
		 pid, vaddr_start, vaddr_end);

	mmap_read_lock(mm);
	/*
	 * always increase addr by PAGE_SIZE, since we could have a PTE page
	 * table filled with PTE-mapped THPs, each of which is distinct.
	 */
	for (addr = vaddr_start; addr < vaddr_end; addr += PAGE_SIZE) {
		struct vm_area_struct *vma = vma_lookup(mm, addr);
		struct page *page;
		struct folio *folio;

		if (!vma)
			break;

		/* skip special VMA and hugetlb VMA */
		if (vma_not_suitable_for_thp_split(vma)) {
			addr = vma->vm_end;
			continue;
		}

		/* FOLL_DUMP to ignore special (like zero) pages */
		page = follow_page(vma, addr, FOLL_GET | FOLL_DUMP);

		if (IS_ERR_OR_NULL(page))
			continue;

		folio = page_folio(page);
		if (!is_transparent_hugepage(folio))
			goto next;

		if (new_order >= folio_order(folio))
			goto next;

		total++;
		/*
		 * For folios with private, split_huge_page_to_list_to_order()
		 * will try to drop it before split and then check if the folio
		 * can be split or not. So skip the check here.
		 */
		if (!folio_test_private(folio) &&
		    !can_split_folio(folio, NULL))
			goto next;

		if (!folio_trylock(folio))
			goto next;

		if (!split_folio_to_order(folio, new_order))
			split++;

		folio_unlock(folio);
next:
		folio_put(folio);
		cond_resched();
	}
	mmap_read_unlock(mm);
	mmput(mm);

	pr_debug("%lu of %lu THP split\n", split, total);

out:
	return ret;
}

static int split_huge_pages_in_file(const char *file_path, pgoff_t off_start,
				pgoff_t off_end, unsigned int new_order)
{
	struct filename *file;
	struct file *candidate;
	struct address_space *mapping;
	int ret = -EINVAL;
	pgoff_t index;
	int nr_pages = 1;
	unsigned long total = 0, split = 0;

	file = getname_kernel(file_path);
	if (IS_ERR(file))
		return ret;

	candidate = file_open_name(file, O_RDONLY, 0);
	if (IS_ERR(candidate))
		goto out;

	pr_debug("split file-backed THPs in file: %s, page offset: [0x%lx - 0x%lx]\n",
		 file_path, off_start, off_end);

	mapping = candidate->f_mapping;

	for (index = off_start; index < off_end; index += nr_pages) {
		struct folio *folio = filemap_get_folio(mapping, index);

		nr_pages = 1;
		if (IS_ERR(folio))
			continue;

		if (!folio_test_large(folio))
			goto next;

		total++;
		nr_pages = folio_nr_pages(folio);

		if (new_order >= folio_order(folio))
			goto next;

		if (!folio_trylock(folio))
			goto next;

		if (!split_folio_to_order(folio, new_order))
			split++;

		folio_unlock(folio);
next:
		folio_put(folio);
		cond_resched();
	}

	filp_close(candidate, NULL);
	ret = 0;

	pr_debug("%lu of %lu file-backed THP split\n", split, total);
out:
	putname(file);
	return ret;
}

#define MAX_INPUT_BUF_SZ 255

static ssize_t split_huge_pages_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppops)
{
	static DEFINE_MUTEX(split_debug_mutex);
	ssize_t ret;
	/*
	 * hold pid, start_vaddr, end_vaddr, new_order or
	 * file_path, off_start, off_end, new_order
	 */
	char input_buf[MAX_INPUT_BUF_SZ];
	int pid;
	unsigned long vaddr_start, vaddr_end;
	unsigned int new_order = 0;

	ret = mutex_lock_interruptible(&split_debug_mutex);
	if (ret)
		return ret;

	ret = -EFAULT;

	memset(input_buf, 0, MAX_INPUT_BUF_SZ);
	if (copy_from_user(input_buf, buf, min_t(size_t, count, MAX_INPUT_BUF_SZ)))
		goto out;

	input_buf[MAX_INPUT_BUF_SZ - 1] = '\0';

	if (input_buf[0] == '/') {
		char *tok;
		char *buf = input_buf;
		char file_path[MAX_INPUT_BUF_SZ];
		pgoff_t off_start = 0, off_end = 0;
		size_t input_len = strlen(input_buf);

		tok = strsep(&buf, ",");
		if (tok) {
			strcpy(file_path, tok);
		} else {
			ret = -EINVAL;
			goto out;
		}

		ret = sscanf(buf, "0x%lx,0x%lx,%d", &off_start, &off_end, &new_order);
		if (ret != 2 && ret != 3) {
			ret = -EINVAL;
			goto out;
		}
		ret = split_huge_pages_in_file(file_path, off_start, off_end, new_order);
		if (!ret)
			ret = input_len;

		goto out;
	}

	ret = sscanf(input_buf, "%d,0x%lx,0x%lx,%d", &pid, &vaddr_start, &vaddr_end, &new_order);
	if (ret == 1 && pid == 1) {
		split_huge_pages_all();
		ret = strlen(input_buf);
		goto out;
	} else if (ret != 3 && ret != 4) {
		ret = -EINVAL;
		goto out;
	}

	ret = split_huge_pages_pid(pid, vaddr_start, vaddr_end, new_order);
	if (!ret)
		ret = strlen(input_buf);
out:
	mutex_unlock(&split_debug_mutex);
	return ret;

}

static const struct file_operations split_huge_pages_fops = {
	.owner	 = THIS_MODULE,
	.write	 = split_huge_pages_write,
	.llseek  = no_llseek,
};

static int __init split_huge_pages_debugfs(void)
{
	debugfs_create_file("split_huge_pages", 0200, NULL, NULL,
			    &split_huge_pages_fops);
	return 0;
}
late_initcall(split_huge_pages_debugfs);
#endif

#ifdef CONFIG_ARCH_ENABLE_THP_MIGRATION
int set_pmd_migration_entry(struct page_vma_mapped_walk *pvmw,
		struct page *page)
{
	struct folio *folio = page_folio(page);
	struct vm_area_struct *vma = pvmw->vma;
	struct mm_struct *mm = vma->vm_mm;
	unsigned long address = pvmw->address;
	bool anon_exclusive;
	pmd_t pmdval;
	swp_entry_t entry;
	pmd_t pmdswp;

	if (!(pvmw->pmd && !pvmw->pte))
		return 0;

	flush_cache_range(vma, address, address + HPAGE_PMD_SIZE);
	pmdval = pmdp_invalidate(vma, address, pvmw->pmd);

	/* See folio_try_share_anon_rmap_pmd(): invalidate PMD first. */
	anon_exclusive = folio_test_anon(folio) && PageAnonExclusive(page);
	if (anon_exclusive && folio_try_share_anon_rmap_pmd(folio, page)) {
		set_pmd_at(mm, address, pvmw->pmd, pmdval);
		return -EBUSY;
	}

	if (pmd_dirty(pmdval))
		folio_mark_dirty(folio);
	if (pmd_write(pmdval))
		entry = make_writable_migration_entry(page_to_pfn(page));
	else if (anon_exclusive)
		entry = make_readable_exclusive_migration_entry(page_to_pfn(page));
	else
		entry = make_readable_migration_entry(page_to_pfn(page));
	if (pmd_young(pmdval))
		entry = make_migration_entry_young(entry);
	if (pmd_dirty(pmdval))
		entry = make_migration_entry_dirty(entry);
	pmdswp = swp_entry_to_pmd(entry);
	if (pmd_soft_dirty(pmdval))
		pmdswp = pmd_swp_mksoft_dirty(pmdswp);
	if (pmd_uffd_wp(pmdval))
		pmdswp = pmd_swp_mkuffd_wp(pmdswp);
	set_pmd_at(mm, address, pvmw->pmd, pmdswp);
	folio_remove_rmap_pmd(folio, page, vma);
	folio_put(folio);
	trace_set_migration_pmd(address, pmd_val(pmdswp));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
void __split_huge_page_pmd(struct mm_struct *mm, pmd_t *pmd)
{
	struct page *page;

	spin_lock(&mm->page_table_lock);
	if (unlikely(!pmd_trans_huge(*pmd))) {
		spin_unlock(&mm->page_table_lock);
		return;
	}
	page = pmd_page(*pmd);
	VM_BUG_ON(!page_count(page));
	get_page(page);
	spin_unlock(&mm->page_table_lock);

	split_huge_page(page);

	put_page(page);
	BUG_ON(pmd_trans_huge(*pmd));
}

static void split_huge_page_address(struct mm_struct *mm,
				    unsigned long address)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;

	VM_BUG_ON(!(address & ~HPAGE_PMD_MASK));

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		return;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		return;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd))
		return;
	/*
	 * Caller holds the mmap_sem write mode, so a huge pmd cannot
	 * materialize from under us.
	 */
	split_huge_page_pmd(mm, pmd);
}

void __vma_adjust_trans_huge(struct vm_area_struct *vma,
			     unsigned long start,
			     unsigned long end,
			     long adjust_next)
{
	/*
	 * If the new start address isn't hpage aligned and it could
	 * previously contain an hugepage: check if we need to split
	 * an huge pmd.
	 */
	if (start & ~HPAGE_PMD_MASK &&
	    (start & HPAGE_PMD_MASK) >= vma->vm_start &&
	    (start & HPAGE_PMD_MASK) + HPAGE_PMD_SIZE <= vma->vm_end)
		split_huge_page_address(vma->vm_mm, start);

	/*
	 * If the new end address isn't hpage aligned and it could
	 * previously contain an hugepage: check if we need to split
	 * an huge pmd.
	 */
	if (end & ~HPAGE_PMD_MASK &&
	    (end & HPAGE_PMD_MASK) >= vma->vm_start &&
	    (end & HPAGE_PMD_MASK) + HPAGE_PMD_SIZE <= vma->vm_end)
		split_huge_page_address(vma->vm_mm, end);

	/*
	 * If we're also updating the vma->vm_next->vm_start, if the new
	 * vm_next->vm_start isn't page aligned and it could previously
	 * contain an hugepage: check if we need to split an huge pmd.
	 */
	if (adjust_next > 0) {
		struct vm_area_struct *next = vma->vm_next;
		unsigned long nstart = next->vm_start;
		nstart += adjust_next << PAGE_SHIFT;
		if (nstart & ~HPAGE_PMD_MASK &&
		    (nstart & HPAGE_PMD_MASK) >= next->vm_start &&
		    (nstart & HPAGE_PMD_MASK) + HPAGE_PMD_SIZE <= next->vm_end)
			split_huge_page_address(next->vm_mm, nstart);
	}
}
=======
void remove_migration_pmd(struct page_vma_mapped_walk *pvmw, struct page *new)
{
	struct folio *folio = page_folio(new);
	struct vm_area_struct *vma = pvmw->vma;
	struct mm_struct *mm = vma->vm_mm;
	unsigned long address = pvmw->address;
	unsigned long haddr = address & HPAGE_PMD_MASK;
	pmd_t pmde;
	swp_entry_t entry;

	if (!(pvmw->pmd && !pvmw->pte))
		return;

	entry = pmd_to_swp_entry(*pvmw->pmd);
	folio_get(folio);
	pmde = mk_huge_pmd(new, READ_ONCE(vma->vm_page_prot));
	if (pmd_swp_soft_dirty(*pvmw->pmd))
		pmde = pmd_mksoft_dirty(pmde);
	if (is_writable_migration_entry(entry))
		pmde = pmd_mkwrite(pmde, vma);
	if (pmd_swp_uffd_wp(*pvmw->pmd))
		pmde = pmd_mkuffd_wp(pmde);
	if (!is_migration_entry_young(entry))
		pmde = pmd_mkold(pmde);
	/* NOTE: this may contain setting soft-dirty on some archs */
	if (folio_test_dirty(folio) && is_migration_entry_dirty(entry))
		pmde = pmd_mkdirty(pmde);

	if (folio_test_anon(folio)) {
		rmap_t rmap_flags = RMAP_NONE;

		if (!is_readable_migration_entry(entry))
			rmap_flags |= RMAP_EXCLUSIVE;

		folio_add_anon_rmap_pmd(folio, new, vma, haddr, rmap_flags);
	} else {
		folio_add_file_rmap_pmd(folio, new, vma);
	}
	VM_BUG_ON(pmd_write(pmde) && folio_test_anon(folio) && !PageAnonExclusive(new));
	set_pmd_at(mm, haddr, pvmw->pmd, pmde);

	/* No need to invalidate - it was non-present before */
	update_mmu_cache_pmd(vma, address, pvmw->pmd);
	trace_remove_migration_pmd(address, pmd_val(pmde));
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
