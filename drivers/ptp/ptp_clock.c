<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * PTP 1588 clock support
 *
 * Copyright (C) 2010 OMICRON electronics GmbH
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <linux/bitops.h>
=======
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/posix-clock.h>
#include <linux/pps_kernel.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
=======
#include <linux/debugfs.h>
#include <linux/xarray.h>
#include <uapi/linux/sched/types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "ptp_private.h"

#define PTP_MAX_ALARMS 4
<<<<<<< HEAD
#define PTP_MAX_CLOCKS 8
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PTP_PPS_DEFAULTS (PPS_CAPTUREASSERT | PPS_OFFSETASSERT)
#define PTP_PPS_EVENT PPS_CAPTUREASSERT
#define PTP_PPS_MODE (PTP_PPS_DEFAULTS | PPS_CANWAIT | PPS_TSFMT_TSPEC)

<<<<<<< HEAD
/* private globals */

static dev_t ptp_devt;
static struct class *ptp_class;

static DECLARE_BITMAP(ptp_clocks_map, PTP_MAX_CLOCKS);
static DEFINE_MUTEX(ptp_clocks_mutex); /* protects 'ptp_clocks_map' */
=======
const struct class ptp_class = {
	.name = "ptp",
	.dev_groups = ptp_groups
};

/* private globals */

static dev_t ptp_devt;

static DEFINE_XARRAY_ALLOC(ptp_clocks_map);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* time stamp event queue operations */

static inline int queue_free(struct timestamp_event_queue *q)
{
	return PTP_MAX_TIMESTAMPS - queue_cnt(q) - 1;
}

static void enqueue_external_timestamp(struct timestamp_event_queue *queue,
				       struct ptp_clock_event *src)
{
	struct ptp_extts_event *dst;
<<<<<<< HEAD
=======
	struct timespec64 offset_ts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	s64 seconds;
	u32 remainder;

<<<<<<< HEAD
	seconds = div_u64_rem(src->timestamp, 1000000000, &remainder);
=======
	if (src->type == PTP_CLOCK_EXTTS) {
		seconds = div_u64_rem(src->timestamp, 1000000000, &remainder);
	} else if (src->type == PTP_CLOCK_EXTOFF) {
		offset_ts = ns_to_timespec64(src->offset);
		seconds = offset_ts.tv_sec;
		remainder = offset_ts.tv_nsec;
	} else {
		WARN(1, "%s: unknown type %d\n", __func__, src->type);
		return;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&queue->lock, flags);

	dst = &queue->buf[queue->tail];
	dst->index = src->index;
<<<<<<< HEAD
	dst->t.sec = seconds;
	dst->t.nsec = remainder;

	if (!queue_free(queue))
		queue->head = (queue->head + 1) % PTP_MAX_TIMESTAMPS;

	queue->tail = (queue->tail + 1) % PTP_MAX_TIMESTAMPS;
=======
	dst->flags = PTP_EXTTS_EVENT_VALID;
	dst->t.sec = seconds;
	dst->t.nsec = remainder;
	if (src->type == PTP_CLOCK_EXTOFF)
		dst->flags |= PTP_EXT_OFFSET;

	/* Both WRITE_ONCE() are paired with READ_ONCE() in queue_cnt() */
	if (!queue_free(queue))
		WRITE_ONCE(queue->head, (queue->head + 1) % PTP_MAX_TIMESTAMPS);

