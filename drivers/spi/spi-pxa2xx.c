<<<<<<< HEAD
/*
 * Copyright (C) 2005 Stephen Street / StreetFire Sound Labs
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
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/spi/pxa2xx_spi.h>
#include <linux/dma-mapping.h>
#include <linux/spi/spi.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/slab.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/delay.h>

=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2005 Stephen Street / StreetFire Sound Labs
 * Copyright (C) 2013, 2021 Intel Corporation
 */

#include <linux/acpi.h>
#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/dmaengine.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/property.h>
#include <linux/slab.h>

#include <linux/spi/pxa2xx_spi.h>
#include <linux/spi/spi.h>

#include "spi-pxa2xx.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Stephen Street");
MODULE_DESCRIPTION("PXA2xx SSP SPI Controller");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:pxa2xx-spi");

<<<<<<< HEAD
#define MAX_BUSES 3

#define TIMOUT_DFLT		1000

#define DMA_INT_MASK		(DCSR_ENDINTR | DCSR_STARTINTR | DCSR_BUSERR)
#define RESET_DMA_CHANNEL	(DCSR_NODESC | DMA_INT_MASK)
#define IS_DMA_ALIGNED(x)	((((u32)(x)) & 0x07) == 0)
#define MAX_DMA_LEN		8191
#define DMA_ALIGNMENT		8

/*
 * for testing SSCR1 changes that require SSP restart, basically
 * everything except the service and interrupt enables, the pxa270 developer
 * manual says only SSCR1_SCFR, SSCR1_SPH, SSCR1_SPO need to be in this
 * list, but the PXA255 dev man says all bits without really meaning the
 * service and interrupt enables
=======
#define TIMOUT_DFLT		1000

/*
 * For testing SSCR1 changes that require SSP restart, basically
 * everything except the service and interrupt enables, the PXA270 developer
 * manual says only SSCR1_SCFR, SSCR1_SPH, SSCR1_SPO need to be in this
 * list, but the PXA255 developer manual says all bits without really meaning
 * the service and interrupt enables.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#define SSCR1_CHANGE_MASK (SSCR1_TTELP | SSCR1_TTE | SSCR1_SCFR \
				| SSCR1_ECRA | SSCR1_ECRB | SSCR1_SCLKDIR \
				| SSCR1_SFRMDIR | SSCR1_RWOT | SSCR1_TRAIL \
				| SSCR1_IFS | SSCR1_STRF | SSCR1_EFWR \
				| SSCR1_RFT | SSCR1_TFT | SSCR1_MWDS \
				| SSCR1_SPH | SSCR1_SPO | SSCR1_LBM)

<<<<<<< HEAD
#define DEFINE_SSP_REG(reg, off) \
static inline u32 read_##reg(void const __iomem *p) \
{ return __raw_readl(p + (off)); } \
\
static inline void write_##reg(u32 v, void __iomem *p) \
{ __raw_writel(v, p + (off)); }

DEFINE_SSP_REG(SSCR0, 0x00)
DEFINE_SSP_REG(SSCR1, 0x04)
DEFINE_SSP_REG(SSSR, 0x08)
DEFINE_SSP_REG(SSITR, 0x0c)
DEFINE_SSP_REG(SSDR, 0x10)
DEFINE_SSP_REG(SSTO, 0x28)
DEFINE_SSP_REG(SSPSP, 0x2c)

#define START_STATE ((void*)0)
#define RUNNING_STATE ((void*)1)
#define DONE_STATE ((void*)2)
#define ERROR_STATE ((void*)-1)

#define QUEUE_RUNNING 0
#define QUEUE_STOPPED 1

struct driver_data {
	/* Driver model hookup */
	struct platform_device *pdev;

	/* SSP Info */
	struct ssp_device *ssp;

	/* SPI framework hookup */
	enum pxa_ssp_type ssp_type;
	struct spi_master *master;

	/* PXA hookup */
	struct pxa2xx_spi_master *master_info;

	/* DMA setup stuff */
	int rx_channel;
	int tx_channel;
	u32 *null_dma_buf;

	/* SSP register addresses */
	void __iomem *ioaddr;
	u32 ssdr_physical;

	/* SSP masks*/
	u32 dma_cr1;
	u32 int_cr1;
	u32 clear_sr;
	u32 mask_sr;

	/* Driver message queue */
	struct workqueue_struct	*workqueue;
	struct work_struct pump_messages;
	spinlock_t lock;
	struct list_head queue;
	int busy;
	int run;

	/* Message Transfer pump */
	struct tasklet_struct pump_transfers;

	/* Current message transfer state info */
	struct spi_message* cur_msg;
	struct spi_transfer* cur_transfer;
	struct chip_data *cur_chip;
	size_t len;
	void *tx;
	void *tx_end;
	void *rx;
	void *rx_end;
	int dma_mapped;
	dma_addr_t rx_dma;
	dma_addr_t tx_dma;
	size_t rx_map_len;
	size_t tx_map_len;
	u8 n_bytes;
	u32 dma_width;
	int (*write)(struct driver_data *drv_data);
	int (*read)(struct driver_data *drv_data);
	irqreturn_t (*transfer_handler)(struct driver_data *drv_data);
	void (*cs_control)(u32 command);
};

struct chip_data {
	u32 cr0;
	u32 cr1;
	u32 psp;
	u32 timeout;
	u8 n_bytes;
	u32 dma_width;
	u32 dma_burst_size;
	u32 threshold;
	u32 dma_threshold;
	u8 enable_dma;
	u8 bits_per_word;
	u32 speed_hz;
	union {
		int gpio_cs;
		unsigned int frm;
	};
	int gpio_cs_inverted;
	int (*write)(struct driver_data *drv_data);
	int (*read)(struct driver_data *drv_data);
	void (*cs_control)(u32 command);
};

static void pump_messages(struct work_struct *work);

static void cs_assert(struct driver_data *drv_data)
{
	struct chip_data *chip = drv_data->cur_chip;

	if (drv_data->ssp_type == CE4100_SSP) {
		write_SSSR(drv_data->cur_chip->frm, drv_data->ioaddr);
		return;
	}

	if (chip->cs_control) {
		chip->cs_control(PXA2XX_CS_ASSERT);
		return;
	}

	if (gpio_is_valid(chip->gpio_cs))
		gpio_set_value(chip->gpio_cs, chip->gpio_cs_inverted);
}

static void cs_deassert(struct driver_data *drv_data)
{
	struct chip_data *chip = drv_data->cur_chip;
=======
#define QUARK_X1000_SSCR1_CHANGE_MASK (QUARK_X1000_SSCR1_STRF	\
				| QUARK_X1000_SSCR1_EFWR	\
				| QUARK_X1000_SSCR1_RFT		\
				| QUARK_X1000_SSCR1_TFT		\
				| SSCR1_SPH | SSCR1_SPO | SSCR1_LBM)

#define CE4100_SSCR1_CHANGE_MASK (SSCR1_TTELP | SSCR1_TTE | SSCR1_SCFR \
				| SSCR1_ECRA | SSCR1_ECRB | SSCR1_SCLKDIR \
				| SSCR1_SFRMDIR | SSCR1_RWOT | SSCR1_TRAIL \
				| SSCR1_IFS | SSCR1_STRF | SSCR1_EFWR \
				| CE4100_SSCR1_RFT | CE4100_SSCR1_TFT | SSCR1_MWDS \
				| SSCR1_SPH | SSCR1_SPO | SSCR1_LBM)

#define LPSS_GENERAL_REG_RXTO_HOLDOFF_DISABLE	BIT(24)
#define LPSS_CS_CONTROL_SW_MODE			BIT(0)
#define LPSS_CS_CONTROL_CS_HIGH			BIT(1)
#define LPSS_CAPS_CS_EN_SHIFT			9
#define LPSS_CAPS_CS_EN_MASK			(0xf << LPSS_CAPS_CS_EN_SHIFT)

#define LPSS_PRIV_CLOCK_GATE 0x38
#define LPSS_PRIV_CLOCK_GATE_CLK_CTL_MASK 0x3
#define LPSS_PRIV_CLOCK_GATE_CLK_CTL_FORCE_ON 0x3

struct lpss_config {
	/* LPSS offset from drv_data->ioaddr */
	unsigned offset;
	/* Register offsets from drv_data->lpss_base or -1 */
	int reg_general;
	int reg_ssp;
	int reg_cs_ctrl;
	int reg_capabilities;
	/* FIFO thresholds */
	u32 rx_threshold;
	u32 tx_threshold_lo;
	u32 tx_threshold_hi;
	/* Chip select control */
	unsigned cs_sel_shift;
	unsigned cs_sel_mask;
	unsigned cs_num;
	/* Quirks */
	unsigned cs_clk_stays_gated : 1;
};

/* Keep these sorted with enum pxa_ssp_type */
static const struct lpss_config lpss_platforms[] = {
	{	/* LPSS_LPT_SSP */
		.offset = 0x800,
		.reg_general = 0x08,
		.reg_ssp = 0x0c,
		.reg_cs_ctrl = 0x18,
		.reg_capabilities = -1,
		.rx_threshold = 64,
		.tx_threshold_lo = 160,
		.tx_threshold_hi = 224,
	},
	{	/* LPSS_BYT_SSP */
		.offset = 0x400,
		.reg_general = 0x08,
		.reg_ssp = 0x0c,
		.reg_cs_ctrl = 0x18,
		.reg_capabilities = -1,
		.rx_threshold = 64,
		.tx_threshold_lo = 160,
		.tx_threshold_hi = 224,
	},
	{	/* LPSS_BSW_SSP */
		.offset = 0x400,
		.reg_general = 0x08,
		.reg_ssp = 0x0c,
		.reg_cs_ctrl = 0x18,
		.reg_capabilities = -1,
		.rx_threshold = 64,
		.tx_threshold_lo = 160,
		.tx_threshold_hi = 224,
		.cs_sel_shift = 2,
		.cs_sel_mask = 1 << 2,
		.cs_num = 2,
	},
	{	/* LPSS_SPT_SSP */
		.offset = 0x200,
		.reg_general = -1,
		.reg_ssp = 0x20,
		.reg_cs_ctrl = 0x24,
		.reg_capabilities = -1,
		.rx_threshold = 1,
		.tx_threshold_lo = 32,
		.tx_threshold_hi = 56,
	},
	{	/* LPSS_BXT_SSP */
		.offset = 0x200,
		.reg_general = -1,
		.reg_ssp = 0x20,
		.reg_cs_ctrl = 0x24,
		.reg_capabilities = 0xfc,
		.rx_threshold = 1,
		.tx_threshold_lo = 16,
		.tx_threshold_hi = 48,
		.cs_sel_shift = 8,
		.cs_sel_mask = 3 << 8,
		.cs_clk_stays_gated = true,
	},
	{	/* LPSS_CNL_SSP */
		.offset = 0x200,
		.reg_general = -1,
		.reg_ssp = 0x20,
		.reg_cs_ctrl = 0x24,
		.reg_capabilities = 0xfc,
		.rx_threshold = 1,
		.tx_threshold_lo = 32,
		.tx_threshold_hi = 56,
		.cs_sel_shift = 8,
		.cs_sel_mask = 3 << 8,
		.cs_clk_stays_gated = true,
	},
};

static inline const struct lpss_config
*lpss_get_config(const struct driver_data *drv_data)
{
	return &lpss_platforms[drv_data->ssp_type - LPSS_LPT_SSP];
}

static bool is_lpss_ssp(const struct driver_data *drv_data)
{
	switch (drv_data->ssp_type) {
	case LPSS_LPT_SSP:
	case LPSS_BYT_SSP:
	case LPSS_BSW_SSP:
	case LPSS_SPT_SSP:
	case LPSS_BXT_SSP:
	case LPSS_CNL_SSP:
		return true;
	default:
		return false;
	}
}

static bool is_quark_x1000_ssp(const struct driver_data *drv_data)
{
	return drv_data->ssp_type == QUARK_X1000_SSP;
}

static bool is_mmp2_ssp(const struct driver_data *drv_data)
{
	return drv_data->ssp_type == MMP2_SSP;
}

static bool is_mrfld_ssp(const struct driver_data *drv_data)
{
	return drv_data->ssp_type == MRFLD_SSP;
}

static void pxa2xx_spi_update(const struct driver_data *drv_data, u32 reg, u32 mask, u32 value)
{
	if ((pxa2xx_spi_read(drv_data, reg) & mask) != value)
		pxa2xx_spi_write(drv_data, reg, value & mask);
}

static u32 pxa2xx_spi_get_ssrc1_change_mask(const struct driver_data *drv_data)
{
	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		return QUARK_X1000_SSCR1_CHANGE_MASK;
	case CE4100_SSP:
		return CE4100_SSCR1_CHANGE_MASK;
	default:
		return SSCR1_CHANGE_MASK;
	}
}

static u32
pxa2xx_spi_get_rx_default_thre(const struct driver_data *drv_data)
{
	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		return RX_THRESH_QUARK_X1000_DFLT;
	case CE4100_SSP:
		return RX_THRESH_CE4100_DFLT;
	default:
		return RX_THRESH_DFLT;
	}
}

static bool pxa2xx_spi_txfifo_full(const struct driver_data *drv_data)
{
	u32 mask;

	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		mask = QUARK_X1000_SSSR_TFL_MASK;
		break;
	case CE4100_SSP:
		mask = CE4100_SSSR_TFL_MASK;
		break;
	default:
		mask = SSSR_TFL_MASK;
		break;
	}

	return read_SSSR_bits(drv_data, mask) == mask;
}

static void pxa2xx_spi_clear_rx_thre(const struct driver_data *drv_data,
				     u32 *sccr1_reg)
{
	u32 mask;

	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		mask = QUARK_X1000_SSCR1_RFT;
		break;
	case CE4100_SSP:
		mask = CE4100_SSCR1_RFT;
		break;
	default:
		mask = SSCR1_RFT;
		break;
	}
	*sccr1_reg &= ~mask;
}

static void pxa2xx_spi_set_rx_thre(const struct driver_data *drv_data,
				   u32 *sccr1_reg, u32 threshold)
{
	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		*sccr1_reg |= QUARK_X1000_SSCR1_RxTresh(threshold);
		break;
	case CE4100_SSP:
		*sccr1_reg |= CE4100_SSCR1_RxTresh(threshold);
		break;
	default:
		*sccr1_reg |= SSCR1_RxTresh(threshold);
		break;
	}
}

