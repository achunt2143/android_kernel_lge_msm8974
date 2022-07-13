<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * GPIO interface for Intel Poulsbo SCH
 *
 *  Copyright (c) 2010 CompuLab Ltd
 *  Author: Denis Turischev <denis@compulab.co.il>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License 2 as published
 *  by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/acpi.h>
#include <linux/platform_device.h>
#include <linux/pci_ids.h>

#include <linux/gpio.h>

static DEFINE_SPINLOCK(gpio_lock);

#define CGEN	(0x00)
#define CGIO	(0x04)
#define CGLV	(0x08)

#define RGEN	(0x20)
#define RGIO	(0x24)
#define RGLV	(0x28)

static unsigned short gpio_ba;

static int sch_gpio_core_direction_in(struct gpio_chip *gc, unsigned  gpio_num)
{
	u8 curr_dirs;
	unsigned short offset, bit;

	spin_lock(&gpio_lock);

	offset = CGIO + gpio_num / 8;
	bit = gpio_num % 8;

	curr_dirs = inb(gpio_ba + offset);

	if (!(curr_dirs & (1 << bit)))
		outb(curr_dirs | (1 << bit), gpio_ba + offset);

	spin_unlock(&gpio_lock);
	return 0;
}

static int sch_gpio_core_get(struct gpio_chip *gc, unsigned gpio_num)
{
	int res;
	unsigned short offset, bit;

	offset = CGLV + gpio_num / 8;
	bit = gpio_num % 8;

	res = !!(inb(gpio_ba + offset) & (1 << bit));
	return res;
}

static void sch_gpio_core_set(struct gpio_chip *gc, unsigned gpio_num, int val)
{
	u8 curr_vals;
	unsigned short offset, bit;

	spin_lock(&gpio_lock);

	offset = CGLV + gpio_num / 8;
	bit = gpio_num % 8;

	curr_vals = inb(gpio_ba + offset);

	if (val)
		outb(curr_vals | (1 << bit), gpio_ba + offset);
	else
		outb((curr_vals & ~(1 << bit)), gpio_ba + offset);
	spin_unlock(&gpio_lock);
}

static int sch_gpio_core_direction_out(struct gpio_chip *gc,
					unsigned gpio_num, int val)
{
	u8 curr_dirs;
	unsigned short offset, bit;

	sch_gpio_core_set(gc, gpio_num, val);

	spin_lock(&gpio_lock);

	offset = CGIO + gpio_num / 8;
	bit = gpio_num % 8;

	curr_dirs = inb(gpio_ba + offset);
	if (curr_dirs & (1 << bit))
		outb(curr_dirs & ~(1 << bit), gpio_ba + offset);

	spin_unlock(&gpio_lock);
	return 0;
}

static struct gpio_chip sch_gpio_core = {
	.label			= "sch_gpio_core",
	.owner			= THIS_MODULE,
	.direction_input	= sch_gpio_core_direction_in,
	.get			= sch_gpio_core_get,
	.direction_output	= sch_gpio_core_direction_out,
	.set			= sch_gpio_core_set,
};

static int sch_gpio_resume_direction_in(struct gpio_chip *gc,
					unsigned gpio_num)
{
	u8 curr_dirs;

	spin_lock(&gpio_lock);

	curr_dirs = inb(gpio_ba + RGIO);

	if (!(curr_dirs & (1 << gpio_num)))
		outb(curr_dirs | (1 << gpio_num) , gpio_ba + RGIO);

	spin_unlock(&gpio_lock);
	return 0;
}

static int sch_gpio_resume_get(struct gpio_chip *gc, unsigned gpio_num)
{
	return !!(inb(gpio_ba + RGLV) & (1 << gpio_num));
}

static void sch_gpio_resume_set(struct gpio_chip *gc,
				unsigned gpio_num, int val)
{
	u8 curr_vals;

	spin_lock(&gpio_lock);

	curr_vals = inb(gpio_ba + RGLV);

	if (val)
		outb(curr_vals | (1 << gpio_num), gpio_ba + RGLV);
	else
		outb((curr_vals & ~(1 << gpio_num)), gpio_ba + RGLV);

	spin_unlock(&gpio_lock);
}

