<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Hardware monitoring driver for PMBus devices
 *
 * Copyright (c) 2010, 2011 Ericsson AB.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/kernel.h>
=======
 * Copyright (c) 2012 Guenter Roeck
 */

#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/math64.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/i2c/pmbus.h>
#include "pmbus.h"

/*
 * Constants needed to determine number of sensors, booleans, and labels.
 */
#define PMBUS_MAX_INPUT_SENSORS		22	/* 10*volt, 7*curr, 5*power */
#define PMBUS_VOUT_SENSORS_PER_PAGE	9	/* input, min, max, lcrit,
						   crit, lowest, highest, avg,
						   reset */
#define PMBUS_IOUT_SENSORS_PER_PAGE	8	/* input, min, max, crit,
						   lowest, highest, avg,
						   reset */
#define PMBUS_POUT_SENSORS_PER_PAGE	7	/* input, cap, max, crit,
						 * highest, avg, reset
						 */
#define PMBUS_MAX_SENSORS_PER_FAN	1	/* input */
#define PMBUS_MAX_SENSORS_PER_TEMP	9	/* input, min, max, lcrit,
						 * crit, lowest, highest, avg,
						 * reset
						 */

#define PMBUS_MAX_INPUT_BOOLEANS	7	/* v: min_alarm, max_alarm,
						   lcrit_alarm, crit_alarm;
						   c: alarm, crit_alarm;
						   p: crit_alarm */
#define PMBUS_VOUT_BOOLEANS_PER_PAGE	4	/* min_alarm, max_alarm,
						   lcrit_alarm, crit_alarm */
#define PMBUS_IOUT_BOOLEANS_PER_PAGE	3	/* alarm, lcrit_alarm,
						   crit_alarm */
#define PMBUS_POUT_BOOLEANS_PER_PAGE	3	/* cap_alarm, alarm, crit_alarm
						 */
#define PMBUS_MAX_BOOLEANS_PER_FAN	2	/* alarm, fault */
#define PMBUS_MAX_BOOLEANS_PER_TEMP	4	/* min_alarm, max_alarm,
						   lcrit_alarm, crit_alarm */

#define PMBUS_MAX_INPUT_LABELS		4	/* vin, vcap, iin, pin */

/*
 * status, status_vout, status_iout, status_fans, status_fan34, and status_temp
 * are paged. status_input is unpaged.
 */
#define PB_NUM_STATUS_REG	(PMBUS_PAGES * 6 + 1)

/*
 * Index into status register array, per status register group
 */
#define PB_STATUS_BASE		0
#define PB_STATUS_VOUT_BASE	(PB_STATUS_BASE + PMBUS_PAGES)
#define PB_STATUS_IOUT_BASE	(PB_STATUS_VOUT_BASE + PMBUS_PAGES)
#define PB_STATUS_FAN_BASE	(PB_STATUS_IOUT_BASE + PMBUS_PAGES)
#define PB_STATUS_FAN34_BASE	(PB_STATUS_FAN_BASE + PMBUS_PAGES)
#define PB_STATUS_INPUT_BASE	(PB_STATUS_FAN34_BASE + PMBUS_PAGES)
#define PB_STATUS_TEMP_BASE	(PB_STATUS_INPUT_BASE + 1)

#define PMBUS_NAME_SIZE		24

struct pmbus_sensor {
	char name[PMBUS_NAME_SIZE];	/* sysfs sensor name */
	struct sensor_device_attribute attribute;
	u8 page;		/* page number */
	u16 reg;		/* register */
	enum pmbus_sensor_classes class;	/* sensor class */
	bool update;		/* runtime sensor update needed */
	int data;		/* Sensor data.
				   Negative if there was a read error */
};
=======
#include <linux/pmbus.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/of.h>
#include <linux/thermal.h>
#include "pmbus.h"

/*
 * Number of additional attribute pointers to allocate
 * with each call to krealloc
 */
#define PMBUS_ATTR_ALLOC_SIZE	32
#define PMBUS_NAME_SIZE		24

struct pmbus_sensor {
	struct pmbus_sensor *next;
	char name[PMBUS_NAME_SIZE];	/* sysfs sensor name */
	struct device_attribute attribute;
	u8 page;		/* page number */
	u8 phase;		/* phase number, 0xff for all phases */
	u16 reg;		/* register */
	enum pmbus_sensor_classes class;	/* sensor class */
	bool update;		/* runtime sensor update needed */
	bool convert;		/* Whether or not to apply linear/vid/direct */
	int data;		/* Sensor data.
				   Negative if there was a read error */
};
#define to_pmbus_sensor(_attr) \
	container_of(_attr, struct pmbus_sensor, attribute)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct pmbus_boolean {
	char name[PMBUS_NAME_SIZE];	/* sysfs boolean name */
	struct sensor_device_attribute attribute;
<<<<<<< HEAD
};

struct pmbus_label {
	char name[PMBUS_NAME_SIZE];	/* sysfs label name */
	struct sensor_device_attribute attribute;
	char label[PMBUS_NAME_SIZE];	/* label */
};

struct pmbus_data {
	struct device *hwmon_dev;

	u32 flags;		/* from platform data */

	int exponent;		/* linear mode: exponent for output voltages */
=======
	struct pmbus_sensor *s1;
	struct pmbus_sensor *s2;
};
#define to_pmbus_boolean(_attr) \
	container_of(_attr, struct pmbus_boolean, attribute)

struct pmbus_label {
	char name[PMBUS_NAME_SIZE];	/* sysfs label name */
	struct device_attribute attribute;
	char label[PMBUS_NAME_SIZE];	/* label */
};
#define to_pmbus_label(_attr) \
	container_of(_attr, struct pmbus_label, attribute)

/* Macros for converting between sensor index and register/page/status mask */

#define PB_STATUS_MASK	0xffff
#define PB_REG_SHIFT	16
#define PB_REG_MASK	0x3ff
#define PB_PAGE_SHIFT	26
#define PB_PAGE_MASK	0x3f

#define pb_reg_to_index(page, reg, mask)	(((page) << PB_PAGE_SHIFT) | \
						 ((reg) << PB_REG_SHIFT) | (mask))

#define pb_index_to_page(index)			(((index) >> PB_PAGE_SHIFT) & PB_PAGE_MASK)
#define pb_index_to_reg(index)			(((index) >> PB_REG_SHIFT) & PB_REG_MASK)
#define pb_index_to_mask(index)			((index) & PB_STATUS_MASK)

struct pmbus_data {
	struct device *dev;
	struct device *hwmon_dev;
	struct regulator_dev **rdevs;

	u32 flags;		/* from platform data */

	int exponent[PMBUS_PAGES];
				/* linear mode: exponent for output voltages */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	const struct pmbus_driver_info *info;

	int max_attributes;
	int num_attributes;
<<<<<<< HEAD
	struct attribute **attributes;
	struct attribute_group group;

	/*
	 * Sensors cover both sensor and limit registers.
	 */
	int max_sensors;
	int num_sensors;
	struct pmbus_sensor *sensors;
	/*
	 * Booleans are used for alarms.
	 * Values are determined from status registers.
	 */
	int max_booleans;
	int num_booleans;
	struct pmbus_boolean *booleans;
	/*
	 * Labels are used to map generic names (e.g., "in1")
	 * to PMBus specific names (e.g., "vin" or "vout1").
	 */
	int max_labels;
	int num_labels;
	struct pmbus_label *labels;

	struct mutex update_lock;
	bool valid;
	unsigned long last_updated;	/* in jiffies */

	/*
	 * A single status register covers multiple attributes,
	 * so we keep them all together.
	 */
	u8 status[PB_NUM_STATUS_REG];

	u8 currpage;
};

int pmbus_set_page(struct i2c_client *client, u8 page)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	int rv = 0;
	int newpage;

	if (page != data->currpage) {
		rv = i2c_smbus_write_byte_data(client, PMBUS_PAGE, page);
		newpage = i2c_smbus_read_byte_data(client, PMBUS_PAGE);
		if (newpage != page)
			rv = -EIO;
		else
			data->currpage = page;
	}
	return rv;
}
EXPORT_SYMBOL_GPL(pmbus_set_page);
=======
	struct attribute_group group;
	const struct attribute_group **groups;
	struct dentry *debugfs;		/* debugfs device directory */

	struct pmbus_sensor *sensors;

	struct mutex update_lock;

	bool has_status_word;		/* device uses STATUS_WORD register */
	int (*read_status)(struct i2c_client *client, int page);

	s16 currpage;	/* current page, -1 for unknown/unset */
	s16 currphase;	/* current phase, 0xff for all, -1 for unknown/unset */

	int vout_low[PMBUS_PAGES];	/* voltage low margin */
	int vout_high[PMBUS_PAGES];	/* voltage high margin */
};

struct pmbus_debugfs_entry {
	struct i2c_client *client;
	u8 page;
	u8 reg;
};

static const int pmbus_fan_rpm_mask[] = {
	PB_FAN_1_RPM,
	PB_FAN_2_RPM,
	PB_FAN_1_RPM,
	PB_FAN_2_RPM,
};

static const int pmbus_fan_config_registers[] = {
	PMBUS_FAN_CONFIG_12,
	PMBUS_FAN_CONFIG_12,
	PMBUS_FAN_CONFIG_34,
	PMBUS_FAN_CONFIG_34
};

static const int pmbus_fan_command_registers[] = {
	PMBUS_FAN_COMMAND_1,
	PMBUS_FAN_COMMAND_2,
	PMBUS_FAN_COMMAND_3,
	PMBUS_FAN_COMMAND_4,
};

void pmbus_clear_cache(struct i2c_client *client)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct pmbus_sensor *sensor;

	for (sensor = data->sensors; sensor; sensor = sensor->next)
		sensor->data = -ENODATA;
}
EXPORT_SYMBOL_NS_GPL(pmbus_clear_cache, PMBUS);

void pmbus_set_update(struct i2c_client *client, u8 reg, bool update)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct pmbus_sensor *sensor;

	for (sensor = data->sensors; sensor; sensor = sensor->next)
		if (sensor->reg == reg)
			sensor->update = update;
}
EXPORT_SYMBOL_NS_GPL(pmbus_set_update, PMBUS);

int pmbus_set_page(struct i2c_client *client, int page, int phase)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	int rv;

	if (page < 0)
		return 0;

	if (!(data->info->func[page] & PMBUS_PAGE_VIRTUAL) &&
	    data->info->pages > 1 && page != data->currpage) {
		rv = i2c_smbus_write_byte_data(client, PMBUS_PAGE, page);
		if (rv < 0)
			return rv;

		rv = i2c_smbus_read_byte_data(client, PMBUS_PAGE);
		if (rv < 0)
			return rv;

		if (rv != page)
			return -EIO;
	}
	data->currpage = page;

	if (data->info->phases[page] && data->currphase != phase &&
	    !(data->info->func[page] & PMBUS_PHASE_VIRTUAL)) {
		rv = i2c_smbus_write_byte_data(client, PMBUS_PHASE,
					       phase);
		if (rv)
			return rv;
	}
	data->currphase = phase;

	return 0;
}
EXPORT_SYMBOL_NS_GPL(pmbus_set_page, PMBUS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int pmbus_write_byte(struct i2c_client *client, int page, u8 value)
{
	int rv;

<<<<<<< HEAD
	if (page >= 0) {
		rv = pmbus_set_page(client, page);
		if (rv < 0)
			return rv;
	}

	return i2c_smbus_write_byte(client, value);
}
EXPORT_SYMBOL_GPL(pmbus_write_byte);

/*
 * _pmbus_write_byte() is similar to pmbus_write_byte(), but checks if
 * a device specific mapping funcion exists and calls it if necessary.
=======
	rv = pmbus_set_page(client, page, 0xff);
	if (rv < 0)
		return rv;

	return i2c_smbus_write_byte(client, value);
}
EXPORT_SYMBOL_NS_GPL(pmbus_write_byte, PMBUS);

/*
 * _pmbus_write_byte() is similar to pmbus_write_byte(), but checks if
 * a device specific mapping function exists and calls it if necessary.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int _pmbus_write_byte(struct i2c_client *client, int page, u8 value)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	const struct pmbus_driver_info *info = data->info;
	int status;

	if (info->write_byte) {
		status = info->write_byte(client, page, value);
		if (status != -ENODATA)
			return status;
	}
	return pmbus_write_byte(client, page, value);
}

<<<<<<< HEAD
int pmbus_write_word_data(struct i2c_client *client, u8 page, u8 reg, u16 word)
{
	int rv;

	rv = pmbus_set_page(client, page);
=======
int pmbus_write_word_data(struct i2c_client *client, int page, u8 reg,
			  u16 word)
{
	int rv;

	rv = pmbus_set_page(client, page, 0xff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rv < 0)
		return rv;

	return i2c_smbus_write_word_data(client, reg, word);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(pmbus_write_word_data);
=======
EXPORT_SYMBOL_NS_GPL(pmbus_write_word_data, PMBUS);


static int pmbus_write_virt_reg(struct i2c_client *client, int page, int reg,
				u16 word)
{
	int bit;
	int id;
	int rv;

	switch (reg) {
	case PMBUS_VIRT_FAN_TARGET_1 ... PMBUS_VIRT_FAN_TARGET_4:
		id = reg - PMBUS_VIRT_FAN_TARGET_1;
		bit = pmbus_fan_rpm_mask[id];
		rv = pmbus_update_fan(client, page, id, bit, bit, word);
		break;
	default:
		rv = -ENXIO;
		break;
	}

	return rv;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * _pmbus_write_word_data() is similar to pmbus_write_word_data(), but checks if
 * a device specific mapping function exists and calls it if necessary.
 */
static int _pmbus_write_word_data(struct i2c_client *client, int page, int reg,
				  u16 word)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	const struct pmbus_driver_info *info = data->info;
	int status;

	if (info->write_word_data) {
		status = info->write_word_data(client, page, reg, word);
		if (status != -ENODATA)
			return status;
	}
<<<<<<< HEAD
	if (reg >= PMBUS_VIRT_BASE)
		return -ENXIO;
	return pmbus_write_word_data(client, page, reg, word);
}

int pmbus_read_word_data(struct i2c_client *client, u8 page, u8 reg)
{
	int rv;

	rv = pmbus_set_page(client, page);
	if (rv < 0)
		return rv;

	return i2c_smbus_read_word_data(client, reg);
}
EXPORT_SYMBOL_GPL(pmbus_read_word_data);

/*
 * _pmbus_read_word_data() is similar to pmbus_read_word_data(), but checks if
 * a device specific mapping function exists and calls it if necessary.
 */
static int _pmbus_read_word_data(struct i2c_client *client, int page, int reg)
=======

	if (reg >= PMBUS_VIRT_BASE)
		return pmbus_write_virt_reg(client, page, reg, word);

	return pmbus_write_word_data(client, page, reg, word);
}

/*
 * _pmbus_write_byte_data() is similar to pmbus_write_byte_data(), but checks if
 * a device specific mapping function exists and calls it if necessary.
 */
static int _pmbus_write_byte_data(struct i2c_client *client, int page, int reg, u8 value)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	const struct pmbus_driver_info *info = data->info;
	int status;

<<<<<<< HEAD
	if (info->read_word_data) {
		status = info->read_word_data(client, page, reg);
		if (status != -ENODATA)
			return status;
	}
	if (reg >= PMBUS_VIRT_BASE)
		return -ENXIO;
	return pmbus_read_word_data(client, page, reg);
}

int pmbus_read_byte_data(struct i2c_client *client, int page, u8 reg)
{
	int rv;

	if (page >= 0) {
		rv = pmbus_set_page(client, page);
		if (rv < 0)
			return rv;
	}

	return i2c_smbus_read_byte_data(client, reg);
}
EXPORT_SYMBOL_GPL(pmbus_read_byte_data);

=======
	if (info->write_byte_data) {
		status = info->write_byte_data(client, page, reg, value);
		if (status != -ENODATA)
			return status;
	}
	return pmbus_write_byte_data(client, page, reg, value);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * _pmbus_read_byte_data() is similar to pmbus_read_byte_data(), but checks if
 * a device specific mapping function exists and calls it if necessary.
 */
static int _pmbus_read_byte_data(struct i2c_client *client, int page, int reg)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	const struct pmbus_driver_info *info = data->info;
	int status;

	if (info->read_byte_data) {
		status = info->read_byte_data(client, page, reg);
		if (status != -ENODATA)
			return status;
	}
	return pmbus_read_byte_data(client, page, reg);
}

<<<<<<< HEAD
=======
int pmbus_update_fan(struct i2c_client *client, int page, int id,
		     u8 config, u8 mask, u16 command)
{
	int from;
	int rv;
	u8 to;

	from = _pmbus_read_byte_data(client, page,
				    pmbus_fan_config_registers[id]);
	if (from < 0)
		return from;

	to = (from & ~mask) | (config & mask);
	if (to != from) {
		rv = _pmbus_write_byte_data(client, page,
					   pmbus_fan_config_registers[id], to);
		if (rv < 0)
			return rv;
	}

	return _pmbus_write_word_data(client, page,
				      pmbus_fan_command_registers[id], command);
}
EXPORT_SYMBOL_NS_GPL(pmbus_update_fan, PMBUS);

int pmbus_read_word_data(struct i2c_client *client, int page, int phase, u8 reg)
{
	int rv;

	rv = pmbus_set_page(client, page, phase);
	if (rv < 0)
		return rv;

	return i2c_smbus_read_word_data(client, reg);
}
EXPORT_SYMBOL_NS_GPL(pmbus_read_word_data, PMBUS);

static int pmbus_read_virt_reg(struct i2c_client *client, int page, int reg)
{
	int rv;
	int id;

	switch (reg) {
	case PMBUS_VIRT_FAN_TARGET_1 ... PMBUS_VIRT_FAN_TARGET_4:
		id = reg - PMBUS_VIRT_FAN_TARGET_1;
		rv = pmbus_get_fan_rate_device(client, page, id, rpm);
		break;
	default:
		rv = -ENXIO;
		break;
	}

	return rv;
}

/*
 * _pmbus_read_word_data() is similar to pmbus_read_word_data(), but checks if
 * a device specific mapping function exists and calls it if necessary.
 */
static int _pmbus_read_word_data(struct i2c_client *client, int page,
				 int phase, int reg)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	const struct pmbus_driver_info *info = data->info;
	int status;

	if (info->read_word_data) {
		status = info->read_word_data(client, page, phase, reg);
		if (status != -ENODATA)
			return status;
	}

	if (reg >= PMBUS_VIRT_BASE)
		return pmbus_read_virt_reg(client, page, reg);

	return pmbus_read_word_data(client, page, phase, reg);
}

/* Same as above, but without phase parameter, for use in check functions */
static int __pmbus_read_word_data(struct i2c_client *client, int page, int reg)
{
	return _pmbus_read_word_data(client, page, 0xff, reg);
}

