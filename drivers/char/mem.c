<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/drivers/char/mem.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  Added devfs support.
 *    Jan-11-1998, C. Scott Ananian <cananian@alumni.princeton.edu>
 *  Shared /dev/zero mmapping support, Feb 2000, Kanoj Sarcar <kanoj@sgi.com>
 */

#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mman.h>
#include <linux/random.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/raw.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/tty.h>
#include <linux/capability.h>
#include <linux/ptrace.h>
#include <linux/device.h>
#include <linux/highmem.h>
<<<<<<< HEAD
#include <linux/crash_dump.h>
#include <linux/backing-dev.h>
#include <linux/bootmem.h>
#include <linux/splice.h>
#include <linux/pfn.h>
#include <linux/export.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#ifdef CONFIG_IA64
# include <linux/efi.h>
#endif
=======
#include <linux/backing-dev.h>
#include <linux/shmem_fs.h>
#include <linux/splice.h>
#include <linux/pfn.h>
#include <linux/export.h>
#include <linux/io.h>
#include <linux/uio.h>
#include <linux/uaccess.h>
#include <linux/security.h>

#define DEVMEM_MINOR	1
#define DEVPORT_MINOR	4
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline unsigned long size_inside_page(unsigned long start,
					     unsigned long size)
{
	unsigned long sz;

	sz = PAGE_SIZE - (start & (PAGE_SIZE - 1));

	return min(sz, size);
}

#ifndef ARCH_HAS_VALID_PHYS_ADDR_RANGE
<<<<<<< HEAD
static inline int valid_phys_addr_range(unsigned long addr, size_t count)
=======
static inline int valid_phys_addr_range(phys_addr_t addr, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return addr + count <= __pa(high_memory);
}

static inline int valid_mmap_phys_addr_range(unsigned long pfn, size_t size)
{
	return 1;
}
#endif

