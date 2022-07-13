<<<<<<< HEAD
/****************************************************************************
 * Driver for Solarflare Solarstorm network controllers and boards
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2005-2011 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
=======
// SPDX-License-Identifier: GPL-2.0-only
/****************************************************************************
 * Driver for Solarflare network controllers and boards
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2005-2013 Solarflare Communications Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/socket.h>
#include <linux/in.h>
#include <linux/slab.h>
#include <linux/ip.h>
<<<<<<< HEAD
=======
#include <linux/ipv6.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/prefetch.h>
#include <linux/moduleparam.h>
<<<<<<< HEAD
#include <net/ip.h>
#include <net/checksum.h>
#include "net_driver.h"
#include "efx.h"
=======
#include <linux/iommu.h>
#include <net/ip.h>
#include <net/checksum.h>
#include <net/xdp.h>
#include <linux/bpf_trace.h>
#include "net_driver.h"
#include "efx.h"
#include "rx_common.h"
#include "filter.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "nic.h"
#include "selftest.h"
#include "workarounds.h"

<<<<<<< HEAD
/* Number of RX descriptors pushed at once. */
#define EFX_RX_BATCH  8

/* Maximum size of a buffer sharing a page */
#define EFX_RX_HALF_PAGE ((PAGE_SIZE >> 1) - sizeof(struct efx_rx_page_state))

/* Size of buffer allocated for skb header area. */
#define EFX_SKB_HEADERS  64u

/*
 * rx_alloc_method - RX buffer allocation method
 *
 * This driver supports two methods for allocating and using RX buffers:
 * each RX buffer may be backed by an skb or by an order-n page.
 *
 * When GRO is in use then the second method has a lower overhead,
 * since we don't have to allocate then free skbs on reassembled frames.
 *
 * Values:
 *   - RX_ALLOC_METHOD_AUTO = 0
 *   - RX_ALLOC_METHOD_SKB  = 1
 *   - RX_ALLOC_METHOD_PAGE = 2
 *
 * The heuristic for %RX_ALLOC_METHOD_AUTO is a simple hysteresis count
 * controlled by the parameters below.
 *
 *   - Since pushing and popping descriptors are separated by the rx_queue
 *     size, so the watermarks should be ~rxd_size.
 *   - The performance win by using page-based allocation for GRO is less
 *     than the performance hit of using page-based allocation of non-GRO,
 *     so the watermarks should reflect this.
 *
 * Per channel we maintain a single variable, updated by each channel:
 *
 *   rx_alloc_level += (gro_performed ? RX_ALLOC_FACTOR_GRO :
 *                      RX_ALLOC_FACTOR_SKB)
 * Per NAPI poll interval, we constrain rx_alloc_level to 0..MAX (which
 * limits the hysteresis), and update the allocation strategy:
 *
 *   rx_alloc_method = (rx_alloc_level > RX_ALLOC_LEVEL_GRO ?
 *                      RX_ALLOC_METHOD_PAGE : RX_ALLOC_METHOD_SKB)
 */
static int rx_alloc_method = RX_ALLOC_METHOD_AUTO;

#define RX_ALLOC_LEVEL_GRO 0x2000
#define RX_ALLOC_LEVEL_MAX 0x3000
#define RX_ALLOC_FACTOR_GRO 1
#define RX_ALLOC_FACTOR_SKB (-2)

/* This is the percentage fill level below which new RX descriptors
 * will be added to the RX descriptor ring.
 */
static unsigned int rx_refill_threshold = 90;

/* This is the percentage fill level to which an RX queue will be refilled
 * when the "RX refill threshold" is reached.
 */
static unsigned int rx_refill_limit = 95;

/*
 * RX maximum head room required.
 *
 * This must be at least 1 to prevent overflow and at least 2 to allow
 * pipelined receives.
 */
#define EFX_RXD_HEAD_ROOM 2

/* Offset of ethernet header within page */
static inline unsigned int efx_rx_buf_offset(struct efx_nic *efx,
					     struct efx_rx_buffer *buf)
{
	return buf->page_offset + efx->type->rx_buffer_hash_size;
}
static inline unsigned int efx_rx_buf_size(struct efx_nic *efx)
{
	return PAGE_SIZE << efx->rx_buffer_order;
}

static u8 *efx_rx_buf_eh(struct efx_nic *efx, struct efx_rx_buffer *buf)
{
	if (buf->flags & EFX_RX_BUF_PAGE)
		return page_address(buf->u.page) + efx_rx_buf_offset(efx, buf);
	else
		return (u8 *)buf->u.skb->data + efx->type->rx_buffer_hash_size;
}

static inline u32 efx_rx_buf_hash(const u8 *eh)
{
	/* The ethernet header is always directly after any hash. */
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) || NET_IP_ALIGN % 4 == 0
	return __le32_to_cpup((const __le32 *)(eh - 4));
#else
	const u8 *data = eh - 4;
	return (u32)data[0]	  |
	       (u32)data[1] << 8  |
	       (u32)data[2] << 16 |
	       (u32)data[3] << 24;
#endif
}

/**
 * efx_init_rx_buffers_skb - create EFX_RX_BATCH skb-based RX buffers
 *
 * @rx_queue:		Efx RX queue
 *
 * This allocates EFX_RX_BATCH skbs, maps them for DMA, and populates a
 * struct efx_rx_buffer for each one. Return a negative error code or 0
 * on success. May fail having only inserted fewer than EFX_RX_BATCH
 * buffers.
 */
