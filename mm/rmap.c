/*
 * mm/rmap.c - physical to virtual reverse mappings
 *
 * Copyright 2001, Rik van Riel <riel@conectiva.com.br>
 * Released under the General Public License (GPL).
 *
 * Simple, low overhead reverse mapping scheme.
 * Please try to keep this thing as modular as possible.
 *
 * Provides methods for unmapping each kind of mapped page:
 * the anon methods track anonymous pages, and
 * the file methods track pages belonging to an inode.
 *
 * Original design by Rik van Riel <riel@conectiva.com.br> 2001
 * File methods by Dave McCracken <dmccr@us.ibm.com> 2003, 2004
 * Anonymous methods by Andrea Arcangeli <andrea@suse.de> 2004
 * Contributions by Hugh Dickins 2003, 2004
 */

/*
 * Lock ordering in mm:
 *
<<<<<<< HEAD
 * inode->i_mutex	(while writing or truncating, not reading or faulting)
 *   mm->mmap_sem
 *     page->flags PG_locked (lock_page)
 *       mapping->i_mmap_mutex
 *         anon_vma->mutex
 *           mm->page_table_lock or pte_lock
 *             zone->lru_lock (in mark_page_accessed, isolate_lru_page)
 *             swap_lock (in swap_duplicate, swap_info_get)
 *               mmlist_lock (in mmput, drain_mmlist and others)
 *               mapping->private_lock (in __set_page_dirty_buffers)
 *               inode->i_lock (in set_page_dirty's __mark_inode_dirty)
 *               bdi.wb->list_lock (in set_page_dirty's __mark_inode_dirty)
 *                 sb_lock (within inode_lock in fs/fs-writeback.c)
 *                 mapping->tree_lock (widely used, in set_page_dirty,
 *                           in arch-dependent flush_dcache_mmap_lock,
 *                           within bdi.wb->list_lock in __sync_single_inode)
 *
 * anon_vma->mutex,mapping->i_mutex      (memory_failure, collect_procs_anon)
 *   ->tasklist_lock
 *     pte map lock
 */

