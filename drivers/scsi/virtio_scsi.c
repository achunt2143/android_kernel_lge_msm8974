<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Virtio SCSI HBA driver
 *
 * Copyright IBM Corp. 2010
 * Copyright Red Hat, Inc. 2011
 *
 * Authors:
 *  Stefan Hajnoczi   <stefanha@linux.vnet.ibm.com>
 *  Paolo Bonzini   <pbonzini@redhat.com>
<<<<<<< HEAD
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mempool.h>
=======
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mempool.h>
#include <linux/interrupt.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/virtio.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_config.h>
#include <linux/virtio_scsi.h>
<<<<<<< HEAD
#include <scsi/scsi_host.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_cmnd.h>

#define VIRTIO_SCSI_MEMPOOL_SZ 64
=======
#include <linux/cpu.h>
#include <linux/blkdev.h>
#include <linux/blk-integrity.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_tcq.h>
#include <scsi/scsi_devinfo.h>
#include <linux/seqlock.h>
#include <linux/blk-mq-virtio.h>

#include "sd.h"

#define VIRTIO_SCSI_MEMPOOL_SZ 64
#define VIRTIO_SCSI_EVENT_LEN 8
#define VIRTIO_SCSI_VQ_BASE 2

static unsigned int virtscsi_poll_queues;
module_param(virtscsi_poll_queues, uint, 0644);
MODULE_PARM_DESC(virtscsi_poll_queues,
		 "The number of dedicated virtqueues for polling I/O");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Command queue element */
struct virtio_scsi_cmd {
	struct scsi_cmnd *sc;
	struct completion *comp;
	union {
		struct virtio_scsi_cmd_req       cmd;
<<<<<<< HEAD
=======
		struct virtio_scsi_cmd_req_pi    cmd_pi;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct virtio_scsi_ctrl_tmf_req  tmf;
		struct virtio_scsi_ctrl_an_req   an;
	} req;
	union {
		struct virtio_scsi_cmd_resp      cmd;
		struct virtio_scsi_ctrl_tmf_resp tmf;
		struct virtio_scsi_ctrl_an_resp  an;
		struct virtio_scsi_event         evt;
	} resp;
} ____cacheline_aligned_in_smp;

<<<<<<< HEAD
/* Driver instance state */
struct virtio_scsi {
	/* Protects ctrl_vq, req_vq and sg[] */
	spinlock_t vq_lock;

	struct virtio_device *vdev;
	struct virtqueue *ctrl_vq;
	struct virtqueue *event_vq;
	struct virtqueue *req_vq;

	/* For sglist construction when adding commands to the virtqueue.  */
	struct scatterlist sg[];
=======
struct virtio_scsi_event_node {
	struct virtio_scsi *vscsi;
	struct virtio_scsi_event event;
	struct work_struct work;
};

struct virtio_scsi_vq {
	/* Protects vq */
	spinlock_t vq_lock;

	struct virtqueue *vq;
};

/* Driver instance state */
struct virtio_scsi {
	struct virtio_device *vdev;

	/* Get some buffers ready for event vq */
	struct virtio_scsi_event_node event_list[VIRTIO_SCSI_EVENT_LEN];

	u32 num_queues;
	int io_queues[HCTX_MAX_TYPES];

	struct hlist_node node;

	/* Protected by event_vq lock */
	bool stop_events;

