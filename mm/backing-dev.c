<<<<<<< HEAD

#include <linux/wait.h>
#include <linux/backing-dev.h>
#include <linux/kthread.h>
=======
// SPDX-License-Identifier: GPL-2.0-only

#include <linux/blkdev.h>
#include <linux/wait.h>
#include <linux/rbtree.h>
#include <linux/kthread.h>
#include <linux/backing-dev.h>
#include <linux/blk-cgroup.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/freezer.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/mm.h>
<<<<<<< HEAD
=======
#include <linux/sched/mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/writeback.h>
#include <linux/device.h>
#include <trace/events/writeback.h>
<<<<<<< HEAD

static atomic_long_t bdi_seq = ATOMIC_LONG_INIT(0);

struct backing_dev_info default_backing_dev_info = {
	.name		= "default",
	.ra_pages	= VM_MAX_READAHEAD * 1024 / PAGE_CACHE_SIZE,
	.state		= 0,
	.capabilities	= BDI_CAP_MAP_COPY,
};
EXPORT_SYMBOL_GPL(default_backing_dev_info);

struct backing_dev_info noop_backing_dev_info = {
	.name		= "noop",
	.capabilities	= BDI_CAP_NO_ACCT_AND_WRITEBACK,
};
EXPORT_SYMBOL_GPL(noop_backing_dev_info);

static struct class *bdi_class;

/*
 * bdi_lock protects updates to bdi_list and bdi_pending_list, as well as
 * reader side protection for bdi_pending_list. bdi_list has RCU reader side
 * locking.
 */
DEFINE_SPINLOCK(bdi_lock);
LIST_HEAD(bdi_list);
LIST_HEAD(bdi_pending_list);

static struct task_struct *sync_supers_tsk;
static struct timer_list sync_supers_timer;

static int bdi_sync_supers(void *);
static void sync_supers_timer_fn(unsigned long);

void bdi_lock_two(struct bdi_writeback *wb1, struct bdi_writeback *wb2)
{
	if (wb1 < wb2) {
		spin_lock(&wb1->list_lock);
		spin_lock_nested(&wb2->list_lock, 1);
	} else {
		spin_lock(&wb2->list_lock);
		spin_lock_nested(&wb1->list_lock, 1);
	}
}
=======
#include "internal.h"

struct backing_dev_info noop_backing_dev_info;
EXPORT_SYMBOL_GPL(noop_backing_dev_info);

static const char *bdi_unknown_name = "(unknown)";

/*
 * bdi_lock protects bdi_tree and updates to bdi_list. bdi_list has RCU
 * reader side locking.
 */
DEFINE_SPINLOCK(bdi_lock);
static u64 bdi_id_cursor;
static struct rb_root bdi_tree = RB_ROOT;
LIST_HEAD(bdi_list);

/* bdi_wq serves all asynchronous writeback tasks */
struct workqueue_struct *bdi_wq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#include <linux/seq_file.h>

static struct dentry *bdi_debug_root;

static void bdi_debug_init(void)
{
	bdi_debug_root = debugfs_create_dir("bdi", NULL);
}

static int bdi_debug_stats_show(struct seq_file *m, void *v)
{
	struct backing_dev_info *bdi = m->private;
	struct bdi_writeback *wb = &bdi->wb;
	unsigned long background_thresh;
	unsigned long dirty_thresh;
<<<<<<< HEAD
	unsigned long bdi_thresh;
	unsigned long nr_dirty, nr_io, nr_more_io;
	struct inode *inode;

	nr_dirty = nr_io = nr_more_io = 0;
	spin_lock(&wb->list_lock);
	list_for_each_entry(inode, &wb->b_dirty, i_wb_list)
		nr_dirty++;
	list_for_each_entry(inode, &wb->b_io, i_wb_list)
		nr_io++;
	list_for_each_entry(inode, &wb->b_more_io, i_wb_list)
		nr_more_io++;
	spin_unlock(&wb->list_lock);

	global_dirty_limits(&background_thresh, &dirty_thresh);
	bdi_thresh = bdi_dirty_limit(bdi, dirty_thresh);

#define K(x) ((x) << (PAGE_SHIFT - 10))
=======
	unsigned long wb_thresh;
	unsigned long nr_dirty, nr_io, nr_more_io, nr_dirty_time;
	struct inode *inode;

	nr_dirty = nr_io = nr_more_io = nr_dirty_time = 0;
	spin_lock(&wb->list_lock);
	list_for_each_entry(inode, &wb->b_dirty, i_io_list)
		nr_dirty++;
	list_for_each_entry(inode, &wb->b_io, i_io_list)
		nr_io++;
	list_for_each_entry(inode, &wb->b_more_io, i_io_list)
		nr_more_io++;
	list_for_each_entry(inode, &wb->b_dirty_time, i_io_list)
		if (inode->i_state & I_DIRTY_TIME)
			nr_dirty_time++;
	spin_unlock(&wb->list_lock);

	global_dirty_limits(&background_thresh, &dirty_thresh);
	wb_thresh = wb_calc_thresh(wb, dirty_thresh);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	seq_printf(m,
		   "BdiWriteback:       %10lu kB\n"
		   "BdiReclaimable:     %10lu kB\n"
		   "BdiDirtyThresh:     %10lu kB\n"
		   "DirtyThresh:        %10lu kB\n"
		   "BackgroundThresh:   %10lu kB\n"
		   "BdiDirtied:         %10lu kB\n"
		   "BdiWritten:         %10lu kB\n"
		   "BdiWriteBandwidth:  %10lu kBps\n"
		   "b_dirty:            %10lu\n"
		   "b_io:               %10lu\n"
		   "b_more_io:          %10lu\n"
<<<<<<< HEAD
		   "bdi_list:           %10u\n"
		   "state:              %10lx\n",
		   (unsigned long) K(bdi_stat(bdi, BDI_WRITEBACK)),
		   (unsigned long) K(bdi_stat(bdi, BDI_RECLAIMABLE)),
		   K(bdi_thresh),
		   K(dirty_thresh),
		   K(background_thresh),
		   (unsigned long) K(bdi_stat(bdi, BDI_DIRTIED)),
		   (unsigned long) K(bdi_stat(bdi, BDI_WRITTEN)),
		   (unsigned long) K(bdi->write_bandwidth),
		   nr_dirty,
		   nr_io,
		   nr_more_io,
		   !list_empty(&bdi->bdi_list), bdi->state);
#undef K

	return 0;
}

static int bdi_debug_stats_open(struct inode *inode, struct file *file)
{
	return single_open(file, bdi_debug_stats_show, inode->i_private);
}

