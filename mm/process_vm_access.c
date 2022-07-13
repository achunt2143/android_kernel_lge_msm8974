<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/mm/process_vm_access.c
 *
 * Copyright (C) 2010-2011 Christopher Yeoh <cyeoh@au1.ibm.com>, IBM Corp.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/mm.h>
#include <linux/uio.h>
#include <linux/sched.h>
=======
 */

#include <linux/compat.h>
#include <linux/mm.h>
#include <linux/uio.h>
#include <linux/sched.h>
#include <linux/sched/mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/highmem.h>
#include <linux/ptrace.h>
#include <linux/slab.h>
#include <linux/syscalls.h>

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

/**
 * process_vm_rw_pages - read/write pages from task specified
 * @task: task to read/write from
 * @mm: mm for task
 * @process_pages: struct pages area that can store at least
 *  nr_pages_to_copy struct page pointers
 * @pa: address of page in task to start copying from/to
 * @start_offset: offset in page to start copying from/to
 * @len: number of bytes to copy
 * @lvec: iovec array specifying where to copy to/from
 * @lvec_cnt: number of elements in iovec array
 * @lvec_current: index in iovec array we are up to
 * @lvec_offset: offset in bytes from current iovec iov_base we are up to
 * @vm_write: 0 means copy from, 1 means copy to
 * @nr_pages_to_copy: number of pages to copy
 * @bytes_copied: returns number of bytes successfully copied
 * Returns 0 on success, error code otherwise
 */
static int process_vm_rw_pages(struct task_struct *task,
			       struct mm_struct *mm,
			       struct page **process_pages,
			       unsigned long pa,
			       unsigned long start_offset,
			       unsigned long len,
			       const struct iovec *lvec,
			       unsigned long lvec_cnt,
			       unsigned long *lvec_current,
			       size_t *lvec_offset,
			       int vm_write,
			       unsigned int nr_pages_to_copy,
			       ssize_t *bytes_copied)
{
	int pages_pinned;
	void *target_kaddr;
	int pgs_copied = 0;
	int j;
	int ret;
	ssize_t bytes_to_copy;
	ssize_t rc = 0;

	*bytes_copied = 0;

	/* Get the pages we're interested in */
	down_read(&mm->mmap_sem);
	pages_pinned = get_user_pages(task, mm, pa,
				      nr_pages_to_copy,
				      vm_write, 0, process_pages, NULL);
	up_read(&mm->mmap_sem);

	if (pages_pinned != nr_pages_to_copy) {
		rc = -EFAULT;
		goto end;
	}

	/* Do the copy for each page */
	for (pgs_copied = 0;
	     (pgs_copied < nr_pages_to_copy) && (*lvec_current < lvec_cnt);
	     pgs_copied++) {
		/* Make sure we have a non zero length iovec */
		while (*lvec_current < lvec_cnt
		       && lvec[*lvec_current].iov_len == 0)
			(*lvec_current)++;
		if (*lvec_current == lvec_cnt)
			break;

		/*
		 * Will copy smallest of:
		 * - bytes remaining in page
		 * - bytes remaining in destination iovec
		 */
		bytes_to_copy = min_t(ssize_t, PAGE_SIZE - start_offset,
				      len - *bytes_copied);
		bytes_to_copy = min_t(ssize_t, bytes_to_copy,
				      lvec[*lvec_current].iov_len
				      - *lvec_offset);

		target_kaddr = kmap(process_pages[pgs_copied]) + start_offset;

		if (vm_write)
			ret = copy_from_user(target_kaddr,
					     lvec[*lvec_current].iov_base
					     + *lvec_offset,
					     bytes_to_copy);
		else
			ret = copy_to_user(lvec[*lvec_current].iov_base
					   + *lvec_offset,
					   target_kaddr, bytes_to_copy);
		kunmap(process_pages[pgs_copied]);
		if (ret) {
			*bytes_copied += bytes_to_copy - ret;
			pgs_copied++;
			rc = -EFAULT;
			goto end;
		}
		*bytes_copied += bytes_to_copy;
		*lvec_offset += bytes_to_copy;
		if (*lvec_offset == lvec[*lvec_current].iov_len) {
			/*
			 * Need to copy remaining part of page into the
			 * next iovec if there are any bytes left in page
			 */
			(*lvec_current)++;
			*lvec_offset = 0;
			start_offset = (start_offset + bytes_to_copy)
				% PAGE_SIZE;
			if (start_offset)
				pgs_copied--;
		} else {
			start_offset = 0;
		}
	}

end:
	if (vm_write) {
		for (j = 0; j < pages_pinned; j++) {
			if (j < pgs_copied)
				set_page_dirty_lock(process_pages[j]);
			put_page(process_pages[j]);
		}
	} else {
		for (j = 0; j < pages_pinned; j++)
			put_page(process_pages[j]);
	}

	return rc;
}

