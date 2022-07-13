<<<<<<< HEAD
/***************************************************************************
 *   Copyright (C) 2010-2012 Hans de Goede <hdegoede@redhat.com>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/***************************************************************************
 *   Copyright (C) 2010-2012 Hans de Goede <hdegoede@redhat.com>           *
 *                                                                         *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 ***************************************************************************/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/init.h>
=======
#include <linux/bits.h>
#include <linux/minmax.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/pm.h>
#include <linux/init.h>
#include <linux/regmap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/platform_device.h>
#include <linux/hwmon.h>
<<<<<<< HEAD
#include <linux/hwmon-sysfs.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/err.h>
#include <linux/mutex.h>
#include "sch56xx-common.h"

#define DRVNAME "sch5627"
#define DEVNAME DRVNAME /* We only support one model */

#define SCH5627_HWMON_ID		0xa5
#define SCH5627_COMPANY_ID		0x5c
#define SCH5627_PRIMARY_ID		0xa0

#define SCH5627_REG_BUILD_CODE		0x39
#define SCH5627_REG_BUILD_ID		0x3a
#define SCH5627_REG_HWMON_ID		0x3c
#define SCH5627_REG_HWMON_REV		0x3d
#define SCH5627_REG_COMPANY_ID		0x3e
#define SCH5627_REG_PRIMARY_ID		0x3f
#define SCH5627_REG_CTRL		0x40

<<<<<<< HEAD
=======
#define SCH5627_CTRL_START		BIT(0)
#define SCH5627_CTRL_LOCK		BIT(1)
#define SCH5627_CTRL_VBAT		BIT(4)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SCH5627_NO_TEMPS		8
#define SCH5627_NO_FANS			4
#define SCH5627_NO_IN			5

static const u16 SCH5627_REG_TEMP_MSB[SCH5627_NO_TEMPS] = {
	0x2B, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x180, 0x181 };
static const u16 SCH5627_REG_TEMP_LSN[SCH5627_NO_TEMPS] = {
	0xE2, 0xE1, 0xE1, 0xE5, 0xE5, 0xE6, 0x182, 0x182 };
static const u16 SCH5627_REG_TEMP_HIGH_NIBBLE[SCH5627_NO_TEMPS] = {
	0, 0, 1, 1, 0, 0, 0, 1 };
static const u16 SCH5627_REG_TEMP_HIGH[SCH5627_NO_TEMPS] = {
	0x61, 0x57, 0x59, 0x5B, 0x5D, 0x5F, 0x184, 0x186 };
static const u16 SCH5627_REG_TEMP_ABS[SCH5627_NO_TEMPS] = {
	0x9B, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x1A8, 0x1A9 };

static const u16 SCH5627_REG_FAN[SCH5627_NO_FANS] = {
	0x2C, 0x2E, 0x30, 0x32 };
static const u16 SCH5627_REG_FAN_MIN[SCH5627_NO_FANS] = {
	0x62, 0x64, 0x66, 0x68 };

<<<<<<< HEAD
=======
static const u16 SCH5627_REG_PWM_MAP[SCH5627_NO_FANS] = {
	0xA0, 0xA1, 0xA2, 0xA3 };

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const u16 SCH5627_REG_IN_MSB[SCH5627_NO_IN] = {
	0x22, 0x23, 0x24, 0x25, 0x189 };
static const u16 SCH5627_REG_IN_LSN[SCH5627_NO_IN] = {
	0xE4, 0xE4, 0xE3, 0xE3, 0x18A };
static const u16 SCH5627_REG_IN_HIGH_NIBBLE[SCH5627_NO_IN] = {
	1, 0, 1, 0, 1 };
static const u16 SCH5627_REG_IN_FACTOR[SCH5627_NO_IN] = {
	10745, 3660, 9765, 10745, 3660 };
static const char * const SCH5627_IN_LABELS[SCH5627_NO_IN] = {
	"VCC", "VTT", "VBAT", "VTR", "V_IN" };

struct sch5627_data {
<<<<<<< HEAD
	unsigned short addr;
	struct device *hwmon_dev;
	struct sch56xx_watchdog_data *watchdog;
	u8 control;
	u8 temp_max[SCH5627_NO_TEMPS];
	u8 temp_crit[SCH5627_NO_TEMPS];
	u16 fan_min[SCH5627_NO_FANS];

	struct mutex update_lock;
	unsigned long last_battery;	/* In jiffies */
	char valid;			/* !=0 if following fields are valid */
	unsigned long last_updated;	/* In jiffies */
=======
	struct regmap *regmap;
	unsigned short addr;
	u8 control;

	struct mutex update_lock;
	unsigned long last_battery;	/* In jiffies */
	char temp_valid;		/* !=0 if following fields are valid */
	char fan_valid;
	char in_valid;
	unsigned long temp_last_updated;	/* In jiffies */
	unsigned long fan_last_updated;
	unsigned long in_last_updated;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 temp[SCH5627_NO_TEMPS];
	u16 fan[SCH5627_NO_FANS];
	u16 in[SCH5627_NO_IN];
};

