<<<<<<< HEAD
#define _GNU_SOURCE
#include <getopt.h>
=======
// SPDX-License-Identifier: GPL-2.0
#define _GNU_SOURCE
#include <getopt.h>
#include <limits.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <string.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
<<<<<<< HEAD
=======
#include <stdbool.h>
#include <linux/virtio_types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/vhost.h>
#include <linux/virtio.h>
#include <linux/virtio_ring.h>
#include "../../drivers/vhost/test.h"

<<<<<<< HEAD
=======
#define RANDOM_BATCH -1

/* Unused */
void *__kmalloc_fake, *__kfree_ignore_start, *__kfree_ignore_end;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct vq_info {
	int kick;
	int call;
	int num;
	int idx;
	void *ring;
	/* copy used for control */
	struct vring vring;
	struct virtqueue *vq;
};

struct vdev_info {
	struct virtio_device vdev;
	int control;
	struct pollfd fds[1];
	struct vq_info vqs[1];
	int nvqs;
	void *buf;
	size_t buf_size;
	struct vhost_memory *mem;
};

<<<<<<< HEAD
void vq_notify(struct virtqueue *vq)
=======
static const struct vhost_vring_file no_backend = { .fd = -1 },
				     backend = { .fd = 1 };
static const struct vhost_vring_state null_state = {};