static int efx_init_rx_buffers_skb(struct efx_rx_queue *rx_queue)
{
	struct efx_nic *efx = rx_queue->efx;
	struct net_device *net_dev = efx->net_dev;
	struct efx_rx_buffer *rx_buf;
	struct sk_buff *skb;
	int skb_len = efx->rx_buffer_len;
	unsigned index, count;

	for (count = 0; count < EFX_RX_BATCH; ++count) {
		index = rx_queue->added_count & rx_queue->ptr_mask;
		rx_buf = efx_rx_buffer(rx_queue, index);

		rx_buf->u.skb = skb = netdev_alloc_skb(net_dev, skb_len);
		if (unlikely(!skb))
			return -ENOMEM;

		/* Adjust the SKB for padding */
		skb_reserve(skb, NET_IP_ALIGN);
		rx_buf->len = skb_len - NET_IP_ALIGN;
		rx_buf->flags = 0;

		rx_buf->dma_addr = pci_map_single(efx->pci_dev,
						  skb->data, rx_buf->len,
						  PCI_DMA_FROMDEVICE);
		if (unlikely(pci_dma_mapping_error(efx->pci_dev,
						   rx_buf->dma_addr))) {
			dev_kfree_skb_any(skb);
			rx_buf->u.skb = NULL;
			return -EIO;
		}

		++rx_queue->added_count;
		++rx_queue->alloc_skb_count;
	}

	return 0;
}

/**
 * efx_init_rx_buffers_page - create EFX_RX_BATCH page-based RX buffers
 *
 * @rx_queue:		Efx RX queue
 *
 * This allocates memory for EFX_RX_BATCH receive buffers, maps them for DMA,
 * and populates struct efx_rx_buffers for each one. Return a negative error
 * code or 0 on success. If a single page can be split between two buffers,
 * then the page will either be inserted fully, or not at at all.
 */
static int efx_init_rx_buffers_page(struct efx_rx_queue *rx_queue)
{
	struct efx_nic *efx = rx_queue->efx;
	struct efx_rx_buffer *rx_buf;
	struct page *page;
	void *page_addr;
	unsigned int page_offset;
	struct efx_rx_page_state *state;
	dma_addr_t dma_addr;
	unsigned index, count;

	/* We can split a page between two buffers */
	BUILD_BUG_ON(EFX_RX_BATCH & 1);

	for (count = 0; count < EFX_RX_BATCH; ++count) {
		page = alloc_pages(__GFP_COLD | __GFP_COMP | GFP_ATOMIC,
				   efx->rx_buffer_order);
		if (unlikely(page == NULL))
			return -ENOMEM;
		dma_addr = pci_map_page(efx->pci_dev, page, 0,
					efx_rx_buf_size(efx),
					PCI_DMA_FROMDEVICE);
		if (unlikely(pci_dma_mapping_error(efx->pci_dev, dma_addr))) {
			__free_pages(page, efx->rx_buffer_order);
			return -EIO;
		}
		page_addr = page_address(page);
		state = page_addr;
		state->refcnt = 0;
		state->dma_addr = dma_addr;

		page_addr += sizeof(struct efx_rx_page_state);
		dma_addr += sizeof(struct efx_rx_page_state);
		page_offset = sizeof(struct efx_rx_page_state);

	split:
		index = rx_queue->added_count & rx_queue->ptr_mask;
		rx_buf = efx_rx_buffer(rx_queue, index);
		rx_buf->dma_addr = dma_addr + EFX_PAGE_IP_ALIGN;
		rx_buf->u.page = page;
		rx_buf->page_offset = page_offset + EFX_PAGE_IP_ALIGN;
		rx_buf->len = efx->rx_buffer_len - EFX_PAGE_IP_ALIGN;
		rx_buf->flags = EFX_RX_BUF_PAGE;
		++rx_queue->added_count;
		++rx_queue->alloc_page_count;
		++state->refcnt;

		if ((~count & 1) && (efx->rx_buffer_len <= EFX_RX_HALF_PAGE)) {
			/* Use the second half of the page */
			get_page(page);
			dma_addr += (PAGE_SIZE >> 1);
			page_addr += (PAGE_SIZE >> 1);
			page_offset += (PAGE_SIZE >> 1);
			++count;
			goto split;
		}
	}

	return 0;
}

static void efx_unmap_rx_buffer(struct efx_nic *efx,
				struct efx_rx_buffer *rx_buf,
				unsigned int used_len)
{
	if ((rx_buf->flags & EFX_RX_BUF_PAGE) && rx_buf->u.page) {
		struct efx_rx_page_state *state;

		state = page_address(rx_buf->u.page);
		if (--state->refcnt == 0) {
			pci_unmap_page(efx->pci_dev,
				       state->dma_addr,
				       efx_rx_buf_size(efx),
				       PCI_DMA_FROMDEVICE);
		} else if (used_len) {
			dma_sync_single_for_cpu(&efx->pci_dev->dev,
						rx_buf->dma_addr, used_len,
						DMA_FROM_DEVICE);
		}
	} else if (!(rx_buf->flags & EFX_RX_BUF_PAGE) && rx_buf->u.skb) {
		pci_unmap_single(efx->pci_dev, rx_buf->dma_addr,
				 rx_buf->len, PCI_DMA_FROMDEVICE);
	}
}

static void efx_free_rx_buffer(struct efx_nic *efx,
			       struct efx_rx_buffer *rx_buf)
{
	if ((rx_buf->flags & EFX_RX_BUF_PAGE) && rx_buf->u.page) {
		__free_pages(rx_buf->u.page, efx->rx_buffer_order);
		rx_buf->u.page = NULL;
	} else if (!(rx_buf->flags & EFX_RX_BUF_PAGE) && rx_buf->u.skb) {
		dev_kfree_skb_any(rx_buf->u.skb);
		rx_buf->u.skb = NULL;
	}
}

