<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Freescale SPI controller driver.
 *
 * Maintainer: Kumar Gala
 *
 * Copyright (C) 2006 Polycom, Inc.
 * Copyright 2010 Freescale Semiconductor, Inc.
 *
 * CPM SPI and QE buffer descriptors mode support:
 * Copyright (c) 2009  MontaVista Software, Inc.
 * Author: Anton Vorontsov <avorontsov@ru.mvista.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/platform_device.h>
#include <linux/fsl_devices.h>
#include <linux/dma-mapping.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>

#include <sysdev/fsl_soc.h>
#include <asm/cpm.h>
#include <asm/qe.h>

#include "spi-fsl-lib.h"

/* CPM1 and CPM2 are mutually exclusive. */
#ifdef CONFIG_CPM1
#include <asm/cpm1.h>
#define CPM_SPI_CMD mk_cr_cmd(CPM_CR_CH_SPI, 0)
#else
#include <asm/cpm2.h>
#define CPM_SPI_CMD mk_cr_cmd(CPM_CR_SPI_PAGE, CPM_CR_SPI_SBLOCK, 0, 0)
#endif

/* SPI Controller registers */
struct fsl_spi_reg {
	u8 res1[0x20];
	__be32 mode;
	__be32 event;
	__be32 mask;
	__be32 command;
	__be32 transmit;
	__be32 receive;
};

/* SPI Controller mode register definitions */
#define	SPMODE_LOOP		(1 << 30)
#define	SPMODE_CI_INACTIVEHIGH	(1 << 29)
#define	SPMODE_CP_BEGIN_EDGECLK	(1 << 28)
#define	SPMODE_DIV16		(1 << 27)
#define	SPMODE_REV		(1 << 26)
#define	SPMODE_MS		(1 << 25)
#define	SPMODE_ENABLE		(1 << 24)
#define	SPMODE_LEN(x)		((x) << 20)
#define	SPMODE_PM(x)		((x) << 16)
#define	SPMODE_OP		(1 << 14)
#define	SPMODE_CG(x)		((x) << 7)

/*
 * Default for SPI Mode:
 *	SPI MODE 0 (inactive low, phase middle, MSB, 8-bit length, slow clk
 */
#define	SPMODE_INIT_VAL (SPMODE_CI_INACTIVEHIGH | SPMODE_DIV16 | SPMODE_REV | \
			 SPMODE_MS | SPMODE_LEN(7) | SPMODE_PM(0xf))

/* SPIE register values */
#define	SPIE_NE		0x00000200	/* Not empty */
#define	SPIE_NF		0x00000100	/* Not full */

/* SPIM register values */
#define	SPIM_NE		0x00000200	/* Not empty */
#define	SPIM_NF		0x00000100	/* Not full */

#define	SPIE_TXB	0x00000200	/* Last char is written to tx fifo */
#define	SPIE_RXB	0x00000100	/* Last char is written to rx buf */

/* SPCOM register values */
#define	SPCOM_STR	(1 << 23)	/* Start transmit */

#define	SPI_PRAM_SIZE	0x100
#define	SPI_MRBLR	((unsigned int)PAGE_SIZE)

static void *fsl_dummy_rx;
static DEFINE_MUTEX(fsl_dummy_rx_lock);
static int fsl_dummy_rx_refcnt;

