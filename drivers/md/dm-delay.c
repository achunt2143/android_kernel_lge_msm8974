<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2005-2007 Red Hat GmbH
 *
 * A target that delays reads and/or writes and can send
 * them to different devices.
 *
 * This file is released under the GPL.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/kthread.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/device-mapper.h>

#define DM_MSG_PREFIX "delay"

<<<<<<< HEAD
=======
struct delay_class {
	struct dm_dev *dev;
	sector_t start;
	unsigned int delay;
	unsigned int ops;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct delay_c {
	struct timer_list delay_timer;
	struct mutex timer_lock;
	struct workqueue_struct *kdelayd_wq;
	struct work_struct flush_expired_bios;
	struct list_head delayed_bios;
<<<<<<< HEAD
	atomic_t may_delay;
	mempool_t *delayed_pool;

	struct dm_dev *dev_read;
	sector_t start_read;
	unsigned read_delay;
	unsigned reads;

	struct dm_dev *dev_write;
	sector_t start_write;
	unsigned write_delay;
	unsigned writes;
=======
	struct task_struct *worker;
	bool may_delay;

	struct delay_class read;
	struct delay_class write;
	struct delay_class flush;

	int argc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct dm_delay_info {
	struct delay_c *context;
<<<<<<< HEAD
	struct list_head list;
	struct bio *bio;
=======
	struct delay_class *class;
	struct list_head list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long expires;
};

static DEFINE_MUTEX(delayed_bios_lock);

<<<<<<< HEAD
static struct kmem_cache *delayed_cache;

static void handle_delayed_timer(unsigned long data)
{
	struct delay_c *dc = (struct delay_c *)data;
=======
static void handle_delayed_timer(struct timer_list *t)
{
	struct delay_c *dc = from_timer(dc, t, delay_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	queue_work(dc->kdelayd_wq, &dc->flush_expired_bios);
}

static void queue_timeout(struct delay_c *dc, unsigned long expires)
{
	mutex_lock(&dc->timer_lock);

	if (!timer_pending(&dc->delay_timer) || expires < dc->delay_timer.expires)
		mod_timer(&dc->delay_timer, expires);

	mutex_unlock(&dc->timer_lock);
}

<<<<<<< HEAD
=======
static inline bool delay_is_fast(struct delay_c *dc)
{
	return !!dc->worker;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void flush_bios(struct bio *bio)
{
	struct bio *n;

	while (bio) {
		n = bio->bi_next;
		bio->bi_next = NULL;
<<<<<<< HEAD
		generic_make_request(bio);
=======
		dm_submit_bio_remap(bio, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bio = n;
	}
}

<<<<<<< HEAD
static struct bio *flush_delayed_bios(struct delay_c *dc, int flush_all)
{
	struct dm_delay_info *delayed, *next;
	unsigned long next_expires = 0;
	int start_timer = 0;
	struct bio_list flush_bios = { };

	mutex_lock(&delayed_bios_lock);
	list_for_each_entry_safe(delayed, next, &dc->delayed_bios, list) {
		if (flush_all || time_after_eq(jiffies, delayed->expires)) {
			list_del(&delayed->list);
			bio_list_add(&flush_bios, delayed->bio);
			if ((bio_data_dir(delayed->bio) == WRITE))
				delayed->context->writes--;
			else
				delayed->context->reads--;
			mempool_free(delayed, dc->delayed_pool);
			continue;
		}

		if (!start_timer) {
			start_timer = 1;
			next_expires = delayed->expires;
		} else
			next_expires = min(next_expires, delayed->expires);
	}

=======
static void flush_delayed_bios(struct delay_c *dc, bool flush_all)
{
	struct dm_delay_info *delayed, *next;
	struct bio_list flush_bio_list;
	unsigned long next_expires = 0;
	bool start_timer = false;
	bio_list_init(&flush_bio_list);

	mutex_lock(&delayed_bios_lock);
	list_for_each_entry_safe(delayed, next, &dc->delayed_bios, list) {
		cond_resched();
		if (flush_all || time_after_eq(jiffies, delayed->expires)) {
			struct bio *bio = dm_bio_from_per_bio_data(delayed,
						sizeof(struct dm_delay_info));
			list_del(&delayed->list);
			bio_list_add(&flush_bio_list, bio);
			delayed->class->ops--;
			continue;
		}

		if (!delay_is_fast(dc)) {
			if (!start_timer) {
				start_timer = true;
				next_expires = delayed->expires;
			} else {
				next_expires = min(next_expires, delayed->expires);
			}
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&delayed_bios_lock);

	if (start_timer)
		queue_timeout(dc, next_expires);

<<<<<<< HEAD
	return bio_list_get(&flush_bios);
=======
	flush_bios(bio_list_get(&flush_bio_list));
}

static int flush_worker_fn(void *data)
{
	struct delay_c *dc = data;

	while (!kthread_should_stop()) {
		flush_delayed_bios(dc, false);
		mutex_lock(&delayed_bios_lock);
		if (unlikely(list_empty(&dc->delayed_bios))) {
			set_current_state(TASK_INTERRUPTIBLE);
			mutex_unlock(&delayed_bios_lock);
			schedule();
		} else {
			mutex_unlock(&delayed_bios_lock);
			cond_resched();
		}
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void flush_expired_bios(struct work_struct *work)
{
	struct delay_c *dc;

	dc = container_of(work, struct delay_c, flush_expired_bios);
<<<<<<< HEAD
	flush_bios(flush_delayed_bios(dc, 0));
=======
	flush_delayed_bios(dc, false);
}

static void delay_dtr(struct dm_target *ti)
{
	struct delay_c *dc = ti->private;

	if (dc->kdelayd_wq)
		destroy_workqueue(dc->kdelayd_wq);

	if (dc->read.dev)
		dm_put_device(ti, dc->read.dev);
	if (dc->write.dev)
		dm_put_device(ti, dc->write.dev);
	if (dc->flush.dev)
		dm_put_device(ti, dc->flush.dev);
	if (dc->worker)
		kthread_stop(dc->worker);

	mutex_destroy(&dc->timer_lock);

	kfree(dc);
}

static int delay_class_ctr(struct dm_target *ti, struct delay_class *c, char **argv)
{
	int ret;
	unsigned long long tmpll;
	char dummy;

	if (sscanf(argv[1], "%llu%c", &tmpll, &dummy) != 1 || tmpll != (sector_t)tmpll) {
		ti->error = "Invalid device sector";
		return -EINVAL;
	}
	c->start = tmpll;

	if (sscanf(argv[2], "%u%c", &c->delay, &dummy) != 1) {
		ti->error = "Invalid delay";
		return -EINVAL;
	}

	ret = dm_get_device(ti, argv[0], dm_table_get_mode(ti->table), &c->dev);
	if (ret) {
		ti->error = "Device lookup failed";
		return ret;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Mapping parameters:
 *    <device> <offset> <delay> [<write_device> <write_offset> <write_delay>]
 *
 * With separate write parameters, the first set is only used for reads.
<<<<<<< HEAD
=======
 * Offsets are specified in sectors.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Delays are specified in milliseconds.
 */
static int delay_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	struct delay_c *dc;
<<<<<<< HEAD
	unsigned long long tmpll;
	char dummy;

	if (argc != 3 && argc != 6) {
		ti->error = "requires exactly 3 or 6 arguments";
		return -EINVAL;
	}

	dc = kmalloc(sizeof(*dc), GFP_KERNEL);
=======
	int ret;
	unsigned int max_delay;

	if (argc != 3 && argc != 6 && argc != 9) {
		ti->error = "Requires exactly 3, 6 or 9 arguments";
		return -EINVAL;
	}

	dc = kzalloc(sizeof(*dc), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!dc) {
		ti->error = "Cannot allocate context";
		return -ENOMEM;
	}

<<<<<<< HEAD
	dc->reads = dc->writes = 0;

	if (sscanf(argv[1], "%llu%c", &tmpll, &dummy) != 1) {
		ti->error = "Invalid device sector";
		goto bad;
	}
	dc->start_read = tmpll;

	if (sscanf(argv[2], "%u%c", &dc->read_delay, &dummy) != 1) {
		ti->error = "Invalid delay";
		goto bad;
	}

	if (dm_get_device(ti, argv[0], dm_table_get_mode(ti->table),
			  &dc->dev_read)) {
		ti->error = "Device lookup failed";
		goto bad;
	}

	dc->dev_write = NULL;
	if (argc == 3)
		goto out;

	if (sscanf(argv[4], "%llu%c", &tmpll, &dummy) != 1) {
		ti->error = "Invalid write device sector";
		goto bad_dev_read;
	}
	dc->start_write = tmpll;

	if (sscanf(argv[5], "%u%c", &dc->write_delay, &dummy) != 1) {
		ti->error = "Invalid write delay";
		goto bad_dev_read;
	}

	if (dm_get_device(ti, argv[3], dm_table_get_mode(ti->table),
			  &dc->dev_write)) {
		ti->error = "Write device lookup failed";
		goto bad_dev_read;
	}

out:
	dc->delayed_pool = mempool_create_slab_pool(128, delayed_cache);
	if (!dc->delayed_pool) {
		DMERR("Couldn't create delayed bio pool.");
		goto bad_dev_write;
	}

	dc->kdelayd_wq = alloc_workqueue("kdelayd", WQ_MEM_RECLAIM, 0);
	if (!dc->kdelayd_wq) {
		DMERR("Couldn't start kdelayd");
		goto bad_queue;
	}

	setup_timer(&dc->delay_timer, handle_delayed_timer, (unsigned long)dc);

	INIT_WORK(&dc->flush_expired_bios, flush_expired_bios);
	INIT_LIST_HEAD(&dc->delayed_bios);
	mutex_init(&dc->timer_lock);
	atomic_set(&dc->may_delay, 1);

	ti->num_flush_requests = 1;
	ti->num_discard_requests = 1;
	ti->private = dc;
	return 0;

bad_queue:
	mempool_destroy(dc->delayed_pool);
bad_dev_write:
	if (dc->dev_write)
		dm_put_device(ti, dc->dev_write);
bad_dev_read:
	dm_put_device(ti, dc->dev_read);
bad:
	kfree(dc);
	return -EINVAL;
}

static void delay_dtr(struct dm_target *ti)
{
	struct delay_c *dc = ti->private;

	destroy_workqueue(dc->kdelayd_wq);

	dm_put_device(ti, dc->dev_read);

	if (dc->dev_write)
		dm_put_device(ti, dc->dev_write);

	mempool_destroy(dc->delayed_pool);
	kfree(dc);
}

static int delay_bio(struct delay_c *dc, int delay, struct bio *bio)
=======
	ti->private = dc;
	INIT_LIST_HEAD(&dc->delayed_bios);
	mutex_init(&dc->timer_lock);
	dc->may_delay = true;
	dc->argc = argc;

	ret = delay_class_ctr(ti, &dc->read, argv);
	if (ret)
		goto bad;
	max_delay = dc->read.delay;

	if (argc == 3) {
		ret = delay_class_ctr(ti, &dc->write, argv);
		if (ret)
			goto bad;
		ret = delay_class_ctr(ti, &dc->flush, argv);
		if (ret)
			goto bad;
		max_delay = max(max_delay, dc->write.delay);
		max_delay = max(max_delay, dc->flush.delay);
		goto out;
	}

	ret = delay_class_ctr(ti, &dc->write, argv + 3);
	if (ret)
		goto bad;
	if (argc == 6) {
		ret = delay_class_ctr(ti, &dc->flush, argv + 3);
		if (ret)
			goto bad;
		max_delay = max(max_delay, dc->flush.delay);
		goto out;
	}

	ret = delay_class_ctr(ti, &dc->flush, argv + 6);
	if (ret)
		goto bad;
	max_delay = max(max_delay, dc->flush.delay);

out:
	if (max_delay < 50) {
		/*
		 * In case of small requested delays, use kthread instead of
		 * timers and workqueue to achieve better latency.
		 */
		dc->worker = kthread_create(&flush_worker_fn, dc,
					    "dm-delay-flush-worker");
		if (IS_ERR(dc->worker)) {
			ret = PTR_ERR(dc->worker);
			dc->worker = NULL;
			goto bad;
		}
	} else {
		timer_setup(&dc->delay_timer, handle_delayed_timer, 0);
		INIT_WORK(&dc->flush_expired_bios, flush_expired_bios);
		dc->kdelayd_wq = alloc_workqueue("kdelayd", WQ_MEM_RECLAIM, 0);
		if (!dc->kdelayd_wq) {
			ret = -EINVAL;
			DMERR("Couldn't start kdelayd");
			goto bad;
		}
	}

	ti->num_flush_bios = 1;
	ti->num_discard_bios = 1;
	ti->accounts_remapped_io = true;
	ti->per_io_data_size = sizeof(struct dm_delay_info);
	return 0;

bad:
	delay_dtr(ti);
	return ret;
}

static int delay_bio(struct delay_c *dc, struct delay_class *c, struct bio *bio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dm_delay_info *delayed;
	unsigned long expires = 0;

<<<<<<< HEAD
	if (!delay || !atomic_read(&dc->may_delay))
		return 1;

	delayed = mempool_alloc(dc->delayed_pool, GFP_NOIO);

	delayed->context = dc;
	delayed->bio = bio;
	delayed->expires = expires = jiffies + (delay * HZ / 1000);

	mutex_lock(&delayed_bios_lock);

	if (bio_data_dir(bio) == WRITE)
		dc->writes++;
	else
		dc->reads++;

	list_add_tail(&delayed->list, &dc->delayed_bios);

	mutex_unlock(&delayed_bios_lock);

	queue_timeout(dc, expires);

	return 0;
=======
	if (!c->delay)
		return DM_MAPIO_REMAPPED;

	delayed = dm_per_bio_data(bio, sizeof(struct dm_delay_info));

	delayed->context = dc;
	delayed->expires = expires = jiffies + msecs_to_jiffies(c->delay);

	mutex_lock(&delayed_bios_lock);
	if (unlikely(!dc->may_delay)) {
		mutex_unlock(&delayed_bios_lock);
		return DM_MAPIO_REMAPPED;
	}
	c->ops++;
	list_add_tail(&delayed->list, &dc->delayed_bios);
	mutex_unlock(&delayed_bios_lock);

	if (delay_is_fast(dc))
		wake_up_process(dc->worker);
	else
		queue_timeout(dc, expires);

	return DM_MAPIO_SUBMITTED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void delay_presuspend(struct dm_target *ti)
{
	struct delay_c *dc = ti->private;

<<<<<<< HEAD
	atomic_set(&dc->may_delay, 0);
	del_timer_sync(&dc->delay_timer);
	flush_bios(flush_delayed_bios(dc, 1));
=======
	mutex_lock(&delayed_bios_lock);
	dc->may_delay = false;
	mutex_unlock(&delayed_bios_lock);

	if (!delay_is_fast(dc))
		del_timer_sync(&dc->delay_timer);
	flush_delayed_bios(dc, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void delay_resume(struct dm_target *ti)
{
	struct delay_c *dc = ti->private;

<<<<<<< HEAD
	atomic_set(&dc->may_delay, 1);
}

static int delay_map(struct dm_target *ti, struct bio *bio,
		     union map_info *map_context)
{
	struct delay_c *dc = ti->private;

	if ((bio_data_dir(bio) == WRITE) && (dc->dev_write)) {
		bio->bi_bdev = dc->dev_write->bdev;
		if (bio_sectors(bio))
			bio->bi_sector = dc->start_write +
					 dm_target_offset(ti, bio->bi_sector);

		return delay_bio(dc, dc->write_delay, bio);
	}

	bio->bi_bdev = dc->dev_read->bdev;
	bio->bi_sector = dc->start_read + dm_target_offset(ti, bio->bi_sector);

	return delay_bio(dc, dc->read_delay, bio);
}

static void delay_status(struct dm_target *ti, status_type_t type,
			 char *result, unsigned maxlen)
=======
	dc->may_delay = true;
}

static int delay_map(struct dm_target *ti, struct bio *bio)
{
	struct delay_c *dc = ti->private;
	struct delay_class *c;
	struct dm_delay_info *delayed = dm_per_bio_data(bio, sizeof(struct dm_delay_info));

	if (bio_data_dir(bio) == WRITE) {
		if (unlikely(bio->bi_opf & REQ_PREFLUSH))
			c = &dc->flush;
		else
			c = &dc->write;
	} else {
		c = &dc->read;
	}
	delayed->class = c;
	bio_set_dev(bio, c->dev->bdev);
	bio->bi_iter.bi_sector = c->start + dm_target_offset(ti, bio->bi_iter.bi_sector);

	return delay_bio(dc, c, bio);
}

#define DMEMIT_DELAY_CLASS(c) \
	DMEMIT("%s %llu %u", (c)->dev->name, (unsigned long long)(c)->start, (c)->delay)

static void delay_status(struct dm_target *ti, status_type_t type,
			 unsigned int status_flags, char *result, unsigned int maxlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct delay_c *dc = ti->private;
	int sz = 0;

	switch (type) {
	case STATUSTYPE_INFO:
<<<<<<< HEAD
		DMEMIT("%u %u", dc->reads, dc->writes);
		break;

	case STATUSTYPE_TABLE:
		DMEMIT("%s %llu %u", dc->dev_read->name,
		       (unsigned long long) dc->start_read,
		       dc->read_delay);
		if (dc->dev_write)
			DMEMIT(" %s %llu %u", dc->dev_write->name,
			       (unsigned long long) dc->start_write,
			       dc->write_delay);
=======
		DMEMIT("%u %u %u", dc->read.ops, dc->write.ops, dc->flush.ops);
		break;

	case STATUSTYPE_TABLE:
		DMEMIT_DELAY_CLASS(&dc->read);
		if (dc->argc >= 6) {
			DMEMIT(" ");
			DMEMIT_DELAY_CLASS(&dc->write);
		}
		if (dc->argc >= 9) {
			DMEMIT(" ");
			DMEMIT_DELAY_CLASS(&dc->flush);
		}
		break;

	case STATUSTYPE_IMA:
		*result = '\0';
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

static int delay_iterate_devices(struct dm_target *ti,
				 iterate_devices_callout_fn fn, void *data)
{
	struct delay_c *dc = ti->private;
	int ret = 0;

<<<<<<< HEAD
	ret = fn(ti, dc->dev_read, dc->start_read, ti->len, data);
	if (ret)
		goto out;

	if (dc->dev_write)
		ret = fn(ti, dc->dev_write, dc->start_write, ti->len, data);
=======
	ret = fn(ti, dc->read.dev, dc->read.start, ti->len, data);
	if (ret)
		goto out;
	ret = fn(ti, dc->write.dev, dc->write.start, ti->len, data);
	if (ret)
		goto out;
	ret = fn(ti, dc->flush.dev, dc->flush.start, ti->len, data);
	if (ret)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	return ret;
}

static struct target_type delay_target = {
	.name	     = "delay",
<<<<<<< HEAD
	.version     = {1, 1, 0},
=======
	.version     = {1, 4, 0},
	.features    = DM_TARGET_PASSES_INTEGRITY,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.module      = THIS_MODULE,
	.ctr	     = delay_ctr,
	.dtr	     = delay_dtr,
	.map	     = delay_map,
	.presuspend  = delay_presuspend,
	.resume	     = delay_resume,
	.status	     = delay_status,
	.iterate_devices = delay_iterate_devices,
};
<<<<<<< HEAD

static int __init dm_delay_init(void)
{
	int r = -ENOMEM;

	delayed_cache = KMEM_CACHE(dm_delay_info, 0);
	if (!delayed_cache) {
		DMERR("Couldn't create delayed bio cache.");
		goto bad_memcache;
	}

	r = dm_register_target(&delay_target);
	if (r < 0) {
		DMERR("register failed %d", r);
		goto bad_register;
	}

	return 0;

bad_register:
	kmem_cache_destroy(delayed_cache);
bad_memcache:
	return r;
}

static void __exit dm_delay_exit(void)
{
	dm_unregister_target(&delay_target);
	kmem_cache_destroy(delayed_cache);
}

/* Module hooks */
module_init(dm_delay_init);
module_exit(dm_delay_exit);
=======
module_dm(delay);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_DESCRIPTION(DM_NAME " delay target");
MODULE_AUTHOR("Heinz Mauelshagen <mauelshagen@redhat.com>");
MODULE_LICENSE("GPL");
