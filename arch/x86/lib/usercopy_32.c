<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * User address space access functions.
 * The non inlined parts of asm-i386/uaccess.h are here.
 *
 * Copyright 1997 Andi Kleen <ak@muc.de>
 * Copyright 1997 Linus Torvalds
 */
<<<<<<< HEAD
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/blkdev.h>
#include <linux/module.h>
#include <linux/backing-dev.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <asm/mmx.h>
=======
#include <linux/export.h>
#include <linux/uaccess.h>
#include <asm/asm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_X86_INTEL_USERCOPY
/*
 * Alignment at which movsl is preferred for bulk memory copies.
 */
struct movsl_mask movsl_mask __read_mostly;
#endif

static inline int __movsl_is_ok(unsigned long a1, unsigned long a2, unsigned long n)
{
#ifdef CONFIG_X86_INTEL_USERCOPY
	if (n >= 64 && ((a1 ^ a2) & movsl_mask.mask))
		return 0;
#endif
	return 1;
}
#define movsl_is_ok(a1, a2, n) \
	__movsl_is_ok((unsigned long)(a1), (unsigned long)(a2), (n))

/*
 * Zero Userspace
 */

#define __do_clear_user(addr,size)					\
do {									\
	int __d0;							\
	might_fault();							\
	__asm__ __volatile__(						\
<<<<<<< HEAD
		"0:	rep; stosl\n"					\
		"	movl %2,%0\n"					\
		"1:	rep; stosb\n"					\
		"2:\n"							\
		".section .fixup,\"ax\"\n"				\
		"3:	lea 0(%2,%0,4),%0\n"				\
		"	jmp 2b\n"					\
		".previous\n"						\
		_ASM_EXTABLE(0b,3b)					\
		_ASM_EXTABLE(1b,2b)					\
=======
		ASM_STAC "\n"						\
		"0:	rep; stosl\n"					\
		"	movl %2,%0\n"					\
		"1:	rep; stosb\n"					\
		"2: " ASM_CLAC "\n"					\
		_ASM_EXTABLE_TYPE_REG(0b, 2b, EX_TYPE_UCOPY_LEN4, %2)	\
		_ASM_EXTABLE_UA(1b, 2b)					\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		: "=&c"(size), "=&D" (__d0)				\
		: "r"(size & 3), "0"(size / 4), "1"(addr), "a"(0));	\
} while (0)

/**
<<<<<<< HEAD
 * clear_user: - Zero a block of memory in user space.
=======
 * clear_user - Zero a block of memory in user space.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @to:   Destination address, in user space.
 * @n:    Number of bytes to zero.
 *
 * Zero a block of memory in user space.
 *
<<<<<<< HEAD
 * Returns number of bytes that could not be cleared.
=======
 * Return: number of bytes that could not be cleared.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * On success, this will be zero.
 */
unsigned long
clear_user(void __user *to, unsigned long n)
{
	might_fault();
<<<<<<< HEAD
	if (access_ok(VERIFY_WRITE, to, n))
=======
	if (access_ok(to, n))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__do_clear_user(to, n);
	return n;
}
EXPORT_SYMBOL(clear_user);

/**
<<<<<<< HEAD
 * __clear_user: - Zero a block of memory in user space, with less checking.
=======
 * __clear_user - Zero a block of memory in user space, with less checking.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @to:   Destination address, in user space.
 * @n:    Number of bytes to zero.
 *
 * Zero a block of memory in user space.  Caller must check
 * the specified block with access_ok() before calling this function.
 *
<<<<<<< HEAD
 * Returns number of bytes that could not be cleared.
=======
 * Return: number of bytes that could not be cleared.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * On success, this will be zero.
 */
unsigned long
__clear_user(void __user *to, unsigned long n)
{
	__do_clear_user(to, n);
	return n;
}
EXPORT_SYMBOL(__clear_user);

<<<<<<< HEAD
/**
 * strnlen_user: - Get the size of a string in user space.
 * @s: The string to measure.
 * @n: The maximum valid length
 *
 * Get the size of a NUL-terminated string in user space.
 *
 * Returns the size of the string INCLUDING the terminating NUL.
 * On exception, returns 0.
 * If the string is too long, returns a value greater than @n.
 */