static int sch_gpio_resume_direction_out(struct gpio_chip *gc,
					unsigned gpio_num, int val)
{
	u8 curr_dirs;

	sch_gpio_resume_set(gc, gpio_num, val);

	spin_lock(&gpio_lock);

	curr_dirs = inb(gpio_ba + RGIO);
	if (curr_dirs & (1 << gpio_num))
		outb(curr_dirs & ~(1 << gpio_num), gpio_ba + RGIO);

	spin_unlock(&gpio_lock);
	return 0;
}

static struct gpio_chip sch_gpio_resume = {
	.label			= "sch_gpio_resume",
	.owner			= THIS_MODULE,
	.direction_input	= sch_gpio_resume_direction_in,
	.get			= sch_gpio_resume_get,
	.direction_output	= sch_gpio_resume_direction_out,
	.set			= sch_gpio_resume_set,
};

static int __devinit sch_gpio_probe(struct platform_device *pdev)
{
	struct resource *res;
	int err, id;

	id = pdev->id;
	if (!id)
		return -ENODEV;
=======
 */

#include <linux/acpi.h>
#include <linux/bitops.h>
#include <linux/errno.h>
#include <linux/gpio/driver.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci_ids.h>
#include <linux/platform_device.h>
#include <linux/types.h>

#define GEN	0x00
#define GIO	0x04
#define GLV	0x08
#define GTPE	0x0c
#define GTNE	0x10
#define GGPE	0x14
#define GSMI	0x18
#define GTS	0x1c

#define CORE_BANK_OFFSET	0x00
#define RESUME_BANK_OFFSET	0x20

/*
 * iLB datasheet describes GPE0BLK registers, in particular GPE0E.GPIO bit.
 * Document Number: 328195-001
 */
#define GPE0E_GPIO	14

struct sch_gpio {
	struct gpio_chip chip;
	spinlock_t lock;
	unsigned short iobase;
	unsigned short resume_base;

	/* GPE handling */
	u32 gpe;
	acpi_gpe_handler gpe_handler;
};

static unsigned int sch_gpio_offset(struct sch_gpio *sch, unsigned int gpio,
				unsigned int reg)
{
	unsigned int base = CORE_BANK_OFFSET;

	if (gpio >= sch->resume_base) {
		gpio -= sch->resume_base;
		base = RESUME_BANK_OFFSET;
	}

	return base + reg + gpio / 8;
}

static unsigned int sch_gpio_bit(struct sch_gpio *sch, unsigned int gpio)
{
	if (gpio >= sch->resume_base)
		gpio -= sch->resume_base;
	return gpio % 8;
}

static int sch_gpio_reg_get(struct sch_gpio *sch, unsigned int gpio, unsigned int reg)
{
	unsigned short offset, bit;
	u8 reg_val;

	offset = sch_gpio_offset(sch, gpio, reg);
	bit = sch_gpio_bit(sch, gpio);

	reg_val = !!(inb(sch->iobase + offset) & BIT(bit));

	return reg_val;
}

static void sch_gpio_reg_set(struct sch_gpio *sch, unsigned int gpio, unsigned int reg,
			     int val)
{
	unsigned short offset, bit;
	u8 reg_val;

	offset = sch_gpio_offset(sch, gpio, reg);
	bit = sch_gpio_bit(sch, gpio);

	reg_val = inb(sch->iobase + offset);

	if (val)
		outb(reg_val | BIT(bit), sch->iobase + offset);
	else
		outb((reg_val & ~BIT(bit)), sch->iobase + offset);
}

static int sch_gpio_direction_in(struct gpio_chip *gc, unsigned int gpio_num)
{
	struct sch_gpio *sch = gpiochip_get_data(gc);
	unsigned long flags;

	spin_lock_irqsave(&sch->lock, flags);
	sch_gpio_reg_set(sch, gpio_num, GIO, 1);
	spin_unlock_irqrestore(&sch->lock, flags);
	return 0;
}

static int sch_gpio_get(struct gpio_chip *gc, unsigned int gpio_num)
{
	struct sch_gpio *sch = gpiochip_get_data(gc);

	return sch_gpio_reg_get(sch, gpio_num, GLV);
}

static void sch_gpio_set(struct gpio_chip *gc, unsigned int gpio_num, int val)
{
	struct sch_gpio *sch = gpiochip_get_data(gc);
	unsigned long flags;

	spin_lock_irqsave(&sch->lock, flags);
	sch_gpio_reg_set(sch, gpio_num, GLV, val);
	spin_unlock_irqrestore(&sch->lock, flags);
}

static int sch_gpio_direction_out(struct gpio_chip *gc, unsigned int gpio_num,
				  int val)
{
	struct sch_gpio *sch = gpiochip_get_data(gc);
	unsigned long flags;

	spin_lock_irqsave(&sch->lock, flags);
	sch_gpio_reg_set(sch, gpio_num, GIO, 0);
	spin_unlock_irqrestore(&sch->lock, flags);

	/*
	 * according to the datasheet, writing to the level register has no
	 * effect when GPIO is programmed as input.
	 * Actually the level register is read-only when configured as input.
	 * Thus presetting the output level before switching to output is _NOT_ possible.
	 * Hence we set the level after configuring the GPIO as output.
	 * But we cannot prevent a short low pulse if direction is set to high
	 * and an external pull-up is connected.
	 */
	sch_gpio_set(gc, gpio_num, val);
	return 0;
}

static int sch_gpio_get_direction(struct gpio_chip *gc, unsigned int gpio_num)
{
	struct sch_gpio *sch = gpiochip_get_data(gc);

	if (sch_gpio_reg_get(sch, gpio_num, GIO))
		return GPIO_LINE_DIRECTION_IN;

	return GPIO_LINE_DIRECTION_OUT;
}

static const struct gpio_chip sch_gpio_chip = {
	.label			= "sch_gpio",
	.owner			= THIS_MODULE,
	.direction_input	= sch_gpio_direction_in,
	.get			= sch_gpio_get,
	.direction_output	= sch_gpio_direction_out,
	.set			= sch_gpio_set,
	.get_direction		= sch_gpio_get_direction,
};

static int sch_irq_type(struct irq_data *d, unsigned int type)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct sch_gpio *sch = gpiochip_get_data(gc);
	irq_hw_number_t gpio_num = irqd_to_hwirq(d);
	unsigned long flags;
	int rising, falling;

	switch (type & IRQ_TYPE_SENSE_MASK) {
	case IRQ_TYPE_EDGE_RISING:
		rising = 1;
		falling = 0;
		break;
	case IRQ_TYPE_EDGE_FALLING:
		rising = 0;
		falling = 1;
		break;
	case IRQ_TYPE_EDGE_BOTH:
		rising = 1;
		falling = 1;
		break;
	default:
		return -EINVAL;
	}

	spin_lock_irqsave(&sch->lock, flags);

	sch_gpio_reg_set(sch, gpio_num, GTPE, rising);
	sch_gpio_reg_set(sch, gpio_num, GTNE, falling);

	irq_set_handler_locked(d, handle_edge_irq);

	spin_unlock_irqrestore(&sch->lock, flags);

	return 0;
}

