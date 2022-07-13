<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/mach-lpc32xx/serial.c
 *
 * Author: Kevin Wells <kevin.wells@nxp.com>
 *
 * Copyright (C) 2010 NXP Semiconductors
<<<<<<< HEAD
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
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/serial_reg.h>
#include <linux/serial_8250.h>
#include <linux/clk.h>
#include <linux/io.h>
<<<<<<< HEAD

#include <mach/hardware.h>
#include <mach/platform.h>
=======
#include <linux/soc/nxp/lpc32xx-misc.h>

#include "lpc32xx.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "common.h"

#define LPC32XX_SUART_FIFO_SIZE	64

<<<<<<< HEAD
/* Standard 8250/16550 compatible serial ports */
static struct plat_serial8250_port serial_std_platform_data[] = {
#ifdef CONFIG_ARCH_LPC32XX_UART5_SELECT
	{
		.membase        = io_p2v(LPC32XX_UART5_BASE),
		.mapbase        = LPC32XX_UART5_BASE,
		.irq		= IRQ_LPC32XX_UART_IIR5,
		.uartclk	= LPC32XX_MAIN_OSC_FREQ,
		.regshift	= 2,
		.iotype		= UPIO_MEM32,
		.flags		= UPF_BOOT_AUTOCONF | UPF_BUGGY_UART |
					UPF_SKIP_TEST,
	},
#endif
#ifdef CONFIG_ARCH_LPC32XX_UART3_SELECT
	{
		.membase	= io_p2v(LPC32XX_UART3_BASE),
		.mapbase        = LPC32XX_UART3_BASE,
		.irq		= IRQ_LPC32XX_UART_IIR3,
		.uartclk	= LPC32XX_MAIN_OSC_FREQ,
		.regshift	= 2,
		.iotype		= UPIO_MEM32,
		.flags		= UPF_BOOT_AUTOCONF | UPF_BUGGY_UART |
					UPF_SKIP_TEST,
	},
#endif
#ifdef CONFIG_ARCH_LPC32XX_UART4_SELECT
	{
		.membase	= io_p2v(LPC32XX_UART4_BASE),
		.mapbase        = LPC32XX_UART4_BASE,
		.irq		= IRQ_LPC32XX_UART_IIR4,
		.uartclk	= LPC32XX_MAIN_OSC_FREQ,
		.regshift	= 2,
		.iotype		= UPIO_MEM32,
		.flags		= UPF_BOOT_AUTOCONF | UPF_BUGGY_UART |
					UPF_SKIP_TEST,
	},
#endif
#ifdef CONFIG_ARCH_LPC32XX_UART6_SELECT
	{
		.membase	= io_p2v(LPC32XX_UART6_BASE),
		.mapbase        = LPC32XX_UART6_BASE,
		.irq		= IRQ_LPC32XX_UART_IIR6,
		.uartclk	= LPC32XX_MAIN_OSC_FREQ,
		.regshift	= 2,
		.iotype		= UPIO_MEM32,
		.flags		= UPF_BOOT_AUTOCONF | UPF_BUGGY_UART |
					UPF_SKIP_TEST,
	},
#endif
	{ },
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct uartinit {
	char *uart_ck_name;
	u32 ck_mode_mask;
	void __iomem *pdiv_clk_reg;
	resource_size_t mapbase;
};

static struct uartinit uartinit_data[] __initdata = {
<<<<<<< HEAD
#ifdef CONFIG_ARCH_LPC32XX_UART5_SELECT
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.uart_ck_name = "uart5_ck",
		.ck_mode_mask =
			LPC32XX_UART_CLKMODE_LOAD(LPC32XX_UART_CLKMODE_ON, 5),
		.pdiv_clk_reg = LPC32XX_CLKPWR_UART5_CLK_CTRL,
		.mapbase = LPC32XX_UART5_BASE,
	},
<<<<<<< HEAD
#endif
#ifdef CONFIG_ARCH_LPC32XX_UART3_SELECT
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.uart_ck_name = "uart3_ck",
		.ck_mode_mask =
			LPC32XX_UART_CLKMODE_LOAD(LPC32XX_UART_CLKMODE_ON, 3),
		.pdiv_clk_reg = LPC32XX_CLKPWR_UART3_CLK_CTRL,
		.mapbase = LPC32XX_UART3_BASE,
	},
<<<<<<< HEAD
#endif
#ifdef CONFIG_ARCH_LPC32XX_UART4_SELECT
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.uart_ck_name = "uart4_ck",
		.ck_mode_mask =
			LPC32XX_UART_CLKMODE_LOAD(LPC32XX_UART_CLKMODE_ON, 4),
		.pdiv_clk_reg = LPC32XX_CLKPWR_UART4_CLK_CTRL,
		.mapbase = LPC32XX_UART4_BASE,
	},
