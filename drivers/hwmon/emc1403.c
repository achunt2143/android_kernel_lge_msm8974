<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * emc1403.c - SMSC Thermal Driver
 *
 * Copyright (C) 2008 Intel Corp
 *
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * TODO
 *	-	cache alarm and critical limit registers
 *	-	add emc1404 support
=======
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/sysfs.h>
#include <linux/mutex.h>
<<<<<<< HEAD
=======
#include <linux/regmap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define THERMAL_PID_REG		0xfd
#define THERMAL_SMSC_ID_REG	0xfe
#define THERMAL_REVISION_REG	0xff

<<<<<<< HEAD
struct thermal_data {
	struct device *hwmon_dev;
	struct mutex mutex;
	/*
	 * Cache the hyst value so we don't keep re-reading it. In theory
	 * we could cache it forever as nobody else should be writing it.
	 */
	u8 cached_hyst;
	unsigned long hyst_valid;
};

static ssize_t show_temp(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct sensor_device_attribute *sda = to_sensor_dev_attr(attr);
	int retval = i2c_smbus_read_byte_data(client, sda->index);

	if (retval < 0)
		return retval;
	return sprintf(buf, "%d000\n", retval);
}

static ssize_t show_bit(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct sensor_device_attribute_2 *sda = to_sensor_dev_attr_2(attr);
	int retval = i2c_smbus_read_byte_data(client, sda->nr);

	if (retval < 0)
		return retval;
	retval &= sda->index;
	return sprintf(buf, "%d\n", retval ? 1 : 0);
}

static ssize_t store_temp(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct sensor_device_attribute *sda = to_sensor_dev_attr(attr);
	struct i2c_client *client = to_i2c_client(dev);
=======
enum emc1403_chip { emc1402, emc1403, emc1404 };

struct thermal_data {
	struct regmap *regmap;
	struct mutex mutex;
	const struct attribute_group *groups[4];
};

static ssize_t temp_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
{
	struct sensor_device_attribute *sda = to_sensor_dev_attr(attr);
	struct thermal_data *data = dev_get_drvdata(dev);
	unsigned int val;
	int retval;

	retval = regmap_read(data->regmap, sda->index, &val);
	if (retval < 0)
		return retval;
	return sprintf(buf, "%d000\n", val);
}

static ssize_t bit_show(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	struct sensor_device_attribute_2 *sda = to_sensor_dev_attr_2(attr);
	struct thermal_data *data = dev_get_drvdata(dev);
	unsigned int val;
	int retval;

	retval = regmap_read(data->regmap, sda->nr, &val);
	if (retval < 0)
		return retval;
	return sprintf(buf, "%d\n", !!(val & sda->index));
}

static ssize_t temp_store(struct device *dev, struct device_attribute *attr,
			  const char *buf, size_t count)
{
	struct sensor_device_attribute *sda = to_sensor_dev_attr(attr);
	struct thermal_data *data = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long val;
	int retval;

	if (kstrtoul(buf, 10, &val))
		return -EINVAL;
<<<<<<< HEAD
	retval = i2c_smbus_write_byte_data(client, sda->index,
					DIV_ROUND_CLOSEST(val, 1000));
=======
	retval = regmap_write(data->regmap, sda->index,
			      DIV_ROUND_CLOSEST(val, 1000));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (retval < 0)
		return retval;
	return count;
}

<<<<<<< HEAD
static ssize_t store_bit(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct thermal_data *data = i2c_get_clientdata(client);
	struct sensor_device_attribute_2 *sda = to_sensor_dev_attr_2(attr);
=======
static ssize_t bit_store(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	struct sensor_device_attribute_2 *sda = to_sensor_dev_attr_2(attr);
	struct thermal_data *data = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long val;
	int retval;

	if (kstrtoul(buf, 10, &val))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&data->mutex);
	retval = i2c_smbus_read_byte_data(client, sda->nr);
	if (retval < 0)
		goto fail;

	retval &= ~sda->index;
	if (val)
		retval |= sda->index;

	retval = i2c_smbus_write_byte_data(client, sda->index, retval);
	if (retval == 0)
		retval = count;
fail:
	mutex_unlock(&data->mutex);
	return retval;
}

static ssize_t show_hyst(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct thermal_data *data = i2c_get_clientdata(client);
	struct sensor_device_attribute *sda = to_sensor_dev_attr(attr);
	int retval;
	int hyst;

	retval = i2c_smbus_read_byte_data(client, sda->index);
	if (retval < 0)
		return retval;

	if (time_after(jiffies, data->hyst_valid)) {
		hyst = i2c_smbus_read_byte_data(client, 0x21);
		if (hyst < 0)
			return retval;
		data->cached_hyst = hyst;
		data->hyst_valid = jiffies + HZ;
	}
	return sprintf(buf, "%d000\n", retval - data->cached_hyst);
}

static ssize_t store_hyst(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct thermal_data *data = i2c_get_clientdata(client);
	struct sensor_device_attribute *sda = to_sensor_dev_attr(attr);
=======
	retval = regmap_update_bits(data->regmap, sda->nr, sda->index,
				    val ? sda->index : 0);
	if (retval < 0)
		return retval;
	return count;
}

static ssize_t show_hyst_common(struct device *dev,
				struct device_attribute *attr, char *buf,
				bool is_min)
{
	struct sensor_device_attribute *sda = to_sensor_dev_attr(attr);
	struct thermal_data *data = dev_get_drvdata(dev);
	struct regmap *regmap = data->regmap;
	unsigned int limit;
	unsigned int hyst;
	int retval;

	retval = regmap_read(regmap, sda->index, &limit);
	if (retval < 0)
		return retval;

	retval = regmap_read(regmap, 0x21, &hyst);
	if (retval < 0)
		return retval;

	return sprintf(buf, "%d000\n", is_min ? limit + hyst : limit - hyst);
}

static ssize_t hyst_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
{
	return show_hyst_common(dev, attr, buf, false);
}

static ssize_t min_hyst_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
	return show_hyst_common(dev, attr, buf, true);
}

