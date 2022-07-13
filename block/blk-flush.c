<<<<<<< HEAD
/*
 * Functions to sequence FLUSH and FUA writes.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Functions to sequence PREFLUSH and FUA writes.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Copyright (C) 2011		Max Planck Institute for Gravitational Physics
 * Copyright (C) 2011		Tejun Heo <tj@kernel.org>
 *
<<<<<<< HEAD
 * This file is released under the GPLv2.
 *
 * REQ_{FLUSH|FUA} requests are decomposed to sequences consisted of three
 * optional steps - PREFLUSH, DATA and POSTFLUSH - according to the request
 * properties and hardware capability.
 *
 * If a request doesn't have data, only REQ_FLUSH makes sense, which
 * indicates a simple flush request.  If there is data, REQ_FLUSH indicates
=======
 * REQ_{PREFLUSH|FUA} requests are decomposed to sequences consisted of three
 * optional steps - PREFLUSH, DATA and POSTFLUSH - according to the request
 * properties and hardware capability.
 *
 * If a request doesn't have data, only REQ_PREFLUSH makes sense, which
 * indicates a simple flush request.  If there is data, REQ_PREFLUSH indicates
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * that the device cache should be flushed before the data is executed, and
 * REQ_FUA means that the data must be on non-volatile media on request
 * completion.
 *
<<<<<<< HEAD
 * If the device doesn't have writeback cache, FLUSH and FUA don't make any
 * difference.  The requests are either completed immediately if there's no
 * data or executed as normal requests otherwise.
 *
 * If the device has writeback cache and supports FUA, REQ_FLUSH is
 * translated to PREFLUSH but REQ_FUA is passed down directly with DATA.
 *
 * If the device has writeback cache and doesn't support FUA, REQ_FLUSH is
 * translated to PREFLUSH and REQ_FUA to POSTFLUSH.
 *
 * The actual execution of flush is double buffered.  Whenever a request
 * needs to execute PRE or POSTFLUSH, it queues at
 * q->flush_queue[q->flush_pending_idx].  Once certain criteria are met, a
 * flush is issued and the pending_idx is toggled.  When the flush
 * completes, all the requests which were pending are proceeded to the next
 * step.  This allows arbitrary merging of different types of FLUSH/FUA
=======
 * If the device doesn't have writeback cache, PREFLUSH and FUA don't make any
 * difference.  The requests are either completed immediately if there's no data
 * or executed as normal requests otherwise.
 *
 * If the device has writeback cache and supports FUA, REQ_PREFLUSH is
 * translated to PREFLUSH but REQ_FUA is passed down directly with DATA.
 *
 * If the device has writeback cache and doesn't support FUA, REQ_PREFLUSH
 * is translated to PREFLUSH and REQ_FUA to POSTFLUSH.
 *
 * The actual execution of flush is double buffered.  Whenever a request
 * needs to execute PRE or POSTFLUSH, it queues at
 * fq->flush_queue[fq->flush_pending_idx].  Once certain criteria are met, a
 * REQ_OP_FLUSH is issued and the pending_idx is toggled.  When the flush
 * completes, all the requests which were pending are proceeded to the next
 * step.  This allows arbitrary merging of different types of PREFLUSH/FUA
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * requests.
 *
 * Currently, the following conditions are used to determine when to issue
 * flush.
 *
 * C1. At any given time, only one flush shall be in progress.  This makes
 *     double buffering sufficient.
 *
 * C2. Flush is deferred if any request is executing DATA of its sequence.
 *     This avoids issuing separate POSTFLUSHes for requests which shared
 *     PREFLUSH.
 *
 * C3. The second condition is ignored if there is a request which has
 *     waited longer than FLUSH_PENDING_TIMEOUT.  This is to avoid
 *     starvation in the unlikely case where there are continuous stream of
<<<<<<< HEAD
 *     FUA (without FLUSH) requests.
=======
 *     FUA (without PREFLUSH) requests.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * For devices which support FUA, it isn't clear whether C2 (and thus C3)
 * is beneficial.
 *
<<<<<<< HEAD
 * Note that a sequenced FLUSH/FUA request with DATA is completed twice.
 * Once while executing DATA and again after the whole sequence is
 * complete.  The first completion updates the contained bio but doesn't
 * finish it so that the bio submitter is notified only after the whole
 * sequence is complete.  This is implemented by testing REQ_FLUSH_SEQ in
 * req_bio_endio().
 *
 * The above peculiarity requires that each FLUSH/FUA request has only one
=======
 * Note that a sequenced PREFLUSH/FUA request with DATA is completed twice.
 * Once while executing DATA and again after the whole sequence is
 * complete.  The first completion updates the contained bio but doesn't
 * finish it so that the bio submitter is notified only after the whole
 * sequence is complete.  This is implemented by testing RQF_FLUSH_SEQ in
 * req_bio_endio().
 *
 * The above peculiarity requires that each PREFLUSH/FUA request has only one
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * bio attached to it, which is guaranteed as they aren't allowed to be
 * merged in the usual way.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/gfp.h>
<<<<<<< HEAD

#include "blk.h"

/* FLUSH/FUA sequences */
=======
#include <linux/part_stat.h>

