<<<<<<< HEAD
/**
 * gadget.h - DesignWare USB3 DRD Gadget Header
 *
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - http://www.ti.com
 *
 * Authors: Felipe Balbi <balbi@ti.com>,
 *	    Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2, as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * gadget.h - DesignWare USB3 DRD Gadget Header
 *
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - https://www.ti.com
 *
 * Authors: Felipe Balbi <balbi@ti.com>,
 *	    Sebastian Andrzej Siewior <bigeasy@linutronix.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __DRIVERS_USB_DWC3_GADGET_H
#define __DRIVERS_USB_DWC3_GADGET_H

#include <linux/list.h>
#include <linux/usb/gadget.h>
#include "io.h"

struct dwc3;
#define to_dwc3_ep(ep)		(container_of(ep, struct dwc3_ep, endpoint))
<<<<<<< HEAD
#define gadget_to_dwc(g)	(container_of(g, struct dwc3, gadget))

/* DEPCFG parameter 1 */
#define DWC3_DEPCFG_INT_NUM(n)		((n) << 0)
#define DWC3_DEPCFG_XFER_COMPLETE_EN	(1 << 8)
#define DWC3_DEPCFG_XFER_IN_PROGRESS_EN	(1 << 9)
#define DWC3_DEPCFG_XFER_NOT_READY_EN	(1 << 10)
#define DWC3_DEPCFG_FIFO_ERROR_EN	(1 << 11)
#define DWC3_DEPCFG_STREAM_EVENT_EN	(1 << 13)
#define DWC3_DEPCFG_BINTERVAL_M1(n)	((n) << 16)
#define DWC3_DEPCFG_STREAM_CAPABLE	(1 << 24)
#define DWC3_DEPCFG_EP_NUMBER(n)	((n) << 25)
#define DWC3_DEPCFG_BULK_BASED		(1 << 30)
#define DWC3_DEPCFG_FIFO_BASED		(1 << 31)

/* DEPCFG parameter 0 */
#define DWC3_DEPCFG_EP_TYPE(n)		((n) << 1)
#define DWC3_DEPCFG_MAX_PACKET_SIZE(n)	((n) << 3)
#define DWC3_DEPCFG_FIFO_NUMBER(n)	((n) << 17)
#define DWC3_DEPCFG_BURST_SIZE(n)	((n) << 22)
#define DWC3_DEPCFG_DATA_SEQ_NUM(n)	((n) << 26)
/* This applies for core versions earlier than 1.94a */
#define DWC3_DEPCFG_IGN_SEQ_NUM		(1 << 31)
/* These apply for core versions 1.94a and later */
#define DWC3_DEPCFG_ACTION_INIT		(0 << 30)
#define DWC3_DEPCFG_ACTION_RESTORE	(1 << 30)
=======
#define gadget_to_dwc(g)	(dev_get_platdata(&g->dev))

/* DEPCFG parameter 1 */
#define DWC3_DEPCFG_INT_NUM(n)		(((n) & 0x1f) << 0)
#define DWC3_DEPCFG_XFER_COMPLETE_EN	BIT(8)
#define DWC3_DEPCFG_XFER_IN_PROGRESS_EN	BIT(9)
#define DWC3_DEPCFG_XFER_NOT_READY_EN	BIT(10)
#define DWC3_DEPCFG_FIFO_ERROR_EN	BIT(11)
#define DWC3_DEPCFG_STREAM_EVENT_EN	BIT(13)
#define DWC3_DEPCFG_BINTERVAL_M1(n)	(((n) & 0xff) << 16)
#define DWC3_DEPCFG_STREAM_CAPABLE	BIT(24)
#define DWC3_DEPCFG_EP_NUMBER(n)	(((n) & 0x1f) << 25)
#define DWC3_DEPCFG_BULK_BASED		BIT(30)
#define DWC3_DEPCFG_FIFO_BASED		BIT(31)

/* DEPCFG parameter 0 */
#define DWC3_DEPCFG_EP_TYPE(n)		(((n) & 0x3) << 1)
#define DWC3_DEPCFG_MAX_PACKET_SIZE(n)	(((n) & 0x7ff) << 3)
#define DWC3_DEPCFG_FIFO_NUMBER(n)	(((n) & 0x1f) << 17)
#define DWC3_DEPCFG_BURST_SIZE(n)	(((n) & 0xf) << 22)
#define DWC3_DEPCFG_DATA_SEQ_NUM(n)	((n) << 26)
/* This applies for core versions earlier than 1.94a */
#define DWC3_DEPCFG_IGN_SEQ_NUM		BIT(31)
/* These apply for core versions 1.94a and later */
#define DWC3_DEPCFG_ACTION_INIT		(0 << 30)
#define DWC3_DEPCFG_ACTION_RESTORE	BIT(30)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DWC3_DEPCFG_ACTION_MODIFY	(2 << 30)

