<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * System call table for UML/i386, copied from arch/x86/kernel/syscall_*.c
 * with some changes for UML.
 */

#include <linux/linkage.h>
#include <linux/sys.h>
#include <linux/cache.h>
<<<<<<< HEAD
#include <generated/user_constants.h>

#define __NO_STUBS
=======
#include <asm/syscall.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Below you can see, in terms of #define's, the differences between the x86-64
 * and the UML syscall table.
 */

/* Not going to be implemented by UML, since we have no hardware. */
#define sys_iopl sys_ni_syscall
#define sys_ioperm sys_ni_syscall

#define sys_vm86old sys_ni_syscall
#define sys_vm86 sys_ni_syscall

<<<<<<< HEAD
#define old_mmap sys_old_mmap

#define ptregs_fork sys_fork
#define ptregs_execve sys_execve
#define ptregs_iopl sys_iopl
#define ptregs_vm86old sys_vm86old
#define ptregs_clone sys_clone
#define ptregs_vm86 sys_vm86
#define ptregs_sigaltstack sys_sigaltstack
#define ptregs_vfork sys_vfork

#define __SYSCALL_I386(nr, sym, compat) extern asmlinkage void sym(void) ;
#include <asm/syscalls_32.h>

#undef __SYSCALL_I386
#define __SYSCALL_I386(nr, sym, compat) [ nr ] = sym,

typedef asmlinkage void (*sys_call_ptr_t)(void);

extern asmlinkage void sys_ni_syscall(void);

const sys_call_ptr_t sys_call_table[] ____cacheline_aligned = {
	/*
	 * Smells like a compiler bug -- it doesn't work
	 * when the & below is removed.
	 */
	[0 ... __NR_syscall_max] = &sys_ni_syscall,
=======
#define __SYSCALL_WITH_COMPAT(nr, native, compat)	__SYSCALL(nr, native)

#define __SYSCALL(nr, sym) extern asmlinkage long sym(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
#include <asm/syscalls_32.h>

#undef __SYSCALL
#define __SYSCALL(nr, sym) sym,

extern asmlinkage long sys_ni_syscall(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);

const sys_call_ptr_t sys_call_table[] ____cacheline_aligned = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/syscalls_32.h>
};

int syscall_table_size = sizeof(sys_call_table);
