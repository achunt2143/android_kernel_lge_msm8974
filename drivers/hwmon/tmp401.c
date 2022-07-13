<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* tmp401.c
 *
 * Copyright (C) 2007,2008 Hans de Goede <hdegoede@redhat.com>
 * Preliminary tmp411 support by:
 * Gabriel Konat, Sander Leget, Wouter Willems
 * Copyright (C) 2009 Andre Prendel <andre.prendel@gmx.de>
 *
<<<<<<< HEAD
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
 * Cleanup and support for TMP431 and TMP432 by Guenter Roeck
 * Copyright (c) 2013 Guenter Roeck <linux@roeck-us.net>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * Driver for the Texas Instruments TMP401 SMBUS temperature sensor IC.
 *
 * Note this IC is in some aspect similar to the LM90, but it has quite a
 * few differences too, for example the local temp has a higher resolution
 * and thus has 16 bits registers for its value and limit instead of 8 bits.
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/sysfs.h>

/* Addresses to scan */
static const unsigned short normal_i2c[] = { 0x4c, I2C_CLIENT_END };

enum chips { tmp401, tmp411 };
=======
#include <linux/bitops.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/regmap.h>
#include <linux/slab.h>

/* Addresses to scan */
static const unsigned short normal_i2c[] = { 0x48, 0x49, 0x4a, 0x4c, 0x4d,
	0x4e, 0x4f, I2C_CLIENT_END };

enum chips { tmp401, tmp411, tmp431, tmp432, tmp435 };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The TMP401 registers, note some registers have different addresses for
 * reading and writing
 */
#define TMP401_STATUS				0x02
<<<<<<< HEAD
#define TMP401_CONFIG_READ			0x03
#define TMP401_CONFIG_WRITE			0x09
#define TMP401_CONVERSION_RATE_READ		0x04
#define TMP401_CONVERSION_RATE_WRITE		0x0A
#define TMP401_TEMP_CRIT_HYST			0x21
#define TMP401_CONSECUTIVE_ALERT		0x22
#define TMP401_MANUFACTURER_ID_REG		0xFE
#define TMP401_DEVICE_ID_REG			0xFF
#define TMP411_N_FACTOR_REG			0x18

static const u8 TMP401_TEMP_MSB[2]			= { 0x00, 0x01 };
static const u8 TMP401_TEMP_LSB[2]			= { 0x15, 0x10 };
static const u8 TMP401_TEMP_LOW_LIMIT_MSB_READ[2]	= { 0x06, 0x08 };
static const u8 TMP401_TEMP_LOW_LIMIT_MSB_WRITE[2]	= { 0x0C, 0x0E };
static const u8 TMP401_TEMP_LOW_LIMIT_LSB[2]		= { 0x17, 0x14 };
static const u8 TMP401_TEMP_HIGH_LIMIT_MSB_READ[2]	= { 0x05, 0x07 };
static const u8 TMP401_TEMP_HIGH_LIMIT_MSB_WRITE[2]	= { 0x0B, 0x0D };
static const u8 TMP401_TEMP_HIGH_LIMIT_LSB[2]		= { 0x16, 0x13 };
/* These are called the THERM limit / hysteresis / mask in the datasheet */
static const u8 TMP401_TEMP_CRIT_LIMIT[2]		= { 0x20, 0x19 };

static const u8 TMP411_TEMP_LOWEST_MSB[2]		= { 0x30, 0x34 };
static const u8 TMP411_TEMP_LOWEST_LSB[2]		= { 0x31, 0x35 };
static const u8 TMP411_TEMP_HIGHEST_MSB[2]		= { 0x32, 0x36 };
static const u8 TMP411_TEMP_HIGHEST_LSB[2]		= { 0x33, 0x37 };

/* Flags */
#define TMP401_CONFIG_RANGE		0x04
#define TMP401_CONFIG_SHUTDOWN		0x40
#define TMP401_STATUS_LOCAL_CRIT		0x01
#define TMP401_STATUS_REMOTE_CRIT		0x02
#define TMP401_STATUS_REMOTE_OPEN		0x04
#define TMP401_STATUS_REMOTE_LOW		0x08
#define TMP401_STATUS_REMOTE_HIGH		0x10
#define TMP401_STATUS_LOCAL_LOW		0x20
#define TMP401_STATUS_LOCAL_HIGH		0x40
=======
#define TMP401_CONFIG				0x03
#define TMP401_CONVERSION_RATE			0x04
#define TMP4XX_N_FACTOR_REG			0x18
#define TMP43X_BETA_RANGE			0x25
#define TMP401_TEMP_CRIT_HYST			0x21
#define TMP401_MANUFACTURER_ID_REG		0xFE
#define TMP401_DEVICE_ID_REG			0xFF

static const u8 TMP401_TEMP_MSB[7][3] = {
	{ 0x00, 0x01, 0x23 },	/* temp */
	{ 0x06, 0x08, 0x16 },	/* low limit */
	{ 0x05, 0x07, 0x15 },	/* high limit */
	{ 0x20, 0x19, 0x1a },	/* therm (crit) limit */
	{ 0x30, 0x34, 0x00 },	/* lowest */
	{ 0x32, 0xf6, 0x00 },	/* highest */
};

/* [0] = fault, [1] = low, [2] = high, [3] = therm/crit */
static const u8 TMP432_STATUS_REG[] = {
	0x1b, 0x36, 0x35, 0x37 };

