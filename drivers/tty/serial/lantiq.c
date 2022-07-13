<<<<<<< HEAD
/*
 *  Based on drivers/char/serial.c, by Linus Torvalds, Theodore Ts'o.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Copyright (C) 2004 Infineon IFAP DC COM CPE
 * Copyright (C) 2007 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2007 John Crispin <blogic@openwrt.org>
 * Copyright (C) 2010 Thomas Langer, <thomas.langer@lantiq.com>
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/clk.h>

#include <lantiq_soc.h>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *  Based on drivers/char/serial.c, by Linus Torvalds, Theodore Ts'o.
 *
 * Copyright (C) 2004 Infineon IFAP DC COM CPE
 * Copyright (C) 2007 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2007 John Crispin <john@phrozen.org>
 * Copyright (C) 2010 Thomas Langer, <thomas.langer@lantiq.com>
 */

#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/console.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/lantiq.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/slab.h>
#include <linux/sysrq.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define PORT_LTQ_ASC		111
#define MAXPORTS		2
#define UART_DUMMY_UER_RX	1
<<<<<<< HEAD
#define DRVNAME			"ltq_asc"
=======
#define DRVNAME			"lantiq,asc"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef __BIG_ENDIAN
#define LTQ_ASC_TBUF		(0x0020 + 3)
#define LTQ_ASC_RBUF		(0x0024 + 3)
#else
#define LTQ_ASC_TBUF		0x0020
#define LTQ_ASC_RBUF		0x0024
#endif
#define LTQ_ASC_FSTAT		0x0048
#define LTQ_ASC_WHBSTATE	0x0018
#define LTQ_ASC_STATE		0x0014
#define LTQ_ASC_IRNCR		0x00F8
#define LTQ_ASC_CLC		0x0000
#define LTQ_ASC_ID		0x0008
#define LTQ_ASC_PISEL		0x0004
#define LTQ_ASC_TXFCON		0x0044
#define LTQ_ASC_RXFCON		0x0040
#define LTQ_ASC_CON		0x0010
#define LTQ_ASC_BG		0x0050
#define LTQ_ASC_IRNREN		0x00F4

#define ASC_IRNREN_TX		0x1
#define ASC_IRNREN_RX		0x2
#define ASC_IRNREN_ERR		0x4
#define ASC_IRNREN_TX_BUF	0x8
#define ASC_IRNCR_TIR		0x1
#define ASC_IRNCR_RIR		0x2
#define ASC_IRNCR_EIR		0x4
<<<<<<< HEAD
=======
#define ASC_IRNCR_MASK		GENMASK(2, 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define ASCOPT_CSIZE		0x3
#define TXFIFO_FL		1
#define RXFIFO_FL		1
#define ASCCLC_DISS		0x2
#define ASCCLC_RMCMASK		0x0000FF00
#define ASCCLC_RMCOFFSET	8
#define ASCCON_M_8ASYNC		0x0
#define ASCCON_M_7ASYNC		0x2
#define ASCCON_ODD		0x00000020
#define ASCCON_STP		0x00000080
#define ASCCON_BRS		0x00000100
#define ASCCON_FDE		0x00000200
#define ASCCON_R		0x00008000
#define ASCCON_FEN		0x00020000
#define ASCCON_ROEN		0x00080000
#define ASCCON_TOEN		0x00100000
#define ASCSTATE_PE		0x00010000
#define ASCSTATE_FE		0x00020000
#define ASCSTATE_ROE		0x00080000
#define ASCSTATE_ANY		(ASCSTATE_ROE|ASCSTATE_PE|ASCSTATE_FE)
#define ASCWHBSTATE_CLRREN	0x00000001
#define ASCWHBSTATE_SETREN	0x00000002
#define ASCWHBSTATE_CLRPE	0x00000004
#define ASCWHBSTATE_CLRFE	0x00000008
#define ASCWHBSTATE_CLRROE	0x00000020
#define ASCTXFCON_TXFEN		0x0001
#define ASCTXFCON_TXFFLU	0x0002
#define ASCTXFCON_TXFITLMASK	0x3F00
#define ASCTXFCON_TXFITLOFF	8
#define ASCRXFCON_RXFEN		0x0001
#define ASCRXFCON_RXFFLU	0x0002
#define ASCRXFCON_RXFITLMASK	0x3F00
#define ASCRXFCON_RXFITLOFF	8
#define ASCFSTAT_RXFFLMASK	0x003F
#define ASCFSTAT_TXFFLMASK	0x3F00
#define ASCFSTAT_TXFREEMASK	0x3F000000
<<<<<<< HEAD
#define ASCFSTAT_TXFREEOFF	24

static void lqasc_tx_chars(struct uart_port *port);
static struct ltq_uart_port *lqasc_port[MAXPORTS];
static struct uart_driver lqasc_reg;
static DEFINE_SPINLOCK(ltq_asc_lock);

