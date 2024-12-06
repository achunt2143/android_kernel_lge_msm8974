<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_VIRTIO_H
#define _LINUX_VIRTIO_H
/* Everything a virtio driver needs to work with any particular virtio
 * implementation. */
#include <linux/types.h>
#include <linux/scatterlist.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/mod_devicetable.h>
#include <linux/gfp.h>
<<<<<<< HEAD

/**
 * virtqueue - a queue to register buffers for sending or receiving.
=======
#include <linux/dma-mapping.h>

/**
 * struct virtqueue - a queue to register buffers for sending or receiving.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @list: the chain of virtqueues for this device
 * @callback: the function to call when buffers are consumed (can be NULL).
 * @name: the name of this virtqueue (mainly for debugging)
 * @vdev: the virtio device this queue was created for.
<<<<<<< HEAD
 * @vq_ops: the operations for this virtqueue (see below).
 * @priv: a pointer for the virtqueue implementation to use.
=======
 * @priv: a pointer for the virtqueue implementation to use.
 * @index: the zero-based ordinal number for this queue.
 * @num_free: number of elements we expect to be able to fit.
 * @num_max: the maximum number of elements supported by the device.
 * @reset: vq is in reset state or not.
 *
 * A note on @num_free: with indirect buffers, each buffer needs one
 * element in the queue, otherwise a buffer will need one element per
 * sg element.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct virtqueue {
	struct list_head list;
	void (*callback)(struct virtqueue *vq);
	const char *name;
	struct virtio_device *vdev;
<<<<<<< HEAD
	struct virtqueue_ops *vq_ops;
	void *priv;
};

/**
 * virtqueue_ops - operations for virtqueue abstraction layer
 * @add_buf: expose buffer to other end
 *	vq: the struct virtqueue we're talking about.
 *	sg: the description of the buffer(s).
 *	out_num: the number of sg readable by other side
 *	in_num: the number of sg which are writable (after readable ones)
 *	data: the token identifying the buffer.
 *	Returns remaining capacity of queue (sg segments) or a negative error.
 * @kick: update after add_buf
 *	vq: the struct virtqueue
 *	After one or more add_buf calls, invoke this to kick the other side.
 * @get_buf: get the next used buffer
 *	vq: the struct virtqueue we're talking about.
 *	len: the length written into the buffer
 *	Returns NULL or the "data" token handed to add_buf.
 * @disable_cb: disable callbacks
 *	vq: the struct virtqueue we're talking about.
 *	Note that this is not necessarily synchronous, hence unreliable and only
 *	useful as an optimization.
 * @enable_cb: restart callbacks after disable_cb.
 *	vq: the struct virtqueue we're talking about.
 *	This re-enables callbacks; it returns "false" if there are pending
 *	buffers in the queue, to detect a possible race between the driver
 *	checking for more work, and enabling callbacks.
 * @enable_cb_delayed: restart callbacks after disable_cb.
 *	vq: the struct virtqueue we're talking about.
 *	This re-enables callbacks but hints to the other side to delay
 *	interrupts until most of the available buffers have been processed;
 *	it returns "false" if there are many pending buffers in the queue,
 *	to detect a possible race between the driver checking for more work,
 *	and enabling callbacks.
 *	Caller must ensure we don't call this with other virtqueue
 *	operations at the same time (except where noted).
 * @detach_unused_buf: detach first unused buffer
 *	vq: the struct virtqueue we're talking about.
 *	Returns NULL or the "data" token handed to add_buf
 * @get_impl_size: return the size of the virtqueue's implementation
 *	vq: the struct virtqueue containing the implementation of interest.
 *	Returns the size of the implementation. This is mainly used for
 *	boasting to userspace. Unlike other operations, this need not
 *	be serialized.
 *
 * Locking rules are straightforward: the driver is responsible for
 * locking.  No two operations may be invoked simultaneously, with the exception
 * of @disable_cb.
 *
 * All operations can be called in any context.
 */