long strnlen_user(const char __user *s, long n)
{
	unsigned long mask = -__addr_ok(s);
	unsigned long res, tmp;

	might_fault();

	__asm__ __volatile__(
		"	testl %0, %0\n"
		"	jz 3f\n"
		"	andl %0,%%ecx\n"
		"0:	repne; scasb\n"
		"	setne %%al\n"
		"	subl %%ecx,%0\n"
		"	addl %0,%%eax\n"
		"1:\n"
		".section .fixup,\"ax\"\n"
		"2:	xorl %%eax,%%eax\n"
		"	jmp 1b\n"
		"3:	movb $1,%%al\n"
		"	jmp 1b\n"
		".previous\n"
		".section __ex_table,\"a\"\n"
		"	.align 4\n"
		"	.long 0b,2b\n"
		".previous"
		:"=&r" (n), "=&D" (s), "=&a" (res), "=&c" (tmp)
		:"0" (n), "1" (s), "2" (0), "3" (mask)
		:"cc");
	return res & mask;
}
EXPORT_SYMBOL(strnlen_user);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_X86_INTEL_USERCOPY
static unsigned long
__copy_user_intel(void __user *to, const void *from, unsigned long size)
{
	int d0, d1;
	__asm__ __volatile__(
		       "       .align 2,0x90\n"
		       "1:     movl 32(%4), %%eax\n"
		       "       cmpl $67, %0\n"
		       "       jbe 3f\n"
		       "2:     movl 64(%4), %%eax\n"
		       "       .align 2,0x90\n"
		       "3:     movl 0(%4), %%eax\n"
		       "4:     movl 4(%4), %%edx\n"
		       "5:     movl %%eax, 0(%3)\n"
		       "6:     movl %%edx, 4(%3)\n"
		       "7:     movl 8(%4), %%eax\n"
		       "8:     movl 12(%4),%%edx\n"
		       "9:     movl %%eax, 8(%3)\n"
		       "10:    movl %%edx, 12(%3)\n"
		       "11:    movl 16(%4), %%eax\n"
		       "12:    movl 20(%4), %%edx\n"
		       "13:    movl %%eax, 16(%3)\n"
		       "14:    movl %%edx, 20(%3)\n"
		       "15:    movl 24(%4), %%eax\n"
		       "16:    movl 28(%4), %%edx\n"
		       "17:    movl %%eax, 24(%3)\n"
		       "18:    movl %%edx, 28(%3)\n"
		       "19:    movl 32(%4), %%eax\n"
		       "20:    movl 36(%4), %%edx\n"
		       "21:    movl %%eax, 32(%3)\n"
		       "22:    movl %%edx, 36(%3)\n"
		       "23:    movl 40(%4), %%eax\n"
		       "24:    movl 44(%4), %%edx\n"
		       "25:    movl %%eax, 40(%3)\n"
		       "26:    movl %%edx, 44(%3)\n"
		       "27:    movl 48(%4), %%eax\n"
		       "28:    movl 52(%4), %%edx\n"
		       "29:    movl %%eax, 48(%3)\n"
		       "30:    movl %%edx, 52(%3)\n"
		       "31:    movl 56(%4), %%eax\n"
		       "32:    movl 60(%4), %%edx\n"
		       "33:    movl %%eax, 56(%3)\n"
		       "34:    movl %%edx, 60(%3)\n"
		       "       addl $-64, %0\n"
		       "       addl $64, %4\n"
		       "       addl $64, %3\n"
		       "       cmpl $63, %0\n"
		       "       ja  1b\n"
		       "35:    movl  %0, %%eax\n"
		       "       shrl  $2, %0\n"
		       "       andl  $3, %%eax\n"
		       "       cld\n"
		       "99:    rep; movsl\n"
		       "36:    movl %%eax, %0\n"
		       "37:    rep; movsb\n"
		       "100:\n"
<<<<<<< HEAD
		       ".section .fixup,\"ax\"\n"
		       "101:   lea 0(%%eax,%0,4),%0\n"
		       "       jmp 100b\n"
		       ".previous\n"
		       ".section __ex_table,\"a\"\n"
		       "       .align 4\n"
		       "       .long 1b,100b\n"
		       "       .long 2b,100b\n"
		       "       .long 3b,100b\n"
		       "       .long 4b,100b\n"
		       "       .long 5b,100b\n"
		       "       .long 6b,100b\n"
		       "       .long 7b,100b\n"
		       "       .long 8b,100b\n"
		       "       .long 9b,100b\n"
		       "       .long 10b,100b\n"
		       "       .long 11b,100b\n"
		       "       .long 12b,100b\n"
		       "       .long 13b,100b\n"
		       "       .long 14b,100b\n"
		       "       .long 15b,100b\n"
		       "       .long 16b,100b\n"
		       "       .long 17b,100b\n"
		       "       .long 18b,100b\n"
		       "       .long 19b,100b\n"
		       "       .long 20b,100b\n"
		       "       .long 21b,100b\n"
		       "       .long 22b,100b\n"
		       "       .long 23b,100b\n"
		       "       .long 24b,100b\n"
		       "       .long 25b,100b\n"
		       "       .long 26b,100b\n"
		       "       .long 27b,100b\n"
		       "       .long 28b,100b\n"
		       "       .long 29b,100b\n"
		       "       .long 30b,100b\n"
		       "       .long 31b,100b\n"
		       "       .long 32b,100b\n"
		       "       .long 33b,100b\n"
		       "       .long 34b,100b\n"
		       "       .long 35b,100b\n"
		       "       .long 36b,100b\n"
		       "       .long 37b,100b\n"
		       "       .long 99b,101b\n"
		       ".previous"
=======
		       _ASM_EXTABLE_UA(1b, 100b)
		       _ASM_EXTABLE_UA(2b, 100b)
		       _ASM_EXTABLE_UA(3b, 100b)
		       _ASM_EXTABLE_UA(4b, 100b)
		       _ASM_EXTABLE_UA(5b, 100b)
		       _ASM_EXTABLE_UA(6b, 100b)
		       _ASM_EXTABLE_UA(7b, 100b)
		       _ASM_EXTABLE_UA(8b, 100b)
		       _ASM_EXTABLE_UA(9b, 100b)
		       _ASM_EXTABLE_UA(10b, 100b)
		       _ASM_EXTABLE_UA(11b, 100b)
		       _ASM_EXTABLE_UA(12b, 100b)
		       _ASM_EXTABLE_UA(13b, 100b)
		       _ASM_EXTABLE_UA(14b, 100b)
		       _ASM_EXTABLE_UA(15b, 100b)
		       _ASM_EXTABLE_UA(16b, 100b)
		       _ASM_EXTABLE_UA(17b, 100b)
		       _ASM_EXTABLE_UA(18b, 100b)
		       _ASM_EXTABLE_UA(19b, 100b)
		       _ASM_EXTABLE_UA(20b, 100b)
		       _ASM_EXTABLE_UA(21b, 100b)
		       _ASM_EXTABLE_UA(22b, 100b)
		       _ASM_EXTABLE_UA(23b, 100b)
		       _ASM_EXTABLE_UA(24b, 100b)
		       _ASM_EXTABLE_UA(25b, 100b)
		       _ASM_EXTABLE_UA(26b, 100b)
		       _ASM_EXTABLE_UA(27b, 100b)
		       _ASM_EXTABLE_UA(28b, 100b)
		       _ASM_EXTABLE_UA(29b, 100b)
		       _ASM_EXTABLE_UA(30b, 100b)
		       _ASM_EXTABLE_UA(31b, 100b)
		       _ASM_EXTABLE_UA(32b, 100b)
		       _ASM_EXTABLE_UA(33b, 100b)
		       _ASM_EXTABLE_UA(34b, 100b)
		       _ASM_EXTABLE_UA(35b, 100b)
		       _ASM_EXTABLE_UA(36b, 100b)
		       _ASM_EXTABLE_UA(37b, 100b)
		       _ASM_EXTABLE_TYPE_REG(99b, 100b, EX_TYPE_UCOPY_LEN4, %%eax)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       : "=&c"(size), "=&D" (d0), "=&S" (d1)
		       :  "1"(to), "2"(from), "0"(size)
		       : "eax", "edx", "memory");
	return size;
}

