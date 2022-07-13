<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
   drbd.c

   This file is part of DRBD by Philipp Reisner and Lars Ellenberg.

   Copyright (C) 2001-2008, LINBIT Information Technologies GmbH.
   Copyright (C) 1999-2008, Philipp Reisner <philipp.reisner@linbit.com>.
   Copyright (C) 2002-2008, Lars Ellenberg <lars.ellenberg@linbit.com>.

   Thanks to Carter Burden, Bart Grantham and Gennadiy Nerubayev
   from Logicworks, Inc. for making SDP replication support possible.

<<<<<<< HEAD
   drbd is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   drbd is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with drbd; see the file COPYING.  If not, write to
   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 */

#include <linux/module.h>
#include <linux/drbd.h>
#include <asm/uaccess.h>
=======

 */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/drbd.h>
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/types.h>
#include <net/sock.h>
#include <linux/ctype.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/memcontrol.h>
#include <linux/mm_inline.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/reboot.h>
#include <linux/notifier.h>
#include <linux/kthread.h>
<<<<<<< HEAD

#define __KERNEL_SYSCALLS__
#include <linux/unistd.h>
#include <linux/vmalloc.h>

#include <linux/drbd_limits.h>
#include "drbd_int.h"
#include "drbd_req.h" /* only for _req_mod in tl_release and tl_clear */

#include "drbd_vli.h"

struct after_state_chg_work {
	struct drbd_work w;
	union drbd_state os;
	union drbd_state ns;
	enum chg_state_flags flags;
	struct completion *done;
};

static DEFINE_MUTEX(drbd_main_mutex);
int drbdd_init(struct drbd_thread *);
int drbd_worker(struct drbd_thread *);
int drbd_asender(struct drbd_thread *);

int drbd_init(void);
static int drbd_open(struct block_device *bdev, fmode_t mode);
static int drbd_release(struct gendisk *gd, fmode_t mode);
static int w_after_state_ch(struct drbd_conf *mdev, struct drbd_work *w, int unused);
static void after_state_ch(struct drbd_conf *mdev, union drbd_state os,
			   union drbd_state ns, enum chg_state_flags flags);
static int w_md_sync(struct drbd_conf *mdev, struct drbd_work *w, int unused);
static void md_sync_timer_fn(unsigned long data);
static int w_bitmap_io(struct drbd_conf *mdev, struct drbd_work *w, int unused);
static int w_go_diskless(struct drbd_conf *mdev, struct drbd_work *w, int unused);
=======
#include <linux/workqueue.h>
#include <linux/unistd.h>
#include <linux/vmalloc.h>
#include <linux/sched/signal.h>

#include <linux/drbd_limits.h>
#include "drbd_int.h"
#include "drbd_protocol.h"
#include "drbd_req.h" /* only for _req_mod in tl_release and tl_clear */
#include "drbd_vli.h"
#include "drbd_debugfs.h"

static DEFINE_MUTEX(drbd_main_mutex);
static int drbd_open(struct gendisk *disk, blk_mode_t mode);
static void drbd_release(struct gendisk *gd);
static void md_sync_timer_fn(struct timer_list *t);
static int w_bitmap_io(struct drbd_work *w, int unused);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Philipp Reisner <phil@linbit.com>, "
	      "Lars Ellenberg <lars@linbit.com>");
MODULE_DESCRIPTION("drbd - Distributed Replicated Block Device v" REL_VERSION);
MODULE_VERSION(REL_VERSION);
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_PARM_DESC(minor_count, "Maximum number of drbd devices ("
=======
MODULE_PARM_DESC(minor_count, "Approximate number of drbd devices ("
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 __stringify(DRBD_MINOR_COUNT_MIN) "-" __stringify(DRBD_MINOR_COUNT_MAX) ")");
MODULE_ALIAS_BLOCKDEV_MAJOR(DRBD_MAJOR);

#include <linux/moduleparam.h>
<<<<<<< HEAD
/* allow_open_on_secondary */
MODULE_PARM_DESC(allow_oos, "DONT USE!");
/* thanks to these macros, if compiled into the kernel (not-module),
 * this becomes the boot parameter drbd.minor_count */
module_param(minor_count, uint, 0444);
module_param(disable_sendpage, bool, 0644);
module_param(allow_oos, bool, 0);
module_param(cn_idx, uint, 0444);
module_param(proc_details, int, 0644);

#ifdef CONFIG_DRBD_FAULT_INJECTION
int enable_faults;
int fault_rate;
static int fault_count;
int fault_devs;
/* bitmap of enabled faults */
module_param(enable_faults, int, 0664);
/* fault rate % value - applies to all enabled faults */
module_param(fault_rate, int, 0664);
/* count of faults inserted */
module_param(fault_count, int, 0664);
/* bitmap of devices to insert faults on */
module_param(fault_devs, int, 0644);
#endif

/* module parameter, defined */
unsigned int minor_count = DRBD_MINOR_COUNT_DEF;
bool disable_sendpage;
bool allow_oos;
unsigned int cn_idx = CN_IDX_DRBD;
int proc_details;       /* Detail level in proc drbd*/

/* Module parameter for setting the user mode helper program
 * to run. Default is /sbin/drbdadm */
char usermode_helper[80] = "/sbin/drbdadm";

module_param_string(usermode_helper, usermode_helper, sizeof(usermode_helper), 0644);
=======
/* thanks to these macros, if compiled into the kernel (not-module),
 * these become boot parameters (e.g., drbd.minor_count) */

#ifdef CONFIG_DRBD_FAULT_INJECTION
int drbd_enable_faults;
int drbd_fault_rate;
static int drbd_fault_count;
static int drbd_fault_devs;
/* bitmap of enabled faults */
module_param_named(enable_faults, drbd_enable_faults, int, 0664);
/* fault rate % value - applies to all enabled faults */
module_param_named(fault_rate, drbd_fault_rate, int, 0664);
/* count of faults inserted */
module_param_named(fault_count, drbd_fault_count, int, 0664);
/* bitmap of devices to insert faults on */
module_param_named(fault_devs, drbd_fault_devs, int, 0644);
#endif

/* module parameters we can keep static */
static bool drbd_allow_oos; /* allow_open_on_secondary */
static bool drbd_disable_sendpage;
MODULE_PARM_DESC(allow_oos, "DONT USE!");
module_param_named(allow_oos, drbd_allow_oos, bool, 0);
module_param_named(disable_sendpage, drbd_disable_sendpage, bool, 0644);

/* module parameters we share */
int drbd_proc_details; /* Detail level in proc drbd*/
module_param_named(proc_details, drbd_proc_details, int, 0644);
/* module parameters shared with defaults */
unsigned int drbd_minor_count = DRBD_MINOR_COUNT_DEF;
/* Module parameter for setting the user mode helper program
 * to run. Default is /sbin/drbdadm */
char drbd_usermode_helper[80] = "/sbin/drbdadm";
module_param_named(minor_count, drbd_minor_count, uint, 0444);
module_param_string(usermode_helper, drbd_usermode_helper, sizeof(drbd_usermode_helper), 0644);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* in 2.6.x, our device mapping and config info contains our virtual gendisks
 * as member "struct gendisk *vdisk;"
 */
<<<<<<< HEAD
struct drbd_conf **minor_table;

struct kmem_cache *drbd_request_cache;
struct kmem_cache *drbd_ee_cache;	/* epoch entries */
struct kmem_cache *drbd_bm_ext_cache;	/* bitmap extents */
struct kmem_cache *drbd_al_ext_cache;	/* activity log extents */
mempool_t *drbd_request_mempool;
mempool_t *drbd_ee_mempool;
=======
struct idr drbd_devices;
struct list_head drbd_resources;
struct mutex resources_mutex;

struct kmem_cache *drbd_request_cache;
struct kmem_cache *drbd_ee_cache;	/* peer requests */
struct kmem_cache *drbd_bm_ext_cache;	/* bitmap extents */
struct kmem_cache *drbd_al_ext_cache;	/* activity log extents */
mempool_t drbd_request_mempool;
mempool_t drbd_ee_mempool;
mempool_t drbd_md_io_page_pool;
struct bio_set drbd_md_io_bio_set;
struct bio_set drbd_io_bio_set;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* I do not use a standard mempool, because:
   1) I want to hand out the pre-allocated objects first.
   2) I want to be able to interrupt sleeping allocation with a signal.
   Note: This is a single linked list, the next pointer is the private
	 member of struct page.
 */
