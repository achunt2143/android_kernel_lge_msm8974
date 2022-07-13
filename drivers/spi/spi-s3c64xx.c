<<<<<<< HEAD
/*
 * Copyright (C) 2009 Samsung Electronics Ltd.
 *	Jaswinder Singh <jassi.brar@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/spi/spi.h>

#include <mach/dma.h>
#include <plat/s3c64xx-spi.h>
=======
// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (c) 2009 Samsung Electronics Co., Ltd.
//      Jaswinder Singh <jassi.brar@samsung.com>

#include <linux/bitops.h>
#include <linux/bits.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_data/spi-s3c64xx.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/spi/spi.h>
#include <linux/types.h>

#define MAX_SPI_PORTS		12
#define S3C64XX_SPI_QUIRK_CS_AUTO	(1 << 1)
#define AUTOSUSPEND_TIMEOUT	2000
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Registers and bit-fields */

#define S3C64XX_SPI_CH_CFG		0x00
#define S3C64XX_SPI_CLK_CFG		0x04
<<<<<<< HEAD
#define S3C64XX_SPI_MODE_CFG	0x08
#define S3C64XX_SPI_SLAVE_SEL	0x0C
=======
#define S3C64XX_SPI_MODE_CFG		0x08
#define S3C64XX_SPI_CS_REG		0x0C
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define S3C64XX_SPI_INT_EN		0x10
#define S3C64XX_SPI_STATUS		0x14
#define S3C64XX_SPI_TX_DATA		0x18
#define S3C64XX_SPI_RX_DATA		0x1C
<<<<<<< HEAD
#define S3C64XX_SPI_PACKET_CNT	0x20
#define S3C64XX_SPI_PENDING_CLR	0x24
#define S3C64XX_SPI_SWAP_CFG	0x28
=======
#define S3C64XX_SPI_PACKET_CNT		0x20
#define S3C64XX_SPI_PENDING_CLR		0x24
#define S3C64XX_SPI_SWAP_CFG		0x28
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define S3C64XX_SPI_FB_CLK		0x2C

#define S3C64XX_SPI_CH_HS_EN		(1<<6)	/* High Speed Enable */
#define S3C64XX_SPI_CH_SW_RST		(1<<5)
#define S3C64XX_SPI_CH_SLAVE		(1<<4)
#define S3C64XX_SPI_CPOL_L		(1<<3)
#define S3C64XX_SPI_CPHA_B		(1<<2)
#define S3C64XX_SPI_CH_RXCH_ON		(1<<1)
#define S3C64XX_SPI_CH_TXCH_ON		(1<<0)

#define S3C64XX_SPI_CLKSEL_SRCMSK	(3<<9)
#define S3C64XX_SPI_CLKSEL_SRCSHFT	9
#define S3C64XX_SPI_ENCLK_ENABLE	(1<<8)
<<<<<<< HEAD
#define S3C64XX_SPI_PSR_MASK 		0xff
=======
#define S3C64XX_SPI_PSR_MASK		0xff
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define S3C64XX_SPI_MODE_CH_TSZ_BYTE		(0<<29)
#define S3C64XX_SPI_MODE_CH_TSZ_HALFWORD	(1<<29)
#define S3C64XX_SPI_MODE_CH_TSZ_WORD		(2<<29)
#define S3C64XX_SPI_MODE_CH_TSZ_MASK		(3<<29)
#define S3C64XX_SPI_MODE_BUS_TSZ_BYTE		(0<<17)
#define S3C64XX_SPI_MODE_BUS_TSZ_HALFWORD	(1<<17)
#define S3C64XX_SPI_MODE_BUS_TSZ_WORD		(2<<17)
#define S3C64XX_SPI_MODE_BUS_TSZ_MASK		(3<<17)
<<<<<<< HEAD
=======
#define S3C64XX_SPI_MODE_RX_RDY_LVL		GENMASK(16, 11)
#define S3C64XX_SPI_MODE_RX_RDY_LVL_SHIFT	11
#define S3C64XX_SPI_MODE_SELF_LOOPBACK		(1<<3)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define S3C64XX_SPI_MODE_RXDMA_ON		(1<<2)
#define S3C64XX_SPI_MODE_TXDMA_ON		(1<<1)
#define S3C64XX_SPI_MODE_4BURST			(1<<0)

<<<<<<< HEAD
#define S3C64XX_SPI_SLAVE_AUTO			(1<<1)
#define S3C64XX_SPI_SLAVE_SIG_INACT		(1<<0)

#define S3C64XX_SPI_ACT(c) writel(0, (c)->regs + S3C64XX_SPI_SLAVE_SEL)

#define S3C64XX_SPI_DEACT(c) writel(S3C64XX_SPI_SLAVE_SIG_INACT, \
					(c)->regs + S3C64XX_SPI_SLAVE_SEL)
=======
#define S3C64XX_SPI_CS_NSC_CNT_2		(2<<4)
#define S3C64XX_SPI_CS_AUTO			(1<<1)
#define S3C64XX_SPI_CS_SIG_INACT		(1<<0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define S3C64XX_SPI_INT_TRAILING_EN		(1<<6)
#define S3C64XX_SPI_INT_RX_OVERRUN_EN		(1<<5)
#define S3C64XX_SPI_INT_RX_UNDERRUN_EN		(1<<4)
#define S3C64XX_SPI_INT_TX_OVERRUN_EN		(1<<3)
#define S3C64XX_SPI_INT_TX_UNDERRUN_EN		(1<<2)
#define S3C64XX_SPI_INT_RX_FIFORDY_EN		(1<<1)
#define S3C64XX_SPI_INT_TX_FIFORDY_EN		(1<<0)

<<<<<<< HEAD
#define S3C64XX_SPI_ST_RX_OVERRUN_ERR		(1<<5)
#define S3C64XX_SPI_ST_RX_UNDERRUN_ERR	(1<<4)
#define S3C64XX_SPI_ST_TX_OVERRUN_ERR		(1<<3)
#define S3C64XX_SPI_ST_TX_UNDERRUN_ERR	(1<<2)
=======
#define S3C64XX_SPI_ST_RX_FIFO_RDY_V2		GENMASK(23, 15)
#define S3C64XX_SPI_ST_TX_FIFO_RDY_V2		GENMASK(14, 6)
#define S3C64XX_SPI_ST_TX_FIFO_LVL_SHIFT	6
#define S3C64XX_SPI_ST_RX_OVERRUN_ERR		(1<<5)
#define S3C64XX_SPI_ST_RX_UNDERRUN_ERR		(1<<4)
#define S3C64XX_SPI_ST_TX_OVERRUN_ERR		(1<<3)
#define S3C64XX_SPI_ST_TX_UNDERRUN_ERR		(1<<2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define S3C64XX_SPI_ST_RX_FIFORDY		(1<<1)
#define S3C64XX_SPI_ST_TX_FIFORDY		(1<<0)

#define S3C64XX_SPI_PACKET_CNT_EN		(1<<16)
<<<<<<< HEAD
=======
#define S3C64XX_SPI_PACKET_CNT_MASK		GENMASK(15, 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define S3C64XX_SPI_PND_TX_UNDERRUN_CLR		(1<<4)
#define S3C64XX_SPI_PND_TX_OVERRUN_CLR		(1<<3)
#define S3C64XX_SPI_PND_RX_UNDERRUN_CLR		(1<<2)
#define S3C64XX_SPI_PND_RX_OVERRUN_CLR		(1<<1)
#define S3C64XX_SPI_PND_TRAILING_CLR		(1<<0)

#define S3C64XX_SPI_SWAP_RX_HALF_WORD		(1<<7)
#define S3C64XX_SPI_SWAP_RX_BYTE		(1<<6)
#define S3C64XX_SPI_SWAP_RX_BIT			(1<<5)
#define S3C64XX_SPI_SWAP_RX_EN			(1<<4)
#define S3C64XX_SPI_SWAP_TX_HALF_WORD		(1<<3)
#define S3C64XX_SPI_SWAP_TX_BYTE		(1<<2)
#define S3C64XX_SPI_SWAP_TX_BIT			(1<<1)
#define S3C64XX_SPI_SWAP_TX_EN			(1<<0)

<<<<<<< HEAD
#define S3C64XX_SPI_FBCLK_MSK		(3<<0)

#define S3C64XX_SPI_ST_TRLCNTZ(v, i) ((((v) >> (i)->rx_lvl_offset) & \
					(((i)->fifo_lvl_mask + 1))) \
					? 1 : 0)

#define S3C64XX_SPI_ST_TX_DONE(v, i) (((v) & (1 << (i)->tx_st_done)) ? 1 : 0)
#define TX_FIFO_LVL(v, i) (((v) >> 6) & (i)->fifo_lvl_mask)
#define RX_FIFO_LVL(v, i) (((v) >> (i)->rx_lvl_offset) & (i)->fifo_lvl_mask)
=======
#define S3C64XX_SPI_FBCLK_MSK			(3<<0)

#define FIFO_LVL_MASK(i) ((i)->port_conf->fifo_lvl_mask[i->port_id])
#define S3C64XX_SPI_ST_TX_DONE(v, i) (((v) & \
				(1 << (i)->port_conf->tx_st_done)) ? 1 : 0)
#define TX_FIFO_LVL(v, sdd)	(((v) & (sdd)->tx_fifomask) >>		\
				 __ffs((sdd)->tx_fifomask))
#define RX_FIFO_LVL(v, sdd)	(((v) & (sdd)->rx_fifomask) >>		\
				 __ffs((sdd)->rx_fifomask))
#define FIFO_DEPTH(i) ((FIFO_LVL_MASK(i) >> 1) + 1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define S3C64XX_SPI_MAX_TRAILCNT	0x3ff
#define S3C64XX_SPI_TRAILCNT_OFF	19

<<<<<<< HEAD
#define S3C64XX_SPI_TRAILCNT		S3C64XX_SPI_MAX_TRAILCNT

#define msecs_to_loops(t) (loops_per_jiffy / 1000 * HZ * t)
=======
#define S3C64XX_SPI_POLLING_SIZE	32

#define msecs_to_loops(t) (loops_per_jiffy / 1000 * HZ * t)
#define is_polling(x)	(x->cntrlr_info->polling)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define RXBUSY    (1<<2)
#define TXBUSY    (1<<3)

struct s3c64xx_spi_dma_data {
<<<<<<< HEAD
	unsigned		ch;
	enum dma_data_direction direction;
	enum dma_ch	dmach;
=======
	struct dma_chan *ch;
	dma_cookie_t cookie;
	enum dma_transfer_direction direction;
};

/**
 * struct s3c64xx_spi_port_config - SPI Controller hardware info
 * @fifo_lvl_mask: [DEPRECATED] use @{rx, tx}_fifomask instead.
 * @rx_lvl_offset: [DEPRECATED] use @{rx,tx}_fifomask instead.
 * @fifo_depth: depth of the FIFO.
 * @rx_fifomask: SPI_STATUS.RX_FIFO_LVL mask. Shifted mask defining the field's
 *               length and position.
 * @tx_fifomask: SPI_STATUS.TX_FIFO_LVL mask. Shifted mask defining the field's
 *               length and position.
 * @tx_st_done: Bit offset of TX_DONE bit in SPI_STATUS regiter.
 * @clk_div: Internal clock divider
 * @quirks: Bitmask of known quirks
 * @high_speed: True, if the controller supports HIGH_SPEED_EN bit.
 * @clk_from_cmu: True, if the controller does not include a clock mux and
 *	prescaler unit.
 * @clk_ioclk: True if clock is present on this device
 * @has_loopback: True if loopback mode can be supported
 * @use_32bit_io: True if the SoC allows only 32-bit register accesses.
 *
 * The Samsung s3c64xx SPI controller are used on various Samsung SoC's but
 * differ in some aspects such as the size of the fifo and spi bus clock
 * setup. Such differences are specified to the driver using this structure
 * which is provided as driver data to the driver.
 */
