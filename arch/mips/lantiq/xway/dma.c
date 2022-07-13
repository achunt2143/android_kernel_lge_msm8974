<<<<<<< HEAD
/*
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License version 2 as published
 *   by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Copyright (C) 2011 John Crispin <blogic@openwrt.org>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *
 *   Copyright (C) 2011 John Crispin <john@phrozen.org>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/export.h>
<<<<<<< HEAD
=======
#include <linux/spinlock.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <lantiq_soc.h>
#include <xway_dma.h>

<<<<<<< HEAD
=======
#define LTQ_DMA_ID		0x08
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LTQ_DMA_CTRL		0x10
#define LTQ_DMA_CPOLL		0x14
#define LTQ_DMA_CS		0x18
#define LTQ_DMA_CCTRL		0x1C
#define LTQ_DMA_CDBA		0x20
#define LTQ_DMA_CDLEN		0x24
#define LTQ_DMA_CIS		0x28
#define LTQ_DMA_CIE		0x2C
#define LTQ_DMA_PS		0x40
#define LTQ_DMA_PCTRL		0x44
#define LTQ_DMA_IRNEN		0xf4

<<<<<<< HEAD
=======
#define DMA_ID_CHNR		GENMASK(26, 20)	/* channel number */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DMA_DESCPT		BIT(3)		/* descriptor complete irq */
#define DMA_TX			BIT(8)		/* TX channel direction */
#define DMA_CHAN_ON		BIT(0)		/* channel on / off bit */
#define DMA_PDEN		BIT(6)		/* enable packet drop */
#define DMA_CHAN_RST		BIT(1)		/* channel on / off bit */
#define DMA_RESET		BIT(0)		/* channel on / off bit */
#define DMA_IRQ_ACK		0x7e		/* IRQ status register */
#define DMA_POLL		BIT(31)		/* turn on channel polling */
#define DMA_CLK_DIV4		BIT(6)		/* polling clock divider */
<<<<<<< HEAD
#define DMA_2W_BURST		BIT(1)		/* 2 word burst length */
#define DMA_MAX_CHANNEL		20		/* the soc has 20 channels */
#define DMA_ETOP_ENDIANESS	(0xf << 8) /* endianess swap etop channels */
=======
#define DMA_PCTRL_2W_BURST	0x1		/* 2 word burst length */
#define DMA_PCTRL_4W_BURST	0x2		/* 4 word burst length */
#define DMA_PCTRL_8W_BURST	0x3		/* 8 word burst length */
#define DMA_TX_BURST_SHIFT	4		/* tx burst shift */
#define DMA_RX_BURST_SHIFT	2		/* rx burst shift */
#define DMA_ETOP_ENDIANNESS	(0xf << 8) /* endianness swap etop channels */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DMA_WEIGHT	(BIT(17) | BIT(16))	/* default channel wheight */

#define ltq_dma_r32(x)			ltq_r32(ltq_dma_membase + (x))
#define ltq_dma_w32(x, y)		ltq_w32(x, ltq_dma_membase + (y))
#define ltq_dma_w32_mask(x, y, z)	ltq_w32_mask(x, y, \
						ltq_dma_membase + (z))

<<<<<<< HEAD
static struct resource ltq_dma_resource = {
	.name	= "dma",
	.start	= LTQ_DMA_BASE_ADDR,
	.end	= LTQ_DMA_BASE_ADDR + LTQ_DMA_SIZE - 1,
	.flags  = IORESOURCE_MEM,
};