	struct virtio_scsi_vq ctrl_vq;
	struct virtio_scsi_vq event_vq;
	struct virtio_scsi_vq req_vqs[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct kmem_cache *virtscsi_cmd_cache;
static mempool_t *virtscsi_cmd_pool;

static inline struct Scsi_Host *virtio_scsi_host(struct virtio_device *vdev)
{
	return vdev->priv;
}

static void virtscsi_compute_resid(struct scsi_cmnd *sc, u32 resid)
{
<<<<<<< HEAD
	if (!resid)
		return;

	if (!scsi_bidi_cmnd(sc)) {
		scsi_set_resid(sc, resid);
		return;
	}

	scsi_in(sc)->resid = min(resid, scsi_in(sc)->length);
	scsi_out(sc)->resid = resid - scsi_in(sc)->resid;
}

/**
=======
	if (resid)
		scsi_set_resid(sc, min(resid, scsi_bufflen(sc)));
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * virtscsi_complete_cmd - finish a scsi_cmd and invoke scsi_done
 *
 * Called with vq_lock held.
 */
<<<<<<< HEAD
static void virtscsi_complete_cmd(void *buf)
=======
static void virtscsi_complete_cmd(struct virtio_scsi *vscsi, void *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct virtio_scsi_cmd *cmd = buf;
	struct scsi_cmnd *sc = cmd->sc;
	struct virtio_scsi_cmd_resp *resp = &cmd->resp.cmd;

	dev_dbg(&sc->device->sdev_gendev,
		"cmd %p response %u status %#02x sense_len %u\n",
		sc, resp->response, resp->status, resp->sense_len);

	sc->result = resp->status;
<<<<<<< HEAD
	virtscsi_compute_resid(sc, resp->resid);
=======
	virtscsi_compute_resid(sc, virtio32_to_cpu(vscsi->vdev, resp->resid));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (resp->response) {
	case VIRTIO_SCSI_S_OK:
		set_host_byte(sc, DID_OK);
		break;
	case VIRTIO_SCSI_S_OVERRUN:
		set_host_byte(sc, DID_ERROR);
		break;
	case VIRTIO_SCSI_S_ABORTED:
		set_host_byte(sc, DID_ABORT);
		break;
	case VIRTIO_SCSI_S_BAD_TARGET:
		set_host_byte(sc, DID_BAD_TARGET);
		break;
	case VIRTIO_SCSI_S_RESET:
		set_host_byte(sc, DID_RESET);
		break;
	case VIRTIO_SCSI_S_BUSY:
		set_host_byte(sc, DID_BUS_BUSY);
		break;
	case VIRTIO_SCSI_S_TRANSPORT_FAILURE:
		set_host_byte(sc, DID_TRANSPORT_DISRUPTED);
		break;
	case VIRTIO_SCSI_S_TARGET_FAILURE:
<<<<<<< HEAD
		set_host_byte(sc, DID_TARGET_FAILURE);
		break;
	case VIRTIO_SCSI_S_NEXUS_FAILURE:
		set_host_byte(sc, DID_NEXUS_FAILURE);
=======
		set_host_byte(sc, DID_BAD_TARGET);
		break;
	case VIRTIO_SCSI_S_NEXUS_FAILURE:
		set_status_byte(sc, SAM_STAT_RESERVATION_CONFLICT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		scmd_printk(KERN_WARNING, sc, "Unknown response %d",
			    resp->response);
<<<<<<< HEAD
		/* fall through */
=======
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case VIRTIO_SCSI_S_FAILURE:
		set_host_byte(sc, DID_ERROR);
		break;
	}

<<<<<<< HEAD
	WARN_ON(resp->sense_len > VIRTIO_SCSI_SENSE_SIZE);
	if (sc->sense_buffer) {
		memcpy(sc->sense_buffer, resp->sense,
		       min_t(u32, resp->sense_len, VIRTIO_SCSI_SENSE_SIZE));
		if (resp->sense_len)
			set_driver_byte(sc, DRIVER_SENSE);
	}

	mempool_free(cmd, virtscsi_cmd_pool);
	sc->scsi_done(sc);
}

static void virtscsi_vq_done(struct virtqueue *vq, void (*fn)(void *buf))
{
	struct Scsi_Host *sh = virtio_scsi_host(vq->vdev);
	struct virtio_scsi *vscsi = shost_priv(sh);
	void *buf;
	unsigned long flags;
	unsigned int len;

	spin_lock_irqsave(&vscsi->vq_lock, flags);

	do {
		virtqueue_disable_cb(vq);
		while ((buf = virtqueue_get_buf(vq, &len)) != NULL)
			fn(buf);
	} while (!virtqueue_enable_cb(vq));

	spin_unlock_irqrestore(&vscsi->vq_lock, flags);
=======
	WARN_ON(virtio32_to_cpu(vscsi->vdev, resp->sense_len) >
		VIRTIO_SCSI_SENSE_SIZE);
	if (resp->sense_len) {
		memcpy(sc->sense_buffer, resp->sense,
		       min_t(u32,
			     virtio32_to_cpu(vscsi->vdev, resp->sense_len),
			     VIRTIO_SCSI_SENSE_SIZE));
	}

	scsi_done(sc);
}

static void virtscsi_vq_done(struct virtio_scsi *vscsi,
			     struct virtio_scsi_vq *virtscsi_vq,
			     void (*fn)(struct virtio_scsi *vscsi, void *buf))
{
	void *buf;
	unsigned int len;
	unsigned long flags;
	struct virtqueue *vq = virtscsi_vq->vq;

	spin_lock_irqsave(&virtscsi_vq->vq_lock, flags);
	do {
		virtqueue_disable_cb(vq);
		while ((buf = virtqueue_get_buf(vq, &len)) != NULL)
			fn(vscsi, buf);

	} while (!virtqueue_enable_cb(vq));
	spin_unlock_irqrestore(&virtscsi_vq->vq_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void virtscsi_req_done(struct virtqueue *vq)
{
<<<<<<< HEAD
	virtscsi_vq_done(vq, virtscsi_complete_cmd);
};

static void virtscsi_complete_free(void *buf)
=======
	struct Scsi_Host *sh = virtio_scsi_host(vq->vdev);
	struct virtio_scsi *vscsi = shost_priv(sh);
	int index = vq->index - VIRTIO_SCSI_VQ_BASE;
	struct virtio_scsi_vq *req_vq = &vscsi->req_vqs[index];

	virtscsi_vq_done(vscsi, req_vq, virtscsi_complete_cmd);
};

static void virtscsi_poll_requests(struct virtio_scsi *vscsi)
{
	int i, num_vqs;

	num_vqs = vscsi->num_queues;
	for (i = 0; i < num_vqs; i++)
		virtscsi_vq_done(vscsi, &vscsi->req_vqs[i],
				 virtscsi_complete_cmd);
}

static void virtscsi_complete_free(struct virtio_scsi *vscsi, void *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct virtio_scsi_cmd *cmd = buf;

	if (cmd->comp)
<<<<<<< HEAD
		complete_all(cmd->comp);
	else
		mempool_free(cmd, virtscsi_cmd_pool);
=======
		complete(cmd->comp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void virtscsi_ctrl_done(struct virtqueue *vq)
{
<<<<<<< HEAD
	virtscsi_vq_done(vq, virtscsi_complete_free);
};

static void virtscsi_event_done(struct virtqueue *vq)
{
	virtscsi_vq_done(vq, virtscsi_complete_free);
};

static void virtscsi_map_sgl(struct scatterlist *sg, unsigned int *p_idx,
			     struct scsi_data_buffer *sdb)
{
	struct sg_table *table = &sdb->table;
	struct scatterlist *sg_elem;
	unsigned int idx = *p_idx;
	int i;

	for_each_sg(table->sgl, sg_elem, table->nents, i)
		sg[idx++] = *sg_elem;

	*p_idx = idx;
}

/**
 * virtscsi_map_cmd - map a scsi_cmd to a virtqueue scatterlist
 * @vscsi	: virtio_scsi state
 * @cmd		: command structure
 * @out_num	: number of read-only elements
 * @in_num	: number of write-only elements
 * @req_size	: size of the request buffer
 * @resp_size	: size of the response buffer
 *
 * Called with vq_lock held.
 */
static void virtscsi_map_cmd(struct virtio_scsi *vscsi,
			     struct virtio_scsi_cmd *cmd,
			     unsigned *out_num, unsigned *in_num,
			     size_t req_size, size_t resp_size)
{
	struct scsi_cmnd *sc = cmd->sc;
	struct scatterlist *sg = vscsi->sg;
	unsigned int idx = 0;

	if (sc) {
		struct Scsi_Host *shost = virtio_scsi_host(vscsi->vdev);
		BUG_ON(scsi_sg_count(sc) > shost->sg_tablesize);

		/* TODO: check feature bit and fail if unsupported?  */
		BUG_ON(sc->sc_data_direction == DMA_BIDIRECTIONAL);
	}

	/* Request header.  */
	sg_set_buf(&sg[idx++], &cmd->req, req_size);

	/* Data-out buffer.  */
	if (sc && sc->sc_data_direction != DMA_FROM_DEVICE)
		virtscsi_map_sgl(sg, &idx, scsi_out(sc));

	*out_num = idx;

	/* Response header.  */
	sg_set_buf(&sg[idx++], &cmd->resp, resp_size);

	/* Data-in buffer */
	if (sc && sc->sc_data_direction != DMA_TO_DEVICE)
		virtscsi_map_sgl(sg, &idx, scsi_in(sc));

	*in_num = idx - *out_num;
}

static int virtscsi_kick_cmd(struct virtio_scsi *vscsi, struct virtqueue *vq,
			     struct virtio_scsi_cmd *cmd,
			     size_t req_size, size_t resp_size, gfp_t gfp)
{
	unsigned int out_num, in_num;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&vscsi->vq_lock, flags);

	virtscsi_map_cmd(vscsi, cmd, &out_num, &in_num, req_size, resp_size);

	ret = virtqueue_add_buf(vq, vscsi->sg, out_num, in_num, cmd, gfp);
	if (ret >= 0)
		virtqueue_kick(vq);

	spin_unlock_irqrestore(&vscsi->vq_lock, flags);
	return ret;
}

static int virtscsi_queuecommand(struct Scsi_Host *sh, struct scsi_cmnd *sc)
{
	struct virtio_scsi *vscsi = shost_priv(sh);
	struct virtio_scsi_cmd *cmd;
	int ret;

	dev_dbg(&sc->device->sdev_gendev,
		"cmd %p CDB: %#02x\n", sc, sc->cmnd[0]);

	ret = SCSI_MLQUEUE_HOST_BUSY;
	cmd = mempool_alloc(virtscsi_cmd_pool, GFP_ATOMIC);
	if (!cmd)
		goto out;

	memset(cmd, 0, sizeof(*cmd));
	cmd->sc = sc;
	cmd->req.cmd = (struct virtio_scsi_cmd_req){
		.lun[0] = 1,
		.lun[1] = sc->device->id,
		.lun[2] = (sc->device->lun >> 8) | 0x40,
		.lun[3] = sc->device->lun & 0xff,
		.tag = (unsigned long)sc,
		.task_attr = VIRTIO_SCSI_S_SIMPLE,
		.prio = 0,
		.crn = 0,
	};

	BUG_ON(sc->cmd_len > VIRTIO_SCSI_CDB_SIZE);
	memcpy(cmd->req.cmd.cdb, sc->cmnd, sc->cmd_len);

	if (virtscsi_kick_cmd(vscsi, vscsi->req_vq, cmd,
			      sizeof cmd->req.cmd, sizeof cmd->resp.cmd,
			      GFP_ATOMIC) >= 0)
		ret = 0;

out:
	return ret;
=======
	struct Scsi_Host *sh = virtio_scsi_host(vq->vdev);
	struct virtio_scsi *vscsi = shost_priv(sh);

	virtscsi_vq_done(vscsi, &vscsi->ctrl_vq, virtscsi_complete_free);
};

static void virtscsi_handle_event(struct work_struct *work);

static int virtscsi_kick_event(struct virtio_scsi *vscsi,
			       struct virtio_scsi_event_node *event_node)
{
	int err;
	struct scatterlist sg;
	unsigned long flags;

	INIT_WORK(&event_node->work, virtscsi_handle_event);
	sg_init_one(&sg, &event_node->event, sizeof(struct virtio_scsi_event));

	spin_lock_irqsave(&vscsi->event_vq.vq_lock, flags);

	err = virtqueue_add_inbuf(vscsi->event_vq.vq, &sg, 1, event_node,
				  GFP_ATOMIC);
	if (!err)
		virtqueue_kick(vscsi->event_vq.vq);

	spin_unlock_irqrestore(&vscsi->event_vq.vq_lock, flags);

	return err;
}

static int virtscsi_kick_event_all(struct virtio_scsi *vscsi)
{
	int i;

	for (i = 0; i < VIRTIO_SCSI_EVENT_LEN; i++) {
		vscsi->event_list[i].vscsi = vscsi;
		virtscsi_kick_event(vscsi, &vscsi->event_list[i]);
	}

	return 0;
}

static void virtscsi_cancel_event_work(struct virtio_scsi *vscsi)
{
	int i;

	/* Stop scheduling work before calling cancel_work_sync.  */
	spin_lock_irq(&vscsi->event_vq.vq_lock);
	vscsi->stop_events = true;
	spin_unlock_irq(&vscsi->event_vq.vq_lock);

	for (i = 0; i < VIRTIO_SCSI_EVENT_LEN; i++)
		cancel_work_sync(&vscsi->event_list[i].work);
}

static void virtscsi_handle_transport_reset(struct virtio_scsi *vscsi,
					    struct virtio_scsi_event *event)
{
	struct scsi_device *sdev;
	struct Scsi_Host *shost = virtio_scsi_host(vscsi->vdev);
	unsigned int target = event->lun[1];
	unsigned int lun = (event->lun[2] << 8) | event->lun[3];

	switch (virtio32_to_cpu(vscsi->vdev, event->reason)) {
	case VIRTIO_SCSI_EVT_RESET_RESCAN:
		if (lun == 0) {
			scsi_scan_target(&shost->shost_gendev, 0, target,
					 SCAN_WILD_CARD, SCSI_SCAN_INITIAL);
		} else {
			scsi_add_device(shost, 0, target, lun);
		}
		break;
	case VIRTIO_SCSI_EVT_RESET_REMOVED:
		sdev = scsi_device_lookup(shost, 0, target, lun);
		if (sdev) {
			scsi_remove_device(sdev);
			scsi_device_put(sdev);
		} else {
			pr_err("SCSI device %d 0 %d %d not found\n",
				shost->host_no, target, lun);
		}
		break;
	default:
		pr_info("Unsupported virtio scsi event reason %x\n", event->reason);
	}
}

static void virtscsi_handle_param_change(struct virtio_scsi *vscsi,
					 struct virtio_scsi_event *event)
{
	struct scsi_device *sdev;
	struct Scsi_Host *shost = virtio_scsi_host(vscsi->vdev);
	unsigned int target = event->lun[1];
	unsigned int lun = (event->lun[2] << 8) | event->lun[3];
	u8 asc = virtio32_to_cpu(vscsi->vdev, event->reason) & 255;
	u8 ascq = virtio32_to_cpu(vscsi->vdev, event->reason) >> 8;

	sdev = scsi_device_lookup(shost, 0, target, lun);
	if (!sdev) {
		pr_err("SCSI device %d 0 %d %d not found\n",
			shost->host_no, target, lun);
		return;
	}

	/* Handle "Parameters changed", "Mode parameters changed", and
	   "Capacity data has changed".  */
	if (asc == 0x2a && (ascq == 0x00 || ascq == 0x01 || ascq == 0x09))
		scsi_rescan_device(sdev);

	scsi_device_put(sdev);
}

static int virtscsi_rescan_hotunplug(struct virtio_scsi *vscsi)
{
	struct scsi_device *sdev;
	struct Scsi_Host *shost = virtio_scsi_host(vscsi->vdev);
	unsigned char scsi_cmd[MAX_COMMAND_SIZE];
	int result, inquiry_len, inq_result_len = 256;
	char *inq_result = kmalloc(inq_result_len, GFP_KERNEL);

	if (!inq_result)
		return -ENOMEM;

	shost_for_each_device(sdev, shost) {
		inquiry_len = sdev->inquiry_len ? sdev->inquiry_len : 36;

		memset(scsi_cmd, 0, sizeof(scsi_cmd));
		scsi_cmd[0] = INQUIRY;
		scsi_cmd[4] = (unsigned char) inquiry_len;

		memset(inq_result, 0, inq_result_len);

		result = scsi_execute_cmd(sdev, scsi_cmd, REQ_OP_DRV_IN,
					  inq_result, inquiry_len,
					  SD_TIMEOUT, SD_MAX_RETRIES, NULL);

		if (result == 0 && inq_result[0] >> 5) {
			/* PQ indicates the LUN is not attached */
			scsi_remove_device(sdev);
		} else if (result > 0 && host_byte(result) == DID_BAD_TARGET) {
			/*
			 * If all LUNs of a virtio-scsi device are unplugged
			 * it will respond with BAD TARGET on any INQUIRY
			 * command.
			 * Remove the device in this case as well.
			 */
			scsi_remove_device(sdev);
		}
	}

	kfree(inq_result);
	return 0;
}

static void virtscsi_handle_event(struct work_struct *work)
{
	struct virtio_scsi_event_node *event_node =
		container_of(work, struct virtio_scsi_event_node, work);
	struct virtio_scsi *vscsi = event_node->vscsi;
	struct virtio_scsi_event *event = &event_node->event;

	if (event->event &
	    cpu_to_virtio32(vscsi->vdev, VIRTIO_SCSI_T_EVENTS_MISSED)) {
		int ret;

		event->event &= ~cpu_to_virtio32(vscsi->vdev,
						   VIRTIO_SCSI_T_EVENTS_MISSED);
		ret = virtscsi_rescan_hotunplug(vscsi);
		if (ret)
			return;
		scsi_scan_host(virtio_scsi_host(vscsi->vdev));
	}

	switch (virtio32_to_cpu(vscsi->vdev, event->event)) {
	case VIRTIO_SCSI_T_NO_EVENT:
		break;
	case VIRTIO_SCSI_T_TRANSPORT_RESET:
		virtscsi_handle_transport_reset(vscsi, event);
		break;
	case VIRTIO_SCSI_T_PARAM_CHANGE:
		virtscsi_handle_param_change(vscsi, event);
		break;
	default:
		pr_err("Unsupported virtio scsi event %x\n", event->event);
	}
	virtscsi_kick_event(vscsi, event_node);
}

static void virtscsi_complete_event(struct virtio_scsi *vscsi, void *buf)
{
	struct virtio_scsi_event_node *event_node = buf;

	if (!vscsi->stop_events)
		queue_work(system_freezable_wq, &event_node->work);
}

static void virtscsi_event_done(struct virtqueue *vq)
{
	struct Scsi_Host *sh = virtio_scsi_host(vq->vdev);
	struct virtio_scsi *vscsi = shost_priv(sh);

	virtscsi_vq_done(vscsi, &vscsi->event_vq, virtscsi_complete_event);
};

static int __virtscsi_add_cmd(struct virtqueue *vq,
			    struct virtio_scsi_cmd *cmd,
			    size_t req_size, size_t resp_size)
{
	struct scsi_cmnd *sc = cmd->sc;
	struct scatterlist *sgs[6], req, resp;
	struct sg_table *out, *in;
	unsigned out_num = 0, in_num = 0;

	out = in = NULL;

	if (sc && sc->sc_data_direction != DMA_NONE) {
		if (sc->sc_data_direction != DMA_FROM_DEVICE)
			out = &sc->sdb.table;
		if (sc->sc_data_direction != DMA_TO_DEVICE)
			in = &sc->sdb.table;
	}

	/* Request header.  */
	sg_init_one(&req, &cmd->req, req_size);
	sgs[out_num++] = &req;

	/* Data-out buffer.  */
	if (out) {
		/* Place WRITE protection SGLs before Data OUT payload */
		if (scsi_prot_sg_count(sc))
			sgs[out_num++] = scsi_prot_sglist(sc);
		sgs[out_num++] = out->sgl;
	}

	/* Response header.  */
	sg_init_one(&resp, &cmd->resp, resp_size);
	sgs[out_num + in_num++] = &resp;

	/* Data-in buffer */
	if (in) {
		/* Place READ protection SGLs before Data IN payload */
		if (scsi_prot_sg_count(sc))
			sgs[out_num + in_num++] = scsi_prot_sglist(sc);
		sgs[out_num + in_num++] = in->sgl;
	}

	return virtqueue_add_sgs(vq, sgs, out_num, in_num, cmd, GFP_ATOMIC);
}

static void virtscsi_kick_vq(struct virtio_scsi_vq *vq)
{
	bool needs_kick;
	unsigned long flags;

	spin_lock_irqsave(&vq->vq_lock, flags);
	needs_kick = virtqueue_kick_prepare(vq->vq);
	spin_unlock_irqrestore(&vq->vq_lock, flags);

	if (needs_kick)
		virtqueue_notify(vq->vq);
}

/**
 * virtscsi_add_cmd - add a virtio_scsi_cmd to a virtqueue, optionally kick it
 * @vq		: the struct virtqueue we're talking about
 * @cmd		: command structure
 * @req_size	: size of the request buffer
 * @resp_size	: size of the response buffer
 * @kick	: whether to kick the virtqueue immediately
 */
static int virtscsi_add_cmd(struct virtio_scsi_vq *vq,
			     struct virtio_scsi_cmd *cmd,
			     size_t req_size, size_t resp_size,
			     bool kick)
{
	unsigned long flags;
	int err;
	bool needs_kick = false;

	spin_lock_irqsave(&vq->vq_lock, flags);
	err = __virtscsi_add_cmd(vq->vq, cmd, req_size, resp_size);
	if (!err && kick)
		needs_kick = virtqueue_kick_prepare(vq->vq);

	spin_unlock_irqrestore(&vq->vq_lock, flags);

	if (needs_kick)
		virtqueue_notify(vq->vq);
	return err;
}

static void virtio_scsi_init_hdr(struct virtio_device *vdev,
				 struct virtio_scsi_cmd_req *cmd,
				 struct scsi_cmnd *sc)
{
	cmd->lun[0] = 1;
	cmd->lun[1] = sc->device->id;
	cmd->lun[2] = (sc->device->lun >> 8) | 0x40;
	cmd->lun[3] = sc->device->lun & 0xff;
	cmd->tag = cpu_to_virtio64(vdev, (unsigned long)sc);
	cmd->task_attr = VIRTIO_SCSI_S_SIMPLE;
	cmd->prio = 0;
	cmd->crn = 0;
}

#ifdef CONFIG_BLK_DEV_INTEGRITY
static void virtio_scsi_init_hdr_pi(struct virtio_device *vdev,
				    struct virtio_scsi_cmd_req_pi *cmd_pi,
				    struct scsi_cmnd *sc)
{
	struct request *rq = scsi_cmd_to_rq(sc);
	struct blk_integrity *bi;

	virtio_scsi_init_hdr(vdev, (struct virtio_scsi_cmd_req *)cmd_pi, sc);

	if (!rq || !scsi_prot_sg_count(sc))
		return;

	bi = blk_get_integrity(rq->q->disk);

	if (sc->sc_data_direction == DMA_TO_DEVICE)
		cmd_pi->pi_bytesout = cpu_to_virtio32(vdev,
						      bio_integrity_bytes(bi,
							blk_rq_sectors(rq)));
	else if (sc->sc_data_direction == DMA_FROM_DEVICE)
		cmd_pi->pi_bytesin = cpu_to_virtio32(vdev,
						     bio_integrity_bytes(bi,
							blk_rq_sectors(rq)));
}
#endif

static struct virtio_scsi_vq *virtscsi_pick_vq_mq(struct virtio_scsi *vscsi,
						  struct scsi_cmnd *sc)
{
	u32 tag = blk_mq_unique_tag(scsi_cmd_to_rq(sc));
	u16 hwq = blk_mq_unique_tag_to_hwq(tag);

	return &vscsi->req_vqs[hwq];
}

static int virtscsi_queuecommand(struct Scsi_Host *shost,
				 struct scsi_cmnd *sc)
{
	struct virtio_scsi *vscsi = shost_priv(shost);
	struct virtio_scsi_vq *req_vq = virtscsi_pick_vq_mq(vscsi, sc);
	struct virtio_scsi_cmd *cmd = scsi_cmd_priv(sc);
	bool kick;
	unsigned long flags;
	int req_size;
	int ret;

	BUG_ON(scsi_sg_count(sc) > shost->sg_tablesize);

	/* TODO: check feature bit and fail if unsupported?  */
	BUG_ON(sc->sc_data_direction == DMA_BIDIRECTIONAL);

	dev_dbg(&sc->device->sdev_gendev,
		"cmd %p CDB: %#02x\n", sc, sc->cmnd[0]);

	cmd->sc = sc;

	BUG_ON(sc->cmd_len > VIRTIO_SCSI_CDB_SIZE);

#ifdef CONFIG_BLK_DEV_INTEGRITY
	if (virtio_has_feature(vscsi->vdev, VIRTIO_SCSI_F_T10_PI)) {
		virtio_scsi_init_hdr_pi(vscsi->vdev, &cmd->req.cmd_pi, sc);
		memcpy(cmd->req.cmd_pi.cdb, sc->cmnd, sc->cmd_len);
		req_size = sizeof(cmd->req.cmd_pi);
	} else
#endif
	{
		virtio_scsi_init_hdr(vscsi->vdev, &cmd->req.cmd, sc);
		memcpy(cmd->req.cmd.cdb, sc->cmnd, sc->cmd_len);
		req_size = sizeof(cmd->req.cmd);
	}

	kick = (sc->flags & SCMD_LAST) != 0;
	ret = virtscsi_add_cmd(req_vq, cmd, req_size, sizeof(cmd->resp.cmd), kick);
	if (ret == -EIO) {
		cmd->resp.cmd.response = VIRTIO_SCSI_S_BAD_TARGET;
		spin_lock_irqsave(&req_vq->vq_lock, flags);
		virtscsi_complete_cmd(vscsi, cmd);
		spin_unlock_irqrestore(&req_vq->vq_lock, flags);
	} else if (ret != 0) {
		return SCSI_MLQUEUE_HOST_BUSY;
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int virtscsi_tmf(struct virtio_scsi *vscsi, struct virtio_scsi_cmd *cmd)
{
	DECLARE_COMPLETION_ONSTACK(comp);
	int ret = FAILED;

	cmd->comp = &comp;
<<<<<<< HEAD
	if (virtscsi_kick_cmd(vscsi, vscsi->ctrl_vq, cmd,
			      sizeof cmd->req.tmf, sizeof cmd->resp.tmf,
			      GFP_NOIO) < 0)
=======
	if (virtscsi_add_cmd(&vscsi->ctrl_vq, cmd,
			      sizeof cmd->req.tmf, sizeof cmd->resp.tmf, true) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	wait_for_completion(&comp);
	if (cmd->resp.tmf.response == VIRTIO_SCSI_S_OK ||
	    cmd->resp.tmf.response == VIRTIO_SCSI_S_FUNCTION_SUCCEEDED)
		ret = SUCCESS;

<<<<<<< HEAD
=======
	/*
	 * The spec guarantees that all requests related to the TMF have
	 * been completed, but the callback might not have run yet if
	 * we're using independent interrupts (e.g. MSI).  Poll the
	 * virtqueues once.
	 *
	 * In the abort case, scsi_done() will do nothing, because the
	 * command timed out and hence SCMD_STATE_COMPLETE has been set.
	 */
	virtscsi_poll_requests(vscsi);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	mempool_free(cmd, virtscsi_cmd_pool);
	return ret;
}

static int virtscsi_device_reset(struct scsi_cmnd *sc)
{
	struct virtio_scsi *vscsi = shost_priv(sc->device->host);
	struct virtio_scsi_cmd *cmd;

	sdev_printk(KERN_INFO, sc->device, "device reset\n");
	cmd = mempool_alloc(virtscsi_cmd_pool, GFP_NOIO);
	if (!cmd)
		return FAILED;

	memset(cmd, 0, sizeof(*cmd));
<<<<<<< HEAD
	cmd->sc = sc;
	cmd->req.tmf = (struct virtio_scsi_ctrl_tmf_req){
		.type = VIRTIO_SCSI_T_TMF,
		.subtype = VIRTIO_SCSI_T_TMF_LOGICAL_UNIT_RESET,
=======
	cmd->req.tmf = (struct virtio_scsi_ctrl_tmf_req){
		.type = VIRTIO_SCSI_T_TMF,
		.subtype = cpu_to_virtio32(vscsi->vdev,
					     VIRTIO_SCSI_T_TMF_LOGICAL_UNIT_RESET),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.lun[0] = 1,
		.lun[1] = sc->device->id,
		.lun[2] = (sc->device->lun >> 8) | 0x40,
		.lun[3] = sc->device->lun & 0xff,
	};
	return virtscsi_tmf(vscsi, cmd);
}

<<<<<<< HEAD
=======
static int virtscsi_device_alloc(struct scsi_device *sdevice)
{
	/*
	 * Passed through SCSI targets (e.g. with qemu's 'scsi-block')
	 * may have transfer limits which come from the host SCSI
	 * controller or something on the host side other than the
	 * target itself.
	 *
	 * To make this work properly, the hypervisor can adjust the
	 * target's VPD information to advertise these limits.  But
	 * for that to work, the guest has to look at the VPD pages,
	 * which we won't do by default if it is an SPC-2 device, even
	 * if it does actually support it.
	 *
	 * So, set the blist to always try to read the VPD pages.
	 */
	sdevice->sdev_bflags = BLIST_TRY_VPD_PAGES;

	return 0;
}


/**
 * virtscsi_change_queue_depth() - Change a virtscsi target's queue depth
 * @sdev:	Virtscsi target whose queue depth to change
 * @qdepth:	New queue depth
 */
static int virtscsi_change_queue_depth(struct scsi_device *sdev, int qdepth)
{
	struct Scsi_Host *shost = sdev->host;
	int max_depth = shost->cmd_per_lun;

	return scsi_change_queue_depth(sdev, min(max_depth, qdepth));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int virtscsi_abort(struct scsi_cmnd *sc)
{
	struct virtio_scsi *vscsi = shost_priv(sc->device->host);
	struct virtio_scsi_cmd *cmd;

	scmd_printk(KERN_INFO, sc, "abort\n");
	cmd = mempool_alloc(virtscsi_cmd_pool, GFP_NOIO);
	if (!cmd)
		return FAILED;

	memset(cmd, 0, sizeof(*cmd));
<<<<<<< HEAD
	cmd->sc = sc;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cmd->req.tmf = (struct virtio_scsi_ctrl_tmf_req){
		.type = VIRTIO_SCSI_T_TMF,
		.subtype = VIRTIO_SCSI_T_TMF_ABORT_TASK,
		.lun[0] = 1,
		.lun[1] = sc->device->id,
		.lun[2] = (sc->device->lun >> 8) | 0x40,
		.lun[3] = sc->device->lun & 0xff,
<<<<<<< HEAD
		.tag = (unsigned long)sc,
=======
		.tag = cpu_to_virtio64(vscsi->vdev, (unsigned long)sc),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	};
	return virtscsi_tmf(vscsi, cmd);
}

<<<<<<< HEAD
static struct scsi_host_template virtscsi_host_template = {
	.module = THIS_MODULE,
	.name = "Virtio SCSI HBA",
	.proc_name = "virtio_scsi",
	.queuecommand = virtscsi_queuecommand,
	.this_id = -1,
	.eh_abort_handler = virtscsi_abort,
	.eh_device_reset_handler = virtscsi_device_reset,

	.can_queue = 1024,
	.dma_boundary = UINT_MAX,
	.use_clustering = ENABLE_CLUSTERING,
=======
static void virtscsi_map_queues(struct Scsi_Host *shost)
{
	struct virtio_scsi *vscsi = shost_priv(shost);
	int i, qoff;

	for (i = 0, qoff = 0; i < shost->nr_maps; i++) {
		struct blk_mq_queue_map *map = &shost->tag_set.map[i];

		map->nr_queues = vscsi->io_queues[i];
		map->queue_offset = qoff;
		qoff += map->nr_queues;

		if (map->nr_queues == 0)
			continue;

		/*
		 * Regular queues have interrupts and hence CPU affinity is
		 * defined by the core virtio code, but polling queues have
		 * no interrupts so we let the block layer assign CPU affinity.
		 */
		if (i == HCTX_TYPE_POLL)
			blk_mq_map_queues(map);
		else
			blk_mq_virtio_map_queues(map, vscsi->vdev, 2);
	}
}

static int virtscsi_mq_poll(struct Scsi_Host *shost, unsigned int queue_num)
{
	struct virtio_scsi *vscsi = shost_priv(shost);
	struct virtio_scsi_vq *virtscsi_vq = &vscsi->req_vqs[queue_num];
	unsigned long flags;
	unsigned int len;
	int found = 0;
	void *buf;

	spin_lock_irqsave(&virtscsi_vq->vq_lock, flags);

	while ((buf = virtqueue_get_buf(virtscsi_vq->vq, &len)) != NULL) {
		virtscsi_complete_cmd(vscsi, buf);
		found++;
	}

	spin_unlock_irqrestore(&virtscsi_vq->vq_lock, flags);

	return found;
}

static void virtscsi_commit_rqs(struct Scsi_Host *shost, u16 hwq)
{
	struct virtio_scsi *vscsi = shost_priv(shost);

	virtscsi_kick_vq(&vscsi->req_vqs[hwq]);
}

/*
 * The host guarantees to respond to each command, although I/O
 * latencies might be higher than on bare metal.  Reset the timer
 * unconditionally to give the host a chance to perform EH.
 */
static enum scsi_timeout_action virtscsi_eh_timed_out(struct scsi_cmnd *scmnd)
{
	return SCSI_EH_RESET_TIMER;
}

static const struct scsi_host_template virtscsi_host_template = {
	.module = THIS_MODULE,
	.name = "Virtio SCSI HBA",
	.proc_name = "virtio_scsi",
	.this_id = -1,
	.cmd_size = sizeof(struct virtio_scsi_cmd),
	.queuecommand = virtscsi_queuecommand,
	.mq_poll = virtscsi_mq_poll,
	.commit_rqs = virtscsi_commit_rqs,
	.change_queue_depth = virtscsi_change_queue_depth,
	.eh_abort_handler = virtscsi_abort,
	.eh_device_reset_handler = virtscsi_device_reset,
	.eh_timed_out = virtscsi_eh_timed_out,
	.slave_alloc = virtscsi_device_alloc,

	.dma_boundary = UINT_MAX,
	.map_queues = virtscsi_map_queues,
	.track_queue_depth = 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define virtscsi_config_get(vdev, fld) \
	({ \
<<<<<<< HEAD
		typeof(((struct virtio_scsi_config *)0)->fld) __val; \
		vdev->config->get(vdev, \
				  offsetof(struct virtio_scsi_config, fld), \
				  &__val, sizeof(__val)); \
=======
		__virtio_native_type(struct virtio_scsi_config, fld) __val; \
		virtio_cread(vdev, struct virtio_scsi_config, fld, &__val); \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__val; \
	})

#define virtscsi_config_set(vdev, fld, val) \
<<<<<<< HEAD
	(void)({ \
		typeof(((struct virtio_scsi_config *)0)->fld) __val = (val); \
		vdev->config->set(vdev, \
				  offsetof(struct virtio_scsi_config, fld), \
				  &__val, sizeof(__val)); \
	})
=======
	do { \
		__virtio_native_type(struct virtio_scsi_config, fld) __val = (val); \
		virtio_cwrite(vdev, struct virtio_scsi_config, fld, &__val); \
	} while(0)

static void virtscsi_init_vq(struct virtio_scsi_vq *virtscsi_vq,
			     struct virtqueue *vq)
{
	spin_lock_init(&virtscsi_vq->vq_lock);
	virtscsi_vq->vq = vq;
}

static void virtscsi_remove_vqs(struct virtio_device *vdev)
{
	/* Stop all the virtqueues. */
	virtio_reset_device(vdev);
	vdev->config->del_vqs(vdev);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int virtscsi_init(struct virtio_device *vdev,
			 struct virtio_scsi *vscsi)
{
	int err;
<<<<<<< HEAD
	struct virtqueue *vqs[3];
	vq_callback_t *callbacks[] = {
		virtscsi_ctrl_done,
		virtscsi_event_done,
		virtscsi_req_done
	};
	const char *names[] = {
		"control",
		"event",
		"request"
	};

	/* Discover virtqueues and write information to configuration.  */
	err = vdev->config->find_vqs(vdev, 3, vqs, callbacks, names);
	if (err)
		return err;

	vscsi->ctrl_vq = vqs[0];
	vscsi->event_vq = vqs[1];
	vscsi->req_vq = vqs[2];

	virtscsi_config_set(vdev, cdb_size, VIRTIO_SCSI_CDB_SIZE);
	virtscsi_config_set(vdev, sense_size, VIRTIO_SCSI_SENSE_SIZE);
	return 0;
}

static int __devinit virtscsi_probe(struct virtio_device *vdev)
=======
	u32 i;
	u32 num_vqs, num_poll_vqs, num_req_vqs;
	vq_callback_t **callbacks;
	const char **names;
	struct virtqueue **vqs;
	struct irq_affinity desc = { .pre_vectors = 2 };

	num_req_vqs = vscsi->num_queues;
	num_vqs = num_req_vqs + VIRTIO_SCSI_VQ_BASE;
	vqs = kmalloc_array(num_vqs, sizeof(struct virtqueue *), GFP_KERNEL);
	callbacks = kmalloc_array(num_vqs, sizeof(vq_callback_t *),
				  GFP_KERNEL);
	names = kmalloc_array(num_vqs, sizeof(char *), GFP_KERNEL);

	if (!callbacks || !vqs || !names) {
		err = -ENOMEM;
		goto out;
	}

	num_poll_vqs = min_t(unsigned int, virtscsi_poll_queues,
			     num_req_vqs - 1);
	vscsi->io_queues[HCTX_TYPE_DEFAULT] = num_req_vqs - num_poll_vqs;
	vscsi->io_queues[HCTX_TYPE_READ] = 0;
	vscsi->io_queues[HCTX_TYPE_POLL] = num_poll_vqs;

	dev_info(&vdev->dev, "%d/%d/%d default/read/poll queues\n",
		 vscsi->io_queues[HCTX_TYPE_DEFAULT],
		 vscsi->io_queues[HCTX_TYPE_READ],
		 vscsi->io_queues[HCTX_TYPE_POLL]);

	callbacks[0] = virtscsi_ctrl_done;
	callbacks[1] = virtscsi_event_done;
	names[0] = "control";
	names[1] = "event";
	for (i = VIRTIO_SCSI_VQ_BASE; i < num_vqs - num_poll_vqs; i++) {
		callbacks[i] = virtscsi_req_done;
		names[i] = "request";
	}

	for (; i < num_vqs; i++) {
		callbacks[i] = NULL;
		names[i] = "request_poll";
	}

	/* Discover virtqueues and write information to configuration.  */
	err = virtio_find_vqs(vdev, num_vqs, vqs, callbacks, names, &desc);
	if (err)
		goto out;

	virtscsi_init_vq(&vscsi->ctrl_vq, vqs[0]);
	virtscsi_init_vq(&vscsi->event_vq, vqs[1]);
	for (i = VIRTIO_SCSI_VQ_BASE; i < num_vqs; i++)
		virtscsi_init_vq(&vscsi->req_vqs[i - VIRTIO_SCSI_VQ_BASE],
				 vqs[i]);

	virtscsi_config_set(vdev, cdb_size, VIRTIO_SCSI_CDB_SIZE);
	virtscsi_config_set(vdev, sense_size, VIRTIO_SCSI_SENSE_SIZE);

	err = 0;

out:
	kfree(names);
	kfree(callbacks);
	kfree(vqs);
	if (err)
		virtscsi_remove_vqs(vdev);
	return err;
}

static int virtscsi_probe(struct virtio_device *vdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct Scsi_Host *shost;
	struct virtio_scsi *vscsi;
	int err;
<<<<<<< HEAD
	u32 sg_elems;
	u32 cmd_per_lun;

	/* We need to know how many segments before we allocate.
	 * We need an extra sg elements at head and tail.
	 */
	sg_elems = virtscsi_config_get(vdev, seg_max) ?: 1;

	/* Allocate memory and link the structs together.  */
	shost = scsi_host_alloc(&virtscsi_host_template,
		sizeof(*vscsi) + sizeof(vscsi->sg[0]) * (sg_elems + 2));

	if (!shost)
		return -ENOMEM;

	shost->sg_tablesize = sg_elems;
	vscsi = shost_priv(shost);
	vscsi->vdev = vdev;
	vdev->priv = shost;

	/* Random initializations.  */
	spin_lock_init(&vscsi->vq_lock);
	sg_init_table(vscsi->sg, sg_elems + 2);

=======
	u32 sg_elems, num_targets;
	u32 cmd_per_lun;
	u32 num_queues;

	if (!vdev->config->get) {
		dev_err(&vdev->dev, "%s failure: config access disabled\n",
			__func__);
		return -EINVAL;
	}

	/* We need to know how many queues before we allocate. */
	num_queues = virtscsi_config_get(vdev, num_queues) ? : 1;
	num_queues = min_t(unsigned int, nr_cpu_ids, num_queues);

	num_targets = virtscsi_config_get(vdev, max_target) + 1;

	shost = scsi_host_alloc(&virtscsi_host_template,
				struct_size(vscsi, req_vqs, num_queues));
	if (!shost)
		return -ENOMEM;

	sg_elems = virtscsi_config_get(vdev, seg_max) ?: 1;
	shost->sg_tablesize = sg_elems;
	shost->nr_maps = 1;
	vscsi = shost_priv(shost);
	vscsi->vdev = vdev;
	vscsi->num_queues = num_queues;
	vdev->priv = shost;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = virtscsi_init(vdev, vscsi);
	if (err)
		goto virtscsi_init_failed;

<<<<<<< HEAD
	cmd_per_lun = virtscsi_config_get(vdev, cmd_per_lun) ?: 1;
	shost->cmd_per_lun = min_t(u32, cmd_per_lun, shost->can_queue);
	shost->max_sectors = virtscsi_config_get(vdev, max_sectors) ?: 0xFFFF;
	shost->max_lun = virtscsi_config_get(vdev, max_lun) + 1;
	shost->max_id = virtscsi_config_get(vdev, max_target) + 1;
	shost->max_channel = 0;
	shost->max_cmd_len = VIRTIO_SCSI_CDB_SIZE;
=======
	if (vscsi->io_queues[HCTX_TYPE_POLL])
		shost->nr_maps = HCTX_TYPE_POLL + 1;

	shost->can_queue = virtqueue_get_vring_size(vscsi->req_vqs[0].vq);

	cmd_per_lun = virtscsi_config_get(vdev, cmd_per_lun) ?: 1;
	shost->cmd_per_lun = min_t(u32, cmd_per_lun, shost->can_queue);
	shost->max_sectors = virtscsi_config_get(vdev, max_sectors) ?: 0xFFFF;

	/* LUNs > 256 are reported with format 1, so they go in the range
	 * 16640-32767.
	 */
	shost->max_lun = virtscsi_config_get(vdev, max_lun) + 1 + 0x4000;
	shost->max_id = num_targets;
	shost->max_channel = 0;
	shost->max_cmd_len = VIRTIO_SCSI_CDB_SIZE;
	shost->nr_hw_queues = num_queues;

#ifdef CONFIG_BLK_DEV_INTEGRITY
	if (virtio_has_feature(vdev, VIRTIO_SCSI_F_T10_PI)) {
		int host_prot;

		host_prot = SHOST_DIF_TYPE1_PROTECTION | SHOST_DIF_TYPE2_PROTECTION |
			    SHOST_DIF_TYPE3_PROTECTION | SHOST_DIX_TYPE1_PROTECTION |
			    SHOST_DIX_TYPE2_PROTECTION | SHOST_DIX_TYPE3_PROTECTION;

		scsi_host_set_prot(shost, host_prot);
		scsi_host_set_guard(shost, SHOST_DIX_GUARD_CRC);
	}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = scsi_add_host(shost, &vdev->dev);
	if (err)
		goto scsi_add_host_failed;

<<<<<<< HEAD
	scsi_scan_host(shost);

=======
	virtio_device_ready(vdev);

	if (virtio_has_feature(vdev, VIRTIO_SCSI_F_HOTPLUG))
		virtscsi_kick_event_all(vscsi);

	scsi_scan_host(shost);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

scsi_add_host_failed:
	vdev->config->del_vqs(vdev);
virtscsi_init_failed:
	scsi_host_put(shost);
	return err;
}

<<<<<<< HEAD
static void virtscsi_remove_vqs(struct virtio_device *vdev)
{
	/* Stop all the virtqueues. */
	vdev->config->reset(vdev);

	vdev->config->del_vqs(vdev);
}

static void __devexit virtscsi_remove(struct virtio_device *vdev)
{
	struct Scsi_Host *shost = virtio_scsi_host(vdev);

	scsi_remove_host(shost);

=======
static void virtscsi_remove(struct virtio_device *vdev)
{
	struct Scsi_Host *shost = virtio_scsi_host(vdev);
	struct virtio_scsi *vscsi = shost_priv(shost);

	if (virtio_has_feature(vdev, VIRTIO_SCSI_F_HOTPLUG))
		virtscsi_cancel_event_work(vscsi);

	scsi_remove_host(shost);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	virtscsi_remove_vqs(vdev);
	scsi_host_put(shost);
}

<<<<<<< HEAD
#ifdef CONFIG_PM
=======
#ifdef CONFIG_PM_SLEEP
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int virtscsi_freeze(struct virtio_device *vdev)
{
	virtscsi_remove_vqs(vdev);
	return 0;
}

static int virtscsi_restore(struct virtio_device *vdev)
{
	struct Scsi_Host *sh = virtio_scsi_host(vdev);
	struct virtio_scsi *vscsi = shost_priv(sh);
<<<<<<< HEAD

	return virtscsi_init(vdev, vscsi);
=======
	int err;

	err = virtscsi_init(vdev, vscsi);
	if (err)
		return err;

	virtio_device_ready(vdev);

	if (virtio_has_feature(vdev, VIRTIO_SCSI_F_HOTPLUG))
		virtscsi_kick_event_all(vscsi);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

static struct virtio_device_id id_table[] = {
	{ VIRTIO_ID_SCSI, VIRTIO_DEV_ANY_ID },
	{ 0 },
};

<<<<<<< HEAD
static struct virtio_driver virtio_scsi_driver = {
=======
static unsigned int features[] = {
	VIRTIO_SCSI_F_HOTPLUG,
	VIRTIO_SCSI_F_CHANGE,
#ifdef CONFIG_BLK_DEV_INTEGRITY
	VIRTIO_SCSI_F_T10_PI,
#endif
};

static struct virtio_driver virtio_scsi_driver = {
	.feature_table = features,
	.feature_table_size = ARRAY_SIZE(features),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.driver.name = KBUILD_MODNAME,
	.driver.owner = THIS_MODULE,
	.id_table = id_table,
	.probe = virtscsi_probe,
<<<<<<< HEAD
#ifdef CONFIG_PM
	.freeze = virtscsi_freeze,
	.restore = virtscsi_restore,
#endif
	.remove = __devexit_p(virtscsi_remove),
};

static int __init init(void)
=======
#ifdef CONFIG_PM_SLEEP
	.freeze = virtscsi_freeze,
	.restore = virtscsi_restore,
#endif
	.remove = virtscsi_remove,
};

static int __init virtio_scsi_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret = -ENOMEM;

	virtscsi_cmd_cache = KMEM_CACHE(virtio_scsi_cmd, 0);
	if (!virtscsi_cmd_cache) {
<<<<<<< HEAD
		printk(KERN_ERR "kmem_cache_create() for "
				"virtscsi_cmd_cache failed\n");
=======
		pr_err("kmem_cache_create() for virtscsi_cmd_cache failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto error;
	}


	virtscsi_cmd_pool =
		mempool_create_slab_pool(VIRTIO_SCSI_MEMPOOL_SZ,
					 virtscsi_cmd_cache);
	if (!virtscsi_cmd_pool) {
<<<<<<< HEAD
		printk(KERN_ERR "mempool_create() for"
				"virtscsi_cmd_pool failed\n");
=======
		pr_err("mempool_create() for virtscsi_cmd_pool failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto error;
	}
	ret = register_virtio_driver(&virtio_scsi_driver);
	if (ret < 0)
		goto error;

	return 0;

error:
<<<<<<< HEAD
	if (virtscsi_cmd_pool) {
		mempool_destroy(virtscsi_cmd_pool);
		virtscsi_cmd_pool = NULL;
	}
	if (virtscsi_cmd_cache) {
		kmem_cache_destroy(virtscsi_cmd_cache);
		virtscsi_cmd_cache = NULL;
	}
	return ret;
}

static void __exit fini(void)
=======
	mempool_destroy(virtscsi_cmd_pool);
	virtscsi_cmd_pool = NULL;
	kmem_cache_destroy(virtscsi_cmd_cache);
	virtscsi_cmd_cache = NULL;
	return ret;
}

static void __exit virtio_scsi_fini(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unregister_virtio_driver(&virtio_scsi_driver);
	mempool_destroy(virtscsi_cmd_pool);
	kmem_cache_destroy(virtscsi_cmd_cache);
}
<<<<<<< HEAD
module_init(init);
module_exit(fini);
=======
module_init(virtio_scsi_init);
module_exit(virtio_scsi_fini);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_DEVICE_TABLE(virtio, id_table);
MODULE_DESCRIPTION("Virtio SCSI HBA driver");
MODULE_LICENSE("GPL");
