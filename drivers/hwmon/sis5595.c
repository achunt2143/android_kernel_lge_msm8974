<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * sis5595.c - Part of lm_sensors, Linux kernel modules
 *	       for hardware monitoring
 *
 * Copyright (C) 1998 - 2001 Frodo Looijaard <frodol@dds.nl>,
 *			     Kyösti Mälkki <kmalkki@cc.hut.fi>, and
 *			     Mark D. Studebaker <mdsxyz123@yahoo.com>
 * Ported to Linux 2.6 by Aurelien Jarno <aurelien@aurel32.net> with
<<<<<<< HEAD
 * the help of Jean Delvare <khali@linux-fr.org>
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
 * the help of Jean Delvare <jdelvare@suse.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * SiS southbridge has a LM78-like chip integrated on the same IC.
 * This driver is a customized copy of lm78.c
 *
 * Supports following revisions:
 *	Version		PCI ID		PCI Revision
 *	1		1039/0008	AF or less
 *	2		1039/0008	B0 or greater
 *
 *  Note: these chips contain a 0008 device which is incompatible with the
 *	 5595. We recognize these by the presence of the listed
 *	 "blacklist" PCI ID and refuse to load.
 *
 * NOT SUPPORTED	PCI ID		BLACKLIST PCI ID
 *	 540		0008		0540
 *	 550		0008		0550
 *	5513		0008		5511
 *	5581		0008		5597
 *	5582		0008		5597
 *	5597		0008		5597
 *	5598		0008		5597/5598
 *	 630		0008		0630
 *	 645		0008		0645
 *	 730		0008		0730
 *	 735		0008		0735
 */

<<<<<<< HEAD
=======
#define DRIVER_NAME "sis5595"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>
#include <linux/sysfs.h>
#include <linux/acpi.h>
#include <linux/io.h>

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * If force_addr is set to anything different from 0, we forcibly enable
 * the device at the given address.
 */
static u16 force_addr;
module_param(force_addr, ushort, 0);
MODULE_PARM_DESC(force_addr,
		 "Initialize the base address of the sensors");

static struct platform_device *pdev;

/* Many SIS5595 constants specified below */

/* Length of ISA address segment */
#define SIS5595_EXTENT 8
/* PCI Config Registers */
#define SIS5595_BASE_REG 0x68
#define SIS5595_PIN_REG 0x7A
#define SIS5595_ENABLE_REG 0x7B

/* Where are the ISA address/data registers relative to the base address */
#define SIS5595_ADDR_REG_OFFSET 5
#define SIS5595_DATA_REG_OFFSET 6

/* The SIS5595 registers */
#define SIS5595_REG_IN_MAX(nr) (0x2b + (nr) * 2)
#define SIS5595_REG_IN_MIN(nr) (0x2c + (nr) * 2)
#define SIS5595_REG_IN(nr) (0x20 + (nr))

#define SIS5595_REG_FAN_MIN(nr) (0x3b + (nr))
#define SIS5595_REG_FAN(nr) (0x28 + (nr))

/*
 * On the first version of the chip, the temp registers are separate.
 * On the second version,
 * TEMP pin is shared with IN4, configured in PCI register 0x7A.
 * The registers are the same as well.
 * OVER and HYST are really MAX and MIN.
 */

#define REV2MIN	0xb0
#define SIS5595_REG_TEMP	(((data->revision) >= REV2MIN) ? \
					SIS5595_REG_IN(4) : 0x27)
#define SIS5595_REG_TEMP_OVER	(((data->revision) >= REV2MIN) ? \
					SIS5595_REG_IN_MAX(4) : 0x39)
#define SIS5595_REG_TEMP_HYST	(((data->revision) >= REV2MIN) ? \
					SIS5595_REG_IN_MIN(4) : 0x3a)

#define SIS5595_REG_CONFIG 0x40
#define SIS5595_REG_ALARM1 0x41
#define SIS5595_REG_ALARM2 0x42
#define SIS5595_REG_FANDIV 0x47

/*
 * Conversions. Limit checking is only done on the TO_REG
 * variants.
 */

/*
 * IN: mV, (0V to 4.08V)
 * REG: 16mV/bit
 */
static inline u8 IN_TO_REG(unsigned long val)
{
<<<<<<< HEAD
	unsigned long nval = SENSORS_LIMIT(val, 0, 4080);
=======
	unsigned long nval = clamp_val(val, 0, 4080);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (nval + 8) / 16;
}
#define IN_FROM_REG(val) ((val) *  16)

