<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *    PARISC specific syscalls
 *
 *    Copyright (C) 1999-2003 Matthew Wilcox <willy at parisc-linux.org>
 *    Copyright (C) 2000-2003 Paul Bame <bame at parisc-linux.org>
 *    Copyright (C) 2001 Thomas Bogendoerfer <tsbogend at parisc-linux.org>
<<<<<<< HEAD
 *
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <asm/uaccess.h>
=======
 *    Copyright (C) 1999-2020 Helge Deller <deller@gmx.de>
 */

#include <linux/uaccess.h>
#include <asm/elf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/linkage.h>
#include <linux/mm.h>
#include <linux/mman.h>
<<<<<<< HEAD
=======
#include <linux/sched/signal.h>
#include <linux/sched/mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/shm.h>
#include <linux/syscalls.h>
#include <linux/utsname.h>
#include <linux/personality.h>
<<<<<<< HEAD

static unsigned long get_unshared_area(unsigned long addr, unsigned long len)
{
	struct vm_area_struct *vma, *prev;
	unsigned long prev_end;

	addr = PAGE_ALIGN(addr);

	for (vma = find_vma_prev(current->mm, addr, &prev); ; prev = vma,
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
		if (TASK_SIZE - len < addr)
			return -ENOMEM;
		if (!vma || addr + len <= vm_start_gap(vma))
			return addr;
	}
}

#define DCACHE_ALIGN(addr) (((addr) + (SHMLBA - 1)) &~ (SHMLBA - 1))

/*
 * We need to know the offset to use.  Old scheme was to look for
 * existing mapping and use the same offset.  New scheme is to use the
 * address of the kernel data structure as the seed for the offset.
 * We'll see how that works...
 *
 * The mapping is cacheline aligned, so there's no information in the bottom
 * few bits of the address.  We're looking for 10 bits (4MB / 4k), so let's
 * drop the bottom 8 bits and use bits 8-17.  
 */
static int get_offset(struct address_space *mapping)
{
	int offset = (unsigned long) mapping << (PAGE_SHIFT - 8);
	return offset & 0x3FF000;
}

static unsigned long get_shared_area(struct address_space *mapping,
		unsigned long addr, unsigned long len, unsigned long pgoff)
{
	struct vm_area_struct *vma, *prev;
	unsigned long prev_end;
	int offset = mapping ? get_offset(mapping) : 0;

	offset = (offset + (pgoff << PAGE_SHIFT)) & 0x3FF000;

	addr = DCACHE_ALIGN(addr - offset) + offset;

	for (vma = find_vma_prev(current->mm, addr, &prev); ; prev = vma,
							vma = vma->vm_next) {
		if (prev) {
			prev_end = vm_end_gap(prev);
			if (addr < prev_end) {
				addr = DCACHE_ALIGN(prev_end - offset) + offset;
				if (addr < prev_end)	/* handle wraparound */
					return -ENOMEM;
				/* If vma already violates gap, forget it */
				if (vma && addr > vma->vm_start)
					addr = vma->vm_start;
			}
		}
		/* At this point:  (!vma || addr < vma->vm_end). */
		if (TASK_SIZE - len < addr)
			return -ENOMEM;
		if (!vma || addr + len <= vm_start_gap(vma))
			return addr;
	}
}

unsigned long arch_get_unmapped_area(struct file *filp, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	if (len > TASK_SIZE)
		return -ENOMEM;
	/* Might want to check for cache aliasing issues for MAP_FIXED case
	 * like ARM or MIPS ??? --BenH.
	 */
	if (flags & MAP_FIXED)
		return addr;
	if (!addr)
		addr = TASK_UNMAPPED_BASE;

	if (filp) {
		addr = get_shared_area(filp->f_mapping, addr, len, pgoff);
	} else if(flags & MAP_SHARED) {
		addr = get_shared_area(NULL, addr, len, pgoff);
	} else {
		addr = get_unshared_area(addr, len);
	}
	return addr;
=======
#include <linux/random.h>
#include <linux/compat.h>
#include <linux/elf-randomize.h>

/*
 * Construct an artificial page offset for the mapping based on the physical
 * address of the kernel file mapping variable.
 */
#define GET_FILP_PGOFF(filp)		\
	(filp ? (((unsigned long) filp->f_mapping) >> 8)	\
		 & ((SHM_COLOUR-1) >> PAGE_SHIFT) : 0UL)

static unsigned long shared_align_offset(unsigned long filp_pgoff,
					 unsigned long pgoff)
{
	return (filp_pgoff + pgoff) << PAGE_SHIFT;
}

static inline unsigned long COLOR_ALIGN(unsigned long addr,
			 unsigned long filp_pgoff, unsigned long pgoff)
{
	unsigned long base = (addr+SHM_COLOUR-1) & ~(SHM_COLOUR-1);
	unsigned long off  = (SHM_COLOUR-1) &
		shared_align_offset(filp_pgoff, pgoff);
	return base + off;
}


#define STACK_SIZE_DEFAULT (USER_WIDE_MODE			\
			? (1 << 30)	/* 1 GB */		\
			: (CONFIG_STACK_MAX_DEFAULT_SIZE_MB*1024*1024))

