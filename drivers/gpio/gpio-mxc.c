<<<<<<< HEAD
/*
 * MXC GPIO support. (c) 2008 Daniel Mack <daniel@caiaq.de>
 * Copyright 2008 Juergen Beisert, kernel@pengutronix.de
 *
 * Based on code from Freescale,
 * Copyright (C) 2004-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

=======
// SPDX-License-Identifier: GPL-2.0+
//
// MXC GPIO support. (c) 2008 Daniel Mack <daniel@caiaq.de>
// Copyright 2008 Juergen Beisert, kernel@pengutronix.de
//
// Based on code from Freescale Semiconductor,
// Authors: Daniel Mack, Juergen Beisert.
// Copyright (C) 2004-2010 Freescale Semiconductor, Inc. All Rights Reserved.

#include <linux/clk.h>
#include <linux/err.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/basic_mmio_gpio.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/module.h>
#include <asm-generic/bug.h>
#include <asm/mach/irq.h>

#define irq_to_gpio(irq)	((irq) - MXC_GPIO_IRQ_START)

enum mxc_gpio_hwtype {
	IMX1_GPIO,	/* runs on i.mx1 */
	IMX21_GPIO,	/* runs on i.mx21 and i.mx27 */
	IMX31_GPIO,	/* runs on all other i.mx */
};
=======
#include <linux/irqdomain.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/syscore_ops.h>
#include <linux/gpio/driver.h>
#include <linux/of.h>
#include <linux/bug.h>

#define IMX_SCU_WAKEUP_OFF		0
#define IMX_SCU_WAKEUP_LOW_LVL		4
#define IMX_SCU_WAKEUP_FALL_EDGE	5
#define IMX_SCU_WAKEUP_RISE_EDGE	6
#define IMX_SCU_WAKEUP_HIGH_LVL		7
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* device type dependent stuff */
struct mxc_gpio_hwdata {
	unsigned dr_reg;
	unsigned gdir_reg;
	unsigned psr_reg;
	unsigned icr1_reg;
	unsigned icr2_reg;
	unsigned imr_reg;
	unsigned isr_reg;
<<<<<<< HEAD
=======
	int edge_sel_reg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned low_level;
	unsigned high_level;
	unsigned rise_edge;
	unsigned fall_edge;
};

