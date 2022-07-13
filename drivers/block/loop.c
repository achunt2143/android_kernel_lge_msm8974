<<<<<<< HEAD
/*
 *  linux/drivers/block/loop.c
 *
 *  Written by Theodore Ts'o, 3/29/93
 *
 * Copyright 1993 by Theodore Ts'o.  Redistribution of this file is
 * permitted under the GNU General Public License.
 *
 * DES encryption plus some minor changes by Werner Almesberger, 30-MAY-1993
 * more DES encryption plus IDEA encryption by Nicholas J. Leon, June 20, 1996
 *
 * Modularized and updated for 1.1.16 kernel - Mitch Dsouza 28th May 1994
 * Adapted for 1.3.59 kernel - Andries Brouwer, 1 Feb 1996
 *
 * Fixed do_loop_request() re-entrancy - Vincent.Renardias@waw.com Mar 20, 1997
 *
 * Added devfs support - Richard Gooch <rgooch@atnf.csiro.au> 16-Jan-1998
 *
 * Handle sparse backing files correctly - Kenn Humborg, Jun 28, 1998
 *
 * Loadable modules and other fixes by AK, 1998
 *
 * Make real block number available to downstream transfer functions, enables
 * CBC (and relatives) mode encryption requiring unique IVs per data block.
 * Reed H. Petty, rhp@draper.net
 *
 * Maximum number of loop devices now dynamic via max_loop module parameter.
 * Russell Kroll <rkroll@exploits.org> 19990701
 *
 * Maximum number of loop devices when compiled-in now selectable by passing
 * max_loop=<1-255> to the kernel on boot.
 * Erik I. Bolsø, <eriki@himolde.no>, Oct 31, 1999
 *
 * Completely rewrite request handling to be make_request_fn style and
 * non blocking, pushing work to a helper thread. Lots of fixes from
 * Al Viro too.
 * Jens Axboe <axboe@suse.de>, Nov 2000
 *
 * Support up to 256 loop devices
 * Heinz Mauelshagen <mge@sistina.com>, Feb 2002
 *
 * Support for falling back on the write file operation when the address space
 * operations write_begin is not available on the backing filesystem.
 * Anton Altaparmakov, 16 Feb 2005
 *
 * Still To Fix:
 * - Advisory locking is ignored here.
 * - Should use an own CAP_* category instead of CAP_SYS_ADMIN
 *
 */

=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright 1993 by Theodore Ts'o.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/fs.h>
<<<<<<< HEAD
=======
#include <linux/pagemap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/file.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/major.h>
#include <linux/wait.h>
<<<<<<< HEAD
#include <linux/blkdev.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/blkpg.h>
#include <linux/init.h>
#include <linux/swap.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/loop.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/compat.h>
#include <linux/suspend.h>
#include <linux/freezer.h>
#include <linux/mutex.h>
#include <linux/writeback.h>
#include <linux/completion.h>
#include <linux/highmem.h>
<<<<<<< HEAD
#include <linux/kthread.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/splice.h>
#include <linux/sysfs.h>
#include <linux/miscdevice.h>
#include <linux/falloc.h>
<<<<<<< HEAD

#include <asm/uaccess.h>

static DEFINE_IDR(loop_index_idr);
static DEFINE_MUTEX(loop_index_mutex);
=======
#include <linux/uio.h>
#include <linux/ioprio.h>
#include <linux/blk-cgroup.h>
#include <linux/sched/mm.h>
#include <linux/statfs.h>
#include <linux/uaccess.h>
#include <linux/blk-mq.h>
#include <linux/spinlock.h>
#include <uapi/linux/loop.h>

/* Possible states of device */
enum {
	Lo_unbound,
	Lo_bound,
	Lo_rundown,
	Lo_deleting,
};

struct loop_func_table;

struct loop_device {
	int		lo_number;
	loff_t		lo_offset;
	loff_t		lo_sizelimit;
	int		lo_flags;
	char		lo_file_name[LO_NAME_SIZE];

	struct file *	lo_backing_file;
	struct block_device *lo_device;

	gfp_t		old_gfp_mask;

	spinlock_t		lo_lock;
	int			lo_state;
	spinlock_t              lo_work_lock;
	struct workqueue_struct *workqueue;
	struct work_struct      rootcg_work;
	struct list_head        rootcg_cmd_list;
	struct list_head        idle_worker_list;
	struct rb_root          worker_tree;
	struct timer_list       timer;
	bool			use_dio;
	bool			sysfs_inited;

	struct request_queue	*lo_queue;
	struct blk_mq_tag_set	tag_set;
	struct gendisk		*lo_disk;
	struct mutex		lo_mutex;
	bool			idr_visible;
};

struct loop_cmd {
	struct list_head list_entry;
	bool use_aio; /* use AIO interface to handle I/O */
	atomic_t ref; /* only for aio */
	long ret;
	struct kiocb iocb;
	struct bio_vec *bvec;
	struct cgroup_subsys_state *blkcg_css;
	struct cgroup_subsys_state *memcg_css;
};

#define LOOP_IDLE_WORKER_TIMEOUT (60 * HZ)
#define LOOP_DEFAULT_HW_Q_DEPTH 128

static DEFINE_IDR(loop_index_idr);
static DEFINE_MUTEX(loop_ctl_mutex);
static DEFINE_MUTEX(loop_validate_mutex);

/**
 * loop_global_lock_killable() - take locks for safe loop_validate_file() test
 *
 * @lo: struct loop_device
 * @global: true if @lo is about to bind another "struct loop_device", false otherwise
 *
 * Returns 0 on success, -EINTR otherwise.
 *
 * Since loop_validate_file() traverses on other "struct loop_device" if
 * is_loop_device() is true, we need a global lock for serializing concurrent
 * loop_configure()/loop_change_fd()/__loop_clr_fd() calls.
 */
static int loop_global_lock_killable(struct loop_device *lo, bool global)
{
	int err;

	if (global) {
		err = mutex_lock_killable(&loop_validate_mutex);
		if (err)
			return err;
	}
	err = mutex_lock_killable(&lo->lo_mutex);
	if (err && global)
		mutex_unlock(&loop_validate_mutex);
	return err;
}

/**
 * loop_global_unlock() - release locks taken by loop_global_lock_killable()
 *
 * @lo: struct loop_device
 * @global: true if @lo was about to bind another "struct loop_device", false otherwise
 */