struct virtqueue_ops {
	int (*add_buf)(struct virtqueue *vq,
		       struct scatterlist sg[],
		       unsigned int out_num,
		       unsigned int in_num,
		       void *data,
		       gfp_t gfp);

	void (*kick)(struct virtqueue *vq);
	bool (*kick_prepare)(struct virtqueue *vq);
	void (*kick_notify)(struct virtqueue *vq);
	void *(*get_buf)(struct virtqueue *vq, unsigned int *len);
	void (*disable_cb)(struct virtqueue *vq);
	bool (*enable_cb)(struct virtqueue *vq);
	bool (*enable_cb_delayed)(struct virtqueue *vq);
	void *(*detach_unused_buf)(struct virtqueue *vq);
	unsigned int (*get_impl_size)(struct virtqueue *vq);
};

/**
 * virtqueue_add_buf - expose buffer to other end
 * @vq: the struct virtqueue we're talking about.
 * @sg: the description of the buffer(s).
 * @out_num: the number of sg readable by other side
 * @in_num: the number of sg which are writable (after readable ones)
 * @data: the token identifying the buffer.
 * @gfp: how to do memory allocations (if necessary).
 *
 * Caller must ensure we don't call this with other virtqueue operations
 * at the same time (except where noted).
 *
 * Returns remaining capacity of queue or a negative error.
 */
static inline int virtqueue_add_buf(struct virtqueue *vq,
				struct scatterlist sg[],
				unsigned int out_num,
				unsigned int in_num,
				void *data,
				gfp_t gfp)
{
	return vq->vq_ops->add_buf(vq, sg, out_num, in_num, data, gfp);
}
/**
 * virtqueue_kick - update after add_buf
 * @vq: the struct virtqueue
 *
 * After one or more virtqueue_add_buf calls, invoke this to kick
 * the other side.
 *
 * Caller must ensure we don't call this with other virtqueue
 * operations at the same time (except where noted).
 */
static inline void virtqueue_kick(struct virtqueue *vq)
{
	vq->vq_ops->kick(vq);
}

/**
 * virtqueue_kick_prepare - first half of split virtqueue_kick call.
 * @vq: the struct virtqueue
 *
 * Instead of virtqueue_kick(), you can do:
 *	if (virtqueue_kick_prepare(vq))
 *		virtqueue_kick_notify(vq);
 *
 * This is sometimes useful because the virtqueue_kick_prepare() needs
 * to be serialized, but the actual virtqueue_kick_notify() call does not.
 */
static inline bool virtqueue_kick_prepare(struct virtqueue *vq)
{
	return vq->vq_ops->kick_prepare(vq);
}

/**
 * virtqueue_kick_notify - second half of split virtqueue_kick call.
 * @vq: the struct virtqueue
 */
static inline void virtqueue_kick_notify(struct virtqueue *vq)
{
	vq->vq_ops->kick_notify(vq);
}

/**
 * virtqueue_get_buf - get the next used buffer
 * @vq: the struct virtqueue we're talking about.
 * @len: the length written into the buffer
 *
 * If the driver wrote data into the buffer, @len will be set to the
 * amount written.  This means you don't need to clear the buffer
 * beforehand to ensure there's no data leakage in the case of short
 * writes.
 *
 * Caller must ensure we don't call this with other virtqueue
 * operations at the same time (except where noted).
 *
 * Returns NULL if there are no used buffers, or the "data" token
 * handed to virtqueue_add_buf().
 */
static inline void *virtqueue_get_buf(struct virtqueue *vq, unsigned int *len)
{
	return vq->vq_ops->get_buf(vq, len);
}

/**
 * virtqueue_disable_cb - disable callbacks
 * @vq: the struct virtqueue we're talking about.
 *
 * Note that this is not necessarily synchronous, hence unreliable and only
 * useful as an optimization.
 *
 * Unlike other operations, this need not be serialized.
 */
static inline void virtqueue_disable_cb(struct virtqueue *vq)
{
	vq->vq_ops->disable_cb(vq);
}

