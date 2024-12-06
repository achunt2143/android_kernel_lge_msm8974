/*
 * Copyright (c) 2004 Topspin Communications.  All rights reserved.
 * Copyright (c) 2005 Voltaire, Inc. All rights reserved.
 * Copyright (c) 2005 Sun Microsystems, Inc. All rights reserved.
 * Copyright (c) 2008 Cisco. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) "user_mad: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/dma-mapping.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/kref.h>
#include <linux/compat.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
<<<<<<< HEAD

#include <asm/uaccess.h>

#include <rdma/ib_mad.h>
#include <rdma/ib_user_mad.h>
=======
#include <linux/nospec.h>

#include <linux/uaccess.h>

#include <rdma/ib_mad.h>
#include <rdma/ib_user_mad.h>
#include <rdma/rdma_netlink.h>

#include "core_priv.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Roland Dreier");
MODULE_DESCRIPTION("InfiniBand userspace MAD packet access");
MODULE_LICENSE("Dual BSD/GPL");

enum {
<<<<<<< HEAD
	IB_UMAD_MAX_PORTS  = 64,
	IB_UMAD_MAX_AGENTS = 32,

	IB_UMAD_MAJOR      = 231,
	IB_UMAD_MINOR_BASE = 0
=======
	IB_UMAD_MAX_PORTS  = RDMA_MAX_PORTS,
	IB_UMAD_MAX_AGENTS = 32,

	IB_UMAD_MAJOR      = 231,
	IB_UMAD_MINOR_BASE = 0,
	IB_UMAD_NUM_FIXED_MINOR = 64,
	IB_UMAD_NUM_DYNAMIC_MINOR = IB_UMAD_MAX_PORTS - IB_UMAD_NUM_FIXED_MINOR,
	IB_ISSM_MINOR_BASE        = IB_UMAD_NUM_FIXED_MINOR,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Our lifetime rules for these structs are the following:
 * device special file is opened, we take a reference on the
 * ib_umad_port's struct ib_umad_device. We drop these
 * references in the corresponding close().
 *
 * In addition to references coming from open character devices, there
 * is one more reference to each ib_umad_device representing the
 * module's reference taken when allocating the ib_umad_device in
 * ib_umad_add_one().
 *
 * When destroying an ib_umad_device, we drop the module's reference.
 */

struct ib_umad_port {
	struct cdev           cdev;
<<<<<<< HEAD
	struct device	      *dev;

	struct cdev           sm_cdev;
	struct device	      *sm_dev;
=======
	struct device	      dev;
	struct cdev           sm_cdev;
	struct device	      sm_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct semaphore       sm_sem;

	struct mutex	       file_mutex;
	struct list_head       file_list;

	struct ib_device      *ib_dev;
	struct ib_umad_device *umad_dev;
	int                    dev_num;
<<<<<<< HEAD
	u8                     port_num;
};

struct ib_umad_device {
	int                  start_port, end_port;
	struct kobject       kobj;
	struct ib_umad_port  port[0];
=======
	u32                     port_num;
};