static void sch_irq_ack(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct sch_gpio *sch = gpiochip_get_data(gc);
	irq_hw_number_t gpio_num = irqd_to_hwirq(d);
	unsigned long flags;

	spin_lock_irqsave(&sch->lock, flags);
	sch_gpio_reg_set(sch, gpio_num, GTS, 1);
	spin_unlock_irqrestore(&sch->lock, flags);
}

static void sch_irq_mask_unmask(struct gpio_chip *gc, irq_hw_number_t gpio_num, int val)
{
	struct sch_gpio *sch = gpiochip_get_data(gc);
	unsigned long flags;

	spin_lock_irqsave(&sch->lock, flags);
	sch_gpio_reg_set(sch, gpio_num, GGPE, val);
	spin_unlock_irqrestore(&sch->lock, flags);
}

static void sch_irq_mask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	irq_hw_number_t gpio_num = irqd_to_hwirq(d);

	sch_irq_mask_unmask(gc, gpio_num, 0);
	gpiochip_disable_irq(gc, gpio_num);
}

static void sch_irq_unmask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	irq_hw_number_t gpio_num = irqd_to_hwirq(d);

	gpiochip_enable_irq(gc, gpio_num);
	sch_irq_mask_unmask(gc, gpio_num, 1);
}

static const struct irq_chip sch_irqchip = {
	.name = "sch_gpio",
	.irq_ack = sch_irq_ack,
	.irq_mask = sch_irq_mask,
	.irq_unmask = sch_irq_unmask,
	.irq_set_type = sch_irq_type,
	.flags = IRQCHIP_IMMUTABLE,
	GPIOCHIP_IRQ_RESOURCE_HELPERS,
};