bool vq_notify(struct virtqueue *vq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vq_info *info = vq->priv;
	unsigned long long v = 1;
	int r;
	r = write(info->kick, &v, sizeof v);
	assert(r == sizeof v);
<<<<<<< HEAD
=======
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void vq_callback(struct virtqueue *vq)
{
}


void vhost_vq_setup(struct vdev_info *dev, struct vq_info *info)
{
	struct vhost_vring_state state = { .index = info->idx };
	struct vhost_vring_file file = { .index = info->idx };
<<<<<<< HEAD
	unsigned long long features = dev->vdev.features[0];
=======
	unsigned long long features = dev->vdev.features;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct vhost_vring_addr addr = {
		.index = info->idx,
		.desc_user_addr = (uint64_t)(unsigned long)info->vring.desc,
		.avail_user_addr = (uint64_t)(unsigned long)info->vring.avail,
		.used_user_addr = (uint64_t)(unsigned long)info->vring.used,
	};
	int r;
	r = ioctl(dev->control, VHOST_SET_FEATURES, &features);
	assert(r >= 0);
	state.num = info->vring.num;
	r = ioctl(dev->control, VHOST_SET_VRING_NUM, &state);
	assert(r >= 0);
	state.num = 0;
	r = ioctl(dev->control, VHOST_SET_VRING_BASE, &state);
	assert(r >= 0);
	r = ioctl(dev->control, VHOST_SET_VRING_ADDR, &addr);
	assert(r >= 0);
	file.fd = info->kick;
	r = ioctl(dev->control, VHOST_SET_VRING_KICK, &file);
	assert(r >= 0);
	file.fd = info->call;
	r = ioctl(dev->control, VHOST_SET_VRING_CALL, &file);
	assert(r >= 0);
}

<<<<<<< HEAD
=======
static void vq_reset(struct vq_info *info, int num, struct virtio_device *vdev)
{
	if (info->vq)
		vring_del_virtqueue(info->vq);

	memset(info->ring, 0, vring_size(num, 4096));
	vring_init(&info->vring, num, info->ring, 4096);
	info->vq = vring_new_virtqueue(info->idx, num, 4096, vdev, true, false,
				       info->ring, vq_notify, vq_callback, "test");
	assert(info->vq);
	info->vq->priv = info;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void vq_info_add(struct vdev_info *dev, int num)
{
	struct vq_info *info = &dev->vqs[dev->nvqs];
	int r;
	info->idx = dev->nvqs;
	info->kick = eventfd(0, EFD_NONBLOCK);
	info->call = eventfd(0, EFD_NONBLOCK);
	r = posix_memalign(&info->ring, 4096, vring_size(num, 4096));
	assert(r >= 0);
<<<<<<< HEAD
	memset(info->ring, 0, vring_size(num, 4096));
	vring_init(&info->vring, num, info->ring, 4096);
	info->vq = vring_new_virtqueue(info->vring.num, 4096, &dev->vdev,
				       true, info->ring,
				       vq_notify, vq_callback, "test");
	assert(info->vq);
	info->vq->priv = info;
=======
	vq_reset(info, num, &dev->vdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vhost_vq_setup(dev, info);
	dev->fds[info->idx].fd = info->call;
	dev->fds[info->idx].events = POLLIN;
	dev->nvqs++;
}

static void vdev_info_init(struct vdev_info* dev, unsigned long long features)
{
	int r;
	memset(dev, 0, sizeof *dev);
<<<<<<< HEAD
	dev->vdev.features[0] = features;
	dev->vdev.features[1] = features >> 32;
	dev->buf_size = 1024;
	dev->buf = malloc(dev->buf_size);
	assert(dev->buf);
        dev->control = open("/dev/vhost-test", O_RDWR);
=======
	dev->vdev.features = features;
	INIT_LIST_HEAD(&dev->vdev.vqs);
	spin_lock_init(&dev->vdev.vqs_list_lock);
	dev->buf_size = 1024;
	dev->buf = malloc(dev->buf_size);
	assert(dev->buf);
	dev->control = open("/dev/vhost-test", O_RDWR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	assert(dev->control >= 0);
	r = ioctl(dev->control, VHOST_SET_OWNER, NULL);
	assert(r >= 0);
	dev->mem = malloc(offsetof(struct vhost_memory, regions) +
			  sizeof dev->mem->regions[0]);
	assert(dev->mem);
	memset(dev->mem, 0, offsetof(struct vhost_memory, regions) +
                          sizeof dev->mem->regions[0]);
	dev->mem->nregions = 1;
	dev->mem->regions[0].guest_phys_addr = (long)dev->buf;
	dev->mem->regions[0].userspace_addr = (long)dev->buf;
	dev->mem->regions[0].memory_size = dev->buf_size;
	r = ioctl(dev->control, VHOST_SET_MEM_TABLE, dev->mem);
	assert(r >= 0);
}

/* TODO: this is pretty bad: we get a cache line bounce
 * for the wait queue on poll and another one on read,
 * plus the read which is there just to clear the
 * current state. */
static void wait_for_interrupt(struct vdev_info *dev)
{
	int i;
	unsigned long long val;
	poll(dev->fds, dev->nvqs, -1);
	for (i = 0; i < dev->nvqs; ++i)
		if (dev->fds[i].revents & POLLIN) {
			read(dev->fds[i].fd, &val, sizeof val);
		}
}

<<<<<<< HEAD
static void run_test(struct vdev_info *dev, struct vq_info *vq, int bufs)
{
	struct scatterlist sl;
	long started = 0, completed = 0;
	long completed_before;
	int r, test = 1;
	unsigned len;
	long long spurious = 0;
	r = ioctl(dev->control, VHOST_TEST_RUN, &test);
	assert(r >= 0);
	for (;;) {
		virtqueue_disable_cb(vq->vq);
		completed_before = completed;
		do {
			if (started < bufs) {
				sg_init_one(&sl, dev->buf, dev->buf_size);
				r = virtqueue_add_buf(vq->vq, &sl, 1, 0,
						      dev->buf + started,
						      GFP_ATOMIC);
				if (likely(r >= 0)) {
					++started;
					virtqueue_kick(vq->vq);
				}
			} else
				r = -1;

			/* Flush out completed bufs if any */
			if (virtqueue_get_buf(vq->vq, &len)) {
=======
static void run_test(struct vdev_info *dev, struct vq_info *vq,
		     bool delayed, int batch, int reset_n, int bufs)
{
	struct scatterlist sl;
	long started = 0, completed = 0, next_reset = reset_n;
	long completed_before, started_before;
	int r, test = 1;
	unsigned int len;
	long long spurious = 0;
	const bool random_batch = batch == RANDOM_BATCH;

	r = ioctl(dev->control, VHOST_TEST_RUN, &test);
	assert(r >= 0);
	if (!reset_n) {
		next_reset = INT_MAX;
	}

	for (;;) {
		virtqueue_disable_cb(vq->vq);
		completed_before = completed;
		started_before = started;
		do {
			const bool reset = completed > next_reset;
			if (random_batch)
				batch = (random() % vq->vring.num) + 1;

			while (started < bufs &&
			       (started - completed) < batch) {
				sg_init_one(&sl, dev->buf, dev->buf_size);
				r = virtqueue_add_outbuf(vq->vq, &sl, 1,
							 dev->buf + started,
							 GFP_ATOMIC);
				if (unlikely(r != 0)) {
					if (r == -ENOSPC &&
					    started > started_before)
						r = 0;
					else
						r = -1;
					break;
				}

				++started;

				if (unlikely(!virtqueue_kick(vq->vq))) {
					r = -1;
					break;
				}
			}

			if (started >= bufs)
				r = -1;

			if (reset) {
				r = ioctl(dev->control, VHOST_TEST_SET_BACKEND,
					  &no_backend);
				assert(!r);
			}

			/* Flush out completed bufs if any */
			while (virtqueue_get_buf(vq->vq, &len)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				++completed;
				r = 0;
			}

<<<<<<< HEAD
		} while (r >= 0);
		if (completed == completed_before)
=======
			if (reset) {
				struct vhost_vring_state s = { .index = 0 };

				vq_reset(vq, vq->vring.num, &dev->vdev);

				r = ioctl(dev->control, VHOST_GET_VRING_BASE,
					  &s);
				assert(!r);

				s.num = 0;
				r = ioctl(dev->control, VHOST_SET_VRING_BASE,
					  &null_state);
				assert(!r);

				r = ioctl(dev->control, VHOST_TEST_SET_BACKEND,
					  &backend);
				assert(!r);

				started = completed;
				while (completed > next_reset)
					next_reset += completed;
			}
		} while (r == 0);
		if (completed == completed_before && started == started_before)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			++spurious;
		assert(completed <= bufs);
		assert(started <= bufs);
		if (completed == bufs)
			break;
<<<<<<< HEAD
		if (virtqueue_enable_cb(vq->vq)) {
			wait_for_interrupt(dev);
=======
		if (delayed) {
			if (virtqueue_enable_cb_delayed(vq->vq))
				wait_for_interrupt(dev);
		} else {
			if (virtqueue_enable_cb(vq->vq))
				wait_for_interrupt(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	test = 0;
	r = ioctl(dev->control, VHOST_TEST_RUN, &test);
	assert(r >= 0);
<<<<<<< HEAD
	fprintf(stderr, "spurious wakeus: 0x%llx\n", spurious);
=======
	fprintf(stderr,
		"spurious wakeups: 0x%llx started=0x%lx completed=0x%lx\n",
		spurious, started, completed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

const char optstring[] = "h";
const struct option longopts[] = {
	{
		.name = "help",
		.val = 'h',
	},
	{
		.name = "event-idx",
		.val = 'E',
	},
	{
		.name = "no-event-idx",
		.val = 'e',
	},
	{
		.name = "indirect",
		.val = 'I',
	},
	{
		.name = "no-indirect",
		.val = 'i',
	},
	{
<<<<<<< HEAD
	}
};

static void help()
=======
		.name = "virtio-1",
		.val = '1',
	},
	{
		.name = "no-virtio-1",
		.val = '0',
	},
	{
		.name = "delayed-interrupt",
		.val = 'D',
	},
	{
		.name = "no-delayed-interrupt",
		.val = 'd',
	},
	{
		.name = "batch",
		.val = 'b',
		.has_arg = required_argument,
	},
	{
		.name = "reset",
		.val = 'r',
		.has_arg = optional_argument,
	},
	{
	}
};

static void help(int status)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	fprintf(stderr, "Usage: virtio_test [--help]"
		" [--no-indirect]"
		" [--no-event-idx]"
<<<<<<< HEAD
		"\n");
=======
		" [--no-virtio-1]"
		" [--delayed-interrupt]"
		" [--batch=random/N]"
		" [--reset=N]"
		"\n");

	exit(status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int main(int argc, char **argv)
{
	struct vdev_info dev;
	unsigned long long features = (1ULL << VIRTIO_RING_F_INDIRECT_DESC) |
<<<<<<< HEAD
		(1ULL << VIRTIO_RING_F_EVENT_IDX);
	int o;
=======
		(1ULL << VIRTIO_RING_F_EVENT_IDX) | (1ULL << VIRTIO_F_VERSION_1);
	long batch = 1, reset = 0;
	int o;
	bool delayed = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (;;) {
		o = getopt_long(argc, argv, optstring, longopts, NULL);
		switch (o) {
		case -1:
			goto done;
		case '?':
<<<<<<< HEAD
			help();
			exit(2);
=======
			help(2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case 'e':
			features &= ~(1ULL << VIRTIO_RING_F_EVENT_IDX);
			break;
		case 'h':
<<<<<<< HEAD
			help();
			goto done;
		case 'i':
			features &= ~(1ULL << VIRTIO_RING_F_INDIRECT_DESC);
			break;
=======
			help(0);
		case 'i':
			features &= ~(1ULL << VIRTIO_RING_F_INDIRECT_DESC);
			break;
		case '0':
			features &= ~(1ULL << VIRTIO_F_VERSION_1);
			break;
		case 'D':
			delayed = true;
			break;
		case 'b':
			if (0 == strcmp(optarg, "random")) {
				batch = RANDOM_BATCH;
			} else {
				batch = strtol(optarg, NULL, 10);
				assert(batch > 0);
				assert(batch < (long)INT_MAX + 1);
			}
			break;
		case 'r':
			if (!optarg) {
				reset = 1;
			} else {
				reset = strtol(optarg, NULL, 10);
				assert(reset > 0);
				assert(reset < (long)INT_MAX + 1);
			}
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			assert(0);
			break;
		}
	}

done:
	vdev_info_init(&dev, features);
	vq_info_add(&dev, 256);
<<<<<<< HEAD
	run_test(&dev, &dev.vqs[0], 0x100000);
=======
	run_test(&dev, &dev.vqs[0], delayed, batch, reset, 0x100000);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