static void fsl_spi_change_mode(struct spi_device *spi)
{
	struct mpc8xxx_spi *mspi = spi_master_get_devdata(spi->master);
	struct spi_mpc8xxx_cs *cs = spi->controller_state;
	struct fsl_spi_reg *reg_base = mspi->reg_base;
=======
 * GRLIB support:
 * Copyright (c) 2012 Aeroflex Gaisler AB.
 * Author: Andreas Larsson <andreas@gaisler.com>
 */
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/fsl_devices.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/types.h>

#ifdef CONFIG_FSL_SOC
#include <sysdev/fsl_soc.h>
#endif

/* Specific to the MPC8306/MPC8309 */
#define IMMR_SPI_CS_OFFSET 0x14c
#define SPI_BOOT_SEL_BIT   0x80000000

#include "spi-fsl-lib.h"
#include "spi-fsl-cpm.h"
#include "spi-fsl-spi.h"

#define TYPE_FSL	0
#define TYPE_GRLIB	1

struct fsl_spi_match_data {
	int type;
};

static struct fsl_spi_match_data of_fsl_spi_fsl_config = {
	.type = TYPE_FSL,
};

static struct fsl_spi_match_data of_fsl_spi_grlib_config = {
	.type = TYPE_GRLIB,
};

static const struct of_device_id of_fsl_spi_match[] = {
	{
		.compatible = "fsl,spi",
		.data = &of_fsl_spi_fsl_config,
	},
	{
		.compatible = "aeroflexgaisler,spictrl",
		.data = &of_fsl_spi_grlib_config,
	},
	{}
};
MODULE_DEVICE_TABLE(of, of_fsl_spi_match);

static int fsl_spi_get_type(struct device *dev)
{
	const struct of_device_id *match;

	if (dev->of_node) {
		match = of_match_node(of_fsl_spi_match, dev->of_node);
		if (match && match->data)
			return ((struct fsl_spi_match_data *)match->data)->type;
	}
	return TYPE_FSL;
}

static void fsl_spi_change_mode(struct spi_device *spi)
{
	struct mpc8xxx_spi *mspi = spi_controller_get_devdata(spi->controller);
	struct spi_mpc8xxx_cs *cs = spi->controller_state;
	struct fsl_spi_reg __iomem *reg_base = mspi->reg_base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be32 __iomem *mode = &reg_base->mode;
	unsigned long flags;

	if (cs->hw_mode == mpc8xxx_spi_read_reg(mode))
		return;

	/* Turn off IRQs locally to minimize time that SPI is disabled. */
	local_irq_save(flags);

	/* Turn off SPI unit prior changing mode */
	mpc8xxx_spi_write_reg(mode, cs->hw_mode & ~SPMODE_ENABLE);

	/* When in CPM mode, we need to reinit tx and rx. */
	if (mspi->flags & SPI_CPM_MODE) {
<<<<<<< HEAD
		if (mspi->flags & SPI_QE) {
			qe_issue_cmd(QE_INIT_TX_RX, mspi->subblock,
				     QE_CR_PROTOCOL_UNSPECIFIED, 0);
		} else {
			cpm_command(CPM_SPI_CMD, CPM_CR_INIT_TRX);
			if (mspi->flags & SPI_CPM1) {
				out_be16(&mspi->pram->rbptr,
					 in_be16(&mspi->pram->rbase));
				out_be16(&mspi->pram->tbptr,
					 in_be16(&mspi->pram->tbase));
			}
		}
=======
		fsl_spi_cpm_reinit_txrx(mspi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mpc8xxx_spi_write_reg(mode, cs->hw_mode);
	local_irq_restore(flags);
}

<<<<<<< HEAD
static void fsl_spi_chipselect(struct spi_device *spi, int value)
{
	struct mpc8xxx_spi *mpc8xxx_spi = spi_master_get_devdata(spi->master);
	struct fsl_spi_platform_data *pdata;
	bool pol = spi->mode & SPI_CS_HIGH;
	struct spi_mpc8xxx_cs	*cs = spi->controller_state;

	pdata = spi->dev.parent->parent->platform_data;

	if (value == BITBANG_CS_INACTIVE) {
		if (pdata->cs_control)
			pdata->cs_control(spi, !pol);
	}

	if (value == BITBANG_CS_ACTIVE) {
		mpc8xxx_spi->rx_shift = cs->rx_shift;
		mpc8xxx_spi->tx_shift = cs->tx_shift;
		mpc8xxx_spi->get_rx = cs->get_rx;
		mpc8xxx_spi->get_tx = cs->get_tx;

		fsl_spi_change_mode(spi);

		if (pdata->cs_control)
			pdata->cs_control(spi, pol);
	}
}

static int mspi_apply_cpu_mode_quirks(struct spi_mpc8xxx_cs *cs,
				struct spi_device *spi,
				struct mpc8xxx_spi *mpc8xxx_spi,
				int bits_per_word)
=======
static void fsl_spi_qe_cpu_set_shifts(u32 *rx_shift, u32 *tx_shift,
				      int bits_per_word, int msb_first)
{
	*rx_shift = 0;
	*tx_shift = 0;
	if (msb_first) {
		if (bits_per_word <= 8) {
			*rx_shift = 16;
			*tx_shift = 24;
		} else if (bits_per_word <= 16) {
			*rx_shift = 16;
			*tx_shift = 16;
		}
	} else {
		if (bits_per_word <= 8)
			*rx_shift = 8;
	}
}

static void fsl_spi_grlib_set_shifts(u32 *rx_shift, u32 *tx_shift,
				     int bits_per_word, int msb_first)
{
	*rx_shift = 0;
	*tx_shift = 0;
	if (bits_per_word <= 16) {
		if (msb_first) {
			*rx_shift = 16; /* LSB in bit 16 */
			*tx_shift = 32 - bits_per_word; /* MSB in bit 31 */
		} else {
			*rx_shift = 16 - bits_per_word; /* MSB in bit 15 */
		}
	}
}

static void mspi_apply_cpu_mode_quirks(struct spi_mpc8xxx_cs *cs,
				       struct spi_device *spi,
				       struct mpc8xxx_spi *mpc8xxx_spi,
				       int bits_per_word)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	cs->rx_shift = 0;
	cs->tx_shift = 0;
	if (bits_per_word <= 8) {
		cs->get_rx = mpc8xxx_spi_rx_buf_u8;
		cs->get_tx = mpc8xxx_spi_tx_buf_u8;
<<<<<<< HEAD
		if (mpc8xxx_spi->flags & SPI_QE_CPU_MODE) {
			cs->rx_shift = 16;
			cs->tx_shift = 24;
		}
	} else if (bits_per_word <= 16) {
		cs->get_rx = mpc8xxx_spi_rx_buf_u16;
		cs->get_tx = mpc8xxx_spi_tx_buf_u16;
		if (mpc8xxx_spi->flags & SPI_QE_CPU_MODE) {
			cs->rx_shift = 16;
			cs->tx_shift = 16;
		}
	} else if (bits_per_word <= 32) {
		cs->get_rx = mpc8xxx_spi_rx_buf_u32;
		cs->get_tx = mpc8xxx_spi_tx_buf_u32;
	} else
		return -EINVAL;

	if (mpc8xxx_spi->flags & SPI_QE_CPU_MODE &&
	    spi->mode & SPI_LSB_FIRST) {
		cs->tx_shift = 0;
		if (bits_per_word <= 8)
			cs->rx_shift = 8;
		else
			cs->rx_shift = 0;
	}
=======
	} else if (bits_per_word <= 16) {
		cs->get_rx = mpc8xxx_spi_rx_buf_u16;
		cs->get_tx = mpc8xxx_spi_tx_buf_u16;
	} else if (bits_per_word <= 32) {
		cs->get_rx = mpc8xxx_spi_rx_buf_u32;
		cs->get_tx = mpc8xxx_spi_tx_buf_u32;
	}

	if (mpc8xxx_spi->set_shifts)
		mpc8xxx_spi->set_shifts(&cs->rx_shift, &cs->tx_shift,
					bits_per_word,
					!(spi->mode & SPI_LSB_FIRST));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mpc8xxx_spi->rx_shift = cs->rx_shift;
	mpc8xxx_spi->tx_shift = cs->tx_shift;
	mpc8xxx_spi->get_rx = cs->get_rx;
	mpc8xxx_spi->get_tx = cs->get_tx;
<<<<<<< HEAD

	return bits_per_word;
}

static int mspi_apply_qe_mode_quirks(struct spi_mpc8xxx_cs *cs,
				struct spi_device *spi,
				int bits_per_word)
{
	/* QE uses Little Endian for words > 8
	 * so transform all words > 8 into 8 bits
	 * Unfortnatly that doesn't work for LSB so
	 * reject these for now */
	/* Note: 32 bits word, LSB works iff
	 * tfcr/rfcr is set to CPMFCR_GBL */
	if (spi->mode & SPI_LSB_FIRST &&
	    bits_per_word > 8)
		return -EINVAL;
	if (bits_per_word > 8)
		return 8; /* pretend its 8 bits */
	return bits_per_word;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fsl_spi_setup_transfer(struct spi_device *spi,
					struct spi_transfer *t)
{
	struct mpc8xxx_spi *mpc8xxx_spi;
	int bits_per_word = 0;
	u8 pm;
	u32 hz = 0;
	struct spi_mpc8xxx_cs	*cs = spi->controller_state;

<<<<<<< HEAD
	mpc8xxx_spi = spi_master_get_devdata(spi->master);
=======
	mpc8xxx_spi = spi_controller_get_devdata(spi->controller);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (t) {
		bits_per_word = t->bits_per_word;
		hz = t->speed_hz;
	}

	/* spi_transfer level calls that work per-word */
	if (!bits_per_word)
		bits_per_word = spi->bits_per_word;

<<<<<<< HEAD
	/* Make sure its a bit width we support [4..16, 32] */
	if ((bits_per_word < 4)
	    || ((bits_per_word > 16) && (bits_per_word != 32)))
		return -EINVAL;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!hz)
		hz = spi->max_speed_hz;

	if (!(mpc8xxx_spi->flags & SPI_CPM_MODE))
<<<<<<< HEAD
		bits_per_word = mspi_apply_cpu_mode_quirks(cs, spi,
							   mpc8xxx_spi,
							   bits_per_word);
	else if (mpc8xxx_spi->flags & SPI_QE)
		bits_per_word = mspi_apply_qe_mode_quirks(cs, spi,
							  bits_per_word);

	if (bits_per_word < 0)
		return bits_per_word;
=======
		mspi_apply_cpu_mode_quirks(cs, spi, mpc8xxx_spi, bits_per_word);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (bits_per_word == 32)
		bits_per_word = 0;
	else
		bits_per_word = bits_per_word - 1;

	/* mask out bits we are going to set */
	cs->hw_mode &= ~(SPMODE_LEN(0xF) | SPMODE_DIV16
				  | SPMODE_PM(0xF));

	cs->hw_mode |= SPMODE_LEN(bits_per_word);

	if ((mpc8xxx_spi->spibrg / hz) > 64) {
		cs->hw_mode |= SPMODE_DIV16;
		pm = (mpc8xxx_spi->spibrg - 1) / (hz * 64) + 1;
<<<<<<< HEAD

		WARN_ONCE(pm > 16, "%s: Requested speed is too low: %d Hz. "
			  "Will use %d Hz instead.\n", dev_name(&spi->dev),
			  hz, mpc8xxx_spi->spibrg / 1024);
=======
		WARN_ONCE(pm > 16,
			  "%s: Requested speed is too low: %d Hz. Will use %d Hz instead.\n",
			  dev_name(&spi->dev), hz, mpc8xxx_spi->spibrg / 1024);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (pm > 16)
			pm = 16;
	} else {
		pm = (mpc8xxx_spi->spibrg - 1) / (hz * 4) + 1;
	}
	if (pm)
		pm--;

	cs->hw_mode |= SPMODE_PM(pm);

	fsl_spi_change_mode(spi);
	return 0;
}

<<<<<<< HEAD
static void fsl_spi_cpm_bufs_start(struct mpc8xxx_spi *mspi)
{
	struct cpm_buf_desc __iomem *tx_bd = mspi->tx_bd;
	struct cpm_buf_desc __iomem *rx_bd = mspi->rx_bd;
	unsigned int xfer_len = min(mspi->count, SPI_MRBLR);
	unsigned int xfer_ofs;
	struct fsl_spi_reg *reg_base = mspi->reg_base;

	xfer_ofs = mspi->xfer_in_progress->len - mspi->count;

	if (mspi->rx_dma == mspi->dma_dummy_rx)
		out_be32(&rx_bd->cbd_bufaddr, mspi->rx_dma);
	else
		out_be32(&rx_bd->cbd_bufaddr, mspi->rx_dma + xfer_ofs);
	out_be16(&rx_bd->cbd_datlen, 0);
	out_be16(&rx_bd->cbd_sc, BD_SC_EMPTY | BD_SC_INTRPT | BD_SC_WRAP);

	if (mspi->tx_dma == mspi->dma_dummy_tx)
		out_be32(&tx_bd->cbd_bufaddr, mspi->tx_dma);
	else
		out_be32(&tx_bd->cbd_bufaddr, mspi->tx_dma + xfer_ofs);
	out_be16(&tx_bd->cbd_datlen, xfer_len);
	out_be16(&tx_bd->cbd_sc, BD_SC_READY | BD_SC_INTRPT | BD_SC_WRAP |
				 BD_SC_LAST);

	/* start transfer */
	mpc8xxx_spi_write_reg(&reg_base->command, SPCOM_STR);
}

static int fsl_spi_cpm_bufs(struct mpc8xxx_spi *mspi,
				struct spi_transfer *t, bool is_dma_mapped)
{
	struct device *dev = mspi->dev;
	struct fsl_spi_reg *reg_base = mspi->reg_base;

	if (is_dma_mapped) {
		mspi->map_tx_dma = 0;
		mspi->map_rx_dma = 0;
	} else {
		mspi->map_tx_dma = 1;
		mspi->map_rx_dma = 1;
	}

	if (!t->tx_buf) {
		mspi->tx_dma = mspi->dma_dummy_tx;
		mspi->map_tx_dma = 0;
	}

	if (!t->rx_buf) {
		mspi->rx_dma = mspi->dma_dummy_rx;
		mspi->map_rx_dma = 0;
	}

	if (mspi->map_tx_dma) {
		void *nonconst_tx = (void *)mspi->tx; /* shut up gcc */

		mspi->tx_dma = dma_map_single(dev, nonconst_tx, t->len,
					      DMA_TO_DEVICE);
		if (dma_mapping_error(dev, mspi->tx_dma)) {
			dev_err(dev, "unable to map tx dma\n");
			return -ENOMEM;
		}
	} else if (t->tx_buf) {
		mspi->tx_dma = t->tx_dma;
	}

	if (mspi->map_rx_dma) {
		mspi->rx_dma = dma_map_single(dev, mspi->rx, t->len,
					      DMA_FROM_DEVICE);
		if (dma_mapping_error(dev, mspi->rx_dma)) {
			dev_err(dev, "unable to map rx dma\n");
			goto err_rx_dma;
		}
	} else if (t->rx_buf) {
		mspi->rx_dma = t->rx_dma;
	}

	/* enable rx ints */
	mpc8xxx_spi_write_reg(&reg_base->mask, SPIE_RXB);

	mspi->xfer_in_progress = t;
	mspi->count = t->len;

	/* start CPM transfers */
	fsl_spi_cpm_bufs_start(mspi);

	return 0;

err_rx_dma:
	if (mspi->map_tx_dma)
		dma_unmap_single(dev, mspi->tx_dma, t->len, DMA_TO_DEVICE);
	return -ENOMEM;
}

static void fsl_spi_cpm_bufs_complete(struct mpc8xxx_spi *mspi)
{
	struct device *dev = mspi->dev;
	struct spi_transfer *t = mspi->xfer_in_progress;

	if (mspi->map_tx_dma)
		dma_unmap_single(dev, mspi->tx_dma, t->len, DMA_TO_DEVICE);
	if (mspi->map_rx_dma)
		dma_unmap_single(dev, mspi->rx_dma, t->len, DMA_FROM_DEVICE);
	mspi->xfer_in_progress = NULL;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int fsl_spi_cpu_bufs(struct mpc8xxx_spi *mspi,
				struct spi_transfer *t, unsigned int len)
{
	u32 word;
<<<<<<< HEAD
	struct fsl_spi_reg *reg_base = mspi->reg_base;
=======
	struct fsl_spi_reg __iomem *reg_base = mspi->reg_base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mspi->count = len;

	/* enable rx ints */
	mpc8xxx_spi_write_reg(&reg_base->mask, SPIM_NE);

	/* transmit word */
	word = mspi->get_tx(mspi);
	mpc8xxx_spi_write_reg(&reg_base->transmit, word);

	return 0;
}

static int fsl_spi_bufs(struct spi_device *spi, struct spi_transfer *t,
			    bool is_dma_mapped)
{
<<<<<<< HEAD
	struct mpc8xxx_spi *mpc8xxx_spi = spi_master_get_devdata(spi->master);
	struct fsl_spi_reg *reg_base;
=======
	struct mpc8xxx_spi *mpc8xxx_spi = spi_controller_get_devdata(spi->controller);
	struct fsl_spi_reg __iomem *reg_base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int len = t->len;
	u8 bits_per_word;
	int ret;

	reg_base = mpc8xxx_spi->reg_base;
	bits_per_word = spi->bits_per_word;
	if (t->bits_per_word)
		bits_per_word = t->bits_per_word;

<<<<<<< HEAD
	if (bits_per_word > 8) {
		/* invalid length? */
		if (len & 1)
			return -EINVAL;
		len /= 2;
	}
	if (bits_per_word > 16) {
		/* invalid length? */
		if (len & 1)
			return -EINVAL;
		len /= 2;
	}
=======
	if (bits_per_word > 8)
		len /= 2;
	if (bits_per_word > 16)
		len /= 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mpc8xxx_spi->tx = t->tx_buf;
	mpc8xxx_spi->rx = t->rx_buf;

<<<<<<< HEAD
	INIT_COMPLETION(mpc8xxx_spi->done);
=======
	reinit_completion(&mpc8xxx_spi->done);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mpc8xxx_spi->flags & SPI_CPM_MODE)
		ret = fsl_spi_cpm_bufs(mpc8xxx_spi, t, is_dma_mapped);
	else
		ret = fsl_spi_cpu_bufs(mpc8xxx_spi, t, len);
	if (ret)
		return ret;

	wait_for_completion(&mpc8xxx_spi->done);

	/* disable rx ints */
	mpc8xxx_spi_write_reg(&reg_base->mask, 0);

	if (mpc8xxx_spi->flags & SPI_CPM_MODE)
		fsl_spi_cpm_bufs_complete(mpc8xxx_spi);

	return mpc8xxx_spi->count;
}

<<<<<<< HEAD
static void fsl_spi_do_one_msg(struct spi_message *m)
{
	struct spi_device *spi = m->spi;
	struct spi_transfer *t;
	unsigned int cs_change;
	const int nsecs = 50;
	int status;

	cs_change = 1;
	status = 0;
	list_for_each_entry(t, &m->transfers, transfer_list) {
		if (t->bits_per_word || t->speed_hz) {
			/* Don't allow changes if CS is active */
			status = -EINVAL;

			if (cs_change)
				status = fsl_spi_setup_transfer(spi, t);
			if (status < 0)
				break;
		}

		if (cs_change) {
			fsl_spi_chipselect(spi, BITBANG_CS_ACTIVE);
			ndelay(nsecs);
		}
		cs_change = t->cs_change;
		if (t->len)
			status = fsl_spi_bufs(spi, t, m->is_dma_mapped);
		if (status) {
			status = -EMSGSIZE;
			break;
		}
		m->actual_length += t->len;

		if (t->delay_usecs)
			udelay(t->delay_usecs);

		if (cs_change) {
			ndelay(nsecs);
			fsl_spi_chipselect(spi, BITBANG_CS_INACTIVE);
			ndelay(nsecs);
		}
	}

	m->status = status;
	m->complete(m->context);

	if (status || !cs_change) {
		ndelay(nsecs);
		fsl_spi_chipselect(spi, BITBANG_CS_INACTIVE);
	}

	fsl_spi_setup_transfer(spi, NULL);
=======
static int fsl_spi_prepare_message(struct spi_controller *ctlr,
				   struct spi_message *m)
{
	struct mpc8xxx_spi *mpc8xxx_spi = spi_controller_get_devdata(ctlr);
	struct spi_transfer *t;
	struct spi_transfer *first;

	first = list_first_entry(&m->transfers, struct spi_transfer,
				 transfer_list);

	/*
	 * In CPU mode, optimize large byte transfers to use larger
	 * bits_per_word values to reduce number of interrupts taken.
	 *
	 * Some glitches can appear on the SPI clock when the mode changes.
	 * Check that there is no speed change during the transfer and set it up
	 * now to change the mode without having a chip-select asserted.
	 */
	list_for_each_entry(t, &m->transfers, transfer_list) {
		if (t->speed_hz != first->speed_hz) {
			dev_err(&m->spi->dev,
				"speed_hz cannot change during message.\n");
			return -EINVAL;
		}
		if (!(mpc8xxx_spi->flags & SPI_CPM_MODE)) {
			if (t->len < 256 || t->bits_per_word != 8)
				continue;
			if ((t->len & 3) == 0)
				t->bits_per_word = 32;
			else if ((t->len & 1) == 0)
				t->bits_per_word = 16;
		} else {
			/*
			 * CPM/QE uses Little Endian for words > 8
			 * so transform 16 and 32 bits words into 8 bits
			 * Unfortnatly that doesn't work for LSB so
			 * reject these for now
			 * Note: 32 bits word, LSB works iff
			 * tfcr/rfcr is set to CPMFCR_GBL
			 */
			if (m->spi->mode & SPI_LSB_FIRST && t->bits_per_word > 8)
				return -EINVAL;
			if (t->bits_per_word == 16 || t->bits_per_word == 32)
				t->bits_per_word = 8; /* pretend its 8 bits */
			if (t->bits_per_word == 8 && t->len >= 256 &&
			    (mpc8xxx_spi->flags & SPI_CPM1))
				t->bits_per_word = 16;
		}
	}
	return fsl_spi_setup_transfer(m->spi, first);
}

static int fsl_spi_transfer_one(struct spi_controller *controller,
				struct spi_device *spi,
				struct spi_transfer *t)
{
	int status;

	status = fsl_spi_setup_transfer(spi, t);
	if (status < 0)
		return status;
	if (t->len)
		status = fsl_spi_bufs(spi, t, !!t->tx_dma || !!t->rx_dma);
	if (status > 0)
		return -EMSGSIZE;

	return status;
}

static int fsl_spi_unprepare_message(struct spi_controller *controller,
				     struct spi_message *msg)
{
	return fsl_spi_setup_transfer(msg->spi, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fsl_spi_setup(struct spi_device *spi)
{
	struct mpc8xxx_spi *mpc8xxx_spi;
<<<<<<< HEAD
	struct fsl_spi_reg *reg_base;
	int retval;
	u32 hw_mode;
	struct spi_mpc8xxx_cs	*cs = spi->controller_state;
=======
	struct fsl_spi_reg __iomem *reg_base;
	bool initial_setup = false;
	int retval;
	u32 hw_mode;
	struct spi_mpc8xxx_cs *cs = spi_get_ctldata(spi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!spi->max_speed_hz)
		return -EINVAL;

	if (!cs) {
<<<<<<< HEAD
		cs = kzalloc(sizeof *cs, GFP_KERNEL);
		if (!cs)
			return -ENOMEM;
		spi->controller_state = cs;
	}
	mpc8xxx_spi = spi_master_get_devdata(spi->master);
=======
		cs = kzalloc(sizeof(*cs), GFP_KERNEL);
		if (!cs)
			return -ENOMEM;
		spi_set_ctldata(spi, cs);
		initial_setup = true;
	}
	mpc8xxx_spi = spi_controller_get_devdata(spi->controller);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	reg_base = mpc8xxx_spi->reg_base;

	hw_mode = cs->hw_mode; /* Save original settings */
	cs->hw_mode = mpc8xxx_spi_read_reg(&reg_base->mode);
	/* mask out bits we are going to set */
	cs->hw_mode &= ~(SPMODE_CP_BEGIN_EDGECLK | SPMODE_CI_INACTIVEHIGH
			 | SPMODE_REV | SPMODE_LOOP);

	if (spi->mode & SPI_CPHA)
		cs->hw_mode |= SPMODE_CP_BEGIN_EDGECLK;
	if (spi->mode & SPI_CPOL)
		cs->hw_mode |= SPMODE_CI_INACTIVEHIGH;
	if (!(spi->mode & SPI_LSB_FIRST))
		cs->hw_mode |= SPMODE_REV;
	if (spi->mode & SPI_LOOP)
		cs->hw_mode |= SPMODE_LOOP;

	retval = fsl_spi_setup_transfer(spi, NULL);
	if (retval < 0) {
		cs->hw_mode = hw_mode; /* Restore settings */
<<<<<<< HEAD
		return retval;
	}
	return 0;
}

static void fsl_spi_cpm_irq(struct mpc8xxx_spi *mspi, u32 events)
{
	u16 len;
	struct fsl_spi_reg *reg_base = mspi->reg_base;

	dev_dbg(mspi->dev, "%s: bd datlen %d, count %d\n", __func__,
		in_be16(&mspi->rx_bd->cbd_datlen), mspi->count);

	len = in_be16(&mspi->rx_bd->cbd_datlen);
	if (len > mspi->count) {
		WARN_ON(1);
		len = mspi->count;
	}

	/* Clear the events */
	mpc8xxx_spi_write_reg(&reg_base->event, events);

	mspi->count -= len;
	if (mspi->count)
		fsl_spi_cpm_bufs_start(mspi);
	else
		complete(&mspi->done);
=======
		if (initial_setup)
			kfree(cs);
		return retval;
	}

	return 0;
}

static void fsl_spi_cleanup(struct spi_device *spi)
{
	struct spi_mpc8xxx_cs *cs = spi_get_ctldata(spi);

	kfree(cs);
	spi_set_ctldata(spi, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void fsl_spi_cpu_irq(struct mpc8xxx_spi *mspi, u32 events)
{
<<<<<<< HEAD
	struct fsl_spi_reg *reg_base = mspi->reg_base;
=======
	struct fsl_spi_reg __iomem *reg_base = mspi->reg_base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We need handle RX first */
	if (events & SPIE_NE) {
		u32 rx_data = mpc8xxx_spi_read_reg(&reg_base->receive);

		if (mspi->rx)
			mspi->get_rx(rx_data, mspi);
	}

	if ((events & SPIE_NF) == 0)
		/* spin until TX is done */
		while (((events =
			mpc8xxx_spi_read_reg(&reg_base->event)) &
						SPIE_NF) == 0)
			cpu_relax();

	/* Clear the events */
	mpc8xxx_spi_write_reg(&reg_base->event, events);

	mspi->count -= 1;
	if (mspi->count) {
		u32 word = mspi->get_tx(mspi);

		mpc8xxx_spi_write_reg(&reg_base->transmit, word);
	} else {
		complete(&mspi->done);
	}
}

static irqreturn_t fsl_spi_irq(s32 irq, void *context_data)
{
	struct mpc8xxx_spi *mspi = context_data;
	irqreturn_t ret = IRQ_NONE;
	u32 events;
<<<<<<< HEAD
	struct fsl_spi_reg *reg_base = mspi->reg_base;
=======
	struct fsl_spi_reg __iomem *reg_base = mspi->reg_base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Get interrupt events(tx/rx) */
	events = mpc8xxx_spi_read_reg(&reg_base->event);
	if (events)
		ret = IRQ_HANDLED;

	dev_dbg(mspi->dev, "%s: events %x\n", __func__, events);

	if (mspi->flags & SPI_CPM_MODE)
		fsl_spi_cpm_irq(mspi, events);
	else
		fsl_spi_cpu_irq(mspi, events);

	return ret;
}

<<<<<<< HEAD
static void *fsl_spi_alloc_dummy_rx(void)
{
	mutex_lock(&fsl_dummy_rx_lock);

	if (!fsl_dummy_rx)
		fsl_dummy_rx = kmalloc(SPI_MRBLR, GFP_KERNEL);
	if (fsl_dummy_rx)
		fsl_dummy_rx_refcnt++;

	mutex_unlock(&fsl_dummy_rx_lock);

	return fsl_dummy_rx;
}

static void fsl_spi_free_dummy_rx(void)
{
	mutex_lock(&fsl_dummy_rx_lock);

	switch (fsl_dummy_rx_refcnt) {
	case 0:
		WARN_ON(1);
		break;
	case 1:
		kfree(fsl_dummy_rx);
		fsl_dummy_rx = NULL;
		/* fall through */
	default:
		fsl_dummy_rx_refcnt--;
		break;
	}

	mutex_unlock(&fsl_dummy_rx_lock);
}

static unsigned long fsl_spi_cpm_get_pram(struct mpc8xxx_spi *mspi)
{
	struct device *dev = mspi->dev;
	struct device_node *np = dev->of_node;
	const u32 *iprop;
	int size;
	void __iomem *spi_base;
	unsigned long pram_ofs = -ENOMEM;

	/* Can't use of_address_to_resource(), QE muram isn't at 0. */
	iprop = of_get_property(np, "reg", &size);

	/* QE with a fixed pram location? */
	if (mspi->flags & SPI_QE && iprop && size == sizeof(*iprop) * 4)
		return cpm_muram_alloc_fixed(iprop[2], SPI_PRAM_SIZE);

	/* QE but with a dynamic pram location? */
	if (mspi->flags & SPI_QE) {
		pram_ofs = cpm_muram_alloc(SPI_PRAM_SIZE, 64);
		qe_issue_cmd(QE_ASSIGN_PAGE_TO_DEVICE, mspi->subblock,
				QE_CR_PROTOCOL_UNSPECIFIED, pram_ofs);
		return pram_ofs;
	}

	spi_base = of_iomap(np, 1);
	if (spi_base == NULL)
		return -EINVAL;

	if (mspi->flags & SPI_CPM2) {
		pram_ofs = cpm_muram_alloc(SPI_PRAM_SIZE, 64);
		out_be16(spi_base, pram_ofs);
	} else {
		struct spi_pram __iomem *pram = spi_base;
		u16 rpbase = in_be16(&pram->rpbase);

		/* Microcode relocation patch applied? */
		if (rpbase)
			pram_ofs = rpbase;
		else {
			pram_ofs = cpm_muram_alloc(SPI_PRAM_SIZE, 64);
			out_be16(spi_base, pram_ofs);
		}
	}

	iounmap(spi_base);
	return pram_ofs;
}

static int fsl_spi_cpm_init(struct mpc8xxx_spi *mspi)
{
	struct device *dev = mspi->dev;
	struct device_node *np = dev->of_node;
	const u32 *iprop;
	int size;
	unsigned long pram_ofs;
	unsigned long bds_ofs;

	if (!(mspi->flags & SPI_CPM_MODE))
		return 0;

	if (!fsl_spi_alloc_dummy_rx())
		return -ENOMEM;

	if (mspi->flags & SPI_QE) {
		iprop = of_get_property(np, "cell-index", &size);
		if (iprop && size == sizeof(*iprop))
			mspi->subblock = *iprop;

		switch (mspi->subblock) {
		default:
			dev_warn(dev, "cell-index unspecified, assuming SPI1");
			/* fall through */
		case 0:
			mspi->subblock = QE_CR_SUBBLOCK_SPI1;
			break;
		case 1:
			mspi->subblock = QE_CR_SUBBLOCK_SPI2;
			break;
		}
	}

	pram_ofs = fsl_spi_cpm_get_pram(mspi);
	if (IS_ERR_VALUE(pram_ofs)) {
		dev_err(dev, "can't allocate spi parameter ram\n");
		goto err_pram;
	}

	bds_ofs = cpm_muram_alloc(sizeof(*mspi->tx_bd) +
				  sizeof(*mspi->rx_bd), 8);
	if (IS_ERR_VALUE(bds_ofs)) {
		dev_err(dev, "can't allocate bds\n");
		goto err_bds;
	}

	mspi->dma_dummy_tx = dma_map_single(dev, empty_zero_page, PAGE_SIZE,
					    DMA_TO_DEVICE);
	if (dma_mapping_error(dev, mspi->dma_dummy_tx)) {
		dev_err(dev, "unable to map dummy tx buffer\n");
		goto err_dummy_tx;
	}

	mspi->dma_dummy_rx = dma_map_single(dev, fsl_dummy_rx, SPI_MRBLR,
					    DMA_FROM_DEVICE);
	if (dma_mapping_error(dev, mspi->dma_dummy_rx)) {
		dev_err(dev, "unable to map dummy rx buffer\n");
		goto err_dummy_rx;
	}

	mspi->pram = cpm_muram_addr(pram_ofs);

	mspi->tx_bd = cpm_muram_addr(bds_ofs);
	mspi->rx_bd = cpm_muram_addr(bds_ofs + sizeof(*mspi->tx_bd));

	/* Initialize parameter ram. */
	out_be16(&mspi->pram->tbase, cpm_muram_offset(mspi->tx_bd));
	out_be16(&mspi->pram->rbase, cpm_muram_offset(mspi->rx_bd));
	out_8(&mspi->pram->tfcr, CPMFCR_EB | CPMFCR_GBL);
	out_8(&mspi->pram->rfcr, CPMFCR_EB | CPMFCR_GBL);
	out_be16(&mspi->pram->mrblr, SPI_MRBLR);
	out_be32(&mspi->pram->rstate, 0);
	out_be32(&mspi->pram->rdp, 0);
	out_be16(&mspi->pram->rbptr, 0);
	out_be16(&mspi->pram->rbc, 0);
	out_be32(&mspi->pram->rxtmp, 0);
	out_be32(&mspi->pram->tstate, 0);
	out_be32(&mspi->pram->tdp, 0);
	out_be16(&mspi->pram->tbptr, 0);
	out_be16(&mspi->pram->tbc, 0);
	out_be32(&mspi->pram->txtmp, 0);

	return 0;

err_dummy_rx:
	dma_unmap_single(dev, mspi->dma_dummy_tx, PAGE_SIZE, DMA_TO_DEVICE);
err_dummy_tx:
	cpm_muram_free(bds_ofs);
err_bds:
	cpm_muram_free(pram_ofs);
err_pram:
	fsl_spi_free_dummy_rx();
	return -ENOMEM;
}

static void fsl_spi_cpm_free(struct mpc8xxx_spi *mspi)
{
	struct device *dev = mspi->dev;

	if (!(mspi->flags & SPI_CPM_MODE))
		return;

	dma_unmap_single(dev, mspi->dma_dummy_rx, SPI_MRBLR, DMA_FROM_DEVICE);
	dma_unmap_single(dev, mspi->dma_dummy_tx, PAGE_SIZE, DMA_TO_DEVICE);
	cpm_muram_free(cpm_muram_offset(mspi->tx_bd));
	cpm_muram_free(cpm_muram_offset(mspi->pram));
	fsl_spi_free_dummy_rx();
}

static void fsl_spi_remove(struct mpc8xxx_spi *mspi)
{
	iounmap(mspi->reg_base);
	fsl_spi_cpm_free(mspi);
}

static struct spi_master * __devinit fsl_spi_probe(struct device *dev,
		struct resource *mem, unsigned int irq)
{
	struct fsl_spi_platform_data *pdata = dev->platform_data;
	struct spi_master *master;
	struct mpc8xxx_spi *mpc8xxx_spi;
	struct fsl_spi_reg *reg_base;
	u32 regval;
	int ret = 0;

	master = spi_alloc_master(dev, sizeof(struct mpc8xxx_spi));
	if (master == NULL) {
=======
static void fsl_spi_grlib_cs_control(struct spi_device *spi, bool on)
{
	struct mpc8xxx_spi *mpc8xxx_spi = spi_controller_get_devdata(spi->controller);
	struct fsl_spi_reg __iomem *reg_base = mpc8xxx_spi->reg_base;
	u32 slvsel;
	u16 cs = spi_get_chipselect(spi, 0);

	if (cs < mpc8xxx_spi->native_chipselects) {
		slvsel = mpc8xxx_spi_read_reg(&reg_base->slvsel);
		slvsel = on ? (slvsel | (1 << cs)) : (slvsel & ~(1 << cs));
		mpc8xxx_spi_write_reg(&reg_base->slvsel, slvsel);
	}
}

static void fsl_spi_grlib_probe(struct device *dev)
{
	struct spi_controller *host = dev_get_drvdata(dev);
	struct mpc8xxx_spi *mpc8xxx_spi = spi_controller_get_devdata(host);
	struct fsl_spi_reg __iomem *reg_base = mpc8xxx_spi->reg_base;
	int mbits;
	u32 capabilities;

	capabilities = mpc8xxx_spi_read_reg(&reg_base->cap);

	mpc8xxx_spi->set_shifts = fsl_spi_grlib_set_shifts;
	mbits = SPCAP_MAXWLEN(capabilities);
	if (mbits)
		mpc8xxx_spi->max_bits_per_word = mbits + 1;

	mpc8xxx_spi->native_chipselects = 0;
	if (SPCAP_SSEN(capabilities)) {
		mpc8xxx_spi->native_chipselects = SPCAP_SSSZ(capabilities);
		mpc8xxx_spi_write_reg(&reg_base->slvsel, 0xffffffff);
	}
	host->num_chipselect = mpc8xxx_spi->native_chipselects;
	host->set_cs = fsl_spi_grlib_cs_control;
}

static void fsl_spi_cs_control(struct spi_device *spi, bool on)
{
	struct device *dev = spi->dev.parent->parent;
	struct fsl_spi_platform_data *pdata = dev_get_platdata(dev);
	struct mpc8xxx_spi_probe_info *pinfo = to_of_pinfo(pdata);

	if (WARN_ON_ONCE(!pinfo->immr_spi_cs))
		return;
	iowrite32be(on ? 0 : SPI_BOOT_SEL_BIT, pinfo->immr_spi_cs);
}

static struct spi_controller *fsl_spi_probe(struct device *dev,
		struct resource *mem, unsigned int irq)
{
	struct fsl_spi_platform_data *pdata = dev_get_platdata(dev);
	struct spi_controller *host;
	struct mpc8xxx_spi *mpc8xxx_spi;
	struct fsl_spi_reg __iomem *reg_base;
	u32 regval;
	int ret = 0;

	host = spi_alloc_host(dev, sizeof(struct mpc8xxx_spi));
	if (host == NULL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ENOMEM;
		goto err;
	}

<<<<<<< HEAD
	dev_set_drvdata(dev, master);

	ret = mpc8xxx_spi_probe(dev, mem, irq);
	if (ret)
		goto err_probe;

	master->setup = fsl_spi_setup;

	mpc8xxx_spi = spi_master_get_devdata(master);
	mpc8xxx_spi->spi_do_one_msg = fsl_spi_do_one_msg;
	mpc8xxx_spi->spi_remove = fsl_spi_remove;

=======
	dev_set_drvdata(dev, host);

	mpc8xxx_spi_probe(dev, mem, irq);

	host->setup = fsl_spi_setup;
	host->cleanup = fsl_spi_cleanup;
	host->prepare_message = fsl_spi_prepare_message;
	host->transfer_one = fsl_spi_transfer_one;
	host->unprepare_message = fsl_spi_unprepare_message;
	host->use_gpio_descriptors = true;
	host->set_cs = fsl_spi_cs_control;

	mpc8xxx_spi = spi_controller_get_devdata(host);
	mpc8xxx_spi->max_bits_per_word = 32;
	mpc8xxx_spi->type = fsl_spi_get_type(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = fsl_spi_cpm_init(mpc8xxx_spi);
	if (ret)
		goto err_cpm_init;

<<<<<<< HEAD
	if (mpc8xxx_spi->flags & SPI_QE_CPU_MODE) {
		mpc8xxx_spi->rx_shift = 16;
		mpc8xxx_spi->tx_shift = 24;
	}

	mpc8xxx_spi->reg_base = ioremap(mem->start, resource_size(mem));
	if (mpc8xxx_spi->reg_base == NULL) {
		ret = -ENOMEM;
		goto err_ioremap;
	}

	/* Register for SPI Interrupt */
	ret = request_irq(mpc8xxx_spi->irq, fsl_spi_irq,
			  0, "fsl_spi", mpc8xxx_spi);

	if (ret != 0)
		goto free_irq;
=======
	mpc8xxx_spi->reg_base = devm_ioremap_resource(dev, mem);
	if (IS_ERR(mpc8xxx_spi->reg_base)) {
		ret = PTR_ERR(mpc8xxx_spi->reg_base);
		goto err_probe;
	}

	if (mpc8xxx_spi->type == TYPE_GRLIB)
		fsl_spi_grlib_probe(dev);

	if (mpc8xxx_spi->flags & SPI_CPM_MODE)
		host->bits_per_word_mask =
			(SPI_BPW_RANGE_MASK(4, 8) | SPI_BPW_MASK(16) | SPI_BPW_MASK(32));
	else
		host->bits_per_word_mask =
			(SPI_BPW_RANGE_MASK(4, 16) | SPI_BPW_MASK(32));

	host->bits_per_word_mask &=
		SPI_BPW_RANGE_MASK(1, mpc8xxx_spi->max_bits_per_word);

	if (mpc8xxx_spi->flags & SPI_QE_CPU_MODE)
		mpc8xxx_spi->set_shifts = fsl_spi_qe_cpu_set_shifts;

	if (mpc8xxx_spi->set_shifts)
		/* 8 bits per word and MSB first */
		mpc8xxx_spi->set_shifts(&mpc8xxx_spi->rx_shift,
					&mpc8xxx_spi->tx_shift, 8, 1);

	/* Register for SPI Interrupt */
	ret = devm_request_irq(dev, mpc8xxx_spi->irq, fsl_spi_irq,
			       0, "fsl_spi", mpc8xxx_spi);

	if (ret != 0)
		goto err_probe;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	reg_base = mpc8xxx_spi->reg_base;

	/* SPI controller initializations */
	mpc8xxx_spi_write_reg(&reg_base->mode, 0);
	mpc8xxx_spi_write_reg(&reg_base->mask, 0);
	mpc8xxx_spi_write_reg(&reg_base->command, 0);
	mpc8xxx_spi_write_reg(&reg_base->event, 0xffffffff);

	/* Enable SPI interface */
	regval = pdata->initial_spmode | SPMODE_INIT_VAL | SPMODE_ENABLE;
<<<<<<< HEAD
=======
	if (mpc8xxx_spi->max_bits_per_word < 8) {
		regval &= ~SPMODE_LEN(0xF);
		regval |= SPMODE_LEN(mpc8xxx_spi->max_bits_per_word - 1);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mpc8xxx_spi->flags & SPI_QE_CPU_MODE)
		regval |= SPMODE_OP;

	mpc8xxx_spi_write_reg(&reg_base->mode, regval);

<<<<<<< HEAD
	ret = spi_register_master(master);
	if (ret < 0)
		goto unreg_master;
=======
	ret = devm_spi_register_controller(dev, host);
	if (ret < 0)
		goto err_probe;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_info(dev, "at 0x%p (irq = %d), %s mode\n", reg_base,
		 mpc8xxx_spi->irq, mpc8xxx_spi_strmode(mpc8xxx_spi->flags));

<<<<<<< HEAD
	return master;

unreg_master:
	free_irq(mpc8xxx_spi->irq, mpc8xxx_spi);
free_irq:
	iounmap(mpc8xxx_spi->reg_base);
err_ioremap:
	fsl_spi_cpm_free(mpc8xxx_spi);
err_cpm_init:
err_probe:
	spi_master_put(master);
=======
	return host;

err_probe:
	fsl_spi_cpm_free(mpc8xxx_spi);
err_cpm_init:
	spi_controller_put(host);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err:
	return ERR_PTR(ret);
}

<<<<<<< HEAD
static void fsl_spi_cs_control(struct spi_device *spi, bool on)
{
	struct device *dev = spi->dev.parent->parent;
	struct mpc8xxx_spi_probe_info *pinfo = to_of_pinfo(dev->platform_data);
	u16 cs = spi->chip_select;
	int gpio = pinfo->gpios[cs];
	bool alow = pinfo->alow_flags[cs];

	gpio_set_value(gpio, on ^ alow);
}

static int of_fsl_spi_get_chipselects(struct device *dev)
{
	struct device_node *np = dev->of_node;
	struct fsl_spi_platform_data *pdata = dev->platform_data;
	struct mpc8xxx_spi_probe_info *pinfo = to_of_pinfo(pdata);
	unsigned int ngpios;
	int i = 0;
	int ret;

	ngpios = of_gpio_count(np);
	if (!ngpios) {
		/*
		 * SPI w/o chip-select line. One SPI device is still permitted
		 * though.
		 */
		pdata->max_chipselect = 1;
		return 0;
	}

	pinfo->gpios = kmalloc(ngpios * sizeof(*pinfo->gpios), GFP_KERNEL);
	if (!pinfo->gpios)
		return -ENOMEM;
	memset(pinfo->gpios, -1, ngpios * sizeof(*pinfo->gpios));

	pinfo->alow_flags = kzalloc(ngpios * sizeof(*pinfo->alow_flags),
				    GFP_KERNEL);
	if (!pinfo->alow_flags) {
		ret = -ENOMEM;
		goto err_alloc_flags;
	}

	for (; i < ngpios; i++) {
		int gpio;
		enum of_gpio_flags flags;

		gpio = of_get_gpio_flags(np, i, &flags);
		if (!gpio_is_valid(gpio)) {
			dev_err(dev, "invalid gpio #%d: %d\n", i, gpio);
			ret = gpio;
			goto err_loop;
		}

		ret = gpio_request(gpio, dev_name(dev));
		if (ret) {
			dev_err(dev, "can't request gpio #%d: %d\n", i, ret);
			goto err_loop;
		}

		pinfo->gpios[i] = gpio;
		pinfo->alow_flags[i] = flags & OF_GPIO_ACTIVE_LOW;

		ret = gpio_direction_output(pinfo->gpios[i],
					    pinfo->alow_flags[i]);
		if (ret) {
			dev_err(dev, "can't set output direction for gpio "
				"#%d: %d\n", i, ret);
			goto err_loop;
		}
	}

	pdata->max_chipselect = ngpios;
	pdata->cs_control = fsl_spi_cs_control;

	return 0;

err_loop:
	while (i >= 0) {
		if (gpio_is_valid(pinfo->gpios[i]))
			gpio_free(pinfo->gpios[i]);
		i--;
	}

	kfree(pinfo->alow_flags);
	pinfo->alow_flags = NULL;
err_alloc_flags:
	kfree(pinfo->gpios);
	pinfo->gpios = NULL;
	return ret;
}

static int of_fsl_spi_free_chipselects(struct device *dev)
{
	struct fsl_spi_platform_data *pdata = dev->platform_data;
	struct mpc8xxx_spi_probe_info *pinfo = to_of_pinfo(pdata);
	int i;

	if (!pinfo->gpios)
		return 0;

	for (i = 0; i < pdata->max_chipselect; i++) {
		if (gpio_is_valid(pinfo->gpios[i]))
			gpio_free(pinfo->gpios[i]);
	}

	kfree(pinfo->gpios);
	kfree(pinfo->alow_flags);
	return 0;
}

static int __devinit of_fsl_spi_probe(struct platform_device *ofdev)
{
	struct device *dev = &ofdev->dev;
	struct device_node *np = ofdev->dev.of_node;
	struct spi_master *master;
	struct resource mem;
	struct resource irq;
	int ret = -ENOMEM;
=======
static int of_fsl_spi_probe(struct platform_device *ofdev)
{
	struct device *dev = &ofdev->dev;
	struct device_node *np = ofdev->dev.of_node;
	struct spi_controller *host;
	struct resource mem;
	int irq, type;
	int ret;
	bool spisel_boot = false;
#if IS_ENABLED(CONFIG_FSL_SOC)
	struct mpc8xxx_spi_probe_info *pinfo = NULL;
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = of_mpc8xxx_spi_probe(ofdev);
	if (ret)
		return ret;

<<<<<<< HEAD
	ret = of_fsl_spi_get_chipselects(dev);
	if (ret)
		goto err;

	ret = of_address_to_resource(np, 0, &mem);
	if (ret)
		goto err;

	ret = of_irq_to_resource(np, 0, &irq);
	if (!ret) {
		ret = -EINVAL;
		goto err;
	}

	master = fsl_spi_probe(dev, &mem, irq.start);
	if (IS_ERR(master)) {
		ret = PTR_ERR(master);
		goto err;
	}

	return 0;

err:
	of_fsl_spi_free_chipselects(dev);
	return ret;
}

static int __devexit of_fsl_spi_remove(struct platform_device *ofdev)
{
	int ret;

	ret = mpc8xxx_spi_remove(&ofdev->dev);
	if (ret)
		return ret;
	of_fsl_spi_free_chipselects(&ofdev->dev);
	return 0;
}

static const struct of_device_id of_fsl_spi_match[] = {
	{ .compatible = "fsl,spi" },
	{}
};
MODULE_DEVICE_TABLE(of, of_fsl_spi_match);

static struct platform_driver of_fsl_spi_driver = {
	.driver = {
		.name = "fsl_spi",
		.owner = THIS_MODULE,
		.of_match_table = of_fsl_spi_match,
	},
	.probe		= of_fsl_spi_probe,
	.remove		= __devexit_p(of_fsl_spi_remove),
=======
	type = fsl_spi_get_type(&ofdev->dev);
	if (type == TYPE_FSL) {
		struct fsl_spi_platform_data *pdata = dev_get_platdata(dev);
#if IS_ENABLED(CONFIG_FSL_SOC)
		pinfo = to_of_pinfo(pdata);

		spisel_boot = of_property_read_bool(np, "fsl,spisel_boot");
		if (spisel_boot) {
			pinfo->immr_spi_cs = ioremap(get_immrbase() + IMMR_SPI_CS_OFFSET, 4);
			if (!pinfo->immr_spi_cs)
				return -ENOMEM;
		}
#endif
		/*
		 * Handle the case where we have one hardwired (always selected)
		 * device on the first "chipselect". Else we let the core code
		 * handle any GPIOs or native chip selects and assign the
		 * appropriate callback for dealing with the CS lines. This isn't
		 * supported on the GRLIB variant.
		 */
		ret = gpiod_count(dev, "cs");
		if (ret < 0)
			ret = 0;
		if (ret == 0 && !spisel_boot)
			pdata->max_chipselect = 1;
		else
			pdata->max_chipselect = ret + spisel_boot;
	}

	ret = of_address_to_resource(np, 0, &mem);
	if (ret)
		goto unmap_out;

	irq = platform_get_irq(ofdev, 0);
	if (irq < 0) {
		ret = irq;
		goto unmap_out;
	}

	host = fsl_spi_probe(dev, &mem, irq);

	return PTR_ERR_OR_ZERO(host);

unmap_out:
#if IS_ENABLED(CONFIG_FSL_SOC)
	if (spisel_boot)
		iounmap(pinfo->immr_spi_cs);
#endif
	return ret;
}

static void of_fsl_spi_remove(struct platform_device *ofdev)
{
	struct spi_controller *host = platform_get_drvdata(ofdev);
	struct mpc8xxx_spi *mpc8xxx_spi = spi_controller_get_devdata(host);

	fsl_spi_cpm_free(mpc8xxx_spi);
}

static struct platform_driver of_fsl_spi_driver = {
	.driver = {
		.name = "fsl_spi",
		.of_match_table = of_fsl_spi_match,
	},
	.probe		= of_fsl_spi_probe,
	.remove_new	= of_fsl_spi_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#ifdef CONFIG_MPC832x_RDB
/*
 * XXX XXX XXX
 * This is "legacy" platform driver, was used by the MPC8323E-RDB boards
 * only. The driver should go away soon, since newer MPC8323E-RDB's device
 * tree can work with OpenFirmware driver. But for now we support old trees
 * as well.
 */
<<<<<<< HEAD
static int __devinit plat_mpc8xxx_spi_probe(struct platform_device *pdev)
{
	struct resource *mem;
	int irq;
	struct spi_master *master;

	if (!pdev->dev.platform_data)
=======
static int plat_mpc8xxx_spi_probe(struct platform_device *pdev)
{
	struct resource *mem;
	int irq;
	struct spi_controller *host;

	if (!dev_get_platdata(&pdev->dev))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem)
		return -EINVAL;

	irq = platform_get_irq(pdev, 0);
<<<<<<< HEAD
	if (irq <= 0)
		return -EINVAL;

	master = fsl_spi_probe(&pdev->dev, mem, irq);
	if (IS_ERR(master))
		return PTR_ERR(master);
	return 0;
}

static int __devexit plat_mpc8xxx_spi_remove(struct platform_device *pdev)
{
	return mpc8xxx_spi_remove(&pdev->dev);
=======
	if (irq < 0)
		return irq;

	host = fsl_spi_probe(&pdev->dev, mem, irq);
	return PTR_ERR_OR_ZERO(host);
}

static void plat_mpc8xxx_spi_remove(struct platform_device *pdev)
{
	struct spi_controller *host = platform_get_drvdata(pdev);
	struct mpc8xxx_spi *mpc8xxx_spi = spi_controller_get_devdata(host);

	fsl_spi_cpm_free(mpc8xxx_spi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

MODULE_ALIAS("platform:mpc8xxx_spi");
static struct platform_driver mpc8xxx_spi_driver = {
	.probe = plat_mpc8xxx_spi_probe,
<<<<<<< HEAD
	.remove = __devexit_p(plat_mpc8xxx_spi_remove),
	.driver = {
		.name = "mpc8xxx_spi",
		.owner = THIS_MODULE,
=======
	.remove_new = plat_mpc8xxx_spi_remove,
	.driver = {
		.name = "mpc8xxx_spi",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static bool legacy_driver_failed;

static void __init legacy_driver_register(void)
{
	legacy_driver_failed = platform_driver_register(&mpc8xxx_spi_driver);
}

static void __exit legacy_driver_unregister(void)
{
	if (legacy_driver_failed)
		return;
	platform_driver_unregister(&mpc8xxx_spi_driver);
}
#else
static void __init legacy_driver_register(void) {}
static void __exit legacy_driver_unregister(void) {}
#endif /* CONFIG_MPC832x_RDB */

static int __init fsl_spi_init(void)
{
	legacy_driver_register();
	return platform_driver_register(&of_fsl_spi_driver);
}
module_init(fsl_spi_init);

static void __exit fsl_spi_exit(void)
{
	platform_driver_unregister(&of_fsl_spi_driver);
	legacy_driver_unregister();
}
module_exit(fsl_spi_exit);

MODULE_AUTHOR("Kumar Gala");
MODULE_DESCRIPTION("Simple Freescale SPI Driver");
MODULE_LICENSE("GPL");
