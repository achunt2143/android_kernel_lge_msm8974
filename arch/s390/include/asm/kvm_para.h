<<<<<<< HEAD
/*
 * asm-s390/kvm_para.h - definition for paravirtual devices on s390
 *
 * Copyright IBM Corp. 2008
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2 only)
 * as published by the Free Software Foundation.
 *
 *    Author(s): Christian Borntraeger <borntraeger@de.ibm.com>
 */

#ifndef __S390_KVM_PARA_H
#define __S390_KVM_PARA_H

#ifdef __KERNEL__

=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * definition for paravirtual devices on s390
 *
 * Copyright IBM Corp. 2008
 *
 *    Author(s): Christian Borntraeger <borntraeger@de.ibm.com>
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Hypercalls for KVM on s390. The calling convention is similar to the
 * s390 ABI, so we use R2-R6 for parameters 1-5. In addition we use R1
 * as hypercall number and R7 as parameter 6. The return value is
 * written to R2. We use the diagnose instruction as hypercall. To avoid
 * conflicts with existing diagnoses for LPAR and z/VM, we do not use
 * the instruction encoded number, but specify the number in R1 and
 * use 0x500 as KVM hypercall
 *
 * Copyright IBM Corp. 2007,2008
 * Author(s): Christian Borntraeger <borntraeger@de.ibm.com>
<<<<<<< HEAD
 *
 * This work is licensed under the terms of the GNU GPL, version 2.
 */

static inline long kvm_hypercall0(unsigned long nr)
{
	register unsigned long __nr asm("1") = nr;
	register long __rc asm("2");

	asm volatile ("diag 2,4,0x500\n"
		      : "=d" (__rc) : "d" (__nr): "memory", "cc");
	return __rc;
}

static inline long kvm_hypercall1(unsigned long nr, unsigned long p1)
{
	register unsigned long __nr asm("1") = nr;
	register unsigned long __p1 asm("2") = p1;
	register long __rc asm("2");

	asm volatile ("diag 2,4,0x500\n"
		      : "=d" (__rc) : "d" (__nr), "0" (__p1) : "memory", "cc");
	return __rc;
}

static inline long kvm_hypercall2(unsigned long nr, unsigned long p1,
			       unsigned long p2)
{
	register unsigned long __nr asm("1") = nr;
	register unsigned long __p1 asm("2") = p1;
	register unsigned long __p2 asm("3") = p2;
	register long __rc asm("2");

	asm volatile ("diag 2,4,0x500\n"
		      : "=d" (__rc) : "d" (__nr), "0" (__p1), "d" (__p2)
		      : "memory", "cc");
	return __rc;
}

static inline long kvm_hypercall3(unsigned long nr, unsigned long p1,
			       unsigned long p2, unsigned long p3)
{
	register unsigned long __nr asm("1") = nr;
	register unsigned long __p1 asm("2") = p1;
	register unsigned long __p2 asm("3") = p2;
	register unsigned long __p3 asm("4") = p3;
	register long __rc asm("2");

	asm volatile ("diag 2,4,0x500\n"
		      : "=d" (__rc) : "d" (__nr), "0" (__p1), "d" (__p2),
			"d" (__p3) : "memory", "cc");
	return __rc;
}


static inline long kvm_hypercall4(unsigned long nr, unsigned long p1,
			       unsigned long p2, unsigned long p3,
			       unsigned long p4)
{
	register unsigned long __nr asm("1") = nr;
	register unsigned long __p1 asm("2") = p1;
	register unsigned long __p2 asm("3") = p2;
	register unsigned long __p3 asm("4") = p3;
	register unsigned long __p4 asm("5") = p4;
	register long __rc asm("2");

	asm volatile ("diag 2,4,0x500\n"
		      : "=d" (__rc) : "d" (__nr), "0" (__p1), "d" (__p2),
			"d" (__p3), "d" (__p4) : "memory", "cc");
	return __rc;
}

static inline long kvm_hypercall5(unsigned long nr, unsigned long p1,
			       unsigned long p2, unsigned long p3,
			       unsigned long p4, unsigned long p5)
{
	register unsigned long __nr asm("1") = nr;
	register unsigned long __p1 asm("2") = p1;
	register unsigned long __p2 asm("3") = p2;
	register unsigned long __p3 asm("4") = p3;
	register unsigned long __p4 asm("5") = p4;
	register unsigned long __p5 asm("6") = p5;
	register long __rc asm("2");

	asm volatile ("diag 2,4,0x500\n"
		      : "=d" (__rc) : "d" (__nr), "0" (__p1), "d" (__p2),
			"d" (__p3), "d" (__p4), "d" (__p5)  : "memory", "cc");
	return __rc;
}

