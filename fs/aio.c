/*
 *	An async IO implementation for Linux
 *	Written by Benjamin LaHaise <bcrl@kvack.org>
 *
 *	Implements an efficient asynchronous io interface.
 *
 *	Copyright 2000, 2001, 2002 Red Hat, Inc.  All Rights Reserved.
<<<<<<< HEAD
 *
 *	See ../COPYING for licensing terms.
 */
=======
 *	Copyright 2018 Christoph Hellwig.
 *
 *	See ../COPYING for licensing terms.
 */
#define pr_fmt(fmt) "%s: " fmt, __func__

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <linux/aio_abi.h>
#include <linux/export.h>
#include <linux/syscalls.h>
#include <linux/backing-dev.h>
<<<<<<< HEAD
#include <linux/uio.h>

#define DEBUG 0

#include <linux/sched.h>
=======
#include <linux/refcount.h>
#include <linux/uio.h>

#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/mman.h>
<<<<<<< HEAD
#include <linux/mmu_context.h>
=======
#include <linux/percpu.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/aio.h>
#include <linux/highmem.h>
#include <linux/workqueue.h>
#include <linux/security.h>
#include <linux/eventfd.h>
#include <linux/blkdev.h>
#include <linux/compat.h>
<<<<<<< HEAD
#include <linux/personality.h>

#include <asm/kmap_types.h>
#include <asm/uaccess.h>

#if DEBUG > 1
#define dprintk		printk
#else
#define dprintk(x...)	do { ; } while (0)
#endif

/*------ sysctl variables----*/
static DEFINE_SPINLOCK(aio_nr_lock);
unsigned long aio_nr;		/* current system wide number of aio requests */
unsigned long aio_max_nr = 0x10000; /* system wide maximum number of aio requests */
/*----end sysctl variables---*/
=======
#include <linux/migrate.h>
#include <linux/ramfs.h>
#include <linux/percpu-refcount.h>
#include <linux/mount.h>
#include <linux/pseudo_fs.h>

#include <linux/uaccess.h>
#include <linux/nospec.h>

#include "internal.h"

#define KIOCB_KEY		0

#define AIO_RING_MAGIC			0xa10a10a1
#define AIO_RING_COMPAT_FEATURES	1
#define AIO_RING_INCOMPAT_FEATURES	0
struct aio_ring {
	unsigned	id;	/* kernel internal index number */
	unsigned	nr;	/* number of io_events */
	unsigned	head;	/* Written to by userland or under ring_lock
				 * mutex by aio_read_events_ring(). */
	unsigned	tail;

	unsigned	magic;
	unsigned	compat_features;
	unsigned	incompat_features;
	unsigned	header_length;	/* size of aio_ring */


	struct io_event		io_events[];
}; /* 128 bytes + ring size */

/*
 * Plugging is meant to work with larger batches of IOs. If we don't
 * have more than the below, then don't bother setting up a plug.
 */
#define AIO_PLUG_THRESHOLD	2

#define AIO_RING_PAGES	8

struct kioctx_table {
	struct rcu_head		rcu;
	unsigned		nr;
	struct kioctx __rcu	*table[] __counted_by(nr);
};

struct kioctx_cpu {
	unsigned		reqs_available;
};

struct ctx_rq_wait {
	struct completion comp;
	atomic_t count;
};

struct kioctx {
	struct percpu_ref	users;
	atomic_t		dead;

	struct percpu_ref	reqs;

	unsigned long		user_id;

	struct __percpu kioctx_cpu *cpu;

	/*
	 * For percpu reqs_available, number of slots we move to/from global
	 * counter at a time:
	 */
	unsigned		req_batch;
	/*
	 * This is what userspace passed to io_setup(), it's not used for
	 * anything but counting against the global max_reqs quota.
	 *
	 * The real limit is nr_events - 1, which will be larger (see
	 * aio_setup_ring())
	 */
	unsigned		max_reqs;

	/* Size of ringbuffer, in units of struct io_event */
	unsigned		nr_events;

	unsigned long		mmap_base;
	unsigned long		mmap_size;

	struct page		**ring_pages;
	long			nr_pages;

	struct rcu_work		free_rwork;	/* see free_ioctx() */

	/*
	 * signals when all in-flight requests are done
	 */
	struct ctx_rq_wait	*rq_wait;

	struct {
		/*
		 * This counts the number of available slots in the ringbuffer,
		 * so we avoid overflowing it: it's decremented (if positive)
		 * when allocating a kiocb and incremented when the resulting
		 * io_event is pulled off the ringbuffer.
		 *
		 * We batch accesses to it with a percpu version.
		 */
		atomic_t	reqs_available;
	} ____cacheline_aligned_in_smp;

	struct {
		spinlock_t	ctx_lock;
		struct list_head active_reqs;	/* used for cancellation */
	} ____cacheline_aligned_in_smp;

	struct {
		struct mutex	ring_lock;
		wait_queue_head_t wait;
	} ____cacheline_aligned_in_smp;

	struct {
		unsigned	tail;
		unsigned	completed_events;
		spinlock_t	completion_lock;
	} ____cacheline_aligned_in_smp;

	struct page		*internal_pages[AIO_RING_PAGES];
	struct file		*aio_ring_file;

	unsigned		id;
};

/*
 * First field must be the file pointer in all the
 * iocb unions! See also 'struct kiocb' in <linux/fs.h>
 */
struct fsync_iocb {
	struct file		*file;
	struct work_struct	work;
	bool			datasync;
	struct cred		*creds;
};

struct poll_iocb {
	struct file		*file;
	struct wait_queue_head	*head;
	__poll_t		events;
	bool			cancelled;
	bool			work_scheduled;
	bool			work_need_resched;
	struct wait_queue_entry	wait;
	struct work_struct	work;
};

/*
 * NOTE! Each of the iocb union members has the file pointer
 * as the first entry in their struct definition. So you can
 * access the file pointer through any of the sub-structs,
 * or directly as just 'ki_filp' in this struct.
 */
struct aio_kiocb {
	union {
		struct file		*ki_filp;
		struct kiocb		rw;
		struct fsync_iocb	fsync;
		struct poll_iocb	poll;
	};

	struct kioctx		*ki_ctx;
	kiocb_cancel_fn		*ki_cancel;

	struct io_event		ki_res;

	struct list_head	ki_list;	/* the aio core uses this
						 * for cancellation */
	refcount_t		ki_refcnt;

	/*
	 * If the aio_resfd field of the userspace iocb is not zero,
	 * this is the underlying eventfd context to deliver events to.
	 */
	struct eventfd_ctx	*ki_eventfd;
};

/*------ sysctl variables----*/
static DEFINE_SPINLOCK(aio_nr_lock);
static unsigned long aio_nr;		/* current system wide number of aio requests */
static unsigned long aio_max_nr = 0x10000; /* system wide maximum number of aio requests */
/*----end sysctl variables---*/
#ifdef CONFIG_SYSCTL
static struct ctl_table aio_sysctls[] = {
	{
		.procname	= "aio-nr",
		.data		= &aio_nr,
		.maxlen		= sizeof(aio_nr),
		.mode		= 0444,
		.proc_handler	= proc_doulongvec_minmax,
	},
	{
		.procname	= "aio-max-nr",
		.data		= &aio_max_nr,
		.maxlen		= sizeof(aio_max_nr),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
	},
};

static void __init aio_sysctl_init(void)
{
	register_sysctl_init("fs", aio_sysctls);
}
#else
#define aio_sysctl_init() do { } while (0)
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct kmem_cache	*kiocb_cachep;
static struct kmem_cache	*kioctx_cachep;

<<<<<<< HEAD
static struct workqueue_struct *aio_wq;

/* Used for rare fput completion. */
static void aio_fput_routine(struct work_struct *);
static DECLARE_WORK(fput_work, aio_fput_routine);

static DEFINE_SPINLOCK(fput_lock);
static LIST_HEAD(fput_head);

static void aio_kick_handler(struct work_struct *);
static void aio_queue_work(struct kioctx *);
=======
static struct vfsmount *aio_mnt;

static const struct file_operations aio_ring_fops;
static const struct address_space_operations aio_ctx_aops;

static struct file *aio_private_file(struct kioctx *ctx, loff_t nr_pages)
{
	struct file *file;
	struct inode *inode = alloc_anon_inode(aio_mnt->mnt_sb);
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	inode->i_mapping->a_ops = &aio_ctx_aops;
	inode->i_mapping->i_private_data = ctx;
	inode->i_size = PAGE_SIZE * nr_pages;

	file = alloc_file_pseudo(inode, aio_mnt, "[aio]",
				O_RDWR, &aio_ring_fops);
	if (IS_ERR(file))
		iput(inode);
	return file;
}