static u32 sch_gpio_gpe_handler(acpi_handle gpe_device, u32 gpe, void *context)
{
	struct sch_gpio *sch = context;
	struct gpio_chip *gc = &sch->chip;
	unsigned long core_status, resume_status;
	unsigned long pending;
	unsigned long flags;
	int offset;
	u32 ret;

	spin_lock_irqsave(&sch->lock, flags);

	core_status = inl(sch->iobase + CORE_BANK_OFFSET + GTS);
	resume_status = inl(sch->iobase + RESUME_BANK_OFFSET + GTS);

	spin_unlock_irqrestore(&sch->lock, flags);

	pending = (resume_status << sch->resume_base) | core_status;
	for_each_set_bit(offset, &pending, sch->chip.ngpio)
		generic_handle_domain_irq(gc->irq.domain, offset);

	/* Set returning value depending on whether we handled an interrupt */
	ret = pending ? ACPI_INTERRUPT_HANDLED : ACPI_INTERRUPT_NOT_HANDLED;

	/* Acknowledge GPE to ACPICA */
	ret |= ACPI_REENABLE_GPE;

	return ret;
}

static void sch_gpio_remove_gpe_handler(void *data)
{
	struct sch_gpio *sch = data;

	acpi_disable_gpe(NULL, sch->gpe);
	acpi_remove_gpe_handler(NULL, sch->gpe, sch->gpe_handler);
}

static int sch_gpio_install_gpe_handler(struct sch_gpio *sch)
{
	struct device *dev = sch->chip.parent;
	acpi_status status;

	status = acpi_install_gpe_handler(NULL, sch->gpe, ACPI_GPE_LEVEL_TRIGGERED,
					  sch->gpe_handler, sch);
	if (ACPI_FAILURE(status)) {
		dev_err(dev, "Failed to install GPE handler for %u: %s\n",
			sch->gpe, acpi_format_exception(status));
		return -ENODEV;
	}

	status = acpi_enable_gpe(NULL, sch->gpe);
	if (ACPI_FAILURE(status)) {
		dev_err(dev, "Failed to enable GPE handler for %u: %s\n",
			sch->gpe, acpi_format_exception(status));
		acpi_remove_gpe_handler(NULL, sch->gpe, sch->gpe_handler);
		return -ENODEV;
	}

	return devm_add_action_or_reset(dev, sch_gpio_remove_gpe_handler, sch);
}

static int sch_gpio_probe(struct platform_device *pdev)
{
	struct gpio_irq_chip *girq;
	struct sch_gpio *sch;
	struct resource *res;
	int ret;

	sch = devm_kzalloc(&pdev->dev, sizeof(*sch), GFP_KERNEL);
	if (!sch)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
	if (!res)
		return -EBUSY;

<<<<<<< HEAD
	if (!request_region(res->start, resource_size(res), pdev->name))
		return -EBUSY;

	gpio_ba = res->start;

	switch (id) {
		case PCI_DEVICE_ID_INTEL_SCH_LPC:
			sch_gpio_core.base = 0;
			sch_gpio_core.ngpio = 10;

			sch_gpio_resume.base = 10;
			sch_gpio_resume.ngpio = 4;

			/*
			 * GPIO[6:0] enabled by default
			 * GPIO7 is configured by the CMC as SLPIOVR
			 * Enable GPIO[9:8] core powered gpios explicitly
			 */
			outb(0x3, gpio_ba + CGEN + 1);
			/*
			 * SUS_GPIO[2:0] enabled by default
			 * Enable SUS_GPIO3 resume powered gpio explicitly
			 */
			outb(0x8, gpio_ba + RGEN);
			break;

		case PCI_DEVICE_ID_INTEL_ITC_LPC:
			sch_gpio_core.base = 0;
			sch_gpio_core.ngpio = 5;

			sch_gpio_resume.base = 5;
			sch_gpio_resume.ngpio = 9;
			break;

		default:
			return -ENODEV;
	}

	sch_gpio_core.dev = &pdev->dev;
	sch_gpio_resume.dev = &pdev->dev;

	err = gpiochip_add(&sch_gpio_core);
	if (err < 0)
		goto err_sch_gpio_core;

	err = gpiochip_add(&sch_gpio_resume);
	if (err < 0)
		goto err_sch_gpio_resume;

	return 0;

err_sch_gpio_resume:
	err = gpiochip_remove(&sch_gpio_core);
	if (err)
		dev_err(&pdev->dev, "%s failed, %d\n",
				"gpiochip_remove()", err);

err_sch_gpio_core:
	release_region(res->start, resource_size(res));
	gpio_ba = 0;

	return err;
}