int pmbus_read_byte_data(struct i2c_client *client, int page, u8 reg)
{
	int rv;

	rv = pmbus_set_page(client, page, 0xff);
	if (rv < 0)
		return rv;

	return i2c_smbus_read_byte_data(client, reg);
}
EXPORT_SYMBOL_NS_GPL(pmbus_read_byte_data, PMBUS);

int pmbus_write_byte_data(struct i2c_client *client, int page, u8 reg, u8 value)
{
	int rv;

	rv = pmbus_set_page(client, page, 0xff);
	if (rv < 0)
		return rv;

	return i2c_smbus_write_byte_data(client, reg, value);
}
EXPORT_SYMBOL_NS_GPL(pmbus_write_byte_data, PMBUS);

int pmbus_update_byte_data(struct i2c_client *client, int page, u8 reg,
			   u8 mask, u8 value)
{
	unsigned int tmp;
	int rv;

	rv = _pmbus_read_byte_data(client, page, reg);
	if (rv < 0)
		return rv;

	tmp = (rv & ~mask) | (value & mask);

	if (tmp != rv)
		rv = _pmbus_write_byte_data(client, page, reg, tmp);

	return rv;
}
EXPORT_SYMBOL_NS_GPL(pmbus_update_byte_data, PMBUS);

static int pmbus_read_block_data(struct i2c_client *client, int page, u8 reg,
				 char *data_buf)
{
	int rv;

	rv = pmbus_set_page(client, page, 0xff);
	if (rv < 0)
		return rv;

	return i2c_smbus_read_block_data(client, reg, data_buf);
}

static struct pmbus_sensor *pmbus_find_sensor(struct pmbus_data *data, int page,
					      int reg)
{
	struct pmbus_sensor *sensor;

	for (sensor = data->sensors; sensor; sensor = sensor->next) {
		if (sensor->page == page && sensor->reg == reg)
			return sensor;
	}

	return ERR_PTR(-EINVAL);
}

static int pmbus_get_fan_rate(struct i2c_client *client, int page, int id,
			      enum pmbus_fan_mode mode,
			      bool from_cache)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	bool want_rpm, have_rpm;
	struct pmbus_sensor *s;
	int config;
	int reg;

	want_rpm = (mode == rpm);

	if (from_cache) {
		reg = want_rpm ? PMBUS_VIRT_FAN_TARGET_1 : PMBUS_VIRT_PWM_1;
		s = pmbus_find_sensor(data, page, reg + id);
		if (IS_ERR(s))
			return PTR_ERR(s);

		return s->data;
	}

	config = _pmbus_read_byte_data(client, page,
				      pmbus_fan_config_registers[id]);
	if (config < 0)
		return config;

	have_rpm = !!(config & pmbus_fan_rpm_mask[id]);
	if (want_rpm == have_rpm)
		return pmbus_read_word_data(client, page, 0xff,
					    pmbus_fan_command_registers[id]);

	/* Can't sensibly map between RPM and PWM, just return zero */
	return 0;
}

int pmbus_get_fan_rate_device(struct i2c_client *client, int page, int id,
			      enum pmbus_fan_mode mode)
{
	return pmbus_get_fan_rate(client, page, id, mode, false);
}
EXPORT_SYMBOL_NS_GPL(pmbus_get_fan_rate_device, PMBUS);

int pmbus_get_fan_rate_cached(struct i2c_client *client, int page, int id,
			      enum pmbus_fan_mode mode)
{
	return pmbus_get_fan_rate(client, page, id, mode, true);
}
EXPORT_SYMBOL_NS_GPL(pmbus_get_fan_rate_cached, PMBUS);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void pmbus_clear_fault_page(struct i2c_client *client, int page)
{
	_pmbus_write_byte(client, page, PMBUS_CLEAR_FAULTS);
}

void pmbus_clear_faults(struct i2c_client *client)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	int i;

	for (i = 0; i < data->info->pages; i++)
		pmbus_clear_fault_page(client, i);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(pmbus_clear_faults);

static int pmbus_check_status_cml(struct i2c_client *client)
{
	int status, status2;

	status = _pmbus_read_byte_data(client, -1, PMBUS_STATUS_BYTE);
=======
EXPORT_SYMBOL_NS_GPL(pmbus_clear_faults, PMBUS);

static int pmbus_check_status_cml(struct i2c_client *client)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	int status, status2;

	status = data->read_status(client, -1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (status < 0 || (status & PB_STATUS_CML)) {
		status2 = _pmbus_read_byte_data(client, -1, PMBUS_STATUS_CML);
		if (status2 < 0 || (status2 & PB_CML_FAULT_INVALID_COMMAND))
			return -EIO;
	}
	return 0;
}

<<<<<<< HEAD
bool pmbus_check_byte_register(struct i2c_client *client, int page, int reg)
=======
static bool pmbus_check_register(struct i2c_client *client,
				 int (*func)(struct i2c_client *client,
					     int page, int reg),
				 int page, int reg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int rv;
	struct pmbus_data *data = i2c_get_clientdata(client);

<<<<<<< HEAD
	rv = _pmbus_read_byte_data(client, page, reg);
	if (rv >= 0 && !(data->flags & PMBUS_SKIP_STATUS_CHECK))
		rv = pmbus_check_status_cml(client);
	pmbus_clear_fault_page(client, -1);
	return rv >= 0;
}
EXPORT_SYMBOL_GPL(pmbus_check_byte_register);

bool pmbus_check_word_register(struct i2c_client *client, int page, int reg)
{
	int rv;
	struct pmbus_data *data = i2c_get_clientdata(client);

	rv = _pmbus_read_word_data(client, page, reg);
	if (rv >= 0 && !(data->flags & PMBUS_SKIP_STATUS_CHECK))
		rv = pmbus_check_status_cml(client);
	pmbus_clear_fault_page(client, -1);
	return rv >= 0;
}
EXPORT_SYMBOL_GPL(pmbus_check_word_register);
=======
	rv = func(client, page, reg);
	if (rv >= 0 && !(data->flags & PMBUS_SKIP_STATUS_CHECK))
		rv = pmbus_check_status_cml(client);
	if (rv < 0 && (data->flags & PMBUS_READ_STATUS_AFTER_FAILED_CHECK))
		data->read_status(client, -1);
	if (reg < PMBUS_VIRT_BASE)
		pmbus_clear_fault_page(client, -1);
	return rv >= 0;
}

static bool pmbus_check_status_register(struct i2c_client *client, int page)
{
	int status;
	struct pmbus_data *data = i2c_get_clientdata(client);

	status = data->read_status(client, page);
	if (status >= 0 && !(data->flags & PMBUS_SKIP_STATUS_CHECK) &&
	    (status & PB_STATUS_CML)) {
		status = _pmbus_read_byte_data(client, -1, PMBUS_STATUS_CML);
		if (status < 0 || (status & PB_CML_FAULT_INVALID_COMMAND))
			status = -EIO;
	}

	pmbus_clear_fault_page(client, -1);
	return status >= 0;
}

bool pmbus_check_byte_register(struct i2c_client *client, int page, int reg)
{
	return pmbus_check_register(client, _pmbus_read_byte_data, page, reg);
}
EXPORT_SYMBOL_NS_GPL(pmbus_check_byte_register, PMBUS);

bool pmbus_check_word_register(struct i2c_client *client, int page, int reg)
{
	return pmbus_check_register(client, __pmbus_read_word_data, page, reg);
}
EXPORT_SYMBOL_NS_GPL(pmbus_check_word_register, PMBUS);

