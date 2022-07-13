<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
   drbd_worker.c

   This file is part of DRBD by Philipp Reisner and Lars Ellenberg.

   Copyright (C) 2001-2008, LINBIT Information Technologies GmbH.
   Copyright (C) 1999-2008, Philipp Reisner <philipp.reisner@linbit.com>.
   Copyright (C) 2002-2008, Lars Ellenberg <lars.ellenberg@linbit.com>.

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
#include <linux/sched.h>
=======

*/

#include <linux/module.h>
#include <linux/drbd.h>
#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/wait.h>
#include <linux/mm.h>
#include <linux/memcontrol.h>
#include <linux/mm_inline.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/string.h>
#include <linux/scatterlist.h>
<<<<<<< HEAD

#include "drbd_int.h"
#include "drbd_req.h"

static int w_make_ov_request(struct drbd_conf *mdev, struct drbd_work *w, int cancel);
static int w_make_resync_request(struct drbd_conf *mdev,
				 struct drbd_work *w, int cancel);



/* endio handlers:
 *   drbd_md_io_complete (defined here)
 *   drbd_endio_pri (defined here)
 *   drbd_endio_sec (defined here)
 *   bm_async_io_complete (defined in drbd_bitmap.c)
=======
#include <linux/part_stat.h>

#include "drbd_int.h"
#include "drbd_protocol.h"
#include "drbd_req.h"

static int make_ov_request(struct drbd_peer_device *, int);
static int make_resync_request(struct drbd_peer_device *, int);

/* endio handlers:
 *   drbd_md_endio (defined here)
 *   drbd_request_endio (defined here)
 *   drbd_peer_request_endio (defined here)
 *   drbd_bm_endio (defined in drbd_bitmap.c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * For all these callbacks, note the following:
 * The callbacks will be called in irq context by the IDE drivers,
 * and in Softirqs/Tasklets/BH context by the SCSI drivers.
 * Try to get the locking right :)
 *
 */

<<<<<<< HEAD

/* About the global_state_lock
   Each state transition on an device holds a read lock. In case we have
   to evaluate the sync after dependencies, we grab a write lock, because
   we need stable states on all devices for that.  */
rwlock_t global_state_lock;

/* used for synchronous meta data and bitmap IO
 * submitted by drbd_md_sync_page_io()
 */
