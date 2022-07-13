<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generic EP93xx GPIO handling
 *
 * Copyright (c) 2008 Ryan Mallon
 * Copyright (c) 2011 H Hartley Sweeten <hsweeten@visionengravers.com>
 *
 * Based on code originally from:
 *  linux/arch/arm/mach-ep93xx/core.c
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/basic_mmio_gpio.h>

#include <mach/hardware.h>
#include <mach/gpio-ep93xx.h>

#define irq_to_gpio(irq)	((irq) - gpio_to_irq(0))

struct ep93xx_gpio {
	void __iomem		*mmio_base;
	struct bgpio_chip	bgc[8];
};

/*************************************************************************
 * Interrupt handling for EP93xx on-chip GPIOs
 *************************************************************************/
static unsigned char gpio_int_unmasked[3];
static unsigned char gpio_int_enabled[3];
static unsigned char gpio_int_type1[3];
static unsigned char gpio_int_type2[3];
static unsigned char gpio_int_debounce[3];

/* Port ordering is: A B F */
static const u8 int_type1_register_offset[3]	= { 0x90, 0xac, 0x4c };
static const u8 int_type2_register_offset[3]	= { 0x94, 0xb0, 0x50 };
static const u8 eoi_register_offset[3]		= { 0x98, 0xb4, 0x54 };
static const u8 int_en_register_offset[3]	= { 0x9c, 0xb8, 0x58 };
static const u8 int_debounce_register_offset[3]	= { 0xa8, 0xc4, 0x64 };

static void ep93xx_gpio_update_int_params(unsigned port)
{
	BUG_ON(port > 2);

	__raw_writeb(0, EP93XX_GPIO_REG(int_en_register_offset[port]));

	__raw_writeb(gpio_int_type2[port],
		EP93XX_GPIO_REG(int_type2_register_offset[port]));

	__raw_writeb(gpio_int_type1[port],
		EP93XX_GPIO_REG(int_type1_register_offset[port]));

	__raw_writeb(gpio_int_unmasked[port] & gpio_int_enabled[port],
		EP93XX_GPIO_REG(int_en_register_offset[port]));
}

static void ep93xx_gpio_int_debounce(unsigned int irq, bool enable)
{
	int line = irq_to_gpio(irq);
	int port = line >> 3;
	int port_mask = 1 << (line & 7);

	if (enable)
		gpio_int_debounce[port] |= port_mask;
	else
		gpio_int_debounce[port] &= ~port_mask;

	__raw_writeb(gpio_int_debounce[port],
		EP93XX_GPIO_REG(int_debounce_register_offset[port]));
}

static void ep93xx_gpio_ab_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	unsigned char status;
	int i;

	status = __raw_readb(EP93XX_GPIO_A_INT_STATUS);
	for (i = 0; i < 8; i++) {
		if (status & (1 << i)) {
			int gpio_irq = gpio_to_irq(EP93XX_GPIO_LINE_A(0)) + i;
			generic_handle_irq(gpio_irq);
		}
	}

	status = __raw_readb(EP93XX_GPIO_B_INT_STATUS);
	for (i = 0; i < 8; i++) {
		if (status & (1 << i)) {
			int gpio_irq = gpio_to_irq(EP93XX_GPIO_LINE_B(0)) + i;
			generic_handle_irq(gpio_irq);
		}
	}
}

static void ep93xx_gpio_f_irq_handler(unsigned int irq, struct irq_desc *desc)
=======
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/gpio/driver.h>
#include <linux/bitops.h>
#include <linux/seq_file.h>

#define EP93XX_GPIO_F_INT_STATUS 0x5c
#define EP93XX_GPIO_A_INT_STATUS 0xa0
#define EP93XX_GPIO_B_INT_STATUS 0xbc

/* Maximum value for gpio line identifiers */
#define EP93XX_GPIO_LINE_MAX 63

/* Number of GPIO chips in EP93XX */
#define EP93XX_GPIO_CHIP_NUM 8

/* Maximum value for irq capable line identifiers */
#define EP93XX_GPIO_LINE_MAX_IRQ 23

#define EP93XX_GPIO_A_IRQ_BASE 64
#define EP93XX_GPIO_B_IRQ_BASE 72
/*
 * Static mapping of GPIO bank F IRQS:
 * F0..F7 (16..24) to irq 80..87.
 */
#define EP93XX_GPIO_F_IRQ_BASE 80

