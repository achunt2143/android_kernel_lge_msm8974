<<<<<<< HEAD
/****************************************************************************
 * Driver for Solarflare Solarstorm network controllers and boards
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2005-2010 Solarflare Communications Inc.
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

#include <linux/pci.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/ipv6.h>
#include <linux/slab.h>
#include <net/ipv6.h>
#include <linux/if_ether.h>
#include <linux/highmem.h>
<<<<<<< HEAD
#include "net_driver.h"
#include "efx.h"
#include "nic.h"
#include "workarounds.h"

/*
 * TX descriptor ring full threshold
 *
 * The tx_queue descriptor ring fill-level must fall below this value
 * before we restart the netif queue
 */
#define EFX_TXQ_THRESHOLD(_efx) ((_efx)->txq_entries / 2u)

static void efx_dequeue_buffer(struct efx_tx_queue *tx_queue,
			       struct efx_tx_buffer *buffer,
			       unsigned int *pkts_compl,
			       unsigned int *bytes_compl)
{
	if (buffer->unmap_len) {
		struct pci_dev *pci_dev = tx_queue->efx->pci_dev;
		dma_addr_t unmap_addr = (buffer->dma_addr + buffer->len -
					 buffer->unmap_len);
		if (buffer->unmap_single)
			pci_unmap_single(pci_dev, unmap_addr, buffer->unmap_len,
					 PCI_DMA_TODEVICE);
		else
			pci_unmap_page(pci_dev, unmap_addr, buffer->unmap_len,
				       PCI_DMA_TODEVICE);
		buffer->unmap_len = 0;
		buffer->unmap_single = false;
	}

	if (buffer->skb) {
		(*pkts_compl)++;
		(*bytes_compl) += buffer->skb->len;
		dev_kfree_skb_any((struct sk_buff *) buffer->skb);
		buffer->skb = NULL;
		netif_vdbg(tx_queue->efx, tx_done, tx_queue->efx->net_dev,
			   "TX queue %d transmission id %x complete\n",
			   tx_queue->queue, tx_queue->read_count);
	}
}

/**
 * struct efx_tso_header - a DMA mapped buffer for packet headers
 * @next: Linked list of free ones.
 *	The list is protected by the TX queue lock.
 * @dma_unmap_len: Length to unmap for an oversize buffer, or 0.
 * @dma_addr: The DMA address of the header below.
 *
 * This controls the memory used for a TSO header.  Use TSOH_DATA()
 * to find the packet header data.  Use TSOH_SIZE() to calculate the
 * total size required for a given packet header length.  TSO headers
 * in the free list are exactly %TSOH_STD_SIZE bytes in size.
 */
struct efx_tso_header {
	union {
		struct efx_tso_header *next;
		size_t unmap_len;
	};
	dma_addr_t dma_addr;
};

static int efx_enqueue_skb_tso(struct efx_tx_queue *tx_queue,
			       struct sk_buff *skb);
static void efx_fini_tso(struct efx_tx_queue *tx_queue);
static void efx_tsoh_heap_free(struct efx_tx_queue *tx_queue,
			       struct efx_tso_header *tsoh);

static void efx_tsoh_free(struct efx_tx_queue *tx_queue,
			  struct efx_tx_buffer *buffer)
{
	if (buffer->tsoh) {
		if (likely(!buffer->tsoh->unmap_len)) {
			buffer->tsoh->next = tx_queue->tso_headers_free;
			tx_queue->tso_headers_free = buffer->tsoh;
		} else {
			efx_tsoh_heap_free(tx_queue, buffer->tsoh);
		}
		buffer->tsoh = NULL;
	}
}


static inline unsigned
efx_max_tx_len(struct efx_nic *efx, dma_addr_t dma_addr)
{
	/* Depending on the NIC revision, we can use descriptor
	 * lengths up to 8K or 8K-1.  However, since PCI Express
	 * devices must split read requests at 4K boundaries, there is
	 * little benefit from using descriptors that cross those
	 * boundaries and we keep things simple by not doing so.
	 */
	unsigned len = (~dma_addr & (EFX_PAGE_SIZE - 1)) + 1;

	/* Work around hardware bug for unaligned buffers. */
	if (EFX_WORKAROUND_5391(efx) && (dma_addr & 0xf))
		len = min_t(unsigned, len, 512 - (dma_addr & 0xf));

	return len;
}