static ssize_t hyst_store(struct device *dev, struct device_attribute *attr,
			  const char *buf, size_t count)
{
	struct sensor_device_attribute *sda = to_sensor_dev_attr(attr);
	struct thermal_data *data = dev_get_drvdata(dev);
	struct regmap *regmap = data->regmap;
	unsigned int limit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int retval;
	int hyst;
	unsigned long val;

	if (kstrtoul(buf, 10, &val))
		return -EINVAL;

	mutex_lock(&data->mutex);
<<<<<<< HEAD
	retval = i2c_smbus_read_byte_data(client, sda->index);
	if (retval < 0)
		goto fail;

	hyst = retval * 1000 - val;
	hyst = DIV_ROUND_CLOSEST(hyst, 1000);
	if (hyst < 0 || hyst > 255) {
		retval = -ERANGE;
		goto fail;
	}

	retval = i2c_smbus_write_byte_data(client, 0x21, hyst);
	if (retval == 0) {
		retval = count;
		data->cached_hyst = hyst;
		data->hyst_valid = jiffies + HZ;
	}
=======
	retval = regmap_read(regmap, sda->index, &limit);
	if (retval < 0)
		goto fail;

	hyst = limit * 1000 - val;
	hyst = clamp_val(DIV_ROUND_CLOSEST(hyst, 1000), 0, 255);
	retval = regmap_write(regmap, 0x21, hyst);
	if (retval == 0)
		retval = count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
fail:
	mutex_unlock(&data->mutex);
	return retval;
}

/*
 *	Sensors. We pass the actual i2c register to the methods.
 */

<<<<<<< HEAD
static SENSOR_DEVICE_ATTR(temp1_min, S_IRUGO | S_IWUSR,
	show_temp, store_temp, 0x06);
static SENSOR_DEVICE_ATTR(temp1_max, S_IRUGO | S_IWUSR,
	show_temp, store_temp, 0x05);
static SENSOR_DEVICE_ATTR(temp1_crit, S_IRUGO | S_IWUSR,
	show_temp, store_temp, 0x20);
static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO, show_temp, NULL, 0x00);
static SENSOR_DEVICE_ATTR_2(temp1_min_alarm, S_IRUGO,
	show_bit, NULL, 0x36, 0x01);
static SENSOR_DEVICE_ATTR_2(temp1_max_alarm, S_IRUGO,
	show_bit, NULL, 0x35, 0x01);