/* Flags */
#define TMP401_CONFIG_RANGE			BIT(2)
#define TMP401_CONFIG_SHUTDOWN			BIT(6)
#define TMP401_STATUS_LOCAL_CRIT		BIT(0)
#define TMP401_STATUS_REMOTE_CRIT		BIT(1)
#define TMP401_STATUS_REMOTE_OPEN		BIT(2)
#define TMP401_STATUS_REMOTE_LOW		BIT(3)
#define TMP401_STATUS_REMOTE_HIGH		BIT(4)
#define TMP401_STATUS_LOCAL_LOW			BIT(5)
#define TMP401_STATUS_LOCAL_HIGH		BIT(6)

/* On TMP432, each status has its own register */
#define TMP432_STATUS_LOCAL			BIT(0)
#define TMP432_STATUS_REMOTE1			BIT(1)
#define TMP432_STATUS_REMOTE2			BIT(2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Manufacturer / Device ID's */
#define TMP401_MANUFACTURER_ID			0x55
#define TMP401_DEVICE_ID			0x11
<<<<<<< HEAD
#define TMP411_DEVICE_ID			0x12
=======
#define TMP411A_DEVICE_ID			0x12
#define TMP411B_DEVICE_ID			0x13
#define TMP411C_DEVICE_ID			0x10
#define TMP431_DEVICE_ID			0x31
#define TMP432_DEVICE_ID			0x32
#define TMP435_DEVICE_ID			0x35
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Driver data (common to all clients)
 */

static const struct i2c_device_id tmp401_id[] = {
	{ "tmp401", tmp401 },
	{ "tmp411", tmp411 },
<<<<<<< HEAD
=======
	{ "tmp431", tmp431 },
	{ "tmp432", tmp432 },
	{ "tmp435", tmp435 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ }
};
MODULE_DEVICE_TABLE(i2c, tmp401_id);

/*
 * Client data (each client gets its own)
 */

struct tmp401_data {
<<<<<<< HEAD
	struct device *hwmon_dev;
	struct mutex update_lock;
	char valid; /* zero until following fields are valid */
	unsigned long last_updated; /* in jiffies */
	enum chips kind;

	/* register values */
	u8 status;
	u8 config;
	u16 temp[2];
	u16 temp_low[2];
	u16 temp_high[2];
	u8 temp_crit[2];
	u8 temp_crit_hyst;
	u16 temp_lowest[2];
	u16 temp_highest[2];
};

/*
 * Sysfs attr show / store functions
 */

static int tmp401_register_to_temp(u16 reg, u8 config)
{
	int temp = reg;

	if (config & TMP401_CONFIG_RANGE)
		temp -= 64 * 256;

	return (temp * 625 + 80) / 160;
}

static u16 tmp401_temp_to_register(long temp, u8 config)
{
	if (config & TMP401_CONFIG_RANGE) {
		temp = SENSORS_LIMIT(temp, -64000, 191000);
		temp += 64000;
	} else
		temp = SENSORS_LIMIT(temp, 0, 127000);

	return (temp * 160 + 312) / 625;
}

static int tmp401_crit_register_to_temp(u8 reg, u8 config)
{
	int temp = reg;

	if (config & TMP401_CONFIG_RANGE)
		temp -= 64;

	return temp * 1000;
}

static u8 tmp401_crit_temp_to_register(long temp, u8 config)
{
	if (config & TMP401_CONFIG_RANGE) {
		temp = SENSORS_LIMIT(temp, -64000, 191000);
		temp += 64000;
	} else
		temp = SENSORS_LIMIT(temp, 0, 127000);

	return (temp + 500) / 1000;
}

static struct tmp401_data *tmp401_update_device_reg16(
	struct i2c_client *client, struct tmp401_data *data)
{
	int i;

	for (i = 0; i < 2; i++) {
		/*
		 * High byte must be read first immediately followed
		 * by the low byte
		 */
		data->temp[i] = i2c_smbus_read_byte_data(client,
			TMP401_TEMP_MSB[i]) << 8;
		data->temp[i] |= i2c_smbus_read_byte_data(client,
			TMP401_TEMP_LSB[i]);
		data->temp_low[i] = i2c_smbus_read_byte_data(client,
			TMP401_TEMP_LOW_LIMIT_MSB_READ[i]) << 8;
		data->temp_low[i] |= i2c_smbus_read_byte_data(client,
			TMP401_TEMP_LOW_LIMIT_LSB[i]);
		data->temp_high[i] = i2c_smbus_read_byte_data(client,
			TMP401_TEMP_HIGH_LIMIT_MSB_READ[i]) << 8;
		data->temp_high[i] |= i2c_smbus_read_byte_data(client,
			TMP401_TEMP_HIGH_LIMIT_LSB[i]);
		data->temp_crit[i] = i2c_smbus_read_byte_data(client,
			TMP401_TEMP_CRIT_LIMIT[i]);

		if (data->kind == tmp411) {
			data->temp_lowest[i] = i2c_smbus_read_byte_data(client,
				TMP411_TEMP_LOWEST_MSB[i]) << 8;
			data->temp_lowest[i] |= i2c_smbus_read_byte_data(
				client, TMP411_TEMP_LOWEST_LSB[i]);

			data->temp_highest[i] = i2c_smbus_read_byte_data(
				client, TMP411_TEMP_HIGHEST_MSB[i]) << 8;
			data->temp_highest[i] |= i2c_smbus_read_byte_data(
				client, TMP411_TEMP_HIGHEST_LSB[i]);
		}
	}
	return data;
}

static struct tmp401_data *tmp401_update_device(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct tmp401_data *data = i2c_get_clientdata(client);

	mutex_lock(&data->update_lock);

	if (time_after(jiffies, data->last_updated + HZ) || !data->valid) {
		data->status = i2c_smbus_read_byte_data(client, TMP401_STATUS);
		data->config = i2c_smbus_read_byte_data(client,
						TMP401_CONFIG_READ);
		tmp401_update_device_reg16(client, data);

		data->temp_crit_hyst = i2c_smbus_read_byte_data(client,
						TMP401_TEMP_CRIT_HYST);

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
	struct tmp401_data *data = tmp401_update_device(dev);

	return sprintf(buf, "%d\n",
		tmp401_register_to_temp(data->temp[index], data->config));
}

static ssize_t show_temp_min(struct device *dev,
	struct device_attribute *devattr, char *buf)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);

	return sprintf(buf, "%d\n",
		tmp401_register_to_temp(data->temp_low[index], data->config));
}

