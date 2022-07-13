<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Block device elevator/IO-scheduler.
 *
 *  Copyright (C) 2000 Andrea Arcangeli <andrea@suse.de> SuSE
 *
 * 30042000 Jens Axboe <axboe@kernel.dk> :
 *
 * Split the elevator a bit so that it is possible to choose a different
 * one or even write a new "plug in". There are three pieces:
 * - elevator_fn, inserts a new request in the queue list
 * - elevator_merge_fn, decides whether a new buffer can be merged with
 *   an existing request
 * - elevator_dequeue_fn, called when a request is taken off the active list
 *
 * 20082000 Dave Jones <davej@suse.de> :
 * Removed tests for max-bomb-segments, which was breaking elvtune
 *  when run without -bN
 *
 * Jens:
 * - Rework again to work with bio instead of buffer_heads
 * - loose bi_dev comparisons, partition handling is right now
 * - completely modularize elevator setup and teardown
 *
 */
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
<<<<<<< HEAD
#include <linux/elevator.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/compiler.h>
#include <linux/blktrace_api.h>
#include <linux/hash.h>
#include <linux/uaccess.h>
#include <linux/pm_runtime.h>

#include <trace/events/block.h>

<<<<<<< HEAD
#include "blk.h"
=======
#include "elevator.h"
#include "blk.h"
#include "blk-mq-sched.h"
#include "blk-pm.h"
#include "blk-wbt.h"
#include "blk-cgroup.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_SPINLOCK(elv_list_lock);
static LIST_HEAD(elv_list);

/*
 * Merge hash stuff.
 */
<<<<<<< HEAD
static const int elv_hash_shift = 6;
#define ELV_HASH_BLOCK(sec)	((sec) >> 3)
#define ELV_HASH_FN(sec)	\
		(hash_long(ELV_HASH_BLOCK((sec)), elv_hash_shift))
#define ELV_HASH_ENTRIES	(1 << elv_hash_shift)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define rq_hash_key(rq)		(blk_rq_pos(rq) + blk_rq_sectors(rq))

/*
 * Query io scheduler to see if the current process issuing bio may be
 * merged with rq.
 */
<<<<<<< HEAD
static int elv_iosched_allow_merge(struct request *rq, struct bio *bio)
=======
static bool elv_iosched_allow_bio_merge(struct request *rq, struct bio *bio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct request_queue *q = rq->q;
	struct elevator_queue *e = q->elevator;

<<<<<<< HEAD
	if (e->type->ops.elevator_allow_merge_fn)
		return e->type->ops.elevator_allow_merge_fn(q, rq, bio);

	return 1;
=======
	if (e->type->ops.allow_merge)
		return e->type->ops.allow_merge(q, rq, bio);

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * can we safely merge with this request?
 */
<<<<<<< HEAD
bool elv_rq_merge_ok(struct request *rq, struct bio *bio)
{
	if (!rq_mergeable(rq))
		return 0;

	/*
	 * Don't merge file system requests and discard requests
	 */
	if ((bio->bi_rw & REQ_DISCARD) != (rq->bio->bi_rw & REQ_DISCARD))
		return 0;

	/*
	 * Don't merge discard requests and secure discard requests
	 */
	if ((bio->bi_rw & REQ_SECURE) != (rq->bio->bi_rw & REQ_SECURE))
		return 0;

	/*
	 * Don't merge sanitize requests
	 */
	if ((bio->bi_rw & REQ_SANITIZE) != (rq->bio->bi_rw & REQ_SANITIZE))
		return 0;

	/*
	 * different data direction or already started, don't merge
	 */
	if (bio_data_dir(bio) != rq_data_dir(rq))
		return 0;

	/*
	 * must be same device and not a special request
	 */
	if (rq->rq_disk != bio->bi_bdev->bd_disk || rq->special)
		return 0;

	/*
	 * only merge integrity protected bio into ditto rq
	 */
	if (bio_integrity(bio) != blk_integrity_rq(rq))
		return 0;

	if (!elv_iosched_allow_merge(rq, bio))
		return 0;

	return 1;
}
EXPORT_SYMBOL(elv_rq_merge_ok);

static struct elevator_type *elevator_find(const char *name)
{
	struct elevator_type *e;

	list_for_each_entry(e, &elv_list, list) {
		if (!strcmp(e->elevator_name, name))
			return e;
	}

	return NULL;
}

static void elevator_put(struct elevator_type *e)
{
	module_put(e->elevator_owner);
}

static struct elevator_type *elevator_get(const char *name)
=======
bool elv_bio_merge_ok(struct request *rq, struct bio *bio)
{
	if (!blk_rq_merge_ok(rq, bio))
		return false;

	if (!elv_iosched_allow_bio_merge(rq, bio))
		return false;

	return true;
}
EXPORT_SYMBOL(elv_bio_merge_ok);

static inline bool elv_support_features(struct request_queue *q,
		const struct elevator_type *e)
{
	return (q->required_elevator_features & e->elevator_features) ==
		q->required_elevator_features;
}

/**
 * elevator_match - Check whether @e's name or alias matches @name
 * @e: Scheduler to test
 * @name: Elevator name to test
 *
 * Return true if the elevator @e's name or alias matches @name.
 */
static bool elevator_match(const struct elevator_type *e, const char *name)
{
	return !strcmp(e->elevator_name, name) ||
		(e->elevator_alias && !strcmp(e->elevator_alias, name));
}

static struct elevator_type *__elevator_find(const char *name)
{
	struct elevator_type *e;

	list_for_each_entry(e, &elv_list, list)
		if (elevator_match(e, name))
			return e;
	return NULL;
}

static struct elevator_type *elevator_find_get(struct request_queue *q,
		const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct elevator_type *e;

	spin_lock(&elv_list_lock);
<<<<<<< HEAD

	e = elevator_find(name);
	if (!e) {
		spin_unlock(&elv_list_lock);
		request_module("%s-iosched", name);
		spin_lock(&elv_list_lock);
		e = elevator_find(name);
	}

	if (e && !try_module_get(e->elevator_owner))
		e = NULL;

	spin_unlock(&elv_list_lock);

	return e;
}

static int elevator_init_queue(struct request_queue *q,
			       struct elevator_queue *eq)
{
	eq->elevator_data = eq->type->ops.elevator_init_fn(q);
	if (eq->elevator_data)
		return 0;
	return -ENOMEM;
}

static char chosen_elevator[ELV_NAME_MAX];

static int __init elevator_setup(char *str)
{
	/*
	 * Be backwards-compatible with previous kernels, so users
	 * won't get the wrong elevator.
	 */
	strncpy(chosen_elevator, str, sizeof(chosen_elevator) - 1);
	return 1;
}

__setup("elevator=", elevator_setup);

static struct kobj_type elv_ktype;

static struct elevator_queue *elevator_alloc(struct request_queue *q,
				  struct elevator_type *e)
{
	struct elevator_queue *eq;
	int i;

	eq = kmalloc_node(sizeof(*eq), GFP_KERNEL | __GFP_ZERO, q->node);
	if (unlikely(!eq))
		goto err;

	eq->type = e;
	kobject_init(&eq->kobj, &elv_ktype);
	mutex_init(&eq->sysfs_lock);

	eq->hash = kmalloc_node(sizeof(struct hlist_head) * ELV_HASH_ENTRIES,
					GFP_KERNEL, q->node);
	if (!eq->hash)
		goto err;

	for (i = 0; i < ELV_HASH_ENTRIES; i++)
		INIT_HLIST_HEAD(&eq->hash[i]);

	return eq;
err:
	kfree(eq);
	elevator_put(e);
	return NULL;
}
=======
	e = __elevator_find(name);
	if (e && (!elv_support_features(q, e) || !elevator_tryget(e)))
		e = NULL;
	spin_unlock(&elv_list_lock);
	return e;
}