static const struct file_operations bdi_debug_stats_fops = {
	.open		= bdi_debug_stats_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
=======
		   "b_dirty_time:       %10lu\n"
		   "bdi_list:           %10u\n"
		   "state:              %10lx\n",
		   (unsigned long) K(wb_stat(wb, WB_WRITEBACK)),
		   (unsigned long) K(wb_stat(wb, WB_RECLAIMABLE)),
		   K(wb_thresh),
		   K(dirty_thresh),
		   K(background_thresh),
		   (unsigned long) K(wb_stat(wb, WB_DIRTIED)),
		   (unsigned long) K(wb_stat(wb, WB_WRITTEN)),
		   (unsigned long) K(wb->write_bandwidth),
		   nr_dirty,
		   nr_io,
		   nr_more_io,
		   nr_dirty_time,
		   !list_empty(&bdi->bdi_list), bdi->wb.state);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(bdi_debug_stats);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void bdi_debug_register(struct backing_dev_info *bdi, const char *name)
{
	bdi->debug_dir = debugfs_create_dir(name, bdi_debug_root);
<<<<<<< HEAD
	bdi->debug_stats = debugfs_create_file("stats", 0444, bdi->debug_dir,
					       bdi, &bdi_debug_stats_fops);
=======

	debugfs_create_file("stats", 0444, bdi->debug_dir, bdi,
			    &bdi_debug_stats_fops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void bdi_debug_unregister(struct backing_dev_info *bdi)
{
<<<<<<< HEAD
	debugfs_remove(bdi->debug_stats);
	debugfs_remove(bdi->debug_dir);
=======
	debugfs_remove_recursive(bdi->debug_dir);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#else
static inline void bdi_debug_init(void)
{
}
static inline void bdi_debug_register(struct backing_dev_info *bdi,
				      const char *name)
{
}
static inline void bdi_debug_unregister(struct backing_dev_info *bdi)
{
}
#endif

static ssize_t read_ahead_kb_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t count)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *end;
	unsigned long read_ahead_kb;
	ssize_t ret = -EINVAL;

	read_ahead_kb = simple_strtoul(buf, &end, 10);
	if (*buf && (end[0] == '\0' || (end[0] == '\n' && end[1] == '\0'))) {
		bdi->ra_pages = read_ahead_kb >> (PAGE_SHIFT - 10);
		ret = count;
	}
	return ret;
}

#define K(pages) ((pages) << (PAGE_SHIFT - 10))

#define BDI_SHOW(name, expr)						\
static ssize_t name##_show(struct device *dev,				\
			   struct device_attribute *attr, char *page)	\
{									\
	struct backing_dev_info *bdi = dev_get_drvdata(dev);		\
									\
	return snprintf(page, PAGE_SIZE-1, "%lld\n", (long long)expr);	\
}
=======
	unsigned long read_ahead_kb;
	ssize_t ret;

	ret = kstrtoul(buf, 10, &read_ahead_kb);
	if (ret < 0)
		return ret;

	bdi->ra_pages = read_ahead_kb >> (PAGE_SHIFT - 10);

	return count;
}

#define BDI_SHOW(name, expr)						\
static ssize_t name##_show(struct device *dev,				\
			   struct device_attribute *attr, char *buf)	\
{									\
	struct backing_dev_info *bdi = dev_get_drvdata(dev);		\
									\
	return sysfs_emit(buf, "%lld\n", (long long)expr);		\
}									\
static DEVICE_ATTR_RW(name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

BDI_SHOW(read_ahead_kb, K(bdi->ra_pages))

static ssize_t min_ratio_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *end;
	unsigned int ratio;
	ssize_t ret = -EINVAL;

	ratio = simple_strtoul(buf, &end, 10);
	if (*buf && (end[0] == '\0' || (end[0] == '\n' && end[1] == '\0'))) {
		ret = bdi_set_min_ratio(bdi, ratio);
		if (!ret)
			ret = count;
	}
	return ret;
}
BDI_SHOW(min_ratio, bdi->min_ratio)
=======
	unsigned int ratio;
	ssize_t ret;

	ret = kstrtouint(buf, 10, &ratio);
	if (ret < 0)
		return ret;

	ret = bdi_set_min_ratio(bdi, ratio);
	if (!ret)
		ret = count;

	return ret;
}
BDI_SHOW(min_ratio, bdi->min_ratio / BDI_RATIO_SCALE)

static ssize_t min_ratio_fine_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);
	unsigned int ratio;
	ssize_t ret;

	ret = kstrtouint(buf, 10, &ratio);
	if (ret < 0)
		return ret;

	ret = bdi_set_min_ratio_no_scale(bdi, ratio);
	if (!ret)
		ret = count;

	return ret;
}
BDI_SHOW(min_ratio_fine, bdi->min_ratio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t max_ratio_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *end;
	unsigned int ratio;
	ssize_t ret = -EINVAL;

	ratio = simple_strtoul(buf, &end, 10);
	if (*buf && (end[0] == '\0' || (end[0] == '\n' && end[1] == '\0'))) {
		ret = bdi_set_max_ratio(bdi, ratio);
		if (!ret)
			ret = count;
	}
	return ret;
}
BDI_SHOW(max_ratio, bdi->max_ratio)

#define __ATTR_RW(attr) __ATTR(attr, 0644, attr##_show, attr##_store)

static struct device_attribute bdi_dev_attrs[] = {
	__ATTR_RW(read_ahead_kb),
	__ATTR_RW(min_ratio),
	__ATTR_RW(max_ratio),
	__ATTR_NULL,
=======
	unsigned int ratio;
	ssize_t ret;

	ret = kstrtouint(buf, 10, &ratio);
	if (ret < 0)
		return ret;

	ret = bdi_set_max_ratio(bdi, ratio);
	if (!ret)
		ret = count;

	return ret;
}
BDI_SHOW(max_ratio, bdi->max_ratio / BDI_RATIO_SCALE)

static ssize_t max_ratio_fine_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);
	unsigned int ratio;
	ssize_t ret;

	ret = kstrtouint(buf, 10, &ratio);
	if (ret < 0)
		return ret;

	ret = bdi_set_max_ratio_no_scale(bdi, ratio);
	if (!ret)
		ret = count;

	return ret;
}
BDI_SHOW(max_ratio_fine, bdi->max_ratio)

static ssize_t min_bytes_show(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%llu\n", bdi_get_min_bytes(bdi));
}

static ssize_t min_bytes_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);
	u64 bytes;
	ssize_t ret;

	ret = kstrtoull(buf, 10, &bytes);
	if (ret < 0)
		return ret;

	ret = bdi_set_min_bytes(bdi, bytes);
	if (!ret)
		ret = count;

	return ret;
}
static DEVICE_ATTR_RW(min_bytes);

static ssize_t max_bytes_show(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%llu\n", bdi_get_max_bytes(bdi));
}

static ssize_t max_bytes_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);
	u64 bytes;
	ssize_t ret;

	ret = kstrtoull(buf, 10, &bytes);
	if (ret < 0)
		return ret;

	ret = bdi_set_max_bytes(bdi, bytes);
	if (!ret)
		ret = count;

	return ret;
}
static DEVICE_ATTR_RW(max_bytes);

static ssize_t stable_pages_required_show(struct device *dev,
					  struct device_attribute *attr,
					  char *buf)
{
	dev_warn_once(dev,
		"the stable_pages_required attribute has been removed. Use the stable_writes queue attribute instead.\n");
	return sysfs_emit(buf, "%d\n", 0);
}
static DEVICE_ATTR_RO(stable_pages_required);

static ssize_t strict_limit_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);
	unsigned int strict_limit;
	ssize_t ret;

	ret = kstrtouint(buf, 10, &strict_limit);
	if (ret < 0)
		return ret;

	ret = bdi_set_strict_limit(bdi, strict_limit);
	if (!ret)
		ret = count;

	return ret;
}

static ssize_t strict_limit_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct backing_dev_info *bdi = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%d\n",
			!!(bdi->capabilities & BDI_CAP_STRICTLIMIT));
}
static DEVICE_ATTR_RW(strict_limit);

static struct attribute *bdi_dev_attrs[] = {
	&dev_attr_read_ahead_kb.attr,
	&dev_attr_min_ratio.attr,
	&dev_attr_min_ratio_fine.attr,
	&dev_attr_max_ratio.attr,
	&dev_attr_max_ratio_fine.attr,
	&dev_attr_min_bytes.attr,
	&dev_attr_max_bytes.attr,
	&dev_attr_stable_pages_required.attr,
	&dev_attr_strict_limit.attr,
	NULL,
};
ATTRIBUTE_GROUPS(bdi_dev);

