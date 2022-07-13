<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* tmp421.c
 *
 * Copyright (C) 2009 Andre Prendel <andre.prendel@gmx.de>
 * Preliminary support by:
 * Melvin Rook, Raymond Ng
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * Driver for the Texas Instruments TMP421 SMBus temperature sensor IC.
<<<<<<< HEAD
 * Supported models: TMP421, TMP422, TMP423
=======
 * Supported models: TMP421, TMP422, TMP423, TMP441, TMP442
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/mutex.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sysfs.h>

/* Addresses to scan */
static const unsigned short normal_i2c[] = { 0x2a, 0x4c, 0x4d, 0x4e, 0x4f,
					     I2C_CLIENT_END };

<<<<<<< HEAD
enum chips { tmp421, tmp422, tmp423 };

/* The TMP421 registers */
#define TMP421_CONFIG_REG_1			0x09
#define TMP421_CONVERSION_RATE_REG		0x0B
#define TMP421_MANUFACTURER_ID_REG		0xFE
#define TMP421_DEVICE_ID_REG			0xFF

static const u8 TMP421_TEMP_MSB[4]		= { 0x00, 0x01, 0x02, 0x03 };
static const u8 TMP421_TEMP_LSB[4]		= { 0x10, 0x11, 0x12, 0x13 };
=======
enum chips { tmp421, tmp422, tmp423, tmp441, tmp442 };

#define MAX_CHANNELS				4
/* The TMP421 registers */
#define TMP421_STATUS_REG			0x08
#define TMP421_CONFIG_REG_1			0x09
#define TMP421_CONFIG_REG_2			0x0A
#define TMP421_CONFIG_REG_REN(x)		(BIT(3 + (x)))
#define TMP421_CONFIG_REG_REN_MASK		GENMASK(6, 3)
#define TMP421_CONVERSION_RATE_REG		0x0B
#define TMP421_N_FACTOR_REG_1			0x21
#define TMP421_MANUFACTURER_ID_REG		0xFE
#define TMP421_DEVICE_ID_REG			0xFF

static const u8 TMP421_TEMP_MSB[MAX_CHANNELS]	= { 0x00, 0x01, 0x02, 0x03 };
static const u8 TMP421_TEMP_LSB[MAX_CHANNELS]	= { 0x10, 0x11, 0x12, 0x13 };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Flags */
#define TMP421_CONFIG_SHUTDOWN			0x40
#define TMP421_CONFIG_RANGE			0x04

/* Manufacturer / Device ID's */
#define TMP421_MANUFACTURER_ID			0x55
#define TMP421_DEVICE_ID			0x21
#define TMP422_DEVICE_ID			0x22
#define TMP423_DEVICE_ID			0x23
<<<<<<< HEAD
=======
#define TMP441_DEVICE_ID			0x41
#define TMP442_DEVICE_ID			0x42
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct i2c_device_id tmp421_id[] = {
	{ "tmp421", 2 },
	{ "tmp422", 3 },
	{ "tmp423", 4 },
<<<<<<< HEAD
=======
	{ "tmp441", 2 },
	{ "tmp442", 3 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ }
};
MODULE_DEVICE_TABLE(i2c, tmp421_id);

<<<<<<< HEAD
struct tmp421_data {
	struct device *hwmon_dev;
	struct mutex update_lock;
	char valid;
	unsigned long last_updated;
	int channels;
	u8 config;
	s16 temp[4];
};

static int temp_from_s16(s16 reg)
=======
static const struct of_device_id __maybe_unused tmp421_of_match[] = {
	{
		.compatible = "ti,tmp421",
		.data = (void *)2
	},
	{
		.compatible = "ti,tmp422",
		.data = (void *)3
	},
	{
		.compatible = "ti,tmp423",
		.data = (void *)4
	},
	{
		.compatible = "ti,tmp441",
		.data = (void *)2
	},
	{
		.compatible = "ti,tmp442",
		.data = (void *)3
	},
	{ },
};
MODULE_DEVICE_TABLE(of, tmp421_of_match);

struct tmp421_channel {
	const char *label;
	bool enabled;
	s16 temp;
};