/* Maximum number of pages kmalloc'd to hold struct page's during copy */
#define PVM_MAX_KMALLOC_PAGES (PAGE_SIZE * 2)
=======
/**
 * process_vm_rw_pages - read/write pages from task specified
 * @pages: array of pointers to pages we want to copy
 * @offset: offset in page to start copying from/to
 * @len: number of bytes to copy
 * @iter: where to copy to/from locally
 * @vm_write: 0 means copy from, 1 means copy to
 * Returns 0 on success, error code otherwise
 */
static int process_vm_rw_pages(struct page **pages,
			       unsigned offset,
			       size_t len,
			       struct iov_iter *iter,
			       int vm_write)
{
	/* Do the copy for each page */
	while (len && iov_iter_count(iter)) {
		struct page *page = *pages++;
		size_t copy = PAGE_SIZE - offset;
		size_t copied;

		if (copy > len)
			copy = len;

		if (vm_write)
			copied = copy_page_from_iter(page, offset, copy, iter);
		else
			copied = copy_page_to_iter(page, offset, copy, iter);

		len -= copied;
		if (copied < copy && iov_iter_count(iter))
			return -EFAULT;
		offset = 0;
	}
	return 0;
}

/* Maximum number of pages kmalloc'd to hold struct page's during copy */
#define PVM_MAX_KMALLOC_PAGES 2

/* Maximum number of pages that can be stored at a time */
#define PVM_MAX_USER_PAGES (PVM_MAX_KMALLOC_PAGES * PAGE_SIZE / sizeof(struct page *))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * process_vm_rw_single_vec - read/write pages from task specified
 * @addr: start memory address of target process
 * @len: size of area to copy to/from
<<<<<<< HEAD
 * @lvec: iovec array specifying where to copy to/from locally
 * @lvec_cnt: number of elements in iovec array
 * @lvec_current: index in iovec array we are up to
 * @lvec_offset: offset in bytes from current iovec iov_base we are up to
=======
 * @iter: where to copy to/from locally
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @process_pages: struct pages area that can store at least
 *  nr_pages_to_copy struct page pointers
 * @mm: mm for task
 * @task: task to read/write from
 * @vm_write: 0 means copy from, 1 means copy to
<<<<<<< HEAD
 * @bytes_copied: returns number of bytes successfully copied
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Returns 0 on success or on failure error code
 */
static int process_vm_rw_single_vec(unsigned long addr,
				    unsigned long len,
<<<<<<< HEAD
				    const struct iovec *lvec,
				    unsigned long lvec_cnt,
				    unsigned long *lvec_current,
				    size_t *lvec_offset,
				    struct page **process_pages,
				    struct mm_struct *mm,
				    struct task_struct *task,
				    int vm_write,
				    ssize_t *bytes_copied)
=======
				    struct iov_iter *iter,
				    struct page **process_pages,
				    struct mm_struct *mm,
				    struct task_struct *task,
				    int vm_write)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long pa = addr & PAGE_MASK;
	unsigned long start_offset = addr - pa;
	unsigned long nr_pages;
<<<<<<< HEAD
	ssize_t bytes_copied_loop;
	ssize_t rc = 0;
	unsigned long nr_pages_copied = 0;
	unsigned long nr_pages_to_copy;
	unsigned long max_pages_per_loop = PVM_MAX_KMALLOC_PAGES
		/ sizeof(struct pages *);

	*bytes_copied = 0;
=======
	ssize_t rc = 0;
	unsigned int flags = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Work out address and page range required */
	if (len == 0)
		return 0;
	nr_pages = (addr + len - 1) / PAGE_SIZE - addr / PAGE_SIZE + 1;

