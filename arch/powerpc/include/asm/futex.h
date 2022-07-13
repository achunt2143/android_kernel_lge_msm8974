<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_POWERPC_FUTEX_H
#define _ASM_POWERPC_FUTEX_H

#ifdef __KERNEL__

#include <linux/futex.h>
#include <linux/uaccess.h>
#include <asm/errno.h>
#include <asm/synch.h>
<<<<<<< HEAD
#include <asm/asm-compat.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define __futex_atomic_op(insn, ret, oldval, uaddr, oparg) \
  __asm__ __volatile ( \
	PPC_ATOMIC_ENTRY_BARRIER \
"1:	lwarx	%0,0,%2\n" \
	insn \
<<<<<<< HEAD
	PPC405_ERR77(0, %2) \
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
"2:	stwcx.	%1,0,%2\n" \
	"bne-	1b\n" \
	PPC_ATOMIC_EXIT_BARRIER \
	"li	%1,0\n" \
"3:	.section .fixup,\"ax\"\n" \
"4:	li	%1,%3\n" \
	"b	3b\n" \
	".previous\n" \
<<<<<<< HEAD
	".section __ex_table,\"a\"\n" \
	".align 3\n" \
	PPC_LONG "1b,4b,2b,4b\n" \
	".previous" \
=======
	EX_TABLE(1b, 4b) \
	EX_TABLE(2b, 4b) \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	: "=&r" (oldval), "=&r" (ret) \
	: "b" (uaddr), "i" (-EFAULT), "r" (oparg) \
	: "cr0", "memory")

<<<<<<< HEAD
static inline int futex_atomic_op_inuser (int encoded_op, u32 __user *uaddr)
{
	int op = (encoded_op >> 28) & 7;
	int cmp = (encoded_op >> 24) & 15;
	int oparg = (encoded_op << 8) >> 20;
	int cmparg = (encoded_op << 20) >> 20;
	int oldval = 0, ret;
	if (encoded_op & (FUTEX_OP_OPARG_SHIFT << 28))
		oparg = 1 << oparg;

	if (! access_ok (VERIFY_WRITE, uaddr, sizeof(u32)))
		return -EFAULT;

	pagefault_disable();

=======
static inline int arch_futex_atomic_op_inuser(int op, int oparg, int *oval,
		u32 __user *uaddr)
{
	int oldval = 0, ret;

	if (!user_access_begin(uaddr, sizeof(u32)))
		return -EFAULT;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (op) {
	case FUTEX_OP_SET:
		__futex_atomic_op("mr %1,%4\n", ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_ADD:
		__futex_atomic_op("add %1,%0,%4\n", ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_OR:
		__futex_atomic_op("or %1,%0,%4\n", ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_ANDN:
		__futex_atomic_op("andc %1,%0,%4\n", ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_XOR:
		__futex_atomic_op("xor %1,%0,%4\n", ret, oldval, uaddr, oparg);
		break;
	default:
		ret = -ENOSYS;
	}
<<<<<<< HEAD

	pagefault_enable();

	if (!ret) {
		switch (cmp) {
		case FUTEX_OP_CMP_EQ: ret = (oldval == cmparg); break;
		case FUTEX_OP_CMP_NE: ret = (oldval != cmparg); break;
		case FUTEX_OP_CMP_LT: ret = (oldval < cmparg); break;
		case FUTEX_OP_CMP_GE: ret = (oldval >= cmparg); break;
		case FUTEX_OP_CMP_LE: ret = (oldval <= cmparg); break;
		case FUTEX_OP_CMP_GT: ret = (oldval > cmparg); break;
		default: ret = -ENOSYS;
		}
	}
=======
	user_access_end();

	*oval = oldval;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static inline int
futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
			      u32 oldval, u32 newval)
{
	int ret = 0;
	u32 prev;

<<<<<<< HEAD
	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(u32)))
=======
	if (!user_access_begin(uaddr, sizeof(u32)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;

        __asm__ __volatile__ (
        PPC_ATOMIC_ENTRY_BARRIER
"1:     lwarx   %1,0,%3         # futex_atomic_cmpxchg_inatomic\n\
        cmpw    0,%1,%4\n\
        bne-    3f\n"
<<<<<<< HEAD
        PPC405_ERR77(0,%3)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
"2:     stwcx.  %5,0,%3\n\
        bne-    1b\n"
        PPC_ATOMIC_EXIT_BARRIER
"3:	.section .fixup,\"ax\"\n\
4:	li	%0,%6\n\
	b	3b\n\
<<<<<<< HEAD
	.previous\n\
	.section __ex_table,\"a\"\n\
	.align 3\n\
	" PPC_LONG "1b,4b,2b,4b\n\
	.previous" \
=======
	.previous\n"
	EX_TABLE(1b, 4b)
	EX_TABLE(2b, 4b)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
        : "+r" (ret), "=&r" (prev), "+m" (*uaddr)
        : "r" (uaddr), "r" (oldval), "r" (newval), "i" (-EFAULT)
        : "cc", "memory");

<<<<<<< HEAD
	*uval = prev;
=======
	user_access_end();

	*uval = prev;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
        return ret;
}

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_FUTEX_H */