static inline u8 FAN_TO_REG(long rpm, int div)
{
	if (rpm <= 0)
		return 255;
	if (rpm > 1350000)
		return 1;
<<<<<<< HEAD
	return SENSORS_LIMIT((1350000 + rpm * div / 2) / (rpm * div), 1, 254);
=======
	return clamp_val((1350000 + rpm * div / 2) / (rpm * div), 1, 254);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int FAN_FROM_REG(u8 val, int div)
{
	return val == 0 ? -1 : val == 255 ? 0 : 1350000 / (val * div);
}

/*
 * TEMP: mC (-54.12C to +157.53C)
 * REG: 0.83C/bit + 52.12, two's complement
 */
static inline int TEMP_FROM_REG(s8 val)
{
	return val * 830 + 52120;
}
<<<<<<< HEAD
static inline s8 TEMP_TO_REG(int val)
{
	int nval = SENSORS_LIMIT(val, -54120, 157530) ;
=======
static inline s8 TEMP_TO_REG(long val)
{
	int nval = clamp_val(val, -54120, 157530) ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return nval < 0 ? (nval - 5212 - 415) / 830 : (nval - 5212 + 415) / 830;
}

/*
<<<<<<< HEAD
 * FAN DIV: 1, 2, 4, or 8 (defaults to 2)
 * REG: 0, 1, 2, or 3 (respectively) (defaults to 1)
 */
static inline u8 DIV_TO_REG(int val)
{
	return val == 8 ? 3 : val == 4 ? 2 : val == 1 ? 0 : 1;
}
=======
 * FAN DIV: 1, 2, 4, or 8
 * REG: 0, 1, 2, or 3 (respectively)
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DIV_FROM_REG(val) (1 << (val))

/*
 * For each registered chip, we need to keep some data in memory.
 * The structure is dynamically allocated.
 */
struct sis5595_data {
	unsigned short addr;
	const char *name;
	struct device *hwmon_dev;
	struct mutex lock;

	struct mutex update_lock;
<<<<<<< HEAD
	char valid;		/* !=0 if following fields are valid */
=======
	bool valid;		/* true if following fields are valid */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long last_updated;	/* In jiffies */
	char maxins;		/* == 3 if temp enabled, otherwise == 4 */
	u8 revision;		/* Reg. value */

	u8 in[5];		/* Register value */
	u8 in_max[5];		/* Register value */
	u8 in_min[5];		/* Register value */
	u8 fan[2];		/* Register value */
	u8 fan_min[2];		/* Register value */
	s8 temp;		/* Register value */
	s8 temp_over;		/* Register value */
	s8 temp_hyst;		/* Register value */
	u8 fan_div[2];		/* Register encoding, shifted right */
	u16 alarms;		/* Register encoding, combined */
};

static struct pci_dev *s_bridge;	/* pointer to the (only) sis5595 */

<<<<<<< HEAD
static int sis5595_probe(struct platform_device *pdev);
static int __devexit sis5595_remove(struct platform_device *pdev);

static int sis5595_read_value(struct sis5595_data *data, u8 reg);
static void sis5595_write_value(struct sis5595_data *data, u8 reg, u8 value);
static struct sis5595_data *sis5595_update_device(struct device *dev);
static void sis5595_init_device(struct sis5595_data *data);

static struct platform_driver sis5595_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "sis5595",
	},
	.probe		= sis5595_probe,
	.remove		= __devexit_p(sis5595_remove),
};

/* 4 Voltages */
static ssize_t show_in(struct device *dev, struct device_attribute *da,
=======
/* ISA access must be locked explicitly. */
static int sis5595_read_value(struct sis5595_data *data, u8 reg)
{
	int res;

	mutex_lock(&data->lock);
	outb_p(reg, data->addr + SIS5595_ADDR_REG_OFFSET);
	res = inb_p(data->addr + SIS5595_DATA_REG_OFFSET);
	mutex_unlock(&data->lock);
	return res;
}

static void sis5595_write_value(struct sis5595_data *data, u8 reg, u8 value)
{
	mutex_lock(&data->lock);
	outb_p(reg, data->addr + SIS5595_ADDR_REG_OFFSET);
	outb_p(value, data->addr + SIS5595_DATA_REG_OFFSET);
	mutex_unlock(&data->lock);
}

static struct sis5595_data *sis5595_update_device(struct device *dev)
{
	struct sis5595_data *data = dev_get_drvdata(dev);
	int i;

	mutex_lock(&data->update_lock);

	if (time_after(jiffies, data->last_updated + HZ + HZ / 2)
	    || !data->valid) {

		for (i = 0; i <= data->maxins; i++) {
			data->in[i] =
			    sis5595_read_value(data, SIS5595_REG_IN(i));
			data->in_min[i] =
			    sis5595_read_value(data,
					       SIS5595_REG_IN_MIN(i));
			data->in_max[i] =
			    sis5595_read_value(data,
					       SIS5595_REG_IN_MAX(i));
		}
		for (i = 0; i < 2; i++) {
			data->fan[i] =
			    sis5595_read_value(data, SIS5595_REG_FAN(i));
			data->fan_min[i] =
			    sis5595_read_value(data,
					       SIS5595_REG_FAN_MIN(i));
		}
		if (data->maxins == 3) {
			data->temp =
			    sis5595_read_value(data, SIS5595_REG_TEMP);
			data->temp_over =
			    sis5595_read_value(data, SIS5595_REG_TEMP_OVER);
			data->temp_hyst =
			    sis5595_read_value(data, SIS5595_REG_TEMP_HYST);
		}
		i = sis5595_read_value(data, SIS5595_REG_FANDIV);
		data->fan_div[0] = (i >> 4) & 0x03;
		data->fan_div[1] = i >> 6;
		data->alarms =
		    sis5595_read_value(data, SIS5595_REG_ALARM1) |
		    (sis5595_read_value(data, SIS5595_REG_ALARM2) << 8);
		data->last_updated = jiffies;
		data->valid = true;
	}

	mutex_unlock(&data->update_lock);

	return data;
}

/* 4 Voltages */
static ssize_t in_show(struct device *dev, struct device_attribute *da,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       char *buf)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	return sprintf(buf, "%d\n", IN_FROM_REG(data->in[nr]));
}