<<<<<<< HEAD
	while ((nr_pages_copied < nr_pages) && (*lvec_current < lvec_cnt)) {
		nr_pages_to_copy = min(nr_pages - nr_pages_copied,
				       max_pages_per_loop);

		rc = process_vm_rw_pages(task, mm, process_pages, pa,
					 start_offset, len,
					 lvec, lvec_cnt,
					 lvec_current, lvec_offset,
					 vm_write, nr_pages_to_copy,
					 &bytes_copied_loop);
		start_offset = 0;
		*bytes_copied += bytes_copied_loop;

		if (rc < 0) {
			return rc;
		} else {
			len -= bytes_copied_loop;
			nr_pages_copied += nr_pages_to_copy;
			pa += nr_pages_to_copy * PAGE_SIZE;
		}
=======
	if (vm_write)
		flags |= FOLL_WRITE;

	while (!rc && nr_pages && iov_iter_count(iter)) {
		int pinned_pages = min_t(unsigned long, nr_pages, PVM_MAX_USER_PAGES);
		int locked = 1;
		size_t bytes;

		/*
		 * Get the pages we're interested in.  We must
		 * access remotely because task/mm might not
		 * current/current->mm
		 */
		mmap_read_lock(mm);
		pinned_pages = pin_user_pages_remote(mm, pa, pinned_pages,
						     flags, process_pages,
						     &locked);
		if (locked)
			mmap_read_unlock(mm);
		if (pinned_pages <= 0)
			return -EFAULT;

		bytes = pinned_pages * PAGE_SIZE - start_offset;
		if (bytes > len)
			bytes = len;

		rc = process_vm_rw_pages(process_pages,
					 start_offset, bytes, iter,
					 vm_write);
		len -= bytes;
		start_offset = 0;
		nr_pages -= pinned_pages;
		pa += pinned_pages * PAGE_SIZE;

		/* If vm_write is set, the pages need to be made dirty: */
		unpin_user_pages_dirty_lock(process_pages, pinned_pages,
					    vm_write);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return rc;
}

/* Maximum number of entries for process pages array
   which lives on stack */
#define PVM_MAX_PP_ARRAY_COUNT 16

/**
 * process_vm_rw_core - core of reading/writing pages from task specified
 * @pid: PID of process to read/write from/to
<<<<<<< HEAD
 * @lvec: iovec array specifying where to copy to/from locally
 * @liovcnt: size of lvec array
=======
 * @iter: where to copy to/from locally
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @rvec: iovec array specifying where to copy to/from in the other process
 * @riovcnt: size of rvec array
 * @flags: currently unused
 * @vm_write: 0 if reading from other process, 1 if writing to other process
<<<<<<< HEAD
=======
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Returns the number of bytes read/written or error code. May
 *  return less bytes than expected if an error occurs during the copying
 *  process.
 */
<<<<<<< HEAD
static ssize_t process_vm_rw_core(pid_t pid, const struct iovec *lvec,
				  unsigned long liovcnt,
=======
static ssize_t process_vm_rw_core(pid_t pid, struct iov_iter *iter,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  const struct iovec *rvec,
				  unsigned long riovcnt,
				  unsigned long flags, int vm_write)
{
	struct task_struct *task;
	struct page *pp_stack[PVM_MAX_PP_ARRAY_COUNT];
	struct page **process_pages = pp_stack;
	struct mm_struct *mm;
	unsigned long i;
	ssize_t rc = 0;
<<<<<<< HEAD
	ssize_t bytes_copied_loop;
	ssize_t bytes_copied = 0;
	unsigned long nr_pages = 0;
	unsigned long nr_pages_iov;
	unsigned long iov_l_curr_idx = 0;
	size_t iov_l_curr_offset = 0;
	ssize_t iov_len;
=======
	unsigned long nr_pages = 0;
	unsigned long nr_pages_iov;
	ssize_t iov_len;
	size_t total_len = iov_iter_count(iter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Work out how many pages of struct pages we're going to need
	 * when eventually calling get_user_pages
	 */
	for (i = 0; i < riovcnt; i++) {
		iov_len = rvec[i].iov_len;
		if (iov_len > 0) {
			nr_pages_iov = ((unsigned long)rvec[i].iov_base
<<<<<<< HEAD
					+ iov_len)
=======
					+ iov_len - 1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				/ PAGE_SIZE - (unsigned long)rvec[i].iov_base
				/ PAGE_SIZE + 1;
			nr_pages = max(nr_pages, nr_pages_iov);
		}
	}

	if (nr_pages == 0)
		return 0;

	if (nr_pages > PVM_MAX_PP_ARRAY_COUNT) {
		/* For reliability don't try to kmalloc more than
		   2 pages worth */
<<<<<<< HEAD
		process_pages = kmalloc(min_t(size_t, PVM_MAX_KMALLOC_PAGES,
					      sizeof(struct pages *)*nr_pages),
=======
		process_pages = kmalloc(min_t(size_t, PVM_MAX_KMALLOC_PAGES * PAGE_SIZE,
					      sizeof(struct page *)*nr_pages),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					GFP_KERNEL);

		if (!process_pages)
			return -ENOMEM;
	}

	/* Get process information */
<<<<<<< HEAD
	rcu_read_lock();
	task = find_task_by_vpid(pid);
	if (task)
		get_task_struct(task);
	rcu_read_unlock();
=======
	task = find_get_task_by_vpid(pid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!task) {
		rc = -ESRCH;
		goto free_proc_pages;
	}

<<<<<<< HEAD
	mm = mm_access(task, PTRACE_MODE_ATTACH);
	if (!mm || IS_ERR(mm)) {
		rc = IS_ERR(mm) ? PTR_ERR(mm) : -ESRCH;
		/*
		 * Explicitly map EACCES to EPERM as EPERM is a more a
=======
	mm = mm_access(task, PTRACE_MODE_ATTACH_REALCREDS);
	if (!mm || IS_ERR(mm)) {
		rc = IS_ERR(mm) ? PTR_ERR(mm) : -ESRCH;
		/*
		 * Explicitly map EACCES to EPERM as EPERM is a more
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * appropriate error code for process_vw_readv/writev
		 */
		if (rc == -EACCES)
			rc = -EPERM;
		goto put_task_struct;
	}

<<<<<<< HEAD
	for (i = 0; i < riovcnt && iov_l_curr_idx < liovcnt; i++) {
		rc = process_vm_rw_single_vec(
			(unsigned long)rvec[i].iov_base, rvec[i].iov_len,
			lvec, liovcnt, &iov_l_curr_idx, &iov_l_curr_offset,
			process_pages, mm, task, vm_write, &bytes_copied_loop);
		bytes_copied += bytes_copied_loop;
		if (rc != 0) {
			/* If we have managed to copy any data at all then
			   we return the number of bytes copied. Otherwise
			   we return the error code */
			if (bytes_copied)
				rc = bytes_copied;
			goto put_mm;
		}
	}

	rc = bytes_copied;
put_mm:
=======
	for (i = 0; i < riovcnt && iov_iter_count(iter) && !rc; i++)
		rc = process_vm_rw_single_vec(
			(unsigned long)rvec[i].iov_base, rvec[i].iov_len,
			iter, process_pages, mm, task, vm_write);

	/* copied = space before - space after */
	total_len -= iov_iter_count(iter);

	/* If we have managed to copy any data at all then
	   we return the number of bytes copied. Otherwise
	   we return the error code */
	if (total_len)
		rc = total_len;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mmput(mm);

put_task_struct:
	put_task_struct(task);

free_proc_pages:
	if (process_pages != pp_stack)
		kfree(process_pages);
	return rc;
}

/**
 * process_vm_rw - check iovecs before calling core routine
 * @pid: PID of process to read/write from/to
 * @lvec: iovec array specifying where to copy to/from locally
 * @liovcnt: size of lvec array
 * @rvec: iovec array specifying where to copy to/from in the other process
 * @riovcnt: size of rvec array
 * @flags: currently unused
 * @vm_write: 0 if reading from other process, 1 if writing to other process
<<<<<<< HEAD
=======
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Returns the number of bytes read/written or error code. May
 *  return less bytes than expected if an error occurs during the copying
 *  process.
 */
static ssize_t process_vm_rw(pid_t pid,
			     const struct iovec __user *lvec,
			     unsigned long liovcnt,
			     const struct iovec __user *rvec,
			     unsigned long riovcnt,
			     unsigned long flags, int vm_write)
{
	struct iovec iovstack_l[UIO_FASTIOV];
	struct iovec iovstack_r[UIO_FASTIOV];
	struct iovec *iov_l = iovstack_l;
<<<<<<< HEAD
	struct iovec *iov_r = iovstack_r;
	ssize_t rc;
=======
	struct iovec *iov_r;
	struct iov_iter iter;
	ssize_t rc;
	int dir = vm_write ? ITER_SOURCE : ITER_DEST;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (flags != 0)
		return -EINVAL;

	/* Check iovecs */
<<<<<<< HEAD
	if (vm_write)
		rc = rw_copy_check_uvector(WRITE, lvec, liovcnt, UIO_FASTIOV,
					   iovstack_l, &iov_l, 1);
	else
		rc = rw_copy_check_uvector(READ, lvec, liovcnt, UIO_FASTIOV,
					   iovstack_l, &iov_l, 1);
	if (rc <= 0)
		goto free_iovecs;

	rc = rw_copy_check_uvector(READ, rvec, riovcnt, UIO_FASTIOV,
				   iovstack_r, &iov_r, 0);
	if (rc <= 0)
		goto free_iovecs;

	rc = process_vm_rw_core(pid, iov_l, liovcnt, iov_r, riovcnt, flags,
				vm_write);

free_iovecs:
	if (iov_r != iovstack_r)
		kfree(iov_r);
	if (iov_l != iovstack_l)
		kfree(iov_l);

=======
	rc = import_iovec(dir, lvec, liovcnt, UIO_FASTIOV, &iov_l, &iter);
	if (rc < 0)
		return rc;
	if (!iov_iter_count(&iter))
		goto free_iov_l;
	iov_r = iovec_from_user(rvec, riovcnt, UIO_FASTIOV, iovstack_r,
				in_compat_syscall());
	if (IS_ERR(iov_r)) {
		rc = PTR_ERR(iov_r);
		goto free_iov_l;
	}
	rc = process_vm_rw_core(pid, &iter, iov_r, riovcnt, flags, vm_write);
	if (iov_r != iovstack_r)
		kfree(iov_r);
free_iov_l:
	kfree(iov_l);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

SYSCALL_DEFINE6(process_vm_readv, pid_t, pid, const struct iovec __user *, lvec,
		unsigned long, liovcnt, const struct iovec __user *, rvec,
		unsigned long, riovcnt,	unsigned long, flags)
{
	return process_vm_rw(pid, lvec, liovcnt, rvec, riovcnt, flags, 0);
}

SYSCALL_DEFINE6(process_vm_writev, pid_t, pid,
		const struct iovec __user *, lvec,
		unsigned long, liovcnt, const struct iovec __user *, rvec,
		unsigned long, riovcnt,	unsigned long, flags)
{
	return process_vm_rw(pid, lvec, liovcnt, rvec, riovcnt, flags, 1);
}
<<<<<<< HEAD

#ifdef CONFIG_COMPAT

asmlinkage ssize_t
compat_process_vm_rw(compat_pid_t pid,
		     const struct compat_iovec __user *lvec,
		     unsigned long liovcnt,
		     const struct compat_iovec __user *rvec,
		     unsigned long riovcnt,
		     unsigned long flags, int vm_write)
{
	struct iovec iovstack_l[UIO_FASTIOV];
	struct iovec iovstack_r[UIO_FASTIOV];
	struct iovec *iov_l = iovstack_l;
	struct iovec *iov_r = iovstack_r;
	ssize_t rc = -EFAULT;

	if (flags != 0)
		return -EINVAL;

	if (vm_write)
		rc = compat_rw_copy_check_uvector(WRITE, lvec, liovcnt,
						  UIO_FASTIOV, iovstack_l,
						  &iov_l, 1);
	else
		rc = compat_rw_copy_check_uvector(READ, lvec, liovcnt,
						  UIO_FASTIOV, iovstack_l,
						  &iov_l, 1);
	if (rc <= 0)
		goto free_iovecs;
	rc = compat_rw_copy_check_uvector(READ, rvec, riovcnt,
					  UIO_FASTIOV, iovstack_r,
					  &iov_r, 0);
	if (rc <= 0)
		goto free_iovecs;

	rc = process_vm_rw_core(pid, iov_l, liovcnt, iov_r, riovcnt, flags,
			   vm_write);

free_iovecs:
	if (iov_r != iovstack_r)
		kfree(iov_r);
	if (iov_l != iovstack_l)
		kfree(iov_l);
	return rc;
}

asmlinkage ssize_t
compat_sys_process_vm_readv(compat_pid_t pid,
			    const struct compat_iovec __user *lvec,
			    unsigned long liovcnt,
			    const struct compat_iovec __user *rvec,
			    unsigned long riovcnt,
			    unsigned long flags)
{
	return compat_process_vm_rw(pid, lvec, liovcnt, rvec,
				    riovcnt, flags, 0);
}

asmlinkage ssize_t
compat_sys_process_vm_writev(compat_pid_t pid,
			     const struct compat_iovec __user *lvec,
			     unsigned long liovcnt,
			     const struct compat_iovec __user *rvec,
			     unsigned long riovcnt,
			     unsigned long flags)
{
	return compat_process_vm_rw(pid, lvec, liovcnt, rvec,
				    riovcnt, flags, 1);
}

#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