static const struct class bdi_class = {
	.name		= "bdi",
	.dev_groups	= bdi_dev_groups,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static __init int bdi_class_init(void)
{
<<<<<<< HEAD
	bdi_class = class_create(THIS_MODULE, "bdi");
	if (IS_ERR(bdi_class))
		return PTR_ERR(bdi_class);

	bdi_class->dev_attrs = bdi_dev_attrs;
	bdi_debug_init();
=======
	int ret;

	ret = class_register(&bdi_class);
	if (ret)
		return ret;

	bdi_debug_init();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
postcore_initcall(bdi_class_init);

static int __init default_bdi_init(void)
{
<<<<<<< HEAD
	int err;

	sync_supers_tsk = kthread_run(bdi_sync_supers, NULL, "sync_supers");
	BUG_ON(IS_ERR(sync_supers_tsk));

	init_timer_deferrable(&sync_supers_timer);
	sync_supers_timer.function = sync_supers_timer_fn;
	sync_supers_timer.data = 0;
	bdi_arm_supers_timer();

	err = bdi_init(&default_backing_dev_info);
	if (!err)
		bdi_register(&default_backing_dev_info, NULL, "default");
	err = bdi_init(&noop_backing_dev_info);

	return err;
}
subsys_initcall(default_bdi_init);

int bdi_has_dirty_io(struct backing_dev_info *bdi)
{
	return wb_has_dirty_io(&bdi->wb);
}

/*
 * kupdated() used to do this. We cannot do it from the bdi_forker_thread()
 * or we risk deadlocking on ->s_umount. The longer term solution would be
 * to implement sync_supers_bdi() or similar and simply do it from the
 * bdi writeback thread individually.
 */
static int bdi_sync_supers(void *unused)
{
	set_user_nice(current, 0);

	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();

		/*
		 * Do this periodically, like kupdated() did before.
		 */
		sync_supers();
	}

	return 0;
}

void bdi_arm_supers_timer(void)
{
	unsigned long next;

	if (!dirty_writeback_interval)
		return;

	next = msecs_to_jiffies(dirty_writeback_interval * 10) + jiffies;
	mod_timer(&sync_supers_timer, round_jiffies_up(next));
}

static void sync_supers_timer_fn(unsigned long unused)
{
	wake_up_process(sync_supers_tsk);
	bdi_arm_supers_timer();
}

static void wakeup_timer_fn(unsigned long data)
{
	struct backing_dev_info *bdi = (struct backing_dev_info *)data;

	spin_lock_bh(&bdi->wb_lock);
	if (bdi->wb.task) {
		trace_writeback_wake_thread(bdi);
		wake_up_process(bdi->wb.task);
	} else if (bdi->dev) {
		/*
		 * When bdi tasks are inactive for long time, they are killed.
		 * In this case we have to wake-up the forker thread which
		 * should create and run the bdi thread.
		 */
		trace_writeback_wake_forker_thread(bdi);
		wake_up_process(default_backing_dev_info.wb.task);
	}
	spin_unlock_bh(&bdi->wb_lock);
}

/*
 * This function is used when the first inode for this bdi is marked dirty. It
 * wakes-up the corresponding bdi thread which should then take care of the
 * periodic background write-out of dirty inodes. Since the write-out would
 * starts only 'dirty_writeback_interval' centisecs from now anyway, we just
 * set up a timer which wakes the bdi thread up later.
 *
 * Note, we wouldn't bother setting up the timer, but this function is on the
 * fast-path (used by '__mark_inode_dirty()'), so we save few context switches
 * by delaying the wake-up.
 */
void bdi_wakeup_thread_delayed(struct backing_dev_info *bdi)
{
	unsigned long timeout;

	timeout = msecs_to_jiffies(dirty_writeback_interval * 10);
	mod_timer(&bdi->wb.wakeup_timer, jiffies + timeout);
}

/*
 * Calculate the longest interval (jiffies) bdi threads are allowed to be
 * inactive.
 */
static unsigned long bdi_longest_inactive(void)
{
	unsigned long interval;

	interval = msecs_to_jiffies(dirty_writeback_interval * 10);
	return max(5UL * 60 * HZ, interval);
}

/*
 * Clear pending bit and wakeup anybody waiting for flusher thread creation or
 * shutdown
 */
static void bdi_clear_pending(struct backing_dev_info *bdi)
{
	clear_bit(BDI_pending, &bdi->state);
	smp_mb__after_clear_bit();
	wake_up_bit(&bdi->state, BDI_pending);
}

static int bdi_forker_thread(void *ptr)
{
	struct bdi_writeback *me = ptr;

	current->flags |= PF_SWAPWRITE;
	set_freezable();

	/*
	 * Our parent may run at a different priority, just set us to normal
	 */
	set_user_nice(current, 0);

	for (;;) {
		struct task_struct *task = NULL;
		struct backing_dev_info *bdi;
		enum {
			NO_ACTION,   /* Nothing to do */
			FORK_THREAD, /* Fork bdi thread */
			KILL_THREAD, /* Kill inactive bdi thread */
		} action = NO_ACTION;

		/*
		 * Temporary measure, we want to make sure we don't see
		 * dirty data on the default backing_dev_info
		 */
		if (wb_has_dirty_io(me) || !list_empty(&me->bdi->work_list)) {
			del_timer(&me->wakeup_timer);
			wb_do_writeback(me, 0);
		}

		spin_lock_bh(&bdi_lock);
		/*
		 * In the following loop we are going to check whether we have
		 * some work to do without any synchronization with tasks
		 * waking us up to do work for them. Set the task state here
		 * so that we don't miss wakeups after verifying conditions.
		 */
		set_current_state(TASK_INTERRUPTIBLE);

		list_for_each_entry(bdi, &bdi_list, bdi_list) {
			bool have_dirty_io;

			if (!bdi_cap_writeback_dirty(bdi) ||
			     bdi_cap_flush_forker(bdi))
				continue;

			WARN(!test_bit(BDI_registered, &bdi->state),
			     "bdi %p/%s is not registered!\n", bdi, bdi->name);

			have_dirty_io = !list_empty(&bdi->work_list) ||
					wb_has_dirty_io(&bdi->wb);

			/*
			 * If the bdi has work to do, but the thread does not
			 * exist - create it.
			 */
			if (!bdi->wb.task && have_dirty_io) {
				/*
				 * Set the pending bit - if someone will try to
				 * unregister this bdi - it'll wait on this bit.
				 */
				set_bit(BDI_pending, &bdi->state);
				action = FORK_THREAD;
				break;
			}

			spin_lock(&bdi->wb_lock);

			/*
			 * If there is no work to do and the bdi thread was
			 * inactive long enough - kill it. The wb_lock is taken
			 * to make sure no-one adds more work to this bdi and
			 * wakes the bdi thread up.
			 */
			if (bdi->wb.task && !have_dirty_io &&
			    time_after(jiffies, bdi->wb.last_active +
						bdi_longest_inactive())) {
				task = bdi->wb.task;
				bdi->wb.task = NULL;
				spin_unlock(&bdi->wb_lock);
				set_bit(BDI_pending, &bdi->state);
				action = KILL_THREAD;
				break;
			}
			spin_unlock(&bdi->wb_lock);
		}
		spin_unlock_bh(&bdi_lock);

		/* Keep working if default bdi still has things to do */
		if (!list_empty(&me->bdi->work_list))
			__set_current_state(TASK_RUNNING);

		switch (action) {
		case FORK_THREAD:
			__set_current_state(TASK_RUNNING);
			task = kthread_create(bdi_writeback_thread, &bdi->wb,
					      "flush-%s", dev_name(bdi->dev));
			if (IS_ERR(task)) {
				/*
				 * If thread creation fails, force writeout of
				 * the bdi from the thread. Hopefully 1024 is
				 * large enough for efficient IO.
				 */
				writeback_inodes_wb(&bdi->wb, 1024,
						    WB_REASON_FORKER_THREAD);
			} else {
				/*
				 * The spinlock makes sure we do not lose
				 * wake-ups when racing with 'bdi_queue_work()'.
				 * And as soon as the bdi thread is visible, we
				 * can start it.
				 */
				spin_lock_bh(&bdi->wb_lock);
				bdi->wb.task = task;
				spin_unlock_bh(&bdi->wb_lock);
				wake_up_process(task);
			}
			bdi_clear_pending(bdi);
			break;

		case KILL_THREAD:
			__set_current_state(TASK_RUNNING);
			kthread_stop(task);
			bdi_clear_pending(bdi);
			break;

		case NO_ACTION:
			if (!wb_has_dirty_io(me) || !dirty_writeback_interval)
				/*
				 * There are no dirty data. The only thing we
				 * should now care about is checking for
				 * inactive bdi threads and killing them. Thus,
				 * let's sleep for longer time, save energy and
				 * be friendly for battery-driven devices.
				 */
				schedule_timeout(bdi_longest_inactive());
			else
				schedule_timeout(msecs_to_jiffies(dirty_writeback_interval * 10));
			try_to_freeze();
			break;
		}
	}

	return 0;
}
=======
	bdi_wq = alloc_workqueue("writeback", WQ_MEM_RECLAIM | WQ_UNBOUND |
				 WQ_SYSFS, 0);
	if (!bdi_wq)
		return -ENOMEM;
	return 0;
}
subsys_initcall(default_bdi_init);