struct page *drbd_pp_pool;
<<<<<<< HEAD
spinlock_t   drbd_pp_lock;
=======
DEFINE_SPINLOCK(drbd_pp_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int          drbd_pp_vacant;
wait_queue_head_t drbd_pp_wait;

DEFINE_RATELIMIT_STATE(drbd_ratelimit_state, 5 * HZ, 5);

static const struct block_device_operations drbd_ops = {
<<<<<<< HEAD
	.owner =   THIS_MODULE,
	.open =    drbd_open,
	.release = drbd_release,
};

#define ARRY_SIZE(A) (sizeof(A)/sizeof(A[0]))

=======
	.owner		= THIS_MODULE,
	.submit_bio	= drbd_submit_bio,
	.open		= drbd_open,
	.release	= drbd_release,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef __CHECKER__
/* When checking with sparse, and this is an inline function, sparse will
   give tons of false positives. When this is a real functions sparse works.
 */
<<<<<<< HEAD
int _get_ldev_if_state(struct drbd_conf *mdev, enum drbd_disk_state mins)
{
	int io_allowed;

	atomic_inc(&mdev->local_cnt);
	io_allowed = (mdev->state.disk >= mins);
	if (!io_allowed) {
		if (atomic_dec_and_test(&mdev->local_cnt))
			wake_up(&mdev->misc_wait);
=======
int _get_ldev_if_state(struct drbd_device *device, enum drbd_disk_state mins)
{
	int io_allowed;

	atomic_inc(&device->local_cnt);
	io_allowed = (device->state.disk >= mins);
	if (!io_allowed) {
		if (atomic_dec_and_test(&device->local_cnt))
			wake_up(&device->misc_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return io_allowed;
}

#endif

/**
<<<<<<< HEAD
 * DOC: The transfer log
 *
 * The transfer log is a single linked list of &struct drbd_tl_epoch objects.
 * mdev->newest_tle points to the head, mdev->oldest_tle points to the tail
 * of the list. There is always at least one &struct drbd_tl_epoch object.
 *
 * Each &struct drbd_tl_epoch has a circular double linked list of requests
 * attached.
 */
static int tl_init(struct drbd_conf *mdev)
{
	struct drbd_tl_epoch *b;

	/* during device minor initialization, we may well use GFP_KERNEL */
	b = kmalloc(sizeof(struct drbd_tl_epoch), GFP_KERNEL);
	if (!b)
		return 0;
	INIT_LIST_HEAD(&b->requests);
	INIT_LIST_HEAD(&b->w.list);
	b->next = NULL;
	b->br_number = 4711;
	b->n_writes = 0;
	b->w.cb = NULL; /* if this is != NULL, we need to dec_ap_pending in tl_clear */

	mdev->oldest_tle = b;
	mdev->newest_tle = b;
	INIT_LIST_HEAD(&mdev->out_of_sequence_requests);

	mdev->tl_hash = NULL;
	mdev->tl_hash_s = 0;

	return 1;
}

static void tl_cleanup(struct drbd_conf *mdev)
{
	D_ASSERT(mdev->oldest_tle == mdev->newest_tle);
	D_ASSERT(list_empty(&mdev->out_of_sequence_requests));
	kfree(mdev->oldest_tle);
	mdev->oldest_tle = NULL;
	kfree(mdev->unused_spare_tle);
	mdev->unused_spare_tle = NULL;
	kfree(mdev->tl_hash);
	mdev->tl_hash = NULL;
	mdev->tl_hash_s = 0;
}

/**
 * _tl_add_barrier() - Adds a barrier to the transfer log
 * @mdev:	DRBD device.
 * @new:	Barrier to be added before the current head of the TL.
 *
 * The caller must hold the req_lock.
 */
void _tl_add_barrier(struct drbd_conf *mdev, struct drbd_tl_epoch *new)
{
	struct drbd_tl_epoch *newest_before;

	INIT_LIST_HEAD(&new->requests);
	INIT_LIST_HEAD(&new->w.list);
	new->w.cb = NULL; /* if this is != NULL, we need to dec_ap_pending in tl_clear */
	new->next = NULL;
	new->n_writes = 0;

	newest_before = mdev->newest_tle;
	/* never send a barrier number == 0, because that is special-cased
	 * when using TCQ for our write ordering code */
	new->br_number = (newest_before->br_number+1) ?: 1;
	if (mdev->newest_tle != new) {
		mdev->newest_tle->next = new;
		mdev->newest_tle = new;
	}
}

/**
 * tl_release() - Free or recycle the oldest &struct drbd_tl_epoch object of the TL
 * @mdev:	DRBD device.
=======
 * tl_release() - mark as BARRIER_ACKED all requests in the corresponding transfer log epoch
 * @connection:	DRBD connection.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @barrier_nr:	Expected identifier of the DRBD write barrier packet.
 * @set_size:	Expected number of requests before that barrier.
 *
 * In case the passed barrier_nr or set_size does not match the oldest
<<<<<<< HEAD
 * &struct drbd_tl_epoch objects this function will cause a termination
 * of the connection.
 */
void tl_release(struct drbd_conf *mdev, unsigned int barrier_nr,
		       unsigned int set_size)
{
	struct drbd_tl_epoch *b, *nob; /* next old barrier */
	struct list_head *le, *tle;
	struct drbd_request *r;

	spin_lock_irq(&mdev->req_lock);

	b = mdev->oldest_tle;

	/* first some paranoia code */
	if (b == NULL) {
		dev_err(DEV, "BAD! BarrierAck #%u received, but no epoch in tl!?\n",
			barrier_nr);
		goto bail;
	}
	if (b->br_number != barrier_nr) {
		dev_err(DEV, "BAD! BarrierAck #%u received, expected #%u!\n",
			barrier_nr, b->br_number);
		goto bail;
	}
	if (b->n_writes != set_size) {
		dev_err(DEV, "BAD! BarrierAck #%u received with n_writes=%u, expected n_writes=%u!\n",
			barrier_nr, set_size, b->n_writes);
		goto bail;
	}

	/* Clean up list of requests processed during current epoch */
	list_for_each_safe(le, tle, &b->requests) {
		r = list_entry(le, struct drbd_request, tl_requests);
		_req_mod(r, barrier_acked);
	}
	/* There could be requests on the list waiting for completion
	   of the write to the local disk. To avoid corruptions of
	   slab's data structures we have to remove the lists head.

	   Also there could have been a barrier ack out of sequence, overtaking
	   the write acks - which would be a bug and violating write ordering.
	   To not deadlock in case we lose connection while such requests are
	   still pending, we need some way to find them for the
	   _req_mode(connection_lost_while_pending).

	   These have been list_move'd to the out_of_sequence_requests list in
	   _req_mod(, barrier_acked) above.
	   */
	list_del_init(&b->requests);

	nob = b->next;
	if (test_and_clear_bit(CREATE_BARRIER, &mdev->flags)) {
		_tl_add_barrier(mdev, b);
		if (nob)
			mdev->oldest_tle = nob;
		/* if nob == NULL b was the only barrier, and becomes the new
		   barrier. Therefore mdev->oldest_tle points already to b */
	} else {
		D_ASSERT(nob != NULL);
		mdev->oldest_tle = nob;
		kfree(b);
	}

	spin_unlock_irq(&mdev->req_lock);
	dec_ap_pending(mdev);
=======
 * epoch of not yet barrier-acked requests, this function will cause a
 * termination of the connection.
 */
void tl_release(struct drbd_connection *connection, unsigned int barrier_nr,
		unsigned int set_size)
{
	struct drbd_request *r;
	struct drbd_request *req = NULL, *tmp = NULL;
	int expect_epoch = 0;
	int expect_size = 0;

	spin_lock_irq(&connection->resource->req_lock);

	/* find oldest not yet barrier-acked write request,
	 * count writes in its epoch. */
	list_for_each_entry(r, &connection->transfer_log, tl_requests) {
		const unsigned s = r->rq_state;
		if (!req) {
			if (!(s & RQ_WRITE))
				continue;
			if (!(s & RQ_NET_MASK))
				continue;
			if (s & RQ_NET_DONE)
				continue;
			req = r;
			expect_epoch = req->epoch;
			expect_size ++;
		} else {
			if (r->epoch != expect_epoch)
				break;
			if (!(s & RQ_WRITE))
				continue;
			/* if (s & RQ_DONE): not expected */
			/* if (!(s & RQ_NET_MASK)): not expected */
			expect_size++;
		}
	}

	/* first some paranoia code */
	if (req == NULL) {
		drbd_err(connection, "BAD! BarrierAck #%u received, but no epoch in tl!?\n",
			 barrier_nr);
		goto bail;
	}
	if (expect_epoch != barrier_nr) {
		drbd_err(connection, "BAD! BarrierAck #%u received, expected #%u!\n",
			 barrier_nr, expect_epoch);
		goto bail;
	}

	if (expect_size != set_size) {
		drbd_err(connection, "BAD! BarrierAck #%u received with n_writes=%u, expected n_writes=%u!\n",
			 barrier_nr, set_size, expect_size);
		goto bail;
	}

	/* Clean up list of requests processed during current epoch. */
	/* this extra list walk restart is paranoia,
	 * to catch requests being barrier-acked "unexpectedly".
	 * It usually should find the same req again, or some READ preceding it. */
	list_for_each_entry(req, &connection->transfer_log, tl_requests)
		if (req->epoch == expect_epoch) {
			tmp = req;
			break;
		}
	req = list_prepare_entry(tmp, &connection->transfer_log, tl_requests);
	list_for_each_entry_safe_from(req, r, &connection->transfer_log, tl_requests) {
		struct drbd_peer_device *peer_device;
		if (req->epoch != expect_epoch)
			break;
		peer_device = conn_peer_device(connection, req->device->vnr);
		_req_mod(req, BARRIER_ACKED, peer_device);
	}
	spin_unlock_irq(&connection->resource->req_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return;

bail:
<<<<<<< HEAD
	spin_unlock_irq(&mdev->req_lock);
	drbd_force_state(mdev, NS(conn, C_PROTOCOL_ERROR));
=======
	spin_unlock_irq(&connection->resource->req_lock);
	conn_request_state(connection, NS(conn, C_PROTOCOL_ERROR), CS_HARD);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/**
 * _tl_restart() - Walks the transfer log, and applies an action to all requests
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @what:       The action/event to perform with all request objects
 *
 * @what might be one of connection_lost_while_pending, resend, fail_frozen_disk_io,
 * restart_frozen_disk_io.
 */
static void _tl_restart(struct drbd_conf *mdev, enum drbd_req_event what)
{
	struct drbd_tl_epoch *b, *tmp, **pn;
	struct list_head *le, *tle, carry_reads;
	struct drbd_request *req;
	int rv, n_writes, n_reads;

	b = mdev->oldest_tle;
	pn = &mdev->oldest_tle;
	while (b) {
		n_writes = 0;
		n_reads = 0;
		INIT_LIST_HEAD(&carry_reads);
		list_for_each_safe(le, tle, &b->requests) {
			req = list_entry(le, struct drbd_request, tl_requests);
			rv = _req_mod(req, what);

			n_writes += (rv & MR_WRITE) >> MR_WRITE_SHIFT;
			n_reads  += (rv & MR_READ) >> MR_READ_SHIFT;
		}
		tmp = b->next;

		if (n_writes) {
			if (what == resend) {
				b->n_writes = n_writes;
				if (b->w.cb == NULL) {
					b->w.cb = w_send_barrier;
					inc_ap_pending(mdev);
					set_bit(CREATE_BARRIER, &mdev->flags);
				}

				drbd_queue_work(&mdev->data.work, &b->w);
			}
			pn = &b->next;
		} else {
			if (n_reads)
				list_add(&carry_reads, &b->requests);
			/* there could still be requests on that ring list,
			 * in case local io is still pending */
			list_del(&b->requests);

			/* dec_ap_pending corresponding to queue_barrier.
			 * the newest barrier may not have been queued yet,
			 * in which case w.cb is still NULL. */
			if (b->w.cb != NULL)
				dec_ap_pending(mdev);

			if (b == mdev->newest_tle) {
				/* recycle, but reinit! */
				D_ASSERT(tmp == NULL);
				INIT_LIST_HEAD(&b->requests);
				list_splice(&carry_reads, &b->requests);
				INIT_LIST_HEAD(&b->w.list);
				b->w.cb = NULL;
				b->br_number = net_random();
				b->n_writes = 0;

				*pn = b;
				break;
			}
			*pn = tmp;
			kfree(b);
		}
		b = tmp;
		list_splice(&carry_reads, &b->requests);
	}
}


/**
 * tl_clear() - Clears all requests and &struct drbd_tl_epoch objects out of the TL
 * @mdev:	DRBD device.
=======
 * @connection:	DRBD connection to operate on.
 * @what:       The action/event to perform with all request objects
 *
 * @what might be one of CONNECTION_LOST_WHILE_PENDING, RESEND, FAIL_FROZEN_DISK_IO,
 * RESTART_FROZEN_DISK_IO.
 */
/* must hold resource->req_lock */
void _tl_restart(struct drbd_connection *connection, enum drbd_req_event what)
{
	struct drbd_peer_device *peer_device;
	struct drbd_request *req, *r;

	list_for_each_entry_safe(req, r, &connection->transfer_log, tl_requests) {
		peer_device = conn_peer_device(connection, req->device->vnr);
		_req_mod(req, what, peer_device);
	}
}

void tl_restart(struct drbd_connection *connection, enum drbd_req_event what)
{
	spin_lock_irq(&connection->resource->req_lock);
	_tl_restart(connection, what);
	spin_unlock_irq(&connection->resource->req_lock);
}

/**
 * tl_clear() - Clears all requests and &struct drbd_tl_epoch objects out of the TL
 * @connection:	DRBD connection.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This is called after the connection to the peer was lost. The storage covered
 * by the requests on the transfer gets marked as our of sync. Called from the
 * receiver thread and the worker thread.
 */
<<<<<<< HEAD
void tl_clear(struct drbd_conf *mdev)
{
	struct list_head *le, *tle;
	struct drbd_request *r;

	spin_lock_irq(&mdev->req_lock);

	_tl_restart(mdev, connection_lost_while_pending);

	/* we expect this list to be empty. */
	D_ASSERT(list_empty(&mdev->out_of_sequence_requests));

	/* but just in case, clean it up anyways! */
	list_for_each_safe(le, tle, &mdev->out_of_sequence_requests) {
		r = list_entry(le, struct drbd_request, tl_requests);
		/* It would be nice to complete outside of spinlock.
		 * But this is easier for now. */
		_req_mod(r, connection_lost_while_pending);
	}

	/* ensure bit indicating barrier is required is clear */
	clear_bit(CREATE_BARRIER, &mdev->flags);

	memset(mdev->app_reads_hash, 0, APP_R_HSIZE*sizeof(void *));

	spin_unlock_irq(&mdev->req_lock);
}

void tl_restart(struct drbd_conf *mdev, enum drbd_req_event what)
{
	spin_lock_irq(&mdev->req_lock);
	_tl_restart(mdev, what);
	spin_unlock_irq(&mdev->req_lock);
}

/**
 * cl_wide_st_chg() - true if the state change is a cluster wide one
 * @mdev:	DRBD device.
 * @os:		old (current) state.
 * @ns:		new (wanted) state.
 */
static int cl_wide_st_chg(struct drbd_conf *mdev,
			  union drbd_state os, union drbd_state ns)
{
	return (os.conn >= C_CONNECTED && ns.conn >= C_CONNECTED &&
		 ((os.role != R_PRIMARY && ns.role == R_PRIMARY) ||
		  (os.conn != C_STARTING_SYNC_T && ns.conn == C_STARTING_SYNC_T) ||
		  (os.conn != C_STARTING_SYNC_S && ns.conn == C_STARTING_SYNC_S) ||
		  (os.disk != D_DISKLESS && ns.disk == D_DISKLESS))) ||
		(os.conn >= C_CONNECTED && ns.conn == C_DISCONNECTING) ||
		(os.conn == C_CONNECTED && ns.conn == C_VERIFY_S);
}

enum drbd_state_rv
drbd_change_state(struct drbd_conf *mdev, enum chg_state_flags f,
		  union drbd_state mask, union drbd_state val)
{
	unsigned long flags;
	union drbd_state os, ns;
	enum drbd_state_rv rv;

	spin_lock_irqsave(&mdev->req_lock, flags);
	os = mdev->state;
	ns.i = (os.i & ~mask.i) | val.i;
	rv = _drbd_set_state(mdev, ns, f, NULL);
	ns = mdev->state;
	spin_unlock_irqrestore(&mdev->req_lock, flags);

	return rv;
}

/**
 * drbd_force_state() - Impose a change which happens outside our control on our state
 * @mdev:	DRBD device.
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 */
void drbd_force_state(struct drbd_conf *mdev,
	union drbd_state mask, union drbd_state val)
{
	drbd_change_state(mdev, CS_HARD, mask, val);
}

static enum drbd_state_rv is_valid_state(struct drbd_conf *, union drbd_state);
static enum drbd_state_rv is_valid_state_transition(struct drbd_conf *,
						    union drbd_state,
						    union drbd_state);
static union drbd_state sanitize_state(struct drbd_conf *mdev, union drbd_state os,
				       union drbd_state ns, const char **warn_sync_abort);
int drbd_send_state_req(struct drbd_conf *,
			union drbd_state, union drbd_state);

static enum drbd_state_rv
_req_st_cond(struct drbd_conf *mdev, union drbd_state mask,
	     union drbd_state val)
{
	union drbd_state os, ns;
	unsigned long flags;
	enum drbd_state_rv rv;

	if (test_and_clear_bit(CL_ST_CHG_SUCCESS, &mdev->flags))
		return SS_CW_SUCCESS;

	if (test_and_clear_bit(CL_ST_CHG_FAIL, &mdev->flags))
		return SS_CW_FAILED_BY_PEER;

	rv = 0;
	spin_lock_irqsave(&mdev->req_lock, flags);
	os = mdev->state;
	ns.i = (os.i & ~mask.i) | val.i;
	ns = sanitize_state(mdev, os, ns, NULL);

	if (!cl_wide_st_chg(mdev, os, ns))
		rv = SS_CW_NO_NEED;
	if (!rv) {
		rv = is_valid_state(mdev, ns);
		if (rv == SS_SUCCESS) {
			rv = is_valid_state_transition(mdev, ns, os);
			if (rv == SS_SUCCESS)
				rv = SS_UNKNOWN_ERROR; /* cont waiting, otherwise fail. */
		}
	}
	spin_unlock_irqrestore(&mdev->req_lock, flags);

	return rv;
}

/**
 * drbd_req_state() - Perform an eventually cluster wide state change
 * @mdev:	DRBD device.
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 * @f:		flags
 *
 * Should not be called directly, use drbd_request_state() or
 * _drbd_request_state().
 */
static enum drbd_state_rv
drbd_req_state(struct drbd_conf *mdev, union drbd_state mask,
	       union drbd_state val, enum chg_state_flags f)
{
	struct completion done;
	unsigned long flags;
	union drbd_state os, ns;
	enum drbd_state_rv rv;

	init_completion(&done);

	if (f & CS_SERIALIZE)
		mutex_lock(&mdev->state_mutex);

	spin_lock_irqsave(&mdev->req_lock, flags);
	os = mdev->state;
	ns.i = (os.i & ~mask.i) | val.i;
	ns = sanitize_state(mdev, os, ns, NULL);

	if (cl_wide_st_chg(mdev, os, ns)) {
		rv = is_valid_state(mdev, ns);
		if (rv == SS_SUCCESS)
			rv = is_valid_state_transition(mdev, ns, os);
		spin_unlock_irqrestore(&mdev->req_lock, flags);

		if (rv < SS_SUCCESS) {
			if (f & CS_VERBOSE)
				print_st_err(mdev, os, ns, rv);
			goto abort;
		}

		drbd_state_lock(mdev);
		if (!drbd_send_state_req(mdev, mask, val)) {
			drbd_state_unlock(mdev);
			rv = SS_CW_FAILED_BY_PEER;
			if (f & CS_VERBOSE)
				print_st_err(mdev, os, ns, rv);
			goto abort;
		}

		wait_event(mdev->state_wait,
			(rv = _req_st_cond(mdev, mask, val)));

		if (rv < SS_SUCCESS) {
			drbd_state_unlock(mdev);
			if (f & CS_VERBOSE)
				print_st_err(mdev, os, ns, rv);
			goto abort;
		}
		spin_lock_irqsave(&mdev->req_lock, flags);
		os = mdev->state;
		ns.i = (os.i & ~mask.i) | val.i;
		rv = _drbd_set_state(mdev, ns, f, &done);
		drbd_state_unlock(mdev);
	} else {
		rv = _drbd_set_state(mdev, ns, f, &done);
	}

	spin_unlock_irqrestore(&mdev->req_lock, flags);

	if (f & CS_WAIT_COMPLETE && rv == SS_SUCCESS) {
		D_ASSERT(current != mdev->worker.task);
		wait_for_completion(&done);
	}

abort:
	if (f & CS_SERIALIZE)
		mutex_unlock(&mdev->state_mutex);

	return rv;
}

/**
 * _drbd_request_state() - Request a state change (with flags)
 * @mdev:	DRBD device.
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 * @f:		flags
 *
 * Cousin of drbd_request_state(), useful with the CS_WAIT_COMPLETE
 * flag, or when logging of failed state change requests is not desired.
 */
enum drbd_state_rv
_drbd_request_state(struct drbd_conf *mdev, union drbd_state mask,
		    union drbd_state val, enum chg_state_flags f)
{
	enum drbd_state_rv rv;

	wait_event(mdev->state_wait,
		   (rv = drbd_req_state(mdev, mask, val, f)) != SS_IN_TRANSIENT_STATE);

	return rv;
}

static void print_st(struct drbd_conf *mdev, char *name, union drbd_state ns)
{
	dev_err(DEV, " %s = { cs:%s ro:%s/%s ds:%s/%s %c%c%c%c }\n",
	    name,
	    drbd_conn_str(ns.conn),
	    drbd_role_str(ns.role),
	    drbd_role_str(ns.peer),
	    drbd_disk_str(ns.disk),
	    drbd_disk_str(ns.pdsk),
	    is_susp(ns) ? 's' : 'r',
	    ns.aftr_isp ? 'a' : '-',
	    ns.peer_isp ? 'p' : '-',
	    ns.user_isp ? 'u' : '-'
	    );
}

void print_st_err(struct drbd_conf *mdev, union drbd_state os,
	          union drbd_state ns, enum drbd_state_rv err)
{
	if (err == SS_IN_TRANSIENT_STATE)
		return;
	dev_err(DEV, "State change failed: %s\n", drbd_set_st_err_str(err));
	print_st(mdev, " state", os);
	print_st(mdev, "wanted", ns);
}


/**
 * is_valid_state() - Returns an SS_ error code if ns is not valid
 * @mdev:	DRBD device.
 * @ns:		State to consider.
 */
static enum drbd_state_rv
is_valid_state(struct drbd_conf *mdev, union drbd_state ns)
{
	/* See drbd_state_sw_errors in drbd_strings.c */

	enum drbd_fencing_p fp;
	enum drbd_state_rv rv = SS_SUCCESS;

	fp = FP_DONT_CARE;
	if (get_ldev(mdev)) {
		fp = mdev->ldev->dc.fencing;
		put_ldev(mdev);
	}

	if (get_net_conf(mdev)) {
		if (!mdev->net_conf->two_primaries &&
		    ns.role == R_PRIMARY && ns.peer == R_PRIMARY)
			rv = SS_TWO_PRIMARIES;
		put_net_conf(mdev);
	}

	if (rv <= 0)
		/* already found a reason to abort */;
	else if (ns.role == R_SECONDARY && mdev->open_cnt)
		rv = SS_DEVICE_IN_USE;

	else if (ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.disk < D_UP_TO_DATE)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if (fp >= FP_RESOURCE &&
		 ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.pdsk >= D_UNKNOWN)
		rv = SS_PRIMARY_NOP;

	else if (ns.role == R_PRIMARY && ns.disk <= D_INCONSISTENT && ns.pdsk <= D_INCONSISTENT)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if (ns.conn > C_CONNECTED && ns.disk < D_INCONSISTENT)
		rv = SS_NO_LOCAL_DISK;

	else if (ns.conn > C_CONNECTED && ns.pdsk < D_INCONSISTENT)
		rv = SS_NO_REMOTE_DISK;

	else if (ns.conn > C_CONNECTED && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if ((ns.conn == C_CONNECTED ||
		  ns.conn == C_WF_BITMAP_S ||
		  ns.conn == C_SYNC_SOURCE ||
		  ns.conn == C_PAUSED_SYNC_S) &&
		  ns.disk == D_OUTDATED)
		rv = SS_CONNECTED_OUTDATES;

	else if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) &&
		 (mdev->sync_conf.verify_alg[0] == 0))
		rv = SS_NO_VERIFY_ALG;

	else if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) &&
		  mdev->agreed_pro_version < 88)
		rv = SS_NOT_SUPPORTED;

	else if (ns.conn >= C_CONNECTED && ns.pdsk == D_UNKNOWN)
		rv = SS_CONNECTED_OUTDATES;

	return rv;
}

/**
 * is_valid_state_transition() - Returns an SS_ error code if the state transition is not possible
 * @mdev:	DRBD device.
 * @ns:		new state.
 * @os:		old state.
 */
static enum drbd_state_rv
is_valid_state_transition(struct drbd_conf *mdev, union drbd_state ns,
			  union drbd_state os)
{
	enum drbd_state_rv rv = SS_SUCCESS;

	if ((ns.conn == C_STARTING_SYNC_T || ns.conn == C_STARTING_SYNC_S) &&
	    os.conn > C_CONNECTED)
		rv = SS_RESYNC_RUNNING;

	if (ns.conn == C_DISCONNECTING && os.conn == C_STANDALONE)
		rv = SS_ALREADY_STANDALONE;

	if (ns.disk > D_ATTACHING && os.disk == D_DISKLESS)
		rv = SS_IS_DISKLESS;

	if (ns.conn == C_WF_CONNECTION && os.conn < C_UNCONNECTED)
		rv = SS_NO_NET_CONFIG;

	if (ns.disk == D_OUTDATED && os.disk < D_OUTDATED && os.disk != D_ATTACHING)
		rv = SS_LOWER_THAN_OUTDATED;

	if (ns.conn == C_DISCONNECTING && os.conn == C_UNCONNECTED)
		rv = SS_IN_TRANSIENT_STATE;

	if (ns.conn == os.conn && ns.conn == C_WF_REPORT_PARAMS)
		rv = SS_IN_TRANSIENT_STATE;

	if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) && os.conn < C_CONNECTED)
		rv = SS_NEED_CONNECTION;

	if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) &&
	    ns.conn != os.conn && os.conn > C_CONNECTED)
		rv = SS_RESYNC_RUNNING;

	if ((ns.conn == C_STARTING_SYNC_S || ns.conn == C_STARTING_SYNC_T) &&
	    os.conn < C_CONNECTED)
		rv = SS_NEED_CONNECTION;

	if ((ns.conn == C_SYNC_TARGET || ns.conn == C_SYNC_SOURCE)
	    && os.conn < C_WF_REPORT_PARAMS)
		rv = SS_NEED_CONNECTION; /* No NetworkFailure -> SyncTarget etc... */

	return rv;
}

/**
 * sanitize_state() - Resolves implicitly necessary additional changes to a state transition
 * @mdev:	DRBD device.
 * @os:		old state.
 * @ns:		new state.
 * @warn_sync_abort:
 *
 * When we loose connection, we have to set the state of the peers disk (pdsk)
 * to D_UNKNOWN. This rule and many more along those lines are in this function.
 */
static union drbd_state sanitize_state(struct drbd_conf *mdev, union drbd_state os,
				       union drbd_state ns, const char **warn_sync_abort)
{
	enum drbd_fencing_p fp;
	enum drbd_disk_state disk_min, disk_max, pdsk_min, pdsk_max;

	fp = FP_DONT_CARE;
	if (get_ldev(mdev)) {
		fp = mdev->ldev->dc.fencing;
		put_ldev(mdev);
	}

	/* Disallow Network errors to configure a device's network part */
	if ((ns.conn >= C_TIMEOUT && ns.conn <= C_TEAR_DOWN) &&
	    os.conn <= C_DISCONNECTING)
		ns.conn = os.conn;

	/* After a network error (+C_TEAR_DOWN) only C_UNCONNECTED or C_DISCONNECTING can follow.
	 * If you try to go into some Sync* state, that shall fail (elsewhere). */
	if (os.conn >= C_TIMEOUT && os.conn <= C_TEAR_DOWN &&
	    ns.conn != C_UNCONNECTED && ns.conn != C_DISCONNECTING && ns.conn <= C_TEAR_DOWN)
		ns.conn = os.conn;

	/* we cannot fail (again) if we already detached */
	if (ns.disk == D_FAILED && os.disk == D_DISKLESS)
		ns.disk = D_DISKLESS;

	/* if we are only D_ATTACHING yet,
	 * we can (and should) go directly to D_DISKLESS. */
	if (ns.disk == D_FAILED && os.disk == D_ATTACHING)
		ns.disk = D_DISKLESS;

	/* After C_DISCONNECTING only C_STANDALONE may follow */
	if (os.conn == C_DISCONNECTING && ns.conn != C_STANDALONE)
		ns.conn = os.conn;

	if (ns.conn < C_CONNECTED) {
		ns.peer_isp = 0;
		ns.peer = R_UNKNOWN;
		if (ns.pdsk > D_UNKNOWN || ns.pdsk < D_INCONSISTENT)
			ns.pdsk = D_UNKNOWN;
	}

	/* Clear the aftr_isp when becoming unconfigured */
	if (ns.conn == C_STANDALONE && ns.disk == D_DISKLESS && ns.role == R_SECONDARY)
		ns.aftr_isp = 0;

	/* Abort resync if a disk fails/detaches */
	if (os.conn > C_CONNECTED && ns.conn > C_CONNECTED &&
	    (ns.disk <= D_FAILED || ns.pdsk <= D_FAILED)) {
		if (warn_sync_abort)
			*warn_sync_abort =
				os.conn == C_VERIFY_S || os.conn == C_VERIFY_T ?
				"Online-verify" : "Resync";
		ns.conn = C_CONNECTED;
	}

	/* Connection breaks down before we finished "Negotiating" */
	if (ns.conn < C_CONNECTED && ns.disk == D_NEGOTIATING &&
	    get_ldev_if_state(mdev, D_NEGOTIATING)) {
		if (mdev->ed_uuid == mdev->ldev->md.uuid[UI_CURRENT]) {
			ns.disk = mdev->new_state_tmp.disk;
			ns.pdsk = mdev->new_state_tmp.pdsk;
		} else {
			dev_alert(DEV, "Connection lost while negotiating, no data!\n");
			ns.disk = D_DISKLESS;
			ns.pdsk = D_UNKNOWN;
		}
		put_ldev(mdev);
	}

	/* D_CONSISTENT and D_OUTDATED vanish when we get connected */
	if (ns.conn >= C_CONNECTED && ns.conn < C_AHEAD) {
		if (ns.disk == D_CONSISTENT || ns.disk == D_OUTDATED)
			ns.disk = D_UP_TO_DATE;
		if (ns.pdsk == D_CONSISTENT || ns.pdsk == D_OUTDATED)
			ns.pdsk = D_UP_TO_DATE;
	}

	/* Implications of the connection stat on the disk states */
	disk_min = D_DISKLESS;
	disk_max = D_UP_TO_DATE;
	pdsk_min = D_INCONSISTENT;
	pdsk_max = D_UNKNOWN;
	switch ((enum drbd_conns)ns.conn) {
	case C_WF_BITMAP_T:
	case C_PAUSED_SYNC_T:
	case C_STARTING_SYNC_T:
	case C_WF_SYNC_UUID:
	case C_BEHIND:
		disk_min = D_INCONSISTENT;
		disk_max = D_OUTDATED;
		pdsk_min = D_UP_TO_DATE;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_VERIFY_S:
	case C_VERIFY_T:
		disk_min = D_UP_TO_DATE;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_UP_TO_DATE;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_CONNECTED:
		disk_min = D_DISKLESS;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_DISKLESS;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_WF_BITMAP_S:
	case C_PAUSED_SYNC_S:
	case C_STARTING_SYNC_S:
	case C_AHEAD:
		disk_min = D_UP_TO_DATE;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_INCONSISTENT;
		pdsk_max = D_CONSISTENT; /* D_OUTDATED would be nice. But explicit outdate necessary*/
		break;
	case C_SYNC_TARGET:
		disk_min = D_INCONSISTENT;
		disk_max = D_INCONSISTENT;
		pdsk_min = D_UP_TO_DATE;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_SYNC_SOURCE:
		disk_min = D_UP_TO_DATE;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_INCONSISTENT;
		pdsk_max = D_INCONSISTENT;
		break;
	case C_STANDALONE:
	case C_DISCONNECTING:
	case C_UNCONNECTED:
	case C_TIMEOUT:
	case C_BROKEN_PIPE:
	case C_NETWORK_FAILURE:
	case C_PROTOCOL_ERROR:
	case C_TEAR_DOWN:
	case C_WF_CONNECTION:
	case C_WF_REPORT_PARAMS:
	case C_MASK:
		break;
	}
	if (ns.disk > disk_max)
		ns.disk = disk_max;

	if (ns.disk < disk_min) {
		dev_warn(DEV, "Implicitly set disk from %s to %s\n",
			 drbd_disk_str(ns.disk), drbd_disk_str(disk_min));
		ns.disk = disk_min;
	}
	if (ns.pdsk > pdsk_max)
		ns.pdsk = pdsk_max;

	if (ns.pdsk < pdsk_min) {
		dev_warn(DEV, "Implicitly set pdsk from %s to %s\n",
			 drbd_disk_str(ns.pdsk), drbd_disk_str(pdsk_min));
		ns.pdsk = pdsk_min;
	}

	if (fp == FP_STONITH &&
	    (ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.pdsk > D_OUTDATED) &&
	    !(os.role == R_PRIMARY && os.conn < C_CONNECTED && os.pdsk > D_OUTDATED))
		ns.susp_fen = 1; /* Suspend IO while fence-peer handler runs (peer lost) */

	if (mdev->sync_conf.on_no_data == OND_SUSPEND_IO &&
	    (ns.role == R_PRIMARY && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE) &&
	    !(os.role == R_PRIMARY && os.disk < D_UP_TO_DATE && os.pdsk < D_UP_TO_DATE))
		ns.susp_nod = 1; /* Suspend IO while no data available (no accessible data available) */

	if (ns.aftr_isp || ns.peer_isp || ns.user_isp) {
		if (ns.conn == C_SYNC_SOURCE)
			ns.conn = C_PAUSED_SYNC_S;
		if (ns.conn == C_SYNC_TARGET)
			ns.conn = C_PAUSED_SYNC_T;
	} else {
		if (ns.conn == C_PAUSED_SYNC_S)
			ns.conn = C_SYNC_SOURCE;
		if (ns.conn == C_PAUSED_SYNC_T)
			ns.conn = C_SYNC_TARGET;
	}

	return ns;
}

/* helper for __drbd_set_state */
static void set_ov_position(struct drbd_conf *mdev, enum drbd_conns cs)
{
	if (mdev->agreed_pro_version < 90)
		mdev->ov_start_sector = 0;
	mdev->rs_total = drbd_bm_bits(mdev);
	mdev->ov_position = 0;
	if (cs == C_VERIFY_T) {
		/* starting online verify from an arbitrary position
		 * does not fit well into the existing protocol.
		 * on C_VERIFY_T, we initialize ov_left and friends
		 * implicitly in receive_DataRequest once the
		 * first P_OV_REQUEST is received */
		mdev->ov_start_sector = ~(sector_t)0;
	} else {
		unsigned long bit = BM_SECT_TO_BIT(mdev->ov_start_sector);
		if (bit >= mdev->rs_total) {
			mdev->ov_start_sector =
				BM_BIT_TO_SECT(mdev->rs_total - 1);
			mdev->rs_total = 1;
		} else
			mdev->rs_total -= bit;
		mdev->ov_position = mdev->ov_start_sector;
	}
	mdev->ov_left = mdev->rs_total;
}

static void drbd_resume_al(struct drbd_conf *mdev)
{
	if (test_and_clear_bit(AL_SUSPENDED, &mdev->flags))
		dev_info(DEV, "Resumed AL updates\n");
}

/**
 * __drbd_set_state() - Set a new DRBD state
 * @mdev:	DRBD device.
 * @ns:		new state.
 * @flags:	Flags
 * @done:	Optional completion, that will get completed after the after_state_ch() finished
 *
 * Caller needs to hold req_lock, and global_state_lock. Do not call directly.
 */
enum drbd_state_rv
__drbd_set_state(struct drbd_conf *mdev, union drbd_state ns,
	         enum chg_state_flags flags, struct completion *done)
{
	union drbd_state os;
	enum drbd_state_rv rv = SS_SUCCESS;
	const char *warn_sync_abort = NULL;
	struct after_state_chg_work *ascw;

	os = mdev->state;

	ns = sanitize_state(mdev, os, ns, &warn_sync_abort);

	if (ns.i == os.i)
		return SS_NOTHING_TO_DO;

	if (!(flags & CS_HARD)) {
		/*  pre-state-change checks ; only look at ns  */
		/* See drbd_state_sw_errors in drbd_strings.c */

		rv = is_valid_state(mdev, ns);
		if (rv < SS_SUCCESS) {
			/* If the old state was illegal as well, then let
			   this happen...*/

			if (is_valid_state(mdev, os) == rv)
				rv = is_valid_state_transition(mdev, ns, os);
		} else
			rv = is_valid_state_transition(mdev, ns, os);
	}

	if (rv < SS_SUCCESS) {
		if (flags & CS_VERBOSE)
			print_st_err(mdev, os, ns, rv);
		return rv;
	}

	if (warn_sync_abort)
		dev_warn(DEV, "%s aborted.\n", warn_sync_abort);

	{
	char *pbp, pb[300];
	pbp = pb;
	*pbp = 0;
	if (ns.role != os.role)
		pbp += sprintf(pbp, "role( %s -> %s ) ",
			       drbd_role_str(os.role),
			       drbd_role_str(ns.role));
	if (ns.peer != os.peer)
		pbp += sprintf(pbp, "peer( %s -> %s ) ",
			       drbd_role_str(os.peer),
			       drbd_role_str(ns.peer));
	if (ns.conn != os.conn)
		pbp += sprintf(pbp, "conn( %s -> %s ) ",
			       drbd_conn_str(os.conn),
			       drbd_conn_str(ns.conn));
	if (ns.disk != os.disk)
		pbp += sprintf(pbp, "disk( %s -> %s ) ",
			       drbd_disk_str(os.disk),
			       drbd_disk_str(ns.disk));
	if (ns.pdsk != os.pdsk)
		pbp += sprintf(pbp, "pdsk( %s -> %s ) ",
			       drbd_disk_str(os.pdsk),
			       drbd_disk_str(ns.pdsk));
	if (is_susp(ns) != is_susp(os))
		pbp += sprintf(pbp, "susp( %d -> %d ) ",
			       is_susp(os),
			       is_susp(ns));
	if (ns.aftr_isp != os.aftr_isp)
		pbp += sprintf(pbp, "aftr_isp( %d -> %d ) ",
			       os.aftr_isp,
			       ns.aftr_isp);
	if (ns.peer_isp != os.peer_isp)
		pbp += sprintf(pbp, "peer_isp( %d -> %d ) ",
			       os.peer_isp,
			       ns.peer_isp);
	if (ns.user_isp != os.user_isp)
		pbp += sprintf(pbp, "user_isp( %d -> %d ) ",
			       os.user_isp,
			       ns.user_isp);
	dev_info(DEV, "%s\n", pb);
	}

	/* solve the race between becoming unconfigured,
	 * worker doing the cleanup, and
	 * admin reconfiguring us:
	 * on (re)configure, first set CONFIG_PENDING,
	 * then wait for a potentially exiting worker,
	 * start the worker, and schedule one no_op.
	 * then proceed with configuration.
	 */
	if (ns.disk == D_DISKLESS &&
	    ns.conn == C_STANDALONE &&
	    ns.role == R_SECONDARY &&
	    !test_and_set_bit(CONFIG_PENDING, &mdev->flags))
		set_bit(DEVICE_DYING, &mdev->flags);

	/* if we are going -> D_FAILED or D_DISKLESS, grab one extra reference
	 * on the ldev here, to be sure the transition -> D_DISKLESS resp.
	 * drbd_ldev_destroy() won't happen before our corresponding
	 * after_state_ch works run, where we put_ldev again. */
	if ((os.disk != D_FAILED && ns.disk == D_FAILED) ||
	    (os.disk != D_DISKLESS && ns.disk == D_DISKLESS))
		atomic_inc(&mdev->local_cnt);

	mdev->state = ns;

	if (os.disk == D_ATTACHING && ns.disk >= D_NEGOTIATING)
		drbd_print_uuids(mdev, "attached to UUIDs");

	wake_up(&mdev->misc_wait);
	wake_up(&mdev->state_wait);

	/* aborted verify run. log the last position */
	if ((os.conn == C_VERIFY_S || os.conn == C_VERIFY_T) &&
	    ns.conn < C_CONNECTED) {
		mdev->ov_start_sector =
			BM_BIT_TO_SECT(drbd_bm_bits(mdev) - mdev->ov_left);
		dev_info(DEV, "Online Verify reached sector %llu\n",
			(unsigned long long)mdev->ov_start_sector);
	}

	if ((os.conn == C_PAUSED_SYNC_T || os.conn == C_PAUSED_SYNC_S) &&
	    (ns.conn == C_SYNC_TARGET  || ns.conn == C_SYNC_SOURCE)) {
		dev_info(DEV, "Syncer continues.\n");
		mdev->rs_paused += (long)jiffies
				  -(long)mdev->rs_mark_time[mdev->rs_last_mark];
		if (ns.conn == C_SYNC_TARGET)
			mod_timer(&mdev->resync_timer, jiffies);
	}

	if ((os.conn == C_SYNC_TARGET  || os.conn == C_SYNC_SOURCE) &&
	    (ns.conn == C_PAUSED_SYNC_T || ns.conn == C_PAUSED_SYNC_S)) {
		dev_info(DEV, "Resync suspended\n");
		mdev->rs_mark_time[mdev->rs_last_mark] = jiffies;
	}

	if (os.conn == C_CONNECTED &&
	    (ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T)) {
		unsigned long now = jiffies;
		int i;

		set_ov_position(mdev, ns.conn);
		mdev->rs_start = now;
		mdev->rs_last_events = 0;
		mdev->rs_last_sect_ev = 0;
		mdev->ov_last_oos_size = 0;
		mdev->ov_last_oos_start = 0;

		for (i = 0; i < DRBD_SYNC_MARKS; i++) {
			mdev->rs_mark_left[i] = mdev->ov_left;
			mdev->rs_mark_time[i] = now;
		}

		drbd_rs_controller_reset(mdev);

		if (ns.conn == C_VERIFY_S) {
			dev_info(DEV, "Starting Online Verify from sector %llu\n",
					(unsigned long long)mdev->ov_position);
			mod_timer(&mdev->resync_timer, jiffies);
		}
	}

	if (get_ldev(mdev)) {
		u32 mdf = mdev->ldev->md.flags & ~(MDF_CONSISTENT|MDF_PRIMARY_IND|
						 MDF_CONNECTED_IND|MDF_WAS_UP_TO_DATE|
						 MDF_PEER_OUT_DATED|MDF_CRASHED_PRIMARY);

		if (test_bit(CRASHED_PRIMARY, &mdev->flags))
			mdf |= MDF_CRASHED_PRIMARY;
		if (mdev->state.role == R_PRIMARY ||
		    (mdev->state.pdsk < D_INCONSISTENT && mdev->state.peer == R_PRIMARY))
			mdf |= MDF_PRIMARY_IND;
		if (mdev->state.conn > C_WF_REPORT_PARAMS)
			mdf |= MDF_CONNECTED_IND;
		if (mdev->state.disk > D_INCONSISTENT)
			mdf |= MDF_CONSISTENT;
		if (mdev->state.disk > D_OUTDATED)
			mdf |= MDF_WAS_UP_TO_DATE;
		if (mdev->state.pdsk <= D_OUTDATED && mdev->state.pdsk >= D_INCONSISTENT)
			mdf |= MDF_PEER_OUT_DATED;
		if (mdf != mdev->ldev->md.flags) {
			mdev->ldev->md.flags = mdf;
			drbd_md_mark_dirty(mdev);
		}
		if (os.disk < D_CONSISTENT && ns.disk >= D_CONSISTENT)
			drbd_set_ed_uuid(mdev, mdev->ldev->md.uuid[UI_CURRENT]);
		put_ldev(mdev);
	}

	/* Peer was forced D_UP_TO_DATE & R_PRIMARY, consider to resync */
	if (os.disk == D_INCONSISTENT && os.pdsk == D_INCONSISTENT &&
	    os.peer == R_SECONDARY && ns.peer == R_PRIMARY)
		set_bit(CONSIDER_RESYNC, &mdev->flags);

	/* Receiver should clean up itself */
	if (os.conn != C_DISCONNECTING && ns.conn == C_DISCONNECTING)
		drbd_thread_stop_nowait(&mdev->receiver);

	/* Now the receiver finished cleaning up itself, it should die */
	if (os.conn != C_STANDALONE && ns.conn == C_STANDALONE)
		drbd_thread_stop_nowait(&mdev->receiver);

	/* Upon network failure, we need to restart the receiver. */
	if (os.conn > C_TEAR_DOWN &&
	    ns.conn <= C_TEAR_DOWN && ns.conn >= C_TIMEOUT)
		drbd_thread_restart_nowait(&mdev->receiver);

	/* Resume AL writing if we get a connection */
	if (os.conn < C_CONNECTED && ns.conn >= C_CONNECTED)
		drbd_resume_al(mdev);

	ascw = kmalloc(sizeof(*ascw), GFP_ATOMIC);
	if (ascw) {
		ascw->os = os;
		ascw->ns = ns;
		ascw->flags = flags;
		ascw->w.cb = w_after_state_ch;
		ascw->done = done;
		drbd_queue_work(&mdev->data.work, &ascw->w);
	} else {
		dev_warn(DEV, "Could not kmalloc an ascw\n");
	}

	return rv;
}

static int w_after_state_ch(struct drbd_conf *mdev, struct drbd_work *w, int unused)
{
	struct after_state_chg_work *ascw =
		container_of(w, struct after_state_chg_work, w);
	after_state_ch(mdev, ascw->os, ascw->ns, ascw->flags);
	if (ascw->flags & CS_WAIT_COMPLETE) {
		D_ASSERT(ascw->done != NULL);
		complete(ascw->done);
	}
	kfree(ascw);

	return 1;
}

static void abw_start_sync(struct drbd_conf *mdev, int rv)
{
	if (rv) {
		dev_err(DEV, "Writing the bitmap failed not starting resync.\n");
		_drbd_request_state(mdev, NS(conn, C_CONNECTED), CS_VERBOSE);
		return;
	}

	switch (mdev->state.conn) {
	case C_STARTING_SYNC_T:
		_drbd_request_state(mdev, NS(conn, C_WF_SYNC_UUID), CS_VERBOSE);
		break;
	case C_STARTING_SYNC_S:
		drbd_start_resync(mdev, C_SYNC_SOURCE);
		break;
	}
}

int drbd_bitmap_io_from_worker(struct drbd_conf *mdev,
		int (*io_fn)(struct drbd_conf *),
		char *why, enum bm_flag flags)
{
	int rv;

	D_ASSERT(current == mdev->worker.task);

	/* open coded non-blocking drbd_suspend_io(mdev); */
	set_bit(SUSPEND_IO, &mdev->flags);

	drbd_bm_lock(mdev, why, flags);
	rv = io_fn(mdev);
	drbd_bm_unlock(mdev);

	drbd_resume_io(mdev);

	return rv;
}

/**
 * after_state_ch() - Perform after state change actions that may sleep
 * @mdev:	DRBD device.
 * @os:		old state.
 * @ns:		new state.
 * @flags:	Flags
 */
static void after_state_ch(struct drbd_conf *mdev, union drbd_state os,
			   union drbd_state ns, enum chg_state_flags flags)
{
	enum drbd_fencing_p fp;
	enum drbd_req_event what = nothing;
	union drbd_state nsm = (union drbd_state){ .i = -1 };

	if (os.conn != C_CONNECTED && ns.conn == C_CONNECTED) {
		clear_bit(CRASHED_PRIMARY, &mdev->flags);
		if (mdev->p_uuid)
			mdev->p_uuid[UI_FLAGS] &= ~((u64)2);
	}

	fp = FP_DONT_CARE;
	if (get_ldev(mdev)) {
		fp = mdev->ldev->dc.fencing;
		put_ldev(mdev);
	}

	/* Inform userspace about the change... */
	drbd_bcast_state(mdev, ns);

	if (!(os.role == R_PRIMARY && os.disk < D_UP_TO_DATE && os.pdsk < D_UP_TO_DATE) &&
	    (ns.role == R_PRIMARY && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE))
		drbd_khelper(mdev, "pri-on-incon-degr");

	/* Here we have the actions that are performed after a
	   state change. This function might sleep */

	nsm.i = -1;
	if (ns.susp_nod) {
		if (os.conn < C_CONNECTED && ns.conn >= C_CONNECTED)
			what = resend;

		if (os.disk == D_ATTACHING && ns.disk > D_ATTACHING)
			what = restart_frozen_disk_io;

		if (what != nothing)
			nsm.susp_nod = 0;
	}

	if (ns.susp_fen) {
		/* case1: The outdate peer handler is successful: */
		if (os.pdsk > D_OUTDATED  && ns.pdsk <= D_OUTDATED) {
			tl_clear(mdev);
			if (test_bit(NEW_CUR_UUID, &mdev->flags)) {
				drbd_uuid_new_current(mdev);
				clear_bit(NEW_CUR_UUID, &mdev->flags);
			}
			spin_lock_irq(&mdev->req_lock);
			_drbd_set_state(_NS(mdev, susp_fen, 0), CS_VERBOSE, NULL);
			spin_unlock_irq(&mdev->req_lock);
		}
		/* case2: The connection was established again: */
		if (os.conn < C_CONNECTED && ns.conn >= C_CONNECTED) {
			clear_bit(NEW_CUR_UUID, &mdev->flags);
			what = resend;
			nsm.susp_fen = 0;
		}
	}

	if (what != nothing) {
		spin_lock_irq(&mdev->req_lock);
		_tl_restart(mdev, what);
		nsm.i &= mdev->state.i;
		_drbd_set_state(mdev, nsm, CS_VERBOSE, NULL);
		spin_unlock_irq(&mdev->req_lock);
	}

	/* Became sync source.  With protocol >= 96, we still need to send out
	 * the sync uuid now. Need to do that before any drbd_send_state, or
	 * the other side may go "paused sync" before receiving the sync uuids,
	 * which is unexpected. */
	if ((os.conn != C_SYNC_SOURCE && os.conn != C_PAUSED_SYNC_S) &&
	    (ns.conn == C_SYNC_SOURCE || ns.conn == C_PAUSED_SYNC_S) &&
	    mdev->agreed_pro_version >= 96 && get_ldev(mdev)) {
		drbd_gen_and_send_sync_uuid(mdev);
		put_ldev(mdev);
	}

	/* Do not change the order of the if above and the two below... */
	if (os.pdsk == D_DISKLESS && ns.pdsk > D_DISKLESS) {      /* attach on the peer */
		drbd_send_uuids(mdev);
		drbd_send_state(mdev);
	}
	/* No point in queuing send_bitmap if we don't have a connection
	 * anymore, so check also the _current_ state, not only the new state
	 * at the time this work was queued. */
	if (os.conn != C_WF_BITMAP_S && ns.conn == C_WF_BITMAP_S &&
	    mdev->state.conn == C_WF_BITMAP_S)
		drbd_queue_bitmap_io(mdev, &drbd_send_bitmap, NULL,
				"send_bitmap (WFBitMapS)",
				BM_LOCKED_TEST_ALLOWED);

	/* Lost contact to peer's copy of the data */
	if ((os.pdsk >= D_INCONSISTENT &&
	     os.pdsk != D_UNKNOWN &&
	     os.pdsk != D_OUTDATED)
	&&  (ns.pdsk < D_INCONSISTENT ||
	     ns.pdsk == D_UNKNOWN ||
	     ns.pdsk == D_OUTDATED)) {
		if (get_ldev(mdev)) {
			if ((ns.role == R_PRIMARY || ns.peer == R_PRIMARY) &&
			    mdev->ldev->md.uuid[UI_BITMAP] == 0 && ns.disk >= D_UP_TO_DATE) {
				if (is_susp(mdev->state)) {
					set_bit(NEW_CUR_UUID, &mdev->flags);
				} else {
					drbd_uuid_new_current(mdev);
					drbd_send_uuids(mdev);
				}
			}
			put_ldev(mdev);
		}
	}

	if (ns.pdsk < D_INCONSISTENT && get_ldev(mdev)) {
		if (ns.peer == R_PRIMARY && mdev->ldev->md.uuid[UI_BITMAP] == 0) {
			drbd_uuid_new_current(mdev);
			drbd_send_uuids(mdev);
		}

		/* D_DISKLESS Peer becomes secondary */
		if (os.peer == R_PRIMARY && ns.peer == R_SECONDARY)
			/* We may still be Primary ourselves.
			 * No harm done if the bitmap still changes,
			 * redirtied pages will follow later. */
			drbd_bitmap_io_from_worker(mdev, &drbd_bm_write,
				"demote diskless peer", BM_LOCKED_SET_ALLOWED);
		put_ldev(mdev);
	}

	/* Write out all changed bits on demote.
	 * Though, no need to da that just yet
	 * if there is a resync going on still */
	if (os.role == R_PRIMARY && ns.role == R_SECONDARY &&
		mdev->state.conn <= C_CONNECTED && get_ldev(mdev)) {
		/* No changes to the bitmap expected this time, so assert that,
		 * even though no harm was done if it did change. */
		drbd_bitmap_io_from_worker(mdev, &drbd_bm_write,
				"demote", BM_LOCKED_TEST_ALLOWED);
		put_ldev(mdev);
	}

	/* Last part of the attaching process ... */
	if (ns.conn >= C_CONNECTED &&
	    os.disk == D_ATTACHING && ns.disk == D_NEGOTIATING) {
		drbd_send_sizes(mdev, 0, 0);  /* to start sync... */
		drbd_send_uuids(mdev);
		drbd_send_state(mdev);
	}

	/* We want to pause/continue resync, tell peer. */
	if (ns.conn >= C_CONNECTED &&
	     ((os.aftr_isp != ns.aftr_isp) ||
	      (os.user_isp != ns.user_isp)))
		drbd_send_state(mdev);

	/* In case one of the isp bits got set, suspend other devices. */
	if ((!os.aftr_isp && !os.peer_isp && !os.user_isp) &&
	    (ns.aftr_isp || ns.peer_isp || ns.user_isp))
		suspend_other_sg(mdev);

	/* Make sure the peer gets informed about eventual state
	   changes (ISP bits) while we were in WFReportParams. */
	if (os.conn == C_WF_REPORT_PARAMS && ns.conn >= C_CONNECTED)
		drbd_send_state(mdev);

	if (os.conn != C_AHEAD && ns.conn == C_AHEAD)
		drbd_send_state(mdev);

	/* We are in the progress to start a full sync... */
	if ((os.conn != C_STARTING_SYNC_T && ns.conn == C_STARTING_SYNC_T) ||
	    (os.conn != C_STARTING_SYNC_S && ns.conn == C_STARTING_SYNC_S))
		/* no other bitmap changes expected during this phase */
		drbd_queue_bitmap_io(mdev,
			&drbd_bmio_set_n_write, &abw_start_sync,
			"set_n_write from StartingSync", BM_LOCKED_TEST_ALLOWED);

	/* We are invalidating our self... */
	if (os.conn < C_CONNECTED && ns.conn < C_CONNECTED &&
	    os.disk > D_INCONSISTENT && ns.disk == D_INCONSISTENT)
		/* other bitmap operation expected during this phase */
		drbd_queue_bitmap_io(mdev, &drbd_bmio_set_n_write, NULL,
			"set_n_write from invalidate", BM_LOCKED_MASK);

	/* first half of local IO error, failure to attach,
	 * or administrative detach */
	if (os.disk != D_FAILED && ns.disk == D_FAILED) {
		enum drbd_io_error_p eh;
		int was_io_error;
		/* corresponding get_ldev was in __drbd_set_state, to serialize
		 * our cleanup here with the transition to D_DISKLESS,
		 * so it is safe to dreference ldev here. */
		eh = mdev->ldev->dc.on_io_error;
		was_io_error = test_and_clear_bit(WAS_IO_ERROR, &mdev->flags);

		/* current state still has to be D_FAILED,
		 * there is only one way out: to D_DISKLESS,
		 * and that may only happen after our put_ldev below. */
		if (mdev->state.disk != D_FAILED)
			dev_err(DEV,
				"ASSERT FAILED: disk is %s during detach\n",
				drbd_disk_str(mdev->state.disk));

		if (drbd_send_state(mdev))
			dev_warn(DEV, "Notified peer that I am detaching my disk\n");
		else
			dev_err(DEV, "Sending state for detaching disk failed\n");

		drbd_rs_cancel_all(mdev);

		/* In case we want to get something to stable storage still,
		 * this may be the last chance.
		 * Following put_ldev may transition to D_DISKLESS. */
		drbd_md_sync(mdev);
		put_ldev(mdev);

		if (was_io_error && eh == EP_CALL_HELPER)
			drbd_khelper(mdev, "local-io-error");
	}

        /* second half of local IO error, failure to attach,
         * or administrative detach,
         * after local_cnt references have reached zero again */
        if (os.disk != D_DISKLESS && ns.disk == D_DISKLESS) {
                /* We must still be diskless,
                 * re-attach has to be serialized with this! */
                if (mdev->state.disk != D_DISKLESS)
                        dev_err(DEV,
                                "ASSERT FAILED: disk is %s while going diskless\n",
                                drbd_disk_str(mdev->state.disk));

                mdev->rs_total = 0;
                mdev->rs_failed = 0;
                atomic_set(&mdev->rs_pending_cnt, 0);

		if (drbd_send_state(mdev))
			dev_warn(DEV, "Notified peer that I'm now diskless.\n");
		/* corresponding get_ldev in __drbd_set_state
		 * this may finally trigger drbd_ldev_destroy. */
		put_ldev(mdev);
	}

	/* Notify peer that I had a local IO error, and did not detached.. */
	if (os.disk == D_UP_TO_DATE && ns.disk == D_INCONSISTENT)
		drbd_send_state(mdev);

	/* Disks got bigger while they were detached */
	if (ns.disk > D_NEGOTIATING && ns.pdsk > D_NEGOTIATING &&
	    test_and_clear_bit(RESYNC_AFTER_NEG, &mdev->flags)) {
		if (ns.conn == C_CONNECTED)
			resync_after_online_grow(mdev);
	}

	/* A resync finished or aborted, wake paused devices... */
	if ((os.conn > C_CONNECTED && ns.conn <= C_CONNECTED) ||
	    (os.peer_isp && !ns.peer_isp) ||
	    (os.user_isp && !ns.user_isp))
		resume_next_sg(mdev);

	/* sync target done with resync.  Explicitly notify peer, even though
	 * it should (at least for non-empty resyncs) already know itself. */
	if (os.disk < D_UP_TO_DATE && os.conn >= C_SYNC_SOURCE && ns.conn == C_CONNECTED)
		drbd_send_state(mdev);

	/* This triggers bitmap writeout of potentially still unwritten pages
	 * if the resync finished cleanly, or aborted because of peer disk
	 * failure, or because of connection loss.
	 * For resync aborted because of local disk failure, we cannot do
	 * any bitmap writeout anymore.
	 * No harm done if some bits change during this phase.
	 */
	if (os.conn > C_CONNECTED && ns.conn <= C_CONNECTED && get_ldev(mdev)) {
		drbd_queue_bitmap_io(mdev, &drbd_bm_write, NULL,
			"write from resync_finished", BM_LOCKED_SET_ALLOWED);
		put_ldev(mdev);
	}

	/* free tl_hash if we Got thawed and are C_STANDALONE */
	if (ns.conn == C_STANDALONE && !is_susp(ns) && mdev->tl_hash)
		drbd_free_tl_hash(mdev);

	/* Upon network connection, we need to start the receiver */
	if (os.conn == C_STANDALONE && ns.conn == C_UNCONNECTED)
		drbd_thread_start(&mdev->receiver);

	/* Terminate worker thread if we are unconfigured - it will be
	   restarted as needed... */
	if (ns.disk == D_DISKLESS &&
	    ns.conn == C_STANDALONE &&
	    ns.role == R_SECONDARY) {
		if (os.aftr_isp != ns.aftr_isp)
			resume_next_sg(mdev);
		/* set in __drbd_set_state, unless CONFIG_PENDING was set */
		if (test_bit(DEVICE_DYING, &mdev->flags))
			drbd_thread_stop_nowait(&mdev->worker);
	}

	drbd_md_sync(mdev);
}


static int drbd_thread_setup(void *arg)
{
	struct drbd_thread *thi = (struct drbd_thread *) arg;
	struct drbd_conf *mdev = thi->mdev;
	unsigned long flags;
	int retval;

=======
void tl_clear(struct drbd_connection *connection)
{
	tl_restart(connection, CONNECTION_LOST_WHILE_PENDING);
}

/**
 * tl_abort_disk_io() - Abort disk I/O for all requests for a certain device in the TL
 * @device:	DRBD device.
 */
void tl_abort_disk_io(struct drbd_device *device)
{
	struct drbd_connection *connection = first_peer_device(device)->connection;
	struct drbd_request *req, *r;

	spin_lock_irq(&connection->resource->req_lock);
	list_for_each_entry_safe(req, r, &connection->transfer_log, tl_requests) {
		if (!(req->rq_state & RQ_LOCAL_PENDING))
			continue;
		if (req->device != device)
			continue;
		_req_mod(req, ABORT_DISK_IO, NULL);
	}
	spin_unlock_irq(&connection->resource->req_lock);
}

static int drbd_thread_setup(void *arg)
{
	struct drbd_thread *thi = (struct drbd_thread *) arg;
	struct drbd_resource *resource = thi->resource;
	unsigned long flags;
	int retval;

	snprintf(current->comm, sizeof(current->comm), "drbd_%c_%s",
		 thi->name[0],
		 resource->name);

	allow_kernel_signal(DRBD_SIGKILL);
	allow_kernel_signal(SIGXCPU);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
restart:
	retval = thi->function(thi);

	spin_lock_irqsave(&thi->t_lock, flags);

<<<<<<< HEAD
	/* if the receiver has been "Exiting", the last thing it did
	 * was set the conn state to "StandAlone",
	 * if now a re-connect request comes in, conn state goes C_UNCONNECTED,
	 * and receiver thread will be "started".
	 * drbd_thread_start needs to set "Restarting" in that case.
	 * t_state check and assignment needs to be within the same spinlock,
	 * so either thread_start sees Exiting, and can remap to Restarting,
	 * or thread_start see None, and can proceed as normal.
	 */

	if (thi->t_state == Restarting) {
		dev_info(DEV, "Restarting %s\n", current->comm);
		thi->t_state = Running;
=======
	/* if the receiver has been "EXITING", the last thing it did
	 * was set the conn state to "StandAlone",
	 * if now a re-connect request comes in, conn state goes C_UNCONNECTED,
	 * and receiver thread will be "started".
	 * drbd_thread_start needs to set "RESTARTING" in that case.
	 * t_state check and assignment needs to be within the same spinlock,
	 * so either thread_start sees EXITING, and can remap to RESTARTING,
	 * or thread_start see NONE, and can proceed as normal.
	 */

	if (thi->t_state == RESTARTING) {
		drbd_info(resource, "Restarting %s thread\n", thi->name);
		thi->t_state = RUNNING;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock_irqrestore(&thi->t_lock, flags);
		goto restart;
	}

	thi->task = NULL;
<<<<<<< HEAD
	thi->t_state = None;
	smp_mb();
	complete(&thi->stop);
	spin_unlock_irqrestore(&thi->t_lock, flags);

	dev_info(DEV, "Terminating %s\n", current->comm);

	/* Release mod reference taken when thread was started */
=======
	thi->t_state = NONE;
	smp_mb();
	complete_all(&thi->stop);
	spin_unlock_irqrestore(&thi->t_lock, flags);

	drbd_info(resource, "Terminating %s\n", current->comm);

	/* Release mod reference taken when thread was started */

	if (thi->connection)
		kref_put(&thi->connection->kref, drbd_destroy_connection);
	kref_put(&resource->kref, drbd_destroy_resource);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	module_put(THIS_MODULE);
	return retval;
}

<<<<<<< HEAD
static void drbd_thread_init(struct drbd_conf *mdev, struct drbd_thread *thi,
		      int (*func) (struct drbd_thread *))
{
	spin_lock_init(&thi->t_lock);
	thi->task    = NULL;
	thi->t_state = None;
	thi->function = func;
	thi->mdev = mdev;
=======
static void drbd_thread_init(struct drbd_resource *resource, struct drbd_thread *thi,
			     int (*func) (struct drbd_thread *), const char *name)
{
	spin_lock_init(&thi->t_lock);
	thi->task    = NULL;
	thi->t_state = NONE;
	thi->function = func;
	thi->resource = resource;
	thi->connection = NULL;
	thi->name = name;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int drbd_thread_start(struct drbd_thread *thi)
{
<<<<<<< HEAD
	struct drbd_conf *mdev = thi->mdev;
	struct task_struct *nt;
	unsigned long flags;

	const char *me =
		thi == &mdev->receiver ? "receiver" :
		thi == &mdev->asender  ? "asender"  :
		thi == &mdev->worker   ? "worker"   : "NONSENSE";

=======
	struct drbd_resource *resource = thi->resource;
	struct task_struct *nt;
	unsigned long flags;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* is used from state engine doing drbd_thread_stop_nowait,
	 * while holding the req lock irqsave */
	spin_lock_irqsave(&thi->t_lock, flags);

	switch (thi->t_state) {
<<<<<<< HEAD
	case None:
		dev_info(DEV, "Starting %s thread (from %s [%d])\n",
				me, current->comm, current->pid);

		/* Get ref on module for thread - this is released when thread exits */
		if (!try_module_get(THIS_MODULE)) {
			dev_err(DEV, "Failed to get module reference in drbd_thread_start\n");
=======
	case NONE:
		drbd_info(resource, "Starting %s thread (from %s [%d])\n",
			 thi->name, current->comm, current->pid);

		/* Get ref on module for thread - this is released when thread exits */
		if (!try_module_get(THIS_MODULE)) {
			drbd_err(resource, "Failed to get module reference in drbd_thread_start\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			spin_unlock_irqrestore(&thi->t_lock, flags);
			return false;
		}

<<<<<<< HEAD
		init_completion(&thi->stop);
		D_ASSERT(thi->task == NULL);
		thi->reset_cpu_mask = 1;
		thi->t_state = Running;
=======
		kref_get(&resource->kref);
		if (thi->connection)
			kref_get(&thi->connection->kref);

		init_completion(&thi->stop);
		thi->reset_cpu_mask = 1;
		thi->t_state = RUNNING;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock_irqrestore(&thi->t_lock, flags);
		flush_signals(current); /* otherw. may get -ERESTARTNOINTR */

		nt = kthread_create(drbd_thread_setup, (void *) thi,
<<<<<<< HEAD
				    "drbd%d_%s", mdev_to_minor(mdev), me);

		if (IS_ERR(nt)) {
			dev_err(DEV, "Couldn't start thread\n");

=======
				    "drbd_%c_%s", thi->name[0], thi->resource->name);

		if (IS_ERR(nt)) {
			drbd_err(resource, "Couldn't start thread\n");

			if (thi->connection)
				kref_put(&thi->connection->kref, drbd_destroy_connection);
			kref_put(&resource->kref, drbd_destroy_resource);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			module_put(THIS_MODULE);
			return false;
		}
		spin_lock_irqsave(&thi->t_lock, flags);
		thi->task = nt;
<<<<<<< HEAD
		thi->t_state = Running;
		spin_unlock_irqrestore(&thi->t_lock, flags);
		wake_up_process(nt);
		break;
	case Exiting:
		thi->t_state = Restarting;
		dev_info(DEV, "Restarting %s thread (from %s [%d])\n",
				me, current->comm, current->pid);
		/* fall through */
	case Running:
	case Restarting:
=======
		thi->t_state = RUNNING;
		spin_unlock_irqrestore(&thi->t_lock, flags);
		wake_up_process(nt);
		break;
	case EXITING:
		thi->t_state = RESTARTING;
		drbd_info(resource, "Restarting %s thread (from %s [%d])\n",
				thi->name, current->comm, current->pid);
		fallthrough;
	case RUNNING:
	case RESTARTING:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		spin_unlock_irqrestore(&thi->t_lock, flags);
		break;
	}

	return true;
}


void _drbd_thread_stop(struct drbd_thread *thi, int restart, int wait)
{
	unsigned long flags;

<<<<<<< HEAD
	enum drbd_thread_state ns = restart ? Restarting : Exiting;
=======
	enum drbd_thread_state ns = restart ? RESTARTING : EXITING;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* may be called from state engine, holding the req lock irqsave */
	spin_lock_irqsave(&thi->t_lock, flags);

<<<<<<< HEAD
	if (thi->t_state == None) {
=======
	if (thi->t_state == NONE) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock_irqrestore(&thi->t_lock, flags);
		if (restart)
			drbd_thread_start(thi);
		return;
	}

	if (thi->t_state != ns) {
		if (thi->task == NULL) {
			spin_unlock_irqrestore(&thi->t_lock, flags);
			return;
		}

		thi->t_state = ns;
		smp_mb();
		init_completion(&thi->stop);
		if (thi->task != current)
<<<<<<< HEAD
			force_sig(DRBD_SIGKILL, thi->task);

=======
			send_sig(DRBD_SIGKILL, thi->task, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	spin_unlock_irqrestore(&thi->t_lock, flags);

	if (wait)
		wait_for_completion(&thi->stop);
}

<<<<<<< HEAD
#ifdef CONFIG_SMP
/**
 * drbd_calc_cpu_mask() - Generate CPU masks, spread over all CPUs
 * @mdev:	DRBD device.
 *
 * Forces all threads of a device onto the same CPU. This is beneficial for
 * DRBD's performance. May be overwritten by user's configuration.
 */
void drbd_calc_cpu_mask(struct drbd_conf *mdev)
{
	int ord, cpu;

	/* user override. */
	if (cpumask_weight(mdev->cpu_mask))
		return;

	ord = mdev_to_minor(mdev) % cpumask_weight(cpu_online_mask);
	for_each_online_cpu(cpu) {
		if (ord-- == 0) {
			cpumask_set_cpu(cpu, mdev->cpu_mask);
			return;
		}
	}
	/* should not be reached */
	cpumask_setall(mdev->cpu_mask);
=======
int conn_lowest_minor(struct drbd_connection *connection)
{
	struct drbd_peer_device *peer_device;
	int vnr = 0, minor = -1;

	rcu_read_lock();
	peer_device = idr_get_next(&connection->peer_devices, &vnr);
	if (peer_device)
		minor = device_to_minor(peer_device->device);
	rcu_read_unlock();

	return minor;
}

#ifdef CONFIG_SMP
/*
 * drbd_calc_cpu_mask() - Generate CPU masks, spread over all CPUs
 *
 * Forces all threads of a resource onto the same CPU. This is beneficial for
 * DRBD's performance. May be overwritten by user's configuration.
 */
static void drbd_calc_cpu_mask(cpumask_var_t *cpu_mask)
{
	unsigned int *resources_per_cpu, min_index = ~0;

	resources_per_cpu = kcalloc(nr_cpu_ids, sizeof(*resources_per_cpu),
				    GFP_KERNEL);
	if (resources_per_cpu) {
		struct drbd_resource *resource;
		unsigned int cpu, min = ~0;

		rcu_read_lock();
		for_each_resource_rcu(resource, &drbd_resources) {
			for_each_cpu(cpu, resource->cpu_mask)
				resources_per_cpu[cpu]++;
		}
		rcu_read_unlock();
		for_each_online_cpu(cpu) {
			if (resources_per_cpu[cpu] < min) {
				min = resources_per_cpu[cpu];
				min_index = cpu;
			}
		}
		kfree(resources_per_cpu);
	}
	if (min_index == ~0) {
		cpumask_setall(*cpu_mask);
		return;
	}
	cpumask_set_cpu(min_index, *cpu_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * drbd_thread_current_set_cpu() - modifies the cpu mask of the _current_ thread
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @thi:	drbd_thread object
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * call in the "main loop" of _all_ threads, no need for any mutex, current won't die
 * prematurely.
 */
<<<<<<< HEAD
void drbd_thread_current_set_cpu(struct drbd_conf *mdev)
{
	struct task_struct *p = current;
	struct drbd_thread *thi =
		p == mdev->asender.task  ? &mdev->asender  :
		p == mdev->receiver.task ? &mdev->receiver :
		p == mdev->worker.task   ? &mdev->worker   :
		NULL;
	ERR_IF(thi == NULL)
		return;
	if (!thi->reset_cpu_mask)
		return;
	thi->reset_cpu_mask = 0;
	set_cpus_allowed_ptr(p, mdev->cpu_mask);
}
#endif

/* the appropriate socket mutex must be held already */
int _drbd_send_cmd(struct drbd_conf *mdev, struct socket *sock,
			  enum drbd_packets cmd, struct p_header80 *h,
			  size_t size, unsigned msg_flags)
{
	int sent, ok;

	ERR_IF(!h) return false;
	ERR_IF(!size) return false;

	h->magic   = BE_DRBD_MAGIC;
	h->command = cpu_to_be16(cmd);
	h->length  = cpu_to_be16(size-sizeof(struct p_header80));

	sent = drbd_send(mdev, sock, h, size, msg_flags);

	ok = (sent == size);
	if (!ok && !signal_pending(current))
		dev_warn(DEV, "short sent %s size=%d sent=%d\n",
		    cmdname(cmd), (int)size, sent);
	return ok;
}

/* don't pass the socket. we may only look at it
 * when we hold the appropriate socket mutex.
 */
int drbd_send_cmd(struct drbd_conf *mdev, int use_data_socket,
		  enum drbd_packets cmd, struct p_header80 *h, size_t size)
{
	int ok = 0;
	struct socket *sock;

	if (use_data_socket) {
		mutex_lock(&mdev->data.mutex);
		sock = mdev->data.socket;
	} else {
		mutex_lock(&mdev->meta.mutex);
		sock = mdev->meta.socket;
	}

	/* drbd_disconnect() could have called drbd_free_sock()
	 * while we were waiting in down()... */
	if (likely(sock != NULL))
		ok = _drbd_send_cmd(mdev, sock, cmd, h, size, 0);

	if (use_data_socket)
		mutex_unlock(&mdev->data.mutex);
	else
		mutex_unlock(&mdev->meta.mutex);
	return ok;
}

int drbd_send_cmd2(struct drbd_conf *mdev, enum drbd_packets cmd, char *data,
		   size_t size)
{
	struct p_header80 h;
	int ok;

	h.magic   = BE_DRBD_MAGIC;
	h.command = cpu_to_be16(cmd);
	h.length  = cpu_to_be16(size);

	if (!drbd_get_data_sock(mdev))
		return 0;

	ok = (sizeof(h) ==
		drbd_send(mdev, mdev->data.socket, &h, sizeof(h), 0));
	ok = ok && (size ==
		drbd_send(mdev, mdev->data.socket, data, size, 0));

	drbd_put_data_sock(mdev);

	return ok;
}

int drbd_send_sync_param(struct drbd_conf *mdev, struct syncer_conf *sc)
{
	struct p_rs_param_95 *p;
	struct socket *sock;
	int size, rv;
	const int apv = mdev->agreed_pro_version;

	size = apv <= 87 ? sizeof(struct p_rs_param)
		: apv == 88 ? sizeof(struct p_rs_param)
			+ strlen(mdev->sync_conf.verify_alg) + 1
		: apv <= 94 ? sizeof(struct p_rs_param_89)
		: /* apv >= 95 */ sizeof(struct p_rs_param_95);

	/* used from admin command context and receiver/worker context.
	 * to avoid kmalloc, grab the socket right here,
	 * then use the pre-allocated sbuf there */
	mutex_lock(&mdev->data.mutex);
	sock = mdev->data.socket;

	if (likely(sock != NULL)) {
		enum drbd_packets cmd = apv >= 89 ? P_SYNC_PARAM89 : P_SYNC_PARAM;

		p = &mdev->data.sbuf.rs_param_95;

		/* initialize verify_alg and csums_alg */
		memset(p->verify_alg, 0, 2 * SHARED_SECRET_MAX);

		p->rate = cpu_to_be32(sc->rate);
		p->c_plan_ahead = cpu_to_be32(sc->c_plan_ahead);
		p->c_delay_target = cpu_to_be32(sc->c_delay_target);
		p->c_fill_target = cpu_to_be32(sc->c_fill_target);
		p->c_max_rate = cpu_to_be32(sc->c_max_rate);

		if (apv >= 88)
			strcpy(p->verify_alg, mdev->sync_conf.verify_alg);
		if (apv >= 89)
			strcpy(p->csums_alg, mdev->sync_conf.csums_alg);

		rv = _drbd_send_cmd(mdev, sock, cmd, &p->head, size, 0);
	} else
		rv = 0; /* not ok */

	mutex_unlock(&mdev->data.mutex);

	return rv;
}

int drbd_send_protocol(struct drbd_conf *mdev)
{
	struct p_protocol *p;
	int size, cf, rv;

	size = sizeof(struct p_protocol);

	if (mdev->agreed_pro_version >= 87)
		size += strlen(mdev->net_conf->integrity_alg) + 1;

	/* we must not recurse into our own queue,
	 * as that is blocked during handshake */
	p = kmalloc(size, GFP_NOIO);
	if (p == NULL)
		return 0;

	p->protocol      = cpu_to_be32(mdev->net_conf->wire_protocol);
	p->after_sb_0p   = cpu_to_be32(mdev->net_conf->after_sb_0p);
	p->after_sb_1p   = cpu_to_be32(mdev->net_conf->after_sb_1p);
	p->after_sb_2p   = cpu_to_be32(mdev->net_conf->after_sb_2p);
	p->two_primaries = cpu_to_be32(mdev->net_conf->two_primaries);

	cf = 0;
	if (mdev->net_conf->want_lose)
		cf |= CF_WANT_LOSE;
	if (mdev->net_conf->dry_run) {
		if (mdev->agreed_pro_version >= 92)
			cf |= CF_DRY_RUN;
		else {
			dev_err(DEV, "--dry-run is not supported by peer");
			kfree(p);
			return -1;
		}
	}
	p->conn_flags    = cpu_to_be32(cf);

	if (mdev->agreed_pro_version >= 87)
		strcpy(p->integrity_alg, mdev->net_conf->integrity_alg);

	rv = drbd_send_cmd(mdev, USE_DATA_SOCKET, P_PROTOCOL,
			   (struct p_header80 *)p, size);
	kfree(p);
	return rv;
}

int _drbd_send_uuids(struct drbd_conf *mdev, u64 uuid_flags)
{
	struct p_uuids p;
	int i;

	if (!get_ldev_if_state(mdev, D_NEGOTIATING))
		return 1;

	for (i = UI_CURRENT; i < UI_SIZE; i++)
		p.uuid[i] = mdev->ldev ? cpu_to_be64(mdev->ldev->md.uuid[i]) : 0;

	mdev->comm_bm_set = drbd_bm_total_weight(mdev);
	p.uuid[UI_SIZE] = cpu_to_be64(mdev->comm_bm_set);
	uuid_flags |= mdev->net_conf->want_lose ? 1 : 0;
	uuid_flags |= test_bit(CRASHED_PRIMARY, &mdev->flags) ? 2 : 0;
	uuid_flags |= mdev->new_state_tmp.disk == D_INCONSISTENT ? 4 : 0;
	p.uuid[UI_FLAGS] = cpu_to_be64(uuid_flags);

	put_ldev(mdev);

	return drbd_send_cmd(mdev, USE_DATA_SOCKET, P_UUIDS,
			     (struct p_header80 *)&p, sizeof(p));
}

int drbd_send_uuids(struct drbd_conf *mdev)
{
	return _drbd_send_uuids(mdev, 0);
}

int drbd_send_uuids_skip_initial_sync(struct drbd_conf *mdev)
{
	return _drbd_send_uuids(mdev, 8);
}

void drbd_print_uuids(struct drbd_conf *mdev, const char *text)
{
	if (get_ldev_if_state(mdev, D_NEGOTIATING)) {
		u64 *uuid = mdev->ldev->md.uuid;
		dev_info(DEV, "%s %016llX:%016llX:%016llX:%016llX\n",
=======
void drbd_thread_current_set_cpu(struct drbd_thread *thi)
{
	struct drbd_resource *resource = thi->resource;
	struct task_struct *p = current;

	if (!thi->reset_cpu_mask)
		return;
	thi->reset_cpu_mask = 0;
	set_cpus_allowed_ptr(p, resource->cpu_mask);
}
#else
#define drbd_calc_cpu_mask(A) ({})
#endif

/*
 * drbd_header_size  -  size of a packet header
 *
 * The header size is a multiple of 8, so any payload following the header is
 * word aligned on 64-bit architectures.  (The bitmap send and receive code
 * relies on this.)
 */
unsigned int drbd_header_size(struct drbd_connection *connection)
{
	if (connection->agreed_pro_version >= 100) {
		BUILD_BUG_ON(!IS_ALIGNED(sizeof(struct p_header100), 8));
		return sizeof(struct p_header100);
	} else {
		BUILD_BUG_ON(sizeof(struct p_header80) !=
			     sizeof(struct p_header95));
		BUILD_BUG_ON(!IS_ALIGNED(sizeof(struct p_header80), 8));
		return sizeof(struct p_header80);
	}
}

static unsigned int prepare_header80(struct p_header80 *h, enum drbd_packet cmd, int size)
{
	h->magic   = cpu_to_be32(DRBD_MAGIC);
	h->command = cpu_to_be16(cmd);
	h->length  = cpu_to_be16(size);
	return sizeof(struct p_header80);
}

static unsigned int prepare_header95(struct p_header95 *h, enum drbd_packet cmd, int size)
{
	h->magic   = cpu_to_be16(DRBD_MAGIC_BIG);
	h->command = cpu_to_be16(cmd);
	h->length = cpu_to_be32(size);
	return sizeof(struct p_header95);
}

static unsigned int prepare_header100(struct p_header100 *h, enum drbd_packet cmd,
				      int size, int vnr)
{
	h->magic = cpu_to_be32(DRBD_MAGIC_100);
	h->volume = cpu_to_be16(vnr);
	h->command = cpu_to_be16(cmd);
	h->length = cpu_to_be32(size);
	h->pad = 0;
	return sizeof(struct p_header100);
}

static unsigned int prepare_header(struct drbd_connection *connection, int vnr,
				   void *buffer, enum drbd_packet cmd, int size)
{
	if (connection->agreed_pro_version >= 100)
		return prepare_header100(buffer, cmd, size, vnr);
	else if (connection->agreed_pro_version >= 95 &&
		 size > DRBD_MAX_SIZE_H80_PACKET)
		return prepare_header95(buffer, cmd, size);
	else
		return prepare_header80(buffer, cmd, size);
}

static void *__conn_prepare_command(struct drbd_connection *connection,
				    struct drbd_socket *sock)
{
	if (!sock->socket)
		return NULL;
	return sock->sbuf + drbd_header_size(connection);
}

void *conn_prepare_command(struct drbd_connection *connection, struct drbd_socket *sock)
{
	void *p;

	mutex_lock(&sock->mutex);
	p = __conn_prepare_command(connection, sock);
	if (!p)
		mutex_unlock(&sock->mutex);

	return p;
}

void *drbd_prepare_command(struct drbd_peer_device *peer_device, struct drbd_socket *sock)
{
	return conn_prepare_command(peer_device->connection, sock);
}

static int __send_command(struct drbd_connection *connection, int vnr,
			  struct drbd_socket *sock, enum drbd_packet cmd,
			  unsigned int header_size, void *data,
			  unsigned int size)
{
	int msg_flags;
	int err;

	/*
	 * Called with @data == NULL and the size of the data blocks in @size
	 * for commands that send data blocks.  For those commands, omit the
	 * MSG_MORE flag: this will increase the likelihood that data blocks
	 * which are page aligned on the sender will end up page aligned on the
	 * receiver.
	 */
	msg_flags = data ? MSG_MORE : 0;

	header_size += prepare_header(connection, vnr, sock->sbuf, cmd,
				      header_size + size);
	err = drbd_send_all(connection, sock->socket, sock->sbuf, header_size,
			    msg_flags);
	if (data && !err)
		err = drbd_send_all(connection, sock->socket, data, size, 0);
	/* DRBD protocol "pings" are latency critical.
	 * This is supposed to trigger tcp_push_pending_frames() */
	if (!err && (cmd == P_PING || cmd == P_PING_ACK))
		tcp_sock_set_nodelay(sock->socket->sk);

	return err;
}

static int __conn_send_command(struct drbd_connection *connection, struct drbd_socket *sock,
			       enum drbd_packet cmd, unsigned int header_size,
			       void *data, unsigned int size)
{
	return __send_command(connection, 0, sock, cmd, header_size, data, size);
}

int conn_send_command(struct drbd_connection *connection, struct drbd_socket *sock,
		      enum drbd_packet cmd, unsigned int header_size,
		      void *data, unsigned int size)
{
	int err;

	err = __conn_send_command(connection, sock, cmd, header_size, data, size);
	mutex_unlock(&sock->mutex);
	return err;
}

int drbd_send_command(struct drbd_peer_device *peer_device, struct drbd_socket *sock,
		      enum drbd_packet cmd, unsigned int header_size,
		      void *data, unsigned int size)
{
	int err;

	err = __send_command(peer_device->connection, peer_device->device->vnr,
			     sock, cmd, header_size, data, size);
	mutex_unlock(&sock->mutex);
	return err;
}

int drbd_send_ping(struct drbd_connection *connection)
{
	struct drbd_socket *sock;

	sock = &connection->meta;
	if (!conn_prepare_command(connection, sock))
		return -EIO;
	return conn_send_command(connection, sock, P_PING, 0, NULL, 0);
}

int drbd_send_ping_ack(struct drbd_connection *connection)
{
	struct drbd_socket *sock;

	sock = &connection->meta;
	if (!conn_prepare_command(connection, sock))
		return -EIO;
	return conn_send_command(connection, sock, P_PING_ACK, 0, NULL, 0);
}

int drbd_send_sync_param(struct drbd_peer_device *peer_device)
{
	struct drbd_socket *sock;
	struct p_rs_param_95 *p;
	int size;
	const int apv = peer_device->connection->agreed_pro_version;
	enum drbd_packet cmd;
	struct net_conf *nc;
	struct disk_conf *dc;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;

	rcu_read_lock();
	nc = rcu_dereference(peer_device->connection->net_conf);

	size = apv <= 87 ? sizeof(struct p_rs_param)
		: apv == 88 ? sizeof(struct p_rs_param)
			+ strlen(nc->verify_alg) + 1
		: apv <= 94 ? sizeof(struct p_rs_param_89)
		: /* apv >= 95 */ sizeof(struct p_rs_param_95);

	cmd = apv >= 89 ? P_SYNC_PARAM89 : P_SYNC_PARAM;

	/* initialize verify_alg and csums_alg */
	BUILD_BUG_ON(sizeof(p->algs) != 2 * SHARED_SECRET_MAX);
	memset(&p->algs, 0, sizeof(p->algs));

	if (get_ldev(peer_device->device)) {
		dc = rcu_dereference(peer_device->device->ldev->disk_conf);
		p->resync_rate = cpu_to_be32(dc->resync_rate);
		p->c_plan_ahead = cpu_to_be32(dc->c_plan_ahead);
		p->c_delay_target = cpu_to_be32(dc->c_delay_target);
		p->c_fill_target = cpu_to_be32(dc->c_fill_target);
		p->c_max_rate = cpu_to_be32(dc->c_max_rate);
		put_ldev(peer_device->device);
	} else {
		p->resync_rate = cpu_to_be32(DRBD_RESYNC_RATE_DEF);
		p->c_plan_ahead = cpu_to_be32(DRBD_C_PLAN_AHEAD_DEF);
		p->c_delay_target = cpu_to_be32(DRBD_C_DELAY_TARGET_DEF);
		p->c_fill_target = cpu_to_be32(DRBD_C_FILL_TARGET_DEF);
		p->c_max_rate = cpu_to_be32(DRBD_C_MAX_RATE_DEF);
	}

	if (apv >= 88)
		strcpy(p->verify_alg, nc->verify_alg);
	if (apv >= 89)
		strcpy(p->csums_alg, nc->csums_alg);
	rcu_read_unlock();

	return drbd_send_command(peer_device, sock, cmd, size, NULL, 0);
}

int __drbd_send_protocol(struct drbd_connection *connection, enum drbd_packet cmd)
{
	struct drbd_socket *sock;
	struct p_protocol *p;
	struct net_conf *nc;
	int size, cf;

	sock = &connection->data;
	p = __conn_prepare_command(connection, sock);
	if (!p)
		return -EIO;

	rcu_read_lock();
	nc = rcu_dereference(connection->net_conf);

	if (nc->tentative && connection->agreed_pro_version < 92) {
		rcu_read_unlock();
		drbd_err(connection, "--dry-run is not supported by peer");
		return -EOPNOTSUPP;
	}

	size = sizeof(*p);
	if (connection->agreed_pro_version >= 87)
		size += strlen(nc->integrity_alg) + 1;

	p->protocol      = cpu_to_be32(nc->wire_protocol);
	p->after_sb_0p   = cpu_to_be32(nc->after_sb_0p);
	p->after_sb_1p   = cpu_to_be32(nc->after_sb_1p);
	p->after_sb_2p   = cpu_to_be32(nc->after_sb_2p);
	p->two_primaries = cpu_to_be32(nc->two_primaries);
	cf = 0;
	if (nc->discard_my_data)
		cf |= CF_DISCARD_MY_DATA;
	if (nc->tentative)
		cf |= CF_DRY_RUN;
	p->conn_flags    = cpu_to_be32(cf);

	if (connection->agreed_pro_version >= 87)
		strcpy(p->integrity_alg, nc->integrity_alg);
	rcu_read_unlock();

	return __conn_send_command(connection, sock, cmd, size, NULL, 0);
}

int drbd_send_protocol(struct drbd_connection *connection)
{
	int err;

	mutex_lock(&connection->data.mutex);
	err = __drbd_send_protocol(connection, P_PROTOCOL);
	mutex_unlock(&connection->data.mutex);

	return err;
}

static int _drbd_send_uuids(struct drbd_peer_device *peer_device, u64 uuid_flags)
{
	struct drbd_device *device = peer_device->device;
	struct drbd_socket *sock;
	struct p_uuids *p;
	int i;

	if (!get_ldev_if_state(device, D_NEGOTIATING))
		return 0;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p) {
		put_ldev(device);
		return -EIO;
	}
	spin_lock_irq(&device->ldev->md.uuid_lock);
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		p->uuid[i] = cpu_to_be64(device->ldev->md.uuid[i]);
	spin_unlock_irq(&device->ldev->md.uuid_lock);

	device->comm_bm_set = drbd_bm_total_weight(device);
	p->uuid[UI_SIZE] = cpu_to_be64(device->comm_bm_set);
	rcu_read_lock();
	uuid_flags |= rcu_dereference(peer_device->connection->net_conf)->discard_my_data ? 1 : 0;
	rcu_read_unlock();
	uuid_flags |= test_bit(CRASHED_PRIMARY, &device->flags) ? 2 : 0;
	uuid_flags |= device->new_state_tmp.disk == D_INCONSISTENT ? 4 : 0;
	p->uuid[UI_FLAGS] = cpu_to_be64(uuid_flags);

	put_ldev(device);
	return drbd_send_command(peer_device, sock, P_UUIDS, sizeof(*p), NULL, 0);
}

int drbd_send_uuids(struct drbd_peer_device *peer_device)
{
	return _drbd_send_uuids(peer_device, 0);
}

int drbd_send_uuids_skip_initial_sync(struct drbd_peer_device *peer_device)
{
	return _drbd_send_uuids(peer_device, 8);
}

void drbd_print_uuids(struct drbd_device *device, const char *text)
{
	if (get_ldev_if_state(device, D_NEGOTIATING)) {
		u64 *uuid = device->ldev->md.uuid;
		drbd_info(device, "%s %016llX:%016llX:%016llX:%016llX\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		     text,
		     (unsigned long long)uuid[UI_CURRENT],
		     (unsigned long long)uuid[UI_BITMAP],
		     (unsigned long long)uuid[UI_HISTORY_START],
		     (unsigned long long)uuid[UI_HISTORY_END]);
<<<<<<< HEAD
		put_ldev(mdev);
	} else {
		dev_info(DEV, "%s effective data uuid: %016llX\n",
				text,
				(unsigned long long)mdev->ed_uuid);
	}
}

int drbd_gen_and_send_sync_uuid(struct drbd_conf *mdev)
{
	struct p_rs_uuid p;
	u64 uuid;

	D_ASSERT(mdev->state.disk == D_UP_TO_DATE);

	uuid = mdev->ldev->md.uuid[UI_BITMAP] + UUID_NEW_BM_OFFSET;
	drbd_uuid_set(mdev, UI_BITMAP, uuid);
	drbd_print_uuids(mdev, "updated sync UUID");
	drbd_md_sync(mdev);
	p.uuid = cpu_to_be64(uuid);

	return drbd_send_cmd(mdev, USE_DATA_SOCKET, P_SYNC_UUID,
			     (struct p_header80 *)&p, sizeof(p));
}

int drbd_send_sizes(struct drbd_conf *mdev, int trigger_reply, enum dds_flags flags)
{
	struct p_sizes p;
	sector_t d_size, u_size;
	int q_order_type, max_bio_size;
	int ok;

	if (get_ldev_if_state(mdev, D_NEGOTIATING)) {
		D_ASSERT(mdev->ldev->backing_bdev);
		d_size = drbd_get_max_capacity(mdev->ldev);
		u_size = mdev->ldev->dc.disk_size;
		q_order_type = drbd_queue_order_type(mdev);
		max_bio_size = queue_max_hw_sectors(mdev->ldev->backing_bdev->bd_disk->queue) << 9;
		max_bio_size = min_t(int, max_bio_size, DRBD_MAX_BIO_SIZE);
		put_ldev(mdev);
	} else {
=======
		put_ldev(device);
	} else {
		drbd_info(device, "%s effective data uuid: %016llX\n",
				text,
				(unsigned long long)device->ed_uuid);
	}
}

void drbd_gen_and_send_sync_uuid(struct drbd_peer_device *peer_device)
{
	struct drbd_device *device = peer_device->device;
	struct drbd_socket *sock;
	struct p_rs_uuid *p;
	u64 uuid;

	D_ASSERT(device, device->state.disk == D_UP_TO_DATE);

	uuid = device->ldev->md.uuid[UI_BITMAP];
	if (uuid && uuid != UUID_JUST_CREATED)
		uuid = uuid + UUID_NEW_BM_OFFSET;
	else
		get_random_bytes(&uuid, sizeof(u64));
	drbd_uuid_set(device, UI_BITMAP, uuid);
	drbd_print_uuids(device, "updated sync UUID");
	drbd_md_sync(device);

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (p) {
		p->uuid = cpu_to_be64(uuid);
		drbd_send_command(peer_device, sock, P_SYNC_UUID, sizeof(*p), NULL, 0);
	}
}

int drbd_send_sizes(struct drbd_peer_device *peer_device, int trigger_reply, enum dds_flags flags)
{
	struct drbd_device *device = peer_device->device;
	struct drbd_socket *sock;
	struct p_sizes *p;
	sector_t d_size, u_size;
	int q_order_type;
	unsigned int max_bio_size;
	unsigned int packet_size;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;

	packet_size = sizeof(*p);
	if (peer_device->connection->agreed_features & DRBD_FF_WSAME)
		packet_size += sizeof(p->qlim[0]);

	memset(p, 0, packet_size);
	if (get_ldev_if_state(device, D_NEGOTIATING)) {
		struct block_device *bdev = device->ldev->backing_bdev;
		struct request_queue *q = bdev_get_queue(bdev);

		d_size = drbd_get_max_capacity(device->ldev);
		rcu_read_lock();
		u_size = rcu_dereference(device->ldev->disk_conf)->disk_size;
		rcu_read_unlock();
		q_order_type = drbd_queue_order_type(device);
		max_bio_size = queue_max_hw_sectors(q) << 9;
		max_bio_size = min(max_bio_size, DRBD_MAX_BIO_SIZE);
		p->qlim->physical_block_size =
			cpu_to_be32(bdev_physical_block_size(bdev));
		p->qlim->logical_block_size =
			cpu_to_be32(bdev_logical_block_size(bdev));
		p->qlim->alignment_offset =
			cpu_to_be32(bdev_alignment_offset(bdev));
		p->qlim->io_min = cpu_to_be32(bdev_io_min(bdev));
		p->qlim->io_opt = cpu_to_be32(bdev_io_opt(bdev));
		p->qlim->discard_enabled = !!bdev_max_discard_sectors(bdev);
		put_ldev(device);
	} else {
		struct request_queue *q = device->rq_queue;

		p->qlim->physical_block_size =
			cpu_to_be32(queue_physical_block_size(q));
		p->qlim->logical_block_size =
			cpu_to_be32(queue_logical_block_size(q));
		p->qlim->alignment_offset = 0;
		p->qlim->io_min = cpu_to_be32(queue_io_min(q));
		p->qlim->io_opt = cpu_to_be32(queue_io_opt(q));
		p->qlim->discard_enabled = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		d_size = 0;
		u_size = 0;
		q_order_type = QUEUE_ORDERED_NONE;
		max_bio_size = DRBD_MAX_BIO_SIZE; /* ... multiple BIOs per peer_request */
	}

<<<<<<< HEAD
	p.d_size = cpu_to_be64(d_size);
	p.u_size = cpu_to_be64(u_size);
	p.c_size = cpu_to_be64(trigger_reply ? 0 : drbd_get_capacity(mdev->this_bdev));
	p.max_bio_size = cpu_to_be32(max_bio_size);
	p.queue_order_type = cpu_to_be16(q_order_type);
	p.dds_flags = cpu_to_be16(flags);

	ok = drbd_send_cmd(mdev, USE_DATA_SOCKET, P_SIZES,
			   (struct p_header80 *)&p, sizeof(p));
	return ok;
}

/**
 * drbd_send_state() - Sends the drbd state to the peer
 * @mdev:	DRBD device.
 */
int drbd_send_state(struct drbd_conf *mdev)
{
	struct socket *sock;
	struct p_state p;
	int ok = 0;

	/* Grab state lock so we wont send state if we're in the middle
	 * of a cluster wide state change on another thread */
	drbd_state_lock(mdev);

	mutex_lock(&mdev->data.mutex);

	p.state = cpu_to_be32(mdev->state.i); /* Within the send mutex */
	sock = mdev->data.socket;

	if (likely(sock != NULL)) {
		ok = _drbd_send_cmd(mdev, sock, P_STATE,
				    (struct p_header80 *)&p, sizeof(p), 0);
	}

	mutex_unlock(&mdev->data.mutex);

	drbd_state_unlock(mdev);
	return ok;
}

int drbd_send_state_req(struct drbd_conf *mdev,
	union drbd_state mask, union drbd_state val)
{
	struct p_req_state p;

	p.mask    = cpu_to_be32(mask.i);
	p.val     = cpu_to_be32(val.i);

	return drbd_send_cmd(mdev, USE_DATA_SOCKET, P_STATE_CHG_REQ,
			     (struct p_header80 *)&p, sizeof(p));
}

int drbd_send_sr_reply(struct drbd_conf *mdev, enum drbd_state_rv retcode)
{
	struct p_req_state_reply p;

	p.retcode    = cpu_to_be32(retcode);

	return drbd_send_cmd(mdev, USE_META_SOCKET, P_STATE_CHG_REPLY,
			     (struct p_header80 *)&p, sizeof(p));
}

int fill_bitmap_rle_bits(struct drbd_conf *mdev,
	struct p_compressed_bm *p,
	struct bm_xfer_ctx *c)
=======
	if (peer_device->connection->agreed_pro_version <= 94)
		max_bio_size = min(max_bio_size, DRBD_MAX_SIZE_H80_PACKET);
	else if (peer_device->connection->agreed_pro_version < 100)
		max_bio_size = min(max_bio_size, DRBD_MAX_BIO_SIZE_P95);

	p->d_size = cpu_to_be64(d_size);
	p->u_size = cpu_to_be64(u_size);
	if (trigger_reply)
		p->c_size = 0;
	else
		p->c_size = cpu_to_be64(get_capacity(device->vdisk));
	p->max_bio_size = cpu_to_be32(max_bio_size);
	p->queue_order_type = cpu_to_be16(q_order_type);
	p->dds_flags = cpu_to_be16(flags);

	return drbd_send_command(peer_device, sock, P_SIZES, packet_size, NULL, 0);
}

/**
 * drbd_send_current_state() - Sends the drbd state to the peer
 * @peer_device:	DRBD peer device.
 */
int drbd_send_current_state(struct drbd_peer_device *peer_device)
{
	struct drbd_socket *sock;
	struct p_state *p;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;
	p->state = cpu_to_be32(peer_device->device->state.i); /* Within the send mutex */
	return drbd_send_command(peer_device, sock, P_STATE, sizeof(*p), NULL, 0);
}

/**
 * drbd_send_state() - After a state change, sends the new state to the peer
 * @peer_device:      DRBD peer device.
 * @state:     the state to send, not necessarily the current state.
 *
 * Each state change queues an "after_state_ch" work, which will eventually
 * send the resulting new state to the peer. If more state changes happen
 * between queuing and processing of the after_state_ch work, we still
 * want to send each intermediary state in the order it occurred.
 */
int drbd_send_state(struct drbd_peer_device *peer_device, union drbd_state state)
{
	struct drbd_socket *sock;
	struct p_state *p;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;
	p->state = cpu_to_be32(state.i); /* Within the send mutex */
	return drbd_send_command(peer_device, sock, P_STATE, sizeof(*p), NULL, 0);
}

int drbd_send_state_req(struct drbd_peer_device *peer_device, union drbd_state mask, union drbd_state val)
{
	struct drbd_socket *sock;
	struct p_req_state *p;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;
	p->mask = cpu_to_be32(mask.i);
	p->val = cpu_to_be32(val.i);
	return drbd_send_command(peer_device, sock, P_STATE_CHG_REQ, sizeof(*p), NULL, 0);
}

int conn_send_state_req(struct drbd_connection *connection, union drbd_state mask, union drbd_state val)
{
	enum drbd_packet cmd;
	struct drbd_socket *sock;
	struct p_req_state *p;

	cmd = connection->agreed_pro_version < 100 ? P_STATE_CHG_REQ : P_CONN_ST_CHG_REQ;
	sock = &connection->data;
	p = conn_prepare_command(connection, sock);
	if (!p)
		return -EIO;
	p->mask = cpu_to_be32(mask.i);
	p->val = cpu_to_be32(val.i);
	return conn_send_command(connection, sock, cmd, sizeof(*p), NULL, 0);
}

void drbd_send_sr_reply(struct drbd_peer_device *peer_device, enum drbd_state_rv retcode)
{
	struct drbd_socket *sock;
	struct p_req_state_reply *p;

	sock = &peer_device->connection->meta;
	p = drbd_prepare_command(peer_device, sock);
	if (p) {
		p->retcode = cpu_to_be32(retcode);
		drbd_send_command(peer_device, sock, P_STATE_CHG_REPLY, sizeof(*p), NULL, 0);
	}
}

void conn_send_sr_reply(struct drbd_connection *connection, enum drbd_state_rv retcode)
{
	struct drbd_socket *sock;
	struct p_req_state_reply *p;
	enum drbd_packet cmd = connection->agreed_pro_version < 100 ? P_STATE_CHG_REPLY : P_CONN_ST_CHG_REPLY;

	sock = &connection->meta;
	p = conn_prepare_command(connection, sock);
	if (p) {
		p->retcode = cpu_to_be32(retcode);
		conn_send_command(connection, sock, cmd, sizeof(*p), NULL, 0);
	}
}

static void dcbp_set_code(struct p_compressed_bm *p, enum drbd_bitmap_code code)
{
	BUG_ON(code & ~0xf);
	p->encoding = (p->encoding & ~0xf) | code;
}

static void dcbp_set_start(struct p_compressed_bm *p, int set)
{
	p->encoding = (p->encoding & ~0x80) | (set ? 0x80 : 0);
}

static void dcbp_set_pad_bits(struct p_compressed_bm *p, int n)
{
	BUG_ON(n & ~0x7);
	p->encoding = (p->encoding & (~0x7 << 4)) | (n << 4);
}

static int fill_bitmap_rle_bits(struct drbd_device *device,
			 struct p_compressed_bm *p,
			 unsigned int size,
			 struct bm_xfer_ctx *c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct bitstream bs;
	unsigned long plain_bits;
	unsigned long tmp;
	unsigned long rl;
	unsigned len;
	unsigned toggle;
<<<<<<< HEAD
	int bits;

	/* may we use this feature? */
	if ((mdev->sync_conf.use_rle == 0) ||
		(mdev->agreed_pro_version < 90))
			return 0;
=======
	int bits, use_rle;

	/* may we use this feature? */
	rcu_read_lock();
	use_rle = rcu_dereference(first_peer_device(device)->connection->net_conf)->use_rle;
	rcu_read_unlock();
	if (!use_rle || first_peer_device(device)->connection->agreed_pro_version < 90)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (c->bit_offset >= c->bm_bits)
		return 0; /* nothing to do. */

	/* use at most thus many bytes */
<<<<<<< HEAD
	bitstream_init(&bs, p->code, BM_PACKET_VLI_BYTES_MAX, 0);
	memset(p->code, 0, BM_PACKET_VLI_BYTES_MAX);
=======
	bitstream_init(&bs, p->code, size, 0);
	memset(p->code, 0, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* plain bits covered in this code string */
	plain_bits = 0;

	/* p->encoding & 0x80 stores whether the first run length is set.
	 * bit offset is implicit.
	 * start with toggle == 2 to be able to tell the first iteration */
	toggle = 2;

	/* see how much plain bits we can stuff into one packet
	 * using RLE and VLI. */
	do {
<<<<<<< HEAD
		tmp = (toggle == 0) ? _drbd_bm_find_next_zero(mdev, c->bit_offset)
				    : _drbd_bm_find_next(mdev, c->bit_offset);
=======
		tmp = (toggle == 0) ? _drbd_bm_find_next_zero(device, c->bit_offset)
				    : _drbd_bm_find_next(device, c->bit_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (tmp == -1UL)
			tmp = c->bm_bits;
		rl = tmp - c->bit_offset;

		if (toggle == 2) { /* first iteration */
			if (rl == 0) {
				/* the first checked bit was set,
				 * store start value, */
<<<<<<< HEAD
				DCBP_set_start(p, 1);
=======
				dcbp_set_start(p, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				/* but skip encoding of zero run length */
				toggle = !toggle;
				continue;
			}
<<<<<<< HEAD
			DCBP_set_start(p, 0);
=======
			dcbp_set_start(p, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		/* paranoia: catch zero runlength.
		 * can only happen if bitmap is modified while we scan it. */
		if (rl == 0) {
<<<<<<< HEAD
			dev_err(DEV, "unexpected zero runlength while encoding bitmap "
=======
			drbd_err(device, "unexpected zero runlength while encoding bitmap "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    "t:%u bo:%lu\n", toggle, c->bit_offset);
			return -1;
		}

		bits = vli_encode_bits(&bs, rl);
		if (bits == -ENOBUFS) /* buffer full */
			break;
		if (bits <= 0) {
<<<<<<< HEAD
			dev_err(DEV, "error while encoding bitmap: %d\n", bits);
=======
			drbd_err(device, "error while encoding bitmap: %d\n", bits);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		}

		toggle = !toggle;
		plain_bits += rl;
		c->bit_offset = tmp;
	} while (c->bit_offset < c->bm_bits);

	len = bs.cur.b - p->code + !!bs.cur.bit;

	if (plain_bits < (len << 3)) {
		/* incompressible with this method.
		 * we need to rewind both word and bit position. */
		c->bit_offset -= plain_bits;
		bm_xfer_ctx_bit_to_word_offset(c);
		c->bit_offset = c->word_offset * BITS_PER_LONG;
		return 0;
	}

	/* RLE + VLI was able to compress it just fine.
	 * update c->word_offset. */
	bm_xfer_ctx_bit_to_word_offset(c);

	/* store pad_bits */
<<<<<<< HEAD
	DCBP_set_pad_bits(p, (8 - bs.cur.bit) & 0x7);
=======
	dcbp_set_pad_bits(p, (8 - bs.cur.bit) & 0x7);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return len;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * send_bitmap_rle_or_plain
 *
 * Return 0 when done, 1 when another iteration is needed, and a negative error
 * code upon failure.
 */
static int
<<<<<<< HEAD
send_bitmap_rle_or_plain(struct drbd_conf *mdev,
			 struct p_header80 *h, struct bm_xfer_ctx *c)
{
	struct p_compressed_bm *p = (void*)h;
	unsigned long num_words;
	int len;
	int ok;

	len = fill_bitmap_rle_bits(mdev, p, c);

=======
send_bitmap_rle_or_plain(struct drbd_peer_device *peer_device, struct bm_xfer_ctx *c)
{
	struct drbd_device *device = peer_device->device;
	struct drbd_socket *sock = &peer_device->connection->data;
	unsigned int header_size = drbd_header_size(peer_device->connection);
	struct p_compressed_bm *p = sock->sbuf + header_size;
	int len, err;

	len = fill_bitmap_rle_bits(device, p,
			DRBD_SOCKET_BUFFER_SIZE - header_size - sizeof(*p), c);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (len < 0)
		return -EIO;

	if (len) {
<<<<<<< HEAD
		DCBP_set_code(p, RLE_VLI_Bits);
		ok = _drbd_send_cmd(mdev, mdev->data.socket, P_COMPRESSED_BITMAP, h,
			sizeof(*p) + len, 0);

		c->packets[0]++;
		c->bytes[0] += sizeof(*p) + len;
=======
		dcbp_set_code(p, RLE_VLI_Bits);
		err = __send_command(peer_device->connection, device->vnr, sock,
				     P_COMPRESSED_BITMAP, sizeof(*p) + len,
				     NULL, 0);
		c->packets[0]++;
		c->bytes[0] += header_size + sizeof(*p) + len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (c->bit_offset >= c->bm_bits)
			len = 0; /* DONE */
	} else {
		/* was not compressible.
		 * send a buffer full of plain text bits instead. */
<<<<<<< HEAD
		num_words = min_t(size_t, BM_PACKET_WORDS, c->bm_words - c->word_offset);
		len = num_words * sizeof(long);
		if (len)
			drbd_bm_get_lel(mdev, c->word_offset, num_words, (unsigned long*)h->payload);
		ok = _drbd_send_cmd(mdev, mdev->data.socket, P_BITMAP,
				   h, sizeof(struct p_header80) + len, 0);
=======
		unsigned int data_size;
		unsigned long num_words;
		unsigned long *p = sock->sbuf + header_size;

		data_size = DRBD_SOCKET_BUFFER_SIZE - header_size;
		num_words = min_t(size_t, data_size / sizeof(*p),
				  c->bm_words - c->word_offset);
		len = num_words * sizeof(*p);
		if (len)
			drbd_bm_get_lel(device, c->word_offset, num_words, p);
		err = __send_command(peer_device->connection, device->vnr, sock, P_BITMAP,
				     len, NULL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		c->word_offset += num_words;
		c->bit_offset = c->word_offset * BITS_PER_LONG;

		c->packets[1]++;
<<<<<<< HEAD
		c->bytes[1] += sizeof(struct p_header80) + len;
=======
		c->bytes[1] += header_size + len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (c->bit_offset > c->bm_bits)
			c->bit_offset = c->bm_bits;
	}
<<<<<<< HEAD
	if (ok) {
		if (len == 0) {
			INFO_bm_xfer_stats(mdev, "send", c);
=======
	if (!err) {
		if (len == 0) {
			INFO_bm_xfer_stats(peer_device, "send", c);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		} else
			return 1;
	}
	return -EIO;
}

/* See the comment at receive_bitmap() */
<<<<<<< HEAD
int _drbd_send_bitmap(struct drbd_conf *mdev)
{
	struct bm_xfer_ctx c;
	struct p_header80 *p;
	int err;

	ERR_IF(!mdev->bitmap) return false;

	/* maybe we should use some per thread scratch page,
	 * and allocate that during initial device creation? */
	p = (struct p_header80 *) __get_free_page(GFP_NOIO);
	if (!p) {
		dev_err(DEV, "failed to allocate one page buffer in %s\n", __func__);
		return false;
	}

	if (get_ldev(mdev)) {
		if (drbd_md_test_flag(mdev->ldev, MDF_FULL_SYNC)) {
			dev_info(DEV, "Writing the whole bitmap, MDF_FullSync was set.\n");
			drbd_bm_set_all(mdev);
			if (drbd_bm_write(mdev)) {
				/* write_bm did fail! Leave full sync flag set in Meta P_DATA
				 * but otherwise process as per normal - need to tell other
				 * side that a full resync is required! */
				dev_err(DEV, "Failed to write bitmap to disk!\n");
			} else {
				drbd_md_clear_flag(mdev, MDF_FULL_SYNC);
				drbd_md_sync(mdev);
			}
		}
		put_ldev(mdev);
	}

	c = (struct bm_xfer_ctx) {
		.bm_bits = drbd_bm_bits(mdev),
		.bm_words = drbd_bm_words(mdev),
	};

	do {
		err = send_bitmap_rle_or_plain(mdev, p, &c);
	} while (err > 0);

	free_page((unsigned long) p);
	return err == 0;
}

int drbd_send_bitmap(struct drbd_conf *mdev)
{
	int err;

	if (!drbd_get_data_sock(mdev))
		return -1;
	err = !_drbd_send_bitmap(mdev);
	drbd_put_data_sock(mdev);
	return err;
}

int drbd_send_b_ack(struct drbd_conf *mdev, u32 barrier_nr, u32 set_size)
{
	int ok;
	struct p_barrier_ack p;

	p.barrier  = barrier_nr;
	p.set_size = cpu_to_be32(set_size);

	if (mdev->state.conn < C_CONNECTED)
		return false;
	ok = drbd_send_cmd(mdev, USE_META_SOCKET, P_BARRIER_ACK,
			(struct p_header80 *)&p, sizeof(p));
	return ok;
=======
static int _drbd_send_bitmap(struct drbd_device *device,
			    struct drbd_peer_device *peer_device)
{
	struct bm_xfer_ctx c;
	int err;

	if (!expect(device, device->bitmap))
		return false;

	if (get_ldev(device)) {
		if (drbd_md_test_flag(device->ldev, MDF_FULL_SYNC)) {
			drbd_info(device, "Writing the whole bitmap, MDF_FullSync was set.\n");
			drbd_bm_set_all(device);
			if (drbd_bm_write(device, peer_device)) {
				/* write_bm did fail! Leave full sync flag set in Meta P_DATA
				 * but otherwise process as per normal - need to tell other
				 * side that a full resync is required! */
				drbd_err(device, "Failed to write bitmap to disk!\n");
			} else {
				drbd_md_clear_flag(device, MDF_FULL_SYNC);
				drbd_md_sync(device);
			}
		}
		put_ldev(device);
	}

	c = (struct bm_xfer_ctx) {
		.bm_bits = drbd_bm_bits(device),
		.bm_words = drbd_bm_words(device),
	};

	do {
		err = send_bitmap_rle_or_plain(peer_device, &c);
	} while (err > 0);

	return err == 0;
}

int drbd_send_bitmap(struct drbd_device *device, struct drbd_peer_device *peer_device)
{
	struct drbd_socket *sock = &peer_device->connection->data;
	int err = -1;

	mutex_lock(&sock->mutex);
	if (sock->socket)
		err = !_drbd_send_bitmap(device, peer_device);
	mutex_unlock(&sock->mutex);
	return err;
}

void drbd_send_b_ack(struct drbd_connection *connection, u32 barrier_nr, u32 set_size)
{
	struct drbd_socket *sock;
	struct p_barrier_ack *p;

	if (connection->cstate < C_WF_REPORT_PARAMS)
		return;

	sock = &connection->meta;
	p = conn_prepare_command(connection, sock);
	if (!p)
		return;
	p->barrier = barrier_nr;
	p->set_size = cpu_to_be32(set_size);
	conn_send_command(connection, sock, P_BARRIER_ACK, sizeof(*p), NULL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * _drbd_send_ack() - Sends an ack packet
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @cmd:	Packet command code.
 * @sector:	sector, needs to be in big endian byte order
 * @blksize:	size in byte, needs to be in big endian byte order
 * @block_id:	Id, big endian byte order
 */
static int _drbd_send_ack(struct drbd_conf *mdev, enum drbd_packets cmd,
			  u64 sector,
			  u32 blksize,
			  u64 block_id)
{
	int ok;
	struct p_block_ack p;

	p.sector   = sector;
	p.block_id = block_id;
	p.blksize  = blksize;
	p.seq_num  = cpu_to_be32(atomic_add_return(1, &mdev->packet_seq));

	if (!mdev->meta.socket || mdev->state.conn < C_CONNECTED)
		return false;
	ok = drbd_send_cmd(mdev, USE_META_SOCKET, cmd,
				(struct p_header80 *)&p, sizeof(p));
	return ok;
=======
 * @peer_device:	DRBD peer device.
 * @cmd:		Packet command code.
 * @sector:		sector, needs to be in big endian byte order
 * @blksize:		size in byte, needs to be in big endian byte order
 * @block_id:		Id, big endian byte order
 */
static int _drbd_send_ack(struct drbd_peer_device *peer_device, enum drbd_packet cmd,
			  u64 sector, u32 blksize, u64 block_id)
{
	struct drbd_socket *sock;
	struct p_block_ack *p;

	if (peer_device->device->state.conn < C_CONNECTED)
		return -EIO;

	sock = &peer_device->connection->meta;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;
	p->sector = sector;
	p->block_id = block_id;
	p->blksize = blksize;
	p->seq_num = cpu_to_be32(atomic_inc_return(&peer_device->device->packet_seq));
	return drbd_send_command(peer_device, sock, cmd, sizeof(*p), NULL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* dp->sector and dp->block_id already/still in network byte order,
 * data_size is payload size according to dp->head,
 * and may need to be corrected for digest size. */
<<<<<<< HEAD
int drbd_send_ack_dp(struct drbd_conf *mdev, enum drbd_packets cmd,
		     struct p_data *dp, int data_size)
{
	data_size -= (mdev->agreed_pro_version >= 87 && mdev->integrity_r_tfm) ?
		crypto_hash_digestsize(mdev->integrity_r_tfm) : 0;
	return _drbd_send_ack(mdev, cmd, dp->sector, cpu_to_be32(data_size),
			      dp->block_id);
}

int drbd_send_ack_rp(struct drbd_conf *mdev, enum drbd_packets cmd,
		     struct p_block_req *rp)
{
	return _drbd_send_ack(mdev, cmd, rp->sector, rp->blksize, rp->block_id);
=======
void drbd_send_ack_dp(struct drbd_peer_device *peer_device, enum drbd_packet cmd,
		      struct p_data *dp, int data_size)
{
	if (peer_device->connection->peer_integrity_tfm)
		data_size -= crypto_shash_digestsize(peer_device->connection->peer_integrity_tfm);
	_drbd_send_ack(peer_device, cmd, dp->sector, cpu_to_be32(data_size),
		       dp->block_id);
}

void drbd_send_ack_rp(struct drbd_peer_device *peer_device, enum drbd_packet cmd,
		      struct p_block_req *rp)
{
	_drbd_send_ack(peer_device, cmd, rp->sector, rp->blksize, rp->block_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * drbd_send_ack() - Sends an ack packet
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @cmd:	Packet command code.
 * @e:		Epoch entry.
 */
int drbd_send_ack(struct drbd_conf *mdev,
	enum drbd_packets cmd, struct drbd_epoch_entry *e)
{
	return _drbd_send_ack(mdev, cmd,
			      cpu_to_be64(e->sector),
			      cpu_to_be32(e->size),
			      e->block_id);
=======
 * @peer_device:	DRBD peer device
 * @cmd:		packet command code
 * @peer_req:		peer request
 */
int drbd_send_ack(struct drbd_peer_device *peer_device, enum drbd_packet cmd,
		  struct drbd_peer_request *peer_req)
{
	return _drbd_send_ack(peer_device, cmd,
			      cpu_to_be64(peer_req->i.sector),
			      cpu_to_be32(peer_req->i.size),
			      peer_req->block_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* This function misuses the block_id field to signal if the blocks
 * are is sync or not. */
<<<<<<< HEAD
int drbd_send_ack_ex(struct drbd_conf *mdev, enum drbd_packets cmd,
		     sector_t sector, int blksize, u64 block_id)
{
	return _drbd_send_ack(mdev, cmd,
=======
int drbd_send_ack_ex(struct drbd_peer_device *peer_device, enum drbd_packet cmd,
		     sector_t sector, int blksize, u64 block_id)
{
	return _drbd_send_ack(peer_device, cmd,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			      cpu_to_be64(sector),
			      cpu_to_be32(blksize),
			      cpu_to_be64(block_id));
}

<<<<<<< HEAD
int drbd_send_drequest(struct drbd_conf *mdev, int cmd,
		       sector_t sector, int size, u64 block_id)
{
	int ok;
	struct p_block_req p;

	p.sector   = cpu_to_be64(sector);
	p.block_id = block_id;
	p.blksize  = cpu_to_be32(size);

	ok = drbd_send_cmd(mdev, USE_DATA_SOCKET, cmd,
				(struct p_header80 *)&p, sizeof(p));
	return ok;
}

int drbd_send_drequest_csum(struct drbd_conf *mdev,
			    sector_t sector, int size,
			    void *digest, int digest_size,
			    enum drbd_packets cmd)
{
	int ok;
	struct p_block_req p;

	p.sector   = cpu_to_be64(sector);
	p.block_id = BE_DRBD_MAGIC + 0xbeef;
	p.blksize  = cpu_to_be32(size);

	p.head.magic   = BE_DRBD_MAGIC;
	p.head.command = cpu_to_be16(cmd);
	p.head.length  = cpu_to_be16(sizeof(p) - sizeof(struct p_header80) + digest_size);

	mutex_lock(&mdev->data.mutex);

	ok = (sizeof(p) == drbd_send(mdev, mdev->data.socket, &p, sizeof(p), 0));
	ok = ok && (digest_size == drbd_send(mdev, mdev->data.socket, digest, digest_size, 0));

	mutex_unlock(&mdev->data.mutex);

	return ok;
}

int drbd_send_ov_request(struct drbd_conf *mdev, sector_t sector, int size)
{
	int ok;
	struct p_block_req p;

	p.sector   = cpu_to_be64(sector);
	p.block_id = BE_DRBD_MAGIC + 0xbabe;
	p.blksize  = cpu_to_be32(size);

	ok = drbd_send_cmd(mdev, USE_DATA_SOCKET, P_OV_REQUEST,
			   (struct p_header80 *)&p, sizeof(p));
	return ok;
=======
int drbd_send_rs_deallocated(struct drbd_peer_device *peer_device,
			     struct drbd_peer_request *peer_req)
{
	struct drbd_socket *sock;
	struct p_block_desc *p;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(peer_req->i.sector);
	p->blksize = cpu_to_be32(peer_req->i.size);
	p->pad = 0;
	return drbd_send_command(peer_device, sock, P_RS_DEALLOCATED, sizeof(*p), NULL, 0);
}

int drbd_send_drequest(struct drbd_peer_device *peer_device, int cmd,
		       sector_t sector, int size, u64 block_id)
{
	struct drbd_socket *sock;
	struct p_block_req *p;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(sector);
	p->block_id = block_id;
	p->blksize = cpu_to_be32(size);
	return drbd_send_command(peer_device, sock, cmd, sizeof(*p), NULL, 0);
}

int drbd_send_drequest_csum(struct drbd_peer_device *peer_device, sector_t sector, int size,
			    void *digest, int digest_size, enum drbd_packet cmd)
{
	struct drbd_socket *sock;
	struct p_block_req *p;

	/* FIXME: Put the digest into the preallocated socket buffer.  */

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(sector);
	p->block_id = ID_SYNCER /* unused */;
	p->blksize = cpu_to_be32(size);
	return drbd_send_command(peer_device, sock, cmd, sizeof(*p), digest, digest_size);
}

int drbd_send_ov_request(struct drbd_peer_device *peer_device, sector_t sector, int size)
{
	struct drbd_socket *sock;
	struct p_block_req *p;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(sector);
	p->block_id = ID_SYNCER /* unused */;
	p->blksize = cpu_to_be32(size);
	return drbd_send_command(peer_device, sock, P_OV_REQUEST, sizeof(*p), NULL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* called on sndtimeo
 * returns false if we should retry,
 * true if we think connection is dead
 */
<<<<<<< HEAD
static int we_should_drop_the_connection(struct drbd_conf *mdev, struct socket *sock)
{
	int drop_it;
	/* long elapsed = (long)(jiffies - mdev->last_received); */

	drop_it =   mdev->meta.socket == sock
		|| !mdev->asender.task
		|| get_t_state(&mdev->asender) != Running
		|| mdev->state.conn < C_CONNECTED;
=======
static int we_should_drop_the_connection(struct drbd_connection *connection, struct socket *sock)
{
	int drop_it;
	/* long elapsed = (long)(jiffies - device->last_received); */

	drop_it =   connection->meta.socket == sock
		|| !connection->ack_receiver.task
		|| get_t_state(&connection->ack_receiver) != RUNNING
		|| connection->cstate < C_WF_REPORT_PARAMS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (drop_it)
		return true;

<<<<<<< HEAD
	drop_it = !--mdev->ko_count;
	if (!drop_it) {
		dev_err(DEV, "[%s/%d] sock_sendmsg time expired, ko = %u\n",
		       current->comm, current->pid, mdev->ko_count);
		request_ping(mdev);
	}

	return drop_it; /* && (mdev->state == R_PRIMARY) */;
=======
	drop_it = !--connection->ko_count;
	if (!drop_it) {
		drbd_err(connection, "[%s/%d] sock_sendmsg time expired, ko = %u\n",
			 current->comm, current->pid, connection->ko_count);
		request_ping(connection);
	}

	return drop_it; /* && (device->state == R_PRIMARY) */;
}

static void drbd_update_congested(struct drbd_connection *connection)
{
	struct sock *sk = connection->data.socket->sk;
	if (sk->sk_wmem_queued > sk->sk_sndbuf * 4 / 5)
		set_bit(NET_CONGESTED, &connection->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* The idea of sendpage seems to be to put some kind of reference
 * to the page into the skb, and to hand it over to the NIC. In
 * this process get_page() gets called.
 *
 * As soon as the page was really sent over the network put_page()
 * gets called by some part of the network layer. [ NIC driver? ]
 *
 * [ get_page() / put_page() increment/decrement the count. If count
 *   reaches 0 the page will be freed. ]
 *
 * This works nicely with pages from FSs.
 * But this means that in protocol A we might signal IO completion too early!
 *
 * In order not to corrupt data during a resync we must make sure
 * that we do not reuse our own buffer pages (EEs) to early, therefore
 * we have the net_ee list.
 *
 * XFS seems to have problems, still, it submits pages with page_count == 0!
 * As a workaround, we disable sendpage on pages
 * with page_count == 0 or PageSlab.
 */
<<<<<<< HEAD
static int _drbd_no_send_page(struct drbd_conf *mdev, struct page *page,
		   int offset, size_t size, unsigned msg_flags)
{
	int sent = drbd_send(mdev, mdev->data.socket, kmap(page) + offset, size, msg_flags);
	kunmap(page);
	if (sent == size)
		mdev->send_cnt += size>>9;
	return sent == size;
}

static int _drbd_send_page(struct drbd_conf *mdev, struct page *page,
		    int offset, size_t size, unsigned msg_flags)
{
	mm_segment_t oldfs = get_fs();
	int sent, ok;
	int len = size;
=======
static int _drbd_no_send_page(struct drbd_peer_device *peer_device, struct page *page,
			      int offset, size_t size, unsigned msg_flags)
{
	struct socket *socket;
	void *addr;
	int err;

	socket = peer_device->connection->data.socket;
	addr = kmap(page) + offset;
	err = drbd_send_all(peer_device->connection, socket, addr, size, msg_flags);
	kunmap(page);
	if (!err)
		peer_device->device->send_cnt += size >> 9;
	return err;
}

static int _drbd_send_page(struct drbd_peer_device *peer_device, struct page *page,
		    int offset, size_t size, unsigned msg_flags)
{
	struct socket *socket = peer_device->connection->data.socket;
	struct msghdr msg = { .msg_flags = msg_flags, };
	struct bio_vec bvec;
	int len = size;
	int err = -EIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* e.g. XFS meta- & log-data is in slab pages, which have a
	 * page_count of 0 and/or have PageSlab() set.
	 * we cannot use send_page for those, as that does get_page();
	 * put_page(); and would cause either a VM_BUG directly, or
	 * __page_cache_release a page that would actually still be referenced
	 * by someone, leading to some obscure delayed Oops somewhere else. */
<<<<<<< HEAD
	if (disable_sendpage || (page_count(page) < 1) || PageSlab(page))
		return _drbd_no_send_page(mdev, page, offset, size, msg_flags);

	msg_flags |= MSG_NOSIGNAL;
	drbd_update_congested(mdev);
	set_fs(KERNEL_DS);
	do {
		sent = mdev->data.socket->ops->sendpage(mdev->data.socket, page,
							offset, len,
							msg_flags);
		if (sent == -EAGAIN) {
			if (we_should_drop_the_connection(mdev,
							  mdev->data.socket))
				break;
			else
				continue;
		}
		if (sent <= 0) {
			dev_warn(DEV, "%s: size=%d len=%d sent=%d\n",
			     __func__, (int)size, len, sent);
=======
	if (!drbd_disable_sendpage && sendpage_ok(page))
		msg.msg_flags |= MSG_NOSIGNAL | MSG_SPLICE_PAGES;

	drbd_update_congested(peer_device->connection);
	do {
		int sent;

		bvec_set_page(&bvec, page, len, offset);
		iov_iter_bvec(&msg.msg_iter, ITER_SOURCE, &bvec, 1, len);

		sent = sock_sendmsg(socket, &msg);
		if (sent <= 0) {
			if (sent == -EAGAIN) {
				if (we_should_drop_the_connection(peer_device->connection, socket))
					break;
				continue;
			}
			drbd_warn(peer_device->device, "%s: size=%d len=%d sent=%d\n",
			     __func__, (int)size, len, sent);
			if (sent < 0)
				err = sent;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		len    -= sent;
		offset += sent;
<<<<<<< HEAD
	} while (len > 0 /* THINK && mdev->cstate >= C_CONNECTED*/);
	set_fs(oldfs);
	clear_bit(NET_CONGESTED, &mdev->flags);

	ok = (len == 0);
	if (likely(ok))
		mdev->send_cnt += size>>9;
	return ok;
}

static int _drbd_send_bio(struct drbd_conf *mdev, struct bio *bio)
{
	struct bio_vec *bvec;
	int i;
	/* hint all but last page with MSG_MORE */
	__bio_for_each_segment(bvec, bio, i, 0) {
		if (!_drbd_no_send_page(mdev, bvec->bv_page,
				     bvec->bv_offset, bvec->bv_len,
				     i == bio->bi_vcnt -1 ? 0 : MSG_MORE))
			return 0;
	}
	return 1;
}

static int _drbd_send_zc_bio(struct drbd_conf *mdev, struct bio *bio)
{
	struct bio_vec *bvec;
	int i;
	/* hint all but last page with MSG_MORE */
	__bio_for_each_segment(bvec, bio, i, 0) {
		if (!_drbd_send_page(mdev, bvec->bv_page,
				     bvec->bv_offset, bvec->bv_len,
				     i == bio->bi_vcnt -1 ? 0 : MSG_MORE))
			return 0;
	}
	return 1;
}

static int _drbd_send_zc_ee(struct drbd_conf *mdev, struct drbd_epoch_entry *e)
{
	struct page *page = e->pages;
	unsigned len = e->size;
	/* hint all but last page with MSG_MORE */
	page_chain_for_each(page) {
		unsigned l = min_t(unsigned, len, PAGE_SIZE);
		if (!_drbd_send_page(mdev, page, 0, l,
				page_chain_next(page) ? MSG_MORE : 0))
			return 0;
		len -= l;
	}
	return 1;
}

static u32 bio_flags_to_wire(struct drbd_conf *mdev, unsigned long bi_rw)
{
	if (mdev->agreed_pro_version >= 95)
		return  (bi_rw & REQ_SYNC ? DP_RW_SYNC : 0) |
			(bi_rw & REQ_FUA ? DP_FUA : 0) |
			(bi_rw & REQ_FLUSH ? DP_FLUSH : 0) |
			(bi_rw & REQ_DISCARD ? DP_DISCARD : 0);
	else
		return bi_rw & REQ_SYNC ? DP_RW_SYNC : 0;
}

/* Used to send write requests
 * R_PRIMARY -> Peer	(P_DATA)
 */
int drbd_send_dblock(struct drbd_conf *mdev, struct drbd_request *req)
{
	int ok = 1;
	struct p_data p;
	unsigned int dp_flags = 0;
	void *dgb;
	int dgs;

	if (!drbd_get_data_sock(mdev))
		return 0;

	dgs = (mdev->agreed_pro_version >= 87 && mdev->integrity_w_tfm) ?
		crypto_hash_digestsize(mdev->integrity_w_tfm) : 0;

	if (req->size <= DRBD_MAX_SIZE_H80_PACKET) {
		p.head.h80.magic   = BE_DRBD_MAGIC;
		p.head.h80.command = cpu_to_be16(P_DATA);
		p.head.h80.length  =
			cpu_to_be16(sizeof(p) - sizeof(union p_header) + dgs + req->size);
	} else {
		p.head.h95.magic   = BE_DRBD_MAGIC_BIG;
		p.head.h95.command = cpu_to_be16(P_DATA);
		p.head.h95.length  =
			cpu_to_be32(sizeof(p) - sizeof(union p_header) + dgs + req->size);
	}

	p.sector   = cpu_to_be64(req->sector);
	p.block_id = (unsigned long)req;
	p.seq_num  = cpu_to_be32(req->seq_num =
				 atomic_add_return(1, &mdev->packet_seq));

	dp_flags = bio_flags_to_wire(mdev, req->master_bio->bi_rw);

	if (mdev->state.conn >= C_SYNC_SOURCE &&
	    mdev->state.conn <= C_PAUSED_SYNC_T)
		dp_flags |= DP_MAY_SET_IN_SYNC;

	p.dp_flags = cpu_to_be32(dp_flags);
	set_bit(UNPLUG_REMOTE, &mdev->flags);
	ok = (sizeof(p) ==
		drbd_send(mdev, mdev->data.socket, &p, sizeof(p), dgs ? MSG_MORE : 0));
	if (ok && dgs) {
		dgb = mdev->int_dig_out;
		drbd_csum_bio(mdev, mdev->integrity_w_tfm, req->master_bio, dgb);
		ok = dgs == drbd_send(mdev, mdev->data.socket, dgb, dgs, 0);
	}
	if (ok) {
=======
	} while (len > 0 /* THINK && device->cstate >= C_CONNECTED*/);
	clear_bit(NET_CONGESTED, &peer_device->connection->flags);

	if (len == 0) {
		err = 0;
		peer_device->device->send_cnt += size >> 9;
	}
	return err;
}

static int _drbd_send_bio(struct drbd_peer_device *peer_device, struct bio *bio)
{
	struct bio_vec bvec;
	struct bvec_iter iter;

	/* hint all but last page with MSG_MORE */
	bio_for_each_segment(bvec, bio, iter) {
		int err;

		err = _drbd_no_send_page(peer_device, bvec.bv_page,
					 bvec.bv_offset, bvec.bv_len,
					 bio_iter_last(bvec, iter)
					 ? 0 : MSG_MORE);
		if (err)
			return err;
	}
	return 0;
}

static int _drbd_send_zc_bio(struct drbd_peer_device *peer_device, struct bio *bio)
{
	struct bio_vec bvec;
	struct bvec_iter iter;

	/* hint all but last page with MSG_MORE */
	bio_for_each_segment(bvec, bio, iter) {
		int err;

		err = _drbd_send_page(peer_device, bvec.bv_page,
				      bvec.bv_offset, bvec.bv_len,
				      bio_iter_last(bvec, iter) ? 0 : MSG_MORE);
		if (err)
			return err;
	}
	return 0;
}

static int _drbd_send_zc_ee(struct drbd_peer_device *peer_device,
			    struct drbd_peer_request *peer_req)
{
	struct page *page = peer_req->pages;
	unsigned len = peer_req->i.size;
	int err;

	/* hint all but last page with MSG_MORE */
	page_chain_for_each(page) {
		unsigned l = min_t(unsigned, len, PAGE_SIZE);

		err = _drbd_send_page(peer_device, page, 0, l,
				      page_chain_next(page) ? MSG_MORE : 0);
		if (err)
			return err;
		len -= l;
	}
	return 0;
}

static u32 bio_flags_to_wire(struct drbd_connection *connection,
			     struct bio *bio)
{
	if (connection->agreed_pro_version >= 95)
		return  (bio->bi_opf & REQ_SYNC ? DP_RW_SYNC : 0) |
			(bio->bi_opf & REQ_FUA ? DP_FUA : 0) |
			(bio->bi_opf & REQ_PREFLUSH ? DP_FLUSH : 0) |
			(bio_op(bio) == REQ_OP_DISCARD ? DP_DISCARD : 0) |
			(bio_op(bio) == REQ_OP_WRITE_ZEROES ?
			  ((connection->agreed_features & DRBD_FF_WZEROES) ?
			   (DP_ZEROES |(!(bio->bi_opf & REQ_NOUNMAP) ? DP_DISCARD : 0))
			   : DP_DISCARD)
			: 0);
	else
		return bio->bi_opf & REQ_SYNC ? DP_RW_SYNC : 0;
}

/* Used to send write or TRIM aka REQ_OP_DISCARD requests
 * R_PRIMARY -> Peer	(P_DATA, P_TRIM)
 */
int drbd_send_dblock(struct drbd_peer_device *peer_device, struct drbd_request *req)
{
	struct drbd_device *device = peer_device->device;
	struct drbd_socket *sock;
	struct p_data *p;
	void *digest_out;
	unsigned int dp_flags = 0;
	int digest_size;
	int err;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	digest_size = peer_device->connection->integrity_tfm ?
		      crypto_shash_digestsize(peer_device->connection->integrity_tfm) : 0;

	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(req->i.sector);
	p->block_id = (unsigned long)req;
	p->seq_num = cpu_to_be32(atomic_inc_return(&device->packet_seq));
	dp_flags = bio_flags_to_wire(peer_device->connection, req->master_bio);
	if (device->state.conn >= C_SYNC_SOURCE &&
	    device->state.conn <= C_PAUSED_SYNC_T)
		dp_flags |= DP_MAY_SET_IN_SYNC;
	if (peer_device->connection->agreed_pro_version >= 100) {
		if (req->rq_state & RQ_EXP_RECEIVE_ACK)
			dp_flags |= DP_SEND_RECEIVE_ACK;
		/* During resync, request an explicit write ack,
		 * even in protocol != C */
		if (req->rq_state & RQ_EXP_WRITE_ACK
		|| (dp_flags & DP_MAY_SET_IN_SYNC))
			dp_flags |= DP_SEND_WRITE_ACK;
	}
	p->dp_flags = cpu_to_be32(dp_flags);

	if (dp_flags & (DP_DISCARD|DP_ZEROES)) {
		enum drbd_packet cmd = (dp_flags & DP_ZEROES) ? P_ZEROES : P_TRIM;
		struct p_trim *t = (struct p_trim*)p;
		t->size = cpu_to_be32(req->i.size);
		err = __send_command(peer_device->connection, device->vnr, sock, cmd, sizeof(*t), NULL, 0);
		goto out;
	}
	digest_out = p + 1;

	/* our digest is still only over the payload.
	 * TRIM does not carry any payload. */
	if (digest_size)
		drbd_csum_bio(peer_device->connection->integrity_tfm, req->master_bio, digest_out);
	err = __send_command(peer_device->connection, device->vnr, sock, P_DATA,
			     sizeof(*p) + digest_size, NULL, req->i.size);
	if (!err) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* For protocol A, we have to memcpy the payload into
		 * socket buffers, as we may complete right away
		 * as soon as we handed it over to tcp, at which point the data
		 * pages may become invalid.
		 *
		 * For data-integrity enabled, we copy it as well, so we can be
		 * sure that even if the bio pages may still be modified, it
		 * won't change the data on the wire, thus if the digest checks
		 * out ok after sending on this side, but does not fit on the
		 * receiving side, we sure have detected corruption elsewhere.
		 */
<<<<<<< HEAD
		if (mdev->net_conf->wire_protocol == DRBD_PROT_A || dgs)
			ok = _drbd_send_bio(mdev, req->master_bio);
		else
			ok = _drbd_send_zc_bio(mdev, req->master_bio);

		/* double check digest, sometimes buffers have been modified in flight. */
		if (dgs > 0 && dgs <= 64) {
			/* 64 byte, 512 bit, is the largest digest size
			 * currently supported in kernel crypto. */
			unsigned char digest[64];
			drbd_csum_bio(mdev, mdev->integrity_w_tfm, req->master_bio, digest);
			if (memcmp(mdev->int_dig_out, digest, dgs)) {
				dev_warn(DEV,
					"Digest mismatch, buffer modified by upper layers during write: %llus +%u\n",
					(unsigned long long)req->sector, req->size);
			}
		} /* else if (dgs > 64) {
		     ... Be noisy about digest too large ...
		} */
	}

	drbd_put_data_sock(mdev);

	return ok;
=======
		if (!(req->rq_state & (RQ_EXP_RECEIVE_ACK | RQ_EXP_WRITE_ACK)) || digest_size)
			err = _drbd_send_bio(peer_device, req->master_bio);
		else
			err = _drbd_send_zc_bio(peer_device, req->master_bio);

		/* double check digest, sometimes buffers have been modified in flight. */
		if (digest_size > 0 && digest_size <= 64) {
			/* 64 byte, 512 bit, is the largest digest size
			 * currently supported in kernel crypto. */
			unsigned char digest[64];
			drbd_csum_bio(peer_device->connection->integrity_tfm, req->master_bio, digest);
			if (memcmp(p + 1, digest, digest_size)) {
				drbd_warn(device,
					"Digest mismatch, buffer modified by upper layers during write: %llus +%u\n",
					(unsigned long long)req->i.sector, req->i.size);
			}
		} /* else if (digest_size > 64) {
		     ... Be noisy about digest too large ...
		} */
	}
out:
	mutex_unlock(&sock->mutex);  /* locked by drbd_prepare_command() */

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* answer packet, used to send data back for read requests:
 *  Peer       -> (diskless) R_PRIMARY   (P_DATA_REPLY)
 *  C_SYNC_SOURCE -> C_SYNC_TARGET         (P_RS_DATA_REPLY)
 */
<<<<<<< HEAD
int drbd_send_block(struct drbd_conf *mdev, enum drbd_packets cmd,
		    struct drbd_epoch_entry *e)
{
	int ok;
	struct p_data p;
	void *dgb;
	int dgs;

	dgs = (mdev->agreed_pro_version >= 87 && mdev->integrity_w_tfm) ?
		crypto_hash_digestsize(mdev->integrity_w_tfm) : 0;

	if (e->size <= DRBD_MAX_SIZE_H80_PACKET) {
		p.head.h80.magic   = BE_DRBD_MAGIC;
		p.head.h80.command = cpu_to_be16(cmd);
		p.head.h80.length  =
			cpu_to_be16(sizeof(p) - sizeof(struct p_header80) + dgs + e->size);
	} else {
		p.head.h95.magic   = BE_DRBD_MAGIC_BIG;
		p.head.h95.command = cpu_to_be16(cmd);
		p.head.h95.length  =
			cpu_to_be32(sizeof(p) - sizeof(struct p_header80) + dgs + e->size);
	}

	p.sector   = cpu_to_be64(e->sector);
	p.block_id = e->block_id;
	/* p.seq_num  = 0;    No sequence numbers here.. */

	/* Only called by our kernel thread.
	 * This one may be interrupted by DRBD_SIG and/or DRBD_SIGKILL
	 * in response to admin command or module unload.
	 */
	if (!drbd_get_data_sock(mdev))
		return 0;

	ok = sizeof(p) == drbd_send(mdev, mdev->data.socket, &p, sizeof(p), dgs ? MSG_MORE : 0);
	if (ok && dgs) {
		dgb = mdev->int_dig_out;
		drbd_csum_ee(mdev, mdev->integrity_w_tfm, e, dgb);
		ok = dgs == drbd_send(mdev, mdev->data.socket, dgb, dgs, 0);
	}
	if (ok)
		ok = _drbd_send_zc_ee(mdev, e);

	drbd_put_data_sock(mdev);

	return ok;
}

int drbd_send_oos(struct drbd_conf *mdev, struct drbd_request *req)
{
	struct p_block_desc p;

	p.sector  = cpu_to_be64(req->sector);
	p.blksize = cpu_to_be32(req->size);

	return drbd_send_cmd(mdev, USE_DATA_SOCKET, P_OUT_OF_SYNC, &p.head, sizeof(p));
=======
int drbd_send_block(struct drbd_peer_device *peer_device, enum drbd_packet cmd,
		    struct drbd_peer_request *peer_req)
{
	struct drbd_device *device = peer_device->device;
	struct drbd_socket *sock;
	struct p_data *p;
	int err;
	int digest_size;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);

	digest_size = peer_device->connection->integrity_tfm ?
		      crypto_shash_digestsize(peer_device->connection->integrity_tfm) : 0;

	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(peer_req->i.sector);
	p->block_id = peer_req->block_id;
	p->seq_num = 0;  /* unused */
	p->dp_flags = 0;
	if (digest_size)
		drbd_csum_ee(peer_device->connection->integrity_tfm, peer_req, p + 1);
	err = __send_command(peer_device->connection, device->vnr, sock, cmd, sizeof(*p) + digest_size, NULL, peer_req->i.size);
	if (!err)
		err = _drbd_send_zc_ee(peer_device, peer_req);
	mutex_unlock(&sock->mutex);  /* locked by drbd_prepare_command() */

	return err;
}

int drbd_send_out_of_sync(struct drbd_peer_device *peer_device, struct drbd_request *req)
{
	struct drbd_socket *sock;
	struct p_block_desc *p;

	sock = &peer_device->connection->data;
	p = drbd_prepare_command(peer_device, sock);
	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(req->i.sector);
	p->blksize = cpu_to_be32(req->i.size);
	return drbd_send_command(peer_device, sock, P_OUT_OF_SYNC, sizeof(*p), NULL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
  drbd_send distinguishes two cases:

  Packets sent via the data socket "sock"
  and packets sent via the meta data socket "msock"

		    sock                      msock
  -----------------+-------------------------+------------------------------
  timeout           conf.timeout / 2          conf.timeout / 2
  timeout action    send a ping via msock     Abort communication
					      and close all sockets
*/

/*
 * you must have down()ed the appropriate [m]sock_mutex elsewhere!
 */
<<<<<<< HEAD
int drbd_send(struct drbd_conf *mdev, struct socket *sock,
	      void *buf, size_t size, unsigned msg_flags)
{
	struct kvec iov;
	struct msghdr msg;
	int rv, sent = 0;

	if (!sock)
		return -1000;

	/* THINK  if (signal_pending) return ... ? */

	iov.iov_base = buf;
	iov.iov_len  = size;

	msg.msg_name       = NULL;
	msg.msg_namelen    = 0;
	msg.msg_control    = NULL;
	msg.msg_controllen = 0;
	msg.msg_flags      = msg_flags | MSG_NOSIGNAL;

	if (sock == mdev->data.socket) {
		mdev->ko_count = mdev->net_conf->ko_count;
		drbd_update_congested(mdev);
	}
	do {
		/* STRANGE
		 * tcp_sendmsg does _not_ use its size parameter at all ?
		 *
		 * -EAGAIN on timeout, -EINTR on signal.
		 */
/* THINK
 * do we need to block DRBD_SIG if sock == &meta.socket ??
 * otherwise wake_asender() might interrupt some send_*Ack !
 */
		rv = kernel_sendmsg(sock, &msg, &iov, 1, size);
		if (rv == -EAGAIN) {
			if (we_should_drop_the_connection(mdev, sock))
=======
int drbd_send(struct drbd_connection *connection, struct socket *sock,
	      void *buf, size_t size, unsigned msg_flags)
{
	struct kvec iov = {.iov_base = buf, .iov_len = size};
	struct msghdr msg = {.msg_flags = msg_flags | MSG_NOSIGNAL};
	int rv, sent = 0;

	if (!sock)
		return -EBADR;

	/* THINK  if (signal_pending) return ... ? */

	iov_iter_kvec(&msg.msg_iter, ITER_SOURCE, &iov, 1, size);

	if (sock == connection->data.socket) {
		rcu_read_lock();
		connection->ko_count = rcu_dereference(connection->net_conf)->ko_count;
		rcu_read_unlock();
		drbd_update_congested(connection);
	}
	do {
		rv = sock_sendmsg(sock, &msg);
		if (rv == -EAGAIN) {
			if (we_should_drop_the_connection(connection, sock))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			else
				continue;
		}
<<<<<<< HEAD
		D_ASSERT(rv != 0);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rv == -EINTR) {
			flush_signals(current);
			rv = 0;
		}
		if (rv < 0)
			break;
		sent += rv;
<<<<<<< HEAD
		iov.iov_base += rv;
		iov.iov_len  -= rv;
	} while (sent < size);

	if (sock == mdev->data.socket)
		clear_bit(NET_CONGESTED, &mdev->flags);

	if (rv <= 0) {
		if (rv != -EAGAIN) {
			dev_err(DEV, "%s_sendmsg returned %d\n",
			    sock == mdev->meta.socket ? "msock" : "sock",
			    rv);
			drbd_force_state(mdev, NS(conn, C_BROKEN_PIPE));
		} else
			drbd_force_state(mdev, NS(conn, C_TIMEOUT));
=======
	} while (sent < size);

	if (sock == connection->data.socket)
		clear_bit(NET_CONGESTED, &connection->flags);

	if (rv <= 0) {
		if (rv != -EAGAIN) {
			drbd_err(connection, "%s_sendmsg returned %d\n",
				 sock == connection->meta.socket ? "msock" : "sock",
				 rv);
			conn_request_state(connection, NS(conn, C_BROKEN_PIPE), CS_HARD);
		} else
			conn_request_state(connection, NS(conn, C_TIMEOUT), CS_HARD);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return sent;
}

<<<<<<< HEAD
static int drbd_open(struct block_device *bdev, fmode_t mode)
{
	struct drbd_conf *mdev = bdev->bd_disk->private_data;
=======
/*
 * drbd_send_all  -  Send an entire buffer
 *
 * Returns 0 upon success and a negative error value otherwise.
 */
int drbd_send_all(struct drbd_connection *connection, struct socket *sock, void *buffer,
		  size_t size, unsigned msg_flags)
{
	int err;

	err = drbd_send(connection, sock, buffer, size, msg_flags);
	if (err < 0)
		return err;
	if (err != size)
		return -EIO;
	return 0;
}

static int drbd_open(struct gendisk *disk, blk_mode_t mode)
{
	struct drbd_device *device = disk->private_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	int rv = 0;

	mutex_lock(&drbd_main_mutex);
<<<<<<< HEAD
	spin_lock_irqsave(&mdev->req_lock, flags);
	/* to have a stable mdev->state.role
	 * and no race with updating open_cnt */

	if (mdev->state.role != R_PRIMARY) {
		if (mode & FMODE_WRITE)
			rv = -EROFS;
		else if (!allow_oos)
=======
	spin_lock_irqsave(&device->resource->req_lock, flags);
	/* to have a stable device->state.role
	 * and no race with updating open_cnt */

	if (device->state.role != R_PRIMARY) {
		if (mode & BLK_OPEN_WRITE)
			rv = -EROFS;
		else if (!drbd_allow_oos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rv = -EMEDIUMTYPE;
	}

	if (!rv)
<<<<<<< HEAD
		mdev->open_cnt++;
	spin_unlock_irqrestore(&mdev->req_lock, flags);
=======
		device->open_cnt++;
	spin_unlock_irqrestore(&device->resource->req_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&drbd_main_mutex);

	return rv;
}

<<<<<<< HEAD
static int drbd_release(struct gendisk *gd, fmode_t mode)
{
	struct drbd_conf *mdev = gd->private_data;
	mutex_lock(&drbd_main_mutex);
	mdev->open_cnt--;
	mutex_unlock(&drbd_main_mutex);
	return 0;
}

static void drbd_set_defaults(struct drbd_conf *mdev)
{
	/* This way we get a compile error when sync_conf grows,
	   and we forgot to initialize it here */
	mdev->sync_conf = (struct syncer_conf) {
		/* .rate = */		DRBD_RATE_DEF,
		/* .after = */		DRBD_AFTER_DEF,
		/* .al_extents = */	DRBD_AL_EXTENTS_DEF,
		/* .verify_alg = */	{}, 0,
		/* .cpu_mask = */	{}, 0,
		/* .csums_alg = */	{}, 0,
		/* .use_rle = */	0,
		/* .on_no_data = */	DRBD_ON_NO_DATA_DEF,
		/* .c_plan_ahead = */	DRBD_C_PLAN_AHEAD_DEF,
		/* .c_delay_target = */	DRBD_C_DELAY_TARGET_DEF,
		/* .c_fill_target = */	DRBD_C_FILL_TARGET_DEF,
		/* .c_max_rate = */	DRBD_C_MAX_RATE_DEF,
		/* .c_min_rate = */	DRBD_C_MIN_RATE_DEF
	};

	/* Have to use that way, because the layout differs between
	   big endian and little endian */
	mdev->state = (union drbd_state) {
=======
static void drbd_release(struct gendisk *gd)
{
	struct drbd_device *device = gd->private_data;

	mutex_lock(&drbd_main_mutex);
	device->open_cnt--;
	mutex_unlock(&drbd_main_mutex);
}

/* need to hold resource->req_lock */
void drbd_queue_unplug(struct drbd_device *device)
{
	if (device->state.pdsk >= D_INCONSISTENT && device->state.conn >= C_CONNECTED) {
		D_ASSERT(device, device->state.role == R_PRIMARY);
		if (test_and_clear_bit(UNPLUG_REMOTE, &device->flags)) {
			drbd_queue_work_if_unqueued(
				&first_peer_device(device)->connection->sender_work,
				&device->unplug_work);
		}
	}
}

static void drbd_set_defaults(struct drbd_device *device)
{
	/* Beware! The actual layout differs
	 * between big endian and little endian */
	device->state = (union drbd_dev_state) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		{ .role = R_SECONDARY,
		  .peer = R_UNKNOWN,
		  .conn = C_STANDALONE,
		  .disk = D_DISKLESS,
		  .pdsk = D_UNKNOWN,
<<<<<<< HEAD
		  .susp = 0,
		  .susp_nod = 0,
		  .susp_fen = 0
		} };
}

void drbd_init_set_defaults(struct drbd_conf *mdev)
=======
		} };
}

void drbd_init_set_defaults(struct drbd_device *device)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* the memset(,0,) did most of this.
	 * note: only assignments, no allocation in here */

<<<<<<< HEAD
	drbd_set_defaults(mdev);

	atomic_set(&mdev->ap_bio_cnt, 0);
	atomic_set(&mdev->ap_pending_cnt, 0);
	atomic_set(&mdev->rs_pending_cnt, 0);
	atomic_set(&mdev->unacked_cnt, 0);
	atomic_set(&mdev->local_cnt, 0);
	atomic_set(&mdev->net_cnt, 0);
	atomic_set(&mdev->packet_seq, 0);
	atomic_set(&mdev->pp_in_use, 0);
	atomic_set(&mdev->pp_in_use_by_net, 0);
	atomic_set(&mdev->rs_sect_in, 0);
	atomic_set(&mdev->rs_sect_ev, 0);
	atomic_set(&mdev->ap_in_flight, 0);

	mutex_init(&mdev->md_io_mutex);
	mutex_init(&mdev->data.mutex);
	mutex_init(&mdev->meta.mutex);
	sema_init(&mdev->data.work.s, 0);
	sema_init(&mdev->meta.work.s, 0);
	mutex_init(&mdev->state_mutex);

	spin_lock_init(&mdev->data.work.q_lock);
	spin_lock_init(&mdev->meta.work.q_lock);

	spin_lock_init(&mdev->al_lock);
	spin_lock_init(&mdev->req_lock);
	spin_lock_init(&mdev->peer_seq_lock);
	spin_lock_init(&mdev->epoch_lock);

	INIT_LIST_HEAD(&mdev->active_ee);
	INIT_LIST_HEAD(&mdev->sync_ee);
	INIT_LIST_HEAD(&mdev->done_ee);
	INIT_LIST_HEAD(&mdev->read_ee);
	INIT_LIST_HEAD(&mdev->net_ee);
	INIT_LIST_HEAD(&mdev->resync_reads);
	INIT_LIST_HEAD(&mdev->data.work.q);
	INIT_LIST_HEAD(&mdev->meta.work.q);
	INIT_LIST_HEAD(&mdev->resync_work.list);
	INIT_LIST_HEAD(&mdev->unplug_work.list);
	INIT_LIST_HEAD(&mdev->go_diskless.list);
	INIT_LIST_HEAD(&mdev->md_sync_work.list);
	INIT_LIST_HEAD(&mdev->start_resync_work.list);
	INIT_LIST_HEAD(&mdev->bm_io_work.w.list);

	mdev->resync_work.cb  = w_resync_timer;
	mdev->unplug_work.cb  = w_send_write_hint;
	mdev->go_diskless.cb  = w_go_diskless;
	mdev->md_sync_work.cb = w_md_sync;
	mdev->bm_io_work.w.cb = w_bitmap_io;
	mdev->start_resync_work.cb = w_start_resync;
	init_timer(&mdev->resync_timer);
	init_timer(&mdev->md_sync_timer);
	init_timer(&mdev->start_resync_timer);
	init_timer(&mdev->request_timer);
	mdev->resync_timer.function = resync_timer_fn;
	mdev->resync_timer.data = (unsigned long) mdev;
	mdev->md_sync_timer.function = md_sync_timer_fn;
	mdev->md_sync_timer.data = (unsigned long) mdev;
	mdev->start_resync_timer.function = start_resync_timer_fn;
	mdev->start_resync_timer.data = (unsigned long) mdev;
	mdev->request_timer.function = request_timer_fn;
	mdev->request_timer.data = (unsigned long) mdev;

	init_waitqueue_head(&mdev->misc_wait);
	init_waitqueue_head(&mdev->state_wait);
	init_waitqueue_head(&mdev->net_cnt_wait);
	init_waitqueue_head(&mdev->ee_wait);
	init_waitqueue_head(&mdev->al_wait);
	init_waitqueue_head(&mdev->seq_wait);

	drbd_thread_init(mdev, &mdev->receiver, drbdd_init);
	drbd_thread_init(mdev, &mdev->worker, drbd_worker);
	drbd_thread_init(mdev, &mdev->asender, drbd_asender);

	mdev->agreed_pro_version = PRO_VERSION_MAX;
	mdev->write_ordering = WO_bdev_flush;
	mdev->resync_wenr = LC_FREE;
	mdev->peer_max_bio_size = DRBD_MAX_BIO_SIZE_SAFE;
	mdev->local_max_bio_size = DRBD_MAX_BIO_SIZE_SAFE;
}

void drbd_mdev_cleanup(struct drbd_conf *mdev)
{
	int i;
	if (mdev->receiver.t_state != None)
		dev_err(DEV, "ASSERT FAILED: receiver t_state == %d expected 0.\n",
				mdev->receiver.t_state);

	/* no need to lock it, I'm the only thread alive */
	if (atomic_read(&mdev->current_epoch->epoch_size) !=  0)
		dev_err(DEV, "epoch_size:%d\n", atomic_read(&mdev->current_epoch->epoch_size));
	mdev->al_writ_cnt  =
	mdev->bm_writ_cnt  =
	mdev->read_cnt     =
	mdev->recv_cnt     =
	mdev->send_cnt     =
	mdev->writ_cnt     =
	mdev->p_size       =
	mdev->rs_start     =
	mdev->rs_total     =
	mdev->rs_failed    = 0;
	mdev->rs_last_events = 0;
	mdev->rs_last_sect_ev = 0;
	for (i = 0; i < DRBD_SYNC_MARKS; i++) {
		mdev->rs_mark_left[i] = 0;
		mdev->rs_mark_time[i] = 0;
	}
	D_ASSERT(mdev->net_conf == NULL);

	drbd_set_my_capacity(mdev, 0);
	if (mdev->bitmap) {
		/* maybe never allocated. */
		drbd_bm_resize(mdev, 0, 1);
		drbd_bm_cleanup(mdev);
	}

	drbd_free_resources(mdev);
	clear_bit(AL_SUSPENDED, &mdev->flags);

	/*
	 * currently we drbd_init_ee only on module load, so
	 * we may do drbd_release_ee only on module unload!
	 */
	D_ASSERT(list_empty(&mdev->active_ee));
	D_ASSERT(list_empty(&mdev->sync_ee));
	D_ASSERT(list_empty(&mdev->done_ee));
	D_ASSERT(list_empty(&mdev->read_ee));
	D_ASSERT(list_empty(&mdev->net_ee));
	D_ASSERT(list_empty(&mdev->resync_reads));
	D_ASSERT(list_empty(&mdev->data.work.q));
	D_ASSERT(list_empty(&mdev->meta.work.q));
	D_ASSERT(list_empty(&mdev->resync_work.list));
	D_ASSERT(list_empty(&mdev->unplug_work.list));
	D_ASSERT(list_empty(&mdev->go_diskless.list));

	drbd_set_defaults(mdev);
=======
	drbd_set_defaults(device);

	atomic_set(&device->ap_bio_cnt, 0);
	atomic_set(&device->ap_actlog_cnt, 0);
	atomic_set(&device->ap_pending_cnt, 0);
	atomic_set(&device->rs_pending_cnt, 0);
	atomic_set(&device->unacked_cnt, 0);
	atomic_set(&device->local_cnt, 0);
	atomic_set(&device->pp_in_use_by_net, 0);
	atomic_set(&device->rs_sect_in, 0);
	atomic_set(&device->rs_sect_ev, 0);
	atomic_set(&device->ap_in_flight, 0);
	atomic_set(&device->md_io.in_use, 0);

	mutex_init(&device->own_state_mutex);
	device->state_mutex = &device->own_state_mutex;

	spin_lock_init(&device->al_lock);
	spin_lock_init(&device->peer_seq_lock);

	INIT_LIST_HEAD(&device->active_ee);
	INIT_LIST_HEAD(&device->sync_ee);
	INIT_LIST_HEAD(&device->done_ee);
	INIT_LIST_HEAD(&device->read_ee);
	INIT_LIST_HEAD(&device->net_ee);
	INIT_LIST_HEAD(&device->resync_reads);
	INIT_LIST_HEAD(&device->resync_work.list);
	INIT_LIST_HEAD(&device->unplug_work.list);
	INIT_LIST_HEAD(&device->bm_io_work.w.list);
	INIT_LIST_HEAD(&device->pending_master_completion[0]);
	INIT_LIST_HEAD(&device->pending_master_completion[1]);
	INIT_LIST_HEAD(&device->pending_completion[0]);
	INIT_LIST_HEAD(&device->pending_completion[1]);

	device->resync_work.cb  = w_resync_timer;
	device->unplug_work.cb  = w_send_write_hint;
	device->bm_io_work.w.cb = w_bitmap_io;

	timer_setup(&device->resync_timer, resync_timer_fn, 0);
	timer_setup(&device->md_sync_timer, md_sync_timer_fn, 0);
	timer_setup(&device->start_resync_timer, start_resync_timer_fn, 0);
	timer_setup(&device->request_timer, request_timer_fn, 0);

	init_waitqueue_head(&device->misc_wait);
	init_waitqueue_head(&device->state_wait);
	init_waitqueue_head(&device->ee_wait);
	init_waitqueue_head(&device->al_wait);
	init_waitqueue_head(&device->seq_wait);

	device->resync_wenr = LC_FREE;
	device->peer_max_bio_size = DRBD_MAX_BIO_SIZE_SAFE;
	device->local_max_bio_size = DRBD_MAX_BIO_SIZE_SAFE;
}

void drbd_set_my_capacity(struct drbd_device *device, sector_t size)
{
	char ppb[10];

	set_capacity_and_notify(device->vdisk, size);

	drbd_info(device, "size = %s (%llu KB)\n",
		ppsize(ppb, size>>1), (unsigned long long)size>>1);
}

void drbd_device_cleanup(struct drbd_device *device)
{
	int i;
	if (first_peer_device(device)->connection->receiver.t_state != NONE)
		drbd_err(device, "ASSERT FAILED: receiver t_state == %d expected 0.\n",
				first_peer_device(device)->connection->receiver.t_state);

	device->al_writ_cnt  =
	device->bm_writ_cnt  =
	device->read_cnt     =
	device->recv_cnt     =
	device->send_cnt     =
	device->writ_cnt     =
	device->p_size       =
	device->rs_start     =
	device->rs_total     =
	device->rs_failed    = 0;
	device->rs_last_events = 0;
	device->rs_last_sect_ev = 0;
	for (i = 0; i < DRBD_SYNC_MARKS; i++) {
		device->rs_mark_left[i] = 0;
		device->rs_mark_time[i] = 0;
	}
	D_ASSERT(device, first_peer_device(device)->connection->net_conf == NULL);

	set_capacity_and_notify(device->vdisk, 0);
	if (device->bitmap) {
		/* maybe never allocated. */
		drbd_bm_resize(device, 0, 1);
		drbd_bm_cleanup(device);
	}

	drbd_backing_dev_free(device, device->ldev);
	device->ldev = NULL;

	clear_bit(AL_SUSPENDED, &device->flags);

	D_ASSERT(device, list_empty(&device->active_ee));
	D_ASSERT(device, list_empty(&device->sync_ee));
	D_ASSERT(device, list_empty(&device->done_ee));
	D_ASSERT(device, list_empty(&device->read_ee));
	D_ASSERT(device, list_empty(&device->net_ee));
	D_ASSERT(device, list_empty(&device->resync_reads));
	D_ASSERT(device, list_empty(&first_peer_device(device)->connection->sender_work.q));
	D_ASSERT(device, list_empty(&device->resync_work.list));
	D_ASSERT(device, list_empty(&device->unplug_work.list));

	drbd_set_defaults(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static void drbd_destroy_mempools(void)
{
	struct page *page;

	while (drbd_pp_pool) {
		page = drbd_pp_pool;
		drbd_pp_pool = (struct page *)page_private(page);
		__free_page(page);
		drbd_pp_vacant--;
	}

<<<<<<< HEAD
	/* D_ASSERT(atomic_read(&drbd_pp_vacant)==0); */

	if (drbd_ee_mempool)
		mempool_destroy(drbd_ee_mempool);
	if (drbd_request_mempool)
		mempool_destroy(drbd_request_mempool);
	if (drbd_ee_cache)
		kmem_cache_destroy(drbd_ee_cache);
	if (drbd_request_cache)
		kmem_cache_destroy(drbd_request_cache);
	if (drbd_bm_ext_cache)
		kmem_cache_destroy(drbd_bm_ext_cache);
	if (drbd_al_ext_cache)
		kmem_cache_destroy(drbd_al_ext_cache);

	drbd_ee_mempool      = NULL;
	drbd_request_mempool = NULL;
=======
	/* D_ASSERT(device, atomic_read(&drbd_pp_vacant)==0); */

	bioset_exit(&drbd_io_bio_set);
	bioset_exit(&drbd_md_io_bio_set);
	mempool_exit(&drbd_md_io_page_pool);
	mempool_exit(&drbd_ee_mempool);
	mempool_exit(&drbd_request_mempool);
	kmem_cache_destroy(drbd_ee_cache);
	kmem_cache_destroy(drbd_request_cache);
	kmem_cache_destroy(drbd_bm_ext_cache);
	kmem_cache_destroy(drbd_al_ext_cache);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	drbd_ee_cache        = NULL;
	drbd_request_cache   = NULL;
	drbd_bm_ext_cache    = NULL;
	drbd_al_ext_cache    = NULL;

	return;
}

static int drbd_create_mempools(void)
{
	struct page *page;
<<<<<<< HEAD
	const int number = (DRBD_MAX_BIO_SIZE/PAGE_SIZE) * minor_count;
	int i;

	/* prepare our caches and mempools */
	drbd_request_mempool = NULL;
	drbd_ee_cache        = NULL;
	drbd_request_cache   = NULL;
	drbd_bm_ext_cache    = NULL;
	drbd_al_ext_cache    = NULL;
	drbd_pp_pool         = NULL;
=======
	const int number = (DRBD_MAX_BIO_SIZE/PAGE_SIZE) * drbd_minor_count;
	int i, ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* caches */
	drbd_request_cache = kmem_cache_create(
		"drbd_req", sizeof(struct drbd_request), 0, 0, NULL);
	if (drbd_request_cache == NULL)
		goto Enomem;

	drbd_ee_cache = kmem_cache_create(
<<<<<<< HEAD
		"drbd_ee", sizeof(struct drbd_epoch_entry), 0, 0, NULL);
=======
		"drbd_ee", sizeof(struct drbd_peer_request), 0, 0, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (drbd_ee_cache == NULL)
		goto Enomem;

	drbd_bm_ext_cache = kmem_cache_create(
		"drbd_bm", sizeof(struct bm_extent), 0, 0, NULL);
	if (drbd_bm_ext_cache == NULL)
		goto Enomem;

	drbd_al_ext_cache = kmem_cache_create(
		"drbd_al", sizeof(struct lc_element), 0, 0, NULL);
	if (drbd_al_ext_cache == NULL)
		goto Enomem;

	/* mempools */
<<<<<<< HEAD
	drbd_request_mempool = mempool_create(number,
		mempool_alloc_slab, mempool_free_slab, drbd_request_cache);
	if (drbd_request_mempool == NULL)
		goto Enomem;

	drbd_ee_mempool = mempool_create(number,
		mempool_alloc_slab, mempool_free_slab, drbd_ee_cache);
	if (drbd_ee_mempool == NULL)
		goto Enomem;

	/* drbd's page pool */
	spin_lock_init(&drbd_pp_lock);
=======
	ret = bioset_init(&drbd_io_bio_set, BIO_POOL_SIZE, 0, 0);
	if (ret)
		goto Enomem;

	ret = bioset_init(&drbd_md_io_bio_set, DRBD_MIN_POOL_PAGES, 0,
			  BIOSET_NEED_BVECS);
	if (ret)
		goto Enomem;

	ret = mempool_init_page_pool(&drbd_md_io_page_pool, DRBD_MIN_POOL_PAGES, 0);
	if (ret)
		goto Enomem;

	ret = mempool_init_slab_pool(&drbd_request_mempool, number,
				     drbd_request_cache);
	if (ret)
		goto Enomem;

	ret = mempool_init_slab_pool(&drbd_ee_mempool, number, drbd_ee_cache);
	if (ret)
		goto Enomem;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < number; i++) {
		page = alloc_page(GFP_HIGHUSER);
		if (!page)
			goto Enomem;
		set_page_private(page, (unsigned long)drbd_pp_pool);
		drbd_pp_pool = page;
	}
	drbd_pp_vacant = number;

	return 0;

Enomem:
	drbd_destroy_mempools(); /* in case we allocated some */
	return -ENOMEM;
}

<<<<<<< HEAD
static int drbd_notify_sys(struct notifier_block *this, unsigned long code,
	void *unused)
{
	/* just so we have it.  you never know what interesting things we
	 * might want to do here some day...
	 */

	return NOTIFY_DONE;
}

static struct notifier_block drbd_notifier = {
	.notifier_call = drbd_notify_sys,
};

static void drbd_release_ee_lists(struct drbd_conf *mdev)
{
	int rr;

	rr = drbd_release_ee(mdev, &mdev->active_ee);
	if (rr)
		dev_err(DEV, "%d EEs in active list found!\n", rr);

	rr = drbd_release_ee(mdev, &mdev->sync_ee);
	if (rr)
		dev_err(DEV, "%d EEs in sync list found!\n", rr);

	rr = drbd_release_ee(mdev, &mdev->read_ee);
	if (rr)
		dev_err(DEV, "%d EEs in read list found!\n", rr);

	rr = drbd_release_ee(mdev, &mdev->done_ee);
	if (rr)
		dev_err(DEV, "%d EEs in done list found!\n", rr);

	rr = drbd_release_ee(mdev, &mdev->net_ee);
	if (rr)
		dev_err(DEV, "%d EEs in net list found!\n", rr);
}

/* caution. no locking.
 * currently only used from module cleanup code. */
static void drbd_delete_device(unsigned int minor)
{
	struct drbd_conf *mdev = minor_to_mdev(minor);

	if (!mdev)
		return;

	/* paranoia asserts */
	if (mdev->open_cnt != 0)
		dev_err(DEV, "open_cnt = %d in %s:%u", mdev->open_cnt,
				__FILE__ , __LINE__);

	ERR_IF (!list_empty(&mdev->data.work.q)) {
		struct list_head *lp;
		list_for_each(lp, &mdev->data.work.q) {
			dev_err(DEV, "lp = %p\n", lp);
		}
	};
	/* end paranoia asserts */

	del_gendisk(mdev->vdisk);

	/* cleanup stuff that may have been allocated during
	 * device (re-)configuration or state changes */

	if (mdev->this_bdev)
		bdput(mdev->this_bdev);

	drbd_free_resources(mdev);

	drbd_release_ee_lists(mdev);

	/* should be freed on disconnect? */
	kfree(mdev->ee_hash);
	/*
	mdev->ee_hash_s = 0;
	mdev->ee_hash = NULL;
	*/

	lc_destroy(mdev->act_log);
	lc_destroy(mdev->resync);

	kfree(mdev->p_uuid);
	/* mdev->p_uuid = NULL; */

	kfree(mdev->int_dig_out);
	kfree(mdev->int_dig_in);
	kfree(mdev->int_dig_vv);

	/* cleanup the rest that has been
	 * allocated from drbd_new_device
	 * and actually free the mdev itself */
	drbd_free_mdev(mdev);
=======
static void drbd_release_all_peer_reqs(struct drbd_device *device)
{
	int rr;

	rr = drbd_free_peer_reqs(device, &device->active_ee);
	if (rr)
		drbd_err(device, "%d EEs in active list found!\n", rr);

	rr = drbd_free_peer_reqs(device, &device->sync_ee);
	if (rr)
		drbd_err(device, "%d EEs in sync list found!\n", rr);

	rr = drbd_free_peer_reqs(device, &device->read_ee);
	if (rr)
		drbd_err(device, "%d EEs in read list found!\n", rr);

	rr = drbd_free_peer_reqs(device, &device->done_ee);
	if (rr)
		drbd_err(device, "%d EEs in done list found!\n", rr);

	rr = drbd_free_peer_reqs(device, &device->net_ee);
	if (rr)
		drbd_err(device, "%d EEs in net list found!\n", rr);
}

/* caution. no locking. */
void drbd_destroy_device(struct kref *kref)
{
	struct drbd_device *device = container_of(kref, struct drbd_device, kref);
	struct drbd_resource *resource = device->resource;
	struct drbd_peer_device *peer_device, *tmp_peer_device;

	timer_shutdown_sync(&device->request_timer);

	/* paranoia asserts */
	D_ASSERT(device, device->open_cnt == 0);
	/* end paranoia asserts */

	/* cleanup stuff that may have been allocated during
	 * device (re-)configuration or state changes */

	drbd_backing_dev_free(device, device->ldev);
	device->ldev = NULL;

	drbd_release_all_peer_reqs(device);

	lc_destroy(device->act_log);
	lc_destroy(device->resync);

	kfree(device->p_uuid);
	/* device->p_uuid = NULL; */

	if (device->bitmap) /* should no longer be there. */
		drbd_bm_cleanup(device);
	__free_page(device->md_io.page);
	put_disk(device->vdisk);
	kfree(device->rs_plan_s);

	/* not for_each_connection(connection, resource):
	 * those may have been cleaned up and disassociated already.
	 */
	for_each_peer_device_safe(peer_device, tmp_peer_device, device) {
		kref_put(&peer_device->connection->kref, drbd_destroy_connection);
		kfree(peer_device);
	}
	if (device->submit.wq)
		destroy_workqueue(device->submit.wq);
	kfree(device);
	kref_put(&resource->kref, drbd_destroy_resource);
}

/* One global retry thread, if we need to push back some bio and have it
 * reinserted through our make request function.
 */
static struct retry_worker {
	struct workqueue_struct *wq;
	struct work_struct worker;

	spinlock_t lock;
	struct list_head writes;
} retry;

static void do_retry(struct work_struct *ws)
{
	struct retry_worker *retry = container_of(ws, struct retry_worker, worker);
	LIST_HEAD(writes);
	struct drbd_request *req, *tmp;

	spin_lock_irq(&retry->lock);
	list_splice_init(&retry->writes, &writes);
	spin_unlock_irq(&retry->lock);

	list_for_each_entry_safe(req, tmp, &writes, tl_requests) {
		struct drbd_device *device = req->device;
		struct bio *bio = req->master_bio;
		bool expected;

		expected =
			expect(device, atomic_read(&req->completion_ref) == 0) &&
			expect(device, req->rq_state & RQ_POSTPONED) &&
			expect(device, (req->rq_state & RQ_LOCAL_PENDING) == 0 ||
				(req->rq_state & RQ_LOCAL_ABORTED) != 0);

		if (!expected)
			drbd_err(device, "req=%p completion_ref=%d rq_state=%x\n",
				req, atomic_read(&req->completion_ref),
				req->rq_state);

		/* We still need to put one kref associated with the
		 * "completion_ref" going zero in the code path that queued it
		 * here.  The request object may still be referenced by a
		 * frozen local req->private_bio, in case we force-detached.
		 */
		kref_put(&req->kref, drbd_req_destroy);

		/* A single suspended or otherwise blocking device may stall
		 * all others as well.  Fortunately, this code path is to
		 * recover from a situation that "should not happen":
		 * concurrent writes in multi-primary setup.
		 * In a "normal" lifecycle, this workqueue is supposed to be
		 * destroyed without ever doing anything.
		 * If it turns out to be an issue anyways, we can do per
		 * resource (replication group) or per device (minor) retry
		 * workqueues instead.
		 */

		/* We are not just doing submit_bio_noacct(),
		 * as we want to keep the start_time information. */
		inc_ap_bio(device);
		__drbd_make_request(device, bio);
	}
}

/* called via drbd_req_put_completion_ref(),
 * holds resource->req_lock */
void drbd_restart_request(struct drbd_request *req)
{
	unsigned long flags;
	spin_lock_irqsave(&retry.lock, flags);
	list_move_tail(&req->tl_requests, &retry.writes);
	spin_unlock_irqrestore(&retry.lock, flags);

	/* Drop the extra reference that would otherwise
	 * have been dropped by complete_master_bio.
	 * do_retry() needs to grab a new one. */
	dec_ap_bio(req->device);

	queue_work(retry.wq, &retry.worker);
}

void drbd_destroy_resource(struct kref *kref)
{
	struct drbd_resource *resource =
		container_of(kref, struct drbd_resource, kref);

	idr_destroy(&resource->devices);
	free_cpumask_var(resource->cpu_mask);
	kfree(resource->name);
	kfree(resource);
}

void drbd_free_resource(struct drbd_resource *resource)
{
	struct drbd_connection *connection, *tmp;

	for_each_connection_safe(connection, tmp, resource) {
		list_del(&connection->connections);
		drbd_debugfs_connection_cleanup(connection);
		kref_put(&connection->kref, drbd_destroy_connection);
	}
	drbd_debugfs_resource_cleanup(resource);
	kref_put(&resource->kref, drbd_destroy_resource);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void drbd_cleanup(void)
{
	unsigned int i;
<<<<<<< HEAD

	unregister_reboot_notifier(&drbd_notifier);
=======
	struct drbd_device *device;
	struct drbd_resource *resource, *tmp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* first remove proc,
	 * drbdsetup uses it's presence to detect
	 * whether DRBD is loaded.
	 * If we would get stuck in proc removal,
	 * but have netlink already deregistered,
	 * some drbdsetup commands may wait forever
	 * for an answer.
	 */
	if (drbd_proc)
		remove_proc_entry("drbd", NULL);

<<<<<<< HEAD
	drbd_nl_cleanup();

	if (minor_table) {
		i = minor_count;
		while (i--)
			drbd_delete_device(i);
		drbd_destroy_mempools();
	}

	kfree(minor_table);

	unregister_blkdev(DRBD_MAJOR, "drbd");

	printk(KERN_INFO "drbd: module cleanup done.\n");
}

/**
 * drbd_congested() - Callback for pdflush
 * @congested_data:	User data
 * @bdi_bits:		Bits pdflush is currently interested in
 *
 * Returns 1<<BDI_async_congested and/or 1<<BDI_sync_congested if we are congested.
 */
static int drbd_congested(void *congested_data, int bdi_bits)
{
	struct drbd_conf *mdev = congested_data;
	struct request_queue *q;
	char reason = '-';
	int r = 0;

	if (!may_inc_ap_bio(mdev)) {
		/* DRBD has frozen IO */
		r = bdi_bits;
		reason = 'd';
		goto out;
	}

	if (get_ldev(mdev)) {
		q = bdev_get_queue(mdev->ldev->backing_bdev);
		r = bdi_congested(&q->backing_dev_info, bdi_bits);
		put_ldev(mdev);
		if (r)
			reason = 'b';
	}

	if (bdi_bits & (1 << BDI_async_congested) && test_bit(NET_CONGESTED, &mdev->flags)) {
		r |= (1 << BDI_async_congested);
		reason = reason == 'b' ? 'a' : 'n';
	}

out:
	mdev->congestion_reason = reason;
	return r;
}

struct drbd_conf *drbd_new_device(unsigned int minor)
{
	struct drbd_conf *mdev;
	struct gendisk *disk;
	struct request_queue *q;

	/* GFP_KERNEL, we are outside of all write-out paths */
	mdev = kzalloc(sizeof(struct drbd_conf), GFP_KERNEL);
	if (!mdev)
		return NULL;
	if (!zalloc_cpumask_var(&mdev->cpu_mask, GFP_KERNEL))
		goto out_no_cpumask;

	mdev->minor = minor;

	drbd_init_set_defaults(mdev);

	q = blk_alloc_queue(GFP_KERNEL);
	if (!q)
		goto out_no_q;
	mdev->rq_queue = q;
	q->queuedata   = mdev;

	disk = alloc_disk(1);
	if (!disk)
		goto out_no_disk;
	mdev->vdisk = disk;

	set_disk_ro(disk, true);

	disk->queue = q;
	disk->major = DRBD_MAJOR;
	disk->first_minor = minor;
	disk->fops = &drbd_ops;
	sprintf(disk->disk_name, "drbd%d", minor);
	disk->private_data = mdev;

	mdev->this_bdev = bdget(MKDEV(DRBD_MAJOR, minor));
	/* we have no partitions. we contain only ourselves. */
	mdev->this_bdev->bd_contains = mdev->this_bdev;

	q->backing_dev_info.congested_fn = drbd_congested;
	q->backing_dev_info.congested_data = mdev;

	blk_queue_make_request(q, drbd_make_request);
	/* Setting the max_hw_sectors to an odd value of 8kibyte here
	   This triggers a max_bio_size message upon first attach or connect */
	blk_queue_max_hw_sectors(q, DRBD_MAX_BIO_SIZE_SAFE >> 8);
	blk_queue_bounce_limit(q, BLK_BOUNCE_ANY);
	blk_queue_merge_bvec(q, drbd_merge_bvec);
	q->queue_lock = &mdev->req_lock;

	mdev->md_io_page = alloc_page(GFP_KERNEL);
	if (!mdev->md_io_page)
		goto out_no_io_page;

	if (drbd_bm_init(mdev))
		goto out_no_bitmap;
	/* no need to lock access, we are still initializing this minor device. */
	if (!tl_init(mdev))
		goto out_no_tl;

	mdev->app_reads_hash = kzalloc(APP_R_HSIZE*sizeof(void *), GFP_KERNEL);
	if (!mdev->app_reads_hash)
		goto out_no_app_reads;

	mdev->current_epoch = kzalloc(sizeof(struct drbd_epoch), GFP_KERNEL);
	if (!mdev->current_epoch)
		goto out_no_epoch;

	INIT_LIST_HEAD(&mdev->current_epoch->list);
	mdev->epochs = 1;

	return mdev;

/* out_whatever_else:
	kfree(mdev->current_epoch); */
out_no_epoch:
	kfree(mdev->app_reads_hash);
out_no_app_reads:
	tl_cleanup(mdev);
out_no_tl:
	drbd_bm_cleanup(mdev);
out_no_bitmap:
	__free_page(mdev->md_io_page);
out_no_io_page:
	put_disk(disk);
out_no_disk:
	blk_cleanup_queue(q);
out_no_q:
	free_cpumask_var(mdev->cpu_mask);
out_no_cpumask:
	kfree(mdev);
	return NULL;
}

/* counterpart of drbd_new_device.
 * last part of drbd_delete_device. */
void drbd_free_mdev(struct drbd_conf *mdev)
{
	kfree(mdev->current_epoch);
	kfree(mdev->app_reads_hash);
	tl_cleanup(mdev);
	if (mdev->bitmap) /* should no longer be there. */
		drbd_bm_cleanup(mdev);
	__free_page(mdev->md_io_page);
	put_disk(mdev->vdisk);
	blk_cleanup_queue(mdev->rq_queue);
	free_cpumask_var(mdev->cpu_mask);
	drbd_free_tl_hash(mdev);
	kfree(mdev);
}


int __init drbd_init(void)
{
	int err;

	if (sizeof(struct p_handshake) != 80) {
		printk(KERN_ERR
		       "drbd: never change the size or layout "
		       "of the HandShake packet.\n");
		return -EINVAL;
	}

	if (minor_count < DRBD_MINOR_COUNT_MIN || minor_count > DRBD_MINOR_COUNT_MAX) {
		printk(KERN_ERR
			"drbd: invalid minor_count (%d)\n", minor_count);
#ifdef MODULE
		return -EINVAL;
#else
		minor_count = 8;
#endif
	}

	err = drbd_nl_init();
	if (err)
		return err;

	err = register_blkdev(DRBD_MAJOR, "drbd");
	if (err) {
		printk(KERN_ERR
		       "drbd: unable to register block device major %d\n",
=======
	if (retry.wq)
		destroy_workqueue(retry.wq);

	drbd_genl_unregister();

	idr_for_each_entry(&drbd_devices, device, i)
		drbd_delete_device(device);

	/* not _rcu since, no other updater anymore. Genl already unregistered */
	for_each_resource_safe(resource, tmp, &drbd_resources) {
		list_del(&resource->resources);
		drbd_free_resource(resource);
	}

	drbd_debugfs_cleanup();

	drbd_destroy_mempools();
	unregister_blkdev(DRBD_MAJOR, "drbd");

	idr_destroy(&drbd_devices);

	pr_info("module cleanup done.\n");
}

static void drbd_init_workqueue(struct drbd_work_queue* wq)
{
	spin_lock_init(&wq->q_lock);
	INIT_LIST_HEAD(&wq->q);
	init_waitqueue_head(&wq->q_wait);
}

struct completion_work {
	struct drbd_work w;
	struct completion done;
};

static int w_complete(struct drbd_work *w, int cancel)
{
	struct completion_work *completion_work =
		container_of(w, struct completion_work, w);

	complete(&completion_work->done);
	return 0;
}

void drbd_flush_workqueue(struct drbd_work_queue *work_queue)
{
	struct completion_work completion_work;

	completion_work.w.cb = w_complete;
	init_completion(&completion_work.done);
	drbd_queue_work(work_queue, &completion_work.w);
	wait_for_completion(&completion_work.done);
}

struct drbd_resource *drbd_find_resource(const char *name)
{
	struct drbd_resource *resource;

	if (!name || !name[0])
		return NULL;

	rcu_read_lock();
	for_each_resource_rcu(resource, &drbd_resources) {
		if (!strcmp(resource->name, name)) {
			kref_get(&resource->kref);
			goto found;
		}
	}
	resource = NULL;
found:
	rcu_read_unlock();
	return resource;
}

struct drbd_connection *conn_get_by_addrs(void *my_addr, int my_addr_len,
				     void *peer_addr, int peer_addr_len)
{
	struct drbd_resource *resource;
	struct drbd_connection *connection;

	rcu_read_lock();
	for_each_resource_rcu(resource, &drbd_resources) {
		for_each_connection_rcu(connection, resource) {
			if (connection->my_addr_len == my_addr_len &&
			    connection->peer_addr_len == peer_addr_len &&
			    !memcmp(&connection->my_addr, my_addr, my_addr_len) &&
			    !memcmp(&connection->peer_addr, peer_addr, peer_addr_len)) {
				kref_get(&connection->kref);
				goto found;
			}
		}
	}
	connection = NULL;
found:
	rcu_read_unlock();
	return connection;
}

static int drbd_alloc_socket(struct drbd_socket *socket)
{
	socket->rbuf = (void *) __get_free_page(GFP_KERNEL);
	if (!socket->rbuf)
		return -ENOMEM;
	socket->sbuf = (void *) __get_free_page(GFP_KERNEL);
	if (!socket->sbuf)
		return -ENOMEM;
	return 0;
}

static void drbd_free_socket(struct drbd_socket *socket)
{
	free_page((unsigned long) socket->sbuf);
	free_page((unsigned long) socket->rbuf);
}

void conn_free_crypto(struct drbd_connection *connection)
{
	drbd_free_sock(connection);

	crypto_free_shash(connection->csums_tfm);
	crypto_free_shash(connection->verify_tfm);
	crypto_free_shash(connection->cram_hmac_tfm);
	crypto_free_shash(connection->integrity_tfm);
	crypto_free_shash(connection->peer_integrity_tfm);
	kfree(connection->int_dig_in);
	kfree(connection->int_dig_vv);

	connection->csums_tfm = NULL;
	connection->verify_tfm = NULL;
	connection->cram_hmac_tfm = NULL;
	connection->integrity_tfm = NULL;
	connection->peer_integrity_tfm = NULL;
	connection->int_dig_in = NULL;
	connection->int_dig_vv = NULL;
}

int set_resource_options(struct drbd_resource *resource, struct res_opts *res_opts)
{
	struct drbd_connection *connection;
	cpumask_var_t new_cpu_mask;
	int err;

	if (!zalloc_cpumask_var(&new_cpu_mask, GFP_KERNEL))
		return -ENOMEM;

	/* silently ignore cpu mask on UP kernel */
	if (nr_cpu_ids > 1 && res_opts->cpu_mask[0] != 0) {
		err = bitmap_parse(res_opts->cpu_mask, DRBD_CPU_MASK_SIZE,
				   cpumask_bits(new_cpu_mask), nr_cpu_ids);
		if (err == -EOVERFLOW) {
			/* So what. mask it out. */
			cpumask_var_t tmp_cpu_mask;
			if (zalloc_cpumask_var(&tmp_cpu_mask, GFP_KERNEL)) {
				cpumask_setall(tmp_cpu_mask);
				cpumask_and(new_cpu_mask, new_cpu_mask, tmp_cpu_mask);
				drbd_warn(resource, "Overflow in bitmap_parse(%.12s%s), truncating to %u bits\n",
					res_opts->cpu_mask,
					strlen(res_opts->cpu_mask) > 12 ? "..." : "",
					nr_cpu_ids);
				free_cpumask_var(tmp_cpu_mask);
				err = 0;
			}
		}
		if (err) {
			drbd_warn(resource, "bitmap_parse() failed with %d\n", err);
			/* retcode = ERR_CPU_MASK_PARSE; */
			goto fail;
		}
	}
	resource->res_opts = *res_opts;
	if (cpumask_empty(new_cpu_mask))
		drbd_calc_cpu_mask(&new_cpu_mask);
	if (!cpumask_equal(resource->cpu_mask, new_cpu_mask)) {
		cpumask_copy(resource->cpu_mask, new_cpu_mask);
		for_each_connection_rcu(connection, resource) {
			connection->receiver.reset_cpu_mask = 1;
			connection->ack_receiver.reset_cpu_mask = 1;
			connection->worker.reset_cpu_mask = 1;
		}
	}
	err = 0;

fail:
	free_cpumask_var(new_cpu_mask);
	return err;

}

struct drbd_resource *drbd_create_resource(const char *name)
{
	struct drbd_resource *resource;

	resource = kzalloc(sizeof(struct drbd_resource), GFP_KERNEL);
	if (!resource)
		goto fail;
	resource->name = kstrdup(name, GFP_KERNEL);
	if (!resource->name)
		goto fail_free_resource;
	if (!zalloc_cpumask_var(&resource->cpu_mask, GFP_KERNEL))
		goto fail_free_name;
	kref_init(&resource->kref);
	idr_init(&resource->devices);
	INIT_LIST_HEAD(&resource->connections);
	resource->write_ordering = WO_BDEV_FLUSH;
	list_add_tail_rcu(&resource->resources, &drbd_resources);
	mutex_init(&resource->conf_update);
	mutex_init(&resource->adm_mutex);
	spin_lock_init(&resource->req_lock);
	drbd_debugfs_resource_add(resource);
	return resource;

fail_free_name:
	kfree(resource->name);
fail_free_resource:
	kfree(resource);
fail:
	return NULL;
}

/* caller must be under adm_mutex */
struct drbd_connection *conn_create(const char *name, struct res_opts *res_opts)
{
	struct drbd_resource *resource;
	struct drbd_connection *connection;

	connection = kzalloc(sizeof(struct drbd_connection), GFP_KERNEL);
	if (!connection)
		return NULL;

	if (drbd_alloc_socket(&connection->data))
		goto fail;
	if (drbd_alloc_socket(&connection->meta))
		goto fail;

	connection->current_epoch = kzalloc(sizeof(struct drbd_epoch), GFP_KERNEL);
	if (!connection->current_epoch)
		goto fail;

	INIT_LIST_HEAD(&connection->transfer_log);

	INIT_LIST_HEAD(&connection->current_epoch->list);
	connection->epochs = 1;
	spin_lock_init(&connection->epoch_lock);

	connection->send.seen_any_write_yet = false;
	connection->send.current_epoch_nr = 0;
	connection->send.current_epoch_writes = 0;

	resource = drbd_create_resource(name);
	if (!resource)
		goto fail;

	connection->cstate = C_STANDALONE;
	mutex_init(&connection->cstate_mutex);
	init_waitqueue_head(&connection->ping_wait);
	idr_init(&connection->peer_devices);

	drbd_init_workqueue(&connection->sender_work);
	mutex_init(&connection->data.mutex);
	mutex_init(&connection->meta.mutex);

	drbd_thread_init(resource, &connection->receiver, drbd_receiver, "receiver");
	connection->receiver.connection = connection;
	drbd_thread_init(resource, &connection->worker, drbd_worker, "worker");
	connection->worker.connection = connection;
	drbd_thread_init(resource, &connection->ack_receiver, drbd_ack_receiver, "ack_recv");
	connection->ack_receiver.connection = connection;

	kref_init(&connection->kref);

	connection->resource = resource;

	if (set_resource_options(resource, res_opts))
		goto fail_resource;

	kref_get(&resource->kref);
	list_add_tail_rcu(&connection->connections, &resource->connections);
	drbd_debugfs_connection_add(connection);
	return connection;

fail_resource:
	list_del(&resource->resources);
	drbd_free_resource(resource);
fail:
	kfree(connection->current_epoch);
	drbd_free_socket(&connection->meta);
	drbd_free_socket(&connection->data);
	kfree(connection);
	return NULL;
}

void drbd_destroy_connection(struct kref *kref)
{
	struct drbd_connection *connection = container_of(kref, struct drbd_connection, kref);
	struct drbd_resource *resource = connection->resource;

	if (atomic_read(&connection->current_epoch->epoch_size) !=  0)
		drbd_err(connection, "epoch_size:%d\n", atomic_read(&connection->current_epoch->epoch_size));
	kfree(connection->current_epoch);

	idr_destroy(&connection->peer_devices);

	drbd_free_socket(&connection->meta);
	drbd_free_socket(&connection->data);
	kfree(connection->int_dig_in);
	kfree(connection->int_dig_vv);
	kfree(connection);
	kref_put(&resource->kref, drbd_destroy_resource);
}

static int init_submitter(struct drbd_device *device)
{
	/* opencoded create_singlethread_workqueue(),
	 * to be able to say "drbd%d", ..., minor */
	device->submit.wq =
		alloc_ordered_workqueue("drbd%u_submit", WQ_MEM_RECLAIM, device->minor);
	if (!device->submit.wq)
		return -ENOMEM;

	INIT_WORK(&device->submit.worker, do_submit);
	INIT_LIST_HEAD(&device->submit.writes);
	return 0;
}

enum drbd_ret_code drbd_create_device(struct drbd_config_context *adm_ctx, unsigned int minor)
{
	struct drbd_resource *resource = adm_ctx->resource;
	struct drbd_connection *connection, *n;
	struct drbd_device *device;
	struct drbd_peer_device *peer_device, *tmp_peer_device;
	struct gendisk *disk;
	int id;
	int vnr = adm_ctx->volume;
	enum drbd_ret_code err = ERR_NOMEM;
	struct queue_limits lim = {
		/*
		 * Setting the max_hw_sectors to an odd value of 8kibyte here.
		 * This triggers a max_bio_size message upon first attach or
		 * connect.
		 */
		.max_hw_sectors		= DRBD_MAX_BIO_SIZE_SAFE >> 8,
	};

	device = minor_to_device(minor);
	if (device)
		return ERR_MINOR_OR_VOLUME_EXISTS;

	/* GFP_KERNEL, we are outside of all write-out paths */
	device = kzalloc(sizeof(struct drbd_device), GFP_KERNEL);
	if (!device)
		return ERR_NOMEM;
	kref_init(&device->kref);

	kref_get(&resource->kref);
	device->resource = resource;
	device->minor = minor;
	device->vnr = vnr;

	drbd_init_set_defaults(device);

	disk = blk_alloc_disk(&lim, NUMA_NO_NODE);
	if (IS_ERR(disk)) {
		err = PTR_ERR(disk);
		goto out_no_disk;
	}

	device->vdisk = disk;
	device->rq_queue = disk->queue;

	set_disk_ro(disk, true);

	disk->major = DRBD_MAJOR;
	disk->first_minor = minor;
	disk->minors = 1;
	disk->fops = &drbd_ops;
	disk->flags |= GENHD_FL_NO_PART;
	sprintf(disk->disk_name, "drbd%d", minor);
	disk->private_data = device;

	blk_queue_flag_set(QUEUE_FLAG_STABLE_WRITES, disk->queue);
	blk_queue_write_cache(disk->queue, true, true);

	device->md_io.page = alloc_page(GFP_KERNEL);
	if (!device->md_io.page)
		goto out_no_io_page;

	if (drbd_bm_init(device))
		goto out_no_bitmap;
	device->read_requests = RB_ROOT;
	device->write_requests = RB_ROOT;

	id = idr_alloc(&drbd_devices, device, minor, minor + 1, GFP_KERNEL);
	if (id < 0) {
		if (id == -ENOSPC)
			err = ERR_MINOR_OR_VOLUME_EXISTS;
		goto out_no_minor_idr;
	}
	kref_get(&device->kref);

	id = idr_alloc(&resource->devices, device, vnr, vnr + 1, GFP_KERNEL);
	if (id < 0) {
		if (id == -ENOSPC)
			err = ERR_MINOR_OR_VOLUME_EXISTS;
		goto out_idr_remove_minor;
	}
	kref_get(&device->kref);

	INIT_LIST_HEAD(&device->peer_devices);
	INIT_LIST_HEAD(&device->pending_bitmap_io);
	for_each_connection(connection, resource) {
		peer_device = kzalloc(sizeof(struct drbd_peer_device), GFP_KERNEL);
		if (!peer_device)
			goto out_idr_remove_from_resource;
		peer_device->connection = connection;
		peer_device->device = device;

		list_add(&peer_device->peer_devices, &device->peer_devices);
		kref_get(&device->kref);

		id = idr_alloc(&connection->peer_devices, peer_device, vnr, vnr + 1, GFP_KERNEL);
		if (id < 0) {
			if (id == -ENOSPC)
				err = ERR_INVALID_REQUEST;
			goto out_idr_remove_from_resource;
		}
		kref_get(&connection->kref);
		INIT_WORK(&peer_device->send_acks_work, drbd_send_acks_wf);
	}

	if (init_submitter(device)) {
		err = ERR_NOMEM;
		goto out_idr_remove_from_resource;
	}

	err = add_disk(disk);
	if (err)
		goto out_destroy_workqueue;

	/* inherit the connection state */
	device->state.conn = first_connection(resource)->cstate;
	if (device->state.conn == C_WF_REPORT_PARAMS) {
		for_each_peer_device(peer_device, device)
			drbd_connected(peer_device);
	}
	/* move to create_peer_device() */
	for_each_peer_device(peer_device, device)
		drbd_debugfs_peer_device_add(peer_device);
	drbd_debugfs_device_add(device);
	return NO_ERROR;

out_destroy_workqueue:
	destroy_workqueue(device->submit.wq);
out_idr_remove_from_resource:
	for_each_connection_safe(connection, n, resource) {
		peer_device = idr_remove(&connection->peer_devices, vnr);
		if (peer_device)
			kref_put(&connection->kref, drbd_destroy_connection);
	}
	for_each_peer_device_safe(peer_device, tmp_peer_device, device) {
		list_del(&peer_device->peer_devices);
		kfree(peer_device);
	}
	idr_remove(&resource->devices, vnr);
out_idr_remove_minor:
	idr_remove(&drbd_devices, minor);
	synchronize_rcu();
out_no_minor_idr:
	drbd_bm_cleanup(device);
out_no_bitmap:
	__free_page(device->md_io.page);
out_no_io_page:
	put_disk(disk);
out_no_disk:
	kref_put(&resource->kref, drbd_destroy_resource);
	kfree(device);
	return err;
}

void drbd_delete_device(struct drbd_device *device)
{
	struct drbd_resource *resource = device->resource;
	struct drbd_connection *connection;
	struct drbd_peer_device *peer_device;

	/* move to free_peer_device() */
	for_each_peer_device(peer_device, device)
		drbd_debugfs_peer_device_cleanup(peer_device);
	drbd_debugfs_device_cleanup(device);
	for_each_connection(connection, resource) {
		idr_remove(&connection->peer_devices, device->vnr);
		kref_put(&device->kref, drbd_destroy_device);
	}
	idr_remove(&resource->devices, device->vnr);
	kref_put(&device->kref, drbd_destroy_device);
	idr_remove(&drbd_devices, device_to_minor(device));
	kref_put(&device->kref, drbd_destroy_device);
	del_gendisk(device->vdisk);
	synchronize_rcu();
	kref_put(&device->kref, drbd_destroy_device);
}

static int __init drbd_init(void)
{
	int err;

	if (drbd_minor_count < DRBD_MINOR_COUNT_MIN || drbd_minor_count > DRBD_MINOR_COUNT_MAX) {
		pr_err("invalid minor_count (%d)\n", drbd_minor_count);
#ifdef MODULE
		return -EINVAL;
#else
		drbd_minor_count = DRBD_MINOR_COUNT_DEF;
#endif
	}

	err = register_blkdev(DRBD_MAJOR, "drbd");
	if (err) {
		pr_err("unable to register block device major %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       DRBD_MAJOR);
		return err;
	}

<<<<<<< HEAD
	register_reboot_notifier(&drbd_notifier);

	/*
	 * allocate all necessary structs
	 */
	err = -ENOMEM;

	init_waitqueue_head(&drbd_pp_wait);

	drbd_proc = NULL; /* play safe for drbd_cleanup */
	minor_table = kzalloc(sizeof(struct drbd_conf *)*minor_count,
				GFP_KERNEL);
	if (!minor_table)
		goto Enomem;

	err = drbd_create_mempools();
	if (err)
		goto Enomem;

	drbd_proc = proc_create_data("drbd", S_IFREG | S_IRUGO , NULL, &drbd_proc_fops, NULL);
	if (!drbd_proc)	{
		printk(KERN_ERR "drbd: unable to register proc file\n");
		goto Enomem;
	}

	rwlock_init(&global_state_lock);

	printk(KERN_INFO "drbd: initialized. "
	       "Version: " REL_VERSION " (api:%d/proto:%d-%d)\n",
	       API_VERSION, PRO_VERSION_MIN, PRO_VERSION_MAX);
	printk(KERN_INFO "drbd: %s\n", drbd_buildtag());
	printk(KERN_INFO "drbd: registered as block device major %d\n",
		DRBD_MAJOR);
	printk(KERN_INFO "drbd: minor_table @ 0x%p\n", minor_table);

	return 0; /* Success! */

Enomem:
	drbd_cleanup();
	if (err == -ENOMEM)
		/* currently always the case */
		printk(KERN_ERR "drbd: ran out of memory\n");
	else
		printk(KERN_ERR "drbd: initialization failure\n");
	return err;
}

void drbd_free_bc(struct drbd_backing_dev *ldev)
{
	if (ldev == NULL)
		return;

	blkdev_put(ldev->backing_bdev, FMODE_READ | FMODE_WRITE | FMODE_EXCL);
	blkdev_put(ldev->md_bdev, FMODE_READ | FMODE_WRITE | FMODE_EXCL);

	kfree(ldev);
}

void drbd_free_sock(struct drbd_conf *mdev)
{
	if (mdev->data.socket) {
		mutex_lock(&mdev->data.mutex);
		kernel_sock_shutdown(mdev->data.socket, SHUT_RDWR);
		sock_release(mdev->data.socket);
		mdev->data.socket = NULL;
		mutex_unlock(&mdev->data.mutex);
	}
	if (mdev->meta.socket) {
		mutex_lock(&mdev->meta.mutex);
		kernel_sock_shutdown(mdev->meta.socket, SHUT_RDWR);
		sock_release(mdev->meta.socket);
		mdev->meta.socket = NULL;
		mutex_unlock(&mdev->meta.mutex);
	}
}


void drbd_free_resources(struct drbd_conf *mdev)
{
	crypto_free_hash(mdev->csums_tfm);
	mdev->csums_tfm = NULL;
	crypto_free_hash(mdev->verify_tfm);
	mdev->verify_tfm = NULL;
	crypto_free_hash(mdev->cram_hmac_tfm);
	mdev->cram_hmac_tfm = NULL;
	crypto_free_hash(mdev->integrity_w_tfm);
	mdev->integrity_w_tfm = NULL;
	crypto_free_hash(mdev->integrity_r_tfm);
	mdev->integrity_r_tfm = NULL;

	drbd_free_sock(mdev);

	__no_warn(local,
		  drbd_free_bc(mdev->ldev);
		  mdev->ldev = NULL;);
=======
	/*
	 * allocate all necessary structs
	 */
	init_waitqueue_head(&drbd_pp_wait);

	drbd_proc = NULL; /* play safe for drbd_cleanup */
	idr_init(&drbd_devices);

	mutex_init(&resources_mutex);
	INIT_LIST_HEAD(&drbd_resources);

	err = drbd_genl_register();
	if (err) {
		pr_err("unable to register generic netlink family\n");
		goto fail;
	}

	err = drbd_create_mempools();
	if (err)
		goto fail;

	err = -ENOMEM;
	drbd_proc = proc_create_single("drbd", S_IFREG | 0444 , NULL, drbd_seq_show);
	if (!drbd_proc)	{
		pr_err("unable to register proc file\n");
		goto fail;
	}

	retry.wq = create_singlethread_workqueue("drbd-reissue");
	if (!retry.wq) {
		pr_err("unable to create retry workqueue\n");
		goto fail;
	}
	INIT_WORK(&retry.worker, do_retry);
	spin_lock_init(&retry.lock);
	INIT_LIST_HEAD(&retry.writes);

	drbd_debugfs_init();

	pr_info("initialized. "
	       "Version: " REL_VERSION " (api:%d/proto:%d-%d)\n",
	       GENL_MAGIC_VERSION, PRO_VERSION_MIN, PRO_VERSION_MAX);
	pr_info("%s\n", drbd_buildtag());
	pr_info("registered as block device major %d\n", DRBD_MAJOR);
	return 0; /* Success! */

fail:
	drbd_cleanup();
	if (err == -ENOMEM)
		pr_err("ran out of memory\n");
	else
		pr_err("initialization failure\n");
	return err;
}

static void drbd_free_one_sock(struct drbd_socket *ds)
{
	struct socket *s;
	mutex_lock(&ds->mutex);
	s = ds->socket;
	ds->socket = NULL;
	mutex_unlock(&ds->mutex);
	if (s) {
		/* so debugfs does not need to mutex_lock() */
		synchronize_rcu();
		kernel_sock_shutdown(s, SHUT_RDWR);
		sock_release(s);
	}
}

void drbd_free_sock(struct drbd_connection *connection)
{
	if (connection->data.socket)
		drbd_free_one_sock(&connection->data);
	if (connection->meta.socket)
		drbd_free_one_sock(&connection->meta);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* meta data management */

<<<<<<< HEAD
struct meta_data_on_disk {
	u64 la_size;           /* last agreed size. */
=======
void conn_md_sync(struct drbd_connection *connection)
{
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;

		kref_get(&device->kref);
		rcu_read_unlock();
		drbd_md_sync(device);
		kref_put(&device->kref, drbd_destroy_device);
		rcu_read_lock();
	}
	rcu_read_unlock();
}

/* aligned 4kByte */
struct meta_data_on_disk {
	u64 la_size_sect;      /* last agreed size. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u64 uuid[UI_SIZE];   /* UUIDs. */
	u64 device_uuid;
	u64 reserved_u64_1;
	u32 flags;             /* MDF */
	u32 magic;
	u32 md_size_sect;
	u32 al_offset;         /* offset to this block */
<<<<<<< HEAD
	u32 al_nr_extents;     /* important for restoring the AL */
	      /* `-- act_log->nr_elements <-- sync_conf.al_extents */
	u32 bm_offset;         /* offset to the bitmap, from here */
	u32 bm_bytes_per_bit;  /* BM_BLOCK_SIZE */
	u32 la_peer_max_bio_size;   /* last peer max_bio_size */
	u32 reserved_u32[3];

} __packed;

/**
 * drbd_md_sync() - Writes the meta data super block if the MD_DIRTY flag bit is set
 * @mdev:	DRBD device.
 */
void drbd_md_sync(struct drbd_conf *mdev)
{
	struct meta_data_on_disk *buffer;
	sector_t sector;
	int i;

	del_timer(&mdev->md_sync_timer);
	/* timer may be rearmed by drbd_md_mark_dirty() now. */
	if (!test_and_clear_bit(MD_DIRTY, &mdev->flags))
=======
	u32 al_nr_extents;     /* important for restoring the AL (userspace) */
	      /* `-- act_log->nr_elements <-- ldev->dc.al_extents */
	u32 bm_offset;         /* offset to the bitmap, from here */
	u32 bm_bytes_per_bit;  /* BM_BLOCK_SIZE */
	u32 la_peer_max_bio_size;   /* last peer max_bio_size */

	/* see al_tr_number_to_on_disk_sector() */
	u32 al_stripes;
	u32 al_stripe_size_4k;

	u8 reserved_u8[4096 - (7*8 + 10*4)];
} __packed;



void drbd_md_write(struct drbd_device *device, void *b)
{
	struct meta_data_on_disk *buffer = b;
	sector_t sector;
	int i;

	memset(buffer, 0, sizeof(*buffer));

	buffer->la_size_sect = cpu_to_be64(get_capacity(device->vdisk));
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		buffer->uuid[i] = cpu_to_be64(device->ldev->md.uuid[i]);
	buffer->flags = cpu_to_be32(device->ldev->md.flags);
	buffer->magic = cpu_to_be32(DRBD_MD_MAGIC_84_UNCLEAN);

	buffer->md_size_sect  = cpu_to_be32(device->ldev->md.md_size_sect);
	buffer->al_offset     = cpu_to_be32(device->ldev->md.al_offset);
	buffer->al_nr_extents = cpu_to_be32(device->act_log->nr_elements);
	buffer->bm_bytes_per_bit = cpu_to_be32(BM_BLOCK_SIZE);
	buffer->device_uuid = cpu_to_be64(device->ldev->md.device_uuid);

	buffer->bm_offset = cpu_to_be32(device->ldev->md.bm_offset);
	buffer->la_peer_max_bio_size = cpu_to_be32(device->peer_max_bio_size);

	buffer->al_stripes = cpu_to_be32(device->ldev->md.al_stripes);
	buffer->al_stripe_size_4k = cpu_to_be32(device->ldev->md.al_stripe_size_4k);

	D_ASSERT(device, drbd_md_ss(device->ldev) == device->ldev->md.md_offset);
	sector = device->ldev->md.md_offset;

	if (drbd_md_sync_page_io(device, device->ldev, sector, REQ_OP_WRITE)) {
		/* this was a try anyways ... */
		drbd_err(device, "meta data update failed!\n");
		drbd_chk_io_error(device, 1, DRBD_META_IO_ERROR);
	}
}

/**
 * drbd_md_sync() - Writes the meta data super block if the MD_DIRTY flag bit is set
 * @device:	DRBD device.
 */
void drbd_md_sync(struct drbd_device *device)
{
	struct meta_data_on_disk *buffer;

	/* Don't accidentally change the DRBD meta data layout. */
	BUILD_BUG_ON(UI_SIZE != 4);
	BUILD_BUG_ON(sizeof(struct meta_data_on_disk) != 4096);

	del_timer(&device->md_sync_timer);
	/* timer may be rearmed by drbd_md_mark_dirty() now. */
	if (!test_and_clear_bit(MD_DIRTY, &device->flags))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	/* We use here D_FAILED and not D_ATTACHING because we try to write
	 * metadata even if we detach due to a disk failure! */
<<<<<<< HEAD
	if (!get_ldev_if_state(mdev, D_FAILED))
		return;

	mutex_lock(&mdev->md_io_mutex);
	buffer = (struct meta_data_on_disk *)page_address(mdev->md_io_page);
	memset(buffer, 0, 512);

	buffer->la_size = cpu_to_be64(drbd_get_capacity(mdev->this_bdev));
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		buffer->uuid[i] = cpu_to_be64(mdev->ldev->md.uuid[i]);
	buffer->flags = cpu_to_be32(mdev->ldev->md.flags);
	buffer->magic = cpu_to_be32(DRBD_MD_MAGIC);

	buffer->md_size_sect  = cpu_to_be32(mdev->ldev->md.md_size_sect);
	buffer->al_offset     = cpu_to_be32(mdev->ldev->md.al_offset);
	buffer->al_nr_extents = cpu_to_be32(mdev->act_log->nr_elements);
	buffer->bm_bytes_per_bit = cpu_to_be32(BM_BLOCK_SIZE);
	buffer->device_uuid = cpu_to_be64(mdev->ldev->md.device_uuid);

	buffer->bm_offset = cpu_to_be32(mdev->ldev->md.bm_offset);
	buffer->la_peer_max_bio_size = cpu_to_be32(mdev->peer_max_bio_size);

	D_ASSERT(drbd_md_ss__(mdev, mdev->ldev) == mdev->ldev->md.md_offset);
	sector = mdev->ldev->md.md_offset;

	if (!drbd_md_sync_page_io(mdev, mdev->ldev, sector, WRITE)) {
		/* this was a try anyways ... */
		dev_err(DEV, "meta data update failed!\n");
		drbd_chk_io_error(mdev, 1, true);
	}

	/* Update mdev->ldev->md.la_size_sect,
	 * since we updated it on metadata. */
	mdev->ldev->md.la_size_sect = drbd_get_capacity(mdev->this_bdev);

	mutex_unlock(&mdev->md_io_mutex);
	put_ldev(mdev);
}

/**
 * drbd_md_read() - Reads in the meta data super block
 * @mdev:	DRBD device.
 * @bdev:	Device from which the meta data should be read in.
 *
 * Return 0 (NO_ERROR) on success, and an enum drbd_ret_code in case
 * something goes wrong.  Currently only: ERR_IO_MD_DISK, ERR_MD_INVALID.
 */
int drbd_md_read(struct drbd_conf *mdev, struct drbd_backing_dev *bdev)
{
	struct meta_data_on_disk *buffer;
	int i, rv = NO_ERROR;

	if (!get_ldev_if_state(mdev, D_ATTACHING))
		return ERR_IO_MD_DISK;

	mutex_lock(&mdev->md_io_mutex);
	buffer = (struct meta_data_on_disk *)page_address(mdev->md_io_page);

	if (!drbd_md_sync_page_io(mdev, bdev, bdev->md.md_offset, READ)) {
		/* NOTE: can't do normal error processing here as this is
		   called BEFORE disk is attached */
		dev_err(DEV, "Error while reading metadata.\n");
=======
	if (!get_ldev_if_state(device, D_FAILED))
		return;

	buffer = drbd_md_get_buffer(device, __func__);
	if (!buffer)
		goto out;

	drbd_md_write(device, buffer);

	/* Update device->ldev->md.la_size_sect,
	 * since we updated it on metadata. */
	device->ldev->md.la_size_sect = get_capacity(device->vdisk);

	drbd_md_put_buffer(device);
out:
	put_ldev(device);
}

static int check_activity_log_stripe_size(struct drbd_device *device,
		struct meta_data_on_disk *on_disk,
		struct drbd_md *in_core)
{
	u32 al_stripes = be32_to_cpu(on_disk->al_stripes);
	u32 al_stripe_size_4k = be32_to_cpu(on_disk->al_stripe_size_4k);
	u64 al_size_4k;

	/* both not set: default to old fixed size activity log */
	if (al_stripes == 0 && al_stripe_size_4k == 0) {
		al_stripes = 1;
		al_stripe_size_4k = MD_32kB_SECT/8;
	}

	/* some paranoia plausibility checks */

	/* we need both values to be set */
	if (al_stripes == 0 || al_stripe_size_4k == 0)
		goto err;

	al_size_4k = (u64)al_stripes * al_stripe_size_4k;

	/* Upper limit of activity log area, to avoid potential overflow
	 * problems in al_tr_number_to_on_disk_sector(). As right now, more
	 * than 72 * 4k blocks total only increases the amount of history,
	 * limiting this arbitrarily to 16 GB is not a real limitation ;-)  */
	if (al_size_4k > (16 * 1024 * 1024/4))
		goto err;

	/* Lower limit: we need at least 8 transaction slots (32kB)
	 * to not break existing setups */
	if (al_size_4k < MD_32kB_SECT/8)
		goto err;

	in_core->al_stripe_size_4k = al_stripe_size_4k;
	in_core->al_stripes = al_stripes;
	in_core->al_size_4k = al_size_4k;

	return 0;
err:
	drbd_err(device, "invalid activity log striping: al_stripes=%u, al_stripe_size_4k=%u\n",
			al_stripes, al_stripe_size_4k);
	return -EINVAL;
}

static int check_offsets_and_sizes(struct drbd_device *device, struct drbd_backing_dev *bdev)
{
	sector_t capacity = drbd_get_capacity(bdev->md_bdev);
	struct drbd_md *in_core = &bdev->md;
	s32 on_disk_al_sect;
	s32 on_disk_bm_sect;

	/* The on-disk size of the activity log, calculated from offsets, and
	 * the size of the activity log calculated from the stripe settings,
	 * should match.
	 * Though we could relax this a bit: it is ok, if the striped activity log
	 * fits in the available on-disk activity log size.
	 * Right now, that would break how resize is implemented.
	 * TODO: make drbd_determine_dev_size() (and the drbdmeta tool) aware
	 * of possible unused padding space in the on disk layout. */
	if (in_core->al_offset < 0) {
		if (in_core->bm_offset > in_core->al_offset)
			goto err;
		on_disk_al_sect = -in_core->al_offset;
		on_disk_bm_sect = in_core->al_offset - in_core->bm_offset;
	} else {
		if (in_core->al_offset != MD_4kB_SECT)
			goto err;
		if (in_core->bm_offset < in_core->al_offset + in_core->al_size_4k * MD_4kB_SECT)
			goto err;

		on_disk_al_sect = in_core->bm_offset - MD_4kB_SECT;
		on_disk_bm_sect = in_core->md_size_sect - in_core->bm_offset;
	}

	/* old fixed size meta data is exactly that: fixed. */
	if (in_core->meta_dev_idx >= 0) {
		if (in_core->md_size_sect != MD_128MB_SECT
		||  in_core->al_offset != MD_4kB_SECT
		||  in_core->bm_offset != MD_4kB_SECT + MD_32kB_SECT
		||  in_core->al_stripes != 1
		||  in_core->al_stripe_size_4k != MD_32kB_SECT/8)
			goto err;
	}

	if (capacity < in_core->md_size_sect)
		goto err;
	if (capacity - in_core->md_size_sect < drbd_md_first_sector(bdev))
		goto err;

	/* should be aligned, and at least 32k */
	if ((on_disk_al_sect & 7) || (on_disk_al_sect < MD_32kB_SECT))
		goto err;

	/* should fit (for now: exactly) into the available on-disk space;
	 * overflow prevention is in check_activity_log_stripe_size() above. */
	if (on_disk_al_sect != in_core->al_size_4k * MD_4kB_SECT)
		goto err;

	/* again, should be aligned */
	if (in_core->bm_offset & 7)
		goto err;

	/* FIXME check for device grow with flex external meta data? */

	/* can the available bitmap space cover the last agreed device size? */
	if (on_disk_bm_sect < (in_core->la_size_sect+7)/MD_4kB_SECT/8/512)
		goto err;

	return 0;

err:
	drbd_err(device, "meta data offsets don't make sense: idx=%d "
			"al_s=%u, al_sz4k=%u, al_offset=%d, bm_offset=%d, "
			"md_size_sect=%u, la_size=%llu, md_capacity=%llu\n",
			in_core->meta_dev_idx,
			in_core->al_stripes, in_core->al_stripe_size_4k,
			in_core->al_offset, in_core->bm_offset, in_core->md_size_sect,
			(unsigned long long)in_core->la_size_sect,
			(unsigned long long)capacity);

	return -EINVAL;
}


/**
 * drbd_md_read() - Reads in the meta data super block
 * @device:	DRBD device.
 * @bdev:	Device from which the meta data should be read in.
 *
 * Return NO_ERROR on success, and an enum drbd_ret_code in case
 * something goes wrong.
 *
 * Called exactly once during drbd_adm_attach(), while still being D_DISKLESS,
 * even before @bdev is assigned to @device->ldev.
 */
int drbd_md_read(struct drbd_device *device, struct drbd_backing_dev *bdev)
{
	struct meta_data_on_disk *buffer;
	u32 magic, flags;
	int i, rv = NO_ERROR;

	if (device->state.disk != D_DISKLESS)
		return ERR_DISK_CONFIGURED;

	buffer = drbd_md_get_buffer(device, __func__);
	if (!buffer)
		return ERR_NOMEM;

	/* First, figure out where our meta data superblock is located,
	 * and read it. */
	bdev->md.meta_dev_idx = bdev->disk_conf->meta_dev_idx;
	bdev->md.md_offset = drbd_md_ss(bdev);
	/* Even for (flexible or indexed) external meta data,
	 * initially restrict us to the 4k superblock for now.
	 * Affects the paranoia out-of-range access check in drbd_md_sync_page_io(). */
	bdev->md.md_size_sect = 8;

	if (drbd_md_sync_page_io(device, bdev, bdev->md.md_offset,
				 REQ_OP_READ)) {
		/* NOTE: can't do normal error processing here as this is
		   called BEFORE disk is attached */
		drbd_err(device, "Error while reading metadata.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rv = ERR_IO_MD_DISK;
		goto err;
	}

<<<<<<< HEAD
	if (be32_to_cpu(buffer->magic) != DRBD_MD_MAGIC) {
		dev_err(DEV, "Error while reading metadata, magic not found.\n");
		rv = ERR_MD_INVALID;
		goto err;
	}
	if (be32_to_cpu(buffer->al_offset) != bdev->md.al_offset) {
		dev_err(DEV, "unexpected al_offset: %d (expected %d)\n",
		    be32_to_cpu(buffer->al_offset), bdev->md.al_offset);
		rv = ERR_MD_INVALID;
		goto err;
	}
	if (be32_to_cpu(buffer->bm_offset) != bdev->md.bm_offset) {
		dev_err(DEV, "unexpected bm_offset: %d (expected %d)\n",
		    be32_to_cpu(buffer->bm_offset), bdev->md.bm_offset);
		rv = ERR_MD_INVALID;
		goto err;
	}
	if (be32_to_cpu(buffer->md_size_sect) != bdev->md.md_size_sect) {
		dev_err(DEV, "unexpected md_size: %u (expected %u)\n",
		    be32_to_cpu(buffer->md_size_sect), bdev->md.md_size_sect);
		rv = ERR_MD_INVALID;
=======
	magic = be32_to_cpu(buffer->magic);
	flags = be32_to_cpu(buffer->flags);
	if (magic == DRBD_MD_MAGIC_84_UNCLEAN ||
	    (magic == DRBD_MD_MAGIC_08 && !(flags & MDF_AL_CLEAN))) {
			/* btw: that's Activity Log clean, not "all" clean. */
		drbd_err(device, "Found unclean meta data. Did you \"drbdadm apply-al\"?\n");
		rv = ERR_MD_UNCLEAN;
		goto err;
	}

	rv = ERR_MD_INVALID;
	if (magic != DRBD_MD_MAGIC_08) {
		if (magic == DRBD_MD_MAGIC_07)
			drbd_err(device, "Found old (0.7) meta data magic. Did you \"drbdadm create-md\"?\n");
		else
			drbd_err(device, "Meta data magic not found. Did you \"drbdadm create-md\"?\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err;
	}

	if (be32_to_cpu(buffer->bm_bytes_per_bit) != BM_BLOCK_SIZE) {
<<<<<<< HEAD
		dev_err(DEV, "unexpected bm_bytes_per_bit: %u (expected %u)\n",
		    be32_to_cpu(buffer->bm_bytes_per_bit), BM_BLOCK_SIZE);
		rv = ERR_MD_INVALID;
		goto err;
	}

	bdev->md.la_size_sect = be64_to_cpu(buffer->la_size);
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		bdev->md.uuid[i] = be64_to_cpu(buffer->uuid[i]);
	bdev->md.flags = be32_to_cpu(buffer->flags);
	mdev->sync_conf.al_extents = be32_to_cpu(buffer->al_nr_extents);
	bdev->md.device_uuid = be64_to_cpu(buffer->device_uuid);

	spin_lock_irq(&mdev->req_lock);
	if (mdev->state.conn < C_CONNECTED) {
		int peer;
		peer = be32_to_cpu(buffer->la_peer_max_bio_size);
		peer = max_t(int, peer, DRBD_MAX_BIO_SIZE_SAFE);
		mdev->peer_max_bio_size = peer;
	}
	spin_unlock_irq(&mdev->req_lock);

	if (mdev->sync_conf.al_extents < 7)
		mdev->sync_conf.al_extents = 127;

 err:
	mutex_unlock(&mdev->md_io_mutex);
	put_ldev(mdev);
=======
		drbd_err(device, "unexpected bm_bytes_per_bit: %u (expected %u)\n",
		    be32_to_cpu(buffer->bm_bytes_per_bit), BM_BLOCK_SIZE);
		goto err;
	}


	/* convert to in_core endian */
	bdev->md.la_size_sect = be64_to_cpu(buffer->la_size_sect);
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		bdev->md.uuid[i] = be64_to_cpu(buffer->uuid[i]);
	bdev->md.flags = be32_to_cpu(buffer->flags);
	bdev->md.device_uuid = be64_to_cpu(buffer->device_uuid);

	bdev->md.md_size_sect = be32_to_cpu(buffer->md_size_sect);
	bdev->md.al_offset = be32_to_cpu(buffer->al_offset);
	bdev->md.bm_offset = be32_to_cpu(buffer->bm_offset);

	if (check_activity_log_stripe_size(device, buffer, &bdev->md))
		goto err;
	if (check_offsets_and_sizes(device, bdev))
		goto err;

	if (be32_to_cpu(buffer->bm_offset) != bdev->md.bm_offset) {
		drbd_err(device, "unexpected bm_offset: %d (expected %d)\n",
		    be32_to_cpu(buffer->bm_offset), bdev->md.bm_offset);
		goto err;
	}
	if (be32_to_cpu(buffer->md_size_sect) != bdev->md.md_size_sect) {
		drbd_err(device, "unexpected md_size: %u (expected %u)\n",
		    be32_to_cpu(buffer->md_size_sect), bdev->md.md_size_sect);
		goto err;
	}

	rv = NO_ERROR;

	spin_lock_irq(&device->resource->req_lock);
	if (device->state.conn < C_CONNECTED) {
		unsigned int peer;
		peer = be32_to_cpu(buffer->la_peer_max_bio_size);
		peer = max(peer, DRBD_MAX_BIO_SIZE_SAFE);
		device->peer_max_bio_size = peer;
	}
	spin_unlock_irq(&device->resource->req_lock);

 err:
	drbd_md_put_buffer(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rv;
}

/**
 * drbd_md_mark_dirty() - Mark meta data super block as dirty
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @device:	DRBD device.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Call this function if you change anything that should be written to
 * the meta-data super block. This function sets MD_DIRTY, and starts a
 * timer that ensures that within five seconds you have to call drbd_md_sync().
 */
<<<<<<< HEAD
#ifdef DEBUG
void drbd_md_mark_dirty_(struct drbd_conf *mdev, unsigned int line, const char *func)
{
	if (!test_and_set_bit(MD_DIRTY, &mdev->flags)) {
		mod_timer(&mdev->md_sync_timer, jiffies + HZ);
		mdev->last_md_mark_dirty.line = line;
		mdev->last_md_mark_dirty.func = func;
	}
}
#else
void drbd_md_mark_dirty(struct drbd_conf *mdev)
{
	if (!test_and_set_bit(MD_DIRTY, &mdev->flags))
		mod_timer(&mdev->md_sync_timer, jiffies + 5*HZ);
}
#endif

static void drbd_uuid_move_history(struct drbd_conf *mdev) __must_hold(local)
=======
void drbd_md_mark_dirty(struct drbd_device *device)
{
	if (!test_and_set_bit(MD_DIRTY, &device->flags))
		mod_timer(&device->md_sync_timer, jiffies + 5*HZ);
}

void drbd_uuid_move_history(struct drbd_device *device) __must_hold(local)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;

	for (i = UI_HISTORY_START; i < UI_HISTORY_END; i++)
<<<<<<< HEAD
		mdev->ldev->md.uuid[i+1] = mdev->ldev->md.uuid[i];
}

void _drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local)
{
	if (idx == UI_CURRENT) {
		if (mdev->state.role == R_PRIMARY)
=======
		device->ldev->md.uuid[i+1] = device->ldev->md.uuid[i];
}

void __drbd_uuid_set(struct drbd_device *device, int idx, u64 val) __must_hold(local)
{
	if (idx == UI_CURRENT) {
		if (device->state.role == R_PRIMARY)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			val |= 1;
		else
			val &= ~((u64)1);

<<<<<<< HEAD
		drbd_set_ed_uuid(mdev, val);
	}

	mdev->ldev->md.uuid[idx] = val;
	drbd_md_mark_dirty(mdev);
}


void drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local)
{
	if (mdev->ldev->md.uuid[idx]) {
		drbd_uuid_move_history(mdev);
		mdev->ldev->md.uuid[UI_HISTORY_START] = mdev->ldev->md.uuid[idx];
	}
	_drbd_uuid_set(mdev, idx, val);
=======
		drbd_set_ed_uuid(device, val);
	}

	device->ldev->md.uuid[idx] = val;
	drbd_md_mark_dirty(device);
}

void _drbd_uuid_set(struct drbd_device *device, int idx, u64 val) __must_hold(local)
{
	unsigned long flags;
	spin_lock_irqsave(&device->ldev->md.uuid_lock, flags);
	__drbd_uuid_set(device, idx, val);
	spin_unlock_irqrestore(&device->ldev->md.uuid_lock, flags);
}

void drbd_uuid_set(struct drbd_device *device, int idx, u64 val) __must_hold(local)
{
	unsigned long flags;
	spin_lock_irqsave(&device->ldev->md.uuid_lock, flags);
	if (device->ldev->md.uuid[idx]) {
		drbd_uuid_move_history(device);
		device->ldev->md.uuid[UI_HISTORY_START] = device->ldev->md.uuid[idx];
	}
	__drbd_uuid_set(device, idx, val);
	spin_unlock_irqrestore(&device->ldev->md.uuid_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * drbd_uuid_new_current() - Creates a new current UUID
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @device:	DRBD device.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Creates a new current UUID, and rotates the old current UUID into
 * the bitmap slot. Causes an incremental resync upon next connect.
 */
<<<<<<< HEAD
void drbd_uuid_new_current(struct drbd_conf *mdev) __must_hold(local)
{
	u64 val;
	unsigned long long bm_uuid = mdev->ldev->md.uuid[UI_BITMAP];

	if (bm_uuid)
		dev_warn(DEV, "bm UUID was already set: %llX\n", bm_uuid);

	mdev->ldev->md.uuid[UI_BITMAP] = mdev->ldev->md.uuid[UI_CURRENT];

	get_random_bytes(&val, sizeof(u64));
	_drbd_uuid_set(mdev, UI_CURRENT, val);
	drbd_print_uuids(mdev, "new current UUID");
	/* get it to stable storage _now_ */
	drbd_md_sync(mdev);
}

void drbd_uuid_set_bm(struct drbd_conf *mdev, u64 val) __must_hold(local)
{
	if (mdev->ldev->md.uuid[UI_BITMAP] == 0 && val == 0)
		return;

	if (val == 0) {
		drbd_uuid_move_history(mdev);
		mdev->ldev->md.uuid[UI_HISTORY_START] = mdev->ldev->md.uuid[UI_BITMAP];
		mdev->ldev->md.uuid[UI_BITMAP] = 0;
	} else {
		unsigned long long bm_uuid = mdev->ldev->md.uuid[UI_BITMAP];
		if (bm_uuid)
			dev_warn(DEV, "bm UUID was already set: %llX\n", bm_uuid);

		mdev->ldev->md.uuid[UI_BITMAP] = val & ~((u64)1);
	}
	drbd_md_mark_dirty(mdev);
=======
void drbd_uuid_new_current(struct drbd_device *device) __must_hold(local)
{
	u64 val;
	unsigned long long bm_uuid;

	get_random_bytes(&val, sizeof(u64));

	spin_lock_irq(&device->ldev->md.uuid_lock);
	bm_uuid = device->ldev->md.uuid[UI_BITMAP];

	if (bm_uuid)
		drbd_warn(device, "bm UUID was already set: %llX\n", bm_uuid);

	device->ldev->md.uuid[UI_BITMAP] = device->ldev->md.uuid[UI_CURRENT];
	__drbd_uuid_set(device, UI_CURRENT, val);
	spin_unlock_irq(&device->ldev->md.uuid_lock);

	drbd_print_uuids(device, "new current UUID");
	/* get it to stable storage _now_ */
	drbd_md_sync(device);
}

void drbd_uuid_set_bm(struct drbd_device *device, u64 val) __must_hold(local)
{
	unsigned long flags;
	if (device->ldev->md.uuid[UI_BITMAP] == 0 && val == 0)
		return;

	spin_lock_irqsave(&device->ldev->md.uuid_lock, flags);
	if (val == 0) {
		drbd_uuid_move_history(device);
		device->ldev->md.uuid[UI_HISTORY_START] = device->ldev->md.uuid[UI_BITMAP];
		device->ldev->md.uuid[UI_BITMAP] = 0;
	} else {
		unsigned long long bm_uuid = device->ldev->md.uuid[UI_BITMAP];
		if (bm_uuid)
			drbd_warn(device, "bm UUID was already set: %llX\n", bm_uuid);

		device->ldev->md.uuid[UI_BITMAP] = val & ~((u64)1);
	}
	spin_unlock_irqrestore(&device->ldev->md.uuid_lock, flags);

	drbd_md_mark_dirty(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * drbd_bmio_set_n_write() - io_fn for drbd_queue_bitmap_io() or drbd_bitmap_io()
<<<<<<< HEAD
 * @mdev:	DRBD device.
 *
 * Sets all bits in the bitmap and writes the whole bitmap to stable storage.
 */
int drbd_bmio_set_n_write(struct drbd_conf *mdev)
{
	int rv = -EIO;

	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		drbd_md_set_flag(mdev, MDF_FULL_SYNC);
		drbd_md_sync(mdev);
		drbd_bm_set_all(mdev);

		rv = drbd_bm_write(mdev);

		if (!rv) {
			drbd_md_clear_flag(mdev, MDF_FULL_SYNC);
			drbd_md_sync(mdev);
		}

		put_ldev(mdev);
=======
 * @device:	DRBD device.
 *
 * Sets all bits in the bitmap and writes the whole bitmap to stable storage.
 */
int drbd_bmio_set_n_write(struct drbd_device *device,
			  struct drbd_peer_device *peer_device) __must_hold(local)

{
	int rv = -EIO;

	drbd_md_set_flag(device, MDF_FULL_SYNC);
	drbd_md_sync(device);
	drbd_bm_set_all(device);

	rv = drbd_bm_write(device, peer_device);

	if (!rv) {
		drbd_md_clear_flag(device, MDF_FULL_SYNC);
		drbd_md_sync(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return rv;
}

/**
 * drbd_bmio_clear_n_write() - io_fn for drbd_queue_bitmap_io() or drbd_bitmap_io()
<<<<<<< HEAD
 * @mdev:	DRBD device.
 *
 * Clears all bits in the bitmap and writes the whole bitmap to stable storage.
 */
int drbd_bmio_clear_n_write(struct drbd_conf *mdev)
{
	int rv = -EIO;

	drbd_resume_al(mdev);
	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		drbd_bm_clear_all(mdev);
		rv = drbd_bm_write(mdev);
		put_ldev(mdev);
	}

	return rv;
}

static int w_bitmap_io(struct drbd_conf *mdev, struct drbd_work *w, int unused)
{
	struct bm_io_work *work = container_of(w, struct bm_io_work, w);
	int rv = -EIO;

	D_ASSERT(atomic_read(&mdev->ap_bio_cnt) == 0);

	if (get_ldev(mdev)) {
		drbd_bm_lock(mdev, work->why, work->flags);
		rv = work->io_fn(mdev);
		drbd_bm_unlock(mdev);
		put_ldev(mdev);
	}

	clear_bit(BITMAP_IO, &mdev->flags);
	smp_mb__after_clear_bit();
	wake_up(&mdev->misc_wait);

	if (work->done)
		work->done(mdev, rv);

	clear_bit(BITMAP_IO_QUEUED, &mdev->flags);
	work->why = NULL;
	work->flags = 0;

	return 1;
}

void drbd_ldev_destroy(struct drbd_conf *mdev)
{
	lc_destroy(mdev->resync);
	mdev->resync = NULL;
	lc_destroy(mdev->act_log);
	mdev->act_log = NULL;
	__no_warn(local,
		drbd_free_bc(mdev->ldev);
		mdev->ldev = NULL;);

	if (mdev->md_io_tmpp) {
		__free_page(mdev->md_io_tmpp);
		mdev->md_io_tmpp = NULL;
	}
	clear_bit(GO_DISKLESS, &mdev->flags);
}

static int w_go_diskless(struct drbd_conf *mdev, struct drbd_work *w, int unused)
{
	D_ASSERT(mdev->state.disk == D_FAILED);
	/* we cannot assert local_cnt == 0 here, as get_ldev_if_state will
	 * inc/dec it frequently. Once we are D_DISKLESS, no one will touch
	 * the protected members anymore, though, so once put_ldev reaches zero
	 * again, it will be safe to free them. */
	drbd_force_state(mdev, NS(disk, D_DISKLESS));
	return 1;
}

void drbd_go_diskless(struct drbd_conf *mdev)
{
	D_ASSERT(mdev->state.disk == D_FAILED);
	if (!test_and_set_bit(GO_DISKLESS, &mdev->flags))
		drbd_queue_work(&mdev->data.work, &mdev->go_diskless);
=======
 * @device:	DRBD device.
 *
 * Clears all bits in the bitmap and writes the whole bitmap to stable storage.
 */
int drbd_bmio_clear_n_write(struct drbd_device *device,
			  struct drbd_peer_device *peer_device) __must_hold(local)

{
	drbd_resume_al(device);
	drbd_bm_clear_all(device);
	return drbd_bm_write(device, peer_device);
}

static int w_bitmap_io(struct drbd_work *w, int unused)
{
	struct drbd_device *device =
		container_of(w, struct drbd_device, bm_io_work.w);
	struct bm_io_work *work = &device->bm_io_work;
	int rv = -EIO;

	if (work->flags != BM_LOCKED_CHANGE_ALLOWED) {
		int cnt = atomic_read(&device->ap_bio_cnt);
		if (cnt)
			drbd_err(device, "FIXME: ap_bio_cnt %d, expected 0; queued for '%s'\n",
					cnt, work->why);
	}

	if (get_ldev(device)) {
		drbd_bm_lock(device, work->why, work->flags);
		rv = work->io_fn(device, work->peer_device);
		drbd_bm_unlock(device);
		put_ldev(device);
	}

	clear_bit_unlock(BITMAP_IO, &device->flags);
	wake_up(&device->misc_wait);

	if (work->done)
		work->done(device, rv);

	clear_bit(BITMAP_IO_QUEUED, &device->flags);
	work->why = NULL;
	work->flags = 0;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * drbd_queue_bitmap_io() - Queues an IO operation on the whole bitmap
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @io_fn:	IO callback to be called when bitmap IO is possible
 * @done:	callback to be called after the bitmap IO was performed
 * @why:	Descriptive text of the reason for doing the IO
=======
 * @device:	DRBD device.
 * @io_fn:	IO callback to be called when bitmap IO is possible
 * @done:	callback to be called after the bitmap IO was performed
 * @why:	Descriptive text of the reason for doing the IO
 * @flags:	Bitmap flags
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * While IO on the bitmap happens we freeze application IO thus we ensure
 * that drbd_set_out_of_sync() can not be called. This function MAY ONLY be
 * called from worker context. It MUST NOT be used while a previous such
 * work is still pending!
<<<<<<< HEAD
 */
void drbd_queue_bitmap_io(struct drbd_conf *mdev,
			  int (*io_fn)(struct drbd_conf *),
			  void (*done)(struct drbd_conf *, int),
			  char *why, enum bm_flag flags)
{
	D_ASSERT(current == mdev->worker.task);

	D_ASSERT(!test_bit(BITMAP_IO_QUEUED, &mdev->flags));
	D_ASSERT(!test_bit(BITMAP_IO, &mdev->flags));
	D_ASSERT(list_empty(&mdev->bm_io_work.w.list));
	if (mdev->bm_io_work.why)
		dev_err(DEV, "FIXME going to queue '%s' but '%s' still pending?\n",
			why, mdev->bm_io_work.why);

	mdev->bm_io_work.io_fn = io_fn;
	mdev->bm_io_work.done = done;
	mdev->bm_io_work.why = why;
	mdev->bm_io_work.flags = flags;

	spin_lock_irq(&mdev->req_lock);
	set_bit(BITMAP_IO, &mdev->flags);
	if (atomic_read(&mdev->ap_bio_cnt) == 0) {
		if (!test_and_set_bit(BITMAP_IO_QUEUED, &mdev->flags))
			drbd_queue_work(&mdev->data.work, &mdev->bm_io_work.w);
	}
	spin_unlock_irq(&mdev->req_lock);
=======
 *
 * Its worker function encloses the call of io_fn() by get_ldev() and
 * put_ldev().
 */
void drbd_queue_bitmap_io(struct drbd_device *device,
			  int (*io_fn)(struct drbd_device *, struct drbd_peer_device *),
			  void (*done)(struct drbd_device *, int),
			  char *why, enum bm_flag flags,
			  struct drbd_peer_device *peer_device)
{
	D_ASSERT(device, current == peer_device->connection->worker.task);

	D_ASSERT(device, !test_bit(BITMAP_IO_QUEUED, &device->flags));
	D_ASSERT(device, !test_bit(BITMAP_IO, &device->flags));
	D_ASSERT(device, list_empty(&device->bm_io_work.w.list));
	if (device->bm_io_work.why)
		drbd_err(device, "FIXME going to queue '%s' but '%s' still pending?\n",
			why, device->bm_io_work.why);

	device->bm_io_work.peer_device = peer_device;
	device->bm_io_work.io_fn = io_fn;
	device->bm_io_work.done = done;
	device->bm_io_work.why = why;
	device->bm_io_work.flags = flags;

	spin_lock_irq(&device->resource->req_lock);
	set_bit(BITMAP_IO, &device->flags);
	/* don't wait for pending application IO if the caller indicates that
	 * application IO does not conflict anyways. */
	if (flags == BM_LOCKED_CHANGE_ALLOWED || atomic_read(&device->ap_bio_cnt) == 0) {
		if (!test_and_set_bit(BITMAP_IO_QUEUED, &device->flags))
			drbd_queue_work(&peer_device->connection->sender_work,
					&device->bm_io_work.w);
	}
	spin_unlock_irq(&device->resource->req_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * drbd_bitmap_io() -  Does an IO operation on the whole bitmap
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @io_fn:	IO callback to be called when bitmap IO is possible
 * @why:	Descriptive text of the reason for doing the IO
=======
 * @device:	DRBD device.
 * @io_fn:	IO callback to be called when bitmap IO is possible
 * @why:	Descriptive text of the reason for doing the IO
 * @flags:	Bitmap flags
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * freezes application IO while that the actual IO operations runs. This
 * functions MAY NOT be called from worker context.
 */
<<<<<<< HEAD
int drbd_bitmap_io(struct drbd_conf *mdev, int (*io_fn)(struct drbd_conf *),
		char *why, enum bm_flag flags)
{
	int rv;

	D_ASSERT(current != mdev->worker.task);

	if ((flags & BM_LOCKED_SET_ALLOWED) == 0)
		drbd_suspend_io(mdev);

	drbd_bm_lock(mdev, why, flags);
	rv = io_fn(mdev);
	drbd_bm_unlock(mdev);

	if ((flags & BM_LOCKED_SET_ALLOWED) == 0)
		drbd_resume_io(mdev);
=======
int drbd_bitmap_io(struct drbd_device *device,
		int (*io_fn)(struct drbd_device *, struct drbd_peer_device *),
		char *why, enum bm_flag flags,
		struct drbd_peer_device *peer_device)
{
	/* Only suspend io, if some operation is supposed to be locked out */
	const bool do_suspend_io = flags & (BM_DONT_CLEAR|BM_DONT_SET|BM_DONT_TEST);
	int rv;

	D_ASSERT(device, current != first_peer_device(device)->connection->worker.task);

	if (do_suspend_io)
		drbd_suspend_io(device);

	drbd_bm_lock(device, why, flags);
	rv = io_fn(device, peer_device);
	drbd_bm_unlock(device);

	if (do_suspend_io)
		drbd_resume_io(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rv;
}

<<<<<<< HEAD
void drbd_md_set_flag(struct drbd_conf *mdev, int flag) __must_hold(local)
{
	if ((mdev->ldev->md.flags & flag) != flag) {
		drbd_md_mark_dirty(mdev);
		mdev->ldev->md.flags |= flag;
	}
}

void drbd_md_clear_flag(struct drbd_conf *mdev, int flag) __must_hold(local)
{
	if ((mdev->ldev->md.flags & flag) != 0) {
		drbd_md_mark_dirty(mdev);
		mdev->ldev->md.flags &= ~flag;
=======
void drbd_md_set_flag(struct drbd_device *device, int flag) __must_hold(local)
{
	if ((device->ldev->md.flags & flag) != flag) {
		drbd_md_mark_dirty(device);
		device->ldev->md.flags |= flag;
	}
}

void drbd_md_clear_flag(struct drbd_device *device, int flag) __must_hold(local)
{
	if ((device->ldev->md.flags & flag) != 0) {
		drbd_md_mark_dirty(device);
		device->ldev->md.flags &= ~flag;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
int drbd_md_test_flag(struct drbd_backing_dev *bdev, int flag)
{
	return (bdev->md.flags & flag) != 0;
}

<<<<<<< HEAD
static void md_sync_timer_fn(unsigned long data)
{
	struct drbd_conf *mdev = (struct drbd_conf *) data;

	drbd_queue_work_front(&mdev->data.work, &mdev->md_sync_work);
}

static int w_md_sync(struct drbd_conf *mdev, struct drbd_work *w, int unused)
{
	dev_warn(DEV, "md_sync_timer expired! Worker calls drbd_md_sync().\n");
#ifdef DEBUG
	dev_warn(DEV, "last md_mark_dirty: %s:%u\n",
		mdev->last_md_mark_dirty.func, mdev->last_md_mark_dirty.line);
#endif
	drbd_md_sync(mdev);
	return 1;
=======
static void md_sync_timer_fn(struct timer_list *t)
{
	struct drbd_device *device = from_timer(device, t, md_sync_timer);
	drbd_device_post_work(device, MD_SYNC);
}

const char *cmdname(enum drbd_packet cmd)
{
	/* THINK may need to become several global tables
	 * when we want to support more than
	 * one PRO_VERSION */
	static const char *cmdnames[] = {

		[P_DATA]	        = "Data",
		[P_DATA_REPLY]	        = "DataReply",
		[P_RS_DATA_REPLY]	= "RSDataReply",
		[P_BARRIER]	        = "Barrier",
		[P_BITMAP]	        = "ReportBitMap",
		[P_BECOME_SYNC_TARGET]  = "BecomeSyncTarget",
		[P_BECOME_SYNC_SOURCE]  = "BecomeSyncSource",
		[P_UNPLUG_REMOTE]	= "UnplugRemote",
		[P_DATA_REQUEST]	= "DataRequest",
		[P_RS_DATA_REQUEST]     = "RSDataRequest",
		[P_SYNC_PARAM]	        = "SyncParam",
		[P_PROTOCOL]            = "ReportProtocol",
		[P_UUIDS]	        = "ReportUUIDs",
		[P_SIZES]	        = "ReportSizes",
		[P_STATE]	        = "ReportState",
		[P_SYNC_UUID]           = "ReportSyncUUID",
		[P_AUTH_CHALLENGE]      = "AuthChallenge",
		[P_AUTH_RESPONSE]	= "AuthResponse",
		[P_STATE_CHG_REQ]       = "StateChgRequest",
		[P_PING]		= "Ping",
		[P_PING_ACK]	        = "PingAck",
		[P_RECV_ACK]	        = "RecvAck",
		[P_WRITE_ACK]	        = "WriteAck",
		[P_RS_WRITE_ACK]	= "RSWriteAck",
		[P_SUPERSEDED]          = "Superseded",
		[P_NEG_ACK]	        = "NegAck",
		[P_NEG_DREPLY]	        = "NegDReply",
		[P_NEG_RS_DREPLY]	= "NegRSDReply",
		[P_BARRIER_ACK]	        = "BarrierAck",
		[P_STATE_CHG_REPLY]     = "StateChgReply",
		[P_OV_REQUEST]          = "OVRequest",
		[P_OV_REPLY]            = "OVReply",
		[P_OV_RESULT]           = "OVResult",
		[P_CSUM_RS_REQUEST]     = "CsumRSRequest",
		[P_RS_IS_IN_SYNC]	= "CsumRSIsInSync",
		[P_SYNC_PARAM89]	= "SyncParam89",
		[P_COMPRESSED_BITMAP]   = "CBitmap",
		[P_DELAY_PROBE]         = "DelayProbe",
		[P_OUT_OF_SYNC]		= "OutOfSync",
		[P_RS_CANCEL]		= "RSCancel",
		[P_CONN_ST_CHG_REQ]	= "conn_st_chg_req",
		[P_CONN_ST_CHG_REPLY]	= "conn_st_chg_reply",
		[P_PROTOCOL_UPDATE]	= "protocol_update",
		[P_TRIM]	        = "Trim",
		[P_RS_THIN_REQ]         = "rs_thin_req",
		[P_RS_DEALLOCATED]      = "rs_deallocated",
		[P_WSAME]	        = "WriteSame",
		[P_ZEROES]		= "Zeroes",

		/* enum drbd_packet, but not commands - obsoleted flags:
		 *	P_MAY_IGNORE
		 *	P_MAX_OPT_CMD
		 */
	};

	/* too big for the array: 0xfffX */
	if (cmd == P_INITIAL_META)
		return "InitialMeta";
	if (cmd == P_INITIAL_DATA)
		return "InitialData";
	if (cmd == P_CONNECTION_FEATURES)
		return "ConnectionFeatures";
	if (cmd >= ARRAY_SIZE(cmdnames))
		return "Unknown";
	return cmdnames[cmd];
}

/**
 * drbd_wait_misc  -  wait for a request to make progress
 * @device:	device associated with the request
 * @i:		the struct drbd_interval embedded in struct drbd_request or
 *		struct drbd_peer_request
 */
int drbd_wait_misc(struct drbd_device *device, struct drbd_interval *i)
{
	struct net_conf *nc;
	DEFINE_WAIT(wait);
	long timeout;

	rcu_read_lock();
	nc = rcu_dereference(first_peer_device(device)->connection->net_conf);
	if (!nc) {
		rcu_read_unlock();
		return -ETIMEDOUT;
	}
	timeout = nc->ko_count ? nc->timeout * HZ / 10 * nc->ko_count : MAX_SCHEDULE_TIMEOUT;
	rcu_read_unlock();

	/* Indicate to wake up device->misc_wait on progress.  */
	i->waiting = true;
	prepare_to_wait(&device->misc_wait, &wait, TASK_INTERRUPTIBLE);
	spin_unlock_irq(&device->resource->req_lock);
	timeout = schedule_timeout(timeout);
	finish_wait(&device->misc_wait, &wait);
	spin_lock_irq(&device->resource->req_lock);
	if (!timeout || device->state.conn < C_CONNECTED)
		return -ETIMEDOUT;
	if (signal_pending(current))
		return -ERESTARTSYS;
	return 0;
}

void lock_all_resources(void)
{
	struct drbd_resource *resource;
	int __maybe_unused i = 0;

	mutex_lock(&resources_mutex);
	local_irq_disable();
	for_each_resource(resource, &drbd_resources)
		spin_lock_nested(&resource->req_lock, i++);
}

void unlock_all_resources(void)
{
	struct drbd_resource *resource;

	for_each_resource(resource, &drbd_resources)
		spin_unlock(&resource->req_lock);
	local_irq_enable();
	mutex_unlock(&resources_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_DRBD_FAULT_INJECTION
/* Fault insertion support including random number generator shamelessly
 * stolen from kernel/rcutorture.c */
struct fault_random_state {
	unsigned long state;
	unsigned long count;
};

#define FAULT_RANDOM_MULT 39916801  /* prime */
#define FAULT_RANDOM_ADD	479001701 /* prime */
#define FAULT_RANDOM_REFRESH 10000

/*
 * Crude but fast random-number generator.  Uses a linear congruential
 * generator, with occasional help from get_random_bytes().
 */
static unsigned long
_drbd_fault_random(struct fault_random_state *rsp)
{
	long refresh;

	if (!rsp->count--) {
		get_random_bytes(&refresh, sizeof(refresh));
		rsp->state += refresh;
		rsp->count = FAULT_RANDOM_REFRESH;
	}
	rsp->state = rsp->state * FAULT_RANDOM_MULT + FAULT_RANDOM_ADD;
	return swahw32(rsp->state);
}

static char *
_drbd_fault_str(unsigned int type) {
	static char *_faults[] = {
		[DRBD_FAULT_MD_WR] = "Meta-data write",
		[DRBD_FAULT_MD_RD] = "Meta-data read",
		[DRBD_FAULT_RS_WR] = "Resync write",
		[DRBD_FAULT_RS_RD] = "Resync read",
		[DRBD_FAULT_DT_WR] = "Data write",
		[DRBD_FAULT_DT_RD] = "Data read",
		[DRBD_FAULT_DT_RA] = "Data read ahead",
		[DRBD_FAULT_BM_ALLOC] = "BM allocation",
		[DRBD_FAULT_AL_EE] = "EE allocation",
		[DRBD_FAULT_RECEIVE] = "receive data corruption",
	};

	return (type < DRBD_FAULT_MAX) ? _faults[type] : "**Unknown**";
}

unsigned int
<<<<<<< HEAD
_drbd_insert_fault(struct drbd_conf *mdev, unsigned int type)
=======
_drbd_insert_fault(struct drbd_device *device, unsigned int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	static struct fault_random_state rrs = {0, 0};

	unsigned int ret = (
<<<<<<< HEAD
		(fault_devs == 0 ||
			((1 << mdev_to_minor(mdev)) & fault_devs) != 0) &&
		(((_drbd_fault_random(&rrs) % 100) + 1) <= fault_rate));

	if (ret) {
		fault_count++;

		if (__ratelimit(&drbd_ratelimit_state))
			dev_warn(DEV, "***Simulating %s failure\n",
=======
		(drbd_fault_devs == 0 ||
			((1 << device_to_minor(device)) & drbd_fault_devs) != 0) &&
		(((_drbd_fault_random(&rrs) % 100) + 1) <= drbd_fault_rate));

	if (ret) {
		drbd_fault_count++;

		if (drbd_ratelimit())
			drbd_warn(device, "***Simulating %s failure\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				_drbd_fault_str(type));
	}

	return ret;
}
#endif

<<<<<<< HEAD
const char *drbd_buildtag(void)
{
	/* DRBD built from external sources has here a reference to the
	   git hash of the source code. */

	static char buildtag[38] = "\0uilt-in";

	if (buildtag[0] == 0) {
#ifdef CONFIG_MODULES
		if (THIS_MODULE != NULL)
			sprintf(buildtag, "srcversion: %-24s", THIS_MODULE->srcversion);
		else
#endif
			buildtag[0] = 'b';
	}

	return buildtag;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_init(drbd_init)
module_exit(drbd_cleanup)

EXPORT_SYMBOL(drbd_conn_str);
EXPORT_SYMBOL(drbd_role_str);
EXPORT_SYMBOL(drbd_disk_str);
EXPORT_SYMBOL(drbd_set_st_err_str);
