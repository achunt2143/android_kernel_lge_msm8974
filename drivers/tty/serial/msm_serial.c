<<<<<<< HEAD
/*
 * drivers/serial/msm_serial.c - driver for msm7k serial device and console
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2009-2013, The Linux Foundation. All rights reserved.
 * Author: Robert Love <rlove@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#if defined(CONFIG_SERIAL_MSM_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
# define SUPPORT_SYSRQ
#endif

#include <linux/hrtimer.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/init.h>
#include <linux/delay.h>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for msm7k serial device and console
 *
 * Copyright (C) 2007 Google, Inc.
 * Author: Robert Love <rlove@google.com>
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 */

#include <linux/kernel.h>
#include <linux/atomic.h>
#include <linux/dma/qcom_adm.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/init.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
<<<<<<< HEAD
#include <linux/serial.h>
#include <linux/nmi.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <mach/msm_serial_pdata.h>
#include "msm_serial.h"


#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
enum msm_clk_states_e {
	MSM_CLK_PORT_OFF,     /* uart port not in use */
	MSM_CLK_OFF,          /* clock enabled */
	MSM_CLK_REQUEST_OFF,  /* disable after TX flushed */
	MSM_CLK_ON,           /* clock disabled */
};
#endif

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
/* optional low power wakeup, typically on a GPIO RX irq */
struct msm_wakeup {
	int irq;  /* < 0 indicates low power wakeup disabled */
	unsigned char ignore;  /* bool */

	/* bool: inject char into rx tty on wakeup */
	unsigned char inject_rx;
	char rx_to_inject;
};
#endif
=======
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/pm_opp.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/wait.h>

#define MSM_UART_MR1			0x0000

#define MSM_UART_MR1_AUTO_RFR_LEVEL0	0x3F
#define MSM_UART_MR1_AUTO_RFR_LEVEL1	0x3FF00
#define MSM_UART_DM_MR1_AUTO_RFR_LEVEL1	0xFFFFFF00
#define MSM_UART_MR1_RX_RDY_CTL		BIT(7)
#define MSM_UART_MR1_CTS_CTL		BIT(6)

#define MSM_UART_MR2			0x0004
#define MSM_UART_MR2_ERROR_MODE		BIT(6)
#define MSM_UART_MR2_BITS_PER_CHAR	0x30
#define MSM_UART_MR2_BITS_PER_CHAR_5	(0x0 << 4)
#define MSM_UART_MR2_BITS_PER_CHAR_6	(0x1 << 4)
#define MSM_UART_MR2_BITS_PER_CHAR_7	(0x2 << 4)
#define MSM_UART_MR2_BITS_PER_CHAR_8	(0x3 << 4)
#define MSM_UART_MR2_STOP_BIT_LEN_ONE	(0x1 << 2)
#define MSM_UART_MR2_STOP_BIT_LEN_TWO	(0x3 << 2)
#define MSM_UART_MR2_PARITY_MODE_NONE	0x0
#define MSM_UART_MR2_PARITY_MODE_ODD	0x1
#define MSM_UART_MR2_PARITY_MODE_EVEN	0x2
#define MSM_UART_MR2_PARITY_MODE_SPACE	0x3
#define MSM_UART_MR2_PARITY_MODE	0x3

#define MSM_UART_CSR			0x0008

#define MSM_UART_TF			0x000C
#define UARTDM_TF			0x0070

#define MSM_UART_CR				0x0010
#define MSM_UART_CR_CMD_NULL			(0 << 4)
#define MSM_UART_CR_CMD_RESET_RX		(1 << 4)
#define MSM_UART_CR_CMD_RESET_TX		(2 << 4)
#define MSM_UART_CR_CMD_RESET_ERR		(3 << 4)
#define MSM_UART_CR_CMD_RESET_BREAK_INT		(4 << 4)
#define MSM_UART_CR_CMD_START_BREAK		(5 << 4)
#define MSM_UART_CR_CMD_STOP_BREAK		(6 << 4)
#define MSM_UART_CR_CMD_RESET_CTS		(7 << 4)
#define MSM_UART_CR_CMD_RESET_STALE_INT		(8 << 4)
#define MSM_UART_CR_CMD_PACKET_MODE		(9 << 4)
#define MSM_UART_CR_CMD_MODE_RESET		(12 << 4)
#define MSM_UART_CR_CMD_SET_RFR			(13 << 4)
#define MSM_UART_CR_CMD_RESET_RFR		(14 << 4)
#define MSM_UART_CR_CMD_PROTECTION_EN		(16 << 4)
#define MSM_UART_CR_CMD_STALE_EVENT_DISABLE	(6 << 8)
#define MSM_UART_CR_CMD_STALE_EVENT_ENABLE	(80 << 4)
#define MSM_UART_CR_CMD_FORCE_STALE		(4 << 8)
#define MSM_UART_CR_CMD_RESET_TX_READY		(3 << 8)
#define MSM_UART_CR_TX_DISABLE			BIT(3)
#define MSM_UART_CR_TX_ENABLE			BIT(2)
#define MSM_UART_CR_RX_DISABLE			BIT(1)
#define MSM_UART_CR_RX_ENABLE			BIT(0)
#define MSM_UART_CR_CMD_RESET_RXBREAK_START	((1 << 11) | (2 << 4))

#define MSM_UART_IMR			0x0014
#define MSM_UART_IMR_TXLEV		BIT(0)
#define MSM_UART_IMR_RXSTALE		BIT(3)
#define MSM_UART_IMR_RXLEV		BIT(4)
#define MSM_UART_IMR_DELTA_CTS		BIT(5)
#define MSM_UART_IMR_CURRENT_CTS	BIT(6)
#define MSM_UART_IMR_RXBREAK_START	BIT(10)

#define MSM_UART_IPR_RXSTALE_LAST		0x20
#define MSM_UART_IPR_STALE_LSB			0x1F
#define MSM_UART_IPR_STALE_TIMEOUT_MSB		0x3FF80
#define MSM_UART_DM_IPR_STALE_TIMEOUT_MSB	0xFFFFFF80

#define MSM_UART_IPR			0x0018
#define MSM_UART_TFWR			0x001C
#define MSM_UART_RFWR			0x0020
#define MSM_UART_HCR			0x0024

#define MSM_UART_MREG			0x0028
#define MSM_UART_NREG			0x002C
#define MSM_UART_DREG			0x0030
#define MSM_UART_MNDREG			0x0034
#define MSM_UART_IRDA			0x0038
#define MSM_UART_MISR_MODE		0x0040
#define MSM_UART_MISR_RESET		0x0044
#define MSM_UART_MISR_EXPORT		0x0048
#define MSM_UART_MISR_VAL		0x004C
#define MSM_UART_TEST_CTRL		0x0050

#define MSM_UART_SR			0x0008
#define MSM_UART_SR_HUNT_CHAR		BIT(7)
#define MSM_UART_SR_RX_BREAK		BIT(6)
#define MSM_UART_SR_PAR_FRAME_ERR	BIT(5)
#define MSM_UART_SR_OVERRUN		BIT(4)
#define MSM_UART_SR_TX_EMPTY		BIT(3)
#define MSM_UART_SR_TX_READY		BIT(2)
#define MSM_UART_SR_RX_FULL		BIT(1)
#define MSM_UART_SR_RX_READY		BIT(0)

#define MSM_UART_RF			0x000C
#define UARTDM_RF			0x0070
#define MSM_UART_MISR			0x0010
#define MSM_UART_ISR			0x0014
#define MSM_UART_ISR_TX_READY		BIT(7)

#define UARTDM_RXFS			0x50
#define UARTDM_RXFS_BUF_SHIFT		0x7
#define UARTDM_RXFS_BUF_MASK		0x7

#define UARTDM_DMEN			0x3C
#define UARTDM_DMEN_RX_SC_ENABLE	BIT(5)
#define UARTDM_DMEN_TX_SC_ENABLE	BIT(4)

#define UARTDM_DMEN_TX_BAM_ENABLE	BIT(2)	/* UARTDM_1P4 */
#define UARTDM_DMEN_TX_DM_ENABLE	BIT(0)	/* < UARTDM_1P4 */

#define UARTDM_DMEN_RX_BAM_ENABLE	BIT(3)	/* UARTDM_1P4 */
#define UARTDM_DMEN_RX_DM_ENABLE	BIT(1)	/* < UARTDM_1P4 */

#define UARTDM_DMRX			0x34
#define UARTDM_NCF_TX			0x40
#define UARTDM_RX_TOTAL_SNAP		0x38

#define UARTDM_BURST_SIZE		16   /* in bytes */
#define UARTDM_TX_AIGN(x)		((x) & ~0x3) /* valid for > 1p3 */
#define UARTDM_TX_MAX			256   /* in bytes, valid for <= 1p3 */
#define UARTDM_RX_SIZE			(UART_XMIT_SIZE / 4)

enum {
	UARTDM_1P1 = 1,
	UARTDM_1P2,
	UARTDM_1P3,
	UARTDM_1P4,
};

struct msm_dma {
	struct dma_chan		*chan;
	enum dma_data_direction dir;
	dma_addr_t		phys;
	unsigned char		*virt;
	dma_cookie_t		cookie;
	u32			enable_bit;
	unsigned int		count;
	struct dma_async_tx_descriptor	*desc;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct msm_port {
	struct uart_port	uart;
	char			name[16];
	struct clk		*clk;
<<<<<<< HEAD
	unsigned int		imr;
#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	enum msm_clk_states_e	clk_state;
	struct hrtimer		clk_off_timer;
	ktime_t			clk_off_delay;
#endif
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	struct msm_wakeup wakeup;
#endif
	int			uim;
};

#define UART_TO_MSM(uart_port)	((struct msm_port *) uart_port)
#define is_console(port)	((port)->cons && \
				(port)->cons->index == (port)->line)


static inline void msm_write(struct uart_port *port, unsigned int val,
			     unsigned int off)
{
	__raw_writel(val, port->membase + off);
}

static inline unsigned int msm_read(struct uart_port *port, unsigned int off)
{
	return __raw_readl(port->membase + off);
}

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
static inline unsigned int use_low_power_wakeup(struct msm_port *msm_port)
{
	return (msm_port->wakeup.irq >= 0);
}
#endif

static void msm_stop_tx(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

	msm_port->imr &= ~UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);
=======
	struct clk		*pclk;
	unsigned int		imr;
	int			is_uartdm;
	unsigned int		old_snap_state;
	bool			break_detected;
	struct msm_dma		tx_dma;
	struct msm_dma		rx_dma;
};

static inline struct msm_port *to_msm_port(struct uart_port *up)
{
	return container_of(up, struct msm_port, uart);
}

static
void msm_write(struct uart_port *port, unsigned int val, unsigned int off)
{
	writel_relaxed(val, port->membase + off);
}

static
unsigned int msm_read(struct uart_port *port, unsigned int off)
{
	return readl_relaxed(port->membase + off);
}

/*
 * Setup the MND registers to use the TCXO clock.
 */
static void msm_serial_set_mnd_regs_tcxo(struct uart_port *port)
{
	msm_write(port, 0x06, MSM_UART_MREG);
	msm_write(port, 0xF1, MSM_UART_NREG);
	msm_write(port, 0x0F, MSM_UART_DREG);
	msm_write(port, 0x1A, MSM_UART_MNDREG);
	port->uartclk = 1843200;
}

/*
 * Setup the MND registers to use the TCXO clock divided by 4.
 */
static void msm_serial_set_mnd_regs_tcxoby4(struct uart_port *port)
{
	msm_write(port, 0x18, MSM_UART_MREG);
	msm_write(port, 0xF6, MSM_UART_NREG);
	msm_write(port, 0x0F, MSM_UART_DREG);
	msm_write(port, 0x0A, MSM_UART_MNDREG);
	port->uartclk = 1843200;
}

