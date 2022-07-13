/*
 * Copyright (C) 2004 Jeff Dike (jdike@addtoit.com)
 * Licensed under the GPL
 */

#ifndef __SYSDEP_STUB_H
#define __SYSDEP_STUB_H

#include <asm/ptrace.h>
<<<<<<< HEAD

#define STUB_SYSCALL_RET EAX
#define STUB_MMAP_NR __NR_mmap2
#define MMAP_OFFSET(o) ((o) >> UM_KERN_PAGE_SHIFT)

static inline long stub_syscall0(long syscall)
{
	long ret;

	__asm__ volatile ("int $0x80" : "=a" (ret) : "0" (syscall));
=======
#include <generated/asm-offsets.h>

#define STUB_MMAP_NR __NR_mmap2
#define MMAP_OFFSET(o) ((o) >> UM_KERN_PAGE_SHIFT)

static __always_inline long stub_syscall0(long syscall)
{
	long ret;

	__asm__ volatile ("int $0x80" : "=a" (ret) : "0" (syscall)
			: "memory");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static inline long stub_syscall1(long syscall, long arg1)
{
	long ret;

	__asm__ volatile ("int $0x80" : "=a" (ret) : "0" (syscall), "b" (arg1));
=======
static __always_inline long stub_syscall1(long syscall, long arg1)
{
	long ret;

	__asm__ volatile ("int $0x80" : "=a" (ret) : "0" (syscall), "b" (arg1)
			: "memory");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static inline long stub_syscall2(long syscall, long arg1, long arg2)
=======
static __always_inline long stub_syscall2(long syscall, long arg1, long arg2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	long ret;

	__asm__ volatile ("int $0x80" : "=a" (ret) : "0" (syscall), "b" (arg1),
<<<<<<< HEAD
			"c" (arg2));
=======
			"c" (arg2)
			: "memory");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

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

	__asm__ volatile ("int $0x80" : "=a" (ret) : "0" (syscall), "b" (arg1),
<<<<<<< HEAD
			"c" (arg2), "d" (arg3));
=======
			"c" (arg2), "d" (arg3)
			: "memory");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static inline long stub_syscall4(long syscall, long arg1, long arg2, long arg3,
				 long arg4)
=======
static __always_inline long stub_syscall4(long syscall, long arg1, long arg2,
					  long arg3, long arg4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	long ret;

	__asm__ volatile ("int $0x80" : "=a" (ret) : "0" (syscall), "b" (arg1),
<<<<<<< HEAD
			"c" (arg2), "d" (arg3), "S" (arg4));
=======
			"c" (arg2), "d" (arg3), "S" (arg4)
			: "memory");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

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

	__asm__ volatile ("int $0x80" : "=a" (ret) : "0" (syscall), "b" (arg1),
<<<<<<< HEAD
			"c" (arg2), "d" (arg3), "S" (arg4), "D" (arg5));
=======
			"c" (arg2), "d" (arg3), "S" (arg4), "D" (arg5)
			: "memory");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

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
static inline void remap_stack(int fd, unsigned long offset)
{
	__asm__ volatile ("movl %%eax,%%ebp ; movl %0,%%eax ; int $0x80 ;"
			  "movl %7, %%ebx ; movl %%eax, (%%ebx)"
			  : : "g" (STUB_MMAP_NR), "b" (STUB_DATA),
			    "c" (UM_KERN_PAGE_SIZE),
			    "d" (PROT_READ | PROT_WRITE),
			    "S" (MAP_FIXED | MAP_SHARED), "D" (fd),
			    "a" (offset),
			    "i" (&((struct stub_data *) STUB_DATA)->err)
			  : "memory");
}

=======
static __always_inline void remap_stack_and_trap(void)
{
	__asm__ volatile (
		"movl %%esp,%%ebx ;"
		"andl %0,%%ebx ;"
		"movl %1,%%eax ;"
		"movl %%ebx,%%edi ; addl %2,%%edi ; movl (%%edi),%%edi ;"
		"movl %%ebx,%%ebp ; addl %3,%%ebp ; movl (%%ebp),%%ebp ;"
		"int $0x80 ;"
		"addl %4,%%ebx ; movl %%eax, (%%ebx) ;"
		"int $3"
		: :
		"g" (~(STUB_DATA_PAGES * UM_KERN_PAGE_SIZE - 1)),
		"g" (STUB_MMAP_NR),
		"g" (UML_STUB_FIELD_FD),
		"g" (UML_STUB_FIELD_OFFSET),
		"g" (UML_STUB_FIELD_CHILD_ERR),
		"c" (STUB_DATA_PAGES * UM_KERN_PAGE_SIZE),
		"d" (PROT_READ | PROT_WRITE),
		"S" (MAP_FIXED | MAP_SHARED)
		:
		"memory");
}

static __always_inline void *get_stub_data(void)
{
	unsigned long ret;

	asm volatile (
		"movl %%esp,%0 ;"
		"andl %1,%0"
		: "=a" (ret)
		: "g" (~(STUB_DATA_PAGES * UM_KERN_PAGE_SIZE - 1)));

	return (void *)ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