#include "blk.h"
#include "blk-mq.h"
#include "blk-mq-sched.h"

/* PREFLUSH/FUA sequences */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
enum {
	REQ_FSEQ_PREFLUSH	= (1 << 0), /* pre-flushing in progress */
	REQ_FSEQ_DATA		= (1 << 1), /* data write in progress */
	REQ_FSEQ_POSTFLUSH	= (1 << 2), /* post-flushing in progress */
	REQ_FSEQ_DONE		= (1 << 3),

	REQ_FSEQ_ACTIONS	= REQ_FSEQ_PREFLUSH | REQ_FSEQ_DATA |
				  REQ_FSEQ_POSTFLUSH,

	/*
	 * If flush has been pending longer than the following timeout,
	 * it's issued even if flush_data requests are still in flight.
	 */
	FLUSH_PENDING_TIMEOUT	= 5 * HZ,
};

<<<<<<< HEAD
static bool blk_kick_flush(struct request_queue *q);

static unsigned int blk_flush_policy(unsigned int fflags, struct request *rq)
=======
static void blk_kick_flush(struct request_queue *q,
			   struct blk_flush_queue *fq, blk_opf_t flags);

static inline struct blk_flush_queue *
blk_get_flush_queue(struct request_queue *q, struct blk_mq_ctx *ctx)
{
	return blk_mq_map_queue(q, REQ_OP_FLUSH, ctx)->fq;
}