static void wb_update_bandwidth_workfn(struct work_struct *work)
{
	struct bdi_writeback *wb = container_of(to_delayed_work(work),
						struct bdi_writeback, bw_dwork);

	wb_update_bandwidth(wb);
}

/*
 * Initial write bandwidth: 100 MB/s
 */
#define INIT_BW		(100 << (20 - PAGE_SHIFT))

static int wb_init(struct bdi_writeback *wb, struct backing_dev_info *bdi,
		   gfp_t gfp)
{
	int i, err;

	memset(wb, 0, sizeof(*wb));

	wb->bdi = bdi;
	wb->last_old_flush = jiffies;
	INIT_LIST_HEAD(&wb->b_dirty);
	INIT_LIST_HEAD(&wb->b_io);
	INIT_LIST_HEAD(&wb->b_more_io);
	INIT_LIST_HEAD(&wb->b_dirty_time);
	spin_lock_init(&wb->list_lock);

	atomic_set(&wb->writeback_inodes, 0);
	wb->bw_time_stamp = jiffies;
	wb->balanced_dirty_ratelimit = INIT_BW;
	wb->dirty_ratelimit = INIT_BW;
	wb->write_bandwidth = INIT_BW;
	wb->avg_write_bandwidth = INIT_BW;

	spin_lock_init(&wb->work_lock);
	INIT_LIST_HEAD(&wb->work_list);
	INIT_DELAYED_WORK(&wb->dwork, wb_workfn);
	INIT_DELAYED_WORK(&wb->bw_dwork, wb_update_bandwidth_workfn);

	err = fprop_local_init_percpu(&wb->completions, gfp);
	if (err)
		return err;

	for (i = 0; i < NR_WB_STAT_ITEMS; i++) {
		err = percpu_counter_init(&wb->stat[i], 0, gfp);
		if (err)
			goto out_destroy_stat;
	}

	return 0;

out_destroy_stat:
	while (i--)
		percpu_counter_destroy(&wb->stat[i]);
	fprop_local_destroy_percpu(&wb->completions);
	return err;
}

static void cgwb_remove_from_bdi_list(struct bdi_writeback *wb);

/*
 * Remove bdi from the global list and shutdown any threads we have running
 */
static void wb_shutdown(struct bdi_writeback *wb)
{
	/* Make sure nobody queues further work */
	spin_lock_irq(&wb->work_lock);
	if (!test_and_clear_bit(WB_registered, &wb->state)) {
		spin_unlock_irq(&wb->work_lock);
		return;
	}
	spin_unlock_irq(&wb->work_lock);

	cgwb_remove_from_bdi_list(wb);
	/*
	 * Drain work list and shutdown the delayed_work.  !WB_registered
	 * tells wb_workfn() that @wb is dying and its work_list needs to
	 * be drained no matter what.
	 */
	mod_delayed_work(bdi_wq, &wb->dwork, 0);
	flush_delayed_work(&wb->dwork);
	WARN_ON(!list_empty(&wb->work_list));
	flush_delayed_work(&wb->bw_dwork);
}

static void wb_exit(struct bdi_writeback *wb)
{
	int i;

	WARN_ON(delayed_work_pending(&wb->dwork));

	for (i = 0; i < NR_WB_STAT_ITEMS; i++)
		percpu_counter_destroy(&wb->stat[i]);

	fprop_local_destroy_percpu(&wb->completions);
}

#ifdef CONFIG_CGROUP_WRITEBACK

#include <linux/memcontrol.h>

/*
 * cgwb_lock protects bdi->cgwb_tree, blkcg->cgwb_list, offline_cgwbs and
 * memcg->cgwb_list.  bdi->cgwb_tree is also RCU protected.
 */
static DEFINE_SPINLOCK(cgwb_lock);
static struct workqueue_struct *cgwb_release_wq;

static LIST_HEAD(offline_cgwbs);
static void cleanup_offline_cgwbs_workfn(struct work_struct *work);
static DECLARE_WORK(cleanup_offline_cgwbs_work, cleanup_offline_cgwbs_workfn);

static void cgwb_free_rcu(struct rcu_head *rcu_head)
{
	struct bdi_writeback *wb = container_of(rcu_head,
			struct bdi_writeback, rcu);

	percpu_ref_exit(&wb->refcnt);
	kfree(wb);
}

static void cgwb_release_workfn(struct work_struct *work)
{
	struct bdi_writeback *wb = container_of(work, struct bdi_writeback,
						release_work);
	struct backing_dev_info *bdi = wb->bdi;

	mutex_lock(&wb->bdi->cgwb_release_mutex);
	wb_shutdown(wb);

	css_put(wb->memcg_css);
	css_put(wb->blkcg_css);
	mutex_unlock(&wb->bdi->cgwb_release_mutex);

	/* triggers blkg destruction if no online users left */
	blkcg_unpin_online(wb->blkcg_css);

	fprop_local_destroy_percpu(&wb->memcg_completions);

	spin_lock_irq(&cgwb_lock);
	list_del(&wb->offline_node);
	spin_unlock_irq(&cgwb_lock);

	wb_exit(wb);
	bdi_put(bdi);
	WARN_ON_ONCE(!list_empty(&wb->b_attached));
	call_rcu(&wb->rcu, cgwb_free_rcu);
}

static void cgwb_release(struct percpu_ref *refcnt)
{
	struct bdi_writeback *wb = container_of(refcnt, struct bdi_writeback,
						refcnt);
	queue_work(cgwb_release_wq, &wb->release_work);
}

static void cgwb_kill(struct bdi_writeback *wb)
{
	lockdep_assert_held(&cgwb_lock);

	WARN_ON(!radix_tree_delete(&wb->bdi->cgwb_tree, wb->memcg_css->id));
	list_del(&wb->memcg_node);
	list_del(&wb->blkcg_node);
	list_add(&wb->offline_node, &offline_cgwbs);
	percpu_ref_kill(&wb->refcnt);
}

static void cgwb_remove_from_bdi_list(struct bdi_writeback *wb)
{
	spin_lock_irq(&cgwb_lock);
	list_del_rcu(&wb->bdi_node);
	spin_unlock_irq(&cgwb_lock);
}

