<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * DMA Engine test module
 *
 * Copyright (C) 2007 Atmel Corporation
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
=======
 * Copyright (C) 2013 Intel Corporation
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/err.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/freezer.h>
#include <linux/init.h>
#include <linux/kthread.h>
<<<<<<< HEAD
=======
#include <linux/sched/task.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/wait.h>

<<<<<<< HEAD
static unsigned int test_buf_size = 16384;
module_param(test_buf_size, uint, S_IRUGO);
MODULE_PARM_DESC(test_buf_size, "Size of the memcpy test buffer");

static char test_channel[20];
module_param_string(channel, test_channel, sizeof(test_channel), S_IRUGO);
MODULE_PARM_DESC(channel, "Bus ID of the channel to test (default: any)");

static char test_device[20];
module_param_string(device, test_device, sizeof(test_device), S_IRUGO);
MODULE_PARM_DESC(device, "Bus ID of the DMA Engine to test (default: any)");

static unsigned int threads_per_chan = 1;
module_param(threads_per_chan, uint, S_IRUGO);
=======
static bool nobounce;
module_param(nobounce, bool, 0644);
MODULE_PARM_DESC(nobounce, "Prevent using swiotlb buffer (default: use swiotlb buffer)");

static unsigned int test_buf_size = 16384;
module_param(test_buf_size, uint, 0644);
MODULE_PARM_DESC(test_buf_size, "Size of the memcpy test buffer");

static char test_device[32];
module_param_string(device, test_device, sizeof(test_device), 0644);
MODULE_PARM_DESC(device, "Bus ID of the DMA Engine to test (default: any)");

static unsigned int threads_per_chan = 1;
module_param(threads_per_chan, uint, 0644);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_PARM_DESC(threads_per_chan,
		"Number of threads to start per channel (default: 1)");

static unsigned int max_channels;
<<<<<<< HEAD
module_param(max_channels, uint, S_IRUGO);
=======
module_param(max_channels, uint, 0644);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_PARM_DESC(max_channels,
		"Maximum number of channels to use (default: all)");

static unsigned int iterations;
<<<<<<< HEAD
module_param(iterations, uint, S_IRUGO);
MODULE_PARM_DESC(iterations,
		"Iterations before stopping test (default: infinite)");

static unsigned int xor_sources = 3;
module_param(xor_sources, uint, S_IRUGO);
=======
module_param(iterations, uint, 0644);
MODULE_PARM_DESC(iterations,
		"Iterations before stopping test (default: infinite)");

static unsigned int dmatest;
module_param(dmatest, uint, 0644);
MODULE_PARM_DESC(dmatest,
		"dmatest 0-memcpy 1-memset (default: 0)");

static unsigned int xor_sources = 3;
module_param(xor_sources, uint, 0644);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_PARM_DESC(xor_sources,
		"Number of xor source buffers (default: 3)");

static unsigned int pq_sources = 3;
<<<<<<< HEAD
module_param(pq_sources, uint, S_IRUGO);
=======
module_param(pq_sources, uint, 0644);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_PARM_DESC(pq_sources,
		"Number of p+q source buffers (default: 3)");

static int timeout = 3000;
<<<<<<< HEAD
module_param(timeout, uint, S_IRUGO);
MODULE_PARM_DESC(timeout, "Transfer Timeout in msec (default: 3000), "
		 "Pass -1 for infinite timeout");

=======
module_param(timeout, int, 0644);
MODULE_PARM_DESC(timeout, "Transfer Timeout in msec (default: 3000), "
		 "Pass -1 for infinite timeout");

static bool noverify;
module_param(noverify, bool, 0644);
MODULE_PARM_DESC(noverify, "Disable data verification (default: verify)");

static bool norandom;
module_param(norandom, bool, 0644);
MODULE_PARM_DESC(norandom, "Disable random offset setup (default: random)");

static bool verbose;
module_param(verbose, bool, 0644);
MODULE_PARM_DESC(verbose, "Enable \"success\" result messages (default: off)");

static int alignment = -1;
module_param(alignment, int, 0644);
MODULE_PARM_DESC(alignment, "Custom data address alignment taken as 2^(alignment) (default: not used (-1))");

static unsigned int transfer_size;
module_param(transfer_size, uint, 0644);
MODULE_PARM_DESC(transfer_size, "Optional custom transfer size in bytes (default: not used (0))");

static bool polled;
module_param(polled, bool, 0644);
MODULE_PARM_DESC(polled, "Use polling for completion instead of interrupts");

/**
 * struct dmatest_params - test parameters.
 * @nobounce:		prevent using swiotlb buffer
 * @buf_size:		size of the memcpy test buffer
 * @channel:		bus ID of the channel to test
 * @device:		bus ID of the DMA Engine to test
 * @threads_per_chan:	number of threads to start per channel
 * @max_channels:	maximum number of channels to use
 * @iterations:		iterations before stopping test
 * @xor_sources:	number of xor source buffers
 * @pq_sources:		number of p+q source buffers
 * @timeout:		transfer timeout in msec, -1 for infinite timeout
 * @noverify:		disable data verification
 * @norandom:		disable random offset setup
 * @alignment:		custom data address alignment taken as 2^alignment
 * @transfer_size:	custom transfer size in bytes
 * @polled:		use polling for completion instead of interrupts
 */
struct dmatest_params {
	bool		nobounce;
	unsigned int	buf_size;
	char		channel[20];
	char		device[32];
	unsigned int	threads_per_chan;
	unsigned int	max_channels;
	unsigned int	iterations;
	unsigned int	xor_sources;
	unsigned int	pq_sources;
	int		timeout;
	bool		noverify;
	bool		norandom;
	int		alignment;
	unsigned int	transfer_size;
	bool		polled;
};

/**
 * struct dmatest_info - test information.
 * @params:		test parameters
 * @channels:		channels under test
 * @nr_channels:	number of channels under test
 * @lock:		access protection to the fields of this structure
 * @did_init:		module has been initialized completely
 * @last_error:		test has faced configuration issues
 */
static struct dmatest_info {
	/* Test parameters */
	struct dmatest_params	params;

	/* Internal state */
	struct list_head	channels;
	unsigned int		nr_channels;
	int			last_error;
	struct mutex		lock;
	bool			did_init;
} test_info = {
	.channels = LIST_HEAD_INIT(test_info.channels),
	.lock = __MUTEX_INITIALIZER(test_info.lock),
};

static int dmatest_run_set(const char *val, const struct kernel_param *kp);
static int dmatest_run_get(char *val, const struct kernel_param *kp);
static const struct kernel_param_ops run_ops = {
	.set = dmatest_run_set,
	.get = dmatest_run_get,
};
static bool dmatest_run;
module_param_cb(run, &run_ops, &dmatest_run, 0644);
MODULE_PARM_DESC(run, "Run the test (default: false)");

static int dmatest_chan_set(const char *val, const struct kernel_param *kp);
static int dmatest_chan_get(char *val, const struct kernel_param *kp);
static const struct kernel_param_ops multi_chan_ops = {
	.set = dmatest_chan_set,
	.get = dmatest_chan_get,
};

static char test_channel[20];
static struct kparam_string newchan_kps = {
	.string = test_channel,
	.maxlen = 20,
};
module_param_cb(channel, &multi_chan_ops, &newchan_kps, 0644);
MODULE_PARM_DESC(channel, "Bus ID of the channel to test (default: any)");

