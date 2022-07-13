<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * mm/mmap.c
 *
 * Written by obz.
 *
 * Address space accounting code	<alan@lxorguk.ukuu.org.uk>
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/backing-dev.h>
#include <linux/mm.h>
<<<<<<< HEAD
=======
#include <linux/mm_inline.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/shm.h>
#include <linux/mman.h>
#include <linux/pagemap.h>
#include <linux/swap.h>
#include <linux/syscalls.h>
#include <linux/capability.h>
#include <linux/init.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/personality.h>
#include <linux/security.h>
#include <linux/hugetlb.h>
<<<<<<< HEAD
=======
#include <linux/shmem_fs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/profile.h>
#include <linux/export.h>
#include <linux/mount.h>
#include <linux/mempolicy.h>
#include <linux/rmap.h>
#include <linux/mmu_notifier.h>
<<<<<<< HEAD
#include <linux/perf_event.h>
#include <linux/audit.h>
#include <linux/khugepaged.h>

#include <asm/uaccess.h>
=======
#include <linux/mmdebug.h>
#include <linux/perf_event.h>
#include <linux/audit.h>
#include <linux/khugepaged.h>
#include <linux/uprobes.h>
#include <linux/notifier.h>
#include <linux/memory.h>
#include <linux/printk.h>
#include <linux/userfaultfd_k.h>
#include <linux/moduleparam.h>
#include <linux/pkeys.h>
#include <linux/oom.h>
#include <linux/sched/mm.h>
#include <linux/ksm.h>

#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/cacheflush.h>
#include <asm/tlb.h>
#include <asm/mmu_context.h>

<<<<<<< HEAD
=======
#define CREATE_TRACE_POINTS
#include <trace/events/mmap.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "internal.h"

#ifndef arch_mmap_check
#define arch_mmap_check(addr, len, flags)	(0)
#endif

<<<<<<< HEAD
#ifndef arch_rebalance_pgtables
#define arch_rebalance_pgtables(addr, len)		(addr)
#endif

#ifdef CONFIG_HAVE_ARCH_MMAP_RND_BITS
const int mmap_rnd_bits_min = CONFIG_ARCH_MMAP_RND_BITS_MIN;
const int mmap_rnd_bits_max = CONFIG_ARCH_MMAP_RND_BITS_MAX;
=======
#ifdef CONFIG_HAVE_ARCH_MMAP_RND_BITS
const int mmap_rnd_bits_min = CONFIG_ARCH_MMAP_RND_BITS_MIN;
int mmap_rnd_bits_max __ro_after_init = CONFIG_ARCH_MMAP_RND_BITS_MAX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int mmap_rnd_bits __read_mostly = CONFIG_ARCH_MMAP_RND_BITS;
#endif
#ifdef CONFIG_HAVE_ARCH_MMAP_RND_COMPAT_BITS
const int mmap_rnd_compat_bits_min = CONFIG_ARCH_MMAP_RND_COMPAT_BITS_MIN;
const int mmap_rnd_compat_bits_max = CONFIG_ARCH_MMAP_RND_COMPAT_BITS_MAX;
int mmap_rnd_compat_bits __read_mostly = CONFIG_ARCH_MMAP_RND_COMPAT_BITS;
#endif

<<<<<<< HEAD

static void unmap_region(struct mm_struct *mm,
		struct vm_area_struct *vma, struct vm_area_struct *prev,
		unsigned long start, unsigned long end);

/*
 * WARNING: the debugging will use recursive algorithms so never enable this
 * unless you know what you are doing.
 */
#undef DEBUG_MM_RB

/* description of effects of mapping type and prot in current implementation.
 * this is due to the limited x86 page protection hardware.  The expected
 * behavior is in parens:
 *
 * map_type	prot
 *		PROT_NONE	PROT_READ	PROT_WRITE	PROT_EXEC
 * MAP_SHARED	r: (no) no	r: (yes) yes	r: (no) yes	r: (no) yes
 *		w: (no) no	w: (no) no	w: (yes) yes	w: (no) no
 *		x: (no) no	x: (no) yes	x: (no) yes	x: (yes) yes
 *		
 * MAP_PRIVATE	r: (no) no	r: (yes) yes	r: (no) yes	r: (no) yes
 *		w: (no) no	w: (no) no	w: (copy) copy	w: (no) no
 *		x: (no) no	x: (no) yes	x: (no) yes	x: (yes) yes
 *
 */
pgprot_t protection_map[16] = {
	__P000, __P001, __P010, __P011, __P100, __P101, __P110, __P111,
	__S000, __S001, __S010, __S011, __S100, __S101, __S110, __S111
};

pgprot_t vm_get_page_prot(unsigned long vm_flags)
{
	return __pgprot(pgprot_val(protection_map[vm_flags &
				(VM_READ|VM_WRITE|VM_EXEC|VM_SHARED)]) |
			pgprot_val(arch_vm_get_page_prot(vm_flags)));
}
EXPORT_SYMBOL(vm_get_page_prot);

int sysctl_overcommit_memory __read_mostly = OVERCOMMIT_GUESS;  /* heuristic overcommit */
int sysctl_overcommit_ratio __read_mostly = 50;	/* default is 50% */
int sysctl_max_map_count __read_mostly = DEFAULT_MAX_MAP_COUNT;
/*
 * Make sure vm_committed_as in one cacheline and not cacheline shared with
 * other variables. It can be updated by several CPUs frequently.
 */
struct percpu_counter vm_committed_as ____cacheline_aligned_in_smp;

/*
 * Check that a process has enough memory to allocate a new virtual
 * mapping. 0 means there is enough memory for the allocation to
 * succeed and -ENOMEM implies there is not.
 *
 * We currently support three overcommit policies, which are set via the
 * vm.overcommit_memory sysctl.  See Documentation/vm/overcommit-accounting
 *
 * Strict overcommit modes added 2002 Feb 26 by Alan Cox.
 * Additional code 2002 Jul 20 by Robert Love.
 *
 * cap_sys_admin is 1 if the process has admin privileges, 0 otherwise.
 *
 * Note this is a helper function intended to be used by LSMs which
 * wish to use this logic.
 */
int __vm_enough_memory(struct mm_struct *mm, long pages, int cap_sys_admin)
{
	long free, allowed;

	vm_acct_memory(pages);

	/*
	 * Sometimes we want to use more memory than we have
	 */
	if (sysctl_overcommit_memory == OVERCOMMIT_ALWAYS)
		return 0;

	if (sysctl_overcommit_memory == OVERCOMMIT_GUESS) {
		free = global_page_state(NR_FREE_PAGES);
		free += global_page_state(NR_FILE_PAGES);

		/*
		 * shmem pages shouldn't be counted as free in this
		 * case, they can't be purged, only swapped out, and
		 * that won't affect the overall amount of available
		 * memory in the system.
		 */
		free -= global_page_state(NR_SHMEM);

		free += get_nr_swap_pages();

		/*
		 * Any slabs which are created with the
		 * SLAB_RECLAIM_ACCOUNT flag claim to have contents
		 * which are reclaimable, under pressure.  The dentry
		 * cache and most inode caches should fall into this
		 */
		free += global_page_state(NR_SLAB_RECLAIMABLE);

		/*
		 * Leave reserved pages. The pages are not for anonymous pages.
		 */
		if (free <= totalreserve_pages)
			goto error;
		else
			free -= totalreserve_pages;

		/*
		 * Leave the last 3% for root
		 */
		if (!cap_sys_admin)
			free -= free / 32;

		if (free > pages)
			return 0;

		goto error;
	}

	allowed = (totalram_pages - hugetlb_total_pages())
	       	* sysctl_overcommit_ratio / 100;
	/*
	 * Leave the last 3% for root
	 */
	if (!cap_sys_admin)
		allowed -= allowed / 32;
	allowed += total_swap_pages;

	/* Don't let a single process grow too big:
	   leave 3% of the size of this process for other processes */
	if (mm)
		allowed -= mm->total_vm / 32;

	if (percpu_counter_read_positive(&vm_committed_as) < allowed)
		return 0;
error:
	vm_unacct_memory(pages);

	return -ENOMEM;
}

/*
 * Requires inode->i_mapping->i_mmap_mutex
 */
static void __remove_shared_vm_struct(struct vm_area_struct *vma,
		struct file *file, struct address_space *mapping)
{
	if (vma->vm_flags & VM_DENYWRITE)
		atomic_inc(&file->f_path.dentry->d_inode->i_writecount);
	if (vma->vm_flags & VM_SHARED)
		mapping->i_mmap_writable--;

	flush_dcache_mmap_lock(mapping);
	if (unlikely(vma->vm_flags & VM_NONLINEAR))
		list_del_init(&vma->shared.vm_set.list);
	else
		vma_prio_tree_remove(vma, &mapping->i_mmap);
=======
static bool ignore_rlimit_data;
core_param(ignore_rlimit_data, ignore_rlimit_data, bool, 0644);

static void unmap_region(struct mm_struct *mm, struct ma_state *mas,
		struct vm_area_struct *vma, struct vm_area_struct *prev,
		struct vm_area_struct *next, unsigned long start,
		unsigned long end, unsigned long tree_end, bool mm_wr_locked);

static pgprot_t vm_pgprot_modify(pgprot_t oldprot, unsigned long vm_flags)
{
	return pgprot_modify(oldprot, vm_get_page_prot(vm_flags));
}

/* Update vma->vm_page_prot to reflect vma->vm_flags. */
void vma_set_page_prot(struct vm_area_struct *vma)
{
	unsigned long vm_flags = vma->vm_flags;
	pgprot_t vm_page_prot;

	vm_page_prot = vm_pgprot_modify(vma->vm_page_prot, vm_flags);
	if (vma_wants_writenotify(vma, vm_page_prot)) {
		vm_flags &= ~VM_SHARED;
		vm_page_prot = vm_pgprot_modify(vm_page_prot, vm_flags);
	}
	/* remove_protection_ptes reads vma->vm_page_prot without mmap_lock */
	WRITE_ONCE(vma->vm_page_prot, vm_page_prot);
}

/*
 * Requires inode->i_mapping->i_mmap_rwsem
 */
static void __remove_shared_vm_struct(struct vm_area_struct *vma,
				      struct address_space *mapping)
{
	if (vma_is_shared_maywrite(vma))
		mapping_unmap_writable(mapping);