<<<<<<< HEAD
static ssize_t show_in_min(struct device *dev, struct device_attribute *da,
=======
static ssize_t in_min_show(struct device *dev, struct device_attribute *da,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   char *buf)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	return sprintf(buf, "%d\n", IN_FROM_REG(data->in_min[nr]));
}

<<<<<<< HEAD
static ssize_t show_in_max(struct device *dev, struct device_attribute *da,
=======
static ssize_t in_max_show(struct device *dev, struct device_attribute *da,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   char *buf)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	return sprintf(buf, "%d\n", IN_FROM_REG(data->in_max[nr]));
}

<<<<<<< HEAD
static ssize_t set_in_min(struct device *dev, struct device_attribute *da,
			  const char *buf, size_t count)
=======
static ssize_t in_min_store(struct device *dev, struct device_attribute *da,
			    const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sis5595_data *data = dev_get_drvdata(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;

	mutex_lock(&data->update_lock);
	data->in_min[nr] = IN_TO_REG(val);
	sis5595_write_value(data, SIS5595_REG_IN_MIN(nr), data->in_min[nr]);
	mutex_unlock(&data->update_lock);
	return count;
}

<<<<<<< HEAD
static ssize_t set_in_max(struct device *dev, struct device_attribute *da,
			  const char *buf, size_t count)
=======
static ssize_t in_max_store(struct device *dev, struct device_attribute *da,
			    const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sis5595_data *data = dev_get_drvdata(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;

	mutex_lock(&data->update_lock);
	data->in_max[nr] = IN_TO_REG(val);
	sis5595_write_value(data, SIS5595_REG_IN_MAX(nr), data->in_max[nr]);
	mutex_unlock(&data->update_lock);
	return count;
}

<<<<<<< HEAD
#define show_in_offset(offset)					\
static SENSOR_DEVICE_ATTR(in##offset##_input, S_IRUGO,		\
		show_in, NULL, offset);				\
static SENSOR_DEVICE_ATTR(in##offset##_min, S_IRUGO | S_IWUSR,	\
		show_in_min, set_in_min, offset);		\
static SENSOR_DEVICE_ATTR(in##offset##_max, S_IRUGO | S_IWUSR,	\
		show_in_max, set_in_max, offset);

show_in_offset(0);
show_in_offset(1);
show_in_offset(2);
show_in_offset(3);
show_in_offset(4);

/* Temperature */
static ssize_t show_temp(struct device *dev, struct device_attribute *attr,
			 char *buf)
=======
static SENSOR_DEVICE_ATTR_RO(in0_input, in, 0);
static SENSOR_DEVICE_ATTR_RW(in0_min, in_min, 0);
static SENSOR_DEVICE_ATTR_RW(in0_max, in_max, 0);
static SENSOR_DEVICE_ATTR_RO(in1_input, in, 1);
static SENSOR_DEVICE_ATTR_RW(in1_min, in_min, 1);
static SENSOR_DEVICE_ATTR_RW(in1_max, in_max, 1);
static SENSOR_DEVICE_ATTR_RO(in2_input, in, 2);
static SENSOR_DEVICE_ATTR_RW(in2_min, in_min, 2);
static SENSOR_DEVICE_ATTR_RW(in2_max, in_max, 2);
static SENSOR_DEVICE_ATTR_RO(in3_input, in, 3);
static SENSOR_DEVICE_ATTR_RW(in3_min, in_min, 3);
static SENSOR_DEVICE_ATTR_RW(in3_max, in_max, 3);
static SENSOR_DEVICE_ATTR_RO(in4_input, in, 4);
static SENSOR_DEVICE_ATTR_RW(in4_min, in_min, 4);
static SENSOR_DEVICE_ATTR_RW(in4_max, in_max, 4);

/* Temperature */
static ssize_t temp1_input_show(struct device *dev,
				struct device_attribute *attr, char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	return sprintf(buf, "%d\n", TEMP_FROM_REG(data->temp));
}

<<<<<<< HEAD
static ssize_t show_temp_over(struct device *dev, struct device_attribute *attr,
=======
static ssize_t temp1_max_show(struct device *dev, struct device_attribute *attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			      char *buf)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	return sprintf(buf, "%d\n", TEMP_FROM_REG(data->temp_over));
}

<<<<<<< HEAD
static ssize_t set_temp_over(struct device *dev, struct device_attribute *attr,
			     const char *buf, size_t count)
=======
static ssize_t temp1_max_store(struct device *dev,
			       struct device_attribute *attr, const char *buf,
			       size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sis5595_data *data = dev_get_drvdata(dev);
	long val;
	int err;

	err = kstrtol(buf, 10, &val);
	if (err)
		return err;

	mutex_lock(&data->update_lock);
	data->temp_over = TEMP_TO_REG(val);
	sis5595_write_value(data, SIS5595_REG_TEMP_OVER, data->temp_over);
	mutex_unlock(&data->update_lock);
	return count;
}

<<<<<<< HEAD
static ssize_t show_temp_hyst(struct device *dev, struct device_attribute *attr,
			      char *buf)
=======
static ssize_t temp1_max_hyst_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	return sprintf(buf, "%d\n", TEMP_FROM_REG(data->temp_hyst));
}

<<<<<<< HEAD
static ssize_t set_temp_hyst(struct device *dev, struct device_attribute *attr,
			     const char *buf, size_t count)
=======
static ssize_t temp1_max_hyst_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sis5595_data *data = dev_get_drvdata(dev);
	long val;
	int err;

	err = kstrtol(buf, 10, &val);
	if (err)
		return err;

	mutex_lock(&data->update_lock);
	data->temp_hyst = TEMP_TO_REG(val);
	sis5595_write_value(data, SIS5595_REG_TEMP_HYST, data->temp_hyst);
	mutex_unlock(&data->update_lock);
	return count;
}