static void efx_fini_rx_buffer(struct efx_rx_queue *rx_queue,
			       struct efx_rx_buffer *rx_buf)
{
	efx_unmap_rx_buffer(rx_queue->efx, rx_buf, 0);
	efx_free_rx_buffer(rx_queue->efx, rx_buf);
}

/* Attempt to resurrect the other receive buffer that used to share this page,
 * which had previously been passed up to the kernel and freed. */
static void efx_resurrect_rx_buffer(struct efx_rx_queue *rx_queue,
				    struct efx_rx_buffer *rx_buf)
{
	struct efx_rx_page_state *state = page_address(rx_buf->u.page);
	struct efx_rx_buffer *new_buf;
	unsigned fill_level, index;

	/* +1 because efx_rx_packet() incremented removed_count. +1 because
	 * we'd like to insert an additional descriptor whilst leaving
	 * EFX_RXD_HEAD_ROOM for the non-recycle path */
	fill_level = (rx_queue->added_count - rx_queue->removed_count + 2);
	if (unlikely(fill_level > rx_queue->max_fill)) {
		/* We could place "state" on a list, and drain the list in
		 * efx_fast_push_rx_descriptors(). For now, this will do. */
		return;
	}

	++state->refcnt;
	get_page(rx_buf->u.page);

	index = rx_queue->added_count & rx_queue->ptr_mask;
	new_buf = efx_rx_buffer(rx_queue, index);
	new_buf->u.page = rx_buf->u.page;
	new_buf->page_offset = rx_buf->page_offset ^ (PAGE_SIZE >> 1);
	new_buf->dma_addr = state->dma_addr + new_buf->page_offset;
	new_buf->len = rx_buf->len;
	new_buf->flags = EFX_RX_BUF_PAGE;
	++rx_queue->added_count;
}

/* Recycle the given rx buffer directly back into the rx_queue. There is
 * always room to add this buffer, because we've just popped a buffer. */
static void efx_recycle_rx_buffer(struct efx_channel *channel,
				  struct efx_rx_buffer *rx_buf)
{
	struct efx_nic *efx = channel->efx;
	struct efx_rx_queue *rx_queue = efx_channel_get_rx_queue(channel);
	struct efx_rx_buffer *new_buf;
	unsigned index;

	rx_buf->flags &= EFX_RX_BUF_PAGE;

	if ((rx_buf->flags & EFX_RX_BUF_PAGE) &&
	    efx->rx_buffer_len <= EFX_RX_HALF_PAGE &&
	    page_count(rx_buf->u.page) == 1)
		efx_resurrect_rx_buffer(rx_queue, rx_buf);

	index = rx_queue->added_count & rx_queue->ptr_mask;
	new_buf = efx_rx_buffer(rx_queue, index);

	memcpy(new_buf, rx_buf, sizeof(*new_buf));
	rx_buf->u.page = NULL;
	++rx_queue->added_count;
}

/**
 * efx_fast_push_rx_descriptors - push new RX descriptors quickly
 * @rx_queue:		RX descriptor queue
 * This will aim to fill the RX descriptor queue up to
 * @rx_queue->@fast_fill_limit. If there is insufficient atomic
 * memory to do so, a slow fill will be scheduled.
 *
 * The caller must provide serialisation (none is used here). In practise,
 * this means this function must run from the NAPI handler, or be called
 * when NAPI is disabled.
 */
void efx_fast_push_rx_descriptors(struct efx_rx_queue *rx_queue)
{
	struct efx_channel *channel = efx_rx_queue_channel(rx_queue);
	unsigned fill_level;
	int space, rc = 0;

	/* Calculate current fill level, and exit if we don't need to fill */
	fill_level = (rx_queue->added_count - rx_queue->removed_count);
	EFX_BUG_ON_PARANOID(fill_level > rx_queue->efx->rxq_entries);
	if (fill_level >= rx_queue->fast_fill_trigger)
		goto out;

	/* Record minimum fill level */
	if (unlikely(fill_level < rx_queue->min_fill)) {
		if (fill_level)
			rx_queue->min_fill = fill_level;
	}

	space = rx_queue->fast_fill_limit - fill_level;
	if (space < EFX_RX_BATCH)
		goto out;

	netif_vdbg(rx_queue->efx, rx_status, rx_queue->efx->net_dev,
		   "RX queue %d fast-filling descriptor ring from"
		   " level %d to level %d using %s allocation\n",
		   efx_rx_queue_index(rx_queue), fill_level,
		   rx_queue->fast_fill_limit,
		   channel->rx_alloc_push_pages ? "page" : "skb");

	do {
		if (channel->rx_alloc_push_pages)
			rc = efx_init_rx_buffers_page(rx_queue);
		else
			rc = efx_init_rx_buffers_skb(rx_queue);
		if (unlikely(rc)) {
			/* Ensure that we don't leave the rx queue empty */
			if (rx_queue->added_count == rx_queue->removed_count)
				efx_schedule_slow_fill(rx_queue);
			goto out;
		}
	} while ((space -= EFX_RX_BATCH) >= EFX_RX_BATCH);

	netif_vdbg(rx_queue->efx, rx_status, rx_queue->efx->net_dev,
		   "RX queue %d fast-filled descriptor ring "
		   "to level %d\n", efx_rx_queue_index(rx_queue),
		   rx_queue->added_count - rx_queue->removed_count);

 out:
	if (rx_queue->notified_count != rx_queue->added_count)
		efx_nic_notify_rx_desc(rx_queue);
}

