<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _VHOST_H
#define _VHOST_H

#include <linux/eventfd.h>
#include <linux/vhost.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/file.h>
<<<<<<< HEAD
#include <linux/skbuff.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/uio.h>
#include <linux/virtio_config.h>
#include <linux/virtio_ring.h>
#include <linux/atomic.h>
<<<<<<< HEAD

/* This is for zerocopy, used buffer len is set to 1 when lower device DMA
 * done */
#define VHOST_DMA_DONE_LEN	1
#define VHOST_DMA_CLEAR_LEN	0

struct vhost_device;

struct vhost_work;
typedef void (*vhost_work_fn_t)(struct vhost_work *work);

struct vhost_work {
	struct list_head	  node;
	vhost_work_fn_t		  fn;
	wait_queue_head_t	  done;
	int			  flushing;
	unsigned		  queue_seq;
	unsigned		  done_seq;
=======
#include <linux/vhost_iotlb.h>
#include <linux/irqbypass.h>

struct vhost_work;
struct vhost_task;
typedef void (*vhost_work_fn_t)(struct vhost_work *work);

#define VHOST_WORK_QUEUED 1
struct vhost_work {
	struct llist_node	node;
	vhost_work_fn_t		fn;
	unsigned long		flags;
};

struct vhost_worker {
	struct vhost_task	*vtsk;
	/* Used to serialize device wide flushing with worker swapping. */
	struct mutex		mutex;
	struct llist_head	work_list;
	u64			kcov_handle;
	u32			id;
	int			attachment_cnt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Poll a file (eventfd or socket) */
/* Note: there's nothing vhost specific about this structure. */
struct vhost_poll {
<<<<<<< HEAD
	poll_table                table;
	wait_queue_head_t        *wqh;
	wait_queue_t              wait;
	struct vhost_work	  work;
	unsigned long		  mask;
	struct vhost_dev	 *dev;
};

void vhost_poll_init(struct vhost_poll *poll, vhost_work_fn_t fn,
		     unsigned long mask, struct vhost_dev *dev);
void vhost_poll_start(struct vhost_poll *poll, struct file *file);
void vhost_poll_stop(struct vhost_poll *poll);
void vhost_poll_flush(struct vhost_poll *poll);
void vhost_poll_queue(struct vhost_poll *poll);

=======
	poll_table		table;
	wait_queue_head_t	*wqh;
	wait_queue_entry_t	wait;
	struct vhost_work	work;
	__poll_t		mask;
	struct vhost_dev	*dev;
	struct vhost_virtqueue	*vq;
};

void vhost_poll_init(struct vhost_poll *poll, vhost_work_fn_t fn,
		     __poll_t mask, struct vhost_dev *dev,
		     struct vhost_virtqueue *vq);
int vhost_poll_start(struct vhost_poll *poll, struct file *file);
void vhost_poll_stop(struct vhost_poll *poll);
void vhost_poll_queue(struct vhost_poll *poll);

void vhost_work_init(struct vhost_work *work, vhost_work_fn_t fn);
void vhost_dev_flush(struct vhost_dev *dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct vhost_log {
	u64 addr;
	u64 len;
};

<<<<<<< HEAD
struct vhost_virtqueue;

struct vhost_ubuf_ref {
	struct kref kref;
	wait_queue_head_t wait;
	struct vhost_virtqueue *vq;
};

struct vhost_ubuf_ref *vhost_ubuf_alloc(struct vhost_virtqueue *, bool zcopy);
void vhost_ubuf_put(struct vhost_ubuf_ref *);
void vhost_ubuf_put_and_wait(struct vhost_ubuf_ref *);
=======
enum vhost_uaddr_type {
	VHOST_ADDR_DESC = 0,
	VHOST_ADDR_AVAIL = 1,
	VHOST_ADDR_USED = 2,
	VHOST_NUM_ADDRS = 3,
};

struct vhost_vring_call {
	struct eventfd_ctx *ctx;
	struct irq_bypass_producer producer;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* The virtqueue structure describes a queue attached to a device. */
struct vhost_virtqueue {
	struct vhost_dev *dev;
<<<<<<< HEAD
=======
	struct vhost_worker __rcu *worker;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* The actual ring of buffers. */
	struct mutex mutex;
	unsigned int num;
<<<<<<< HEAD
	struct vring_desc __user *desc;
	struct vring_avail __user *avail;
	struct vring_used __user *used;
	struct file *kick;
	struct file *call;
	struct file *error;
	struct eventfd_ctx *call_ctx;
=======
	vring_desc_t __user *desc;
	vring_avail_t __user *avail;
	vring_used_t __user *used;
	const struct vhost_iotlb_map *meta_iotlb[VHOST_NUM_ADDRS];
	struct file *kick;
	struct vhost_vring_call call_ctx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct eventfd_ctx *error_ctx;
	struct eventfd_ctx *log_ctx;

	struct vhost_poll poll;

	/* The routine to call when the Guest pings us, or timeout. */
	vhost_work_fn_t handle_kick;

<<<<<<< HEAD
	/* Last available index we saw. */
=======
	/* Last available index we saw.
	 * Values are limited to 0x7fff, and the high bit is used as
	 * a wrap counter when using VIRTIO_F_RING_PACKED. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 last_avail_idx;

	/* Caches available index value from user. */
	u16 avail_idx;

<<<<<<< HEAD
	/* Last index we used. */
=======
	/* Last index we used.
	 * Values are limited to 0x7fff, and the high bit is used as
	 * a wrap counter when using VIRTIO_F_RING_PACKED. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 last_used_idx;

	/* Used flags */
	u16 used_flags;

	/* Last used index value we have signalled on */
	u16 signalled_used;

	/* Last used index value we have signalled on */
	bool signalled_used_valid;

	/* Log writes to used structure. */
	bool log_used;
	u64 log_addr;

	struct iovec iov[UIO_MAXIOV];
<<<<<<< HEAD
	/* hdr is used to store the virtio header.
	 * Since each iovec has >= 1 byte length, we never need more than
	 * header length entries to store the header. */
	struct iovec hdr[sizeof(struct virtio_net_hdr_mrg_rxbuf)];
	struct iovec *indirect;
	size_t vhost_hlen;
	size_t sock_hlen;
	struct vring_used_elem *heads;
	/* We use a kind of RCU to access private pointer.
	 * All readers access it from worker, which makes it possible to
	 * flush the vhost_work instead of synchronize_rcu. Therefore readers do
	 * not need to call rcu_read_lock/rcu_read_unlock: the beginning of
	 * vhost_work execution acts instead of rcu_read_lock() and the end of
	 * vhost_work execution acts instead of rcu_read_unlock().
	 * Writers use virtqueue mutex. */
	void __rcu *private_data;
	/* Log write descriptors */
	void __user *log_base;
	struct vhost_log *log;
	/* vhost zerocopy support fields below: */
	/* last used idx for outstanding DMA zerocopy buffers */
	int upend_idx;
	/* first used idx for DMA done zerocopy buffers */
	int done_idx;
	/* an array of userspace buffers info */
	struct ubuf_info *ubuf_info;
	/* Reference counting for outstanding ubufs.
	 * Protected by vq mutex. Writers must also take device mutex. */
	struct vhost_ubuf_ref *ubufs;
};

struct vhost_dev {
	/* Readers use RCU to access memory table pointer
	 * log base pointer and features.
	 * Writers use mutex below.*/
	struct vhost_memory __rcu *memory;
	struct mm_struct *mm;
	struct mutex mutex;
	unsigned acked_features;
	struct vhost_virtqueue *vqs;
	int nvqs;
	struct file *log_file;
	struct eventfd_ctx *log_ctx;
	spinlock_t work_lock;
	struct list_head work_list;
	struct task_struct *worker;
};

long vhost_dev_init(struct vhost_dev *, struct vhost_virtqueue *vqs, int nvqs);
long vhost_dev_check_owner(struct vhost_dev *);
long vhost_dev_reset_owner(struct vhost_dev *);
void vhost_dev_cleanup(struct vhost_dev *, bool locked);
long vhost_dev_ioctl(struct vhost_dev *, unsigned int ioctl, unsigned long arg);
int vhost_vq_access_ok(struct vhost_virtqueue *vq);
int vhost_log_access_ok(struct vhost_dev *);

int vhost_get_vq_desc(struct vhost_dev *, struct vhost_virtqueue *,
		      struct iovec iov[], unsigned int iov_count,
=======
	struct iovec iotlb_iov[64];
	struct iovec *indirect;
	struct vring_used_elem *heads;
	/* Protected by virtqueue mutex. */
	struct vhost_iotlb *umem;
	struct vhost_iotlb *iotlb;
	void *private_data;
	u64 acked_features;
	u64 acked_backend_features;
	/* Log write descriptors */
	void __user *log_base;
	struct vhost_log *log;
	struct iovec log_iov[64];

	/* Ring endianness. Defaults to legacy native endianness.
	 * Set to true when starting a modern virtio device. */
	bool is_le;
#ifdef CONFIG_VHOST_CROSS_ENDIAN_LEGACY
	/* Ring endianness requested by userspace for cross-endian support. */
	bool user_be;
#endif
	u32 busyloop_timeout;
};

struct vhost_msg_node {
  union {
	  struct vhost_msg msg;
	  struct vhost_msg_v2 msg_v2;
  };
  struct vhost_virtqueue *vq;
  struct list_head node;
};

struct vhost_dev {
	struct mm_struct *mm;
	struct mutex mutex;
	struct vhost_virtqueue **vqs;
	int nvqs;
	struct eventfd_ctx *log_ctx;
	struct vhost_iotlb *umem;
	struct vhost_iotlb *iotlb;
	spinlock_t iotlb_lock;
	struct list_head read_list;
	struct list_head pending_list;
	wait_queue_head_t wait;
	int iov_limit;
	int weight;
	int byte_weight;
	struct xarray worker_xa;
	bool use_worker;
	int (*msg_handler)(struct vhost_dev *dev, u32 asid,
			   struct vhost_iotlb_msg *msg);
};

bool vhost_exceeds_weight(struct vhost_virtqueue *vq, int pkts, int total_len);
void vhost_dev_init(struct vhost_dev *, struct vhost_virtqueue **vqs,
		    int nvqs, int iov_limit, int weight, int byte_weight,
		    bool use_worker,
		    int (*msg_handler)(struct vhost_dev *dev, u32 asid,
				       struct vhost_iotlb_msg *msg));
long vhost_dev_set_owner(struct vhost_dev *dev);
bool vhost_dev_has_owner(struct vhost_dev *dev);
long vhost_dev_check_owner(struct vhost_dev *);
struct vhost_iotlb *vhost_dev_reset_owner_prepare(void);
void vhost_dev_reset_owner(struct vhost_dev *dev, struct vhost_iotlb *iotlb);
void vhost_dev_cleanup(struct vhost_dev *);
void vhost_dev_stop(struct vhost_dev *);
long vhost_dev_ioctl(struct vhost_dev *, unsigned int ioctl, void __user *argp);
long vhost_vring_ioctl(struct vhost_dev *d, unsigned int ioctl, void __user *argp);
long vhost_worker_ioctl(struct vhost_dev *dev, unsigned int ioctl,
			void __user *argp);
bool vhost_vq_access_ok(struct vhost_virtqueue *vq);
bool vhost_log_access_ok(struct vhost_dev *);
void vhost_clear_msg(struct vhost_dev *dev);

int vhost_get_vq_desc(struct vhost_virtqueue *,
		      struct iovec iov[], unsigned int iov_size,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		      unsigned int *out_num, unsigned int *in_num,
		      struct vhost_log *log, unsigned int *log_num);
void vhost_discard_vq_desc(struct vhost_virtqueue *, int n);

<<<<<<< HEAD
int vhost_init_used(struct vhost_virtqueue *);
=======
void vhost_vq_flush(struct vhost_virtqueue *vq);
bool vhost_vq_work_queue(struct vhost_virtqueue *vq, struct vhost_work *work);
bool vhost_vq_has_work(struct vhost_virtqueue *vq);
bool vhost_vq_is_setup(struct vhost_virtqueue *vq);
int vhost_vq_init_access(struct vhost_virtqueue *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int vhost_add_used(struct vhost_virtqueue *, unsigned int head, int len);
int vhost_add_used_n(struct vhost_virtqueue *, struct vring_used_elem *heads,
		     unsigned count);
void vhost_add_used_and_signal(struct vhost_dev *, struct vhost_virtqueue *,
			       unsigned int id, int len);
void vhost_add_used_and_signal_n(struct vhost_dev *, struct vhost_virtqueue *,
			       struct vring_used_elem *heads, unsigned count);
void vhost_signal(struct vhost_dev *, struct vhost_virtqueue *);
void vhost_disable_notify(struct vhost_dev *, struct vhost_virtqueue *);
<<<<<<< HEAD
bool vhost_enable_notify(struct vhost_dev *, struct vhost_virtqueue *);

int vhost_log_write(struct vhost_virtqueue *vq, struct vhost_log *log,
		    unsigned int log_num, u64 len);
void vhost_zerocopy_callback(struct ubuf_info *);
int vhost_zerocopy_signal_used(struct vhost_virtqueue *vq);
=======
bool vhost_vq_avail_empty(struct vhost_dev *, struct vhost_virtqueue *);
bool vhost_enable_notify(struct vhost_dev *, struct vhost_virtqueue *);

int vhost_log_write(struct vhost_virtqueue *vq, struct vhost_log *log,
		    unsigned int log_num, u64 len,
		    struct iovec *iov, int count);
int vq_meta_prefetch(struct vhost_virtqueue *vq);

struct vhost_msg_node *vhost_new_msg(struct vhost_virtqueue *vq, int type);
void vhost_enqueue_msg(struct vhost_dev *dev,
		       struct list_head *head,
		       struct vhost_msg_node *node);
struct vhost_msg_node *vhost_dequeue_msg(struct vhost_dev *dev,
					 struct list_head *head);
void vhost_set_backend_features(struct vhost_dev *dev, u64 features);

__poll_t vhost_chr_poll(struct file *file, struct vhost_dev *dev,
			    poll_table *wait);
ssize_t vhost_chr_read_iter(struct vhost_dev *dev, struct iov_iter *to,
			    int noblock);
ssize_t vhost_chr_write_iter(struct vhost_dev *dev,
			     struct iov_iter *from);
int vhost_init_device_iotlb(struct vhost_dev *d);

void vhost_iotlb_map_free(struct vhost_iotlb *iotlb,
			  struct vhost_iotlb_map *map);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define vq_err(vq, fmt, ...) do {                                  \
		pr_debug(pr_fmt(fmt), ##__VA_ARGS__);       \
		if ((vq)->error_ctx)                               \
<<<<<<< HEAD
				eventfd_signal((vq)->error_ctx, 1);\
=======
				eventfd_signal((vq)->error_ctx);\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (0)

enum {
	VHOST_FEATURES = (1ULL << VIRTIO_F_NOTIFY_ON_EMPTY) |
			 (1ULL << VIRTIO_RING_F_INDIRECT_DESC) |
			 (1ULL << VIRTIO_RING_F_EVENT_IDX) |
			 (1ULL << VHOST_F_LOG_ALL) |
<<<<<<< HEAD
			 (1ULL << VHOST_NET_F_VIRTIO_NET_HDR) |
			 (1ULL << VIRTIO_NET_F_MRG_RXBUF),
};

static inline int vhost_has_feature(struct vhost_dev *dev, int bit)
{
	unsigned acked_features;

	/* TODO: check that we are running from vhost_worker or dev mutex is
	 * held? */
	acked_features = rcu_dereference_index_check(dev->acked_features, 1);
	return acked_features & (1 << bit);
}

void vhost_enable_zcopy(int vq);

=======
			 (1ULL << VIRTIO_F_ANY_LAYOUT) |
			 (1ULL << VIRTIO_F_VERSION_1)
};

/**
 * vhost_vq_set_backend - Set backend.
 *
 * @vq            Virtqueue.
 * @private_data  The private data.
 *
 * Context: Need to call with vq->mutex acquired.
 */
static inline void vhost_vq_set_backend(struct vhost_virtqueue *vq,
					void *private_data)
{
	vq->private_data = private_data;
}

/**
 * vhost_vq_get_backend - Get backend.
 *
 * @vq            Virtqueue.
 *
 * Context: Need to call with vq->mutex acquired.
 * Return: Private data previously set with vhost_vq_set_backend.
 */
static inline void *vhost_vq_get_backend(struct vhost_virtqueue *vq)
{
	return vq->private_data;
}

static inline bool vhost_has_feature(struct vhost_virtqueue *vq, int bit)
{
	return vq->acked_features & (1ULL << bit);
}

static inline bool vhost_backend_has_feature(struct vhost_virtqueue *vq, int bit)
{
	return vq->acked_backend_features & (1ULL << bit);
}

#ifdef CONFIG_VHOST_CROSS_ENDIAN_LEGACY
static inline bool vhost_is_little_endian(struct vhost_virtqueue *vq)
{
	return vq->is_le;
}
#else
static inline bool vhost_is_little_endian(struct vhost_virtqueue *vq)
{
	return virtio_legacy_is_little_endian() || vq->is_le;
}
#endif

/* Memory accessors */
static inline u16 vhost16_to_cpu(struct vhost_virtqueue *vq, __virtio16 val)
{
	return __virtio16_to_cpu(vhost_is_little_endian(vq), val);
}

static inline __virtio16 cpu_to_vhost16(struct vhost_virtqueue *vq, u16 val)
{
	return __cpu_to_virtio16(vhost_is_little_endian(vq), val);
}

static inline u32 vhost32_to_cpu(struct vhost_virtqueue *vq, __virtio32 val)
{
	return __virtio32_to_cpu(vhost_is_little_endian(vq), val);
}

static inline __virtio32 cpu_to_vhost32(struct vhost_virtqueue *vq, u32 val)
{
	return __cpu_to_virtio32(vhost_is_little_endian(vq), val);
}

static inline u64 vhost64_to_cpu(struct vhost_virtqueue *vq, __virtio64 val)
{
	return __virtio64_to_cpu(vhost_is_little_endian(vq), val);
}

static inline __virtio64 cpu_to_vhost64(struct vhost_virtqueue *vq, u64 val)
{
	return __cpu_to_virtio64(vhost_is_little_endian(vq), val);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