static u32 pxa2xx_configure_sscr0(const struct driver_data *drv_data,
				  u32 clk_div, u8 bits)
{
	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		return clk_div
			| QUARK_X1000_SSCR0_Motorola
			| QUARK_X1000_SSCR0_DataSize(bits > 32 ? 8 : bits);
	default:
		return clk_div
			| SSCR0_Motorola
			| SSCR0_DataSize(bits > 16 ? bits - 16 : bits)
			| (bits > 16 ? SSCR0_EDSS : 0);
	}
}

/*
 * Read and write LPSS SSP private registers. Caller must first check that
 * is_lpss_ssp() returns true before these can be called.
 */
static u32 __lpss_ssp_read_priv(struct driver_data *drv_data, unsigned offset)
{
	WARN_ON(!drv_data->lpss_base);
	return readl(drv_data->lpss_base + offset);
}

static void __lpss_ssp_write_priv(struct driver_data *drv_data,
				  unsigned offset, u32 value)
{
	WARN_ON(!drv_data->lpss_base);
	writel(value, drv_data->lpss_base + offset);
}

/*
 * lpss_ssp_setup - perform LPSS SSP specific setup
 * @drv_data: pointer to the driver private data
 *
 * Perform LPSS SSP specific setup. This function must be called first if
 * one is going to use LPSS SSP private registers.
 */
static void lpss_ssp_setup(struct driver_data *drv_data)
{
	const struct lpss_config *config;
	u32 value;

	config = lpss_get_config(drv_data);
	drv_data->lpss_base = drv_data->ssp->mmio_base + config->offset;

	/* Enable software chip select control */
	value = __lpss_ssp_read_priv(drv_data, config->reg_cs_ctrl);
	value &= ~(LPSS_CS_CONTROL_SW_MODE | LPSS_CS_CONTROL_CS_HIGH);
	value |= LPSS_CS_CONTROL_SW_MODE | LPSS_CS_CONTROL_CS_HIGH;
	__lpss_ssp_write_priv(drv_data, config->reg_cs_ctrl, value);

	/* Enable multiblock DMA transfers */
	if (drv_data->controller_info->enable_dma) {
		__lpss_ssp_write_priv(drv_data, config->reg_ssp, 1);

		if (config->reg_general >= 0) {
			value = __lpss_ssp_read_priv(drv_data,
						     config->reg_general);
			value |= LPSS_GENERAL_REG_RXTO_HOLDOFF_DISABLE;
			__lpss_ssp_write_priv(drv_data,
					      config->reg_general, value);
		}
	}
}

static void lpss_ssp_select_cs(struct spi_device *spi,
			       const struct lpss_config *config)
{
	struct driver_data *drv_data =
		spi_controller_get_devdata(spi->controller);
	u32 value, cs;

	if (!config->cs_sel_mask)
		return;

	value = __lpss_ssp_read_priv(drv_data, config->reg_cs_ctrl);

	cs = spi_get_chipselect(spi, 0);
	cs <<= config->cs_sel_shift;
	if (cs != (value & config->cs_sel_mask)) {
		/*
		 * When switching another chip select output active the
		 * output must be selected first and wait 2 ssp_clk cycles
		 * before changing state to active. Otherwise a short
		 * glitch will occur on the previous chip select since
		 * output select is latched but state control is not.
		 */
		value &= ~config->cs_sel_mask;
		value |= cs;
		__lpss_ssp_write_priv(drv_data,
				      config->reg_cs_ctrl, value);
		ndelay(1000000000 /
		       (drv_data->controller->max_speed_hz / 2));
	}
}

static void lpss_ssp_cs_control(struct spi_device *spi, bool enable)
{
	struct driver_data *drv_data =
		spi_controller_get_devdata(spi->controller);
	const struct lpss_config *config;
	u32 value;

	config = lpss_get_config(drv_data);

	if (enable)
		lpss_ssp_select_cs(spi, config);

	value = __lpss_ssp_read_priv(drv_data, config->reg_cs_ctrl);
	if (enable)
		value &= ~LPSS_CS_CONTROL_CS_HIGH;
	else
		value |= LPSS_CS_CONTROL_CS_HIGH;
	__lpss_ssp_write_priv(drv_data, config->reg_cs_ctrl, value);
	if (config->cs_clk_stays_gated) {
		u32 clkgate;

		/*
		 * Changing CS alone when dynamic clock gating is on won't
		 * actually flip CS at that time. This ruins SPI transfers
		 * that specify delays, or have no data. Toggle the clock mode
		 * to force on briefly to poke the CS pin to move.
		 */
		clkgate = __lpss_ssp_read_priv(drv_data, LPSS_PRIV_CLOCK_GATE);
		value = (clkgate & ~LPSS_PRIV_CLOCK_GATE_CLK_CTL_MASK) |
			LPSS_PRIV_CLOCK_GATE_CLK_CTL_FORCE_ON;

		__lpss_ssp_write_priv(drv_data, LPSS_PRIV_CLOCK_GATE, value);
		__lpss_ssp_write_priv(drv_data, LPSS_PRIV_CLOCK_GATE, clkgate);
	}
}

static void cs_assert(struct spi_device *spi)
{
	struct driver_data *drv_data =
		spi_controller_get_devdata(spi->controller);

	if (drv_data->ssp_type == CE4100_SSP) {
		pxa2xx_spi_write(drv_data, SSSR, spi_get_chipselect(spi, 0));
		return;
	}

	if (is_lpss_ssp(drv_data))
		lpss_ssp_cs_control(spi, true);
}

static void cs_deassert(struct spi_device *spi)
{
	struct driver_data *drv_data =
		spi_controller_get_devdata(spi->controller);
	unsigned long timeout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (drv_data->ssp_type == CE4100_SSP)
		return;

<<<<<<< HEAD
	if (chip->cs_control) {
		chip->cs_control(PXA2XX_CS_DEASSERT);
		return;
	}

	if (gpio_is_valid(chip->gpio_cs))
		gpio_set_value(chip->gpio_cs, !chip->gpio_cs_inverted);
}

static void write_SSSR_CS(struct driver_data *drv_data, u32 val)
{
	void __iomem *reg = drv_data->ioaddr;

	if (drv_data->ssp_type == CE4100_SSP)
		val |= read_SSSR(reg) & SSSR_ALT_FRM_MASK;

	write_SSSR(val, reg);
}

static int pxa25x_ssp_comp(struct driver_data *drv_data)
{
	if (drv_data->ssp_type == PXA25x_SSP)
		return 1;
	if (drv_data->ssp_type == CE4100_SSP)
		return 1;
	return 0;
}

static int flush(struct driver_data *drv_data)
{
	unsigned long limit = loops_per_jiffy << 1;

	void __iomem *reg = drv_data->ioaddr;

	do {
		while (read_SSSR(reg) & SSSR_RNE) {
			read_SSDR(reg);
		}
	} while ((read_SSSR(reg) & SSSR_BSY) && --limit);
=======
	/* Wait until SSP becomes idle before deasserting the CS */
	timeout = jiffies + msecs_to_jiffies(10);
	while (pxa2xx_spi_read(drv_data, SSSR) & SSSR_BSY &&
	       !time_after(jiffies, timeout))
		cpu_relax();

	if (is_lpss_ssp(drv_data))
		lpss_ssp_cs_control(spi, false);
}

static void pxa2xx_spi_set_cs(struct spi_device *spi, bool level)
{
	if (level)
		cs_deassert(spi);
	else
		cs_assert(spi);
}

int pxa2xx_spi_flush(struct driver_data *drv_data)
{
	unsigned long limit = loops_per_jiffy << 1;

	do {
		while (read_SSSR_bits(drv_data, SSSR_RNE))
			pxa2xx_spi_read(drv_data, SSDR);
	} while ((pxa2xx_spi_read(drv_data, SSSR) & SSSR_BSY) && --limit);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	write_SSSR_CS(drv_data, SSSR_ROR);

	return limit;
}