<<<<<<< HEAD
#if defined(CONFIG_DEVMEM) || defined(CONFIG_DEVKMEM)
#ifdef CONFIG_STRICT_DEVMEM
=======
#ifdef CONFIG_STRICT_DEVMEM
static inline int page_is_allowed(unsigned long pfn)
{
	return devmem_is_allowed(pfn);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int range_is_allowed(unsigned long pfn, unsigned long size)
{
	u64 from = ((u64)pfn) << PAGE_SHIFT;
	u64 to = from + size;
	u64 cursor = from;

	while (cursor < to) {
<<<<<<< HEAD
		if (!devmem_is_allowed(pfn)) {
			printk(KERN_INFO
		"Program %s tried to access /dev/mem between %Lx->%Lx.\n",
				current->comm, from, to);
			return 0;
		}
=======
		if (!devmem_is_allowed(pfn))
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cursor += PAGE_SIZE;
		pfn++;
	}
	return 1;
}
#else
<<<<<<< HEAD
=======
static inline int page_is_allowed(unsigned long pfn)
{
	return 1;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int range_is_allowed(unsigned long pfn, unsigned long size)
{
	return 1;
}
#endif
<<<<<<< HEAD
#endif

#ifdef CONFIG_DEVMEM
void __weak unxlate_dev_mem_ptr(unsigned long phys, void *addr)
{
=======

static inline bool should_stop_iteration(void)
{
	if (need_resched())
		cond_resched();
	return signal_pending(current);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This funcion reads the *physical* memory. The f_pos points directly to the
 * memory location.
 */
static ssize_t read_mem(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	unsigned long p = *ppos;
	ssize_t read, sz;
	char *ptr;
=======
	phys_addr_t p = *ppos;
	ssize_t read, sz;
	void *ptr;
	char *bounce;
	int err;

	if (p != *ppos)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!valid_phys_addr_range(p, count))
		return -EFAULT;
	read = 0;
#ifdef __ARCH_HAS_NO_PAGE_ZERO_MAPPED
	/* we don't have page 0 mapped on sparc and m68k.. */
	if (p < PAGE_SIZE) {
		sz = size_inside_page(p, count);
		if (sz > 0) {
			if (clear_user(buf, sz))
				return -EFAULT;
			buf += sz;
			p += sz;
			count -= sz;
			read += sz;
		}
	}
#endif

<<<<<<< HEAD
	while (count > 0) {
		unsigned long remaining;

		sz = size_inside_page(p, count);

		if (!range_is_allowed(p >> PAGE_SHIFT, count))
			return -EPERM;

		/*
		 * On ia64 if a page has been mapped somewhere as uncached, then
		 * it must also be accessed uncached by the kernel or data
		 * corruption may occur.
		 */
		ptr = xlate_dev_mem_ptr(p);
		if (!ptr)
			return -EFAULT;

		remaining = copy_to_user(buf, ptr, sz);
		unxlate_dev_mem_ptr(p, ptr);
		if (remaining)
			return -EFAULT;
=======
	bounce = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!bounce)
		return -ENOMEM;

	while (count > 0) {
		unsigned long remaining;
		int allowed, probe;

		sz = size_inside_page(p, count);

		err = -EPERM;
		allowed = page_is_allowed(p >> PAGE_SHIFT);
		if (!allowed)
			goto failed;

		err = -EFAULT;
		if (allowed == 2) {
			/* Show zeros for restricted memory. */
			remaining = clear_user(buf, sz);
		} else {
			/*
			 * On ia64 if a page has been mapped somewhere as
			 * uncached, then it must also be accessed uncached
			 * by the kernel or data corruption may occur.
			 */
			ptr = xlate_dev_mem_ptr(p);
			if (!ptr)
				goto failed;

			probe = copy_from_kernel_nofault(bounce, ptr, sz);
			unxlate_dev_mem_ptr(p, ptr);
			if (probe)
				goto failed;

			remaining = copy_to_user(buf, bounce, sz);
		}

		if (remaining)
			goto failed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		buf += sz;
		p += sz;
		count -= sz;
		read += sz;
<<<<<<< HEAD
	}

	*ppos += read;
	return read;
=======
		if (should_stop_iteration())
			break;
	}
	kfree(bounce);

	*ppos += read;
	return read;

failed:
	kfree(bounce);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t write_mem(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	unsigned long p = *ppos;
=======
	phys_addr_t p = *ppos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ssize_t written, sz;
	unsigned long copied;
	void *ptr;

<<<<<<< HEAD
=======
	if (p != *ppos)
		return -EFBIG;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!valid_phys_addr_range(p, count))
		return -EFAULT;

	written = 0;

#ifdef __ARCH_HAS_NO_PAGE_ZERO_MAPPED
	/* we don't have page 0 mapped on sparc and m68k.. */
	if (p < PAGE_SIZE) {
		sz = size_inside_page(p, count);
		/* Hmm. Do something? */
		buf += sz;
		p += sz;
		count -= sz;
		written += sz;
	}
#endif

	while (count > 0) {
<<<<<<< HEAD
		sz = size_inside_page(p, count);

		if (!range_is_allowed(p >> PAGE_SHIFT, sz))
			return -EPERM;

		/*
		 * On ia64 if a page has been mapped somewhere as uncached, then
		 * it must also be accessed uncached by the kernel or data
		 * corruption may occur.
		 */
		ptr = xlate_dev_mem_ptr(p);
		if (!ptr) {
			if (written)
				break;
			return -EFAULT;
		}

		copied = copy_from_user(ptr, buf, sz);
		unxlate_dev_mem_ptr(p, ptr);
		if (copied) {
			written += sz - copied;
			if (written)
				break;
			return -EFAULT;
=======
		int allowed;

		sz = size_inside_page(p, count);

		allowed = page_is_allowed(p >> PAGE_SHIFT);
		if (!allowed)
			return -EPERM;

		/* Skip actual writing when a page is marked as restricted. */
		if (allowed == 1) {
			/*
			 * On ia64 if a page has been mapped somewhere as
			 * uncached, then it must also be accessed uncached
			 * by the kernel or data corruption may occur.
			 */
			ptr = xlate_dev_mem_ptr(p);
			if (!ptr) {
				if (written)
					break;
				return -EFAULT;
			}

			copied = copy_from_user(ptr, buf, sz);
			unxlate_dev_mem_ptr(p, ptr);
			if (copied) {
				written += sz - copied;
				if (written)
					break;
				return -EFAULT;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		buf += sz;
		p += sz;
		count -= sz;
		written += sz;
<<<<<<< HEAD
=======
		if (should_stop_iteration())
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	*ppos += written;
	return written;
}
<<<<<<< HEAD
#endif	/* CONFIG_DEVMEM */

#if defined(CONFIG_DEVMEM) || defined(CONFIG_DEVKMEM)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int __weak phys_mem_access_prot_allowed(struct file *file,
	unsigned long pfn, unsigned long size, pgprot_t *vma_prot)
{
	return 1;
}

#ifndef __HAVE_PHYS_MEM_ACCESS_PROT

/*
 * Architectures vary in how they handle caching for addresses
 * outside of main memory.
 *
 */
#ifdef pgprot_noncached
<<<<<<< HEAD
static int uncached_access(struct file *file, unsigned long addr)
{
#if defined(CONFIG_IA64)
	/*
	 * On ia64, we ignore O_DSYNC because we cannot tolerate memory
	 * attribute aliases.
	 */
	return !(efi_mem_attributes(addr) & EFI_MEMORY_WB);
#elif defined(CONFIG_MIPS)
	{
		extern int __uncached_access(struct file *file,
					     unsigned long addr);

		return __uncached_access(file, addr);
	}
#else
=======
static int uncached_access(struct file *file, phys_addr_t addr)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Accessing memory above the top the kernel knows about or through a
	 * file pointer
	 * that was marked O_DSYNC will be done non-cached.
	 */
	if (file->f_flags & O_DSYNC)
		return 1;
	return addr >= __pa(high_memory);
<<<<<<< HEAD
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

static pgprot_t phys_mem_access_prot(struct file *file, unsigned long pfn,
				     unsigned long size, pgprot_t vma_prot)
{
#ifdef pgprot_noncached
<<<<<<< HEAD
	unsigned long offset = pfn << PAGE_SHIFT;
=======
	phys_addr_t offset = pfn << PAGE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (uncached_access(file, offset))
		return pgprot_noncached(vma_prot);
#endif
	return vma_prot;
}
#endif

#ifndef CONFIG_MMU
static unsigned long get_unmapped_area_mem(struct file *file,
					   unsigned long addr,
					   unsigned long len,
					   unsigned long pgoff,
					   unsigned long flags)
{
	if (!valid_mmap_phys_addr_range(pgoff, len))
		return (unsigned long) -EINVAL;
	return pgoff << PAGE_SHIFT;
}

<<<<<<< HEAD
/* can't do an in-place private mapping if there's no MMU */
static inline int private_mapping_ok(struct vm_area_struct *vma)
{
	return vma->vm_flags & VM_MAYSHARE;
}
#else
#define get_unmapped_area_mem	NULL
=======
/* permit direct mmap, for read, write or exec */
static unsigned memory_mmap_capabilities(struct file *file)
{
	return NOMMU_MAP_DIRECT |
		NOMMU_MAP_READ | NOMMU_MAP_WRITE | NOMMU_MAP_EXEC;
}

static unsigned zero_mmap_capabilities(struct file *file)
{
	return NOMMU_MAP_COPY;
}

/* can't do an in-place private mapping if there's no MMU */
static inline int private_mapping_ok(struct vm_area_struct *vma)
{
	return is_nommu_shared_mapping(vma->vm_flags);
}
#else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int private_mapping_ok(struct vm_area_struct *vma)
{
	return 1;
}
#endif

static const struct vm_operations_struct mmap_mem_ops = {
#ifdef CONFIG_HAVE_IOREMAP_PROT
	.access = generic_access_phys
#endif
};

static int mmap_mem(struct file *file, struct vm_area_struct *vma)
{
	size_t size = vma->vm_end - vma->vm_start;
<<<<<<< HEAD
=======
	phys_addr_t offset = (phys_addr_t)vma->vm_pgoff << PAGE_SHIFT;

	/* Does it even fit in phys_addr_t? */
	if (offset >> PAGE_SHIFT != vma->vm_pgoff)
		return -EINVAL;

	/* It's illegal to wrap around the end of the physical address space. */
	if (offset + (phys_addr_t)size - 1 < offset)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!valid_mmap_phys_addr_range(vma->vm_pgoff, size))
		return -EINVAL;

	if (!private_mapping_ok(vma))
		return -ENOSYS;

	if (!range_is_allowed(vma->vm_pgoff, size))
		return -EPERM;

	if (!phys_mem_access_prot_allowed(file, vma->vm_pgoff, size,
						&vma->vm_page_prot))
		return -EINVAL;

	vma->vm_page_prot = phys_mem_access_prot(file, vma->vm_pgoff,
						 size,
						 vma->vm_page_prot);

	vma->vm_ops = &mmap_mem_ops;

<<<<<<< HEAD
	/* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
=======
	/* Remap-pfn-range will mark the range VM_IO */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (remap_pfn_range(vma,
			    vma->vm_start,
			    vma->vm_pgoff,
			    size,
			    vma->vm_page_prot)) {
		return -EAGAIN;
	}
	return 0;
}
<<<<<<< HEAD
#endif	/* CONFIG_DEVMEM */

#ifdef CONFIG_DEVKMEM
static int mmap_kmem(struct file *file, struct vm_area_struct *vma)
{
	unsigned long pfn;

	/* Turn a kernel-virtual address into a physical page frame */
	pfn = __pa((u64)vma->vm_pgoff << PAGE_SHIFT) >> PAGE_SHIFT;

	/*
	 * RED-PEN: on some architectures there is more mapped memory than
	 * available in mem_map which pfn_valid checks for. Perhaps should add a
	 * new macro here.
	 *
	 * RED-PEN: vmalloc is not supported right now.
	 */
	if (!pfn_valid(pfn))
		return -EIO;

	vma->vm_pgoff = pfn;
	return mmap_mem(file, vma);
}
#endif

#ifdef CONFIG_CRASH_DUMP
/*
 * Read memory corresponding to the old kernel.
 */
static ssize_t read_oldmem(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
	unsigned long pfn, offset;
	size_t read = 0, csize;
	int rc = 0;

	while (count) {
		pfn = *ppos / PAGE_SIZE;
		if (pfn > saved_max_pfn)
			return read;

		offset = (unsigned long)(*ppos % PAGE_SIZE);
		if (count > PAGE_SIZE - offset)
			csize = PAGE_SIZE - offset;
		else
			csize = count;

		rc = copy_oldmem_page(pfn, buf, csize, offset, 1);
		if (rc < 0)
			return rc;
		buf += csize;
		*ppos += csize;
		read += csize;
		count -= csize;
	}
	return read;
}
#endif

#ifdef CONFIG_DEVKMEM
/*
 * This function reads the *virtual* memory as seen by the kernel.
 */
static ssize_t read_kmem(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos)
{
	unsigned long p = *ppos;
	ssize_t low_count, read, sz;
	char * kbuf; /* k-addr because vread() takes vmlist_lock rwlock */
	int err = 0;

	read = 0;
	if (p < (unsigned long) high_memory) {
		low_count = count;
		if (count > (unsigned long)high_memory - p)
			low_count = (unsigned long)high_memory - p;

#ifdef __ARCH_HAS_NO_PAGE_ZERO_MAPPED
		/* we don't have page 0 mapped on sparc and m68k.. */
		if (p < PAGE_SIZE && low_count > 0) {
			sz = size_inside_page(p, low_count);
			if (clear_user(buf, sz))
				return -EFAULT;
			buf += sz;
			p += sz;
			read += sz;
			low_count -= sz;
			count -= sz;
		}
#endif
		while (low_count > 0) {
			sz = size_inside_page(p, low_count);

			/*
			 * On ia64 if a page has been mapped somewhere as
			 * uncached, then it must also be accessed uncached
			 * by the kernel or data corruption may occur
			 */
			kbuf = xlate_dev_kmem_ptr((char *)p);

			if (copy_to_user(buf, kbuf, sz))
				return -EFAULT;
			buf += sz;
			p += sz;
			read += sz;
			low_count -= sz;
			count -= sz;
		}
	}

	if (count > 0) {
		kbuf = (char *)__get_free_page(GFP_KERNEL);
		if (!kbuf)
			return -ENOMEM;
		while (count > 0) {
			sz = size_inside_page(p, count);
			if (!is_vmalloc_or_module_addr((void *)p)) {
				err = -ENXIO;
				break;
			}
			sz = vread(kbuf, (char *)p, sz);
			if (!sz)
				break;
			if (copy_to_user(buf, kbuf, sz)) {
				err = -EFAULT;
				break;
			}
			count -= sz;
			buf += sz;
			read += sz;
			p += sz;
		}
		free_page((unsigned long)kbuf);
	}
	*ppos = p;
	return read ? read : err;
}


static ssize_t do_write_kmem(unsigned long p, const char __user *buf,
				size_t count, loff_t *ppos)
{
	ssize_t written, sz;
	unsigned long copied;

	written = 0;
#ifdef __ARCH_HAS_NO_PAGE_ZERO_MAPPED
	/* we don't have page 0 mapped on sparc and m68k.. */
	if (p < PAGE_SIZE) {
		sz = size_inside_page(p, count);
		/* Hmm. Do something? */
		buf += sz;
		p += sz;
		count -= sz;
		written += sz;
	}
#endif

	while (count > 0) {
		char *ptr;

		sz = size_inside_page(p, count);

		/*
		 * On ia64 if a page has been mapped somewhere as uncached, then
		 * it must also be accessed uncached by the kernel or data
		 * corruption may occur.
		 */
		ptr = xlate_dev_kmem_ptr((char *)p);

		copied = copy_from_user(ptr, buf, sz);
		if (copied) {
			written += sz - copied;
			if (written)
				break;
			return -EFAULT;
		}
		buf += sz;
		p += sz;
		count -= sz;
		written += sz;
	}

	*ppos += written;
	return written;
}

/*
 * This function writes to the *virtual* memory as seen by the kernel.
 */
static ssize_t write_kmem(struct file *file, const char __user *buf,
			  size_t count, loff_t *ppos)
{
	unsigned long p = *ppos;
	ssize_t wrote = 0;
	ssize_t virtr = 0;
	char * kbuf; /* k-addr because vwrite() takes vmlist_lock rwlock */
	int err = 0;

	if (p < (unsigned long) high_memory) {
		unsigned long to_write = min_t(unsigned long, count,
					       (unsigned long)high_memory - p);
		wrote = do_write_kmem(p, buf, to_write, ppos);
		if (wrote != to_write)
			return wrote;
		p += wrote;
		buf += wrote;
		count -= wrote;
	}

	if (count > 0) {
		kbuf = (char *)__get_free_page(GFP_KERNEL);
		if (!kbuf)
			return wrote ? wrote : -ENOMEM;
		while (count > 0) {
			unsigned long sz = size_inside_page(p, count);
			unsigned long n;

			if (!is_vmalloc_or_module_addr((void *)p)) {
				err = -ENXIO;
				break;
			}
			n = copy_from_user(kbuf, buf, sz);
			if (n) {
				err = -EFAULT;
				break;
			}
			vwrite(kbuf, (char *)p, sz);
			count -= sz;
			buf += sz;
			virtr += sz;
			p += sz;
		}
		free_page((unsigned long)kbuf);
	}

	*ppos = p;
	return virtr + wrote ? : err;
}
#endif

#ifdef CONFIG_DEVPORT
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t read_port(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos)
{
	unsigned long i = *ppos;
	char __user *tmp = buf;

<<<<<<< HEAD
	if (!access_ok(VERIFY_WRITE, buf, count))
=======
	if (!access_ok(buf, count))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;
	while (count-- > 0 && i < 65536) {
		if (__put_user(inb(i), tmp) < 0)
			return -EFAULT;
		i++;
		tmp++;
	}
	*ppos = i;
	return tmp-buf;
}

static ssize_t write_port(struct file *file, const char __user *buf,
			  size_t count, loff_t *ppos)
{
	unsigned long i = *ppos;
<<<<<<< HEAD
	const char __user * tmp = buf;

	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;
	while (count-- > 0 && i < 65536) {
		char c;
=======
	const char __user *tmp = buf;

	if (!access_ok(buf, count))
		return -EFAULT;
	while (count-- > 0 && i < 65536) {
		char c;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (__get_user(c, tmp)) {
			if (tmp > buf)
				break;
			return -EFAULT;
		}
		outb(c, i);
		i++;
		tmp++;
	}
	*ppos = i;
	return tmp-buf;
}
<<<<<<< HEAD
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t read_null(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos)
{
	return 0;
}

static ssize_t write_null(struct file *file, const char __user *buf,
			  size_t count, loff_t *ppos)
{
	return count;
}

<<<<<<< HEAD
=======
static ssize_t read_iter_null(struct kiocb *iocb, struct iov_iter *to)
{
	return 0;
}

static ssize_t write_iter_null(struct kiocb *iocb, struct iov_iter *from)
{
	size_t count = iov_iter_count(from);
	iov_iter_advance(from, count);
	return count;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int pipe_to_null(struct pipe_inode_info *info, struct pipe_buffer *buf,
			struct splice_desc *sd)
{
	return sd->len;
}

static ssize_t splice_write_null(struct pipe_inode_info *pipe, struct file *out,
				 loff_t *ppos, size_t len, unsigned int flags)
{
	return splice_from_pipe(pipe, out, ppos, len, flags, pipe_to_null);
}

<<<<<<< HEAD
static ssize_t read_zero(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos)
{
	size_t written;

	if (!count)
		return 0;

	if (!access_ok(VERIFY_WRITE, buf, count))
		return -EFAULT;

	written = 0;
	while (count) {
		unsigned long unwritten;
		size_t chunk = count;

		if (chunk > PAGE_SIZE)
			chunk = PAGE_SIZE;	/* Just for latency reasons */
		unwritten = __clear_user(buf, chunk);
		written += chunk - unwritten;
		if (unwritten)
			break;
		if (signal_pending(current))
			return written ? written : -ERESTARTSYS;
		buf += chunk;
		count -= chunk;
		cond_resched();
	}
	return written ? written : -EFAULT;
=======
static int uring_cmd_null(struct io_uring_cmd *ioucmd, unsigned int issue_flags)
{
	return 0;
}

static ssize_t read_iter_zero(struct kiocb *iocb, struct iov_iter *iter)
{
	size_t written = 0;

	while (iov_iter_count(iter)) {
		size_t chunk = iov_iter_count(iter), n;

		if (chunk > PAGE_SIZE)
			chunk = PAGE_SIZE;	/* Just for latency reasons */
		n = iov_iter_zero(chunk, iter);
		if (!n && iov_iter_count(iter))
			return written ? written : -EFAULT;
		written += n;
		if (signal_pending(current))
			return written ? written : -ERESTARTSYS;
		if (!need_resched())
			continue;
		if (iocb->ki_flags & IOCB_NOWAIT)
			return written ? written : -EAGAIN;
		cond_resched();
	}
	return written;
}

static ssize_t read_zero(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos)
{
	size_t cleared = 0;

	while (count) {
		size_t chunk = min_t(size_t, count, PAGE_SIZE);
		size_t left;

		left = clear_user(buf + cleared, chunk);
		if (unlikely(left)) {
			cleared += (chunk - left);
			if (!cleared)
				return -EFAULT;
			break;
		}
		cleared += chunk;
		count -= chunk;

		if (signal_pending(current))
			break;
		cond_resched();
	}

	return cleared;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mmap_zero(struct file *file, struct vm_area_struct *vma)
{
#ifndef CONFIG_MMU
	return -ENOSYS;
#endif
	if (vma->vm_flags & VM_SHARED)
		return shmem_zero_setup(vma);
<<<<<<< HEAD
	return 0;
}

=======
	vma_set_anonymous(vma);
	return 0;
}

static unsigned long get_unmapped_area_zero(struct file *file,
				unsigned long addr, unsigned long len,
				unsigned long pgoff, unsigned long flags)
{
#ifdef CONFIG_MMU
	if (flags & MAP_SHARED) {
		/*
		 * mmap_zero() will call shmem_zero_setup() to create a file,
		 * so use shmem's get_unmapped_area in case it can be huge;
		 * and pass NULL for file as in mmap.c's get_unmapped_area(),
		 * so as not to confuse shmem with our handle on "/dev/zero".
		 */
		return shmem_get_unmapped_area(NULL, addr, len, pgoff, flags);
	}

	/* Otherwise flags & MAP_PRIVATE: with no shmem object beneath it */
	return current->mm->get_unmapped_area(file, addr, len, pgoff, flags);
#else
	return -ENOSYS;
#endif
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t write_full(struct file *file, const char __user *buf,
			  size_t count, loff_t *ppos)
{
	return -ENOSPC;
}

/*
 * Special lseek() function for /dev/null and /dev/zero.  Most notably, you
 * can fopen() both devices with "a" now.  This was previously impossible.
 * -- SRB.
 */
static loff_t null_lseek(struct file *file, loff_t offset, int orig)
{
	return file->f_pos = 0;
}

<<<<<<< HEAD
#if defined(CONFIG_DEVMEM) || defined(CONFIG_DEVKMEM) || defined(CONFIG_DEVPORT)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * The memory devices use the full 32/64 bits of the offset, and so we cannot
 * check against negative addresses: they are ok. The return value is weird,
 * though, in that case (0).
 *
 * also note that seeking relative to the "end of file" isn't supported:
 * it has no meaning, so it returns -EINVAL.
 */
static loff_t memory_lseek(struct file *file, loff_t offset, int orig)
{
	loff_t ret;

<<<<<<< HEAD
	mutex_lock(&file->f_path.dentry->d_inode->i_mutex);
	switch (orig) {
	case SEEK_CUR:
		offset += file->f_pos;
	case SEEK_SET:
		/* to avoid userland mistaking f_pos=-9 as -EBADF=-9 */
		if ((unsigned long long)offset >= ~0xFFFULL) {
=======
	inode_lock(file_inode(file));
	switch (orig) {
	case SEEK_CUR:
		offset += file->f_pos;
		fallthrough;
	case SEEK_SET:
		/* to avoid userland mistaking f_pos=-9 as -EBADF=-9 */
		if ((unsigned long long)offset >= -MAX_ERRNO) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -EOVERFLOW;
			break;
		}
		file->f_pos = offset;
		ret = file->f_pos;
		force_successful_syscall_return();
		break;
	default:
		ret = -EINVAL;
	}
<<<<<<< HEAD
	mutex_unlock(&file->f_path.dentry->d_inode->i_mutex);
	return ret;
}

#endif

#if defined(CONFIG_DEVMEM) || defined(CONFIG_DEVKMEM) || defined(CONFIG_DEVPORT)
static int open_port(struct inode * inode, struct file * filp)
{
	return capable(CAP_SYS_RAWIO) ? 0 : -EPERM;
}
#endif
=======
	inode_unlock(file_inode(file));
	return ret;
}

static int open_port(struct inode *inode, struct file *filp)
{
	int rc;

	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;

	rc = security_locked_down(LOCKDOWN_DEV_MEM);
	if (rc)
		return rc;

	if (iminor(inode) != DEVMEM_MINOR)
		return 0;

	/*
	 * Use a unified address space to have a single point to manage
	 * revocations when drivers want to take over a /dev/mem mapped
	 * range.
	 */
	filp->f_mapping = iomem_get_mapping();

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define zero_lseek	null_lseek
#define full_lseek      null_lseek
#define write_zero	write_null
<<<<<<< HEAD
#define read_full       read_zero
#define open_mem	open_port
#define open_kmem	open_mem
#define open_oldmem	open_mem

#ifdef CONFIG_DEVMEM
static const struct file_operations mem_fops = {
=======
#define write_iter_zero	write_iter_null
#define splice_write_zero	splice_write_null
#define open_mem	open_port

static const struct file_operations __maybe_unused mem_fops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek		= memory_lseek,
	.read		= read_mem,
	.write		= write_mem,
	.mmap		= mmap_mem,
	.open		= open_mem,
<<<<<<< HEAD
	.get_unmapped_area = get_unmapped_area_mem,
};
#endif

#ifdef CONFIG_DEVKMEM
static const struct file_operations kmem_fops = {
	.llseek		= memory_lseek,
	.read		= read_kmem,
	.write		= write_kmem,
	.mmap		= mmap_kmem,
	.open		= open_kmem,
	.get_unmapped_area = get_unmapped_area_mem,
};
#endif
=======
#ifndef CONFIG_MMU
	.get_unmapped_area = get_unmapped_area_mem,
	.mmap_capabilities = memory_mmap_capabilities,
#endif
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct file_operations null_fops = {
	.llseek		= null_lseek,
	.read		= read_null,
	.write		= write_null,
<<<<<<< HEAD
	.splice_write	= splice_write_null,
};

#ifdef CONFIG_DEVPORT
static const struct file_operations port_fops = {
=======
	.read_iter	= read_iter_null,
	.write_iter	= write_iter_null,
	.splice_write	= splice_write_null,
	.uring_cmd	= uring_cmd_null,
};

static const struct file_operations __maybe_unused port_fops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek		= memory_lseek,
	.read		= read_port,
	.write		= write_port,
	.open		= open_port,
};
<<<<<<< HEAD
#endif

static const struct file_operations zero_fops = {
	.llseek		= zero_lseek,
	.read		= read_zero,
	.write		= write_zero,
	.mmap		= mmap_zero,
};

/*
 * capabilities for /dev/zero
 * - permits private mappings, "copies" are taken of the source of zeros
 * - no writeback happens
 */
static struct backing_dev_info zero_bdi = {
	.name		= "char/mem",
	.capabilities	= BDI_CAP_MAP_COPY | BDI_CAP_NO_ACCT_AND_WRITEBACK,
=======

static const struct file_operations zero_fops = {
	.llseek		= zero_lseek,
	.write		= write_zero,
	.read_iter	= read_iter_zero,
	.read		= read_zero,
	.write_iter	= write_iter_zero,
	.splice_read	= copy_splice_read,
	.splice_write	= splice_write_zero,
	.mmap		= mmap_zero,
	.get_unmapped_area = get_unmapped_area_zero,
#ifndef CONFIG_MMU
	.mmap_capabilities = zero_mmap_capabilities,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct file_operations full_fops = {
	.llseek		= full_lseek,
<<<<<<< HEAD
	.read		= read_full,
	.write		= write_full,
};

#ifdef CONFIG_CRASH_DUMP
static const struct file_operations oldmem_fops = {
	.read	= read_oldmem,
	.open	= open_oldmem,
	.llseek = default_llseek,
};
#endif

static const struct memdev {
	const char *name;
	umode_t mode;
	const struct file_operations *fops;
	struct backing_dev_info *dev_info;
} devlist[] = {
#ifdef CONFIG_DEVMEM
	 [1] = { "mem", 0, &mem_fops, &directly_mappable_cdev_bdi },
#endif
#ifdef CONFIG_DEVKMEM
	 [2] = { "kmem", 0, &kmem_fops, &directly_mappable_cdev_bdi },
#endif
	 [3] = { "null", 0666, &null_fops, NULL },
#ifdef CONFIG_DEVPORT
	 [4] = { "port", 0, &port_fops, NULL },
#endif
	 [5] = { "zero", 0666, &zero_fops, &zero_bdi },
	 [7] = { "full", 0666, &full_fops, NULL },
	 [8] = { "random", 0666, &random_fops, NULL },
	 [9] = { "urandom", 0666, &urandom_fops, NULL },
#ifdef CONFIG_PRINTK
	[11] = { "kmsg", 0644, &kmsg_fops, NULL },
#endif
#ifdef CONFIG_CRASH_DUMP
	[12] = { "oldmem", 0, &oldmem_fops, NULL },
=======
	.read_iter	= read_iter_zero,
	.write		= write_full,
	.splice_read	= copy_splice_read,
};

static const struct memdev {
	const char *name;
	const struct file_operations *fops;
	fmode_t fmode;
	umode_t mode;
} devlist[] = {
#ifdef CONFIG_DEVMEM
	[DEVMEM_MINOR] = { "mem", &mem_fops, FMODE_UNSIGNED_OFFSET, 0 },
#endif
	[3] = { "null", &null_fops, FMODE_NOWAIT, 0666 },
#ifdef CONFIG_DEVPORT
	[4] = { "port", &port_fops, 0, 0 },
#endif
	[5] = { "zero", &zero_fops, FMODE_NOWAIT, 0666 },
	[7] = { "full", &full_fops, 0, 0666 },
	[8] = { "random", &random_fops, FMODE_NOWAIT, 0666 },
	[9] = { "urandom", &urandom_fops, FMODE_NOWAIT, 0666 },
#ifdef CONFIG_PRINTK
	[11] = { "kmsg", &kmsg_fops, 0, 0644 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
};

static int memory_open(struct inode *inode, struct file *filp)
{
	int minor;
	const struct memdev *dev;

	minor = iminor(inode);
	if (minor >= ARRAY_SIZE(devlist))
		return -ENXIO;

	dev = &devlist[minor];
	if (!dev->fops)
		return -ENXIO;

	filp->f_op = dev->fops;
<<<<<<< HEAD
	if (dev->dev_info)
		filp->f_mapping->backing_dev_info = dev->dev_info;

	/* Is /dev/mem or /dev/kmem ? */
	if (dev->dev_info == &directly_mappable_cdev_bdi)
		filp->f_mode |= FMODE_UNSIGNED_OFFSET;
=======
	filp->f_mode |= dev->fmode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (dev->fops->open)
		return dev->fops->open(inode, filp);

	return 0;
}

static const struct file_operations memory_fops = {
	.open = memory_open,
	.llseek = noop_llseek,
};

<<<<<<< HEAD
static char *mem_devnode(struct device *dev, umode_t *mode)
=======
static char *mem_devnode(const struct device *dev, umode_t *mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (mode && devlist[MINOR(dev->devt)].mode)
		*mode = devlist[MINOR(dev->devt)].mode;
	return NULL;
}

<<<<<<< HEAD
static struct class *mem_class;

static int __init chr_dev_init(void)
{
	int minor;
	int err;

	err = bdi_init(&zero_bdi);
	if (err)
		return err;
=======
static const struct class mem_class = {
	.name		= "mem",
	.devnode	= mem_devnode,
};

static int __init chr_dev_init(void)
{
	int retval;
	int minor;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (register_chrdev(MEM_MAJOR, "mem", &memory_fops))
		printk("unable to get major %d for memory devs\n", MEM_MAJOR);

<<<<<<< HEAD
	mem_class = class_create(THIS_MODULE, "mem");
	if (IS_ERR(mem_class))
		return PTR_ERR(mem_class);

	mem_class->devnode = mem_devnode;
	for (minor = 1; minor < ARRAY_SIZE(devlist); minor++) {
		if (!devlist[minor].name)
			continue;
		device_create(mem_class, NULL, MKDEV(MEM_MAJOR, minor),
=======
	retval = class_register(&mem_class);
	if (retval)
		return retval;

	for (minor = 1; minor < ARRAY_SIZE(devlist); minor++) {
		if (!devlist[minor].name)
			continue;

		/*
		 * Create /dev/port?
		 */
		if ((minor == DEVPORT_MINOR) && !arch_has_dev_port())
			continue;

		device_create(&mem_class, NULL, MKDEV(MEM_MAJOR, minor),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			      NULL, devlist[minor].name);
	}

	return tty_init();
}

fs_initcall(chr_dev_init);
