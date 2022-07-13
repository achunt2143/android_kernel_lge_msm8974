<<<<<<< HEAD
/*
 * linux/kernel/time/tick-broadcast.c
 *
=======
// SPDX-License-Identifier: GPL-2.0
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This file contains functions which emulate a local clock-event
 * device via a broadcast event source.
 *
 * Copyright(C) 2005-2006, Thomas Gleixner <tglx@linutronix.de>
 * Copyright(C) 2005-2007, Red Hat, Inc., Ingo Molnar
 * Copyright(C) 2006-2007, Timesys Corp., Thomas Gleixner
<<<<<<< HEAD
 *
 * This code is licenced under the GPL version 2. For details see
 * kernel-base/COPYING.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/cpu.h>
#include <linux/err.h>
#include <linux/hrtimer.h>
#include <linux/interrupt.h>
#include <linux/percpu.h>
#include <linux/profile.h>
#include <linux/sched.h>
<<<<<<< HEAD
=======
#include <linux/smp.h>
#include <linux/module.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "tick-internal.h"

/*
 * Broadcast support for broken x86 hardware, where the local apic
 * timer stops in C3 state.
 */

static struct tick_device tick_broadcast_device;
<<<<<<< HEAD
/* FIXME: Use cpumask_var_t. */
static DECLARE_BITMAP(tick_broadcast_mask, NR_CPUS);
static DECLARE_BITMAP(tmpmask, NR_CPUS);
static DEFINE_RAW_SPINLOCK(tick_broadcast_lock);
static int tick_broadcast_force;

#ifdef CONFIG_TICK_ONESHOT
static void tick_broadcast_clear_oneshot(int cpu);
#else
static inline void tick_broadcast_clear_oneshot(int cpu) { }
=======
static cpumask_var_t tick_broadcast_mask __cpumask_var_read_mostly;
static cpumask_var_t tick_broadcast_on __cpumask_var_read_mostly;
static cpumask_var_t tmpmask __cpumask_var_read_mostly;
static int tick_broadcast_forced;

static __cacheline_aligned_in_smp DEFINE_RAW_SPINLOCK(tick_broadcast_lock);

#ifdef CONFIG_TICK_ONESHOT
static DEFINE_PER_CPU(struct clock_event_device *, tick_oneshot_wakeup_device);

static void tick_broadcast_setup_oneshot(struct clock_event_device *bc, bool from_periodic);
static void tick_broadcast_clear_oneshot(int cpu);
static void tick_resume_broadcast_oneshot(struct clock_event_device *bc);
# ifdef CONFIG_HOTPLUG_CPU
static void tick_broadcast_oneshot_offline(unsigned int cpu);
# endif
#else
static inline void
tick_broadcast_setup_oneshot(struct clock_event_device *bc, bool from_periodic) { BUG(); }
static inline void tick_broadcast_clear_oneshot(int cpu) { }
static inline void tick_resume_broadcast_oneshot(struct clock_event_device *bc) { }
# ifdef CONFIG_HOTPLUG_CPU
static inline void tick_broadcast_oneshot_offline(unsigned int cpu) { }
# endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/*
 * Debugging: see timer_list.c
 */
struct tick_device *tick_get_broadcast_device(void)
{
	return &tick_broadcast_device;
}

struct cpumask *tick_get_broadcast_mask(void)
{
<<<<<<< HEAD
	return to_cpumask(tick_broadcast_mask);
=======
	return tick_broadcast_mask;
}

static struct clock_event_device *tick_get_oneshot_wakeup_device(int cpu);

