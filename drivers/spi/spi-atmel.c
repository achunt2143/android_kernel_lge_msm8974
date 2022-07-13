<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Driver for Atmel AT32 and AT91 SPI Controllers
 *
 * Copyright (C) 2006 Atmel Corporation
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
=======
 */

#include <linux/kernel.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/clk.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
<<<<<<< HEAD
=======
#include <linux/dmaengine.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
<<<<<<< HEAD

#include <asm/io.h>
#include <mach/board.h>
#include <asm/gpio.h>
#include <mach/cpu.h>
=======
#include <linux/of.h>

#include <linux/io.h>
#include <linux/gpio/consumer.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pm_runtime.h>
#include <linux/iopoll.h>
#include <trace/events/spi.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* SPI register offsets */
#define SPI_CR					0x0000
#define SPI_MR					0x0004
#define SPI_RDR					0x0008
#define SPI_TDR					0x000c
#define SPI_SR					0x0010
#define SPI_IER					0x0014
#define SPI_IDR					0x0018
#define SPI_IMR					0x001c
#define SPI_CSR0				0x0030
#define SPI_CSR1				0x0034
#define SPI_CSR2				0x0038
#define SPI_CSR3				0x003c
<<<<<<< HEAD
=======
#define SPI_FMR					0x0040
#define SPI_FLR					0x0044
#define SPI_VERSION				0x00fc
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SPI_RPR					0x0100
#define SPI_RCR					0x0104
#define SPI_TPR					0x0108
#define SPI_TCR					0x010c
#define SPI_RNPR				0x0110
#define SPI_RNCR				0x0114
#define SPI_TNPR				0x0118
#define SPI_TNCR				0x011c
#define SPI_PTCR				0x0120
#define SPI_PTSR				0x0124

/* Bitfields in CR */
#define SPI_SPIEN_OFFSET			0
#define SPI_SPIEN_SIZE				1
#define SPI_SPIDIS_OFFSET			1
#define SPI_SPIDIS_SIZE				1
#define SPI_SWRST_OFFSET			7
#define SPI_SWRST_SIZE				1
#define SPI_LASTXFER_OFFSET			24
#define SPI_LASTXFER_SIZE			1
<<<<<<< HEAD
=======
#define SPI_TXFCLR_OFFSET			16
#define SPI_TXFCLR_SIZE				1
#define SPI_RXFCLR_OFFSET			17
#define SPI_RXFCLR_SIZE				1
#define SPI_FIFOEN_OFFSET			30
#define SPI_FIFOEN_SIZE				1
#define SPI_FIFODIS_OFFSET			31
#define SPI_FIFODIS_SIZE			1
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Bitfields in MR */
#define SPI_MSTR_OFFSET				0
#define SPI_MSTR_SIZE				1
#define SPI_PS_OFFSET				1
#define SPI_PS_SIZE				1
#define SPI_PCSDEC_OFFSET			2
#define SPI_PCSDEC_SIZE				1
#define SPI_FDIV_OFFSET				3
#define SPI_FDIV_SIZE				1
#define SPI_MODFDIS_OFFSET			4
#define SPI_MODFDIS_SIZE			1
<<<<<<< HEAD
=======
#define SPI_WDRBT_OFFSET			5
#define SPI_WDRBT_SIZE				1
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SPI_LLB_OFFSET				7
#define SPI_LLB_SIZE				1
#define SPI_PCS_OFFSET				16
#define SPI_PCS_SIZE				4
#define SPI_DLYBCS_OFFSET			24
#define SPI_DLYBCS_SIZE				8

/* Bitfields in RDR */
#define SPI_RD_OFFSET				0
#define SPI_RD_SIZE				16

/* Bitfields in TDR */
#define SPI_TD_OFFSET				0
#define SPI_TD_SIZE				16

/* Bitfields in SR */
#define SPI_RDRF_OFFSET				0
#define SPI_RDRF_SIZE				1
#define SPI_TDRE_OFFSET				1
#define SPI_TDRE_SIZE				1
#define SPI_MODF_OFFSET				2
#define SPI_MODF_SIZE				1
#define SPI_OVRES_OFFSET			3
#define SPI_OVRES_SIZE				1
#define SPI_ENDRX_OFFSET			4
#define SPI_ENDRX_SIZE				1
#define SPI_ENDTX_OFFSET			5
#define SPI_ENDTX_SIZE				1
#define SPI_RXBUFF_OFFSET			6
#define SPI_RXBUFF_SIZE				1
#define SPI_TXBUFE_OFFSET			7
#define SPI_TXBUFE_SIZE				1
#define SPI_NSSR_OFFSET				8
#define SPI_NSSR_SIZE				1
#define SPI_TXEMPTY_OFFSET			9
#define SPI_TXEMPTY_SIZE			1
#define SPI_SPIENS_OFFSET			16
#define SPI_SPIENS_SIZE				1
<<<<<<< HEAD
=======
#define SPI_TXFEF_OFFSET			24
#define SPI_TXFEF_SIZE				1
#define SPI_TXFFF_OFFSET			25
#define SPI_TXFFF_SIZE				1
#define SPI_TXFTHF_OFFSET			26
#define SPI_TXFTHF_SIZE				1
#define SPI_RXFEF_OFFSET			27
#define SPI_RXFEF_SIZE				1
#define SPI_RXFFF_OFFSET			28
#define SPI_RXFFF_SIZE				1
#define SPI_RXFTHF_OFFSET			29
#define SPI_RXFTHF_SIZE				1
#define SPI_TXFPTEF_OFFSET			30
#define SPI_TXFPTEF_SIZE			1
#define SPI_RXFPTEF_OFFSET			31
#define SPI_RXFPTEF_SIZE			1
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Bitfields in CSR0 */
#define SPI_CPOL_OFFSET				0
#define SPI_CPOL_SIZE				1
#define SPI_NCPHA_OFFSET			1
#define SPI_NCPHA_SIZE				1
#define SPI_CSAAT_OFFSET			3
#define SPI_CSAAT_SIZE				1
#define SPI_BITS_OFFSET				4
#define SPI_BITS_SIZE				4
#define SPI_SCBR_OFFSET				8
#define SPI_SCBR_SIZE				8
#define SPI_DLYBS_OFFSET			16
#define SPI_DLYBS_SIZE				8
#define SPI_DLYBCT_OFFSET			24
#define SPI_DLYBCT_SIZE				8

/* Bitfields in RCR */
#define SPI_RXCTR_OFFSET			0
#define SPI_RXCTR_SIZE				16

/* Bitfields in TCR */
#define SPI_TXCTR_OFFSET			0
#define SPI_TXCTR_SIZE				16

/* Bitfields in RNCR */
#define SPI_RXNCR_OFFSET			0
#define SPI_RXNCR_SIZE				16

/* Bitfields in TNCR */
#define SPI_TXNCR_OFFSET			0
#define SPI_TXNCR_SIZE				16

/* Bitfields in PTCR */
#define SPI_RXTEN_OFFSET			0
#define SPI_RXTEN_SIZE				1
#define SPI_RXTDIS_OFFSET			1
#define SPI_RXTDIS_SIZE				1
#define SPI_TXTEN_OFFSET			8
#define SPI_TXTEN_SIZE				1
#define SPI_TXTDIS_OFFSET			9
#define SPI_TXTDIS_SIZE				1

<<<<<<< HEAD
=======
/* Bitfields in FMR */
#define SPI_TXRDYM_OFFSET			0
#define SPI_TXRDYM_SIZE				2
#define SPI_RXRDYM_OFFSET			4
#define SPI_RXRDYM_SIZE				2
#define SPI_TXFTHRES_OFFSET			16
#define SPI_TXFTHRES_SIZE			6
#define SPI_RXFTHRES_OFFSET			24
#define SPI_RXFTHRES_SIZE			6

