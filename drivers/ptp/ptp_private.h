<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * PTP 1588 clock support - private declarations for the core module.
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
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _PTP_PRIVATE_H_
#define _PTP_PRIVATE_H_

#include <linux/cdev.h>
#include <linux/device.h>
<<<<<<< HEAD
=======
#include <linux/kthread.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mutex.h>
#include <linux/posix-clock.h>
#include <linux/ptp_clock.h>
#include <linux/ptp_clock_kernel.h>
#include <linux/time.h>
<<<<<<< HEAD

#define PTP_MAX_TIMESTAMPS 128
#define PTP_BUF_TIMESTAMPS 30
=======
#include <linux/list.h>
#include <linux/bitmap.h>
#include <linux/debugfs.h>

#define PTP_MAX_TIMESTAMPS 128
#define PTP_BUF_TIMESTAMPS 30
#define PTP_DEFAULT_MAX_VCLOCKS 20
#define PTP_MAX_CHANNELS 2048
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct timestamp_event_queue {
	struct ptp_extts_event buf[PTP_MAX_TIMESTAMPS];
	int head;
	int tail;
	spinlock_t lock;
<<<<<<< HEAD
=======
	struct list_head qlist;
	unsigned long *mask;
	struct dentry *debugfs_instance;
	struct debugfs_u32_array dfs_bitmap;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct ptp_clock {
	struct posix_clock clock;
<<<<<<< HEAD
	struct device *dev;
=======
	struct device dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ptp_clock_info *info;
	dev_t devid;
	int index; /* index into clocks.map */
	struct pps_device *pps_source;
<<<<<<< HEAD
	struct timestamp_event_queue tsevq; /* simple fifo for time stamps */
	struct mutex tsevq_mux; /* one process at a time reading the fifo */
	wait_queue_head_t tsev_wq;
	int defunct; /* tells readers to go away when clock is being removed */
=======
	long dialed_frequency; /* remembers the frequency adjustment */
	struct list_head tsevqs; /* timestamp fifo list */
	spinlock_t tsevqs_lock; /* protects tsevqs from concurrent access */
	struct mutex pincfg_mux; /* protect concurrent info->pin_config access */
	wait_queue_head_t tsev_wq;
	int defunct; /* tells readers to go away when clock is being removed */
	struct device_attribute *pin_dev_attr;
	struct attribute **pin_attr;
	struct attribute_group pin_attr_group;
	/* 1st entry is a pointer to the real group, 2nd is NULL terminator */
	const struct attribute_group *pin_attr_groups[2];
	struct kthread_worker *kworker;
	struct kthread_delayed_work aux_work;
	unsigned int max_vclocks;
	unsigned int n_vclocks;
	int *vclock_index;
	struct mutex n_vclocks_mux; /* protect concurrent n_vclocks access */
	bool is_virtual_clock;
	bool has_cycles;
	struct dentry *debugfs_root;
};

#define info_to_vclock(d) container_of((d), struct ptp_vclock, info)
#define cc_to_vclock(d) container_of((d), struct ptp_vclock, cc)
#define dw_to_vclock(d) container_of((d), struct ptp_vclock, refresh_work)

struct ptp_vclock {
	struct ptp_clock *pclock;
	struct ptp_clock_info info;
	struct ptp_clock *clock;
	struct hlist_node vclock_hash_node;
	struct cyclecounter cc;
	struct timecounter tc;
	struct mutex lock;	/* protects tc/cc */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * The function queue_cnt() is safe for readers to call without
 * holding q->lock. Readers use this function to verify that the queue
 * is nonempty before proceeding with a dequeue operation. The fact
 * that a writer might concurrently increment the tail does not
 * matter, since the queue remains nonempty nonetheless.
 */
<<<<<<< HEAD
static inline int queue_cnt(struct timestamp_event_queue *q)
{
	int cnt = q->tail - q->head;
	return cnt < 0 ? PTP_MAX_TIMESTAMPS + cnt : cnt;
}

=======
static inline int queue_cnt(const struct timestamp_event_queue *q)
{
	/*
	 * Paired with WRITE_ONCE() in enqueue_external_timestamp(),
	 * ptp_read(), extts_fifo_show().
	 */
	int cnt = READ_ONCE(q->tail) - READ_ONCE(q->head);
	return cnt < 0 ? PTP_MAX_TIMESTAMPS + cnt : cnt;
}

/* Check if ptp virtual clock is in use */
static inline bool ptp_vclock_in_use(struct ptp_clock *ptp)
{
	bool in_use = false;

	if (mutex_lock_interruptible(&ptp->n_vclocks_mux))
		return true;

	if (!ptp->is_virtual_clock && ptp->n_vclocks)
		in_use = true;

	mutex_unlock(&ptp->n_vclocks_mux);

	return in_use;
}

/* Check if ptp clock shall be free running */
static inline bool ptp_clock_freerun(struct ptp_clock *ptp)
{
	if (ptp->has_cycles)
		return false;

	return ptp_vclock_in_use(ptp);
}

extern const struct class ptp_class;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * see ptp_chardev.c
 */

<<<<<<< HEAD
long ptp_ioctl(struct posix_clock *pc,
	       unsigned int cmd, unsigned long arg);

int ptp_open(struct posix_clock *pc, fmode_t fmode);

ssize_t ptp_read(struct posix_clock *pc,
		 uint flags, char __user *buf, size_t cnt);

uint ptp_poll(struct posix_clock *pc,
	      struct file *fp, poll_table *wait);
=======
/* caller must hold pincfg_mux */
int ptp_set_pinfunc(struct ptp_clock *ptp, unsigned int pin,
		    enum ptp_pin_function func, unsigned int chan);

long ptp_ioctl(struct posix_clock_context *pccontext, unsigned int cmd,
	       unsigned long arg);

int ptp_open(struct posix_clock_context *pccontext, fmode_t fmode);

int ptp_release(struct posix_clock_context *pccontext);

ssize_t ptp_read(struct posix_clock_context *pccontext, uint flags, char __user *buf,
		 size_t cnt);

__poll_t ptp_poll(struct posix_clock_context *pccontext, struct file *fp,
		  poll_table *wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * see ptp_sysfs.c
 */

<<<<<<< HEAD
extern struct device_attribute ptp_dev_attrs[];

int ptp_cleanup_sysfs(struct ptp_clock *ptp);

int ptp_populate_sysfs(struct ptp_clock *ptp);

=======
extern const struct attribute_group *ptp_groups[];

int ptp_populate_pin_groups(struct ptp_clock *ptp);
void ptp_cleanup_pin_groups(struct ptp_clock *ptp);

struct ptp_vclock *ptp_vclock_register(struct ptp_clock *pclock);
void ptp_vclock_unregister(struct ptp_vclock *vclock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
