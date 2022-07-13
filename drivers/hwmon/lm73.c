<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * LM73 Sensor driver
 * Based on LM75
 *
 * Copyright (C) 2007, CenoSYS (www.cenosys.com).
 * Copyright (C) 2009, Bollore telecom (www.bolloretelecom.eu).
 *
 * Guillaume Ligneul <guillaume.ligneul@gmail.com>
 * Adrien Demarez <adrien.demarez@bolloretelecom.eu>
 * Jeremy Laine <jeremy.laine@bolloretelecom.eu>
<<<<<<< HEAD
 *
 * This software program is licensed subject to the GNU General Public License
 * (GPL).Version 2,June 1991, available at
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
=======
 * Chris Verges <kg4ysn@gmail.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>


/* Addresses scanned */
static const unsigned short normal_i2c[] = { 0x48, 0x49, 0x4a, 0x4c,
					0x4d, 0x4e, I2C_CLIENT_END };

/* LM73 registers */
#define LM73_REG_INPUT		0x00
#define LM73_REG_CONF		0x01
#define LM73_REG_MAX		0x02
#define LM73_REG_MIN		0x03
#define LM73_REG_CTRL		0x04
#define LM73_REG_ID		0x07

#define LM73_ID			0x9001	/* 0x0190, byte-swapped */
#define DRVNAME			"lm73"
<<<<<<< HEAD
#define LM73_TEMP_MIN		(-40)
#define LM73_TEMP_MAX		150

/*-----------------------------------------------------------------------*/


static ssize_t set_temp(struct device *dev, struct device_attribute *da,
			const char *buf, size_t count)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct i2c_client *client = to_i2c_client(dev);
=======
#define LM73_TEMP_MIN		(-256000 / 250)
#define LM73_TEMP_MAX		(255750 / 250)

#define LM73_CTRL_RES_SHIFT	5
#define LM73_CTRL_RES_MASK	(BIT(5) | BIT(6))
#define LM73_CTRL_TO_MASK	BIT(7)

#define LM73_CTRL_HI_SHIFT	2
#define LM73_CTRL_LO_SHIFT	1

static const unsigned short lm73_convrates[] = {
	14,	/* 11-bits (0.25000 C/LSB): RES1 Bit = 0, RES0 Bit = 0 */
	28,	/* 12-bits (0.12500 C/LSB): RES1 Bit = 0, RES0 Bit = 1 */
	56,	/* 13-bits (0.06250 C/LSB): RES1 Bit = 1, RES0 Bit = 0 */
	112,	/* 14-bits (0.03125 C/LSB): RES1 Bit = 1, RES0 Bit = 1 */
};

struct lm73_data {
	struct i2c_client *client;
	struct mutex lock;
	u8 ctrl;			/* control register value */
};

/*-----------------------------------------------------------------------*/

static ssize_t temp_store(struct device *dev, struct device_attribute *da,
			  const char *buf, size_t count)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm73_data *data = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	long temp;
	short value;
	s32 err;

	int status = kstrtol(buf, 10, &temp);
	if (status < 0)
		return status;

	/* Write value */
<<<<<<< HEAD
	value = (short) SENSORS_LIMIT(temp/250, (LM73_TEMP_MIN*4),
		(LM73_TEMP_MAX*4)) << 5;
	err = i2c_smbus_write_word_swapped(client, attr->index, value);
	return (err < 0) ? err : count;
}

static ssize_t show_temp(struct device *dev, struct device_attribute *da,
			 char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct i2c_client *client = to_i2c_client(dev);
	int temp;

	s32 err = i2c_smbus_read_word_swapped(client, attr->index);
=======
	value = clamp_val(temp / 250, LM73_TEMP_MIN, LM73_TEMP_MAX) << 5;
	err = i2c_smbus_write_word_swapped(data->client, attr->index, value);
	return (err < 0) ? err : count;
}

static ssize_t temp_show(struct device *dev, struct device_attribute *da,
			 char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm73_data *data = dev_get_drvdata(dev);
	int temp;

	s32 err = i2c_smbus_read_word_swapped(data->client, attr->index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		return err;

	/* use integer division instead of equivalent right shift to
	   guarantee arithmetic shift and preserve the sign */
	temp = (((s16) err) * 250) / 32;
<<<<<<< HEAD
	return scnprintf(buf, PAGE_SIZE, "%d\n", temp);
}

=======
	return sysfs_emit(buf, "%d\n", temp);
}

