<<<<<<< HEAD
/*
 * Copyright (C) ST-Ericsson SA 2010
 *
 * License Terms: GNU General Public License, version 2
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) ST-Ericsson SA 2010
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author: Hanumath Prasad <hanumath.prasad@stericsson.com> for ST-Ericsson
 * Author: Rabin Vincent <rabin.vincent@stericsson.com> for ST-Ericsson
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/mfd/tc3589x.h>
=======
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio/driver.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/mfd/tc3589x.h>
#include <linux/bitops.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * These registers are modified under the irq bus lock and cached to avoid
 * unnecessary writes in bus_sync_unlock.
 */
<<<<<<< HEAD
enum { REG_IBE, REG_IEV, REG_IS, REG_IE };

#define CACHE_NR_REGS	4
=======
enum { REG_IBE, REG_IEV, REG_IS, REG_IE, REG_DIRECT };

#define CACHE_NR_REGS	5
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define CACHE_NR_BANKS	3

struct tc3589x_gpio {
	struct gpio_chip chip;
	struct tc3589x *tc3589x;
	struct device *dev;
	struct mutex irq_lock;
<<<<<<< HEAD

	int irq_base;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Caches of interrupt control registers for bus_lock */
	u8 regs[CACHE_NR_REGS][CACHE_NR_BANKS];
	u8 oldregs[CACHE_NR_REGS][CACHE_NR_BANKS];
};

<<<<<<< HEAD
static inline struct tc3589x_gpio *to_tc3589x_gpio(struct gpio_chip *chip)
{
	return container_of(chip, struct tc3589x_gpio, chip);
}