<<<<<<< HEAD
static DEVICE_ATTR(temp1_input, S_IRUGO, show_temp, NULL);
static DEVICE_ATTR(temp1_max, S_IRUGO | S_IWUSR,
		show_temp_over, set_temp_over);
static DEVICE_ATTR(temp1_max_hyst, S_IRUGO | S_IWUSR,
		show_temp_hyst, set_temp_hyst);

/* 2 Fans */
static ssize_t show_fan(struct device *dev, struct device_attribute *da,
=======
static DEVICE_ATTR_RO(temp1_input);
static DEVICE_ATTR_RW(temp1_max);
static DEVICE_ATTR_RW(temp1_max_hyst);

/* 2 Fans */
static ssize_t fan_show(struct device *dev, struct device_attribute *da,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			char *buf)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	return sprintf(buf, "%d\n", FAN_FROM_REG(data->fan[nr],
		DIV_FROM_REG(data->fan_div[nr])));
}

<<<<<<< HEAD
static ssize_t show_fan_min(struct device *dev, struct device_attribute *da,
=======
static ssize_t fan_min_show(struct device *dev, struct device_attribute *da,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    char *buf)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	return sprintf(buf, "%d\n", FAN_FROM_REG(data->fan_min[nr],
		DIV_FROM_REG(data->fan_div[nr])));
}

<<<<<<< HEAD
static ssize_t set_fan_min(struct device *dev, struct device_attribute *da,
			   const char *buf, size_t count)