struct ep93xx_gpio_irq_chip {
	u8 irq_offset;
	u8 int_unmasked;
	u8 int_enabled;
	u8 int_type1;
	u8 int_type2;
	u8 int_debounce;
};

struct ep93xx_gpio_chip {
	struct gpio_chip		gc;
	struct ep93xx_gpio_irq_chip	*eic;
};

struct ep93xx_gpio {
	void __iomem		*base;
	struct ep93xx_gpio_chip	gc[EP93XX_GPIO_CHIP_NUM];
};

#define to_ep93xx_gpio_chip(x) container_of(x, struct ep93xx_gpio_chip, gc)

static struct ep93xx_gpio_irq_chip *to_ep93xx_gpio_irq_chip(struct gpio_chip *gc)
{
	struct ep93xx_gpio_chip *egc = to_ep93xx_gpio_chip(gc);

	return egc->eic;
}

/*************************************************************************
 * Interrupt handling for EP93xx on-chip GPIOs
 *************************************************************************/
#define EP93XX_INT_TYPE1_OFFSET		0x00
#define EP93XX_INT_TYPE2_OFFSET		0x04
#define EP93XX_INT_EOI_OFFSET		0x08
#define EP93XX_INT_EN_OFFSET		0x0c
#define EP93XX_INT_STATUS_OFFSET	0x10
#define EP93XX_INT_RAW_STATUS_OFFSET	0x14
#define EP93XX_INT_DEBOUNCE_OFFSET	0x18

static void ep93xx_gpio_update_int_params(struct ep93xx_gpio *epg,
					  struct ep93xx_gpio_irq_chip *eic)
{
	writeb_relaxed(0, epg->base + eic->irq_offset + EP93XX_INT_EN_OFFSET);

	writeb_relaxed(eic->int_type2,
		       epg->base + eic->irq_offset + EP93XX_INT_TYPE2_OFFSET);

	writeb_relaxed(eic->int_type1,
		       epg->base + eic->irq_offset + EP93XX_INT_TYPE1_OFFSET);

	writeb_relaxed(eic->int_unmasked & eic->int_enabled,
		       epg->base + eic->irq_offset + EP93XX_INT_EN_OFFSET);
}

static void ep93xx_gpio_int_debounce(struct gpio_chip *gc,
				     unsigned int offset, bool enable)
{
	struct ep93xx_gpio *epg = gpiochip_get_data(gc);
	struct ep93xx_gpio_irq_chip *eic = to_ep93xx_gpio_irq_chip(gc);
	int port_mask = BIT(offset);

	if (enable)
		eic->int_debounce |= port_mask;
	else
		eic->int_debounce &= ~port_mask;

	writeb(eic->int_debounce,
	       epg->base + eic->irq_offset + EP93XX_INT_DEBOUNCE_OFFSET);
}

static void ep93xx_gpio_ab_irq_handler(struct irq_desc *desc)
{
	struct gpio_chip *gc = irq_desc_get_handler_data(desc);
	struct ep93xx_gpio *epg = gpiochip_get_data(gc);
	struct irq_chip *irqchip = irq_desc_get_chip(desc);
	unsigned long stat;
	int offset;

	chained_irq_enter(irqchip, desc);

	/*
	 * Dispatch the IRQs to the irqdomain of each A and B
	 * gpiochip irqdomains depending on what has fired.
	 * The tricky part is that the IRQ line is shared
	 * between bank A and B and each has their own gpiochip.
	 */
	stat = readb(epg->base + EP93XX_GPIO_A_INT_STATUS);
	for_each_set_bit(offset, &stat, 8)
		generic_handle_domain_irq(epg->gc[0].gc.irq.domain,
					  offset);

	stat = readb(epg->base + EP93XX_GPIO_B_INT_STATUS);
	for_each_set_bit(offset, &stat, 8)
		generic_handle_domain_irq(epg->gc[1].gc.irq.domain,
					  offset);

	chained_irq_exit(irqchip, desc);
}