static ssize_t convrate_store(struct device *dev, struct device_attribute *da,
			      const char *buf, size_t count)
{
	struct lm73_data *data = dev_get_drvdata(dev);
	unsigned long convrate;
	s32 err;
	int res = 0;

	err = kstrtoul(buf, 10, &convrate);
	if (err < 0)
		return err;

	/*
	 * Convert the desired conversion rate into register bits.
	 * res is already initialized, and everything past the second-to-last
	 * value in the array is treated as belonging to the last value
	 * in the array.
	 */
	while (res < (ARRAY_SIZE(lm73_convrates) - 1) &&
			convrate > lm73_convrates[res])
		res++;

	mutex_lock(&data->lock);
	data->ctrl &= LM73_CTRL_TO_MASK;
	data->ctrl |= res << LM73_CTRL_RES_SHIFT;
	err = i2c_smbus_write_byte_data(data->client, LM73_REG_CTRL,
					data->ctrl);
	mutex_unlock(&data->lock);

	if (err < 0)
		return err;

	return count;
}

static ssize_t convrate_show(struct device *dev, struct device_attribute *da,
			     char *buf)
{
	struct lm73_data *data = dev_get_drvdata(dev);
	int res;

	res = (data->ctrl & LM73_CTRL_RES_MASK) >> LM73_CTRL_RES_SHIFT;
	return sysfs_emit(buf, "%hu\n", lm73_convrates[res]);
}

static ssize_t maxmin_alarm_show(struct device *dev,
				 struct device_attribute *da, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm73_data *data = dev_get_drvdata(dev);
	s32 ctrl;

	mutex_lock(&data->lock);
	ctrl = i2c_smbus_read_byte_data(data->client, LM73_REG_CTRL);
	if (ctrl < 0)
		goto abort;
	data->ctrl = ctrl;
	mutex_unlock(&data->lock);

	return sysfs_emit(buf, "%d\n", (ctrl >> attr->index) & 1);

abort:
	mutex_unlock(&data->lock);
	return ctrl;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*-----------------------------------------------------------------------*/

/* sysfs attributes for hwmon */

<<<<<<< HEAD
static SENSOR_DEVICE_ATTR(temp1_max, S_IWUSR | S_IRUGO,
			show_temp, set_temp, LM73_REG_MAX);
static SENSOR_DEVICE_ATTR(temp1_min, S_IWUSR | S_IRUGO,
			show_temp, set_temp, LM73_REG_MIN);
static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO,
			show_temp, NULL, LM73_REG_INPUT);


static struct attribute *lm73_attributes[] = {
	&sensor_dev_attr_temp1_input.dev_attr.attr,
	&sensor_dev_attr_temp1_max.dev_attr.attr,
	&sensor_dev_attr_temp1_min.dev_attr.attr,

	NULL
};

static const struct attribute_group lm73_group = {
	.attrs = lm73_attributes,
};
=======
static SENSOR_DEVICE_ATTR_RW(temp1_max, temp, LM73_REG_MAX);
static SENSOR_DEVICE_ATTR_RW(temp1_min, temp, LM73_REG_MIN);
static SENSOR_DEVICE_ATTR_RO(temp1_input, temp, LM73_REG_INPUT);
static SENSOR_DEVICE_ATTR_RW(update_interval, convrate, 0);
static SENSOR_DEVICE_ATTR_RO(temp1_max_alarm, maxmin_alarm,
			     LM73_CTRL_HI_SHIFT);
static SENSOR_DEVICE_ATTR_RO(temp1_min_alarm, maxmin_alarm,
			     LM73_CTRL_LO_SHIFT);

