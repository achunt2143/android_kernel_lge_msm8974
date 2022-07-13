<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Copyright (c) 2009, Citrix Systems, Inc.
 *  Copyright (c) 2010, Microsoft Corporation.
 *  Copyright (c) 2011, Novell Inc.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms and conditions of the GNU General Public License,
 *  version 2, as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/completion.h>
#include <linux/input.h>
#include <linux/hid.h>
#include <linux/hiddev.h>
#include <linux/hyperv.h>


struct hv_input_dev_info {
	unsigned int size;
	unsigned short vendor;
	unsigned short product;
	unsigned short version;
	unsigned short reserved[11];
};

<<<<<<< HEAD
/* The maximum size of a synthetic input message. */
#define SYNTHHID_MAX_INPUT_REPORT_SIZE 16

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Current version
 *
 * History:
 * Beta, RC < 2008/1/22        1,0
 * RC > 2008/1/22              2,0
 */
#define SYNTHHID_INPUT_VERSION_MAJOR	2
#define SYNTHHID_INPUT_VERSION_MINOR	0
#define SYNTHHID_INPUT_VERSION		(SYNTHHID_INPUT_VERSION_MINOR | \
					 (SYNTHHID_INPUT_VERSION_MAJOR << 16))


#pragma pack(push, 1)
/*
 * Message types in the synthetic input protocol
 */
enum synthhid_msg_type {
	SYNTH_HID_PROTOCOL_REQUEST,
	SYNTH_HID_PROTOCOL_RESPONSE,
	SYNTH_HID_INITIAL_DEVICE_INFO,
	SYNTH_HID_INITIAL_DEVICE_INFO_ACK,
	SYNTH_HID_INPUT_REPORT,
	SYNTH_HID_MAX
};

/*
 * Basic message structures.
 */
struct synthhid_msg_hdr {
	enum synthhid_msg_type type;
	u32 size;
};