static const struct kobj_type elv_ktype;

struct elevator_queue *elevator_alloc(struct request_queue *q,
				  struct elevator_type *e)
{
	struct elevator_queue *eq;

	eq = kzalloc_node(sizeof(*eq), GFP_KERNEL, q->node);
	if (unlikely(!eq))
		return NULL;

	__elevator_get(e);
	eq->type = e;
	kobject_init(&eq->kobj, &elv_ktype);
	mutex_init(&eq->sysfs_lock);
	hash_init(eq->hash);

	return eq;
}
EXPORT_SYMBOL(elevator_alloc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void elevator_release(struct kobject *kobj)
{
	struct elevator_queue *e;

	e = container_of(kobj, struct elevator_queue, kobj);
	elevator_put(e->type);
<<<<<<< HEAD
	kfree(e->hash);
	kfree(e);
}

int elevator_init(struct request_queue *q, char *name)
{
	struct elevator_type *e = NULL;
	struct elevator_queue *eq;
	int err;

	if (unlikely(q->elevator))
		return 0;

	INIT_LIST_HEAD(&q->queue_head);
	q->last_merge = NULL;
	q->end_sector = 0;
	q->boundary_rq = NULL;

	if (name) {
		e = elevator_get(name);
		if (!e)
			return -EINVAL;
	}

	if (!e && *chosen_elevator) {
		e = elevator_get(chosen_elevator);
		if (!e)
			printk(KERN_ERR "I/O scheduler %s not found\n",
							chosen_elevator);
	}

	if (!e) {
		e = elevator_get(CONFIG_DEFAULT_IOSCHED);
		if (!e) {
			printk(KERN_ERR
				"Default I/O scheduler not found. " \
				"Using noop.\n");
			e = elevator_get("noop");
		}
	}

	eq = elevator_alloc(q, e);
	if (!eq)
		return -ENOMEM;

	err = elevator_init_queue(q, eq);
	if (err) {
		kobject_put(&eq->kobj);
		return err;
	}

	q->elevator = eq;
	return 0;
}
EXPORT_SYMBOL(elevator_init);

void elevator_exit(struct elevator_queue *e)
{
	mutex_lock(&e->sysfs_lock);
	if (e->type->ops.elevator_exit_fn)
		e->type->ops.elevator_exit_fn(e);
=======
	kfree(e);
}

void elevator_exit(struct request_queue *q)
{
	struct elevator_queue *e = q->elevator;

	ioc_clear_queue(q);
	blk_mq_sched_free_rqs(q);

	mutex_lock(&e->sysfs_lock);
	blk_mq_exit_sched(q, e);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&e->sysfs_lock);

	kobject_put(&e->kobj);
}
<<<<<<< HEAD
EXPORT_SYMBOL(elevator_exit);

static inline void __elv_rqhash_del(struct request *rq)
{
	hlist_del_init(&rq->hash);
}

static void elv_rqhash_del(struct request_queue *q, struct request *rq)
=======

static inline void __elv_rqhash_del(struct request *rq)
{
	hash_del(&rq->hash);
	rq->rq_flags &= ~RQF_HASHED;
}

void elv_rqhash_del(struct request_queue *q, struct request *rq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (ELV_ON_HASH(rq))
		__elv_rqhash_del(rq);
}
<<<<<<< HEAD

static void elv_rqhash_add(struct request_queue *q, struct request *rq)
=======
EXPORT_SYMBOL_GPL(elv_rqhash_del);

void elv_rqhash_add(struct request_queue *q, struct request *rq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct elevator_queue *e = q->elevator;

	BUG_ON(ELV_ON_HASH(rq));
<<<<<<< HEAD
	hlist_add_head(&rq->hash, &e->hash[ELV_HASH_FN(rq_hash_key(rq))]);
}

static void elv_rqhash_reposition(struct request_queue *q, struct request *rq)
=======
	hash_add(e->hash, &rq->hash, rq_hash_key(rq));
	rq->rq_flags |= RQF_HASHED;
}
EXPORT_SYMBOL_GPL(elv_rqhash_add);

void elv_rqhash_reposition(struct request_queue *q, struct request *rq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__elv_rqhash_del(rq);
	elv_rqhash_add(q, rq);
}

