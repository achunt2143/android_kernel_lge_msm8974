<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* power.c: Power management driver.
 *
 * Copyright (C) 1999, 2007, 2008 David S. Miller (davem@davemloft.net)
 */

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/reboot.h>
<<<<<<< HEAD
#include <linux/of_device.h>
=======
#include <linux/of.h>
#include <linux/platform_device.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/prom.h>
#include <asm/io.h>

static void __iomem *power_reg;

static irqreturn_t power_handler(int irq, void *dev_id)
{
	orderly_poweroff(true);

	/* FIXME: Check registers for status... */
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int __devinit has_button_interrupt(unsigned int irq, struct device_node *dp)
{
	if (irq == 0xffffffff)
		return 0;
	if (!of_find_property(dp, "button", NULL))
=======
static int has_button_interrupt(unsigned int irq, struct device_node *dp)
{
	if (irq == 0xffffffff)
		return 0;
	if (!of_property_read_bool(dp, "button"))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	return 1;
}

<<<<<<< HEAD
static int __devinit power_probe(struct platform_device *op)
=======
static int power_probe(struct platform_device *op)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct resource *res = &op->resource[0];
	unsigned int irq = op->archdata.irqs[0];

	power_reg = of_ioremap(res, 0, 0x4, "power");

<<<<<<< HEAD
	printk(KERN_INFO "%s: Control reg at %llx\n",
	       op->dev.of_node->name, res->start);
=======
	printk(KERN_INFO "%pOFn: Control reg at %llx\n",
	       op->dev.of_node, res->start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (has_button_interrupt(irq, op->dev.of_node)) {
		if (request_irq(irq,
				power_handler, 0, "power", NULL) < 0)
			printk(KERN_ERR "power: Cannot setup IRQ handler.\n");
	}

	return 0;
}

static const struct of_device_id power_match[] = {
	{
		.name = "power",
	},
	{},
};

static struct platform_driver power_driver = {
	.probe		= power_probe,
	.driver = {
		.name = "power",
<<<<<<< HEAD
		.owner = THIS_MODULE,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.of_match_table = power_match,
	},
};

<<<<<<< HEAD
static int __init power_init(void)
{
	return platform_driver_register(&power_driver);
}

device_initcall(power_init);
=======
builtin_platform_driver(power_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