static SENSOR_DEVICE_ATTR_2(temp1_crit_alarm, S_IRUGO,
	show_bit, NULL, 0x37, 0x01);
static SENSOR_DEVICE_ATTR(temp1_crit_hyst, S_IRUGO | S_IWUSR,
	show_hyst, store_hyst, 0x20);

static SENSOR_DEVICE_ATTR(temp2_min, S_IRUGO | S_IWUSR,
	show_temp, store_temp, 0x08);
static SENSOR_DEVICE_ATTR(temp2_max, S_IRUGO | S_IWUSR,
	show_temp, store_temp, 0x07);
static SENSOR_DEVICE_ATTR(temp2_crit, S_IRUGO | S_IWUSR,
	show_temp, store_temp, 0x19);
static SENSOR_DEVICE_ATTR(temp2_input, S_IRUGO, show_temp, NULL, 0x01);
static SENSOR_DEVICE_ATTR_2(temp2_min_alarm, S_IRUGO,
	show_bit, NULL, 0x36, 0x02);
static SENSOR_DEVICE_ATTR_2(temp2_max_alarm, S_IRUGO,
	show_bit, NULL, 0x35, 0x02);
static SENSOR_DEVICE_ATTR_2(temp2_crit_alarm, S_IRUGO,
	show_bit, NULL, 0x37, 0x02);
static SENSOR_DEVICE_ATTR(temp2_crit_hyst, S_IRUGO | S_IWUSR,
	show_hyst, store_hyst, 0x19);

static SENSOR_DEVICE_ATTR(temp3_min, S_IRUGO | S_IWUSR,
	show_temp, store_temp, 0x16);
static SENSOR_DEVICE_ATTR(temp3_max, S_IRUGO | S_IWUSR,
	show_temp, store_temp, 0x15);
static SENSOR_DEVICE_ATTR(temp3_crit, S_IRUGO | S_IWUSR,
	show_temp, store_temp, 0x1A);
static SENSOR_DEVICE_ATTR(temp3_input, S_IRUGO, show_temp, NULL, 0x23);
static SENSOR_DEVICE_ATTR_2(temp3_min_alarm, S_IRUGO,
	show_bit, NULL, 0x36, 0x04);
static SENSOR_DEVICE_ATTR_2(temp3_max_alarm, S_IRUGO,
	show_bit, NULL, 0x35, 0x04);
static SENSOR_DEVICE_ATTR_2(temp3_crit_alarm, S_IRUGO,
	show_bit, NULL, 0x37, 0x04);
static SENSOR_DEVICE_ATTR(temp3_crit_hyst, S_IRUGO | S_IWUSR,
	show_hyst, store_hyst, 0x1A);

static SENSOR_DEVICE_ATTR_2(power_state, S_IRUGO | S_IWUSR,
	show_bit, store_bit, 0x03, 0x40);