static int cgwb_create(struct backing_dev_info *bdi,
		       struct cgroup_subsys_state *memcg_css, gfp_t gfp)
{
	struct mem_cgroup *memcg;
	struct cgroup_subsys_state *blkcg_css;
	struct list_head *memcg_cgwb_list, *blkcg_cgwb_list;
	struct bdi_writeback *wb;
	unsigned long flags;
	int ret = 0;

	memcg = mem_cgroup_from_css(memcg_css);
	blkcg_css = cgroup_get_e_css(memcg_css->cgroup, &io_cgrp_subsys);
	memcg_cgwb_list = &memcg->cgwb_list;
	blkcg_cgwb_list = blkcg_get_cgwb_list(blkcg_css);

	/* look up again under lock and discard on blkcg mismatch */
	spin_lock_irqsave(&cgwb_lock, flags);
	wb = radix_tree_lookup(&bdi->cgwb_tree, memcg_css->id);
	if (wb && wb->blkcg_css != blkcg_css) {
		cgwb_kill(wb);
		wb = NULL;
	}
	spin_unlock_irqrestore(&cgwb_lock, flags);
	if (wb)
		goto out_put;

	/* need to create a new one */
	wb = kmalloc(sizeof(*wb), gfp);
	if (!wb) {
		ret = -ENOMEM;
		goto out_put;
	}

	ret = wb_init(wb, bdi, gfp);
	if (ret)
		goto err_free;

	ret = percpu_ref_init(&wb->refcnt, cgwb_release, 0, gfp);
	if (ret)
		goto err_wb_exit;

	ret = fprop_local_init_percpu(&wb->memcg_completions, gfp);
	if (ret)
		goto err_ref_exit;

	wb->memcg_css = memcg_css;
	wb->blkcg_css = blkcg_css;
	INIT_LIST_HEAD(&wb->b_attached);
	INIT_WORK(&wb->release_work, cgwb_release_workfn);
	set_bit(WB_registered, &wb->state);
	bdi_get(bdi);

	/*
	 * The root wb determines the registered state of the whole bdi and
	 * memcg_cgwb_list and blkcg_cgwb_list's next pointers indicate
	 * whether they're still online.  Don't link @wb if any is dead.
	 * See wb_memcg_offline() and wb_blkcg_offline().
	 */
	ret = -ENODEV;
	spin_lock_irqsave(&cgwb_lock, flags);
	if (test_bit(WB_registered, &bdi->wb.state) &&
	    blkcg_cgwb_list->next && memcg_cgwb_list->next) {
		/* we might have raced another instance of this function */
		ret = radix_tree_insert(&bdi->cgwb_tree, memcg_css->id, wb);
		if (!ret) {
			list_add_tail_rcu(&wb->bdi_node, &bdi->wb_list);
			list_add(&wb->memcg_node, memcg_cgwb_list);
			list_add(&wb->blkcg_node, blkcg_cgwb_list);
			blkcg_pin_online(blkcg_css);
			css_get(memcg_css);
			css_get(blkcg_css);
		}
	}
	spin_unlock_irqrestore(&cgwb_lock, flags);
	if (ret) {
		if (ret == -EEXIST)
			ret = 0;
		goto err_fprop_exit;
	}
	goto out_put;

err_fprop_exit:
	bdi_put(bdi);
	fprop_local_destroy_percpu(&wb->memcg_completions);
err_ref_exit:
	percpu_ref_exit(&wb->refcnt);
err_wb_exit:
	wb_exit(wb);
err_free:
	kfree(wb);
out_put:
	css_put(blkcg_css);
	return ret;
}

/**
 * wb_get_lookup - get wb for a given memcg
 * @bdi: target bdi
 * @memcg_css: cgroup_subsys_state of the target memcg (must have positive ref)
 *
 * Try to get the wb for @memcg_css on @bdi.  The returned wb has its
 * refcount incremented.
 *
 * This function uses css_get() on @memcg_css and thus expects its refcnt
 * to be positive on invocation.  IOW, rcu_read_lock() protection on
 * @memcg_css isn't enough.  try_get it before calling this function.
 *
 * A wb is keyed by its associated memcg.  As blkcg implicitly enables
 * memcg on the default hierarchy, memcg association is guaranteed to be
 * more specific (equal or descendant to the associated blkcg) and thus can
 * identify both the memcg and blkcg associations.
 *
 * Because the blkcg associated with a memcg may change as blkcg is enabled
 * and disabled closer to root in the hierarchy, each wb keeps track of
 * both the memcg and blkcg associated with it and verifies the blkcg on
 * each lookup.  On mismatch, the existing wb is discarded and a new one is
 * created.
 */
struct bdi_writeback *wb_get_lookup(struct backing_dev_info *bdi,
				    struct cgroup_subsys_state *memcg_css)
{
	struct bdi_writeback *wb;

	if (!memcg_css->parent)
		return &bdi->wb;

	rcu_read_lock();
	wb = radix_tree_lookup(&bdi->cgwb_tree, memcg_css->id);
	if (wb) {
		struct cgroup_subsys_state *blkcg_css;

		/* see whether the blkcg association has changed */
		blkcg_css = cgroup_get_e_css(memcg_css->cgroup, &io_cgrp_subsys);
		if (unlikely(wb->blkcg_css != blkcg_css || !wb_tryget(wb)))
			wb = NULL;
		css_put(blkcg_css);
	}
	rcu_read_unlock();

	return wb;
}

/**
 * wb_get_create - get wb for a given memcg, create if necessary
 * @bdi: target bdi
 * @memcg_css: cgroup_subsys_state of the target memcg (must have positive ref)
 * @gfp: allocation mask to use
 *
 * Try to get the wb for @memcg_css on @bdi.  If it doesn't exist, try to
 * create one.  See wb_get_lookup() for more details.
 */
struct bdi_writeback *wb_get_create(struct backing_dev_info *bdi,
				    struct cgroup_subsys_state *memcg_css,
				    gfp_t gfp)
{
	struct bdi_writeback *wb;

	might_alloc(gfp);

	do {
		wb = wb_get_lookup(bdi, memcg_css);
	} while (!wb && !cgwb_create(bdi, memcg_css, gfp));

	return wb;
}

static int cgwb_bdi_init(struct backing_dev_info *bdi)
{
	int ret;

	INIT_RADIX_TREE(&bdi->cgwb_tree, GFP_ATOMIC);
	mutex_init(&bdi->cgwb_release_mutex);
	init_rwsem(&bdi->wb_switch_rwsem);

	ret = wb_init(&bdi->wb, bdi, GFP_KERNEL);
	if (!ret) {
		bdi->wb.memcg_css = &root_mem_cgroup->css;
		bdi->wb.blkcg_css = blkcg_root_css;
	}
	return ret;
}

static void cgwb_bdi_unregister(struct backing_dev_info *bdi)
{
	struct radix_tree_iter iter;
	void **slot;
	struct bdi_writeback *wb;

	WARN_ON(test_bit(WB_registered, &bdi->wb.state));

	spin_lock_irq(&cgwb_lock);
	radix_tree_for_each_slot(slot, &bdi->cgwb_tree, &iter, 0)
		cgwb_kill(*slot);
	spin_unlock_irq(&cgwb_lock);

	mutex_lock(&bdi->cgwb_release_mutex);
	spin_lock_irq(&cgwb_lock);
	while (!list_empty(&bdi->wb_list)) {
		wb = list_first_entry(&bdi->wb_list, struct bdi_writeback,
				      bdi_node);
		spin_unlock_irq(&cgwb_lock);
		wb_shutdown(wb);
		spin_lock_irq(&cgwb_lock);
	}
	spin_unlock_irq(&cgwb_lock);
	mutex_unlock(&bdi->cgwb_release_mutex);
}

/*
 * cleanup_offline_cgwbs_workfn - try to release dying cgwbs
 *
 * Try to release dying cgwbs by switching attached inodes to the nearest
 * living ancestor's writeback. Processed wbs are placed at the end
 * of the list to guarantee the forward progress.
 */