static int dmatest_test_list_get(char *val, const struct kernel_param *kp);
static const struct kernel_param_ops test_list_ops = {
	.get = dmatest_test_list_get,
};
module_param_cb(test_list, &test_list_ops, NULL, 0444);
MODULE_PARM_DESC(test_list, "Print current test list");

/* Maximum amount of mismatched bytes in buffer to print */
#define MAX_ERROR_COUNT		32

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Initialization patterns. All bytes in the source buffer has bit 7
 * set, all bytes in the destination buffer has bit 7 cleared.
 *
 * Bit 6 is set for all bytes which are to be copied by the DMA
 * engine. Bit 5 is set for all bytes which are to be overwritten by
 * the DMA engine.
 *
 * The remaining bits are the inverse of a counter which increments by
 * one for each byte address.
 */
#define PATTERN_SRC		0x80
#define PATTERN_DST		0x00
#define PATTERN_COPY		0x40
#define PATTERN_OVERWRITE	0x20
#define PATTERN_COUNT_MASK	0x1f
<<<<<<< HEAD

struct dmatest_thread {
	struct list_head	node;
	struct task_struct	*task;
	struct dma_chan		*chan;
	u8			**srcs;
	u8			**dsts;
	enum dma_transaction_type type;
=======
#define PATTERN_MEMSET_IDX	0x01

/* Fixed point arithmetic ops */
#define FIXPT_SHIFT		8
#define FIXPNT_MASK		0xFF
#define FIXPT_TO_INT(a)	((a) >> FIXPT_SHIFT)
#define INT_TO_FIXPT(a)	((a) << FIXPT_SHIFT)
#define FIXPT_GET_FRAC(a)	((((a) & FIXPNT_MASK) * 100) >> FIXPT_SHIFT)

/* poor man's completion - we want to use wait_event_freezable() on it */
struct dmatest_done {
	bool			done;
	wait_queue_head_t	*wait;
};

struct dmatest_data {
	u8		**raw;
	u8		**aligned;
	gfp_t		gfp_flags;
	unsigned int	cnt;
	unsigned int	off;
};

struct dmatest_thread {
	struct list_head	node;
	struct dmatest_info	*info;
	struct task_struct	*task;
	struct dma_chan		*chan;
	struct dmatest_data	src;
	struct dmatest_data	dst;
	enum dma_transaction_type type;
	wait_queue_head_t done_wait;
	struct dmatest_done test_done;
	bool			done;
	bool			pending;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct dmatest_chan {
	struct list_head	node;
	struct dma_chan		*chan;
	struct list_head	threads;
};

<<<<<<< HEAD
/*
 * These are protected by dma_list_mutex since they're only used by
 * the DMA filter function callback
 */
static LIST_HEAD(dmatest_channels);
static unsigned int nr_channels;

static bool dmatest_match_channel(struct dma_chan *chan)
{
	if (test_channel[0] == '\0')
		return true;
	return strcmp(dma_chan_name(chan), test_channel) == 0;
}

static bool dmatest_match_device(struct dma_device *device)
{
	if (test_device[0] == '\0')
		return true;
	return strcmp(dev_name(device->dev), test_device) == 0;
=======
static DECLARE_WAIT_QUEUE_HEAD(thread_wait);
static bool wait;

static bool is_threaded_test_run(struct dmatest_info *info)
{
	struct dmatest_chan *dtc;

	list_for_each_entry(dtc, &info->channels, node) {
		struct dmatest_thread *thread;

		list_for_each_entry(thread, &dtc->threads, node) {
			if (!thread->done && !thread->pending)
				return true;
		}
	}

	return false;
}

static bool is_threaded_test_pending(struct dmatest_info *info)
{
	struct dmatest_chan *dtc;

	list_for_each_entry(dtc, &info->channels, node) {
		struct dmatest_thread *thread;

		list_for_each_entry(thread, &dtc->threads, node) {
			if (thread->pending)
				return true;
		}
	}

	return false;
}

static int dmatest_wait_get(char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;
	struct dmatest_params *params = &info->params;

	if (params->iterations)
		wait_event(thread_wait, !is_threaded_test_run(info));
	wait = true;
	return param_get_bool(val, kp);
}

static const struct kernel_param_ops wait_ops = {
	.get = dmatest_wait_get,
	.set = param_set_bool,
};
module_param_cb(wait, &wait_ops, &wait, 0444);
MODULE_PARM_DESC(wait, "Wait for tests to complete (default: false)");

static bool dmatest_match_channel(struct dmatest_params *params,
		struct dma_chan *chan)
{
	if (params->channel[0] == '\0')
		return true;
	return strcmp(dma_chan_name(chan), params->channel) == 0;
}

static bool dmatest_match_device(struct dmatest_params *params,
		struct dma_device *device)
{
	if (params->device[0] == '\0')
		return true;
	return strcmp(dev_name(device->dev), params->device) == 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned long dmatest_random(void)
{
	unsigned long buf;

	get_random_bytes(&buf, sizeof(buf));
	return buf;
}

<<<<<<< HEAD
static void dmatest_init_srcs(u8 **bufs, unsigned int start, unsigned int len)
=======
static inline u8 gen_inv_idx(u8 index, bool is_memset)
{
	u8 val = is_memset ? PATTERN_MEMSET_IDX : index;

	return ~val & PATTERN_COUNT_MASK;
}

static inline u8 gen_src_value(u8 index, bool is_memset)
{
	return PATTERN_SRC | gen_inv_idx(index, is_memset);
}

static inline u8 gen_dst_value(u8 index, bool is_memset)
{
	return PATTERN_DST | gen_inv_idx(index, is_memset);
}

static void dmatest_init_srcs(u8 **bufs, unsigned int start, unsigned int len,
		unsigned int buf_size, bool is_memset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int i;
	u8 *buf;

	for (; (buf = *bufs); bufs++) {
		for (i = 0; i < start; i++)
<<<<<<< HEAD
			buf[i] = PATTERN_SRC | (~i & PATTERN_COUNT_MASK);
		for ( ; i < start + len; i++)
			buf[i] = PATTERN_SRC | PATTERN_COPY
				| (~i & PATTERN_COUNT_MASK);
		for ( ; i < test_buf_size; i++)
			buf[i] = PATTERN_SRC | (~i & PATTERN_COUNT_MASK);
=======
			buf[i] = gen_src_value(i, is_memset);
		for ( ; i < start + len; i++)
			buf[i] = gen_src_value(i, is_memset) | PATTERN_COPY;
		for ( ; i < buf_size; i++)
			buf[i] = gen_src_value(i, is_memset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		buf++;
	}
}

<<<<<<< HEAD
static void dmatest_init_dsts(u8 **bufs, unsigned int start, unsigned int len)
=======
static void dmatest_init_dsts(u8 **bufs, unsigned int start, unsigned int len,
		unsigned int buf_size, bool is_memset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int i;
	u8 *buf;

	for (; (buf = *bufs); bufs++) {
		for (i = 0; i < start; i++)
<<<<<<< HEAD
			buf[i] = PATTERN_DST | (~i & PATTERN_COUNT_MASK);
		for ( ; i < start + len; i++)
			buf[i] = PATTERN_DST | PATTERN_OVERWRITE
				| (~i & PATTERN_COUNT_MASK);
		for ( ; i < test_buf_size; i++)
			buf[i] = PATTERN_DST | (~i & PATTERN_COUNT_MASK);
=======
			buf[i] = gen_dst_value(i, is_memset);
		for ( ; i < start + len; i++)
			buf[i] = gen_dst_value(i, is_memset) |
						PATTERN_OVERWRITE;
		for ( ; i < buf_size; i++)
			buf[i] = gen_dst_value(i, is_memset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void dmatest_mismatch(u8 actual, u8 pattern, unsigned int index,
<<<<<<< HEAD
		unsigned int counter, bool is_srcbuf)
{
	u8		diff = actual ^ pattern;
	u8		expected = pattern | (~counter & PATTERN_COUNT_MASK);
	const char	*thread_name = current->comm;

	if (is_srcbuf)
		pr_warning("%s: srcbuf[0x%x] overwritten!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
	else if ((pattern & PATTERN_COPY)
			&& (diff & (PATTERN_COPY | PATTERN_OVERWRITE)))
		pr_warning("%s: dstbuf[0x%x] not copied!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
	else if (diff & PATTERN_SRC)
		pr_warning("%s: dstbuf[0x%x] was copied!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
	else
		pr_warning("%s: dstbuf[0x%x] mismatch!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
=======
		unsigned int counter, bool is_srcbuf, bool is_memset)
{
	u8		diff = actual ^ pattern;
	u8		expected = pattern | gen_inv_idx(counter, is_memset);
	const char	*thread_name = current->comm;

	if (is_srcbuf)
		pr_warn("%s: srcbuf[0x%x] overwritten! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
	else if ((pattern & PATTERN_COPY)
			&& (diff & (PATTERN_COPY | PATTERN_OVERWRITE)))
		pr_warn("%s: dstbuf[0x%x] not copied! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
	else if (diff & PATTERN_SRC)
		pr_warn("%s: dstbuf[0x%x] was copied! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
	else
		pr_warn("%s: dstbuf[0x%x] mismatch! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned int dmatest_verify(u8 **bufs, unsigned int start,
		unsigned int end, unsigned int counter, u8 pattern,
<<<<<<< HEAD
		bool is_srcbuf)
=======
		bool is_srcbuf, bool is_memset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int i;
	unsigned int error_count = 0;
	u8 actual;
	u8 expected;
	u8 *buf;
	unsigned int counter_orig = counter;

	for (; (buf = *bufs); bufs++) {
		counter = counter_orig;
		for (i = start; i < end; i++) {
			actual = buf[i];
<<<<<<< HEAD
			expected = pattern | (~counter & PATTERN_COUNT_MASK);
			if (actual != expected) {
				if (error_count < 32)
					dmatest_mismatch(actual, pattern, i,
							 counter, is_srcbuf);
=======
			expected = pattern | gen_inv_idx(counter, is_memset);
			if (actual != expected) {
				if (error_count < MAX_ERROR_COUNT)
					dmatest_mismatch(actual, pattern, i,
							 counter, is_srcbuf,
							 is_memset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				error_count++;
			}
			counter++;
		}
	}

<<<<<<< HEAD
	if (error_count > 32)
		pr_warning("%s: %u errors suppressed\n",
			current->comm, error_count - 32);
=======
	if (error_count > MAX_ERROR_COUNT)
		pr_warn("%s: %u errors suppressed\n",
			current->comm, error_count - MAX_ERROR_COUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return error_count;
}

<<<<<<< HEAD
/* poor man's completion - we want to use wait_event_freezable() on it */
struct dmatest_done {
	bool			done;
	wait_queue_head_t	*wait;
};
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void dmatest_callback(void *arg)
{
	struct dmatest_done *done = arg;
<<<<<<< HEAD

	done->done = true;
	wake_up_all(done->wait);
=======
	struct dmatest_thread *thread =
		container_of(done, struct dmatest_thread, test_done);
	if (!thread->done) {
		done->done = true;
		wake_up_all(done->wait);
	} else {
		/*
		 * If thread->done, it means that this callback occurred
		 * after the parent thread has cleaned up. This can
		 * happen in the case that driver doesn't implement
		 * the terminate_all() functionality and a dma operation
		 * did not occur within the timeout period
		 */
		WARN(1, "dmatest: Kernel memory may be corrupted!!\n");
	}
}

static unsigned int min_odd(unsigned int x, unsigned int y)
{
	unsigned int val = min(x, y);

	return val % 2 ? val : val - 1;
}

static void result(const char *err, unsigned int n, unsigned int src_off,
		   unsigned int dst_off, unsigned int len, unsigned long data)
{
	if (IS_ERR_VALUE(data)) {
		pr_info("%s: result #%u: '%s' with src_off=0x%x dst_off=0x%x len=0x%x (%ld)\n",
			current->comm, n, err, src_off, dst_off, len, data);
	} else {
		pr_info("%s: result #%u: '%s' with src_off=0x%x dst_off=0x%x len=0x%x (%lu)\n",
			current->comm, n, err, src_off, dst_off, len, data);
	}
}

static void dbg_result(const char *err, unsigned int n, unsigned int src_off,
		       unsigned int dst_off, unsigned int len,
		       unsigned long data)
{
	pr_debug("%s: result #%u: '%s' with src_off=0x%x dst_off=0x%x len=0x%x (%lu)\n",
		 current->comm, n, err, src_off, dst_off, len, data);
}

#define verbose_result(err, n, src_off, dst_off, len, data) ({	\
	if (verbose)						\
		result(err, n, src_off, dst_off, len, data);	\
	else							\
		dbg_result(err, n, src_off, dst_off, len, data);\
})

static unsigned long long dmatest_persec(s64 runtime, unsigned int val)
{
	unsigned long long per_sec = 1000000;

	if (runtime <= 0)
		return 0;

	/* drop precision until runtime is 32-bits */
	while (runtime > UINT_MAX) {
		runtime >>= 1;
		per_sec <<= 1;
	}

	per_sec *= val;
	per_sec = INT_TO_FIXPT(per_sec);
	do_div(per_sec, runtime);

	return per_sec;
}

static unsigned long long dmatest_KBs(s64 runtime, unsigned long long len)
{
	return FIXPT_TO_INT(dmatest_persec(runtime, len >> 10));
}

static void __dmatest_free_test_data(struct dmatest_data *d, unsigned int cnt)
{
	unsigned int i;

	for (i = 0; i < cnt; i++)
		kfree(d->raw[i]);

	kfree(d->aligned);
	kfree(d->raw);
}

static void dmatest_free_test_data(struct dmatest_data *d)
{
	__dmatest_free_test_data(d, d->cnt);
}

static int dmatest_alloc_test_data(struct dmatest_data *d,
		unsigned int buf_size, u8 align)
{
	unsigned int i = 0;

	d->raw = kcalloc(d->cnt + 1, sizeof(u8 *), GFP_KERNEL);
	if (!d->raw)
		return -ENOMEM;

	d->aligned = kcalloc(d->cnt + 1, sizeof(u8 *), GFP_KERNEL);
	if (!d->aligned)
		goto err;

	for (i = 0; i < d->cnt; i++) {
		d->raw[i] = kmalloc(buf_size + align, d->gfp_flags);
		if (!d->raw[i])
			goto err;

		/* align to alignment restriction */
		if (align)
			d->aligned[i] = PTR_ALIGN(d->raw[i], align);
		else
			d->aligned[i] = d->raw[i];
	}

	return 0;
err:
	__dmatest_free_test_data(d, i);
	return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This function repeatedly tests DMA transfers of various lengths and
 * offsets for a given operation type until it is told to exit by
 * kthread_stop(). There may be multiple threads running this function
 * in parallel for a single channel, and there may be multiple channels
 * being tested in parallel.
 *
 * Before each test, the source and destination buffer is initialized
 * with a known pattern. This pattern is different depending on
 * whether it's in an area which is supposed to be copied or
 * overwritten, and different in the source and destination buffers.
 * So if the DMA engine doesn't copy exactly what we tell it to copy,
 * we'll notice.
 */
static int dmatest_func(void *data)
{
<<<<<<< HEAD
	DECLARE_WAIT_QUEUE_HEAD_ONSTACK(done_wait);
	struct dmatest_thread	*thread = data;
	struct dmatest_done	done = { .wait = &done_wait };
	struct dma_chan		*chan;
	const char		*thread_name;
	unsigned int		src_off, dst_off, len;
=======
	struct dmatest_thread	*thread = data;
	struct dmatest_done	*done = &thread->test_done;
	struct dmatest_info	*info;
	struct dmatest_params	*params;
	struct dma_chan		*chan;
	struct dma_device	*dev;
	struct device		*dma_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int		error_count;
	unsigned int		failed_tests = 0;
	unsigned int		total_tests = 0;
	dma_cookie_t		cookie;
	enum dma_status		status;
<<<<<<< HEAD
	enum dma_ctrl_flags 	flags;
	u8			pq_coefs[pq_sources + 1];
	int			ret;
	int			src_cnt;
	int			dst_cnt;
	int			i;

	thread_name = current->comm;
=======
	enum dma_ctrl_flags	flags;
	u8			*pq_coefs = NULL;
	int			ret;
	unsigned int		buf_size;
	struct dmatest_data	*src;
	struct dmatest_data	*dst;
	int			i;
	ktime_t			ktime, start, diff;
	ktime_t			filltime = 0;
	ktime_t			comparetime = 0;
	s64			runtime = 0;
	unsigned long long	total_len = 0;
	unsigned long long	iops = 0;
	u8			align = 0;
	bool			is_memset = false;
	dma_addr_t		*srcs;
	dma_addr_t		*dma_pq;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_freezable();

	ret = -ENOMEM;

	smp_rmb();
<<<<<<< HEAD
	chan = thread->chan;
	if (thread->type == DMA_MEMCPY)
		src_cnt = dst_cnt = 1;
	else if (thread->type == DMA_XOR) {
		src_cnt = xor_sources | 1; /* force odd to ensure dst = src */
		dst_cnt = 1;
	} else if (thread->type == DMA_PQ) {
		src_cnt = pq_sources | 1; /* force odd to ensure dst = src */
		dst_cnt = 2;
		for (i = 0; i < src_cnt; i++)
			pq_coefs[i] = 1;
	} else
		goto err_srcs;

	thread->srcs = kcalloc(src_cnt+1, sizeof(u8 *), GFP_KERNEL);
	if (!thread->srcs)
		goto err_srcs;
	for (i = 0; i < src_cnt; i++) {
		thread->srcs[i] = kmalloc(test_buf_size, GFP_KERNEL);
		if (!thread->srcs[i])
			goto err_srcbuf;
	}
	thread->srcs[i] = NULL;

	thread->dsts = kcalloc(dst_cnt+1, sizeof(u8 *), GFP_KERNEL);
	if (!thread->dsts)
		goto err_dsts;
	for (i = 0; i < dst_cnt; i++) {
		thread->dsts[i] = kmalloc(test_buf_size, GFP_KERNEL);
		if (!thread->dsts[i])
			goto err_dstbuf;
	}
	thread->dsts[i] = NULL;

	set_user_nice(current, 10);

	/*
	 * src buffers are freed by the DMAEngine code with dma_unmap_single()
	 * dst buffers are freed by ourselves below
	 */
	flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT
	      | DMA_COMPL_SKIP_DEST_UNMAP | DMA_COMPL_SRC_UNMAP_SINGLE;

	while (!kthread_should_stop()
	       && !(iterations && total_tests >= iterations)) {
		struct dma_device *dev = chan->device;
		struct dma_async_tx_descriptor *tx = NULL;
		dma_addr_t dma_srcs[src_cnt];
		dma_addr_t dma_dsts[dst_cnt];
		u8 align = 0;

		total_tests++;

		/* honor alignment restrictions */
		if (thread->type == DMA_MEMCPY)
			align = dev->copy_align;
		else if (thread->type == DMA_XOR)
			align = dev->xor_align;
		else if (thread->type == DMA_PQ)
			align = dev->pq_align;

		if (1 << align > test_buf_size) {
			pr_err("%u-byte buffer too small for %d-byte alignment\n",
			       test_buf_size, 1 << align);
			break;
		}

		len = dmatest_random() % test_buf_size + 1;
		len = (len >> align) << align;
		if (!len)
			len = 1 << align;
		src_off = dmatest_random() % (test_buf_size - len + 1);
		dst_off = dmatest_random() % (test_buf_size - len + 1);

		src_off = (src_off >> align) << align;
		dst_off = (dst_off >> align) << align;

		dmatest_init_srcs(thread->srcs, src_off, len);
		dmatest_init_dsts(thread->dsts, dst_off, len);

		for (i = 0; i < src_cnt; i++) {
			u8 *buf = thread->srcs[i] + src_off;

			dma_srcs[i] = dma_map_single(dev->dev, buf, len,
						     DMA_TO_DEVICE);
		}
		/* map with DMA_BIDIRECTIONAL to force writeback/invalidate */
		for (i = 0; i < dst_cnt; i++) {
			dma_dsts[i] = dma_map_single(dev->dev, thread->dsts[i],
						     test_buf_size,
						     DMA_BIDIRECTIONAL);
		}


		if (thread->type == DMA_MEMCPY)
			tx = dev->device_prep_dma_memcpy(chan,
							 dma_dsts[0] + dst_off,
							 dma_srcs[0], len,
							 flags);
		else if (thread->type == DMA_XOR)
			tx = dev->device_prep_dma_xor(chan,
						      dma_dsts[0] + dst_off,
						      dma_srcs, src_cnt,
						      len, flags);
		else if (thread->type == DMA_PQ) {
			dma_addr_t dma_pq[dst_cnt];

			for (i = 0; i < dst_cnt; i++)
				dma_pq[i] = dma_dsts[i] + dst_off;
			tx = dev->device_prep_dma_pq(chan, dma_pq, dma_srcs,
						     src_cnt, pq_coefs,
=======
	thread->pending = false;
	info = thread->info;
	params = &info->params;
	chan = thread->chan;
	dev = chan->device;
	dma_dev = dmaengine_get_dma_device(chan);

	src = &thread->src;
	dst = &thread->dst;
	if (thread->type == DMA_MEMCPY) {
		align = params->alignment < 0 ? dev->copy_align :
						params->alignment;
		src->cnt = dst->cnt = 1;
	} else if (thread->type == DMA_MEMSET) {
		align = params->alignment < 0 ? dev->fill_align :
						params->alignment;
		src->cnt = dst->cnt = 1;
		is_memset = true;
	} else if (thread->type == DMA_XOR) {
		/* force odd to ensure dst = src */
		src->cnt = min_odd(params->xor_sources | 1, dev->max_xor);
		dst->cnt = 1;
		align = params->alignment < 0 ? dev->xor_align :
						params->alignment;
	} else if (thread->type == DMA_PQ) {
		/* force odd to ensure dst = src */
		src->cnt = min_odd(params->pq_sources | 1, dma_maxpq(dev, 0));
		dst->cnt = 2;
		align = params->alignment < 0 ? dev->pq_align :
						params->alignment;

		pq_coefs = kmalloc(params->pq_sources + 1, GFP_KERNEL);
		if (!pq_coefs)
			goto err_thread_type;

		for (i = 0; i < src->cnt; i++)
			pq_coefs[i] = 1;
	} else
		goto err_thread_type;

	/* Check if buffer count fits into map count variable (u8) */
	if ((src->cnt + dst->cnt) >= 255) {
		pr_err("too many buffers (%d of 255 supported)\n",
		       src->cnt + dst->cnt);
		goto err_free_coefs;
	}

	buf_size = params->buf_size;
	if (1 << align > buf_size) {
		pr_err("%u-byte buffer too small for %d-byte alignment\n",
		       buf_size, 1 << align);
		goto err_free_coefs;
	}

	src->gfp_flags = GFP_KERNEL;
	dst->gfp_flags = GFP_KERNEL;
	if (params->nobounce) {
		src->gfp_flags = GFP_DMA;
		dst->gfp_flags = GFP_DMA;
	}

	if (dmatest_alloc_test_data(src, buf_size, align) < 0)
		goto err_free_coefs;

	if (dmatest_alloc_test_data(dst, buf_size, align) < 0)
		goto err_src;

	set_user_nice(current, 10);

	srcs = kcalloc(src->cnt, sizeof(dma_addr_t), GFP_KERNEL);
	if (!srcs)
		goto err_dst;

	dma_pq = kcalloc(dst->cnt, sizeof(dma_addr_t), GFP_KERNEL);
	if (!dma_pq)
		goto err_srcs_array;

	/*
	 * src and dst buffers are freed by ourselves below
	 */
	if (params->polled)
		flags = DMA_CTRL_ACK;
	else
		flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT;

	ktime = ktime_get();
	while (!(kthread_should_stop() ||
	       (params->iterations && total_tests >= params->iterations))) {
		struct dma_async_tx_descriptor *tx = NULL;
		struct dmaengine_unmap_data *um;
		dma_addr_t *dsts;
		unsigned int len;

		total_tests++;

		if (params->transfer_size) {
			if (params->transfer_size >= buf_size) {
				pr_err("%u-byte transfer size must be lower than %u-buffer size\n",
				       params->transfer_size, buf_size);
				break;
			}
			len = params->transfer_size;
		} else if (params->norandom) {
			len = buf_size;
		} else {
			len = dmatest_random() % buf_size + 1;
		}

		/* Do not alter transfer size explicitly defined by user */
		if (!params->transfer_size) {
			len = (len >> align) << align;
			if (!len)
				len = 1 << align;
		}
		total_len += len;

		if (params->norandom) {
			src->off = 0;
			dst->off = 0;
		} else {
			src->off = dmatest_random() % (buf_size - len + 1);
			dst->off = dmatest_random() % (buf_size - len + 1);

			src->off = (src->off >> align) << align;
			dst->off = (dst->off >> align) << align;
		}

		if (!params->noverify) {
			start = ktime_get();
			dmatest_init_srcs(src->aligned, src->off, len,
					  buf_size, is_memset);
			dmatest_init_dsts(dst->aligned, dst->off, len,
					  buf_size, is_memset);

			diff = ktime_sub(ktime_get(), start);
			filltime = ktime_add(filltime, diff);
		}

		um = dmaengine_get_unmap_data(dma_dev, src->cnt + dst->cnt,
					      GFP_KERNEL);
		if (!um) {
			failed_tests++;
			result("unmap data NULL", total_tests,
			       src->off, dst->off, len, ret);
			continue;
		}

		um->len = buf_size;
		for (i = 0; i < src->cnt; i++) {
			void *buf = src->aligned[i];
			struct page *pg = virt_to_page(buf);
			unsigned long pg_off = offset_in_page(buf);

			um->addr[i] = dma_map_page(dma_dev, pg, pg_off,
						   um->len, DMA_TO_DEVICE);
			srcs[i] = um->addr[i] + src->off;
			ret = dma_mapping_error(dma_dev, um->addr[i]);
			if (ret) {
				result("src mapping error", total_tests,
				       src->off, dst->off, len, ret);
				goto error_unmap_continue;
			}
			um->to_cnt++;
		}
		/* map with DMA_BIDIRECTIONAL to force writeback/invalidate */
		dsts = &um->addr[src->cnt];
		for (i = 0; i < dst->cnt; i++) {
			void *buf = dst->aligned[i];
			struct page *pg = virt_to_page(buf);
			unsigned long pg_off = offset_in_page(buf);

			dsts[i] = dma_map_page(dma_dev, pg, pg_off, um->len,
					       DMA_BIDIRECTIONAL);
			ret = dma_mapping_error(dma_dev, dsts[i]);
			if (ret) {
				result("dst mapping error", total_tests,
				       src->off, dst->off, len, ret);
				goto error_unmap_continue;
			}
			um->bidi_cnt++;
		}

		if (thread->type == DMA_MEMCPY)
			tx = dev->device_prep_dma_memcpy(chan,
							 dsts[0] + dst->off,
							 srcs[0], len, flags);
		else if (thread->type == DMA_MEMSET)
			tx = dev->device_prep_dma_memset(chan,
						dsts[0] + dst->off,
						*(src->aligned[0] + src->off),
						len, flags);
		else if (thread->type == DMA_XOR)
			tx = dev->device_prep_dma_xor(chan,
						      dsts[0] + dst->off,
						      srcs, src->cnt,
						      len, flags);
		else if (thread->type == DMA_PQ) {
			for (i = 0; i < dst->cnt; i++)
				dma_pq[i] = dsts[i] + dst->off;
			tx = dev->device_prep_dma_pq(chan, dma_pq, srcs,
						     src->cnt, pq_coefs,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						     len, flags);
		}

		if (!tx) {
<<<<<<< HEAD
			for (i = 0; i < src_cnt; i++)
				dma_unmap_single(dev->dev, dma_srcs[i], len,
						 DMA_TO_DEVICE);
			for (i = 0; i < dst_cnt; i++)
				dma_unmap_single(dev->dev, dma_dsts[i],
						 test_buf_size,
						 DMA_BIDIRECTIONAL);
			pr_warning("%s: #%u: prep error with src_off=0x%x "
					"dst_off=0x%x len=0x%x\n",
					thread_name, total_tests - 1,
					src_off, dst_off, len);
			msleep(100);
			failed_tests++;
			continue;
		}

		done.done = false;
		tx->callback = dmatest_callback;
		tx->callback_param = &done;
		cookie = tx->tx_submit(tx);

		if (dma_submit_error(cookie)) {
			pr_warning("%s: #%u: submit error %d with src_off=0x%x "
					"dst_off=0x%x len=0x%x\n",
					thread_name, total_tests - 1, cookie,
					src_off, dst_off, len);
			msleep(100);
			failed_tests++;
			continue;
		}
		dma_async_issue_pending(chan);

		wait_event_freezable_timeout(done_wait, done.done,
					     msecs_to_jiffies(timeout));

		status = dma_async_is_tx_complete(chan, cookie, NULL, NULL);

		if (!done.done) {
			/*
			 * We're leaving the timed out dma operation with
			 * dangling pointer to done_wait.  To make this
			 * correct, we'll need to allocate wait_done for
			 * each test iteration and perform "who's gonna
			 * free it this time?" dancing.  For now, just
			 * leave it dangling.
			 */
			pr_warning("%s: #%u: test timed out\n",
				   thread_name, total_tests - 1);
			failed_tests++;
			continue;
		} else if (status != DMA_SUCCESS) {
			pr_warning("%s: #%u: got completion callback,"
				   " but status is \'%s\'\n",
				   thread_name, total_tests - 1,
				   status == DMA_ERROR ? "error" : "in progress");
			failed_tests++;
			continue;
		}

		/* Unmap by myself (see DMA_COMPL_SKIP_DEST_UNMAP above) */
		for (i = 0; i < dst_cnt; i++)
			dma_unmap_single(dev->dev, dma_dsts[i], test_buf_size,
					 DMA_BIDIRECTIONAL);

		error_count = 0;

		pr_debug("%s: verifying source buffer...\n", thread_name);
		error_count += dmatest_verify(thread->srcs, 0, src_off,
				0, PATTERN_SRC, true);
		error_count += dmatest_verify(thread->srcs, src_off,
				src_off + len, src_off,
				PATTERN_SRC | PATTERN_COPY, true);
		error_count += dmatest_verify(thread->srcs, src_off + len,
				test_buf_size, src_off + len,
				PATTERN_SRC, true);

		pr_debug("%s: verifying dest buffer...\n",
				thread->task->comm);
		error_count += dmatest_verify(thread->dsts, 0, dst_off,
				0, PATTERN_DST, false);
		error_count += dmatest_verify(thread->dsts, dst_off,
				dst_off + len, src_off,
				PATTERN_SRC | PATTERN_COPY, false);
		error_count += dmatest_verify(thread->dsts, dst_off + len,
				test_buf_size, dst_off + len,
				PATTERN_DST, false);

		if (error_count) {
			pr_warning("%s: #%u: %u errors with "
				"src_off=0x%x dst_off=0x%x len=0x%x\n",
				thread_name, total_tests - 1, error_count,
				src_off, dst_off, len);
			failed_tests++;
		} else {
			pr_debug("%s: #%u: No errors with "
				"src_off=0x%x dst_off=0x%x len=0x%x\n",
				thread_name, total_tests - 1,
				src_off, dst_off, len);
		}
	}

	ret = 0;
	for (i = 0; thread->dsts[i]; i++)
		kfree(thread->dsts[i]);
err_dstbuf:
	kfree(thread->dsts);
err_dsts:
	for (i = 0; thread->srcs[i]; i++)
		kfree(thread->srcs[i]);
err_srcbuf:
	kfree(thread->srcs);
err_srcs:
	pr_notice("%s: terminating after %u tests, %u failures (status %d)\n",
			thread_name, total_tests, failed_tests, ret);

	/* terminate all transfers on specified channels */
	chan->device->device_control(chan, DMA_TERMINATE_ALL, 0);
	if (iterations > 0)
		while (!kthread_should_stop()) {
			DECLARE_WAIT_QUEUE_HEAD_ONSTACK(wait_dmatest_exit);
			interruptible_sleep_on(&wait_dmatest_exit);
		}
=======
			result("prep error", total_tests, src->off,
			       dst->off, len, ret);
			msleep(100);
			goto error_unmap_continue;
		}

		done->done = false;
		if (!params->polled) {
			tx->callback = dmatest_callback;
			tx->callback_param = done;
		}
		cookie = tx->tx_submit(tx);

		if (dma_submit_error(cookie)) {
			result("submit error", total_tests, src->off,
			       dst->off, len, ret);
			msleep(100);
			goto error_unmap_continue;
		}

		if (params->polled) {
			status = dma_sync_wait(chan, cookie);
			dmaengine_terminate_sync(chan);
			if (status == DMA_COMPLETE)
				done->done = true;
		} else {
			dma_async_issue_pending(chan);

			wait_event_freezable_timeout(thread->done_wait,
					done->done,
					msecs_to_jiffies(params->timeout));

			status = dma_async_is_tx_complete(chan, cookie, NULL,
							  NULL);
		}

		if (!done->done) {
			result("test timed out", total_tests, src->off, dst->off,
			       len, 0);
			goto error_unmap_continue;
		} else if (status != DMA_COMPLETE &&
			   !(dma_has_cap(DMA_COMPLETION_NO_ORDER,
					 dev->cap_mask) &&
			     status == DMA_OUT_OF_ORDER)) {
			result(status == DMA_ERROR ?
			       "completion error status" :
			       "completion busy status", total_tests, src->off,
			       dst->off, len, ret);
			goto error_unmap_continue;
		}

		dmaengine_unmap_put(um);

		if (params->noverify) {
			verbose_result("test passed", total_tests, src->off,
				       dst->off, len, 0);
			continue;
		}

		start = ktime_get();
		pr_debug("%s: verifying source buffer...\n", current->comm);
		error_count = dmatest_verify(src->aligned, 0, src->off,
				0, PATTERN_SRC, true, is_memset);
		error_count += dmatest_verify(src->aligned, src->off,
				src->off + len, src->off,
				PATTERN_SRC | PATTERN_COPY, true, is_memset);
		error_count += dmatest_verify(src->aligned, src->off + len,
				buf_size, src->off + len,
				PATTERN_SRC, true, is_memset);

		pr_debug("%s: verifying dest buffer...\n", current->comm);
		error_count += dmatest_verify(dst->aligned, 0, dst->off,
				0, PATTERN_DST, false, is_memset);

		error_count += dmatest_verify(dst->aligned, dst->off,
				dst->off + len, src->off,
				PATTERN_SRC | PATTERN_COPY, false, is_memset);

		error_count += dmatest_verify(dst->aligned, dst->off + len,
				buf_size, dst->off + len,
				PATTERN_DST, false, is_memset);

		diff = ktime_sub(ktime_get(), start);
		comparetime = ktime_add(comparetime, diff);

		if (error_count) {
			result("data error", total_tests, src->off, dst->off,
			       len, error_count);
			failed_tests++;
		} else {
			verbose_result("test passed", total_tests, src->off,
				       dst->off, len, 0);
		}

		continue;

error_unmap_continue:
		dmaengine_unmap_put(um);
		failed_tests++;
	}
	ktime = ktime_sub(ktime_get(), ktime);
	ktime = ktime_sub(ktime, comparetime);
	ktime = ktime_sub(ktime, filltime);
	runtime = ktime_to_us(ktime);

	ret = 0;
	kfree(dma_pq);
err_srcs_array:
	kfree(srcs);
err_dst:
	dmatest_free_test_data(dst);
err_src:
	dmatest_free_test_data(src);
err_free_coefs:
	kfree(pq_coefs);
err_thread_type:
	iops = dmatest_persec(runtime, total_tests);
	pr_info("%s: summary %u tests, %u failures %llu.%02llu iops %llu KB/s (%d)\n",
		current->comm, total_tests, failed_tests,
		FIXPT_TO_INT(iops), FIXPT_GET_FRAC(iops),
		dmatest_KBs(runtime, total_len), ret);

	/* terminate all transfers on specified channels */
	if (ret || failed_tests)
		dmaengine_terminate_sync(chan);

	thread->done = true;
	wake_up(&thread_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static void dmatest_cleanup_channel(struct dmatest_chan *dtc)
{
	struct dmatest_thread	*thread;
	struct dmatest_thread	*_thread;
	int			ret;

	list_for_each_entry_safe(thread, _thread, &dtc->threads, node) {
		ret = kthread_stop(thread->task);
<<<<<<< HEAD
		pr_debug("dmatest: thread %s exited with status %d\n",
				thread->task->comm, ret);
		list_del(&thread->node);
=======
		pr_debug("thread %s exited with status %d\n",
			 thread->task->comm, ret);
		list_del(&thread->node);
		put_task_struct(thread->task);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(thread);
	}

	/* terminate all transfers on specified channels */
<<<<<<< HEAD
	dtc->chan->device->device_control(dtc->chan, DMA_TERMINATE_ALL, 0);
=======
	dmaengine_terminate_sync(dtc->chan);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree(dtc);
}

<<<<<<< HEAD
static int dmatest_add_threads(struct dmatest_chan *dtc, enum dma_transaction_type type)
{
=======
static int dmatest_add_threads(struct dmatest_info *info,
		struct dmatest_chan *dtc, enum dma_transaction_type type)
{
	struct dmatest_params *params = &info->params;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct dmatest_thread *thread;
	struct dma_chan *chan = dtc->chan;
	char *op;
	unsigned int i;

	if (type == DMA_MEMCPY)
		op = "copy";
<<<<<<< HEAD
=======
	else if (type == DMA_MEMSET)
		op = "set";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else if (type == DMA_XOR)
		op = "xor";
	else if (type == DMA_PQ)
		op = "pq";
	else
		return -EINVAL;

<<<<<<< HEAD
	for (i = 0; i < threads_per_chan; i++) {
		thread = kzalloc(sizeof(struct dmatest_thread), GFP_KERNEL);
		if (!thread) {
			pr_warning("dmatest: No memory for %s-%s%u\n",
				   dma_chan_name(chan), op, i);

			break;
		}
		thread->chan = dtc->chan;
		thread->type = type;
		smp_wmb();
		thread->task = kthread_run(dmatest_func, thread, "%s-%s%u",
				dma_chan_name(chan), op, i);
		if (IS_ERR(thread->task)) {
			pr_warning("dmatest: Failed to run thread %s-%s%u\n",
					dma_chan_name(chan), op, i);
=======
	for (i = 0; i < params->threads_per_chan; i++) {
		thread = kzalloc(sizeof(struct dmatest_thread), GFP_KERNEL);
		if (!thread) {
			pr_warn("No memory for %s-%s%u\n",
				dma_chan_name(chan), op, i);
			break;
		}
		thread->info = info;
		thread->chan = dtc->chan;
		thread->type = type;
		thread->test_done.wait = &thread->done_wait;
		init_waitqueue_head(&thread->done_wait);
		smp_wmb();
		thread->task = kthread_create(dmatest_func, thread, "%s-%s%u",
				dma_chan_name(chan), op, i);
		if (IS_ERR(thread->task)) {
			pr_warn("Failed to create thread %s-%s%u\n",
				dma_chan_name(chan), op, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			kfree(thread);
			break;
		}

		/* srcbuf and dstbuf are allocated by the thread itself */
<<<<<<< HEAD

		list_add_tail(&thread->node, &dtc->threads);
=======
		get_task_struct(thread->task);
		list_add_tail(&thread->node, &dtc->threads);
		thread->pending = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return i;
}

<<<<<<< HEAD
static int dmatest_add_channel(struct dma_chan *chan)
=======
static int dmatest_add_channel(struct dmatest_info *info,
		struct dma_chan *chan)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dmatest_chan	*dtc;
	struct dma_device	*dma_dev = chan->device;
	unsigned int		thread_count = 0;
	int cnt;

	dtc = kmalloc(sizeof(struct dmatest_chan), GFP_KERNEL);
	if (!dtc) {
<<<<<<< HEAD
		pr_warning("dmatest: No memory for %s\n", dma_chan_name(chan));
=======
		pr_warn("No memory for %s\n", dma_chan_name(chan));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;
	}

	dtc->chan = chan;
	INIT_LIST_HEAD(&dtc->threads);

<<<<<<< HEAD
	if (dma_has_cap(DMA_MEMCPY, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(dtc, DMA_MEMCPY);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_XOR, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(dtc, DMA_XOR);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_PQ, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(dtc, DMA_PQ);
		thread_count += cnt > 0 ? cnt : 0;
	}

	pr_info("dmatest: Started %u threads using %s\n",
		thread_count, dma_chan_name(chan));

	list_add_tail(&dtc->node, &dmatest_channels);
	nr_channels++;
=======
	if (dma_has_cap(DMA_COMPLETION_NO_ORDER, dma_dev->cap_mask) &&
	    info->params.polled) {
		info->params.polled = false;
		pr_warn("DMA_COMPLETION_NO_ORDER, polled disabled\n");
	}

	if (dma_has_cap(DMA_MEMCPY, dma_dev->cap_mask)) {
		if (dmatest == 0) {
			cnt = dmatest_add_threads(info, dtc, DMA_MEMCPY);
			thread_count += cnt > 0 ? cnt : 0;
		}
	}

	if (dma_has_cap(DMA_MEMSET, dma_dev->cap_mask)) {
		if (dmatest == 1) {
			cnt = dmatest_add_threads(info, dtc, DMA_MEMSET);
			thread_count += cnt > 0 ? cnt : 0;
		}
	}

	if (dma_has_cap(DMA_XOR, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(info, dtc, DMA_XOR);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_PQ, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(info, dtc, DMA_PQ);
		thread_count += cnt > 0 ? cnt : 0;
	}

	pr_info("Added %u threads using %s\n",
		thread_count, dma_chan_name(chan));

	list_add_tail(&dtc->node, &info->channels);
	info->nr_channels++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static bool filter(struct dma_chan *chan, void *param)
{
<<<<<<< HEAD
	if (!dmatest_match_channel(chan) || !dmatest_match_device(chan->device))
		return false;
	else
		return true;
}

static int __init dmatest_init(void)
{
	dma_cap_mask_t mask;
	struct dma_chan *chan;
	int err = 0;

	dma_cap_zero(mask);
	dma_cap_set(DMA_MEMCPY, mask);
	for (;;) {
		chan = dma_request_channel(mask, filter, NULL);
		if (chan) {
			err = dmatest_add_channel(chan);
			if (err) {
=======
	return dmatest_match_channel(param, chan) && dmatest_match_device(param, chan->device);
}

static void request_channels(struct dmatest_info *info,
			     enum dma_transaction_type type)
{
	dma_cap_mask_t mask;

	dma_cap_zero(mask);
	dma_cap_set(type, mask);
	for (;;) {
		struct dmatest_params *params = &info->params;
		struct dma_chan *chan;

		chan = dma_request_channel(mask, filter, params);
		if (chan) {
			if (dmatest_add_channel(info, chan)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				dma_release_channel(chan);
				break; /* add_channel failed, punt */
			}
		} else
			break; /* no more channels available */
<<<<<<< HEAD
		if (max_channels && nr_channels >= max_channels)
			break; /* we have all we need */
	}

	return err;
=======
		if (params->max_channels &&
		    info->nr_channels >= params->max_channels)
			break; /* we have all we need */
	}
}

static void add_threaded_test(struct dmatest_info *info)
{
	struct dmatest_params *params = &info->params;

	/* Copy test parameters */
	params->nobounce = nobounce;
	params->buf_size = test_buf_size;
	strscpy(params->channel, strim(test_channel), sizeof(params->channel));
	strscpy(params->device, strim(test_device), sizeof(params->device));
	params->threads_per_chan = threads_per_chan;
	params->max_channels = max_channels;
	params->iterations = iterations;
	params->xor_sources = xor_sources;
	params->pq_sources = pq_sources;
	params->timeout = timeout;
	params->noverify = noverify;
	params->norandom = norandom;
	params->alignment = alignment;
	params->transfer_size = transfer_size;
	params->polled = polled;

	request_channels(info, DMA_MEMCPY);
	request_channels(info, DMA_MEMSET);
	request_channels(info, DMA_XOR);
	request_channels(info, DMA_PQ);
}

static void run_pending_tests(struct dmatest_info *info)
{
	struct dmatest_chan *dtc;
	unsigned int thread_count = 0;

	list_for_each_entry(dtc, &info->channels, node) {
		struct dmatest_thread *thread;

		thread_count = 0;
		list_for_each_entry(thread, &dtc->threads, node) {
			wake_up_process(thread->task);
			thread_count++;
		}
		pr_info("Started %u threads using %s\n",
			thread_count, dma_chan_name(dtc->chan));
	}
}

static void stop_threaded_test(struct dmatest_info *info)
{
	struct dmatest_chan *dtc, *_dtc;
	struct dma_chan *chan;

	list_for_each_entry_safe(dtc, _dtc, &info->channels, node) {
		list_del(&dtc->node);
		chan = dtc->chan;
		dmatest_cleanup_channel(dtc);
		pr_debug("dropped channel %s\n", dma_chan_name(chan));
		dma_release_channel(chan);
	}

	info->nr_channels = 0;
}

static void start_threaded_tests(struct dmatest_info *info)
{
	/* we might be called early to set run=, defer running until all
	 * parameters have been evaluated
	 */
	if (!info->did_init)
		return;

	run_pending_tests(info);
}

static int dmatest_run_get(char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;

	mutex_lock(&info->lock);
	if (is_threaded_test_run(info)) {
		dmatest_run = true;
	} else {
		if (!is_threaded_test_pending(info))
			stop_threaded_test(info);
		dmatest_run = false;
	}
	mutex_unlock(&info->lock);

	return param_get_bool(val, kp);
}

static int dmatest_run_set(const char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;
	int ret;

	mutex_lock(&info->lock);
	ret = param_set_bool(val, kp);
	if (ret) {
		mutex_unlock(&info->lock);
		return ret;
	} else if (dmatest_run) {
		if (!is_threaded_test_pending(info)) {
			/*
			 * We have nothing to run. This can be due to:
			 */
			ret = info->last_error;
			if (ret) {
				/* 1) Misconfiguration */
				pr_err("Channel misconfigured, can't continue\n");
				mutex_unlock(&info->lock);
				return ret;
			} else {
				/* 2) We rely on defaults */
				pr_info("No channels configured, continue with any\n");
				if (!is_threaded_test_run(info))
					stop_threaded_test(info);
				add_threaded_test(info);
			}
		}
		start_threaded_tests(info);
	} else {
		stop_threaded_test(info);
	}

	mutex_unlock(&info->lock);

	return ret;
}

static int dmatest_chan_set(const char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;
	struct dmatest_chan *dtc;
	char chan_reset_val[20];
	int ret;

	mutex_lock(&info->lock);
	ret = param_set_copystring(val, kp);
	if (ret) {
		mutex_unlock(&info->lock);
		return ret;
	}
	/*Clear any previously run threads */
	if (!is_threaded_test_run(info) && !is_threaded_test_pending(info))
		stop_threaded_test(info);
	/* Reject channels that are already registered */
	if (is_threaded_test_pending(info)) {
		list_for_each_entry(dtc, &info->channels, node) {
			if (strcmp(dma_chan_name(dtc->chan),
				   strim(test_channel)) == 0) {
				dtc = list_last_entry(&info->channels,
						      struct dmatest_chan,
						      node);
				strscpy(chan_reset_val,
					dma_chan_name(dtc->chan),
					sizeof(chan_reset_val));
				ret = -EBUSY;
				goto add_chan_err;
			}
		}
	}

	add_threaded_test(info);

	/* Check if channel was added successfully */
	if (!list_empty(&info->channels)) {
		/*
		 * if new channel was not successfully added, revert the
		 * "test_channel" string to the name of the last successfully
		 * added channel. exception for when users issues empty string
		 * to channel parameter.
		 */
		dtc = list_last_entry(&info->channels, struct dmatest_chan, node);
		if ((strcmp(dma_chan_name(dtc->chan), strim(test_channel)) != 0)
		    && (strcmp("", strim(test_channel)) != 0)) {
			ret = -EINVAL;
			strscpy(chan_reset_val, dma_chan_name(dtc->chan),
				sizeof(chan_reset_val));
			goto add_chan_err;
		}

	} else {
		/* Clear test_channel if no channels were added successfully */
		strscpy(chan_reset_val, "", sizeof(chan_reset_val));
		ret = -EBUSY;
		goto add_chan_err;
	}

	info->last_error = ret;
	mutex_unlock(&info->lock);

	return ret;

add_chan_err:
	param_set_copystring(chan_reset_val, kp);
	info->last_error = ret;
	mutex_unlock(&info->lock);

	return ret;
}

static int dmatest_chan_get(char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;

	mutex_lock(&info->lock);
	if (!is_threaded_test_run(info) && !is_threaded_test_pending(info)) {
		stop_threaded_test(info);
		strscpy(test_channel, "", sizeof(test_channel));
	}
	mutex_unlock(&info->lock);

	return param_get_string(val, kp);
}

static int dmatest_test_list_get(char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;
	struct dmatest_chan *dtc;
	unsigned int thread_count = 0;

	list_for_each_entry(dtc, &info->channels, node) {
		struct dmatest_thread *thread;

		thread_count = 0;
		list_for_each_entry(thread, &dtc->threads, node) {
			thread_count++;
		}
		pr_info("%u threads using %s\n",
			thread_count, dma_chan_name(dtc->chan));
	}

	return 0;
}

static int __init dmatest_init(void)
{
	struct dmatest_info *info = &test_info;
	struct dmatest_params *params = &info->params;

	if (dmatest_run) {
		mutex_lock(&info->lock);
		add_threaded_test(info);
		run_pending_tests(info);
		mutex_unlock(&info->lock);
	}

	if (params->iterations && wait)
		wait_event(thread_wait, !is_threaded_test_run(info));

	/* module parameters are stable, inittime tests are started,
	 * let userspace take over 'run' control
	 */
	info->did_init = true;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
/* when compiled-in wait for drivers to load first */
late_initcall(dmatest_init);

static void __exit dmatest_exit(void)
{
<<<<<<< HEAD
	struct dmatest_chan *dtc, *_dtc;
	struct dma_chan *chan;

	list_for_each_entry_safe(dtc, _dtc, &dmatest_channels, node) {
		list_del(&dtc->node);
		chan = dtc->chan;
		dmatest_cleanup_channel(dtc);
		pr_debug("dmatest: dropped channel %s\n",
			 dma_chan_name(chan));
		dma_release_channel(chan);
	}
=======
	struct dmatest_info *info = &test_info;

	mutex_lock(&info->lock);
	stop_threaded_test(info);
	mutex_unlock(&info->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
module_exit(dmatest_exit);

MODULE_AUTHOR("Haavard Skinnemoen (Atmel)");
MODULE_LICENSE("GPL v2");