struct tmp421_data {
	struct i2c_client *client;
	struct mutex update_lock;
	u32 temp_config[MAX_CHANNELS + 1];
	struct hwmon_channel_info temp_info;
	const struct hwmon_channel_info *info[2];
	struct hwmon_chip_info chip;
	bool valid;
	unsigned long last_updated;
	unsigned long channels;
	u8 config;
	struct tmp421_channel channel[MAX_CHANNELS];
};

static int temp_from_raw(u16 reg, bool extended)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Mask out status bits */
	int temp = reg & ~0xf;

<<<<<<< HEAD
	return (temp * 1000 + 128) / 256;
}

static int temp_from_u16(u16 reg)
{
	/* Mask out status bits */
	int temp = reg & ~0xf;

	/* Add offset for extended temperature range. */
	temp -= 64 * 256;

	return (temp * 1000 + 128) / 256;
}

static struct tmp421_data *tmp421_update_device(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct tmp421_data *data = i2c_get_clientdata(client);
=======
	if (extended)
		temp = temp - 64 * 256;
	else
		temp = (s16)temp;

	return DIV_ROUND_CLOSEST(temp * 1000, 256);
}

static int tmp421_update_device(struct tmp421_data *data)
{
	struct i2c_client *client = data->client;
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;

	mutex_lock(&data->update_lock);

<<<<<<< HEAD
	if (time_after(jiffies, data->last_updated + 2 * HZ) || !data->valid) {
		data->config = i2c_smbus_read_byte_data(client,
			TMP421_CONFIG_REG_1);

		for (i = 0; i < data->channels; i++) {
			data->temp[i] = i2c_smbus_read_byte_data(client,
				TMP421_TEMP_MSB[i]) << 8;
			data->temp[i] |= i2c_smbus_read_byte_data(client,
				TMP421_TEMP_LSB[i]);
		}
		data->last_updated = jiffies;
		data->valid = 1;
	}

	mutex_unlock(&data->update_lock);

	return data;
}

static ssize_t show_temp_value(struct device *dev,
			       struct device_attribute *devattr, char *buf)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp421_data *data = tmp421_update_device(dev);
	int temp;

	mutex_lock(&data->update_lock);
	if (data->config & TMP421_CONFIG_RANGE)
		temp = temp_from_u16(data->temp[index]);
	else
		temp = temp_from_s16(data->temp[index]);
	mutex_unlock(&data->update_lock);

	return sprintf(buf, "%d\n", temp);
}

static ssize_t show_fault(struct device *dev,
			  struct device_attribute *devattr, char *buf)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp421_data *data = tmp421_update_device(dev);

	/*
	 * The OPEN bit signals a fault. This is bit 0 of the temperature
	 * register (low byte).
	 */
	if (data->temp[index] & 0x01)
		return sprintf(buf, "1\n");
	else
		return sprintf(buf, "0\n");
}

static umode_t tmp421_is_visible(struct kobject *kobj, struct attribute *a,
				int n)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct tmp421_data *data = dev_get_drvdata(dev);
	struct device_attribute *devattr;
	unsigned int index;

	devattr = container_of(a, struct device_attribute, attr);
	index = to_sensor_dev_attr(devattr)->index;

	if (index < data->channels)
		return a->mode;
=======
	if (time_after(jiffies, data->last_updated + (HZ / 2)) ||
	    !data->valid) {
		ret = i2c_smbus_read_byte_data(client, TMP421_CONFIG_REG_1);
		if (ret < 0)
			goto exit;
		data->config = ret;

		for (i = 0; i < data->channels; i++) {
			ret = i2c_smbus_read_byte_data(client, TMP421_TEMP_MSB[i]);
			if (ret < 0)
				goto exit;
			data->channel[i].temp = ret << 8;

			ret = i2c_smbus_read_byte_data(client, TMP421_TEMP_LSB[i]);
			if (ret < 0)
				goto exit;
			data->channel[i].temp |= ret;
		}
		data->last_updated = jiffies;
		data->valid = true;
	}

exit:
	mutex_unlock(&data->update_lock);

	if (ret < 0) {
		data->valid = false;
		return ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO, show_temp_value, NULL, 0);