unsigned int efx_tx_max_skb_descs(struct efx_nic *efx)
{
	/* Header and payload descriptor for each output segment, plus
	 * one for every input fragment boundary within a segment
	 */
	unsigned int max_descs = EFX_TSO_MAX_SEGS * 2 + MAX_SKB_FRAGS;

	/* Possibly one more per segment for the alignment workaround */
	if (EFX_WORKAROUND_5391(efx))
		max_descs += EFX_TSO_MAX_SEGS;

	/* Possibly more for PCIe page boundaries within input fragments */
	if (PAGE_SIZE > EFX_PAGE_SIZE)
		max_descs += max_t(unsigned int, MAX_SKB_FRAGS,
				   DIV_ROUND_UP(GSO_MAX_SIZE, EFX_PAGE_SIZE));

	return max_descs;
=======
#include <linux/cache.h>
#include "net_driver.h"
#include "efx.h"
#include "io.h"
#include "nic.h"
#include "tx.h"
#include "tx_common.h"
#include "workarounds.h"
#include "ef10_regs.h"

#ifdef EFX_USE_PIO

#define EFX_PIOBUF_SIZE_DEF ALIGN(256, L1_CACHE_BYTES)
unsigned int efx_piobuf_size __read_mostly = EFX_PIOBUF_SIZE_DEF;

#endif /* EFX_USE_PIO */

static inline u8 *efx_tx_get_copy_buffer(struct efx_tx_queue *tx_queue,
					 struct efx_tx_buffer *buffer)
{
	unsigned int index = efx_tx_queue_get_insert_index(tx_queue);
	struct efx_buffer *page_buf =
		&tx_queue->cb_page[index >> (PAGE_SHIFT - EFX_TX_CB_ORDER)];
	unsigned int offset =
		((index << EFX_TX_CB_ORDER) + NET_IP_ALIGN) & (PAGE_SIZE - 1);

	if (unlikely(!page_buf->addr) &&
	    efx_nic_alloc_buffer(tx_queue->efx, page_buf, PAGE_SIZE,
				 GFP_ATOMIC))
		return NULL;
	buffer->dma_addr = page_buf->dma_addr + offset;
	buffer->unmap_len = 0;
	return (u8 *)page_buf->addr + offset;
}

u8 *efx_tx_get_copy_buffer_limited(struct efx_tx_queue *tx_queue,
				   struct efx_tx_buffer *buffer, size_t len)
{
	if (len > EFX_TX_CB_SIZE)
		return NULL;
	return efx_tx_get_copy_buffer(tx_queue, buffer);
}

static void efx_tx_maybe_stop_queue(struct efx_tx_queue *txq1)
{
	/* We need to consider all queues that the net core sees as one */
	struct efx_nic *efx = txq1->efx;
	struct efx_tx_queue *txq2;
	unsigned int fill_level;

	fill_level = efx_channel_tx_old_fill_level(txq1->channel);
	if (likely(fill_level < efx->txq_stop_thresh))
		return;

	/* We used the stale old_read_count above, which gives us a
	 * pessimistic estimate of the fill level (which may even
	 * validly be >= efx->txq_entries).  Now try again using
	 * read_count (more likely to be a cache miss).
	 *
	 * If we read read_count and then conditionally stop the
	 * queue, it is possible for the completion path to race with
	 * us and complete all outstanding descriptors in the middle,
	 * after which there will be no more completions to wake it.
	 * Therefore we stop the queue first, then read read_count
	 * (with a memory barrier to ensure the ordering), then
	 * restart the queue if the fill level turns out to be low
	 * enough.
	 */
	netif_tx_stop_queue(txq1->core_txq);
	smp_mb();
	efx_for_each_channel_tx_queue(txq2, txq1->channel)
		txq2->old_read_count = READ_ONCE(txq2->read_count);

	fill_level = efx_channel_tx_old_fill_level(txq1->channel);
	EFX_WARN_ON_ONCE_PARANOID(fill_level >= efx->txq_entries);
	if (likely(fill_level < efx->txq_stop_thresh)) {
		smp_mb();
		if (likely(!efx->loopback_selftest))
			netif_tx_start_queue(txq1->core_txq);
	}
}

static int efx_enqueue_skb_copy(struct efx_tx_queue *tx_queue,
				struct sk_buff *skb)
{
	unsigned int copy_len = skb->len;
	struct efx_tx_buffer *buffer;
	u8 *copy_buffer;
	int rc;

	EFX_WARN_ON_ONCE_PARANOID(copy_len > EFX_TX_CB_SIZE);

	buffer = efx_tx_queue_get_insert_buffer(tx_queue);

	copy_buffer = efx_tx_get_copy_buffer(tx_queue, buffer);
	if (unlikely(!copy_buffer))
		return -ENOMEM;

	rc = skb_copy_bits(skb, 0, copy_buffer, copy_len);
	EFX_WARN_ON_PARANOID(rc);
	buffer->len = copy_len;

	buffer->skb = skb;
	buffer->flags = EFX_TX_BUF_SKB;

	++tx_queue->insert_count;
	return rc;
}

#ifdef EFX_USE_PIO

struct efx_short_copy_buffer {
	int used;
	u8 buf[L1_CACHE_BYTES];
};

/* Copy to PIO, respecting that writes to PIO buffers must be dword aligned.
 * Advances piobuf pointer. Leaves additional data in the copy buffer.
 */
static void efx_memcpy_toio_aligned(struct efx_nic *efx, u8 __iomem **piobuf,
				    u8 *data, int len,
				    struct efx_short_copy_buffer *copy_buf)
{
	int block_len = len & ~(sizeof(copy_buf->buf) - 1);

	__iowrite64_copy(*piobuf, data, block_len >> 3);
	*piobuf += block_len;
	len -= block_len;

	if (len) {
		data += block_len;
		BUG_ON(copy_buf->used);
		BUG_ON(len > sizeof(copy_buf->buf));
		memcpy(copy_buf->buf, data, len);
		copy_buf->used = len;
	}
}

/* Copy to PIO, respecting dword alignment, popping data from copy buffer first.
 * Advances piobuf pointer. Leaves additional data in the copy buffer.
 */
static void efx_memcpy_toio_aligned_cb(struct efx_nic *efx, u8 __iomem **piobuf,
				       u8 *data, int len,
				       struct efx_short_copy_buffer *copy_buf)
{
	if (copy_buf->used) {
		/* if the copy buffer is partially full, fill it up and write */
		int copy_to_buf =
			min_t(int, sizeof(copy_buf->buf) - copy_buf->used, len);

		memcpy(copy_buf->buf + copy_buf->used, data, copy_to_buf);
		copy_buf->used += copy_to_buf;

		/* if we didn't fill it up then we're done for now */
		if (copy_buf->used < sizeof(copy_buf->buf))
			return;

		__iowrite64_copy(*piobuf, copy_buf->buf,
				 sizeof(copy_buf->buf) >> 3);
		*piobuf += sizeof(copy_buf->buf);
		data += copy_to_buf;
		len -= copy_to_buf;
		copy_buf->used = 0;
	}

	efx_memcpy_toio_aligned(efx, piobuf, data, len, copy_buf);
}

static void efx_flush_copy_buffer(struct efx_nic *efx, u8 __iomem *piobuf,
				  struct efx_short_copy_buffer *copy_buf)
{
	/* if there's anything in it, write the whole buffer, including junk */
	if (copy_buf->used)
		__iowrite64_copy(piobuf, copy_buf->buf,
				 sizeof(copy_buf->buf) >> 3);
}

/* Traverse skb structure and copy fragments in to PIO buffer.
 * Advances piobuf pointer.
 */
static void efx_skb_copy_bits_to_pio(struct efx_nic *efx, struct sk_buff *skb,
				     u8 __iomem **piobuf,
				     struct efx_short_copy_buffer *copy_buf)
{
	int i;

	efx_memcpy_toio_aligned(efx, piobuf, skb->data, skb_headlen(skb),
				copy_buf);

	for (i = 0; i < skb_shinfo(skb)->nr_frags; ++i) {
		skb_frag_t *f = &skb_shinfo(skb)->frags[i];
		u8 *vaddr;

		vaddr = kmap_local_page(skb_frag_page(f));

		efx_memcpy_toio_aligned_cb(efx, piobuf, vaddr + skb_frag_off(f),
					   skb_frag_size(f), copy_buf);
		kunmap_local(vaddr);
	}

	EFX_WARN_ON_ONCE_PARANOID(skb_shinfo(skb)->frag_list);
}

static int efx_enqueue_skb_pio(struct efx_tx_queue *tx_queue,
			       struct sk_buff *skb)
{
	struct efx_tx_buffer *buffer =
		efx_tx_queue_get_insert_buffer(tx_queue);
	u8 __iomem *piobuf = tx_queue->piobuf;

	/* Copy to PIO buffer. Ensure the writes are padded to the end
	 * of a cache line, as this is required for write-combining to be
	 * effective on at least x86.
	 */

	if (skb_shinfo(skb)->nr_frags) {
		/* The size of the copy buffer will ensure all writes
		 * are the size of a cache line.
		 */
		struct efx_short_copy_buffer copy_buf;

		copy_buf.used = 0;

		efx_skb_copy_bits_to_pio(tx_queue->efx, skb,
					 &piobuf, &copy_buf);
		efx_flush_copy_buffer(tx_queue->efx, piobuf, &copy_buf);
	} else {
		/* Pad the write to the size of a cache line.
		 * We can do this because we know the skb_shared_info struct is
		 * after the source, and the destination buffer is big enough.
		 */
		BUILD_BUG_ON(L1_CACHE_BYTES >
			     SKB_DATA_ALIGN(sizeof(struct skb_shared_info)));
		__iowrite64_copy(tx_queue->piobuf, skb->data,
				 ALIGN(skb->len, L1_CACHE_BYTES) >> 3);
	}

	buffer->skb = skb;
	buffer->flags = EFX_TX_BUF_SKB | EFX_TX_BUF_OPTION;

	EFX_POPULATE_QWORD_5(buffer->option,
			     ESF_DZ_TX_DESC_IS_OPT, 1,
			     ESF_DZ_TX_OPTION_TYPE, ESE_DZ_TX_OPTION_DESC_PIO,
			     ESF_DZ_TX_PIO_CONT, 0,
			     ESF_DZ_TX_PIO_BYTE_CNT, skb->len,
			     ESF_DZ_TX_PIO_BUF_ADDR,
			     tx_queue->piobuf_offset);
	++tx_queue->insert_count;
	return 0;
}

/* Decide whether we can use TX PIO, ie. write packet data directly into
 * a buffer on the device.  This can reduce latency at the expense of
 * throughput, so we only do this if both hardware and software TX rings
 * are empty, including all queues for the channel.  This also ensures that
 * only one packet at a time can be using the PIO buffer. If the xmit_more
 * flag is set then we don't use this - there'll be another packet along
 * shortly and we want to hold off the doorbell.
 */
static bool efx_tx_may_pio(struct efx_tx_queue *tx_queue)
{
	struct efx_channel *channel = tx_queue->channel;

	if (!tx_queue->piobuf)
		return false;

	EFX_WARN_ON_ONCE_PARANOID(!channel->efx->type->option_descriptors);

	efx_for_each_channel_tx_queue(tx_queue, channel)
		if (!efx_nic_tx_is_empty(tx_queue, tx_queue->packet_write_count))
			return false;

	return true;
}
#endif /* EFX_USE_PIO */

/* Send any pending traffic for a channel. xmit_more is shared across all
 * queues for a channel, so we must check all of them.
 */
static void efx_tx_send_pending(struct efx_channel *channel)
{
	struct efx_tx_queue *q;

	efx_for_each_channel_tx_queue(q, channel) {
		if (q->xmit_pending)
			efx_nic_push_buffers(q);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Add a socket buffer to a TX queue
 *
 * This maps all fragments of a socket buffer for DMA and adds them to
 * the TX queue.  The queue's insert pointer will be incremented by
 * the number of fragments in the socket buffer.
 *
 * If any DMA mapping fails, any mapped fragments will be unmapped,
 * the queue's insert pointer will be restored to its original value.
 *
 * This function is split out from efx_hard_start_xmit to allow the
 * loopback test to direct packets via specific TX queues.
 *
<<<<<<< HEAD
 * Returns NETDEV_TX_OK or NETDEV_TX_BUSY
 * You must hold netif_tx_lock() to call this function.
 */
netdev_tx_t efx_enqueue_skb(struct efx_tx_queue *tx_queue, struct sk_buff *skb)
{
	struct efx_nic *efx = tx_queue->efx;
	struct pci_dev *pci_dev = efx->pci_dev;
	struct efx_tx_buffer *buffer;
	skb_frag_t *fragment;
	unsigned int len, unmap_len = 0, fill_level, insert_ptr;
	dma_addr_t dma_addr, unmap_addr = 0;
	unsigned int dma_len;
	bool unmap_single;
	int q_space, i = 0;
	netdev_tx_t rc = NETDEV_TX_OK;

	EFX_BUG_ON_PARANOID(tx_queue->write_count != tx_queue->insert_count);

	if (skb_shinfo(skb)->gso_size)
		return efx_enqueue_skb_tso(tx_queue, skb);

	/* Get size of the initial fragment */
	len = skb_headlen(skb);

	/* Pad if necessary */
	if (EFX_WORKAROUND_15592(efx) && skb->len <= 32) {
		EFX_BUG_ON_PARANOID(skb->data_len);
		len = 32 + 1;
		if (skb_pad(skb, len - skb->len))
			return NETDEV_TX_OK;
	}

	fill_level = tx_queue->insert_count - tx_queue->old_read_count;
	q_space = efx->txq_entries - 1 - fill_level;

	/* Map for DMA.  Use pci_map_single rather than pci_map_page
	 * since this is more efficient on machines with sparse
	 * memory.
	 */
	unmap_single = true;
	dma_addr = pci_map_single(pci_dev, skb->data, len, PCI_DMA_TODEVICE);

	/* Process all fragments */
	while (1) {
		if (unlikely(pci_dma_mapping_error(pci_dev, dma_addr)))
			goto pci_err;

		/* Store fields for marking in the per-fragment final
		 * descriptor */
		unmap_len = len;
		unmap_addr = dma_addr;

		/* Add to TX queue, splitting across DMA boundaries */
		do {
			if (unlikely(q_space-- <= 0)) {
				/* It might be that completions have
				 * happened since the xmit path last
				 * checked.  Update the xmit path's
				 * copy of read_count.
				 */
				netif_tx_stop_queue(tx_queue->core_txq);
				/* This memory barrier protects the
				 * change of queue state from the access
				 * of read_count. */
				smp_mb();
				tx_queue->old_read_count =
					ACCESS_ONCE(tx_queue->read_count);
				fill_level = (tx_queue->insert_count
					      - tx_queue->old_read_count);
				q_space = efx->txq_entries - 1 - fill_level;
				if (unlikely(q_space-- <= 0)) {
					rc = NETDEV_TX_BUSY;
					goto unwind;
				}
				smp_mb();
				if (likely(!efx->loopback_selftest))
					netif_tx_start_queue(
						tx_queue->core_txq);
			}

			insert_ptr = tx_queue->insert_count & tx_queue->ptr_mask;
			buffer = &tx_queue->buffer[insert_ptr];
			efx_tsoh_free(tx_queue, buffer);
			EFX_BUG_ON_PARANOID(buffer->tsoh);
			EFX_BUG_ON_PARANOID(buffer->skb);
			EFX_BUG_ON_PARANOID(buffer->len);
			EFX_BUG_ON_PARANOID(!buffer->continuation);
			EFX_BUG_ON_PARANOID(buffer->unmap_len);

			dma_len = efx_max_tx_len(efx, dma_addr);
			if (likely(dma_len >= len))
				dma_len = len;

			/* Fill out per descriptor fields */
			buffer->len = dma_len;
			buffer->dma_addr = dma_addr;
			len -= dma_len;
			dma_addr += dma_len;
			++tx_queue->insert_count;
		} while (len);

		/* Transfer ownership of the unmapping to the final buffer */
		buffer->unmap_single = unmap_single;
		buffer->unmap_len = unmap_len;
		unmap_len = 0;

		/* Get address and size of next fragment */
		if (i >= skb_shinfo(skb)->nr_frags)
			break;
		fragment = &skb_shinfo(skb)->frags[i];
		len = skb_frag_size(fragment);
		i++;
		/* Map for DMA */
		unmap_single = false;
		dma_addr = skb_frag_dma_map(&pci_dev->dev, fragment, 0, len,
					    DMA_TO_DEVICE);
	}

	/* Transfer ownership of the skb to the final buffer */
	buffer->skb = skb;
	buffer->continuation = false;

	netdev_tx_sent_queue(tx_queue->core_txq, skb->len);

	/* Pass off to hardware */
	efx_nic_push_buffers(tx_queue);

	return NETDEV_TX_OK;

 pci_err:
	netif_err(efx, tx_err, efx->net_dev,
		  " TX queue %d could not map skb with %d bytes %d "
		  "fragments for DMA\n", tx_queue->queue, skb->len,
		  skb_shinfo(skb)->nr_frags + 1);

	/* Mark the packet as transmitted, and free the SKB ourselves */
	dev_kfree_skb_any(skb);

 unwind:
	/* Work backwards until we hit the original insert pointer value */
	while (tx_queue->insert_count != tx_queue->write_count) {
		unsigned int pkts_compl = 0, bytes_compl = 0;
		--tx_queue->insert_count;
		insert_ptr = tx_queue->insert_count & tx_queue->ptr_mask;
		buffer = &tx_queue->buffer[insert_ptr];
		efx_dequeue_buffer(tx_queue, buffer, &pkts_compl, &bytes_compl);
		buffer->len = 0;
	}

	/* Free the fragment we were mid-way through pushing */
	if (unmap_len) {
		if (unmap_single)
			pci_unmap_single(pci_dev, unmap_addr, unmap_len,
					 PCI_DMA_TODEVICE);
		else
			pci_unmap_page(pci_dev, unmap_addr, unmap_len,
				       PCI_DMA_TODEVICE);
	}

	return rc;
}

/* Remove packets from the TX queue
 *
 * This removes packets from the TX queue, up to and including the
 * specified index.
 */
static void efx_dequeue_buffers(struct efx_tx_queue *tx_queue,
				unsigned int index,
				unsigned int *pkts_compl,
				unsigned int *bytes_compl)
{
	struct efx_nic *efx = tx_queue->efx;
	unsigned int stop_index, read_ptr;

	stop_index = (index + 1) & tx_queue->ptr_mask;
	read_ptr = tx_queue->read_count & tx_queue->ptr_mask;

	while (read_ptr != stop_index) {
		struct efx_tx_buffer *buffer = &tx_queue->buffer[read_ptr];
		if (unlikely(buffer->len == 0)) {
			netif_err(efx, tx_err, efx->net_dev,
				  "TX queue %d spurious TX completion id %x\n",
				  tx_queue->queue, read_ptr);
			efx_schedule_reset(efx, RESET_TYPE_TX_SKIP);
			return;
		}

		efx_dequeue_buffer(tx_queue, buffer, pkts_compl, bytes_compl);
		buffer->continuation = true;
		buffer->len = 0;

		++tx_queue->read_count;
		read_ptr = tx_queue->read_count & tx_queue->ptr_mask;
	}
}

/* Initiate a packet transmission.  We use one channel per CPU
 * (sharing when we have more CPUs than channels).  On Falcon, the TX
 * completion events will be directed back to the CPU that transmitted
 * the packet, which should be cache-efficient.
 *
 * Context: non-blocking.
 * Note that returning anything other than NETDEV_TX_OK will cause the
 * OS to free the skb.
=======
 * Returns NETDEV_TX_OK.
 * You must hold netif_tx_lock() to call this function.
 */
netdev_tx_t __efx_enqueue_skb(struct efx_tx_queue *tx_queue, struct sk_buff *skb)
{
	unsigned int old_insert_count = tx_queue->insert_count;
	bool xmit_more = netdev_xmit_more();
	bool data_mapped = false;
	unsigned int segments;
	unsigned int skb_len;
	int rc;

	skb_len = skb->len;
	segments = skb_is_gso(skb) ? skb_shinfo(skb)->gso_segs : 0;
	if (segments == 1)
		segments = 0; /* Don't use TSO for a single segment. */

	/* Handle TSO first - it's *possible* (although unlikely) that we might
	 * be passed a packet to segment that's smaller than the copybreak/PIO
	 * size limit.
	 */
	if (segments) {
		switch (tx_queue->tso_version) {
		case 1:
			rc = efx_enqueue_skb_tso(tx_queue, skb, &data_mapped);
			break;
		case 2:
			rc = efx_ef10_tx_tso_desc(tx_queue, skb, &data_mapped);
			break;
		case 0: /* No TSO on this queue, SW fallback needed */
		default:
			rc = -EINVAL;
			break;
		}
		if (rc == -EINVAL) {
			rc = efx_tx_tso_fallback(tx_queue, skb);
			tx_queue->tso_fallbacks++;
			if (rc == 0)
				return 0;
		}
		if (rc)
			goto err;
#ifdef EFX_USE_PIO
	} else if (skb_len <= efx_piobuf_size && !xmit_more &&
		   efx_tx_may_pio(tx_queue)) {
		/* Use PIO for short packets with an empty queue. */
		if (efx_enqueue_skb_pio(tx_queue, skb))
			goto err;
		tx_queue->pio_packets++;
		data_mapped = true;
#endif
	} else if (skb->data_len && skb_len <= EFX_TX_CB_SIZE) {
		/* Pad short packets or coalesce short fragmented packets. */
		if (efx_enqueue_skb_copy(tx_queue, skb))
			goto err;
		tx_queue->cb_packets++;
		data_mapped = true;
	}

	/* Map for DMA and create descriptors if we haven't done so already. */
	if (!data_mapped && (efx_tx_map_data(tx_queue, skb, segments)))
		goto err;

	efx_tx_maybe_stop_queue(tx_queue);

	tx_queue->xmit_pending = true;

	/* Pass off to hardware */
	if (__netdev_tx_sent_queue(tx_queue->core_txq, skb_len, xmit_more))
		efx_tx_send_pending(tx_queue->channel);

	if (segments) {
		tx_queue->tso_bursts++;
		tx_queue->tso_packets += segments;
		tx_queue->tx_packets  += segments;
	} else {
		tx_queue->tx_packets++;
	}

	return NETDEV_TX_OK;


err:
	efx_enqueue_unwind(tx_queue, old_insert_count);
	dev_kfree_skb_any(skb);

	/* If we're not expecting another transmit and we had something to push
	 * on this queue or a partner queue then we need to push here to get the
	 * previous packets out.
	 */
	if (!xmit_more)
		efx_tx_send_pending(tx_queue->channel);

	return NETDEV_TX_OK;
}

/* Transmit a packet from an XDP buffer
 *
 * Returns number of packets sent on success, error code otherwise.
 * Runs in NAPI context, either in our poll (for XDP TX) or a different NIC
 * (for XDP redirect).
 */
int efx_xdp_tx_buffers(struct efx_nic *efx, int n, struct xdp_frame **xdpfs,
		       bool flush)
{
	struct efx_tx_buffer *tx_buffer;
	struct efx_tx_queue *tx_queue;
	struct xdp_frame *xdpf;
	dma_addr_t dma_addr;
	unsigned int len;
	int space;
	int cpu;
	int i = 0;

	if (unlikely(n && !xdpfs))
		return -EINVAL;
	if (unlikely(!n))
		return 0;

	cpu = raw_smp_processor_id();
	if (unlikely(cpu >= efx->xdp_tx_queue_count))
		return -EINVAL;

	tx_queue = efx->xdp_tx_queues[cpu];
	if (unlikely(!tx_queue))
		return -EINVAL;

	if (!tx_queue->initialised)
		return -EINVAL;

	if (efx->xdp_txq_queues_mode != EFX_XDP_TX_QUEUES_DEDICATED)
		HARD_TX_LOCK(efx->net_dev, tx_queue->core_txq, cpu);

	/* If we're borrowing net stack queues we have to handle stop-restart
	 * or we might block the queue and it will be considered as frozen
	 */
	if (efx->xdp_txq_queues_mode == EFX_XDP_TX_QUEUES_BORROWED) {
		if (netif_tx_queue_stopped(tx_queue->core_txq))
			goto unlock;
		efx_tx_maybe_stop_queue(tx_queue);
	}

	/* Check for available space. We should never need multiple
	 * descriptors per frame.
	 */
	space = efx->txq_entries +
		tx_queue->read_count - tx_queue->insert_count;

	for (i = 0; i < n; i++) {
		xdpf = xdpfs[i];

		if (i >= space)
			break;

		/* We'll want a descriptor for this tx. */
		prefetchw(__efx_tx_queue_get_insert_buffer(tx_queue));

		len = xdpf->len;

		/* Map for DMA. */
		dma_addr = dma_map_single(&efx->pci_dev->dev,
					  xdpf->data, len,
					  DMA_TO_DEVICE);
		if (dma_mapping_error(&efx->pci_dev->dev, dma_addr))
			break;

		/*  Create descriptor and set up for unmapping DMA. */
		tx_buffer = efx_tx_map_chunk(tx_queue, dma_addr, len);
		tx_buffer->xdpf = xdpf;
		tx_buffer->flags = EFX_TX_BUF_XDP |
				   EFX_TX_BUF_MAP_SINGLE;
		tx_buffer->dma_offset = 0;
		tx_buffer->unmap_len = len;
		tx_queue->tx_packets++;
	}

	/* Pass mapped frames to hardware. */
	if (flush && i > 0)
		efx_nic_push_buffers(tx_queue);

unlock:
	if (efx->xdp_txq_queues_mode != EFX_XDP_TX_QUEUES_DEDICATED)
		HARD_TX_UNLOCK(efx->net_dev, tx_queue->core_txq);

	return i == 0 ? -EIO : i;
}

/* Initiate a packet transmission.  We use one channel per CPU
 * (sharing when we have more CPUs than channels).
 *
 * Context: non-blocking.
 * Should always return NETDEV_TX_OK and consume the skb.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
netdev_tx_t efx_hard_start_xmit(struct sk_buff *skb,
				struct net_device *net_dev)
{
<<<<<<< HEAD
	struct efx_nic *efx = netdev_priv(net_dev);
=======
	struct efx_nic *efx = efx_netdev_priv(net_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct efx_tx_queue *tx_queue;
	unsigned index, type;

	EFX_WARN_ON_PARANOID(!netif_device_present(net_dev));
<<<<<<< HEAD

	index = skb_get_queue_mapping(skb);
	type = skb->ip_summed == CHECKSUM_PARTIAL ? EFX_TXQ_TYPE_OFFLOAD : 0;
	if (index >= efx->n_tx_channels) {
		index -= efx->n_tx_channels;
		type |= EFX_TXQ_TYPE_HIGHPRI;
	}
	tx_queue = efx_get_tx_queue(efx, index, type);

	return efx_enqueue_skb(tx_queue, skb);
=======
	index = skb_get_queue_mapping(skb);
	type = efx_tx_csum_type_skb(skb);

	/* PTP "event" packet */
	if (unlikely(efx_xmit_with_hwtstamp(skb)) &&
	    ((efx_ptp_use_mac_tx_timestamps(efx) && efx->ptp_data) ||
	    unlikely(efx_ptp_is_ptp_tx(efx, skb)))) {
		/* There may be existing transmits on the channel that are
		 * waiting for this packet to trigger the doorbell write.
		 * We need to send the packets at this point.
		 */
		efx_tx_send_pending(efx_get_tx_channel(efx, index));
		return efx_ptp_tx(efx, skb);
	}

	tx_queue = efx_get_tx_queue(efx, index, type);
	if (WARN_ON_ONCE(!tx_queue)) {
		/* We don't have a TXQ of the right type.
		 * This should never happen, as we don't advertise offload
		 * features unless we can support them.
		 */
		dev_kfree_skb_any(skb);
		/* If we're not expecting another transmit and we had something to push
		 * on this queue or a partner queue then we need to push here to get the
		 * previous packets out.
		 */
		if (!netdev_xmit_more())
			efx_tx_send_pending(efx_get_tx_channel(efx, index));
		return NETDEV_TX_OK;
	}

	return __efx_enqueue_skb(tx_queue, skb);
}

void efx_xmit_done_single(struct efx_tx_queue *tx_queue)
{
	unsigned int pkts_compl = 0, bytes_compl = 0;
	unsigned int efv_pkts_compl = 0;
	unsigned int read_ptr;
	bool finished = false;

	read_ptr = tx_queue->read_count & tx_queue->ptr_mask;

	while (!finished) {
		struct efx_tx_buffer *buffer = &tx_queue->buffer[read_ptr];

		if (!efx_tx_buffer_in_use(buffer)) {
			struct efx_nic *efx = tx_queue->efx;

			netif_err(efx, hw, efx->net_dev,
				  "TX queue %d spurious single TX completion\n",
				  tx_queue->queue);
			efx_schedule_reset(efx, RESET_TYPE_TX_SKIP);
			return;
		}

		/* Need to check the flag before dequeueing. */
		if (buffer->flags & EFX_TX_BUF_SKB)
			finished = true;
		efx_dequeue_buffer(tx_queue, buffer, &pkts_compl, &bytes_compl,
				   &efv_pkts_compl);

		++tx_queue->read_count;
		read_ptr = tx_queue->read_count & tx_queue->ptr_mask;
	}

	tx_queue->pkts_compl += pkts_compl;
	tx_queue->bytes_compl += bytes_compl;

	EFX_WARN_ON_PARANOID(pkts_compl + efv_pkts_compl != 1);

	efx_xmit_done_check_empty(tx_queue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void efx_init_tx_queue_core_txq(struct efx_tx_queue *tx_queue)
{
	struct efx_nic *efx = tx_queue->efx;

	/* Must be inverse of queue lookup in efx_hard_start_xmit() */
	tx_queue->core_txq =
		netdev_get_tx_queue(efx->net_dev,
<<<<<<< HEAD
				    tx_queue->queue / EFX_TXQ_TYPES +
				    ((tx_queue->queue & EFX_TXQ_TYPE_HIGHPRI) ?
				     efx->n_tx_channels : 0));
}

int efx_setup_tc(struct net_device *net_dev, u8 num_tc)
{
	struct efx_nic *efx = netdev_priv(net_dev);
	struct efx_channel *channel;
	struct efx_tx_queue *tx_queue;
	unsigned tc;
	int rc;

	if (efx_nic_rev(efx) < EFX_REV_FALCON_B0 || num_tc > EFX_MAX_TX_TC)
		return -EINVAL;

	if (num_tc == net_dev->num_tc)
		return 0;

	for (tc = 0; tc < num_tc; tc++) {
		net_dev->tc_to_txq[tc].offset = tc * efx->n_tx_channels;
		net_dev->tc_to_txq[tc].count = efx->n_tx_channels;
	}

	if (num_tc > net_dev->num_tc) {
		/* Initialise high-priority queues as necessary */
		efx_for_each_channel(channel, efx) {
			efx_for_each_possible_channel_tx_queue(tx_queue,
							       channel) {
				if (!(tx_queue->queue & EFX_TXQ_TYPE_HIGHPRI))
					continue;
				if (!tx_queue->buffer) {
					rc = efx_probe_tx_queue(tx_queue);
					if (rc)
						return rc;
				}
				if (!tx_queue->initialised)
					efx_init_tx_queue(tx_queue);
				efx_init_tx_queue_core_txq(tx_queue);
			}
		}
	} else {
		/* Reduce number of classes before number of queues */
		net_dev->num_tc = num_tc;
	}

	rc = netif_set_real_num_tx_queues(net_dev,
					  max_t(int, num_tc, 1) *
					  efx->n_tx_channels);
	if (rc)
		return rc;

	/* Do not destroy high-priority queues when they become
	 * unused.  We would have to flush them first, and it is
	 * fairly difficult to flush a subset of TX queues.  Leave
	 * it to efx_fini_channels().
	 */

	net_dev->num_tc = num_tc;
	return 0;
}

void efx_xmit_done(struct efx_tx_queue *tx_queue, unsigned int index)
{
	unsigned fill_level;
	struct efx_nic *efx = tx_queue->efx;
	unsigned int pkts_compl = 0, bytes_compl = 0;

	EFX_BUG_ON_PARANOID(index > tx_queue->ptr_mask);

	efx_dequeue_buffers(tx_queue, index, &pkts_compl, &bytes_compl);
	netdev_tx_completed_queue(tx_queue->core_txq, pkts_compl, bytes_compl);

	/* See if we need to restart the netif queue.  This barrier
	 * separates the update of read_count from the test of the
	 * queue state. */
	smp_mb();
	if (unlikely(netif_tx_queue_stopped(tx_queue->core_txq)) &&
	    likely(efx->port_enabled) &&
	    likely(netif_device_present(efx->net_dev))) {
		fill_level = tx_queue->insert_count - tx_queue->read_count;
		if (fill_level < EFX_TXQ_THRESHOLD(efx))
			netif_tx_wake_queue(tx_queue->core_txq);
	}

	/* Check whether the hardware queue is now empty */
	if ((int)(tx_queue->read_count - tx_queue->old_write_count) >= 0) {
		tx_queue->old_write_count = ACCESS_ONCE(tx_queue->write_count);
		if (tx_queue->read_count == tx_queue->old_write_count) {
			smp_mb();
			tx_queue->empty_read_count =
				tx_queue->read_count | EFX_EMPTY_COUNT_VALID;
		}
	}
}

int efx_probe_tx_queue(struct efx_tx_queue *tx_queue)
{
	struct efx_nic *efx = tx_queue->efx;
	unsigned int entries;
	int i, rc;

	/* Create the smallest power-of-two aligned ring */
	entries = max(roundup_pow_of_two(efx->txq_entries), EFX_MIN_DMAQ_SIZE);
	EFX_BUG_ON_PARANOID(entries > EFX_MAX_DMAQ_SIZE);
	tx_queue->ptr_mask = entries - 1;

	netif_dbg(efx, probe, efx->net_dev,
		  "creating TX queue %d size %#x mask %#x\n",
		  tx_queue->queue, efx->txq_entries, tx_queue->ptr_mask);

	/* Allocate software ring */
	tx_queue->buffer = kcalloc(entries, sizeof(*tx_queue->buffer),
				   GFP_KERNEL);
	if (!tx_queue->buffer)
		return -ENOMEM;
	for (i = 0; i <= tx_queue->ptr_mask; ++i)
		tx_queue->buffer[i].continuation = true;

	/* Allocate hardware ring */
	rc = efx_nic_probe_tx(tx_queue);
	if (rc)
		goto fail;

	return 0;

 fail:
	kfree(tx_queue->buffer);
	tx_queue->buffer = NULL;
	return rc;
}

void efx_init_tx_queue(struct efx_tx_queue *tx_queue)
{
	netif_dbg(tx_queue->efx, drv, tx_queue->efx->net_dev,
		  "initialising TX queue %d\n", tx_queue->queue);

	tx_queue->insert_count = 0;
	tx_queue->write_count = 0;
	tx_queue->old_write_count = 0;
	tx_queue->read_count = 0;
	tx_queue->old_read_count = 0;
	tx_queue->empty_read_count = 0 | EFX_EMPTY_COUNT_VALID;

	/* Set up TX descriptor ring */
	efx_nic_init_tx(tx_queue);

	tx_queue->initialised = true;
}

void efx_release_tx_buffers(struct efx_tx_queue *tx_queue)
{
	struct efx_tx_buffer *buffer;

	if (!tx_queue->buffer)
		return;

	/* Free any buffers left in the ring */
	while (tx_queue->read_count != tx_queue->write_count) {
		unsigned int pkts_compl = 0, bytes_compl = 0;
		buffer = &tx_queue->buffer[tx_queue->read_count & tx_queue->ptr_mask];
		efx_dequeue_buffer(tx_queue, buffer, &pkts_compl, &bytes_compl);
		buffer->continuation = true;
		buffer->len = 0;

		++tx_queue->read_count;
	}
	netdev_tx_reset_queue(tx_queue->core_txq);
}

void efx_fini_tx_queue(struct efx_tx_queue *tx_queue)
{
	if (!tx_queue->initialised)
		return;

	netif_dbg(tx_queue->efx, drv, tx_queue->efx->net_dev,
		  "shutting down TX queue %d\n", tx_queue->queue);

	tx_queue->initialised = false;

	/* Flush TX queue, remove descriptor ring */
	efx_nic_fini_tx(tx_queue);

	efx_release_tx_buffers(tx_queue);

	/* Free up TSO header cache */
	efx_fini_tso(tx_queue);
}

void efx_remove_tx_queue(struct efx_tx_queue *tx_queue)
{
	if (!tx_queue->buffer)
		return;

	netif_dbg(tx_queue->efx, drv, tx_queue->efx->net_dev,
		  "destroying TX queue %d\n", tx_queue->queue);
	efx_nic_remove_tx(tx_queue);

	kfree(tx_queue->buffer);
	tx_queue->buffer = NULL;
}


/* Efx TCP segmentation acceleration.
 *
 * Why?  Because by doing it here in the driver we can go significantly
 * faster than the GSO.
 *
 * Requires TX checksum offload support.
 */

/* Number of bytes inserted at the start of a TSO header buffer,
 * similar to NET_IP_ALIGN.
 */
#ifdef CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS
#define TSOH_OFFSET	0
#else
#define TSOH_OFFSET	NET_IP_ALIGN
#endif

#define TSOH_BUFFER(tsoh)	((u8 *)(tsoh + 1) + TSOH_OFFSET)

/* Total size of struct efx_tso_header, buffer and padding */
#define TSOH_SIZE(hdr_len)					\
	(sizeof(struct efx_tso_header) + TSOH_OFFSET + hdr_len)

/* Size of blocks on free list.  Larger blocks must be allocated from
 * the heap.
 */
#define TSOH_STD_SIZE		128

#define PTR_DIFF(p1, p2)  ((u8 *)(p1) - (u8 *)(p2))
#define ETH_HDR_LEN(skb)  (skb_network_header(skb) - (skb)->data)
#define SKB_TCP_OFF(skb)  PTR_DIFF(tcp_hdr(skb), (skb)->data)
#define SKB_IPV4_OFF(skb) PTR_DIFF(ip_hdr(skb), (skb)->data)
#define SKB_IPV6_OFF(skb) PTR_DIFF(ipv6_hdr(skb), (skb)->data)

/**
 * struct tso_state - TSO state for an SKB
 * @out_len: Remaining length in current segment
 * @seqnum: Current sequence number
 * @ipv4_id: Current IPv4 ID, host endian
 * @packet_space: Remaining space in current packet
 * @dma_addr: DMA address of current position
 * @in_len: Remaining length in current SKB fragment
 * @unmap_len: Length of SKB fragment
 * @unmap_addr: DMA address of SKB fragment
 * @unmap_single: DMA single vs page mapping flag
 * @protocol: Network protocol (after any VLAN header)
 * @header_len: Number of bytes of header
 * @full_packet_size: Number of bytes to put in each outgoing segment
 *
 * The state used during segmentation.  It is put into this data structure
 * just to make it easy to pass into inline functions.
 */
struct tso_state {
	/* Output position */
	unsigned out_len;
	unsigned seqnum;
	unsigned ipv4_id;
	unsigned packet_space;

	/* Input position */
	dma_addr_t dma_addr;
	unsigned in_len;
	unsigned unmap_len;
	dma_addr_t unmap_addr;
	bool unmap_single;

	__be16 protocol;
	unsigned header_len;
	int full_packet_size;
};


/*
 * Verify that our various assumptions about sk_buffs and the conditions
 * under which TSO will be attempted hold true.  Return the protocol number.
 */
static __be16 efx_tso_check_protocol(struct sk_buff *skb)
{
	__be16 protocol = skb->protocol;

	EFX_BUG_ON_PARANOID(((struct ethhdr *)skb->data)->h_proto !=
			    protocol);
	if (protocol == htons(ETH_P_8021Q)) {
		/* Find the encapsulated protocol; reset network header
		 * and transport header based on that. */
		struct vlan_ethhdr *veh = (struct vlan_ethhdr *)skb->data;
		protocol = veh->h_vlan_encapsulated_proto;
		skb_set_network_header(skb, sizeof(*veh));
		if (protocol == htons(ETH_P_IP))
			skb_set_transport_header(skb, sizeof(*veh) +
						 4 * ip_hdr(skb)->ihl);
		else if (protocol == htons(ETH_P_IPV6))
			skb_set_transport_header(skb, sizeof(*veh) +
						 sizeof(struct ipv6hdr));
	}

	if (protocol == htons(ETH_P_IP)) {
		EFX_BUG_ON_PARANOID(ip_hdr(skb)->protocol != IPPROTO_TCP);
	} else {
		EFX_BUG_ON_PARANOID(protocol != htons(ETH_P_IPV6));
		EFX_BUG_ON_PARANOID(ipv6_hdr(skb)->nexthdr != NEXTHDR_TCP);
	}
	EFX_BUG_ON_PARANOID((PTR_DIFF(tcp_hdr(skb), skb->data)
			     + (tcp_hdr(skb)->doff << 2u)) >
			    skb_headlen(skb));

	return protocol;
}


/*
 * Allocate a page worth of efx_tso_header structures, and string them
 * into the tx_queue->tso_headers_free linked list. Return 0 or -ENOMEM.
 */
static int efx_tsoh_block_alloc(struct efx_tx_queue *tx_queue)
{

	struct pci_dev *pci_dev = tx_queue->efx->pci_dev;
	struct efx_tso_header *tsoh;
	dma_addr_t dma_addr;
	u8 *base_kva, *kva;

	base_kva = pci_alloc_consistent(pci_dev, PAGE_SIZE, &dma_addr);
	if (base_kva == NULL) {
		netif_err(tx_queue->efx, tx_err, tx_queue->efx->net_dev,
			  "Unable to allocate page for TSO headers\n");
		return -ENOMEM;
	}

	/* pci_alloc_consistent() allocates pages. */
	EFX_BUG_ON_PARANOID(dma_addr & (PAGE_SIZE - 1u));

	for (kva = base_kva; kva < base_kva + PAGE_SIZE; kva += TSOH_STD_SIZE) {
		tsoh = (struct efx_tso_header *)kva;
		tsoh->dma_addr = dma_addr + (TSOH_BUFFER(tsoh) - base_kva);
		tsoh->next = tx_queue->tso_headers_free;
		tx_queue->tso_headers_free = tsoh;
	}

	return 0;
}


/* Free up a TSO header, and all others in the same page. */
static void efx_tsoh_block_free(struct efx_tx_queue *tx_queue,
				struct efx_tso_header *tsoh,
				struct pci_dev *pci_dev)
{
	struct efx_tso_header **p;
	unsigned long base_kva;
	dma_addr_t base_dma;

	base_kva = (unsigned long)tsoh & PAGE_MASK;
	base_dma = tsoh->dma_addr & PAGE_MASK;

	p = &tx_queue->tso_headers_free;
	while (*p != NULL) {
		if (((unsigned long)*p & PAGE_MASK) == base_kva)
			*p = (*p)->next;
		else
			p = &(*p)->next;
	}

	pci_free_consistent(pci_dev, PAGE_SIZE, (void *)base_kva, base_dma);
}

static struct efx_tso_header *
efx_tsoh_heap_alloc(struct efx_tx_queue *tx_queue, size_t header_len)
{
	struct efx_tso_header *tsoh;

	tsoh = kmalloc(TSOH_SIZE(header_len), GFP_ATOMIC | GFP_DMA);
	if (unlikely(!tsoh))
		return NULL;

	tsoh->dma_addr = pci_map_single(tx_queue->efx->pci_dev,
					TSOH_BUFFER(tsoh), header_len,
					PCI_DMA_TODEVICE);
	if (unlikely(pci_dma_mapping_error(tx_queue->efx->pci_dev,
					   tsoh->dma_addr))) {
		kfree(tsoh);
		return NULL;
	}

	tsoh->unmap_len = header_len;
	return tsoh;
}

static void
efx_tsoh_heap_free(struct efx_tx_queue *tx_queue, struct efx_tso_header *tsoh)
{
	pci_unmap_single(tx_queue->efx->pci_dev,
			 tsoh->dma_addr, tsoh->unmap_len,
			 PCI_DMA_TODEVICE);
	kfree(tsoh);
}

/**
 * efx_tx_queue_insert - push descriptors onto the TX queue
 * @tx_queue:		Efx TX queue
 * @dma_addr:		DMA address of fragment
 * @len:		Length of fragment
 * @final_buffer:	The final buffer inserted into the queue
 *
 * Push descriptors onto the TX queue.  Return 0 on success or 1 if
 * @tx_queue full.
 */
static int efx_tx_queue_insert(struct efx_tx_queue *tx_queue,
			       dma_addr_t dma_addr, unsigned len,
			       struct efx_tx_buffer **final_buffer)
{
	struct efx_tx_buffer *buffer;
	struct efx_nic *efx = tx_queue->efx;
	unsigned dma_len, fill_level, insert_ptr;
	int q_space;

	EFX_BUG_ON_PARANOID(len <= 0);

	fill_level = tx_queue->insert_count - tx_queue->old_read_count;
	/* -1 as there is no way to represent all descriptors used */
	q_space = efx->txq_entries - 1 - fill_level;

	while (1) {
		if (unlikely(q_space-- <= 0)) {
			/* It might be that completions have happened
			 * since the xmit path last checked.  Update
			 * the xmit path's copy of read_count.
			 */
			netif_tx_stop_queue(tx_queue->core_txq);
			/* This memory barrier protects the change of
			 * queue state from the access of read_count. */
			smp_mb();
			tx_queue->old_read_count =
				ACCESS_ONCE(tx_queue->read_count);
			fill_level = (tx_queue->insert_count
				      - tx_queue->old_read_count);
			q_space = efx->txq_entries - 1 - fill_level;
			if (unlikely(q_space-- <= 0)) {
				*final_buffer = NULL;
				return 1;
			}
			smp_mb();
			netif_tx_start_queue(tx_queue->core_txq);
		}

		insert_ptr = tx_queue->insert_count & tx_queue->ptr_mask;
		buffer = &tx_queue->buffer[insert_ptr];
		++tx_queue->insert_count;

		EFX_BUG_ON_PARANOID(tx_queue->insert_count -
				    tx_queue->read_count >=
				    efx->txq_entries);

		efx_tsoh_free(tx_queue, buffer);
		EFX_BUG_ON_PARANOID(buffer->len);
		EFX_BUG_ON_PARANOID(buffer->unmap_len);
		EFX_BUG_ON_PARANOID(buffer->skb);
		EFX_BUG_ON_PARANOID(!buffer->continuation);
		EFX_BUG_ON_PARANOID(buffer->tsoh);

		buffer->dma_addr = dma_addr;

		dma_len = efx_max_tx_len(efx, dma_addr);

		/* If there is enough space to send then do so */
		if (dma_len >= len)
			break;

		buffer->len = dma_len; /* Don't set the other members */
		dma_addr += dma_len;
		len -= dma_len;
	}

	EFX_BUG_ON_PARANOID(!len);
	buffer->len = len;
	*final_buffer = buffer;
	return 0;
}


/*
 * Put a TSO header into the TX queue.
 *
 * This is special-cased because we know that it is small enough to fit in
 * a single fragment, and we know it doesn't cross a page boundary.  It
 * also allows us to not worry about end-of-packet etc.
 */
static void efx_tso_put_header(struct efx_tx_queue *tx_queue,
			       struct efx_tso_header *tsoh, unsigned len)
{
	struct efx_tx_buffer *buffer;

	buffer = &tx_queue->buffer[tx_queue->insert_count & tx_queue->ptr_mask];
	efx_tsoh_free(tx_queue, buffer);
	EFX_BUG_ON_PARANOID(buffer->len);
	EFX_BUG_ON_PARANOID(buffer->unmap_len);
	EFX_BUG_ON_PARANOID(buffer->skb);
	EFX_BUG_ON_PARANOID(!buffer->continuation);
	EFX_BUG_ON_PARANOID(buffer->tsoh);
	buffer->len = len;
	buffer->dma_addr = tsoh->dma_addr;
	buffer->tsoh = tsoh;

	++tx_queue->insert_count;
}


/* Remove descriptors put into a tx_queue. */
static void efx_enqueue_unwind(struct efx_tx_queue *tx_queue)
{
	struct efx_tx_buffer *buffer;
	dma_addr_t unmap_addr;

	/* Work backwards until we hit the original insert pointer value */
	while (tx_queue->insert_count != tx_queue->write_count) {
		--tx_queue->insert_count;
		buffer = &tx_queue->buffer[tx_queue->insert_count &
					   tx_queue->ptr_mask];
		efx_tsoh_free(tx_queue, buffer);
		EFX_BUG_ON_PARANOID(buffer->skb);
		if (buffer->unmap_len) {
			unmap_addr = (buffer->dma_addr + buffer->len -
				      buffer->unmap_len);
			if (buffer->unmap_single)
				pci_unmap_single(tx_queue->efx->pci_dev,
						 unmap_addr, buffer->unmap_len,
						 PCI_DMA_TODEVICE);
			else
				pci_unmap_page(tx_queue->efx->pci_dev,
					       unmap_addr, buffer->unmap_len,
					       PCI_DMA_TODEVICE);
			buffer->unmap_len = 0;
		}
		buffer->len = 0;
		buffer->continuation = true;
	}
}


/* Parse the SKB header and initialise state. */
static void tso_start(struct tso_state *st, const struct sk_buff *skb)
{
	/* All ethernet/IP/TCP headers combined size is TCP header size
	 * plus offset of TCP header relative to start of packet.
	 */
	st->header_len = ((tcp_hdr(skb)->doff << 2u)
			  + PTR_DIFF(tcp_hdr(skb), skb->data));
	st->full_packet_size = st->header_len + skb_shinfo(skb)->gso_size;

	if (st->protocol == htons(ETH_P_IP))
		st->ipv4_id = ntohs(ip_hdr(skb)->id);
	else
		st->ipv4_id = 0;
	st->seqnum = ntohl(tcp_hdr(skb)->seq);

	EFX_BUG_ON_PARANOID(tcp_hdr(skb)->urg);
	EFX_BUG_ON_PARANOID(tcp_hdr(skb)->syn);
	EFX_BUG_ON_PARANOID(tcp_hdr(skb)->rst);

	st->packet_space = st->full_packet_size;
	st->out_len = skb->len - st->header_len;
	st->unmap_len = 0;
	st->unmap_single = false;
}

static int tso_get_fragment(struct tso_state *st, struct efx_nic *efx,
			    skb_frag_t *frag)
{
	st->unmap_addr = skb_frag_dma_map(&efx->pci_dev->dev, frag, 0,
					  skb_frag_size(frag), DMA_TO_DEVICE);
	if (likely(!dma_mapping_error(&efx->pci_dev->dev, st->unmap_addr))) {
		st->unmap_single = false;
		st->unmap_len = skb_frag_size(frag);
		st->in_len = skb_frag_size(frag);
		st->dma_addr = st->unmap_addr;
		return 0;
	}
	return -ENOMEM;
}

static int tso_get_head_fragment(struct tso_state *st, struct efx_nic *efx,
				 const struct sk_buff *skb)
{
	int hl = st->header_len;
	int len = skb_headlen(skb) - hl;

	st->unmap_addr = pci_map_single(efx->pci_dev, skb->data + hl,
					len, PCI_DMA_TODEVICE);
	if (likely(!pci_dma_mapping_error(efx->pci_dev, st->unmap_addr))) {
		st->unmap_single = true;
		st->unmap_len = len;
		st->in_len = len;
		st->dma_addr = st->unmap_addr;
		return 0;
	}
	return -ENOMEM;
}


/**
 * tso_fill_packet_with_fragment - form descriptors for the current fragment
 * @tx_queue:		Efx TX queue
 * @skb:		Socket buffer
 * @st:			TSO state
 *
 * Form descriptors for the current fragment, until we reach the end
 * of fragment or end-of-packet.  Return 0 on success, 1 if not enough
 * space in @tx_queue.
 */
static int tso_fill_packet_with_fragment(struct efx_tx_queue *tx_queue,
					 const struct sk_buff *skb,
					 struct tso_state *st)
{
	struct efx_tx_buffer *buffer;
	int n, end_of_packet, rc;

	if (st->in_len == 0)
		return 0;
	if (st->packet_space == 0)
		return 0;

	EFX_BUG_ON_PARANOID(st->in_len <= 0);
	EFX_BUG_ON_PARANOID(st->packet_space <= 0);

	n = min(st->in_len, st->packet_space);

	st->packet_space -= n;
	st->out_len -= n;
	st->in_len -= n;

	rc = efx_tx_queue_insert(tx_queue, st->dma_addr, n, &buffer);
	if (likely(rc == 0)) {
		if (st->out_len == 0)
			/* Transfer ownership of the skb */
			buffer->skb = skb;

		end_of_packet = st->out_len == 0 || st->packet_space == 0;
		buffer->continuation = !end_of_packet;

		if (st->in_len == 0) {
			/* Transfer ownership of the pci mapping */
			buffer->unmap_len = st->unmap_len;
			buffer->unmap_single = st->unmap_single;
			st->unmap_len = 0;
		}
	}

	st->dma_addr += n;
	return rc;
}


/**
 * tso_start_new_packet - generate a new header and prepare for the new packet
 * @tx_queue:		Efx TX queue
 * @skb:		Socket buffer
 * @st:			TSO state
 *
 * Generate a new header and prepare for the new packet.  Return 0 on
 * success, or -1 if failed to alloc header.
 */
static int tso_start_new_packet(struct efx_tx_queue *tx_queue,
				const struct sk_buff *skb,
				struct tso_state *st)
{
	struct efx_tso_header *tsoh;
	struct tcphdr *tsoh_th;
	unsigned ip_length;
	u8 *header;

	/* Allocate a DMA-mapped header buffer. */
	if (likely(TSOH_SIZE(st->header_len) <= TSOH_STD_SIZE)) {
		if (tx_queue->tso_headers_free == NULL) {
			if (efx_tsoh_block_alloc(tx_queue))
				return -1;
		}
		EFX_BUG_ON_PARANOID(!tx_queue->tso_headers_free);
		tsoh = tx_queue->tso_headers_free;
		tx_queue->tso_headers_free = tsoh->next;
		tsoh->unmap_len = 0;
	} else {
		tx_queue->tso_long_headers++;
		tsoh = efx_tsoh_heap_alloc(tx_queue, st->header_len);
		if (unlikely(!tsoh))
			return -1;
	}

	header = TSOH_BUFFER(tsoh);
	tsoh_th = (struct tcphdr *)(header + SKB_TCP_OFF(skb));

	/* Copy and update the headers. */
	memcpy(header, skb->data, st->header_len);

	tsoh_th->seq = htonl(st->seqnum);
	st->seqnum += skb_shinfo(skb)->gso_size;
	if (st->out_len > skb_shinfo(skb)->gso_size) {
		/* This packet will not finish the TSO burst. */
		ip_length = st->full_packet_size - ETH_HDR_LEN(skb);
		tsoh_th->fin = 0;
		tsoh_th->psh = 0;
	} else {
		/* This packet will be the last in the TSO burst. */
		ip_length = st->header_len - ETH_HDR_LEN(skb) + st->out_len;
		tsoh_th->fin = tcp_hdr(skb)->fin;
		tsoh_th->psh = tcp_hdr(skb)->psh;
	}

	if (st->protocol == htons(ETH_P_IP)) {
		struct iphdr *tsoh_iph =
			(struct iphdr *)(header + SKB_IPV4_OFF(skb));

		tsoh_iph->tot_len = htons(ip_length);

		/* Linux leaves suitable gaps in the IP ID space for us to fill. */
		tsoh_iph->id = htons(st->ipv4_id);
		st->ipv4_id++;
	} else {
		struct ipv6hdr *tsoh_iph =
			(struct ipv6hdr *)(header + SKB_IPV6_OFF(skb));

		tsoh_iph->payload_len = htons(ip_length - sizeof(*tsoh_iph));
	}

	st->packet_space = skb_shinfo(skb)->gso_size;
	++tx_queue->tso_packets;

	/* Form a descriptor for this header. */
	efx_tso_put_header(tx_queue, tsoh, st->header_len);

	return 0;
}


/**
 * efx_enqueue_skb_tso - segment and transmit a TSO socket buffer
 * @tx_queue:		Efx TX queue
 * @skb:		Socket buffer
 *
 * Context: You must hold netif_tx_lock() to call this function.
 *
 * Add socket buffer @skb to @tx_queue, doing TSO or return != 0 if
 * @skb was not enqueued.  In all cases @skb is consumed.  Return
 * %NETDEV_TX_OK or %NETDEV_TX_BUSY.
 */
static int efx_enqueue_skb_tso(struct efx_tx_queue *tx_queue,
			       struct sk_buff *skb)
{
	struct efx_nic *efx = tx_queue->efx;
	int frag_i, rc, rc2 = NETDEV_TX_OK;
	struct tso_state state;

	/* Find the packet protocol and sanity-check it */
	state.protocol = efx_tso_check_protocol(skb);

	EFX_BUG_ON_PARANOID(tx_queue->write_count != tx_queue->insert_count);

	tso_start(&state, skb);

	/* Assume that skb header area contains exactly the headers, and
	 * all payload is in the frag list.
	 */
	if (skb_headlen(skb) == state.header_len) {
		/* Grab the first payload fragment. */
		EFX_BUG_ON_PARANOID(skb_shinfo(skb)->nr_frags < 1);
		frag_i = 0;
		rc = tso_get_fragment(&state, efx,
				      skb_shinfo(skb)->frags + frag_i);
		if (rc)
			goto mem_err;
	} else {
		rc = tso_get_head_fragment(&state, efx, skb);
		if (rc)
			goto mem_err;
		frag_i = -1;
	}

	if (tso_start_new_packet(tx_queue, skb, &state) < 0)
		goto mem_err;

	while (1) {
		rc = tso_fill_packet_with_fragment(tx_queue, skb, &state);
		if (unlikely(rc)) {
			rc2 = NETDEV_TX_BUSY;
			goto unwind;
		}

		/* Move onto the next fragment? */
		if (state.in_len == 0) {
			if (++frag_i >= skb_shinfo(skb)->nr_frags)
				/* End of payload reached. */
				break;
			rc = tso_get_fragment(&state, efx,
					      skb_shinfo(skb)->frags + frag_i);
			if (rc)
				goto mem_err;
		}

		/* Start at new packet? */
		if (state.packet_space == 0 &&
		    tso_start_new_packet(tx_queue, skb, &state) < 0)
			goto mem_err;
	}

	netdev_tx_sent_queue(tx_queue->core_txq, skb->len);

	/* Pass off to hardware */
	efx_nic_push_buffers(tx_queue);

	tx_queue->tso_bursts++;
	return NETDEV_TX_OK;

 mem_err:
	netif_err(efx, tx_err, efx->net_dev,
		  "Out of memory for TSO headers, or PCI mapping error\n");
	dev_kfree_skb_any(skb);

 unwind:
	/* Free the DMA mapping we were in the process of writing out */
	if (state.unmap_len) {
		if (state.unmap_single)
			pci_unmap_single(efx->pci_dev, state.unmap_addr,
					 state.unmap_len, PCI_DMA_TODEVICE);
		else
			pci_unmap_page(efx->pci_dev, state.unmap_addr,
				       state.unmap_len, PCI_DMA_TODEVICE);
	}

	efx_enqueue_unwind(tx_queue);
	return rc2;
}


/*
 * Free up all TSO datastructures associated with tx_queue. This
 * routine should be called only once the tx_queue is both empty and
 * will no longer be used.
 */
static void efx_fini_tso(struct efx_tx_queue *tx_queue)
{
	unsigned i;

	if (tx_queue->buffer) {
		for (i = 0; i <= tx_queue->ptr_mask; ++i)
			efx_tsoh_free(tx_queue, &tx_queue->buffer[i]);
	}

	while (tx_queue->tso_headers_free != NULL)
		efx_tsoh_block_free(tx_queue, tx_queue->tso_headers_free,
				    tx_queue->efx->pci_dev);
=======
				    tx_queue->channel->channel);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