#include <linux/mm.h>
=======
 * inode->i_rwsem	(while writing or truncating, not reading or faulting)
 *   mm->mmap_lock
 *     mapping->invalidate_lock (in filemap_fault)
 *       page->flags PG_locked (lock_page)
 *         hugetlbfs_i_mmap_rwsem_key (in huge_pmd_share, see hugetlbfs below)
 *           vma_start_write
 *             mapping->i_mmap_rwsem
 *               anon_vma->rwsem
 *                 mm->page_table_lock or pte_lock
 *                   swap_lock (in swap_duplicate, swap_info_get)
 *                     mmlist_lock (in mmput, drain_mmlist and others)
 *                     mapping->private_lock (in block_dirty_folio)
 *                       folio_lock_memcg move_lock (in block_dirty_folio)
 *                         i_pages lock (widely used)
 *                           lruvec->lru_lock (in folio_lruvec_lock_irq)
 *                     inode->i_lock (in set_page_dirty's __mark_inode_dirty)
 *                     bdi.wb->list_lock (in set_page_dirty's __mark_inode_dirty)
 *                       sb_lock (within inode_lock in fs/fs-writeback.c)
 *                       i_pages lock (widely used, in set_page_dirty,
 *                                 in arch-dependent flush_dcache_mmap_lock,
 *                                 within bdi.wb->list_lock in __sync_single_inode)
 *
 * anon_vma->rwsem,mapping->i_mmap_rwsem   (memory_failure, collect_procs_anon)
 *   ->tasklist_lock
 *     pte map lock
 *
 * hugetlbfs PageHuge() take locks in this order:
 *   hugetlb_fault_mutex (hugetlbfs specific page fault mutex)
 *     vma_lock (hugetlb specific lock for pmd_sharing)
 *       mapping->i_mmap_rwsem (also used for hugetlb pmd sharing)
 *         page->flags PG_locked (lock_page)
 */

#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pagemap.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/ksm.h>
#include <linux/rmap.h>
#include <linux/rcupdate.h>
#include <linux/export.h>
#include <linux/memcontrol.h>
#include <linux/mmu_notifier.h>
#include <linux/migrate.h>
#include <linux/hugetlb.h>
<<<<<<< HEAD
#include <linux/backing-dev.h>

#include <asm/tlbflush.h>

=======
#include <linux/huge_mm.h>
#include <linux/backing-dev.h>
#include <linux/page_idle.h>
#include <linux/memremap.h>
#include <linux/userfaultfd_k.h>
#include <linux/mm_inline.h>

#include <asm/tlbflush.h>

#define CREATE_TRACE_POINTS
#include <trace/events/tlb.h>
#include <trace/events/migrate.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "internal.h"

static struct kmem_cache *anon_vma_cachep;
static struct kmem_cache *anon_vma_chain_cachep;

static inline struct anon_vma *anon_vma_alloc(void)
{
	struct anon_vma *anon_vma;

	anon_vma = kmem_cache_alloc(anon_vma_cachep, GFP_KERNEL);
	if (anon_vma) {
		atomic_set(&anon_vma->refcount, 1);
<<<<<<< HEAD
		anon_vma->degree = 1;	/* Reference for first vma */
=======
		anon_vma->num_children = 0;
		anon_vma->num_active_vmas = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		anon_vma->parent = anon_vma;
		/*
		 * Initialise the anon_vma root to point to itself. If called
		 * from fork, the root will be reset to the parents anon_vma.
		 */
		anon_vma->root = anon_vma;
	}

	return anon_vma;
}

static inline void anon_vma_free(struct anon_vma *anon_vma)
{
	VM_BUG_ON(atomic_read(&anon_vma->refcount));

	/*
<<<<<<< HEAD
	 * Synchronize against page_lock_anon_vma() such that
=======
	 * Synchronize against folio_lock_anon_vma_read() such that
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * we can safely hold the lock without the anon_vma getting
	 * freed.
	 *
	 * Relies on the full mb implied by the atomic_dec_and_test() from
	 * put_anon_vma() against the acquire barrier implied by
<<<<<<< HEAD
	 * mutex_trylock() from page_lock_anon_vma(). This orders:
	 *
	 * page_lock_anon_vma()		VS	put_anon_vma()
	 *   mutex_trylock()			  atomic_dec_and_test()
	 *   LOCK				  MB
	 *   atomic_read()			  mutex_is_locked()
=======
	 * down_read_trylock() from folio_lock_anon_vma_read(). This orders:
	 *
	 * folio_lock_anon_vma_read()	VS	put_anon_vma()
	 *   down_read_trylock()		  atomic_dec_and_test()
	 *   LOCK				  MB
	 *   atomic_read()			  rwsem_is_locked()
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *
	 * LOCK should suffice since the actual taking of the lock must
	 * happen _before_ what follows.
	 */
	might_sleep();
<<<<<<< HEAD
	if (mutex_is_locked(&anon_vma->root->mutex)) {
		anon_vma_lock(anon_vma);
		anon_vma_unlock(anon_vma);
=======
	if (rwsem_is_locked(&anon_vma->root->rwsem)) {
		anon_vma_lock_write(anon_vma);
		anon_vma_unlock_write(anon_vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	kmem_cache_free(anon_vma_cachep, anon_vma);
}

static inline struct anon_vma_chain *anon_vma_chain_alloc(gfp_t gfp)
{
	return kmem_cache_alloc(anon_vma_chain_cachep, gfp);
}

static void anon_vma_chain_free(struct anon_vma_chain *anon_vma_chain)
{
	kmem_cache_free(anon_vma_chain_cachep, anon_vma_chain);
}

static void anon_vma_chain_link(struct vm_area_struct *vma,
				struct anon_vma_chain *avc,
				struct anon_vma *anon_vma)
{
	avc->vma = vma;
	avc->anon_vma = anon_vma;
	list_add(&avc->same_vma, &vma->anon_vma_chain);
<<<<<<< HEAD

	/*
	 * It's critical to add new vmas to the tail of the anon_vma,
	 * see comment in huge_memory.c:__split_huge_page().
	 */
	list_add_tail(&avc->same_anon_vma, &anon_vma->head);
}

/**
 * anon_vma_prepare - attach an anon_vma to a memory region
=======
	anon_vma_interval_tree_insert(avc, &anon_vma->rb_root);
}

/**
 * __anon_vma_prepare - attach an anon_vma to a memory region
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @vma: the memory region in question
 *
 * This makes sure the memory mapping described by 'vma' has
 * an 'anon_vma' attached to it, so that we can associate the
 * anonymous pages mapped into it with that anon_vma.
 *
<<<<<<< HEAD
 * The common case will be that we already have one, but if
=======
 * The common case will be that we already have one, which
 * is handled inline by anon_vma_prepare(). But if
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * not we either need to find an adjacent mapping that we
 * can re-use the anon_vma from (very common when the only
 * reason for splitting a vma has been mprotect()), or we
 * allocate a new one.
 *
 * Anon-vma allocations are very subtle, because we may have
<<<<<<< HEAD
 * optimistically looked up an anon_vma in page_lock_anon_vma()
 * and that may actually touch the spinlock even in the newly
=======
 * optimistically looked up an anon_vma in folio_lock_anon_vma_read()
 * and that may actually touch the rwsem even in the newly
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * allocated vma (it depends on RCU to make sure that the
 * anon_vma isn't actually destroyed).
 *
 * As a result, we need to do proper anon_vma locking even
 * for the new allocation. At the same time, we do not want
 * to do any locking for the common case of already having
 * an anon_vma.
 *
<<<<<<< HEAD
 * This must be called with the mmap_sem held for reading.
 */
int anon_vma_prepare(struct vm_area_struct *vma)
{
	struct anon_vma *anon_vma = vma->anon_vma;
	struct anon_vma_chain *avc;

	might_sleep();
	if (unlikely(!anon_vma)) {
		struct mm_struct *mm = vma->vm_mm;
		struct anon_vma *allocated;

		avc = anon_vma_chain_alloc(GFP_KERNEL);
		if (!avc)
			goto out_enomem;

		anon_vma = find_mergeable_anon_vma(vma);
		allocated = NULL;
		if (!anon_vma) {
			anon_vma = anon_vma_alloc();
			if (unlikely(!anon_vma))
				goto out_enomem_free_avc;
			allocated = anon_vma;
		}

		anon_vma_lock(anon_vma);
		/* page_table_lock to protect against threads */
		spin_lock(&mm->page_table_lock);
		if (likely(!vma->anon_vma)) {
			vma->anon_vma = anon_vma;
			anon_vma_chain_link(vma, avc, anon_vma);
			/* vma reference or self-parent link for new root */
			anon_vma->degree++;
			allocated = NULL;
			avc = NULL;
		}
		spin_unlock(&mm->page_table_lock);
		anon_vma_unlock(anon_vma);

		if (unlikely(allocated))
			put_anon_vma(allocated);
		if (unlikely(avc))
			anon_vma_chain_free(avc);
	}
=======
 * This must be called with the mmap_lock held for reading.
 */
int __anon_vma_prepare(struct vm_area_struct *vma)
{
	struct mm_struct *mm = vma->vm_mm;
	struct anon_vma *anon_vma, *allocated;
	struct anon_vma_chain *avc;

	might_sleep();

	avc = anon_vma_chain_alloc(GFP_KERNEL);
	if (!avc)
		goto out_enomem;

	anon_vma = find_mergeable_anon_vma(vma);
	allocated = NULL;
	if (!anon_vma) {
		anon_vma = anon_vma_alloc();
		if (unlikely(!anon_vma))
			goto out_enomem_free_avc;
		anon_vma->num_children++; /* self-parent link for new root */
		allocated = anon_vma;
	}

	anon_vma_lock_write(anon_vma);
	/* page_table_lock to protect against threads */
	spin_lock(&mm->page_table_lock);
	if (likely(!vma->anon_vma)) {
		vma->anon_vma = anon_vma;
		anon_vma_chain_link(vma, avc, anon_vma);
		anon_vma->num_active_vmas++;
		allocated = NULL;
		avc = NULL;
	}
	spin_unlock(&mm->page_table_lock);
	anon_vma_unlock_write(anon_vma);

	if (unlikely(allocated))
		put_anon_vma(allocated);
	if (unlikely(avc))
		anon_vma_chain_free(avc);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

 out_enomem_free_avc:
	anon_vma_chain_free(avc);
 out_enomem:
	return -ENOMEM;
}

/*
 * This is a useful helper function for locking the anon_vma root as
 * we traverse the vma->anon_vma_chain, looping over anon_vma's that
 * have the same vma.
 *
 * Such anon_vma's should have the same root, so you'd expect to see
 * just a single mutex_lock for the whole traversal.
 */
static inline struct anon_vma *lock_anon_vma_root(struct anon_vma *root, struct anon_vma *anon_vma)
{
	struct anon_vma *new_root = anon_vma->root;
	if (new_root != root) {
		if (WARN_ON_ONCE(root))
<<<<<<< HEAD
			mutex_unlock(&root->mutex);
		root = new_root;
		mutex_lock(&root->mutex);
=======
			up_write(&root->rwsem);
		root = new_root;
		down_write(&root->rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return root;
}

static inline void unlock_anon_vma_root(struct anon_vma *root)
{
	if (root)
<<<<<<< HEAD
		mutex_unlock(&root->mutex);
=======
		up_write(&root->rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Attach the anon_vmas from src to dst.
 * Returns 0 on success, -ENOMEM on failure.
 *
<<<<<<< HEAD
 * If dst->anon_vma is NULL this function tries to find and reuse existing
 * anon_vma which has no vmas and only one child anon_vma. This prevents
 * degradation of anon_vma hierarchy to endless linear chain in case of
 * constantly forking task. On the other hand, an anon_vma with more than one
 * child isn't reused even if there was no alive vma, thus rmap walker has a
 * good chance of avoiding scanning the whole hierarchy when it searches where
 * page is mapped.
=======
 * anon_vma_clone() is called by vma_expand(), vma_merge(), __split_vma(),
 * copy_vma() and anon_vma_fork(). The first four want an exact copy of src,
 * while the last one, anon_vma_fork(), may try to reuse an existing anon_vma to
 * prevent endless growth of anon_vma. Since dst->anon_vma is set to NULL before
 * call, we can identify this case by checking (!dst->anon_vma &&
 * src->anon_vma).
 *
 * If (!dst->anon_vma && src->anon_vma) is true, this function tries to find
 * and reuse existing anon_vma which has no vmas and only one child anon_vma.
 * This prevents degradation of anon_vma hierarchy to endless linear chain in
 * case of constantly forking task. On the other hand, an anon_vma with more
 * than one child isn't reused even if there was no alive vma, thus rmap
 * walker has a good chance of avoiding scanning the whole hierarchy when it
 * searches where page is mapped.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int anon_vma_clone(struct vm_area_struct *dst, struct vm_area_struct *src)
{
	struct anon_vma_chain *avc, *pavc;
	struct anon_vma *root = NULL;

	list_for_each_entry_reverse(pavc, &src->anon_vma_chain, same_vma) {
		struct anon_vma *anon_vma;

		avc = anon_vma_chain_alloc(GFP_NOWAIT | __GFP_NOWARN);
		if (unlikely(!avc)) {
			unlock_anon_vma_root(root);
			root = NULL;
			avc = anon_vma_chain_alloc(GFP_KERNEL);
			if (!avc)
				goto enomem_failure;
		}
		anon_vma = pavc->anon_vma;
		root = lock_anon_vma_root(root, anon_vma);
		anon_vma_chain_link(dst, avc, anon_vma);

		/*
<<<<<<< HEAD
		 * Reuse existing anon_vma if its degree lower than two,
		 * that means it has no vma and only one anon_vma child.
		 *
		 * Do not chose parent anon_vma, otherwise first child
		 * will always reuse it. Root anon_vma is never reused:
		 * it has self-parent reference and at least one child.
		 */
		if (!dst->anon_vma && anon_vma != src->anon_vma &&
				anon_vma->degree < 2)
			dst->anon_vma = anon_vma;
	}
	if (dst->anon_vma)
		dst->anon_vma->degree++;
=======
		 * Reuse existing anon_vma if it has no vma and only one
		 * anon_vma child.
		 *
		 * Root anon_vma is never reused:
		 * it has self-parent reference and at least one child.
		 */
		if (!dst->anon_vma && src->anon_vma &&
		    anon_vma->num_children < 2 &&
		    anon_vma->num_active_vmas == 0)
			dst->anon_vma = anon_vma;
	}
	if (dst->anon_vma)
		dst->anon_vma->num_active_vmas++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unlock_anon_vma_root(root);
	return 0;

 enomem_failure:
	/*
<<<<<<< HEAD
	 * dst->anon_vma is dropped here otherwise its degree can be incorrectly
	 * decremented in unlink_anon_vmas().
=======
	 * dst->anon_vma is dropped here otherwise its num_active_vmas can
	 * be incorrectly decremented in unlink_anon_vmas().
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * We can safely do this because callers of anon_vma_clone() don't care
	 * about dst->anon_vma if anon_vma_clone() failed.
	 */
	dst->anon_vma = NULL;
	unlink_anon_vmas(dst);
	return -ENOMEM;
}

/*
<<<<<<< HEAD
 * Some rmap walk that needs to find all ptes/hugepmds without false
 * negatives (like migrate and split_huge_page) running concurrent
 * with operations that copy or move pagetables (like mremap() and
 * fork()) to be safe. They depend on the anon_vma "same_anon_vma"
 * list to be in a certain order: the dst_vma must be placed after the
 * src_vma in the list. This is always guaranteed by fork() but
 * mremap() needs to call this function to enforce it in case the
 * dst_vma isn't newly allocated and chained with the anon_vma_clone()
 * function but just an extension of a pre-existing vma through
 * vma_merge.
 *
 * NOTE: the same_anon_vma list can still be changed by other
 * processes while mremap runs because mremap doesn't hold the
 * anon_vma mutex to prevent modifications to the list while it
 * runs. All we need to enforce is that the relative order of this
 * process vmas isn't changing (we don't care about other vmas
 * order). Each vma corresponds to an anon_vma_chain structure so
 * there's no risk that other processes calling anon_vma_moveto_tail()
 * and changing the same_anon_vma list under mremap() will screw with
 * the relative order of this process vmas in the list, because we
 * they can't alter the order of any vma that belongs to this
 * process. And there can't be another anon_vma_moveto_tail() running
 * concurrently with mremap() coming from this process because we hold
 * the mmap_sem for the whole mremap(). fork() ordering dependency
 * also shouldn't be affected because fork() only cares that the
 * parent vmas are placed in the list before the child vmas and
 * anon_vma_moveto_tail() won't reorder vmas from either the fork()
 * parent or child.
 */
void anon_vma_moveto_tail(struct vm_area_struct *dst)
{
	struct anon_vma_chain *pavc;
	struct anon_vma *root = NULL;

	list_for_each_entry_reverse(pavc, &dst->anon_vma_chain, same_vma) {
		struct anon_vma *anon_vma = pavc->anon_vma;
		VM_BUG_ON(pavc->vma != dst);
		root = lock_anon_vma_root(root, anon_vma);
		list_del(&pavc->same_anon_vma);
		list_add_tail(&pavc->same_anon_vma, &anon_vma->head);
	}
	unlock_anon_vma_root(root);
}

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Attach vma to its own anon_vma, as well as to the anon_vmas that
 * the corresponding VMA in the parent process is attached to.
 * Returns 0 on success, non-zero on failure.
 */
int anon_vma_fork(struct vm_area_struct *vma, struct vm_area_struct *pvma)
{
	struct anon_vma_chain *avc;
	struct anon_vma *anon_vma;
<<<<<<< HEAD
=======
	int error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Don't bother if the parent process has no anon_vma here. */
	if (!pvma->anon_vma)
		return 0;

	/* Drop inherited anon_vma, we'll reuse existing or allocate new. */
	vma->anon_vma = NULL;

	/*
	 * First, attach the new VMA to the parent VMA's anon_vmas,
	 * so rmap can find non-COWed pages in child processes.
	 */
<<<<<<< HEAD
	if (anon_vma_clone(vma, pvma))
		return -ENOMEM;
=======
	error = anon_vma_clone(vma, pvma);
	if (error)
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* An existing anon_vma has been reused, all done then. */
	if (vma->anon_vma)
		return 0;

	/* Then add our own anon_vma. */
	anon_vma = anon_vma_alloc();
	if (!anon_vma)
		goto out_error;
<<<<<<< HEAD
=======
	anon_vma->num_active_vmas++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	avc = anon_vma_chain_alloc(GFP_KERNEL);
	if (!avc)
		goto out_error_free_anon_vma;

	/*
<<<<<<< HEAD
	 * The root anon_vma's spinlock is the lock actually used when we
=======
	 * The root anon_vma's rwsem is the lock actually used when we
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * lock any of the anon_vmas in this anon_vma tree.
	 */
	anon_vma->root = pvma->anon_vma->root;
	anon_vma->parent = pvma->anon_vma;
	/*
	 * With refcounts, an anon_vma can stay around longer than the
	 * process it belongs to. The root anon_vma needs to be pinned until
	 * this anon_vma is freed, because the lock lives in the root.
	 */
	get_anon_vma(anon_vma->root);
	/* Mark this anon_vma as the one where our new (COWed) pages go. */
	vma->anon_vma = anon_vma;
<<<<<<< HEAD
	anon_vma_lock(anon_vma);
	anon_vma_chain_link(vma, avc, anon_vma);
	anon_vma->parent->degree++;
	anon_vma_unlock(anon_vma);
=======
	anon_vma_lock_write(anon_vma);
	anon_vma_chain_link(vma, avc, anon_vma);
	anon_vma->parent->num_children++;
	anon_vma_unlock_write(anon_vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 out_error_free_anon_vma:
	put_anon_vma(anon_vma);
 out_error:
	unlink_anon_vmas(vma);
	return -ENOMEM;
}

void unlink_anon_vmas(struct vm_area_struct *vma)
{
	struct anon_vma_chain *avc, *next;
	struct anon_vma *root = NULL;

	/*
	 * Unlink each anon_vma chained to the VMA.  This list is ordered
	 * from newest to oldest, ensuring the root anon_vma gets freed last.
	 */
	list_for_each_entry_safe(avc, next, &vma->anon_vma_chain, same_vma) {
		struct anon_vma *anon_vma = avc->anon_vma;

		root = lock_anon_vma_root(root, anon_vma);
<<<<<<< HEAD
		list_del(&avc->same_anon_vma);
=======
		anon_vma_interval_tree_remove(avc, &anon_vma->rb_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Leave empty anon_vmas on the list - we'll need
		 * to free them outside the lock.
		 */
<<<<<<< HEAD
		if (list_empty(&anon_vma->head)) {
			anon_vma->parent->degree--;
=======
		if (RB_EMPTY_ROOT(&anon_vma->rb_root.rb_root)) {
			anon_vma->parent->num_children--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;
		}

		list_del(&avc->same_vma);
		anon_vma_chain_free(avc);
	}
<<<<<<< HEAD
	if (vma->anon_vma)
		vma->anon_vma->degree--;
=======
	if (vma->anon_vma) {
		vma->anon_vma->num_active_vmas--;

		/*
		 * vma would still be needed after unlink, and anon_vma will be prepared
		 * when handle fault.
		 */
		vma->anon_vma = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unlock_anon_vma_root(root);

	/*
	 * Iterate the list once more, it now only contains empty and unlinked
	 * anon_vmas, destroy them. Could not do before due to __put_anon_vma()
<<<<<<< HEAD
	 * needing to acquire the anon_vma->root->mutex.
=======
	 * needing to write-acquire the anon_vma->root->rwsem.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	list_for_each_entry_safe(avc, next, &vma->anon_vma_chain, same_vma) {
		struct anon_vma *anon_vma = avc->anon_vma;

<<<<<<< HEAD
		BUG_ON(anon_vma->degree);
=======
		VM_WARN_ON(anon_vma->num_children);
		VM_WARN_ON(anon_vma->num_active_vmas);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_anon_vma(anon_vma);

		list_del(&avc->same_vma);
		anon_vma_chain_free(avc);
	}
}

static void anon_vma_ctor(void *data)
{
	struct anon_vma *anon_vma = data;

<<<<<<< HEAD
	mutex_init(&anon_vma->mutex);
	atomic_set(&anon_vma->refcount, 0);
	INIT_LIST_HEAD(&anon_vma->head);
=======
	init_rwsem(&anon_vma->rwsem);
	atomic_set(&anon_vma->refcount, 0);
	anon_vma->rb_root = RB_ROOT_CACHED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init anon_vma_init(void)
{
	anon_vma_cachep = kmem_cache_create("anon_vma", sizeof(struct anon_vma),
<<<<<<< HEAD
			0, SLAB_DESTROY_BY_RCU|SLAB_PANIC, anon_vma_ctor);
	anon_vma_chain_cachep = KMEM_CACHE(anon_vma_chain, SLAB_PANIC);
=======
			0, SLAB_TYPESAFE_BY_RCU|SLAB_PANIC|SLAB_ACCOUNT,
			anon_vma_ctor);
	anon_vma_chain_cachep = KMEM_CACHE(anon_vma_chain,
			SLAB_PANIC|SLAB_ACCOUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Getting a lock on a stable anon_vma from a page off the LRU is tricky!
 *
<<<<<<< HEAD
 * Since there is no serialization what so ever against page_remove_rmap()
 * the best this function can do is return a locked anon_vma that might
 * have been relevant to this page.
=======
 * Since there is no serialization what so ever against folio_remove_rmap_*()
 * the best this function can do is return a refcount increased anon_vma
 * that might have been relevant to this page.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * The page might have been remapped to a different anon_vma or the anon_vma
 * returned may already be freed (and even reused).
 *
 * In case it was remapped to a different anon_vma, the new anon_vma will be a
 * child of the old anon_vma, and the anon_vma lifetime rules will therefore
 * ensure that any anon_vma obtained from the page will still be valid for as
 * long as we observe page_mapped() [ hence all those page_mapped() tests ].
 *
 * All users of this function must be very careful when walking the anon_vma
 * chain and verify that the page in question is indeed mapped in it
 * [ something equivalent to page_mapped_in_vma() ].
 *
<<<<<<< HEAD
 * Since anon_vma's slab is DESTROY_BY_RCU and we know from page_remove_rmap()
 * that the anon_vma pointer from page->mapping is valid if there is a
 * mapcount, we can dereference the anon_vma after observing those.
 */
struct anon_vma *page_get_anon_vma(struct page *page)
=======
 * Since anon_vma's slab is SLAB_TYPESAFE_BY_RCU and we know from
 * folio_remove_rmap_*() that the anon_vma pointer from page->mapping is valid
 * if there is a mapcount, we can dereference the anon_vma after observing
 * those.
 *
 * NOTE: the caller should normally hold folio lock when calling this.  If
 * not, the caller needs to double check the anon_vma didn't change after
 * taking the anon_vma lock for either read or write (UFFDIO_MOVE can modify it
 * concurrently without folio lock protection). See folio_lock_anon_vma_read()
 * which has already covered that, and comment above remap_pages().
 */
struct anon_vma *folio_get_anon_vma(struct folio *folio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct anon_vma *anon_vma = NULL;
	unsigned long anon_mapping;

	rcu_read_lock();
<<<<<<< HEAD
	anon_mapping = (unsigned long) ACCESS_ONCE(page->mapping);
	if ((anon_mapping & PAGE_MAPPING_FLAGS) != PAGE_MAPPING_ANON)
		goto out;
	if (!page_mapped(page))
=======
	anon_mapping = (unsigned long)READ_ONCE(folio->mapping);
	if ((anon_mapping & PAGE_MAPPING_FLAGS) != PAGE_MAPPING_ANON)
		goto out;
	if (!folio_mapped(folio))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	anon_vma = (struct anon_vma *) (anon_mapping - PAGE_MAPPING_ANON);
	if (!atomic_inc_not_zero(&anon_vma->refcount)) {
		anon_vma = NULL;
		goto out;
	}

	/*
<<<<<<< HEAD
	 * If this page is still mapped, then its anon_vma cannot have been
	 * freed.  But if it has been unmapped, we have no security against the
	 * anon_vma structure being freed and reused (for another anon_vma:
	 * SLAB_DESTROY_BY_RCU guarantees that - so the atomic_inc_not_zero()
	 * above cannot corrupt).
	 */
	if (!page_mapped(page)) {
=======
	 * If this folio is still mapped, then its anon_vma cannot have been
	 * freed.  But if it has been unmapped, we have no security against the
	 * anon_vma structure being freed and reused (for another anon_vma:
	 * SLAB_TYPESAFE_BY_RCU guarantees that - so the atomic_inc_not_zero()
	 * above cannot corrupt).
	 */
	if (!folio_mapped(folio)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rcu_read_unlock();
		put_anon_vma(anon_vma);
		return NULL;
	}
out:
	rcu_read_unlock();

	return anon_vma;
}

/*
<<<<<<< HEAD
 * Similar to page_get_anon_vma() except it locks the anon_vma.
 *
 * Its a little more complex as it tries to keep the fast path to a single
 * atomic op -- the trylock. If we fail the trylock, we fall back to getting a
 * reference like with page_get_anon_vma() and then block on the mutex.
 */
struct anon_vma *page_lock_anon_vma(struct page *page)
=======
 * Similar to folio_get_anon_vma() except it locks the anon_vma.
 *
 * Its a little more complex as it tries to keep the fast path to a single
 * atomic op -- the trylock. If we fail the trylock, we fall back to getting a
 * reference like with folio_get_anon_vma() and then block on the mutex
 * on !rwc->try_lock case.
 */
struct anon_vma *folio_lock_anon_vma_read(struct folio *folio,
					  struct rmap_walk_control *rwc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct anon_vma *anon_vma = NULL;
	struct anon_vma *root_anon_vma;
	unsigned long anon_mapping;

<<<<<<< HEAD
	rcu_read_lock();
	anon_mapping = (unsigned long) ACCESS_ONCE(page->mapping);
	if ((anon_mapping & PAGE_MAPPING_FLAGS) != PAGE_MAPPING_ANON)
		goto out;
	if (!page_mapped(page))
		goto out;

	anon_vma = (struct anon_vma *) (anon_mapping - PAGE_MAPPING_ANON);
	root_anon_vma = ACCESS_ONCE(anon_vma->root);
	if (mutex_trylock(&root_anon_vma->mutex)) {
		/*
		 * If the page is still mapped, then this anon_vma is still
		 * its anon_vma, and holding the mutex ensures that it will
		 * not go away, see anon_vma_free().
		 */
		if (!page_mapped(page)) {
			mutex_unlock(&root_anon_vma->mutex);
=======
retry:
	rcu_read_lock();
	anon_mapping = (unsigned long)READ_ONCE(folio->mapping);
	if ((anon_mapping & PAGE_MAPPING_FLAGS) != PAGE_MAPPING_ANON)
		goto out;
	if (!folio_mapped(folio))
		goto out;

	anon_vma = (struct anon_vma *) (anon_mapping - PAGE_MAPPING_ANON);
	root_anon_vma = READ_ONCE(anon_vma->root);
	if (down_read_trylock(&root_anon_vma->rwsem)) {
		/*
		 * folio_move_anon_rmap() might have changed the anon_vma as we
		 * might not hold the folio lock here.
		 */
		if (unlikely((unsigned long)READ_ONCE(folio->mapping) !=
			     anon_mapping)) {
			up_read(&root_anon_vma->rwsem);
			rcu_read_unlock();
			goto retry;
		}

		/*
		 * If the folio is still mapped, then this anon_vma is still
		 * its anon_vma, and holding the mutex ensures that it will
		 * not go away, see anon_vma_free().
		 */
		if (!folio_mapped(folio)) {
			up_read(&root_anon_vma->rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			anon_vma = NULL;
		}
		goto out;
	}

<<<<<<< HEAD
=======
	if (rwc && rwc->try_lock) {
		anon_vma = NULL;
		rwc->contended = true;
		goto out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* trylock failed, we got to sleep */
	if (!atomic_inc_not_zero(&anon_vma->refcount)) {
		anon_vma = NULL;
		goto out;
	}

<<<<<<< HEAD
	if (!page_mapped(page)) {
=======
	if (!folio_mapped(folio)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rcu_read_unlock();
		put_anon_vma(anon_vma);
		return NULL;
	}

	/* we pinned the anon_vma, its safe to sleep */
	rcu_read_unlock();
<<<<<<< HEAD
	anon_vma_lock(anon_vma);
=======
	anon_vma_lock_read(anon_vma);

	/*
	 * folio_move_anon_rmap() might have changed the anon_vma as we might
	 * not hold the folio lock here.
	 */
	if (unlikely((unsigned long)READ_ONCE(folio->mapping) !=
		     anon_mapping)) {
		anon_vma_unlock_read(anon_vma);
		put_anon_vma(anon_vma);
		anon_vma = NULL;
		goto retry;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (atomic_dec_and_test(&anon_vma->refcount)) {
		/*
		 * Oops, we held the last refcount, release the lock
		 * and bail -- can't simply use put_anon_vma() because
<<<<<<< HEAD
		 * we'll deadlock on the anon_vma_lock() recursion.
		 */
		anon_vma_unlock(anon_vma);
=======
		 * we'll deadlock on the anon_vma_lock_write() recursion.
		 */
		anon_vma_unlock_read(anon_vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__put_anon_vma(anon_vma);
		anon_vma = NULL;
	}

	return anon_vma;

out:
	rcu_read_unlock();
	return anon_vma;
}

<<<<<<< HEAD
void page_unlock_anon_vma(struct anon_vma *anon_vma)
{
	anon_vma_unlock(anon_vma);
}

/*
 * At what user virtual address is page expected in @vma?
 * Returns virtual address or -EFAULT if page's index/offset is not
 * within the range mapped the @vma.
 */
inline unsigned long
vma_address(struct page *page, struct vm_area_struct *vma)
{
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	unsigned long address;

	if (unlikely(is_vm_hugetlb_page(vma)))
		pgoff = page->index << huge_page_order(page_hstate(page));
	address = vma->vm_start + ((pgoff - vma->vm_pgoff) << PAGE_SHIFT);
	if (unlikely(address < vma->vm_start || address >= vma->vm_end)) {
		/* page should be within @vma mapping range */
		return -EFAULT;
	}
	return address;
}

/*
=======
#ifdef CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH
/*
 * Flush TLB entries for recently unmapped pages from remote CPUs. It is
 * important if a PTE was dirty when it was unmapped that it's flushed
 * before any IO is initiated on the page to prevent lost writes. Similarly,
 * it must be flushed before freeing to prevent data leakage.
 */
void try_to_unmap_flush(void)
{
	struct tlbflush_unmap_batch *tlb_ubc = &current->tlb_ubc;

	if (!tlb_ubc->flush_required)
		return;

	arch_tlbbatch_flush(&tlb_ubc->arch);
	tlb_ubc->flush_required = false;
	tlb_ubc->writable = false;
}

/* Flush iff there are potentially writable TLB entries that can race with IO */
void try_to_unmap_flush_dirty(void)
{
	struct tlbflush_unmap_batch *tlb_ubc = &current->tlb_ubc;

	if (tlb_ubc->writable)
		try_to_unmap_flush();
}

/*
 * Bits 0-14 of mm->tlb_flush_batched record pending generations.
 * Bits 16-30 of mm->tlb_flush_batched bit record flushed generations.
 */
#define TLB_FLUSH_BATCH_FLUSHED_SHIFT	16
#define TLB_FLUSH_BATCH_PENDING_MASK			\
	((1 << (TLB_FLUSH_BATCH_FLUSHED_SHIFT - 1)) - 1)
#define TLB_FLUSH_BATCH_PENDING_LARGE			\
	(TLB_FLUSH_BATCH_PENDING_MASK / 2)

static void set_tlb_ubc_flush_pending(struct mm_struct *mm, pte_t pteval,
				      unsigned long uaddr)
{
	struct tlbflush_unmap_batch *tlb_ubc = &current->tlb_ubc;
	int batch;
	bool writable = pte_dirty(pteval);

	if (!pte_accessible(mm, pteval))
		return;

	arch_tlbbatch_add_pending(&tlb_ubc->arch, mm, uaddr);
	tlb_ubc->flush_required = true;

	/*
	 * Ensure compiler does not re-order the setting of tlb_flush_batched
	 * before the PTE is cleared.
	 */
	barrier();
	batch = atomic_read(&mm->tlb_flush_batched);
retry:
	if ((batch & TLB_FLUSH_BATCH_PENDING_MASK) > TLB_FLUSH_BATCH_PENDING_LARGE) {
		/*
		 * Prevent `pending' from catching up with `flushed' because of
		 * overflow.  Reset `pending' and `flushed' to be 1 and 0 if
		 * `pending' becomes large.
		 */
		if (!atomic_try_cmpxchg(&mm->tlb_flush_batched, &batch, 1))
			goto retry;
	} else {
		atomic_inc(&mm->tlb_flush_batched);
	}

	/*
	 * If the PTE was dirty then it's best to assume it's writable. The
	 * caller must use try_to_unmap_flush_dirty() or try_to_unmap_flush()
	 * before the page is queued for IO.
	 */
	if (writable)
		tlb_ubc->writable = true;
}

/*
 * Returns true if the TLB flush should be deferred to the end of a batch of
 * unmap operations to reduce IPIs.
 */
static bool should_defer_flush(struct mm_struct *mm, enum ttu_flags flags)
{
	if (!(flags & TTU_BATCH_FLUSH))
		return false;

	return arch_tlbbatch_should_defer(mm);
}

/*
 * Reclaim unmaps pages under the PTL but do not flush the TLB prior to
 * releasing the PTL if TLB flushes are batched. It's possible for a parallel
 * operation such as mprotect or munmap to race between reclaim unmapping
 * the page and flushing the page. If this race occurs, it potentially allows
 * access to data via a stale TLB entry. Tracking all mm's that have TLB
 * batching in flight would be expensive during reclaim so instead track
 * whether TLB batching occurred in the past and if so then do a flush here
 * if required. This will cost one additional flush per reclaim cycle paid
 * by the first operation at risk such as mprotect and mumap.
 *
 * This must be called under the PTL so that an access to tlb_flush_batched
 * that is potentially a "reclaim vs mprotect/munmap/etc" race will synchronise
 * via the PTL.
 */
void flush_tlb_batched_pending(struct mm_struct *mm)
{
	int batch = atomic_read(&mm->tlb_flush_batched);
	int pending = batch & TLB_FLUSH_BATCH_PENDING_MASK;
	int flushed = batch >> TLB_FLUSH_BATCH_FLUSHED_SHIFT;

	if (pending != flushed) {
		arch_flush_tlb_batched_pending(mm);
		/*
		 * If the new TLB flushing is pending during flushing, leave
		 * mm->tlb_flush_batched as is, to avoid losing flushing.
		 */
		atomic_cmpxchg(&mm->tlb_flush_batched, batch,
			       pending | (pending << TLB_FLUSH_BATCH_FLUSHED_SHIFT));
	}
}
#else
static void set_tlb_ubc_flush_pending(struct mm_struct *mm, pte_t pteval,
				      unsigned long uaddr)
{
}

static bool should_defer_flush(struct mm_struct *mm, enum ttu_flags flags)
{
	return false;
}
#endif /* CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH */

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * At what user virtual address is page expected in vma?
 * Caller should check the page is actually part of the vma.
 */
unsigned long page_address_in_vma(struct page *page, struct vm_area_struct *vma)
{
<<<<<<< HEAD
	if (PageAnon(page)) {
		struct anon_vma *page__anon_vma = page_anon_vma(page);
=======
	struct folio *folio = page_folio(page);
	if (folio_test_anon(folio)) {
		struct anon_vma *page__anon_vma = folio_anon_vma(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Note: swapoff's unuse_vma() is more efficient with this
		 * check, and needs it to match anon_vma when KSM is active.
		 */
		if (!vma->anon_vma || !page__anon_vma ||
		    vma->anon_vma->root != page__anon_vma->root)
			return -EFAULT;
<<<<<<< HEAD
	} else if (page->mapping && !(vma->vm_flags & VM_NONLINEAR)) {
		if (!vma->vm_file ||
		    vma->vm_file->f_mapping != page->mapping)
			return -EFAULT;
	} else
		return -EFAULT;
=======
	} else if (!vma->vm_file) {
		return -EFAULT;
	} else if (vma->vm_file->f_mapping != folio->mapping) {
		return -EFAULT;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return vma_address(page, vma);
}

/*
<<<<<<< HEAD
 * Check that @page is mapped at @address into @mm.
 *
 * If @sync is false, page_check_address may perform a racy check to avoid
 * the page table lock when the pte is not present (helpful when reclaiming
 * highly shared pages).
 *
 * On success returns with pte mapped and locked.
 */
pte_t *__page_check_address(struct page *page, struct mm_struct *mm,
			  unsigned long address, spinlock_t **ptlp, int sync)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	spinlock_t *ptl;

	if (unlikely(PageHuge(page))) {
		/* when pud is not present, pte will be NULL */
		pte = huge_pte_offset(mm, address);
		if (!pte)
			return NULL;

		ptl = &mm->page_table_lock;
		goto check;
	}

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		return NULL;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		return NULL;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd))
		return NULL;
	if (pmd_trans_huge(*pmd))
		return NULL;

	pte = pte_offset_map(pmd, address);
	/* Make a quick check before getting the lock */
	if (!sync && !pte_present(*pte)) {
		pte_unmap(pte);
		return NULL;
	}

	ptl = pte_lockptr(mm, pmd);
check:
	spin_lock(ptl);
	if (pte_present(*pte) && page_to_pfn(page) == pte_pfn(*pte)) {
		*ptlp = ptl;
		return pte;
	}
	pte_unmap_unlock(pte, ptl);
	return NULL;
}

/**
 * page_mapped_in_vma - check whether a page is really mapped in a VMA
 * @page: the page to test
 * @vma: the VMA to test
 *
 * Returns 1 if the page is mapped into the page tables of the VMA, 0
 * if the page is not mapped into the page tables of this VMA.  Only
 * valid for normal file or anonymous VMAs.
 */
int page_mapped_in_vma(struct page *page, struct vm_area_struct *vma)
{
	unsigned long address;
	pte_t *pte;
	spinlock_t *ptl;

	address = vma_address(page, vma);
	if (address == -EFAULT)		/* out of vma range */
		return 0;
	pte = page_check_address(page, vma->vm_mm, address, &ptl, 1);
	if (!pte)			/* the page is not in this mm */
		return 0;
	pte_unmap_unlock(pte, ptl);

	return 1;
}

/*
 * Subfunctions of page_referenced: page_referenced_one called
 * repeatedly from either page_referenced_anon or page_referenced_file.
 */
int page_referenced_one(struct page *page, struct vm_area_struct *vma,
			unsigned long address, unsigned int *mapcount,
			unsigned long *vm_flags)
{
	struct mm_struct *mm = vma->vm_mm;
	int referenced = 0;

	if (unlikely(PageTransHuge(page))) {
		pmd_t *pmd;

		spin_lock(&mm->page_table_lock);
		/*
		 * rmap might return false positives; we must filter
		 * these out using page_check_address_pmd().
		 */
		pmd = page_check_address_pmd(page, mm, address,
					     PAGE_CHECK_ADDRESS_PMD_FLAG);
		if (!pmd) {
			spin_unlock(&mm->page_table_lock);
			goto out;
		}

		if (vma->vm_flags & VM_LOCKED) {
			spin_unlock(&mm->page_table_lock);
			*mapcount = 0;	/* break early from loop */
			*vm_flags |= VM_LOCKED;
			goto out;
		}

		/* go ahead even if the pmd is pmd_trans_splitting() */
		if (pmdp_clear_flush_young_notify(vma, address, pmd))
			referenced++;
		spin_unlock(&mm->page_table_lock);
	} else {
		pte_t *pte;
		spinlock_t *ptl;

		/*
		 * rmap might return false positives; we must filter
		 * these out using page_check_address().
		 */
		pte = page_check_address(page, mm, address, &ptl, 0);
		if (!pte)
			goto out;

		if (vma->vm_flags & VM_LOCKED) {
			pte_unmap_unlock(pte, ptl);
			*mapcount = 0;	/* break early from loop */
			*vm_flags |= VM_LOCKED;
			goto out;
		}

		if (ptep_clear_flush_young_notify(vma, address, pte)) {
			/*
			 * Don't treat a reference through a sequentially read
			 * mapping as such.  If the page has been used in
			 * another mapping, we will catch it; if this other
			 * mapping is already gone, the unmap path will have
			 * set PG_referenced or activated the page.
			 */
			if (likely(!VM_SequentialReadHint(vma)))
				referenced++;
		}
		pte_unmap_unlock(pte, ptl);
	}

	(*mapcount)--;

	if (referenced)
		*vm_flags |= vma->vm_flags;
out:
	return referenced;
}

static int page_referenced_anon(struct page *page,
				struct mem_cgroup *memcg,
				unsigned long *vm_flags)
{
	unsigned int mapcount;
	struct anon_vma *anon_vma;
	struct anon_vma_chain *avc;
	int referenced = 0;

	anon_vma = page_lock_anon_vma(page);
	if (!anon_vma)
		return referenced;

	mapcount = page_mapcount(page);
	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		/*
		 * If we are reclaiming on behalf of a cgroup, skip
		 * counting on behalf of references from different
		 * cgroups
		 */
		if (memcg && !mm_match_cgroup(vma->vm_mm, memcg))
			continue;
		referenced += page_referenced_one(page, vma, address,
						  &mapcount, vm_flags);
		if (!mapcount)
			break;
	}

	page_unlock_anon_vma(anon_vma);
	return referenced;
}

/**
 * page_referenced_file - referenced check for object-based rmap
 * @page: the page we're checking references on.
 * @memcg: target memory control group
 * @vm_flags: collect encountered vma->vm_flags who actually referenced the page
 *
 * For an object-based mapped page, find all the places it is mapped and
 * check/clear the referenced flag.  This is done by following the page->mapping
 * pointer, then walking the chain of vmas it holds.  It returns the number
 * of references it found.
 *
 * This function is only called from page_referenced for object-based pages.
 */
static int page_referenced_file(struct page *page,
				struct mem_cgroup *memcg,
				unsigned long *vm_flags)
{
	unsigned int mapcount;
	struct address_space *mapping = page->mapping;
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	struct vm_area_struct *vma;
	struct prio_tree_iter iter;
	int referenced = 0;

	/*
	 * The caller's checks on page->mapping and !PageAnon have made
	 * sure that this is a file page: the check for page->mapping
	 * excludes the case just before it gets set on an anon page.
	 */
	BUG_ON(PageAnon(page));

	/*
	 * The page lock not only makes sure that page->mapping cannot
	 * suddenly be NULLified by truncation, it makes sure that the
	 * structure at mapping cannot be freed and reused yet,
	 * so we can safely take mapping->i_mmap_mutex.
	 */
	BUG_ON(!PageLocked(page));

	mutex_lock(&mapping->i_mmap_mutex);

	/*
	 * i_mmap_mutex does not stabilize mapcount at all, but mapcount
	 * is more likely to be accurate if we note it after spinning.
	 */
	mapcount = page_mapcount(page);

	vma_prio_tree_foreach(vma, &iter, &mapping->i_mmap, pgoff, pgoff) {
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		/*
		 * If we are reclaiming on behalf of a cgroup, skip
		 * counting on behalf of references from different
		 * cgroups
		 */
		if (memcg && !mm_match_cgroup(vma->vm_mm, memcg))
			continue;
		referenced += page_referenced_one(page, vma, address,
						  &mapcount, vm_flags);
		if (!mapcount)
			break;
	}

	mutex_unlock(&mapping->i_mmap_mutex);
	return referenced;
}

/**
 * page_referenced - test if the page was referenced
 * @page: the page to test
 * @is_locked: caller holds lock on the page
 * @memcg: target memory cgroup
 * @vm_flags: collect encountered vma->vm_flags who actually referenced the page
 *
 * Quick test_and_clear_referenced for all mappings to a page,
 * returns the number of ptes which referenced the page.
 */
int page_referenced(struct page *page,
		    int is_locked,
		    struct mem_cgroup *memcg,
		    unsigned long *vm_flags)
{
	int referenced = 0;
	int we_locked = 0;

	*vm_flags = 0;
	if (page_mapped(page) && page_rmapping(page)) {
		if (!is_locked && (!PageAnon(page) || PageKsm(page))) {
			we_locked = trylock_page(page);
			if (!we_locked) {
				referenced++;
				goto out;
			}
		}
		if (unlikely(PageKsm(page)))
			referenced += page_referenced_ksm(page, memcg,
								vm_flags);
		else if (PageAnon(page))
			referenced += page_referenced_anon(page, memcg,
								vm_flags);
		else if (page->mapping)
			referenced += page_referenced_file(page, memcg,
								vm_flags);
		if (we_locked)
			unlock_page(page);

		if (page_test_and_clear_young(page_to_pfn(page)))
			referenced++;
	}
out:
	return referenced;
}

static int page_mkclean_one(struct page *page, struct vm_area_struct *vma,
			    unsigned long address)
{
	struct mm_struct *mm = vma->vm_mm;
	pte_t *pte;
	spinlock_t *ptl;
	int ret = 0;

	pte = page_check_address(page, mm, address, &ptl, 1);
	if (!pte)
		goto out;

	if (pte_dirty(*pte) || pte_write(*pte)) {
		pte_t entry;

		flush_cache_page(vma, address, pte_pfn(*pte));
		entry = ptep_clear_flush_notify(vma, address, pte);
		entry = pte_wrprotect(entry);
		entry = pte_mkclean(entry);
		set_pte_at(mm, address, pte, entry);
		ret = 1;
	}

	pte_unmap_unlock(pte, ptl);
out:
	return ret;
}

static int page_mkclean_file(struct address_space *mapping, struct page *page)
{
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	struct vm_area_struct *vma;
	struct prio_tree_iter iter;
	int ret = 0;

	BUG_ON(PageAnon(page));

	mutex_lock(&mapping->i_mmap_mutex);
	vma_prio_tree_foreach(vma, &iter, &mapping->i_mmap, pgoff, pgoff) {
		if (vma->vm_flags & VM_SHARED) {
			unsigned long address = vma_address(page, vma);
			if (address == -EFAULT)
				continue;
			ret += page_mkclean_one(page, vma, address);
		}
	}
	mutex_unlock(&mapping->i_mmap_mutex);
	return ret;
}

int page_mkclean(struct page *page)
{
	int ret = 0;

	BUG_ON(!PageLocked(page));

	if (page_mapped(page)) {
		struct address_space *mapping = page_mapping(page);
		if (mapping)
			ret = page_mkclean_file(mapping, page);
	}

	return ret;
}
EXPORT_SYMBOL_GPL(page_mkclean);

/**
 * page_move_anon_rmap - move a page to our anon_vma
 * @page:	the page to move to our anon_vma
 * @vma:	the vma the page belongs to
 * @address:	the user virtual address mapped
 *
 * When a page belongs exclusively to one process after a COW event,
 * that page can be moved into the anon_vma that belongs to just that
 * process, so the rmap code will not search the parent or sibling
 * processes.
 */
void page_move_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address)
{
	struct anon_vma *anon_vma = vma->anon_vma;

	VM_BUG_ON(!PageLocked(page));
	VM_BUG_ON(!anon_vma);
	VM_BUG_ON(page->index != linear_page_index(vma, address));

	anon_vma = (void *) anon_vma + PAGE_MAPPING_ANON;
	page->mapping = (struct address_space *) anon_vma;
}

/**
 * __page_set_anon_rmap - set up new anonymous rmap
 * @page:	Page to add to rmap	
 * @vma:	VM area to add page to.
 * @address:	User virtual address of the mapping	
 * @exclusive:	the page is exclusively owned by the current process
 */
static void __page_set_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address, int exclusive)
=======
 * Returns the actual pmd_t* where we expect 'address' to be mapped from, or
 * NULL if it doesn't exist.  No guarantees / checks on what the pmd_t*
 * represents.
 */
pmd_t *mm_find_pmd(struct mm_struct *mm, unsigned long address)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd = NULL;

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		goto out;

	p4d = p4d_offset(pgd, address);
	if (!p4d_present(*p4d))
		goto out;

	pud = pud_offset(p4d, address);
	if (!pud_present(*pud))
		goto out;

	pmd = pmd_offset(pud, address);
out:
	return pmd;
}

struct folio_referenced_arg {
	int mapcount;
	int referenced;
	unsigned long vm_flags;
	struct mem_cgroup *memcg;
};

/*
 * arg: folio_referenced_arg will be passed
 */
static bool folio_referenced_one(struct folio *folio,
		struct vm_area_struct *vma, unsigned long address, void *arg)
{
	struct folio_referenced_arg *pra = arg;
	DEFINE_FOLIO_VMA_WALK(pvmw, folio, vma, address, 0);
	int referenced = 0;
	unsigned long start = address, ptes = 0;

	while (page_vma_mapped_walk(&pvmw)) {
		address = pvmw.address;

		if (vma->vm_flags & VM_LOCKED) {
			if (!folio_test_large(folio) || !pvmw.pte) {
				/* Restore the mlock which got missed */
				mlock_vma_folio(folio, vma);
				page_vma_mapped_walk_done(&pvmw);
				pra->vm_flags |= VM_LOCKED;
				return false; /* To break the loop */
			}
			/*
			 * For large folio fully mapped to VMA, will
			 * be handled after the pvmw loop.
			 *
			 * For large folio cross VMA boundaries, it's
			 * expected to be picked  by page reclaim. But
			 * should skip reference of pages which are in
			 * the range of VM_LOCKED vma. As page reclaim
			 * should just count the reference of pages out
			 * the range of VM_LOCKED vma.
			 */
			ptes++;
			pra->mapcount--;
			continue;
		}

		if (pvmw.pte) {
			if (lru_gen_enabled() &&
			    pte_young(ptep_get(pvmw.pte))) {
				lru_gen_look_around(&pvmw);
				referenced++;
			}

			if (ptep_clear_flush_young_notify(vma, address,
						pvmw.pte))
				referenced++;
		} else if (IS_ENABLED(CONFIG_TRANSPARENT_HUGEPAGE)) {
			if (pmdp_clear_flush_young_notify(vma, address,
						pvmw.pmd))
				referenced++;
		} else {
			/* unexpected pmd-mapped folio? */
			WARN_ON_ONCE(1);
		}

		pra->mapcount--;
	}

	if ((vma->vm_flags & VM_LOCKED) &&
			folio_test_large(folio) &&
			folio_within_vma(folio, vma)) {
		unsigned long s_align, e_align;

		s_align = ALIGN_DOWN(start, PMD_SIZE);
		e_align = ALIGN_DOWN(start + folio_size(folio) - 1, PMD_SIZE);

		/* folio doesn't cross page table boundary and fully mapped */
		if ((s_align == e_align) && (ptes == folio_nr_pages(folio))) {
			/* Restore the mlock which got missed */
			mlock_vma_folio(folio, vma);
			pra->vm_flags |= VM_LOCKED;
			return false; /* To break the loop */
		}
	}

	if (referenced)
		folio_clear_idle(folio);
	if (folio_test_clear_young(folio))
		referenced++;

	if (referenced) {
		pra->referenced++;
		pra->vm_flags |= vma->vm_flags & ~VM_LOCKED;
	}

	if (!pra->mapcount)
		return false; /* To break the loop */

	return true;
}

static bool invalid_folio_referenced_vma(struct vm_area_struct *vma, void *arg)
{
	struct folio_referenced_arg *pra = arg;
	struct mem_cgroup *memcg = pra->memcg;

	/*
	 * Ignore references from this mapping if it has no recency. If the
	 * folio has been used in another mapping, we will catch it; if this
	 * other mapping is already gone, the unmap path will have set the
	 * referenced flag or activated the folio in zap_pte_range().
	 */
	if (!vma_has_recency(vma))
		return true;

	/*
	 * If we are reclaiming on behalf of a cgroup, skip counting on behalf
	 * of references from different cgroups.
	 */
	if (memcg && !mm_match_cgroup(vma->vm_mm, memcg))
		return true;

	return false;
}

/**
 * folio_referenced() - Test if the folio was referenced.
 * @folio: The folio to test.
 * @is_locked: Caller holds lock on the folio.
 * @memcg: target memory cgroup
 * @vm_flags: A combination of all the vma->vm_flags which referenced the folio.
 *
 * Quick test_and_clear_referenced for all mappings of a folio,
 *
 * Return: The number of mappings which referenced the folio. Return -1 if
 * the function bailed out due to rmap lock contention.
 */
int folio_referenced(struct folio *folio, int is_locked,
		     struct mem_cgroup *memcg, unsigned long *vm_flags)
{
	int we_locked = 0;
	struct folio_referenced_arg pra = {
		.mapcount = folio_mapcount(folio),
		.memcg = memcg,
	};
	struct rmap_walk_control rwc = {
		.rmap_one = folio_referenced_one,
		.arg = (void *)&pra,
		.anon_lock = folio_lock_anon_vma_read,
		.try_lock = true,
		.invalid_vma = invalid_folio_referenced_vma,
	};

	*vm_flags = 0;
	if (!pra.mapcount)
		return 0;

	if (!folio_raw_mapping(folio))
		return 0;

	if (!is_locked && (!folio_test_anon(folio) || folio_test_ksm(folio))) {
		we_locked = folio_trylock(folio);
		if (!we_locked)
			return 1;
	}

	rmap_walk(folio, &rwc);
	*vm_flags = pra.vm_flags;

	if (we_locked)
		folio_unlock(folio);

	return rwc.contended ? -1 : pra.referenced;
}

static int page_vma_mkclean_one(struct page_vma_mapped_walk *pvmw)
{
	int cleaned = 0;
	struct vm_area_struct *vma = pvmw->vma;
	struct mmu_notifier_range range;
	unsigned long address = pvmw->address;

	/*
	 * We have to assume the worse case ie pmd for invalidation. Note that
	 * the folio can not be freed from this function.
	 */
	mmu_notifier_range_init(&range, MMU_NOTIFY_PROTECTION_PAGE, 0,
				vma->vm_mm, address, vma_address_end(pvmw));
	mmu_notifier_invalidate_range_start(&range);

	while (page_vma_mapped_walk(pvmw)) {
		int ret = 0;

		address = pvmw->address;
		if (pvmw->pte) {
			pte_t *pte = pvmw->pte;
			pte_t entry = ptep_get(pte);

			if (!pte_dirty(entry) && !pte_write(entry))
				continue;

			flush_cache_page(vma, address, pte_pfn(entry));
			entry = ptep_clear_flush(vma, address, pte);
			entry = pte_wrprotect(entry);
			entry = pte_mkclean(entry);
			set_pte_at(vma->vm_mm, address, pte, entry);
			ret = 1;
		} else {
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
			pmd_t *pmd = pvmw->pmd;
			pmd_t entry;

			if (!pmd_dirty(*pmd) && !pmd_write(*pmd))
				continue;

			flush_cache_range(vma, address,
					  address + HPAGE_PMD_SIZE);
			entry = pmdp_invalidate(vma, address, pmd);
			entry = pmd_wrprotect(entry);
			entry = pmd_mkclean(entry);
			set_pmd_at(vma->vm_mm, address, pmd, entry);
			ret = 1;
#else
			/* unexpected pmd-mapped folio? */
			WARN_ON_ONCE(1);
#endif
		}

		if (ret)
			cleaned++;
	}

	mmu_notifier_invalidate_range_end(&range);

	return cleaned;
}

static bool page_mkclean_one(struct folio *folio, struct vm_area_struct *vma,
			     unsigned long address, void *arg)
{
	DEFINE_FOLIO_VMA_WALK(pvmw, folio, vma, address, PVMW_SYNC);
	int *cleaned = arg;

	*cleaned += page_vma_mkclean_one(&pvmw);

	return true;
}

static bool invalid_mkclean_vma(struct vm_area_struct *vma, void *arg)
{
	if (vma->vm_flags & VM_SHARED)
		return false;

	return true;
}

int folio_mkclean(struct folio *folio)
{
	int cleaned = 0;
	struct address_space *mapping;
	struct rmap_walk_control rwc = {
		.arg = (void *)&cleaned,
		.rmap_one = page_mkclean_one,
		.invalid_vma = invalid_mkclean_vma,
	};

	BUG_ON(!folio_test_locked(folio));

	if (!folio_mapped(folio))
		return 0;

	mapping = folio_mapping(folio);
	if (!mapping)
		return 0;

	rmap_walk(folio, &rwc);

	return cleaned;
}
EXPORT_SYMBOL_GPL(folio_mkclean);

/**
 * pfn_mkclean_range - Cleans the PTEs (including PMDs) mapped with range of
 *                     [@pfn, @pfn + @nr_pages) at the specific offset (@pgoff)
 *                     within the @vma of shared mappings. And since clean PTEs
 *                     should also be readonly, write protects them too.
 * @pfn: start pfn.
 * @nr_pages: number of physically contiguous pages srarting with @pfn.
 * @pgoff: page offset that the @pfn mapped with.
 * @vma: vma that @pfn mapped within.
 *
 * Returns the number of cleaned PTEs (including PMDs).
 */
int pfn_mkclean_range(unsigned long pfn, unsigned long nr_pages, pgoff_t pgoff,
		      struct vm_area_struct *vma)
{
	struct page_vma_mapped_walk pvmw = {
		.pfn		= pfn,
		.nr_pages	= nr_pages,
		.pgoff		= pgoff,
		.vma		= vma,
		.flags		= PVMW_SYNC,
	};

	if (invalid_mkclean_vma(vma, NULL))
		return 0;

	pvmw.address = vma_pgoff_address(pgoff, nr_pages, vma);
	VM_BUG_ON_VMA(pvmw.address == -EFAULT, vma);

	return page_vma_mkclean_one(&pvmw);
}

int folio_total_mapcount(struct folio *folio)
{
	int mapcount = folio_entire_mapcount(folio);
	int nr_pages;
	int i;

	/* In the common case, avoid the loop when no pages mapped by PTE */
	if (folio_nr_pages_mapped(folio) == 0)
		return mapcount;
	/*
	 * Add all the PTE mappings of those pages mapped by PTE.
	 * Limit the loop to folio_nr_pages_mapped()?
	 * Perhaps: given all the raciness, that may be a good or a bad idea.
	 */
	nr_pages = folio_nr_pages(folio);
	for (i = 0; i < nr_pages; i++)
		mapcount += atomic_read(&folio_page(folio, i)->_mapcount);

	/* But each of those _mapcounts was based on -1 */
	mapcount += nr_pages;
	return mapcount;
}

static __always_inline unsigned int __folio_add_rmap(struct folio *folio,
		struct page *page, int nr_pages, enum rmap_level level,
		int *nr_pmdmapped)
{
	atomic_t *mapped = &folio->_nr_pages_mapped;
	int first, nr = 0;

	__folio_rmap_sanity_checks(folio, page, nr_pages, level);

	switch (level) {
	case RMAP_LEVEL_PTE:
		do {
			first = atomic_inc_and_test(&page->_mapcount);
			if (first && folio_test_large(folio)) {
				first = atomic_inc_return_relaxed(mapped);
				first = (first < ENTIRELY_MAPPED);
			}

			if (first)
				nr++;
		} while (page++, --nr_pages > 0);
		break;
	case RMAP_LEVEL_PMD:
		first = atomic_inc_and_test(&folio->_entire_mapcount);
		if (first) {
			nr = atomic_add_return_relaxed(ENTIRELY_MAPPED, mapped);
			if (likely(nr < ENTIRELY_MAPPED + ENTIRELY_MAPPED)) {
				*nr_pmdmapped = folio_nr_pages(folio);
				nr = *nr_pmdmapped - (nr & FOLIO_PAGES_MAPPED);
				/* Raced ahead of a remove and another add? */
				if (unlikely(nr < 0))
					nr = 0;
			} else {
				/* Raced ahead of a remove of ENTIRELY_MAPPED */
				nr = 0;
			}
		}
		break;
	}
	return nr;
}

/**
 * folio_move_anon_rmap - move a folio to our anon_vma
 * @folio:	The folio to move to our anon_vma
 * @vma:	The vma the folio belongs to
 *
 * When a folio belongs exclusively to one process after a COW event,
 * that folio can be moved into the anon_vma that belongs to just that
 * process, so the rmap code will not search the parent or sibling processes.
 */
void folio_move_anon_rmap(struct folio *folio, struct vm_area_struct *vma)
{
	void *anon_vma = vma->anon_vma;

	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);
	VM_BUG_ON_VMA(!anon_vma, vma);

	anon_vma += PAGE_MAPPING_ANON;
	/*
	 * Ensure that anon_vma and the PAGE_MAPPING_ANON bit are written
	 * simultaneously, so a concurrent reader (eg folio_referenced()'s
	 * folio_test_anon()) will not see one without the other.
	 */
	WRITE_ONCE(folio->mapping, anon_vma);
}

/**
 * __folio_set_anon - set up a new anonymous rmap for a folio
 * @folio:	The folio to set up the new anonymous rmap for.
 * @vma:	VM area to add the folio to.
 * @address:	User virtual address of the mapping
 * @exclusive:	Whether the folio is exclusive to the process.
 */
static void __folio_set_anon(struct folio *folio, struct vm_area_struct *vma,
			     unsigned long address, bool exclusive)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct anon_vma *anon_vma = vma->anon_vma;

	BUG_ON(!anon_vma);

<<<<<<< HEAD
	if (PageAnon(page))
		return;

	/*
	 * If the page isn't exclusively mapped into this vma,
	 * we must use the _oldest_ possible anon_vma for the
	 * page mapping!
=======
	/*
	 * If the folio isn't exclusive to this vma, we must use the _oldest_
	 * possible anon_vma for the folio mapping!
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	if (!exclusive)
		anon_vma = anon_vma->root;

<<<<<<< HEAD
	anon_vma = (void *) anon_vma + PAGE_MAPPING_ANON;
	page->mapping = (struct address_space *) anon_vma;
	page->index = linear_page_index(vma, address);
=======
	/*
	 * page_idle does a lockless/optimistic rmap scan on folio->mapping.
	 * Make sure the compiler doesn't split the stores of anon_vma and
	 * the PAGE_MAPPING_ANON type identifier, otherwise the rmap code
	 * could mistake the mapping for a struct address_space and crash.
	 */
	anon_vma = (void *) anon_vma + PAGE_MAPPING_ANON;
	WRITE_ONCE(folio->mapping, (struct address_space *) anon_vma);
	folio->index = linear_page_index(vma, address);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * __page_check_anon_rmap - sanity check anonymous rmap addition
<<<<<<< HEAD
 * @page:	the page to add the mapping to
 * @vma:	the vm area in which the mapping is added
 * @address:	the user virtual address mapped
 */
static void __page_check_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address)
{
#ifdef CONFIG_DEBUG_VM
=======
 * @folio:	The folio containing @page.
 * @page:	the page to check the mapping of
 * @vma:	the vm area in which the mapping is added
 * @address:	the user virtual address mapped
 */
static void __page_check_anon_rmap(struct folio *folio, struct page *page,
	struct vm_area_struct *vma, unsigned long address)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * The page's anon-rmap details (mapping and index) are guaranteed to
	 * be set up correctly at this point.
	 *
<<<<<<< HEAD
	 * We have exclusion against page_add_anon_rmap because the caller
	 * always holds the page locked, except if called from page_dup_rmap,
	 * in which case the page is already known to be setup.
	 *
	 * We have exclusion against page_add_new_anon_rmap because those pages
	 * are initially only visible via the pagetables, and the pte is locked
	 * over the call to page_add_new_anon_rmap.
	 */
	BUG_ON(page_anon_vma(page)->root != vma->anon_vma->root);
	BUG_ON(page->index != linear_page_index(vma, address));
=======
	 * We have exclusion against folio_add_anon_rmap_*() because the caller
	 * always holds the page locked.
	 *
	 * We have exclusion against folio_add_new_anon_rmap because those pages
	 * are initially only visible via the pagetables, and the pte is locked
	 * over the call to folio_add_new_anon_rmap.
	 */
	VM_BUG_ON_FOLIO(folio_anon_vma(folio)->root != vma->anon_vma->root,
			folio);
	VM_BUG_ON_PAGE(page_to_pgoff(page) != linear_page_index(vma, address),
		       page);
}

static __always_inline void __folio_add_anon_rmap(struct folio *folio,
		struct page *page, int nr_pages, struct vm_area_struct *vma,
		unsigned long address, rmap_t flags, enum rmap_level level)
{
	int i, nr, nr_pmdmapped = 0;

	nr = __folio_add_rmap(folio, page, nr_pages, level, &nr_pmdmapped);
	if (nr_pmdmapped)
		__lruvec_stat_mod_folio(folio, NR_ANON_THPS, nr_pmdmapped);
	if (nr)
		__lruvec_stat_mod_folio(folio, NR_ANON_MAPPED, nr);

	if (unlikely(!folio_test_anon(folio))) {
		VM_WARN_ON_FOLIO(!folio_test_locked(folio), folio);
		/*
		 * For a PTE-mapped large folio, we only know that the single
		 * PTE is exclusive. Further, __folio_set_anon() might not get
		 * folio->index right when not given the address of the head
		 * page.
		 */
		VM_WARN_ON_FOLIO(folio_test_large(folio) &&
				 level != RMAP_LEVEL_PMD, folio);
		__folio_set_anon(folio, vma, address,
				 !!(flags & RMAP_EXCLUSIVE));
	} else if (likely(!folio_test_ksm(folio))) {
		__page_check_anon_rmap(folio, page, vma, address);
	}

	if (flags & RMAP_EXCLUSIVE) {
		switch (level) {
		case RMAP_LEVEL_PTE:
			for (i = 0; i < nr_pages; i++)
				SetPageAnonExclusive(page + i);
			break;
		case RMAP_LEVEL_PMD:
			SetPageAnonExclusive(page);
			break;
		}
	}
	for (i = 0; i < nr_pages; i++) {
		struct page *cur_page = page + i;

		/* While PTE-mapping a THP we have a PMD and a PTE mapping. */
		VM_WARN_ON_FOLIO((atomic_read(&cur_page->_mapcount) > 0 ||
				  (folio_test_large(folio) &&
				   folio_entire_mapcount(folio) > 1)) &&
				 PageAnonExclusive(cur_page), folio);
	}

	/*
	 * For large folio, only mlock it if it's fully mapped to VMA. It's
	 * not easy to check whether the large folio is fully mapped to VMA
	 * here. Only mlock normal 4K folio and leave page reclaim to handle
	 * large folio.
	 */
	if (!folio_test_large(folio))
		mlock_vma_folio(folio, vma);
}

/**
 * folio_add_anon_rmap_ptes - add PTE mappings to a page range of an anon folio
 * @folio:	The folio to add the mappings to
 * @page:	The first page to add
 * @nr_pages:	The number of pages which will be mapped
 * @vma:	The vm area in which the mappings are added
 * @address:	The user virtual address of the first page to map
 * @flags:	The rmap flags
 *
 * The page range of folio is defined by [first_page, first_page + nr_pages)
 *
 * The caller needs to hold the page table lock, and the page must be locked in
 * the anon_vma case: to serialize mapping,index checking after setting,
 * and to ensure that an anon folio is not being upgraded racily to a KSM folio
 * (but KSM folios are never downgraded).
 */
void folio_add_anon_rmap_ptes(struct folio *folio, struct page *page,
		int nr_pages, struct vm_area_struct *vma, unsigned long address,
		rmap_t flags)
{
	__folio_add_anon_rmap(folio, page, nr_pages, vma, address, flags,
			      RMAP_LEVEL_PTE);
}

/**
 * folio_add_anon_rmap_pmd - add a PMD mapping to a page range of an anon folio
 * @folio:	The folio to add the mapping to
 * @page:	The first page to add
 * @vma:	The vm area in which the mapping is added
 * @address:	The user virtual address of the first page to map
 * @flags:	The rmap flags
 *
 * The page range of folio is defined by [first_page, first_page + HPAGE_PMD_NR)
 *
 * The caller needs to hold the page table lock, and the page must be locked in
 * the anon_vma case: to serialize mapping,index checking after setting.
 */
void folio_add_anon_rmap_pmd(struct folio *folio, struct page *page,
		struct vm_area_struct *vma, unsigned long address, rmap_t flags)
{
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	__folio_add_anon_rmap(folio, page, HPAGE_PMD_NR, vma, address, flags,
			      RMAP_LEVEL_PMD);
#else
	WARN_ON_ONCE(true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

/**
<<<<<<< HEAD
 * page_add_anon_rmap - add pte mapping to an anonymous page
 * @page:	the page to add the mapping to
 * @vma:	the vm area in which the mapping is added
 * @address:	the user virtual address mapped
 *
 * The caller needs to hold the pte lock, and the page must be locked in
 * the anon_vma case: to serialize mapping,index checking after setting,
 * and to ensure that PageAnon is not being upgraded racily to PageKsm
 * (but PageKsm is never downgraded to PageAnon).
 */
void page_add_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address)
{
	do_page_add_anon_rmap(page, vma, address, 0);
}

/*
 * Special version of the above for do_swap_page, which often runs
 * into pages that are exclusively owned by the current process.
 * Everybody else should continue to use page_add_anon_rmap above.
 */
void do_page_add_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address, int exclusive)
{
	int first = atomic_inc_and_test(&page->_mapcount);
	if (first) {
		if (!PageTransHuge(page))
			__inc_zone_page_state(page, NR_ANON_PAGES);
		else
			__inc_zone_page_state(page,
					      NR_ANON_TRANSPARENT_HUGEPAGES);
	}
	if (unlikely(PageKsm(page)))
		return;

	VM_BUG_ON(!PageLocked(page));
	/* address might be in next vma when migration races vma_adjust */
	if (first)
		__page_set_anon_rmap(page, vma, address, exclusive);
	else
		__page_check_anon_rmap(page, vma, address);
}

/**
 * page_add_new_anon_rmap - add pte mapping to a new anonymous page
 * @page:	the page to add the mapping to
 * @vma:	the vm area in which the mapping is added
 * @address:	the user virtual address mapped
 *
 * Same as page_add_anon_rmap but must only be called on *new* pages.
 * This means the inc-and-test can be bypassed.
 * Page does not have to be locked.
 */
void page_add_new_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address)
{
	VM_BUG_ON(address < vma->vm_start || address >= vma->vm_end);
	SetPageSwapBacked(page);
	atomic_set(&page->_mapcount, 0); /* increment count (starts at -1) */
	if (!PageTransHuge(page))
		__inc_zone_page_state(page, NR_ANON_PAGES);
	else
		__inc_zone_page_state(page, NR_ANON_TRANSPARENT_HUGEPAGES);
	__page_set_anon_rmap(page, vma, address, 1);
	if (page_evictable(page, vma))
		lru_cache_add_lru(page, LRU_ACTIVE_ANON);
	else
		add_page_to_unevictable_list(page);
}

/**
 * page_add_file_rmap - add pte mapping to a file page
 * @page: the page to add the mapping to
 *
 * The caller needs to hold the pte lock.
 */
void page_add_file_rmap(struct page *page)
{
	bool locked;
	unsigned long flags;

	mem_cgroup_begin_update_page_stat(page, &locked, &flags);
	if (atomic_inc_and_test(&page->_mapcount)) {
		__inc_zone_page_state(page, NR_FILE_MAPPED);
		mem_cgroup_inc_page_stat(page, MEMCG_NR_FILE_MAPPED);
	}
	mem_cgroup_end_update_page_stat(page, &locked, &flags);
}

/**
 * page_remove_rmap - take down pte mapping from a page
 * @page: page to remove mapping from
 *
 * The caller needs to hold the pte lock.
 */
void page_remove_rmap(struct page *page)
{
	struct address_space *mapping = page_mapping(page);
	bool anon = PageAnon(page);
	bool locked;
	unsigned long flags;

	/*
	 * The anon case has no mem_cgroup page_stat to update; but may
	 * uncharge_page() below, where the lock ordering can deadlock if
	 * we hold the lock against page_stat move: so avoid it on anon.
	 */
	if (!anon)
		mem_cgroup_begin_update_page_stat(page, &locked, &flags);

	/* page still mapped by someone else? */
	if (!atomic_add_negative(-1, &page->_mapcount))
		goto out;

	/*
	 * Now that the last pte has gone, s390 must transfer dirty
	 * flag from storage key to struct page.  We can usually skip
	 * this if the page is anon, so about to be freed; but perhaps
	 * not if it's in swapcache - there might be another pte slot
	 * containing the swap entry, but page not yet written to swap.
	 *
	 * And we can skip it on file pages, so long as the filesystem
	 * participates in dirty tracking; but need to catch shm and tmpfs
	 * and ramfs pages which have been modified since creation by read
	 * fault.
	 *
	 * Note that mapping must be decided above, before decrementing
	 * mapcount (which luckily provides a barrier): once page is unmapped,
	 * it could be truncated and page->mapping reset to NULL at any moment.
	 * Note also that we are relying on page_mapping(page) to set mapping
	 * to &swapper_space when PageSwapCache(page).
	 */
	if (mapping && !mapping_cap_account_dirty(mapping) &&
	    page_test_and_clear_dirty(page_to_pfn(page), 1))
		set_page_dirty(page);
	/*
	 * Hugepages are not counted in NR_ANON_PAGES nor NR_FILE_MAPPED
	 * and not charged by memcg for now.
	 */
	if (unlikely(PageHuge(page)))
		goto out;
	if (anon) {
		mem_cgroup_uncharge_page(page);
		if (!PageTransHuge(page))
			__dec_zone_page_state(page, NR_ANON_PAGES);
		else
			__dec_zone_page_state(page,
					      NR_ANON_TRANSPARENT_HUGEPAGES);
	} else {
		__dec_zone_page_state(page, NR_FILE_MAPPED);
		mem_cgroup_dec_page_stat(page, MEMCG_NR_FILE_MAPPED);
	}
	/*
	 * It would be tidy to reset the PageAnon mapping here,
	 * but that might overwrite a racing page_add_anon_rmap
	 * which increments mapcount after us but sets mapping
	 * before us: so leave the reset to free_hot_cold_page,
	 * and remember that it's only reliable while mapped.
	 * Leaving it set also helps swapoff to reinstate ptes
	 * faster for those pages still in swapcache.
	 */
out:
	if (!anon)
		mem_cgroup_end_update_page_stat(page, &locked, &flags);
}

/*
 * Subfunctions of try_to_unmap: try_to_unmap_one called
 * repeatedly from try_to_unmap_ksm, try_to_unmap_anon or try_to_unmap_file.
 */
int try_to_unmap_one(struct page *page, struct vm_area_struct *vma,
		     unsigned long address, enum ttu_flags flags)
{
	struct mm_struct *mm = vma->vm_mm;
	pte_t *pte;
	pte_t pteval;
	spinlock_t *ptl;
	int ret = SWAP_AGAIN;

	pte = page_check_address(page, mm, address, &ptl, 0);
	if (!pte)
		goto out;

	/*
	 * If the page is mlock()d, we cannot swap it out.
	 * If it's recently referenced (perhaps page_referenced
	 * skipped over this mm) then we should reactivate it.
	 */
	if (!(flags & TTU_IGNORE_MLOCK)) {
		if (vma->vm_flags & VM_LOCKED)
			goto out_mlock;

		if (TTU_ACTION(flags) == TTU_MUNLOCK)
			goto out_unmap;
	}
	if (!(flags & TTU_IGNORE_ACCESS)) {
		if (ptep_clear_flush_young_notify(vma, address, pte)) {
			ret = SWAP_FAIL;
			goto out_unmap;
		}
  	}

	/* Nuke the page table entry. */
	flush_cache_page(vma, address, page_to_pfn(page));
	pteval = ptep_clear_flush_notify(vma, address, pte);

	/* Move the dirty bit to the physical page now the pte is gone. */
	if (pte_dirty(pteval))
		set_page_dirty(page);

	/* Update high watermark before we lower rss */
	update_hiwater_rss(mm);

	if (PageHWPoison(page) && !(flags & TTU_IGNORE_HWPOISON)) {
		if (PageAnon(page))
			dec_mm_counter(mm, MM_ANONPAGES);
		else
			dec_mm_counter(mm, MM_FILEPAGES);
		set_pte_at(mm, address, pte,
				swp_entry_to_pte(make_hwpoison_entry(page)));
	} else if (PageAnon(page)) {
		swp_entry_t entry = { .val = page_private(page) };

		if (PageSwapCache(page)) {
=======
 * folio_add_new_anon_rmap - Add mapping to a new anonymous folio.
 * @folio:	The folio to add the mapping to.
 * @vma:	the vm area in which the mapping is added
 * @address:	the user virtual address mapped
 *
 * Like folio_add_anon_rmap_*() but must only be called on *new* folios.
 * This means the inc-and-test can be bypassed.
 * The folio does not have to be locked.
 *
 * If the folio is pmd-mappable, it is accounted as a THP.  As the folio
 * is new, it's assumed to be mapped exclusively by a single process.
 */
void folio_add_new_anon_rmap(struct folio *folio, struct vm_area_struct *vma,
		unsigned long address)
{
	int nr = folio_nr_pages(folio);

	VM_WARN_ON_FOLIO(folio_test_hugetlb(folio), folio);
	VM_BUG_ON_VMA(address < vma->vm_start ||
			address + (nr << PAGE_SHIFT) > vma->vm_end, vma);
	__folio_set_swapbacked(folio);
	__folio_set_anon(folio, vma, address, true);

	if (likely(!folio_test_large(folio))) {
		/* increment count (starts at -1) */
		atomic_set(&folio->_mapcount, 0);
		SetPageAnonExclusive(&folio->page);
	} else if (!folio_test_pmd_mappable(folio)) {
		int i;

		for (i = 0; i < nr; i++) {
			struct page *page = folio_page(folio, i);

			/* increment count (starts at -1) */
			atomic_set(&page->_mapcount, 0);
			SetPageAnonExclusive(page);
		}

		atomic_set(&folio->_nr_pages_mapped, nr);
	} else {
		/* increment count (starts at -1) */
		atomic_set(&folio->_entire_mapcount, 0);
		atomic_set(&folio->_nr_pages_mapped, ENTIRELY_MAPPED);
		SetPageAnonExclusive(&folio->page);
		__lruvec_stat_mod_folio(folio, NR_ANON_THPS, nr);
	}

	__lruvec_stat_mod_folio(folio, NR_ANON_MAPPED, nr);
}

static __always_inline void __folio_add_file_rmap(struct folio *folio,
		struct page *page, int nr_pages, struct vm_area_struct *vma,
		enum rmap_level level)
{
	int nr, nr_pmdmapped = 0;

	VM_WARN_ON_FOLIO(folio_test_anon(folio), folio);

	nr = __folio_add_rmap(folio, page, nr_pages, level, &nr_pmdmapped);
	if (nr_pmdmapped)
		__lruvec_stat_mod_folio(folio, folio_test_swapbacked(folio) ?
			NR_SHMEM_PMDMAPPED : NR_FILE_PMDMAPPED, nr_pmdmapped);
	if (nr)
		__lruvec_stat_mod_folio(folio, NR_FILE_MAPPED, nr);

	/* See comments in folio_add_anon_rmap_*() */
	if (!folio_test_large(folio))
		mlock_vma_folio(folio, vma);
}

/**
 * folio_add_file_rmap_ptes - add PTE mappings to a page range of a folio
 * @folio:	The folio to add the mappings to
 * @page:	The first page to add
 * @nr_pages:	The number of pages that will be mapped using PTEs
 * @vma:	The vm area in which the mappings are added
 *
 * The page range of the folio is defined by [page, page + nr_pages)
 *
 * The caller needs to hold the page table lock.
 */
void folio_add_file_rmap_ptes(struct folio *folio, struct page *page,
		int nr_pages, struct vm_area_struct *vma)
{
	__folio_add_file_rmap(folio, page, nr_pages, vma, RMAP_LEVEL_PTE);
}

/**
 * folio_add_file_rmap_pmd - add a PMD mapping to a page range of a folio
 * @folio:	The folio to add the mapping to
 * @page:	The first page to add
 * @vma:	The vm area in which the mapping is added
 *
 * The page range of the folio is defined by [page, page + HPAGE_PMD_NR)
 *
 * The caller needs to hold the page table lock.
 */
void folio_add_file_rmap_pmd(struct folio *folio, struct page *page,
		struct vm_area_struct *vma)
{
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	__folio_add_file_rmap(folio, page, HPAGE_PMD_NR, vma, RMAP_LEVEL_PMD);
#else
	WARN_ON_ONCE(true);
#endif
}

static __always_inline void __folio_remove_rmap(struct folio *folio,
		struct page *page, int nr_pages, struct vm_area_struct *vma,
		enum rmap_level level)
{
	atomic_t *mapped = &folio->_nr_pages_mapped;
	int last, nr = 0, nr_pmdmapped = 0;
	enum node_stat_item idx;

	__folio_rmap_sanity_checks(folio, page, nr_pages, level);

	switch (level) {
	case RMAP_LEVEL_PTE:
		do {
			last = atomic_add_negative(-1, &page->_mapcount);
			if (last && folio_test_large(folio)) {
				last = atomic_dec_return_relaxed(mapped);
				last = (last < ENTIRELY_MAPPED);
			}

			if (last)
				nr++;
		} while (page++, --nr_pages > 0);
		break;
	case RMAP_LEVEL_PMD:
		last = atomic_add_negative(-1, &folio->_entire_mapcount);
		if (last) {
			nr = atomic_sub_return_relaxed(ENTIRELY_MAPPED, mapped);
			if (likely(nr < ENTIRELY_MAPPED)) {
				nr_pmdmapped = folio_nr_pages(folio);
				nr = nr_pmdmapped - (nr & FOLIO_PAGES_MAPPED);
				/* Raced ahead of another remove and an add? */
				if (unlikely(nr < 0))
					nr = 0;
			} else {
				/* An add of ENTIRELY_MAPPED raced ahead */
				nr = 0;
			}
		}
		break;
	}

	if (nr_pmdmapped) {
		if (folio_test_anon(folio))
			idx = NR_ANON_THPS;
		else if (folio_test_swapbacked(folio))
			idx = NR_SHMEM_PMDMAPPED;
		else
			idx = NR_FILE_PMDMAPPED;
		__lruvec_stat_mod_folio(folio, idx, -nr_pmdmapped);
	}
	if (nr) {
		idx = folio_test_anon(folio) ? NR_ANON_MAPPED : NR_FILE_MAPPED;
		__lruvec_stat_mod_folio(folio, idx, -nr);

		/*
		 * Queue anon large folio for deferred split if at least one
		 * page of the folio is unmapped and at least one page
		 * is still mapped.
		 */
		if (folio_test_large(folio) && folio_test_anon(folio))
			if (level == RMAP_LEVEL_PTE || nr < nr_pmdmapped)
				deferred_split_folio(folio);
	}

	/*
	 * It would be tidy to reset folio_test_anon mapping when fully
	 * unmapped, but that might overwrite a racing folio_add_anon_rmap_*()
	 * which increments mapcount after us but sets mapping before us:
	 * so leave the reset to free_pages_prepare, and remember that
	 * it's only reliable while mapped.
	 */

	munlock_vma_folio(folio, vma);
}

/**
 * folio_remove_rmap_ptes - remove PTE mappings from a page range of a folio
 * @folio:	The folio to remove the mappings from
 * @page:	The first page to remove
 * @nr_pages:	The number of pages that will be removed from the mapping
 * @vma:	The vm area from which the mappings are removed
 *
 * The page range of the folio is defined by [page, page + nr_pages)
 *
 * The caller needs to hold the page table lock.
 */
void folio_remove_rmap_ptes(struct folio *folio, struct page *page,
		int nr_pages, struct vm_area_struct *vma)
{
	__folio_remove_rmap(folio, page, nr_pages, vma, RMAP_LEVEL_PTE);
}

/**
 * folio_remove_rmap_pmd - remove a PMD mapping from a page range of a folio
 * @folio:	The folio to remove the mapping from
 * @page:	The first page to remove
 * @vma:	The vm area from which the mapping is removed
 *
 * The page range of the folio is defined by [page, page + HPAGE_PMD_NR)
 *
 * The caller needs to hold the page table lock.
 */
void folio_remove_rmap_pmd(struct folio *folio, struct page *page,
		struct vm_area_struct *vma)
{
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	__folio_remove_rmap(folio, page, HPAGE_PMD_NR, vma, RMAP_LEVEL_PMD);
#else
	WARN_ON_ONCE(true);
#endif
}

/*
 * @arg: enum ttu_flags will be passed to this argument
 */
static bool try_to_unmap_one(struct folio *folio, struct vm_area_struct *vma,
		     unsigned long address, void *arg)
{
	struct mm_struct *mm = vma->vm_mm;
	DEFINE_FOLIO_VMA_WALK(pvmw, folio, vma, address, 0);
	pte_t pteval;
	struct page *subpage;
	bool anon_exclusive, ret = true;
	struct mmu_notifier_range range;
	enum ttu_flags flags = (enum ttu_flags)(long)arg;
	unsigned long pfn;
	unsigned long hsz = 0;

	/*
	 * When racing against e.g. zap_pte_range() on another cpu,
	 * in between its ptep_get_and_clear_full() and folio_remove_rmap_*(),
	 * try_to_unmap() may return before page_mapped() has become false,
	 * if page table locking is skipped: use TTU_SYNC to wait for that.
	 */
	if (flags & TTU_SYNC)
		pvmw.flags = PVMW_SYNC;

	if (flags & TTU_SPLIT_HUGE_PMD)
		split_huge_pmd_address(vma, address, false, folio);

	/*
	 * For THP, we have to assume the worse case ie pmd for invalidation.
	 * For hugetlb, it could be much worse if we need to do pud
	 * invalidation in the case of pmd sharing.
	 *
	 * Note that the folio can not be freed in this function as call of
	 * try_to_unmap() must hold a reference on the folio.
	 */
	range.end = vma_address_end(&pvmw);
	mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, vma->vm_mm,
				address, range.end);
	if (folio_test_hugetlb(folio)) {
		/*
		 * If sharing is possible, start and end will be adjusted
		 * accordingly.
		 */
		adjust_range_if_pmd_sharing_possible(vma, &range.start,
						     &range.end);

		/* We need the huge page size for set_huge_pte_at() */
		hsz = huge_page_size(hstate_vma(vma));
	}
	mmu_notifier_invalidate_range_start(&range);

	while (page_vma_mapped_walk(&pvmw)) {
		/* Unexpected PMD-mapped THP? */
		VM_BUG_ON_FOLIO(!pvmw.pte, folio);

		/*
		 * If the folio is in an mlock()d vma, we must not swap it out.
		 */
		if (!(flags & TTU_IGNORE_MLOCK) &&
		    (vma->vm_flags & VM_LOCKED)) {
			/* Restore the mlock which got missed */
			if (!folio_test_large(folio))
				mlock_vma_folio(folio, vma);
			page_vma_mapped_walk_done(&pvmw);
			ret = false;
			break;
		}

		pfn = pte_pfn(ptep_get(pvmw.pte));
		subpage = folio_page(folio, pfn - folio_pfn(folio));
		address = pvmw.address;
		anon_exclusive = folio_test_anon(folio) &&
				 PageAnonExclusive(subpage);

		if (folio_test_hugetlb(folio)) {
			bool anon = folio_test_anon(folio);

			/*
			 * The try_to_unmap() is only passed a hugetlb page
			 * in the case where the hugetlb page is poisoned.
			 */
			VM_BUG_ON_PAGE(!PageHWPoison(subpage), subpage);
			/*
			 * huge_pmd_unshare may unmap an entire PMD page.
			 * There is no way of knowing exactly which PMDs may
			 * be cached for this mm, so we must flush them all.
			 * start/end were already adjusted above to cover this
			 * range.
			 */
			flush_cache_range(vma, range.start, range.end);

			/*
			 * To call huge_pmd_unshare, i_mmap_rwsem must be
			 * held in write mode.  Caller needs to explicitly
			 * do this outside rmap routines.
			 *
			 * We also must hold hugetlb vma_lock in write mode.
			 * Lock order dictates acquiring vma_lock BEFORE
			 * i_mmap_rwsem.  We can only try lock here and fail
			 * if unsuccessful.
			 */
			if (!anon) {
				VM_BUG_ON(!(flags & TTU_RMAP_LOCKED));
				if (!hugetlb_vma_trylock_write(vma)) {
					page_vma_mapped_walk_done(&pvmw);
					ret = false;
					break;
				}
				if (huge_pmd_unshare(mm, vma, address, pvmw.pte)) {
					hugetlb_vma_unlock_write(vma);
					flush_tlb_range(vma,
						range.start, range.end);
					/*
					 * The ref count of the PMD page was
					 * dropped which is part of the way map
					 * counting is done for shared PMDs.
					 * Return 'true' here.  When there is
					 * no other sharing, huge_pmd_unshare
					 * returns false and we will unmap the
					 * actual page and drop map count
					 * to zero.
					 */
					page_vma_mapped_walk_done(&pvmw);
					break;
				}
				hugetlb_vma_unlock_write(vma);
			}
			pteval = huge_ptep_clear_flush(vma, address, pvmw.pte);
		} else {
			flush_cache_page(vma, address, pfn);
			/* Nuke the page table entry. */
			if (should_defer_flush(mm, flags)) {
				/*
				 * We clear the PTE but do not flush so potentially
				 * a remote CPU could still be writing to the folio.
				 * If the entry was previously clean then the
				 * architecture must guarantee that a clear->dirty
				 * transition on a cached TLB entry is written through
				 * and traps if the PTE is unmapped.
				 */
				pteval = ptep_get_and_clear(mm, address, pvmw.pte);

				set_tlb_ubc_flush_pending(mm, pteval, address);
			} else {
				pteval = ptep_clear_flush(vma, address, pvmw.pte);
			}
		}

		/*
		 * Now the pte is cleared. If this pte was uffd-wp armed,
		 * we may want to replace a none pte with a marker pte if
		 * it's file-backed, so we don't lose the tracking info.
		 */
		pte_install_uffd_wp_if_needed(vma, address, pvmw.pte, pteval);

		/* Set the dirty flag on the folio now the pte is gone. */
		if (pte_dirty(pteval))
			folio_mark_dirty(folio);

		/* Update high watermark before we lower rss */
		update_hiwater_rss(mm);

		if (PageHWPoison(subpage) && (flags & TTU_HWPOISON)) {
			pteval = swp_entry_to_pte(make_hwpoison_entry(subpage));
			if (folio_test_hugetlb(folio)) {
				hugetlb_count_sub(folio_nr_pages(folio), mm);
				set_huge_pte_at(mm, address, pvmw.pte, pteval,
						hsz);
			} else {
				dec_mm_counter(mm, mm_counter(folio));
				set_pte_at(mm, address, pvmw.pte, pteval);
			}

		} else if (pte_unused(pteval) && !userfaultfd_armed(vma)) {
			/*
			 * The guest indicated that the page content is of no
			 * interest anymore. Simply discard the pte, vmscan
			 * will take care of the rest.
			 * A future reference will then fault in a new zero
			 * page. When userfaultfd is active, we must not drop
			 * this page though, as its main user (postcopy
			 * migration) will not expect userfaults on already
			 * copied pages.
			 */
			dec_mm_counter(mm, mm_counter(folio));
		} else if (folio_test_anon(folio)) {
			swp_entry_t entry = page_swap_entry(subpage);
			pte_t swp_pte;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Store the swap location in the pte.
			 * See handle_pte_fault() ...
			 */
<<<<<<< HEAD
			if (swap_duplicate(entry) < 0) {
				set_pte_at(mm, address, pte, pteval);
				ret = SWAP_FAIL;
				goto out_unmap;
=======
			if (unlikely(folio_test_swapbacked(folio) !=
					folio_test_swapcache(folio))) {
				WARN_ON_ONCE(1);
				ret = false;
				page_vma_mapped_walk_done(&pvmw);
				break;
			}

			/* MADV_FREE page check */
			if (!folio_test_swapbacked(folio)) {
				int ref_count, map_count;

				/*
				 * Synchronize with gup_pte_range():
				 * - clear PTE; barrier; read refcount
				 * - inc refcount; barrier; read PTE
				 */
				smp_mb();

				ref_count = folio_ref_count(folio);
				map_count = folio_mapcount(folio);

				/*
				 * Order reads for page refcount and dirty flag
				 * (see comments in __remove_mapping()).
				 */
				smp_rmb();

				/*
				 * The only page refs must be one from isolation
				 * plus the rmap(s) (dropped by discard:).
				 */
				if (ref_count == 1 + map_count &&
				    !folio_test_dirty(folio)) {
					dec_mm_counter(mm, MM_ANONPAGES);
					goto discard;
				}

				/*
				 * If the folio was redirtied, it cannot be
				 * discarded. Remap the page to page table.
				 */
				set_pte_at(mm, address, pvmw.pte, pteval);
				folio_set_swapbacked(folio);
				ret = false;
				page_vma_mapped_walk_done(&pvmw);
				break;
			}

			if (swap_duplicate(entry) < 0) {
				set_pte_at(mm, address, pvmw.pte, pteval);
				ret = false;
				page_vma_mapped_walk_done(&pvmw);
				break;
			}
			if (arch_unmap_one(mm, vma, address, pteval) < 0) {
				swap_free(entry);
				set_pte_at(mm, address, pvmw.pte, pteval);
				ret = false;
				page_vma_mapped_walk_done(&pvmw);
				break;
			}

			/* See folio_try_share_anon_rmap(): clear PTE first. */
			if (anon_exclusive &&
			    folio_try_share_anon_rmap_pte(folio, subpage)) {
				swap_free(entry);
				set_pte_at(mm, address, pvmw.pte, pteval);
				ret = false;
				page_vma_mapped_walk_done(&pvmw);
				break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			if (list_empty(&mm->mmlist)) {
				spin_lock(&mmlist_lock);
				if (list_empty(&mm->mmlist))
					list_add(&mm->mmlist, &init_mm.mmlist);
				spin_unlock(&mmlist_lock);
			}
			dec_mm_counter(mm, MM_ANONPAGES);
			inc_mm_counter(mm, MM_SWAPENTS);
<<<<<<< HEAD
		} else if (IS_ENABLED(CONFIG_MIGRATION)) {
=======
			swp_pte = swp_entry_to_pte(entry);
			if (anon_exclusive)
				swp_pte = pte_swp_mkexclusive(swp_pte);
			if (pte_soft_dirty(pteval))
				swp_pte = pte_swp_mksoft_dirty(swp_pte);
			if (pte_uffd_wp(pteval))
				swp_pte = pte_swp_mkuffd_wp(swp_pte);
			set_pte_at(mm, address, pvmw.pte, swp_pte);
		} else {
			/*
			 * This is a locked file-backed folio,
			 * so it cannot be removed from the page
			 * cache and replaced by a new folio before
			 * mmu_notifier_invalidate_range_end, so no
			 * concurrent thread might update its page table
			 * to point at a new folio while a device is
			 * still using this folio.
			 *
			 * See Documentation/mm/mmu_notifier.rst
			 */
			dec_mm_counter(mm, mm_counter_file(folio));
		}
discard:
		if (unlikely(folio_test_hugetlb(folio)))
			hugetlb_remove_rmap(folio);
		else
			folio_remove_rmap_pte(folio, subpage, vma);
		if (vma->vm_flags & VM_LOCKED)
			mlock_drain_local();
		folio_put(folio);
	}

	mmu_notifier_invalidate_range_end(&range);

	return ret;
}

static bool invalid_migration_vma(struct vm_area_struct *vma, void *arg)
{
	return vma_is_temporary_stack(vma);
}

static int folio_not_mapped(struct folio *folio)
{
	return !folio_mapped(folio);
}

/**
 * try_to_unmap - Try to remove all page table mappings to a folio.
 * @folio: The folio to unmap.
 * @flags: action and flags
 *
 * Tries to remove all the page table entries which are mapping this
 * folio.  It is the caller's responsibility to check if the folio is
 * still mapped if needed (use TTU_SYNC to prevent accounting races).
 *
 * Context: Caller must hold the folio lock.
 */
void try_to_unmap(struct folio *folio, enum ttu_flags flags)
{
	struct rmap_walk_control rwc = {
		.rmap_one = try_to_unmap_one,
		.arg = (void *)flags,
		.done = folio_not_mapped,
		.anon_lock = folio_lock_anon_vma_read,
	};

	if (flags & TTU_RMAP_LOCKED)
		rmap_walk_locked(folio, &rwc);
	else
		rmap_walk(folio, &rwc);
}

/*
 * @arg: enum ttu_flags will be passed to this argument.
 *
 * If TTU_SPLIT_HUGE_PMD is specified any PMD mappings will be split into PTEs
 * containing migration entries.
 */
static bool try_to_migrate_one(struct folio *folio, struct vm_area_struct *vma,
		     unsigned long address, void *arg)
{
	struct mm_struct *mm = vma->vm_mm;
	DEFINE_FOLIO_VMA_WALK(pvmw, folio, vma, address, 0);
	pte_t pteval;
	struct page *subpage;
	bool anon_exclusive, ret = true;
	struct mmu_notifier_range range;
	enum ttu_flags flags = (enum ttu_flags)(long)arg;
	unsigned long pfn;
	unsigned long hsz = 0;

	/*
	 * When racing against e.g. zap_pte_range() on another cpu,
	 * in between its ptep_get_and_clear_full() and folio_remove_rmap_*(),
	 * try_to_migrate() may return before page_mapped() has become false,
	 * if page table locking is skipped: use TTU_SYNC to wait for that.
	 */
	if (flags & TTU_SYNC)
		pvmw.flags = PVMW_SYNC;

	/*
	 * unmap_page() in mm/huge_memory.c is the only user of migration with
	 * TTU_SPLIT_HUGE_PMD and it wants to freeze.
	 */
	if (flags & TTU_SPLIT_HUGE_PMD)
		split_huge_pmd_address(vma, address, true, folio);

	/*
	 * For THP, we have to assume the worse case ie pmd for invalidation.
	 * For hugetlb, it could be much worse if we need to do pud
	 * invalidation in the case of pmd sharing.
	 *
	 * Note that the page can not be free in this function as call of
	 * try_to_unmap() must hold a reference on the page.
	 */
	range.end = vma_address_end(&pvmw);
	mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, vma->vm_mm,
				address, range.end);
	if (folio_test_hugetlb(folio)) {
		/*
		 * If sharing is possible, start and end will be adjusted
		 * accordingly.
		 */
		adjust_range_if_pmd_sharing_possible(vma, &range.start,
						     &range.end);

		/* We need the huge page size for set_huge_pte_at() */
		hsz = huge_page_size(hstate_vma(vma));
	}
	mmu_notifier_invalidate_range_start(&range);

	while (page_vma_mapped_walk(&pvmw)) {
#ifdef CONFIG_ARCH_ENABLE_THP_MIGRATION
		/* PMD-mapped THP migration entry */
		if (!pvmw.pte) {
			subpage = folio_page(folio,
				pmd_pfn(*pvmw.pmd) - folio_pfn(folio));
			VM_BUG_ON_FOLIO(folio_test_hugetlb(folio) ||
					!folio_test_pmd_mappable(folio), folio);

			if (set_pmd_migration_entry(&pvmw, subpage)) {
				ret = false;
				page_vma_mapped_walk_done(&pvmw);
				break;
			}
			continue;
		}
#endif

		/* Unexpected PMD-mapped THP? */
		VM_BUG_ON_FOLIO(!pvmw.pte, folio);

		pfn = pte_pfn(ptep_get(pvmw.pte));

		if (folio_is_zone_device(folio)) {
			/*
			 * Our PTE is a non-present device exclusive entry and
			 * calculating the subpage as for the common case would
			 * result in an invalid pointer.
			 *
			 * Since only PAGE_SIZE pages can currently be
			 * migrated, just set it to page. This will need to be
			 * changed when hugepage migrations to device private
			 * memory are supported.
			 */
			VM_BUG_ON_FOLIO(folio_nr_pages(folio) > 1, folio);
			subpage = &folio->page;
		} else {
			subpage = folio_page(folio, pfn - folio_pfn(folio));
		}
		address = pvmw.address;
		anon_exclusive = folio_test_anon(folio) &&
				 PageAnonExclusive(subpage);

		if (folio_test_hugetlb(folio)) {
			bool anon = folio_test_anon(folio);

			/*
			 * huge_pmd_unshare may unmap an entire PMD page.
			 * There is no way of knowing exactly which PMDs may
			 * be cached for this mm, so we must flush them all.
			 * start/end were already adjusted above to cover this
			 * range.
			 */
			flush_cache_range(vma, range.start, range.end);

			/*
			 * To call huge_pmd_unshare, i_mmap_rwsem must be
			 * held in write mode.  Caller needs to explicitly
			 * do this outside rmap routines.
			 *
			 * We also must hold hugetlb vma_lock in write mode.
			 * Lock order dictates acquiring vma_lock BEFORE
			 * i_mmap_rwsem.  We can only try lock here and
			 * fail if unsuccessful.
			 */
			if (!anon) {
				VM_BUG_ON(!(flags & TTU_RMAP_LOCKED));
				if (!hugetlb_vma_trylock_write(vma)) {
					page_vma_mapped_walk_done(&pvmw);
					ret = false;
					break;
				}
				if (huge_pmd_unshare(mm, vma, address, pvmw.pte)) {
					hugetlb_vma_unlock_write(vma);
					flush_tlb_range(vma,
						range.start, range.end);

					/*
					 * The ref count of the PMD page was
					 * dropped which is part of the way map
					 * counting is done for shared PMDs.
					 * Return 'true' here.  When there is
					 * no other sharing, huge_pmd_unshare
					 * returns false and we will unmap the
					 * actual page and drop map count
					 * to zero.
					 */
					page_vma_mapped_walk_done(&pvmw);
					break;
				}
				hugetlb_vma_unlock_write(vma);
			}
			/* Nuke the hugetlb page table entry */
			pteval = huge_ptep_clear_flush(vma, address, pvmw.pte);
		} else {
			flush_cache_page(vma, address, pfn);
			/* Nuke the page table entry. */
			if (should_defer_flush(mm, flags)) {
				/*
				 * We clear the PTE but do not flush so potentially
				 * a remote CPU could still be writing to the folio.
				 * If the entry was previously clean then the
				 * architecture must guarantee that a clear->dirty
				 * transition on a cached TLB entry is written through
				 * and traps if the PTE is unmapped.
				 */
				pteval = ptep_get_and_clear(mm, address, pvmw.pte);

				set_tlb_ubc_flush_pending(mm, pteval, address);
			} else {
				pteval = ptep_clear_flush(vma, address, pvmw.pte);
			}
		}

		/* Set the dirty flag on the folio now the pte is gone. */
		if (pte_dirty(pteval))
			folio_mark_dirty(folio);

		/* Update high watermark before we lower rss */
		update_hiwater_rss(mm);

		if (folio_is_device_private(folio)) {
			unsigned long pfn = folio_pfn(folio);
			swp_entry_t entry;
			pte_t swp_pte;

			if (anon_exclusive)
				WARN_ON_ONCE(folio_try_share_anon_rmap_pte(folio,
									   subpage));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Store the pfn of the page in a special migration
			 * pte. do_swap_page() will wait until the migration
			 * pte is removed and then restart fault handling.
			 */
<<<<<<< HEAD
			BUG_ON(TTU_ACTION(flags) != TTU_MIGRATION);
			entry = make_migration_entry(page, pte_write(pteval));
		}
		set_pte_at(mm, address, pte, swp_entry_to_pte(entry));
		BUG_ON(pte_file(*pte));
	} else if (IS_ENABLED(CONFIG_MIGRATION) &&
		   (TTU_ACTION(flags) == TTU_MIGRATION)) {
		/* Establish migration entry for a file page */
		swp_entry_t entry;
		entry = make_migration_entry(page, pte_write(pteval));
		set_pte_at(mm, address, pte, swp_entry_to_pte(entry));
	} else
		dec_mm_counter(mm, MM_FILEPAGES);

	page_remove_rmap(page);
	page_cache_release(page);

out_unmap:
	pte_unmap_unlock(pte, ptl);
out:
	return ret;

out_mlock:
	pte_unmap_unlock(pte, ptl);


	/*
	 * We need mmap_sem locking, Otherwise VM_LOCKED check makes
	 * unstable result and race. Plus, We can't wait here because
	 * we now hold anon_vma->mutex or mapping->i_mmap_mutex.
	 * if trylock failed, the page remain in evictable lru and later
	 * vmscan could retry to move the page to unevictable lru if the
	 * page is actually mlocked.
	 */
	if (down_read_trylock(&vma->vm_mm->mmap_sem)) {
		if (vma->vm_flags & VM_LOCKED) {
			mlock_vma_page(page);
			ret = SWAP_MLOCK;
		}
		up_read(&vma->vm_mm->mmap_sem);
	}
	return ret;
}

/*
 * objrmap doesn't work for nonlinear VMAs because the assumption that
 * offset-into-file correlates with offset-into-virtual-addresses does not hold.
 * Consequently, given a particular page and its ->index, we cannot locate the
 * ptes which are mapping that page without an exhaustive linear search.
 *
 * So what this code does is a mini "virtual scan" of each nonlinear VMA which
 * maps the file to which the target page belongs.  The ->vm_private_data field
 * holds the current cursor into that scan.  Successive searches will circulate
 * around the vma's virtual address space.
 *
 * So as more replacement pressure is applied to the pages in a nonlinear VMA,
 * more scanning pressure is placed against them as well.   Eventually pages
 * will become fully unmapped and are eligible for eviction.
 *
 * For very sparsely populated VMAs this is a little inefficient - chances are
 * there there won't be many ptes located within the scan cluster.  In this case
 * maybe we could scan further - to the end of the pte page, perhaps.
 *
 * Mlocked pages:  check VM_LOCKED under mmap_sem held for read, if we can
 * acquire it without blocking.  If vma locked, mlock the pages in the cluster,
 * rather than unmapping them.  If we encounter the "check_page" that vmscan is
 * trying to unmap, return SWAP_MLOCK, else default SWAP_AGAIN.
 */
#define CLUSTER_SIZE	min(32*PAGE_SIZE, PMD_SIZE)
#define CLUSTER_MASK	(~(CLUSTER_SIZE - 1))

static int try_to_unmap_cluster(unsigned long cursor, unsigned int *mapcount,
		struct vm_area_struct *vma, struct page *check_page)
{
	struct mm_struct *mm = vma->vm_mm;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	pte_t pteval;
	spinlock_t *ptl;
	struct page *page;
	unsigned long address;
	unsigned long end;
	int ret = SWAP_AGAIN;
	int locked_vma = 0;

	address = (vma->vm_start + cursor) & CLUSTER_MASK;
	end = address + CLUSTER_SIZE;
	if (address < vma->vm_start)
		address = vma->vm_start;
	if (end > vma->vm_end)
		end = vma->vm_end;

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		return ret;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		return ret;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd))
		return ret;

	/*
	 * If we can acquire the mmap_sem for read, and vma is VM_LOCKED,
	 * keep the sem while scanning the cluster for mlocking pages.
	 */
	if (down_read_trylock(&vma->vm_mm->mmap_sem)) {
		locked_vma = (vma->vm_flags & VM_LOCKED);
		if (!locked_vma)
			up_read(&vma->vm_mm->mmap_sem); /* don't need it */
	}

	pte = pte_offset_map_lock(mm, pmd, address, &ptl);

	/* Update high watermark before we lower rss */
	update_hiwater_rss(mm);

	for (; address < end; pte++, address += PAGE_SIZE) {
		if (!pte_present(*pte))
			continue;
		page = vm_normal_page(vma, address, *pte);
		BUG_ON(!page || PageAnon(page));

		if (locked_vma) {
			if (page == check_page) {
				/* we know we have check_page locked */
				mlock_vma_page(page);
				ret = SWAP_MLOCK;
			} else if (trylock_page(page)) {
				/*
				 * If we can lock the page, perform mlock.
				 * Otherwise leave the page alone, it will be
				 * eventually encountered again later.
				 */
				mlock_vma_page(page);
				unlock_page(page);
			}
			continue;	/* don't unmap */
		}

		if (ptep_clear_flush_young_notify(vma, address, pte))
			continue;

		/* Nuke the page table entry. */
		flush_cache_page(vma, address, pte_pfn(*pte));
		pteval = ptep_clear_flush_notify(vma, address, pte);

		/* If nonlinear, store the file page offset in the pte. */
		if (page->index != linear_page_index(vma, address))
			set_pte_at(mm, address, pte, pgoff_to_pte(page->index));

		/* Move the dirty bit to the physical page now the pte is gone. */
		if (pte_dirty(pteval))
			set_page_dirty(page);

		page_remove_rmap(page);
		page_cache_release(page);
		dec_mm_counter(mm, MM_FILEPAGES);
		(*mapcount)--;
	}
	pte_unmap_unlock(pte - 1, ptl);
	if (locked_vma)
		up_read(&vma->vm_mm->mmap_sem);
	return ret;
}

bool is_vma_temporary_stack(struct vm_area_struct *vma)
{
	int maybe_stack = vma->vm_flags & (VM_GROWSDOWN | VM_GROWSUP);

	if (!maybe_stack)
		return false;

	if ((vma->vm_flags & VM_STACK_INCOMPLETE_SETUP) ==
						VM_STACK_INCOMPLETE_SETUP)
		return true;

	return false;
}

/**
 * try_to_unmap_anon - unmap or unlock anonymous page using the object-based
 * rmap method
 * @page: the page to unmap/unlock
 * @flags: action and flags
 *
 * Find all the mappings of a page using the mapping pointer and the vma chains
 * contained in the anon_vma struct it points to.
 *
 * This function is only called from try_to_unmap/try_to_munlock for
 * anonymous pages.
 * When called from try_to_munlock(), the mmap_sem of the mm containing the vma
 * where the page was found will be held for write.  So, we won't recheck
 * vm_flags for that VMA.  That should be OK, because that vma shouldn't be
 * 'LOCKED.
 */
static int try_to_unmap_anon(struct page *page, enum ttu_flags flags)
{
	struct anon_vma *anon_vma;
	struct anon_vma_chain *avc;
	int ret = SWAP_AGAIN;

	anon_vma = page_lock_anon_vma(page);
	if (!anon_vma)
		return ret;

	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long address;

		/*
		 * During exec, a temporary VMA is setup and later moved.
		 * The VMA is moved under the anon_vma lock but not the
		 * page tables leading to a race where migration cannot
		 * find the migration ptes. Rather than increasing the
		 * locking requirements of exec(), migration skips
		 * temporary VMAs until after exec() completes.
		 */
		if (IS_ENABLED(CONFIG_MIGRATION) && (flags & TTU_MIGRATION) &&
				is_vma_temporary_stack(vma))
			continue;

		address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		ret = try_to_unmap_one(page, vma, address, flags);
		if (ret != SWAP_AGAIN || !page_mapped(page))
			break;
	}

	page_unlock_anon_vma(anon_vma);
=======
			entry = pte_to_swp_entry(pteval);
			if (is_writable_device_private_entry(entry))
				entry = make_writable_migration_entry(pfn);
			else if (anon_exclusive)
				entry = make_readable_exclusive_migration_entry(pfn);
			else
				entry = make_readable_migration_entry(pfn);
			swp_pte = swp_entry_to_pte(entry);

			/*
			 * pteval maps a zone device page and is therefore
			 * a swap pte.
			 */
			if (pte_swp_soft_dirty(pteval))
				swp_pte = pte_swp_mksoft_dirty(swp_pte);
			if (pte_swp_uffd_wp(pteval))
				swp_pte = pte_swp_mkuffd_wp(swp_pte);
			set_pte_at(mm, pvmw.address, pvmw.pte, swp_pte);
			trace_set_migration_pte(pvmw.address, pte_val(swp_pte),
						folio_order(folio));
			/*
			 * No need to invalidate here it will synchronize on
			 * against the special swap migration pte.
			 */
		} else if (PageHWPoison(subpage)) {
			pteval = swp_entry_to_pte(make_hwpoison_entry(subpage));
			if (folio_test_hugetlb(folio)) {
				hugetlb_count_sub(folio_nr_pages(folio), mm);
				set_huge_pte_at(mm, address, pvmw.pte, pteval,
						hsz);
			} else {
				dec_mm_counter(mm, mm_counter(folio));
				set_pte_at(mm, address, pvmw.pte, pteval);
			}

		} else if (pte_unused(pteval) && !userfaultfd_armed(vma)) {
			/*
			 * The guest indicated that the page content is of no
			 * interest anymore. Simply discard the pte, vmscan
			 * will take care of the rest.
			 * A future reference will then fault in a new zero
			 * page. When userfaultfd is active, we must not drop
			 * this page though, as its main user (postcopy
			 * migration) will not expect userfaults on already
			 * copied pages.
			 */
			dec_mm_counter(mm, mm_counter(folio));
		} else {
			swp_entry_t entry;
			pte_t swp_pte;

			if (arch_unmap_one(mm, vma, address, pteval) < 0) {
				if (folio_test_hugetlb(folio))
					set_huge_pte_at(mm, address, pvmw.pte,
							pteval, hsz);
				else
					set_pte_at(mm, address, pvmw.pte, pteval);
				ret = false;
				page_vma_mapped_walk_done(&pvmw);
				break;
			}
			VM_BUG_ON_PAGE(pte_write(pteval) && folio_test_anon(folio) &&
				       !anon_exclusive, subpage);

			/* See folio_try_share_anon_rmap_pte(): clear PTE first. */
			if (folio_test_hugetlb(folio)) {
				if (anon_exclusive &&
				    hugetlb_try_share_anon_rmap(folio)) {
					set_huge_pte_at(mm, address, pvmw.pte,
							pteval, hsz);
					ret = false;
					page_vma_mapped_walk_done(&pvmw);
					break;
				}
			} else if (anon_exclusive &&
				   folio_try_share_anon_rmap_pte(folio, subpage)) {
				set_pte_at(mm, address, pvmw.pte, pteval);
				ret = false;
				page_vma_mapped_walk_done(&pvmw);
				break;
			}

			/*
			 * Store the pfn of the page in a special migration
			 * pte. do_swap_page() will wait until the migration
			 * pte is removed and then restart fault handling.
			 */
			if (pte_write(pteval))
				entry = make_writable_migration_entry(
							page_to_pfn(subpage));
			else if (anon_exclusive)
				entry = make_readable_exclusive_migration_entry(
							page_to_pfn(subpage));
			else
				entry = make_readable_migration_entry(
							page_to_pfn(subpage));
			if (pte_young(pteval))
				entry = make_migration_entry_young(entry);
			if (pte_dirty(pteval))
				entry = make_migration_entry_dirty(entry);
			swp_pte = swp_entry_to_pte(entry);
			if (pte_soft_dirty(pteval))
				swp_pte = pte_swp_mksoft_dirty(swp_pte);
			if (pte_uffd_wp(pteval))
				swp_pte = pte_swp_mkuffd_wp(swp_pte);
			if (folio_test_hugetlb(folio))
				set_huge_pte_at(mm, address, pvmw.pte, swp_pte,
						hsz);
			else
				set_pte_at(mm, address, pvmw.pte, swp_pte);
			trace_set_migration_pte(address, pte_val(swp_pte),
						folio_order(folio));
			/*
			 * No need to invalidate here it will synchronize on
			 * against the special swap migration pte.
			 */
		}

		if (unlikely(folio_test_hugetlb(folio)))
			hugetlb_remove_rmap(folio);
		else
			folio_remove_rmap_pte(folio, subpage, vma);
		if (vma->vm_flags & VM_LOCKED)
			mlock_drain_local();
		folio_put(folio);
	}

	mmu_notifier_invalidate_range_end(&range);

	return ret;
}

/**
 * try_to_migrate - try to replace all page table mappings with swap entries
 * @folio: the folio to replace page table entries for
 * @flags: action and flags
 *
 * Tries to remove all the page table entries which are mapping this folio and
 * replace them with special swap entries. Caller must hold the folio lock.
 */
void try_to_migrate(struct folio *folio, enum ttu_flags flags)
{
	struct rmap_walk_control rwc = {
		.rmap_one = try_to_migrate_one,
		.arg = (void *)flags,
		.done = folio_not_mapped,
		.anon_lock = folio_lock_anon_vma_read,
	};

	/*
	 * Migration always ignores mlock and only supports TTU_RMAP_LOCKED and
	 * TTU_SPLIT_HUGE_PMD, TTU_SYNC, and TTU_BATCH_FLUSH flags.
	 */
	if (WARN_ON_ONCE(flags & ~(TTU_RMAP_LOCKED | TTU_SPLIT_HUGE_PMD |
					TTU_SYNC | TTU_BATCH_FLUSH)))
		return;

	if (folio_is_zone_device(folio) &&
	    (!folio_is_device_private(folio) && !folio_is_device_coherent(folio)))
		return;

	/*
	 * During exec, a temporary VMA is setup and later moved.
	 * The VMA is moved under the anon_vma lock but not the
	 * page tables leading to a race where migration cannot
	 * find the migration ptes. Rather than increasing the
	 * locking requirements of exec(), migration skips
	 * temporary VMAs until after exec() completes.
	 */
	if (!folio_test_ksm(folio) && folio_test_anon(folio))
		rwc.invalid_vma = invalid_migration_vma;

	if (flags & TTU_RMAP_LOCKED)
		rmap_walk_locked(folio, &rwc);
	else
		rmap_walk(folio, &rwc);
}

#ifdef CONFIG_DEVICE_PRIVATE
struct make_exclusive_args {
	struct mm_struct *mm;
	unsigned long address;
	void *owner;
	bool valid;
};

static bool page_make_device_exclusive_one(struct folio *folio,
		struct vm_area_struct *vma, unsigned long address, void *priv)
{
	struct mm_struct *mm = vma->vm_mm;
	DEFINE_FOLIO_VMA_WALK(pvmw, folio, vma, address, 0);
	struct make_exclusive_args *args = priv;
	pte_t pteval;
	struct page *subpage;
	bool ret = true;
	struct mmu_notifier_range range;
	swp_entry_t entry;
	pte_t swp_pte;
	pte_t ptent;

	mmu_notifier_range_init_owner(&range, MMU_NOTIFY_EXCLUSIVE, 0,
				      vma->vm_mm, address, min(vma->vm_end,
				      address + folio_size(folio)),
				      args->owner);
	mmu_notifier_invalidate_range_start(&range);

	while (page_vma_mapped_walk(&pvmw)) {
		/* Unexpected PMD-mapped THP? */
		VM_BUG_ON_FOLIO(!pvmw.pte, folio);

		ptent = ptep_get(pvmw.pte);
		if (!pte_present(ptent)) {
			ret = false;
			page_vma_mapped_walk_done(&pvmw);
			break;
		}

		subpage = folio_page(folio,
				pte_pfn(ptent) - folio_pfn(folio));
		address = pvmw.address;

		/* Nuke the page table entry. */
		flush_cache_page(vma, address, pte_pfn(ptent));
		pteval = ptep_clear_flush(vma, address, pvmw.pte);

		/* Set the dirty flag on the folio now the pte is gone. */
		if (pte_dirty(pteval))
			folio_mark_dirty(folio);

		/*
		 * Check that our target page is still mapped at the expected
		 * address.
		 */
		if (args->mm == mm && args->address == address &&
		    pte_write(pteval))
			args->valid = true;

		/*
		 * Store the pfn of the page in a special migration
		 * pte. do_swap_page() will wait until the migration
		 * pte is removed and then restart fault handling.
		 */
		if (pte_write(pteval))
			entry = make_writable_device_exclusive_entry(
							page_to_pfn(subpage));
		else
			entry = make_readable_device_exclusive_entry(
							page_to_pfn(subpage));
		swp_pte = swp_entry_to_pte(entry);
		if (pte_soft_dirty(pteval))
			swp_pte = pte_swp_mksoft_dirty(swp_pte);
		if (pte_uffd_wp(pteval))
			swp_pte = pte_swp_mkuffd_wp(swp_pte);

		set_pte_at(mm, address, pvmw.pte, swp_pte);

		/*
		 * There is a reference on the page for the swap entry which has
		 * been removed, so shouldn't take another.
		 */
		folio_remove_rmap_pte(folio, subpage, vma);
	}

	mmu_notifier_invalidate_range_end(&range);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/**
<<<<<<< HEAD
 * try_to_unmap_file - unmap/unlock file page using the object-based rmap method
 * @page: the page to unmap/unlock
 * @flags: action and flags
 *
 * Find all the mappings of a page using the mapping pointer and the vma chains
 * contained in the address_space struct it points to.
 *
 * This function is only called from try_to_unmap/try_to_munlock for
 * object-based pages.
 * When called from try_to_munlock(), the mmap_sem of the mm containing the vma
 * where the page was found will be held for write.  So, we won't recheck
 * vm_flags for that VMA.  That should be OK, because that vma shouldn't be
 * 'LOCKED.
 */
static int try_to_unmap_file(struct page *page, enum ttu_flags flags)
{
	struct address_space *mapping = page->mapping;
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	struct vm_area_struct *vma;
	struct prio_tree_iter iter;
	int ret = SWAP_AGAIN;
	unsigned long cursor;
	unsigned long max_nl_cursor = 0;
	unsigned long max_nl_size = 0;
	unsigned int mapcount;

	mutex_lock(&mapping->i_mmap_mutex);
	vma_prio_tree_foreach(vma, &iter, &mapping->i_mmap, pgoff, pgoff) {
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		ret = try_to_unmap_one(page, vma, address, flags);
		if (ret != SWAP_AGAIN || !page_mapped(page))
			goto out;
	}

	if (list_empty(&mapping->i_mmap_nonlinear))
		goto out;

	/*
	 * We don't bother to try to find the munlocked page in nonlinears.
	 * It's costly. Instead, later, page reclaim logic may call
	 * try_to_unmap(TTU_MUNLOCK) and recover PG_mlocked lazily.
	 */
	if (TTU_ACTION(flags) == TTU_MUNLOCK)
		goto out;

	list_for_each_entry(vma, &mapping->i_mmap_nonlinear,
						shared.vm_set.list) {
		cursor = (unsigned long) vma->vm_private_data;
		if (cursor > max_nl_cursor)
			max_nl_cursor = cursor;
		cursor = vma->vm_end - vma->vm_start;
		if (cursor > max_nl_size)
			max_nl_size = cursor;
	}

	if (max_nl_size == 0) {	/* all nonlinears locked or reserved ? */
		ret = SWAP_FAIL;
		goto out;
	}

	/*
	 * We don't try to search for this page in the nonlinear vmas,
	 * and page_referenced wouldn't have found it anyway.  Instead
	 * just walk the nonlinear vmas trying to age and unmap some.
	 * The mapcount of the page we came in with is irrelevant,
	 * but even so use it as a guide to how hard we should try?
	 */
	mapcount = page_mapcount(page);
	if (!mapcount)
		goto out;
	cond_resched();

	max_nl_size = (max_nl_size + CLUSTER_SIZE - 1) & CLUSTER_MASK;
	if (max_nl_cursor == 0)
		max_nl_cursor = CLUSTER_SIZE;

	do {
		list_for_each_entry(vma, &mapping->i_mmap_nonlinear,
						shared.vm_set.list) {
			cursor = (unsigned long) vma->vm_private_data;
			while ( cursor < max_nl_cursor &&
				cursor < vma->vm_end - vma->vm_start) {
				if (try_to_unmap_cluster(cursor, &mapcount,
						vma, page) == SWAP_MLOCK)
					ret = SWAP_MLOCK;
				cursor += CLUSTER_SIZE;
				vma->vm_private_data = (void *) cursor;
				if ((int)mapcount <= 0)
					goto out;
			}
			vma->vm_private_data = (void *) max_nl_cursor;
		}
		cond_resched();
		max_nl_cursor += CLUSTER_SIZE;
	} while (max_nl_cursor <= max_nl_size);

	/*
	 * Don't loop forever (perhaps all the remaining pages are
	 * in locked vmas).  Reset cursor on all unreserved nonlinear
	 * vmas, now forgetting on which ones it had fallen behind.
	 */
	list_for_each_entry(vma, &mapping->i_mmap_nonlinear, shared.vm_set.list)
		vma->vm_private_data = NULL;
out:
	mutex_unlock(&mapping->i_mmap_mutex);
	return ret;
}

/**
 * try_to_unmap - try to remove all page table mappings to a page
 * @page: the page to get unmapped
 * @flags: action and flags
 *
 * Tries to remove all the page table entries which are mapping this
 * page, used in the pageout path.  Caller must hold the page lock.
 * Return values are:
 *
 * SWAP_SUCCESS	- we succeeded in removing all mappings
 * SWAP_AGAIN	- we missed a mapping, try again later
 * SWAP_FAIL	- the page is unswappable
 * SWAP_MLOCK	- page is mlocked.
 */
int try_to_unmap(struct page *page, enum ttu_flags flags)
{
	int ret;

	BUG_ON(!PageLocked(page));
	VM_BUG_ON(!PageHuge(page) && PageTransHuge(page));

	if (unlikely(PageKsm(page)))
		ret = try_to_unmap_ksm(page, flags);
	else if (PageAnon(page))
		ret = try_to_unmap_anon(page, flags);
	else
		ret = try_to_unmap_file(page, flags);
	if (ret != SWAP_MLOCK && !page_mapped(page))
		ret = SWAP_SUCCESS;
	return ret;
}

/**
 * try_to_munlock - try to munlock a page
 * @page: the page to be munlocked
 *
 * Called from munlock code.  Checks all of the VMAs mapping the page
 * to make sure nobody else has this page mlocked. The page will be
 * returned with PG_mlocked cleared if no other vmas have it mlocked.
 *
 * Return values are:
 *
 * SWAP_AGAIN	- no vma is holding page mlocked, or,
 * SWAP_AGAIN	- page mapped in mlocked vma -- couldn't acquire mmap sem
 * SWAP_FAIL	- page cannot be located at present
 * SWAP_MLOCK	- page is now mlocked.
 */
int try_to_munlock(struct page *page)
{
	VM_BUG_ON(!PageLocked(page) || PageLRU(page));

	if (unlikely(PageKsm(page)))
		return try_to_unmap_ksm(page, TTU_MUNLOCK);
	else if (PageAnon(page))
		return try_to_unmap_anon(page, TTU_MUNLOCK);
	else
		return try_to_unmap_file(page, TTU_MUNLOCK);
}
=======
 * folio_make_device_exclusive - Mark the folio exclusively owned by a device.
 * @folio: The folio to replace page table entries for.
 * @mm: The mm_struct where the folio is expected to be mapped.
 * @address: Address where the folio is expected to be mapped.
 * @owner: passed to MMU_NOTIFY_EXCLUSIVE range notifier callbacks
 *
 * Tries to remove all the page table entries which are mapping this
 * folio and replace them with special device exclusive swap entries to
 * grant a device exclusive access to the folio.
 *
 * Context: Caller must hold the folio lock.
 * Return: false if the page is still mapped, or if it could not be unmapped
 * from the expected address. Otherwise returns true (success).
 */
static bool folio_make_device_exclusive(struct folio *folio,
		struct mm_struct *mm, unsigned long address, void *owner)
{
	struct make_exclusive_args args = {
		.mm = mm,
		.address = address,
		.owner = owner,
		.valid = false,
	};
	struct rmap_walk_control rwc = {
		.rmap_one = page_make_device_exclusive_one,
		.done = folio_not_mapped,
		.anon_lock = folio_lock_anon_vma_read,
		.arg = &args,
	};

	/*
	 * Restrict to anonymous folios for now to avoid potential writeback
	 * issues.
	 */
	if (!folio_test_anon(folio))
		return false;

	rmap_walk(folio, &rwc);

	return args.valid && !folio_mapcount(folio);
}

/**
 * make_device_exclusive_range() - Mark a range for exclusive use by a device
 * @mm: mm_struct of associated target process
 * @start: start of the region to mark for exclusive device access
 * @end: end address of region
 * @pages: returns the pages which were successfully marked for exclusive access
 * @owner: passed to MMU_NOTIFY_EXCLUSIVE range notifier to allow filtering
 *
 * Returns: number of pages found in the range by GUP. A page is marked for
 * exclusive access only if the page pointer is non-NULL.
 *
 * This function finds ptes mapping page(s) to the given address range, locks
 * them and replaces mappings with special swap entries preventing userspace CPU
 * access. On fault these entries are replaced with the original mapping after
 * calling MMU notifiers.
 *
 * A driver using this to program access from a device must use a mmu notifier
 * critical section to hold a device specific lock during programming. Once
 * programming is complete it should drop the page lock and reference after
 * which point CPU access to the page will revoke the exclusive access.
 */
int make_device_exclusive_range(struct mm_struct *mm, unsigned long start,
				unsigned long end, struct page **pages,
				void *owner)
{
	long npages = (end - start) >> PAGE_SHIFT;
	long i;

	npages = get_user_pages_remote(mm, start, npages,
				       FOLL_GET | FOLL_WRITE | FOLL_SPLIT_PMD,
				       pages, NULL);
	if (npages < 0)
		return npages;

	for (i = 0; i < npages; i++, start += PAGE_SIZE) {
		struct folio *folio = page_folio(pages[i]);
		if (PageTail(pages[i]) || !folio_trylock(folio)) {
			folio_put(folio);
			pages[i] = NULL;
			continue;
		}

		if (!folio_make_device_exclusive(folio, mm, start, owner)) {
			folio_unlock(folio);
			folio_put(folio);
			pages[i] = NULL;
		}
	}

	return npages;
}
EXPORT_SYMBOL_GPL(make_device_exclusive_range);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void __put_anon_vma(struct anon_vma *anon_vma)
{
	struct anon_vma *root = anon_vma->root;

	anon_vma_free(anon_vma);
	if (root != anon_vma && atomic_dec_and_test(&root->refcount))
		anon_vma_free(root);
}

<<<<<<< HEAD
#ifdef CONFIG_MIGRATION
/*
 * rmap_walk() and its helpers rmap_walk_anon() and rmap_walk_file():
 * Called by migrate.c to remove migration ptes, but might be used more later.
 */
static int rmap_walk_anon(struct page *page, int (*rmap_one)(struct page *,
		struct vm_area_struct *, unsigned long, void *), void *arg)
{
	struct anon_vma *anon_vma;
	struct anon_vma_chain *avc;
	int ret = SWAP_AGAIN;

	/*
	 * Note: remove_migration_ptes() cannot use page_lock_anon_vma()
	 * because that depends on page_mapped(); but not all its usages
	 * are holding mmap_sem. Users without mmap_sem are required to
	 * take a reference count to prevent the anon_vma disappearing
	 */
	anon_vma = page_anon_vma(page);
	if (!anon_vma)
		return ret;
	anon_vma_lock(anon_vma);
	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		ret = rmap_one(page, vma, address, arg);
		if (ret != SWAP_AGAIN)
			break;
	}
	anon_vma_unlock(anon_vma);
	return ret;
}

static int rmap_walk_file(struct page *page, int (*rmap_one)(struct page *,
		struct vm_area_struct *, unsigned long, void *), void *arg)
{
	struct address_space *mapping = page->mapping;
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	struct vm_area_struct *vma;
	struct prio_tree_iter iter;
	int ret = SWAP_AGAIN;

	if (!mapping)
		return ret;
	mutex_lock(&mapping->i_mmap_mutex);
	vma_prio_tree_foreach(vma, &iter, &mapping->i_mmap, pgoff, pgoff) {
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		ret = rmap_one(page, vma, address, arg);
		if (ret != SWAP_AGAIN)
			break;
	}
	/*
	 * No nonlinear handling: being always shared, nonlinear vmas
	 * never contain migration ptes.  Decide what to do about this
	 * limitation to linear when we need rmap_walk() on nonlinear.
	 */
	mutex_unlock(&mapping->i_mmap_mutex);
	return ret;
}

int rmap_walk(struct page *page, int (*rmap_one)(struct page *,
		struct vm_area_struct *, unsigned long, void *), void *arg)
{
	VM_BUG_ON(!PageLocked(page));

	if (unlikely(PageKsm(page)))
		return rmap_walk_ksm(page, rmap_one, arg);
	else if (PageAnon(page))
		return rmap_walk_anon(page, rmap_one, arg);
	else
		return rmap_walk_file(page, rmap_one, arg);
}
#endif /* CONFIG_MIGRATION */

#ifdef CONFIG_HUGETLB_PAGE
/*
 * The following three functions are for anonymous (private mapped) hugepages.
 * Unlike common anonymous pages, anonymous hugepages have no accounting code
 * and no lru code, because we handle hugepages differently from common pages.
 */
static void __hugepage_set_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address, int exclusive)
{
	struct anon_vma *anon_vma = vma->anon_vma;

	BUG_ON(!anon_vma);

	if (PageAnon(page))
		return;
	if (!exclusive)
		anon_vma = anon_vma->root;

	anon_vma = (void *) anon_vma + PAGE_MAPPING_ANON;
	page->mapping = (struct address_space *) anon_vma;
	page->index = linear_page_index(vma, address);
}

void hugepage_add_anon_rmap(struct page *page,
			    struct vm_area_struct *vma, unsigned long address)
{
	struct anon_vma *anon_vma = vma->anon_vma;
	int first;

	BUG_ON(!PageLocked(page));
	BUG_ON(!anon_vma);
	/* address might be in next vma when migration races vma_adjust */
	first = atomic_inc_and_test(&page->_mapcount);
	if (first)
		__hugepage_set_anon_rmap(page, vma, address, 0);
}

void hugepage_add_new_anon_rmap(struct page *page,
			struct vm_area_struct *vma, unsigned long address)
{
	BUG_ON(address < vma->vm_start || address >= vma->vm_end);
	atomic_set(&page->_mapcount, 0);
	__hugepage_set_anon_rmap(page, vma, address, 1);
=======
static struct anon_vma *rmap_walk_anon_lock(struct folio *folio,
					    struct rmap_walk_control *rwc)
{
	struct anon_vma *anon_vma;

	if (rwc->anon_lock)
		return rwc->anon_lock(folio, rwc);

	/*
	 * Note: remove_migration_ptes() cannot use folio_lock_anon_vma_read()
	 * because that depends on page_mapped(); but not all its usages
	 * are holding mmap_lock. Users without mmap_lock are required to
	 * take a reference count to prevent the anon_vma disappearing
	 */
	anon_vma = folio_anon_vma(folio);
	if (!anon_vma)
		return NULL;

	if (anon_vma_trylock_read(anon_vma))
		goto out;

	if (rwc->try_lock) {
		anon_vma = NULL;
		rwc->contended = true;
		goto out;
	}

	anon_vma_lock_read(anon_vma);
out:
	return anon_vma;
}

/*
 * rmap_walk_anon - do something to anonymous page using the object-based
 * rmap method
 * @folio: the folio to be handled
 * @rwc: control variable according to each walk type
 * @locked: caller holds relevant rmap lock
 *
 * Find all the mappings of a folio using the mapping pointer and the vma
 * chains contained in the anon_vma struct it points to.
 */
static void rmap_walk_anon(struct folio *folio,
		struct rmap_walk_control *rwc, bool locked)
{
	struct anon_vma *anon_vma;
	pgoff_t pgoff_start, pgoff_end;
	struct anon_vma_chain *avc;

	if (locked) {
		anon_vma = folio_anon_vma(folio);
		/* anon_vma disappear under us? */
		VM_BUG_ON_FOLIO(!anon_vma, folio);
	} else {
		anon_vma = rmap_walk_anon_lock(folio, rwc);
	}
	if (!anon_vma)
		return;

	pgoff_start = folio_pgoff(folio);
	pgoff_end = pgoff_start + folio_nr_pages(folio) - 1;
	anon_vma_interval_tree_foreach(avc, &anon_vma->rb_root,
			pgoff_start, pgoff_end) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long address = vma_address(&folio->page, vma);

		VM_BUG_ON_VMA(address == -EFAULT, vma);
		cond_resched();

		if (rwc->invalid_vma && rwc->invalid_vma(vma, rwc->arg))
			continue;

		if (!rwc->rmap_one(folio, vma, address, rwc->arg))
			break;
		if (rwc->done && rwc->done(folio))
			break;
	}

	if (!locked)
		anon_vma_unlock_read(anon_vma);
}

/*
 * rmap_walk_file - do something to file page using the object-based rmap method
 * @folio: the folio to be handled
 * @rwc: control variable according to each walk type
 * @locked: caller holds relevant rmap lock
 *
 * Find all the mappings of a folio using the mapping pointer and the vma chains
 * contained in the address_space struct it points to.
 */
static void rmap_walk_file(struct folio *folio,
		struct rmap_walk_control *rwc, bool locked)
{
	struct address_space *mapping = folio_mapping(folio);
	pgoff_t pgoff_start, pgoff_end;
	struct vm_area_struct *vma;

	/*
	 * The page lock not only makes sure that page->mapping cannot
	 * suddenly be NULLified by truncation, it makes sure that the
	 * structure at mapping cannot be freed and reused yet,
	 * so we can safely take mapping->i_mmap_rwsem.
	 */
	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);

	if (!mapping)
		return;

	pgoff_start = folio_pgoff(folio);
	pgoff_end = pgoff_start + folio_nr_pages(folio) - 1;
	if (!locked) {
		if (i_mmap_trylock_read(mapping))
			goto lookup;

		if (rwc->try_lock) {
			rwc->contended = true;
			return;
		}

		i_mmap_lock_read(mapping);
	}
lookup:
	vma_interval_tree_foreach(vma, &mapping->i_mmap,
			pgoff_start, pgoff_end) {
		unsigned long address = vma_address(&folio->page, vma);

		VM_BUG_ON_VMA(address == -EFAULT, vma);
		cond_resched();

		if (rwc->invalid_vma && rwc->invalid_vma(vma, rwc->arg))
			continue;

		if (!rwc->rmap_one(folio, vma, address, rwc->arg))
			goto done;
		if (rwc->done && rwc->done(folio))
			goto done;
	}

done:
	if (!locked)
		i_mmap_unlock_read(mapping);
}

void rmap_walk(struct folio *folio, struct rmap_walk_control *rwc)
{
	if (unlikely(folio_test_ksm(folio)))
		rmap_walk_ksm(folio, rwc);
	else if (folio_test_anon(folio))
		rmap_walk_anon(folio, rwc, false);
	else
		rmap_walk_file(folio, rwc, false);
}

/* Like rmap_walk, but caller holds relevant rmap lock */
void rmap_walk_locked(struct folio *folio, struct rmap_walk_control *rwc)
{
	/* no ksm support for now */
	VM_BUG_ON_FOLIO(folio_test_ksm(folio), folio);
	if (folio_test_anon(folio))
		rmap_walk_anon(folio, rwc, true);
	else
		rmap_walk_file(folio, rwc, true);
}

#ifdef CONFIG_HUGETLB_PAGE
/*
 * The following two functions are for anonymous (private mapped) hugepages.
 * Unlike common anonymous pages, anonymous hugepages have no accounting code
 * and no lru code, because we handle hugepages differently from common pages.
 */
void hugetlb_add_anon_rmap(struct folio *folio, struct vm_area_struct *vma,
		unsigned long address, rmap_t flags)
{
	VM_WARN_ON_FOLIO(!folio_test_hugetlb(folio), folio);
	VM_WARN_ON_FOLIO(!folio_test_anon(folio), folio);

	atomic_inc(&folio->_entire_mapcount);
	if (flags & RMAP_EXCLUSIVE)
		SetPageAnonExclusive(&folio->page);
	VM_WARN_ON_FOLIO(folio_entire_mapcount(folio) > 1 &&
			 PageAnonExclusive(&folio->page), folio);
}

void hugetlb_add_new_anon_rmap(struct folio *folio,
		struct vm_area_struct *vma, unsigned long address)
{
	VM_WARN_ON_FOLIO(!folio_test_hugetlb(folio), folio);

	BUG_ON(address < vma->vm_start || address >= vma->vm_end);
	/* increment count (starts at -1) */
	atomic_set(&folio->_entire_mapcount, 0);
	folio_clear_hugetlb_restore_reserve(folio);
	__folio_set_anon(folio, vma, address, true);
	SetPageAnonExclusive(&folio->page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif /* CONFIG_HUGETLB_PAGE */