<<<<<<< HEAD
static int null_writer(struct driver_data *drv_data)
{
	void __iomem *reg = drv_data->ioaddr;
	u8 n_bytes = drv_data->n_bytes;

	if (((read_SSSR(reg) & SSSR_TFL_MASK) == SSSR_TFL_MASK)
		|| (drv_data->tx == drv_data->tx_end))
		return 0;

	write_SSDR(0, reg);
=======
static void pxa2xx_spi_off(struct driver_data *drv_data)
{
	/* On MMP, disabling SSE seems to corrupt the Rx FIFO */
	if (is_mmp2_ssp(drv_data))
		return;

	pxa_ssp_disable(drv_data->ssp);
}

static int null_writer(struct driver_data *drv_data)
{
	u8 n_bytes = drv_data->n_bytes;

	if (pxa2xx_spi_txfifo_full(drv_data)
		|| (drv_data->tx == drv_data->tx_end))
		return 0;

	pxa2xx_spi_write(drv_data, SSDR, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	drv_data->tx += n_bytes;

	return 1;
}

static int null_reader(struct driver_data *drv_data)
{
<<<<<<< HEAD
	void __iomem *reg = drv_data->ioaddr;
	u8 n_bytes = drv_data->n_bytes;

	while ((read_SSSR(reg) & SSSR_RNE)
		&& (drv_data->rx < drv_data->rx_end)) {
		read_SSDR(reg);
=======
	u8 n_bytes = drv_data->n_bytes;

	while (read_SSSR_bits(drv_data, SSSR_RNE) && drv_data->rx < drv_data->rx_end) {
		pxa2xx_spi_read(drv_data, SSDR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		drv_data->rx += n_bytes;
	}

	return drv_data->rx == drv_data->rx_end;
}

static int u8_writer(struct driver_data *drv_data)
{
<<<<<<< HEAD
	void __iomem *reg = drv_data->ioaddr;

	if (((read_SSSR(reg) & SSSR_TFL_MASK) == SSSR_TFL_MASK)
		|| (drv_data->tx == drv_data->tx_end))
		return 0;

	write_SSDR(*(u8 *)(drv_data->tx), reg);
=======
	if (pxa2xx_spi_txfifo_full(drv_data)
		|| (drv_data->tx == drv_data->tx_end))
		return 0;

	pxa2xx_spi_write(drv_data, SSDR, *(u8 *)(drv_data->tx));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	++drv_data->tx;

	return 1;
}

static int u8_reader(struct driver_data *drv_data)
{
<<<<<<< HEAD
	void __iomem *reg = drv_data->ioaddr;

	while ((read_SSSR(reg) & SSSR_RNE)
		&& (drv_data->rx < drv_data->rx_end)) {
		*(u8 *)(drv_data->rx) = read_SSDR(reg);
=======
	while (read_SSSR_bits(drv_data, SSSR_RNE) && drv_data->rx < drv_data->rx_end) {
		*(u8 *)(drv_data->rx) = pxa2xx_spi_read(drv_data, SSDR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		++drv_data->rx;
	}

	return drv_data->rx == drv_data->rx_end;
}

static int u16_writer(struct driver_data *drv_data)
{
<<<<<<< HEAD
	void __iomem *reg = drv_data->ioaddr;

	if (((read_SSSR(reg) & SSSR_TFL_MASK) == SSSR_TFL_MASK)
		|| (drv_data->tx == drv_data->tx_end))
		return 0;

	write_SSDR(*(u16 *)(drv_data->tx), reg);
=======
	if (pxa2xx_spi_txfifo_full(drv_data)
		|| (drv_data->tx == drv_data->tx_end))
		return 0;

	pxa2xx_spi_write(drv_data, SSDR, *(u16 *)(drv_data->tx));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	drv_data->tx += 2;

	return 1;
}

static int u16_reader(struct driver_data *drv_data)
{
<<<<<<< HEAD
	void __iomem *reg = drv_data->ioaddr;

	while ((read_SSSR(reg) & SSSR_RNE)
		&& (drv_data->rx < drv_data->rx_end)) {
		*(u16 *)(drv_data->rx) = read_SSDR(reg);
=======
	while (read_SSSR_bits(drv_data, SSSR_RNE) && drv_data->rx < drv_data->rx_end) {
		*(u16 *)(drv_data->rx) = pxa2xx_spi_read(drv_data, SSDR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		drv_data->rx += 2;
	}

	return drv_data->rx == drv_data->rx_end;
}

static int u32_writer(struct driver_data *drv_data)
{
<<<<<<< HEAD
	void __iomem *reg = drv_data->ioaddr;

	if (((read_SSSR(reg) & SSSR_TFL_MASK) == SSSR_TFL_MASK)
		|| (drv_data->tx == drv_data->tx_end))
		return 0;

	write_SSDR(*(u32 *)(drv_data->tx), reg);
=======
	if (pxa2xx_spi_txfifo_full(drv_data)
		|| (drv_data->tx == drv_data->tx_end))
		return 0;

	pxa2xx_spi_write(drv_data, SSDR, *(u32 *)(drv_data->tx));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	drv_data->tx += 4;

	return 1;
}

static int u32_reader(struct driver_data *drv_data)
{
<<<<<<< HEAD
	void __iomem *reg = drv_data->ioaddr;

	while ((read_SSSR(reg) & SSSR_RNE)
		&& (drv_data->rx < drv_data->rx_end)) {
		*(u32 *)(drv_data->rx) = read_SSDR(reg);
=======
	while (read_SSSR_bits(drv_data, SSSR_RNE) && drv_data->rx < drv_data->rx_end) {
		*(u32 *)(drv_data->rx) = pxa2xx_spi_read(drv_data, SSDR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		drv_data->rx += 4;
	}

	return drv_data->rx == drv_data->rx_end;
}

<<<<<<< HEAD
static void *next_transfer(struct driver_data *drv_data)
{
	struct spi_message *msg = drv_data->cur_msg;
	struct spi_transfer *trans = drv_data->cur_transfer;

	/* Move to next transfer */
	if (trans->transfer_list.next != &msg->transfers) {
		drv_data->cur_transfer =
			list_entry(trans->transfer_list.next,
					struct spi_transfer,
					transfer_list);
		return RUNNING_STATE;
	} else
		return DONE_STATE;
}

static int map_dma_buffers(struct driver_data *drv_data)
{
	struct spi_message *msg = drv_data->cur_msg;
	struct device *dev = &msg->spi->dev;

	if (!drv_data->cur_chip->enable_dma)
		return 0;

	if (msg->is_dma_mapped)
		return  drv_data->rx_dma && drv_data->tx_dma;

	if (!IS_DMA_ALIGNED(drv_data->rx) || !IS_DMA_ALIGNED(drv_data->tx))
		return 0;

	/* Modify setup if rx buffer is null */
	if (drv_data->rx == NULL) {
		*drv_data->null_dma_buf = 0;
		drv_data->rx = drv_data->null_dma_buf;
		drv_data->rx_map_len = 4;
	} else
		drv_data->rx_map_len = drv_data->len;


	/* Modify setup if tx buffer is null */
	if (drv_data->tx == NULL) {
		*drv_data->null_dma_buf = 0;
		drv_data->tx = drv_data->null_dma_buf;
		drv_data->tx_map_len = 4;
	} else
		drv_data->tx_map_len = drv_data->len;

	/* Stream map the tx buffer. Always do DMA_TO_DEVICE first
	 * so we flush the cache *before* invalidating it, in case
	 * the tx and rx buffers overlap.
	 */
	drv_data->tx_dma = dma_map_single(dev, drv_data->tx,
					drv_data->tx_map_len, DMA_TO_DEVICE);
	if (dma_mapping_error(dev, drv_data->tx_dma))
		return 0;

	/* Stream map the rx buffer */
	drv_data->rx_dma = dma_map_single(dev, drv_data->rx,
					drv_data->rx_map_len, DMA_FROM_DEVICE);
	if (dma_mapping_error(dev, drv_data->rx_dma)) {
		dma_unmap_single(dev, drv_data->tx_dma,
					drv_data->tx_map_len, DMA_TO_DEVICE);
		return 0;
	}

	return 1;
}

static void unmap_dma_buffers(struct driver_data *drv_data)
{
	struct device *dev;

	if (!drv_data->dma_mapped)
		return;

	if (!drv_data->cur_msg->is_dma_mapped) {
		dev = &drv_data->cur_msg->spi->dev;
		dma_unmap_single(dev, drv_data->rx_dma,
					drv_data->rx_map_len, DMA_FROM_DEVICE);
		dma_unmap_single(dev, drv_data->tx_dma,
					drv_data->tx_map_len, DMA_TO_DEVICE);
	}

	drv_data->dma_mapped = 0;
}

/* caller already set message->status; dma and pio irqs are blocked */
static void giveback(struct driver_data *drv_data)
{
	struct spi_transfer* last_transfer;
	unsigned long flags;
	struct spi_message *msg;

	spin_lock_irqsave(&drv_data->lock, flags);
	msg = drv_data->cur_msg;
	drv_data->cur_msg = NULL;
	drv_data->cur_transfer = NULL;
	queue_work(drv_data->workqueue, &drv_data->pump_messages);
	spin_unlock_irqrestore(&drv_data->lock, flags);

	last_transfer = list_entry(msg->transfers.prev,
					struct spi_transfer,
					transfer_list);

	/* Delay if requested before any change in chip select */
	if (last_transfer->delay_usecs)
		udelay(last_transfer->delay_usecs);

	/* Drop chip select UNLESS cs_change is true or we are returning
	 * a message with an error, or next message is for another chip
	 */
	if (!last_transfer->cs_change)
		cs_deassert(drv_data);
	else {
		struct spi_message *next_msg;

		/* Holding of cs was hinted, but we need to make sure
		 * the next message is for the same chip.  Don't waste
		 * time with the following tests unless this was hinted.
		 *
		 * We cannot postpone this until pump_messages, because
		 * after calling msg->complete (below) the driver that
		 * sent the current message could be unloaded, which
		 * could invalidate the cs_control() callback...
		 */

		/* get a pointer to the next message, if any */
		spin_lock_irqsave(&drv_data->lock, flags);
		if (list_empty(&drv_data->queue))
			next_msg = NULL;
		else
			next_msg = list_entry(drv_data->queue.next,
					struct spi_message, queue);
		spin_unlock_irqrestore(&drv_data->lock, flags);

		/* see if the next and current messages point
		 * to the same chip
		 */
		if (next_msg && next_msg->spi != msg->spi)
			next_msg = NULL;
		if (!next_msg || msg->state == ERROR_STATE)
			cs_deassert(drv_data);
	}

	msg->state = NULL;
	if (msg->complete)
		msg->complete(msg->context);

	drv_data->cur_chip = NULL;
}

static int wait_ssp_rx_stall(void const __iomem *ioaddr)
{
	unsigned long limit = loops_per_jiffy << 1;

	while ((read_SSSR(ioaddr) & SSSR_BSY) && --limit)
		cpu_relax();

	return limit;
}

static int wait_dma_channel_stop(int channel)
{
	unsigned long limit = loops_per_jiffy << 1;

	while (!(DCSR(channel) & DCSR_STOPSTATE) && --limit)
		cpu_relax();

	return limit;
}

static void dma_error_stop(struct driver_data *drv_data, const char *msg)
{
	void __iomem *reg = drv_data->ioaddr;

	/* Stop and reset */
	DCSR(drv_data->rx_channel) = RESET_DMA_CHANNEL;
	DCSR(drv_data->tx_channel) = RESET_DMA_CHANNEL;
	write_SSSR_CS(drv_data, drv_data->clear_sr);
	write_SSCR1(read_SSCR1(reg) & ~drv_data->dma_cr1, reg);
	if (!pxa25x_ssp_comp(drv_data))
		write_SSTO(0, reg);
	flush(drv_data);
	write_SSCR0(read_SSCR0(reg) & ~SSCR0_SSE, reg);

	unmap_dma_buffers(drv_data);

	dev_err(&drv_data->pdev->dev, "%s\n", msg);

	drv_data->cur_msg->state = ERROR_STATE;
	tasklet_schedule(&drv_data->pump_transfers);
}

static void dma_transfer_complete(struct driver_data *drv_data)
{
	void __iomem *reg = drv_data->ioaddr;
	struct spi_message *msg = drv_data->cur_msg;

	/* Clear and disable interrupts on SSP and DMA channels*/
	write_SSCR1(read_SSCR1(reg) & ~drv_data->dma_cr1, reg);
	write_SSSR_CS(drv_data, drv_data->clear_sr);
	DCSR(drv_data->tx_channel) = RESET_DMA_CHANNEL;
	DCSR(drv_data->rx_channel) = RESET_DMA_CHANNEL;

	if (wait_dma_channel_stop(drv_data->rx_channel) == 0)
		dev_err(&drv_data->pdev->dev,
			"dma_handler: dma rx channel stop failed\n");

	if (wait_ssp_rx_stall(drv_data->ioaddr) == 0)
		dev_err(&drv_data->pdev->dev,
			"dma_transfer: ssp rx stall failed\n");

	unmap_dma_buffers(drv_data);

	/* update the buffer pointer for the amount completed in dma */
	drv_data->rx += drv_data->len -
			(DCMD(drv_data->rx_channel) & DCMD_LENGTH);

	/* read trailing data from fifo, it does not matter how many
	 * bytes are in the fifo just read until buffer is full
	 * or fifo is empty, which ever occurs first */
	drv_data->read(drv_data);

	/* return count of what was actually read */
	msg->actual_length += drv_data->len -
				(drv_data->rx_end - drv_data->rx);

	/* Transfer delays and chip select release are
	 * handled in pump_transfers or giveback
	 */

	/* Move to next transfer */
	msg->state = next_transfer(drv_data);

	/* Schedule transfer tasklet */
	tasklet_schedule(&drv_data->pump_transfers);
}

static void dma_handler(int channel, void *data)
{
	struct driver_data *drv_data = data;
	u32 irq_status = DCSR(channel) & DMA_INT_MASK;

	if (irq_status & DCSR_BUSERR) {

		if (channel == drv_data->tx_channel)
			dma_error_stop(drv_data,
					"dma_handler: "
					"bad bus address on tx channel");
		else
			dma_error_stop(drv_data,
					"dma_handler: "
					"bad bus address on rx channel");
		return;
	}

	/* PXA255x_SSP has no timeout interrupt, wait for tailing bytes */
	if ((channel == drv_data->tx_channel)
		&& (irq_status & DCSR_ENDINTR)
		&& (drv_data->ssp_type == PXA25x_SSP)) {

		/* Wait for rx to stall */
		if (wait_ssp_rx_stall(drv_data->ioaddr) == 0)
			dev_err(&drv_data->pdev->dev,
				"dma_handler: ssp rx stall failed\n");

		/* finish this transfer, start the next */
		dma_transfer_complete(drv_data);
	}
}

static irqreturn_t dma_transfer(struct driver_data *drv_data)
{
	u32 irq_status;
	void __iomem *reg = drv_data->ioaddr;

	irq_status = read_SSSR(reg) & drv_data->mask_sr;
	if (irq_status & SSSR_ROR) {
		dma_error_stop(drv_data, "dma_transfer: fifo overrun");
		return IRQ_HANDLED;
	}

	/* Check for false positive timeout */
	if ((irq_status & SSSR_TINT)
		&& (DCSR(drv_data->tx_channel) & DCSR_RUN)) {
		write_SSSR(SSSR_TINT, reg);
		return IRQ_HANDLED;
	}

	if (irq_status & SSSR_TINT || drv_data->rx == drv_data->rx_end) {

		/* Clear and disable timeout interrupt, do the rest in
		 * dma_transfer_complete */
		if (!pxa25x_ssp_comp(drv_data))
			write_SSTO(0, reg);

		/* finish this transfer, start the next */
		dma_transfer_complete(drv_data);

		return IRQ_HANDLED;
	}

	/* Opps problem detected */
	return IRQ_NONE;
}

static void reset_sccr1(struct driver_data *drv_data)
{
	void __iomem *reg = drv_data->ioaddr;
	struct chip_data *chip = drv_data->cur_chip;
	u32 sccr1_reg;

	sccr1_reg = read_SSCR1(reg) & ~drv_data->int_cr1;
	sccr1_reg &= ~SSCR1_RFT;
	sccr1_reg |= chip->threshold;
	write_SSCR1(sccr1_reg, reg);
}

static void int_error_stop(struct driver_data *drv_data, const char* msg)
{
	void __iomem *reg = drv_data->ioaddr;

	/* Stop and reset SSP */
	write_SSSR_CS(drv_data, drv_data->clear_sr);
	reset_sccr1(drv_data);
	if (!pxa25x_ssp_comp(drv_data))
		write_SSTO(0, reg);
	flush(drv_data);
	write_SSCR0(read_SSCR0(reg) & ~SSCR0_SSE, reg);

	dev_err(&drv_data->pdev->dev, "%s\n", msg);

	drv_data->cur_msg->state = ERROR_STATE;
	tasklet_schedule(&drv_data->pump_transfers);
=======
static void reset_sccr1(struct driver_data *drv_data)
{
	u32 mask = drv_data->int_cr1 | drv_data->dma_cr1, threshold;
	struct chip_data *chip;

	if (drv_data->controller->cur_msg) {
		chip = spi_get_ctldata(drv_data->controller->cur_msg->spi);
		threshold = chip->threshold;
	} else {
		threshold = 0;
	}

	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		mask |= QUARK_X1000_SSCR1_RFT;
		break;
	case CE4100_SSP:
		mask |= CE4100_SSCR1_RFT;
		break;
	default:
		mask |= SSCR1_RFT;
		break;
	}

	pxa2xx_spi_update(drv_data, SSCR1, mask, threshold);
}

static void int_stop_and_reset(struct driver_data *drv_data)
{
	/* Clear and disable interrupts */
	write_SSSR_CS(drv_data, drv_data->clear_sr);
	reset_sccr1(drv_data);
	if (pxa25x_ssp_comp(drv_data))
		return;

	pxa2xx_spi_write(drv_data, SSTO, 0);
}

static void int_error_stop(struct driver_data *drv_data, const char *msg, int err)
{
	int_stop_and_reset(drv_data);
	pxa2xx_spi_flush(drv_data);
	pxa2xx_spi_off(drv_data);

	dev_err(drv_data->ssp->dev, "%s\n", msg);

	drv_data->controller->cur_msg->status = err;
	spi_finalize_current_transfer(drv_data->controller);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void int_transfer_complete(struct driver_data *drv_data)
{
<<<<<<< HEAD
	void __iomem *reg = drv_data->ioaddr;

	/* Stop SSP */
	write_SSSR_CS(drv_data, drv_data->clear_sr);
	reset_sccr1(drv_data);
	if (!pxa25x_ssp_comp(drv_data))
		write_SSTO(0, reg);

	/* Update total byte transferred return count actual bytes read */
	drv_data->cur_msg->actual_length += drv_data->len -
				(drv_data->rx_end - drv_data->rx);

	/* Transfer delays and chip select release are
	 * handled in pump_transfers or giveback
	 */

	/* Move to next transfer */
	drv_data->cur_msg->state = next_transfer(drv_data);

	/* Schedule transfer tasklet */
	tasklet_schedule(&drv_data->pump_transfers);
=======
	int_stop_and_reset(drv_data);

	spi_finalize_current_transfer(drv_data->controller);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static irqreturn_t interrupt_transfer(struct driver_data *drv_data)
{
<<<<<<< HEAD
	void __iomem *reg = drv_data->ioaddr;

	u32 irq_mask = (read_SSCR1(reg) & SSCR1_TIE) ?
			drv_data->mask_sr : drv_data->mask_sr & ~SSSR_TFS;

	u32 irq_status = read_SSSR(reg) & irq_mask;

	if (irq_status & SSSR_ROR) {
		int_error_stop(drv_data, "interrupt_transfer: fifo overrun");
=======
	u32 irq_status;

	irq_status = read_SSSR_bits(drv_data, drv_data->mask_sr);
	if (!(pxa2xx_spi_read(drv_data, SSCR1) & SSCR1_TIE))
		irq_status &= ~SSSR_TFS;

	if (irq_status & SSSR_ROR) {
		int_error_stop(drv_data, "interrupt_transfer: FIFO overrun", -EIO);
		return IRQ_HANDLED;
	}

	if (irq_status & SSSR_TUR) {
		int_error_stop(drv_data, "interrupt_transfer: FIFO underrun", -EIO);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return IRQ_HANDLED;
	}

	if (irq_status & SSSR_TINT) {
<<<<<<< HEAD
		write_SSSR(SSSR_TINT, reg);
=======
		pxa2xx_spi_write(drv_data, SSSR, SSSR_TINT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (drv_data->read(drv_data)) {
			int_transfer_complete(drv_data);
			return IRQ_HANDLED;
		}
	}

<<<<<<< HEAD
	/* Drain rx fifo, Fill tx fifo and prevent overruns */
=======
	/* Drain Rx FIFO, Fill Tx FIFO and prevent overruns */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	do {
		if (drv_data->read(drv_data)) {
			int_transfer_complete(drv_data);
			return IRQ_HANDLED;
		}
	} while (drv_data->write(drv_data));

	if (drv_data->read(drv_data)) {
		int_transfer_complete(drv_data);
		return IRQ_HANDLED;
	}

	if (drv_data->tx == drv_data->tx_end) {
		u32 bytes_left;
		u32 sccr1_reg;

<<<<<<< HEAD
		sccr1_reg = read_SSCR1(reg);
		sccr1_reg &= ~SSCR1_TIE;

		/*
		 * PXA25x_SSP has no timeout, set up rx threshould for the
		 * remaining RX bytes.
		 */
		if (pxa25x_ssp_comp(drv_data)) {

			sccr1_reg &= ~SSCR1_RFT;
=======
		sccr1_reg = pxa2xx_spi_read(drv_data, SSCR1);
		sccr1_reg &= ~SSCR1_TIE;

		/*
		 * PXA25x_SSP has no timeout, set up Rx threshold for
		 * the remaining Rx bytes.
		 */
		if (pxa25x_ssp_comp(drv_data)) {
			u32 rx_thre;

			pxa2xx_spi_clear_rx_thre(drv_data, &sccr1_reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			bytes_left = drv_data->rx_end - drv_data->rx;
			switch (drv_data->n_bytes) {
			case 4:
<<<<<<< HEAD
				bytes_left >>= 1;
			case 2:
				bytes_left >>= 1;
			}

			if (bytes_left > RX_THRESH_DFLT)
				bytes_left = RX_THRESH_DFLT;

			sccr1_reg |= SSCR1_RxTresh(bytes_left);
		}
		write_SSCR1(sccr1_reg, reg);
=======
				bytes_left >>= 2;
				break;
			case 2:
				bytes_left >>= 1;
				break;
			}

			rx_thre = pxa2xx_spi_get_rx_default_thre(drv_data);
			if (rx_thre > bytes_left)
				rx_thre = bytes_left;

			pxa2xx_spi_set_rx_thre(drv_data, &sccr1_reg, rx_thre);
		}
		pxa2xx_spi_write(drv_data, SSCR1, sccr1_reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* We did something */
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static irqreturn_t ssp_int(int irq, void *dev_id)
{
	struct driver_data *drv_data = dev_id;
	void __iomem *reg = drv_data->ioaddr;
	u32 sccr1_reg = read_SSCR1(reg);
	u32 mask = drv_data->mask_sr;
	u32 status;

	status = read_SSSR(reg);
=======
static void handle_bad_msg(struct driver_data *drv_data)
{
	int_stop_and_reset(drv_data);
	pxa2xx_spi_off(drv_data);

	dev_err(drv_data->ssp->dev, "bad message state in interrupt handler\n");
}

static irqreturn_t ssp_int(int irq, void *dev_id)
{
	struct driver_data *drv_data = dev_id;
	u32 sccr1_reg;
	u32 mask = drv_data->mask_sr;
	u32 status;

	/*
	 * The IRQ might be shared with other peripherals so we must first
	 * check that are we RPM suspended or not. If we are we assume that
	 * the IRQ was not for us (we shouldn't be RPM suspended when the
	 * interrupt is enabled).
	 */
	if (pm_runtime_suspended(drv_data->ssp->dev))
		return IRQ_NONE;

	/*
	 * If the device is not yet in RPM suspended state and we get an
	 * interrupt that is meant for another device, check if status bits
	 * are all set to one. That means that the device is already
	 * powered off.
	 */
	status = pxa2xx_spi_read(drv_data, SSSR);
	if (status == ~0)
		return IRQ_NONE;

	sccr1_reg = pxa2xx_spi_read(drv_data, SSCR1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Ignore possible writes if we don't need to write */
	if (!(sccr1_reg & SSCR1_TIE))
		mask &= ~SSSR_TFS;

	/* Ignore RX timeout interrupt if it is disabled */
	if (!(sccr1_reg & SSCR1_TINTE))
		mask &= ~SSSR_TINT;

	if (!(status & mask))
		return IRQ_NONE;

<<<<<<< HEAD
	if (!drv_data->cur_msg) {

		write_SSCR0(read_SSCR0(reg) & ~SSCR0_SSE, reg);
		write_SSCR1(read_SSCR1(reg) & ~drv_data->int_cr1, reg);
		if (!pxa25x_ssp_comp(drv_data))
			write_SSTO(0, reg);
		write_SSSR_CS(drv_data, drv_data->clear_sr);

		dev_err(&drv_data->pdev->dev, "bad message state "
			"in interrupt handler\n");

=======
	pxa2xx_spi_write(drv_data, SSCR1, sccr1_reg & ~drv_data->int_cr1);
	pxa2xx_spi_write(drv_data, SSCR1, sccr1_reg);

	if (!drv_data->controller->cur_msg) {
		handle_bad_msg(drv_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Never fail */
		return IRQ_HANDLED;
	}

	return drv_data->transfer_handler(drv_data);
}

<<<<<<< HEAD
static int set_dma_burst_and_threshold(struct chip_data *chip,
				struct spi_device *spi,
				u8 bits_per_word, u32 *burst_code,
				u32 *threshold)
{
	struct pxa2xx_spi_chip *chip_info =
			(struct pxa2xx_spi_chip *)spi->controller_data;
	int bytes_per_word;
	int burst_bytes;
	int thresh_words;
	int req_burst_size;
	int retval = 0;

	/* Set the threshold (in registers) to equal the same amount of data
	 * as represented by burst size (in bytes).  The computation below
	 * is (burst_size rounded up to nearest 8 byte, word or long word)
	 * divided by (bytes/register); the tx threshold is the inverse of
	 * the rx, so that there will always be enough data in the rx fifo
	 * to satisfy a burst, and there will always be enough space in the
	 * tx fifo to accept a burst (a tx burst will overwrite the fifo if
	 * there is not enough space), there must always remain enough empty
	 * space in the rx fifo for any data loaded to the tx fifo.
	 * Whenever burst_size (in bytes) equals bits/word, the fifo threshold
	 * will be 8, or half the fifo;
	 * The threshold can only be set to 2, 4 or 8, but not 16, because
	 * to burst 16 to the tx fifo, the fifo would have to be empty;
	 * however, the minimum fifo trigger level is 1, and the tx will
	 * request service when the fifo is at this level, with only 15 spaces.
	 */

	/* find bytes/word */
	if (bits_per_word <= 8)
		bytes_per_word = 1;
	else if (bits_per_word <= 16)
		bytes_per_word = 2;
	else
		bytes_per_word = 4;

	/* use struct pxa2xx_spi_chip->dma_burst_size if available */
	if (chip_info)
		req_burst_size = chip_info->dma_burst_size;
	else {
		switch (chip->dma_burst_size) {
		default:
			/* if the default burst size is not set,
			 * do it now */
			chip->dma_burst_size = DCMD_BURST8;
		case DCMD_BURST8:
			req_burst_size = 8;
			break;
		case DCMD_BURST16:
			req_burst_size = 16;
			break;
		case DCMD_BURST32:
			req_burst_size = 32;
			break;
		}
	}
	if (req_burst_size <= 8) {
		*burst_code = DCMD_BURST8;
		burst_bytes = 8;
	} else if (req_burst_size <= 16) {
		if (bytes_per_word == 1) {
			/* don't burst more than 1/2 the fifo */
			*burst_code = DCMD_BURST8;
			burst_bytes = 8;
			retval = 1;
		} else {
			*burst_code = DCMD_BURST16;
			burst_bytes = 16;
		}
	} else {
		if (bytes_per_word == 1) {
			/* don't burst more than 1/2 the fifo */
			*burst_code = DCMD_BURST8;
			burst_bytes = 8;
			retval = 1;
		} else if (bytes_per_word == 2) {
			/* don't burst more than 1/2 the fifo */
			*burst_code = DCMD_BURST16;
			burst_bytes = 16;
			retval = 1;
		} else {
			*burst_code = DCMD_BURST32;
			burst_bytes = 32;
		}
	}

	thresh_words = burst_bytes / bytes_per_word;

	/* thresh_words will be between 2 and 8 */
	*threshold = (SSCR1_RxTresh(thresh_words) & SSCR1_RFT)
			| (SSCR1_TxTresh(16-thresh_words) & SSCR1_TFT);

	return retval;
}

static unsigned int ssp_get_clk_div(struct ssp_device *ssp, int rate)
{
	unsigned long ssp_clk = clk_get_rate(ssp->clk);

	if (ssp->type == PXA25x_SSP || ssp->type == CE4100_SSP)
		return ((ssp_clk / (2 * rate) - 1) & 0xff) << 8;
	else
		return ((ssp_clk / rate - 1) & 0xfff) << 8;
}

static void pump_transfers(unsigned long data)
{
	struct driver_data *drv_data = (struct driver_data *)data;
	struct spi_message *message = NULL;
	struct spi_transfer *transfer = NULL;
	struct spi_transfer *previous = NULL;
	struct chip_data *chip = NULL;
	struct ssp_device *ssp = drv_data->ssp;
	void __iomem *reg = drv_data->ioaddr;
	u32 clk_div = 0;
	u8 bits = 0;
	u32 speed = 0;
	u32 cr0;
	u32 cr1;
	u32 dma_thresh = drv_data->cur_chip->dma_threshold;
	u32 dma_burst = drv_data->cur_chip->dma_burst_size;

	/* Get current state information */
	message = drv_data->cur_msg;
	transfer = drv_data->cur_transfer;
	chip = drv_data->cur_chip;

	/* Handle for abort */
	if (message->state == ERROR_STATE) {
		message->status = -EIO;
		giveback(drv_data);
		return;
	}

	/* Handle end of message */
	if (message->state == DONE_STATE) {
		message->status = 0;
		giveback(drv_data);
		return;
	}

	/* Delay if requested at end of transfer before CS change */
	if (message->state == RUNNING_STATE) {
		previous = list_entry(transfer->transfer_list.prev,
					struct spi_transfer,
					transfer_list);
		if (previous->delay_usecs)
			udelay(previous->delay_usecs);

		/* Drop chip select only if cs_change is requested */
		if (previous->cs_change)
			cs_deassert(drv_data);
	}

	/* Check for transfers that need multiple DMA segments */
	if (transfer->len > MAX_DMA_LEN && chip->enable_dma) {

		/* reject already-mapped transfers; PIO won't always work */
		if (message->is_dma_mapped
				|| transfer->rx_dma || transfer->tx_dma) {
			dev_err(&drv_data->pdev->dev,
				"pump_transfers: mapped transfer length "
				"of %u is greater than %d\n",
				transfer->len, MAX_DMA_LEN);
			message->status = -EINVAL;
			giveback(drv_data);
			return;
		}

		/* warn ... we force this to PIO mode */
		if (printk_ratelimit())
			dev_warn(&message->spi->dev, "pump_transfers: "
				"DMA disabled for transfer length %ld "
				"greater than %d\n",
				(long)drv_data->len, MAX_DMA_LEN);
	}

	/* Setup the transfer state based on the type of transfer */
	if (flush(drv_data) == 0) {
		dev_err(&drv_data->pdev->dev, "pump_transfers: flush failed\n");
		message->status = -EIO;
		giveback(drv_data);
		return;
	}
	drv_data->n_bytes = chip->n_bytes;
	drv_data->dma_width = chip->dma_width;
=======
/*
 * The Quark SPI has an additional 24 bit register (DDS_CLK_RATE) to multiply
 * input frequency by fractions of 2^24. It also has a divider by 5.
 *
 * There are formulas to get baud rate value for given input frequency and
 * divider parameters, such as DDS_CLK_RATE and SCR:
 *
 * Fsys = 200MHz
 *
 * Fssp = Fsys * DDS_CLK_RATE / 2^24			(1)
 * Baud rate = Fsclk = Fssp / (2 * (SCR + 1))		(2)
 *
 * DDS_CLK_RATE either 2^n or 2^n / 5.
 * SCR is in range 0 .. 255
 *
 * Divisor = 5^i * 2^j * 2 * k
 *       i = [0, 1]      i = 1 iff j = 0 or j > 3
 *       j = [0, 23]     j = 0 iff i = 1
 *       k = [1, 256]
 * Special case: j = 0, i = 1: Divisor = 2 / 5
 *
 * Accordingly to the specification the recommended values for DDS_CLK_RATE
 * are:
 *	Case 1:		2^n, n = [0, 23]
 *	Case 2:		2^24 * 2 / 5 (0x666666)
 *	Case 3:		less than or equal to 2^24 / 5 / 16 (0x33333)
 *
 * In all cases the lowest possible value is better.
 *
 * The function calculates parameters for all cases and chooses the one closest
 * to the asked baud rate.
 */
static unsigned int quark_x1000_get_clk_div(int rate, u32 *dds)
{
	unsigned long xtal = 200000000;
	unsigned long fref = xtal / 2;		/* mandatory division by 2,
						   see (2) */
						/* case 3 */
	unsigned long fref1 = fref / 2;		/* case 1 */
	unsigned long fref2 = fref * 2 / 5;	/* case 2 */
	unsigned long scale;
	unsigned long q, q1, q2;
	long r, r1, r2;
	u32 mul;

	/* Case 1 */

	/* Set initial value for DDS_CLK_RATE */
	mul = (1 << 24) >> 1;

	/* Calculate initial quot */
	q1 = DIV_ROUND_UP(fref1, rate);

	/* Scale q1 if it's too big */
	if (q1 > 256) {
		/* Scale q1 to range [1, 512] */
		scale = fls_long(q1 - 1);
		if (scale > 9) {
			q1 >>= scale - 9;
			mul >>= scale - 9;
		}

		/* Round the result if we have a remainder */
		q1 += q1 & 1;
	}

	/* Decrease DDS_CLK_RATE as much as we can without loss in precision */
	scale = __ffs(q1);
	q1 >>= scale;
	mul >>= scale;

	/* Get the remainder */
	r1 = abs(fref1 / (1 << (24 - fls_long(mul))) / q1 - rate);

	/* Case 2 */

	q2 = DIV_ROUND_UP(fref2, rate);
	r2 = abs(fref2 / q2 - rate);

	/*
	 * Choose the best between two: less remainder we have the better. We
	 * can't go case 2 if q2 is greater than 256 since SCR register can
	 * hold only values 0 .. 255.
	 */
	if (r2 >= r1 || q2 > 256) {
		/* case 1 is better */
		r = r1;
		q = q1;
	} else {
		/* case 2 is better */
		r = r2;
		q = q2;
		mul = (1 << 24) * 2 / 5;
	}

	/* Check case 3 only if the divisor is big enough */
	if (fref / rate >= 80) {
		u64 fssp;
		u32 m;

		/* Calculate initial quot */
		q1 = DIV_ROUND_UP(fref, rate);
		m = (1 << 24) / q1;

		/* Get the remainder */
		fssp = (u64)fref * m;
		do_div(fssp, 1 << 24);
		r1 = abs(fssp - rate);

		/* Choose this one if it suits better */
		if (r1 < r) {
			/* case 3 is better */
			q = 1;
			mul = m;
		}
	}

	*dds = mul;
	return q - 1;
}

static unsigned int ssp_get_clk_div(struct driver_data *drv_data, int rate)
{
	unsigned long ssp_clk = drv_data->controller->max_speed_hz;
	const struct ssp_device *ssp = drv_data->ssp;

	rate = min_t(int, ssp_clk, rate);

	/*
	 * Calculate the divisor for the SCR (Serial Clock Rate), avoiding
	 * that the SSP transmission rate can be greater than the device rate.
	 */
	if (ssp->type == PXA25x_SSP || ssp->type == CE4100_SSP)
		return (DIV_ROUND_UP(ssp_clk, 2 * rate) - 1) & 0xff;
	else
		return (DIV_ROUND_UP(ssp_clk, rate) - 1)  & 0xfff;
}

static unsigned int pxa2xx_ssp_get_clk_div(struct driver_data *drv_data,
					   int rate)
{
	struct chip_data *chip =
		spi_get_ctldata(drv_data->controller->cur_msg->spi);
	unsigned int clk_div;

	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		clk_div = quark_x1000_get_clk_div(rate, &chip->dds_rate);
		break;
	default:
		clk_div = ssp_get_clk_div(drv_data, rate);
		break;
	}
	return clk_div << 8;
}

static bool pxa2xx_spi_can_dma(struct spi_controller *controller,
			       struct spi_device *spi,
			       struct spi_transfer *xfer)
{
	struct chip_data *chip = spi_get_ctldata(spi);

	return chip->enable_dma &&
	       xfer->len <= MAX_DMA_LEN &&
	       xfer->len >= chip->dma_burst_size;
}

static int pxa2xx_spi_transfer_one(struct spi_controller *controller,
				   struct spi_device *spi,
				   struct spi_transfer *transfer)
{
	struct driver_data *drv_data = spi_controller_get_devdata(controller);
	struct spi_message *message = controller->cur_msg;
	struct chip_data *chip = spi_get_ctldata(spi);
	u32 dma_thresh = chip->dma_threshold;
	u32 dma_burst = chip->dma_burst_size;
	u32 change_mask = pxa2xx_spi_get_ssrc1_change_mask(drv_data);
	u32 clk_div;
	u8 bits;
	u32 speed;
	u32 cr0;
	u32 cr1;
	int err;
	int dma_mapped;

	/* Check if we can DMA this transfer */
	if (transfer->len > MAX_DMA_LEN && chip->enable_dma) {

		/* Reject already-mapped transfers; PIO won't always work */
		if (message->is_dma_mapped
				|| transfer->rx_dma || transfer->tx_dma) {
			dev_err(&spi->dev,
				"Mapped transfer length of %u is greater than %d\n",
				transfer->len, MAX_DMA_LEN);
			return -EINVAL;
		}

		/* Warn ... we force this to PIO mode */
		dev_warn_ratelimited(&spi->dev,
				     "DMA disabled for transfer length %u greater than %d\n",
				     transfer->len, MAX_DMA_LEN);
	}

	/* Setup the transfer state based on the type of transfer */
	if (pxa2xx_spi_flush(drv_data) == 0) {
		dev_err(&spi->dev, "Flush failed\n");
		return -EIO;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	drv_data->tx = (void *)transfer->tx_buf;
	drv_data->tx_end = drv_data->tx + transfer->len;
	drv_data->rx = transfer->rx_buf;
	drv_data->rx_end = drv_data->rx + transfer->len;
<<<<<<< HEAD
	drv_data->rx_dma = transfer->rx_dma;
	drv_data->tx_dma = transfer->tx_dma;
	drv_data->len = transfer->len & DCMD_LENGTH;
	drv_data->write = drv_data->tx ? chip->write : null_writer;
	drv_data->read = drv_data->rx ? chip->read : null_reader;

	/* Change speed and bit per word on a per transfer */
	cr0 = chip->cr0;
	if (transfer->speed_hz || transfer->bits_per_word) {

		bits = chip->bits_per_word;
		speed = chip->speed_hz;

		if (transfer->speed_hz)
			speed = transfer->speed_hz;

		if (transfer->bits_per_word)
			bits = transfer->bits_per_word;

		clk_div = ssp_get_clk_div(ssp, speed);

		if (bits <= 8) {
			drv_data->n_bytes = 1;
			drv_data->dma_width = DCMD_WIDTH1;
			drv_data->read = drv_data->read != null_reader ?
						u8_reader : null_reader;
			drv_data->write = drv_data->write != null_writer ?
						u8_writer : null_writer;
		} else if (bits <= 16) {
			drv_data->n_bytes = 2;
			drv_data->dma_width = DCMD_WIDTH2;
			drv_data->read = drv_data->read != null_reader ?
						u16_reader : null_reader;
			drv_data->write = drv_data->write != null_writer ?
						u16_writer : null_writer;
		} else if (bits <= 32) {
			drv_data->n_bytes = 4;
			drv_data->dma_width = DCMD_WIDTH4;
			drv_data->read = drv_data->read != null_reader ?
						u32_reader : null_reader;
			drv_data->write = drv_data->write != null_writer ?
						u32_writer : null_writer;
		}
		/* if bits/word is changed in dma mode, then must check the
		 * thresholds and burst also */
		if (chip->enable_dma) {
			if (set_dma_burst_and_threshold(chip, message->spi,
							bits, &dma_burst,
							&dma_thresh))
				if (printk_ratelimit())
					dev_warn(&message->spi->dev,
						"pump_transfers: "
						"DMA burst size reduced to "
						"match bits_per_word\n");
		}

		cr0 = clk_div
			| SSCR0_Motorola
			| SSCR0_DataSize(bits > 16 ? bits - 16 : bits)
			| SSCR0_SSE
			| (bits > 16 ? SSCR0_EDSS : 0);
	}

	message->state = RUNNING_STATE;

	/* Try to map dma buffer and do a dma transfer if successful, but
	 * only if the length is non-zero and less than MAX_DMA_LEN.
	 *
	 * Zero-length non-descriptor DMA is illegal on PXA2xx; force use
	 * of PIO instead.  Care is needed above because the transfer may
	 * have have been passed with buffers that are already dma mapped.
	 * A zero-length transfer in PIO mode will not try to write/read
	 * to/from the buffers
	 *
	 * REVISIT large transfers are exactly where we most want to be
	 * using DMA.  If this happens much, split those transfers into
	 * multiple DMA segments rather than forcing PIO.
	 */
	drv_data->dma_mapped = 0;
	if (drv_data->len > 0 && drv_data->len <= MAX_DMA_LEN)
		drv_data->dma_mapped = map_dma_buffers(drv_data);
	if (drv_data->dma_mapped) {

		/* Ensure we have the correct interrupt handler */
		drv_data->transfer_handler = dma_transfer;

		/* Setup rx DMA Channel */
		DCSR(drv_data->rx_channel) = RESET_DMA_CHANNEL;
		DSADR(drv_data->rx_channel) = drv_data->ssdr_physical;
		DTADR(drv_data->rx_channel) = drv_data->rx_dma;
		if (drv_data->rx == drv_data->null_dma_buf)
			/* No target address increment */
			DCMD(drv_data->rx_channel) = DCMD_FLOWSRC
							| drv_data->dma_width
							| dma_burst
							| drv_data->len;
		else
			DCMD(drv_data->rx_channel) = DCMD_INCTRGADDR
							| DCMD_FLOWSRC
							| drv_data->dma_width
							| dma_burst
							| drv_data->len;

		/* Setup tx DMA Channel */
		DCSR(drv_data->tx_channel) = RESET_DMA_CHANNEL;
		DSADR(drv_data->tx_channel) = drv_data->tx_dma;
		DTADR(drv_data->tx_channel) = drv_data->ssdr_physical;
		if (drv_data->tx == drv_data->null_dma_buf)
			/* No source address increment */
			DCMD(drv_data->tx_channel) = DCMD_FLOWTRG
							| drv_data->dma_width
							| dma_burst
							| drv_data->len;
		else
			DCMD(drv_data->tx_channel) = DCMD_INCSRCADDR
							| DCMD_FLOWTRG
							| drv_data->dma_width
							| dma_burst
							| drv_data->len;

		/* Enable dma end irqs on SSP to detect end of transfer */
		if (drv_data->ssp_type == PXA25x_SSP)
			DCMD(drv_data->tx_channel) |= DCMD_ENDIRQEN;

		/* Clear status and start DMA engine */
		cr1 = chip->cr1 | dma_thresh | drv_data->dma_cr1;
		write_SSSR(drv_data->clear_sr, reg);
		DCSR(drv_data->rx_channel) |= DCSR_RUN;
		DCSR(drv_data->tx_channel) |= DCSR_RUN;
=======

	/* Change speed and bit per word on a per transfer */
	bits = transfer->bits_per_word;
	speed = transfer->speed_hz;

	clk_div = pxa2xx_ssp_get_clk_div(drv_data, speed);

	if (bits <= 8) {
		drv_data->n_bytes = 1;
		drv_data->read = drv_data->rx ? u8_reader : null_reader;
		drv_data->write = drv_data->tx ? u8_writer : null_writer;
	} else if (bits <= 16) {
		drv_data->n_bytes = 2;
		drv_data->read = drv_data->rx ? u16_reader : null_reader;
		drv_data->write = drv_data->tx ? u16_writer : null_writer;
	} else if (bits <= 32) {
		drv_data->n_bytes = 4;
		drv_data->read = drv_data->rx ? u32_reader : null_reader;
		drv_data->write = drv_data->tx ? u32_writer : null_writer;
	}
	/*
	 * If bits per word is changed in DMA mode, then must check
	 * the thresholds and burst also.
	 */
	if (chip->enable_dma) {
		if (pxa2xx_spi_set_dma_burst_and_threshold(chip,
						spi,
						bits, &dma_burst,
						&dma_thresh))
			dev_warn_ratelimited(&spi->dev,
					     "DMA burst size reduced to match bits_per_word\n");
	}

	dma_mapped = controller->can_dma &&
		     controller->can_dma(controller, spi, transfer) &&
		     controller->cur_msg_mapped;
	if (dma_mapped) {

		/* Ensure we have the correct interrupt handler */
		drv_data->transfer_handler = pxa2xx_spi_dma_transfer;

		err = pxa2xx_spi_dma_prepare(drv_data, transfer);
		if (err)
			return err;

		/* Clear status and start DMA engine */
		cr1 = chip->cr1 | dma_thresh | drv_data->dma_cr1;
		pxa2xx_spi_write(drv_data, SSSR, drv_data->clear_sr);

		pxa2xx_spi_dma_start(drv_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		/* Ensure we have the correct interrupt handler	*/
		drv_data->transfer_handler = interrupt_transfer;

		/* Clear status  */
		cr1 = chip->cr1 | chip->threshold | drv_data->int_cr1;
		write_SSSR_CS(drv_data, drv_data->clear_sr);
	}

<<<<<<< HEAD
	/* see if we need to reload the config registers */
	if ((read_SSCR0(reg) != cr0)
		|| (read_SSCR1(reg) & SSCR1_CHANGE_MASK) !=
			(cr1 & SSCR1_CHANGE_MASK)) {

		/* stop the SSP, and update the other bits */
		write_SSCR0(cr0 & ~SSCR0_SSE, reg);
		if (!pxa25x_ssp_comp(drv_data))
			write_SSTO(chip->timeout, reg);
		/* first set CR1 without interrupt and service enables */
		write_SSCR1(cr1 & SSCR1_CHANGE_MASK, reg);
		/* restart the SSP */
		write_SSCR0(cr0, reg);

	} else {
		if (!pxa25x_ssp_comp(drv_data))
			write_SSTO(chip->timeout, reg);
	}

	cs_assert(drv_data);

	/* after chip select, release the data by enabling service
	 * requests and interrupts, without changing any mode bits */
	write_SSCR1(cr1, reg);
}

static void pump_messages(struct work_struct *work)
{
	struct driver_data *drv_data =
		container_of(work, struct driver_data, pump_messages);
	unsigned long flags;

	/* Lock queue and check for queue work */
	spin_lock_irqsave(&drv_data->lock, flags);
	if (list_empty(&drv_data->queue) || drv_data->run == QUEUE_STOPPED) {
		drv_data->busy = 0;
		spin_unlock_irqrestore(&drv_data->lock, flags);
		return;
	}

	/* Make sure we are not already running a message */
	if (drv_data->cur_msg) {
		spin_unlock_irqrestore(&drv_data->lock, flags);
		return;
	}

	/* Extract head of queue */
	drv_data->cur_msg = list_entry(drv_data->queue.next,
					struct spi_message, queue);
	list_del_init(&drv_data->cur_msg->queue);

	/* Initial message state*/
	drv_data->cur_msg->state = START_STATE;
	drv_data->cur_transfer = list_entry(drv_data->cur_msg->transfers.next,
						struct spi_transfer,
						transfer_list);

	/* prepare to setup the SSP, in pump_transfers, using the per
	 * chip configuration */
	drv_data->cur_chip = spi_get_ctldata(drv_data->cur_msg->spi);

	/* Mark as busy and launch transfers */
	tasklet_schedule(&drv_data->pump_transfers);

	drv_data->busy = 1;
	spin_unlock_irqrestore(&drv_data->lock, flags);
}

static int transfer(struct spi_device *spi, struct spi_message *msg)
{
	struct driver_data *drv_data = spi_master_get_devdata(spi->master);
	unsigned long flags;

	spin_lock_irqsave(&drv_data->lock, flags);

	if (drv_data->run == QUEUE_STOPPED) {
		spin_unlock_irqrestore(&drv_data->lock, flags);
		return -ESHUTDOWN;
	}

	msg->actual_length = 0;
	msg->status = -EINPROGRESS;
	msg->state = START_STATE;

	list_add_tail(&msg->queue, &drv_data->queue);

	if (drv_data->run == QUEUE_RUNNING && !drv_data->busy)
		queue_work(drv_data->workqueue, &drv_data->pump_messages);

	spin_unlock_irqrestore(&drv_data->lock, flags);
=======
	/* NOTE:  PXA25x_SSP _could_ use external clocking ... */
	cr0 = pxa2xx_configure_sscr0(drv_data, clk_div, bits);
	if (!pxa25x_ssp_comp(drv_data))
		dev_dbg(&spi->dev, "%u Hz actual, %s\n",
			controller->max_speed_hz
				/ (1 + ((cr0 & SSCR0_SCR(0xfff)) >> 8)),
			dma_mapped ? "DMA" : "PIO");
	else
		dev_dbg(&spi->dev, "%u Hz actual, %s\n",
			controller->max_speed_hz / 2
				/ (1 + ((cr0 & SSCR0_SCR(0x0ff)) >> 8)),
			dma_mapped ? "DMA" : "PIO");

	if (is_lpss_ssp(drv_data)) {
		pxa2xx_spi_update(drv_data, SSIRF, GENMASK(7, 0), chip->lpss_rx_threshold);
		pxa2xx_spi_update(drv_data, SSITF, GENMASK(15, 0), chip->lpss_tx_threshold);
	}

	if (is_mrfld_ssp(drv_data)) {
		u32 mask = SFIFOTT_RFT | SFIFOTT_TFT;
		u32 thresh = 0;

		thresh |= SFIFOTT_RxThresh(chip->lpss_rx_threshold);
		thresh |= SFIFOTT_TxThresh(chip->lpss_tx_threshold);

		pxa2xx_spi_update(drv_data, SFIFOTT, mask, thresh);
	}

	if (is_quark_x1000_ssp(drv_data))
		pxa2xx_spi_update(drv_data, DDS_RATE, GENMASK(23, 0), chip->dds_rate);

	/* Stop the SSP */
	if (!is_mmp2_ssp(drv_data))
		pxa_ssp_disable(drv_data->ssp);

	if (!pxa25x_ssp_comp(drv_data))
		pxa2xx_spi_write(drv_data, SSTO, chip->timeout);

	/* First set CR1 without interrupt and service enables */
	pxa2xx_spi_update(drv_data, SSCR1, change_mask, cr1);

	/* See if we need to reload the configuration registers */
	pxa2xx_spi_update(drv_data, SSCR0, GENMASK(31, 0), cr0);

	/* Restart the SSP */
	pxa_ssp_enable(drv_data->ssp);

	if (is_mmp2_ssp(drv_data)) {
		u8 tx_level = read_SSSR_bits(drv_data, SSSR_TFL_MASK) >> 8;

		if (tx_level) {
			/* On MMP2, flipping SSE doesn't to empty Tx FIFO. */
			dev_warn(&spi->dev, "%u bytes of garbage in Tx FIFO!\n", tx_level);
			if (tx_level > transfer->len)
				tx_level = transfer->len;
			drv_data->tx += tx_level;
		}
	}

	if (spi_controller_is_target(controller)) {
		while (drv_data->write(drv_data))
			;
		if (drv_data->gpiod_ready) {
			gpiod_set_value(drv_data->gpiod_ready, 1);
			udelay(1);
			gpiod_set_value(drv_data->gpiod_ready, 0);
		}
	}

	/*
	 * Release the data by enabling service requests and interrupts,
	 * without changing any mode bits.
	 */
	pxa2xx_spi_write(drv_data, SSCR1, cr1);

	return 1;
}

static int pxa2xx_spi_target_abort(struct spi_controller *controller)
{
	struct driver_data *drv_data = spi_controller_get_devdata(controller);

	int_error_stop(drv_data, "transfer aborted", -EINTR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int setup_cs(struct spi_device *spi, struct chip_data *chip,
		    struct pxa2xx_spi_chip *chip_info)
{
	int err = 0;

	if (chip == NULL || chip_info == NULL)
		return 0;

	/* NOTE: setup() can be called multiple times, possibly with
	 * different chip_info, release previously requested GPIO
	 */
	if (gpio_is_valid(chip->gpio_cs))
		gpio_free(chip->gpio_cs);

	/* If (*cs_control) is provided, ignore GPIO chip select */
	if (chip_info->cs_control) {
		chip->cs_control = chip_info->cs_control;
		return 0;
	}

	if (gpio_is_valid(chip_info->gpio_cs)) {
		err = gpio_request(chip_info->gpio_cs, "SPI_CS");
		if (err) {
			dev_err(&spi->dev, "failed to request chip select "
					"GPIO%d\n", chip_info->gpio_cs);
			return err;
		}

		chip->gpio_cs = chip_info->gpio_cs;
		chip->gpio_cs_inverted = spi->mode & SPI_CS_HIGH;

		err = gpio_direction_output(chip->gpio_cs,
					!chip->gpio_cs_inverted);
	}

	return err;
=======
static void pxa2xx_spi_handle_err(struct spi_controller *controller,
				 struct spi_message *msg)
{
	struct driver_data *drv_data = spi_controller_get_devdata(controller);

	int_stop_and_reset(drv_data);

	/* Disable the SSP */
	pxa2xx_spi_off(drv_data);

	/*
	 * Stop the DMA if running. Note DMA callback handler may have unset
	 * the dma_running already, which is fine as stopping is not needed
	 * then but we shouldn't rely this flag for anything else than
	 * stopping. For instance to differentiate between PIO and DMA
	 * transfers.
	 */
	if (atomic_read(&drv_data->dma_running))
		pxa2xx_spi_dma_stop(drv_data);
}

static int pxa2xx_spi_unprepare_transfer(struct spi_controller *controller)
{
	struct driver_data *drv_data = spi_controller_get_devdata(controller);

	/* Disable the SSP now */
	pxa2xx_spi_off(drv_data);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int setup(struct spi_device *spi)
{
<<<<<<< HEAD
	struct pxa2xx_spi_chip *chip_info = NULL;
	struct chip_data *chip;
	struct driver_data *drv_data = spi_master_get_devdata(spi->master);
	struct ssp_device *ssp = drv_data->ssp;
	unsigned int clk_div;
	uint tx_thres = TX_THRESH_DFLT;
	uint rx_thres = RX_THRESH_DFLT;

	if (!pxa25x_ssp_comp(drv_data)
		&& (spi->bits_per_word < 4 || spi->bits_per_word > 32)) {
		dev_err(&spi->dev, "failed setup: ssp_type=%d, bits/wrd=%d "
				"b/w not 4-32 for type non-PXA25x_SSP\n",
				drv_data->ssp_type, spi->bits_per_word);
		return -EINVAL;
	} else if (pxa25x_ssp_comp(drv_data)
			&& (spi->bits_per_word < 4
				|| spi->bits_per_word > 16)) {
		dev_err(&spi->dev, "failed setup: ssp_type=%d, bits/wrd=%d "
				"b/w not 4-16 for type PXA25x_SSP\n",
				drv_data->ssp_type, spi->bits_per_word);
		return -EINVAL;
	}

	/* Only alloc on first setup */
	chip = spi_get_ctldata(spi);
	if (!chip) {
		chip = kzalloc(sizeof(struct chip_data), GFP_KERNEL);
		if (!chip) {
			dev_err(&spi->dev,
				"failed setup: can't allocate chip data\n");
			return -ENOMEM;
		}

		if (drv_data->ssp_type == CE4100_SSP) {
			if (spi->chip_select > 4) {
				dev_err(&spi->dev, "failed setup: "
				"cs number must not be > 4.\n");
				kfree(chip);
				return -EINVAL;
			}

			chip->frm = spi->chip_select;
		} else
			chip->gpio_cs = -1;
		chip->enable_dma = 0;
		chip->timeout = TIMOUT_DFLT;
		chip->dma_burst_size = drv_data->master_info->enable_dma ?
					DCMD_BURST8 : 0;
	}

	/* protocol drivers may change the chip settings, so...
	 * if chip_info exists, use it */
	chip_info = spi->controller_data;

	/* chip_info isn't always needed */
	chip->cr1 = 0;
=======
	struct pxa2xx_spi_chip *chip_info;
	struct chip_data *chip;
	const struct lpss_config *config;
	struct driver_data *drv_data =
		spi_controller_get_devdata(spi->controller);
	uint tx_thres, tx_hi_thres, rx_thres;

	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		tx_thres = TX_THRESH_QUARK_X1000_DFLT;
		tx_hi_thres = 0;
		rx_thres = RX_THRESH_QUARK_X1000_DFLT;
		break;
	case MRFLD_SSP:
		tx_thres = TX_THRESH_MRFLD_DFLT;
		tx_hi_thres = 0;
		rx_thres = RX_THRESH_MRFLD_DFLT;
		break;
	case CE4100_SSP:
		tx_thres = TX_THRESH_CE4100_DFLT;
		tx_hi_thres = 0;
		rx_thres = RX_THRESH_CE4100_DFLT;
		break;
	case LPSS_LPT_SSP:
	case LPSS_BYT_SSP:
	case LPSS_BSW_SSP:
	case LPSS_SPT_SSP:
	case LPSS_BXT_SSP:
	case LPSS_CNL_SSP:
		config = lpss_get_config(drv_data);
		tx_thres = config->tx_threshold_lo;
		tx_hi_thres = config->tx_threshold_hi;
		rx_thres = config->rx_threshold;
		break;
	default:
		tx_hi_thres = 0;
		if (spi_controller_is_target(drv_data->controller)) {
			tx_thres = 1;
			rx_thres = 2;
		} else {
			tx_thres = TX_THRESH_DFLT;
			rx_thres = RX_THRESH_DFLT;
		}
		break;
	}

	/* Only allocate on the first setup */
	chip = spi_get_ctldata(spi);
	if (!chip) {
		chip = kzalloc(sizeof(struct chip_data), GFP_KERNEL);
		if (!chip)
			return -ENOMEM;

		if (drv_data->ssp_type == CE4100_SSP) {
			if (spi_get_chipselect(spi, 0) > 4) {
				dev_err(&spi->dev,
					"failed setup: cs number must not be > 4.\n");
				kfree(chip);
				return -EINVAL;
			}
		}
		chip->enable_dma = drv_data->controller_info->enable_dma;
		chip->timeout = TIMOUT_DFLT;
	}

	/*
	 * Protocol drivers may change the chip settings, so...
	 * if chip_info exists, use it.
	 */
	chip_info = spi->controller_data;

	/* chip_info isn't always needed */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (chip_info) {
		if (chip_info->timeout)
			chip->timeout = chip_info->timeout;
		if (chip_info->tx_threshold)
			tx_thres = chip_info->tx_threshold;
<<<<<<< HEAD
		if (chip_info->rx_threshold)
			rx_thres = chip_info->rx_threshold;
		chip->enable_dma = drv_data->master_info->enable_dma;
		chip->dma_threshold = 0;
		if (chip_info->enable_loopback)
			chip->cr1 = SSCR1_LBM;
	}

	chip->threshold = (SSCR1_RxTresh(rx_thres) & SSCR1_RFT) |
			(SSCR1_TxTresh(tx_thres) & SSCR1_TFT);

	/* set dma burst and threshold outside of chip_info path so that if
	 * chip_info goes away after setting chip->enable_dma, the
	 * burst and threshold can still respond to changes in bits_per_word */
	if (chip->enable_dma) {
		/* set up legal burst and threshold for dma */
		if (set_dma_burst_and_threshold(chip, spi, spi->bits_per_word,
						&chip->dma_burst_size,
						&chip->dma_threshold)) {
			dev_warn(&spi->dev, "in setup: DMA burst size reduced "
					"to match bits_per_word\n");
		}
	}

	clk_div = ssp_get_clk_div(ssp, spi->max_speed_hz);
	chip->speed_hz = spi->max_speed_hz;

	chip->cr0 = clk_div
			| SSCR0_Motorola
			| SSCR0_DataSize(spi->bits_per_word > 16 ?
				spi->bits_per_word - 16 : spi->bits_per_word)
			| SSCR0_SSE
			| (spi->bits_per_word > 16 ? SSCR0_EDSS : 0);
	chip->cr1 &= ~(SSCR1_SPO | SSCR1_SPH);
	chip->cr1 |= (((spi->mode & SPI_CPHA) != 0) ? SSCR1_SPH : 0)
			| (((spi->mode & SPI_CPOL) != 0) ? SSCR1_SPO : 0);

	/* NOTE:  PXA25x_SSP _could_ use external clocking ... */
	if (!pxa25x_ssp_comp(drv_data))
		dev_dbg(&spi->dev, "%ld Hz actual, %s\n",
			clk_get_rate(ssp->clk)
				/ (1 + ((chip->cr0 & SSCR0_SCR(0xfff)) >> 8)),
			chip->enable_dma ? "DMA" : "PIO");
	else
		dev_dbg(&spi->dev, "%ld Hz actual, %s\n",
			clk_get_rate(ssp->clk) / 2
				/ (1 + ((chip->cr0 & SSCR0_SCR(0x0ff)) >> 8)),
			chip->enable_dma ? "DMA" : "PIO");

	if (spi->bits_per_word <= 8) {
		chip->n_bytes = 1;
		chip->dma_width = DCMD_WIDTH1;
		chip->read = u8_reader;
		chip->write = u8_writer;
	} else if (spi->bits_per_word <= 16) {
		chip->n_bytes = 2;
		chip->dma_width = DCMD_WIDTH2;
		chip->read = u16_reader;
		chip->write = u16_writer;
	} else if (spi->bits_per_word <= 32) {
		chip->cr0 |= SSCR0_EDSS;
		chip->n_bytes = 4;
		chip->dma_width = DCMD_WIDTH4;
		chip->read = u32_reader;
		chip->write = u32_writer;
	} else {
		dev_err(&spi->dev, "invalid wordsize\n");
		return -ENODEV;
	}
	chip->bits_per_word = spi->bits_per_word;

	spi_set_ctldata(spi, chip);

	if (drv_data->ssp_type == CE4100_SSP)
		return 0;

	return setup_cs(spi, chip, chip_info);
=======
		if (chip_info->tx_hi_threshold)
			tx_hi_thres = chip_info->tx_hi_threshold;
		if (chip_info->rx_threshold)
			rx_thres = chip_info->rx_threshold;
		chip->dma_threshold = 0;
	}

	chip->cr1 = 0;
	if (spi_controller_is_target(drv_data->controller)) {
		chip->cr1 |= SSCR1_SCFR;
		chip->cr1 |= SSCR1_SCLKDIR;
		chip->cr1 |= SSCR1_SFRMDIR;
		chip->cr1 |= SSCR1_SPH;
	}

	if (is_lpss_ssp(drv_data)) {
		chip->lpss_rx_threshold = SSIRF_RxThresh(rx_thres);
		chip->lpss_tx_threshold = SSITF_TxLoThresh(tx_thres) |
					  SSITF_TxHiThresh(tx_hi_thres);
	}

	if (is_mrfld_ssp(drv_data)) {
		chip->lpss_rx_threshold = rx_thres;
		chip->lpss_tx_threshold = tx_thres;
	}

	/*
	 * Set DMA burst and threshold outside of chip_info path so that if
	 * chip_info goes away after setting chip->enable_dma, the burst and
	 * threshold can still respond to changes in bits_per_word.
	 */
	if (chip->enable_dma) {
		/* Set up legal burst and threshold for DMA */
		if (pxa2xx_spi_set_dma_burst_and_threshold(chip, spi,
						spi->bits_per_word,
						&chip->dma_burst_size,
						&chip->dma_threshold)) {
			dev_warn(&spi->dev,
				 "in setup: DMA burst size reduced to match bits_per_word\n");
		}
		dev_dbg(&spi->dev,
			"in setup: DMA burst size set to %u\n",
			chip->dma_burst_size);
	}

	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		chip->threshold = (QUARK_X1000_SSCR1_RxTresh(rx_thres)
				   & QUARK_X1000_SSCR1_RFT)
				   | (QUARK_X1000_SSCR1_TxTresh(tx_thres)
				   & QUARK_X1000_SSCR1_TFT);
		break;
	case CE4100_SSP:
		chip->threshold = (CE4100_SSCR1_RxTresh(rx_thres) & CE4100_SSCR1_RFT) |
			(CE4100_SSCR1_TxTresh(tx_thres) & CE4100_SSCR1_TFT);
		break;
	default:
		chip->threshold = (SSCR1_RxTresh(rx_thres) & SSCR1_RFT) |
			(SSCR1_TxTresh(tx_thres) & SSCR1_TFT);
		break;
	}

	chip->cr1 &= ~(SSCR1_SPO | SSCR1_SPH);
	chip->cr1 |= ((spi->mode & SPI_CPHA) ? SSCR1_SPH : 0) |
		     ((spi->mode & SPI_CPOL) ? SSCR1_SPO : 0);

	if (spi->mode & SPI_LOOP)
		chip->cr1 |= SSCR1_LBM;

	spi_set_ctldata(spi, chip);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void cleanup(struct spi_device *spi)
{
	struct chip_data *chip = spi_get_ctldata(spi);
<<<<<<< HEAD
	struct driver_data *drv_data = spi_master_get_devdata(spi->master);

	if (!chip)
		return;

	if (drv_data->ssp_type != CE4100_SSP && gpio_is_valid(chip->gpio_cs))
		gpio_free(chip->gpio_cs);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree(chip);
}

<<<<<<< HEAD
static int __devinit init_queue(struct driver_data *drv_data)
{
	INIT_LIST_HEAD(&drv_data->queue);
	spin_lock_init(&drv_data->lock);

	drv_data->run = QUEUE_STOPPED;
	drv_data->busy = 0;

	tasklet_init(&drv_data->pump_transfers,
			pump_transfers,	(unsigned long)drv_data);

	INIT_WORK(&drv_data->pump_messages, pump_messages);
	drv_data->workqueue = create_singlethread_workqueue(
				dev_name(drv_data->master->dev.parent));
	if (drv_data->workqueue == NULL)
		return -EBUSY;

	return 0;
}

static int start_queue(struct driver_data *drv_data)
{
	unsigned long flags;

	spin_lock_irqsave(&drv_data->lock, flags);

	if (drv_data->run == QUEUE_RUNNING || drv_data->busy) {
		spin_unlock_irqrestore(&drv_data->lock, flags);
		return -EBUSY;
	}

	drv_data->run = QUEUE_RUNNING;
	drv_data->cur_msg = NULL;
	drv_data->cur_transfer = NULL;
	drv_data->cur_chip = NULL;
	spin_unlock_irqrestore(&drv_data->lock, flags);

	queue_work(drv_data->workqueue, &drv_data->pump_messages);

	return 0;
}

static int stop_queue(struct driver_data *drv_data)
{
	unsigned long flags;
	unsigned limit = 500;
	int status = 0;

	spin_lock_irqsave(&drv_data->lock, flags);

	/* This is a bit lame, but is optimized for the common execution path.
	 * A wait_queue on the drv_data->busy could be used, but then the common
	 * execution path (pump_messages) would be required to call wake_up or
	 * friends on every SPI message. Do this instead */
	drv_data->run = QUEUE_STOPPED;
	while ((!list_empty(&drv_data->queue) || drv_data->busy) && limit--) {
		spin_unlock_irqrestore(&drv_data->lock, flags);
		msleep(10);
		spin_lock_irqsave(&drv_data->lock, flags);
	}

	if (!list_empty(&drv_data->queue) || drv_data->busy)
		status = -EBUSY;

	spin_unlock_irqrestore(&drv_data->lock, flags);

	return status;
}

static int destroy_queue(struct driver_data *drv_data)
{
	int status;

	status = stop_queue(drv_data);
	/* we are unloading the module or failing to load (only two calls
	 * to this routine), and neither call can handle a return value.
	 * However, destroy_workqueue calls flush_workqueue, and that will
	 * block until all work is done.  If the reason that stop_queue
	 * timed out is that the work will never finish, then it does no
	 * good to call destroy_workqueue, so return anyway. */
	if (status != 0)
		return status;

	destroy_workqueue(drv_data->workqueue);

	return 0;
}

static int __devinit pxa2xx_spi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pxa2xx_spi_master *platform_info;
	struct spi_master *master;
	struct driver_data *drv_data;
	struct ssp_device *ssp;
	int status;

	platform_info = dev->platform_data;

	ssp = pxa_ssp_request(pdev->id, pdev->name);
	if (ssp == NULL) {
		dev_err(&pdev->dev, "failed to request SSP%d\n", pdev->id);
		return -ENODEV;
	}

	/* Allocate master with space for drv_data and null dma buffer */
	master = spi_alloc_master(dev, sizeof(struct driver_data) + 16);
	if (!master) {
		dev_err(&pdev->dev, "cannot alloc spi_master\n");
		pxa_ssp_free(ssp);
		return -ENOMEM;
	}
	drv_data = spi_master_get_devdata(master);
	drv_data->master = master;
	drv_data->master_info = platform_info;
	drv_data->pdev = pdev;
	drv_data->ssp = ssp;

	master->dev.parent = &pdev->dev;
	master->dev.of_node = pdev->dev.of_node;
	/* the spi->mode bits understood by this driver: */
	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;

	master->bus_num = pdev->id;
	master->num_chipselect = platform_info->num_chipselect;
	master->dma_alignment = DMA_ALIGNMENT;
	master->cleanup = cleanup;
	master->setup = setup;
	master->transfer = transfer;

	drv_data->ssp_type = ssp->type;
	drv_data->null_dma_buf = (u32 *)ALIGN((u32)(drv_data +
						sizeof(struct driver_data)), 8);

	drv_data->ioaddr = ssp->mmio_base;
	drv_data->ssdr_physical = ssp->phys_base + SSDR;
	if (pxa25x_ssp_comp(drv_data)) {
=======
static bool pxa2xx_spi_idma_filter(struct dma_chan *chan, void *param)
{
	return param == chan->device->dev;
}

static struct pxa2xx_spi_controller *
pxa2xx_spi_init_pdata(struct platform_device *pdev)
{
	struct pxa2xx_spi_controller *pdata;
	struct device *dev = &pdev->dev;
	struct device *parent = dev->parent;
	struct ssp_device *ssp;
	struct resource *res;
	enum pxa_ssp_type type = SSP_UNDEFINED;
	const void *match;
	bool is_lpss_priv;
	int status;
	u64 uid;

	is_lpss_priv = platform_get_resource_byname(pdev, IORESOURCE_MEM, "lpss_priv");

	match = device_get_match_data(dev);
	if (match)
		type = (uintptr_t)match;
	else if (is_lpss_priv) {
		u32 value;

		status = device_property_read_u32(dev, "intel,spi-pxa2xx-type", &value);
		if (status)
			return ERR_PTR(status);

		type = (enum pxa_ssp_type)value;
	}

	/* Validate the SSP type correctness */
	if (!(type > SSP_UNDEFINED && type < SSP_MAX))
		return ERR_PTR(-EINVAL);

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return ERR_PTR(-ENOMEM);

	ssp = &pdata->ssp;

	ssp->mmio_base = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(ssp->mmio_base))
		return ERR_CAST(ssp->mmio_base);

	ssp->phys_base = res->start;

	/* Platforms with iDMA 64-bit */
	if (is_lpss_priv) {
		pdata->tx_param = parent;
		pdata->rx_param = parent;
		pdata->dma_filter = pxa2xx_spi_idma_filter;
	}

	ssp->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(ssp->clk))
		return ERR_CAST(ssp->clk);

	ssp->irq = platform_get_irq(pdev, 0);
	if (ssp->irq < 0)
		return ERR_PTR(ssp->irq);

	ssp->type = type;
	ssp->dev = dev;

	status = acpi_dev_uid_to_integer(ACPI_COMPANION(dev), &uid);
	if (status)
		ssp->port_id = -1;
	else
		ssp->port_id = uid;

	pdata->is_target = device_property_read_bool(dev, "spi-slave");
	pdata->num_chipselect = 1;
	pdata->enable_dma = true;
	pdata->dma_burst_size = 1;

	return pdata;
}

static int pxa2xx_spi_fw_translate_cs(struct spi_controller *controller,
				      unsigned int cs)
{
	struct driver_data *drv_data = spi_controller_get_devdata(controller);

	if (has_acpi_companion(drv_data->ssp->dev)) {
		switch (drv_data->ssp_type) {
		/*
		 * For Atoms the ACPI DeviceSelection used by the Windows
		 * driver starts from 1 instead of 0 so translate it here
		 * to match what Linux expects.
		 */
		case LPSS_BYT_SSP:
		case LPSS_BSW_SSP:
			return cs - 1;

		default:
			break;
		}
	}

	return cs;
}

static size_t pxa2xx_spi_max_dma_transfer_size(struct spi_device *spi)
{
	return MAX_DMA_LEN;
}

static int pxa2xx_spi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pxa2xx_spi_controller *platform_info;
	struct spi_controller *controller;
	struct driver_data *drv_data;
	struct ssp_device *ssp;
	const struct lpss_config *config;
	int status;
	u32 tmp;

	platform_info = dev_get_platdata(dev);
	if (!platform_info) {
		platform_info = pxa2xx_spi_init_pdata(pdev);
		if (IS_ERR(platform_info)) {
			dev_err(&pdev->dev, "missing platform data\n");
			return PTR_ERR(platform_info);
		}
	}

	ssp = pxa_ssp_request(pdev->id, pdev->name);
	if (!ssp)
		ssp = &platform_info->ssp;

	if (!ssp->mmio_base) {
		dev_err(&pdev->dev, "failed to get SSP\n");
		return -ENODEV;
	}

	if (platform_info->is_target)
		controller = devm_spi_alloc_target(dev, sizeof(*drv_data));
	else
		controller = devm_spi_alloc_host(dev, sizeof(*drv_data));

	if (!controller) {
		dev_err(&pdev->dev, "cannot alloc spi_controller\n");
		status = -ENOMEM;
		goto out_error_controller_alloc;
	}
	drv_data = spi_controller_get_devdata(controller);
	drv_data->controller = controller;
	drv_data->controller_info = platform_info;
	drv_data->ssp = ssp;

	device_set_node(&controller->dev, dev_fwnode(dev));

	/* The spi->mode bits understood by this driver: */
	controller->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH | SPI_LOOP;

	controller->bus_num = ssp->port_id;
	controller->dma_alignment = DMA_ALIGNMENT;
	controller->cleanup = cleanup;
	controller->setup = setup;
	controller->set_cs = pxa2xx_spi_set_cs;
	controller->transfer_one = pxa2xx_spi_transfer_one;
	controller->target_abort = pxa2xx_spi_target_abort;
	controller->handle_err = pxa2xx_spi_handle_err;
	controller->unprepare_transfer_hardware = pxa2xx_spi_unprepare_transfer;
	controller->fw_translate_cs = pxa2xx_spi_fw_translate_cs;
	controller->auto_runtime_pm = true;
	controller->flags = SPI_CONTROLLER_MUST_RX | SPI_CONTROLLER_MUST_TX;

	drv_data->ssp_type = ssp->type;

	if (pxa25x_ssp_comp(drv_data)) {
		switch (drv_data->ssp_type) {
		case QUARK_X1000_SSP:
			controller->bits_per_word_mask = SPI_BPW_RANGE_MASK(4, 32);
			break;
		default:
			controller->bits_per_word_mask = SPI_BPW_RANGE_MASK(4, 16);
			break;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		drv_data->int_cr1 = SSCR1_TIE | SSCR1_RIE;
		drv_data->dma_cr1 = 0;
		drv_data->clear_sr = SSSR_ROR;
		drv_data->mask_sr = SSSR_RFS | SSSR_TFS | SSSR_ROR;
	} else {
<<<<<<< HEAD
		drv_data->int_cr1 = SSCR1_TIE | SSCR1_RIE | SSCR1_TINTE;
		drv_data->dma_cr1 = SSCR1_TSRE | SSCR1_RSRE | SSCR1_TINTE;
		drv_data->clear_sr = SSSR_ROR | SSSR_TINT;
		drv_data->mask_sr = SSSR_TINT | SSSR_RFS | SSSR_TFS | SSSR_ROR;
=======
		controller->bits_per_word_mask = SPI_BPW_RANGE_MASK(4, 32);
		drv_data->int_cr1 = SSCR1_TIE | SSCR1_RIE | SSCR1_TINTE;
		drv_data->dma_cr1 = DEFAULT_DMA_CR1;
		drv_data->clear_sr = SSSR_ROR | SSSR_TINT;
		drv_data->mask_sr = SSSR_TINT | SSSR_RFS | SSSR_TFS
						| SSSR_ROR | SSSR_TUR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	status = request_irq(ssp->irq, ssp_int, IRQF_SHARED, dev_name(dev),
			drv_data);
	if (status < 0) {
		dev_err(&pdev->dev, "cannot get IRQ %d\n", ssp->irq);
<<<<<<< HEAD
		goto out_error_master_alloc;
	}

	/* Setup DMA if requested */
	drv_data->tx_channel = -1;
	drv_data->rx_channel = -1;
	if (platform_info->enable_dma) {

		/* Get two DMA channels	(rx and tx) */
		drv_data->rx_channel = pxa_request_dma("pxa2xx_spi_ssp_rx",
							DMA_PRIO_HIGH,
							dma_handler,
							drv_data);
		if (drv_data->rx_channel < 0) {
			dev_err(dev, "problem (%d) requesting rx channel\n",
				drv_data->rx_channel);
			status = -ENODEV;
			goto out_error_irq_alloc;
		}
		drv_data->tx_channel = pxa_request_dma("pxa2xx_spi_ssp_tx",
							DMA_PRIO_MEDIUM,
							dma_handler,
							drv_data);
		if (drv_data->tx_channel < 0) {
			dev_err(dev, "problem (%d) requesting tx channel\n",
				drv_data->tx_channel);
			status = -ENODEV;
			goto out_error_dma_alloc;
		}

		DRCMR(ssp->drcmr_rx) = DRCMR_MAPVLD | drv_data->rx_channel;
		DRCMR(ssp->drcmr_tx) = DRCMR_MAPVLD | drv_data->tx_channel;
	}

	/* Enable SOC clock */
	clk_enable(ssp->clk);

	/* Load default SSP configuration */
	write_SSCR0(0, drv_data->ioaddr);
	write_SSCR1(SSCR1_RxTresh(RX_THRESH_DFLT) |
				SSCR1_TxTresh(TX_THRESH_DFLT),
				drv_data->ioaddr);
	write_SSCR0(SSCR0_SCR(2)
			| SSCR0_Motorola
			| SSCR0_DataSize(8),
			drv_data->ioaddr);
	if (!pxa25x_ssp_comp(drv_data))
		write_SSTO(0, drv_data->ioaddr);
	write_SSPSP(0, drv_data->ioaddr);

	/* Initial and start queue */
	status = init_queue(drv_data);
	if (status != 0) {
		dev_err(&pdev->dev, "problem initializing queue\n");
		goto out_error_clock_enabled;
	}
	status = start_queue(drv_data);
	if (status != 0) {
		dev_err(&pdev->dev, "problem starting queue\n");
		goto out_error_clock_enabled;
	}

	/* Register with the SPI framework */
	platform_set_drvdata(pdev, drv_data);
	status = spi_register_master(master);
	if (status != 0) {
		dev_err(&pdev->dev, "problem registering spi master\n");
		goto out_error_queue_alloc;
=======
		goto out_error_controller_alloc;
	}

	/* Setup DMA if requested */
	if (platform_info->enable_dma) {
		status = pxa2xx_spi_dma_setup(drv_data);
		if (status) {
			dev_warn(dev, "no DMA channels available, using PIO\n");
			platform_info->enable_dma = false;
		} else {
			controller->can_dma = pxa2xx_spi_can_dma;
			controller->max_dma_len = MAX_DMA_LEN;
			controller->max_transfer_size =
				pxa2xx_spi_max_dma_transfer_size;
		}
	}

	/* Enable SOC clock */
	status = clk_prepare_enable(ssp->clk);
	if (status)
		goto out_error_dma_irq_alloc;

	controller->max_speed_hz = clk_get_rate(ssp->clk);
	/*
	 * Set minimum speed for all other platforms than Intel Quark which is
	 * able do under 1 Hz transfers.
	 */
	if (!pxa25x_ssp_comp(drv_data))
		controller->min_speed_hz =
			DIV_ROUND_UP(controller->max_speed_hz, 4096);
	else if (!is_quark_x1000_ssp(drv_data))
		controller->min_speed_hz =
			DIV_ROUND_UP(controller->max_speed_hz, 512);

	pxa_ssp_disable(ssp);

	/* Load default SSP configuration */
	switch (drv_data->ssp_type) {
	case QUARK_X1000_SSP:
		tmp = QUARK_X1000_SSCR1_RxTresh(RX_THRESH_QUARK_X1000_DFLT) |
		      QUARK_X1000_SSCR1_TxTresh(TX_THRESH_QUARK_X1000_DFLT);
		pxa2xx_spi_write(drv_data, SSCR1, tmp);

		/* Using the Motorola SPI protocol and use 8 bit frame */
		tmp = QUARK_X1000_SSCR0_Motorola | QUARK_X1000_SSCR0_DataSize(8);
		pxa2xx_spi_write(drv_data, SSCR0, tmp);
		break;
	case CE4100_SSP:
		tmp = CE4100_SSCR1_RxTresh(RX_THRESH_CE4100_DFLT) |
		      CE4100_SSCR1_TxTresh(TX_THRESH_CE4100_DFLT);
		pxa2xx_spi_write(drv_data, SSCR1, tmp);
		tmp = SSCR0_SCR(2) | SSCR0_Motorola | SSCR0_DataSize(8);
		pxa2xx_spi_write(drv_data, SSCR0, tmp);
		break;
	default:

		if (spi_controller_is_target(controller)) {
			tmp = SSCR1_SCFR |
			      SSCR1_SCLKDIR |
			      SSCR1_SFRMDIR |
			      SSCR1_RxTresh(2) |
			      SSCR1_TxTresh(1) |
			      SSCR1_SPH;
		} else {
			tmp = SSCR1_RxTresh(RX_THRESH_DFLT) |
			      SSCR1_TxTresh(TX_THRESH_DFLT);
		}
		pxa2xx_spi_write(drv_data, SSCR1, tmp);
		tmp = SSCR0_Motorola | SSCR0_DataSize(8);
		if (!spi_controller_is_target(controller))
			tmp |= SSCR0_SCR(2);
		pxa2xx_spi_write(drv_data, SSCR0, tmp);
		break;
	}

	if (!pxa25x_ssp_comp(drv_data))
		pxa2xx_spi_write(drv_data, SSTO, 0);

	if (!is_quark_x1000_ssp(drv_data))
		pxa2xx_spi_write(drv_data, SSPSP, 0);

	if (is_lpss_ssp(drv_data)) {
		lpss_ssp_setup(drv_data);
		config = lpss_get_config(drv_data);
		if (config->reg_capabilities >= 0) {
			tmp = __lpss_ssp_read_priv(drv_data,
						   config->reg_capabilities);
			tmp &= LPSS_CAPS_CS_EN_MASK;
			tmp >>= LPSS_CAPS_CS_EN_SHIFT;
			platform_info->num_chipselect = ffz(tmp);
		} else if (config->cs_num) {
			platform_info->num_chipselect = config->cs_num;
		}
	}
	controller->num_chipselect = platform_info->num_chipselect;
	controller->use_gpio_descriptors = true;

	if (platform_info->is_target) {
		drv_data->gpiod_ready = devm_gpiod_get_optional(dev,
						"ready", GPIOD_OUT_LOW);
		if (IS_ERR(drv_data->gpiod_ready)) {
			status = PTR_ERR(drv_data->gpiod_ready);
			goto out_error_clock_enabled;
		}
	}

	pm_runtime_set_autosuspend_delay(&pdev->dev, 50);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_set_active(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

	/* Register with the SPI framework */
	platform_set_drvdata(pdev, drv_data);
	status = spi_register_controller(controller);
	if (status) {
		dev_err(&pdev->dev, "problem registering SPI controller\n");
		goto out_error_pm_runtime_enabled;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return status;

<<<<<<< HEAD
out_error_queue_alloc:
	destroy_queue(drv_data);

out_error_clock_enabled:
	clk_disable(ssp->clk);

out_error_dma_alloc:
	if (drv_data->tx_channel != -1)
		pxa_free_dma(drv_data->tx_channel);
	if (drv_data->rx_channel != -1)
		pxa_free_dma(drv_data->rx_channel);

out_error_irq_alloc:
	free_irq(ssp->irq, drv_data);

out_error_master_alloc:
	spi_master_put(master);
=======
out_error_pm_runtime_enabled:
	pm_runtime_disable(&pdev->dev);

out_error_clock_enabled:
	clk_disable_unprepare(ssp->clk);

out_error_dma_irq_alloc:
	pxa2xx_spi_dma_release(drv_data);
	free_irq(ssp->irq, drv_data);

out_error_controller_alloc:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pxa_ssp_free(ssp);
	return status;
}

<<<<<<< HEAD
static int pxa2xx_spi_remove(struct platform_device *pdev)
{
	struct driver_data *drv_data = platform_get_drvdata(pdev);
	struct ssp_device *ssp;
	int status = 0;

	if (!drv_data)
		return 0;
	ssp = drv_data->ssp;

	/* Remove the queue */
	status = destroy_queue(drv_data);
	if (status != 0)
		/* the kernel does not check the return status of this
		 * this routine (mod->exit, within the kernel).  Therefore
		 * nothing is gained by returning from here, the module is
		 * going away regardless, and we should not leave any more
		 * resources allocated than necessary.  We cannot free the
		 * message memory in drv_data->queue, but we can release the
		 * resources below.  I think the kernel should honor -EBUSY
		 * returns but... */
		dev_err(&pdev->dev, "pxa2xx_spi_remove: workqueue will not "
			"complete, message memory not freed\n");

	/* Disable the SSP at the peripheral and SOC level */
	write_SSCR0(0, drv_data->ioaddr);
	clk_disable(ssp->clk);

	/* Release DMA */
	if (drv_data->master_info->enable_dma) {
		DRCMR(ssp->drcmr_rx) = 0;
		DRCMR(ssp->drcmr_tx) = 0;
		pxa_free_dma(drv_data->tx_channel);
		pxa_free_dma(drv_data->rx_channel);
	}
=======
static void pxa2xx_spi_remove(struct platform_device *pdev)
{
	struct driver_data *drv_data = platform_get_drvdata(pdev);
	struct ssp_device *ssp = drv_data->ssp;

	pm_runtime_get_sync(&pdev->dev);

	spi_unregister_controller(drv_data->controller);

	/* Disable the SSP at the peripheral and SOC level */
	pxa_ssp_disable(ssp);
	clk_disable_unprepare(ssp->clk);

	/* Release DMA */
	if (drv_data->controller_info->enable_dma)
		pxa2xx_spi_dma_release(drv_data);

	pm_runtime_put_noidle(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Release IRQ */
	free_irq(ssp->irq, drv_data);

	/* Release SSP */
	pxa_ssp_free(ssp);
<<<<<<< HEAD

	/* Disconnect from the SPI framework */
	spi_unregister_master(drv_data->master);

	/* Prevent double remove */
	platform_set_drvdata(pdev, NULL);

	return 0;
}

static void pxa2xx_spi_shutdown(struct platform_device *pdev)
{
	int status = 0;

	if ((status = pxa2xx_spi_remove(pdev)) != 0)
		dev_err(&pdev->dev, "shutdown failed with %d\n", status);
}

#ifdef CONFIG_PM
=======
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int pxa2xx_spi_suspend(struct device *dev)
{
	struct driver_data *drv_data = dev_get_drvdata(dev);
	struct ssp_device *ssp = drv_data->ssp;
<<<<<<< HEAD
	int status = 0;

	status = stop_queue(drv_data);
	if (status != 0)
		return status;
	write_SSCR0(0, drv_data->ioaddr);
	clk_disable(ssp->clk);
=======
	int status;

	status = spi_controller_suspend(drv_data->controller);
	if (status)
		return status;

	pxa_ssp_disable(ssp);

	if (!pm_runtime_suspended(dev))
		clk_disable_unprepare(ssp->clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int pxa2xx_spi_resume(struct device *dev)
{
	struct driver_data *drv_data = dev_get_drvdata(dev);
	struct ssp_device *ssp = drv_data->ssp;
<<<<<<< HEAD
	int status = 0;

	if (drv_data->rx_channel != -1)
		DRCMR(drv_data->ssp->drcmr_rx) =
			DRCMR_MAPVLD | drv_data->rx_channel;
	if (drv_data->tx_channel != -1)
		DRCMR(drv_data->ssp->drcmr_tx) =
			DRCMR_MAPVLD | drv_data->tx_channel;

	/* Enable the SSP clock */
	clk_enable(ssp->clk);

	/* Start the queue running */
	status = start_queue(drv_data);
	if (status != 0) {
		dev_err(dev, "problem starting queue (%d)\n", status);
		return status;
	}

	return 0;
}

static const struct dev_pm_ops pxa2xx_spi_pm_ops = {
	.suspend	= pxa2xx_spi_suspend,
	.resume		= pxa2xx_spi_resume,
};
#endif

static struct platform_driver driver = {
	.driver = {
		.name	= "pxa2xx-spi",
		.owner	= THIS_MODULE,
#ifdef CONFIG_PM
		.pm	= &pxa2xx_spi_pm_ops,
#endif
	},
	.probe = pxa2xx_spi_probe,
	.remove = pxa2xx_spi_remove,
	.shutdown = pxa2xx_spi_shutdown,
=======
	int status;

	/* Enable the SSP clock */
	if (!pm_runtime_suspended(dev)) {
		status = clk_prepare_enable(ssp->clk);
		if (status)
			return status;
	}

	/* Start the queue running */
	return spi_controller_resume(drv_data->controller);
}

static int pxa2xx_spi_runtime_suspend(struct device *dev)
{
	struct driver_data *drv_data = dev_get_drvdata(dev);

	clk_disable_unprepare(drv_data->ssp->clk);
	return 0;
}

static int pxa2xx_spi_runtime_resume(struct device *dev)
{
	struct driver_data *drv_data = dev_get_drvdata(dev);

	return clk_prepare_enable(drv_data->ssp->clk);
}

static const struct dev_pm_ops pxa2xx_spi_pm_ops = {
	SYSTEM_SLEEP_PM_OPS(pxa2xx_spi_suspend, pxa2xx_spi_resume)
	RUNTIME_PM_OPS(pxa2xx_spi_runtime_suspend, pxa2xx_spi_runtime_resume, NULL)
};

#ifdef CONFIG_ACPI
static const struct acpi_device_id pxa2xx_spi_acpi_match[] = {
	{ "80860F0E", LPSS_BYT_SSP },
	{ "8086228E", LPSS_BSW_SSP },
	{ "INT33C0", LPSS_LPT_SSP },
	{ "INT33C1", LPSS_LPT_SSP },
	{ "INT3430", LPSS_LPT_SSP },
	{ "INT3431", LPSS_LPT_SSP },
	{}
};
MODULE_DEVICE_TABLE(acpi, pxa2xx_spi_acpi_match);
#endif

static const struct of_device_id pxa2xx_spi_of_match[] __maybe_unused = {
	{ .compatible = "marvell,mmp2-ssp", .data = (void *)MMP2_SSP },
	{}
};
MODULE_DEVICE_TABLE(of, pxa2xx_spi_of_match);

static struct platform_driver driver = {
	.driver = {
		.name	= "pxa2xx-spi",
		.pm	= pm_ptr(&pxa2xx_spi_pm_ops),
		.acpi_match_table = ACPI_PTR(pxa2xx_spi_acpi_match),
		.of_match_table = of_match_ptr(pxa2xx_spi_of_match),
	},
	.probe = pxa2xx_spi_probe,
	.remove_new = pxa2xx_spi_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init pxa2xx_spi_init(void)
{
	return platform_driver_register(&driver);
}
subsys_initcall(pxa2xx_spi_init);

static void __exit pxa2xx_spi_exit(void)
{
	platform_driver_unregister(&driver);
}
module_exit(pxa2xx_spi_exit);
<<<<<<< HEAD
=======

MODULE_SOFTDEP("pre: dw_dmac");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