static void ep93xx_gpio_f_irq_handler(struct irq_desc *desc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * map discontiguous hw irq range to continuous sw irq range:
	 *
<<<<<<< HEAD
	 *  IRQ_EP93XX_GPIO{0..7}MUX -> gpio_to_irq(EP93XX_GPIO_LINE_F({0..7})
	 */
	int port_f_idx = ((irq + 1) & 7) ^ 4; /* {19..22,47..50} -> {0..7} */
	int gpio_irq = gpio_to_irq(EP93XX_GPIO_LINE_F(0)) + port_f_idx;

	generic_handle_irq(gpio_irq);
=======
	 *  IRQ_EP93XX_GPIO{0..7}MUX -> EP93XX_GPIO_LINE_F{0..7}
	 */
	struct irq_chip *irqchip = irq_desc_get_chip(desc);
	unsigned int irq = irq_desc_get_irq(desc);
	int port_f_idx = (irq & 7) ^ 4; /* {20..23,48..51} -> {0..7} */
	int gpio_irq = EP93XX_GPIO_F_IRQ_BASE + port_f_idx;

	chained_irq_enter(irqchip, desc);
	generic_handle_irq(gpio_irq);
	chained_irq_exit(irqchip, desc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ep93xx_gpio_irq_ack(struct irq_data *d)
{
<<<<<<< HEAD
	int line = irq_to_gpio(d->irq);
	int port = line >> 3;
	int port_mask = 1 << (line & 7);

	if (irqd_get_trigger_type(d) == IRQ_TYPE_EDGE_BOTH) {
		gpio_int_type2[port] ^= port_mask; /* switch edge direction */
		ep93xx_gpio_update_int_params(port);
	}

	__raw_writeb(port_mask, EP93XX_GPIO_REG(eoi_register_offset[port]));
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct ep93xx_gpio_irq_chip *eic = to_ep93xx_gpio_irq_chip(gc);
	struct ep93xx_gpio *epg = gpiochip_get_data(gc);
	int port_mask = BIT(d->irq & 7);

	if (irqd_get_trigger_type(d) == IRQ_TYPE_EDGE_BOTH) {
		eic->int_type2 ^= port_mask; /* switch edge direction */
		ep93xx_gpio_update_int_params(epg, eic);
	}

	writeb(port_mask, epg->base + eic->irq_offset + EP93XX_INT_EOI_OFFSET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ep93xx_gpio_irq_mask_ack(struct irq_data *d)
{
<<<<<<< HEAD
	int line = irq_to_gpio(d->irq);
	int port = line >> 3;
	int port_mask = 1 << (line & 7);

	if (irqd_get_trigger_type(d) == IRQ_TYPE_EDGE_BOTH)
		gpio_int_type2[port] ^= port_mask; /* switch edge direction */

	gpio_int_unmasked[port] &= ~port_mask;
	ep93xx_gpio_update_int_params(port);

	__raw_writeb(port_mask, EP93XX_GPIO_REG(eoi_register_offset[port]));
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct ep93xx_gpio_irq_chip *eic = to_ep93xx_gpio_irq_chip(gc);
	struct ep93xx_gpio *epg = gpiochip_get_data(gc);
	int port_mask = BIT(d->irq & 7);

	if (irqd_get_trigger_type(d) == IRQ_TYPE_EDGE_BOTH)
		eic->int_type2 ^= port_mask; /* switch edge direction */

	eic->int_unmasked &= ~port_mask;
	ep93xx_gpio_update_int_params(epg, eic);

	writeb(port_mask, epg->base + eic->irq_offset + EP93XX_INT_EOI_OFFSET);
	gpiochip_disable_irq(gc, irqd_to_hwirq(d));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ep93xx_gpio_irq_mask(struct irq_data *d)
{
<<<<<<< HEAD
	int line = irq_to_gpio(d->irq);
	int port = line >> 3;

	gpio_int_unmasked[port] &= ~(1 << (line & 7));
	ep93xx_gpio_update_int_params(port);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct ep93xx_gpio_irq_chip *eic = to_ep93xx_gpio_irq_chip(gc);
	struct ep93xx_gpio *epg = gpiochip_get_data(gc);

	eic->int_unmasked &= ~BIT(d->irq & 7);
	ep93xx_gpio_update_int_params(epg, eic);
	gpiochip_disable_irq(gc, irqd_to_hwirq(d));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ep93xx_gpio_irq_unmask(struct irq_data *d)
{
<<<<<<< HEAD
	int line = irq_to_gpio(d->irq);
	int port = line >> 3;

	gpio_int_unmasked[port] |= 1 << (line & 7);
	ep93xx_gpio_update_int_params(port);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct ep93xx_gpio_irq_chip *eic = to_ep93xx_gpio_irq_chip(gc);
	struct ep93xx_gpio *epg = gpiochip_get_data(gc);

	gpiochip_enable_irq(gc, irqd_to_hwirq(d));
	eic->int_unmasked |= BIT(d->irq & 7);
	ep93xx_gpio_update_int_params(epg, eic);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * gpio_int_type1 controls whether the interrupt is level (0) or
 * edge (1) triggered, while gpio_int_type2 controls whether it
 * triggers on low/falling (0) or high/rising (1).
 */
static int ep93xx_gpio_irq_type(struct irq_data *d, unsigned int type)
{
<<<<<<< HEAD
	const int gpio = irq_to_gpio(d->irq);
	const int port = gpio >> 3;
	const int port_mask = 1 << (gpio & 7);
	irq_flow_handler_t handler;

	gpio_direction_input(gpio);

	switch (type) {
	case IRQ_TYPE_EDGE_RISING:
		gpio_int_type1[port] |= port_mask;
		gpio_int_type2[port] |= port_mask;
		handler = handle_edge_irq;
		break;
	case IRQ_TYPE_EDGE_FALLING:
		gpio_int_type1[port] |= port_mask;
		gpio_int_type2[port] &= ~port_mask;
		handler = handle_edge_irq;
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		gpio_int_type1[port] &= ~port_mask;
		gpio_int_type2[port] |= port_mask;
		handler = handle_level_irq;
		break;
	case IRQ_TYPE_LEVEL_LOW:
		gpio_int_type1[port] &= ~port_mask;
		gpio_int_type2[port] &= ~port_mask;
		handler = handle_level_irq;
		break;
	case IRQ_TYPE_EDGE_BOTH:
		gpio_int_type1[port] |= port_mask;
		/* set initial polarity based on current input level */
		if (gpio_get_value(gpio))
			gpio_int_type2[port] &= ~port_mask; /* falling */
		else
			gpio_int_type2[port] |= port_mask; /* rising */
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct ep93xx_gpio_irq_chip *eic = to_ep93xx_gpio_irq_chip(gc);
	struct ep93xx_gpio *epg = gpiochip_get_data(gc);
	int offset = d->irq & 7;
	int port_mask = BIT(offset);
	irq_flow_handler_t handler;

	gc->direction_input(gc, offset);

	switch (type) {
	case IRQ_TYPE_EDGE_RISING:
		eic->int_type1 |= port_mask;
		eic->int_type2 |= port_mask;
		handler = handle_edge_irq;
		break;
	case IRQ_TYPE_EDGE_FALLING:
		eic->int_type1 |= port_mask;
		eic->int_type2 &= ~port_mask;
		handler = handle_edge_irq;
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		eic->int_type1 &= ~port_mask;
		eic->int_type2 |= port_mask;
		handler = handle_level_irq;
		break;
	case IRQ_TYPE_LEVEL_LOW:
		eic->int_type1 &= ~port_mask;
		eic->int_type2 &= ~port_mask;
		handler = handle_level_irq;
		break;
	case IRQ_TYPE_EDGE_BOTH:
		eic->int_type1 |= port_mask;
		/* set initial polarity based on current input level */
		if (gc->get(gc, offset))
			eic->int_type2 &= ~port_mask; /* falling */
		else
			eic->int_type2 |= port_mask; /* rising */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		handler = handle_edge_irq;
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	__irq_set_handler_locked(d->irq, handler);

	gpio_int_enabled[port] |= port_mask;

	ep93xx_gpio_update_int_params(port);
=======
	irq_set_handler_locked(d, handler);

	eic->int_enabled |= port_mask;

	ep93xx_gpio_update_int_params(epg, eic);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static struct irq_chip ep93xx_gpio_irq_chip = {
	.name		= "GPIO",
	.irq_ack	= ep93xx_gpio_irq_ack,
	.irq_mask_ack	= ep93xx_gpio_irq_mask_ack,
	.irq_mask	= ep93xx_gpio_irq_mask,
	.irq_unmask	= ep93xx_gpio_irq_unmask,
	.irq_set_type	= ep93xx_gpio_irq_type,
};

static void ep93xx_gpio_init_irq(void)
{
	int gpio_irq;

	for (gpio_irq = gpio_to_irq(0);
	     gpio_irq <= gpio_to_irq(EP93XX_GPIO_LINE_MAX_IRQ); ++gpio_irq) {
		irq_set_chip_and_handler(gpio_irq, &ep93xx_gpio_irq_chip,
					 handle_level_irq);
		set_irq_flags(gpio_irq, IRQF_VALID);
	}

	irq_set_chained_handler(IRQ_EP93XX_GPIO_AB,
				ep93xx_gpio_ab_irq_handler);
	irq_set_chained_handler(IRQ_EP93XX_GPIO0MUX,
				ep93xx_gpio_f_irq_handler);
	irq_set_chained_handler(IRQ_EP93XX_GPIO1MUX,
				ep93xx_gpio_f_irq_handler);
	irq_set_chained_handler(IRQ_EP93XX_GPIO2MUX,
				ep93xx_gpio_f_irq_handler);
	irq_set_chained_handler(IRQ_EP93XX_GPIO3MUX,
				ep93xx_gpio_f_irq_handler);
	irq_set_chained_handler(IRQ_EP93XX_GPIO4MUX,
				ep93xx_gpio_f_irq_handler);
	irq_set_chained_handler(IRQ_EP93XX_GPIO5MUX,
				ep93xx_gpio_f_irq_handler);
	irq_set_chained_handler(IRQ_EP93XX_GPIO6MUX,
				ep93xx_gpio_f_irq_handler);
	irq_set_chained_handler(IRQ_EP93XX_GPIO7MUX,
				ep93xx_gpio_f_irq_handler);
}


=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*************************************************************************
 * gpiolib interface for EP93xx on-chip GPIOs
 *************************************************************************/
struct ep93xx_gpio_bank {
	const char	*label;
	int		data;
	int		dir;
<<<<<<< HEAD
	int		base;
	bool		has_debounce;
};

#define EP93XX_GPIO_BANK(_label, _data, _dir, _base, _debounce)	\
=======
	int		irq;
	int		base;
	bool		has_irq;
	bool		has_hierarchical_irq;
	unsigned int	irq_base;
};

#define EP93XX_GPIO_BANK(_label, _data, _dir, _irq, _base, _has_irq, _has_hier, _irq_base) \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{							\
		.label		= _label,			\
		.data		= _data,			\
		.dir		= _dir,				\
<<<<<<< HEAD
		.base		= _base,			\
		.has_debounce	= _debounce,			\
	}

static struct ep93xx_gpio_bank ep93xx_gpio_banks[] = {
	EP93XX_GPIO_BANK("A", 0x00, 0x10, 0, true),
	EP93XX_GPIO_BANK("B", 0x04, 0x14, 8, true),
	EP93XX_GPIO_BANK("C", 0x08, 0x18, 40, false),
	EP93XX_GPIO_BANK("D", 0x0c, 0x1c, 24, false),
	EP93XX_GPIO_BANK("E", 0x20, 0x24, 32, false),
	EP93XX_GPIO_BANK("F", 0x30, 0x34, 16, true),
	EP93XX_GPIO_BANK("G", 0x38, 0x3c, 48, false),
	EP93XX_GPIO_BANK("H", 0x40, 0x44, 56, false),
};

static int ep93xx_gpio_set_debounce(struct gpio_chip *chip,
				    unsigned offset, unsigned debounce)
{
	int gpio = chip->base + offset;
	int irq = gpio_to_irq(gpio);

	if (irq < 0)
		return -EINVAL;

	ep93xx_gpio_int_debounce(irq, debounce ? true : false);
=======
		.irq		= _irq,				\
		.base		= _base,			\
		.has_irq	= _has_irq,			\
		.has_hierarchical_irq = _has_hier,		\
		.irq_base	= _irq_base,			\
	}

static struct ep93xx_gpio_bank ep93xx_gpio_banks[] = {
	/* Bank A has 8 IRQs */
	EP93XX_GPIO_BANK("A", 0x00, 0x10, 0x90, 0, true, false, EP93XX_GPIO_A_IRQ_BASE),
	/* Bank B has 8 IRQs */
	EP93XX_GPIO_BANK("B", 0x04, 0x14, 0xac, 8, true, false, EP93XX_GPIO_B_IRQ_BASE),
	EP93XX_GPIO_BANK("C", 0x08, 0x18, 0x00, 40, false, false, 0),
	EP93XX_GPIO_BANK("D", 0x0c, 0x1c, 0x00, 24, false, false, 0),
	EP93XX_GPIO_BANK("E", 0x20, 0x24, 0x00, 32, false, false, 0),
	/* Bank F has 8 IRQs */
	EP93XX_GPIO_BANK("F", 0x30, 0x34, 0x4c, 16, false, true, EP93XX_GPIO_F_IRQ_BASE),
	EP93XX_GPIO_BANK("G", 0x38, 0x3c, 0x00, 48, false, false, 0),
	EP93XX_GPIO_BANK("H", 0x40, 0x44, 0x00, 56, false, false, 0),
};

static int ep93xx_gpio_set_config(struct gpio_chip *gc, unsigned offset,
				  unsigned long config)
{
	u32 debounce;

	if (pinconf_to_config_param(config) != PIN_CONFIG_INPUT_DEBOUNCE)
		return -ENOTSUPP;

	debounce = pinconf_to_config_argument(config);
	ep93xx_gpio_int_debounce(gc, offset, debounce ? true : false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
/*
 * Map GPIO A0..A7  (0..7)  to irq 64..71,
 *          B0..B7  (7..15) to irq 72..79, and
 *          F0..F7 (16..24) to irq 80..87.
 */
static int ep93xx_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	int gpio = chip->base + offset;

	if (gpio > EP93XX_GPIO_LINE_MAX_IRQ)
		return -EINVAL;

	return 64 + gpio;
}

static int ep93xx_gpio_add_bank(struct bgpio_chip *bgc, struct device *dev,
	void __iomem *mmio_base, struct ep93xx_gpio_bank *bank)
{
	void __iomem *data = mmio_base + bank->data;
	void __iomem *dir =  mmio_base + bank->dir;
	int err;

	err = bgpio_init(bgc, dev, 1, data, NULL, NULL, dir, NULL, false);
	if (err)
		return err;

	bgc->gc.label = bank->label;
	bgc->gc.base = bank->base;

	if (bank->has_debounce) {
		bgc->gc.set_debounce = ep93xx_gpio_set_debounce;
		bgc->gc.to_irq = ep93xx_gpio_to_irq;
	}

	return gpiochip_add(&bgc->gc);
}

static int __devinit ep93xx_gpio_probe(struct platform_device *pdev)
{
	struct ep93xx_gpio *ep93xx_gpio;
	struct resource *res;
	void __iomem *mmio;
	int i;
	int ret;

	ep93xx_gpio = kzalloc(sizeof(*ep93xx_gpio), GFP_KERNEL);
	if (!ep93xx_gpio)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		ret = -ENXIO;
		goto exit_free;
	}

	if (!request_mem_region(res->start, resource_size(res), pdev->name)) {
		ret = -EBUSY;
		goto exit_free;
	}

	mmio = ioremap(res->start, resource_size(res));
	if (!mmio) {
		ret = -ENXIO;
		goto exit_release;
	}
	ep93xx_gpio->mmio_base = mmio;

	for (i = 0; i < ARRAY_SIZE(ep93xx_gpio_banks); i++) {
		struct bgpio_chip *bgc = &ep93xx_gpio->bgc[i];
		struct ep93xx_gpio_bank *bank = &ep93xx_gpio_banks[i];

		if (ep93xx_gpio_add_bank(bgc, &pdev->dev, mmio, bank))
			dev_warn(&pdev->dev, "Unable to add gpio bank %s\n",
				bank->label);
	}

	ep93xx_gpio_init_irq();

	return 0;

exit_release:
	release_mem_region(res->start, resource_size(res));
exit_free:
	kfree(ep93xx_gpio);
	dev_info(&pdev->dev, "%s failed with errno %d\n", __func__, ret);
	return ret;
=======
static void ep93xx_irq_print_chip(struct irq_data *data, struct seq_file *p)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(data);

	seq_printf(p, dev_name(gc->parent));
}

static const struct irq_chip gpio_eic_irq_chip = {
	.name			= "ep93xx-gpio-eic",
	.irq_ack		= ep93xx_gpio_irq_ack,
	.irq_mask		= ep93xx_gpio_irq_mask,
	.irq_unmask		= ep93xx_gpio_irq_unmask,
	.irq_mask_ack	= ep93xx_gpio_irq_mask_ack,
	.irq_set_type	= ep93xx_gpio_irq_type,
	.irq_print_chip	= ep93xx_irq_print_chip,
	.flags			= IRQCHIP_IMMUTABLE,
	GPIOCHIP_IRQ_RESOURCE_HELPERS,
};

static int ep93xx_gpio_add_bank(struct ep93xx_gpio_chip *egc,
				struct platform_device *pdev,
				struct ep93xx_gpio *epg,
				struct ep93xx_gpio_bank *bank)
{
	void __iomem *data = epg->base + bank->data;
	void __iomem *dir = epg->base + bank->dir;
	struct gpio_chip *gc = &egc->gc;
	struct device *dev = &pdev->dev;
	struct gpio_irq_chip *girq;
	int err;

	err = bgpio_init(gc, dev, 1, data, NULL, NULL, dir, NULL, 0);
	if (err)
		return err;

	gc->label = bank->label;
	gc->base = bank->base;

	girq = &gc->irq;
	if (bank->has_irq || bank->has_hierarchical_irq) {
		gc->set_config = ep93xx_gpio_set_config;
		egc->eic = devm_kcalloc(dev, 1,
					sizeof(*egc->eic),
					GFP_KERNEL);
		if (!egc->eic)
			return -ENOMEM;
		egc->eic->irq_offset = bank->irq;
		gpio_irq_chip_set_chip(girq, &gpio_eic_irq_chip);
	}

	if (bank->has_irq) {
		int ab_parent_irq = platform_get_irq(pdev, 0);

		girq->parent_handler = ep93xx_gpio_ab_irq_handler;
		girq->num_parents = 1;
		girq->parents = devm_kcalloc(dev, girq->num_parents,
					     sizeof(*girq->parents),
					     GFP_KERNEL);
		if (!girq->parents)
			return -ENOMEM;
		girq->default_type = IRQ_TYPE_NONE;
		girq->handler = handle_level_irq;
		girq->parents[0] = ab_parent_irq;
		girq->first = bank->irq_base;
	}

	/* Only bank F has especially funky IRQ handling */
	if (bank->has_hierarchical_irq) {
		int gpio_irq;
		int i;

		/*
		 * FIXME: convert this to use hierarchical IRQ support!
		 * this requires fixing the root irqchip to be hierarchical.
		 */
		girq->parent_handler = ep93xx_gpio_f_irq_handler;
		girq->num_parents = 8;
		girq->parents = devm_kcalloc(dev, girq->num_parents,
					     sizeof(*girq->parents),
					     GFP_KERNEL);
		if (!girq->parents)
			return -ENOMEM;
		/* Pick resources 1..8 for these IRQs */
		for (i = 0; i < girq->num_parents; i++) {
			girq->parents[i] = platform_get_irq(pdev, i + 1);
			gpio_irq = bank->irq_base + i;
			irq_set_chip_data(gpio_irq, &epg->gc[5]);
			irq_set_chip_and_handler(gpio_irq,
						 girq->chip,
						 handle_level_irq);
			irq_clear_status_flags(gpio_irq, IRQ_NOREQUEST);
		}
		girq->default_type = IRQ_TYPE_NONE;
		girq->handler = handle_level_irq;
		girq->first = bank->irq_base;
	}

	return devm_gpiochip_add_data(dev, gc, epg);
}

static int ep93xx_gpio_probe(struct platform_device *pdev)
{
	struct ep93xx_gpio *epg;
	int i;

	epg = devm_kzalloc(&pdev->dev, sizeof(*epg), GFP_KERNEL);
	if (!epg)
		return -ENOMEM;

	epg->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(epg->base))
		return PTR_ERR(epg->base);

	for (i = 0; i < ARRAY_SIZE(ep93xx_gpio_banks); i++) {
		struct ep93xx_gpio_chip *gc = &epg->gc[i];
		struct ep93xx_gpio_bank *bank = &ep93xx_gpio_banks[i];

		if (ep93xx_gpio_add_bank(gc, pdev, epg, bank))
			dev_warn(&pdev->dev, "Unable to add gpio bank %s\n",
				 bank->label);
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver ep93xx_gpio_driver = {
	.driver		= {
		.name	= "gpio-ep93xx",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	.probe		= ep93xx_gpio_probe,
};

static int __init ep93xx_gpio_init(void)
{
	return platform_driver_register(&ep93xx_gpio_driver);
}
postcore_initcall(ep93xx_gpio_init);

MODULE_AUTHOR("Ryan Mallon <ryan@bluewatersys.com> "
		"H Hartley Sweeten <hsweeten@visionengravers.com>");
MODULE_DESCRIPTION("EP93XX GPIO driver");
MODULE_LICENSE("GPL");