/**
 * virtqueue_enable_cb - restart callbacks after disable_cb.
 * @vq: the struct virtqueue we're talking about.
 *
 * This re-enables callbacks; it returns "false" if there are pending
 * buffers in the queue, to detect a possible race between the driver
 * checking for more work, and enabling callbacks.
 *
 * Caller must ensure we don't call this with other virtqueue
 * operations at the same time (except where noted).
 */
static inline bool virtqueue_enable_cb(struct virtqueue *vq)
{
	return vq->vq_ops->enable_cb(vq);
}

/**
 * virtqueue_enable_cb_delayed - restart callbacks after disable_cb.
 * @vq: the struct virtqueue we're talking about.
 *
 * This re-enables callbacks but hints to the other side to delay
 * interrupts until most of the available buffers have been processed;
 * it returns "false" if there are many pending buffers in the queue,
 * to detect a possible race between the driver checking for more work,
 * and enabling callbacks.
 *
 * Caller must ensure we don't call this with other virtqueue
 * operations at the same time (except where noted).
 */
static inline bool virtqueue_enable_cb_delayed(struct virtqueue *vq)
{
	return vq->vq_ops->enable_cb_delayed(vq);
}

/**
 * virtqueue_detach_unused_buf - detach first unused buffer
 * @vq: the struct virtqueue we're talking about.
 *
 * Returns NULL or the "data" token handed to virtqueue_add_buf().
 * This is not valid on an active queue; it is useful only for device
 * shutdown.
 */
static inline void *virtqueue_detach_unused_buf(struct virtqueue *vq)
{
	return vq->vq_ops->detach_unused_buf(vq);
}

/**
 * virtqueue_get_impl_size - return the size of the virtqueue's implementation
 * @vq: the struct virtqueue containing the implementation of interest.
 *
 * Returns the size of the virtqueue implementation.  This is mainly used
 * for boasting to userspace.  Unlike other operations, this need not
 * be serialized.
 */
static inline unsigned int virtqueue_get_impl_size(struct virtqueue *vq)
{
	return vq->vq_ops->get_impl_size(vq);
}

/**
 * virtio_device - representation of a device using virtio
 * @index: unique position on the virtio bus
 * @dev: underlying device.
 * @id: the device type identification (used to match it with a driver).
 * @config: the configuration ops for this device.
=======
	unsigned int index;
	unsigned int num_free;
	unsigned int num_max;
	bool reset;
	void *priv;
};

int virtqueue_add_outbuf(struct virtqueue *vq,
			 struct scatterlist sg[], unsigned int num,
			 void *data,
			 gfp_t gfp);

int virtqueue_add_inbuf(struct virtqueue *vq,
			struct scatterlist sg[], unsigned int num,
			void *data,
			gfp_t gfp);

int virtqueue_add_inbuf_ctx(struct virtqueue *vq,
			    struct scatterlist sg[], unsigned int num,
			    void *data,
			    void *ctx,
			    gfp_t gfp);

int virtqueue_add_sgs(struct virtqueue *vq,
		      struct scatterlist *sgs[],
		      unsigned int out_sgs,
		      unsigned int in_sgs,
		      void *data,
		      gfp_t gfp);

struct device *virtqueue_dma_dev(struct virtqueue *vq);

bool virtqueue_kick(struct virtqueue *vq);

bool virtqueue_kick_prepare(struct virtqueue *vq);

bool virtqueue_notify(struct virtqueue *vq);

void *virtqueue_get_buf(struct virtqueue *vq, unsigned int *len);

void *virtqueue_get_buf_ctx(struct virtqueue *vq, unsigned int *len,
			    void **ctx);

void virtqueue_disable_cb(struct virtqueue *vq);

bool virtqueue_enable_cb(struct virtqueue *vq);

unsigned virtqueue_enable_cb_prepare(struct virtqueue *vq);

int virtqueue_set_dma_premapped(struct virtqueue *_vq);

bool virtqueue_poll(struct virtqueue *vq, unsigned);

bool virtqueue_enable_cb_delayed(struct virtqueue *vq);

void *virtqueue_detach_unused_buf(struct virtqueue *vq);

unsigned int virtqueue_get_vring_size(const struct virtqueue *vq);

bool virtqueue_is_broken(const struct virtqueue *vq);

const struct vring *virtqueue_get_vring(const struct virtqueue *vq);
dma_addr_t virtqueue_get_desc_addr(const struct virtqueue *vq);
dma_addr_t virtqueue_get_avail_addr(const struct virtqueue *vq);
dma_addr_t virtqueue_get_used_addr(const struct virtqueue *vq);

int virtqueue_resize(struct virtqueue *vq, u32 num,
		     void (*recycle)(struct virtqueue *vq, void *buf));
int virtqueue_reset(struct virtqueue *vq,
		    void (*recycle)(struct virtqueue *vq, void *buf));

struct virtio_admin_cmd {
	__le16 opcode;
	__le16 group_type;
	__le64 group_member_id;
	struct scatterlist *data_sg;
	struct scatterlist *result_sg;
};

/**
 * struct virtio_device - representation of a device using virtio
 * @index: unique position on the virtio bus
 * @failed: saved value for VIRTIO_CONFIG_S_FAILED bit (for restore)
 * @config_enabled: configuration change reporting enabled
 * @config_change_pending: configuration change reported while disabled
 * @config_lock: protects configuration change reporting
 * @vqs_list_lock: protects @vqs.
 * @dev: underlying device.
 * @id: the device type identification (used to match it with a driver).
 * @config: the configuration ops for this device.
 * @vringh_config: configuration ops for host vrings.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @vqs: the list of virtqueues for this device.
 * @features: the features supported by both driver and device.
 * @priv: private pointer for the driver's use.
 */
