<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Virtio-based remote processor messaging bus
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 * Copyright (C) 2011 Google, Inc.
 *
 * Ohad Ben-Cohen <ohad@wizery.com>
 * Brian Swetland <swetland@google.com>
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/virtio.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_config.h>
#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/idr.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/rpmsg.h>
#include <linux/mutex.h>
=======
#include <linux/dma-mapping.h>
#include <linux/idr.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/rpmsg.h>
#include <linux/rpmsg/byteorder.h>
#include <linux/rpmsg/ns.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/virtio.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_config.h>
#include <linux/wait.h>

#include "rpmsg_internal.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * struct virtproc_info - virtual remote processor state
 * @vdev:	the virtio device
 * @rvq:	rx virtqueue
 * @svq:	tx virtqueue
 * @rbufs:	kernel address of rx buffers
 * @sbufs:	kernel address of tx buffers
<<<<<<< HEAD
=======
 * @num_bufs:	total number of buffers for rx and tx
 * @buf_size:   size of one rx or tx buffer
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @last_sbuf:	index of last tx buffer used
 * @bufs_dma:	dma base addr of the buffers
 * @tx_lock:	protects svq, sbufs and sleepers, to allow concurrent senders.
 *		sending a message might require waking up a dozing remote
 *		processor, which involves sleeping, hence the mutex.
 * @endpoints:	idr of local endpoints, allows fast retrieval
 * @endpoints_lock: lock of the endpoints set
 * @sendq:	wait queue of sending contexts waiting for a tx buffers
 * @sleepers:	number of senders that are waiting for a tx buffer
<<<<<<< HEAD
 * @ns_ept:	the bus's name service endpoint
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This structure stores the rpmsg state of a given virtio remote processor
 * device (there might be several virtio proc devices for each physical
 * remote processor).
 */
struct virtproc_info {
	struct virtio_device *vdev;
	struct virtqueue *rvq, *svq;
	void *rbufs, *sbufs;
<<<<<<< HEAD
=======
	unsigned int num_bufs;
	unsigned int buf_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int last_sbuf;
	dma_addr_t bufs_dma;
	struct mutex tx_lock;
	struct idr endpoints;
	struct mutex endpoints_lock;
	wait_queue_head_t sendq;
	atomic_t sleepers;
<<<<<<< HEAD
	struct rpmsg_endpoint *ns_ept;
};

/**
 * struct rpmsg_channel_info - internal channel info representation
 * @name: name of service
 * @src: local address
 * @dst: destination address
 */
struct rpmsg_channel_info {
	char name[RPMSG_NAME_SIZE];
	u32 src;
	u32 dst;
};

#define to_rpmsg_channel(d) container_of(d, struct rpmsg_channel, dev)
#define to_rpmsg_driver(d) container_of(d, struct rpmsg_driver, drv)

/*
 * We're allocating 512 buffers of 512 bytes for communications, and then
 * using the first 256 buffers for RX, and the last 256 buffers for TX.
=======
};

/* The feature bitmap for virtio rpmsg */
#define VIRTIO_RPMSG_F_NS	0 /* RP supports name service notifications */

/**
 * struct rpmsg_hdr - common header for all rpmsg messages
 * @src: source address
 * @dst: destination address
 * @reserved: reserved for future use
 * @len: length of payload (in bytes)
 * @flags: message flags
 * @data: @len bytes of message payload data
 *
 * Every message sent(/received) on the rpmsg bus begins with this header.
 */
struct rpmsg_hdr {
	__rpmsg32 src;
	__rpmsg32 dst;
	__rpmsg32 reserved;
	__rpmsg16 len;
	__rpmsg16 flags;
	u8 data[];
} __packed;


/**
 * struct virtio_rpmsg_channel - rpmsg channel descriptor
 * @rpdev: the rpmsg channel device
 * @vrp: the virtio remote processor device this channel belongs to
 *
 * This structure stores the channel that links the rpmsg device to the virtio
 * remote processor device.
 */
struct virtio_rpmsg_channel {
	struct rpmsg_device rpdev;

	struct virtproc_info *vrp;
};

#define to_virtio_rpmsg_channel(_rpdev) \
	container_of(_rpdev, struct virtio_rpmsg_channel, rpdev)

/*
 * We're allocating buffers of 512 bytes each for communications. The
 * number of buffers will be computed from the number of buffers supported
 * by the vring, upto a maximum of 512 buffers (256 in each direction).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Each buffer will have 16 bytes for the msg header and 496 bytes for
 * the payload.
 *
<<<<<<< HEAD
 * This will require a total space of 256KB for the buffers.
=======
 * This will utilize a maximum total space of 256KB for the buffers.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * We might also want to add support for user-provided buffers in time.
 * This will allow bigger buffer size flexibility, and can also be used
 * to achieve zero-copy messaging.
 *
 * Note that these numbers are purely a decision of this driver - we
 * can change this without changing anything in the firmware of the remote
 * processor.
 */
<<<<<<< HEAD
#define RPMSG_NUM_BUFS		(512)
#define RPMSG_BUF_SIZE		(512)
#define RPMSG_TOTAL_BUF_SPACE	(RPMSG_NUM_BUFS * RPMSG_BUF_SIZE)
=======
#define MAX_RPMSG_NUM_BUFS	(512)
#define MAX_RPMSG_BUF_SIZE	(512)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Local addresses are dynamically allocated on-demand.
 * We do not dynamically assign addresses from the low 1024 range,
 * in order to reserve that address range for predefined services.
 */
#define RPMSG_RESERVED_ADDRESSES	(1024)

<<<<<<< HEAD
/* Address 53 is reserved for advertising remote services */
#define RPMSG_NS_ADDR			(53)

/* sysfs show configuration fields */
#define rpmsg_show_attr(field, path, format_string)			\
static ssize_t								\
field##_show(struct device *dev,					\
			struct device_attribute *attr, char *buf)	\
{									\
	struct rpmsg_channel *rpdev = to_rpmsg_channel(dev);		\
									\
	return sprintf(buf, format_string, rpdev->path);		\
}

/* for more info, see Documentation/ABI/testing/sysfs-bus-rpmsg */
rpmsg_show_attr(name, id.name, "%s\n");
rpmsg_show_attr(src, src, "0x%x\n");
rpmsg_show_attr(dst, dst, "0x%x\n");
rpmsg_show_attr(announce, announce ? "true" : "false", "%s\n");

/*
 * Unique (and free running) index for rpmsg devices.
 *
 * Yeah, we're not recycling those numbers (yet?). will be easy
 * to change if/when we want to.
 */
static unsigned int rpmsg_dev_index;

static ssize_t modalias_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
	struct rpmsg_channel *rpdev = to_rpmsg_channel(dev);

	return sprintf(buf, RPMSG_DEVICE_MODALIAS_FMT "\n", rpdev->id.name);
}

static struct device_attribute rpmsg_dev_attrs[] = {
	__ATTR_RO(name),
	__ATTR_RO(modalias),
	__ATTR_RO(dst),
	__ATTR_RO(src),
	__ATTR_RO(announce),
	__ATTR_NULL
};

/* rpmsg devices and drivers are matched using the service name */
static inline int rpmsg_id_match(const struct rpmsg_channel *rpdev,
				  const struct rpmsg_device_id *id)
{
	return strncmp(id->name, rpdev->id.name, RPMSG_NAME_SIZE) == 0;
}

/* match rpmsg channel and rpmsg driver */
static int rpmsg_dev_match(struct device *dev, struct device_driver *drv)
{
	struct rpmsg_channel *rpdev = to_rpmsg_channel(dev);
	struct rpmsg_driver *rpdrv = to_rpmsg_driver(drv);
	const struct rpmsg_device_id *ids = rpdrv->id_table;
	unsigned int i;

	for (i = 0; ids[i].name[0]; i++)
		if (rpmsg_id_match(rpdev, &ids[i]))
			return 1;

	return 0;
}