static ssize_t show_temp_max(struct device *dev,
	struct device_attribute *devattr, char *buf)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);

	return sprintf(buf, "%d\n",
		tmp401_register_to_temp(data->temp_high[index], data->config));
}

static ssize_t show_temp_crit(struct device *dev,
	struct device_attribute *devattr, char *buf)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);

	return sprintf(buf, "%d\n",
			tmp401_crit_register_to_temp(data->temp_crit[index],
							data->config));
}

static ssize_t show_temp_crit_hyst(struct device *dev,
	struct device_attribute *devattr, char *buf)
{
	int temp, index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);

	mutex_lock(&data->update_lock);
	temp = tmp401_crit_register_to_temp(data->temp_crit[index],
						data->config);
	temp -= data->temp_crit_hyst * 1000;
	mutex_unlock(&data->update_lock);

	return sprintf(buf, "%d\n", temp);
}

static ssize_t show_temp_lowest(struct device *dev,
	struct device_attribute *devattr, char *buf)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);

	return sprintf(buf, "%d\n",
		tmp401_register_to_temp(data->temp_lowest[index],
					data->config));
}

static ssize_t show_temp_highest(struct device *dev,
	struct device_attribute *devattr, char *buf)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);

	return sprintf(buf, "%d\n",
		tmp401_register_to_temp(data->temp_highest[index],
					data->config));
}

static ssize_t show_status(struct device *dev,
	struct device_attribute *devattr, char *buf)
{
	int mask = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);

	if (data->status & mask)
		return sprintf(buf, "1\n");
	else
		return sprintf(buf, "0\n");
}

static ssize_t store_temp_min(struct device *dev, struct device_attribute
	*devattr, const char *buf, size_t count)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);
	long val;
	u16 reg;

	if (kstrtol(buf, 10, &val))
		return -EINVAL;

	reg = tmp401_temp_to_register(val, data->config);

	mutex_lock(&data->update_lock);

	i2c_smbus_write_byte_data(to_i2c_client(dev),
		TMP401_TEMP_LOW_LIMIT_MSB_WRITE[index], reg >> 8);
	i2c_smbus_write_byte_data(to_i2c_client(dev),
		TMP401_TEMP_LOW_LIMIT_LSB[index], reg & 0xFF);

	data->temp_low[index] = reg;

	mutex_unlock(&data->update_lock);

	return count;
}

static ssize_t store_temp_max(struct device *dev, struct device_attribute
	*devattr, const char *buf, size_t count)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);
	long val;
	u16 reg;

	if (kstrtol(buf, 10, &val))
		return -EINVAL;

	reg = tmp401_temp_to_register(val, data->config);

	mutex_lock(&data->update_lock);

	i2c_smbus_write_byte_data(to_i2c_client(dev),
		TMP401_TEMP_HIGH_LIMIT_MSB_WRITE[index], reg >> 8);
	i2c_smbus_write_byte_data(to_i2c_client(dev),
		TMP401_TEMP_HIGH_LIMIT_LSB[index], reg & 0xFF);

	data->temp_high[index] = reg;

	mutex_unlock(&data->update_lock);

	return count;
}

static ssize_t store_temp_crit(struct device *dev, struct device_attribute
	*devattr, const char *buf, size_t count)
{
	int index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);
	long val;
	u8 reg;

	if (kstrtol(buf, 10, &val))
		return -EINVAL;

	reg = tmp401_crit_temp_to_register(val, data->config);

	mutex_lock(&data->update_lock);

	i2c_smbus_write_byte_data(to_i2c_client(dev),
		TMP401_TEMP_CRIT_LIMIT[index], reg);

	data->temp_crit[index] = reg;

	mutex_unlock(&data->update_lock);

	return count;
}

static ssize_t store_temp_crit_hyst(struct device *dev, struct device_attribute
	*devattr, const char *buf, size_t count)
{
	int temp, index = to_sensor_dev_attr(devattr)->index;
	struct tmp401_data *data = tmp401_update_device(dev);
	long val;
	u8 reg;

	if (kstrtol(buf, 10, &val))
		return -EINVAL;

	if (data->config & TMP401_CONFIG_RANGE)
		val = SENSORS_LIMIT(val, -64000, 191000);
	else
		val = SENSORS_LIMIT(val, 0, 127000);

	mutex_lock(&data->update_lock);
	temp = tmp401_crit_register_to_temp(data->temp_crit[index],
						data->config);
	val = SENSORS_LIMIT(val, temp - 255000, temp);
	reg = ((temp - val) + 500) / 1000;

	i2c_smbus_write_byte_data(to_i2c_client(dev),
		TMP401_TEMP_CRIT_HYST, reg);

	data->temp_crit_hyst = reg;

	mutex_unlock(&data->update_lock);

	return count;
}

/*
 * Resets the historical measurements of minimum and maximum temperatures.
 * This is done by writing any value to any of the minimum/maximum registers
 * (0x30-0x37).
 */
static ssize_t reset_temp_history(struct device *dev,
	struct device_attribute	*devattr, const char *buf, size_t count)
{
	long val;

	if (kstrtol(buf, 10, &val))
		return -EINVAL;