static struct attribute *mid_att_thermal[] = {
=======
static SENSOR_DEVICE_ATTR_RW(temp1_min, temp, 0x06);
static SENSOR_DEVICE_ATTR_RW(temp1_max, temp, 0x05);
static SENSOR_DEVICE_ATTR_RW(temp1_crit, temp, 0x20);
static SENSOR_DEVICE_ATTR_RO(temp1_input, temp, 0x00);
static SENSOR_DEVICE_ATTR_2_RO(temp1_min_alarm, bit, 0x36, 0x01);
static SENSOR_DEVICE_ATTR_2_RO(temp1_max_alarm, bit, 0x35, 0x01);
static SENSOR_DEVICE_ATTR_2_RO(temp1_crit_alarm, bit, 0x37, 0x01);
static SENSOR_DEVICE_ATTR_RO(temp1_min_hyst, min_hyst, 0x06);
static SENSOR_DEVICE_ATTR_RO(temp1_max_hyst, hyst, 0x05);
static SENSOR_DEVICE_ATTR_RW(temp1_crit_hyst, hyst, 0x20);

static SENSOR_DEVICE_ATTR_RW(temp2_min, temp, 0x08);
static SENSOR_DEVICE_ATTR_RW(temp2_max, temp, 0x07);
static SENSOR_DEVICE_ATTR_RW(temp2_crit, temp, 0x19);
static SENSOR_DEVICE_ATTR_RO(temp2_input, temp, 0x01);
static SENSOR_DEVICE_ATTR_2_RO(temp2_fault, bit, 0x1b, 0x02);
static SENSOR_DEVICE_ATTR_2_RO(temp2_min_alarm, bit, 0x36, 0x02);
static SENSOR_DEVICE_ATTR_2_RO(temp2_max_alarm, bit, 0x35, 0x02);
static SENSOR_DEVICE_ATTR_2_RO(temp2_crit_alarm, bit, 0x37, 0x02);
static SENSOR_DEVICE_ATTR_RO(temp2_min_hyst, min_hyst, 0x08);
static SENSOR_DEVICE_ATTR_RO(temp2_max_hyst, hyst, 0x07);
static SENSOR_DEVICE_ATTR_RO(temp2_crit_hyst, hyst, 0x19);

static SENSOR_DEVICE_ATTR_RW(temp3_min, temp, 0x16);
static SENSOR_DEVICE_ATTR_RW(temp3_max, temp, 0x15);
static SENSOR_DEVICE_ATTR_RW(temp3_crit, temp, 0x1A);
static SENSOR_DEVICE_ATTR_RO(temp3_input, temp, 0x23);
static SENSOR_DEVICE_ATTR_2_RO(temp3_fault, bit, 0x1b, 0x04);
static SENSOR_DEVICE_ATTR_2_RO(temp3_min_alarm, bit, 0x36, 0x04);
static SENSOR_DEVICE_ATTR_2_RO(temp3_max_alarm, bit, 0x35, 0x04);
static SENSOR_DEVICE_ATTR_2_RO(temp3_crit_alarm, bit, 0x37, 0x04);
static SENSOR_DEVICE_ATTR_RO(temp3_min_hyst, min_hyst, 0x16);
static SENSOR_DEVICE_ATTR_RO(temp3_max_hyst, hyst, 0x15);
static SENSOR_DEVICE_ATTR_RO(temp3_crit_hyst, hyst, 0x1A);

static SENSOR_DEVICE_ATTR_RW(temp4_min, temp, 0x2D);
static SENSOR_DEVICE_ATTR_RW(temp4_max, temp, 0x2C);
static SENSOR_DEVICE_ATTR_RW(temp4_crit, temp, 0x30);
static SENSOR_DEVICE_ATTR_RO(temp4_input, temp, 0x2A);
static SENSOR_DEVICE_ATTR_2_RO(temp4_fault, bit, 0x1b, 0x08);
static SENSOR_DEVICE_ATTR_2_RO(temp4_min_alarm, bit, 0x36, 0x08);
static SENSOR_DEVICE_ATTR_2_RO(temp4_max_alarm, bit, 0x35, 0x08);
static SENSOR_DEVICE_ATTR_2_RO(temp4_crit_alarm, bit, 0x37, 0x08);
static SENSOR_DEVICE_ATTR_RO(temp4_min_hyst, min_hyst, 0x2D);
static SENSOR_DEVICE_ATTR_RO(temp4_max_hyst, hyst, 0x2C);
static SENSOR_DEVICE_ATTR_RO(temp4_crit_hyst, hyst, 0x30);

static SENSOR_DEVICE_ATTR_2_RW(power_state, bit, 0x03, 0x40);

static struct attribute *emc1402_attrs[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&sensor_dev_attr_temp1_min.dev_attr.attr,
	&sensor_dev_attr_temp1_max.dev_attr.attr,
	&sensor_dev_attr_temp1_crit.dev_attr.attr,
	&sensor_dev_attr_temp1_input.dev_attr.attr,
<<<<<<< HEAD
	&sensor_dev_attr_temp1_min_alarm.dev_attr.attr,
	&sensor_dev_attr_temp1_max_alarm.dev_attr.attr,
	&sensor_dev_attr_temp1_crit_alarm.dev_attr.attr,
	&sensor_dev_attr_temp1_crit_hyst.dev_attr.attr,
=======
	&sensor_dev_attr_temp1_min_hyst.dev_attr.attr,
	&sensor_dev_attr_temp1_max_hyst.dev_attr.attr,
	&sensor_dev_attr_temp1_crit_hyst.dev_attr.attr,

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&sensor_dev_attr_temp2_min.dev_attr.attr,
	&sensor_dev_attr_temp2_max.dev_attr.attr,
	&sensor_dev_attr_temp2_crit.dev_attr.attr,
	&sensor_dev_attr_temp2_input.dev_attr.attr,
<<<<<<< HEAD
	&sensor_dev_attr_temp2_min_alarm.dev_attr.attr,
	&sensor_dev_attr_temp2_max_alarm.dev_attr.attr,
	&sensor_dev_attr_temp2_crit_alarm.dev_attr.attr,
	&sensor_dev_attr_temp2_crit_hyst.dev_attr.attr,
	&sensor_dev_attr_temp3_min.dev_attr.attr,
	&sensor_dev_attr_temp3_max.dev_attr.attr,
	&sensor_dev_attr_temp3_crit.dev_attr.attr,
	&sensor_dev_attr_temp3_input.dev_attr.attr,
	&sensor_dev_attr_temp3_min_alarm.dev_attr.attr,
	&sensor_dev_attr_temp3_max_alarm.dev_attr.attr,
	&sensor_dev_attr_temp3_crit_alarm.dev_attr.attr,
	&sensor_dev_attr_temp3_crit_hyst.dev_attr.attr,
=======
	&sensor_dev_attr_temp2_min_hyst.dev_attr.attr,
	&sensor_dev_attr_temp2_max_hyst.dev_attr.attr,
	&sensor_dev_attr_temp2_crit_hyst.dev_attr.attr,

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&sensor_dev_attr_power_state.dev_attr.attr,
	NULL
};

<<<<<<< HEAD
static const struct attribute_group m_thermal_gr = {
	.attrs = mid_att_thermal
=======
static const struct attribute_group emc1402_group = {
		.attrs = emc1402_attrs,
};

static struct attribute *emc1403_attrs[] = {
	&sensor_dev_attr_temp1_min_alarm.dev_attr.attr,
	&sensor_dev_attr_temp1_max_alarm.dev_attr.attr,
	&sensor_dev_attr_temp1_crit_alarm.dev_attr.attr,

	&sensor_dev_attr_temp2_fault.dev_attr.attr,
	&sensor_dev_attr_temp2_min_alarm.dev_attr.attr,
	&sensor_dev_attr_temp2_max_alarm.dev_attr.attr,
	&sensor_dev_attr_temp2_crit_alarm.dev_attr.attr,

	&sensor_dev_attr_temp3_min.dev_attr.attr,
	&sensor_dev_attr_temp3_max.dev_attr.attr,
	&sensor_dev_attr_temp3_crit.dev_attr.attr,
	&sensor_dev_attr_temp3_input.dev_attr.attr,
	&sensor_dev_attr_temp3_fault.dev_attr.attr,
	&sensor_dev_attr_temp3_min_alarm.dev_attr.attr,
	&sensor_dev_attr_temp3_max_alarm.dev_attr.attr,
	&sensor_dev_attr_temp3_crit_alarm.dev_attr.attr,
	&sensor_dev_attr_temp3_min_hyst.dev_attr.attr,
	&sensor_dev_attr_temp3_max_hyst.dev_attr.attr,
	&sensor_dev_attr_temp3_crit_hyst.dev_attr.attr,
	NULL
};

static const struct attribute_group emc1403_group = {
	.attrs = emc1403_attrs,
};

static struct attribute *emc1404_attrs[] = {
	&sensor_dev_attr_temp4_min.dev_attr.attr,
	&sensor_dev_attr_temp4_max.dev_attr.attr,
	&sensor_dev_attr_temp4_crit.dev_attr.attr,
	&sensor_dev_attr_temp4_input.dev_attr.attr,
	&sensor_dev_attr_temp4_fault.dev_attr.attr,
	&sensor_dev_attr_temp4_min_alarm.dev_attr.attr,
	&sensor_dev_attr_temp4_max_alarm.dev_attr.attr,
	&sensor_dev_attr_temp4_crit_alarm.dev_attr.attr,
	&sensor_dev_attr_temp4_min_hyst.dev_attr.attr,
	&sensor_dev_attr_temp4_max_hyst.dev_attr.attr,
	&sensor_dev_attr_temp4_crit_hyst.dev_attr.attr,
	NULL
};

static const struct attribute_group emc1404_group = {
	.attrs = emc1404_attrs,
};

/*
 * EMC14x2 uses a different register and different bits to report alarm and
 * fault status. For simplicity, provide a separate attribute group for this
 * chip series.
 * Since we can not re-use the same attribute names, create a separate attribute
 * array.
 */
static struct sensor_device_attribute_2 emc1402_alarms[] = {
	SENSOR_ATTR_2_RO(temp1_min_alarm, bit, 0x02, 0x20),
	SENSOR_ATTR_2_RO(temp1_max_alarm, bit, 0x02, 0x40),
	SENSOR_ATTR_2_RO(temp1_crit_alarm, bit, 0x02, 0x01),

	SENSOR_ATTR_2_RO(temp2_fault, bit, 0x02, 0x04),
	SENSOR_ATTR_2_RO(temp2_min_alarm, bit, 0x02, 0x08),
	SENSOR_ATTR_2_RO(temp2_max_alarm, bit, 0x02, 0x10),
	SENSOR_ATTR_2_RO(temp2_crit_alarm, bit, 0x02, 0x02),
};

static struct attribute *emc1402_alarm_attrs[] = {
	&emc1402_alarms[0].dev_attr.attr,
	&emc1402_alarms[1].dev_attr.attr,
	&emc1402_alarms[2].dev_attr.attr,
	&emc1402_alarms[3].dev_attr.attr,
	&emc1402_alarms[4].dev_attr.attr,
	&emc1402_alarms[5].dev_attr.attr,
	&emc1402_alarms[6].dev_attr.attr,
	NULL,
};

static const struct attribute_group emc1402_alarm_group = {
	.attrs = emc1402_alarm_attrs,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int emc1403_detect(struct i2c_client *client,
			struct i2c_board_info *info)
{
	int id;
	/* Check if thermal chip is SMSC and EMC1403 or EMC1423 */

	id = i2c_smbus_read_byte_data(client, THERMAL_SMSC_ID_REG);
	if (id != 0x5d)
		return -ENODEV;

	id = i2c_smbus_read_byte_data(client, THERMAL_PID_REG);
	switch (id) {
<<<<<<< HEAD
	case 0x21:
		strlcpy(info->type, "emc1403", I2C_NAME_SIZE);
		break;
	case 0x23:
		strlcpy(info->type, "emc1423", I2C_NAME_SIZE);
		break;
	/*
	 * Note: 0x25 is the 1404 which is very similar and this
	 * driver could be extended
	 */
=======
	case 0x20:
		strscpy(info->type, "emc1402", I2C_NAME_SIZE);
		break;
	case 0x21:
		strscpy(info->type, "emc1403", I2C_NAME_SIZE);
		break;
	case 0x22:
		strscpy(info->type, "emc1422", I2C_NAME_SIZE);
		break;
	case 0x23:
		strscpy(info->type, "emc1423", I2C_NAME_SIZE);
		break;
	case 0x25:
		strscpy(info->type, "emc1404", I2C_NAME_SIZE);
		break;
	case 0x27:
		strscpy(info->type, "emc1424", I2C_NAME_SIZE);
		break;
	case 0x60:
		strscpy(info->type, "emc1442", I2C_NAME_SIZE);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -ENODEV;
	}

	id = i2c_smbus_read_byte_data(client, THERMAL_REVISION_REG);
	if (id < 0x01 || id > 0x04)
		return -ENODEV;

	return 0;
}

<<<<<<< HEAD
static int emc1403_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int res;
	struct thermal_data *data;

	data = kzalloc(sizeof(struct thermal_data), GFP_KERNEL);
	if (data == NULL) {
		dev_warn(&client->dev, "out of memory");
		return -ENOMEM;
	}

	i2c_set_clientdata(client, data);
	mutex_init(&data->mutex);
	data->hyst_valid = jiffies - 1;		/* Expired */

	res = sysfs_create_group(&client->dev.kobj, &m_thermal_gr);
	if (res) {
		dev_warn(&client->dev, "create group failed\n");
		goto thermal_error1;
	}
	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		res = PTR_ERR(data->hwmon_dev);
		dev_warn(&client->dev, "register hwmon dev failed\n");
		goto thermal_error2;
	}
	dev_info(&client->dev, "EMC1403 Thermal chip found\n");
	return res;

thermal_error2:
	sysfs_remove_group(&client->dev.kobj, &m_thermal_gr);
thermal_error1:
	kfree(data);
	return res;
}

static int emc1403_remove(struct i2c_client *client)
{
	struct thermal_data *data = i2c_get_clientdata(client);

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &m_thermal_gr);
	kfree(data);
=======
static bool emc1403_regmap_is_volatile(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case 0x00:	/* internal diode high byte */
	case 0x01:	/* external diode 1 high byte */
	case 0x02:	/* status */
	case 0x10:	/* external diode 1 low byte */
	case 0x1b:	/* external diode fault */
	case 0x23:	/* external diode 2 high byte */
	case 0x24:	/* external diode 2 low byte */
	case 0x29:	/* internal diode low byte */
	case 0x2a:	/* externl diode 3 high byte */
	case 0x2b:	/* external diode 3 low byte */
	case 0x35:	/* high limit status */
	case 0x36:	/* low limit status */
	case 0x37:	/* therm limit status */
		return true;
	default:
		return false;
	}
}