	flush_dcache_mmap_lock(mapping);
	vma_interval_tree_remove(vma, &mapping->i_mmap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	flush_dcache_mmap_unlock(mapping);
}

/*
<<<<<<< HEAD
 * Unlink a file-based vm structure from its prio_tree, to hide
=======
 * Unlink a file-based vm structure from its interval tree, to hide
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * vma from rmap and vmtruncate before freeing its page tables.
 */
void unlink_file_vma(struct vm_area_struct *vma)
{
	struct file *file = vma->vm_file;

	if (file) {
		struct address_space *mapping = file->f_mapping;
<<<<<<< HEAD
		mutex_lock(&mapping->i_mmap_mutex);
		__remove_shared_vm_struct(vma, file, mapping);
		mutex_unlock(&mapping->i_mmap_mutex);
=======
		i_mmap_lock_write(mapping);
		__remove_shared_vm_struct(vma, mapping);
		i_mmap_unlock_write(mapping);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
<<<<<<< HEAD
 * Close a vm structure and free it, returning the next.
 */
static struct vm_area_struct *remove_vma(struct vm_area_struct *vma)
{
	struct vm_area_struct *next = vma->vm_next;

	might_sleep();
	if (vma->vm_ops && vma->vm_ops->close)
		vma->vm_ops->close(vma);
	if (vma->vm_file) {
		fput(vma->vm_file);
		if (vma->vm_flags & VM_EXECUTABLE)
			removed_exe_file_vma(vma->vm_mm);
	}
	mpol_put(vma_policy(vma));
	kmem_cache_free(vm_area_cachep, vma);
	return next;
}

static unsigned long do_brk(unsigned long addr, unsigned long len);

SYSCALL_DEFINE1(brk, unsigned long, brk)
{
	unsigned long rlim, retval;
	unsigned long newbrk, oldbrk;
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *next;
	unsigned long min_brk;

	down_write(&mm->mmap_sem);
=======
 * Close a vm structure and free it.
 */
static void remove_vma(struct vm_area_struct *vma, bool unreachable)
{
	might_sleep();
	if (vma->vm_ops && vma->vm_ops->close)
		vma->vm_ops->close(vma);
	if (vma->vm_file)
		fput(vma->vm_file);
	mpol_put(vma_policy(vma));
	if (unreachable)
		__vm_area_free(vma);
	else
		vm_area_free(vma);
}

static inline struct vm_area_struct *vma_prev_limit(struct vma_iterator *vmi,
						    unsigned long min)
{
	return mas_prev(&vmi->mas, min);
}

/*
 * check_brk_limits() - Use platform specific check of range & verify mlock
 * limits.
 * @addr: The address to check
 * @len: The size of increase.
 *
 * Return: 0 on success.
 */
static int check_brk_limits(unsigned long addr, unsigned long len)
{
	unsigned long mapped_addr;

	mapped_addr = get_unmapped_area(NULL, addr, len, 0, MAP_FIXED);
	if (IS_ERR_VALUE(mapped_addr))
		return mapped_addr;

	return mlock_future_ok(current->mm, current->mm->def_flags, len)
		? 0 : -EAGAIN;
}
static int do_brk_flags(struct vma_iterator *vmi, struct vm_area_struct *brkvma,
		unsigned long addr, unsigned long request, unsigned long flags);
SYSCALL_DEFINE1(brk, unsigned long, brk)
{
	unsigned long newbrk, oldbrk, origbrk;
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *brkvma, *next = NULL;
	unsigned long min_brk;
	bool populate = false;
	LIST_HEAD(uf);
	struct vma_iterator vmi;

	if (mmap_write_lock_killable(mm))
		return -EINTR;

	origbrk = mm->brk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_COMPAT_BRK
	/*
	 * CONFIG_COMPAT_BRK can still be overridden by setting
	 * randomize_va_space to 2, which will still cause mm->start_brk
	 * to be arbitrarily shifted
	 */
	if (current->brk_randomized)
		min_brk = mm->start_brk;
	else
		min_brk = mm->end_data;
#else
	min_brk = mm->start_brk;
#endif
	if (brk < min_brk)
		goto out;

	/*
	 * Check against rlimit here. If this check is done later after the test
	 * of oldbrk with newbrk then it can escape the test and let the data
	 * segment grow beyond its set limit the in case where the limit is
	 * not page aligned -Ram Gupta
	 */
<<<<<<< HEAD
	rlim = rlimit(RLIMIT_DATA);
	if (rlim < RLIM_INFINITY && (brk - mm->start_brk) +
			(mm->end_data - mm->start_data) > rlim)
=======
	if (check_data_rlimit(rlimit(RLIMIT_DATA), brk, mm->start_brk,
			      mm->end_data, mm->start_data))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	newbrk = PAGE_ALIGN(brk);
	oldbrk = PAGE_ALIGN(mm->brk);
<<<<<<< HEAD
	if (oldbrk == newbrk)
		goto set_brk;

	/* Always allow shrinking brk. */
	if (brk <= mm->brk) {
		if (!do_munmap(mm, newbrk, oldbrk-newbrk))
			goto set_brk;
		goto out;
	}

	/* Check against existing mmap mappings. */
	next = find_vma(mm, oldbrk);
	if (next && newbrk + PAGE_SIZE > vm_start_gap(next))
		goto out;

	/* Ok, looks good - let it rip. */
	if (do_brk(oldbrk, newbrk-oldbrk) != oldbrk)
		goto out;
set_brk:
	mm->brk = brk;
out:
	retval = mm->brk;
	up_write(&mm->mmap_sem);
	return retval;
}

#ifdef DEBUG_MM_RB
static int browse_rb(struct rb_root *root)
{
	int i = 0, j;
	struct rb_node *nd, *pn = NULL;
	unsigned long prev = 0, pend = 0;

	for (nd = rb_first(root); nd; nd = rb_next(nd)) {
		struct vm_area_struct *vma;
		vma = rb_entry(nd, struct vm_area_struct, vm_rb);
		if (vma->vm_start < prev)
			printk("vm_start %lx prev %lx\n", vma->vm_start, prev), i = -1;
		if (vma->vm_start < pend)
			printk("vm_start %lx pend %lx\n", vma->vm_start, pend);
		if (vma->vm_start > vma->vm_end)
			printk("vm_end %lx < vm_start %lx\n", vma->vm_end, vma->vm_start);
		i++;
		pn = nd;
		prev = vma->vm_start;
		pend = vma->vm_end;
	}
	j = 0;
	for (nd = pn; nd; nd = rb_prev(nd)) {
		j++;
	}
	if (i != j)
		printk("backwards %d, forwards %d\n", j, i), i = 0;
	return i;
}

void validate_mm(struct mm_struct *mm)
{
	int bug = 0;
	int i = 0;
	struct vm_area_struct *tmp = mm->mmap;
	while (tmp) {
		tmp = tmp->vm_next;
		i++;
	}
	if (i != mm->map_count)
		printk("map_count %d vm_next %d\n", mm->map_count, i), bug = 1;
	i = browse_rb(&mm->mm_rb);
	if (i != mm->map_count)
		printk("map_count %d rb %d\n", mm->map_count, i), bug = 1;
	BUG_ON(bug);
}
#else
#define validate_mm(mm) do { } while (0)
#endif

static struct vm_area_struct *
find_vma_prepare(struct mm_struct *mm, unsigned long addr,
		struct vm_area_struct **pprev, struct rb_node ***rb_link,
		struct rb_node ** rb_parent)
{
	struct vm_area_struct * vma;
	struct rb_node ** __rb_link, * __rb_parent, * rb_prev;

	__rb_link = &mm->mm_rb.rb_node;
	rb_prev = __rb_parent = NULL;
	vma = NULL;

	while (*__rb_link) {
		struct vm_area_struct *vma_tmp;

		__rb_parent = *__rb_link;
		vma_tmp = rb_entry(__rb_parent, struct vm_area_struct, vm_rb);

		if (vma_tmp->vm_end > addr) {
			vma = vma_tmp;
			if (vma_tmp->vm_start <= addr)
				break;
			__rb_link = &__rb_parent->rb_left;
		} else {
			rb_prev = __rb_parent;
			__rb_link = &__rb_parent->rb_right;
		}
	}

	*pprev = NULL;
	if (rb_prev)
		*pprev = rb_entry(rb_prev, struct vm_area_struct, vm_rb);
	*rb_link = __rb_link;
	*rb_parent = __rb_parent;
	return vma;
=======
	if (oldbrk == newbrk) {
		mm->brk = brk;
		goto success;
	}

	/* Always allow shrinking brk. */
	if (brk <= mm->brk) {
		/* Search one past newbrk */
		vma_iter_init(&vmi, mm, newbrk);
		brkvma = vma_find(&vmi, oldbrk);
		if (!brkvma || brkvma->vm_start >= oldbrk)
			goto out; /* mapping intersects with an existing non-brk vma. */
		/*
		 * mm->brk must be protected by write mmap_lock.
		 * do_vma_munmap() will drop the lock on success,  so update it
		 * before calling do_vma_munmap().
		 */
		mm->brk = brk;
		if (do_vma_munmap(&vmi, brkvma, newbrk, oldbrk, &uf, true))
			goto out;

		goto success_unlocked;
	}

	if (check_brk_limits(oldbrk, newbrk - oldbrk))
		goto out;

	/*
	 * Only check if the next VMA is within the stack_guard_gap of the
	 * expansion area
	 */
	vma_iter_init(&vmi, mm, oldbrk);
	next = vma_find(&vmi, newbrk + PAGE_SIZE + stack_guard_gap);
	if (next && newbrk + PAGE_SIZE > vm_start_gap(next))
		goto out;

	brkvma = vma_prev_limit(&vmi, mm->start_brk);
	/* Ok, looks good - let it rip. */
	if (do_brk_flags(&vmi, brkvma, oldbrk, newbrk - oldbrk, 0) < 0)
		goto out;

	mm->brk = brk;
	if (mm->def_flags & VM_LOCKED)
		populate = true;

success:
	mmap_write_unlock(mm);
success_unlocked:
	userfaultfd_unmap_complete(mm, &uf);
	if (populate)
		mm_populate(oldbrk, newbrk - oldbrk);
	return brk;

out:
	mm->brk = origbrk;
	mmap_write_unlock(mm);
	return origbrk;
}

#if defined(CONFIG_DEBUG_VM_MAPLE_TREE)
static void validate_mm(struct mm_struct *mm)
{
	int bug = 0;
	int i = 0;
	struct vm_area_struct *vma;
	VMA_ITERATOR(vmi, mm, 0);

	mt_validate(&mm->mm_mt);
	for_each_vma(vmi, vma) {
#ifdef CONFIG_DEBUG_VM_RB
		struct anon_vma *anon_vma = vma->anon_vma;
		struct anon_vma_chain *avc;
#endif
		unsigned long vmi_start, vmi_end;
		bool warn = 0;

		vmi_start = vma_iter_addr(&vmi);
		vmi_end = vma_iter_end(&vmi);
		if (VM_WARN_ON_ONCE_MM(vma->vm_end != vmi_end, mm))
			warn = 1;

		if (VM_WARN_ON_ONCE_MM(vma->vm_start != vmi_start, mm))
			warn = 1;

		if (warn) {
			pr_emerg("issue in %s\n", current->comm);
			dump_stack();
			dump_vma(vma);
			pr_emerg("tree range: %px start %lx end %lx\n", vma,
				 vmi_start, vmi_end - 1);
			vma_iter_dump_tree(&vmi);
		}

#ifdef CONFIG_DEBUG_VM_RB
		if (anon_vma) {
			anon_vma_lock_read(anon_vma);
			list_for_each_entry(avc, &vma->anon_vma_chain, same_vma)
				anon_vma_interval_tree_verify(avc);
			anon_vma_unlock_read(anon_vma);
		}
#endif
		i++;
	}
	if (i != mm->map_count) {
		pr_emerg("map_count %d vma iterator %d\n", mm->map_count, i);
		bug = 1;
	}
	VM_BUG_ON_MM(bug, mm);
}

#else /* !CONFIG_DEBUG_VM_MAPLE_TREE */
#define validate_mm(mm) do { } while (0)
#endif /* CONFIG_DEBUG_VM_MAPLE_TREE */

/*
 * vma has some anon_vma assigned, and is already inserted on that
 * anon_vma's interval trees.
 *
 * Before updating the vma's vm_start / vm_end / vm_pgoff fields, the
 * vma must be removed from the anon_vma's interval trees using
 * anon_vma_interval_tree_pre_update_vma().
 *
 * After the update, the vma will be reinserted using
 * anon_vma_interval_tree_post_update_vma().
 *
 * The entire update must be protected by exclusive mmap_lock and by
 * the root anon_vma's mutex.
 */
static inline void
anon_vma_interval_tree_pre_update_vma(struct vm_area_struct *vma)
{
	struct anon_vma_chain *avc;

	list_for_each_entry(avc, &vma->anon_vma_chain, same_vma)
		anon_vma_interval_tree_remove(avc, &avc->anon_vma->rb_root);
}

static inline void
anon_vma_interval_tree_post_update_vma(struct vm_area_struct *vma)
{
	struct anon_vma_chain *avc;

	list_for_each_entry(avc, &vma->anon_vma_chain, same_vma)
		anon_vma_interval_tree_insert(avc, &avc->anon_vma->rb_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned long count_vma_pages_range(struct mm_struct *mm,
		unsigned long addr, unsigned long end)
{
<<<<<<< HEAD
	unsigned long nr_pages = 0;
	struct vm_area_struct *vma;

	/* Find first overlaping mapping */
	vma = find_vma_intersection(mm, addr, end);
	if (!vma)
		return 0;

	nr_pages = (min(end, vma->vm_end) -
		max(addr, vma->vm_start)) >> PAGE_SHIFT;

	/* Iterate over the rest of the overlaps */
	for (vma = vma->vm_next; vma; vma = vma->vm_next) {
		unsigned long overlap_len;

		if (vma->vm_start > end)
			break;

		overlap_len = min(end, vma->vm_end) - vma->vm_start;
		nr_pages += overlap_len >> PAGE_SHIFT;
=======
	VMA_ITERATOR(vmi, mm, addr);
	struct vm_area_struct *vma;
	unsigned long nr_pages = 0;

	for_each_vma_range(vmi, vma, end) {
		unsigned long vm_start = max(addr, vma->vm_start);
		unsigned long vm_end = min(end, vma->vm_end);

		nr_pages += PHYS_PFN(vm_end - vm_start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return nr_pages;
}

<<<<<<< HEAD
void __vma_link_rb(struct mm_struct *mm, struct vm_area_struct *vma,
		struct rb_node **rb_link, struct rb_node *rb_parent)
{
	rb_link_node(&vma->vm_rb, rb_parent, rb_link);
	rb_insert_color(&vma->vm_rb, &mm->mm_rb);
}

static void __vma_link_file(struct vm_area_struct *vma)
{
	struct file *file;

	file = vma->vm_file;
	if (file) {
		struct address_space *mapping = file->f_mapping;

		if (vma->vm_flags & VM_DENYWRITE)
			atomic_dec(&file->f_path.dentry->d_inode->i_writecount);
		if (vma->vm_flags & VM_SHARED)
			mapping->i_mmap_writable++;

		flush_dcache_mmap_lock(mapping);
		if (unlikely(vma->vm_flags & VM_NONLINEAR))
			vma_nonlinear_insert(vma, &mapping->i_mmap_nonlinear);
		else
			vma_prio_tree_insert(vma, &mapping->i_mmap);
		flush_dcache_mmap_unlock(mapping);
	}
}

static void
__vma_link(struct mm_struct *mm, struct vm_area_struct *vma,
	struct vm_area_struct *prev, struct rb_node **rb_link,
	struct rb_node *rb_parent)
{
	__vma_link_list(mm, vma, prev, rb_parent);
	__vma_link_rb(mm, vma, rb_link, rb_parent);
}

static void vma_link(struct mm_struct *mm, struct vm_area_struct *vma,
			struct vm_area_struct *prev, struct rb_node **rb_link,
			struct rb_node *rb_parent)
{
	struct address_space *mapping = NULL;

	if (vma->vm_file)
		mapping = vma->vm_file->f_mapping;

	if (mapping)
		mutex_lock(&mapping->i_mmap_mutex);

	__vma_link(mm, vma, prev, rb_link, rb_parent);
	__vma_link_file(vma);

	if (mapping)
		mutex_unlock(&mapping->i_mmap_mutex);

	mm->map_count++;
	validate_mm(mm);
}

/*
 * Helper for vma_adjust() in the split_vma insert case: insert a vma into the
 * mm's list and rbtree.  It has already been inserted into the prio_tree.
 */
static void __insert_vm_struct(struct mm_struct *mm, struct vm_area_struct *vma)
{
	struct vm_area_struct *__vma, *prev;
	struct rb_node **rb_link, *rb_parent;

	__vma = find_vma_prepare(mm, vma->vm_start,&prev, &rb_link, &rb_parent);
	BUG_ON(__vma && __vma->vm_start < vma->vm_end);
	__vma_link(mm, vma, prev, rb_link, rb_parent);
	mm->map_count++;
}

static inline void
__vma_unlink(struct mm_struct *mm, struct vm_area_struct *vma,
		struct vm_area_struct *prev)
{
	struct vm_area_struct *next = vma->vm_next;

	prev->vm_next = next;
	if (next)
		next->vm_prev = prev;
	rb_erase(&vma->vm_rb, &mm->mm_rb);
	if (mm->mmap_cache == vma)
		mm->mmap_cache = prev;
}

/*
 * We cannot adjust vm_start, vm_end, vm_pgoff fields of a vma that
 * is already present in an i_mmap tree without adjusting the tree.
 * The following helper function should be used when such adjustments
 * are necessary.  The "insert" vma (if any) is to be inserted
 * before we drop the necessary locks.
 */
int vma_adjust(struct vm_area_struct *vma, unsigned long start,
	unsigned long end, pgoff_t pgoff, struct vm_area_struct *insert)
{
	struct mm_struct *mm = vma->vm_mm;
	struct vm_area_struct *next = vma->vm_next;
	struct vm_area_struct *importer = NULL;
	struct address_space *mapping = NULL;
	struct prio_tree_root *root = NULL;
	struct anon_vma *anon_vma = NULL;
	struct file *file = vma->vm_file;
	long adjust_next = 0;
	int remove_next = 0;

	if (next && !insert) {
		struct vm_area_struct *exporter = NULL;

		if (end >= next->vm_end) {
			/*
			 * vma expands, overlapping all the next, and
			 * perhaps the one after too (mprotect case 6).
			 */
again:			remove_next = 1 + (end > next->vm_end);
			end = next->vm_end;
			exporter = next;
			importer = vma;
		} else if (end > next->vm_start) {
			/*
			 * vma expands, overlapping part of the next:
			 * mprotect case 5 shifting the boundary up.
			 */
			adjust_next = (end - next->vm_start) >> PAGE_SHIFT;
			exporter = next;
			importer = vma;
		} else if (end < vma->vm_end) {
			/*
			 * vma shrinks, and !insert tells it's not
			 * split_vma inserting another: so it must be
			 * mprotect case 4 shifting the boundary down.
			 */
			adjust_next = - ((vma->vm_end - end) >> PAGE_SHIFT);
			exporter = vma;
			importer = next;
		}

		/*
		 * Easily overlooked: when mprotect shifts the boundary,
		 * make sure the expanding vma has anon_vma set if the
		 * shrinking vma had, to cover any anon pages imported.
		 */
		if (exporter && exporter->anon_vma && !importer->anon_vma) {
			int error;

			importer->anon_vma = exporter->anon_vma;
			error = anon_vma_clone(importer, exporter);
			if (error)
				return error;
		}
	}

	if (file) {
		mapping = file->f_mapping;
		if (!(vma->vm_flags & VM_NONLINEAR))
			root = &mapping->i_mmap;
		mutex_lock(&mapping->i_mmap_mutex);
		if (insert) {
			/*
			 * Put into prio_tree now, so instantiated pages
=======
static void __vma_link_file(struct vm_area_struct *vma,
			    struct address_space *mapping)
{
	if (vma_is_shared_maywrite(vma))
		mapping_allow_writable(mapping);

	flush_dcache_mmap_lock(mapping);
	vma_interval_tree_insert(vma, &mapping->i_mmap);
	flush_dcache_mmap_unlock(mapping);
}

static void vma_link_file(struct vm_area_struct *vma)
{
	struct file *file = vma->vm_file;
	struct address_space *mapping;

	if (file) {
		mapping = file->f_mapping;
		i_mmap_lock_write(mapping);
		__vma_link_file(vma, mapping);
		i_mmap_unlock_write(mapping);
	}
}

static int vma_link(struct mm_struct *mm, struct vm_area_struct *vma)
{
	VMA_ITERATOR(vmi, mm, 0);

	vma_iter_config(&vmi, vma->vm_start, vma->vm_end);
	if (vma_iter_prealloc(&vmi, vma))
		return -ENOMEM;

	vma_start_write(vma);
	vma_iter_store(&vmi, vma);
	vma_link_file(vma);
	mm->map_count++;
	validate_mm(mm);
	return 0;
}

/*
 * init_multi_vma_prep() - Initializer for struct vma_prepare
 * @vp: The vma_prepare struct
 * @vma: The vma that will be altered once locked
 * @next: The next vma if it is to be adjusted
 * @remove: The first vma to be removed
 * @remove2: The second vma to be removed
 */
static inline void init_multi_vma_prep(struct vma_prepare *vp,
		struct vm_area_struct *vma, struct vm_area_struct *next,
		struct vm_area_struct *remove, struct vm_area_struct *remove2)
{
	memset(vp, 0, sizeof(struct vma_prepare));
	vp->vma = vma;
	vp->anon_vma = vma->anon_vma;
	vp->remove = remove;
	vp->remove2 = remove2;
	vp->adj_next = next;
	if (!vp->anon_vma && next)
		vp->anon_vma = next->anon_vma;

	vp->file = vma->vm_file;
	if (vp->file)
		vp->mapping = vma->vm_file->f_mapping;

}

/*
 * init_vma_prep() - Initializer wrapper for vma_prepare struct
 * @vp: The vma_prepare struct
 * @vma: The vma that will be altered once locked
 */
static inline void init_vma_prep(struct vma_prepare *vp,
				 struct vm_area_struct *vma)
{
	init_multi_vma_prep(vp, vma, NULL, NULL, NULL);
}


/*
 * vma_prepare() - Helper function for handling locking VMAs prior to altering
 * @vp: The initialized vma_prepare struct
 */
static inline void vma_prepare(struct vma_prepare *vp)
{
	if (vp->file) {
		uprobe_munmap(vp->vma, vp->vma->vm_start, vp->vma->vm_end);

		if (vp->adj_next)
			uprobe_munmap(vp->adj_next, vp->adj_next->vm_start,
				      vp->adj_next->vm_end);

		i_mmap_lock_write(vp->mapping);
		if (vp->insert && vp->insert->vm_file) {
			/*
			 * Put into interval tree now, so instantiated pages
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 * are visible to arm/parisc __flush_dcache_page
			 * throughout; but we cannot insert into address
			 * space until vma start or end is updated.
			 */
<<<<<<< HEAD
			__vma_link_file(insert);
		}
	}

	vma_adjust_trans_huge(vma, start, end, adjust_next);

	/*
	 * When changing only vma->vm_end, we don't really need anon_vma
	 * lock. This is a fairly rare case by itself, but the anon_vma
	 * lock may be shared between many sibling processes.  Skipping
	 * the lock for brk adjustments makes a difference sometimes.
	 */
	if (vma->anon_vma && (importer || start != vma->vm_start)) {
		anon_vma = vma->anon_vma;
		anon_vma_lock(anon_vma);
	}

	if (root) {
		flush_dcache_mmap_lock(mapping);
		vma_prio_tree_remove(vma, root);
		if (adjust_next)
			vma_prio_tree_remove(next, root);
	}

	vma->vm_start = start;
	vma->vm_end = end;
	vma->vm_pgoff = pgoff;
	if (adjust_next) {
		next->vm_start += adjust_next << PAGE_SHIFT;
		next->vm_pgoff += adjust_next;
	}

	if (root) {
		if (adjust_next)
			vma_prio_tree_insert(next, root);
		vma_prio_tree_insert(vma, root);
		flush_dcache_mmap_unlock(mapping);
	}

	if (remove_next) {
		/*
		 * vma_merge has merged next into vma, and needs
		 * us to remove next before dropping the locks.
		 */
		__vma_unlink(mm, next, vma);
		if (file)
			__remove_shared_vm_struct(next, file, mapping);
	} else if (insert) {
=======
			__vma_link_file(vp->insert,
					vp->insert->vm_file->f_mapping);
		}
	}

	if (vp->anon_vma) {
		anon_vma_lock_write(vp->anon_vma);
		anon_vma_interval_tree_pre_update_vma(vp->vma);
		if (vp->adj_next)
			anon_vma_interval_tree_pre_update_vma(vp->adj_next);
	}

	if (vp->file) {
		flush_dcache_mmap_lock(vp->mapping);
		vma_interval_tree_remove(vp->vma, &vp->mapping->i_mmap);
		if (vp->adj_next)
			vma_interval_tree_remove(vp->adj_next,
						 &vp->mapping->i_mmap);
	}

}

/*
 * vma_complete- Helper function for handling the unlocking after altering VMAs,
 * or for inserting a VMA.
 *
 * @vp: The vma_prepare struct
 * @vmi: The vma iterator
 * @mm: The mm_struct
 */
static inline void vma_complete(struct vma_prepare *vp,
				struct vma_iterator *vmi, struct mm_struct *mm)
{
	if (vp->file) {
		if (vp->adj_next)
			vma_interval_tree_insert(vp->adj_next,
						 &vp->mapping->i_mmap);
		vma_interval_tree_insert(vp->vma, &vp->mapping->i_mmap);
		flush_dcache_mmap_unlock(vp->mapping);
	}

	if (vp->remove && vp->file) {
		__remove_shared_vm_struct(vp->remove, vp->mapping);
		if (vp->remove2)
			__remove_shared_vm_struct(vp->remove2, vp->mapping);
	} else if (vp->insert) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * split_vma has split insert from vma, and needs
		 * us to insert it before dropping the locks
		 * (it may either follow vma or precede it).
		 */
<<<<<<< HEAD
		__insert_vm_struct(mm, insert);
	}

	if (anon_vma)
		anon_vma_unlock(anon_vma);
	if (mapping)
		mutex_unlock(&mapping->i_mmap_mutex);

	if (remove_next) {
		if (file) {
			fput(file);
			if (next->vm_flags & VM_EXECUTABLE)
				removed_exe_file_vma(mm);
		}
		if (next->anon_vma)
			anon_vma_merge(vma, next);
		mm->map_count--;
		mpol_put(vma_policy(next));
		kmem_cache_free(vm_area_cachep, next);
		/*
		 * In mprotect's case 6 (see comments on vma_merge),
		 * we must remove another next too. It would clutter
		 * up the code too much to do both in one go.
		 */
		if (remove_next == 2) {
			next = vma->vm_next;
			goto again;
		}
	}

	validate_mm(mm);
=======
		vma_iter_store(vmi, vp->insert);
		mm->map_count++;
	}

	if (vp->anon_vma) {
		anon_vma_interval_tree_post_update_vma(vp->vma);
		if (vp->adj_next)
			anon_vma_interval_tree_post_update_vma(vp->adj_next);
		anon_vma_unlock_write(vp->anon_vma);
	}

	if (vp->file) {
		i_mmap_unlock_write(vp->mapping);
		uprobe_mmap(vp->vma);

		if (vp->adj_next)
			uprobe_mmap(vp->adj_next);
	}

	if (vp->remove) {
again:
		vma_mark_detached(vp->remove, true);
		if (vp->file) {
			uprobe_munmap(vp->remove, vp->remove->vm_start,
				      vp->remove->vm_end);
			fput(vp->file);
		}
		if (vp->remove->anon_vma)
			anon_vma_merge(vp->vma, vp->remove);
		mm->map_count--;
		mpol_put(vma_policy(vp->remove));
		if (!vp->remove2)
			WARN_ON_ONCE(vp->vma->vm_end < vp->remove->vm_end);
		vm_area_free(vp->remove);

		/*
		 * In mprotect's case 6 (see comments on vma_merge),
		 * we are removing both mid and next vmas
		 */
		if (vp->remove2) {
			vp->remove = vp->remove2;
			vp->remove2 = NULL;
			goto again;
		}
	}
	if (vp->insert && vp->file)
		uprobe_mmap(vp->insert);
	validate_mm(mm);
}

/*
 * dup_anon_vma() - Helper function to duplicate anon_vma
 * @dst: The destination VMA
 * @src: The source VMA
 * @dup: Pointer to the destination VMA when successful.
 *
 * Returns: 0 on success.
 */
static inline int dup_anon_vma(struct vm_area_struct *dst,
		struct vm_area_struct *src, struct vm_area_struct **dup)
{
	/*
	 * Easily overlooked: when mprotect shifts the boundary, make sure the
	 * expanding vma has anon_vma set if the shrinking vma had, to cover any
	 * anon pages imported.
	 */
	if (src->anon_vma && !dst->anon_vma) {
		int ret;

		vma_assert_write_locked(dst);
		dst->anon_vma = src->anon_vma;
		ret = anon_vma_clone(dst, src);
		if (ret)
			return ret;

		*dup = dst;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
<<<<<<< HEAD
 * If the vma has a ->close operation then the driver probably needs to release
 * per-vma resources, so we don't attempt to merge those.
 */
static inline int is_mergeable_vma(struct vm_area_struct *vma,
			struct file *file, unsigned long vm_flags,
			const char __user *anon_name)
{
	/* VM_CAN_NONLINEAR may get set later by f_op->mmap() */
	if ((vma->vm_flags ^ vm_flags) & ~VM_CAN_NONLINEAR)
		return 0;
	if (vma->vm_file != file)
		return 0;
	if (vma->vm_ops && vma->vm_ops->close)
		return 0;
	if (vma_get_anon_name(vma) != anon_name)
		return 0;
	return 1;
}

static inline int is_mergeable_anon_vma(struct anon_vma *anon_vma1,
					struct anon_vma *anon_vma2,
					struct vm_area_struct *vma)
=======
 * vma_expand - Expand an existing VMA
 *
 * @vmi: The vma iterator
 * @vma: The vma to expand
 * @start: The start of the vma
 * @end: The exclusive end of the vma
 * @pgoff: The page offset of vma
 * @next: The current of next vma.
 *
 * Expand @vma to @start and @end.  Can expand off the start and end.  Will
 * expand over @next if it's different from @vma and @end == @next->vm_end.
 * Checking if the @vma can expand and merge with @next needs to be handled by
 * the caller.
 *
 * Returns: 0 on success
 */
int vma_expand(struct vma_iterator *vmi, struct vm_area_struct *vma,
	       unsigned long start, unsigned long end, pgoff_t pgoff,
	       struct vm_area_struct *next)
{
	struct vm_area_struct *anon_dup = NULL;
	bool remove_next = false;
	struct vma_prepare vp;

	vma_start_write(vma);
	if (next && (vma != next) && (end == next->vm_end)) {
		int ret;

		remove_next = true;
		vma_start_write(next);
		ret = dup_anon_vma(vma, next, &anon_dup);
		if (ret)
			return ret;
	}

	init_multi_vma_prep(&vp, vma, NULL, remove_next ? next : NULL, NULL);
	/* Not merging but overwriting any part of next is not handled. */
	VM_WARN_ON(next && !vp.remove &&
		  next != vma && end > next->vm_start);
	/* Only handles expanding */
	VM_WARN_ON(vma->vm_start < start || vma->vm_end > end);

	/* Note: vma iterator must be pointing to 'start' */
	vma_iter_config(vmi, start, end);
	if (vma_iter_prealloc(vmi, vma))
		goto nomem;

	vma_prepare(&vp);
	vma_adjust_trans_huge(vma, start, end, 0);
	vma_set_range(vma, start, end, pgoff);
	vma_iter_store(vmi, vma);

	vma_complete(&vp, vmi, vma->vm_mm);
	return 0;

nomem:
	if (anon_dup)
		unlink_anon_vmas(anon_dup);
	return -ENOMEM;
}

/*
 * vma_shrink() - Reduce an existing VMAs memory area
 * @vmi: The vma iterator
 * @vma: The VMA to modify
 * @start: The new start
 * @end: The new end
 *
 * Returns: 0 on success, -ENOMEM otherwise
 */
int vma_shrink(struct vma_iterator *vmi, struct vm_area_struct *vma,
	       unsigned long start, unsigned long end, pgoff_t pgoff)
{
	struct vma_prepare vp;

	WARN_ON((vma->vm_start != start) && (vma->vm_end != end));

	if (vma->vm_start < start)
		vma_iter_config(vmi, vma->vm_start, start);
	else
		vma_iter_config(vmi, end, vma->vm_end);

	if (vma_iter_prealloc(vmi, NULL))
		return -ENOMEM;

	vma_start_write(vma);

	init_vma_prep(&vp, vma);
	vma_prepare(&vp);
	vma_adjust_trans_huge(vma, start, end, 0);

	vma_iter_clear(vmi);
	vma_set_range(vma, start, end, pgoff);
	vma_complete(&vp, vmi, vma->vm_mm);
	return 0;
}

/*
 * If the vma has a ->close operation then the driver probably needs to release
 * per-vma resources, so we don't attempt to merge those if the caller indicates
 * the current vma may be removed as part of the merge.
 */
static inline bool is_mergeable_vma(struct vm_area_struct *vma,
		struct file *file, unsigned long vm_flags,
		struct vm_userfaultfd_ctx vm_userfaultfd_ctx,
		struct anon_vma_name *anon_name, bool may_remove_vma)
{
	/*
	 * VM_SOFTDIRTY should not prevent from VMA merging, if we
	 * match the flags but dirty bit -- the caller should mark
	 * merged VMA as dirty. If dirty bit won't be excluded from
	 * comparison, we increase pressure on the memory system forcing
	 * the kernel to generate new VMAs when old one could be
	 * extended instead.
	 */
	if ((vma->vm_flags ^ vm_flags) & ~VM_SOFTDIRTY)
		return false;
	if (vma->vm_file != file)
		return false;
	if (may_remove_vma && vma->vm_ops && vma->vm_ops->close)
		return false;
	if (!is_mergeable_vm_userfaultfd_ctx(vma, vm_userfaultfd_ctx))
		return false;
	if (!anon_vma_name_eq(anon_vma_name(vma), anon_name))
		return false;
	return true;
}

static inline bool is_mergeable_anon_vma(struct anon_vma *anon_vma1,
		 struct anon_vma *anon_vma2, struct vm_area_struct *vma)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * The list_is_singular() test is to avoid merging VMA cloned from
	 * parents. This can improve scalability caused by anon_vma lock.
	 */
	if ((!anon_vma1 || !anon_vma2) && (!vma ||
		list_is_singular(&vma->anon_vma_chain)))
<<<<<<< HEAD
		return 1;
=======
		return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return anon_vma1 == anon_vma2;
}

/*
 * Return true if we can merge this (vm_flags,anon_vma,file,vm_pgoff)
 * in front of (at a lower virtual address and file offset than) the vma.
 *
 * We cannot merge two vmas if they have differently assigned (non-NULL)
 * anon_vmas, nor if same anon_vma is assigned but offsets incompatible.
 *
 * We don't check here for the merged mmap wrapping around the end of pagecache
<<<<<<< HEAD
 * indices (16TB on ia32) because do_mmap_pgoff() does not permit mmap's which
 * wrap, nor mmaps which cover the final page at index -1UL.
 */
static int
can_vma_merge_before(struct vm_area_struct *vma, unsigned long vm_flags,
	struct anon_vma *anon_vma, struct file *file, pgoff_t vm_pgoff,
	const char __user *anon_name)
{
	if (is_mergeable_vma(vma, file, vm_flags, anon_name) &&
	    is_mergeable_anon_vma(anon_vma, vma->anon_vma, vma)) {
		if (vma->vm_pgoff == vm_pgoff)
			return 1;
	}
	return 0;
=======
 * indices (16TB on ia32) because do_mmap() does not permit mmap's which
 * wrap, nor mmaps which cover the final page at index -1UL.
 *
 * We assume the vma may be removed as part of the merge.
 */
static bool
can_vma_merge_before(struct vm_area_struct *vma, unsigned long vm_flags,
		struct anon_vma *anon_vma, struct file *file,
		pgoff_t vm_pgoff, struct vm_userfaultfd_ctx vm_userfaultfd_ctx,
		struct anon_vma_name *anon_name)
{
	if (is_mergeable_vma(vma, file, vm_flags, vm_userfaultfd_ctx, anon_name, true) &&
	    is_mergeable_anon_vma(anon_vma, vma->anon_vma, vma)) {
		if (vma->vm_pgoff == vm_pgoff)
			return true;
	}
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Return true if we can merge this (vm_flags,anon_vma,file,vm_pgoff)
 * beyond (at a higher virtual address and file offset than) the vma.
 *
 * We cannot merge two vmas if they have differently assigned (non-NULL)
 * anon_vmas, nor if same anon_vma is assigned but offsets incompatible.
<<<<<<< HEAD
 */
static int
can_vma_merge_after(struct vm_area_struct *vma, unsigned long vm_flags,
	struct anon_vma *anon_vma, struct file *file, pgoff_t vm_pgoff,
	const char __user *anon_name)
{
	if (is_mergeable_vma(vma, file, vm_flags, anon_name) &&
	    is_mergeable_anon_vma(anon_vma, vma->anon_vma, vma)) {
		pgoff_t vm_pglen;
		vm_pglen = (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
		if (vma->vm_pgoff + vm_pglen == vm_pgoff)
			return 1;
	}
	return 0;
=======
 *
 * We assume that vma is not removed as part of the merge.
 */
static bool
can_vma_merge_after(struct vm_area_struct *vma, unsigned long vm_flags,
		struct anon_vma *anon_vma, struct file *file,
		pgoff_t vm_pgoff, struct vm_userfaultfd_ctx vm_userfaultfd_ctx,
		struct anon_vma_name *anon_name)
{
	if (is_mergeable_vma(vma, file, vm_flags, vm_userfaultfd_ctx, anon_name, false) &&
	    is_mergeable_anon_vma(anon_vma, vma->anon_vma, vma)) {
		pgoff_t vm_pglen;
		vm_pglen = vma_pages(vma);
		if (vma->vm_pgoff + vm_pglen == vm_pgoff)
			return true;
	}
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Given a mapping request (addr,end,vm_flags,file,pgoff,anon_name),
 * figure out whether that can be merged with its predecessor or its
 * successor.  Or both (it neatly fills a hole).
 *
 * In most cases - when called for mmap, brk or mremap - [addr,end) is
 * certain not to be mapped by the time vma_merge is called; but when
 * called for mprotect, it is certain to be already mapped (either at
 * an offset within prev, or at the start of next), and the flags of
 * this area are about to be changed to vm_flags - and the no-change
 * case has already been eliminated.
 *
<<<<<<< HEAD
 * The following mprotect cases have to be considered, where AAAA is
 * the area passed down from mprotect_fixup, never extending beyond one
 * vma, PPPPPP is the prev vma specified, and NNNNNN the next vma after:
 *
 *     AAAA             AAAA                AAAA          AAAA
 *    PPPPPPNNNNNN    PPPPPPNNNNNN    PPPPPPNNNNNN    PPPPNNNNXXXX
 *    cannot merge    might become    might become    might become
 *                    PPNNNNNNNNNN    PPPPPPPPPPNN    PPPPPPPPPPPP 6 or
 *    mmap, brk or    case 4 below    case 5 below    PPPPPPPPXXXX 7 or
 *    mremap move:                                    PPPPNNNNNNNN 8
 *        AAAA
 *    PPPP    NNNN    PPPPPPPPPPPP    PPPPPPPPNNNN    PPPPNNNNNNNN
 *    might become    case 1 below    case 2 below    case 3 below
 *
 * Odd one out? Case 8, because it extends NNNN but needs flags of XXXX:
 * mprotect_fixup updates vm_flags & vm_page_prot on successful return.
 */
struct vm_area_struct *vma_merge(struct mm_struct *mm,
			struct vm_area_struct *prev, unsigned long addr,
			unsigned long end, unsigned long vm_flags,
		     	struct anon_vma *anon_vma, struct file *file,
			pgoff_t pgoff, struct mempolicy *policy,
			const char __user *anon_name)
{
	pgoff_t pglen = (end - addr) >> PAGE_SHIFT;
	struct vm_area_struct *area, *next;
	int err;
=======
 * The following mprotect cases have to be considered, where **** is
 * the area passed down from mprotect_fixup, never extending beyond one
 * vma, PPPP is the previous vma, CCCC is a concurrent vma that starts
 * at the same address as **** and is of the same or larger span, and
 * NNNN the next vma after ****:
 *
 *     ****             ****                   ****
 *    PPPPPPNNNNNN    PPPPPPNNNNNN       PPPPPPCCCCCC
 *    cannot merge    might become       might become
 *                    PPNNNNNNNNNN       PPPPPPPPPPCC
 *    mmap, brk or    case 4 below       case 5 below
 *    mremap move:
 *                        ****               ****
 *                    PPPP    NNNN       PPPPCCCCNNNN
 *                    might become       might become
 *                    PPPPPPPPPPPP 1 or  PPPPPPPPPPPP 6 or
 *                    PPPPPPPPNNNN 2 or  PPPPPPPPNNNN 7 or
 *                    PPPPNNNNNNNN 3     PPPPNNNNNNNN 8
 *
 * It is important for case 8 that the vma CCCC overlapping the
 * region **** is never going to extended over NNNN. Instead NNNN must
 * be extended in region **** and CCCC must be removed. This way in
 * all cases where vma_merge succeeds, the moment vma_merge drops the
 * rmap_locks, the properties of the merged vma will be already
 * correct for the whole merged range. Some of those properties like
 * vm_page_prot/vm_flags may be accessed by rmap_walks and they must
 * be correct for the whole merged range immediately after the
 * rmap_locks are released. Otherwise if NNNN would be removed and
 * CCCC would be extended over the NNNN range, remove_migration_ptes
 * or other rmap walkers (if working on addresses beyond the "end"
 * parameter) may establish ptes with the wrong permissions of CCCC
 * instead of the right permissions of NNNN.
 *
 * In the code below:
 * PPPP is represented by *prev
 * CCCC is represented by *curr or not represented at all (NULL)
 * NNNN is represented by *next or not represented at all (NULL)
 * **** is not represented - it will be merged and the vma containing the
 *      area is returned, or the function will return NULL
 */
static struct vm_area_struct
*vma_merge(struct vma_iterator *vmi, struct vm_area_struct *prev,
	   struct vm_area_struct *src, unsigned long addr, unsigned long end,
	   unsigned long vm_flags, pgoff_t pgoff, struct mempolicy *policy,
	   struct vm_userfaultfd_ctx vm_userfaultfd_ctx,
	   struct anon_vma_name *anon_name)
{
	struct mm_struct *mm = src->vm_mm;
	struct anon_vma *anon_vma = src->anon_vma;
	struct file *file = src->vm_file;
	struct vm_area_struct *curr, *next, *res;
	struct vm_area_struct *vma, *adjust, *remove, *remove2;
	struct vm_area_struct *anon_dup = NULL;
	struct vma_prepare vp;
	pgoff_t vma_pgoff;
	int err = 0;
	bool merge_prev = false;
	bool merge_next = false;
	bool vma_expanded = false;
	unsigned long vma_start = addr;
	unsigned long vma_end = end;
	pgoff_t pglen = (end - addr) >> PAGE_SHIFT;
	long adj_start = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * We later require that vma->vm_flags == vm_flags,
	 * so this tests vma->vm_flags & VM_SPECIAL, too.
	 */
	if (vm_flags & VM_SPECIAL)
		return NULL;

<<<<<<< HEAD
	if (prev)
		next = prev->vm_next;
	else
		next = mm->mmap;
	area = next;
	if (next && next->vm_end == end)		/* cases 6, 7, 8 */
		next = next->vm_next;

	/*
	 * Can it merge with the predecessor?
	 */
	if (prev && prev->vm_end == addr &&
  			mpol_equal(vma_policy(prev), policy) &&
			can_vma_merge_after(prev, vm_flags, anon_vma,
						file, pgoff, anon_name)) {
		/*
		 * OK, it can.  Can we now merge in the successor as well?
		 */
		if (next && end == next->vm_start &&
				mpol_equal(policy, vma_policy(next)) &&
				can_vma_merge_before(next, vm_flags, anon_vma,
						file, pgoff+pglen, anon_name) &&
				is_mergeable_anon_vma(prev->anon_vma,
						      next->anon_vma, NULL)) {
							/* cases 1, 6 */
			err = vma_adjust(prev, prev->vm_start,
				next->vm_end, prev->vm_pgoff, NULL);
		} else					/* cases 2, 5, 7 */
			err = vma_adjust(prev, prev->vm_start,
				end, prev->vm_pgoff, NULL);
		if (err)
			return NULL;
		khugepaged_enter_vma_merge(prev, vm_flags);
		return prev;
	}

	/*
	 * Can this new request be merged in front of next?
	 */
	if (next && end == next->vm_start &&
 			mpol_equal(policy, vma_policy(next)) &&
			can_vma_merge_before(next, vm_flags, anon_vma,
					file, pgoff+pglen, anon_name)) {
		if (prev && addr < prev->vm_end)	/* case 4 */
			err = vma_adjust(prev, prev->vm_start,
				addr, prev->vm_pgoff, NULL);
		else					/* cases 3, 8 */
			err = vma_adjust(area, addr, next->vm_end,
				next->vm_pgoff - pglen, NULL);
		if (err)
			return NULL;
		khugepaged_enter_vma_merge(area, vm_flags);
		return area;
	}

=======
	/* Does the input range span an existing VMA? (cases 5 - 8) */
	curr = find_vma_intersection(mm, prev ? prev->vm_end : 0, end);

	if (!curr ||			/* cases 1 - 4 */
	    end == curr->vm_end)	/* cases 6 - 8, adjacent VMA */
		next = vma_lookup(mm, end);
	else
		next = NULL;		/* case 5 */

	if (prev) {
		vma_start = prev->vm_start;
		vma_pgoff = prev->vm_pgoff;

		/* Can we merge the predecessor? */
		if (addr == prev->vm_end && mpol_equal(vma_policy(prev), policy)
		    && can_vma_merge_after(prev, vm_flags, anon_vma, file,
					   pgoff, vm_userfaultfd_ctx, anon_name)) {
			merge_prev = true;
			vma_prev(vmi);
		}
	}

	/* Can we merge the successor? */
	if (next && mpol_equal(policy, vma_policy(next)) &&
	    can_vma_merge_before(next, vm_flags, anon_vma, file, pgoff+pglen,
				 vm_userfaultfd_ctx, anon_name)) {
		merge_next = true;
	}

	/* Verify some invariant that must be enforced by the caller. */
	VM_WARN_ON(prev && addr <= prev->vm_start);
	VM_WARN_ON(curr && (addr != curr->vm_start || end > curr->vm_end));
	VM_WARN_ON(addr >= end);

	if (!merge_prev && !merge_next)
		return NULL; /* Not mergeable. */

	if (merge_prev)
		vma_start_write(prev);

	res = vma = prev;
	remove = remove2 = adjust = NULL;

	/* Can we merge both the predecessor and the successor? */
	if (merge_prev && merge_next &&
	    is_mergeable_anon_vma(prev->anon_vma, next->anon_vma, NULL)) {
		vma_start_write(next);
		remove = next;				/* case 1 */
		vma_end = next->vm_end;
		err = dup_anon_vma(prev, next, &anon_dup);
		if (curr) {				/* case 6 */
			vma_start_write(curr);
			remove = curr;
			remove2 = next;
			/*
			 * Note that the dup_anon_vma below cannot overwrite err
			 * since the first caller would do nothing unless next
			 * has an anon_vma.
			 */
			if (!next->anon_vma)
				err = dup_anon_vma(prev, curr, &anon_dup);
		}
	} else if (merge_prev) {			/* case 2 */
		if (curr) {
			vma_start_write(curr);
			if (end == curr->vm_end) {	/* case 7 */
				/*
				 * can_vma_merge_after() assumed we would not be
				 * removing prev vma, so it skipped the check
				 * for vm_ops->close, but we are removing curr
				 */
				if (curr->vm_ops && curr->vm_ops->close)
					err = -EINVAL;
				remove = curr;
			} else {			/* case 5 */
				adjust = curr;
				adj_start = (end - curr->vm_start);
			}
			if (!err)
				err = dup_anon_vma(prev, curr, &anon_dup);
		}
	} else { /* merge_next */
		vma_start_write(next);
		res = next;
		if (prev && addr < prev->vm_end) {	/* case 4 */
			vma_start_write(prev);
			vma_end = addr;
			adjust = next;
			adj_start = -(prev->vm_end - addr);
			err = dup_anon_vma(next, prev, &anon_dup);
		} else {
			/*
			 * Note that cases 3 and 8 are the ONLY ones where prev
			 * is permitted to be (but is not necessarily) NULL.
			 */
			vma = next;			/* case 3 */
			vma_start = addr;
			vma_end = next->vm_end;
			vma_pgoff = next->vm_pgoff - pglen;
			if (curr) {			/* case 8 */
				vma_pgoff = curr->vm_pgoff;
				vma_start_write(curr);
				remove = curr;
				err = dup_anon_vma(next, curr, &anon_dup);
			}
		}
	}

	/* Error in anon_vma clone. */
	if (err)
		goto anon_vma_fail;

	if (vma_start < vma->vm_start || vma_end > vma->vm_end)
		vma_expanded = true;

	if (vma_expanded) {
		vma_iter_config(vmi, vma_start, vma_end);
	} else {
		vma_iter_config(vmi, adjust->vm_start + adj_start,
				adjust->vm_end);
	}

	if (vma_iter_prealloc(vmi, vma))
		goto prealloc_fail;

	init_multi_vma_prep(&vp, vma, adjust, remove, remove2);
	VM_WARN_ON(vp.anon_vma && adjust && adjust->anon_vma &&
		   vp.anon_vma != adjust->anon_vma);

	vma_prepare(&vp);
	vma_adjust_trans_huge(vma, vma_start, vma_end, adj_start);
	vma_set_range(vma, vma_start, vma_end, vma_pgoff);

	if (vma_expanded)
		vma_iter_store(vmi, vma);

	if (adj_start) {
		adjust->vm_start += adj_start;
		adjust->vm_pgoff += adj_start >> PAGE_SHIFT;
		if (adj_start < 0) {
			WARN_ON(vma_expanded);
			vma_iter_store(vmi, next);
		}
	}

	vma_complete(&vp, vmi, mm);
	khugepaged_enter_vma(res, vm_flags);
	return res;

prealloc_fail:
	if (anon_dup)
		unlink_anon_vmas(anon_dup);

anon_vma_fail:
	vma_iter_set(vmi, addr);
	vma_iter_load(vmi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}

/*
<<<<<<< HEAD
 * Rough compatbility check to quickly see if it's even worth looking
=======
 * Rough compatibility check to quickly see if it's even worth looking
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * at sharing an anon_vma.
 *
 * They need to have the same vm_file, and the flags can only differ
 * in things that mprotect may change.
 *
 * NOTE! The fact that we share an anon_vma doesn't _have_ to mean that
 * we can merge the two vma's. For example, we refuse to merge a vma if
 * there is a vm_ops->close() function, because that indicates that the
 * driver is doing some kind of reference counting. But that doesn't
 * really matter for the anon_vma sharing case.
 */
static int anon_vma_compatible(struct vm_area_struct *a, struct vm_area_struct *b)
{
	return a->vm_end == b->vm_start &&
		mpol_equal(vma_policy(a), vma_policy(b)) &&
		a->vm_file == b->vm_file &&
<<<<<<< HEAD
		!((a->vm_flags ^ b->vm_flags) & ~(VM_READ|VM_WRITE|VM_EXEC)) &&
=======
		!((a->vm_flags ^ b->vm_flags) & ~(VM_ACCESS_FLAGS | VM_SOFTDIRTY)) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		b->vm_pgoff == a->vm_pgoff + ((b->vm_start - a->vm_start) >> PAGE_SHIFT);
}

/*
 * Do some basic sanity checking to see if we can re-use the anon_vma
 * from 'old'. The 'a'/'b' vma's are in VM order - one of them will be
 * the same as 'old', the other will be the new one that is trying
 * to share the anon_vma.
 *
<<<<<<< HEAD
 * NOTE! This runs with mm_sem held for reading, so it is possible that
=======
 * NOTE! This runs with mmap_lock held for reading, so it is possible that
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * the anon_vma of 'old' is concurrently in the process of being set up
 * by another page fault trying to merge _that_. But that's ok: if it
 * is being set up, that automatically means that it will be a singleton
 * acceptable for merging, so we can do all of this optimistically. But
<<<<<<< HEAD
 * we do that ACCESS_ONCE() to make sure that we never re-load the pointer.
=======
 * we do that READ_ONCE() to make sure that we never re-load the pointer.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * IOW: that the "list_is_singular()" test on the anon_vma_chain only
 * matters for the 'stable anon_vma' case (ie the thing we want to avoid
 * is to return an anon_vma that is "complex" due to having gone through
 * a fork).
 *
 * We also make sure that the two vma's are compatible (adjacent,
 * and with the same memory policies). That's all stable, even with just
<<<<<<< HEAD
 * a read lock on the mm_sem.
=======
 * a read lock on the mmap_lock.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static struct anon_vma *reusable_anon_vma(struct vm_area_struct *old, struct vm_area_struct *a, struct vm_area_struct *b)
{
	if (anon_vma_compatible(a, b)) {
<<<<<<< HEAD
		struct anon_vma *anon_vma = ACCESS_ONCE(old->anon_vma);
=======
		struct anon_vma *anon_vma = READ_ONCE(old->anon_vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (anon_vma && list_is_singular(&old->anon_vma_chain))
			return anon_vma;
	}
	return NULL;
}

/*
 * find_mergeable_anon_vma is used by anon_vma_prepare, to check
 * neighbouring vmas for a suitable anon_vma, before it goes off
 * to allocate a new anon_vma.  It checks because a repetitive
 * sequence of mprotects and faults may otherwise lead to distinct
 * anon_vmas being allocated, preventing vma merge in subsequent
 * mprotect.
 */
struct anon_vma *find_mergeable_anon_vma(struct vm_area_struct *vma)
{
<<<<<<< HEAD
	struct anon_vma *anon_vma;
	struct vm_area_struct *near;

	near = vma->vm_next;
	if (!near)
		goto try_prev;

	anon_vma = reusable_anon_vma(near, vma, near);
	if (anon_vma)
		return anon_vma;
try_prev:
	near = vma->vm_prev;
	if (!near)
		goto none;

	anon_vma = reusable_anon_vma(near, near, vma);
	if (anon_vma)
		return anon_vma;
none:
	/*
=======
	MA_STATE(mas, &vma->vm_mm->mm_mt, vma->vm_end, vma->vm_end);
	struct anon_vma *anon_vma = NULL;
	struct vm_area_struct *prev, *next;

	/* Try next first. */
	next = mas_walk(&mas);
	if (next) {
		anon_vma = reusable_anon_vma(next, vma, next);
		if (anon_vma)
			return anon_vma;
	}

	prev = mas_prev(&mas, 0);
	VM_BUG_ON_VMA(prev != vma, vma);
	prev = mas_prev(&mas, 0);
	/* Try prev next. */
	if (prev)
		anon_vma = reusable_anon_vma(prev, prev, vma);

	/*
	 * We might reach here with anon_vma == NULL if we can't find
	 * any reusable anon_vma.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * There's no absolute need to look only at touching neighbours:
	 * we could search further afield for "compatible" anon_vmas.
	 * But it would probably just be a waste of time searching,
	 * or lead to too many vmas hanging off the same anon_vma.
	 * We're trying to allow mprotect remerging later on,
	 * not trying to minimize memory used for anon_vmas.
	 */
<<<<<<< HEAD
	return NULL;
}

#ifdef CONFIG_PROC_FS
void vm_stat_account(struct mm_struct *mm, unsigned long flags,
						struct file *file, long pages)
{
	const unsigned long stack_flags
		= VM_STACK_FLAGS & (VM_GROWSUP|VM_GROWSDOWN);

	if (file) {
		mm->shared_vm += pages;
		if ((flags & (VM_EXEC|VM_WRITE)) == VM_EXEC)
			mm->exec_vm += pages;
	} else if (flags & stack_flags)
		mm->stack_vm += pages;
	if (flags & (VM_RESERVED|VM_IO))
		mm->reserved_vm += pages;
}
#endif /* CONFIG_PROC_FS */

=======
	return anon_vma;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * If a hint addr is less than mmap_min_addr change hint to be as
 * low as possible but still greater than mmap_min_addr
 */
static inline unsigned long round_hint_to_min(unsigned long hint)
{
	hint &= PAGE_MASK;
	if (((void *)hint != NULL) &&
	    (hint < mmap_min_addr))
		return PAGE_ALIGN(mmap_min_addr);
	return hint;
}

<<<<<<< HEAD
/*
 * The caller must hold down_write(&current->mm->mmap_sem).
 */

static unsigned long do_mmap_pgoff(struct file *file, unsigned long addr,
			unsigned long len, unsigned long prot,
			unsigned long flags, unsigned long pgoff)
{
	struct mm_struct * mm = current->mm;
	struct inode *inode;
	vm_flags_t vm_flags;
	int error;
	unsigned long reqprot = prot;

	while (file && (file->f_mode & FMODE_NONMAPPABLE))
		file = file->f_op->get_lower_file(file);
=======
bool mlock_future_ok(struct mm_struct *mm, unsigned long flags,
			unsigned long bytes)
{
	unsigned long locked_pages, limit_pages;

	if (!(flags & VM_LOCKED) || capable(CAP_IPC_LOCK))
		return true;

	locked_pages = bytes >> PAGE_SHIFT;
	locked_pages += mm->locked_vm;

	limit_pages = rlimit(RLIMIT_MEMLOCK);
	limit_pages >>= PAGE_SHIFT;

	return locked_pages <= limit_pages;
}

static inline u64 file_mmap_size_max(struct file *file, struct inode *inode)
{
	if (S_ISREG(inode->i_mode))
		return MAX_LFS_FILESIZE;

	if (S_ISBLK(inode->i_mode))
		return MAX_LFS_FILESIZE;

	if (S_ISSOCK(inode->i_mode))
		return MAX_LFS_FILESIZE;

	/* Special "we do even unsigned file positions" case */
	if (file->f_mode & FMODE_UNSIGNED_OFFSET)
		return 0;

	/* Yes, random drivers might want more. But I'm tired of buggy drivers */
	return ULONG_MAX;
}

static inline bool file_mmap_ok(struct file *file, struct inode *inode,
				unsigned long pgoff, unsigned long len)
{
	u64 maxsize = file_mmap_size_max(file, inode);

	if (maxsize && len > maxsize)
		return false;
	maxsize -= len;
	if (pgoff > maxsize >> PAGE_SHIFT)
		return false;
	return true;
}

/*
 * The caller must write-lock current->mm->mmap_lock.
 */
unsigned long do_mmap(struct file *file, unsigned long addr,
			unsigned long len, unsigned long prot,
			unsigned long flags, vm_flags_t vm_flags,
			unsigned long pgoff, unsigned long *populate,
			struct list_head *uf)
{
	struct mm_struct *mm = current->mm;
	int pkey = 0;

	*populate = 0;

	if (!len)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Does the application expect PROT_READ to imply PROT_EXEC?
	 *
	 * (the exception is when the underlying filesystem is noexec
<<<<<<< HEAD
	 *  mounted, in which case we dont add PROT_EXEC.)
	 */
	if ((prot & PROT_READ) && (current->personality & READ_IMPLIES_EXEC))
		if (!(file && (file->f_path.mnt->mnt_flags & MNT_NOEXEC)))
			prot |= PROT_EXEC;

	if (!len)
		return -EINVAL;
=======
	 *  mounted, in which case we don't add PROT_EXEC.)
	 */
	if ((prot & PROT_READ) && (current->personality & READ_IMPLIES_EXEC))
		if (!(file && path_noexec(&file->f_path)))
			prot |= PROT_EXEC;

	/* force arch specific MAP_FIXED handling in get_unmapped_area */
	if (flags & MAP_FIXED_NOREPLACE)
		flags |= MAP_FIXED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!(flags & MAP_FIXED))
		addr = round_hint_to_min(addr);

	/* Careful about overflows.. */
	len = PAGE_ALIGN(len);
	if (!len)
		return -ENOMEM;

	/* offset overflow? */
	if ((pgoff + (len >> PAGE_SHIFT)) < pgoff)
<<<<<<< HEAD
               return -EOVERFLOW;
=======
		return -EOVERFLOW;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Too many mappings? */
	if (mm->map_count > sysctl_max_map_count)
		return -ENOMEM;

	/* Obtain the address to map to. we verify (or select) it and ensure
	 * that it represents a valid section of the address space.
	 */
	addr = get_unmapped_area(file, addr, len, pgoff, flags);
<<<<<<< HEAD
	if (addr & ~PAGE_MASK)
		return addr;

=======
	if (IS_ERR_VALUE(addr))
		return addr;

	if (flags & MAP_FIXED_NOREPLACE) {
		if (find_vma_intersection(mm, addr, addr + len))
			return -EEXIST;
	}

	if (prot == PROT_EXEC) {
		pkey = execute_only_pkey(mm);
		if (pkey < 0)
			pkey = 0;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Do simple checking here so the lower-level routines won't have
	 * to. we assume access permissions have been handled by the open
	 * of the memory object, so we don't do any here.
	 */
<<<<<<< HEAD
	vm_flags = calc_vm_prot_bits(prot) | calc_vm_flag_bits(flags) |
=======
	vm_flags |= calc_vm_prot_bits(prot, pkey) | calc_vm_flag_bits(flags) |
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mm->def_flags | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC;

	if (flags & MAP_LOCKED)
		if (!can_do_mlock())
			return -EPERM;

<<<<<<< HEAD
	/* mlock MCL_FUTURE? */
	if (vm_flags & VM_LOCKED) {
		unsigned long locked, lock_limit;
		locked = len >> PAGE_SHIFT;
		locked += mm->locked_vm;
		lock_limit = rlimit(RLIMIT_MEMLOCK);
		lock_limit >>= PAGE_SHIFT;
		if (locked > lock_limit && !capable(CAP_IPC_LOCK))
			return -EAGAIN;
	}

	inode = file ? file->f_path.dentry->d_inode : NULL;

	if (file) {
		switch (flags & MAP_TYPE) {
		case MAP_SHARED:
			if ((prot&PROT_WRITE) && !(file->f_mode&FMODE_WRITE))
				return -EACCES;
=======
	if (!mlock_future_ok(mm, vm_flags, len))
		return -EAGAIN;

	if (file) {
		struct inode *inode = file_inode(file);
		unsigned long flags_mask;

		if (!file_mmap_ok(file, inode, pgoff, len))
			return -EOVERFLOW;

		flags_mask = LEGACY_MAP_MASK | file->f_op->mmap_supported_flags;

		switch (flags & MAP_TYPE) {
		case MAP_SHARED:
			/*
			 * Force use of MAP_SHARED_VALIDATE with non-legacy
			 * flags. E.g. MAP_SYNC is dangerous to use with
			 * MAP_SHARED as you don't know which consistency model
			 * you will get. We silently ignore unsupported flags
			 * with MAP_SHARED to preserve backward compatibility.
			 */
			flags &= LEGACY_MAP_MASK;
			fallthrough;
		case MAP_SHARED_VALIDATE:
			if (flags & ~flags_mask)
				return -EOPNOTSUPP;
			if (prot & PROT_WRITE) {
				if (!(file->f_mode & FMODE_WRITE))
					return -EACCES;
				if (IS_SWAPFILE(file->f_mapping->host))
					return -ETXTBSY;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/*
			 * Make sure we don't allow writing to an append-only
			 * file..
			 */
			if (IS_APPEND(inode) && (file->f_mode & FMODE_WRITE))
				return -EACCES;

<<<<<<< HEAD
			/*
			 * Make sure there are no mandatory locks on the file.
			 */
			if (locks_verify_locked(inode))
				return -EAGAIN;

			vm_flags |= VM_SHARED | VM_MAYSHARE;
			if (!(file->f_mode & FMODE_WRITE))
				vm_flags &= ~(VM_MAYWRITE | VM_SHARED);

			/* fall through */
		case MAP_PRIVATE:
			if (!(file->f_mode & FMODE_READ))
				return -EACCES;
			if (file->f_path.mnt->mnt_flags & MNT_NOEXEC) {
=======
			vm_flags |= VM_SHARED | VM_MAYSHARE;
			if (!(file->f_mode & FMODE_WRITE))
				vm_flags &= ~(VM_MAYWRITE | VM_SHARED);
			fallthrough;
		case MAP_PRIVATE:
			if (!(file->f_mode & FMODE_READ))
				return -EACCES;
			if (path_noexec(&file->f_path)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (vm_flags & VM_EXEC)
					return -EPERM;
				vm_flags &= ~VM_MAYEXEC;
			}

<<<<<<< HEAD
			if (!file->f_op || !file->f_op->mmap)
				return -ENODEV;
=======
			if (!file->f_op->mmap)
				return -ENODEV;
			if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
				return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		default:
			return -EINVAL;
		}
	} else {
		switch (flags & MAP_TYPE) {
		case MAP_SHARED:
<<<<<<< HEAD
=======
			if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
				return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Ignore pgoff.
			 */
			pgoff = 0;
			vm_flags |= VM_SHARED | VM_MAYSHARE;
			break;
		case MAP_PRIVATE:
			/*
			 * Set pgoff according to addr for anon_vma.
			 */
			pgoff = addr >> PAGE_SHIFT;
			break;
		default:
			return -EINVAL;
		}
	}

<<<<<<< HEAD
	error = security_file_mmap(file, reqprot, prot, flags, addr, 0);
	if (error)
		return error;

	return mmap_region(file, addr, len, flags, vm_flags, pgoff);
}

unsigned long do_mmap(struct file *file, unsigned long addr,
	unsigned long len, unsigned long prot,
	unsigned long flag, unsigned long offset)
{
	if (unlikely(offset + PAGE_ALIGN(len) < offset))
		return -EINVAL;
	if (unlikely(offset & ~PAGE_MASK))
		return -EINVAL;
	return do_mmap_pgoff(file, addr, len, prot, flag, offset >> PAGE_SHIFT);
}
EXPORT_SYMBOL(do_mmap);

unsigned long vm_mmap(struct file *file, unsigned long addr,
	unsigned long len, unsigned long prot,
	unsigned long flag, unsigned long offset)
{
	unsigned long ret;
	struct mm_struct *mm = current->mm;

	down_write(&mm->mmap_sem);
	ret = do_mmap(file, addr, len, prot, flag, offset);
	up_write(&mm->mmap_sem);
	return ret;
}
EXPORT_SYMBOL(vm_mmap);
=======
	/*
	 * Set 'VM_NORESERVE' if we should not account for the
	 * memory use of this mapping.
	 */
	if (flags & MAP_NORESERVE) {
		/* We honor MAP_NORESERVE if allowed to overcommit */
		if (sysctl_overcommit_memory != OVERCOMMIT_NEVER)
			vm_flags |= VM_NORESERVE;

		/* hugetlb applies strict overcommit unless MAP_NORESERVE */
		if (file && is_file_hugepages(file))
			vm_flags |= VM_NORESERVE;
	}

	addr = mmap_region(file, addr, len, vm_flags, pgoff, uf);
	if (!IS_ERR_VALUE(addr) &&
	    ((vm_flags & VM_LOCKED) ||
	     (flags & (MAP_POPULATE | MAP_NONBLOCK)) == MAP_POPULATE))
		*populate = len;
	return addr;
}

unsigned long ksys_mmap_pgoff(unsigned long addr, unsigned long len,
			      unsigned long prot, unsigned long flags,
			      unsigned long fd, unsigned long pgoff)
{
	struct file *file = NULL;
	unsigned long retval;

	if (!(flags & MAP_ANONYMOUS)) {
		audit_mmap_fd(fd, flags);
		file = fget(fd);
		if (!file)
			return -EBADF;
		if (is_file_hugepages(file)) {
			len = ALIGN(len, huge_page_size(hstate_file(file)));
		} else if (unlikely(flags & MAP_HUGETLB)) {
			retval = -EINVAL;
			goto out_fput;
		}
	} else if (flags & MAP_HUGETLB) {
		struct hstate *hs;

		hs = hstate_sizelog((flags >> MAP_HUGE_SHIFT) & MAP_HUGE_MASK);
		if (!hs)
			return -EINVAL;

		len = ALIGN(len, huge_page_size(hs));
		/*
		 * VM_NORESERVE is used because the reservations will be
		 * taken when vm_ops->mmap() is called
		 */
		file = hugetlb_file_setup(HUGETLB_ANON_FILE, len,
				VM_NORESERVE,
				HUGETLB_ANONHUGE_INODE,
				(flags >> MAP_HUGE_SHIFT) & MAP_HUGE_MASK);
		if (IS_ERR(file))
			return PTR_ERR(file);
	}

	retval = vm_mmap_pgoff(file, addr, len, prot, flags, pgoff);
out_fput:
	if (file)
		fput(file);
	return retval;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

SYSCALL_DEFINE6(mmap_pgoff, unsigned long, addr, unsigned long, len,
		unsigned long, prot, unsigned long, flags,
		unsigned long, fd, unsigned long, pgoff)
{
<<<<<<< HEAD
	struct file *file = NULL;
	unsigned long retval = -EBADF;

	if (!(flags & MAP_ANONYMOUS)) {
		audit_mmap_fd(fd, flags);
		if (unlikely(flags & MAP_HUGETLB))
			return -EINVAL;
		file = fget(fd);
		if (!file)
			goto out;
		if (is_file_hugepages(file))
			len = ALIGN(len, huge_page_size(hstate_file(file)));
	} else if (flags & MAP_HUGETLB) {
		struct user_struct *user = NULL;

		len = ALIGN(len, huge_page_size(&default_hstate));
		/*
		 * VM_NORESERVE is used because the reservations will be
		 * taken when vm_ops->mmap() is called
		 * A dummy user value is used because we are not locking
		 * memory so no accounting is necessary
		 */
		file = hugetlb_file_setup(HUGETLB_ANON_FILE, len,
						VM_NORESERVE, &user,
						HUGETLB_ANONHUGE_INODE);
		if (IS_ERR(file))
			return PTR_ERR(file);
	}

	flags &= ~(MAP_EXECUTABLE | MAP_DENYWRITE);

	down_write(&current->mm->mmap_sem);
	retval = do_mmap_pgoff(file, addr, len, prot, flags, pgoff);
	up_write(&current->mm->mmap_sem);

	if (file)
		fput(file);
out:
	return retval;
=======
	return ksys_mmap_pgoff(addr, len, prot, flags, fd, pgoff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef __ARCH_WANT_SYS_OLD_MMAP
struct mmap_arg_struct {
	unsigned long addr;
	unsigned long len;
	unsigned long prot;
	unsigned long flags;
	unsigned long fd;
	unsigned long offset;
};

SYSCALL_DEFINE1(old_mmap, struct mmap_arg_struct __user *, arg)
{
	struct mmap_arg_struct a;

	if (copy_from_user(&a, arg, sizeof(a)))
		return -EFAULT;
<<<<<<< HEAD
	if (a.offset & ~PAGE_MASK)
		return -EINVAL;

	return sys_mmap_pgoff(a.addr, a.len, a.prot, a.flags, a.fd,
			      a.offset >> PAGE_SHIFT);
}
#endif /* __ARCH_WANT_SYS_OLD_MMAP */

/*
 * Some shared mappigns will want the pages marked read-only
=======
	if (offset_in_page(a.offset))
		return -EINVAL;

	return ksys_mmap_pgoff(a.addr, a.len, a.prot, a.flags, a.fd,
			       a.offset >> PAGE_SHIFT);
}
#endif /* __ARCH_WANT_SYS_OLD_MMAP */

static bool vm_ops_needs_writenotify(const struct vm_operations_struct *vm_ops)
{
	return vm_ops && (vm_ops->page_mkwrite || vm_ops->pfn_mkwrite);
}

static bool vma_is_shared_writable(struct vm_area_struct *vma)
{
	return (vma->vm_flags & (VM_WRITE | VM_SHARED)) ==
		(VM_WRITE | VM_SHARED);
}

static bool vma_fs_can_writeback(struct vm_area_struct *vma)
{
	/* No managed pages to writeback. */
	if (vma->vm_flags & VM_PFNMAP)
		return false;

	return vma->vm_file && vma->vm_file->f_mapping &&
		mapping_can_writeback(vma->vm_file->f_mapping);
}

/*
 * Does this VMA require the underlying folios to have their dirty state
 * tracked?
 */
bool vma_needs_dirty_tracking(struct vm_area_struct *vma)
{
	/* Only shared, writable VMAs require dirty tracking. */
	if (!vma_is_shared_writable(vma))
		return false;

	/* Does the filesystem need to be notified? */
	if (vm_ops_needs_writenotify(vma->vm_ops))
		return true;

	/*
	 * Even if the filesystem doesn't indicate a need for writenotify, if it
	 * can writeback, dirty tracking is still required.
	 */
	return vma_fs_can_writeback(vma);
}

/*
 * Some shared mappings will want the pages marked read-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * to track write events. If so, we'll downgrade vm_page_prot
 * to the private version (using protection_map[] without the
 * VM_SHARED bit).
 */
<<<<<<< HEAD
int vma_wants_writenotify(struct vm_area_struct *vma)
{
	vm_flags_t vm_flags = vma->vm_flags;

	/* If it was private or non-writable, the write bit is already clear */
	if ((vm_flags & (VM_WRITE|VM_SHARED)) != ((VM_WRITE|VM_SHARED)))
		return 0;

	/* The backer wishes to know when pages are first written to? */
	if (vma->vm_ops && vma->vm_ops->page_mkwrite)
		return 1;

	/* The open routine did something to the protections already? */
	if (pgprot_val(vma->vm_page_prot) !=
	    pgprot_val(vm_get_page_prot(vm_flags)))
		return 0;

	/* Specialty mapping? */
	if (vm_flags & (VM_PFNMAP|VM_INSERTPAGE))
		return 0;

	/* Can the mapping track the dirty pages? */
	return vma->vm_file && vma->vm_file->f_mapping &&
		mapping_cap_account_dirty(vma->vm_file->f_mapping);
=======
int vma_wants_writenotify(struct vm_area_struct *vma, pgprot_t vm_page_prot)
{
	/* If it was private or non-writable, the write bit is already clear */
	if (!vma_is_shared_writable(vma))
		return 0;

	/* The backer wishes to know when pages are first written to? */
	if (vm_ops_needs_writenotify(vma->vm_ops))
		return 1;

	/* The open routine did something to the protections that pgprot_modify
	 * won't preserve? */
	if (pgprot_val(vm_page_prot) !=
	    pgprot_val(vm_pgprot_modify(vm_page_prot, vma->vm_flags)))
		return 0;

	/*
	 * Do we need to track softdirty? hugetlb does not support softdirty
	 * tracking yet.
	 */
	if (vma_soft_dirty_enabled(vma) && !is_vm_hugetlb_page(vma))
		return 1;

	/* Do we need write faults for uffd-wp tracking? */
	if (userfaultfd_wp(vma))
		return 1;

	/* Can the mapping track the dirty pages? */
	return vma_fs_can_writeback(vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * We account for memory if it's a private writeable mapping,
 * not hugepages and VM_NORESERVE wasn't set.
 */
static inline int accountable_mapping(struct file *file, vm_flags_t vm_flags)
{
	/*
	 * hugetlb has its own accounting separate from the core VM
	 * VM_HUGETLB may not be set yet so we cannot check for that flag.
	 */
	if (file && is_file_hugepages(file))
		return 0;

	return (vm_flags & (VM_NORESERVE | VM_SHARED | VM_WRITE)) == VM_WRITE;
}

<<<<<<< HEAD
unsigned long mmap_region(struct file *file, unsigned long addr,
			  unsigned long len, unsigned long flags,
			  vm_flags_t vm_flags, unsigned long pgoff)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma, *prev;
	int correct_wcount = 0;
	int error;
	struct rb_node **rb_link, *rb_parent;
	unsigned long charged = 0;
	struct inode *inode =  file ? file->f_path.dentry->d_inode : NULL;

	/* Check against address space limit. */
	if (!may_expand_vm(mm, len >> PAGE_SHIFT)) {
		unsigned long nr_pages;

		/*
		 * MAP_FIXED may remove pages of mappings that intersects with
		 * requested mapping. Account for the pages it would unmap.
		 */
		if (!(vm_flags & MAP_FIXED))
			return -ENOMEM;

		nr_pages = count_vma_pages_range(mm, addr, addr + len);

		if (!may_expand_vm(mm, (len >> PAGE_SHIFT) - nr_pages))
			return -ENOMEM;
	}

	/* Clear old maps */
	error = -ENOMEM;
munmap_back:
	vma = find_vma_prepare(mm, addr, &prev, &rb_link, &rb_parent);
	if (vma && vma->vm_start < addr + len) {
		if (do_munmap(mm, addr, len))
			return -ENOMEM;
		goto munmap_back;
	}

	/*
	 * Set 'VM_NORESERVE' if we should not account for the
	 * memory use of this mapping.
	 */
	if ((flags & MAP_NORESERVE)) {
		/* We honor MAP_NORESERVE if allowed to overcommit */
		if (sysctl_overcommit_memory != OVERCOMMIT_NEVER)
			vm_flags |= VM_NORESERVE;

		/* hugetlb applies strict overcommit unless MAP_NORESERVE */
		if (file && is_file_hugepages(file))
			vm_flags |= VM_NORESERVE;
	}

	/*
	 * Private writable mapping: check memory availability
	 */
	if (accountable_mapping(file, vm_flags)) {
		charged = len >> PAGE_SHIFT;
		if (security_vm_enough_memory_mm(mm, charged))
			return -ENOMEM;
		vm_flags |= VM_ACCOUNT;
	}

	/*
	 * Can we just expand an old mapping?
	 */
	vma = vma_merge(mm, prev, addr, addr + len, vm_flags, NULL, file, pgoff,
			NULL, NULL);
	if (vma)
		goto out;

	/*
	 * Determine the object being mapped and call the appropriate
	 * specific mapper. the address has already been validated, but
	 * not unmapped, but the maps are removed from the list.
	 */
	vma = kmem_cache_zalloc(vm_area_cachep, GFP_KERNEL);
	if (!vma) {
		error = -ENOMEM;
		goto unacct_error;
	}

	vma->vm_mm = mm;
	vma->vm_start = addr;
	vma->vm_end = addr + len;
	vma->vm_flags = vm_flags;
	vma->vm_page_prot = vm_get_page_prot(vm_flags);
	vma->vm_pgoff = pgoff;
	INIT_LIST_HEAD(&vma->anon_vma_chain);

	error = -EINVAL;	/* when rejecting VM_GROWSDOWN|VM_GROWSUP */

	if (file) {
		if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
			goto free_vma;
		if (vm_flags & VM_DENYWRITE) {
			error = deny_write_access(file);
			if (error)
				goto free_vma;
			correct_wcount = 1;
		}
		vma->vm_file = file;
		get_file(file);
		error = file->f_op->mmap(file, vma);
		if (error)
			goto unmap_and_free_vma;
		if (vm_flags & VM_EXECUTABLE)
			added_exe_file_vma(mm);

		/* Can addr have changed??
		 *
		 * Answer: Yes, several device drivers can do it in their
		 *         f_op->mmap method. -DaveM
		 */
		addr = vma->vm_start;
		pgoff = vma->vm_pgoff;
		vm_flags = vma->vm_flags;
	} else if (vm_flags & VM_SHARED) {
		if (unlikely(vm_flags & (VM_GROWSDOWN|VM_GROWSUP)))
			goto free_vma;
		error = shmem_zero_setup(vma);
		if (error)
			goto free_vma;
	}

	if (vma_wants_writenotify(vma)) {
		pgprot_t pprot = vma->vm_page_prot;

		/* Can vma->vm_page_prot have changed??
		 *
		 * Answer: Yes, drivers may have changed it in their
		 *         f_op->mmap method.
		 *
		 * Ensures that vmas marked as uncached stay that way.
		 */
		vma->vm_page_prot = vm_get_page_prot(vm_flags & ~VM_SHARED);
		if (pgprot_val(pprot) == pgprot_val(pgprot_noncached(pprot)))
			vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	}

	vma_link(mm, vma, prev, rb_link, rb_parent);
	file = vma->vm_file;

	/* Once vma denies write, undo our temporary denial count */
	if (correct_wcount)
		atomic_inc(&inode->i_writecount);
out:
	perf_event_mmap(vma);

	mm->total_vm += len >> PAGE_SHIFT;
	vm_stat_account(mm, vm_flags, file, len >> PAGE_SHIFT);
	if (vm_flags & VM_LOCKED) {
		if (!mlock_vma_pages_range(vma, addr, addr + len))
			mm->locked_vm += (len >> PAGE_SHIFT);
	} else if ((flags & MAP_POPULATE) && !(flags & MAP_NONBLOCK))
		make_pages_present(addr, addr + len);
	return addr;

unmap_and_free_vma:
	if (correct_wcount)
		atomic_inc(&inode->i_writecount);
	vma->vm_file = NULL;
	fput(file);

	/* Undo any partial mapping done by a device driver. */
	unmap_region(mm, vma, prev, vma->vm_start, vma->vm_end);
	charged = 0;
free_vma:
	kmem_cache_free(vm_area_cachep, vma);
unacct_error:
	if (charged)
		vm_unacct_memory(charged);
	return error;
=======
/**
 * unmapped_area() - Find an area between the low_limit and the high_limit with
 * the correct alignment and offset, all from @info. Note: current->mm is used
 * for the search.
 *
 * @info: The unmapped area information including the range [low_limit -
 * high_limit), the alignment offset and mask.
 *
 * Return: A memory address or -ENOMEM.
 */
static unsigned long unmapped_area(struct vm_unmapped_area_info *info)
{
	unsigned long length, gap;
	unsigned long low_limit, high_limit;
	struct vm_area_struct *tmp;

	MA_STATE(mas, &current->mm->mm_mt, 0, 0);

	/* Adjust search length to account for worst case alignment overhead */
	length = info->length + info->align_mask;
	if (length < info->length)
		return -ENOMEM;

	low_limit = info->low_limit;
	if (low_limit < mmap_min_addr)
		low_limit = mmap_min_addr;
	high_limit = info->high_limit;
retry:
	if (mas_empty_area(&mas, low_limit, high_limit - 1, length))
		return -ENOMEM;

	gap = mas.index;
	gap += (info->align_offset - gap) & info->align_mask;
	tmp = mas_next(&mas, ULONG_MAX);
	if (tmp && (tmp->vm_flags & VM_STARTGAP_FLAGS)) { /* Avoid prev check if possible */
		if (vm_start_gap(tmp) < gap + length - 1) {
			low_limit = tmp->vm_end;
			mas_reset(&mas);
			goto retry;
		}
	} else {
		tmp = mas_prev(&mas, 0);
		if (tmp && vm_end_gap(tmp) > gap) {
			low_limit = vm_end_gap(tmp);
			mas_reset(&mas);
			goto retry;
		}
	}

	return gap;
}

/**
 * unmapped_area_topdown() - Find an area between the low_limit and the
 * high_limit with the correct alignment and offset at the highest available
 * address, all from @info. Note: current->mm is used for the search.
 *
 * @info: The unmapped area information including the range [low_limit -
 * high_limit), the alignment offset and mask.
 *
 * Return: A memory address or -ENOMEM.
 */
static unsigned long unmapped_area_topdown(struct vm_unmapped_area_info *info)
{
	unsigned long length, gap, gap_end;
	unsigned long low_limit, high_limit;
	struct vm_area_struct *tmp;

	MA_STATE(mas, &current->mm->mm_mt, 0, 0);
	/* Adjust search length to account for worst case alignment overhead */
	length = info->length + info->align_mask;
	if (length < info->length)
		return -ENOMEM;

	low_limit = info->low_limit;
	if (low_limit < mmap_min_addr)
		low_limit = mmap_min_addr;
	high_limit = info->high_limit;
retry:
	if (mas_empty_area_rev(&mas, low_limit, high_limit - 1, length))
		return -ENOMEM;

	gap = mas.last + 1 - info->length;
	gap -= (gap - info->align_offset) & info->align_mask;
	gap_end = mas.last;
	tmp = mas_next(&mas, ULONG_MAX);
	if (tmp && (tmp->vm_flags & VM_STARTGAP_FLAGS)) { /* Avoid prev check if possible */
		if (vm_start_gap(tmp) <= gap_end) {
			high_limit = vm_start_gap(tmp);
			mas_reset(&mas);
			goto retry;
		}
	} else {
		tmp = mas_prev(&mas, 0);
		if (tmp && vm_end_gap(tmp) > gap) {
			high_limit = tmp->vm_start;
			mas_reset(&mas);
			goto retry;
		}
	}

	return gap;
}

/*
 * Search for an unmapped address range.
 *
 * We are looking for a range that:
 * - does not intersect with any VMA;
 * - is contained within the [low_limit, high_limit) interval;
 * - is at least the desired size.
 * - satisfies (begin_addr & align_mask) == (align_offset & align_mask)
 */
unsigned long vm_unmapped_area(struct vm_unmapped_area_info *info)
{
	unsigned long addr;

	if (info->flags & VM_UNMAPPED_AREA_TOPDOWN)
		addr = unmapped_area_topdown(info);
	else
		addr = unmapped_area(info);

	trace_vm_unmapped_area(addr, info);
	return addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Get an address range which is currently unmapped.
 * For shmat() with addr=0.
 *
 * Ugly calling convention alert:
 * Return value with the low bits set means error value,
 * ie
 *	if (ret & ~PAGE_MASK)
 *		error = ret;
 *
 * This function "knows" that -ENOMEM has the bits set.
 */
<<<<<<< HEAD
#ifndef HAVE_ARCH_UNMAPPED_AREA
unsigned long
arch_get_unmapped_area(struct file *filp, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma, *prev;
	unsigned long start_addr, vm_start, prev_end;

	if (len > TASK_SIZE)
=======
unsigned long
generic_get_unmapped_area(struct file *filp, unsigned long addr,
			  unsigned long len, unsigned long pgoff,
			  unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma, *prev;
	struct vm_unmapped_area_info info;
	const unsigned long mmap_end = arch_get_mmap_end(addr, len, flags);

	if (len > mmap_end - mmap_min_addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	if (flags & MAP_FIXED)
		return addr;

	if (addr) {
		addr = PAGE_ALIGN(addr);
		vma = find_vma_prev(mm, addr, &prev);
<<<<<<< HEAD
		if (TASK_SIZE - len >= addr &&
=======
		if (mmap_end - len >= addr && addr >= mmap_min_addr &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    (!vma || addr + len <= vm_start_gap(vma)) &&
		    (!prev || addr >= vm_end_gap(prev)))
			return addr;
	}
<<<<<<< HEAD
	if (len > mm->cached_hole_size) {
	        start_addr = addr = mm->free_area_cache;
	} else {
	        start_addr = addr = TASK_UNMAPPED_BASE;
	        mm->cached_hole_size = 0;
	}

full_search:
	for (vma = find_vma_prev(mm, addr, &prev); ; prev = vma,
						vma = vma->vm_next) {
		if (prev) {
			prev_end = vm_end_gap(prev);
			if (addr < prev_end) {
				addr = prev_end;
				/* If vma already violates gap, forget it */
				if (vma && addr > vma->vm_start)
					addr = vma->vm_start;
			}
		}
		/* At this point:  (!vma || addr < vma->vm_end). */
		if (TASK_SIZE - len < addr) {
			/*
			 * Start a new search - just in case we missed
			 * some holes.
			 */
			if (start_addr != TASK_UNMAPPED_BASE) {
				addr = TASK_UNMAPPED_BASE;
			        start_addr = addr;
				mm->cached_hole_size = 0;
				goto full_search;
			}
			return -ENOMEM;
		}
		vm_start = vma ? vm_start_gap(vma) : TASK_SIZE;
		if (addr + len <= vm_start) {
			/*
			 * Remember the place where we stopped the search:
			 */
			mm->free_area_cache = addr + len;
			return addr;
		}
		if (addr + mm->cached_hole_size < vm_start)
			mm->cached_hole_size = vm_start - addr;
	}
}
#endif	

void arch_unmap_area(struct mm_struct *mm, unsigned long addr)
{
	/*
	 * Is this a new hole at the lowest possible address?
	 */
	if (addr >= TASK_UNMAPPED_BASE && addr < mm->free_area_cache)
		mm->free_area_cache = addr;
}
=======

	info.flags = 0;
	info.length = len;
	info.low_limit = mm->mmap_base;
	info.high_limit = mmap_end;
	info.align_mask = 0;
	info.align_offset = 0;
	return vm_unmapped_area(&info);
}

#ifndef HAVE_ARCH_UNMAPPED_AREA
unsigned long
arch_get_unmapped_area(struct file *filp, unsigned long addr,
		       unsigned long len, unsigned long pgoff,
		       unsigned long flags)
{
	return generic_get_unmapped_area(filp, addr, len, pgoff, flags);
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This mmap-allocator allocates new areas top-down from below the
 * stack's low limit (the base):
 */
<<<<<<< HEAD
#ifndef HAVE_ARCH_UNMAPPED_AREA_TOPDOWN
unsigned long
arch_get_unmapped_area_topdown(struct file *filp, const unsigned long addr0,
			  const unsigned long len, const unsigned long pgoff,
			  const unsigned long flags)
{
	struct vm_area_struct *vma, *prev;
	struct mm_struct *mm = current->mm;
	unsigned long addr = addr0, start_addr;
	unsigned long vm_start, prev_end;

	/* requested length too big for entire address space */
	if (len > TASK_SIZE)
=======
unsigned long
generic_get_unmapped_area_topdown(struct file *filp, unsigned long addr,
				  unsigned long len, unsigned long pgoff,
				  unsigned long flags)
{
	struct vm_area_struct *vma, *prev;
	struct mm_struct *mm = current->mm;
	struct vm_unmapped_area_info info;
	const unsigned long mmap_end = arch_get_mmap_end(addr, len, flags);

	/* requested length too big for entire address space */
	if (len > mmap_end - mmap_min_addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	if (flags & MAP_FIXED)
		return addr;

	/* requesting a specific address */
	if (addr) {
		addr = PAGE_ALIGN(addr);
		vma = find_vma_prev(mm, addr, &prev);
<<<<<<< HEAD
		if (TASK_SIZE - len >= addr &&
=======
		if (mmap_end - len >= addr && addr >= mmap_min_addr &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				(!vma || addr + len <= vm_start_gap(vma)) &&
				(!prev || addr >= vm_end_gap(prev)))
			return addr;
	}

<<<<<<< HEAD
	/* check if free_area_cache is useful for us */
	if (len <= mm->cached_hole_size) {
 	        mm->cached_hole_size = 0;
 		mm->free_area_cache = mm->mmap_base;
 	}

try_again:
	/* either no address requested or can't fit in requested address hole */
	start_addr = addr = mm->free_area_cache;

	if (addr < len)
		goto fail;

	addr -= len;
	do {
		/*
		 * Lookup failure means no vma is above this address,
		 * else if new region fits below vma->vm_start,
		 * return with success:
		 */
		vma = find_vma_prev(mm, addr, &prev);
		vm_start = vma ? vm_start_gap(vma) : mm->mmap_base;
		prev_end = prev ? vm_end_gap(prev) : low_limit;

		if (addr + len <= vm_start && addr >= prev_end)
			/* remember the address as a hint for next time */
			return (mm->free_area_cache = addr);

 		/* remember the largest hole we saw so far */
 		if (addr + mm->cached_hole_size < vm_start)
 		        mm->cached_hole_size = vm_start - addr;

		/* try just below the current vma->vm_start */
		addr = vm_start-len;
	} while (len < vm_start);

fail:
	/*
	 * if hint left us with no space for the requested
	 * mapping then try again:
	 *
	 * Note: this is different with the case of bottomup
	 * which does the fully line-search, but we use find_vma
	 * here that causes some holes skipped.
	 */
	if (start_addr != mm->mmap_base) {
		mm->free_area_cache = mm->mmap_base;
		mm->cached_hole_size = 0;
		goto try_again;
	}
=======
	info.flags = VM_UNMAPPED_AREA_TOPDOWN;
	info.length = len;
	info.low_limit = PAGE_SIZE;
	info.high_limit = arch_get_mmap_base(addr, mm->mmap_base);
	info.align_mask = 0;
	info.align_offset = 0;
	addr = vm_unmapped_area(&info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * A failed mmap() very likely causes application failure,
	 * so fall back to the bottom-up function here. This scenario
	 * can happen with large stack limits and large mmap()
	 * allocations.
	 */
<<<<<<< HEAD
	mm->cached_hole_size = ~0UL;
  	mm->free_area_cache = TASK_UNMAPPED_BASE;
	addr = arch_get_unmapped_area(filp, addr0, len, pgoff, flags);
	/*
	 * Restore the topdown base:
	 */
	mm->free_area_cache = mm->mmap_base;
	mm->cached_hole_size = ~0UL;

	return addr;
}
#endif

void arch_unmap_area_topdown(struct mm_struct *mm, unsigned long addr)
{
	/*
	 * Is this a new hole at the highest possible address?
	 */
	if (addr > mm->free_area_cache)
		mm->free_area_cache = addr;

	/* dont allow allocations above current base */
	if (mm->free_area_cache > mm->mmap_base)
		mm->free_area_cache = mm->mmap_base;
}
=======
	if (offset_in_page(addr)) {
		VM_BUG_ON(addr != -ENOMEM);
		info.flags = 0;
		info.low_limit = TASK_UNMAPPED_BASE;
		info.high_limit = mmap_end;
		addr = vm_unmapped_area(&info);
	}

	return addr;
}

#ifndef HAVE_ARCH_UNMAPPED_AREA_TOPDOWN
unsigned long
arch_get_unmapped_area_topdown(struct file *filp, unsigned long addr,
			       unsigned long len, unsigned long pgoff,
			       unsigned long flags)
{
	return generic_get_unmapped_area_topdown(filp, addr, len, pgoff, flags);
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

unsigned long
get_unmapped_area(struct file *file, unsigned long addr, unsigned long len,
		unsigned long pgoff, unsigned long flags)
{
	unsigned long (*get_area)(struct file *, unsigned long,
				  unsigned long, unsigned long, unsigned long);

	unsigned long error = arch_mmap_check(addr, len, flags);
	if (error)
		return error;

	/* Careful about overflows.. */
	if (len > TASK_SIZE)
		return -ENOMEM;

	get_area = current->mm->get_unmapped_area;
<<<<<<< HEAD
	if (file && file->f_op && file->f_op->get_unmapped_area)
		get_area = file->f_op->get_unmapped_area;
=======
	if (file) {
		if (file->f_op->get_unmapped_area)
			get_area = file->f_op->get_unmapped_area;
	} else if (flags & MAP_SHARED) {
		/*
		 * mmap_region() will call shmem_zero_setup() to create a file,
		 * so use shmem's get_unmapped_area in case it can be huge.
		 */
		get_area = shmem_get_unmapped_area;
	} else if (IS_ENABLED(CONFIG_TRANSPARENT_HUGEPAGE)) {
		/* Ensures that larger anonymous mappings are THP aligned. */
		get_area = thp_get_unmapped_area;
	}

	/* Always treat pgoff as zero for anonymous memory. */
	if (!file)
		pgoff = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	addr = get_area(file, addr, len, pgoff, flags);
	if (IS_ERR_VALUE(addr))
		return addr;

	if (addr > TASK_SIZE - len)
		return -ENOMEM;
<<<<<<< HEAD
	if (addr & ~PAGE_MASK)
		return -EINVAL;

	return arch_rebalance_pgtables(addr, len);
=======
	if (offset_in_page(addr))
		return -EINVAL;

	error = security_mmap_addr(addr);
	return error ? error : addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL(get_unmapped_area);

<<<<<<< HEAD
/* Look up the first VMA which satisfies  addr < vm_end,  NULL if none. */
struct vm_area_struct *find_vma(struct mm_struct *mm, unsigned long addr)
{
	struct vm_area_struct *vma = NULL;

	if (mm) {
		/* Check the cache first. */
		/* (Cache hit rate is typically around 35%.) */
		vma = ACCESS_ONCE(mm->mmap_cache);
		if (!(vma && vma->vm_end > addr && vma->vm_start <= addr)) {
			struct rb_node * rb_node;

			rb_node = mm->mm_rb.rb_node;
			vma = NULL;

			while (rb_node) {
				struct vm_area_struct * vma_tmp;

				vma_tmp = rb_entry(rb_node,
						struct vm_area_struct, vm_rb);

				if (vma_tmp->vm_end > addr) {
					vma = vma_tmp;
					if (vma_tmp->vm_start <= addr)
						break;
					rb_node = rb_node->rb_left;
				} else
					rb_node = rb_node->rb_right;
			}
			if (vma)
				mm->mmap_cache = vma;
		}
	}
	return vma;
}

EXPORT_SYMBOL(find_vma);

/*
 * Same as find_vma, but also return a pointer to the previous VMA in *pprev.
=======
/**
 * find_vma_intersection() - Look up the first VMA which intersects the interval
 * @mm: The process address space.
 * @start_addr: The inclusive start user address.
 * @end_addr: The exclusive end user address.
 *
 * Returns: The first VMA within the provided range, %NULL otherwise.  Assumes
 * start_addr < end_addr.
 */
struct vm_area_struct *find_vma_intersection(struct mm_struct *mm,
					     unsigned long start_addr,
					     unsigned long end_addr)
{
	unsigned long index = start_addr;

	mmap_assert_locked(mm);
	return mt_find(&mm->mm_mt, &index, end_addr - 1);
}
EXPORT_SYMBOL(find_vma_intersection);

/**
 * find_vma() - Find the VMA for a given address, or the next VMA.
 * @mm: The mm_struct to check
 * @addr: The address
 *
 * Returns: The VMA associated with addr, or the next VMA.
 * May return %NULL in the case of no VMA at addr or above.
 */
struct vm_area_struct *find_vma(struct mm_struct *mm, unsigned long addr)
{
	unsigned long index = addr;

	mmap_assert_locked(mm);
	return mt_find(&mm->mm_mt, &index, ULONG_MAX);
}
EXPORT_SYMBOL(find_vma);

/**
 * find_vma_prev() - Find the VMA for a given address, or the next vma and
 * set %pprev to the previous VMA, if any.
 * @mm: The mm_struct to check
 * @addr: The address
 * @pprev: The pointer to set to the previous VMA
 *
 * Note that RCU lock is missing here since the external mmap_lock() is used
 * instead.
 *
 * Returns: The VMA associated with @addr, or the next vma.
 * May return %NULL in the case of no vma at addr or above.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct vm_area_struct *
find_vma_prev(struct mm_struct *mm, unsigned long addr,
			struct vm_area_struct **pprev)
{
	struct vm_area_struct *vma;
<<<<<<< HEAD

	vma = find_vma(mm, addr);
	if (vma) {
		*pprev = vma->vm_prev;
	} else {
		struct rb_node *rb_node = mm->mm_rb.rb_node;
		*pprev = NULL;
		while (rb_node) {
			*pprev = rb_entry(rb_node, struct vm_area_struct, vm_rb);
			rb_node = rb_node->rb_right;
		}
	}
=======
	MA_STATE(mas, &mm->mm_mt, addr, addr);

	vma = mas_walk(&mas);
	*pprev = mas_prev(&mas, 0);
	if (!vma)
		vma = mas_next(&mas, ULONG_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return vma;
}

/*
 * Verify that the stack growth is acceptable and
 * update accounting. This is shared with both the
 * grow-up and grow-down cases.
 */
static int acct_stack_growth(struct vm_area_struct *vma,
			     unsigned long size, unsigned long grow)
{
	struct mm_struct *mm = vma->vm_mm;
<<<<<<< HEAD
	struct rlimit *rlim = current->signal->rlim;
	unsigned long new_start;

	/* address space limit tests */
	if (!may_expand_vm(mm, grow))
		return -ENOMEM;

	/* Stack limit test */
	if (size > ACCESS_ONCE(rlim[RLIMIT_STACK].rlim_cur))
		return -ENOMEM;

	/* mlock limit tests */
	if (vma->vm_flags & VM_LOCKED) {
		unsigned long locked;
		unsigned long limit;
		locked = mm->locked_vm + grow;
		limit = ACCESS_ONCE(rlim[RLIMIT_MEMLOCK].rlim_cur);
		limit >>= PAGE_SHIFT;
		if (locked > limit && !capable(CAP_IPC_LOCK))
			return -ENOMEM;
	}
=======
	unsigned long new_start;

	/* address space limit tests */
	if (!may_expand_vm(mm, vma->vm_flags, grow))
		return -ENOMEM;

	/* Stack limit test */
	if (size > rlimit(RLIMIT_STACK))
		return -ENOMEM;

	/* mlock limit tests */
	if (!mlock_future_ok(mm, vma->vm_flags, grow << PAGE_SHIFT))
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check to ensure the stack will not grow into a hugetlb-only region */
	new_start = (vma->vm_flags & VM_GROWSUP) ? vma->vm_start :
			vma->vm_end - size;
	if (is_hugepage_only_range(vma->vm_mm, new_start, size))
		return -EFAULT;

	/*
	 * Overcommit..  This must be the final test, as it will
	 * update security statistics.
	 */
	if (security_vm_enough_memory_mm(mm, grow))
		return -ENOMEM;

<<<<<<< HEAD
	/* Ok, everything looks good - let it rip */
	mm->total_vm += grow;
	if (vma->vm_flags & VM_LOCKED)
		mm->locked_vm += grow;
	vm_stat_account(mm, vma->vm_flags, vma->vm_file, grow);
	return 0;
}

#if defined(CONFIG_STACK_GROWSUP) || defined(CONFIG_IA64)
/*
 * PA-RISC uses this for its stack; IA64 for its Register Backing Store.
 * vma is the last one with address > vma->vm_end.  Have to extend vma.
 */
int expand_upwards(struct vm_area_struct *vma, unsigned long address)
{
	struct vm_area_struct *next;
	unsigned long gap_addr;
	int error = 0;
=======
	return 0;
}

#if defined(CONFIG_STACK_GROWSUP)
/*
 * PA-RISC uses this for its stack.
 * vma is the last one with address > vma->vm_end.  Have to extend vma.
 */
static int expand_upwards(struct vm_area_struct *vma, unsigned long address)
{
	struct mm_struct *mm = vma->vm_mm;
	struct vm_area_struct *next;
	unsigned long gap_addr;
	int error = 0;
	MA_STATE(mas, &mm->mm_mt, vma->vm_start, address);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!(vma->vm_flags & VM_GROWSUP))
		return -EFAULT;

	/* Guard against exceeding limits of the address space. */
	address &= PAGE_MASK;
<<<<<<< HEAD
	if (address >= TASK_SIZE)
=======
	if (address >= (TASK_SIZE & PAGE_MASK))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;
	address += PAGE_SIZE;

	/* Enforce stack_guard_gap */
	gap_addr = address + stack_guard_gap;

	/* Guard against overflow */
	if (gap_addr < address || gap_addr > TASK_SIZE)
		gap_addr = TASK_SIZE;

<<<<<<< HEAD
	next = vma->vm_next;
	if (next && next->vm_start < gap_addr) {
=======
	next = find_vma_intersection(mm, vma->vm_end, gap_addr);
	if (next && vma_is_accessible(next)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!(next->vm_flags & VM_GROWSUP))
			return -ENOMEM;
		/* Check that both stack segments have the same anon_vma? */
	}

<<<<<<< HEAD
	/* We must make sure the anon_vma is allocated. */
	if (unlikely(anon_vma_prepare(vma)))
		return -ENOMEM;

	/*
	 * vma->vm_start/vm_end cannot change under us because the caller
	 * is required to hold the mmap_sem in read mode.  We need the
	 * anon_vma lock to serialize against concurrent expand_stacks.
	 */
	vma_lock_anon_vma(vma);
=======
	if (next)
		mas_prev_range(&mas, address);

	__mas_set_range(&mas, vma->vm_start, address - 1);
	if (mas_preallocate(&mas, vma, GFP_KERNEL))
		return -ENOMEM;

	/* We must make sure the anon_vma is allocated. */
	if (unlikely(anon_vma_prepare(vma))) {
		mas_destroy(&mas);
		return -ENOMEM;
	}

	/* Lock the VMA before expanding to prevent concurrent page faults */
	vma_start_write(vma);
	/*
	 * vma->vm_start/vm_end cannot change under us because the caller
	 * is required to hold the mmap_lock in read mode.  We need the
	 * anon_vma lock to serialize against concurrent expand_stacks.
	 */
	anon_vma_lock_write(vma->anon_vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Somebody else might have raced and expanded it already */
	if (address > vma->vm_end) {
		unsigned long size, grow;

		size = address - vma->vm_start;
		grow = (address - vma->vm_end) >> PAGE_SHIFT;

		error = -ENOMEM;
		if (vma->vm_pgoff + (size >> PAGE_SHIFT) >= vma->vm_pgoff) {
			error = acct_stack_growth(vma, size, grow);
			if (!error) {
<<<<<<< HEAD
				vma->vm_end = address;
=======
				/*
				 * We only hold a shared mmap_lock lock here, so
				 * we need to protect against concurrent vma
				 * expansions.  anon_vma_lock_write() doesn't
				 * help here, as we don't guarantee that all
				 * growable vmas in a mm share the same root
				 * anon vma.  So, we reuse mm->page_table_lock
				 * to guard against concurrent vma expansions.
				 */
				spin_lock(&mm->page_table_lock);
				if (vma->vm_flags & VM_LOCKED)
					mm->locked_vm += grow;
				vm_stat_account(mm, vma->vm_flags, grow);
				anon_vma_interval_tree_pre_update_vma(vma);
				vma->vm_end = address;
				/* Overwrite old entry in mtree. */
				mas_store_prealloc(&mas, vma);
				anon_vma_interval_tree_post_update_vma(vma);
				spin_unlock(&mm->page_table_lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				perf_event_mmap(vma);
			}
		}
	}
<<<<<<< HEAD
	vma_unlock_anon_vma(vma);
	khugepaged_enter_vma_merge(vma, vma->vm_flags);
	return error;
}
#endif /* CONFIG_STACK_GROWSUP || CONFIG_IA64 */

/*
 * vma is the first one with address < vma->vm_start.  Have to extend vma.
 */
int expand_downwards(struct vm_area_struct *vma,
				   unsigned long address)
{
	struct vm_area_struct *prev;
	unsigned long gap_addr;
	int error;

	address &= PAGE_MASK;
	error = security_file_mmap(NULL, 0, 0, 0, address, 1);
	if (error)
		return error;

	/* Enforce stack_guard_gap */
	gap_addr = address - stack_guard_gap;
	if (gap_addr > address)
		return -ENOMEM;
	prev = vma->vm_prev;
	if (prev && prev->vm_end > gap_addr) {
		if (!(prev->vm_flags & VM_GROWSDOWN))
			return -ENOMEM;
		/* Check that both stack segments have the same anon_vma? */
	}

	/* We must make sure the anon_vma is allocated. */
	if (unlikely(anon_vma_prepare(vma)))
		return -ENOMEM;

	/*
	 * vma->vm_start/vm_end cannot change under us because the caller
	 * is required to hold the mmap_sem in read mode.  We need the
	 * anon_vma lock to serialize against concurrent expand_stacks.
	 */
	vma_lock_anon_vma(vma);
=======
	anon_vma_unlock_write(vma->anon_vma);
	mas_destroy(&mas);
	validate_mm(mm);
	return error;
}
#endif /* CONFIG_STACK_GROWSUP */

/*
 * vma is the first one with address < vma->vm_start.  Have to extend vma.
 * mmap_lock held for writing.
 */
int expand_downwards(struct vm_area_struct *vma, unsigned long address)
{
	struct mm_struct *mm = vma->vm_mm;
	MA_STATE(mas, &mm->mm_mt, vma->vm_start, vma->vm_start);
	struct vm_area_struct *prev;
	int error = 0;

	if (!(vma->vm_flags & VM_GROWSDOWN))
		return -EFAULT;

	address &= PAGE_MASK;
	if (address < mmap_min_addr || address < FIRST_USER_ADDRESS)
		return -EPERM;

	/* Enforce stack_guard_gap */
	prev = mas_prev(&mas, 0);
	/* Check that both stack segments have the same anon_vma? */
	if (prev) {
		if (!(prev->vm_flags & VM_GROWSDOWN) &&
		    vma_is_accessible(prev) &&
		    (address - prev->vm_end < stack_guard_gap))
			return -ENOMEM;
	}

	if (prev)
		mas_next_range(&mas, vma->vm_start);

	__mas_set_range(&mas, address, vma->vm_end - 1);
	if (mas_preallocate(&mas, vma, GFP_KERNEL))
		return -ENOMEM;

	/* We must make sure the anon_vma is allocated. */
	if (unlikely(anon_vma_prepare(vma))) {
		mas_destroy(&mas);
		return -ENOMEM;
	}

	/* Lock the VMA before expanding to prevent concurrent page faults */
	vma_start_write(vma);
	/*
	 * vma->vm_start/vm_end cannot change under us because the caller
	 * is required to hold the mmap_lock in read mode.  We need the
	 * anon_vma lock to serialize against concurrent expand_stacks.
	 */
	anon_vma_lock_write(vma->anon_vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Somebody else might have raced and expanded it already */
	if (address < vma->vm_start) {
		unsigned long size, grow;

		size = vma->vm_end - address;
		grow = (vma->vm_start - address) >> PAGE_SHIFT;

		error = -ENOMEM;
		if (grow <= vma->vm_pgoff) {
			error = acct_stack_growth(vma, size, grow);
			if (!error) {
<<<<<<< HEAD
				vma->vm_start = address;
				vma->vm_pgoff -= grow;
=======
				/*
				 * We only hold a shared mmap_lock lock here, so
				 * we need to protect against concurrent vma
				 * expansions.  anon_vma_lock_write() doesn't
				 * help here, as we don't guarantee that all
				 * growable vmas in a mm share the same root
				 * anon vma.  So, we reuse mm->page_table_lock
				 * to guard against concurrent vma expansions.
				 */
				spin_lock(&mm->page_table_lock);
				if (vma->vm_flags & VM_LOCKED)
					mm->locked_vm += grow;
				vm_stat_account(mm, vma->vm_flags, grow);
				anon_vma_interval_tree_pre_update_vma(vma);
				vma->vm_start = address;
				vma->vm_pgoff -= grow;
				/* Overwrite old entry in mtree. */
				mas_store_prealloc(&mas, vma);
				anon_vma_interval_tree_post_update_vma(vma);
				spin_unlock(&mm->page_table_lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				perf_event_mmap(vma);
			}
		}
	}
<<<<<<< HEAD
	vma_unlock_anon_vma(vma);
	khugepaged_enter_vma_merge(vma, vma->vm_flags);
=======
	anon_vma_unlock_write(vma->anon_vma);
	mas_destroy(&mas);
	validate_mm(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/* enforced gap between the expanding stack and other mappings. */
unsigned long stack_guard_gap = 256UL<<PAGE_SHIFT;

static int __init cmdline_parse_stack_guard_gap(char *p)
{
	unsigned long val;
	char *endptr;

	val = simple_strtoul(p, &endptr, 10);
	if (!*endptr)
		stack_guard_gap = val << PAGE_SHIFT;

<<<<<<< HEAD
	return 0;
=======
	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
__setup("stack_guard_gap=", cmdline_parse_stack_guard_gap);

#ifdef CONFIG_STACK_GROWSUP
<<<<<<< HEAD
int expand_stack(struct vm_area_struct *vma, unsigned long address)
=======
int expand_stack_locked(struct vm_area_struct *vma, unsigned long address)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return expand_upwards(vma, address);
}

<<<<<<< HEAD
struct vm_area_struct *
find_extend_vma(struct mm_struct *mm, unsigned long addr)
=======
struct vm_area_struct *find_extend_vma_locked(struct mm_struct *mm, unsigned long addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vm_area_struct *vma, *prev;

	addr &= PAGE_MASK;
	vma = find_vma_prev(mm, addr, &prev);
	if (vma && (vma->vm_start <= addr))
		return vma;
<<<<<<< HEAD
	if (!prev || expand_stack(prev, addr))
		return NULL;
	if (prev->vm_flags & VM_LOCKED) {
		mlock_vma_pages_range(prev, addr, prev->vm_end);
	}
	return prev;
}
#else
int expand_stack(struct vm_area_struct *vma, unsigned long address)
=======
	if (!prev)
		return NULL;
	if (expand_stack_locked(prev, addr))
		return NULL;
	if (prev->vm_flags & VM_LOCKED)
		populate_vma_page_range(prev, addr, prev->vm_end, NULL);
	return prev;
}
#else
int expand_stack_locked(struct vm_area_struct *vma, unsigned long address)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return expand_downwards(vma, address);
}

<<<<<<< HEAD
struct vm_area_struct *
find_extend_vma(struct mm_struct * mm, unsigned long addr)
{
	struct vm_area_struct * vma;
	unsigned long start;

	addr &= PAGE_MASK;
	vma = find_vma(mm,addr);
=======
struct vm_area_struct *find_extend_vma_locked(struct mm_struct *mm, unsigned long addr)
{
	struct vm_area_struct *vma;
	unsigned long start;

	addr &= PAGE_MASK;
	vma = find_vma(mm, addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!vma)
		return NULL;
	if (vma->vm_start <= addr)
		return vma;
<<<<<<< HEAD
	if (!(vma->vm_flags & VM_GROWSDOWN))
		return NULL;
	start = vma->vm_start;
	if (expand_stack(vma, addr))
		return NULL;
	if (vma->vm_flags & VM_LOCKED) {
		mlock_vma_pages_range(vma, addr, start);
	}
=======
	start = vma->vm_start;
	if (expand_stack_locked(vma, addr))
		return NULL;
	if (vma->vm_flags & VM_LOCKED)
		populate_vma_page_range(vma, addr, start, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return vma;
}
#endif

<<<<<<< HEAD
/*
 * Ok - we have the memory areas we should free on the vma list,
 * so release them, and do the vma updates.
 *
 * Called with the mm semaphore held.
 */
static void remove_vma_list(struct mm_struct *mm, struct vm_area_struct *vma)
{
	/* Update high watermark before we lower total_vm */
	update_hiwater_vm(mm);
	do {
		long nrpages = vma_pages(vma);

		mm->total_vm -= nrpages;
		vm_stat_account(mm, vma->vm_flags, vma->vm_file, -nrpages);
		vma = remove_vma(vma);
	} while (vma);
	validate_mm(mm);
=======
#if defined(CONFIG_STACK_GROWSUP)

#define vma_expand_up(vma,addr) expand_upwards(vma, addr)
#define vma_expand_down(vma, addr) (-EFAULT)

#else

#define vma_expand_up(vma,addr) (-EFAULT)
#define vma_expand_down(vma, addr) expand_downwards(vma, addr)

#endif

/*
 * expand_stack(): legacy interface for page faulting. Don't use unless
 * you have to.
 *
 * This is called with the mm locked for reading, drops the lock, takes
 * the lock for writing, tries to look up a vma again, expands it if
 * necessary, and downgrades the lock to reading again.
 *
 * If no vma is found or it can't be expanded, it returns NULL and has
 * dropped the lock.
 */
struct vm_area_struct *expand_stack(struct mm_struct *mm, unsigned long addr)
{
	struct vm_area_struct *vma, *prev;

	mmap_read_unlock(mm);
	if (mmap_write_lock_killable(mm))
		return NULL;

	vma = find_vma_prev(mm, addr, &prev);
	if (vma && vma->vm_start <= addr)
		goto success;

	if (prev && !vma_expand_up(prev, addr)) {
		vma = prev;
		goto success;
	}

	if (vma && !vma_expand_down(vma, addr))
		goto success;

	mmap_write_unlock(mm);
	return NULL;

success:
	mmap_write_downgrade(mm);
	return vma;
}

/*
 * Ok - we have the memory areas we should free on a maple tree so release them,
 * and do the vma updates.
 *
 * Called with the mm semaphore held.
 */
static inline void remove_mt(struct mm_struct *mm, struct ma_state *mas)
{
	unsigned long nr_accounted = 0;
	struct vm_area_struct *vma;

	/* Update high watermark before we lower total_vm */
	update_hiwater_vm(mm);
	mas_for_each(mas, vma, ULONG_MAX) {
		long nrpages = vma_pages(vma);

		if (vma->vm_flags & VM_ACCOUNT)
			nr_accounted += nrpages;
		vm_stat_account(mm, vma->vm_flags, -nrpages);
		remove_vma(vma, false);
	}
	vm_unacct_memory(nr_accounted);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Get rid of page table information in the indicated region.
 *
 * Called with the mm semaphore held.
 */
<<<<<<< HEAD
static void unmap_region(struct mm_struct *mm,
		struct vm_area_struct *vma, struct vm_area_struct *prev,
		unsigned long start, unsigned long end)
{
	struct vm_area_struct *next = prev? prev->vm_next: mm->mmap;
	struct mmu_gather tlb;
	unsigned long nr_accounted = 0;

	lru_add_drain();
	tlb_gather_mmu(&tlb, mm, 0);
	update_hiwater_rss(mm);
	unmap_vmas(&tlb, vma, start, end, &nr_accounted, NULL);
	vm_unacct_memory(nr_accounted);
	free_pgtables(&tlb, vma, prev ? prev->vm_end : FIRST_USER_ADDRESS,
				 next ? next->vm_start : USER_PGTABLES_CEILING);
	tlb_finish_mmu(&tlb, start, end);
}

/*
 * Create a list of vma's touched by the unmap, removing them from the mm's
 * vma list as we go..
 */
static void
detach_vmas_to_be_unmapped(struct mm_struct *mm, struct vm_area_struct *vma,
	struct vm_area_struct *prev, unsigned long end)
{
	struct vm_area_struct **insertion_point;
	struct vm_area_struct *tail_vma = NULL;
	unsigned long addr;

	insertion_point = (prev ? &prev->vm_next : &mm->mmap);
	vma->vm_prev = NULL;
	do {
		rb_erase(&vma->vm_rb, &mm->mm_rb);
		mm->map_count--;
		tail_vma = vma;
		vma = vma->vm_next;
	} while (vma && vma->vm_start < end);
	*insertion_point = vma;
	if (vma)
		vma->vm_prev = prev;
	tail_vma->vm_next = NULL;
	if (mm->unmap_area == arch_unmap_area)
		addr = prev ? prev->vm_end : mm->mmap_base;
	else
		addr = vma ?  vma->vm_start : mm->mmap_base;
	mm->unmap_area(mm, addr);
	mm->mmap_cache = NULL;		/* Kill the cache. */
}

/*
 * __split_vma() bypasses sysctl_max_map_count checking.  We use this on the
 * munmap path where it doesn't make sense to fail.
 */
static int __split_vma(struct mm_struct * mm, struct vm_area_struct * vma,
	      unsigned long addr, int new_below)
{
	struct mempolicy *pol;
	struct vm_area_struct *new;
	int err = -ENOMEM;

	if (is_vm_hugetlb_page(vma) && (addr &
					~(huge_page_mask(hstate_vma(vma)))))
		return -EINVAL;

	new = kmem_cache_alloc(vm_area_cachep, GFP_KERNEL);
	if (!new)
		goto out_err;

	/* most fields are the same, copy all, and then fixup */
	*new = *vma;

	INIT_LIST_HEAD(&new->anon_vma_chain);

	if (new_below)
		new->vm_end = addr;
	else {
=======
static void unmap_region(struct mm_struct *mm, struct ma_state *mas,
		struct vm_area_struct *vma, struct vm_area_struct *prev,
		struct vm_area_struct *next, unsigned long start,
		unsigned long end, unsigned long tree_end, bool mm_wr_locked)
{
	struct mmu_gather tlb;
	unsigned long mt_start = mas->index;

	lru_add_drain();
	tlb_gather_mmu(&tlb, mm);
	update_hiwater_rss(mm);
	unmap_vmas(&tlb, mas, vma, start, end, tree_end, mm_wr_locked);
	mas_set(mas, mt_start);
	free_pgtables(&tlb, mas, vma, prev ? prev->vm_end : FIRST_USER_ADDRESS,
				 next ? next->vm_start : USER_PGTABLES_CEILING,
				 mm_wr_locked);
	tlb_finish_mmu(&tlb);
}

/*
 * __split_vma() bypasses sysctl_max_map_count checking.  We use this where it
 * has already been checked or doesn't make sense to fail.
 * VMA Iterator will point to the end VMA.
 */
static int __split_vma(struct vma_iterator *vmi, struct vm_area_struct *vma,
		       unsigned long addr, int new_below)
{
	struct vma_prepare vp;
	struct vm_area_struct *new;
	int err;

	WARN_ON(vma->vm_start >= addr);
	WARN_ON(vma->vm_end <= addr);

	if (vma->vm_ops && vma->vm_ops->may_split) {
		err = vma->vm_ops->may_split(vma, addr);
		if (err)
			return err;
	}

	new = vm_area_dup(vma);
	if (!new)
		return -ENOMEM;

	if (new_below) {
		new->vm_end = addr;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		new->vm_start = addr;
		new->vm_pgoff += ((addr - vma->vm_start) >> PAGE_SHIFT);
	}

<<<<<<< HEAD
	pol = mpol_dup(vma_policy(vma));
	if (IS_ERR(pol)) {
		err = PTR_ERR(pol);
		goto out_free_vma;
	}
	vma_set_policy(new, pol);

	if (anon_vma_clone(new, vma))
		goto out_free_mpol;

	if (new->vm_file) {
		get_file(new->vm_file);
		if (vma->vm_flags & VM_EXECUTABLE)
			added_exe_file_vma(mm);
	}
=======
	err = -ENOMEM;
	vma_iter_config(vmi, new->vm_start, new->vm_end);
	if (vma_iter_prealloc(vmi, new))
		goto out_free_vma;

	err = vma_dup_policy(vma, new);
	if (err)
		goto out_free_vmi;

	err = anon_vma_clone(new, vma);
	if (err)
		goto out_free_mpol;

	if (new->vm_file)
		get_file(new->vm_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (new->vm_ops && new->vm_ops->open)
		new->vm_ops->open(new);

<<<<<<< HEAD
	if (new_below)
		err = vma_adjust(vma, addr, vma->vm_end, vma->vm_pgoff +
			((addr - new->vm_start) >> PAGE_SHIFT), new);
	else
		err = vma_adjust(vma, vma->vm_start, addr, vma->vm_pgoff, new);

	/* Success. */
	if (!err)
		return 0;

	/* Clean everything up if vma_adjust failed. */
	if (new->vm_ops && new->vm_ops->close)
		new->vm_ops->close(new);
	if (new->vm_file) {
		if (vma->vm_flags & VM_EXECUTABLE)
			removed_exe_file_vma(mm);
		fput(new->vm_file);
	}
	unlink_anon_vmas(new);
 out_free_mpol:
	mpol_put(pol);
 out_free_vma:
	kmem_cache_free(vm_area_cachep, new);
 out_err:
=======
	vma_start_write(vma);
	vma_start_write(new);

	init_vma_prep(&vp, vma);
	vp.insert = new;
	vma_prepare(&vp);
	vma_adjust_trans_huge(vma, vma->vm_start, addr, 0);

	if (new_below) {
		vma->vm_start = addr;
		vma->vm_pgoff += (addr - new->vm_start) >> PAGE_SHIFT;
	} else {
		vma->vm_end = addr;
	}

	/* vma_complete stores the new vma */
	vma_complete(&vp, vmi, vma->vm_mm);

	/* Success. */
	if (new_below)
		vma_next(vmi);
	return 0;

out_free_mpol:
	mpol_put(vma_policy(new));
out_free_vmi:
	vma_iter_free(vmi);
out_free_vma:
	vm_area_free(new);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/*
 * Split a vma into two pieces at address 'addr', a new vma is allocated
 * either for the first part or the tail.
 */
<<<<<<< HEAD
int split_vma(struct mm_struct *mm, struct vm_area_struct *vma,
	      unsigned long addr, int new_below)
{
	if (mm->map_count >= sysctl_max_map_count)
		return -ENOMEM;

	return __split_vma(mm, vma, addr, new_below);
}

/* Munmap is split into 2 main parts -- this part which finds
 * what needs doing, and the areas themselves, which do the
 * work.  This now handles partial unmappings.
 * Jeremy Fitzhardinge <jeremy@goop.org>
 */
int do_munmap(struct mm_struct *mm, unsigned long start, size_t len)
{
	unsigned long end;
	struct vm_area_struct *vma, *prev, *last;

	if ((start & ~PAGE_MASK) || start > TASK_SIZE || len > TASK_SIZE-start)
		return -EINVAL;

	if ((len = PAGE_ALIGN(len)) == 0)
		return -EINVAL;

	/* Find the first overlapping VMA */
	vma = find_vma(mm, start);
	if (!vma)
		return 0;
	prev = vma->vm_prev;
	/* we have  start < vma->vm_end  */

	/* if it doesn't overlap, we have nothing.. */
	end = start + len;
	if (vma->vm_start >= end)
		return 0;
=======
static int split_vma(struct vma_iterator *vmi, struct vm_area_struct *vma,
		     unsigned long addr, int new_below)
{
	if (vma->vm_mm->map_count >= sysctl_max_map_count)
		return -ENOMEM;

	return __split_vma(vmi, vma, addr, new_below);
}

/*
 * We are about to modify one or multiple of a VMA's flags, policy, userfaultfd
 * context and anonymous VMA name within the range [start, end).
 *
 * As a result, we might be able to merge the newly modified VMA range with an
 * adjacent VMA with identical properties.
 *
 * If no merge is possible and the range does not span the entirety of the VMA,
 * we then need to split the VMA to accommodate the change.
 *
 * The function returns either the merged VMA, the original VMA if a split was
 * required instead, or an error if the split failed.
 */
struct vm_area_struct *vma_modify(struct vma_iterator *vmi,
				  struct vm_area_struct *prev,
				  struct vm_area_struct *vma,
				  unsigned long start, unsigned long end,
				  unsigned long vm_flags,
				  struct mempolicy *policy,
				  struct vm_userfaultfd_ctx uffd_ctx,
				  struct anon_vma_name *anon_name)
{
	pgoff_t pgoff = vma->vm_pgoff + ((start - vma->vm_start) >> PAGE_SHIFT);
	struct vm_area_struct *merged;

	merged = vma_merge(vmi, prev, vma, start, end, vm_flags,
			   pgoff, policy, uffd_ctx, anon_name);
	if (merged)
		return merged;

	if (vma->vm_start < start) {
		int err = split_vma(vmi, vma, start, 1);

		if (err)
			return ERR_PTR(err);
	}

	if (vma->vm_end > end) {
		int err = split_vma(vmi, vma, end, 0);

		if (err)
			return ERR_PTR(err);
	}

	return vma;
}

/*
 * Attempt to merge a newly mapped VMA with those adjacent to it. The caller
 * must ensure that [start, end) does not overlap any existing VMA.
 */
static struct vm_area_struct
*vma_merge_new_vma(struct vma_iterator *vmi, struct vm_area_struct *prev,
		   struct vm_area_struct *vma, unsigned long start,
		   unsigned long end, pgoff_t pgoff)
{
	return vma_merge(vmi, prev, vma, start, end, vma->vm_flags, pgoff,
			 vma_policy(vma), vma->vm_userfaultfd_ctx, anon_vma_name(vma));
}

/*
 * Expand vma by delta bytes, potentially merging with an immediately adjacent
 * VMA with identical properties.
 */
struct vm_area_struct *vma_merge_extend(struct vma_iterator *vmi,
					struct vm_area_struct *vma,
					unsigned long delta)
{
	pgoff_t pgoff = vma->vm_pgoff + vma_pages(vma);

	/* vma is specified as prev, so case 1 or 2 will apply. */
	return vma_merge(vmi, vma, vma, vma->vm_end, vma->vm_end + delta,
			 vma->vm_flags, pgoff, vma_policy(vma),
			 vma->vm_userfaultfd_ctx, anon_vma_name(vma));
}

/*
 * do_vmi_align_munmap() - munmap the aligned region from @start to @end.
 * @vmi: The vma iterator
 * @vma: The starting vm_area_struct
 * @mm: The mm_struct
 * @start: The aligned start address to munmap.
 * @end: The aligned end address to munmap.
 * @uf: The userfaultfd list_head
 * @unlock: Set to true to drop the mmap_lock.  unlocking only happens on
 * success.
 *
 * Return: 0 on success and drops the lock if so directed, error and leaves the
 * lock held otherwise.
 */
static int
do_vmi_align_munmap(struct vma_iterator *vmi, struct vm_area_struct *vma,
		    struct mm_struct *mm, unsigned long start,
		    unsigned long end, struct list_head *uf, bool unlock)
{
	struct vm_area_struct *prev, *next = NULL;
	struct maple_tree mt_detach;
	int count = 0;
	int error = -ENOMEM;
	unsigned long locked_vm = 0;
	MA_STATE(mas_detach, &mt_detach, 0, 0);
	mt_init_flags(&mt_detach, vmi->mas.tree->ma_flags & MT_FLAGS_LOCK_MASK);
	mt_on_stack(mt_detach);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If we need to split any vma, do it now to save pain later.
	 *
	 * Note: mremap's move_vma VM_ACCOUNT handling assumes a partially
	 * unmapped vm_area_struct will remain in use: so lower split_vma
	 * places tmp vma above, and higher split_vma places tmp vma below.
	 */
<<<<<<< HEAD
	if (start > vma->vm_start) {
		int error;
=======

	/* Does it split the first one? */
	if (start > vma->vm_start) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Make sure that map_count on return from munmap() will
		 * not exceed its limit; but let map_count go just above
		 * its limit temporarily, to help free resources as expected.
		 */
		if (end < vma->vm_end && mm->map_count >= sysctl_max_map_count)
<<<<<<< HEAD
			return -ENOMEM;

		error = __split_vma(mm, vma, start, 0);
		if (error)
			return error;
		prev = vma;
	}

	/* Does it split the last one? */
	last = find_vma(mm, end);
	if (last && end > last->vm_start) {
		int error = __split_vma(mm, last, end, 1);
		if (error)
			return error;
	}
	vma = prev? prev->vm_next: mm->mmap;

	/*
	 * unlock any mlock()ed ranges before detaching vmas
	 */
	if (mm->locked_vm) {
		struct vm_area_struct *tmp = vma;
		while (tmp && tmp->vm_start < end) {
			if (tmp->vm_flags & VM_LOCKED) {
				mm->locked_vm -= vma_pages(tmp);
				munlock_vma_pages_all(tmp);
			}
			tmp = tmp->vm_next;
		}
	}

	/*
	 * Remove the vma's, and unmap the actual pages
	 */
	detach_vmas_to_be_unmapped(mm, vma, prev, end);
	unmap_region(mm, vma, prev, start, end);

	/* Fix up all other VM information */
	remove_vma_list(mm, vma);

	return 0;
}
EXPORT_SYMBOL(do_munmap);

int vm_munmap(unsigned long start, size_t len)
{
	int ret;
	struct mm_struct *mm = current->mm;

	down_write(&mm->mmap_sem);
	ret = do_munmap(mm, start, len);
	up_write(&mm->mmap_sem);
	return ret;
}
=======
			goto map_count_exceeded;

		error = __split_vma(vmi, vma, start, 1);
		if (error)
			goto start_split_failed;
	}

	/*
	 * Detach a range of VMAs from the mm. Using next as a temp variable as
	 * it is always overwritten.
	 */
	next = vma;
	do {
		/* Does it split the end? */
		if (next->vm_end > end) {
			error = __split_vma(vmi, next, end, 0);
			if (error)
				goto end_split_failed;
		}
		vma_start_write(next);
		mas_set(&mas_detach, count);
		error = mas_store_gfp(&mas_detach, next, GFP_KERNEL);
		if (error)
			goto munmap_gather_failed;
		vma_mark_detached(next, true);
		if (next->vm_flags & VM_LOCKED)
			locked_vm += vma_pages(next);

		count++;
		if (unlikely(uf)) {
			/*
			 * If userfaultfd_unmap_prep returns an error the vmas
			 * will remain split, but userland will get a
			 * highly unexpected error anyway. This is no
			 * different than the case where the first of the two
			 * __split_vma fails, but we don't undo the first
			 * split, despite we could. This is unlikely enough
			 * failure that it's not worth optimizing it for.
			 */
			error = userfaultfd_unmap_prep(next, start, end, uf);

			if (error)
				goto userfaultfd_error;
		}
#ifdef CONFIG_DEBUG_VM_MAPLE_TREE
		BUG_ON(next->vm_start < start);
		BUG_ON(next->vm_start > end);
#endif
	} for_each_vma_range(*vmi, next, end);

#if defined(CONFIG_DEBUG_VM_MAPLE_TREE)
	/* Make sure no VMAs are about to be lost. */
	{
		MA_STATE(test, &mt_detach, 0, 0);
		struct vm_area_struct *vma_mas, *vma_test;
		int test_count = 0;

		vma_iter_set(vmi, start);
		rcu_read_lock();
		vma_test = mas_find(&test, count - 1);
		for_each_vma_range(*vmi, vma_mas, end) {
			BUG_ON(vma_mas != vma_test);
			test_count++;
			vma_test = mas_next(&test, count - 1);
		}
		rcu_read_unlock();
		BUG_ON(count != test_count);
	}
#endif

	while (vma_iter_addr(vmi) > start)
		vma_iter_prev_range(vmi);

	error = vma_iter_clear_gfp(vmi, start, end, GFP_KERNEL);
	if (error)
		goto clear_tree_failed;

	/* Point of no return */
	mm->locked_vm -= locked_vm;
	mm->map_count -= count;
	if (unlock)
		mmap_write_downgrade(mm);

	prev = vma_iter_prev_range(vmi);
	next = vma_next(vmi);
	if (next)
		vma_iter_prev_range(vmi);

	/*
	 * We can free page tables without write-locking mmap_lock because VMAs
	 * were isolated before we downgraded mmap_lock.
	 */
	mas_set(&mas_detach, 1);
	unmap_region(mm, &mas_detach, vma, prev, next, start, end, count,
		     !unlock);
	/* Statistics and freeing VMAs */
	mas_set(&mas_detach, 0);
	remove_mt(mm, &mas_detach);
	validate_mm(mm);
	if (unlock)
		mmap_read_unlock(mm);

	__mt_destroy(&mt_detach);
	return 0;

clear_tree_failed:
userfaultfd_error:
munmap_gather_failed:
end_split_failed:
	mas_set(&mas_detach, 0);
	mas_for_each(&mas_detach, next, end)
		vma_mark_detached(next, false);

	__mt_destroy(&mt_detach);
start_split_failed:
map_count_exceeded:
	validate_mm(mm);
	return error;
}

/*
 * do_vmi_munmap() - munmap a given range.
 * @vmi: The vma iterator
 * @mm: The mm_struct
 * @start: The start address to munmap
 * @len: The length of the range to munmap
 * @uf: The userfaultfd list_head
 * @unlock: set to true if the user wants to drop the mmap_lock on success
 *
 * This function takes a @mas that is either pointing to the previous VMA or set
 * to MA_START and sets it up to remove the mapping(s).  The @len will be
 * aligned and any arch_unmap work will be preformed.
 *
 * Return: 0 on success and drops the lock if so directed, error and leaves the
 * lock held otherwise.
 */
int do_vmi_munmap(struct vma_iterator *vmi, struct mm_struct *mm,
		  unsigned long start, size_t len, struct list_head *uf,
		  bool unlock)
{
	unsigned long end;
	struct vm_area_struct *vma;

	if ((offset_in_page(start)) || start > TASK_SIZE || len > TASK_SIZE-start)
		return -EINVAL;

	end = start + PAGE_ALIGN(len);
	if (end == start)
		return -EINVAL;

	 /* arch_unmap() might do unmaps itself.  */
	arch_unmap(mm, start, end);

	/* Find the first overlapping VMA */
	vma = vma_find(vmi, end);
	if (!vma) {
		if (unlock)
			mmap_write_unlock(mm);
		return 0;
	}

	return do_vmi_align_munmap(vmi, vma, mm, start, end, uf, unlock);
}

/* do_munmap() - Wrapper function for non-maple tree aware do_munmap() calls.
 * @mm: The mm_struct
 * @start: The start address to munmap
 * @len: The length to be munmapped.
 * @uf: The userfaultfd list_head
 *
 * Return: 0 on success, error otherwise.
 */
int do_munmap(struct mm_struct *mm, unsigned long start, size_t len,
	      struct list_head *uf)
{
	VMA_ITERATOR(vmi, mm, start);

	return do_vmi_munmap(&vmi, mm, start, len, uf, false);
}

unsigned long mmap_region(struct file *file, unsigned long addr,
		unsigned long len, vm_flags_t vm_flags, unsigned long pgoff,
		struct list_head *uf)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma = NULL;
	struct vm_area_struct *next, *prev, *merge;
	pgoff_t pglen = len >> PAGE_SHIFT;
	unsigned long charged = 0;
	unsigned long end = addr + len;
	unsigned long merge_start = addr, merge_end = end;
	bool writable_file_mapping = false;
	pgoff_t vm_pgoff;
	int error;
	VMA_ITERATOR(vmi, mm, addr);

	/* Check against address space limit. */
	if (!may_expand_vm(mm, vm_flags, len >> PAGE_SHIFT)) {
		unsigned long nr_pages;

		/*
		 * MAP_FIXED may remove pages of mappings that intersects with
		 * requested mapping. Account for the pages it would unmap.
		 */
		nr_pages = count_vma_pages_range(mm, addr, end);

		if (!may_expand_vm(mm, vm_flags,
					(len >> PAGE_SHIFT) - nr_pages))
			return -ENOMEM;
	}

	/* Unmap any existing mapping in the area */
	if (do_vmi_munmap(&vmi, mm, addr, len, uf, false))
		return -ENOMEM;

	/*
	 * Private writable mapping: check memory availability
	 */
	if (accountable_mapping(file, vm_flags)) {
		charged = len >> PAGE_SHIFT;
		if (security_vm_enough_memory_mm(mm, charged))
			return -ENOMEM;
		vm_flags |= VM_ACCOUNT;
	}

	next = vma_next(&vmi);
	prev = vma_prev(&vmi);
	if (vm_flags & VM_SPECIAL) {
		if (prev)
			vma_iter_next_range(&vmi);
		goto cannot_expand;
	}

	/* Attempt to expand an old mapping */
	/* Check next */
	if (next && next->vm_start == end && !vma_policy(next) &&
	    can_vma_merge_before(next, vm_flags, NULL, file, pgoff+pglen,
				 NULL_VM_UFFD_CTX, NULL)) {
		merge_end = next->vm_end;
		vma = next;
		vm_pgoff = next->vm_pgoff - pglen;
	}

	/* Check prev */
	if (prev && prev->vm_end == addr && !vma_policy(prev) &&
	    (vma ? can_vma_merge_after(prev, vm_flags, vma->anon_vma, file,
				       pgoff, vma->vm_userfaultfd_ctx, NULL) :
		   can_vma_merge_after(prev, vm_flags, NULL, file, pgoff,
				       NULL_VM_UFFD_CTX, NULL))) {
		merge_start = prev->vm_start;
		vma = prev;
		vm_pgoff = prev->vm_pgoff;
	} else if (prev) {
		vma_iter_next_range(&vmi);
	}

	/* Actually expand, if possible */
	if (vma &&
	    !vma_expand(&vmi, vma, merge_start, merge_end, vm_pgoff, next)) {
		khugepaged_enter_vma(vma, vm_flags);
		goto expanded;
	}

	if (vma == prev)
		vma_iter_set(&vmi, addr);
cannot_expand:

	/*
	 * Determine the object being mapped and call the appropriate
	 * specific mapper. the address has already been validated, but
	 * not unmapped, but the maps are removed from the list.
	 */
	vma = vm_area_alloc(mm);
	if (!vma) {
		error = -ENOMEM;
		goto unacct_error;
	}

	vma_iter_config(&vmi, addr, end);
	vma_set_range(vma, addr, end, pgoff);
	vm_flags_init(vma, vm_flags);
	vma->vm_page_prot = vm_get_page_prot(vm_flags);

	if (file) {
		vma->vm_file = get_file(file);
		error = call_mmap(file, vma);
		if (error)
			goto unmap_and_free_vma;

		if (vma_is_shared_maywrite(vma)) {
			error = mapping_map_writable(file->f_mapping);
			if (error)
				goto close_and_free_vma;

			writable_file_mapping = true;
		}

		/*
		 * Expansion is handled above, merging is handled below.
		 * Drivers should not alter the address of the VMA.
		 */
		error = -EINVAL;
		if (WARN_ON((addr != vma->vm_start)))
			goto close_and_free_vma;

		vma_iter_config(&vmi, addr, end);
		/*
		 * If vm_flags changed after call_mmap(), we should try merge
		 * vma again as we may succeed this time.
		 */
		if (unlikely(vm_flags != vma->vm_flags && prev)) {
			merge = vma_merge_new_vma(&vmi, prev, vma,
						  vma->vm_start, vma->vm_end,
						  vma->vm_pgoff);
			if (merge) {
				/*
				 * ->mmap() can change vma->vm_file and fput
				 * the original file. So fput the vma->vm_file
				 * here or we would add an extra fput for file
				 * and cause general protection fault
				 * ultimately.
				 */
				fput(vma->vm_file);
				vm_area_free(vma);
				vma = merge;
				/* Update vm_flags to pick up the change. */
				vm_flags = vma->vm_flags;
				goto unmap_writable;
			}
		}

		vm_flags = vma->vm_flags;
	} else if (vm_flags & VM_SHARED) {
		error = shmem_zero_setup(vma);
		if (error)
			goto free_vma;
	} else {
		vma_set_anonymous(vma);
	}

	if (map_deny_write_exec(vma, vma->vm_flags)) {
		error = -EACCES;
		goto close_and_free_vma;
	}

	/* Allow architectures to sanity-check the vm_flags */
	error = -EINVAL;
	if (!arch_validate_flags(vma->vm_flags))
		goto close_and_free_vma;

	error = -ENOMEM;
	if (vma_iter_prealloc(&vmi, vma))
		goto close_and_free_vma;

	/* Lock the VMA since it is modified after insertion into VMA tree */
	vma_start_write(vma);
	vma_iter_store(&vmi, vma);
	mm->map_count++;
	vma_link_file(vma);

	/*
	 * vma_merge() calls khugepaged_enter_vma() either, the below
	 * call covers the non-merge case.
	 */
	khugepaged_enter_vma(vma, vma->vm_flags);

	/* Once vma denies write, undo our temporary denial count */
unmap_writable:
	if (writable_file_mapping)
		mapping_unmap_writable(file->f_mapping);
	file = vma->vm_file;
	ksm_add_vma(vma);
expanded:
	perf_event_mmap(vma);

	vm_stat_account(mm, vm_flags, len >> PAGE_SHIFT);
	if (vm_flags & VM_LOCKED) {
		if ((vm_flags & VM_SPECIAL) || vma_is_dax(vma) ||
					is_vm_hugetlb_page(vma) ||
					vma == get_gate_vma(current->mm))
			vm_flags_clear(vma, VM_LOCKED_MASK);
		else
			mm->locked_vm += (len >> PAGE_SHIFT);
	}

	if (file)
		uprobe_mmap(vma);

	/*
	 * New (or expanded) vma always get soft dirty status.
	 * Otherwise user-space soft-dirty page tracker won't
	 * be able to distinguish situation when vma area unmapped,
	 * then new mapped in-place (which must be aimed as
	 * a completely new data area).
	 */
	vm_flags_set(vma, VM_SOFTDIRTY);

	vma_set_page_prot(vma);

	validate_mm(mm);
	return addr;

close_and_free_vma:
	if (file && vma->vm_ops && vma->vm_ops->close)
		vma->vm_ops->close(vma);

	if (file || vma->vm_file) {
unmap_and_free_vma:
		fput(vma->vm_file);
		vma->vm_file = NULL;

		vma_iter_set(&vmi, vma->vm_end);
		/* Undo any partial mapping done by a device driver. */
		unmap_region(mm, &vmi.mas, vma, prev, next, vma->vm_start,
			     vma->vm_end, vma->vm_end, true);
	}
	if (writable_file_mapping)
		mapping_unmap_writable(file->f_mapping);
free_vma:
	vm_area_free(vma);
unacct_error:
	if (charged)
		vm_unacct_memory(charged);
	validate_mm(mm);
	return error;
}

static int __vm_munmap(unsigned long start, size_t len, bool unlock)
{
	int ret;
	struct mm_struct *mm = current->mm;
	LIST_HEAD(uf);
	VMA_ITERATOR(vmi, mm, start);

	if (mmap_write_lock_killable(mm))
		return -EINTR;

	ret = do_vmi_munmap(&vmi, mm, start, len, &uf, unlock);
	if (ret || !unlock)
		mmap_write_unlock(mm);

	userfaultfd_unmap_complete(mm, &uf);
	return ret;
}

int vm_munmap(unsigned long start, size_t len)
{
	return __vm_munmap(start, len, false);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(vm_munmap);

SYSCALL_DEFINE2(munmap, unsigned long, addr, size_t, len)
{
<<<<<<< HEAD
	profile_munmap(addr);
	return vm_munmap(addr, len);
}

static inline void verify_mm_writelocked(struct mm_struct *mm)
{
#ifdef CONFIG_DEBUG_VM
	if (unlikely(down_read_trylock(&mm->mmap_sem))) {
		WARN_ON(1);
		up_read(&mm->mmap_sem);
	}
#endif
}

/*
 *  this is really a simplified "do_mmap".  it only handles
 *  anonymous maps.  eventually we may be able to do some
 *  brk-specific accounting here.
 */
static unsigned long do_brk(unsigned long addr, unsigned long len)
{
	struct mm_struct * mm = current->mm;
	struct vm_area_struct * vma, * prev;
	unsigned long flags;
	struct rb_node ** rb_link, * rb_parent;
	pgoff_t pgoff = addr >> PAGE_SHIFT;
	int error;

	len = PAGE_ALIGN(len);
	if (!len)
		return addr;

	error = security_file_mmap(NULL, 0, 0, 0, addr, 1);
	if (error)
		return error;

	flags = VM_DATA_DEFAULT_FLAGS | VM_ACCOUNT | mm->def_flags;

	error = get_unmapped_area(NULL, addr, len, 0, MAP_FIXED);
	if (error & ~PAGE_MASK)
		return error;

	/*
	 * mlock MCL_FUTURE?
	 */
	if (mm->def_flags & VM_LOCKED) {
		unsigned long locked, lock_limit;
		locked = len >> PAGE_SHIFT;
		locked += mm->locked_vm;
		lock_limit = rlimit(RLIMIT_MEMLOCK);
		lock_limit >>= PAGE_SHIFT;
		if (locked > lock_limit && !capable(CAP_IPC_LOCK))
			return -EAGAIN;
	}

	/*
	 * mm->mmap_sem is required to protect against another thread
	 * changing the mappings in case we sleep.
	 */
	verify_mm_writelocked(mm);

	/*
	 * Clear old maps.  this also does some error checking for us
	 */
 munmap_back:
	vma = find_vma_prepare(mm, addr, &prev, &rb_link, &rb_parent);
	if (vma && vma->vm_start < addr + len) {
		if (do_munmap(mm, addr, len))
			return -ENOMEM;
		goto munmap_back;
	}

	/* Check against address space limits *after* clearing old maps... */
	if (!may_expand_vm(mm, len >> PAGE_SHIFT))
=======
	addr = untagged_addr(addr);
	return __vm_munmap(addr, len, true);
}


/*
 * Emulation of deprecated remap_file_pages() syscall.
 */
SYSCALL_DEFINE5(remap_file_pages, unsigned long, start, unsigned long, size,
		unsigned long, prot, unsigned long, pgoff, unsigned long, flags)
{

	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	unsigned long populate = 0;
	unsigned long ret = -EINVAL;
	struct file *file;

	pr_warn_once("%s (%d) uses deprecated remap_file_pages() syscall. See Documentation/mm/remap_file_pages.rst.\n",
		     current->comm, current->pid);

	if (prot)
		return ret;
	start = start & PAGE_MASK;
	size = size & PAGE_MASK;

	if (start + size <= start)
		return ret;

	/* Does pgoff wrap? */
	if (pgoff + (size >> PAGE_SHIFT) < pgoff)
		return ret;

	if (mmap_write_lock_killable(mm))
		return -EINTR;

	vma = vma_lookup(mm, start);

	if (!vma || !(vma->vm_flags & VM_SHARED))
		goto out;

	if (start + size > vma->vm_end) {
		VMA_ITERATOR(vmi, mm, vma->vm_end);
		struct vm_area_struct *next, *prev = vma;

		for_each_vma_range(vmi, next, start + size) {
			/* hole between vmas ? */
			if (next->vm_start != prev->vm_end)
				goto out;

			if (next->vm_file != vma->vm_file)
				goto out;

			if (next->vm_flags != vma->vm_flags)
				goto out;

			if (start + size <= next->vm_end)
				break;

			prev = next;
		}

		if (!next)
			goto out;
	}

	prot |= vma->vm_flags & VM_READ ? PROT_READ : 0;
	prot |= vma->vm_flags & VM_WRITE ? PROT_WRITE : 0;
	prot |= vma->vm_flags & VM_EXEC ? PROT_EXEC : 0;

	flags &= MAP_NONBLOCK;
	flags |= MAP_SHARED | MAP_FIXED | MAP_POPULATE;
	if (vma->vm_flags & VM_LOCKED)
		flags |= MAP_LOCKED;

	file = get_file(vma->vm_file);
	ret = do_mmap(vma->vm_file, start, size,
			prot, flags, 0, pgoff, &populate, NULL);
	fput(file);
out:
	mmap_write_unlock(mm);
	if (populate)
		mm_populate(ret, populate);
	if (!IS_ERR_VALUE(ret))
		ret = 0;
	return ret;
}

/*
 * do_vma_munmap() - Unmap a full or partial vma.
 * @vmi: The vma iterator pointing at the vma
 * @vma: The first vma to be munmapped
 * @start: the start of the address to unmap
 * @end: The end of the address to unmap
 * @uf: The userfaultfd list_head
 * @unlock: Drop the lock on success
 *
 * unmaps a VMA mapping when the vma iterator is already in position.
 * Does not handle alignment.
 *
 * Return: 0 on success drops the lock of so directed, error on failure and will
 * still hold the lock.
 */
int do_vma_munmap(struct vma_iterator *vmi, struct vm_area_struct *vma,
		unsigned long start, unsigned long end, struct list_head *uf,
		bool unlock)
{
	struct mm_struct *mm = vma->vm_mm;

	arch_unmap(mm, start, end);
	return do_vmi_align_munmap(vmi, vma, mm, start, end, uf, unlock);
}

/*
 * do_brk_flags() - Increase the brk vma if the flags match.
 * @vmi: The vma iterator
 * @addr: The start address
 * @len: The length of the increase
 * @vma: The vma,
 * @flags: The VMA Flags
 *
 * Extend the brk VMA from addr to addr + len.  If the VMA is NULL or the flags
 * do not match then create a new anonymous VMA.  Eventually we may be able to
 * do some brk-specific accounting here.
 */
static int do_brk_flags(struct vma_iterator *vmi, struct vm_area_struct *vma,
		unsigned long addr, unsigned long len, unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	struct vma_prepare vp;

	/*
	 * Check against address space limits by the changed size
	 * Note: This happens *after* clearing old mappings in some code paths.
	 */
	flags |= VM_DATA_DEFAULT_FLAGS | VM_ACCOUNT | mm->def_flags;
	if (!may_expand_vm(mm, flags, len >> PAGE_SHIFT))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	if (mm->map_count > sysctl_max_map_count)
		return -ENOMEM;

	if (security_vm_enough_memory_mm(mm, len >> PAGE_SHIFT))
		return -ENOMEM;

<<<<<<< HEAD
	/* Can we just expand an old private anonymous mapping? */
	vma = vma_merge(mm, prev, addr, addr + len, flags,
					NULL, NULL, pgoff, NULL, NULL);
	if (vma)
		goto out;

	/*
	 * create a vma struct for an anonymous mapping
	 */
	vma = kmem_cache_zalloc(vm_area_cachep, GFP_KERNEL);
	if (!vma) {
		vm_unacct_memory(len >> PAGE_SHIFT);
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&vma->anon_vma_chain);
	vma->vm_mm = mm;
	vma->vm_start = addr;
	vma->vm_end = addr + len;
	vma->vm_pgoff = pgoff;
	vma->vm_flags = flags;
	vma->vm_page_prot = vm_get_page_prot(flags);
	vma_link(mm, vma, prev, rb_link, rb_parent);
out:
	perf_event_mmap(vma);
	mm->total_vm += len >> PAGE_SHIFT;
	if (flags & VM_LOCKED) {
		if (!mlock_vma_pages_range(vma, addr, addr + len))
			mm->locked_vm += (len >> PAGE_SHIFT);
	}
	return addr;
}

unsigned long vm_brk(unsigned long addr, unsigned long len)
{
	struct mm_struct *mm = current->mm;
	unsigned long ret;

	down_write(&mm->mmap_sem);
	ret = do_brk(addr, len);
	up_write(&mm->mmap_sem);
	return ret;
}
EXPORT_SYMBOL(vm_brk);
=======
	/*
	 * Expand the existing vma if possible; Note that singular lists do not
	 * occur after forking, so the expand will only happen on new VMAs.
	 */
	if (vma && vma->vm_end == addr && !vma_policy(vma) &&
	    can_vma_merge_after(vma, flags, NULL, NULL,
				addr >> PAGE_SHIFT, NULL_VM_UFFD_CTX, NULL)) {
		vma_iter_config(vmi, vma->vm_start, addr + len);
		if (vma_iter_prealloc(vmi, vma))
			goto unacct_fail;

		vma_start_write(vma);

		init_vma_prep(&vp, vma);
		vma_prepare(&vp);
		vma_adjust_trans_huge(vma, vma->vm_start, addr + len, 0);
		vma->vm_end = addr + len;
		vm_flags_set(vma, VM_SOFTDIRTY);
		vma_iter_store(vmi, vma);

		vma_complete(&vp, vmi, mm);
		khugepaged_enter_vma(vma, flags);
		goto out;
	}

	if (vma)
		vma_iter_next_range(vmi);
	/* create a vma struct for an anonymous mapping */
	vma = vm_area_alloc(mm);
	if (!vma)
		goto unacct_fail;

	vma_set_anonymous(vma);
	vma_set_range(vma, addr, addr + len, addr >> PAGE_SHIFT);
	vm_flags_init(vma, flags);
	vma->vm_page_prot = vm_get_page_prot(flags);
	vma_start_write(vma);
	if (vma_iter_store_gfp(vmi, vma, GFP_KERNEL))
		goto mas_store_fail;

	mm->map_count++;
	validate_mm(mm);
	ksm_add_vma(vma);
out:
	perf_event_mmap(vma);
	mm->total_vm += len >> PAGE_SHIFT;
	mm->data_vm += len >> PAGE_SHIFT;
	if (flags & VM_LOCKED)
		mm->locked_vm += (len >> PAGE_SHIFT);
	vm_flags_set(vma, VM_SOFTDIRTY);
	return 0;

mas_store_fail:
	vm_area_free(vma);
unacct_fail:
	vm_unacct_memory(len >> PAGE_SHIFT);
	return -ENOMEM;
}

int vm_brk_flags(unsigned long addr, unsigned long request, unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma = NULL;
	unsigned long len;
	int ret;
	bool populate;
	LIST_HEAD(uf);
	VMA_ITERATOR(vmi, mm, addr);

	len = PAGE_ALIGN(request);
	if (len < request)
		return -ENOMEM;
	if (!len)
		return 0;

	/* Until we need other flags, refuse anything except VM_EXEC. */
	if ((flags & (~VM_EXEC)) != 0)
		return -EINVAL;

	if (mmap_write_lock_killable(mm))
		return -EINTR;

	ret = check_brk_limits(addr, len);
	if (ret)
		goto limits_failed;

	ret = do_vmi_munmap(&vmi, mm, addr, len, &uf, 0);
	if (ret)
		goto munmap_failed;

	vma = vma_prev(&vmi);
	ret = do_brk_flags(&vmi, vma, addr, len, flags);
	populate = ((mm->def_flags & VM_LOCKED) != 0);
	mmap_write_unlock(mm);
	userfaultfd_unmap_complete(mm, &uf);
	if (populate && !ret)
		mm_populate(addr, len);
	return ret;

munmap_failed:
limits_failed:
	mmap_write_unlock(mm);
	return ret;
}
EXPORT_SYMBOL(vm_brk_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Release all mmaps. */
void exit_mmap(struct mm_struct *mm)
{
	struct mmu_gather tlb;
	struct vm_area_struct *vma;
	unsigned long nr_accounted = 0;
<<<<<<< HEAD
=======
	MA_STATE(mas, &mm->mm_mt, 0, 0);
	int count = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* mm's last user has gone, and its about to be pulled down */
	mmu_notifier_release(mm);

<<<<<<< HEAD
	if (mm->locked_vm) {
		vma = mm->mmap;
		while (vma) {
			if (vma->vm_flags & VM_LOCKED)
				munlock_vma_pages_all(vma);
			vma = vma->vm_next;
		}
	}

	arch_exit_mmap(mm);

	vma = mm->mmap;
	if (!vma)	/* Can happen if dup_mmap() received an OOM */
		return;

	lru_add_drain();
	flush_cache_mm(mm);
	tlb_gather_mmu(&tlb, mm, 1);
	/* update_hiwater_rss(mm) here? but nobody should be looking */
	/* Use -1 here to ensure all VMAs in the mm are unmapped */
	unmap_vmas(&tlb, vma, 0, -1, &nr_accounted, NULL);
	vm_unacct_memory(nr_accounted);

	free_pgtables(&tlb, vma, FIRST_USER_ADDRESS, USER_PGTABLES_CEILING);
	tlb_finish_mmu(&tlb, 0, -1);

	/*
	 * Walk the list again, actually closing and freeing it,
	 * with preemption enabled, without holding any MM locks.
	 */
	while (vma)
		vma = remove_vma(vma);

	BUG_ON(mm->nr_ptes > (FIRST_USER_ADDRESS+PMD_SIZE-1)>>PMD_SHIFT);
=======
	mmap_read_lock(mm);
	arch_exit_mmap(mm);

	vma = mas_find(&mas, ULONG_MAX);
	if (!vma || unlikely(xa_is_zero(vma))) {
		/* Can happen if dup_mmap() received an OOM */
		mmap_read_unlock(mm);
		mmap_write_lock(mm);
		goto destroy;
	}

	lru_add_drain();
	flush_cache_mm(mm);
	tlb_gather_mmu_fullmm(&tlb, mm);
	/* update_hiwater_rss(mm) here? but nobody should be looking */
	/* Use ULONG_MAX here to ensure all VMAs in the mm are unmapped */
	unmap_vmas(&tlb, &mas, vma, 0, ULONG_MAX, ULONG_MAX, false);
	mmap_read_unlock(mm);

	/*
	 * Set MMF_OOM_SKIP to hide this task from the oom killer/reaper
	 * because the memory has been already freed.
	 */
	set_bit(MMF_OOM_SKIP, &mm->flags);
	mmap_write_lock(mm);
	mt_clear_in_rcu(&mm->mm_mt);
	mas_set(&mas, vma->vm_end);
	free_pgtables(&tlb, &mas, vma, FIRST_USER_ADDRESS,
		      USER_PGTABLES_CEILING, true);
	tlb_finish_mmu(&tlb);

	/*
	 * Walk the list again, actually closing and freeing it, with preemption
	 * enabled, without holding any MM locks besides the unreachable
	 * mmap_write_lock.
	 */
	mas_set(&mas, vma->vm_end);
	do {
		if (vma->vm_flags & VM_ACCOUNT)
			nr_accounted += vma_pages(vma);
		remove_vma(vma, true);
		count++;
		cond_resched();
		vma = mas_find(&mas, ULONG_MAX);
	} while (vma && likely(!xa_is_zero(vma)));

	BUG_ON(count != mm->map_count);

	trace_exit_mmap(mm);
destroy:
	__mt_destroy(&mm->mm_mt);
	mmap_write_unlock(mm);
	vm_unacct_memory(nr_accounted);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Insert vm structure into process list sorted by address
 * and into the inode's i_mmap tree.  If vm_file is non-NULL
<<<<<<< HEAD
 * then i_mmap_mutex is taken here.
 */
int insert_vm_struct(struct mm_struct * mm, struct vm_area_struct * vma)
{
	struct vm_area_struct * __vma, * prev;
	struct rb_node ** rb_link, * rb_parent;
=======
 * then i_mmap_rwsem is taken here.
 */
int insert_vm_struct(struct mm_struct *mm, struct vm_area_struct *vma)
{
	unsigned long charged = vma_pages(vma);


	if (find_vma_intersection(mm, vma->vm_start, vma->vm_end))
		return -ENOMEM;

	if ((vma->vm_flags & VM_ACCOUNT) &&
	     security_vm_enough_memory_mm(mm, charged))
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The vm_pgoff of a purely anonymous vma should be irrelevant
	 * until its first write fault, when page's anon_vma and index
	 * are set.  But now set the vm_pgoff it will almost certainly
	 * end up with (unless mremap moves it elsewhere before that
	 * first wfault), so /proc/pid/maps tells a consistent story.
	 *
	 * By setting it to reflect the virtual start address of the
	 * vma, merges and splits can happen in a seamless way, just
	 * using the existing file pgoff checks and manipulations.
<<<<<<< HEAD
	 * Similarly in do_mmap_pgoff and in do_brk.
	 */
	if (!vma->vm_file) {
		BUG_ON(vma->anon_vma);
		vma->vm_pgoff = vma->vm_start >> PAGE_SHIFT;
	}
	__vma = find_vma_prepare(mm,vma->vm_start,&prev,&rb_link,&rb_parent);
	if (__vma && __vma->vm_start < vma->vm_end)
		return -ENOMEM;
	if ((vma->vm_flags & VM_ACCOUNT) &&
	     security_vm_enough_memory_mm(mm, vma_pages(vma)))
		return -ENOMEM;
	vma_link(mm, vma, prev, rb_link, rb_parent);
=======
	 * Similarly in do_mmap and in do_brk_flags.
	 */
	if (vma_is_anonymous(vma)) {
		BUG_ON(vma->anon_vma);
		vma->vm_pgoff = vma->vm_start >> PAGE_SHIFT;
	}

	if (vma_link(mm, vma)) {
		if (vma->vm_flags & VM_ACCOUNT)
			vm_unacct_memory(charged);
		return -ENOMEM;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Copy the vma structure to a new location in the same mm,
 * prior to moving page table entries, to effect an mremap move.
 */
struct vm_area_struct *copy_vma(struct vm_area_struct **vmap,
<<<<<<< HEAD
	unsigned long addr, unsigned long len, pgoff_t pgoff)
=======
	unsigned long addr, unsigned long len, pgoff_t pgoff,
	bool *need_rmap_locks)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vm_area_struct *vma = *vmap;
	unsigned long vma_start = vma->vm_start;
	struct mm_struct *mm = vma->vm_mm;
	struct vm_area_struct *new_vma, *prev;
<<<<<<< HEAD
	struct rb_node **rb_link, *rb_parent;
	struct mempolicy *pol;
	bool faulted_in_anon_vma = true;
=======
	bool faulted_in_anon_vma = true;
	VMA_ITERATOR(vmi, mm, addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If anonymous vma has not yet been faulted, update new pgoff
	 * to match new location, to increase its chance of merging.
	 */
<<<<<<< HEAD
	if (unlikely(!vma->vm_file && !vma->anon_vma)) {
=======
	if (unlikely(vma_is_anonymous(vma) && !vma->anon_vma)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pgoff = addr >> PAGE_SHIFT;
		faulted_in_anon_vma = false;
	}

<<<<<<< HEAD
	find_vma_prepare(mm, addr, &prev, &rb_link, &rb_parent);
	new_vma = vma_merge(mm, prev, addr, addr + len, vma->vm_flags,
			vma->anon_vma, vma->vm_file, pgoff, vma_policy(vma),
			vma_get_anon_name(vma));
=======
	new_vma = find_vma_prev(mm, addr, &prev);
	if (new_vma && new_vma->vm_start < addr + len)
		return NULL;	/* should never get here */

	new_vma = vma_merge_new_vma(&vmi, prev, vma, addr, addr + len, pgoff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (new_vma) {
		/*
		 * Source vma may have been merged into new_vma
		 */
		if (unlikely(vma_start >= new_vma->vm_start &&
			     vma_start < new_vma->vm_end)) {
			/*
			 * The only way we can get a vma_merge with
			 * self during an mremap is if the vma hasn't
			 * been faulted in yet and we were allowed to
			 * reset the dst vma->vm_pgoff to the
			 * destination address of the mremap to allow
			 * the merge to happen. mremap must change the
			 * vm_pgoff linearity between src and dst vmas
			 * (in turn preventing a vma_merge) to be
			 * safe. It is only safe to keep the vm_pgoff
			 * linear if there are no pages mapped yet.
			 */
<<<<<<< HEAD
			VM_BUG_ON(faulted_in_anon_vma);
			*vmap = new_vma;
		} else
			anon_vma_moveto_tail(new_vma);
	} else {
		new_vma = kmem_cache_alloc(vm_area_cachep, GFP_KERNEL);
		if (new_vma) {
			*new_vma = *vma;
			pol = mpol_dup(vma_policy(vma));
			if (IS_ERR(pol))
				goto out_free_vma;
			INIT_LIST_HEAD(&new_vma->anon_vma_chain);
			if (anon_vma_clone(new_vma, vma))
				goto out_free_mempol;
			vma_set_policy(new_vma, pol);
			new_vma->vm_start = addr;
			new_vma->vm_end = addr + len;
			new_vma->vm_pgoff = pgoff;
			if (new_vma->vm_file) {
				get_file(new_vma->vm_file);
				if (vma->vm_flags & VM_EXECUTABLE)
					added_exe_file_vma(mm);
			}
			if (new_vma->vm_ops && new_vma->vm_ops->open)
				new_vma->vm_ops->open(new_vma);
			vma_link(mm, new_vma, prev, rb_link, rb_parent);
		}
	}
	return new_vma;

 out_free_mempol:
	mpol_put(pol);
 out_free_vma:
	kmem_cache_free(vm_area_cachep, new_vma);
=======
			VM_BUG_ON_VMA(faulted_in_anon_vma, new_vma);
			*vmap = vma = new_vma;
		}
		*need_rmap_locks = (new_vma->vm_pgoff <= vma->vm_pgoff);
	} else {
		new_vma = vm_area_dup(vma);
		if (!new_vma)
			goto out;
		vma_set_range(new_vma, addr, addr + len, pgoff);
		if (vma_dup_policy(vma, new_vma))
			goto out_free_vma;
		if (anon_vma_clone(new_vma, vma))
			goto out_free_mempol;
		if (new_vma->vm_file)
			get_file(new_vma->vm_file);
		if (new_vma->vm_ops && new_vma->vm_ops->open)
			new_vma->vm_ops->open(new_vma);
		if (vma_link(mm, new_vma))
			goto out_vma_link;
		*need_rmap_locks = false;
	}
	return new_vma;

out_vma_link:
	if (new_vma->vm_ops && new_vma->vm_ops->close)
		new_vma->vm_ops->close(new_vma);

	if (new_vma->vm_file)
		fput(new_vma->vm_file);

	unlink_anon_vmas(new_vma);
out_free_mempol:
	mpol_put(vma_policy(new_vma));
out_free_vma:
	vm_area_free(new_vma);
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}

/*
 * Return true if the calling process may expand its vm space by the passed
 * number of pages
 */
<<<<<<< HEAD
int may_expand_vm(struct mm_struct *mm, unsigned long npages)
{
	unsigned long cur = mm->total_vm;	/* pages */
	unsigned long lim;

	lim = rlimit(RLIMIT_AS) >> PAGE_SHIFT;

	if (cur + npages > lim)
		return 0;
	return 1;
}


static int special_mapping_fault(struct vm_area_struct *vma,
				struct vm_fault *vmf)
{
	pgoff_t pgoff;
	struct page **pages;

	/*
	 * special mappings have no vm_file, and in that case, the mm
	 * uses vm_pgoff internally. So we have to subtract it from here.
	 * We are allowed to do this because we are the mm; do not copy
	 * this code into drivers!
	 */
	pgoff = vmf->pgoff - vma->vm_pgoff;

	for (pages = vma->vm_private_data; pgoff && *pages; ++pages)
=======
bool may_expand_vm(struct mm_struct *mm, vm_flags_t flags, unsigned long npages)
{
	if (mm->total_vm + npages > rlimit(RLIMIT_AS) >> PAGE_SHIFT)
		return false;

	if (is_data_mapping(flags) &&
	    mm->data_vm + npages > rlimit(RLIMIT_DATA) >> PAGE_SHIFT) {
		/* Workaround for Valgrind */
		if (rlimit(RLIMIT_DATA) == 0 &&
		    mm->data_vm + npages <= rlimit_max(RLIMIT_DATA) >> PAGE_SHIFT)
			return true;

		pr_warn_once("%s (%d): VmData %lu exceed data ulimit %lu. Update limits%s.\n",
			     current->comm, current->pid,
			     (mm->data_vm + npages) << PAGE_SHIFT,
			     rlimit(RLIMIT_DATA),
			     ignore_rlimit_data ? "" : " or use boot option ignore_rlimit_data");

		if (!ignore_rlimit_data)
			return false;
	}

	return true;
}

void vm_stat_account(struct mm_struct *mm, vm_flags_t flags, long npages)
{
	WRITE_ONCE(mm->total_vm, READ_ONCE(mm->total_vm)+npages);

	if (is_exec_mapping(flags))
		mm->exec_vm += npages;
	else if (is_stack_mapping(flags))
		mm->stack_vm += npages;
	else if (is_data_mapping(flags))
		mm->data_vm += npages;
}

static vm_fault_t special_mapping_fault(struct vm_fault *vmf);

/*
 * Having a close hook prevents vma merging regardless of flags.
 */
static void special_mapping_close(struct vm_area_struct *vma)
{
}

static const char *special_mapping_name(struct vm_area_struct *vma)
{
	return ((struct vm_special_mapping *)vma->vm_private_data)->name;
}

static int special_mapping_mremap(struct vm_area_struct *new_vma)
{
	struct vm_special_mapping *sm = new_vma->vm_private_data;

	if (WARN_ON_ONCE(current->mm != new_vma->vm_mm))
		return -EFAULT;

	if (sm->mremap)
		return sm->mremap(sm, new_vma);

	return 0;
}

static int special_mapping_split(struct vm_area_struct *vma, unsigned long addr)
{
	/*
	 * Forbid splitting special mappings - kernel has expectations over
	 * the number of pages in mapping. Together with VM_DONTEXPAND
	 * the size of vma should stay the same over the special mapping's
	 * lifetime.
	 */
	return -EINVAL;
}

static const struct vm_operations_struct special_mapping_vmops = {
	.close = special_mapping_close,
	.fault = special_mapping_fault,
	.mremap = special_mapping_mremap,
	.name = special_mapping_name,
	/* vDSO code relies that VVAR can't be accessed remotely */
	.access = NULL,
	.may_split = special_mapping_split,
};

static const struct vm_operations_struct legacy_special_mapping_vmops = {
	.close = special_mapping_close,
	.fault = special_mapping_fault,
};

static vm_fault_t special_mapping_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	pgoff_t pgoff;
	struct page **pages;

	if (vma->vm_ops == &legacy_special_mapping_vmops) {
		pages = vma->vm_private_data;
	} else {
		struct vm_special_mapping *sm = vma->vm_private_data;

		if (sm->fault)
			return sm->fault(sm, vmf->vma, vmf);

		pages = sm->pages;
	}

	for (pgoff = vmf->pgoff; pgoff && *pages; ++pages)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pgoff--;

	if (*pages) {
		struct page *page = *pages;
		get_page(page);
		vmf->page = page;
		return 0;
	}

	return VM_FAULT_SIGBUS;
}

<<<<<<< HEAD
/*
 * Having a close hook prevents vma merging regardless of flags.
 */
static void special_mapping_close(struct vm_area_struct *vma)
{
}

static const struct vm_operations_struct special_mapping_vmops = {
	.close = special_mapping_close,
	.fault = special_mapping_fault,
};

/*
 * Called with mm->mmap_sem held for writing.
=======
static struct vm_area_struct *__install_special_mapping(
	struct mm_struct *mm,
	unsigned long addr, unsigned long len,
	unsigned long vm_flags, void *priv,
	const struct vm_operations_struct *ops)
{
	int ret;
	struct vm_area_struct *vma;

	vma = vm_area_alloc(mm);
	if (unlikely(vma == NULL))
		return ERR_PTR(-ENOMEM);

	vma_set_range(vma, addr, addr + len, 0);
	vm_flags_init(vma, (vm_flags | mm->def_flags |
		      VM_DONTEXPAND | VM_SOFTDIRTY) & ~VM_LOCKED_MASK);
	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);

	vma->vm_ops = ops;
	vma->vm_private_data = priv;

	ret = insert_vm_struct(mm, vma);
	if (ret)
		goto out;

	vm_stat_account(mm, vma->vm_flags, len >> PAGE_SHIFT);

	perf_event_mmap(vma);

	return vma;

out:
	vm_area_free(vma);
	return ERR_PTR(ret);
}

bool vma_is_special_mapping(const struct vm_area_struct *vma,
	const struct vm_special_mapping *sm)
{
	return vma->vm_private_data == sm &&
		(vma->vm_ops == &special_mapping_vmops ||
		 vma->vm_ops == &legacy_special_mapping_vmops);
}

/*
 * Called with mm->mmap_lock held for writing.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Insert a new vma covering the given region, with the given flags.
 * Its pages are supplied by the given array of struct page *.
 * The array can be shorter than len >> PAGE_SHIFT if it's null-terminated.
 * The region past the last page supplied will always produce SIGBUS.
 * The array pointer and the pages it points to are assumed to stay alive
 * for as long as this mapping might exist.
 */
<<<<<<< HEAD
=======
struct vm_area_struct *_install_special_mapping(
	struct mm_struct *mm,
	unsigned long addr, unsigned long len,
	unsigned long vm_flags, const struct vm_special_mapping *spec)
{
	return __install_special_mapping(mm, addr, len, vm_flags, (void *)spec,
					&special_mapping_vmops);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int install_special_mapping(struct mm_struct *mm,
			    unsigned long addr, unsigned long len,
			    unsigned long vm_flags, struct page **pages)
{
<<<<<<< HEAD
	int ret;
	struct vm_area_struct *vma;

	vma = kmem_cache_zalloc(vm_area_cachep, GFP_KERNEL);
	if (unlikely(vma == NULL))
		return -ENOMEM;

	INIT_LIST_HEAD(&vma->anon_vma_chain);
	vma->vm_mm = mm;
	vma->vm_start = addr;
	vma->vm_end = addr + len;

	vma->vm_flags = vm_flags | mm->def_flags | VM_DONTEXPAND;
	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);

	vma->vm_ops = &special_mapping_vmops;
	vma->vm_private_data = pages;

	ret = security_file_mmap(NULL, 0, 0, 0, vma->vm_start, 1);
	if (ret)
		goto out;

	ret = insert_vm_struct(mm, vma);
	if (ret)
		goto out;

	mm->total_vm += len >> PAGE_SHIFT;

	perf_event_mmap(vma);

	return 0;

out:
	kmem_cache_free(vm_area_cachep, vma);
	return ret;
=======
	struct vm_area_struct *vma = __install_special_mapping(
		mm, addr, len, vm_flags, (void *)pages,
		&legacy_special_mapping_vmops);

	return PTR_ERR_OR_ZERO(vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEFINE_MUTEX(mm_all_locks_mutex);

static void vm_lock_anon_vma(struct mm_struct *mm, struct anon_vma *anon_vma)
{
<<<<<<< HEAD
	if (!test_bit(0, (unsigned long *) &anon_vma->root->head.next)) {
=======
	if (!test_bit(0, (unsigned long *) &anon_vma->root->rb_root.rb_root.rb_node)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * The LSB of head.next can't change from under us
		 * because we hold the mm_all_locks_mutex.
		 */
<<<<<<< HEAD
		mutex_lock_nest_lock(&anon_vma->root->mutex, &mm->mmap_sem);
		/*
		 * We can safely modify head.next after taking the
		 * anon_vma->root->mutex. If some other vma in this mm shares
=======
		down_write_nest_lock(&anon_vma->root->rwsem, &mm->mmap_lock);
		/*
		 * We can safely modify head.next after taking the
		 * anon_vma->root->rwsem. If some other vma in this mm shares
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * the same anon_vma we won't take it again.
		 *
		 * No need of atomic instructions here, head.next
		 * can't change from under us thanks to the
<<<<<<< HEAD
		 * anon_vma->root->mutex.
		 */
		if (__test_and_set_bit(0, (unsigned long *)
				       &anon_vma->root->head.next))
=======
		 * anon_vma->root->rwsem.
		 */
		if (__test_and_set_bit(0, (unsigned long *)
				       &anon_vma->root->rb_root.rb_root.rb_node))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			BUG();
	}
}

static void vm_lock_mapping(struct mm_struct *mm, struct address_space *mapping)
{
	if (!test_bit(AS_MM_ALL_LOCKS, &mapping->flags)) {
		/*
		 * AS_MM_ALL_LOCKS can't change from under us because
		 * we hold the mm_all_locks_mutex.
		 *
		 * Operations on ->flags have to be atomic because
		 * even if AS_MM_ALL_LOCKS is stable thanks to the
		 * mm_all_locks_mutex, there may be other cpus
		 * changing other bitflags in parallel to us.
		 */
		if (test_and_set_bit(AS_MM_ALL_LOCKS, &mapping->flags))
			BUG();
<<<<<<< HEAD
		mutex_lock_nest_lock(&mapping->i_mmap_mutex, &mm->mmap_sem);
=======
		down_write_nest_lock(&mapping->i_mmap_rwsem, &mm->mmap_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * This operation locks against the VM for all pte/vma/mm related
 * operations that could ever happen on a certain mm. This includes
 * vmtruncate, try_to_unmap, and all page faults.
 *
<<<<<<< HEAD
 * The caller must take the mmap_sem in write mode before calling
 * mm_take_all_locks(). The caller isn't allowed to release the
 * mmap_sem until mm_drop_all_locks() returns.
 *
 * mmap_sem in write mode is required in order to block all operations
 * that could modify pagetables and free pages without need of
 * altering the vma layout (for example populate_range() with
 * nonlinear vmas). It's also needed in write mode to avoid new
=======
 * The caller must take the mmap_lock in write mode before calling
 * mm_take_all_locks(). The caller isn't allowed to release the
 * mmap_lock until mm_drop_all_locks() returns.
 *
 * mmap_lock in write mode is required in order to block all operations
 * that could modify pagetables and free pages without need of
 * altering the vma layout. It's also needed in write mode to avoid new
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * anon_vmas to be associated with existing vmas.
 *
 * A single task can't take more than one mm_take_all_locks() in a row
 * or it would deadlock.
 *
<<<<<<< HEAD
 * The LSB in anon_vma->head.next and the AS_MM_ALL_LOCKS bitflag in
 * mapping->flags avoid to take the same lock twice, if more than one
 * vma in this mm is backed by the same anon_vma or address_space.
 *
 * We can take all the locks in random order because the VM code
 * taking i_mmap_mutex or anon_vma->mutex outside the mmap_sem never
 * takes more than one of them in a row. Secondly we're protected
 * against a concurrent mm_take_all_locks() by the mm_all_locks_mutex.
=======
 * The LSB in anon_vma->rb_root.rb_node and the AS_MM_ALL_LOCKS bitflag in
 * mapping->flags avoid to take the same lock twice, if more than one
 * vma in this mm is backed by the same anon_vma or address_space.
 *
 * We take locks in following order, accordingly to comment at beginning
 * of mm/rmap.c:
 *   - all hugetlbfs_i_mmap_rwsem_key locks (aka mapping->i_mmap_rwsem for
 *     hugetlb mapping);
 *   - all vmas marked locked
 *   - all i_mmap_rwsem locks;
 *   - all anon_vma->rwseml
 *
 * We can take all locks within these types randomly because the VM code
 * doesn't nest them and we protected from parallel mm_take_all_locks() by
 * mm_all_locks_mutex.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * mm_take_all_locks() and mm_drop_all_locks are expensive operations
 * that may have to take thousand of locks.
 *
 * mm_take_all_locks() can fail if it's interrupted by signals.
 */
int mm_take_all_locks(struct mm_struct *mm)
{
	struct vm_area_struct *vma;
	struct anon_vma_chain *avc;
<<<<<<< HEAD

	BUG_ON(down_read_trylock(&mm->mmap_sem));

	mutex_lock(&mm_all_locks_mutex);

	for (vma = mm->mmap; vma; vma = vma->vm_next) {
		if (signal_pending(current))
			goto out_unlock;
		if (vma->vm_file && vma->vm_file->f_mapping)
			vm_lock_mapping(mm, vma->vm_file->f_mapping);
	}

	for (vma = mm->mmap; vma; vma = vma->vm_next) {
=======
	MA_STATE(mas, &mm->mm_mt, 0, 0);

	mmap_assert_write_locked(mm);

	mutex_lock(&mm_all_locks_mutex);

	/*
	 * vma_start_write() does not have a complement in mm_drop_all_locks()
	 * because vma_start_write() is always asymmetrical; it marks a VMA as
	 * being written to until mmap_write_unlock() or mmap_write_downgrade()
	 * is reached.
	 */
	mas_for_each(&mas, vma, ULONG_MAX) {
		if (signal_pending(current))
			goto out_unlock;
		vma_start_write(vma);
	}

	mas_set(&mas, 0);
	mas_for_each(&mas, vma, ULONG_MAX) {
		if (signal_pending(current))
			goto out_unlock;
		if (vma->vm_file && vma->vm_file->f_mapping &&
				is_vm_hugetlb_page(vma))
			vm_lock_mapping(mm, vma->vm_file->f_mapping);
	}

	mas_set(&mas, 0);
	mas_for_each(&mas, vma, ULONG_MAX) {
		if (signal_pending(current))
			goto out_unlock;
		if (vma->vm_file && vma->vm_file->f_mapping &&
				!is_vm_hugetlb_page(vma))
			vm_lock_mapping(mm, vma->vm_file->f_mapping);
	}

	mas_set(&mas, 0);
	mas_for_each(&mas, vma, ULONG_MAX) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (signal_pending(current))
			goto out_unlock;
		if (vma->anon_vma)
			list_for_each_entry(avc, &vma->anon_vma_chain, same_vma)
				vm_lock_anon_vma(mm, avc->anon_vma);
	}

	return 0;

out_unlock:
	mm_drop_all_locks(mm);
	return -EINTR;
}

static void vm_unlock_anon_vma(struct anon_vma *anon_vma)
{
<<<<<<< HEAD
	if (test_bit(0, (unsigned long *) &anon_vma->root->head.next)) {
=======
	if (test_bit(0, (unsigned long *) &anon_vma->root->rb_root.rb_root.rb_node)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * The LSB of head.next can't change to 0 from under
		 * us because we hold the mm_all_locks_mutex.
		 *
		 * We must however clear the bitflag before unlocking
<<<<<<< HEAD
		 * the vma so the users using the anon_vma->head will
=======
		 * the vma so the users using the anon_vma->rb_root will
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * never see our bitflag.
		 *
		 * No need of atomic instructions here, head.next
		 * can't change from under us until we release the
<<<<<<< HEAD
		 * anon_vma->root->mutex.
		 */
		if (!__test_and_clear_bit(0, (unsigned long *)
					  &anon_vma->root->head.next))
			BUG();
		anon_vma_unlock(anon_vma);
=======
		 * anon_vma->root->rwsem.
		 */
		if (!__test_and_clear_bit(0, (unsigned long *)
					  &anon_vma->root->rb_root.rb_root.rb_node))
			BUG();
		anon_vma_unlock_write(anon_vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void vm_unlock_mapping(struct address_space *mapping)
{
	if (test_bit(AS_MM_ALL_LOCKS, &mapping->flags)) {
		/*
		 * AS_MM_ALL_LOCKS can't change to 0 from under us
		 * because we hold the mm_all_locks_mutex.
		 */
<<<<<<< HEAD
		mutex_unlock(&mapping->i_mmap_mutex);
=======
		i_mmap_unlock_write(mapping);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!test_and_clear_bit(AS_MM_ALL_LOCKS,
					&mapping->flags))
			BUG();
	}
}

/*
<<<<<<< HEAD
 * The mmap_sem cannot be released by the caller until
=======
 * The mmap_lock cannot be released by the caller until
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * mm_drop_all_locks() returns.
 */
void mm_drop_all_locks(struct mm_struct *mm)
{
	struct vm_area_struct *vma;
	struct anon_vma_chain *avc;
<<<<<<< HEAD

	BUG_ON(down_read_trylock(&mm->mmap_sem));
	BUG_ON(!mutex_is_locked(&mm_all_locks_mutex));

	for (vma = mm->mmap; vma; vma = vma->vm_next) {
=======
	MA_STATE(mas, &mm->mm_mt, 0, 0);

	mmap_assert_write_locked(mm);
	BUG_ON(!mutex_is_locked(&mm_all_locks_mutex));

	mas_for_each(&mas, vma, ULONG_MAX) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (vma->anon_vma)
			list_for_each_entry(avc, &vma->anon_vma_chain, same_vma)
				vm_unlock_anon_vma(avc->anon_vma);
		if (vma->vm_file && vma->vm_file->f_mapping)
			vm_unlock_mapping(vma->vm_file->f_mapping);
	}

	mutex_unlock(&mm_all_locks_mutex);
}

/*
<<<<<<< HEAD
 * initialise the VMA slab
=======
 * initialise the percpu counter for VM
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void __init mmap_init(void)
{
	int ret;

<<<<<<< HEAD
	ret = percpu_counter_init(&vm_committed_as, 0);
	VM_BUG_ON(ret);
}
=======
	ret = percpu_counter_init(&vm_committed_as, 0, GFP_KERNEL);
	VM_BUG_ON(ret);
}

/*
 * Initialise sysctl_user_reserve_kbytes.
 *
 * This is intended to prevent a user from starting a single memory hogging
 * process, such that they cannot recover (kill the hog) in OVERCOMMIT_NEVER
 * mode.
 *
 * The default value is min(3% of free memory, 128MB)
 * 128MB is enough to recover with sshd/login, bash, and top/kill.
 */
static int init_user_reserve(void)
{
	unsigned long free_kbytes;

	free_kbytes = K(global_zone_page_state(NR_FREE_PAGES));

	sysctl_user_reserve_kbytes = min(free_kbytes / 32, SZ_128K);
	return 0;
}
subsys_initcall(init_user_reserve);

/*
 * Initialise sysctl_admin_reserve_kbytes.
 *
 * The purpose of sysctl_admin_reserve_kbytes is to allow the sys admin
 * to log in and kill a memory hogging process.
 *
 * Systems with more than 256MB will reserve 8MB, enough to recover
 * with sshd, bash, and top in OVERCOMMIT_GUESS. Smaller systems will
 * only reserve 3% of free pages by default.
 */
static int init_admin_reserve(void)
{
	unsigned long free_kbytes;

	free_kbytes = K(global_zone_page_state(NR_FREE_PAGES));

	sysctl_admin_reserve_kbytes = min(free_kbytes / 32, SZ_8K);
	return 0;
}
subsys_initcall(init_admin_reserve);

/*
 * Reinititalise user and admin reserves if memory is added or removed.
 *
 * The default user reserve max is 128MB, and the default max for the
 * admin reserve is 8MB. These are usually, but not always, enough to
 * enable recovery from a memory hogging process using login/sshd, a shell,
 * and tools like top. It may make sense to increase or even disable the
 * reserve depending on the existence of swap or variations in the recovery
 * tools. So, the admin may have changed them.
 *
 * If memory is added and the reserves have been eliminated or increased above
 * the default max, then we'll trust the admin.
 *
 * If memory is removed and there isn't enough free memory, then we
 * need to reset the reserves.
 *
 * Otherwise keep the reserve set by the admin.
 */
static int reserve_mem_notifier(struct notifier_block *nb,
			     unsigned long action, void *data)
{
	unsigned long tmp, free_kbytes;

	switch (action) {
	case MEM_ONLINE:
		/* Default max is 128MB. Leave alone if modified by operator. */
		tmp = sysctl_user_reserve_kbytes;
		if (tmp > 0 && tmp < SZ_128K)
			init_user_reserve();

		/* Default max is 8MB.  Leave alone if modified by operator. */
		tmp = sysctl_admin_reserve_kbytes;
		if (tmp > 0 && tmp < SZ_8K)
			init_admin_reserve();

		break;
	case MEM_OFFLINE:
		free_kbytes = K(global_zone_page_state(NR_FREE_PAGES));

		if (sysctl_user_reserve_kbytes > free_kbytes) {
			init_user_reserve();
			pr_info("vm.user_reserve_kbytes reset to %lu\n",
				sysctl_user_reserve_kbytes);
		}

		if (sysctl_admin_reserve_kbytes > free_kbytes) {
			init_admin_reserve();
			pr_info("vm.admin_reserve_kbytes reset to %lu\n",
				sysctl_admin_reserve_kbytes);
		}
		break;
	default:
		break;
	}
	return NOTIFY_OK;
}

static int __meminit init_reserve_notifier(void)
{
	if (hotplug_memory_notifier(reserve_mem_notifier, DEFAULT_CALLBACK_PRI))
		pr_err("Failed registering memory add/remove notifier for admin reserve\n");

	return 0;
}
subsys_initcall(init_reserve_notifier);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
