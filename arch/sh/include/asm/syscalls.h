<<<<<<< HEAD
#ifndef __ASM_SH_SYSCALLS_H
#define __ASM_SH_SYSCALLS_H

#ifdef __KERNEL__

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_SYSCALLS_H
#define __ASM_SH_SYSCALLS_H

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
asmlinkage int old_mmap(unsigned long addr, unsigned long len,
			unsigned long prot, unsigned long flags,
			int fd, unsigned long off);
asmlinkage long sys_mmap2(unsigned long addr, unsigned long len,
			  unsigned long prot, unsigned long flags,
			  unsigned long fd, unsigned long pgoff);

<<<<<<< HEAD
#ifdef CONFIG_SUPERH32
# include "syscalls_32.h"
#else
# include "syscalls_64.h"
#endif

#endif /* __KERNEL__ */
=======
#include <asm/syscalls_32.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASM_SH_SYSCALLS_H */