void efx_rx_slow_fill(unsigned long context)
{
	struct efx_rx_queue *rx_queue = (struct efx_rx_queue *)context;

	/* Post an event to cause NAPI to run and refill the queue */
	efx_nic_generate_fill_event(rx_queue);
	++rx_queue->slow_fill_count;
}

static void efx_rx_packet__check_len(struct efx_rx_queue *rx_queue,
				     struct efx_rx_buffer *rx_buf,
				     int len, bool *leak_packet)
=======
/* Preferred number of descriptors to fill at once */
#define EFX_RX_PREFERRED_BATCH 8U

/* Maximum rx prefix used by any architecture. */
#define EFX_MAX_RX_PREFIX_SIZE 16

/* Size of buffer allocated for skb header area. */
#define EFX_SKB_HEADERS  128u

/* Each packet can consume up to ceil(max_frame_len / buffer_size) buffers */
#define EFX_RX_MAX_FRAGS DIV_ROUND_UP(EFX_MAX_FRAME_LEN(EFX_MAX_MTU), \
				      EFX_RX_USR_BUF_SIZE)

static void efx_rx_packet__check_len(struct efx_rx_queue *rx_queue,
				     struct efx_rx_buffer *rx_buf,
				     int len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct efx_nic *efx = rx_queue->efx;
	unsigned max_len = rx_buf->len - efx->type->rx_buffer_padding;

	if (likely(len <= max_len))
		return;

	/* The packet must be discarded, but this is only a fatal error
	 * if the caller indicated it was
	 */
	rx_buf->flags |= EFX_RX_PKT_DISCARD;

<<<<<<< HEAD
	if ((len > rx_buf->len) && EFX_WORKAROUND_8071(efx)) {
		if (net_ratelimit())
			netif_err(efx, rx_err, efx->net_dev,
				  " RX queue %d seriously overlength "
				  "RX event (0x%x > 0x%x+0x%x). Leaking\n",
				  efx_rx_queue_index(rx_queue), len, max_len,
				  efx->type->rx_buffer_padding);
		/* If this buffer was skb-allocated, then the meta
		 * data at the end of the skb will be trashed. So
		 * we have no choice but to leak the fragment.
		 */
		*leak_packet = !(rx_buf->flags & EFX_RX_BUF_PAGE);
		efx_schedule_reset(efx, RESET_TYPE_RX_RECOVERY);
	} else {
		if (net_ratelimit())
			netif_err(efx, rx_err, efx->net_dev,
				  " RX queue %d overlength RX event "
				  "(0x%x > 0x%x)\n",
				  efx_rx_queue_index(rx_queue), len, max_len);
	}
=======
	if (net_ratelimit())
		netif_err(efx, rx_err, efx->net_dev,
			  "RX queue %d overlength RX event (%#x > %#x)\n",
			  efx_rx_queue_index(rx_queue), len, max_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	efx_rx_queue_channel(rx_queue)->n_rx_overlength++;
}

<<<<<<< HEAD
/* Pass a received packet up through GRO.  GRO can handle pages
 * regardless of checksum state and skbs with a good checksum.
 */
static void efx_rx_packet_gro(struct efx_channel *channel,
			      struct efx_rx_buffer *rx_buf,
			      const u8 *eh)
{
	struct napi_struct *napi = &channel->napi_str;
	gro_result_t gro_result;

	if (rx_buf->flags & EFX_RX_BUF_PAGE) {
		struct efx_nic *efx = channel->efx;
		struct page *page = rx_buf->u.page;
		struct sk_buff *skb;

		rx_buf->u.page = NULL;

		skb = napi_get_frags(napi);
		if (!skb) {
			put_page(page);
			return;
		}

		if (efx->net_dev->features & NETIF_F_RXHASH)
			skb->rxhash = efx_rx_buf_hash(eh);

		skb_fill_page_desc(skb, 0, page,
				   efx_rx_buf_offset(efx, rx_buf), rx_buf->len);

		skb->len = rx_buf->len;
		skb->data_len = rx_buf->len;
		skb->truesize += rx_buf->len;
		skb->ip_summed = ((rx_buf->flags & EFX_RX_PKT_CSUMMED) ?
				  CHECKSUM_UNNECESSARY : CHECKSUM_NONE);

		skb_record_rx_queue(skb, channel->channel);

		gro_result = napi_gro_frags(napi);
	} else {
		struct sk_buff *skb = rx_buf->u.skb;

		EFX_BUG_ON_PARANOID(!(rx_buf->flags & EFX_RX_PKT_CSUMMED));
		rx_buf->u.skb = NULL;
		skb->ip_summed = CHECKSUM_UNNECESSARY;

		gro_result = napi_gro_receive(napi, skb);
	}

	if (gro_result == GRO_NORMAL) {
		channel->rx_alloc_level += RX_ALLOC_FACTOR_SKB;
	} else if (gro_result != GRO_DROP) {
		channel->rx_alloc_level += RX_ALLOC_FACTOR_GRO;
		channel->irq_mod_score += 2;
	}
}

void efx_rx_packet(struct efx_rx_queue *rx_queue, unsigned int index,
		   unsigned int len, u16 flags)
=======
/* Allocate and construct an SKB around page fragments */
static struct sk_buff *efx_rx_mk_skb(struct efx_channel *channel,
				     struct efx_rx_buffer *rx_buf,
				     unsigned int n_frags,
				     u8 *eh, int hdr_len)
{
	struct efx_nic *efx = channel->efx;
	struct sk_buff *skb;

	/* Allocate an SKB to store the headers */
	skb = netdev_alloc_skb(efx->net_dev,
			       efx->rx_ip_align + efx->rx_prefix_size +
			       hdr_len);
	if (unlikely(skb == NULL)) {
		atomic_inc(&efx->n_rx_noskb_drops);
		return NULL;
	}

	EFX_WARN_ON_ONCE_PARANOID(rx_buf->len < hdr_len);

	memcpy(skb->data + efx->rx_ip_align, eh - efx->rx_prefix_size,
	       efx->rx_prefix_size + hdr_len);
	skb_reserve(skb, efx->rx_ip_align + efx->rx_prefix_size);
	__skb_put(skb, hdr_len);

	/* Append the remaining page(s) onto the frag list */
	if (rx_buf->len > hdr_len) {
		rx_buf->page_offset += hdr_len;
		rx_buf->len -= hdr_len;

		for (;;) {
			skb_add_rx_frag(skb, skb_shinfo(skb)->nr_frags,
					rx_buf->page, rx_buf->page_offset,
					rx_buf->len, efx->rx_buffer_truesize);
			rx_buf->page = NULL;

			if (skb_shinfo(skb)->nr_frags == n_frags)
				break;

			rx_buf = efx_rx_buf_next(&channel->rx_queue, rx_buf);
		}
	} else {
		__free_pages(rx_buf->page, efx->rx_buffer_order);
		rx_buf->page = NULL;
		n_frags = 0;
	}

	/* Move past the ethernet header */
	skb->protocol = eth_type_trans(skb, efx->net_dev);

	skb_mark_napi_id(skb, &channel->napi_str);

	return skb;
}

void efx_rx_packet(struct efx_rx_queue *rx_queue, unsigned int index,
		   unsigned int n_frags, unsigned int len, u16 flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct efx_nic *efx = rx_queue->efx;
	struct efx_channel *channel = efx_rx_queue_channel(rx_queue);
	struct efx_rx_buffer *rx_buf;
<<<<<<< HEAD
	bool leak_packet = false;
=======

	rx_queue->rx_packets++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rx_buf = efx_rx_buffer(rx_queue, index);
	rx_buf->flags |= flags;

<<<<<<< HEAD
	/* This allows the refill path to post another buffer.
	 * EFX_RXD_HEAD_ROOM ensures that the slot we are using
	 * isn't overwritten yet.
	 */
	rx_queue->removed_count++;

	/* Validate the length encoded in the event vs the descriptor pushed */
	efx_rx_packet__check_len(rx_queue, rx_buf, len, &leak_packet);

	netif_vdbg(efx, rx_status, efx->net_dev,
		   "RX queue %d received id %x at %llx+%x %s%s\n",
		   efx_rx_queue_index(rx_queue), index,
		   (unsigned long long)rx_buf->dma_addr, len,
		   (rx_buf->flags & EFX_RX_PKT_CSUMMED) ? " [SUMMED]" : "",
		   (rx_buf->flags & EFX_RX_PKT_DISCARD) ? " [DISCARD]" : "");

	/* Discard packet, if instructed to do so */
	if (unlikely(rx_buf->flags & EFX_RX_PKT_DISCARD)) {
		if (unlikely(leak_packet))
			channel->n_skbuff_leaks++;
		else
			efx_recycle_rx_buffer(channel, rx_buf);

		/* Don't hold off the previous receive */
		rx_buf = NULL;
		goto out;
	}

	/* Release and/or sync DMA mapping - assumes all RX buffers
	 * consumed in-order per RX queue
	 */
	efx_unmap_rx_buffer(efx, rx_buf, len);
=======
	/* Validate the number of fragments and completed length */
	if (n_frags == 1) {
		if (!(flags & EFX_RX_PKT_PREFIX_LEN))
			efx_rx_packet__check_len(rx_queue, rx_buf, len);
	} else if (unlikely(n_frags > EFX_RX_MAX_FRAGS) ||
		   unlikely(len <= (n_frags - 1) * efx->rx_dma_len) ||
		   unlikely(len > n_frags * efx->rx_dma_len) ||
		   unlikely(!efx->rx_scatter)) {
		/* If this isn't an explicit discard request, either
		 * the hardware or the driver is broken.
		 */
		WARN_ON(!(len == 0 && rx_buf->flags & EFX_RX_PKT_DISCARD));
		rx_buf->flags |= EFX_RX_PKT_DISCARD;
	}

	netif_vdbg(efx, rx_status, efx->net_dev,
		   "RX queue %d received ids %x-%x len %d %s%s\n",
		   efx_rx_queue_index(rx_queue), index,
		   (index + n_frags - 1) & rx_queue->ptr_mask, len,
		   (rx_buf->flags & EFX_RX_PKT_CSUMMED) ? " [SUMMED]" : "",
		   (rx_buf->flags & EFX_RX_PKT_DISCARD) ? " [DISCARD]" : "");

	/* Discard packet, if instructed to do so.  Process the
	 * previous receive first.
	 */
	if (unlikely(rx_buf->flags & EFX_RX_PKT_DISCARD)) {
		efx_rx_flush_packet(channel);
		efx_discard_rx_packet(channel, rx_buf, n_frags);
		return;
	}

	if (n_frags == 1 && !(flags & EFX_RX_PKT_PREFIX_LEN))
		rx_buf->len = len;

	/* Release and/or sync the DMA mapping - assumes all RX buffers
	 * consumed in-order per RX queue.
	 */
	efx_sync_rx_buffer(efx, rx_buf, rx_buf->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Prefetch nice and early so data will (hopefully) be in cache by
	 * the time we look at it.
	 */
<<<<<<< HEAD
	prefetch(efx_rx_buf_eh(efx, rx_buf));
=======
	prefetch(efx_rx_buf_va(rx_buf));

	rx_buf->page_offset += efx->rx_prefix_size;
	rx_buf->len -= efx->rx_prefix_size;

	if (n_frags > 1) {
		/* Release/sync DMA mapping for additional fragments.
		 * Fix length for last fragment.
		 */
		unsigned int tail_frags = n_frags - 1;

		for (;;) {
			rx_buf = efx_rx_buf_next(rx_queue, rx_buf);
			if (--tail_frags == 0)
				break;
			efx_sync_rx_buffer(efx, rx_buf, efx->rx_dma_len);
		}
		rx_buf->len = len - (n_frags - 1) * efx->rx_dma_len;
		efx_sync_rx_buffer(efx, rx_buf, rx_buf->len);
	}

	/* All fragments have been DMA-synced, so recycle pages. */
	rx_buf = efx_rx_buffer(rx_queue, index);
	efx_recycle_rx_pages(channel, rx_buf, n_frags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Pipeline receives so that we give time for packet headers to be
	 * prefetched into cache.
	 */
<<<<<<< HEAD
	rx_buf->len = len - efx->type->rx_buffer_hash_size;
out:
	if (channel->rx_pkt)
		__efx_rx_packet(channel, channel->rx_pkt);
	channel->rx_pkt = rx_buf;
}

static void efx_rx_deliver(struct efx_channel *channel,
			   struct efx_rx_buffer *rx_buf)
{
	struct sk_buff *skb;

	/* We now own the SKB */
	skb = rx_buf->u.skb;
	rx_buf->u.skb = NULL;

	/* Set the SKB flags */
	skb_checksum_none_assert(skb);

	/* Pass the packet up */
	netif_receive_skb(skb);

	/* Update allocation strategy method */
	channel->rx_alloc_level += RX_ALLOC_FACTOR_SKB;
}

/* Handle a received packet.  Second half: Touches packet payload. */
void __efx_rx_packet(struct efx_channel *channel, struct efx_rx_buffer *rx_buf)
{
	struct efx_nic *efx = channel->efx;
	u8 *eh = efx_rx_buf_eh(efx, rx_buf);
=======
	efx_rx_flush_packet(channel);
	channel->rx_pkt_n_frags = n_frags;
	channel->rx_pkt_index = index;
}

static void efx_rx_deliver(struct efx_channel *channel, u8 *eh,
			   struct efx_rx_buffer *rx_buf,
			   unsigned int n_frags)
{
	struct sk_buff *skb;
	u16 hdr_len = min_t(u16, rx_buf->len, EFX_SKB_HEADERS);

	skb = efx_rx_mk_skb(channel, rx_buf, n_frags, eh, hdr_len);
	if (unlikely(skb == NULL)) {
		struct efx_rx_queue *rx_queue;

		rx_queue = efx_channel_get_rx_queue(channel);
		efx_free_rx_buffers(rx_queue, rx_buf, n_frags);
		return;
	}
	skb_record_rx_queue(skb, channel->rx_queue.core_index);

	/* Set the SKB flags */
	skb_checksum_none_assert(skb);
	if (likely(rx_buf->flags & EFX_RX_PKT_CSUMMED)) {
		skb->ip_summed = CHECKSUM_UNNECESSARY;
		skb->csum_level = !!(rx_buf->flags & EFX_RX_PKT_CSUM_LEVEL);
	}

	efx_rx_skb_attach_timestamp(channel, skb);

	if (channel->type->receive_skb)
		if (channel->type->receive_skb(channel, skb))
			return;

	/* Pass the packet up */
	if (channel->rx_list != NULL)
		/* Add to list, will pass up later */
		list_add_tail(&skb->list, channel->rx_list);
	else
		/* No list, so pass it up now */
		netif_receive_skb(skb);
}

/** efx_do_xdp: perform XDP processing on a received packet
 *
 * Returns true if packet should still be delivered.
 */
static bool efx_do_xdp(struct efx_nic *efx, struct efx_channel *channel,
		       struct efx_rx_buffer *rx_buf, u8 **ehp)
{
	u8 rx_prefix[EFX_MAX_RX_PREFIX_SIZE];
	struct efx_rx_queue *rx_queue;
	struct bpf_prog *xdp_prog;
	struct xdp_frame *xdpf;
	struct xdp_buff xdp;
	u32 xdp_act;
	s16 offset;
	int err;

	xdp_prog = rcu_dereference_bh(efx->xdp_prog);
	if (!xdp_prog)
		return true;

	rx_queue = efx_channel_get_rx_queue(channel);

	if (unlikely(channel->rx_pkt_n_frags > 1)) {
		/* We can't do XDP on fragmented packets - drop. */
		efx_free_rx_buffers(rx_queue, rx_buf,
				    channel->rx_pkt_n_frags);
		if (net_ratelimit())
			netif_err(efx, rx_err, efx->net_dev,
				  "XDP is not possible with multiple receive fragments (%d)\n",
				  channel->rx_pkt_n_frags);
		channel->n_rx_xdp_bad_drops++;
		return false;
	}

	dma_sync_single_for_cpu(&efx->pci_dev->dev, rx_buf->dma_addr,
				rx_buf->len, DMA_FROM_DEVICE);

	/* Save the rx prefix. */
	EFX_WARN_ON_PARANOID(efx->rx_prefix_size > EFX_MAX_RX_PREFIX_SIZE);
	memcpy(rx_prefix, *ehp - efx->rx_prefix_size,
	       efx->rx_prefix_size);

	xdp_init_buff(&xdp, efx->rx_page_buf_step, &rx_queue->xdp_rxq_info);
	/* No support yet for XDP metadata */
	xdp_prepare_buff(&xdp, *ehp - EFX_XDP_HEADROOM, EFX_XDP_HEADROOM,
			 rx_buf->len, false);

	xdp_act = bpf_prog_run_xdp(xdp_prog, &xdp);

	offset = (u8 *)xdp.data - *ehp;

	switch (xdp_act) {
	case XDP_PASS:
		/* Fix up rx prefix. */
		if (offset) {
			*ehp += offset;
			rx_buf->page_offset += offset;
			rx_buf->len -= offset;
			memcpy(*ehp - efx->rx_prefix_size, rx_prefix,
			       efx->rx_prefix_size);
		}
		break;

	case XDP_TX:
		/* Buffer ownership passes to tx on success. */
		xdpf = xdp_convert_buff_to_frame(&xdp);
		err = efx_xdp_tx_buffers(efx, 1, &xdpf, true);
		if (unlikely(err != 1)) {
			efx_free_rx_buffers(rx_queue, rx_buf, 1);
			if (net_ratelimit())
				netif_err(efx, rx_err, efx->net_dev,
					  "XDP TX failed (%d)\n", err);
			channel->n_rx_xdp_bad_drops++;
			trace_xdp_exception(efx->net_dev, xdp_prog, xdp_act);
		} else {
			channel->n_rx_xdp_tx++;
		}
		break;

	case XDP_REDIRECT:
		err = xdp_do_redirect(efx->net_dev, &xdp, xdp_prog);
		if (unlikely(err)) {
			efx_free_rx_buffers(rx_queue, rx_buf, 1);
			if (net_ratelimit())
				netif_err(efx, rx_err, efx->net_dev,
					  "XDP redirect failed (%d)\n", err);
			channel->n_rx_xdp_bad_drops++;
			trace_xdp_exception(efx->net_dev, xdp_prog, xdp_act);
		} else {
			channel->n_rx_xdp_redirect++;
		}
		break;

	default:
		bpf_warn_invalid_xdp_action(efx->net_dev, xdp_prog, xdp_act);
		efx_free_rx_buffers(rx_queue, rx_buf, 1);
		channel->n_rx_xdp_bad_drops++;
		trace_xdp_exception(efx->net_dev, xdp_prog, xdp_act);
		break;

	case XDP_ABORTED:
		trace_xdp_exception(efx->net_dev, xdp_prog, xdp_act);
		fallthrough;
	case XDP_DROP:
		efx_free_rx_buffers(rx_queue, rx_buf, 1);
		channel->n_rx_xdp_drops++;
		break;
	}

	return xdp_act == XDP_PASS;
}

/* Handle a received packet.  Second half: Touches packet payload. */
void __efx_rx_packet(struct efx_channel *channel)
{
	struct efx_rx_queue *rx_queue = efx_channel_get_rx_queue(channel);
	struct efx_nic *efx = channel->efx;
	struct efx_rx_buffer *rx_buf =
		efx_rx_buffer(rx_queue, channel->rx_pkt_index);
	u8 *eh = efx_rx_buf_va(rx_buf);

	/* Read length from the prefix if necessary.  This already
	 * excludes the length of the prefix itself.
	 */
	if (rx_buf->flags & EFX_RX_PKT_PREFIX_LEN) {
		rx_buf->len = le16_to_cpup((__le16 *)
					   (eh + efx->rx_packet_len_offset));
		/* A known issue may prevent this being filled in;
		 * if that happens, just drop the packet.
		 * Must do that in the driver since passing a zero-length
		 * packet up to the stack may cause a crash.
		 */
		if (unlikely(!rx_buf->len)) {
			efx_free_rx_buffers(rx_queue, rx_buf,
					    channel->rx_pkt_n_frags);
			channel->n_rx_frm_trunc++;
			goto out;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If we're in loopback test, then pass the packet directly to the
	 * loopback layer, and free the rx_buf here
	 */
	if (unlikely(efx->loopback_selftest)) {
		efx_loopback_rx_packet(efx, eh, rx_buf->len);
<<<<<<< HEAD
		efx_free_rx_buffer(efx, rx_buf);
		return;
	}

	if (!(rx_buf->flags & EFX_RX_BUF_PAGE)) {
		struct sk_buff *skb = rx_buf->u.skb;

		prefetch(skb_shinfo(skb));

		skb_reserve(skb, efx->type->rx_buffer_hash_size);
		skb_put(skb, rx_buf->len);

		if (efx->net_dev->features & NETIF_F_RXHASH)
			skb->rxhash = efx_rx_buf_hash(eh);

		/* Move past the ethernet header. rx_buf->data still points
		 * at the ethernet header */
		skb->protocol = eth_type_trans(skb, efx->net_dev);

		skb_record_rx_queue(skb, channel->channel);
	}
=======
		efx_free_rx_buffers(rx_queue, rx_buf,
				    channel->rx_pkt_n_frags);
		goto out;
	}

	if (!efx_do_xdp(efx, channel, rx_buf, &eh))
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(!(efx->net_dev->features & NETIF_F_RXCSUM)))
		rx_buf->flags &= ~EFX_RX_PKT_CSUMMED;

<<<<<<< HEAD
	if (likely(rx_buf->flags & (EFX_RX_BUF_PAGE | EFX_RX_PKT_CSUMMED)))
		efx_rx_packet_gro(channel, rx_buf, eh);
	else
		efx_rx_deliver(channel, rx_buf);
}

void efx_rx_strategy(struct efx_channel *channel)
{
	enum efx_rx_alloc_method method = rx_alloc_method;

	/* Only makes sense to use page based allocation if GRO is enabled */
	if (!(channel->efx->net_dev->features & NETIF_F_GRO)) {
		method = RX_ALLOC_METHOD_SKB;
	} else if (method == RX_ALLOC_METHOD_AUTO) {
		/* Constrain the rx_alloc_level */
		if (channel->rx_alloc_level < 0)
			channel->rx_alloc_level = 0;
		else if (channel->rx_alloc_level > RX_ALLOC_LEVEL_MAX)
			channel->rx_alloc_level = RX_ALLOC_LEVEL_MAX;

		/* Decide on the allocation method */
		method = ((channel->rx_alloc_level > RX_ALLOC_LEVEL_GRO) ?
			  RX_ALLOC_METHOD_PAGE : RX_ALLOC_METHOD_SKB);
	}

	/* Push the option */
	channel->rx_alloc_push_pages = (method == RX_ALLOC_METHOD_PAGE);
}

int efx_probe_rx_queue(struct efx_rx_queue *rx_queue)
{
	struct efx_nic *efx = rx_queue->efx;
	unsigned int entries;
	int rc;

	/* Create the smallest power-of-two aligned ring */
	entries = max(roundup_pow_of_two(efx->rxq_entries), EFX_MIN_DMAQ_SIZE);
	EFX_BUG_ON_PARANOID(entries > EFX_MAX_DMAQ_SIZE);
	rx_queue->ptr_mask = entries - 1;

	netif_dbg(efx, probe, efx->net_dev,
		  "creating RX queue %d size %#x mask %#x\n",
		  efx_rx_queue_index(rx_queue), efx->rxq_entries,
		  rx_queue->ptr_mask);

	/* Allocate RX buffers */
	rx_queue->buffer = kcalloc(entries, sizeof(*rx_queue->buffer),
				   GFP_KERNEL);
	if (!rx_queue->buffer)
		return -ENOMEM;

	rc = efx_nic_probe_rx(rx_queue);
	if (rc) {
		kfree(rx_queue->buffer);
		rx_queue->buffer = NULL;
	}
	return rc;
}

void efx_init_rx_queue(struct efx_rx_queue *rx_queue)
{
	struct efx_nic *efx = rx_queue->efx;
	unsigned int max_fill, trigger, limit;

	netif_dbg(rx_queue->efx, drv, rx_queue->efx->net_dev,
		  "initialising RX queue %d\n", efx_rx_queue_index(rx_queue));

	/* Initialise ptr fields */
	rx_queue->added_count = 0;
	rx_queue->notified_count = 0;
	rx_queue->removed_count = 0;
	rx_queue->min_fill = -1U;

	/* Initialise limit fields */
	max_fill = efx->rxq_entries - EFX_RXD_HEAD_ROOM;
	trigger = max_fill * min(rx_refill_threshold, 100U) / 100U;
	limit = max_fill * min(rx_refill_limit, 100U) / 100U;

	rx_queue->max_fill = max_fill;
	rx_queue->fast_fill_trigger = trigger;
	rx_queue->fast_fill_limit = limit;

	/* Set up RX descriptor ring */
	rx_queue->enabled = true;
	efx_nic_init_rx(rx_queue);
}

void efx_fini_rx_queue(struct efx_rx_queue *rx_queue)
{
	int i;
	struct efx_rx_buffer *rx_buf;

	netif_dbg(rx_queue->efx, drv, rx_queue->efx->net_dev,
		  "shutting down RX queue %d\n", efx_rx_queue_index(rx_queue));

	/* A flush failure might have left rx_queue->enabled */
	rx_queue->enabled = false;

	del_timer_sync(&rx_queue->slow_fill);
	efx_nic_fini_rx(rx_queue);

	/* Release RX buffers NB start at index 0 not current HW ptr */
	if (rx_queue->buffer) {
		for (i = 0; i <= rx_queue->ptr_mask; i++) {
			rx_buf = efx_rx_buffer(rx_queue, i);
			efx_fini_rx_buffer(rx_queue, rx_buf);
		}
	}
}

void efx_remove_rx_queue(struct efx_rx_queue *rx_queue)
{
	netif_dbg(rx_queue->efx, drv, rx_queue->efx->net_dev,
		  "destroying RX queue %d\n", efx_rx_queue_index(rx_queue));

	efx_nic_remove_rx(rx_queue);

	kfree(rx_queue->buffer);
	rx_queue->buffer = NULL;
}


module_param(rx_alloc_method, int, 0644);
MODULE_PARM_DESC(rx_alloc_method, "Allocation method used for RX buffers");

module_param(rx_refill_threshold, uint, 0444);
MODULE_PARM_DESC(rx_refill_threshold,
		 "RX descriptor ring fast/slow fill threshold (%)");

=======
	if ((rx_buf->flags & EFX_RX_PKT_TCP) && !channel->type->receive_skb)
		efx_rx_packet_gro(channel, rx_buf, channel->rx_pkt_n_frags, eh, 0);
	else
		efx_rx_deliver(channel, eh, rx_buf, channel->rx_pkt_n_frags);
out:
	channel->rx_pkt_n_frags = 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