static int tc3589x_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct tc3589x_gpio *tc3589x_gpio = to_tc3589x_gpio(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 reg = TC3589x_GPIODATA0 + (offset / 8) * 2;
	u8 mask = 1 << (offset % 8);
=======
static int tc3589x_gpio_get(struct gpio_chip *chip, unsigned int offset)
{
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 reg = TC3589x_GPIODATA0 + (offset / 8) * 2;
	u8 mask = BIT(offset % 8);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	ret = tc3589x_reg_read(tc3589x, reg);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	return ret & mask;
}

static void tc3589x_gpio_set(struct gpio_chip *chip, unsigned offset, int val)
{
	struct tc3589x_gpio *tc3589x_gpio = to_tc3589x_gpio(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 reg = TC3589x_GPIODATA0 + (offset / 8) * 2;
	unsigned pos = offset % 8;
	u8 data[] = {!!val << pos, 1 << pos};
=======
	return !!(ret & mask);
}

static void tc3589x_gpio_set(struct gpio_chip *chip, unsigned int offset, int val)
{
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 reg = TC3589x_GPIODATA0 + (offset / 8) * 2;
	unsigned int pos = offset % 8;
	u8 data[] = {val ? BIT(pos) : 0, BIT(pos)};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	tc3589x_block_write(tc3589x, reg, ARRAY_SIZE(data), data);
}

static int tc3589x_gpio_direction_output(struct gpio_chip *chip,
<<<<<<< HEAD
					 unsigned offset, int val)
{
	struct tc3589x_gpio *tc3589x_gpio = to_tc3589x_gpio(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 reg = TC3589x_GPIODIR0 + offset / 8;
	unsigned pos = offset % 8;

	tc3589x_gpio_set(chip, offset, val);

	return tc3589x_set_bits(tc3589x, reg, 1 << pos, 1 << pos);
}

static int tc3589x_gpio_direction_input(struct gpio_chip *chip,
					unsigned offset)
{
	struct tc3589x_gpio *tc3589x_gpio = to_tc3589x_gpio(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 reg = TC3589x_GPIODIR0 + offset / 8;
	unsigned pos = offset % 8;

	return tc3589x_set_bits(tc3589x, reg, 1 << pos, 0);
}

static int tc3589x_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct tc3589x_gpio *tc3589x_gpio = to_tc3589x_gpio(chip);

	return tc3589x_gpio->irq_base + offset;
}

static struct gpio_chip template_chip = {
	.label			= "tc3589x",
	.owner			= THIS_MODULE,
	.direction_input	= tc3589x_gpio_direction_input,
	.get			= tc3589x_gpio_get,
	.direction_output	= tc3589x_gpio_direction_output,
	.set			= tc3589x_gpio_set,
	.to_irq			= tc3589x_gpio_to_irq,
	.can_sleep		= 1,
=======
					 unsigned int offset, int val)
{
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 reg = TC3589x_GPIODIR0 + offset / 8;
	unsigned int pos = offset % 8;

	tc3589x_gpio_set(chip, offset, val);

	return tc3589x_set_bits(tc3589x, reg, BIT(pos), BIT(pos));
}

static int tc3589x_gpio_direction_input(struct gpio_chip *chip,
					unsigned int offset)
{
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 reg = TC3589x_GPIODIR0 + offset / 8;
	unsigned int pos = offset % 8;

	return tc3589x_set_bits(tc3589x, reg, BIT(pos), 0);
}

static int tc3589x_gpio_get_direction(struct gpio_chip *chip,
				      unsigned int offset)
{
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 reg = TC3589x_GPIODIR0 + offset / 8;
	unsigned int pos = offset % 8;
	int ret;

	ret = tc3589x_reg_read(tc3589x, reg);
	if (ret < 0)
		return ret;

	if (ret & BIT(pos))
		return GPIO_LINE_DIRECTION_OUT;

	return GPIO_LINE_DIRECTION_IN;
}

static int tc3589x_gpio_set_config(struct gpio_chip *chip, unsigned int offset,
				   unsigned long config)
{
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(chip);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	/*
	 * These registers are alterated at each second address
	 * ODM bit 0 = drive to GND or Hi-Z (open drain)
	 * ODM bit 1 = drive to VDD or Hi-Z (open source)
	 */
	u8 odmreg = TC3589x_GPIOODM0 + (offset / 8) * 2;
	u8 odereg = TC3589x_GPIOODE0 + (offset / 8) * 2;
	unsigned int pos = offset % 8;
	int ret;

	switch (pinconf_to_config_param(config)) {
	case PIN_CONFIG_DRIVE_OPEN_DRAIN:
		/* Set open drain mode */
		ret = tc3589x_set_bits(tc3589x, odmreg, BIT(pos), 0);
		if (ret)
			return ret;
		/* Enable open drain/source mode */
		return tc3589x_set_bits(tc3589x, odereg, BIT(pos), BIT(pos));
	case PIN_CONFIG_DRIVE_OPEN_SOURCE:
		/* Set open source mode */
		ret = tc3589x_set_bits(tc3589x, odmreg, BIT(pos), BIT(pos));
		if (ret)
			return ret;
		/* Enable open drain/source mode */
		return tc3589x_set_bits(tc3589x, odereg, BIT(pos), BIT(pos));
	case PIN_CONFIG_DRIVE_PUSH_PULL:
		/* Disable open drain/source mode */
		return tc3589x_set_bits(tc3589x, odereg, BIT(pos), 0);
	default:
		break;
	}
	return -ENOTSUPP;
}

static const struct gpio_chip template_chip = {
	.label			= "tc3589x",
	.owner			= THIS_MODULE,
	.get			= tc3589x_gpio_get,
	.set			= tc3589x_gpio_set,
	.direction_output	= tc3589x_gpio_direction_output,
	.direction_input	= tc3589x_gpio_direction_input,
	.get_direction		= tc3589x_gpio_get_direction,
	.set_config		= tc3589x_gpio_set_config,
	.can_sleep		= true,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int tc3589x_gpio_irq_set_type(struct irq_data *d, unsigned int type)
{
<<<<<<< HEAD
	struct tc3589x_gpio *tc3589x_gpio = irq_data_get_irq_chip_data(d);
	int offset = d->irq - tc3589x_gpio->irq_base;
	int regoffset = offset / 8;
	int mask = 1 << (offset % 8);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(gc);
	int offset = d->hwirq;
	int regoffset = offset / 8;
	int mask = BIT(offset % 8);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (type == IRQ_TYPE_EDGE_BOTH) {
		tc3589x_gpio->regs[REG_IBE][regoffset] |= mask;
		return 0;
	}

	tc3589x_gpio->regs[REG_IBE][regoffset] &= ~mask;

	if (type == IRQ_TYPE_LEVEL_LOW || type == IRQ_TYPE_LEVEL_HIGH)
		tc3589x_gpio->regs[REG_IS][regoffset] |= mask;
	else
		tc3589x_gpio->regs[REG_IS][regoffset] &= ~mask;

	if (type == IRQ_TYPE_EDGE_RISING || type == IRQ_TYPE_LEVEL_HIGH)
		tc3589x_gpio->regs[REG_IEV][regoffset] |= mask;
	else
		tc3589x_gpio->regs[REG_IEV][regoffset] &= ~mask;

	return 0;
}

static void tc3589x_gpio_irq_lock(struct irq_data *d)
{
<<<<<<< HEAD
	struct tc3589x_gpio *tc3589x_gpio = irq_data_get_irq_chip_data(d);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(gc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&tc3589x_gpio->irq_lock);
}

static void tc3589x_gpio_irq_sync_unlock(struct irq_data *d)
{
<<<<<<< HEAD
	struct tc3589x_gpio *tc3589x_gpio = irq_data_get_irq_chip_data(d);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(gc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	static const u8 regmap[] = {
		[REG_IBE]	= TC3589x_GPIOIBE0,
		[REG_IEV]	= TC3589x_GPIOIEV0,
		[REG_IS]	= TC3589x_GPIOIS0,
		[REG_IE]	= TC3589x_GPIOIE0,
<<<<<<< HEAD
=======
		[REG_DIRECT]	= TC3589x_DIRECT0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	};
	int i, j;

	for (i = 0; i < CACHE_NR_REGS; i++) {
		for (j = 0; j < CACHE_NR_BANKS; j++) {
			u8 old = tc3589x_gpio->oldregs[i][j];
			u8 new = tc3589x_gpio->regs[i][j];

			if (new == old)
				continue;

			tc3589x_gpio->oldregs[i][j] = new;
<<<<<<< HEAD
			tc3589x_reg_write(tc3589x, regmap[i] + j * 8, new);
=======
			tc3589x_reg_write(tc3589x, regmap[i] + j, new);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	mutex_unlock(&tc3589x_gpio->irq_lock);
}

static void tc3589x_gpio_irq_mask(struct irq_data *d)
{
<<<<<<< HEAD
	struct tc3589x_gpio *tc3589x_gpio = irq_data_get_irq_chip_data(d);
	int offset = d->irq - tc3589x_gpio->irq_base;
	int regoffset = offset / 8;
	int mask = 1 << (offset % 8);

	tc3589x_gpio->regs[REG_IE][regoffset] &= ~mask;
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(gc);
	int offset = d->hwirq;
	int regoffset = offset / 8;
	int mask = BIT(offset % 8);

	tc3589x_gpio->regs[REG_IE][regoffset] &= ~mask;
	tc3589x_gpio->regs[REG_DIRECT][regoffset] |= mask;
	gpiochip_disable_irq(gc, offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void tc3589x_gpio_irq_unmask(struct irq_data *d)
{
<<<<<<< HEAD
	struct tc3589x_gpio *tc3589x_gpio = irq_data_get_irq_chip_data(d);
	int offset = d->irq - tc3589x_gpio->irq_base;
	int regoffset = offset / 8;
	int mask = 1 << (offset % 8);

	tc3589x_gpio->regs[REG_IE][regoffset] |= mask;
}

static struct irq_chip tc3589x_gpio_irq_chip = {
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct tc3589x_gpio *tc3589x_gpio = gpiochip_get_data(gc);
	int offset = d->hwirq;
	int regoffset = offset / 8;
	int mask = BIT(offset % 8);

	gpiochip_enable_irq(gc, offset);
	tc3589x_gpio->regs[REG_IE][regoffset] |= mask;
	tc3589x_gpio->regs[REG_DIRECT][regoffset] &= ~mask;
}

static const struct irq_chip tc3589x_gpio_irq_chip = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name			= "tc3589x-gpio",
	.irq_bus_lock		= tc3589x_gpio_irq_lock,
	.irq_bus_sync_unlock	= tc3589x_gpio_irq_sync_unlock,
	.irq_mask		= tc3589x_gpio_irq_mask,
	.irq_unmask		= tc3589x_gpio_irq_unmask,
	.irq_set_type		= tc3589x_gpio_irq_set_type,
<<<<<<< HEAD
=======
	.flags =		IRQCHIP_IMMUTABLE,
	GPIOCHIP_IRQ_RESOURCE_HELPERS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static irqreturn_t tc3589x_gpio_irq(int irq, void *dev)
{
	struct tc3589x_gpio *tc3589x_gpio = dev;
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	u8 status[CACHE_NR_BANKS];
	int ret;
	int i;

	ret = tc3589x_block_read(tc3589x, TC3589x_GPIOMIS0,
				 ARRAY_SIZE(status), status);
	if (ret < 0)
		return IRQ_NONE;

	for (i = 0; i < ARRAY_SIZE(status); i++) {
		unsigned int stat = status[i];
		if (!stat)
			continue;

		while (stat) {
			int bit = __ffs(stat);
			int line = i * 8 + bit;
<<<<<<< HEAD

			handle_nested_irq(tc3589x_gpio->irq_base + line);
=======
			int irq = irq_find_mapping(tc3589x_gpio->chip.irq.domain,
						   line);

			handle_nested_irq(irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			stat &= ~(1 << bit);
		}

		tc3589x_reg_write(tc3589x, TC3589x_GPIOIC0 + i, status[i]);
	}

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int tc3589x_gpio_irq_init(struct tc3589x_gpio *tc3589x_gpio)
{
	int base = tc3589x_gpio->irq_base;
	int irq;

	for (irq = base; irq < base + tc3589x_gpio->chip.ngpio; irq++) {
		irq_set_chip_data(irq, tc3589x_gpio);
		irq_set_chip_and_handler(irq, &tc3589x_gpio_irq_chip,
					 handle_simple_irq);
		irq_set_nested_thread(irq, 1);
#ifdef CONFIG_ARM
		set_irq_flags(irq, IRQF_VALID);
#else
		irq_set_noprobe(irq);
#endif
	}

	return 0;
}

static void tc3589x_gpio_irq_remove(struct tc3589x_gpio *tc3589x_gpio)
{
	int base = tc3589x_gpio->irq_base;
	int irq;

	for (irq = base; irq < base + tc3589x_gpio->chip.ngpio; irq++) {
#ifdef CONFIG_ARM
		set_irq_flags(irq, 0);
#endif
		irq_set_chip_and_handler(irq, NULL, NULL);
		irq_set_chip_data(irq, NULL);
	}
}

static int __devinit tc3589x_gpio_probe(struct platform_device *pdev)
{
	struct tc3589x *tc3589x = dev_get_drvdata(pdev->dev.parent);
	struct tc3589x_gpio_platform_data *pdata;
	struct tc3589x_gpio *tc3589x_gpio;
	int ret;
	int irq;

	pdata = tc3589x->pdata->gpio;
	if (!pdata)
		return -ENODEV;
=======
static int tc3589x_gpio_probe(struct platform_device *pdev)
{
	struct tc3589x *tc3589x = dev_get_drvdata(pdev->dev.parent);
	struct device_node *np = pdev->dev.of_node;
	struct tc3589x_gpio *tc3589x_gpio;
	struct gpio_irq_chip *girq;
	int ret;
	int irq;

	if (!np) {
		dev_err(&pdev->dev, "No Device Tree node found\n");
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

<<<<<<< HEAD
	tc3589x_gpio = kzalloc(sizeof(struct tc3589x_gpio), GFP_KERNEL);
=======
	tc3589x_gpio = devm_kzalloc(&pdev->dev, sizeof(struct tc3589x_gpio),
				    GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!tc3589x_gpio)
		return -ENOMEM;

	mutex_init(&tc3589x_gpio->irq_lock);

	tc3589x_gpio->dev = &pdev->dev;
	tc3589x_gpio->tc3589x = tc3589x;

	tc3589x_gpio->chip = template_chip;
	tc3589x_gpio->chip.ngpio = tc3589x->num_gpio;
<<<<<<< HEAD
	tc3589x_gpio->chip.dev = &pdev->dev;
	tc3589x_gpio->chip.base = pdata->gpio_base;

	tc3589x_gpio->irq_base = tc3589x->irq_base + TC3589x_INT_GPIO(0);
=======
	tc3589x_gpio->chip.parent = &pdev->dev;
	tc3589x_gpio->chip.base = -1;

	girq = &tc3589x_gpio->chip.irq;
	gpio_irq_chip_set_chip(girq, &tc3589x_gpio_irq_chip);
	/* This will let us handle the parent IRQ in the driver */
	girq->parent_handler = NULL;
	girq->num_parents = 0;
	girq->parents = NULL;
	girq->default_type = IRQ_TYPE_NONE;
	girq->handler = handle_simple_irq;
	girq->threaded = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Bring the GPIO module out of reset */
	ret = tc3589x_set_bits(tc3589x, TC3589x_RSTCTRL,
			       TC3589x_RSTCTRL_GPIRST, 0);
	if (ret < 0)
<<<<<<< HEAD
		goto out_free;

	ret = tc3589x_gpio_irq_init(tc3589x_gpio);
	if (ret)
		goto out_free;

	ret = request_threaded_irq(irq, NULL, tc3589x_gpio_irq, IRQF_ONESHOT,
				   "tc3589x-gpio", tc3589x_gpio);
	if (ret) {
		dev_err(&pdev->dev, "unable to get irq: %d\n", ret);
		goto out_removeirq;
	}

	ret = gpiochip_add(&tc3589x_gpio->chip);
	if (ret) {
		dev_err(&pdev->dev, "unable to add gpiochip: %d\n", ret);
		goto out_freeirq;
	}

	if (pdata->setup)
		pdata->setup(tc3589x, tc3589x_gpio->chip.base);

	platform_set_drvdata(pdev, tc3589x_gpio);

	return 0;

out_freeirq:
	free_irq(irq, tc3589x_gpio);
out_removeirq:
	tc3589x_gpio_irq_remove(tc3589x_gpio);
out_free:
	kfree(tc3589x_gpio);
	return ret;
}

static int __devexit tc3589x_gpio_remove(struct platform_device *pdev)
{
	struct tc3589x_gpio *tc3589x_gpio = platform_get_drvdata(pdev);
	struct tc3589x *tc3589x = tc3589x_gpio->tc3589x;
	struct tc3589x_gpio_platform_data *pdata = tc3589x->pdata->gpio;
	int irq = platform_get_irq(pdev, 0);
	int ret;

	if (pdata->remove)
		pdata->remove(tc3589x, tc3589x_gpio->chip.base);

	ret = gpiochip_remove(&tc3589x_gpio->chip);
	if (ret < 0) {
		dev_err(tc3589x_gpio->dev,
			"unable to remove gpiochip: %d\n", ret);
		return ret;
	}

	free_irq(irq, tc3589x_gpio);
	tc3589x_gpio_irq_remove(tc3589x_gpio);

	platform_set_drvdata(pdev, NULL);
	kfree(tc3589x_gpio);

	return 0;
=======
		return ret;

	 /* For tc35894, have to disable Direct KBD interrupts,
	  * else IRQST will always be 0x20, IRQN low level, can't
	  * clear the irq status.
	  * TODO: need more test on other tc3589x chip.
	  *
	  */
	ret = tc3589x_reg_write(tc3589x, TC3589x_DKBDMSK,
			TC3589x_DKBDMSK_ELINT | TC3589x_DKBDMSK_EINT);
	if (ret < 0)
		return ret;

	ret = devm_request_threaded_irq(&pdev->dev,
					irq, NULL, tc3589x_gpio_irq,
					IRQF_ONESHOT, "tc3589x-gpio",
					tc3589x_gpio);
	if (ret) {
		dev_err(&pdev->dev, "unable to get irq: %d\n", ret);
		return ret;
	}

	return devm_gpiochip_add_data(&pdev->dev, &tc3589x_gpio->chip, tc3589x_gpio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver tc3589x_gpio_driver = {
	.driver.name	= "tc3589x-gpio",
<<<<<<< HEAD
	.driver.owner	= THIS_MODULE,
	.probe		= tc3589x_gpio_probe,
	.remove		= __devexit_p(tc3589x_gpio_remove),
=======
	.probe		= tc3589x_gpio_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init tc3589x_gpio_init(void)
{
	return platform_driver_register(&tc3589x_gpio_driver);
}
subsys_initcall(tc3589x_gpio_init);
<<<<<<< HEAD

static void __exit tc3589x_gpio_exit(void)
{
	platform_driver_unregister(&tc3589x_gpio_driver);
}
module_exit(tc3589x_gpio_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("TC3589x GPIO driver");
MODULE_AUTHOR("Hanumath Prasad, Rabin Vincent");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