unsigned long calc_max_stack_size(unsigned long stack_max)
{
#ifdef CONFIG_COMPAT
	if (!USER_WIDE_MODE && (stack_max == COMPAT_RLIM_INFINITY))
		stack_max = STACK_SIZE_DEFAULT;
	else
#endif
	if (stack_max == RLIM_INFINITY)
		stack_max = STACK_SIZE_DEFAULT;

	return stack_max;
}


/*
 * Top of mmap area (just below the process stack).
 */

/*
 * When called from arch_get_unmapped_area(), rlim_stack will be NULL,
 * indicating that "current" should be used instead of a passed-in
 * value from the exec bprm as done with arch_pick_mmap_layout().
 */
unsigned long mmap_upper_limit(struct rlimit *rlim_stack)
{
	unsigned long stack_base;

	/* Limit stack size - see setup_arg_pages() in fs/exec.c */
	stack_base = rlim_stack ? rlim_stack->rlim_max
				: rlimit_max(RLIMIT_STACK);

	stack_base = calc_max_stack_size(stack_base);

	/* Add space for stack randomization. */
	if (current->flags & PF_RANDOMIZE)
		stack_base += (STACK_RND_MASK << PAGE_SHIFT);

	return PAGE_ALIGN(STACK_TOP - stack_base);
}

enum mmap_allocation_direction {UP, DOWN};

static unsigned long arch_get_unmapped_area_common(struct file *filp,
	unsigned long addr, unsigned long len, unsigned long pgoff,
	unsigned long flags, enum mmap_allocation_direction dir)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma, *prev;
	unsigned long filp_pgoff;
	int do_color_align;
	struct vm_unmapped_area_info info;

	if (unlikely(len > TASK_SIZE))
		return -ENOMEM;

	do_color_align = 0;
	if (filp || (flags & MAP_SHARED))
		do_color_align = 1;
	filp_pgoff = GET_FILP_PGOFF(filp);

	if (flags & MAP_FIXED) {
		/* Even MAP_FIXED mappings must reside within TASK_SIZE */
		if (TASK_SIZE - len < addr)
			return -EINVAL;

		if ((flags & MAP_SHARED) && filp &&
		    (addr - shared_align_offset(filp_pgoff, pgoff))
				& (SHM_COLOUR - 1))
			return -EINVAL;
		return addr;
	}

	if (addr) {
		if (do_color_align)
			addr = COLOR_ALIGN(addr, filp_pgoff, pgoff);
		else
			addr = PAGE_ALIGN(addr);

		vma = find_vma_prev(mm, addr, &prev);
		if (TASK_SIZE - len >= addr &&
		    (!vma || addr + len <= vm_start_gap(vma)) &&
		    (!prev || addr >= vm_end_gap(prev)))
			return addr;
	}

	info.length = len;
	info.align_mask = do_color_align ? (PAGE_MASK & (SHM_COLOUR - 1)) : 0;
	info.align_offset = shared_align_offset(filp_pgoff, pgoff);

	if (dir == DOWN) {
		info.flags = VM_UNMAPPED_AREA_TOPDOWN;
		info.low_limit = PAGE_SIZE;
		info.high_limit = mm->mmap_base;
		addr = vm_unmapped_area(&info);
		if (!(addr & ~PAGE_MASK))
			return addr;
		VM_BUG_ON(addr != -ENOMEM);

		/*
		 * A failed mmap() very likely causes application failure,
		 * so fall back to the bottom-up function here. This scenario
		 * can happen with large stack limits and large mmap()
		 * allocations.
		 */
	}

	info.flags = 0;
	info.low_limit = mm->mmap_base;
	info.high_limit = mmap_upper_limit(NULL);
	return vm_unmapped_area(&info);
}

unsigned long arch_get_unmapped_area(struct file *filp, unsigned long addr,
	unsigned long len, unsigned long pgoff, unsigned long flags)
{
	return arch_get_unmapped_area_common(filp,
			addr, len, pgoff, flags, UP);
}

