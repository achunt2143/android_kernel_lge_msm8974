<<<<<<< HEAD
/*
 * Copyright (C) 2008, 2009 Provigent Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2008, 2009 Provigent Ltd.
 *
 * Author: Baruch Siach <baruch@tkos.co.il>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Driver for the ARM PrimeCell(tm) General Purpose Input/Output (PL061)
 *
 * Data sheet: ARM DDI 0190B, September 2000
 */
<<<<<<< HEAD
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/bitops.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/amba/bus.h>
#include <linux/amba/pl061.h>
#include <linux/slab.h>
#include <linux/pm.h>
#include <asm/mach/irq.h>
=======
#include <linux/amba/bus.h>
#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/gpio/driver.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/module.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pm.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define GPIODIR 0x400
#define GPIOIS  0x404
#define GPIOIBE 0x408
#define GPIOIEV 0x40C
#define GPIOIE  0x410
#define GPIORIS 0x414
#define GPIOMIS 0x418
#define GPIOIC  0x41C

#define PL061_GPIO_NR	8

#ifdef CONFIG_PM
struct pl061_context_save_regs {
	u8 gpio_data;
	u8 gpio_dir;
	u8 gpio_is;
	u8 gpio_ibe;
	u8 gpio_iev;
	u8 gpio_ie;
};
#endif

<<<<<<< HEAD
struct pl061_gpio {
	/* Each of the two spinlocks protects a different set of hardware
	 * regiters and data structurs. This decouples the code of the IRQ from
	 * the GPIO code. This also makes the case of a GPIO routine call from
	 * the IRQ code simpler.
	 */
	spinlock_t		lock;		/* GPIO registers */

	void __iomem		*base;
	int			irq_base;
	struct irq_chip_generic	*irq_gc;
	struct gpio_chip	gc;
=======
struct pl061 {
	raw_spinlock_t		lock;

