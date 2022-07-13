<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  74Hx164 - Generic serial-in/parallel-out 8-bits shift register GPIO driver
 *
 *  Copyright (C) 2010 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2010 Miguel Gaio <miguel.gaio@efixo.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/spi/spi.h>
#include <linux/spi/74x164.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/module.h>

struct gen_74x164_chip {
	struct spi_device	*spi;
	struct gpio_chip	gpio_chip;
	struct mutex		lock;
	u8			port_config;
};

static struct gen_74x164_chip *gpio_to_74x164_chip(struct gpio_chip *gc)
{
	return container_of(gc, struct gen_74x164_chip, gpio_chip);
}

static int __gen_74x164_write_config(struct gen_74x164_chip *chip)
{
	return spi_write(chip->spi,
			 &chip->port_config, sizeof(chip->port_config));
=======
 */

#include <linux/bitops.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio/driver.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/property.h>
#include <linux/slab.h>
#include <linux/spi/spi.h>

#define GEN_74X164_NUMBER_GPIOS	8

struct gen_74x164_chip {
	struct gpio_chip	gpio_chip;
	struct mutex		lock;
	struct gpio_desc	*gpiod_oe;
	u32			registers;
	/*
	 * Since the registers are chained, every byte sent will make
	 * the previous byte shift to the next register in the
	 * chain. Thus, the first byte sent will end up in the last
	 * register at the end of the transfer. So, to have a logical
	 * numbering, store the bytes in reverse order.
	 */
	u8			buffer[];
};

static int __gen_74x164_write_config(struct gen_74x164_chip *chip)
{
	return spi_write(to_spi_device(chip->gpio_chip.parent), chip->buffer,
			 chip->registers);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int gen_74x164_get_value(struct gpio_chip *gc, unsigned offset)
{
<<<<<<< HEAD
	struct gen_74x164_chip *chip = gpio_to_74x164_chip(gc);
	int ret;

	mutex_lock(&chip->lock);
	ret = (chip->port_config >> offset) & 0x1;
=======
	struct gen_74x164_chip *chip = gpiochip_get_data(gc);
	u8 bank = chip->registers - 1 - offset / 8;
	u8 pin = offset % 8;
	int ret;

	mutex_lock(&chip->lock);
	ret = (chip->buffer[bank] >> pin) & 0x1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&chip->lock);

	return ret;
}

static void gen_74x164_set_value(struct gpio_chip *gc,
		unsigned offset, int val)
{
<<<<<<< HEAD
	struct gen_74x164_chip *chip = gpio_to_74x164_chip(gc);

	mutex_lock(&chip->lock);
	if (val)
		chip->port_config |= (1 << offset);
	else
		chip->port_config &= ~(1 << offset);
=======
	struct gen_74x164_chip *chip = gpiochip_get_data(gc);
	u8 bank = chip->registers - 1 - offset / 8;
	u8 pin = offset % 8;

	mutex_lock(&chip->lock);
	if (val)
		chip->buffer[bank] |= (1 << pin);
	else
		chip->buffer[bank] &= ~(1 << pin);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__gen_74x164_write_config(chip);
	mutex_unlock(&chip->lock);
}

<<<<<<< HEAD
=======
static void gen_74x164_set_multiple(struct gpio_chip *gc, unsigned long *mask,
				    unsigned long *bits)
{
	struct gen_74x164_chip *chip = gpiochip_get_data(gc);
	unsigned long offset;
	unsigned long bankmask;
	size_t bank;
	unsigned long bitmask;

	mutex_lock(&chip->lock);
	for_each_set_clump8(offset, bankmask, mask, chip->registers * 8) {
		bank = chip->registers - 1 - offset / 8;
		bitmask = bitmap_get_value8(bits, offset) & bankmask;

		chip->buffer[bank] &= ~bankmask;
		chip->buffer[bank] |= bitmask;
	}
	__gen_74x164_write_config(chip);
	mutex_unlock(&chip->lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int gen_74x164_direction_output(struct gpio_chip *gc,
		unsigned offset, int val)
{
	gen_74x164_set_value(gc, offset, val);
	return 0;
}

<<<<<<< HEAD
static int __devinit gen_74x164_probe(struct spi_device *spi)
{
	struct gen_74x164_chip *chip;
	struct gen_74x164_chip_platform_data *pdata;
	int ret;

	pdata = spi->dev.platform_data;
	if (!pdata || !pdata->base) {
		dev_dbg(&spi->dev, "incorrect or missing platform data\n");
		return -EINVAL;
	}

=======
static int gen_74x164_probe(struct spi_device *spi)
{
	struct gen_74x164_chip *chip;
	u32 nregs;
	int ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * bits_per_word cannot be configured in platform data
	 */
	spi->bits_per_word = 8;

	ret = spi_setup(spi);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	mutex_init(&chip->lock);

	dev_set_drvdata(&spi->dev, chip);

	chip->spi = spi;
=======
	ret = device_property_read_u32(&spi->dev, "registers-number", &nregs);
	if (ret) {
		dev_err(&spi->dev, "Missing 'registers-number' property.\n");
		return -EINVAL;
	}

	chip = devm_kzalloc(&spi->dev, sizeof(*chip) + nregs, GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	chip->gpiod_oe = devm_gpiod_get_optional(&spi->dev, "enable",
						 GPIOD_OUT_LOW);
	if (IS_ERR(chip->gpiod_oe))
		return PTR_ERR(chip->gpiod_oe);

	spi_set_drvdata(spi, chip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	chip->gpio_chip.label = spi->modalias;
	chip->gpio_chip.direction_output = gen_74x164_direction_output;
	chip->gpio_chip.get = gen_74x164_get_value;
	chip->gpio_chip.set = gen_74x164_set_value;
<<<<<<< HEAD
	chip->gpio_chip.base = pdata->base;
	chip->gpio_chip.ngpio = 8;
	chip->gpio_chip.can_sleep = 1;
	chip->gpio_chip.dev = &spi->dev;
	chip->gpio_chip.owner = THIS_MODULE;

=======
	chip->gpio_chip.set_multiple = gen_74x164_set_multiple;
	chip->gpio_chip.base = -1;

	chip->registers = nregs;
	chip->gpio_chip.ngpio = GEN_74X164_NUMBER_GPIOS * chip->registers;

	chip->gpio_chip.can_sleep = true;
	chip->gpio_chip.parent = &spi->dev;
	chip->gpio_chip.owner = THIS_MODULE;

	mutex_init(&chip->lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = __gen_74x164_write_config(chip);
	if (ret) {
		dev_err(&spi->dev, "Failed writing: %d\n", ret);
		goto exit_destroy;
	}

<<<<<<< HEAD
	ret = gpiochip_add(&chip->gpio_chip);
	if (ret)
		goto exit_destroy;

	return ret;

exit_destroy:
	dev_set_drvdata(&spi->dev, NULL);
	mutex_destroy(&chip->lock);
	kfree(chip);
	return ret;
}

static int __devexit gen_74x164_remove(struct spi_device *spi)
{
	struct gen_74x164_chip *chip;
	int ret;

	chip = dev_get_drvdata(&spi->dev);
	if (chip == NULL)
		return -ENODEV;

	dev_set_drvdata(&spi->dev, NULL);

	ret = gpiochip_remove(&chip->gpio_chip);
	if (!ret) {
		mutex_destroy(&chip->lock);
		kfree(chip);
	} else
		dev_err(&spi->dev, "Failed to remove the GPIO controller: %d\n",
				ret);

	return ret;
}

static struct spi_driver gen_74x164_driver = {
	.driver = {
		.name		= "74x164",
		.owner		= THIS_MODULE,
	},
	.probe		= gen_74x164_probe,
	.remove		= __devexit_p(gen_74x164_remove),
};

static int __init gen_74x164_init(void)
{
	return spi_register_driver(&gen_74x164_driver);
}
subsys_initcall(gen_74x164_init);

static void __exit gen_74x164_exit(void)
{
	spi_unregister_driver(&gen_74x164_driver);
}
module_exit(gen_74x164_exit);
=======
	gpiod_set_value_cansleep(chip->gpiod_oe, 1);

	ret = gpiochip_add_data(&chip->gpio_chip, chip);
	if (!ret)
		return 0;

exit_destroy:
	mutex_destroy(&chip->lock);

	return ret;
}

static void gen_74x164_remove(struct spi_device *spi)
{
	struct gen_74x164_chip *chip = spi_get_drvdata(spi);

	gpiod_set_value_cansleep(chip->gpiod_oe, 0);
	gpiochip_remove(&chip->gpio_chip);
	mutex_destroy(&chip->lock);
}

static const struct spi_device_id gen_74x164_spi_ids[] = {
	{ .name = "74hc595" },
	{ .name = "74lvc594" },
	{},
};
MODULE_DEVICE_TABLE(spi, gen_74x164_spi_ids);

static const struct of_device_id gen_74x164_dt_ids[] = {
	{ .compatible = "fairchild,74hc595" },
	{ .compatible = "nxp,74lvc594" },
	{},
};
MODULE_DEVICE_TABLE(of, gen_74x164_dt_ids);

static struct spi_driver gen_74x164_driver = {
	.driver = {
		.name		= "74x164",
		.of_match_table	= gen_74x164_dt_ids,
	},
	.probe		= gen_74x164_probe,
	.remove		= gen_74x164_remove,
	.id_table	= gen_74x164_spi_ids,
};
module_spi_driver(gen_74x164_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Gabor Juhos <juhosg@openwrt.org>");
MODULE_AUTHOR("Miguel Gaio <miguel.gaio@efixo.com>");
MODULE_DESCRIPTION("GPIO expander driver for 74X164 8-bits shift register");
MODULE_LICENSE("GPL v2");