struct s3c64xx_spi_port_config {
	int	fifo_lvl_mask[MAX_SPI_PORTS];
	int	rx_lvl_offset;
	unsigned int fifo_depth;
	u32	rx_fifomask;
	u32	tx_fifomask;
	int	tx_st_done;
	int	quirks;
	int	clk_div;
	bool	high_speed;
	bool	clk_from_cmu;
	bool	clk_ioclk;
	bool	has_loopback;
	bool	use_32bit_io;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * struct s3c64xx_spi_driver_data - Runtime info holder for SPI driver.
 * @clk: Pointer to the spi clock.
 * @src_clk: Pointer to the clock used to generate SPI signals.
<<<<<<< HEAD
 * @master: Pointer to the SPI Protocol master.
 * @cntrlr_info: Platform specific data for the controller this driver manages.
 * @tgl_spi: Pointer to the last CS left untoggled by the cs_change hint.
 * @queue: To log SPI xfer requests.
 * @lock: Controller specific lock.
 * @state: Set of FLAGS to indicate status.
 * @rx_dmach: Controller's DMA channel for Rx.
 * @tx_dmach: Controller's DMA channel for Tx.
 * @sfr_start: BUS address of SPI controller regs.
 * @regs: Pointer to ioremap'ed controller registers.
 * @irq: interrupt
 * @xfer_completion: To indicate completion of xfer task.
 * @cur_mode: Stores the active configuration of the controller.
 * @cur_bpw: Stores the active bits per word settings.
 * @cur_speed: Stores the active xfer clock speed.
=======
 * @ioclk: Pointer to the i/o clock between host and target
 * @pdev: Pointer to device's platform device data
 * @host: Pointer to the SPI Protocol host.
 * @cntrlr_info: Platform specific data for the controller this driver manages.
 * @lock: Controller specific lock.
 * @state: Set of FLAGS to indicate status.
 * @sfr_start: BUS address of SPI controller regs.
 * @regs: Pointer to ioremap'ed controller registers.
 * @xfer_completion: To indicate completion of xfer task.
 * @cur_mode: Stores the active configuration of the controller.
 * @cur_bpw: Stores the active bits per word settings.
 * @cur_speed: Current clock speed
 * @rx_dma: Local receive DMA data (e.g. chan and direction)
 * @tx_dma: Local transmit DMA data (e.g. chan and direction)
 * @port_conf: Local SPI port configuration data
 * @port_id: [DEPRECATED] use @{rx,tx}_fifomask instead.
 * @fifo_depth: depth of the FIFO.
 * @rx_fifomask: SPI_STATUS.RX_FIFO_LVL mask. Shifted mask defining the field's
 *               length and position.
 * @tx_fifomask: SPI_STATUS.TX_FIFO_LVL mask. Shifted mask defining the field's
 *               length and position.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct s3c64xx_spi_driver_data {
	void __iomem                    *regs;
	struct clk                      *clk;
	struct clk                      *src_clk;
<<<<<<< HEAD
	struct platform_device          *pdev;
	struct spi_master               *master;
	struct s3c64xx_spi_info  *cntrlr_info;
	struct spi_device               *tgl_spi;
	struct list_head                queue;
=======
	struct clk                      *ioclk;
	struct platform_device          *pdev;
	struct spi_controller           *host;
	struct s3c64xx_spi_info         *cntrlr_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spinlock_t                      lock;
	unsigned long                   sfr_start;
	struct completion               xfer_completion;
	unsigned                        state;
	unsigned                        cur_mode, cur_bpw;
	unsigned                        cur_speed;
	struct s3c64xx_spi_dma_data	rx_dma;
	struct s3c64xx_spi_dma_data	tx_dma;
<<<<<<< HEAD
	struct samsung_dma_ops		*ops;
};

static struct s3c2410_dma_client s3c64xx_spi_dma_client = {
	.name = "samsung-spi-dma",
};

static void flush_fifo(struct s3c64xx_spi_driver_data *sdd)
{
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;
=======
	const struct s3c64xx_spi_port_config	*port_conf;
	unsigned int			port_id;
	unsigned int			fifo_depth;
	u32				rx_fifomask;
	u32				tx_fifomask;
};

static void s3c64xx_flush_fifo(struct s3c64xx_spi_driver_data *sdd)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __iomem *regs = sdd->regs;
	unsigned long loops;
	u32 val;

	writel(0, regs + S3C64XX_SPI_PACKET_CNT);

	val = readl(regs + S3C64XX_SPI_CH_CFG);
<<<<<<< HEAD
=======
	val &= ~(S3C64XX_SPI_CH_RXCH_ON | S3C64XX_SPI_CH_TXCH_ON);
	writel(val, regs + S3C64XX_SPI_CH_CFG);

	val = readl(regs + S3C64XX_SPI_CH_CFG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	val |= S3C64XX_SPI_CH_SW_RST;
	val &= ~S3C64XX_SPI_CH_HS_EN;
	writel(val, regs + S3C64XX_SPI_CH_CFG);

	/* Flush TxFIFO*/
	loops = msecs_to_loops(1);
	do {
		val = readl(regs + S3C64XX_SPI_STATUS);
<<<<<<< HEAD
	} while (TX_FIFO_LVL(val, sci) && loops--);
=======
	} while (TX_FIFO_LVL(val, sdd) && loops--);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (loops == 0)
		dev_warn(&sdd->pdev->dev, "Timed out flushing TX FIFO\n");

	/* Flush RxFIFO*/
	loops = msecs_to_loops(1);
	do {
		val = readl(regs + S3C64XX_SPI_STATUS);
<<<<<<< HEAD
		if (RX_FIFO_LVL(val, sci))
=======
		if (RX_FIFO_LVL(val, sdd))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			readl(regs + S3C64XX_SPI_RX_DATA);
		else
			break;
	} while (loops--);

	if (loops == 0)
		dev_warn(&sdd->pdev->dev, "Timed out flushing RX FIFO\n");

	val = readl(regs + S3C64XX_SPI_CH_CFG);
	val &= ~S3C64XX_SPI_CH_SW_RST;
	writel(val, regs + S3C64XX_SPI_CH_CFG);

	val = readl(regs + S3C64XX_SPI_MODE_CFG);
	val &= ~(S3C64XX_SPI_MODE_TXDMA_ON | S3C64XX_SPI_MODE_RXDMA_ON);
	writel(val, regs + S3C64XX_SPI_MODE_CFG);
<<<<<<< HEAD

	val = readl(regs + S3C64XX_SPI_CH_CFG);
	val &= ~(S3C64XX_SPI_CH_RXCH_ON | S3C64XX_SPI_CH_TXCH_ON);
	writel(val, regs + S3C64XX_SPI_CH_CFG);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void s3c64xx_spi_dmacb(void *data)
{
	struct s3c64xx_spi_driver_data *sdd;
	struct s3c64xx_spi_dma_data *dma = data;
	unsigned long flags;

	if (dma->direction == DMA_DEV_TO_MEM)
		sdd = container_of(data,
			struct s3c64xx_spi_driver_data, rx_dma);
	else
		sdd = container_of(data,
			struct s3c64xx_spi_driver_data, tx_dma);

	spin_lock_irqsave(&sdd->lock, flags);

	if (dma->direction == DMA_DEV_TO_MEM) {
		sdd->state &= ~RXBUSY;
		if (!(sdd->state & TXBUSY))
			complete(&sdd->xfer_completion);
	} else {
		sdd->state &= ~TXBUSY;
		if (!(sdd->state & RXBUSY))
			complete(&sdd->xfer_completion);
	}

	spin_unlock_irqrestore(&sdd->lock, flags);
}

<<<<<<< HEAD
static void prepare_dma(struct s3c64xx_spi_dma_data *dma,
					unsigned len, dma_addr_t buf)
{
	struct s3c64xx_spi_driver_data *sdd;
	struct samsung_dma_prep_info info;

	if (dma->direction == DMA_DEV_TO_MEM)
		sdd = container_of((void *)dma,
			struct s3c64xx_spi_driver_data, rx_dma);
	else
		sdd = container_of((void *)dma,
			struct s3c64xx_spi_driver_data, tx_dma);

	info.cap = DMA_SLAVE;
	info.len = len;
	info.fp = s3c64xx_spi_dmacb;
	info.fp_param = dma;
	info.direction = dma->direction;
	info.buf = buf;

	sdd->ops->prepare(dma->ch, &info);
	sdd->ops->trigger(dma->ch);
}

static int acquire_dma(struct s3c64xx_spi_driver_data *sdd)
{
	struct samsung_dma_info info;

	sdd->ops = samsung_dma_get_ops();

	info.cap = DMA_SLAVE;
	info.client = &s3c64xx_spi_dma_client;
	info.width = sdd->cur_bpw / 8;

	info.direction = sdd->rx_dma.direction;
	info.fifo = sdd->sfr_start + S3C64XX_SPI_RX_DATA;
	sdd->rx_dma.ch = sdd->ops->request(sdd->rx_dma.dmach, &info);
	info.direction =  sdd->tx_dma.direction;
	info.fifo = sdd->sfr_start + S3C64XX_SPI_TX_DATA;
	sdd->tx_dma.ch = sdd->ops->request(sdd->tx_dma.dmach, &info);

	return 1;
}

static void enable_datapath(struct s3c64xx_spi_driver_data *sdd,
				struct spi_device *spi,
				struct spi_transfer *xfer, int dma_mode)
{
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;
	void __iomem *regs = sdd->regs;
	u32 modecfg, chcfg;
=======
static int s3c64xx_prepare_dma(struct s3c64xx_spi_dma_data *dma,
			       struct sg_table *sgt)
{
	struct s3c64xx_spi_driver_data *sdd;
	struct dma_slave_config config;
	struct dma_async_tx_descriptor *desc;
	int ret;

	memset(&config, 0, sizeof(config));

	if (dma->direction == DMA_DEV_TO_MEM) {
		sdd = container_of((void *)dma,
			struct s3c64xx_spi_driver_data, rx_dma);
		config.src_addr = sdd->sfr_start + S3C64XX_SPI_RX_DATA;
		config.src_addr_width = sdd->cur_bpw / 8;
		config.src_maxburst = 1;
	} else {
		sdd = container_of((void *)dma,
			struct s3c64xx_spi_driver_data, tx_dma);
		config.dst_addr = sdd->sfr_start + S3C64XX_SPI_TX_DATA;
		config.dst_addr_width = sdd->cur_bpw / 8;
		config.dst_maxburst = 1;
	}
	config.direction = dma->direction;
	ret = dmaengine_slave_config(dma->ch, &config);
	if (ret)
		return ret;

	desc = dmaengine_prep_slave_sg(dma->ch, sgt->sgl, sgt->nents,
				       dma->direction, DMA_PREP_INTERRUPT);
	if (!desc) {
		dev_err(&sdd->pdev->dev, "unable to prepare %s scatterlist",
			dma->direction == DMA_DEV_TO_MEM ? "rx" : "tx");
		return -ENOMEM;
	}

	desc->callback = s3c64xx_spi_dmacb;
	desc->callback_param = dma;

	dma->cookie = dmaengine_submit(desc);
	ret = dma_submit_error(dma->cookie);
	if (ret) {
		dev_err(&sdd->pdev->dev, "DMA submission failed");
		return ret;
	}