static inline long kvm_hypercall6(unsigned long nr, unsigned long p1,
			       unsigned long p2, unsigned long p3,
			       unsigned long p4, unsigned long p5,
			       unsigned long p6)
{
	register unsigned long __nr asm("1") = nr;
	register unsigned long __p1 asm("2") = p1;
	register unsigned long __p2 asm("3") = p2;
	register unsigned long __p3 asm("4") = p3;
	register unsigned long __p4 asm("5") = p4;
	register unsigned long __p5 asm("6") = p5;
	register unsigned long __p6 asm("7") = p6;
	register long __rc asm("2");

	asm volatile ("diag 2,4,0x500\n"
		      : "=d" (__rc) : "d" (__nr), "0" (__p1), "d" (__p2),
			"d" (__p3), "d" (__p4), "d" (__p5), "d" (__p6)
		      : "memory", "cc");
	return __rc;
}
=======
 */
#ifndef __S390_KVM_PARA_H
#define __S390_KVM_PARA_H

#include <uapi/asm/kvm_para.h>
#include <asm/diag.h>

#define HYPERCALL_FMT_0
#define HYPERCALL_FMT_1 , "0" (r2)
#define HYPERCALL_FMT_2 , "d" (r3) HYPERCALL_FMT_1
#define HYPERCALL_FMT_3 , "d" (r4) HYPERCALL_FMT_2
#define HYPERCALL_FMT_4 , "d" (r5) HYPERCALL_FMT_3
#define HYPERCALL_FMT_5 , "d" (r6) HYPERCALL_FMT_4
#define HYPERCALL_FMT_6 , "d" (r7) HYPERCALL_FMT_5

#define HYPERCALL_PARM_0
#define HYPERCALL_PARM_1 , unsigned long arg1
#define HYPERCALL_PARM_2 HYPERCALL_PARM_1, unsigned long arg2
#define HYPERCALL_PARM_3 HYPERCALL_PARM_2, unsigned long arg3
#define HYPERCALL_PARM_4 HYPERCALL_PARM_3, unsigned long arg4
#define HYPERCALL_PARM_5 HYPERCALL_PARM_4, unsigned long arg5
#define HYPERCALL_PARM_6 HYPERCALL_PARM_5, unsigned long arg6

#define HYPERCALL_REGS_0
#define HYPERCALL_REGS_1						\
	register unsigned long r2 asm("2") = arg1
#define HYPERCALL_REGS_2						\
	HYPERCALL_REGS_1;						\
	register unsigned long r3 asm("3") = arg2
#define HYPERCALL_REGS_3						\
	HYPERCALL_REGS_2;						\
	register unsigned long r4 asm("4") = arg3
#define HYPERCALL_REGS_4						\
	HYPERCALL_REGS_3;						\
	register unsigned long r5 asm("5") = arg4
#define HYPERCALL_REGS_5						\
	HYPERCALL_REGS_4;						\
	register unsigned long r6 asm("6") = arg5
#define HYPERCALL_REGS_6						\
	HYPERCALL_REGS_5;						\
	register unsigned long r7 asm("7") = arg6

#define HYPERCALL_ARGS_0
#define HYPERCALL_ARGS_1 , arg1
#define HYPERCALL_ARGS_2 HYPERCALL_ARGS_1, arg2
#define HYPERCALL_ARGS_3 HYPERCALL_ARGS_2, arg3
#define HYPERCALL_ARGS_4 HYPERCALL_ARGS_3, arg4
#define HYPERCALL_ARGS_5 HYPERCALL_ARGS_4, arg5
#define HYPERCALL_ARGS_6 HYPERCALL_ARGS_5, arg6

#define GENERATE_KVM_HYPERCALL_FUNC(args)				\
static inline								\
long __kvm_hypercall##args(unsigned long nr HYPERCALL_PARM_##args)	\
{									\
	register unsigned long __nr asm("1") = nr;			\
	register long __rc asm("2");					\
	HYPERCALL_REGS_##args;						\
									\
	asm volatile (							\
		"	diag	2,4,0x500\n"				\
		: "=d" (__rc)						\
		: "d" (__nr) HYPERCALL_FMT_##args			\
		: "memory", "cc");					\
	return __rc;							\
}									\
									\
static inline								\
long kvm_hypercall##args(unsigned long nr HYPERCALL_PARM_##args)	\
{									\
	diag_stat_inc(DIAG_STAT_X500);					\
	return __kvm_hypercall##args(nr HYPERCALL_ARGS_##args);		\
}

GENERATE_KVM_HYPERCALL_FUNC(0)
GENERATE_KVM_HYPERCALL_FUNC(1)
GENERATE_KVM_HYPERCALL_FUNC(2)
GENERATE_KVM_HYPERCALL_FUNC(3)
GENERATE_KVM_HYPERCALL_FUNC(4)
GENERATE_KVM_HYPERCALL_FUNC(5)
GENERATE_KVM_HYPERCALL_FUNC(6)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* kvm on s390 is always paravirtualization enabled */
static inline int kvm_para_available(void)
{
	return 1;
}

/* No feature bits are currently assigned for kvm on s390 */
static inline unsigned int kvm_arch_para_features(void)
{
	return 0;
}

<<<<<<< HEAD
#endif
=======
static inline unsigned int kvm_arch_para_hints(void)
{
	return 0;
}

static inline bool kvm_check_and_clear_guest_paused(void)
{
	return false;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __S390_KVM_PARA_H */