	if (val != 1) {
		dev_err(dev, "temp_reset_history value %ld not"
			" supported. Use 1 to reset the history!\n", val);
		return -EINVAL;
	}
	i2c_smbus_write_byte_data(to_i2c_client(dev),
		TMP411_TEMP_LOWEST_MSB[0], val);

	return count;
}

static struct sensor_device_attribute tmp401_attr[] = {
	SENSOR_ATTR(temp1_input, S_IRUGO, show_temp_value, NULL, 0),
	SENSOR_ATTR(temp1_min, S_IWUSR | S_IRUGO, show_temp_min,
		    store_temp_min, 0),
	SENSOR_ATTR(temp1_max, S_IWUSR | S_IRUGO, show_temp_max,
		    store_temp_max, 0),
	SENSOR_ATTR(temp1_crit, S_IWUSR | S_IRUGO, show_temp_crit,
		    store_temp_crit, 0),
	SENSOR_ATTR(temp1_crit_hyst, S_IWUSR | S_IRUGO, show_temp_crit_hyst,
		    store_temp_crit_hyst, 0),
	SENSOR_ATTR(temp1_min_alarm, S_IRUGO, show_status, NULL,
		    TMP401_STATUS_LOCAL_LOW),
	SENSOR_ATTR(temp1_max_alarm, S_IRUGO, show_status, NULL,
		    TMP401_STATUS_LOCAL_HIGH),
	SENSOR_ATTR(temp1_crit_alarm, S_IRUGO, show_status, NULL,
		    TMP401_STATUS_LOCAL_CRIT),
	SENSOR_ATTR(temp2_input, S_IRUGO, show_temp_value, NULL, 1),
	SENSOR_ATTR(temp2_min, S_IWUSR | S_IRUGO, show_temp_min,
		    store_temp_min, 1),
	SENSOR_ATTR(temp2_max, S_IWUSR | S_IRUGO, show_temp_max,
		    store_temp_max, 1),
	SENSOR_ATTR(temp2_crit, S_IWUSR | S_IRUGO, show_temp_crit,
		    store_temp_crit, 1),
	SENSOR_ATTR(temp2_crit_hyst, S_IRUGO, show_temp_crit_hyst, NULL, 1),
	SENSOR_ATTR(temp2_fault, S_IRUGO, show_status, NULL,
		    TMP401_STATUS_REMOTE_OPEN),
	SENSOR_ATTR(temp2_min_alarm, S_IRUGO, show_status, NULL,
		    TMP401_STATUS_REMOTE_LOW),
	SENSOR_ATTR(temp2_max_alarm, S_IRUGO, show_status, NULL,
		    TMP401_STATUS_REMOTE_HIGH),
	SENSOR_ATTR(temp2_crit_alarm, S_IRUGO, show_status, NULL,
		    TMP401_STATUS_REMOTE_CRIT),
};

/*
 * Additional features of the TMP411 chip.
 * The TMP411 stores the minimum and maximum
 * temperature measured since power-on, chip-reset, or
 * minimum and maximum register reset for both the local
 * and remote channels.
 */
static struct sensor_device_attribute tmp411_attr[] = {
	SENSOR_ATTR(temp1_highest, S_IRUGO, show_temp_highest, NULL, 0),
	SENSOR_ATTR(temp1_lowest, S_IRUGO, show_temp_lowest, NULL, 0),
	SENSOR_ATTR(temp2_highest, S_IRUGO, show_temp_highest, NULL, 1),
	SENSOR_ATTR(temp2_lowest, S_IRUGO, show_temp_lowest, NULL, 1),
	SENSOR_ATTR(temp_reset_history, S_IWUSR, NULL, reset_temp_history, 0),
};

/*
 * Begin non sysfs callback code (aka Real code)
 */

static void tmp401_init_client(struct i2c_client *client)
{
	int config, config_orig;

	/* Set the conversion rate to 2 Hz */
	i2c_smbus_write_byte_data(client, TMP401_CONVERSION_RATE_WRITE, 5);

	/* Start conversions (disable shutdown if necessary) */
	config = i2c_smbus_read_byte_data(client, TMP401_CONFIG_READ);
	if (config < 0) {
		dev_warn(&client->dev, "Initialization failed!\n");
		return;
	}
=======
	struct i2c_client *client;
	struct regmap *regmap;
	struct mutex update_lock;
	enum chips kind;

	bool extended_range;

	/* hwmon API configuration data */
	u32 chip_channel_config[4];
	struct hwmon_channel_info chip_info;
	u32 temp_channel_config[4];
	struct hwmon_channel_info temp_info;
	const struct hwmon_channel_info *info[3];
	struct hwmon_chip_info chip;
};

/* regmap */

static bool tmp401_regmap_is_volatile(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case 0:			/* local temp msb */
	case 1:			/* remote temp msb */
	case 2:			/* status */
	case 0x10:		/* remote temp lsb */
	case 0x15:		/* local temp lsb */
	case 0x1b:		/* status (tmp432) */
	case 0x23 ... 0x24:	/* remote temp 2 msb / lsb */
	case 0x30 ... 0x37:	/* lowest/highest temp; status (tmp432) */
		return true;
	default:
		return false;
	}
}

