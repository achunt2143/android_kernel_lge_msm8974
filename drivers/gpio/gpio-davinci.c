<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * TI DaVinci GPIO Support
 *
 * Copyright (c) 2006-2007 David Brownell
 * Copyright (c) 2007, MontaVista Software, Inc. <source@mvista.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include <linux/gpio.h>
=======
 */

#include <linux/gpio/driver.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
<<<<<<< HEAD

#include <asm/mach/irq.h>
=======
#include <linux/irq.h>
#include <linux/irqdomain.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>
#include <linux/platform_data/gpio-davinci.h>
#include <linux/property.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/spinlock.h>
#include <linux/pm_runtime.h>

#define MAX_REGS_BANKS 5
#define MAX_INT_PER_BANK 32
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct davinci_gpio_regs {
	u32	dir;
	u32	out_data;
	u32	set_data;
	u32	clr_data;
	u32	in_data;
	u32	set_rising;
	u32	clr_rising;
	u32	set_falling;
	u32	clr_falling;
	u32	intstat;
};

<<<<<<< HEAD
#define chip2controller(chip)	\
	container_of(chip, struct davinci_gpio_controller, chip)

static struct davinci_gpio_controller chips[DIV_ROUND_UP(DAVINCI_N_GPIO, 32)];
static void __iomem *gpio_base;

static struct davinci_gpio_regs __iomem __init *gpio2regs(unsigned gpio)
{
	void __iomem *ptr;

	if (gpio < 32 * 1)
		ptr = gpio_base + 0x10;
	else if (gpio < 32 * 2)
		ptr = gpio_base + 0x38;
	else if (gpio < 32 * 3)
		ptr = gpio_base + 0x60;
	else if (gpio < 32 * 4)
		ptr = gpio_base + 0x88;
	else if (gpio < 32 * 5)
		ptr = gpio_base + 0xb0;
	else
		ptr = NULL;
	return ptr;
}