<<<<<<< HEAD
struct mxc_gpio_port {
	struct list_head node;
	void __iomem *base;
	int irq;
	int irq_high;
	int virtual_irq_start;
	struct bgpio_chip bgc;
	u32 both_edges;
=======
struct mxc_gpio_reg_saved {
	u32 icr1;
	u32 icr2;
	u32 imr;
	u32 gdir;
	u32 edge_sel;
	u32 dr;
};

struct mxc_gpio_port {
	struct list_head node;
	void __iomem *base;
	struct clk *clk;
	int irq;
	int irq_high;
	void (*mx_irq_handler)(struct irq_desc *desc);
	struct irq_domain *domain;
	struct gpio_chip gc;
	struct device *dev;
	u32 both_edges;
	struct mxc_gpio_reg_saved gpio_saved_reg;
	bool power_off;
	u32 wakeup_pads;
	bool is_pad_wakeup;
	u32 pad_type[32];
	const struct mxc_gpio_hwdata *hwdata;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct mxc_gpio_hwdata imx1_imx21_gpio_hwdata = {
	.dr_reg		= 0x1c,
	.gdir_reg	= 0x00,
	.psr_reg	= 0x24,
	.icr1_reg	= 0x28,
	.icr2_reg	= 0x2c,
	.imr_reg	= 0x30,
	.isr_reg	= 0x34,
<<<<<<< HEAD
=======
	.edge_sel_reg	= -EINVAL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.low_level	= 0x03,
	.high_level	= 0x02,
	.rise_edge	= 0x00,
	.fall_edge	= 0x01,
};

static struct mxc_gpio_hwdata imx31_gpio_hwdata = {
	.dr_reg		= 0x00,
	.gdir_reg	= 0x04,
	.psr_reg	= 0x08,
	.icr1_reg	= 0x0c,
	.icr2_reg	= 0x10,
	.imr_reg	= 0x14,
	.isr_reg	= 0x18,
<<<<<<< HEAD
=======
	.edge_sel_reg	= -EINVAL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.low_level	= 0x00,
	.high_level	= 0x01,
	.rise_edge	= 0x02,
	.fall_edge	= 0x03,
};

<<<<<<< HEAD
static enum mxc_gpio_hwtype mxc_gpio_hwtype;
static struct mxc_gpio_hwdata *mxc_gpio_hwdata;

#define GPIO_DR			(mxc_gpio_hwdata->dr_reg)
#define GPIO_GDIR		(mxc_gpio_hwdata->gdir_reg)
#define GPIO_PSR		(mxc_gpio_hwdata->psr_reg)
#define GPIO_ICR1		(mxc_gpio_hwdata->icr1_reg)
#define GPIO_ICR2		(mxc_gpio_hwdata->icr2_reg)
#define GPIO_IMR		(mxc_gpio_hwdata->imr_reg)
#define GPIO_ISR		(mxc_gpio_hwdata->isr_reg)

#define GPIO_INT_LOW_LEV	(mxc_gpio_hwdata->low_level)
#define GPIO_INT_HIGH_LEV	(mxc_gpio_hwdata->high_level)
#define GPIO_INT_RISE_EDGE	(mxc_gpio_hwdata->rise_edge)
#define GPIO_INT_FALL_EDGE	(mxc_gpio_hwdata->fall_edge)
#define GPIO_INT_NONE		0x4

static struct platform_device_id mxc_gpio_devtype[] = {
	{
		.name = "imx1-gpio",
		.driver_data = IMX1_GPIO,
	}, {
		.name = "imx21-gpio",
		.driver_data = IMX21_GPIO,
	}, {
		.name = "imx31-gpio",
		.driver_data = IMX31_GPIO,
	}, {
		/* sentinel */
	}
};

static const struct of_device_id mxc_gpio_dt_ids[] = {
	{ .compatible = "fsl,imx1-gpio", .data = &mxc_gpio_devtype[IMX1_GPIO], },
	{ .compatible = "fsl,imx21-gpio", .data = &mxc_gpio_devtype[IMX21_GPIO], },
	{ .compatible = "fsl,imx31-gpio", .data = &mxc_gpio_devtype[IMX31_GPIO], },
	{ /* sentinel */ }
};
=======
static struct mxc_gpio_hwdata imx35_gpio_hwdata = {
	.dr_reg		= 0x00,
	.gdir_reg	= 0x04,
	.psr_reg	= 0x08,
	.icr1_reg	= 0x0c,
	.icr2_reg	= 0x10,
	.imr_reg	= 0x14,
	.isr_reg	= 0x18,
	.edge_sel_reg	= 0x1c,
	.low_level	= 0x00,
	.high_level	= 0x01,
	.rise_edge	= 0x02,
	.fall_edge	= 0x03,
};

#define GPIO_DR			(port->hwdata->dr_reg)
#define GPIO_GDIR		(port->hwdata->gdir_reg)
#define GPIO_PSR		(port->hwdata->psr_reg)
#define GPIO_ICR1		(port->hwdata->icr1_reg)
#define GPIO_ICR2		(port->hwdata->icr2_reg)
#define GPIO_IMR		(port->hwdata->imr_reg)
#define GPIO_ISR		(port->hwdata->isr_reg)
#define GPIO_EDGE_SEL		(port->hwdata->edge_sel_reg)

#define GPIO_INT_LOW_LEV	(port->hwdata->low_level)
#define GPIO_INT_HIGH_LEV	(port->hwdata->high_level)
#define GPIO_INT_RISE_EDGE	(port->hwdata->rise_edge)
#define GPIO_INT_FALL_EDGE	(port->hwdata->fall_edge)
#define GPIO_INT_BOTH_EDGES	0x4

static const struct of_device_id mxc_gpio_dt_ids[] = {
	{ .compatible = "fsl,imx1-gpio", .data =  &imx1_imx21_gpio_hwdata },
	{ .compatible = "fsl,imx21-gpio", .data = &imx1_imx21_gpio_hwdata },
	{ .compatible = "fsl,imx31-gpio", .data = &imx31_gpio_hwdata },
	{ .compatible = "fsl,imx35-gpio", .data = &imx35_gpio_hwdata },
	{ .compatible = "fsl,imx7d-gpio", .data = &imx35_gpio_hwdata },
	{ .compatible = "fsl,imx8dxl-gpio", .data = &imx35_gpio_hwdata },
	{ .compatible = "fsl,imx8qm-gpio", .data = &imx35_gpio_hwdata },
	{ .compatible = "fsl,imx8qxp-gpio", .data = &imx35_gpio_hwdata },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mxc_gpio_dt_ids);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * MX2 has one interrupt *for all* gpio ports. The list is used
 * to save the references to all ports, so that mx2_gpio_irq_handler
 * can walk through all interrupt status registers.
 */
static LIST_HEAD(mxc_gpio_ports);

/* Note: This driver assumes 32 GPIOs are handled in one register */

static int gpio_set_irq_type(struct irq_data *d, u32 type)
{
<<<<<<< HEAD
	u32 gpio = irq_to_gpio(d->irq);
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);
	struct mxc_gpio_port *port = gc->private;
	u32 bit, val;
	int edge;
	void __iomem *reg = port->base;