static void loop_global_unlock(struct loop_device *lo, bool global)
{
	mutex_unlock(&lo->lo_mutex);
	if (global)
		mutex_unlock(&loop_validate_mutex);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int max_part;
static int part_shift;

<<<<<<< HEAD
/*
 * Transfer functions
 */
static int transfer_none(struct loop_device *lo, int cmd,
			 struct page *raw_page, unsigned raw_off,
			 struct page *loop_page, unsigned loop_off,
			 int size, sector_t real_block)
{
	char *raw_buf = kmap_atomic(raw_page) + raw_off;
	char *loop_buf = kmap_atomic(loop_page) + loop_off;

	if (cmd == READ)
		memcpy(loop_buf, raw_buf, size);
	else
		memcpy(raw_buf, loop_buf, size);

	kunmap_atomic(loop_buf);
	kunmap_atomic(raw_buf);
	cond_resched();
	return 0;
}

static int transfer_xor(struct loop_device *lo, int cmd,
			struct page *raw_page, unsigned raw_off,
			struct page *loop_page, unsigned loop_off,
			int size, sector_t real_block)
{
	char *raw_buf = kmap_atomic(raw_page) + raw_off;
	char *loop_buf = kmap_atomic(loop_page) + loop_off;
	char *in, *out, *key;
	int i, keysize;

	if (cmd == READ) {
		in = raw_buf;
		out = loop_buf;
	} else {
		in = loop_buf;
		out = raw_buf;
	}

	key = lo->lo_encrypt_key;
	keysize = lo->lo_encrypt_key_size;
	for (i = 0; i < size; i++)
		*out++ = *in++ ^ key[(i & 511) % keysize];

	kunmap_atomic(loop_buf);
	kunmap_atomic(raw_buf);
	cond_resched();
	return 0;
}

static int xor_init(struct loop_device *lo, const struct loop_info64 *info)
{
	if (unlikely(info->lo_encrypt_key_size <= 0))
		return -EINVAL;
	return 0;
}

static struct loop_func_table none_funcs = {
	.number = LO_CRYPT_NONE,
	.transfer = transfer_none,
}; 	

static struct loop_func_table xor_funcs = {
	.number = LO_CRYPT_XOR,
	.transfer = transfer_xor,
	.init = xor_init
}; 	

/* xfer_funcs[0] is special - its release function is never called */
static struct loop_func_table *xfer_funcs[MAX_LO_CRYPT] = {
	&none_funcs,
	&xor_funcs
};

static loff_t get_size(loff_t offset, loff_t sizelimit, struct file *file)
{
	loff_t size, loopsize;

	/* Compute loopsize in bytes */
	size = i_size_read(file->f_mapping->host);
	loopsize = size - offset;
	/* offset is beyond i_size, wierd but possible */
=======
static loff_t get_size(loff_t offset, loff_t sizelimit, struct file *file)
{
	loff_t loopsize;

	/* Compute loopsize in bytes */
	loopsize = i_size_read(file->f_mapping->host);
	if (offset > 0)
		loopsize -= offset;
	/* offset is beyond i_size, weird but possible */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (loopsize < 0)
		return 0;

	if (sizelimit > 0 && sizelimit < loopsize)
		loopsize = sizelimit;
	/*
	 * Unfortunately, if we want to do I/O on the device,
	 * the number of 512-byte sectors has to fit into a sector_t.
	 */
	return loopsize >> 9;
}

static loff_t get_loop_size(struct loop_device *lo, struct file *file)
{
	return get_size(lo->lo_offset, lo->lo_sizelimit, file);
}

<<<<<<< HEAD
static int
figure_loop_size(struct loop_device *lo, loff_t offset, loff_t sizelimit)
{
	loff_t size = get_size(offset, sizelimit, lo->lo_backing_file);
	sector_t x = (sector_t)size;

	if (unlikely((loff_t)x != size))
		return -EFBIG;
	if (lo->lo_offset != offset)
		lo->lo_offset = offset;
	if (lo->lo_sizelimit != sizelimit)
		lo->lo_sizelimit = sizelimit;
	set_capacity(lo->lo_disk, x);
	return 0;
}

static inline int
lo_do_transfer(struct loop_device *lo, int cmd,
	       struct page *rpage, unsigned roffs,
	       struct page *lpage, unsigned loffs,
	       int size, sector_t rblock)
{
	if (unlikely(!lo->transfer))
		return 0;

	return lo->transfer(lo, cmd, rpage, roffs, lpage, loffs, size, rblock);
}

/**
 * __do_lo_send_write - helper for writing data to a loop device
 *
 * This helper just factors out common code between do_lo_send_direct_write()
 * and do_lo_send_write().
 */
static int __do_lo_send_write(struct file *file,
		u8 *buf, const int len, loff_t pos)
{
	ssize_t bw;
	mm_segment_t old_fs = get_fs();

	set_fs(get_ds());
	bw = file->f_op->write(file, buf, len, &pos);
	set_fs(old_fs);
	if (likely(bw == len))
		return 0;
	printk(KERN_ERR "loop: Write error at byte offset %llu, length %i.\n",
			(unsigned long long)pos, len);
=======
/*
 * We support direct I/O only if lo_offset is aligned with the logical I/O size
 * of backing device, and the logical block size of loop is bigger than that of
 * the backing device.
 */
static bool lo_bdev_can_use_dio(struct loop_device *lo,
		struct block_device *backing_bdev)
{
	unsigned short sb_bsize = bdev_logical_block_size(backing_bdev);

	if (queue_logical_block_size(lo->lo_queue) < sb_bsize)
		return false;
	if (lo->lo_offset & (sb_bsize - 1))
		return false;
	return true;
}

static void __loop_update_dio(struct loop_device *lo, bool dio)
{
	struct file *file = lo->lo_backing_file;
	struct inode *inode = file->f_mapping->host;
	struct block_device *backing_bdev = NULL;
	bool use_dio;

	if (S_ISBLK(inode->i_mode))
		backing_bdev = I_BDEV(inode);
	else if (inode->i_sb->s_bdev)
		backing_bdev = inode->i_sb->s_bdev;

	use_dio = dio && (file->f_mode & FMODE_CAN_ODIRECT) &&
		(!backing_bdev || lo_bdev_can_use_dio(lo, backing_bdev));

	if (lo->use_dio == use_dio)
		return;

	/* flush dirty pages before changing direct IO */
	vfs_fsync(file, 0);

	/*
	 * The flag of LO_FLAGS_DIRECT_IO is handled similarly with
	 * LO_FLAGS_READ_ONLY, both are set from kernel, and losetup
	 * will get updated by ioctl(LOOP_GET_STATUS)
	 */
	if (lo->lo_state == Lo_bound)
		blk_mq_freeze_queue(lo->lo_queue);
	lo->use_dio = use_dio;
	if (use_dio) {
		blk_queue_flag_clear(QUEUE_FLAG_NOMERGES, lo->lo_queue);
		lo->lo_flags |= LO_FLAGS_DIRECT_IO;
	} else {
		blk_queue_flag_set(QUEUE_FLAG_NOMERGES, lo->lo_queue);
		lo->lo_flags &= ~LO_FLAGS_DIRECT_IO;
	}
	if (lo->lo_state == Lo_bound)
		blk_mq_unfreeze_queue(lo->lo_queue);
}

/**
 * loop_set_size() - sets device size and notifies userspace
 * @lo: struct loop_device to set the size for
 * @size: new size of the loop device
 *
 * Callers must validate that the size passed into this function fits into
 * a sector_t, eg using loop_validate_size()
 */
static void loop_set_size(struct loop_device *lo, loff_t size)
{
	if (!set_capacity_and_notify(lo->lo_disk, size))
		kobject_uevent(&disk_to_dev(lo->lo_disk)->kobj, KOBJ_CHANGE);
}

static int lo_write_bvec(struct file *file, struct bio_vec *bvec, loff_t *ppos)
{
	struct iov_iter i;
	ssize_t bw;

	iov_iter_bvec(&i, ITER_SOURCE, bvec, 1, bvec->bv_len);

	bw = vfs_iter_write(file, &i, ppos, 0);

	if (likely(bw ==  bvec->bv_len))
		return 0;

	printk_ratelimited(KERN_ERR
		"loop: Write error at byte offset %llu, length %i.\n",
		(unsigned long long)*ppos, bvec->bv_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (bw >= 0)
		bw = -EIO;
	return bw;
}

<<<<<<< HEAD
/**
 * do_lo_send_direct_write - helper for writing data to a loop device
 *
 * This is the fast, non-transforming version that does not need double
 * buffering.
 */
static int do_lo_send_direct_write(struct loop_device *lo,
		struct bio_vec *bvec, loff_t pos, struct page *page)
{
	ssize_t bw = __do_lo_send_write(lo->lo_backing_file,
			kmap(bvec->bv_page) + bvec->bv_offset,
			bvec->bv_len, pos);
	kunmap(bvec->bv_page);
	cond_resched();
	return bw;
}

/**
 * do_lo_send_write - helper for writing data to a loop device
 *
 * This is the slow, transforming version that needs to double buffer the
 * data as it cannot do the transformations in place without having direct
 * access to the destination pages of the backing file.
 */
static int do_lo_send_write(struct loop_device *lo, struct bio_vec *bvec,
		loff_t pos, struct page *page)
{
	int ret = lo_do_transfer(lo, WRITE, page, 0, bvec->bv_page,
			bvec->bv_offset, bvec->bv_len, pos >> 9);
	if (likely(!ret))
		return __do_lo_send_write(lo->lo_backing_file,
				page_address(page), bvec->bv_len,
				pos);
	printk(KERN_ERR "loop: Transfer error at byte offset %llu, "
			"length %i.\n", (unsigned long long)pos, bvec->bv_len);
	if (ret > 0)
		ret = -EIO;
	return ret;
}

static int lo_send(struct loop_device *lo, struct bio *bio, loff_t pos)
{
	int (*do_lo_send)(struct loop_device *, struct bio_vec *, loff_t,
			struct page *page);
	struct bio_vec *bvec;
	struct page *page = NULL;
	int i, ret = 0;

	if (lo->transfer != transfer_none) {
		page = alloc_page(GFP_NOIO | __GFP_HIGHMEM);
		if (unlikely(!page))
			goto fail;
		kmap(page);
		do_lo_send = do_lo_send_write;
	} else {
		do_lo_send = do_lo_send_direct_write;
	}

	bio_for_each_segment(bvec, bio, i) {
		ret = do_lo_send(lo, bvec, pos, page);
		if (ret < 0)
			break;
		pos += bvec->bv_len;
	}
	if (page) {
		kunmap(page);
		__free_page(page);
	}
out:
	return ret;
fail:
	printk(KERN_ERR "loop: Failed to allocate temporary page for write.\n");
	ret = -ENOMEM;
	goto out;
}

struct lo_read_data {
	struct loop_device *lo;
	struct page *page;
	unsigned offset;
	int bsize;
};

static int
lo_splice_actor(struct pipe_inode_info *pipe, struct pipe_buffer *buf,
		struct splice_desc *sd)
{
	struct lo_read_data *p = sd->u.data;
	struct loop_device *lo = p->lo;
	struct page *page = buf->page;
	sector_t IV;
	int size;

	IV = ((sector_t) page->index << (PAGE_CACHE_SHIFT - 9)) +
							(buf->offset >> 9);
	size = sd->len;
	if (size > p->bsize)
		size = p->bsize;

	if (lo_do_transfer(lo, READ, page, buf->offset, p->page, p->offset, size, IV)) {
		printk(KERN_ERR "loop: transfer error block %ld\n",
		       page->index);
		size = -EINVAL;
	}

	flush_dcache_page(p->page);

	if (size > 0)
		p->offset += size;

	return size;
}

static int
lo_direct_splice_actor(struct pipe_inode_info *pipe, struct splice_desc *sd)
{
	return __splice_from_pipe(pipe, sd, lo_splice_actor);
}

static ssize_t
do_lo_receive(struct loop_device *lo,
	      struct bio_vec *bvec, int bsize, loff_t pos)
{
	struct lo_read_data cookie;
	struct splice_desc sd;
	struct file *file;
	ssize_t retval;

	cookie.lo = lo;
	cookie.page = bvec->bv_page;
	cookie.offset = bvec->bv_offset;
	cookie.bsize = bsize;

	sd.len = 0;
	sd.total_len = bvec->bv_len;
	sd.flags = 0;
	sd.pos = pos;
	sd.u.data = &cookie;

	file = lo->lo_backing_file;
	retval = splice_direct_to_actor(file, &sd, lo_direct_splice_actor);

	return retval;
}

static int
lo_receive(struct loop_device *lo, struct bio *bio, int bsize, loff_t pos)
{
	struct bio_vec *bvec;
	ssize_t s;
	int i;

	bio_for_each_segment(bvec, bio, i) {
		s = do_lo_receive(lo, bvec, bsize, pos);
		if (s < 0)
			return s;

		if (s != bvec->bv_len) {
			zero_fill_bio(bio);
			break;
		}
		pos += bvec->bv_len;
	}
	return 0;
}

static int do_bio_filebacked(struct loop_device *lo, struct bio *bio)
{
	loff_t pos;
	int ret;

	pos = ((loff_t) bio->bi_sector << 9) + lo->lo_offset;

	if (bio_rw(bio) == WRITE) {
		struct file *file = lo->lo_backing_file;

		if (bio->bi_rw & REQ_FLUSH) {
			ret = vfs_fsync(file, 0);
			if (unlikely(ret && ret != -EINVAL)) {
				ret = -EIO;
				goto out;
			}
		}

		/*
		 * We use punch hole to reclaim the free space used by the
		 * image a.k.a. discard. However we do not support discard if
		 * encryption is enabled, because it may give an attacker
		 * useful information.
		 */
		if (bio->bi_rw & REQ_DISCARD) {
			struct file *file = lo->lo_backing_file;
			int mode = FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE;

			if ((!file->f_op->fallocate) ||
			    lo->lo_encrypt_key_size) {
				ret = -EOPNOTSUPP;
				goto out;
			}
			ret = file->f_op->fallocate(file, mode, pos,
						    bio->bi_size);
			if (unlikely(ret && ret != -EINVAL &&
				     ret != -EOPNOTSUPP))
				ret = -EIO;
			goto out;
		}

		ret = lo_send(lo, bio, pos);

		if ((bio->bi_rw & REQ_FUA) && !ret) {
			ret = vfs_fsync(file, 0);
			if (unlikely(ret && ret != -EINVAL))
				ret = -EIO;
		}
	} else
		ret = lo_receive(lo, bio, lo->lo_blocksize, pos);

out:
	return ret;
}

/*
 * Add bio to back of pending list
 */
static void loop_add_bio(struct loop_device *lo, struct bio *bio)
{
	bio_list_add(&lo->lo_bio_list, bio);
}

/*
 * Grab first pending buffer
 */
static struct bio *loop_get_bio(struct loop_device *lo)
{
	return bio_list_pop(&lo->lo_bio_list);
}

static void loop_make_request(struct request_queue *q, struct bio *old_bio)
{
	struct loop_device *lo = q->queuedata;
	int rw = bio_rw(old_bio);

	if (rw == READA)
		rw = READ;

	BUG_ON(!lo || (rw != READ && rw != WRITE));

	spin_lock_irq(&lo->lo_lock);
	if (lo->lo_state != Lo_bound)
		goto out;
	if (unlikely(rw == WRITE && (lo->lo_flags & LO_FLAGS_READ_ONLY)))
		goto out;
	loop_add_bio(lo, old_bio);
	wake_up(&lo->lo_event);
	spin_unlock_irq(&lo->lo_lock);
	return;

out:
	spin_unlock_irq(&lo->lo_lock);
	bio_io_error(old_bio);
}

struct switch_request {
	struct file *file;
	struct completion wait;
};

static void do_loop_switch(struct loop_device *, struct switch_request *);

static inline void loop_handle_bio(struct loop_device *lo, struct bio *bio)
{
	if (unlikely(!bio->bi_bdev)) {
		do_loop_switch(lo, bio->bi_private);
		bio_put(bio);
	} else {
		int ret = do_bio_filebacked(lo, bio);
		bio_endio(bio, ret);
	}
}

/*
 * worker thread that handles reads/writes to file backed loop devices,
 * to avoid blocking in our make_request_fn. it also does loop decrypting
 * on reads for block backed loop, as that is too heavy to do from
 * b_end_io context where irqs may be disabled.
 *
 * Loop explanation:  loop_clr_fd() sets lo_state to Lo_rundown before
 * calling kthread_stop().  Therefore once kthread_should_stop() is
 * true, make_request will not place any more requests.  Therefore
 * once kthread_should_stop() is true and lo_bio is NULL, we are
 * done with the loop.
 */
static int loop_thread(void *data)
{
	struct loop_device *lo = data;
	struct bio *bio;

	set_user_nice(current, -20);

	while (!kthread_should_stop() || !bio_list_empty(&lo->lo_bio_list)) {

		wait_event_interruptible(lo->lo_event,
				!bio_list_empty(&lo->lo_bio_list) ||
				kthread_should_stop());

		if (bio_list_empty(&lo->lo_bio_list))
			continue;
		spin_lock_irq(&lo->lo_lock);
		bio = loop_get_bio(lo);
		spin_unlock_irq(&lo->lo_lock);

		BUG_ON(!bio);
		loop_handle_bio(lo, bio);
	}

	return 0;
}

/*
 * loop_switch performs the hard work of switching a backing store.
 * First it needs to flush existing IO, it does this by sending a magic
 * BIO down the pipe. The completion of this BIO does the actual switch.
 */
static int loop_switch(struct loop_device *lo, struct file *file)
{
	struct switch_request w;
	struct bio *bio = bio_alloc(GFP_KERNEL, 0);
	if (!bio)
		return -ENOMEM;
	init_completion(&w.wait);
	w.file = file;
	bio->bi_private = &w;
	bio->bi_bdev = NULL;
	loop_make_request(lo->lo_queue, bio);
	wait_for_completion(&w.wait);
=======
static int lo_write_simple(struct loop_device *lo, struct request *rq,
		loff_t pos)
{
	struct bio_vec bvec;
	struct req_iterator iter;
	int ret = 0;

	rq_for_each_segment(bvec, rq, iter) {
		ret = lo_write_bvec(lo->lo_backing_file, &bvec, &pos);
		if (ret < 0)
			break;
		cond_resched();
	}

	return ret;
}

static int lo_read_simple(struct loop_device *lo, struct request *rq,
		loff_t pos)
{
	struct bio_vec bvec;
	struct req_iterator iter;
	struct iov_iter i;
	ssize_t len;

	rq_for_each_segment(bvec, rq, iter) {
		iov_iter_bvec(&i, ITER_DEST, &bvec, 1, bvec.bv_len);
		len = vfs_iter_read(lo->lo_backing_file, &i, &pos, 0);
		if (len < 0)
			return len;

		flush_dcache_page(bvec.bv_page);

		if (len != bvec.bv_len) {
			struct bio *bio;

			__rq_for_each_bio(bio, rq)
				zero_fill_bio(bio);
			break;
		}
		cond_resched();
	}

	return 0;
}

static int lo_fallocate(struct loop_device *lo, struct request *rq, loff_t pos,
			int mode)
{
	/*
	 * We use fallocate to manipulate the space mappings used by the image
	 * a.k.a. discard/zerorange.
	 */
	struct file *file = lo->lo_backing_file;
	int ret;

	mode |= FALLOC_FL_KEEP_SIZE;

	if (!bdev_max_discard_sectors(lo->lo_device))
		return -EOPNOTSUPP;

	ret = file->f_op->fallocate(file, mode, pos, blk_rq_bytes(rq));
	if (unlikely(ret && ret != -EINVAL && ret != -EOPNOTSUPP))
		return -EIO;
	return ret;
}

static int lo_req_flush(struct loop_device *lo, struct request *rq)
{
	int ret = vfs_fsync(lo->lo_backing_file, 0);
	if (unlikely(ret && ret != -EINVAL))
		ret = -EIO;

	return ret;
}

static void lo_complete_rq(struct request *rq)
{
	struct loop_cmd *cmd = blk_mq_rq_to_pdu(rq);
	blk_status_t ret = BLK_STS_OK;

	if (!cmd->use_aio || cmd->ret < 0 || cmd->ret == blk_rq_bytes(rq) ||
	    req_op(rq) != REQ_OP_READ) {
		if (cmd->ret < 0)
			ret = errno_to_blk_status(cmd->ret);
		goto end_io;
	}

	/*
	 * Short READ - if we got some data, advance our request and
	 * retry it. If we got no data, end the rest with EIO.
	 */
	if (cmd->ret) {
		blk_update_request(rq, BLK_STS_OK, cmd->ret);
		cmd->ret = 0;
		blk_mq_requeue_request(rq, true);
	} else {
		if (cmd->use_aio) {
			struct bio *bio = rq->bio;

			while (bio) {
				zero_fill_bio(bio);
				bio = bio->bi_next;
			}
		}
		ret = BLK_STS_IOERR;
end_io:
		blk_mq_end_request(rq, ret);
	}
}

static void lo_rw_aio_do_completion(struct loop_cmd *cmd)
{
	struct request *rq = blk_mq_rq_from_pdu(cmd);

	if (!atomic_dec_and_test(&cmd->ref))
		return;
	kfree(cmd->bvec);
	cmd->bvec = NULL;
	if (likely(!blk_should_fake_timeout(rq->q)))
		blk_mq_complete_request(rq);
}

static void lo_rw_aio_complete(struct kiocb *iocb, long ret)
{
	struct loop_cmd *cmd = container_of(iocb, struct loop_cmd, iocb);

	cmd->ret = ret;
	lo_rw_aio_do_completion(cmd);
}

static int lo_rw_aio(struct loop_device *lo, struct loop_cmd *cmd,
		     loff_t pos, int rw)
{
	struct iov_iter iter;
	struct req_iterator rq_iter;
	struct bio_vec *bvec;
	struct request *rq = blk_mq_rq_from_pdu(cmd);
	struct bio *bio = rq->bio;
	struct file *file = lo->lo_backing_file;
	struct bio_vec tmp;
	unsigned int offset;
	int nr_bvec = 0;
	int ret;

	rq_for_each_bvec(tmp, rq, rq_iter)
		nr_bvec++;

	if (rq->bio != rq->biotail) {

		bvec = kmalloc_array(nr_bvec, sizeof(struct bio_vec),
				     GFP_NOIO);
		if (!bvec)
			return -EIO;
		cmd->bvec = bvec;

		/*
		 * The bios of the request may be started from the middle of
		 * the 'bvec' because of bio splitting, so we can't directly
		 * copy bio->bi_iov_vec to new bvec. The rq_for_each_bvec
		 * API will take care of all details for us.
		 */
		rq_for_each_bvec(tmp, rq, rq_iter) {
			*bvec = tmp;
			bvec++;
		}
		bvec = cmd->bvec;
		offset = 0;
	} else {
		/*
		 * Same here, this bio may be started from the middle of the
		 * 'bvec' because of bio splitting, so offset from the bvec
		 * must be passed to iov iterator
		 */
		offset = bio->bi_iter.bi_bvec_done;
		bvec = __bvec_iter_bvec(bio->bi_io_vec, bio->bi_iter);
	}
	atomic_set(&cmd->ref, 2);

	iov_iter_bvec(&iter, rw, bvec, nr_bvec, blk_rq_bytes(rq));
	iter.iov_offset = offset;

	cmd->iocb.ki_pos = pos;
	cmd->iocb.ki_filp = file;
	cmd->iocb.ki_complete = lo_rw_aio_complete;
	cmd->iocb.ki_flags = IOCB_DIRECT;
	cmd->iocb.ki_ioprio = IOPRIO_PRIO_VALUE(IOPRIO_CLASS_NONE, 0);

	if (rw == ITER_SOURCE)
		ret = call_write_iter(file, &cmd->iocb, &iter);
	else
		ret = call_read_iter(file, &cmd->iocb, &iter);

	lo_rw_aio_do_completion(cmd);

	if (ret != -EIOCBQUEUED)
		lo_rw_aio_complete(&cmd->iocb, ret);
	return 0;
}

static int do_req_filebacked(struct loop_device *lo, struct request *rq)
{
	struct loop_cmd *cmd = blk_mq_rq_to_pdu(rq);
	loff_t pos = ((loff_t) blk_rq_pos(rq) << 9) + lo->lo_offset;

	/*
	 * lo_write_simple and lo_read_simple should have been covered
	 * by io submit style function like lo_rw_aio(), one blocker
	 * is that lo_read_simple() need to call flush_dcache_page after
	 * the page is written from kernel, and it isn't easy to handle
	 * this in io submit style function which submits all segments
	 * of the req at one time. And direct read IO doesn't need to
	 * run flush_dcache_page().
	 */
	switch (req_op(rq)) {
	case REQ_OP_FLUSH:
		return lo_req_flush(lo, rq);
	case REQ_OP_WRITE_ZEROES:
		/*
		 * If the caller doesn't want deallocation, call zeroout to
		 * write zeroes the range.  Otherwise, punch them out.
		 */
		return lo_fallocate(lo, rq, pos,
			(rq->cmd_flags & REQ_NOUNMAP) ?
				FALLOC_FL_ZERO_RANGE :
				FALLOC_FL_PUNCH_HOLE);
	case REQ_OP_DISCARD:
		return lo_fallocate(lo, rq, pos, FALLOC_FL_PUNCH_HOLE);
	case REQ_OP_WRITE:
		if (cmd->use_aio)
			return lo_rw_aio(lo, cmd, pos, ITER_SOURCE);
		else
			return lo_write_simple(lo, rq, pos);
	case REQ_OP_READ:
		if (cmd->use_aio)
			return lo_rw_aio(lo, cmd, pos, ITER_DEST);
		else
			return lo_read_simple(lo, rq, pos);
	default:
		WARN_ON_ONCE(1);
		return -EIO;
	}
}

static inline void loop_update_dio(struct loop_device *lo)
{
	__loop_update_dio(lo, (lo->lo_backing_file->f_flags & O_DIRECT) |
				lo->use_dio);
}

static void loop_reread_partitions(struct loop_device *lo)
{
	int rc;

	mutex_lock(&lo->lo_disk->open_mutex);
	rc = bdev_disk_changed(lo->lo_disk, false);
	mutex_unlock(&lo->lo_disk->open_mutex);
	if (rc)
		pr_warn("%s: partition scan of loop%d (%s) failed (rc=%d)\n",
			__func__, lo->lo_number, lo->lo_file_name, rc);
}

static inline int is_loop_device(struct file *file)
{
	struct inode *i = file->f_mapping->host;

	return i && S_ISBLK(i->i_mode) && imajor(i) == LOOP_MAJOR;
}

static int loop_validate_file(struct file *file, struct block_device *bdev)
{
	struct inode	*inode = file->f_mapping->host;
	struct file	*f = file;

	/* Avoid recursion */
	while (is_loop_device(f)) {
		struct loop_device *l;

		lockdep_assert_held(&loop_validate_mutex);
		if (f->f_mapping->host->i_rdev == bdev->bd_dev)
			return -EBADF;

		l = I_BDEV(f->f_mapping->host)->bd_disk->private_data;
		if (l->lo_state != Lo_bound)
			return -EINVAL;
		/* Order wrt setting lo->lo_backing_file in loop_configure(). */
		rmb();
		f = l->lo_backing_file;
	}
	if (!S_ISREG(inode->i_mode) && !S_ISBLK(inode->i_mode))
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
<<<<<<< HEAD
 * Helper to flush the IOs in loop, but keeping loop thread running
 */
static int loop_flush(struct loop_device *lo)
{
	/* loop not yet configured, no running thread, nothing to flush */
	if (!lo->lo_thread)
		return 0;

	return loop_switch(lo, NULL);
}

/*
 * Do the actual switch; called from the BIO completion routine
 */
static void do_loop_switch(struct loop_device *lo, struct switch_request *p)
{
	struct file *file = p->file;
	struct file *old_file = lo->lo_backing_file;
	struct address_space *mapping;

	/* if no new file, only flush of queued bios requested */
	if (!file)
		goto out;

	mapping = file->f_mapping;
	mapping_set_gfp_mask(old_file->f_mapping, lo->old_gfp_mask);
	lo->lo_backing_file = file;
	lo->lo_blocksize = S_ISBLK(mapping->host->i_mode) ?
		mapping->host->i_bdev->bd_block_size : PAGE_SIZE;
	lo->old_gfp_mask = mapping_gfp_mask(mapping);
	mapping_set_gfp_mask(mapping, lo->old_gfp_mask & ~(__GFP_IO|__GFP_FS));
out:
	complete(&p->wait);
}


/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * loop_change_fd switched the backing store of a loopback device to
 * a new file. This is useful for operating system installers to free up
 * the original file and in High Availability environments to switch to
 * an alternative location for the content in case of server meltdown.
 * This can only work if the loop device is used read-only, and if the
 * new backing store is the same size and type as the old backing store.
 */
static int loop_change_fd(struct loop_device *lo, struct block_device *bdev,
			  unsigned int arg)
{
<<<<<<< HEAD
	struct file	*file, *old_file;
	struct inode	*inode;
	int		error;

	error = -ENXIO;
	if (lo->lo_state != Lo_bound)
		goto out;
=======
	struct file *file = fget(arg);
	struct file *old_file;
	int error;
	bool partscan;
	bool is_loop;

	if (!file)
		return -EBADF;

	/* suppress uevents while reconfiguring the device */
	dev_set_uevent_suppress(disk_to_dev(lo->lo_disk), 1);

	is_loop = is_loop_device(file);
	error = loop_global_lock_killable(lo, is_loop);
	if (error)
		goto out_putf;
	error = -ENXIO;
	if (lo->lo_state != Lo_bound)
		goto out_err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* the loop device has to be read-only */
	error = -EINVAL;
	if (!(lo->lo_flags & LO_FLAGS_READ_ONLY))
<<<<<<< HEAD
		goto out;

	error = -EBADF;
	file = fget(arg);
	if (!file)
		goto out;

	inode = file->f_mapping->host;
=======
		goto out_err;

	error = loop_validate_file(file, bdev);
	if (error)
		goto out_err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	old_file = lo->lo_backing_file;

	error = -EINVAL;

<<<<<<< HEAD
	if (!S_ISREG(inode->i_mode) && !S_ISBLK(inode->i_mode))
		goto out_putf;

	/* size of the new backing store needs to be the same */
	if (get_loop_size(lo, file) != get_loop_size(lo, old_file))
		goto out_putf;

	/* and ... switch */
	error = loop_switch(lo, file);
	if (error)
		goto out_putf;

	fput(old_file);
	if (lo->lo_flags & LO_FLAGS_PARTSCAN)
		ioctl_by_bdev(bdev, BLKRRPART, 0);
	return 0;

 out_putf:
	fput(file);
 out:
	return error;
}

static inline int is_loop_device(struct file *file)
{
	struct inode *i = file->f_mapping->host;

	return i && S_ISBLK(i->i_mode) && MAJOR(i->i_rdev) == LOOP_MAJOR;
=======
	/* size of the new backing store needs to be the same */
	if (get_loop_size(lo, file) != get_loop_size(lo, old_file))
		goto out_err;

	/* and ... switch */
	disk_force_media_change(lo->lo_disk);
	blk_mq_freeze_queue(lo->lo_queue);
	mapping_set_gfp_mask(old_file->f_mapping, lo->old_gfp_mask);
	lo->lo_backing_file = file;
	lo->old_gfp_mask = mapping_gfp_mask(file->f_mapping);
	mapping_set_gfp_mask(file->f_mapping,
			     lo->old_gfp_mask & ~(__GFP_IO|__GFP_FS));
	loop_update_dio(lo);
	blk_mq_unfreeze_queue(lo->lo_queue);
	partscan = lo->lo_flags & LO_FLAGS_PARTSCAN;
	loop_global_unlock(lo, is_loop);

	/*
	 * Flush loop_validate_file() before fput(), for l->lo_backing_file
	 * might be pointing at old_file which might be the last reference.
	 */
	if (!is_loop) {
		mutex_lock(&loop_validate_mutex);
		mutex_unlock(&loop_validate_mutex);
	}
	/*
	 * We must drop file reference outside of lo_mutex as dropping
	 * the file ref can take open_mutex which creates circular locking
	 * dependency.
	 */
	fput(old_file);
	if (partscan)
		loop_reread_partitions(lo);

	error = 0;
done:
	/* enable and uncork uevent now that we are done */
	dev_set_uevent_suppress(disk_to_dev(lo->lo_disk), 0);
	return error;

out_err:
	loop_global_unlock(lo, is_loop);
out_putf:
	fput(file);
	goto done;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* loop sysfs attributes */

static ssize_t loop_attr_show(struct device *dev, char *page,
			      ssize_t (*callback)(struct loop_device *, char *))
{
	struct gendisk *disk = dev_to_disk(dev);
	struct loop_device *lo = disk->private_data;

	return callback(lo, page);
}

#define LOOP_ATTR_RO(_name)						\
static ssize_t loop_attr_##_name##_show(struct loop_device *, char *);	\
static ssize_t loop_attr_do_show_##_name(struct device *d,		\
				struct device_attribute *attr, char *b)	\
{									\
	return loop_attr_show(d, b, loop_attr_##_name##_show);		\
}									\
static struct device_attribute loop_attr_##_name =			\
<<<<<<< HEAD
	__ATTR(_name, S_IRUGO, loop_attr_do_show_##_name, NULL);
=======
	__ATTR(_name, 0444, loop_attr_do_show_##_name, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t loop_attr_backing_file_show(struct loop_device *lo, char *buf)
{
	ssize_t ret;
	char *p = NULL;

	spin_lock_irq(&lo->lo_lock);
	if (lo->lo_backing_file)
<<<<<<< HEAD
		p = d_path(&lo->lo_backing_file->f_path, buf, PAGE_SIZE - 1);
=======
		p = file_path(lo->lo_backing_file, buf, PAGE_SIZE - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irq(&lo->lo_lock);

	if (IS_ERR_OR_NULL(p))
		ret = PTR_ERR(p);
	else {
		ret = strlen(p);
		memmove(buf, p, ret);
		buf[ret++] = '\n';
		buf[ret] = 0;
	}

	return ret;
}

static ssize_t loop_attr_offset_show(struct loop_device *lo, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%llu\n", (unsigned long long)lo->lo_offset);
=======
	return sysfs_emit(buf, "%llu\n", (unsigned long long)lo->lo_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t loop_attr_sizelimit_show(struct loop_device *lo, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%llu\n", (unsigned long long)lo->lo_sizelimit);
=======
	return sysfs_emit(buf, "%llu\n", (unsigned long long)lo->lo_sizelimit);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t loop_attr_autoclear_show(struct loop_device *lo, char *buf)
{
	int autoclear = (lo->lo_flags & LO_FLAGS_AUTOCLEAR);

<<<<<<< HEAD
	return sprintf(buf, "%s\n", autoclear ? "1" : "0");
=======
	return sysfs_emit(buf, "%s\n", autoclear ? "1" : "0");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t loop_attr_partscan_show(struct loop_device *lo, char *buf)
{
	int partscan = (lo->lo_flags & LO_FLAGS_PARTSCAN);

<<<<<<< HEAD
	return sprintf(buf, "%s\n", partscan ? "1" : "0");
=======
	return sysfs_emit(buf, "%s\n", partscan ? "1" : "0");
}

static ssize_t loop_attr_dio_show(struct loop_device *lo, char *buf)
{
	int dio = (lo->lo_flags & LO_FLAGS_DIRECT_IO);

	return sysfs_emit(buf, "%s\n", dio ? "1" : "0");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

LOOP_ATTR_RO(backing_file);
LOOP_ATTR_RO(offset);
LOOP_ATTR_RO(sizelimit);
LOOP_ATTR_RO(autoclear);
LOOP_ATTR_RO(partscan);
<<<<<<< HEAD
=======
LOOP_ATTR_RO(dio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *loop_attrs[] = {
	&loop_attr_backing_file.attr,
	&loop_attr_offset.attr,
	&loop_attr_sizelimit.attr,
	&loop_attr_autoclear.attr,
	&loop_attr_partscan.attr,
<<<<<<< HEAD
=======
	&loop_attr_dio.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	NULL,
};

static struct attribute_group loop_attribute_group = {
	.name = "loop",
	.attrs= loop_attrs,
};

<<<<<<< HEAD
static int loop_sysfs_init(struct loop_device *lo)
{
	return sysfs_create_group(&disk_to_dev(lo->lo_disk)->kobj,
				  &loop_attribute_group);
=======
static void loop_sysfs_init(struct loop_device *lo)
{
	lo->sysfs_inited = !sysfs_create_group(&disk_to_dev(lo->lo_disk)->kobj,
						&loop_attribute_group);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void loop_sysfs_exit(struct loop_device *lo)
{
<<<<<<< HEAD
	sysfs_remove_group(&disk_to_dev(lo->lo_disk)->kobj,
			   &loop_attribute_group);
}

static void loop_config_discard(struct loop_device *lo)
{
	struct file *file = lo->lo_backing_file;
	struct inode *inode = file->f_mapping->host;
	struct request_queue *q = lo->lo_queue;

	/*
	 * We use punch hole to reclaim the free space used by the
	 * image a.k.a. discard. However we do support discard if
	 * encryption is enabled, because it may give an attacker
	 * useful information.
	 */
	if ((!file->f_op->fallocate) ||
	    lo->lo_encrypt_key_size) {
		q->limits.discard_granularity = 0;
		q->limits.discard_alignment = 0;
		q->limits.max_discard_sectors = 0;
		q->limits.discard_zeroes_data = 0;
		queue_flag_clear_unlocked(QUEUE_FLAG_DISCARD, q);
		return;
	}

	q->limits.discard_granularity = inode->i_sb->s_blocksize;
	q->limits.discard_alignment = 0;
	q->limits.max_discard_sectors = UINT_MAX >> 9;
	q->limits.discard_zeroes_data = 1;
	queue_flag_set_unlocked(QUEUE_FLAG_DISCARD, q);
}

static int loop_set_fd(struct loop_device *lo, fmode_t mode,
		       struct block_device *bdev, unsigned int arg)
{
	struct file	*file, *f;
	struct inode	*inode;
	struct address_space *mapping;
	unsigned lo_blocksize;
	int		lo_flags = 0;
	int		error;
	loff_t		size;
=======
	if (lo->sysfs_inited)
		sysfs_remove_group(&disk_to_dev(lo->lo_disk)->kobj,
				   &loop_attribute_group);
}

static void loop_config_discard(struct loop_device *lo,
		struct queue_limits *lim)
{
	struct file *file = lo->lo_backing_file;
	struct inode *inode = file->f_mapping->host;
	u32 granularity = 0, max_discard_sectors = 0;
	struct kstatfs sbuf;

	/*
	 * If the backing device is a block device, mirror its zeroing
	 * capability. Set the discard sectors to the block device's zeroing
	 * capabilities because loop discards result in blkdev_issue_zeroout(),
	 * not blkdev_issue_discard(). This maintains consistent behavior with
	 * file-backed loop devices: discarded regions read back as zero.
	 */
	if (S_ISBLK(inode->i_mode)) {
		struct request_queue *backingq = bdev_get_queue(I_BDEV(inode));

		max_discard_sectors = backingq->limits.max_write_zeroes_sectors;
		granularity = bdev_discard_granularity(I_BDEV(inode)) ?:
			queue_physical_block_size(backingq);

	/*
	 * We use punch hole to reclaim the free space used by the
	 * image a.k.a. discard.
	 */
	} else if (file->f_op->fallocate && !vfs_statfs(&file->f_path, &sbuf)) {
		max_discard_sectors = UINT_MAX >> 9;
		granularity = sbuf.f_bsize;
	}

	lim->max_hw_discard_sectors = max_discard_sectors;
	lim->max_write_zeroes_sectors = max_discard_sectors;
	if (max_discard_sectors)
		lim->discard_granularity = granularity;
	else
		lim->discard_granularity = 0;
}

struct loop_worker {
	struct rb_node rb_node;
	struct work_struct work;
	struct list_head cmd_list;
	struct list_head idle_list;
	struct loop_device *lo;
	struct cgroup_subsys_state *blkcg_css;
	unsigned long last_ran_at;
};

static void loop_workfn(struct work_struct *work);

#ifdef CONFIG_BLK_CGROUP
static inline int queue_on_root_worker(struct cgroup_subsys_state *css)
{
	return !css || css == blkcg_root_css;
}
#else
static inline int queue_on_root_worker(struct cgroup_subsys_state *css)
{
	return !css;
}
#endif

static void loop_queue_work(struct loop_device *lo, struct loop_cmd *cmd)
{
	struct rb_node **node, *parent = NULL;
	struct loop_worker *cur_worker, *worker = NULL;
	struct work_struct *work;
	struct list_head *cmd_list;

	spin_lock_irq(&lo->lo_work_lock);

	if (queue_on_root_worker(cmd->blkcg_css))
		goto queue_work;

	node = &lo->worker_tree.rb_node;

	while (*node) {
		parent = *node;
		cur_worker = container_of(*node, struct loop_worker, rb_node);
		if (cur_worker->blkcg_css == cmd->blkcg_css) {
			worker = cur_worker;
			break;
		} else if ((long)cur_worker->blkcg_css < (long)cmd->blkcg_css) {
			node = &(*node)->rb_left;
		} else {
			node = &(*node)->rb_right;
		}
	}
	if (worker)
		goto queue_work;

	worker = kzalloc(sizeof(struct loop_worker), GFP_NOWAIT | __GFP_NOWARN);
	/*
	 * In the event we cannot allocate a worker, just queue on the
	 * rootcg worker and issue the I/O as the rootcg
	 */
	if (!worker) {
		cmd->blkcg_css = NULL;
		if (cmd->memcg_css)
			css_put(cmd->memcg_css);
		cmd->memcg_css = NULL;
		goto queue_work;
	}

	worker->blkcg_css = cmd->blkcg_css;
	css_get(worker->blkcg_css);
	INIT_WORK(&worker->work, loop_workfn);
	INIT_LIST_HEAD(&worker->cmd_list);
	INIT_LIST_HEAD(&worker->idle_list);
	worker->lo = lo;
	rb_link_node(&worker->rb_node, parent, node);
	rb_insert_color(&worker->rb_node, &lo->worker_tree);
queue_work:
	if (worker) {
		/*
		 * We need to remove from the idle list here while
		 * holding the lock so that the idle timer doesn't
		 * free the worker
		 */
		if (!list_empty(&worker->idle_list))
			list_del_init(&worker->idle_list);
		work = &worker->work;
		cmd_list = &worker->cmd_list;
	} else {
		work = &lo->rootcg_work;
		cmd_list = &lo->rootcg_cmd_list;
	}
	list_add_tail(&cmd->list_entry, cmd_list);
	queue_work(lo->workqueue, work);
	spin_unlock_irq(&lo->lo_work_lock);
}

static void loop_set_timer(struct loop_device *lo)
{
	timer_reduce(&lo->timer, jiffies + LOOP_IDLE_WORKER_TIMEOUT);
}

static void loop_free_idle_workers(struct loop_device *lo, bool delete_all)
{
	struct loop_worker *pos, *worker;

	spin_lock_irq(&lo->lo_work_lock);
	list_for_each_entry_safe(worker, pos, &lo->idle_worker_list,
				idle_list) {
		if (!delete_all &&
		    time_is_after_jiffies(worker->last_ran_at +
					  LOOP_IDLE_WORKER_TIMEOUT))
			break;
		list_del(&worker->idle_list);
		rb_erase(&worker->rb_node, &lo->worker_tree);
		css_put(worker->blkcg_css);
		kfree(worker);
	}
	if (!list_empty(&lo->idle_worker_list))
		loop_set_timer(lo);
	spin_unlock_irq(&lo->lo_work_lock);
}

static void loop_free_idle_workers_timer(struct timer_list *timer)
{
	struct loop_device *lo = container_of(timer, struct loop_device, timer);

	return loop_free_idle_workers(lo, false);
}

static void loop_update_rotational(struct loop_device *lo)
{
	struct file *file = lo->lo_backing_file;
	struct inode *file_inode = file->f_mapping->host;
	struct block_device *file_bdev = file_inode->i_sb->s_bdev;
	struct request_queue *q = lo->lo_queue;
	bool nonrot = true;

	/* not all filesystems (e.g. tmpfs) have a sb->s_bdev */
	if (file_bdev)
		nonrot = bdev_nonrot(file_bdev);

	if (nonrot)
		blk_queue_flag_set(QUEUE_FLAG_NONROT, q);
	else
		blk_queue_flag_clear(QUEUE_FLAG_NONROT, q);
}

/**
 * loop_set_status_from_info - configure device from loop_info
 * @lo: struct loop_device to configure
 * @info: struct loop_info64 to configure the device with
 *
 * Configures the loop device parameters according to the passed
 * in loop_info64 configuration.
 */
static int
loop_set_status_from_info(struct loop_device *lo,
			  const struct loop_info64 *info)
{
	if ((unsigned int) info->lo_encrypt_key_size > LO_KEY_SIZE)
		return -EINVAL;

	switch (info->lo_encrypt_type) {
	case LO_CRYPT_NONE:
		break;
	case LO_CRYPT_XOR:
		pr_warn("support for the xor transformation has been removed.\n");
		return -EINVAL;
	case LO_CRYPT_CRYPTOAPI:
		pr_warn("support for cryptoloop has been removed.  Use dm-crypt instead.\n");
		return -EINVAL;
	default:
		return -EINVAL;
	}

	/* Avoid assigning overflow values */
	if (info->lo_offset > LLONG_MAX || info->lo_sizelimit > LLONG_MAX)
		return -EOVERFLOW;

	lo->lo_offset = info->lo_offset;
	lo->lo_sizelimit = info->lo_sizelimit;

	memcpy(lo->lo_file_name, info->lo_file_name, LO_NAME_SIZE);
	lo->lo_file_name[LO_NAME_SIZE-1] = 0;
	lo->lo_flags = info->lo_flags;
	return 0;
}

static int loop_reconfigure_limits(struct loop_device *lo, unsigned short bsize,
		bool update_discard_settings)
{
	struct queue_limits lim;

	lim = queue_limits_start_update(lo->lo_queue);
	lim.logical_block_size = bsize;
	lim.physical_block_size = bsize;
	lim.io_min = bsize;
	if (update_discard_settings)
		loop_config_discard(lo, &lim);
	return queue_limits_commit_update(lo->lo_queue, &lim);
}

static int loop_configure(struct loop_device *lo, blk_mode_t mode,
			  struct block_device *bdev,
			  const struct loop_config *config)
{
	struct file *file = fget(config->fd);
	struct inode *inode;
	struct address_space *mapping;
	int error;
	loff_t size;
	bool partscan;
	unsigned short bsize;
	bool is_loop;

	if (!file)
		return -EBADF;
	is_loop = is_loop_device(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* This is safe, since we have a reference from open(). */
	__module_get(THIS_MODULE);

<<<<<<< HEAD
	error = -EBADF;
	file = fget(arg);
	if (!file)
		goto out;

	error = -EBUSY;
	if (lo->lo_state != Lo_unbound)
		goto out_putf;

	/* Avoid recursion */
	f = file;
	while (is_loop_device(f)) {
		struct loop_device *l;

		if (f->f_mapping->host->i_bdev == bdev)
			goto out_putf;

		l = f->f_mapping->host->i_bdev->bd_disk->private_data;
		if (l->lo_state == Lo_unbound) {
			error = -EINVAL;
			goto out_putf;
		}
		f = l->lo_backing_file;
	}
=======
	/*
	 * If we don't hold exclusive handle for the device, upgrade to it
	 * here to avoid changing device under exclusive owner.
	 */
	if (!(mode & BLK_OPEN_EXCL)) {
		error = bd_prepare_to_claim(bdev, loop_configure, NULL);
		if (error)
			goto out_putf;
	}

	error = loop_global_lock_killable(lo, is_loop);
	if (error)
		goto out_bdev;

	error = -EBUSY;
	if (lo->lo_state != Lo_unbound)
		goto out_unlock;

	error = loop_validate_file(file, bdev);
	if (error)
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mapping = file->f_mapping;
	inode = mapping->host;

<<<<<<< HEAD
	error = -EINVAL;
	if (!S_ISREG(inode->i_mode) && !S_ISBLK(inode->i_mode))
		goto out_putf;

	if (!(file->f_mode & FMODE_WRITE) || !(mode & FMODE_WRITE) ||
	    !file->f_op->write)
		lo_flags |= LO_FLAGS_READ_ONLY;

	lo_blocksize = S_ISBLK(inode->i_mode) ?
		inode->i_bdev->bd_block_size : PAGE_SIZE;

	error = -EFBIG;
	size = get_loop_size(lo, file);
	if ((loff_t)(sector_t)size != size)
		goto out_putf;

	error = 0;

	set_device_ro(bdev, (lo_flags & LO_FLAGS_READ_ONLY) != 0);

	lo->lo_blocksize = lo_blocksize;
	lo->lo_device = bdev;
	lo->lo_flags = lo_flags;
	lo->lo_backing_file = file;
	lo->transfer = transfer_none;
	lo->ioctl = NULL;
	lo->lo_sizelimit = 0;
	lo->old_gfp_mask = mapping_gfp_mask(mapping);
	mapping_set_gfp_mask(mapping, lo->old_gfp_mask & ~(__GFP_IO|__GFP_FS));

	bio_list_init(&lo->lo_bio_list);

	/*
	 * set queue make_request_fn, and add limits based on lower level
	 * device
	 */
	blk_queue_make_request(lo->lo_queue, loop_make_request);
	lo->lo_queue->queuedata = lo;

	if (!(lo_flags & LO_FLAGS_READ_ONLY) && file->f_op->fsync)
		blk_queue_flush(lo->lo_queue, REQ_FLUSH);

	set_capacity(lo->lo_disk, size);
	bd_set_size(bdev, size << 9);
	loop_sysfs_init(lo);
	/* let user-space know about the new size */
	kobject_uevent(&disk_to_dev(bdev->bd_disk)->kobj, KOBJ_CHANGE);

	set_blocksize(bdev, lo_blocksize);

	lo->lo_thread = kthread_create(loop_thread, lo, "loop%d",
						lo->lo_number);
	if (IS_ERR(lo->lo_thread)) {
		error = PTR_ERR(lo->lo_thread);
		goto out_clr;
	}
	lo->lo_state = Lo_bound;
	wake_up_process(lo->lo_thread);
	if (part_shift)
		lo->lo_flags |= LO_FLAGS_PARTSCAN;
	if (lo->lo_flags & LO_FLAGS_PARTSCAN)
		ioctl_by_bdev(bdev, BLKRRPART, 0);

	/* Grab the block_device to prevent its destruction after we
	 * put /dev/loopXX inode. Later in loop_clr_fd() we bdput(bdev).
	 */
	bdgrab(bdev);
	return 0;

out_clr:
	loop_sysfs_exit(lo);
	lo->lo_thread = NULL;
	lo->lo_device = NULL;
	lo->lo_backing_file = NULL;
	lo->lo_flags = 0;
	set_capacity(lo->lo_disk, 0);
	invalidate_bdev(bdev);
	bd_set_size(bdev, 0);
	kobject_uevent(&disk_to_dev(bdev->bd_disk)->kobj, KOBJ_CHANGE);
	mapping_set_gfp_mask(mapping, lo->old_gfp_mask);
	lo->lo_state = Lo_unbound;
 out_putf:
	fput(file);
 out:
=======
	if ((config->info.lo_flags & ~LOOP_CONFIGURE_SETTABLE_FLAGS) != 0) {
		error = -EINVAL;
		goto out_unlock;
	}

	if (config->block_size) {
		error = blk_validate_block_size(config->block_size);
		if (error)
			goto out_unlock;
	}

	error = loop_set_status_from_info(lo, &config->info);
	if (error)
		goto out_unlock;

	if (!(file->f_mode & FMODE_WRITE) || !(mode & BLK_OPEN_WRITE) ||
	    !file->f_op->write_iter)
		lo->lo_flags |= LO_FLAGS_READ_ONLY;

	if (!lo->workqueue) {
		lo->workqueue = alloc_workqueue("loop%d",
						WQ_UNBOUND | WQ_FREEZABLE,
						0, lo->lo_number);
		if (!lo->workqueue) {
			error = -ENOMEM;
			goto out_unlock;
		}
	}

	/* suppress uevents while reconfiguring the device */
	dev_set_uevent_suppress(disk_to_dev(lo->lo_disk), 1);

	disk_force_media_change(lo->lo_disk);
	set_disk_ro(lo->lo_disk, (lo->lo_flags & LO_FLAGS_READ_ONLY) != 0);

	lo->use_dio = lo->lo_flags & LO_FLAGS_DIRECT_IO;
	lo->lo_device = bdev;
	lo->lo_backing_file = file;
	lo->old_gfp_mask = mapping_gfp_mask(mapping);
	mapping_set_gfp_mask(mapping, lo->old_gfp_mask & ~(__GFP_IO|__GFP_FS));

	if (!(lo->lo_flags & LO_FLAGS_READ_ONLY) && file->f_op->fsync)
		blk_queue_write_cache(lo->lo_queue, true, false);

	if (config->block_size)
		bsize = config->block_size;
	else if ((lo->lo_backing_file->f_flags & O_DIRECT) && inode->i_sb->s_bdev)
		/* In case of direct I/O, match underlying block size */
		bsize = bdev_logical_block_size(inode->i_sb->s_bdev);
	else
		bsize = 512;

	error = loop_reconfigure_limits(lo, bsize, true);
	if (WARN_ON_ONCE(error))
		goto out_unlock;

	loop_update_rotational(lo);
	loop_update_dio(lo);
	loop_sysfs_init(lo);

	size = get_loop_size(lo, file);
	loop_set_size(lo, size);

	/* Order wrt reading lo_state in loop_validate_file(). */
	wmb();

	lo->lo_state = Lo_bound;
	if (part_shift)
		lo->lo_flags |= LO_FLAGS_PARTSCAN;
	partscan = lo->lo_flags & LO_FLAGS_PARTSCAN;
	if (partscan)
		clear_bit(GD_SUPPRESS_PART_SCAN, &lo->lo_disk->state);

	/* enable and uncork uevent now that we are done */
	dev_set_uevent_suppress(disk_to_dev(lo->lo_disk), 0);

	loop_global_unlock(lo, is_loop);
	if (partscan)
		loop_reread_partitions(lo);

	if (!(mode & BLK_OPEN_EXCL))
		bd_abort_claiming(bdev, loop_configure);

	return 0;

out_unlock:
	loop_global_unlock(lo, is_loop);
out_bdev:
	if (!(mode & BLK_OPEN_EXCL))
		bd_abort_claiming(bdev, loop_configure);
out_putf:
	fput(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* This is safe: open() is still holding a reference. */
	module_put(THIS_MODULE);
	return error;
}

<<<<<<< HEAD
static int
loop_release_xfer(struct loop_device *lo)
{
	int err = 0;
	struct loop_func_table *xfer = lo->lo_encryption;

	if (xfer) {
		if (xfer->release)
			err = xfer->release(lo);
		lo->transfer = NULL;
		lo->lo_encryption = NULL;
		module_put(xfer->owner);
	}
	return err;
}

static int
loop_init_xfer(struct loop_device *lo, struct loop_func_table *xfer,
	       const struct loop_info64 *i)
{
	int err = 0;

	if (xfer) {
		struct module *owner = xfer->owner;

		if (!try_module_get(owner))
			return -EINVAL;
		if (xfer->init)
			err = xfer->init(lo, i);
		if (err)
			module_put(owner);
		else
			lo->lo_encryption = xfer;
	}
	return err;
=======
static void __loop_clr_fd(struct loop_device *lo, bool release)
{
	struct file *filp;
	gfp_t gfp = lo->old_gfp_mask;

	if (test_bit(QUEUE_FLAG_WC, &lo->lo_queue->queue_flags))
		blk_queue_write_cache(lo->lo_queue, false, false);

	/*
	 * Freeze the request queue when unbinding on a live file descriptor and
	 * thus an open device.  When called from ->release we are guaranteed
	 * that there is no I/O in progress already.
	 */
	if (!release)
		blk_mq_freeze_queue(lo->lo_queue);

	spin_lock_irq(&lo->lo_lock);
	filp = lo->lo_backing_file;
	lo->lo_backing_file = NULL;
	spin_unlock_irq(&lo->lo_lock);

	lo->lo_device = NULL;
	lo->lo_offset = 0;
	lo->lo_sizelimit = 0;
	memset(lo->lo_file_name, 0, LO_NAME_SIZE);
	loop_reconfigure_limits(lo, 512, false);
	invalidate_disk(lo->lo_disk);
	loop_sysfs_exit(lo);
	/* let user-space know about this change */
	kobject_uevent(&disk_to_dev(lo->lo_disk)->kobj, KOBJ_CHANGE);
	mapping_set_gfp_mask(filp->f_mapping, gfp);
	/* This is safe: open() is still holding a reference. */
	module_put(THIS_MODULE);
	if (!release)
		blk_mq_unfreeze_queue(lo->lo_queue);

	disk_force_media_change(lo->lo_disk);

	if (lo->lo_flags & LO_FLAGS_PARTSCAN) {
		int err;

		/*
		 * open_mutex has been held already in release path, so don't
		 * acquire it if this function is called in such case.
		 *
		 * If the reread partition isn't from release path, lo_refcnt
		 * must be at least one and it can only become zero when the
		 * current holder is released.
		 */
		if (!release)
			mutex_lock(&lo->lo_disk->open_mutex);
		err = bdev_disk_changed(lo->lo_disk, false);
		if (!release)
			mutex_unlock(&lo->lo_disk->open_mutex);
		if (err)
			pr_warn("%s: partition scan of loop%d failed (rc=%d)\n",
				__func__, lo->lo_number, err);
		/* Device is gone, no point in returning error */
	}

	/*
	 * lo->lo_state is set to Lo_unbound here after above partscan has
	 * finished. There cannot be anybody else entering __loop_clr_fd() as
	 * Lo_rundown state protects us from all the other places trying to
	 * change the 'lo' device.
	 */
	lo->lo_flags = 0;
	if (!part_shift)
		set_bit(GD_SUPPRESS_PART_SCAN, &lo->lo_disk->state);
	mutex_lock(&lo->lo_mutex);
	lo->lo_state = Lo_unbound;
	mutex_unlock(&lo->lo_mutex);

	/*
	 * Need not hold lo_mutex to fput backing file. Calling fput holding
	 * lo_mutex triggers a circular lock dependency possibility warning as
	 * fput can take open_mutex which is usually taken before lo_mutex.
	 */
	fput(filp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int loop_clr_fd(struct loop_device *lo)
{
<<<<<<< HEAD
	struct file *filp = lo->lo_backing_file;
	gfp_t gfp = lo->old_gfp_mask;
	struct block_device *bdev = lo->lo_device;

	if (lo->lo_state != Lo_bound)
		return -ENXIO;

	if (lo->lo_refcnt > 1)	/* we needed one fd for the ioctl */
		return -EBUSY;

	if (filp == NULL)
		return -EINVAL;

	spin_lock_irq(&lo->lo_lock);
	lo->lo_state = Lo_rundown;
	spin_unlock_irq(&lo->lo_lock);

	kthread_stop(lo->lo_thread);

	spin_lock_irq(&lo->lo_lock);
	lo->lo_backing_file = NULL;
	spin_unlock_irq(&lo->lo_lock);

	loop_release_xfer(lo);
	lo->transfer = NULL;
	lo->ioctl = NULL;
	lo->lo_device = NULL;
	lo->lo_encryption = NULL;
	lo->lo_offset = 0;
	lo->lo_sizelimit = 0;
	lo->lo_encrypt_key_size = 0;
	lo->lo_thread = NULL;
	memset(lo->lo_encrypt_key, 0, LO_KEY_SIZE);
	memset(lo->lo_crypt_name, 0, LO_NAME_SIZE);
	memset(lo->lo_file_name, 0, LO_NAME_SIZE);
	if (bdev) {
		bdput(bdev);
		invalidate_bdev(bdev);
	}
	set_capacity(lo->lo_disk, 0);
	loop_sysfs_exit(lo);
	if (bdev) {
		bd_set_size(bdev, 0);
		/* let user-space know about this change */
		kobject_uevent(&disk_to_dev(bdev->bd_disk)->kobj, KOBJ_CHANGE);
	}
	mapping_set_gfp_mask(filp->f_mapping, gfp);
	lo->lo_state = Lo_unbound;
	/* This is safe: open() is still holding a reference. */
	module_put(THIS_MODULE);
	if (lo->lo_flags & LO_FLAGS_PARTSCAN && bdev)
		ioctl_by_bdev(bdev, BLKRRPART, 0);
	lo->lo_flags = 0;
	if (!part_shift)
		lo->lo_disk->flags |= GENHD_FL_NO_PART_SCAN;
	mutex_unlock(&lo->lo_ctl_mutex);
	/*
	 * Need not hold lo_ctl_mutex to fput backing file.
	 * Calling fput holding lo_ctl_mutex triggers a circular
	 * lock dependency possibility warning as fput can take
	 * bd_mutex which is usually taken before lo_ctl_mutex.
	 */
	fput(filp);
=======
	int err;

	/*
	 * Since lo_ioctl() is called without locks held, it is possible that
	 * loop_configure()/loop_change_fd() and loop_clr_fd() run in parallel.
	 *
	 * Therefore, use global lock when setting Lo_rundown state in order to
	 * make sure that loop_validate_file() will fail if the "struct file"
	 * which loop_configure()/loop_change_fd() found via fget() was this
	 * loop device.
	 */
	err = loop_global_lock_killable(lo, true);
	if (err)
		return err;
	if (lo->lo_state != Lo_bound) {
		loop_global_unlock(lo, true);
		return -ENXIO;
	}
	/*
	 * If we've explicitly asked to tear down the loop device,
	 * and it has an elevated reference count, set it for auto-teardown when
	 * the last reference goes away. This stops $!~#$@ udev from
	 * preventing teardown because it decided that it needs to run blkid on
	 * the loopback device whenever they appear. xfstests is notorious for
	 * failing tests because blkid via udev races with a losetup
	 * <dev>/do something like mkfs/losetup -d <dev> causing the losetup -d
	 * command to fail with EBUSY.
	 */
	if (disk_openers(lo->lo_disk) > 1) {
		lo->lo_flags |= LO_FLAGS_AUTOCLEAR;
		loop_global_unlock(lo, true);
		return 0;
	}
	lo->lo_state = Lo_rundown;
	loop_global_unlock(lo, true);

	__loop_clr_fd(lo, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int
loop_set_status(struct loop_device *lo, const struct loop_info64 *info)
{
	int err;
<<<<<<< HEAD
	struct loop_func_table *xfer;
	uid_t uid = current_uid();

	if (lo->lo_encrypt_key_size &&
	    lo->lo_key_owner != uid &&
	    !capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (lo->lo_state != Lo_bound)
		return -ENXIO;
	if ((unsigned int) info->lo_encrypt_key_size > LO_KEY_SIZE)
		return -EINVAL;

	err = loop_release_xfer(lo);
	if (err)
		return err;

	if (info->lo_encrypt_type) {
		unsigned int type = info->lo_encrypt_type;

		if (type >= MAX_LO_CRYPT)
			return -EINVAL;
		xfer = xfer_funcs[type];
		if (xfer == NULL)
			return -EINVAL;
	} else
		xfer = NULL;

	err = loop_init_xfer(lo, xfer, info);
	if (err)
		return err;

	if (lo->lo_offset != info->lo_offset ||
	    lo->lo_sizelimit != info->lo_sizelimit) {
		if (figure_loop_size(lo, info->lo_offset, info->lo_sizelimit))
			return -EFBIG;
	}
	loop_config_discard(lo);

	memcpy(lo->lo_file_name, info->lo_file_name, LO_NAME_SIZE);
	memcpy(lo->lo_crypt_name, info->lo_crypt_name, LO_NAME_SIZE);
	lo->lo_file_name[LO_NAME_SIZE-1] = 0;
	lo->lo_crypt_name[LO_NAME_SIZE-1] = 0;

	if (!xfer)
		xfer = &none_funcs;
	lo->transfer = xfer->transfer;
	lo->ioctl = xfer->ioctl;

	if ((lo->lo_flags & LO_FLAGS_AUTOCLEAR) !=
	     (info->lo_flags & LO_FLAGS_AUTOCLEAR))
		lo->lo_flags ^= LO_FLAGS_AUTOCLEAR;

	if ((info->lo_flags & LO_FLAGS_PARTSCAN) &&
	     !(lo->lo_flags & LO_FLAGS_PARTSCAN)) {
		lo->lo_flags |= LO_FLAGS_PARTSCAN;
		lo->lo_disk->flags &= ~GENHD_FL_NO_PART_SCAN;
		ioctl_by_bdev(lo->lo_device, BLKRRPART, 0);
	}

	lo->lo_encrypt_key_size = info->lo_encrypt_key_size;
	lo->lo_init[0] = info->lo_init[0];
	lo->lo_init[1] = info->lo_init[1];
	if (info->lo_encrypt_key_size) {
		memcpy(lo->lo_encrypt_key, info->lo_encrypt_key,
		       info->lo_encrypt_key_size);
		lo->lo_key_owner = uid;
	}	

	return 0;
=======
	int prev_lo_flags;
	bool partscan = false;
	bool size_changed = false;

	err = mutex_lock_killable(&lo->lo_mutex);
	if (err)
		return err;
	if (lo->lo_state != Lo_bound) {
		err = -ENXIO;
		goto out_unlock;
	}

	if (lo->lo_offset != info->lo_offset ||
	    lo->lo_sizelimit != info->lo_sizelimit) {
		size_changed = true;
		sync_blockdev(lo->lo_device);
		invalidate_bdev(lo->lo_device);
	}

	/* I/O need to be drained during transfer transition */
	blk_mq_freeze_queue(lo->lo_queue);

	prev_lo_flags = lo->lo_flags;

	err = loop_set_status_from_info(lo, info);
	if (err)
		goto out_unfreeze;

	/* Mask out flags that can't be set using LOOP_SET_STATUS. */
	lo->lo_flags &= LOOP_SET_STATUS_SETTABLE_FLAGS;
	/* For those flags, use the previous values instead */
	lo->lo_flags |= prev_lo_flags & ~LOOP_SET_STATUS_SETTABLE_FLAGS;
	/* For flags that can't be cleared, use previous values too */
	lo->lo_flags |= prev_lo_flags & ~LOOP_SET_STATUS_CLEARABLE_FLAGS;

	if (size_changed) {
		loff_t new_size = get_size(lo->lo_offset, lo->lo_sizelimit,
					   lo->lo_backing_file);
		loop_set_size(lo, new_size);
	}

	/* update dio if lo_offset or transfer is changed */
	__loop_update_dio(lo, lo->use_dio);

out_unfreeze:
	blk_mq_unfreeze_queue(lo->lo_queue);

	if (!err && (lo->lo_flags & LO_FLAGS_PARTSCAN) &&
	     !(prev_lo_flags & LO_FLAGS_PARTSCAN)) {
		clear_bit(GD_SUPPRESS_PART_SCAN, &lo->lo_disk->state);
		partscan = true;
	}
out_unlock:
	mutex_unlock(&lo->lo_mutex);
	if (partscan)
		loop_reread_partitions(lo);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
loop_get_status(struct loop_device *lo, struct loop_info64 *info)
{
<<<<<<< HEAD
	struct file *file = lo->lo_backing_file;
	struct kstat stat;
	int error;

	if (lo->lo_state != Lo_bound)
		return -ENXIO;
	error = vfs_getattr(file->f_path.mnt, file->f_path.dentry, &stat);
	if (error)
		return error;
	memset(info, 0, sizeof(*info));
	info->lo_number = lo->lo_number;
	info->lo_device = huge_encode_dev(stat.dev);
	info->lo_inode = stat.ino;
	info->lo_rdevice = huge_encode_dev(lo->lo_device ? stat.rdev : stat.dev);
=======
	struct path path;
	struct kstat stat;
	int ret;

	ret = mutex_lock_killable(&lo->lo_mutex);
	if (ret)
		return ret;
	if (lo->lo_state != Lo_bound) {
		mutex_unlock(&lo->lo_mutex);
		return -ENXIO;
	}

	memset(info, 0, sizeof(*info));
	info->lo_number = lo->lo_number;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	info->lo_offset = lo->lo_offset;
	info->lo_sizelimit = lo->lo_sizelimit;
	info->lo_flags = lo->lo_flags;
	memcpy(info->lo_file_name, lo->lo_file_name, LO_NAME_SIZE);
<<<<<<< HEAD
	memcpy(info->lo_crypt_name, lo->lo_crypt_name, LO_NAME_SIZE);
	info->lo_encrypt_type =
		lo->lo_encryption ? lo->lo_encryption->number : 0;
	if (lo->lo_encrypt_key_size && capable(CAP_SYS_ADMIN)) {
		info->lo_encrypt_key_size = lo->lo_encrypt_key_size;
		memcpy(info->lo_encrypt_key, lo->lo_encrypt_key,
		       lo->lo_encrypt_key_size);
	}
	return 0;
=======

	/* Drop lo_mutex while we call into the filesystem. */
	path = lo->lo_backing_file->f_path;
	path_get(&path);
	mutex_unlock(&lo->lo_mutex);
	ret = vfs_getattr(&path, &stat, STATX_INO, AT_STATX_SYNC_AS_STAT);
	if (!ret) {
		info->lo_device = huge_encode_dev(stat.dev);
		info->lo_inode = stat.ino;
		info->lo_rdevice = huge_encode_dev(stat.rdev);
	}
	path_put(&path);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
loop_info64_from_old(const struct loop_info *info, struct loop_info64 *info64)
{
	memset(info64, 0, sizeof(*info64));
	info64->lo_number = info->lo_number;
	info64->lo_device = info->lo_device;
	info64->lo_inode = info->lo_inode;
	info64->lo_rdevice = info->lo_rdevice;
	info64->lo_offset = info->lo_offset;
	info64->lo_sizelimit = 0;
<<<<<<< HEAD
	info64->lo_encrypt_type = info->lo_encrypt_type;
	info64->lo_encrypt_key_size = info->lo_encrypt_key_size;
	info64->lo_flags = info->lo_flags;
	info64->lo_init[0] = info->lo_init[0];
	info64->lo_init[1] = info->lo_init[1];
	if (info->lo_encrypt_type == LO_CRYPT_CRYPTOAPI)
		memcpy(info64->lo_crypt_name, info->lo_name, LO_NAME_SIZE);
	else
		memcpy(info64->lo_file_name, info->lo_name, LO_NAME_SIZE);
	memcpy(info64->lo_encrypt_key, info->lo_encrypt_key, LO_KEY_SIZE);
=======
	info64->lo_flags = info->lo_flags;
	memcpy(info64->lo_file_name, info->lo_name, LO_NAME_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
loop_info64_to_old(const struct loop_info64 *info64, struct loop_info *info)
{
	memset(info, 0, sizeof(*info));
	info->lo_number = info64->lo_number;
	info->lo_device = info64->lo_device;
	info->lo_inode = info64->lo_inode;
	info->lo_rdevice = info64->lo_rdevice;
	info->lo_offset = info64->lo_offset;
<<<<<<< HEAD
	info->lo_encrypt_type = info64->lo_encrypt_type;
	info->lo_encrypt_key_size = info64->lo_encrypt_key_size;
	info->lo_flags = info64->lo_flags;
	info->lo_init[0] = info64->lo_init[0];
	info->lo_init[1] = info64->lo_init[1];
	if (info->lo_encrypt_type == LO_CRYPT_CRYPTOAPI)
		memcpy(info->lo_name, info64->lo_crypt_name, LO_NAME_SIZE);
	else
		memcpy(info->lo_name, info64->lo_file_name, LO_NAME_SIZE);
	memcpy(info->lo_encrypt_key, info64->lo_encrypt_key, LO_KEY_SIZE);
=======
	info->lo_flags = info64->lo_flags;
	memcpy(info->lo_name, info64->lo_file_name, LO_NAME_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* error in case values were truncated */
	if (info->lo_device != info64->lo_device ||
	    info->lo_rdevice != info64->lo_rdevice ||
	    info->lo_inode != info64->lo_inode ||
	    info->lo_offset != info64->lo_offset)
		return -EOVERFLOW;

	return 0;
}

static int
loop_set_status_old(struct loop_device *lo, const struct loop_info __user *arg)
{
	struct loop_info info;
	struct loop_info64 info64;

	if (copy_from_user(&info, arg, sizeof (struct loop_info)))
		return -EFAULT;
	loop_info64_from_old(&info, &info64);
	return loop_set_status(lo, &info64);
}

static int
loop_set_status64(struct loop_device *lo, const struct loop_info64 __user *arg)
{
	struct loop_info64 info64;

	if (copy_from_user(&info64, arg, sizeof (struct loop_info64)))
		return -EFAULT;
	return loop_set_status(lo, &info64);
}

static int
loop_get_status_old(struct loop_device *lo, struct loop_info __user *arg) {
	struct loop_info info;
	struct loop_info64 info64;
<<<<<<< HEAD
	int err = 0;

	if (!arg)
		err = -EINVAL;
	if (!err)
		err = loop_get_status(lo, &info64);
=======
	int err;

	if (!arg)
		return -EINVAL;
	err = loop_get_status(lo, &info64);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!err)
		err = loop_info64_to_old(&info64, &info);
	if (!err && copy_to_user(arg, &info, sizeof(info)))
		err = -EFAULT;

	return err;
}

static int
loop_get_status64(struct loop_device *lo, struct loop_info64 __user *arg) {
	struct loop_info64 info64;
<<<<<<< HEAD
	int err = 0;

	if (!arg)
		err = -EINVAL;
	if (!err)
		err = loop_get_status(lo, &info64);
=======
	int err;

	if (!arg)
		return -EINVAL;
	err = loop_get_status(lo, &info64);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!err && copy_to_user(arg, &info64, sizeof(info64)))
		err = -EFAULT;

	return err;
}

<<<<<<< HEAD
static int loop_set_capacity(struct loop_device *lo, struct block_device *bdev)
{
	int err;
	sector_t sec;
	loff_t sz;

	err = -ENXIO;
	if (unlikely(lo->lo_state != Lo_bound))
		goto out;
	err = figure_loop_size(lo, lo->lo_offset, lo->lo_sizelimit);
	if (unlikely(err))
		goto out;
	sec = get_capacity(lo->lo_disk);
	/* the width of sector_t may be narrow for bit-shift */
	sz = sec;
	sz <<= 9;
	bd_set_size(bdev, sz);
	/* let user-space know about the new size */
	kobject_uevent(&disk_to_dev(bdev->bd_disk)->kobj, KOBJ_CHANGE);

 out:
	return err;
}

static int lo_ioctl(struct block_device *bdev, fmode_t mode,
	unsigned int cmd, unsigned long arg)
{
	struct loop_device *lo = bdev->bd_disk->private_data;
	int err;

	mutex_lock_nested(&lo->lo_ctl_mutex, 1);
	switch (cmd) {
	case LOOP_SET_FD:
		err = loop_set_fd(lo, mode, bdev, arg);
		break;
	case LOOP_CHANGE_FD:
		err = loop_change_fd(lo, bdev, arg);
		break;
	case LOOP_CLR_FD:
		/* loop_clr_fd would have unlocked lo_ctl_mutex on success */
		err = loop_clr_fd(lo);
		if (!err)
			goto out_unlocked;
		break;
	case LOOP_SET_STATUS:
		err = -EPERM;
		if ((mode & FMODE_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_status_old(lo,
					(struct loop_info __user *)arg);
		break;
	case LOOP_GET_STATUS:
		err = loop_get_status_old(lo, (struct loop_info __user *) arg);
		break;
	case LOOP_SET_STATUS64:
		err = -EPERM;
		if ((mode & FMODE_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_status64(lo,
					(struct loop_info64 __user *) arg);
		break;
	case LOOP_GET_STATUS64:
		err = loop_get_status64(lo, (struct loop_info64 __user *) arg);
		break;
	case LOOP_SET_CAPACITY:
		err = -EPERM;
		if ((mode & FMODE_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_capacity(lo, bdev);
		break;
	default:
		err = lo->ioctl ? lo->ioctl(lo, cmd, arg) : -EINVAL;
	}
	mutex_unlock(&lo->lo_ctl_mutex);

out_unlocked:
=======
static int loop_set_capacity(struct loop_device *lo)
{
	loff_t size;

	if (unlikely(lo->lo_state != Lo_bound))
		return -ENXIO;

	size = get_loop_size(lo, lo->lo_backing_file);
	loop_set_size(lo, size);

	return 0;
}

static int loop_set_dio(struct loop_device *lo, unsigned long arg)
{
	int error = -ENXIO;
	if (lo->lo_state != Lo_bound)
		goto out;

	__loop_update_dio(lo, !!arg);
	if (lo->use_dio == !!arg)
		return 0;
	error = -EINVAL;
 out:
	return error;
}

static int loop_set_block_size(struct loop_device *lo, unsigned long arg)
{
	int err = 0;

	if (lo->lo_state != Lo_bound)
		return -ENXIO;

	err = blk_validate_block_size(arg);
	if (err)
		return err;

	if (lo->lo_queue->limits.logical_block_size == arg)
		return 0;

	sync_blockdev(lo->lo_device);
	invalidate_bdev(lo->lo_device);

	blk_mq_freeze_queue(lo->lo_queue);
	err = loop_reconfigure_limits(lo, arg, false);
	loop_update_dio(lo);
	blk_mq_unfreeze_queue(lo->lo_queue);

	return err;
}

static int lo_simple_ioctl(struct loop_device *lo, unsigned int cmd,
			   unsigned long arg)
{
	int err;

	err = mutex_lock_killable(&lo->lo_mutex);
	if (err)
		return err;
	switch (cmd) {
	case LOOP_SET_CAPACITY:
		err = loop_set_capacity(lo);
		break;
	case LOOP_SET_DIRECT_IO:
		err = loop_set_dio(lo, arg);
		break;
	case LOOP_SET_BLOCK_SIZE:
		err = loop_set_block_size(lo, arg);
		break;
	default:
		err = -EINVAL;
	}
	mutex_unlock(&lo->lo_mutex);
	return err;
}

static int lo_ioctl(struct block_device *bdev, blk_mode_t mode,
	unsigned int cmd, unsigned long arg)
{
	struct loop_device *lo = bdev->bd_disk->private_data;
	void __user *argp = (void __user *) arg;
	int err;

	switch (cmd) {
	case LOOP_SET_FD: {
		/*
		 * Legacy case - pass in a zeroed out struct loop_config with
		 * only the file descriptor set , which corresponds with the
		 * default parameters we'd have used otherwise.
		 */
		struct loop_config config;

		memset(&config, 0, sizeof(config));
		config.fd = arg;

		return loop_configure(lo, mode, bdev, &config);
	}
	case LOOP_CONFIGURE: {
		struct loop_config config;

		if (copy_from_user(&config, argp, sizeof(config)))
			return -EFAULT;

		return loop_configure(lo, mode, bdev, &config);
	}
	case LOOP_CHANGE_FD:
		return loop_change_fd(lo, bdev, arg);
	case LOOP_CLR_FD:
		return loop_clr_fd(lo);
	case LOOP_SET_STATUS:
		err = -EPERM;
		if ((mode & BLK_OPEN_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_status_old(lo, argp);
		break;
	case LOOP_GET_STATUS:
		return loop_get_status_old(lo, argp);
	case LOOP_SET_STATUS64:
		err = -EPERM;
		if ((mode & BLK_OPEN_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_status64(lo, argp);
		break;
	case LOOP_GET_STATUS64:
		return loop_get_status64(lo, argp);
	case LOOP_SET_CAPACITY:
	case LOOP_SET_DIRECT_IO:
	case LOOP_SET_BLOCK_SIZE:
		if (!(mode & BLK_OPEN_WRITE) && !capable(CAP_SYS_ADMIN))
			return -EPERM;
		fallthrough;
	default:
		err = lo_simple_ioctl(lo, cmd, arg);
		break;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

#ifdef CONFIG_COMPAT
struct compat_loop_info {
	compat_int_t	lo_number;      /* ioctl r/o */
	compat_dev_t	lo_device;      /* ioctl r/o */
	compat_ulong_t	lo_inode;       /* ioctl r/o */
	compat_dev_t	lo_rdevice;     /* ioctl r/o */
	compat_int_t	lo_offset;
<<<<<<< HEAD
	compat_int_t	lo_encrypt_type;
=======
	compat_int_t	lo_encrypt_type;        /* obsolete, ignored */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	compat_int_t	lo_encrypt_key_size;    /* ioctl w/o */
	compat_int_t	lo_flags;       /* ioctl r/o */
	char		lo_name[LO_NAME_SIZE];
	unsigned char	lo_encrypt_key[LO_KEY_SIZE]; /* ioctl w/o */
	compat_ulong_t	lo_init[2];
	char		reserved[4];
};

/*
 * Transfer 32-bit compatibility structure in userspace to 64-bit loop info
 * - noinlined to reduce stack space usage in main part of driver
 */
static noinline int
loop_info64_from_compat(const struct compat_loop_info __user *arg,
			struct loop_info64 *info64)
{
	struct compat_loop_info info;

	if (copy_from_user(&info, arg, sizeof(info)))
		return -EFAULT;

	memset(info64, 0, sizeof(*info64));
	info64->lo_number = info.lo_number;
	info64->lo_device = info.lo_device;
	info64->lo_inode = info.lo_inode;
	info64->lo_rdevice = info.lo_rdevice;
	info64->lo_offset = info.lo_offset;
	info64->lo_sizelimit = 0;
<<<<<<< HEAD
	info64->lo_encrypt_type = info.lo_encrypt_type;
	info64->lo_encrypt_key_size = info.lo_encrypt_key_size;
	info64->lo_flags = info.lo_flags;
	info64->lo_init[0] = info.lo_init[0];
	info64->lo_init[1] = info.lo_init[1];
	if (info.lo_encrypt_type == LO_CRYPT_CRYPTOAPI)
		memcpy(info64->lo_crypt_name, info.lo_name, LO_NAME_SIZE);
	else
		memcpy(info64->lo_file_name, info.lo_name, LO_NAME_SIZE);
	memcpy(info64->lo_encrypt_key, info.lo_encrypt_key, LO_KEY_SIZE);
=======
	info64->lo_flags = info.lo_flags;
	memcpy(info64->lo_file_name, info.lo_name, LO_NAME_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Transfer 64-bit loop info to 32-bit compatibility structure in userspace
 * - noinlined to reduce stack space usage in main part of driver
 */
static noinline int
loop_info64_to_compat(const struct loop_info64 *info64,
		      struct compat_loop_info __user *arg)
{
	struct compat_loop_info info;

	memset(&info, 0, sizeof(info));
	info.lo_number = info64->lo_number;
	info.lo_device = info64->lo_device;
	info.lo_inode = info64->lo_inode;
	info.lo_rdevice = info64->lo_rdevice;
	info.lo_offset = info64->lo_offset;
<<<<<<< HEAD
	info.lo_encrypt_type = info64->lo_encrypt_type;
	info.lo_encrypt_key_size = info64->lo_encrypt_key_size;
	info.lo_flags = info64->lo_flags;
	info.lo_init[0] = info64->lo_init[0];
	info.lo_init[1] = info64->lo_init[1];
	if (info.lo_encrypt_type == LO_CRYPT_CRYPTOAPI)
		memcpy(info.lo_name, info64->lo_crypt_name, LO_NAME_SIZE);
	else
		memcpy(info.lo_name, info64->lo_file_name, LO_NAME_SIZE);
	memcpy(info.lo_encrypt_key, info64->lo_encrypt_key, LO_KEY_SIZE);
=======
	info.lo_flags = info64->lo_flags;
	memcpy(info.lo_name, info64->lo_file_name, LO_NAME_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* error in case values were truncated */
	if (info.lo_device != info64->lo_device ||
	    info.lo_rdevice != info64->lo_rdevice ||
	    info.lo_inode != info64->lo_inode ||
<<<<<<< HEAD
	    info.lo_offset != info64->lo_offset ||
	    info.lo_init[0] != info64->lo_init[0] ||
	    info.lo_init[1] != info64->lo_init[1])
=======
	    info.lo_offset != info64->lo_offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EOVERFLOW;

	if (copy_to_user(arg, &info, sizeof(info)))
		return -EFAULT;
	return 0;
}

static int
loop_set_status_compat(struct loop_device *lo,
		       const struct compat_loop_info __user *arg)
{
	struct loop_info64 info64;
	int ret;

	ret = loop_info64_from_compat(arg, &info64);
	if (ret < 0)
		return ret;
	return loop_set_status(lo, &info64);
}

static int
loop_get_status_compat(struct loop_device *lo,
		       struct compat_loop_info __user *arg)
{
	struct loop_info64 info64;
<<<<<<< HEAD
	int err = 0;

	if (!arg)
		err = -EINVAL;
	if (!err)
		err = loop_get_status(lo, &info64);
=======
	int err;

	if (!arg)
		return -EINVAL;
	err = loop_get_status(lo, &info64);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!err)
		err = loop_info64_to_compat(&info64, arg);
	return err;
}

<<<<<<< HEAD
static int lo_compat_ioctl(struct block_device *bdev, fmode_t mode,
=======
static int lo_compat_ioctl(struct block_device *bdev, blk_mode_t mode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   unsigned int cmd, unsigned long arg)
{
	struct loop_device *lo = bdev->bd_disk->private_data;
	int err;

	switch(cmd) {
	case LOOP_SET_STATUS:
<<<<<<< HEAD
		mutex_lock(&lo->lo_ctl_mutex);
		err = loop_set_status_compat(
			lo, (const struct compat_loop_info __user *) arg);
		mutex_unlock(&lo->lo_ctl_mutex);
		break;
	case LOOP_GET_STATUS:
		mutex_lock(&lo->lo_ctl_mutex);
		err = loop_get_status_compat(
			lo, (struct compat_loop_info __user *) arg);
		mutex_unlock(&lo->lo_ctl_mutex);
=======
		err = loop_set_status_compat(lo,
			     (const struct compat_loop_info __user *)arg);
		break;
	case LOOP_GET_STATUS:
		err = loop_get_status_compat(lo,
				     (struct compat_loop_info __user *)arg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case LOOP_SET_CAPACITY:
	case LOOP_CLR_FD:
	case LOOP_GET_STATUS64:
	case LOOP_SET_STATUS64:
<<<<<<< HEAD
		arg = (unsigned long) compat_ptr(arg);
	case LOOP_SET_FD:
	case LOOP_CHANGE_FD:
=======
	case LOOP_CONFIGURE:
		arg = (unsigned long) compat_ptr(arg);
		fallthrough;
	case LOOP_SET_FD:
	case LOOP_CHANGE_FD:
	case LOOP_SET_BLOCK_SIZE:
	case LOOP_SET_DIRECT_IO:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = lo_ioctl(bdev, mode, cmd, arg);
		break;
	default:
		err = -ENOIOCTLCMD;
		break;
	}
	return err;
}
#endif

<<<<<<< HEAD
static int lo_open(struct block_device *bdev, fmode_t mode)
{
	struct loop_device *lo;
	int err = 0;

	mutex_lock(&loop_index_mutex);
	lo = bdev->bd_disk->private_data;
	if (!lo) {
		err = -ENXIO;
		goto out;
	}

	mutex_lock(&lo->lo_ctl_mutex);
	lo->lo_refcnt++;
	mutex_unlock(&lo->lo_ctl_mutex);
out:
	mutex_unlock(&loop_index_mutex);
	return err;
}

static int __lo_release(struct loop_device *lo)
{
	int err;

	mutex_lock(&lo->lo_ctl_mutex);

	if (--lo->lo_refcnt)
		goto out;

	if (lo->lo_flags & LO_FLAGS_AUTOCLEAR) {
=======
static void lo_release(struct gendisk *disk)
{
	struct loop_device *lo = disk->private_data;

	if (disk_openers(disk) > 0)
		return;

	mutex_lock(&lo->lo_mutex);
	if (lo->lo_state == Lo_bound && (lo->lo_flags & LO_FLAGS_AUTOCLEAR)) {
		lo->lo_state = Lo_rundown;
		mutex_unlock(&lo->lo_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * In autoclear mode, stop the loop thread
		 * and remove configuration after last close.
		 */
<<<<<<< HEAD
		err = loop_clr_fd(lo);
		if (!err)
			goto out_unlocked;
	} else {
		/*
		 * Otherwise keep thread (if running) and config,
		 * but flush possible ongoing bios in thread.
		 */
		loop_flush(lo);
	}

out:
	mutex_unlock(&lo->lo_ctl_mutex);
out_unlocked:
	return 0;
}

static int lo_release(struct gendisk *disk, fmode_t mode)
{
	int err;
	mutex_lock(&loop_index_mutex);
	err =__lo_release(disk->private_data);
	mutex_unlock(&loop_index_mutex);
	return err;
=======
		__loop_clr_fd(lo, true);
		return;
	}
	mutex_unlock(&lo->lo_mutex);
}

static void lo_free_disk(struct gendisk *disk)
{
	struct loop_device *lo = disk->private_data;

	if (lo->workqueue)
		destroy_workqueue(lo->workqueue);
	loop_free_idle_workers(lo, true);
	timer_shutdown_sync(&lo->timer);
	mutex_destroy(&lo->lo_mutex);
	kfree(lo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct block_device_operations lo_fops = {
	.owner =	THIS_MODULE,
<<<<<<< HEAD
	.open =		lo_open,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.release =	lo_release,
	.ioctl =	lo_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl =	lo_compat_ioctl,
#endif
<<<<<<< HEAD
=======
	.free_disk =	lo_free_disk,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * And now the modules code and kernel interface.
 */
<<<<<<< HEAD
static int max_loop;
module_param(max_loop, int, S_IRUGO);
MODULE_PARM_DESC(max_loop, "Maximum number of loop devices");
module_param(max_part, int, S_IRUGO);
MODULE_PARM_DESC(max_part, "Maximum number of partitions per loop device");
MODULE_LICENSE("GPL");
MODULE_ALIAS_BLOCKDEV_MAJOR(LOOP_MAJOR);

int loop_register_transfer(struct loop_func_table *funcs)
{
	unsigned int n = funcs->number;

	if (n >= MAX_LO_CRYPT || xfer_funcs[n])
		return -EINVAL;
	xfer_funcs[n] = funcs;
	return 0;
}

static int unregister_transfer_cb(int id, void *ptr, void *data)
{
	struct loop_device *lo = ptr;
	struct loop_func_table *xfer = data;

	mutex_lock(&lo->lo_ctl_mutex);
	if (lo->lo_encryption == xfer)
		loop_release_xfer(lo);
	mutex_unlock(&lo->lo_ctl_mutex);
	return 0;
}

int loop_unregister_transfer(int number)
{
	unsigned int n = number;
	struct loop_func_table *xfer;

	if (n == 0 || n >= MAX_LO_CRYPT || (xfer = xfer_funcs[n]) == NULL)
		return -EINVAL;

	xfer_funcs[n] = NULL;
	idr_for_each(&loop_index_idr, &unregister_transfer_cb, xfer);
	return 0;
}

EXPORT_SYMBOL(loop_register_transfer);
EXPORT_SYMBOL(loop_unregister_transfer);

static int loop_add(struct loop_device **l, int i)
{
=======

/*
 * If max_loop is specified, create that many devices upfront.
 * This also becomes a hard limit. If max_loop is not specified,
 * the default isn't a hard limit (as before commit 85c50197716c
 * changed the default value from 0 for max_loop=0 reasons), just
 * create CONFIG_BLK_DEV_LOOP_MIN_COUNT loop devices at module
 * init time. Loop devices can be requested on-demand with the
 * /dev/loop-control interface, or be instantiated by accessing
 * a 'dead' device node.
 */
static int max_loop = CONFIG_BLK_DEV_LOOP_MIN_COUNT;

#ifdef CONFIG_BLOCK_LEGACY_AUTOLOAD
static bool max_loop_specified;

static int max_loop_param_set_int(const char *val,
				  const struct kernel_param *kp)
{
	int ret;

	ret = param_set_int(val, kp);
	if (ret < 0)
		return ret;

	max_loop_specified = true;
	return 0;
}

static const struct kernel_param_ops max_loop_param_ops = {
	.set = max_loop_param_set_int,
	.get = param_get_int,
};

module_param_cb(max_loop, &max_loop_param_ops, &max_loop, 0444);
MODULE_PARM_DESC(max_loop, "Maximum number of loop devices");
#else
module_param(max_loop, int, 0444);
MODULE_PARM_DESC(max_loop, "Initial number of loop devices");
#endif

module_param(max_part, int, 0444);
MODULE_PARM_DESC(max_part, "Maximum number of partitions per loop device");

static int hw_queue_depth = LOOP_DEFAULT_HW_Q_DEPTH;

static int loop_set_hw_queue_depth(const char *s, const struct kernel_param *p)
{
	int qd, ret;

	ret = kstrtoint(s, 0, &qd);
	if (ret < 0)
		return ret;
	if (qd < 1)
		return -EINVAL;
	hw_queue_depth = qd;
	return 0;
}

static const struct kernel_param_ops loop_hw_qdepth_param_ops = {
	.set	= loop_set_hw_queue_depth,
	.get	= param_get_int,
};

device_param_cb(hw_queue_depth, &loop_hw_qdepth_param_ops, &hw_queue_depth, 0444);
MODULE_PARM_DESC(hw_queue_depth, "Queue depth for each hardware queue. Default: " __stringify(LOOP_DEFAULT_HW_Q_DEPTH));

MODULE_LICENSE("GPL");
MODULE_ALIAS_BLOCKDEV_MAJOR(LOOP_MAJOR);

static blk_status_t loop_queue_rq(struct blk_mq_hw_ctx *hctx,
		const struct blk_mq_queue_data *bd)
{
	struct request *rq = bd->rq;
	struct loop_cmd *cmd = blk_mq_rq_to_pdu(rq);
	struct loop_device *lo = rq->q->queuedata;

	blk_mq_start_request(rq);

	if (lo->lo_state != Lo_bound)
		return BLK_STS_IOERR;

	switch (req_op(rq)) {
	case REQ_OP_FLUSH:
	case REQ_OP_DISCARD:
	case REQ_OP_WRITE_ZEROES:
		cmd->use_aio = false;
		break;
	default:
		cmd->use_aio = lo->use_dio;
		break;
	}

	/* always use the first bio's css */
	cmd->blkcg_css = NULL;
	cmd->memcg_css = NULL;
#ifdef CONFIG_BLK_CGROUP
	if (rq->bio) {
		cmd->blkcg_css = bio_blkcg_css(rq->bio);
#ifdef CONFIG_MEMCG
		if (cmd->blkcg_css) {
			cmd->memcg_css =
				cgroup_get_e_css(cmd->blkcg_css->cgroup,
						&memory_cgrp_subsys);
		}
#endif
	}
#endif
	loop_queue_work(lo, cmd);

	return BLK_STS_OK;
}

static void loop_handle_cmd(struct loop_cmd *cmd)
{
	struct cgroup_subsys_state *cmd_blkcg_css = cmd->blkcg_css;
	struct cgroup_subsys_state *cmd_memcg_css = cmd->memcg_css;
	struct request *rq = blk_mq_rq_from_pdu(cmd);
	const bool write = op_is_write(req_op(rq));
	struct loop_device *lo = rq->q->queuedata;
	int ret = 0;
	struct mem_cgroup *old_memcg = NULL;
	const bool use_aio = cmd->use_aio;

	if (write && (lo->lo_flags & LO_FLAGS_READ_ONLY)) {
		ret = -EIO;
		goto failed;
	}

	if (cmd_blkcg_css)
		kthread_associate_blkcg(cmd_blkcg_css);
	if (cmd_memcg_css)
		old_memcg = set_active_memcg(
			mem_cgroup_from_css(cmd_memcg_css));

	/*
	 * do_req_filebacked() may call blk_mq_complete_request() synchronously
	 * or asynchronously if using aio. Hence, do not touch 'cmd' after
	 * do_req_filebacked() has returned unless we are sure that 'cmd' has
	 * not yet been completed.
	 */
	ret = do_req_filebacked(lo, rq);

	if (cmd_blkcg_css)
		kthread_associate_blkcg(NULL);

	if (cmd_memcg_css) {
		set_active_memcg(old_memcg);
		css_put(cmd_memcg_css);
	}
 failed:
	/* complete non-aio request */
	if (!use_aio || ret) {
		if (ret == -EOPNOTSUPP)
			cmd->ret = ret;
		else
			cmd->ret = ret ? -EIO : 0;
		if (likely(!blk_should_fake_timeout(rq->q)))
			blk_mq_complete_request(rq);
	}
}

static void loop_process_work(struct loop_worker *worker,
			struct list_head *cmd_list, struct loop_device *lo)
{
	int orig_flags = current->flags;
	struct loop_cmd *cmd;

	current->flags |= PF_LOCAL_THROTTLE | PF_MEMALLOC_NOIO;
	spin_lock_irq(&lo->lo_work_lock);
	while (!list_empty(cmd_list)) {
		cmd = container_of(
			cmd_list->next, struct loop_cmd, list_entry);
		list_del(cmd_list->next);
		spin_unlock_irq(&lo->lo_work_lock);

		loop_handle_cmd(cmd);
		cond_resched();

		spin_lock_irq(&lo->lo_work_lock);
	}

	/*
	 * We only add to the idle list if there are no pending cmds
	 * *and* the worker will not run again which ensures that it
	 * is safe to free any worker on the idle list
	 */
	if (worker && !work_pending(&worker->work)) {
		worker->last_ran_at = jiffies;
		list_add_tail(&worker->idle_list, &lo->idle_worker_list);
		loop_set_timer(lo);
	}
	spin_unlock_irq(&lo->lo_work_lock);
	current->flags = orig_flags;
}

static void loop_workfn(struct work_struct *work)
{
	struct loop_worker *worker =
		container_of(work, struct loop_worker, work);
	loop_process_work(worker, &worker->cmd_list, worker->lo);
}

static void loop_rootcg_workfn(struct work_struct *work)
{
	struct loop_device *lo =
		container_of(work, struct loop_device, rootcg_work);
	loop_process_work(NULL, &lo->rootcg_cmd_list, lo);
}

static const struct blk_mq_ops loop_mq_ops = {
	.queue_rq       = loop_queue_rq,
	.complete	= lo_complete_rq,
};

static int loop_add(int i)
{
	struct queue_limits lim = {
		/*
		 * Random number picked from the historic block max_sectors cap.
		 */
		.max_hw_sectors		= 2560u,
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct loop_device *lo;
	struct gendisk *disk;
	int err;

<<<<<<< HEAD
	lo = kzalloc(sizeof(*lo), GFP_KERNEL);
	if (!lo) {
		err = -ENOMEM;
		goto out;
	}

	err = idr_pre_get(&loop_index_idr, GFP_KERNEL);
	if (err < 0)
		goto out_free_dev;

	if (i >= 0) {
		int m;

		/* create specific i in the index */
		err = idr_get_new_above(&loop_index_idr, lo, i, &m);
		if (err >= 0 && i != m) {
			idr_remove(&loop_index_idr, m);
			err = -EEXIST;
		}
	} else if (i == -1) {
		int m;

		/* get next free nr */
		err = idr_get_new(&loop_index_idr, lo, &m);
		if (err >= 0)
			i = m;
	} else {
		err = -EINVAL;
	}
	if (err < 0)
		goto out_free_dev;

	lo->lo_queue = blk_alloc_queue(GFP_KERNEL);
	if (!lo->lo_queue)
		goto out_free_idr;

	disk = lo->lo_disk = alloc_disk(1 << part_shift);
	if (!disk)
		goto out_free_queue;
=======
	err = -ENOMEM;
	lo = kzalloc(sizeof(*lo), GFP_KERNEL);
	if (!lo)
		goto out;
	lo->worker_tree = RB_ROOT;
	INIT_LIST_HEAD(&lo->idle_worker_list);
	timer_setup(&lo->timer, loop_free_idle_workers_timer, TIMER_DEFERRABLE);
	lo->lo_state = Lo_unbound;

	err = mutex_lock_killable(&loop_ctl_mutex);
	if (err)
		goto out_free_dev;

	/* allocate id, if @id >= 0, we're requesting that specific id */
	if (i >= 0) {
		err = idr_alloc(&loop_index_idr, lo, i, i + 1, GFP_KERNEL);
		if (err == -ENOSPC)
			err = -EEXIST;
	} else {
		err = idr_alloc(&loop_index_idr, lo, 0, 0, GFP_KERNEL);
	}
	mutex_unlock(&loop_ctl_mutex);
	if (err < 0)
		goto out_free_dev;
	i = err;

	lo->tag_set.ops = &loop_mq_ops;
	lo->tag_set.nr_hw_queues = 1;
	lo->tag_set.queue_depth = hw_queue_depth;
	lo->tag_set.numa_node = NUMA_NO_NODE;
	lo->tag_set.cmd_size = sizeof(struct loop_cmd);
	lo->tag_set.flags = BLK_MQ_F_SHOULD_MERGE | BLK_MQ_F_STACKING |
		BLK_MQ_F_NO_SCHED_BY_DEFAULT;
	lo->tag_set.driver_data = lo;

	err = blk_mq_alloc_tag_set(&lo->tag_set);
	if (err)
		goto out_free_idr;

	disk = lo->lo_disk = blk_mq_alloc_disk(&lo->tag_set, &lim, lo);
	if (IS_ERR(disk)) {
		err = PTR_ERR(disk);
		goto out_cleanup_tags;
	}
	lo->lo_queue = lo->lo_disk->queue;

	/*
	 * By default, we do buffer IO, so it doesn't make sense to enable
	 * merge because the I/O submitted to backing file is handled page by
	 * page. For directio mode, merge does help to dispatch bigger request
	 * to underlayer disk. We will enable merge once directio is enabled.
	 */
	blk_queue_flag_set(QUEUE_FLAG_NOMERGES, lo->lo_queue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Disable partition scanning by default. The in-kernel partition
	 * scanning can be requested individually per-device during its
	 * setup. Userspace can always add and remove partitions from all
	 * devices. The needed partition minors are allocated from the
	 * extended minor space, the main loop device numbers will continue
	 * to match the loop minors, regardless of the number of partitions
	 * used.
	 *
	 * If max_part is given, partition scanning is globally enabled for
	 * all loop devices. The minors for the main loop devices will be
	 * multiples of max_part.
	 *
	 * Note: Global-for-all-devices, set-only-at-init, read-only module
	 * parameteters like 'max_loop' and 'max_part' make things needlessly
	 * complicated, are too static, inflexible and may surprise
	 * userspace tools. Parameters like this in general should be avoided.
	 */
	if (!part_shift)
<<<<<<< HEAD
		disk->flags |= GENHD_FL_NO_PART_SCAN;
	disk->flags |= GENHD_FL_EXT_DEVT;
	mutex_init(&lo->lo_ctl_mutex);
	lo->lo_number		= i;
	lo->lo_thread		= NULL;
	init_waitqueue_head(&lo->lo_event);
	spin_lock_init(&lo->lo_lock);
	disk->major		= LOOP_MAJOR;
	disk->first_minor	= i << part_shift;
	disk->fops		= &lo_fops;
	disk->private_data	= lo;
	disk->queue		= lo->lo_queue;
	sprintf(disk->disk_name, "loop%d", i);
	add_disk(disk);
	*l = lo;
	return lo->lo_number;

out_free_queue:
	blk_cleanup_queue(lo->lo_queue);
out_free_idr:
	idr_remove(&loop_index_idr, i);
=======
		set_bit(GD_SUPPRESS_PART_SCAN, &disk->state);
	mutex_init(&lo->lo_mutex);
	lo->lo_number		= i;
	spin_lock_init(&lo->lo_lock);
	spin_lock_init(&lo->lo_work_lock);
	INIT_WORK(&lo->rootcg_work, loop_rootcg_workfn);
	INIT_LIST_HEAD(&lo->rootcg_cmd_list);
	disk->major		= LOOP_MAJOR;
	disk->first_minor	= i << part_shift;
	disk->minors		= 1 << part_shift;
	disk->fops		= &lo_fops;
	disk->private_data	= lo;
	disk->queue		= lo->lo_queue;
	disk->events		= DISK_EVENT_MEDIA_CHANGE;
	disk->event_flags	= DISK_EVENT_FLAG_UEVENT;
	sprintf(disk->disk_name, "loop%d", i);
	/* Make this loop device reachable from pathname. */
	err = add_disk(disk);
	if (err)
		goto out_cleanup_disk;

	/* Show this loop device. */
	mutex_lock(&loop_ctl_mutex);
	lo->idr_visible = true;
	mutex_unlock(&loop_ctl_mutex);

	return i;

out_cleanup_disk:
	put_disk(disk);
out_cleanup_tags:
	blk_mq_free_tag_set(&lo->tag_set);
out_free_idr:
	mutex_lock(&loop_ctl_mutex);
	idr_remove(&loop_index_idr, i);
	mutex_unlock(&loop_ctl_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_free_dev:
	kfree(lo);
out:
	return err;
}

static void loop_remove(struct loop_device *lo)
{
<<<<<<< HEAD
	del_gendisk(lo->lo_disk);
	blk_cleanup_queue(lo->lo_queue);
	put_disk(lo->lo_disk);
	kfree(lo);
}

static int find_free_cb(int id, void *ptr, void *data)
{
	struct loop_device *lo = ptr;
	struct loop_device **l = data;

	if (lo->lo_state == Lo_unbound) {
		*l = lo;
		return 1;
	}
	return 0;
}

static int loop_lookup(struct loop_device **l, int i)
{
	struct loop_device *lo;
	int ret = -ENODEV;

	if (i < 0) {
		int err;

		err = idr_for_each(&loop_index_idr, &find_free_cb, &lo);
		if (err == 1) {
			*l = lo;
			ret = lo->lo_number;
		}
		goto out;
	}

	/* lookup and return a specific i */
	lo = idr_find(&loop_index_idr, i);
	if (lo) {
		*l = lo;
		ret = lo->lo_number;
	}
out:
	return ret;
}

static struct kobject *loop_probe(dev_t dev, int *part, void *data)
{
	struct loop_device *lo;
	struct kobject *kobj;
	int err;

	mutex_lock(&loop_index_mutex);
	err = loop_lookup(&lo, MINOR(dev) >> part_shift);
	if (err < 0)
		err = loop_add(&lo, MINOR(dev) >> part_shift);
	if (err < 0)
		kobj = NULL;
	else
		kobj = get_disk(lo->lo_disk);
	mutex_unlock(&loop_index_mutex);

	*part = 0;
	return kobj;
=======
	/* Make this loop device unreachable from pathname. */
	del_gendisk(lo->lo_disk);
	blk_mq_free_tag_set(&lo->tag_set);

	mutex_lock(&loop_ctl_mutex);
	idr_remove(&loop_index_idr, lo->lo_number);
	mutex_unlock(&loop_ctl_mutex);

	put_disk(lo->lo_disk);
}

#ifdef CONFIG_BLOCK_LEGACY_AUTOLOAD
static void loop_probe(dev_t dev)
{
	int idx = MINOR(dev) >> part_shift;

	if (max_loop_specified && max_loop && idx >= max_loop)
		return;
	loop_add(idx);
}
#else
#define loop_probe NULL
#endif /* !CONFIG_BLOCK_LEGACY_AUTOLOAD */

static int loop_control_remove(int idx)
{
	struct loop_device *lo;
	int ret;

	if (idx < 0) {
		pr_warn_once("deleting an unspecified loop device is not supported.\n");
		return -EINVAL;
	}
		
	/* Hide this loop device for serialization. */
	ret = mutex_lock_killable(&loop_ctl_mutex);
	if (ret)
		return ret;
	lo = idr_find(&loop_index_idr, idx);
	if (!lo || !lo->idr_visible)
		ret = -ENODEV;
	else
		lo->idr_visible = false;
	mutex_unlock(&loop_ctl_mutex);
	if (ret)
		return ret;

	/* Check whether this loop device can be removed. */
	ret = mutex_lock_killable(&lo->lo_mutex);
	if (ret)
		goto mark_visible;
	if (lo->lo_state != Lo_unbound || disk_openers(lo->lo_disk) > 0) {
		mutex_unlock(&lo->lo_mutex);
		ret = -EBUSY;
		goto mark_visible;
	}
	/* Mark this loop device as no more bound, but not quite unbound yet */
	lo->lo_state = Lo_deleting;
	mutex_unlock(&lo->lo_mutex);

	loop_remove(lo);
	return 0;

mark_visible:
	/* Show this loop device again. */
	mutex_lock(&loop_ctl_mutex);
	lo->idr_visible = true;
	mutex_unlock(&loop_ctl_mutex);
	return ret;
}

static int loop_control_get_free(int idx)
{
	struct loop_device *lo;
	int id, ret;

	ret = mutex_lock_killable(&loop_ctl_mutex);
	if (ret)
		return ret;
	idr_for_each_entry(&loop_index_idr, lo, id) {
		/* Hitting a race results in creating a new loop device which is harmless. */
		if (lo->idr_visible && data_race(lo->lo_state) == Lo_unbound)
			goto found;
	}
	mutex_unlock(&loop_ctl_mutex);
	return loop_add(-1);
found:
	mutex_unlock(&loop_ctl_mutex);
	return id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static long loop_control_ioctl(struct file *file, unsigned int cmd,
			       unsigned long parm)
{
<<<<<<< HEAD
	struct loop_device *lo;
	int ret = -ENOSYS;

	mutex_lock(&loop_index_mutex);
	switch (cmd) {
	case LOOP_CTL_ADD:
		ret = loop_lookup(&lo, parm);
		if (ret >= 0) {
			ret = -EEXIST;
			break;
		}
		ret = loop_add(&lo, parm);
		break;
	case LOOP_CTL_REMOVE:
		ret = loop_lookup(&lo, parm);
		if (ret < 0)
			break;
		mutex_lock(&lo->lo_ctl_mutex);
		if (lo->lo_state != Lo_unbound) {
			ret = -EBUSY;
			mutex_unlock(&lo->lo_ctl_mutex);
			break;
		}
		if (lo->lo_refcnt > 0) {
			ret = -EBUSY;
			mutex_unlock(&lo->lo_ctl_mutex);
			break;
		}
		lo->lo_disk->private_data = NULL;
		mutex_unlock(&lo->lo_ctl_mutex);
		idr_remove(&loop_index_idr, lo->lo_number);
		loop_remove(lo);
		break;
	case LOOP_CTL_GET_FREE:
		ret = loop_lookup(&lo, -1);
		if (ret >= 0)
			break;
		ret = loop_add(&lo, -1);
	}
	mutex_unlock(&loop_index_mutex);

	return ret;
=======
	switch (cmd) {
	case LOOP_CTL_ADD:
		return loop_add(parm);
	case LOOP_CTL_REMOVE:
		return loop_control_remove(parm);
	case LOOP_CTL_GET_FREE:
		return loop_control_get_free(parm);
	default:
		return -ENOSYS;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations loop_ctl_fops = {
	.open		= nonseekable_open,
	.unlocked_ioctl	= loop_control_ioctl,
	.compat_ioctl	= loop_control_ioctl,
	.owner		= THIS_MODULE,
	.llseek		= noop_llseek,
};

static struct miscdevice loop_misc = {
	.minor		= LOOP_CTRL_MINOR,
	.name		= "loop-control",
	.fops		= &loop_ctl_fops,
};

MODULE_ALIAS_MISCDEV(LOOP_CTRL_MINOR);
MODULE_ALIAS("devname:loop-control");

static int __init loop_init(void)
{
<<<<<<< HEAD
	int i, nr;
	unsigned long range;
	struct loop_device *lo;
	int err;

	err = misc_register(&loop_misc);
	if (err < 0)
		return err;

=======
	int i;
	int err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	part_shift = 0;
	if (max_part > 0) {
		part_shift = fls(max_part);

		/*
		 * Adjust max_part according to part_shift as it is exported
		 * to user space so that user can decide correct minor number
		 * if [s]he want to create more devices.
		 *
		 * Note that -1 is required because partition 0 is reserved
		 * for the whole disk.
		 */
		max_part = (1UL << part_shift) - 1;
	}

	if ((1UL << part_shift) > DISK_MAX_PARTS) {
		err = -EINVAL;
<<<<<<< HEAD
		goto misc_out;
=======
		goto err_out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (max_loop > 1UL << (MINORBITS - part_shift)) {
		err = -EINVAL;
<<<<<<< HEAD
		goto misc_out;
	}

	/*
	 * If max_loop is specified, create that many devices upfront.
	 * This also becomes a hard limit. If max_loop is not specified,
	 * create CONFIG_BLK_DEV_LOOP_MIN_COUNT loop devices at module
	 * init time. Loop devices can be requested on-demand with the
	 * /dev/loop-control interface, or be instantiated by accessing
	 * a 'dead' device node.
	 */
	if (max_loop) {
		nr = max_loop;
		range = max_loop << part_shift;
	} else {
		nr = CONFIG_BLK_DEV_LOOP_MIN_COUNT;
		range = 1UL << MINORBITS;
	}

	if (register_blkdev(LOOP_MAJOR, "loop")) {
=======
		goto err_out;
	}

	err = misc_register(&loop_misc);
	if (err < 0)
		goto err_out;


	if (__register_blkdev(LOOP_MAJOR, "loop", loop_probe)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EIO;
		goto misc_out;
	}

<<<<<<< HEAD
	blk_register_region(MKDEV(LOOP_MAJOR, 0), range,
				  THIS_MODULE, loop_probe, NULL, NULL);

	/* pre-create number of devices given by config or max_loop */
	mutex_lock(&loop_index_mutex);
	for (i = 0; i < nr; i++)
		loop_add(&lo, i);
	mutex_unlock(&loop_index_mutex);
=======
	/* pre-create number of devices given by config or max_loop */
	for (i = 0; i < max_loop; i++)
		loop_add(i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	printk(KERN_INFO "loop: module loaded\n");
	return 0;

misc_out:
	misc_deregister(&loop_misc);
<<<<<<< HEAD
	return err;
}

static int loop_exit_cb(int id, void *ptr, void *data)
{
	struct loop_device *lo = ptr;

	loop_remove(lo);
	return 0;
}

static void __exit loop_exit(void)
{
	unsigned long range;

	range = max_loop ? max_loop << part_shift : 1UL << MINORBITS;

	idr_for_each(&loop_index_idr, &loop_exit_cb, NULL);
	idr_remove_all(&loop_index_idr);
	idr_destroy(&loop_index_idr);

	blk_unregister_region(MKDEV(LOOP_MAJOR, 0), range);
	unregister_blkdev(LOOP_MAJOR, "loop");

	misc_deregister(&loop_misc);
=======
err_out:
	return err;
}

static void __exit loop_exit(void)
{
	struct loop_device *lo;
	int id;

	unregister_blkdev(LOOP_MAJOR, "loop");
	misc_deregister(&loop_misc);

	/*
	 * There is no need to use loop_ctl_mutex here, for nobody else can
	 * access loop_index_idr when this module is unloading (unless forced
	 * module unloading is requested). If this is not a clean unloading,
	 * we have no means to avoid kernel crash.
	 */
	idr_for_each_entry(&loop_index_idr, lo, id)
		loop_remove(lo);

	idr_destroy(&loop_index_idr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(loop_init);
module_exit(loop_exit);

#ifndef MODULE
static int __init max_loop_setup(char *str)
{
	max_loop = simple_strtol(str, NULL, 0);
<<<<<<< HEAD
=======
#ifdef CONFIG_BLOCK_LEGACY_AUTOLOAD
	max_loop_specified = true;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}

__setup("max_loop=", max_loop_setup);
#endif
