/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2008 Maxime Bizon <mbizon@freebox.fr>
 */

<<<<<<< HEAD
#include <linux/init.h>
#include <bcm63xx_io.h>
#include <bcm63xx_regs.h>

static void __init wait_xfered(void)
=======
#include <bcm63xx_io.h>
#include <linux/serial_bcm63xx.h>
#include <asm/setup.h>

static void wait_xfered(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int val;

	/* wait for any previous char to be transmitted */
	do {
		val = bcm_uart0_readl(UART_IR_REG);
		if (val & UART_IR_STAT(UART_IR_TXEMPTY))
			break;
	} while (1);
}

<<<<<<< HEAD
void __init prom_putchar(char c)
=======
void prom_putchar(char c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	wait_xfered();
	bcm_uart0_writel(c, UART_FIFO_REG);
	wait_xfered();
}
