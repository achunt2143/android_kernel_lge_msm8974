<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Split spinlock implementation out into its own file, so it can be
 * compiled in a FTRACE-compatible way.
 */
#include <linux/spinlock.h>
<<<<<<< HEAD
#include <linux/module.h>

#include <asm/paravirt.h>

static inline void
default_spin_lock_flags(arch_spinlock_t *lock, unsigned long flags)
{
	arch_spin_lock(lock);
}

struct pv_lock_ops pv_lock_ops = {
#ifdef CONFIG_SMP
	.spin_is_locked = __ticket_spin_is_locked,
	.spin_is_contended = __ticket_spin_is_contended,

	.spin_lock = __ticket_spin_lock,
	.spin_lock_flags = default_spin_lock_flags,
	.spin_trylock = __ticket_spin_trylock,
	.spin_unlock = __ticket_spin_unlock,
#endif
};
EXPORT_SYMBOL(pv_lock_ops);

=======
#include <linux/export.h>
#include <linux/jump_label.h>

#include <asm/paravirt.h>

__visible void __native_queued_spin_unlock(struct qspinlock *lock)
{
	native_queued_spin_unlock(lock);
}
PV_CALLEE_SAVE_REGS_THUNK(__native_queued_spin_unlock);

bool pv_is_native_spin_unlock(void)
{
	return pv_ops.lock.queued_spin_unlock.func ==
		__raw_callee_save___native_queued_spin_unlock;
}

__visible bool __native_vcpu_is_preempted(long cpu)
{
	return false;
}
PV_CALLEE_SAVE_REGS_THUNK(__native_vcpu_is_preempted);

bool pv_is_native_vcpu_is_preempted(void)
{
	return pv_ops.lock.vcpu_is_preempted.func ==
		__raw_callee_save___native_vcpu_is_preempted;
}

void __init paravirt_set_cap(void)
{
	if (!pv_is_native_spin_unlock())
		setup_force_cpu_cap(X86_FEATURE_PVUNLOCK);

	if (!pv_is_native_vcpu_is_preempted())
		setup_force_cpu_cap(X86_FEATURE_VCPUPREEMPT);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
