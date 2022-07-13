<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2011 Google, Inc.
 *
 * Author:
 *	Colin Cross <ccross@android.com>
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/cpu_pm.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/spinlock.h>
#include <linux/syscore_ops.h>

<<<<<<< HEAD
static DEFINE_RWLOCK(cpu_pm_notifier_lock);
static RAW_NOTIFIER_HEAD(cpu_pm_notifier_chain);

static int cpu_pm_notify(enum cpu_pm_event event, int nr_to_call, int *nr_calls)
{
	int ret;

	ret = __raw_notifier_call_chain(&cpu_pm_notifier_chain, event, NULL,
		nr_to_call, nr_calls);
=======
/*
 * atomic_notifiers use a spinlock_t, which can block under PREEMPT_RT.
 * Notifications for cpu_pm will be issued by the idle task itself, which can
 * never block, IOW it requires using a raw_spinlock_t.
 */
static struct {
	struct raw_notifier_head chain;
	raw_spinlock_t lock;
} cpu_pm_notifier = {
	.chain = RAW_NOTIFIER_INIT(cpu_pm_notifier.chain),
	.lock  = __RAW_SPIN_LOCK_UNLOCKED(cpu_pm_notifier.lock),
};

static int cpu_pm_notify(enum cpu_pm_event event)
{
	int ret;

	rcu_read_lock();
	ret = raw_notifier_call_chain(&cpu_pm_notifier.chain, event, NULL);
	rcu_read_unlock();

	return notifier_to_errno(ret);
}

static int cpu_pm_notify_robust(enum cpu_pm_event event_up, enum cpu_pm_event event_down)
{
	unsigned long flags;
	int ret;

	raw_spin_lock_irqsave(&cpu_pm_notifier.lock, flags);
	ret = raw_notifier_call_chain_robust(&cpu_pm_notifier.chain, event_up, event_down, NULL);
	raw_spin_unlock_irqrestore(&cpu_pm_notifier.lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return notifier_to_errno(ret);
}

/**
 * cpu_pm_register_notifier - register a driver with cpu_pm
 * @nb: notifier block to register
 *
 * Add a driver to a list of drivers that are notified about
 * CPU and CPU cluster low power entry and exit.
 *
<<<<<<< HEAD
 * This function may sleep, and has the same return conditions as
 * raw_notifier_chain_register.
=======
 * This function has the same return conditions as raw_notifier_chain_register.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int cpu_pm_register_notifier(struct notifier_block *nb)
{
	unsigned long flags;
	int ret;

<<<<<<< HEAD
	write_lock_irqsave(&cpu_pm_notifier_lock, flags);
	ret = raw_notifier_chain_register(&cpu_pm_notifier_chain, nb);
	write_unlock_irqrestore(&cpu_pm_notifier_lock, flags);

=======
	raw_spin_lock_irqsave(&cpu_pm_notifier.lock, flags);
	ret = raw_notifier_chain_register(&cpu_pm_notifier.chain, nb);
	raw_spin_unlock_irqrestore(&cpu_pm_notifier.lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL_GPL(cpu_pm_register_notifier);

/**
 * cpu_pm_unregister_notifier - unregister a driver with cpu_pm
 * @nb: notifier block to be unregistered
 *
 * Remove a driver from the CPU PM notifier list.
 *
<<<<<<< HEAD
 * This function may sleep, and has the same return conditions as
 * raw_notifier_chain_unregister.
=======
 * This function has the same return conditions as raw_notifier_chain_unregister.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int cpu_pm_unregister_notifier(struct notifier_block *nb)
{
	unsigned long flags;
	int ret;

<<<<<<< HEAD
	write_lock_irqsave(&cpu_pm_notifier_lock, flags);
	ret = raw_notifier_chain_unregister(&cpu_pm_notifier_chain, nb);
	write_unlock_irqrestore(&cpu_pm_notifier_lock, flags);

=======
	raw_spin_lock_irqsave(&cpu_pm_notifier.lock, flags);
	ret = raw_notifier_chain_unregister(&cpu_pm_notifier.chain, nb);
	raw_spin_unlock_irqrestore(&cpu_pm_notifier.lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL_GPL(cpu_pm_unregister_notifier);

/**
<<<<<<< HEAD
 * cpm_pm_enter - CPU low power entry notifier
=======
 * cpu_pm_enter - CPU low power entry notifier
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Notifies listeners that a single CPU is entering a low power state that may
 * cause some blocks in the same power domain as the cpu to reset.
 *
 * Must be called on the affected CPU with interrupts disabled.  Platform is
 * responsible for ensuring that cpu_pm_enter is not called twice on the same
 * CPU before cpu_pm_exit is called. Notified drivers can include VFP
<<<<<<< HEAD
 * co-processor, interrupt controller and it's PM extensions, local CPU
=======
 * co-processor, interrupt controller and its PM extensions, local CPU
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * timers context save/restore which shouldn't be interrupted. Hence it
 * must be called with interrupts disabled.
 *
 * Return conditions are same as __raw_notifier_call_chain.
 */
int cpu_pm_enter(void)
{
<<<<<<< HEAD
	int nr_calls;
	int ret = 0;

	read_lock(&cpu_pm_notifier_lock);
	ret = cpu_pm_notify(CPU_PM_ENTER, -1, &nr_calls);
	if (ret)
		/*
		 * Inform listeners (nr_calls - 1) about failure of CPU PM
		 * PM entry who are notified earlier to prepare for it.
		 */
		cpu_pm_notify(CPU_PM_ENTER_FAILED, nr_calls - 1, NULL);
	read_unlock(&cpu_pm_notifier_lock);

	return ret;
=======
	return cpu_pm_notify_robust(CPU_PM_ENTER, CPU_PM_ENTER_FAILED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(cpu_pm_enter);

/**
<<<<<<< HEAD
 * cpm_pm_exit - CPU low power exit notifier
=======
 * cpu_pm_exit - CPU low power exit notifier
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Notifies listeners that a single CPU is exiting a low power state that may
 * have caused some blocks in the same power domain as the cpu to reset.
 *
 * Notified drivers can include VFP co-processor, interrupt controller
<<<<<<< HEAD
 * and it's PM extensions, local CPU timers context save/restore which
=======
 * and its PM extensions, local CPU timers context save/restore which
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * shouldn't be interrupted. Hence it must be called with interrupts disabled.
 *
 * Return conditions are same as __raw_notifier_call_chain.
 */
int cpu_pm_exit(void)
{
<<<<<<< HEAD
	int ret;

	read_lock(&cpu_pm_notifier_lock);
	ret = cpu_pm_notify(CPU_PM_EXIT, -1, NULL);
	read_unlock(&cpu_pm_notifier_lock);

	return ret;
=======
	return cpu_pm_notify(CPU_PM_EXIT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(cpu_pm_exit);

/**
<<<<<<< HEAD
 * cpm_cluster_pm_enter - CPU cluster low power entry notifier
=======
 * cpu_cluster_pm_enter - CPU cluster low power entry notifier
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Notifies listeners that all cpus in a power domain are entering a low power
 * state that may cause some blocks in the same power domain to reset.
 *
 * Must be called after cpu_pm_enter has been called on all cpus in the power
 * domain, and before cpu_pm_exit has been called on any cpu in the power
 * domain. Notified drivers can include VFP co-processor, interrupt controller
<<<<<<< HEAD
 * and it's PM extensions, local CPU timers context save/restore which
=======
 * and its PM extensions, local CPU timers context save/restore which
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * shouldn't be interrupted. Hence it must be called with interrupts disabled.
 *
 * Must be called with interrupts disabled.
 *
 * Return conditions are same as __raw_notifier_call_chain.
 */
int cpu_cluster_pm_enter(void)
{
<<<<<<< HEAD
	int nr_calls;
	int ret = 0;

	read_lock(&cpu_pm_notifier_lock);
	ret = cpu_pm_notify(CPU_CLUSTER_PM_ENTER, -1, &nr_calls);
	if (ret)
		/*
		 * Inform listeners (nr_calls - 1) about failure of CPU cluster
		 * PM entry who are notified earlier to prepare for it.
		 */
		cpu_pm_notify(CPU_CLUSTER_PM_ENTER_FAILED, nr_calls - 1, NULL);
	read_unlock(&cpu_pm_notifier_lock);

	return ret;
=======
	return cpu_pm_notify_robust(CPU_CLUSTER_PM_ENTER, CPU_CLUSTER_PM_ENTER_FAILED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(cpu_cluster_pm_enter);

/**
<<<<<<< HEAD
 * cpm_cluster_pm_exit - CPU cluster low power exit notifier
=======
 * cpu_cluster_pm_exit - CPU cluster low power exit notifier
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Notifies listeners that all cpus in a power domain are exiting form a
 * low power state that may have caused some blocks in the same power domain
 * to reset.
 *
<<<<<<< HEAD
 * Must be called after cpu_pm_exit has been called on all cpus in the power
 * domain, and before cpu_pm_exit has been called on any cpu in the power
 * domain. Notified drivers can include VFP co-processor, interrupt controller
 * and it's PM extensions, local CPU timers context save/restore which
=======
 * Must be called after cpu_cluster_pm_enter has been called for the power
 * domain, and before cpu_pm_exit has been called on any cpu in the power
 * domain. Notified drivers can include VFP co-processor, interrupt controller
 * and its PM extensions, local CPU timers context save/restore which
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * shouldn't be interrupted. Hence it must be called with interrupts disabled.
 *
 * Return conditions are same as __raw_notifier_call_chain.
 */
int cpu_cluster_pm_exit(void)
{
<<<<<<< HEAD
	int ret;

	read_lock(&cpu_pm_notifier_lock);
	ret = cpu_pm_notify(CPU_CLUSTER_PM_EXIT, -1, NULL);
	read_unlock(&cpu_pm_notifier_lock);

	return ret;
=======
	return cpu_pm_notify(CPU_CLUSTER_PM_EXIT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(cpu_cluster_pm_exit);

#ifdef CONFIG_PM
static int cpu_pm_suspend(void)
{
	int ret;

	ret = cpu_pm_enter();
	if (ret)
		return ret;

	ret = cpu_cluster_pm_enter();
	return ret;
}

static void cpu_pm_resume(void)
{
	cpu_cluster_pm_exit();
	cpu_pm_exit();
}

static struct syscore_ops cpu_pm_syscore_ops = {
	.suspend = cpu_pm_suspend,
	.resume = cpu_pm_resume,
};

static int cpu_pm_init(void)
{
	register_syscore_ops(&cpu_pm_syscore_ops);
	return 0;
}
core_initcall(cpu_pm_init);
#endif