static int tmp401_reg_read(void *context, unsigned int reg, unsigned int *val)
{
	struct tmp401_data *data = context;
	struct i2c_client *client = data->client;
	int regval;

	switch (reg) {
	case 0:			/* local temp msb */
	case 1:			/* remote temp msb */
	case 5:			/* local temp high limit msb */
	case 6:			/* local temp low limit msb */
	case 7:			/* remote temp ligh limit msb */
	case 8:			/* remote temp low limit msb */
	case 0x15:		/* remote temp 2 high limit msb */
	case 0x16:		/* remote temp 2 low limit msb */
	case 0x23:		/* remote temp 2 msb */
	case 0x30:		/* local temp minimum, tmp411 */
	case 0x32:		/* local temp maximum, tmp411 */
	case 0x34:		/* remote temp minimum, tmp411 */
	case 0xf6:		/* remote temp maximum, tmp411 (really 0x36) */
		/* work around register overlap between TMP411 and TMP432 */
		if (reg == 0xf6)
			reg = 0x36;
		regval = i2c_smbus_read_word_swapped(client, reg);
		if (regval < 0)
			return regval;
		*val = regval;
		break;
	case 0x19:		/* critical limits, 8-bit registers */
	case 0x1a:
	case 0x20:
		regval = i2c_smbus_read_byte_data(client, reg);
		if (regval < 0)
			return regval;
		*val = regval << 8;
		break;
	case 0x1b:
	case 0x35 ... 0x37:
		if (data->kind == tmp432) {
			regval = i2c_smbus_read_byte_data(client, reg);
			if (regval < 0)
				return regval;
			*val = regval;
			break;
		}
		/* simulate TMP432 status registers */
		regval = i2c_smbus_read_byte_data(client, TMP401_STATUS);
		if (regval < 0)
			return regval;
		*val = 0;
		switch (reg) {
		case 0x1b:	/* open / fault */
			if (regval & TMP401_STATUS_REMOTE_OPEN)
				*val |= BIT(1);
			break;
		case 0x35:	/* high limit */
			if (regval & TMP401_STATUS_LOCAL_HIGH)
				*val |= BIT(0);
			if (regval & TMP401_STATUS_REMOTE_HIGH)
				*val |= BIT(1);
			break;
		case 0x36:	/* low limit */
			if (regval & TMP401_STATUS_LOCAL_LOW)
				*val |= BIT(0);
			if (regval & TMP401_STATUS_REMOTE_LOW)
				*val |= BIT(1);
			break;
		case 0x37:	/* therm / crit limit */
			if (regval & TMP401_STATUS_LOCAL_CRIT)
				*val |= BIT(0);
			if (regval & TMP401_STATUS_REMOTE_CRIT)
				*val |= BIT(1);
			break;
		}
		break;
	default:
		regval = i2c_smbus_read_byte_data(client, reg);
		if (regval < 0)
			return regval;
		*val = regval;
		break;
	}
	return 0;
}

static int tmp401_reg_write(void *context, unsigned int reg, unsigned int val)
{
	struct tmp401_data *data = context;
	struct i2c_client *client = data->client;

	switch (reg) {
	case 0x05:		/* local temp high limit msb */
	case 0x06:		/* local temp low limit msb */
	case 0x07:		/* remote temp ligh limit msb */
	case 0x08:		/* remote temp low limit msb */
		reg += 6;	/* adjust for register write address */
		fallthrough;
	case 0x15:		/* remote temp 2 high limit msb */
	case 0x16:		/* remote temp 2 low limit msb */
		return i2c_smbus_write_word_swapped(client, reg, val);
	case 0x19:		/* critical limits, 8-bit registers */
	case 0x1a:
	case 0x20:
		return i2c_smbus_write_byte_data(client, reg, val >> 8);
	case TMP401_CONVERSION_RATE:
	case TMP401_CONFIG:
		reg += 6;	/* adjust for register write address */
		fallthrough;
	default:
		return i2c_smbus_write_byte_data(client, reg, val);
	}
}

static const struct regmap_config tmp401_regmap_config = {
	.reg_bits = 8,
	.val_bits = 16,
	.cache_type = REGCACHE_MAPLE,
	.volatile_reg = tmp401_regmap_is_volatile,
	.reg_read = tmp401_reg_read,
	.reg_write = tmp401_reg_write,
};

/* temperature conversion */

static int tmp401_register_to_temp(u16 reg, bool extended)
{
	int temp = reg;

	if (extended)
		temp -= 64 * 256;

	return DIV_ROUND_CLOSEST(temp * 125, 32);
}

static u16 tmp401_temp_to_register(long temp, bool extended, int zbits)
{
	if (extended) {
		temp = clamp_val(temp, -64000, 191000);
		temp += 64000;
	} else {
		temp = clamp_val(temp, 0, 127000);
	}

	return DIV_ROUND_CLOSEST(temp * (1 << (8 - zbits)), 1000) << zbits;
}

/* hwmon API functions */

static const u8 tmp401_temp_reg_index[] = {
	[hwmon_temp_input] = 0,
	[hwmon_temp_min] = 1,
	[hwmon_temp_max] = 2,
	[hwmon_temp_crit] = 3,
	[hwmon_temp_lowest] = 4,
	[hwmon_temp_highest] = 5,
};

static const u8 tmp401_status_reg_index[] = {
	[hwmon_temp_fault] = 0,
	[hwmon_temp_min_alarm] = 1,
	[hwmon_temp_max_alarm] = 2,
	[hwmon_temp_crit_alarm] = 3,
};