	dma_async_issue_pending(dma->ch);
	return 0;
}

static void s3c64xx_spi_set_cs(struct spi_device *spi, bool enable)
{
	struct s3c64xx_spi_driver_data *sdd =
					spi_controller_get_devdata(spi->controller);

	if (sdd->cntrlr_info->no_cs)
		return;

	if (enable) {
		if (!(sdd->port_conf->quirks & S3C64XX_SPI_QUIRK_CS_AUTO)) {
			writel(0, sdd->regs + S3C64XX_SPI_CS_REG);
		} else {
			u32 ssel = readl(sdd->regs + S3C64XX_SPI_CS_REG);

			ssel |= (S3C64XX_SPI_CS_AUTO |
						S3C64XX_SPI_CS_NSC_CNT_2);
			writel(ssel, sdd->regs + S3C64XX_SPI_CS_REG);
		}
	} else {
		if (!(sdd->port_conf->quirks & S3C64XX_SPI_QUIRK_CS_AUTO))
			writel(S3C64XX_SPI_CS_SIG_INACT,
			       sdd->regs + S3C64XX_SPI_CS_REG);
	}
}

static int s3c64xx_spi_prepare_transfer(struct spi_controller *spi)
{
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(spi);

	if (is_polling(sdd))
		return 0;

	/* Requests DMA channels */
	sdd->rx_dma.ch = dma_request_chan(&sdd->pdev->dev, "rx");
	if (IS_ERR(sdd->rx_dma.ch)) {
		dev_err(&sdd->pdev->dev, "Failed to get RX DMA channel\n");
		sdd->rx_dma.ch = NULL;
		return 0;
	}

	sdd->tx_dma.ch = dma_request_chan(&sdd->pdev->dev, "tx");
	if (IS_ERR(sdd->tx_dma.ch)) {
		dev_err(&sdd->pdev->dev, "Failed to get TX DMA channel\n");
		dma_release_channel(sdd->rx_dma.ch);
		sdd->tx_dma.ch = NULL;
		sdd->rx_dma.ch = NULL;
		return 0;
	}

	spi->dma_rx = sdd->rx_dma.ch;
	spi->dma_tx = sdd->tx_dma.ch;

	return 0;
}

static int s3c64xx_spi_unprepare_transfer(struct spi_controller *spi)
{
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(spi);

	if (is_polling(sdd))
		return 0;

	/* Releases DMA channels if they are allocated */
	if (sdd->rx_dma.ch && sdd->tx_dma.ch) {
		dma_release_channel(sdd->rx_dma.ch);
		dma_release_channel(sdd->tx_dma.ch);
		sdd->rx_dma.ch = NULL;
		sdd->tx_dma.ch = NULL;
	}

	return 0;
}

static bool s3c64xx_spi_can_dma(struct spi_controller *host,
				struct spi_device *spi,
				struct spi_transfer *xfer)
{
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(host);

	if (sdd->rx_dma.ch && sdd->tx_dma.ch)
		return xfer->len >= sdd->fifo_depth;

	return false;
}

static void s3c64xx_iowrite8_32_rep(volatile void __iomem *addr,
				    const void *buffer, unsigned int count)
{
	if (count) {
		const u8 *buf = buffer;

		do {
			__raw_writel(*buf++, addr);
		} while (--count);
	}
}

static void s3c64xx_iowrite16_32_rep(volatile void __iomem *addr,
				     const void *buffer, unsigned int count)
{
	if (count) {
		const u16 *buf = buffer;

		do {
			__raw_writel(*buf++, addr);
		} while (--count);
	}
}

static void s3c64xx_iowrite_rep(const struct s3c64xx_spi_driver_data *sdd,
				struct spi_transfer *xfer)
{
	void __iomem *addr = sdd->regs + S3C64XX_SPI_TX_DATA;
	const void *buf = xfer->tx_buf;
	unsigned int len = xfer->len;

	switch (sdd->cur_bpw) {
	case 32:
		iowrite32_rep(addr, buf, len / 4);
		break;
	case 16:
		if (sdd->port_conf->use_32bit_io)
			s3c64xx_iowrite16_32_rep(addr, buf, len / 2);
		else
			iowrite16_rep(addr, buf, len / 2);
		break;
	default:
		if (sdd->port_conf->use_32bit_io)
			s3c64xx_iowrite8_32_rep(addr, buf, len);
		else
			iowrite8_rep(addr, buf, len);
		break;
	}
}

static int s3c64xx_enable_datapath(struct s3c64xx_spi_driver_data *sdd,
				    struct spi_transfer *xfer, int dma_mode)
{
	void __iomem *regs = sdd->regs;
	u32 modecfg, chcfg;
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	modecfg = readl(regs + S3C64XX_SPI_MODE_CFG);
	modecfg &= ~(S3C64XX_SPI_MODE_TXDMA_ON | S3C64XX_SPI_MODE_RXDMA_ON);

	chcfg = readl(regs + S3C64XX_SPI_CH_CFG);
	chcfg &= ~S3C64XX_SPI_CH_TXCH_ON;

	if (dma_mode) {
		chcfg &= ~S3C64XX_SPI_CH_RXCH_ON;
	} else {
		/* Always shift in data in FIFO, even if xfer is Tx only,
		 * this helps setting PCKT_CNT value for generating clocks
		 * as exactly needed.
		 */
		chcfg |= S3C64XX_SPI_CH_RXCH_ON;
		writel(((xfer->len * 8 / sdd->cur_bpw) & 0xffff)
					| S3C64XX_SPI_PACKET_CNT_EN,
					regs + S3C64XX_SPI_PACKET_CNT);
	}