static void msm_serial_set_mnd_regs(struct uart_port *port)
{
	struct msm_port *msm_port = to_msm_port(port);

	/*
	 * These registers don't exist so we change the clk input rate
	 * on uartdm hardware instead
	 */
	if (msm_port->is_uartdm)
		return;

	if (port->uartclk == 19200000)
		msm_serial_set_mnd_regs_tcxo(port);
	else if (port->uartclk == 4800000)
		msm_serial_set_mnd_regs_tcxoby4(port);
}

static void msm_handle_tx(struct uart_port *port);
static void msm_start_rx_dma(struct msm_port *msm_port);

static void msm_stop_dma(struct uart_port *port, struct msm_dma *dma)
{
	struct device *dev = port->dev;
	unsigned int mapped;
	u32 val;

	mapped = dma->count;
	dma->count = 0;

	dmaengine_terminate_all(dma->chan);

	/*
	 * DMA Stall happens if enqueue and flush command happens concurrently.
	 * For example before changing the baud rate/protocol configuration and
	 * sending flush command to ADM, disable the channel of UARTDM.
	 * Note: should not reset the receiver here immediately as it is not
	 * suggested to do disable/reset or reset/disable at the same time.
	 */
	val = msm_read(port, UARTDM_DMEN);
	val &= ~dma->enable_bit;
	msm_write(port, val, UARTDM_DMEN);

	if (mapped)
		dma_unmap_single(dev, dma->phys, mapped, dma->dir);
}

static void msm_release_dma(struct msm_port *msm_port)
{
	struct msm_dma *dma;

	dma = &msm_port->tx_dma;
	if (dma->chan) {
		msm_stop_dma(&msm_port->uart, dma);
		dma_release_channel(dma->chan);
	}

	memset(dma, 0, sizeof(*dma));

	dma = &msm_port->rx_dma;
	if (dma->chan) {
		msm_stop_dma(&msm_port->uart, dma);
		dma_release_channel(dma->chan);
		kfree(dma->virt);
	}

	memset(dma, 0, sizeof(*dma));
}

static void msm_request_tx_dma(struct msm_port *msm_port, resource_size_t base)
{
	struct device *dev = msm_port->uart.dev;
	struct dma_slave_config conf;
	struct qcom_adm_peripheral_config periph_conf = {};
	struct msm_dma *dma;
	u32 crci = 0;
	int ret;

	dma = &msm_port->tx_dma;

	/* allocate DMA resources, if available */
	dma->chan = dma_request_chan(dev, "tx");
	if (IS_ERR(dma->chan))
		goto no_tx;

	of_property_read_u32(dev->of_node, "qcom,tx-crci", &crci);

	memset(&conf, 0, sizeof(conf));
	conf.direction = DMA_MEM_TO_DEV;
	conf.device_fc = true;
	conf.dst_addr = base + UARTDM_TF;
	conf.dst_maxburst = UARTDM_BURST_SIZE;
	if (crci) {
		conf.peripheral_config = &periph_conf;
		conf.peripheral_size = sizeof(periph_conf);
		periph_conf.crci = crci;
	}

	ret = dmaengine_slave_config(dma->chan, &conf);
	if (ret)
		goto rel_tx;

	dma->dir = DMA_TO_DEVICE;

	if (msm_port->is_uartdm < UARTDM_1P4)
		dma->enable_bit = UARTDM_DMEN_TX_DM_ENABLE;
	else
		dma->enable_bit = UARTDM_DMEN_TX_BAM_ENABLE;

	return;

rel_tx:
	dma_release_channel(dma->chan);
no_tx:
	memset(dma, 0, sizeof(*dma));
}

static void msm_request_rx_dma(struct msm_port *msm_port, resource_size_t base)
{
	struct device *dev = msm_port->uart.dev;
	struct dma_slave_config conf;
	struct qcom_adm_peripheral_config periph_conf = {};
	struct msm_dma *dma;
	u32 crci = 0;
	int ret;

	dma = &msm_port->rx_dma;

	/* allocate DMA resources, if available */
	dma->chan = dma_request_chan(dev, "rx");
	if (IS_ERR(dma->chan))
		goto no_rx;

	of_property_read_u32(dev->of_node, "qcom,rx-crci", &crci);

	dma->virt = kzalloc(UARTDM_RX_SIZE, GFP_KERNEL);
	if (!dma->virt)
		goto rel_rx;

	memset(&conf, 0, sizeof(conf));
	conf.direction = DMA_DEV_TO_MEM;
	conf.device_fc = true;
	conf.src_addr = base + UARTDM_RF;
	conf.src_maxburst = UARTDM_BURST_SIZE;
	if (crci) {
		conf.peripheral_config = &periph_conf;
		conf.peripheral_size = sizeof(periph_conf);
		periph_conf.crci = crci;
	}

	ret = dmaengine_slave_config(dma->chan, &conf);
	if (ret)
		goto err;

	dma->dir = DMA_FROM_DEVICE;

	if (msm_port->is_uartdm < UARTDM_1P4)
		dma->enable_bit = UARTDM_DMEN_RX_DM_ENABLE;
	else
		dma->enable_bit = UARTDM_DMEN_RX_BAM_ENABLE;

	return;
err:
	kfree(dma->virt);
rel_rx:
	dma_release_channel(dma->chan);
no_rx:
	memset(dma, 0, sizeof(*dma));
}

static inline void msm_wait_for_xmitr(struct uart_port *port)
{
	unsigned int timeout = 500000;

	while (!(msm_read(port, MSM_UART_SR) & MSM_UART_SR_TX_EMPTY)) {
		if (msm_read(port, MSM_UART_ISR) & MSM_UART_ISR_TX_READY)
			break;
		udelay(1);
		if (!timeout--)
			break;
	}
	msm_write(port, MSM_UART_CR_CMD_RESET_TX_READY, MSM_UART_CR);
}

static void msm_stop_tx(struct uart_port *port)
{
	struct msm_port *msm_port = to_msm_port(port);

	msm_port->imr &= ~MSM_UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, MSM_UART_IMR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void msm_start_tx(struct uart_port *port)
{
<<<<<<< HEAD
	struct msm_port *msm_port = UART_TO_MSM(port);

	msm_port->imr |= UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);
=======
	struct msm_port *msm_port = to_msm_port(port);
	struct msm_dma *dma = &msm_port->tx_dma;

	/* Already started in DMA mode */
	if (dma->count)
		return;

	msm_port->imr |= MSM_UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, MSM_UART_IMR);
}

static void msm_reset_dm_count(struct uart_port *port, int count)
{
	msm_wait_for_xmitr(port);
	msm_write(port, count, UARTDM_NCF_TX);
	msm_read(port, UARTDM_NCF_TX);
}

static void msm_complete_tx_dma(void *args)
{
	struct msm_port *msm_port = args;
	struct uart_port *port = &msm_port->uart;
	struct circ_buf *xmit = &port->state->xmit;
	struct msm_dma *dma = &msm_port->tx_dma;
	struct dma_tx_state state;
	unsigned long flags;
	unsigned int count;
	u32 val;

	uart_port_lock_irqsave(port, &flags);

	/* Already stopped */
	if (!dma->count)
		goto done;

	dmaengine_tx_status(dma->chan, dma->cookie, &state);

	dma_unmap_single(port->dev, dma->phys, dma->count, dma->dir);

	val = msm_read(port, UARTDM_DMEN);
	val &= ~dma->enable_bit;
	msm_write(port, val, UARTDM_DMEN);

	if (msm_port->is_uartdm > UARTDM_1P3) {
		msm_write(port, MSM_UART_CR_CMD_RESET_TX, MSM_UART_CR);
		msm_write(port, MSM_UART_CR_TX_ENABLE, MSM_UART_CR);
	}

	count = dma->count - state.residue;
	uart_xmit_advance(port, count);
	dma->count = 0;

	/* Restore "Tx FIFO below watermark" interrupt */
	msm_port->imr |= MSM_UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, MSM_UART_IMR);

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	msm_handle_tx(port);
done:
	uart_port_unlock_irqrestore(port, flags);
}

static int msm_handle_tx_dma(struct msm_port *msm_port, unsigned int count)
{
	struct circ_buf *xmit = &msm_port->uart.state->xmit;
	struct uart_port *port = &msm_port->uart;
	struct msm_dma *dma = &msm_port->tx_dma;
	void *cpu_addr;
	int ret;
	u32 val;

	cpu_addr = &xmit->buf[xmit->tail];

	dma->phys = dma_map_single(port->dev, cpu_addr, count, dma->dir);
	ret = dma_mapping_error(port->dev, dma->phys);
	if (ret)
		return ret;

	dma->desc = dmaengine_prep_slave_single(dma->chan, dma->phys,
						count, DMA_MEM_TO_DEV,
						DMA_PREP_INTERRUPT |
						DMA_PREP_FENCE);
	if (!dma->desc) {
		ret = -EIO;
		goto unmap;
	}

	dma->desc->callback = msm_complete_tx_dma;
	dma->desc->callback_param = msm_port;

	dma->cookie = dmaengine_submit(dma->desc);
	ret = dma_submit_error(dma->cookie);
	if (ret)
		goto unmap;

	/*
	 * Using DMA complete for Tx FIFO reload, no need for
	 * "Tx FIFO below watermark" one, disable it
	 */
	msm_port->imr &= ~MSM_UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, MSM_UART_IMR);

	dma->count = count;

	val = msm_read(port, UARTDM_DMEN);
	val |= dma->enable_bit;

	if (msm_port->is_uartdm < UARTDM_1P4)
		msm_write(port, val, UARTDM_DMEN);

	msm_reset_dm_count(port, count);

	if (msm_port->is_uartdm > UARTDM_1P3)
		msm_write(port, val, UARTDM_DMEN);

	dma_async_issue_pending(dma->chan);
	return 0;
unmap:
	dma_unmap_single(port->dev, dma->phys, count, dma->dir);
	return ret;
}

static void msm_complete_rx_dma(void *args)
{
	struct msm_port *msm_port = args;
	struct uart_port *port = &msm_port->uart;
	struct tty_port *tport = &port->state->port;
	struct msm_dma *dma = &msm_port->rx_dma;
	int count = 0, i, sysrq;
	unsigned long flags;
	u32 val;

	uart_port_lock_irqsave(port, &flags);

	/* Already stopped */
	if (!dma->count)
		goto done;

	val = msm_read(port, UARTDM_DMEN);
	val &= ~dma->enable_bit;
	msm_write(port, val, UARTDM_DMEN);

	if (msm_read(port, MSM_UART_SR) & MSM_UART_SR_OVERRUN) {
		port->icount.overrun++;
		tty_insert_flip_char(tport, 0, TTY_OVERRUN);
		msm_write(port, MSM_UART_CR_CMD_RESET_ERR, MSM_UART_CR);
	}

	count = msm_read(port, UARTDM_RX_TOTAL_SNAP);

	port->icount.rx += count;

	dma->count = 0;

	dma_unmap_single(port->dev, dma->phys, UARTDM_RX_SIZE, dma->dir);

	for (i = 0; i < count; i++) {
		char flag = TTY_NORMAL;

		if (msm_port->break_detected && dma->virt[i] == 0) {
			port->icount.brk++;
			flag = TTY_BREAK;
			msm_port->break_detected = false;
			if (uart_handle_break(port))
				continue;
		}

		if (!(port->read_status_mask & MSM_UART_SR_RX_BREAK))
			flag = TTY_NORMAL;

		sysrq = uart_prepare_sysrq_char(port, dma->virt[i]);
		if (!sysrq)
			tty_insert_flip_char(tport, dma->virt[i], flag);
	}

	msm_start_rx_dma(msm_port);
done:
	uart_unlock_and_check_sysrq_irqrestore(port, flags);

	if (count)
		tty_flip_buffer_push(tport);
}

