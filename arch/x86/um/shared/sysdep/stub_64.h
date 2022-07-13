/*
 * Copyright (C) 2004 Jeff Dike (jdike@addtoit.com)
 * Licensed under the GPL
 */

#ifndef __SYSDEP_STUB_H
#define __SYSDEP_STUB_H

#include <sysdep/ptrace_user.h>
<<<<<<< HEAD

#define STUB_SYSCALL_RET PT_INDEX(RAX)
=======
#include <generated/asm-offsets.h>
#include <linux/stddef.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define STUB_MMAP_NR __NR_mmap
#define MMAP_OFFSET(o) (o)

#define __syscall_clobber "r11","rcx","memory"
#define __syscall "syscall"

<<<<<<< HEAD
static inline long stub_syscall0(long syscall)
=======
static __always_inline long stub_syscall0(long syscall)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	long ret;

	__asm__ volatile (__syscall
		: "=a" (ret)
		: "0" (syscall) : __syscall_clobber );

	return ret;
}

<<<<<<< HEAD
static inline long stub_syscall2(long syscall, long arg1, long arg2)
=======
static __always_inline long stub_syscall2(long syscall, long arg1, long arg2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	long ret;

	__asm__ volatile (__syscall
		: "=a" (ret)
		: "0" (syscall), "D" (arg1), "S" (arg2) : __syscall_clobber );

	return ret;
}

<<<<<<< HEAD
static inline long stub_syscall3(long syscall, long arg1, long arg2, long arg3)
=======
static __always_inline long stub_syscall3(long syscall, long arg1, long arg2,
					  long arg3)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	long ret;

	__asm__ volatile (__syscall
		: "=a" (ret)
		: "0" (syscall), "D" (arg1), "S" (arg2), "d" (arg3)
		: __syscall_clobber );

	return ret;
}

<<<<<<< HEAD
static inline long stub_syscall4(long syscall, long arg1, long arg2, long arg3,
=======
static __always_inline long stub_syscall4(long syscall, long arg1, long arg2, long arg3,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 long arg4)
{
	long ret;

	__asm__ volatile ("movq %5,%%r10 ; " __syscall
		: "=a" (ret)
		: "0" (syscall), "D" (arg1), "S" (arg2), "d" (arg3),
		  "g" (arg4)
		: __syscall_clobber, "r10" );

	return ret;
}

<<<<<<< HEAD
static inline long stub_syscall5(long syscall, long arg1, long arg2, long arg3,
				 long arg4, long arg5)
=======
static __always_inline long stub_syscall5(long syscall, long arg1, long arg2,
					  long arg3, long arg4, long arg5)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	long ret;

	__asm__ volatile ("movq %5,%%r10 ; movq %6,%%r8 ; " __syscall
		: "=a" (ret)
		: "0" (syscall), "D" (arg1), "S" (arg2), "d" (arg3),
		  "g" (arg4), "g" (arg5)
		: __syscall_clobber, "r10", "r8" );

	return ret;
}

<<<<<<< HEAD
static inline void trap_myself(void)
=======
static __always_inline void trap_myself(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__asm("int3");
}

<<<<<<< HEAD
static inline void remap_stack(long fd, unsigned long offset)
{
	__asm__ volatile ("movq %4,%%r10 ; movq %5,%%r8 ; "
			  "movq %6, %%r9; " __syscall "; movq %7, %%rbx ; "
			  "movq %%rax, (%%rbx)":
			  : "a" (STUB_MMAP_NR), "D" (STUB_DATA),
			    "S" (UM_KERN_PAGE_SIZE),
			    "d" (PROT_READ | PROT_WRITE),
                            "g" (MAP_FIXED | MAP_SHARED), "g" (fd),
			    "g" (offset),
			    "i" (&((struct stub_data *) STUB_DATA)->err)
			  : __syscall_clobber, "r10", "r8", "r9" );
}

=======
static __always_inline void remap_stack_and_trap(void)
{
	__asm__ volatile (
		"movq %0,%%rax ;"
		"movq %%rsp,%%rdi ;"
		"andq %1,%%rdi ;"
		"movq %2,%%r10 ;"
		"movq %%rdi,%%r8 ; addq %3,%%r8 ; movq (%%r8),%%r8 ;"
		"movq %%rdi,%%r9 ; addq %4,%%r9 ; movq (%%r9),%%r9 ;"
		__syscall ";"
		"movq %%rsp,%%rdi ; andq %1,%%rdi ;"
		"addq %5,%%rdi ; movq %%rax, (%%rdi) ;"
		"int3"
		: :
		"g" (STUB_MMAP_NR),
		"g" (~(STUB_DATA_PAGES * UM_KERN_PAGE_SIZE - 1)),
		"g" (MAP_FIXED | MAP_SHARED),
		"g" (UML_STUB_FIELD_FD),
		"g" (UML_STUB_FIELD_OFFSET),
		"g" (UML_STUB_FIELD_CHILD_ERR),
		"S" (STUB_DATA_PAGES * UM_KERN_PAGE_SIZE),
		"d" (PROT_READ | PROT_WRITE)
		:
		__syscall_clobber, "r10", "r8", "r9");
}

static __always_inline void *get_stub_data(void)
{
	unsigned long ret;

	asm volatile (
		"movq %%rsp,%0 ;"
		"andq %1,%0"
		: "=a" (ret)
		: "g" (~(STUB_DATA_PAGES * UM_KERN_PAGE_SIZE - 1)));

	return (void *)ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