	port->both_edges &= ~(1 << (gpio & 31));
=======
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);
	struct mxc_gpio_port *port = gc->private;
	unsigned long flags;
	u32 bit, val;
	u32 gpio_idx = d->hwirq;
	int edge;
	void __iomem *reg = port->base;

	port->both_edges &= ~(1 << gpio_idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (type) {
	case IRQ_TYPE_EDGE_RISING:
		edge = GPIO_INT_RISE_EDGE;
		break;
	case IRQ_TYPE_EDGE_FALLING:
		edge = GPIO_INT_FALL_EDGE;
		break;
	case IRQ_TYPE_EDGE_BOTH:
<<<<<<< HEAD
		val = gpio_get_value(gpio);
		if (val) {
			edge = GPIO_INT_LOW_LEV;
			pr_debug("mxc: set GPIO %d to low trigger\n", gpio);
		} else {
			edge = GPIO_INT_HIGH_LEV;
			pr_debug("mxc: set GPIO %d to high trigger\n", gpio);
		}
		port->both_edges |= 1 << (gpio & 31);
=======
		if (GPIO_EDGE_SEL >= 0) {
			edge = GPIO_INT_BOTH_EDGES;
		} else {
			val = port->gc.get(&port->gc, gpio_idx);
			if (val) {
				edge = GPIO_INT_LOW_LEV;
				pr_debug("mxc: set GPIO %d to low trigger\n", gpio_idx);
			} else {
				edge = GPIO_INT_HIGH_LEV;
				pr_debug("mxc: set GPIO %d to high trigger\n", gpio_idx);
			}
			port->both_edges |= 1 << gpio_idx;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case IRQ_TYPE_LEVEL_LOW:
		edge = GPIO_INT_LOW_LEV;
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		edge = GPIO_INT_HIGH_LEV;
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	reg += GPIO_ICR1 + ((gpio & 0x10) >> 2); /* lower or upper register */
	bit = gpio & 0xf;
	val = readl(reg) & ~(0x3 << (bit << 1));
	writel(val | (edge << (bit << 1)), reg);
	writel(1 << (gpio & 0x1f), port->base + GPIO_ISR);

	return 0;
=======
	raw_spin_lock_irqsave(&port->gc.bgpio_lock, flags);

	if (GPIO_EDGE_SEL >= 0) {
		val = readl(port->base + GPIO_EDGE_SEL);
		if (edge == GPIO_INT_BOTH_EDGES)
			writel(val | (1 << gpio_idx),
				port->base + GPIO_EDGE_SEL);
		else
			writel(val & ~(1 << gpio_idx),
				port->base + GPIO_EDGE_SEL);
	}

	if (edge != GPIO_INT_BOTH_EDGES) {
		reg += GPIO_ICR1 + ((gpio_idx & 0x10) >> 2); /* lower or upper register */
		bit = gpio_idx & 0xf;
		val = readl(reg) & ~(0x3 << (bit << 1));
		writel(val | (edge << (bit << 1)), reg);
	}

	writel(1 << gpio_idx, port->base + GPIO_ISR);
	port->pad_type[gpio_idx] = type;

	raw_spin_unlock_irqrestore(&port->gc.bgpio_lock, flags);

	return port->gc.direction_input(&port->gc, gpio_idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void mxc_flip_edge(struct mxc_gpio_port *port, u32 gpio)
{
	void __iomem *reg = port->base;
<<<<<<< HEAD
	u32 bit, val;
	int edge;

=======
	unsigned long flags;
	u32 bit, val;
	int edge;

	raw_spin_lock_irqsave(&port->gc.bgpio_lock, flags);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	reg += GPIO_ICR1 + ((gpio & 0x10) >> 2); /* lower or upper register */
	bit = gpio & 0xf;
	val = readl(reg);
	edge = (val >> (bit << 1)) & 3;
	val &= ~(0x3 << (bit << 1));
	if (edge == GPIO_INT_HIGH_LEV) {
		edge = GPIO_INT_LOW_LEV;
		pr_debug("mxc: switch GPIO %d to low trigger\n", gpio);
	} else if (edge == GPIO_INT_LOW_LEV) {
		edge = GPIO_INT_HIGH_LEV;
		pr_debug("mxc: switch GPIO %d to high trigger\n", gpio);
	} else {
		pr_err("mxc: invalid configuration for GPIO %d: %x\n",
		       gpio, edge);
<<<<<<< HEAD
		return;
	}
	writel(val | (edge << (bit << 1)), reg);
=======
		goto unlock;
	}
	writel(val | (edge << (bit << 1)), reg);

unlock:
	raw_spin_unlock_irqrestore(&port->gc.bgpio_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* handle 32 interrupts in one status register */
static void mxc_gpio_irq_handler(struct mxc_gpio_port *port, u32 irq_stat)
{
<<<<<<< HEAD
	u32 gpio_irq_no_base = port->virtual_irq_start;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (irq_stat != 0) {
		int irqoffset = fls(irq_stat) - 1;

		if (port->both_edges & (1 << irqoffset))
			mxc_flip_edge(port, irqoffset);

<<<<<<< HEAD
		generic_handle_irq(gpio_irq_no_base + irqoffset);
=======
		generic_handle_domain_irq(port->domain, irqoffset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		irq_stat &= ~(1 << irqoffset);
	}
}

/* MX1 and MX3 has one interrupt *per* gpio port */
<<<<<<< HEAD
static void mx3_gpio_irq_handler(u32 irq, struct irq_desc *desc)
{
	u32 irq_stat;
	struct mxc_gpio_port *port = irq_get_handler_data(irq);
	struct irq_chip *chip = irq_get_chip(irq);
=======
static void mx3_gpio_irq_handler(struct irq_desc *desc)
{
	u32 irq_stat;
	struct mxc_gpio_port *port = irq_desc_get_handler_data(desc);
	struct irq_chip *chip = irq_desc_get_chip(desc);

	if (port->is_pad_wakeup)
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	chained_irq_enter(chip, desc);

	irq_stat = readl(port->base + GPIO_ISR) & readl(port->base + GPIO_IMR);

	mxc_gpio_irq_handler(port, irq_stat);

	chained_irq_exit(chip, desc);
}

/* MX2 has one interrupt *for all* gpio ports */
<<<<<<< HEAD
static void mx2_gpio_irq_handler(u32 irq, struct irq_desc *desc)
{
	u32 irq_msk, irq_stat;
	struct mxc_gpio_port *port;
=======
static void mx2_gpio_irq_handler(struct irq_desc *desc)
{
	u32 irq_msk, irq_stat;
	struct mxc_gpio_port *port;
	struct irq_chip *chip = irq_desc_get_chip(desc);

	chained_irq_enter(chip, desc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* walk through all interrupt status registers */
	list_for_each_entry(port, &mxc_gpio_ports, node) {
		irq_msk = readl(port->base + GPIO_IMR);
		if (!irq_msk)
			continue;

		irq_stat = readl(port->base + GPIO_ISR) & irq_msk;
		if (irq_stat)
			mxc_gpio_irq_handler(port, irq_stat);
	}
<<<<<<< HEAD
=======
	chained_irq_exit(chip, desc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Set interrupt number "irq" in the GPIO as a wake-up source.
 * While system is running, all registered GPIO interrupts need to have
 * wake-up enabled. When system is suspended, only selected GPIO interrupts
 * need to have wake-up enabled.
 * @param  irq          interrupt source number
 * @param  enable       enable as wake-up if equal to non-zero
 * @return       This function returns 0 on success.
 */
static int gpio_set_wake_irq(struct irq_data *d, u32 enable)
{
<<<<<<< HEAD
	u32 gpio = irq_to_gpio(d->irq);
	u32 gpio_idx = gpio & 0x1F;
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);
	struct mxc_gpio_port *port = gc->private;

	if (enable) {
		if (port->irq_high && (gpio_idx >= 16))
			enable_irq_wake(port->irq_high);
		else
			enable_irq_wake(port->irq);
	} else {
		if (port->irq_high && (gpio_idx >= 16))
			disable_irq_wake(port->irq_high);
		else
			disable_irq_wake(port->irq);
	}

	return 0;
}

static void __init mxc_gpio_init_gc(struct mxc_gpio_port *port)
{
	struct irq_chip_generic *gc;
	struct irq_chip_type *ct;

	gc = irq_alloc_generic_chip("gpio-mxc", 1, port->virtual_irq_start,
				    port->base, handle_level_irq);
=======
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);
	struct mxc_gpio_port *port = gc->private;
	u32 gpio_idx = d->hwirq;
	int ret;

	if (enable) {
		if (port->irq_high && (gpio_idx >= 16))
			ret = enable_irq_wake(port->irq_high);
		else
			ret = enable_irq_wake(port->irq);
		port->wakeup_pads |= (1 << gpio_idx);
	} else {
		if (port->irq_high && (gpio_idx >= 16))
			ret = disable_irq_wake(port->irq_high);
		else
			ret = disable_irq_wake(port->irq);
		port->wakeup_pads &= ~(1 << gpio_idx);
	}

	return ret;
}

static int mxc_gpio_init_gc(struct mxc_gpio_port *port, int irq_base)
{
	struct irq_chip_generic *gc;
	struct irq_chip_type *ct;
	int rv;

	gc = devm_irq_alloc_generic_chip(port->dev, "gpio-mxc", 1, irq_base,
					 port->base, handle_level_irq);
	if (!gc)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	gc->private = port;

	ct = gc->chip_types;
	ct->chip.irq_ack = irq_gc_ack_set_bit;
	ct->chip.irq_mask = irq_gc_mask_clr_bit;
	ct->chip.irq_unmask = irq_gc_mask_set_bit;
	ct->chip.irq_set_type = gpio_set_irq_type;
	ct->chip.irq_set_wake = gpio_set_wake_irq;
<<<<<<< HEAD
	ct->regs.ack = GPIO_ISR;
	ct->regs.mask = GPIO_IMR;

	irq_setup_generic_chip(gc, IRQ_MSK(32), IRQ_GC_INIT_NESTED_LOCK,
			       IRQ_NOREQUEST, 0);
}

static void __devinit mxc_gpio_get_hw(struct platform_device *pdev)
{
	const struct of_device_id *of_id =
			of_match_device(mxc_gpio_dt_ids, &pdev->dev);
	enum mxc_gpio_hwtype hwtype;

	if (of_id)
		pdev->id_entry = of_id->data;
	hwtype = pdev->id_entry->driver_data;

	if (mxc_gpio_hwtype) {
		/*
		 * The driver works with a reasonable presupposition,
		 * that is all gpio ports must be the same type when
		 * running on one soc.
		 */
		BUG_ON(mxc_gpio_hwtype != hwtype);
		return;
	}

	if (hwtype == IMX31_GPIO)
		mxc_gpio_hwdata = &imx31_gpio_hwdata;
	else
		mxc_gpio_hwdata = &imx1_imx21_gpio_hwdata;

	mxc_gpio_hwtype = hwtype;
=======
	ct->chip.flags = IRQCHIP_MASK_ON_SUSPEND | IRQCHIP_ENABLE_WAKEUP_ON_SUSPEND;
	ct->regs.ack = GPIO_ISR;
	ct->regs.mask = GPIO_IMR;

	rv = devm_irq_setup_generic_chip(port->dev, gc, IRQ_MSK(32),
					 IRQ_GC_INIT_NESTED_LOCK,
					 IRQ_NOREQUEST, 0);

	return rv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mxc_gpio_to_irq(struct gpio_chip *gc, unsigned offset)
{
<<<<<<< HEAD
	struct bgpio_chip *bgc = to_bgpio_chip(gc);
	struct mxc_gpio_port *port =
		container_of(bgc, struct mxc_gpio_port, bgc);

	return port->virtual_irq_start + offset;
}

static int __devinit mxc_gpio_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct mxc_gpio_port *port;
	struct resource *iores;
	int err;

	mxc_gpio_get_hw(pdev);

	port = kzalloc(sizeof(struct mxc_gpio_port), GFP_KERNEL);
	if (!port)
		return -ENOMEM;

	iores = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!iores) {
		err = -ENODEV;
		goto out_kfree;
	}

	if (!request_mem_region(iores->start, resource_size(iores),
				pdev->name)) {
		err = -EBUSY;
		goto out_kfree;
	}

	port->base = ioremap(iores->start, resource_size(iores));
	if (!port->base) {
		err = -ENOMEM;
		goto out_release_mem;
	}

	port->irq_high = platform_get_irq(pdev, 1);
	port->irq = platform_get_irq(pdev, 0);
	if (port->irq < 0) {
		err = -EINVAL;
		goto out_iounmap;
	}
=======
	struct mxc_gpio_port *port = gpiochip_get_data(gc);

	return irq_find_mapping(port->domain, offset);
}

static int mxc_gpio_request(struct gpio_chip *chip, unsigned int offset)
{
	int ret;

	ret = gpiochip_generic_request(chip, offset);
	if (ret)
		return ret;

	return pm_runtime_resume_and_get(chip->parent);
}

static void mxc_gpio_free(struct gpio_chip *chip, unsigned int offset)
{
	gpiochip_generic_free(chip, offset);
	pm_runtime_put(chip->parent);
}

static void mxc_update_irq_chained_handler(struct mxc_gpio_port *port, bool enable)
{
	if (enable)
		irq_set_chained_handler_and_data(port->irq, port->mx_irq_handler, port);
	else
		irq_set_chained_handler_and_data(port->irq, NULL, NULL);

	/* setup handler for GPIO 16 to 31 */
	if (port->irq_high > 0) {
		if (enable)
			irq_set_chained_handler_and_data(port->irq_high,
							 port->mx_irq_handler,
							 port);
		else
			irq_set_chained_handler_and_data(port->irq_high, NULL, NULL);
	}
}

static int mxc_gpio_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct mxc_gpio_port *port;
	int irq_count;
	int irq_base;
	int err;

	port = devm_kzalloc(&pdev->dev, sizeof(*port), GFP_KERNEL);
	if (!port)
		return -ENOMEM;

	port->dev = &pdev->dev;
	port->hwdata = device_get_match_data(&pdev->dev);

	port->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(port->base))
		return PTR_ERR(port->base);

	irq_count = platform_irq_count(pdev);
	if (irq_count < 0)
		return irq_count;

	if (irq_count > 1) {
		port->irq_high = platform_get_irq(pdev, 1);
		if (port->irq_high < 0)
			port->irq_high = 0;
	}

	port->irq = platform_get_irq(pdev, 0);
	if (port->irq < 0)
		return port->irq;

	/* the controller clock is optional */
	port->clk = devm_clk_get_optional_enabled(&pdev->dev, NULL);
	if (IS_ERR(port->clk))
		return PTR_ERR(port->clk);

	if (of_device_is_compatible(np, "fsl,imx7d-gpio"))
		port->power_off = true;

	pm_runtime_get_noresume(&pdev->dev);
	pm_runtime_set_active(&pdev->dev);
	pm_runtime_enable(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* disable the interrupt and clear the status */
	writel(0, port->base + GPIO_IMR);
	writel(~0, port->base + GPIO_ISR);

<<<<<<< HEAD
	if (mxc_gpio_hwtype == IMX21_GPIO) {
		/* setup one handler for all GPIO interrupts */
		if (pdev->id == 0)
			irq_set_chained_handler(port->irq,
						mx2_gpio_irq_handler);
	} else {
		/* setup one handler for each entry */
		irq_set_chained_handler(port->irq, mx3_gpio_irq_handler);
		irq_set_handler_data(port->irq, port);
		if (port->irq_high > 0) {
			/* setup handler for GPIO 16 to 31 */
			irq_set_chained_handler(port->irq_high,
						mx3_gpio_irq_handler);
			irq_set_handler_data(port->irq_high, port);
		}
	}

	err = bgpio_init(&port->bgc, &pdev->dev, 4,
			 port->base + GPIO_PSR,
			 port->base + GPIO_DR, NULL,
			 port->base + GPIO_GDIR, NULL, false);
	if (err)
		goto out_iounmap;

	port->bgc.gc.to_irq = mxc_gpio_to_irq;
	port->bgc.gc.base = pdev->id * 32;
	port->bgc.dir = port->bgc.read_reg(port->bgc.reg_dir);
	port->bgc.data = port->bgc.read_reg(port->bgc.reg_set);

	err = gpiochip_add(&port->bgc.gc);
	if (err)
		goto out_bgpio_remove;

	/*
	 * In dt case, we use gpio number range dynamically
	 * allocated by gpio core.
	 */
	port->virtual_irq_start = MXC_GPIO_IRQ_START + (np ? port->bgc.gc.base :
							     pdev->id * 32);

	/* gpio-mxc can be a generic irq chip */
	mxc_gpio_init_gc(port);

	list_add_tail(&port->node, &mxc_gpio_ports);

	return 0;

out_bgpio_remove:
	bgpio_remove(&port->bgc);
out_iounmap:
	iounmap(port->base);
out_release_mem:
	release_mem_region(iores->start, resource_size(iores));
out_kfree:
	kfree(port);
=======
	if (of_device_is_compatible(np, "fsl,imx21-gpio")) {
		/*
		 * Setup one handler for all GPIO interrupts. Actually setting
		 * the handler is needed only once, but doing it for every port
		 * is more robust and easier.
		 */
		port->irq_high = -1;
		port->mx_irq_handler = mx2_gpio_irq_handler;
	} else
		port->mx_irq_handler = mx3_gpio_irq_handler;

	mxc_update_irq_chained_handler(port, true);
	err = bgpio_init(&port->gc, &pdev->dev, 4,
			 port->base + GPIO_PSR,
			 port->base + GPIO_DR, NULL,
			 port->base + GPIO_GDIR, NULL,
			 BGPIOF_READ_OUTPUT_REG_SET);
	if (err)
		goto out_bgio;

	port->gc.request = mxc_gpio_request;
	port->gc.free = mxc_gpio_free;
	port->gc.to_irq = mxc_gpio_to_irq;
	port->gc.base = (pdev->id < 0) ? of_alias_get_id(np, "gpio") * 32 :
					     pdev->id * 32;

	err = devm_gpiochip_add_data(&pdev->dev, &port->gc, port);
	if (err)
		goto out_bgio;

	irq_base = devm_irq_alloc_descs(&pdev->dev, -1, 0, 32, numa_node_id());
	if (irq_base < 0) {
		err = irq_base;
		goto out_bgio;
	}

	port->domain = irq_domain_add_legacy(np, 32, irq_base, 0,
					     &irq_domain_simple_ops, NULL);
	if (!port->domain) {
		err = -ENODEV;
		goto out_bgio;
	}

	irq_domain_set_pm_device(port->domain, &pdev->dev);

	/* gpio-mxc can be a generic irq chip */
	err = mxc_gpio_init_gc(port, irq_base);
	if (err < 0)
		goto out_irqdomain_remove;

	list_add_tail(&port->node, &mxc_gpio_ports);

	platform_set_drvdata(pdev, port);
	pm_runtime_put_autosuspend(&pdev->dev);

	return 0;

out_irqdomain_remove:
	irq_domain_remove(port->domain);
out_bgio:
	pm_runtime_disable(&pdev->dev);
	pm_runtime_put_noidle(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev_info(&pdev->dev, "%s failed with errno %d\n", __func__, err);
	return err;
}

<<<<<<< HEAD
static struct platform_driver mxc_gpio_driver = {
	.driver		= {
		.name	= "gpio-mxc",
		.owner	= THIS_MODULE,
		.of_match_table = mxc_gpio_dt_ids,
	},
	.probe		= mxc_gpio_probe,
	.id_table	= mxc_gpio_devtype,
=======
static void mxc_gpio_save_regs(struct mxc_gpio_port *port)
{
	if (!port->power_off)
		return;

	port->gpio_saved_reg.icr1 = readl(port->base + GPIO_ICR1);
	port->gpio_saved_reg.icr2 = readl(port->base + GPIO_ICR2);
	port->gpio_saved_reg.imr = readl(port->base + GPIO_IMR);
	port->gpio_saved_reg.gdir = readl(port->base + GPIO_GDIR);
	port->gpio_saved_reg.edge_sel = readl(port->base + GPIO_EDGE_SEL);
	port->gpio_saved_reg.dr = readl(port->base + GPIO_DR);
}

static void mxc_gpio_restore_regs(struct mxc_gpio_port *port)
{
	if (!port->power_off)
		return;

	writel(port->gpio_saved_reg.icr1, port->base + GPIO_ICR1);
	writel(port->gpio_saved_reg.icr2, port->base + GPIO_ICR2);
	writel(port->gpio_saved_reg.imr, port->base + GPIO_IMR);
	writel(port->gpio_saved_reg.gdir, port->base + GPIO_GDIR);
	writel(port->gpio_saved_reg.edge_sel, port->base + GPIO_EDGE_SEL);
	writel(port->gpio_saved_reg.dr, port->base + GPIO_DR);
}

static bool mxc_gpio_generic_config(struct mxc_gpio_port *port,
		unsigned int offset, unsigned long conf)
{
	struct device_node *np = port->dev->of_node;

	if (of_device_is_compatible(np, "fsl,imx8dxl-gpio") ||
	    of_device_is_compatible(np, "fsl,imx8qxp-gpio") ||
	    of_device_is_compatible(np, "fsl,imx8qm-gpio"))
		return (gpiochip_generic_config(&port->gc, offset, conf) == 0);

	return false;
}

static bool mxc_gpio_set_pad_wakeup(struct mxc_gpio_port *port, bool enable)
{
	unsigned long config;
	bool ret = false;
	int i, type;

	static const u32 pad_type_map[] = {
		IMX_SCU_WAKEUP_OFF,		/* 0 */
		IMX_SCU_WAKEUP_RISE_EDGE,	/* IRQ_TYPE_EDGE_RISING */
		IMX_SCU_WAKEUP_FALL_EDGE,	/* IRQ_TYPE_EDGE_FALLING */
		IMX_SCU_WAKEUP_FALL_EDGE,	/* IRQ_TYPE_EDGE_BOTH */
		IMX_SCU_WAKEUP_HIGH_LVL,	/* IRQ_TYPE_LEVEL_HIGH */
		IMX_SCU_WAKEUP_OFF,		/* 5 */
		IMX_SCU_WAKEUP_OFF,		/* 6 */
		IMX_SCU_WAKEUP_OFF,		/* 7 */
		IMX_SCU_WAKEUP_LOW_LVL,		/* IRQ_TYPE_LEVEL_LOW */
	};

	for (i = 0; i < 32; i++) {
		if ((port->wakeup_pads & (1 << i))) {
			type = port->pad_type[i];
			if (enable)
				config = pad_type_map[type];
			else
				config = IMX_SCU_WAKEUP_OFF;
			ret |= mxc_gpio_generic_config(port, i, config);
		}
	}

	return ret;
}

static int mxc_gpio_runtime_suspend(struct device *dev)
{
	struct mxc_gpio_port *port = dev_get_drvdata(dev);

	mxc_gpio_save_regs(port);
	clk_disable_unprepare(port->clk);
	mxc_update_irq_chained_handler(port, false);

	return 0;
}

static int mxc_gpio_runtime_resume(struct device *dev)
{
	struct mxc_gpio_port *port = dev_get_drvdata(dev);
	int ret;

	mxc_update_irq_chained_handler(port, true);
	ret = clk_prepare_enable(port->clk);
	if (ret) {
		mxc_update_irq_chained_handler(port, false);
		return ret;
	}

	mxc_gpio_restore_regs(port);

	return 0;
}

static int mxc_gpio_noirq_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct mxc_gpio_port *port = platform_get_drvdata(pdev);

	if (port->wakeup_pads > 0)
		port->is_pad_wakeup = mxc_gpio_set_pad_wakeup(port, true);

	return 0;
}

static int mxc_gpio_noirq_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct mxc_gpio_port *port = platform_get_drvdata(pdev);

	if (port->wakeup_pads > 0)
		mxc_gpio_set_pad_wakeup(port, false);
	port->is_pad_wakeup = false;

	return 0;
}

static const struct dev_pm_ops mxc_gpio_dev_pm_ops = {
	NOIRQ_SYSTEM_SLEEP_PM_OPS(mxc_gpio_noirq_suspend, mxc_gpio_noirq_resume)
	RUNTIME_PM_OPS(mxc_gpio_runtime_suspend, mxc_gpio_runtime_resume, NULL)
};

static int mxc_gpio_syscore_suspend(void)
{
	struct mxc_gpio_port *port;
	int ret;

	/* walk through all ports */
	list_for_each_entry(port, &mxc_gpio_ports, node) {
		ret = clk_prepare_enable(port->clk);
		if (ret)
			return ret;
		mxc_gpio_save_regs(port);
		clk_disable_unprepare(port->clk);
	}

	return 0;
}

static void mxc_gpio_syscore_resume(void)
{
	struct mxc_gpio_port *port;
	int ret;

	/* walk through all ports */
	list_for_each_entry(port, &mxc_gpio_ports, node) {
		ret = clk_prepare_enable(port->clk);
		if (ret) {
			pr_err("mxc: failed to enable gpio clock %d\n", ret);
			return;
		}
		mxc_gpio_restore_regs(port);
		clk_disable_unprepare(port->clk);
	}
}

static struct syscore_ops mxc_gpio_syscore_ops = {
	.suspend = mxc_gpio_syscore_suspend,
	.resume = mxc_gpio_syscore_resume,
};

static struct platform_driver mxc_gpio_driver = {
	.driver		= {
		.name	= "gpio-mxc",
		.of_match_table = mxc_gpio_dt_ids,
		.suppress_bind_attrs = true,
		.pm = pm_ptr(&mxc_gpio_dev_pm_ops),
	},
	.probe		= mxc_gpio_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init gpio_mxc_init(void)
{
<<<<<<< HEAD
	return platform_driver_register(&mxc_gpio_driver);
}
postcore_initcall(gpio_mxc_init);

MODULE_AUTHOR("Freescale Semiconductor, "
	      "Daniel Mack <danielncaiaq.de>, "
	      "Juergen Beisert <kernel@pengutronix.de>");
MODULE_DESCRIPTION("Freescale MXC GPIO");
=======
	register_syscore_ops(&mxc_gpio_syscore_ops);

	return platform_driver_register(&mxc_gpio_driver);
}
subsys_initcall(gpio_mxc_init);

MODULE_AUTHOR("Shawn Guo <shawn.guo@linaro.org>");
MODULE_DESCRIPTION("i.MX GPIO Driver");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