static int tmp401_temp_read(struct device *dev, u32 attr, int channel, long *val)
{
	struct tmp401_data *data = dev_get_drvdata(dev);
	struct regmap *regmap = data->regmap;
	unsigned int regval;
	int reg, ret;

	switch (attr) {
	case hwmon_temp_input:
	case hwmon_temp_min:
	case hwmon_temp_max:
	case hwmon_temp_crit:
	case hwmon_temp_lowest:
	case hwmon_temp_highest:
		reg = TMP401_TEMP_MSB[tmp401_temp_reg_index[attr]][channel];
		ret = regmap_read(regmap, reg, &regval);
		if (ret < 0)
			return ret;
		*val = tmp401_register_to_temp(regval, data->extended_range);
		break;
	case hwmon_temp_crit_hyst:
		mutex_lock(&data->update_lock);
		reg = TMP401_TEMP_MSB[3][channel];
		ret = regmap_read(regmap, reg, &regval);
		if (ret < 0)
			goto unlock;
		*val = tmp401_register_to_temp(regval, data->extended_range);
		ret = regmap_read(regmap, TMP401_TEMP_CRIT_HYST, &regval);
		if (ret < 0)
			goto unlock;
		*val -= regval * 1000;
unlock:
		mutex_unlock(&data->update_lock);
		if (ret < 0)
			return ret;
		break;
	case hwmon_temp_fault:
	case hwmon_temp_min_alarm:
	case hwmon_temp_max_alarm:
	case hwmon_temp_crit_alarm:
		reg = TMP432_STATUS_REG[tmp401_status_reg_index[attr]];
		ret = regmap_read(regmap, reg, &regval);
		if (ret < 0)
			return ret;
		*val = !!(regval & BIT(channel));
		break;
	default:
		return -EOPNOTSUPP;
	}
	return 0;
}

static int tmp401_temp_write(struct device *dev, u32 attr, int channel,
			     long val)
{
	struct tmp401_data *data = dev_get_drvdata(dev);
	struct regmap *regmap = data->regmap;
	unsigned int regval;
	int reg, ret, temp;

	mutex_lock(&data->update_lock);
	switch (attr) {
	case hwmon_temp_min:
	case hwmon_temp_max:
	case hwmon_temp_crit:
		reg = TMP401_TEMP_MSB[tmp401_temp_reg_index[attr]][channel];
		regval = tmp401_temp_to_register(val, data->extended_range,
						 attr == hwmon_temp_crit ? 8 : 4);
		ret = regmap_write(regmap, reg, regval);
		break;
	case hwmon_temp_crit_hyst:
		if (data->extended_range)
			val = clamp_val(val, -64000, 191000);
		else
			val = clamp_val(val, 0, 127000);

		reg = TMP401_TEMP_MSB[3][channel];
		ret = regmap_read(regmap, reg, &regval);
		if (ret < 0)
			break;
		temp = tmp401_register_to_temp(regval, data->extended_range);
		val = clamp_val(val, temp - 255000, temp);
		regval = ((temp - val) + 500) / 1000;
		ret = regmap_write(regmap, TMP401_TEMP_CRIT_HYST, regval);
		break;
	default:
		ret = -EOPNOTSUPP;
		break;
	}
	mutex_unlock(&data->update_lock);
	return ret;
}