static int rpmsg_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct rpmsg_channel *rpdev = to_rpmsg_channel(dev);

	return add_uevent_var(env, "MODALIAS=" RPMSG_DEVICE_MODALIAS_FMT,
					rpdev->id.name);
=======
static void virtio_rpmsg_destroy_ept(struct rpmsg_endpoint *ept);
static int virtio_rpmsg_send(struct rpmsg_endpoint *ept, void *data, int len);
static int virtio_rpmsg_sendto(struct rpmsg_endpoint *ept, void *data, int len,
			       u32 dst);
static int virtio_rpmsg_send_offchannel(struct rpmsg_endpoint *ept, u32 src,
					u32 dst, void *data, int len);
static int virtio_rpmsg_trysend(struct rpmsg_endpoint *ept, void *data, int len);
static int virtio_rpmsg_trysendto(struct rpmsg_endpoint *ept, void *data,
				  int len, u32 dst);
static int virtio_rpmsg_trysend_offchannel(struct rpmsg_endpoint *ept, u32 src,
					   u32 dst, void *data, int len);
static ssize_t virtio_rpmsg_get_mtu(struct rpmsg_endpoint *ept);
static struct rpmsg_device *__rpmsg_create_channel(struct virtproc_info *vrp,
						   struct rpmsg_channel_info *chinfo);

static const struct rpmsg_endpoint_ops virtio_endpoint_ops = {
	.destroy_ept = virtio_rpmsg_destroy_ept,
	.send = virtio_rpmsg_send,
	.sendto = virtio_rpmsg_sendto,
	.send_offchannel = virtio_rpmsg_send_offchannel,
	.trysend = virtio_rpmsg_trysend,
	.trysendto = virtio_rpmsg_trysendto,
	.trysend_offchannel = virtio_rpmsg_trysend_offchannel,
	.get_mtu = virtio_rpmsg_get_mtu,
};

/**
 * rpmsg_sg_init - initialize scatterlist according to cpu address location
 * @sg: scatterlist to fill
 * @cpu_addr: virtual address of the buffer
 * @len: buffer length
 *
 * An internal function filling scatterlist according to virtual address
 * location (in vmalloc or in kernel).
 */
