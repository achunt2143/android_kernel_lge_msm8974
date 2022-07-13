<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/fs/exec.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/*
 * #!-checking implemented by tytso.
 */
/*
 * Demand-loading implemented 01.12.91 - no need to read anything but
 * the header into memory. The inode of the executable is put into
 * "current->executable", and page faults do the actual loading. Clean.
 *
 * Once more I can proudly say that linux stood up to being changed: it
 * was less than 2 hours work to get demand-loading completely implemented.
 *
 * Demand loading changed July 1993 by Eric Youngdale.   Use mmap instead,
 * current->executable is only used by the procfs.  This allows a dispatch
 * table to check for several different types  of binary formats.  We keep
 * trying until we recognize the file or we run out of supported binary
<<<<<<< HEAD
 * formats. 
 */

=======
 * formats.
 */

#include <linux/kernel_read_file.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/mm.h>
#include <linux/stat.h>
#include <linux/fcntl.h>
#include <linux/swap.h>
#include <linux/string.h>
#include <linux/init.h>
<<<<<<< HEAD
=======
#include <linux/sched/mm.h>
#include <linux/sched/coredump.h>
#include <linux/sched/signal.h>
#include <linux/sched/numa_balancing.h>
#include <linux/sched/task.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pagemap.h>
#include <linux/perf_event.h>
#include <linux/highmem.h>
#include <linux/spinlock.h>
#include <linux/key.h>
#include <linux/personality.h>
#include <linux/binfmts.h>
#include <linux/utsname.h>
#include <linux/pid_namespace.h>
#include <linux/module.h>
#include <linux/namei.h>
#include <linux/mount.h>
#include <linux/security.h>
#include <linux/syscalls.h>
#include <linux/tsacct_kern.h>
#include <linux/cn_proc.h>
#include <linux/audit.h>
<<<<<<< HEAD
#include <linux/tracehook.h>
#include <linux/kmod.h>
#include <linux/fsnotify.h>
#include <linux/fs_struct.h>
#include <linux/pipe_fs_i.h>
#include <linux/oom.h>
#include <linux/compat.h>

#include <asm/uaccess.h>
#include <asm/mmu_context.h>
#include <asm/tlb.h>
#include <asm/exec.h>
=======
#include <linux/kmod.h>
#include <linux/fsnotify.h>
#include <linux/fs_struct.h>
#include <linux/oom.h>
#include <linux/compat.h>
#include <linux/vmalloc.h>
#include <linux/io_uring.h>
#include <linux/syscall_user_dispatch.h>
#include <linux/coredump.h>
#include <linux/time_namespace.h>
#include <linux/user_events.h>
#include <linux/rseq.h>

#include <linux/uaccess.h>
#include <asm/mmu_context.h>
#include <asm/tlb.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <trace/events/task.h>
#include "internal.h"

#include <trace/events/sched.h>
<<<<<<< HEAD
/* LGE_CHANGE_S
 *
 * do read/mmap profiling during booting
 * in order to use the data as readahead args
 *
 * byungchul.park@lge.com 20120503
 */
#include "sreadahead_prof.h"
/* LGE_CHAGE_E */


int core_uses_pid;
char core_pattern[CORENAME_MAX_SIZE] = "core";
unsigned int core_pipe_limit;
int suid_dumpable = 0;

struct core_name {
	char *corename;
	int used, size;
};
static atomic_t call_count = ATOMIC_INIT(1);

/* The maximal length of core_pattern is also specified in sysctl.c */

=======

static int bprm_creds_from_file(struct linux_binprm *bprm);

int suid_dumpable = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static LIST_HEAD(formats);
static DEFINE_RWLOCK(binfmt_lock);

void __register_binfmt(struct linux_binfmt * fmt, int insert)
{
<<<<<<< HEAD
	BUG_ON(!fmt);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	write_lock(&binfmt_lock);
	insert ? list_add(&fmt->lh, &formats) :
		 list_add_tail(&fmt->lh, &formats);
	write_unlock(&binfmt_lock);
}

EXPORT_SYMBOL(__register_binfmt);

void unregister_binfmt(struct linux_binfmt * fmt)
{
	write_lock(&binfmt_lock);
	list_del(&fmt->lh);
	write_unlock(&binfmt_lock);
}

EXPORT_SYMBOL(unregister_binfmt);

static inline void put_binfmt(struct linux_binfmt * fmt)
{
	module_put(fmt->module);
}

<<<<<<< HEAD
=======
bool path_noexec(const struct path *path)
{
	return (path->mnt->mnt_flags & MNT_NOEXEC) ||
	       (path->mnt->mnt_sb->s_iflags & SB_I_NOEXEC);
}

#ifdef CONFIG_USELIB
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Note that a shared library must be both readable and executable due to
 * security reasons.
 *
<<<<<<< HEAD
 * Also note that we take the address to load from from the file itself.
 */