<<<<<<< HEAD
static unsigned long
__copy_user_zeroing_intel(void *to, const void __user *from, unsigned long size)
{
	int d0, d1;
	__asm__ __volatile__(
		       "        .align 2,0x90\n"
		       "0:      movl 32(%4), %%eax\n"
		       "        cmpl $67, %0\n"
		       "        jbe 2f\n"
		       "1:      movl 64(%4), %%eax\n"
		       "        .align 2,0x90\n"
		       "2:      movl 0(%4), %%eax\n"
		       "21:     movl 4(%4), %%edx\n"
		       "        movl %%eax, 0(%3)\n"
		       "        movl %%edx, 4(%3)\n"
		       "3:      movl 8(%4), %%eax\n"
		       "31:     movl 12(%4),%%edx\n"
		       "        movl %%eax, 8(%3)\n"
		       "        movl %%edx, 12(%3)\n"
		       "4:      movl 16(%4), %%eax\n"
		       "41:     movl 20(%4), %%edx\n"
		       "        movl %%eax, 16(%3)\n"
		       "        movl %%edx, 20(%3)\n"
		       "10:     movl 24(%4), %%eax\n"
		       "51:     movl 28(%4), %%edx\n"
		       "        movl %%eax, 24(%3)\n"
		       "        movl %%edx, 28(%3)\n"
		       "11:     movl 32(%4), %%eax\n"
		       "61:     movl 36(%4), %%edx\n"
		       "        movl %%eax, 32(%3)\n"
		       "        movl %%edx, 36(%3)\n"
		       "12:     movl 40(%4), %%eax\n"
		       "71:     movl 44(%4), %%edx\n"
		       "        movl %%eax, 40(%3)\n"
		       "        movl %%edx, 44(%3)\n"
		       "13:     movl 48(%4), %%eax\n"
		       "81:     movl 52(%4), %%edx\n"
		       "        movl %%eax, 48(%3)\n"
		       "        movl %%edx, 52(%3)\n"
		       "14:     movl 56(%4), %%eax\n"
		       "91:     movl 60(%4), %%edx\n"
		       "        movl %%eax, 56(%3)\n"
		       "        movl %%edx, 60(%3)\n"
		       "        addl $-64, %0\n"
		       "        addl $64, %4\n"
		       "        addl $64, %3\n"
		       "        cmpl $63, %0\n"
		       "        ja  0b\n"
		       "5:      movl  %0, %%eax\n"
		       "        shrl  $2, %0\n"
		       "        andl $3, %%eax\n"
		       "        cld\n"
		       "6:      rep; movsl\n"
		       "        movl %%eax,%0\n"
		       "7:      rep; movsb\n"
		       "8:\n"
		       ".section .fixup,\"ax\"\n"
		       "9:      lea 0(%%eax,%0,4),%0\n"
		       "16:     pushl %0\n"
		       "        pushl %%eax\n"
		       "        xorl %%eax,%%eax\n"
		       "        rep; stosb\n"
		       "        popl %%eax\n"
		       "        popl %0\n"
		       "        jmp 8b\n"
		       ".previous\n"
		       ".section __ex_table,\"a\"\n"
		       "	.align 4\n"
		       "	.long 0b,16b\n"
		       "	.long 1b,16b\n"
		       "	.long 2b,16b\n"
		       "	.long 21b,16b\n"
		       "	.long 3b,16b\n"
		       "	.long 31b,16b\n"
		       "	.long 4b,16b\n"
		       "	.long 41b,16b\n"
		       "	.long 10b,16b\n"
		       "	.long 51b,16b\n"
		       "	.long 11b,16b\n"
		       "	.long 61b,16b\n"
		       "	.long 12b,16b\n"
		       "	.long 71b,16b\n"
		       "	.long 13b,16b\n"
		       "	.long 81b,16b\n"
		       "	.long 14b,16b\n"
		       "	.long 91b,16b\n"
		       "	.long 6b,9b\n"
		       "        .long 7b,16b\n"
		       ".previous"
		       : "=&c"(size), "=&D" (d0), "=&S" (d1)
		       :  "1"(to), "2"(from), "0"(size)
		       : "eax", "edx", "memory");
	return size;
}

