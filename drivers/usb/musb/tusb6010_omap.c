<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * TUSB6010 USB 2.0 OTG Dual Role controller OMAP DMA interface
 *
 * Copyright (C) 2006 Nokia Corporation
 * Tony Lindgren <tony@atomide.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/usb.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <plat/dma.h>
#include <plat/mux.h>
=======
#include <linux/dmaengine.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "musb_core.h"
#include "tusb6010.h"

#define to_chdat(c)		((struct tusb_omap_dma_ch *)(c)->private_data)

#define MAX_DMAREQ		5	/* REVISIT: Really 6, but req5 not OK */

<<<<<<< HEAD
=======
struct tusb_dma_data {
	s8			dmareq;
	struct dma_chan		*chan;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct tusb_omap_dma_ch {
	struct musb		*musb;
	void __iomem		*tbase;
	unsigned long		phys_offset;
	int			epnum;
	u8			tx;
	struct musb_hw_ep	*hw_ep;

<<<<<<< HEAD
	int			ch;
	s8			dmareq;
	s8			sync_dev;
=======
	struct tusb_dma_data	*dma_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct tusb_omap_dma	*tusb_dma;

	dma_addr_t		dma_addr;

	u32			len;
	u16			packet_sz;
	u16			transfer_packet_sz;
	u32			transfer_len;
	u32			completed_len;
};

struct tusb_omap_dma {
	struct dma_controller		controller;
<<<<<<< HEAD
	struct musb			*musb;
	void __iomem			*tbase;

	int				ch;
	s8				dmareq;
	s8				sync_dev;
	unsigned			multichannel:1;
};

static int tusb_omap_dma_start(struct dma_controller *c)
{
	struct tusb_omap_dma	*tusb_dma;

	tusb_dma = container_of(c, struct tusb_omap_dma, controller);

	/* dev_dbg(musb->controller, "ep%i ch: %i\n", chdat->epnum, chdat->ch); */

	return 0;
}

static int tusb_omap_dma_stop(struct dma_controller *c)
{
	struct tusb_omap_dma	*tusb_dma;

	tusb_dma = container_of(c, struct tusb_omap_dma, controller);

	/* dev_dbg(musb->controller, "ep%i ch: %i\n", chdat->epnum, chdat->ch); */

	return 0;
}

=======
	void __iomem			*tbase;