/* DEPXFERCFG parameter 0 */
#define DWC3_DEPXFERCFG_NUM_XFER_RES(n)	((n) & 0xffff)

<<<<<<< HEAD
struct dwc3_gadget_ep_cmd_params {
	u32	param2;
	u32	param1;
	u32	param0;
};

=======
/* U1 Device exit Latency */
#define DWC3_DEFAULT_U1_DEV_EXIT_LAT	0x0A	/* Less then 10 microsec */

/* U2 Device exit Latency */
#define DWC3_DEFAULT_U2_DEV_EXIT_LAT	0x1FF	/* Less then 511 microsec */

/* Frame/Microframe Number Mask */
#define DWC3_FRNUMBER_MASK		0x3fff
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* -------------------------------------------------------------------------- */

#define to_dwc3_request(r)	(container_of(r, struct dwc3_request, request))

<<<<<<< HEAD
static inline struct dwc3_request *next_request(struct list_head *list)
{
	if (list_empty(list))
		return NULL;

	return list_first_entry(list, struct dwc3_request, list);
}

static inline void dwc3_gadget_move_request_list_front(struct dwc3_request *req)
{
	struct dwc3_ep		*dep = req->dep;

	req->queued = false;
	list_move(&req->list, &dep->request_list);
}

static inline void dwc3_gadget_move_request_queued(struct dwc3_request *req)
{
	struct dwc3_ep		*dep = req->dep;

	req->queued = true;
	list_move_tail(&req->list, &dep->req_queued);
=======
/**
 * next_request - gets the next request on the given list
 * @list: the request list to operate on
 *
 * Caller should take care of locking. This function return %NULL or the first
 * request available on @list.
 */
static inline struct dwc3_request *next_request(struct list_head *list)
{
	return list_first_entry_or_null(list, struct dwc3_request, list);
}

/**
 * dwc3_gadget_move_started_request - move @req to the started_list
 * @req: the request to be moved
 *
 * Caller should take care of locking. This function will move @req from its
 * current list to the endpoint's started_list.
 */
static inline void dwc3_gadget_move_started_request(struct dwc3_request *req)
{
	struct dwc3_ep		*dep = req->dep;

	req->status = DWC3_REQUEST_STATUS_STARTED;
	list_move_tail(&req->list, &dep->started_list);
}

/**
 * dwc3_gadget_move_cancelled_request - move @req to the cancelled_list
 * @req: the request to be moved
 * @reason: cancelled reason for the dwc3 request
 *
 * Caller should take care of locking. This function will move @req from its
 * current list to the endpoint's cancelled_list.
 */
static inline void dwc3_gadget_move_cancelled_request(struct dwc3_request *req,
		unsigned int reason)
{
	struct dwc3_ep		*dep = req->dep;

	req->status = reason;
	list_move_tail(&req->list, &dep->cancelled_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void dwc3_gadget_giveback(struct dwc3_ep *dep, struct dwc3_request *req,
		int status);

<<<<<<< HEAD
int dwc3_gadget_set_test_mode(struct dwc3 *dwc, int mode);
int dwc3_gadget_set_link_state(struct dwc3 *dwc, enum dwc3_link_state state);

void dwc3_ep0_interrupt(struct dwc3 *dwc,
		const struct dwc3_event_depevt *event);
void dwc3_ep0_out_start(struct dwc3 *dwc);
=======
void dwc3_ep0_interrupt(struct dwc3 *dwc,
		const struct dwc3_event_depevt *event);
void dwc3_ep0_out_start(struct dwc3 *dwc);
void dwc3_ep0_end_control_data(struct dwc3 *dwc, struct dwc3_ep *dep);
void dwc3_ep0_stall_and_restart(struct dwc3 *dwc);
int __dwc3_gadget_ep0_set_halt(struct usb_ep *ep, int value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int dwc3_gadget_ep0_set_halt(struct usb_ep *ep, int value);
int dwc3_gadget_ep0_queue(struct usb_ep *ep, struct usb_request *request,
		gfp_t gfp_flags);
int __dwc3_gadget_ep_set_halt(struct dwc3_ep *dep, int value, int protocol);
<<<<<<< HEAD
int dwc3_send_gadget_ep_cmd(struct dwc3 *dwc, unsigned ep,
		unsigned cmd, struct dwc3_gadget_ep_cmd_params *params);
int dwc3_send_gadget_generic_command(struct dwc3 *dwc, int cmd, u32 param);
dma_addr_t dwc3_trb_dma_offset(struct dwc3_ep *dep,
		struct dwc3_trb *trb);

/**
 * dwc3_gadget_ep_get_transfer_index - Gets transfer index from HW
 * @dwc: DesignWare USB3 Pointer
 * @number: DWC endpoint number
 *
 * Caller should take care of locking
 */
static inline u32 dwc3_gadget_ep_get_transfer_index(struct dwc3 *dwc, u8 number)
{
	u32			res_id;

	res_id = dwc3_readl(dwc->regs, DWC3_DEPCMD(number));

	return DWC3_DEPCMD_GET_RSC_IDX(res_id);
}

/**
 * dwc3_gadget_event_string - returns event name
 * @event: the event code
 */
static inline const char *dwc3_gadget_event_string(u8 event)
{
	switch (event) {
	case DWC3_DEVICE_EVENT_DISCONNECT:
		return "Disconnect";
	case DWC3_DEVICE_EVENT_RESET:
		return "Reset";
	case DWC3_DEVICE_EVENT_CONNECT_DONE:
		return "Connection Done";
	case DWC3_DEVICE_EVENT_LINK_STATUS_CHANGE:
		return "Link Status Change";
	case DWC3_DEVICE_EVENT_WAKEUP:
		return "WakeUp";
	case DWC3_DEVICE_EVENT_EOPF:
		return "End-Of-Frame";
	case DWC3_DEVICE_EVENT_SOF:
		return "Start-Of-Frame";
	case DWC3_DEVICE_EVENT_ERRATIC_ERROR:
		return "Erratic Error";
	case DWC3_DEVICE_EVENT_CMD_CMPL:
		return "Command Complete";
	case DWC3_DEVICE_EVENT_OVERFLOW:
		return "Overflow";
	}

	return "UNKNOWN";
}

/**
 * dwc3_ep_event_string - returns event name
 * @event: then event code
 */
static inline const char *dwc3_ep_event_string(u8 event)
{
	switch (event) {
	case DWC3_DEPEVT_XFERCOMPLETE:
		return "Transfer Complete";
	case DWC3_DEPEVT_XFERINPROGRESS:
		return "Transfer In-Progress";
	case DWC3_DEPEVT_XFERNOTREADY:
		return "Transfer Not Ready";
	case DWC3_DEPEVT_RXTXFIFOEVT:
		return "FIFO";
	case DWC3_DEPEVT_STREAMEVT:
		return "Stream";
	case DWC3_DEPEVT_EPCMDCMPLT:
		return "Endpoint Command Complete";
	}

	return "UNKNOWN";
=======
void dwc3_ep0_send_delayed_status(struct dwc3 *dwc);
void dwc3_stop_active_transfer(struct dwc3_ep *dep, bool force, bool interrupt);
int dwc3_gadget_start_config(struct dwc3 *dwc, unsigned int resource_index);

/**
 * dwc3_gadget_ep_get_transfer_index - Gets transfer index from HW
 * @dep: dwc3 endpoint
 *
 * Caller should take care of locking. Returns the transfer resource
 * index for a given endpoint.
 */
static inline void dwc3_gadget_ep_get_transfer_index(struct dwc3_ep *dep)
{
	u32			res_id;

	res_id = dwc3_readl(dep->regs, DWC3_DEPCMD);
	dep->resource_index = DWC3_DEPCMD_GET_RSC_IDX(res_id);
}

/**
 * dwc3_gadget_dctl_write_safe - write to DCTL safe from link state change
 * @dwc: pointer to our context structure
 * @value: value to write to DCTL
 *
 * Use this function when doing read-modify-write to DCTL. It will not
 * send link state change request.
 */
static inline void dwc3_gadget_dctl_write_safe(struct dwc3 *dwc, u32 value)
{
	value &= ~DWC3_DCTL_ULSTCHNGREQ_MASK;
	dwc3_writel(dwc->regs, DWC3_DCTL, value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif /* __DRIVERS_USB_DWC3_GADGET_H */