struct ltq_uart_port {
	struct uart_port	port;
=======

static struct ltq_uart_port *lqasc_port[MAXPORTS];
static struct uart_driver lqasc_reg;

struct ltq_soc_data {
	int	(*fetch_irq)(struct device *dev, struct ltq_uart_port *ltq_port);
	int	(*request_irq)(struct uart_port *port);
	void	(*free_irq)(struct uart_port *port);
};

struct ltq_uart_port {
	struct uart_port	port;
	/* clock used to derive divider */
	struct clk		*freqclk;
	/* clock gating of the ASC core */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct clk		*clk;
	unsigned int		tx_irq;
	unsigned int		rx_irq;
	unsigned int		err_irq;
<<<<<<< HEAD
};

=======
	unsigned int		common_irq;
	spinlock_t		lock; /* exclusive access for multi core */

	const struct ltq_soc_data	*soc;
};

static inline void asc_update_bits(u32 clear, u32 set, void __iomem *reg)
{
	u32 tmp = __raw_readl(reg);

	__raw_writel((tmp & ~clear) | set, reg);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct
ltq_uart_port *to_ltq_uart_port(struct uart_port *port)
{
	return container_of(port, struct ltq_uart_port, port);
}

static void
lqasc_stop_tx(struct uart_port *port)
{
	return;
}

<<<<<<< HEAD
=======
static bool lqasc_tx_ready(struct uart_port *port)
{
	u32 fstat = __raw_readl(port->membase + LTQ_ASC_FSTAT);

	return FIELD_GET(ASCFSTAT_TXFREEMASK, fstat);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void
lqasc_start_tx(struct uart_port *port)
{
	unsigned long flags;
<<<<<<< HEAD
	spin_lock_irqsave(&ltq_asc_lock, flags);
	lqasc_tx_chars(port);
	spin_unlock_irqrestore(&ltq_asc_lock, flags);
=======
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);
	u8 ch;

	spin_lock_irqsave(&ltq_port->lock, flags);
	uart_port_tx(port, ch,
		lqasc_tx_ready(port),
		writeb(ch, port->membase + LTQ_ASC_TBUF));
	spin_unlock_irqrestore(&ltq_port->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return;
}

static void
lqasc_stop_rx(struct uart_port *port)
{
<<<<<<< HEAD
	ltq_w32(ASCWHBSTATE_CLRREN, port->membase + LTQ_ASC_WHBSTATE);
}

static void
lqasc_enable_ms(struct uart_port *port)
{
=======
	__raw_writel(ASCWHBSTATE_CLRREN, port->membase + LTQ_ASC_WHBSTATE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
lqasc_rx_chars(struct uart_port *port)
{
<<<<<<< HEAD
	struct tty_struct *tty = tty_port_tty_get(&port->state->port);
	unsigned int ch = 0, rsr = 0, fifocnt;

	if (!tty) {
		dev_dbg(port->dev, "%s:tty is busy now", __func__);
		return -EBUSY;
	}
	fifocnt =
		ltq_r32(port->membase + LTQ_ASC_FSTAT) & ASCFSTAT_RXFFLMASK;
	while (fifocnt--) {
		u8 flag = TTY_NORMAL;
		ch = ltq_r8(port->membase + LTQ_ASC_RBUF);
		rsr = (ltq_r32(port->membase + LTQ_ASC_STATE)
			& ASCSTATE_ANY) | UART_DUMMY_UER_RX;
		tty_flip_buffer_push(tty);
=======
	struct tty_port *tport = &port->state->port;
	unsigned int ch = 0, rsr = 0, fifocnt;

	fifocnt = __raw_readl(port->membase + LTQ_ASC_FSTAT) &
		  ASCFSTAT_RXFFLMASK;
	while (fifocnt--) {
		u8 flag = TTY_NORMAL;
		ch = readb(port->membase + LTQ_ASC_RBUF);
		rsr = (__raw_readl(port->membase + LTQ_ASC_STATE)
			& ASCSTATE_ANY) | UART_DUMMY_UER_RX;
		tty_flip_buffer_push(tport);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		port->icount.rx++;

		/*
		 * Note that the error handling code is
		 * out of the main execution path
		 */
		if (rsr & ASCSTATE_ANY) {
			if (rsr & ASCSTATE_PE) {
				port->icount.parity++;
<<<<<<< HEAD
				ltq_w32_mask(0, ASCWHBSTATE_CLRPE,
					port->membase + LTQ_ASC_WHBSTATE);
			} else if (rsr & ASCSTATE_FE) {
				port->icount.frame++;
				ltq_w32_mask(0, ASCWHBSTATE_CLRFE,
=======
				asc_update_bits(0, ASCWHBSTATE_CLRPE,
					port->membase + LTQ_ASC_WHBSTATE);
			} else if (rsr & ASCSTATE_FE) {
				port->icount.frame++;
				asc_update_bits(0, ASCWHBSTATE_CLRFE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					port->membase + LTQ_ASC_WHBSTATE);
			}
			if (rsr & ASCSTATE_ROE) {
				port->icount.overrun++;
<<<<<<< HEAD
				ltq_w32_mask(0, ASCWHBSTATE_CLRROE,
=======
				asc_update_bits(0, ASCWHBSTATE_CLRROE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					port->membase + LTQ_ASC_WHBSTATE);
			}

			rsr &= port->read_status_mask;

			if (rsr & ASCSTATE_PE)
				flag = TTY_PARITY;
			else if (rsr & ASCSTATE_FE)
				flag = TTY_FRAME;
		}

		if ((rsr & port->ignore_status_mask) == 0)
<<<<<<< HEAD
			tty_insert_flip_char(tty, ch, flag);
=======
			tty_insert_flip_char(tport, ch, flag);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (rsr & ASCSTATE_ROE)
			/*
			 * Overrun is special, since it's reported
			 * immediately, and doesn't affect the current
			 * character
			 */
<<<<<<< HEAD
			tty_insert_flip_char(tty, 0, TTY_OVERRUN);
	}
	if (ch != 0)
		tty_flip_buffer_push(tty);
	tty_kref_put(tty);
	return 0;
}

static void
lqasc_tx_chars(struct uart_port *port)
{
	struct circ_buf *xmit = &port->state->xmit;
	if (uart_tx_stopped(port)) {
		lqasc_stop_tx(port);
		return;
	}

	while (((ltq_r32(port->membase + LTQ_ASC_FSTAT) &
		ASCFSTAT_TXFREEMASK) >> ASCFSTAT_TXFREEOFF) != 0) {
		if (port->x_char) {
			ltq_w8(port->x_char, port->membase + LTQ_ASC_TBUF);
			port->icount.tx++;
			port->x_char = 0;
			continue;
		}

		if (uart_circ_empty(xmit))
			break;

		ltq_w8(port->state->xmit.buf[port->state->xmit.tail],
			port->membase + LTQ_ASC_TBUF);
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
	}

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);
}

=======
			tty_insert_flip_char(tport, 0, TTY_OVERRUN);
	}

	if (ch != 0)
		tty_flip_buffer_push(tport);

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static irqreturn_t
lqasc_tx_int(int irq, void *_port)
{
	unsigned long flags;
	struct uart_port *port = (struct uart_port *)_port;
<<<<<<< HEAD
	spin_lock_irqsave(&ltq_asc_lock, flags);
	ltq_w32(ASC_IRNCR_TIR, port->membase + LTQ_ASC_IRNCR);
	spin_unlock_irqrestore(&ltq_asc_lock, flags);
=======
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);

	spin_lock_irqsave(&ltq_port->lock, flags);
	__raw_writel(ASC_IRNCR_TIR, port->membase + LTQ_ASC_IRNCR);
	spin_unlock_irqrestore(&ltq_port->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lqasc_start_tx(port);
	return IRQ_HANDLED;
}

static irqreturn_t
lqasc_err_int(int irq, void *_port)
{
	unsigned long flags;
	struct uart_port *port = (struct uart_port *)_port;
<<<<<<< HEAD
	spin_lock_irqsave(&ltq_asc_lock, flags);
	/* clear any pending interrupts */
	ltq_w32_mask(0, ASCWHBSTATE_CLRPE | ASCWHBSTATE_CLRFE |
		ASCWHBSTATE_CLRROE, port->membase + LTQ_ASC_WHBSTATE);
	spin_unlock_irqrestore(&ltq_asc_lock, flags);
=======
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);

	spin_lock_irqsave(&ltq_port->lock, flags);
	__raw_writel(ASC_IRNCR_EIR, port->membase + LTQ_ASC_IRNCR);
	/* clear any pending interrupts */
	asc_update_bits(0, ASCWHBSTATE_CLRPE | ASCWHBSTATE_CLRFE |
		ASCWHBSTATE_CLRROE, port->membase + LTQ_ASC_WHBSTATE);
	spin_unlock_irqrestore(&ltq_port->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return IRQ_HANDLED;
}

static irqreturn_t
lqasc_rx_int(int irq, void *_port)
{
	unsigned long flags;
	struct uart_port *port = (struct uart_port *)_port;
<<<<<<< HEAD
	spin_lock_irqsave(&ltq_asc_lock, flags);
	ltq_w32(ASC_IRNCR_RIR, port->membase + LTQ_ASC_IRNCR);
	lqasc_rx_chars(port);
	spin_unlock_irqrestore(&ltq_asc_lock, flags);
=======
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);

	spin_lock_irqsave(&ltq_port->lock, flags);
	__raw_writel(ASC_IRNCR_RIR, port->membase + LTQ_ASC_IRNCR);
	lqasc_rx_chars(port);
	spin_unlock_irqrestore(&ltq_port->lock, flags);
	return IRQ_HANDLED;
}

static irqreturn_t lqasc_irq(int irq, void *p)
{
	unsigned long flags;
	u32 stat;
	struct uart_port *port = p;
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);

	spin_lock_irqsave(&ltq_port->lock, flags);
	stat = readl(port->membase + LTQ_ASC_IRNCR);
	spin_unlock_irqrestore(&ltq_port->lock, flags);
	if (!(stat & ASC_IRNCR_MASK))
		return IRQ_NONE;

	if (stat & ASC_IRNCR_TIR)
		lqasc_tx_int(irq, p);

	if (stat & ASC_IRNCR_RIR)
		lqasc_rx_int(irq, p);

	if (stat & ASC_IRNCR_EIR)
		lqasc_err_int(irq, p);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return IRQ_HANDLED;
}

static unsigned int
lqasc_tx_empty(struct uart_port *port)
{
	int status;
<<<<<<< HEAD
	status = ltq_r32(port->membase + LTQ_ASC_FSTAT) & ASCFSTAT_TXFFLMASK;
=======
	status = __raw_readl(port->membase + LTQ_ASC_FSTAT) &
		 ASCFSTAT_TXFFLMASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return status ? 0 : TIOCSER_TEMT;
}

static unsigned int
lqasc_get_mctrl(struct uart_port *port)
{
	return TIOCM_CTS | TIOCM_CAR | TIOCM_DSR;
}

static void
lqasc_set_mctrl(struct uart_port *port, u_int mctrl)
{
}

static void
lqasc_break_ctl(struct uart_port *port, int break_state)
{
}

static int
lqasc_startup(struct uart_port *port)
{
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);
	int retval;
<<<<<<< HEAD

	port->uartclk = clk_get_rate(ltq_port->clk);

	ltq_w32_mask(ASCCLC_DISS | ASCCLC_RMCMASK, (1 << ASCCLC_RMCOFFSET),
		port->membase + LTQ_ASC_CLC);

	ltq_w32(0, port->membase + LTQ_ASC_PISEL);
	ltq_w32(
		((TXFIFO_FL << ASCTXFCON_TXFITLOFF) & ASCTXFCON_TXFITLMASK) |
		ASCTXFCON_TXFEN | ASCTXFCON_TXFFLU,
		port->membase + LTQ_ASC_TXFCON);
	ltq_w32(
=======
	unsigned long flags;

	if (!IS_ERR(ltq_port->clk))
		clk_prepare_enable(ltq_port->clk);
	port->uartclk = clk_get_rate(ltq_port->freqclk);

	spin_lock_irqsave(&ltq_port->lock, flags);
	asc_update_bits(ASCCLC_DISS | ASCCLC_RMCMASK, (1 << ASCCLC_RMCOFFSET),
		port->membase + LTQ_ASC_CLC);

	__raw_writel(0, port->membase + LTQ_ASC_PISEL);
	__raw_writel(
		((TXFIFO_FL << ASCTXFCON_TXFITLOFF) & ASCTXFCON_TXFITLMASK) |
		ASCTXFCON_TXFEN | ASCTXFCON_TXFFLU,
		port->membase + LTQ_ASC_TXFCON);
	__raw_writel(
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		((RXFIFO_FL << ASCRXFCON_RXFITLOFF) & ASCRXFCON_RXFITLMASK)
		| ASCRXFCON_RXFEN | ASCRXFCON_RXFFLU,
		port->membase + LTQ_ASC_RXFCON);
	/* make sure other settings are written to hardware before
	 * setting enable bits
	 */
	wmb();
<<<<<<< HEAD
	ltq_w32_mask(0, ASCCON_M_8ASYNC | ASCCON_FEN | ASCCON_TOEN |
		ASCCON_ROEN, port->membase + LTQ_ASC_CON);

	retval = request_irq(ltq_port->tx_irq, lqasc_tx_int,
		0, "asc_tx", port);
	if (retval) {
		pr_err("failed to request lqasc_tx_int\n");
		return retval;
	}

	retval = request_irq(ltq_port->rx_irq, lqasc_rx_int,
		0, "asc_rx", port);
	if (retval) {
		pr_err("failed to request lqasc_rx_int\n");
		goto err1;
	}

	retval = request_irq(ltq_port->err_irq, lqasc_err_int,
		0, "asc_err", port);
	if (retval) {
		pr_err("failed to request lqasc_err_int\n");
		goto err2;
	}

	ltq_w32(ASC_IRNREN_RX | ASC_IRNREN_ERR | ASC_IRNREN_TX,
		port->membase + LTQ_ASC_IRNREN);
	return 0;

err2:
	free_irq(ltq_port->rx_irq, port);
err1:
	free_irq(ltq_port->tx_irq, port);
=======
	asc_update_bits(0, ASCCON_M_8ASYNC | ASCCON_FEN | ASCCON_TOEN |
		ASCCON_ROEN, port->membase + LTQ_ASC_CON);

	spin_unlock_irqrestore(&ltq_port->lock, flags);

	retval = ltq_port->soc->request_irq(port);
	if (retval)
		return retval;

	__raw_writel(ASC_IRNREN_RX | ASC_IRNREN_ERR | ASC_IRNREN_TX,
		port->membase + LTQ_ASC_IRNREN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

static void
lqasc_shutdown(struct uart_port *port)
{
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);
<<<<<<< HEAD
	free_irq(ltq_port->tx_irq, port);
	free_irq(ltq_port->rx_irq, port);
	free_irq(ltq_port->err_irq, port);

	ltq_w32(0, port->membase + LTQ_ASC_CON);
	ltq_w32_mask(ASCRXFCON_RXFEN, ASCRXFCON_RXFFLU,
		port->membase + LTQ_ASC_RXFCON);
	ltq_w32_mask(ASCTXFCON_TXFEN, ASCTXFCON_TXFFLU,
		port->membase + LTQ_ASC_TXFCON);
}

static void
lqasc_set_termios(struct uart_port *port,
	struct ktermios *new, struct ktermios *old)
=======
	unsigned long flags;

	ltq_port->soc->free_irq(port);

	spin_lock_irqsave(&ltq_port->lock, flags);
	__raw_writel(0, port->membase + LTQ_ASC_CON);
	asc_update_bits(ASCRXFCON_RXFEN, ASCRXFCON_RXFFLU,
		port->membase + LTQ_ASC_RXFCON);
	asc_update_bits(ASCTXFCON_TXFEN, ASCTXFCON_TXFFLU,
		port->membase + LTQ_ASC_TXFCON);
	spin_unlock_irqrestore(&ltq_port->lock, flags);
	if (!IS_ERR(ltq_port->clk))
		clk_disable_unprepare(ltq_port->clk);
}

static void
lqasc_set_termios(struct uart_port *port, struct ktermios *new,
		  const struct ktermios *old)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int cflag;
	unsigned int iflag;
	unsigned int divisor;
	unsigned int baud;
	unsigned int con = 0;
	unsigned long flags;
<<<<<<< HEAD
=======
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cflag = new->c_cflag;
	iflag = new->c_iflag;

	switch (cflag & CSIZE) {
	case CS7:
		con = ASCCON_M_7ASYNC;
		break;

	case CS5:
	case CS6:
	default:
		new->c_cflag &= ~ CSIZE;
		new->c_cflag |= CS8;
		con = ASCCON_M_8ASYNC;
		break;
	}

	cflag &= ~CMSPAR; /* Mark/Space parity is not supported */

	if (cflag & CSTOPB)
		con |= ASCCON_STP;

	if (cflag & PARENB) {
		if (!(cflag & PARODD))
			con &= ~ASCCON_ODD;
		else
			con |= ASCCON_ODD;
	}

	port->read_status_mask = ASCSTATE_ROE;
	if (iflag & INPCK)
		port->read_status_mask |= ASCSTATE_FE | ASCSTATE_PE;

	port->ignore_status_mask = 0;
	if (iflag & IGNPAR)
		port->ignore_status_mask |= ASCSTATE_FE | ASCSTATE_PE;

	if (iflag & IGNBRK) {
		/*
		 * If we're ignoring parity and break indicators,
		 * ignore overruns too (for real raw support).
		 */
		if (iflag & IGNPAR)
			port->ignore_status_mask |= ASCSTATE_ROE;
	}

	if ((cflag & CREAD) == 0)
		port->ignore_status_mask |= UART_DUMMY_UER_RX;

	/* set error signals  - framing, parity  and overrun, enable receiver */
	con |= ASCCON_FEN | ASCCON_TOEN | ASCCON_ROEN;

<<<<<<< HEAD
	spin_lock_irqsave(&ltq_asc_lock, flags);

	/* set up CON */
	ltq_w32_mask(0, con, port->membase + LTQ_ASC_CON);
=======
	spin_lock_irqsave(&ltq_port->lock, flags);

	/* set up CON */
	asc_update_bits(0, con, port->membase + LTQ_ASC_CON);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set baud rate - take a divider of 2 into account */
	baud = uart_get_baud_rate(port, new, old, 0, port->uartclk / 16);
	divisor = uart_get_divisor(port, baud);
	divisor = divisor / 2 - 1;

	/* disable the baudrate generator */
<<<<<<< HEAD
	ltq_w32_mask(ASCCON_R, 0, port->membase + LTQ_ASC_CON);

	/* make sure the fractional divider is off */
	ltq_w32_mask(ASCCON_FDE, 0, port->membase + LTQ_ASC_CON);

	/* set up to use divisor of 2 */
	ltq_w32_mask(ASCCON_BRS, 0, port->membase + LTQ_ASC_CON);

	/* now we can write the new baudrate into the register */
	ltq_w32(divisor, port->membase + LTQ_ASC_BG);

	/* turn the baudrate generator back on */
	ltq_w32_mask(0, ASCCON_R, port->membase + LTQ_ASC_CON);

	/* enable rx */
	ltq_w32(ASCWHBSTATE_SETREN, port->membase + LTQ_ASC_WHBSTATE);

	spin_unlock_irqrestore(&ltq_asc_lock, flags);
=======
	asc_update_bits(ASCCON_R, 0, port->membase + LTQ_ASC_CON);

	/* make sure the fractional divider is off */
	asc_update_bits(ASCCON_FDE, 0, port->membase + LTQ_ASC_CON);

	/* set up to use divisor of 2 */
	asc_update_bits(ASCCON_BRS, 0, port->membase + LTQ_ASC_CON);

	/* now we can write the new baudrate into the register */
	__raw_writel(divisor, port->membase + LTQ_ASC_BG);

	/* turn the baudrate generator back on */
	asc_update_bits(0, ASCCON_R, port->membase + LTQ_ASC_CON);

	/* enable rx */
	__raw_writel(ASCWHBSTATE_SETREN, port->membase + LTQ_ASC_WHBSTATE);

	spin_unlock_irqrestore(&ltq_port->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Don't rewrite B0 */
	if (tty_termios_baud_rate(new))
		tty_termios_encode_baud_rate(new, baud, baud);

	uart_update_timeout(port, cflag, baud);
}

static const char*
lqasc_type(struct uart_port *port)
{
	if (port->type == PORT_LTQ_ASC)
		return DRVNAME;
	else
		return NULL;
}

static void
lqasc_release_port(struct uart_port *port)
{
<<<<<<< HEAD
	if (port->flags & UPF_IOREMAP) {
		iounmap(port->membase);
=======
	struct platform_device *pdev = to_platform_device(port->dev);

	if (port->flags & UPF_IOREMAP) {
		devm_iounmap(&pdev->dev, port->membase);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		port->membase = NULL;
	}
}

static int
lqasc_request_port(struct uart_port *port)
{
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *res;
	int size;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "cannot obtain I/O memory region");
		return -ENODEV;
	}
	size = resource_size(res);

	res = devm_request_mem_region(&pdev->dev, res->start,
		size, dev_name(&pdev->dev));
	if (!res) {
		dev_err(&pdev->dev, "cannot request I/O memory region");
		return -EBUSY;
	}

	if (port->flags & UPF_IOREMAP) {
<<<<<<< HEAD
		port->membase = devm_ioremap_nocache(&pdev->dev,
=======
		port->membase = devm_ioremap(&pdev->dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			port->mapbase, size);
		if (port->membase == NULL)
			return -ENOMEM;
	}
	return 0;
}

static void
lqasc_config_port(struct uart_port *port, int flags)
{
	if (flags & UART_CONFIG_TYPE) {
		port->type = PORT_LTQ_ASC;
		lqasc_request_port(port);
	}
}

static int
lqasc_verify_port(struct uart_port *port,
	struct serial_struct *ser)
{
	int ret = 0;
	if (ser->type != PORT_UNKNOWN && ser->type != PORT_LTQ_ASC)
		ret = -EINVAL;
	if (ser->irq < 0 || ser->irq >= NR_IRQS)
		ret = -EINVAL;
	if (ser->baud_base < 9600)
		ret = -EINVAL;
	return ret;
}

<<<<<<< HEAD
static struct uart_ops lqasc_pops = {
=======
static const struct uart_ops lqasc_pops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.tx_empty =	lqasc_tx_empty,
	.set_mctrl =	lqasc_set_mctrl,
	.get_mctrl =	lqasc_get_mctrl,
	.stop_tx =	lqasc_stop_tx,
	.start_tx =	lqasc_start_tx,
	.stop_rx =	lqasc_stop_rx,
<<<<<<< HEAD
	.enable_ms =	lqasc_enable_ms,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.break_ctl =	lqasc_break_ctl,
	.startup =	lqasc_startup,
	.shutdown =	lqasc_shutdown,
	.set_termios =	lqasc_set_termios,
	.type =		lqasc_type,
	.release_port =	lqasc_release_port,
	.request_port =	lqasc_request_port,
	.config_port =	lqasc_config_port,
	.verify_port =	lqasc_verify_port,
};

<<<<<<< HEAD
static void
lqasc_console_putchar(struct uart_port *port, int ch)
{
	int fifofree;

	if (!port->membase)
		return;

	do {
		fifofree = (ltq_r32(port->membase + LTQ_ASC_FSTAT)
			& ASCFSTAT_TXFREEMASK) >> ASCFSTAT_TXFREEOFF;
	} while (fifofree == 0);
	ltq_w8(ch, port->membase + LTQ_ASC_TBUF);
}

=======
#ifdef CONFIG_SERIAL_LANTIQ_CONSOLE
static void
lqasc_console_putchar(struct uart_port *port, unsigned char ch)
{
	if (!port->membase)
		return;

	while (!lqasc_tx_ready(port))
		;

	writeb(ch, port->membase + LTQ_ASC_TBUF);
}

static void lqasc_serial_port_write(struct uart_port *port, const char *s,
				    u_int count)
{
	uart_console_write(port, s, count, lqasc_console_putchar);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void
lqasc_console_write(struct console *co, const char *s, u_int count)
{
	struct ltq_uart_port *ltq_port;
<<<<<<< HEAD
	struct uart_port *port;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;

	if (co->index >= MAXPORTS)
		return;

	ltq_port = lqasc_port[co->index];
	if (!ltq_port)
		return;

<<<<<<< HEAD
	port = &ltq_port->port;

	spin_lock_irqsave(&ltq_asc_lock, flags);
	uart_console_write(port, s, count, lqasc_console_putchar);
	spin_unlock_irqrestore(&ltq_asc_lock, flags);
=======
	spin_lock_irqsave(&ltq_port->lock, flags);
	lqasc_serial_port_write(&ltq_port->port, s, count);
	spin_unlock_irqrestore(&ltq_port->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init
lqasc_console_setup(struct console *co, char *options)
{
	struct ltq_uart_port *ltq_port;
	struct uart_port *port;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	if (co->index >= MAXPORTS)
		return -ENODEV;

	ltq_port = lqasc_port[co->index];
	if (!ltq_port)
		return -ENODEV;

	port = &ltq_port->port;

<<<<<<< HEAD
	port->uartclk = clk_get_rate(ltq_port->clk);
=======
	if (!IS_ERR(ltq_port->clk))
		clk_prepare_enable(ltq_port->clk);

	port->uartclk = clk_get_rate(ltq_port->freqclk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct console lqasc_console = {
	.name =		"ttyLTQ",
	.write =	lqasc_console_write,
	.device =	uart_console_device,
	.setup =	lqasc_console_setup,
	.flags =	CON_PRINTBUFFER,
	.index =	-1,
	.data =		&lqasc_reg,
};

static int __init
lqasc_console_init(void)
{
	register_console(&lqasc_console);
	return 0;
}
console_initcall(lqasc_console_init);

<<<<<<< HEAD
=======
static void lqasc_serial_early_console_write(struct console *co,
					     const char *s,
					     u_int count)
{
	struct earlycon_device *dev = co->data;

	lqasc_serial_port_write(&dev->port, s, count);
}

static int __init
lqasc_serial_early_console_setup(struct earlycon_device *device,
				 const char *opt)
{
	if (!device->port.membase)
		return -ENODEV;

	device->con->write = lqasc_serial_early_console_write;
	return 0;
}
OF_EARLYCON_DECLARE(lantiq, "lantiq,asc", lqasc_serial_early_console_setup);
OF_EARLYCON_DECLARE(lantiq, "intel,lgm-asc", lqasc_serial_early_console_setup);

#define LANTIQ_SERIAL_CONSOLE	(&lqasc_console)

#else

#define LANTIQ_SERIAL_CONSOLE	NULL

#endif /* CONFIG_SERIAL_LANTIQ_CONSOLE */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct uart_driver lqasc_reg = {
	.owner =	THIS_MODULE,
	.driver_name =	DRVNAME,
	.dev_name =	"ttyLTQ",
	.major =	0,
	.minor =	0,
	.nr =		MAXPORTS,
<<<<<<< HEAD
	.cons =		&lqasc_console,
};

static int __init
lqasc_probe(struct platform_device *pdev)
{
	struct ltq_uart_port *ltq_port;
	struct uart_port *port;
	struct resource *mmres, *irqres;
	int tx_irq, rx_irq, err_irq;
	struct clk *clk;
	int ret;

	mmres = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irqres = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!mmres || !irqres)
		return -ENODEV;

	if (pdev->id >= MAXPORTS)
		return -EBUSY;

	if (lqasc_port[pdev->id] != NULL)
		return -EBUSY;

	clk = clk_get(&pdev->dev, "fpi");
	if (IS_ERR(clk)) {
		pr_err("failed to get fpi clk\n");
		return -ENOENT;
	}

	tx_irq = platform_get_irq_byname(pdev, "tx");
	rx_irq = platform_get_irq_byname(pdev, "rx");
	err_irq = platform_get_irq_byname(pdev, "err");
	if ((tx_irq < 0) | (rx_irq < 0) | (err_irq < 0))
		return -ENODEV;

	ltq_port = kzalloc(sizeof(struct ltq_uart_port), GFP_KERNEL);
=======
	.cons =		LANTIQ_SERIAL_CONSOLE,
};

static int fetch_irq_lantiq(struct device *dev, struct ltq_uart_port *ltq_port)
{
	struct uart_port *port = &ltq_port->port;
	struct platform_device *pdev = to_platform_device(dev);
	int irq;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;
	ltq_port->tx_irq = irq;
	irq = platform_get_irq(pdev, 1);
	if (irq < 0)
		return irq;
	ltq_port->rx_irq = irq;
	irq = platform_get_irq(pdev, 2);
	if (irq < 0)
		return irq;
	ltq_port->err_irq = irq;

	port->irq = ltq_port->tx_irq;

	return 0;
}

static int request_irq_lantiq(struct uart_port *port)
{
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);
	int retval;

	retval = request_irq(ltq_port->tx_irq, lqasc_tx_int,
			     0, "asc_tx", port);
	if (retval) {
		dev_err(port->dev, "failed to request asc_tx\n");
		return retval;
	}

	retval = request_irq(ltq_port->rx_irq, lqasc_rx_int,
			     0, "asc_rx", port);
	if (retval) {
		dev_err(port->dev, "failed to request asc_rx\n");
		goto err1;
	}

	retval = request_irq(ltq_port->err_irq, lqasc_err_int,
			     0, "asc_err", port);
	if (retval) {
		dev_err(port->dev, "failed to request asc_err\n");
		goto err2;
	}
	return 0;

err2:
	free_irq(ltq_port->rx_irq, port);
err1:
	free_irq(ltq_port->tx_irq, port);
	return retval;
}

static void free_irq_lantiq(struct uart_port *port)
{
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);

	free_irq(ltq_port->tx_irq, port);
	free_irq(ltq_port->rx_irq, port);
	free_irq(ltq_port->err_irq, port);
}

static int fetch_irq_intel(struct device *dev, struct ltq_uart_port *ltq_port)
{
	struct uart_port *port = &ltq_port->port;
	int ret;

	ret = platform_get_irq(to_platform_device(dev), 0);
	if (ret < 0) {
		dev_err(dev, "failed to fetch IRQ for serial port\n");
		return ret;
	}
	ltq_port->common_irq = ret;
	port->irq = ret;

	return 0;
}

static int request_irq_intel(struct uart_port *port)
{
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);
	int retval;

	retval = request_irq(ltq_port->common_irq, lqasc_irq, 0,
			     "asc_irq", port);
	if (retval)
		dev_err(port->dev, "failed to request asc_irq\n");

	return retval;
}

static void free_irq_intel(struct uart_port *port)
{
	struct ltq_uart_port *ltq_port = to_ltq_uart_port(port);

	free_irq(ltq_port->common_irq, port);
}

static int lqasc_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct ltq_uart_port *ltq_port;
	struct uart_port *port;
	struct resource *mmres;
	int line;
	int ret;

	mmres = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mmres) {
		dev_err(&pdev->dev,
			"failed to get memory for serial port\n");
		return -ENODEV;
	}

	ltq_port = devm_kzalloc(&pdev->dev, sizeof(struct ltq_uart_port),
				GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ltq_port)
		return -ENOMEM;

	port = &ltq_port->port;

<<<<<<< HEAD
	port->iotype	= SERIAL_IO_MEM;
	port->flags	= ASYNC_BOOT_AUTOCONF | UPF_IOREMAP;
	port->ops	= &lqasc_pops;
	port->fifosize	= 16;
	port->type	= PORT_LTQ_ASC,
	port->line	= pdev->id;
	port->dev	= &pdev->dev;

	port->irq	= tx_irq; /* unused, just to be backward-compatibe */
	port->mapbase	= mmres->start;

	ltq_port->clk	= clk;

	ltq_port->tx_irq = tx_irq;
	ltq_port->rx_irq = rx_irq;
	ltq_port->err_irq = err_irq;

	lqasc_port[pdev->id] = ltq_port;
=======
	ltq_port->soc = of_device_get_match_data(&pdev->dev);
	ret = ltq_port->soc->fetch_irq(&pdev->dev, ltq_port);
	if (ret)
		return ret;

	/* get serial id */
	line = of_alias_get_id(node, "serial");
	if (line < 0) {
		if (IS_ENABLED(CONFIG_LANTIQ)) {
			if (mmres->start == CPHYSADDR(LTQ_EARLY_ASC))
				line = 0;
			else
				line = 1;
		} else {
			dev_err(&pdev->dev, "failed to get alias id, errno %d\n",
				line);
			return line;
		}
	}

	if (lqasc_port[line]) {
		dev_err(&pdev->dev, "port %d already allocated\n", line);
		return -EBUSY;
	}

	port->iotype	= SERIAL_IO_MEM;
	port->flags	= UPF_BOOT_AUTOCONF | UPF_IOREMAP;
	port->ops	= &lqasc_pops;
	port->fifosize	= 16;
	port->type	= PORT_LTQ_ASC;
	port->line	= line;
	port->dev	= &pdev->dev;
	/* unused, just to be backward-compatible */
	port->mapbase	= mmres->start;

	if (IS_ENABLED(CONFIG_LANTIQ) && !IS_ENABLED(CONFIG_COMMON_CLK))
		ltq_port->freqclk = clk_get_fpi();
	else
		ltq_port->freqclk = devm_clk_get(&pdev->dev, "freq");


	if (IS_ERR(ltq_port->freqclk)) {
		pr_err("failed to get fpi clk\n");
		return -ENOENT;
	}

	/* not all asc ports have clock gates, lets ignore the return code */
	if (IS_ENABLED(CONFIG_LANTIQ) && !IS_ENABLED(CONFIG_COMMON_CLK))
		ltq_port->clk = clk_get(&pdev->dev, NULL);
	else
		ltq_port->clk = devm_clk_get(&pdev->dev, "asc");

	spin_lock_init(&ltq_port->lock);
	lqasc_port[line] = ltq_port;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	platform_set_drvdata(pdev, ltq_port);

	ret = uart_add_one_port(&lqasc_reg, port);

	return ret;
}

<<<<<<< HEAD
static struct platform_driver lqasc_driver = {
	.driver		= {
		.name	= DRVNAME,
		.owner	= THIS_MODULE,
	},
};

int __init
=======
static void lqasc_remove(struct platform_device *pdev)
{
	struct uart_port *port = platform_get_drvdata(pdev);

	uart_remove_one_port(&lqasc_reg, port);
}

static const struct ltq_soc_data soc_data_lantiq = {
	.fetch_irq = fetch_irq_lantiq,
	.request_irq = request_irq_lantiq,
	.free_irq = free_irq_lantiq,
};

static const struct ltq_soc_data soc_data_intel = {
	.fetch_irq = fetch_irq_intel,
	.request_irq = request_irq_intel,
	.free_irq = free_irq_intel,
};

static const struct of_device_id ltq_asc_match[] = {
	{ .compatible = "lantiq,asc", .data = &soc_data_lantiq },
	{ .compatible = "intel,lgm-asc", .data = &soc_data_intel },
	{},
};
MODULE_DEVICE_TABLE(of, ltq_asc_match);

static struct platform_driver lqasc_driver = {
	.probe		= lqasc_probe,
	.remove_new	= lqasc_remove,
	.driver		= {
		.name	= DRVNAME,
		.of_match_table = ltq_asc_match,
	},
};

static int __init
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
init_lqasc(void)
{
	int ret;

	ret = uart_register_driver(&lqasc_reg);
	if (ret != 0)
		return ret;

<<<<<<< HEAD
	ret = platform_driver_probe(&lqasc_driver, lqasc_probe);
=======
	ret = platform_driver_register(&lqasc_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret != 0)
		uart_unregister_driver(&lqasc_reg);

	return ret;
}

<<<<<<< HEAD
module_init(init_lqasc);

MODULE_DESCRIPTION("Lantiq serial port driver");
MODULE_LICENSE("GPL");
=======
static void __exit exit_lqasc(void)
{
	platform_driver_unregister(&lqasc_driver);
	uart_unregister_driver(&lqasc_reg);
}

module_init(init_lqasc);
module_exit(exit_lqasc);

MODULE_DESCRIPTION("Serial driver for Lantiq & Intel gateway SoCs");
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