<<<<<<< HEAD
#endif
#ifdef CONFIG_ARCH_LPC32XX_UART6_SELECT
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.uart_ck_name = "uart6_ck",
		.ck_mode_mask =
			LPC32XX_UART_CLKMODE_LOAD(LPC32XX_UART_CLKMODE_ON, 6),
		.pdiv_clk_reg = LPC32XX_CLKPWR_UART6_CLK_CTRL,
		.mapbase = LPC32XX_UART6_BASE,
	},
<<<<<<< HEAD
#endif
};

static struct platform_device serial_std_platform_device = {
	.name			= "serial8250",
	.id			= 0,
	.dev			= {
		.platform_data	= serial_std_platform_data,
	},
};

static struct platform_device *lpc32xx_serial_devs[] __initdata = {
	&serial_std_platform_device,
};
=======
};

/* LPC3250 Errata HSUART.1: Hang workaround via loopback mode on inactivity */
void lpc32xx_loopback_set(resource_size_t mapbase, int state)
{
	int bit;
	u32 tmp;

	switch (mapbase) {
	case LPC32XX_HS_UART1_BASE:
		bit = 0;
		break;
	case LPC32XX_HS_UART2_BASE:
		bit = 1;
		break;
	case LPC32XX_HS_UART7_BASE:
		bit = 6;
		break;
	default:
		WARN(1, "lpc32xx_hs: Warning: Unknown port at %08x\n", mapbase);
		return;
	}

	tmp = readl(LPC32XX_UARTCTL_CLOOP);
	if (state)
		tmp |= (1 << bit);
	else
		tmp &= ~(1 << bit);
	writel(tmp, LPC32XX_UARTCTL_CLOOP);
}
EXPORT_SYMBOL_GPL(lpc32xx_loopback_set);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void __init lpc32xx_serial_init(void)
{
	u32 tmp, clkmodes = 0;
	struct clk *clk;
	unsigned int puart;
	int i, j;

<<<<<<< HEAD
	/* UART clocks are off, let clock driver manage them */
	__raw_writel(0, LPC32XX_CLKPWR_UART_CLK_CTRL);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < ARRAY_SIZE(uartinit_data); i++) {
		clk = clk_get(NULL, uartinit_data[i].uart_ck_name);
		if (!IS_ERR(clk)) {
			clk_enable(clk);
<<<<<<< HEAD
			serial_std_platform_data[i].uartclk =
				clk_get_rate(clk);
		}

		/* Fall back on main osc rate if clock rate return fails */
		if (serial_std_platform_data[i].uartclk == 0)
			serial_std_platform_data[i].uartclk =
				LPC32XX_MAIN_OSC_FREQ;

=======
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Setup UART clock modes for all UARTs, disable autoclock */
		clkmodes |= uartinit_data[i].ck_mode_mask;

		/* pre-UART clock divider set to 1 */
		__raw_writel(0x0101, uartinit_data[i].pdiv_clk_reg);

		/*
		 * Force a flush of the RX FIFOs to work around a
		 * HW bug
		 */
		puart = uartinit_data[i].mapbase;
		__raw_writel(0xC1, LPC32XX_UART_IIR_FCR(puart));
		__raw_writel(0x00, LPC32XX_UART_DLL_FIFO(puart));
		j = LPC32XX_SUART_FIFO_SIZE;
		while (j--)
			tmp = __raw_readl(
				LPC32XX_UART_DLL_FIFO(puart));
		__raw_writel(0, LPC32XX_UART_IIR_FCR(puart));
	}

	/* This needs to be done after all UART clocks are setup */
	__raw_writel(clkmodes, LPC32XX_UARTCTL_CLKMODE);
	for (i = 0; i < ARRAY_SIZE(uartinit_data); i++) {
		/* Force a flush of the RX FIFOs to work around a HW bug */
<<<<<<< HEAD
		puart = serial_std_platform_data[i].mapbase;
=======
		puart = uartinit_data[i].mapbase;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__raw_writel(0xC1, LPC32XX_UART_IIR_FCR(puart));
		__raw_writel(0x00, LPC32XX_UART_DLL_FIFO(puart));
		j = LPC32XX_SUART_FIFO_SIZE;
		while (j--)
			tmp = __raw_readl(LPC32XX_UART_DLL_FIFO(puart));
		__raw_writel(0, LPC32XX_UART_IIR_FCR(puart));
	}

<<<<<<< HEAD
=======
	/* Disable IrDA pulsing support on UART6 */
	tmp = __raw_readl(LPC32XX_UARTCTL_CTRL);
	tmp |= LPC32XX_UART_UART6_IRDAMOD_BYPASS;
	__raw_writel(tmp, LPC32XX_UARTCTL_CTRL);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Disable UART5->USB transparent mode or USB won't work */
	tmp = __raw_readl(LPC32XX_UARTCTL_CTRL);
	tmp &= ~LPC32XX_UART_U5_ROUTE_TO_USB;
	__raw_writel(tmp, LPC32XX_UARTCTL_CTRL);
<<<<<<< HEAD

	platform_add_devices(lpc32xx_serial_devs,
		ARRAY_SIZE(lpc32xx_serial_devs));
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
