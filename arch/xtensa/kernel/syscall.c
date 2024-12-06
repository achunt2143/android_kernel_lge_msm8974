/*
 * arch/xtensa/kernel/syscall.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 * Copyright (C) 2000 Silicon Graphics, Inc.
 * Copyright (C) 1995 - 2000 by Ralf Baechle
 *
 * Joe Taylor <joe@tensilica.com, joetylr@yahoo.com>
 * Marc Gauthier <marc@tensilica.com, marc@alumni.uwaterloo.ca>
 * Chris Zankel <chris@zankel.net>
 * Kevin Chea
 *
 */
<<<<<<< HEAD
#include <asm/uaccess.h>
#include <asm/syscall.h>
#include <asm/unistd.h>
=======
#include <linux/uaccess.h>
#include <asm/syscall.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/linkage.h>
#include <linux/stringify.h>
#include <linux/errno.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/mman.h>
<<<<<<< HEAD
#include <linux/shm.h>

typedef void (*syscall_t)(void);

syscall_t sys_call_table[__NR_syscall_count] /* FIXME __cacheline_aligned */= {
	[0 ... __NR_syscall_count - 1] = (syscall_t)&sys_ni_syscall,

#undef __SYSCALL
#define __SYSCALL(nr,symbol,nargs) [ nr ] = (syscall_t)symbol,
#undef _XTENSA_UNISTD_H
#undef  __KERNEL_SYSCALLS__
#include <asm/unistd.h>
};

=======
#include <linux/sched/mm.h>
#include <linux/shm.h>

syscall_t sys_call_table[] /* FIXME __cacheline_aligned */= {
#define __SYSCALL(nr, entry)	(syscall_t)entry,
#include <asm/syscall_table.h>
};

#define COLOUR_ALIGN(addr, pgoff) \
	((((addr) + SHMLBA - 1) & ~(SHMLBA - 1)) + \
	 (((pgoff) << PAGE_SHIFT) & (SHMLBA - 1)))

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
asmlinkage long xtensa_shmat(int shmid, char __user *shmaddr, int shmflg)
{
	unsigned long ret;
	long err;

<<<<<<< HEAD
	err = do_shmat(shmid, shmaddr, shmflg, &ret);
=======
	err = do_shmat(shmid, shmaddr, shmflg, &ret, SHMLBA);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return err;
	return (long)ret;
}

<<<<<<< HEAD
asmlinkage long xtensa_fadvise64_64(int fd, int advice, unsigned long long offset, unsigned long long len)
{
	return sys_fadvise64_64(fd, offset, len, advice);
}

=======
asmlinkage long xtensa_fadvise64_64(int fd, int advice,
		unsigned long long offset, unsigned long long len)
{
	return ksys_fadvise64_64(fd, offset, len, advice);
}

#ifdef CONFIG_MMU
unsigned long arch_get_unmapped_area(struct file *filp, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct vm_area_struct *vmm;
	struct vma_iterator vmi;

	if (flags & MAP_FIXED) {
		/* We do not accept a shared mapping if it would violate
		 * cache aliasing constraints.
		 */
		if ((flags & MAP_SHARED) &&
				((addr - (pgoff << PAGE_SHIFT)) & (SHMLBA - 1)))
			return -EINVAL;
		return addr;
	}

	if (len > TASK_SIZE)
		return -ENOMEM;
	if (!addr)
		addr = TASK_UNMAPPED_BASE;

	if (flags & MAP_SHARED)
		addr = COLOUR_ALIGN(addr, pgoff);
	else
		addr = PAGE_ALIGN(addr);

	vma_iter_init(&vmi, current->mm, addr);
	for_each_vma(vmi, vmm) {
		/* At this point:  (addr < vmm->vm_end). */
		if (addr + len <= vm_start_gap(vmm))
			break;

		addr = vmm->vm_end;
		if (flags & MAP_SHARED)
			addr = COLOUR_ALIGN(addr, pgoff);
	}

	if (TASK_SIZE - len < addr)
		return -ENOMEM;

	return addr;
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