SYSCALL_DEFINE1(uselib, const char __user *, library)
{
=======
 * Also note that we take the address to load from the file itself.
 */
SYSCALL_DEFINE1(uselib, const char __user *, library)
{
	struct linux_binfmt *fmt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct file *file;
	struct filename *tmp = getname(library);
	int error = PTR_ERR(tmp);
	static const struct open_flags uselib_flags = {
<<<<<<< HEAD
		.open_flag = O_LARGEFILE | O_RDONLY | __FMODE_EXEC,
		.acc_mode = MAY_READ | MAY_EXEC | MAY_OPEN,
		.intent = LOOKUP_OPEN
=======
		.open_flag = O_LARGEFILE | O_RDONLY,
		.acc_mode = MAY_READ | MAY_EXEC,
		.intent = LOOKUP_OPEN,
		.lookup_flags = LOOKUP_FOLLOW,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	};

	if (IS_ERR(tmp))
		goto out;

<<<<<<< HEAD
	file = do_filp_open(AT_FDCWD, tmp, &uselib_flags, LOOKUP_FOLLOW);
=======
	file = do_filp_open(AT_FDCWD, tmp, &uselib_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	putname(tmp);
	error = PTR_ERR(file);
	if (IS_ERR(file))
		goto out;

<<<<<<< HEAD
	error = -EINVAL;
	if (!S_ISREG(file->f_path.dentry->d_inode->i_mode))
		goto exit;

	error = -EACCES;
	if (file->f_path.mnt->mnt_flags & MNT_NOEXEC)
		goto exit;

	fsnotify_open(file);
/* LGE_CHANGE_S
 *
 * do read/mmap profiling during booting
 * in order to use the data as readahead args
 *
 * byungchul.park@lge.com 20120503
 */
	sreadahead_prof(file, 0, 0);
/* LGE_CHANGE_E */


	error = -ENOEXEC;
	if(file->f_op) {
		struct linux_binfmt * fmt;

		read_lock(&binfmt_lock);
		list_for_each_entry(fmt, &formats, lh) {
			if (!fmt->load_shlib)
				continue;
			if (!try_module_get(fmt->module))
				continue;
			read_unlock(&binfmt_lock);
			error = fmt->load_shlib(file);
			read_lock(&binfmt_lock);
			put_binfmt(fmt);
			if (error != -ENOEXEC)
				break;
		}
		read_unlock(&binfmt_lock);
	}
exit:
	fput(file);
out:
  	return error;
}
=======
	/*
	 * may_open() has already checked for this, so it should be
	 * impossible to trip now. But we need to be extra cautious
	 * and check again at the very end too.
	 */
	error = -EACCES;
	if (WARN_ON_ONCE(!S_ISREG(file_inode(file)->i_mode) ||
			 path_noexec(&file->f_path)))
		goto exit;

	error = -ENOEXEC;

	read_lock(&binfmt_lock);
	list_for_each_entry(fmt, &formats, lh) {
		if (!fmt->load_shlib)
			continue;
		if (!try_module_get(fmt->module))
			continue;
		read_unlock(&binfmt_lock);
		error = fmt->load_shlib(file);
		read_lock(&binfmt_lock);
		put_binfmt(fmt);
		if (error != -ENOEXEC)
			break;
	}
	read_unlock(&binfmt_lock);
exit:
	fput(file);
out:
	return error;
}
#endif /* #ifdef CONFIG_USELIB */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_MMU
/*
 * The nascent bprm->mm is not visible until exec_mmap() but it can
 * use a lot of memory, account these pages in current->mm temporary
 * for oom_badness()->get_mm_rss(). Once exec succeeds or fails, we
 * change the counter back via acct_arg_size(0).
 */
static void acct_arg_size(struct linux_binprm *bprm, unsigned long pages)
{
	struct mm_struct *mm = current->mm;
	long diff = (long)(pages - bprm->vma_pages);

	if (!mm || !diff)
		return;

	bprm->vma_pages = pages;
	add_mm_counter(mm, MM_ANONPAGES, diff);
}

static struct page *get_arg_page(struct linux_binprm *bprm, unsigned long pos,
		int write)
{
	struct page *page;
<<<<<<< HEAD
	int ret;

#ifdef CONFIG_STACK_GROWSUP
	if (write) {
		ret = expand_downwards(bprm->vma, pos);
		if (ret < 0)
			return NULL;
	}
#endif
	ret = get_user_pages(current, bprm->mm, pos,
			1, write, 1, &page, NULL);
	if (ret <= 0)
		return NULL;

	if (write) {
		unsigned long size = bprm->vma->vm_end - bprm->vma->vm_start;
		unsigned long ptr_size, limit;

		/*
		 * Since the stack will hold pointers to the strings, we
		 * must account for them as well.
		 *
		 * The size calculation is the entire vma while each arg page is
		 * built, so each time we get here it's calculating how far it
		 * is currently (rather than each call being just the newly
		 * added size from the arg page).  As a result, we need to
		 * always add the entire size of the pointers, so that on the
		 * last call to get_arg_page() we'll actually have the entire
		 * correct size.
		 */
		ptr_size = (bprm->argc + bprm->envc) * sizeof(void *);
		if (ptr_size > ULONG_MAX - size)
			goto fail;
		size += ptr_size;

		acct_arg_size(bprm, size / PAGE_SIZE);

		/*
		 * We've historically supported up to 32 pages (ARG_MAX)
		 * of argument strings even with small stacks
		 */
		if (size <= ARG_MAX)
			return page;

		/*
		 * Limit to 1/4 of the max stack size or 3/4 of _STK_LIM
		 * (whichever is smaller) for the argv+env strings.
		 * This ensures that:
		 *  - the remaining binfmt code will not run out of stack space,
		 *  - the program will have a reasonable amount of stack left
		 *    to work from.
		 */
		limit = _STK_LIM / 4 * 3;
		limit = min(limit, rlimit(RLIMIT_STACK) / 4);
		if (size > limit)
			goto fail;
	}

	return page;

fail:
	put_page(page);
	return NULL;
=======
	struct vm_area_struct *vma = bprm->vma;
	struct mm_struct *mm = bprm->mm;
	int ret;

	/*
	 * Avoid relying on expanding the stack down in GUP (which
	 * does not work for STACK_GROWSUP anyway), and just do it
	 * by hand ahead of time.
	 */
	if (write && pos < vma->vm_start) {
		mmap_write_lock(mm);
		ret = expand_downwards(vma, pos);
		if (unlikely(ret < 0)) {
			mmap_write_unlock(mm);
			return NULL;
		}
		mmap_write_downgrade(mm);
	} else
		mmap_read_lock(mm);

	/*
	 * We are doing an exec().  'current' is the process
	 * doing the exec and 'mm' is the new process's mm.
	 */
	ret = get_user_pages_remote(mm, pos, 1,
			write ? FOLL_WRITE : 0,
			&page, NULL);
	mmap_read_unlock(mm);
	if (ret <= 0)
		return NULL;

	if (write)
		acct_arg_size(bprm, vma_pages(vma));

	return page;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void put_arg_page(struct page *page)
{
	put_page(page);
}

<<<<<<< HEAD
static void free_arg_page(struct linux_binprm *bprm, int i)
{
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void free_arg_pages(struct linux_binprm *bprm)
{
}

static void flush_arg_page(struct linux_binprm *bprm, unsigned long pos,
		struct page *page)
{
	flush_cache_page(bprm->vma, pos, page_to_pfn(page));
}

static int __bprm_mm_init(struct linux_binprm *bprm)
{
	int err;
	struct vm_area_struct *vma = NULL;
	struct mm_struct *mm = bprm->mm;

<<<<<<< HEAD
	bprm->vma = vma = kmem_cache_zalloc(vm_area_cachep, GFP_KERNEL);
	if (!vma)
		return -ENOMEM;

	down_write(&mm->mmap_sem);
	vma->vm_mm = mm;
=======
	bprm->vma = vma = vm_area_alloc(mm);
	if (!vma)
		return -ENOMEM;
	vma_set_anonymous(vma);

	if (mmap_write_lock_killable(mm)) {
		err = -EINTR;
		goto err_free;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Place the stack at the largest stack address the architecture
	 * supports. Later, we'll move this to an appropriate place. We don't
	 * use STACK_TOP because that can depend on attributes which aren't
	 * configured yet.
	 */
	BUILD_BUG_ON(VM_STACK_FLAGS & VM_STACK_INCOMPLETE_SETUP);
	vma->vm_end = STACK_TOP_MAX;
	vma->vm_start = vma->vm_end - PAGE_SIZE;
<<<<<<< HEAD
	vma->vm_flags = VM_STACK_FLAGS | VM_STACK_INCOMPLETE_SETUP;
	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);
	INIT_LIST_HEAD(&vma->anon_vma_chain);

	err = security_file_mmap(NULL, 0, 0, 0, vma->vm_start, 1);
	if (err)
		goto err;
=======
	vm_flags_init(vma, VM_SOFTDIRTY | VM_STACK_FLAGS | VM_STACK_INCOMPLETE_SETUP);
	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = insert_vm_struct(mm, vma);
	if (err)
		goto err;

	mm->stack_vm = mm->total_vm = 1;
<<<<<<< HEAD
	up_write(&mm->mmap_sem);
	bprm->p = vma->vm_end - sizeof(void *);
	return 0;
err:
	up_write(&mm->mmap_sem);
	bprm->vma = NULL;
	kmem_cache_free(vm_area_cachep, vma);
=======
	mmap_write_unlock(mm);
	bprm->p = vma->vm_end - sizeof(void *);
	return 0;
err:
	mmap_write_unlock(mm);
err_free:
	bprm->vma = NULL;
	vm_area_free(vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static bool valid_arg_len(struct linux_binprm *bprm, long len)
{
	return len <= MAX_ARG_STRLEN;
}

#else

static inline void acct_arg_size(struct linux_binprm *bprm, unsigned long pages)
{
}

static struct page *get_arg_page(struct linux_binprm *bprm, unsigned long pos,
		int write)
{
	struct page *page;

	page = bprm->page[pos / PAGE_SIZE];
	if (!page && write) {
		page = alloc_page(GFP_HIGHUSER|__GFP_ZERO);
		if (!page)
			return NULL;
		bprm->page[pos / PAGE_SIZE] = page;
	}

	return page;
}

static void put_arg_page(struct page *page)
{
}

static void free_arg_page(struct linux_binprm *bprm, int i)
{
	if (bprm->page[i]) {
		__free_page(bprm->page[i]);
		bprm->page[i] = NULL;
	}
}

static void free_arg_pages(struct linux_binprm *bprm)
{
	int i;

	for (i = 0; i < MAX_ARG_PAGES; i++)
		free_arg_page(bprm, i);
}

static void flush_arg_page(struct linux_binprm *bprm, unsigned long pos,
		struct page *page)
{
}

static int __bprm_mm_init(struct linux_binprm *bprm)
{
	bprm->p = PAGE_SIZE * MAX_ARG_PAGES - sizeof(void *);
	return 0;
}

static bool valid_arg_len(struct linux_binprm *bprm, long len)
{
	return len <= bprm->p;
}

#endif /* CONFIG_MMU */

/*
 * Create a new mm_struct and populate it with a temporary stack
 * vm_area_struct.  We don't have enough context at this point to set the stack
 * flags, permissions, and offset, so we use temporary values.  We'll update
 * them later in setup_arg_pages().
 */
<<<<<<< HEAD
int bprm_mm_init(struct linux_binprm *bprm)
=======
static int bprm_mm_init(struct linux_binprm *bprm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	struct mm_struct *mm = NULL;

	bprm->mm = mm = mm_alloc();
	err = -ENOMEM;
	if (!mm)
		goto err;

<<<<<<< HEAD
	err = init_new_context(current, mm);
	if (err)
		goto err;
=======
	/* Save current stack limit for all calculations made during exec. */
	task_lock(current->group_leader);
	bprm->rlim_stack = current->signal->rlim[RLIMIT_STACK];
	task_unlock(current->group_leader);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = __bprm_mm_init(bprm);
	if (err)
		goto err;

	return 0;

err:
	if (mm) {
		bprm->mm = NULL;
		mmdrop(mm);
	}

	return err;
}

struct user_arg_ptr {
#ifdef CONFIG_COMPAT
	bool is_compat;
#endif
	union {
		const char __user *const __user *native;
#ifdef CONFIG_COMPAT
<<<<<<< HEAD
		compat_uptr_t __user *compat;
=======
		const compat_uptr_t __user *compat;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	} ptr;
};

static const char __user *get_user_arg_ptr(struct user_arg_ptr argv, int nr)
{
	const char __user *native;

#ifdef CONFIG_COMPAT
	if (unlikely(argv.is_compat)) {
		compat_uptr_t compat;

		if (get_user(compat, argv.ptr.compat + nr))
			return ERR_PTR(-EFAULT);

		return compat_ptr(compat);
	}
#endif

	if (get_user(native, argv.ptr.native + nr))
		return ERR_PTR(-EFAULT);

	return native;
}

/*
 * count() counts the number of strings in array ARGV.
 */
static int count(struct user_arg_ptr argv, int max)
{
	int i = 0;

	if (argv.ptr.native != NULL) {
		for (;;) {
			const char __user *p = get_user_arg_ptr(argv, i);

			if (!p)
				break;

			if (IS_ERR(p))
				return -EFAULT;

<<<<<<< HEAD
			if (i++ >= max)
				return -E2BIG;
=======
			if (i >= max)
				return -E2BIG;
			++i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			if (fatal_signal_pending(current))
				return -ERESTARTNOHAND;
			cond_resched();
		}
	}
	return i;
}

<<<<<<< HEAD
=======
static int count_strings_kernel(const char *const *argv)
{
	int i;

	if (!argv)
		return 0;

	for (i = 0; argv[i]; ++i) {
		if (i >= MAX_ARG_STRINGS)
			return -E2BIG;
		if (fatal_signal_pending(current))
			return -ERESTARTNOHAND;
		cond_resched();
	}
	return i;
}

static int bprm_stack_limits(struct linux_binprm *bprm)
{
	unsigned long limit, ptr_size;

	/*
	 * Limit to 1/4 of the max stack size or 3/4 of _STK_LIM
	 * (whichever is smaller) for the argv+env strings.
	 * This ensures that:
	 *  - the remaining binfmt code will not run out of stack space,
	 *  - the program will have a reasonable amount of stack left
	 *    to work from.
	 */
	limit = _STK_LIM / 4 * 3;
	limit = min(limit, bprm->rlim_stack.rlim_cur / 4);
	/*
	 * We've historically supported up to 32 pages (ARG_MAX)
	 * of argument strings even with small stacks
	 */
	limit = max_t(unsigned long, limit, ARG_MAX);
	/*
	 * We must account for the size of all the argv and envp pointers to
	 * the argv and envp strings, since they will also take up space in
	 * the stack. They aren't stored until much later when we can't
	 * signal to the parent that the child has run out of stack space.
	 * Instead, calculate it here so it's possible to fail gracefully.
	 *
	 * In the case of argc = 0, make sure there is space for adding a
	 * empty string (which will bump argc to 1), to ensure confused
	 * userspace programs don't start processing from argv[1], thinking
	 * argc can never be 0, to keep them from walking envp by accident.
	 * See do_execveat_common().
	 */
	ptr_size = (max(bprm->argc, 1) + bprm->envc) * sizeof(void *);
	if (limit <= ptr_size)
		return -E2BIG;
	limit -= ptr_size;

	bprm->argmin = bprm->p - limit;
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * 'copy_strings()' copies argument/environment strings from the old
 * processes's memory to the new process's stack.  The call to get_user_pages()
 * ensures the destination page is created and not swapped out.
 */
static int copy_strings(int argc, struct user_arg_ptr argv,
			struct linux_binprm *bprm)
{
	struct page *kmapped_page = NULL;
	char *kaddr = NULL;
	unsigned long kpos = 0;
	int ret;

	while (argc-- > 0) {
		const char __user *str;
		int len;
		unsigned long pos;

		ret = -EFAULT;
		str = get_user_arg_ptr(argv, argc);
		if (IS_ERR(str))
			goto out;

		len = strnlen_user(str, MAX_ARG_STRLEN);
		if (!len)
			goto out;

		ret = -E2BIG;
		if (!valid_arg_len(bprm, len))
			goto out;

<<<<<<< HEAD
		/* We're going to work our way backwords. */
		pos = bprm->p;
		str += len;
		bprm->p -= len;
=======
		/* We're going to work our way backwards. */
		pos = bprm->p;
		str += len;
		bprm->p -= len;
#ifdef CONFIG_MMU
		if (bprm->p < bprm->argmin)
			goto out;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		while (len > 0) {
			int offset, bytes_to_copy;

			if (fatal_signal_pending(current)) {
				ret = -ERESTARTNOHAND;
				goto out;
			}
			cond_resched();

			offset = pos % PAGE_SIZE;
			if (offset == 0)
				offset = PAGE_SIZE;

			bytes_to_copy = offset;
			if (bytes_to_copy > len)
				bytes_to_copy = len;

			offset -= bytes_to_copy;
			pos -= bytes_to_copy;
			str -= bytes_to_copy;
			len -= bytes_to_copy;

			if (!kmapped_page || kpos != (pos & PAGE_MASK)) {
				struct page *page;

				page = get_arg_page(bprm, pos, 1);
				if (!page) {
					ret = -E2BIG;
					goto out;
				}

				if (kmapped_page) {
<<<<<<< HEAD
					flush_kernel_dcache_page(kmapped_page);
					kunmap(kmapped_page);
					put_arg_page(kmapped_page);
				}
				kmapped_page = page;
				kaddr = kmap(kmapped_page);
=======
					flush_dcache_page(kmapped_page);
					kunmap_local(kaddr);
					put_arg_page(kmapped_page);
				}
				kmapped_page = page;
				kaddr = kmap_local_page(kmapped_page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				kpos = pos & PAGE_MASK;
				flush_arg_page(bprm, kpos, kmapped_page);
			}
			if (copy_from_user(kaddr+offset, str, bytes_to_copy)) {
				ret = -EFAULT;
				goto out;
			}
		}
	}
	ret = 0;
out:
	if (kmapped_page) {
<<<<<<< HEAD
		flush_kernel_dcache_page(kmapped_page);
		kunmap(kmapped_page);
=======
		flush_dcache_page(kmapped_page);
		kunmap_local(kaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_arg_page(kmapped_page);
	}
	return ret;
}

/*
<<<<<<< HEAD
 * Like copy_strings, but get argv and its values from kernel memory.
 */
int copy_strings_kernel(int argc, const char *const *__argv,
			struct linux_binprm *bprm)
{
	int r;
	mm_segment_t oldfs = get_fs();
	struct user_arg_ptr argv = {
		.ptr.native = (const char __user *const  __user *)__argv,
	};

	set_fs(KERNEL_DS);
	r = copy_strings(argc, argv, bprm);
	set_fs(oldfs);

	return r;
}
EXPORT_SYMBOL(copy_strings_kernel);
=======
 * Copy and argument/environment string from the kernel to the processes stack.
 */
int copy_string_kernel(const char *arg, struct linux_binprm *bprm)
{
	int len = strnlen(arg, MAX_ARG_STRLEN) + 1 /* terminating NUL */;
	unsigned long pos = bprm->p;

	if (len == 0)
		return -EFAULT;
	if (!valid_arg_len(bprm, len))
		return -E2BIG;

	/* We're going to work our way backwards. */
	arg += len;
	bprm->p -= len;
	if (IS_ENABLED(CONFIG_MMU) && bprm->p < bprm->argmin)
		return -E2BIG;

	while (len > 0) {
		unsigned int bytes_to_copy = min_t(unsigned int, len,
				min_not_zero(offset_in_page(pos), PAGE_SIZE));
		struct page *page;

		pos -= bytes_to_copy;
		arg -= bytes_to_copy;
		len -= bytes_to_copy;

		page = get_arg_page(bprm, pos, 1);
		if (!page)
			return -E2BIG;
		flush_arg_page(bprm, pos & PAGE_MASK, page);
		memcpy_to_page(page, offset_in_page(pos), arg, bytes_to_copy);
		put_arg_page(page);
	}

	return 0;
}
EXPORT_SYMBOL(copy_string_kernel);

static int copy_strings_kernel(int argc, const char *const *argv,
			       struct linux_binprm *bprm)
{
	while (argc-- > 0) {
		int ret = copy_string_kernel(argv[argc], bprm);
		if (ret < 0)
			return ret;
		if (fatal_signal_pending(current))
			return -ERESTARTNOHAND;
		cond_resched();
	}
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_MMU

/*
 * During bprm_mm_init(), we create a temporary stack at STACK_TOP_MAX.  Once
 * the binfmt code determines where the new stack should reside, we shift it to
 * its final location.  The process proceeds as follows:
 *
 * 1) Use shift to calculate the new vma endpoints.
 * 2) Extend vma to cover both the old and new ranges.  This ensures the
 *    arguments passed to subsequent functions are consistent.
 * 3) Move vma's page tables to the new range.
 * 4) Free up any cleared pgd range.
 * 5) Shrink the vma to cover only the new range.
 */
static int shift_arg_pages(struct vm_area_struct *vma, unsigned long shift)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned long old_start = vma->vm_start;
	unsigned long old_end = vma->vm_end;
	unsigned long length = old_end - old_start;
	unsigned long new_start = old_start - shift;
	unsigned long new_end = old_end - shift;
<<<<<<< HEAD
=======
	VMA_ITERATOR(vmi, mm, new_start);
	struct vm_area_struct *next;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mmu_gather tlb;

	BUG_ON(new_start > new_end);

	/*
	 * ensure there are no vmas between where we want to go
	 * and where we are
	 */
<<<<<<< HEAD
	if (vma != find_vma(mm, new_start))
		return -EFAULT;

	/*
	 * cover the whole range: [new_start, old_end)
	 */
	if (vma_adjust(vma, new_start, old_end, vma->vm_pgoff, NULL))
=======
	if (vma != vma_next(&vmi))
		return -EFAULT;

	vma_iter_prev_range(&vmi);
	/*
	 * cover the whole range: [new_start, old_end)
	 */
	if (vma_expand(&vmi, vma, new_start, old_end, vma->vm_pgoff, NULL))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	/*
	 * move the page tables downwards, on failure we rely on
	 * process cleanup to remove whatever mess we made.
	 */
	if (length != move_page_tables(vma, old_start,
<<<<<<< HEAD
				       vma, new_start, length))
		return -ENOMEM;

	lru_add_drain();
	tlb_gather_mmu(&tlb, mm, 0);
=======
				       vma, new_start, length, false, true))
		return -ENOMEM;

	lru_add_drain();
	tlb_gather_mmu(&tlb, mm);
	next = vma_next(&vmi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (new_end > old_start) {
		/*
		 * when the old and new regions overlap clear from new_end.
		 */
		free_pgd_range(&tlb, new_end, old_end, new_end,
<<<<<<< HEAD
			vma->vm_next ? vma->vm_next->vm_start : USER_PGTABLES_CEILING);
=======
			next ? next->vm_start : USER_PGTABLES_CEILING);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		/*
		 * otherwise, clean from old_start; this is done to not touch
		 * the address space in [new_end, old_start) some architectures
		 * have constraints on va-space that make this illegal (IA64) -
		 * for the others its just a little faster.
		 */
		free_pgd_range(&tlb, old_start, old_end, new_end,
<<<<<<< HEAD
			vma->vm_next ? vma->vm_next->vm_start : USER_PGTABLES_CEILING);
	}
	tlb_finish_mmu(&tlb, new_end, old_end);

	/*
	 * Shrink the vma to just the new range.  Always succeeds.
	 */
	vma_adjust(vma, new_start, new_end, vma->vm_pgoff, NULL);

	return 0;
=======
			next ? next->vm_start : USER_PGTABLES_CEILING);
	}
	tlb_finish_mmu(&tlb);

	vma_prev(&vmi);
	/* Shrink the vma to just the new range */
	return vma_shrink(&vmi, vma, new_start, new_end, vma->vm_pgoff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Finalizes the stack vm_area_struct. The flags and permissions are updated,
 * the stack is optionally relocated, and some extra space is added.
 */
int setup_arg_pages(struct linux_binprm *bprm,
		    unsigned long stack_top,
		    int executable_stack)
{
	unsigned long ret;
	unsigned long stack_shift;
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma = bprm->vma;
	struct vm_area_struct *prev = NULL;
	unsigned long vm_flags;
	unsigned long stack_base;
	unsigned long stack_size;
	unsigned long stack_expand;
	unsigned long rlim_stack;
<<<<<<< HEAD

#ifdef CONFIG_STACK_GROWSUP
	/* Limit stack size to 1GB */
	stack_base = rlimit_max(RLIMIT_STACK);
	if (stack_base > (1 << 30))
		stack_base = 1 << 30;
=======
	struct mmu_gather tlb;
	struct vma_iterator vmi;

#ifdef CONFIG_STACK_GROWSUP
	/* Limit stack size */
	stack_base = bprm->rlim_stack.rlim_max;

	stack_base = calc_max_stack_size(stack_base);

	/* Add space for stack randomization. */
	stack_base += (STACK_RND_MASK << PAGE_SHIFT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Make sure we didn't let the argument array grow too large. */
	if (vma->vm_end - vma->vm_start > stack_base)
		return -ENOMEM;

	stack_base = PAGE_ALIGN(stack_top - stack_base);

	stack_shift = vma->vm_start - stack_base;
	mm->arg_start = bprm->p - stack_shift;
	bprm->p = vma->vm_end - stack_shift;
#else
	stack_top = arch_align_stack(stack_top);
	stack_top = PAGE_ALIGN(stack_top);

	if (unlikely(stack_top < mmap_min_addr) ||
	    unlikely(vma->vm_end - vma->vm_start >= stack_top - mmap_min_addr))
		return -ENOMEM;

	stack_shift = vma->vm_end - stack_top;

	bprm->p -= stack_shift;
	mm->arg_start = bprm->p;
#endif

	if (bprm->loader)
		bprm->loader -= stack_shift;
	bprm->exec -= stack_shift;

<<<<<<< HEAD
	down_write(&mm->mmap_sem);
=======
	if (mmap_write_lock_killable(mm))
		return -EINTR;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vm_flags = VM_STACK_FLAGS;

	/*
	 * Adjust stack execute permissions; explicitly enable for
	 * EXSTACK_ENABLE_X, disable for EXSTACK_DISABLE_X and leave alone
	 * (arch default) otherwise.
	 */
	if (unlikely(executable_stack == EXSTACK_ENABLE_X))
		vm_flags |= VM_EXEC;
	else if (executable_stack == EXSTACK_DISABLE_X)
		vm_flags &= ~VM_EXEC;
	vm_flags |= mm->def_flags;
	vm_flags |= VM_STACK_INCOMPLETE_SETUP;

<<<<<<< HEAD
	ret = mprotect_fixup(vma, &prev, vma->vm_start, vma->vm_end,
			vm_flags);
=======
	vma_iter_init(&vmi, mm, vma->vm_start);

	tlb_gather_mmu(&tlb, mm);
	ret = mprotect_fixup(&vmi, &tlb, vma, &prev, vma->vm_start, vma->vm_end,
			vm_flags);
	tlb_finish_mmu(&tlb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		goto out_unlock;
	BUG_ON(prev != vma);

<<<<<<< HEAD
=======
	if (unlikely(vm_flags & VM_EXEC)) {
		pr_warn_once("process '%pD4' started with executable stack\n",
			     bprm->file);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Move stack pages down in memory. */
	if (stack_shift) {
		ret = shift_arg_pages(vma, stack_shift);
		if (ret)
			goto out_unlock;
	}

	/* mprotect_fixup is overkill to remove the temporary stack flags */
<<<<<<< HEAD
	vma->vm_flags &= ~VM_STACK_INCOMPLETE_SETUP;
=======
	vm_flags_clear(vma, VM_STACK_INCOMPLETE_SETUP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	stack_expand = 131072UL; /* randomly 32*4k (or 2*64k) pages */
	stack_size = vma->vm_end - vma->vm_start;
	/*
	 * Align this down to a page boundary as expand_stack
	 * will align it up.
	 */
<<<<<<< HEAD
	rlim_stack = rlimit(RLIMIT_STACK) & PAGE_MASK;
#ifdef CONFIG_STACK_GROWSUP
	if (stack_size + stack_expand > rlim_stack)
		stack_base = vma->vm_start + rlim_stack;
	else
		stack_base = vma->vm_end + stack_expand;
#else
	if (stack_size + stack_expand > rlim_stack)
		stack_base = vma->vm_end - rlim_stack;
	else
		stack_base = vma->vm_start - stack_expand;
#endif
	current->mm->start_stack = bprm->p;
	ret = expand_stack(vma, stack_base);
=======
	rlim_stack = bprm->rlim_stack.rlim_cur & PAGE_MASK;

	stack_expand = min(rlim_stack, stack_size + stack_expand);

#ifdef CONFIG_STACK_GROWSUP
	stack_base = vma->vm_start + stack_expand;
#else
	stack_base = vma->vm_end - stack_expand;
#endif
	current->mm->start_stack = bprm->p;
	ret = expand_stack_locked(vma, stack_base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		ret = -EFAULT;

out_unlock:
<<<<<<< HEAD
	up_write(&mm->mmap_sem);
=======
	mmap_write_unlock(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(setup_arg_pages);

<<<<<<< HEAD
#endif /* CONFIG_MMU */

struct file *open_exec(const char *name)
{
	struct file *file;
	int err;
	struct filename tmp = { .name = name };
	static const struct open_flags open_exec_flags = {
		.open_flag = O_LARGEFILE | O_RDONLY | __FMODE_EXEC,
		.acc_mode = MAY_EXEC | MAY_OPEN,
		.intent = LOOKUP_OPEN
	};

	file = do_filp_open(AT_FDCWD, &tmp, &open_exec_flags, LOOKUP_FOLLOW);
	if (IS_ERR(file))
		goto out;

	err = -EACCES;
	if (!S_ISREG(file->f_path.dentry->d_inode->i_mode))
		goto exit;

	if (file->f_path.mnt->mnt_flags & MNT_NOEXEC)
		goto exit;

	fsnotify_open(file);
/* LGE_CHANGE_S
 *
 * do read/mmap profiling during booting
 * in order to use the data as readahead args
 *
 * byungchul.park@lge.com 20120503
 */
	sreadahead_prof(file, 0, 0);
/* LGE_CHANGE_E */


=======
#else

/*
 * Transfer the program arguments and environment from the holding pages
 * onto the stack. The provided stack pointer is adjusted accordingly.
 */
int transfer_args_to_stack(struct linux_binprm *bprm,
			   unsigned long *sp_location)
{
	unsigned long index, stop, sp;
	int ret = 0;

	stop = bprm->p >> PAGE_SHIFT;
	sp = *sp_location;

	for (index = MAX_ARG_PAGES - 1; index >= stop; index--) {
		unsigned int offset = index == stop ? bprm->p & ~PAGE_MASK : 0;
		char *src = kmap_local_page(bprm->page[index]) + offset;
		sp -= PAGE_SIZE - offset;
		if (copy_to_user((void *) sp, src, PAGE_SIZE - offset) != 0)
			ret = -EFAULT;
		kunmap_local(src);
		if (ret)
			goto out;
	}

	bprm->exec += *sp_location - MAX_ARG_PAGES * PAGE_SIZE;
	*sp_location = sp;

out:
	return ret;
}
EXPORT_SYMBOL(transfer_args_to_stack);

#endif /* CONFIG_MMU */

/*
 * On success, caller must call do_close_execat() on the returned
 * struct file to close it.
 */
static struct file *do_open_execat(int fd, struct filename *name, int flags)
{
	struct file *file;
	int err;
	struct open_flags open_exec_flags = {
		.open_flag = O_LARGEFILE | O_RDONLY | __FMODE_EXEC,
		.acc_mode = MAY_EXEC,
		.intent = LOOKUP_OPEN,
		.lookup_flags = LOOKUP_FOLLOW,
	};

	if ((flags & ~(AT_SYMLINK_NOFOLLOW | AT_EMPTY_PATH)) != 0)
		return ERR_PTR(-EINVAL);
	if (flags & AT_SYMLINK_NOFOLLOW)
		open_exec_flags.lookup_flags &= ~LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
		open_exec_flags.lookup_flags |= LOOKUP_EMPTY;

	file = do_filp_open(fd, name, &open_exec_flags);
	if (IS_ERR(file))
		goto out;

	/*
	 * may_open() has already checked for this, so it should be
	 * impossible to trip now. But we need to be extra cautious
	 * and check again at the very end too.
	 */
	err = -EACCES;
	if (WARN_ON_ONCE(!S_ISREG(file_inode(file)->i_mode) ||
			 path_noexec(&file->f_path)))
		goto exit;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = deny_write_access(file);
	if (err)
		goto exit;

out:
	return file;

exit:
	fput(file);
	return ERR_PTR(err);
}
<<<<<<< HEAD
EXPORT_SYMBOL(open_exec);

int kernel_read(struct file *file, loff_t offset,
		char *addr, unsigned long count)
{
	mm_segment_t old_fs;
	loff_t pos = offset;
	int result;

	old_fs = get_fs();
	set_fs(get_ds());
	/* The cast to a user pointer is valid due to the set_fs() */
	result = vfs_read(file, (void __user *)addr, count, &pos);
	set_fs(old_fs);
	return result;
}

EXPORT_SYMBOL(kernel_read);

static int exec_mmap(struct mm_struct *mm)
{
	struct task_struct *tsk;
	struct mm_struct * old_mm, *active_mm;
=======

/**
 * open_exec - Open a path name for execution
 *
 * @name: path name to open with the intent of executing it.
 *
 * Returns ERR_PTR on failure or allocated struct file on success.
 *
 * As this is a wrapper for the internal do_open_execat(), callers
 * must call allow_write_access() before fput() on release. Also see
 * do_close_execat().
 */
struct file *open_exec(const char *name)
{
	struct filename *filename = getname_kernel(name);
	struct file *f = ERR_CAST(filename);

	if (!IS_ERR(filename)) {
		f = do_open_execat(AT_FDCWD, filename, 0);
		putname(filename);
	}
	return f;
}
EXPORT_SYMBOL(open_exec);

#if defined(CONFIG_BINFMT_FLAT) || defined(CONFIG_BINFMT_ELF_FDPIC)
ssize_t read_code(struct file *file, unsigned long addr, loff_t pos, size_t len)
{
	ssize_t res = vfs_read(file, (void __user *)addr, len, &pos);
	if (res > 0)
		flush_icache_user_range(addr, addr + len);
	return res;
}
EXPORT_SYMBOL(read_code);
#endif

/*
 * Maps the mm_struct mm into the current task struct.
 * On success, this function returns with exec_update_lock
 * held for writing.
 */
static int exec_mmap(struct mm_struct *mm)
{
	struct task_struct *tsk;
	struct mm_struct *old_mm, *active_mm;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Notify parent that we're no longer interested in the old VM */
	tsk = current;
	old_mm = current->mm;
<<<<<<< HEAD
	mm_release(tsk, old_mm);

	if (old_mm) {
		sync_mm_rss(old_mm);
		/*
		 * Make sure that if there is a core dump in progress
		 * for the old mm, we get out and die instead of going
		 * through with the exec.  We must hold mmap_sem around
		 * checking core_state and changing tsk->mm.
		 */
		down_read(&old_mm->mmap_sem);
		if (unlikely(old_mm->core_state)) {
			up_read(&old_mm->mmap_sem);
			return -EINTR;
		}
	}
	task_lock(tsk);
	active_mm = tsk->active_mm;
	tsk->mm = mm;
	tsk->active_mm = mm;
	activate_mm(active_mm, mm);
	task_unlock(tsk);
	arch_pick_mmap_layout(mm);
	if (old_mm) {
		up_read(&old_mm->mmap_sem);
=======
	exec_mm_release(tsk, old_mm);

	ret = down_write_killable(&tsk->signal->exec_update_lock);
	if (ret)
		return ret;

	if (old_mm) {
		/*
		 * If there is a pending fatal signal perhaps a signal
		 * whose default action is to create a coredump get
		 * out and die instead of going through with the exec.
		 */
		ret = mmap_read_lock_killable(old_mm);
		if (ret) {
			up_write(&tsk->signal->exec_update_lock);
			return ret;
		}
	}

	task_lock(tsk);
	membarrier_exec_mmap(mm);

	local_irq_disable();
	active_mm = tsk->active_mm;
	tsk->active_mm = mm;
	tsk->mm = mm;
	mm_init_cid(mm);
	/*
	 * This prevents preemption while active_mm is being loaded and
	 * it and mm are being updated, which could cause problems for
	 * lazy tlb mm refcounting when these are updated by context
	 * switches. Not all architectures can handle irqs off over
	 * activate_mm yet.
	 */
	if (!IS_ENABLED(CONFIG_ARCH_WANT_IRQS_OFF_ACTIVATE_MM))
		local_irq_enable();
	activate_mm(active_mm, mm);
	if (IS_ENABLED(CONFIG_ARCH_WANT_IRQS_OFF_ACTIVATE_MM))
		local_irq_enable();
	lru_gen_add_mm(mm);
	task_unlock(tsk);
	lru_gen_use_mm(mm);
	if (old_mm) {
		mmap_read_unlock(old_mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		BUG_ON(active_mm != old_mm);
		setmax_mm_hiwater_rss(&tsk->signal->maxrss, old_mm);
		mm_update_next_owner(old_mm);
		mmput(old_mm);
		return 0;
	}
<<<<<<< HEAD
	mmdrop(active_mm);
	return 0;
}

/*
 * This function makes sure the current process has its own signal table,
 * so that flush_signal_handlers can later reset the handlers without
 * disturbing other processes.  (Other processes might share the signal
 * table via the CLONE_SIGHAND option to clone().)
 */
=======
	mmdrop_lazy_tlb(active_mm);
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int de_thread(struct task_struct *tsk)
{
	struct signal_struct *sig = tsk->signal;
	struct sighand_struct *oldsighand = tsk->sighand;
	spinlock_t *lock = &oldsighand->siglock;

	if (thread_group_empty(tsk))
		goto no_thread_group;

	/*
	 * Kill all other threads in the thread group.
	 */
	spin_lock_irq(lock);
<<<<<<< HEAD
	if (signal_group_exit(sig)) {
=======
	if ((sig->flags & SIGNAL_GROUP_EXIT) || sig->group_exec_task) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Another group action in progress, just
		 * return so that the signal is processed.
		 */
		spin_unlock_irq(lock);
		return -EAGAIN;
	}

<<<<<<< HEAD
	sig->group_exit_task = tsk;
=======
	sig->group_exec_task = tsk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sig->notify_count = zap_other_threads(tsk);
	if (!thread_group_leader(tsk))
		sig->notify_count--;

	while (sig->notify_count) {
<<<<<<< HEAD
		__set_current_state(TASK_UNINTERRUPTIBLE);
		spin_unlock_irq(lock);
		schedule();
=======
		__set_current_state(TASK_KILLABLE);
		spin_unlock_irq(lock);
		schedule();
		if (__fatal_signal_pending(tsk))
			goto killed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_lock_irq(lock);
	}
	spin_unlock_irq(lock);

	/*
	 * At this point all other threads have exited, all we have to
	 * do is to wait for the thread group leader to become inactive,
	 * and to assume its PID:
	 */
	if (!thread_group_leader(tsk)) {
		struct task_struct *leader = tsk->group_leader;

<<<<<<< HEAD
		sig->notify_count = -1;	/* for exit_notify() */
		for (;;) {
			threadgroup_change_begin(tsk);
			write_lock_irq(&tasklist_lock);
			if (likely(leader->exit_state))
				break;
			__set_current_state(TASK_UNINTERRUPTIBLE);
			write_unlock_irq(&tasklist_lock);
			threadgroup_change_end(tsk);
			schedule();
=======
		for (;;) {
			cgroup_threadgroup_change_begin(tsk);
			write_lock_irq(&tasklist_lock);
			/*
			 * Do this under tasklist_lock to ensure that
			 * exit_notify() can't miss ->group_exec_task
			 */
			sig->notify_count = -1;
			if (likely(leader->exit_state))
				break;
			__set_current_state(TASK_KILLABLE);
			write_unlock_irq(&tasklist_lock);
			cgroup_threadgroup_change_end(tsk);
			schedule();
			if (__fatal_signal_pending(tsk))
				goto killed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		/*
		 * The only record we have of the real-time age of a
		 * process, regardless of execs it's done, is start_time.
		 * All the past CPU time is accumulated in signal_struct
		 * from sister threads now dead.  But in this non-leader
		 * exec, nothing survives from the original leader thread,
		 * whose birth marks the true age of this process now.
		 * When we take on its identity by switching to its PID, we
		 * also take its birthdate (always earlier than our own).
		 */
		tsk->start_time = leader->start_time;
<<<<<<< HEAD

		BUG_ON(!same_thread_group(leader, tsk));
		BUG_ON(has_group_leader_pid(tsk));
=======
		tsk->start_boottime = leader->start_boottime;

		BUG_ON(!same_thread_group(leader, tsk));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * An exec() starts a new thread group with the
		 * TGID of the previous thread group. Rehash the
		 * two threads with a switched PID, and release
		 * the former thread group leader:
		 */

		/* Become a process group leader with the old leader's pid.
		 * The old leader becomes a thread of the this thread group.
<<<<<<< HEAD
		 * Note: The old leader also uses this pid until release_task
		 *       is called.  Odd but simple and correct.
		 */
		detach_pid(tsk, PIDTYPE_PID);
		tsk->pid = leader->pid;
		attach_pid(tsk, PIDTYPE_PID,  task_pid(leader));
=======
		 */
		exchange_tids(tsk, leader);
		transfer_pid(leader, tsk, PIDTYPE_TGID);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		transfer_pid(leader, tsk, PIDTYPE_PGID);
		transfer_pid(leader, tsk, PIDTYPE_SID);

		list_replace_rcu(&leader->tasks, &tsk->tasks);
		list_replace_init(&leader->sibling, &tsk->sibling);

		tsk->group_leader = tsk;
		leader->group_leader = tsk;

		tsk->exit_signal = SIGCHLD;
		leader->exit_signal = -1;

		BUG_ON(leader->exit_state != EXIT_ZOMBIE);
		leader->exit_state = EXIT_DEAD;
<<<<<<< HEAD

		/*
		 * We are going to release_task()->ptrace_unlink() silently,
		 * the tracer can sleep in do_wait(). EXIT_DEAD guarantees
		 * the tracer wont't block again waiting for this thread.
=======
		/*
		 * We are going to release_task()->ptrace_unlink() silently,
		 * the tracer can sleep in do_wait(). EXIT_DEAD guarantees
		 * the tracer won't block again waiting for this thread.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 */
		if (unlikely(leader->ptrace))
			__wake_up_parent(leader, leader->parent);
		write_unlock_irq(&tasklist_lock);
<<<<<<< HEAD
		threadgroup_change_end(tsk);
=======
		cgroup_threadgroup_change_end(tsk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		release_task(leader);
	}

<<<<<<< HEAD
	sig->group_exit_task = NULL;
=======
	sig->group_exec_task = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sig->notify_count = 0;

no_thread_group:
	/* we have changed execution domain */
	tsk->exit_signal = SIGCHLD;

<<<<<<< HEAD
	exit_itimers(sig);
	flush_itimer_signals();

	if (atomic_read(&oldsighand->count) != 1) {
=======
	BUG_ON(!thread_group_leader(tsk));
	return 0;

killed:
	/* protects against exit_notify() and __exit_signal() */
	read_lock(&tasklist_lock);
	sig->group_exec_task = NULL;
	sig->notify_count = 0;
	read_unlock(&tasklist_lock);
	return -EAGAIN;
}


/*
 * This function makes sure the current process has its own signal table,
 * so that flush_signal_handlers can later reset the handlers without
 * disturbing other processes.  (Other processes might share the signal
 * table via the CLONE_SIGHAND option to clone().)
 */
static int unshare_sighand(struct task_struct *me)
{
	struct sighand_struct *oldsighand = me->sighand;

	if (refcount_read(&oldsighand->count) != 1) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct sighand_struct *newsighand;
		/*
		 * This ->sighand is shared with the CLONE_SIGHAND
		 * but not CLONE_THREAD task, switch to the new one.
		 */
		newsighand = kmem_cache_alloc(sighand_cachep, GFP_KERNEL);
		if (!newsighand)
			return -ENOMEM;

<<<<<<< HEAD
		atomic_set(&newsighand->count, 1);
		memcpy(newsighand->action, oldsighand->action,
		       sizeof(newsighand->action));

		write_lock_irq(&tasklist_lock);
		spin_lock(&oldsighand->siglock);
		rcu_assign_pointer(tsk->sighand, newsighand);
=======
		refcount_set(&newsighand->count, 1);

		write_lock_irq(&tasklist_lock);
		spin_lock(&oldsighand->siglock);
		memcpy(newsighand->action, oldsighand->action,
		       sizeof(newsighand->action));
		rcu_assign_pointer(me->sighand, newsighand);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock(&oldsighand->siglock);
		write_unlock_irq(&tasklist_lock);

		__cleanup_sighand(oldsighand);
	}
<<<<<<< HEAD

	BUG_ON(!thread_group_leader(tsk));
	return 0;
}

=======
	return 0;
}

char *__get_task_comm(char *buf, size_t buf_size, struct task_struct *tsk)
{
	task_lock(tsk);
	/* Always NUL terminated and zero-padded */
	strscpy_pad(buf, tsk->comm, buf_size);
	task_unlock(tsk);
	return buf;
}
EXPORT_SYMBOL_GPL(__get_task_comm);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * These functions flushes out all traces of the currently running executable
 * so that a new one can be started
 */
<<<<<<< HEAD
static void flush_old_files(struct files_struct * files)
{
	long j = -1;
	struct fdtable *fdt;

	spin_lock(&files->file_lock);
	for (;;) {
		unsigned long set, i;

		j++;
		i = j * BITS_PER_LONG;
		fdt = files_fdtable(files);
		if (i >= fdt->max_fds)
			break;
		set = fdt->close_on_exec[j];
		if (!set)
			continue;
		fdt->close_on_exec[j] = 0;
		spin_unlock(&files->file_lock);
		for ( ; set ; i++,set >>= 1) {
			if (set & 1) {
				sys_close(i);
			}
		}
		spin_lock(&files->file_lock);

	}
	spin_unlock(&files->file_lock);
}

char *get_task_comm(char *buf, struct task_struct *tsk)
{
	/* buf must be at least sizeof(tsk->comm) in size */
	task_lock(tsk);
	strncpy(buf, tsk->comm, sizeof(tsk->comm));
	task_unlock(tsk);
	return buf;
}
EXPORT_SYMBOL_GPL(get_task_comm);

void set_task_comm(struct task_struct *tsk, char *buf)
{
	task_lock(tsk);

	trace_task_rename(tsk, buf);

	/*
	 * Threads may access current->comm without holding
	 * the task lock, so write the string carefully.
	 * Readers without a lock may see incomplete new
	 * names but are safe from non-terminating string reads.
	 */
	memset(tsk->comm, 0, TASK_COMM_LEN);
	wmb();
	strlcpy(tsk->comm, buf, sizeof(tsk->comm));
	task_unlock(tsk);
	perf_event_comm(tsk);
}

static void filename_to_taskname(char *tcomm, const char *fn, unsigned int len)
{
	int i, ch;

	/* Copies the binary name from after last slash */
	for (i = 0; (ch = *(fn++)) != '\0';) {
		if (ch == '/')
			i = 0; /* overwrite what we wrote */
		else
			if (i < len - 1)
				tcomm[i++] = ch;
	}
	tcomm[i] = '\0';
}

int flush_old_exec(struct linux_binprm * bprm)
{
	int retval;

	/*
	 * Make sure we have a private signal table and that
	 * we are unassociated from the previous thread group.
	 */
	retval = de_thread(current);
	if (retval)
		goto out;

	set_mm_exe_file(bprm->mm, bprm->file);

	filename_to_taskname(bprm->tcomm, bprm->filename, sizeof(bprm->tcomm));
=======

void __set_task_comm(struct task_struct *tsk, const char *buf, bool exec)
{
	task_lock(tsk);
	trace_task_rename(tsk, buf);
	strscpy_pad(tsk->comm, buf, sizeof(tsk->comm));
	task_unlock(tsk);
	perf_event_comm(tsk, exec);
}

/*
 * Calling this is the point of no return. None of the failures will be
 * seen by userspace since either the process is already taking a fatal
 * signal (via de_thread() or coredump), or will have SEGV raised
 * (after exec_mmap()) by search_binary_handler (see below).
 */
int begin_new_exec(struct linux_binprm * bprm)
{
	struct task_struct *me = current;
	int retval;

	/* Once we are committed compute the creds */
	retval = bprm_creds_from_file(bprm);
	if (retval)
		return retval;

	/*
	 * Ensure all future errors are fatal.
	 */
	bprm->point_of_no_return = true;

	/*
	 * Make this the only thread in the thread group.
	 */
	retval = de_thread(me);
	if (retval)
		goto out;

	/*
	 * Cancel any io_uring activity across execve
	 */
	io_uring_task_cancel();

	/* Ensure the files table is not shared. */
	retval = unshare_files();
	if (retval)
		goto out;

	/*
	 * Must be called _before_ exec_mmap() as bprm->mm is
	 * not visible until then. Doing it here also ensures
	 * we don't race against replace_mm_exe_file().
	 */
	retval = set_mm_exe_file(bprm->mm, bprm->file);
	if (retval)
		goto out;

	/* If the binary is not readable then enforce mm->dumpable=0 */
	would_dump(bprm, bprm->file);
	if (bprm->have_execfd)
		would_dump(bprm, bprm->executable);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Release all of the old mmap stuff
	 */
	acct_arg_size(bprm, 0);
	retval = exec_mmap(bprm->mm);
	if (retval)
		goto out;

<<<<<<< HEAD
	bprm->mm = NULL;		/* We're using it now */

	set_fs(USER_DS);
	current->flags &=
		~(PF_RANDOMIZE | PF_FORKNOEXEC | PF_KTHREAD | PF_NOFREEZE);
	flush_thread();
	current->personality &= ~bprm->per_clear;

	return 0;

out:
	return retval;
}
EXPORT_SYMBOL(flush_old_exec);

void would_dump(struct linux_binprm *bprm, struct file *file)
{
	if (inode_permission2(file->f_path.mnt, file->f_path.dentry->d_inode, MAY_READ) < 0)
		bprm->interp_flags |= BINPRM_FLAGS_ENFORCE_NONDUMP;
=======
	bprm->mm = NULL;

	retval = exec_task_namespaces();
	if (retval)
		goto out_unlock;

#ifdef CONFIG_POSIX_TIMERS
	spin_lock_irq(&me->sighand->siglock);
	posix_cpu_timers_exit(me);
	spin_unlock_irq(&me->sighand->siglock);
	exit_itimers(me);
	flush_itimer_signals();
#endif

	/*
	 * Make the signal table private.
	 */
	retval = unshare_sighand(me);
	if (retval)
		goto out_unlock;

	me->flags &= ~(PF_RANDOMIZE | PF_FORKNOEXEC |
					PF_NOFREEZE | PF_NO_SETAFFINITY);
	flush_thread();
	me->personality &= ~bprm->per_clear;

	clear_syscall_work_syscall_user_dispatch(me);

	/*
	 * We have to apply CLOEXEC before we change whether the process is
	 * dumpable (in setup_new_exec) to avoid a race with a process in userspace
	 * trying to access the should-be-closed file descriptors of a process
	 * undergoing exec(2).
	 */
	do_close_on_exec(me->files);

	if (bprm->secureexec) {
		/* Make sure parent cannot signal privileged process. */
		me->pdeath_signal = 0;

		/*
		 * For secureexec, reset the stack limit to sane default to
		 * avoid bad behavior from the prior rlimits. This has to
		 * happen before arch_pick_mmap_layout(), which examines
		 * RLIMIT_STACK, but after the point of no return to avoid
		 * needing to clean up the change on failure.
		 */
		if (bprm->rlim_stack.rlim_cur > _STK_LIM)
			bprm->rlim_stack.rlim_cur = _STK_LIM;
	}

	me->sas_ss_sp = me->sas_ss_size = 0;

	/*
	 * Figure out dumpability. Note that this checking only of current
	 * is wrong, but userspace depends on it. This should be testing
	 * bprm->secureexec instead.
	 */
	if (bprm->interp_flags & BINPRM_FLAGS_ENFORCE_NONDUMP ||
	    !(uid_eq(current_euid(), current_uid()) &&
	      gid_eq(current_egid(), current_gid())))
		set_dumpable(current->mm, suid_dumpable);
	else
		set_dumpable(current->mm, SUID_DUMP_USER);

	perf_event_exec();
	__set_task_comm(me, kbasename(bprm->filename), true);

	/* An exec changes our domain. We are no longer part of the thread
	   group */
	WRITE_ONCE(me->self_exec_id, me->self_exec_id + 1);
	flush_signal_handlers(me, 0);

	retval = set_cred_ucounts(bprm->cred);
	if (retval < 0)
		goto out_unlock;

	/*
	 * install the new credentials for this executable
	 */
	security_bprm_committing_creds(bprm);

	commit_creds(bprm->cred);
	bprm->cred = NULL;

	/*
	 * Disable monitoring for regular users
	 * when executing setuid binaries. Must
	 * wait until new credentials are committed
	 * by commit_creds() above
	 */
	if (get_dumpable(me->mm) != SUID_DUMP_USER)
		perf_event_exit_task(me);
	/*
	 * cred_guard_mutex must be held at least to this point to prevent
	 * ptrace_attach() from altering our determination of the task's
	 * credentials; any time after this it may be unlocked.
	 */
	security_bprm_committed_creds(bprm);

	/* Pass the opened binary to the interpreter. */
	if (bprm->have_execfd) {
		retval = get_unused_fd_flags(0);
		if (retval < 0)
			goto out_unlock;
		fd_install(retval, bprm->executable);
		bprm->executable = NULL;
		bprm->execfd = retval;
	}
	return 0;

out_unlock:
	up_write(&me->signal->exec_update_lock);
	if (!bprm->cred)
		mutex_unlock(&me->signal->cred_guard_mutex);

out:
	return retval;
}
EXPORT_SYMBOL(begin_new_exec);

void would_dump(struct linux_binprm *bprm, struct file *file)
{
	struct inode *inode = file_inode(file);
	struct mnt_idmap *idmap = file_mnt_idmap(file);
	if (inode_permission(idmap, inode, MAY_READ) < 0) {
		struct user_namespace *old, *user_ns;
		bprm->interp_flags |= BINPRM_FLAGS_ENFORCE_NONDUMP;

		/* Ensure mm->user_ns contains the executable */
		user_ns = old = bprm->mm->user_ns;
		while ((user_ns != &init_user_ns) &&
		       !privileged_wrt_inode_uidgid(user_ns, idmap, inode))
			user_ns = user_ns->parent;

		if (old != user_ns) {
			bprm->mm->user_ns = get_user_ns(user_ns);
			put_user_ns(old);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(would_dump);

void setup_new_exec(struct linux_binprm * bprm)
{
<<<<<<< HEAD
	arch_pick_mmap_layout(current->mm);

	/* This is the point of no return */
	current->sas_ss_sp = current->sas_ss_size = 0;

	if (current_euid() == current_uid() && current_egid() == current_gid())
		set_dumpable(current->mm, 1);
	else
		set_dumpable(current->mm, suid_dumpable);

	set_task_comm(current, bprm->tcomm);
=======
	/* Setup things that can depend upon the personality */
	struct task_struct *me = current;

	arch_pick_mmap_layout(me->mm, &bprm->rlim_stack);

	arch_setup_new_exec();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set the new mm task size. We have to do that late because it may
	 * depend on TIF_32BIT which is only updated in flush_thread() on
	 * some architectures like powerpc
	 */
<<<<<<< HEAD
	current->mm->task_size = TASK_SIZE;

	/* install the new credentials */
	if (bprm->cred->uid != current_euid() ||
	    bprm->cred->gid != current_egid()) {
		current->pdeath_signal = 0;
	} else {
		would_dump(bprm, bprm->file);
		if (bprm->interp_flags & BINPRM_FLAGS_ENFORCE_NONDUMP)
			set_dumpable(current->mm, suid_dumpable);
	}

	/* An exec changes our domain. We are no longer part of the thread
	   group */

	current->self_exec_id++;
			
	flush_signal_handlers(current, 0);
	flush_old_files(current->files);
}
EXPORT_SYMBOL(setup_new_exec);

/*
 * Prepare credentials and lock ->cred_guard_mutex.
 * install_exec_creds() commits the new creds and drops the lock.
 * Or, if exec fails before, free_bprm() should release ->cred and
 * and unlock.
 */
int prepare_bprm_creds(struct linux_binprm *bprm)
=======
	me->mm->task_size = TASK_SIZE;
	up_write(&me->signal->exec_update_lock);
	mutex_unlock(&me->signal->cred_guard_mutex);
}
EXPORT_SYMBOL(setup_new_exec);

/* Runs immediately before start_thread() takes over. */
void finalize_exec(struct linux_binprm *bprm)
{
	/* Store any stack rlimit changes before starting thread. */
	task_lock(current->group_leader);
	current->signal->rlim[RLIMIT_STACK] = bprm->rlim_stack;
	task_unlock(current->group_leader);
}
EXPORT_SYMBOL(finalize_exec);

/*
 * Prepare credentials and lock ->cred_guard_mutex.
 * setup_new_exec() commits the new creds and drops the lock.
 * Or, if exec fails before, free_bprm() should release ->cred
 * and unlock.
 */
static int prepare_bprm_creds(struct linux_binprm *bprm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (mutex_lock_interruptible(&current->signal->cred_guard_mutex))
		return -ERESTARTNOINTR;

	bprm->cred = prepare_exec_creds();
	if (likely(bprm->cred))
		return 0;

	mutex_unlock(&current->signal->cred_guard_mutex);
	return -ENOMEM;
}

<<<<<<< HEAD
void free_bprm(struct linux_binprm *bprm)
{
=======
/* Matches do_open_execat() */
static void do_close_execat(struct file *file)
{
	if (!file)
		return;
	allow_write_access(file);
	fput(file);
}

static void free_bprm(struct linux_binprm *bprm)
{
	if (bprm->mm) {
		acct_arg_size(bprm, 0);
		mmput(bprm->mm);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	free_arg_pages(bprm);
	if (bprm->cred) {
		mutex_unlock(&current->signal->cred_guard_mutex);
		abort_creds(bprm->cred);
	}
<<<<<<< HEAD
	/* If a binfmt changed the interp, free it. */
	if (bprm->interp != bprm->filename)
		kfree(bprm->interp);
	kfree(bprm);
}

int bprm_change_interp(char *interp, struct linux_binprm *bprm)
=======
	do_close_execat(bprm->file);
	if (bprm->executable)
		fput(bprm->executable);
	/* If a binfmt changed the interp, free it. */
	if (bprm->interp != bprm->filename)
		kfree(bprm->interp);
	kfree(bprm->fdpath);
	kfree(bprm);
}

static struct linux_binprm *alloc_bprm(int fd, struct filename *filename, int flags)
{
	struct linux_binprm *bprm;
	struct file *file;
	int retval = -ENOMEM;

	file = do_open_execat(fd, filename, flags);
	if (IS_ERR(file))
		return ERR_CAST(file);

	bprm = kzalloc(sizeof(*bprm), GFP_KERNEL);
	if (!bprm) {
		do_close_execat(file);
		return ERR_PTR(-ENOMEM);
	}

	bprm->file = file;

	if (fd == AT_FDCWD || filename->name[0] == '/') {
		bprm->filename = filename->name;
	} else {
		if (filename->name[0] == '\0')
			bprm->fdpath = kasprintf(GFP_KERNEL, "/dev/fd/%d", fd);
		else
			bprm->fdpath = kasprintf(GFP_KERNEL, "/dev/fd/%d/%s",
						  fd, filename->name);
		if (!bprm->fdpath)
			goto out_free;

		/*
		 * Record that a name derived from an O_CLOEXEC fd will be
		 * inaccessible after exec.  This allows the code in exec to
		 * choose to fail when the executable is not mmaped into the
		 * interpreter and an open file descriptor is not passed to
		 * the interpreter.  This makes for a better user experience
		 * than having the interpreter start and then immediately fail
		 * when it finds the executable is inaccessible.
		 */
		if (get_close_on_exec(fd))
			bprm->interp_flags |= BINPRM_FLAGS_PATH_INACCESSIBLE;

		bprm->filename = bprm->fdpath;
	}
	bprm->interp = bprm->filename;

	retval = bprm_mm_init(bprm);
	if (!retval)
		return bprm;

out_free:
	free_bprm(bprm);
	return ERR_PTR(retval);
}

int bprm_change_interp(const char *interp, struct linux_binprm *bprm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* If a binfmt changed the interp, free it first. */
	if (bprm->interp != bprm->filename)
		kfree(bprm->interp);
	bprm->interp = kstrdup(interp, GFP_KERNEL);
	if (!bprm->interp)
		return -ENOMEM;
	return 0;
}
EXPORT_SYMBOL(bprm_change_interp);

/*
<<<<<<< HEAD
 * install the new credentials for this executable
 */
void install_exec_creds(struct linux_binprm *bprm)
{
	security_bprm_committing_creds(bprm);

	commit_creds(bprm->cred);
	bprm->cred = NULL;

	/*
	 * Disable monitoring for regular users
	 * when executing setuid binaries. Must
	 * wait until new credentials are committed
	 * by commit_creds() above
	 */
	if (get_dumpable(current->mm) != SUID_DUMP_USER)
		perf_event_exit_task(current);
	/*
	 * cred_guard_mutex must be held at least to this point to prevent
	 * ptrace_attach() from altering our determination of the task's
	 * credentials; any time after this it may be unlocked.
	 */
	security_bprm_committed_creds(bprm);
	mutex_unlock(&current->signal->cred_guard_mutex);
}
EXPORT_SYMBOL(install_exec_creds);

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * determine how safe it is to execute the proposed program
 * - the caller must hold ->cred_guard_mutex to protect against
 *   PTRACE_ATTACH or seccomp thread-sync
 */
<<<<<<< HEAD
static int check_unsafe_exec(struct linux_binprm *bprm)
{
	struct task_struct *p = current, *t;
	unsigned n_fs;
	int res = 0;

	if (p->ptrace) {
		if (p->ptrace & PT_PTRACE_CAP)
			bprm->unsafe |= LSM_UNSAFE_PTRACE_CAP;
		else
			bprm->unsafe |= LSM_UNSAFE_PTRACE;
	}
=======
static void check_unsafe_exec(struct linux_binprm *bprm)
{
	struct task_struct *p = current, *t;
	unsigned n_fs;

	if (p->ptrace)
		bprm->unsafe |= LSM_UNSAFE_PTRACE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * This isn't strictly necessary, but it makes it harder for LSMs to
	 * mess up.
	 */
	if (task_no_new_privs(current))
		bprm->unsafe |= LSM_UNSAFE_NO_NEW_PRIVS;

<<<<<<< HEAD
	n_fs = 1;
	spin_lock(&p->fs->lock);
	rcu_read_lock();
	for (t = next_thread(p); t != p; t = next_thread(t)) {
=======
	/*
	 * If another task is sharing our fs, we cannot safely
	 * suid exec because the differently privileged task
	 * will be able to manipulate the current directory, etc.
	 * It would be nice to force an unshare instead...
	 */
	n_fs = 1;
	spin_lock(&p->fs->lock);
	rcu_read_lock();
	for_other_threads(p, t) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (t->fs == p->fs)
			n_fs++;
	}
	rcu_read_unlock();

<<<<<<< HEAD
	if (p->fs->users > n_fs) {
		bprm->unsafe |= LSM_UNSAFE_SHARE;
	} else {
		res = -EAGAIN;
		if (!p->fs->in_exec) {
			p->fs->in_exec = 1;
			res = 1;
		}
	}
	spin_unlock(&p->fs->lock);

	return res;
}

static void bprm_fill_uid(struct linux_binprm *bprm)
{
	struct inode *inode;
	unsigned int mode;
	uid_t uid;
	gid_t gid;

	/* clear any previous set[ug]id data from a previous binary */
	bprm->cred->euid = current_euid();
	bprm->cred->egid = current_egid();

	if (bprm->file->f_path.mnt->mnt_flags & MNT_NOSUID)
=======
	/* "users" and "in_exec" locked for copy_fs() */
	if (p->fs->users > n_fs)
		bprm->unsafe |= LSM_UNSAFE_SHARE;
	else
		p->fs->in_exec = 1;
	spin_unlock(&p->fs->lock);
}

static void bprm_fill_uid(struct linux_binprm *bprm, struct file *file)
{
	/* Handle suid and sgid on files */
	struct mnt_idmap *idmap;
	struct inode *inode = file_inode(file);
	unsigned int mode;
	vfsuid_t vfsuid;
	vfsgid_t vfsgid;

	if (!mnt_may_suid(file->f_path.mnt))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	if (task_no_new_privs(current))
		return;

<<<<<<< HEAD
	inode = bprm->file->f_path.dentry->d_inode;
	mode = ACCESS_ONCE(inode->i_mode);
	if (!(mode & (S_ISUID|S_ISGID)))
		return;

	/* Be careful if suid/sgid is set */
	mutex_lock(&inode->i_mutex);

	/* reload atomically mode/uid/gid now that lock held */
	mode = inode->i_mode;
	uid = inode->i_uid;
	gid = inode->i_gid;
	mutex_unlock(&inode->i_mutex);

	if (mode & S_ISUID) {
		bprm->per_clear |= PER_CLEAR_ON_SETID;
		bprm->cred->euid = uid;
=======
	mode = READ_ONCE(inode->i_mode);
	if (!(mode & (S_ISUID|S_ISGID)))
		return;

	idmap = file_mnt_idmap(file);

	/* Be careful if suid/sgid is set */
	inode_lock(inode);

	/* reload atomically mode/uid/gid now that lock held */
	mode = inode->i_mode;
	vfsuid = i_uid_into_vfsuid(idmap, inode);
	vfsgid = i_gid_into_vfsgid(idmap, inode);
	inode_unlock(inode);

	/* We ignore suid/sgid if there are no mappings for them in the ns */
	if (!vfsuid_has_mapping(bprm->cred->user_ns, vfsuid) ||
	    !vfsgid_has_mapping(bprm->cred->user_ns, vfsgid))
		return;

	if (mode & S_ISUID) {
		bprm->per_clear |= PER_CLEAR_ON_SETID;
		bprm->cred->euid = vfsuid_into_kuid(vfsuid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if ((mode & (S_ISGID | S_IXGRP)) == (S_ISGID | S_IXGRP)) {
		bprm->per_clear |= PER_CLEAR_ON_SETID;
<<<<<<< HEAD
		bprm->cred->egid = gid;
	}
}

/* 
 * Fill the binprm structure from the inode. 
 * Check permissions, then read the first 128 (BINPRM_BUF_SIZE) bytes
 *
 * This may be called multiple times for binary chains (scripts for example).
 */
int prepare_binprm(struct linux_binprm *bprm)
{
	int retval;

	if (bprm->file->f_op == NULL)
		return -EACCES;

	bprm_fill_uid(bprm);

	/* fill in binprm security blob */
	retval = security_bprm_set_creds(bprm);
	if (retval)
		return retval;
	bprm->cred_prepared = 1;

	memset(bprm->buf, 0, BINPRM_BUF_SIZE);
	return kernel_read(bprm->file, 0, bprm->buf, BINPRM_BUF_SIZE);
}

EXPORT_SYMBOL(prepare_binprm);

=======
		bprm->cred->egid = vfsgid_into_kgid(vfsgid);
	}
}

/*
 * Compute brpm->cred based upon the final binary.
 */
static int bprm_creds_from_file(struct linux_binprm *bprm)
{
	/* Compute creds based on which file? */
	struct file *file = bprm->execfd_creds ? bprm->executable : bprm->file;

	bprm_fill_uid(bprm, file);
	return security_bprm_creds_from_file(bprm, file);
}

/*
 * Fill the binprm structure from the inode.
 * Read the first BINPRM_BUF_SIZE bytes
 *
 * This may be called multiple times for binary chains (scripts for example).
 */
static int prepare_binprm(struct linux_binprm *bprm)
{
	loff_t pos = 0;

	memset(bprm->buf, 0, BINPRM_BUF_SIZE);
	return kernel_read(bprm->file, bprm->buf, BINPRM_BUF_SIZE, &pos);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Arguments are '\0' separated strings found at the location bprm->p
 * points to; chop off the first by relocating brpm->p to right after
 * the first '\0' encountered.
 */
int remove_arg_zero(struct linux_binprm *bprm)
{
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long offset;
	char *kaddr;
	struct page *page;

	if (!bprm->argc)
		return 0;

	do {
		offset = bprm->p & ~PAGE_MASK;
		page = get_arg_page(bprm, bprm->p, 0);
<<<<<<< HEAD
		if (!page) {
			ret = -EFAULT;
			goto out;
		}
		kaddr = kmap_atomic(page);
=======
		if (!page)
			return -EFAULT;
		kaddr = kmap_local_page(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		for (; offset < PAGE_SIZE && kaddr[offset];
				offset++, bprm->p++)
			;

<<<<<<< HEAD
		kunmap_atomic(kaddr);
		put_arg_page(page);

		if (offset == PAGE_SIZE)
			free_arg_page(bprm, (bprm->p >> PAGE_SHIFT) - 1);
=======
		kunmap_local(kaddr);
		put_arg_page(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (offset == PAGE_SIZE);

	bprm->p++;
	bprm->argc--;
<<<<<<< HEAD
	ret = 0;

out:
	return ret;
}
EXPORT_SYMBOL(remove_arg_zero);

/*
 * cycle the list of binary formats handler, until one recognizes the image
 */
int search_binary_handler(struct linux_binprm *bprm,struct pt_regs *regs)
{
	unsigned int depth = bprm->recursion_depth;
	int try,retval;
	struct linux_binfmt *fmt;
	pid_t old_pid, old_vpid;

	/* This allows 4 levels of binfmt rewrites before failing hard. */
	if (depth > 5)
		return -ELOOP;
=======

	return 0;
}
EXPORT_SYMBOL(remove_arg_zero);

#define printable(c) (((c)=='\t') || ((c)=='\n') || (0x20<=(c) && (c)<=0x7e))
/*
 * cycle the list of binary formats handler, until one recognizes the image
 */
static int search_binary_handler(struct linux_binprm *bprm)
{
	bool need_retry = IS_ENABLED(CONFIG_MODULES);
	struct linux_binfmt *fmt;
	int retval;

	retval = prepare_binprm(bprm);
	if (retval < 0)
		return retval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	retval = security_bprm_check(bprm);
	if (retval)
		return retval;

<<<<<<< HEAD
	retval = audit_bprm(bprm);
	if (retval)
		return retval;
=======
	retval = -ENOENT;
 retry:
	read_lock(&binfmt_lock);
	list_for_each_entry(fmt, &formats, lh) {
		if (!try_module_get(fmt->module))
			continue;
		read_unlock(&binfmt_lock);

		retval = fmt->load_binary(bprm);

		read_lock(&binfmt_lock);
		put_binfmt(fmt);
		if (bprm->point_of_no_return || (retval != -ENOEXEC)) {
			read_unlock(&binfmt_lock);
			return retval;
		}
	}
	read_unlock(&binfmt_lock);

	if (need_retry) {
		if (printable(bprm->buf[0]) && printable(bprm->buf[1]) &&
		    printable(bprm->buf[2]) && printable(bprm->buf[3]))
			return retval;
		if (request_module("binfmt-%04x", *(ushort *)(bprm->buf + 2)) < 0)
			return retval;
		need_retry = false;
		goto retry;
	}

	return retval;
}

/* binfmt handlers will call back into begin_new_exec() on success. */
static int exec_binprm(struct linux_binprm *bprm)
{
	pid_t old_pid, old_vpid;
	int ret, depth;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Need to fetch pid before load_binary changes it */
	old_pid = current->pid;
	rcu_read_lock();
	old_vpid = task_pid_nr_ns(current, task_active_pid_ns(current->parent));
	rcu_read_unlock();

<<<<<<< HEAD
	retval = -ENOENT;
	for (try=0; try<2; try++) {
		read_lock(&binfmt_lock);
		list_for_each_entry(fmt, &formats, lh) {
			int (*fn)(struct linux_binprm *, struct pt_regs *) = fmt->load_binary;
			if (!fn)
				continue;
			if (!try_module_get(fmt->module))
				continue;
			read_unlock(&binfmt_lock);
			bprm->recursion_depth = depth + 1;
			retval = fn(bprm, regs);
			bprm->recursion_depth = depth;
			if (retval >= 0) {
				if (depth == 0) {
					trace_sched_process_exec(current, old_pid, bprm);
					ptrace_event(PTRACE_EVENT_EXEC, old_vpid);
				}
				put_binfmt(fmt);
				allow_write_access(bprm->file);
				if (bprm->file)
					fput(bprm->file);
				bprm->file = NULL;
				current->did_exec = 1;
				proc_exec_connector(current);
				return retval;
			}
			read_lock(&binfmt_lock);
			put_binfmt(fmt);
			if (retval != -ENOEXEC || bprm->mm == NULL)
				break;
			if (!bprm->file) {
				read_unlock(&binfmt_lock);
				return retval;
			}
		}
		read_unlock(&binfmt_lock);
#ifdef CONFIG_MODULES
		if (retval != -ENOEXEC || bprm->mm == NULL) {
			break;
		} else {
#define printable(c) (((c)=='\t') || ((c)=='\n') || (0x20<=(c) && (c)<=0x7e))
			if (printable(bprm->buf[0]) &&
			    printable(bprm->buf[1]) &&
			    printable(bprm->buf[2]) &&
			    printable(bprm->buf[3]))
				break; /* -ENOEXEC */
			if (try)
				break; /* -ENOEXEC */
			request_module("binfmt-%04x", *(unsigned short *)(&bprm->buf[2]));
		}
#else
		break;
#endif
	}
	return retval;
}

EXPORT_SYMBOL(search_binary_handler);

/*
 * sys_execve() executes a new program.
 */
static int do_execve_common(const char *filename,
				struct user_arg_ptr argv,
				struct user_arg_ptr envp,
				struct pt_regs *regs)
{
	struct linux_binprm *bprm;
	struct file *file;
	struct files_struct *displaced;
	bool clear_in_exec;
	int retval;
	const struct cred *cred = current_cred();
	bool is_su;
=======
	/* This allows 4 levels of binfmt rewrites before failing hard. */
	for (depth = 0;; depth++) {
		struct file *exec;
		if (depth > 5)
			return -ELOOP;

		ret = search_binary_handler(bprm);
		if (ret < 0)
			return ret;
		if (!bprm->interpreter)
			break;

		exec = bprm->file;
		bprm->file = bprm->interpreter;
		bprm->interpreter = NULL;

		allow_write_access(exec);
		if (unlikely(bprm->have_execfd)) {
			if (bprm->executable) {
				fput(exec);
				return -ENOEXEC;
			}
			bprm->executable = exec;
		} else
			fput(exec);
	}

	audit_bprm(bprm);
	trace_sched_process_exec(current, old_pid, bprm);
	ptrace_event(PTRACE_EVENT_EXEC, old_vpid);
	proc_exec_connector(current);
	return 0;
}

static int bprm_execve(struct linux_binprm *bprm)
{
	int retval;

	retval = prepare_bprm_creds(bprm);
	if (retval)
		return retval;

	/*
	 * Check for unsafe execution states before exec_binprm(), which
	 * will call back into begin_new_exec(), into bprm_creds_from_file(),
	 * where setuid-ness is evaluated.
	 */
	check_unsafe_exec(bprm);
	current->in_execve = 1;
	sched_mm_cid_before_execve(current);

	sched_exec();

	/* Set the unchanging part of bprm->cred */
	retval = security_bprm_creds_for_exec(bprm);
	if (retval)
		goto out;

	retval = exec_binprm(bprm);
	if (retval < 0)
		goto out;

	sched_mm_cid_after_execve(current);
	/* execve succeeded */
	current->fs->in_exec = 0;
	current->in_execve = 0;
	rseq_execve(current);
	user_events_execve(current);
	acct_update_integrals(current);
	task_numa_free(current, false);
	return retval;

out:
	/*
	 * If past the point of no return ensure the code never
	 * returns to the userspace process.  Use an existing fatal
	 * signal if present otherwise terminate the process with
	 * SIGSEGV.
	 */
	if (bprm->point_of_no_return && !fatal_signal_pending(current))
		force_fatal_sig(SIGSEGV);

	sched_mm_cid_after_execve(current);
	current->fs->in_exec = 0;
	current->in_execve = 0;

	return retval;
}

static int do_execveat_common(int fd, struct filename *filename,
			      struct user_arg_ptr argv,
			      struct user_arg_ptr envp,
			      int flags)
{
	struct linux_binprm *bprm;
	int retval;

	if (IS_ERR(filename))
		return PTR_ERR(filename);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * We move the actual failure in case of RLIMIT_NPROC excess from
	 * set*uid() to execve() because too many poorly written programs
	 * don't check setuid() return code.  Here we additionally recheck
	 * whether NPROC limit is still exceeded.
	 */
	if ((current->flags & PF_NPROC_EXCEEDED) &&
<<<<<<< HEAD
	    atomic_read(&cred->user->processes) > rlimit(RLIMIT_NPROC)) {
=======
	    is_rlimit_overlimit(current_ucounts(), UCOUNT_RLIMIT_NPROC, rlimit(RLIMIT_NPROC))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		retval = -EAGAIN;
		goto out_ret;
	}

	/* We're below the limit (still or again), so we don't want to make
	 * further execve() calls fail. */
	current->flags &= ~PF_NPROC_EXCEEDED;

<<<<<<< HEAD
	retval = unshare_files(&displaced);
	if (retval)
		goto out_ret;

	retval = -ENOMEM;
	bprm = kzalloc(sizeof(*bprm), GFP_KERNEL);
	if (!bprm)
		goto out_files;

	retval = prepare_bprm_creds(bprm);
	if (retval)
		goto out_free;

	retval = check_unsafe_exec(bprm);
	if (retval < 0)
		goto out_free;
	clear_in_exec = retval;
	current->in_execve = 1;

	file = open_exec(filename);
	retval = PTR_ERR(file);
	if (IS_ERR(file))
		goto out_unmark;

	sched_exec();

	bprm->file = file;
	bprm->filename = filename;
	bprm->interp = filename;

	retval = bprm_mm_init(bprm);
	if (retval)
		goto out_file;

	bprm->argc = count(argv, MAX_ARG_STRINGS);
	if ((retval = bprm->argc) < 0)
		goto out;

	bprm->envc = count(envp, MAX_ARG_STRINGS);
	if ((retval = bprm->envc) < 0)
		goto out;

	retval = prepare_binprm(bprm);
	if (retval < 0)
		goto out;

	retval = copy_strings_kernel(1, &bprm->filename, bprm);
	if (retval < 0)
		goto out;

	bprm->exec = bprm->p;
	retval = copy_strings(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out;

	retval = copy_strings(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out;

	/* search_binary_handler can release file and it may be freed */
	is_su = d_is_su(file->f_dentry);

	retval = search_binary_handler(bprm,regs);
	if (retval < 0)
		goto out;

	if (is_su && capable(CAP_SYS_ADMIN)) {
		current->flags |= PF_SU;
		su_exec();
	}

	/* execve succeeded */
	current->fs->in_exec = 0;
	current->in_execve = 0;
	acct_update_integrals(current);
	free_bprm(bprm);
	if (displaced)
		put_files_struct(displaced);
	return retval;

out:
	if (bprm->mm) {
		acct_arg_size(bprm, 0);
		mmput(bprm->mm);
	}

out_file:
	if (bprm->file) {
		allow_write_access(bprm->file);
		fput(bprm->file);
	}

out_unmark:
	if (clear_in_exec)
		current->fs->in_exec = 0;
	current->in_execve = 0;

out_free:
	free_bprm(bprm);

out_files:
	if (displaced)
		reset_files_struct(displaced);
out_ret:
	return retval;
}

int do_execve(const char *filename,
	const char __user *const __user *__argv,
	const char __user *const __user *__envp,
	struct pt_regs *regs)
{
	struct user_arg_ptr argv = { .ptr.native = __argv };
	struct user_arg_ptr envp = { .ptr.native = __envp };
	return do_execve_common(filename, argv, envp, regs);
}

#ifdef CONFIG_COMPAT
int compat_do_execve(char *filename,
	compat_uptr_t __user *__argv,
	compat_uptr_t __user *__envp,
	struct pt_regs *regs)
=======
	bprm = alloc_bprm(fd, filename, flags);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}

	retval = count(argv, MAX_ARG_STRINGS);
	if (retval == 0)
		pr_warn_once("process '%s' launched '%s' with NULL argv: empty string added\n",
			     current->comm, bprm->filename);
	if (retval < 0)
		goto out_free;
	bprm->argc = retval;

	retval = count(envp, MAX_ARG_STRINGS);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;

	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;

	retval = copy_strings(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_strings(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;

	/*
	 * When argv is empty, add an empty string ("") as argv[0] to
	 * ensure confused userspace programs that start processing
	 * from argv[1] won't end up walking envp. See also
	 * bprm_stack_limits().
	 */
	if (bprm->argc == 0) {
		retval = copy_string_kernel("", bprm);
		if (retval < 0)
			goto out_free;
		bprm->argc = 1;
	}

	retval = bprm_execve(bprm);
out_free:
	free_bprm(bprm);

out_ret:
	putname(filename);
	return retval;
}

int kernel_execve(const char *kernel_filename,
		  const char *const *argv, const char *const *envp)
{
	struct filename *filename;
	struct linux_binprm *bprm;
	int fd = AT_FDCWD;
	int retval;

	/* It is non-sense for kernel threads to call execve */
	if (WARN_ON_ONCE(current->flags & PF_KTHREAD))
		return -EINVAL;

	filename = getname_kernel(kernel_filename);
	if (IS_ERR(filename))
		return PTR_ERR(filename);

	bprm = alloc_bprm(fd, filename, 0);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}

	retval = count_strings_kernel(argv);
	if (WARN_ON_ONCE(retval == 0))
		retval = -EINVAL;
	if (retval < 0)
		goto out_free;
	bprm->argc = retval;

	retval = count_strings_kernel(envp);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;

	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;

	retval = copy_strings_kernel(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_strings_kernel(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;

	retval = bprm_execve(bprm);
out_free:
	free_bprm(bprm);
out_ret:
	putname(filename);
	return retval;
}

static int do_execve(struct filename *filename,
	const char __user *const __user *__argv,
	const char __user *const __user *__envp)
{
	struct user_arg_ptr argv = { .ptr.native = __argv };
	struct user_arg_ptr envp = { .ptr.native = __envp };
	return do_execveat_common(AT_FDCWD, filename, argv, envp, 0);
}

static int do_execveat(int fd, struct filename *filename,
		const char __user *const __user *__argv,
		const char __user *const __user *__envp,
		int flags)
{
	struct user_arg_ptr argv = { .ptr.native = __argv };
	struct user_arg_ptr envp = { .ptr.native = __envp };

	return do_execveat_common(fd, filename, argv, envp, flags);
}

#ifdef CONFIG_COMPAT
static int compat_do_execve(struct filename *filename,
	const compat_uptr_t __user *__argv,
	const compat_uptr_t __user *__envp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct user_arg_ptr argv = {
		.is_compat = true,
		.ptr.compat = __argv,
	};
	struct user_arg_ptr envp = {
		.is_compat = true,
		.ptr.compat = __envp,
	};
<<<<<<< HEAD
	return do_execve_common(filename, argv, envp, regs);
=======
	return do_execveat_common(AT_FDCWD, filename, argv, envp, 0);
}

static int compat_do_execveat(int fd, struct filename *filename,
			      const compat_uptr_t __user *__argv,
			      const compat_uptr_t __user *__envp,
			      int flags)
{
	struct user_arg_ptr argv = {
		.is_compat = true,
		.ptr.compat = __argv,
	};
	struct user_arg_ptr envp = {
		.is_compat = true,
		.ptr.compat = __envp,
	};
	return do_execveat_common(fd, filename, argv, envp, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

void set_binfmt(struct linux_binfmt *new)
{
	struct mm_struct *mm = current->mm;

	if (mm->binfmt)
		module_put(mm->binfmt->module);

	mm->binfmt = new;
	if (new)
		__module_get(new->module);
}
<<<<<<< HEAD

EXPORT_SYMBOL(set_binfmt);

static int expand_corename(struct core_name *cn)
{
	char *old_corename = cn->corename;

	cn->size = CORENAME_MAX_SIZE * atomic_inc_return(&call_count);
	cn->corename = krealloc(old_corename, cn->size, GFP_KERNEL);

	if (!cn->corename) {
		kfree(old_corename);
		return -ENOMEM;
	}

	return 0;
}

static int cn_printf(struct core_name *cn, const char *fmt, ...)
{
	char *cur;
	int need;
	int ret;
	va_list arg;

	va_start(arg, fmt);
	need = vsnprintf(NULL, 0, fmt, arg);
	va_end(arg);

	if (likely(need < cn->size - cn->used - 1))
		goto out_printf;

	ret = expand_corename(cn);
	if (ret)
		goto expand_fail;

out_printf:
	cur = cn->corename + cn->used;
	va_start(arg, fmt);
	vsnprintf(cur, need + 1, fmt, arg);
	va_end(arg);
	cn->used += need;
	return 0;

expand_fail:
	return ret;
}

static void cn_escape(char *str)
{
	for (; *str; str++)
		if (*str == '/')
			*str = '!';
}

static int cn_print_exe_file(struct core_name *cn)
{
	struct file *exe_file;
	char *pathbuf, *path;
	int ret;

	exe_file = get_mm_exe_file(current->mm);
	if (!exe_file) {
		char *commstart = cn->corename + cn->used;
		ret = cn_printf(cn, "%s (path unknown)", current->comm);
		cn_escape(commstart);
		return ret;
	}

	pathbuf = kmalloc(PATH_MAX, GFP_TEMPORARY);
	if (!pathbuf) {
		ret = -ENOMEM;
		goto put_exe_file;
	}

	path = d_path(&exe_file->f_path, pathbuf, PATH_MAX);
	if (IS_ERR(path)) {
		ret = PTR_ERR(path);
		goto free_buf;
	}

	cn_escape(path);

	ret = cn_printf(cn, "%s", path);

free_buf:
	kfree(pathbuf);
put_exe_file:
	fput(exe_file);
	return ret;
}

/* format_corename will inspect the pattern parameter, and output a
 * name into corename, which must have space for at least
 * CORENAME_MAX_SIZE bytes plus one byte for the zero terminator.
 */
static int format_corename(struct core_name *cn, long signr)
{
	const struct cred *cred = current_cred();
	const char *pat_ptr = core_pattern;
	int ispipe = (*pat_ptr == '|');
	int pid_in_pattern = 0;
	int err = 0;

	cn->size = CORENAME_MAX_SIZE * atomic_read(&call_count);
	cn->corename = kmalloc(cn->size, GFP_KERNEL);
	cn->used = 0;

	if (!cn->corename)
		return -ENOMEM;

	/* Repeat as long as we have more pattern to process and more output
	   space */
	while (*pat_ptr) {
		if (*pat_ptr != '%') {
			if (*pat_ptr == 0)
				goto out;
			err = cn_printf(cn, "%c", *pat_ptr++);
		} else {
			switch (*++pat_ptr) {
			/* single % at the end, drop that */
			case 0:
				goto out;
			/* Double percent, output one percent */
			case '%':
				err = cn_printf(cn, "%c", '%');
				break;
			/* pid */
			case 'p':
				pid_in_pattern = 1;
				err = cn_printf(cn, "%d",
					      task_tgid_vnr(current));
				break;
			/* uid */
			case 'u':
				err = cn_printf(cn, "%d", cred->uid);
				break;
			/* gid */
			case 'g':
				err = cn_printf(cn, "%d", cred->gid);
				break;
			/* signal that caused the coredump */
			case 's':
				err = cn_printf(cn, "%ld", signr);
				break;
			/* UNIX time of coredump */
			case 't': {
				struct timeval tv;
				do_gettimeofday(&tv);
				err = cn_printf(cn, "%lu", tv.tv_sec);
				break;
			}
			/* hostname */
			case 'h': {
				char *namestart = cn->corename + cn->used;
				down_read(&uts_sem);
				err = cn_printf(cn, "%s",
					      utsname()->nodename);
				up_read(&uts_sem);
				cn_escape(namestart);
				break;
			}
			/* executable */
			case 'e': {
				char *commstart = cn->corename + cn->used;
				err = cn_printf(cn, "%s", current->comm);
				cn_escape(commstart);
				break;
			}
			case 'E':
				err = cn_print_exe_file(cn);
				break;
			/* core limit size */
			case 'c':
				err = cn_printf(cn, "%lu",
					      rlimit(RLIMIT_CORE));
				break;
			default:
				break;
			}
			++pat_ptr;
		}

		if (err)
			return err;
	}

	/* Backward compatibility with core_uses_pid:
	 *
	 * If core_pattern does not include a %p (as is the default)
	 * and core_uses_pid is set, then .%pid will be appended to
	 * the filename. Do not do this for piped commands. */
	if (!ispipe && !pid_in_pattern && core_uses_pid) {
		err = cn_printf(cn, ".%d", task_tgid_vnr(current));
		if (err)
			return err;
	}
out:
	return ispipe;
}

static int zap_process(struct task_struct *start, int exit_code)
{
	struct task_struct *t;
	int nr = 0;

	start->signal->flags = SIGNAL_GROUP_EXIT;
	start->signal->group_exit_code = exit_code;
	start->signal->group_stop_count = 0;

	t = start;
	do {
		task_clear_jobctl_pending(t, JOBCTL_PENDING_MASK);
		if (t != current && t->mm) {
			sigaddset(&t->pending.signal, SIGKILL);
			signal_wake_up(t, 1);
			nr++;
		}
	} while_each_thread(start, t);

	return nr;
}

static inline int zap_threads(struct task_struct *tsk, struct mm_struct *mm,
				struct core_state *core_state, int exit_code)
{
	struct task_struct *g, *p;
	unsigned long flags;
	int nr = -EAGAIN;

	spin_lock_irq(&tsk->sighand->siglock);
	if (!signal_group_exit(tsk->signal)) {
		mm->core_state = core_state;
		nr = zap_process(tsk, exit_code);
	}
	spin_unlock_irq(&tsk->sighand->siglock);
	if (unlikely(nr < 0))
		return nr;

	if (atomic_read(&mm->mm_users) == nr + 1)
		goto done;
	/*
	 * We should find and kill all tasks which use this mm, and we should
	 * count them correctly into ->nr_threads. We don't take tasklist
	 * lock, but this is safe wrt:
	 *
	 * fork:
	 *	None of sub-threads can fork after zap_process(leader). All
	 *	processes which were created before this point should be
	 *	visible to zap_threads() because copy_process() adds the new
	 *	process to the tail of init_task.tasks list, and lock/unlock
	 *	of ->siglock provides a memory barrier.
	 *
	 * do_exit:
	 *	The caller holds mm->mmap_sem. This means that the task which
	 *	uses this mm can't pass exit_mm(), so it can't exit or clear
	 *	its ->mm.
	 *
	 * de_thread:
	 *	It does list_replace_rcu(&leader->tasks, &current->tasks),
	 *	we must see either old or new leader, this does not matter.
	 *	However, it can change p->sighand, so lock_task_sighand(p)
	 *	must be used. Since p->mm != NULL and we hold ->mmap_sem
	 *	it can't fail.
	 *
	 *	Note also that "g" can be the old leader with ->mm == NULL
	 *	and already unhashed and thus removed from ->thread_group.
	 *	This is OK, __unhash_process()->list_del_rcu() does not
	 *	clear the ->next pointer, we will find the new leader via
	 *	next_thread().
	 */
	rcu_read_lock();
	for_each_process(g) {
		if (g == tsk->group_leader)
			continue;
		if (g->flags & PF_KTHREAD)
			continue;
		p = g;
		do {
			if (p->mm) {
				if (unlikely(p->mm == mm)) {
					lock_task_sighand(p, &flags);
					nr += zap_process(p, exit_code);
					unlock_task_sighand(p, &flags);
				}
				break;
			}
		} while_each_thread(g, p);
	}
	rcu_read_unlock();
done:
	atomic_set(&core_state->nr_threads, nr);
	return nr;
}

static int coredump_wait(int exit_code, struct core_state *core_state)
{
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->mm;
	int core_waiters = -EBUSY;

	init_completion(&core_state->startup);
	core_state->dumper.task = tsk;
	core_state->dumper.next = NULL;

	down_write(&mm->mmap_sem);
	if (!mm->core_state)
		core_waiters = zap_threads(tsk, mm, core_state, exit_code);
	up_write(&mm->mmap_sem);

	if (core_waiters > 0)
		wait_for_completion(&core_state->startup);

	return core_waiters;
}

static void coredump_finish(struct mm_struct *mm)
{
	struct core_thread *curr, *next;
	struct task_struct *task;

	next = mm->core_state->dumper.next;
	while ((curr = next) != NULL) {
		next = curr->next;
		task = curr->task;
		/*
		 * see exit_mm(), curr->task must not see
		 * ->task == NULL before we read ->next.
		 */
		smp_mb();
		curr->task = NULL;
		wake_up_process(task);
	}

	mm->core_state = NULL;
}

/*
 * set_dumpable converts traditional three-value dumpable to two flags and
 * stores them into mm->flags.  It modifies lower two bits of mm->flags, but
 * these bits are not changed atomically.  So get_dumpable can observe the
 * intermediate state.  To avoid doing unexpected behavior, get get_dumpable
 * return either old dumpable or new one by paying attention to the order of
 * modifying the bits.
 *
 * dumpable |   mm->flags (binary)
 * old  new | initial interim  final
 * ---------+-----------------------
 *  0    1  |   00      01      01
 *  0    2  |   00      10(*)   11
 *  1    0  |   01      00      00
 *  1    2  |   01      11      11
 *  2    0  |   11      10(*)   00
 *  2    1  |   11      11      01
 *
 * (*) get_dumpable regards interim value of 10 as 11.
 */
void set_dumpable(struct mm_struct *mm, int value)
{
	switch (value) {
	case 0:
		clear_bit(MMF_DUMPABLE, &mm->flags);
		smp_wmb();
		clear_bit(MMF_DUMP_SECURELY, &mm->flags);
		break;
	case 1:
		set_bit(MMF_DUMPABLE, &mm->flags);
		smp_wmb();
		clear_bit(MMF_DUMP_SECURELY, &mm->flags);
		break;
	case 2:
		set_bit(MMF_DUMP_SECURELY, &mm->flags);
		smp_wmb();
		set_bit(MMF_DUMPABLE, &mm->flags);
		break;
	}
}

static int __get_dumpable(unsigned long mm_flags)
{
	int ret;

	ret = mm_flags & MMF_DUMPABLE_MASK;
	return (ret >= 2) ? 2 : ret;
}

/*
 * This returns the actual value of the suid_dumpable flag. For things
 * that are using this for checking for privilege transitions, it must
 * test against SUID_DUMP_USER rather than treating it as a boolean
 * value.
 */
int get_dumpable(struct mm_struct *mm)
{
	return __get_dumpable(mm->flags);
}

static void wait_for_dump_helpers(struct file *file)
{
	struct pipe_inode_info *pipe;

	pipe = file->f_path.dentry->d_inode->i_pipe;

	pipe_lock(pipe);
	pipe->readers++;
	pipe->writers--;

	while ((pipe->readers > 1) && (!signal_pending(current))) {
		wake_up_interruptible_sync(&pipe->wait);
		kill_fasync(&pipe->fasync_readers, SIGIO, POLL_IN);
		pipe_wait(pipe);
	}

	pipe->readers--;
	pipe->writers++;
	pipe_unlock(pipe);

}


/*
 * umh_pipe_setup
 * helper function to customize the process used
 * to collect the core in userspace.  Specifically
 * it sets up a pipe and installs it as fd 0 (stdin)
 * for the process.  Returns 0 on success, or
 * PTR_ERR on failure.
 * Note that it also sets the core limit to 1.  This
 * is a special value that we use to trap recursive
 * core dumps
 */
static int umh_pipe_setup(struct subprocess_info *info, struct cred *new)
{
	struct file *rp, *wp;
	struct fdtable *fdt;
	struct coredump_params *cp = (struct coredump_params *)info->data;
	struct files_struct *cf = current->files;

	wp = create_write_pipe(0);
	if (IS_ERR(wp))
		return PTR_ERR(wp);

	rp = create_read_pipe(wp, 0);
	if (IS_ERR(rp)) {
		free_write_pipe(wp);
		return PTR_ERR(rp);
	}

	cp->file = wp;

	sys_close(0);
	fd_install(0, rp);
	spin_lock(&cf->file_lock);
	fdt = files_fdtable(cf);
	__set_open_fd(0, fdt);
	__clear_close_on_exec(0, fdt);
	spin_unlock(&cf->file_lock);

	/* and disallow core files too */
	current->signal->rlim[RLIMIT_CORE] = (struct rlimit){1, 1};

	return 0;
}

void do_coredump(long signr, int exit_code, struct pt_regs *regs)
{
	struct core_state core_state;
	struct core_name cn;
	struct mm_struct *mm = current->mm;
	struct linux_binfmt * binfmt;
	const struct cred *old_cred;
	struct cred *cred;
	int retval = 0;
	int flag = 0;
	int ispipe;
	static atomic_t core_dump_count = ATOMIC_INIT(0);
	struct coredump_params cprm = {
		.signr = signr,
		.regs = regs,
		.limit = rlimit(RLIMIT_CORE),
		/*
		 * We must use the same mm->flags while dumping core to avoid
		 * inconsistency of bit flags, since this flag is not protected
		 * by any locks.
		 */
		.mm_flags = mm->flags,
	};

	audit_core_dumps(signr);

	binfmt = mm->binfmt;
	if (!binfmt || !binfmt->core_dump)
		goto fail;
	if (!__get_dumpable(cprm.mm_flags))
		goto fail;

	cred = prepare_creds();
	if (!cred)
		goto fail;
	/*
	 *	We cannot trust fsuid as being the "true" uid of the
	 *	process nor do we know its entire history. We only know it
	 *	was tainted so we dump it as root in mode 2.
	 */
	if (__get_dumpable(cprm.mm_flags) == 2) {
		/* Setuid core dump mode */
		flag = O_EXCL;		/* Stop rewrite attacks */
		cred->fsuid = 0;	/* Dump root private */
	}

	retval = coredump_wait(exit_code, &core_state);
	if (retval < 0)
		goto fail_creds;

	old_cred = override_creds(cred);

	/*
	 * Clear any false indication of pending signals that might
	 * be seen by the filesystem code called to write the core file.
	 */
	clear_thread_flag(TIF_SIGPENDING);

	ispipe = format_corename(&cn, signr);

 	if (ispipe) {
		int dump_count;
		char **helper_argv;

		if (ispipe < 0) {
			printk(KERN_WARNING "format_corename failed\n");
			printk(KERN_WARNING "Aborting core\n");
			goto fail_corename;
		}

		if (cprm.limit == 1) {
			/*
			 * Normally core limits are irrelevant to pipes, since
			 * we're not writing to the file system, but we use
			 * cprm.limit of 1 here as a speacial value. Any
			 * non-1 limit gets set to RLIM_INFINITY below, but
			 * a limit of 0 skips the dump.  This is a consistent
			 * way to catch recursive crashes.  We can still crash
			 * if the core_pattern binary sets RLIM_CORE =  !1
			 * but it runs as root, and can do lots of stupid things
			 * Note that we use task_tgid_vnr here to grab the pid
			 * of the process group leader.  That way we get the
			 * right pid if a thread in a multi-threaded
			 * core_pattern process dies.
			 */
			printk(KERN_WARNING
				"Process %d(%s) has RLIMIT_CORE set to 1\n",
				task_tgid_vnr(current), current->comm);
			printk(KERN_WARNING "Aborting core\n");
			goto fail_unlock;
		}
		cprm.limit = RLIM_INFINITY;

		dump_count = atomic_inc_return(&core_dump_count);
		if (core_pipe_limit && (core_pipe_limit < dump_count)) {
			printk(KERN_WARNING "Pid %d(%s) over core_pipe_limit\n",
			       task_tgid_vnr(current), current->comm);
			printk(KERN_WARNING "Skipping core dump\n");
			goto fail_dropcount;
		}

		helper_argv = argv_split(GFP_KERNEL, cn.corename+1, NULL);
		if (!helper_argv) {
			printk(KERN_WARNING "%s failed to allocate memory\n",
			       __func__);
			goto fail_dropcount;
		}

		retval = call_usermodehelper_fns(helper_argv[0], helper_argv,
					NULL, UMH_WAIT_EXEC, umh_pipe_setup,
					NULL, &cprm);
		argv_free(helper_argv);
		if (retval) {
 			printk(KERN_INFO "Core dump to %s pipe failed\n",
			       cn.corename);
			goto close_fail;
 		}
	} else {
		struct inode *inode;

		if (cprm.limit < binfmt->min_coredump)
			goto fail_unlock;

		cprm.file = filp_open(cn.corename,
				 O_CREAT | 2 | O_NOFOLLOW | O_LARGEFILE | flag,
				 0600);
		if (IS_ERR(cprm.file))
			goto fail_unlock;

		inode = cprm.file->f_path.dentry->d_inode;
		if (inode->i_nlink > 1)
			goto close_fail;
		if (d_unhashed(cprm.file->f_path.dentry))
			goto close_fail;
		/*
		 * AK: actually i see no reason to not allow this for named
		 * pipes etc, but keep the previous behaviour for now.
		 */
		if (!S_ISREG(inode->i_mode))
			goto close_fail;
		/*
		 * Dont allow local users get cute and trick others to coredump
		 * into their pre-created files.
		 */
		if (inode->i_uid != current_fsuid())
			goto close_fail;
		if (!cprm.file->f_op || !cprm.file->f_op->write)
			goto close_fail;
		if (do_truncate(cprm.file->f_path.dentry, 0, 0, cprm.file))
			goto close_fail;
	}

	retval = binfmt->core_dump(&cprm);
	if (retval)
		current->signal->group_exit_code |= 0x80;

	if (ispipe && core_pipe_limit)
		wait_for_dump_helpers(cprm.file);
close_fail:
	if (cprm.file)
		filp_close(cprm.file, NULL);
fail_dropcount:
	if (ispipe)
		atomic_dec(&core_dump_count);
fail_unlock:
	kfree(cn.corename);
fail_corename:
	coredump_finish(mm);
	revert_creds(old_cred);
fail_creds:
	put_cred(cred);
fail:
	return;
}

/*
 * Core dumping helper functions.  These are the only things you should
 * do on a core-file: use only these functions to write out all the
 * necessary info.
 */
int dump_write(struct file *file, const void *addr, int nr)
{
	return access_ok(VERIFY_READ, addr, nr) && file->f_op->write(file, addr, nr, &file->f_pos) == nr;
}
EXPORT_SYMBOL(dump_write);

int dump_seek(struct file *file, loff_t off)
{
	int ret = 1;

	if (file->f_op->llseek && file->f_op->llseek != no_llseek) {
		if (file->f_op->llseek(file, off, SEEK_CUR) < 0)
			return 0;
	} else {
		char *buf = (char *)get_zeroed_page(GFP_KERNEL);

		if (!buf)
			return 0;
		while (off > 0) {
			unsigned long n = off;

			if (n > PAGE_SIZE)
				n = PAGE_SIZE;
			if (!dump_write(file, buf, n)) {
				ret = 0;
				break;
			}
			off -= n;
		}
		free_page((unsigned long)buf);
	}
	return ret;
}
EXPORT_SYMBOL(dump_seek);
=======
EXPORT_SYMBOL(set_binfmt);

/*
 * set_dumpable stores three-value SUID_DUMP_* into mm->flags.
 */
void set_dumpable(struct mm_struct *mm, int value)
{
	if (WARN_ON((unsigned)value > SUID_DUMP_ROOT))
		return;

	set_mask_bits(&mm->flags, MMF_DUMPABLE_MASK, value);
}

SYSCALL_DEFINE3(execve,
		const char __user *, filename,
		const char __user *const __user *, argv,
		const char __user *const __user *, envp)
{
	return do_execve(getname(filename), argv, envp);
}

SYSCALL_DEFINE5(execveat,
		int, fd, const char __user *, filename,
		const char __user *const __user *, argv,
		const char __user *const __user *, envp,
		int, flags)
{
	return do_execveat(fd,
			   getname_uflags(filename, flags),
			   argv, envp, flags);
}

#ifdef CONFIG_COMPAT
COMPAT_SYSCALL_DEFINE3(execve, const char __user *, filename,
	const compat_uptr_t __user *, argv,
	const compat_uptr_t __user *, envp)
{
	return compat_do_execve(getname(filename), argv, envp);
}

COMPAT_SYSCALL_DEFINE5(execveat, int, fd,
		       const char __user *, filename,
		       const compat_uptr_t __user *, argv,
		       const compat_uptr_t __user *, envp,
		       int,  flags)
{
	return compat_do_execveat(fd,
				  getname_uflags(filename, flags),
				  argv, envp, flags);
}
#endif

#ifdef CONFIG_SYSCTL

static int proc_dointvec_minmax_coredump(struct ctl_table *table, int write,
		void *buffer, size_t *lenp, loff_t *ppos)
{
	int error = proc_dointvec_minmax(table, write, buffer, lenp, ppos);

	if (!error)
		validate_coredump_safety();
	return error;
}

static struct ctl_table fs_exec_sysctls[] = {
	{
		.procname	= "suid_dumpable",
		.data		= &suid_dumpable,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax_coredump,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_TWO,
	},
};

static int __init init_fs_exec_sysctls(void)
{
	register_sysctl_init("fs", fs_exec_sysctls);
	return 0;
}

fs_initcall(init_fs_exec_sysctls);
#endif /* CONFIG_SYSCTL */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