struct virtio_device {
	int index;
<<<<<<< HEAD
	struct device dev;
	struct virtio_device_id id;
	struct virtio_config_ops *config;
	struct list_head vqs;
	/* Note that this is a Linux set_bit-style bitmap. */
	unsigned long features[1];
	void *priv;
};

static inline struct virtio_device *dev_to_virtio(struct device *_dev)
{
	return container_of(_dev, struct virtio_device, dev);
}

int register_virtio_device(struct virtio_device *dev);
void unregister_virtio_device(struct virtio_device *dev);

/**
 * virtio_driver - operations for a virtio I/O driver
 * @driver: underlying device driver (populate name and owner).
 * @id_table: the ids serviced by this driver.
 * @feature_table: an array of feature numbers supported by this driver.
 * @feature_table_size: number of entries in the feature table array.
 * @probe: the function to call when a device is found.  Returns 0 or -errno.
 * @remove: the function to call when a device is removed.
 * @config_changed: optional function to call when the device configuration
 *    changes; may be called in interrupt context.
=======
	bool failed;
	bool config_enabled;
	bool config_change_pending;
	spinlock_t config_lock;
	spinlock_t vqs_list_lock;
	struct device dev;
	struct virtio_device_id id;
	const struct virtio_config_ops *config;
	const struct vringh_config_ops *vringh_config;
	struct list_head vqs;
	u64 features;
	void *priv;
};

#define dev_to_virtio(_dev)	container_of_const(_dev, struct virtio_device, dev)

void virtio_add_status(struct virtio_device *dev, unsigned int status);
int register_virtio_device(struct virtio_device *dev);
void unregister_virtio_device(struct virtio_device *dev);
bool is_virtio_device(struct device *dev);

void virtio_break_device(struct virtio_device *dev);
void __virtio_unbreak_device(struct virtio_device *dev);

void __virtqueue_break(struct virtqueue *_vq);
void __virtqueue_unbreak(struct virtqueue *_vq);

void virtio_config_changed(struct virtio_device *dev);
#ifdef CONFIG_PM_SLEEP
int virtio_device_freeze(struct virtio_device *dev);
int virtio_device_restore(struct virtio_device *dev);
#endif
void virtio_reset_device(struct virtio_device *dev);

size_t virtio_max_dma_size(const struct virtio_device *vdev);

#define virtio_device_for_each_vq(vdev, vq) \
	list_for_each_entry(vq, &vdev->vqs, list)

/**
 * struct virtio_driver - operations for a virtio I/O driver
 * @driver: underlying device driver (populate name).
 * @id_table: the ids serviced by this driver.
 * @feature_table: an array of feature numbers supported by this driver.
 * @feature_table_size: number of entries in the feature table array.
 * @feature_table_legacy: same as feature_table but when working in legacy mode.
 * @feature_table_size_legacy: number of entries in feature table legacy array.
 * @validate: the function to call to validate features and config space.
 *            Returns 0 or -errno.
 * @probe: the function to call when a device is found.  Returns 0 or -errno.
 * @scan: optional function to call after successful probe; intended
 *    for virtio-scsi to invoke a scan.
 * @remove: the function to call when a device is removed.
 * @config_changed: optional function to call when the device configuration
 *    changes; may be called in interrupt context.
 * @freeze: optional function to call during suspend/hibernation.
 * @restore: optional function to call on resume.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct virtio_driver {
	struct device_driver driver;
	const struct virtio_device_id *id_table;
	const unsigned int *feature_table;
	unsigned int feature_table_size;
<<<<<<< HEAD
	int (*probe)(struct virtio_device *dev);
	void (*remove)(struct virtio_device *dev);
	void (*config_changed)(struct virtio_device *dev);
#ifdef CONFIG_PM
	int (*freeze)(struct virtio_device *dev);
	int (*restore)(struct virtio_device *dev);
#endif
};

int register_virtio_driver(struct virtio_driver *drv);
void unregister_virtio_driver(struct virtio_driver *drv);
=======
	const unsigned int *feature_table_legacy;
	unsigned int feature_table_size_legacy;
	int (*validate)(struct virtio_device *dev);
	int (*probe)(struct virtio_device *dev);
	void (*scan)(struct virtio_device *dev);
	void (*remove)(struct virtio_device *dev);
	void (*config_changed)(struct virtio_device *dev);
	int (*freeze)(struct virtio_device *dev);
	int (*restore)(struct virtio_device *dev);
};

static inline struct virtio_driver *drv_to_virtio(struct device_driver *drv)
{
	return container_of(drv, struct virtio_driver, driver);
}

/* use a macro to avoid include chaining to get THIS_MODULE */
#define register_virtio_driver(drv) \
	__register_virtio_driver(drv, THIS_MODULE)