static bool __maybe_unused pmbus_check_block_register(struct i2c_client *client,
						      int page, int reg)
{
	int rv;
	struct pmbus_data *data = i2c_get_clientdata(client);
	char data_buf[I2C_SMBUS_BLOCK_MAX + 2];

	rv = pmbus_read_block_data(client, page, reg, data_buf);
	if (rv >= 0 && !(data->flags & PMBUS_SKIP_STATUS_CHECK))
		rv = pmbus_check_status_cml(client);
	if (rv < 0 && (data->flags & PMBUS_READ_STATUS_AFTER_FAILED_CHECK))
		data->read_status(client, -1);
	pmbus_clear_fault_page(client, -1);
	return rv >= 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

const struct pmbus_driver_info *pmbus_get_driver_info(struct i2c_client *client)
{
	struct pmbus_data *data = i2c_get_clientdata(client);

	return data->info;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(pmbus_get_driver_info);

static struct pmbus_data *pmbus_update_device(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct pmbus_data *data = i2c_get_clientdata(client);
	const struct pmbus_driver_info *info = data->info;

	mutex_lock(&data->update_lock);
	if (time_after(jiffies, data->last_updated + HZ) || !data->valid) {
		int i;

		for (i = 0; i < info->pages; i++)
			data->status[PB_STATUS_BASE + i]
			    = _pmbus_read_byte_data(client, i,
						    PMBUS_STATUS_BYTE);
		for (i = 0; i < info->pages; i++) {
			if (!(info->func[i] & PMBUS_HAVE_STATUS_VOUT))
				continue;
			data->status[PB_STATUS_VOUT_BASE + i]
			  = _pmbus_read_byte_data(client, i, PMBUS_STATUS_VOUT);
		}
		for (i = 0; i < info->pages; i++) {
			if (!(info->func[i] & PMBUS_HAVE_STATUS_IOUT))
				continue;
			data->status[PB_STATUS_IOUT_BASE + i]
			  = _pmbus_read_byte_data(client, i, PMBUS_STATUS_IOUT);
		}
		for (i = 0; i < info->pages; i++) {
			if (!(info->func[i] & PMBUS_HAVE_STATUS_TEMP))
				continue;
			data->status[PB_STATUS_TEMP_BASE + i]
			  = _pmbus_read_byte_data(client, i,
						  PMBUS_STATUS_TEMPERATURE);
		}
		for (i = 0; i < info->pages; i++) {
			if (!(info->func[i] & PMBUS_HAVE_STATUS_FAN12))
				continue;
			data->status[PB_STATUS_FAN_BASE + i]
			  = _pmbus_read_byte_data(client, i,
						  PMBUS_STATUS_FAN_12);
		}

		for (i = 0; i < info->pages; i++) {
			if (!(info->func[i] & PMBUS_HAVE_STATUS_FAN34))
				continue;
			data->status[PB_STATUS_FAN34_BASE + i]
			  = _pmbus_read_byte_data(client, i,
						  PMBUS_STATUS_FAN_34);
		}

		if (info->func[0] & PMBUS_HAVE_STATUS_INPUT)
			data->status[PB_STATUS_INPUT_BASE]
			  = _pmbus_read_byte_data(client, 0,
						  PMBUS_STATUS_INPUT);

		for (i = 0; i < data->num_sensors; i++) {
			struct pmbus_sensor *sensor = &data->sensors[i];

			if (!data->valid || sensor->update)
				sensor->data
				    = _pmbus_read_word_data(client,
							    sensor->page,
							    sensor->reg);
		}
		pmbus_clear_faults(client);
		data->last_updated = jiffies;
		data->valid = 1;
	}
	mutex_unlock(&data->update_lock);
	return data;
}

/*
 * Convert linear sensor values to milli- or micro-units
 * depending on sensor type.
 */
static long pmbus_reg2data_linear(struct pmbus_data *data,
				  struct pmbus_sensor *sensor)
{
	s16 exponent;
	s32 mantissa;
	long val;

	if (sensor->class == PSC_VOLTAGE_OUT) {	/* LINEAR16 */
		exponent = data->exponent;
		mantissa = (u16) sensor->data;
	} else {				/* LINEAR11 */
		exponent = ((s16)sensor->data) >> 11;
		mantissa = ((s16)((sensor->data & 0x7ff) << 5)) >> 5;
	}

	val = mantissa;
=======
EXPORT_SYMBOL_NS_GPL(pmbus_get_driver_info, PMBUS);

static int pmbus_get_status(struct i2c_client *client, int page, int reg)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	int status;

	switch (reg) {
	case PMBUS_STATUS_WORD:
		status = data->read_status(client, page);
		break;
	default:
		status = _pmbus_read_byte_data(client, page, reg);
		break;
	}
	if (status < 0)
		pmbus_clear_faults(client);
	return status;
}

static void pmbus_update_sensor_data(struct i2c_client *client, struct pmbus_sensor *sensor)
{
	if (sensor->data < 0 || sensor->update)
		sensor->data = _pmbus_read_word_data(client, sensor->page,
						     sensor->phase, sensor->reg);
}

/*
 * Convert ieee754 sensor values to milli- or micro-units
 * depending on sensor type.
 *
 * ieee754 data format:
 *	bit 15:		sign
 *	bit 10..14:	exponent
 *	bit 0..9:	mantissa
 * exponent=0:
 *	v=(−1)^signbit * 2^(−14) * 0.significantbits
 * exponent=1..30:
 *	v=(−1)^signbit * 2^(exponent - 15) * 1.significantbits
 * exponent=31:
 *	v=NaN
 *
 * Add the number mantissa bits into the calculations for simplicity.
 * To do that, add '10' to the exponent. By doing that, we can just add
 * 0x400 to normal values and get the expected result.
 */
static long pmbus_reg2data_ieee754(struct pmbus_data *data,
				   struct pmbus_sensor *sensor)
{
	int exponent;
	bool sign;
	long val;

	/* only support half precision for now */
	sign = sensor->data & 0x8000;
	exponent = (sensor->data >> 10) & 0x1f;
	val = sensor->data & 0x3ff;

	if (exponent == 0) {			/* subnormal */
		exponent = -(14 + 10);
	} else if (exponent ==  0x1f) {		/* NaN, convert to min/max */
		exponent = 0;
		val = 65504;
	} else {
		exponent -= (15 + 10);		/* normal */
		val |= 0x400;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* scale result to milli-units for all sensors except fans */
	if (sensor->class != PSC_FAN)
		val = val * 1000L;

	/* scale result to micro-units for power sensors */
	if (sensor->class == PSC_POWER)
		val = val * 1000L;

	if (exponent >= 0)
		val <<= exponent;
	else
		val >>= -exponent;

<<<<<<< HEAD
=======
	if (sign)
		val = -val;

	return val;
}

/*
 * Convert linear sensor values to milli- or micro-units
 * depending on sensor type.
 */
static s64 pmbus_reg2data_linear(struct pmbus_data *data,
				 struct pmbus_sensor *sensor)
{
	s16 exponent;
	s32 mantissa;
	s64 val;

	if (sensor->class == PSC_VOLTAGE_OUT) {	/* LINEAR16 */
		exponent = data->exponent[sensor->page];
		mantissa = (u16) sensor->data;
	} else {				/* LINEAR11 */
		exponent = ((s16)sensor->data) >> 11;
		mantissa = ((s16)((sensor->data & 0x7ff) << 5)) >> 5;
	}

	val = mantissa;

	/* scale result to milli-units for all sensors except fans */
	if (sensor->class != PSC_FAN)
		val = val * 1000LL;

	/* scale result to micro-units for power sensors */
	if (sensor->class == PSC_POWER)
		val = val * 1000LL;

	if (exponent >= 0)
		val <<= exponent;
	else
		val >>= -exponent;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return val;
}

/*
 * Convert direct sensor values to milli- or micro-units
 * depending on sensor type.
 */
<<<<<<< HEAD
static long pmbus_reg2data_direct(struct pmbus_data *data,
				  struct pmbus_sensor *sensor)
{
	long val = (s16) sensor->data;
	long m, b, R;
=======
static s64 pmbus_reg2data_direct(struct pmbus_data *data,
				 struct pmbus_sensor *sensor)
{
	s64 b, val = (s16)sensor->data;
	s32 m, R;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	m = data->info->m[sensor->class];
	b = data->info->b[sensor->class];
	R = data->info->R[sensor->class];

	if (m == 0)
		return 0;

	/* X = 1/m * (Y * 10^-R - b) */
	R = -R;
	/* scale result to milli-units for everything but fans */
<<<<<<< HEAD
	if (sensor->class != PSC_FAN) {
=======
	if (!(sensor->class == PSC_FAN || sensor->class == PSC_PWM)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		R += 3;
		b *= 1000;
	}

	/* scale result to micro-units for power sensors */
	if (sensor->class == PSC_POWER) {
		R += 3;
		b *= 1000;
	}

	while (R > 0) {
		val *= 10;
		R--;
	}
	while (R < 0) {
<<<<<<< HEAD
		val = DIV_ROUND_CLOSEST(val, 10);
		R++;
	}

	return (val - b) / m;
=======
		val = div_s64(val + 5LL, 10L);  /* round closest */
		R++;
	}

	val = div_s64(val - b, m);
	return val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Convert VID sensor values to milli- or micro-units
 * depending on sensor type.
<<<<<<< HEAD
 * We currently only support VR11.
 */
static long pmbus_reg2data_vid(struct pmbus_data *data,
			       struct pmbus_sensor *sensor)
{
	long val = sensor->data;

	if (val < 0x02 || val > 0xb2)
		return 0;
	return DIV_ROUND_CLOSEST(160000 - (val - 2) * 625, 100);
}

static long pmbus_reg2data(struct pmbus_data *data, struct pmbus_sensor *sensor)
{
	long val;
=======
 */
static s64 pmbus_reg2data_vid(struct pmbus_data *data,
			      struct pmbus_sensor *sensor)
{
	long val = sensor->data;
	long rv = 0;

	switch (data->info->vrm_version[sensor->page]) {
	case vr11:
		if (val >= 0x02 && val <= 0xb2)
			rv = DIV_ROUND_CLOSEST(160000 - (val - 2) * 625, 100);
		break;
	case vr12:
		if (val >= 0x01)
			rv = 250 + (val - 1) * 5;
		break;
	case vr13:
		if (val >= 0x01)
			rv = 500 + (val - 1) * 10;
		break;
	case imvp9:
		if (val >= 0x01)
			rv = 200 + (val - 1) * 10;
		break;
	case amd625mv:
		if (val >= 0x0 && val <= 0xd8)
			rv = DIV_ROUND_CLOSEST(155000 - val * 625, 100);
		break;
	}
	return rv;
}

static s64 pmbus_reg2data(struct pmbus_data *data, struct pmbus_sensor *sensor)
{
	s64 val;

	if (!sensor->convert)
		return sensor->data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (data->info->format[sensor->class]) {
	case direct:
		val = pmbus_reg2data_direct(data, sensor);
		break;
	case vid:
		val = pmbus_reg2data_vid(data, sensor);
		break;
<<<<<<< HEAD
=======
	case ieee754:
		val = pmbus_reg2data_ieee754(data, sensor);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case linear:
	default:
		val = pmbus_reg2data_linear(data, sensor);
		break;
	}
	return val;
}

<<<<<<< HEAD
#define MAX_MANTISSA	(1023 * 1000)
#define MIN_MANTISSA	(511 * 1000)

static u16 pmbus_data2reg_linear(struct pmbus_data *data,
				 enum pmbus_sensor_classes class, long val)
=======
#define MAX_IEEE_MANTISSA	(0x7ff * 1000)
#define MIN_IEEE_MANTISSA	(0x400 * 1000)

static u16 pmbus_data2reg_ieee754(struct pmbus_data *data,
				  struct pmbus_sensor *sensor, long val)
{
	u16 exponent = (15 + 10);
	long mantissa;
	u16 sign = 0;

	/* simple case */
	if (val == 0)
		return 0;

	if (val < 0) {
		sign = 0x8000;
		val = -val;
	}

	/* Power is in uW. Convert to mW before converting. */
	if (sensor->class == PSC_POWER)
		val = DIV_ROUND_CLOSEST(val, 1000L);

	/*
	 * For simplicity, convert fan data to milli-units
	 * before calculating the exponent.
	 */
	if (sensor->class == PSC_FAN)
		val = val * 1000;

	/* Reduce large mantissa until it fits into 10 bit */
	while (val > MAX_IEEE_MANTISSA && exponent < 30) {
		exponent++;
		val >>= 1;
	}
	/*
	 * Increase small mantissa to generate valid 'normal'
	 * number
	 */
	while (val < MIN_IEEE_MANTISSA && exponent > 1) {
		exponent--;
		val <<= 1;
	}

	/* Convert mantissa from milli-units to units */
	mantissa = DIV_ROUND_CLOSEST(val, 1000);

	/*
	 * Ensure that the resulting number is within range.
	 * Valid range is 0x400..0x7ff, where bit 10 reflects
	 * the implied high bit in normalized ieee754 numbers.
	 * Set the range to 0x400..0x7ff to reflect this.
	 * The upper bit is then removed by the mask against
	 * 0x3ff in the final assignment.
	 */
	if (mantissa > 0x7ff)
		mantissa = 0x7ff;
	else if (mantissa < 0x400)
		mantissa = 0x400;

	/* Convert to sign, 5 bit exponent, 10 bit mantissa */
	return sign | (mantissa & 0x3ff) | ((exponent << 10) & 0x7c00);
}

#define MAX_LIN_MANTISSA	(1023 * 1000)
#define MIN_LIN_MANTISSA	(511 * 1000)

static u16 pmbus_data2reg_linear(struct pmbus_data *data,
				 struct pmbus_sensor *sensor, s64 val)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	s16 exponent = 0, mantissa;
	bool negative = false;

	/* simple case */
	if (val == 0)
		return 0;

<<<<<<< HEAD
	if (class == PSC_VOLTAGE_OUT) {
=======
	if (sensor->class == PSC_VOLTAGE_OUT) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* LINEAR16 does not support negative voltages */
		if (val < 0)
			return 0;

		/*
		 * For a static exponents, we don't have a choice
		 * but to adjust the value to it.
		 */
<<<<<<< HEAD
		if (data->exponent < 0)
			val <<= -data->exponent;
		else
			val >>= data->exponent;
		val = DIV_ROUND_CLOSEST(val, 1000);
		return val & 0xffff;
=======
		if (data->exponent[sensor->page] < 0)
			val <<= -data->exponent[sensor->page];
		else
			val >>= data->exponent[sensor->page];
		val = DIV_ROUND_CLOSEST_ULL(val, 1000);
		return clamp_val(val, 0, 0xffff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (val < 0) {
		negative = true;
		val = -val;
	}

	/* Power is in uW. Convert to mW before converting. */
<<<<<<< HEAD
	if (class == PSC_POWER)
		val = DIV_ROUND_CLOSEST(val, 1000L);
=======
	if (sensor->class == PSC_POWER)
		val = DIV_ROUND_CLOSEST_ULL(val, 1000);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * For simplicity, convert fan data to milli-units
	 * before calculating the exponent.
	 */
<<<<<<< HEAD
	if (class == PSC_FAN)
		val = val * 1000;

	/* Reduce large mantissa until it fits into 10 bit */
	while (val >= MAX_MANTISSA && exponent < 15) {
=======
	if (sensor->class == PSC_FAN)
		val = val * 1000LL;

	/* Reduce large mantissa until it fits into 10 bit */
	while (val >= MAX_LIN_MANTISSA && exponent < 15) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		exponent++;
		val >>= 1;
	}
	/* Increase small mantissa to improve precision */
<<<<<<< HEAD
	while (val < MIN_MANTISSA && exponent > -15) {
=======
	while (val < MIN_LIN_MANTISSA && exponent > -15) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		exponent--;
		val <<= 1;
	}

	/* Convert mantissa from milli-units to units */
<<<<<<< HEAD
	mantissa = DIV_ROUND_CLOSEST(val, 1000);

	/* Ensure that resulting number is within range */
	if (mantissa > 0x3ff)
		mantissa = 0x3ff;
=======
	mantissa = clamp_val(DIV_ROUND_CLOSEST_ULL(val, 1000), 0, 0x3ff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* restore sign */
	if (negative)
		mantissa = -mantissa;

	/* Convert to 5 bit exponent, 11 bit mantissa */
	return (mantissa & 0x7ff) | ((exponent << 11) & 0xf800);
}

static u16 pmbus_data2reg_direct(struct pmbus_data *data,
<<<<<<< HEAD
				 enum pmbus_sensor_classes class, long val)
{
	long m, b, R;

	m = data->info->m[class];
	b = data->info->b[class];
	R = data->info->R[class];

	/* Power is in uW. Adjust R and b. */
	if (class == PSC_POWER) {
=======
				 struct pmbus_sensor *sensor, s64 val)
{
	s64 b;
	s32 m, R;

	m = data->info->m[sensor->class];
	b = data->info->b[sensor->class];
	R = data->info->R[sensor->class];

	/* Power is in uW. Adjust R and b. */
	if (sensor->class == PSC_POWER) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		R -= 3;
		b *= 1000;
	}

	/* Calculate Y = (m * X + b) * 10^R */
<<<<<<< HEAD
	if (class != PSC_FAN) {
=======
	if (!(sensor->class == PSC_FAN || sensor->class == PSC_PWM)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		R -= 3;		/* Adjust R and b for data in milli-units */
		b *= 1000;
	}
	val = val * m + b;

	while (R > 0) {
		val *= 10;
		R--;
	}
	while (R < 0) {
<<<<<<< HEAD
		val = DIV_ROUND_CLOSEST(val, 10);
		R++;
	}

	return val;
}

static u16 pmbus_data2reg_vid(struct pmbus_data *data,
			      enum pmbus_sensor_classes class, long val)
{
	val = SENSORS_LIMIT(val, 500, 1600);

	return 2 + DIV_ROUND_CLOSEST((1600 - val) * 100, 625);
}

static u16 pmbus_data2reg(struct pmbus_data *data,
			  enum pmbus_sensor_classes class, long val)
{
	u16 regval;

	switch (data->info->format[class]) {
	case direct:
		regval = pmbus_data2reg_direct(data, class, val);
		break;
	case vid:
		regval = pmbus_data2reg_vid(data, class, val);
		break;
	case linear:
	default:
		regval = pmbus_data2reg_linear(data, class, val);
=======
		val = div_s64(val + 5LL, 10L);  /* round closest */
		R++;
	}

	return (u16)clamp_val(val, S16_MIN, S16_MAX);
}

static u16 pmbus_data2reg_vid(struct pmbus_data *data,
			      struct pmbus_sensor *sensor, s64 val)
{
	val = clamp_val(val, 500, 1600);

	return 2 + DIV_ROUND_CLOSEST_ULL((1600LL - val) * 100LL, 625);
}

static u16 pmbus_data2reg(struct pmbus_data *data,
			  struct pmbus_sensor *sensor, s64 val)
{
	u16 regval;

	if (!sensor->convert)
		return val;

	switch (data->info->format[sensor->class]) {
	case direct:
		regval = pmbus_data2reg_direct(data, sensor, val);
		break;
	case vid:
		regval = pmbus_data2reg_vid(data, sensor, val);
		break;
	case ieee754:
		regval = pmbus_data2reg_ieee754(data, sensor, val);
		break;
	case linear:
	default:
		regval = pmbus_data2reg_linear(data, sensor, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	return regval;
}

/*
 * Return boolean calculated from converted data.
<<<<<<< HEAD
 * <index> defines a status register index and mask, and optionally
 * two sensor indexes.
 * The upper half-word references the two sensors,
 * two sensor indices.
 * The upper half-word references the two optional sensors,
 * the lower half word references status register and mask.
 * The function returns true if (status[reg] & mask) is true and,
 * if specified, if v1 >= v2.
 * To determine if an object exceeds upper limits, specify <v, limit>.
 * To determine if an object exceeds lower limits, specify <limit, v>.
 *
 * For booleans created with pmbus_add_boolean_reg(), only the lower 16 bits of
 * index are set. s1 and s2 (the sensor index values) are zero in this case.
 * The function returns true if (status[reg] & mask) is true.
 *
 * If the boolean was created with pmbus_add_boolean_cmp(), a comparison against
 * a specified limit has to be performed to determine the boolean result.
 * In this case, the function returns true if v1 >= v2 (where v1 and v2 are
 * sensor values referenced by sensor indices s1 and s2).
=======
 * <index> defines a status register index and mask.
 * The mask is in the lower 8 bits, the register index is in bits 8..23.
 *
 * The associated pmbus_boolean structure contains optional pointers to two
 * sensor attributes. If specified, those attributes are compared against each
 * other to determine if a limit has been exceeded.
 *
 * If the sensor attribute pointers are NULL, the function returns true if
 * (status[reg] & mask) is true.
 *
 * If sensor attribute pointers are provided, a comparison against a specified
 * limit has to be performed to determine the boolean result.
 * In this case, the function returns true if v1 >= v2 (where v1 and v2 are
 * sensor values referenced by sensor attribute pointers s1 and s2).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * To determine if an object exceeds upper limits, specify <s1,s2> = <v,limit>.
 * To determine if an object exceeds lower limits, specify <s1,s2> = <limit,v>.
 *
 * If a negative value is stored in any of the referenced registers, this value
 * reflects an error code which will be returned.
 */
<<<<<<< HEAD
static int pmbus_get_boolean(struct pmbus_data *data, int index)
{
	u8 s1 = (index >> 24) & 0xff;
	u8 s2 = (index >> 16) & 0xff;
	u8 reg = (index >> 8) & 0xff;
	u8 mask = index & 0xff;
	int ret, status;
	u8 regval;

	status = data->status[reg];
	if (status < 0)
		return status;

	regval = status & mask;
	if (!s1 && !s2)
		ret = !!regval;
	else {
		long v1, v2;
		struct pmbus_sensor *sensor1, *sensor2;

		sensor1 = &data->sensors[s1];
		if (sensor1->data < 0)
			return sensor1->data;
		sensor2 = &data->sensors[s2];
		if (sensor2->data < 0)
			return sensor2->data;

		v1 = pmbus_reg2data(data, sensor1);
		v2 = pmbus_reg2data(data, sensor2);
		ret = !!(regval && v1 >= v2);
	}
=======
static int pmbus_get_boolean(struct i2c_client *client, struct pmbus_boolean *b,
			     int index)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct pmbus_sensor *s1 = b->s1;
	struct pmbus_sensor *s2 = b->s2;
	u16 mask = pb_index_to_mask(index);
	u8 page = pb_index_to_page(index);
	u16 reg = pb_index_to_reg(index);
	int ret, status;
	u16 regval;

	mutex_lock(&data->update_lock);
	status = pmbus_get_status(client, page, reg);
	if (status < 0) {
		ret = status;
		goto unlock;
	}

	if (s1)
		pmbus_update_sensor_data(client, s1);
	if (s2)
		pmbus_update_sensor_data(client, s2);

	regval = status & mask;
	if (regval) {
		ret = _pmbus_write_byte_data(client, page, reg, regval);
		if (ret)
			goto unlock;
	}
	if (s1 && s2) {
		s64 v1, v2;

		if (s1->data < 0) {
			ret = s1->data;
			goto unlock;
		}
		if (s2->data < 0) {
			ret = s2->data;
			goto unlock;
		}

		v1 = pmbus_reg2data(data, s1);
		v2 = pmbus_reg2data(data, s2);
		ret = !!(regval && v1 >= v2);
	} else {
		ret = !!regval;
	}
unlock:
	mutex_unlock(&data->update_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static ssize_t pmbus_show_boolean(struct device *dev,
				  struct device_attribute *da, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
<<<<<<< HEAD
	struct pmbus_data *data = pmbus_update_device(dev);
	int val;

	val = pmbus_get_boolean(data, attr->index);
	if (val < 0)
		return val;
	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t pmbus_show_sensor(struct device *dev,
				 struct device_attribute *da, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct pmbus_data *data = pmbus_update_device(dev);
	struct pmbus_sensor *sensor;

	sensor = &data->sensors[attr->index];
	if (sensor->data < 0)
		return sensor->data;

	return snprintf(buf, PAGE_SIZE, "%ld\n", pmbus_reg2data(data, sensor));
=======
	struct pmbus_boolean *boolean = to_pmbus_boolean(attr);
	struct i2c_client *client = to_i2c_client(dev->parent);
	int val;

	val = pmbus_get_boolean(client, boolean, attr->index);
	if (val < 0)
		return val;
	return sysfs_emit(buf, "%d\n", val);
}

static ssize_t pmbus_show_sensor(struct device *dev,
				 struct device_attribute *devattr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev->parent);
	struct pmbus_sensor *sensor = to_pmbus_sensor(devattr);
	struct pmbus_data *data = i2c_get_clientdata(client);
	ssize_t ret;

	mutex_lock(&data->update_lock);
	pmbus_update_sensor_data(client, sensor);
	if (sensor->data < 0)
		ret = sensor->data;
	else
		ret = sysfs_emit(buf, "%lld\n", pmbus_reg2data(data, sensor));
	mutex_unlock(&data->update_lock);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t pmbus_set_sensor(struct device *dev,
				struct device_attribute *devattr,
				const char *buf, size_t count)
{
<<<<<<< HEAD
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	struct i2c_client *client = to_i2c_client(dev);
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct pmbus_sensor *sensor = &data->sensors[attr->index];
	ssize_t rv = count;
	long val = 0;
	int ret;
	u16 regval;

	if (kstrtol(buf, 10, &val) < 0)
		return -EINVAL;

	mutex_lock(&data->update_lock);
	regval = pmbus_data2reg(data, sensor->class, val);
=======
	struct i2c_client *client = to_i2c_client(dev->parent);
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct pmbus_sensor *sensor = to_pmbus_sensor(devattr);
	ssize_t rv = count;
	s64 val;
	int ret;
	u16 regval;

	if (kstrtos64(buf, 10, &val) < 0)
		return -EINVAL;

	mutex_lock(&data->update_lock);
	regval = pmbus_data2reg(data, sensor, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = _pmbus_write_word_data(client, sensor->page, sensor->reg, regval);
	if (ret < 0)
		rv = ret;
	else
<<<<<<< HEAD
		data->sensors[attr->index].data = regval;
=======
		sensor->data = -ENODATA;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&data->update_lock);
	return rv;
}

static ssize_t pmbus_show_label(struct device *dev,
				struct device_attribute *da, char *buf)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);

	return snprintf(buf, PAGE_SIZE, "%s\n",
			data->labels[attr->index].label);
}

#define PMBUS_ADD_ATTR(data, _name, _idx, _mode, _type, _show, _set)	\
do {									\
	struct sensor_device_attribute *a				\
	    = &data->_type##s[data->num_##_type##s].attribute;		\
	BUG_ON(data->num_attributes >= data->max_attributes);		\
	sysfs_attr_init(&a->dev_attr.attr);				\
	a->dev_attr.attr.name = _name;					\
	a->dev_attr.attr.mode = _mode;					\
	a->dev_attr.show = _show;					\
	a->dev_attr.store = _set;					\
	a->index = _idx;						\
	data->attributes[data->num_attributes] = &a->dev_attr.attr;	\
	data->num_attributes++;						\
} while (0)

#define PMBUS_ADD_GET_ATTR(data, _name, _type, _idx)			\
	PMBUS_ADD_ATTR(data, _name, _idx, S_IRUGO, _type,		\
		       pmbus_show_##_type,  NULL)

#define PMBUS_ADD_SET_ATTR(data, _name, _type, _idx)			\
	PMBUS_ADD_ATTR(data, _name, _idx, S_IWUSR | S_IRUGO, _type,	\
		       pmbus_show_##_type, pmbus_set_##_type)

static void pmbus_add_boolean(struct pmbus_data *data,
			      const char *name, const char *type, int seq,
			      int idx)
{
	struct pmbus_boolean *boolean;

	BUG_ON(data->num_booleans >= data->max_booleans);

	boolean = &data->booleans[data->num_booleans];

	snprintf(boolean->name, sizeof(boolean->name), "%s%d_%s",
		 name, seq, type);
	PMBUS_ADD_GET_ATTR(data, boolean->name, boolean, idx);
	data->num_booleans++;
}

static void pmbus_add_boolean_reg(struct pmbus_data *data,
				  const char *name, const char *type,
				  int seq, int reg, int bit)
{
	pmbus_add_boolean(data, name, type, seq, (reg << 8) | bit);
}

static void pmbus_add_boolean_cmp(struct pmbus_data *data,
				  const char *name, const char *type,
				  int seq, int i1, int i2, int reg, int mask)
{
	pmbus_add_boolean(data, name, type, seq,
			  (i1 << 24) | (i2 << 16) | (reg << 8) | mask);
}

static void pmbus_add_sensor(struct pmbus_data *data,
			     const char *name, const char *type, int seq,
			     int page, int reg, enum pmbus_sensor_classes class,
			     bool update, bool readonly)
{
	struct pmbus_sensor *sensor;

	BUG_ON(data->num_sensors >= data->max_sensors);

	sensor = &data->sensors[data->num_sensors];
	snprintf(sensor->name, sizeof(sensor->name), "%s%d_%s",
		 name, seq, type);
	sensor->page = page;
	sensor->reg = reg;
	sensor->class = class;
	sensor->update = update;
	if (readonly)
		PMBUS_ADD_GET_ATTR(data, sensor->name, sensor,
				   data->num_sensors);
	else
		PMBUS_ADD_SET_ATTR(data, sensor->name, sensor,
				   data->num_sensors);
	data->num_sensors++;
}

static void pmbus_add_label(struct pmbus_data *data,
			    const char *name, int seq,
			    const char *lstring, int index)
{
	struct pmbus_label *label;

	BUG_ON(data->num_labels >= data->max_labels);

	label = &data->labels[data->num_labels];
	snprintf(label->name, sizeof(label->name), "%s%d_label", name, seq);
	if (!index)
		strncpy(label->label, lstring, sizeof(label->label) - 1);
	else
		snprintf(label->label, sizeof(label->label), "%s%d", lstring,
			 index);

	PMBUS_ADD_GET_ATTR(data, label->name, label, data->num_labels);
	data->num_labels++;
}

/*
 * Determine maximum number of sensors, booleans, and labels.
 * To keep things simple, only make a rough high estimate.
 */
static void pmbus_find_max_attr(struct i2c_client *client,
				struct pmbus_data *data)
{
	const struct pmbus_driver_info *info = data->info;
	int page, max_sensors, max_booleans, max_labels;

	max_sensors = PMBUS_MAX_INPUT_SENSORS;
	max_booleans = PMBUS_MAX_INPUT_BOOLEANS;
	max_labels = PMBUS_MAX_INPUT_LABELS;

	for (page = 0; page < info->pages; page++) {
		if (info->func[page] & PMBUS_HAVE_VOUT) {
			max_sensors += PMBUS_VOUT_SENSORS_PER_PAGE;
			max_booleans += PMBUS_VOUT_BOOLEANS_PER_PAGE;
			max_labels++;
		}
		if (info->func[page] & PMBUS_HAVE_IOUT) {
			max_sensors += PMBUS_IOUT_SENSORS_PER_PAGE;
			max_booleans += PMBUS_IOUT_BOOLEANS_PER_PAGE;
			max_labels++;
		}
		if (info->func[page] & PMBUS_HAVE_POUT) {
			max_sensors += PMBUS_POUT_SENSORS_PER_PAGE;
			max_booleans += PMBUS_POUT_BOOLEANS_PER_PAGE;
			max_labels++;
		}
		if (info->func[page] & PMBUS_HAVE_FAN12) {
			max_sensors += 2 * PMBUS_MAX_SENSORS_PER_FAN;
			max_booleans += 2 * PMBUS_MAX_BOOLEANS_PER_FAN;
		}
		if (info->func[page] & PMBUS_HAVE_FAN34) {
			max_sensors += 2 * PMBUS_MAX_SENSORS_PER_FAN;
			max_booleans += 2 * PMBUS_MAX_BOOLEANS_PER_FAN;
		}
		if (info->func[page] & PMBUS_HAVE_TEMP) {
			max_sensors += PMBUS_MAX_SENSORS_PER_TEMP;
			max_booleans += PMBUS_MAX_BOOLEANS_PER_TEMP;
		}
		if (info->func[page] & PMBUS_HAVE_TEMP2) {
			max_sensors += PMBUS_MAX_SENSORS_PER_TEMP;
			max_booleans += PMBUS_MAX_BOOLEANS_PER_TEMP;
		}
		if (info->func[page] & PMBUS_HAVE_TEMP3) {
			max_sensors += PMBUS_MAX_SENSORS_PER_TEMP;
			max_booleans += PMBUS_MAX_BOOLEANS_PER_TEMP;
		}
	}
	data->max_sensors = max_sensors;
	data->max_booleans = max_booleans;
	data->max_labels = max_labels;
	data->max_attributes = max_sensors + max_booleans + max_labels;
=======
	struct pmbus_label *label = to_pmbus_label(da);

	return sysfs_emit(buf, "%s\n", label->label);
}

static int pmbus_add_attribute(struct pmbus_data *data, struct attribute *attr)
{
	if (data->num_attributes >= data->max_attributes - 1) {
		int new_max_attrs = data->max_attributes + PMBUS_ATTR_ALLOC_SIZE;
		void *new_attrs = devm_krealloc_array(data->dev, data->group.attrs,
						      new_max_attrs, sizeof(void *),
						      GFP_KERNEL);
		if (!new_attrs)
			return -ENOMEM;
		data->group.attrs = new_attrs;
		data->max_attributes = new_max_attrs;
	}

	data->group.attrs[data->num_attributes++] = attr;
	data->group.attrs[data->num_attributes] = NULL;
	return 0;
}

static void pmbus_dev_attr_init(struct device_attribute *dev_attr,
				const char *name,
				umode_t mode,
				ssize_t (*show)(struct device *dev,
						struct device_attribute *attr,
						char *buf),
				ssize_t (*store)(struct device *dev,
						 struct device_attribute *attr,
						 const char *buf, size_t count))
{
	sysfs_attr_init(&dev_attr->attr);
	dev_attr->attr.name = name;
	dev_attr->attr.mode = mode;
	dev_attr->show = show;
	dev_attr->store = store;
}

static void pmbus_attr_init(struct sensor_device_attribute *a,
			    const char *name,
			    umode_t mode,
			    ssize_t (*show)(struct device *dev,
					    struct device_attribute *attr,
					    char *buf),
			    ssize_t (*store)(struct device *dev,
					     struct device_attribute *attr,
					     const char *buf, size_t count),
			    int idx)
{
	pmbus_dev_attr_init(&a->dev_attr, name, mode, show, store);
	a->index = idx;
}

static int pmbus_add_boolean(struct pmbus_data *data,
			     const char *name, const char *type, int seq,
			     struct pmbus_sensor *s1,
			     struct pmbus_sensor *s2,
			     u8 page, u16 reg, u16 mask)
{
	struct pmbus_boolean *boolean;
	struct sensor_device_attribute *a;

	if (WARN((s1 && !s2) || (!s1 && s2), "Bad s1/s2 parameters\n"))
		return -EINVAL;

	boolean = devm_kzalloc(data->dev, sizeof(*boolean), GFP_KERNEL);
	if (!boolean)
		return -ENOMEM;

	a = &boolean->attribute;

	snprintf(boolean->name, sizeof(boolean->name), "%s%d_%s",
		 name, seq, type);
	boolean->s1 = s1;
	boolean->s2 = s2;
	pmbus_attr_init(a, boolean->name, 0444, pmbus_show_boolean, NULL,
			pb_reg_to_index(page, reg, mask));

	return pmbus_add_attribute(data, &a->dev_attr.attr);
}

/* of thermal for pmbus temperature sensors */
struct pmbus_thermal_data {
	struct pmbus_data *pmbus_data;
	struct pmbus_sensor *sensor;
};

static int pmbus_thermal_get_temp(struct thermal_zone_device *tz, int *temp)
{
	struct pmbus_thermal_data *tdata = thermal_zone_device_priv(tz);
	struct pmbus_sensor *sensor = tdata->sensor;
	struct pmbus_data *pmbus_data = tdata->pmbus_data;
	struct i2c_client *client = to_i2c_client(pmbus_data->dev);
	struct device *dev = pmbus_data->hwmon_dev;
	int ret = 0;

	if (!dev) {
		/* May not even get to hwmon yet */
		*temp = 0;
		return 0;
	}

	mutex_lock(&pmbus_data->update_lock);
	pmbus_update_sensor_data(client, sensor);
	if (sensor->data < 0)
		ret = sensor->data;
	else
		*temp = (int)pmbus_reg2data(pmbus_data, sensor);
	mutex_unlock(&pmbus_data->update_lock);

	return ret;
}

static const struct thermal_zone_device_ops pmbus_thermal_ops = {
	.get_temp = pmbus_thermal_get_temp,
};

static int pmbus_thermal_add_sensor(struct pmbus_data *pmbus_data,
				    struct pmbus_sensor *sensor, int index)
{
	struct device *dev = pmbus_data->dev;
	struct pmbus_thermal_data *tdata;
	struct thermal_zone_device *tzd;

	tdata = devm_kzalloc(dev, sizeof(*tdata), GFP_KERNEL);
	if (!tdata)
		return -ENOMEM;

	tdata->sensor = sensor;
	tdata->pmbus_data = pmbus_data;

	tzd = devm_thermal_of_zone_register(dev, index, tdata,
					    &pmbus_thermal_ops);
	/*
	 * If CONFIG_THERMAL_OF is disabled, this returns -ENODEV,
	 * so ignore that error but forward any other error.
	 */
	if (IS_ERR(tzd) && (PTR_ERR(tzd) != -ENODEV))
		return PTR_ERR(tzd);

	return 0;
}

static struct pmbus_sensor *pmbus_add_sensor(struct pmbus_data *data,
					     const char *name, const char *type,
					     int seq, int page, int phase,
					     int reg,
					     enum pmbus_sensor_classes class,
					     bool update, bool readonly,
					     bool convert)
{
	struct pmbus_sensor *sensor;
	struct device_attribute *a;

	sensor = devm_kzalloc(data->dev, sizeof(*sensor), GFP_KERNEL);
	if (!sensor)
		return NULL;
	a = &sensor->attribute;

	if (type)
		snprintf(sensor->name, sizeof(sensor->name), "%s%d_%s",
			 name, seq, type);
	else
		snprintf(sensor->name, sizeof(sensor->name), "%s%d",
			 name, seq);

	if (data->flags & PMBUS_WRITE_PROTECTED)
		readonly = true;

	sensor->page = page;
	sensor->phase = phase;
	sensor->reg = reg;
	sensor->class = class;
	sensor->update = update;
	sensor->convert = convert;
	sensor->data = -ENODATA;
	pmbus_dev_attr_init(a, sensor->name,
			    readonly ? 0444 : 0644,
			    pmbus_show_sensor, pmbus_set_sensor);

	if (pmbus_add_attribute(data, &a->attr))
		return NULL;

	sensor->next = data->sensors;
	data->sensors = sensor;

	/* temperature sensors with _input values are registered with thermal */
	if (class == PSC_TEMPERATURE && strcmp(type, "input") == 0)
		pmbus_thermal_add_sensor(data, sensor, seq);

	return sensor;
}

static int pmbus_add_label(struct pmbus_data *data,
			   const char *name, int seq,
			   const char *lstring, int index, int phase)
{
	struct pmbus_label *label;
	struct device_attribute *a;

	label = devm_kzalloc(data->dev, sizeof(*label), GFP_KERNEL);
	if (!label)
		return -ENOMEM;

	a = &label->attribute;

	snprintf(label->name, sizeof(label->name), "%s%d_label", name, seq);
	if (!index) {
		if (phase == 0xff)
			strncpy(label->label, lstring,
				sizeof(label->label) - 1);
		else
			snprintf(label->label, sizeof(label->label), "%s.%d",
				 lstring, phase);
	} else {
		if (phase == 0xff)
			snprintf(label->label, sizeof(label->label), "%s%d",
				 lstring, index);
		else
			snprintf(label->label, sizeof(label->label), "%s%d.%d",
				 lstring, index, phase);
	}

	pmbus_dev_attr_init(a, label->name, 0444, pmbus_show_label, NULL);
	return pmbus_add_attribute(data, &a->attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Search for attributes. Allocate sensors, booleans, and labels as needed.
 */

/*
 * The pmbus_limit_attr structure describes a single limit attribute
 * and its associated alarm attribute.
 */
struct pmbus_limit_attr {
	u16 reg;		/* Limit register */
<<<<<<< HEAD
=======
	u16 sbit;		/* Alarm attribute status bit */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool update;		/* True if register needs updates */
	bool low;		/* True if low limit; for limits with compare
				   functions only */
	const char *attr;	/* Attribute name */
	const char *alarm;	/* Alarm attribute name */
<<<<<<< HEAD
	u32 sbit;		/* Alarm attribute status bit */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * The pmbus_sensor_attr structure describes one sensor attribute. This
 * description includes a reference to the associated limit attributes.
 */
struct pmbus_sensor_attr {
<<<<<<< HEAD
	u8 reg;				/* sensor register */
=======
	u16 reg;			/* sensor register */
	u16 gbit;			/* generic status bit */
	u8 nlimit;			/* # of limit registers */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	enum pmbus_sensor_classes class;/* sensor class */
	const char *label;		/* sensor label */
	bool paged;			/* true if paged sensor */
	bool update;			/* true if update needed */
	bool compare;			/* true if compare function needed */
	u32 func;			/* sensor mask */
	u32 sfunc;			/* sensor status mask */
<<<<<<< HEAD
	int sbase;			/* status base register */
	u32 gbit;			/* generic status bit */
	const struct pmbus_limit_attr *limit;/* limit registers */
	int nlimit;			/* # of limit registers */
=======
	int sreg;			/* status register */
	const struct pmbus_limit_attr *limit;/* limit registers */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Add a set of limit attributes and, if supported, the associated
 * alarm attributes.
<<<<<<< HEAD
 */
static bool pmbus_add_limit_attrs(struct i2c_client *client,
				  struct pmbus_data *data,
				  const struct pmbus_driver_info *info,
				  const char *name, int index, int page,
				  int cbase,
				  const struct pmbus_sensor_attr *attr)
{
	const struct pmbus_limit_attr *l = attr->limit;
	int nlimit = attr->nlimit;
	bool have_alarm = false;
	int i, cindex;

	for (i = 0; i < nlimit; i++) {
		if (pmbus_check_word_register(client, page, l->reg)) {
			cindex = data->num_sensors;
			pmbus_add_sensor(data, name, l->attr, index, page,
					 l->reg, attr->class,
					 attr->update || l->update,
					 false);
			if (l->sbit && (info->func[page] & attr->sfunc)) {
				if (attr->compare) {
					pmbus_add_boolean_cmp(data, name,
						l->alarm, index,
						l->low ? cindex : cbase,
						l->low ? cbase : cindex,
						attr->sbase + page, l->sbit);
				} else {
					pmbus_add_boolean_reg(data, name,
						l->alarm, index,
						attr->sbase + page, l->sbit);
				}
				have_alarm = true;
=======
 * returns 0 if no alarm register found, 1 if an alarm register was found,
 * < 0 on errors.
 */
static int pmbus_add_limit_attrs(struct i2c_client *client,
				 struct pmbus_data *data,
				 const struct pmbus_driver_info *info,
				 const char *name, int index, int page,
				 struct pmbus_sensor *base,
				 const struct pmbus_sensor_attr *attr)
{
	const struct pmbus_limit_attr *l = attr->limit;
	int nlimit = attr->nlimit;
	int have_alarm = 0;
	int i, ret;
	struct pmbus_sensor *curr;

	for (i = 0; i < nlimit; i++) {
		if (pmbus_check_word_register(client, page, l->reg)) {
			curr = pmbus_add_sensor(data, name, l->attr, index,
						page, 0xff, l->reg, attr->class,
						attr->update || l->update,
						false, true);
			if (!curr)
				return -ENOMEM;
			if (l->sbit && (info->func[page] & attr->sfunc)) {
				ret = pmbus_add_boolean(data, name,
					l->alarm, index,
					attr->compare ?  l->low ? curr : base
						      : NULL,
					attr->compare ? l->low ? base : curr
						      : NULL,
					page, attr->sreg, l->sbit);
				if (ret)
					return ret;
				have_alarm = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
		l++;
	}
	return have_alarm;
}

<<<<<<< HEAD
static void pmbus_add_sensor_attrs_one(struct i2c_client *client,
				       struct pmbus_data *data,
				       const struct pmbus_driver_info *info,
				       const char *name,
				       int index, int page,
				       const struct pmbus_sensor_attr *attr)
{
	bool have_alarm;
	int cbase = data->num_sensors;

	if (attr->label)
		pmbus_add_label(data, name, index, attr->label,
				attr->paged ? page + 1 : 0);
	pmbus_add_sensor(data, name, "input", index, page, attr->reg,
			 attr->class, true, true);
	if (attr->sfunc) {
		have_alarm = pmbus_add_limit_attrs(client, data, info, name,
						   index, page, cbase, attr);
		/*
		 * Add generic alarm attribute only if there are no individual
		 * alarm attributes, if there is a global alarm bit, and if
		 * the generic status register for this page is accessible.
		 */
		if (!have_alarm && attr->gbit &&
		    pmbus_check_byte_register(client, page, PMBUS_STATUS_BYTE))
			pmbus_add_boolean_reg(data, name, "alarm", index,
					      PB_STATUS_BASE + page,
					      attr->gbit);
	}
}

static void pmbus_add_sensor_attrs(struct i2c_client *client,
				   struct pmbus_data *data,
				   const char *name,
				   const struct pmbus_sensor_attr *attrs,
				   int nattrs)
{
	const struct pmbus_driver_info *info = data->info;
	int index, i;
=======
static int pmbus_add_sensor_attrs_one(struct i2c_client *client,
				      struct pmbus_data *data,
				      const struct pmbus_driver_info *info,
				      const char *name,
				      int index, int page, int phase,
				      const struct pmbus_sensor_attr *attr,
				      bool paged)
{
	struct pmbus_sensor *base;
	bool upper = !!(attr->gbit & 0xff00);	/* need to check STATUS_WORD */
	int ret;

	if (attr->label) {
		ret = pmbus_add_label(data, name, index, attr->label,
				      paged ? page + 1 : 0, phase);
		if (ret)
			return ret;
	}
	base = pmbus_add_sensor(data, name, "input", index, page, phase,
				attr->reg, attr->class, true, true, true);
	if (!base)
		return -ENOMEM;
	/* No limit and alarm attributes for phase specific sensors */
	if (attr->sfunc && phase == 0xff) {
		ret = pmbus_add_limit_attrs(client, data, info, name,
					    index, page, base, attr);
		if (ret < 0)
			return ret;
		/*
		 * Add generic alarm attribute only if there are no individual
		 * alarm attributes, if there is a global alarm bit, and if
		 * the generic status register (word or byte, depending on
		 * which global bit is set) for this page is accessible.
		 */
		if (!ret && attr->gbit &&
		    (!upper || data->has_status_word) &&
		    pmbus_check_status_register(client, page)) {
			ret = pmbus_add_boolean(data, name, "alarm", index,
						NULL, NULL,
						page, PMBUS_STATUS_WORD,
						attr->gbit);
			if (ret)
				return ret;
		}
	}
	return 0;
}

static bool pmbus_sensor_is_paged(const struct pmbus_driver_info *info,
				  const struct pmbus_sensor_attr *attr)
{
	int p;

	if (attr->paged)
		return true;

	/*
	 * Some attributes may be present on more than one page despite
	 * not being marked with the paged attribute. If that is the case,
	 * then treat the sensor as being paged and add the page suffix to the
	 * attribute name.
	 * We don't just add the paged attribute to all such attributes, in
	 * order to maintain the un-suffixed labels in the case where the
	 * attribute is only on page 0.
	 */
	for (p = 1; p < info->pages; p++) {
		if (info->func[p] & attr->func)
			return true;
	}
	return false;
}

static int pmbus_add_sensor_attrs(struct i2c_client *client,
				  struct pmbus_data *data,
				  const char *name,
				  const struct pmbus_sensor_attr *attrs,
				  int nattrs)
{
	const struct pmbus_driver_info *info = data->info;
	int index, i;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	index = 1;
	for (i = 0; i < nattrs; i++) {
		int page, pages;
<<<<<<< HEAD

		pages = attrs->paged ? info->pages : 1;
		for (page = 0; page < pages; page++) {
			if (!(info->func[page] & attrs->func))
				continue;
			pmbus_add_sensor_attrs_one(client, data, info, name,
						   index, page, attrs);
			index++;
		}
		attrs++;
	}
=======
		bool paged = pmbus_sensor_is_paged(info, attrs);

		pages = paged ? info->pages : 1;
		for (page = 0; page < pages; page++) {
			if (info->func[page] & attrs->func) {
				ret = pmbus_add_sensor_attrs_one(client, data, info,
								 name, index, page,
								 0xff, attrs, paged);
				if (ret)
					return ret;
				index++;
			}
			if (info->phases[page]) {
				int phase;

				for (phase = 0; phase < info->phases[page];
				     phase++) {
					if (!(info->pfunc[phase] & attrs->func))
						continue;
					ret = pmbus_add_sensor_attrs_one(client,
						data, info, name, index, page,
						phase, attrs, paged);
					if (ret)
						return ret;
					index++;
				}
			}
		}
		attrs++;
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct pmbus_limit_attr vin_limit_attrs[] = {
	{
		.reg = PMBUS_VIN_UV_WARN_LIMIT,
		.attr = "min",
		.alarm = "min_alarm",
		.sbit = PB_VOLTAGE_UV_WARNING,
	}, {
		.reg = PMBUS_VIN_UV_FAULT_LIMIT,
		.attr = "lcrit",
		.alarm = "lcrit_alarm",
<<<<<<< HEAD
		.sbit = PB_VOLTAGE_UV_FAULT,
=======
		.sbit = PB_VOLTAGE_UV_FAULT | PB_VOLTAGE_VIN_OFF,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}, {
		.reg = PMBUS_VIN_OV_WARN_LIMIT,
		.attr = "max",
		.alarm = "max_alarm",
		.sbit = PB_VOLTAGE_OV_WARNING,
	}, {
		.reg = PMBUS_VIN_OV_FAULT_LIMIT,
		.attr = "crit",
		.alarm = "crit_alarm",
		.sbit = PB_VOLTAGE_OV_FAULT,
	}, {
		.reg = PMBUS_VIRT_READ_VIN_AVG,
		.update = true,
		.attr = "average",
	}, {
		.reg = PMBUS_VIRT_READ_VIN_MIN,
		.update = true,
		.attr = "lowest",
	}, {
		.reg = PMBUS_VIRT_READ_VIN_MAX,
		.update = true,
		.attr = "highest",
	}, {
		.reg = PMBUS_VIRT_RESET_VIN_HISTORY,
		.attr = "reset_history",
<<<<<<< HEAD
	},
};

=======
	}, {
		.reg = PMBUS_MFR_VIN_MIN,
		.attr = "rated_min",
	}, {
		.reg = PMBUS_MFR_VIN_MAX,
		.attr = "rated_max",
	},
};

static const struct pmbus_limit_attr vmon_limit_attrs[] = {
	{
		.reg = PMBUS_VIRT_VMON_UV_WARN_LIMIT,
		.attr = "min",
		.alarm = "min_alarm",
		.sbit = PB_VOLTAGE_UV_WARNING,
	}, {
		.reg = PMBUS_VIRT_VMON_UV_FAULT_LIMIT,
		.attr = "lcrit",
		.alarm = "lcrit_alarm",
		.sbit = PB_VOLTAGE_UV_FAULT,
	}, {
		.reg = PMBUS_VIRT_VMON_OV_WARN_LIMIT,
		.attr = "max",
		.alarm = "max_alarm",
		.sbit = PB_VOLTAGE_OV_WARNING,
	}, {
		.reg = PMBUS_VIRT_VMON_OV_FAULT_LIMIT,
		.attr = "crit",
		.alarm = "crit_alarm",
		.sbit = PB_VOLTAGE_OV_FAULT,
	}
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct pmbus_limit_attr vout_limit_attrs[] = {
	{
		.reg = PMBUS_VOUT_UV_WARN_LIMIT,
		.attr = "min",
		.alarm = "min_alarm",
		.sbit = PB_VOLTAGE_UV_WARNING,
	}, {
		.reg = PMBUS_VOUT_UV_FAULT_LIMIT,
		.attr = "lcrit",
		.alarm = "lcrit_alarm",
		.sbit = PB_VOLTAGE_UV_FAULT,
	}, {
		.reg = PMBUS_VOUT_OV_WARN_LIMIT,
		.attr = "max",
		.alarm = "max_alarm",
		.sbit = PB_VOLTAGE_OV_WARNING,
	}, {
		.reg = PMBUS_VOUT_OV_FAULT_LIMIT,
		.attr = "crit",
		.alarm = "crit_alarm",
		.sbit = PB_VOLTAGE_OV_FAULT,
	}, {
		.reg = PMBUS_VIRT_READ_VOUT_AVG,
		.update = true,
		.attr = "average",
	}, {
		.reg = PMBUS_VIRT_READ_VOUT_MIN,
		.update = true,
		.attr = "lowest",
	}, {
		.reg = PMBUS_VIRT_READ_VOUT_MAX,
		.update = true,
		.attr = "highest",
	}, {
		.reg = PMBUS_VIRT_RESET_VOUT_HISTORY,
		.attr = "reset_history",
<<<<<<< HEAD
	}
=======
	}, {
		.reg = PMBUS_MFR_VOUT_MIN,
		.attr = "rated_min",
	}, {
		.reg = PMBUS_MFR_VOUT_MAX,
		.attr = "rated_max",
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct pmbus_sensor_attr voltage_attributes[] = {
	{
		.reg = PMBUS_READ_VIN,
		.class = PSC_VOLTAGE_IN,
		.label = "vin",
		.func = PMBUS_HAVE_VIN,
		.sfunc = PMBUS_HAVE_STATUS_INPUT,
<<<<<<< HEAD
		.sbase = PB_STATUS_INPUT_BASE,
=======
		.sreg = PMBUS_STATUS_INPUT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.gbit = PB_STATUS_VIN_UV,
		.limit = vin_limit_attrs,
		.nlimit = ARRAY_SIZE(vin_limit_attrs),
	}, {
<<<<<<< HEAD
=======
		.reg = PMBUS_VIRT_READ_VMON,
		.class = PSC_VOLTAGE_IN,
		.label = "vmon",
		.func = PMBUS_HAVE_VMON,
		.sfunc = PMBUS_HAVE_STATUS_VMON,
		.sreg = PMBUS_VIRT_STATUS_VMON,
		.limit = vmon_limit_attrs,
		.nlimit = ARRAY_SIZE(vmon_limit_attrs),
	}, {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.reg = PMBUS_READ_VCAP,
		.class = PSC_VOLTAGE_IN,
		.label = "vcap",
		.func = PMBUS_HAVE_VCAP,
	}, {
		.reg = PMBUS_READ_VOUT,
		.class = PSC_VOLTAGE_OUT,
		.label = "vout",
		.paged = true,
		.func = PMBUS_HAVE_VOUT,
		.sfunc = PMBUS_HAVE_STATUS_VOUT,
<<<<<<< HEAD
		.sbase = PB_STATUS_VOUT_BASE,
=======
		.sreg = PMBUS_STATUS_VOUT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.gbit = PB_STATUS_VOUT_OV,
		.limit = vout_limit_attrs,
		.nlimit = ARRAY_SIZE(vout_limit_attrs),
	}
};

/* Current attributes */

static const struct pmbus_limit_attr iin_limit_attrs[] = {
	{
		.reg = PMBUS_IIN_OC_WARN_LIMIT,
		.attr = "max",
		.alarm = "max_alarm",
		.sbit = PB_IIN_OC_WARNING,
	}, {
		.reg = PMBUS_IIN_OC_FAULT_LIMIT,
		.attr = "crit",
		.alarm = "crit_alarm",
		.sbit = PB_IIN_OC_FAULT,
	}, {
		.reg = PMBUS_VIRT_READ_IIN_AVG,
		.update = true,
		.attr = "average",
	}, {
		.reg = PMBUS_VIRT_READ_IIN_MIN,
		.update = true,
		.attr = "lowest",
	}, {
		.reg = PMBUS_VIRT_READ_IIN_MAX,
		.update = true,
		.attr = "highest",
	}, {
		.reg = PMBUS_VIRT_RESET_IIN_HISTORY,
		.attr = "reset_history",
<<<<<<< HEAD
	}
=======
	}, {
		.reg = PMBUS_MFR_IIN_MAX,
		.attr = "rated_max",
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct pmbus_limit_attr iout_limit_attrs[] = {
	{
		.reg = PMBUS_IOUT_OC_WARN_LIMIT,
		.attr = "max",
		.alarm = "max_alarm",
		.sbit = PB_IOUT_OC_WARNING,
	}, {
		.reg = PMBUS_IOUT_UC_FAULT_LIMIT,
		.attr = "lcrit",
		.alarm = "lcrit_alarm",
		.sbit = PB_IOUT_UC_FAULT,
	}, {
		.reg = PMBUS_IOUT_OC_FAULT_LIMIT,
		.attr = "crit",
		.alarm = "crit_alarm",
		.sbit = PB_IOUT_OC_FAULT,
	}, {
		.reg = PMBUS_VIRT_READ_IOUT_AVG,
		.update = true,
		.attr = "average",
	}, {
		.reg = PMBUS_VIRT_READ_IOUT_MIN,
		.update = true,
		.attr = "lowest",
	}, {
		.reg = PMBUS_VIRT_READ_IOUT_MAX,
		.update = true,
		.attr = "highest",
	}, {
		.reg = PMBUS_VIRT_RESET_IOUT_HISTORY,
		.attr = "reset_history",
<<<<<<< HEAD
	}
=======
	}, {
		.reg = PMBUS_MFR_IOUT_MAX,
		.attr = "rated_max",
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct pmbus_sensor_attr current_attributes[] = {
	{
		.reg = PMBUS_READ_IIN,
		.class = PSC_CURRENT_IN,
		.label = "iin",
		.func = PMBUS_HAVE_IIN,
		.sfunc = PMBUS_HAVE_STATUS_INPUT,
<<<<<<< HEAD
		.sbase = PB_STATUS_INPUT_BASE,
=======
		.sreg = PMBUS_STATUS_INPUT,
		.gbit = PB_STATUS_INPUT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.limit = iin_limit_attrs,
		.nlimit = ARRAY_SIZE(iin_limit_attrs),
	}, {
		.reg = PMBUS_READ_IOUT,
		.class = PSC_CURRENT_OUT,
		.label = "iout",
		.paged = true,
		.func = PMBUS_HAVE_IOUT,
		.sfunc = PMBUS_HAVE_STATUS_IOUT,
<<<<<<< HEAD
		.sbase = PB_STATUS_IOUT_BASE,
=======
		.sreg = PMBUS_STATUS_IOUT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.gbit = PB_STATUS_IOUT_OC,
		.limit = iout_limit_attrs,
		.nlimit = ARRAY_SIZE(iout_limit_attrs),
	}
};

/* Power attributes */

static const struct pmbus_limit_attr pin_limit_attrs[] = {
	{
		.reg = PMBUS_PIN_OP_WARN_LIMIT,
		.attr = "max",
		.alarm = "alarm",
		.sbit = PB_PIN_OP_WARNING,
	}, {
		.reg = PMBUS_VIRT_READ_PIN_AVG,
		.update = true,
		.attr = "average",
	}, {
<<<<<<< HEAD
=======
		.reg = PMBUS_VIRT_READ_PIN_MIN,
		.update = true,
		.attr = "input_lowest",
	}, {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.reg = PMBUS_VIRT_READ_PIN_MAX,
		.update = true,
		.attr = "input_highest",
	}, {
		.reg = PMBUS_VIRT_RESET_PIN_HISTORY,
		.attr = "reset_history",
<<<<<<< HEAD
	}
=======
	}, {
		.reg = PMBUS_MFR_PIN_MAX,
		.attr = "rated_max",
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct pmbus_limit_attr pout_limit_attrs[] = {
	{
		.reg = PMBUS_POUT_MAX,
		.attr = "cap",
		.alarm = "cap_alarm",
		.sbit = PB_POWER_LIMITING,
	}, {
		.reg = PMBUS_POUT_OP_WARN_LIMIT,
		.attr = "max",
		.alarm = "max_alarm",
		.sbit = PB_POUT_OP_WARNING,
	}, {
		.reg = PMBUS_POUT_OP_FAULT_LIMIT,
		.attr = "crit",
		.alarm = "crit_alarm",
		.sbit = PB_POUT_OP_FAULT,
	}, {
		.reg = PMBUS_VIRT_READ_POUT_AVG,
		.update = true,
		.attr = "average",
	}, {
<<<<<<< HEAD
=======
		.reg = PMBUS_VIRT_READ_POUT_MIN,
		.update = true,
		.attr = "input_lowest",
	}, {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.reg = PMBUS_VIRT_READ_POUT_MAX,
		.update = true,
		.attr = "input_highest",
	}, {
		.reg = PMBUS_VIRT_RESET_POUT_HISTORY,
		.attr = "reset_history",
<<<<<<< HEAD
	}
=======
	}, {
		.reg = PMBUS_MFR_POUT_MAX,
		.attr = "rated_max",
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct pmbus_sensor_attr power_attributes[] = {
	{
		.reg = PMBUS_READ_PIN,
		.class = PSC_POWER,
		.label = "pin",
		.func = PMBUS_HAVE_PIN,
		.sfunc = PMBUS_HAVE_STATUS_INPUT,
<<<<<<< HEAD
		.sbase = PB_STATUS_INPUT_BASE,
=======
		.sreg = PMBUS_STATUS_INPUT,
		.gbit = PB_STATUS_INPUT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.limit = pin_limit_attrs,
		.nlimit = ARRAY_SIZE(pin_limit_attrs),
	}, {
		.reg = PMBUS_READ_POUT,
		.class = PSC_POWER,
		.label = "pout",
		.paged = true,
		.func = PMBUS_HAVE_POUT,
		.sfunc = PMBUS_HAVE_STATUS_IOUT,
<<<<<<< HEAD
		.sbase = PB_STATUS_IOUT_BASE,
=======
		.sreg = PMBUS_STATUS_IOUT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.limit = pout_limit_attrs,
		.nlimit = ARRAY_SIZE(pout_limit_attrs),
	}
};

/* Temperature atributes */

static const struct pmbus_limit_attr temp_limit_attrs[] = {
	{
		.reg = PMBUS_UT_WARN_LIMIT,
		.low = true,
		.attr = "min",
		.alarm = "min_alarm",
		.sbit = PB_TEMP_UT_WARNING,
	}, {
		.reg = PMBUS_UT_FAULT_LIMIT,
		.low = true,
		.attr = "lcrit",
		.alarm = "lcrit_alarm",
		.sbit = PB_TEMP_UT_FAULT,
	}, {
		.reg = PMBUS_OT_WARN_LIMIT,
		.attr = "max",
		.alarm = "max_alarm",
		.sbit = PB_TEMP_OT_WARNING,
	}, {
		.reg = PMBUS_OT_FAULT_LIMIT,
		.attr = "crit",
		.alarm = "crit_alarm",
		.sbit = PB_TEMP_OT_FAULT,
	}, {
		.reg = PMBUS_VIRT_READ_TEMP_MIN,
		.attr = "lowest",
	}, {
		.reg = PMBUS_VIRT_READ_TEMP_AVG,
		.attr = "average",
	}, {
		.reg = PMBUS_VIRT_READ_TEMP_MAX,
		.attr = "highest",
	}, {
		.reg = PMBUS_VIRT_RESET_TEMP_HISTORY,
		.attr = "reset_history",
<<<<<<< HEAD
	}
=======
	}, {
		.reg = PMBUS_MFR_MAX_TEMP_1,
		.attr = "rated_max",
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct pmbus_limit_attr temp_limit_attrs2[] = {
	{
		.reg = PMBUS_UT_WARN_LIMIT,
		.low = true,
		.attr = "min",
		.alarm = "min_alarm",
		.sbit = PB_TEMP_UT_WARNING,
	}, {
		.reg = PMBUS_UT_FAULT_LIMIT,
		.low = true,
		.attr = "lcrit",
		.alarm = "lcrit_alarm",
		.sbit = PB_TEMP_UT_FAULT,
	}, {
		.reg = PMBUS_OT_WARN_LIMIT,
		.attr = "max",
		.alarm = "max_alarm",
		.sbit = PB_TEMP_OT_WARNING,
	}, {
		.reg = PMBUS_OT_FAULT_LIMIT,
		.attr = "crit",
		.alarm = "crit_alarm",
		.sbit = PB_TEMP_OT_FAULT,
	}, {
		.reg = PMBUS_VIRT_READ_TEMP2_MIN,
		.attr = "lowest",
	}, {
		.reg = PMBUS_VIRT_READ_TEMP2_AVG,
		.attr = "average",
	}, {
		.reg = PMBUS_VIRT_READ_TEMP2_MAX,
		.attr = "highest",
	}, {
		.reg = PMBUS_VIRT_RESET_TEMP2_HISTORY,
		.attr = "reset_history",
<<<<<<< HEAD
	}
=======
	}, {
		.reg = PMBUS_MFR_MAX_TEMP_2,
		.attr = "rated_max",
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct pmbus_limit_attr temp_limit_attrs3[] = {
	{
		.reg = PMBUS_UT_WARN_LIMIT,
		.low = true,
		.attr = "min",
		.alarm = "min_alarm",
		.sbit = PB_TEMP_UT_WARNING,
	}, {
		.reg = PMBUS_UT_FAULT_LIMIT,
		.low = true,
		.attr = "lcrit",
		.alarm = "lcrit_alarm",
		.sbit = PB_TEMP_UT_FAULT,
	}, {
		.reg = PMBUS_OT_WARN_LIMIT,
		.attr = "max",
		.alarm = "max_alarm",
		.sbit = PB_TEMP_OT_WARNING,
	}, {
		.reg = PMBUS_OT_FAULT_LIMIT,
		.attr = "crit",
		.alarm = "crit_alarm",
		.sbit = PB_TEMP_OT_FAULT,
<<<<<<< HEAD
	}
=======
	}, {
		.reg = PMBUS_MFR_MAX_TEMP_3,
		.attr = "rated_max",
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct pmbus_sensor_attr temp_attributes[] = {
	{
		.reg = PMBUS_READ_TEMPERATURE_1,
		.class = PSC_TEMPERATURE,
		.paged = true,
		.update = true,
		.compare = true,
		.func = PMBUS_HAVE_TEMP,
		.sfunc = PMBUS_HAVE_STATUS_TEMP,
<<<<<<< HEAD
		.sbase = PB_STATUS_TEMP_BASE,
=======
		.sreg = PMBUS_STATUS_TEMPERATURE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.gbit = PB_STATUS_TEMPERATURE,
		.limit = temp_limit_attrs,
		.nlimit = ARRAY_SIZE(temp_limit_attrs),
	}, {
		.reg = PMBUS_READ_TEMPERATURE_2,
		.class = PSC_TEMPERATURE,
		.paged = true,
		.update = true,
		.compare = true,
		.func = PMBUS_HAVE_TEMP2,
		.sfunc = PMBUS_HAVE_STATUS_TEMP,
<<<<<<< HEAD
		.sbase = PB_STATUS_TEMP_BASE,
=======
		.sreg = PMBUS_STATUS_TEMPERATURE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.gbit = PB_STATUS_TEMPERATURE,
		.limit = temp_limit_attrs2,
		.nlimit = ARRAY_SIZE(temp_limit_attrs2),
	}, {
		.reg = PMBUS_READ_TEMPERATURE_3,
		.class = PSC_TEMPERATURE,
		.paged = true,
		.update = true,
		.compare = true,
		.func = PMBUS_HAVE_TEMP3,
		.sfunc = PMBUS_HAVE_STATUS_TEMP,
<<<<<<< HEAD
		.sbase = PB_STATUS_TEMP_BASE,
=======
		.sreg = PMBUS_STATUS_TEMPERATURE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.gbit = PB_STATUS_TEMPERATURE,
		.limit = temp_limit_attrs3,
		.nlimit = ARRAY_SIZE(temp_limit_attrs3),
	}
};

static const int pmbus_fan_registers[] = {
	PMBUS_READ_FAN_SPEED_1,
	PMBUS_READ_FAN_SPEED_2,
	PMBUS_READ_FAN_SPEED_3,
	PMBUS_READ_FAN_SPEED_4
};

<<<<<<< HEAD
static const int pmbus_fan_config_registers[] = {
	PMBUS_FAN_CONFIG_12,
	PMBUS_FAN_CONFIG_12,
	PMBUS_FAN_CONFIG_34,
	PMBUS_FAN_CONFIG_34
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const int pmbus_fan_status_registers[] = {
	PMBUS_STATUS_FAN_12,
	PMBUS_STATUS_FAN_12,
	PMBUS_STATUS_FAN_34,
	PMBUS_STATUS_FAN_34
};

static const u32 pmbus_fan_flags[] = {
	PMBUS_HAVE_FAN12,
	PMBUS_HAVE_FAN12,
	PMBUS_HAVE_FAN34,
	PMBUS_HAVE_FAN34
};

static const u32 pmbus_fan_status_flags[] = {
	PMBUS_HAVE_STATUS_FAN12,
	PMBUS_HAVE_STATUS_FAN12,
	PMBUS_HAVE_STATUS_FAN34,
	PMBUS_HAVE_STATUS_FAN34
};

/* Fans */
<<<<<<< HEAD
static void pmbus_add_fan_attributes(struct i2c_client *client,
				     struct pmbus_data *data)
=======

/* Precondition: FAN_CONFIG_x_y and FAN_COMMAND_x must exist for the fan ID */
static int pmbus_add_fan_ctrl(struct i2c_client *client,
		struct pmbus_data *data, int index, int page, int id,
		u8 config)
{
	struct pmbus_sensor *sensor;

	sensor = pmbus_add_sensor(data, "fan", "target", index, page,
				  0xff, PMBUS_VIRT_FAN_TARGET_1 + id, PSC_FAN,
				  false, false, true);

	if (!sensor)
		return -ENOMEM;

	if (!((data->info->func[page] & PMBUS_HAVE_PWM12) ||
			(data->info->func[page] & PMBUS_HAVE_PWM34)))
		return 0;

	sensor = pmbus_add_sensor(data, "pwm", NULL, index, page,
				  0xff, PMBUS_VIRT_PWM_1 + id, PSC_PWM,
				  false, false, true);

	if (!sensor)
		return -ENOMEM;

	sensor = pmbus_add_sensor(data, "pwm", "enable", index, page,
				  0xff, PMBUS_VIRT_PWM_ENABLE_1 + id, PSC_PWM,
				  true, false, false);

	if (!sensor)
		return -ENOMEM;

	return 0;
}

static int pmbus_add_fan_attributes(struct i2c_client *client,
				    struct pmbus_data *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct pmbus_driver_info *info = data->info;
	int index = 1;
	int page;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (page = 0; page < info->pages; page++) {
		int f;

		for (f = 0; f < ARRAY_SIZE(pmbus_fan_registers); f++) {
			int regval;

			if (!(info->func[page] & pmbus_fan_flags[f]))
				break;

			if (!pmbus_check_word_register(client, page,
						       pmbus_fan_registers[f]))
				break;

			/*
			 * Skip fan if not installed.
			 * Each fan configuration register covers multiple fans,
			 * so we have to do some magic.
			 */
			regval = _pmbus_read_byte_data(client, page,
				pmbus_fan_config_registers[f]);
			if (regval < 0 ||
			    (!(regval & (PB_FAN_1_INSTALLED >> ((f & 1) * 4)))))
				continue;

<<<<<<< HEAD
			pmbus_add_sensor(data, "fan", "input", index, page,
					 pmbus_fan_registers[f], PSC_FAN, true,
					 true);
=======
			if (pmbus_add_sensor(data, "fan", "input", index,
					     page, 0xff, pmbus_fan_registers[f],
					     PSC_FAN, true, true, true) == NULL)
				return -ENOMEM;

			/* Fan control */
			if (pmbus_check_word_register(client, page,
					pmbus_fan_command_registers[f])) {
				ret = pmbus_add_fan_ctrl(client, data, index,
							 page, f, regval);
				if (ret < 0)
					return ret;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/*
			 * Each fan status register covers multiple fans,
			 * so we have to do some magic.
			 */
			if ((info->func[page] & pmbus_fan_status_flags[f]) &&
			    pmbus_check_byte_register(client,
					page, pmbus_fan_status_registers[f])) {
<<<<<<< HEAD
				int base;

				if (f > 1)	/* fan 3, 4 */
					base = PB_STATUS_FAN34_BASE + page;
				else
					base = PB_STATUS_FAN_BASE + page;
				pmbus_add_boolean_reg(data, "fan", "alarm",
					index, base,
					PB_FAN_FAN1_WARNING >> (f & 1));
				pmbus_add_boolean_reg(data, "fan", "fault",
					index, base,
					PB_FAN_FAN1_FAULT >> (f & 1));
=======
				int reg;

				if (f > 1)	/* fan 3, 4 */
					reg = PMBUS_STATUS_FAN_34;
				else
					reg = PMBUS_STATUS_FAN_12;
				ret = pmbus_add_boolean(data, "fan",
					"alarm", index, NULL, NULL, page, reg,
					PB_FAN_FAN1_WARNING >> (f & 1));
				if (ret)
					return ret;
				ret = pmbus_add_boolean(data, "fan",
					"fault", index, NULL, NULL, page, reg,
					PB_FAN_FAN1_FAULT >> (f & 1));
				if (ret)
					return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			index++;
		}
	}
<<<<<<< HEAD
}

static void pmbus_find_attributes(struct i2c_client *client,
				  struct pmbus_data *data)
{
	/* Voltage sensors */
	pmbus_add_sensor_attrs(client, data, "in", voltage_attributes,
			       ARRAY_SIZE(voltage_attributes));

	/* Current sensors */
	pmbus_add_sensor_attrs(client, data, "curr", current_attributes,
			       ARRAY_SIZE(current_attributes));

	/* Power sensors */
	pmbus_add_sensor_attrs(client, data, "power", power_attributes,
			       ARRAY_SIZE(power_attributes));

	/* Temperature sensors */
	pmbus_add_sensor_attrs(client, data, "temp", temp_attributes,
			       ARRAY_SIZE(temp_attributes));

	/* Fans */
	pmbus_add_fan_attributes(client, data);
=======
	return 0;
}

struct pmbus_samples_attr {
	int reg;
	char *name;
};

struct pmbus_samples_reg {
	int page;
	struct pmbus_samples_attr *attr;
	struct device_attribute dev_attr;
};

static struct pmbus_samples_attr pmbus_samples_registers[] = {
	{
		.reg = PMBUS_VIRT_SAMPLES,
		.name = "samples",
	}, {
		.reg = PMBUS_VIRT_IN_SAMPLES,
		.name = "in_samples",
	}, {
		.reg = PMBUS_VIRT_CURR_SAMPLES,
		.name = "curr_samples",
	}, {
		.reg = PMBUS_VIRT_POWER_SAMPLES,
		.name = "power_samples",
	}, {
		.reg = PMBUS_VIRT_TEMP_SAMPLES,
		.name = "temp_samples",
	}
};

#define to_samples_reg(x) container_of(x, struct pmbus_samples_reg, dev_attr)

static ssize_t pmbus_show_samples(struct device *dev,
				  struct device_attribute *devattr, char *buf)
{
	int val;
	struct i2c_client *client = to_i2c_client(dev->parent);
	struct pmbus_samples_reg *reg = to_samples_reg(devattr);
	struct pmbus_data *data = i2c_get_clientdata(client);

	mutex_lock(&data->update_lock);
	val = _pmbus_read_word_data(client, reg->page, 0xff, reg->attr->reg);
	mutex_unlock(&data->update_lock);
	if (val < 0)
		return val;

	return sysfs_emit(buf, "%d\n", val);
}

static ssize_t pmbus_set_samples(struct device *dev,
				 struct device_attribute *devattr,
				 const char *buf, size_t count)
{
	int ret;
	long val;
	struct i2c_client *client = to_i2c_client(dev->parent);
	struct pmbus_samples_reg *reg = to_samples_reg(devattr);
	struct pmbus_data *data = i2c_get_clientdata(client);

	if (kstrtol(buf, 0, &val) < 0)
		return -EINVAL;

	mutex_lock(&data->update_lock);
	ret = _pmbus_write_word_data(client, reg->page, reg->attr->reg, val);
	mutex_unlock(&data->update_lock);

	return ret ? : count;
}

static int pmbus_add_samples_attr(struct pmbus_data *data, int page,
				  struct pmbus_samples_attr *attr)
{
	struct pmbus_samples_reg *reg;

	reg = devm_kzalloc(data->dev, sizeof(*reg), GFP_KERNEL);
	if (!reg)
		return -ENOMEM;

	reg->attr = attr;
	reg->page = page;

	pmbus_dev_attr_init(&reg->dev_attr, attr->name, 0644,
			    pmbus_show_samples, pmbus_set_samples);

	return pmbus_add_attribute(data, &reg->dev_attr.attr);
}

static int pmbus_add_samples_attributes(struct i2c_client *client,
					struct pmbus_data *data)
{
	const struct pmbus_driver_info *info = data->info;
	int s;

	if (!(info->func[0] & PMBUS_HAVE_SAMPLES))
		return 0;

	for (s = 0; s < ARRAY_SIZE(pmbus_samples_registers); s++) {
		struct pmbus_samples_attr *attr;
		int ret;

		attr = &pmbus_samples_registers[s];
		if (!pmbus_check_word_register(client, 0, attr->reg))
			continue;

		ret = pmbus_add_samples_attr(data, 0, attr);
		if (ret)
			return ret;
	}

	return 0;
}

static int pmbus_find_attributes(struct i2c_client *client,
				 struct pmbus_data *data)
{
	int ret;

	/* Voltage sensors */
	ret = pmbus_add_sensor_attrs(client, data, "in", voltage_attributes,
				     ARRAY_SIZE(voltage_attributes));
	if (ret)
		return ret;

	/* Current sensors */
	ret = pmbus_add_sensor_attrs(client, data, "curr", current_attributes,
				     ARRAY_SIZE(current_attributes));
	if (ret)
		return ret;

	/* Power sensors */
	ret = pmbus_add_sensor_attrs(client, data, "power", power_attributes,
				     ARRAY_SIZE(power_attributes));
	if (ret)
		return ret;

	/* Temperature sensors */
	ret = pmbus_add_sensor_attrs(client, data, "temp", temp_attributes,
				     ARRAY_SIZE(temp_attributes));
	if (ret)
		return ret;

	/* Fans */
	ret = pmbus_add_fan_attributes(client, data);
	if (ret)
		return ret;

	ret = pmbus_add_samples_attributes(client, data);
	return ret;
}

/*
 * The pmbus_class_attr_map structure maps one sensor class to
 * it's corresponding sensor attributes array.
 */
struct pmbus_class_attr_map {
	enum pmbus_sensor_classes class;
	int nattr;
	const struct pmbus_sensor_attr *attr;
};

static const struct pmbus_class_attr_map class_attr_map[] = {
	{
		.class = PSC_VOLTAGE_IN,
		.attr = voltage_attributes,
		.nattr = ARRAY_SIZE(voltage_attributes),
	}, {
		.class = PSC_VOLTAGE_OUT,
		.attr = voltage_attributes,
		.nattr = ARRAY_SIZE(voltage_attributes),
	}, {
		.class = PSC_CURRENT_IN,
		.attr = current_attributes,
		.nattr = ARRAY_SIZE(current_attributes),
	}, {
		.class = PSC_CURRENT_OUT,
		.attr = current_attributes,
		.nattr = ARRAY_SIZE(current_attributes),
	}, {
		.class = PSC_POWER,
		.attr = power_attributes,
		.nattr = ARRAY_SIZE(power_attributes),
	}, {
		.class = PSC_TEMPERATURE,
		.attr = temp_attributes,
		.nattr = ARRAY_SIZE(temp_attributes),
	}
};

/*
 * Read the coefficients for direct mode.
 */
static int pmbus_read_coefficients(struct i2c_client *client,
				   struct pmbus_driver_info *info,
				   const struct pmbus_sensor_attr *attr)
{
	int rv;
	union i2c_smbus_data data;
	enum pmbus_sensor_classes class = attr->class;
	s8 R;
	s16 m, b;

	data.block[0] = 2;
	data.block[1] = attr->reg;
	data.block[2] = 0x01;

	rv = i2c_smbus_xfer(client->adapter, client->addr, client->flags,
			    I2C_SMBUS_WRITE, PMBUS_COEFFICIENTS,
			    I2C_SMBUS_BLOCK_PROC_CALL, &data);

	if (rv < 0)
		return rv;

	if (data.block[0] != 5)
		return -EIO;

	m = data.block[1] | (data.block[2] << 8);
	b = data.block[3] | (data.block[4] << 8);
	R = data.block[5];
	info->m[class] = m;
	info->b[class] = b;
	info->R[class] = R;

	return rv;
}

static int pmbus_init_coefficients(struct i2c_client *client,
				   struct pmbus_driver_info *info)
{
	int i, n, ret = -EINVAL;
	const struct pmbus_class_attr_map *map;
	const struct pmbus_sensor_attr *attr;

	for (i = 0; i < ARRAY_SIZE(class_attr_map); i++) {
		map = &class_attr_map[i];
		if (info->format[map->class] != direct)
			continue;
		for (n = 0; n < map->nattr; n++) {
			attr = &map->attr[n];
			if (map->class != attr->class)
				continue;
			ret = pmbus_read_coefficients(client, info, attr);
			if (ret >= 0)
				break;
		}
		if (ret < 0) {
			dev_err(&client->dev,
				"No coefficients found for sensor class %d\n",
				map->class);
			return -EINVAL;
		}
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Identify chip parameters.
 * This function is called for all chips.
 */
static int pmbus_identify_common(struct i2c_client *client,
<<<<<<< HEAD
				 struct pmbus_data *data)
{
	int vout_mode = -1;

	if (pmbus_check_byte_register(client, 0, PMBUS_VOUT_MODE))
		vout_mode = _pmbus_read_byte_data(client, 0, PMBUS_VOUT_MODE);
=======
				 struct pmbus_data *data, int page)
{
	int vout_mode = -1;

	if (pmbus_check_byte_register(client, page, PMBUS_VOUT_MODE))
		vout_mode = _pmbus_read_byte_data(client, page,
						  PMBUS_VOUT_MODE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (vout_mode >= 0 && vout_mode != 0xff) {
		/*
		 * Not all chips support the VOUT_MODE command,
		 * so a failure to read it is not an error.
		 */
		switch (vout_mode >> 5) {
		case 0:	/* linear mode      */
			if (data->info->format[PSC_VOLTAGE_OUT] != linear)
				return -ENODEV;

<<<<<<< HEAD
			data->exponent = ((s8)(vout_mode << 3)) >> 3;
=======
			data->exponent[page] = ((s8)(vout_mode << 3)) >> 3;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case 1: /* VID mode         */
			if (data->info->format[PSC_VOLTAGE_OUT] != vid)
				return -ENODEV;
			break;
		case 2:	/* direct mode      */
			if (data->info->format[PSC_VOLTAGE_OUT] != direct)
				return -ENODEV;
			break;
<<<<<<< HEAD
=======
		case 3:	/* ieee 754 half precision */
			if (data->info->format[PSC_VOLTAGE_OUT] != ieee754)
				return -ENODEV;
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			return -ENODEV;
		}
	}

<<<<<<< HEAD
	/* Determine maximum number of sensors, booleans, and labels */
	pmbus_find_max_attr(client, data);
	pmbus_clear_fault_page(client, 0);
	return 0;
}

int pmbus_do_probe(struct i2c_client *client, const struct i2c_device_id *id,
		   struct pmbus_driver_info *info)
{
	const struct pmbus_platform_data *pdata = client->dev.platform_data;
	struct pmbus_data *data;
	int ret;

	if (!info) {
		dev_err(&client->dev, "Missing chip information");
		return -ENODEV;
	}

=======
	return 0;
}

static int pmbus_read_status_byte(struct i2c_client *client, int page)
{
	return _pmbus_read_byte_data(client, page, PMBUS_STATUS_BYTE);
}

static int pmbus_read_status_word(struct i2c_client *client, int page)
{
	return _pmbus_read_word_data(client, page, 0xff, PMBUS_STATUS_WORD);
}

/* PEC attribute support */

static ssize_t pec_show(struct device *dev, struct device_attribute *dummy,
			char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);

	return sysfs_emit(buf, "%d\n", !!(client->flags & I2C_CLIENT_PEC));
}

static ssize_t pec_store(struct device *dev, struct device_attribute *dummy,
			 const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	bool enable;
	int err;

	err = kstrtobool(buf, &enable);
	if (err < 0)
		return err;

	if (enable)
		client->flags |= I2C_CLIENT_PEC;
	else
		client->flags &= ~I2C_CLIENT_PEC;

	return count;
}

static DEVICE_ATTR_RW(pec);

static void pmbus_remove_pec(void *dev)
{
	device_remove_file(dev, &dev_attr_pec);
}

static int pmbus_init_common(struct i2c_client *client, struct pmbus_data *data,
			     struct pmbus_driver_info *info)
{
	struct device *dev = &client->dev;
	int page, ret;

	/*
	 * Figure out if PEC is enabled before accessing any other register.
	 * Make sure PEC is disabled, will be enabled later if needed.
	 */
	client->flags &= ~I2C_CLIENT_PEC;

	/* Enable PEC if the controller and bus supports it */
	if (!(data->flags & PMBUS_NO_CAPABILITY)) {
		ret = i2c_smbus_read_byte_data(client, PMBUS_CAPABILITY);
		if (ret >= 0 && (ret & PB_CAPABILITY_ERROR_CHECK)) {
			if (i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_PEC))
				client->flags |= I2C_CLIENT_PEC;
		}
	}

	/*
	 * Some PMBus chips don't support PMBUS_STATUS_WORD, so try
	 * to use PMBUS_STATUS_BYTE instead if that is the case.
	 * Bail out if both registers are not supported.
	 */
	data->read_status = pmbus_read_status_word;
	ret = i2c_smbus_read_word_data(client, PMBUS_STATUS_WORD);
	if (ret < 0 || ret == 0xffff) {
		data->read_status = pmbus_read_status_byte;
		ret = i2c_smbus_read_byte_data(client, PMBUS_STATUS_BYTE);
		if (ret < 0 || ret == 0xff) {
			dev_err(dev, "PMBus status register not found\n");
			return -ENODEV;
		}
	} else {
		data->has_status_word = true;
	}

	/*
	 * Check if the chip is write protected. If it is, we can not clear
	 * faults, and we should not try it. Also, in that case, writes into
	 * limit registers need to be disabled.
	 */
	if (!(data->flags & PMBUS_NO_WRITE_PROTECT)) {
		ret = i2c_smbus_read_byte_data(client, PMBUS_WRITE_PROTECT);
		if (ret > 0 && (ret & PB_WP_ANY))
			data->flags |= PMBUS_WRITE_PROTECTED | PMBUS_SKIP_STATUS_CHECK;
	}

	if (data->info->pages)
		pmbus_clear_faults(client);
	else
		pmbus_clear_fault_page(client, -1);

	if (info->identify) {
		ret = (*info->identify)(client, info);
		if (ret < 0) {
			dev_err(dev, "Chip identification failed\n");
			return ret;
		}
	}

	if (info->pages <= 0 || info->pages > PMBUS_PAGES) {
		dev_err(dev, "Bad number of PMBus pages: %d\n", info->pages);
		return -ENODEV;
	}

	for (page = 0; page < info->pages; page++) {
		ret = pmbus_identify_common(client, data, page);
		if (ret < 0) {
			dev_err(dev, "Failed to identify chip capabilities\n");
			return ret;
		}
	}

	if (data->flags & PMBUS_USE_COEFFICIENTS_CMD) {
		if (!i2c_check_functionality(client->adapter,
					     I2C_FUNC_SMBUS_BLOCK_PROC_CALL))
			return -ENODEV;

		ret = pmbus_init_coefficients(client, info);
		if (ret < 0)
			return ret;
	}

	if (client->flags & I2C_CLIENT_PEC) {
		/*
		 * If I2C_CLIENT_PEC is set here, both the I2C adapter and the
		 * chip support PEC. Add 'pec' attribute to client device to let
		 * the user control it.
		 */
		ret = device_create_file(dev, &dev_attr_pec);
		if (ret)
			return ret;
		ret = devm_add_action_or_reset(dev, pmbus_remove_pec, dev);
		if (ret)
			return ret;
	}

	return 0;
}

/* A PMBus status flag and the corresponding REGULATOR_ERROR_* and REGULATOR_EVENTS_* flag */
struct pmbus_status_assoc {
	int pflag, rflag, eflag;
};

/* PMBus->regulator bit mappings for a PMBus status register */
struct pmbus_status_category {
	int func;
	int reg;
	const struct pmbus_status_assoc *bits; /* zero-terminated */
};

static const struct pmbus_status_category __maybe_unused pmbus_status_flag_map[] = {
	{
		.func = PMBUS_HAVE_STATUS_VOUT,
		.reg = PMBUS_STATUS_VOUT,
		.bits = (const struct pmbus_status_assoc[]) {
			{ PB_VOLTAGE_UV_WARNING, REGULATOR_ERROR_UNDER_VOLTAGE_WARN,
			REGULATOR_EVENT_UNDER_VOLTAGE_WARN },
			{ PB_VOLTAGE_UV_FAULT,   REGULATOR_ERROR_UNDER_VOLTAGE,
			REGULATOR_EVENT_UNDER_VOLTAGE },
			{ PB_VOLTAGE_OV_WARNING, REGULATOR_ERROR_OVER_VOLTAGE_WARN,
			REGULATOR_EVENT_OVER_VOLTAGE_WARN },
			{ PB_VOLTAGE_OV_FAULT,   REGULATOR_ERROR_REGULATION_OUT,
			REGULATOR_EVENT_OVER_VOLTAGE_WARN },
			{ },
		},
	}, {
		.func = PMBUS_HAVE_STATUS_IOUT,
		.reg = PMBUS_STATUS_IOUT,
		.bits = (const struct pmbus_status_assoc[]) {
			{ PB_IOUT_OC_WARNING,   REGULATOR_ERROR_OVER_CURRENT_WARN,
			REGULATOR_EVENT_OVER_CURRENT_WARN },
			{ PB_IOUT_OC_FAULT,     REGULATOR_ERROR_OVER_CURRENT,
			REGULATOR_EVENT_OVER_CURRENT },
			{ PB_IOUT_OC_LV_FAULT,  REGULATOR_ERROR_OVER_CURRENT,
			REGULATOR_EVENT_OVER_CURRENT },
			{ },
		},
	}, {
		.func = PMBUS_HAVE_STATUS_TEMP,
		.reg = PMBUS_STATUS_TEMPERATURE,
		.bits = (const struct pmbus_status_assoc[]) {
			{ PB_TEMP_OT_WARNING,    REGULATOR_ERROR_OVER_TEMP_WARN,
			REGULATOR_EVENT_OVER_TEMP_WARN },
			{ PB_TEMP_OT_FAULT,      REGULATOR_ERROR_OVER_TEMP,
			REGULATOR_EVENT_OVER_TEMP },
			{ },
		},
	},
};

static int _pmbus_is_enabled(struct i2c_client *client, u8 page)
{
	int ret;

	ret = _pmbus_read_byte_data(client, page, PMBUS_OPERATION);

	if (ret < 0)
		return ret;

	return !!(ret & PB_OPERATION_CONTROL_ON);
}

static int __maybe_unused pmbus_is_enabled(struct i2c_client *client, u8 page)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	int ret;

	mutex_lock(&data->update_lock);
	ret = _pmbus_is_enabled(client, page);
	mutex_unlock(&data->update_lock);

	return ret;
}

#define to_dev_attr(_dev_attr) \
	container_of(_dev_attr, struct device_attribute, attr)

static void pmbus_notify(struct pmbus_data *data, int page, int reg, int flags)
{
	int i;

	for (i = 0; i < data->num_attributes; i++) {
		struct device_attribute *da = to_dev_attr(data->group.attrs[i]);
		struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
		int index = attr->index;
		u16 smask = pb_index_to_mask(index);
		u8 spage = pb_index_to_page(index);
		u16 sreg = pb_index_to_reg(index);

		if (reg == sreg && page == spage && (smask & flags)) {
			dev_dbg(data->dev, "sysfs notify: %s", da->attr.name);
			sysfs_notify(&data->dev->kobj, NULL, da->attr.name);
			kobject_uevent(&data->dev->kobj, KOBJ_CHANGE);
			flags &= ~smask;
		}

		if (!flags)
			break;
	}
}

static int _pmbus_get_flags(struct pmbus_data *data, u8 page, unsigned int *flags,
			   unsigned int *event, bool notify)
{
	int i, status;
	const struct pmbus_status_category *cat;
	const struct pmbus_status_assoc *bit;
	struct device *dev = data->dev;
	struct i2c_client *client = to_i2c_client(dev);
	int func = data->info->func[page];

	*flags = 0;
	*event = 0;

	for (i = 0; i < ARRAY_SIZE(pmbus_status_flag_map); i++) {
		cat = &pmbus_status_flag_map[i];
		if (!(func & cat->func))
			continue;

		status = _pmbus_read_byte_data(client, page, cat->reg);
		if (status < 0)
			return status;

		for (bit = cat->bits; bit->pflag; bit++)
			if (status & bit->pflag) {
				*flags |= bit->rflag;
				*event |= bit->eflag;
			}

		if (notify && status)
			pmbus_notify(data, page, cat->reg, status);

	}

	/*
	 * Map what bits of STATUS_{WORD,BYTE} we can to REGULATOR_ERROR_*
	 * bits.  Some of the other bits are tempting (especially for cases
	 * where we don't have the relevant PMBUS_HAVE_STATUS_*
	 * functionality), but there's an unfortunate ambiguity in that
	 * they're defined as indicating a fault *or* a warning, so we can't
	 * easily determine whether to report REGULATOR_ERROR_<foo> or
	 * REGULATOR_ERROR_<foo>_WARN.
	 */
	status = pmbus_get_status(client, page, PMBUS_STATUS_WORD);
	if (status < 0)
		return status;

	if (_pmbus_is_enabled(client, page)) {
		if (status & PB_STATUS_OFF) {
			*flags |= REGULATOR_ERROR_FAIL;
			*event |= REGULATOR_EVENT_FAIL;
		}

		if (status & PB_STATUS_POWER_GOOD_N) {
			*flags |= REGULATOR_ERROR_REGULATION_OUT;
			*event |= REGULATOR_EVENT_REGULATION_OUT;
		}
	}
	/*
	 * Unlike most other status bits, PB_STATUS_{IOUT_OC,VOUT_OV} are
	 * defined strictly as fault indicators (not warnings).
	 */
	if (status & PB_STATUS_IOUT_OC) {
		*flags |= REGULATOR_ERROR_OVER_CURRENT;
		*event |= REGULATOR_EVENT_OVER_CURRENT;
	}
	if (status & PB_STATUS_VOUT_OV) {
		*flags |= REGULATOR_ERROR_REGULATION_OUT;
		*event |= REGULATOR_EVENT_FAIL;
	}

	/*
	 * If we haven't discovered any thermal faults or warnings via
	 * PMBUS_STATUS_TEMPERATURE, map PB_STATUS_TEMPERATURE to a warning as
	 * a (conservative) best-effort interpretation.
	 */
	if (!(*flags & (REGULATOR_ERROR_OVER_TEMP | REGULATOR_ERROR_OVER_TEMP_WARN)) &&
	    (status & PB_STATUS_TEMPERATURE)) {
		*flags |= REGULATOR_ERROR_OVER_TEMP_WARN;
		*event |= REGULATOR_EVENT_OVER_TEMP_WARN;
	}


	return 0;
}

static int __maybe_unused pmbus_get_flags(struct pmbus_data *data, u8 page, unsigned int *flags,
					  unsigned int *event, bool notify)
{
	int ret;

	mutex_lock(&data->update_lock);
	ret = _pmbus_get_flags(data, page, flags, event, notify);
	mutex_unlock(&data->update_lock);

	return ret;
}

#if IS_ENABLED(CONFIG_REGULATOR)
static int pmbus_regulator_is_enabled(struct regulator_dev *rdev)
{
	struct device *dev = rdev_get_dev(rdev);
	struct i2c_client *client = to_i2c_client(dev->parent);

	return pmbus_is_enabled(client, rdev_get_id(rdev));
}

static int _pmbus_regulator_on_off(struct regulator_dev *rdev, bool enable)
{
	struct device *dev = rdev_get_dev(rdev);
	struct i2c_client *client = to_i2c_client(dev->parent);
	struct pmbus_data *data = i2c_get_clientdata(client);
	u8 page = rdev_get_id(rdev);
	int ret;

	mutex_lock(&data->update_lock);
	ret = pmbus_update_byte_data(client, page, PMBUS_OPERATION,
				     PB_OPERATION_CONTROL_ON,
				     enable ? PB_OPERATION_CONTROL_ON : 0);
	mutex_unlock(&data->update_lock);

	return ret;
}

static int pmbus_regulator_enable(struct regulator_dev *rdev)
{
	return _pmbus_regulator_on_off(rdev, 1);
}

static int pmbus_regulator_disable(struct regulator_dev *rdev)
{
	return _pmbus_regulator_on_off(rdev, 0);
}

static int pmbus_regulator_get_error_flags(struct regulator_dev *rdev, unsigned int *flags)
{
	struct device *dev = rdev_get_dev(rdev);
	struct i2c_client *client = to_i2c_client(dev->parent);
	struct pmbus_data *data = i2c_get_clientdata(client);
	int event;

	return pmbus_get_flags(data, rdev_get_id(rdev), flags, &event, false);
}

static int pmbus_regulator_get_status(struct regulator_dev *rdev)
{
	struct device *dev = rdev_get_dev(rdev);
	struct i2c_client *client = to_i2c_client(dev->parent);
	struct pmbus_data *data = i2c_get_clientdata(client);
	u8 page = rdev_get_id(rdev);
	int status, ret;
	int event;

	mutex_lock(&data->update_lock);
	status = pmbus_get_status(client, page, PMBUS_STATUS_WORD);
	if (status < 0) {
		ret = status;
		goto unlock;
	}

	if (status & PB_STATUS_OFF) {
		ret = REGULATOR_STATUS_OFF;
		goto unlock;
	}

	/* If regulator is ON & reports power good then return ON */
	if (!(status & PB_STATUS_POWER_GOOD_N)) {
		ret = REGULATOR_STATUS_ON;
		goto unlock;
	}

	ret = _pmbus_get_flags(data, rdev_get_id(rdev), &status, &event, false);
	if (ret)
		goto unlock;

	if (status & (REGULATOR_ERROR_UNDER_VOLTAGE | REGULATOR_ERROR_OVER_CURRENT |
	   REGULATOR_ERROR_REGULATION_OUT | REGULATOR_ERROR_FAIL | REGULATOR_ERROR_OVER_TEMP)) {
		ret = REGULATOR_STATUS_ERROR;
		goto unlock;
	}

	ret = REGULATOR_STATUS_UNDEFINED;

unlock:
	mutex_unlock(&data->update_lock);
	return ret;
}

static int pmbus_regulator_get_low_margin(struct i2c_client *client, int page)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct pmbus_sensor s = {
		.page = page,
		.class = PSC_VOLTAGE_OUT,
		.convert = true,
		.data = -1,
	};

	if (data->vout_low[page] < 0) {
		if (pmbus_check_word_register(client, page, PMBUS_MFR_VOUT_MIN))
			s.data = _pmbus_read_word_data(client, page, 0xff,
						       PMBUS_MFR_VOUT_MIN);
		if (s.data < 0) {
			s.data = _pmbus_read_word_data(client, page, 0xff,
						       PMBUS_VOUT_MARGIN_LOW);
			if (s.data < 0)
				return s.data;
		}
		data->vout_low[page] = pmbus_reg2data(data, &s);
	}

	return data->vout_low[page];
}

static int pmbus_regulator_get_high_margin(struct i2c_client *client, int page)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct pmbus_sensor s = {
		.page = page,
		.class = PSC_VOLTAGE_OUT,
		.convert = true,
		.data = -1,
	};

	if (data->vout_high[page] < 0) {
		if (pmbus_check_word_register(client, page, PMBUS_MFR_VOUT_MAX))
			s.data = _pmbus_read_word_data(client, page, 0xff,
						       PMBUS_MFR_VOUT_MAX);
		if (s.data < 0) {
			s.data = _pmbus_read_word_data(client, page, 0xff,
						       PMBUS_VOUT_MARGIN_HIGH);
			if (s.data < 0)
				return s.data;
		}
		data->vout_high[page] = pmbus_reg2data(data, &s);
	}

	return data->vout_high[page];
}

static int pmbus_regulator_get_voltage(struct regulator_dev *rdev)
{
	struct device *dev = rdev_get_dev(rdev);
	struct i2c_client *client = to_i2c_client(dev->parent);
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct pmbus_sensor s = {
		.page = rdev_get_id(rdev),
		.class = PSC_VOLTAGE_OUT,
		.convert = true,
	};

	s.data = _pmbus_read_word_data(client, s.page, 0xff, PMBUS_READ_VOUT);
	if (s.data < 0)
		return s.data;

	return (int)pmbus_reg2data(data, &s) * 1000; /* unit is uV */
}

static int pmbus_regulator_set_voltage(struct regulator_dev *rdev, int min_uv,
				       int max_uv, unsigned int *selector)
{
	struct device *dev = rdev_get_dev(rdev);
	struct i2c_client *client = to_i2c_client(dev->parent);
	struct pmbus_data *data = i2c_get_clientdata(client);
	struct pmbus_sensor s = {
		.page = rdev_get_id(rdev),
		.class = PSC_VOLTAGE_OUT,
		.convert = true,
		.data = -1,
	};
	int val = DIV_ROUND_CLOSEST(min_uv, 1000); /* convert to mV */
	int low, high;

	*selector = 0;

	low = pmbus_regulator_get_low_margin(client, s.page);
	if (low < 0)
		return low;

	high = pmbus_regulator_get_high_margin(client, s.page);
	if (high < 0)
		return high;

	/* Make sure we are within margins */
	if (low > val)
		val = low;
	if (high < val)
		val = high;

	val = pmbus_data2reg(data, &s, val);

	return _pmbus_write_word_data(client, s.page, PMBUS_VOUT_COMMAND, (u16)val);
}

static int pmbus_regulator_list_voltage(struct regulator_dev *rdev,
					 unsigned int selector)
{
	struct device *dev = rdev_get_dev(rdev);
	struct i2c_client *client = to_i2c_client(dev->parent);
	int val, low, high;

	if (selector >= rdev->desc->n_voltages ||
	    selector < rdev->desc->linear_min_sel)
		return -EINVAL;

	selector -= rdev->desc->linear_min_sel;
	val = DIV_ROUND_CLOSEST(rdev->desc->min_uV +
				(rdev->desc->uV_step * selector), 1000); /* convert to mV */

	low = pmbus_regulator_get_low_margin(client, rdev_get_id(rdev));
	if (low < 0)
		return low;

	high = pmbus_regulator_get_high_margin(client, rdev_get_id(rdev));
	if (high < 0)
		return high;

	if (val >= low && val <= high)
		return val * 1000; /* unit is uV */

	return 0;
}

const struct regulator_ops pmbus_regulator_ops = {
	.enable = pmbus_regulator_enable,
	.disable = pmbus_regulator_disable,
	.is_enabled = pmbus_regulator_is_enabled,
	.get_error_flags = pmbus_regulator_get_error_flags,
	.get_status = pmbus_regulator_get_status,
	.get_voltage = pmbus_regulator_get_voltage,
	.set_voltage = pmbus_regulator_set_voltage,
	.list_voltage = pmbus_regulator_list_voltage,
};
EXPORT_SYMBOL_NS_GPL(pmbus_regulator_ops, PMBUS);

static int pmbus_regulator_register(struct pmbus_data *data)
{
	struct device *dev = data->dev;
	const struct pmbus_driver_info *info = data->info;
	const struct pmbus_platform_data *pdata = dev_get_platdata(dev);
	int i;

	data->rdevs = devm_kzalloc(dev, sizeof(struct regulator_dev *) * info->num_regulators,
				   GFP_KERNEL);
	if (!data->rdevs)
		return -ENOMEM;

	for (i = 0; i < info->num_regulators; i++) {
		struct regulator_config config = { };

		config.dev = dev;
		config.driver_data = data;

		if (pdata && pdata->reg_init_data)
			config.init_data = &pdata->reg_init_data[i];

		data->rdevs[i] = devm_regulator_register(dev, &info->reg_desc[i],
							 &config);
		if (IS_ERR(data->rdevs[i]))
			return dev_err_probe(dev, PTR_ERR(data->rdevs[i]),
					     "Failed to register %s regulator\n",
					     info->reg_desc[i].name);
	}

	return 0;
}

static int pmbus_regulator_notify(struct pmbus_data *data, int page, int event)
{
		int j;

		for (j = 0; j < data->info->num_regulators; j++) {
			if (page == rdev_get_id(data->rdevs[j])) {
				regulator_notifier_call_chain(data->rdevs[j], event, NULL);
				break;
			}
		}
		return 0;
}
#else
static int pmbus_regulator_register(struct pmbus_data *data)
{
	return 0;
}

static int pmbus_regulator_notify(struct pmbus_data *data, int page, int event)
{
		return 0;
}
#endif

static int pmbus_write_smbalert_mask(struct i2c_client *client, u8 page, u8 reg, u8 val)
{
	return _pmbus_write_word_data(client, page, PMBUS_SMBALERT_MASK, reg | (val << 8));
}

static irqreturn_t pmbus_fault_handler(int irq, void *pdata)
{
	struct pmbus_data *data = pdata;
	struct i2c_client *client = to_i2c_client(data->dev);

	int i, status, event;
	mutex_lock(&data->update_lock);
	for (i = 0; i < data->info->pages; i++) {
		_pmbus_get_flags(data, i, &status, &event, true);

		if (event)
			pmbus_regulator_notify(data, i, event);
	}

	pmbus_clear_faults(client);
	mutex_unlock(&data->update_lock);

	return IRQ_HANDLED;
}

static int pmbus_irq_setup(struct i2c_client *client, struct pmbus_data *data)
{
	struct device *dev = &client->dev;
	const struct pmbus_status_category *cat;
	const struct pmbus_status_assoc *bit;
	int i, j, err, func;
	u8 mask;

	static const u8 misc_status[] = {PMBUS_STATUS_CML, PMBUS_STATUS_OTHER,
					 PMBUS_STATUS_MFR_SPECIFIC, PMBUS_STATUS_FAN_12,
					 PMBUS_STATUS_FAN_34};

	if (!client->irq)
		return 0;

	for (i = 0; i < data->info->pages; i++) {
		func = data->info->func[i];

		for (j = 0; j < ARRAY_SIZE(pmbus_status_flag_map); j++) {
			cat = &pmbus_status_flag_map[j];
			if (!(func & cat->func))
				continue;
			mask = 0;
			for (bit = cat->bits; bit->pflag; bit++)
				mask |= bit->pflag;

			err = pmbus_write_smbalert_mask(client, i, cat->reg, ~mask);
			if (err)
				dev_dbg_once(dev, "Failed to set smbalert for reg 0x%02x\n",
					     cat->reg);
		}

		for (j = 0; j < ARRAY_SIZE(misc_status); j++)
			pmbus_write_smbalert_mask(client, i, misc_status[j], 0xff);
	}

	/* Register notifiers */
	err = devm_request_threaded_irq(dev, client->irq, NULL, pmbus_fault_handler,
					IRQF_ONESHOT, "pmbus-irq", data);
	if (err) {
		dev_err(dev, "failed to request an irq %d\n", err);
		return err;
	}

	return 0;
}

static struct dentry *pmbus_debugfs_dir;	/* pmbus debugfs directory */

#if IS_ENABLED(CONFIG_DEBUG_FS)
static int pmbus_debugfs_get(void *data, u64 *val)
{
	int rc;
	struct pmbus_debugfs_entry *entry = data;
	struct pmbus_data *pdata = i2c_get_clientdata(entry->client);

	rc = mutex_lock_interruptible(&pdata->update_lock);
	if (rc)
		return rc;
	rc = _pmbus_read_byte_data(entry->client, entry->page, entry->reg);
	mutex_unlock(&pdata->update_lock);
	if (rc < 0)
		return rc;

	*val = rc;

	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(pmbus_debugfs_ops, pmbus_debugfs_get, NULL,
			 "0x%02llx\n");

static int pmbus_debugfs_get_status(void *data, u64 *val)
{
	int rc;
	struct pmbus_debugfs_entry *entry = data;
	struct pmbus_data *pdata = i2c_get_clientdata(entry->client);

	rc = mutex_lock_interruptible(&pdata->update_lock);
	if (rc)
		return rc;
	rc = pdata->read_status(entry->client, entry->page);
	mutex_unlock(&pdata->update_lock);
	if (rc < 0)
		return rc;

	*val = rc;

	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(pmbus_debugfs_ops_status, pmbus_debugfs_get_status,
			 NULL, "0x%04llx\n");

static ssize_t pmbus_debugfs_mfr_read(struct file *file, char __user *buf,
				       size_t count, loff_t *ppos)
{
	int rc;
	struct pmbus_debugfs_entry *entry = file->private_data;
	struct pmbus_data *pdata = i2c_get_clientdata(entry->client);
	char data[I2C_SMBUS_BLOCK_MAX + 2] = { 0 };

	rc = mutex_lock_interruptible(&pdata->update_lock);
	if (rc)
		return rc;
	rc = pmbus_read_block_data(entry->client, entry->page, entry->reg,
				   data);
	mutex_unlock(&pdata->update_lock);
	if (rc < 0)
		return rc;

	/* Add newline at the end of a read data */
	data[rc] = '\n';

	/* Include newline into the length */
	rc += 1;

	return simple_read_from_buffer(buf, count, ppos, data, rc);
}

static const struct file_operations pmbus_debugfs_ops_mfr = {
	.llseek = noop_llseek,
	.read = pmbus_debugfs_mfr_read,
	.write = NULL,
	.open = simple_open,
};

static void pmbus_remove_debugfs(void *data)
{
	struct dentry *entry = data;

	debugfs_remove_recursive(entry);
}

static int pmbus_init_debugfs(struct i2c_client *client,
			      struct pmbus_data *data)
{
	int i, idx = 0;
	char name[PMBUS_NAME_SIZE];
	struct pmbus_debugfs_entry *entries;

	if (!pmbus_debugfs_dir)
		return -ENODEV;

	/*
	 * Create the debugfs directory for this device. Use the hwmon device
	 * name to avoid conflicts (hwmon numbers are globally unique).
	 */
	data->debugfs = debugfs_create_dir(dev_name(data->hwmon_dev),
					   pmbus_debugfs_dir);
	if (IS_ERR_OR_NULL(data->debugfs)) {
		data->debugfs = NULL;
		return -ENODEV;
	}

	/*
	 * Allocate the max possible entries we need.
	 * 6 entries device-specific
	 * 10 entries page-specific
	 */
	entries = devm_kcalloc(data->dev,
			       6 + data->info->pages * 10, sizeof(*entries),
			       GFP_KERNEL);
	if (!entries)
		return -ENOMEM;

	/*
	 * Add device-specific entries.
	 * Please note that the PMBUS standard allows all registers to be
	 * page-specific.
	 * To reduce the number of debugfs entries for devices with many pages
	 * assume that values of the following registers are the same for all
	 * pages and report values only for page 0.
	 */
	if (pmbus_check_block_register(client, 0, PMBUS_MFR_ID)) {
		entries[idx].client = client;
		entries[idx].page = 0;
		entries[idx].reg = PMBUS_MFR_ID;
		debugfs_create_file("mfr_id", 0444, data->debugfs,
				    &entries[idx++],
				    &pmbus_debugfs_ops_mfr);
	}

	if (pmbus_check_block_register(client, 0, PMBUS_MFR_MODEL)) {
		entries[idx].client = client;
		entries[idx].page = 0;
		entries[idx].reg = PMBUS_MFR_MODEL;
		debugfs_create_file("mfr_model", 0444, data->debugfs,
				    &entries[idx++],
				    &pmbus_debugfs_ops_mfr);
	}

	if (pmbus_check_block_register(client, 0, PMBUS_MFR_REVISION)) {
		entries[idx].client = client;
		entries[idx].page = 0;
		entries[idx].reg = PMBUS_MFR_REVISION;
		debugfs_create_file("mfr_revision", 0444, data->debugfs,
				    &entries[idx++],
				    &pmbus_debugfs_ops_mfr);
	}

	if (pmbus_check_block_register(client, 0, PMBUS_MFR_LOCATION)) {
		entries[idx].client = client;
		entries[idx].page = 0;
		entries[idx].reg = PMBUS_MFR_LOCATION;
		debugfs_create_file("mfr_location", 0444, data->debugfs,
				    &entries[idx++],
				    &pmbus_debugfs_ops_mfr);
	}

	if (pmbus_check_block_register(client, 0, PMBUS_MFR_DATE)) {
		entries[idx].client = client;
		entries[idx].page = 0;
		entries[idx].reg = PMBUS_MFR_DATE;
		debugfs_create_file("mfr_date", 0444, data->debugfs,
				    &entries[idx++],
				    &pmbus_debugfs_ops_mfr);
	}

	if (pmbus_check_block_register(client, 0, PMBUS_MFR_SERIAL)) {
		entries[idx].client = client;
		entries[idx].page = 0;
		entries[idx].reg = PMBUS_MFR_SERIAL;
		debugfs_create_file("mfr_serial", 0444, data->debugfs,
				    &entries[idx++],
				    &pmbus_debugfs_ops_mfr);
	}

	/* Add page specific entries */
	for (i = 0; i < data->info->pages; ++i) {
		/* Check accessibility of status register if it's not page 0 */
		if (!i || pmbus_check_status_register(client, i)) {
			/* No need to set reg as we have special read op. */
			entries[idx].client = client;
			entries[idx].page = i;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops_status);
		}

		if (data->info->func[i] & PMBUS_HAVE_STATUS_VOUT) {
			entries[idx].client = client;
			entries[idx].page = i;
			entries[idx].reg = PMBUS_STATUS_VOUT;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d_vout", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops);
		}

		if (data->info->func[i] & PMBUS_HAVE_STATUS_IOUT) {
			entries[idx].client = client;
			entries[idx].page = i;
			entries[idx].reg = PMBUS_STATUS_IOUT;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d_iout", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops);
		}

		if (data->info->func[i] & PMBUS_HAVE_STATUS_INPUT) {
			entries[idx].client = client;
			entries[idx].page = i;
			entries[idx].reg = PMBUS_STATUS_INPUT;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d_input", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops);
		}

		if (data->info->func[i] & PMBUS_HAVE_STATUS_TEMP) {
			entries[idx].client = client;
			entries[idx].page = i;
			entries[idx].reg = PMBUS_STATUS_TEMPERATURE;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d_temp", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops);
		}

		if (pmbus_check_byte_register(client, i, PMBUS_STATUS_CML)) {
			entries[idx].client = client;
			entries[idx].page = i;
			entries[idx].reg = PMBUS_STATUS_CML;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d_cml", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops);
		}

		if (pmbus_check_byte_register(client, i, PMBUS_STATUS_OTHER)) {
			entries[idx].client = client;
			entries[idx].page = i;
			entries[idx].reg = PMBUS_STATUS_OTHER;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d_other", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops);
		}

		if (pmbus_check_byte_register(client, i,
					      PMBUS_STATUS_MFR_SPECIFIC)) {
			entries[idx].client = client;
			entries[idx].page = i;
			entries[idx].reg = PMBUS_STATUS_MFR_SPECIFIC;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d_mfr", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops);
		}

		if (data->info->func[i] & PMBUS_HAVE_STATUS_FAN12) {
			entries[idx].client = client;
			entries[idx].page = i;
			entries[idx].reg = PMBUS_STATUS_FAN_12;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d_fan12", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops);
		}

		if (data->info->func[i] & PMBUS_HAVE_STATUS_FAN34) {
			entries[idx].client = client;
			entries[idx].page = i;
			entries[idx].reg = PMBUS_STATUS_FAN_34;
			scnprintf(name, PMBUS_NAME_SIZE, "status%d_fan34", i);
			debugfs_create_file(name, 0444, data->debugfs,
					    &entries[idx++],
					    &pmbus_debugfs_ops);
		}
	}

	return devm_add_action_or_reset(data->dev,
					pmbus_remove_debugfs, data->debugfs);
}
#else
static int pmbus_init_debugfs(struct i2c_client *client,
			      struct pmbus_data *data)
{
	return 0;
}
#endif	/* IS_ENABLED(CONFIG_DEBUG_FS) */

int pmbus_do_probe(struct i2c_client *client, struct pmbus_driver_info *info)
{
	struct device *dev = &client->dev;
	const struct pmbus_platform_data *pdata = dev_get_platdata(dev);
	struct pmbus_data *data;
	size_t groups_num = 0;
	int ret;
	int i;
	char *name;

	if (!info)
		return -ENODEV;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_WRITE_BYTE
				     | I2C_FUNC_SMBUS_BYTE_DATA
				     | I2C_FUNC_SMBUS_WORD_DATA))
		return -ENODEV;

<<<<<<< HEAD
	data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
	if (!data) {
		dev_err(&client->dev, "No memory to allocate driver data\n");
		return -ENOMEM;
	}

	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);

	/* Bail out if PMBus status register does not exist. */
	if (i2c_smbus_read_byte_data(client, PMBUS_STATUS_BYTE) < 0) {
		dev_err(&client->dev, "PMBus status register not found\n");
		return -ENODEV;
	}
=======
	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	if (info->groups)
		while (info->groups[groups_num])
			groups_num++;

	data->groups = devm_kcalloc(dev, groups_num + 2, sizeof(void *),
				    GFP_KERNEL);
	if (!data->groups)
		return -ENOMEM;

	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);
	data->dev = dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (pdata)
		data->flags = pdata->flags;
	data->info = info;
<<<<<<< HEAD

	pmbus_clear_faults(client);

	if (info->identify) {
		ret = (*info->identify)(client, info);
		if (ret < 0) {
			dev_err(&client->dev, "Chip identification failed\n");
			return ret;
		}
	}

	if (info->pages <= 0 || info->pages > PMBUS_PAGES) {
		dev_err(&client->dev, "Bad number of PMBus pages: %d\n",
			info->pages);
		return -ENODEV;
	}

	ret = pmbus_identify_common(client, data);
	if (ret < 0) {
		dev_err(&client->dev, "Failed to identify chip capabilities\n");
		return ret;
	}

	ret = -ENOMEM;
	data->sensors = devm_kzalloc(&client->dev, sizeof(struct pmbus_sensor)
				     * data->max_sensors, GFP_KERNEL);
	if (!data->sensors) {
		dev_err(&client->dev, "No memory to allocate sensor data\n");
		return -ENOMEM;
	}

	data->booleans = devm_kzalloc(&client->dev, sizeof(struct pmbus_boolean)
				 * data->max_booleans, GFP_KERNEL);
	if (!data->booleans) {
		dev_err(&client->dev, "No memory to allocate boolean data\n");
		return -ENOMEM;
	}

	data->labels = devm_kzalloc(&client->dev, sizeof(struct pmbus_label)
				    * data->max_labels, GFP_KERNEL);
	if (!data->labels) {
		dev_err(&client->dev, "No memory to allocate label data\n");
		return -ENOMEM;
	}

	data->attributes = devm_kzalloc(&client->dev, sizeof(struct attribute *)
					* data->max_attributes, GFP_KERNEL);
	if (!data->attributes) {
		dev_err(&client->dev, "No memory to allocate attribute data\n");
		return -ENOMEM;
	}

	pmbus_find_attributes(client, data);
=======
	data->currpage = -1;
	data->currphase = -1;

	for (i = 0; i < ARRAY_SIZE(data->vout_low); i++) {
		data->vout_low[i] = -1;
		data->vout_high[i] = -1;
	}

	ret = pmbus_init_common(client, data, info);
	if (ret < 0)
		return ret;

	ret = pmbus_find_attributes(client, data);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If there are no attributes, something is wrong.
	 * Bail out instead of trying to register nothing.
	 */
	if (!data->num_attributes) {
<<<<<<< HEAD
		dev_err(&client->dev, "No attributes found\n");
		return -ENODEV;
	}

	/* Register sysfs hooks */
	data->group.attrs = data->attributes;
	ret = sysfs_create_group(&client->dev.kobj, &data->group);
	if (ret) {
		dev_err(&client->dev, "Failed to create sysfs entries\n");
		return ret;
	}
	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		ret = PTR_ERR(data->hwmon_dev);
		dev_err(&client->dev, "Failed to register hwmon device\n");
		goto out_hwmon_device_register;
	}
	return 0;

out_hwmon_device_register:
	sysfs_remove_group(&client->dev.kobj, &data->group);
	return ret;
}
EXPORT_SYMBOL_GPL(pmbus_do_probe);

int pmbus_do_remove(struct i2c_client *client)
{
	struct pmbus_data *data = i2c_get_clientdata(client);
	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &data->group);
	return 0;
}
EXPORT_SYMBOL_GPL(pmbus_do_remove);
=======
		dev_err(dev, "No attributes found\n");
		return -ENODEV;
	}

	name = devm_kstrdup(dev, client->name, GFP_KERNEL);
	if (!name)
		return -ENOMEM;
	strreplace(name, '-', '_');

	data->groups[0] = &data->group;
	memcpy(data->groups + 1, info->groups, sizeof(void *) * groups_num);
	data->hwmon_dev = devm_hwmon_device_register_with_groups(dev,
					name, data, data->groups);
	if (IS_ERR(data->hwmon_dev)) {
		dev_err(dev, "Failed to register hwmon device\n");
		return PTR_ERR(data->hwmon_dev);
	}

	ret = pmbus_regulator_register(data);
	if (ret)
		return ret;

	ret = pmbus_irq_setup(client, data);
	if (ret)
		return ret;

	ret = pmbus_init_debugfs(client, data);
	if (ret)
		dev_warn(dev, "Failed to register debugfs\n");

	return 0;
}
EXPORT_SYMBOL_NS_GPL(pmbus_do_probe, PMBUS);

struct dentry *pmbus_get_debugfs_dir(struct i2c_client *client)
{
	struct pmbus_data *data = i2c_get_clientdata(client);

	return data->debugfs;
}
EXPORT_SYMBOL_NS_GPL(pmbus_get_debugfs_dir, PMBUS);

int pmbus_lock_interruptible(struct i2c_client *client)
{
	struct pmbus_data *data = i2c_get_clientdata(client);

	return mutex_lock_interruptible(&data->update_lock);
}
EXPORT_SYMBOL_NS_GPL(pmbus_lock_interruptible, PMBUS);

void pmbus_unlock(struct i2c_client *client)
{
	struct pmbus_data *data = i2c_get_clientdata(client);

	mutex_unlock(&data->update_lock);
}
EXPORT_SYMBOL_NS_GPL(pmbus_unlock, PMBUS);

static int __init pmbus_core_init(void)
{
	pmbus_debugfs_dir = debugfs_create_dir("pmbus", NULL);
	if (IS_ERR(pmbus_debugfs_dir))
		pmbus_debugfs_dir = NULL;

	return 0;
}

static void __exit pmbus_core_exit(void)
{
	debugfs_remove_recursive(pmbus_debugfs_dir);
}

module_init(pmbus_core_init);
module_exit(pmbus_core_exit);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Guenter Roeck");
MODULE_DESCRIPTION("PMBus core driver");
MODULE_LICENSE("GPL");
