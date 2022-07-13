<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * I2C bus driver for ADT7316/7/8 ADT7516/7/9 digital temperature
 * sensor, ADC and DAC
 *
 * Copyright 2010 Analog Devices Inc.
<<<<<<< HEAD
 *
 * Licensed under the GPL-2 or later.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/module.h>

#include "adt7316.h"

/*
 * adt7316 register access by I2C
 */
static int adt7316_i2c_read(void *client, u8 reg, u8 *data)
{
	struct i2c_client *cl = client;
<<<<<<< HEAD
	int ret = 0;
=======
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = i2c_smbus_write_byte(cl, reg);
	if (ret < 0) {
		dev_err(&cl->dev, "I2C fail to select reg\n");
		return ret;
	}

	ret = i2c_smbus_read_byte(client);
	if (ret < 0) {
		dev_err(&cl->dev, "I2C read error\n");
		return ret;
	}

<<<<<<< HEAD
=======
	*data = ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int adt7316_i2c_write(void *client, u8 reg, u8 data)
{
	struct i2c_client *cl = client;
<<<<<<< HEAD
	int ret = 0;
=======
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = i2c_smbus_write_byte_data(cl, reg, data);
	if (ret < 0)
		dev_err(&cl->dev, "I2C write error\n");

	return ret;
}

static int adt7316_i2c_multi_read(void *client, u8 reg, u8 count, u8 *data)
{
	struct i2c_client *cl = client;
<<<<<<< HEAD
	int i, ret = 0;
=======
	int i, ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (count > ADT7316_REG_MAX_ADDR)
		count = ADT7316_REG_MAX_ADDR;

	for (i = 0; i < count; i++) {
		ret = adt7316_i2c_read(cl, reg, &data[i]);
		if (ret < 0) {
			dev_err(&cl->dev, "I2C multi read error\n");
			return ret;
		}
	}

	return 0;
}

static int adt7316_i2c_multi_write(void *client, u8 reg, u8 count, u8 *data)
{
	struct i2c_client *cl = client;
<<<<<<< HEAD
	int i, ret = 0;
=======
	int i, ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (count > ADT7316_REG_MAX_ADDR)
		count = ADT7316_REG_MAX_ADDR;

	for (i = 0; i < count; i++) {
		ret = adt7316_i2c_write(cl, reg, data[i]);
		if (ret < 0) {
			dev_err(&cl->dev, "I2C multi write error\n");
			return ret;
		}
	}

	return 0;
}

/*
 * device probe and remove
 */

<<<<<<< HEAD
static int __devinit adt7316_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct adt7316_bus bus = {
		.client = client,
		.irq = client->irq,
		.irq_flags = IRQF_TRIGGER_LOW,
=======
static int adt7316_i2c_probe(struct i2c_client *client)
{
	const struct i2c_device_id *id = i2c_client_get_device_id(client);
	struct adt7316_bus bus = {
		.client = client,
		.irq = client->irq,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.read = adt7316_i2c_read,
		.write = adt7316_i2c_write,
		.multi_read = adt7316_i2c_multi_read,
		.multi_write = adt7316_i2c_multi_write,
	};

	return adt7316_probe(&client->dev, &bus, id->name);
}

<<<<<<< HEAD
static int __devexit adt7316_i2c_remove(struct i2c_client *client)
{
	return adt7316_remove(&client->dev);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct i2c_device_id adt7316_i2c_id[] = {
	{ "adt7316", 0 },
	{ "adt7317", 0 },
	{ "adt7318", 0 },
	{ "adt7516", 0 },
	{ "adt7517", 0 },
	{ "adt7519", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, adt7316_i2c_id);

<<<<<<< HEAD
static struct i2c_driver adt7316_driver = {
	.driver = {
		.name = "adt7316",
		.pm = ADT7316_PM_OPS,
		.owner  = THIS_MODULE,
	},
	.probe = adt7316_i2c_probe,
	.remove = __devexit_p(adt7316_i2c_remove),
=======
static const struct of_device_id adt7316_of_match[] = {
	{ .compatible = "adi,adt7316" },
	{ .compatible = "adi,adt7317" },
	{ .compatible = "adi,adt7318" },
	{ .compatible = "adi,adt7516" },
	{ .compatible = "adi,adt7517" },
	{ .compatible = "adi,adt7519" },
	{ },
};

MODULE_DEVICE_TABLE(of, adt7316_of_match);

static struct i2c_driver adt7316_driver = {
	.driver = {
		.name = "adt7316",
		.of_match_table = adt7316_of_match,
		.pm = ADT7316_PM_OPS,
	},
	.probe = adt7316_i2c_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table = adt7316_i2c_id,
};
module_i2c_driver(adt7316_driver);

MODULE_AUTHOR("Sonic Zhang <sonic.zhang@analog.com>");
<<<<<<< HEAD
MODULE_DESCRIPTION("I2C bus driver for Analog Devices ADT7316/7/9 and"
			"ADT7516/7/8 digital temperature sensor, ADC and DAC");
=======
MODULE_DESCRIPTION("I2C bus driver for Analog Devices ADT7316/7/9 and ADT7516/7/8 digital temperature sensor, ADC and DAC");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL v2");