static void msm_start_rx_dma(struct msm_port *msm_port)
{
	struct msm_dma *dma = &msm_port->rx_dma;
	struct uart_port *uart = &msm_port->uart;
	u32 val;
	int ret;

	if (IS_ENABLED(CONFIG_CONSOLE_POLL))
		return;

	if (!dma->chan)
		return;

	dma->phys = dma_map_single(uart->dev, dma->virt,
				   UARTDM_RX_SIZE, dma->dir);
	ret = dma_mapping_error(uart->dev, dma->phys);
	if (ret)
		goto sw_mode;

	dma->desc = dmaengine_prep_slave_single(dma->chan, dma->phys,
						UARTDM_RX_SIZE, DMA_DEV_TO_MEM,
						DMA_PREP_INTERRUPT);
	if (!dma->desc)
		goto unmap;

	dma->desc->callback = msm_complete_rx_dma;
	dma->desc->callback_param = msm_port;

	dma->cookie = dmaengine_submit(dma->desc);
	ret = dma_submit_error(dma->cookie);
	if (ret)
		goto unmap;
	/*
	 * Using DMA for FIFO off-load, no need for "Rx FIFO over
	 * watermark" or "stale" interrupts, disable them
	 */
	msm_port->imr &= ~(MSM_UART_IMR_RXLEV | MSM_UART_IMR_RXSTALE);

	/*
	 * Well, when DMA is ADM3 engine(implied by <= UARTDM v1.3),
	 * we need RXSTALE to flush input DMA fifo to memory
	 */
	if (msm_port->is_uartdm < UARTDM_1P4)
		msm_port->imr |= MSM_UART_IMR_RXSTALE;

	msm_write(uart, msm_port->imr, MSM_UART_IMR);

	dma->count = UARTDM_RX_SIZE;

	dma_async_issue_pending(dma->chan);

	msm_write(uart, MSM_UART_CR_CMD_RESET_STALE_INT, MSM_UART_CR);
	msm_write(uart, MSM_UART_CR_CMD_STALE_EVENT_ENABLE, MSM_UART_CR);

	val = msm_read(uart, UARTDM_DMEN);
	val |= dma->enable_bit;

	if (msm_port->is_uartdm < UARTDM_1P4)
		msm_write(uart, val, UARTDM_DMEN);

	msm_write(uart, UARTDM_RX_SIZE, UARTDM_DMRX);

	if (msm_port->is_uartdm > UARTDM_1P3)
		msm_write(uart, val, UARTDM_DMEN);

	return;
unmap:
	dma_unmap_single(uart->dev, dma->phys, UARTDM_RX_SIZE, dma->dir);

sw_mode:
	/*
	 * Switch from DMA to SW/FIFO mode. After clearing Rx BAM (UARTDM_DMEN),
	 * receiver must be reset.
	 */
	msm_write(uart, MSM_UART_CR_CMD_RESET_RX, MSM_UART_CR);
	msm_write(uart, MSM_UART_CR_RX_ENABLE, MSM_UART_CR);

	msm_write(uart, MSM_UART_CR_CMD_RESET_STALE_INT, MSM_UART_CR);
	msm_write(uart, 0xFFFFFF, UARTDM_DMRX);
	msm_write(uart, MSM_UART_CR_CMD_STALE_EVENT_ENABLE, MSM_UART_CR);

	/* Re-enable RX interrupts */
	msm_port->imr |= MSM_UART_IMR_RXLEV | MSM_UART_IMR_RXSTALE;
	msm_write(uart, msm_port->imr, MSM_UART_IMR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void msm_stop_rx(struct uart_port *port)
{
<<<<<<< HEAD
	struct msm_port *msm_port = UART_TO_MSM(port);

	msm_port->imr &= ~(UART_IMR_RXLEV | UART_IMR_RXSTALE);
	msm_write(port, msm_port->imr, UART_IMR);
=======
	struct msm_port *msm_port = to_msm_port(port);
	struct msm_dma *dma = &msm_port->rx_dma;

	msm_port->imr &= ~(MSM_UART_IMR_RXLEV | MSM_UART_IMR_RXSTALE);
	msm_write(port, msm_port->imr, MSM_UART_IMR);

	if (dma->chan)
		msm_stop_dma(port, dma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void msm_enable_ms(struct uart_port *port)
{
<<<<<<< HEAD
	struct msm_port *msm_port = UART_TO_MSM(port);

	msm_port->imr |= UART_IMR_DELTA_CTS;
	msm_write(port, msm_port->imr, UART_IMR);
}

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
/* turn clock off if TX buffer is empty, otherwise reschedule */
static enum hrtimer_restart msm_serial_clock_off(struct hrtimer *timer) {
	struct msm_port *msm_port = container_of(timer, struct msm_port,
						 clk_off_timer);
	struct uart_port *port = &msm_port->uart;
	struct circ_buf *xmit = &port->state->xmit;
	unsigned long flags;
	int ret = HRTIMER_NORESTART;

	spin_lock_irqsave(&port->lock, flags);

	if (msm_port->clk_state == MSM_CLK_REQUEST_OFF) {
		if (uart_circ_empty(xmit)) {
			struct msm_port *msm_port = UART_TO_MSM(port);
			clk_disable(msm_port->clk);
			msm_port->clk_state = MSM_CLK_OFF;
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
			if (use_low_power_wakeup(msm_port)) {
				msm_port->wakeup.ignore = 1;
				enable_irq(msm_port->wakeup.irq);
			}
#endif
		} else {
			hrtimer_forward_now(timer, msm_port->clk_off_delay);
			ret = HRTIMER_RESTART;
		}
	}

	spin_unlock_irqrestore(&port->lock, flags);

	return HRTIMER_NORESTART;
}

/* request to turn off uart clock once pending TX is flushed */
void msm_serial_clock_request_off(struct uart_port *port) {
	unsigned long flags;
	struct msm_port *msm_port = UART_TO_MSM(port);

	spin_lock_irqsave(&port->lock, flags);
	if (msm_port->clk_state == MSM_CLK_ON) {
		msm_port->clk_state = MSM_CLK_REQUEST_OFF;
		/* turn off TX later. unfortunately not all msm uart's have a
		 * TXDONE available, and TXLEV does not wait until completely
		 * flushed, so a timer is our only option
		 */
		hrtimer_start(&msm_port->clk_off_timer,
			      msm_port->clk_off_delay, HRTIMER_MODE_REL);
	}
	spin_unlock_irqrestore(&port->lock, flags);
}

/* request to immediately turn on uart clock.
 * ignored if there is a pending off request, unless force = 1.
 */
void msm_serial_clock_on(struct uart_port *port, int force) {
	unsigned long flags;
	struct msm_port *msm_port = UART_TO_MSM(port);

	spin_lock_irqsave(&port->lock, flags);

	switch (msm_port->clk_state) {
	case MSM_CLK_OFF:
		clk_enable(msm_port->clk);
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
		if (use_low_power_wakeup(msm_port))
			disable_irq(msm_port->wakeup.irq);
#endif
		force = 1;
	case MSM_CLK_REQUEST_OFF:
		if (force) {
			hrtimer_try_to_cancel(&msm_port->clk_off_timer);
			msm_port->clk_state = MSM_CLK_ON;
		}
		break;
	case MSM_CLK_ON: break;
	case MSM_CLK_PORT_OFF: break;
	}

	spin_unlock_irqrestore(&port->lock, flags);
}
#endif

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
static irqreturn_t msm_rx_irq(int irq, void *dev_id)
{
	unsigned long flags;
	struct uart_port *port = dev_id;
	struct msm_port *msm_port = UART_TO_MSM(port);
	int inject_wakeup = 0;

	spin_lock_irqsave(&port->lock, flags);

	if (msm_port->clk_state == MSM_CLK_OFF) {
		/* ignore the first irq - it is a pending irq that occured
		 * before enable_irq() */
		if (msm_port->wakeup.ignore)
			msm_port->wakeup.ignore = 0;
		else
			inject_wakeup = 1;
	}

	msm_serial_clock_on(port, 0);

	/* we missed an rx while asleep - it must be a wakeup indicator
	 */
	if (inject_wakeup) {
		struct tty_struct *tty = port->state->port.tty;
		tty_insert_flip_char(tty, WAKE_UP_IND, TTY_NORMAL);
		tty_flip_buffer_push(tty);
	}

	spin_unlock_irqrestore(&port->lock, flags);
	return IRQ_HANDLED;
}
#endif

static void handle_rx(struct uart_port *port)
{
	struct tty_struct *tty = port->state->port.tty;
=======
	struct msm_port *msm_port = to_msm_port(port);

	msm_port->imr |= MSM_UART_IMR_DELTA_CTS;
	msm_write(port, msm_port->imr, MSM_UART_IMR);
}

static void msm_handle_rx_dm(struct uart_port *port, unsigned int misr)
	__must_hold(&port->lock)
{
	struct tty_port *tport = &port->state->port;
	unsigned int sr;
	int count = 0;
	struct msm_port *msm_port = to_msm_port(port);

	if ((msm_read(port, MSM_UART_SR) & MSM_UART_SR_OVERRUN)) {
		port->icount.overrun++;
		tty_insert_flip_char(tport, 0, TTY_OVERRUN);
		msm_write(port, MSM_UART_CR_CMD_RESET_ERR, MSM_UART_CR);
	}

	if (misr & MSM_UART_IMR_RXSTALE) {
		count = msm_read(port, UARTDM_RX_TOTAL_SNAP) -
			msm_port->old_snap_state;
		msm_port->old_snap_state = 0;
	} else {
		count = 4 * (msm_read(port, MSM_UART_RFWR));
		msm_port->old_snap_state += count;
	}

	/* TODO: Precise error reporting */

	port->icount.rx += count;

	while (count > 0) {
		unsigned char buf[4];
		int sysrq, r_count, i;

		sr = msm_read(port, MSM_UART_SR);
		if ((sr & MSM_UART_SR_RX_READY) == 0) {
			msm_port->old_snap_state -= count;
			break;
		}

		ioread32_rep(port->membase + UARTDM_RF, buf, 1);
		r_count = min_t(int, count, sizeof(buf));

		for (i = 0; i < r_count; i++) {
			char flag = TTY_NORMAL;

			if (msm_port->break_detected && buf[i] == 0) {
				port->icount.brk++;
				flag = TTY_BREAK;
				msm_port->break_detected = false;
				if (uart_handle_break(port))
					continue;
			}

			if (!(port->read_status_mask & MSM_UART_SR_RX_BREAK))
				flag = TTY_NORMAL;

			sysrq = uart_prepare_sysrq_char(port, buf[i]);
			if (!sysrq)
				tty_insert_flip_char(tport, buf[i], flag);
		}
		count -= r_count;
	}

	tty_flip_buffer_push(tport);

	if (misr & (MSM_UART_IMR_RXSTALE))
		msm_write(port, MSM_UART_CR_CMD_RESET_STALE_INT, MSM_UART_CR);
	msm_write(port, 0xFFFFFF, UARTDM_DMRX);
	msm_write(port, MSM_UART_CR_CMD_STALE_EVENT_ENABLE, MSM_UART_CR);

	/* Try to use DMA */
	msm_start_rx_dma(msm_port);
}

static void msm_handle_rx(struct uart_port *port)
	__must_hold(&port->lock)
{
	struct tty_port *tport = &port->state->port;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int sr;

	/*
	 * Handle overrun. My understanding of the hardware is that overrun
	 * is not tied to the RX buffer, so we handle the case out of band.
	 */
<<<<<<< HEAD
	if ((msm_read(port, UART_SR) & UART_SR_OVERRUN)) {
		port->icount.overrun++;
		tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	}

	/* and now the main RX loop */
	while ((sr = msm_read(port, UART_SR)) & UART_SR_RX_READY) {
		unsigned int c;
		char flag = TTY_NORMAL;

		c = msm_read(port, UART_RF);

		if (sr & UART_SR_RX_BREAK) {
			port->icount.brk++;
			if (uart_handle_break(port))
				continue;
		} else if (sr & UART_SR_PAR_FRAME_ERR) {
=======
	if ((msm_read(port, MSM_UART_SR) & MSM_UART_SR_OVERRUN)) {
		port->icount.overrun++;
		tty_insert_flip_char(tport, 0, TTY_OVERRUN);
		msm_write(port, MSM_UART_CR_CMD_RESET_ERR, MSM_UART_CR);
	}

	/* and now the main RX loop */
	while ((sr = msm_read(port, MSM_UART_SR)) & MSM_UART_SR_RX_READY) {
		unsigned int c;
		char flag = TTY_NORMAL;
		int sysrq;

		c = msm_read(port, MSM_UART_RF);

		if (sr & MSM_UART_SR_RX_BREAK) {
			port->icount.brk++;
			if (uart_handle_break(port))
				continue;
		} else if (sr & MSM_UART_SR_PAR_FRAME_ERR) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			port->icount.frame++;
		} else {
			port->icount.rx++;
		}

<<<<<<< HEAD
		/* Mask conditions we're ignorning. */
		sr &= port->read_status_mask;

		if (sr & UART_SR_RX_BREAK) {
			flag = TTY_BREAK;
		} else if (sr & UART_SR_PAR_FRAME_ERR) {
			flag = TTY_FRAME;
		}

		if (!uart_handle_sysrq_char(port, c))
			tty_insert_flip_char(tty, c, flag);
	}

	tty_flip_buffer_push(tty);
}

static void handle_tx(struct uart_port *port)
{
	struct circ_buf *xmit = &port->state->xmit;
	struct msm_port *msm_port = UART_TO_MSM(port);
	int sent_tx;

	if (port->x_char) {
		msm_write(port, port->x_char, UART_TF);
		port->icount.tx++;
		port->x_char = 0;
	}

	while (msm_read(port, UART_SR) & UART_SR_TX_READY) {
		if (uart_circ_empty(xmit)) {
			/* disable tx interrupts */
			msm_port->imr &= ~UART_IMR_TXLEV;
			msm_write(port, msm_port->imr, UART_IMR);
			break;
		}

		msm_write(port, xmit->buf[xmit->tail], UART_TF);

		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
		sent_tx = 1;
	}

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	if (sent_tx && msm_port->clk_state == MSM_CLK_REQUEST_OFF)
		/* new TX - restart the timer */
		if (hrtimer_try_to_cancel(&msm_port->clk_off_timer) == 1)
			hrtimer_start(&msm_port->clk_off_timer,
				msm_port->clk_off_delay, HRTIMER_MODE_REL);
#endif
=======
		/* Mask conditions we're ignoring. */
		sr &= port->read_status_mask;

		if (sr & MSM_UART_SR_RX_BREAK)
			flag = TTY_BREAK;
		else if (sr & MSM_UART_SR_PAR_FRAME_ERR)
			flag = TTY_FRAME;

		sysrq = uart_prepare_sysrq_char(port, c);
		if (!sysrq)
			tty_insert_flip_char(tport, c, flag);
	}

	tty_flip_buffer_push(tport);
}

static void msm_handle_tx_pio(struct uart_port *port, unsigned int tx_count)
{
	struct circ_buf *xmit = &port->state->xmit;
	struct msm_port *msm_port = to_msm_port(port);
	unsigned int num_chars;
	unsigned int tf_pointer = 0;
	void __iomem *tf;

	if (msm_port->is_uartdm)
		tf = port->membase + UARTDM_TF;
	else
		tf = port->membase + MSM_UART_TF;

	if (tx_count && msm_port->is_uartdm)
		msm_reset_dm_count(port, tx_count);

	while (tf_pointer < tx_count) {
		int i;
		char buf[4] = { 0 };

		if (!(msm_read(port, MSM_UART_SR) & MSM_UART_SR_TX_READY))
			break;

		if (msm_port->is_uartdm)
			num_chars = min(tx_count - tf_pointer,
					(unsigned int)sizeof(buf));
		else
			num_chars = 1;

		for (i = 0; i < num_chars; i++)
			buf[i] = xmit->buf[xmit->tail + i];

		iowrite32_rep(tf, buf, 1);
		uart_xmit_advance(port, num_chars);
		tf_pointer += num_chars;
	}

	/* disable tx interrupts if nothing more to send */
	if (uart_circ_empty(xmit))
		msm_stop_tx(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);
}

<<<<<<< HEAD
static void handle_delta_cts(struct uart_port *port)
{
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
=======
static void msm_handle_tx(struct uart_port *port)
{
	struct msm_port *msm_port = to_msm_port(port);
	struct circ_buf *xmit = &msm_port->uart.state->xmit;
	struct msm_dma *dma = &msm_port->tx_dma;
	unsigned int pio_count, dma_count, dma_min;
	char buf[4] = { 0 };
	void __iomem *tf;
	int err = 0;

	if (port->x_char) {
		if (msm_port->is_uartdm)
			tf = port->membase + UARTDM_TF;
		else
			tf = port->membase + MSM_UART_TF;

		buf[0] = port->x_char;

		if (msm_port->is_uartdm)
			msm_reset_dm_count(port, 1);

		iowrite32_rep(tf, buf, 1);
		port->icount.tx++;
		port->x_char = 0;
		return;
	}

	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		msm_stop_tx(port);
		return;
	}

	pio_count = CIRC_CNT_TO_END(xmit->head, xmit->tail, UART_XMIT_SIZE);
	dma_count = CIRC_CNT_TO_END(xmit->head, xmit->tail, UART_XMIT_SIZE);

	dma_min = 1;	/* Always DMA */
	if (msm_port->is_uartdm > UARTDM_1P3) {
		dma_count = UARTDM_TX_AIGN(dma_count);
		dma_min = UARTDM_BURST_SIZE;
	} else {
		if (dma_count > UARTDM_TX_MAX)
			dma_count = UARTDM_TX_MAX;
	}

	if (pio_count > port->fifosize)
		pio_count = port->fifosize;

	if (!dma->chan || dma_count < dma_min)
		msm_handle_tx_pio(port, pio_count);
	else
		err = msm_handle_tx_dma(msm_port, dma_count);

	if (err)	/* fall back to PIO mode */
		msm_handle_tx_pio(port, pio_count);
}

static void msm_handle_delta_cts(struct uart_port *port)
{
	msm_write(port, MSM_UART_CR_CMD_RESET_CTS, MSM_UART_CR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	port->icount.cts++;
	wake_up_interruptible(&port->state->port.delta_msr_wait);
}

<<<<<<< HEAD
static irqreturn_t msm_irq(int irq, void *dev_id)
{
	unsigned long flags;
	struct uart_port *port = dev_id;
	struct msm_port *msm_port = UART_TO_MSM(port);
	unsigned int misr;

	spin_lock_irqsave(&port->lock, flags);
	misr = msm_read(port, UART_MISR);
	msm_write(port, 0, UART_IMR); /* disable interrupt */

	if (misr & (UART_IMR_RXLEV | UART_IMR_RXSTALE))
		handle_rx(port);
	if (misr & UART_IMR_TXLEV)
		handle_tx(port);
	if (misr & UART_IMR_DELTA_CTS)
		handle_delta_cts(port);

	msm_write(port, msm_port->imr, UART_IMR); /* restore interrupt */
	spin_unlock_irqrestore(&port->lock, flags);
=======
static irqreturn_t msm_uart_irq(int irq, void *dev_id)
{
	struct uart_port *port = dev_id;
	struct msm_port *msm_port = to_msm_port(port);
	struct msm_dma *dma = &msm_port->rx_dma;
	unsigned int misr;
	u32 val;

	uart_port_lock(port);
	misr = msm_read(port, MSM_UART_MISR);
	msm_write(port, 0, MSM_UART_IMR); /* disable interrupt */

	if (misr & MSM_UART_IMR_RXBREAK_START) {
		msm_port->break_detected = true;
		msm_write(port, MSM_UART_CR_CMD_RESET_RXBREAK_START, MSM_UART_CR);
	}

	if (misr & (MSM_UART_IMR_RXLEV | MSM_UART_IMR_RXSTALE)) {
		if (dma->count) {
			val = MSM_UART_CR_CMD_STALE_EVENT_DISABLE;
			msm_write(port, val, MSM_UART_CR);
			val = MSM_UART_CR_CMD_RESET_STALE_INT;
			msm_write(port, val, MSM_UART_CR);
			/*
			 * Flush DMA input fifo to memory, this will also
			 * trigger DMA RX completion
			 */
			dmaengine_terminate_all(dma->chan);
		} else if (msm_port->is_uartdm) {
			msm_handle_rx_dm(port, misr);
		} else {
			msm_handle_rx(port);
		}
	}
	if (misr & MSM_UART_IMR_TXLEV)
		msm_handle_tx(port);
	if (misr & MSM_UART_IMR_DELTA_CTS)
		msm_handle_delta_cts(port);

	msm_write(port, msm_port->imr, MSM_UART_IMR); /* restore interrupt */
	uart_unlock_and_check_sysrq(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return IRQ_HANDLED;
}

static unsigned int msm_tx_empty(struct uart_port *port)
{
<<<<<<< HEAD
	unsigned int ret;

	ret = (msm_read(port, UART_SR) & UART_SR_TX_EMPTY) ? TIOCSER_TEMT : 0;
	return ret;
=======
	return (msm_read(port, MSM_UART_SR) & MSM_UART_SR_TX_EMPTY) ? TIOCSER_TEMT : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned int msm_get_mctrl(struct uart_port *port)
{
	return TIOCM_CAR | TIOCM_CTS | TIOCM_DSR | TIOCM_RTS;
}

<<<<<<< HEAD
=======
static void msm_reset(struct uart_port *port)
{
	struct msm_port *msm_port = to_msm_port(port);
	unsigned int mr;

	/* reset everything */
	msm_write(port, MSM_UART_CR_CMD_RESET_RX, MSM_UART_CR);
	msm_write(port, MSM_UART_CR_CMD_RESET_TX, MSM_UART_CR);
	msm_write(port, MSM_UART_CR_CMD_RESET_ERR, MSM_UART_CR);
	msm_write(port, MSM_UART_CR_CMD_RESET_BREAK_INT, MSM_UART_CR);
	msm_write(port, MSM_UART_CR_CMD_RESET_CTS, MSM_UART_CR);
	msm_write(port, MSM_UART_CR_CMD_RESET_RFR, MSM_UART_CR);
	mr = msm_read(port, MSM_UART_MR1);
	mr &= ~MSM_UART_MR1_RX_RDY_CTL;
	msm_write(port, mr, MSM_UART_MR1);

	/* Disable DM modes */
	if (msm_port->is_uartdm)
		msm_write(port, 0, UARTDM_DMEN);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void msm_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	unsigned int mr;

<<<<<<< HEAD
	mr = msm_read(port, UART_MR1);

	if (!(mctrl & TIOCM_RTS)) {
		mr &= ~UART_MR1_RX_RDY_CTL;
		msm_write(port, mr, UART_MR1);
		msm_write(port, UART_CR_CMD_RESET_RFR, UART_CR);
	} else {
		mr |= UART_MR1_RX_RDY_CTL;
		msm_write(port, mr, UART_MR1);
=======
	mr = msm_read(port, MSM_UART_MR1);

	if (!(mctrl & TIOCM_RTS)) {
		mr &= ~MSM_UART_MR1_RX_RDY_CTL;
		msm_write(port, mr, MSM_UART_MR1);
		msm_write(port, MSM_UART_CR_CMD_RESET_RFR, MSM_UART_CR);
	} else {
		mr |= MSM_UART_MR1_RX_RDY_CTL;
		msm_write(port, mr, MSM_UART_MR1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void msm_break_ctl(struct uart_port *port, int break_ctl)
{
	if (break_ctl)
<<<<<<< HEAD
		msm_write(port, UART_CR_CMD_START_BREAK, UART_CR);
	else
		msm_write(port, UART_CR_CMD_STOP_BREAK, UART_CR);
}

static void msm_set_baud_rate(struct uart_port *port, unsigned int baud)
{
	unsigned int baud_code, rxstale, watermark;

	switch (baud) {
	case 300:
		baud_code = UART_CSR_300;
		rxstale = 1;
		break;
	case 600:
		baud_code = UART_CSR_600;
		rxstale = 1;
		break;
	case 1200:
		baud_code = UART_CSR_1200;
		rxstale = 1;
		break;
	case 2400:
		baud_code = UART_CSR_2400;
		rxstale = 1;
		break;
	case 4800:
		baud_code = UART_CSR_4800;
		rxstale = 1;
		break;
	case 9600:
		baud_code = UART_CSR_9600;
		rxstale = 2;
		break;
	case 14400:
		baud_code = UART_CSR_14400;
		rxstale = 3;
		break;
	case 19200:
		baud_code = UART_CSR_19200;
		rxstale = 4;
		break;
	case 28800:
		baud_code = UART_CSR_28800;
		rxstale = 6;
		break;
	case 38400:
		baud_code = UART_CSR_38400;
		rxstale = 8;
		break;
	case 57600:
		baud_code = UART_CSR_57600;
		rxstale = 16;
		break;
	case 115200:
	default:
		baud_code = UART_CSR_115200;
		rxstale = 31;
		break;
	}

	msm_write(port, baud_code, UART_CSR);

	/* RX stale watermark */
	watermark = UART_IPR_STALE_LSB & rxstale;
	watermark |= UART_IPR_RXSTALE_LAST;
	watermark |= UART_IPR_STALE_TIMEOUT_MSB & (rxstale << 2);
	msm_write(port, watermark, UART_IPR);

	/* set RX watermark */
	watermark = (port->fifosize * 3) / 4;
	msm_write(port, watermark, UART_RFWR);

	/* set TX watermark */
	msm_write(port, 10, UART_TFWR);
}

static void msm_reset(struct uart_port *port)
{
	/* reset everything */
	msm_write(port, UART_CR_CMD_RESET_RX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_TX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_BREAK_INT, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
	msm_write(port, UART_CR_CMD_SET_RFR, UART_CR);
=======
		msm_write(port, MSM_UART_CR_CMD_START_BREAK, MSM_UART_CR);
	else
		msm_write(port, MSM_UART_CR_CMD_STOP_BREAK, MSM_UART_CR);
}

struct msm_baud_map {
	u16	divisor;
	u8	code;
	u8	rxstale;
};

static const struct msm_baud_map *
msm_find_best_baud(struct uart_port *port, unsigned int baud,
		   unsigned long *rate)
{
	struct msm_port *msm_port = to_msm_port(port);
	unsigned int divisor, result;
	unsigned long target, old, best_rate = 0, diff, best_diff = ULONG_MAX;
	const struct msm_baud_map *entry, *end, *best;
	static const struct msm_baud_map table[] = {
		{    1, 0xff, 31 },
		{    2, 0xee, 16 },
		{    3, 0xdd,  8 },
		{    4, 0xcc,  6 },
		{    6, 0xbb,  6 },
		{    8, 0xaa,  6 },
		{   12, 0x99,  6 },
		{   16, 0x88,  1 },
		{   24, 0x77,  1 },
		{   32, 0x66,  1 },
		{   48, 0x55,  1 },
		{   96, 0x44,  1 },
		{  192, 0x33,  1 },
		{  384, 0x22,  1 },
		{  768, 0x11,  1 },
		{ 1536, 0x00,  1 },
	};

	best = table; /* Default to smallest divider */
	target = clk_round_rate(msm_port->clk, 16 * baud);
	divisor = DIV_ROUND_CLOSEST(target, 16 * baud);

	end = table + ARRAY_SIZE(table);
	entry = table;
	while (entry < end) {
		if (entry->divisor <= divisor) {
			result = target / entry->divisor / 16;
			diff = abs(result - baud);

			/* Keep track of best entry */
			if (diff < best_diff) {
				best_diff = diff;
				best = entry;
				best_rate = target;
			}

			if (result == baud)
				break;
		} else if (entry->divisor > divisor) {
			old = target;
			target = clk_round_rate(msm_port->clk, old + 1);
			/*
			 * The rate didn't get any faster so we can't do
			 * better at dividing it down
			 */
			if (target == old)
				break;

			/* Start the divisor search over at this new rate */
			entry = table;
			divisor = DIV_ROUND_CLOSEST(target, 16 * baud);
			continue;
		}
		entry++;
	}

	*rate = best_rate;
	return best;
}

static int msm_set_baud_rate(struct uart_port *port, unsigned int baud,
			     unsigned long *saved_flags)
	__must_hold(&port->lock)
{
	unsigned int rxstale, watermark, mask;
	struct msm_port *msm_port = to_msm_port(port);
	const struct msm_baud_map *entry;
	unsigned long flags, rate;

	flags = *saved_flags;
	uart_port_unlock_irqrestore(port, flags);

	entry = msm_find_best_baud(port, baud, &rate);
	dev_pm_opp_set_rate(port->dev, rate);
	baud = rate / 16 / entry->divisor;

	uart_port_lock_irqsave(port, &flags);
	*saved_flags = flags;
	port->uartclk = rate;

	msm_write(port, entry->code, MSM_UART_CSR);

	/* RX stale watermark */
	rxstale = entry->rxstale;
	watermark = MSM_UART_IPR_STALE_LSB & rxstale;
	if (msm_port->is_uartdm) {
		mask = MSM_UART_DM_IPR_STALE_TIMEOUT_MSB;
	} else {
		watermark |= MSM_UART_IPR_RXSTALE_LAST;
		mask = MSM_UART_IPR_STALE_TIMEOUT_MSB;
	}

	watermark |= mask & (rxstale << 2);

	msm_write(port, watermark, MSM_UART_IPR);

	/* set RX watermark */
	watermark = (port->fifosize * 3) / 4;
	msm_write(port, watermark, MSM_UART_RFWR);

	/* set TX watermark */
	msm_write(port, 10, MSM_UART_TFWR);

	msm_write(port, MSM_UART_CR_CMD_PROTECTION_EN, MSM_UART_CR);
	msm_reset(port);

	/* Enable RX and TX */
	msm_write(port, MSM_UART_CR_TX_ENABLE | MSM_UART_CR_RX_ENABLE, MSM_UART_CR);

	/* turn on RX and CTS interrupts */
	msm_port->imr = MSM_UART_IMR_RXLEV | MSM_UART_IMR_RXSTALE |
			MSM_UART_IMR_CURRENT_CTS | MSM_UART_IMR_RXBREAK_START;

	msm_write(port, msm_port->imr, MSM_UART_IMR);

	if (msm_port->is_uartdm) {
		msm_write(port, MSM_UART_CR_CMD_RESET_STALE_INT, MSM_UART_CR);
		msm_write(port, 0xFFFFFF, UARTDM_DMRX);
		msm_write(port, MSM_UART_CR_CMD_STALE_EVENT_ENABLE, MSM_UART_CR);
	}

	return baud;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void msm_init_clock(struct uart_port *port)
{
<<<<<<< HEAD
	int ret;
	struct msm_port *msm_port = UART_TO_MSM(port);

	ret = clk_prepare_enable(msm_port->clk);
	if (ret) {
		pr_err("%s(): Can't enable uartclk. ret:%d\n", __func__, ret);
		return;
	}

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	msm_port->clk_state = MSM_CLK_ON;
#endif

	if (msm_port->uim) {
		msm_write(port,
			UART_SIM_CFG_STOP_BIT_LEN_N(2) |
			UART_SIM_CFG_SIM_CLK_ON |
			UART_SIM_CFG_SIM_CLK_STOP_HIGH |
			UART_SIM_CFG_MASK_RX |
			UART_SIM_CFG_SIM_SEL,
			UART_SIM_CFG);

		/* (TCXO * 16) / (5 * 372) = TCXO * 16 / 1860 */
		msm_write(port, 0x08, UART_MREG);
		msm_write(port, 0x19, UART_NREG);
		msm_write(port, 0xe8, UART_DREG);
		msm_write(port, 0x0e, UART_MNDREG);
	} else if (port->uartclk == 19200000) {
		/* clock is TCXO (19.2MHz) */
		msm_write(port, 0x06, UART_MREG);
		msm_write(port, 0xF1, UART_NREG);
		msm_write(port, 0x0F, UART_DREG);
		msm_write(port, 0x1A, UART_MNDREG);
	} else {
		/* clock must be TCXO/4 */
		msm_write(port, 0x18, UART_MREG);
		msm_write(port, 0xF6, UART_NREG);
		msm_write(port, 0x0F, UART_DREG);
		msm_write(port, 0x0A, UART_MNDREG);
	}
}

static void msm_deinit_clock(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	if (msm_port->clk_state != MSM_CLK_OFF)
		clk_disable(msm_port->clk);
	msm_port->clk_state = MSM_CLK_PORT_OFF;
#else
	clk_disable_unprepare(msm_port->clk);
#endif

}
static int msm_startup(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);
	unsigned int data, rfr_level;
=======
	struct msm_port *msm_port = to_msm_port(port);

	dev_pm_opp_set_rate(port->dev, port->uartclk);
	clk_prepare_enable(msm_port->clk);
	clk_prepare_enable(msm_port->pclk);
	msm_serial_set_mnd_regs(port);
}

static int msm_startup(struct uart_port *port)
{
	struct msm_port *msm_port = to_msm_port(port);
	unsigned int data, rfr_level, mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	snprintf(msm_port->name, sizeof(msm_port->name),
		 "msm_serial%d", port->line);

<<<<<<< HEAD
	ret = request_irq(port->irq, msm_irq, IRQF_TRIGGER_HIGH,
			  msm_port->name, port);
	if (unlikely(ret))
		return ret;


#ifndef CONFIG_PM_RUNTIME
	msm_init_clock(port);
#endif
	pm_runtime_get_sync(port->dev);
=======
	msm_init_clock(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (likely(port->fifosize > 12))
		rfr_level = port->fifosize - 12;
	else
		rfr_level = port->fifosize;

	/* set automatic RFR level */
<<<<<<< HEAD
	data = msm_read(port, UART_MR1);
	data &= ~UART_MR1_AUTO_RFR_LEVEL1;
	data &= ~UART_MR1_AUTO_RFR_LEVEL0;
	data |= UART_MR1_AUTO_RFR_LEVEL1 & (rfr_level << 2);
	data |= UART_MR1_AUTO_RFR_LEVEL0 & rfr_level;
	msm_write(port, data, UART_MR1);

	/* make sure that RXSTALE count is non-zero */
	data = msm_read(port, UART_IPR);
	if (unlikely(!data)) {
		data |= UART_IPR_RXSTALE_LAST;
		data |= UART_IPR_STALE_LSB;
		msm_write(port, data, UART_IPR);
	}

	msm_reset(port);

	msm_write(port, 0x05, UART_CR);	/* enable TX & RX */

	/* turn on RX and CTS interrupts */
	msm_port->imr = UART_IMR_RXLEV | UART_IMR_RXSTALE |
			UART_IMR_CURRENT_CTS;
	msm_write(port, msm_port->imr, UART_IMR);

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	if (use_low_power_wakeup(msm_port)) {
		ret = irq_set_irq_wake(msm_port->wakeup.irq, 1);
		if (unlikely(ret))
			return ret;
		ret = request_irq(msm_port->wakeup.irq, msm_rx_irq,
				  IRQF_TRIGGER_FALLING,
				  "msm_serial_wakeup", msm_port);
		if (unlikely(ret))
			return ret;
		disable_irq(msm_port->wakeup.irq);
	}
#endif

	return 0;
=======
	data = msm_read(port, MSM_UART_MR1);

	if (msm_port->is_uartdm)
		mask = MSM_UART_DM_MR1_AUTO_RFR_LEVEL1;
	else
		mask = MSM_UART_MR1_AUTO_RFR_LEVEL1;

	data &= ~mask;
	data &= ~MSM_UART_MR1_AUTO_RFR_LEVEL0;
	data |= mask & (rfr_level << 2);
	data |= MSM_UART_MR1_AUTO_RFR_LEVEL0 & rfr_level;
	msm_write(port, data, MSM_UART_MR1);

	if (msm_port->is_uartdm) {
		msm_request_tx_dma(msm_port, msm_port->uart.mapbase);
		msm_request_rx_dma(msm_port, msm_port->uart.mapbase);
	}

	ret = request_irq(port->irq, msm_uart_irq, IRQF_TRIGGER_HIGH,
			  msm_port->name, port);
	if (unlikely(ret))
		goto err_irq;

	return 0;

err_irq:
	if (msm_port->is_uartdm)
		msm_release_dma(msm_port);

	clk_disable_unprepare(msm_port->pclk);
	clk_disable_unprepare(msm_port->clk);
	dev_pm_opp_set_rate(port->dev, 0);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void msm_shutdown(struct uart_port *port)
{
<<<<<<< HEAD
	struct msm_port *msm_port = UART_TO_MSM(port);

	if (msm_port->uim)
		msm_write(port,
			UART_SIM_CFG_SIM_CLK_STOP_HIGH,
			UART_SIM_CFG);

	msm_port->imr = 0;
	msm_write(port, 0, UART_IMR); /* disable interrupts */

	free_irq(port->irq, port);

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	if (use_low_power_wakeup(msm_port)) {
		irq_set_irq_wake(msm_port->wakeup.irq, 0);
		free_irq(msm_port->wakeup.irq, msm_port);
	}
#endif
#ifndef CONFIG_PM_RUNTIME
	msm_deinit_clock(port);
#endif
	pm_runtime_put_sync(port->dev);
}

static void msm_set_termios(struct uart_port *port, struct ktermios *termios,
			    struct ktermios *old)
{
	unsigned long flags;
	unsigned int baud, mr;

	if (!termios->c_cflag)
		return;

	spin_lock_irqsave(&port->lock, flags);

	/* calculate and set baud rate */
	baud = uart_get_baud_rate(port, termios, old, 300, 115200);
	msm_set_baud_rate(port, baud);

	/* calculate parity */
	mr = msm_read(port, UART_MR2);
	mr &= ~UART_MR2_PARITY_MODE;
	if (termios->c_cflag & PARENB) {
		if (termios->c_cflag & PARODD)
			mr |= UART_MR2_PARITY_MODE_ODD;
		else if (termios->c_cflag & CMSPAR)
			mr |= UART_MR2_PARITY_MODE_SPACE;
		else
			mr |= UART_MR2_PARITY_MODE_EVEN;
	}

	/* calculate bits per char */
	mr &= ~UART_MR2_BITS_PER_CHAR;
	switch (termios->c_cflag & CSIZE) {
	case CS5:
		mr |= UART_MR2_BITS_PER_CHAR_5;
		break;
	case CS6:
		mr |= UART_MR2_BITS_PER_CHAR_6;
		break;
	case CS7:
		mr |= UART_MR2_BITS_PER_CHAR_7;
		break;
	case CS8:
	default:
		mr |= UART_MR2_BITS_PER_CHAR_8;
=======
	struct msm_port *msm_port = to_msm_port(port);

	msm_port->imr = 0;
	msm_write(port, 0, MSM_UART_IMR); /* disable interrupts */

	if (msm_port->is_uartdm)
		msm_release_dma(msm_port);

	clk_disable_unprepare(msm_port->clk);
	dev_pm_opp_set_rate(port->dev, 0);

	free_irq(port->irq, port);
}

static void msm_set_termios(struct uart_port *port, struct ktermios *termios,
			    const struct ktermios *old)
{
	struct msm_port *msm_port = to_msm_port(port);
	struct msm_dma *dma = &msm_port->rx_dma;
	unsigned long flags;
	unsigned int baud, mr;

	uart_port_lock_irqsave(port, &flags);

	if (dma->chan) /* Terminate if any */
		msm_stop_dma(port, dma);

	/* calculate and set baud rate */
	baud = uart_get_baud_rate(port, termios, old, 300, 4000000);
	baud = msm_set_baud_rate(port, baud, &flags);
	if (tty_termios_baud_rate(termios))
		tty_termios_encode_baud_rate(termios, baud, baud);

	/* calculate parity */
	mr = msm_read(port, MSM_UART_MR2);
	mr &= ~MSM_UART_MR2_PARITY_MODE;
	if (termios->c_cflag & PARENB) {
		if (termios->c_cflag & PARODD)
			mr |= MSM_UART_MR2_PARITY_MODE_ODD;
		else if (termios->c_cflag & CMSPAR)
			mr |= MSM_UART_MR2_PARITY_MODE_SPACE;
		else
			mr |= MSM_UART_MR2_PARITY_MODE_EVEN;
	}

	/* calculate bits per char */
	mr &= ~MSM_UART_MR2_BITS_PER_CHAR;
	switch (termios->c_cflag & CSIZE) {
	case CS5:
		mr |= MSM_UART_MR2_BITS_PER_CHAR_5;
		break;
	case CS6:
		mr |= MSM_UART_MR2_BITS_PER_CHAR_6;
		break;
	case CS7:
		mr |= MSM_UART_MR2_BITS_PER_CHAR_7;
		break;
	case CS8:
	default:
		mr |= MSM_UART_MR2_BITS_PER_CHAR_8;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	/* calculate stop bits */
<<<<<<< HEAD
	mr &= ~(UART_MR2_STOP_BIT_LEN_ONE | UART_MR2_STOP_BIT_LEN_TWO);
	if (termios->c_cflag & CSTOPB)
		mr |= UART_MR2_STOP_BIT_LEN_TWO;
	else
		mr |= UART_MR2_STOP_BIT_LEN_ONE;

	/* set parity, bits per char, and stop bit */
	msm_write(port, mr, UART_MR2);

	/* calculate and set hardware flow control */
	mr = msm_read(port, UART_MR1);
	mr &= ~(UART_MR1_CTS_CTL | UART_MR1_RX_RDY_CTL);
	if (termios->c_cflag & CRTSCTS) {
		mr |= UART_MR1_CTS_CTL;
		mr |= UART_MR1_RX_RDY_CTL;
	}
	msm_write(port, mr, UART_MR1);
=======
	mr &= ~(MSM_UART_MR2_STOP_BIT_LEN_ONE | MSM_UART_MR2_STOP_BIT_LEN_TWO);
	if (termios->c_cflag & CSTOPB)
		mr |= MSM_UART_MR2_STOP_BIT_LEN_TWO;
	else
		mr |= MSM_UART_MR2_STOP_BIT_LEN_ONE;

	/* set parity, bits per char, and stop bit */
	msm_write(port, mr, MSM_UART_MR2);

	/* calculate and set hardware flow control */
	mr = msm_read(port, MSM_UART_MR1);
	mr &= ~(MSM_UART_MR1_CTS_CTL | MSM_UART_MR1_RX_RDY_CTL);
	if (termios->c_cflag & CRTSCTS) {
		mr |= MSM_UART_MR1_CTS_CTL;
		mr |= MSM_UART_MR1_RX_RDY_CTL;
	}
	msm_write(port, mr, MSM_UART_MR1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Configure status bits to ignore based on termio flags. */
	port->read_status_mask = 0;
	if (termios->c_iflag & INPCK)
<<<<<<< HEAD
		port->read_status_mask |= UART_SR_PAR_FRAME_ERR;
	if (termios->c_iflag & (BRKINT | PARMRK))
		port->read_status_mask |= UART_SR_RX_BREAK;

	uart_update_timeout(port, termios->c_cflag, baud);
	spin_unlock_irqrestore(&port->lock, flags);
=======
		port->read_status_mask |= MSM_UART_SR_PAR_FRAME_ERR;
	if (termios->c_iflag & (IGNBRK | BRKINT | PARMRK))
		port->read_status_mask |= MSM_UART_SR_RX_BREAK;

	uart_update_timeout(port, termios->c_cflag, baud);

	/* Try to use DMA */
	msm_start_rx_dma(msm_port);

	uart_port_unlock_irqrestore(port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const char *msm_type(struct uart_port *port)
{
	return "MSM";
}

static void msm_release_port(struct uart_port *port)
{
	struct platform_device *pdev = to_platform_device(port->dev);
<<<<<<< HEAD
	struct resource *resource;
	resource_size_t size;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return;
	size = resource->end - resource->start + 1;
=======
	struct resource *uart_resource;
	resource_size_t size;

	uart_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!uart_resource))
		return;
	size = resource_size(uart_resource);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	release_mem_region(port->mapbase, size);
	iounmap(port->membase);
	port->membase = NULL;
}

static int msm_request_port(struct uart_port *port)
{
	struct platform_device *pdev = to_platform_device(port->dev);
<<<<<<< HEAD
	struct resource *resource;
	resource_size_t size;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	size = resource->end - resource->start + 1;

	if (unlikely(!request_mem_region(port->mapbase, size, "msm_serial")))
=======
	struct resource *uart_resource;
	resource_size_t size;
	int ret;

	uart_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!uart_resource))
		return -ENXIO;

	size = resource_size(uart_resource);

	if (!request_mem_region(port->mapbase, size, "msm_serial"))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EBUSY;

	port->membase = ioremap(port->mapbase, size);
	if (!port->membase) {
<<<<<<< HEAD
		release_mem_region(port->mapbase, size);
		return -EBUSY;
	}

	return 0;
=======
		ret = -EBUSY;
		goto fail_release_port;
	}

	return 0;

fail_release_port:
	release_mem_region(port->mapbase, size);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void msm_config_port(struct uart_port *port, int flags)
{
<<<<<<< HEAD
	if (flags & UART_CONFIG_TYPE) {
		port->type = PORT_MSM;
		msm_request_port(port);
=======
	int ret;

	if (flags & UART_CONFIG_TYPE) {
		port->type = PORT_MSM;
		ret = msm_request_port(port);
		if (ret)
			return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static int msm_verify_port(struct uart_port *port, struct serial_struct *ser)
{
	if (unlikely(ser->type != PORT_UNKNOWN && ser->type != PORT_MSM))
		return -EINVAL;
	if (unlikely(port->irq != ser->irq))
		return -EINVAL;
	return 0;
}

static void msm_power(struct uart_port *port, unsigned int state,
		      unsigned int oldstate)
{
<<<<<<< HEAD
	int ret;
	struct msm_port *msm_port = UART_TO_MSM(port);

	switch (state) {
	case 0:
		ret = clk_prepare_enable(msm_port->clk);
		if (ret)
			pr_err("msm_serial: %s(): Can't enable uartclk.\n",
						__func__);
		break;
	case 3:
		clk_disable_unprepare(msm_port->clk);
		break;
	default:
		pr_err("msm_serial: %s(): Unknown PM state %d\n",
						__func__, state);
	}
}

static struct uart_ops msm_uart_pops = {
=======
	struct msm_port *msm_port = to_msm_port(port);

	switch (state) {
	case 0:
		dev_pm_opp_set_rate(port->dev, port->uartclk);
		clk_prepare_enable(msm_port->clk);
		clk_prepare_enable(msm_port->pclk);
		break;
	case 3:
		clk_disable_unprepare(msm_port->clk);
		dev_pm_opp_set_rate(port->dev, 0);
		clk_disable_unprepare(msm_port->pclk);
		break;
	default:
		pr_err("msm_serial: Unknown PM state %d\n", state);
	}
}

#ifdef CONFIG_CONSOLE_POLL
static int msm_poll_get_char_single(struct uart_port *port)
{
	struct msm_port *msm_port = to_msm_port(port);
	unsigned int rf_reg = msm_port->is_uartdm ? UARTDM_RF : MSM_UART_RF;

	if (!(msm_read(port, MSM_UART_SR) & MSM_UART_SR_RX_READY))
		return NO_POLL_CHAR;

	return msm_read(port, rf_reg) & 0xff;
}

static int msm_poll_get_char_dm(struct uart_port *port)
{
	int c;
	static u32 slop;
	static int count;
	unsigned char *sp = (unsigned char *)&slop;

	/* Check if a previous read had more than one char */
	if (count) {
		c = sp[sizeof(slop) - count];
		count--;
	/* Or if FIFO is empty */
	} else if (!(msm_read(port, MSM_UART_SR) & MSM_UART_SR_RX_READY)) {
		/*
		 * If RX packing buffer has less than a word, force stale to
		 * push contents into RX FIFO
		 */
		count = msm_read(port, UARTDM_RXFS);
		count = (count >> UARTDM_RXFS_BUF_SHIFT) & UARTDM_RXFS_BUF_MASK;
		if (count) {
			msm_write(port, MSM_UART_CR_CMD_FORCE_STALE, MSM_UART_CR);
			slop = msm_read(port, UARTDM_RF);
			c = sp[0];
			count--;
			msm_write(port, MSM_UART_CR_CMD_RESET_STALE_INT, MSM_UART_CR);
			msm_write(port, 0xFFFFFF, UARTDM_DMRX);
			msm_write(port, MSM_UART_CR_CMD_STALE_EVENT_ENABLE, MSM_UART_CR);
		} else {
			c = NO_POLL_CHAR;
		}
	/* FIFO has a word */
	} else {
		slop = msm_read(port, UARTDM_RF);
		c = sp[0];
		count = sizeof(slop) - 1;
	}

	return c;
}

static int msm_poll_get_char(struct uart_port *port)
{
	u32 imr;
	int c;
	struct msm_port *msm_port = to_msm_port(port);

	/* Disable all interrupts */
	imr = msm_read(port, MSM_UART_IMR);
	msm_write(port, 0, MSM_UART_IMR);

	if (msm_port->is_uartdm)
		c = msm_poll_get_char_dm(port);
	else
		c = msm_poll_get_char_single(port);

	/* Enable interrupts */
	msm_write(port, imr, MSM_UART_IMR);

	return c;
}

static void msm_poll_put_char(struct uart_port *port, unsigned char c)
{
	u32 imr;
	struct msm_port *msm_port = to_msm_port(port);

	/* Disable all interrupts */
	imr = msm_read(port, MSM_UART_IMR);
	msm_write(port, 0, MSM_UART_IMR);

	if (msm_port->is_uartdm)
		msm_reset_dm_count(port, 1);

	/* Wait until FIFO is empty */
	while (!(msm_read(port, MSM_UART_SR) & MSM_UART_SR_TX_READY))
		cpu_relax();

	/* Write a character */
	msm_write(port, c, msm_port->is_uartdm ? UARTDM_TF : MSM_UART_TF);

	/* Wait until FIFO is empty */
	while (!(msm_read(port, MSM_UART_SR) & MSM_UART_SR_TX_READY))
		cpu_relax();

	/* Enable interrupts */
	msm_write(port, imr, MSM_UART_IMR);
}
#endif

static const struct uart_ops msm_uart_pops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.tx_empty = msm_tx_empty,
	.set_mctrl = msm_set_mctrl,
	.get_mctrl = msm_get_mctrl,
	.stop_tx = msm_stop_tx,
	.start_tx = msm_start_tx,
	.stop_rx = msm_stop_rx,
	.enable_ms = msm_enable_ms,
	.break_ctl = msm_break_ctl,
	.startup = msm_startup,
	.shutdown = msm_shutdown,
	.set_termios = msm_set_termios,
	.type = msm_type,
	.release_port = msm_release_port,
	.request_port = msm_request_port,
	.config_port = msm_config_port,
	.verify_port = msm_verify_port,
	.pm = msm_power,
<<<<<<< HEAD
=======
#ifdef CONFIG_CONSOLE_POLL
	.poll_get_char	= msm_poll_get_char,
	.poll_put_char	= msm_poll_put_char,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct msm_port msm_uart_ports[] = {
	{
		.uart = {
			.iotype = UPIO_MEM,
			.ops = &msm_uart_pops,
			.flags = UPF_BOOT_AUTOCONF,
<<<<<<< HEAD
			.fifosize = 512,
=======
			.fifosize = 64,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			.line = 0,
		},
	},
	{
		.uart = {
			.iotype = UPIO_MEM,
			.ops = &msm_uart_pops,
			.flags = UPF_BOOT_AUTOCONF,
<<<<<<< HEAD
			.fifosize = 512,
=======
			.fifosize = 64,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			.line = 1,
		},
	},
	{
		.uart = {
			.iotype = UPIO_MEM,
			.ops = &msm_uart_pops,
			.flags = UPF_BOOT_AUTOCONF,
			.fifosize = 64,
			.line = 2,
		},
	},
};

<<<<<<< HEAD
#define UART_NR 256
static inline struct uart_port * get_port_from_line(unsigned int line)
=======
#define MSM_UART_NR	ARRAY_SIZE(msm_uart_ports)

static inline struct uart_port *msm_get_port_from_line(unsigned int line)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return &msm_uart_ports[line].uart;
}

#ifdef CONFIG_SERIAL_MSM_CONSOLE
<<<<<<< HEAD

/*
 *  Wait for transmitter & holding register to empty
 *  Derived from wait_for_xmitr in 8250 serial driver by Russell King
 */
static inline void wait_for_xmitr(struct uart_port *port, int bits)
{
	unsigned int status, mr, tmout = 10000;

	/* Wait up to 10ms for the character(s) to be sent. */
	do {
		status = msm_read(port, UART_SR);

		if (--tmout == 0)
			break;
		udelay(1);
	} while ((status & bits) != bits);

	mr = msm_read(port, UART_MR1);

	/* Wait up to 1s for flow control if necessary */
	if (mr & UART_MR1_CTS_CTL) {
		unsigned int tmout;
		for (tmout = 1000000; tmout; tmout--) {
			unsigned int isr = msm_read(port, UART_ISR);

			/* CTS input is active lo */
			if (!(isr & UART_IMR_CURRENT_CTS))
				break;
			udelay(1);
			touch_nmi_watchdog();
		}
	}
}


static void msm_console_putchar(struct uart_port *port, int c)
{
	/* This call can incur significant delay if CTS flowcontrol is enabled
	 * on port and no serial cable is attached.
	 */
	wait_for_xmitr(port, UART_SR_TX_READY);

	msm_write(port, c, UART_TF);
=======
static void __msm_console_write(struct uart_port *port, const char *s,
				unsigned int count, bool is_uartdm)
{
	unsigned long flags;
	int i;
	int num_newlines = 0;
	bool replaced = false;
	void __iomem *tf;
	int locked = 1;

	if (is_uartdm)
		tf = port->membase + UARTDM_TF;
	else
		tf = port->membase + MSM_UART_TF;

	/* Account for newlines that will get a carriage return added */
	for (i = 0; i < count; i++)
		if (s[i] == '\n')
			num_newlines++;
	count += num_newlines;

	if (oops_in_progress)
		locked = uart_port_trylock_irqsave(port, &flags);
	else
		uart_port_lock_irqsave(port, &flags);

	if (is_uartdm)
		msm_reset_dm_count(port, count);

	i = 0;
	while (i < count) {
		int j;
		unsigned int num_chars;
		char buf[4] = { 0 };

		if (is_uartdm)
			num_chars = min(count - i, (unsigned int)sizeof(buf));
		else
			num_chars = 1;

		for (j = 0; j < num_chars; j++) {
			char c = *s;

			if (c == '\n' && !replaced) {
				buf[j] = '\r';
				j++;
				replaced = true;
			}
			if (j < num_chars) {
				buf[j] = c;
				s++;
				replaced = false;
			}
		}

		while (!(msm_read(port, MSM_UART_SR) & MSM_UART_SR_TX_READY))
			cpu_relax();

		iowrite32_rep(tf, buf, 1);
		i += num_chars;
	}

	if (locked)
		uart_port_unlock_irqrestore(port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void msm_console_write(struct console *co, const char *s,
			      unsigned int count)
{
	struct uart_port *port;
	struct msm_port *msm_port;
<<<<<<< HEAD
	int locked;

	BUG_ON(co->index < 0 || co->index >= UART_NR);

	port = get_port_from_line(co->index);
	msm_port = UART_TO_MSM(port);

	/* not pretty, but we can end up here via various convoluted paths */
	if (port->sysrq || oops_in_progress)
		locked = spin_trylock(&port->lock);
	else {
		locked = 1;
		spin_lock(&port->lock);
	}

	uart_console_write(port, s, count, msm_console_putchar);

	if (locked)
		spin_unlock(&port->lock);
}

static int __init msm_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
	int baud = 0, flow, bits, parity;

	if (unlikely(co->index >= UART_NR || co->index < 0))
		return -ENXIO;

	port = get_port_from_line(co->index);
=======

	BUG_ON(co->index < 0 || co->index >= MSM_UART_NR);

	port = msm_get_port_from_line(co->index);
	msm_port = to_msm_port(port);

	__msm_console_write(port, s, count, msm_port->is_uartdm);
}

static int msm_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	if (unlikely(co->index >= MSM_UART_NR || co->index < 0))
		return -ENXIO;

	port = msm_get_port_from_line(co->index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(!port->membase))
		return -ENXIO;

<<<<<<< HEAD
	port->cons = co;

	pm_runtime_get_noresume(port->dev);

#ifndef CONFIG_PM_RUNTIME
	msm_init_clock(port);
#endif
	pm_runtime_resume(port->dev);
=======
	msm_init_clock(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);

<<<<<<< HEAD
	bits = 8;
	parity = 'n';
	flow = 'n';
	msm_write(port, UART_MR2_BITS_PER_CHAR_8 | UART_MR2_STOP_BIT_LEN_ONE,
		  UART_MR2);	/* 8N1 */

	if (baud < 300 || baud > 115200)
		baud = 115200;
	msm_set_baud_rate(port, baud);

	msm_reset(port);

	printk(KERN_INFO "msm_serial: console setup on port #%d\n", port->line);
=======
	pr_info("msm_serial: console setup on port #%d\n", port->line);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return uart_set_options(port, co, baud, parity, bits, flow);
}

<<<<<<< HEAD
=======
static void
msm_serial_early_write(struct console *con, const char *s, unsigned n)
{
	struct earlycon_device *dev = con->data;

	__msm_console_write(&dev->port, s, n, false);
}

static int __init
msm_serial_early_console_setup(struct earlycon_device *device, const char *opt)
{
	if (!device->port.membase)
		return -ENODEV;

	device->con->write = msm_serial_early_write;
	return 0;
}
OF_EARLYCON_DECLARE(msm_serial, "qcom,msm-uart",
		    msm_serial_early_console_setup);

static void
msm_serial_early_write_dm(struct console *con, const char *s, unsigned n)
{
	struct earlycon_device *dev = con->data;

	__msm_console_write(&dev->port, s, n, true);
}

static int __init
msm_serial_early_console_setup_dm(struct earlycon_device *device,
				  const char *opt)
{
	if (!device->port.membase)
		return -ENODEV;

	device->con->write = msm_serial_early_write_dm;
	return 0;
}
OF_EARLYCON_DECLARE(msm_serial_dm, "qcom,msm-uartdm",
		    msm_serial_early_console_setup_dm);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct uart_driver msm_uart_driver;

static struct console msm_console = {
	.name = "ttyMSM",
	.write = msm_console_write,
	.device = uart_console_device,
	.setup = msm_console_setup,
	.flags = CON_PRINTBUFFER,
	.index = -1,
	.data = &msm_uart_driver,
};

<<<<<<< HEAD
#define MSM_CONSOLE	&msm_console
=======
#define MSM_CONSOLE	(&msm_console)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#else
#define MSM_CONSOLE	NULL
#endif

static struct uart_driver msm_uart_driver = {
	.owner = THIS_MODULE,
	.driver_name = "msm_serial",
	.dev_name = "ttyMSM",
<<<<<<< HEAD
	.nr = UART_NR,
	.cons = MSM_CONSOLE,
};

static int __init msm_serial_probe(struct platform_device *pdev)
=======
	.nr = MSM_UART_NR,
	.cons = MSM_CONSOLE,
};

static atomic_t msm_uart_next_id = ATOMIC_INIT(0);

static const struct of_device_id msm_uartdm_table[] = {
	{ .compatible = "qcom,msm-uartdm-v1.1", .data = (void *)UARTDM_1P1 },
	{ .compatible = "qcom,msm-uartdm-v1.2", .data = (void *)UARTDM_1P2 },
	{ .compatible = "qcom,msm-uartdm-v1.3", .data = (void *)UARTDM_1P3 },
	{ .compatible = "qcom,msm-uartdm-v1.4", .data = (void *)UARTDM_1P4 },
	{ }
};

static int msm_serial_probe(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct msm_port *msm_port;
	struct resource *resource;
	struct uart_port *port;
<<<<<<< HEAD
	int irq;
	struct msm_serial_platform_data *pdata = pdev->dev.platform_data;

	if (unlikely(pdev->id < 0 || pdev->id >= UART_NR))
		return -ENXIO;

	printk(KERN_INFO "msm_serial: detected port #%d\n", pdev->id);

	port = get_port_from_line(pdev->id);
	port->dev = &pdev->dev;
	msm_port = UART_TO_MSM(port);

	msm_port->clk = clk_get(&pdev->dev, "core_clk");
	if (unlikely(IS_ERR(msm_port->clk)))
		return PTR_ERR(msm_port->clk);
	port->uartclk = clk_get_rate(msm_port->clk);
	if (!port->uartclk)
		port->uartclk = 19200000;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	port->mapbase = resource->start;

	irq = platform_get_irq(pdev, 0);
	if (unlikely(irq < 0))
		return -ENXIO;
	port->irq = irq;

	platform_set_drvdata(pdev, port);


#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	if (pdata == NULL)
		msm_port->wakeup.irq = -1;
	else {
		msm_port->wakeup.irq = pdata->wakeup_irq;
		msm_port->wakeup.ignore = 1;
		msm_port->wakeup.inject_rx = pdata->inject_rx_on_wakeup;
		msm_port->wakeup.rx_to_inject = pdata->rx_to_inject;

		if (unlikely(msm_port->wakeup.irq <= 0))
			return -EINVAL;
	}
#endif

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	msm_port->clk_state = MSM_CLK_PORT_OFF;
	hrtimer_init(&msm_port->clk_off_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	msm_port->clk_off_timer.function = msm_serial_clock_off;
	msm_port->clk_off_delay = ktime_set(0, 1000000);  /* 1 ms */
#endif

	pm_runtime_enable(port->dev);
	if (pdata != NULL && pdata->userid && pdata->userid <= UART_NR)
		port->line = pdata->userid;
	return uart_add_one_port(&msm_uart_driver, port);
}

static int __init msm_uim_probe(struct platform_device *pdev)
{
	struct msm_port *msm_port;
	struct resource *resource;
	struct uart_port *port;
	int irq;

	if (unlikely(pdev->id < 0 || pdev->id >= UART_NR))
		return -ENXIO;

	pr_info("msm_uim: detected port #%d\n", pdev->id);

	port = get_port_from_line(pdev->id);
	port->dev = &pdev->dev;
	msm_port = UART_TO_MSM(port);

	msm_port->uim = true;
=======
	const struct of_device_id *id;
	int irq, line, ret;

	if (pdev->dev.of_node)
		line = of_alias_get_id(pdev->dev.of_node, "serial");
	else
		line = pdev->id;

	if (line < 0)
		line = atomic_inc_return(&msm_uart_next_id) - 1;

	if (unlikely(line < 0 || line >= MSM_UART_NR))
		return -ENXIO;

	dev_info(&pdev->dev, "msm_serial: detected port #%d\n", line);

	port = msm_get_port_from_line(line);
	port->dev = &pdev->dev;
	msm_port = to_msm_port(port);

	id = of_match_device(msm_uartdm_table, &pdev->dev);
	if (id)
		msm_port->is_uartdm = (unsigned long)id->data;
	else
		msm_port->is_uartdm = 0;

	msm_port->clk = devm_clk_get(&pdev->dev, "core");
	if (IS_ERR(msm_port->clk))
		return PTR_ERR(msm_port->clk);

	if (msm_port->is_uartdm) {
		msm_port->pclk = devm_clk_get(&pdev->dev, "iface");
		if (IS_ERR(msm_port->pclk))
			return PTR_ERR(msm_port->pclk);
	}

	ret = devm_pm_opp_set_clkname(&pdev->dev, "core");
	if (ret)
		return ret;

	/* OPP table is optional */
	ret = devm_pm_opp_of_add_table(&pdev->dev);
	if (ret && ret != -ENODEV)
		return dev_err_probe(&pdev->dev, ret, "invalid OPP table\n");

	port->uartclk = clk_get_rate(msm_port->clk);
	dev_info(&pdev->dev, "uartclk = %d\n", port->uartclk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	port->mapbase = resource->start;

	irq = platform_get_irq(pdev, 0);
	if (unlikely(irq < 0))
		return -ENXIO;
	port->irq = irq;
<<<<<<< HEAD
=======
	port->has_sysrq = IS_ENABLED(CONFIG_SERIAL_MSM_CONSOLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	platform_set_drvdata(pdev, port);

	return uart_add_one_port(&msm_uart_driver, port);
}

<<<<<<< HEAD
static int __devexit msm_serial_remove(struct platform_device *pdev)
{
	struct msm_port *msm_port = platform_get_drvdata(pdev);

	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	clk_put(msm_port->clk);
=======
static void msm_serial_remove(struct platform_device *pdev)
{
	struct uart_port *port = platform_get_drvdata(pdev);

	uart_remove_one_port(&msm_uart_driver, port);
}

static const struct of_device_id msm_match_table[] = {
	{ .compatible = "qcom,msm-uart" },
	{ .compatible = "qcom,msm-uartdm" },
	{}
};
MODULE_DEVICE_TABLE(of, msm_match_table);

static int __maybe_unused msm_serial_suspend(struct device *dev)
{
	struct msm_port *port = dev_get_drvdata(dev);

	uart_suspend_port(&msm_uart_driver, &port->uart);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int msm_serial_suspend(struct device *dev)
{
	struct uart_port *port;
	struct platform_device *pdev = to_platform_device(dev);
	port = get_port_from_line(pdev->id);

	if (port) {
		uart_suspend_port(&msm_uart_driver, port);
		if (is_console(port))
			msm_deinit_clock(port);
	}
=======
static int __maybe_unused msm_serial_resume(struct device *dev)
{
	struct msm_port *port = dev_get_drvdata(dev);

	uart_resume_port(&msm_uart_driver, &port->uart);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int msm_serial_resume(struct device *dev)
{
	struct uart_port *port;
	struct platform_device *pdev = to_platform_device(dev);
	port = get_port_from_line(pdev->id);

	if (port) {
		if (is_console(port))
			msm_init_clock(port);
		uart_resume_port(&msm_uart_driver, port);
	}

	return 0;
}
#else
#define msm_serial_suspend NULL
#define msm_serial_resume NULL
#endif

static int msm_serial_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct uart_port *port;
	port = get_port_from_line(pdev->id);

	dev_dbg(dev, "pm_runtime: suspending\n");
	msm_deinit_clock(port);
	return 0;
}

static int msm_serial_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct uart_port *port;
	port = get_port_from_line(pdev->id);

	dev_dbg(dev, "pm_runtime: resuming\n");
	msm_init_clock(port);
	return 0;
}

static struct dev_pm_ops msm_serial_dev_pm_ops = {
	.suspend = msm_serial_suspend,
	.resume = msm_serial_resume,
	.runtime_suspend = msm_serial_runtime_suspend,
	.runtime_resume = msm_serial_runtime_resume,
};

static struct platform_driver msm_platform_driver = {
	.remove = msm_serial_remove,
	.driver = {
		.name = "msm_serial",
		.owner = THIS_MODULE,
		.pm = &msm_serial_dev_pm_ops,
	},
};

static struct platform_driver msm_platform_uim_driver = {
	.remove = msm_serial_remove,
	.driver = {
		.name = "msm_uim",
		.owner = THIS_MODULE,
=======
static const struct dev_pm_ops msm_serial_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(msm_serial_suspend, msm_serial_resume)
};

static struct platform_driver msm_platform_driver = {
	.remove_new = msm_serial_remove,
	.probe = msm_serial_probe,
	.driver = {
		.name = "msm_serial",
		.pm = &msm_serial_dev_pm_ops,
		.of_match_table = msm_match_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static int __init msm_serial_init(void)
{
	int ret;

	ret = uart_register_driver(&msm_uart_driver);
	if (unlikely(ret))
		return ret;

<<<<<<< HEAD
	ret = platform_driver_probe(&msm_platform_driver, msm_serial_probe);
	if (unlikely(ret))
		uart_unregister_driver(&msm_uart_driver);

	platform_driver_probe(&msm_platform_uim_driver, msm_uim_probe);

	printk(KERN_INFO "msm_serial: driver initialized\n");
=======
	ret = platform_driver_register(&msm_platform_driver);
	if (unlikely(ret))
		uart_unregister_driver(&msm_uart_driver);

	pr_info("msm_serial: driver initialized\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static void __exit msm_serial_exit(void)
{
<<<<<<< HEAD
#ifdef CONFIG_SERIAL_MSM_CONSOLE
	unregister_console(&msm_console);
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	platform_driver_unregister(&msm_platform_driver);
	uart_unregister_driver(&msm_uart_driver);
}

module_init(msm_serial_init);
module_exit(msm_serial_exit);

MODULE_AUTHOR("Robert Love <rlove@google.com>");
MODULE_DESCRIPTION("Driver for msm7x serial device");
<<<<<<< HEAD
MODULE_LICENSE("GPL v2");
=======
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