	struct tusb_dma_data		dma_pool[MAX_DMAREQ];
	unsigned			multichannel:1;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Allocate dmareq0 to the current channel unless it's already taken
 */
static inline int tusb_omap_use_shared_dmareq(struct tusb_omap_dma_ch *chdat)
{
	u32		reg = musb_readl(chdat->tbase, TUSB_DMA_EP_MAP);

	if (reg != 0) {
		dev_dbg(chdat->musb->controller, "ep%i dmareq0 is busy for ep%i\n",
			chdat->epnum, reg & 0xf);
		return -EAGAIN;
	}

	if (chdat->tx)
		reg = (1 << 4) | chdat->epnum;
	else
		reg = chdat->epnum;

	musb_writel(chdat->tbase, TUSB_DMA_EP_MAP, reg);

	return 0;
}

static inline void tusb_omap_free_shared_dmareq(struct tusb_omap_dma_ch *chdat)
{
	u32		reg = musb_readl(chdat->tbase, TUSB_DMA_EP_MAP);

	if ((reg & 0xf) != chdat->epnum) {
		printk(KERN_ERR "ep%i trying to release dmareq0 for ep%i\n",
			chdat->epnum, reg & 0xf);
		return;
	}
	musb_writel(chdat->tbase, TUSB_DMA_EP_MAP, 0);
}

/*
 * See also musb_dma_completion in plat_uds.c and musb_g_[tx|rx]() in
 * musb_gadget.c.
 */
<<<<<<< HEAD
static void tusb_omap_dma_cb(int lch, u16 ch_status, void *data)
=======
static void tusb_omap_dma_cb(void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dma_channel	*channel = (struct dma_channel *)data;
	struct tusb_omap_dma_ch	*chdat = to_chdat(channel);
	struct tusb_omap_dma	*tusb_dma = chdat->tusb_dma;
	struct musb		*musb = chdat->musb;
	struct device		*dev = musb->controller;
	struct musb_hw_ep	*hw_ep = chdat->hw_ep;
	void __iomem		*ep_conf = hw_ep->conf;
	void __iomem		*mbase = musb->mregs;
	unsigned long		remaining, flags, pio;
<<<<<<< HEAD
	int			ch;

	spin_lock_irqsave(&musb->lock, flags);

	if (tusb_dma->multichannel)
		ch = chdat->ch;
	else
		ch = tusb_dma->ch;

	if (ch_status != OMAP_DMA_BLOCK_IRQ)
		printk(KERN_ERR "TUSB DMA error status: %i\n", ch_status);

	dev_dbg(musb->controller, "ep%i %s dma callback ch: %i status: %x\n",
		chdat->epnum, chdat->tx ? "tx" : "rx",
		ch, ch_status);
=======

	spin_lock_irqsave(&musb->lock, flags);

	dev_dbg(musb->controller, "ep%i %s dma callback\n",
		chdat->epnum, chdat->tx ? "tx" : "rx");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (chdat->tx)
		remaining = musb_readl(ep_conf, TUSB_EP_TX_OFFSET);
	else
		remaining = musb_readl(ep_conf, TUSB_EP_RX_OFFSET);

	remaining = TUSB_EP_CONFIG_XFR_SIZE(remaining);

	/* HW issue #10: XFR_SIZE may get corrupt on DMA (both async & sync) */
	if (unlikely(remaining > chdat->transfer_len)) {
<<<<<<< HEAD
		dev_dbg(musb->controller, "Corrupt %s dma ch%i XFR_SIZE: 0x%08lx\n",
			chdat->tx ? "tx" : "rx", chdat->ch,
			remaining);
=======
		dev_dbg(musb->controller, "Corrupt %s XFR_SIZE: 0x%08lx\n",
			chdat->tx ? "tx" : "rx", remaining);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		remaining = 0;
	}

	channel->actual_len = chdat->transfer_len - remaining;
	pio = chdat->len - channel->actual_len;

	dev_dbg(musb->controller, "DMA remaining %lu/%u\n", remaining, chdat->transfer_len);

	/* Transfer remaining 1 - 31 bytes */
	if (pio > 0 && pio < 32) {
		u8	*buf;

		dev_dbg(musb->controller, "Using PIO for remaining %lu bytes\n", pio);
		buf = phys_to_virt((u32)chdat->dma_addr) + chdat->transfer_len;
		if (chdat->tx) {
			dma_unmap_single(dev, chdat->dma_addr,
						chdat->transfer_len,
						DMA_TO_DEVICE);
			musb_write_fifo(hw_ep, pio, buf);
		} else {
			dma_unmap_single(dev, chdat->dma_addr,
						chdat->transfer_len,
						DMA_FROM_DEVICE);
			musb_read_fifo(hw_ep, pio, buf);
		}
		channel->actual_len += pio;
	}

	if (!tusb_dma->multichannel)
		tusb_omap_free_shared_dmareq(chdat);

	channel->status = MUSB_DMA_STATUS_FREE;

<<<<<<< HEAD
	/* Handle only RX callbacks here. TX callbacks must be handled based
	 * on the TUSB DMA status interrupt.
	 * REVISIT: Use both TUSB DMA status interrupt and OMAP DMA callback
	 * interrupt for RX and TX.
	 */
	if (!chdat->tx)
		musb_dma_completion(musb, chdat->epnum, chdat->tx);
=======
	musb_dma_completion(musb, chdat->epnum, chdat->tx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We must terminate short tx transfers manually by setting TXPKTRDY.
	 * REVISIT: This same problem may occur with other MUSB dma as well.
	 * Easy to test with g_ether by pinging the MUSB board with ping -s54.
	 */
	if ((chdat->transfer_len < chdat->packet_sz)
			|| (chdat->transfer_len % chdat->packet_sz != 0)) {
		u16	csr;

		if (chdat->tx) {
			dev_dbg(musb->controller, "terminating short tx packet\n");
			musb_ep_select(mbase, chdat->epnum);
			csr = musb_readw(hw_ep->regs, MUSB_TXCSR);
			csr |= MUSB_TXCSR_MODE | MUSB_TXCSR_TXPKTRDY
				| MUSB_TXCSR_P_WZC_BITS;
			musb_writew(hw_ep->regs, MUSB_TXCSR, csr);
		}
	}

	spin_unlock_irqrestore(&musb->lock, flags);
}

static int tusb_omap_dma_program(struct dma_channel *channel, u16 packet_sz,
				u8 rndis_mode, dma_addr_t dma_addr, u32 len)
{
	struct tusb_omap_dma_ch		*chdat = to_chdat(channel);
	struct tusb_omap_dma		*tusb_dma = chdat->tusb_dma;
	struct musb			*musb = chdat->musb;
	struct device			*dev = musb->controller;
	struct musb_hw_ep		*hw_ep = chdat->hw_ep;
	void __iomem			*mbase = musb->mregs;
	void __iomem			*ep_conf = hw_ep->conf;
<<<<<<< HEAD
	dma_addr_t			fifo = hw_ep->fifo_sync;
	struct omap_dma_channel_params	dma_params;
	u32				dma_remaining;
	int				src_burst, dst_burst;
	u16				csr;
	int				ch;
	s8				dmareq;
	s8				sync_dev;
=======
	dma_addr_t			fifo_addr = hw_ep->fifo_sync;
	u32				dma_remaining;
	u16				csr;
	u32				psize;
	struct tusb_dma_data		*dma_data;
	struct dma_async_tx_descriptor	*dma_desc;
	struct dma_slave_config		dma_cfg;
	enum dma_transfer_direction	dma_dir;
	u32				port_window;
	int				ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(dma_addr & 0x1) || (len < 32) || (len > packet_sz))
		return false;

	/*
	 * HW issue #10: Async dma will eventually corrupt the XFR_SIZE
	 * register which will cause missed DMA interrupt. We could try to
	 * use a timer for the callback, but it is unsafe as the XFR_SIZE
	 * register is corrupt, and we won't know if the DMA worked.
	 */
	if (dma_addr & 0x2)
		return false;

	/*
	 * Because of HW issue #10, it seems like mixing sync DMA and async
	 * PIO access can confuse the DMA. Make sure XFR_SIZE is reset before
	 * using the channel for DMA.
	 */
	if (chdat->tx)
		dma_remaining = musb_readl(ep_conf, TUSB_EP_TX_OFFSET);
	else
		dma_remaining = musb_readl(ep_conf, TUSB_EP_RX_OFFSET);

	dma_remaining = TUSB_EP_CONFIG_XFR_SIZE(dma_remaining);
	if (dma_remaining) {
<<<<<<< HEAD
		dev_dbg(musb->controller, "Busy %s dma ch%i, not using: %08x\n",
			chdat->tx ? "tx" : "rx", chdat->ch,
			dma_remaining);
=======
		dev_dbg(musb->controller, "Busy %s dma, not using: %08x\n",
			chdat->tx ? "tx" : "rx", dma_remaining);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return false;
	}

	chdat->transfer_len = len & ~0x1f;

	if (len < packet_sz)
		chdat->transfer_packet_sz = chdat->transfer_len;
	else
		chdat->transfer_packet_sz = packet_sz;

<<<<<<< HEAD
	if (tusb_dma->multichannel) {
		ch = chdat->ch;
		dmareq = chdat->dmareq;
		sync_dev = chdat->sync_dev;
	} else {
=======
	dma_data = chdat->dma_data;
	if (!tusb_dma->multichannel) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (tusb_omap_use_shared_dmareq(chdat) != 0) {
			dev_dbg(musb->controller, "could not get dma for ep%i\n", chdat->epnum);
			return false;
		}
<<<<<<< HEAD
		if (tusb_dma->ch < 0) {
=======
		if (dma_data->dmareq < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* REVISIT: This should get blocked earlier, happens
			 * with MSC ErrorRecoveryTest
			 */
			WARN_ON(1);
			return false;
		}
<<<<<<< HEAD

		ch = tusb_dma->ch;
		dmareq = tusb_dma->dmareq;
		sync_dev = tusb_dma->sync_dev;
		omap_set_dma_callback(ch, tusb_omap_dma_cb, channel);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	chdat->packet_sz = packet_sz;
	chdat->len = len;
	channel->actual_len = 0;
	chdat->dma_addr = dma_addr;
	channel->status = MUSB_DMA_STATUS_BUSY;

	/* Since we're recycling dma areas, we need to clean or invalidate */
<<<<<<< HEAD
	if (chdat->tx)
		dma_map_single(dev, phys_to_virt(dma_addr), len,
				DMA_TO_DEVICE);
	else
		dma_map_single(dev, phys_to_virt(dma_addr), len,
				DMA_FROM_DEVICE);

	/* Use 16-bit transfer if dma_addr is not 32-bit aligned */
	if ((dma_addr & 0x3) == 0) {
		dma_params.data_type = OMAP_DMA_DATA_TYPE_S32;
		dma_params.elem_count = 8;		/* Elements in frame */
	} else {
		dma_params.data_type = OMAP_DMA_DATA_TYPE_S16;
		dma_params.elem_count = 16;		/* Elements in frame */
		fifo = hw_ep->fifo_async;
	}

	dma_params.frame_count	= chdat->transfer_len / 32; /* Burst sz frame */

	dev_dbg(musb->controller, "ep%i %s dma ch%i dma: %08x len: %u(%u) packet_sz: %i(%i)\n",
		chdat->epnum, chdat->tx ? "tx" : "rx",
		ch, dma_addr, chdat->transfer_len, len,
		chdat->transfer_packet_sz, packet_sz);

	/*
	 * Prepare omap DMA for transfer
	 */
	if (chdat->tx) {
		dma_params.src_amode	= OMAP_DMA_AMODE_POST_INC;
		dma_params.src_start	= (unsigned long)dma_addr;
		dma_params.src_ei	= 0;
		dma_params.src_fi	= 0;

		dma_params.dst_amode	= OMAP_DMA_AMODE_DOUBLE_IDX;
		dma_params.dst_start	= (unsigned long)fifo;
		dma_params.dst_ei	= 1;
		dma_params.dst_fi	= -31;	/* Loop 32 byte window */

		dma_params.trigger	= sync_dev;
		dma_params.sync_mode	= OMAP_DMA_SYNC_FRAME;
		dma_params.src_or_dst_synch	= 0;	/* Dest sync */

		src_burst = OMAP_DMA_DATA_BURST_16;	/* 16x32 read */
		dst_burst = OMAP_DMA_DATA_BURST_8;	/* 8x32 write */
	} else {
		dma_params.src_amode	= OMAP_DMA_AMODE_DOUBLE_IDX;
		dma_params.src_start	= (unsigned long)fifo;
		dma_params.src_ei	= 1;
		dma_params.src_fi	= -31;	/* Loop 32 byte window */

		dma_params.dst_amode	= OMAP_DMA_AMODE_POST_INC;
		dma_params.dst_start	= (unsigned long)dma_addr;
		dma_params.dst_ei	= 0;
		dma_params.dst_fi	= 0;

		dma_params.trigger	= sync_dev;
		dma_params.sync_mode	= OMAP_DMA_SYNC_FRAME;
		dma_params.src_or_dst_synch	= 1;	/* Source sync */

		src_burst = OMAP_DMA_DATA_BURST_8;	/* 8x32 read */
		dst_burst = OMAP_DMA_DATA_BURST_16;	/* 16x32 write */
	}

	dev_dbg(musb->controller, "ep%i %s using %i-bit %s dma from 0x%08lx to 0x%08lx\n",
		chdat->epnum, chdat->tx ? "tx" : "rx",
		(dma_params.data_type == OMAP_DMA_DATA_TYPE_S32) ? 32 : 16,
		((dma_addr & 0x3) == 0) ? "sync" : "async",
		dma_params.src_start, dma_params.dst_start);

	omap_set_dma_params(ch, &dma_params);
	omap_set_dma_src_burst_mode(ch, src_burst);
	omap_set_dma_dest_burst_mode(ch, dst_burst);
	omap_set_dma_write_mode(ch, OMAP_DMA_WRITE_LAST_NON_POSTED);
=======
	if (chdat->tx) {
		dma_dir = DMA_MEM_TO_DEV;
		dma_map_single(dev, phys_to_virt(dma_addr), len,
				DMA_TO_DEVICE);
	} else {
		dma_dir = DMA_DEV_TO_MEM;
		dma_map_single(dev, phys_to_virt(dma_addr), len,
				DMA_FROM_DEVICE);
	}

	memset(&dma_cfg, 0, sizeof(dma_cfg));

	/* Use 16-bit transfer if dma_addr is not 32-bit aligned */
	if ((dma_addr & 0x3) == 0) {
		dma_cfg.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
		dma_cfg.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
		port_window = 8;
	} else {
		dma_cfg.src_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
		dma_cfg.dst_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
		port_window = 16;

		fifo_addr = hw_ep->fifo_async;
	}

	dev_dbg(musb->controller,
		"ep%i %s dma: %pad len: %u(%u) packet_sz: %i(%i)\n",
		chdat->epnum, chdat->tx ? "tx" : "rx", &dma_addr,
		chdat->transfer_len, len, chdat->transfer_packet_sz, packet_sz);

	dma_cfg.src_addr = fifo_addr;
	dma_cfg.dst_addr = fifo_addr;
	dma_cfg.src_port_window_size = port_window;
	dma_cfg.src_maxburst = port_window;
	dma_cfg.dst_port_window_size = port_window;
	dma_cfg.dst_maxburst = port_window;

	ret = dmaengine_slave_config(dma_data->chan, &dma_cfg);
	if (ret) {
		dev_err(musb->controller, "DMA slave config failed: %d\n", ret);
		return false;
	}

	dma_desc = dmaengine_prep_slave_single(dma_data->chan, dma_addr,
					chdat->transfer_len, dma_dir,
					DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!dma_desc) {
		dev_err(musb->controller, "DMA prep_slave_single failed\n");
		return false;
	}

	dma_desc->callback = tusb_omap_dma_cb;
	dma_desc->callback_param = channel;
	dmaengine_submit(dma_desc);

	dev_dbg(musb->controller,
		"ep%i %s using %i-bit %s dma from %pad to %pad\n",
		chdat->epnum, chdat->tx ? "tx" : "rx",
		dma_cfg.src_addr_width * 8,
		((dma_addr & 0x3) == 0) ? "sync" : "async",
		(dma_dir == DMA_MEM_TO_DEV) ? &dma_addr : &fifo_addr,
		(dma_dir == DMA_MEM_TO_DEV) ? &fifo_addr : &dma_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Prepare MUSB for DMA transfer
	 */
<<<<<<< HEAD
	if (chdat->tx) {
		musb_ep_select(mbase, chdat->epnum);
=======
	musb_ep_select(mbase, chdat->epnum);
	if (chdat->tx) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		csr = musb_readw(hw_ep->regs, MUSB_TXCSR);
		csr |= (MUSB_TXCSR_AUTOSET | MUSB_TXCSR_DMAENAB
			| MUSB_TXCSR_DMAMODE | MUSB_TXCSR_MODE);
		csr &= ~MUSB_TXCSR_P_UNDERRUN;
		musb_writew(hw_ep->regs, MUSB_TXCSR, csr);
	} else {
<<<<<<< HEAD
		musb_ep_select(mbase, chdat->epnum);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		csr = musb_readw(hw_ep->regs, MUSB_RXCSR);
		csr |= MUSB_RXCSR_DMAENAB;
		csr &= ~(MUSB_RXCSR_AUTOCLEAR | MUSB_RXCSR_DMAMODE);
		musb_writew(hw_ep->regs, MUSB_RXCSR,
			csr | MUSB_RXCSR_P_WZC_BITS);
	}

<<<<<<< HEAD
	/*
	 * Start DMA transfer
	 */
	omap_start_dma(ch);

	if (chdat->tx) {
		/* Send transfer_packet_sz packets at a time */
		musb_writel(ep_conf, TUSB_EP_MAX_PACKET_SIZE_OFFSET,
			chdat->transfer_packet_sz);
=======
	/* Start DMA transfer */
	dma_async_issue_pending(dma_data->chan);

	if (chdat->tx) {
		/* Send transfer_packet_sz packets at a time */
		psize = musb_readl(ep_conf, TUSB_EP_MAX_PACKET_SIZE_OFFSET);
		psize &= ~0x7ff;
		psize |= chdat->transfer_packet_sz;
		musb_writel(ep_conf, TUSB_EP_MAX_PACKET_SIZE_OFFSET, psize);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		musb_writel(ep_conf, TUSB_EP_TX_OFFSET,
			TUSB_EP_CONFIG_XFR_SIZE(chdat->transfer_len));
	} else {
		/* Receive transfer_packet_sz packets at a time */
<<<<<<< HEAD
		musb_writel(ep_conf, TUSB_EP_MAX_PACKET_SIZE_OFFSET,
			chdat->transfer_packet_sz << 16);
=======
		psize = musb_readl(ep_conf, TUSB_EP_MAX_PACKET_SIZE_OFFSET);
		psize &= ~(0x7ff << 16);
		psize |= (chdat->transfer_packet_sz << 16);
		musb_writel(ep_conf, TUSB_EP_MAX_PACKET_SIZE_OFFSET, psize);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		musb_writel(ep_conf, TUSB_EP_RX_OFFSET,
			TUSB_EP_CONFIG_XFR_SIZE(chdat->transfer_len));
	}

	return true;
}

static int tusb_omap_dma_abort(struct dma_channel *channel)
{
	struct tusb_omap_dma_ch	*chdat = to_chdat(channel);
<<<<<<< HEAD
	struct tusb_omap_dma	*tusb_dma = chdat->tusb_dma;

	if (!tusb_dma->multichannel) {
		if (tusb_dma->ch >= 0) {
			omap_stop_dma(tusb_dma->ch);
			omap_free_dma(tusb_dma->ch);
			tusb_dma->ch = -1;
		}

		tusb_dma->dmareq = -1;
		tusb_dma->sync_dev = -1;
	}
=======

	if (chdat->dma_data)
		dmaengine_terminate_all(chdat->dma_data->chan);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	channel->status = MUSB_DMA_STATUS_FREE;

	return 0;
}

static inline int tusb_omap_dma_allocate_dmareq(struct tusb_omap_dma_ch *chdat)
{
	u32		reg = musb_readl(chdat->tbase, TUSB_DMA_EP_MAP);
	int		i, dmareq_nr = -1;

<<<<<<< HEAD
	const int sync_dev[6] = {
		OMAP24XX_DMA_EXT_DMAREQ0,
		OMAP24XX_DMA_EXT_DMAREQ1,
		OMAP242X_DMA_EXT_DMAREQ2,
		OMAP242X_DMA_EXT_DMAREQ3,
		OMAP242X_DMA_EXT_DMAREQ4,
		OMAP242X_DMA_EXT_DMAREQ5,
	};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < MAX_DMAREQ; i++) {
		int cur = (reg & (0xf << (i * 5))) >> (i * 5);
		if (cur == 0) {
			dmareq_nr = i;
			break;
		}
	}

	if (dmareq_nr == -1)
		return -EAGAIN;

	reg |= (chdat->epnum << (dmareq_nr * 5));
	if (chdat->tx)
		reg |= ((1 << 4) << (dmareq_nr * 5));
	musb_writel(chdat->tbase, TUSB_DMA_EP_MAP, reg);

<<<<<<< HEAD
	chdat->dmareq = dmareq_nr;
	chdat->sync_dev = sync_dev[chdat->dmareq];
=======
	chdat->dma_data = &chdat->tusb_dma->dma_pool[dmareq_nr];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static inline void tusb_omap_dma_free_dmareq(struct tusb_omap_dma_ch *chdat)
{
	u32 reg;

<<<<<<< HEAD
	if (!chdat || chdat->dmareq < 0)
		return;

	reg = musb_readl(chdat->tbase, TUSB_DMA_EP_MAP);
	reg &= ~(0x1f << (chdat->dmareq * 5));
	musb_writel(chdat->tbase, TUSB_DMA_EP_MAP, reg);

	chdat->dmareq = -1;
	chdat->sync_dev = -1;
=======
	if (!chdat || !chdat->dma_data || chdat->dma_data->dmareq < 0)
		return;

	reg = musb_readl(chdat->tbase, TUSB_DMA_EP_MAP);
	reg &= ~(0x1f << (chdat->dma_data->dmareq * 5));
	musb_writel(chdat->tbase, TUSB_DMA_EP_MAP, reg);

	chdat->dma_data = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct dma_channel *dma_channel_pool[MAX_DMAREQ];

static struct dma_channel *
tusb_omap_dma_allocate(struct dma_controller *c,
		struct musb_hw_ep *hw_ep,
		u8 tx)
{
	int ret, i;
<<<<<<< HEAD
	const char		*dev_name;
	struct tusb_omap_dma	*tusb_dma;
	struct musb		*musb;
	void __iomem		*tbase;
	struct dma_channel	*channel = NULL;
	struct tusb_omap_dma_ch	*chdat = NULL;
	u32			reg;

	tusb_dma = container_of(c, struct tusb_omap_dma, controller);
	musb = tusb_dma->musb;
	tbase = musb->ctrl_base;

	reg = musb_readl(tbase, TUSB_DMA_INT_MASK);
	if (tx)
		reg &= ~(1 << hw_ep->epnum);
	else
		reg &= ~(1 << (hw_ep->epnum + 15));
	musb_writel(tbase, TUSB_DMA_INT_MASK, reg);
=======
	struct tusb_omap_dma	*tusb_dma;
	struct musb		*musb;
	struct dma_channel	*channel = NULL;
	struct tusb_omap_dma_ch	*chdat = NULL;
	struct tusb_dma_data	*dma_data = NULL;

	tusb_dma = container_of(c, struct tusb_omap_dma, controller);
	musb = tusb_dma->controller.musb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* REVISIT: Why does dmareq5 not work? */
	if (hw_ep->epnum == 0) {
		dev_dbg(musb->controller, "Not allowing DMA for ep0 %s\n", tx ? "tx" : "rx");
		return NULL;
	}

	for (i = 0; i < MAX_DMAREQ; i++) {
		struct dma_channel *ch = dma_channel_pool[i];
		if (ch->status == MUSB_DMA_STATUS_UNKNOWN) {
			ch->status = MUSB_DMA_STATUS_FREE;
			channel = ch;
			chdat = ch->private_data;
			break;
		}
	}

	if (!channel)
		return NULL;

<<<<<<< HEAD
	if (tx) {
		chdat->tx = 1;
		dev_name = "TUSB transmit";
	} else {
		chdat->tx = 0;
		dev_name = "TUSB receive";
	}

	chdat->musb = tusb_dma->musb;
	chdat->tbase = tusb_dma->tbase;
	chdat->hw_ep = hw_ep;
	chdat->epnum = hw_ep->epnum;
	chdat->dmareq = -1;
	chdat->completed_len = 0;
	chdat->tusb_dma = tusb_dma;
=======
	chdat->musb = tusb_dma->controller.musb;
	chdat->tbase = tusb_dma->tbase;
	chdat->hw_ep = hw_ep;
	chdat->epnum = hw_ep->epnum;
	chdat->completed_len = 0;
	chdat->tusb_dma = tusb_dma;
	if (tx)
		chdat->tx = 1;
	else
		chdat->tx = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	channel->max_len = 0x7fffffff;
	channel->desired_mode = 0;
	channel->actual_len = 0;

<<<<<<< HEAD
	if (tusb_dma->multichannel) {
		ret = tusb_omap_dma_allocate_dmareq(chdat);
		if (ret != 0)
			goto free_dmareq;

		ret = omap_request_dma(chdat->sync_dev, dev_name,
				tusb_omap_dma_cb, channel, &chdat->ch);
		if (ret != 0)
			goto free_dmareq;
	} else if (tusb_dma->ch == -1) {
		tusb_dma->dmareq = 0;
		tusb_dma->sync_dev = OMAP24XX_DMA_EXT_DMAREQ0;

		/* Callback data gets set later in the shared dmareq case */
		ret = omap_request_dma(tusb_dma->sync_dev, "TUSB shared",
				tusb_omap_dma_cb, NULL, &tusb_dma->ch);
		if (ret != 0)
			goto free_dmareq;

		chdat->dmareq = -1;
		chdat->ch = -1;
	}

	dev_dbg(musb->controller, "ep%i %s dma: %s dma%i dmareq%i sync%i\n",
		chdat->epnum,
		chdat->tx ? "tx" : "rx",
		chdat->ch >= 0 ? "dedicated" : "shared",
		chdat->ch >= 0 ? chdat->ch : tusb_dma->ch,
		chdat->dmareq >= 0 ? chdat->dmareq : tusb_dma->dmareq,
		chdat->sync_dev >= 0 ? chdat->sync_dev : tusb_dma->sync_dev);
=======
	if (!chdat->dma_data) {
		if (tusb_dma->multichannel) {
			ret = tusb_omap_dma_allocate_dmareq(chdat);
			if (ret != 0)
				goto free_dmareq;
		} else {
			chdat->dma_data = &tusb_dma->dma_pool[0];
		}
	}

	dma_data = chdat->dma_data;

	dev_dbg(musb->controller, "ep%i %s dma: %s dmareq%i\n",
		chdat->epnum,
		chdat->tx ? "tx" : "rx",
		tusb_dma->multichannel ? "shared" : "dedicated",
		dma_data->dmareq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return channel;

free_dmareq:
	tusb_omap_dma_free_dmareq(chdat);

	dev_dbg(musb->controller, "ep%i: Could not get a DMA channel\n", chdat->epnum);
	channel->status = MUSB_DMA_STATUS_UNKNOWN;

	return NULL;
}

static void tusb_omap_dma_release(struct dma_channel *channel)
{
	struct tusb_omap_dma_ch	*chdat = to_chdat(channel);
	struct musb		*musb = chdat->musb;
<<<<<<< HEAD
	void __iomem		*tbase = musb->ctrl_base;
	u32			reg;

	dev_dbg(musb->controller, "ep%i ch%i\n", chdat->epnum, chdat->ch);

	reg = musb_readl(tbase, TUSB_DMA_INT_MASK);
	if (chdat->tx)
		reg |= (1 << chdat->epnum);
	else
		reg |= (1 << (chdat->epnum + 15));
	musb_writel(tbase, TUSB_DMA_INT_MASK, reg);

	reg = musb_readl(tbase, TUSB_DMA_INT_CLEAR);
	if (chdat->tx)
		reg |= (1 << chdat->epnum);
	else
		reg |= (1 << (chdat->epnum + 15));
	musb_writel(tbase, TUSB_DMA_INT_CLEAR, reg);

	channel->status = MUSB_DMA_STATUS_UNKNOWN;

	if (chdat->ch >= 0) {
		omap_stop_dma(chdat->ch);
		omap_free_dma(chdat->ch);
		chdat->ch = -1;
	}

	if (chdat->dmareq >= 0)
		tusb_omap_dma_free_dmareq(chdat);
=======

	dev_dbg(musb->controller, "Release for ep%i\n", chdat->epnum);

	channel->status = MUSB_DMA_STATUS_UNKNOWN;

	dmaengine_terminate_sync(chdat->dma_data->chan);
	tusb_omap_dma_free_dmareq(chdat);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	channel = NULL;
}

<<<<<<< HEAD
void dma_controller_destroy(struct dma_controller *c)
=======
void tusb_dma_controller_destroy(struct dma_controller *c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tusb_omap_dma	*tusb_dma;
	int			i;

	tusb_dma = container_of(c, struct tusb_omap_dma, controller);
	for (i = 0; i < MAX_DMAREQ; i++) {
		struct dma_channel *ch = dma_channel_pool[i];
		if (ch) {
			kfree(ch->private_data);
			kfree(ch);
		}
<<<<<<< HEAD
	}

	if (tusb_dma && !tusb_dma->multichannel && tusb_dma->ch >= 0)
		omap_free_dma(tusb_dma->ch);

	kfree(tusb_dma);
}

struct dma_controller *__init
dma_controller_create(struct musb *musb, void __iomem *base)
=======

		/* Free up the DMA channels */
		if (tusb_dma && tusb_dma->dma_pool[i].chan)
			dma_release_channel(tusb_dma->dma_pool[i].chan);
	}

	kfree(tusb_dma);
}
EXPORT_SYMBOL_GPL(tusb_dma_controller_destroy);

static int tusb_omap_allocate_dma_pool(struct tusb_omap_dma *tusb_dma)
{
	struct musb *musb = tusb_dma->controller.musb;
	int i;
	int ret = 0;

	for (i = 0; i < MAX_DMAREQ; i++) {
		struct tusb_dma_data *dma_data = &tusb_dma->dma_pool[i];

		/*
		 * Request DMA channels:
		 * - one channel in case of non multichannel mode
		 * - MAX_DMAREQ number of channels in multichannel mode
		 */
		if (i == 0 || tusb_dma->multichannel) {
			char ch_name[8];

			sprintf(ch_name, "dmareq%d", i);
			dma_data->chan = dma_request_chan(musb->controller,
							  ch_name);
			if (IS_ERR(dma_data->chan)) {
				dev_err(musb->controller,
					"Failed to request %s\n", ch_name);
				ret = PTR_ERR(dma_data->chan);
				goto dma_error;
			}

			dma_data->dmareq = i;
		} else {
			dma_data->dmareq = -1;
		}
	}

	return 0;

dma_error:
	for (; i >= 0; i--) {
		struct tusb_dma_data *dma_data = &tusb_dma->dma_pool[i];

		if (dma_data->dmareq >= 0)
			dma_release_channel(dma_data->chan);
	}

	return ret;
}

struct dma_controller *
tusb_dma_controller_create(struct musb *musb, void __iomem *base)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	void __iomem		*tbase = musb->ctrl_base;
	struct tusb_omap_dma	*tusb_dma;
	int			i;

	/* REVISIT: Get dmareq lines used from board-*.c */

	musb_writel(musb->ctrl_base, TUSB_DMA_INT_MASK, 0x7fffffff);
	musb_writel(musb->ctrl_base, TUSB_DMA_EP_MAP, 0);

	musb_writel(tbase, TUSB_DMA_REQ_CONF,
		TUSB_DMA_REQ_CONF_BURST_SIZE(2)
		| TUSB_DMA_REQ_CONF_DMA_REQ_EN(0x3f)
		| TUSB_DMA_REQ_CONF_DMA_REQ_ASSER(2));

	tusb_dma = kzalloc(sizeof(struct tusb_omap_dma), GFP_KERNEL);
	if (!tusb_dma)
		goto out;

<<<<<<< HEAD
	tusb_dma->musb = musb;
	tusb_dma->tbase = musb->ctrl_base;

	tusb_dma->ch = -1;
	tusb_dma->dmareq = -1;
	tusb_dma->sync_dev = -1;

	tusb_dma->controller.start = tusb_omap_dma_start;
	tusb_dma->controller.stop = tusb_omap_dma_stop;
=======
	tusb_dma->controller.musb = musb;
	tusb_dma->tbase = musb->ctrl_base;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tusb_dma->controller.channel_alloc = tusb_omap_dma_allocate;
	tusb_dma->controller.channel_release = tusb_omap_dma_release;
	tusb_dma->controller.channel_program = tusb_omap_dma_program;
	tusb_dma->controller.channel_abort = tusb_omap_dma_abort;

<<<<<<< HEAD
	if (tusb_get_revision(musb) >= TUSB_REV_30)
=======
	if (musb->tusb_revision >= TUSB_REV_30)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tusb_dma->multichannel = 1;

	for (i = 0; i < MAX_DMAREQ; i++) {
		struct dma_channel	*ch;
		struct tusb_omap_dma_ch	*chdat;

		ch = kzalloc(sizeof(struct dma_channel), GFP_KERNEL);
		if (!ch)
			goto cleanup;

		dma_channel_pool[i] = ch;

		chdat = kzalloc(sizeof(struct tusb_omap_dma_ch), GFP_KERNEL);
		if (!chdat)
			goto cleanup;

		ch->status = MUSB_DMA_STATUS_UNKNOWN;
		ch->private_data = chdat;
	}

<<<<<<< HEAD
	return &tusb_dma->controller;

cleanup:
	dma_controller_destroy(&tusb_dma->controller);
out:
	return NULL;
}
=======
	if (tusb_omap_allocate_dma_pool(tusb_dma))
		goto cleanup;

	return &tusb_dma->controller;

cleanup:
	musb_dma_controller_destroy(&tusb_dma->controller);
out:
	return NULL;
}
EXPORT_SYMBOL_GPL(tusb_dma_controller_create);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