static unsigned int blk_flush_policy(unsigned long fflags, struct request *rq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int policy = 0;

	if (blk_rq_sectors(rq))
		policy |= REQ_FSEQ_DATA;

<<<<<<< HEAD
	if (fflags & REQ_FLUSH) {
		if (rq->cmd_flags & REQ_FLUSH)
			policy |= REQ_FSEQ_PREFLUSH;
		if (!(fflags & REQ_FUA) && (rq->cmd_flags & REQ_FUA))
=======
	if (fflags & (1UL << QUEUE_FLAG_WC)) {
		if (rq->cmd_flags & REQ_PREFLUSH)
			policy |= REQ_FSEQ_PREFLUSH;
		if (!(fflags & (1UL << QUEUE_FLAG_FUA)) &&
		    (rq->cmd_flags & REQ_FUA))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			policy |= REQ_FSEQ_POSTFLUSH;
	}
	return policy;
}

static unsigned int blk_flush_cur_seq(struct request *rq)
{
	return 1 << ffz(rq->flush.seq);
}

static void blk_flush_restore_request(struct request *rq)
{
	/*
	 * After flush data completion, @rq->bio is %NULL but we need to
	 * complete the bio again.  @rq->biotail is guaranteed to equal the
	 * original @rq->bio.  Restore it.
	 */
	rq->bio = rq->biotail;

	/* make @rq a normal request */
<<<<<<< HEAD
	rq->cmd_flags &= ~REQ_FLUSH_SEQ;
	rq->end_io = rq->flush.saved_end_io;
}

/**
 * blk_flush_complete_seq - complete flush sequence
 * @rq: FLUSH/FUA request being sequenced
=======
	rq->rq_flags &= ~RQF_FLUSH_SEQ;
	rq->end_io = rq->flush.saved_end_io;
}

static void blk_account_io_flush(struct request *rq)
{
	struct block_device *part = rq->q->disk->part0;

	part_stat_lock();
	part_stat_inc(part, ios[STAT_FLUSH]);
	part_stat_add(part, nsecs[STAT_FLUSH],
		      blk_time_get_ns() - rq->start_time_ns);
	part_stat_unlock();
}

/**
 * blk_flush_complete_seq - complete flush sequence
 * @rq: PREFLUSH/FUA request being sequenced
 * @fq: flush queue
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @seq: sequences to complete (mask of %REQ_FSEQ_*, can be zero)
 * @error: whether an error occurred
 *
 * @rq just completed @seq part of its flush sequence, record the
 * completion and trigger the next step.
 *
 * CONTEXT:
<<<<<<< HEAD
 * spin_lock_irq(q->queue_lock)
 *
 * RETURNS:
 * %true if requests were added to the dispatch queue, %false otherwise.
 */
static bool blk_flush_complete_seq(struct request *rq, unsigned int seq,
				   int error)
{
	struct request_queue *q = rq->q;
	struct list_head *pending = &q->flush_queue[q->flush_pending_idx];
	bool queued = false;

	BUG_ON(rq->flush.seq & seq);
	rq->flush.seq |= seq;
=======
 * spin_lock_irq(fq->mq_flush_lock)
 */
static void blk_flush_complete_seq(struct request *rq,
				   struct blk_flush_queue *fq,
				   unsigned int seq, blk_status_t error)
{
	struct request_queue *q = rq->q;
	struct list_head *pending = &fq->flush_queue[fq->flush_pending_idx];
	blk_opf_t cmd_flags;

	BUG_ON(rq->flush.seq & seq);
	rq->flush.seq |= seq;
	cmd_flags = rq->cmd_flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (likely(!error))
		seq = blk_flush_cur_seq(rq);
	else
		seq = REQ_FSEQ_DONE;

	switch (seq) {
	case REQ_FSEQ_PREFLUSH:
	case REQ_FSEQ_POSTFLUSH:
		/* queue for flush */
		if (list_empty(pending))
<<<<<<< HEAD
			q->flush_pending_since = jiffies;
		list_move_tail(&rq->flush.list, pending);
		break;

	case REQ_FSEQ_DATA:
		list_move_tail(&rq->flush.list, &q->flush_data_in_flight);
		list_add(&rq->queuelist, &q->queue_head);
		queued = true;
=======
			fq->flush_pending_since = jiffies;
		list_move_tail(&rq->queuelist, pending);
		break;

	case REQ_FSEQ_DATA:
		fq->flush_data_in_flight++;
		spin_lock(&q->requeue_lock);
		list_move(&rq->queuelist, &q->requeue_list);
		spin_unlock(&q->requeue_lock);
		blk_mq_kick_requeue_list(q);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case REQ_FSEQ_DONE:
		/*
<<<<<<< HEAD
		 * @rq was previously adjusted by blk_flush_issue() for
=======
		 * @rq was previously adjusted by blk_insert_flush() for
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * flush sequencing and may already have gone through the
		 * flush data request completion path.  Restore @rq for
		 * normal completion and end it.
		 */
<<<<<<< HEAD
		BUG_ON(!list_empty(&rq->queuelist));
		list_del_init(&rq->flush.list);
		blk_flush_restore_request(rq);
		__blk_end_request_all(rq, error);
=======
		list_del_init(&rq->queuelist);
		blk_flush_restore_request(rq);
		blk_mq_end_request(rq, error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	default:
		BUG();
	}

<<<<<<< HEAD
	return blk_kick_flush(q) | queued;
}

static void flush_end_io(struct request *flush_rq, int error)
{
	struct request_queue *q = flush_rq->q;
	struct list_head *running = &q->flush_queue[q->flush_running_idx];
	bool queued = false;
	struct request *rq, *n;

	BUG_ON(q->flush_pending_idx == q->flush_running_idx);

	/* account completion of the flush request */
	q->flush_running_idx ^= 1;
	elv_completed_request(q, flush_rq);

	/* and push the waiting requests to the next stage */
	list_for_each_entry_safe(rq, n, running, flush.list) {
		unsigned int seq = blk_flush_cur_seq(rq);

		BUG_ON(seq != REQ_FSEQ_PREFLUSH && seq != REQ_FSEQ_POSTFLUSH);
		queued |= blk_flush_complete_seq(rq, seq, error);
	}

	/*
	 * Kick the queue to avoid stall for two cases:
	 * 1. Moving a request silently to empty queue_head may stall the
	 * queue.
	 * 2. When flush request is running in non-queueable queue, the
	 * queue is hold. Restart the queue after flush request is finished
	 * to avoid stall.
	 * This function is called from request completion path and calling
	 * directly into request_fn may confuse the driver.  Always use
	 * kblockd.
	 */
	if (queued || q->flush_queue_delayed)
		blk_run_queue_async(q);
	q->flush_queue_delayed = 0;
=======
	blk_kick_flush(q, fq, cmd_flags);
}

static enum rq_end_io_ret flush_end_io(struct request *flush_rq,
				       blk_status_t error)
{
	struct request_queue *q = flush_rq->q;
	struct list_head *running;
	struct request *rq, *n;
	unsigned long flags = 0;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, flush_rq->mq_ctx);

	/* release the tag's ownership to the req cloned from */
	spin_lock_irqsave(&fq->mq_flush_lock, flags);

	if (!req_ref_put_and_test(flush_rq)) {
		fq->rq_status = error;
		spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
		return RQ_END_IO_NONE;
	}

	blk_account_io_flush(flush_rq);
	/*
	 * Flush request has to be marked as IDLE when it is really ended
	 * because its .end_io() is called from timeout code path too for
	 * avoiding use-after-free.
	 */
	WRITE_ONCE(flush_rq->state, MQ_RQ_IDLE);
	if (fq->rq_status != BLK_STS_OK) {
		error = fq->rq_status;
		fq->rq_status = BLK_STS_OK;
	}

	if (!q->elevator) {
		flush_rq->tag = BLK_MQ_NO_TAG;
	} else {
		blk_mq_put_driver_tag(flush_rq);
		flush_rq->internal_tag = BLK_MQ_NO_TAG;
	}

	running = &fq->flush_queue[fq->flush_running_idx];
	BUG_ON(fq->flush_pending_idx == fq->flush_running_idx);

	/* account completion of the flush request */
	fq->flush_running_idx ^= 1;

	/* and push the waiting requests to the next stage */
	list_for_each_entry_safe(rq, n, running, queuelist) {
		unsigned int seq = blk_flush_cur_seq(rq);

		BUG_ON(seq != REQ_FSEQ_PREFLUSH && seq != REQ_FSEQ_POSTFLUSH);
		blk_flush_complete_seq(rq, fq, seq, error);
	}

	spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
	return RQ_END_IO_NONE;
}

bool is_flush_rq(struct request *rq)
{
	return rq->end_io == flush_end_io;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * blk_kick_flush - consider issuing flush request
 * @q: request_queue being kicked
<<<<<<< HEAD
=======
 * @fq: flush queue
 * @flags: cmd_flags of the original request
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Flush related states of @q have changed, consider issuing flush request.
 * Please read the comment at the top of this file for more info.
 *
 * CONTEXT:
<<<<<<< HEAD
 * spin_lock_irq(q->queue_lock)
 *
 * RETURNS:
 * %true if flush was issued, %false otherwise.
 */
static bool blk_kick_flush(struct request_queue *q)
{
	struct list_head *pending = &q->flush_queue[q->flush_pending_idx];
	struct request *first_rq =
		list_first_entry(pending, struct request, flush.list);

	/* C1 described at the top of this file */
	if (q->flush_pending_idx != q->flush_running_idx || list_empty(pending))
		return false;

	/* C2 and C3 */
	if (!list_empty(&q->flush_data_in_flight) &&
	    time_before(jiffies,
			q->flush_pending_since + FLUSH_PENDING_TIMEOUT))
		return false;
=======
 * spin_lock_irq(fq->mq_flush_lock)
 *
 */
static void blk_kick_flush(struct request_queue *q, struct blk_flush_queue *fq,
			   blk_opf_t flags)
{
	struct list_head *pending = &fq->flush_queue[fq->flush_pending_idx];
	struct request *first_rq =
		list_first_entry(pending, struct request, queuelist);
	struct request *flush_rq = fq->flush_rq;

	/* C1 described at the top of this file */
	if (fq->flush_pending_idx != fq->flush_running_idx || list_empty(pending))
		return;

	/* C2 and C3 */
	if (fq->flush_data_in_flight &&
	    time_before(jiffies,
			fq->flush_pending_since + FLUSH_PENDING_TIMEOUT))
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Issue flush and toggle pending_idx.  This makes pending_idx
	 * different from running_idx, which means flush is in flight.
	 */
<<<<<<< HEAD
	blk_rq_init(q, &q->flush_rq);
	q->flush_rq.cmd_type = REQ_TYPE_FS;
	q->flush_rq.cmd_flags = WRITE_FLUSH | REQ_FLUSH_SEQ;
	q->flush_rq.rq_disk = first_rq->rq_disk;
	q->flush_rq.end_io = flush_end_io;

	q->flush_pending_idx ^= 1;
	list_add_tail(&q->flush_rq.queuelist, &q->queue_head);
	return true;
}

static void flush_data_end_io(struct request *rq, int error)
{
	struct request_queue *q = rq->q;
=======
	fq->flush_pending_idx ^= 1;

	blk_rq_init(q, flush_rq);

	/*
	 * In case of none scheduler, borrow tag from the first request
	 * since they can't be in flight at the same time. And acquire
	 * the tag's ownership for flush req.
	 *
	 * In case of IO scheduler, flush rq need to borrow scheduler tag
	 * just for cheating put/get driver tag.
	 */
	flush_rq->mq_ctx = first_rq->mq_ctx;
	flush_rq->mq_hctx = first_rq->mq_hctx;

	if (!q->elevator)
		flush_rq->tag = first_rq->tag;
	else
		flush_rq->internal_tag = first_rq->internal_tag;

	flush_rq->cmd_flags = REQ_OP_FLUSH | REQ_PREFLUSH;
	flush_rq->cmd_flags |= (flags & REQ_DRV) | (flags & REQ_FAILFAST_MASK);
	flush_rq->rq_flags |= RQF_FLUSH_SEQ;
	flush_rq->end_io = flush_end_io;
	/*
	 * Order WRITE ->end_io and WRITE rq->ref, and its pair is the one
	 * implied in refcount_inc_not_zero() called from
	 * blk_mq_find_and_get_req(), which orders WRITE/READ flush_rq->ref
	 * and READ flush_rq->end_io
	 */
	smp_wmb();
	req_ref_set(flush_rq, 1);

	spin_lock(&q->requeue_lock);
	list_add_tail(&flush_rq->queuelist, &q->flush_list);
	spin_unlock(&q->requeue_lock);

	blk_mq_kick_requeue_list(q);
}

static enum rq_end_io_ret mq_flush_data_end_io(struct request *rq,
					       blk_status_t error)
{
	struct request_queue *q = rq->q;
	struct blk_mq_hw_ctx *hctx = rq->mq_hctx;
	struct blk_mq_ctx *ctx = rq->mq_ctx;
	unsigned long flags;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, ctx);

	if (q->elevator) {
		WARN_ON(rq->tag < 0);
		blk_mq_put_driver_tag(rq);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * After populating an empty queue, kick it to avoid stall.  Read
	 * the comment in flush_end_io().
	 */
<<<<<<< HEAD
	if (blk_flush_complete_seq(rq, REQ_FSEQ_DATA, error))
		blk_run_queue_async(q);
}

/**
 * blk_insert_flush - insert a new FLUSH/FUA request
 * @rq: request to insert
 *
 * To be called from __elv_add_request() for %ELEVATOR_INSERT_FLUSH insertions.
 * @rq is being submitted.  Analyze what needs to be done and put it on the
 * right queue.
 *
 * CONTEXT:
 * spin_lock_irq(q->queue_lock)
 */
void blk_insert_flush(struct request *rq)
{
	struct request_queue *q = rq->q;
	unsigned int fflags = q->flush_flags;	/* may change, cache */
	unsigned int policy = blk_flush_policy(fflags, rq);

	/*
	 * @policy now records what operations need to be done.  Adjust
	 * REQ_FLUSH and FUA for the driver.
	 */
	rq->cmd_flags &= ~REQ_FLUSH;
	if (!(fflags & REQ_FUA))
		rq->cmd_flags &= ~REQ_FUA;

	/*
	 * An empty flush handed down from a stacking driver may
	 * translate into nothing if the underlying device does not
	 * advertise a write-back cache.  In this case, simply
	 * complete the request.
	 */
	if (!policy) {
		__blk_end_bidi_request(rq, 0, 0, 0);
		return;
	}

	BUG_ON(rq->bio != rq->biotail); /*assumes zero or single bio rq */

	/*
	 * If there's data but flush is not necessary, the request can be
	 * processed directly without going through flush machinery.  Queue
	 * for normal execution.
	 */
	if ((policy & REQ_FSEQ_DATA) &&
	    !(policy & (REQ_FSEQ_PREFLUSH | REQ_FSEQ_POSTFLUSH))) {
		list_add_tail(&rq->queuelist, &q->queue_head);
		return;
	}

	/*
	 * @rq should go through flush machinery.  Mark it part of flush
	 * sequence and submit for further processing.
	 */
	memset(&rq->flush, 0, sizeof(rq->flush));
	INIT_LIST_HEAD(&rq->flush.list);
	rq->cmd_flags |= REQ_FLUSH_SEQ;
	rq->flush.saved_end_io = rq->end_io; /* Usually NULL */
	rq->end_io = flush_data_end_io;

	blk_flush_complete_seq(rq, REQ_FSEQ_ACTIONS & ~policy, 0);
}

/**
 * blk_abort_flushes - @q is being aborted, abort flush requests
 * @q: request_queue being aborted
 *
 * To be called from elv_abort_queue().  @q is being aborted.  Prepare all
 * FLUSH/FUA requests for abortion.
 *
 * CONTEXT:
 * spin_lock_irq(q->queue_lock)
 */
void blk_abort_flushes(struct request_queue *q)
{
	struct request *rq, *n;
	int i;

	/*
	 * Requests in flight for data are already owned by the dispatch
	 * queue or the device driver.  Just restore for normal completion.
	 */
	list_for_each_entry_safe(rq, n, &q->flush_data_in_flight, flush.list) {
		list_del_init(&rq->flush.list);
		blk_flush_restore_request(rq);
	}

	/*
	 * We need to give away requests on flush queues.  Restore for
	 * normal completion and put them on the dispatch queue.
	 */
	for (i = 0; i < ARRAY_SIZE(q->flush_queue); i++) {
		list_for_each_entry_safe(rq, n, &q->flush_queue[i],
					 flush.list) {
			list_del_init(&rq->flush.list);
			blk_flush_restore_request(rq);
			list_add_tail(&rq->queuelist, &q->queue_head);
		}
	}
}

static void bio_end_flush(struct bio *bio, int err)
{
	if (err)
		clear_bit(BIO_UPTODATE, &bio->bi_flags);
	if (bio->bi_private)
		complete(bio->bi_private);
	bio_put(bio);
=======
	spin_lock_irqsave(&fq->mq_flush_lock, flags);
	fq->flush_data_in_flight--;
	/*
	 * May have been corrupted by rq->rq_next reuse, we need to
	 * re-initialize rq->queuelist before reusing it here.
	 */
	INIT_LIST_HEAD(&rq->queuelist);
	blk_flush_complete_seq(rq, fq, REQ_FSEQ_DATA, error);
	spin_unlock_irqrestore(&fq->mq_flush_lock, flags);

	blk_mq_sched_restart(hctx);
	return RQ_END_IO_NONE;
}

static void blk_rq_init_flush(struct request *rq)
{
	rq->flush.seq = 0;
	rq->rq_flags |= RQF_FLUSH_SEQ;
	rq->flush.saved_end_io = rq->end_io; /* Usually NULL */
	rq->end_io = mq_flush_data_end_io;
}

/*
 * Insert a PREFLUSH/FUA request into the flush state machine.
 * Returns true if the request has been consumed by the flush state machine,
 * or false if the caller should continue to process it.
 */
bool blk_insert_flush(struct request *rq)
{
	struct request_queue *q = rq->q;
	unsigned long fflags = q->queue_flags;	/* may change, cache */
	unsigned int policy = blk_flush_policy(fflags, rq);
	struct blk_flush_queue *fq = blk_get_flush_queue(q, rq->mq_ctx);

	/* FLUSH/FUA request must never be merged */
	WARN_ON_ONCE(rq->bio != rq->biotail);

	/*
	 * @policy now records what operations need to be done.  Adjust
	 * REQ_PREFLUSH and FUA for the driver.
	 */
	rq->cmd_flags &= ~REQ_PREFLUSH;
	if (!(fflags & (1UL << QUEUE_FLAG_FUA)))
		rq->cmd_flags &= ~REQ_FUA;

	/*
	 * REQ_PREFLUSH|REQ_FUA implies REQ_SYNC, so if we clear any
	 * of those flags, we have to set REQ_SYNC to avoid skewing
	 * the request accounting.
	 */
	rq->cmd_flags |= REQ_SYNC;

	switch (policy) {
	case 0:
		/*
		 * An empty flush handed down from a stacking driver may
		 * translate into nothing if the underlying device does not
		 * advertise a write-back cache.  In this case, simply
		 * complete the request.
		 */
		blk_mq_end_request(rq, 0);
		return true;
	case REQ_FSEQ_DATA:
		/*
		 * If there's data, but no flush is necessary, the request can
		 * be processed directly without going through flush machinery.
		 * Queue for normal execution.
		 */
		return false;
	case REQ_FSEQ_DATA | REQ_FSEQ_POSTFLUSH:
		/*
		 * Initialize the flush fields and completion handler to trigger
		 * the post flush, and then just pass the command on.
		 */
		blk_rq_init_flush(rq);
		rq->flush.seq |= REQ_FSEQ_PREFLUSH;
		spin_lock_irq(&fq->mq_flush_lock);
		fq->flush_data_in_flight++;
		spin_unlock_irq(&fq->mq_flush_lock);
		return false;
	default:
		/*
		 * Mark the request as part of a flush sequence and submit it
		 * for further processing to the flush state machine.
		 */
		blk_rq_init_flush(rq);
		spin_lock_irq(&fq->mq_flush_lock);
		blk_flush_complete_seq(rq, fq, REQ_FSEQ_ACTIONS & ~policy, 0);
		spin_unlock_irq(&fq->mq_flush_lock);
		return true;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * blkdev_issue_flush - queue a flush
 * @bdev:	blockdev to issue flush for
<<<<<<< HEAD
 * @gfp_mask:	memory allocation flags (for bio_alloc)
 * @error_sector:	error sector
 *
 * Description:
 *    Issue a flush for the block device in question. Caller can supply
 *    room for storing the error offset in case of a flush error, if they
 *    wish to. If WAIT flag is not passed then caller may check only what
 *    request was pushed in some internal queue for later handling.
 */
int blkdev_issue_flush(struct block_device *bdev, gfp_t gfp_mask,
		sector_t *error_sector)
{
	DECLARE_COMPLETION_ONSTACK(wait);
	struct request_queue *q;
	struct bio *bio;
	int ret = 0;

	if (bdev->bd_disk == NULL)
		return -ENXIO;

	q = bdev_get_queue(bdev);
	if (!q)
		return -ENXIO;

	/*
	 * some block devices may not have their queue correctly set up here
	 * (e.g. loop device without a backing file) and so issuing a flush
	 * here will panic. Ensure there is a request function before issuing
	 * the flush.
	 */
	if (!q->make_request_fn)
		return -ENXIO;

	bio = bio_alloc(gfp_mask, 0);
	bio->bi_end_io = bio_end_flush;
	bio->bi_bdev = bdev;
	bio->bi_private = &wait;

	bio_get(bio);
	submit_bio(WRITE_FLUSH, bio);
	wait_for_completion(&wait);

	/*
	 * The driver must store the error location in ->bi_sector, if
	 * it supports it. For non-stacked drivers, this should be
	 * copied from blk_rq_pos(rq).
	 */
	if (error_sector)
               *error_sector = bio->bi_sector;

	if (!bio_flagged(bio, BIO_UPTODATE))
		ret = -EIO;

	bio_put(bio);
	return ret;
}
EXPORT_SYMBOL(blkdev_issue_flush);
=======
 *
 * Description:
 *    Issue a flush for the block device in question.
 */
int blkdev_issue_flush(struct block_device *bdev)
{
	struct bio bio;

	bio_init(&bio, bdev, NULL, 0, REQ_OP_WRITE | REQ_PREFLUSH);
	return submit_bio_wait(&bio);
}
EXPORT_SYMBOL(blkdev_issue_flush);

struct blk_flush_queue *blk_alloc_flush_queue(int node, int cmd_size,
					      gfp_t flags)
{
	struct blk_flush_queue *fq;
	int rq_sz = sizeof(struct request);

	fq = kzalloc_node(sizeof(*fq), flags, node);
	if (!fq)
		goto fail;

	spin_lock_init(&fq->mq_flush_lock);

	rq_sz = round_up(rq_sz + cmd_size, cache_line_size());
	fq->flush_rq = kzalloc_node(rq_sz, flags, node);
	if (!fq->flush_rq)
		goto fail_rq;

	INIT_LIST_HEAD(&fq->flush_queue[0]);
	INIT_LIST_HEAD(&fq->flush_queue[1]);

	return fq;

 fail_rq:
	kfree(fq);
 fail:
	return NULL;
}

void blk_free_flush_queue(struct blk_flush_queue *fq)
{
	/* bio based request queue hasn't flush queue */
	if (!fq)
		return;

	kfree(fq->flush_rq);
	kfree(fq);
}

/*
 * Allow driver to set its own lock class to fq->mq_flush_lock for
 * avoiding lockdep complaint.
 *
 * flush_end_io() may be called recursively from some driver, such as
 * nvme-loop, so lockdep may complain 'possible recursive locking' because
 * all 'struct blk_flush_queue' instance share same mq_flush_lock lock class
 * key. We need to assign different lock class for these driver's
 * fq->mq_flush_lock for avoiding the lockdep warning.
 *
 * Use dynamically allocated lock class key for each 'blk_flush_queue'
 * instance is over-kill, and more worse it introduces horrible boot delay
 * issue because synchronize_rcu() is implied in lockdep_unregister_key which
 * is called for each hctx release. SCSI probing may synchronously create and
 * destroy lots of MQ request_queues for non-existent devices, and some robot
 * test kernel always enable lockdep option. It is observed that more than half
 * an hour is taken during SCSI MQ probe with per-fq lock class.
 */
void blk_mq_hctx_set_fq_lock_class(struct blk_mq_hw_ctx *hctx,
		struct lock_class_key *key)
{
	lockdep_set_class(&hctx->fq->mq_flush_lock, key);
}
EXPORT_SYMBOL_GPL(blk_mq_hctx_set_fq_lock_class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