/* Bitfields in FLR */
#define SPI_TXFL_OFFSET				0
#define SPI_TXFL_SIZE				6
#define SPI_RXFL_OFFSET				16
#define SPI_RXFL_SIZE				6

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Constants for BITS */
#define SPI_BITS_8_BPT				0
#define SPI_BITS_9_BPT				1
#define SPI_BITS_10_BPT				2
#define SPI_BITS_11_BPT				3
#define SPI_BITS_12_BPT				4
#define SPI_BITS_13_BPT				5
#define SPI_BITS_14_BPT				6
#define SPI_BITS_15_BPT				7
#define SPI_BITS_16_BPT				8
<<<<<<< HEAD
=======
#define SPI_ONE_DATA				0
#define SPI_TWO_DATA				1
#define SPI_FOUR_DATA				2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Bit manipulation macros */
#define SPI_BIT(name) \
	(1 << SPI_##name##_OFFSET)
<<<<<<< HEAD
#define SPI_BF(name,value) \
	(((value) & ((1 << SPI_##name##_SIZE) - 1)) << SPI_##name##_OFFSET)
#define SPI_BFEXT(name,value) \
	(((value) >> SPI_##name##_OFFSET) & ((1 << SPI_##name##_SIZE) - 1))
#define SPI_BFINS(name,value,old) \
	( ((old) & ~(((1 << SPI_##name##_SIZE) - 1) << SPI_##name##_OFFSET)) \
	  | SPI_BF(name,value))

/* Register access macros */
#define spi_readl(port,reg) \
	__raw_readl((port)->regs + SPI_##reg)
#define spi_writel(port,reg,value) \
	__raw_writel((value), (port)->regs + SPI_##reg)

=======
#define SPI_BF(name, value) \
	(((value) & ((1 << SPI_##name##_SIZE) - 1)) << SPI_##name##_OFFSET)
#define SPI_BFEXT(name, value) \
	(((value) >> SPI_##name##_OFFSET) & ((1 << SPI_##name##_SIZE) - 1))
#define SPI_BFINS(name, value, old) \
	(((old) & ~(((1 << SPI_##name##_SIZE) - 1) << SPI_##name##_OFFSET)) \
	  | SPI_BF(name, value))

/* Register access macros */
#define spi_readl(port, reg) \
	readl_relaxed((port)->regs + SPI_##reg)
#define spi_writel(port, reg, value) \
	writel_relaxed((value), (port)->regs + SPI_##reg)
#define spi_writew(port, reg, value) \
	writew_relaxed((value), (port)->regs + SPI_##reg)

/* use PIO for small transfers, avoiding DMA setup/teardown overhead and
 * cache operations; better heuristics consider wordsize and bitrate.
 */
#define DMA_MIN_BYTES	16

#define AUTOSUSPEND_TIMEOUT	2000

struct atmel_spi_caps {
	bool	is_spi2;
	bool	has_wdrbt;
	bool	has_dma_support;
	bool	has_pdc_support;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The core SPI transfer engine just talks to a register bank to set up
 * DMA transfers; transfer queue progress is driven by IRQs.  The clock
 * framework provides the base clock, subdivided for each spi_device.
 */
struct atmel_spi {
	spinlock_t		lock;
<<<<<<< HEAD

=======
	unsigned long		flags;

	phys_addr_t		phybase;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __iomem		*regs;
	int			irq;
	struct clk		*clk;
	struct platform_device	*pdev;
<<<<<<< HEAD
	struct spi_device	*stay;

	u8			stopping;
	struct list_head	queue;
	struct spi_transfer	*current_transfer;
	unsigned long		current_remaining_bytes;
	struct spi_transfer	*next_transfer;
	unsigned long		next_remaining_bytes;

	void			*buffer;
	dma_addr_t		buffer_dma;
=======
	unsigned long		spi_clk;

	struct spi_transfer	*current_transfer;
	int			current_remaining_bytes;
	int			done_status;
	dma_addr_t		dma_addr_rx_bbuf;
	dma_addr_t		dma_addr_tx_bbuf;
	void			*addr_rx_bbuf;
	void			*addr_tx_bbuf;

	struct completion	xfer_completion;

	struct atmel_spi_caps	caps;

	bool			use_dma;
	bool			use_pdc;

	bool			keep_cs;

	u32			fifo_size;
	bool			last_polarity;
	u8			native_cs_free;
	u8			native_cs_for_gpio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Controller-specific per-slave state */
struct atmel_spi_device {
<<<<<<< HEAD
	unsigned int		npcs_pin;
	u32			csr;
};

#define BUFFER_SIZE		PAGE_SIZE
#define INVALID_DMA_ADDRESS	0xffffffff

/*
=======
	u32			csr;
};

#define SPI_MAX_DMA_XFER	65535 /* true for both PDC and DMA */
#define INVALID_DMA_ADDRESS	0xffffffff

/*
 * This frequency can be anything supported by the controller, but to avoid
 * unnecessary delay, the highest possible frequency is chosen.
 *
 * This frequency is the highest possible which is not interfering with other
 * chip select registers (see Note for Serial Clock Bit Rate configuration in
 * Atmel-11121F-ATARM-SAMA5D3-Series-Datasheet_02-Feb-16, page 1283)
 */
#define DUMMY_MSG_FREQUENCY	0x02
/*
 * 8 bits is the minimum data the controller is capable of sending.
 *
 * This message can be anything as it should not be treated by any SPI device.
 */
#define DUMMY_MSG		0xAA

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Version 2 of the SPI controller has
 *  - CR.LASTXFER
 *  - SPI_MR.DIV32 may become FDIV or must-be-zero (here: always zero)
 *  - SPI_SR.TXEMPTY, SPI_SR.NSSR (and corresponding irqs)
 *  - SPI_CSRx.CSAAT
 *  - SPI_CSRx.SBCR allows faster clocking
<<<<<<< HEAD
 *
 * We can determine the controller version by reading the VERSION
 * register, but I haven't checked that it exists on all chips, and
 * this is cheaper anyway.
 */
static bool atmel_spi_is_v2(void)
{
	return !cpu_is_at91rm9200();
}

/*
=======
 */
static bool atmel_spi_is_v2(struct atmel_spi *as)
{
	return as->caps.is_spi2;
}

/*
 * Send a dummy message.
 *
 * This is sometimes needed when using a CS GPIO to force clock transition when
 * switching between devices with different polarities.
 */
static void atmel_spi_send_dummy(struct atmel_spi *as, struct spi_device *spi, int chip_select)
{
	u32 status;
	u32 csr;

	/*
	 * Set a clock frequency to allow sending message on SPI bus.
	 * The frequency here can be anything, but is needed for
	 * the controller to send the data.
	 */
	csr = spi_readl(as, CSR0 + 4 * chip_select);
	csr = SPI_BFINS(SCBR, DUMMY_MSG_FREQUENCY, csr);
	spi_writel(as, CSR0 + 4 * chip_select, csr);

	/*
	 * Read all data coming from SPI bus, needed to be able to send
	 * the message.
	 */
	spi_readl(as, RDR);
	while (spi_readl(as, SR) & SPI_BIT(RDRF)) {
		spi_readl(as, RDR);
		cpu_relax();
	}

	spi_writel(as, TDR, DUMMY_MSG);

	readl_poll_timeout_atomic(as->regs + SPI_SR, status,
				  (status & SPI_BIT(TXEMPTY)), 1, 1000);
}


/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Earlier SPI controllers (e.g. on at91rm9200) have a design bug whereby
 * they assume that spi slave device state will not change on deselect, so
 * that automagic deselection is OK.  ("NPCSx rises if no data is to be
 * transmitted")  Not so!  Workaround uses nCSx pins as GPIOs; or newer
 * controllers have CSAAT and friends.
 *
<<<<<<< HEAD
 * Since the CSAAT functionality is a bit weird on newer controllers as
 * well, we use GPIO to control nCSx pins on all controllers, updating
 * MR.PCS to avoid confusing the controller.  Using GPIOs also lets us
 * support active-high chipselects despite the controller's belief that
 * only active-low devices/systems exists.
=======
 * Even controller newer than ar91rm9200, using GPIOs can make sens as
 * it lets us support active-high chipselects despite the controller's
 * belief that only active-low devices/systems exists.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * However, at91rm9200 has a second erratum whereby nCS0 doesn't work
 * right when driven with GPIO.  ("Mode Fault does not allow more than one
 * Master on Chip Select 0.")  No workaround exists for that ... so for
 * nCS0 on that chip, we (a) don't use the GPIO, (b) can't support CS_HIGH,
 * and (c) will trigger that first erratum in some cases.
 *
<<<<<<< HEAD
 * TODO: Test if the atmel_spi_is_v2() branch below works on
 * AT91RM9200 if we use some other register than CSR0. However, don't
 * do this unconditionally since AP7000 has an errata where the BITS
 * field in CSR0 overrides all other CSRs.
 */

static void cs_activate(struct atmel_spi *as, struct spi_device *spi)
{
	struct atmel_spi_device *asd = spi->controller_state;
	unsigned active = spi->mode & SPI_CS_HIGH;
	u32 mr;

	if (atmel_spi_is_v2()) {
		/*
		 * Always use CSR0. This ensures that the clock
		 * switches to the correct idle polarity before we
		 * toggle the CS.
		 */
		spi_writel(as, CSR0, asd->csr);
		spi_writel(as, MR, SPI_BF(PCS, 0x0e) | SPI_BIT(MODFDIS)
				| SPI_BIT(MSTR));
		mr = spi_readl(as, MR);
		gpio_set_value(asd->npcs_pin, active);
=======
 * When changing the clock polarity, the SPI controller waits for the next
 * transmission to enforce the default clock state. This may be an issue when
 * using a GPIO as Chip Select: the clock level is applied only when the first
 * packet is sent, once the CS has already been asserted. The workaround is to
 * avoid this by sending a first (dummy) message before toggling the CS state.
 */
static void cs_activate(struct atmel_spi *as, struct spi_device *spi)
{
	struct atmel_spi_device *asd = spi->controller_state;
	bool new_polarity;
	int chip_select;
	u32 mr;

	if (spi_get_csgpiod(spi, 0))
		chip_select = as->native_cs_for_gpio;
	else
		chip_select = spi_get_chipselect(spi, 0);

	if (atmel_spi_is_v2(as)) {
		spi_writel(as, CSR0 + 4 * chip_select, asd->csr);
		/* For the low SPI version, there is a issue that PDC transfer
		 * on CS1,2,3 needs SPI_CSR0.BITS config as SPI_CSR1,2,3.BITS
		 */
		spi_writel(as, CSR0, asd->csr);
		if (as->caps.has_wdrbt) {
			spi_writel(as, MR,
					SPI_BF(PCS, ~(0x01 << chip_select))
					| SPI_BIT(WDRBT)
					| SPI_BIT(MODFDIS)
					| SPI_BIT(MSTR));
		} else {
			spi_writel(as, MR,
					SPI_BF(PCS, ~(0x01 << chip_select))
					| SPI_BIT(MODFDIS)
					| SPI_BIT(MSTR));
		}

		mr = spi_readl(as, MR);

		/*
		 * Ensures the clock polarity is valid before we actually
		 * assert the CS to avoid spurious clock edges to be
		 * processed by the spi devices.
		 */
		if (spi_get_csgpiod(spi, 0)) {
			new_polarity = (asd->csr & SPI_BIT(CPOL)) != 0;
			if (new_polarity != as->last_polarity) {
				/*
				 * Need to disable the GPIO before sending the dummy
				 * message because it is already set by the spi core.
				 */
				gpiod_set_value_cansleep(spi_get_csgpiod(spi, 0), 0);
				atmel_spi_send_dummy(as, spi, chip_select);
				as->last_polarity = new_polarity;
				gpiod_set_value_cansleep(spi_get_csgpiod(spi, 0), 1);
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		u32 cpol = (spi->mode & SPI_CPOL) ? SPI_BIT(CPOL) : 0;
		int i;
		u32 csr;

		/* Make sure clock polarity is correct */
<<<<<<< HEAD
		for (i = 0; i < spi->master->num_chipselect; i++) {
=======
		for (i = 0; i < spi->controller->num_chipselect; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			csr = spi_readl(as, CSR0 + 4 * i);
			if ((csr ^ cpol) & SPI_BIT(CPOL))
				spi_writel(as, CSR0 + 4 * i,
						csr ^ SPI_BIT(CPOL));
		}

		mr = spi_readl(as, MR);
<<<<<<< HEAD
		mr = SPI_BFINS(PCS, ~(1 << spi->chip_select), mr);
		if (spi->chip_select != 0)
			gpio_set_value(asd->npcs_pin, active);
		spi_writel(as, MR, mr);
	}

	dev_dbg(&spi->dev, "activate %u%s, mr %08x\n",
			asd->npcs_pin, active ? " (high)" : "",
			mr);
=======
		mr = SPI_BFINS(PCS, ~(1 << chip_select), mr);
		spi_writel(as, MR, mr);
	}

	dev_dbg(&spi->dev, "activate NPCS, mr %08x\n", mr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void cs_deactivate(struct atmel_spi *as, struct spi_device *spi)
{
<<<<<<< HEAD
	struct atmel_spi_device *asd = spi->controller_state;
	unsigned active = spi->mode & SPI_CS_HIGH;
	u32 mr;

=======
	int chip_select;
	u32 mr;

	if (spi_get_csgpiod(spi, 0))
		chip_select = as->native_cs_for_gpio;
	else
		chip_select = spi_get_chipselect(spi, 0);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* only deactivate *this* device; sometimes transfers to
	 * another device may be active when this routine is called.
	 */
	mr = spi_readl(as, MR);
<<<<<<< HEAD
	if (~SPI_BFEXT(PCS, mr) & (1 << spi->chip_select)) {
=======
	if (~SPI_BFEXT(PCS, mr) & (1 << chip_select)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mr = SPI_BFINS(PCS, 0xf, mr);
		spi_writel(as, MR, mr);
	}

<<<<<<< HEAD
	dev_dbg(&spi->dev, "DEactivate %u%s, mr %08x\n",
			asd->npcs_pin, active ? " (low)" : "",
			mr);

	if (atmel_spi_is_v2() || spi->chip_select != 0)
		gpio_set_value(asd->npcs_pin, !active);
}

static inline int atmel_spi_xfer_is_last(struct spi_message *msg,
					struct spi_transfer *xfer)
{
	return msg->transfers.prev == &xfer->transfer_list;
}

static inline int atmel_spi_xfer_can_be_chained(struct spi_transfer *xfer)
{
	return xfer->delay_usecs == 0 && !xfer->cs_change;
}

static void atmel_spi_next_xfer_data(struct spi_master *master,
=======
	dev_dbg(&spi->dev, "DEactivate NPCS, mr %08x\n", mr);

	if (!spi_get_csgpiod(spi, 0))
		spi_writel(as, CR, SPI_BIT(LASTXFER));
}

static void atmel_spi_lock(struct atmel_spi *as) __acquires(&as->lock)
{
	spin_lock_irqsave(&as->lock, as->flags);
}

static void atmel_spi_unlock(struct atmel_spi *as) __releases(&as->lock)
{
	spin_unlock_irqrestore(&as->lock, as->flags);
}

static inline bool atmel_spi_is_vmalloc_xfer(struct spi_transfer *xfer)
{
	return is_vmalloc_addr(xfer->tx_buf) || is_vmalloc_addr(xfer->rx_buf);
}

static inline bool atmel_spi_use_dma(struct atmel_spi *as,
				struct spi_transfer *xfer)
{
	return as->use_dma && xfer->len >= DMA_MIN_BYTES;
}

static bool atmel_spi_can_dma(struct spi_controller *host,
			      struct spi_device *spi,
			      struct spi_transfer *xfer)
{
	struct atmel_spi *as = spi_controller_get_devdata(host);

	if (IS_ENABLED(CONFIG_SOC_SAM_V4_V5))
		return atmel_spi_use_dma(as, xfer) &&
			!atmel_spi_is_vmalloc_xfer(xfer);
	else
		return atmel_spi_use_dma(as, xfer);

}

static int atmel_spi_dma_slave_config(struct atmel_spi *as, u8 bits_per_word)
{
	struct spi_controller *host = platform_get_drvdata(as->pdev);
	struct dma_slave_config	slave_config;
	int err = 0;

	if (bits_per_word > 8) {
		slave_config.dst_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
		slave_config.src_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
	} else {
		slave_config.dst_addr_width = DMA_SLAVE_BUSWIDTH_1_BYTE;
		slave_config.src_addr_width = DMA_SLAVE_BUSWIDTH_1_BYTE;
	}

	slave_config.dst_addr = (dma_addr_t)as->phybase + SPI_TDR;
	slave_config.src_addr = (dma_addr_t)as->phybase + SPI_RDR;
	slave_config.src_maxburst = 1;
	slave_config.dst_maxburst = 1;
	slave_config.device_fc = false;

	/*
	 * This driver uses fixed peripheral select mode (PS bit set to '0' in
	 * the Mode Register).
	 * So according to the datasheet, when FIFOs are available (and
	 * enabled), the Transmit FIFO operates in Multiple Data Mode.
	 * In this mode, up to 2 data, not 4, can be written into the Transmit
	 * Data Register in a single access.
	 * However, the first data has to be written into the lowest 16 bits and
	 * the second data into the highest 16 bits of the Transmit
	 * Data Register. For 8bit data (the most frequent case), it would
	 * require to rework tx_buf so each data would actually fit 16 bits.
	 * So we'd rather write only one data at the time. Hence the transmit
	 * path works the same whether FIFOs are available (and enabled) or not.
	 */
	if (dmaengine_slave_config(host->dma_tx, &slave_config)) {
		dev_err(&as->pdev->dev,
			"failed to configure tx dma channel\n");
		err = -EINVAL;
	}

	/*
	 * This driver configures the spi controller for host mode (MSTR bit
	 * set to '1' in the Mode Register).
	 * So according to the datasheet, when FIFOs are available (and
	 * enabled), the Receive FIFO operates in Single Data Mode.
	 * So the receive path works the same whether FIFOs are available (and
	 * enabled) or not.
	 */
	if (dmaengine_slave_config(host->dma_rx, &slave_config)) {
		dev_err(&as->pdev->dev,
			"failed to configure rx dma channel\n");
		err = -EINVAL;
	}

	return err;
}

static int atmel_spi_configure_dma(struct spi_controller *host,
				   struct atmel_spi *as)
{
	struct device *dev = &as->pdev->dev;
	int err;

	host->dma_tx = dma_request_chan(dev, "tx");
	if (IS_ERR(host->dma_tx)) {
		err = PTR_ERR(host->dma_tx);
		dev_dbg(dev, "No TX DMA channel, DMA is disabled\n");
		goto error_clear;
	}

	host->dma_rx = dma_request_chan(dev, "rx");
	if (IS_ERR(host->dma_rx)) {
		err = PTR_ERR(host->dma_rx);
		/*
		 * No reason to check EPROBE_DEFER here since we have already
		 * requested tx channel.
		 */
		dev_dbg(dev, "No RX DMA channel, DMA is disabled\n");
		goto error;
	}

	err = atmel_spi_dma_slave_config(as, 8);
	if (err)
		goto error;

	dev_info(&as->pdev->dev,
			"Using %s (tx) and %s (rx) for DMA transfers\n",
			dma_chan_name(host->dma_tx),
			dma_chan_name(host->dma_rx));

	return 0;
error:
	if (!IS_ERR(host->dma_rx))
		dma_release_channel(host->dma_rx);
	if (!IS_ERR(host->dma_tx))
		dma_release_channel(host->dma_tx);
error_clear:
	host->dma_tx = host->dma_rx = NULL;
	return err;
}

static void atmel_spi_stop_dma(struct spi_controller *host)
{
	if (host->dma_rx)
		dmaengine_terminate_all(host->dma_rx);
	if (host->dma_tx)
		dmaengine_terminate_all(host->dma_tx);
}

static void atmel_spi_release_dma(struct spi_controller *host)
{
	if (host->dma_rx) {
		dma_release_channel(host->dma_rx);
		host->dma_rx = NULL;
	}
	if (host->dma_tx) {
		dma_release_channel(host->dma_tx);
		host->dma_tx = NULL;
	}
}

/* This function is called by the DMA driver from tasklet context */
static void dma_callback(void *data)
{
	struct spi_controller	*host = data;
	struct atmel_spi	*as = spi_controller_get_devdata(host);

	if (is_vmalloc_addr(as->current_transfer->rx_buf) &&
	    IS_ENABLED(CONFIG_SOC_SAM_V4_V5)) {
		memcpy(as->current_transfer->rx_buf, as->addr_rx_bbuf,
		       as->current_transfer->len);
	}
	complete(&as->xfer_completion);
}

/*
 * Next transfer using PIO without FIFO.
 */
static void atmel_spi_next_xfer_single(struct spi_controller *host,
				       struct spi_transfer *xfer)
{
	struct atmel_spi	*as = spi_controller_get_devdata(host);
	unsigned long xfer_pos = xfer->len - as->current_remaining_bytes;

	dev_vdbg(host->dev.parent, "atmel_spi_next_xfer_pio\n");

	/* Make sure data is not remaining in RDR */
	spi_readl(as, RDR);
	while (spi_readl(as, SR) & SPI_BIT(RDRF)) {
		spi_readl(as, RDR);
		cpu_relax();
	}

	if (xfer->bits_per_word > 8)
		spi_writel(as, TDR, *(u16 *)(xfer->tx_buf + xfer_pos));
	else
		spi_writel(as, TDR, *(u8 *)(xfer->tx_buf + xfer_pos));

	dev_dbg(host->dev.parent,
		"  start pio xfer %p: len %u tx %p rx %p bitpw %d\n",
		xfer, xfer->len, xfer->tx_buf, xfer->rx_buf,
		xfer->bits_per_word);

	/* Enable relevant interrupts */
	spi_writel(as, IER, SPI_BIT(RDRF) | SPI_BIT(OVRES));
}

/*
 * Next transfer using PIO with FIFO.
 */
static void atmel_spi_next_xfer_fifo(struct spi_controller *host,
				     struct spi_transfer *xfer)
{
	struct atmel_spi *as = spi_controller_get_devdata(host);
	u32 current_remaining_data, num_data;
	u32 offset = xfer->len - as->current_remaining_bytes;
	const u16 *words = (const u16 *)((u8 *)xfer->tx_buf + offset);
	const u8  *bytes = (const u8  *)((u8 *)xfer->tx_buf + offset);
	u16 td0, td1;
	u32 fifomr;

	dev_vdbg(host->dev.parent, "atmel_spi_next_xfer_fifo\n");

	/* Compute the number of data to transfer in the current iteration */
	current_remaining_data = ((xfer->bits_per_word > 8) ?
				  ((u32)as->current_remaining_bytes >> 1) :
				  (u32)as->current_remaining_bytes);
	num_data = min(current_remaining_data, as->fifo_size);

	/* Flush RX and TX FIFOs */
	spi_writel(as, CR, SPI_BIT(RXFCLR) | SPI_BIT(TXFCLR));
	while (spi_readl(as, FLR))
		cpu_relax();

	/* Set RX FIFO Threshold to the number of data to transfer */
	fifomr = spi_readl(as, FMR);
	spi_writel(as, FMR, SPI_BFINS(RXFTHRES, num_data, fifomr));

	/* Clear FIFO flags in the Status Register, especially RXFTHF */
	(void)spi_readl(as, SR);

	/* Fill TX FIFO */
	while (num_data >= 2) {
		if (xfer->bits_per_word > 8) {
			td0 = *words++;
			td1 = *words++;
		} else {
			td0 = *bytes++;
			td1 = *bytes++;
		}

		spi_writel(as, TDR, (td1 << 16) | td0);
		num_data -= 2;
	}

	if (num_data) {
		if (xfer->bits_per_word > 8)
			td0 = *words++;
		else
			td0 = *bytes++;

		spi_writew(as, TDR, td0);
		num_data--;
	}

	dev_dbg(host->dev.parent,
		"  start fifo xfer %p: len %u tx %p rx %p bitpw %d\n",
		xfer, xfer->len, xfer->tx_buf, xfer->rx_buf,
		xfer->bits_per_word);

	/*
	 * Enable RX FIFO Threshold Flag interrupt to be notified about
	 * transfer completion.
	 */
	spi_writel(as, IER, SPI_BIT(RXFTHF) | SPI_BIT(OVRES));
}

/*
 * Next transfer using PIO.
 */
static void atmel_spi_next_xfer_pio(struct spi_controller *host,
				    struct spi_transfer *xfer)
{
	struct atmel_spi *as = spi_controller_get_devdata(host);

	if (as->fifo_size)
		atmel_spi_next_xfer_fifo(host, xfer);
	else
		atmel_spi_next_xfer_single(host, xfer);
}

/*
 * Submit next transfer for DMA.
 */
static int atmel_spi_next_xfer_dma_submit(struct spi_controller *host,
				struct spi_transfer *xfer,
				u32 *plen)
{
	struct atmel_spi	*as = spi_controller_get_devdata(host);
	struct dma_chan		*rxchan = host->dma_rx;
	struct dma_chan		*txchan = host->dma_tx;
	struct dma_async_tx_descriptor *rxdesc;
	struct dma_async_tx_descriptor *txdesc;
	dma_cookie_t		cookie;

	dev_vdbg(host->dev.parent, "atmel_spi_next_xfer_dma_submit\n");

	/* Check that the channels are available */
	if (!rxchan || !txchan)
		return -ENODEV;


	*plen = xfer->len;

	if (atmel_spi_dma_slave_config(as, xfer->bits_per_word))
		goto err_exit;

	/* Send both scatterlists */
	if (atmel_spi_is_vmalloc_xfer(xfer) &&
	    IS_ENABLED(CONFIG_SOC_SAM_V4_V5)) {
		rxdesc = dmaengine_prep_slave_single(rxchan,
						     as->dma_addr_rx_bbuf,
						     xfer->len,
						     DMA_DEV_TO_MEM,
						     DMA_PREP_INTERRUPT |
						     DMA_CTRL_ACK);
	} else {
		rxdesc = dmaengine_prep_slave_sg(rxchan,
						 xfer->rx_sg.sgl,
						 xfer->rx_sg.nents,
						 DMA_DEV_TO_MEM,
						 DMA_PREP_INTERRUPT |
						 DMA_CTRL_ACK);
	}
	if (!rxdesc)
		goto err_dma;

	if (atmel_spi_is_vmalloc_xfer(xfer) &&
	    IS_ENABLED(CONFIG_SOC_SAM_V4_V5)) {
		memcpy(as->addr_tx_bbuf, xfer->tx_buf, xfer->len);
		txdesc = dmaengine_prep_slave_single(txchan,
						     as->dma_addr_tx_bbuf,
						     xfer->len, DMA_MEM_TO_DEV,
						     DMA_PREP_INTERRUPT |
						     DMA_CTRL_ACK);
	} else {
		txdesc = dmaengine_prep_slave_sg(txchan,
						 xfer->tx_sg.sgl,
						 xfer->tx_sg.nents,
						 DMA_MEM_TO_DEV,
						 DMA_PREP_INTERRUPT |
						 DMA_CTRL_ACK);
	}
	if (!txdesc)
		goto err_dma;

	dev_dbg(host->dev.parent,
		"  start dma xfer %p: len %u tx %p/%08llx rx %p/%08llx\n",
		xfer, xfer->len, xfer->tx_buf, (unsigned long long)xfer->tx_dma,
		xfer->rx_buf, (unsigned long long)xfer->rx_dma);

	/* Enable relevant interrupts */
	spi_writel(as, IER, SPI_BIT(OVRES));

	/* Put the callback on the RX transfer only, that should finish last */
	rxdesc->callback = dma_callback;
	rxdesc->callback_param = host;

	/* Submit and fire RX and TX with TX last so we're ready to read! */
	cookie = rxdesc->tx_submit(rxdesc);
	if (dma_submit_error(cookie))
		goto err_dma;
	cookie = txdesc->tx_submit(txdesc);
	if (dma_submit_error(cookie))
		goto err_dma;
	rxchan->device->device_issue_pending(rxchan);
	txchan->device->device_issue_pending(txchan);

	return 0;

err_dma:
	spi_writel(as, IDR, SPI_BIT(OVRES));
	atmel_spi_stop_dma(host);
err_exit:
	return -ENOMEM;
}

static void atmel_spi_next_xfer_data(struct spi_controller *host,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct spi_transfer *xfer,
				dma_addr_t *tx_dma,
				dma_addr_t *rx_dma,
				u32 *plen)
{
<<<<<<< HEAD
	struct atmel_spi	*as = spi_master_get_devdata(master);
	u32			len = *plen;

	/* use scratch buffer only when rx or tx data is unspecified */
	if (xfer->rx_buf)
		*rx_dma = xfer->rx_dma + xfer->len - *plen;
	else {
		*rx_dma = as->buffer_dma;
		if (len > BUFFER_SIZE)
			len = BUFFER_SIZE;
	}
	if (xfer->tx_buf)
		*tx_dma = xfer->tx_dma + xfer->len - *plen;
	else {
		*tx_dma = as->buffer_dma;
		if (len > BUFFER_SIZE)
			len = BUFFER_SIZE;
		memset(as->buffer, 0, len);
		dma_sync_single_for_device(&as->pdev->dev,
				as->buffer_dma, len, DMA_TO_DEVICE);
	}

	*plen = len;
}

/*
 * Submit next transfer for DMA.
 * lock is held, spi irq is blocked
 */
static void atmel_spi_next_xfer(struct spi_master *master,
				struct spi_message *msg)
{
	struct atmel_spi	*as = spi_master_get_devdata(master);
	struct spi_transfer	*xfer;
	u32			len, remaining;
	u32			ieval;
	dma_addr_t		tx_dma, rx_dma;

	if (!as->current_transfer)
		xfer = list_entry(msg->transfers.next,
				struct spi_transfer, transfer_list);
	else if (!as->next_transfer)
		xfer = list_entry(as->current_transfer->transfer_list.next,
				struct spi_transfer, transfer_list);
	else
		xfer = NULL;

	if (xfer) {
		spi_writel(as, PTCR, SPI_BIT(RXTDIS) | SPI_BIT(TXTDIS));

		len = xfer->len;
		atmel_spi_next_xfer_data(master, xfer, &tx_dma, &rx_dma, &len);
		remaining = xfer->len - len;

		spi_writel(as, RPR, rx_dma);
		spi_writel(as, TPR, tx_dma);

		if (msg->spi->bits_per_word > 8)
			len >>= 1;
		spi_writel(as, RCR, len);
		spi_writel(as, TCR, len);

		dev_dbg(&msg->spi->dev,
			"  start xfer %p: len %u tx %p/%08x rx %p/%08x\n",
			xfer, xfer->len, xfer->tx_buf, xfer->tx_dma,
			xfer->rx_buf, xfer->rx_dma);
	} else {
		xfer = as->next_transfer;
		remaining = as->next_remaining_bytes;
	}

	as->current_transfer = xfer;
	as->current_remaining_bytes = remaining;

	if (remaining > 0)
		len = remaining;
	else if (!atmel_spi_xfer_is_last(msg, xfer)
			&& atmel_spi_xfer_can_be_chained(xfer)) {
		xfer = list_entry(xfer->transfer_list.next,
				struct spi_transfer, transfer_list);
		len = xfer->len;
	} else
		xfer = NULL;

	as->next_transfer = xfer;

	if (xfer) {
		u32	total;

		total = len;
		atmel_spi_next_xfer_data(master, xfer, &tx_dma, &rx_dma, &len);
		as->next_remaining_bytes = total - len;
=======
	*rx_dma = xfer->rx_dma + xfer->len - *plen;
	*tx_dma = xfer->tx_dma + xfer->len - *plen;
	if (*plen > host->max_dma_len)
		*plen = host->max_dma_len;
}

static int atmel_spi_set_xfer_speed(struct atmel_spi *as,
				    struct spi_device *spi,
				    struct spi_transfer *xfer)
{
	u32			scbr, csr;
	unsigned long		bus_hz;
	int chip_select;

	if (spi_get_csgpiod(spi, 0))
		chip_select = as->native_cs_for_gpio;
	else
		chip_select = spi_get_chipselect(spi, 0);

	/* v1 chips start out at half the peripheral bus speed. */
	bus_hz = as->spi_clk;
	if (!atmel_spi_is_v2(as))
		bus_hz /= 2;

	/*
	 * Calculate the lowest divider that satisfies the
	 * constraint, assuming div32/fdiv/mbz == 0.
	 */
	scbr = DIV_ROUND_UP(bus_hz, xfer->speed_hz);

	/*
	 * If the resulting divider doesn't fit into the
	 * register bitfield, we can't satisfy the constraint.
	 */
	if (scbr >= (1 << SPI_SCBR_SIZE)) {
		dev_err(&spi->dev,
			"setup: %d Hz too slow, scbr %u; min %ld Hz\n",
			xfer->speed_hz, scbr, bus_hz/255);
		return -EINVAL;
	}
	if (scbr == 0) {
		dev_err(&spi->dev,
			"setup: %d Hz too high, scbr %u; max %ld Hz\n",
			xfer->speed_hz, scbr, bus_hz);
		return -EINVAL;
	}
	csr = spi_readl(as, CSR0 + 4 * chip_select);
	csr = SPI_BFINS(SCBR, scbr, csr);
	spi_writel(as, CSR0 + 4 * chip_select, csr);
	xfer->effective_speed_hz = bus_hz / scbr;

	return 0;
}

/*
 * Submit next transfer for PDC.
 * lock is held, spi irq is blocked
 */
static void atmel_spi_pdc_next_xfer(struct spi_controller *host,
					struct spi_transfer *xfer)
{
	struct atmel_spi	*as = spi_controller_get_devdata(host);
	u32			len;
	dma_addr_t		tx_dma, rx_dma;

	spi_writel(as, PTCR, SPI_BIT(RXTDIS) | SPI_BIT(TXTDIS));

	len = as->current_remaining_bytes;
	atmel_spi_next_xfer_data(host, xfer, &tx_dma, &rx_dma, &len);
	as->current_remaining_bytes -= len;

	spi_writel(as, RPR, rx_dma);
	spi_writel(as, TPR, tx_dma);

	if (xfer->bits_per_word > 8)
		len >>= 1;
	spi_writel(as, RCR, len);
	spi_writel(as, TCR, len);

	dev_dbg(&host->dev,
		"  start xfer %p: len %u tx %p/%08llx rx %p/%08llx\n",
		xfer, xfer->len, xfer->tx_buf,
		(unsigned long long)xfer->tx_dma, xfer->rx_buf,
		(unsigned long long)xfer->rx_dma);

	if (as->current_remaining_bytes) {
		len = as->current_remaining_bytes;
		atmel_spi_next_xfer_data(host, xfer, &tx_dma, &rx_dma, &len);
		as->current_remaining_bytes -= len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		spi_writel(as, RNPR, rx_dma);
		spi_writel(as, TNPR, tx_dma);

<<<<<<< HEAD
		if (msg->spi->bits_per_word > 8)
=======
		if (xfer->bits_per_word > 8)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			len >>= 1;
		spi_writel(as, RNCR, len);
		spi_writel(as, TNCR, len);

<<<<<<< HEAD
		dev_dbg(&msg->spi->dev,
			"  next xfer %p: len %u tx %p/%08x rx %p/%08x\n",
			xfer, xfer->len, xfer->tx_buf, xfer->tx_dma,
			xfer->rx_buf, xfer->rx_dma);
		ieval = SPI_BIT(ENDRX) | SPI_BIT(OVRES);
	} else {
		spi_writel(as, RNCR, 0);
		spi_writel(as, TNCR, 0);
		ieval = SPI_BIT(RXBUFF) | SPI_BIT(ENDRX) | SPI_BIT(OVRES);
	}

	/* REVISIT: We're waiting for ENDRX before we start the next
	 * transfer because we need to handle some difficult timing
	 * issues otherwise. If we wait for ENDTX in one transfer and
	 * then starts waiting for ENDRX in the next, it's difficult
	 * to tell the difference between the ENDRX interrupt we're
	 * actually waiting for and the ENDRX interrupt of the
=======
		dev_dbg(&host->dev,
			"  next xfer %p: len %u tx %p/%08llx rx %p/%08llx\n",
			xfer, xfer->len, xfer->tx_buf,
			(unsigned long long)xfer->tx_dma, xfer->rx_buf,
			(unsigned long long)xfer->rx_dma);
	}

	/* REVISIT: We're waiting for RXBUFF before we start the next
	 * transfer because we need to handle some difficult timing
	 * issues otherwise. If we wait for TXBUFE in one transfer and
	 * then starts waiting for RXBUFF in the next, it's difficult
	 * to tell the difference between the RXBUFF interrupt we're
	 * actually waiting for and the RXBUFF interrupt of the
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * previous transfer.
	 *
	 * It should be doable, though. Just not now...
	 */
<<<<<<< HEAD
	spi_writel(as, IER, ieval);
	spi_writel(as, PTCR, SPI_BIT(TXTEN) | SPI_BIT(RXTEN));
}

static void atmel_spi_next_message(struct spi_master *master)
{
	struct atmel_spi	*as = spi_master_get_devdata(master);
	struct spi_message	*msg;
	struct spi_device	*spi;

	BUG_ON(as->current_transfer);

	msg = list_entry(as->queue.next, struct spi_message, queue);
	spi = msg->spi;

	dev_dbg(master->dev.parent, "start message %p for %s\n",
			msg, dev_name(&spi->dev));

	/* select chip if it's not still active */
	if (as->stay) {
		if (as->stay != spi) {
			cs_deactivate(as, as->stay);
			cs_activate(as, spi);
		}
		as->stay = NULL;
	} else
		cs_activate(as, spi);

	atmel_spi_next_xfer(master, msg);
}

=======
	spi_writel(as, IER, SPI_BIT(RXBUFF) | SPI_BIT(OVRES));
	spi_writel(as, PTCR, SPI_BIT(TXTEN) | SPI_BIT(RXTEN));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * For DMA, tx_buf/tx_dma have the same relationship as rx_buf/rx_dma:
 *  - The buffer is either valid for CPU access, else NULL
 *  - If the buffer is valid, so is its DMA address
 *
 * This driver manages the dma address unless message->is_dma_mapped.
 */
static int
atmel_spi_dma_map_xfer(struct atmel_spi *as, struct spi_transfer *xfer)
{
	struct device	*dev = &as->pdev->dev;

	xfer->tx_dma = xfer->rx_dma = INVALID_DMA_ADDRESS;
	if (xfer->tx_buf) {
		/* tx_buf is a const void* where we need a void * for the dma
		 * mapping */
		void *nonconst_tx = (void *)xfer->tx_buf;

		xfer->tx_dma = dma_map_single(dev,
				nonconst_tx, xfer->len,
				DMA_TO_DEVICE);
		if (dma_mapping_error(dev, xfer->tx_dma))
			return -ENOMEM;
	}
	if (xfer->rx_buf) {
		xfer->rx_dma = dma_map_single(dev,
				xfer->rx_buf, xfer->len,
				DMA_FROM_DEVICE);
		if (dma_mapping_error(dev, xfer->rx_dma)) {
			if (xfer->tx_buf)
				dma_unmap_single(dev,
						xfer->tx_dma, xfer->len,
						DMA_TO_DEVICE);
			return -ENOMEM;
		}
	}
	return 0;
}

<<<<<<< HEAD
static void atmel_spi_dma_unmap_xfer(struct spi_master *master,
				     struct spi_transfer *xfer)
{
	if (xfer->tx_dma != INVALID_DMA_ADDRESS)
		dma_unmap_single(master->dev.parent, xfer->tx_dma,
				 xfer->len, DMA_TO_DEVICE);
	if (xfer->rx_dma != INVALID_DMA_ADDRESS)
		dma_unmap_single(master->dev.parent, xfer->rx_dma,
				 xfer->len, DMA_FROM_DEVICE);
}

static void
atmel_spi_msg_done(struct spi_master *master, struct atmel_spi *as,
		struct spi_message *msg, int status, int stay)
{
	if (!stay || status < 0)
		cs_deactivate(as, msg->spi);
	else
		as->stay = msg->spi;

	list_del(&msg->queue);
	msg->status = status;

	dev_dbg(master->dev.parent,
		"xfer complete: %u bytes transferred\n",
		msg->actual_length);

	spin_unlock(&as->lock);
	msg->complete(msg->context);
	spin_lock(&as->lock);

	as->current_transfer = NULL;
	as->next_transfer = NULL;

	/* continue if needed */
	if (list_empty(&as->queue) || as->stopping)
		spi_writel(as, PTCR, SPI_BIT(RXTDIS) | SPI_BIT(TXTDIS));
	else
		atmel_spi_next_message(master);
}

static irqreturn_t
atmel_spi_interrupt(int irq, void *dev_id)
{
	struct spi_master	*master = dev_id;
	struct atmel_spi	*as = spi_master_get_devdata(master);
	struct spi_message	*msg;
	struct spi_transfer	*xfer;
	u32			status, pending, imr;
	int			ret = IRQ_NONE;

	spin_lock(&as->lock);

	xfer = as->current_transfer;
	msg = list_entry(as->queue.next, struct spi_message, queue);

=======
static void atmel_spi_dma_unmap_xfer(struct spi_controller *host,
				     struct spi_transfer *xfer)
{
	if (xfer->tx_dma != INVALID_DMA_ADDRESS)
		dma_unmap_single(host->dev.parent, xfer->tx_dma,
				 xfer->len, DMA_TO_DEVICE);
	if (xfer->rx_dma != INVALID_DMA_ADDRESS)
		dma_unmap_single(host->dev.parent, xfer->rx_dma,
				 xfer->len, DMA_FROM_DEVICE);
}

static void atmel_spi_disable_pdc_transfer(struct atmel_spi *as)
{
	spi_writel(as, PTCR, SPI_BIT(RXTDIS) | SPI_BIT(TXTDIS));
}

static void
atmel_spi_pump_single_data(struct atmel_spi *as, struct spi_transfer *xfer)
{
	u8		*rxp;
	u16		*rxp16;
	unsigned long	xfer_pos = xfer->len - as->current_remaining_bytes;

	if (xfer->bits_per_word > 8) {
		rxp16 = (u16 *)(((u8 *)xfer->rx_buf) + xfer_pos);
		*rxp16 = spi_readl(as, RDR);
	} else {
		rxp = ((u8 *)xfer->rx_buf) + xfer_pos;
		*rxp = spi_readl(as, RDR);
	}
	if (xfer->bits_per_word > 8) {
		if (as->current_remaining_bytes > 2)
			as->current_remaining_bytes -= 2;
		else
			as->current_remaining_bytes = 0;
	} else {
		as->current_remaining_bytes--;
	}
}

static void
atmel_spi_pump_fifo_data(struct atmel_spi *as, struct spi_transfer *xfer)
{
	u32 fifolr = spi_readl(as, FLR);
	u32 num_bytes, num_data = SPI_BFEXT(RXFL, fifolr);
	u32 offset = xfer->len - as->current_remaining_bytes;
	u16 *words = (u16 *)((u8 *)xfer->rx_buf + offset);
	u8  *bytes = (u8  *)((u8 *)xfer->rx_buf + offset);
	u16 rd; /* RD field is the lowest 16 bits of RDR */

	/* Update the number of remaining bytes to transfer */
	num_bytes = ((xfer->bits_per_word > 8) ?
		     (num_data << 1) :
		     num_data);

	if (as->current_remaining_bytes > num_bytes)
		as->current_remaining_bytes -= num_bytes;
	else
		as->current_remaining_bytes = 0;

	/* Handle odd number of bytes when data are more than 8bit width */
	if (xfer->bits_per_word > 8)
		as->current_remaining_bytes &= ~0x1;

	/* Read data */
	while (num_data) {
		rd = spi_readl(as, RDR);
		if (xfer->bits_per_word > 8)
			*words++ = rd;
		else
			*bytes++ = rd;
		num_data--;
	}
}

/* Called from IRQ
 *
 * Must update "current_remaining_bytes" to keep track of data
 * to transfer.
 */
static void
atmel_spi_pump_pio_data(struct atmel_spi *as, struct spi_transfer *xfer)
{
	if (as->fifo_size)
		atmel_spi_pump_fifo_data(as, xfer);
	else
		atmel_spi_pump_single_data(as, xfer);
}

/* Interrupt
 *
 */
static irqreturn_t
atmel_spi_pio_interrupt(int irq, void *dev_id)
{
	struct spi_controller	*host = dev_id;
	struct atmel_spi	*as = spi_controller_get_devdata(host);
	u32			status, pending, imr;
	struct spi_transfer	*xfer;
	int			ret = IRQ_NONE;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	imr = spi_readl(as, IMR);
	status = spi_readl(as, SR);
	pending = status & imr;

	if (pending & SPI_BIT(OVRES)) {
<<<<<<< HEAD
		int timeout;

		ret = IRQ_HANDLED;

		spi_writel(as, IDR, (SPI_BIT(RXBUFF) | SPI_BIT(ENDRX)
				     | SPI_BIT(OVRES)));
=======
		ret = IRQ_HANDLED;
		spi_writel(as, IDR, SPI_BIT(OVRES));
		dev_warn(host->dev.parent, "overrun\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * When we get an overrun, we disregard the current
		 * transfer. Data will not be copied back from any
		 * bounce buffer and msg->actual_len will not be
		 * updated with the last xfer.
		 *
		 * We will also not process any remaning transfers in
		 * the message.
<<<<<<< HEAD
		 *
		 * First, stop the transfer and unmap the DMA buffers.
		 */
		spi_writel(as, PTCR, SPI_BIT(RXTDIS) | SPI_BIT(TXTDIS));
		if (!msg->is_dma_mapped)
			atmel_spi_dma_unmap_xfer(master, xfer);

		/* REVISIT: udelay in irq is unfriendly */
		if (xfer->delay_usecs)
			udelay(xfer->delay_usecs);

		dev_warn(master->dev.parent, "overrun (%u/%u remaining)\n",
			 spi_readl(as, TCR), spi_readl(as, RCR));

		/*
		 * Clean up DMA registers and make sure the data
		 * registers are empty.
		 */
		spi_writel(as, RNCR, 0);
		spi_writel(as, TNCR, 0);
		spi_writel(as, RCR, 0);
		spi_writel(as, TCR, 0);
		for (timeout = 1000; timeout; timeout--)
			if (spi_readl(as, SR) & SPI_BIT(TXEMPTY))
				break;
		if (!timeout)
			dev_warn(master->dev.parent,
				 "timeout waiting for TXEMPTY");
		while (spi_readl(as, SR) & SPI_BIT(RDRF))
			spi_readl(as, RDR);
=======
		 */
		as->done_status = -EIO;
		smp_wmb();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* Clear any overrun happening while cleaning up */
		spi_readl(as, SR);

<<<<<<< HEAD
		atmel_spi_msg_done(master, as, msg, -EIO, 0);
=======
		complete(&as->xfer_completion);

	} else if (pending & (SPI_BIT(RDRF) | SPI_BIT(RXFTHF))) {
		atmel_spi_lock(as);

		if (as->current_remaining_bytes) {
			ret = IRQ_HANDLED;
			xfer = as->current_transfer;
			atmel_spi_pump_pio_data(as, xfer);
			if (!as->current_remaining_bytes)
				spi_writel(as, IDR, pending);

			complete(&as->xfer_completion);
		}

		atmel_spi_unlock(as);
	} else {
		WARN_ONCE(pending, "IRQ not handled, pending = %x\n", pending);
		ret = IRQ_HANDLED;
		spi_writel(as, IDR, pending);
	}

	return ret;
}

static irqreturn_t
atmel_spi_pdc_interrupt(int irq, void *dev_id)
{
	struct spi_controller	*host = dev_id;
	struct atmel_spi	*as = spi_controller_get_devdata(host);
	u32			status, pending, imr;
	int			ret = IRQ_NONE;

	imr = spi_readl(as, IMR);
	status = spi_readl(as, SR);
	pending = status & imr;

	if (pending & SPI_BIT(OVRES)) {

		ret = IRQ_HANDLED;

		spi_writel(as, IDR, (SPI_BIT(RXBUFF) | SPI_BIT(ENDRX)
				     | SPI_BIT(OVRES)));

		/* Clear any overrun happening while cleaning up */
		spi_readl(as, SR);

		as->done_status = -EIO;

		complete(&as->xfer_completion);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (pending & (SPI_BIT(RXBUFF) | SPI_BIT(ENDRX))) {
		ret = IRQ_HANDLED;

		spi_writel(as, IDR, pending);

<<<<<<< HEAD
		if (as->current_remaining_bytes == 0) {
			msg->actual_length += xfer->len;

			if (!msg->is_dma_mapped)
				atmel_spi_dma_unmap_xfer(master, xfer);

			/* REVISIT: udelay in irq is unfriendly */
			if (xfer->delay_usecs)
				udelay(xfer->delay_usecs);

			if (atmel_spi_xfer_is_last(msg, xfer)) {
				/* report completed message */
				atmel_spi_msg_done(master, as, msg, 0,
						xfer->cs_change);
			} else {
				if (xfer->cs_change) {
					cs_deactivate(as, msg->spi);
					udelay(1);
					cs_activate(as, msg->spi);
				}

				/*
				 * Not done yet. Submit the next transfer.
				 *
				 * FIXME handle protocol options for xfer
				 */
				atmel_spi_next_xfer(master, msg);
			}
		} else {
			/*
			 * Keep going, we still have data to send in
			 * the current transfer.
			 */
			atmel_spi_next_xfer(master, msg);
		}
	}

	spin_unlock(&as->lock);

	return ret;
}

=======
		complete(&as->xfer_completion);
	}

	return ret;
}

static int atmel_word_delay_csr(struct spi_device *spi, struct atmel_spi *as)
{
	struct spi_delay *delay = &spi->word_delay;
	u32 value = delay->value;

	switch (delay->unit) {
	case SPI_DELAY_UNIT_NSECS:
		value /= 1000;
		break;
	case SPI_DELAY_UNIT_USECS:
		break;
	default:
		return -EINVAL;
	}

	return (as->spi_clk / 1000000 * value) >> 5;
}

static void initialize_native_cs_for_gpio(struct atmel_spi *as)
{
	int i;
	struct spi_controller *host = platform_get_drvdata(as->pdev);

	if (!as->native_cs_free)
		return; /* already initialized */

	if (!host->cs_gpiods)
		return; /* No CS GPIO */

	/*
	 * On the first version of the controller (AT91RM9200), CS0
	 * can't be used associated with GPIO
	 */
	if (atmel_spi_is_v2(as))
		i = 0;
	else
		i = 1;

	for (; i < 4; i++)
		if (host->cs_gpiods[i])
			as->native_cs_free |= BIT(i);

	if (as->native_cs_free)
		as->native_cs_for_gpio = ffs(as->native_cs_free);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int atmel_spi_setup(struct spi_device *spi)
{
	struct atmel_spi	*as;
	struct atmel_spi_device	*asd;
<<<<<<< HEAD
	u32			scbr, csr;
	unsigned int		bits = spi->bits_per_word;
	unsigned long		bus_hz;
	unsigned int		npcs_pin;
	int			ret;

	as = spi_master_get_devdata(spi->master);

	if (as->stopping)
		return -ESHUTDOWN;

	if (spi->chip_select > spi->master->num_chipselect) {
		dev_dbg(&spi->dev,
				"setup: invalid chipselect %u (%u defined)\n",
				spi->chip_select, spi->master->num_chipselect);
		return -EINVAL;
	}

	if (bits < 8 || bits > 16) {
		dev_dbg(&spi->dev,
				"setup: invalid bits_per_word %u (8 to 16)\n",
				bits);
		return -EINVAL;
	}

	/* see notes above re chipselect */
	if (!atmel_spi_is_v2()
			&& spi->chip_select == 0
			&& (spi->mode & SPI_CS_HIGH)) {
		dev_dbg(&spi->dev, "setup: can't be active-high\n");
		return -EINVAL;
	}

	/* v1 chips start out at half the peripheral bus speed. */
	bus_hz = clk_get_rate(as->clk);
	if (!atmel_spi_is_v2())
		bus_hz /= 2;

	if (spi->max_speed_hz) {
		/*
		 * Calculate the lowest divider that satisfies the
		 * constraint, assuming div32/fdiv/mbz == 0.
		 */
		scbr = DIV_ROUND_UP(bus_hz, spi->max_speed_hz);

		/*
		 * If the resulting divider doesn't fit into the
		 * register bitfield, we can't satisfy the constraint.
		 */
		if (scbr >= (1 << SPI_SCBR_SIZE)) {
			dev_dbg(&spi->dev,
				"setup: %d Hz too slow, scbr %u; min %ld Hz\n",
				spi->max_speed_hz, scbr, bus_hz/255);
			return -EINVAL;
		}
	} else
		/* speed zero means "as slow as possible" */
		scbr = 0xff;

	csr = SPI_BF(SCBR, scbr) | SPI_BF(BITS, bits - 8);
=======
	u32			csr;
	unsigned int		bits = spi->bits_per_word;
	int chip_select;
	int			word_delay_csr;

	as = spi_controller_get_devdata(spi->controller);

	/* see notes above re chipselect */
	if (!spi_get_csgpiod(spi, 0) && (spi->mode & SPI_CS_HIGH)) {
		dev_warn(&spi->dev, "setup: non GPIO CS can't be active-high\n");
		return -EINVAL;
	}

	/* Setup() is called during spi_register_controller(aka
	 * spi_register_master) but after all membmers of the cs_gpiod
	 * array have been filled, so we can looked for which native
	 * CS will be free for using with GPIO
	 */
	initialize_native_cs_for_gpio(as);

	if (spi_get_csgpiod(spi, 0) && as->native_cs_free) {
		dev_err(&spi->dev,
			"No native CS available to support this GPIO CS\n");
		return -EBUSY;
	}

	if (spi_get_csgpiod(spi, 0))
		chip_select = as->native_cs_for_gpio;
	else
		chip_select = spi_get_chipselect(spi, 0);

	csr = SPI_BF(BITS, bits - 8);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (spi->mode & SPI_CPOL)
		csr |= SPI_BIT(CPOL);
	if (!(spi->mode & SPI_CPHA))
		csr |= SPI_BIT(NCPHA);

<<<<<<< HEAD
	/* DLYBS is mostly irrelevant since we manage chipselect using GPIOs.
	 *
	 * DLYBCT would add delays between words, slowing down transfers.
	 * It could potentially be useful to cope with DMA bottlenecks, but
	 * in those cases it's probably best to just use a lower bitrate.
	 */
	csr |= SPI_BF(DLYBS, 0);
	csr |= SPI_BF(DLYBCT, 0);

	/* chipselect must have been muxed as GPIO (e.g. in board setup) */
	npcs_pin = (unsigned int)spi->controller_data;
=======
	if (!spi_get_csgpiod(spi, 0))
		csr |= SPI_BIT(CSAAT);
	csr |= SPI_BF(DLYBS, 0);

	word_delay_csr = atmel_word_delay_csr(spi, as);
	if (word_delay_csr < 0)
		return word_delay_csr;

	/* DLYBCT adds delays between words.  This is useful for slow devices
	 * that need a bit of time to setup the next transfer.
	 */
	csr |= SPI_BF(DLYBCT, word_delay_csr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	asd = spi->controller_state;
	if (!asd) {
		asd = kzalloc(sizeof(struct atmel_spi_device), GFP_KERNEL);
		if (!asd)
			return -ENOMEM;

<<<<<<< HEAD
		ret = gpio_request(npcs_pin, dev_name(&spi->dev));
		if (ret) {
			kfree(asd);
			return ret;
		}

		asd->npcs_pin = npcs_pin;
		spi->controller_state = asd;
		gpio_direction_output(npcs_pin, !(spi->mode & SPI_CS_HIGH));
	} else {
		unsigned long		flags;

		spin_lock_irqsave(&as->lock, flags);
		if (as->stay == spi)
			as->stay = NULL;
		cs_deactivate(as, spi);
		spin_unlock_irqrestore(&as->lock, flags);
=======
		spi->controller_state = asd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	asd->csr = csr;

	dev_dbg(&spi->dev,
<<<<<<< HEAD
		"setup: %lu Hz bpw %u mode 0x%x -> csr%d %08x\n",
		bus_hz / scbr, bits, spi->mode, spi->chip_select, csr);

	if (!atmel_spi_is_v2())
		spi_writel(as, CSR0 + 4 * spi->chip_select, csr);
=======
		"setup: bpw %u mode 0x%x -> csr%d %08x\n",
		bits, spi->mode, spi_get_chipselect(spi, 0), csr);

	if (!atmel_spi_is_v2(as))
		spi_writel(as, CSR0 + 4 * chip_select, csr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int atmel_spi_transfer(struct spi_device *spi, struct spi_message *msg)
{
	struct atmel_spi	*as;
	struct spi_transfer	*xfer;
	unsigned long		flags;
	struct device		*controller = spi->master->dev.parent;
	u8			bits;
	struct atmel_spi_device	*asd;

	as = spi_master_get_devdata(spi->master);

	dev_dbg(controller, "new message %p submitted for %s\n",
			msg, dev_name(&spi->dev));

	if (unlikely(list_empty(&msg->transfers)))
		return -EINVAL;

	if (as->stopping)
		return -ESHUTDOWN;

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		if (!(xfer->tx_buf || xfer->rx_buf) && xfer->len) {
			dev_dbg(&spi->dev, "missing rx or tx buf\n");
			return -EINVAL;
		}

		if (xfer->bits_per_word) {
			asd = spi->controller_state;
			bits = (asd->csr >> 4) & 0xf;
			if (bits != xfer->bits_per_word - 8) {
				dev_dbg(&spi->dev, "you can't yet change "
					 "bits_per_word in transfers\n");
				return -ENOPROTOOPT;
			}
		}

		/* FIXME implement these protocol options!! */
		if (xfer->speed_hz) {
			dev_dbg(&spi->dev, "no protocol options yet\n");
			return -ENOPROTOOPT;
		}

		/*
		 * DMA map early, for performance (empties dcache ASAP) and
		 * better fault reporting.  This is a DMA-only driver.
		 *
		 * NOTE that if dma_unmap_single() ever starts to do work on
		 * platforms supported by this driver, we would need to clean
		 * up mappings for previously-mapped transfers.
		 */
		if (!msg->is_dma_mapped) {
			if (atmel_spi_dma_map_xfer(as, xfer) < 0)
				return -ENOMEM;
		}
	}

#ifdef VERBOSE
	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		dev_dbg(controller,
			"  xfer %p: len %u tx %p/%08x rx %p/%08x\n",
			xfer, xfer->len,
			xfer->tx_buf, xfer->tx_dma,
			xfer->rx_buf, xfer->rx_dma);
	}
#endif

	msg->status = -EINPROGRESS;
	msg->actual_length = 0;

	spin_lock_irqsave(&as->lock, flags);
	list_add_tail(&msg->queue, &as->queue);
	if (!as->current_transfer)
		atmel_spi_next_message(spi->master);
	spin_unlock_irqrestore(&as->lock, flags);

	return 0;
=======
static void atmel_spi_set_cs(struct spi_device *spi, bool enable)
{
	struct atmel_spi *as = spi_controller_get_devdata(spi->controller);
	/* the core doesn't really pass us enable/disable, but CS HIGH vs CS LOW
	 * since we already have routines for activate/deactivate translate
	 * high/low to active/inactive
	 */
	enable = (!!(spi->mode & SPI_CS_HIGH) == enable);

	if (enable) {
		cs_activate(as, spi);
	} else {
		cs_deactivate(as, spi);
	}

}

static int atmel_spi_one_transfer(struct spi_controller *host,
					struct spi_device *spi,
					struct spi_transfer *xfer)
{
	struct atmel_spi	*as;
	u8			bits;
	u32			len;
	struct atmel_spi_device	*asd;
	int			timeout;
	int			ret;
	unsigned int		dma_timeout;
	long			ret_timeout;

	as = spi_controller_get_devdata(host);

	asd = spi->controller_state;
	bits = (asd->csr >> 4) & 0xf;
	if (bits != xfer->bits_per_word - 8) {
		dev_dbg(&spi->dev,
			"you can't yet change bits_per_word in transfers\n");
		return -ENOPROTOOPT;
	}

	/*
	 * DMA map early, for performance (empties dcache ASAP) and
	 * better fault reporting.
	 */
	if ((!host->cur_msg->is_dma_mapped)
		&& as->use_pdc) {
		if (atmel_spi_dma_map_xfer(as, xfer) < 0)
			return -ENOMEM;
	}

	atmel_spi_set_xfer_speed(as, spi, xfer);

	as->done_status = 0;
	as->current_transfer = xfer;
	as->current_remaining_bytes = xfer->len;
	while (as->current_remaining_bytes) {
		reinit_completion(&as->xfer_completion);

		if (as->use_pdc) {
			atmel_spi_lock(as);
			atmel_spi_pdc_next_xfer(host, xfer);
			atmel_spi_unlock(as);
		} else if (atmel_spi_use_dma(as, xfer)) {
			len = as->current_remaining_bytes;
			ret = atmel_spi_next_xfer_dma_submit(host,
								xfer, &len);
			if (ret) {
				dev_err(&spi->dev,
					"unable to use DMA, fallback to PIO\n");
				as->done_status = ret;
				break;
			} else {
				as->current_remaining_bytes -= len;
				if (as->current_remaining_bytes < 0)
					as->current_remaining_bytes = 0;
			}
		} else {
			atmel_spi_lock(as);
			atmel_spi_next_xfer_pio(host, xfer);
			atmel_spi_unlock(as);
		}

		dma_timeout = msecs_to_jiffies(spi_controller_xfer_timeout(host, xfer));
		ret_timeout = wait_for_completion_timeout(&as->xfer_completion, dma_timeout);
		if (!ret_timeout) {
			dev_err(&spi->dev, "spi transfer timeout\n");
			as->done_status = -EIO;
		}

		if (as->done_status)
			break;
	}

	if (as->done_status) {
		if (as->use_pdc) {
			dev_warn(host->dev.parent,
				"overrun (%u/%u remaining)\n",
				spi_readl(as, TCR), spi_readl(as, RCR));

			/*
			 * Clean up DMA registers and make sure the data
			 * registers are empty.
			 */
			spi_writel(as, RNCR, 0);
			spi_writel(as, TNCR, 0);
			spi_writel(as, RCR, 0);
			spi_writel(as, TCR, 0);
			for (timeout = 1000; timeout; timeout--)
				if (spi_readl(as, SR) & SPI_BIT(TXEMPTY))
					break;
			if (!timeout)
				dev_warn(host->dev.parent,
					 "timeout waiting for TXEMPTY");
			while (spi_readl(as, SR) & SPI_BIT(RDRF))
				spi_readl(as, RDR);

			/* Clear any overrun happening while cleaning up */
			spi_readl(as, SR);

		} else if (atmel_spi_use_dma(as, xfer)) {
			atmel_spi_stop_dma(host);
		}
	}

	if (!host->cur_msg->is_dma_mapped
		&& as->use_pdc)
		atmel_spi_dma_unmap_xfer(host, xfer);

	if (as->use_pdc)
		atmel_spi_disable_pdc_transfer(as);

	return as->done_status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void atmel_spi_cleanup(struct spi_device *spi)
{
<<<<<<< HEAD
	struct atmel_spi	*as = spi_master_get_devdata(spi->master);
	struct atmel_spi_device	*asd = spi->controller_state;
	unsigned		gpio = (unsigned) spi->controller_data;
	unsigned long		flags;
=======
	struct atmel_spi_device	*asd = spi->controller_state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!asd)
		return;

<<<<<<< HEAD
	spin_lock_irqsave(&as->lock, flags);
	if (as->stay == spi) {
		as->stay = NULL;
		cs_deactivate(as, spi);
	}
	spin_unlock_irqrestore(&as->lock, flags);

	spi->controller_state = NULL;
	gpio_free(gpio);
	kfree(asd);
}

/*-------------------------------------------------------------------------*/

static int __devinit atmel_spi_probe(struct platform_device *pdev)
=======
	spi->controller_state = NULL;
	kfree(asd);
}

static inline unsigned int atmel_get_version(struct atmel_spi *as)
{
	return spi_readl(as, VERSION) & 0x00000fff;
}

static void atmel_get_caps(struct atmel_spi *as)
{
	unsigned int version;

	version = atmel_get_version(as);

	as->caps.is_spi2 = version > 0x121;
	as->caps.has_wdrbt = version >= 0x210;
	as->caps.has_dma_support = version >= 0x212;
	as->caps.has_pdc_support = version < 0x212;
}

static void atmel_spi_init(struct atmel_spi *as)
{
	spi_writel(as, CR, SPI_BIT(SWRST));
	spi_writel(as, CR, SPI_BIT(SWRST)); /* AT91SAM9263 Rev B workaround */

	/* It is recommended to enable FIFOs first thing after reset */
	if (as->fifo_size)
		spi_writel(as, CR, SPI_BIT(FIFOEN));

	if (as->caps.has_wdrbt) {
		spi_writel(as, MR, SPI_BIT(WDRBT) | SPI_BIT(MODFDIS)
				| SPI_BIT(MSTR));
	} else {
		spi_writel(as, MR, SPI_BIT(MSTR) | SPI_BIT(MODFDIS));
	}

	if (as->use_pdc)
		spi_writel(as, PTCR, SPI_BIT(RXTDIS) | SPI_BIT(TXTDIS));
	spi_writel(as, CR, SPI_BIT(SPIEN));
}

static int atmel_spi_probe(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct resource		*regs;
	int			irq;
	struct clk		*clk;
	int			ret;
<<<<<<< HEAD
	struct spi_master	*master;
	struct atmel_spi	*as;

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!regs)
		return -ENXIO;
=======
	struct spi_controller	*host;
	struct atmel_spi	*as;

	/* Select default pin state */
	pinctrl_pm_select_default_state(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

<<<<<<< HEAD
	clk = clk_get(&pdev->dev, "spi_clk");
=======
	clk = devm_clk_get(&pdev->dev, "spi_clk");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(clk))
		return PTR_ERR(clk);

	/* setup spi core then atmel-specific driver state */
<<<<<<< HEAD
	ret = -ENOMEM;
	master = spi_alloc_master(&pdev->dev, sizeof *as);
	if (!master)
		goto out_free;

	/* the spi->mode bits understood by this driver: */
	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;

	master->bus_num = pdev->id;
	master->num_chipselect = 4;
	master->setup = atmel_spi_setup;
	master->transfer = atmel_spi_transfer;
	master->cleanup = atmel_spi_cleanup;
	platform_set_drvdata(pdev, master);

	as = spi_master_get_devdata(master);

	/*
	 * Scratch buffer is used for throwaway rx and tx data.
	 * It's coherent to minimize dcache pollution.
	 */
	as->buffer = dma_alloc_coherent(&pdev->dev, BUFFER_SIZE,
					&as->buffer_dma, GFP_KERNEL);
	if (!as->buffer)
		goto out_free;

	spin_lock_init(&as->lock);
	INIT_LIST_HEAD(&as->queue);
	as->pdev = pdev;
	as->regs = ioremap(regs->start, resource_size(regs));
	if (!as->regs)
		goto out_free_buffer;
	as->irq = irq;
	as->clk = clk;

	ret = request_irq(irq, atmel_spi_interrupt, 0,
			dev_name(&pdev->dev), master);
=======
	host = spi_alloc_host(&pdev->dev, sizeof(*as));
	if (!host)
		return -ENOMEM;

	/* the spi->mode bits understood by this driver: */
	host->use_gpio_descriptors = true;
	host->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;
	host->bits_per_word_mask = SPI_BPW_RANGE_MASK(8, 16);
	host->dev.of_node = pdev->dev.of_node;
	host->bus_num = pdev->id;
	host->num_chipselect = 4;
	host->setup = atmel_spi_setup;
	host->flags = (SPI_CONTROLLER_MUST_RX | SPI_CONTROLLER_MUST_TX |
			SPI_CONTROLLER_GPIO_SS);
	host->transfer_one = atmel_spi_one_transfer;
	host->set_cs = atmel_spi_set_cs;
	host->cleanup = atmel_spi_cleanup;
	host->auto_runtime_pm = true;
	host->max_dma_len = SPI_MAX_DMA_XFER;
	host->can_dma = atmel_spi_can_dma;
	platform_set_drvdata(pdev, host);

	as = spi_controller_get_devdata(host);

	spin_lock_init(&as->lock);

	as->pdev = pdev;
	as->regs = devm_platform_get_and_ioremap_resource(pdev, 0, &regs);
	if (IS_ERR(as->regs)) {
		ret = PTR_ERR(as->regs);
		goto out_unmap_regs;
	}
	as->phybase = regs->start;
	as->irq = irq;
	as->clk = clk;

	init_completion(&as->xfer_completion);

	atmel_get_caps(as);

	as->use_dma = false;
	as->use_pdc = false;
	if (as->caps.has_dma_support) {
		ret = atmel_spi_configure_dma(host, as);
		if (ret == 0) {
			as->use_dma = true;
		} else if (ret == -EPROBE_DEFER) {
			goto out_unmap_regs;
		}
	} else if (as->caps.has_pdc_support) {
		as->use_pdc = true;
	}

	if (IS_ENABLED(CONFIG_SOC_SAM_V4_V5)) {
		as->addr_rx_bbuf = dma_alloc_coherent(&pdev->dev,
						      SPI_MAX_DMA_XFER,
						      &as->dma_addr_rx_bbuf,
						      GFP_KERNEL | GFP_DMA);
		if (!as->addr_rx_bbuf) {
			as->use_dma = false;
		} else {
			as->addr_tx_bbuf = dma_alloc_coherent(&pdev->dev,
					SPI_MAX_DMA_XFER,
					&as->dma_addr_tx_bbuf,
					GFP_KERNEL | GFP_DMA);
			if (!as->addr_tx_bbuf) {
				as->use_dma = false;
				dma_free_coherent(&pdev->dev, SPI_MAX_DMA_XFER,
						  as->addr_rx_bbuf,
						  as->dma_addr_rx_bbuf);
			}
		}
		if (!as->use_dma)
			dev_info(host->dev.parent,
				 "  can not allocate dma coherent memory\n");
	}

	if (as->caps.has_dma_support && !as->use_dma)
		dev_info(&pdev->dev, "Atmel SPI Controller using PIO only\n");

	if (as->use_pdc) {
		ret = devm_request_irq(&pdev->dev, irq, atmel_spi_pdc_interrupt,
					0, dev_name(&pdev->dev), host);
	} else {
		ret = devm_request_irq(&pdev->dev, irq, atmel_spi_pio_interrupt,
					0, dev_name(&pdev->dev), host);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		goto out_unmap_regs;

	/* Initialize the hardware */
<<<<<<< HEAD
	clk_enable(clk);
	spi_writel(as, CR, SPI_BIT(SWRST));
	spi_writel(as, CR, SPI_BIT(SWRST)); /* AT91SAM9263 Rev B workaround */
	spi_writel(as, MR, SPI_BIT(MSTR) | SPI_BIT(MODFDIS));
	spi_writel(as, PTCR, SPI_BIT(RXTDIS) | SPI_BIT(TXTDIS));
	spi_writel(as, CR, SPI_BIT(SPIEN));

	/* go! */
	dev_info(&pdev->dev, "Atmel SPI Controller at 0x%08lx (irq %d)\n",
			(unsigned long)regs->start, irq);

	ret = spi_register_master(master);
	if (ret)
		goto out_reset_hw;

	return 0;

out_reset_hw:
	spi_writel(as, CR, SPI_BIT(SWRST));
	spi_writel(as, CR, SPI_BIT(SWRST)); /* AT91SAM9263 Rev B workaround */
	clk_disable(clk);
	free_irq(irq, master);
out_unmap_regs:
	iounmap(as->regs);
out_free_buffer:
	dma_free_coherent(&pdev->dev, BUFFER_SIZE, as->buffer,
			as->buffer_dma);
out_free:
	clk_put(clk);
	spi_master_put(master);
	return ret;
}

static int __devexit atmel_spi_remove(struct platform_device *pdev)
{
	struct spi_master	*master = platform_get_drvdata(pdev);
	struct atmel_spi	*as = spi_master_get_devdata(master);
	struct spi_message	*msg;

	/* reset the hardware and block queue progress */
	spin_lock_irq(&as->lock);
	as->stopping = 1;
=======
	ret = clk_prepare_enable(clk);
	if (ret)
		goto out_free_irq;

	as->spi_clk = clk_get_rate(clk);

	as->fifo_size = 0;
	if (!of_property_read_u32(pdev->dev.of_node, "atmel,fifo-size",
				  &as->fifo_size)) {
		dev_info(&pdev->dev, "Using FIFO (%u data)\n", as->fifo_size);
	}

	atmel_spi_init(as);

	pm_runtime_set_autosuspend_delay(&pdev->dev, AUTOSUSPEND_TIMEOUT);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_set_active(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

	ret = devm_spi_register_controller(&pdev->dev, host);
	if (ret)
		goto out_free_dma;

	/* go! */
	dev_info(&pdev->dev, "Atmel SPI Controller version 0x%x at 0x%08lx (irq %d)\n",
			atmel_get_version(as), (unsigned long)regs->start,
			irq);

	return 0;

out_free_dma:
	pm_runtime_disable(&pdev->dev);
	pm_runtime_set_suspended(&pdev->dev);

	if (as->use_dma)
		atmel_spi_release_dma(host);

	spi_writel(as, CR, SPI_BIT(SWRST));
	spi_writel(as, CR, SPI_BIT(SWRST)); /* AT91SAM9263 Rev B workaround */
	clk_disable_unprepare(clk);
out_free_irq:
out_unmap_regs:
	spi_controller_put(host);
	return ret;
}

static void atmel_spi_remove(struct platform_device *pdev)
{
	struct spi_controller	*host = platform_get_drvdata(pdev);
	struct atmel_spi	*as = spi_controller_get_devdata(host);

	pm_runtime_get_sync(&pdev->dev);

	/* reset the hardware and block queue progress */
	if (as->use_dma) {
		atmel_spi_stop_dma(host);
		atmel_spi_release_dma(host);
		if (IS_ENABLED(CONFIG_SOC_SAM_V4_V5)) {
			dma_free_coherent(&pdev->dev, SPI_MAX_DMA_XFER,
					  as->addr_tx_bbuf,
					  as->dma_addr_tx_bbuf);
			dma_free_coherent(&pdev->dev, SPI_MAX_DMA_XFER,
					  as->addr_rx_bbuf,
					  as->dma_addr_rx_bbuf);
		}
	}

	spin_lock_irq(&as->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spi_writel(as, CR, SPI_BIT(SWRST));
	spi_writel(as, CR, SPI_BIT(SWRST)); /* AT91SAM9263 Rev B workaround */
	spi_readl(as, SR);
	spin_unlock_irq(&as->lock);

<<<<<<< HEAD
	/* Terminate remaining queued transfers */
	list_for_each_entry(msg, &as->queue, queue) {
		/* REVISIT unmapping the dma is a NOP on ARM and AVR32
		 * but we shouldn't depend on that...
		 */
		msg->status = -ESHUTDOWN;
		msg->complete(msg->context);
	}

	dma_free_coherent(&pdev->dev, BUFFER_SIZE, as->buffer,
			as->buffer_dma);

	clk_disable(as->clk);
	clk_put(as->clk);
	free_irq(as->irq, master);
	iounmap(as->regs);

	spi_unregister_master(master);

	return 0;
}

#ifdef	CONFIG_PM

static int atmel_spi_suspend(struct platform_device *pdev, pm_message_t mesg)
{
	struct spi_master	*master = platform_get_drvdata(pdev);
	struct atmel_spi	*as = spi_master_get_devdata(master);

	clk_disable(as->clk);
	return 0;
}

static int atmel_spi_resume(struct platform_device *pdev)
{
	struct spi_master	*master = platform_get_drvdata(pdev);
	struct atmel_spi	*as = spi_master_get_devdata(master);

	clk_enable(as->clk);
	return 0;
}

#else
#define	atmel_spi_suspend	NULL
#define	atmel_spi_resume	NULL
#endif

=======
	clk_disable_unprepare(as->clk);

	pm_runtime_put_noidle(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
}

static int atmel_spi_runtime_suspend(struct device *dev)
{
	struct spi_controller *host = dev_get_drvdata(dev);
	struct atmel_spi *as = spi_controller_get_devdata(host);

	clk_disable_unprepare(as->clk);
	pinctrl_pm_select_sleep_state(dev);

	return 0;
}

static int atmel_spi_runtime_resume(struct device *dev)
{
	struct spi_controller *host = dev_get_drvdata(dev);
	struct atmel_spi *as = spi_controller_get_devdata(host);

	pinctrl_pm_select_default_state(dev);

	return clk_prepare_enable(as->clk);
}

static int atmel_spi_suspend(struct device *dev)
{
	struct spi_controller *host = dev_get_drvdata(dev);
	int ret;

	/* Stop the queue running */
	ret = spi_controller_suspend(host);
	if (ret)
		return ret;

	if (!pm_runtime_suspended(dev))
		atmel_spi_runtime_suspend(dev);

	return 0;
}

static int atmel_spi_resume(struct device *dev)
{
	struct spi_controller *host = dev_get_drvdata(dev);
	struct atmel_spi *as = spi_controller_get_devdata(host);
	int ret;

	ret = clk_prepare_enable(as->clk);
	if (ret)
		return ret;

	atmel_spi_init(as);

	clk_disable_unprepare(as->clk);

	if (!pm_runtime_suspended(dev)) {
		ret = atmel_spi_runtime_resume(dev);
		if (ret)
			return ret;
	}

	/* Start the queue running */
	return spi_controller_resume(host);
}

static const struct dev_pm_ops atmel_spi_pm_ops = {
	SYSTEM_SLEEP_PM_OPS(atmel_spi_suspend, atmel_spi_resume)
	RUNTIME_PM_OPS(atmel_spi_runtime_suspend,
		       atmel_spi_runtime_resume, NULL)
};

static const struct of_device_id atmel_spi_dt_ids[] = {
	{ .compatible = "atmel,at91rm9200-spi" },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, atmel_spi_dt_ids);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct platform_driver atmel_spi_driver = {
	.driver		= {
		.name	= "atmel_spi",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.suspend	= atmel_spi_suspend,
	.resume		= atmel_spi_resume,
	.probe		= atmel_spi_probe,
	.remove		= __exit_p(atmel_spi_remove),
=======
		.pm	= pm_ptr(&atmel_spi_pm_ops),
		.of_match_table	= atmel_spi_dt_ids,
	},
	.probe		= atmel_spi_probe,
	.remove_new	= atmel_spi_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
module_platform_driver(atmel_spi_driver);

MODULE_DESCRIPTION("Atmel AT32/AT91 SPI Controller driver");
MODULE_AUTHOR("Haavard Skinnemoen (Atmel)");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:atmel_spi");