static void
rpmsg_sg_init(struct scatterlist *sg, void *cpu_addr, unsigned int len)
{
	if (is_vmalloc_addr(cpu_addr)) {
		sg_init_table(sg, 1);
		sg_set_page(sg, vmalloc_to_page(cpu_addr), len,
			    offset_in_page(cpu_addr));
	} else {
		WARN_ON(!virt_addr_valid(cpu_addr));
		sg_init_one(sg, cpu_addr, len);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * __ept_release() - deallocate an rpmsg endpoint
 * @kref: the ept's reference count
 *
 * This function deallocates an ept, and is invoked when its @kref refcount
 * drops to zero.
 *
 * Never invoke this function directly!
 */
static void __ept_release(struct kref *kref)
{
	struct rpmsg_endpoint *ept = container_of(kref, struct rpmsg_endpoint,
						  refcount);
	/*
	 * At this point no one holds a reference to ept anymore,
	 * so we can directly free it
	 */
	kfree(ept);
}

/* for more info, see below documentation of rpmsg_create_ept() */
static struct rpmsg_endpoint *__rpmsg_create_ept(struct virtproc_info *vrp,
<<<<<<< HEAD
		struct rpmsg_channel *rpdev, rpmsg_rx_cb_t cb,
		void *priv, u32 addr)
{
	int err, tmpaddr, request;
	struct rpmsg_endpoint *ept;
	struct device *dev = rpdev ? &rpdev->dev : &vrp->vdev->dev;

	if (!idr_pre_get(&vrp->endpoints, GFP_KERNEL))
		return NULL;

	ept = kzalloc(sizeof(*ept), GFP_KERNEL);
	if (!ept) {
		dev_err(dev, "failed to kzalloc a new ept\n");
		return NULL;
	}
=======
						 struct rpmsg_device *rpdev,
						 rpmsg_rx_cb_t cb,
						 void *priv, u32 addr)
{
	int id_min, id_max, id;
	struct rpmsg_endpoint *ept;
	struct device *dev = rpdev ? &rpdev->dev : &vrp->vdev->dev;

	ept = kzalloc(sizeof(*ept), GFP_KERNEL);
	if (!ept)
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kref_init(&ept->refcount);
	mutex_init(&ept->cb_lock);

	ept->rpdev = rpdev;
	ept->cb = cb;
	ept->priv = priv;
<<<<<<< HEAD

	/* do we need to allocate a local address ? */
	request = addr == RPMSG_ADDR_ANY ? RPMSG_RESERVED_ADDRESSES : addr;
=======
	ept->ops = &virtio_endpoint_ops;

	/* do we need to allocate a local address ? */
	if (addr == RPMSG_ADDR_ANY) {
		id_min = RPMSG_RESERVED_ADDRESSES;
		id_max = 0;
	} else {
		id_min = addr;
		id_max = addr + 1;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&vrp->endpoints_lock);

	/* bind the endpoint to an rpmsg address (and allocate one if needed) */
<<<<<<< HEAD
	err = idr_get_new_above(&vrp->endpoints, ept, request, &tmpaddr);
	if (err) {
		dev_err(dev, "idr_get_new_above failed: %d\n", err);
		goto free_ept;
	}

	/* make sure the user's address request is fulfilled, if relevant */
	if (addr != RPMSG_ADDR_ANY && tmpaddr != addr) {
		dev_err(dev, "address 0x%x already in use\n", addr);
		goto rem_idr;
	}

	ept->addr = tmpaddr;
=======
	id = idr_alloc(&vrp->endpoints, ept, id_min, id_max, GFP_KERNEL);
	if (id < 0) {
		dev_err(dev, "idr_alloc failed: %d\n", id);
		goto free_ept;
	}
	ept->addr = id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_unlock(&vrp->endpoints_lock);

	return ept;

<<<<<<< HEAD
rem_idr:
	idr_remove(&vrp->endpoints, request);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
free_ept:
	mutex_unlock(&vrp->endpoints_lock);
	kref_put(&ept->refcount, __ept_release);
	return NULL;
}

<<<<<<< HEAD
/**
 * rpmsg_create_ept() - create a new rpmsg_endpoint
 * @rpdev: rpmsg channel device
 * @cb: rx callback handler
 * @priv: private data for the driver's use
 * @addr: local rpmsg address to bind with @cb
 *
 * Every rpmsg address in the system is bound to an rx callback (so when
 * inbound messages arrive, they are dispatched by the rpmsg bus using the
 * appropriate callback handler) by means of an rpmsg_endpoint struct.
 *
 * This function allows drivers to create such an endpoint, and by that,
 * bind a callback, and possibly some private data too, to an rpmsg address
 * (either one that is known in advance, or one that will be dynamically
 * assigned for them).
 *
 * Simple rpmsg drivers need not call rpmsg_create_ept, because an endpoint
 * is already created for them when they are probed by the rpmsg bus
 * (using the rx callback provided when they registered to the rpmsg bus).
 *
 * So things should just work for simple drivers: they already have an
 * endpoint, their rx callback is bound to their rpmsg address, and when
 * relevant inbound messages arrive (i.e. messages which their dst address
 * equals to the src address of their rpmsg channel), the driver's handler
 * is invoked to process it.
 *
 * That said, more complicated drivers might do need to allocate
 * additional rpmsg addresses, and bind them to different rx callbacks.
 * To accomplish that, those drivers need to call this function.
 *
 * Drivers should provide their @rpdev channel (so the new endpoint would belong
 * to the same remote processor their channel belongs to), an rx callback
 * function, an optional private data (which is provided back when the
 * rx callback is invoked), and an address they want to bind with the
 * callback. If @addr is RPMSG_ADDR_ANY, then rpmsg_create_ept will
 * dynamically assign them an available rpmsg address (drivers should have
 * a very good reason why not to always use RPMSG_ADDR_ANY here).
 *
 * Returns a pointer to the endpoint on success, or NULL on error.
 */
struct rpmsg_endpoint *rpmsg_create_ept(struct rpmsg_channel *rpdev,
				rpmsg_rx_cb_t cb, void *priv, u32 addr)
{
	return __rpmsg_create_ept(rpdev->vrp, rpdev, cb, priv, addr);
}
EXPORT_SYMBOL(rpmsg_create_ept);
=======
static struct rpmsg_device *virtio_rpmsg_create_channel(struct rpmsg_device *rpdev,
							struct rpmsg_channel_info *chinfo)
{
	struct virtio_rpmsg_channel *vch = to_virtio_rpmsg_channel(rpdev);
	struct virtproc_info *vrp = vch->vrp;

	return __rpmsg_create_channel(vrp, chinfo);
}

static int virtio_rpmsg_release_channel(struct rpmsg_device *rpdev,
					struct rpmsg_channel_info *chinfo)
{
	struct virtio_rpmsg_channel *vch = to_virtio_rpmsg_channel(rpdev);
	struct virtproc_info *vrp = vch->vrp;

	return rpmsg_unregister_device(&vrp->vdev->dev, chinfo);
}

static struct rpmsg_endpoint *virtio_rpmsg_create_ept(struct rpmsg_device *rpdev,
						      rpmsg_rx_cb_t cb,
						      void *priv,
						      struct rpmsg_channel_info chinfo)
{
	struct virtio_rpmsg_channel *vch = to_virtio_rpmsg_channel(rpdev);

	return __rpmsg_create_ept(vch->vrp, rpdev, cb, priv, chinfo.src);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * __rpmsg_destroy_ept() - destroy an existing rpmsg endpoint
 * @vrp: virtproc which owns this ept
 * @ept: endpoing to destroy
 *
 * An internal function which destroy an ept without assuming it is
 * bound to an rpmsg channel. This is needed for handling the internal
 * name service endpoint, which isn't bound to an rpmsg channel.
 * See also __rpmsg_create_ept().
 */
static void
__rpmsg_destroy_ept(struct virtproc_info *vrp, struct rpmsg_endpoint *ept)
{
	/* make sure new inbound messages can't find this ept anymore */
	mutex_lock(&vrp->endpoints_lock);
	idr_remove(&vrp->endpoints, ept->addr);
	mutex_unlock(&vrp->endpoints_lock);

	/* make sure in-flight inbound messages won't invoke cb anymore */
	mutex_lock(&ept->cb_lock);
	ept->cb = NULL;
	mutex_unlock(&ept->cb_lock);

	kref_put(&ept->refcount, __ept_release);
}

<<<<<<< HEAD
/**
 * rpmsg_destroy_ept() - destroy an existing rpmsg endpoint
 * @ept: endpoing to destroy
 *
 * Should be used by drivers to destroy an rpmsg endpoint previously
 * created with rpmsg_create_ept().
 */
void rpmsg_destroy_ept(struct rpmsg_endpoint *ept)
{
	__rpmsg_destroy_ept(ept->rpdev->vrp, ept);
}
EXPORT_SYMBOL(rpmsg_destroy_ept);

/*
 * when an rpmsg driver is probed with a channel, we seamlessly create
 * it an endpoint, binding its rx callback to a unique local rpmsg
 * address.
 *
 * if we need to, we also announce about this channel to the remote
 * processor (needed in case the driver is exposing an rpmsg service).
 */
static int rpmsg_dev_probe(struct device *dev)
{
	struct rpmsg_channel *rpdev = to_rpmsg_channel(dev);
	struct rpmsg_driver *rpdrv = to_rpmsg_driver(rpdev->dev.driver);
	struct virtproc_info *vrp = rpdev->vrp;
	struct rpmsg_endpoint *ept;
	int err;

	ept = rpmsg_create_ept(rpdev, rpdrv->callback, NULL, rpdev->src);
	if (!ept) {
		dev_err(dev, "failed to create endpoint\n");
		err = -ENOMEM;
		goto out;
	}

	rpdev->ept = ept;
	rpdev->src = ept->addr;

	err = rpdrv->probe(rpdev);
	if (err) {
		dev_err(dev, "%s: failed: %d\n", __func__, err);
		rpmsg_destroy_ept(ept);
		goto out;
	}

	/* need to tell remote processor's name service about this channel ? */
	if (rpdev->announce &&
			virtio_has_feature(vrp->vdev, VIRTIO_RPMSG_F_NS)) {
		struct rpmsg_ns_msg nsm;

		strncpy(nsm.name, rpdev->id.name, RPMSG_NAME_SIZE);
		nsm.addr = rpdev->src;
		nsm.flags = RPMSG_NS_CREATE;

		err = rpmsg_sendto(rpdev, &nsm, sizeof(nsm), RPMSG_NS_ADDR);
=======
static void virtio_rpmsg_destroy_ept(struct rpmsg_endpoint *ept)
{
	struct virtio_rpmsg_channel *vch = to_virtio_rpmsg_channel(ept->rpdev);

	__rpmsg_destroy_ept(vch->vrp, ept);
}

static int virtio_rpmsg_announce_create(struct rpmsg_device *rpdev)
{
	struct virtio_rpmsg_channel *vch = to_virtio_rpmsg_channel(rpdev);
	struct virtproc_info *vrp = vch->vrp;
	struct device *dev = &rpdev->dev;
	int err = 0;

	/* need to tell remote processor's name service about this channel ? */
	if (rpdev->announce && rpdev->ept &&
	    virtio_has_feature(vrp->vdev, VIRTIO_RPMSG_F_NS)) {
		struct rpmsg_ns_msg nsm;

		strscpy_pad(nsm.name, rpdev->id.name, sizeof(nsm.name));
		nsm.addr = cpu_to_rpmsg32(rpdev, rpdev->ept->addr);
		nsm.flags = cpu_to_rpmsg32(rpdev, RPMSG_NS_CREATE);

		err = rpmsg_sendto(rpdev->ept, &nsm, sizeof(nsm), RPMSG_NS_ADDR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			dev_err(dev, "failed to announce service %d\n", err);
	}

<<<<<<< HEAD
out:
	return err;
}

static int rpmsg_dev_remove(struct device *dev)
{
	struct rpmsg_channel *rpdev = to_rpmsg_channel(dev);
	struct rpmsg_driver *rpdrv = to_rpmsg_driver(rpdev->dev.driver);
	struct virtproc_info *vrp = rpdev->vrp;
	int err = 0;

	/* tell remote processor's name service we're removing this channel */
	if (rpdev->announce &&
			virtio_has_feature(vrp->vdev, VIRTIO_RPMSG_F_NS)) {
		struct rpmsg_ns_msg nsm;

		strncpy(nsm.name, rpdev->id.name, RPMSG_NAME_SIZE);
		nsm.addr = rpdev->src;
		nsm.flags = RPMSG_NS_DESTROY;

		err = rpmsg_sendto(rpdev, &nsm, sizeof(nsm), RPMSG_NS_ADDR);
=======
	return err;
}

static int virtio_rpmsg_announce_destroy(struct rpmsg_device *rpdev)
{
	struct virtio_rpmsg_channel *vch = to_virtio_rpmsg_channel(rpdev);
	struct virtproc_info *vrp = vch->vrp;
	struct device *dev = &rpdev->dev;
	int err = 0;

	/* tell remote processor's name service we're removing this channel */
	if (rpdev->announce && rpdev->ept &&
	    virtio_has_feature(vrp->vdev, VIRTIO_RPMSG_F_NS)) {
		struct rpmsg_ns_msg nsm;

		strscpy_pad(nsm.name, rpdev->id.name, sizeof(nsm.name));
		nsm.addr = cpu_to_rpmsg32(rpdev, rpdev->ept->addr);
		nsm.flags = cpu_to_rpmsg32(rpdev, RPMSG_NS_DESTROY);

		err = rpmsg_sendto(rpdev->ept, &nsm, sizeof(nsm), RPMSG_NS_ADDR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			dev_err(dev, "failed to announce service %d\n", err);
	}

<<<<<<< HEAD
	rpdrv->remove(rpdev);

	rpmsg_destroy_ept(rpdev->ept);

	return err;
}

static struct bus_type rpmsg_bus = {
	.name		= "rpmsg",
	.match		= rpmsg_dev_match,
	.dev_attrs	= rpmsg_dev_attrs,
	.uevent		= rpmsg_uevent,
	.probe		= rpmsg_dev_probe,
	.remove		= rpmsg_dev_remove,
};

/**
 * register_rpmsg_driver() - register an rpmsg driver with the rpmsg bus
 * @rpdrv: pointer to a struct rpmsg_driver
 *
 * Returns 0 on success, and an appropriate error value on failure.
 */
int register_rpmsg_driver(struct rpmsg_driver *rpdrv)
{
	rpdrv->drv.bus = &rpmsg_bus;
	return driver_register(&rpdrv->drv);
}
EXPORT_SYMBOL(register_rpmsg_driver);

/**
 * unregister_rpmsg_driver() - unregister an rpmsg driver from the rpmsg bus
 * @rpdrv: pointer to a struct rpmsg_driver
 *
 * Returns 0 on success, and an appropriate error value on failure.
 */
void unregister_rpmsg_driver(struct rpmsg_driver *rpdrv)
{
	driver_unregister(&rpdrv->drv);
}
EXPORT_SYMBOL(unregister_rpmsg_driver);

static void rpmsg_release_device(struct device *dev)
{
	struct rpmsg_channel *rpdev = to_rpmsg_channel(dev);

	kfree(rpdev);
}

/*
 * match an rpmsg channel with a channel info struct.
 * this is used to make sure we're not creating rpmsg devices for channels
 * that already exist.
 */
static int rpmsg_channel_match(struct device *dev, void *data)
{
	struct rpmsg_channel_info *chinfo = data;
	struct rpmsg_channel *rpdev = to_rpmsg_channel(dev);

	if (chinfo->src != RPMSG_ADDR_ANY && chinfo->src != rpdev->src)
		return 0;

	if (chinfo->dst != RPMSG_ADDR_ANY && chinfo->dst != rpdev->dst)
		return 0;

	if (strncmp(chinfo->name, rpdev->id.name, RPMSG_NAME_SIZE))
		return 0;

	/* found a match ! */
	return 1;
=======
	return err;
}

static const struct rpmsg_device_ops virtio_rpmsg_ops = {
	.create_channel = virtio_rpmsg_create_channel,
	.release_channel = virtio_rpmsg_release_channel,
	.create_ept = virtio_rpmsg_create_ept,
	.announce_create = virtio_rpmsg_announce_create,
	.announce_destroy = virtio_rpmsg_announce_destroy,
};

static void virtio_rpmsg_release_device(struct device *dev)
{
	struct rpmsg_device *rpdev = to_rpmsg_device(dev);
	struct virtio_rpmsg_channel *vch = to_virtio_rpmsg_channel(rpdev);

	kfree(rpdev->driver_override);
	kfree(vch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * create an rpmsg channel using its name and address info.
 * this function will be used to create both static and dynamic
 * channels.
 */
<<<<<<< HEAD
static struct rpmsg_channel *rpmsg_create_channel(struct virtproc_info *vrp,
				struct rpmsg_channel_info *chinfo)
{
	struct rpmsg_channel *rpdev;
=======
static struct rpmsg_device *__rpmsg_create_channel(struct virtproc_info *vrp,
						   struct rpmsg_channel_info *chinfo)
{
	struct virtio_rpmsg_channel *vch;
	struct rpmsg_device *rpdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct device *tmp, *dev = &vrp->vdev->dev;
	int ret;

	/* make sure a similar channel doesn't already exist */
<<<<<<< HEAD
	tmp = device_find_child(dev, chinfo, rpmsg_channel_match);
=======
	tmp = rpmsg_find_device(dev, chinfo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (tmp) {
		/* decrement the matched device's refcount back */
		put_device(tmp);
		dev_err(dev, "channel %s:%x:%x already exist\n",
				chinfo->name, chinfo->src, chinfo->dst);
		return NULL;
	}

<<<<<<< HEAD
	rpdev = kzalloc(sizeof(struct rpmsg_channel), GFP_KERNEL);
	if (!rpdev) {
		pr_err("kzalloc failed\n");
		return NULL;
	}

	rpdev->vrp = vrp;
	rpdev->src = chinfo->src;
	rpdev->dst = chinfo->dst;
=======
	vch = kzalloc(sizeof(*vch), GFP_KERNEL);
	if (!vch)
		return NULL;

	/* Link the channel to our vrp */
	vch->vrp = vrp;

	/* Assign public information to the rpmsg_device */
	rpdev = &vch->rpdev;
	rpdev->src = chinfo->src;
	rpdev->dst = chinfo->dst;
	rpdev->ops = &virtio_rpmsg_ops;
	rpdev->little_endian = virtio_is_little_endian(vrp->vdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * rpmsg server channels has predefined local address (for now),
	 * and their existence needs to be announced remotely
	 */
<<<<<<< HEAD
	rpdev->announce = rpdev->src != RPMSG_ADDR_ANY ? true : false;

	strncpy(rpdev->id.name, chinfo->name, RPMSG_NAME_SIZE);

	/* very simple device indexing plumbing which is enough for now */
	dev_set_name(&rpdev->dev, "rpmsg%d", rpmsg_dev_index++);

	rpdev->dev.parent = &vrp->vdev->dev;
	rpdev->dev.bus = &rpmsg_bus;
	rpdev->dev.release = rpmsg_release_device;

	ret = device_register(&rpdev->dev);
	if (ret) {
		dev_err(dev, "device_register failed: %d\n", ret);
		put_device(&rpdev->dev);
		return NULL;
	}
=======
	rpdev->announce = rpdev->src != RPMSG_ADDR_ANY;

	strscpy(rpdev->id.name, chinfo->name, sizeof(rpdev->id.name));

	rpdev->dev.parent = &vrp->vdev->dev;
	rpdev->dev.release = virtio_rpmsg_release_device;
	ret = rpmsg_register_device(rpdev);
	if (ret)
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rpdev;
}

<<<<<<< HEAD
/*
 * find an existing channel using its name + address properties,
 * and destroy it
 */
static int rpmsg_destroy_channel(struct virtproc_info *vrp,
					struct rpmsg_channel_info *chinfo)
{
	struct virtio_device *vdev = vrp->vdev;
	struct device *dev;

	dev = device_find_child(&vdev->dev, chinfo, rpmsg_channel_match);
	if (!dev)
		return -EINVAL;

	device_unregister(dev);

	put_device(dev);

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* super simple buffer "allocator" that is just enough for now */
static void *get_a_tx_buf(struct virtproc_info *vrp)
{
	unsigned int len;
	void *ret;

	/* support multiple concurrent senders */
	mutex_lock(&vrp->tx_lock);

	/*
	 * either pick the next unused tx buffer
	 * (half of our buffers are used for sending messages)
	 */
<<<<<<< HEAD
	if (vrp->last_sbuf < RPMSG_NUM_BUFS / 2)
		ret = vrp->sbufs + RPMSG_BUF_SIZE * vrp->last_sbuf++;
=======
	if (vrp->last_sbuf < vrp->num_bufs / 2)
		ret = vrp->sbufs + vrp->buf_size * vrp->last_sbuf++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* or recycle a used one */
	else
		ret = virtqueue_get_buf(vrp->svq, &len);

	mutex_unlock(&vrp->tx_lock);

	return ret;
}

/**
 * rpmsg_upref_sleepers() - enable "tx-complete" interrupts, if needed
 * @vrp: virtual remote processor state
 *
 * This function is called before a sender is blocked, waiting for
 * a tx buffer to become available.
 *
 * If we already have blocking senders, this function merely increases
 * the "sleepers" reference count, and exits.
 *
 * Otherwise, if this is the first sender to block, we also enable
 * virtio's tx callbacks, so we'd be immediately notified when a tx
 * buffer is consumed (we rely on virtio's tx callback in order
 * to wake up sleeping senders as soon as a tx buffer is used by the
 * remote processor).
 */
static void rpmsg_upref_sleepers(struct virtproc_info *vrp)
{
	/* support multiple concurrent senders */
	mutex_lock(&vrp->tx_lock);

	/* are we the first sleeping context waiting for tx buffers ? */
	if (atomic_inc_return(&vrp->sleepers) == 1)
		/* enable "tx-complete" interrupts before dozing off */
		virtqueue_enable_cb(vrp->svq);

	mutex_unlock(&vrp->tx_lock);
}

/**
 * rpmsg_downref_sleepers() - disable "tx-complete" interrupts, if needed
 * @vrp: virtual remote processor state
 *
 * This function is called after a sender, that waited for a tx buffer
 * to become available, is unblocked.
 *
 * If we still have blocking senders, this function merely decreases
 * the "sleepers" reference count, and exits.
 *
 * Otherwise, if there are no more blocking senders, we also disable
 * virtio's tx callbacks, to avoid the overhead incurred with handling
 * those (now redundant) interrupts.
 */
static void rpmsg_downref_sleepers(struct virtproc_info *vrp)
{
	/* support multiple concurrent senders */
	mutex_lock(&vrp->tx_lock);

	/* are we the last sleeping context waiting for tx buffers ? */
	if (atomic_dec_and_test(&vrp->sleepers))
		/* disable "tx-complete" interrupts */
		virtqueue_disable_cb(vrp->svq);

	mutex_unlock(&vrp->tx_lock);
}

/**
 * rpmsg_send_offchannel_raw() - send a message across to the remote processor
 * @rpdev: the rpmsg channel
 * @src: source address
 * @dst: destination address
 * @data: payload of message
 * @len: length of payload
 * @wait: indicates whether caller should block in case no TX buffers available
 *
 * This function is the base implementation for all of the rpmsg sending API.
 *
 * It will send @data of length @len to @dst, and say it's from @src. The
 * message will be sent to the remote processor which the @rpdev channel
 * belongs to.
 *
 * The message is sent using one of the TX buffers that are available for
 * communication with this remote processor.
 *
 * If @wait is true, the caller will be blocked until either a TX buffer is
 * available, or 15 seconds elapses (we don't want callers to
 * sleep indefinitely due to misbehaving remote processors), and in that
 * case -ERESTARTSYS is returned. The number '15' itself was picked
 * arbitrarily; there's little point in asking drivers to provide a timeout
 * value themselves.
 *
 * Otherwise, if @wait is false, and there are no TX buffers available,
 * the function will immediately fail, and -ENOMEM will be returned.
 *
 * Normally drivers shouldn't use this function directly; instead, drivers
 * should use the appropriate rpmsg_{try}send{to, _offchannel} API
 * (see include/linux/rpmsg.h).
 *
<<<<<<< HEAD
 * Returns 0 on success and an appropriate error value on failure.
 */
int rpmsg_send_offchannel_raw(struct rpmsg_channel *rpdev, u32 src, u32 dst,
					void *data, int len, bool wait)
{
	struct virtproc_info *vrp = rpdev->vrp;
=======
 * Return: 0 on success and an appropriate error value on failure.
 */
static int rpmsg_send_offchannel_raw(struct rpmsg_device *rpdev,
				     u32 src, u32 dst,
				     void *data, int len, bool wait)
{
	struct virtio_rpmsg_channel *vch = to_virtio_rpmsg_channel(rpdev);
	struct virtproc_info *vrp = vch->vrp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct device *dev = &rpdev->dev;
	struct scatterlist sg;
	struct rpmsg_hdr *msg;
	int err;

	/* bcasting isn't allowed */
	if (src == RPMSG_ADDR_ANY || dst == RPMSG_ADDR_ANY) {
		dev_err(dev, "invalid addr (src 0x%x, dst 0x%x)\n", src, dst);
		return -EINVAL;
	}

	/*
	 * We currently use fixed-sized buffers, and therefore the payload
	 * length is limited.
	 *
	 * One of the possible improvements here is either to support
	 * user-provided buffers (and then we can also support zero-copy
	 * messaging), or to improve the buffer allocator, to support
	 * variable-length buffer sizes.
	 */
<<<<<<< HEAD
	if (len > RPMSG_BUF_SIZE - sizeof(struct rpmsg_hdr)) {
=======
	if (len > vrp->buf_size - sizeof(struct rpmsg_hdr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(dev, "message is too big (%d)\n", len);
		return -EMSGSIZE;
	}

	/* grab a buffer */
	msg = get_a_tx_buf(vrp);
	if (!msg && !wait)
		return -ENOMEM;

	/* no free buffer ? wait for one (but bail after 15 seconds) */
	while (!msg) {
		/* enable "tx-complete" interrupts, if not already enabled */
		rpmsg_upref_sleepers(vrp);

		/*
		 * sleep until a free buffer is available or 15 secs elapse.
		 * the timeout period is not configurable because there's
		 * little point in asking drivers to specify that.
		 * if later this happens to be required, it'd be easy to add.
		 */
		err = wait_event_interruptible_timeout(vrp->sendq,
					(msg = get_a_tx_buf(vrp)),
					msecs_to_jiffies(15000));

		/* disable "tx-complete" interrupts if we're the last sleeper */
		rpmsg_downref_sleepers(vrp);

		/* timeout ? */
		if (!err) {
			dev_err(dev, "timeout waiting for a tx buffer\n");
			return -ERESTARTSYS;
		}
	}

<<<<<<< HEAD
	msg->len = len;
	msg->flags = 0;
	msg->src = src;
	msg->dst = dst;
=======
	msg->len = cpu_to_rpmsg16(rpdev, len);
	msg->flags = 0;
	msg->src = cpu_to_rpmsg32(rpdev, src);
	msg->dst = cpu_to_rpmsg32(rpdev, dst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	msg->reserved = 0;
	memcpy(msg->data, data, len);

	dev_dbg(dev, "TX From 0x%x, To 0x%x, Len %d, Flags %d, Reserved %d\n",
<<<<<<< HEAD
					msg->src, msg->dst, msg->len,
					msg->flags, msg->reserved);
	print_hex_dump(KERN_DEBUG, "rpmsg_virtio TX: ", DUMP_PREFIX_NONE, 16, 1,
					msg, sizeof(*msg) + msg->len, true);

	sg_init_one(&sg, msg, sizeof(*msg) + len);
=======
		src, dst, len, msg->flags, msg->reserved);
#if defined(CONFIG_DYNAMIC_DEBUG)
	dynamic_hex_dump("rpmsg_virtio TX: ", DUMP_PREFIX_NONE, 16, 1,
			 msg, sizeof(*msg) + len, true);
#endif

	rpmsg_sg_init(&sg, msg, sizeof(*msg) + len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&vrp->tx_lock);

	/* add message to the remote processor's virtqueue */
<<<<<<< HEAD
	err = virtqueue_add_buf(vrp->svq, &sg, 1, 0, msg, GFP_KERNEL);
	if (err < 0) {
=======
	err = virtqueue_add_outbuf(vrp->svq, &sg, 1, msg, GFP_KERNEL);
	if (err) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * need to reclaim the buffer here, otherwise it's lost
		 * (memory won't leak, but rpmsg won't use it again for TX).
		 * this will wait for a buffer management overhaul.
		 */
<<<<<<< HEAD
		dev_err(dev, "virtqueue_add_buf failed: %d\n", err);
=======
		dev_err(dev, "virtqueue_add_outbuf failed: %d\n", err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/* tell the remote processor it has a pending message to read */
	virtqueue_kick(vrp->svq);
<<<<<<< HEAD

	err = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	mutex_unlock(&vrp->tx_lock);
	return err;
}
<<<<<<< HEAD
EXPORT_SYMBOL(rpmsg_send_offchannel_raw);

/* called when an rx buffer is used, and it's time to digest a message */
static void rpmsg_recv_done(struct virtqueue *rvq)
{
	struct rpmsg_hdr *msg;
	unsigned int len;
	struct rpmsg_endpoint *ept;
	struct scatterlist sg;
	struct virtproc_info *vrp = rvq->vdev->priv;
	struct device *dev = &rvq->vdev->dev;
	int err;

	msg = virtqueue_get_buf(rvq, &len);
	if (!msg) {
		dev_err(dev, "uhm, incoming signal, but no used buffer ?\n");
		return;
	}

	dev_dbg(dev, "From: 0x%x, To: 0x%x, Len: %d, Flags: %d, Reserved: %d\n",
					msg->src, msg->dst, msg->len,
					msg->flags, msg->reserved);
	print_hex_dump(KERN_DEBUG, "rpmsg_virtio RX: ", DUMP_PREFIX_NONE, 16, 1,
					msg, sizeof(*msg) + msg->len, true);
=======

static int virtio_rpmsg_send(struct rpmsg_endpoint *ept, void *data, int len)
{
	struct rpmsg_device *rpdev = ept->rpdev;
	u32 src = ept->addr, dst = rpdev->dst;

	return rpmsg_send_offchannel_raw(rpdev, src, dst, data, len, true);
}

static int virtio_rpmsg_sendto(struct rpmsg_endpoint *ept, void *data, int len,
			       u32 dst)
{
	struct rpmsg_device *rpdev = ept->rpdev;
	u32 src = ept->addr;

	return rpmsg_send_offchannel_raw(rpdev, src, dst, data, len, true);
}

static int virtio_rpmsg_send_offchannel(struct rpmsg_endpoint *ept, u32 src,
					u32 dst, void *data, int len)
{
	struct rpmsg_device *rpdev = ept->rpdev;

	return rpmsg_send_offchannel_raw(rpdev, src, dst, data, len, true);
}

static int virtio_rpmsg_trysend(struct rpmsg_endpoint *ept, void *data, int len)
{
	struct rpmsg_device *rpdev = ept->rpdev;
	u32 src = ept->addr, dst = rpdev->dst;

	return rpmsg_send_offchannel_raw(rpdev, src, dst, data, len, false);
}

static int virtio_rpmsg_trysendto(struct rpmsg_endpoint *ept, void *data,
				  int len, u32 dst)
{
	struct rpmsg_device *rpdev = ept->rpdev;
	u32 src = ept->addr;

	return rpmsg_send_offchannel_raw(rpdev, src, dst, data, len, false);
}

static int virtio_rpmsg_trysend_offchannel(struct rpmsg_endpoint *ept, u32 src,
					   u32 dst, void *data, int len)
{
	struct rpmsg_device *rpdev = ept->rpdev;

	return rpmsg_send_offchannel_raw(rpdev, src, dst, data, len, false);
}

static ssize_t virtio_rpmsg_get_mtu(struct rpmsg_endpoint *ept)
{
	struct rpmsg_device *rpdev = ept->rpdev;
	struct virtio_rpmsg_channel *vch = to_virtio_rpmsg_channel(rpdev);

	return vch->vrp->buf_size - sizeof(struct rpmsg_hdr);
}

static int rpmsg_recv_single(struct virtproc_info *vrp, struct device *dev,
			     struct rpmsg_hdr *msg, unsigned int len)
{
	struct rpmsg_endpoint *ept;
	struct scatterlist sg;
	bool little_endian = virtio_is_little_endian(vrp->vdev);
	unsigned int msg_len = __rpmsg16_to_cpu(little_endian, msg->len);
	int err;

	dev_dbg(dev, "From: 0x%x, To: 0x%x, Len: %d, Flags: %d, Reserved: %d\n",
		__rpmsg32_to_cpu(little_endian, msg->src),
		__rpmsg32_to_cpu(little_endian, msg->dst), msg_len,
		__rpmsg16_to_cpu(little_endian, msg->flags),
		__rpmsg32_to_cpu(little_endian, msg->reserved));
#if defined(CONFIG_DYNAMIC_DEBUG)
	dynamic_hex_dump("rpmsg_virtio RX: ", DUMP_PREFIX_NONE, 16, 1,
			 msg, sizeof(*msg) + msg_len, true);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * We currently use fixed-sized buffers, so trivially sanitize
	 * the reported payload length.
	 */
<<<<<<< HEAD
	if (len > RPMSG_BUF_SIZE ||
		msg->len > (len - sizeof(struct rpmsg_hdr))) {
		dev_warn(dev, "inbound msg too big: (%d, %d)\n", len, msg->len);
		return;
=======
	if (len > vrp->buf_size ||
	    msg_len > (len - sizeof(struct rpmsg_hdr))) {
		dev_warn(dev, "inbound msg too big: (%d, %d)\n", len, msg_len);
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* use the dst addr to fetch the callback of the appropriate user */
	mutex_lock(&vrp->endpoints_lock);

<<<<<<< HEAD
	ept = idr_find(&vrp->endpoints, msg->dst);
=======
	ept = idr_find(&vrp->endpoints, __rpmsg32_to_cpu(little_endian, msg->dst));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* let's make sure no one deallocates ept while we use it */
	if (ept)
		kref_get(&ept->refcount);

	mutex_unlock(&vrp->endpoints_lock);

	if (ept) {
		/* make sure ept->cb doesn't go away while we use it */
		mutex_lock(&ept->cb_lock);

		if (ept->cb)
<<<<<<< HEAD
			ept->cb(ept->rpdev, msg->data, msg->len, ept->priv,
				msg->src);
=======
			ept->cb(ept->rpdev, msg->data, msg_len, ept->priv,
				__rpmsg32_to_cpu(little_endian, msg->src));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		mutex_unlock(&ept->cb_lock);

		/* farewell, ept, we don't need you anymore */
		kref_put(&ept->refcount, __ept_release);
	} else
<<<<<<< HEAD
		dev_warn(dev, "msg received with no recepient\n");

	/* publish the real size of the buffer */
	sg_init_one(&sg, msg, RPMSG_BUF_SIZE);

	/* add the buffer back to the remote processor's virtqueue */
	err = virtqueue_add_buf(vrp->rvq, &sg, 0, 1, msg, GFP_KERNEL);
	if (err < 0) {
		dev_err(dev, "failed to add a virtqueue buffer: %d\n", err);
		return;
	}

	/* tell the remote processor we added another available rx buffer */
	virtqueue_kick(vrp->rvq);
=======
		dev_warn_ratelimited(dev, "msg received with no recipient\n");

	/* publish the real size of the buffer */
	rpmsg_sg_init(&sg, msg, vrp->buf_size);

	/* add the buffer back to the remote processor's virtqueue */
	err = virtqueue_add_inbuf(vrp->rvq, &sg, 1, msg, GFP_KERNEL);
	if (err < 0) {
		dev_err(dev, "failed to add a virtqueue buffer: %d\n", err);
		return err;
	}

	return 0;
}

/* called when an rx buffer is used, and it's time to digest a message */
static void rpmsg_recv_done(struct virtqueue *rvq)
{
	struct virtproc_info *vrp = rvq->vdev->priv;
	struct device *dev = &rvq->vdev->dev;
	struct rpmsg_hdr *msg;
	unsigned int len, msgs_received = 0;
	int err;

	msg = virtqueue_get_buf(rvq, &len);
	if (!msg) {
		dev_err(dev, "uhm, incoming signal, but no used buffer ?\n");
		return;
	}

	while (msg) {
		err = rpmsg_recv_single(vrp, dev, msg, len);
		if (err)
			break;

		msgs_received++;

		msg = virtqueue_get_buf(rvq, &len);
	}

	dev_dbg(dev, "Received %u messages\n", msgs_received);

	/* tell the remote processor we added another available rx buffer */
	if (msgs_received)
		virtqueue_kick(vrp->rvq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This is invoked whenever the remote processor completed processing
 * a TX msg we just sent it, and the buffer is put back to the used ring.
 *
 * Normally, though, we suppress this "tx complete" interrupt in order to
 * avoid the incurred overhead.
 */
static void rpmsg_xmit_done(struct virtqueue *svq)
{
	struct virtproc_info *vrp = svq->vdev->priv;

	dev_dbg(&svq->vdev->dev, "%s\n", __func__);

	/* wake up potential senders that are waiting for a tx buffer */
	wake_up_interruptible(&vrp->sendq);
}

<<<<<<< HEAD
/* invoked when a name service announcement arrives */
static void rpmsg_ns_cb(struct rpmsg_channel *rpdev, void *data, int len,
							void *priv, u32 src)
{
	struct rpmsg_ns_msg *msg = data;
	struct rpmsg_channel *newch;
	struct rpmsg_channel_info chinfo;
	struct virtproc_info *vrp = priv;
	struct device *dev = &vrp->vdev->dev;
	int ret;

	print_hex_dump(KERN_DEBUG, "NS announcement: ",
			DUMP_PREFIX_NONE, 16, 1,
			data, len, true);

	if (len != sizeof(*msg)) {
		dev_err(dev, "malformed ns msg (%d)\n", len);
		return;
	}

	/*
	 * the name service ept does _not_ belong to a real rpmsg channel,
	 * and is handled by the rpmsg bus itself.
	 * for sanity reasons, make sure a valid rpdev has _not_ sneaked
	 * in somehow.
	 */
	if (rpdev) {
		dev_err(dev, "anomaly: ns ept has an rpdev handle\n");
		return;
	}

	/* don't trust the remote processor for null terminating the name */
	msg->name[RPMSG_NAME_SIZE - 1] = '\0';

	dev_info(dev, "%sing channel %s addr 0x%x\n",
			msg->flags & RPMSG_NS_DESTROY ? "destroy" : "creat",
			msg->name, msg->addr);

	strncpy(chinfo.name, msg->name, sizeof(chinfo.name));
	chinfo.src = RPMSG_ADDR_ANY;
	chinfo.dst = msg->addr;

	if (msg->flags & RPMSG_NS_DESTROY) {
		ret = rpmsg_destroy_channel(vrp, &chinfo);
		if (ret)
			dev_err(dev, "rpmsg_destroy_channel failed: %d\n", ret);
	} else {
		newch = rpmsg_create_channel(vrp, &chinfo);
		if (!newch)
			dev_err(dev, "rpmsg_create_channel failed\n");
	}
=======
/*
 * Called to expose to user a /dev/rpmsg_ctrlX interface allowing to
 * create endpoint-to-endpoint communication without associated RPMsg channel.
 * The endpoints are rattached to the ctrldev RPMsg device.
 */
static struct rpmsg_device *rpmsg_virtio_add_ctrl_dev(struct virtio_device *vdev)
{
	struct virtproc_info *vrp = vdev->priv;
	struct virtio_rpmsg_channel *vch;
	struct rpmsg_device *rpdev_ctrl;
	int err = 0;

	vch = kzalloc(sizeof(*vch), GFP_KERNEL);
	if (!vch)
		return ERR_PTR(-ENOMEM);

	/* Link the channel to the vrp */
	vch->vrp = vrp;

	/* Assign public information to the rpmsg_device */
	rpdev_ctrl = &vch->rpdev;
	rpdev_ctrl->ops = &virtio_rpmsg_ops;

	rpdev_ctrl->dev.parent = &vrp->vdev->dev;
	rpdev_ctrl->dev.release = virtio_rpmsg_release_device;
	rpdev_ctrl->little_endian = virtio_is_little_endian(vrp->vdev);

	err = rpmsg_ctrldev_register_device(rpdev_ctrl);
	if (err) {
		/* vch will be free in virtio_rpmsg_release_device() */
		return ERR_PTR(err);
	}

	return rpdev_ctrl;
}

static void rpmsg_virtio_del_ctrl_dev(struct rpmsg_device *rpdev_ctrl)
{
	if (!rpdev_ctrl)
		return;
	device_unregister(&rpdev_ctrl->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int rpmsg_probe(struct virtio_device *vdev)
{
	vq_callback_t *vq_cbs[] = { rpmsg_recv_done, rpmsg_xmit_done };
<<<<<<< HEAD
	const char *names[] = { "input", "output" };
	struct virtqueue *vqs[2];
	struct virtproc_info *vrp;
	void *bufs_va;
	int err = 0, i;
=======
	static const char * const names[] = { "input", "output" };
	struct virtqueue *vqs[2];
	struct virtproc_info *vrp;
	struct virtio_rpmsg_channel *vch = NULL;
	struct rpmsg_device *rpdev_ns, *rpdev_ctrl;
	void *bufs_va;
	int err = 0, i;
	size_t total_buf_space;
	bool notify;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	vrp = kzalloc(sizeof(*vrp), GFP_KERNEL);
	if (!vrp)
		return -ENOMEM;

	vrp->vdev = vdev;

	idr_init(&vrp->endpoints);
	mutex_init(&vrp->endpoints_lock);
	mutex_init(&vrp->tx_lock);
	init_waitqueue_head(&vrp->sendq);

	/* We expect two virtqueues, rx and tx (and in this order) */
<<<<<<< HEAD
	err = vdev->config->find_vqs(vdev, 2, vqs, vq_cbs, names);
=======
	err = virtio_find_vqs(vdev, 2, vqs, vq_cbs, names, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto free_vrp;

	vrp->rvq = vqs[0];
	vrp->svq = vqs[1];

<<<<<<< HEAD
	/* allocate coherent memory for the buffers */
	bufs_va = dma_alloc_coherent(vdev->dev.parent, RPMSG_TOTAL_BUF_SPACE,
				&vrp->bufs_dma, GFP_KERNEL);
	if (!bufs_va)
		goto vqs_del;

	dev_dbg(&vdev->dev, "buffers: va %p, dma 0x%llx\n", bufs_va,
					(unsigned long long)vrp->bufs_dma);
=======
	/* we expect symmetric tx/rx vrings */
	WARN_ON(virtqueue_get_vring_size(vrp->rvq) !=
		virtqueue_get_vring_size(vrp->svq));

	/* we need less buffers if vrings are small */
	if (virtqueue_get_vring_size(vrp->rvq) < MAX_RPMSG_NUM_BUFS / 2)
		vrp->num_bufs = virtqueue_get_vring_size(vrp->rvq) * 2;
	else
		vrp->num_bufs = MAX_RPMSG_NUM_BUFS;

	vrp->buf_size = MAX_RPMSG_BUF_SIZE;

	total_buf_space = vrp->num_bufs * vrp->buf_size;

	/* allocate coherent memory for the buffers */
	bufs_va = dma_alloc_coherent(vdev->dev.parent,
				     total_buf_space, &vrp->bufs_dma,
				     GFP_KERNEL);
	if (!bufs_va) {
		err = -ENOMEM;
		goto vqs_del;
	}

	dev_dbg(&vdev->dev, "buffers: va %pK, dma %pad\n",
		bufs_va, &vrp->bufs_dma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* half of the buffers is dedicated for RX */
	vrp->rbufs = bufs_va;

	/* and half is dedicated for TX */
<<<<<<< HEAD
	vrp->sbufs = bufs_va + RPMSG_TOTAL_BUF_SPACE / 2;

	/* set up the receive buffers */
	for (i = 0; i < RPMSG_NUM_BUFS / 2; i++) {
		struct scatterlist sg;
		void *cpu_addr = vrp->rbufs + i * RPMSG_BUF_SIZE;

		sg_init_one(&sg, cpu_addr, RPMSG_BUF_SIZE);

		err = virtqueue_add_buf(vrp->rvq, &sg, 0, 1, cpu_addr,
								GFP_KERNEL);
		WARN_ON(err < 0); /* sanity check; this can't really happen */
=======
	vrp->sbufs = bufs_va + total_buf_space / 2;

	/* set up the receive buffers */
	for (i = 0; i < vrp->num_bufs / 2; i++) {
		struct scatterlist sg;
		void *cpu_addr = vrp->rbufs + i * vrp->buf_size;

		rpmsg_sg_init(&sg, cpu_addr, vrp->buf_size);

		err = virtqueue_add_inbuf(vrp->rvq, &sg, 1, cpu_addr,
					  GFP_KERNEL);
		WARN_ON(err); /* sanity check; this can't really happen */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* suppress "tx-complete" interrupts */
	virtqueue_disable_cb(vrp->svq);

	vdev->priv = vrp;

<<<<<<< HEAD
	/* if supported by the remote processor, enable the name service */
	if (virtio_has_feature(vdev, VIRTIO_RPMSG_F_NS)) {
		/* a dedicated endpoint handles the name service msgs */
		vrp->ns_ept = __rpmsg_create_ept(vrp, NULL, rpmsg_ns_cb,
						vrp, RPMSG_NS_ADDR);
		if (!vrp->ns_ept) {
			dev_err(&vdev->dev, "failed to create the ns ept\n");
			err = -ENOMEM;
			goto free_coherent;
		}
	}

	/* tell the remote processor it can start sending messages */
	virtqueue_kick(vrp->rvq);
=======
	rpdev_ctrl = rpmsg_virtio_add_ctrl_dev(vdev);
	if (IS_ERR(rpdev_ctrl)) {
		err = PTR_ERR(rpdev_ctrl);
		goto free_coherent;
	}

	/* if supported by the remote processor, enable the name service */
	if (virtio_has_feature(vdev, VIRTIO_RPMSG_F_NS)) {
		vch = kzalloc(sizeof(*vch), GFP_KERNEL);
		if (!vch) {
			err = -ENOMEM;
			goto free_ctrldev;
		}

		/* Link the channel to our vrp */
		vch->vrp = vrp;

		/* Assign public information to the rpmsg_device */
		rpdev_ns = &vch->rpdev;
		rpdev_ns->ops = &virtio_rpmsg_ops;
		rpdev_ns->little_endian = virtio_is_little_endian(vrp->vdev);

		rpdev_ns->dev.parent = &vrp->vdev->dev;
		rpdev_ns->dev.release = virtio_rpmsg_release_device;

		err = rpmsg_ns_register_device(rpdev_ns);
		if (err)
			/* vch will be free in virtio_rpmsg_release_device() */
			goto free_ctrldev;
	}

	/*
	 * Prepare to kick but don't notify yet - we can't do this before
	 * device is ready.
	 */
	notify = virtqueue_kick_prepare(vrp->rvq);

	/* From this point on, we can notify and get callbacks. */
	virtio_device_ready(vdev);

	/* tell the remote processor it can start sending messages */
	/*
	 * this might be concurrent with callbacks, but we are only
	 * doing notify, not a full kick here, so that's ok.
	 */
	if (notify)
		virtqueue_notify(vrp->rvq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_info(&vdev->dev, "rpmsg host is online\n");

	return 0;

<<<<<<< HEAD
free_coherent:
	dma_free_coherent(vdev->dev.parent, RPMSG_TOTAL_BUF_SPACE, bufs_va,
					vrp->bufs_dma);
=======
free_ctrldev:
	rpmsg_virtio_del_ctrl_dev(rpdev_ctrl);
free_coherent:
	dma_free_coherent(vdev->dev.parent, total_buf_space,
			  bufs_va, vrp->bufs_dma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
vqs_del:
	vdev->config->del_vqs(vrp->vdev);
free_vrp:
	kfree(vrp);
	return err;
}

static int rpmsg_remove_device(struct device *dev, void *data)
{
	device_unregister(dev);

	return 0;
}

<<<<<<< HEAD
static void __devexit rpmsg_remove(struct virtio_device *vdev)
{
	struct virtproc_info *vrp = vdev->priv;
	int ret;

	vdev->config->reset(vdev);
=======
static void rpmsg_remove(struct virtio_device *vdev)
{
	struct virtproc_info *vrp = vdev->priv;
	size_t total_buf_space = vrp->num_bufs * vrp->buf_size;
	int ret;

	virtio_reset_device(vdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = device_for_each_child(&vdev->dev, NULL, rpmsg_remove_device);
	if (ret)
		dev_warn(&vdev->dev, "can't remove rpmsg device: %d\n", ret);

<<<<<<< HEAD
	if (vrp->ns_ept)
		__rpmsg_destroy_ept(vrp, vrp->ns_ept);

	idr_remove_all(&vrp->endpoints);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	idr_destroy(&vrp->endpoints);

	vdev->config->del_vqs(vrp->vdev);

<<<<<<< HEAD
	dma_free_coherent(vdev->dev.parent, RPMSG_TOTAL_BUF_SPACE,
					vrp->rbufs, vrp->bufs_dma);
=======
	dma_free_coherent(vdev->dev.parent, total_buf_space,
			  vrp->rbufs, vrp->bufs_dma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree(vrp);
}

static struct virtio_device_id id_table[] = {
	{ VIRTIO_ID_RPMSG, VIRTIO_DEV_ANY_ID },
	{ 0 },
};

static unsigned int features[] = {
	VIRTIO_RPMSG_F_NS,
};

static struct virtio_driver virtio_ipc_driver = {
	.feature_table	= features,
	.feature_table_size = ARRAY_SIZE(features),
	.driver.name	= KBUILD_MODNAME,
	.driver.owner	= THIS_MODULE,
	.id_table	= id_table,
	.probe		= rpmsg_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(rpmsg_remove),
=======
	.remove		= rpmsg_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init rpmsg_init(void)
{
	int ret;

<<<<<<< HEAD
	ret = bus_register(&rpmsg_bus);
	if (ret) {
		pr_err("failed to register rpmsg bus: %d\n", ret);
		return ret;
	}

	ret = register_virtio_driver(&virtio_ipc_driver);
	if (ret) {
		pr_err("failed to register virtio driver: %d\n", ret);
		bus_unregister(&rpmsg_bus);
	}
=======
	ret = register_virtio_driver(&virtio_ipc_driver);
	if (ret)
		pr_err("failed to register virtio driver: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}
subsys_initcall(rpmsg_init);

static void __exit rpmsg_fini(void)
{
	unregister_virtio_driver(&virtio_ipc_driver);
<<<<<<< HEAD
	bus_unregister(&rpmsg_bus);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
module_exit(rpmsg_fini);

MODULE_DEVICE_TABLE(virtio, id_table);
MODULE_DESCRIPTION("Virtio-based remote processor messaging bus");
MODULE_LICENSE("GPL v2");