static struct attribute *lm73_attrs[] = {
	&sensor_dev_attr_temp1_input.dev_attr.attr,
	&sensor_dev_attr_temp1_max.dev_attr.attr,
	&sensor_dev_attr_temp1_min.dev_attr.attr,
	&sensor_dev_attr_update_interval.dev_attr.attr,
	&sensor_dev_attr_temp1_max_alarm.dev_attr.attr,
	&sensor_dev_attr_temp1_min_alarm.dev_attr.attr,
	NULL
};
ATTRIBUTE_GROUPS(lm73);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*-----------------------------------------------------------------------*/

/* device probe and removal */

static int
<<<<<<< HEAD
lm73_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct device *hwmon_dev;
	int status;

	/* Register sysfs hooks */
	status = sysfs_create_group(&client->dev.kobj, &lm73_group);
	if (status)
		return status;

	hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(hwmon_dev)) {
		status = PTR_ERR(hwmon_dev);
		goto exit_remove;
	}
	i2c_set_clientdata(client, hwmon_dev);

	dev_info(&client->dev, "%s: sensor '%s'\n",
		 dev_name(hwmon_dev), client->name);

	return 0;

exit_remove:
	sysfs_remove_group(&client->dev.kobj, &lm73_group);
	return status;
}

static int lm73_remove(struct i2c_client *client)
{
	struct device *hwmon_dev = i2c_get_clientdata(client);

	hwmon_device_unregister(hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &lm73_group);
=======
lm73_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct device *hwmon_dev;
	struct lm73_data *data;
	int ctrl;

	data = devm_kzalloc(dev, sizeof(struct lm73_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->client = client;
	mutex_init(&data->lock);

	ctrl = i2c_smbus_read_byte_data(client, LM73_REG_CTRL);
	if (ctrl < 0)
		return ctrl;
	data->ctrl = ctrl;

	hwmon_dev = devm_hwmon_device_register_with_groups(dev, client->name,
							   data, lm73_groups);
	if (IS_ERR(hwmon_dev))
		return PTR_ERR(hwmon_dev);

	dev_info(dev, "sensor '%s'\n", client->name);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct i2c_device_id lm73_ids[] = {
	{ "lm73", 0 },
	{ /* LIST END */ }
};
MODULE_DEVICE_TABLE(i2c, lm73_ids);

/* Return 0 if detection is successful, -ENODEV otherwise */
static int lm73_detect(struct i2c_client *new_client,
			struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = new_client->adapter;
	int id, ctrl, conf;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA |
					I2C_FUNC_SMBUS_WORD_DATA))
		return -ENODEV;

	/*
	 * Do as much detection as possible with byte reads first, as word
	 * reads can confuse other devices.
	 */
	ctrl = i2c_smbus_read_byte_data(new_client, LM73_REG_CTRL);
	if (ctrl < 0 || (ctrl & 0x10))
		return -ENODEV;

	conf = i2c_smbus_read_byte_data(new_client, LM73_REG_CONF);
	if (conf < 0 || (conf & 0x0c))
		return -ENODEV;

	id = i2c_smbus_read_byte_data(new_client, LM73_REG_ID);
	if (id < 0 || id != (LM73_ID & 0xff))
		return -ENODEV;

	/* Check device ID */
	id = i2c_smbus_read_word_data(new_client, LM73_REG_ID);
	if (id < 0 || id != LM73_ID)
		return -ENODEV;

<<<<<<< HEAD
	strlcpy(info->type, "lm73", I2C_NAME_SIZE);
=======
	strscpy(info->type, "lm73", I2C_NAME_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
=======
static const struct of_device_id lm73_of_match[] = {
	{
		.compatible = "ti,lm73",
	},
	{ },
};

MODULE_DEVICE_TABLE(of, lm73_of_match);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct i2c_driver lm73_driver = {
	.class		= I2C_CLASS_HWMON,
	.driver = {
		.name	= "lm73",
<<<<<<< HEAD
	},
	.probe		= lm73_probe,
	.remove		= lm73_remove,
=======
		.of_match_table = lm73_of_match,
	},
	.probe		= lm73_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table	= lm73_ids,
	.detect		= lm73_detect,
	.address_list	= normal_i2c,
};

module_i2c_driver(lm73_driver);

MODULE_AUTHOR("Guillaume Ligneul <guillaume.ligneul@gmail.com>");
MODULE_DESCRIPTION("LM73 driver");
MODULE_LICENSE("GPL");