unsigned long arch_get_unmapped_area_topdown(struct file *filp,
	unsigned long addr, unsigned long len, unsigned long pgoff,
	unsigned long flags)
{
	return arch_get_unmapped_area_common(filp,
			addr, len, pgoff, flags, DOWN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

asmlinkage unsigned long sys_mmap2(unsigned long addr, unsigned long len,
	unsigned long prot, unsigned long flags, unsigned long fd,
	unsigned long pgoff)
{
	/* Make sure the shift for mmap2 is constant (12), no matter what PAGE_SIZE
	   we have. */
<<<<<<< HEAD
	return sys_mmap_pgoff(addr, len, prot, flags, fd,
			      pgoff >> (PAGE_SHIFT - 12));
=======
	return ksys_mmap_pgoff(addr, len, prot, flags, fd,
			       pgoff >> (PAGE_SHIFT - 12));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

asmlinkage unsigned long sys_mmap(unsigned long addr, unsigned long len,
		unsigned long prot, unsigned long flags, unsigned long fd,
		unsigned long offset)
{
	if (!(offset & ~PAGE_MASK)) {
<<<<<<< HEAD
		return sys_mmap_pgoff(addr, len, prot, flags, fd,
=======
		return ksys_mmap_pgoff(addr, len, prot, flags, fd,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					offset >> PAGE_SHIFT);
	} else {
		return -EINVAL;
	}
}

/* Fucking broken ABI */

#ifdef CONFIG_64BIT
asmlinkage long parisc_truncate64(const char __user * path,
					unsigned int high, unsigned int low)
{
<<<<<<< HEAD
	return sys_truncate(path, (long)high << 32 | low);
=======
	return ksys_truncate(path, (long)high << 32 | low);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

asmlinkage long parisc_ftruncate64(unsigned int fd,
					unsigned int high, unsigned int low)
{
<<<<<<< HEAD
	return sys_ftruncate(fd, (long)high << 32 | low);
=======
	return ksys_ftruncate(fd, (long)high << 32 | low);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* stubs for the benefit of the syscall_table since truncate64 and truncate 
 * are identical on LP64 */
asmlinkage long sys_truncate64(const char __user * path, unsigned long length)
{
<<<<<<< HEAD
	return sys_truncate(path, length);
}
asmlinkage long sys_ftruncate64(unsigned int fd, unsigned long length)
{
	return sys_ftruncate(fd, length);
=======
	return ksys_truncate(path, length);
}
asmlinkage long sys_ftruncate64(unsigned int fd, unsigned long length)
{
	return ksys_ftruncate(fd, length);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
asmlinkage long sys_fcntl64(unsigned int fd, unsigned int cmd, unsigned long arg)
{
	return sys_fcntl(fd, cmd, arg);
}
#else

asmlinkage long parisc_truncate64(const char __user * path,
					unsigned int high, unsigned int low)
{
<<<<<<< HEAD
	return sys_truncate64(path, (loff_t)high << 32 | low);
=======
	return ksys_truncate(path, (loff_t)high << 32 | low);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

asmlinkage long parisc_ftruncate64(unsigned int fd,
					unsigned int high, unsigned int low)
{
	return sys_ftruncate64(fd, (loff_t)high << 32 | low);
}
#endif

asmlinkage ssize_t parisc_pread64(unsigned int fd, char __user *buf, size_t count,
					unsigned int high, unsigned int low)
{
<<<<<<< HEAD
	return sys_pread64(fd, buf, count, (loff_t)high << 32 | low);
=======
	return ksys_pread64(fd, buf, count, (loff_t)high << 32 | low);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

asmlinkage ssize_t parisc_pwrite64(unsigned int fd, const char __user *buf,
			size_t count, unsigned int high, unsigned int low)
{
<<<<<<< HEAD
	return sys_pwrite64(fd, buf, count, (loff_t)high << 32 | low);
=======
	return ksys_pwrite64(fd, buf, count, (loff_t)high << 32 | low);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

asmlinkage ssize_t parisc_readahead(int fd, unsigned int high, unsigned int low,
		                    size_t count)
{
<<<<<<< HEAD
	return sys_readahead(fd, (loff_t)high << 32 | low, count);
=======
	return ksys_readahead(fd, (loff_t)high << 32 | low, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

asmlinkage long parisc_fadvise64_64(int fd,
			unsigned int high_off, unsigned int low_off,
			unsigned int high_len, unsigned int low_len, int advice)
{
<<<<<<< HEAD
	return sys_fadvise64_64(fd, (loff_t)high_off << 32 | low_off,
=======
	return ksys_fadvise64_64(fd, (loff_t)high_off << 32 | low_off,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			(loff_t)high_len << 32 | low_len, advice);
}

asmlinkage long parisc_sync_file_range(int fd,
			u32 hi_off, u32 lo_off, u32 hi_nbytes, u32 lo_nbytes,
			unsigned int flags)
{
<<<<<<< HEAD
	return sys_sync_file_range(fd, (loff_t)hi_off << 32 | lo_off,
			(loff_t)hi_nbytes << 32 | lo_nbytes, flags);
}

asmlinkage unsigned long sys_alloc_hugepages(int key, unsigned long addr, unsigned long len, int prot, int flag)
{
	return -ENOMEM;
}

asmlinkage int sys_free_hugepages(unsigned long addr)
{
	return -EINVAL;
}

long parisc_personality(unsigned long personality)
=======
	return ksys_sync_file_range(fd, (loff_t)hi_off << 32 | lo_off,
			(loff_t)hi_nbytes << 32 | lo_nbytes, flags);
}

asmlinkage long parisc_fallocate(int fd, int mode, u32 offhi, u32 offlo,
				u32 lenhi, u32 lenlo)
{
	return ksys_fallocate(fd, mode, ((u64)offhi << 32) | offlo,
			      ((u64)lenhi << 32) | lenlo);
}

asmlinkage long parisc_personality(unsigned long personality)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	long err;

	if (personality(current->personality) == PER_LINUX32
<<<<<<< HEAD
	    && personality == PER_LINUX)
		personality = PER_LINUX32;

	err = sys_personality(personality);
	if (err == PER_LINUX32)
		err = PER_LINUX;

	return err;
}
=======
	    && personality(personality) == PER_LINUX)
		personality = (personality & ~PER_MASK) | PER_LINUX32;

	err = sys_personality(personality);
	if (personality(err) == PER_LINUX32)
		err = (err & ~PER_MASK) | PER_LINUX;

	return err;
}

/*
 * Up to kernel v5.9 we defined O_NONBLOCK as 000200004,
 * since then O_NONBLOCK is defined as 000200000.
 *
 * The following wrapper functions mask out the old
 * O_NDELAY bit from calls which use O_NONBLOCK.
 *
 * XXX: Remove those in year 2022 (or later)?
 */

#define O_NONBLOCK_OLD		000200004
#define O_NONBLOCK_MASK_OUT	(O_NONBLOCK_OLD & ~O_NONBLOCK)

static int FIX_O_NONBLOCK(int flags)
{
	if ((flags & O_NONBLOCK_MASK_OUT) &&
			!test_thread_flag(TIF_NONBLOCK_WARNING)) {
		set_thread_flag(TIF_NONBLOCK_WARNING);
		pr_warn("%s(%d) uses a deprecated O_NONBLOCK value."
			" Please recompile with newer glibc.\n",
			current->comm, current->pid);
	}
	return flags & ~O_NONBLOCK_MASK_OUT;
}

asmlinkage long parisc_timerfd_create(int clockid, int flags)
{
	flags = FIX_O_NONBLOCK(flags);
	return sys_timerfd_create(clockid, flags);
}

asmlinkage long parisc_signalfd4(int ufd, sigset_t __user *user_mask,
	size_t sizemask, int flags)
{
	flags = FIX_O_NONBLOCK(flags);
	return sys_signalfd4(ufd, user_mask, sizemask, flags);
}

#ifdef CONFIG_COMPAT
asmlinkage long parisc_compat_signalfd4(int ufd,
	compat_sigset_t __user *user_mask,
	compat_size_t sizemask, int flags)
{
	flags = FIX_O_NONBLOCK(flags);
	return compat_sys_signalfd4(ufd, user_mask, sizemask, flags);
}
#endif

asmlinkage long parisc_eventfd2(unsigned int count, int flags)
{
	flags = FIX_O_NONBLOCK(flags);
	return sys_eventfd2(count, flags);
}

asmlinkage long parisc_userfaultfd(int flags)
{
	flags = FIX_O_NONBLOCK(flags);
	return sys_userfaultfd(flags);
}

asmlinkage long parisc_pipe2(int __user *fildes, int flags)
{
	flags = FIX_O_NONBLOCK(flags);
	return sys_pipe2(fildes, flags);
}

asmlinkage long parisc_inotify_init1(int flags)
{
	flags = FIX_O_NONBLOCK(flags);
	return sys_inotify_init1(flags);
}

/*
 * madvise() wrapper
 *
 * Up to kernel v6.1 parisc has different values than all other
 * platforms for the MADV_xxx flags listed below.
 * To keep binary compatibility with existing userspace programs
 * translate the former values to the new values.
 *
 * XXX: Remove this wrapper in year 2025 (or later)
 */

asmlinkage notrace long parisc_madvise(unsigned long start, size_t len_in, int behavior)
{
	switch (behavior) {
	case 65: behavior = MADV_MERGEABLE;	break;
	case 66: behavior = MADV_UNMERGEABLE;	break;
	case 67: behavior = MADV_HUGEPAGE;	break;
	case 68: behavior = MADV_NOHUGEPAGE;	break;
	case 69: behavior = MADV_DONTDUMP;	break;
	case 70: behavior = MADV_DODUMP;	break;
	case 71: behavior = MADV_WIPEONFORK;	break;
	case 72: behavior = MADV_KEEPONFORK;	break;
	case 73: behavior = MADV_COLLAPSE;	break;
	}

	return sys_madvise(start, len_in, behavior);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