<<<<<<< HEAD
static struct sch5627_data *sch5627_update_device(struct device *dev)
{
	struct sch5627_data *data = dev_get_drvdata(dev);
	struct sch5627_data *ret = data;
=======
static const struct regmap_range sch5627_tunables_ranges[] = {
	regmap_reg_range(0x57, 0x57),
	regmap_reg_range(0x59, 0x59),
	regmap_reg_range(0x5B, 0x5B),
	regmap_reg_range(0x5D, 0x5D),
	regmap_reg_range(0x5F, 0x5F),
	regmap_reg_range(0x61, 0x69),
	regmap_reg_range(0x96, 0x9B),
	regmap_reg_range(0xA0, 0xA3),
	regmap_reg_range(0x184, 0x184),
	regmap_reg_range(0x186, 0x186),
	regmap_reg_range(0x1A8, 0x1A9),
};

static const struct regmap_access_table sch5627_tunables_table = {
	.yes_ranges = sch5627_tunables_ranges,
	.n_yes_ranges = ARRAY_SIZE(sch5627_tunables_ranges),
};

static const struct regmap_config sch5627_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.wr_table = &sch5627_tunables_table,
	.rd_table = &sch5627_tunables_table,
	.cache_type = REGCACHE_MAPLE,
	.use_single_read = true,
	.use_single_write = true,
	.can_sleep = true,
};

static int sch5627_update_temp(struct sch5627_data *data)
{
	int ret = 0;
	int i, val;

	mutex_lock(&data->update_lock);

	/* Cache the values for 1 second */
	if (time_after(jiffies, data->temp_last_updated + HZ) || !data->temp_valid) {
		for (i = 0; i < SCH5627_NO_TEMPS; i++) {
			val = sch56xx_read_virtual_reg12(data->addr, SCH5627_REG_TEMP_MSB[i],
							 SCH5627_REG_TEMP_LSN[i],
							 SCH5627_REG_TEMP_HIGH_NIBBLE[i]);
			if (unlikely(val < 0)) {
				ret = val;
				goto abort;
			}
			data->temp[i] = val;
		}
		data->temp_last_updated = jiffies;
		data->temp_valid = 1;
	}
abort:
	mutex_unlock(&data->update_lock);
	return ret;
}

static int sch5627_update_fan(struct sch5627_data *data)
{
	int ret = 0;
	int i, val;

	mutex_lock(&data->update_lock);

	/* Cache the values for 1 second */
	if (time_after(jiffies, data->fan_last_updated + HZ) || !data->fan_valid) {
		for (i = 0; i < SCH5627_NO_FANS; i++) {
			val = sch56xx_read_virtual_reg16(data->addr, SCH5627_REG_FAN[i]);
			if (unlikely(val < 0)) {
				ret = val;
				goto abort;
			}
			data->fan[i] = val;
		}
		data->fan_last_updated = jiffies;
		data->fan_valid = 1;
	}
abort:
	mutex_unlock(&data->update_lock);
	return ret;
}