void drbd_md_io_complete(struct bio *bio, int error)
{
	struct drbd_md_io *md_io;

	md_io = (struct drbd_md_io *)bio->bi_private;
	md_io->error = error;

	complete(&md_io->event);
=======
/* used for synchronous meta data and bitmap IO
 * submitted by drbd_md_sync_page_io()
 */
void drbd_md_endio(struct bio *bio)
{
	struct drbd_device *device;

	device = bio->bi_private;
	device->md_io.error = blk_status_to_errno(bio->bi_status);

	/* special case: drbd_md_read() during drbd_adm_attach() */
	if (device->ldev)
		put_ldev(device);
	bio_put(bio);

	/* We grabbed an extra reference in _drbd_md_sync_page_io() to be able
	 * to timeout on the lower level device, and eventually detach from it.
	 * If this io completion runs after that timeout expired, this
	 * drbd_md_put_buffer() may allow us to finally try and re-attach.
	 * During normal operation, this only puts that extra reference
	 * down to 1 again.
	 * Make sure we first drop the reference, and only then signal
	 * completion, or we may (in drbd_al_read_log()) cycle so fast into the
	 * next drbd_md_sync_page_io(), that we trigger the
	 * ASSERT(atomic_read(&device->md_io_in_use) == 1) there.
	 */
	drbd_md_put_buffer(device);
	device->md_io.done = 1;
	wake_up(&device->misc_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* reads on behalf of the partner,
 * "submitted" by the receiver
 */
<<<<<<< HEAD
void drbd_endio_read_sec_final(struct drbd_epoch_entry *e) __releases(local)
{
	unsigned long flags = 0;
	struct drbd_conf *mdev = e->mdev;

	D_ASSERT(e->block_id != ID_VACANT);

	spin_lock_irqsave(&mdev->req_lock, flags);
	mdev->read_cnt += e->size >> 9;
	list_del(&e->w.list);
	if (list_empty(&mdev->read_ee))
		wake_up(&mdev->ee_wait);
	if (test_bit(__EE_WAS_ERROR, &e->flags))
		__drbd_chk_io_error(mdev, false);
	spin_unlock_irqrestore(&mdev->req_lock, flags);

	drbd_queue_work(&mdev->data.work, &e->w);
	put_ldev(mdev);
=======
static void drbd_endio_read_sec_final(struct drbd_peer_request *peer_req) __releases(local)
{
	unsigned long flags = 0;
	struct drbd_peer_device *peer_device = peer_req->peer_device;
	struct drbd_device *device = peer_device->device;

	spin_lock_irqsave(&device->resource->req_lock, flags);
	device->read_cnt += peer_req->i.size >> 9;
	list_del(&peer_req->w.list);
	if (list_empty(&device->read_ee))
		wake_up(&device->ee_wait);
	if (test_bit(__EE_WAS_ERROR, &peer_req->flags))
		__drbd_chk_io_error(device, DRBD_READ_ERROR);
	spin_unlock_irqrestore(&device->resource->req_lock, flags);

	drbd_queue_work(&peer_device->connection->sender_work, &peer_req->w);
	put_ldev(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* writes on behalf of the partner, or resync writes,
 * "submitted" by the receiver, final stage.  */
<<<<<<< HEAD
static void drbd_endio_write_sec_final(struct drbd_epoch_entry *e) __releases(local)
{
	unsigned long flags = 0;
	struct drbd_conf *mdev = e->mdev;
	sector_t e_sector;
	int do_wake;
	int is_syncer_req;
	int do_al_complete_io;

	D_ASSERT(e->block_id != ID_VACANT);

	/* after we moved e to done_ee,
	 * we may no longer access it,
	 * it may be freed/reused already!
	 * (as soon as we release the req_lock) */
	e_sector = e->sector;
	do_al_complete_io = e->flags & EE_CALL_AL_COMPLETE_IO;
	is_syncer_req = is_syncer_block_id(e->block_id);

	spin_lock_irqsave(&mdev->req_lock, flags);
	mdev->writ_cnt += e->size >> 9;
	list_del(&e->w.list); /* has been on active_ee or sync_ee */
	list_add_tail(&e->w.list, &mdev->done_ee);

	/* No hlist_del_init(&e->collision) here, we did not send the Ack yet,
	 * neither did we wake possibly waiting conflicting requests.
	 * done from "drbd_process_done_ee" within the appropriate w.cb
	 * (e_end_block/e_end_resync_block) or from _drbd_clear_done_ee */

	do_wake = is_syncer_req
		? list_empty(&mdev->sync_ee)
		: list_empty(&mdev->active_ee);

	if (test_bit(__EE_WAS_ERROR, &e->flags))
		__drbd_chk_io_error(mdev, false);
	spin_unlock_irqrestore(&mdev->req_lock, flags);

	if (is_syncer_req)
		drbd_rs_complete_io(mdev, e_sector);

	if (do_wake)
		wake_up(&mdev->ee_wait);

	if (do_al_complete_io)
		drbd_al_complete_io(mdev, e_sector);

	wake_asender(mdev);
	put_ldev(mdev);
=======
void drbd_endio_write_sec_final(struct drbd_peer_request *peer_req) __releases(local)
{
	unsigned long flags = 0;
	struct drbd_peer_device *peer_device = peer_req->peer_device;
	struct drbd_device *device = peer_device->device;
	struct drbd_connection *connection = peer_device->connection;
	struct drbd_interval i;
	int do_wake;
	u64 block_id;
	int do_al_complete_io;

	/* after we moved peer_req to done_ee,
	 * we may no longer access it,
	 * it may be freed/reused already!
	 * (as soon as we release the req_lock) */
	i = peer_req->i;
	do_al_complete_io = peer_req->flags & EE_CALL_AL_COMPLETE_IO;
	block_id = peer_req->block_id;
	peer_req->flags &= ~EE_CALL_AL_COMPLETE_IO;

	if (peer_req->flags & EE_WAS_ERROR) {
		/* In protocol != C, we usually do not send write acks.
		 * In case of a write error, send the neg ack anyways. */
		if (!__test_and_set_bit(__EE_SEND_WRITE_ACK, &peer_req->flags))
			inc_unacked(device);
		drbd_set_out_of_sync(peer_device, peer_req->i.sector, peer_req->i.size);
	}

	spin_lock_irqsave(&device->resource->req_lock, flags);
	device->writ_cnt += peer_req->i.size >> 9;
	list_move_tail(&peer_req->w.list, &device->done_ee);

	/*
	 * Do not remove from the write_requests tree here: we did not send the
	 * Ack yet and did not wake possibly waiting conflicting requests.
	 * Removed from the tree from "drbd_process_done_ee" within the
	 * appropriate dw.cb (e_end_block/e_end_resync_block) or from
	 * _drbd_clear_done_ee.
	 */

	do_wake = list_empty(block_id == ID_SYNCER ? &device->sync_ee : &device->active_ee);

	/* FIXME do we want to detach for failed REQ_OP_DISCARD?
	 * ((peer_req->flags & (EE_WAS_ERROR|EE_TRIM)) == EE_WAS_ERROR) */
	if (peer_req->flags & EE_WAS_ERROR)
		__drbd_chk_io_error(device, DRBD_WRITE_ERROR);

	if (connection->cstate >= C_WF_REPORT_PARAMS) {
		kref_get(&device->kref); /* put is in drbd_send_acks_wf() */
		if (!queue_work(connection->ack_sender, &peer_device->send_acks_work))
			kref_put(&device->kref, drbd_destroy_device);
	}
	spin_unlock_irqrestore(&device->resource->req_lock, flags);

	if (block_id == ID_SYNCER)
		drbd_rs_complete_io(device, i.sector);

	if (do_wake)
		wake_up(&device->ee_wait);

	if (do_al_complete_io)
		drbd_al_complete_io(device, &i);

	put_ldev(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* writes on behalf of the partner, or resync writes,
 * "submitted" by the receiver.
 */
<<<<<<< HEAD
void drbd_endio_sec(struct bio *bio, int error)
{
	struct drbd_epoch_entry *e = bio->bi_private;
	struct drbd_conf *mdev = e->mdev;
	int uptodate = bio_flagged(bio, BIO_UPTODATE);
	int is_write = bio_data_dir(bio) == WRITE;

	if (error && __ratelimit(&drbd_ratelimit_state))
		dev_warn(DEV, "%s: error=%d s=%llus\n",
				is_write ? "write" : "read", error,
				(unsigned long long)e->sector);
	if (!error && !uptodate) {
		if (__ratelimit(&drbd_ratelimit_state))
			dev_warn(DEV, "%s: setting error to -EIO s=%llus\n",
					is_write ? "write" : "read",
					(unsigned long long)e->sector);
		/* strange behavior of some lower level drivers...
		 * fail the request by clearing the uptodate flag,
		 * but do not return any error?! */
		error = -EIO;
	}

	if (error)
		set_bit(__EE_WAS_ERROR, &e->flags);

	bio_put(bio); /* no need for the bio anymore */
	if (atomic_dec_and_test(&e->pending_bios)) {
		if (is_write)
			drbd_endio_write_sec_final(e);
		else
			drbd_endio_read_sec_final(e);
	}
}

/* read, readA or write requests on R_PRIMARY coming from drbd_make_request
 */
void drbd_endio_pri(struct bio *bio, int error)
{
	unsigned long flags;
	struct drbd_request *req = bio->bi_private;
	struct drbd_conf *mdev = req->mdev;
	struct bio_and_error m;
	enum drbd_req_event what;
	int uptodate = bio_flagged(bio, BIO_UPTODATE);

	if (!error && !uptodate) {
		dev_warn(DEV, "p %s: setting error to -EIO\n",
			 bio_data_dir(bio) == WRITE ? "write" : "read");
		/* strange behavior of some lower level drivers...
		 * fail the request by clearing the uptodate flag,
		 * but do not return any error?! */
		error = -EIO;
	}

	/* to avoid recursion in __req_mod */
	if (unlikely(error)) {
		what = (bio_data_dir(bio) == WRITE)
			? write_completed_with_error
			: (bio_rw(bio) == READ)
			  ? read_completed_with_error
			  : read_ahead_completed_with_error;
	} else
		what = completed_ok;

	bio_put(req->private_bio);
	req->private_bio = ERR_PTR(error);

	/* not req_mod(), we need irqsave here! */
	spin_lock_irqsave(&mdev->req_lock, flags);
	__req_mod(req, what, &m);
	spin_unlock_irqrestore(&mdev->req_lock, flags);

	if (m.bio)
		complete_master_bio(mdev, &m);
}

int w_read_retry_remote(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);

	/* We should not detach for read io-error,
	 * but try to WRITE the P_DATA_REPLY to the failed location,
	 * to give the disk the chance to relocate that block */

	spin_lock_irq(&mdev->req_lock);
	if (cancel || mdev->state.pdsk != D_UP_TO_DATE) {
		_req_mod(req, read_retry_remote_canceled);
		spin_unlock_irq(&mdev->req_lock);
		return 1;
	}
	spin_unlock_irq(&mdev->req_lock);

	return w_send_read_req(mdev, w, 0);
}

void drbd_csum_ee(struct drbd_conf *mdev, struct crypto_hash *tfm, struct drbd_epoch_entry *e, void *digest)
{
	struct hash_desc desc;
	struct scatterlist sg;
	struct page *page = e->pages;
	struct page *tmp;
	unsigned len;

	desc.tfm = tfm;
	desc.flags = 0;

	sg_init_table(&sg, 1);
	crypto_hash_init(&desc);

	while ((tmp = page_chain_next(page))) {
		/* all but the last page will be fully used */
		sg_set_page(&sg, page, PAGE_SIZE, 0);
		crypto_hash_update(&desc, &sg, sg.length);
		page = tmp;
	}
	/* and now the last, possibly only partially used page */
	len = e->size & (PAGE_SIZE - 1);
	sg_set_page(&sg, page, len ?: PAGE_SIZE, 0);
	crypto_hash_update(&desc, &sg, sg.length);
	crypto_hash_final(&desc, digest);
}

void drbd_csum_bio(struct drbd_conf *mdev, struct crypto_hash *tfm, struct bio *bio, void *digest)
{
	struct hash_desc desc;
	struct scatterlist sg;
	struct bio_vec *bvec;
	int i;

	desc.tfm = tfm;
	desc.flags = 0;

	sg_init_table(&sg, 1);
	crypto_hash_init(&desc);

	__bio_for_each_segment(bvec, bio, i, 0) {
		sg_set_page(&sg, bvec->bv_page, bvec->bv_len, bvec->bv_offset);
		crypto_hash_update(&desc, &sg, sg.length);
	}
	crypto_hash_final(&desc, digest);
}

/* TODO merge common code with w_e_end_ov_req */
int w_e_send_csum(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	int digest_size;
	void *digest;
	int ok = 1;

	D_ASSERT(e->block_id == DRBD_MAGIC + 0xbeef);
=======
void drbd_peer_request_endio(struct bio *bio)
{
	struct drbd_peer_request *peer_req = bio->bi_private;
	struct drbd_device *device = peer_req->peer_device->device;
	bool is_write = bio_data_dir(bio) == WRITE;
	bool is_discard = bio_op(bio) == REQ_OP_WRITE_ZEROES ||
			  bio_op(bio) == REQ_OP_DISCARD;

	if (bio->bi_status && drbd_ratelimit())
		drbd_warn(device, "%s: error=%d s=%llus\n",
				is_write ? (is_discard ? "discard" : "write")
					: "read", bio->bi_status,
				(unsigned long long)peer_req->i.sector);

	if (bio->bi_status)
		set_bit(__EE_WAS_ERROR, &peer_req->flags);

	bio_put(bio); /* no need for the bio anymore */
	if (atomic_dec_and_test(&peer_req->pending_bios)) {
		if (is_write)
			drbd_endio_write_sec_final(peer_req);
		else
			drbd_endio_read_sec_final(peer_req);
	}
}

static void
drbd_panic_after_delayed_completion_of_aborted_request(struct drbd_device *device)
{
	panic("drbd%u %s/%u potential random memory corruption caused by delayed completion of aborted local request\n",
		device->minor, device->resource->name, device->vnr);
}

/* read, readA or write requests on R_PRIMARY coming from drbd_make_request
 */
void drbd_request_endio(struct bio *bio)
{
	unsigned long flags;
	struct drbd_request *req = bio->bi_private;
	struct drbd_device *device = req->device;
	struct bio_and_error m;
	enum drbd_req_event what;

	/* If this request was aborted locally before,
	 * but now was completed "successfully",
	 * chances are that this caused arbitrary data corruption.
	 *
	 * "aborting" requests, or force-detaching the disk, is intended for
	 * completely blocked/hung local backing devices which do no longer
	 * complete requests at all, not even do error completions.  In this
	 * situation, usually a hard-reset and failover is the only way out.
	 *
	 * By "aborting", basically faking a local error-completion,
	 * we allow for a more graceful swichover by cleanly migrating services.
	 * Still the affected node has to be rebooted "soon".
	 *
	 * By completing these requests, we allow the upper layers to re-use
	 * the associated data pages.
	 *
	 * If later the local backing device "recovers", and now DMAs some data
	 * from disk into the original request pages, in the best case it will
	 * just put random data into unused pages; but typically it will corrupt
	 * meanwhile completely unrelated data, causing all sorts of damage.
	 *
	 * Which means delayed successful completion,
	 * especially for READ requests,
	 * is a reason to panic().
	 *
	 * We assume that a delayed *error* completion is OK,
	 * though we still will complain noisily about it.
	 */
	if (unlikely(req->rq_state & RQ_LOCAL_ABORTED)) {
		if (drbd_ratelimit())
			drbd_emerg(device, "delayed completion of aborted local request; disk-timeout may be too aggressive\n");

		if (!bio->bi_status)
			drbd_panic_after_delayed_completion_of_aborted_request(device);
	}

	/* to avoid recursion in __req_mod */
	if (unlikely(bio->bi_status)) {
		switch (bio_op(bio)) {
		case REQ_OP_WRITE_ZEROES:
		case REQ_OP_DISCARD:
			if (bio->bi_status == BLK_STS_NOTSUPP)
				what = DISCARD_COMPLETED_NOTSUPP;
			else
				what = DISCARD_COMPLETED_WITH_ERROR;
			break;
		case REQ_OP_READ:
			if (bio->bi_opf & REQ_RAHEAD)
				what = READ_AHEAD_COMPLETED_WITH_ERROR;
			else
				what = READ_COMPLETED_WITH_ERROR;
			break;
		default:
			what = WRITE_COMPLETED_WITH_ERROR;
			break;
		}
	} else {
		what = COMPLETED_OK;
	}

	req->private_bio = ERR_PTR(blk_status_to_errno(bio->bi_status));
	bio_put(bio);

	/* not req_mod(), we need irqsave here! */
	spin_lock_irqsave(&device->resource->req_lock, flags);
	__req_mod(req, what, NULL, &m);
	spin_unlock_irqrestore(&device->resource->req_lock, flags);
	put_ldev(device);

	if (m.bio)
		complete_master_bio(device, &m);
}

void drbd_csum_ee(struct crypto_shash *tfm, struct drbd_peer_request *peer_req, void *digest)
{
	SHASH_DESC_ON_STACK(desc, tfm);
	struct page *page = peer_req->pages;
	struct page *tmp;
	unsigned len;
	void *src;

	desc->tfm = tfm;

	crypto_shash_init(desc);

	src = kmap_atomic(page);
	while ((tmp = page_chain_next(page))) {
		/* all but the last page will be fully used */
		crypto_shash_update(desc, src, PAGE_SIZE);
		kunmap_atomic(src);
		page = tmp;
		src = kmap_atomic(page);
	}
	/* and now the last, possibly only partially used page */
	len = peer_req->i.size & (PAGE_SIZE - 1);
	crypto_shash_update(desc, src, len ?: PAGE_SIZE);
	kunmap_atomic(src);

	crypto_shash_final(desc, digest);
	shash_desc_zero(desc);
}

void drbd_csum_bio(struct crypto_shash *tfm, struct bio *bio, void *digest)
{
	SHASH_DESC_ON_STACK(desc, tfm);
	struct bio_vec bvec;
	struct bvec_iter iter;

	desc->tfm = tfm;

	crypto_shash_init(desc);

	bio_for_each_segment(bvec, bio, iter) {
		u8 *src;

		src = bvec_kmap_local(&bvec);
		crypto_shash_update(desc, src, bvec.bv_len);
		kunmap_local(src);
	}
	crypto_shash_final(desc, digest);
	shash_desc_zero(desc);
}

/* MAYBE merge common code with w_e_end_ov_req */
static int w_e_send_csum(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_peer_device *peer_device = peer_req->peer_device;
	struct drbd_device *device = peer_device->device;
	int digest_size;
	void *digest;
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(cancel))
		goto out;

<<<<<<< HEAD
	if (likely((e->flags & EE_WAS_ERROR) != 0))
		goto out;

	digest_size = crypto_hash_digestsize(mdev->csums_tfm);
	digest = kmalloc(digest_size, GFP_NOIO);
	if (digest) {
		sector_t sector = e->sector;
		unsigned int size = e->size;
		drbd_csum_ee(mdev, mdev->csums_tfm, e, digest);
		/* Free e and pages before send.
		 * In case we block on congestion, we could otherwise run into
		 * some distributed deadlock, if the other side blocks on
		 * congestion as well, because our receiver blocks in
		 * drbd_pp_alloc due to pp_in_use > max_buffers. */
		drbd_free_ee(mdev, e);
		e = NULL;
		inc_rs_pending(mdev);
		ok = drbd_send_drequest_csum(mdev, sector, size,
					     digest, digest_size,
					     P_CSUM_RS_REQUEST);
		kfree(digest);
	} else {
		dev_err(DEV, "kmalloc() of digest failed.\n");
		ok = 0;
	}

out:
	if (e)
		drbd_free_ee(mdev, e);

	if (unlikely(!ok))
		dev_err(DEV, "drbd_send_drequest(..., csum) failed\n");
	return ok;
=======
	if (unlikely((peer_req->flags & EE_WAS_ERROR) != 0))
		goto out;

	digest_size = crypto_shash_digestsize(peer_device->connection->csums_tfm);
	digest = kmalloc(digest_size, GFP_NOIO);
	if (digest) {
		sector_t sector = peer_req->i.sector;
		unsigned int size = peer_req->i.size;
		drbd_csum_ee(peer_device->connection->csums_tfm, peer_req, digest);
		/* Free peer_req and pages before send.
		 * In case we block on congestion, we could otherwise run into
		 * some distributed deadlock, if the other side blocks on
		 * congestion as well, because our receiver blocks in
		 * drbd_alloc_pages due to pp_in_use > max_buffers. */
		drbd_free_peer_req(device, peer_req);
		peer_req = NULL;
		inc_rs_pending(peer_device);
		err = drbd_send_drequest_csum(peer_device, sector, size,
					      digest, digest_size,
					      P_CSUM_RS_REQUEST);
		kfree(digest);
	} else {
		drbd_err(device, "kmalloc() of digest failed.\n");
		err = -ENOMEM;
	}

out:
	if (peer_req)
		drbd_free_peer_req(device, peer_req);

	if (unlikely(err))
		drbd_err(device, "drbd_send_drequest(..., csum) failed\n");
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define GFP_TRY	(__GFP_HIGHMEM | __GFP_NOWARN)

<<<<<<< HEAD
static int read_for_csum(struct drbd_conf *mdev, sector_t sector, int size)
{
	struct drbd_epoch_entry *e;

	if (!get_ldev(mdev))
		return -EIO;

	if (drbd_rs_should_slow_down(mdev, sector))
		goto defer;

	/* GFP_TRY, because if there is no memory available right now, this may
	 * be rescheduled for later. It is "only" background resync, after all. */
	e = drbd_alloc_ee(mdev, DRBD_MAGIC+0xbeef, sector, size, GFP_TRY);
	if (!e)
		goto defer;

	e->w.cb = w_e_send_csum;
	spin_lock_irq(&mdev->req_lock);
	list_add(&e->w.list, &mdev->read_ee);
	spin_unlock_irq(&mdev->req_lock);

	atomic_add(size >> 9, &mdev->rs_sect_ev);
	if (drbd_submit_ee(mdev, e, READ, DRBD_FAULT_RS_RD) == 0)
=======
static int read_for_csum(struct drbd_peer_device *peer_device, sector_t sector, int size)
{
	struct drbd_device *device = peer_device->device;
	struct drbd_peer_request *peer_req;

	if (!get_ldev(device))
		return -EIO;

	/* GFP_TRY, because if there is no memory available right now, this may
	 * be rescheduled for later. It is "only" background resync, after all. */
	peer_req = drbd_alloc_peer_req(peer_device, ID_SYNCER /* unused */, sector,
				       size, size, GFP_TRY);
	if (!peer_req)
		goto defer;

	peer_req->w.cb = w_e_send_csum;
	peer_req->opf = REQ_OP_READ;
	spin_lock_irq(&device->resource->req_lock);
	list_add_tail(&peer_req->w.list, &device->read_ee);
	spin_unlock_irq(&device->resource->req_lock);

	atomic_add(size >> 9, &device->rs_sect_ev);
	if (drbd_submit_peer_request(peer_req) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	/* If it failed because of ENOMEM, retry should help.  If it failed
	 * because bio_add_page failed (probably broken lower level driver),
	 * retry may or may not help.
	 * If it does not, you may need to force disconnect. */
<<<<<<< HEAD
	spin_lock_irq(&mdev->req_lock);
	list_del(&e->w.list);
	spin_unlock_irq(&mdev->req_lock);

	drbd_free_ee(mdev, e);
defer:
	put_ldev(mdev);
	return -EAGAIN;
}

int w_resync_timer(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	switch (mdev->state.conn) {
	case C_VERIFY_S:
		w_make_ov_request(mdev, w, cancel);
		break;
	case C_SYNC_TARGET:
		w_make_resync_request(mdev, w, cancel);
		break;
	}

	return 1;
}

void resync_timer_fn(unsigned long data)
{
	struct drbd_conf *mdev = (struct drbd_conf *) data;

	if (list_empty(&mdev->resync_work.list))
		drbd_queue_work(&mdev->data.work, &mdev->resync_work);
=======
	spin_lock_irq(&device->resource->req_lock);
	list_del(&peer_req->w.list);
	spin_unlock_irq(&device->resource->req_lock);

	drbd_free_peer_req(device, peer_req);
defer:
	put_ldev(device);
	return -EAGAIN;
}

int w_resync_timer(struct drbd_work *w, int cancel)
{
	struct drbd_device *device =
		container_of(w, struct drbd_device, resync_work);

	switch (device->state.conn) {
	case C_VERIFY_S:
		make_ov_request(first_peer_device(device), cancel);
		break;
	case C_SYNC_TARGET:
		make_resync_request(first_peer_device(device), cancel);
		break;
	}

	return 0;
}

void resync_timer_fn(struct timer_list *t)
{
	struct drbd_device *device = from_timer(device, t, resync_timer);

	drbd_queue_work_if_unqueued(
		&first_peer_device(device)->connection->sender_work,
		&device->resync_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void fifo_set(struct fifo_buffer *fb, int value)
{
	int i;

	for (i = 0; i < fb->size; i++)
		fb->values[i] = value;
}

static int fifo_push(struct fifo_buffer *fb, int value)
{
	int ov;

	ov = fb->values[fb->head_index];
	fb->values[fb->head_index++] = value;

	if (fb->head_index >= fb->size)
		fb->head_index = 0;

	return ov;
}

static void fifo_add_val(struct fifo_buffer *fb, int value)
{
	int i;

	for (i = 0; i < fb->size; i++)
		fb->values[i] += value;
}

<<<<<<< HEAD
static int drbd_rs_controller(struct drbd_conf *mdev)
{
	unsigned int sect_in;  /* Number of sectors that came in since the last turn */
	unsigned int want;     /* The number of sectors we want in the proxy */
	int req_sect; /* Number of sectors to request in this turn */
	int correction; /* Number of sectors more we need in the proxy*/
=======
struct fifo_buffer *fifo_alloc(unsigned int fifo_size)
{
	struct fifo_buffer *fb;

	fb = kzalloc(struct_size(fb, values, fifo_size), GFP_NOIO);
	if (!fb)
		return NULL;

	fb->head_index = 0;
	fb->size = fifo_size;
	fb->total = 0;

	return fb;
}

static int drbd_rs_controller(struct drbd_peer_device *peer_device, unsigned int sect_in)
{
	struct drbd_device *device = peer_device->device;
	struct disk_conf *dc;
	unsigned int want;     /* The number of sectors we want in-flight */
	int req_sect; /* Number of sectors to request in this turn */
	int correction; /* Number of sectors more we need in-flight */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int cps; /* correction per invocation of drbd_rs_controller() */
	int steps; /* Number of time steps to plan ahead */
	int curr_corr;
	int max_sect;
<<<<<<< HEAD

	sect_in = atomic_xchg(&mdev->rs_sect_in, 0); /* Number of sectors that came in */
	mdev->rs_in_flight -= sect_in;

	spin_lock(&mdev->peer_seq_lock); /* get an atomic view on mdev->rs_plan_s */

	steps = mdev->rs_plan_s.size; /* (mdev->sync_conf.c_plan_ahead * 10 * SLEEP_TIME) / HZ; */

	if (mdev->rs_in_flight + sect_in == 0) { /* At start of resync */
		want = ((mdev->sync_conf.rate * 2 * SLEEP_TIME) / HZ) * steps;
	} else { /* normal path */
		want = mdev->sync_conf.c_fill_target ? mdev->sync_conf.c_fill_target :
			sect_in * mdev->sync_conf.c_delay_target * HZ / (SLEEP_TIME * 10);
	}

	correction = want - mdev->rs_in_flight - mdev->rs_planed;

	/* Plan ahead */
	cps = correction / steps;
	fifo_add_val(&mdev->rs_plan_s, cps);
	mdev->rs_planed += cps * steps;

	/* What we do in this step */
	curr_corr = fifo_push(&mdev->rs_plan_s, 0);
	spin_unlock(&mdev->peer_seq_lock);
	mdev->rs_planed -= curr_corr;
=======
	struct fifo_buffer *plan;

	dc = rcu_dereference(device->ldev->disk_conf);
	plan = rcu_dereference(device->rs_plan_s);

	steps = plan->size; /* (dc->c_plan_ahead * 10 * SLEEP_TIME) / HZ; */

	if (device->rs_in_flight + sect_in == 0) { /* At start of resync */
		want = ((dc->resync_rate * 2 * SLEEP_TIME) / HZ) * steps;
	} else { /* normal path */
		want = dc->c_fill_target ? dc->c_fill_target :
			sect_in * dc->c_delay_target * HZ / (SLEEP_TIME * 10);
	}

	correction = want - device->rs_in_flight - plan->total;

	/* Plan ahead */
	cps = correction / steps;
	fifo_add_val(plan, cps);
	plan->total += cps * steps;

	/* What we do in this step */
	curr_corr = fifo_push(plan, 0);
	plan->total -= curr_corr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	req_sect = sect_in + curr_corr;
	if (req_sect < 0)
		req_sect = 0;

<<<<<<< HEAD
	max_sect = (mdev->sync_conf.c_max_rate * 2 * SLEEP_TIME) / HZ;
=======
	max_sect = (dc->c_max_rate * 2 * SLEEP_TIME) / HZ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (req_sect > max_sect)
		req_sect = max_sect;

	/*
<<<<<<< HEAD
	dev_warn(DEV, "si=%u if=%d wa=%u co=%d st=%d cps=%d pl=%d cc=%d rs=%d\n",
		 sect_in, mdev->rs_in_flight, want, correction,
		 steps, cps, mdev->rs_planed, curr_corr, req_sect);
=======
	drbd_warn(device, "si=%u if=%d wa=%u co=%d st=%d cps=%d pl=%d cc=%d rs=%d\n",
		 sect_in, device->rs_in_flight, want, correction,
		 steps, cps, device->rs_planed, curr_corr, req_sect);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*/

	return req_sect;
}

<<<<<<< HEAD
static int drbd_rs_number_requests(struct drbd_conf *mdev)
{
	int number;
	if (mdev->rs_plan_s.size) { /* mdev->sync_conf.c_plan_ahead */
		number = drbd_rs_controller(mdev) >> (BM_BLOCK_SHIFT - 9);
		mdev->c_sync_rate = number * HZ * (BM_BLOCK_SIZE / 1024) / SLEEP_TIME;
	} else {
		mdev->c_sync_rate = mdev->sync_conf.rate;
		number = SLEEP_TIME * mdev->c_sync_rate  / ((BM_BLOCK_SIZE / 1024) * HZ);
	}

	/* ignore the amount of pending requests, the resync controller should
	 * throttle down to incoming reply rate soon enough anyways. */
	return number;
}

static int w_make_resync_request(struct drbd_conf *mdev,
				 struct drbd_work *w, int cancel)
{
	unsigned long bit;
	sector_t sector;
	const sector_t capacity = drbd_get_capacity(mdev->this_bdev);
	int max_bio_size;
	int number, rollback_i, size;
	int align, queued, sndbuf;
	int i = 0;

	if (unlikely(cancel))
		return 1;

	if (mdev->rs_total == 0) {
		/* empty resync? */
		drbd_resync_finished(mdev);
		return 1;
	}

	if (!get_ldev(mdev)) {
		/* Since we only need to access mdev->rsync a
		   get_ldev_if_state(mdev,D_FAILED) would be sufficient, but
		   to continue resync with a broken disk makes no sense at
		   all */
		dev_err(DEV, "Disk broke down during resync!\n");
		return 1;
	}

	max_bio_size = queue_max_hw_sectors(mdev->rq_queue) << 9;
	number = drbd_rs_number_requests(mdev);
	if (number == 0)
		goto requeue;

	for (i = 0; i < number; i++) {
		/* Stop generating RS requests, when half of the send buffer is filled */
		mutex_lock(&mdev->data.mutex);
		if (mdev->data.socket) {
			queued = mdev->data.socket->sk->sk_wmem_queued;
			sndbuf = mdev->data.socket->sk->sk_sndbuf;
		} else {
			queued = 1;
			sndbuf = 0;
		}
		mutex_unlock(&mdev->data.mutex);
		if (queued > sndbuf / 2)
=======
static int drbd_rs_number_requests(struct drbd_peer_device *peer_device)
{
	struct drbd_device *device = peer_device->device;
	unsigned int sect_in;  /* Number of sectors that came in since the last turn */
	int number, mxb;

	sect_in = atomic_xchg(&device->rs_sect_in, 0);
	device->rs_in_flight -= sect_in;

	rcu_read_lock();
	mxb = drbd_get_max_buffers(device) / 2;
	if (rcu_dereference(device->rs_plan_s)->size) {
		number = drbd_rs_controller(peer_device, sect_in) >> (BM_BLOCK_SHIFT - 9);
		device->c_sync_rate = number * HZ * (BM_BLOCK_SIZE / 1024) / SLEEP_TIME;
	} else {
		device->c_sync_rate = rcu_dereference(device->ldev->disk_conf)->resync_rate;
		number = SLEEP_TIME * device->c_sync_rate  / ((BM_BLOCK_SIZE / 1024) * HZ);
	}
	rcu_read_unlock();

	/* Don't have more than "max-buffers"/2 in-flight.
	 * Otherwise we may cause the remote site to stall on drbd_alloc_pages(),
	 * potentially causing a distributed deadlock on congestion during
	 * online-verify or (checksum-based) resync, if max-buffers,
	 * socket buffer sizes and resync rate settings are mis-configured. */

	/* note that "number" is in units of "BM_BLOCK_SIZE" (which is 4k),
	 * mxb (as used here, and in drbd_alloc_pages on the peer) is
	 * "number of pages" (typically also 4k),
	 * but "rs_in_flight" is in "sectors" (512 Byte). */
	if (mxb - device->rs_in_flight/8 < number)
		number = mxb - device->rs_in_flight/8;

	return number;
}

static int make_resync_request(struct drbd_peer_device *const peer_device, int cancel)
{
	struct drbd_device *const device = peer_device->device;
	struct drbd_connection *const connection = peer_device ? peer_device->connection : NULL;
	unsigned long bit;
	sector_t sector;
	const sector_t capacity = get_capacity(device->vdisk);
	int max_bio_size;
	int number, rollback_i, size;
	int align, requeue = 0;
	int i = 0;
	int discard_granularity = 0;

	if (unlikely(cancel))
		return 0;

	if (device->rs_total == 0) {
		/* empty resync? */
		drbd_resync_finished(peer_device);
		return 0;
	}

	if (!get_ldev(device)) {
		/* Since we only need to access device->rsync a
		   get_ldev_if_state(device,D_FAILED) would be sufficient, but
		   to continue resync with a broken disk makes no sense at
		   all */
		drbd_err(device, "Disk broke down during resync!\n");
		return 0;
	}

	if (connection->agreed_features & DRBD_FF_THIN_RESYNC) {
		rcu_read_lock();
		discard_granularity = rcu_dereference(device->ldev->disk_conf)->rs_discard_granularity;
		rcu_read_unlock();
	}

	max_bio_size = queue_max_hw_sectors(device->rq_queue) << 9;
	number = drbd_rs_number_requests(peer_device);
	if (number <= 0)
		goto requeue;

	for (i = 0; i < number; i++) {
		/* Stop generating RS requests when half of the send buffer is filled,
		 * but notify TCP that we'd like to have more space. */
		mutex_lock(&connection->data.mutex);
		if (connection->data.socket) {
			struct sock *sk = connection->data.socket->sk;
			int queued = sk->sk_wmem_queued;
			int sndbuf = sk->sk_sndbuf;
			if (queued > sndbuf / 2) {
				requeue = 1;
				if (sk->sk_socket)
					set_bit(SOCK_NOSPACE, &sk->sk_socket->flags);
			}
		} else
			requeue = 1;
		mutex_unlock(&connection->data.mutex);
		if (requeue)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto requeue;

next_sector:
		size = BM_BLOCK_SIZE;
<<<<<<< HEAD
		bit  = drbd_bm_find_next(mdev, mdev->bm_resync_fo);

		if (bit == DRBD_END_OF_BITMAP) {
			mdev->bm_resync_fo = drbd_bm_bits(mdev);
			put_ldev(mdev);
			return 1;
=======
		bit  = drbd_bm_find_next(device, device->bm_resync_fo);

		if (bit == DRBD_END_OF_BITMAP) {
			device->bm_resync_fo = drbd_bm_bits(device);
			put_ldev(device);
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		sector = BM_BIT_TO_SECT(bit);

<<<<<<< HEAD
		if (drbd_rs_should_slow_down(mdev, sector) ||
		    drbd_try_rs_begin_io(mdev, sector)) {
			mdev->bm_resync_fo = bit;
			goto requeue;
		}
		mdev->bm_resync_fo = bit + 1;

		if (unlikely(drbd_bm_test_bit(mdev, bit) == 0)) {
			drbd_rs_complete_io(mdev, sector);
=======
		if (drbd_try_rs_begin_io(peer_device, sector)) {
			device->bm_resync_fo = bit;
			goto requeue;
		}
		device->bm_resync_fo = bit + 1;

		if (unlikely(drbd_bm_test_bit(device, bit) == 0)) {
			drbd_rs_complete_io(device, sector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto next_sector;
		}

#if DRBD_MAX_BIO_SIZE > BM_BLOCK_SIZE
		/* try to find some adjacent bits.
		 * we stop if we have already the maximum req size.
		 *
		 * Additionally always align bigger requests, in order to
		 * be prepared for all stripe sizes of software RAIDs.
		 */
		align = 1;
		rollback_i = i;
<<<<<<< HEAD
		for (;;) {
=======
		while (i < number) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (size + BM_BLOCK_SIZE > max_bio_size)
				break;

			/* Be always aligned */
			if (sector & ((1<<(align+3))-1))
				break;

<<<<<<< HEAD
=======
			if (discard_granularity && size == discard_granularity)
				break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* do not cross extent boundaries */
			if (((bit+1) & BM_BLOCKS_PER_BM_EXT_MASK) == 0)
				break;
			/* now, is it actually dirty, after all?
			 * caution, drbd_bm_test_bit is tri-state for some
			 * obscure reason; ( b == 0 ) would get the out-of-band
			 * only accidentally right because of the "oddly sized"
			 * adjustment below */
<<<<<<< HEAD
			if (drbd_bm_test_bit(mdev, bit+1) != 1)
=======
			if (drbd_bm_test_bit(device, bit+1) != 1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			bit++;
			size += BM_BLOCK_SIZE;
			if ((BM_BLOCK_SIZE << align) <= size)
				align++;
			i++;
		}
		/* if we merged some,
		 * reset the offset to start the next drbd_bm_find_next from */
		if (size > BM_BLOCK_SIZE)
<<<<<<< HEAD
			mdev->bm_resync_fo = bit + 1;
=======
			device->bm_resync_fo = bit + 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

		/* adjust very last sectors, in case we are oddly sized */
		if (sector + (size>>9) > capacity)
			size = (capacity-sector)<<9;
<<<<<<< HEAD
		if (mdev->agreed_pro_version >= 89 && mdev->csums_tfm) {
			switch (read_for_csum(mdev, sector, size)) {
			case -EIO: /* Disk failure */
				put_ldev(mdev);
				return 0;
			case -EAGAIN: /* allocation failed, or ldev busy */
				drbd_rs_complete_io(mdev, sector);
				mdev->bm_resync_fo = BM_SECT_TO_BIT(sector);
=======

		if (device->use_csums) {
			switch (read_for_csum(peer_device, sector, size)) {
			case -EIO: /* Disk failure */
				put_ldev(device);
				return -EIO;
			case -EAGAIN: /* allocation failed, or ldev busy */
				drbd_rs_complete_io(device, sector);
				device->bm_resync_fo = BM_SECT_TO_BIT(sector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				i = rollback_i;
				goto requeue;
			case 0:
				/* everything ok */
				break;
			default:
				BUG();
			}
		} else {
<<<<<<< HEAD
			inc_rs_pending(mdev);
			if (!drbd_send_drequest(mdev, P_RS_DATA_REQUEST,
					       sector, size, ID_SYNCER)) {
				dev_err(DEV, "drbd_send_drequest() failed, aborting...\n");
				dec_rs_pending(mdev);
				put_ldev(mdev);
				return 0;
=======
			int err;

			inc_rs_pending(peer_device);
			err = drbd_send_drequest(peer_device,
						 size == discard_granularity ? P_RS_THIN_REQ : P_RS_DATA_REQUEST,
						 sector, size, ID_SYNCER);
			if (err) {
				drbd_err(device, "drbd_send_drequest() failed, aborting...\n");
				dec_rs_pending(peer_device);
				put_ldev(device);
				return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}

<<<<<<< HEAD
	if (mdev->bm_resync_fo >= drbd_bm_bits(mdev)) {
=======
	if (device->bm_resync_fo >= drbd_bm_bits(device)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* last syncer _request_ was sent,
		 * but the P_RS_DATA_REPLY not yet received.  sync will end (and
		 * next sync group will resume), as soon as we receive the last
		 * resync data block, and the last bit is cleared.
		 * until then resync "work" is "inactive" ...
		 */
<<<<<<< HEAD
		put_ldev(mdev);
		return 1;
	}

 requeue:
	mdev->rs_in_flight += (i << (BM_BLOCK_SHIFT - 9));
	mod_timer(&mdev->resync_timer, jiffies + SLEEP_TIME);
	put_ldev(mdev);
	return 1;
}

static int w_make_ov_request(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	int number, i, size;
	sector_t sector;
	const sector_t capacity = drbd_get_capacity(mdev->this_bdev);
=======
		put_ldev(device);
		return 0;
	}

 requeue:
	device->rs_in_flight += (i << (BM_BLOCK_SHIFT - 9));
	mod_timer(&device->resync_timer, jiffies + SLEEP_TIME);
	put_ldev(device);
	return 0;
}

static int make_ov_request(struct drbd_peer_device *peer_device, int cancel)
{
	struct drbd_device *device = peer_device->device;
	int number, i, size;
	sector_t sector;
	const sector_t capacity = get_capacity(device->vdisk);
	bool stop_sector_reached = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(cancel))
		return 1;

<<<<<<< HEAD
	number = drbd_rs_number_requests(mdev);

	sector = mdev->ov_position;
	for (i = 0; i < number; i++) {
		if (sector >= capacity) {
			return 1;
		}

		size = BM_BLOCK_SIZE;

		if (drbd_rs_should_slow_down(mdev, sector) ||
		    drbd_try_rs_begin_io(mdev, sector)) {
			mdev->ov_position = sector;
=======
	number = drbd_rs_number_requests(peer_device);

	sector = device->ov_position;
	for (i = 0; i < number; i++) {
		if (sector >= capacity)
			return 1;

		/* We check for "finished" only in the reply path:
		 * w_e_end_ov_reply().
		 * We need to send at least one request out. */
		stop_sector_reached = i > 0
			&& verify_can_do_stop_sector(device)
			&& sector >= device->ov_stop_sector;
		if (stop_sector_reached)
			break;

		size = BM_BLOCK_SIZE;

		if (drbd_try_rs_begin_io(peer_device, sector)) {
			device->ov_position = sector;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto requeue;
		}

		if (sector + (size>>9) > capacity)
			size = (capacity-sector)<<9;

<<<<<<< HEAD
		inc_rs_pending(mdev);
		if (!drbd_send_ov_request(mdev, sector, size)) {
			dec_rs_pending(mdev);
=======
		inc_rs_pending(peer_device);
		if (drbd_send_ov_request(first_peer_device(device), sector, size)) {
			dec_rs_pending(peer_device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		}
		sector += BM_SECT_PER_BIT;
	}
<<<<<<< HEAD
	mdev->ov_position = sector;

 requeue:
	mdev->rs_in_flight += (i << (BM_BLOCK_SHIFT - 9));
	mod_timer(&mdev->resync_timer, jiffies + SLEEP_TIME);
	return 1;
}


void start_resync_timer_fn(unsigned long data)
{
	struct drbd_conf *mdev = (struct drbd_conf *) data;

	drbd_queue_work(&mdev->data.work, &mdev->start_resync_work);
}

int w_start_resync(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	if (atomic_read(&mdev->unacked_cnt) || atomic_read(&mdev->rs_pending_cnt)) {
		dev_warn(DEV, "w_start_resync later...\n");
		mdev->start_resync_timer.expires = jiffies + HZ/10;
		add_timer(&mdev->start_resync_timer);
		return 1;
	}

	drbd_start_resync(mdev, C_SYNC_SOURCE);
	clear_bit(AHEAD_TO_SYNC_SOURCE, &mdev->current_epoch->flags);
	return 1;
}

int w_ov_finished(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	kfree(w);
	ov_oos_print(mdev);
	drbd_resync_finished(mdev);

	return 1;
}

static int w_resync_finished(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	kfree(w);

	drbd_resync_finished(mdev);

	return 1;
}

static void ping_peer(struct drbd_conf *mdev)
{
	clear_bit(GOT_PING_ACK, &mdev->flags);
	request_ping(mdev);
	wait_event(mdev->misc_wait,
		   test_bit(GOT_PING_ACK, &mdev->flags) || mdev->state.conn < C_CONNECTED);
}

int drbd_resync_finished(struct drbd_conf *mdev)
{
	unsigned long db, dt, dbdt;
	unsigned long n_oos;
	union drbd_state os, ns;
	struct drbd_work *w;
=======
	device->ov_position = sector;

 requeue:
	device->rs_in_flight += (i << (BM_BLOCK_SHIFT - 9));
	if (i == 0 || !stop_sector_reached)
		mod_timer(&device->resync_timer, jiffies + SLEEP_TIME);
	return 1;
}

int w_ov_finished(struct drbd_work *w, int cancel)
{
	struct drbd_device_work *dw =
		container_of(w, struct drbd_device_work, w);
	struct drbd_device *device = dw->device;
	kfree(dw);
	ov_out_of_sync_print(first_peer_device(device));
	drbd_resync_finished(first_peer_device(device));

	return 0;
}

static int w_resync_finished(struct drbd_work *w, int cancel)
{
	struct drbd_device_work *dw =
		container_of(w, struct drbd_device_work, w);
	struct drbd_device *device = dw->device;
	kfree(dw);

	drbd_resync_finished(first_peer_device(device));

	return 0;
}

static void ping_peer(struct drbd_device *device)
{
	struct drbd_connection *connection = first_peer_device(device)->connection;

	clear_bit(GOT_PING_ACK, &connection->flags);
	request_ping(connection);
	wait_event(connection->ping_wait,
		   test_bit(GOT_PING_ACK, &connection->flags) || device->state.conn < C_CONNECTED);
}

int drbd_resync_finished(struct drbd_peer_device *peer_device)
{
	struct drbd_device *device = peer_device->device;
	struct drbd_connection *connection = peer_device->connection;
	unsigned long db, dt, dbdt;
	unsigned long n_oos;
	union drbd_state os, ns;
	struct drbd_device_work *dw;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *khelper_cmd = NULL;
	int verify_done = 0;

	/* Remove all elements from the resync LRU. Since future actions
	 * might set bits in the (main) bitmap, then the entries in the
	 * resync LRU would be wrong. */
<<<<<<< HEAD
	if (drbd_rs_del_all(mdev)) {
=======
	if (drbd_rs_del_all(device)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* In case this is not possible now, most probably because
		 * there are P_RS_DATA_REPLY Packets lingering on the worker's
		 * queue (or even the read operations for those packets
		 * is not finished by now).   Retry in 100ms. */

		schedule_timeout_interruptible(HZ / 10);
<<<<<<< HEAD
		w = kmalloc(sizeof(struct drbd_work), GFP_ATOMIC);
		if (w) {
			w->cb = w_resync_finished;
			drbd_queue_work(&mdev->data.work, w);
			return 1;
		}
		dev_err(DEV, "Warn failed to drbd_rs_del_all() and to kmalloc(w).\n");
	}

	dt = (jiffies - mdev->rs_start - mdev->rs_paused) / HZ;
	if (dt <= 0)
		dt = 1;
	db = mdev->rs_total;
	dbdt = Bit2KB(db/dt);
	mdev->rs_paused /= HZ;

	if (!get_ldev(mdev))
		goto out;

	ping_peer(mdev);

	spin_lock_irq(&mdev->req_lock);
	os = mdev->state;
=======
		dw = kmalloc(sizeof(struct drbd_device_work), GFP_ATOMIC);
		if (dw) {
			dw->w.cb = w_resync_finished;
			dw->device = device;
			drbd_queue_work(&connection->sender_work, &dw->w);
			return 1;
		}
		drbd_err(device, "Warn failed to drbd_rs_del_all() and to kmalloc(dw).\n");
	}

	dt = (jiffies - device->rs_start - device->rs_paused) / HZ;
	if (dt <= 0)
		dt = 1;

	db = device->rs_total;
	/* adjust for verify start and stop sectors, respective reached position */
	if (device->state.conn == C_VERIFY_S || device->state.conn == C_VERIFY_T)
		db -= device->ov_left;

	dbdt = Bit2KB(db/dt);
	device->rs_paused /= HZ;

	if (!get_ldev(device))
		goto out;

	ping_peer(device);

	spin_lock_irq(&device->resource->req_lock);
	os = drbd_read_state(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	verify_done = (os.conn == C_VERIFY_S || os.conn == C_VERIFY_T);

	/* This protects us against multiple calls (that can happen in the presence
	   of application IO), and against connectivity loss just before we arrive here. */
	if (os.conn <= C_CONNECTED)
		goto out_unlock;

	ns = os;
	ns.conn = C_CONNECTED;

<<<<<<< HEAD
	dev_info(DEV, "%s done (total %lu sec; paused %lu sec; %lu K/sec)\n",
	     verify_done ? "Online verify " : "Resync",
	     dt + mdev->rs_paused, mdev->rs_paused, dbdt);

	n_oos = drbd_bm_total_weight(mdev);

	if (os.conn == C_VERIFY_S || os.conn == C_VERIFY_T) {
		if (n_oos) {
			dev_alert(DEV, "Online verify found %lu %dk block out of sync!\n",
=======
	drbd_info(device, "%s done (total %lu sec; paused %lu sec; %lu K/sec)\n",
	     verify_done ? "Online verify" : "Resync",
	     dt + device->rs_paused, device->rs_paused, dbdt);

	n_oos = drbd_bm_total_weight(device);

	if (os.conn == C_VERIFY_S || os.conn == C_VERIFY_T) {
		if (n_oos) {
			drbd_alert(device, "Online verify found %lu %dk block out of sync!\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			      n_oos, Bit2KB(1));
			khelper_cmd = "out-of-sync";
		}
	} else {
<<<<<<< HEAD
		D_ASSERT((n_oos - mdev->rs_failed) == 0);
=======
		D_ASSERT(device, (n_oos - device->rs_failed) == 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (os.conn == C_SYNC_TARGET || os.conn == C_PAUSED_SYNC_T)
			khelper_cmd = "after-resync-target";

<<<<<<< HEAD
		if (mdev->csums_tfm && mdev->rs_total) {
			const unsigned long s = mdev->rs_same_csum;
			const unsigned long t = mdev->rs_total;
			const int ratio =
				(t == 0)     ? 0 :
			(t < 100000) ? ((s*100)/t) : (s/(t/100));
			dev_info(DEV, "%u %% had equal checksums, eliminated: %luK; "
			     "transferred %luK total %luK\n",
			     ratio,
			     Bit2KB(mdev->rs_same_csum),
			     Bit2KB(mdev->rs_total - mdev->rs_same_csum),
			     Bit2KB(mdev->rs_total));
		}
	}

	if (mdev->rs_failed) {
		dev_info(DEV, "            %lu failed blocks\n", mdev->rs_failed);
=======
		if (device->use_csums && device->rs_total) {
			const unsigned long s = device->rs_same_csum;
			const unsigned long t = device->rs_total;
			const int ratio =
				(t == 0)     ? 0 :
			(t < 100000) ? ((s*100)/t) : (s/(t/100));
			drbd_info(device, "%u %% had equal checksums, eliminated: %luK; "
			     "transferred %luK total %luK\n",
			     ratio,
			     Bit2KB(device->rs_same_csum),
			     Bit2KB(device->rs_total - device->rs_same_csum),
			     Bit2KB(device->rs_total));
		}
	}

	if (device->rs_failed) {
		drbd_info(device, "            %lu failed blocks\n", device->rs_failed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (os.conn == C_SYNC_TARGET || os.conn == C_PAUSED_SYNC_T) {
			ns.disk = D_INCONSISTENT;
			ns.pdsk = D_UP_TO_DATE;
		} else {
			ns.disk = D_UP_TO_DATE;
			ns.pdsk = D_INCONSISTENT;
		}
	} else {
		ns.disk = D_UP_TO_DATE;
		ns.pdsk = D_UP_TO_DATE;

		if (os.conn == C_SYNC_TARGET || os.conn == C_PAUSED_SYNC_T) {
<<<<<<< HEAD
			if (mdev->p_uuid) {
				int i;
				for (i = UI_BITMAP ; i <= UI_HISTORY_END ; i++)
					_drbd_uuid_set(mdev, i, mdev->p_uuid[i]);
				drbd_uuid_set(mdev, UI_BITMAP, mdev->ldev->md.uuid[UI_CURRENT]);
				_drbd_uuid_set(mdev, UI_CURRENT, mdev->p_uuid[UI_CURRENT]);
			} else {
				dev_err(DEV, "mdev->p_uuid is NULL! BUG\n");
=======
			if (device->p_uuid) {
				int i;
				for (i = UI_BITMAP ; i <= UI_HISTORY_END ; i++)
					_drbd_uuid_set(device, i, device->p_uuid[i]);
				drbd_uuid_set(device, UI_BITMAP, device->ldev->md.uuid[UI_CURRENT]);
				_drbd_uuid_set(device, UI_CURRENT, device->p_uuid[UI_CURRENT]);
			} else {
				drbd_err(device, "device->p_uuid is NULL! BUG\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}

		if (!(os.conn == C_VERIFY_S || os.conn == C_VERIFY_T)) {
			/* for verify runs, we don't update uuids here,
			 * so there would be nothing to report. */
<<<<<<< HEAD
			drbd_uuid_set_bm(mdev, 0UL);
			drbd_print_uuids(mdev, "updated UUIDs");
			if (mdev->p_uuid) {
=======
			drbd_uuid_set_bm(device, 0UL);
			drbd_print_uuids(device, "updated UUIDs");
			if (device->p_uuid) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				/* Now the two UUID sets are equal, update what we
				 * know of the peer. */
				int i;
				for (i = UI_CURRENT ; i <= UI_HISTORY_END ; i++)
<<<<<<< HEAD
					mdev->p_uuid[i] = mdev->ldev->md.uuid[i];
=======
					device->p_uuid[i] = device->ldev->md.uuid[i];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}

<<<<<<< HEAD
	_drbd_set_state(mdev, ns, CS_VERBOSE, NULL);
out_unlock:
	spin_unlock_irq(&mdev->req_lock);
	put_ldev(mdev);
out:
	mdev->rs_total  = 0;
	mdev->rs_failed = 0;
	mdev->rs_paused = 0;
	if (verify_done)
		mdev->ov_start_sector = 0;

	drbd_md_sync(mdev);

	if (khelper_cmd)
		drbd_khelper(mdev, khelper_cmd);
=======
	_drbd_set_state(device, ns, CS_VERBOSE, NULL);
out_unlock:
	spin_unlock_irq(&device->resource->req_lock);

	/* If we have been sync source, and have an effective fencing-policy,
	 * once *all* volumes are back in sync, call "unfence". */
	if (os.conn == C_SYNC_SOURCE) {
		enum drbd_disk_state disk_state = D_MASK;
		enum drbd_disk_state pdsk_state = D_MASK;
		enum drbd_fencing_p fp = FP_DONT_CARE;

		rcu_read_lock();
		fp = rcu_dereference(device->ldev->disk_conf)->fencing;
		if (fp != FP_DONT_CARE) {
			struct drbd_peer_device *peer_device;
			int vnr;
			idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
				struct drbd_device *device = peer_device->device;
				disk_state = min_t(enum drbd_disk_state, disk_state, device->state.disk);
				pdsk_state = min_t(enum drbd_disk_state, pdsk_state, device->state.pdsk);
			}
		}
		rcu_read_unlock();
		if (disk_state == D_UP_TO_DATE && pdsk_state == D_UP_TO_DATE)
			conn_khelper(connection, "unfence-peer");
	}

	put_ldev(device);
out:
	device->rs_total  = 0;
	device->rs_failed = 0;
	device->rs_paused = 0;

	/* reset start sector, if we reached end of device */
	if (verify_done && device->ov_left == 0)
		device->ov_start_sector = 0;

	drbd_md_sync(device);

	if (khelper_cmd)
		drbd_khelper(device, khelper_cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}

/* helper */
<<<<<<< HEAD
static void move_to_net_ee_or_free(struct drbd_conf *mdev, struct drbd_epoch_entry *e)
{
	if (drbd_ee_has_active_page(e)) {
		/* This might happen if sendpage() has not finished */
		int i = (e->size + PAGE_SIZE -1) >> PAGE_SHIFT;
		atomic_add(i, &mdev->pp_in_use_by_net);
		atomic_sub(i, &mdev->pp_in_use);
		spin_lock_irq(&mdev->req_lock);
		list_add_tail(&e->w.list, &mdev->net_ee);
		spin_unlock_irq(&mdev->req_lock);
		wake_up(&drbd_pp_wait);
	} else
		drbd_free_ee(mdev, e);
=======
static void move_to_net_ee_or_free(struct drbd_device *device, struct drbd_peer_request *peer_req)
{
	if (drbd_peer_req_has_active_page(peer_req)) {
		/* This might happen if sendpage() has not finished */
		int i = PFN_UP(peer_req->i.size);
		atomic_add(i, &device->pp_in_use_by_net);
		atomic_sub(i, &device->pp_in_use);
		spin_lock_irq(&device->resource->req_lock);
		list_add_tail(&peer_req->w.list, &device->net_ee);
		spin_unlock_irq(&device->resource->req_lock);
		wake_up(&drbd_pp_wait);
	} else
		drbd_free_peer_req(device, peer_req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * w_e_end_data_req() - Worker callback, to send a P_DATA_REPLY packet in response to a P_DATA_REQUEST
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
int w_e_end_data_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	int ok;

	if (unlikely(cancel)) {
		drbd_free_ee(mdev, e);
		dec_unacked(mdev);
		return 1;
	}

	if (likely((e->flags & EE_WAS_ERROR) == 0)) {
		ok = drbd_send_block(mdev, P_DATA_REPLY, e);
	} else {
		if (__ratelimit(&drbd_ratelimit_state))
			dev_err(DEV, "Sending NegDReply. sector=%llus.\n",
			    (unsigned long long)e->sector);

		ok = drbd_send_ack(mdev, P_NEG_DREPLY, e);
	}

	dec_unacked(mdev);

	move_to_net_ee_or_free(mdev, e);

	if (unlikely(!ok))
		dev_err(DEV, "drbd_send_block() failed\n");
	return ok;
}

/**
 * w_e_end_rsdata_req() - Worker callback to send a P_RS_DATA_REPLY packet in response to a P_RS_DATA_REQUESTRS
 * @mdev:	DRBD device.
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
int w_e_end_rsdata_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	int ok;

	if (unlikely(cancel)) {
		drbd_free_ee(mdev, e);
		dec_unacked(mdev);
		return 1;
	}

	if (get_ldev_if_state(mdev, D_FAILED)) {
		drbd_rs_complete_io(mdev, e->sector);
		put_ldev(mdev);
	}

	if (mdev->state.conn == C_AHEAD) {
		ok = drbd_send_ack(mdev, P_RS_CANCEL, e);
	} else if (likely((e->flags & EE_WAS_ERROR) == 0)) {
		if (likely(mdev->state.pdsk >= D_INCONSISTENT)) {
			inc_rs_pending(mdev);
			ok = drbd_send_block(mdev, P_RS_DATA_REPLY, e);
		} else {
			if (__ratelimit(&drbd_ratelimit_state))
				dev_err(DEV, "Not sending RSDataReply, "
				    "partner DISKLESS!\n");
			ok = 1;
		}
	} else {
		if (__ratelimit(&drbd_ratelimit_state))
			dev_err(DEV, "Sending NegRSDReply. sector %llus.\n",
			    (unsigned long long)e->sector);

		ok = drbd_send_ack(mdev, P_NEG_RS_DREPLY, e);

		/* update resync data with failure */
		drbd_rs_failed_io(mdev, e->sector, e->size);
	}

	dec_unacked(mdev);

	move_to_net_ee_or_free(mdev, e);

	if (unlikely(!ok))
		dev_err(DEV, "drbd_send_block() failed\n");
	return ok;
}

int w_e_end_csum_rs_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	struct digest_info *di;
	int digest_size;
	void *digest = NULL;
	int ok, eq = 0;

	if (unlikely(cancel)) {
		drbd_free_ee(mdev, e);
		dec_unacked(mdev);
		return 1;
	}

	if (get_ldev(mdev)) {
		drbd_rs_complete_io(mdev, e->sector);
		put_ldev(mdev);
	}

	di = e->digest;

	if (likely((e->flags & EE_WAS_ERROR) == 0)) {
		/* quick hack to try to avoid a race against reconfiguration.
		 * a real fix would be much more involved,
		 * introducing more locking mechanisms */
		if (mdev->csums_tfm) {
			digest_size = crypto_hash_digestsize(mdev->csums_tfm);
			D_ASSERT(digest_size == di->digest_size);
			digest = kmalloc(digest_size, GFP_NOIO);
		}
		if (digest) {
			drbd_csum_ee(mdev, mdev->csums_tfm, e, digest);
=======
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
int w_e_end_data_req(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_peer_device *peer_device = peer_req->peer_device;
	struct drbd_device *device = peer_device->device;
	int err;

	if (unlikely(cancel)) {
		drbd_free_peer_req(device, peer_req);
		dec_unacked(device);
		return 0;
	}

	if (likely((peer_req->flags & EE_WAS_ERROR) == 0)) {
		err = drbd_send_block(peer_device, P_DATA_REPLY, peer_req);
	} else {
		if (drbd_ratelimit())
			drbd_err(device, "Sending NegDReply. sector=%llus.\n",
			    (unsigned long long)peer_req->i.sector);

		err = drbd_send_ack(peer_device, P_NEG_DREPLY, peer_req);
	}

	dec_unacked(device);

	move_to_net_ee_or_free(device, peer_req);

	if (unlikely(err))
		drbd_err(device, "drbd_send_block() failed\n");
	return err;
}

static bool all_zero(struct drbd_peer_request *peer_req)
{
	struct page *page = peer_req->pages;
	unsigned int len = peer_req->i.size;

	page_chain_for_each(page) {
		unsigned int l = min_t(unsigned int, len, PAGE_SIZE);
		unsigned int i, words = l / sizeof(long);
		unsigned long *d;

		d = kmap_atomic(page);
		for (i = 0; i < words; i++) {
			if (d[i]) {
				kunmap_atomic(d);
				return false;
			}
		}
		kunmap_atomic(d);
		len -= l;
	}

	return true;
}

/**
 * w_e_end_rsdata_req() - Worker callback to send a P_RS_DATA_REPLY packet in response to a P_RS_DATA_REQUEST
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
int w_e_end_rsdata_req(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_peer_device *peer_device = peer_req->peer_device;
	struct drbd_device *device = peer_device->device;
	int err;

	if (unlikely(cancel)) {
		drbd_free_peer_req(device, peer_req);
		dec_unacked(device);
		return 0;
	}

	if (get_ldev_if_state(device, D_FAILED)) {
		drbd_rs_complete_io(device, peer_req->i.sector);
		put_ldev(device);
	}

	if (device->state.conn == C_AHEAD) {
		err = drbd_send_ack(peer_device, P_RS_CANCEL, peer_req);
	} else if (likely((peer_req->flags & EE_WAS_ERROR) == 0)) {
		if (likely(device->state.pdsk >= D_INCONSISTENT)) {
			inc_rs_pending(peer_device);
			if (peer_req->flags & EE_RS_THIN_REQ && all_zero(peer_req))
				err = drbd_send_rs_deallocated(peer_device, peer_req);
			else
				err = drbd_send_block(peer_device, P_RS_DATA_REPLY, peer_req);
		} else {
			if (drbd_ratelimit())
				drbd_err(device, "Not sending RSDataReply, "
				    "partner DISKLESS!\n");
			err = 0;
		}
	} else {
		if (drbd_ratelimit())
			drbd_err(device, "Sending NegRSDReply. sector %llus.\n",
			    (unsigned long long)peer_req->i.sector);

		err = drbd_send_ack(peer_device, P_NEG_RS_DREPLY, peer_req);

		/* update resync data with failure */
		drbd_rs_failed_io(peer_device, peer_req->i.sector, peer_req->i.size);
	}

	dec_unacked(device);

	move_to_net_ee_or_free(device, peer_req);

	if (unlikely(err))
		drbd_err(device, "drbd_send_block() failed\n");
	return err;
}

int w_e_end_csum_rs_req(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_peer_device *peer_device = peer_req->peer_device;
	struct drbd_device *device = peer_device->device;
	struct digest_info *di;
	int digest_size;
	void *digest = NULL;
	int err, eq = 0;

	if (unlikely(cancel)) {
		drbd_free_peer_req(device, peer_req);
		dec_unacked(device);
		return 0;
	}

	if (get_ldev(device)) {
		drbd_rs_complete_io(device, peer_req->i.sector);
		put_ldev(device);
	}

	di = peer_req->digest;

	if (likely((peer_req->flags & EE_WAS_ERROR) == 0)) {
		/* quick hack to try to avoid a race against reconfiguration.
		 * a real fix would be much more involved,
		 * introducing more locking mechanisms */
		if (peer_device->connection->csums_tfm) {
			digest_size = crypto_shash_digestsize(peer_device->connection->csums_tfm);
			D_ASSERT(device, digest_size == di->digest_size);
			digest = kmalloc(digest_size, GFP_NOIO);
		}
		if (digest) {
			drbd_csum_ee(peer_device->connection->csums_tfm, peer_req, digest);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			eq = !memcmp(digest, di->digest, digest_size);
			kfree(digest);
		}

		if (eq) {
<<<<<<< HEAD
			drbd_set_in_sync(mdev, e->sector, e->size);
			/* rs_same_csums unit is BM_BLOCK_SIZE */
			mdev->rs_same_csum += e->size >> BM_BLOCK_SHIFT;
			ok = drbd_send_ack(mdev, P_RS_IS_IN_SYNC, e);
		} else {
			inc_rs_pending(mdev);
			e->block_id = ID_SYNCER; /* By setting block_id, digest pointer becomes invalid! */
			e->flags &= ~EE_HAS_DIGEST; /* This e no longer has a digest pointer */
			kfree(di);
			ok = drbd_send_block(mdev, P_RS_DATA_REPLY, e);
		}
	} else {
		ok = drbd_send_ack(mdev, P_NEG_RS_DREPLY, e);
		if (__ratelimit(&drbd_ratelimit_state))
			dev_err(DEV, "Sending NegDReply. I guess it gets messy.\n");
	}

	dec_unacked(mdev);
	move_to_net_ee_or_free(mdev, e);

	if (unlikely(!ok))
		dev_err(DEV, "drbd_send_block/ack() failed\n");
	return ok;
}

/* TODO merge common code with w_e_send_csum */
int w_e_end_ov_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	sector_t sector = e->sector;
	unsigned int size = e->size;
	int digest_size;
	void *digest;
	int ok = 1;
=======
			drbd_set_in_sync(peer_device, peer_req->i.sector, peer_req->i.size);
			/* rs_same_csums unit is BM_BLOCK_SIZE */
			device->rs_same_csum += peer_req->i.size >> BM_BLOCK_SHIFT;
			err = drbd_send_ack(peer_device, P_RS_IS_IN_SYNC, peer_req);
		} else {
			inc_rs_pending(peer_device);
			peer_req->block_id = ID_SYNCER; /* By setting block_id, digest pointer becomes invalid! */
			peer_req->flags &= ~EE_HAS_DIGEST; /* This peer request no longer has a digest pointer */
			kfree(di);
			err = drbd_send_block(peer_device, P_RS_DATA_REPLY, peer_req);
		}
	} else {
		err = drbd_send_ack(peer_device, P_NEG_RS_DREPLY, peer_req);
		if (drbd_ratelimit())
			drbd_err(device, "Sending NegDReply. I guess it gets messy.\n");
	}

	dec_unacked(device);
	move_to_net_ee_or_free(device, peer_req);

	if (unlikely(err))
		drbd_err(device, "drbd_send_block/ack() failed\n");
	return err;
}

int w_e_end_ov_req(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_peer_device *peer_device = peer_req->peer_device;
	struct drbd_device *device = peer_device->device;
	sector_t sector = peer_req->i.sector;
	unsigned int size = peer_req->i.size;
	int digest_size;
	void *digest;
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(cancel))
		goto out;

<<<<<<< HEAD
	digest_size = crypto_hash_digestsize(mdev->verify_tfm);
	digest = kmalloc(digest_size, GFP_NOIO);
	if (!digest) {
		ok = 0;	/* terminate the connection in case the allocation failed */
		goto out;
	}

	if (likely(!(e->flags & EE_WAS_ERROR)))
		drbd_csum_ee(mdev, mdev->verify_tfm, e, digest);
=======
	digest_size = crypto_shash_digestsize(peer_device->connection->verify_tfm);
	digest = kmalloc(digest_size, GFP_NOIO);
	if (!digest) {
		err = 1;	/* terminate the connection in case the allocation failed */
		goto out;
	}

	if (likely(!(peer_req->flags & EE_WAS_ERROR)))
		drbd_csum_ee(peer_device->connection->verify_tfm, peer_req, digest);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		memset(digest, 0, digest_size);

	/* Free e and pages before send.
	 * In case we block on congestion, we could otherwise run into
	 * some distributed deadlock, if the other side blocks on
	 * congestion as well, because our receiver blocks in
<<<<<<< HEAD
	 * drbd_pp_alloc due to pp_in_use > max_buffers. */
	drbd_free_ee(mdev, e);
	e = NULL;
	inc_rs_pending(mdev);
	ok = drbd_send_drequest_csum(mdev, sector, size,
				     digest, digest_size,
				     P_OV_REPLY);
	if (!ok)
		dec_rs_pending(mdev);
	kfree(digest);

out:
	if (e)
		drbd_free_ee(mdev, e);
	dec_unacked(mdev);
	return ok;
}

void drbd_ov_oos_found(struct drbd_conf *mdev, sector_t sector, int size)
{
	if (mdev->ov_last_oos_start + mdev->ov_last_oos_size == sector) {
		mdev->ov_last_oos_size += size>>9;
	} else {
		mdev->ov_last_oos_start = sector;
		mdev->ov_last_oos_size = size>>9;
	}
	drbd_set_out_of_sync(mdev, sector, size);
}

int w_e_end_ov_reply(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	struct digest_info *di;
	void *digest;
	sector_t sector = e->sector;
	unsigned int size = e->size;
	int digest_size;
	int ok, eq = 0;

	if (unlikely(cancel)) {
		drbd_free_ee(mdev, e);
		dec_unacked(mdev);
		return 1;
=======
	 * drbd_alloc_pages due to pp_in_use > max_buffers. */
	drbd_free_peer_req(device, peer_req);
	peer_req = NULL;
	inc_rs_pending(peer_device);
	err = drbd_send_drequest_csum(peer_device, sector, size, digest, digest_size, P_OV_REPLY);
	if (err)
		dec_rs_pending(peer_device);
	kfree(digest);

out:
	if (peer_req)
		drbd_free_peer_req(device, peer_req);
	dec_unacked(device);
	return err;
}

void drbd_ov_out_of_sync_found(struct drbd_peer_device *peer_device, sector_t sector, int size)
{
	struct drbd_device *device = peer_device->device;
	if (device->ov_last_oos_start + device->ov_last_oos_size == sector) {
		device->ov_last_oos_size += size>>9;
	} else {
		device->ov_last_oos_start = sector;
		device->ov_last_oos_size = size>>9;
	}
	drbd_set_out_of_sync(peer_device, sector, size);
}

int w_e_end_ov_reply(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_peer_device *peer_device = peer_req->peer_device;
	struct drbd_device *device = peer_device->device;
	struct digest_info *di;
	void *digest;
	sector_t sector = peer_req->i.sector;
	unsigned int size = peer_req->i.size;
	int digest_size;
	int err, eq = 0;
	bool stop_sector_reached = false;

	if (unlikely(cancel)) {
		drbd_free_peer_req(device, peer_req);
		dec_unacked(device);
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* after "cancel", because after drbd_disconnect/drbd_rs_cancel_all
	 * the resync lru has been cleaned up already */
<<<<<<< HEAD
	if (get_ldev(mdev)) {
		drbd_rs_complete_io(mdev, e->sector);
		put_ldev(mdev);
	}

	di = e->digest;

	if (likely((e->flags & EE_WAS_ERROR) == 0)) {
		digest_size = crypto_hash_digestsize(mdev->verify_tfm);
		digest = kmalloc(digest_size, GFP_NOIO);
		if (digest) {
			drbd_csum_ee(mdev, mdev->verify_tfm, e, digest);

			D_ASSERT(digest_size == di->digest_size);
=======
	if (get_ldev(device)) {
		drbd_rs_complete_io(device, peer_req->i.sector);
		put_ldev(device);
	}

	di = peer_req->digest;

	if (likely((peer_req->flags & EE_WAS_ERROR) == 0)) {
		digest_size = crypto_shash_digestsize(peer_device->connection->verify_tfm);
		digest = kmalloc(digest_size, GFP_NOIO);
		if (digest) {
			drbd_csum_ee(peer_device->connection->verify_tfm, peer_req, digest);

			D_ASSERT(device, digest_size == di->digest_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			eq = !memcmp(digest, di->digest, digest_size);
			kfree(digest);
		}
	}

<<<<<<< HEAD
		/* Free e and pages before send.
		 * In case we block on congestion, we could otherwise run into
		 * some distributed deadlock, if the other side blocks on
		 * congestion as well, because our receiver blocks in
		 * drbd_pp_alloc due to pp_in_use > max_buffers. */
	drbd_free_ee(mdev, e);
	if (!eq)
		drbd_ov_oos_found(mdev, sector, size);
	else
		ov_oos_print(mdev);

	ok = drbd_send_ack_ex(mdev, P_OV_RESULT, sector, size,
			      eq ? ID_IN_SYNC : ID_OUT_OF_SYNC);

	dec_unacked(mdev);

	--mdev->ov_left;

	/* let's advance progress step marks only for every other megabyte */
	if ((mdev->ov_left & 0x200) == 0x200)
		drbd_advance_rs_marks(mdev, mdev->ov_left);

	if (mdev->ov_left == 0) {
		ov_oos_print(mdev);
		drbd_resync_finished(mdev);
	}

	return ok;
}

int w_prev_work_done(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_wq_barrier *b = container_of(w, struct drbd_wq_barrier, w);
	complete(&b->done);
	return 1;
}

int w_send_barrier(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_tl_epoch *b = container_of(w, struct drbd_tl_epoch, w);
	struct p_barrier *p = &mdev->data.sbuf.barrier;
	int ok = 1;

	/* really avoid racing with tl_clear.  w.cb may have been referenced
	 * just before it was reassigned and re-queued, so double check that.
	 * actually, this race was harmless, since we only try to send the
	 * barrier packet here, and otherwise do nothing with the object.
	 * but compare with the head of w_clear_epoch */
	spin_lock_irq(&mdev->req_lock);
	if (w->cb != w_send_barrier || mdev->state.conn < C_CONNECTED)
		cancel = 1;
	spin_unlock_irq(&mdev->req_lock);
	if (cancel)
		return 1;

	if (!drbd_get_data_sock(mdev))
		return 0;
	p->barrier = b->br_number;
	/* inc_ap_pending was done where this was queued.
	 * dec_ap_pending will be done in got_BarrierAck
	 * or (on connection loss) in w_clear_epoch.  */
	ok = _drbd_send_cmd(mdev, mdev->data.socket, P_BARRIER,
				(struct p_header80 *)p, sizeof(*p), 0);
	drbd_put_data_sock(mdev);

	return ok;
}

int w_send_write_hint(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	if (cancel)
		return 1;
	return drbd_send_short_cmd(mdev, P_UNPLUG_REMOTE);
}

int w_send_oos(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	int ok;

	if (unlikely(cancel)) {
		req_mod(req, send_canceled);
		return 1;
	}

	ok = drbd_send_oos(mdev, req);
	req_mod(req, oos_handed_to_network);

	return ok;
=======
	/* Free peer_req and pages before send.
	 * In case we block on congestion, we could otherwise run into
	 * some distributed deadlock, if the other side blocks on
	 * congestion as well, because our receiver blocks in
	 * drbd_alloc_pages due to pp_in_use > max_buffers. */
	drbd_free_peer_req(device, peer_req);
	if (!eq)
		drbd_ov_out_of_sync_found(peer_device, sector, size);
	else
		ov_out_of_sync_print(peer_device);

	err = drbd_send_ack_ex(peer_device, P_OV_RESULT, sector, size,
			       eq ? ID_IN_SYNC : ID_OUT_OF_SYNC);

	dec_unacked(device);

	--device->ov_left;

	/* let's advance progress step marks only for every other megabyte */
	if ((device->ov_left & 0x200) == 0x200)
		drbd_advance_rs_marks(peer_device, device->ov_left);

	stop_sector_reached = verify_can_do_stop_sector(device) &&
		(sector + (size>>9)) >= device->ov_stop_sector;

	if (device->ov_left == 0 || stop_sector_reached) {
		ov_out_of_sync_print(peer_device);
		drbd_resync_finished(peer_device);
	}

	return err;
}

/* FIXME
 * We need to track the number of pending barrier acks,
 * and to be able to wait for them.
 * See also comment in drbd_adm_attach before drbd_suspend_io.
 */
static int drbd_send_barrier(struct drbd_connection *connection)
{
	struct p_barrier *p;
	struct drbd_socket *sock;

	sock = &connection->data;
	p = conn_prepare_command(connection, sock);
	if (!p)
		return -EIO;
	p->barrier = connection->send.current_epoch_nr;
	p->pad = 0;
	connection->send.current_epoch_writes = 0;
	connection->send.last_sent_barrier_jif = jiffies;

	return conn_send_command(connection, sock, P_BARRIER, sizeof(*p), NULL, 0);
}

static int pd_send_unplug_remote(struct drbd_peer_device *pd)
{
	struct drbd_socket *sock = &pd->connection->data;
	if (!drbd_prepare_command(pd, sock))
		return -EIO;
	return drbd_send_command(pd, sock, P_UNPLUG_REMOTE, 0, NULL, 0);
}

int w_send_write_hint(struct drbd_work *w, int cancel)
{
	struct drbd_device *device =
		container_of(w, struct drbd_device, unplug_work);

	if (cancel)
		return 0;
	return pd_send_unplug_remote(first_peer_device(device));
}

static void re_init_if_first_write(struct drbd_connection *connection, unsigned int epoch)
{
	if (!connection->send.seen_any_write_yet) {
		connection->send.seen_any_write_yet = true;
		connection->send.current_epoch_nr = epoch;
		connection->send.current_epoch_writes = 0;
		connection->send.last_sent_barrier_jif = jiffies;
	}
}

static void maybe_send_barrier(struct drbd_connection *connection, unsigned int epoch)
{
	/* re-init if first write on this connection */
	if (!connection->send.seen_any_write_yet)
		return;
	if (connection->send.current_epoch_nr != epoch) {
		if (connection->send.current_epoch_writes)
			drbd_send_barrier(connection);
		connection->send.current_epoch_nr = epoch;
	}
}

int w_send_out_of_sync(struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	struct drbd_device *device = req->device;
	struct drbd_peer_device *const peer_device = first_peer_device(device);
	struct drbd_connection *const connection = peer_device->connection;
	int err;

	if (unlikely(cancel)) {
		req_mod(req, SEND_CANCELED, peer_device);
		return 0;
	}
	req->pre_send_jif = jiffies;

	/* this time, no connection->send.current_epoch_writes++;
	 * If it was sent, it was the closing barrier for the last
	 * replicated epoch, before we went into AHEAD mode.
	 * No more barriers will be sent, until we leave AHEAD mode again. */
	maybe_send_barrier(connection, req->epoch);

	err = drbd_send_out_of_sync(peer_device, req);
	req_mod(req, OOS_HANDED_TO_NETWORK, peer_device);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * w_send_dblock() - Worker callback to send a P_DATA packet in order to mirror a write request
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
int w_send_dblock(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	int ok;

	if (unlikely(cancel)) {
		req_mod(req, send_canceled);
		return 1;
	}

	ok = drbd_send_dblock(mdev, req);
	req_mod(req, ok ? handed_over_to_network : send_failed);

	return ok;
=======
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
int w_send_dblock(struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	struct drbd_device *device = req->device;
	struct drbd_peer_device *const peer_device = first_peer_device(device);
	struct drbd_connection *connection = peer_device->connection;
	bool do_send_unplug = req->rq_state & RQ_UNPLUG;
	int err;

	if (unlikely(cancel)) {
		req_mod(req, SEND_CANCELED, peer_device);
		return 0;
	}
	req->pre_send_jif = jiffies;

	re_init_if_first_write(connection, req->epoch);
	maybe_send_barrier(connection, req->epoch);
	connection->send.current_epoch_writes++;

	err = drbd_send_dblock(peer_device, req);
	req_mod(req, err ? SEND_FAILED : HANDED_OVER_TO_NETWORK, peer_device);

	if (do_send_unplug && !err)
		pd_send_unplug_remote(peer_device);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * w_send_read_req() - Worker callback to send a read request (P_DATA_REQUEST) packet
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
int w_send_read_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	int ok;

	if (unlikely(cancel)) {
		req_mod(req, send_canceled);
		return 1;
	}

	ok = drbd_send_drequest(mdev, P_DATA_REQUEST, req->sector, req->size,
				(unsigned long)req);

	if (!ok) {
		/* ?? we set C_TIMEOUT or C_BROKEN_PIPE in drbd_send();
		 * so this is probably redundant */
		if (mdev->state.conn >= C_CONNECTED)
			drbd_force_state(mdev, NS(conn, C_NETWORK_FAILURE));
	}
	req_mod(req, ok ? handed_over_to_network : send_failed);

	return ok;
}

int w_restart_disk_io(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);

	if (bio_data_dir(req->master_bio) == WRITE && req->rq_state & RQ_IN_ACT_LOG)
		drbd_al_begin_io(mdev, req->sector);
	/* Calling drbd_al_begin_io() out of the worker might deadlocks
	   theoretically. Practically it can not deadlock, since this is
	   only used when unfreezing IOs. All the extents of the requests
	   that made it into the TL are already active */

	drbd_req_make_private_bio(req, req->master_bio);
	req->private_bio->bi_bdev = mdev->ldev->backing_bdev;
	generic_make_request(req->private_bio);

	return 1;
}

static int _drbd_may_sync_now(struct drbd_conf *mdev)
{
	struct drbd_conf *odev = mdev;

	while (1) {
		if (odev->sync_conf.after == -1)
			return 1;
		odev = minor_to_mdev(odev->sync_conf.after);
		ERR_IF(!odev) return 1;
=======
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
int w_send_read_req(struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	struct drbd_device *device = req->device;
	struct drbd_peer_device *const peer_device = first_peer_device(device);
	struct drbd_connection *connection = peer_device->connection;
	bool do_send_unplug = req->rq_state & RQ_UNPLUG;
	int err;

	if (unlikely(cancel)) {
		req_mod(req, SEND_CANCELED, peer_device);
		return 0;
	}
	req->pre_send_jif = jiffies;

	/* Even read requests may close a write epoch,
	 * if there was any yet. */
	maybe_send_barrier(connection, req->epoch);

	err = drbd_send_drequest(peer_device, P_DATA_REQUEST, req->i.sector, req->i.size,
				 (unsigned long)req);

	req_mod(req, err ? SEND_FAILED : HANDED_OVER_TO_NETWORK, peer_device);

	if (do_send_unplug && !err)
		pd_send_unplug_remote(peer_device);

	return err;
}

int w_restart_disk_io(struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	struct drbd_device *device = req->device;

	if (bio_data_dir(req->master_bio) == WRITE && req->rq_state & RQ_IN_ACT_LOG)
		drbd_al_begin_io(device, &req->i);

	req->private_bio = bio_alloc_clone(device->ldev->backing_bdev,
					   req->master_bio, GFP_NOIO,
					  &drbd_io_bio_set);
	req->private_bio->bi_private = req;
	req->private_bio->bi_end_io = drbd_request_endio;
	submit_bio_noacct(req->private_bio);

	return 0;
}

static int _drbd_may_sync_now(struct drbd_device *device)
{
	struct drbd_device *odev = device;
	int resync_after;

	while (1) {
		if (!odev->ldev || odev->state.disk == D_DISKLESS)
			return 1;
		rcu_read_lock();
		resync_after = rcu_dereference(odev->ldev->disk_conf)->resync_after;
		rcu_read_unlock();
		if (resync_after == -1)
			return 1;
		odev = minor_to_device(resync_after);
		if (!odev)
			return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if ((odev->state.conn >= C_SYNC_SOURCE &&
		     odev->state.conn <= C_PAUSED_SYNC_T) ||
		    odev->state.aftr_isp || odev->state.peer_isp ||
		    odev->state.user_isp)
			return 0;
	}
}

/**
<<<<<<< HEAD
 * _drbd_pause_after() - Pause resync on all devices that may not resync now
 * @mdev:	DRBD device.
 *
 * Called from process context only (admin command and after_state_ch).
 */
static int _drbd_pause_after(struct drbd_conf *mdev)
{
	struct drbd_conf *odev;
	int i, rv = 0;

	for (i = 0; i < minor_count; i++) {
		odev = minor_to_mdev(i);
		if (!odev)
			continue;
		if (odev->state.conn == C_STANDALONE && odev->state.disk == D_DISKLESS)
			continue;
		if (!_drbd_may_sync_now(odev))
			rv |= (__drbd_set_state(_NS(odev, aftr_isp, 1), CS_HARD, NULL)
			       != SS_NOTHING_TO_DO);
	}

	return rv;
}

/**
 * _drbd_resume_next() - Resume resync on all devices that may resync now
 * @mdev:	DRBD device.
 *
 * Called from process context only (admin command and worker).
 */
static int _drbd_resume_next(struct drbd_conf *mdev)
{
	struct drbd_conf *odev;
	int i, rv = 0;

	for (i = 0; i < minor_count; i++) {
		odev = minor_to_mdev(i);
		if (!odev)
			continue;
		if (odev->state.conn == C_STANDALONE && odev->state.disk == D_DISKLESS)
			continue;
		if (odev->state.aftr_isp) {
			if (_drbd_may_sync_now(odev))
				rv |= (__drbd_set_state(_NS(odev, aftr_isp, 0),
							CS_HARD, NULL)
				       != SS_NOTHING_TO_DO) ;
		}
	}
	return rv;
}

void resume_next_sg(struct drbd_conf *mdev)
{
	write_lock_irq(&global_state_lock);
	_drbd_resume_next(mdev);
	write_unlock_irq(&global_state_lock);
}

void suspend_other_sg(struct drbd_conf *mdev)
{
	write_lock_irq(&global_state_lock);
	_drbd_pause_after(mdev);
	write_unlock_irq(&global_state_lock);
}

static int sync_after_error(struct drbd_conf *mdev, int o_minor)
{
	struct drbd_conf *odev;

	if (o_minor == -1)
		return NO_ERROR;
	if (o_minor < -1 || minor_to_mdev(o_minor) == NULL)
		return ERR_SYNC_AFTER;

	/* check for loops */
	odev = minor_to_mdev(o_minor);
	while (1) {
		if (odev == mdev)
			return ERR_SYNC_AFTER_CYCLE;

		/* dependency chain ends here, no cycles. */
		if (odev->sync_conf.after == -1)
			return NO_ERROR;

		/* follow the dependency chain */
		odev = minor_to_mdev(odev->sync_conf.after);
	}
}

int drbd_alter_sa(struct drbd_conf *mdev, int na)
{
	int changes;
	int retcode;

	write_lock_irq(&global_state_lock);
	retcode = sync_after_error(mdev, na);
	if (retcode == NO_ERROR) {
		mdev->sync_conf.after = na;
		do {
			changes  = _drbd_pause_after(mdev);
			changes |= _drbd_resume_next(mdev);
		} while (changes);
	}
	write_unlock_irq(&global_state_lock);
	return retcode;
}

void drbd_rs_controller_reset(struct drbd_conf *mdev)
{
	atomic_set(&mdev->rs_sect_in, 0);
	atomic_set(&mdev->rs_sect_ev, 0);
	mdev->rs_in_flight = 0;
	mdev->rs_planed = 0;
	spin_lock(&mdev->peer_seq_lock);
	fifo_set(&mdev->rs_plan_s, 0);
	spin_unlock(&mdev->peer_seq_lock);
=======
 * drbd_pause_after() - Pause resync on all devices that may not resync now
 * @device:	DRBD device.
 *
 * Called from process context only (admin command and after_state_ch).
 */
static bool drbd_pause_after(struct drbd_device *device)
{
	bool changed = false;
	struct drbd_device *odev;
	int i;

	rcu_read_lock();
	idr_for_each_entry(&drbd_devices, odev, i) {
		if (odev->state.conn == C_STANDALONE && odev->state.disk == D_DISKLESS)
			continue;
		if (!_drbd_may_sync_now(odev) &&
		    _drbd_set_state(_NS(odev, aftr_isp, 1),
				    CS_HARD, NULL) != SS_NOTHING_TO_DO)
			changed = true;
	}
	rcu_read_unlock();

	return changed;
}

/**
 * drbd_resume_next() - Resume resync on all devices that may resync now
 * @device:	DRBD device.
 *
 * Called from process context only (admin command and worker).
 */
static bool drbd_resume_next(struct drbd_device *device)
{
	bool changed = false;
	struct drbd_device *odev;
	int i;

	rcu_read_lock();
	idr_for_each_entry(&drbd_devices, odev, i) {
		if (odev->state.conn == C_STANDALONE && odev->state.disk == D_DISKLESS)
			continue;
		if (odev->state.aftr_isp) {
			if (_drbd_may_sync_now(odev) &&
			    _drbd_set_state(_NS(odev, aftr_isp, 0),
					    CS_HARD, NULL) != SS_NOTHING_TO_DO)
				changed = true;
		}
	}
	rcu_read_unlock();
	return changed;
}

void resume_next_sg(struct drbd_device *device)
{
	lock_all_resources();
	drbd_resume_next(device);
	unlock_all_resources();
}

void suspend_other_sg(struct drbd_device *device)
{
	lock_all_resources();
	drbd_pause_after(device);
	unlock_all_resources();
}

/* caller must lock_all_resources() */
enum drbd_ret_code drbd_resync_after_valid(struct drbd_device *device, int o_minor)
{
	struct drbd_device *odev;
	int resync_after;

	if (o_minor == -1)
		return NO_ERROR;
	if (o_minor < -1 || o_minor > MINORMASK)
		return ERR_RESYNC_AFTER;

	/* check for loops */
	odev = minor_to_device(o_minor);
	while (1) {
		if (odev == device)
			return ERR_RESYNC_AFTER_CYCLE;

		/* You are free to depend on diskless, non-existing,
		 * or not yet/no longer existing minors.
		 * We only reject dependency loops.
		 * We cannot follow the dependency chain beyond a detached or
		 * missing minor.
		 */
		if (!odev || !odev->ldev || odev->state.disk == D_DISKLESS)
			return NO_ERROR;

		rcu_read_lock();
		resync_after = rcu_dereference(odev->ldev->disk_conf)->resync_after;
		rcu_read_unlock();
		/* dependency chain ends here, no cycles. */
		if (resync_after == -1)
			return NO_ERROR;

		/* follow the dependency chain */
		odev = minor_to_device(resync_after);
	}
}

/* caller must lock_all_resources() */
void drbd_resync_after_changed(struct drbd_device *device)
{
	int changed;

	do {
		changed  = drbd_pause_after(device);
		changed |= drbd_resume_next(device);
	} while (changed);
}

void drbd_rs_controller_reset(struct drbd_peer_device *peer_device)
{
	struct drbd_device *device = peer_device->device;
	struct gendisk *disk = device->ldev->backing_bdev->bd_disk;
	struct fifo_buffer *plan;

	atomic_set(&device->rs_sect_in, 0);
	atomic_set(&device->rs_sect_ev, 0);
	device->rs_in_flight = 0;
	device->rs_last_events =
		(int)part_stat_read_accum(disk->part0, sectors);

	/* Updating the RCU protected object in place is necessary since
	   this function gets called from atomic context.
	   It is valid since all other updates also lead to an completely
	   empty fifo */
	rcu_read_lock();
	plan = rcu_dereference(device->rs_plan_s);
	plan->total = 0;
	fifo_set(plan, 0);
	rcu_read_unlock();
}

void start_resync_timer_fn(struct timer_list *t)
{
	struct drbd_device *device = from_timer(device, t, start_resync_timer);
	drbd_device_post_work(device, RS_START);
}

static void do_start_resync(struct drbd_device *device)
{
	if (atomic_read(&device->unacked_cnt) || atomic_read(&device->rs_pending_cnt)) {
		drbd_warn(device, "postponing start_resync ...\n");
		device->start_resync_timer.expires = jiffies + HZ/10;
		add_timer(&device->start_resync_timer);
		return;
	}

	drbd_start_resync(device, C_SYNC_SOURCE);
	clear_bit(AHEAD_TO_SYNC_SOURCE, &device->flags);
}

static bool use_checksum_based_resync(struct drbd_connection *connection, struct drbd_device *device)
{
	bool csums_after_crash_only;
	rcu_read_lock();
	csums_after_crash_only = rcu_dereference(connection->net_conf)->csums_after_crash_only;
	rcu_read_unlock();
	return connection->agreed_pro_version >= 89 &&		/* supported? */
		connection->csums_tfm &&			/* configured? */
		(csums_after_crash_only == false		/* use for each resync? */
		 || test_bit(CRASHED_PRIMARY, &device->flags));	/* or only after Primary crash? */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * drbd_start_resync() - Start the resync process
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @device:	DRBD device.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @side:	Either C_SYNC_SOURCE or C_SYNC_TARGET
 *
 * This function might bring you directly into one of the
 * C_PAUSED_SYNC_* states.
 */
<<<<<<< HEAD
void drbd_start_resync(struct drbd_conf *mdev, enum drbd_conns side)
{
	union drbd_state ns;
	int r;

	if (mdev->state.conn >= C_SYNC_SOURCE && mdev->state.conn < C_AHEAD) {
		dev_err(DEV, "Resync already running!\n");
		return;
	}

	if (mdev->state.conn < C_AHEAD) {
		/* In case a previous resync run was aborted by an IO error/detach on the peer. */
		drbd_rs_cancel_all(mdev);
		/* This should be done when we abort the resync. We definitely do not
		   want to have this for connections going back and forth between
		   Ahead/Behind and SyncSource/SyncTarget */
	}

	if (side == C_SYNC_TARGET) {
		/* Since application IO was locked out during C_WF_BITMAP_T and
		   C_WF_SYNC_UUID we are still unmodified. Before going to C_SYNC_TARGET
		   we check that we might make the data inconsistent. */
		r = drbd_khelper(mdev, "before-resync-target");
		r = (r >> 8) & 0xff;
		if (r > 0) {
			dev_info(DEV, "before-resync-target handler returned %d, "
			     "dropping connection.\n", r);
			drbd_force_state(mdev, NS(conn, C_DISCONNECTING));
			return;
		}
	} else /* C_SYNC_SOURCE */ {
		r = drbd_khelper(mdev, "before-resync-source");
		r = (r >> 8) & 0xff;
		if (r > 0) {
			if (r == 3) {
				dev_info(DEV, "before-resync-source handler returned %d, "
					 "ignoring. Old userland tools?", r);
			} else {
				dev_info(DEV, "before-resync-source handler returned %d, "
					 "dropping connection.\n", r);
				drbd_force_state(mdev, NS(conn, C_DISCONNECTING));
				return;
			}
		}
	}

	drbd_state_lock(mdev);

	if (!get_ldev_if_state(mdev, D_NEGOTIATING)) {
		drbd_state_unlock(mdev);
		return;
	}

	write_lock_irq(&global_state_lock);
	ns = mdev->state;

	ns.aftr_isp = !_drbd_may_sync_now(mdev);
=======
void drbd_start_resync(struct drbd_device *device, enum drbd_conns side)
{
	struct drbd_peer_device *peer_device = first_peer_device(device);
	struct drbd_connection *connection = peer_device ? peer_device->connection : NULL;
	union drbd_state ns;
	int r;

	if (device->state.conn >= C_SYNC_SOURCE && device->state.conn < C_AHEAD) {
		drbd_err(device, "Resync already running!\n");
		return;
	}

	if (!connection) {
		drbd_err(device, "No connection to peer, aborting!\n");
		return;
	}

	if (!test_bit(B_RS_H_DONE, &device->flags)) {
		if (side == C_SYNC_TARGET) {
			/* Since application IO was locked out during C_WF_BITMAP_T and
			   C_WF_SYNC_UUID we are still unmodified. Before going to C_SYNC_TARGET
			   we check that we might make the data inconsistent. */
			r = drbd_khelper(device, "before-resync-target");
			r = (r >> 8) & 0xff;
			if (r > 0) {
				drbd_info(device, "before-resync-target handler returned %d, "
					 "dropping connection.\n", r);
				conn_request_state(connection, NS(conn, C_DISCONNECTING), CS_HARD);
				return;
			}
		} else /* C_SYNC_SOURCE */ {
			r = drbd_khelper(device, "before-resync-source");
			r = (r >> 8) & 0xff;
			if (r > 0) {
				if (r == 3) {
					drbd_info(device, "before-resync-source handler returned %d, "
						 "ignoring. Old userland tools?", r);
				} else {
					drbd_info(device, "before-resync-source handler returned %d, "
						 "dropping connection.\n", r);
					conn_request_state(connection,
							   NS(conn, C_DISCONNECTING), CS_HARD);
					return;
				}
			}
		}
	}

	if (current == connection->worker.task) {
		/* The worker should not sleep waiting for state_mutex,
		   that can take long */
		if (!mutex_trylock(device->state_mutex)) {
			set_bit(B_RS_H_DONE, &device->flags);
			device->start_resync_timer.expires = jiffies + HZ/5;
			add_timer(&device->start_resync_timer);
			return;
		}
	} else {
		mutex_lock(device->state_mutex);
	}

	lock_all_resources();
	clear_bit(B_RS_H_DONE, &device->flags);
	/* Did some connection breakage or IO error race with us? */
	if (device->state.conn < C_CONNECTED
	|| !get_ldev_if_state(device, D_NEGOTIATING)) {
		unlock_all_resources();
		goto out;
	}

	ns = drbd_read_state(device);

	ns.aftr_isp = !_drbd_may_sync_now(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ns.conn = side;

	if (side == C_SYNC_TARGET)
		ns.disk = D_INCONSISTENT;
	else /* side == C_SYNC_SOURCE */
		ns.pdsk = D_INCONSISTENT;

<<<<<<< HEAD
	r = __drbd_set_state(mdev, ns, CS_VERBOSE, NULL);
	ns = mdev->state;
=======
	r = _drbd_set_state(device, ns, CS_VERBOSE, NULL);
	ns = drbd_read_state(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ns.conn < C_CONNECTED)
		r = SS_UNKNOWN_ERROR;

	if (r == SS_SUCCESS) {
<<<<<<< HEAD
		unsigned long tw = drbd_bm_total_weight(mdev);
		unsigned long now = jiffies;
		int i;

		mdev->rs_failed    = 0;
		mdev->rs_paused    = 0;
		mdev->rs_same_csum = 0;
		mdev->rs_last_events = 0;
		mdev->rs_last_sect_ev = 0;
		mdev->rs_total     = tw;
		mdev->rs_start     = now;
		for (i = 0; i < DRBD_SYNC_MARKS; i++) {
			mdev->rs_mark_left[i] = tw;
			mdev->rs_mark_time[i] = now;
		}
		_drbd_pause_after(mdev);
	}
	write_unlock_irq(&global_state_lock);

	if (r == SS_SUCCESS) {
		dev_info(DEV, "Began resync as %s (will sync %lu KB [%lu bits set]).\n",
		     drbd_conn_str(ns.conn),
		     (unsigned long) mdev->rs_total << (BM_BLOCK_SHIFT-10),
		     (unsigned long) mdev->rs_total);
		if (side == C_SYNC_TARGET)
			mdev->bm_resync_fo = 0;
=======
		unsigned long tw = drbd_bm_total_weight(device);
		unsigned long now = jiffies;
		int i;

		device->rs_failed    = 0;
		device->rs_paused    = 0;
		device->rs_same_csum = 0;
		device->rs_last_sect_ev = 0;
		device->rs_total     = tw;
		device->rs_start     = now;
		for (i = 0; i < DRBD_SYNC_MARKS; i++) {
			device->rs_mark_left[i] = tw;
			device->rs_mark_time[i] = now;
		}
		drbd_pause_after(device);
		/* Forget potentially stale cached per resync extent bit-counts.
		 * Open coded drbd_rs_cancel_all(device), we already have IRQs
		 * disabled, and know the disk state is ok. */
		spin_lock(&device->al_lock);
		lc_reset(device->resync);
		device->resync_locked = 0;
		device->resync_wenr = LC_FREE;
		spin_unlock(&device->al_lock);
	}
	unlock_all_resources();

	if (r == SS_SUCCESS) {
		wake_up(&device->al_wait); /* for lc_reset() above */
		/* reset rs_last_bcast when a resync or verify is started,
		 * to deal with potential jiffies wrap. */
		device->rs_last_bcast = jiffies - HZ;

		drbd_info(device, "Began resync as %s (will sync %lu KB [%lu bits set]).\n",
		     drbd_conn_str(ns.conn),
		     (unsigned long) device->rs_total << (BM_BLOCK_SHIFT-10),
		     (unsigned long) device->rs_total);
		if (side == C_SYNC_TARGET) {
			device->bm_resync_fo = 0;
			device->use_csums = use_checksum_based_resync(connection, device);
		} else {
			device->use_csums = false;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* Since protocol 96, we must serialize drbd_gen_and_send_sync_uuid
		 * with w_send_oos, or the sync target will get confused as to
		 * how much bits to resync.  We cannot do that always, because for an
		 * empty resync and protocol < 95, we need to do it here, as we call
		 * drbd_resync_finished from here in that case.
		 * We drbd_gen_and_send_sync_uuid here for protocol < 96,
		 * and from after_state_ch otherwise. */
<<<<<<< HEAD
		if (side == C_SYNC_SOURCE && mdev->agreed_pro_version < 96)
			drbd_gen_and_send_sync_uuid(mdev);

		if (mdev->agreed_pro_version < 95 && mdev->rs_total == 0) {
=======
		if (side == C_SYNC_SOURCE && connection->agreed_pro_version < 96)
			drbd_gen_and_send_sync_uuid(peer_device);

		if (connection->agreed_pro_version < 95 && device->rs_total == 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* This still has a race (about when exactly the peers
			 * detect connection loss) that can lead to a full sync
			 * on next handshake. In 8.3.9 we fixed this with explicit
			 * resync-finished notifications, but the fix
			 * introduces a protocol change.  Sleeping for some
			 * time longer than the ping interval + timeout on the
			 * SyncSource, to give the SyncTarget the chance to
			 * detect connection loss, then waiting for a ping
			 * response (implicit in drbd_resync_finished) reduces
			 * the race considerably, but does not solve it. */
<<<<<<< HEAD
			if (side == C_SYNC_SOURCE)
				schedule_timeout_interruptible(
					mdev->net_conf->ping_int * HZ +
					mdev->net_conf->ping_timeo*HZ/9);
			drbd_resync_finished(mdev);
		}

		drbd_rs_controller_reset(mdev);
		/* ns.conn may already be != mdev->state.conn,
=======
			if (side == C_SYNC_SOURCE) {
				struct net_conf *nc;
				int timeo;

				rcu_read_lock();
				nc = rcu_dereference(connection->net_conf);
				timeo = nc->ping_int * HZ + nc->ping_timeo * HZ / 9;
				rcu_read_unlock();
				schedule_timeout_interruptible(timeo);
			}
			drbd_resync_finished(peer_device);
		}

		drbd_rs_controller_reset(peer_device);
		/* ns.conn may already be != device->state.conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * we may have been paused in between, or become paused until
		 * the timer triggers.
		 * No matter, that is handled in resync_timer_fn() */
		if (ns.conn == C_SYNC_TARGET)
<<<<<<< HEAD
			mod_timer(&mdev->resync_timer, jiffies);

		drbd_md_sync(mdev);
	}
	put_ldev(mdev);
	drbd_state_unlock(mdev);
=======
			mod_timer(&device->resync_timer, jiffies);

		drbd_md_sync(device);
	}
	put_ldev(device);
out:
	mutex_unlock(device->state_mutex);
}

static void update_on_disk_bitmap(struct drbd_peer_device *peer_device, bool resync_done)
{
	struct drbd_device *device = peer_device->device;
	struct sib_info sib = { .sib_reason = SIB_SYNC_PROGRESS, };
	device->rs_last_bcast = jiffies;

	if (!get_ldev(device))
		return;

	drbd_bm_write_lazy(device, 0);
	if (resync_done && is_sync_state(device->state.conn))
		drbd_resync_finished(peer_device);

	drbd_bcast_event(device, &sib);
	/* update timestamp, in case it took a while to write out stuff */
	device->rs_last_bcast = jiffies;
	put_ldev(device);
}

static void drbd_ldev_destroy(struct drbd_device *device)
{
	lc_destroy(device->resync);
	device->resync = NULL;
	lc_destroy(device->act_log);
	device->act_log = NULL;

	__acquire(local);
	drbd_backing_dev_free(device, device->ldev);
	device->ldev = NULL;
	__release(local);

	clear_bit(GOING_DISKLESS, &device->flags);
	wake_up(&device->misc_wait);
}

static void go_diskless(struct drbd_device *device)
{
	struct drbd_peer_device *peer_device = first_peer_device(device);
	D_ASSERT(device, device->state.disk == D_FAILED);
	/* we cannot assert local_cnt == 0 here, as get_ldev_if_state will
	 * inc/dec it frequently. Once we are D_DISKLESS, no one will touch
	 * the protected members anymore, though, so once put_ldev reaches zero
	 * again, it will be safe to free them. */

	/* Try to write changed bitmap pages, read errors may have just
	 * set some bits outside the area covered by the activity log.
	 *
	 * If we have an IO error during the bitmap writeout,
	 * we will want a full sync next time, just in case.
	 * (Do we want a specific meta data flag for this?)
	 *
	 * If that does not make it to stable storage either,
	 * we cannot do anything about that anymore.
	 *
	 * We still need to check if both bitmap and ldev are present, we may
	 * end up here after a failed attach, before ldev was even assigned.
	 */
	if (device->bitmap && device->ldev) {
		/* An interrupted resync or similar is allowed to recounts bits
		 * while we detach.
		 * Any modifications would not be expected anymore, though.
		 */
		if (drbd_bitmap_io_from_worker(device, drbd_bm_write,
					"detach", BM_LOCKED_TEST_ALLOWED, peer_device)) {
			if (test_bit(WAS_READ_ERROR, &device->flags)) {
				drbd_md_set_flag(device, MDF_FULL_SYNC);
				drbd_md_sync(device);
			}
		}
	}

	drbd_force_state(device, NS(disk, D_DISKLESS));
}

static int do_md_sync(struct drbd_device *device)
{
	drbd_warn(device, "md_sync_timer expired! Worker calls drbd_md_sync().\n");
	drbd_md_sync(device);
	return 0;
}

/* only called from drbd_worker thread, no locking */
void __update_timing_details(
		struct drbd_thread_timing_details *tdp,
		unsigned int *cb_nr,
		void *cb,
		const char *fn, const unsigned int line)
{
	unsigned int i = *cb_nr % DRBD_THREAD_DETAILS_HIST;
	struct drbd_thread_timing_details *td = tdp + i;

	td->start_jif = jiffies;
	td->cb_addr = cb;
	td->caller_fn = fn;
	td->line = line;
	td->cb_nr = *cb_nr;

	i = (i+1) % DRBD_THREAD_DETAILS_HIST;
	td = tdp + i;
	memset(td, 0, sizeof(*td));

	++(*cb_nr);
}

static void do_device_work(struct drbd_device *device, const unsigned long todo)
{
	if (test_bit(MD_SYNC, &todo))
		do_md_sync(device);
	if (test_bit(RS_DONE, &todo) ||
	    test_bit(RS_PROGRESS, &todo))
		update_on_disk_bitmap(first_peer_device(device), test_bit(RS_DONE, &todo));
	if (test_bit(GO_DISKLESS, &todo))
		go_diskless(device);
	if (test_bit(DESTROY_DISK, &todo))
		drbd_ldev_destroy(device);
	if (test_bit(RS_START, &todo))
		do_start_resync(device);
}

#define DRBD_DEVICE_WORK_MASK	\
	((1UL << GO_DISKLESS)	\
	|(1UL << DESTROY_DISK)	\
	|(1UL << MD_SYNC)	\
	|(1UL << RS_START)	\
	|(1UL << RS_PROGRESS)	\
	|(1UL << RS_DONE)	\
	)

static unsigned long get_work_bits(unsigned long *flags)
{
	unsigned long old, new;
	do {
		old = *flags;
		new = old & ~DRBD_DEVICE_WORK_MASK;
	} while (cmpxchg(flags, old, new) != old);
	return old & DRBD_DEVICE_WORK_MASK;
}

static void do_unqueued_work(struct drbd_connection *connection)
{
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		unsigned long todo = get_work_bits(&device->flags);
		if (!todo)
			continue;

		kref_get(&device->kref);
		rcu_read_unlock();
		do_device_work(device, todo);
		kref_put(&device->kref, drbd_destroy_device);
		rcu_read_lock();
	}
	rcu_read_unlock();
}

static bool dequeue_work_batch(struct drbd_work_queue *queue, struct list_head *work_list)
{
	spin_lock_irq(&queue->q_lock);
	list_splice_tail_init(&queue->q, work_list);
	spin_unlock_irq(&queue->q_lock);
	return !list_empty(work_list);
}

static void wait_for_work(struct drbd_connection *connection, struct list_head *work_list)
{
	DEFINE_WAIT(wait);
	struct net_conf *nc;
	int uncork, cork;

	dequeue_work_batch(&connection->sender_work, work_list);
	if (!list_empty(work_list))
		return;

	/* Still nothing to do?
	 * Maybe we still need to close the current epoch,
	 * even if no new requests are queued yet.
	 *
	 * Also, poke TCP, just in case.
	 * Then wait for new work (or signal). */
	rcu_read_lock();
	nc = rcu_dereference(connection->net_conf);
	uncork = nc ? nc->tcp_cork : 0;
	rcu_read_unlock();
	if (uncork) {
		mutex_lock(&connection->data.mutex);
		if (connection->data.socket)
			tcp_sock_set_cork(connection->data.socket->sk, false);
		mutex_unlock(&connection->data.mutex);
	}

	for (;;) {
		int send_barrier;
		prepare_to_wait(&connection->sender_work.q_wait, &wait, TASK_INTERRUPTIBLE);
		spin_lock_irq(&connection->resource->req_lock);
		spin_lock(&connection->sender_work.q_lock);	/* FIXME get rid of this one? */
		if (!list_empty(&connection->sender_work.q))
			list_splice_tail_init(&connection->sender_work.q, work_list);
		spin_unlock(&connection->sender_work.q_lock);	/* FIXME get rid of this one? */
		if (!list_empty(work_list) || signal_pending(current)) {
			spin_unlock_irq(&connection->resource->req_lock);
			break;
		}

		/* We found nothing new to do, no to-be-communicated request,
		 * no other work item.  We may still need to close the last
		 * epoch.  Next incoming request epoch will be connection ->
		 * current transfer log epoch number.  If that is different
		 * from the epoch of the last request we communicated, it is
		 * safe to send the epoch separating barrier now.
		 */
		send_barrier =
			atomic_read(&connection->current_tle_nr) !=
			connection->send.current_epoch_nr;
		spin_unlock_irq(&connection->resource->req_lock);

		if (send_barrier)
			maybe_send_barrier(connection,
					connection->send.current_epoch_nr + 1);

		if (test_bit(DEVICE_WORK_PENDING, &connection->flags))
			break;

		/* drbd_send() may have called flush_signals() */
		if (get_t_state(&connection->worker) != RUNNING)
			break;

		schedule();
		/* may be woken up for other things but new work, too,
		 * e.g. if the current epoch got closed.
		 * In which case we send the barrier above. */
	}
	finish_wait(&connection->sender_work.q_wait, &wait);

	/* someone may have changed the config while we have been waiting above. */
	rcu_read_lock();
	nc = rcu_dereference(connection->net_conf);
	cork = nc ? nc->tcp_cork : 0;
	rcu_read_unlock();
	mutex_lock(&connection->data.mutex);
	if (connection->data.socket) {
		if (cork)
			tcp_sock_set_cork(connection->data.socket->sk, true);
		else if (!uncork)
			tcp_sock_set_cork(connection->data.socket->sk, false);
	}
	mutex_unlock(&connection->data.mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int drbd_worker(struct drbd_thread *thi)
{
<<<<<<< HEAD
	struct drbd_conf *mdev = thi->mdev;
	struct drbd_work *w = NULL;
	LIST_HEAD(work_list);
	int intr = 0, i;

	sprintf(current->comm, "drbd%d_worker", mdev_to_minor(mdev));

	while (get_t_state(thi) == Running) {
		drbd_thread_current_set_cpu(mdev);

		if (down_trylock(&mdev->data.work.s)) {
			mutex_lock(&mdev->data.mutex);
			if (mdev->data.socket && !mdev->net_conf->no_cork)
				drbd_tcp_uncork(mdev->data.socket);
			mutex_unlock(&mdev->data.mutex);

			intr = down_interruptible(&mdev->data.work.s);

			mutex_lock(&mdev->data.mutex);
			if (mdev->data.socket  && !mdev->net_conf->no_cork)
				drbd_tcp_cork(mdev->data.socket);
			mutex_unlock(&mdev->data.mutex);
		}

		if (intr) {
			D_ASSERT(intr == -EINTR);
			flush_signals(current);
			ERR_IF (get_t_state(thi) == Running)
				continue;
			break;
		}

		if (get_t_state(thi) != Running)
			break;
		/* With this break, we have done a down() but not consumed
		   the entry from the list. The cleanup code takes care of
		   this...   */

		w = NULL;
		spin_lock_irq(&mdev->data.work.q_lock);
		ERR_IF(list_empty(&mdev->data.work.q)) {
			/* something terribly wrong in our logic.
			 * we were able to down() the semaphore,
			 * but the list is empty... doh.
			 *
			 * what is the best thing to do now?
			 * try again from scratch, restarting the receiver,
			 * asender, whatnot? could break even more ugly,
			 * e.g. when we are primary, but no good local data.
			 *
			 * I'll try to get away just starting over this loop.
			 */
			spin_unlock_irq(&mdev->data.work.q_lock);
			continue;
		}
		w = list_entry(mdev->data.work.q.next, struct drbd_work, list);
		list_del_init(&w->list);
		spin_unlock_irq(&mdev->data.work.q_lock);

		if (!w->cb(mdev, w, mdev->state.conn < C_CONNECTED)) {
			/* dev_warn(DEV, "worker: a callback failed! \n"); */
			if (mdev->state.conn >= C_CONNECTED)
				drbd_force_state(mdev,
						NS(conn, C_NETWORK_FAILURE));
		}
	}
	D_ASSERT(test_bit(DEVICE_DYING, &mdev->flags));
	D_ASSERT(test_bit(CONFIG_PENDING, &mdev->flags));

	spin_lock_irq(&mdev->data.work.q_lock);
	i = 0;
	while (!list_empty(&mdev->data.work.q)) {
		list_splice_init(&mdev->data.work.q, &work_list);
		spin_unlock_irq(&mdev->data.work.q_lock);

		while (!list_empty(&work_list)) {
			w = list_entry(work_list.next, struct drbd_work, list);
			list_del_init(&w->list);
			w->cb(mdev, w, 1);
			i++; /* dead debugging code */
		}

		spin_lock_irq(&mdev->data.work.q_lock);
	}
	sema_init(&mdev->data.work.s, 0);
	/* DANGEROUS race: if someone did queue his work within the spinlock,
	 * but up() ed outside the spinlock, we could get an up() on the
	 * semaphore without corresponding list entry.
	 * So don't do that.
	 */
	spin_unlock_irq(&mdev->data.work.q_lock);

	D_ASSERT(mdev->state.disk == D_DISKLESS && mdev->state.conn == C_STANDALONE);
	/* _drbd_set_state only uses stop_nowait.
	 * wait here for the Exiting receiver. */
	drbd_thread_stop(&mdev->receiver);
	drbd_mdev_cleanup(mdev);

	dev_info(DEV, "worker terminated\n");

	clear_bit(DEVICE_DYING, &mdev->flags);
	clear_bit(CONFIG_PENDING, &mdev->flags);
	wake_up(&mdev->state_wait);
=======
	struct drbd_connection *connection = thi->connection;
	struct drbd_work *w = NULL;
	struct drbd_peer_device *peer_device;
	LIST_HEAD(work_list);
	int vnr;

	while (get_t_state(thi) == RUNNING) {
		drbd_thread_current_set_cpu(thi);

		if (list_empty(&work_list)) {
			update_worker_timing_details(connection, wait_for_work);
			wait_for_work(connection, &work_list);
		}

		if (test_and_clear_bit(DEVICE_WORK_PENDING, &connection->flags)) {
			update_worker_timing_details(connection, do_unqueued_work);
			do_unqueued_work(connection);
		}

		if (signal_pending(current)) {
			flush_signals(current);
			if (get_t_state(thi) == RUNNING) {
				drbd_warn(connection, "Worker got an unexpected signal\n");
				continue;
			}
			break;
		}

		if (get_t_state(thi) != RUNNING)
			break;

		if (!list_empty(&work_list)) {
			w = list_first_entry(&work_list, struct drbd_work, list);
			list_del_init(&w->list);
			update_worker_timing_details(connection, w->cb);
			if (w->cb(w, connection->cstate < C_WF_REPORT_PARAMS) == 0)
				continue;
			if (connection->cstate >= C_WF_REPORT_PARAMS)
				conn_request_state(connection, NS(conn, C_NETWORK_FAILURE), CS_HARD);
		}
	}

	do {
		if (test_and_clear_bit(DEVICE_WORK_PENDING, &connection->flags)) {
			update_worker_timing_details(connection, do_unqueued_work);
			do_unqueued_work(connection);
		}
		if (!list_empty(&work_list)) {
			w = list_first_entry(&work_list, struct drbd_work, list);
			list_del_init(&w->list);
			update_worker_timing_details(connection, w->cb);
			w->cb(w, 1);
		} else
			dequeue_work_batch(&connection->sender_work, &work_list);
	} while (!list_empty(&work_list) || test_bit(DEVICE_WORK_PENDING, &connection->flags));

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		D_ASSERT(device, device->state.disk == D_DISKLESS && device->state.conn == C_STANDALONE);
		kref_get(&device->kref);
		rcu_read_unlock();
		drbd_device_cleanup(device);
		kref_put(&device->kref, drbd_destroy_device);
		rcu_read_lock();
	}
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