=======
static ssize_t fan_min_store(struct device *dev, struct device_attribute *da,
			     const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sis5595_data *data = dev_get_drvdata(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;

	mutex_lock(&data->update_lock);
	data->fan_min[nr] = FAN_TO_REG(val, DIV_FROM_REG(data->fan_div[nr]));
	sis5595_write_value(data, SIS5595_REG_FAN_MIN(nr), data->fan_min[nr]);
	mutex_unlock(&data->update_lock);
	return count;
}

<<<<<<< HEAD
static ssize_t show_fan_div(struct device *dev, struct device_attribute *da,
=======
static ssize_t fan_div_show(struct device *dev, struct device_attribute *da,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    char *buf)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	return sprintf(buf, "%d\n", DIV_FROM_REG(data->fan_div[nr]));
}

/*
 * Note: we save and restore the fan minimum here, because its value is
 * determined in part by the fan divisor.  This follows the principle of
 * least surprise; the user doesn't expect the fan minimum to change just
 * because the divisor changed.
 */
<<<<<<< HEAD
static ssize_t set_fan_div(struct device *dev, struct device_attribute *da,
			   const char *buf, size_t count)
=======
static ssize_t fan_div_store(struct device *dev, struct device_attribute *da,
			     const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sis5595_data *data = dev_get_drvdata(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int nr = attr->index;
	unsigned long min;
	int reg;
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;

	mutex_lock(&data->update_lock);
	min = FAN_FROM_REG(data->fan_min[nr],
			DIV_FROM_REG(data->fan_div[nr]));
	reg = sis5595_read_value(data, SIS5595_REG_FANDIV);

	switch (val) {
	case 1:
		data->fan_div[nr] = 0;
		break;
	case 2:
		data->fan_div[nr] = 1;
		break;
	case 4:
		data->fan_div[nr] = 2;
		break;
	case 8:
		data->fan_div[nr] = 3;
		break;
	default:
<<<<<<< HEAD
		dev_err(dev, "fan_div value %ld not "
			"supported. Choose one of 1, 2, 4 or 8!\n", val);
=======
		dev_err(dev,
			"fan_div value %ld not supported. Choose one of 1, 2, 4 or 8!\n",
			val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mutex_unlock(&data->update_lock);
		return -EINVAL;
	}

	switch (nr) {
	case 0:
		reg = (reg & 0xcf) | (data->fan_div[nr] << 4);
		break;
	case 1:
		reg = (reg & 0x3f) | (data->fan_div[nr] << 6);
		break;
	}
	sis5595_write_value(data, SIS5595_REG_FANDIV, reg);
	data->fan_min[nr] =
		FAN_TO_REG(min, DIV_FROM_REG(data->fan_div[nr]));
	sis5595_write_value(data, SIS5595_REG_FAN_MIN(nr), data->fan_min[nr]);
	mutex_unlock(&data->update_lock);
	return count;
}

<<<<<<< HEAD
#define show_fan_offset(offset)						\
static SENSOR_DEVICE_ATTR(fan##offset##_input, S_IRUGO,			\
		show_fan, NULL, offset - 1);				\
static SENSOR_DEVICE_ATTR(fan##offset##_min, S_IRUGO | S_IWUSR,		\
		show_fan_min, set_fan_min, offset - 1);			\
static SENSOR_DEVICE_ATTR(fan##offset##_div, S_IRUGO | S_IWUSR,		\
		show_fan_div, set_fan_div, offset - 1);

show_fan_offset(1);
show_fan_offset(2);

/* Alarms */
static ssize_t show_alarms(struct device *dev, struct device_attribute *attr,
=======
static SENSOR_DEVICE_ATTR_RO(fan1_input, fan, 0);
static SENSOR_DEVICE_ATTR_RW(fan1_min, fan_min, 0);
static SENSOR_DEVICE_ATTR_RW(fan1_div, fan_div, 0);
static SENSOR_DEVICE_ATTR_RO(fan2_input, fan, 1);
static SENSOR_DEVICE_ATTR_RW(fan2_min, fan_min, 1);
static SENSOR_DEVICE_ATTR_RW(fan2_div, fan_div, 1);

/* Alarms */
static ssize_t alarms_show(struct device *dev, struct device_attribute *attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   char *buf)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	return sprintf(buf, "%d\n", data->alarms);
}
<<<<<<< HEAD
static DEVICE_ATTR(alarms, S_IRUGO, show_alarms, NULL);

static ssize_t show_alarm(struct device *dev, struct device_attribute *da,
=======
static DEVICE_ATTR_RO(alarms);

static ssize_t alarm_show(struct device *dev, struct device_attribute *da,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  char *buf)
{
	struct sis5595_data *data = sis5595_update_device(dev);
	int nr = to_sensor_dev_attr(da)->index;
	return sprintf(buf, "%u\n", (data->alarms >> nr) & 1);
}
<<<<<<< HEAD
static SENSOR_DEVICE_ATTR(in0_alarm, S_IRUGO, show_alarm, NULL, 0);
static SENSOR_DEVICE_ATTR(in1_alarm, S_IRUGO, show_alarm, NULL, 1);
static SENSOR_DEVICE_ATTR(in2_alarm, S_IRUGO, show_alarm, NULL, 2);
static SENSOR_DEVICE_ATTR(in3_alarm, S_IRUGO, show_alarm, NULL, 3);
static SENSOR_DEVICE_ATTR(in4_alarm, S_IRUGO, show_alarm, NULL, 15);
static SENSOR_DEVICE_ATTR(fan1_alarm, S_IRUGO, show_alarm, NULL, 6);
static SENSOR_DEVICE_ATTR(fan2_alarm, S_IRUGO, show_alarm, NULL, 7);
static SENSOR_DEVICE_ATTR(temp1_alarm, S_IRUGO, show_alarm, NULL, 15);

static ssize_t show_name(struct device *dev, struct device_attribute *attr,
=======
static SENSOR_DEVICE_ATTR_RO(in0_alarm, alarm, 0);
static SENSOR_DEVICE_ATTR_RO(in1_alarm, alarm, 1);
static SENSOR_DEVICE_ATTR_RO(in2_alarm, alarm, 2);
static SENSOR_DEVICE_ATTR_RO(in3_alarm, alarm, 3);
static SENSOR_DEVICE_ATTR_RO(in4_alarm, alarm, 15);
static SENSOR_DEVICE_ATTR_RO(fan1_alarm, alarm, 6);
static SENSOR_DEVICE_ATTR_RO(fan2_alarm, alarm, 7);
static SENSOR_DEVICE_ATTR_RO(temp1_alarm, alarm, 15);

static ssize_t name_show(struct device *dev, struct device_attribute *attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 char *buf)
{
	struct sis5595_data *data = dev_get_drvdata(dev);
	return sprintf(buf, "%s\n", data->name);
}
<<<<<<< HEAD
static DEVICE_ATTR(name, S_IRUGO, show_name, NULL);
=======
static DEVICE_ATTR_RO(name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *sis5595_attributes[] = {
	&sensor_dev_attr_in0_input.dev_attr.attr,
	&sensor_dev_attr_in0_min.dev_attr.attr,
	&sensor_dev_attr_in0_max.dev_attr.attr,
	&sensor_dev_attr_in0_alarm.dev_attr.attr,
	&sensor_dev_attr_in1_input.dev_attr.attr,
	&sensor_dev_attr_in1_min.dev_attr.attr,
	&sensor_dev_attr_in1_max.dev_attr.attr,
	&sensor_dev_attr_in1_alarm.dev_attr.attr,
	&sensor_dev_attr_in2_input.dev_attr.attr,
	&sensor_dev_attr_in2_min.dev_attr.attr,
	&sensor_dev_attr_in2_max.dev_attr.attr,
	&sensor_dev_attr_in2_alarm.dev_attr.attr,
	&sensor_dev_attr_in3_input.dev_attr.attr,
	&sensor_dev_attr_in3_min.dev_attr.attr,
	&sensor_dev_attr_in3_max.dev_attr.attr,
	&sensor_dev_attr_in3_alarm.dev_attr.attr,

	&sensor_dev_attr_fan1_input.dev_attr.attr,
	&sensor_dev_attr_fan1_min.dev_attr.attr,
	&sensor_dev_attr_fan1_div.dev_attr.attr,
	&sensor_dev_attr_fan1_alarm.dev_attr.attr,
	&sensor_dev_attr_fan2_input.dev_attr.attr,
	&sensor_dev_attr_fan2_min.dev_attr.attr,
	&sensor_dev_attr_fan2_div.dev_attr.attr,
	&sensor_dev_attr_fan2_alarm.dev_attr.attr,

	&dev_attr_alarms.attr,
	&dev_attr_name.attr,
	NULL
};

static const struct attribute_group sis5595_group = {
	.attrs = sis5595_attributes,
};

static struct attribute *sis5595_attributes_in4[] = {
	&sensor_dev_attr_in4_input.dev_attr.attr,
	&sensor_dev_attr_in4_min.dev_attr.attr,
	&sensor_dev_attr_in4_max.dev_attr.attr,
	&sensor_dev_attr_in4_alarm.dev_attr.attr,
	NULL
};

static const struct attribute_group sis5595_group_in4 = {
	.attrs = sis5595_attributes_in4,
};

static struct attribute *sis5595_attributes_temp1[] = {
	&dev_attr_temp1_input.attr,
	&dev_attr_temp1_max.attr,
	&dev_attr_temp1_max_hyst.attr,
	&sensor_dev_attr_temp1_alarm.dev_attr.attr,
	NULL
};

static const struct attribute_group sis5595_group_temp1 = {
	.attrs = sis5595_attributes_temp1,
};

<<<<<<< HEAD
/* This is called when the module is loaded */
static int __devinit sis5595_probe(struct platform_device *pdev)
=======
/* Called when we have found a new SIS5595. */
static void sis5595_init_device(struct sis5595_data *data)
{
	u8 config = sis5595_read_value(data, SIS5595_REG_CONFIG);
	if (!(config & 0x01))
		sis5595_write_value(data, SIS5595_REG_CONFIG,
				(config & 0xf7) | 0x01);
}

/* This is called when the module is loaded */
static int sis5595_probe(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err = 0;
	int i;
	struct sis5595_data *data;
	struct resource *res;
	char val;

	/* Reserve the ISA region */
	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
<<<<<<< HEAD
	if (!request_region(res->start, SIS5595_EXTENT,
			    sis5595_driver.driver.name)) {
		err = -EBUSY;
		goto exit;
	}

	data = kzalloc(sizeof(struct sis5595_data), GFP_KERNEL);
	if (!data) {
		err = -ENOMEM;
		goto exit_release;
	}
=======
	if (!devm_request_region(&pdev->dev, res->start, SIS5595_EXTENT,
				 DRIVER_NAME))
		return -EBUSY;

	data = devm_kzalloc(&pdev->dev, sizeof(struct sis5595_data),
			    GFP_KERNEL);
	if (!data)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_init(&data->lock);
	mutex_init(&data->update_lock);
	data->addr = res->start;
<<<<<<< HEAD
	data->name = "sis5595";
=======
	data->name = DRIVER_NAME;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	platform_set_drvdata(pdev, data);

	/*
	 * Check revision and pin registers to determine whether 4 or 5 voltages
	 */
	data->revision = s_bridge->revision;
	/* 4 voltages, 1 temp */
	data->maxins = 3;
	if (data->revision >= REV2MIN) {
		pci_read_config_byte(s_bridge, SIS5595_PIN_REG, &val);
		if (!(val & 0x80))
			/* 5 voltages, no temps */
			data->maxins = 4;
	}

	/* Initialize the SIS5595 chip */
	sis5595_init_device(data);

	/* A few vars need to be filled upon startup */
	for (i = 0; i < 2; i++) {
		data->fan_min[i] = sis5595_read_value(data,
					SIS5595_REG_FAN_MIN(i));
	}

	/* Register sysfs hooks */
	err = sysfs_create_group(&pdev->dev.kobj, &sis5595_group);
	if (err)
<<<<<<< HEAD
		goto exit_free;
=======
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (data->maxins == 4) {
		err = sysfs_create_group(&pdev->dev.kobj, &sis5595_group_in4);
		if (err)
			goto exit_remove_files;
	} else {
		err = sysfs_create_group(&pdev->dev.kobj, &sis5595_group_temp1);
		if (err)
			goto exit_remove_files;
	}

	data->hwmon_dev = hwmon_device_register(&pdev->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		goto exit_remove_files;
	}

	return 0;

exit_remove_files:
	sysfs_remove_group(&pdev->dev.kobj, &sis5595_group);
	sysfs_remove_group(&pdev->dev.kobj, &sis5595_group_in4);
	sysfs_remove_group(&pdev->dev.kobj, &sis5595_group_temp1);
<<<<<<< HEAD
exit_free:
	kfree(data);
exit_release:
	release_region(res->start, SIS5595_EXTENT);
exit:
	return err;
}

static int __devexit sis5595_remove(struct platform_device *pdev)
=======
	return err;
}

static void sis5595_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sis5595_data *data = platform_get_drvdata(pdev);

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&pdev->dev.kobj, &sis5595_group);
	sysfs_remove_group(&pdev->dev.kobj, &sis5595_group_in4);
	sysfs_remove_group(&pdev->dev.kobj, &sis5595_group_temp1);
<<<<<<< HEAD

	release_region(data->addr, SIS5595_EXTENT);
	platform_set_drvdata(pdev, NULL);
	kfree(data);

	return 0;
}


/* ISA access must be locked explicitly. */
static int sis5595_read_value(struct sis5595_data *data, u8 reg)
{
	int res;

	mutex_lock(&data->lock);
	outb_p(reg, data->addr + SIS5595_ADDR_REG_OFFSET);
	res = inb_p(data->addr + SIS5595_DATA_REG_OFFSET);
	mutex_unlock(&data->lock);
	return res;
}

static void sis5595_write_value(struct sis5595_data *data, u8 reg, u8 value)
{
	mutex_lock(&data->lock);
	outb_p(reg, data->addr + SIS5595_ADDR_REG_OFFSET);
	outb_p(value, data->addr + SIS5595_DATA_REG_OFFSET);
	mutex_unlock(&data->lock);
}

/* Called when we have found a new SIS5595. */
static void __devinit sis5595_init_device(struct sis5595_data *data)
{
	u8 config = sis5595_read_value(data, SIS5595_REG_CONFIG);
	if (!(config & 0x01))
		sis5595_write_value(data, SIS5595_REG_CONFIG,
				(config & 0xf7) | 0x01);
}

static struct sis5595_data *sis5595_update_device(struct device *dev)
{
	struct sis5595_data *data = dev_get_drvdata(dev);
	int i;

	mutex_lock(&data->update_lock);

	if (time_after(jiffies, data->last_updated + HZ + HZ / 2)
	    || !data->valid) {

		for (i = 0; i <= data->maxins; i++) {
			data->in[i] =
			    sis5595_read_value(data, SIS5595_REG_IN(i));
			data->in_min[i] =
			    sis5595_read_value(data,
					       SIS5595_REG_IN_MIN(i));
			data->in_max[i] =
			    sis5595_read_value(data,
					       SIS5595_REG_IN_MAX(i));
		}
		for (i = 0; i < 2; i++) {
			data->fan[i] =
			    sis5595_read_value(data, SIS5595_REG_FAN(i));
			data->fan_min[i] =
			    sis5595_read_value(data,
					       SIS5595_REG_FAN_MIN(i));
		}
		if (data->maxins == 3) {
			data->temp =
			    sis5595_read_value(data, SIS5595_REG_TEMP);
			data->temp_over =
			    sis5595_read_value(data, SIS5595_REG_TEMP_OVER);
			data->temp_hyst =
			    sis5595_read_value(data, SIS5595_REG_TEMP_HYST);
		}
		i = sis5595_read_value(data, SIS5595_REG_FANDIV);
		data->fan_div[0] = (i >> 4) & 0x03;
		data->fan_div[1] = i >> 6;
		data->alarms =
		    sis5595_read_value(data, SIS5595_REG_ALARM1) |
		    (sis5595_read_value(data, SIS5595_REG_ALARM2) << 8);
		data->last_updated = jiffies;
		data->valid = 1;
	}

	mutex_unlock(&data->update_lock);

	return data;
}

static DEFINE_PCI_DEVICE_TABLE(sis5595_pci_ids) = {
=======
}

static const struct pci_device_id sis5595_pci_ids[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ PCI_DEVICE(PCI_VENDOR_ID_SI, PCI_DEVICE_ID_SI_503) },
	{ 0, }
};

MODULE_DEVICE_TABLE(pci, sis5595_pci_ids);

<<<<<<< HEAD
static int blacklist[] __devinitdata = {
=======
static int blacklist[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	PCI_DEVICE_ID_SI_540,
	PCI_DEVICE_ID_SI_550,
	PCI_DEVICE_ID_SI_630,
	PCI_DEVICE_ID_SI_645,
	PCI_DEVICE_ID_SI_730,
	PCI_DEVICE_ID_SI_735,
	PCI_DEVICE_ID_SI_5511, /*
				* 5513 chip has the 0008 device but
				* that ID shows up in other chips so we
				* use the 5511 ID for recognition
				*/
	PCI_DEVICE_ID_SI_5597,
	PCI_DEVICE_ID_SI_5598,
	0 };

<<<<<<< HEAD
static int __devinit sis5595_device_add(unsigned short address)
=======
static int sis5595_device_add(unsigned short address)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct resource res = {
		.start	= address,
		.end	= address + SIS5595_EXTENT - 1,
<<<<<<< HEAD
		.name	= "sis5595",
=======
		.name	= DRIVER_NAME,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.flags	= IORESOURCE_IO,
	};
	int err;

	err = acpi_check_resource_conflict(&res);
	if (err)
		goto exit;

<<<<<<< HEAD
	pdev = platform_device_alloc("sis5595", address);
=======
	pdev = platform_device_alloc(DRIVER_NAME, address);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!pdev) {
		err = -ENOMEM;
		pr_err("Device allocation failed\n");
		goto exit;
	}

	err = platform_device_add_resources(pdev, &res, 1);
	if (err) {
		pr_err("Device resource addition failed (%d)\n", err);
		goto exit_device_put;
	}

	err = platform_device_add(pdev);
	if (err) {
		pr_err("Device addition failed (%d)\n", err);
		goto exit_device_put;
	}

	return 0;

exit_device_put:
	platform_device_put(pdev);
exit:
	return err;
}

<<<<<<< HEAD
static int __devinit sis5595_pci_probe(struct pci_dev *dev,
=======
static struct platform_driver sis5595_driver = {
	.driver = {
		.name	= DRIVER_NAME,
	},
	.probe		= sis5595_probe,
	.remove_new	= sis5595_remove,
};

static int sis5595_pci_probe(struct pci_dev *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				       const struct pci_device_id *id)
{
	u16 address;
	u8 enable;
<<<<<<< HEAD
	int *i;
=======
	int *i, err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = blacklist; *i != 0; i++) {
		struct pci_dev *d;
		d = pci_get_device(PCI_VENDOR_ID_SI, *i, NULL);
		if (d) {
			dev_err(&d->dev,
				"Looked for SIS5595 but found unsupported device %.4x\n",
				*i);
			pci_dev_put(d);
			return -ENODEV;
		}
	}

	force_addr &= ~(SIS5595_EXTENT - 1);
	if (force_addr) {
		dev_warn(&dev->dev, "Forcing ISA address 0x%x\n", force_addr);
		pci_write_config_word(dev, SIS5595_BASE_REG, force_addr);
	}

<<<<<<< HEAD
	if (PCIBIOS_SUCCESSFUL !=
	    pci_read_config_word(dev, SIS5595_BASE_REG, &address)) {
=======
	err = pci_read_config_word(dev, SIS5595_BASE_REG, &address);
	if (err != PCIBIOS_SUCCESSFUL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&dev->dev, "Failed to read ISA address\n");
		return -ENODEV;
	}

	address &= ~(SIS5595_EXTENT - 1);
	if (!address) {
		dev_err(&dev->dev,
			"Base address not set - upgrade BIOS or use force_addr=0xaddr\n");
		return -ENODEV;
	}
	if (force_addr && address != force_addr) {
		/* doesn't work for some chips? */
		dev_err(&dev->dev, "Failed to force ISA address\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	if (PCIBIOS_SUCCESSFUL !=
	    pci_read_config_byte(dev, SIS5595_ENABLE_REG, &enable)) {
=======
	err = pci_read_config_byte(dev, SIS5595_ENABLE_REG, &enable);
	if (err != PCIBIOS_SUCCESSFUL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&dev->dev, "Failed to read enable register\n");
		return -ENODEV;
	}
	if (!(enable & 0x80)) {
<<<<<<< HEAD
		if ((PCIBIOS_SUCCESSFUL !=
		     pci_write_config_byte(dev, SIS5595_ENABLE_REG,
					   enable | 0x80))
		 || (PCIBIOS_SUCCESSFUL !=
		     pci_read_config_byte(dev, SIS5595_ENABLE_REG, &enable))
		 || (!(enable & 0x80))) {
			/* doesn't work for some chips! */
			dev_err(&dev->dev, "Failed to enable HWM device\n");
			return -ENODEV;
		}
=======
		err = pci_write_config_byte(dev, SIS5595_ENABLE_REG, enable | 0x80);
		if (err != PCIBIOS_SUCCESSFUL)
			goto enable_fail;

		err = pci_read_config_byte(dev, SIS5595_ENABLE_REG, &enable);
		if (err != PCIBIOS_SUCCESSFUL)
			goto enable_fail;

		/* doesn't work for some chips! */
		if (!(enable & 0x80))
			goto enable_fail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (platform_driver_register(&sis5595_driver)) {
		dev_dbg(&dev->dev, "Failed to register sis5595 driver\n");
		goto exit;
	}

	s_bridge = pci_dev_get(dev);
	/* Sets global pdev as a side effect */
	if (sis5595_device_add(address))
		goto exit_unregister;

	/*
	 * Always return failure here.  This is to allow other drivers to bind
	 * to this pci device.  We don't really want to have control over the
	 * pci device, we only wanted to read as few register values from it.
	 */
	return -ENODEV;

<<<<<<< HEAD
=======
enable_fail:
	dev_err(&dev->dev, "Failed to enable HWM device\n");
	goto exit;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
exit_unregister:
	pci_dev_put(dev);
	platform_driver_unregister(&sis5595_driver);
exit:
	return -ENODEV;
}

static struct pci_driver sis5595_pci_driver = {
<<<<<<< HEAD
	.name            = "sis5595",
=======
	.name            = DRIVER_NAME,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table        = sis5595_pci_ids,
	.probe           = sis5595_pci_probe,
};

static int __init sm_sis5595_init(void)
{
	return pci_register_driver(&sis5595_pci_driver);
}

static void __exit sm_sis5595_exit(void)
{
	pci_unregister_driver(&sis5595_pci_driver);
	if (s_bridge != NULL) {
		platform_device_unregister(pdev);
		platform_driver_unregister(&sis5595_driver);
		pci_dev_put(s_bridge);
		s_bridge = NULL;
	}
}

MODULE_AUTHOR("Aurelien Jarno <aurelien@aurel32.net>");
MODULE_DESCRIPTION("SiS 5595 Sensor device");
MODULE_LICENSE("GPL");

module_init(sm_sis5595_init);
module_exit(sm_sis5595_exit);