static int sch5627_update_in(struct sch5627_data *data)
{
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i, val;

	mutex_lock(&data->update_lock);

	/* Trigger a Vbat voltage measurement every 5 minutes */
	if (time_after(jiffies, data->last_battery + 300 * HZ)) {
		sch56xx_write_virtual_reg(data->addr, SCH5627_REG_CTRL,
<<<<<<< HEAD
					  data->control | 0x10);
=======
					  data->control | SCH5627_CTRL_VBAT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		data->last_battery = jiffies;
	}

	/* Cache the values for 1 second */
<<<<<<< HEAD
	if (time_after(jiffies, data->last_updated + HZ) || !data->valid) {
		for (i = 0; i < SCH5627_NO_TEMPS; i++) {
			val = sch56xx_read_virtual_reg12(data->addr,
				SCH5627_REG_TEMP_MSB[i],
				SCH5627_REG_TEMP_LSN[i],
				SCH5627_REG_TEMP_HIGH_NIBBLE[i]);
			if (unlikely(val < 0)) {
				ret = ERR_PTR(val);
				goto abort;
			}
			data->temp[i] = val;
		}

		for (i = 0; i < SCH5627_NO_FANS; i++) {
			val = sch56xx_read_virtual_reg16(data->addr,
							 SCH5627_REG_FAN[i]);
			if (unlikely(val < 0)) {
				ret = ERR_PTR(val);
				goto abort;
			}
			data->fan[i] = val;
		}

		for (i = 0; i < SCH5627_NO_IN; i++) {
			val = sch56xx_read_virtual_reg12(data->addr,
				SCH5627_REG_IN_MSB[i],
				SCH5627_REG_IN_LSN[i],
				SCH5627_REG_IN_HIGH_NIBBLE[i]);
			if (unlikely(val < 0)) {
				ret = ERR_PTR(val);
=======
	if (time_after(jiffies, data->in_last_updated + HZ) || !data->in_valid) {
		for (i = 0; i < SCH5627_NO_IN; i++) {
			val = sch56xx_read_virtual_reg12(data->addr, SCH5627_REG_IN_MSB[i],
							 SCH5627_REG_IN_LSN[i],
							 SCH5627_REG_IN_HIGH_NIBBLE[i]);
			if (unlikely(val < 0)) {
				ret = val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto abort;
			}
			data->in[i] = val;
		}
<<<<<<< HEAD

		data->last_updated = jiffies;
		data->valid = 1;
=======
		data->in_last_updated = jiffies;
		data->in_valid = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
abort:
	mutex_unlock(&data->update_lock);
	return ret;
}

<<<<<<< HEAD
static int __devinit sch5627_read_limits(struct sch5627_data *data)
{
	int i, val;

	for (i = 0; i < SCH5627_NO_TEMPS; i++) {
		/*
		 * Note what SMSC calls ABS, is what lm_sensors calls max
		 * (aka high), and HIGH is what lm_sensors calls crit.
		 */
		val = sch56xx_read_virtual_reg(data->addr,
					       SCH5627_REG_TEMP_ABS[i]);
		if (val < 0)
			return val;
		data->temp_max[i] = val;

		val = sch56xx_read_virtual_reg(data->addr,
					       SCH5627_REG_TEMP_HIGH[i]);
		if (val < 0)
			return val;
		data->temp_crit[i] = val;
	}
	for (i = 0; i < SCH5627_NO_FANS; i++) {
		val = sch56xx_read_virtual_reg16(data->addr,
						 SCH5627_REG_FAN_MIN[i]);
		if (val < 0)
			return val;
		data->fan_min[i] = val;
	}

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int reg_to_temp(u16 reg)
{
	return (reg * 625) / 10 - 64000;
}

static int reg_to_temp_limit(u8 reg)
{
	return (reg - 64) * 1000;
}

static int reg_to_rpm(u16 reg)
{
	if (reg == 0)
		return -EIO;
	if (reg == 0xffff)
		return 0;

	return 5400540 / reg;
}

<<<<<<< HEAD
static ssize_t show_name(struct device *dev, struct device_attribute *devattr,
	char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", DEVNAME);
}

static ssize_t show_temp(struct device *dev, struct device_attribute
	*devattr, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	struct sch5627_data *data = sch5627_update_device(dev);
	int val;

	if (IS_ERR(data))
		return PTR_ERR(data);

	val = reg_to_temp(data->temp[attr->index]);
	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t show_temp_fault(struct device *dev, struct device_attribute
	*devattr, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	struct sch5627_data *data = sch5627_update_device(dev);

	if (IS_ERR(data))
		return PTR_ERR(data);

	return snprintf(buf, PAGE_SIZE, "%d\n", data->temp[attr->index] == 0);
}

static ssize_t show_temp_max(struct device *dev, struct device_attribute
	*devattr, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	struct sch5627_data *data = dev_get_drvdata(dev);
	int val;

	val = reg_to_temp_limit(data->temp_max[attr->index]);
	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t show_temp_crit(struct device *dev, struct device_attribute
	*devattr, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	struct sch5627_data *data = dev_get_drvdata(dev);
	int val;

	val = reg_to_temp_limit(data->temp_crit[attr->index]);
	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t show_fan(struct device *dev, struct device_attribute
	*devattr, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	struct sch5627_data *data = sch5627_update_device(dev);
	int val;

	if (IS_ERR(data))
		return PTR_ERR(data);

	val = reg_to_rpm(data->fan[attr->index]);
	if (val < 0)
		return val;

	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t show_fan_fault(struct device *dev, struct device_attribute
	*devattr, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	struct sch5627_data *data = sch5627_update_device(dev);

	if (IS_ERR(data))
		return PTR_ERR(data);

	return snprintf(buf, PAGE_SIZE, "%d\n",
			data->fan[attr->index] == 0xffff);
}

static ssize_t show_fan_min(struct device *dev, struct device_attribute
	*devattr, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	struct sch5627_data *data = dev_get_drvdata(dev);
	int val = reg_to_rpm(data->fan_min[attr->index]);
	if (val < 0)
		return val;

	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t show_in(struct device *dev, struct device_attribute
	*devattr, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	struct sch5627_data *data = sch5627_update_device(dev);
	int val;

	if (IS_ERR(data))
		return PTR_ERR(data);

	val = DIV_ROUND_CLOSEST(
		data->in[attr->index] * SCH5627_REG_IN_FACTOR[attr->index],
		10000);
	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t show_in_label(struct device *dev, struct device_attribute
	*devattr, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);

	return snprintf(buf, PAGE_SIZE, "%s\n",
			SCH5627_IN_LABELS[attr->index]);
}

static DEVICE_ATTR(name, S_IRUGO, show_name, NULL);
static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO, show_temp, NULL, 0);
static SENSOR_DEVICE_ATTR(temp2_input, S_IRUGO, show_temp, NULL, 1);
static SENSOR_DEVICE_ATTR(temp3_input, S_IRUGO, show_temp, NULL, 2);
static SENSOR_DEVICE_ATTR(temp4_input, S_IRUGO, show_temp, NULL, 3);
static SENSOR_DEVICE_ATTR(temp5_input, S_IRUGO, show_temp, NULL, 4);
static SENSOR_DEVICE_ATTR(temp6_input, S_IRUGO, show_temp, NULL, 5);
static SENSOR_DEVICE_ATTR(temp7_input, S_IRUGO, show_temp, NULL, 6);
static SENSOR_DEVICE_ATTR(temp8_input, S_IRUGO, show_temp, NULL, 7);
static SENSOR_DEVICE_ATTR(temp1_fault, S_IRUGO, show_temp_fault, NULL, 0);
static SENSOR_DEVICE_ATTR(temp2_fault, S_IRUGO, show_temp_fault, NULL, 1);
static SENSOR_DEVICE_ATTR(temp3_fault, S_IRUGO, show_temp_fault, NULL, 2);
static SENSOR_DEVICE_ATTR(temp4_fault, S_IRUGO, show_temp_fault, NULL, 3);
static SENSOR_DEVICE_ATTR(temp5_fault, S_IRUGO, show_temp_fault, NULL, 4);
static SENSOR_DEVICE_ATTR(temp6_fault, S_IRUGO, show_temp_fault, NULL, 5);
static SENSOR_DEVICE_ATTR(temp7_fault, S_IRUGO, show_temp_fault, NULL, 6);
static SENSOR_DEVICE_ATTR(temp8_fault, S_IRUGO, show_temp_fault, NULL, 7);
static SENSOR_DEVICE_ATTR(temp1_max, S_IRUGO, show_temp_max, NULL, 0);
static SENSOR_DEVICE_ATTR(temp2_max, S_IRUGO, show_temp_max, NULL, 1);
static SENSOR_DEVICE_ATTR(temp3_max, S_IRUGO, show_temp_max, NULL, 2);
static SENSOR_DEVICE_ATTR(temp4_max, S_IRUGO, show_temp_max, NULL, 3);
static SENSOR_DEVICE_ATTR(temp5_max, S_IRUGO, show_temp_max, NULL, 4);
static SENSOR_DEVICE_ATTR(temp6_max, S_IRUGO, show_temp_max, NULL, 5);
static SENSOR_DEVICE_ATTR(temp7_max, S_IRUGO, show_temp_max, NULL, 6);
static SENSOR_DEVICE_ATTR(temp8_max, S_IRUGO, show_temp_max, NULL, 7);
static SENSOR_DEVICE_ATTR(temp1_crit, S_IRUGO, show_temp_crit, NULL, 0);
static SENSOR_DEVICE_ATTR(temp2_crit, S_IRUGO, show_temp_crit, NULL, 1);
static SENSOR_DEVICE_ATTR(temp3_crit, S_IRUGO, show_temp_crit, NULL, 2);
static SENSOR_DEVICE_ATTR(temp4_crit, S_IRUGO, show_temp_crit, NULL, 3);
static SENSOR_DEVICE_ATTR(temp5_crit, S_IRUGO, show_temp_crit, NULL, 4);
static SENSOR_DEVICE_ATTR(temp6_crit, S_IRUGO, show_temp_crit, NULL, 5);
static SENSOR_DEVICE_ATTR(temp7_crit, S_IRUGO, show_temp_crit, NULL, 6);
static SENSOR_DEVICE_ATTR(temp8_crit, S_IRUGO, show_temp_crit, NULL, 7);

static SENSOR_DEVICE_ATTR(fan1_input, S_IRUGO, show_fan, NULL, 0);
static SENSOR_DEVICE_ATTR(fan2_input, S_IRUGO, show_fan, NULL, 1);
static SENSOR_DEVICE_ATTR(fan3_input, S_IRUGO, show_fan, NULL, 2);
static SENSOR_DEVICE_ATTR(fan4_input, S_IRUGO, show_fan, NULL, 3);
static SENSOR_DEVICE_ATTR(fan1_fault, S_IRUGO, show_fan_fault, NULL, 0);
static SENSOR_DEVICE_ATTR(fan2_fault, S_IRUGO, show_fan_fault, NULL, 1);
static SENSOR_DEVICE_ATTR(fan3_fault, S_IRUGO, show_fan_fault, NULL, 2);
static SENSOR_DEVICE_ATTR(fan4_fault, S_IRUGO, show_fan_fault, NULL, 3);
static SENSOR_DEVICE_ATTR(fan1_min, S_IRUGO, show_fan_min, NULL, 0);
static SENSOR_DEVICE_ATTR(fan2_min, S_IRUGO, show_fan_min, NULL, 1);
static SENSOR_DEVICE_ATTR(fan3_min, S_IRUGO, show_fan_min, NULL, 2);
static SENSOR_DEVICE_ATTR(fan4_min, S_IRUGO, show_fan_min, NULL, 3);

static SENSOR_DEVICE_ATTR(in0_input, S_IRUGO, show_in, NULL, 0);
static SENSOR_DEVICE_ATTR(in1_input, S_IRUGO, show_in, NULL, 1);
static SENSOR_DEVICE_ATTR(in2_input, S_IRUGO, show_in, NULL, 2);
static SENSOR_DEVICE_ATTR(in3_input, S_IRUGO, show_in, NULL, 3);
static SENSOR_DEVICE_ATTR(in4_input, S_IRUGO, show_in, NULL, 4);
static SENSOR_DEVICE_ATTR(in0_label, S_IRUGO, show_in_label, NULL, 0);
static SENSOR_DEVICE_ATTR(in1_label, S_IRUGO, show_in_label, NULL, 1);
static SENSOR_DEVICE_ATTR(in2_label, S_IRUGO, show_in_label, NULL, 2);
static SENSOR_DEVICE_ATTR(in3_label, S_IRUGO, show_in_label, NULL, 3);

static struct attribute *sch5627_attributes[] = {
	&dev_attr_name.attr,

	&sensor_dev_attr_temp1_input.dev_attr.attr,
	&sensor_dev_attr_temp2_input.dev_attr.attr,
	&sensor_dev_attr_temp3_input.dev_attr.attr,
	&sensor_dev_attr_temp4_input.dev_attr.attr,
	&sensor_dev_attr_temp5_input.dev_attr.attr,
	&sensor_dev_attr_temp6_input.dev_attr.attr,
	&sensor_dev_attr_temp7_input.dev_attr.attr,
	&sensor_dev_attr_temp8_input.dev_attr.attr,
	&sensor_dev_attr_temp1_fault.dev_attr.attr,
	&sensor_dev_attr_temp2_fault.dev_attr.attr,
	&sensor_dev_attr_temp3_fault.dev_attr.attr,
	&sensor_dev_attr_temp4_fault.dev_attr.attr,
	&sensor_dev_attr_temp5_fault.dev_attr.attr,
	&sensor_dev_attr_temp6_fault.dev_attr.attr,
	&sensor_dev_attr_temp7_fault.dev_attr.attr,
	&sensor_dev_attr_temp8_fault.dev_attr.attr,
	&sensor_dev_attr_temp1_max.dev_attr.attr,
	&sensor_dev_attr_temp2_max.dev_attr.attr,
	&sensor_dev_attr_temp3_max.dev_attr.attr,
	&sensor_dev_attr_temp4_max.dev_attr.attr,
	&sensor_dev_attr_temp5_max.dev_attr.attr,
	&sensor_dev_attr_temp6_max.dev_attr.attr,
	&sensor_dev_attr_temp7_max.dev_attr.attr,
	&sensor_dev_attr_temp8_max.dev_attr.attr,
	&sensor_dev_attr_temp1_crit.dev_attr.attr,
	&sensor_dev_attr_temp2_crit.dev_attr.attr,
	&sensor_dev_attr_temp3_crit.dev_attr.attr,
	&sensor_dev_attr_temp4_crit.dev_attr.attr,
	&sensor_dev_attr_temp5_crit.dev_attr.attr,
	&sensor_dev_attr_temp6_crit.dev_attr.attr,
	&sensor_dev_attr_temp7_crit.dev_attr.attr,
	&sensor_dev_attr_temp8_crit.dev_attr.attr,

	&sensor_dev_attr_fan1_input.dev_attr.attr,
	&sensor_dev_attr_fan2_input.dev_attr.attr,
	&sensor_dev_attr_fan3_input.dev_attr.attr,
	&sensor_dev_attr_fan4_input.dev_attr.attr,
	&sensor_dev_attr_fan1_fault.dev_attr.attr,
	&sensor_dev_attr_fan2_fault.dev_attr.attr,
	&sensor_dev_attr_fan3_fault.dev_attr.attr,
	&sensor_dev_attr_fan4_fault.dev_attr.attr,
	&sensor_dev_attr_fan1_min.dev_attr.attr,
	&sensor_dev_attr_fan2_min.dev_attr.attr,
	&sensor_dev_attr_fan3_min.dev_attr.attr,
	&sensor_dev_attr_fan4_min.dev_attr.attr,

	&sensor_dev_attr_in0_input.dev_attr.attr,
	&sensor_dev_attr_in1_input.dev_attr.attr,
	&sensor_dev_attr_in2_input.dev_attr.attr,
	&sensor_dev_attr_in3_input.dev_attr.attr,
	&sensor_dev_attr_in4_input.dev_attr.attr,
	&sensor_dev_attr_in0_label.dev_attr.attr,
	&sensor_dev_attr_in1_label.dev_attr.attr,
	&sensor_dev_attr_in2_label.dev_attr.attr,
	&sensor_dev_attr_in3_label.dev_attr.attr,
	/* No in4_label as in4 is a generic input pin */

	NULL
};

static const struct attribute_group sch5627_group = {
	.attrs = sch5627_attributes,
};

static int sch5627_remove(struct platform_device *pdev)
{
	struct sch5627_data *data = platform_get_drvdata(pdev);

	if (data->watchdog)
		sch56xx_watchdog_unregister(data->watchdog);

	if (data->hwmon_dev)
		hwmon_device_unregister(data->hwmon_dev);

	sysfs_remove_group(&pdev->dev.kobj, &sch5627_group);
	platform_set_drvdata(pdev, NULL);
	kfree(data);

	return 0;
}

static int __devinit sch5627_probe(struct platform_device *pdev)
{
	struct sch5627_data *data;
	int err, build_code, build_id, hwmon_rev, val;

	data = kzalloc(sizeof(struct sch5627_data), GFP_KERNEL);
=======
static u8 sch5627_temp_limit_to_reg(long value)
{
	long limit = (value / 1000) + 64;

	return clamp_val(limit, 0, U8_MAX);
}

static u16 sch5627_rpm_to_reg(long value)
{
	long pulses;

	if (value <= 0)
		return U16_MAX - 1;

	pulses = 5400540 / value;

	return clamp_val(pulses, 1, U16_MAX - 1);
}

static umode_t sch5627_is_visible(const void *drvdata, enum hwmon_sensor_types type, u32 attr,
				  int channel)
{
	const struct sch5627_data *data = drvdata;

	/* Once the lock bit is set, the virtual registers become read-only
	 * until the next power cycle.
	 */
	if (data->control & SCH5627_CTRL_LOCK)
		return 0444;

	switch (type) {
	case hwmon_temp:
		switch (attr) {
		case hwmon_temp_max:
		case hwmon_temp_crit:
			return 0644;
		default:
			break;
		}
		break;
	case hwmon_fan:
		switch (attr) {
		case hwmon_fan_min:
			return 0644;
		default:
			break;
		}
		break;
	case hwmon_pwm:
		switch (attr) {
		case hwmon_pwm_auto_channels_temp:
			return 0644;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return 0444;
}

static int sch5627_read(struct device *dev, enum hwmon_sensor_types type, u32 attr, int channel,
			long *val)
{
	struct sch5627_data *data = dev_get_drvdata(dev);
	int ret, value;

	switch (type) {
	case hwmon_temp:
		switch (attr) {
		case hwmon_temp_input:
			ret = sch5627_update_temp(data);
			if (ret < 0)
				return ret;

			*val = reg_to_temp(data->temp[channel]);
			return 0;
		case hwmon_temp_max:
			ret = regmap_read(data->regmap, SCH5627_REG_TEMP_ABS[channel], &value);
			if (ret < 0)
				return ret;

			*val = reg_to_temp_limit((u8)value);
			return 0;
		case hwmon_temp_crit:
			ret = regmap_read(data->regmap, SCH5627_REG_TEMP_HIGH[channel], &value);
			if (ret < 0)
				return ret;

			*val = reg_to_temp_limit((u8)value);
			return 0;
		case hwmon_temp_fault:
			ret = sch5627_update_temp(data);
			if (ret < 0)
				return ret;

			*val = (data->temp[channel] == 0);
			return 0;
		default:
			break;
		}
		break;
	case hwmon_fan:
		switch (attr) {
		case hwmon_fan_input:
			ret = sch5627_update_fan(data);
			if (ret < 0)
				return ret;

			ret = reg_to_rpm(data->fan[channel]);
			if (ret < 0)
				return ret;

			*val = ret;
			return 0;
		case hwmon_fan_min:
			ret = sch56xx_regmap_read16(data->regmap, SCH5627_REG_FAN_MIN[channel],
						    &value);
			if (ret < 0)
				return ret;

			ret = reg_to_rpm((u16)value);
			if (ret < 0)
				return ret;

			*val = ret;
			return 0;
		case hwmon_fan_fault:
			ret = sch5627_update_fan(data);
			if (ret < 0)
				return ret;

			*val = (data->fan[channel] == 0xffff);
			return 0;
		default:
			break;
		}
		break;
	case hwmon_pwm:
		switch (attr) {
		case hwmon_pwm_auto_channels_temp:
			ret = regmap_read(data->regmap, SCH5627_REG_PWM_MAP[channel], &value);
			if (ret < 0)
				return ret;

			*val = value;
			return 0;
		default:
			break;
		}
		break;
	case hwmon_in:
		ret = sch5627_update_in(data);
		if (ret < 0)
			return ret;
		switch (attr) {
		case hwmon_in_input:
			*val = DIV_ROUND_CLOSEST(data->in[channel] * SCH5627_REG_IN_FACTOR[channel],
						 10000);
			return 0;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return -EOPNOTSUPP;
}

static int sch5627_read_string(struct device *dev, enum hwmon_sensor_types type, u32 attr,
			       int channel, const char **str)
{
	switch (type) {
	case hwmon_in:
		switch (attr) {
		case hwmon_in_label:
			*str = SCH5627_IN_LABELS[channel];
			return 0;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return -EOPNOTSUPP;
}

static int sch5627_write(struct device *dev, enum hwmon_sensor_types type, u32 attr, int channel,
			 long val)
{
	struct sch5627_data *data = dev_get_drvdata(dev);
	u16 fan;
	u8 temp;

	switch (type) {
	case hwmon_temp:
		temp = sch5627_temp_limit_to_reg(val);

		switch (attr) {
		case hwmon_temp_max:
			return regmap_write(data->regmap, SCH5627_REG_TEMP_ABS[channel], temp);
		case hwmon_temp_crit:
			return regmap_write(data->regmap, SCH5627_REG_TEMP_HIGH[channel], temp);
		default:
			break;
		}
		break;
	case hwmon_fan:
		switch (attr) {
		case hwmon_fan_min:
			fan = sch5627_rpm_to_reg(val);

			return sch56xx_regmap_write16(data->regmap, SCH5627_REG_FAN_MIN[channel],
						      fan);
		default:
			break;
		}
		break;
	case hwmon_pwm:
		switch (attr) {
		case hwmon_pwm_auto_channels_temp:
			/* registers are 8 bit wide */
			if (val > U8_MAX || val < 0)
				return -EINVAL;

			return regmap_write(data->regmap, SCH5627_REG_PWM_MAP[channel], val);
		default:
			break;
		}
		break;
	default:
		break;
	}

	return -EOPNOTSUPP;
}

static const struct hwmon_ops sch5627_ops = {
	.is_visible = sch5627_is_visible,
	.read = sch5627_read,
	.read_string = sch5627_read_string,
	.write = sch5627_write,
};

static const struct hwmon_channel_info * const sch5627_info[] = {
	HWMON_CHANNEL_INFO(chip, HWMON_C_REGISTER_TZ),
	HWMON_CHANNEL_INFO(temp,
			   HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_CRIT | HWMON_T_FAULT,
			   HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_CRIT | HWMON_T_FAULT,
			   HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_CRIT | HWMON_T_FAULT,
			   HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_CRIT | HWMON_T_FAULT,
			   HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_CRIT | HWMON_T_FAULT,
			   HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_CRIT | HWMON_T_FAULT,
			   HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_CRIT | HWMON_T_FAULT,
			   HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_CRIT | HWMON_T_FAULT
			   ),
	HWMON_CHANNEL_INFO(fan,
			   HWMON_F_INPUT | HWMON_F_MIN | HWMON_F_FAULT,
			   HWMON_F_INPUT | HWMON_F_MIN | HWMON_F_FAULT,
			   HWMON_F_INPUT | HWMON_F_MIN | HWMON_F_FAULT,
			   HWMON_F_INPUT | HWMON_F_MIN | HWMON_F_FAULT
			   ),
	HWMON_CHANNEL_INFO(pwm,
			   HWMON_PWM_AUTO_CHANNELS_TEMP,
			   HWMON_PWM_AUTO_CHANNELS_TEMP,
			   HWMON_PWM_AUTO_CHANNELS_TEMP,
			   HWMON_PWM_AUTO_CHANNELS_TEMP
			   ),
	HWMON_CHANNEL_INFO(in,
			   HWMON_I_INPUT | HWMON_I_LABEL,
			   HWMON_I_INPUT | HWMON_I_LABEL,
			   HWMON_I_INPUT | HWMON_I_LABEL,
			   HWMON_I_INPUT | HWMON_I_LABEL,
			   HWMON_I_INPUT
			   ),
	NULL
};

static const struct hwmon_chip_info sch5627_chip_info = {
	.ops = &sch5627_ops,
	.info = sch5627_info,
};

static int sch5627_probe(struct platform_device *pdev)
{
	struct sch5627_data *data;
	struct device *hwmon_dev;
	int build_code, build_id, hwmon_rev, val;

	data = devm_kzalloc(&pdev->dev, sizeof(struct sch5627_data),
			    GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!data)
		return -ENOMEM;

	data->addr = platform_get_resource(pdev, IORESOURCE_IO, 0)->start;
	mutex_init(&data->update_lock);
	platform_set_drvdata(pdev, data);

	val = sch56xx_read_virtual_reg(data->addr, SCH5627_REG_HWMON_ID);
<<<<<<< HEAD
	if (val < 0) {
		err = val;
		goto error;
	}
	if (val != SCH5627_HWMON_ID) {
		pr_err("invalid %s id: 0x%02X (expected 0x%02X)\n", "hwmon",
		       val, SCH5627_HWMON_ID);
		err = -ENODEV;
		goto error;
	}

	val = sch56xx_read_virtual_reg(data->addr, SCH5627_REG_COMPANY_ID);
	if (val < 0) {
		err = val;
		goto error;
	}
	if (val != SCH5627_COMPANY_ID) {
		pr_err("invalid %s id: 0x%02X (expected 0x%02X)\n", "company",
		       val, SCH5627_COMPANY_ID);
		err = -ENODEV;
		goto error;
	}

	val = sch56xx_read_virtual_reg(data->addr, SCH5627_REG_PRIMARY_ID);
	if (val < 0) {
		err = val;
		goto error;
	}
	if (val != SCH5627_PRIMARY_ID) {
		pr_err("invalid %s id: 0x%02X (expected 0x%02X)\n", "primary",
		       val, SCH5627_PRIMARY_ID);
		err = -ENODEV;
		goto error;
=======
	if (val < 0)
		return val;

	if (val != SCH5627_HWMON_ID) {
		pr_err("invalid %s id: 0x%02X (expected 0x%02X)\n", "hwmon",
		       val, SCH5627_HWMON_ID);
		return -ENODEV;
	}

	val = sch56xx_read_virtual_reg(data->addr, SCH5627_REG_COMPANY_ID);
	if (val < 0)
		return val;

	if (val != SCH5627_COMPANY_ID) {
		pr_err("invalid %s id: 0x%02X (expected 0x%02X)\n", "company",
		       val, SCH5627_COMPANY_ID);
		return -ENODEV;
	}

	val = sch56xx_read_virtual_reg(data->addr, SCH5627_REG_PRIMARY_ID);
	if (val < 0)
		return val;

	if (val != SCH5627_PRIMARY_ID) {
		pr_err("invalid %s id: 0x%02X (expected 0x%02X)\n", "primary",
		       val, SCH5627_PRIMARY_ID);
		return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	build_code = sch56xx_read_virtual_reg(data->addr,
					      SCH5627_REG_BUILD_CODE);
<<<<<<< HEAD
	if (build_code < 0) {
		err = build_code;
		goto error;
	}

	build_id = sch56xx_read_virtual_reg16(data->addr,
					      SCH5627_REG_BUILD_ID);
	if (build_id < 0) {
		err = build_id;
		goto error;
	}

	hwmon_rev = sch56xx_read_virtual_reg(data->addr,
					     SCH5627_REG_HWMON_REV);
	if (hwmon_rev < 0) {
		err = hwmon_rev;
		goto error;
	}

	val = sch56xx_read_virtual_reg(data->addr, SCH5627_REG_CTRL);
	if (val < 0) {
		err = val;
		goto error;
	}
	data->control = val;
	if (!(data->control & 0x01)) {
		pr_err("hardware monitoring not enabled\n");
		err = -ENODEV;
		goto error;
	}
	/* Trigger a Vbat voltage measurement, so that we get a valid reading
	   the first time we read Vbat */
	sch56xx_write_virtual_reg(data->addr, SCH5627_REG_CTRL,
				  data->control | 0x10);
	data->last_battery = jiffies;

	/*
	 * Read limits, we do this only once as reading a register on
	 * the sch5627 is quite expensive (and they don't change).
	 */
	err = sch5627_read_limits(data);
	if (err)
		goto error;

=======
	if (build_code < 0)
		return build_code;

	build_id = sch56xx_read_virtual_reg16(data->addr,
					      SCH5627_REG_BUILD_ID);
	if (build_id < 0)
		return build_id;

	hwmon_rev = sch56xx_read_virtual_reg(data->addr,
					     SCH5627_REG_HWMON_REV);
	if (hwmon_rev < 0)
		return hwmon_rev;

	val = sch56xx_read_virtual_reg(data->addr, SCH5627_REG_CTRL);
	if (val < 0)
		return val;

	data->control = val;
	if (!(data->control & SCH5627_CTRL_START)) {
		pr_err("hardware monitoring not enabled\n");
		return -ENODEV;
	}

	data->regmap = devm_regmap_init_sch56xx(&pdev->dev, &data->update_lock, data->addr,
						&sch5627_regmap_config);
	if (IS_ERR(data->regmap))
		return PTR_ERR(data->regmap);

	/* Trigger a Vbat voltage measurement, so that we get a valid reading
	   the first time we read Vbat */
	sch56xx_write_virtual_reg(data->addr, SCH5627_REG_CTRL, data->control | SCH5627_CTRL_VBAT);
	data->last_battery = jiffies;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pr_info("found %s chip at %#hx\n", DEVNAME, data->addr);
	pr_info("firmware build: code 0x%02X, id 0x%04X, hwmon: rev 0x%02X\n",
		build_code, build_id, hwmon_rev);

<<<<<<< HEAD
	/* Register sysfs interface files */
	err = sysfs_create_group(&pdev->dev.kobj, &sch5627_group);
	if (err)
		goto error;

	data->hwmon_dev = hwmon_device_register(&pdev->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		data->hwmon_dev = NULL;
		goto error;
	}

	/* Note failing to register the watchdog is not a fatal error */
	data->watchdog = sch56xx_watchdog_register(data->addr,
			(build_code << 24) | (build_id << 8) | hwmon_rev,
			&data->update_lock, 1);

	return 0;

error:
	sch5627_remove(pdev);
	return err;
}

static struct platform_driver sch5627_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= DRVNAME,
	},
	.probe		= sch5627_probe,
	.remove		= sch5627_remove,
=======
	hwmon_dev = devm_hwmon_device_register_with_info(&pdev->dev, DEVNAME, data,
							 &sch5627_chip_info, NULL);
	if (IS_ERR(hwmon_dev))
		return PTR_ERR(hwmon_dev);

	/* Note failing to register the watchdog is not a fatal error */
	sch56xx_watchdog_register(&pdev->dev, data->addr,
				  (build_code << 24) | (build_id << 8) | hwmon_rev,
				  &data->update_lock, 1);

	return 0;
}

static int sch5627_suspend(struct device *dev)
{
	struct sch5627_data *data = dev_get_drvdata(dev);

	regcache_cache_only(data->regmap, true);
	regcache_mark_dirty(data->regmap);

	return 0;
}

static int sch5627_resume(struct device *dev)
{
	struct sch5627_data *data = dev_get_drvdata(dev);

	regcache_cache_only(data->regmap, false);
	/* We must not access the virtual registers when the lock bit is set */
	if (data->control & SCH5627_CTRL_LOCK)
		return regcache_drop_region(data->regmap, 0, U16_MAX);

	return regcache_sync(data->regmap);
}

static DEFINE_SIMPLE_DEV_PM_OPS(sch5627_dev_pm_ops, sch5627_suspend, sch5627_resume);

static const struct platform_device_id sch5627_device_id[] = {
	{
		.name = "sch5627",
	},
	{ }
};
MODULE_DEVICE_TABLE(platform, sch5627_device_id);

static struct platform_driver sch5627_driver = {
	.driver = {
		.name	= DRVNAME,
		.pm	= pm_sleep_ptr(&sch5627_dev_pm_ops),
	},
	.probe		= sch5627_probe,
	.id_table	= sch5627_device_id,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(sch5627_driver);

MODULE_DESCRIPTION("SMSC SCH5627 Hardware Monitoring Driver");
MODULE_AUTHOR("Hans de Goede <hdegoede@redhat.com>");
MODULE_LICENSE("GPL");