	void __iomem		*base;
	struct gpio_chip	gc;
	int			parent_irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PM
	struct pl061_context_save_regs csave_regs;
#endif
};

<<<<<<< HEAD
static int pl061_direction_input(struct gpio_chip *gc, unsigned offset)
{
	struct pl061_gpio *chip = container_of(gc, struct pl061_gpio, gc);
	unsigned long flags;
	unsigned char gpiodir;

	if (offset >= gc->ngpio)
		return -EINVAL;

	spin_lock_irqsave(&chip->lock, flags);
	gpiodir = readb(chip->base + GPIODIR);
	gpiodir &= ~(1 << offset);
	writeb(gpiodir, chip->base + GPIODIR);
	spin_unlock_irqrestore(&chip->lock, flags);
=======
static int pl061_get_direction(struct gpio_chip *gc, unsigned offset)
{
	struct pl061 *pl061 = gpiochip_get_data(gc);

	if (readb(pl061->base + GPIODIR) & BIT(offset))
		return GPIO_LINE_DIRECTION_OUT;

	return GPIO_LINE_DIRECTION_IN;
}

static int pl061_direction_input(struct gpio_chip *gc, unsigned offset)
{
	struct pl061 *pl061 = gpiochip_get_data(gc);
	unsigned long flags;
	unsigned char gpiodir;

	raw_spin_lock_irqsave(&pl061->lock, flags);
	gpiodir = readb(pl061->base + GPIODIR);
	gpiodir &= ~(BIT(offset));
	writeb(gpiodir, pl061->base + GPIODIR);
	raw_spin_unlock_irqrestore(&pl061->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int pl061_direction_output(struct gpio_chip *gc, unsigned offset,
		int value)
{
<<<<<<< HEAD
	struct pl061_gpio *chip = container_of(gc, struct pl061_gpio, gc);
	unsigned long flags;
	unsigned char gpiodir;

	if (offset >= gc->ngpio)
		return -EINVAL;

	spin_lock_irqsave(&chip->lock, flags);
	writeb(!!value << offset, chip->base + (1 << (offset + 2)));
	gpiodir = readb(chip->base + GPIODIR);
	gpiodir |= 1 << offset;
	writeb(gpiodir, chip->base + GPIODIR);
=======
	struct pl061 *pl061 = gpiochip_get_data(gc);
	unsigned long flags;
	unsigned char gpiodir;

	raw_spin_lock_irqsave(&pl061->lock, flags);
	writeb(!!value << offset, pl061->base + (BIT(offset + 2)));
	gpiodir = readb(pl061->base + GPIODIR);
	gpiodir |= BIT(offset);
	writeb(gpiodir, pl061->base + GPIODIR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * gpio value is set again, because pl061 doesn't allow to set value of
	 * a gpio pin before configuring it in OUT mode.
	 */
<<<<<<< HEAD
	writeb(!!value << offset, chip->base + (1 << (offset + 2)));
	spin_unlock_irqrestore(&chip->lock, flags);
=======
	writeb(!!value << offset, pl061->base + (BIT(offset + 2)));
	raw_spin_unlock_irqrestore(&pl061->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int pl061_get_value(struct gpio_chip *gc, unsigned offset)
{
<<<<<<< HEAD
	struct pl061_gpio *chip = container_of(gc, struct pl061_gpio, gc);

	return !!readb(chip->base + (1 << (offset + 2)));
=======
	struct pl061 *pl061 = gpiochip_get_data(gc);

	return !!readb(pl061->base + (BIT(offset + 2)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void pl061_set_value(struct gpio_chip *gc, unsigned offset, int value)
{
<<<<<<< HEAD
	struct pl061_gpio *chip = container_of(gc, struct pl061_gpio, gc);

	writeb(!!value << offset, chip->base + (1 << (offset + 2)));
}

static int pl061_to_irq(struct gpio_chip *gc, unsigned offset)
{
	struct pl061_gpio *chip = container_of(gc, struct pl061_gpio, gc);

	if (chip->irq_base <= 0)
		return -EINVAL;

	return chip->irq_base + offset;
=======
	struct pl061 *pl061 = gpiochip_get_data(gc);

	writeb(!!value << offset, pl061->base + (BIT(offset + 2)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pl061_irq_type(struct irq_data *d, unsigned trigger)
{
<<<<<<< HEAD
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);
	struct pl061_gpio *chip = gc->private;
	int offset = d->irq - chip->irq_base;
	unsigned long flags;
	u8 gpiois, gpioibe, gpioiev;
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pl061 *pl061 = gpiochip_get_data(gc);
	int offset = irqd_to_hwirq(d);
	unsigned long flags;
	u8 gpiois, gpioibe, gpioiev;
	u8 bit = BIT(offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (offset < 0 || offset >= PL061_GPIO_NR)
		return -EINVAL;

<<<<<<< HEAD
	raw_spin_lock_irqsave(&gc->lock, flags);

	gpioiev = readb(chip->base + GPIOIEV);

	gpiois = readb(chip->base + GPIOIS);
	if (trigger & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW)) {
		gpiois |= 1 << offset;
		if (trigger & IRQ_TYPE_LEVEL_HIGH)
			gpioiev |= 1 << offset;
		else
			gpioiev &= ~(1 << offset);
	} else
		gpiois &= ~(1 << offset);
	writeb(gpiois, chip->base + GPIOIS);

	gpioibe = readb(chip->base + GPIOIBE);
	if ((trigger & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH)
		gpioibe |= 1 << offset;
	else {
		gpioibe &= ~(1 << offset);
		if (trigger & IRQ_TYPE_EDGE_RISING)
			gpioiev |= 1 << offset;
		else if (trigger & IRQ_TYPE_EDGE_FALLING)
			gpioiev &= ~(1 << offset);
	}
	writeb(gpioibe, chip->base + GPIOIBE);

	writeb(gpioiev, chip->base + GPIOIEV);

	raw_spin_unlock_irqrestore(&gc->lock, flags);
=======
	if ((trigger & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW)) &&
	    (trigger & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_EDGE_FALLING)))
	{
		dev_err(gc->parent,
			"trying to configure line %d for both level and edge "
			"detection, choose one!\n",
			offset);
		return -EINVAL;
	}


	raw_spin_lock_irqsave(&pl061->lock, flags);

	gpioiev = readb(pl061->base + GPIOIEV);
	gpiois = readb(pl061->base + GPIOIS);
	gpioibe = readb(pl061->base + GPIOIBE);

	if (trigger & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW)) {
		bool polarity = trigger & IRQ_TYPE_LEVEL_HIGH;

		/* Disable edge detection */
		gpioibe &= ~bit;
		/* Enable level detection */
		gpiois |= bit;
		/* Select polarity */
		if (polarity)
			gpioiev |= bit;
		else
			gpioiev &= ~bit;
		irq_set_handler_locked(d, handle_level_irq);
		dev_dbg(gc->parent, "line %d: IRQ on %s level\n",
			offset,
			polarity ? "HIGH" : "LOW");
	} else if ((trigger & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH) {
		/* Disable level detection */
		gpiois &= ~bit;
		/* Select both edges, setting this makes GPIOEV be ignored */
		gpioibe |= bit;
		irq_set_handler_locked(d, handle_edge_irq);
		dev_dbg(gc->parent, "line %d: IRQ on both edges\n", offset);
	} else if ((trigger & IRQ_TYPE_EDGE_RISING) ||
		   (trigger & IRQ_TYPE_EDGE_FALLING)) {
		bool rising = trigger & IRQ_TYPE_EDGE_RISING;

		/* Disable level detection */
		gpiois &= ~bit;
		/* Clear detection on both edges */
		gpioibe &= ~bit;
		/* Select edge */
		if (rising)
			gpioiev |= bit;
		else
			gpioiev &= ~bit;
		irq_set_handler_locked(d, handle_edge_irq);
		dev_dbg(gc->parent, "line %d: IRQ on %s edge\n",
			offset,
			rising ? "RISING" : "FALLING");
	} else {
		/* No trigger: disable everything */
		gpiois &= ~bit;
		gpioibe &= ~bit;
		gpioiev &= ~bit;
		irq_set_handler_locked(d, handle_bad_irq);
		dev_warn(gc->parent, "no trigger selected for line %d\n",
			 offset);
	}

	writeb(gpiois, pl061->base + GPIOIS);
	writeb(gpioibe, pl061->base + GPIOIBE);
	writeb(gpioiev, pl061->base + GPIOIEV);

	raw_spin_unlock_irqrestore(&pl061->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void pl061_irq_handler(unsigned irq, struct irq_desc *desc)
{
	unsigned long pending;
	int offset;
	struct pl061_gpio *chip = irq_desc_get_handler_data(desc);
=======
static void pl061_irq_handler(struct irq_desc *desc)
{
	unsigned long pending;
	int offset;
	struct gpio_chip *gc = irq_desc_get_handler_data(desc);
	struct pl061 *pl061 = gpiochip_get_data(gc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct irq_chip *irqchip = irq_desc_get_chip(desc);

	chained_irq_enter(irqchip, desc);

<<<<<<< HEAD
	pending = readb(chip->base + GPIOMIS);
	writeb(pending, chip->base + GPIOIC);
	if (pending) {
		for_each_set_bit(offset, &pending, PL061_GPIO_NR)
			generic_handle_irq(pl061_to_irq(&chip->gc, offset));
=======
	pending = readb(pl061->base + GPIOMIS);
	if (pending) {
		for_each_set_bit(offset, &pending, PL061_GPIO_NR)
			generic_handle_domain_irq(gc->irq.domain,
						  offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	chained_irq_exit(irqchip, desc);
}

<<<<<<< HEAD
static void __init pl061_init_gc(struct pl061_gpio *chip, int irq_base)
{
	struct irq_chip_type *ct;

	chip->irq_gc = irq_alloc_generic_chip("gpio-pl061", 1, irq_base,
					      chip->base, handle_simple_irq);
	chip->irq_gc->private = chip;

	ct = chip->irq_gc->chip_types;
	ct->chip.irq_mask = irq_gc_mask_clr_bit;
	ct->chip.irq_unmask = irq_gc_mask_set_bit;
	ct->chip.irq_set_type = pl061_irq_type;
	ct->chip.irq_set_wake = irq_gc_set_wake;
	ct->regs.mask = GPIOIE;

	irq_setup_generic_chip(chip->irq_gc, IRQ_MSK(PL061_GPIO_NR),
			       IRQ_GC_INIT_NESTED_LOCK, IRQ_NOREQUEST, 0);
}

static int pl061_probe(struct amba_device *dev, const struct amba_id *id)
{
	struct pl061_platform_data *pdata;
	struct pl061_gpio *chip;
	int ret, irq, i;

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (chip == NULL)
		return -ENOMEM;

	pdata = dev->dev.platform_data;
	if (pdata) {
		chip->gc.base = pdata->gpio_base;
		chip->irq_base = pdata->irq_base;
	} else if (dev->dev.of_node) {
		chip->gc.base = -1;
		chip->irq_base = 0;
	} else {
		ret = -ENODEV;
		goto free_mem;
	}

	if (!request_mem_region(dev->res.start,
				resource_size(&dev->res), "pl061")) {
		ret = -EBUSY;
		goto free_mem;
	}

	chip->base = ioremap(dev->res.start, resource_size(&dev->res));
	if (chip->base == NULL) {
		ret = -ENOMEM;
		goto release_region;
	}

	spin_lock_init(&chip->lock);

	chip->gc.direction_input = pl061_direction_input;
	chip->gc.direction_output = pl061_direction_output;
	chip->gc.get = pl061_get_value;
	chip->gc.set = pl061_set_value;
	chip->gc.to_irq = pl061_to_irq;
	chip->gc.ngpio = PL061_GPIO_NR;
	chip->gc.label = dev_name(&dev->dev);
	chip->gc.dev = &dev->dev;
	chip->gc.owner = THIS_MODULE;

	ret = gpiochip_add(&chip->gc);
	if (ret)
		goto iounmap;
=======
static void pl061_irq_mask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pl061 *pl061 = gpiochip_get_data(gc);
	u8 mask = BIT(irqd_to_hwirq(d) % PL061_GPIO_NR);
	u8 gpioie;

	raw_spin_lock(&pl061->lock);
	gpioie = readb(pl061->base + GPIOIE) & ~mask;
	writeb(gpioie, pl061->base + GPIOIE);
	raw_spin_unlock(&pl061->lock);

	gpiochip_disable_irq(gc, d->hwirq);
}

static void pl061_irq_unmask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pl061 *pl061 = gpiochip_get_data(gc);
	u8 mask = BIT(irqd_to_hwirq(d) % PL061_GPIO_NR);
	u8 gpioie;

	gpiochip_enable_irq(gc, d->hwirq);

	raw_spin_lock(&pl061->lock);
	gpioie = readb(pl061->base + GPIOIE) | mask;
	writeb(gpioie, pl061->base + GPIOIE);
	raw_spin_unlock(&pl061->lock);
}

/**
 * pl061_irq_ack() - ACK an edge IRQ
 * @d: IRQ data for this IRQ
 *
 * This gets called from the edge IRQ handler to ACK the edge IRQ
 * in the GPIOIC (interrupt-clear) register. For level IRQs this is
 * not needed: these go away when the level signal goes away.
 */
static void pl061_irq_ack(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pl061 *pl061 = gpiochip_get_data(gc);
	u8 mask = BIT(irqd_to_hwirq(d) % PL061_GPIO_NR);

	raw_spin_lock(&pl061->lock);
	writeb(mask, pl061->base + GPIOIC);
	raw_spin_unlock(&pl061->lock);
}

static int pl061_irq_set_wake(struct irq_data *d, unsigned int state)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pl061 *pl061 = gpiochip_get_data(gc);

	return irq_set_irq_wake(pl061->parent_irq, state);
}

static void pl061_irq_print_chip(struct irq_data *data, struct seq_file *p)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(data);

	seq_printf(p, dev_name(gc->parent));
}

static const struct irq_chip pl061_irq_chip = {
	.irq_ack		= pl061_irq_ack,
	.irq_mask		= pl061_irq_mask,
	.irq_unmask		= pl061_irq_unmask,
	.irq_set_type		= pl061_irq_type,
	.irq_set_wake		= pl061_irq_set_wake,
	.irq_print_chip		= pl061_irq_print_chip,
	.flags			= IRQCHIP_IMMUTABLE,
	GPIOCHIP_IRQ_RESOURCE_HELPERS,
};

static int pl061_probe(struct amba_device *adev, const struct amba_id *id)
{
	struct device *dev = &adev->dev;
	struct pl061 *pl061;
	struct gpio_irq_chip *girq;
	int ret, irq;

	pl061 = devm_kzalloc(dev, sizeof(*pl061), GFP_KERNEL);
	if (pl061 == NULL)
		return -ENOMEM;

	pl061->base = devm_ioremap_resource(dev, &adev->res);
	if (IS_ERR(pl061->base))
		return PTR_ERR(pl061->base);

	raw_spin_lock_init(&pl061->lock);
	pl061->gc.request = gpiochip_generic_request;
	pl061->gc.free = gpiochip_generic_free;
	pl061->gc.base = -1;
	pl061->gc.get_direction = pl061_get_direction;
	pl061->gc.direction_input = pl061_direction_input;
	pl061->gc.direction_output = pl061_direction_output;
	pl061->gc.get = pl061_get_value;
	pl061->gc.set = pl061_set_value;
	pl061->gc.ngpio = PL061_GPIO_NR;
	pl061->gc.label = dev_name(dev);
	pl061->gc.parent = dev;
	pl061->gc.owner = THIS_MODULE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * irq_chip support
	 */
<<<<<<< HEAD

	if (chip->irq_base <= 0)
		return 0;

	pl061_init_gc(chip, chip->irq_base);

	writeb(0, chip->base + GPIOIE); /* disable irqs */
	irq = dev->irq[0];
	if (irq < 0) {
		ret = -ENODEV;
		goto iounmap;
	}
	irq_set_chained_handler(irq, pl061_irq_handler);
	irq_set_handler_data(irq, chip);

	for (i = 0; i < PL061_GPIO_NR; i++) {
		if (pdata) {
			if (pdata->directions & (1 << i))
				pl061_direction_output(&chip->gc, i,
						pdata->values & (1 << i));
			else
				pl061_direction_input(&chip->gc, i);
		}
	}

	amba_set_drvdata(dev, chip);

	return 0;

iounmap:
	iounmap(chip->base);
release_region:
	release_mem_region(dev->res.start, resource_size(&dev->res));
free_mem:
	kfree(chip);

	return ret;
=======
	writeb(0, pl061->base + GPIOIE); /* disable irqs */
	irq = adev->irq[0];
	if (!irq)
		dev_warn(&adev->dev, "IRQ support disabled\n");
	pl061->parent_irq = irq;

	girq = &pl061->gc.irq;
	gpio_irq_chip_set_chip(girq, &pl061_irq_chip);
	girq->parent_handler = pl061_irq_handler;
	girq->num_parents = 1;
	girq->parents = devm_kcalloc(dev, 1, sizeof(*girq->parents),
				     GFP_KERNEL);
	if (!girq->parents)
		return -ENOMEM;
	girq->parents[0] = irq;
	girq->default_type = IRQ_TYPE_NONE;
	girq->handler = handle_bad_irq;

	ret = devm_gpiochip_add_data(dev, &pl061->gc, pl061);
	if (ret)
		return ret;

	amba_set_drvdata(adev, pl061);
	dev_info(dev, "PL061 GPIO chip registered\n");

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_PM
static int pl061_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct pl061_gpio *chip = dev_get_drvdata(dev);
	int offset;

	chip->csave_regs.gpio_data = 0;
	chip->csave_regs.gpio_dir = readb(chip->base + GPIODIR);
	chip->csave_regs.gpio_is = readb(chip->base + GPIOIS);
	chip->csave_regs.gpio_ibe = readb(chip->base + GPIOIBE);
	chip->csave_regs.gpio_iev = readb(chip->base + GPIOIEV);
	chip->csave_regs.gpio_ie = readb(chip->base + GPIOIE);

	for (offset = 0; offset < PL061_GPIO_NR; offset++) {
		if (chip->csave_regs.gpio_dir & (1 << offset))
			chip->csave_regs.gpio_data |=
				pl061_get_value(&chip->gc, offset) << offset;
=======
	struct pl061 *pl061 = dev_get_drvdata(dev);
	int offset;

	pl061->csave_regs.gpio_data = 0;
	pl061->csave_regs.gpio_dir = readb(pl061->base + GPIODIR);
	pl061->csave_regs.gpio_is = readb(pl061->base + GPIOIS);
	pl061->csave_regs.gpio_ibe = readb(pl061->base + GPIOIBE);
	pl061->csave_regs.gpio_iev = readb(pl061->base + GPIOIEV);
	pl061->csave_regs.gpio_ie = readb(pl061->base + GPIOIE);

	for (offset = 0; offset < PL061_GPIO_NR; offset++) {
		if (pl061->csave_regs.gpio_dir & (BIT(offset)))
			pl061->csave_regs.gpio_data |=
				pl061_get_value(&pl061->gc, offset) << offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

static int pl061_resume(struct device *dev)
{
<<<<<<< HEAD
	struct pl061_gpio *chip = dev_get_drvdata(dev);
	int offset;

	for (offset = 0; offset < PL061_GPIO_NR; offset++) {
		if (chip->csave_regs.gpio_dir & (1 << offset))
			pl061_direction_output(&chip->gc, offset,
					chip->csave_regs.gpio_data &
					(1 << offset));
		else
			pl061_direction_input(&chip->gc, offset);
	}

	writeb(chip->csave_regs.gpio_is, chip->base + GPIOIS);
	writeb(chip->csave_regs.gpio_ibe, chip->base + GPIOIBE);
	writeb(chip->csave_regs.gpio_iev, chip->base + GPIOIEV);
	writeb(chip->csave_regs.gpio_ie, chip->base + GPIOIE);
=======
	struct pl061 *pl061 = dev_get_drvdata(dev);
	int offset;

	for (offset = 0; offset < PL061_GPIO_NR; offset++) {
		if (pl061->csave_regs.gpio_dir & (BIT(offset)))
			pl061_direction_output(&pl061->gc, offset,
					pl061->csave_regs.gpio_data &
					(BIT(offset)));
		else
			pl061_direction_input(&pl061->gc, offset);
	}

	writeb(pl061->csave_regs.gpio_is, pl061->base + GPIOIS);
	writeb(pl061->csave_regs.gpio_ibe, pl061->base + GPIOIBE);
	writeb(pl061->csave_regs.gpio_iev, pl061->base + GPIOIEV);
	writeb(pl061->csave_regs.gpio_ie, pl061->base + GPIOIE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct dev_pm_ops pl061_dev_pm_ops = {
	.suspend = pl061_suspend,
	.resume = pl061_resume,
	.freeze = pl061_suspend,
	.restore = pl061_resume,
};
#endif

<<<<<<< HEAD
static struct amba_id pl061_ids[] = {
=======
static const struct amba_id pl061_ids[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.id	= 0x00041061,
		.mask	= 0x000fffff,
	},
	{ 0, 0 },
};
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DEVICE_TABLE(amba, pl061_ids);

static struct amba_driver pl061_gpio_driver = {
	.drv = {
		.name	= "pl061_gpio",
#ifdef CONFIG_PM
		.pm	= &pl061_dev_pm_ops,
#endif
	},
	.id_table	= pl061_ids,
	.probe		= pl061_probe,
};
<<<<<<< HEAD

static int __init pl061_gpio_init(void)
{
	return amba_driver_register(&pl061_gpio_driver);
}
subsys_initcall(pl061_gpio_init);

MODULE_AUTHOR("Baruch Siach <baruch@tkos.co.il>");
MODULE_DESCRIPTION("PL061 GPIO driver");
MODULE_LICENSE("GPL");
=======
module_amba_driver(pl061_gpio_driver);

MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