static SENSOR_DEVICE_ATTR(temp2_input, S_IRUGO, show_temp_value, NULL, 1);
static SENSOR_DEVICE_ATTR(temp2_fault, S_IRUGO, show_fault, NULL, 1);
static SENSOR_DEVICE_ATTR(temp3_input, S_IRUGO, show_temp_value, NULL, 2);
static SENSOR_DEVICE_ATTR(temp3_fault, S_IRUGO, show_fault, NULL, 2);
static SENSOR_DEVICE_ATTR(temp4_input, S_IRUGO, show_temp_value, NULL, 3);
static SENSOR_DEVICE_ATTR(temp4_fault, S_IRUGO, show_fault, NULL, 3);

static struct attribute *tmp421_attr[] = {
	&sensor_dev_attr_temp1_input.dev_attr.attr,
	&sensor_dev_attr_temp2_input.dev_attr.attr,
	&sensor_dev_attr_temp2_fault.dev_attr.attr,
	&sensor_dev_attr_temp3_input.dev_attr.attr,
	&sensor_dev_attr_temp3_fault.dev_attr.attr,
	&sensor_dev_attr_temp4_input.dev_attr.attr,
	&sensor_dev_attr_temp4_fault.dev_attr.attr,
	NULL
};

static const struct attribute_group tmp421_group = {
	.attrs = tmp421_attr,
	.is_visible = tmp421_is_visible,
};