/*
 * Non Temporal Hint version of __copy_user_zeroing_intel.  It is cache aware.
 * hyoshiok@miraclelinux.com
 */

static unsigned long __copy_user_zeroing_intel_nocache(void *to,
				const void __user *from, unsigned long size)
{
	int d0, d1;

	__asm__ __volatile__(
	       "        .align 2,0x90\n"
	       "0:      movl 32(%4), %%eax\n"
	       "        cmpl $67, %0\n"
	       "        jbe 2f\n"
	       "1:      movl 64(%4), %%eax\n"
	       "        .align 2,0x90\n"
	       "2:      movl 0(%4), %%eax\n"
	       "21:     movl 4(%4), %%edx\n"
	       "        movnti %%eax, 0(%3)\n"
	       "        movnti %%edx, 4(%3)\n"
	       "3:      movl 8(%4), %%eax\n"
	       "31:     movl 12(%4),%%edx\n"
	       "        movnti %%eax, 8(%3)\n"
	       "        movnti %%edx, 12(%3)\n"
	       "4:      movl 16(%4), %%eax\n"
	       "41:     movl 20(%4), %%edx\n"
	       "        movnti %%eax, 16(%3)\n"
	       "        movnti %%edx, 20(%3)\n"
	       "10:     movl 24(%4), %%eax\n"
	       "51:     movl 28(%4), %%edx\n"
	       "        movnti %%eax, 24(%3)\n"
	       "        movnti %%edx, 28(%3)\n"
	       "11:     movl 32(%4), %%eax\n"
	       "61:     movl 36(%4), %%edx\n"
	       "        movnti %%eax, 32(%3)\n"
	       "        movnti %%edx, 36(%3)\n"
	       "12:     movl 40(%4), %%eax\n"
	       "71:     movl 44(%4), %%edx\n"
	       "        movnti %%eax, 40(%3)\n"
	       "        movnti %%edx, 44(%3)\n"
	       "13:     movl 48(%4), %%eax\n"
	       "81:     movl 52(%4), %%edx\n"
	       "        movnti %%eax, 48(%3)\n"
	       "        movnti %%edx, 52(%3)\n"
	       "14:     movl 56(%4), %%eax\n"
	       "91:     movl 60(%4), %%edx\n"
	       "        movnti %%eax, 56(%3)\n"
	       "        movnti %%edx, 60(%3)\n"
	       "        addl $-64, %0\n"
	       "        addl $64, %4\n"
	       "        addl $64, %3\n"
	       "        cmpl $63, %0\n"
	       "        ja  0b\n"
	       "        sfence \n"
	       "5:      movl  %0, %%eax\n"
	       "        shrl  $2, %0\n"
	       "        andl $3, %%eax\n"
	       "        cld\n"
	       "6:      rep; movsl\n"
	       "        movl %%eax,%0\n"
	       "7:      rep; movsb\n"
	       "8:\n"
	       ".section .fixup,\"ax\"\n"
	       "9:      lea 0(%%eax,%0,4),%0\n"
	       "16:     pushl %0\n"
	       "        pushl %%eax\n"
	       "        xorl %%eax,%%eax\n"
	       "        rep; stosb\n"
	       "        popl %%eax\n"
	       "        popl %0\n"
	       "        jmp 8b\n"
	       ".previous\n"
	       ".section __ex_table,\"a\"\n"
	       "	.align 4\n"
	       "	.long 0b,16b\n"
	       "	.long 1b,16b\n"
	       "	.long 2b,16b\n"
	       "	.long 21b,16b\n"
	       "	.long 3b,16b\n"
	       "	.long 31b,16b\n"
	       "	.long 4b,16b\n"
	       "	.long 41b,16b\n"
	       "	.long 10b,16b\n"
	       "	.long 51b,16b\n"
	       "	.long 11b,16b\n"
	       "	.long 61b,16b\n"
	       "	.long 12b,16b\n"
	       "	.long 71b,16b\n"
	       "	.long 13b,16b\n"
	       "	.long 81b,16b\n"
	       "	.long 14b,16b\n"
	       "	.long 91b,16b\n"
	       "	.long 6b,9b\n"
	       "        .long 7b,16b\n"
	       ".previous"
	       : "=&c"(size), "=&D" (d0), "=&S" (d1)
	       :  "1"(to), "2"(from), "0"(size)
	       : "eax", "edx", "memory");
	return size;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned long __copy_user_intel_nocache(void *to,
				const void __user *from, unsigned long size)
{
	int d0, d1;

	__asm__ __volatile__(
	       "        .align 2,0x90\n"
	       "0:      movl 32(%4), %%eax\n"
	       "        cmpl $67, %0\n"
	       "        jbe 2f\n"
	       "1:      movl 64(%4), %%eax\n"
	       "        .align 2,0x90\n"
	       "2:      movl 0(%4), %%eax\n"
	       "21:     movl 4(%4), %%edx\n"
	       "        movnti %%eax, 0(%3)\n"
	       "        movnti %%edx, 4(%3)\n"
	       "3:      movl 8(%4), %%eax\n"
	       "31:     movl 12(%4),%%edx\n"
	       "        movnti %%eax, 8(%3)\n"
	       "        movnti %%edx, 12(%3)\n"
	       "4:      movl 16(%4), %%eax\n"
	       "41:     movl 20(%4), %%edx\n"
	       "        movnti %%eax, 16(%3)\n"
	       "        movnti %%edx, 20(%3)\n"
	       "10:     movl 24(%4), %%eax\n"
	       "51:     movl 28(%4), %%edx\n"
	       "        movnti %%eax, 24(%3)\n"
	       "        movnti %%edx, 28(%3)\n"
	       "11:     movl 32(%4), %%eax\n"
	       "61:     movl 36(%4), %%edx\n"
	       "        movnti %%eax, 32(%3)\n"
	       "        movnti %%edx, 36(%3)\n"
	       "12:     movl 40(%4), %%eax\n"
	       "71:     movl 44(%4), %%edx\n"
	       "        movnti %%eax, 40(%3)\n"
	       "        movnti %%edx, 44(%3)\n"
	       "13:     movl 48(%4), %%eax\n"
	       "81:     movl 52(%4), %%edx\n"
	       "        movnti %%eax, 48(%3)\n"
	       "        movnti %%edx, 52(%3)\n"
	       "14:     movl 56(%4), %%eax\n"
	       "91:     movl 60(%4), %%edx\n"
	       "        movnti %%eax, 56(%3)\n"
	       "        movnti %%edx, 60(%3)\n"
	       "        addl $-64, %0\n"
	       "        addl $64, %4\n"
	       "        addl $64, %3\n"
	       "        cmpl $63, %0\n"
	       "        ja  0b\n"
	       "        sfence \n"
	       "5:      movl  %0, %%eax\n"
	       "        shrl  $2, %0\n"
	       "        andl $3, %%eax\n"
	       "        cld\n"
	       "6:      rep; movsl\n"
	       "        movl %%eax,%0\n"
	       "7:      rep; movsb\n"
	       "8:\n"
<<<<<<< HEAD
	       ".section .fixup,\"ax\"\n"
	       "9:      lea 0(%%eax,%0,4),%0\n"
	       "16:     jmp 8b\n"
	       ".previous\n"
	       ".section __ex_table,\"a\"\n"
	       "	.align 4\n"
	       "	.long 0b,16b\n"
	       "	.long 1b,16b\n"
	       "	.long 2b,16b\n"
	       "	.long 21b,16b\n"
	       "	.long 3b,16b\n"
	       "	.long 31b,16b\n"
	       "	.long 4b,16b\n"
	       "	.long 41b,16b\n"
	       "	.long 10b,16b\n"
	       "	.long 51b,16b\n"
	       "	.long 11b,16b\n"
	       "	.long 61b,16b\n"
	       "	.long 12b,16b\n"
	       "	.long 71b,16b\n"
	       "	.long 13b,16b\n"
	       "	.long 81b,16b\n"
	       "	.long 14b,16b\n"
	       "	.long 91b,16b\n"
	       "	.long 6b,9b\n"
	       "        .long 7b,16b\n"
	       ".previous"
=======
	       _ASM_EXTABLE_UA(0b, 8b)
	       _ASM_EXTABLE_UA(1b, 8b)
	       _ASM_EXTABLE_UA(2b, 8b)
	       _ASM_EXTABLE_UA(21b, 8b)
	       _ASM_EXTABLE_UA(3b, 8b)
	       _ASM_EXTABLE_UA(31b, 8b)
	       _ASM_EXTABLE_UA(4b, 8b)
	       _ASM_EXTABLE_UA(41b, 8b)
	       _ASM_EXTABLE_UA(10b, 8b)
	       _ASM_EXTABLE_UA(51b, 8b)
	       _ASM_EXTABLE_UA(11b, 8b)
	       _ASM_EXTABLE_UA(61b, 8b)
	       _ASM_EXTABLE_UA(12b, 8b)
	       _ASM_EXTABLE_UA(71b, 8b)
	       _ASM_EXTABLE_UA(13b, 8b)
	       _ASM_EXTABLE_UA(81b, 8b)
	       _ASM_EXTABLE_UA(14b, 8b)
	       _ASM_EXTABLE_UA(91b, 8b)
	       _ASM_EXTABLE_TYPE_REG(6b, 8b, EX_TYPE_UCOPY_LEN4, %%eax)
	       _ASM_EXTABLE_UA(7b, 8b)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       : "=&c"(size), "=&D" (d0), "=&S" (d1)
	       :  "1"(to), "2"(from), "0"(size)
	       : "eax", "edx", "memory");
	return size;
}

#else

/*
 * Leave these declared but undefined.  They should not be any references to
 * them
 */
<<<<<<< HEAD
unsigned long __copy_user_zeroing_intel(void *to, const void __user *from,
					unsigned long size);
unsigned long __copy_user_intel(void __user *to, const void *from,
					unsigned long size);
unsigned long __copy_user_zeroing_intel_nocache(void *to,
				const void __user *from, unsigned long size);
=======
unsigned long __copy_user_intel(void __user *to, const void *from,
					unsigned long size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_X86_INTEL_USERCOPY */

/* Generic arbitrary sized copy.  */
#define __copy_user(to, from, size)					\
do {									\
	int __d0, __d1, __d2;						\
	__asm__ __volatile__(						\
		"	cmp  $7,%0\n"					\
		"	jbe  1f\n"					\
		"	movl %1,%0\n"					\
		"	negl %0\n"					\
		"	andl $7,%0\n"					\
		"	subl %0,%3\n"					\
		"4:	rep; movsb\n"					\
		"	movl %3,%0\n"					\
		"	shrl $2,%0\n"					\
		"	andl $3,%3\n"					\
		"	.align 2,0x90\n"				\
		"0:	rep; movsl\n"					\
		"	movl %3,%0\n"					\
		"1:	rep; movsb\n"					\
		"2:\n"							\
<<<<<<< HEAD
		".section .fixup,\"ax\"\n"				\
		"5:	addl %3,%0\n"					\
		"	jmp 2b\n"					\
		"3:	lea 0(%3,%0,4),%0\n"				\
		"	jmp 2b\n"					\
		".previous\n"						\
		".section __ex_table,\"a\"\n"				\
		"	.align 4\n"					\
		"	.long 4b,5b\n"					\
		"	.long 0b,3b\n"					\
		"	.long 1b,2b\n"					\
		".previous"						\
=======
		_ASM_EXTABLE_TYPE_REG(4b, 2b, EX_TYPE_UCOPY_LEN1, %3)	\
		_ASM_EXTABLE_TYPE_REG(0b, 2b, EX_TYPE_UCOPY_LEN4, %3)	\
		_ASM_EXTABLE_UA(1b, 2b)					\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		: "=&c"(size), "=&D" (__d0), "=&S" (__d1), "=r"(__d2)	\
		: "3"(size), "0"(size), "1"(to), "2"(from)		\
		: "memory");						\
} while (0)

<<<<<<< HEAD
#define __copy_user_zeroing(to, from, size)				\
do {									\
	int __d0, __d1, __d2;						\
	__asm__ __volatile__(						\
		"	cmp  $7,%0\n"					\
		"	jbe  1f\n"					\
		"	movl %1,%0\n"					\
		"	negl %0\n"					\
		"	andl $7,%0\n"					\
		"	subl %0,%3\n"					\
		"4:	rep; movsb\n"					\
		"	movl %3,%0\n"					\
		"	shrl $2,%0\n"					\
		"	andl $3,%3\n"					\
		"	.align 2,0x90\n"				\
		"0:	rep; movsl\n"					\
		"	movl %3,%0\n"					\
		"1:	rep; movsb\n"					\
		"2:\n"							\
		".section .fixup,\"ax\"\n"				\
		"5:	addl %3,%0\n"					\
		"	jmp 6f\n"					\
		"3:	lea 0(%3,%0,4),%0\n"				\
		"6:	pushl %0\n"					\
		"	pushl %%eax\n"					\
		"	xorl %%eax,%%eax\n"				\
		"	rep; stosb\n"					\
		"	popl %%eax\n"					\
		"	popl %0\n"					\
		"	jmp 2b\n"					\
		".previous\n"						\
		".section __ex_table,\"a\"\n"				\
		"	.align 4\n"					\
		"	.long 4b,5b\n"					\
		"	.long 0b,3b\n"					\
		"	.long 1b,6b\n"					\
		".previous"						\
		: "=&c"(size), "=&D" (__d0), "=&S" (__d1), "=r"(__d2)	\
		: "3"(size), "0"(size), "1"(to), "2"(from)		\
		: "memory");						\
} while (0)

unsigned long __copy_to_user_ll(void __user *to, const void *from,
				unsigned long n)
{
#ifndef CONFIG_X86_WP_WORKS_OK
	if (unlikely(boot_cpu_data.wp_works_ok == 0) &&
			((unsigned long)to) < TASK_SIZE) {
		/*
		 * When we are in an atomic section (see
		 * mm/filemap.c:file_read_actor), return the full
		 * length to take the slow path.
		 */
		if (in_atomic())
			return n;

		/*
		 * CPU does not honor the WP bit when writing
		 * from supervisory mode, and due to preemption or SMP,
		 * the page tables can change at any time.
		 * Do it manually.	Manfred <manfred@colorfullife.com>
		 */
		while (n) {
			unsigned long offset = ((unsigned long)to)%PAGE_SIZE;
			unsigned long len = PAGE_SIZE - offset;
			int retval;
			struct page *pg;
			void *maddr;

			if (len > n)
				len = n;

survive:
			down_read(&current->mm->mmap_sem);
			retval = get_user_pages(current, current->mm,
					(unsigned long)to, 1, 1, 0, &pg, NULL);

			if (retval == -ENOMEM && is_global_init(current)) {
				up_read(&current->mm->mmap_sem);
				congestion_wait(BLK_RW_ASYNC, HZ/50);
				goto survive;
			}

			if (retval != 1) {
				up_read(&current->mm->mmap_sem);
				break;
			}

			maddr = kmap_atomic(pg);
			memcpy(maddr + offset, from, len);
			kunmap_atomic(maddr);
			set_page_dirty_lock(pg);
			put_page(pg);
			up_read(&current->mm->mmap_sem);

			from += len;
			to += len;
			n -= len;
		}
		return n;
	}
#endif
=======
unsigned long __copy_user_ll(void *to, const void *from, unsigned long n)
{
	__uaccess_begin_nospec();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (movsl_is_ok(to, from, n))
		__copy_user(to, from, n);
	else
		n = __copy_user_intel(to, from, n);
<<<<<<< HEAD
	return n;
}
EXPORT_SYMBOL(__copy_to_user_ll);

unsigned long __copy_from_user_ll(void *to, const void __user *from,
					unsigned long n)
{
	if (movsl_is_ok(to, from, n))
		__copy_user_zeroing(to, from, n);
	else
		n = __copy_user_zeroing_intel(to, from, n);
	return n;
}
EXPORT_SYMBOL(__copy_from_user_ll);

unsigned long __copy_from_user_ll_nozero(void *to, const void __user *from,
					 unsigned long n)
{
	if (movsl_is_ok(to, from, n))
		__copy_user(to, from, n);
	else
		n = __copy_user_intel((void __user *)to,
				      (const void *)from, n);
	return n;
}
EXPORT_SYMBOL(__copy_from_user_ll_nozero);

unsigned long __copy_from_user_ll_nocache(void *to, const void __user *from,
					unsigned long n)
{
#ifdef CONFIG_X86_INTEL_USERCOPY
	if (n > 64 && cpu_has_xmm2)
		n = __copy_user_zeroing_intel_nocache(to, from, n);
	else
		__copy_user_zeroing(to, from, n);
#else
	__copy_user_zeroing(to, from, n);
#endif
	return n;
}
EXPORT_SYMBOL(__copy_from_user_ll_nocache);
=======
	__uaccess_end();
	return n;
}
EXPORT_SYMBOL(__copy_user_ll);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

unsigned long __copy_from_user_ll_nocache_nozero(void *to, const void __user *from,
					unsigned long n)
{
<<<<<<< HEAD
#ifdef CONFIG_X86_INTEL_USERCOPY
	if (n > 64 && cpu_has_xmm2)
=======
	__uaccess_begin_nospec();
#ifdef CONFIG_X86_INTEL_USERCOPY
	if (n > 64 && static_cpu_has(X86_FEATURE_XMM2))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		n = __copy_user_intel_nocache(to, from, n);
	else
		__copy_user(to, from, n);
#else
	__copy_user(to, from, n);
#endif
<<<<<<< HEAD
	return n;
}
EXPORT_SYMBOL(__copy_from_user_ll_nocache_nozero);

/**
 * copy_to_user: - Copy a block of data into user space.
 * @to:   Destination address, in user space.
 * @from: Source address, in kernel space.
 * @n:    Number of bytes to copy.
 *
 * Context: User context only.  This function may sleep.
 *
 * Copy data from kernel space to user space.
 *
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 */
unsigned long
copy_to_user(void __user *to, const void *from, unsigned long n)
{
	if (access_ok(VERIFY_WRITE, to, n))
		n = __copy_to_user(to, from, n);
	return n;
}
EXPORT_SYMBOL(copy_to_user);

/**
 * copy_from_user: - Copy a block of data from user space.
 * @to:   Destination address, in kernel space.
 * @from: Source address, in user space.
 * @n:    Number of bytes to copy.
 *
 * Context: User context only.  This function may sleep.
 *
 * Copy data from user space to kernel space.
 *
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 *
 * If some data could not be copied, this function will pad the copied
 * data to the requested size using zero bytes.
 */
unsigned long
_copy_from_user(void *to, const void __user *from, unsigned long n)
{
	if (access_ok(VERIFY_READ, from, n))
		n = __copy_from_user(to, from, n);
	else
		memset(to, 0, n);
	return n;
}
EXPORT_SYMBOL(_copy_from_user);

void copy_from_user_overflow(void)
{
	WARN(1, "Buffer overflow detected!\n");
}
EXPORT_SYMBOL(copy_from_user_overflow);
=======
	__uaccess_end();
	return n;
}
EXPORT_SYMBOL(__copy_from_user_ll_nocache_nozero);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