static int tmp401_chip_read(struct device *dev, u32 attr, int channel, long *val)
{
	struct tmp401_data *data = dev_get_drvdata(dev);
	u32 regval;
	int ret;

	switch (attr) {
	case hwmon_chip_update_interval:
		ret = regmap_read(data->regmap, TMP401_CONVERSION_RATE, &regval);
		if (ret < 0)
			return ret;
		*val = (1 << (7 - regval)) * 125;
		break;
	case hwmon_chip_temp_reset_history:
		*val = 0;
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static int tmp401_set_convrate(struct regmap *regmap, long val)
{
	int rate;

	/*
	 * For valid rates, interval can be calculated as
	 *	interval = (1 << (7 - rate)) * 125;
	 * Rounded rate is therefore
	 *	rate = 7 - __fls(interval * 4 / (125 * 3));
	 * Use clamp_val() to avoid overflows, and to ensure valid input
	 * for __fls.
	 */
	val = clamp_val(val, 125, 16000);
	rate = 7 - __fls(val * 4 / (125 * 3));
	return regmap_write(regmap, TMP401_CONVERSION_RATE, rate);
}

static int tmp401_chip_write(struct device *dev, u32 attr, int channel, long val)
{
	struct tmp401_data *data = dev_get_drvdata(dev);
	struct regmap *regmap = data->regmap;
	int err;

	mutex_lock(&data->update_lock);
	switch (attr) {
	case hwmon_chip_update_interval:
		err = tmp401_set_convrate(regmap, val);
		break;
	case hwmon_chip_temp_reset_history:
		if (val != 1) {
			err = -EINVAL;
			break;
		}
		/*
		 * Reset history by writing any value to any of the
		 * minimum/maximum registers (0x30-0x37).
		 */
		err = regmap_write(regmap, 0x30, 0);
		break;
	default:
		err = -EOPNOTSUPP;
		break;
	}
	mutex_unlock(&data->update_lock);

	return err;
}

static int tmp401_read(struct device *dev, enum hwmon_sensor_types type,
		       u32 attr, int channel, long *val)
{
	switch (type) {
	case hwmon_chip:
		return tmp401_chip_read(dev, attr, channel, val);
	case hwmon_temp:
		return tmp401_temp_read(dev, attr, channel, val);
	default:
		return -EOPNOTSUPP;
	}
}

static int tmp401_write(struct device *dev, enum hwmon_sensor_types type,
			u32 attr, int channel, long val)
{
	switch (type) {
	case hwmon_chip:
		return tmp401_chip_write(dev, attr, channel, val);
	case hwmon_temp:
		return tmp401_temp_write(dev, attr, channel, val);
	default:
		return -EOPNOTSUPP;
	}
}

static umode_t tmp401_is_visible(const void *data, enum hwmon_sensor_types type,
				 u32 attr, int channel)
{
	switch (type) {
	case hwmon_chip:
		switch (attr) {
		case hwmon_chip_update_interval:
		case hwmon_chip_temp_reset_history:
			return 0644;
		default:
			break;
		}
		break;
	case hwmon_temp:
		switch (attr) {
		case hwmon_temp_input:
		case hwmon_temp_min_alarm:
		case hwmon_temp_max_alarm:
		case hwmon_temp_crit_alarm:
		case hwmon_temp_fault:
		case hwmon_temp_lowest:
		case hwmon_temp_highest:
			return 0444;
		case hwmon_temp_min:
		case hwmon_temp_max:
		case hwmon_temp_crit:
		case hwmon_temp_crit_hyst:
			return 0644;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return 0;
}

static const struct hwmon_ops tmp401_ops = {
	.is_visible = tmp401_is_visible,
	.read = tmp401_read,
	.write = tmp401_write,
};

/* chip initialization, detect, probe */

static int tmp401_init_client(struct tmp401_data *data)
{
	struct regmap *regmap = data->regmap;
	u32 config, config_orig;
	int ret;
	u32 val = 0;
	s32 nfactor = 0;

	/* Set conversion rate to 2 Hz */
	ret = regmap_write(regmap, TMP401_CONVERSION_RATE, 5);
	if (ret < 0)
		return ret;

	/* Start conversions (disable shutdown if necessary) */
	ret = regmap_read(regmap, TMP401_CONFIG, &config);
	if (ret < 0)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	config_orig = config;
	config &= ~TMP401_CONFIG_SHUTDOWN;

<<<<<<< HEAD
	if (config != config_orig)
		i2c_smbus_write_byte_data(client, TMP401_CONFIG_WRITE, config);
=======
	if (of_property_read_bool(data->client->dev.of_node, "ti,extended-range-enable")) {
		/* Enable measurement over extended temperature range */
		config |= TMP401_CONFIG_RANGE;
	}

	data->extended_range = !!(config & TMP401_CONFIG_RANGE);

	if (config != config_orig) {
		ret = regmap_write(regmap, TMP401_CONFIG, config);
		if (ret < 0)
			return ret;
	}

	ret = of_property_read_u32(data->client->dev.of_node, "ti,n-factor", &nfactor);
	if (!ret) {
		if (data->kind == tmp401) {
			dev_err(&data->client->dev, "ti,tmp401 does not support n-factor correction\n");
			return -EINVAL;
		}
		if (nfactor < -128 || nfactor > 127) {
			dev_err(&data->client->dev, "n-factor is invalid (%d)\n", nfactor);
			return -EINVAL;
		}
		ret = regmap_write(regmap, TMP4XX_N_FACTOR_REG, (unsigned int)nfactor);
		if (ret < 0)
			return ret;
	}

	ret = of_property_read_u32(data->client->dev.of_node, "ti,beta-compensation", &val);
	if (!ret) {
		if (data->kind == tmp401 || data->kind == tmp411) {
			dev_err(&data->client->dev, "ti,tmp401 or ti,tmp411 does not support beta compensation\n");
			return -EINVAL;
		}
		if (val > 15) {
			dev_err(&data->client->dev, "beta-compensation is invalid (%u)\n", val);
			return -EINVAL;
		}
		ret = regmap_write(regmap, TMP43X_BETA_RANGE, val);
		if (ret < 0)
			return ret;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tmp401_detect(struct i2c_client *client,
			 struct i2c_board_info *info)
{
	enum chips kind;
	struct i2c_adapter *adapter = client->adapter;
	u8 reg;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;

	/* Detect and identify the chip */
	reg = i2c_smbus_read_byte_data(client, TMP401_MANUFACTURER_ID_REG);
	if (reg != TMP401_MANUFACTURER_ID)
		return -ENODEV;

	reg = i2c_smbus_read_byte_data(client, TMP401_DEVICE_ID_REG);

	switch (reg) {
	case TMP401_DEVICE_ID:
<<<<<<< HEAD
		kind = tmp401;
		break;
	case TMP411_DEVICE_ID:
		kind = tmp411;
		break;
=======
		if (client->addr != 0x4c)
			return -ENODEV;
		kind = tmp401;
		break;
	case TMP411A_DEVICE_ID:
		if (client->addr != 0x4c)
			return -ENODEV;
		kind = tmp411;
		break;
	case TMP411B_DEVICE_ID:
		if (client->addr != 0x4d)
			return -ENODEV;
		kind = tmp411;
		break;
	case TMP411C_DEVICE_ID:
		if (client->addr != 0x4e)
			return -ENODEV;
		kind = tmp411;
		break;
	case TMP431_DEVICE_ID:
		if (client->addr != 0x4c && client->addr != 0x4d)
			return -ENODEV;
		kind = tmp431;
		break;
	case TMP432_DEVICE_ID:
		if (client->addr != 0x4c && client->addr != 0x4d)
			return -ENODEV;
		kind = tmp432;
		break;
	case TMP435_DEVICE_ID:
		kind = tmp435;
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -ENODEV;
	}

<<<<<<< HEAD
	reg = i2c_smbus_read_byte_data(client, TMP401_CONFIG_READ);
	if (reg & 0x1b)
		return -ENODEV;

	reg = i2c_smbus_read_byte_data(client, TMP401_CONVERSION_RATE_READ);
=======
	reg = i2c_smbus_read_byte_data(client, TMP401_CONFIG);
	if (reg & 0x1b)
		return -ENODEV;

	reg = i2c_smbus_read_byte_data(client, TMP401_CONVERSION_RATE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Datasheet says: 0x1-0x6 */
	if (reg > 15)
		return -ENODEV;

<<<<<<< HEAD
	strlcpy(info->type, tmp401_id[kind].name, I2C_NAME_SIZE);
=======
	strscpy(info->type, tmp401_id[kind].name, I2C_NAME_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int tmp401_remove(struct i2c_client *client)
{
	struct tmp401_data *data = i2c_get_clientdata(client);
	int i;

	if (data->hwmon_dev)
		hwmon_device_unregister(data->hwmon_dev);

	for (i = 0; i < ARRAY_SIZE(tmp401_attr); i++)
		device_remove_file(&client->dev, &tmp401_attr[i].dev_attr);

	if (data->kind == tmp411) {
		for (i = 0; i < ARRAY_SIZE(tmp411_attr); i++)
			device_remove_file(&client->dev,
					   &tmp411_attr[i].dev_attr);
	}

	kfree(data);
	return 0;
}

static int tmp401_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int i, err = 0;
	struct tmp401_data *data;
	const char *names[] = { "TMP401", "TMP411" };

	data = kzalloc(sizeof(struct tmp401_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);
	data->kind = id->driver_data;

	/* Initialize the TMP401 chip */
	tmp401_init_client(client);

	/* Register sysfs hooks */
	for (i = 0; i < ARRAY_SIZE(tmp401_attr); i++) {
		err = device_create_file(&client->dev,
					 &tmp401_attr[i].dev_attr);
		if (err)
			goto exit_remove;
	}

	/* Register additional tmp411 sysfs hooks */
	if (data->kind == tmp411) {
		for (i = 0; i < ARRAY_SIZE(tmp411_attr); i++) {
			err = device_create_file(&client->dev,
						 &tmp411_attr[i].dev_attr);
			if (err)
				goto exit_remove;
		}
	}

	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		data->hwmon_dev = NULL;
		goto exit_remove;
	}

	dev_info(&client->dev, "Detected TI %s chip\n", names[data->kind]);

	return 0;

exit_remove:
	tmp401_remove(client); /* will also free data for us */
	return err;
}

=======
static int tmp401_probe(struct i2c_client *client)
{
	static const char * const names[] = {
		"TMP401", "TMP411", "TMP431", "TMP432", "TMP435"
	};
	struct device *dev = &client->dev;
	struct hwmon_channel_info *info;
	struct device *hwmon_dev;
	struct tmp401_data *data;
	int status;

	data = devm_kzalloc(dev, sizeof(struct tmp401_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->client = client;
	mutex_init(&data->update_lock);
	data->kind = i2c_match_id(tmp401_id, client)->driver_data;

	data->regmap = devm_regmap_init(dev, NULL, data, &tmp401_regmap_config);
	if (IS_ERR(data->regmap))
		return PTR_ERR(data->regmap);

	/* initialize configuration data */
	data->chip.ops = &tmp401_ops;
	data->chip.info = data->info;

	data->info[0] = &data->chip_info;
	data->info[1] = &data->temp_info;

	info = &data->chip_info;
	info->type = hwmon_chip;
	info->config = data->chip_channel_config;

	data->chip_channel_config[0] = HWMON_C_UPDATE_INTERVAL;

	info = &data->temp_info;
	info->type = hwmon_temp;
	info->config = data->temp_channel_config;

	data->temp_channel_config[0] = HWMON_T_INPUT | HWMON_T_MIN | HWMON_T_MAX |
		HWMON_T_CRIT | HWMON_T_CRIT_HYST | HWMON_T_MIN_ALARM |
		HWMON_T_MAX_ALARM | HWMON_T_CRIT_ALARM;
	data->temp_channel_config[1] = HWMON_T_INPUT | HWMON_T_MIN | HWMON_T_MAX |
		HWMON_T_CRIT | HWMON_T_CRIT_HYST | HWMON_T_MIN_ALARM |
		HWMON_T_MAX_ALARM | HWMON_T_CRIT_ALARM | HWMON_T_FAULT;

	if (data->kind == tmp411) {
		data->temp_channel_config[0] |= HWMON_T_HIGHEST | HWMON_T_LOWEST;
		data->temp_channel_config[1] |= HWMON_T_HIGHEST | HWMON_T_LOWEST;
		data->chip_channel_config[0] |= HWMON_C_TEMP_RESET_HISTORY;
	}

	if (data->kind == tmp432) {
		data->temp_channel_config[2] = HWMON_T_INPUT | HWMON_T_MIN | HWMON_T_MAX |
			HWMON_T_CRIT | HWMON_T_CRIT_HYST | HWMON_T_MIN_ALARM |
			HWMON_T_MAX_ALARM | HWMON_T_CRIT_ALARM | HWMON_T_FAULT;
	}

	/* Initialize the TMP401 chip */
	status = tmp401_init_client(data);
	if (status < 0)
		return status;

	hwmon_dev = devm_hwmon_device_register_with_info(dev, client->name, data,
							 &data->chip, NULL);
	if (IS_ERR(hwmon_dev))
		return PTR_ERR(hwmon_dev);

	dev_info(dev, "Detected TI %s chip\n", names[data->kind]);

	return 0;
}

static const struct of_device_id __maybe_unused tmp4xx_of_match[] = {
	{ .compatible = "ti,tmp401", },
	{ .compatible = "ti,tmp411", },
	{ .compatible = "ti,tmp431", },
	{ .compatible = "ti,tmp432", },
	{ .compatible = "ti,tmp435", },
	{ },
};
MODULE_DEVICE_TABLE(of, tmp4xx_of_match);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct i2c_driver tmp401_driver = {
	.class		= I2C_CLASS_HWMON,
	.driver = {
		.name	= "tmp401",
<<<<<<< HEAD
	},
	.probe		= tmp401_probe,
	.remove		= tmp401_remove,
=======
		.of_match_table = of_match_ptr(tmp4xx_of_match),
	},
	.probe		= tmp401_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table	= tmp401_id,
	.detect		= tmp401_detect,
	.address_list	= normal_i2c,
};

module_i2c_driver(tmp401_driver);

MODULE_AUTHOR("Hans de Goede <hdegoede@redhat.com>");
MODULE_DESCRIPTION("Texas Instruments TMP401 temperature sensor driver");
MODULE_LICENSE("GPL");