<<<<<<< HEAD
static struct request *elv_rqhash_find(struct request_queue *q, sector_t offset)
{
	struct elevator_queue *e = q->elevator;
	struct hlist_head *hash_list = &e->hash[ELV_HASH_FN(offset)];
	struct hlist_node *entry, *next;
	struct request *rq;

	hlist_for_each_entry_safe(rq, entry, next, hash_list, hash) {
=======
struct request *elv_rqhash_find(struct request_queue *q, sector_t offset)
{
	struct elevator_queue *e = q->elevator;
	struct hlist_node *next;
	struct request *rq;

	hash_for_each_possible_safe(e->hash, rq, next, hash, offset) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		BUG_ON(!ELV_ON_HASH(rq));

		if (unlikely(!rq_mergeable(rq))) {
			__elv_rqhash_del(rq);
			continue;
		}

		if (rq_hash_key(rq) == offset)
			return rq;
	}

	return NULL;
}

/*
 * RB-tree support functions for inserting/lookup/removal of requests
 * in a sorted RB tree.
 */
void elv_rb_add(struct rb_root *root, struct request *rq)
{
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;
	struct request *__rq;

	while (*p) {
		parent = *p;
		__rq = rb_entry(parent, struct request, rb_node);

		if (blk_rq_pos(rq) < blk_rq_pos(__rq))
			p = &(*p)->rb_left;
		else if (blk_rq_pos(rq) >= blk_rq_pos(__rq))
			p = &(*p)->rb_right;
	}

	rb_link_node(&rq->rb_node, parent, p);
	rb_insert_color(&rq->rb_node, root);
}
EXPORT_SYMBOL(elv_rb_add);

void elv_rb_del(struct rb_root *root, struct request *rq)
{
	BUG_ON(RB_EMPTY_NODE(&rq->rb_node));
	rb_erase(&rq->rb_node, root);
	RB_CLEAR_NODE(&rq->rb_node);
}
EXPORT_SYMBOL(elv_rb_del);

struct request *elv_rb_find(struct rb_root *root, sector_t sector)
{
	struct rb_node *n = root->rb_node;
	struct request *rq;

	while (n) {
		rq = rb_entry(n, struct request, rb_node);

		if (sector < blk_rq_pos(rq))
			n = n->rb_left;
		else if (sector > blk_rq_pos(rq))
			n = n->rb_right;
		else
			return rq;
	}