static void cleanup_offline_cgwbs_workfn(struct work_struct *work)
{
	struct bdi_writeback *wb;
	LIST_HEAD(processed);

	spin_lock_irq(&cgwb_lock);

	while (!list_empty(&offline_cgwbs)) {
		wb = list_first_entry(&offline_cgwbs, struct bdi_writeback,
				      offline_node);
		list_move(&wb->offline_node, &processed);

		/*
		 * If wb is dirty, cleaning up the writeback by switching
		 * attached inodes will result in an effective removal of any
		 * bandwidth restrictions, which isn't the goal.  Instead,
		 * it can be postponed until the next time, when all io
		 * will be likely completed.  If in the meantime some inodes
		 * will get re-dirtied, they should be eventually switched to
		 * a new cgwb.
		 */
		if (wb_has_dirty_io(wb))
			continue;

		if (!wb_tryget(wb))
			continue;

		spin_unlock_irq(&cgwb_lock);
		while (cleanup_offline_cgwb(wb))
			cond_resched();
		spin_lock_irq(&cgwb_lock);

		wb_put(wb);
	}

	if (!list_empty(&processed))
		list_splice_tail(&processed, &offline_cgwbs);

	spin_unlock_irq(&cgwb_lock);
}

/**
 * wb_memcg_offline - kill all wb's associated with a memcg being offlined
 * @memcg: memcg being offlined
 *
 * Also prevents creation of any new wb's associated with @memcg.
 */
void wb_memcg_offline(struct mem_cgroup *memcg)
{
	struct list_head *memcg_cgwb_list = &memcg->cgwb_list;
	struct bdi_writeback *wb, *next;

	spin_lock_irq(&cgwb_lock);
	list_for_each_entry_safe(wb, next, memcg_cgwb_list, memcg_node)
		cgwb_kill(wb);
	memcg_cgwb_list->next = NULL;	/* prevent new wb's */
	spin_unlock_irq(&cgwb_lock);

	queue_work(system_unbound_wq, &cleanup_offline_cgwbs_work);
}

/**
 * wb_blkcg_offline - kill all wb's associated with a blkcg being offlined
 * @css: blkcg being offlined
 *
 * Also prevents creation of any new wb's associated with @blkcg.
 */
void wb_blkcg_offline(struct cgroup_subsys_state *css)
{
	struct bdi_writeback *wb, *next;
	struct list_head *list = blkcg_get_cgwb_list(css);

	spin_lock_irq(&cgwb_lock);
	list_for_each_entry_safe(wb, next, list, blkcg_node)
		cgwb_kill(wb);
	list->next = NULL;	/* prevent new wb's */
	spin_unlock_irq(&cgwb_lock);
}

static void cgwb_bdi_register(struct backing_dev_info *bdi)
{
	spin_lock_irq(&cgwb_lock);
	list_add_tail_rcu(&bdi->wb.bdi_node, &bdi->wb_list);
	spin_unlock_irq(&cgwb_lock);
}

static int __init cgwb_init(void)
{
	/*
	 * There can be many concurrent release work items overwhelming
	 * system_wq.  Put them in a separate wq and limit concurrency.
	 * There's no point in executing many of these in parallel.
	 */
	cgwb_release_wq = alloc_workqueue("cgwb_release", 0, 1);
	if (!cgwb_release_wq)
		return -ENOMEM;

	return 0;
}
subsys_initcall(cgwb_init);

#else	/* CONFIG_CGROUP_WRITEBACK */

static int cgwb_bdi_init(struct backing_dev_info *bdi)
{
	return wb_init(&bdi->wb, bdi, GFP_KERNEL);
}

static void cgwb_bdi_unregister(struct backing_dev_info *bdi) { }

static void cgwb_bdi_register(struct backing_dev_info *bdi)
{
	list_add_tail_rcu(&bdi->wb.bdi_node, &bdi->wb_list);
}

static void cgwb_remove_from_bdi_list(struct bdi_writeback *wb)
{
	list_del_rcu(&wb->bdi_node);
}

#endif	/* CONFIG_CGROUP_WRITEBACK */

int bdi_init(struct backing_dev_info *bdi)
{
	bdi->dev = NULL;

	kref_init(&bdi->refcnt);
	bdi->min_ratio = 0;
	bdi->max_ratio = 100 * BDI_RATIO_SCALE;
	bdi->max_prop_frac = FPROP_FRAC_BASE;
	INIT_LIST_HEAD(&bdi->bdi_list);
	INIT_LIST_HEAD(&bdi->wb_list);
	init_waitqueue_head(&bdi->wb_waitq);
	bdi->last_bdp_sleep = jiffies;

	return cgwb_bdi_init(bdi);
}

struct backing_dev_info *bdi_alloc(int node_id)
{
	struct backing_dev_info *bdi;

	bdi = kzalloc_node(sizeof(*bdi), GFP_KERNEL, node_id);
	if (!bdi)
		return NULL;

	if (bdi_init(bdi)) {
		kfree(bdi);
		return NULL;
	}
	bdi->capabilities = BDI_CAP_WRITEBACK | BDI_CAP_WRITEBACK_ACCT;
	bdi->ra_pages = VM_READAHEAD_PAGES;
	bdi->io_pages = VM_READAHEAD_PAGES;
	timer_setup(&bdi->laptop_mode_wb_timer, laptop_mode_timer_fn, 0);
	return bdi;
}
EXPORT_SYMBOL(bdi_alloc);

static struct rb_node **bdi_lookup_rb_node(u64 id, struct rb_node **parentp)
{
	struct rb_node **p = &bdi_tree.rb_node;
	struct rb_node *parent = NULL;
	struct backing_dev_info *bdi;

	lockdep_assert_held(&bdi_lock);

	while (*p) {
		parent = *p;
		bdi = rb_entry(parent, struct backing_dev_info, rb_node);

		if (bdi->id > id)
			p = &(*p)->rb_left;
		else if (bdi->id < id)
			p = &(*p)->rb_right;
		else
			break;
	}

	if (parentp)
		*parentp = parent;
	return p;
}

/**
 * bdi_get_by_id - lookup and get bdi from its id
 * @id: bdi id to lookup
 *
 * Find bdi matching @id and get it.  Returns NULL if the matching bdi
 * doesn't exist or is already unregistered.
 */
struct backing_dev_info *bdi_get_by_id(u64 id)
{
	struct backing_dev_info *bdi = NULL;
	struct rb_node **p;

	spin_lock_bh(&bdi_lock);
	p = bdi_lookup_rb_node(id, NULL);
	if (*p) {
		bdi = rb_entry(*p, struct backing_dev_info, rb_node);
		bdi_get(bdi);
	}
	spin_unlock_bh(&bdi_lock);

	return bdi;
}

int bdi_register_va(struct backing_dev_info *bdi, const char *fmt, va_list args)
{
	struct device *dev;
	struct rb_node *parent, **p;

	if (bdi->dev)	/* The driver needs to use separate queues per device */
		return 0;

	vsnprintf(bdi->dev_name, sizeof(bdi->dev_name), fmt, args);
	dev = device_create(&bdi_class, NULL, MKDEV(0, 0), bdi, bdi->dev_name);
	if (IS_ERR(dev))
		return PTR_ERR(dev);

	cgwb_bdi_register(bdi);
	bdi->dev = dev;

	bdi_debug_register(bdi, dev_name(dev));
	set_bit(WB_registered, &bdi->wb.state);

	spin_lock_bh(&bdi_lock);

	bdi->id = ++bdi_id_cursor;

	p = bdi_lookup_rb_node(bdi->id, &parent);
	rb_link_node(&bdi->rb_node, parent, p);
	rb_insert_color(&bdi->rb_node, &bdi_tree);

	list_add_tail_rcu(&bdi->bdi_list, &bdi_list);

	spin_unlock_bh(&bdi_lock);

	trace_writeback_bdi_register(bdi);
	return 0;
}