static void __iomem *ltq_dma_membase;
=======
static void __iomem *ltq_dma_membase;
static DEFINE_SPINLOCK(ltq_dma_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void
ltq_dma_enable_irq(struct ltq_dma_channel *ch)
{
	unsigned long flags;

<<<<<<< HEAD
	local_irq_save(flags);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32_mask(0, 1 << ch->nr, LTQ_DMA_IRNEN);
	local_irq_restore(flags);
=======
	spin_lock_irqsave(&ltq_dma_lock, flags);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32_mask(0, 1 << ch->nr, LTQ_DMA_IRNEN);
	spin_unlock_irqrestore(&ltq_dma_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(ltq_dma_enable_irq);

void
ltq_dma_disable_irq(struct ltq_dma_channel *ch)
{
	unsigned long flags;

<<<<<<< HEAD
	local_irq_save(flags);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32_mask(1 << ch->nr, 0, LTQ_DMA_IRNEN);
	local_irq_restore(flags);
=======
	spin_lock_irqsave(&ltq_dma_lock, flags);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32_mask(1 << ch->nr, 0, LTQ_DMA_IRNEN);
	spin_unlock_irqrestore(&ltq_dma_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(ltq_dma_disable_irq);

void
ltq_dma_ack_irq(struct ltq_dma_channel *ch)
{
	unsigned long flags;

<<<<<<< HEAD
	local_irq_save(flags);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32(DMA_IRQ_ACK, LTQ_DMA_CIS);
	local_irq_restore(flags);
=======
	spin_lock_irqsave(&ltq_dma_lock, flags);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32(DMA_IRQ_ACK, LTQ_DMA_CIS);
	spin_unlock_irqrestore(&ltq_dma_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(ltq_dma_ack_irq);

void
ltq_dma_open(struct ltq_dma_channel *ch)
{
	unsigned long flag;

<<<<<<< HEAD
	local_irq_save(flag);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32_mask(0, DMA_CHAN_ON, LTQ_DMA_CCTRL);
	ltq_dma_enable_irq(ch);
	local_irq_restore(flag);
=======
	spin_lock_irqsave(&ltq_dma_lock, flag);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32_mask(0, DMA_CHAN_ON, LTQ_DMA_CCTRL);
	spin_unlock_irqrestore(&ltq_dma_lock, flag);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(ltq_dma_open);

void
ltq_dma_close(struct ltq_dma_channel *ch)
{
	unsigned long flag;

<<<<<<< HEAD
	local_irq_save(flag);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32_mask(DMA_CHAN_ON, 0, LTQ_DMA_CCTRL);
	ltq_dma_disable_irq(ch);
	local_irq_restore(flag);
=======
	spin_lock_irqsave(&ltq_dma_lock, flag);
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32_mask(DMA_CHAN_ON, 0, LTQ_DMA_CCTRL);
	ltq_dma_w32_mask(1 << ch->nr, 0, LTQ_DMA_IRNEN);
	spin_unlock_irqrestore(&ltq_dma_lock, flag);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(ltq_dma_close);

static void
ltq_dma_alloc(struct ltq_dma_channel *ch)
{
	unsigned long flags;

	ch->desc = 0;
<<<<<<< HEAD
	ch->desc_base = dma_alloc_coherent(NULL,
				LTQ_DESC_NUM * LTQ_DESC_SIZE,
				&ch->phys, GFP_ATOMIC);
	memset(ch->desc_base, 0, LTQ_DESC_NUM * LTQ_DESC_SIZE);

	local_irq_save(flags);
=======
	ch->desc_base = dma_alloc_coherent(ch->dev,
					   LTQ_DESC_NUM * LTQ_DESC_SIZE,
					   &ch->phys, GFP_ATOMIC);

	spin_lock_irqsave(&ltq_dma_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ltq_dma_w32(ch->nr, LTQ_DMA_CS);
	ltq_dma_w32(ch->phys, LTQ_DMA_CDBA);
	ltq_dma_w32(LTQ_DESC_NUM, LTQ_DMA_CDLEN);
	ltq_dma_w32_mask(DMA_CHAN_ON, 0, LTQ_DMA_CCTRL);
	wmb();
	ltq_dma_w32_mask(0, DMA_CHAN_RST, LTQ_DMA_CCTRL);
	while (ltq_dma_r32(LTQ_DMA_CCTRL) & DMA_CHAN_RST)
		;
<<<<<<< HEAD
	local_irq_restore(flags);
=======
	spin_unlock_irqrestore(&ltq_dma_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void
ltq_dma_alloc_tx(struct ltq_dma_channel *ch)
{
	unsigned long flags;

	ltq_dma_alloc(ch);

<<<<<<< HEAD
	local_irq_save(flags);
	ltq_dma_w32(DMA_DESCPT, LTQ_DMA_CIE);
	ltq_dma_w32_mask(0, 1 << ch->nr, LTQ_DMA_IRNEN);
	ltq_dma_w32(DMA_WEIGHT | DMA_TX, LTQ_DMA_CCTRL);
	local_irq_restore(flags);
=======
	spin_lock_irqsave(&ltq_dma_lock, flags);
	ltq_dma_w32(DMA_DESCPT, LTQ_DMA_CIE);
	ltq_dma_w32_mask(0, 1 << ch->nr, LTQ_DMA_IRNEN);
	ltq_dma_w32(DMA_WEIGHT | DMA_TX, LTQ_DMA_CCTRL);
	spin_unlock_irqrestore(&ltq_dma_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(ltq_dma_alloc_tx);

void
ltq_dma_alloc_rx(struct ltq_dma_channel *ch)
{
	unsigned long flags;

	ltq_dma_alloc(ch);

<<<<<<< HEAD
	local_irq_save(flags);
	ltq_dma_w32(DMA_DESCPT, LTQ_DMA_CIE);
	ltq_dma_w32_mask(0, 1 << ch->nr, LTQ_DMA_IRNEN);
	ltq_dma_w32(DMA_WEIGHT, LTQ_DMA_CCTRL);
	local_irq_restore(flags);
=======
	spin_lock_irqsave(&ltq_dma_lock, flags);
	ltq_dma_w32(DMA_DESCPT, LTQ_DMA_CIE);
	ltq_dma_w32_mask(0, 1 << ch->nr, LTQ_DMA_IRNEN);
	ltq_dma_w32(DMA_WEIGHT, LTQ_DMA_CCTRL);
	spin_unlock_irqrestore(&ltq_dma_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(ltq_dma_alloc_rx);

void
ltq_dma_free(struct ltq_dma_channel *ch)
{
	if (!ch->desc_base)
		return;
	ltq_dma_close(ch);
<<<<<<< HEAD
	dma_free_coherent(NULL, LTQ_DESC_NUM * LTQ_DESC_SIZE,
=======
	dma_free_coherent(ch->dev, LTQ_DESC_NUM * LTQ_DESC_SIZE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ch->desc_base, ch->phys);
}
EXPORT_SYMBOL_GPL(ltq_dma_free);

void
<<<<<<< HEAD
ltq_dma_init_port(int p)
=======
ltq_dma_init_port(int p, int tx_burst, int rx_burst)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	ltq_dma_w32(p, LTQ_DMA_PS);
	switch (p) {
	case DMA_PORT_ETOP:
		/*
<<<<<<< HEAD
		 * Tell the DMA engine to swap the endianess of data frames and
		 * drop packets if the channel arbitration fails.
		 */
		ltq_dma_w32_mask(0, DMA_ETOP_ENDIANESS | DMA_PDEN,
			LTQ_DMA_PCTRL);
		break;

	case DMA_PORT_DEU:
		ltq_dma_w32((DMA_2W_BURST << 4) | (DMA_2W_BURST << 2),
=======
		 * Tell the DMA engine to swap the endianness of data frames and
		 * drop packets if the channel arbitration fails.
		 */
		ltq_dma_w32_mask(0, (DMA_ETOP_ENDIANNESS | DMA_PDEN),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			LTQ_DMA_PCTRL);
		break;

	default:
		break;
	}
<<<<<<< HEAD
}
EXPORT_SYMBOL_GPL(ltq_dma_init_port);

int __init
ltq_dma_init(void)
{
	int i;

	/* insert and request the memory region */
	if (insert_resource(&iomem_resource, &ltq_dma_resource) < 0)
		panic("Failed to insert dma memory");

	if (request_mem_region(ltq_dma_resource.start,
			resource_size(&ltq_dma_resource), "dma") < 0)
		panic("Failed to request dma memory");

	/* remap dma register range */
	ltq_dma_membase = ioremap_nocache(ltq_dma_resource.start,
				resource_size(&ltq_dma_resource));
	if (!ltq_dma_membase)
		panic("Failed to remap dma memory");

	/* power up and reset the dma engine */
	ltq_pmu_enable(PMU_DMA);
	ltq_dma_w32_mask(0, DMA_RESET, LTQ_DMA_CTRL);

=======

	switch (rx_burst) {
	case 8:
		ltq_dma_w32_mask(0x0c, (DMA_PCTRL_8W_BURST << DMA_RX_BURST_SHIFT),
			LTQ_DMA_PCTRL);
		break;
	case 4:
		ltq_dma_w32_mask(0x0c, (DMA_PCTRL_4W_BURST << DMA_RX_BURST_SHIFT),
			LTQ_DMA_PCTRL);
		break;
	case 2:
		ltq_dma_w32_mask(0x0c, (DMA_PCTRL_2W_BURST << DMA_RX_BURST_SHIFT),
			LTQ_DMA_PCTRL);
		break;
	default:
		break;
	}

	switch (tx_burst) {
	case 8:
		ltq_dma_w32_mask(0x30, (DMA_PCTRL_8W_BURST << DMA_TX_BURST_SHIFT),
			LTQ_DMA_PCTRL);
		break;
	case 4:
		ltq_dma_w32_mask(0x30, (DMA_PCTRL_4W_BURST << DMA_TX_BURST_SHIFT),
			LTQ_DMA_PCTRL);
		break;
	case 2:
		ltq_dma_w32_mask(0x30, (DMA_PCTRL_2W_BURST << DMA_TX_BURST_SHIFT),
			LTQ_DMA_PCTRL);
		break;
	default:
		break;
	}
}
EXPORT_SYMBOL_GPL(ltq_dma_init_port);

static int
ltq_dma_init(struct platform_device *pdev)
{
	struct clk *clk;
	unsigned int id, nchannels;
	int i;

	ltq_dma_membase = devm_platform_get_and_ioremap_resource(pdev, 0, NULL);
	if (IS_ERR(ltq_dma_membase))
		panic("Failed to remap dma resource");

	/* power up and reset the dma engine */
	clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(clk))
		panic("Failed to get dma clock");

	clk_enable(clk);
	ltq_dma_w32_mask(0, DMA_RESET, LTQ_DMA_CTRL);

	usleep_range(1, 10);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* disable all interrupts */
	ltq_dma_w32(0, LTQ_DMA_IRNEN);

	/* reset/configure each channel */
<<<<<<< HEAD
	for (i = 0; i < DMA_MAX_CHANNEL; i++) {
=======
	id = ltq_dma_r32(LTQ_DMA_ID);
	nchannels = ((id & DMA_ID_CHNR) >> 20);
	for (i = 0; i < nchannels; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ltq_dma_w32(i, LTQ_DMA_CS);
		ltq_dma_w32(DMA_CHAN_RST, LTQ_DMA_CCTRL);
		ltq_dma_w32(DMA_POLL | DMA_CLK_DIV4, LTQ_DMA_CPOLL);
		ltq_dma_w32_mask(DMA_CHAN_ON, 0, LTQ_DMA_CCTRL);
	}
<<<<<<< HEAD
	return 0;
}

postcore_initcall(ltq_dma_init);
=======

	dev_info(&pdev->dev,
		"Init done - hw rev: %X, ports: %d, channels: %d\n",
		id & 0x1f, (id >> 16) & 0xf, nchannels);

	return 0;
}

static const struct of_device_id dma_match[] = {
	{ .compatible = "lantiq,dma-xway" },
	{},
};

static struct platform_driver dma_driver = {
	.probe = ltq_dma_init,
	.driver = {
		.name = "dma-xway",
		.of_match_table = dma_match,
	},
};

int __init
dma_init(void)
{
	return platform_driver_register(&dma_driver);
}

postcore_initcall(dma_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