static int aio_init_fs_context(struct fs_context *fc)
{
	if (!init_pseudo(fc, AIO_RING_MAGIC))
		return -ENOMEM;
	fc->s_iflags |= SB_I_NOEXEC;
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* aio_setup
 *	Creates the slab caches used by the aio routines, panic on
 *	failure as this is done early during the boot sequence.
 */
static int __init aio_setup(void)
{
<<<<<<< HEAD
	kiocb_cachep = KMEM_CACHE(kiocb, SLAB_HWCACHE_ALIGN|SLAB_PANIC);
	kioctx_cachep = KMEM_CACHE(kioctx,SLAB_HWCACHE_ALIGN|SLAB_PANIC);

	aio_wq = alloc_workqueue("aio", 0, 1);	/* used to limit concurrency */
	BUG_ON(!aio_wq);

	pr_debug("aio_setup: sizeof(struct page) = %d\n", (int)sizeof(struct page));

=======
	static struct file_system_type aio_fs = {
		.name		= "aio",
		.init_fs_context = aio_init_fs_context,
		.kill_sb	= kill_anon_super,
	};
	aio_mnt = kern_mount(&aio_fs);
	if (IS_ERR(aio_mnt))
		panic("Failed to create aio fs mount.");

	kiocb_cachep = KMEM_CACHE(aio_kiocb, SLAB_HWCACHE_ALIGN|SLAB_PANIC);
	kioctx_cachep = KMEM_CACHE(kioctx,SLAB_HWCACHE_ALIGN|SLAB_PANIC);
	aio_sysctl_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
__initcall(aio_setup);

<<<<<<< HEAD
static void aio_free_ring(struct kioctx *ctx)
{
	struct aio_ring_info *info = &ctx->ring_info;
	long i;

	for (i=0; i<info->nr_pages; i++)
		put_page(info->ring_pages[i]);

	if (info->mmap_size) {
		BUG_ON(ctx->mm != current->mm);
		vm_munmap(info->mmap_base, info->mmap_size);
	}

	if (info->ring_pages && info->ring_pages != info->internal_pages)
		kfree(info->ring_pages);
	info->ring_pages = NULL;
	info->nr = 0;
}

static int aio_setup_ring(struct kioctx *ctx)
{
	struct aio_ring *ring;
	struct aio_ring_info *info = &ctx->ring_info;
	unsigned nr_events = ctx->max_reqs;
	unsigned long size;
	int nr_pages;

	if (current->personality & READ_IMPLIES_EXEC)
		return -EPERM;
=======
static void put_aio_ring_file(struct kioctx *ctx)
{
	struct file *aio_ring_file = ctx->aio_ring_file;
	struct address_space *i_mapping;

	if (aio_ring_file) {
		truncate_setsize(file_inode(aio_ring_file), 0);

		/* Prevent further access to the kioctx from migratepages */
		i_mapping = aio_ring_file->f_mapping;
		spin_lock(&i_mapping->i_private_lock);
		i_mapping->i_private_data = NULL;
		ctx->aio_ring_file = NULL;
		spin_unlock(&i_mapping->i_private_lock);

		fput(aio_ring_file);
	}
}

static void aio_free_ring(struct kioctx *ctx)
{
	int i;

	/* Disconnect the kiotx from the ring file.  This prevents future
	 * accesses to the kioctx from page migration.
	 */
	put_aio_ring_file(ctx);

	for (i = 0; i < ctx->nr_pages; i++) {
		struct page *page;
		pr_debug("pid(%d) [%d] page->count=%d\n", current->pid, i,
				page_count(ctx->ring_pages[i]));
		page = ctx->ring_pages[i];
		if (!page)
			continue;
		ctx->ring_pages[i] = NULL;
		put_page(page);
	}

	if (ctx->ring_pages && ctx->ring_pages != ctx->internal_pages) {
		kfree(ctx->ring_pages);
		ctx->ring_pages = NULL;
	}
}

static int aio_ring_mremap(struct vm_area_struct *vma)
{
	struct file *file = vma->vm_file;
	struct mm_struct *mm = vma->vm_mm;
	struct kioctx_table *table;
	int i, res = -EINVAL;

	spin_lock(&mm->ioctx_lock);
	rcu_read_lock();
	table = rcu_dereference(mm->ioctx_table);
	if (!table)
		goto out_unlock;

	for (i = 0; i < table->nr; i++) {
		struct kioctx *ctx;

		ctx = rcu_dereference(table->table[i]);
		if (ctx && ctx->aio_ring_file == file) {
			if (!atomic_read(&ctx->dead)) {
				ctx->user_id = ctx->mmap_base = vma->vm_start;
				res = 0;
			}
			break;
		}
	}

out_unlock:
	rcu_read_unlock();
	spin_unlock(&mm->ioctx_lock);
	return res;
}

static const struct vm_operations_struct aio_ring_vm_ops = {
	.mremap		= aio_ring_mremap,
#if IS_ENABLED(CONFIG_MMU)
	.fault		= filemap_fault,
	.map_pages	= filemap_map_pages,
	.page_mkwrite	= filemap_page_mkwrite,
#endif
};

static int aio_ring_mmap(struct file *file, struct vm_area_struct *vma)
{
	vm_flags_set(vma, VM_DONTEXPAND);
	vma->vm_ops = &aio_ring_vm_ops;
	return 0;
}

static const struct file_operations aio_ring_fops = {
	.mmap = aio_ring_mmap,
};

#if IS_ENABLED(CONFIG_MIGRATION)
static int aio_migrate_folio(struct address_space *mapping, struct folio *dst,
			struct folio *src, enum migrate_mode mode)
{
	struct kioctx *ctx;
	unsigned long flags;
	pgoff_t idx;
	int rc;

	/*
	 * We cannot support the _NO_COPY case here, because copy needs to
	 * happen under the ctx->completion_lock. That does not work with the
	 * migration workflow of MIGRATE_SYNC_NO_COPY.
	 */
	if (mode == MIGRATE_SYNC_NO_COPY)
		return -EINVAL;

	rc = 0;

	/* mapping->i_private_lock here protects against the kioctx teardown.  */
	spin_lock(&mapping->i_private_lock);
	ctx = mapping->i_private_data;
	if (!ctx) {
		rc = -EINVAL;
		goto out;
	}

	/* The ring_lock mutex.  The prevents aio_read_events() from writing
	 * to the ring's head, and prevents page migration from mucking in
	 * a partially initialized kiotx.
	 */
	if (!mutex_trylock(&ctx->ring_lock)) {
		rc = -EAGAIN;
		goto out;
	}

	idx = src->index;
	if (idx < (pgoff_t)ctx->nr_pages) {
		/* Make sure the old folio hasn't already been changed */
		if (ctx->ring_pages[idx] != &src->page)
			rc = -EAGAIN;
	} else
		rc = -EINVAL;

	if (rc != 0)
		goto out_unlock;

	/* Writeback must be complete */
	BUG_ON(folio_test_writeback(src));
	folio_get(dst);

	rc = folio_migrate_mapping(mapping, dst, src, 1);
	if (rc != MIGRATEPAGE_SUCCESS) {
		folio_put(dst);
		goto out_unlock;
	}

	/* Take completion_lock to prevent other writes to the ring buffer
	 * while the old folio is copied to the new.  This prevents new
	 * events from being lost.
	 */
	spin_lock_irqsave(&ctx->completion_lock, flags);
	folio_migrate_copy(dst, src);
	BUG_ON(ctx->ring_pages[idx] != &src->page);
	ctx->ring_pages[idx] = &dst->page;
	spin_unlock_irqrestore(&ctx->completion_lock, flags);

	/* The old folio is no longer accessible. */
	folio_put(src);

out_unlock:
	mutex_unlock(&ctx->ring_lock);
out:
	spin_unlock(&mapping->i_private_lock);
	return rc;
}
#else
#define aio_migrate_folio NULL
#endif

static const struct address_space_operations aio_ctx_aops = {
	.dirty_folio	= noop_dirty_folio,
	.migrate_folio	= aio_migrate_folio,
};

static int aio_setup_ring(struct kioctx *ctx, unsigned int nr_events)
{
	struct aio_ring *ring;
	struct mm_struct *mm = current->mm;
	unsigned long size, unused;
	int nr_pages;
	int i;
	struct file *file;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Compensate for the ring buffer's head/tail overlap entry */
	nr_events += 2;	/* 1 is required, 2 for good luck */

	size = sizeof(struct aio_ring);
	size += sizeof(struct io_event) * nr_events;
<<<<<<< HEAD
	nr_pages = (size + PAGE_SIZE-1) >> PAGE_SHIFT;

	if (nr_pages < 0)
		return -EINVAL;

	nr_events = (PAGE_SIZE * nr_pages - sizeof(struct aio_ring)) / sizeof(struct io_event);

	info->nr = 0;
	info->ring_pages = info->internal_pages;
	if (nr_pages > AIO_RING_PAGES) {
		info->ring_pages = kcalloc(nr_pages, sizeof(struct page *), GFP_KERNEL);
		if (!info->ring_pages)
			return -ENOMEM;
	}

	info->mmap_size = nr_pages * PAGE_SIZE;
	dprintk("attempting mmap of %lu bytes\n", info->mmap_size);
	down_write(&ctx->mm->mmap_sem);
	info->mmap_base = do_mmap(NULL, 0, info->mmap_size, 
				  PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE,
				  0);
	if (IS_ERR((void *)info->mmap_base)) {
		up_write(&ctx->mm->mmap_sem);
		info->mmap_size = 0;
		aio_free_ring(ctx);
		return -EAGAIN;
	}

	dprintk("mmap address: 0x%08lx\n", info->mmap_base);
	info->nr_pages = get_user_pages(current, ctx->mm,
					info->mmap_base, nr_pages, 
					1, 0, info->ring_pages, NULL);
	up_write(&ctx->mm->mmap_sem);

	if (unlikely(info->nr_pages != nr_pages)) {
		aio_free_ring(ctx);
		return -EAGAIN;
	}

	ctx->user_id = info->mmap_base;

	info->nr = nr_events;		/* trusted copy */

	ring = kmap_atomic(info->ring_pages[0]);
	ring->nr = nr_events;	/* user copy */
	ring->id = ctx->user_id;
=======

	nr_pages = PFN_UP(size);
	if (nr_pages < 0)
		return -EINVAL;

	file = aio_private_file(ctx, nr_pages);
	if (IS_ERR(file)) {
		ctx->aio_ring_file = NULL;
		return -ENOMEM;
	}

	ctx->aio_ring_file = file;
	nr_events = (PAGE_SIZE * nr_pages - sizeof(struct aio_ring))
			/ sizeof(struct io_event);

	ctx->ring_pages = ctx->internal_pages;
	if (nr_pages > AIO_RING_PAGES) {
		ctx->ring_pages = kcalloc(nr_pages, sizeof(struct page *),
					  GFP_KERNEL);
		if (!ctx->ring_pages) {
			put_aio_ring_file(ctx);
			return -ENOMEM;
		}
	}

	for (i = 0; i < nr_pages; i++) {
		struct page *page;
		page = find_or_create_page(file->f_mapping,
					   i, GFP_USER | __GFP_ZERO);
		if (!page)
			break;
		pr_debug("pid(%d) page[%d]->count=%d\n",
			 current->pid, i, page_count(page));
		SetPageUptodate(page);
		unlock_page(page);

		ctx->ring_pages[i] = page;
	}
	ctx->nr_pages = i;

	if (unlikely(i != nr_pages)) {
		aio_free_ring(ctx);
		return -ENOMEM;
	}

	ctx->mmap_size = nr_pages * PAGE_SIZE;
	pr_debug("attempting mmap of %lu bytes\n", ctx->mmap_size);

	if (mmap_write_lock_killable(mm)) {
		ctx->mmap_size = 0;
		aio_free_ring(ctx);
		return -EINTR;
	}

	ctx->mmap_base = do_mmap(ctx->aio_ring_file, 0, ctx->mmap_size,
				 PROT_READ | PROT_WRITE,
				 MAP_SHARED, 0, 0, &unused, NULL);
	mmap_write_unlock(mm);
	if (IS_ERR((void *)ctx->mmap_base)) {
		ctx->mmap_size = 0;
		aio_free_ring(ctx);
		return -ENOMEM;
	}

	pr_debug("mmap address: 0x%08lx\n", ctx->mmap_base);

	ctx->user_id = ctx->mmap_base;
	ctx->nr_events = nr_events; /* trusted copy */

	ring = page_address(ctx->ring_pages[0]);
	ring->nr = nr_events;	/* user copy */
	ring->id = ~0U;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ring->head = ring->tail = 0;
	ring->magic = AIO_RING_MAGIC;
	ring->compat_features = AIO_RING_COMPAT_FEATURES;
	ring->incompat_features = AIO_RING_INCOMPAT_FEATURES;
	ring->header_length = sizeof(struct aio_ring);
<<<<<<< HEAD
	kunmap_atomic(ring);
=======
	flush_dcache_page(ctx->ring_pages[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD

/* aio_ring_event: returns a pointer to the event at the given index from
 * kmap_atomic().  Release the pointer with put_aio_ring_event();
 */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define AIO_EVENTS_PER_PAGE	(PAGE_SIZE / sizeof(struct io_event))
#define AIO_EVENTS_FIRST_PAGE	((PAGE_SIZE - sizeof(struct aio_ring)) / sizeof(struct io_event))
#define AIO_EVENTS_OFFSET	(AIO_EVENTS_PER_PAGE - AIO_EVENTS_FIRST_PAGE)

<<<<<<< HEAD
#define aio_ring_event(info, nr) ({					\
	unsigned pos = (nr) + AIO_EVENTS_OFFSET;			\
	struct io_event *__event;					\
	__event = kmap_atomic(						\
			(info)->ring_pages[pos / AIO_EVENTS_PER_PAGE]); \
	__event += pos % AIO_EVENTS_PER_PAGE;				\
	__event;							\
})

#define put_aio_ring_event(event) do {		\
	struct io_event *__event = (event);	\
	(void)__event;				\
	kunmap_atomic((void *)((unsigned long)__event & PAGE_MASK)); \
} while(0)

static void ctx_rcu_free(struct rcu_head *head)
{
	struct kioctx *ctx = container_of(head, struct kioctx, rcu_head);
	kmem_cache_free(kioctx_cachep, ctx);
}

/* __put_ioctx
 *	Called when the last user of an aio context has gone away,
 *	and the struct needs to be freed.
 */
static void __put_ioctx(struct kioctx *ctx)
{
	unsigned nr_events = ctx->max_reqs;
	BUG_ON(ctx->reqs_active);

	cancel_delayed_work_sync(&ctx->wq);
	aio_free_ring(ctx);
	mmdrop(ctx->mm);
	ctx->mm = NULL;
	if (nr_events) {
		spin_lock(&aio_nr_lock);
		BUG_ON(aio_nr - nr_events > aio_nr);
		aio_nr -= nr_events;
		spin_unlock(&aio_nr_lock);
	}
	pr_debug("__put_ioctx: freeing %p\n", ctx);
	call_rcu(&ctx->rcu_head, ctx_rcu_free);
}

static inline int try_get_ioctx(struct kioctx *kioctx)
{
	return atomic_inc_not_zero(&kioctx->users);
}

static inline void put_ioctx(struct kioctx *kioctx)
{
	BUG_ON(atomic_read(&kioctx->users) <= 0);
	if (unlikely(atomic_dec_and_test(&kioctx->users)))
		__put_ioctx(kioctx);
=======
void kiocb_set_cancel_fn(struct kiocb *iocb, kiocb_cancel_fn *cancel)
{
	struct aio_kiocb *req;
	struct kioctx *ctx;
	unsigned long flags;

	/*
	 * kiocb didn't come from aio or is neither a read nor a write, hence
	 * ignore it.
	 */
	if (!(iocb->ki_flags & IOCB_AIO_RW))
		return;

	req = container_of(iocb, struct aio_kiocb, rw);

	if (WARN_ON_ONCE(!list_empty(&req->ki_list)))
		return;

	ctx = req->ki_ctx;

	spin_lock_irqsave(&ctx->ctx_lock, flags);
	list_add_tail(&req->ki_list, &ctx->active_reqs);
	req->ki_cancel = cancel;
	spin_unlock_irqrestore(&ctx->ctx_lock, flags);
}
EXPORT_SYMBOL(kiocb_set_cancel_fn);

/*
 * free_ioctx() should be RCU delayed to synchronize against the RCU
 * protected lookup_ioctx() and also needs process context to call
 * aio_free_ring().  Use rcu_work.
 */
static void free_ioctx(struct work_struct *work)
{
	struct kioctx *ctx = container_of(to_rcu_work(work), struct kioctx,
					  free_rwork);
	pr_debug("freeing %p\n", ctx);

	aio_free_ring(ctx);
	free_percpu(ctx->cpu);
	percpu_ref_exit(&ctx->reqs);
	percpu_ref_exit(&ctx->users);
	kmem_cache_free(kioctx_cachep, ctx);
}

static void free_ioctx_reqs(struct percpu_ref *ref)
{
	struct kioctx *ctx = container_of(ref, struct kioctx, reqs);

	/* At this point we know that there are no any in-flight requests */
	if (ctx->rq_wait && atomic_dec_and_test(&ctx->rq_wait->count))
		complete(&ctx->rq_wait->comp);

	/* Synchronize against RCU protected table->table[] dereferences */
	INIT_RCU_WORK(&ctx->free_rwork, free_ioctx);
	queue_rcu_work(system_wq, &ctx->free_rwork);
}

/*
 * When this function runs, the kioctx has been removed from the "hash table"
 * and ctx->users has dropped to 0, so we know no more kiocbs can be submitted -
 * now it's safe to cancel any that need to be.
 */
static void free_ioctx_users(struct percpu_ref *ref)
{
	struct kioctx *ctx = container_of(ref, struct kioctx, users);
	struct aio_kiocb *req;

	spin_lock_irq(&ctx->ctx_lock);

	while (!list_empty(&ctx->active_reqs)) {
		req = list_first_entry(&ctx->active_reqs,
				       struct aio_kiocb, ki_list);
		req->ki_cancel(&req->rw);
		list_del_init(&req->ki_list);
	}

	spin_unlock_irq(&ctx->ctx_lock);

	percpu_ref_kill(&ctx->reqs);
	percpu_ref_put(&ctx->reqs);
}

static int ioctx_add_table(struct kioctx *ctx, struct mm_struct *mm)
{
	unsigned i, new_nr;
	struct kioctx_table *table, *old;
	struct aio_ring *ring;

	spin_lock(&mm->ioctx_lock);
	table = rcu_dereference_raw(mm->ioctx_table);

	while (1) {
		if (table)
			for (i = 0; i < table->nr; i++)
				if (!rcu_access_pointer(table->table[i])) {
					ctx->id = i;
					rcu_assign_pointer(table->table[i], ctx);
					spin_unlock(&mm->ioctx_lock);

					/* While kioctx setup is in progress,
					 * we are protected from page migration
					 * changes ring_pages by ->ring_lock.
					 */
					ring = page_address(ctx->ring_pages[0]);
					ring->id = ctx->id;
					return 0;
				}

		new_nr = (table ? table->nr : 1) * 4;
		spin_unlock(&mm->ioctx_lock);

		table = kzalloc(struct_size(table, table, new_nr), GFP_KERNEL);
		if (!table)
			return -ENOMEM;

		table->nr = new_nr;

		spin_lock(&mm->ioctx_lock);
		old = rcu_dereference_raw(mm->ioctx_table);

		if (!old) {
			rcu_assign_pointer(mm->ioctx_table, table);
		} else if (table->nr > old->nr) {
			memcpy(table->table, old->table,
			       old->nr * sizeof(struct kioctx *));

			rcu_assign_pointer(mm->ioctx_table, table);
			kfree_rcu(old, rcu);
		} else {
			kfree(table);
			table = old;
		}
	}
}

static void aio_nr_sub(unsigned nr)
{
	spin_lock(&aio_nr_lock);
	if (WARN_ON(aio_nr - nr > aio_nr))
		aio_nr = 0;
	else
		aio_nr -= nr;
	spin_unlock(&aio_nr_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* ioctx_alloc
 *	Allocates and initializes an ioctx.  Returns an ERR_PTR if it failed.
 */
static struct kioctx *ioctx_alloc(unsigned nr_events)
{
<<<<<<< HEAD
	struct mm_struct *mm;
	struct kioctx *ctx;
	int err = -ENOMEM;

	/* Prevent overflows */
	if ((nr_events > (0x10000000U / sizeof(struct io_event))) ||
	    (nr_events > (0x10000000U / sizeof(struct kiocb)))) {
=======
	struct mm_struct *mm = current->mm;
	struct kioctx *ctx;
	int err = -ENOMEM;

	/*
	 * Store the original nr_events -- what userspace passed to io_setup(),
	 * for counting against the global limit -- before it changes.
	 */
	unsigned int max_reqs = nr_events;

	/*
	 * We keep track of the number of available ringbuffer slots, to prevent
	 * overflow (reqs_available), and we also use percpu counters for this.
	 *
	 * So since up to half the slots might be on other cpu's percpu counters
	 * and unavailable, double nr_events so userspace sees what they
	 * expected: additionally, we move req_batch slots to/from percpu
	 * counters at a time, so make sure that isn't 0:
	 */
	nr_events = max(nr_events, num_possible_cpus() * 4);
	nr_events *= 2;

	/* Prevent overflows */
	if (nr_events > (0x10000000U / sizeof(struct io_event))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pr_debug("ENOMEM: nr_events too high\n");
		return ERR_PTR(-EINVAL);
	}

<<<<<<< HEAD
	if (!nr_events || (unsigned long)nr_events > aio_max_nr)
=======
	if (!nr_events || (unsigned long)max_reqs > aio_max_nr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ERR_PTR(-EAGAIN);

	ctx = kmem_cache_zalloc(kioctx_cachep, GFP_KERNEL);
	if (!ctx)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	ctx->max_reqs = nr_events;
	mm = ctx->mm = current->mm;
	atomic_inc(&mm->mm_count);

	atomic_set(&ctx->users, 2);
	spin_lock_init(&ctx->ctx_lock);
	spin_lock_init(&ctx->ring_info.ring_lock);
	init_waitqueue_head(&ctx->wait);

	INIT_LIST_HEAD(&ctx->active_reqs);
	INIT_LIST_HEAD(&ctx->run_list);
	INIT_DELAYED_WORK(&ctx->wq, aio_kick_handler);

	if (aio_setup_ring(ctx) < 0)
		goto out_freectx;

	/* limit the number of system wide aios */
	spin_lock(&aio_nr_lock);
	if (aio_nr + nr_events > aio_max_nr ||
	    aio_nr + nr_events < aio_nr) {
		spin_unlock(&aio_nr_lock);
		goto out_cleanup;
=======
	ctx->max_reqs = max_reqs;

	spin_lock_init(&ctx->ctx_lock);
	spin_lock_init(&ctx->completion_lock);
	mutex_init(&ctx->ring_lock);
	/* Protect against page migration throughout kiotx setup by keeping
	 * the ring_lock mutex held until setup is complete. */
	mutex_lock(&ctx->ring_lock);
	init_waitqueue_head(&ctx->wait);

	INIT_LIST_HEAD(&ctx->active_reqs);

	if (percpu_ref_init(&ctx->users, free_ioctx_users, 0, GFP_KERNEL))
		goto err;

	if (percpu_ref_init(&ctx->reqs, free_ioctx_reqs, 0, GFP_KERNEL))
		goto err;

	ctx->cpu = alloc_percpu(struct kioctx_cpu);
	if (!ctx->cpu)
		goto err;

	err = aio_setup_ring(ctx, nr_events);
	if (err < 0)
		goto err;

	atomic_set(&ctx->reqs_available, ctx->nr_events - 1);
	ctx->req_batch = (ctx->nr_events - 1) / (num_possible_cpus() * 4);
	if (ctx->req_batch < 1)
		ctx->req_batch = 1;

	/* limit the number of system wide aios */
	spin_lock(&aio_nr_lock);
	if (aio_nr + ctx->max_reqs > aio_max_nr ||
	    aio_nr + ctx->max_reqs < aio_nr) {
		spin_unlock(&aio_nr_lock);
		err = -EAGAIN;
		goto err_ctx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	aio_nr += ctx->max_reqs;
	spin_unlock(&aio_nr_lock);

<<<<<<< HEAD
	/* now link into global list. */
	spin_lock(&mm->ioctx_lock);
	hlist_add_head_rcu(&ctx->list, &mm->ioctx_list);
	spin_unlock(&mm->ioctx_lock);

	dprintk("aio: allocated ioctx %p[%ld]: mm=%p mask=0x%x\n",
		ctx, ctx->user_id, current->mm, ctx->ring_info.nr);
	return ctx;

out_cleanup:
	err = -EAGAIN;
	aio_free_ring(ctx);
out_freectx:
	mmdrop(mm);
	kmem_cache_free(kioctx_cachep, ctx);
	dprintk("aio: error allocating ioctx %d\n", err);
	return ERR_PTR(err);
}

/* kill_ctx
 *	Cancels all outstanding aio requests on an aio context.  Used 
 *	when the processes owning a context have all exited to encourage 
 *	the rapid destruction of the kioctx.
 */
static void kill_ctx(struct kioctx *ctx)
{
	int (*cancel)(struct kiocb *, struct io_event *);
	struct task_struct *tsk = current;
	DECLARE_WAITQUEUE(wait, tsk);
	struct io_event res;

	spin_lock_irq(&ctx->ctx_lock);
	ctx->dead = 1;
	while (!list_empty(&ctx->active_reqs)) {
		struct list_head *pos = ctx->active_reqs.next;
		struct kiocb *iocb = list_kiocb(pos);
		list_del_init(&iocb->ki_list);
		cancel = iocb->ki_cancel;
		kiocbSetCancelled(iocb);
		if (cancel) {
			iocb->ki_users++;
			spin_unlock_irq(&ctx->ctx_lock);
			cancel(iocb, &res);
			spin_lock_irq(&ctx->ctx_lock);
		}
	}

	if (!ctx->reqs_active)
		goto out;

	add_wait_queue(&ctx->wait, &wait);
	set_task_state(tsk, TASK_UNINTERRUPTIBLE);
	while (ctx->reqs_active) {
		spin_unlock_irq(&ctx->ctx_lock);
		io_schedule();
		set_task_state(tsk, TASK_UNINTERRUPTIBLE);
		spin_lock_irq(&ctx->ctx_lock);
	}
	__set_task_state(tsk, TASK_RUNNING);
	remove_wait_queue(&ctx->wait, &wait);

out:
	spin_unlock_irq(&ctx->ctx_lock);
}

/* wait_on_sync_kiocb:
 *	Waits on the given sync kiocb to complete.
 */
ssize_t wait_on_sync_kiocb(struct kiocb *iocb)
{
	while (iocb->ki_users) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		if (!iocb->ki_users)
			break;
		io_schedule();
	}
	__set_current_state(TASK_RUNNING);
	return iocb->ki_user_data;
}
EXPORT_SYMBOL(wait_on_sync_kiocb);

/* exit_aio: called when the last user of mm goes away.  At this point, 
 * there is no way for any new requests to be submited or any of the 
 * io_* syscalls to be called on the context.  However, there may be 
 * outstanding requests which hold references to the context; as they 
 * go away, they will call put_ioctx and release any pinned memory
 * associated with the request (held via struct page * references).
 */
void exit_aio(struct mm_struct *mm)
{
	struct kioctx *ctx;

	while (!hlist_empty(&mm->ioctx_list)) {
		ctx = hlist_entry(mm->ioctx_list.first, struct kioctx, list);
		hlist_del_rcu(&ctx->list);

		kill_ctx(ctx);

		if (1 != atomic_read(&ctx->users))
			printk(KERN_DEBUG
				"exit_aio:ioctx still alive: %d %d %d\n",
				atomic_read(&ctx->users), ctx->dead,
				ctx->reqs_active);
		/*
		 * We don't need to bother with munmap() here -
		 * exit_mmap(mm) is coming and it'll unmap everything.
		 * Since aio_free_ring() uses non-zero ->mmap_size
		 * as indicator that it needs to unmap the area,
		 * just set it to 0; aio_free_ring() is the only
		 * place that uses ->mmap_size, so it's safe.
		 * That way we get all munmap done to current->mm -
		 * all other callers have ctx->mm == current->mm.
		 */
		ctx->ring_info.mmap_size = 0;
		put_ioctx(ctx);
	}
}

/* aio_get_req
 *	Allocate a slot for an aio request.  Increments the users count
 * of the kioctx so that the kioctx stays around until all requests are
 * complete.  Returns NULL if no requests are free.
 *
 * Returns with kiocb->users set to 2.  The io submit code path holds
 * an extra reference while submitting the i/o.
 * This prevents races between the aio code path referencing the
 * req (after submitting it) and aio_complete() freeing the req.
 */
static struct kiocb *__aio_get_req(struct kioctx *ctx)
{
	struct kiocb *req = NULL;
=======
	percpu_ref_get(&ctx->users);	/* io_setup() will drop this ref */
	percpu_ref_get(&ctx->reqs);	/* free_ioctx_users() will drop this */

	err = ioctx_add_table(ctx, mm);
	if (err)
		goto err_cleanup;

	/* Release the ring_lock mutex now that all setup is complete. */
	mutex_unlock(&ctx->ring_lock);

	pr_debug("allocated ioctx %p[%ld]: mm=%p mask=0x%x\n",
		 ctx, ctx->user_id, mm, ctx->nr_events);
	return ctx;

err_cleanup:
	aio_nr_sub(ctx->max_reqs);
err_ctx:
	atomic_set(&ctx->dead, 1);
	if (ctx->mmap_size)
		vm_munmap(ctx->mmap_base, ctx->mmap_size);
	aio_free_ring(ctx);
err:
	mutex_unlock(&ctx->ring_lock);
	free_percpu(ctx->cpu);
	percpu_ref_exit(&ctx->reqs);
	percpu_ref_exit(&ctx->users);
	kmem_cache_free(kioctx_cachep, ctx);
	pr_debug("error allocating ioctx %d\n", err);
	return ERR_PTR(err);
}

/* kill_ioctx
 *	Cancels all outstanding aio requests on an aio context.  Used
 *	when the processes owning a context have all exited to encourage
 *	the rapid destruction of the kioctx.
 */
static int kill_ioctx(struct mm_struct *mm, struct kioctx *ctx,
		      struct ctx_rq_wait *wait)
{
	struct kioctx_table *table;

	spin_lock(&mm->ioctx_lock);
	if (atomic_xchg(&ctx->dead, 1)) {
		spin_unlock(&mm->ioctx_lock);
		return -EINVAL;
	}

	table = rcu_dereference_raw(mm->ioctx_table);
	WARN_ON(ctx != rcu_access_pointer(table->table[ctx->id]));
	RCU_INIT_POINTER(table->table[ctx->id], NULL);
	spin_unlock(&mm->ioctx_lock);

	/* free_ioctx_reqs() will do the necessary RCU synchronization */
	wake_up_all(&ctx->wait);

	/*
	 * It'd be more correct to do this in free_ioctx(), after all
	 * the outstanding kiocbs have finished - but by then io_destroy
	 * has already returned, so io_setup() could potentially return
	 * -EAGAIN with no ioctxs actually in use (as far as userspace
	 *  could tell).
	 */
	aio_nr_sub(ctx->max_reqs);

	if (ctx->mmap_size)
		vm_munmap(ctx->mmap_base, ctx->mmap_size);

	ctx->rq_wait = wait;
	percpu_ref_kill(&ctx->users);
	return 0;
}

/*
 * exit_aio: called when the last user of mm goes away.  At this point, there is
 * no way for any new requests to be submited or any of the io_* syscalls to be
 * called on the context.
 *
 * There may be outstanding kiocbs, but free_ioctx() will explicitly wait on
 * them.
 */
void exit_aio(struct mm_struct *mm)
{
	struct kioctx_table *table = rcu_dereference_raw(mm->ioctx_table);
	struct ctx_rq_wait wait;
	int i, skipped;

	if (!table)
		return;

	atomic_set(&wait.count, table->nr);
	init_completion(&wait.comp);

	skipped = 0;
	for (i = 0; i < table->nr; ++i) {
		struct kioctx *ctx =
			rcu_dereference_protected(table->table[i], true);

		if (!ctx) {
			skipped++;
			continue;
		}

		/*
		 * We don't need to bother with munmap() here - exit_mmap(mm)
		 * is coming and it'll unmap everything. And we simply can't,
		 * this is not necessarily our ->mm.
		 * Since kill_ioctx() uses non-zero ->mmap_size as indicator
		 * that it needs to unmap the area, just set it to 0.
		 */
		ctx->mmap_size = 0;
		kill_ioctx(mm, ctx, &wait);
	}

	if (!atomic_sub_and_test(skipped, &wait.count)) {
		/* Wait until all IO for the context are done. */
		wait_for_completion(&wait.comp);
	}

	RCU_INIT_POINTER(mm->ioctx_table, NULL);
	kfree(table);
}

static void put_reqs_available(struct kioctx *ctx, unsigned nr)
{
	struct kioctx_cpu *kcpu;
	unsigned long flags;

	local_irq_save(flags);
	kcpu = this_cpu_ptr(ctx->cpu);
	kcpu->reqs_available += nr;

	while (kcpu->reqs_available >= ctx->req_batch * 2) {
		kcpu->reqs_available -= ctx->req_batch;
		atomic_add(ctx->req_batch, &ctx->reqs_available);
	}

	local_irq_restore(flags);
}

static bool __get_reqs_available(struct kioctx *ctx)
{
	struct kioctx_cpu *kcpu;
	bool ret = false;
	unsigned long flags;

	local_irq_save(flags);
	kcpu = this_cpu_ptr(ctx->cpu);
	if (!kcpu->reqs_available) {
		int avail = atomic_read(&ctx->reqs_available);

		do {
			if (avail < ctx->req_batch)
				goto out;
		} while (!atomic_try_cmpxchg(&ctx->reqs_available,
					     &avail, avail - ctx->req_batch));

		kcpu->reqs_available += ctx->req_batch;
	}

	ret = true;
	kcpu->reqs_available--;
out:
	local_irq_restore(flags);
	return ret;
}

/* refill_reqs_available
 *	Updates the reqs_available reference counts used for tracking the
 *	number of free slots in the completion ring.  This can be called
 *	from aio_complete() (to optimistically update reqs_available) or
 *	from aio_get_req() (the we're out of events case).  It must be
 *	called holding ctx->completion_lock.
 */
static void refill_reqs_available(struct kioctx *ctx, unsigned head,
                                  unsigned tail)
{
	unsigned events_in_ring, completed;

	/* Clamp head since userland can write to it. */
	head %= ctx->nr_events;
	if (head <= tail)
		events_in_ring = tail - head;
	else
		events_in_ring = ctx->nr_events - (head - tail);

	completed = ctx->completed_events;
	if (events_in_ring < completed)
		completed -= events_in_ring;
	else
		completed = 0;

	if (!completed)
		return;

	ctx->completed_events -= completed;
	put_reqs_available(ctx, completed);
}

/* user_refill_reqs_available
 *	Called to refill reqs_available when aio_get_req() encounters an
 *	out of space in the completion ring.
 */
static void user_refill_reqs_available(struct kioctx *ctx)
{
	spin_lock_irq(&ctx->completion_lock);
	if (ctx->completed_events) {
		struct aio_ring *ring;
		unsigned head;

		/* Access of ring->head may race with aio_read_events_ring()
		 * here, but that's okay since whether we read the old version
		 * or the new version, and either will be valid.  The important
		 * part is that head cannot pass tail since we prevent
		 * aio_complete() from updating tail by holding
		 * ctx->completion_lock.  Even if head is invalid, the check
		 * against ctx->completed_events below will make sure we do the
		 * safe/right thing.
		 */
		ring = page_address(ctx->ring_pages[0]);
		head = ring->head;

		refill_reqs_available(ctx, head, ctx->tail);
	}

	spin_unlock_irq(&ctx->completion_lock);
}

static bool get_reqs_available(struct kioctx *ctx)
{
	if (__get_reqs_available(ctx))
		return true;
	user_refill_reqs_available(ctx);
	return __get_reqs_available(ctx);
}

/* aio_get_req
 *	Allocate a slot for an aio request.
 * Returns NULL if no requests are free.
 *
 * The refcount is initialized to 2 - one for the async op completion,
 * one for the synchronous code that does this.
 */
static inline struct aio_kiocb *aio_get_req(struct kioctx *ctx)
{
	struct aio_kiocb *req;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	req = kmem_cache_alloc(kiocb_cachep, GFP_KERNEL);
	if (unlikely(!req))
		return NULL;

<<<<<<< HEAD
	req->ki_flags = 0;
	req->ki_users = 2;
	req->ki_key = 0;
	req->ki_ctx = ctx;
	req->ki_cancel = NULL;
	req->ki_retry = NULL;
	req->ki_dtor = NULL;
	req->private = NULL;
	req->ki_iovec = NULL;
	INIT_LIST_HEAD(&req->ki_run_list);
	req->ki_eventfd = NULL;

	return req;
}

/*
 * struct kiocb's are allocated in batches to reduce the number of
 * times the ctx lock is acquired and released.
 */
#define KIOCB_BATCH_SIZE	32L
struct kiocb_batch {
	struct list_head head;
	long count; /* number of requests left to allocate */
};

static void kiocb_batch_init(struct kiocb_batch *batch, long total)
{
	INIT_LIST_HEAD(&batch->head);
	batch->count = total;
}

static void kiocb_batch_free(struct kioctx *ctx, struct kiocb_batch *batch)
{
	struct kiocb *req, *n;

	if (list_empty(&batch->head))
		return;

	spin_lock_irq(&ctx->ctx_lock);
	list_for_each_entry_safe(req, n, &batch->head, ki_batch) {
		list_del(&req->ki_batch);
		list_del(&req->ki_list);
		kmem_cache_free(kiocb_cachep, req);
		ctx->reqs_active--;
	}
	if (unlikely(!ctx->reqs_active && ctx->dead))
		wake_up_all(&ctx->wait);
	spin_unlock_irq(&ctx->ctx_lock);
}

/*
 * Allocate a batch of kiocbs.  This avoids taking and dropping the
 * context lock a lot during setup.
 */
static int kiocb_batch_refill(struct kioctx *ctx, struct kiocb_batch *batch)
{
	unsigned short allocated, to_alloc;
	long avail;
	bool called_fput = false;
	struct kiocb *req, *n;
	struct aio_ring *ring;

	to_alloc = min(batch->count, KIOCB_BATCH_SIZE);
	for (allocated = 0; allocated < to_alloc; allocated++) {
		req = __aio_get_req(ctx);
		if (!req)
			/* allocation failed, go with what we've got */
			break;
		list_add(&req->ki_batch, &batch->head);
	}

	if (allocated == 0)
		goto out;

retry:
	spin_lock_irq(&ctx->ctx_lock);
	ring = kmap_atomic(ctx->ring_info.ring_pages[0]);

	avail = aio_ring_avail(&ctx->ring_info, ring) - ctx->reqs_active;
	BUG_ON(avail < 0);
	if (avail == 0 && !called_fput) {
		/*
		 * Handle a potential starvation case.  It is possible that
		 * we hold the last reference on a struct file, causing us
		 * to delay the final fput to non-irq context.  In this case,
		 * ctx->reqs_active is artificially high.  Calling the fput
		 * routine here may free up a slot in the event completion
		 * ring, allowing this allocation to succeed.
		 */
		kunmap_atomic(ring);
		spin_unlock_irq(&ctx->ctx_lock);
		aio_fput_routine(NULL);
		called_fput = true;
		goto retry;
	}

	if (avail < allocated) {
		/* Trim back the number of requests. */
		list_for_each_entry_safe(req, n, &batch->head, ki_batch) {
			list_del(&req->ki_batch);
			kmem_cache_free(kiocb_cachep, req);
			if (--allocated <= avail)
				break;
		}
	}

	batch->count -= allocated;
	list_for_each_entry(req, &batch->head, ki_batch) {
		list_add(&req->ki_list, &ctx->active_reqs);
		ctx->reqs_active++;
	}

	kunmap_atomic(ring);
	spin_unlock_irq(&ctx->ctx_lock);

out:
	return allocated;
}

static inline struct kiocb *aio_get_req(struct kioctx *ctx,
					struct kiocb_batch *batch)
{
	struct kiocb *req;

	if (list_empty(&batch->head))
		if (kiocb_batch_refill(ctx, batch) == 0)
			return NULL;
	req = list_first_entry(&batch->head, struct kiocb, ki_batch);
	list_del(&req->ki_batch);
	return req;
}

static inline void really_put_req(struct kioctx *ctx, struct kiocb *req)
{
	assert_spin_locked(&ctx->ctx_lock);

	if (req->ki_eventfd != NULL)
		eventfd_ctx_put(req->ki_eventfd);
	if (req->ki_dtor)
		req->ki_dtor(req);
	if (req->ki_iovec != &req->ki_inline_vec)
		kfree(req->ki_iovec);
	kmem_cache_free(kiocb_cachep, req);
	ctx->reqs_active--;

	if (unlikely(!ctx->reqs_active && ctx->dead))
		wake_up_all(&ctx->wait);
}

static void aio_fput_routine(struct work_struct *data)
{
	spin_lock_irq(&fput_lock);
	while (likely(!list_empty(&fput_head))) {
		struct kiocb *req = list_kiocb(fput_head.next);
		struct kioctx *ctx = req->ki_ctx;

		list_del(&req->ki_list);
		spin_unlock_irq(&fput_lock);

		/* Complete the fput(s) */
		if (req->ki_filp != NULL)
			fput(req->ki_filp);

		/* Link the iocb into the context's free list */
		rcu_read_lock();
		spin_lock_irq(&ctx->ctx_lock);
		really_put_req(ctx, req);
		/*
		 * at that point ctx might've been killed, but actual
		 * freeing is RCU'd
		 */
		spin_unlock_irq(&ctx->ctx_lock);
		rcu_read_unlock();

		spin_lock_irq(&fput_lock);
	}
	spin_unlock_irq(&fput_lock);
}

/* __aio_put_req
 *	Returns true if this put was the last user of the request.
 */
static int __aio_put_req(struct kioctx *ctx, struct kiocb *req)
{
	dprintk(KERN_DEBUG "aio_put(%p): f_count=%ld\n",
		req, atomic_long_read(&req->ki_filp->f_count));

	assert_spin_locked(&ctx->ctx_lock);

	req->ki_users--;
	BUG_ON(req->ki_users < 0);
	if (likely(req->ki_users))
		return 0;
	list_del(&req->ki_list);		/* remove from active_reqs */
	req->ki_cancel = NULL;
	req->ki_retry = NULL;

	/*
	 * Try to optimize the aio and eventfd file* puts, by avoiding to
	 * schedule work in case it is not final fput() time. In normal cases,
	 * we would not be holding the last reference to the file*, so
	 * this function will be executed w/out any aio kthread wakeup.
	 */
	if (unlikely(!fput_atomic(req->ki_filp))) {
		spin_lock(&fput_lock);
		list_add(&req->ki_list, &fput_head);
		spin_unlock(&fput_lock);
		schedule_work(&fput_work);
	} else {
		req->ki_filp = NULL;
		really_put_req(ctx, req);
	}
	return 1;
}

/* aio_put_req
 *	Returns true if this put was the last user of the kiocb,
 *	false if the request is still in use.
 */
int aio_put_req(struct kiocb *req)
{
	struct kioctx *ctx = req->ki_ctx;
	int ret;
	spin_lock_irq(&ctx->ctx_lock);
	ret = __aio_put_req(ctx, req);
	spin_unlock_irq(&ctx->ctx_lock);
	return ret;
}
EXPORT_SYMBOL(aio_put_req);

static struct kioctx *lookup_ioctx(unsigned long ctx_id)
{
	struct mm_struct *mm = current->mm;
	struct kioctx *ctx, *ret = NULL;
	struct hlist_node *n;

	rcu_read_lock();

	hlist_for_each_entry_rcu(ctx, n, &mm->ioctx_list, list) {
		/*
		 * RCU protects us against accessing freed memory but
		 * we have to be careful not to get a reference when the
		 * reference count already dropped to 0 (ctx->dead test
		 * is unreliable because of races).
		 */
		if (ctx->user_id == ctx_id && !ctx->dead && try_get_ioctx(ctx)){
			ret = ctx;
			break;
		}
	}

=======
	if (unlikely(!get_reqs_available(ctx))) {
		kmem_cache_free(kiocb_cachep, req);
		return NULL;
	}

	percpu_ref_get(&ctx->reqs);
	req->ki_ctx = ctx;
	INIT_LIST_HEAD(&req->ki_list);
	refcount_set(&req->ki_refcnt, 2);
	req->ki_eventfd = NULL;
	return req;
}

static struct kioctx *lookup_ioctx(unsigned long ctx_id)
{
	struct aio_ring __user *ring  = (void __user *)ctx_id;
	struct mm_struct *mm = current->mm;
	struct kioctx *ctx, *ret = NULL;
	struct kioctx_table *table;
	unsigned id;

	if (get_user(id, &ring->id))
		return NULL;

	rcu_read_lock();
	table = rcu_dereference(mm->ioctx_table);

	if (!table || id >= table->nr)
		goto out;

	id = array_index_nospec(id, table->nr);
	ctx = rcu_dereference(table->table[id]);
	if (ctx && ctx->user_id == ctx_id) {
		if (percpu_ref_tryget_live(&ctx->users))
			ret = ctx;
	}
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();
	return ret;
}

<<<<<<< HEAD
/*
 * Queue up a kiocb to be retried. Assumes that the kiocb
 * has already been marked as kicked, and places it on
 * the retry run list for the corresponding ioctx, if it
 * isn't already queued. Returns 1 if it actually queued
 * the kiocb (to tell the caller to activate the work
 * queue to process it), or 0, if it found that it was
 * already queued.
 */
static inline int __queue_kicked_iocb(struct kiocb *iocb)
{
	struct kioctx *ctx = iocb->ki_ctx;

	assert_spin_locked(&ctx->ctx_lock);

	if (list_empty(&iocb->ki_run_list)) {
		list_add_tail(&iocb->ki_run_list,
			&ctx->run_list);
		return 1;
	}
	return 0;
}

/* aio_run_iocb
 *	This is the core aio execution routine. It is
 *	invoked both for initial i/o submission and
 *	subsequent retries via the aio_kick_handler.
 *	Expects to be invoked with iocb->ki_ctx->lock
 *	already held. The lock is released and reacquired
 *	as needed during processing.
 *
 * Calls the iocb retry method (already setup for the
 * iocb on initial submission) for operation specific
 * handling, but takes care of most of common retry
 * execution details for a given iocb. The retry method
 * needs to be non-blocking as far as possible, to avoid
 * holding up other iocbs waiting to be serviced by the
 * retry kernel thread.
 *
 * The trickier parts in this code have to do with
 * ensuring that only one retry instance is in progress
 * for a given iocb at any time. Providing that guarantee
 * simplifies the coding of individual aio operations as
 * it avoids various potential races.
 */
static ssize_t aio_run_iocb(struct kiocb *iocb)
{
	struct kioctx	*ctx = iocb->ki_ctx;
	ssize_t (*retry)(struct kiocb *);
	ssize_t ret;

	if (!(retry = iocb->ki_retry)) {
		printk("aio_run_iocb: iocb->ki_retry = NULL\n");
		return 0;
	}

	/*
	 * We don't want the next retry iteration for this
	 * operation to start until this one has returned and
	 * updated the iocb state. However, wait_queue functions
	 * can trigger a kick_iocb from interrupt context in the
	 * meantime, indicating that data is available for the next
	 * iteration. We want to remember that and enable the
	 * next retry iteration _after_ we are through with
	 * this one.
	 *
	 * So, in order to be able to register a "kick", but
	 * prevent it from being queued now, we clear the kick
	 * flag, but make the kick code *think* that the iocb is
	 * still on the run list until we are actually done.
	 * When we are done with this iteration, we check if
	 * the iocb was kicked in the meantime and if so, queue
	 * it up afresh.
	 */

	kiocbClearKicked(iocb);

	/*
	 * This is so that aio_complete knows it doesn't need to
	 * pull the iocb off the run list (We can't just call
	 * INIT_LIST_HEAD because we don't want a kick_iocb to
	 * queue this on the run list yet)
	 */
	iocb->ki_run_list.next = iocb->ki_run_list.prev = NULL;
	spin_unlock_irq(&ctx->ctx_lock);

	/* Quit retrying if the i/o has been cancelled */
	if (kiocbIsCancelled(iocb)) {
		ret = -EINTR;
		aio_complete(iocb, ret, 0);
		/* must not access the iocb after this */
		goto out;
	}

	/*
	 * Now we are all set to call the retry method in async
	 * context.
	 */
	ret = retry(iocb);

	if (ret != -EIOCBRETRY && ret != -EIOCBQUEUED) {
		/*
		 * There's no easy way to restart the syscall since other AIO's
		 * may be already running. Just fail this IO with EINTR.
		 */
		if (unlikely(ret == -ERESTARTSYS || ret == -ERESTARTNOINTR ||
			     ret == -ERESTARTNOHAND || ret == -ERESTART_RESTARTBLOCK))
			ret = -EINTR;
		aio_complete(iocb, ret, 0);
	}
out:
	spin_lock_irq(&ctx->ctx_lock);

	if (-EIOCBRETRY == ret) {
		/*
		 * OK, now that we are done with this iteration
		 * and know that there is more left to go,
		 * this is where we let go so that a subsequent
		 * "kick" can start the next iteration
		 */

		/* will make __queue_kicked_iocb succeed from here on */
		INIT_LIST_HEAD(&iocb->ki_run_list);
		/* we must queue the next iteration ourselves, if it
		 * has already been kicked */
		if (kiocbIsKicked(iocb)) {
			__queue_kicked_iocb(iocb);

			/*
			 * __queue_kicked_iocb will always return 1 here, because
			 * iocb->ki_run_list is empty at this point so it should
			 * be safe to unconditionally queue the context into the
			 * work queue.
			 */
			aio_queue_work(ctx);
		}
	}
	return ret;
}

/*
 * __aio_run_iocbs:
 * 	Process all pending retries queued on the ioctx
 * 	run list.
 * Assumes it is operating within the aio issuer's mm
 * context.
 */
static int __aio_run_iocbs(struct kioctx *ctx)
{
	struct kiocb *iocb;
	struct list_head run_list;

	assert_spin_locked(&ctx->ctx_lock);

	list_replace_init(&ctx->run_list, &run_list);
	while (!list_empty(&run_list)) {
		iocb = list_entry(run_list.next, struct kiocb,
			ki_run_list);
		list_del(&iocb->ki_run_list);
		/*
		 * Hold an extra reference while retrying i/o.
		 */
		iocb->ki_users++;       /* grab extra reference */
		aio_run_iocb(iocb);
		__aio_put_req(ctx, iocb);
 	}
	if (!list_empty(&ctx->run_list))
		return 1;
	return 0;
}

static void aio_queue_work(struct kioctx * ctx)
{
	unsigned long timeout;
	/*
	 * if someone is waiting, get the work started right
	 * away, otherwise, use a longer delay
	 */
	smp_mb();
	if (waitqueue_active(&ctx->wait))
		timeout = 1;
	else
		timeout = HZ/10;
	queue_delayed_work(aio_wq, &ctx->wq, timeout);
}

/*
 * aio_run_all_iocbs:
 *	Process all pending retries queued on the ioctx
 *	run list, and keep running them until the list
 *	stays empty.
 * Assumes it is operating within the aio issuer's mm context.
 */
static inline void aio_run_all_iocbs(struct kioctx *ctx)
{
	spin_lock_irq(&ctx->ctx_lock);
	while (__aio_run_iocbs(ctx))
		;
	spin_unlock_irq(&ctx->ctx_lock);
}

/*
 * aio_kick_handler:
 * 	Work queue handler triggered to process pending
 * 	retries on an ioctx. Takes on the aio issuer's
 *	mm context before running the iocbs, so that
 *	copy_xxx_user operates on the issuer's address
 *      space.
 * Run on aiod's context.
 */
static void aio_kick_handler(struct work_struct *work)
{
	struct kioctx *ctx = container_of(work, struct kioctx, wq.work);
	mm_segment_t oldfs = get_fs();
	struct mm_struct *mm;
	int requeue;

	set_fs(USER_DS);
	use_mm(ctx->mm);
	spin_lock_irq(&ctx->ctx_lock);
	requeue =__aio_run_iocbs(ctx);
	mm = ctx->mm;
	spin_unlock_irq(&ctx->ctx_lock);
 	unuse_mm(mm);
	set_fs(oldfs);
	/*
	 * we're in a worker thread already; no point using non-zero delay
	 */
	if (requeue)
		queue_delayed_work(aio_wq, &ctx->wq, 0);
}


/*
 * Called by kick_iocb to queue the kiocb for retry
 * and if required activate the aio work queue to process
 * it
 */
static void try_queue_kicked_iocb(struct kiocb *iocb)
{
 	struct kioctx	*ctx = iocb->ki_ctx;
	unsigned long flags;
	int run = 0;

	spin_lock_irqsave(&ctx->ctx_lock, flags);
	/* set this inside the lock so that we can't race with aio_run_iocb()
	 * testing it and putting the iocb on the run list under the lock */
	if (!kiocbTryKick(iocb))
		run = __queue_kicked_iocb(iocb);
	spin_unlock_irqrestore(&ctx->ctx_lock, flags);
	if (run)
		aio_queue_work(ctx);
}

/*
 * kick_iocb:
 *      Called typically from a wait queue callback context
 *      to trigger a retry of the iocb.
 *      The retry is usually executed by aio workqueue
 *      threads (See aio_kick_handler).
 */
void kick_iocb(struct kiocb *iocb)
{
	/* sync iocbs are easy: they can only ever be executing from a 
	 * single context. */
	if (is_sync_kiocb(iocb)) {
		kiocbSetKicked(iocb);
	        wake_up_process(iocb->ki_obj.tsk);
		return;
	}

	try_queue_kicked_iocb(iocb);
}
EXPORT_SYMBOL(kick_iocb);

/* aio_complete
 *	Called when the io request on the given iocb is complete.
 *	Returns true if this is the last user of the request.  The 
 *	only other user of the request can be the cancellation code.
 */
int aio_complete(struct kiocb *iocb, long res, long res2)
{
	struct kioctx	*ctx = iocb->ki_ctx;
	struct aio_ring_info	*info;
	struct aio_ring	*ring;
	struct io_event	*event;
	unsigned long	flags;
	unsigned long	tail;
	int		ret;

	/*
	 * Special case handling for sync iocbs:
	 *  - events go directly into the iocb for fast handling
	 *  - the sync task with the iocb in its stack holds the single iocb
	 *    ref, no other paths have a way to get another ref
	 *  - the sync task helpfully left a reference to itself in the iocb
	 */
	if (is_sync_kiocb(iocb)) {
		BUG_ON(iocb->ki_users != 1);
		iocb->ki_user_data = res;
		iocb->ki_users = 0;
		wake_up_process(iocb->ki_obj.tsk);
		return 1;
	}

	info = &ctx->ring_info;

	/* add a completion event to the ring buffer.
	 * must be done holding ctx->ctx_lock to prevent
	 * other code from messing with the tail
	 * pointer since we might be called from irq
	 * context.
	 */
	spin_lock_irqsave(&ctx->ctx_lock, flags);

	if (iocb->ki_run_list.prev && !list_empty(&iocb->ki_run_list))
		list_del_init(&iocb->ki_run_list);

	/*
	 * cancelled requests don't get events, userland was given one
	 * when the event got cancelled.
	 */
	if (kiocbIsCancelled(iocb))
		goto put_rq;

	ring = kmap_atomic(info->ring_pages[0]);

	tail = info->tail;
	event = aio_ring_event(info, tail);
	if (++tail >= info->nr)
		tail = 0;

	event->obj = (u64)(unsigned long)iocb->ki_obj.user;
	event->data = iocb->ki_user_data;
	event->res = res;
	event->res2 = res2;

	dprintk("aio_complete: %p[%lu]: %p: %p %Lx %lx %lx\n",
		ctx, tail, iocb, iocb->ki_obj.user, iocb->ki_user_data,
		res, res2);
=======
static inline void iocb_destroy(struct aio_kiocb *iocb)
{
	if (iocb->ki_eventfd)
		eventfd_ctx_put(iocb->ki_eventfd);
	if (iocb->ki_filp)
		fput(iocb->ki_filp);
	percpu_ref_put(&iocb->ki_ctx->reqs);
	kmem_cache_free(kiocb_cachep, iocb);
}

struct aio_waiter {
	struct wait_queue_entry	w;
	size_t			min_nr;
};

/* aio_complete
 *	Called when the io request on the given iocb is complete.
 */
static void aio_complete(struct aio_kiocb *iocb)
{
	struct kioctx	*ctx = iocb->ki_ctx;
	struct aio_ring	*ring;
	struct io_event	*ev_page, *event;
	unsigned tail, pos, head, avail;
	unsigned long	flags;

	/*
	 * Add a completion event to the ring buffer. Must be done holding
	 * ctx->completion_lock to prevent other code from messing with the tail
	 * pointer since we might be called from irq context.
	 */
	spin_lock_irqsave(&ctx->completion_lock, flags);

	tail = ctx->tail;
	pos = tail + AIO_EVENTS_OFFSET;

	if (++tail >= ctx->nr_events)
		tail = 0;

	ev_page = page_address(ctx->ring_pages[pos / AIO_EVENTS_PER_PAGE]);
	event = ev_page + pos % AIO_EVENTS_PER_PAGE;

	*event = iocb->ki_res;

	flush_dcache_page(ctx->ring_pages[pos / AIO_EVENTS_PER_PAGE]);

	pr_debug("%p[%u]: %p: %p %Lx %Lx %Lx\n", ctx, tail, iocb,
		 (void __user *)(unsigned long)iocb->ki_res.obj,
		 iocb->ki_res.data, iocb->ki_res.res, iocb->ki_res.res2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* after flagging the request as done, we
	 * must never even look at it again
	 */
	smp_wmb();	/* make event visible before updating tail */

<<<<<<< HEAD
	info->tail = tail;
	ring->tail = tail;

	put_aio_ring_event(event);
	kunmap_atomic(ring);

	pr_debug("added to ring %p at [%lu]\n", iocb, tail);
=======
	ctx->tail = tail;

	ring = page_address(ctx->ring_pages[0]);
	head = ring->head;
	ring->tail = tail;
	flush_dcache_page(ctx->ring_pages[0]);

	ctx->completed_events++;
	if (ctx->completed_events > 1)
		refill_reqs_available(ctx, head, tail);

	avail = tail > head
		? tail - head
		: tail + ctx->nr_events - head;
	spin_unlock_irqrestore(&ctx->completion_lock, flags);

	pr_debug("added to ring %p at [%u]\n", iocb, tail);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Check if the user asked us to deliver the result through an
	 * eventfd. The eventfd_signal() function is safe to be called
	 * from IRQ context.
	 */
<<<<<<< HEAD
	if (iocb->ki_eventfd != NULL)
		eventfd_signal(iocb->ki_eventfd, 1);

put_rq:
	/* everything turned out well, dispose of the aiocb. */
	ret = __aio_put_req(ctx, iocb);
=======
	if (iocb->ki_eventfd)
		eventfd_signal(iocb->ki_eventfd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * We have to order our ring_info tail store above and test
	 * of the wait list below outside the wait lock.  This is
	 * like in wake_up_bit() where clearing a bit has to be
	 * ordered with the unlocked test.
	 */
	smp_mb();

<<<<<<< HEAD
	if (waitqueue_active(&ctx->wait))
		wake_up(&ctx->wait);

	spin_unlock_irqrestore(&ctx->ctx_lock, flags);
	return ret;
}
EXPORT_SYMBOL(aio_complete);

/* aio_read_evt
 *	Pull an event off of the ioctx's event ring.  Returns the number of 
 *	events fetched (0 or 1 ;-)
 *	FIXME: make this use cmpxchg.
 *	TODO: make the ringbuffer user mmap()able (requires FIXME).
 */
static int aio_read_evt(struct kioctx *ioctx, struct io_event *ent)
{
	struct aio_ring_info *info = &ioctx->ring_info;
	struct aio_ring *ring;
	unsigned long head;
	int ret = 0;

	ring = kmap_atomic(info->ring_pages[0]);
	dprintk("in aio_read_evt h%lu t%lu m%lu\n",
		 (unsigned long)ring->head, (unsigned long)ring->tail,
		 (unsigned long)ring->nr);

	if (ring->head == ring->tail)
		goto out;

	spin_lock(&info->ring_lock);

	head = ring->head % info->nr;
	if (head != ring->tail) {
		struct io_event *evp = aio_ring_event(info, head);
		*ent = *evp;
		head = (head + 1) % info->nr;
		smp_mb(); /* finish reading the event before updatng the head */
		ring->head = head;
		ret = 1;
		put_aio_ring_event(evp);
	}
	spin_unlock(&info->ring_lock);

out:
	dprintk("leaving aio_read_evt: %d  h%lu t%lu\n", ret,
		 (unsigned long)ring->head, (unsigned long)ring->tail);
	kunmap_atomic(ring);
	return ret;
}

struct aio_timeout {
	struct timer_list	timer;
	int			timed_out;
	struct task_struct	*p;
};

static void timeout_func(unsigned long data)
{
	struct aio_timeout *to = (struct aio_timeout *)data;

	to->timed_out = 1;
	wake_up_process(to->p);
}

static inline void init_timeout(struct aio_timeout *to)
{
	setup_timer_on_stack(&to->timer, timeout_func, (unsigned long) to);
	to->timed_out = 0;
	to->p = current;
}

static inline void set_timeout(long start_jiffies, struct aio_timeout *to,
			       const struct timespec *ts)
{
	to->timer.expires = start_jiffies + timespec_to_jiffies(ts);
	if (time_after(to->timer.expires, jiffies))
		add_timer(&to->timer);
	else
		to->timed_out = 1;
}

static inline void clear_timeout(struct aio_timeout *to)
{
	del_singleshot_timer_sync(&to->timer);
}

static int read_events(struct kioctx *ctx,
			long min_nr, long nr,
			struct io_event __user *event,
			struct timespec __user *timeout)
{
	long			start_jiffies = jiffies;
	struct task_struct	*tsk = current;
	DECLARE_WAITQUEUE(wait, tsk);
	int			ret;
	int			i = 0;
	struct io_event		ent;
	struct aio_timeout	to;
	int			retry = 0;

	/* needed to zero any padding within an entry (there shouldn't be 
	 * any, but C is fun!
	 */
	memset(&ent, 0, sizeof(ent));
retry:
	ret = 0;
	while (likely(i < nr)) {
		ret = aio_read_evt(ctx, &ent);
		if (unlikely(ret <= 0))
			break;

		dprintk("read event: %Lx %Lx %Lx %Lx\n",
			ent.data, ent.obj, ent.res, ent.res2);

		/* Could we split the check in two? */
		ret = -EFAULT;
		if (unlikely(copy_to_user(event, &ent, sizeof(ent)))) {
			dprintk("aio: lost an event due to EFAULT.\n");
			break;
		}
		ret = 0;

		/* Good, event copied to userland, update counts. */
		event ++;
		i ++;
	}

	if (min_nr <= i)
		return i;
	if (ret)
		return ret;

	/* End fast path */

	/* racey check, but it gets redone */
	if (!retry && unlikely(!list_empty(&ctx->run_list))) {
		retry = 1;
		aio_run_all_iocbs(ctx);
		goto retry;
	}

	init_timeout(&to);
	if (timeout) {
		struct timespec	ts;
		ret = -EFAULT;
		if (unlikely(copy_from_user(&ts, timeout, sizeof(ts))))
			goto out;

		set_timeout(start_jiffies, &to, &ts);
	}

	while (likely(i < nr)) {
		add_wait_queue_exclusive(&ctx->wait, &wait);
		do {
			set_task_state(tsk, TASK_INTERRUPTIBLE);
			ret = aio_read_evt(ctx, &ent);
			if (ret)
				break;
			if (min_nr <= i)
				break;
			if (unlikely(ctx->dead)) {
				ret = -EINVAL;
				break;
			}
			if (to.timed_out)	/* Only check after read evt */
				break;
			/* Try to only show up in io wait if there are ops
			 *  in flight */
			if (ctx->reqs_active)
				io_schedule();
			else
				schedule();
			if (signal_pending(tsk)) {
				ret = -EINTR;
				break;
			}
			/*ret = aio_read_evt(ctx, &ent);*/
		} while (1) ;

		set_task_state(tsk, TASK_RUNNING);
		remove_wait_queue(&ctx->wait, &wait);

		if (unlikely(ret <= 0))
			break;

		ret = -EFAULT;
		if (unlikely(copy_to_user(event, &ent, sizeof(ent)))) {
			dprintk("aio: lost an event due to EFAULT.\n");
			break;
		}

		/* Good, event copied to userland, update counts. */
		event ++;
		i ++;
	}

	if (timeout)
		clear_timeout(&to);
out:
	destroy_timer_on_stack(&to.timer);
	return i ? i : ret;
}

/* Take an ioctx and remove it from the list of ioctx's.  Protects 
 * against races with itself via ->dead.
 */
static void io_destroy(struct kioctx *ioctx)
{
	struct mm_struct *mm = current->mm;
	int was_dead;

	/* delete the entry from the list is someone else hasn't already */
	spin_lock(&mm->ioctx_lock);
	was_dead = ioctx->dead;
	ioctx->dead = 1;
	hlist_del_rcu(&ioctx->list);
	spin_unlock(&mm->ioctx_lock);

	dprintk("aio_release(%p)\n", ioctx);
	if (likely(!was_dead))
		put_ioctx(ioctx);	/* twice for the list */

	kill_ctx(ioctx);

	/*
	 * Wake up any waiters.  The setting of ctx->dead must be seen
	 * by other CPUs at this point.  Right now, we rely on the
	 * locking done by the above calls to ensure this consistency.
	 */
	wake_up_all(&ioctx->wait);
=======
	if (waitqueue_active(&ctx->wait)) {
		struct aio_waiter *curr, *next;
		unsigned long flags;

		spin_lock_irqsave(&ctx->wait.lock, flags);
		list_for_each_entry_safe(curr, next, &ctx->wait.head, w.entry)
			if (avail >= curr->min_nr) {
				wake_up_process(curr->w.private);
				list_del_init_careful(&curr->w.entry);
			}
		spin_unlock_irqrestore(&ctx->wait.lock, flags);
	}
}

static inline void iocb_put(struct aio_kiocb *iocb)
{
	if (refcount_dec_and_test(&iocb->ki_refcnt)) {
		aio_complete(iocb);
		iocb_destroy(iocb);
	}
}

/* aio_read_events_ring
 *	Pull an event off of the ioctx's event ring.  Returns the number of
 *	events fetched
 */
static long aio_read_events_ring(struct kioctx *ctx,
				 struct io_event __user *event, long nr)
{
	struct aio_ring *ring;
	unsigned head, tail, pos;
	long ret = 0;
	int copy_ret;

	/*
	 * The mutex can block and wake us up and that will cause
	 * wait_event_interruptible_hrtimeout() to schedule without sleeping
	 * and repeat. This should be rare enough that it doesn't cause
	 * peformance issues. See the comment in read_events() for more detail.
	 */
	sched_annotate_sleep();
	mutex_lock(&ctx->ring_lock);

	/* Access to ->ring_pages here is protected by ctx->ring_lock. */
	ring = page_address(ctx->ring_pages[0]);
	head = ring->head;
	tail = ring->tail;

	/*
	 * Ensure that once we've read the current tail pointer, that
	 * we also see the events that were stored up to the tail.
	 */
	smp_rmb();

	pr_debug("h%u t%u m%u\n", head, tail, ctx->nr_events);

	if (head == tail)
		goto out;

	head %= ctx->nr_events;
	tail %= ctx->nr_events;

	while (ret < nr) {
		long avail;
		struct io_event *ev;
		struct page *page;

		avail = (head <= tail ?  tail : ctx->nr_events) - head;
		if (head == tail)
			break;

		pos = head + AIO_EVENTS_OFFSET;
		page = ctx->ring_pages[pos / AIO_EVENTS_PER_PAGE];
		pos %= AIO_EVENTS_PER_PAGE;

		avail = min(avail, nr - ret);
		avail = min_t(long, avail, AIO_EVENTS_PER_PAGE - pos);

		ev = page_address(page);
		copy_ret = copy_to_user(event + ret, ev + pos,
					sizeof(*ev) * avail);

		if (unlikely(copy_ret)) {
			ret = -EFAULT;
			goto out;
		}

		ret += avail;
		head += avail;
		head %= ctx->nr_events;
	}

	ring = page_address(ctx->ring_pages[0]);
	ring->head = head;
	flush_dcache_page(ctx->ring_pages[0]);

	pr_debug("%li  h%u t%u\n", ret, head, tail);
out:
	mutex_unlock(&ctx->ring_lock);

	return ret;
}

static bool aio_read_events(struct kioctx *ctx, long min_nr, long nr,
			    struct io_event __user *event, long *i)
{
	long ret = aio_read_events_ring(ctx, event + *i, nr - *i);

	if (ret > 0)
		*i += ret;

	if (unlikely(atomic_read(&ctx->dead)))
		ret = -EINVAL;

	if (!*i)
		*i = ret;

	return ret < 0 || *i >= min_nr;
}

static long read_events(struct kioctx *ctx, long min_nr, long nr,
			struct io_event __user *event,
			ktime_t until)
{
	struct hrtimer_sleeper	t;
	struct aio_waiter	w;
	long ret = 0, ret2 = 0;

	/*
	 * Note that aio_read_events() is being called as the conditional - i.e.
	 * we're calling it after prepare_to_wait() has set task state to
	 * TASK_INTERRUPTIBLE.
	 *
	 * But aio_read_events() can block, and if it blocks it's going to flip
	 * the task state back to TASK_RUNNING.
	 *
	 * This should be ok, provided it doesn't flip the state back to
	 * TASK_RUNNING and return 0 too much - that causes us to spin. That
	 * will only happen if the mutex_lock() call blocks, and we then find
	 * the ringbuffer empty. So in practice we should be ok, but it's
	 * something to be aware of when touching this code.
	 */
	aio_read_events(ctx, min_nr, nr, event, &ret);
	if (until == 0 || ret < 0 || ret >= min_nr)
		return ret;

	hrtimer_init_sleeper_on_stack(&t, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	if (until != KTIME_MAX) {
		hrtimer_set_expires_range_ns(&t.timer, until, current->timer_slack_ns);
		hrtimer_sleeper_start_expires(&t, HRTIMER_MODE_REL);
	}

	init_wait(&w.w);

	while (1) {
		unsigned long nr_got = ret;

		w.min_nr = min_nr - ret;

		ret2 = prepare_to_wait_event(&ctx->wait, &w.w, TASK_INTERRUPTIBLE);
		if (!ret2 && !t.task)
			ret2 = -ETIME;

		if (aio_read_events(ctx, min_nr, nr, event, &ret) || ret2)
			break;

		if (nr_got == ret)
			schedule();
	}

	finish_wait(&ctx->wait, &w.w);
	hrtimer_cancel(&t.timer);
	destroy_hrtimer_on_stack(&t.timer);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* sys_io_setup:
 *	Create an aio_context capable of receiving at least nr_events.
 *	ctxp must not point to an aio_context that already exists, and
 *	must be initialized to 0 prior to the call.  On successful
 *	creation of the aio_context, *ctxp is filled in with the resulting 
 *	handle.  May fail with -EINVAL if *ctxp is not initialized,
 *	if the specified nr_events exceeds internal limits.  May fail 
 *	with -EAGAIN if the specified nr_events exceeds the user's limit 
 *	of available events.  May fail with -ENOMEM if insufficient kernel
 *	resources are available.  May fail with -EFAULT if an invalid
 *	pointer is passed for ctxp.  Will fail with -ENOSYS if not
 *	implemented.
 */
SYSCALL_DEFINE2(io_setup, unsigned, nr_events, aio_context_t __user *, ctxp)
{
	struct kioctx *ioctx = NULL;
	unsigned long ctx;
	long ret;

	ret = get_user(ctx, ctxp);
	if (unlikely(ret))
		goto out;

	ret = -EINVAL;
	if (unlikely(ctx || nr_events == 0)) {
<<<<<<< HEAD
		pr_debug("EINVAL: io_setup: ctx %lu nr_events %u\n",
=======
		pr_debug("EINVAL: ctx %lu nr_events %u\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		         ctx, nr_events);
		goto out;
	}

	ioctx = ioctx_alloc(nr_events);
	ret = PTR_ERR(ioctx);
	if (!IS_ERR(ioctx)) {
		ret = put_user(ioctx->user_id, ctxp);
		if (ret)
<<<<<<< HEAD
			io_destroy(ioctx);
		put_ioctx(ioctx);
=======
			kill_ioctx(current->mm, ioctx, NULL);
		percpu_ref_put(&ioctx->users);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

out:
	return ret;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_COMPAT
COMPAT_SYSCALL_DEFINE2(io_setup, unsigned, nr_events, u32 __user *, ctx32p)
{
	struct kioctx *ioctx = NULL;
	unsigned long ctx;
	long ret;

	ret = get_user(ctx, ctx32p);
	if (unlikely(ret))
		goto out;

	ret = -EINVAL;
	if (unlikely(ctx || nr_events == 0)) {
		pr_debug("EINVAL: ctx %lu nr_events %u\n",
		         ctx, nr_events);
		goto out;
	}

	ioctx = ioctx_alloc(nr_events);
	ret = PTR_ERR(ioctx);
	if (!IS_ERR(ioctx)) {
		/* truncating is ok because it's a user address */
		ret = put_user((u32)ioctx->user_id, ctx32p);
		if (ret)
			kill_ioctx(current->mm, ioctx, NULL);
		percpu_ref_put(&ioctx->users);
	}

out:
	return ret;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* sys_io_destroy:
 *	Destroy the aio_context specified.  May cancel any outstanding 
 *	AIOs and block on completion.  Will fail with -ENOSYS if not
 *	implemented.  May fail with -EINVAL if the context pointed to
 *	is invalid.
 */
SYSCALL_DEFINE1(io_destroy, aio_context_t, ctx)
{
	struct kioctx *ioctx = lookup_ioctx(ctx);
	if (likely(NULL != ioctx)) {
<<<<<<< HEAD
		io_destroy(ioctx);
		put_ioctx(ioctx);
		return 0;
	}
	pr_debug("EINVAL: io_destroy: invalid context id\n");
	return -EINVAL;
}

static void aio_advance_iovec(struct kiocb *iocb, ssize_t ret)
{
	struct iovec *iov = &iocb->ki_iovec[iocb->ki_cur_seg];

	BUG_ON(ret <= 0);

	while (iocb->ki_cur_seg < iocb->ki_nr_segs && ret > 0) {
		ssize_t this = min((ssize_t)iov->iov_len, ret);
		iov->iov_base += this;
		iov->iov_len -= this;
		iocb->ki_left -= this;
		ret -= this;
		if (iov->iov_len == 0) {
			iocb->ki_cur_seg++;
			iov++;
		}
	}

	/* the caller should not have done more io than what fit in
	 * the remaining iovecs */
	BUG_ON(ret > 0 && iocb->ki_left == 0);
}

static ssize_t aio_rw_vect_retry(struct kiocb *iocb)
{
	struct file *file = iocb->ki_filp;
	struct address_space *mapping = file->f_mapping;
	struct inode *inode = mapping->host;
	ssize_t (*rw_op)(struct kiocb *, const struct iovec *,
			 unsigned long, loff_t);
	ssize_t ret = 0;
	unsigned short opcode;

	if ((iocb->ki_opcode == IOCB_CMD_PREADV) ||
		(iocb->ki_opcode == IOCB_CMD_PREAD)) {
		rw_op = file->f_op->aio_read;
		opcode = IOCB_CMD_PREADV;
	} else {
		rw_op = file->f_op->aio_write;
		opcode = IOCB_CMD_PWRITEV;
	}

	/* This matches the pread()/pwrite() logic */
	if (iocb->ki_pos < 0)
		return -EINVAL;

	do {
		ret = rw_op(iocb, &iocb->ki_iovec[iocb->ki_cur_seg],
			    iocb->ki_nr_segs - iocb->ki_cur_seg,
			    iocb->ki_pos);
		if (ret > 0)
			aio_advance_iovec(iocb, ret);

	/* retry all partial writes.  retry partial reads as long as its a
	 * regular file. */
	} while (ret > 0 && iocb->ki_left > 0 &&
		 (opcode == IOCB_CMD_PWRITEV ||
		  (!S_ISFIFO(inode->i_mode) && !S_ISSOCK(inode->i_mode))));

	/* This means we must have transferred all that we could */
	/* No need to retry anymore */
	if ((ret == 0) || (iocb->ki_left == 0))
		ret = iocb->ki_nbytes - iocb->ki_left;

	/* If we managed to write some out we return that, rather than
	 * the eventual error. */
	if (opcode == IOCB_CMD_PWRITEV
	    && ret < 0 && ret != -EIOCBQUEUED && ret != -EIOCBRETRY
	    && iocb->ki_nbytes - iocb->ki_left)
		ret = iocb->ki_nbytes - iocb->ki_left;

	return ret;
}

static ssize_t aio_fdsync(struct kiocb *iocb)
{
	struct file *file = iocb->ki_filp;
	ssize_t ret = -EINVAL;

	if (file->f_op->aio_fsync)
		ret = file->f_op->aio_fsync(iocb, 1);
	return ret;
}

static ssize_t aio_fsync(struct kiocb *iocb)
{
	struct file *file = iocb->ki_filp;
	ssize_t ret = -EINVAL;

	if (file->f_op->aio_fsync)
		ret = file->f_op->aio_fsync(iocb, 0);
	return ret;
}

static ssize_t aio_setup_vectored_rw(int type, struct kiocb *kiocb, bool compat)
{
	ssize_t ret;

#ifdef CONFIG_COMPAT
	if (compat)
		ret = compat_rw_copy_check_uvector(type,
				(struct compat_iovec __user *)kiocb->ki_buf,
				kiocb->ki_nbytes, 1, &kiocb->ki_inline_vec,
				&kiocb->ki_iovec, 1);
	else
#endif
		ret = rw_copy_check_uvector(type,
				(struct iovec __user *)kiocb->ki_buf,
				kiocb->ki_nbytes, 1, &kiocb->ki_inline_vec,
				&kiocb->ki_iovec, 1);
	if (ret < 0)
		goto out;

	ret = rw_verify_area(type, kiocb->ki_filp, &kiocb->ki_pos, ret);
	if (ret < 0)
		goto out;

	kiocb->ki_nr_segs = kiocb->ki_nbytes;
	kiocb->ki_cur_seg = 0;
	/* ki_nbytes/left now reflect bytes instead of segs */
	kiocb->ki_nbytes = ret;
	kiocb->ki_left = ret;

	ret = 0;
out:
	return ret;
}

static ssize_t aio_setup_single_vector(int type, struct file * file, struct kiocb *kiocb)
{
	int bytes;

	bytes = rw_verify_area(type, file, &kiocb->ki_pos, kiocb->ki_left);
	if (bytes < 0)
		return bytes;

	kiocb->ki_iovec = &kiocb->ki_inline_vec;
	kiocb->ki_iovec->iov_base = kiocb->ki_buf;
	kiocb->ki_iovec->iov_len = bytes;
	kiocb->ki_nr_segs = 1;
	kiocb->ki_cur_seg = 0;
	return 0;
}

/*
 * aio_setup_iocb:
 *	Performs the initial checks and aio retry method
 *	setup for the kiocb at the time of io submission.
 */
static ssize_t aio_setup_iocb(struct kiocb *kiocb, bool compat)
{
	struct file *file = kiocb->ki_filp;
	ssize_t ret = 0;

	switch (kiocb->ki_opcode) {
	case IOCB_CMD_PREAD:
		ret = -EBADF;
		if (unlikely(!(file->f_mode & FMODE_READ)))
			break;
		ret = -EFAULT;
		if (unlikely(!access_ok(VERIFY_WRITE, kiocb->ki_buf,
			kiocb->ki_left)))
			break;
		ret = aio_setup_single_vector(READ, file, kiocb);
		if (ret)
			break;
		ret = -EINVAL;
		if (file->f_op->aio_read)
			kiocb->ki_retry = aio_rw_vect_retry;
		break;
	case IOCB_CMD_PWRITE:
		ret = -EBADF;
		if (unlikely(!(file->f_mode & FMODE_WRITE)))
			break;
		ret = -EFAULT;
		if (unlikely(!access_ok(VERIFY_READ, kiocb->ki_buf,
			kiocb->ki_left)))
			break;
		ret = aio_setup_single_vector(WRITE, file, kiocb);
		if (ret)
			break;
		ret = -EINVAL;
		if (file->f_op->aio_write)
			kiocb->ki_retry = aio_rw_vect_retry;
		break;
	case IOCB_CMD_PREADV:
		ret = -EBADF;
		if (unlikely(!(file->f_mode & FMODE_READ)))
			break;
		ret = aio_setup_vectored_rw(READ, kiocb, compat);
		if (ret)
			break;
		ret = -EINVAL;
		if (file->f_op->aio_read)
			kiocb->ki_retry = aio_rw_vect_retry;
		break;
	case IOCB_CMD_PWRITEV:
		ret = -EBADF;
		if (unlikely(!(file->f_mode & FMODE_WRITE)))
			break;
		ret = aio_setup_vectored_rw(WRITE, kiocb, compat);
		if (ret)
			break;
		ret = -EINVAL;
		if (file->f_op->aio_write)
			kiocb->ki_retry = aio_rw_vect_retry;
		break;
	case IOCB_CMD_FDSYNC:
		ret = -EINVAL;
		if (file->f_op->aio_fsync)
			kiocb->ki_retry = aio_fdsync;
		break;
	case IOCB_CMD_FSYNC:
		ret = -EINVAL;
		if (file->f_op->aio_fsync)
			kiocb->ki_retry = aio_fsync;
		break;
	default:
		dprintk("EINVAL: io_submit: no operation provided\n");
		ret = -EINVAL;
	}

	if (!kiocb->ki_retry)
		return ret;
=======
		struct ctx_rq_wait wait;
		int ret;

		init_completion(&wait.comp);
		atomic_set(&wait.count, 1);

		/* Pass requests_done to kill_ioctx() where it can be set
		 * in a thread-safe way. If we try to set it here then we have
		 * a race condition if two io_destroy() called simultaneously.
		 */
		ret = kill_ioctx(current->mm, ioctx, &wait);
		percpu_ref_put(&ioctx->users);

		/* Wait until all IO for the context are done. Otherwise kernel
		 * keep using user-space buffers even if user thinks the context
		 * is destroyed.
		 */
		if (!ret)
			wait_for_completion(&wait.comp);

		return ret;
	}
	pr_debug("EINVAL: invalid context id\n");
	return -EINVAL;
}

static void aio_remove_iocb(struct aio_kiocb *iocb)
{
	struct kioctx *ctx = iocb->ki_ctx;
	unsigned long flags;

	spin_lock_irqsave(&ctx->ctx_lock, flags);
	list_del(&iocb->ki_list);
	spin_unlock_irqrestore(&ctx->ctx_lock, flags);
}

static void aio_complete_rw(struct kiocb *kiocb, long res)
{
	struct aio_kiocb *iocb = container_of(kiocb, struct aio_kiocb, rw);

	if (!list_empty_careful(&iocb->ki_list))
		aio_remove_iocb(iocb);

	if (kiocb->ki_flags & IOCB_WRITE) {
		struct inode *inode = file_inode(kiocb->ki_filp);

		if (S_ISREG(inode->i_mode))
			kiocb_end_write(kiocb);
	}

	iocb->ki_res.res = res;
	iocb->ki_res.res2 = 0;
	iocb_put(iocb);
}

static int aio_prep_rw(struct kiocb *req, const struct iocb *iocb)
{
	int ret;

	req->ki_complete = aio_complete_rw;
	req->private = NULL;
	req->ki_pos = iocb->aio_offset;
	req->ki_flags = req->ki_filp->f_iocb_flags | IOCB_AIO_RW;
	if (iocb->aio_flags & IOCB_FLAG_RESFD)
		req->ki_flags |= IOCB_EVENTFD;
	if (iocb->aio_flags & IOCB_FLAG_IOPRIO) {
		/*
		 * If the IOCB_FLAG_IOPRIO flag of aio_flags is set, then
		 * aio_reqprio is interpreted as an I/O scheduling
		 * class and priority.
		 */
		ret = ioprio_check_cap(iocb->aio_reqprio);
		if (ret) {
			pr_debug("aio ioprio check cap error: %d\n", ret);
			return ret;
		}

		req->ki_ioprio = iocb->aio_reqprio;
	} else
		req->ki_ioprio = get_current_ioprio();

	ret = kiocb_set_rw_flags(req, iocb->aio_rw_flags);
	if (unlikely(ret))
		return ret;

	req->ki_flags &= ~IOCB_HIPRI; /* no one is going to poll for this I/O */
	return 0;
}

static ssize_t aio_setup_rw(int rw, const struct iocb *iocb,
		struct iovec **iovec, bool vectored, bool compat,
		struct iov_iter *iter)
{
	void __user *buf = (void __user *)(uintptr_t)iocb->aio_buf;
	size_t len = iocb->aio_nbytes;

	if (!vectored) {
		ssize_t ret = import_ubuf(rw, buf, len, iter);
		*iovec = NULL;
		return ret;
	}

	return __import_iovec(rw, buf, len, UIO_FASTIOV, iovec, iter, compat);
}

static inline void aio_rw_done(struct kiocb *req, ssize_t ret)
{
	switch (ret) {
	case -EIOCBQUEUED:
		break;
	case -ERESTARTSYS:
	case -ERESTARTNOINTR:
	case -ERESTARTNOHAND:
	case -ERESTART_RESTARTBLOCK:
		/*
		 * There's no easy way to restart the syscall since other AIO's
		 * may be already running. Just fail this IO with EINTR.
		 */
		ret = -EINTR;
		fallthrough;
	default:
		req->ki_complete(req, ret);
	}
}

static int aio_read(struct kiocb *req, const struct iocb *iocb,
			bool vectored, bool compat)
{
	struct iovec inline_vecs[UIO_FASTIOV], *iovec = inline_vecs;
	struct iov_iter iter;
	struct file *file;
	int ret;

	ret = aio_prep_rw(req, iocb);
	if (ret)
		return ret;
	file = req->ki_filp;
	if (unlikely(!(file->f_mode & FMODE_READ)))
		return -EBADF;
	if (unlikely(!file->f_op->read_iter))
		return -EINVAL;

	ret = aio_setup_rw(ITER_DEST, iocb, &iovec, vectored, compat, &iter);
	if (ret < 0)
		return ret;
	ret = rw_verify_area(READ, file, &req->ki_pos, iov_iter_count(&iter));
	if (!ret)
		aio_rw_done(req, call_read_iter(file, req, &iter));
	kfree(iovec);
	return ret;
}

static int aio_write(struct kiocb *req, const struct iocb *iocb,
			 bool vectored, bool compat)
{
	struct iovec inline_vecs[UIO_FASTIOV], *iovec = inline_vecs;
	struct iov_iter iter;
	struct file *file;
	int ret;

	ret = aio_prep_rw(req, iocb);
	if (ret)
		return ret;
	file = req->ki_filp;

	if (unlikely(!(file->f_mode & FMODE_WRITE)))
		return -EBADF;
	if (unlikely(!file->f_op->write_iter))
		return -EINVAL;

	ret = aio_setup_rw(ITER_SOURCE, iocb, &iovec, vectored, compat, &iter);
	if (ret < 0)
		return ret;
	ret = rw_verify_area(WRITE, file, &req->ki_pos, iov_iter_count(&iter));
	if (!ret) {
		if (S_ISREG(file_inode(file)->i_mode))
			kiocb_start_write(req);
		req->ki_flags |= IOCB_WRITE;
		aio_rw_done(req, call_write_iter(file, req, &iter));
	}
	kfree(iovec);
	return ret;
}

static void aio_fsync_work(struct work_struct *work)
{
	struct aio_kiocb *iocb = container_of(work, struct aio_kiocb, fsync.work);
	const struct cred *old_cred = override_creds(iocb->fsync.creds);

	iocb->ki_res.res = vfs_fsync(iocb->fsync.file, iocb->fsync.datasync);
	revert_creds(old_cred);
	put_cred(iocb->fsync.creds);
	iocb_put(iocb);
}

static int aio_fsync(struct fsync_iocb *req, const struct iocb *iocb,
		     bool datasync)
{
	if (unlikely(iocb->aio_buf || iocb->aio_offset || iocb->aio_nbytes ||
			iocb->aio_rw_flags))
		return -EINVAL;

	if (unlikely(!req->file->f_op->fsync))
		return -EINVAL;

	req->creds = prepare_creds();
	if (!req->creds)
		return -ENOMEM;

	req->datasync = datasync;
	INIT_WORK(&req->work, aio_fsync_work);
	schedule_work(&req->work);
	return 0;
}

static void aio_poll_put_work(struct work_struct *work)
{
	struct poll_iocb *req = container_of(work, struct poll_iocb, work);
	struct aio_kiocb *iocb = container_of(req, struct aio_kiocb, poll);

	iocb_put(iocb);
}

/*
 * Safely lock the waitqueue which the request is on, synchronizing with the
 * case where the ->poll() provider decides to free its waitqueue early.
 *
 * Returns true on success, meaning that req->head->lock was locked, req->wait
 * is on req->head, and an RCU read lock was taken.  Returns false if the
 * request was already removed from its waitqueue (which might no longer exist).
 */
static bool poll_iocb_lock_wq(struct poll_iocb *req)
{
	wait_queue_head_t *head;

	/*
	 * While we hold the waitqueue lock and the waitqueue is nonempty,
	 * wake_up_pollfree() will wait for us.  However, taking the waitqueue
	 * lock in the first place can race with the waitqueue being freed.
	 *
	 * We solve this as eventpoll does: by taking advantage of the fact that
	 * all users of wake_up_pollfree() will RCU-delay the actual free.  If
	 * we enter rcu_read_lock() and see that the pointer to the queue is
	 * non-NULL, we can then lock it without the memory being freed out from
	 * under us, then check whether the request is still on the queue.
	 *
	 * Keep holding rcu_read_lock() as long as we hold the queue lock, in
	 * case the caller deletes the entry from the queue, leaving it empty.
	 * In that case, only RCU prevents the queue memory from being freed.
	 */
	rcu_read_lock();
	head = smp_load_acquire(&req->head);
	if (head) {
		spin_lock(&head->lock);
		if (!list_empty(&req->wait.entry))
			return true;
		spin_unlock(&head->lock);
	}
	rcu_read_unlock();
	return false;
}

static void poll_iocb_unlock_wq(struct poll_iocb *req)
{
	spin_unlock(&req->head->lock);
	rcu_read_unlock();
}

static void aio_poll_complete_work(struct work_struct *work)
{
	struct poll_iocb *req = container_of(work, struct poll_iocb, work);
	struct aio_kiocb *iocb = container_of(req, struct aio_kiocb, poll);
	struct poll_table_struct pt = { ._key = req->events };
	struct kioctx *ctx = iocb->ki_ctx;
	__poll_t mask = 0;

	if (!READ_ONCE(req->cancelled))
		mask = vfs_poll(req->file, &pt) & req->events;

	/*
	 * Note that ->ki_cancel callers also delete iocb from active_reqs after
	 * calling ->ki_cancel.  We need the ctx_lock roundtrip here to
	 * synchronize with them.  In the cancellation case the list_del_init
	 * itself is not actually needed, but harmless so we keep it in to
	 * avoid further branches in the fast path.
	 */
	spin_lock_irq(&ctx->ctx_lock);
	if (poll_iocb_lock_wq(req)) {
		if (!mask && !READ_ONCE(req->cancelled)) {
			/*
			 * The request isn't actually ready to be completed yet.
			 * Reschedule completion if another wakeup came in.
			 */
			if (req->work_need_resched) {
				schedule_work(&req->work);
				req->work_need_resched = false;
			} else {
				req->work_scheduled = false;
			}
			poll_iocb_unlock_wq(req);
			spin_unlock_irq(&ctx->ctx_lock);
			return;
		}
		list_del_init(&req->wait.entry);
		poll_iocb_unlock_wq(req);
	} /* else, POLLFREE has freed the waitqueue, so we must complete */
	list_del_init(&iocb->ki_list);
	iocb->ki_res.res = mangle_poll(mask);
	spin_unlock_irq(&ctx->ctx_lock);

	iocb_put(iocb);
}

/* assumes we are called with irqs disabled */
static int aio_poll_cancel(struct kiocb *iocb)
{
	struct aio_kiocb *aiocb = container_of(iocb, struct aio_kiocb, rw);
	struct poll_iocb *req = &aiocb->poll;

	if (poll_iocb_lock_wq(req)) {
		WRITE_ONCE(req->cancelled, true);
		if (!req->work_scheduled) {
			schedule_work(&aiocb->poll.work);
			req->work_scheduled = true;
		}
		poll_iocb_unlock_wq(req);
	} /* else, the request was force-cancelled by POLLFREE already */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int io_submit_one(struct kioctx *ctx, struct iocb __user *user_iocb,
			 struct iocb *iocb, struct kiocb_batch *batch,
			 bool compat)
{
	struct kiocb *req;
	struct file *file;
	ssize_t ret;

	/* enforce forwards compatibility on users */
	if (unlikely(iocb->aio_reserved1 || iocb->aio_reserved2)) {
		pr_debug("EINVAL: io_submit: reserve field set\n");
		return -EINVAL;
	}

	/* prevent overflows */
	if (unlikely(
	    (iocb->aio_buf != (unsigned long)iocb->aio_buf) ||
	    (iocb->aio_nbytes != (size_t)iocb->aio_nbytes) ||
	    ((ssize_t)iocb->aio_nbytes < 0)
	   )) {
		pr_debug("EINVAL: io_submit: overflow check\n");
		return -EINVAL;
	}

	file = fget(iocb->aio_fildes);
	if (unlikely(!file))
		return -EBADF;

	req = aio_get_req(ctx, batch);  /* returns with 2 references to req */
	if (unlikely(!req)) {
		fput(file);
		return -EAGAIN;
	}
	req->ki_filp = file;
	if (iocb->aio_flags & IOCB_FLAG_RESFD) {
=======
static int aio_poll_wake(struct wait_queue_entry *wait, unsigned mode, int sync,
		void *key)
{
	struct poll_iocb *req = container_of(wait, struct poll_iocb, wait);
	struct aio_kiocb *iocb = container_of(req, struct aio_kiocb, poll);
	__poll_t mask = key_to_poll(key);
	unsigned long flags;

	/* for instances that support it check for an event match first: */
	if (mask && !(mask & req->events))
		return 0;

	/*
	 * Complete the request inline if possible.  This requires that three
	 * conditions be met:
	 *   1. An event mask must have been passed.  If a plain wakeup was done
	 *	instead, then mask == 0 and we have to call vfs_poll() to get
	 *	the events, so inline completion isn't possible.
	 *   2. The completion work must not have already been scheduled.
	 *   3. ctx_lock must not be busy.  We have to use trylock because we
	 *	already hold the waitqueue lock, so this inverts the normal
	 *	locking order.  Use irqsave/irqrestore because not all
	 *	filesystems (e.g. fuse) call this function with IRQs disabled,
	 *	yet IRQs have to be disabled before ctx_lock is obtained.
	 */
	if (mask && !req->work_scheduled &&
	    spin_trylock_irqsave(&iocb->ki_ctx->ctx_lock, flags)) {
		struct kioctx *ctx = iocb->ki_ctx;

		list_del_init(&req->wait.entry);
		list_del(&iocb->ki_list);
		iocb->ki_res.res = mangle_poll(mask);
		if (iocb->ki_eventfd && !eventfd_signal_allowed()) {
			iocb = NULL;
			INIT_WORK(&req->work, aio_poll_put_work);
			schedule_work(&req->work);
		}
		spin_unlock_irqrestore(&ctx->ctx_lock, flags);
		if (iocb)
			iocb_put(iocb);
	} else {
		/*
		 * Schedule the completion work if needed.  If it was already
		 * scheduled, record that another wakeup came in.
		 *
		 * Don't remove the request from the waitqueue here, as it might
		 * not actually be complete yet (we won't know until vfs_poll()
		 * is called), and we must not miss any wakeups.  POLLFREE is an
		 * exception to this; see below.
		 */
		if (req->work_scheduled) {
			req->work_need_resched = true;
		} else {
			schedule_work(&req->work);
			req->work_scheduled = true;
		}

		/*
		 * If the waitqueue is being freed early but we can't complete
		 * the request inline, we have to tear down the request as best
		 * we can.  That means immediately removing the request from its
		 * waitqueue and preventing all further accesses to the
		 * waitqueue via the request.  We also need to schedule the
		 * completion work (done above).  Also mark the request as
		 * cancelled, to potentially skip an unneeded call to ->poll().
		 */
		if (mask & POLLFREE) {
			WRITE_ONCE(req->cancelled, true);
			list_del_init(&req->wait.entry);

			/*
			 * Careful: this *must* be the last step, since as soon
			 * as req->head is NULL'ed out, the request can be
			 * completed and freed, since aio_poll_complete_work()
			 * will no longer need to take the waitqueue lock.
			 */
			smp_store_release(&req->head, NULL);
		}
	}
	return 1;
}

struct aio_poll_table {
	struct poll_table_struct	pt;
	struct aio_kiocb		*iocb;
	bool				queued;
	int				error;
};

static void
aio_poll_queue_proc(struct file *file, struct wait_queue_head *head,
		struct poll_table_struct *p)
{
	struct aio_poll_table *pt = container_of(p, struct aio_poll_table, pt);

	/* multiple wait queues per file are not supported */
	if (unlikely(pt->queued)) {
		pt->error = -EINVAL;
		return;
	}

	pt->queued = true;
	pt->error = 0;
	pt->iocb->poll.head = head;
	add_wait_queue(head, &pt->iocb->poll.wait);
}

static int aio_poll(struct aio_kiocb *aiocb, const struct iocb *iocb)
{
	struct kioctx *ctx = aiocb->ki_ctx;
	struct poll_iocb *req = &aiocb->poll;
	struct aio_poll_table apt;
	bool cancel = false;
	__poll_t mask;

	/* reject any unknown events outside the normal event mask. */
	if ((u16)iocb->aio_buf != iocb->aio_buf)
		return -EINVAL;
	/* reject fields that are not defined for poll */
	if (iocb->aio_offset || iocb->aio_nbytes || iocb->aio_rw_flags)
		return -EINVAL;

	INIT_WORK(&req->work, aio_poll_complete_work);
	req->events = demangle_poll(iocb->aio_buf) | EPOLLERR | EPOLLHUP;

	req->head = NULL;
	req->cancelled = false;
	req->work_scheduled = false;
	req->work_need_resched = false;

	apt.pt._qproc = aio_poll_queue_proc;
	apt.pt._key = req->events;
	apt.iocb = aiocb;
	apt.queued = false;
	apt.error = -EINVAL; /* same as no support for IOCB_CMD_POLL */

	/* initialized the list so that we can do list_empty checks */
	INIT_LIST_HEAD(&req->wait.entry);
	init_waitqueue_func_entry(&req->wait, aio_poll_wake);

	mask = vfs_poll(req->file, &apt.pt) & req->events;
	spin_lock_irq(&ctx->ctx_lock);
	if (likely(apt.queued)) {
		bool on_queue = poll_iocb_lock_wq(req);

		if (!on_queue || req->work_scheduled) {
			/*
			 * aio_poll_wake() already either scheduled the async
			 * completion work, or completed the request inline.
			 */
			if (apt.error) /* unsupported case: multiple queues */
				cancel = true;
			apt.error = 0;
			mask = 0;
		}
		if (mask || apt.error) {
			/* Steal to complete synchronously. */
			list_del_init(&req->wait.entry);
		} else if (cancel) {
			/* Cancel if possible (may be too late though). */
			WRITE_ONCE(req->cancelled, true);
		} else if (on_queue) {
			/*
			 * Actually waiting for an event, so add the request to
			 * active_reqs so that it can be cancelled if needed.
			 */
			list_add_tail(&aiocb->ki_list, &ctx->active_reqs);
			aiocb->ki_cancel = aio_poll_cancel;
		}
		if (on_queue)
			poll_iocb_unlock_wq(req);
	}
	if (mask) { /* no async, we'd stolen it */
		aiocb->ki_res.res = mangle_poll(mask);
		apt.error = 0;
	}
	spin_unlock_irq(&ctx->ctx_lock);
	if (mask)
		iocb_put(aiocb);
	return apt.error;
}

static int __io_submit_one(struct kioctx *ctx, const struct iocb *iocb,
			   struct iocb __user *user_iocb, struct aio_kiocb *req,
			   bool compat)
{
	req->ki_filp = fget(iocb->aio_fildes);
	if (unlikely(!req->ki_filp))
		return -EBADF;

	if (iocb->aio_flags & IOCB_FLAG_RESFD) {
		struct eventfd_ctx *eventfd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * If the IOCB_FLAG_RESFD flag of aio_flags is set, get an
		 * instance of the file* now. The file descriptor must be
		 * an eventfd() fd, and will be signaled for each completed
		 * event using the eventfd_signal() function.
		 */
<<<<<<< HEAD
		req->ki_eventfd = eventfd_ctx_fdget((int) iocb->aio_resfd);
		if (IS_ERR(req->ki_eventfd)) {
			ret = PTR_ERR(req->ki_eventfd);
			req->ki_eventfd = NULL;
			goto out_put_req;
		}
	}

	ret = put_user(req->ki_key, &user_iocb->aio_key);
	if (unlikely(ret)) {
		dprintk("EFAULT: aio_key\n");
		goto out_put_req;
	}

	req->ki_obj.user = user_iocb;
	req->ki_user_data = iocb->aio_data;
	req->ki_pos = iocb->aio_offset;

	req->ki_buf = (char __user *)(unsigned long)iocb->aio_buf;
	req->ki_left = req->ki_nbytes = iocb->aio_nbytes;
	req->ki_opcode = iocb->aio_lio_opcode;

	ret = aio_setup_iocb(req, compat);

	if (ret)
		goto out_put_req;

	spin_lock_irq(&ctx->ctx_lock);
	/*
	 * We could have raced with io_destroy() and are currently holding a
	 * reference to ctx which should be destroyed. We cannot submit IO
	 * since ctx gets freed as soon as io_submit() puts its reference.  The
	 * check here is reliable: io_destroy() sets ctx->dead before waiting
	 * for outstanding IO and the barrier between these two is realized by
	 * unlock of mm->ioctx_lock and lock of ctx->ctx_lock.  Analogously we
	 * increment ctx->reqs_active before checking for ctx->dead and the
	 * barrier is realized by unlock and lock of ctx->ctx_lock. Thus if we
	 * don't see ctx->dead set here, io_destroy() waits for our IO to
	 * finish.
	 */
	if (ctx->dead) {
		spin_unlock_irq(&ctx->ctx_lock);
		ret = -EINVAL;
		goto out_put_req;
	}
	aio_run_iocb(req);
	if (!list_empty(&ctx->run_list)) {
		/* drain the run list */
		while (__aio_run_iocbs(ctx))
			;
	}
	spin_unlock_irq(&ctx->ctx_lock);

	aio_put_req(req);	/* drop extra ref to req */
	return 0;

out_put_req:
	aio_put_req(req);	/* drop extra ref to req */
	aio_put_req(req);	/* drop i/o ref to req */
	return ret;
}

long do_io_submit(aio_context_t ctx_id, long nr,
		  struct iocb __user *__user *iocbpp, bool compat)
{
	struct kioctx *ctx;
	long ret = 0;
	int i = 0;
	struct blk_plug plug;
	struct kiocb_batch batch;

	if (unlikely(nr < 0))
		return -EINVAL;

	if (unlikely(nr > LONG_MAX/sizeof(*iocbpp)))
		nr = LONG_MAX/sizeof(*iocbpp);

	if (unlikely(!access_ok(VERIFY_READ, iocbpp, (nr*sizeof(*iocbpp)))))
		return -EFAULT;

	ctx = lookup_ioctx(ctx_id);
	if (unlikely(!ctx)) {
		pr_debug("EINVAL: io_submit: invalid context id\n");
		return -EINVAL;
	}

	kiocb_batch_init(&batch, nr);

	blk_start_plug(&plug);

	/*
	 * AKPM: should this return a partial result if some of the IOs were
	 * successfully submitted?
	 */
	for (i=0; i<nr; i++) {
		struct iocb __user *user_iocb;
		struct iocb tmp;

		if (unlikely(__get_user(user_iocb, iocbpp + i))) {
			ret = -EFAULT;
			break;
		}

		if (unlikely(copy_from_user(&tmp, user_iocb, sizeof(tmp)))) {
			ret = -EFAULT;
			break;
		}

		ret = io_submit_one(ctx, user_iocb, &tmp, &batch, compat);
		if (ret)
			break;
	}
	blk_finish_plug(&plug);

	kiocb_batch_free(ctx, &batch);
	put_ioctx(ctx);
	return i ? i : ret;
=======
		eventfd = eventfd_ctx_fdget(iocb->aio_resfd);
		if (IS_ERR(eventfd))
			return PTR_ERR(eventfd);

		req->ki_eventfd = eventfd;
	}

	if (unlikely(put_user(KIOCB_KEY, &user_iocb->aio_key))) {
		pr_debug("EFAULT: aio_key\n");
		return -EFAULT;
	}

	req->ki_res.obj = (u64)(unsigned long)user_iocb;
	req->ki_res.data = iocb->aio_data;
	req->ki_res.res = 0;
	req->ki_res.res2 = 0;

	switch (iocb->aio_lio_opcode) {
	case IOCB_CMD_PREAD:
		return aio_read(&req->rw, iocb, false, compat);
	case IOCB_CMD_PWRITE:
		return aio_write(&req->rw, iocb, false, compat);
	case IOCB_CMD_PREADV:
		return aio_read(&req->rw, iocb, true, compat);
	case IOCB_CMD_PWRITEV:
		return aio_write(&req->rw, iocb, true, compat);
	case IOCB_CMD_FSYNC:
		return aio_fsync(&req->fsync, iocb, false);
	case IOCB_CMD_FDSYNC:
		return aio_fsync(&req->fsync, iocb, true);
	case IOCB_CMD_POLL:
		return aio_poll(req, iocb);
	default:
		pr_debug("invalid aio operation %d\n", iocb->aio_lio_opcode);
		return -EINVAL;
	}
}

static int io_submit_one(struct kioctx *ctx, struct iocb __user *user_iocb,
			 bool compat)
{
	struct aio_kiocb *req;
	struct iocb iocb;
	int err;

	if (unlikely(copy_from_user(&iocb, user_iocb, sizeof(iocb))))
		return -EFAULT;

	/* enforce forwards compatibility on users */
	if (unlikely(iocb.aio_reserved2)) {
		pr_debug("EINVAL: reserve field set\n");
		return -EINVAL;
	}

	/* prevent overflows */
	if (unlikely(
	    (iocb.aio_buf != (unsigned long)iocb.aio_buf) ||
	    (iocb.aio_nbytes != (size_t)iocb.aio_nbytes) ||
	    ((ssize_t)iocb.aio_nbytes < 0)
	   )) {
		pr_debug("EINVAL: overflow check\n");
		return -EINVAL;
	}

	req = aio_get_req(ctx);
	if (unlikely(!req))
		return -EAGAIN;

	err = __io_submit_one(ctx, &iocb, user_iocb, req, compat);

	/* Done with the synchronous reference */
	iocb_put(req);

	/*
	 * If err is 0, we'd either done aio_complete() ourselves or have
	 * arranged for that to be done asynchronously.  Anything non-zero
	 * means that we need to destroy req ourselves.
	 */
	if (unlikely(err)) {
		iocb_destroy(req);
		put_reqs_available(ctx, 1);
	}
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* sys_io_submit:
 *	Queue the nr iocbs pointed to by iocbpp for processing.  Returns
 *	the number of iocbs queued.  May return -EINVAL if the aio_context
 *	specified by ctx_id is invalid, if nr is < 0, if the iocb at
 *	*iocbpp[0] is not properly initialized, if the operation specified
 *	is invalid for the file descriptor in the iocb.  May fail with
 *	-EFAULT if any of the data structures point to invalid data.  May
 *	fail with -EBADF if the file descriptor specified in the first
 *	iocb is invalid.  May fail with -EAGAIN if insufficient resources
 *	are available to queue any iocbs.  Will return 0 if nr is 0.  Will
 *	fail with -ENOSYS if not implemented.
 */
SYSCALL_DEFINE3(io_submit, aio_context_t, ctx_id, long, nr,
		struct iocb __user * __user *, iocbpp)
{
<<<<<<< HEAD
	return do_io_submit(ctx_id, nr, iocbpp, 0);
}

/* lookup_kiocb
 *	Finds a given iocb for cancellation.
 */
static struct kiocb *lookup_kiocb(struct kioctx *ctx, struct iocb __user *iocb,
				  u32 key)
{
	struct list_head *pos;

	assert_spin_locked(&ctx->ctx_lock);

	/* TODO: use a hash or array, this sucks. */
	list_for_each(pos, &ctx->active_reqs) {
		struct kiocb *kiocb = list_kiocb(pos);
		if (kiocb->ki_obj.user == iocb && kiocb->ki_key == key)
			return kiocb;
	}
	return NULL;
}

=======
	struct kioctx *ctx;
	long ret = 0;
	int i = 0;
	struct blk_plug plug;

	if (unlikely(nr < 0))
		return -EINVAL;

	ctx = lookup_ioctx(ctx_id);
	if (unlikely(!ctx)) {
		pr_debug("EINVAL: invalid context id\n");
		return -EINVAL;
	}

	if (nr > ctx->nr_events)
		nr = ctx->nr_events;

	if (nr > AIO_PLUG_THRESHOLD)
		blk_start_plug(&plug);
	for (i = 0; i < nr; i++) {
		struct iocb __user *user_iocb;

		if (unlikely(get_user(user_iocb, iocbpp + i))) {
			ret = -EFAULT;
			break;
		}

		ret = io_submit_one(ctx, user_iocb, false);
		if (ret)
			break;
	}
	if (nr > AIO_PLUG_THRESHOLD)
		blk_finish_plug(&plug);

	percpu_ref_put(&ctx->users);
	return i ? i : ret;
}

#ifdef CONFIG_COMPAT
COMPAT_SYSCALL_DEFINE3(io_submit, compat_aio_context_t, ctx_id,
		       int, nr, compat_uptr_t __user *, iocbpp)
{
	struct kioctx *ctx;
	long ret = 0;
	int i = 0;
	struct blk_plug plug;

	if (unlikely(nr < 0))
		return -EINVAL;

	ctx = lookup_ioctx(ctx_id);
	if (unlikely(!ctx)) {
		pr_debug("EINVAL: invalid context id\n");
		return -EINVAL;
	}

	if (nr > ctx->nr_events)
		nr = ctx->nr_events;

	if (nr > AIO_PLUG_THRESHOLD)
		blk_start_plug(&plug);
	for (i = 0; i < nr; i++) {
		compat_uptr_t user_iocb;

		if (unlikely(get_user(user_iocb, iocbpp + i))) {
			ret = -EFAULT;
			break;
		}

		ret = io_submit_one(ctx, compat_ptr(user_iocb), true);
		if (ret)
			break;
	}
	if (nr > AIO_PLUG_THRESHOLD)
		blk_finish_plug(&plug);

	percpu_ref_put(&ctx->users);
	return i ? i : ret;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* sys_io_cancel:
 *	Attempts to cancel an iocb previously passed to io_submit.  If
 *	the operation is successfully cancelled, the resulting event is
 *	copied into the memory pointed to by result without being placed
 *	into the completion queue and 0 is returned.  May fail with
 *	-EFAULT if any of the data structures pointed to are invalid.
 *	May fail with -EINVAL if aio_context specified by ctx_id is
 *	invalid.  May fail with -EAGAIN if the iocb specified was not
 *	cancelled.  Will fail with -ENOSYS if not implemented.
 */
SYSCALL_DEFINE3(io_cancel, aio_context_t, ctx_id, struct iocb __user *, iocb,
		struct io_event __user *, result)
{
<<<<<<< HEAD
	int (*cancel)(struct kiocb *iocb, struct io_event *res);
	struct kioctx *ctx;
	struct kiocb *kiocb;
	u32 key;
	int ret;

	ret = get_user(key, &iocb->aio_key);
	if (unlikely(ret))
		return -EFAULT;
=======
	struct kioctx *ctx;
	struct aio_kiocb *kiocb;
	int ret = -EINVAL;
	u32 key;
	u64 obj = (u64)(unsigned long)iocb;

	if (unlikely(get_user(key, &iocb->aio_key)))
		return -EFAULT;
	if (unlikely(key != KIOCB_KEY))
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ctx = lookup_ioctx(ctx_id);
	if (unlikely(!ctx))
		return -EINVAL;

	spin_lock_irq(&ctx->ctx_lock);
<<<<<<< HEAD
	ret = -EAGAIN;
	kiocb = lookup_kiocb(ctx, iocb, key);
	if (kiocb && kiocb->ki_cancel) {
		cancel = kiocb->ki_cancel;
		kiocb->ki_users ++;
		kiocbSetCancelled(kiocb);
	} else
		cancel = NULL;
	spin_unlock_irq(&ctx->ctx_lock);

	if (NULL != cancel) {
		struct io_event tmp;
		pr_debug("calling cancel\n");
		memset(&tmp, 0, sizeof(tmp));
		tmp.obj = (u64)(unsigned long)kiocb->ki_obj.user;
		tmp.data = kiocb->ki_user_data;
		ret = cancel(kiocb, &tmp);
		if (!ret) {
			/* Cancellation succeeded -- copy the result
			 * into the user's buffer.
			 */
			if (copy_to_user(result, &tmp, sizeof(tmp)))
				ret = -EFAULT;
		}
	} else
		ret = -EINVAL;

	put_ioctx(ctx);
=======
	/* TODO: use a hash or array, this sucks. */
	list_for_each_entry(kiocb, &ctx->active_reqs, ki_list) {
		if (kiocb->ki_res.obj == obj) {
			ret = kiocb->ki_cancel(&kiocb->rw);
			list_del_init(&kiocb->ki_list);
			break;
		}
	}
	spin_unlock_irq(&ctx->ctx_lock);

	if (!ret) {
		/*
		 * The result argument is no longer used - the io_event is
		 * always delivered via the ring buffer. -EINPROGRESS indicates
		 * cancellation is progress:
		 */
		ret = -EINPROGRESS;
	}

	percpu_ref_put(&ctx->users);

	return ret;
}

static long do_io_getevents(aio_context_t ctx_id,
		long min_nr,
		long nr,
		struct io_event __user *events,
		struct timespec64 *ts)
{
	ktime_t until = ts ? timespec64_to_ktime(*ts) : KTIME_MAX;
	struct kioctx *ioctx = lookup_ioctx(ctx_id);
	long ret = -EINVAL;

	if (likely(ioctx)) {
		if (likely(min_nr <= nr && min_nr >= 0))
			ret = read_events(ioctx, min_nr, nr, events, until);
		percpu_ref_put(&ioctx->users);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/* io_getevents:
 *	Attempts to read at least min_nr events and up to nr events from
 *	the completion queue for the aio_context specified by ctx_id. If
 *	it succeeds, the number of read events is returned. May fail with
 *	-EINVAL if ctx_id is invalid, if min_nr is out of range, if nr is
 *	out of range, if timeout is out of range.  May fail with -EFAULT
 *	if any of the memory specified is invalid.  May return 0 or
 *	< min_nr if the timeout specified by timeout has elapsed
 *	before sufficient events are available, where timeout == NULL
 *	specifies an infinite timeout. Note that the timeout pointed to by
<<<<<<< HEAD
 *	timeout is relative and will be updated if not NULL and the
 *	operation blocks. Will fail with -ENOSYS if not implemented.
 */
=======
 *	timeout is relative.  Will fail with -ENOSYS if not implemented.
 */
#ifdef CONFIG_64BIT

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
SYSCALL_DEFINE5(io_getevents, aio_context_t, ctx_id,
		long, min_nr,
		long, nr,
		struct io_event __user *, events,
<<<<<<< HEAD
		struct timespec __user *, timeout)
{
	struct kioctx *ioctx = lookup_ioctx(ctx_id);
	long ret = -EINVAL;

	if (likely(ioctx)) {
		if (likely(min_nr <= nr && min_nr >= 0))
			ret = read_events(ioctx, min_nr, nr, events, timeout);
		put_ioctx(ioctx);
	}

	asmlinkage_protect(5, ret, ctx_id, min_nr, nr, events, timeout);
	return ret;
}
=======
		struct __kernel_timespec __user *, timeout)
{
	struct timespec64	ts;
	int			ret;

	if (timeout && unlikely(get_timespec64(&ts, timeout)))
		return -EFAULT;

	ret = do_io_getevents(ctx_id, min_nr, nr, events, timeout ? &ts : NULL);
	if (!ret && signal_pending(current))
		ret = -EINTR;
	return ret;
}

#endif

struct __aio_sigset {
	const sigset_t __user	*sigmask;
	size_t		sigsetsize;
};

SYSCALL_DEFINE6(io_pgetevents,
		aio_context_t, ctx_id,
		long, min_nr,
		long, nr,
		struct io_event __user *, events,
		struct __kernel_timespec __user *, timeout,
		const struct __aio_sigset __user *, usig)
{
	struct __aio_sigset	ksig = { NULL, };
	struct timespec64	ts;
	bool interrupted;
	int ret;

	if (timeout && unlikely(get_timespec64(&ts, timeout)))
		return -EFAULT;

	if (usig && copy_from_user(&ksig, usig, sizeof(ksig)))
		return -EFAULT;

	ret = set_user_sigmask(ksig.sigmask, ksig.sigsetsize);
	if (ret)
		return ret;

	ret = do_io_getevents(ctx_id, min_nr, nr, events, timeout ? &ts : NULL);

	interrupted = signal_pending(current);
	restore_saved_sigmask_unless(interrupted);
	if (interrupted && !ret)
		ret = -ERESTARTNOHAND;

	return ret;
}

#if defined(CONFIG_COMPAT_32BIT_TIME) && !defined(CONFIG_64BIT)

SYSCALL_DEFINE6(io_pgetevents_time32,
		aio_context_t, ctx_id,
		long, min_nr,
		long, nr,
		struct io_event __user *, events,
		struct old_timespec32 __user *, timeout,
		const struct __aio_sigset __user *, usig)
{
	struct __aio_sigset	ksig = { NULL, };
	struct timespec64	ts;
	bool interrupted;
	int ret;

	if (timeout && unlikely(get_old_timespec32(&ts, timeout)))
		return -EFAULT;

	if (usig && copy_from_user(&ksig, usig, sizeof(ksig)))
		return -EFAULT;


	ret = set_user_sigmask(ksig.sigmask, ksig.sigsetsize);
	if (ret)
		return ret;

	ret = do_io_getevents(ctx_id, min_nr, nr, events, timeout ? &ts : NULL);

	interrupted = signal_pending(current);
	restore_saved_sigmask_unless(interrupted);
	if (interrupted && !ret)
		ret = -ERESTARTNOHAND;

	return ret;
}

#endif

#if defined(CONFIG_COMPAT_32BIT_TIME)

SYSCALL_DEFINE5(io_getevents_time32, __u32, ctx_id,
		__s32, min_nr,
		__s32, nr,
		struct io_event __user *, events,
		struct old_timespec32 __user *, timeout)
{
	struct timespec64 t;
	int ret;

	if (timeout && get_old_timespec32(&t, timeout))
		return -EFAULT;

	ret = do_io_getevents(ctx_id, min_nr, nr, events, timeout ? &t : NULL);
	if (!ret && signal_pending(current))
		ret = -EINTR;
	return ret;
}

#endif

#ifdef CONFIG_COMPAT

struct __compat_aio_sigset {
	compat_uptr_t		sigmask;
	compat_size_t		sigsetsize;
};

#if defined(CONFIG_COMPAT_32BIT_TIME)

COMPAT_SYSCALL_DEFINE6(io_pgetevents,
		compat_aio_context_t, ctx_id,
		compat_long_t, min_nr,
		compat_long_t, nr,
		struct io_event __user *, events,
		struct old_timespec32 __user *, timeout,
		const struct __compat_aio_sigset __user *, usig)
{
	struct __compat_aio_sigset ksig = { 0, };
	struct timespec64 t;
	bool interrupted;
	int ret;

	if (timeout && get_old_timespec32(&t, timeout))
		return -EFAULT;

	if (usig && copy_from_user(&ksig, usig, sizeof(ksig)))
		return -EFAULT;

	ret = set_compat_user_sigmask(compat_ptr(ksig.sigmask), ksig.sigsetsize);
	if (ret)
		return ret;

	ret = do_io_getevents(ctx_id, min_nr, nr, events, timeout ? &t : NULL);

	interrupted = signal_pending(current);
	restore_saved_sigmask_unless(interrupted);
	if (interrupted && !ret)
		ret = -ERESTARTNOHAND;

	return ret;
}

#endif

COMPAT_SYSCALL_DEFINE6(io_pgetevents_time64,
		compat_aio_context_t, ctx_id,
		compat_long_t, min_nr,
		compat_long_t, nr,
		struct io_event __user *, events,
		struct __kernel_timespec __user *, timeout,
		const struct __compat_aio_sigset __user *, usig)
{
	struct __compat_aio_sigset ksig = { 0, };
	struct timespec64 t;
	bool interrupted;
	int ret;

	if (timeout && get_timespec64(&t, timeout))
		return -EFAULT;

	if (usig && copy_from_user(&ksig, usig, sizeof(ksig)))
		return -EFAULT;

	ret = set_compat_user_sigmask(compat_ptr(ksig.sigmask), ksig.sigsetsize);
	if (ret)
		return ret;

	ret = do_io_getevents(ctx_id, min_nr, nr, events, timeout ? &t : NULL);

	interrupted = signal_pending(current);
	restore_saved_sigmask_unless(interrupted);
	if (interrupted && !ret)
		ret = -ERESTARTNOHAND;

	return ret;
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