static const struct regmap_config emc1403_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.cache_type = REGCACHE_MAPLE,
	.volatile_reg = emc1403_regmap_is_volatile,
};

static const struct i2c_device_id emc1403_idtable[];

static int emc1403_probe(struct i2c_client *client)
{
	struct thermal_data *data;
	struct device *hwmon_dev;
	const struct i2c_device_id *id = i2c_match_id(emc1403_idtable, client);

	data = devm_kzalloc(&client->dev, sizeof(struct thermal_data),
			    GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;

	data->regmap = devm_regmap_init_i2c(client, &emc1403_regmap_config);
	if (IS_ERR(data->regmap))
		return PTR_ERR(data->regmap);

	mutex_init(&data->mutex);

	switch (id->driver_data) {
	case emc1404:
		data->groups[2] = &emc1404_group;
		fallthrough;
	case emc1403:
		data->groups[1] = &emc1403_group;
		fallthrough;
	case emc1402:
		data->groups[0] = &emc1402_group;
	}

	if (id->driver_data == emc1402)
		data->groups[1] = &emc1402_alarm_group;

	hwmon_dev = devm_hwmon_device_register_with_groups(&client->dev,
							   client->name, data,
							   data->groups);
	if (IS_ERR(hwmon_dev))
		return PTR_ERR(hwmon_dev);

	dev_info(&client->dev, "%s Thermal chip found\n", id->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const unsigned short emc1403_address_list[] = {
<<<<<<< HEAD
	0x18, 0x29, 0x4c, 0x4d, I2C_CLIENT_END
};

static const struct i2c_device_id emc1403_idtable[] = {
	{ "emc1403", 0 },
	{ "emc1423", 0 },
=======
	0x18, 0x1c, 0x29, 0x3c, 0x4c, 0x4d, 0x5c, I2C_CLIENT_END
};

/* Last digit of chip name indicates number of channels */
static const struct i2c_device_id emc1403_idtable[] = {
	{ "emc1402", emc1402 },
	{ "emc1403", emc1403 },
	{ "emc1404", emc1404 },
	{ "emc1412", emc1402 },
	{ "emc1413", emc1403 },
	{ "emc1414", emc1404 },
	{ "emc1422", emc1402 },
	{ "emc1423", emc1403 },
	{ "emc1424", emc1404 },
	{ "emc1442", emc1402 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ }
};
MODULE_DEVICE_TABLE(i2c, emc1403_idtable);

static struct i2c_driver sensor_emc1403 = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		.name = "emc1403",
	},
	.detect = emc1403_detect,
	.probe = emc1403_probe,
<<<<<<< HEAD
	.remove = emc1403_remove,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table = emc1403_idtable,
	.address_list = emc1403_address_list,
};

module_i2c_driver(sensor_emc1403);

MODULE_AUTHOR("Kalhan Trisal <kalhan.trisal@intel.com");
MODULE_DESCRIPTION("emc1403 Thermal Driver");
MODULE_LICENSE("GPL v2");