const struct clock_event_device *tick_get_wakeup_device(int cpu)
{
	return tick_get_oneshot_wakeup_device(cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Start the device in periodic mode
 */
static void tick_broadcast_start_periodic(struct clock_event_device *bc)
{
	if (bc)
		tick_setup_periodic(bc, 1);
}

/*
 * Check, if the device can be utilized as broadcast device:
 */
<<<<<<< HEAD
int tick_check_broadcast_device(struct clock_event_device *dev)
{
	struct clock_event_device *cur = tick_broadcast_device.evtdev;

	if ((dev->features & CLOCK_EVT_FEAT_DUMMY) ||
	    (tick_broadcast_device.evtdev &&
	     tick_broadcast_device.evtdev->rating >= dev->rating) ||
	     (dev->features & CLOCK_EVT_FEAT_C3STOP))
		return 0;

	clockevents_exchange_device(tick_broadcast_device.evtdev, dev);
	if (cur)
		cur->event_handler = clockevents_handle_noop;
	tick_broadcast_device.evtdev = dev;
	if (!cpumask_empty(tick_get_broadcast_mask()))
		tick_broadcast_start_periodic(dev);
	return 1;
=======
static bool tick_check_broadcast_device(struct clock_event_device *curdev,
					struct clock_event_device *newdev)
{
	if ((newdev->features & CLOCK_EVT_FEAT_DUMMY) ||
	    (newdev->features & CLOCK_EVT_FEAT_PERCPU) ||
	    (newdev->features & CLOCK_EVT_FEAT_C3STOP))
		return false;

	if (tick_broadcast_device.mode == TICKDEV_MODE_ONESHOT &&
	    !(newdev->features & CLOCK_EVT_FEAT_ONESHOT))
		return false;

	return !curdev || newdev->rating > curdev->rating;
}

#ifdef CONFIG_TICK_ONESHOT
static struct clock_event_device *tick_get_oneshot_wakeup_device(int cpu)
{
	return per_cpu(tick_oneshot_wakeup_device, cpu);
}

static void tick_oneshot_wakeup_handler(struct clock_event_device *wd)
{
	/*
	 * If we woke up early and the tick was reprogrammed in the
	 * meantime then this may be spurious but harmless.
	 */
	tick_receive_broadcast();
}

static bool tick_set_oneshot_wakeup_device(struct clock_event_device *newdev,
					   int cpu)
{
	struct clock_event_device *curdev = tick_get_oneshot_wakeup_device(cpu);

	if (!newdev)
		goto set_device;

	if ((newdev->features & CLOCK_EVT_FEAT_DUMMY) ||
	    (newdev->features & CLOCK_EVT_FEAT_C3STOP))
		 return false;

	if (!(newdev->features & CLOCK_EVT_FEAT_PERCPU) ||
	    !(newdev->features & CLOCK_EVT_FEAT_ONESHOT))
		return false;

	if (!cpumask_equal(newdev->cpumask, cpumask_of(cpu)))
		return false;

	if (curdev && newdev->rating <= curdev->rating)
		return false;

	if (!try_module_get(newdev->owner))
		return false;

	newdev->event_handler = tick_oneshot_wakeup_handler;
set_device:
	clockevents_exchange_device(curdev, newdev);
	per_cpu(tick_oneshot_wakeup_device, cpu) = newdev;
	return true;
}
#else
static struct clock_event_device *tick_get_oneshot_wakeup_device(int cpu)
{
	return NULL;
}

static bool tick_set_oneshot_wakeup_device(struct clock_event_device *newdev,
					   int cpu)
{
	return false;
}
#endif

/*
 * Conditionally install/replace broadcast device
 */
void tick_install_broadcast_device(struct clock_event_device *dev, int cpu)
{
	struct clock_event_device *cur = tick_broadcast_device.evtdev;

	if (tick_set_oneshot_wakeup_device(dev, cpu))
		return;

	if (!tick_check_broadcast_device(cur, dev))
		return;

	if (!try_module_get(dev->owner))
		return;

	clockevents_exchange_device(cur, dev);
	if (cur)
		cur->event_handler = clockevents_handle_noop;
	tick_broadcast_device.evtdev = dev;
	if (!cpumask_empty(tick_broadcast_mask))
		tick_broadcast_start_periodic(dev);

	if (!(dev->features & CLOCK_EVT_FEAT_ONESHOT))
		return;

	/*
	 * If the system already runs in oneshot mode, switch the newly
	 * registered broadcast device to oneshot mode explicitly.
	 */
	if (tick_broadcast_oneshot_active()) {
		tick_broadcast_switch_to_oneshot();
		return;
	}

	/*
	 * Inform all cpus about this. We might be in a situation
	 * where we did not switch to oneshot mode because the per cpu
	 * devices are affected by CLOCK_EVT_FEAT_C3STOP and the lack
	 * of a oneshot capable broadcast device. Without that
	 * notification the systems stays stuck in periodic mode
	 * forever.
	 */
	tick_clock_notify();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Check, if the device is the broadcast device
 */
int tick_is_broadcast_device(struct clock_event_device *dev)
{
	return (dev && tick_broadcast_device.evtdev == dev);
}

<<<<<<< HEAD
/*
 * Check, if the device is disfunctional and a place holder, which
=======
int tick_broadcast_update_freq(struct clock_event_device *dev, u32 freq)
{
	int ret = -ENODEV;

	if (tick_is_broadcast_device(dev)) {
		raw_spin_lock(&tick_broadcast_lock);
		ret = __clockevents_update_freq(dev, freq);
		raw_spin_unlock(&tick_broadcast_lock);
	}
	return ret;
}


static void err_broadcast(const struct cpumask *mask)
{
	pr_crit_once("Failed to broadcast timer tick. Some CPUs may be unresponsive.\n");
}

static void tick_device_setup_broadcast_func(struct clock_event_device *dev)
{
	if (!dev->broadcast)
		dev->broadcast = tick_broadcast;
	if (!dev->broadcast) {
		pr_warn_once("%s depends on broadcast, but no broadcast function available\n",
			     dev->name);
		dev->broadcast = err_broadcast;
	}
}

/*
 * Check, if the device is dysfunctional and a placeholder, which
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * needs to be handled by the broadcast device.
 */
int tick_device_uses_broadcast(struct clock_event_device *dev, int cpu)
{
<<<<<<< HEAD
=======
	struct clock_event_device *bc = tick_broadcast_device.evtdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	int ret = 0;

	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);

	/*
	 * Devices might be registered with both periodic and oneshot
	 * mode disabled. This signals, that the device needs to be
	 * operated from the broadcast device and is a placeholder for
	 * the cpu local device.
	 */
	if (!tick_device_is_functional(dev)) {
		dev->event_handler = tick_handle_periodic;
<<<<<<< HEAD
		cpumask_set_cpu(cpu, tick_get_broadcast_mask());
		tick_broadcast_start_periodic(tick_broadcast_device.evtdev);
		ret = 1;
	} else {
		/*
		 * When the new device is not affected by the stop
		 * feature and the cpu is marked in the broadcast mask
		 * then clear the broadcast bit.
		 */
		if (!(dev->features & CLOCK_EVT_FEAT_C3STOP)) {
			int cpu = smp_processor_id();

			cpumask_clear_cpu(cpu, tick_get_broadcast_mask());
			tick_broadcast_clear_oneshot(cpu);
=======
		tick_device_setup_broadcast_func(dev);
		cpumask_set_cpu(cpu, tick_broadcast_mask);
		if (tick_broadcast_device.mode == TICKDEV_MODE_PERIODIC)
			tick_broadcast_start_periodic(bc);
		else
			tick_broadcast_setup_oneshot(bc, false);
		ret = 1;
	} else {
		/*
		 * Clear the broadcast bit for this cpu if the
		 * device is not power state affected.
		 */
		if (!(dev->features & CLOCK_EVT_FEAT_C3STOP))
			cpumask_clear_cpu(cpu, tick_broadcast_mask);
		else
			tick_device_setup_broadcast_func(dev);

		/*
		 * Clear the broadcast bit if the CPU is not in
		 * periodic broadcast on state.
		 */
		if (!cpumask_test_cpu(cpu, tick_broadcast_on))
			cpumask_clear_cpu(cpu, tick_broadcast_mask);

		switch (tick_broadcast_device.mode) {
		case TICKDEV_MODE_ONESHOT:
			/*
			 * If the system is in oneshot mode we can
			 * unconditionally clear the oneshot mask bit,
			 * because the CPU is running and therefore
			 * not in an idle state which causes the power
			 * state affected device to stop. Let the
			 * caller initialize the device.
			 */
			tick_broadcast_clear_oneshot(cpu);
			ret = 0;
			break;

		case TICKDEV_MODE_PERIODIC:
			/*
			 * If the system is in periodic mode, check
			 * whether the broadcast device can be
			 * switched off now.
			 */
			if (cpumask_empty(tick_broadcast_mask) && bc)
				clockevents_shutdown(bc);
			/*
			 * If we kept the cpu in the broadcast mask,
			 * tell the caller to leave the per cpu device
			 * in shutdown state. The periodic interrupt
			 * is delivered by the broadcast device, if
			 * the broadcast device exists and is not
			 * hrtimer based.
			 */
			if (bc && !(bc->features & CLOCK_EVT_FEAT_HRTIMER))
				ret = cpumask_test_cpu(cpu, tick_broadcast_mask);
			break;
		default:
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	raw_spin_unlock_irqrestore(&tick_broadcast_lock, flags);
	return ret;
}

<<<<<<< HEAD
/*
 * Broadcast the event to the cpus, which are set in the mask (mangled).
 */
static void tick_do_broadcast(struct cpumask *mask)
{
	int cpu = smp_processor_id();
	struct tick_device *td;
=======
int tick_receive_broadcast(void)
{
	struct tick_device *td = this_cpu_ptr(&tick_cpu_device);
	struct clock_event_device *evt = td->evtdev;

	if (!evt)
		return -ENODEV;

	if (!evt->event_handler)
		return -EINVAL;

	evt->event_handler(evt);
	return 0;
}

/*
 * Broadcast the event to the cpus, which are set in the mask (mangled).
 */
static bool tick_do_broadcast(struct cpumask *mask)
{
	int cpu = smp_processor_id();
	struct tick_device *td;
	bool local = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Check, if the current cpu is in the mask
	 */
	if (cpumask_test_cpu(cpu, mask)) {
<<<<<<< HEAD
		cpumask_clear_cpu(cpu, mask);
		td = &per_cpu(tick_cpu_device, cpu);
		td->evtdev->event_handler(td->evtdev);
=======
		struct clock_event_device *bc = tick_broadcast_device.evtdev;

		cpumask_clear_cpu(cpu, mask);
		/*
		 * We only run the local handler, if the broadcast
		 * device is not hrtimer based. Otherwise we run into
		 * a hrtimer recursion.
		 *
		 * local timer_interrupt()
		 *   local_handler()
		 *     expire_hrtimers()
		 *       bc_handler()
		 *         local_handler()
		 *	     expire_hrtimers()
		 */
		local = !(bc->features & CLOCK_EVT_FEAT_HRTIMER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!cpumask_empty(mask)) {
		/*
		 * It might be necessary to actually check whether the devices
		 * have different broadcast functions. For now, just use the
		 * one of the first device. This works as long as we have this
		 * misfeature only on x86 (lapic)
		 */
		td = &per_cpu(tick_cpu_device, cpumask_first(mask));
		td->evtdev->broadcast(mask);
	}
<<<<<<< HEAD
=======
	return local;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Periodic broadcast:
 * - invoke the broadcast handlers
 */
<<<<<<< HEAD
static void tick_do_periodic_broadcast(void)
{
	raw_spin_lock(&tick_broadcast_lock);

	cpumask_and(to_cpumask(tmpmask),
		    cpu_online_mask, tick_get_broadcast_mask());
	tick_do_broadcast(to_cpumask(tmpmask));

	raw_spin_unlock(&tick_broadcast_lock);
=======
static bool tick_do_periodic_broadcast(void)
{
	cpumask_and(tmpmask, cpu_online_mask, tick_broadcast_mask);
	return tick_do_broadcast(tmpmask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Event handler for periodic broadcast ticks
 */
static void tick_handle_periodic_broadcast(struct clock_event_device *dev)
{
<<<<<<< HEAD
	ktime_t next;

	tick_do_periodic_broadcast();

	/*
	 * The device is in periodic mode. No reprogramming necessary:
	 */
	if (dev->mode == CLOCK_EVT_MODE_PERIODIC)
		return;

	/*
	 * Setup the next period for devices, which do not have
	 * periodic mode. We read dev->next_event first and add to it
	 * when the event already expired. clockevents_program_event()
	 * sets dev->next_event only when the event is really
	 * programmed to the device.
	 */
	for (next = dev->next_event; ;) {
		next = ktime_add(next, tick_period);

		if (!clockevents_program_event(dev, next, false))
			return;
		tick_do_periodic_broadcast();
	}
}

/*
 * Powerstate information: The system enters/leaves a state, where
 * affected devices might stop
 */
static void tick_do_broadcast_on_off(unsigned long *reason)
{
	struct clock_event_device *bc, *dev;
	struct tick_device *td;
	unsigned long flags;
	int cpu, bc_stopped;

	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);

	cpu = smp_processor_id();
	td = &per_cpu(tick_cpu_device, cpu);
	dev = td->evtdev;
	bc = tick_broadcast_device.evtdev;
=======
	struct tick_device *td = this_cpu_ptr(&tick_cpu_device);
	bool bc_local;

	raw_spin_lock(&tick_broadcast_lock);

	/* Handle spurious interrupts gracefully */
	if (clockevent_state_shutdown(tick_broadcast_device.evtdev)) {
		raw_spin_unlock(&tick_broadcast_lock);
		return;
	}

	bc_local = tick_do_periodic_broadcast();

	if (clockevent_state_oneshot(dev)) {
		ktime_t next = ktime_add_ns(dev->next_event, TICK_NSEC);

		clockevents_program_event(dev, next, true);
	}
	raw_spin_unlock(&tick_broadcast_lock);

	/*
	 * We run the handler of the local cpu after dropping
	 * tick_broadcast_lock because the handler might deadlock when
	 * trying to switch to oneshot mode.
	 */
	if (bc_local)
		td->evtdev->event_handler(td->evtdev);
}

/**
 * tick_broadcast_control - Enable/disable or force broadcast mode
 * @mode:	The selected broadcast mode
 *
 * Called when the system enters a state where affected tick devices
 * might stop. Note: TICK_BROADCAST_FORCE cannot be undone.
 */
void tick_broadcast_control(enum tick_broadcast_mode mode)
{
	struct clock_event_device *bc, *dev;
	struct tick_device *td;
	int cpu, bc_stopped;
	unsigned long flags;

	/* Protects also the local clockevent device. */
	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);
	td = this_cpu_ptr(&tick_cpu_device);
	dev = td->evtdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Is the device not affected by the powerstate ?
	 */
	if (!dev || !(dev->features & CLOCK_EVT_FEAT_C3STOP))
		goto out;

	if (!tick_device_is_functional(dev))
		goto out;

<<<<<<< HEAD
	bc_stopped = cpumask_empty(tick_get_broadcast_mask());

	switch (*reason) {
	case CLOCK_EVT_NOTIFY_BROADCAST_ON:
	case CLOCK_EVT_NOTIFY_BROADCAST_FORCE:
		if (!cpumask_test_cpu(cpu, tick_get_broadcast_mask())) {
			cpumask_set_cpu(cpu, tick_get_broadcast_mask());
			if (tick_broadcast_device.mode ==
			    TICKDEV_MODE_PERIODIC)
				clockevents_shutdown(dev);
		}
		if (*reason == CLOCK_EVT_NOTIFY_BROADCAST_FORCE)
			tick_broadcast_force = 1;
		break;
	case CLOCK_EVT_NOTIFY_BROADCAST_OFF:
		if (!tick_broadcast_force &&
		    cpumask_test_cpu(cpu, tick_get_broadcast_mask())) {
			cpumask_clear_cpu(cpu, tick_get_broadcast_mask());
=======
	cpu = smp_processor_id();
	bc = tick_broadcast_device.evtdev;
	bc_stopped = cpumask_empty(tick_broadcast_mask);

	switch (mode) {
	case TICK_BROADCAST_FORCE:
		tick_broadcast_forced = 1;
		fallthrough;
	case TICK_BROADCAST_ON:
		cpumask_set_cpu(cpu, tick_broadcast_on);
		if (!cpumask_test_and_set_cpu(cpu, tick_broadcast_mask)) {
			/*
			 * Only shutdown the cpu local device, if:
			 *
			 * - the broadcast device exists
			 * - the broadcast device is not a hrtimer based one
			 * - the broadcast device is in periodic mode to
			 *   avoid a hiccup during switch to oneshot mode
			 */
			if (bc && !(bc->features & CLOCK_EVT_FEAT_HRTIMER) &&
			    tick_broadcast_device.mode == TICKDEV_MODE_PERIODIC)
				clockevents_shutdown(dev);
		}
		break;

	case TICK_BROADCAST_OFF:
		if (tick_broadcast_forced)
			break;
		cpumask_clear_cpu(cpu, tick_broadcast_on);
		if (cpumask_test_and_clear_cpu(cpu, tick_broadcast_mask)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (tick_broadcast_device.mode ==
			    TICKDEV_MODE_PERIODIC)
				tick_setup_periodic(dev, 0);
		}
		break;
	}

<<<<<<< HEAD
	if (cpumask_empty(tick_get_broadcast_mask())) {
		if (!bc_stopped)
			clockevents_shutdown(bc);
	} else if (bc_stopped) {
		if (tick_broadcast_device.mode == TICKDEV_MODE_PERIODIC)
			tick_broadcast_start_periodic(bc);
		else
			tick_broadcast_setup_oneshot(bc);
=======
	if (bc) {
		if (cpumask_empty(tick_broadcast_mask)) {
			if (!bc_stopped)
				clockevents_shutdown(bc);
		} else if (bc_stopped) {
			if (tick_broadcast_device.mode == TICKDEV_MODE_PERIODIC)
				tick_broadcast_start_periodic(bc);
			else
				tick_broadcast_setup_oneshot(bc, false);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
out:
	raw_spin_unlock_irqrestore(&tick_broadcast_lock, flags);
}
<<<<<<< HEAD

/*
 * Powerstate information: The system enters/leaves a state, where
 * affected devices might stop.
 */
void tick_broadcast_on_off(unsigned long reason, int *oncpu)
{
	if (!cpumask_test_cpu(*oncpu, cpu_online_mask))
		printk(KERN_ERR "tick-broadcast: ignoring broadcast for "
		       "offline CPU #%d\n", *oncpu);
	else
		tick_do_broadcast_on_off(&reason);
}
=======
EXPORT_SYMBOL_GPL(tick_broadcast_control);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Set the periodic handler depending on broadcast on/off
 */
void tick_set_periodic_handler(struct clock_event_device *dev, int broadcast)
{
	if (!broadcast)
		dev->event_handler = tick_handle_periodic;
	else
		dev->event_handler = tick_handle_periodic_broadcast;
}

<<<<<<< HEAD
/*
 * Remove a CPU from broadcasting
 */
void tick_shutdown_broadcast(unsigned int *cpup)
{
	struct clock_event_device *bc;
	unsigned long flags;
	unsigned int cpu = *cpup;

	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);

	bc = tick_broadcast_device.evtdev;
	cpumask_clear_cpu(cpu, tick_get_broadcast_mask());

	if (tick_broadcast_device.mode == TICKDEV_MODE_PERIODIC) {
		if (bc && cpumask_empty(tick_get_broadcast_mask()))
			clockevents_shutdown(bc);
	}

	raw_spin_unlock_irqrestore(&tick_broadcast_lock, flags);
}

=======
#ifdef CONFIG_HOTPLUG_CPU
static void tick_shutdown_broadcast(void)
{
	struct clock_event_device *bc = tick_broadcast_device.evtdev;

	if (tick_broadcast_device.mode == TICKDEV_MODE_PERIODIC) {
		if (bc && cpumask_empty(tick_broadcast_mask))
			clockevents_shutdown(bc);
	}
}

/*
 * Remove a CPU from broadcasting
 */
void tick_broadcast_offline(unsigned int cpu)
{
	raw_spin_lock(&tick_broadcast_lock);
	cpumask_clear_cpu(cpu, tick_broadcast_mask);
	cpumask_clear_cpu(cpu, tick_broadcast_on);
	tick_broadcast_oneshot_offline(cpu);
	tick_shutdown_broadcast();
	raw_spin_unlock(&tick_broadcast_lock);
}

#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void tick_suspend_broadcast(void)
{
	struct clock_event_device *bc;
	unsigned long flags;

	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);

	bc = tick_broadcast_device.evtdev;
	if (bc)
		clockevents_shutdown(bc);

	raw_spin_unlock_irqrestore(&tick_broadcast_lock, flags);
}

<<<<<<< HEAD
int tick_resume_broadcast(void)
{
	struct clock_event_device *bc;
	unsigned long flags;
	int broadcast = 0;
=======
/*
 * This is called from tick_resume_local() on a resuming CPU. That's
 * called from the core resume function, tick_unfreeze() and the magic XEN
 * resume hackery.
 *
 * In none of these cases the broadcast device mode can change and the
 * bit of the resuming CPU in the broadcast mask is safe as well.
 */
bool tick_resume_check_broadcast(void)
{
	if (tick_broadcast_device.mode == TICKDEV_MODE_ONESHOT)
		return false;
	else
		return cpumask_test_cpu(smp_processor_id(), tick_broadcast_mask);
}

void tick_resume_broadcast(void)
{
	struct clock_event_device *bc;
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);

	bc = tick_broadcast_device.evtdev;

	if (bc) {
<<<<<<< HEAD
		clockevents_set_mode(bc, CLOCK_EVT_MODE_RESUME);

		switch (tick_broadcast_device.mode) {
		case TICKDEV_MODE_PERIODIC:
			if (!cpumask_empty(tick_get_broadcast_mask()))
				tick_broadcast_start_periodic(bc);
			broadcast = cpumask_test_cpu(smp_processor_id(),
						     tick_get_broadcast_mask());
			break;
		case TICKDEV_MODE_ONESHOT:
			if (!cpumask_empty(tick_get_broadcast_mask()))
				broadcast = tick_resume_broadcast_oneshot(bc);
=======
		clockevents_tick_resume(bc);

		switch (tick_broadcast_device.mode) {
		case TICKDEV_MODE_PERIODIC:
			if (!cpumask_empty(tick_broadcast_mask))
				tick_broadcast_start_periodic(bc);
			break;
		case TICKDEV_MODE_ONESHOT:
			if (!cpumask_empty(tick_broadcast_mask))
				tick_resume_broadcast_oneshot(bc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}
	raw_spin_unlock_irqrestore(&tick_broadcast_lock, flags);
<<<<<<< HEAD

	return broadcast;
}


#ifdef CONFIG_TICK_ONESHOT

/* FIXME: use cpumask_var_t. */
static DECLARE_BITMAP(tick_broadcast_oneshot_mask, NR_CPUS);
=======
}

#ifdef CONFIG_TICK_ONESHOT

static cpumask_var_t tick_broadcast_oneshot_mask __cpumask_var_read_mostly;
static cpumask_var_t tick_broadcast_pending_mask __cpumask_var_read_mostly;
static cpumask_var_t tick_broadcast_force_mask __cpumask_var_read_mostly;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Exposed for debugging: see timer_list.c
 */
struct cpumask *tick_get_broadcast_oneshot_mask(void)
{
<<<<<<< HEAD
	return to_cpumask(tick_broadcast_oneshot_mask);
=======
	return tick_broadcast_oneshot_mask;
}

/*
 * Called before going idle with interrupts disabled. Checks whether a
 * broadcast event from the other core is about to happen. We detected
 * that in tick_broadcast_oneshot_control(). The callsite can use this
 * to avoid a deep idle transition as we are about to get the
 * broadcast IPI right away.
 */
noinstr int tick_check_broadcast_expired(void)
{
#ifdef _ASM_GENERIC_BITOPS_INSTRUMENTED_NON_ATOMIC_H
	return arch_test_bit(smp_processor_id(), cpumask_bits(tick_broadcast_force_mask));
#else
	return cpumask_test_cpu(smp_processor_id(), tick_broadcast_force_mask);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Set broadcast interrupt affinity
 */
static void tick_broadcast_set_affinity(struct clock_event_device *bc,
					const struct cpumask *cpumask)
{
	if (!(bc->features & CLOCK_EVT_FEAT_DYNIRQ))
		return;

	if (cpumask_equal(bc->cpumask, cpumask))
		return;

	bc->cpumask = cpumask;
	irq_set_affinity(bc->irq, bc->cpumask);
}

<<<<<<< HEAD
static int tick_broadcast_set_event(struct clock_event_device *bc, int cpu,
				    ktime_t expires, int force)
{
	int ret;

	if (bc->mode != CLOCK_EVT_MODE_ONESHOT)
		clockevents_set_mode(bc, CLOCK_EVT_MODE_ONESHOT);

	ret = clockevents_program_event(bc, expires, force);
	if (!ret)
		tick_broadcast_set_affinity(bc, cpumask_of(cpu));
	return ret;
}

int tick_resume_broadcast_oneshot(struct clock_event_device *bc)
{
	clockevents_set_mode(bc, CLOCK_EVT_MODE_ONESHOT);
	return 0;
=======
static void tick_broadcast_set_event(struct clock_event_device *bc, int cpu,
				     ktime_t expires)
{
	if (!clockevent_state_oneshot(bc))
		clockevents_switch_state(bc, CLOCK_EVT_STATE_ONESHOT);

	clockevents_program_event(bc, expires, 1);
	tick_broadcast_set_affinity(bc, cpumask_of(cpu));
}

static void tick_resume_broadcast_oneshot(struct clock_event_device *bc)
{
	clockevents_switch_state(bc, CLOCK_EVT_STATE_ONESHOT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Called from irq_enter() when idle was interrupted to reenable the
 * per cpu device.
 */
<<<<<<< HEAD
void tick_check_oneshot_broadcast(int cpu)
{
	if (cpumask_test_cpu(cpu, to_cpumask(tick_broadcast_oneshot_mask))) {
		struct tick_device *td = &per_cpu(tick_cpu_device, cpu);
=======
void tick_check_oneshot_broadcast_this_cpu(void)
{
	if (cpumask_test_cpu(smp_processor_id(), tick_broadcast_oneshot_mask)) {
		struct tick_device *td = this_cpu_ptr(&tick_cpu_device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * We might be in the middle of switching over from
		 * periodic to oneshot. If the CPU has not yet
		 * switched over, leave the device alone.
		 */
		if (td->mode == TICKDEV_MODE_ONESHOT) {
<<<<<<< HEAD
			clockevents_set_mode(td->evtdev,
					     CLOCK_EVT_MODE_ONESHOT);
=======
			clockevents_switch_state(td->evtdev,
					      CLOCK_EVT_STATE_ONESHOT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

/*
 * Handle oneshot mode broadcasting
 */
static void tick_handle_oneshot_broadcast(struct clock_event_device *dev)
{
	struct tick_device *td;
	ktime_t now, next_event;
	int cpu, next_cpu = 0;
<<<<<<< HEAD

	raw_spin_lock(&tick_broadcast_lock);
again:
	dev->next_event.tv64 = KTIME_MAX;
	next_event.tv64 = KTIME_MAX;
	cpumask_clear(to_cpumask(tmpmask));
	now = ktime_get();
	/* Find all expired events */
	for_each_cpu(cpu, tick_get_broadcast_oneshot_mask()) {
		td = &per_cpu(tick_cpu_device, cpu);
		if (td->evtdev->next_event.tv64 <= now.tv64) {
			cpumask_set_cpu(cpu, to_cpumask(tmpmask));
		} else if (td->evtdev->next_event.tv64 < next_event.tv64) {
			next_event.tv64 = td->evtdev->next_event.tv64;
=======
	bool bc_local;

	raw_spin_lock(&tick_broadcast_lock);
	dev->next_event = KTIME_MAX;
	next_event = KTIME_MAX;
	cpumask_clear(tmpmask);
	now = ktime_get();
	/* Find all expired events */
	for_each_cpu(cpu, tick_broadcast_oneshot_mask) {
		/*
		 * Required for !SMP because for_each_cpu() reports
		 * unconditionally CPU0 as set on UP kernels.
		 */
		if (!IS_ENABLED(CONFIG_SMP) &&
		    cpumask_empty(tick_broadcast_oneshot_mask))
			break;

		td = &per_cpu(tick_cpu_device, cpu);
		if (td->evtdev->next_event <= now) {
			cpumask_set_cpu(cpu, tmpmask);
			/*
			 * Mark the remote cpu in the pending mask, so
			 * it can avoid reprogramming the cpu local
			 * timer in tick_broadcast_oneshot_control().
			 */
			cpumask_set_cpu(cpu, tick_broadcast_pending_mask);
		} else if (td->evtdev->next_event < next_event) {
			next_event = td->evtdev->next_event;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			next_cpu = cpu;
		}
	}

	/*
<<<<<<< HEAD
	 * Wakeup the cpus which have an expired event.
	 */
	tick_do_broadcast(to_cpumask(tmpmask));
=======
	 * Remove the current cpu from the pending mask. The event is
	 * delivered immediately in tick_do_broadcast() !
	 */
	cpumask_clear_cpu(smp_processor_id(), tick_broadcast_pending_mask);

	/* Take care of enforced broadcast requests */
	cpumask_or(tmpmask, tmpmask, tick_broadcast_force_mask);
	cpumask_clear(tick_broadcast_force_mask);

	/*
	 * Sanity check. Catch the case where we try to broadcast to
	 * offline cpus.
	 */
	if (WARN_ON_ONCE(!cpumask_subset(tmpmask, cpu_online_mask)))
		cpumask_and(tmpmask, tmpmask, cpu_online_mask);

	/*
	 * Wakeup the cpus which have an expired event.
	 */
	bc_local = tick_do_broadcast(tmpmask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Two reasons for reprogram:
	 *
	 * - The global event did not expire any CPU local
	 * events. This happens in dyntick mode, as the maximum PIT
	 * delta is quite small.
	 *
	 * - There are pending events on sleeping CPUs which were not
	 * in the event mask
	 */
<<<<<<< HEAD
	if (next_event.tv64 != KTIME_MAX) {
		/*
		 * Rearm the broadcast device. If event expired,
		 * repeat the above
		 */
		if (tick_broadcast_set_event(dev, next_cpu, next_event, 0))
			goto again;
	}
	raw_spin_unlock(&tick_broadcast_lock);
}

/*
 * Powerstate information: The system enters/leaves a state, where
 * affected devices might stop
 */
void tick_broadcast_oneshot_control(unsigned long reason)
{
	struct clock_event_device *bc, *dev;
	struct tick_device *td;
	unsigned long flags;
	int cpu;

	/*
	 * Periodic mode does not care about the enter/exit of power
	 * states
	 */
	if (tick_broadcast_device.mode == TICKDEV_MODE_PERIODIC)
		return;

	/*
	 * We are called with preemtion disabled from the depth of the
	 * idle code, so we can't be moved away.
	 */
	cpu = smp_processor_id();
	td = &per_cpu(tick_cpu_device, cpu);
	dev = td->evtdev;

	if (!(dev->features & CLOCK_EVT_FEAT_C3STOP))
		return;

	bc = tick_broadcast_device.evtdev;

	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);
	if (reason == CLOCK_EVT_NOTIFY_BROADCAST_ENTER) {
		if (!cpumask_test_cpu(cpu, tick_get_broadcast_oneshot_mask())) {
			cpumask_set_cpu(cpu, tick_get_broadcast_oneshot_mask());
			clockevents_set_mode(dev, CLOCK_EVT_MODE_SHUTDOWN);
			if (dev->next_event.tv64 < bc->next_event.tv64)
				tick_broadcast_set_event(bc, cpu, dev->next_event, 1);
		}
	} else {
		if (cpumask_test_cpu(cpu, tick_get_broadcast_oneshot_mask())) {
			cpumask_clear_cpu(cpu,
					  tick_get_broadcast_oneshot_mask());
			clockevents_set_mode(dev, CLOCK_EVT_MODE_ONESHOT);
			if (dev->next_event.tv64 != KTIME_MAX)
				tick_program_event(dev->next_event, 1);
		}
	}
	raw_spin_unlock_irqrestore(&tick_broadcast_lock, flags);
=======
	if (next_event != KTIME_MAX)
		tick_broadcast_set_event(dev, next_cpu, next_event);

	raw_spin_unlock(&tick_broadcast_lock);

	if (bc_local) {
		td = this_cpu_ptr(&tick_cpu_device);
		td->evtdev->event_handler(td->evtdev);
	}
}

static int broadcast_needs_cpu(struct clock_event_device *bc, int cpu)
{
	if (!(bc->features & CLOCK_EVT_FEAT_HRTIMER))
		return 0;
	if (bc->next_event == KTIME_MAX)
		return 0;
	return bc->bound_on == cpu ? -EBUSY : 0;
}

static void broadcast_shutdown_local(struct clock_event_device *bc,
				     struct clock_event_device *dev)
{
	/*
	 * For hrtimer based broadcasting we cannot shutdown the cpu
	 * local device if our own event is the first one to expire or
	 * if we own the broadcast timer.
	 */
	if (bc->features & CLOCK_EVT_FEAT_HRTIMER) {
		if (broadcast_needs_cpu(bc, smp_processor_id()))
			return;
		if (dev->next_event < bc->next_event)
			return;
	}
	clockevents_switch_state(dev, CLOCK_EVT_STATE_SHUTDOWN);
}

static int ___tick_broadcast_oneshot_control(enum tick_broadcast_state state,
					     struct tick_device *td,
					     int cpu)
{
	struct clock_event_device *bc, *dev = td->evtdev;
	int ret = 0;
	ktime_t now;

	raw_spin_lock(&tick_broadcast_lock);
	bc = tick_broadcast_device.evtdev;

	if (state == TICK_BROADCAST_ENTER) {
		/*
		 * If the current CPU owns the hrtimer broadcast
		 * mechanism, it cannot go deep idle and we do not add
		 * the CPU to the broadcast mask. We don't have to go
		 * through the EXIT path as the local timer is not
		 * shutdown.
		 */
		ret = broadcast_needs_cpu(bc, cpu);
		if (ret)
			goto out;

		/*
		 * If the broadcast device is in periodic mode, we
		 * return.
		 */
		if (tick_broadcast_device.mode == TICKDEV_MODE_PERIODIC) {
			/* If it is a hrtimer based broadcast, return busy */
			if (bc->features & CLOCK_EVT_FEAT_HRTIMER)
				ret = -EBUSY;
			goto out;
		}

		if (!cpumask_test_and_set_cpu(cpu, tick_broadcast_oneshot_mask)) {
			WARN_ON_ONCE(cpumask_test_cpu(cpu, tick_broadcast_pending_mask));

			/* Conditionally shut down the local timer. */
			broadcast_shutdown_local(bc, dev);

			/*
			 * We only reprogram the broadcast timer if we
			 * did not mark ourself in the force mask and
			 * if the cpu local event is earlier than the
			 * broadcast event. If the current CPU is in
			 * the force mask, then we are going to be
			 * woken by the IPI right away; we return
			 * busy, so the CPU does not try to go deep
			 * idle.
			 */
			if (cpumask_test_cpu(cpu, tick_broadcast_force_mask)) {
				ret = -EBUSY;
			} else if (dev->next_event < bc->next_event) {
				tick_broadcast_set_event(bc, cpu, dev->next_event);
				/*
				 * In case of hrtimer broadcasts the
				 * programming might have moved the
				 * timer to this cpu. If yes, remove
				 * us from the broadcast mask and
				 * return busy.
				 */
				ret = broadcast_needs_cpu(bc, cpu);
				if (ret) {
					cpumask_clear_cpu(cpu,
						tick_broadcast_oneshot_mask);
				}
			}
		}
	} else {
		if (cpumask_test_and_clear_cpu(cpu, tick_broadcast_oneshot_mask)) {
			clockevents_switch_state(dev, CLOCK_EVT_STATE_ONESHOT);
			/*
			 * The cpu which was handling the broadcast
			 * timer marked this cpu in the broadcast
			 * pending mask and fired the broadcast
			 * IPI. So we are going to handle the expired
			 * event anyway via the broadcast IPI
			 * handler. No need to reprogram the timer
			 * with an already expired event.
			 */
			if (cpumask_test_and_clear_cpu(cpu,
				       tick_broadcast_pending_mask))
				goto out;

			/*
			 * Bail out if there is no next event.
			 */
			if (dev->next_event == KTIME_MAX)
				goto out;
			/*
			 * If the pending bit is not set, then we are
			 * either the CPU handling the broadcast
			 * interrupt or we got woken by something else.
			 *
			 * We are no longer in the broadcast mask, so
			 * if the cpu local expiry time is already
			 * reached, we would reprogram the cpu local
			 * timer with an already expired event.
			 *
			 * This can lead to a ping-pong when we return
			 * to idle and therefore rearm the broadcast
			 * timer before the cpu local timer was able
			 * to fire. This happens because the forced
			 * reprogramming makes sure that the event
			 * will happen in the future and depending on
			 * the min_delta setting this might be far
			 * enough out that the ping-pong starts.
			 *
			 * If the cpu local next_event has expired
			 * then we know that the broadcast timer
			 * next_event has expired as well and
			 * broadcast is about to be handled. So we
			 * avoid reprogramming and enforce that the
			 * broadcast handler, which did not run yet,
			 * will invoke the cpu local handler.
			 *
			 * We cannot call the handler directly from
			 * here, because we might be in a NOHZ phase
			 * and we did not go through the irq_enter()
			 * nohz fixups.
			 */
			now = ktime_get();
			if (dev->next_event <= now) {
				cpumask_set_cpu(cpu, tick_broadcast_force_mask);
				goto out;
			}
			/*
			 * We got woken by something else. Reprogram
			 * the cpu local timer device.
			 */
			tick_program_event(dev->next_event, 1);
		}
	}
out:
	raw_spin_unlock(&tick_broadcast_lock);
	return ret;
}

static int tick_oneshot_wakeup_control(enum tick_broadcast_state state,
				       struct tick_device *td,
				       int cpu)
{
	struct clock_event_device *dev, *wd;

	dev = td->evtdev;
	if (td->mode != TICKDEV_MODE_ONESHOT)
		return -EINVAL;

	wd = tick_get_oneshot_wakeup_device(cpu);
	if (!wd)
		return -ENODEV;

	switch (state) {
	case TICK_BROADCAST_ENTER:
		clockevents_switch_state(dev, CLOCK_EVT_STATE_ONESHOT_STOPPED);
		clockevents_switch_state(wd, CLOCK_EVT_STATE_ONESHOT);
		clockevents_program_event(wd, dev->next_event, 1);
		break;
	case TICK_BROADCAST_EXIT:
		/* We may have transitioned to oneshot mode while idle */
		if (clockevent_get_state(wd) != CLOCK_EVT_STATE_ONESHOT)
			return -ENODEV;
	}

	return 0;
}

int __tick_broadcast_oneshot_control(enum tick_broadcast_state state)
{
	struct tick_device *td = this_cpu_ptr(&tick_cpu_device);
	int cpu = smp_processor_id();

	if (!tick_oneshot_wakeup_control(state, td, cpu))
		return 0;

	if (tick_broadcast_device.evtdev)
		return ___tick_broadcast_oneshot_control(state, td, cpu);

	/*
	 * If there is no broadcast or wakeup device, tell the caller not
	 * to go into deep idle.
	 */
	return -EBUSY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Reset the one shot broadcast for a cpu
 *
 * Called with tick_broadcast_lock held
 */
static void tick_broadcast_clear_oneshot(int cpu)
{
<<<<<<< HEAD
	cpumask_clear_cpu(cpu, tick_get_broadcast_oneshot_mask());
=======
	cpumask_clear_cpu(cpu, tick_broadcast_oneshot_mask);
	cpumask_clear_cpu(cpu, tick_broadcast_pending_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void tick_broadcast_init_next_event(struct cpumask *mask,
					   ktime_t expires)
{
	struct tick_device *td;
	int cpu;

	for_each_cpu(cpu, mask) {
		td = &per_cpu(tick_cpu_device, cpu);
		if (td->evtdev)
			td->evtdev->next_event = expires;
	}
}

<<<<<<< HEAD
/**
 * tick_broadcast_setup_oneshot - setup the broadcast device
 */
void tick_broadcast_setup_oneshot(struct clock_event_device *bc)
{
	int cpu = smp_processor_id();

	/* Set it up only once ! */
	if (bc->event_handler != tick_handle_oneshot_broadcast) {
		int was_periodic = bc->mode == CLOCK_EVT_MODE_PERIODIC;

		bc->event_handler = tick_handle_oneshot_broadcast;

		/* Take the do_timer update */
		tick_do_timer_cpu = cpu;

		/*
		 * We must be careful here. There might be other CPUs
		 * waiting for periodic broadcast. We need to set the
		 * oneshot_mask bits for those and program the
		 * broadcast device to fire.
		 */
		cpumask_copy(to_cpumask(tmpmask), tick_get_broadcast_mask());
		cpumask_clear_cpu(cpu, to_cpumask(tmpmask));
		cpumask_or(tick_get_broadcast_oneshot_mask(),
			   tick_get_broadcast_oneshot_mask(),
			   to_cpumask(tmpmask));

		if (was_periodic && !cpumask_empty(to_cpumask(tmpmask))) {
			clockevents_set_mode(bc, CLOCK_EVT_MODE_ONESHOT);
			tick_broadcast_init_next_event(to_cpumask(tmpmask),
						       tick_next_period);
			tick_broadcast_set_event(bc, cpu, tick_next_period, 1);
		} else
			bc->next_event.tv64 = KTIME_MAX;
	} else {
		/*
		 * The first cpu which switches to oneshot mode sets
		 * the bit for all other cpus which are in the general
		 * (periodic) broadcast mask. So the bit is set and
		 * would prevent the first broadcast enter after this
		 * to program the bc device.
		 */
		tick_broadcast_clear_oneshot(cpu);
	}
=======
static inline ktime_t tick_get_next_period(void)
{
	ktime_t next;

	/*
	 * Protect against concurrent updates (store /load tearing on
	 * 32bit). It does not matter if the time is already in the
	 * past. The broadcast device which is about to be programmed will
	 * fire in any case.
	 */
	raw_spin_lock(&jiffies_lock);
	next = tick_next_period;
	raw_spin_unlock(&jiffies_lock);
	return next;
}

/**
 * tick_broadcast_setup_oneshot - setup the broadcast device
 */
static void tick_broadcast_setup_oneshot(struct clock_event_device *bc,
					 bool from_periodic)
{
	int cpu = smp_processor_id();
	ktime_t nexttick = 0;

	if (!bc)
		return;

	/*
	 * When the broadcast device was switched to oneshot by the first
	 * CPU handling the NOHZ change, the other CPUs will reach this
	 * code via hrtimer_run_queues() -> tick_check_oneshot_change()
	 * too. Set up the broadcast device only once!
	 */
	if (bc->event_handler == tick_handle_oneshot_broadcast) {
		/*
		 * The CPU which switched from periodic to oneshot mode
		 * set the broadcast oneshot bit for all other CPUs which
		 * are in the general (periodic) broadcast mask to ensure
		 * that CPUs which wait for the periodic broadcast are
		 * woken up.
		 *
		 * Clear the bit for the local CPU as the set bit would
		 * prevent the first tick_broadcast_enter() after this CPU
		 * switched to oneshot state to program the broadcast
		 * device.
		 *
		 * This code can also be reached via tick_broadcast_control(),
		 * but this cannot avoid the tick_broadcast_clear_oneshot()
		 * as that would break the periodic to oneshot transition of
		 * secondary CPUs. But that's harmless as the below only
		 * clears already cleared bits.
		 */
		tick_broadcast_clear_oneshot(cpu);
		return;
	}


	bc->event_handler = tick_handle_oneshot_broadcast;
	bc->next_event = KTIME_MAX;

	/*
	 * When the tick mode is switched from periodic to oneshot it must
	 * be ensured that CPUs which are waiting for periodic broadcast
	 * get their wake-up at the next tick.  This is achieved by ORing
	 * tick_broadcast_mask into tick_broadcast_oneshot_mask.
	 *
	 * For other callers, e.g. broadcast device replacement,
	 * tick_broadcast_oneshot_mask must not be touched as this would
	 * set bits for CPUs which are already NOHZ, but not idle. Their
	 * next tick_broadcast_enter() would observe the bit set and fail
	 * to update the expiry time and the broadcast event device.
	 */
	if (from_periodic) {
		cpumask_copy(tmpmask, tick_broadcast_mask);
		/* Remove the local CPU as it is obviously not idle */
		cpumask_clear_cpu(cpu, tmpmask);
		cpumask_or(tick_broadcast_oneshot_mask, tick_broadcast_oneshot_mask, tmpmask);

		/*
		 * Ensure that the oneshot broadcast handler will wake the
		 * CPUs which are still waiting for periodic broadcast.
		 */
		nexttick = tick_get_next_period();
		tick_broadcast_init_next_event(tmpmask, nexttick);

		/*
		 * If the underlying broadcast clock event device is
		 * already in oneshot state, then there is nothing to do.
		 * The device was already armed for the next tick
		 * in tick_handle_broadcast_periodic()
		 */
		if (clockevent_state_oneshot(bc))
			return;
	}

	/*
	 * When switching from periodic to oneshot mode arm the broadcast
	 * device for the next tick.
	 *
	 * If the broadcast device has been replaced in oneshot mode and
	 * the oneshot broadcast mask is not empty, then arm it to expire
	 * immediately in order to reevaluate the next expiring timer.
	 * @nexttick is 0 and therefore in the past which will cause the
	 * clockevent code to force an event.
	 *
	 * For both cases the programming can be avoided when the oneshot
	 * broadcast mask is empty.
	 *
	 * tick_broadcast_set_event() implicitly switches the broadcast
	 * device to oneshot state.
	 */
	if (!cpumask_empty(tick_broadcast_oneshot_mask))
		tick_broadcast_set_event(bc, cpu, nexttick);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Select oneshot operating mode for the broadcast device
 */
void tick_broadcast_switch_to_oneshot(void)
{
	struct clock_event_device *bc;
<<<<<<< HEAD
=======
	enum tick_device_mode oldmode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;

	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);

<<<<<<< HEAD
	tick_broadcast_device.mode = TICKDEV_MODE_ONESHOT;
	bc = tick_broadcast_device.evtdev;
	if (bc)
		tick_broadcast_setup_oneshot(bc);
=======
	oldmode = tick_broadcast_device.mode;
	tick_broadcast_device.mode = TICKDEV_MODE_ONESHOT;
	bc = tick_broadcast_device.evtdev;
	if (bc)
		tick_broadcast_setup_oneshot(bc, oldmode == TICKDEV_MODE_PERIODIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	raw_spin_unlock_irqrestore(&tick_broadcast_lock, flags);
}

<<<<<<< HEAD

/*
 * Remove a dead CPU from broadcasting
 */
void tick_shutdown_broadcast_oneshot(unsigned int *cpup)
{
	unsigned long flags;
	unsigned int cpu = *cpup;

	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);

	/*
	 * Clear the broadcast mask flag for the dead cpu, but do not
	 * stop the broadcast device!
	 */
	cpumask_clear_cpu(cpu, tick_get_broadcast_oneshot_mask());

=======
#ifdef CONFIG_HOTPLUG_CPU
void hotplug_cpu__broadcast_tick_pull(int deadcpu)
{
	struct clock_event_device *bc;
	unsigned long flags;

	raw_spin_lock_irqsave(&tick_broadcast_lock, flags);
	bc = tick_broadcast_device.evtdev;

	if (bc && broadcast_needs_cpu(bc, deadcpu)) {
		/* This moves the broadcast assignment to this CPU: */
		clockevents_program_event(bc, bc->next_event, 1);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	raw_spin_unlock_irqrestore(&tick_broadcast_lock, flags);
}

/*
<<<<<<< HEAD
=======
 * Remove a dying CPU from broadcasting
 */
static void tick_broadcast_oneshot_offline(unsigned int cpu)
{
	if (tick_get_oneshot_wakeup_device(cpu))
		tick_set_oneshot_wakeup_device(NULL, cpu);

	/*
	 * Clear the broadcast masks for the dead cpu, but do not stop
	 * the broadcast device!
	 */
	cpumask_clear_cpu(cpu, tick_broadcast_oneshot_mask);
	cpumask_clear_cpu(cpu, tick_broadcast_pending_mask);
	cpumask_clear_cpu(cpu, tick_broadcast_force_mask);
}
#endif

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Check, whether the broadcast device is in one shot mode
 */
int tick_broadcast_oneshot_active(void)
{
	return tick_broadcast_device.mode == TICKDEV_MODE_ONESHOT;
}

/*
 * Check whether the broadcast device supports oneshot.
 */
bool tick_broadcast_oneshot_available(void)
{
	struct clock_event_device *bc = tick_broadcast_device.evtdev;

	return bc ? bc->features & CLOCK_EVT_FEAT_ONESHOT : false;
}

<<<<<<< HEAD
#endif
=======
#else
int __tick_broadcast_oneshot_control(enum tick_broadcast_state state)
{
	struct clock_event_device *bc = tick_broadcast_device.evtdev;

	if (!bc || (bc->features & CLOCK_EVT_FEAT_HRTIMER))
		return -EBUSY;

	return 0;
}
#endif

void __init tick_broadcast_init(void)
{
	zalloc_cpumask_var(&tick_broadcast_mask, GFP_NOWAIT);
	zalloc_cpumask_var(&tick_broadcast_on, GFP_NOWAIT);
	zalloc_cpumask_var(&tmpmask, GFP_NOWAIT);
#ifdef CONFIG_TICK_ONESHOT
	zalloc_cpumask_var(&tick_broadcast_oneshot_mask, GFP_NOWAIT);
	zalloc_cpumask_var(&tick_broadcast_pending_mask, GFP_NOWAIT);
	zalloc_cpumask_var(&tick_broadcast_force_mask, GFP_NOWAIT);
#endif
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