static inline struct davinci_gpio_regs __iomem *irq2regs(int irq)
{
	struct davinci_gpio_regs __iomem *g;

	g = (__force struct davinci_gpio_regs __iomem *)irq_get_chip_data(irq);
=======
typedef struct irq_chip *(*gpio_get_irq_chip_cb_t)(unsigned int irq);

#define BINTEN	0x8 /* GPIO Interrupt Per-Bank Enable Register */

static void __iomem *gpio_base;
static unsigned int offset_array[5] = {0x10, 0x38, 0x60, 0x88, 0xb0};

struct davinci_gpio_irq_data {
	void __iomem			*regs;
	struct davinci_gpio_controller	*chip;
	int				bank_num;
};

struct davinci_gpio_controller {
	struct gpio_chip	chip;
	struct irq_domain	*irq_domain;
	/* Serialize access to GPIO registers */
	spinlock_t		lock;
	void __iomem		*regs[MAX_REGS_BANKS];
	int			gpio_unbanked;
	int			irqs[MAX_INT_PER_BANK];
	struct davinci_gpio_regs context[MAX_REGS_BANKS];
	u32			binten_context;
};

static inline u32 __gpio_mask(unsigned gpio)
{
	return 1 << (gpio % 32);
}

static inline struct davinci_gpio_regs __iomem *irq2regs(struct irq_data *d)
{
	struct davinci_gpio_regs __iomem *g;

	g = (__force struct davinci_gpio_regs __iomem *)irq_data_get_irq_chip_data(d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return g;
}

<<<<<<< HEAD
static int __init davinci_gpio_irq_setup(void);
=======
static int davinci_gpio_irq_setup(struct platform_device *pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*--------------------------------------------------------------------------*/

/* board setup code *MUST* setup pinmux and enable the GPIO clock. */
static inline int __davinci_direction(struct gpio_chip *chip,
			unsigned offset, bool out, int value)
{
<<<<<<< HEAD
	struct davinci_gpio_controller *d = chip2controller(chip);
	struct davinci_gpio_regs __iomem *g = d->regs;
	unsigned long flags;
	u32 temp;
	u32 mask = 1 << offset;

	spin_lock_irqsave(&d->lock, flags);
	temp = __raw_readl(&g->dir);
	if (out) {
		temp &= ~mask;
		__raw_writel(mask, value ? &g->set_data : &g->clr_data);
	} else {
		temp |= mask;
	}
	__raw_writel(temp, &g->dir);
=======
	struct davinci_gpio_controller *d = gpiochip_get_data(chip);
	struct davinci_gpio_regs __iomem *g;
	unsigned long flags;
	u32 temp;
	int bank = offset / 32;
	u32 mask = __gpio_mask(offset);

	g = d->regs[bank];
	spin_lock_irqsave(&d->lock, flags);
	temp = readl_relaxed(&g->dir);
	if (out) {
		temp &= ~mask;
		writel_relaxed(mask, value ? &g->set_data : &g->clr_data);
	} else {
		temp |= mask;
	}
	writel_relaxed(temp, &g->dir);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irqrestore(&d->lock, flags);

	return 0;
}

static int davinci_direction_in(struct gpio_chip *chip, unsigned offset)
{
	return __davinci_direction(chip, offset, false, 0);
}

static int
davinci_direction_out(struct gpio_chip *chip, unsigned offset, int value)
{
	return __davinci_direction(chip, offset, true, value);
}

/*
 * Read the pin's value (works even if it's set up as output);
 * returns zero/nonzero.
 *
 * Note that changes are synched to the GPIO clock, so reading values back
 * right after you've set them may give old values.
 */
static int davinci_gpio_get(struct gpio_chip *chip, unsigned offset)
{
<<<<<<< HEAD
	struct davinci_gpio_controller *d = chip2controller(chip);
	struct davinci_gpio_regs __iomem *g = d->regs;

	return (1 << offset) & __raw_readl(&g->in_data);
=======
	struct davinci_gpio_controller *d = gpiochip_get_data(chip);
	struct davinci_gpio_regs __iomem *g;
	int bank = offset / 32;

	g = d->regs[bank];

	return !!(__gpio_mask(offset) & readl_relaxed(&g->in_data));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Assuming the pin is muxed as a gpio output, set its output value.
 */
static void
davinci_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
<<<<<<< HEAD
	struct davinci_gpio_controller *d = chip2controller(chip);
	struct davinci_gpio_regs __iomem *g = d->regs;

	__raw_writel((1 << offset), value ? &g->set_data : &g->clr_data);
}

static int __init davinci_gpio_setup(void)
{
	int i, base;
	unsigned ngpio;
	struct davinci_soc_info *soc_info = &davinci_soc_info;
	struct davinci_gpio_regs *regs;

	if (soc_info->gpio_type != GPIO_TYPE_DAVINCI)
		return 0;
=======
	struct davinci_gpio_controller *d = gpiochip_get_data(chip);
	struct davinci_gpio_regs __iomem *g;
	int bank = offset / 32;

	g = d->regs[bank];

	writel_relaxed(__gpio_mask(offset),
		       value ? &g->set_data : &g->clr_data);
}

static struct davinci_gpio_platform_data *
davinci_gpio_get_pdata(struct platform_device *pdev)
{
	struct device_node *dn = pdev->dev.of_node;
	struct davinci_gpio_platform_data *pdata;
	int ret;
	u32 val;

	if (!IS_ENABLED(CONFIG_OF) || !pdev->dev.of_node)
		return dev_get_platdata(&pdev->dev);

	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return NULL;

	ret = of_property_read_u32(dn, "ti,ngpio", &val);
	if (ret)
		goto of_err;

	pdata->ngpio = val;

	ret = of_property_read_u32(dn, "ti,davinci-gpio-unbanked", &val);
	if (ret)
		goto of_err;

	pdata->gpio_unbanked = val;

	return pdata;

of_err:
	dev_err(&pdev->dev, "Populating pdata from DT failed: err %d\n", ret);
	return NULL;
}

static int davinci_gpio_probe(struct platform_device *pdev)
{
	int bank, i, ret = 0;
	unsigned int ngpio, nbank, nirq;
	struct davinci_gpio_controller *chips;
	struct davinci_gpio_platform_data *pdata;
	struct device *dev = &pdev->dev;

	pdata = davinci_gpio_get_pdata(pdev);
	if (!pdata) {
		dev_err(dev, "No platform data found\n");
		return -EINVAL;
	}

	dev->platform_data = pdata;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The gpio banks conceptually expose a segmented bitmap,
	 * and "ngpio" is one more than the largest zero-based
	 * bit index that's valid.
	 */
<<<<<<< HEAD
	ngpio = soc_info->gpio_num;
	if (ngpio == 0) {
		pr_err("GPIO setup:  how many GPIOs?\n");
		return -EINVAL;
	}

	if (WARN_ON(DAVINCI_N_GPIO < ngpio))
		ngpio = DAVINCI_N_GPIO;

	gpio_base = ioremap(soc_info->gpio_base, SZ_4K);
	if (WARN_ON(!gpio_base))
		return -ENOMEM;

	for (i = 0, base = 0; base < ngpio; i++, base += 32) {
		chips[i].chip.label = "DaVinci";

		chips[i].chip.direction_input = davinci_direction_in;
		chips[i].chip.get = davinci_gpio_get;
		chips[i].chip.direction_output = davinci_direction_out;
		chips[i].chip.set = davinci_gpio_set;

		chips[i].chip.base = base;
		chips[i].chip.ngpio = ngpio - base;
		if (chips[i].chip.ngpio > 32)
			chips[i].chip.ngpio = 32;

		spin_lock_init(&chips[i].lock);

		regs = gpio2regs(base);
		chips[i].regs = regs;
		chips[i].set_data = &regs->set_data;
		chips[i].clr_data = &regs->clr_data;
		chips[i].in_data = &regs->in_data;

		gpiochip_add(&chips[i].chip);
	}

	soc_info->gpio_ctlrs = chips;
	soc_info->gpio_ctlrs_num = DIV_ROUND_UP(ngpio, 32);

	davinci_gpio_irq_setup();
	return 0;
}
pure_initcall(davinci_gpio_setup);
=======
	ngpio = pdata->ngpio;
	if (ngpio == 0) {
		dev_err(dev, "How many GPIOs?\n");
		return -EINVAL;
	}

	/*
	 * If there are unbanked interrupts then the number of
	 * interrupts is equal to number of gpios else all are banked so
	 * number of interrupts is equal to number of banks(each with 16 gpios)
	 */
	if (pdata->gpio_unbanked)
		nirq = pdata->gpio_unbanked;
	else
		nirq = DIV_ROUND_UP(ngpio, 16);

	chips = devm_kzalloc(dev, sizeof(*chips), GFP_KERNEL);
	if (!chips)
		return -ENOMEM;

	gpio_base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(gpio_base))
		return PTR_ERR(gpio_base);

	for (i = 0; i < nirq; i++) {
		chips->irqs[i] = platform_get_irq(pdev, i);
		if (chips->irqs[i] < 0)
			return chips->irqs[i];
	}

	chips->chip.label = dev_name(dev);

	chips->chip.direction_input = davinci_direction_in;
	chips->chip.get = davinci_gpio_get;
	chips->chip.direction_output = davinci_direction_out;
	chips->chip.set = davinci_gpio_set;

	chips->chip.ngpio = ngpio;
	chips->chip.base = pdata->no_auto_base ? pdata->base : -1;

#ifdef CONFIG_OF_GPIO
	chips->chip.parent = dev;
	chips->chip.request = gpiochip_generic_request;
	chips->chip.free = gpiochip_generic_free;
#endif
	spin_lock_init(&chips->lock);

	nbank = DIV_ROUND_UP(ngpio, 32);
	for (bank = 0; bank < nbank; bank++)
		chips->regs[bank] = gpio_base + offset_array[bank];

	ret = devm_gpiochip_add_data(dev, &chips->chip, chips);
	if (ret)
		return ret;

	platform_set_drvdata(pdev, chips);
	ret = davinci_gpio_irq_setup(pdev);
	if (ret)
		return ret;

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*--------------------------------------------------------------------------*/
/*
 * We expect irqs will normally be set up as input pins, but they can also be
 * used as output pins ... which is convenient for testing.
 *
 * NOTE:  The first few GPIOs also have direct INTC hookups in addition
 * to their GPIOBNK0 irq, with a bit less overhead.
 *
 * All those INTC hookups (direct, plus several IRQ banks) can also
 * serve as EDMA event triggers.
 */

static void gpio_irq_disable(struct irq_data *d)
{
<<<<<<< HEAD
	struct davinci_gpio_regs __iomem *g = irq2regs(d->irq);
	u32 mask = (u32) irq_data_get_irq_handler_data(d);

	__raw_writel(mask, &g->clr_falling);
	__raw_writel(mask, &g->clr_rising);
=======
	struct davinci_gpio_regs __iomem *g = irq2regs(d);
	uintptr_t mask = (uintptr_t)irq_data_get_irq_handler_data(d);

	writel_relaxed(mask, &g->clr_falling);
	writel_relaxed(mask, &g->clr_rising);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void gpio_irq_enable(struct irq_data *d)
{
<<<<<<< HEAD
	struct davinci_gpio_regs __iomem *g = irq2regs(d->irq);
	u32 mask = (u32) irq_data_get_irq_handler_data(d);
=======
	struct davinci_gpio_regs __iomem *g = irq2regs(d);
	uintptr_t mask = (uintptr_t)irq_data_get_irq_handler_data(d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned status = irqd_get_trigger_type(d);

	status &= IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING;
	if (!status)
		status = IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING;

	if (status & IRQ_TYPE_EDGE_FALLING)
<<<<<<< HEAD
		__raw_writel(mask, &g->set_falling);
	if (status & IRQ_TYPE_EDGE_RISING)
		__raw_writel(mask, &g->set_rising);
=======
		writel_relaxed(mask, &g->set_falling);
	if (status & IRQ_TYPE_EDGE_RISING)
		writel_relaxed(mask, &g->set_rising);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int gpio_irq_type(struct irq_data *d, unsigned trigger)
{
	if (trigger & ~(IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING))
		return -EINVAL;

	return 0;
}

static struct irq_chip gpio_irqchip = {
	.name		= "GPIO",
	.irq_enable	= gpio_irq_enable,
	.irq_disable	= gpio_irq_disable,
	.irq_set_type	= gpio_irq_type,
<<<<<<< HEAD
	.flags		= IRQCHIP_SET_TYPE_MASKED,
};

static void
gpio_irq_handler(unsigned irq, struct irq_desc *desc)
{
	struct davinci_gpio_regs __iomem *g;
	u32 mask = 0xffff;
	struct davinci_gpio_controller *d;

	d = (struct davinci_gpio_controller *)irq_desc_get_handler_data(desc);
	g = (struct davinci_gpio_regs __iomem *)d->regs;

	/* we only care about one bank */
	if (irq & 1)
		mask <<= 16;

	/* temporarily mask (level sensitive) parent IRQ */
	desc->irq_data.chip->irq_mask(&desc->irq_data);
	desc->irq_data.chip->irq_ack(&desc->irq_data);
	while (1) {
		u32		status;
		int		n;
		int		res;

		/* ack any irqs */
		status = __raw_readl(&g->intstat) & mask;
		if (!status)
			break;
		__raw_writel(status, &g->intstat);

		/* now demux them to the right lowlevel handler */
		n = d->irq_base;
		if (irq & 1) {
			n += 16;
			status >>= 16;
		}

		while (status) {
			res = ffs(status);
			n += res;
			generic_handle_irq(n - 1);
			status >>= res;
		}
	}
	desc->irq_data.chip->irq_unmask(&desc->irq_data);
=======
	.flags		= IRQCHIP_SET_TYPE_MASKED | IRQCHIP_SKIP_SET_WAKE,
};

static void gpio_irq_handler(struct irq_desc *desc)
{
	struct davinci_gpio_regs __iomem *g;
	u32 mask = 0xffff;
	int bank_num;
	struct davinci_gpio_controller *d;
	struct davinci_gpio_irq_data *irqdata;

	irqdata = (struct davinci_gpio_irq_data *)irq_desc_get_handler_data(desc);
	bank_num = irqdata->bank_num;
	g = irqdata->regs;
	d = irqdata->chip;

	/* we only care about one bank */
	if ((bank_num % 2) == 1)
		mask <<= 16;

	/* temporarily mask (level sensitive) parent IRQ */
	chained_irq_enter(irq_desc_get_chip(desc), desc);
	while (1) {
		u32		status;
		int		bit;
		irq_hw_number_t hw_irq;

		/* ack any irqs */
		status = readl_relaxed(&g->intstat) & mask;
		if (!status)
			break;
		writel_relaxed(status, &g->intstat);

		/* now demux them to the right lowlevel handler */

		while (status) {
			bit = __ffs(status);
			status &= ~BIT(bit);
			/* Max number of gpios per controller is 144 so
			 * hw_irq will be in [0..143]
			 */
			hw_irq = (bank_num / 2) * 32 + bit;

			generic_handle_domain_irq(d->irq_domain, hw_irq);
		}
	}
	chained_irq_exit(irq_desc_get_chip(desc), desc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* now it may re-trigger */
}

static int gpio_to_irq_banked(struct gpio_chip *chip, unsigned offset)
{
<<<<<<< HEAD
	struct davinci_gpio_controller *d = chip2controller(chip);

	if (d->irq_base >= 0)
		return d->irq_base + offset;
	else
		return -ENODEV;
=======
	struct davinci_gpio_controller *d = gpiochip_get_data(chip);

	if (d->irq_domain)
		return irq_create_mapping(d->irq_domain, offset);
	else
		return -ENXIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int gpio_to_irq_unbanked(struct gpio_chip *chip, unsigned offset)
{
<<<<<<< HEAD
	struct davinci_soc_info *soc_info = &davinci_soc_info;

	/* NOTE:  we assume for now that only irqs in the first gpio_chip
	 * can provide direct-mapped IRQs to AINTC (up to 32 GPIOs).
	 */
	if (offset < soc_info->gpio_unbanked)
		return soc_info->gpio_irq + offset;
=======
	struct davinci_gpio_controller *d = gpiochip_get_data(chip);

	/*
	 * NOTE:  we assume for now that only irqs in the first gpio_chip
	 * can provide direct-mapped IRQs to AINTC (up to 32 GPIOs).
	 */
	if (offset < d->gpio_unbanked)
		return d->irqs[offset];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		return -ENODEV;
}

static int gpio_irq_type_unbanked(struct irq_data *data, unsigned trigger)
{
	struct davinci_gpio_controller *d;
	struct davinci_gpio_regs __iomem *g;
<<<<<<< HEAD
	struct davinci_soc_info *soc_info = &davinci_soc_info;
	u32 mask;

	d = (struct davinci_gpio_controller *)data->handler_data;
	g = (struct davinci_gpio_regs __iomem *)d->regs;
	mask = __gpio_mask(data->irq - soc_info->gpio_irq);
=======
	u32 mask, i;

	d = (struct davinci_gpio_controller *)irq_data_get_irq_handler_data(data);
	g = (struct davinci_gpio_regs __iomem *)d->regs[0];
	for (i = 0; i < MAX_INT_PER_BANK; i++)
		if (data->irq == d->irqs[i])
			break;

	if (i == MAX_INT_PER_BANK)
		return -EINVAL;

	mask = __gpio_mask(i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (trigger & ~(IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING))
		return -EINVAL;

<<<<<<< HEAD
	__raw_writel(mask, (trigger & IRQ_TYPE_EDGE_FALLING)
		     ? &g->set_falling : &g->clr_falling);
	__raw_writel(mask, (trigger & IRQ_TYPE_EDGE_RISING)
=======
	writel_relaxed(mask, (trigger & IRQ_TYPE_EDGE_FALLING)
		     ? &g->set_falling : &g->clr_falling);
	writel_relaxed(mask, (trigger & IRQ_TYPE_EDGE_RISING)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		     ? &g->set_rising : &g->clr_rising);

	return 0;
}

<<<<<<< HEAD
=======
static int
davinci_gpio_irq_map(struct irq_domain *d, unsigned int irq,
		     irq_hw_number_t hw)
{
	struct davinci_gpio_controller *chips =
				(struct davinci_gpio_controller *)d->host_data;
	struct davinci_gpio_regs __iomem *g = chips->regs[hw / 32];

	irq_set_chip_and_handler_name(irq, &gpio_irqchip, handle_simple_irq,
				"davinci_gpio");
	irq_set_irq_type(irq, IRQ_TYPE_NONE);
	irq_set_chip_data(irq, (__force void *)g);
	irq_set_handler_data(irq, (void *)(uintptr_t)__gpio_mask(hw));

	return 0;
}

static const struct irq_domain_ops davinci_gpio_irq_ops = {
	.map = davinci_gpio_irq_map,
	.xlate = irq_domain_xlate_onetwocell,
};

static struct irq_chip *davinci_gpio_get_irq_chip(unsigned int irq)
{
	static struct irq_chip_type gpio_unbanked;

	gpio_unbanked = *irq_data_get_chip_type(irq_get_irq_data(irq));

	return &gpio_unbanked.chip;
};

static struct irq_chip *keystone_gpio_get_irq_chip(unsigned int irq)
{
	static struct irq_chip gpio_unbanked;

	gpio_unbanked = *irq_get_chip(irq);
	return &gpio_unbanked;
};

static const struct of_device_id davinci_gpio_ids[];

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * NOTE:  for suspend/resume, probably best to make a platform_device with
 * suspend_late/resume_resume calls hooking into results of the set_wake()
 * calls ... so if no gpios are wakeup events the clock can be disabled,
 * with outputs left at previously set levels, and so that VDD3P3V.IOPWDN0
 * (dm6446) can be set appropriately for GPIOV33 pins.
 */

<<<<<<< HEAD
static int __init davinci_gpio_irq_setup(void)
{
	unsigned	gpio, irq, bank;
	struct clk	*clk;
	u32		binten = 0;
	unsigned	ngpio, bank_irq;
	struct davinci_soc_info *soc_info = &davinci_soc_info;
	struct davinci_gpio_regs	__iomem *g;

	ngpio = soc_info->gpio_num;

	bank_irq = soc_info->gpio_irq;
	if (bank_irq == 0) {
		printk(KERN_ERR "Don't know first GPIO bank IRQ.\n");
		return -EINVAL;
	}

	clk = clk_get(NULL, "gpio");
	if (IS_ERR(clk)) {
		printk(KERN_ERR "Error %ld getting gpio clock?\n",
		       PTR_ERR(clk));
		return PTR_ERR(clk);
	}
	clk_enable(clk);

	/* Arrange gpio_to_irq() support, handling either direct IRQs or
=======
static int davinci_gpio_irq_setup(struct platform_device *pdev)
{
	unsigned	gpio, bank;
	int		irq;
	int		ret;
	struct clk	*clk;
	u32		binten = 0;
	unsigned	ngpio;
	struct device *dev = &pdev->dev;
	struct davinci_gpio_controller *chips = platform_get_drvdata(pdev);
	struct davinci_gpio_platform_data *pdata = dev->platform_data;
	struct davinci_gpio_regs __iomem *g;
	struct irq_domain	*irq_domain = NULL;
	struct irq_chip *irq_chip;
	struct davinci_gpio_irq_data *irqdata;
	gpio_get_irq_chip_cb_t gpio_get_irq_chip;

	/*
	 * Use davinci_gpio_get_irq_chip by default to handle non DT cases
	 */
	gpio_get_irq_chip = davinci_gpio_get_irq_chip;
	if (dev->of_node)
		gpio_get_irq_chip = (gpio_get_irq_chip_cb_t)device_get_match_data(dev);

	ngpio = pdata->ngpio;

	clk = devm_clk_get(dev, "gpio");
	if (IS_ERR(clk)) {
		dev_err(dev, "Error %ld getting gpio clock\n", PTR_ERR(clk));
		return PTR_ERR(clk);
	}

	ret = clk_prepare_enable(clk);
	if (ret)
		return ret;

	if (!pdata->gpio_unbanked) {
		irq = devm_irq_alloc_descs(dev, -1, 0, ngpio, 0);
		if (irq < 0) {
			dev_err(dev, "Couldn't allocate IRQ numbers\n");
			clk_disable_unprepare(clk);
			return irq;
		}

		irq_domain = irq_domain_add_legacy(dev->of_node, ngpio, irq, 0,
							&davinci_gpio_irq_ops,
							chips);
		if (!irq_domain) {
			dev_err(dev, "Couldn't register an IRQ domain\n");
			clk_disable_unprepare(clk);
			return -ENODEV;
		}
	}

	/*
	 * Arrange gpiod_to_irq() support, handling either direct IRQs or
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * banked IRQs.  Having GPIOs in the first GPIO bank use direct
	 * IRQs, while the others use banked IRQs, would need some setup
	 * tweaks to recognize hardware which can do that.
	 */
<<<<<<< HEAD
	for (gpio = 0, bank = 0; gpio < ngpio; bank++, gpio += 32) {
		chips[bank].chip.to_irq = gpio_to_irq_banked;
		chips[bank].irq_base = soc_info->gpio_unbanked
			? -EINVAL
			: (soc_info->intc_irq_num + gpio);
	}
=======
	chips->chip.to_irq = gpio_to_irq_banked;
	chips->irq_domain = irq_domain;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * AINTC can handle direct/unbanked IRQs for GPIOs, with the GPIO
	 * controller only handling trigger modes.  We currently assume no
	 * IRQ mux conflicts; gpio_irq_type_unbanked() is only for GPIOs.
	 */
<<<<<<< HEAD
	if (soc_info->gpio_unbanked) {
		static struct irq_chip_type gpio_unbanked;

		/* pass "bank 0" GPIO IRQs to AINTC */
		chips[0].chip.to_irq = gpio_to_irq_unbanked;
		binten = BIT(0);

		/* AINTC handles mask/unmask; GPIO handles triggering */
		irq = bank_irq;
		gpio_unbanked = *container_of(irq_get_chip(irq),
					      struct irq_chip_type, chip);
		gpio_unbanked.chip.name = "GPIO-AINTC";
		gpio_unbanked.chip.irq_set_type = gpio_irq_type_unbanked;

		/* default trigger: both edges */
		g = gpio2regs(0);
		__raw_writel(~0, &g->set_falling);
		__raw_writel(~0, &g->set_rising);

		/* set the direct IRQs up to use that irqchip */
		for (gpio = 0; gpio < soc_info->gpio_unbanked; gpio++, irq++) {
			irq_set_chip(irq, &gpio_unbanked.chip);
			irq_set_handler_data(irq, &chips[gpio / 32]);
			irq_set_status_flags(irq, IRQ_TYPE_EDGE_BOTH);
=======
	if (pdata->gpio_unbanked) {
		/* pass "bank 0" GPIO IRQs to AINTC */
		chips->chip.to_irq = gpio_to_irq_unbanked;
		chips->gpio_unbanked = pdata->gpio_unbanked;
		binten = GENMASK(pdata->gpio_unbanked / 16, 0);

		/* AINTC handles mask/unmask; GPIO handles triggering */
		irq = chips->irqs[0];
		irq_chip = gpio_get_irq_chip(irq);
		irq_chip->name = "GPIO-AINTC";
		irq_chip->irq_set_type = gpio_irq_type_unbanked;

		/* default trigger: both edges */
		g = chips->regs[0];
		writel_relaxed(~0, &g->set_falling);
		writel_relaxed(~0, &g->set_rising);

		/* set the direct IRQs up to use that irqchip */
		for (gpio = 0; gpio < pdata->gpio_unbanked; gpio++) {
			irq_set_chip(chips->irqs[gpio], irq_chip);
			irq_set_handler_data(chips->irqs[gpio], chips);
			irq_set_status_flags(chips->irqs[gpio],
					     IRQ_TYPE_EDGE_BOTH);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		goto done;
	}

	/*
	 * Or, AINTC can handle IRQs for banks of 16 GPIO IRQs, which we
	 * then chain through our own handler.
	 */
<<<<<<< HEAD
	for (gpio = 0, irq = gpio_to_irq(0), bank = 0;
			gpio < ngpio;
			bank++, bank_irq++) {
		unsigned		i;

		/* disabled by default, enabled only as needed */
		g = gpio2regs(gpio);
		__raw_writel(~0, &g->clr_falling);
		__raw_writel(~0, &g->clr_rising);

		/* set up all irqs in this bank */
		irq_set_chained_handler(bank_irq, gpio_irq_handler);
=======
	for (gpio = 0, bank = 0; gpio < ngpio; bank++, gpio += 16) {
		/* disabled by default, enabled only as needed
		 * There are register sets for 32 GPIOs. 2 banks of 16
		 * GPIOs are covered by each set of registers hence divide by 2
		 */
		g = chips->regs[bank / 2];
		writel_relaxed(~0, &g->clr_falling);
		writel_relaxed(~0, &g->clr_rising);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Each chip handles 32 gpios, and each irq bank consists of 16
		 * gpio irqs. Pass the irq bank's corresponding controller to
		 * the chained irq handler.
		 */
<<<<<<< HEAD
		irq_set_handler_data(bank_irq, &chips[gpio / 32]);

		for (i = 0; i < 16 && gpio < ngpio; i++, irq++, gpio++) {
			irq_set_chip(irq, &gpio_irqchip);
			irq_set_chip_data(irq, (__force void *)g);
			irq_set_handler_data(irq, (void *)__gpio_mask(gpio));
			irq_set_handler(irq, handle_simple_irq);
			set_irq_flags(irq, IRQF_VALID);
		}

=======
		irqdata = devm_kzalloc(&pdev->dev,
				       sizeof(struct
					      davinci_gpio_irq_data),
					      GFP_KERNEL);
		if (!irqdata) {
			clk_disable_unprepare(clk);
			return -ENOMEM;
		}

		irqdata->regs = g;
		irqdata->bank_num = bank;
		irqdata->chip = chips;

		irq_set_chained_handler_and_data(chips->irqs[bank],
						 gpio_irq_handler, irqdata);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		binten |= BIT(bank);
	}

done:
<<<<<<< HEAD
	/* BINTEN -- per-bank interrupt enable. genirq would also let these
	 * bits be set/cleared dynamically.
	 */
	__raw_writel(binten, gpio_base + 0x08);

	printk(KERN_INFO "DaVinci: %d gpio irqs\n", irq - gpio_to_irq(0));

	return 0;
}
=======
	/*
	 * BINTEN -- per-bank interrupt enable. genirq would also let these
	 * bits be set/cleared dynamically.
	 */
	writel_relaxed(binten, gpio_base + BINTEN);

	return 0;
}

static void davinci_gpio_save_context(struct davinci_gpio_controller *chips,
				      u32 nbank)
{
	struct davinci_gpio_regs __iomem *g;
	struct davinci_gpio_regs *context;
	u32 bank;
	void __iomem *base;

	base = chips->regs[0] - offset_array[0];
	chips->binten_context = readl_relaxed(base + BINTEN);

	for (bank = 0; bank < nbank; bank++) {
		g = chips->regs[bank];
		context = &chips->context[bank];
		context->dir = readl_relaxed(&g->dir);
		context->set_data = readl_relaxed(&g->set_data);
		context->set_rising = readl_relaxed(&g->set_rising);
		context->set_falling = readl_relaxed(&g->set_falling);
	}

	/* Clear all interrupt status registers */
	writel_relaxed(GENMASK(31, 0), &g->intstat);
}

static void davinci_gpio_restore_context(struct davinci_gpio_controller *chips,
					 u32 nbank)
{
	struct davinci_gpio_regs __iomem *g;
	struct davinci_gpio_regs *context;
	u32 bank;
	void __iomem *base;

	base = chips->regs[0] - offset_array[0];

	if (readl_relaxed(base + BINTEN) != chips->binten_context)
		writel_relaxed(chips->binten_context, base + BINTEN);

	for (bank = 0; bank < nbank; bank++) {
		g = chips->regs[bank];
		context = &chips->context[bank];
		if (readl_relaxed(&g->dir) != context->dir)
			writel_relaxed(context->dir, &g->dir);
		if (readl_relaxed(&g->set_data) != context->set_data)
			writel_relaxed(context->set_data, &g->set_data);
		if (readl_relaxed(&g->set_rising) != context->set_rising)
			writel_relaxed(context->set_rising, &g->set_rising);
		if (readl_relaxed(&g->set_falling) != context->set_falling)
			writel_relaxed(context->set_falling, &g->set_falling);
	}
}

static int davinci_gpio_suspend(struct device *dev)
{
	struct davinci_gpio_controller *chips = dev_get_drvdata(dev);
	struct davinci_gpio_platform_data *pdata = dev_get_platdata(dev);
	u32 nbank = DIV_ROUND_UP(pdata->ngpio, 32);

	davinci_gpio_save_context(chips, nbank);

	return 0;
}

static int davinci_gpio_resume(struct device *dev)
{
	struct davinci_gpio_controller *chips = dev_get_drvdata(dev);
	struct davinci_gpio_platform_data *pdata = dev_get_platdata(dev);
	u32 nbank = DIV_ROUND_UP(pdata->ngpio, 32);

	davinci_gpio_restore_context(chips, nbank);

	return 0;
}

static DEFINE_SIMPLE_DEV_PM_OPS(davinci_gpio_dev_pm_ops, davinci_gpio_suspend,
			 davinci_gpio_resume);

static const struct of_device_id davinci_gpio_ids[] = {
	{ .compatible = "ti,keystone-gpio", keystone_gpio_get_irq_chip},
	{ .compatible = "ti,am654-gpio", keystone_gpio_get_irq_chip},
	{ .compatible = "ti,dm6441-gpio", davinci_gpio_get_irq_chip},
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, davinci_gpio_ids);

static struct platform_driver davinci_gpio_driver = {
	.probe		= davinci_gpio_probe,
	.driver		= {
		.name		= "davinci_gpio",
		.pm = pm_sleep_ptr(&davinci_gpio_dev_pm_ops),
		.of_match_table	= of_match_ptr(davinci_gpio_ids),
	},
};

/*
 * GPIO driver registration needs to be done before machine_init functions
 * access GPIO. Hence davinci_gpio_drv_reg() is a postcore_initcall.
 */
static int __init davinci_gpio_drv_reg(void)
{
	return platform_driver_register(&davinci_gpio_driver);
}
postcore_initcall(davinci_gpio_drv_reg);

static void __exit davinci_gpio_exit(void)
{
	platform_driver_unregister(&davinci_gpio_driver);
}
module_exit(davinci_gpio_exit);

MODULE_AUTHOR("Jan Kotas <jank@cadence.com>");
MODULE_DESCRIPTION("DAVINCI GPIO driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:gpio-davinci");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