	if (xfer->tx_buf != NULL) {
		sdd->state |= TXBUSY;
		chcfg |= S3C64XX_SPI_CH_TXCH_ON;
		if (dma_mode) {
			modecfg |= S3C64XX_SPI_MODE_TXDMA_ON;
<<<<<<< HEAD
			prepare_dma(&sdd->tx_dma, xfer->len, xfer->tx_dma);
		} else {
			switch (sdd->cur_bpw) {
			case 32:
				iowrite32_rep(regs + S3C64XX_SPI_TX_DATA,
					xfer->tx_buf, xfer->len / 4);
				break;
			case 16:
				iowrite16_rep(regs + S3C64XX_SPI_TX_DATA,
					xfer->tx_buf, xfer->len / 2);
				break;
			default:
				iowrite8_rep(regs + S3C64XX_SPI_TX_DATA,
					xfer->tx_buf, xfer->len);
				break;
			}
=======
			ret = s3c64xx_prepare_dma(&sdd->tx_dma, &xfer->tx_sg);
		} else {
			s3c64xx_iowrite_rep(sdd, xfer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	if (xfer->rx_buf != NULL) {
		sdd->state |= RXBUSY;

<<<<<<< HEAD
		if (sci->high_speed && sdd->cur_speed >= 30000000UL
=======
		if (sdd->port_conf->high_speed && sdd->cur_speed >= 30000000UL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					&& !(sdd->cur_mode & SPI_CPHA))
			chcfg |= S3C64XX_SPI_CH_HS_EN;

		if (dma_mode) {
			modecfg |= S3C64XX_SPI_MODE_RXDMA_ON;
			chcfg |= S3C64XX_SPI_CH_RXCH_ON;
			writel(((xfer->len * 8 / sdd->cur_bpw) & 0xffff)
					| S3C64XX_SPI_PACKET_CNT_EN,
					regs + S3C64XX_SPI_PACKET_CNT);
<<<<<<< HEAD
			prepare_dma(&sdd->rx_dma, xfer->len, xfer->rx_dma);
		}
	}

	writel(modecfg, regs + S3C64XX_SPI_MODE_CFG);
	writel(chcfg, regs + S3C64XX_SPI_CH_CFG);
}

static inline void enable_cs(struct s3c64xx_spi_driver_data *sdd,
						struct spi_device *spi)
{
	struct s3c64xx_spi_csinfo *cs;

	if (sdd->tgl_spi != NULL) { /* If last device toggled after mssg */
		if (sdd->tgl_spi != spi) { /* if last mssg on diff device */
			/* Deselect the last toggled device */
			cs = sdd->tgl_spi->controller_data;
			cs->set_level(cs->line,
					spi->mode & SPI_CS_HIGH ? 0 : 1);
		}
		sdd->tgl_spi = NULL;
	}

	cs = spi->controller_data;
	cs->set_level(cs->line, spi->mode & SPI_CS_HIGH ? 1 : 0);
}

static int wait_for_xfer(struct s3c64xx_spi_driver_data *sdd,
				struct spi_transfer *xfer, int dma_mode)
{
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;
	void __iomem *regs = sdd->regs;
	unsigned long val;
	int ms;

	/* millisecs to xfer 'len' bytes @ 'cur_speed' */
	ms = xfer->len * 8 * 1000 / sdd->cur_speed;
	ms += 10; /* some tolerance */

	if (dma_mode) {
		val = msecs_to_jiffies(ms) + 10;
		val = wait_for_completion_timeout(&sdd->xfer_completion, val);
	} else {
		u32 status;
		val = msecs_to_loops(ms);
		do {
			status = readl(regs + S3C64XX_SPI_STATUS);
		} while (RX_FIFO_LVL(status, sci) < xfer->len && --val);
	}

	if (!val)
		return -EIO;

	if (dma_mode) {
		u32 status;

		/*
		 * DmaTx returns after simply writing data in the FIFO,
		 * w/o waiting for real transmission on the bus to finish.
		 * DmaRx returns only after Dma read data from FIFO which
		 * needs bus transmission to finish, so we don't worry if
		 * Xfer involved Rx(with or without Tx).
		 */
		if (xfer->rx_buf == NULL) {
			val = msecs_to_loops(10);
			status = readl(regs + S3C64XX_SPI_STATUS);
			while ((TX_FIFO_LVL(status, sci)
				|| !S3C64XX_SPI_ST_TX_DONE(status, sci))
					&& --val) {
				cpu_relax();
				status = readl(regs + S3C64XX_SPI_STATUS);
			}

			if (!val)
				return -EIO;
		}
	} else {
		/* If it was only Tx */
		if (xfer->rx_buf == NULL) {
			sdd->state &= ~TXBUSY;
			return 0;
		}

		switch (sdd->cur_bpw) {
		case 32:
			ioread32_rep(regs + S3C64XX_SPI_RX_DATA,
				xfer->rx_buf, xfer->len / 4);
			break;
		case 16:
			ioread16_rep(regs + S3C64XX_SPI_RX_DATA,
				xfer->rx_buf, xfer->len / 2);
			break;
		default:
			ioread8_rep(regs + S3C64XX_SPI_RX_DATA,
				xfer->rx_buf, xfer->len);
			break;
		}
		sdd->state &= ~RXBUSY;
	}
=======
			ret = s3c64xx_prepare_dma(&sdd->rx_dma, &xfer->rx_sg);
		}
	}

	if (ret)
		return ret;

	writel(modecfg, regs + S3C64XX_SPI_MODE_CFG);
	writel(chcfg, regs + S3C64XX_SPI_CH_CFG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static inline void disable_cs(struct s3c64xx_spi_driver_data *sdd,
						struct spi_device *spi)
{
	struct s3c64xx_spi_csinfo *cs = spi->controller_data;

	if (sdd->tgl_spi == spi)
		sdd->tgl_spi = NULL;

	cs->set_level(cs->line, spi->mode & SPI_CS_HIGH ? 0 : 1);
}

static void s3c64xx_spi_config(struct s3c64xx_spi_driver_data *sdd)
{
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;
	void __iomem *regs = sdd->regs;
	u32 val;

	/* Disable Clock */
	if (sci->clk_from_cmu) {
		clk_disable(sdd->src_clk);
	} else {
=======
static u32 s3c64xx_spi_wait_for_timeout(struct s3c64xx_spi_driver_data *sdd,
					int timeout_ms)
{
	void __iomem *regs = sdd->regs;
	unsigned long val = 1;
	u32 status;
	u32 max_fifo = sdd->fifo_depth;

	if (timeout_ms)
		val = msecs_to_loops(timeout_ms);

	do {
		status = readl(regs + S3C64XX_SPI_STATUS);
	} while (RX_FIFO_LVL(status, sdd) < max_fifo && --val);

	/* return the actual received data length */
	return RX_FIFO_LVL(status, sdd);
}

static int s3c64xx_wait_for_dma(struct s3c64xx_spi_driver_data *sdd,
				struct spi_transfer *xfer)
{
	void __iomem *regs = sdd->regs;
	unsigned long val;
	u32 status;
	int ms;

	/* millisecs to xfer 'len' bytes @ 'cur_speed' */
	ms = xfer->len * 8 * 1000 / sdd->cur_speed;
	ms += 30;               /* some tolerance */
	ms = max(ms, 100);      /* minimum timeout */

	val = msecs_to_jiffies(ms) + 10;
	val = wait_for_completion_timeout(&sdd->xfer_completion, val);

	/*
	 * If the previous xfer was completed within timeout, then
	 * proceed further else return -ETIMEDOUT.
	 * DmaTx returns after simply writing data in the FIFO,
	 * w/o waiting for real transmission on the bus to finish.
	 * DmaRx returns only after Dma read data from FIFO which
	 * needs bus transmission to finish, so we don't worry if
	 * Xfer involved Rx(with or without Tx).
	 */
	if (val && !xfer->rx_buf) {
		val = msecs_to_loops(10);
		status = readl(regs + S3C64XX_SPI_STATUS);
		while ((TX_FIFO_LVL(status, sdd)
			|| !S3C64XX_SPI_ST_TX_DONE(status, sdd))
		       && --val) {
			cpu_relax();
			status = readl(regs + S3C64XX_SPI_STATUS);
		}

	}

	/* If timed out while checking rx/tx status return error */
	if (!val)
		return -ETIMEDOUT;

	return 0;
}

static int s3c64xx_wait_for_pio(struct s3c64xx_spi_driver_data *sdd,
				struct spi_transfer *xfer, bool use_irq)
{
	void __iomem *regs = sdd->regs;
	unsigned long val;
	u32 status;
	int loops;
	u32 cpy_len;
	u8 *buf;
	int ms;
	unsigned long time_us;

	/* microsecs to xfer 'len' bytes @ 'cur_speed' */
	time_us = (xfer->len * 8 * 1000 * 1000) / sdd->cur_speed;
	ms = (time_us / 1000);
	ms += 10; /* some tolerance */

	/* sleep during signal transfer time */
	status = readl(regs + S3C64XX_SPI_STATUS);
	if (RX_FIFO_LVL(status, sdd) < xfer->len)
		usleep_range(time_us / 2, time_us);

	if (use_irq) {
		val = msecs_to_jiffies(ms);
		if (!wait_for_completion_timeout(&sdd->xfer_completion, val))
			return -ETIMEDOUT;
	}

	val = msecs_to_loops(ms);
	do {
		status = readl(regs + S3C64XX_SPI_STATUS);
	} while (RX_FIFO_LVL(status, sdd) < xfer->len && --val);

	if (!val)
		return -EIO;

	/* If it was only Tx */
	if (!xfer->rx_buf) {
		sdd->state &= ~TXBUSY;
		return 0;
	}

	/*
	 * If the receive length is bigger than the controller fifo
	 * size, calculate the loops and read the fifo as many times.
	 * loops = length / max fifo size (calculated by using the
	 * fifo mask).
	 * For any size less than the fifo size the below code is
	 * executed atleast once.
	 */
	loops = xfer->len / sdd->fifo_depth;
	buf = xfer->rx_buf;
	do {
		/* wait for data to be received in the fifo */
		cpy_len = s3c64xx_spi_wait_for_timeout(sdd,
						       (loops ? ms : 0));

		switch (sdd->cur_bpw) {
		case 32:
			ioread32_rep(regs + S3C64XX_SPI_RX_DATA,
				     buf, cpy_len / 4);
			break;
		case 16:
			ioread16_rep(regs + S3C64XX_SPI_RX_DATA,
				     buf, cpy_len / 2);
			break;
		default:
			ioread8_rep(regs + S3C64XX_SPI_RX_DATA,
				    buf, cpy_len);
			break;
		}

		buf = buf + cpy_len;
	} while (loops--);
	sdd->state &= ~RXBUSY;

	return 0;
}

static int s3c64xx_spi_config(struct s3c64xx_spi_driver_data *sdd)
{
	void __iomem *regs = sdd->regs;
	int ret;
	u32 val;
	int div = sdd->port_conf->clk_div;

	/* Disable Clock */
	if (!sdd->port_conf->clk_from_cmu) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		val = readl(regs + S3C64XX_SPI_CLK_CFG);
		val &= ~S3C64XX_SPI_ENCLK_ENABLE;
		writel(val, regs + S3C64XX_SPI_CLK_CFG);
	}

	/* Set Polarity and Phase */
	val = readl(regs + S3C64XX_SPI_CH_CFG);
	val &= ~(S3C64XX_SPI_CH_SLAVE |
			S3C64XX_SPI_CPOL_L |
			S3C64XX_SPI_CPHA_B);

	if (sdd->cur_mode & SPI_CPOL)
		val |= S3C64XX_SPI_CPOL_L;

	if (sdd->cur_mode & SPI_CPHA)
		val |= S3C64XX_SPI_CPHA_B;

	writel(val, regs + S3C64XX_SPI_CH_CFG);

	/* Set Channel & DMA Mode */
	val = readl(regs + S3C64XX_SPI_MODE_CFG);
	val &= ~(S3C64XX_SPI_MODE_BUS_TSZ_MASK
			| S3C64XX_SPI_MODE_CH_TSZ_MASK);

	switch (sdd->cur_bpw) {
	case 32:
		val |= S3C64XX_SPI_MODE_BUS_TSZ_WORD;
		val |= S3C64XX_SPI_MODE_CH_TSZ_WORD;
		break;
	case 16:
		val |= S3C64XX_SPI_MODE_BUS_TSZ_HALFWORD;
		val |= S3C64XX_SPI_MODE_CH_TSZ_HALFWORD;
		break;
	default:
		val |= S3C64XX_SPI_MODE_BUS_TSZ_BYTE;
		val |= S3C64XX_SPI_MODE_CH_TSZ_BYTE;
		break;
	}

<<<<<<< HEAD
	writel(val, regs + S3C64XX_SPI_MODE_CFG);

	if (sci->clk_from_cmu) {
		/* Configure Clock */
		/* There is half-multiplier before the SPI */
		clk_set_rate(sdd->src_clk, sdd->cur_speed * 2);
		/* Enable Clock */
		clk_enable(sdd->src_clk);
=======
	if ((sdd->cur_mode & SPI_LOOP) && sdd->port_conf->has_loopback)
		val |= S3C64XX_SPI_MODE_SELF_LOOPBACK;
	else
		val &= ~S3C64XX_SPI_MODE_SELF_LOOPBACK;

	writel(val, regs + S3C64XX_SPI_MODE_CFG);

	if (sdd->port_conf->clk_from_cmu) {
		ret = clk_set_rate(sdd->src_clk, sdd->cur_speed * div);
		if (ret)
			return ret;
		sdd->cur_speed = clk_get_rate(sdd->src_clk) / div;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		/* Configure Clock */
		val = readl(regs + S3C64XX_SPI_CLK_CFG);
		val &= ~S3C64XX_SPI_PSR_MASK;
<<<<<<< HEAD
		val |= ((clk_get_rate(sdd->src_clk) / sdd->cur_speed / 2 - 1)
=======
		val |= ((clk_get_rate(sdd->src_clk) / sdd->cur_speed / div - 1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				& S3C64XX_SPI_PSR_MASK);
		writel(val, regs + S3C64XX_SPI_CLK_CFG);

		/* Enable Clock */
		val = readl(regs + S3C64XX_SPI_CLK_CFG);
		val |= S3C64XX_SPI_ENCLK_ENABLE;
		writel(val, regs + S3C64XX_SPI_CLK_CFG);
	}
<<<<<<< HEAD
}

#define XFER_DMAADDR_INVALID DMA_BIT_MASK(32)

static int s3c64xx_spi_map_mssg(struct s3c64xx_spi_driver_data *sdd,
						struct spi_message *msg)
{
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;
	struct device *dev = &sdd->pdev->dev;
	struct spi_transfer *xfer;

	if (msg->is_dma_mapped)
		return 0;

	/* First mark all xfer unmapped */
	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		xfer->rx_dma = XFER_DMAADDR_INVALID;
		xfer->tx_dma = XFER_DMAADDR_INVALID;
	}

	/* Map until end or first fail */
	list_for_each_entry(xfer, &msg->transfers, transfer_list) {

		if (xfer->len <= ((sci->fifo_lvl_mask >> 1) + 1))
			continue;

		if (xfer->tx_buf != NULL) {
			xfer->tx_dma = dma_map_single(dev,
					(void *)xfer->tx_buf, xfer->len,
					DMA_TO_DEVICE);
			if (dma_mapping_error(dev, xfer->tx_dma)) {
				dev_err(dev, "dma_map_single Tx failed\n");
				xfer->tx_dma = XFER_DMAADDR_INVALID;
				return -ENOMEM;
			}
		}

		if (xfer->rx_buf != NULL) {
			xfer->rx_dma = dma_map_single(dev, xfer->rx_buf,
						xfer->len, DMA_FROM_DEVICE);
			if (dma_mapping_error(dev, xfer->rx_dma)) {
				dev_err(dev, "dma_map_single Rx failed\n");
				dma_unmap_single(dev, xfer->tx_dma,
						xfer->len, DMA_TO_DEVICE);
				xfer->tx_dma = XFER_DMAADDR_INVALID;
				xfer->rx_dma = XFER_DMAADDR_INVALID;
				return -ENOMEM;
			}
		}
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void s3c64xx_spi_unmap_mssg(struct s3c64xx_spi_driver_data *sdd,
						struct spi_message *msg)
{
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;
	struct device *dev = &sdd->pdev->dev;
	struct spi_transfer *xfer;

	if (msg->is_dma_mapped)
		return;

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {

		if (xfer->len <= ((sci->fifo_lvl_mask >> 1) + 1))
			continue;

		if (xfer->rx_buf != NULL
				&& xfer->rx_dma != XFER_DMAADDR_INVALID)
			dma_unmap_single(dev, xfer->rx_dma,
						xfer->len, DMA_FROM_DEVICE);

		if (xfer->tx_buf != NULL
				&& xfer->tx_dma != XFER_DMAADDR_INVALID)
			dma_unmap_single(dev, xfer->tx_dma,
						xfer->len, DMA_TO_DEVICE);
	}
}

static int s3c64xx_spi_transfer_one_message(struct spi_master *master,
					    struct spi_message *msg)
{
	struct s3c64xx_spi_driver_data *sdd = spi_master_get_devdata(master);
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;
	struct spi_device *spi = msg->spi;
	struct s3c64xx_spi_csinfo *cs = spi->controller_data;
	struct spi_transfer *xfer;
	int status = 0, cs_toggle = 0;
	u32 speed;
	u8 bpw;

	/* If Master's(controller) state differs from that needed by Slave */
	if (sdd->cur_speed != spi->max_speed_hz
			|| sdd->cur_mode != spi->mode
			|| sdd->cur_bpw != spi->bits_per_word) {
		sdd->cur_bpw = spi->bits_per_word;
		sdd->cur_speed = spi->max_speed_hz;
		sdd->cur_mode = spi->mode;
		s3c64xx_spi_config(sdd);
	}

	/* Map all the transfers if needed */
	if (s3c64xx_spi_map_mssg(sdd, msg)) {
		dev_err(&spi->dev,
			"Xfer: Unable to map message buffers!\n");
		status = -ENOMEM;
		goto out;
	}

	/* Configure feedback delay */
	writel(cs->fb_delay & 0x3, sdd->regs + S3C64XX_SPI_FB_CLK);

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {

		unsigned long flags;
		int use_dma;

		INIT_COMPLETION(sdd->xfer_completion);

		/* Only BPW and Speed may change across transfers */
		bpw = xfer->bits_per_word ? : spi->bits_per_word;
		speed = xfer->speed_hz ? : spi->max_speed_hz;

		if (xfer->len % (bpw / 8)) {
			dev_err(&spi->dev,
				"Xfer length(%u) not a multiple of word size(%u)\n",
				xfer->len, bpw / 8);
			status = -EIO;
			goto out;
		}

		if (bpw != sdd->cur_bpw || speed != sdd->cur_speed) {
			sdd->cur_bpw = bpw;
			sdd->cur_speed = speed;
			s3c64xx_spi_config(sdd);
		}

		/* Polling method for xfers not bigger than FIFO capacity */
		if (xfer->len <= ((sci->fifo_lvl_mask >> 1) + 1))
			use_dma = 0;
		else
			use_dma = 1;

=======
#define XFER_DMAADDR_INVALID DMA_BIT_MASK(32)

static int s3c64xx_spi_prepare_message(struct spi_controller *host,
				       struct spi_message *msg)
{
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(host);
	struct spi_device *spi = msg->spi;
	struct s3c64xx_spi_csinfo *cs = spi->controller_data;

	/* Configure feedback delay */
	if (!cs)
		/* No delay if not defined */
		writel(0, sdd->regs + S3C64XX_SPI_FB_CLK);
	else
		writel(cs->fb_delay & 0x3, sdd->regs + S3C64XX_SPI_FB_CLK);

	return 0;
}

static size_t s3c64xx_spi_max_transfer_size(struct spi_device *spi)
{
	struct spi_controller *ctlr = spi->controller;

	return ctlr->can_dma ? S3C64XX_SPI_PACKET_CNT_MASK : SIZE_MAX;
}

static int s3c64xx_spi_transfer_one(struct spi_controller *host,
				    struct spi_device *spi,
				    struct spi_transfer *xfer)
{
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(host);
	const unsigned int fifo_len = sdd->fifo_depth;
	const void *tx_buf = NULL;
	void *rx_buf = NULL;
	int target_len = 0, origin_len = 0;
	int use_dma = 0;
	bool use_irq = false;
	int status;
	u32 speed;
	u8 bpw;
	unsigned long flags;
	u32 rdy_lv;
	u32 val;

	reinit_completion(&sdd->xfer_completion);

	/* Only BPW and Speed may change across transfers */
	bpw = xfer->bits_per_word;
	speed = xfer->speed_hz;

	if (bpw != sdd->cur_bpw || speed != sdd->cur_speed) {
		sdd->cur_bpw = bpw;
		sdd->cur_speed = speed;
		sdd->cur_mode = spi->mode;
		status = s3c64xx_spi_config(sdd);
		if (status)
			return status;
	}

	if (!is_polling(sdd) && xfer->len >= fifo_len &&
	    sdd->rx_dma.ch && sdd->tx_dma.ch) {
		use_dma = 1;
	} else if (xfer->len >= fifo_len) {
		tx_buf = xfer->tx_buf;
		rx_buf = xfer->rx_buf;
		origin_len = xfer->len;
		target_len = xfer->len;
		xfer->len = fifo_len - 1;
	}

	do {
		/* transfer size is greater than 32, change to IRQ mode */
		if (!use_dma && xfer->len > S3C64XX_SPI_POLLING_SIZE)
			use_irq = true;

		if (use_irq) {
			reinit_completion(&sdd->xfer_completion);

			rdy_lv = xfer->len;
			/* Setup RDY_FIFO trigger Level
			 * RDY_LVL =
			 * fifo_lvl up to 64 byte -> N bytes
			 *               128 byte -> RDY_LVL * 2 bytes
			 *               256 byte -> RDY_LVL * 4 bytes
			 */
			if (fifo_len == 128)
				rdy_lv /= 2;
			else if (fifo_len == 256)
				rdy_lv /= 4;

			val = readl(sdd->regs + S3C64XX_SPI_MODE_CFG);
			val &= ~S3C64XX_SPI_MODE_RX_RDY_LVL;
			val |= (rdy_lv << S3C64XX_SPI_MODE_RX_RDY_LVL_SHIFT);
			writel(val, sdd->regs + S3C64XX_SPI_MODE_CFG);

			/* Enable FIFO_RDY_EN IRQ */
			val = readl(sdd->regs + S3C64XX_SPI_INT_EN);
			writel((val | S3C64XX_SPI_INT_RX_FIFORDY_EN),
					sdd->regs + S3C64XX_SPI_INT_EN);

		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_lock_irqsave(&sdd->lock, flags);

		/* Pending only which is to be done */
		sdd->state &= ~RXBUSY;
		sdd->state &= ~TXBUSY;

<<<<<<< HEAD
		enable_datapath(sdd, spi, xfer, use_dma);

		/* Slave Select */
		enable_cs(sdd, spi);

		/* Start the signals */
		S3C64XX_SPI_ACT(sdd);

		spin_unlock_irqrestore(&sdd->lock, flags);

		status = wait_for_xfer(sdd, xfer, use_dma);

		/* Quiese the signals */
		S3C64XX_SPI_DEACT(sdd);

		if (status) {
			dev_err(&spi->dev, "I/O Error: "
				"rx-%d tx-%d res:rx-%c tx-%c len-%d\n",
				xfer->rx_buf ? 1 : 0, xfer->tx_buf ? 1 : 0,
				(sdd->state & RXBUSY) ? 'f' : 'p',
				(sdd->state & TXBUSY) ? 'f' : 'p',
				xfer->len);

			if (use_dma) {
				if (xfer->tx_buf != NULL
						&& (sdd->state & TXBUSY))
					sdd->ops->stop(sdd->tx_dma.ch);
				if (xfer->rx_buf != NULL
						&& (sdd->state & RXBUSY))
					sdd->ops->stop(sdd->rx_dma.ch);
			}

			goto out;
		}

		if (xfer->delay_usecs)
			udelay(xfer->delay_usecs);

		if (xfer->cs_change) {
			/* Hint that the next mssg is gonna be
			   for the same device */
			if (list_is_last(&xfer->transfer_list,
						&msg->transfers))
				cs_toggle = 1;
			else
				disable_cs(sdd, spi);
		}

		msg->actual_length += xfer->len;

		flush_fifo(sdd);
	}

out:
	if (!cs_toggle || status)
		disable_cs(sdd, spi);
	else
		sdd->tgl_spi = spi;

	s3c64xx_spi_unmap_mssg(sdd, msg);

	msg->status = status;

	spi_finalize_current_message(master);

	return 0;
}

static int s3c64xx_spi_prepare_transfer(struct spi_master *spi)
{
	struct s3c64xx_spi_driver_data *sdd = spi_master_get_devdata(spi);

	/* Acquire DMA channels */
	while (!acquire_dma(sdd))
		msleep(10);

	pm_runtime_get_sync(&sdd->pdev->dev);

	return 0;
}

static int s3c64xx_spi_unprepare_transfer(struct spi_master *spi)
{
	struct s3c64xx_spi_driver_data *sdd = spi_master_get_devdata(spi);

	/* Free DMA channels */
	sdd->ops->release(sdd->rx_dma.ch, &s3c64xx_spi_dma_client);
	sdd->ops->release(sdd->tx_dma.ch, &s3c64xx_spi_dma_client);

	pm_runtime_put(&sdd->pdev->dev);

	return 0;
=======
		/* Start the signals */
		s3c64xx_spi_set_cs(spi, true);

		status = s3c64xx_enable_datapath(sdd, xfer, use_dma);

		spin_unlock_irqrestore(&sdd->lock, flags);

		if (status) {
			dev_err(&spi->dev, "failed to enable data path for transfer: %d\n", status);
			break;
		}

		if (use_dma)
			status = s3c64xx_wait_for_dma(sdd, xfer);
		else
			status = s3c64xx_wait_for_pio(sdd, xfer, use_irq);

		if (status) {
			dev_err(&spi->dev,
				"I/O Error: rx-%d tx-%d rx-%c tx-%c len-%d dma-%d res-(%d)\n",
				xfer->rx_buf ? 1 : 0, xfer->tx_buf ? 1 : 0,
				(sdd->state & RXBUSY) ? 'f' : 'p',
				(sdd->state & TXBUSY) ? 'f' : 'p',
				xfer->len, use_dma ? 1 : 0, status);

			if (use_dma) {
				struct dma_tx_state s;

				if (xfer->tx_buf && (sdd->state & TXBUSY)) {
					dmaengine_pause(sdd->tx_dma.ch);
					dmaengine_tx_status(sdd->tx_dma.ch, sdd->tx_dma.cookie, &s);
					dmaengine_terminate_all(sdd->tx_dma.ch);
					dev_err(&spi->dev, "TX residue: %d\n", s.residue);

				}
				if (xfer->rx_buf && (sdd->state & RXBUSY)) {
					dmaengine_pause(sdd->rx_dma.ch);
					dmaengine_tx_status(sdd->rx_dma.ch, sdd->rx_dma.cookie, &s);
					dmaengine_terminate_all(sdd->rx_dma.ch);
					dev_err(&spi->dev, "RX residue: %d\n", s.residue);
				}
			}
		} else {
			s3c64xx_flush_fifo(sdd);
		}
		if (target_len > 0) {
			target_len -= xfer->len;

			if (xfer->tx_buf)
				xfer->tx_buf += xfer->len;

			if (xfer->rx_buf)
				xfer->rx_buf += xfer->len;

			if (target_len >= fifo_len)
				xfer->len = fifo_len - 1;
			else
				xfer->len = target_len;
		}
	} while (target_len > 0);

	if (origin_len) {
		/* Restore original xfer buffers and length */
		xfer->tx_buf = tx_buf;
		xfer->rx_buf = rx_buf;
		xfer->len = origin_len;
	}

	return status;
}

static struct s3c64xx_spi_csinfo *s3c64xx_get_target_ctrldata(
				struct spi_device *spi)
{
	struct s3c64xx_spi_csinfo *cs;
	struct device_node *target_np, *data_np = NULL;
	u32 fb_delay = 0;

	target_np = spi->dev.of_node;
	if (!target_np) {
		dev_err(&spi->dev, "device node not found\n");
		return ERR_PTR(-EINVAL);
	}

	cs = kzalloc(sizeof(*cs), GFP_KERNEL);
	if (!cs)
		return ERR_PTR(-ENOMEM);

	data_np = of_get_child_by_name(target_np, "controller-data");
	if (!data_np) {
		dev_info(&spi->dev, "feedback delay set to default (0)\n");
		return cs;
	}

	of_property_read_u32(data_np, "samsung,spi-feedback-delay", &fb_delay);
	cs->fb_delay = fb_delay;
	of_node_put(data_np);
	return cs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Here we only check the validity of requested configuration
 * and save the configuration in a local data-structure.
 * The controller is actually configured only just before we
 * get a message to transfer.
 */
static int s3c64xx_spi_setup(struct spi_device *spi)
{
	struct s3c64xx_spi_csinfo *cs = spi->controller_data;
	struct s3c64xx_spi_driver_data *sdd;
<<<<<<< HEAD
	struct s3c64xx_spi_info *sci;
	struct spi_message *msg;
	unsigned long flags;
	int err = 0;

	if (cs == NULL || cs->set_level == NULL) {
		dev_err(&spi->dev, "No CS for SPI(%d)\n", spi->chip_select);
		return -ENODEV;
	}

	sdd = spi_master_get_devdata(spi->master);
	sci = sdd->cntrlr_info;

	spin_lock_irqsave(&sdd->lock, flags);

	list_for_each_entry(msg, &sdd->queue, queue) {
		/* Is some mssg is already queued for this device */
		if (msg->spi == spi) {
			dev_err(&spi->dev,
				"setup: attempt while mssg in queue!\n");
			spin_unlock_irqrestore(&sdd->lock, flags);
			return -EBUSY;
		}
	}

	spin_unlock_irqrestore(&sdd->lock, flags);

	if (spi->bits_per_word != 8
			&& spi->bits_per_word != 16
			&& spi->bits_per_word != 32) {
		dev_err(&spi->dev, "setup: %dbits/wrd not supported!\n",
							spi->bits_per_word);
		err = -EINVAL;
		goto setup_exit;
	}

	pm_runtime_get_sync(&sdd->pdev->dev);

	/* Check if we can provide the requested rate */
	if (!sci->clk_from_cmu) {
		u32 psr, speed;

		/* Max possible */
		speed = clk_get_rate(sdd->src_clk) / 2 / (0 + 1);
=======
	int err;
	int div;

	sdd = spi_controller_get_devdata(spi->controller);
	if (spi->dev.of_node) {
		cs = s3c64xx_get_target_ctrldata(spi);
		spi->controller_data = cs;
	}

	/* NULL is fine, we just avoid using the FB delay (=0) */
	if (IS_ERR(cs)) {
		dev_err(&spi->dev, "No CS for SPI(%d)\n", spi_get_chipselect(spi, 0));
		return -ENODEV;
	}

	if (!spi_get_ctldata(spi))
		spi_set_ctldata(spi, cs);

	pm_runtime_get_sync(&sdd->pdev->dev);

	div = sdd->port_conf->clk_div;

	/* Check if we can provide the requested rate */
	if (!sdd->port_conf->clk_from_cmu) {
		u32 psr, speed;

		/* Max possible */
		speed = clk_get_rate(sdd->src_clk) / div / (0 + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (spi->max_speed_hz > speed)
			spi->max_speed_hz = speed;

<<<<<<< HEAD
		psr = clk_get_rate(sdd->src_clk) / 2 / spi->max_speed_hz - 1;
=======
		psr = clk_get_rate(sdd->src_clk) / div / spi->max_speed_hz - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		psr &= S3C64XX_SPI_PSR_MASK;
		if (psr == S3C64XX_SPI_PSR_MASK)
			psr--;

<<<<<<< HEAD
		speed = clk_get_rate(sdd->src_clk) / 2 / (psr + 1);
=======
		speed = clk_get_rate(sdd->src_clk) / div / (psr + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (spi->max_speed_hz < speed) {
			if (psr+1 < S3C64XX_SPI_PSR_MASK) {
				psr++;
			} else {
				err = -EINVAL;
				goto setup_exit;
			}
		}

<<<<<<< HEAD
		speed = clk_get_rate(sdd->src_clk) / 2 / (psr + 1);
		if (spi->max_speed_hz >= speed)
			spi->max_speed_hz = speed;
		else
			err = -EINVAL;
	}

	pm_runtime_put(&sdd->pdev->dev);

setup_exit:

	/* setup() returns with device de-selected */
	disable_cs(sdd, spi);
=======
		speed = clk_get_rate(sdd->src_clk) / div / (psr + 1);
		if (spi->max_speed_hz >= speed) {
			spi->max_speed_hz = speed;
		} else {
			dev_err(&spi->dev, "Can't set %dHz transfer speed\n",
				spi->max_speed_hz);
			err = -EINVAL;
			goto setup_exit;
		}
	}

	pm_runtime_mark_last_busy(&sdd->pdev->dev);
	pm_runtime_put_autosuspend(&sdd->pdev->dev);
	s3c64xx_spi_set_cs(spi, false);

	return 0;

setup_exit:
	pm_runtime_mark_last_busy(&sdd->pdev->dev);
	pm_runtime_put_autosuspend(&sdd->pdev->dev);
	/* setup() returns with device de-selected */
	s3c64xx_spi_set_cs(spi, false);

	spi_set_ctldata(spi, NULL);

	/* This was dynamically allocated on the DT path */
	if (spi->dev.of_node)
		kfree(cs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
static irqreturn_t s3c64xx_spi_irq(int irq, void *data)
{
	struct s3c64xx_spi_driver_data *sdd = data;
	struct spi_master *spi = sdd->master;
=======
static void s3c64xx_spi_cleanup(struct spi_device *spi)
{
	struct s3c64xx_spi_csinfo *cs = spi_get_ctldata(spi);

	/* This was dynamically allocated on the DT path */
	if (spi->dev.of_node)
		kfree(cs);

	spi_set_ctldata(spi, NULL);
}

static irqreturn_t s3c64xx_spi_irq(int irq, void *data)
{
	struct s3c64xx_spi_driver_data *sdd = data;
	struct spi_controller *spi = sdd->host;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int val, clr = 0;

	val = readl(sdd->regs + S3C64XX_SPI_STATUS);

	if (val & S3C64XX_SPI_ST_RX_OVERRUN_ERR) {
		clr = S3C64XX_SPI_PND_RX_OVERRUN_CLR;
		dev_err(&spi->dev, "RX overrun\n");
	}
	if (val & S3C64XX_SPI_ST_RX_UNDERRUN_ERR) {
		clr |= S3C64XX_SPI_PND_RX_UNDERRUN_CLR;
		dev_err(&spi->dev, "RX underrun\n");
	}
	if (val & S3C64XX_SPI_ST_TX_OVERRUN_ERR) {
		clr |= S3C64XX_SPI_PND_TX_OVERRUN_CLR;
		dev_err(&spi->dev, "TX overrun\n");
	}
	if (val & S3C64XX_SPI_ST_TX_UNDERRUN_ERR) {
		clr |= S3C64XX_SPI_PND_TX_UNDERRUN_CLR;
		dev_err(&spi->dev, "TX underrun\n");
	}

<<<<<<< HEAD
=======
	if (val & S3C64XX_SPI_ST_RX_FIFORDY) {
		complete(&sdd->xfer_completion);
		/* No pending clear irq, turn-off INT_EN_RX_FIFO_RDY */
		val = readl(sdd->regs + S3C64XX_SPI_INT_EN);
		writel((val & ~S3C64XX_SPI_INT_RX_FIFORDY_EN),
				sdd->regs + S3C64XX_SPI_INT_EN);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Clear the pending irq by setting and then clearing it */
	writel(clr, sdd->regs + S3C64XX_SPI_PENDING_CLR);
	writel(0, sdd->regs + S3C64XX_SPI_PENDING_CLR);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static void s3c64xx_spi_hwinit(struct s3c64xx_spi_driver_data *sdd, int channel)
=======
static void s3c64xx_spi_hwinit(struct s3c64xx_spi_driver_data *sdd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;
	void __iomem *regs = sdd->regs;
	unsigned int val;

	sdd->cur_speed = 0;

<<<<<<< HEAD
	S3C64XX_SPI_DEACT(sdd);
=======
	if (sci->no_cs)
		writel(0, sdd->regs + S3C64XX_SPI_CS_REG);
	else if (!(sdd->port_conf->quirks & S3C64XX_SPI_QUIRK_CS_AUTO))
		writel(S3C64XX_SPI_CS_SIG_INACT, sdd->regs + S3C64XX_SPI_CS_REG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Disable Interrupts - we use Polling if not DMA mode */
	writel(0, regs + S3C64XX_SPI_INT_EN);

<<<<<<< HEAD
	if (!sci->clk_from_cmu)
=======
	if (!sdd->port_conf->clk_from_cmu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		writel(sci->src_clk_nr << S3C64XX_SPI_CLKSEL_SRCSHFT,
				regs + S3C64XX_SPI_CLK_CFG);
	writel(0, regs + S3C64XX_SPI_MODE_CFG);
	writel(0, regs + S3C64XX_SPI_PACKET_CNT);

	/* Clear any irq pending bits, should set and clear the bits */
	val = S3C64XX_SPI_PND_RX_OVERRUN_CLR |
		S3C64XX_SPI_PND_RX_UNDERRUN_CLR |
		S3C64XX_SPI_PND_TX_OVERRUN_CLR |
		S3C64XX_SPI_PND_TX_UNDERRUN_CLR;
	writel(val, regs + S3C64XX_SPI_PENDING_CLR);
	writel(0, regs + S3C64XX_SPI_PENDING_CLR);

	writel(0, regs + S3C64XX_SPI_SWAP_CFG);

	val = readl(regs + S3C64XX_SPI_MODE_CFG);
	val &= ~S3C64XX_SPI_MODE_4BURST;
<<<<<<< HEAD
	val &= ~(S3C64XX_SPI_MAX_TRAILCNT << S3C64XX_SPI_TRAILCNT_OFF);
	val |= (S3C64XX_SPI_TRAILCNT << S3C64XX_SPI_TRAILCNT_OFF);
	writel(val, regs + S3C64XX_SPI_MODE_CFG);

	flush_fifo(sdd);
}

static int __init s3c64xx_spi_probe(struct platform_device *pdev)
{
	struct resource	*mem_res, *dmatx_res, *dmarx_res;
	struct s3c64xx_spi_driver_data *sdd;
	struct s3c64xx_spi_info *sci;
	struct spi_master *master;
	int ret, irq;
	char clk_name[16];

	if (pdev->id < 0) {
		dev_err(&pdev->dev,
				"Invalid platform device id-%d\n", pdev->id);
		return -ENODEV;
	}

	if (pdev->dev.platform_data == NULL) {
		dev_err(&pdev->dev, "platform_data missing!\n");
		return -ENODEV;
	}

	sci = pdev->dev.platform_data;

	/* Check for availability of necessary resource */

	dmatx_res = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (dmatx_res == NULL) {
		dev_err(&pdev->dev, "Unable to get SPI-Tx dma resource\n");
		return -ENXIO;
	}

	dmarx_res = platform_get_resource(pdev, IORESOURCE_DMA, 1);
	if (dmarx_res == NULL) {
		dev_err(&pdev->dev, "Unable to get SPI-Rx dma resource\n");
		return -ENXIO;
	}

	mem_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (mem_res == NULL) {
		dev_err(&pdev->dev, "Unable to get SPI MEM resource\n");
		return -ENXIO;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_warn(&pdev->dev, "Failed to get IRQ: %d\n", irq);
		return irq;
	}

	master = spi_alloc_master(&pdev->dev,
				sizeof(struct s3c64xx_spi_driver_data));
	if (master == NULL) {
		dev_err(&pdev->dev, "Unable to allocate SPI Master\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, master);

	sdd = spi_master_get_devdata(master);
	sdd->master = master;
	sdd->cntrlr_info = sci;
	sdd->pdev = pdev;
	sdd->sfr_start = mem_res->start;
	sdd->tx_dma.dmach = dmatx_res->start;
	sdd->tx_dma.direction = DMA_MEM_TO_DEV;
	sdd->rx_dma.dmach = dmarx_res->start;
	sdd->rx_dma.direction = DMA_DEV_TO_MEM;

	sdd->cur_bpw = 8;

	master->bus_num = pdev->id;
	master->setup = s3c64xx_spi_setup;
	master->prepare_transfer_hardware = s3c64xx_spi_prepare_transfer;
	master->transfer_one_message = s3c64xx_spi_transfer_one_message;
	master->unprepare_transfer_hardware = s3c64xx_spi_unprepare_transfer;
	master->num_chipselect = sci->num_cs;
	master->dma_alignment = 8;
	/* the spi->mode bits understood by this driver: */
	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;

	if (request_mem_region(mem_res->start,
			resource_size(mem_res), pdev->name) == NULL) {
		dev_err(&pdev->dev, "Req mem region failed\n");
		ret = -ENXIO;
		goto err0;
	}

	sdd->regs = ioremap(mem_res->start, resource_size(mem_res));
	if (sdd->regs == NULL) {
		dev_err(&pdev->dev, "Unable to remap IO\n");
		ret = -ENXIO;
		goto err1;
	}

	if (sci->cfg_gpio == NULL || sci->cfg_gpio(pdev)) {
		dev_err(&pdev->dev, "Unable to config gpio\n");
		ret = -EBUSY;
		goto err2;
	}

	/* Setup clocks */
	sdd->clk = clk_get(&pdev->dev, "spi");
	if (IS_ERR(sdd->clk)) {
		dev_err(&pdev->dev, "Unable to acquire clock 'spi'\n");
		ret = PTR_ERR(sdd->clk);
		goto err3;
	}

	if (clk_enable(sdd->clk)) {
		dev_err(&pdev->dev, "Couldn't enable clock 'spi'\n");
		ret = -EBUSY;
		goto err4;
	}

	sprintf(clk_name, "spi_busclk%d", sci->src_clk_nr);
	sdd->src_clk = clk_get(&pdev->dev, clk_name);
	if (IS_ERR(sdd->src_clk)) {
		dev_err(&pdev->dev,
			"Unable to acquire clock '%s'\n", clk_name);
		ret = PTR_ERR(sdd->src_clk);
		goto err5;
	}

	if (clk_enable(sdd->src_clk)) {
		dev_err(&pdev->dev, "Couldn't enable clock '%s'\n", clk_name);
		ret = -EBUSY;
		goto err6;
	}

	/* Setup Deufult Mode */
	s3c64xx_spi_hwinit(sdd, pdev->id);

	spin_lock_init(&sdd->lock);
	init_completion(&sdd->xfer_completion);
	INIT_LIST_HEAD(&sdd->queue);

	ret = request_irq(irq, s3c64xx_spi_irq, 0, "spi-s3c64xx", sdd);
	if (ret != 0) {
		dev_err(&pdev->dev, "Failed to request IRQ %d: %d\n",
			irq, ret);
		goto err7;
=======
	val |= (S3C64XX_SPI_MAX_TRAILCNT << S3C64XX_SPI_TRAILCNT_OFF);
	writel(val, regs + S3C64XX_SPI_MODE_CFG);

	s3c64xx_flush_fifo(sdd);
}

#ifdef CONFIG_OF
static struct s3c64xx_spi_info *s3c64xx_spi_parse_dt(struct device *dev)
{
	struct s3c64xx_spi_info *sci;
	u32 temp;

	sci = devm_kzalloc(dev, sizeof(*sci), GFP_KERNEL);
	if (!sci)
		return ERR_PTR(-ENOMEM);

	if (of_property_read_u32(dev->of_node, "samsung,spi-src-clk", &temp)) {
		dev_dbg(dev, "spi bus clock parent not specified, using clock at index 0 as parent\n");
		sci->src_clk_nr = 0;
	} else {
		sci->src_clk_nr = temp;
	}

	if (of_property_read_u32(dev->of_node, "num-cs", &temp)) {
		dev_dbg(dev, "number of chip select lines not specified, assuming 1 chip select line\n");
		sci->num_cs = 1;
	} else {
		sci->num_cs = temp;
	}

	sci->no_cs = of_property_read_bool(dev->of_node, "no-cs-readback");
	sci->polling = !of_property_present(dev->of_node, "dmas");

	return sci;
}
#else
static struct s3c64xx_spi_info *s3c64xx_spi_parse_dt(struct device *dev)
{
	return dev_get_platdata(dev);
}
#endif

static inline const struct s3c64xx_spi_port_config *s3c64xx_spi_get_port_config(
						struct platform_device *pdev)
{
#ifdef CONFIG_OF
	if (pdev->dev.of_node)
		return of_device_get_match_data(&pdev->dev);
#endif
	return (const struct s3c64xx_spi_port_config *)platform_get_device_id(pdev)->driver_data;
}

static int s3c64xx_spi_set_port_id(struct platform_device *pdev,
				   struct s3c64xx_spi_driver_data *sdd)
{
	const struct s3c64xx_spi_port_config *port_conf = sdd->port_conf;
	int ret;

	if (port_conf->rx_fifomask && port_conf->tx_fifomask)
		return 0;

	if (pdev->dev.of_node) {
		ret = of_alias_get_id(pdev->dev.of_node, "spi");
		if (ret < 0)
			return dev_err_probe(&pdev->dev, ret,
					     "Failed to get alias id\n");
		sdd->port_id = ret;
	} else {
		if (pdev->id < 0)
			return dev_err_probe(&pdev->dev, -EINVAL,
					     "Negative platform ID is not allowed\n");
		sdd->port_id = pdev->id;
	}

	return 0;
}

static void s3c64xx_spi_set_fifomask(struct s3c64xx_spi_driver_data *sdd)
{
	const struct s3c64xx_spi_port_config *port_conf = sdd->port_conf;

	if (port_conf->rx_fifomask)
		sdd->rx_fifomask = port_conf->rx_fifomask;
	else
		sdd->rx_fifomask = FIFO_LVL_MASK(sdd) <<
			port_conf->rx_lvl_offset;

	if (port_conf->tx_fifomask)
		sdd->tx_fifomask = port_conf->tx_fifomask;
	else
		sdd->tx_fifomask = FIFO_LVL_MASK(sdd) <<
			S3C64XX_SPI_ST_TX_FIFO_LVL_SHIFT;
}

static int s3c64xx_spi_probe(struct platform_device *pdev)
{
	struct resource	*mem_res;
	struct s3c64xx_spi_driver_data *sdd;
	struct s3c64xx_spi_info *sci = dev_get_platdata(&pdev->dev);
	struct spi_controller *host;
	int ret, irq;
	char clk_name[16];

	if (!sci && pdev->dev.of_node) {
		sci = s3c64xx_spi_parse_dt(&pdev->dev);
		if (IS_ERR(sci))
			return PTR_ERR(sci);
	}

	if (!sci)
		return dev_err_probe(&pdev->dev, -ENODEV,
				     "Platform_data missing!\n");

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	host = devm_spi_alloc_host(&pdev->dev, sizeof(*sdd));
	if (!host)
		return dev_err_probe(&pdev->dev, -ENOMEM,
				     "Unable to allocate SPI Host\n");

	platform_set_drvdata(pdev, host);

	sdd = spi_controller_get_devdata(host);
	sdd->port_conf = s3c64xx_spi_get_port_config(pdev);
	sdd->host = host;
	sdd->cntrlr_info = sci;
	sdd->pdev = pdev;

	ret = s3c64xx_spi_set_port_id(pdev, sdd);
	if (ret)
		return ret;

	if (sdd->port_conf->fifo_depth)
		sdd->fifo_depth = sdd->port_conf->fifo_depth;
	else if (of_property_read_u32(pdev->dev.of_node, "fifo-depth",
				      &sdd->fifo_depth))
		sdd->fifo_depth = FIFO_DEPTH(sdd);

	s3c64xx_spi_set_fifomask(sdd);

	sdd->cur_bpw = 8;

	sdd->tx_dma.direction = DMA_MEM_TO_DEV;
	sdd->rx_dma.direction = DMA_DEV_TO_MEM;

	host->dev.of_node = pdev->dev.of_node;
	host->bus_num = -1;
	host->setup = s3c64xx_spi_setup;
	host->cleanup = s3c64xx_spi_cleanup;
	host->prepare_transfer_hardware = s3c64xx_spi_prepare_transfer;
	host->unprepare_transfer_hardware = s3c64xx_spi_unprepare_transfer;
	host->prepare_message = s3c64xx_spi_prepare_message;
	host->transfer_one = s3c64xx_spi_transfer_one;
	host->max_transfer_size = s3c64xx_spi_max_transfer_size;
	host->num_chipselect = sci->num_cs;
	host->use_gpio_descriptors = true;
	host->dma_alignment = 8;
	host->bits_per_word_mask = SPI_BPW_MASK(32) | SPI_BPW_MASK(16) |
				   SPI_BPW_MASK(8);
	/* the spi->mode bits understood by this driver: */
	host->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;
	if (sdd->port_conf->has_loopback)
		host->mode_bits |= SPI_LOOP;
	host->auto_runtime_pm = true;
	if (!is_polling(sdd))
		host->can_dma = s3c64xx_spi_can_dma;

	sdd->regs = devm_platform_get_and_ioremap_resource(pdev, 0, &mem_res);
	if (IS_ERR(sdd->regs))
		return PTR_ERR(sdd->regs);
	sdd->sfr_start = mem_res->start;

	if (sci->cfg_gpio && sci->cfg_gpio())
		return dev_err_probe(&pdev->dev, -EBUSY,
				     "Unable to config gpio\n");

	/* Setup clocks */
	sdd->clk = devm_clk_get_enabled(&pdev->dev, "spi");
	if (IS_ERR(sdd->clk))
		return dev_err_probe(&pdev->dev, PTR_ERR(sdd->clk),
				     "Unable to acquire clock 'spi'\n");

	sprintf(clk_name, "spi_busclk%d", sci->src_clk_nr);
	sdd->src_clk = devm_clk_get_enabled(&pdev->dev, clk_name);
	if (IS_ERR(sdd->src_clk))
		return dev_err_probe(&pdev->dev, PTR_ERR(sdd->src_clk),
				     "Unable to acquire clock '%s'\n",
				     clk_name);

	if (sdd->port_conf->clk_ioclk) {
		sdd->ioclk = devm_clk_get_enabled(&pdev->dev, "spi_ioclk");
		if (IS_ERR(sdd->ioclk))
			return dev_err_probe(&pdev->dev, PTR_ERR(sdd->ioclk),
					     "Unable to acquire 'ioclk'\n");
	}

	pm_runtime_set_autosuspend_delay(&pdev->dev, AUTOSUSPEND_TIMEOUT);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_set_active(&pdev->dev);
	pm_runtime_enable(&pdev->dev);
	pm_runtime_get_sync(&pdev->dev);

	/* Setup Deufult Mode */
	s3c64xx_spi_hwinit(sdd);

	spin_lock_init(&sdd->lock);
	init_completion(&sdd->xfer_completion);

	ret = devm_request_irq(&pdev->dev, irq, s3c64xx_spi_irq, 0,
				"spi-s3c64xx", sdd);
	if (ret != 0) {
		dev_err(&pdev->dev, "Failed to request IRQ %d: %d\n",
			irq, ret);
		goto err_pm_put;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	writel(S3C64XX_SPI_INT_RX_OVERRUN_EN | S3C64XX_SPI_INT_RX_UNDERRUN_EN |
	       S3C64XX_SPI_INT_TX_OVERRUN_EN | S3C64XX_SPI_INT_TX_UNDERRUN_EN,
	       sdd->regs + S3C64XX_SPI_INT_EN);

<<<<<<< HEAD
	if (spi_register_master(master)) {
		dev_err(&pdev->dev, "cannot register SPI master\n");
		ret = -EBUSY;
		goto err8;
	}

	dev_dbg(&pdev->dev, "Samsung SoC SPI Driver loaded for Bus SPI-%d "
					"with %d Slaves attached\n",
					pdev->id, master->num_chipselect);
	dev_dbg(&pdev->dev, "\tIOmem=[0x%x-0x%x]\tDMA=[Rx-%d, Tx-%d]\n",
					mem_res->end, mem_res->start,
					sdd->rx_dma.dmach, sdd->tx_dma.dmach);

	pm_runtime_enable(&pdev->dev);

	return 0;

err8:
	free_irq(irq, sdd);
err7:
	clk_disable(sdd->src_clk);
err6:
	clk_put(sdd->src_clk);
err5:
	clk_disable(sdd->clk);
err4:
	clk_put(sdd->clk);
err3:
err2:
	iounmap((void *) sdd->regs);
err1:
	release_mem_region(mem_res->start, resource_size(mem_res));
err0:
	platform_set_drvdata(pdev, NULL);
	spi_master_put(master);
=======
	ret = devm_spi_register_controller(&pdev->dev, host);
	if (ret != 0) {
		dev_err(&pdev->dev, "cannot register SPI host: %d\n", ret);
		goto err_pm_put;
	}

	dev_dbg(&pdev->dev, "Samsung SoC SPI Driver loaded for Bus SPI-%d with %d Targets attached\n",
		host->bus_num, host->num_chipselect);
	dev_dbg(&pdev->dev, "\tIOmem=[%pR]\tFIFO %dbytes\n",
		mem_res, sdd->fifo_depth);

	pm_runtime_mark_last_busy(&pdev->dev);
	pm_runtime_put_autosuspend(&pdev->dev);

	return 0;

err_pm_put:
	pm_runtime_put_noidle(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
	pm_runtime_set_suspended(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static int s3c64xx_spi_remove(struct platform_device *pdev)
{
	struct spi_master *master = spi_master_get(platform_get_drvdata(pdev));
	struct s3c64xx_spi_driver_data *sdd = spi_master_get_devdata(master);
	struct resource	*mem_res;

	pm_runtime_disable(&pdev->dev);

	spi_unregister_master(master);

	writel(0, sdd->regs + S3C64XX_SPI_INT_EN);

	free_irq(platform_get_irq(pdev, 0), sdd);

	clk_disable(sdd->src_clk);
	clk_put(sdd->src_clk);

	clk_disable(sdd->clk);
	clk_put(sdd->clk);

	iounmap((void *) sdd->regs);

	mem_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (mem_res != NULL)
		release_mem_region(mem_res->start, resource_size(mem_res));

	platform_set_drvdata(pdev, NULL);
	spi_master_put(master);

	return 0;
}

#ifdef CONFIG_PM
static int s3c64xx_spi_suspend(struct device *dev)
{
	struct spi_master *master = spi_master_get(dev_get_drvdata(dev));
	struct s3c64xx_spi_driver_data *sdd = spi_master_get_devdata(master);

	spi_master_suspend(master);

	/* Disable the clock */
	clk_disable(sdd->src_clk);
	clk_disable(sdd->clk);
=======
static void s3c64xx_spi_remove(struct platform_device *pdev)
{
	struct spi_controller *host = platform_get_drvdata(pdev);
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(host);

	pm_runtime_get_sync(&pdev->dev);

	writel(0, sdd->regs + S3C64XX_SPI_INT_EN);

	if (!is_polling(sdd)) {
		dma_release_channel(sdd->rx_dma.ch);
		dma_release_channel(sdd->tx_dma.ch);
	}

	pm_runtime_put_noidle(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
	pm_runtime_set_suspended(&pdev->dev);
}

#ifdef CONFIG_PM_SLEEP
static int s3c64xx_spi_suspend(struct device *dev)
{
	struct spi_controller *host = dev_get_drvdata(dev);
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(host);
	int ret;

	ret = spi_controller_suspend(host);
	if (ret)
		return ret;

	ret = pm_runtime_force_suspend(dev);
	if (ret < 0)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sdd->cur_speed = 0; /* Output Clock is stopped */

	return 0;
}

static int s3c64xx_spi_resume(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct spi_master *master = spi_master_get(dev_get_drvdata(dev));
	struct s3c64xx_spi_driver_data *sdd = spi_master_get_devdata(master);
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;

	sci->cfg_gpio(pdev);

	/* Enable the clock */
	clk_enable(sdd->src_clk);
	clk_enable(sdd->clk);

	s3c64xx_spi_hwinit(sdd, pdev->id);

	spi_master_resume(master);

	return 0;
}
#endif /* CONFIG_PM */

#ifdef CONFIG_PM_RUNTIME
static int s3c64xx_spi_runtime_suspend(struct device *dev)
{
	struct spi_master *master = spi_master_get(dev_get_drvdata(dev));
	struct s3c64xx_spi_driver_data *sdd = spi_master_get_devdata(master);

	clk_disable(sdd->clk);
	clk_disable(sdd->src_clk);
=======
	struct spi_controller *host = dev_get_drvdata(dev);
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(host);
	struct s3c64xx_spi_info *sci = sdd->cntrlr_info;
	int ret;

	if (sci->cfg_gpio)
		sci->cfg_gpio();

	ret = pm_runtime_force_resume(dev);
	if (ret < 0)
		return ret;

	return spi_controller_resume(host);
}
#endif /* CONFIG_PM_SLEEP */

#ifdef CONFIG_PM
static int s3c64xx_spi_runtime_suspend(struct device *dev)
{
	struct spi_controller *host = dev_get_drvdata(dev);
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(host);

	clk_disable_unprepare(sdd->clk);
	clk_disable_unprepare(sdd->src_clk);
	clk_disable_unprepare(sdd->ioclk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int s3c64xx_spi_runtime_resume(struct device *dev)
{
<<<<<<< HEAD
	struct spi_master *master = spi_master_get(dev_get_drvdata(dev));
	struct s3c64xx_spi_driver_data *sdd = spi_master_get_devdata(master);

	clk_enable(sdd->src_clk);
	clk_enable(sdd->clk);

	return 0;
}
#endif /* CONFIG_PM_RUNTIME */
=======
	struct spi_controller *host = dev_get_drvdata(dev);
	struct s3c64xx_spi_driver_data *sdd = spi_controller_get_devdata(host);
	int ret;

	if (sdd->port_conf->clk_ioclk) {
		ret = clk_prepare_enable(sdd->ioclk);
		if (ret != 0)
			return ret;
	}

	ret = clk_prepare_enable(sdd->src_clk);
	if (ret != 0)
		goto err_disable_ioclk;

	ret = clk_prepare_enable(sdd->clk);
	if (ret != 0)
		goto err_disable_src_clk;

	s3c64xx_spi_hwinit(sdd);

	writel(S3C64XX_SPI_INT_RX_OVERRUN_EN | S3C64XX_SPI_INT_RX_UNDERRUN_EN |
	       S3C64XX_SPI_INT_TX_OVERRUN_EN | S3C64XX_SPI_INT_TX_UNDERRUN_EN,
	       sdd->regs + S3C64XX_SPI_INT_EN);

	return 0;

err_disable_src_clk:
	clk_disable_unprepare(sdd->src_clk);
err_disable_ioclk:
	clk_disable_unprepare(sdd->ioclk);

	return ret;
}
#endif /* CONFIG_PM */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct dev_pm_ops s3c64xx_spi_pm = {
	SET_SYSTEM_SLEEP_PM_OPS(s3c64xx_spi_suspend, s3c64xx_spi_resume)
	SET_RUNTIME_PM_OPS(s3c64xx_spi_runtime_suspend,
			   s3c64xx_spi_runtime_resume, NULL)
};

<<<<<<< HEAD
static struct platform_driver s3c64xx_spi_driver = {
	.driver = {
		.name	= "s3c64xx-spi",
		.owner = THIS_MODULE,
		.pm = &s3c64xx_spi_pm,
	},
	.remove = s3c64xx_spi_remove,
};
MODULE_ALIAS("platform:s3c64xx-spi");

static int __init s3c64xx_spi_init(void)
{
	return platform_driver_probe(&s3c64xx_spi_driver, s3c64xx_spi_probe);
}
subsys_initcall(s3c64xx_spi_init);

static void __exit s3c64xx_spi_exit(void)
{
	platform_driver_unregister(&s3c64xx_spi_driver);
}
module_exit(s3c64xx_spi_exit);
=======
static const struct s3c64xx_spi_port_config s3c2443_spi_port_config = {
	/* fifo_lvl_mask is deprecated. Use {rx, tx}_fifomask instead. */
	.fifo_lvl_mask	= { 0x7f },
	/* rx_lvl_offset is deprecated. Use {rx, tx}_fifomask instead. */
	.rx_lvl_offset	= 13,
	.tx_st_done	= 21,
	.clk_div	= 2,
	.high_speed	= true,
};

static const struct s3c64xx_spi_port_config s3c6410_spi_port_config = {
	/* fifo_lvl_mask is deprecated. Use {rx, tx}_fifomask instead. */
	.fifo_lvl_mask	= { 0x7f, 0x7F },
	/* rx_lvl_offset is deprecated. Use {rx, tx}_fifomask instead. */
	.rx_lvl_offset	= 13,
	.tx_st_done	= 21,
	.clk_div	= 2,
};

static const struct s3c64xx_spi_port_config s5pv210_spi_port_config = {
	/* fifo_lvl_mask is deprecated. Use {rx, tx}_fifomask instead. */
	.fifo_lvl_mask	= { 0x1ff, 0x7F },
	/* rx_lvl_offset is deprecated. Use {rx, tx}_fifomask instead. */
	.rx_lvl_offset	= 15,
	.tx_st_done	= 25,
	.clk_div	= 2,
	.high_speed	= true,
};

static const struct s3c64xx_spi_port_config exynos4_spi_port_config = {
	/* fifo_lvl_mask is deprecated. Use {rx, tx}_fifomask instead. */
	.fifo_lvl_mask	= { 0x1ff, 0x7F, 0x7F },
	/* rx_lvl_offset is deprecated. Use {rx, tx}_fifomask instead. */
	.rx_lvl_offset	= 15,
	.tx_st_done	= 25,
	.clk_div	= 2,
	.high_speed	= true,
	.clk_from_cmu	= true,
	.quirks		= S3C64XX_SPI_QUIRK_CS_AUTO,
};

static const struct s3c64xx_spi_port_config exynos7_spi_port_config = {
	/* fifo_lvl_mask is deprecated. Use {rx, tx}_fifomask instead. */
	.fifo_lvl_mask	= { 0x1ff, 0x7F, 0x7F, 0x7F, 0x7F, 0x1ff},
	/* rx_lvl_offset is deprecated. Use {rx, tx}_fifomask instead. */
	.rx_lvl_offset	= 15,
	.tx_st_done	= 25,
	.clk_div	= 2,
	.high_speed	= true,
	.clk_from_cmu	= true,
	.quirks		= S3C64XX_SPI_QUIRK_CS_AUTO,
};

static const struct s3c64xx_spi_port_config exynos5433_spi_port_config = {
	/* fifo_lvl_mask is deprecated. Use {rx, tx}_fifomask instead. */
	.fifo_lvl_mask	= { 0x1ff, 0x7f, 0x7f, 0x7f, 0x7f, 0x1ff},
	/* rx_lvl_offset is deprecated. Use {rx, tx}_fifomask instead. */
	.rx_lvl_offset	= 15,
	.tx_st_done	= 25,
	.clk_div	= 2,
	.high_speed	= true,
	.clk_from_cmu	= true,
	.clk_ioclk	= true,
	.quirks		= S3C64XX_SPI_QUIRK_CS_AUTO,
};

static const struct s3c64xx_spi_port_config exynos850_spi_port_config = {
	.fifo_depth	= 64,
	.rx_fifomask	= S3C64XX_SPI_ST_RX_FIFO_RDY_V2,
	.tx_fifomask	= S3C64XX_SPI_ST_TX_FIFO_RDY_V2,
	.tx_st_done	= 25,
	.clk_div	= 4,
	.high_speed	= true,
	.clk_from_cmu	= true,
	.has_loopback	= true,
	.quirks		= S3C64XX_SPI_QUIRK_CS_AUTO,
};

static const struct s3c64xx_spi_port_config exynosautov9_spi_port_config = {
	/* fifo_lvl_mask is deprecated. Use {rx, tx}_fifomask instead. */
	.fifo_lvl_mask	= { 0x1ff, 0x1ff, 0x7f, 0x7f, 0x7f, 0x7f, 0x1ff, 0x7f,
			    0x7f, 0x7f, 0x7f, 0x7f},
	/* rx_lvl_offset is deprecated. Use {rx, tx}_fifomask instead. */
	.rx_lvl_offset	= 15,
	.tx_st_done	= 25,
	.clk_div	= 4,
	.high_speed	= true,
	.clk_from_cmu	= true,
	.clk_ioclk	= true,
	.has_loopback	= true,
	.quirks		= S3C64XX_SPI_QUIRK_CS_AUTO,
};

static const struct s3c64xx_spi_port_config fsd_spi_port_config = {
	/* fifo_lvl_mask is deprecated. Use {rx, tx}_fifomask instead. */
	.fifo_lvl_mask	= { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f},
	/* rx_lvl_offset is deprecated. Use {rx, tx}_fifomask instead. */
	.rx_lvl_offset	= 15,
	.tx_st_done	= 25,
	.clk_div	= 2,
	.high_speed	= true,
	.clk_from_cmu	= true,
	.clk_ioclk	= false,
	.quirks		= S3C64XX_SPI_QUIRK_CS_AUTO,
};

static const struct s3c64xx_spi_port_config gs101_spi_port_config = {
	.fifo_depth	= 64,
	.rx_fifomask	= S3C64XX_SPI_ST_RX_FIFO_RDY_V2,
	.tx_fifomask	= S3C64XX_SPI_ST_TX_FIFO_RDY_V2,
	.tx_st_done	= 25,
	.clk_div	= 4,
	.high_speed	= true,
	.clk_from_cmu	= true,
	.has_loopback	= true,
	.use_32bit_io	= true,
	.quirks		= S3C64XX_SPI_QUIRK_CS_AUTO,
};

static const struct platform_device_id s3c64xx_spi_driver_ids[] = {
	{
		.name		= "s3c2443-spi",
		.driver_data	= (kernel_ulong_t)&s3c2443_spi_port_config,
	}, {
		.name		= "s3c6410-spi",
		.driver_data	= (kernel_ulong_t)&s3c6410_spi_port_config,
	},
	{ },
};

static const struct of_device_id s3c64xx_spi_dt_match[] = {
	{ .compatible = "google,gs101-spi",
			.data = &gs101_spi_port_config,
	},
	{ .compatible = "samsung,s3c2443-spi",
			.data = &s3c2443_spi_port_config,
	},
	{ .compatible = "samsung,s3c6410-spi",
			.data = &s3c6410_spi_port_config,
	},
	{ .compatible = "samsung,s5pv210-spi",
			.data = &s5pv210_spi_port_config,
	},
	{ .compatible = "samsung,exynos4210-spi",
			.data = &exynos4_spi_port_config,
	},
	{ .compatible = "samsung,exynos7-spi",
			.data = &exynos7_spi_port_config,
	},
	{ .compatible = "samsung,exynos5433-spi",
			.data = &exynos5433_spi_port_config,
	},
	{ .compatible = "samsung,exynos850-spi",
			.data = &exynos850_spi_port_config,
	},
	{ .compatible = "samsung,exynosautov9-spi",
			.data = &exynosautov9_spi_port_config,
	},
	{ .compatible = "tesla,fsd-spi",
			.data = &fsd_spi_port_config,
	},
	{ },
};
MODULE_DEVICE_TABLE(of, s3c64xx_spi_dt_match);

static struct platform_driver s3c64xx_spi_driver = {
	.driver = {
		.name	= "s3c64xx-spi",
		.pm = &s3c64xx_spi_pm,
		.of_match_table = of_match_ptr(s3c64xx_spi_dt_match),
	},
	.probe = s3c64xx_spi_probe,
	.remove_new = s3c64xx_spi_remove,
	.id_table = s3c64xx_spi_driver_ids,
};
MODULE_ALIAS("platform:s3c64xx-spi");

module_platform_driver(s3c64xx_spi_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Jaswinder Singh <jassi.brar@samsung.com>");
MODULE_DESCRIPTION("S3C64XX SPI Controller Driver");
MODULE_LICENSE("GPL");