static int __devexit sch_gpio_remove(struct platform_device *pdev)
{
	struct resource *res;
	if (gpio_ba) {
		int err;

		err  = gpiochip_remove(&sch_gpio_core);
		if (err)
			dev_err(&pdev->dev, "%s failed, %d\n",
				"gpiochip_remove()", err);
		err = gpiochip_remove(&sch_gpio_resume);
		if (err)
			dev_err(&pdev->dev, "%s failed, %d\n",
				"gpiochip_remove()", err);

		res = platform_get_resource(pdev, IORESOURCE_IO, 0);

		release_region(res->start, resource_size(res));
		gpio_ba = 0;

		return err;
	}

	return 0;
=======
	if (!devm_request_region(&pdev->dev, res->start, resource_size(res),
				 pdev->name))
		return -EBUSY;

	spin_lock_init(&sch->lock);
	sch->iobase = res->start;
	sch->chip = sch_gpio_chip;
	sch->chip.label = dev_name(&pdev->dev);
	sch->chip.parent = &pdev->dev;

	switch (pdev->id) {
	case PCI_DEVICE_ID_INTEL_SCH_LPC:
		sch->resume_base = 10;
		sch->chip.ngpio = 14;

		/*
		 * GPIO[6:0] enabled by default
		 * GPIO7 is configured by the CMC as SLPIOVR
		 * Enable GPIO[9:8] core powered gpios explicitly
		 */
		sch_gpio_reg_set(sch, 8, GEN, 1);
		sch_gpio_reg_set(sch, 9, GEN, 1);
		/*
		 * SUS_GPIO[2:0] enabled by default
		 * Enable SUS_GPIO3 resume powered gpio explicitly
		 */
		sch_gpio_reg_set(sch, 13, GEN, 1);
		break;

	case PCI_DEVICE_ID_INTEL_ITC_LPC:
		sch->resume_base = 5;
		sch->chip.ngpio = 14;
		break;

	case PCI_DEVICE_ID_INTEL_CENTERTON_ILB:
		sch->resume_base = 21;
		sch->chip.ngpio = 30;
		break;

	case PCI_DEVICE_ID_INTEL_QUARK_X1000_ILB:
		sch->resume_base = 2;
		sch->chip.ngpio = 8;
		break;

	default:
		return -ENODEV;
	}

	girq = &sch->chip.irq;
	gpio_irq_chip_set_chip(girq, &sch_irqchip);
	girq->num_parents = 0;
	girq->parents = NULL;
	girq->parent_handler = NULL;
	girq->default_type = IRQ_TYPE_NONE;
	girq->handler = handle_bad_irq;

	/* GPE setup is optional */
	sch->gpe = GPE0E_GPIO;
	sch->gpe_handler = sch_gpio_gpe_handler;

	ret = sch_gpio_install_gpe_handler(sch);
	if (ret)
		dev_warn(&pdev->dev, "Can't setup GPE, no IRQ support\n");

	return devm_gpiochip_add_data(&pdev->dev, &sch->chip, sch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver sch_gpio_driver = {
	.driver = {
		.name = "sch_gpio",
<<<<<<< HEAD
		.owner = THIS_MODULE,
	},
	.probe		= sch_gpio_probe,
	.remove		= __devexit_p(sch_gpio_remove),
=======
	},
	.probe		= sch_gpio_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(sch_gpio_driver);

MODULE_AUTHOR("Denis Turischev <denis@compulab.co.il>");
MODULE_DESCRIPTION("GPIO interface for Intel Poulsbo SCH");
<<<<<<< HEAD
MODULE_LICENSE("GPL");
=======
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS("platform:sch_gpio");