	WRITE_ONCE(queue->tail, (queue->tail + 1) % PTP_MAX_TIMESTAMPS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock_irqrestore(&queue->lock, flags);
}

<<<<<<< HEAD
static s32 scaled_ppm_to_ppb(long ppm)
{
	/*
	 * The 'freq' field in the 'struct timex' is in parts per
	 * million, but with a 16 bit binary fractional field.
	 *
	 * We want to calculate
	 *
	 *    ppb = scaled_ppm * 1000 / 2^16
	 *
	 * which simplifies to
	 *
	 *    ppb = scaled_ppm * 125 / 2^13
	 */
	s64 ppb = 1 + ppm;
	ppb *= 125;
	ppb >>= 13;
	return (s32) ppb;
}

/* posix clock implementation */

static int ptp_clock_getres(struct posix_clock *pc, struct timespec *tp)
=======
/* posix clock implementation */

static int ptp_clock_getres(struct posix_clock *pc, struct timespec64 *tp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	tp->tv_sec = 0;
	tp->tv_nsec = 1;
	return 0;
}

<<<<<<< HEAD
static int ptp_clock_settime(struct posix_clock *pc, const struct timespec *tp)
{
	struct ptp_clock *ptp = container_of(pc, struct ptp_clock, clock);
	return ptp->info->settime(ptp->info, tp);
}

static int ptp_clock_gettime(struct posix_clock *pc, struct timespec *tp)
{
	struct ptp_clock *ptp = container_of(pc, struct ptp_clock, clock);
	return ptp->info->gettime(ptp->info, tp);
}

static int ptp_clock_adjtime(struct posix_clock *pc, struct timex *tx)
=======
static int ptp_clock_settime(struct posix_clock *pc, const struct timespec64 *tp)
{
	struct ptp_clock *ptp = container_of(pc, struct ptp_clock, clock);

	if (ptp_clock_freerun(ptp)) {
		pr_err("ptp: physical clock is free running\n");
		return -EBUSY;
	}

	return  ptp->info->settime64(ptp->info, tp);
}

static int ptp_clock_gettime(struct posix_clock *pc, struct timespec64 *tp)
{
	struct ptp_clock *ptp = container_of(pc, struct ptp_clock, clock);
	int err;

	if (ptp->info->gettimex64)
		err = ptp->info->gettimex64(ptp->info, tp, NULL);
	else
		err = ptp->info->gettime64(ptp->info, tp);
	return err;
}

static int ptp_clock_adjtime(struct posix_clock *pc, struct __kernel_timex *tx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ptp_clock *ptp = container_of(pc, struct ptp_clock, clock);
	struct ptp_clock_info *ops;
	int err = -EOPNOTSUPP;

<<<<<<< HEAD
	ops = ptp->info;

	if (tx->modes & ADJ_SETOFFSET) {
		struct timespec ts;
=======
	if (ptp_clock_freerun(ptp)) {
		pr_err("ptp: physical clock is free running\n");
		return -EBUSY;
	}

	ops = ptp->info;

	if (tx->modes & ADJ_SETOFFSET) {
		struct timespec64 ts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ktime_t kt;
		s64 delta;

		ts.tv_sec  = tx->time.tv_sec;
		ts.tv_nsec = tx->time.tv_usec;

		if (!(tx->modes & ADJ_NANO))
			ts.tv_nsec *= 1000;

		if ((unsigned long) ts.tv_nsec >= NSEC_PER_SEC)
			return -EINVAL;

<<<<<<< HEAD
		kt = timespec_to_ktime(ts);
		delta = ktime_to_ns(kt);
		err = ops->adjtime(ops, delta);

	} else if (tx->modes & ADJ_FREQUENCY) {

		err = ops->adjfreq(ops, scaled_ppm_to_ppb(tx->freq));
=======
		kt = timespec64_to_ktime(ts);
		delta = ktime_to_ns(kt);
		err = ops->adjtime(ops, delta);
	} else if (tx->modes & ADJ_FREQUENCY) {
		long ppb = scaled_ppm_to_ppb(tx->freq);
		if (ppb > ops->max_adj || ppb < -ops->max_adj)
			return -ERANGE;
		err = ops->adjfine(ops, tx->freq);
		ptp->dialed_frequency = tx->freq;
	} else if (tx->modes & ADJ_OFFSET) {
		if (ops->adjphase) {
			s32 max_phase_adj = ops->getmaxphase(ops);
			s32 offset = tx->offset;

			if (!(tx->modes & ADJ_NANO))
				offset *= NSEC_PER_USEC;

			if (offset > max_phase_adj || offset < -max_phase_adj)
				return -ERANGE;

			err = ops->adjphase(ops, offset);
		}
	} else if (tx->modes == 0) {
		tx->freq = ptp->dialed_frequency;
		err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

static struct posix_clock_operations ptp_clock_ops = {
	.owner		= THIS_MODULE,
	.clock_adjtime	= ptp_clock_adjtime,
	.clock_gettime	= ptp_clock_gettime,
	.clock_getres	= ptp_clock_getres,
	.clock_settime	= ptp_clock_settime,
	.ioctl		= ptp_ioctl,
	.open		= ptp_open,
<<<<<<< HEAD
=======
	.release	= ptp_release,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.poll		= ptp_poll,
	.read		= ptp_read,
};

<<<<<<< HEAD
static void delete_ptp_clock(struct posix_clock *pc)
{
	struct ptp_clock *ptp = container_of(pc, struct ptp_clock, clock);

	mutex_destroy(&ptp->tsevq_mux);

	/* Remove the clock from the bit map. */
	mutex_lock(&ptp_clocks_mutex);
	clear_bit(ptp->index, ptp_clocks_map);
	mutex_unlock(&ptp_clocks_mutex);

	kfree(ptp);
}

/* public interface */

struct ptp_clock *ptp_clock_register(struct ptp_clock_info *info)
{
	struct ptp_clock *ptp;
	int err = 0, index, major = MAJOR(ptp_devt);
=======
static void ptp_clock_release(struct device *dev)
{
	struct ptp_clock *ptp = container_of(dev, struct ptp_clock, dev);
	struct timestamp_event_queue *tsevq;
	unsigned long flags;

	ptp_cleanup_pin_groups(ptp);
	kfree(ptp->vclock_index);
	mutex_destroy(&ptp->pincfg_mux);
	mutex_destroy(&ptp->n_vclocks_mux);
	/* Delete first entry */
	spin_lock_irqsave(&ptp->tsevqs_lock, flags);
	tsevq = list_first_entry(&ptp->tsevqs, struct timestamp_event_queue,
				 qlist);
	list_del(&tsevq->qlist);
	spin_unlock_irqrestore(&ptp->tsevqs_lock, flags);
	bitmap_free(tsevq->mask);
	kfree(tsevq);
	debugfs_remove(ptp->debugfs_root);
	xa_erase(&ptp_clocks_map, ptp->index);
	kfree(ptp);
}

static int ptp_getcycles64(struct ptp_clock_info *info, struct timespec64 *ts)
{
	if (info->getcyclesx64)
		return info->getcyclesx64(info, ts, NULL);
	else
		return info->gettime64(info, ts);
}

static void ptp_aux_kworker(struct kthread_work *work)
{
	struct ptp_clock *ptp = container_of(work, struct ptp_clock,
					     aux_work.work);
	struct ptp_clock_info *info = ptp->info;
	long delay;

	delay = info->do_aux_work(info);

	if (delay >= 0)
		kthread_queue_delayed_work(ptp->kworker, &ptp->aux_work, delay);
}

/* public interface */

struct ptp_clock *ptp_clock_register(struct ptp_clock_info *info,
				     struct device *parent)
{
	struct ptp_clock *ptp;
	struct timestamp_event_queue *queue = NULL;
	int err, index, major = MAJOR(ptp_devt);
	char debugfsname[16];
	size_t size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (info->n_alarm > PTP_MAX_ALARMS)
		return ERR_PTR(-EINVAL);

<<<<<<< HEAD
	/* Find a free clock slot and reserve it. */
	err = -EBUSY;
	mutex_lock(&ptp_clocks_mutex);
	index = find_first_zero_bit(ptp_clocks_map, PTP_MAX_CLOCKS);
	if (index < PTP_MAX_CLOCKS)
		set_bit(index, ptp_clocks_map);
	else
		goto no_slot;

	/* Initialize a clock structure. */
	err = -ENOMEM;
	ptp = kzalloc(sizeof(struct ptp_clock), GFP_KERNEL);
	if (ptp == NULL)
		goto no_memory;

	ptp->clock.ops = ptp_clock_ops;
	ptp->clock.release = delete_ptp_clock;
	ptp->info = info;
	ptp->devid = MKDEV(major, index);
	ptp->index = index;
	spin_lock_init(&ptp->tsevq.lock);
	mutex_init(&ptp->tsevq_mux);
	init_waitqueue_head(&ptp->tsev_wq);

	/* Create a new device in our class. */
	ptp->dev = device_create(ptp_class, NULL, ptp->devid, ptp,
				 "ptp%d", ptp->index);
	if (IS_ERR(ptp->dev))
		goto no_device;

	dev_set_drvdata(ptp->dev, ptp);

	err = ptp_populate_sysfs(ptp);
	if (err)
		goto no_sysfs;
=======
	/* Initialize a clock structure. */
	ptp = kzalloc(sizeof(struct ptp_clock), GFP_KERNEL);
	if (!ptp) {
		err = -ENOMEM;
		goto no_memory;
	}

	err = xa_alloc(&ptp_clocks_map, &index, ptp, xa_limit_31b,
		       GFP_KERNEL);
	if (err)
		goto no_slot;

	ptp->clock.ops = ptp_clock_ops;
	ptp->info = info;
	ptp->devid = MKDEV(major, index);
	ptp->index = index;
	INIT_LIST_HEAD(&ptp->tsevqs);
	queue = kzalloc(sizeof(*queue), GFP_KERNEL);
	if (!queue) {
		err = -ENOMEM;
		goto no_memory_queue;
	}
	list_add_tail(&queue->qlist, &ptp->tsevqs);
	spin_lock_init(&ptp->tsevqs_lock);
	queue->mask = bitmap_alloc(PTP_MAX_CHANNELS, GFP_KERNEL);
	if (!queue->mask) {
		err = -ENOMEM;
		goto no_memory_bitmap;
	}
	bitmap_set(queue->mask, 0, PTP_MAX_CHANNELS);
	spin_lock_init(&queue->lock);
	mutex_init(&ptp->pincfg_mux);
	mutex_init(&ptp->n_vclocks_mux);
	init_waitqueue_head(&ptp->tsev_wq);

	if (ptp->info->getcycles64 || ptp->info->getcyclesx64) {
		ptp->has_cycles = true;
		if (!ptp->info->getcycles64 && ptp->info->getcyclesx64)
			ptp->info->getcycles64 = ptp_getcycles64;
	} else {
		/* Free running cycle counter not supported, use time. */
		ptp->info->getcycles64 = ptp_getcycles64;

		if (ptp->info->gettimex64)
			ptp->info->getcyclesx64 = ptp->info->gettimex64;

		if (ptp->info->getcrosststamp)
			ptp->info->getcrosscycles = ptp->info->getcrosststamp;
	}

	if (ptp->info->do_aux_work) {
		kthread_init_delayed_work(&ptp->aux_work, ptp_aux_kworker);
		ptp->kworker = kthread_create_worker(0, "ptp%d", ptp->index);
		if (IS_ERR(ptp->kworker)) {
			err = PTR_ERR(ptp->kworker);
			pr_err("failed to create ptp aux_worker %d\n", err);
			goto kworker_err;
		}
	}

	/* PTP virtual clock is being registered under physical clock */
	if (parent && parent->class && parent->class->name &&
	    strcmp(parent->class->name, "ptp") == 0)
		ptp->is_virtual_clock = true;

	if (!ptp->is_virtual_clock) {
		ptp->max_vclocks = PTP_DEFAULT_MAX_VCLOCKS;

		size = sizeof(int) * ptp->max_vclocks;
		ptp->vclock_index = kzalloc(size, GFP_KERNEL);
		if (!ptp->vclock_index) {
			err = -ENOMEM;
			goto no_mem_for_vclocks;
		}
	}

	err = ptp_populate_pin_groups(ptp);
	if (err)
		goto no_pin_groups;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Register a new PPS source. */
	if (info->pps) {
		struct pps_source_info pps;
		memset(&pps, 0, sizeof(pps));
		snprintf(pps.name, PPS_MAX_NAME_LEN, "ptp%d", index);
		pps.mode = PTP_PPS_MODE;
		pps.owner = info->owner;
		ptp->pps_source = pps_register_source(&pps, PTP_PPS_DEFAULTS);
<<<<<<< HEAD
		if (!ptp->pps_source) {
			pr_err("failed to register pps source\n");
			goto no_pps;
		}
	}

	/* Create a posix clock. */
	err = posix_clock_register(&ptp->clock, ptp->devid);
	if (err) {
		pr_err("failed to create posix clock\n");
		goto no_clock;
	}

	mutex_unlock(&ptp_clocks_mutex);
	return ptp;

no_clock:
	if (ptp->pps_source)
		pps_unregister_source(ptp->pps_source);
no_pps:
	ptp_cleanup_sysfs(ptp);
no_sysfs:
	device_destroy(ptp_class, ptp->devid);
no_device:
	mutex_destroy(&ptp->tsevq_mux);
	kfree(ptp);
no_memory:
	clear_bit(index, ptp_clocks_map);
no_slot:
	mutex_unlock(&ptp_clocks_mutex);
=======
		if (IS_ERR(ptp->pps_source)) {
			err = PTR_ERR(ptp->pps_source);
			pr_err("failed to register pps source\n");
			goto no_pps;
		}
		ptp->pps_source->lookup_cookie = ptp;
	}

	/* Initialize a new device of our class in our clock structure. */
	device_initialize(&ptp->dev);
	ptp->dev.devt = ptp->devid;
	ptp->dev.class = &ptp_class;
	ptp->dev.parent = parent;
	ptp->dev.groups = ptp->pin_attr_groups;
	ptp->dev.release = ptp_clock_release;
	dev_set_drvdata(&ptp->dev, ptp);
	dev_set_name(&ptp->dev, "ptp%d", ptp->index);

	/* Create a posix clock and link it to the device. */
	err = posix_clock_register(&ptp->clock, &ptp->dev);
	if (err) {
		if (ptp->pps_source)
			pps_unregister_source(ptp->pps_source);

		if (ptp->kworker)
			kthread_destroy_worker(ptp->kworker);

		put_device(&ptp->dev);

		pr_err("failed to create posix clock\n");
		return ERR_PTR(err);
	}

	/* Debugfs initialization */
	snprintf(debugfsname, sizeof(debugfsname), "ptp%d", ptp->index);
	ptp->debugfs_root = debugfs_create_dir(debugfsname, NULL);

	return ptp;

no_pps:
	ptp_cleanup_pin_groups(ptp);
no_pin_groups:
	kfree(ptp->vclock_index);
no_mem_for_vclocks:
	if (ptp->kworker)
		kthread_destroy_worker(ptp->kworker);
kworker_err:
	mutex_destroy(&ptp->pincfg_mux);
	mutex_destroy(&ptp->n_vclocks_mux);
	bitmap_free(queue->mask);
no_memory_bitmap:
	list_del(&queue->qlist);
	kfree(queue);
no_memory_queue:
	xa_erase(&ptp_clocks_map, index);
no_slot:
	kfree(ptp);
no_memory:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ERR_PTR(err);
}
EXPORT_SYMBOL(ptp_clock_register);

<<<<<<< HEAD
int ptp_clock_unregister(struct ptp_clock *ptp)
{
	ptp->defunct = 1;
	wake_up_interruptible(&ptp->tsev_wq);

	/* Release the clock's resources. */
	if (ptp->pps_source)
		pps_unregister_source(ptp->pps_source);
	ptp_cleanup_sysfs(ptp);
	device_destroy(ptp_class, ptp->devid);

	posix_clock_unregister(&ptp->clock);
=======
static int unregister_vclock(struct device *dev, void *data)
{
	struct ptp_clock *ptp = dev_get_drvdata(dev);

	ptp_vclock_unregister(info_to_vclock(ptp->info));
	return 0;
}

int ptp_clock_unregister(struct ptp_clock *ptp)
{
	if (ptp_vclock_in_use(ptp)) {
		device_for_each_child(&ptp->dev, NULL, unregister_vclock);
	}

	ptp->defunct = 1;
	wake_up_interruptible(&ptp->tsev_wq);

	if (ptp->kworker) {
		kthread_cancel_delayed_work_sync(&ptp->aux_work);
		kthread_destroy_worker(ptp->kworker);
	}

	/* Release the clock's resources. */
	if (ptp->pps_source)
		pps_unregister_source(ptp->pps_source);

	posix_clock_unregister(&ptp->clock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL(ptp_clock_unregister);

void ptp_clock_event(struct ptp_clock *ptp, struct ptp_clock_event *event)
{
<<<<<<< HEAD
	struct pps_event_time evt;
=======
	struct timestamp_event_queue *tsevq;
	struct pps_event_time evt;
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (event->type) {

	case PTP_CLOCK_ALARM:
		break;

	case PTP_CLOCK_EXTTS:
<<<<<<< HEAD
		enqueue_external_timestamp(&ptp->tsevq, event);
=======
	case PTP_CLOCK_EXTOFF:
		/* Enqueue timestamp on selected queues */
		spin_lock_irqsave(&ptp->tsevqs_lock, flags);
		list_for_each_entry(tsevq, &ptp->tsevqs, qlist) {
			if (test_bit((unsigned int)event->index, tsevq->mask))
				enqueue_external_timestamp(tsevq, event);
		}
		spin_unlock_irqrestore(&ptp->tsevqs_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wake_up_interruptible(&ptp->tsev_wq);
		break;

	case PTP_CLOCK_PPS:
		pps_get_ts(&evt);
		pps_event(ptp->pps_source, &evt, PTP_PPS_EVENT, NULL);
		break;
<<<<<<< HEAD
=======

	case PTP_CLOCK_PPSUSR:
		pps_event(ptp->pps_source, &event->pps_times,
			  PTP_PPS_EVENT, NULL);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
EXPORT_SYMBOL(ptp_clock_event);

<<<<<<< HEAD
=======
int ptp_clock_index(struct ptp_clock *ptp)
{
	return ptp->index;
}
EXPORT_SYMBOL(ptp_clock_index);

int ptp_find_pin(struct ptp_clock *ptp,
		 enum ptp_pin_function func, unsigned int chan)
{
	struct ptp_pin_desc *pin = NULL;
	int i;

	for (i = 0; i < ptp->info->n_pins; i++) {
		if (ptp->info->pin_config[i].func == func &&
		    ptp->info->pin_config[i].chan == chan) {
			pin = &ptp->info->pin_config[i];
			break;
		}
	}

	return pin ? i : -1;
}
EXPORT_SYMBOL(ptp_find_pin);

int ptp_find_pin_unlocked(struct ptp_clock *ptp,
			  enum ptp_pin_function func, unsigned int chan)
{
	int result;

	mutex_lock(&ptp->pincfg_mux);

	result = ptp_find_pin(ptp, func, chan);

	mutex_unlock(&ptp->pincfg_mux);

	return result;
}
EXPORT_SYMBOL(ptp_find_pin_unlocked);

int ptp_schedule_worker(struct ptp_clock *ptp, unsigned long delay)
{
	return kthread_mod_delayed_work(ptp->kworker, &ptp->aux_work, delay);
}
EXPORT_SYMBOL(ptp_schedule_worker);

void ptp_cancel_worker_sync(struct ptp_clock *ptp)
{
	kthread_cancel_delayed_work_sync(&ptp->aux_work);
}
EXPORT_SYMBOL(ptp_cancel_worker_sync);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* module operations */

static void __exit ptp_exit(void)
{
<<<<<<< HEAD
	class_destroy(ptp_class);
	unregister_chrdev_region(ptp_devt, PTP_MAX_CLOCKS);
=======
	class_unregister(&ptp_class);
	unregister_chrdev_region(ptp_devt, MINORMASK + 1);
	xa_destroy(&ptp_clocks_map);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init ptp_init(void)
{
	int err;

<<<<<<< HEAD
	ptp_class = class_create(THIS_MODULE, "ptp");
	if (IS_ERR(ptp_class)) {
		pr_err("ptp: failed to allocate class\n");
		return PTR_ERR(ptp_class);
	}

	err = alloc_chrdev_region(&ptp_devt, 0, PTP_MAX_CLOCKS, "ptp");
=======
	err = class_register(&ptp_class);
	if (err) {
		pr_err("ptp: failed to allocate class\n");
		return err;
	}

	err = alloc_chrdev_region(&ptp_devt, 0, MINORMASK + 1, "ptp");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0) {
		pr_err("ptp: failed to allocate device region\n");
		goto no_region;
	}

<<<<<<< HEAD
	ptp_class->dev_attrs = ptp_dev_attrs;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pr_info("PTP clock support registered\n");
	return 0;

no_region:
<<<<<<< HEAD
	class_destroy(ptp_class);
=======
	class_unregister(&ptp_class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

subsys_initcall(ptp_init);
module_exit(ptp_exit);

MODULE_AUTHOR("Richard Cochran <richardcochran@gmail.com>");
MODULE_DESCRIPTION("PTP clocks support");
MODULE_LICENSE("GPL");