struct ib_umad_device {
	struct kref kref;
	struct ib_umad_port ports[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct ib_umad_file {
	struct mutex		mutex;
	struct ib_umad_port    *port;
	struct list_head	recv_list;
	struct list_head	send_list;
	struct list_head	port_list;
	spinlock_t		send_lock;
	wait_queue_head_t	recv_wait;
	struct ib_mad_agent    *agent[IB_UMAD_MAX_AGENTS];
	int			agents_dead;
	u8			use_pkey_index;
	u8			already_used;
};

struct ib_umad_packet {
	struct ib_mad_send_buf *msg;
	struct ib_mad_recv_wc  *recv_wc;
	struct list_head   list;
	int		   length;
	struct ib_user_mad mad;
};

<<<<<<< HEAD
static struct class *umad_class;

static const dev_t base_dev = MKDEV(IB_UMAD_MAJOR, IB_UMAD_MINOR_BASE);

static DEFINE_SPINLOCK(port_lock);
static DECLARE_BITMAP(dev_map, IB_UMAD_MAX_PORTS);

static void ib_umad_add_one(struct ib_device *device);
static void ib_umad_remove_one(struct ib_device *device);

static void ib_umad_release_dev(struct kobject *kobj)
{
	struct ib_umad_device *dev =
		container_of(kobj, struct ib_umad_device, kobj);
=======
struct ib_rmpp_mad_hdr {
	struct ib_mad_hdr	mad_hdr;
	struct ib_rmpp_hdr      rmpp_hdr;
} __packed;

#define CREATE_TRACE_POINTS
#include <trace/events/ib_umad.h>

static const dev_t base_umad_dev = MKDEV(IB_UMAD_MAJOR, IB_UMAD_MINOR_BASE);
static const dev_t base_issm_dev = MKDEV(IB_UMAD_MAJOR, IB_UMAD_MINOR_BASE) +
				   IB_UMAD_NUM_FIXED_MINOR;
static dev_t dynamic_umad_dev;
static dev_t dynamic_issm_dev;

static DEFINE_IDA(umad_ida);

static int ib_umad_add_one(struct ib_device *device);
static void ib_umad_remove_one(struct ib_device *device, void *client_data);

static void ib_umad_dev_free(struct kref *kref)
{
	struct ib_umad_device *dev =
		container_of(kref, struct ib_umad_device, kref);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree(dev);
}

<<<<<<< HEAD
static struct kobj_type ib_umad_dev_ktype = {
	.release = ib_umad_release_dev,
};

static int hdr_size(struct ib_umad_file *file)
{
	return file->use_pkey_index ? sizeof (struct ib_user_mad_hdr) :
		sizeof (struct ib_user_mad_hdr_old);
=======
static void ib_umad_dev_get(struct ib_umad_device *dev)
{
	kref_get(&dev->kref);
}

static void ib_umad_dev_put(struct ib_umad_device *dev)
{
	kref_put(&dev->kref, ib_umad_dev_free);
}

static int hdr_size(struct ib_umad_file *file)
{
	return file->use_pkey_index ? sizeof(struct ib_user_mad_hdr) :
				      sizeof(struct ib_user_mad_hdr_old);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* caller must hold file->mutex */
static struct ib_mad_agent *__get_agent(struct ib_umad_file *file, int id)
{
	return file->agents_dead ? NULL : file->agent[id];
}

static int queue_packet(struct ib_umad_file *file,
			struct ib_mad_agent *agent,
			struct ib_umad_packet *packet)
{
	int ret = 1;

	mutex_lock(&file->mutex);

	for (packet->mad.hdr.id = 0;
	     packet->mad.hdr.id < IB_UMAD_MAX_AGENTS;
	     packet->mad.hdr.id++)
		if (agent == __get_agent(file, packet->mad.hdr.id)) {
			list_add_tail(&packet->list, &file->recv_list);
			wake_up_interruptible(&file->recv_wait);
			ret = 0;
			break;
		}

	mutex_unlock(&file->mutex);

	return ret;
}

static void dequeue_send(struct ib_umad_file *file,
			 struct ib_umad_packet *packet)
{
	spin_lock_irq(&file->send_lock);
	list_del(&packet->list);
	spin_unlock_irq(&file->send_lock);
}

static void send_handler(struct ib_mad_agent *agent,
			 struct ib_mad_send_wc *send_wc)
{
	struct ib_umad_file *file = agent->context;
	struct ib_umad_packet *packet = send_wc->send_buf->context[0];

	dequeue_send(file, packet);
<<<<<<< HEAD
	ib_destroy_ah(packet->msg->ah);
=======
	rdma_destroy_ah(packet->msg->ah, RDMA_DESTROY_AH_SLEEPABLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ib_free_send_mad(packet->msg);

	if (send_wc->status == IB_WC_RESP_TIMEOUT_ERR) {
		packet->length = IB_MGMT_MAD_HDR;
		packet->mad.hdr.status = ETIMEDOUT;
		if (!queue_packet(file, agent, packet))
			return;
	}
	kfree(packet);
}

static void recv_handler(struct ib_mad_agent *agent,
<<<<<<< HEAD
=======
			 struct ib_mad_send_buf *send_buf,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 struct ib_mad_recv_wc *mad_recv_wc)
{
	struct ib_umad_file *file = agent->context;
	struct ib_umad_packet *packet;

	if (mad_recv_wc->wc->status != IB_WC_SUCCESS)
		goto err1;

	packet = kzalloc(sizeof *packet, GFP_KERNEL);
	if (!packet)
		goto err1;

	packet->length = mad_recv_wc->mad_len;
	packet->recv_wc = mad_recv_wc;

	packet->mad.hdr.status	   = 0;
	packet->mad.hdr.length	   = hdr_size(file) + mad_recv_wc->mad_len;
	packet->mad.hdr.qpn	   = cpu_to_be32(mad_recv_wc->wc->src_qp);
<<<<<<< HEAD
	packet->mad.hdr.lid	   = cpu_to_be16(mad_recv_wc->wc->slid);
=======
	/*
	 * On OPA devices it is okay to lose the upper 16 bits of LID as this
	 * information is obtained elsewhere. Mask off the upper 16 bits.
	 */
	if (rdma_cap_opa_mad(agent->device, agent->port_num))
		packet->mad.hdr.lid = ib_lid_be16(0xFFFF &
						  mad_recv_wc->wc->slid);
	else
		packet->mad.hdr.lid = ib_lid_be16(mad_recv_wc->wc->slid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	packet->mad.hdr.sl	   = mad_recv_wc->wc->sl;
	packet->mad.hdr.path_bits  = mad_recv_wc->wc->dlid_path_bits;
	packet->mad.hdr.pkey_index = mad_recv_wc->wc->pkey_index;
	packet->mad.hdr.grh_present = !!(mad_recv_wc->wc->wc_flags & IB_WC_GRH);
	if (packet->mad.hdr.grh_present) {
<<<<<<< HEAD
		struct ib_ah_attr ah_attr;

		ib_init_ah_from_wc(agent->device, agent->port_num,
				   mad_recv_wc->wc, mad_recv_wc->recv_buf.grh,
				   &ah_attr);

		packet->mad.hdr.gid_index = ah_attr.grh.sgid_index;
		packet->mad.hdr.hop_limit = ah_attr.grh.hop_limit;
		packet->mad.hdr.traffic_class = ah_attr.grh.traffic_class;
		memcpy(packet->mad.hdr.gid, &ah_attr.grh.dgid, 16);
		packet->mad.hdr.flow_label = cpu_to_be32(ah_attr.grh.flow_label);
=======
		struct rdma_ah_attr ah_attr;
		const struct ib_global_route *grh;
		int ret;

		ret = ib_init_ah_attr_from_wc(agent->device, agent->port_num,
					      mad_recv_wc->wc,
					      mad_recv_wc->recv_buf.grh,
					      &ah_attr);
		if (ret)
			goto err2;

		grh = rdma_ah_read_grh(&ah_attr);
		packet->mad.hdr.gid_index = grh->sgid_index;
		packet->mad.hdr.hop_limit = grh->hop_limit;
		packet->mad.hdr.traffic_class = grh->traffic_class;
		memcpy(packet->mad.hdr.gid, &grh->dgid, 16);
		packet->mad.hdr.flow_label = cpu_to_be32(grh->flow_label);
		rdma_destroy_ah_attr(&ah_attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (queue_packet(file, agent, packet))
		goto err2;
	return;

err2:
	kfree(packet);
err1:
	ib_free_recv_mad(mad_recv_wc);
}

static ssize_t copy_recv_mad(struct ib_umad_file *file, char __user *buf,
			     struct ib_umad_packet *packet, size_t count)
{
	struct ib_mad_recv_buf *recv_buf;
	int left, seg_payload, offset, max_seg_payload;
<<<<<<< HEAD

	/* We need enough room to copy the first (or only) MAD segment. */
	recv_buf = &packet->recv_wc->recv_buf;
	if ((packet->length <= sizeof (*recv_buf->mad) &&
	     count < hdr_size(file) + packet->length) ||
	    (packet->length > sizeof (*recv_buf->mad) &&
	     count < hdr_size(file) + sizeof (*recv_buf->mad)))
=======
	size_t seg_size;

	recv_buf = &packet->recv_wc->recv_buf;
	seg_size = packet->recv_wc->mad_seg_size;

	/* We need enough room to copy the first (or only) MAD segment. */
	if ((packet->length <= seg_size &&
	     count < hdr_size(file) + packet->length) ||
	    (packet->length > seg_size &&
	     count < hdr_size(file) + seg_size))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	if (copy_to_user(buf, &packet->mad, hdr_size(file)))
		return -EFAULT;

	buf += hdr_size(file);
<<<<<<< HEAD
	seg_payload = min_t(int, packet->length, sizeof (*recv_buf->mad));
=======
	seg_payload = min_t(int, packet->length, seg_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (copy_to_user(buf, recv_buf->mad, seg_payload))
		return -EFAULT;

	if (seg_payload < packet->length) {
		/*
		 * Multipacket RMPP MAD message. Copy remainder of message.
		 * Note that last segment may have a shorter payload.
		 */
		if (count < hdr_size(file) + packet->length) {
			/*
			 * The buffer is too small, return the first RMPP segment,
			 * which includes the RMPP message length.
			 */
			return -ENOSPC;
		}
		offset = ib_get_mad_data_offset(recv_buf->mad->mad_hdr.mgmt_class);
<<<<<<< HEAD
		max_seg_payload = sizeof (struct ib_mad) - offset;
=======
		max_seg_payload = seg_size - offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		for (left = packet->length - seg_payload, buf += seg_payload;
		     left; left -= seg_payload, buf += seg_payload) {
			recv_buf = container_of(recv_buf->list.next,
						struct ib_mad_recv_buf, list);
			seg_payload = min(left, max_seg_payload);
			if (copy_to_user(buf, ((void *) recv_buf->mad) + offset,
					 seg_payload))
				return -EFAULT;
		}
	}
<<<<<<< HEAD
=======

	trace_ib_umad_read_recv(file, &packet->mad.hdr, &recv_buf->mad->mad_hdr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return hdr_size(file) + packet->length;
}

static ssize_t copy_send_mad(struct ib_umad_file *file, char __user *buf,
			     struct ib_umad_packet *packet, size_t count)
{
	ssize_t size = hdr_size(file) + packet->length;

	if (count < size)
		return -EINVAL;

	if (copy_to_user(buf, &packet->mad, hdr_size(file)))
		return -EFAULT;

	buf += hdr_size(file);

	if (copy_to_user(buf, packet->mad.data, packet->length))
		return -EFAULT;

<<<<<<< HEAD
=======
	trace_ib_umad_read_send(file, &packet->mad.hdr,
				(struct ib_mad_hdr *)&packet->mad.data);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return size;
}

static ssize_t ib_umad_read(struct file *filp, char __user *buf,
			    size_t count, loff_t *pos)
{
	struct ib_umad_file *file = filp->private_data;
	struct ib_umad_packet *packet;
	ssize_t ret;

	if (count < hdr_size(file))
		return -EINVAL;

	mutex_lock(&file->mutex);

<<<<<<< HEAD
=======
	if (file->agents_dead) {
		mutex_unlock(&file->mutex);
		return -EIO;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (list_empty(&file->recv_list)) {
		mutex_unlock(&file->mutex);

		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;

		if (wait_event_interruptible(file->recv_wait,
					     !list_empty(&file->recv_list)))
			return -ERESTARTSYS;

		mutex_lock(&file->mutex);
	}

<<<<<<< HEAD
=======
	if (file->agents_dead) {
		mutex_unlock(&file->mutex);
		return -EIO;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	packet = list_entry(file->recv_list.next, struct ib_umad_packet, list);
	list_del(&packet->list);

	mutex_unlock(&file->mutex);

	if (packet->recv_wc)
		ret = copy_recv_mad(file, buf, packet, count);
	else
		ret = copy_send_mad(file, buf, packet, count);

	if (ret < 0) {
		/* Requeue packet */
		mutex_lock(&file->mutex);
		list_add(&packet->list, &file->recv_list);
		mutex_unlock(&file->mutex);
	} else {
		if (packet->recv_wc)
			ib_free_recv_mad(packet->recv_wc);
		kfree(packet);
	}
	return ret;
}

static int copy_rmpp_mad(struct ib_mad_send_buf *msg, const char __user *buf)
{
	int left, seg;

	/* Copy class specific header */
	if ((msg->hdr_len > IB_MGMT_RMPP_HDR) &&
	    copy_from_user(msg->mad + IB_MGMT_RMPP_HDR, buf + IB_MGMT_RMPP_HDR,
			   msg->hdr_len - IB_MGMT_RMPP_HDR))
		return -EFAULT;

	/* All headers are in place.  Copy data segments. */
	for (seg = 1, left = msg->data_len, buf += msg->hdr_len; left > 0;
	     seg++, left -= msg->seg_size, buf += msg->seg_size) {
		if (copy_from_user(ib_get_rmpp_segment(msg, seg), buf,
				   min(left, msg->seg_size)))
			return -EFAULT;
	}
	return 0;
}

static int same_destination(struct ib_user_mad_hdr *hdr1,
			    struct ib_user_mad_hdr *hdr2)
{
	if (!hdr1->grh_present && !hdr2->grh_present)
	   return (hdr1->lid == hdr2->lid);

	if (hdr1->grh_present && hdr2->grh_present)
	   return !memcmp(hdr1->gid, hdr2->gid, 16);

	return 0;
}

static int is_duplicate(struct ib_umad_file *file,
			struct ib_umad_packet *packet)
{
	struct ib_umad_packet *sent_packet;
	struct ib_mad_hdr *sent_hdr, *hdr;

	hdr = (struct ib_mad_hdr *) packet->mad.data;
	list_for_each_entry(sent_packet, &file->send_list, list) {
		sent_hdr = (struct ib_mad_hdr *) sent_packet->mad.data;

		if ((hdr->tid != sent_hdr->tid) ||
		    (hdr->mgmt_class != sent_hdr->mgmt_class))
			continue;

		/*
		 * No need to be overly clever here.  If two new operations have
		 * the same TID, reject the second as a duplicate.  This is more
		 * restrictive than required by the spec.
		 */
<<<<<<< HEAD
		if (!ib_response_mad((struct ib_mad *) hdr)) {
			if (!ib_response_mad((struct ib_mad *) sent_hdr))
				return 1;
			continue;
		} else if (!ib_response_mad((struct ib_mad *) sent_hdr))
=======
		if (!ib_response_mad(hdr)) {
			if (!ib_response_mad(sent_hdr))
				return 1;
			continue;
		} else if (!ib_response_mad(sent_hdr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;

		if (same_destination(&packet->mad.hdr, &sent_packet->mad.hdr))
			return 1;
	}

	return 0;
}

static ssize_t ib_umad_write(struct file *filp, const char __user *buf,
			     size_t count, loff_t *pos)
{
	struct ib_umad_file *file = filp->private_data;
<<<<<<< HEAD
	struct ib_umad_packet *packet;
	struct ib_mad_agent *agent;
	struct ib_ah_attr ah_attr;
	struct ib_ah *ah;
	struct ib_rmpp_mad *rmpp_mad;
	__be64 *tid;
	int ret, data_len, hdr_len, copy_offset, rmpp_active;
=======
	struct ib_rmpp_mad_hdr *rmpp_mad_hdr;
	struct ib_umad_packet *packet;
	struct ib_mad_agent *agent;
	struct rdma_ah_attr ah_attr;
	struct ib_ah *ah;
	__be64 *tid;
	int ret, data_len, hdr_len, copy_offset, rmpp_active;
	u8 base_version;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (count < hdr_size(file) + IB_MGMT_RMPP_HDR)
		return -EINVAL;

<<<<<<< HEAD
	packet = kzalloc(sizeof *packet + IB_MGMT_RMPP_HDR, GFP_KERNEL);
=======
	packet = kzalloc(sizeof(*packet) + IB_MGMT_RMPP_HDR, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!packet)
		return -ENOMEM;

	if (copy_from_user(&packet->mad, buf, hdr_size(file))) {
		ret = -EFAULT;
		goto err;
	}

	if (packet->mad.hdr.id >= IB_UMAD_MAX_AGENTS) {
		ret = -EINVAL;
		goto err;
	}

	buf += hdr_size(file);

	if (copy_from_user(packet->mad.data, buf, IB_MGMT_RMPP_HDR)) {
		ret = -EFAULT;
		goto err;
	}

	mutex_lock(&file->mutex);

<<<<<<< HEAD
	agent = __get_agent(file, packet->mad.hdr.id);
	if (!agent) {
		ret = -EINVAL;
=======
	trace_ib_umad_write(file, &packet->mad.hdr,
			    (struct ib_mad_hdr *)&packet->mad.data);

	agent = __get_agent(file, packet->mad.hdr.id);
	if (!agent) {
		ret = -EIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_up;
	}

	memset(&ah_attr, 0, sizeof ah_attr);
<<<<<<< HEAD
	ah_attr.dlid          = be16_to_cpu(packet->mad.hdr.lid);
	ah_attr.sl            = packet->mad.hdr.sl;
	ah_attr.src_path_bits = packet->mad.hdr.path_bits;
	ah_attr.port_num      = file->port->port_num;
	if (packet->mad.hdr.grh_present) {
		ah_attr.ah_flags = IB_AH_GRH;
		memcpy(ah_attr.grh.dgid.raw, packet->mad.hdr.gid, 16);
		ah_attr.grh.sgid_index	   = packet->mad.hdr.gid_index;
		ah_attr.grh.flow_label	   = be32_to_cpu(packet->mad.hdr.flow_label);
		ah_attr.grh.hop_limit	   = packet->mad.hdr.hop_limit;
		ah_attr.grh.traffic_class  = packet->mad.hdr.traffic_class;
	}

	ah = ib_create_ah(agent->qp->pd, &ah_attr);
=======
	ah_attr.type = rdma_ah_find_type(agent->device,
					 file->port->port_num);
	rdma_ah_set_dlid(&ah_attr, be16_to_cpu(packet->mad.hdr.lid));
	rdma_ah_set_sl(&ah_attr, packet->mad.hdr.sl);
	rdma_ah_set_path_bits(&ah_attr, packet->mad.hdr.path_bits);
	rdma_ah_set_port_num(&ah_attr, file->port->port_num);
	if (packet->mad.hdr.grh_present) {
		rdma_ah_set_grh(&ah_attr, NULL,
				be32_to_cpu(packet->mad.hdr.flow_label),
				packet->mad.hdr.gid_index,
				packet->mad.hdr.hop_limit,
				packet->mad.hdr.traffic_class);
		rdma_ah_set_dgid_raw(&ah_attr, packet->mad.hdr.gid);
	}

	ah = rdma_create_user_ah(agent->qp->pd, &ah_attr, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(ah)) {
		ret = PTR_ERR(ah);
		goto err_up;
	}

<<<<<<< HEAD
	rmpp_mad = (struct ib_rmpp_mad *) packet->mad.data;
	hdr_len = ib_get_mad_data_offset(rmpp_mad->mad_hdr.mgmt_class);
	if (!ib_is_mad_class_rmpp(rmpp_mad->mad_hdr.mgmt_class)) {
		copy_offset = IB_MGMT_MAD_HDR;
		rmpp_active = 0;
	} else {
		copy_offset = IB_MGMT_RMPP_HDR;
		rmpp_active = ib_get_rmpp_flags(&rmpp_mad->rmpp_hdr) &
			      IB_MGMT_RMPP_FLAG_ACTIVE;
	}

=======
	rmpp_mad_hdr = (struct ib_rmpp_mad_hdr *)packet->mad.data;
	hdr_len = ib_get_mad_data_offset(rmpp_mad_hdr->mad_hdr.mgmt_class);

	if (ib_is_mad_class_rmpp(rmpp_mad_hdr->mad_hdr.mgmt_class)
	    && ib_mad_kernel_rmpp_agent(agent)) {
		copy_offset = IB_MGMT_RMPP_HDR;
		rmpp_active = ib_get_rmpp_flags(&rmpp_mad_hdr->rmpp_hdr) &
						IB_MGMT_RMPP_FLAG_ACTIVE;
	} else {
		copy_offset = IB_MGMT_MAD_HDR;
		rmpp_active = 0;
	}

	base_version = ((struct ib_mad_hdr *)&packet->mad.data)->base_version;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	data_len = count - hdr_size(file) - hdr_len;
	packet->msg = ib_create_send_mad(agent,
					 be32_to_cpu(packet->mad.hdr.qpn),
					 packet->mad.hdr.pkey_index, rmpp_active,
<<<<<<< HEAD
					 hdr_len, data_len, GFP_KERNEL);
=======
					 hdr_len, data_len, GFP_KERNEL,
					 base_version);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(packet->msg)) {
		ret = PTR_ERR(packet->msg);
		goto err_ah;
	}

	packet->msg->ah		= ah;
	packet->msg->timeout_ms = packet->mad.hdr.timeout_ms;
	packet->msg->retries	= packet->mad.hdr.retries;
	packet->msg->context[0] = packet;

	/* Copy MAD header.  Any RMPP header is already in place. */
	memcpy(packet->msg->mad, packet->mad.data, IB_MGMT_MAD_HDR);

	if (!rmpp_active) {
		if (copy_from_user(packet->msg->mad + copy_offset,
				   buf + copy_offset,
				   hdr_len + data_len - copy_offset)) {
			ret = -EFAULT;
			goto err_msg;
		}
	} else {
		ret = copy_rmpp_mad(packet->msg, buf);
		if (ret)
			goto err_msg;
	}

	/*
	 * Set the high-order part of the transaction ID to make MADs from
	 * different agents unique, and allow routing responses back to the
	 * original requestor.
	 */
	if (!ib_response_mad(packet->msg->mad)) {
		tid = &((struct ib_mad_hdr *) packet->msg->mad)->tid;
		*tid = cpu_to_be64(((u64) agent->hi_tid) << 32 |
				   (be64_to_cpup(tid) & 0xffffffff));
<<<<<<< HEAD
		rmpp_mad->mad_hdr.tid = *tid;
	}

	spin_lock_irq(&file->send_lock);
	ret = is_duplicate(file, packet);
	if (!ret)
		list_add_tail(&packet->list, &file->send_list);
	spin_unlock_irq(&file->send_lock);
	if (ret) {
		ret = -EINVAL;
		goto err_msg;
=======
		rmpp_mad_hdr->mad_hdr.tid = *tid;
	}

	if (!ib_mad_kernel_rmpp_agent(agent)
	    && ib_is_mad_class_rmpp(rmpp_mad_hdr->mad_hdr.mgmt_class)
	    && (ib_get_rmpp_flags(&rmpp_mad_hdr->rmpp_hdr) & IB_MGMT_RMPP_FLAG_ACTIVE)) {
		spin_lock_irq(&file->send_lock);
		list_add_tail(&packet->list, &file->send_list);
		spin_unlock_irq(&file->send_lock);
	} else {
		spin_lock_irq(&file->send_lock);
		ret = is_duplicate(file, packet);
		if (!ret)
			list_add_tail(&packet->list, &file->send_list);
		spin_unlock_irq(&file->send_lock);
		if (ret) {
			ret = -EINVAL;
			goto err_msg;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ret = ib_post_send_mad(packet->msg, NULL);
	if (ret)
		goto err_send;

	mutex_unlock(&file->mutex);
	return count;

err_send:
	dequeue_send(file, packet);
err_msg:
	ib_free_send_mad(packet->msg);
err_ah:
<<<<<<< HEAD
	ib_destroy_ah(ah);
=======
	rdma_destroy_ah(ah, RDMA_DESTROY_AH_SLEEPABLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_up:
	mutex_unlock(&file->mutex);
err:
	kfree(packet);
	return ret;
}

<<<<<<< HEAD
static unsigned int ib_umad_poll(struct file *filp, struct poll_table_struct *wait)
=======
static __poll_t ib_umad_poll(struct file *filp, struct poll_table_struct *wait)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_umad_file *file = filp->private_data;

	/* we will always be able to post a MAD send */
<<<<<<< HEAD
	unsigned int mask = POLLOUT | POLLWRNORM;

	poll_wait(filp, &file->recv_wait, wait);

	if (!list_empty(&file->recv_list))
		mask |= POLLIN | POLLRDNORM;
=======
	__poll_t mask = EPOLLOUT | EPOLLWRNORM;

	mutex_lock(&file->mutex);
	poll_wait(filp, &file->recv_wait, wait);

	if (!list_empty(&file->recv_list))
		mask |= EPOLLIN | EPOLLRDNORM;
	if (file->agents_dead)
		mask = EPOLLERR;
	mutex_unlock(&file->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return mask;
}

static int ib_umad_reg_agent(struct ib_umad_file *file, void __user *arg,
			     int compat_method_mask)
{
	struct ib_user_mad_reg_req ureq;
	struct ib_mad_reg_req req;
	struct ib_mad_agent *agent = NULL;
	int agent_id;
	int ret;

	mutex_lock(&file->port->file_mutex);
	mutex_lock(&file->mutex);

	if (!file->port->ib_dev) {
<<<<<<< HEAD
=======
		dev_notice(&file->port->dev, "%s: invalid device\n", __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EPIPE;
		goto out;
	}

	if (copy_from_user(&ureq, arg, sizeof ureq)) {
		ret = -EFAULT;
		goto out;
	}

	if (ureq.qpn != 0 && ureq.qpn != 1) {
<<<<<<< HEAD
=======
		dev_notice(&file->port->dev,
			   "%s: invalid QPN %u specified\n", __func__,
			   ureq.qpn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto out;
	}

	for (agent_id = 0; agent_id < IB_UMAD_MAX_AGENTS; ++agent_id)
		if (!__get_agent(file, agent_id))
			goto found;

<<<<<<< HEAD
=======
	dev_notice(&file->port->dev, "%s: Max Agents (%u) reached\n", __func__,
		   IB_UMAD_MAX_AGENTS);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = -ENOMEM;
	goto out;

found:
	if (ureq.mgmt_class) {
<<<<<<< HEAD
=======
		memset(&req, 0, sizeof(req));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		req.mgmt_class         = ureq.mgmt_class;
		req.mgmt_class_version = ureq.mgmt_class_version;
		memcpy(req.oui, ureq.oui, sizeof req.oui);

		if (compat_method_mask) {
			u32 *umm = (u32 *) ureq.method_mask;
			int i;

			for (i = 0; i < BITS_TO_LONGS(IB_MGMT_MAX_METHODS); ++i)
				req.method_mask[i] =
					umm[i * 2] | ((u64) umm[i * 2 + 1] << 32);
		} else
			memcpy(req.method_mask, ureq.method_mask,
			       sizeof req.method_mask);
	}

	agent = ib_register_mad_agent(file->port->ib_dev, file->port->port_num,
				      ureq.qpn ? IB_QPT_GSI : IB_QPT_SMI,
				      ureq.mgmt_class ? &req : NULL,
				      ureq.rmpp_version,
<<<<<<< HEAD
				      send_handler, recv_handler, file);
=======
				      send_handler, recv_handler, file, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(agent)) {
		ret = PTR_ERR(agent);
		agent = NULL;
		goto out;
	}

	if (put_user(agent_id,
		     (u32 __user *) (arg + offsetof(struct ib_user_mad_reg_req, id)))) {
		ret = -EFAULT;
		goto out;
	}

	if (!file->already_used) {
		file->already_used = 1;
		if (!file->use_pkey_index) {
<<<<<<< HEAD
			printk(KERN_WARNING "user_mad: process %s did not enable "
			       "P_Key index support.\n", current->comm);
			printk(KERN_WARNING "user_mad:   Documentation/infiniband/user_mad.txt "
			       "has info on the new ABI.\n");
=======
			dev_warn(&file->port->dev,
				"process %s did not enable P_Key index support.\n",
				current->comm);
			dev_warn(&file->port->dev,
				"   Documentation/infiniband/user_mad.rst has info on the new ABI.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	file->agent[agent_id] = agent;
	ret = 0;

out:
	mutex_unlock(&file->mutex);

	if (ret && agent)
		ib_unregister_mad_agent(agent);

	mutex_unlock(&file->port->file_mutex);

	return ret;
}

<<<<<<< HEAD
=======
static int ib_umad_reg_agent2(struct ib_umad_file *file, void __user *arg)
{
	struct ib_user_mad_reg_req2 ureq;
	struct ib_mad_reg_req req;
	struct ib_mad_agent *agent = NULL;
	int agent_id;
	int ret;

	mutex_lock(&file->port->file_mutex);
	mutex_lock(&file->mutex);

	if (!file->port->ib_dev) {
		dev_notice(&file->port->dev, "%s: invalid device\n", __func__);
		ret = -EPIPE;
		goto out;
	}

	if (copy_from_user(&ureq, arg, sizeof(ureq))) {
		ret = -EFAULT;
		goto out;
	}

	if (ureq.qpn != 0 && ureq.qpn != 1) {
		dev_notice(&file->port->dev, "%s: invalid QPN %u specified\n",
			   __func__, ureq.qpn);
		ret = -EINVAL;
		goto out;
	}

	if (ureq.flags & ~IB_USER_MAD_REG_FLAGS_CAP) {
		dev_notice(&file->port->dev,
			   "%s failed: invalid registration flags specified 0x%x; supported 0x%x\n",
			   __func__, ureq.flags, IB_USER_MAD_REG_FLAGS_CAP);
		ret = -EINVAL;

		if (put_user((u32)IB_USER_MAD_REG_FLAGS_CAP,
				(u32 __user *) (arg + offsetof(struct
				ib_user_mad_reg_req2, flags))))
			ret = -EFAULT;

		goto out;
	}

	for (agent_id = 0; agent_id < IB_UMAD_MAX_AGENTS; ++agent_id)
		if (!__get_agent(file, agent_id))
			goto found;

	dev_notice(&file->port->dev, "%s: Max Agents (%u) reached\n", __func__,
		   IB_UMAD_MAX_AGENTS);
	ret = -ENOMEM;
	goto out;

found:
	if (ureq.mgmt_class) {
		memset(&req, 0, sizeof(req));
		req.mgmt_class         = ureq.mgmt_class;
		req.mgmt_class_version = ureq.mgmt_class_version;
		if (ureq.oui & 0xff000000) {
			dev_notice(&file->port->dev,
				   "%s failed: oui invalid 0x%08x\n", __func__,
				   ureq.oui);
			ret = -EINVAL;
			goto out;
		}
		req.oui[2] =  ureq.oui & 0x0000ff;
		req.oui[1] = (ureq.oui & 0x00ff00) >> 8;
		req.oui[0] = (ureq.oui & 0xff0000) >> 16;
		memcpy(req.method_mask, ureq.method_mask,
			sizeof(req.method_mask));
	}

	agent = ib_register_mad_agent(file->port->ib_dev, file->port->port_num,
				      ureq.qpn ? IB_QPT_GSI : IB_QPT_SMI,
				      ureq.mgmt_class ? &req : NULL,
				      ureq.rmpp_version,
				      send_handler, recv_handler, file,
				      ureq.flags);
	if (IS_ERR(agent)) {
		ret = PTR_ERR(agent);
		agent = NULL;
		goto out;
	}

	if (put_user(agent_id,
		     (u32 __user *)(arg +
				offsetof(struct ib_user_mad_reg_req2, id)))) {
		ret = -EFAULT;
		goto out;
	}

	if (!file->already_used) {
		file->already_used = 1;
		file->use_pkey_index = 1;
	}

	file->agent[agent_id] = agent;
	ret = 0;

out:
	mutex_unlock(&file->mutex);

	if (ret && agent)
		ib_unregister_mad_agent(agent);

	mutex_unlock(&file->port->file_mutex);

	return ret;
}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ib_umad_unreg_agent(struct ib_umad_file *file, u32 __user *arg)
{
	struct ib_mad_agent *agent = NULL;
	u32 id;
	int ret = 0;

	if (get_user(id, arg))
		return -EFAULT;
<<<<<<< HEAD
=======
	if (id >= IB_UMAD_MAX_AGENTS)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&file->port->file_mutex);
	mutex_lock(&file->mutex);

<<<<<<< HEAD
	if (id >= IB_UMAD_MAX_AGENTS || !__get_agent(file, id)) {
=======
	id = array_index_nospec(id, IB_UMAD_MAX_AGENTS);
	if (!__get_agent(file, id)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto out;
	}

	agent = file->agent[id];
	file->agent[id] = NULL;

out:
	mutex_unlock(&file->mutex);

	if (agent)
		ib_unregister_mad_agent(agent);

	mutex_unlock(&file->port->file_mutex);

	return ret;
}

static long ib_umad_enable_pkey(struct ib_umad_file *file)
{
	int ret = 0;

	mutex_lock(&file->mutex);
	if (file->already_used)
		ret = -EINVAL;
	else
		file->use_pkey_index = 1;
	mutex_unlock(&file->mutex);

	return ret;
}

static long ib_umad_ioctl(struct file *filp, unsigned int cmd,
			  unsigned long arg)
{
	switch (cmd) {
	case IB_USER_MAD_REGISTER_AGENT:
		return ib_umad_reg_agent(filp->private_data, (void __user *) arg, 0);
	case IB_USER_MAD_UNREGISTER_AGENT:
		return ib_umad_unreg_agent(filp->private_data, (__u32 __user *) arg);
	case IB_USER_MAD_ENABLE_PKEY:
		return ib_umad_enable_pkey(filp->private_data);
<<<<<<< HEAD
=======
	case IB_USER_MAD_REGISTER_AGENT2:
		return ib_umad_reg_agent2(filp->private_data, (void __user *) arg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -ENOIOCTLCMD;
	}
}

#ifdef CONFIG_COMPAT
static long ib_umad_compat_ioctl(struct file *filp, unsigned int cmd,
				 unsigned long arg)
{
	switch (cmd) {
	case IB_USER_MAD_REGISTER_AGENT:
		return ib_umad_reg_agent(filp->private_data, compat_ptr(arg), 1);
	case IB_USER_MAD_UNREGISTER_AGENT:
		return ib_umad_unreg_agent(filp->private_data, compat_ptr(arg));
	case IB_USER_MAD_ENABLE_PKEY:
		return ib_umad_enable_pkey(filp->private_data);
<<<<<<< HEAD
=======
	case IB_USER_MAD_REGISTER_AGENT2:
		return ib_umad_reg_agent2(filp->private_data, compat_ptr(arg));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -ENOIOCTLCMD;
	}
}
#endif

/*
 * ib_umad_open() does not need the BKL:
 *
 *  - the ib_umad_port structures are properly reference counted, and
 *    everything else is purely local to the file being created, so
 *    races against other open calls are not a problem;
 *  - the ioctl method does not affect any global state outside of the
 *    file structure being operated on;
 */
static int ib_umad_open(struct inode *inode, struct file *filp)
{
	struct ib_umad_port *port;
	struct ib_umad_file *file;
<<<<<<< HEAD
	int ret = -ENXIO;
=======
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	port = container_of(inode->i_cdev, struct ib_umad_port, cdev);

	mutex_lock(&port->file_mutex);

<<<<<<< HEAD
	if (!port->ib_dev)
		goto out;

	ret = -ENOMEM;
	file = kzalloc(sizeof *file, GFP_KERNEL);
	if (!file)
		goto out;
=======
	if (!port->ib_dev) {
		ret = -ENXIO;
		goto out;
	}

	if (!rdma_dev_access_netns(port->ib_dev, current->nsproxy->net_ns)) {
		ret = -EPERM;
		goto out;
	}

	file = kzalloc(sizeof(*file), GFP_KERNEL);
	if (!file) {
		ret = -ENOMEM;
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_init(&file->mutex);
	spin_lock_init(&file->send_lock);
	INIT_LIST_HEAD(&file->recv_list);
	INIT_LIST_HEAD(&file->send_list);
	init_waitqueue_head(&file->recv_wait);

	file->port = port;
	filp->private_data = file;

	list_add_tail(&file->port_list, &port->file_list);

<<<<<<< HEAD
	ret = nonseekable_open(inode, filp);
	if (ret) {
		list_del(&file->port_list);
		kfree(file);
		goto out;
	}

	kobject_get(&port->umad_dev->kobj);

=======
	stream_open(inode, filp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	mutex_unlock(&port->file_mutex);
	return ret;
}

static int ib_umad_close(struct inode *inode, struct file *filp)
{
	struct ib_umad_file *file = filp->private_data;
<<<<<<< HEAD
	struct ib_umad_device *dev = file->port->umad_dev;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ib_umad_packet *packet, *tmp;
	int already_dead;
	int i;

	mutex_lock(&file->port->file_mutex);
	mutex_lock(&file->mutex);

	already_dead = file->agents_dead;
	file->agents_dead = 1;

	list_for_each_entry_safe(packet, tmp, &file->recv_list, list) {
		if (packet->recv_wc)
			ib_free_recv_mad(packet->recv_wc);
		kfree(packet);
	}

	list_del(&file->port_list);

	mutex_unlock(&file->mutex);

	if (!already_dead)
		for (i = 0; i < IB_UMAD_MAX_AGENTS; ++i)
			if (file->agent[i])
				ib_unregister_mad_agent(file->agent[i]);

	mutex_unlock(&file->port->file_mutex);
<<<<<<< HEAD

	kfree(file);
	kobject_put(&dev->kobj);

=======
	mutex_destroy(&file->mutex);
	kfree(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct file_operations umad_fops = {
	.owner		= THIS_MODULE,
	.read		= ib_umad_read,
	.write		= ib_umad_write,
	.poll		= ib_umad_poll,
	.unlocked_ioctl = ib_umad_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= ib_umad_compat_ioctl,
#endif
	.open		= ib_umad_open,
	.release	= ib_umad_close,
	.llseek		= no_llseek,
};

static int ib_umad_sm_open(struct inode *inode, struct file *filp)
{
	struct ib_umad_port *port;
	struct ib_port_modify props = {
		.set_port_cap_mask = IB_PORT_SM
	};
	int ret;

	port = container_of(inode->i_cdev, struct ib_umad_port, sm_cdev);

	if (filp->f_flags & O_NONBLOCK) {
		if (down_trylock(&port->sm_sem)) {
			ret = -EAGAIN;
			goto fail;
		}
	} else {
		if (down_interruptible(&port->sm_sem)) {
			ret = -ERESTARTSYS;
			goto fail;
		}
	}

<<<<<<< HEAD
=======
	if (!rdma_dev_access_netns(port->ib_dev, current->nsproxy->net_ns)) {
		ret = -EPERM;
		goto err_up_sem;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = ib_modify_port(port->ib_dev, port->port_num, 0, &props);
	if (ret)
		goto err_up_sem;

	filp->private_data = port;

<<<<<<< HEAD
	ret = nonseekable_open(inode, filp);
	if (ret)
		goto err_clr_sm_cap;

	kobject_get(&port->umad_dev->kobj);

	return 0;

err_clr_sm_cap:
	swap(props.set_port_cap_mask, props.clr_port_cap_mask);
	ib_modify_port(port->ib_dev, port->port_num, 0, &props);

=======
	nonseekable_open(inode, filp);
	return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_up_sem:
	up(&port->sm_sem);

fail:
	return ret;
}

static int ib_umad_sm_close(struct inode *inode, struct file *filp)
{
	struct ib_umad_port *port = filp->private_data;
	struct ib_port_modify props = {
		.clr_port_cap_mask = IB_PORT_SM
	};
	int ret = 0;

	mutex_lock(&port->file_mutex);
	if (port->ib_dev)
		ret = ib_modify_port(port->ib_dev, port->port_num, 0, &props);
	mutex_unlock(&port->file_mutex);

	up(&port->sm_sem);

<<<<<<< HEAD
	kobject_put(&port->umad_dev->kobj);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static const struct file_operations umad_sm_fops = {
	.owner	 = THIS_MODULE,
	.open	 = ib_umad_sm_open,
	.release = ib_umad_sm_close,
	.llseek	 = no_llseek,
};

<<<<<<< HEAD
static struct ib_client umad_client = {
	.name   = "umad",
	.add    = ib_umad_add_one,
	.remove = ib_umad_remove_one
};

static ssize_t show_ibdev(struct device *dev, struct device_attribute *attr,
=======
static struct ib_umad_port *get_port(struct ib_device *ibdev,
				     struct ib_umad_device *umad_dev,
				     u32 port)
{
	if (!umad_dev)
		return ERR_PTR(-EOPNOTSUPP);
	if (!rdma_is_port_valid(ibdev, port))
		return ERR_PTR(-EINVAL);
	if (!rdma_cap_ib_mad(ibdev, port))
		return ERR_PTR(-EOPNOTSUPP);

	return &umad_dev->ports[port - rdma_start_port(ibdev)];
}

static int ib_umad_get_nl_info(struct ib_device *ibdev, void *client_data,
			       struct ib_client_nl_info *res)
{
	struct ib_umad_port *port = get_port(ibdev, client_data, res->port);

	if (IS_ERR(port))
		return PTR_ERR(port);

	res->abi = IB_USER_MAD_ABI_VERSION;
	res->cdev = &port->dev;
	return 0;
}

static struct ib_client umad_client = {
	.name   = "umad",
	.add    = ib_umad_add_one,
	.remove = ib_umad_remove_one,
	.get_nl_info = ib_umad_get_nl_info,
};
MODULE_ALIAS_RDMA_CLIENT("umad");

static int ib_issm_get_nl_info(struct ib_device *ibdev, void *client_data,
			       struct ib_client_nl_info *res)
{
	struct ib_umad_port *port = get_port(ibdev, client_data, res->port);

	if (IS_ERR(port))
		return PTR_ERR(port);

	res->abi = IB_USER_MAD_ABI_VERSION;
	res->cdev = &port->sm_dev;
	return 0;
}

static struct ib_client issm_client = {
	.name = "issm",
	.get_nl_info = ib_issm_get_nl_info,
};
MODULE_ALIAS_RDMA_CLIENT("issm");

static ssize_t ibdev_show(struct device *dev, struct device_attribute *attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  char *buf)
{
	struct ib_umad_port *port = dev_get_drvdata(dev);

	if (!port)
		return -ENODEV;

<<<<<<< HEAD
	return sprintf(buf, "%s\n", port->ib_dev->name);
}
static DEVICE_ATTR(ibdev, S_IRUGO, show_ibdev, NULL);

static ssize_t show_port(struct device *dev, struct device_attribute *attr,
=======
	return sysfs_emit(buf, "%s\n", dev_name(&port->ib_dev->dev));
}
static DEVICE_ATTR_RO(ibdev);

static ssize_t port_show(struct device *dev, struct device_attribute *attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 char *buf)
{
	struct ib_umad_port *port = dev_get_drvdata(dev);

	if (!port)
		return -ENODEV;

<<<<<<< HEAD
	return sprintf(buf, "%d\n", port->port_num);
}
static DEVICE_ATTR(port, S_IRUGO, show_port, NULL);

static CLASS_ATTR_STRING(abi_version, S_IRUGO,
			 __stringify(IB_USER_MAD_ABI_VERSION));

static dev_t overflow_maj;
static DECLARE_BITMAP(overflow_map, IB_UMAD_MAX_PORTS);
static int find_overflow_devnum(void)
{
	int ret;

	if (!overflow_maj) {
		ret = alloc_chrdev_region(&overflow_maj, 0, IB_UMAD_MAX_PORTS * 2,
					  "infiniband_mad");
		if (ret) {
			printk(KERN_ERR "user_mad: couldn't register dynamic device number\n");
			return ret;
		}
	}

	ret = find_first_zero_bit(overflow_map, IB_UMAD_MAX_PORTS);
	if (ret >= IB_UMAD_MAX_PORTS)
		return -1;

	return ret;
=======
	return sysfs_emit(buf, "%d\n", port->port_num);
}
static DEVICE_ATTR_RO(port);

static struct attribute *umad_class_dev_attrs[] = {
	&dev_attr_ibdev.attr,
	&dev_attr_port.attr,
	NULL,
};
ATTRIBUTE_GROUPS(umad_class_dev);

static char *umad_devnode(const struct device *dev, umode_t *mode)
{
	return kasprintf(GFP_KERNEL, "infiniband/%s", dev_name(dev));
}

static ssize_t abi_version_show(const struct class *class,
				const struct class_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%d\n", IB_USER_MAD_ABI_VERSION);
}
static CLASS_ATTR_RO(abi_version);

static struct attribute *umad_class_attrs[] = {
	&class_attr_abi_version.attr,
	NULL,
};
ATTRIBUTE_GROUPS(umad_class);

static struct class umad_class = {
	.name		= "infiniband_mad",
	.devnode	= umad_devnode,
	.class_groups	= umad_class_groups,
	.dev_groups	= umad_class_dev_groups,
};

static void ib_umad_release_port(struct device *device)
{
	struct ib_umad_port *port = dev_get_drvdata(device);
	struct ib_umad_device *umad_dev = port->umad_dev;

	ib_umad_dev_put(umad_dev);
}

static void ib_umad_init_port_dev(struct device *dev,
				  struct ib_umad_port *port,
				  const struct ib_device *device)
{
	device_initialize(dev);
	ib_umad_dev_get(port->umad_dev);
	dev->class = &umad_class;
	dev->parent = device->dev.parent;
	dev_set_drvdata(dev, port);
	dev->release = ib_umad_release_port;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ib_umad_init_port(struct ib_device *device, int port_num,
			     struct ib_umad_device *umad_dev,
			     struct ib_umad_port *port)
{
	int devnum;
<<<<<<< HEAD
	dev_t base;

	spin_lock(&port_lock);
	devnum = find_first_zero_bit(dev_map, IB_UMAD_MAX_PORTS);
	if (devnum >= IB_UMAD_MAX_PORTS) {
		spin_unlock(&port_lock);
		devnum = find_overflow_devnum();
		if (devnum < 0)
			return -1;

		spin_lock(&port_lock);
		port->dev_num = devnum + IB_UMAD_MAX_PORTS;
		base = devnum + overflow_maj;
		set_bit(devnum, overflow_map);
	} else {
		port->dev_num = devnum;
		base = devnum + base_dev;
		set_bit(devnum, dev_map);
	}
	spin_unlock(&port_lock);

	port->ib_dev   = device;
=======
	dev_t base_umad;
	dev_t base_issm;
	int ret;

	devnum = ida_alloc_max(&umad_ida, IB_UMAD_MAX_PORTS - 1, GFP_KERNEL);
	if (devnum < 0)
		return -1;
	port->dev_num = devnum;
	if (devnum >= IB_UMAD_NUM_FIXED_MINOR) {
		base_umad = dynamic_umad_dev + devnum - IB_UMAD_NUM_FIXED_MINOR;
		base_issm = dynamic_issm_dev + devnum - IB_UMAD_NUM_FIXED_MINOR;
	} else {
		base_umad = devnum + base_umad_dev;
		base_issm = devnum + base_issm_dev;
	}

	port->ib_dev   = device;
	port->umad_dev = umad_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	port->port_num = port_num;
	sema_init(&port->sm_sem, 1);
	mutex_init(&port->file_mutex);
	INIT_LIST_HEAD(&port->file_list);

<<<<<<< HEAD
	cdev_init(&port->cdev, &umad_fops);
	port->cdev.owner = THIS_MODULE;
	port->cdev.kobj.parent = &umad_dev->kobj;
	kobject_set_name(&port->cdev.kobj, "umad%d", port->dev_num);
	if (cdev_add(&port->cdev, base, 1))
		goto err_cdev;

	port->dev = device_create(umad_class, device->dma_device,
				  port->cdev.dev, port,
				  "umad%d", port->dev_num);
	if (IS_ERR(port->dev))
		goto err_cdev;

	if (device_create_file(port->dev, &dev_attr_ibdev))
		goto err_dev;
	if (device_create_file(port->dev, &dev_attr_port))
		goto err_dev;

	base += IB_UMAD_MAX_PORTS;
	cdev_init(&port->sm_cdev, &umad_sm_fops);
	port->sm_cdev.owner = THIS_MODULE;
	port->sm_cdev.kobj.parent = &umad_dev->kobj;
	kobject_set_name(&port->sm_cdev.kobj, "issm%d", port->dev_num);
	if (cdev_add(&port->sm_cdev, base, 1))
		goto err_sm_cdev;

	port->sm_dev = device_create(umad_class, device->dma_device,
				     port->sm_cdev.dev, port,
				     "issm%d", port->dev_num);
	if (IS_ERR(port->sm_dev))
		goto err_sm_cdev;

	if (device_create_file(port->sm_dev, &dev_attr_ibdev))
		goto err_sm_dev;
	if (device_create_file(port->sm_dev, &dev_attr_port))
		goto err_sm_dev;

	return 0;

err_sm_dev:
	device_destroy(umad_class, port->sm_cdev.dev);

err_sm_cdev:
	cdev_del(&port->sm_cdev);

err_dev:
	device_destroy(umad_class, port->cdev.dev);

err_cdev:
	cdev_del(&port->cdev);
	if (port->dev_num < IB_UMAD_MAX_PORTS)
		clear_bit(devnum, dev_map);
	else
		clear_bit(devnum, overflow_map);

	return -1;
=======
	ib_umad_init_port_dev(&port->dev, port, device);
	port->dev.devt = base_umad;
	dev_set_name(&port->dev, "umad%d", port->dev_num);
	cdev_init(&port->cdev, &umad_fops);
	port->cdev.owner = THIS_MODULE;

	ret = cdev_device_add(&port->cdev, &port->dev);
	if (ret)
		goto err_cdev;

	ib_umad_init_port_dev(&port->sm_dev, port, device);
	port->sm_dev.devt = base_issm;
	dev_set_name(&port->sm_dev, "issm%d", port->dev_num);
	cdev_init(&port->sm_cdev, &umad_sm_fops);
	port->sm_cdev.owner = THIS_MODULE;

	ret = cdev_device_add(&port->sm_cdev, &port->sm_dev);
	if (ret)
		goto err_dev;

	return 0;

err_dev:
	put_device(&port->sm_dev);
	cdev_device_del(&port->cdev, &port->dev);
err_cdev:
	put_device(&port->dev);
	ida_free(&umad_ida, devnum);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ib_umad_kill_port(struct ib_umad_port *port)
{
	struct ib_umad_file *file;
	int id;

<<<<<<< HEAD
	dev_set_drvdata(port->dev,    NULL);
	dev_set_drvdata(port->sm_dev, NULL);

	device_destroy(umad_class, port->cdev.dev);
	device_destroy(umad_class, port->sm_cdev.dev);

	cdev_del(&port->cdev);
	cdev_del(&port->sm_cdev);

	mutex_lock(&port->file_mutex);

=======
	cdev_device_del(&port->sm_cdev, &port->sm_dev);
	cdev_device_del(&port->cdev, &port->dev);

	mutex_lock(&port->file_mutex);

	/* Mark ib_dev NULL and block ioctl or other file ops to progress
	 * further.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	port->ib_dev = NULL;

	list_for_each_entry(file, &port->file_list, port_list) {
		mutex_lock(&file->mutex);
		file->agents_dead = 1;
<<<<<<< HEAD
=======
		wake_up_interruptible(&file->recv_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mutex_unlock(&file->mutex);

		for (id = 0; id < IB_UMAD_MAX_AGENTS; ++id)
			if (file->agent[id])
				ib_unregister_mad_agent(file->agent[id]);
	}

	mutex_unlock(&port->file_mutex);

<<<<<<< HEAD
	if (port->dev_num < IB_UMAD_MAX_PORTS)
		clear_bit(port->dev_num, dev_map);
	else
		clear_bit(port->dev_num - IB_UMAD_MAX_PORTS, overflow_map);
}

static void ib_umad_add_one(struct ib_device *device)
{
	struct ib_umad_device *umad_dev;
	int s, e, i;

	if (rdma_node_get_transport(device->node_type) != RDMA_TRANSPORT_IB)
		return;

	if (device->node_type == RDMA_NODE_IB_SWITCH)
		s = e = 0;
	else {
		s = 1;
		e = device->phys_port_cnt;
	}

	umad_dev = kzalloc(sizeof *umad_dev +
			   (e - s + 1) * sizeof (struct ib_umad_port),
			   GFP_KERNEL);
	if (!umad_dev)
		return;

	kobject_init(&umad_dev->kobj, &ib_umad_dev_ktype);

	umad_dev->start_port = s;
	umad_dev->end_port   = e;

	for (i = s; i <= e; ++i) {
		umad_dev->port[i - s].umad_dev = umad_dev;

		if (ib_umad_init_port(device, i, umad_dev,
				      &umad_dev->port[i - s]))
			goto err;
=======
	ida_free(&umad_ida, port->dev_num);

	/* balances device_initialize() */
	put_device(&port->sm_dev);
	put_device(&port->dev);
}

static int ib_umad_add_one(struct ib_device *device)
{
	struct ib_umad_device *umad_dev;
	int s, e, i;
	int count = 0;
	int ret;

	s = rdma_start_port(device);
	e = rdma_end_port(device);

	umad_dev = kzalloc(struct_size(umad_dev, ports,
				       size_add(size_sub(e, s), 1)),
			   GFP_KERNEL);
	if (!umad_dev)
		return -ENOMEM;

	kref_init(&umad_dev->kref);
	for (i = s; i <= e; ++i) {
		if (!rdma_cap_ib_mad(device, i))
			continue;

		ret = ib_umad_init_port(device, i, umad_dev,
					&umad_dev->ports[i - s]);
		if (ret)
			goto err;

		count++;
	}

	if (!count) {
		ret = -EOPNOTSUPP;
		goto free;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ib_set_client_data(device, &umad_client, umad_dev);

<<<<<<< HEAD
	return;

err:
	while (--i >= s)
		ib_umad_kill_port(&umad_dev->port[i - s]);

	kobject_put(&umad_dev->kobj);
}

static void ib_umad_remove_one(struct ib_device *device)
{
	struct ib_umad_device *umad_dev = ib_get_client_data(device, &umad_client);
	int i;

	if (!umad_dev)
		return;

	for (i = 0; i <= umad_dev->end_port - umad_dev->start_port; ++i)
		ib_umad_kill_port(&umad_dev->port[i]);

	kobject_put(&umad_dev->kobj);
}

static char *umad_devnode(struct device *dev, umode_t *mode)
{
	return kasprintf(GFP_KERNEL, "infiniband/%s", dev_name(dev));
=======
	return 0;

err:
	while (--i >= s) {
		if (!rdma_cap_ib_mad(device, i))
			continue;

		ib_umad_kill_port(&umad_dev->ports[i - s]);
	}
free:
	/* balances kref_init */
	ib_umad_dev_put(umad_dev);
	return ret;
}

static void ib_umad_remove_one(struct ib_device *device, void *client_data)
{
	struct ib_umad_device *umad_dev = client_data;
	unsigned int i;

	rdma_for_each_port (device, i) {
		if (rdma_cap_ib_mad(device, i))
			ib_umad_kill_port(
				&umad_dev->ports[i - rdma_start_port(device)]);
	}
	/* balances kref_init() */
	ib_umad_dev_put(umad_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init ib_umad_init(void)
{
	int ret;

<<<<<<< HEAD
	ret = register_chrdev_region(base_dev, IB_UMAD_MAX_PORTS * 2,
				     "infiniband_mad");
	if (ret) {
		printk(KERN_ERR "user_mad: couldn't register device number\n");
		goto out;
	}

	umad_class = class_create(THIS_MODULE, "infiniband_mad");
	if (IS_ERR(umad_class)) {
		ret = PTR_ERR(umad_class);
		printk(KERN_ERR "user_mad: couldn't create class infiniband_mad\n");
		goto out_chrdev;
	}

	umad_class->devnode = umad_devnode;

	ret = class_create_file(umad_class, &class_attr_abi_version.attr);
	if (ret) {
		printk(KERN_ERR "user_mad: couldn't create abi_version attribute\n");
		goto out_class;
	}

	ret = ib_register_client(&umad_client);
	if (ret) {
		printk(KERN_ERR "user_mad: couldn't register ib_umad client\n");
		goto out_class;
	}

	return 0;

out_class:
	class_destroy(umad_class);

out_chrdev:
	unregister_chrdev_region(base_dev, IB_UMAD_MAX_PORTS * 2);
=======
	ret = register_chrdev_region(base_umad_dev,
				     IB_UMAD_NUM_FIXED_MINOR * 2,
				     umad_class.name);
	if (ret) {
		pr_err("couldn't register device number\n");
		goto out;
	}

	ret = alloc_chrdev_region(&dynamic_umad_dev, 0,
				  IB_UMAD_NUM_DYNAMIC_MINOR * 2,
				  umad_class.name);
	if (ret) {
		pr_err("couldn't register dynamic device number\n");
		goto out_alloc;
	}
	dynamic_issm_dev = dynamic_umad_dev + IB_UMAD_NUM_DYNAMIC_MINOR;

	ret = class_register(&umad_class);
	if (ret) {
		pr_err("couldn't create class infiniband_mad\n");
		goto out_chrdev;
	}

	ret = ib_register_client(&umad_client);
	if (ret)
		goto out_class;

	ret = ib_register_client(&issm_client);
	if (ret)
		goto out_client;

	return 0;

out_client:
	ib_unregister_client(&umad_client);
out_class:
	class_unregister(&umad_class);

out_chrdev:
	unregister_chrdev_region(dynamic_umad_dev,
				 IB_UMAD_NUM_DYNAMIC_MINOR * 2);

out_alloc:
	unregister_chrdev_region(base_umad_dev,
				 IB_UMAD_NUM_FIXED_MINOR * 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	return ret;
}

static void __exit ib_umad_cleanup(void)
{
<<<<<<< HEAD
	ib_unregister_client(&umad_client);
	class_destroy(umad_class);
	unregister_chrdev_region(base_dev, IB_UMAD_MAX_PORTS * 2);
	if (overflow_maj)
		unregister_chrdev_region(overflow_maj, IB_UMAD_MAX_PORTS * 2);
=======
	ib_unregister_client(&issm_client);
	ib_unregister_client(&umad_client);
	class_unregister(&umad_class);
	unregister_chrdev_region(base_umad_dev,
				 IB_UMAD_NUM_FIXED_MINOR * 2);
	unregister_chrdev_region(dynamic_umad_dev,
				 IB_UMAD_NUM_DYNAMIC_MINOR * 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(ib_umad_init);
module_exit(ib_umad_cleanup);
