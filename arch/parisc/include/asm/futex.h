<<<<<<< HEAD
#ifndef _ASM_PARISC_FUTEX_H
#define _ASM_PARISC_FUTEX_H

#ifdef __KERNEL__

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_PARISC_FUTEX_H
#define _ASM_PARISC_FUTEX_H

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/futex.h>
#include <linux/uaccess.h>
#include <asm/atomic.h>
#include <asm/errno.h>

/* The following has to match the LWS code in syscall.S.  We have
<<<<<<< HEAD
   sixteen four-word locks. */

static inline void
_futex_spin_lock_irqsave(u32 __user *uaddr, unsigned long int *flags)
{
	extern u32 lws_lock_start[];
	long index = ((long)uaddr & 0xf0) >> 2;
	arch_spinlock_t *s = (arch_spinlock_t *)&lws_lock_start[index];
=======
 * 256 four-word locks. We use bits 20-27 of the futex virtual
 * address for the hash index.
 */

static inline unsigned long _futex_hash_index(unsigned long ua)
{
	return (ua >> 2) & 0x3fc;
}

static inline void
_futex_spin_lock_irqsave(arch_spinlock_t *s, unsigned long *flags)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	local_irq_save(*flags);
	arch_spin_lock(s);
}

static inline void
<<<<<<< HEAD
_futex_spin_unlock_irqrestore(u32 __user *uaddr, unsigned long int *flags)
{
	extern u32 lws_lock_start[];
	long index = ((long)uaddr & 0xf0) >> 2;
	arch_spinlock_t *s = (arch_spinlock_t *)&lws_lock_start[index];
=======
_futex_spin_unlock_irqrestore(arch_spinlock_t *s, unsigned long *flags)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	arch_spin_unlock(s);
	local_irq_restore(*flags);
}

static inline int
<<<<<<< HEAD
futex_atomic_op_inuser (int encoded_op, u32 __user *uaddr)
{
	unsigned long int flags;
	u32 val;
	int op = (encoded_op >> 28) & 7;
	int cmp = (encoded_op >> 24) & 15;
	int oparg = (encoded_op << 8) >> 20;
	int cmparg = (encoded_op << 20) >> 20;
	int oldval = 0, ret;
	if (encoded_op & (FUTEX_OP_OPARG_SHIFT << 28))
		oparg = 1 << oparg;

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(*uaddr)))
		return -EFAULT;

	pagefault_disable();

	_futex_spin_lock_irqsave(uaddr, &flags);

	switch (op) {
	case FUTEX_OP_SET:
		/* *(int *)UADDR2 = OPARG; */
		ret = get_user(oldval, uaddr);
		if (!ret)
			ret = put_user(oparg, uaddr);
		break;
	case FUTEX_OP_ADD:
		/* *(int *)UADDR2 += OPARG; */
		ret = get_user(oldval, uaddr);
		if (!ret) {
			val = oldval + oparg;
			ret = put_user(val, uaddr);
		}
		break;
	case FUTEX_OP_OR:
		/* *(int *)UADDR2 |= OPARG; */
		ret = get_user(oldval, uaddr);
		if (!ret) {
			val = oldval | oparg;
			ret = put_user(val, uaddr);
		}
		break;
	case FUTEX_OP_ANDN:
		/* *(int *)UADDR2 &= ~OPARG; */
		ret = get_user(oldval, uaddr);
		if (!ret) {
			val = oldval & ~oparg;
			ret = put_user(val, uaddr);
		}
		break;
	case FUTEX_OP_XOR:
		/* *(int *)UADDR2 ^= OPARG; */
		ret = get_user(oldval, uaddr);
		if (!ret) {
			val = oldval ^ oparg;
			ret = put_user(val, uaddr);
		}
		break;
	default:
		ret = -ENOSYS;
	}

	_futex_spin_unlock_irqrestore(uaddr, &flags);

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
	return ret;
}

/* Non-atomic version */
=======
arch_futex_atomic_op_inuser(int op, int oparg, int *oval, u32 __user *uaddr)
{
	extern u32 lws_lock_start[];
	unsigned long ua = (unsigned long)uaddr;
	arch_spinlock_t *s;
	unsigned long flags;
	int oldval, ret;
	u32 tmp;

	s = (arch_spinlock_t *)&lws_lock_start[_futex_hash_index(ua)];
	_futex_spin_lock_irqsave(s, &flags);

	/* Return -EFAULT if we encounter a page fault or COW break */
	if (unlikely(get_user(oldval, uaddr) != 0)) {
		ret = -EFAULT;
		goto out_pagefault_enable;
	}

	ret = 0;
	tmp = oldval;

	switch (op) {
	case FUTEX_OP_SET:
		tmp = oparg;
		break;
	case FUTEX_OP_ADD:
		tmp += oparg;
		break;
	case FUTEX_OP_OR:
		tmp |= oparg;
		break;
	case FUTEX_OP_ANDN:
		tmp &= ~oparg;
		break;
	case FUTEX_OP_XOR:
		tmp ^= oparg;
		break;
	default:
		ret = -ENOSYS;
		goto out_pagefault_enable;
	}

	if (unlikely(put_user(tmp, uaddr) != 0))
		ret = -EFAULT;

out_pagefault_enable:
	_futex_spin_unlock_irqrestore(s, &flags);

	if (!ret)
		*oval = oldval;

	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int
futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
			      u32 oldval, u32 newval)
{
<<<<<<< HEAD
	int ret;
	u32 val;
	unsigned long flags;

	/* futex.c wants to do a cmpxchg_inatomic on kernel NULL, which is
	 * our gateway page, and causes no end of trouble...
	 */
	if (segment_eq(KERNEL_DS, get_fs()) && !uaddr)
		return -EFAULT;

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(u32)))
=======
	extern u32 lws_lock_start[];
	unsigned long ua = (unsigned long)uaddr;
	arch_spinlock_t *s;
	u32 val;
	unsigned long flags;

	if (!access_ok(uaddr, sizeof(u32)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;

	/* HPPA has no cmpxchg in hardware and therefore the
	 * best we can do here is use an array of locks. The
<<<<<<< HEAD
	 * lock selected is based on a hash of the userspace
	 * address. This should scale to a couple of CPUs.
	 */

	_futex_spin_lock_irqsave(uaddr, &flags);

	ret = get_user(val, uaddr);

	if (!ret && val == oldval)
		ret = put_user(newval, uaddr);

	*uval = val;

	_futex_spin_unlock_irqrestore(uaddr, &flags);

	return ret;
}

#endif /*__KERNEL__*/
=======
	 * lock selected is based on a hash of the virtual
	 * address of the futex. This should scale to a couple
	 * of CPUs.
	 */

	s = (arch_spinlock_t *)&lws_lock_start[_futex_hash_index(ua)];
	_futex_spin_lock_irqsave(s, &flags);
	if (unlikely(get_user(val, uaddr) != 0)) {
		_futex_spin_unlock_irqrestore(s, &flags);
		return -EFAULT;
	}

	if (val == oldval && unlikely(put_user(newval, uaddr) != 0)) {
		_futex_spin_unlock_irqrestore(s, &flags);
		return -EFAULT;
	}

	*uval = val;
	_futex_spin_unlock_irqrestore(s, &flags);

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /*_ASM_PARISC_FUTEX_H*/