int bdi_register(struct backing_dev_info *bdi, const char *fmt, ...)
{
	va_list args;
	int ret;

	va_start(args, fmt);
	ret = bdi_register_va(bdi, fmt, args);
	va_end(args);
	return ret;
}
EXPORT_SYMBOL(bdi_register);

void bdi_set_owner(struct backing_dev_info *bdi, struct device *owner)
{
	WARN_ON_ONCE(bdi->owner);
	bdi->owner = owner;
	get_device(owner);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Remove bdi from bdi_list, and ensure that it is no longer visible
 */
static void bdi_remove_from_list(struct backing_dev_info *bdi)
{
	spin_lock_bh(&bdi_lock);
<<<<<<< HEAD
=======
	rb_erase(&bdi->rb_node, &bdi_tree);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_del_rcu(&bdi->bdi_list);
	spin_unlock_bh(&bdi_lock);

	synchronize_rcu_expedited();
}

<<<<<<< HEAD
int bdi_register(struct backing_dev_info *bdi, struct device *parent,
		const char *fmt, ...)
{
	va_list args;
	struct device *dev;

	if (bdi->dev)	/* The driver needs to use separate queues per device */
		return 0;

	va_start(args, fmt);
	dev = device_create_vargs(bdi_class, parent, MKDEV(0, 0), bdi, fmt, args);
	va_end(args);
	if (IS_ERR(dev))
		return PTR_ERR(dev);

	bdi->dev = dev;

	/*
	 * Just start the forker thread for our default backing_dev_info,
	 * and add other bdi's to the list. They will get a thread created
	 * on-demand when they need it.
	 */
	if (bdi_cap_flush_forker(bdi)) {
		struct bdi_writeback *wb = &bdi->wb;

		wb->task = kthread_run(bdi_forker_thread, wb, "bdi-%s",
						dev_name(dev));
		if (IS_ERR(wb->task))
			return PTR_ERR(wb->task);
	}

	bdi_debug_register(bdi, dev_name(dev));
	set_bit(BDI_registered, &bdi->state);

	spin_lock_bh(&bdi_lock);
	list_add_tail_rcu(&bdi->bdi_list, &bdi_list);
	spin_unlock_bh(&bdi_lock);

	trace_writeback_bdi_register(bdi);
	return 0;
}
EXPORT_SYMBOL(bdi_register);

int bdi_register_dev(struct backing_dev_info *bdi, dev_t dev)
{
	return bdi_register(bdi, NULL, "%u:%u", MAJOR(dev), MINOR(dev));
}
EXPORT_SYMBOL(bdi_register_dev);

/*
 * Remove bdi from the global list and shutdown any threads we have running
 */
static void bdi_wb_shutdown(struct backing_dev_info *bdi)
{
	struct task_struct *task;

	if (!bdi_cap_writeback_dirty(bdi))
		return;

	/*
	 * Make sure nobody finds us on the bdi_list anymore
	 */
	bdi_remove_from_list(bdi);

	/*
	 * If setup is pending, wait for that to complete first
	 */
	wait_on_bit(&bdi->state, BDI_pending, bdi_sched_wait,
			TASK_UNINTERRUPTIBLE);

	/*
	 * Finally, kill the kernel thread. We don't need to be RCU
	 * safe anymore, since the bdi is gone from visibility.
	 */
	spin_lock_bh(&bdi->wb_lock);
	task = bdi->wb.task;
	bdi->wb.task = NULL;
	spin_unlock_bh(&bdi->wb_lock);

	if (task)
		kthread_stop(task);
}

/*
 * This bdi is going away now, make sure that no super_blocks point to it
 */
static void bdi_prune_sb(struct backing_dev_info *bdi)
{
	struct super_block *sb;

	spin_lock(&sb_lock);
	list_for_each_entry(sb, &super_blocks, s_list) {
		if (sb->s_bdi == bdi)
			sb->s_bdi = &default_backing_dev_info;
	}
	spin_unlock(&sb_lock);
}

void bdi_unregister(struct backing_dev_info *bdi)
{
	struct device *dev = bdi->dev;

	if (dev) {
		bdi_set_min_ratio(bdi, 0);
		trace_writeback_bdi_unregister(bdi);
		bdi_prune_sb(bdi);
		del_timer_sync(&bdi->wb.wakeup_timer);

		if (!bdi_cap_flush_forker(bdi))
			bdi_wb_shutdown(bdi);
		bdi_debug_unregister(bdi);

		spin_lock_bh(&bdi->wb_lock);
		bdi->dev = NULL;
		spin_unlock_bh(&bdi->wb_lock);

		device_unregister(dev);
=======
void bdi_unregister(struct backing_dev_info *bdi)
{
	del_timer_sync(&bdi->laptop_mode_wb_timer);

	/* make sure nobody finds us on the bdi_list anymore */
	bdi_remove_from_list(bdi);
	wb_shutdown(&bdi->wb);
	cgwb_bdi_unregister(bdi);

	/*
	 * If this BDI's min ratio has been set, use bdi_set_min_ratio() to
	 * update the global bdi_min_ratio.
	 */
	if (bdi->min_ratio)
		bdi_set_min_ratio(bdi, 0);

	if (bdi->dev) {
		bdi_debug_unregister(bdi);
		device_unregister(bdi->dev);
		bdi->dev = NULL;
	}

	if (bdi->owner) {
		put_device(bdi->owner);
		bdi->owner = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
EXPORT_SYMBOL(bdi_unregister);

<<<<<<< HEAD
static void bdi_wb_init(struct bdi_writeback *wb, struct backing_dev_info *bdi)
{
	memset(wb, 0, sizeof(*wb));

	wb->bdi = bdi;
	wb->last_old_flush = jiffies;
	INIT_LIST_HEAD(&wb->b_dirty);
	INIT_LIST_HEAD(&wb->b_io);
	INIT_LIST_HEAD(&wb->b_more_io);
	spin_lock_init(&wb->list_lock);
	setup_timer(&wb->wakeup_timer, wakeup_timer_fn, (unsigned long)bdi);
}

/*
 * Initial write bandwidth: 100 MB/s
 */
#define INIT_BW		(100 << (20 - PAGE_SHIFT))

int bdi_init(struct backing_dev_info *bdi)
{
	int i, err;

	bdi->dev = NULL;

	bdi->min_ratio = 0;
	bdi->max_ratio = 100;
	bdi->max_prop_frac = PROP_FRAC_BASE;
	spin_lock_init(&bdi->wb_lock);
	INIT_LIST_HEAD(&bdi->bdi_list);
	INIT_LIST_HEAD(&bdi->work_list);

	bdi_wb_init(&bdi->wb, bdi);

	for (i = 0; i < NR_BDI_STAT_ITEMS; i++) {
		err = percpu_counter_init(&bdi->bdi_stat[i], 0);
		if (err)
			goto err;
	}

	bdi->dirty_exceeded = 0;

	bdi->bw_time_stamp = jiffies;
	bdi->written_stamp = 0;

	bdi->balanced_dirty_ratelimit = INIT_BW;
	bdi->dirty_ratelimit = INIT_BW;
	bdi->write_bandwidth = INIT_BW;
	bdi->avg_write_bandwidth = INIT_BW;

	err = prop_local_init_percpu(&bdi->completions);

	if (err) {
err:
		while (i--)
			percpu_counter_destroy(&bdi->bdi_stat[i]);
	}

	return err;
}
EXPORT_SYMBOL(bdi_init);

void bdi_destroy(struct backing_dev_info *bdi)
{
	int i;

	/*
	 * Splice our entries to the default_backing_dev_info, if this
	 * bdi disappears
	 */
	if (bdi_has_dirty_io(bdi)) {
		struct bdi_writeback *dst = &default_backing_dev_info.wb;

		bdi_lock_two(&bdi->wb, dst);
		list_splice(&bdi->wb.b_dirty, &dst->b_dirty);
		list_splice(&bdi->wb.b_io, &dst->b_io);
		list_splice(&bdi->wb.b_more_io, &dst->b_more_io);
		spin_unlock(&bdi->wb.list_lock);
		spin_unlock(&dst->list_lock);
	}

	bdi_unregister(bdi);

	/*
	 * If bdi_unregister() had already been called earlier, the
	 * wakeup_timer could still be armed because bdi_prune_sb()
	 * can race with the bdi_wakeup_thread_delayed() calls from
	 * __mark_inode_dirty().
	 */
	del_timer_sync(&bdi->wb.wakeup_timer);

	for (i = 0; i < NR_BDI_STAT_ITEMS; i++)
		percpu_counter_destroy(&bdi->bdi_stat[i]);

	prop_local_destroy_percpu(&bdi->completions);
}
EXPORT_SYMBOL(bdi_destroy);

/*
 * For use from filesystems to quickly init and register a bdi associated
 * with dirty writeback
 */
int bdi_setup_and_register(struct backing_dev_info *bdi, char *name,
			   unsigned int cap)
{
	char tmp[32];
	int err;

	bdi->name = name;
	bdi->capabilities = cap;
	err = bdi_init(bdi);
	if (err)
		return err;

	sprintf(tmp, "%.28s%s", name, "-%d");
	err = bdi_register(bdi, NULL, tmp, atomic_long_inc_return(&bdi_seq));
	if (err) {
		bdi_destroy(bdi);
		return err;
	}

	return 0;
}
EXPORT_SYMBOL(bdi_setup_and_register);

static wait_queue_head_t congestion_wqh[2] = {
		__WAIT_QUEUE_HEAD_INITIALIZER(congestion_wqh[0]),
		__WAIT_QUEUE_HEAD_INITIALIZER(congestion_wqh[1])
	};
static atomic_t nr_bdi_congested[2];

void clear_bdi_congested(struct backing_dev_info *bdi, int sync)
{
	enum bdi_state bit;
	wait_queue_head_t *wqh = &congestion_wqh[sync];

	bit = sync ? BDI_sync_congested : BDI_async_congested;
	if (test_and_clear_bit(bit, &bdi->state))
		atomic_dec(&nr_bdi_congested[sync]);
	smp_mb__after_clear_bit();
	if (waitqueue_active(wqh))
		wake_up(wqh);
}
EXPORT_SYMBOL(clear_bdi_congested);

void set_bdi_congested(struct backing_dev_info *bdi, int sync)
{
	enum bdi_state bit;

	bit = sync ? BDI_sync_congested : BDI_async_congested;
	if (!test_and_set_bit(bit, &bdi->state))
		atomic_inc(&nr_bdi_congested[sync]);
}
EXPORT_SYMBOL(set_bdi_congested);

/**
 * congestion_wait - wait for a backing_dev to become uncongested
 * @sync: SYNC or ASYNC IO
 * @timeout: timeout in jiffies
 *
 * Waits for up to @timeout jiffies for a backing_dev (any backing_dev) to exit
 * write congestion.  If no backing_devs are congested then just wait for the
 * next write to be completed.
 */
long congestion_wait(int sync, long timeout)
{
	long ret;
	unsigned long start = jiffies;
	DEFINE_WAIT(wait);
	wait_queue_head_t *wqh = &congestion_wqh[sync];

	prepare_to_wait(wqh, &wait, TASK_UNINTERRUPTIBLE);
	ret = io_schedule_timeout(timeout);
	finish_wait(wqh, &wait);

	trace_writeback_congestion_wait(jiffies_to_usecs(timeout),
					jiffies_to_usecs(jiffies - start));

	return ret;
}
EXPORT_SYMBOL(congestion_wait);

/**
 * wait_iff_congested - Conditionally wait for a backing_dev to become uncongested or a zone to complete writes
 * @zone: A zone to check if it is heavily congested
 * @sync: SYNC or ASYNC IO
 * @timeout: timeout in jiffies
 *
 * In the event of a congested backing_dev (any backing_dev) and the given
 * @zone has experienced recent congestion, this waits for up to @timeout
 * jiffies for either a BDI to exit congestion of the given @sync queue
 * or a write to complete.
 *
 * In the absence of zone congestion, a short sleep or a cond_resched is
 * performed to yield the processor and to allow other subsystems to make
 * a forward progress.
 *
 * The return value is 0 if the sleep is for the full timeout. Otherwise,
 * it is the number of jiffies that were still remaining when the function
 * returned. return_value == timeout implies the function did not sleep.
 */
long wait_iff_congested(struct zone *zone, int sync, long timeout)
{
	long ret;
	unsigned long start = jiffies;
	DEFINE_WAIT(wait);
	wait_queue_head_t *wqh = &congestion_wqh[sync];

	/*
	 * If there is no congestion, or heavy congestion is not being
	 * encountered in the current zone, yield if necessary instead
	 * of sleeping on the congestion queue
	 */
	if (atomic_read(&nr_bdi_congested[sync]) == 0 ||
			!zone_is_reclaim_congested(zone)) {

		/*
		 * Memory allocation/reclaim might be called from a WQ
		 * context and the current implementation of the WQ
		 * concurrency control doesn't recognize that a particular
		 * WQ is congested if the worker thread is looping without
		 * ever sleeping. Therefore we have to do a short sleep
		 * here rather than calling cond_resched().
		 */
		if (current->flags & PF_WQ_WORKER)
			schedule_timeout_uninterruptible(1);
		else
			cond_resched();

		/* In case we scheduled, work out time remaining */
		ret = timeout - (jiffies - start);
		if (ret < 0)
			ret = 0;

		goto out;
	}

	/* Sleep until uncongested or a write happens */
	prepare_to_wait(wqh, &wait, TASK_UNINTERRUPTIBLE);
	ret = io_schedule_timeout(timeout);
	finish_wait(wqh, &wait);

out:
	trace_writeback_wait_iff_congested(jiffies_to_usecs(timeout),
					jiffies_to_usecs(jiffies - start));

	return ret;
}
EXPORT_SYMBOL(wait_iff_congested);
=======
static void release_bdi(struct kref *ref)
{
	struct backing_dev_info *bdi =
			container_of(ref, struct backing_dev_info, refcnt);

	WARN_ON_ONCE(test_bit(WB_registered, &bdi->wb.state));
	WARN_ON_ONCE(bdi->dev);
	wb_exit(&bdi->wb);
	kfree(bdi);
}

void bdi_put(struct backing_dev_info *bdi)
{
	kref_put(&bdi->refcnt, release_bdi);
}
EXPORT_SYMBOL(bdi_put);

struct backing_dev_info *inode_to_bdi(struct inode *inode)
{
	struct super_block *sb;

	if (!inode)
		return &noop_backing_dev_info;

	sb = inode->i_sb;
#ifdef CONFIG_BLOCK
	if (sb_is_blkdev_sb(sb))
		return I_BDEV(inode)->bd_disk->bdi;
#endif
	return sb->s_bdi;
}
EXPORT_SYMBOL(inode_to_bdi);

const char *bdi_dev_name(struct backing_dev_info *bdi)
{
	if (!bdi || !bdi->dev)
		return bdi_unknown_name;
	return bdi->dev_name;
}
EXPORT_SYMBOL_GPL(bdi_dev_name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