	return NULL;
}
EXPORT_SYMBOL(elv_rb_find);

<<<<<<< HEAD
/*
 * Insert rq into dispatch queue of q.  Queue lock must be held on
 * entry.  rq is sort instead into the dispatch queue. To be used by
 * specific elevators.
 */
void elv_dispatch_sort(struct request_queue *q, struct request *rq)
{
	sector_t boundary;
	struct list_head *entry;
	int stop_flags;

	if (q->last_merge == rq)
		q->last_merge = NULL;

	elv_rqhash_del(q, rq);

	q->nr_sorted--;

	boundary = q->end_sector;
	stop_flags = REQ_SOFTBARRIER | REQ_STARTED;
	list_for_each_prev(entry, &q->queue_head) {
		struct request *pos = list_entry_rq(entry);

		if ((rq->cmd_flags & REQ_DISCARD) !=
		    (pos->cmd_flags & REQ_DISCARD))
			break;
		if (rq_data_dir(rq) != rq_data_dir(pos))
			break;
		if (pos->cmd_flags & stop_flags)
			break;
		if (blk_rq_pos(rq) >= boundary) {
			if (blk_rq_pos(pos) < boundary)
				continue;
		} else {
			if (blk_rq_pos(pos) >= boundary)
				break;
		}
		if (blk_rq_pos(rq) >= blk_rq_pos(pos))
			break;
	}

	list_add(&rq->queuelist, entry);
}
EXPORT_SYMBOL(elv_dispatch_sort);

/*
 * Insert rq into dispatch queue of q.  Queue lock must be held on
 * entry.  rq is added to the back of the dispatch queue. To be used by
 * specific elevators.
 */
void elv_dispatch_add_tail(struct request_queue *q, struct request *rq)
{
	if (q->last_merge == rq)
		q->last_merge = NULL;

	elv_rqhash_del(q, rq);

	q->nr_sorted--;

	q->end_sector = rq_end_sector(rq);
	q->boundary_rq = rq;
	list_add_tail(&rq->queuelist, &q->queue_head);
}
EXPORT_SYMBOL(elv_dispatch_add_tail);

int elv_merge(struct request_queue *q, struct request **req, struct bio *bio)
{
	struct elevator_queue *e = q->elevator;
	struct request *__rq;
	int ret;
=======
enum elv_merge elv_merge(struct request_queue *q, struct request **req,
		struct bio *bio)
{
	struct elevator_queue *e = q->elevator;
	struct request *__rq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Levels of merges:
	 * 	nomerges:  No merges at all attempted
	 * 	noxmerges: Only simple one-hit cache try
	 * 	merges:	   All merge tries attempted
	 */
<<<<<<< HEAD
	if (blk_queue_nomerges(q))
=======
	if (blk_queue_nomerges(q) || !bio_mergeable(bio))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ELEVATOR_NO_MERGE;

	/*
	 * First try one-hit cache.
	 */
<<<<<<< HEAD
	if (q->last_merge && elv_rq_merge_ok(q->last_merge, bio)) {
		ret = blk_try_merge(q->last_merge, bio);
=======
	if (q->last_merge && elv_bio_merge_ok(q->last_merge, bio)) {
		enum elv_merge ret = blk_try_merge(q->last_merge, bio);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret != ELEVATOR_NO_MERGE) {
			*req = q->last_merge;
			return ret;
		}
	}

	if (blk_queue_noxmerges(q))
		return ELEVATOR_NO_MERGE;

	/*
	 * See if our hash lookup can find a potential backmerge.
	 */
<<<<<<< HEAD
	__rq = elv_rqhash_find(q, bio->bi_sector);
	if (__rq && elv_rq_merge_ok(__rq, bio)) {
		*req = __rq;
		return ELEVATOR_BACK_MERGE;
	}

	if (e->type->ops.elevator_merge_fn)
		return e->type->ops.elevator_merge_fn(q, req, bio);
=======
	__rq = elv_rqhash_find(q, bio->bi_iter.bi_sector);
	if (__rq && elv_bio_merge_ok(__rq, bio)) {
		*req = __rq;

		if (blk_discard_mergable(__rq))
			return ELEVATOR_DISCARD_MERGE;
		return ELEVATOR_BACK_MERGE;
	}

	if (e->type->ops.request_merge)
		return e->type->ops.request_merge(q, req, bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ELEVATOR_NO_MERGE;
}

/*
 * Attempt to do an insertion back merge. Only check for the case where
 * we can append 'rq' to an existing request, so we can throw 'rq' away
 * afterwards.
 *
<<<<<<< HEAD
 * Returns true if we merged, false otherwise
 */
static bool elv_attempt_insert_merge(struct request_queue *q,
				     struct request *rq)
{
	struct request *__rq;
=======
 * Returns true if we merged, false otherwise. 'free' will contain all
 * requests that need to be freed.
 */
bool elv_attempt_insert_merge(struct request_queue *q, struct request *rq,
			      struct list_head *free)
{
	struct request *__rq;
	bool ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (blk_queue_nomerges(q))
		return false;

	/*
	 * First try one-hit cache.
	 */
<<<<<<< HEAD
	if (q->last_merge && blk_attempt_req_merge(q, q->last_merge, rq))
		return true;
=======
	if (q->last_merge && blk_attempt_req_merge(q, q->last_merge, rq)) {
		list_add(&rq->queuelist, free);
		return true;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (blk_queue_noxmerges(q))
		return false;

<<<<<<< HEAD
	/*
	 * See if our hash lookup can find a potential backmerge.
	 */
	__rq = elv_rqhash_find(q, blk_rq_pos(rq));
	if (__rq && blk_attempt_req_merge(q, __rq, rq))
		return true;

	return false;
}

void elv_merged_request(struct request_queue *q, struct request *rq, int type)
{
	struct elevator_queue *e = q->elevator;

	if (e->type->ops.elevator_merged_fn)
		e->type->ops.elevator_merged_fn(q, rq, type);
=======
	ret = false;
	/*
	 * See if our hash lookup can find a potential backmerge.
	 */
	while (1) {
		__rq = elv_rqhash_find(q, blk_rq_pos(rq));
		if (!__rq || !blk_attempt_req_merge(q, __rq, rq))
			break;

		list_add(&rq->queuelist, free);
		/* The merged request could be merged with others, try again */
		ret = true;
		rq = __rq;
	}

	return ret;
}

void elv_merged_request(struct request_queue *q, struct request *rq,
		enum elv_merge type)
{
	struct elevator_queue *e = q->elevator;

	if (e->type->ops.request_merged)
		e->type->ops.request_merged(q, rq, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (type == ELEVATOR_BACK_MERGE)
		elv_rqhash_reposition(q, rq);

	q->last_merge = rq;
}

void elv_merge_requests(struct request_queue *q, struct request *rq,
			     struct request *next)
{
	struct elevator_queue *e = q->elevator;
<<<<<<< HEAD
	const int next_sorted = next->cmd_flags & REQ_SORTED;

	if (next_sorted && e->type->ops.elevator_merge_req_fn)
		e->type->ops.elevator_merge_req_fn(q, rq, next);

	elv_rqhash_reposition(q, rq);

	if (next_sorted) {
		elv_rqhash_del(q, next);
		q->nr_sorted--;
	}

	q->last_merge = rq;
}

void elv_bio_merged(struct request_queue *q, struct request *rq,
			struct bio *bio)
{
	struct elevator_queue *e = q->elevator;

	if (e->type->ops.elevator_bio_merged_fn)
		e->type->ops.elevator_bio_merged_fn(q, rq, bio);
}

#ifdef CONFIG_PM_RUNTIME
static void blk_pm_requeue_request(struct request *rq)
{
	if (rq->q->dev && !(rq->cmd_flags & REQ_PM))
		rq->q->nr_pending--;
}

static void blk_pm_add_request(struct request_queue *q, struct request *rq)
{
	if (q->dev && !(rq->cmd_flags & REQ_PM) && q->nr_pending++ == 0 &&
	    (q->rpm_status == RPM_SUSPENDED || q->rpm_status == RPM_SUSPENDING))
		pm_request_resume(q->dev);
}
#else
static inline void blk_pm_requeue_request(struct request *rq) {}
static inline void blk_pm_add_request(struct request_queue *q,
				      struct request *rq)
{
}
#endif

void elv_requeue_request(struct request_queue *q, struct request *rq)
{
	/*
	 * it already went through dequeue, we need to decrement the
	 * in_flight count again
	 */
	if (blk_account_rq(rq)) {
		q->in_flight[rq_is_sync(rq)]--;
		if (rq->cmd_flags & REQ_SORTED)
			elv_deactivate_rq(q, rq);
	}

	rq->cmd_flags &= ~REQ_STARTED;

	blk_pm_requeue_request(rq);

	__elv_add_request(q, rq, ELEVATOR_INSERT_REQUEUE);
}

/**
 * elv_reinsert_request() - Insert a request back to the scheduler
 * @q:		request queue where request should be inserted
 * @rq:		request to be inserted
 *
 * This function returns the request back to the scheduler to be
 * inserted as if it was never dispatched
 *
 * Return: 0 on success, error code on failure
 */
int elv_reinsert_request(struct request_queue *q, struct request *rq)
{
	int res;

	if (!q->elevator->type->ops.elevator_reinsert_req_fn)
		return -EPERM;

	res = q->elevator->type->ops.elevator_reinsert_req_fn(q, rq);
	if (!res) {
		/*
		 * it already went through dequeue, we need to decrement the
		 * in_flight count again
		 */
		if (blk_account_rq(rq)) {
			q->in_flight[rq_is_sync(rq)]--;
			if (rq->cmd_flags & REQ_SORTED)
				elv_deactivate_rq(q, rq);
		}
		rq->cmd_flags &= ~REQ_STARTED;
		q->nr_sorted++;
	}

	return res;
}

void elv_drain_elevator(struct request_queue *q)
{
	static int printed;

	lockdep_assert_held(q->queue_lock);

	while (q->elevator->type->ops.elevator_dispatch_fn(q, 1))
		;
	if (q->nr_sorted && printed++ < 10) {
		printk(KERN_ERR "%s: forced dispatching is broken "
		       "(nr_sorted=%u), please report this\n",
		       q->elevator->type->elevator_name, q->nr_sorted);
	}
}

void elv_quiesce_start(struct request_queue *q)
{
	if (!q->elevator)
		return;

	spin_lock_irq(q->queue_lock);
	queue_flag_set(QUEUE_FLAG_ELVSWITCH, q);
	spin_unlock_irq(q->queue_lock);

	blk_drain_queue(q, false);
}

void elv_quiesce_end(struct request_queue *q)
{
	spin_lock_irq(q->queue_lock);
	queue_flag_clear(QUEUE_FLAG_ELVSWITCH, q);
	spin_unlock_irq(q->queue_lock);
}

void __elv_add_request(struct request_queue *q, struct request *rq, int where)
{
	trace_block_rq_insert(q, rq);

	blk_pm_add_request(q, rq);

	rq->q = q;

	if (rq->cmd_flags & REQ_SOFTBARRIER) {
		/* barriers are scheduling boundary, update end_sector */
		if (rq->cmd_type == REQ_TYPE_FS ||
		    (rq->cmd_flags & (REQ_DISCARD | REQ_SANITIZE))) {
			q->end_sector = rq_end_sector(rq);
			q->boundary_rq = rq;
		}
	} else if (!(rq->cmd_flags & REQ_ELVPRIV) &&
		    (where == ELEVATOR_INSERT_SORT ||
		     where == ELEVATOR_INSERT_SORT_MERGE))
		where = ELEVATOR_INSERT_BACK;

	switch (where) {
	case ELEVATOR_INSERT_REQUEUE:
	case ELEVATOR_INSERT_FRONT:
		rq->cmd_flags |= REQ_SOFTBARRIER;
		list_add(&rq->queuelist, &q->queue_head);
		break;

	case ELEVATOR_INSERT_BACK:
		rq->cmd_flags |= REQ_SOFTBARRIER;
		elv_drain_elevator(q);
		list_add_tail(&rq->queuelist, &q->queue_head);
		/*
		 * We kick the queue here for the following reasons.
		 * - The elevator might have returned NULL previously
		 *   to delay requests and returned them now.  As the
		 *   queue wasn't empty before this request, ll_rw_blk
		 *   won't run the queue on return, resulting in hang.
		 * - Usually, back inserted requests won't be merged
		 *   with anything.  There's no point in delaying queue
		 *   processing.
		 */
		__blk_run_queue(q);
		break;

	case ELEVATOR_INSERT_SORT_MERGE:
		/*
		 * If we succeed in merging this request with one in the
		 * queue already, we are done - rq has now been freed,
		 * so no need to do anything further.
		 */
		if (elv_attempt_insert_merge(q, rq))
			break;
	case ELEVATOR_INSERT_SORT:
		BUG_ON(rq->cmd_type != REQ_TYPE_FS &&
		       !(rq->cmd_flags & REQ_DISCARD));
		rq->cmd_flags |= REQ_SORTED;
		q->nr_sorted++;
		if (rq_mergeable(rq)) {
			elv_rqhash_add(q, rq);
			if (!q->last_merge)
				q->last_merge = rq;
		}

		/*
		 * Some ioscheds (cfq) run q->request_fn directly, so
		 * rq cannot be accessed after calling
		 * elevator_add_req_fn.
		 */
		q->elevator->type->ops.elevator_add_req_fn(q, rq);
		break;

	case ELEVATOR_INSERT_FLUSH:
		rq->cmd_flags |= REQ_SOFTBARRIER;
		blk_insert_flush(rq);
		break;
	default:
		printk(KERN_ERR "%s: bad insertion point %d\n",
		       __func__, where);
		BUG();
	}
}
EXPORT_SYMBOL(__elv_add_request);

void elv_add_request(struct request_queue *q, struct request *rq, int where)
{
	unsigned long flags;

	spin_lock_irqsave(q->queue_lock, flags);
	__elv_add_request(q, rq, where);
	spin_unlock_irqrestore(q->queue_lock, flags);
}
EXPORT_SYMBOL(elv_add_request);

=======

	if (e->type->ops.requests_merged)
		e->type->ops.requests_merged(q, rq, next);

	elv_rqhash_reposition(q, rq);
	q->last_merge = rq;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct request *elv_latter_request(struct request_queue *q, struct request *rq)
{
	struct elevator_queue *e = q->elevator;

<<<<<<< HEAD
	if (e->type->ops.elevator_latter_req_fn)
		return e->type->ops.elevator_latter_req_fn(q, rq);
=======
	if (e->type->ops.next_request)
		return e->type->ops.next_request(q, rq);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}

struct request *elv_former_request(struct request_queue *q, struct request *rq)
{
	struct elevator_queue *e = q->elevator;

<<<<<<< HEAD
	if (e->type->ops.elevator_former_req_fn)
		return e->type->ops.elevator_former_req_fn(q, rq);
	return NULL;
}

int elv_set_request(struct request_queue *q, struct request *rq, gfp_t gfp_mask)
{
	struct elevator_queue *e = q->elevator;

	if (e->type->ops.elevator_set_req_fn)
		return e->type->ops.elevator_set_req_fn(q, rq, gfp_mask);
	return 0;
}

void elv_put_request(struct request_queue *q, struct request *rq)
{
	struct elevator_queue *e = q->elevator;

	if (e->type->ops.elevator_put_req_fn)
		e->type->ops.elevator_put_req_fn(rq);
}

int elv_may_queue(struct request_queue *q, int rw)
{
	struct elevator_queue *e = q->elevator;

	if (e->type->ops.elevator_may_queue_fn)
		return e->type->ops.elevator_may_queue_fn(q, rw);

	return ELV_MQUEUE_MAY;
}

void elv_abort_queue(struct request_queue *q)
{
	struct request *rq;

	blk_abort_flushes(q);

	while (!list_empty(&q->queue_head)) {
		rq = list_entry_rq(q->queue_head.next);
		rq->cmd_flags |= REQ_QUIET;
		trace_block_rq_abort(q, rq);
		/*
		 * Mark this request as started so we don't trigger
		 * any debug logic in the end I/O path.
		 */
		blk_start_request(rq);
		__blk_end_request_all(rq, -EIO);
	}
}
EXPORT_SYMBOL(elv_abort_queue);

void elv_completed_request(struct request_queue *q, struct request *rq)
{
	struct elevator_queue *e = q->elevator;

	if (rq->cmd_flags & REQ_URGENT) {
		q->notified_urgent = false;
		WARN_ON(!q->dispatched_urgent);
		q->dispatched_urgent = false;
	}
	/*
	 * request is released from the driver, io must be done
	 */
	if (blk_account_rq(rq)) {
		q->in_flight[rq_is_sync(rq)]--;
		if ((rq->cmd_flags & REQ_SORTED) &&
		    e->type->ops.elevator_completed_req_fn)
			e->type->ops.elevator_completed_req_fn(q, rq);
	}
}

=======
	if (e->type->ops.former_request)
		return e->type->ops.former_request(q, rq);

	return NULL;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define to_elv(atr) container_of((atr), struct elv_fs_entry, attr)

static ssize_t
elv_attr_show(struct kobject *kobj, struct attribute *attr, char *page)
{
	struct elv_fs_entry *entry = to_elv(attr);
	struct elevator_queue *e;
	ssize_t error;

	if (!entry->show)
		return -EIO;

	e = container_of(kobj, struct elevator_queue, kobj);
	mutex_lock(&e->sysfs_lock);
	error = e->type ? entry->show(e, page) : -ENOENT;
	mutex_unlock(&e->sysfs_lock);
	return error;
}

static ssize_t
elv_attr_store(struct kobject *kobj, struct attribute *attr,
	       const char *page, size_t length)
{
	struct elv_fs_entry *entry = to_elv(attr);
	struct elevator_queue *e;
	ssize_t error;

	if (!entry->store)
		return -EIO;

	e = container_of(kobj, struct elevator_queue, kobj);
	mutex_lock(&e->sysfs_lock);
	error = e->type ? entry->store(e, page, length) : -ENOENT;
	mutex_unlock(&e->sysfs_lock);
	return error;
}

static const struct sysfs_ops elv_sysfs_ops = {
	.show	= elv_attr_show,
	.store	= elv_attr_store,
};

<<<<<<< HEAD
static struct kobj_type elv_ktype = {
=======
static const struct kobj_type elv_ktype = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.sysfs_ops	= &elv_sysfs_ops,
	.release	= elevator_release,
};

<<<<<<< HEAD
int __elv_register_queue(struct request_queue *q, struct elevator_queue *e)
{
	int error;

	error = kobject_add(&e->kobj, &q->kobj, "%s", "iosched");
=======
int elv_register_queue(struct request_queue *q, bool uevent)
{
	struct elevator_queue *e = q->elevator;
	int error;

	lockdep_assert_held(&q->sysfs_lock);

	error = kobject_add(&e->kobj, &q->disk->queue_kobj, "iosched");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!error) {
		struct elv_fs_entry *attr = e->type->elevator_attrs;
		if (attr) {
			while (attr->attr.name) {
				if (sysfs_create_file(&e->kobj, &attr->attr))
					break;
				attr++;
			}
		}
<<<<<<< HEAD
		kobject_uevent(&e->kobj, KOBJ_ADD);
		e->registered = 1;
=======
		if (uevent)
			kobject_uevent(&e->kobj, KOBJ_ADD);

		set_bit(ELEVATOR_FLAG_REGISTERED, &e->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return error;
}

<<<<<<< HEAD
int elv_register_queue(struct request_queue *q)
{
	return __elv_register_queue(q, q->elevator);
}
EXPORT_SYMBOL(elv_register_queue);

void elv_unregister_queue(struct request_queue *q)
{
	if (q) {
		struct elevator_queue *e = q->elevator;

		kobject_uevent(&e->kobj, KOBJ_REMOVE);
		kobject_del(&e->kobj);
		e->registered = 0;
	}
}
EXPORT_SYMBOL(elv_unregister_queue);

int elv_register(struct elevator_type *e)
{
	char *def = "";
=======
void elv_unregister_queue(struct request_queue *q)
{
	struct elevator_queue *e = q->elevator;

	lockdep_assert_held(&q->sysfs_lock);

	if (e && test_and_clear_bit(ELEVATOR_FLAG_REGISTERED, &e->flags)) {
		kobject_uevent(&e->kobj, KOBJ_REMOVE);
		kobject_del(&e->kobj);
	}
}

int elv_register(struct elevator_type *e)
{
	/* finish request is mandatory */
	if (WARN_ON_ONCE(!e->ops.finish_request))
		return -EINVAL;
	/* insert_requests and dispatch_request are mandatory */
	if (WARN_ON_ONCE(!e->ops.insert_requests || !e->ops.dispatch_request))
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* create icq_cache if requested */
	if (e->icq_size) {
		if (WARN_ON(e->icq_size < sizeof(struct io_cq)) ||
		    WARN_ON(e->icq_align < __alignof__(struct io_cq)))
			return -EINVAL;

		snprintf(e->icq_cache_name, sizeof(e->icq_cache_name),
			 "%s_io_cq", e->elevator_name);
		e->icq_cache = kmem_cache_create(e->icq_cache_name, e->icq_size,
						 e->icq_align, 0, NULL);
		if (!e->icq_cache)
			return -ENOMEM;
	}

	/* register, don't allow duplicate names */
	spin_lock(&elv_list_lock);
<<<<<<< HEAD
	if (elevator_find(e->elevator_name)) {
		spin_unlock(&elv_list_lock);
		if (e->icq_cache)
			kmem_cache_destroy(e->icq_cache);
=======
	if (__elevator_find(e->elevator_name)) {
		spin_unlock(&elv_list_lock);
		kmem_cache_destroy(e->icq_cache);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EBUSY;
	}
	list_add_tail(&e->list, &elv_list);
	spin_unlock(&elv_list_lock);

<<<<<<< HEAD
	/* print pretty message */
	if (!strcmp(e->elevator_name, chosen_elevator) ||
			(!*chosen_elevator &&
			 !strcmp(e->elevator_name, CONFIG_DEFAULT_IOSCHED)))
				def = " (default)";

	printk(KERN_INFO "io scheduler %s registered%s\n", e->elevator_name,
								def);
=======
	printk(KERN_INFO "io scheduler %s registered\n", e->elevator_name);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL_GPL(elv_register);

void elv_unregister(struct elevator_type *e)
{
	/* unregister */
	spin_lock(&elv_list_lock);
	list_del_init(&e->list);
	spin_unlock(&elv_list_lock);

	/*
	 * Destroy icq_cache if it exists.  icq's are RCU managed.  Make
	 * sure all RCU operations are complete before proceeding.
	 */
	if (e->icq_cache) {
		rcu_barrier();
		kmem_cache_destroy(e->icq_cache);
		e->icq_cache = NULL;
	}
}
EXPORT_SYMBOL_GPL(elv_unregister);

<<<<<<< HEAD
/*
 * switch to new_e io scheduler. be careful not to introduce deadlocks -
 * we don't free the old io scheduler, before we have allocated what we
 * need for the new one. this way we have a chance of going back to the old
 * one, if the new one fails init for some reason.
 */
static int elevator_switch(struct request_queue *q, struct elevator_type *new_e)
{
	struct elevator_queue *old_elevator, *e;
	int err;

	/* allocate new elevator */
	e = elevator_alloc(q, new_e);
	if (!e)
		return -ENOMEM;

	err = elevator_init_queue(q, e);
	if (err) {
		kobject_put(&e->kobj);
		return err;
	}

	/* turn on BYPASS and drain all requests w/ elevator private data */
	elv_quiesce_start(q);

	/* unregister old queue, register new one and kill old elevator */
	if (q->elevator->registered) {
		elv_unregister_queue(q);
		err = __elv_register_queue(q, e);
		if (err)
			goto fail_register;
	}

	/* done, clear io_cq's, switch elevators and turn off BYPASS */
	spin_lock_irq(q->queue_lock);
	ioc_clear_queue(q);
	old_elevator = q->elevator;
	q->elevator = e;
	spin_unlock_irq(q->queue_lock);

	elevator_exit(old_elevator);
	elv_quiesce_end(q);

	blk_add_trace_msg(q, "elv switch: %s", e->type->elevator_name);

	return 0;

fail_register:
	/*
	 * switch failed, exit the new io scheduler and reattach the old
	 * one again (along with re-adding the sysfs dir)
	 */
	elevator_exit(e);
	elv_register_queue(q);
	elv_quiesce_end(q);

	return err;
=======
static inline bool elv_support_iosched(struct request_queue *q)
{
	if (!queue_is_mq(q) ||
	    (q->tag_set && (q->tag_set->flags & BLK_MQ_F_NO_SCHED)))
		return false;
	return true;
}

/*
 * For single queue devices, default to using mq-deadline. If we have multiple
 * queues or mq-deadline is not available, default to "none".
 */
static struct elevator_type *elevator_get_default(struct request_queue *q)
{
	if (q->tag_set && q->tag_set->flags & BLK_MQ_F_NO_SCHED_BY_DEFAULT)
		return NULL;

	if (q->nr_hw_queues != 1 &&
	    !blk_mq_is_shared_tags(q->tag_set->flags))
		return NULL;

	return elevator_find_get(q, "mq-deadline");
}

/*
 * Get the first elevator providing the features required by the request queue.
 * Default to "none" if no matching elevator is found.
 */
static struct elevator_type *elevator_get_by_features(struct request_queue *q)
{
	struct elevator_type *e, *found = NULL;

	spin_lock(&elv_list_lock);

	list_for_each_entry(e, &elv_list, list) {
		if (elv_support_features(q, e)) {
			found = e;
			break;
		}
	}

	if (found && !elevator_tryget(found))
		found = NULL;

	spin_unlock(&elv_list_lock);
	return found;
}

/*
 * For a device queue that has no required features, use the default elevator
 * settings. Otherwise, use the first elevator available matching the required
 * features. If no suitable elevator is find or if the chosen elevator
 * initialization fails, fall back to the "none" elevator (no elevator).
 */
void elevator_init_mq(struct request_queue *q)
{
	struct elevator_type *e;
	int err;

	if (!elv_support_iosched(q))
		return;

	WARN_ON_ONCE(blk_queue_registered(q));

	if (unlikely(q->elevator))
		return;

	if (!q->required_elevator_features)
		e = elevator_get_default(q);
	else
		e = elevator_get_by_features(q);
	if (!e)
		return;

	/*
	 * We are called before adding disk, when there isn't any FS I/O,
	 * so freezing queue plus canceling dispatch work is enough to
	 * drain any dispatch activities originated from passthrough
	 * requests, then no need to quiesce queue which may add long boot
	 * latency, especially when lots of disks are involved.
	 */
	blk_mq_freeze_queue(q);
	blk_mq_cancel_work_sync(q);

	err = blk_mq_init_sched(q, e);

	blk_mq_unfreeze_queue(q);

	if (err) {
		pr_warn("\"%s\" elevator initialization failed, "
			"falling back to \"none\"\n", e->elevator_name);
	}

	elevator_put(e);
}

/*
 * Switch to new_e io scheduler.
 *
 * If switching fails, we are most likely running out of memory and not able
 * to restore the old io scheduler, so leaving the io scheduler being none.
 */
int elevator_switch(struct request_queue *q, struct elevator_type *new_e)
{
	int ret;

	lockdep_assert_held(&q->sysfs_lock);

	blk_mq_freeze_queue(q);
	blk_mq_quiesce_queue(q);

	if (q->elevator) {
		elv_unregister_queue(q);
		elevator_exit(q);
	}

	ret = blk_mq_init_sched(q, new_e);
	if (ret)
		goto out_unfreeze;

	ret = elv_register_queue(q, true);
	if (ret) {
		elevator_exit(q);
		goto out_unfreeze;
	}
	blk_add_trace_msg(q, "elv switch: %s", new_e->elevator_name);

out_unfreeze:
	blk_mq_unquiesce_queue(q);
	blk_mq_unfreeze_queue(q);

	if (ret) {
		pr_warn("elv: switch to \"%s\" failed, falling back to \"none\"\n",
			new_e->elevator_name);
	}

	return ret;
}

void elevator_disable(struct request_queue *q)
{
	lockdep_assert_held(&q->sysfs_lock);

	blk_mq_freeze_queue(q);
	blk_mq_quiesce_queue(q);

	elv_unregister_queue(q);
	elevator_exit(q);
	blk_queue_flag_clear(QUEUE_FLAG_SQ_SCHED, q);
	q->elevator = NULL;
	q->nr_requests = q->tag_set->queue_depth;
	blk_add_trace_msg(q, "elv switch: none");

	blk_mq_unquiesce_queue(q);
	blk_mq_unfreeze_queue(q);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Switch this queue to the given IO scheduler.
 */
<<<<<<< HEAD
static int __elevator_change(struct request_queue *q, const char *name)
{
	char elevator_name[ELV_NAME_MAX];
	struct elevator_type *e;

	if (!q->elevator)
		return -ENXIO;

	strlcpy(elevator_name, name, sizeof(elevator_name));
	e = elevator_get(strstrip(elevator_name));
	if (!e) {
		printk(KERN_ERR "elevator: type %s not found\n", elevator_name);
		return -EINVAL;
	}

	if (!strcmp(elevator_name, q->elevator->type->elevator_name)) {
		elevator_put(e);
		return 0;
	}

	return elevator_switch(q, e);
}

int elevator_change(struct request_queue *q, const char *name)
{
	int ret;

	/* Protect q->elevator from elevator_init() */
	mutex_lock(&q->sysfs_lock);
	ret = __elevator_change(q, name);
	mutex_unlock(&q->sysfs_lock);

	return ret;
}
EXPORT_SYMBOL(elevator_change);

ssize_t elv_iosched_store(struct request_queue *q, const char *name,
			  size_t count)
{
	int ret;

	if (!q->elevator)
		return count;

	ret = __elevator_change(q, name);
	if (!ret)
		return count;

	printk(KERN_ERR "elevator: switch to %s failed\n", name);
=======
static int elevator_change(struct request_queue *q, const char *elevator_name)
{
	struct elevator_type *e;
	int ret;

	/* Make sure queue is not in the middle of being removed */
	if (!blk_queue_registered(q))
		return -ENOENT;

	if (!strncmp(elevator_name, "none", 4)) {
		if (q->elevator)
			elevator_disable(q);
		return 0;
	}

	if (q->elevator && elevator_match(q->elevator->type, elevator_name))
		return 0;

	e = elevator_find_get(q, elevator_name);
	if (!e) {
		request_module("%s-iosched", elevator_name);
		e = elevator_find_get(q, elevator_name);
		if (!e)
			return -EINVAL;
	}
	ret = elevator_switch(q, e);
	elevator_put(e);
	return ret;
}

ssize_t elv_iosched_store(struct request_queue *q, const char *buf,
			  size_t count)
{
	char elevator_name[ELV_NAME_MAX];
	int ret;

	if (!elv_support_iosched(q))
		return count;

	strscpy(elevator_name, buf, sizeof(elevator_name));
	ret = elevator_change(q, strstrip(elevator_name));
	if (!ret)
		return count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

ssize_t elv_iosched_show(struct request_queue *q, char *name)
{
<<<<<<< HEAD
	struct elevator_queue *e = q->elevator;
	struct elevator_type *elv;
	struct elevator_type *__e;
	int len = 0;

	if (!q->elevator || !blk_queue_stackable(q))
		return sprintf(name, "none\n");

	elv = e->type;

	spin_lock(&elv_list_lock);
	list_for_each_entry(__e, &elv_list, list) {
		if (!strcmp(elv->elevator_name, __e->elevator_name))
			len += sprintf(name+len, "[%s] ", elv->elevator_name);
		else
			len += sprintf(name+len, "%s ", __e->elevator_name);
	}
	spin_unlock(&elv_list_lock);

	len += sprintf(len+name, "\n");
=======
	struct elevator_queue *eq = q->elevator;
	struct elevator_type *cur = NULL, *e;
	int len = 0;

	if (!elv_support_iosched(q))
		return sprintf(name, "none\n");

	if (!q->elevator) {
		len += sprintf(name+len, "[none] ");
	} else {
		len += sprintf(name+len, "none ");
		cur = eq->type;
	}

	spin_lock(&elv_list_lock);
	list_for_each_entry(e, &elv_list, list) {
		if (e == cur)
			len += sprintf(name+len, "[%s] ", e->elevator_name);
		else if (elv_support_features(q, e))
			len += sprintf(name+len, "%s ", e->elevator_name);
	}
	spin_unlock(&elv_list_lock);

	len += sprintf(name+len, "\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return len;
}

struct request *elv_rb_former_request(struct request_queue *q,
				      struct request *rq)
{
	struct rb_node *rbprev = rb_prev(&rq->rb_node);

	if (rbprev)
		return rb_entry_rq(rbprev);

	return NULL;
}
EXPORT_SYMBOL(elv_rb_former_request);

struct request *elv_rb_latter_request(struct request_queue *q,
				      struct request *rq)
{
	struct rb_node *rbnext = rb_next(&rq->rb_node);

	if (rbnext)
		return rb_entry_rq(rbnext);

	return NULL;
}
EXPORT_SYMBOL(elv_rb_latter_request);
<<<<<<< HEAD
=======

static int __init elevator_setup(char *str)
{
	pr_warn("Kernel parameter elevator= does not have any effect anymore.\n"
		"Please use sysfs to set IO scheduler for individual devices.\n");
	return 1;
}

__setup("elevator=", elevator_setup);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