<<<<<<< HEAD
struct synthhid_msg {
	struct synthhid_msg_hdr header;
	char data[1]; /* Enclosed message */
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
union synthhid_version {
	struct {
		u16 minor_version;
		u16 major_version;
	};
	u32 version;
};

/*
 * Protocol messages
 */
struct synthhid_protocol_request {
	struct synthhid_msg_hdr header;
	union synthhid_version version_requested;
};

struct synthhid_protocol_response {
	struct synthhid_msg_hdr header;
	union synthhid_version version_requested;
	unsigned char approved;
};

struct synthhid_device_info {
	struct synthhid_msg_hdr header;
	struct hv_input_dev_info hid_dev_info;
	struct hid_descriptor hid_descriptor;
};

struct synthhid_device_info_ack {
	struct synthhid_msg_hdr header;
	unsigned char reserved;
};

struct synthhid_input_report {
	struct synthhid_msg_hdr header;
<<<<<<< HEAD
	char buffer[1];
=======
	char buffer[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#pragma pack(pop)

<<<<<<< HEAD
#define INPUTVSC_SEND_RING_BUFFER_SIZE		(10*PAGE_SIZE)
#define INPUTVSC_RECV_RING_BUFFER_SIZE		(10*PAGE_SIZE)
=======
#define INPUTVSC_SEND_RING_BUFFER_SIZE	VMBUS_RING_SIZE(36 * 1024)
#define INPUTVSC_RECV_RING_BUFFER_SIZE	VMBUS_RING_SIZE(36 * 1024)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


enum pipe_prot_msg_type {
	PIPE_MESSAGE_INVALID,
	PIPE_MESSAGE_DATA,
	PIPE_MESSAGE_MAXIMUM
};


struct pipe_prt_msg {
	enum pipe_prot_msg_type type;
	u32 size;
<<<<<<< HEAD
	char data[1];
=======
	char data[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct  mousevsc_prt_msg {
	enum pipe_prot_msg_type type;
	u32 size;
	union {
		struct synthhid_protocol_request request;
		struct synthhid_protocol_response response;
		struct synthhid_device_info_ack ack;
	};
};

/*
 * Represents an mousevsc device
 */
struct mousevsc_dev {
	struct hv_device	*device;
	bool			init_complete;
	bool			connected;
	struct mousevsc_prt_msg	protocol_req;
	struct mousevsc_prt_msg	protocol_resp;
	/* Synchronize the request/response if needed */
	struct completion	wait_event;
	int			dev_info_status;

	struct hid_descriptor	*hid_desc;
	unsigned char		*report_desc;
	u32			report_desc_size;
	struct hv_input_dev_info hid_dev_info;
	struct hid_device       *hid_device;
<<<<<<< HEAD
=======
	u8			input_buf[HID_MAX_BUFFER_SIZE];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};


static struct mousevsc_dev *mousevsc_alloc_device(struct hv_device *device)
{
	struct mousevsc_dev *input_dev;

	input_dev = kzalloc(sizeof(struct mousevsc_dev), GFP_KERNEL);

	if (!input_dev)
		return NULL;

	input_dev->device = device;
	hv_set_drvdata(device, input_dev);
	init_completion(&input_dev->wait_event);
	input_dev->init_complete = false;

	return input_dev;
}

static void mousevsc_free_device(struct mousevsc_dev *device)
{
	kfree(device->hid_desc);
	kfree(device->report_desc);
	hv_set_drvdata(device->device, NULL);
	kfree(device);
}

static void mousevsc_on_receive_device_info(struct mousevsc_dev *input_device,
				struct synthhid_device_info *device_info)
{
	int ret = 0;
	struct hid_descriptor *desc;
	struct mousevsc_prt_msg ack;

	input_device->dev_info_status = -ENOMEM;

	input_device->hid_dev_info = device_info->hid_dev_info;
	desc = &device_info->hid_descriptor;
	if (desc->bLength == 0)
		goto cleanup;

<<<<<<< HEAD
	input_device->hid_desc = kzalloc(desc->bLength, GFP_ATOMIC);
=======
	/* The pointer is not NULL when we resume from hibernation */
	kfree(input_device->hid_desc);
	input_device->hid_desc = kmemdup(desc, desc->bLength, GFP_ATOMIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!input_device->hid_desc)
		goto cleanup;

<<<<<<< HEAD
	memcpy(input_device->hid_desc, desc, desc->bLength);

	input_device->report_desc_size = desc->desc[0].wDescriptorLength;
=======
	input_device->report_desc_size = le16_to_cpu(
					desc->desc[0].wDescriptorLength);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (input_device->report_desc_size == 0) {
		input_device->dev_info_status = -EINVAL;
		goto cleanup;
	}

<<<<<<< HEAD
=======
	/* The pointer is not NULL when we resume from hibernation */
	kfree(input_device->report_desc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	input_device->report_desc = kzalloc(input_device->report_desc_size,
					  GFP_ATOMIC);

	if (!input_device->report_desc) {
		input_device->dev_info_status = -ENOMEM;
		goto cleanup;
	}

	memcpy(input_device->report_desc,
	       ((unsigned char *)desc) + desc->bLength,
<<<<<<< HEAD
	       desc->desc[0].wDescriptorLength);
=======
	       le16_to_cpu(desc->desc[0].wDescriptorLength));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Send the ack */
	memset(&ack, 0, sizeof(struct mousevsc_prt_msg));

	ack.type = PIPE_MESSAGE_DATA;
	ack.size = sizeof(struct synthhid_device_info_ack);

	ack.ack.header.type = SYNTH_HID_INITIAL_DEVICE_INFO_ACK;
	ack.ack.header.size = 1;
	ack.ack.reserved = 0;

	ret = vmbus_sendpacket(input_device->device->channel,
			&ack,
<<<<<<< HEAD
			sizeof(struct pipe_prt_msg) - sizeof(unsigned char) +
=======
			sizeof(struct pipe_prt_msg) +
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			sizeof(struct synthhid_device_info_ack),
			(unsigned long)&ack,
			VM_PKT_DATA_INBAND,
			VMBUS_DATA_PACKET_FLAG_COMPLETION_REQUESTED);

	if (!ret)
		input_device->dev_info_status = 0;

cleanup:
	complete(&input_device->wait_event);

	return;
}

static void mousevsc_on_receive(struct hv_device *device,
				struct vmpacket_descriptor *packet)
{
	struct pipe_prt_msg *pipe_msg;
<<<<<<< HEAD
	struct synthhid_msg *hid_msg;
	struct mousevsc_dev *input_dev = hv_get_drvdata(device);
	struct synthhid_input_report *input_report;
=======
	struct synthhid_msg_hdr *hid_msg_hdr;
	struct mousevsc_dev *input_dev = hv_get_drvdata(device);
	struct synthhid_input_report *input_report;
	size_t len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pipe_msg = (struct pipe_prt_msg *)((unsigned long)packet +
						(packet->offset8 << 3));

	if (pipe_msg->type != PIPE_MESSAGE_DATA)
		return;

<<<<<<< HEAD
	hid_msg = (struct synthhid_msg *)pipe_msg->data;

	switch (hid_msg->header.type) {
	case SYNTH_HID_PROTOCOL_RESPONSE:
=======
	hid_msg_hdr = (struct synthhid_msg_hdr *)pipe_msg->data;

	switch (hid_msg_hdr->type) {
	case SYNTH_HID_PROTOCOL_RESPONSE:
		len = struct_size(pipe_msg, data, pipe_msg->size);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * While it will be impossible for us to protect against
		 * malicious/buggy hypervisor/host, add a check here to
		 * ensure we don't corrupt memory.
		 */
<<<<<<< HEAD
		if ((pipe_msg->size + sizeof(struct pipe_prt_msg)
			- sizeof(unsigned char))
			> sizeof(struct mousevsc_prt_msg)) {
			WARN_ON(1);
			break;
		}

		memcpy(&input_dev->protocol_resp, pipe_msg,
		       pipe_msg->size + sizeof(struct pipe_prt_msg) -
		       sizeof(unsigned char));
=======
		if (WARN_ON(len > sizeof(struct mousevsc_prt_msg)))
			break;

		memcpy(&input_dev->protocol_resp, pipe_msg, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		complete(&input_dev->wait_event);
		break;

	case SYNTH_HID_INITIAL_DEVICE_INFO:
		WARN_ON(pipe_msg->size < sizeof(struct hv_input_dev_info));

		/*
		 * Parse out the device info into device attr,
		 * hid desc and report desc
		 */
		mousevsc_on_receive_device_info(input_dev,
			(struct synthhid_device_info *)pipe_msg->data);
		break;
	case SYNTH_HID_INPUT_REPORT:
		input_report =
			(struct synthhid_input_report *)pipe_msg->data;
		if (!input_dev->init_complete)
			break;
<<<<<<< HEAD
		hid_input_report(input_dev->hid_device,
				HID_INPUT_REPORT, input_report->buffer,
				input_report->header.size, 1);
		break;
	default:
		pr_err("unsupported hid msg type - type %d len %d",
		       hid_msg->header.type, hid_msg->header.size);
=======

		len = min(input_report->header.size,
			  (u32)sizeof(input_dev->input_buf));
		memcpy(input_dev->input_buf, input_report->buffer, len);
		hid_input_report(input_dev->hid_device, HID_INPUT_REPORT,
				 input_dev->input_buf, len, 1);

		pm_wakeup_hard_event(&input_dev->device->device);

		break;
	default:
		pr_err("unsupported hid msg type - type %d len %d\n",
		       hid_msg_hdr->type, hid_msg_hdr->size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

}

static void mousevsc_on_channel_callback(void *context)
{
<<<<<<< HEAD
	const int packet_size = 0x100;
	int ret;
	struct hv_device *device = context;
	u32 bytes_recvd;
	u64 req_id;
	struct vmpacket_descriptor *desc;
	unsigned char	*buffer;
	int	bufferlen = packet_size;

	buffer = kmalloc(bufferlen, GFP_ATOMIC);
	if (!buffer)
		return;

	do {
		ret = vmbus_recvpacket_raw(device->channel, buffer,
					bufferlen, &bytes_recvd, &req_id);

		switch (ret) {
		case 0:
			if (bytes_recvd <= 0) {
				kfree(buffer);
				return;
			}
			desc = (struct vmpacket_descriptor *)buffer;

			switch (desc->type) {
			case VM_PKT_COMP:
				break;

			case VM_PKT_DATA_INBAND:
				mousevsc_on_receive(device, desc);
				break;

			default:
				pr_err("unhandled packet type %d, tid %llx len %d\n",
					desc->type, req_id, bytes_recvd);
				break;
			}

			break;

		case -ENOBUFS:
			kfree(buffer);
			/* Handle large packet */
			bufferlen = bytes_recvd;
			buffer = kmalloc(bytes_recvd, GFP_ATOMIC);

			if (!buffer)
				return;

			break;
		}
	} while (1);

=======
	struct hv_device *device = context;
	struct vmpacket_descriptor *desc;

	foreach_vmbus_pkt(desc, device->channel) {
		switch (desc->type) {
		case VM_PKT_COMP:
			break;

		case VM_PKT_DATA_INBAND:
			mousevsc_on_receive(device, desc);
			break;

		default:
			pr_err("Unhandled packet type %d, tid %llx len %d\n",
			       desc->type, desc->trans_id, desc->len8 * 8);
			break;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mousevsc_connect_to_vsp(struct hv_device *device)
{
	int ret = 0;
<<<<<<< HEAD
	int t;
=======
	unsigned long t;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mousevsc_dev *input_dev = hv_get_drvdata(device);
	struct mousevsc_prt_msg *request;
	struct mousevsc_prt_msg *response;

<<<<<<< HEAD
=======
	reinit_completion(&input_dev->wait_event);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	request = &input_dev->protocol_req;
	memset(request, 0, sizeof(struct mousevsc_prt_msg));

	request->type = PIPE_MESSAGE_DATA;
	request->size = sizeof(struct synthhid_protocol_request);
	request->request.header.type = SYNTH_HID_PROTOCOL_REQUEST;
	request->request.header.size = sizeof(unsigned int);
	request->request.version_requested.version = SYNTHHID_INPUT_VERSION;

	ret = vmbus_sendpacket(device->channel, request,
<<<<<<< HEAD
				sizeof(struct pipe_prt_msg) -
				sizeof(unsigned char) +
=======
				sizeof(struct pipe_prt_msg) +
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				sizeof(struct synthhid_protocol_request),
				(unsigned long)request,
				VM_PKT_DATA_INBAND,
				VMBUS_DATA_PACKET_FLAG_COMPLETION_REQUESTED);
	if (ret)
		goto cleanup;

	t = wait_for_completion_timeout(&input_dev->wait_event, 5*HZ);
	if (!t) {
		ret = -ETIMEDOUT;
		goto cleanup;
	}

	response = &input_dev->protocol_resp;

	if (!response->response.approved) {
		pr_err("synthhid protocol request failed (version %d)\n",
		       SYNTHHID_INPUT_VERSION);
		ret = -ENODEV;
		goto cleanup;
	}

	t = wait_for_completion_timeout(&input_dev->wait_event, 5*HZ);
	if (!t) {
		ret = -ETIMEDOUT;
		goto cleanup;
	}

	/*
	 * We should have gotten the device attr, hid desc and report
	 * desc at this point
	 */
	ret = input_dev->dev_info_status;

cleanup:
	return ret;
}

<<<<<<< HEAD
=======
static int mousevsc_hid_parse(struct hid_device *hid)
{
	struct hv_device *dev = hid_get_drvdata(hid);
	struct mousevsc_dev *input_dev = hv_get_drvdata(dev);

	return hid_parse_report(hid, input_dev->report_desc,
				input_dev->report_desc_size);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int mousevsc_hid_open(struct hid_device *hid)
{
	return 0;
}

static int mousevsc_hid_start(struct hid_device *hid)
{
	return 0;
}

static void mousevsc_hid_close(struct hid_device *hid)
{
}

static void mousevsc_hid_stop(struct hid_device *hid)
{
}

<<<<<<< HEAD
static struct hid_ll_driver mousevsc_ll_driver = {
=======
static int mousevsc_hid_raw_request(struct hid_device *hid,
				    unsigned char report_num,
				    __u8 *buf, size_t len,
				    unsigned char rtype,
				    int reqtype)
{
	return 0;
}

static const struct hid_ll_driver mousevsc_ll_driver = {
	.parse = mousevsc_hid_parse,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open = mousevsc_hid_open,
	.close = mousevsc_hid_close,
	.start = mousevsc_hid_start,
	.stop = mousevsc_hid_stop,
<<<<<<< HEAD
=======
	.raw_request = mousevsc_hid_raw_request,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct hid_driver mousevsc_hid_driver;

static int mousevsc_probe(struct hv_device *device,
			const struct hv_vmbus_device_id *dev_id)
{
	int ret;
	struct mousevsc_dev *input_dev;
	struct hid_device *hid_dev;

	input_dev = mousevsc_alloc_device(device);

	if (!input_dev)
		return -ENOMEM;

	ret = vmbus_open(device->channel,
		INPUTVSC_SEND_RING_BUFFER_SIZE,
		INPUTVSC_RECV_RING_BUFFER_SIZE,
		NULL,
		0,
		mousevsc_on_channel_callback,
		device
		);

	if (ret)
		goto probe_err0;

	ret = mousevsc_connect_to_vsp(device);

	if (ret)
		goto probe_err1;

	/* workaround SA-167 */
	if (input_dev->report_desc[14] == 0x25)
		input_dev->report_desc[14] = 0x29;

	hid_dev = hid_allocate_device();
	if (IS_ERR(hid_dev)) {
		ret = PTR_ERR(hid_dev);
		goto probe_err1;
	}

	hid_dev->ll_driver = &mousevsc_ll_driver;
	hid_dev->driver = &mousevsc_hid_driver;
	hid_dev->bus = BUS_VIRTUAL;
	hid_dev->vendor = input_dev->hid_dev_info.vendor;
	hid_dev->product = input_dev->hid_dev_info.product;
	hid_dev->version = input_dev->hid_dev_info.version;
	input_dev->hid_device = hid_dev;

	sprintf(hid_dev->name, "%s", "Microsoft Vmbus HID-compliant Mouse");

<<<<<<< HEAD
	ret = hid_add_device(hid_dev);
	if (ret)
		goto probe_err1;

	ret = hid_parse_report(hid_dev, input_dev->report_desc,
				input_dev->report_desc_size);

=======
	hid_set_drvdata(hid_dev, device);

	ret = hid_add_device(hid_dev);
	if (ret)
		goto probe_err2;


	ret = hid_parse(hid_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret) {
		hid_err(hid_dev, "parse failed\n");
		goto probe_err2;
	}

	ret = hid_hw_start(hid_dev, HID_CONNECT_HIDINPUT | HID_CONNECT_HIDDEV);

	if (ret) {
		hid_err(hid_dev, "hw start failed\n");
		goto probe_err2;
	}

<<<<<<< HEAD
=======
	device_init_wakeup(&device->device, true);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	input_dev->connected = true;
	input_dev->init_complete = true;

	return ret;

probe_err2:
	hid_destroy_device(hid_dev);

probe_err1:
	vmbus_close(device->channel);

probe_err0:
	mousevsc_free_device(input_dev);

	return ret;
}


<<<<<<< HEAD
static int mousevsc_remove(struct hv_device *dev)
{
	struct mousevsc_dev *input_dev = hv_get_drvdata(dev);

=======
static void mousevsc_remove(struct hv_device *dev)
{
	struct mousevsc_dev *input_dev = hv_get_drvdata(dev);

	device_init_wakeup(&dev->device, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vmbus_close(dev->channel);
	hid_hw_stop(input_dev->hid_device);
	hid_destroy_device(input_dev->hid_device);
	mousevsc_free_device(input_dev);
<<<<<<< HEAD
=======
}

static int mousevsc_suspend(struct hv_device *dev)
{
	vmbus_close(dev->channel);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct hv_vmbus_device_id id_table[] = {
	/* Mouse guid */
	{ VMBUS_DEVICE(0x9E, 0xB6, 0xA8, 0xCF, 0x4A, 0x5B, 0xc0, 0x4c,
		       0xB9, 0x8B, 0x8B, 0xA1, 0xA1, 0xF3, 0xF9, 0x5A) },
=======
static int mousevsc_resume(struct hv_device *dev)
{
	int ret;

	ret = vmbus_open(dev->channel,
			 INPUTVSC_SEND_RING_BUFFER_SIZE,
			 INPUTVSC_RECV_RING_BUFFER_SIZE,
			 NULL, 0,
			 mousevsc_on_channel_callback,
			 dev);
	if (ret)
		return ret;

	ret = mousevsc_connect_to_vsp(dev);
	return ret;
}

static const struct hv_vmbus_device_id id_table[] = {
	/* Mouse guid */
	{ HV_MOUSE_GUID, },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ },
};

MODULE_DEVICE_TABLE(vmbus, id_table);

static struct  hv_driver mousevsc_drv = {
	.name = KBUILD_MODNAME,
	.id_table = id_table,
	.probe = mousevsc_probe,
	.remove = mousevsc_remove,
<<<<<<< HEAD
=======
	.suspend = mousevsc_suspend,
	.resume = mousevsc_resume,
	.driver = {
		.probe_type = PROBE_PREFER_ASYNCHRONOUS,
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init mousevsc_init(void)
{
	return vmbus_driver_register(&mousevsc_drv);
}

static void __exit mousevsc_exit(void)
{
	vmbus_driver_unregister(&mousevsc_drv);
}

MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_VERSION(HV_DRV_VERSION);
=======
MODULE_DESCRIPTION("Microsoft Hyper-V Synthetic HID Driver");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_init(mousevsc_init);
module_exit(mousevsc_exit);