static int tmp421_init_client(struct i2c_client *client)
{
	int config, config_orig;
=======
static int tmp421_enable_channels(struct tmp421_data *data)
{
	int err;
	struct i2c_client *client = data->client;
	struct device *dev = &client->dev;
	int old = i2c_smbus_read_byte_data(client, TMP421_CONFIG_REG_2);
	int new, i;

	if (old < 0) {
		dev_err(dev, "error reading register, can't disable channels\n");
		return old;
	}

	new = old & ~TMP421_CONFIG_REG_REN_MASK;
	for (i = 0; i < data->channels; i++)
		if (data->channel[i].enabled)
			new |= TMP421_CONFIG_REG_REN(i);

	if (new == old)
		return 0;

	err = i2c_smbus_write_byte_data(client, TMP421_CONFIG_REG_2, new);
	if (err < 0)
		dev_err(dev, "error writing register, can't disable channels\n");

	return err;
}

static int tmp421_read(struct device *dev, enum hwmon_sensor_types type,
		       u32 attr, int channel, long *val)
{
	struct tmp421_data *tmp421 = dev_get_drvdata(dev);
	int ret = 0;

	ret = tmp421_update_device(tmp421);
	if (ret)
		return ret;

	switch (attr) {
	case hwmon_temp_input:
		if (!tmp421->channel[channel].enabled)
			return -ENODATA;
		*val = temp_from_raw(tmp421->channel[channel].temp,
				     tmp421->config & TMP421_CONFIG_RANGE);
		return 0;
	case hwmon_temp_fault:
		if (!tmp421->channel[channel].enabled)
			return -ENODATA;
		/*
		 * Any of OPEN or /PVLD bits indicate a hardware mulfunction
		 * and the conversion result may be incorrect
		 */
		*val = !!(tmp421->channel[channel].temp & 0x03);
		return 0;
	case hwmon_temp_enable:
		*val = tmp421->channel[channel].enabled;
		return 0;
	default:
		return -EOPNOTSUPP;
	}

}

static int tmp421_read_string(struct device *dev, enum hwmon_sensor_types type,
			     u32 attr, int channel, const char **str)
{
	struct tmp421_data *data = dev_get_drvdata(dev);

	*str = data->channel[channel].label;

	return 0;
}

static int tmp421_write(struct device *dev, enum hwmon_sensor_types type,
			u32 attr, int channel, long val)
{
	struct tmp421_data *data = dev_get_drvdata(dev);
	int ret;

	switch (attr) {
	case hwmon_temp_enable:
		data->channel[channel].enabled = val;
		ret = tmp421_enable_channels(data);
		break;
	default:
	    ret = -EOPNOTSUPP;
	}

	return ret;
}

static umode_t tmp421_is_visible(const void *data, enum hwmon_sensor_types type,
				 u32 attr, int channel)
{
	switch (attr) {
	case hwmon_temp_fault:
	case hwmon_temp_input:
		return 0444;
	case hwmon_temp_label:
		return 0444;
	case hwmon_temp_enable:
		return 0644;
	default:
		return 0;
	}
}

static int tmp421_init_client(struct tmp421_data *data)
{
	int config, config_orig;
	struct i2c_client *client = data->client;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set the conversion rate to 2 Hz */
	i2c_smbus_write_byte_data(client, TMP421_CONVERSION_RATE_REG, 0x05);

	/* Start conversions (disable shutdown if necessary) */
	config = i2c_smbus_read_byte_data(client, TMP421_CONFIG_REG_1);
	if (config < 0) {
<<<<<<< HEAD
		dev_err(&client->dev, "Could not read configuration"
			 " register (%d)\n", config);
		return -ENODEV;
=======
		dev_err(&client->dev,
			"Could not read configuration register (%d)\n", config);
		return config;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	config_orig = config;
	config &= ~TMP421_CONFIG_SHUTDOWN;

	if (config != config_orig) {
		dev_info(&client->dev, "Enable monitoring chip\n");
		i2c_smbus_write_byte_data(client, TMP421_CONFIG_REG_1, config);
	}

<<<<<<< HEAD
	return 0;
=======
	return tmp421_enable_channels(data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tmp421_detect(struct i2c_client *client,
			 struct i2c_board_info *info)
{
	enum chips kind;
	struct i2c_adapter *adapter = client->adapter;
<<<<<<< HEAD
	const char *names[] = { "TMP421", "TMP422", "TMP423" };
=======
	static const char * const names[] = {
		"TMP421", "TMP422", "TMP423",
		"TMP441", "TMP442"
	};
	int addr = client->addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 reg;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;

	reg = i2c_smbus_read_byte_data(client, TMP421_MANUFACTURER_ID_REG);
	if (reg != TMP421_MANUFACTURER_ID)
		return -ENODEV;

<<<<<<< HEAD
=======
	reg = i2c_smbus_read_byte_data(client, TMP421_CONVERSION_RATE_REG);
	if (reg & 0xf8)
		return -ENODEV;

	reg = i2c_smbus_read_byte_data(client, TMP421_STATUS_REG);
	if (reg & 0x7f)
		return -ENODEV;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	reg = i2c_smbus_read_byte_data(client, TMP421_DEVICE_ID_REG);
	switch (reg) {
	case TMP421_DEVICE_ID:
		kind = tmp421;
		break;
	case TMP422_DEVICE_ID:
<<<<<<< HEAD
		kind = tmp422;
		break;
	case TMP423_DEVICE_ID:
		kind = tmp423;
		break;
=======
		if (addr == 0x2a)
			return -ENODEV;
		kind = tmp422;
		break;
	case TMP423_DEVICE_ID:
		if (addr != 0x4c && addr != 0x4d)
			return -ENODEV;
		kind = tmp423;
		break;
	case TMP441_DEVICE_ID:
		kind = tmp441;
		break;
	case TMP442_DEVICE_ID:
		if (addr != 0x4c && addr != 0x4d)
			return -ENODEV;
		kind = tmp442;
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -ENODEV;
	}

<<<<<<< HEAD
	strlcpy(info->type, tmp421_id[kind].name, I2C_NAME_SIZE);
=======
	strscpy(info->type, tmp421_id[kind].name, I2C_NAME_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev_info(&adapter->dev, "Detected TI %s chip at 0x%02x\n",
		 names[kind], client->addr);

	return 0;
}

<<<<<<< HEAD
static int tmp421_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct tmp421_data *data;
	int err;

	data = kzalloc(sizeof(struct tmp421_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);
	data->channels = id->driver_data;

	err = tmp421_init_client(client);
	if (err)
		goto exit_free;

	err = sysfs_create_group(&client->dev.kobj, &tmp421_group);
	if (err)
		goto exit_free;

	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		data->hwmon_dev = NULL;
		goto exit_remove;
	}
	return 0;

exit_remove:
	sysfs_remove_group(&client->dev.kobj, &tmp421_group);

exit_free:
	kfree(data);

	return err;
}

static int tmp421_remove(struct i2c_client *client)
{
	struct tmp421_data *data = i2c_get_clientdata(client);

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &tmp421_group);

	kfree(data);

	return 0;
=======
static int tmp421_probe_child_from_dt(struct i2c_client *client,
				      struct device_node *child,
				      struct tmp421_data *data)

{
	struct device *dev = &client->dev;
	u32 i;
	s32 val;
	int err;

	err = of_property_read_u32(child, "reg", &i);
	if (err) {
		dev_err(dev, "missing reg property of %pOFn\n", child);
		return err;
	}

	if (i >= data->channels) {
		dev_err(dev, "invalid reg %d of %pOFn\n", i, child);
		return -EINVAL;
	}

	of_property_read_string(child, "label", &data->channel[i].label);
	if (data->channel[i].label)
		data->temp_config[i] |= HWMON_T_LABEL;

	data->channel[i].enabled = of_device_is_available(child);

	err = of_property_read_s32(child, "ti,n-factor", &val);
	if (!err) {
		if (i == 0) {
			dev_err(dev, "n-factor can't be set for internal channel\n");
			return -EINVAL;
		}

		if (val > 127 || val < -128) {
			dev_err(dev, "n-factor for channel %d invalid (%d)\n",
				i, val);
			return -EINVAL;
		}
		i2c_smbus_write_byte_data(client, TMP421_N_FACTOR_REG_1 + i - 1,
					  val);
	}

	return 0;
}

static int tmp421_probe_from_dt(struct i2c_client *client, struct tmp421_data *data)
{
	struct device *dev = &client->dev;
	const struct device_node *np = dev->of_node;
	struct device_node *child;
	int err;

	for_each_child_of_node(np, child) {
		if (strcmp(child->name, "channel"))
			continue;

		err = tmp421_probe_child_from_dt(client, child, data);
		if (err) {
			of_node_put(child);
			return err;
		}
	}

	return 0;
}

static const struct hwmon_ops tmp421_ops = {
	.is_visible = tmp421_is_visible,
	.read = tmp421_read,
	.read_string = tmp421_read_string,
	.write = tmp421_write,
};

static int tmp421_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct device *hwmon_dev;
	struct tmp421_data *data;
	int i, err;

	data = devm_kzalloc(dev, sizeof(struct tmp421_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	mutex_init(&data->update_lock);
	if (client->dev.of_node)
		data->channels = (unsigned long)
			of_device_get_match_data(&client->dev);
	else
		data->channels = i2c_match_id(tmp421_id, client)->driver_data;
	data->client = client;

	for (i = 0; i < data->channels; i++) {
		data->temp_config[i] = HWMON_T_INPUT | HWMON_T_FAULT | HWMON_T_ENABLE;
		data->channel[i].enabled = true;
	}

	err = tmp421_probe_from_dt(client, data);
	if (err)
		return err;

	err = tmp421_init_client(data);
	if (err)
		return err;

	data->chip.ops = &tmp421_ops;
	data->chip.info = data->info;

	data->info[0] = &data->temp_info;

	data->temp_info.type = hwmon_temp;
	data->temp_info.config = data->temp_config;

	hwmon_dev = devm_hwmon_device_register_with_info(dev, client->name,
							 data,
							 &data->chip,
							 NULL);
	return PTR_ERR_OR_ZERO(hwmon_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct i2c_driver tmp421_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		.name	= "tmp421",
<<<<<<< HEAD
	},
	.probe = tmp421_probe,
	.remove = tmp421_remove,
=======
		.of_match_table = of_match_ptr(tmp421_of_match),
	},
	.probe = tmp421_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table = tmp421_id,
	.detect = tmp421_detect,
	.address_list = normal_i2c,
};

module_i2c_driver(tmp421_driver);

MODULE_AUTHOR("Andre Prendel <andre.prendel@gmx.de>");
<<<<<<< HEAD
MODULE_DESCRIPTION("Texas Instruments TMP421/422/423 temperature sensor"
		   " driver");
=======
MODULE_DESCRIPTION("Texas Instruments TMP421/422/423/441/442 temperature sensor driver");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
