<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AD7879-1/AD7889-1 touchscreen (I2C bus)
 *
 * Copyright (C) 2008-2010 Michael Hennerich, Analog Devices Inc.
<<<<<<< HEAD
 *
 * Licensed under the GPL-2 or later.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/input.h>	/* BUS_I2C */
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <linux/pm.h>
=======
#include <linux/of.h>
#include <linux/pm.h>
#include <linux/regmap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "ad7879.h"

#define AD7879_DEVID		0x79	/* AD7879-1/AD7889-1 */

<<<<<<< HEAD
/* All registers are word-sized.
 * AD7879 uses a high-byte first convention.
 */
static int ad7879_i2c_read(struct device *dev, u8 reg)
{
	struct i2c_client *client = to_i2c_client(dev);

	return i2c_smbus_read_word_swapped(client, reg);
}

static int ad7879_i2c_multi_read(struct device *dev,
				 u8 first_reg, u8 count, u16 *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 idx;

	i2c_smbus_read_i2c_block_data(client, first_reg, count * 2, (u8 *)buf);

	for (idx = 0; idx < count; ++idx)
		buf[idx] = swab16(buf[idx]);

	return 0;
}

static int ad7879_i2c_write(struct device *dev, u8 reg, u16 val)
{
	struct i2c_client *client = to_i2c_client(dev);

	return i2c_smbus_write_word_swapped(client, reg, val);
}

static const struct ad7879_bus_ops ad7879_i2c_bus_ops = {
	.bustype	= BUS_I2C,
	.read		= ad7879_i2c_read,
	.multi_read	= ad7879_i2c_multi_read,
	.write		= ad7879_i2c_write,
};

static int __devinit ad7879_i2c_probe(struct i2c_client *client,
				      const struct i2c_device_id *id)
{
	struct ad7879 *ts;
=======
static const struct regmap_config ad7879_i2c_regmap_config = {
	.reg_bits = 8,
	.val_bits = 16,
	.max_register = 15,
};

static int ad7879_i2c_probe(struct i2c_client *client)
{
	struct regmap *regmap;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!i2c_check_functionality(client->adapter,
				     I2C_FUNC_SMBUS_WORD_DATA)) {
		dev_err(&client->dev, "SMBUS Word Data not Supported\n");
		return -EIO;
	}

<<<<<<< HEAD
	ts = ad7879_probe(&client->dev, AD7879_DEVID, client->irq,
			  &ad7879_i2c_bus_ops);
	if (IS_ERR(ts))
		return PTR_ERR(ts);

	i2c_set_clientdata(client, ts);

	return 0;
}

static int __devexit ad7879_i2c_remove(struct i2c_client *client)
{
	struct ad7879 *ts = i2c_get_clientdata(client);

	ad7879_remove(ts);

	return 0;
=======
	regmap = devm_regmap_init_i2c(client, &ad7879_i2c_regmap_config);
	if (IS_ERR(regmap))
		return PTR_ERR(regmap);

	return ad7879_probe(&client->dev, regmap, client->irq,
			    BUS_I2C, AD7879_DEVID);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct i2c_device_id ad7879_id[] = {
	{ "ad7879", 0 },
	{ "ad7889", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ad7879_id);

<<<<<<< HEAD
static struct i2c_driver ad7879_i2c_driver = {
	.driver = {
		.name	= "ad7879",
		.owner	= THIS_MODULE,
		.pm	= &ad7879_pm_ops,
	},
	.probe		= ad7879_i2c_probe,
	.remove		= __devexit_p(ad7879_i2c_remove),
=======
#ifdef CONFIG_OF
static const struct of_device_id ad7879_i2c_dt_ids[] = {
	{ .compatible = "adi,ad7879-1", },
	{ }
};
MODULE_DEVICE_TABLE(of, ad7879_i2c_dt_ids);
#endif

static struct i2c_driver ad7879_i2c_driver = {
	.driver = {
		.name		= "ad7879",
		.dev_groups	= ad7879_groups,
		.pm		= &ad7879_pm_ops,
		.of_match_table	= of_match_ptr(ad7879_i2c_dt_ids),
	},
	.probe		= ad7879_i2c_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table	= ad7879_id,
};

module_i2c_driver(ad7879_i2c_driver);

<<<<<<< HEAD
MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>");
=======
MODULE_AUTHOR("Michael Hennerich <michael.hennerich@analog.com>");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION("AD7879(-1) touchscreen I2C bus driver");
MODULE_LICENSE("GPL");