int __register_virtio_driver(struct virtio_driver *drv, struct module *owner);
void unregister_virtio_driver(struct virtio_driver *drv);

/* module_virtio_driver() - Helper macro for drivers that don't do
 * anything special in module init/exit.  This eliminates a lot of
 * boilerplate.  Each module may only use this macro once, and
 * calling it replaces module_init() and module_exit()
 */
#define module_virtio_driver(__virtio_driver) \
	module_driver(__virtio_driver, register_virtio_driver, \
			unregister_virtio_driver)

dma_addr_t virtqueue_dma_map_single_attrs(struct virtqueue *_vq, void *ptr, size_t size,
					  enum dma_data_direction dir, unsigned long attrs);
void virtqueue_dma_unmap_single_attrs(struct virtqueue *_vq, dma_addr_t addr,
				      size_t size, enum dma_data_direction dir,
				      unsigned long attrs);
int virtqueue_dma_mapping_error(struct virtqueue *_vq, dma_addr_t addr);

bool virtqueue_dma_need_sync(struct virtqueue *_vq, dma_addr_t addr);
void virtqueue_dma_sync_single_range_for_cpu(struct virtqueue *_vq, dma_addr_t addr,
					     unsigned long offset, size_t size,
					     enum dma_data_direction dir);
void virtqueue_dma_sync_single_range_for_device(struct virtqueue *_vq, dma_addr_t addr,
						unsigned long offset, size_t size,
						enum dma_data_direction dir);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_VIRTIO_H */
