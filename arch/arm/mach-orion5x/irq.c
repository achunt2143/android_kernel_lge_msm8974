<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/mach-orion5x/irq.c
 *
 * Core IRQ functions for Marvell Orion System On Chip
 *
 * Maintainer: Tzachi Perelstein <tzachi@marvell.com>
<<<<<<< HEAD
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <mach/bridge-regs.h>
#include <plat/irq.h>
#include "common.h"

static void gpio_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	BUG_ON(irq < IRQ_ORION5X_GPIO_0_7 || irq > IRQ_ORION5X_GPIO_24_31);

	orion_gpio_irq_handler((irq - IRQ_ORION5X_GPIO_0_7) << 3);
=======
 */
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <plat/orion-gpio.h>
#include <plat/irq.h>
#include <asm/exception.h>
#include "bridge-regs.h"
#include "common.h"

static int __initdata gpio0_irqs[4] = {
	IRQ_ORION5X_GPIO_0_7,
	IRQ_ORION5X_GPIO_8_15,
	IRQ_ORION5X_GPIO_16_23,
	IRQ_ORION5X_GPIO_24_31,
};

static asmlinkage void
__exception_irq_entry orion5x_legacy_handle_irq(struct pt_regs *regs)
{
	u32 stat;

	stat = readl_relaxed(MAIN_IRQ_CAUSE);
	stat &= readl_relaxed(MAIN_IRQ_MASK);
	if (stat) {
		unsigned int hwirq = 1 + __fls(stat);
		handle_IRQ(hwirq, regs);
		return;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init orion5x_init_irq(void)
{
<<<<<<< HEAD
	orion_irq_init(0, (void __iomem *)MAIN_IRQ_MASK);
=======
	orion_irq_init(1, MAIN_IRQ_MASK);

	set_handle_irq(orion5x_legacy_handle_irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Initialize gpiolib for GPIOs 0-31.
	 */
<<<<<<< HEAD
	orion_gpio_init(0, 32, GPIO_VIRT_BASE, 0, IRQ_ORION5X_GPIO_START);
	irq_set_chained_handler(IRQ_ORION5X_GPIO_0_7, gpio_irq_handler);
	irq_set_chained_handler(IRQ_ORION5X_GPIO_8_15, gpio_irq_handler);
	irq_set_chained_handler(IRQ_ORION5X_GPIO_16_23, gpio_irq_handler);
	irq_set_chained_handler(IRQ_ORION5X_GPIO_24_31, gpio_irq_handler);
=======
	orion_gpio_init(0, 32, GPIO_VIRT_BASE, 0,
			IRQ_ORION5X_GPIO_START, gpio0_irqs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
