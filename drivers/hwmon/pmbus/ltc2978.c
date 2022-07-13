<<<<<<< HEAD
/*
 * Hardware monitoring driver for LTC2978 and LTC3880
 *
 * Copyright (c) 2011 Ericsson AB.
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

=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Hardware monitoring driver for LTC2978 and compatible chips.
 *
 * Copyright (c) 2011 Ericsson AB.
 * Copyright (c) 2013, 2014, 2015 Guenter Roeck
 * Copyright (c) 2015 Linear Technology
 * Copyright (c) 2018 Analog Devices Inc.
 */

#include <linux/delay.h>
#include <linux/jiffies.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/i2c.h>
<<<<<<< HEAD
#include "pmbus.h"

enum chips { ltc2978, ltc3880 };

/* LTC2978 and LTC3880 */
#define LTC2978_MFR_VOUT_PEAK		0xdd
#define LTC2978_MFR_VIN_PEAK		0xde
#define LTC2978_MFR_TEMPERATURE_PEAK	0xdf
#define LTC2978_MFR_SPECIAL_ID		0xe7

/* LTC2978 only */
=======
#include <linux/regulator/driver.h>
#include "pmbus.h"

enum chips {
	/* Managers */
	ltc2972, ltc2974, ltc2975, ltc2977, ltc2978, ltc2979, ltc2980,
	/* Controllers */
	ltc3880, ltc3882, ltc3883, ltc3884, ltc3886, ltc3887, ltc3889, ltc7132, ltc7880,
	/* Modules */
	ltm2987, ltm4664, ltm4675, ltm4676, ltm4677, ltm4678, ltm4680, ltm4686,
	ltm4700,
};

/* Common for all chips */
#define LTC2978_MFR_VOUT_PEAK		0xdd
#define LTC2978_MFR_VIN_PEAK		0xde
#define LTC2978_MFR_TEMPERATURE_PEAK	0xdf
#define LTC2978_MFR_SPECIAL_ID		0xe7	/* Undocumented on LTC3882 */
#define LTC2978_MFR_COMMON		0xef

/* LTC2974, LTC2975, LCT2977, LTC2980, LTC2978, and LTM2987 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LTC2978_MFR_VOUT_MIN		0xfb
#define LTC2978_MFR_VIN_MIN		0xfc
#define LTC2978_MFR_TEMPERATURE_MIN	0xfd

<<<<<<< HEAD
/* LTC3880 only */
=======
/* LTC2974, LTC2975 */
#define LTC2974_MFR_IOUT_PEAK		0xd7
#define LTC2974_MFR_IOUT_MIN		0xd8

/* LTC3880, LTC3882, LTC3883, LTC3887, LTM4675, LTM4676, LTC7132 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LTC3880_MFR_IOUT_PEAK		0xd7
#define LTC3880_MFR_CLEAR_PEAKS		0xe3
#define LTC3880_MFR_TEMPERATURE2_PEAK	0xf4

<<<<<<< HEAD
#define LTC2978_ID_REV1			0x0121
#define LTC2978_ID_REV2			0x0122
#define LTC3880_ID			0x4000
#define LTC3880_ID_MASK			0xff00
=======
/* LTC3883, LTC3884, LTC3886, LTC3889, LTC7132, LTC7880 */
#define LTC3883_MFR_IIN_PEAK		0xe1

/* LTC2975 only */
#define LTC2975_MFR_IIN_PEAK		0xc4
#define LTC2975_MFR_IIN_MIN		0xc5
#define LTC2975_MFR_PIN_PEAK		0xc6
#define LTC2975_MFR_PIN_MIN		0xc7

#define LTC2978_ID_MASK			0xfff0

#define LTC2972_ID			0x0310
#define LTC2974_ID			0x0210
#define LTC2975_ID			0x0220
#define LTC2977_ID			0x0130
#define LTC2978_ID_REV1			0x0110	/* Early revision */
#define LTC2978_ID_REV2			0x0120
#define LTC2979_ID_A			0x8060
#define LTC2979_ID_B			0x8070
#define LTC2980_ID_A			0x8030	/* A/B for two die IDs */
#define LTC2980_ID_B			0x8040
#define LTC3880_ID			0x4020
#define LTC3882_ID			0x4200
#define LTC3882_ID_D1			0x4240	/* Dash 1 */
#define LTC3883_ID			0x4300
#define LTC3884_ID			0x4C00
#define LTC3886_ID			0x4600
#define LTC3887_ID			0x4700
#define LTC3889_ID			0x4900
#define LTC7132_ID			0x4CE0
#define LTC7880_ID			0x49E0
#define LTM2987_ID_A			0x8010	/* A/B for two die IDs */
#define LTM2987_ID_B			0x8020
#define LTM4664_ID			0x4120
#define LTM4675_ID			0x47a0
#define LTM4676_ID_REV1			0x4400
#define LTM4676_ID_REV2			0x4480
#define LTM4676A_ID			0x47e0
#define LTM4677_ID_REV1			0x47B0
#define LTM4677_ID_REV2			0x47D0
#define LTM4678_ID_REV1			0x4100
#define LTM4678_ID_REV2			0x4110
#define LTM4680_ID			0x4140
#define LTM4686_ID			0x4770
#define LTM4700_ID			0x4130

#define LTC2972_NUM_PAGES		2
#define LTC2974_NUM_PAGES		4
#define LTC2978_NUM_PAGES		8
#define LTC3880_NUM_PAGES		2
#define LTC3883_NUM_PAGES		1

#define LTC_POLL_TIMEOUT		100	/* in milli-seconds */

#define LTC_NOT_BUSY			BIT(6)
#define LTC_NOT_PENDING			BIT(5)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * LTC2978 clears peak data whenever the CLEAR_FAULTS command is executed, which
 * happens pretty much each time chip data is updated. Raw peak data therefore
 * does not provide much value. To be able to provide useful peak data, keep an
 * internal cache of measured peak data, which is only cleared if an explicit
 * "clear peak" command is executed for the sensor in question.
 */
<<<<<<< HEAD
struct ltc2978_data {
	enum chips id;
	int vin_min, vin_max;
	int temp_min, temp_max[2];
	int vout_min[8], vout_max[8];
	int iout_max[2];
	int temp2_max;
	struct pmbus_driver_info info;
};

#define to_ltc2978_data(x)  container_of(x, struct ltc2978_data, info)

=======

struct ltc2978_data {
	enum chips id;
	u16 vin_min, vin_max;
	u16 temp_min[LTC2974_NUM_PAGES], temp_max[LTC2974_NUM_PAGES];
	u16 vout_min[LTC2978_NUM_PAGES], vout_max[LTC2978_NUM_PAGES];
	u16 iout_min[LTC2974_NUM_PAGES], iout_max[LTC2974_NUM_PAGES];
	u16 iin_min, iin_max;
	u16 pin_min, pin_max;
	u16 temp2_max;
	struct pmbus_driver_info info;
	u32 features;
};
#define to_ltc2978_data(x)  container_of(x, struct ltc2978_data, info)

#define FEAT_CLEAR_PEAKS	BIT(0)
#define FEAT_NEEDS_POLLING	BIT(1)

#define has_clear_peaks(d)	((d)->features & FEAT_CLEAR_PEAKS)
#define needs_polling(d)	((d)->features & FEAT_NEEDS_POLLING)

static int ltc_wait_ready(struct i2c_client *client)
{
	unsigned long timeout = jiffies + msecs_to_jiffies(LTC_POLL_TIMEOUT);
	const struct pmbus_driver_info *info = pmbus_get_driver_info(client);
	struct ltc2978_data *data = to_ltc2978_data(info);
	int status;
	u8 mask;

	if (!needs_polling(data))
		return 0;

	/*
	 * LTC3883 does not support LTC_NOT_PENDING, even though
	 * the datasheet claims that it does.
	 */
	mask = LTC_NOT_BUSY;
	if (data->id != ltc3883)
		mask |= LTC_NOT_PENDING;

	do {
		status = pmbus_read_byte_data(client, 0, LTC2978_MFR_COMMON);
		if (status == -EBADMSG || status == -ENXIO) {
			/* PEC error or NACK: chip may be busy, try again */
			usleep_range(50, 100);
			continue;
		}
		if (status < 0)
			return status;

		if ((status & mask) == mask)
			return 0;

		usleep_range(50, 100);
	} while (time_before(jiffies, timeout));

	return -ETIMEDOUT;
}

static int ltc_read_word_data(struct i2c_client *client, int page, int phase,
			      int reg)
{
	int ret;

	ret = ltc_wait_ready(client);
	if (ret < 0)
		return ret;

	return pmbus_read_word_data(client, page, 0xff, reg);
}

static int ltc_read_byte_data(struct i2c_client *client, int page, int reg)
{
	int ret;

	ret = ltc_wait_ready(client);
	if (ret < 0)
		return ret;

	return pmbus_read_byte_data(client, page, reg);
}

static int ltc_write_byte_data(struct i2c_client *client, int page, int reg, u8 value)
{
	int ret;

	ret = ltc_wait_ready(client);
	if (ret < 0)
		return ret;

	return pmbus_write_byte_data(client, page, reg, value);
}

static int ltc_write_byte(struct i2c_client *client, int page, u8 byte)
{
	int ret;

	ret = ltc_wait_ready(client);
	if (ret < 0)
		return ret;

	return pmbus_write_byte(client, page, byte);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int lin11_to_val(int data)
{
	s16 e = ((s16)data) >> 11;
	s32 m = (((s16)(data << 5)) >> 5);

	/*
	 * mantissa is 10 bit + sign, exponent adds up to 15 bit.
	 * Add 6 bit to exponent for maximum accuracy (10 + 15 + 6 = 31).
	 */
	e += 6;
	return (e < 0 ? m >> -e : m << e);
}

<<<<<<< HEAD
=======
static int ltc_get_max(struct ltc2978_data *data, struct i2c_client *client,
		       int page, int reg, u16 *pmax)
{
	int ret;

	ret = ltc_read_word_data(client, page, 0xff, reg);
	if (ret >= 0) {
		if (lin11_to_val(ret) > lin11_to_val(*pmax))
			*pmax = ret;
		ret = *pmax;
	}
	return ret;
}

static int ltc_get_min(struct ltc2978_data *data, struct i2c_client *client,
		       int page, int reg, u16 *pmin)
{
	int ret;

	ret = ltc_read_word_data(client, page, 0xff, reg);
	if (ret >= 0) {
		if (lin11_to_val(ret) < lin11_to_val(*pmin))
			*pmin = ret;
		ret = *pmin;
	}
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ltc2978_read_word_data_common(struct i2c_client *client, int page,
					 int reg)
{
	const struct pmbus_driver_info *info = pmbus_get_driver_info(client);
	struct ltc2978_data *data = to_ltc2978_data(info);
	int ret;

	switch (reg) {
	case PMBUS_VIRT_READ_VIN_MAX:
<<<<<<< HEAD
		ret = pmbus_read_word_data(client, page, LTC2978_MFR_VIN_PEAK);
		if (ret >= 0) {
			if (lin11_to_val(ret) > lin11_to_val(data->vin_max))
				data->vin_max = ret;
			ret = data->vin_max;
		}
		break;
	case PMBUS_VIRT_READ_VOUT_MAX:
		ret = pmbus_read_word_data(client, page, LTC2978_MFR_VOUT_PEAK);
=======
		ret = ltc_get_max(data, client, page, LTC2978_MFR_VIN_PEAK,
				  &data->vin_max);
		break;
	case PMBUS_VIRT_READ_VOUT_MAX:
		ret = ltc_read_word_data(client, page, 0xff,
					 LTC2978_MFR_VOUT_PEAK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret >= 0) {
			/*
			 * VOUT is 16 bit unsigned with fixed exponent,
			 * so we can compare it directly
			 */
			if (ret > data->vout_max[page])
				data->vout_max[page] = ret;
			ret = data->vout_max[page];
		}
		break;
	case PMBUS_VIRT_READ_TEMP_MAX:
<<<<<<< HEAD
		ret = pmbus_read_word_data(client, page,
					   LTC2978_MFR_TEMPERATURE_PEAK);
		if (ret >= 0) {
			if (lin11_to_val(ret)
			    > lin11_to_val(data->temp_max[page]))
				data->temp_max[page] = ret;
			ret = data->temp_max[page];
		}
=======
		ret = ltc_get_max(data, client, page,
				  LTC2978_MFR_TEMPERATURE_PEAK,
				  &data->temp_max[page]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case PMBUS_VIRT_RESET_VOUT_HISTORY:
	case PMBUS_VIRT_RESET_VIN_HISTORY:
	case PMBUS_VIRT_RESET_TEMP_HISTORY:
		ret = 0;
		break;
	default:
<<<<<<< HEAD
=======
		ret = ltc_wait_ready(client);
		if (ret < 0)
			return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ENODATA;
		break;
	}
	return ret;
}

<<<<<<< HEAD
static int ltc2978_read_word_data(struct i2c_client *client, int page, int reg)
=======
static int ltc2978_read_word_data(struct i2c_client *client, int page,
				  int phase, int reg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct pmbus_driver_info *info = pmbus_get_driver_info(client);
	struct ltc2978_data *data = to_ltc2978_data(info);
	int ret;

	switch (reg) {
	case PMBUS_VIRT_READ_VIN_MIN:
<<<<<<< HEAD
		ret = pmbus_read_word_data(client, page, LTC2978_MFR_VIN_MIN);
		if (ret >= 0) {
			if (lin11_to_val(ret) < lin11_to_val(data->vin_min))
				data->vin_min = ret;
			ret = data->vin_min;
		}
		break;
	case PMBUS_VIRT_READ_VOUT_MIN:
		ret = pmbus_read_word_data(client, page, LTC2978_MFR_VOUT_MIN);
=======
		ret = ltc_get_min(data, client, page, LTC2978_MFR_VIN_MIN,
				  &data->vin_min);
		break;
	case PMBUS_VIRT_READ_VOUT_MIN:
		ret = ltc_read_word_data(client, page, phase,
					 LTC2978_MFR_VOUT_MIN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret >= 0) {
			/*
			 * VOUT_MIN is known to not be supported on some lots
			 * of LTC2978 revision 1, and will return the maximum
			 * possible voltage if read. If VOUT_MAX is valid and
			 * lower than the reading of VOUT_MIN, use it instead.
			 */
			if (data->vout_max[page] && ret > data->vout_max[page])
				ret = data->vout_max[page];
			if (ret < data->vout_min[page])
				data->vout_min[page] = ret;
			ret = data->vout_min[page];
		}
		break;
	case PMBUS_VIRT_READ_TEMP_MIN:
<<<<<<< HEAD
		ret = pmbus_read_word_data(client, page,
					   LTC2978_MFR_TEMPERATURE_MIN);
		if (ret >= 0) {
			if (lin11_to_val(ret)
			    < lin11_to_val(data->temp_min))
				data->temp_min = ret;
			ret = data->temp_min;
		}
=======
		ret = ltc_get_min(data, client, page,
				  LTC2978_MFR_TEMPERATURE_MIN,
				  &data->temp_min[page]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case PMBUS_VIRT_READ_IOUT_MAX:
	case PMBUS_VIRT_RESET_IOUT_HISTORY:
	case PMBUS_VIRT_READ_TEMP2_MAX:
	case PMBUS_VIRT_RESET_TEMP2_HISTORY:
		ret = -ENXIO;
		break;
	default:
		ret = ltc2978_read_word_data_common(client, page, reg);
		break;
	}
	return ret;
}

<<<<<<< HEAD
static int ltc3880_read_word_data(struct i2c_client *client, int page, int reg)
=======
static int ltc2974_read_word_data(struct i2c_client *client, int page,
				  int phase, int reg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct pmbus_driver_info *info = pmbus_get_driver_info(client);
	struct ltc2978_data *data = to_ltc2978_data(info);
	int ret;

	switch (reg) {
	case PMBUS_VIRT_READ_IOUT_MAX:
<<<<<<< HEAD
		ret = pmbus_read_word_data(client, page, LTC3880_MFR_IOUT_PEAK);
		if (ret >= 0) {
			if (lin11_to_val(ret)
			    > lin11_to_val(data->iout_max[page]))
				data->iout_max[page] = ret;
			ret = data->iout_max[page];
		}
		break;
	case PMBUS_VIRT_READ_TEMP2_MAX:
		ret = pmbus_read_word_data(client, page,
					   LTC3880_MFR_TEMPERATURE2_PEAK);
		if (ret >= 0) {
			if (lin11_to_val(ret) > lin11_to_val(data->temp2_max))
				data->temp2_max = ret;
			ret = data->temp2_max;
		}
=======
		ret = ltc_get_max(data, client, page, LTC2974_MFR_IOUT_PEAK,
				  &data->iout_max[page]);
		break;
	case PMBUS_VIRT_READ_IOUT_MIN:
		ret = ltc_get_min(data, client, page, LTC2974_MFR_IOUT_MIN,
				  &data->iout_min[page]);
		break;
	case PMBUS_VIRT_RESET_IOUT_HISTORY:
		ret = 0;
		break;
	default:
		ret = ltc2978_read_word_data(client, page, phase, reg);
		break;
	}
	return ret;
}

static int ltc2975_read_word_data(struct i2c_client *client, int page,
				  int phase, int reg)
{
	const struct pmbus_driver_info *info = pmbus_get_driver_info(client);
	struct ltc2978_data *data = to_ltc2978_data(info);
	int ret;

	switch (reg) {
	case PMBUS_VIRT_READ_IIN_MAX:
		ret = ltc_get_max(data, client, page, LTC2975_MFR_IIN_PEAK,
				  &data->iin_max);
		break;
	case PMBUS_VIRT_READ_IIN_MIN:
		ret = ltc_get_min(data, client, page, LTC2975_MFR_IIN_MIN,
				  &data->iin_min);
		break;
	case PMBUS_VIRT_READ_PIN_MAX:
		ret = ltc_get_max(data, client, page, LTC2975_MFR_PIN_PEAK,
				  &data->pin_max);
		break;
	case PMBUS_VIRT_READ_PIN_MIN:
		ret = ltc_get_min(data, client, page, LTC2975_MFR_PIN_MIN,
				  &data->pin_min);
		break;
	case PMBUS_VIRT_RESET_IIN_HISTORY:
	case PMBUS_VIRT_RESET_PIN_HISTORY:
		ret = 0;
		break;
	default:
		ret = ltc2978_read_word_data(client, page, phase, reg);
		break;
	}
	return ret;
}

static int ltc3880_read_word_data(struct i2c_client *client, int page,
				  int phase, int reg)
{
	const struct pmbus_driver_info *info = pmbus_get_driver_info(client);
	struct ltc2978_data *data = to_ltc2978_data(info);
	int ret;

	switch (reg) {
	case PMBUS_VIRT_READ_IOUT_MAX:
		ret = ltc_get_max(data, client, page, LTC3880_MFR_IOUT_PEAK,
				  &data->iout_max[page]);
		break;
	case PMBUS_VIRT_READ_TEMP2_MAX:
		ret = ltc_get_max(data, client, page,
				  LTC3880_MFR_TEMPERATURE2_PEAK,
				  &data->temp2_max);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case PMBUS_VIRT_READ_VIN_MIN:
	case PMBUS_VIRT_READ_VOUT_MIN:
	case PMBUS_VIRT_READ_TEMP_MIN:
		ret = -ENXIO;
		break;
	case PMBUS_VIRT_RESET_IOUT_HISTORY:
	case PMBUS_VIRT_RESET_TEMP2_HISTORY:
		ret = 0;
		break;
	default:
		ret = ltc2978_read_word_data_common(client, page, reg);
		break;
	}
	return ret;
}

<<<<<<< HEAD
static int ltc2978_clear_peaks(struct i2c_client *client, int page,
			       enum chips id)
{
	int ret;

	if (id == ltc2978)
		ret = pmbus_write_byte(client, page, PMBUS_CLEAR_FAULTS);
	else
		ret = pmbus_write_byte(client, 0, LTC3880_MFR_CLEAR_PEAKS);
=======
static int ltc3883_read_word_data(struct i2c_client *client, int page,
				  int phase, int reg)
{
	const struct pmbus_driver_info *info = pmbus_get_driver_info(client);
	struct ltc2978_data *data = to_ltc2978_data(info);
	int ret;

	switch (reg) {
	case PMBUS_VIRT_READ_IIN_MAX:
		ret = ltc_get_max(data, client, page, LTC3883_MFR_IIN_PEAK,
				  &data->iin_max);
		break;
	case PMBUS_VIRT_RESET_IIN_HISTORY:
		ret = 0;
		break;
	default:
		ret = ltc3880_read_word_data(client, page, phase, reg);
		break;
	}
	return ret;
}

static int ltc2978_clear_peaks(struct ltc2978_data *data,
			       struct i2c_client *client, int page)
{
	int ret;

	if (has_clear_peaks(data))
		ret = ltc_write_byte(client, 0, LTC3880_MFR_CLEAR_PEAKS);
	else
		ret = ltc_write_byte(client, page, PMBUS_CLEAR_FAULTS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static int ltc2978_write_word_data(struct i2c_client *client, int page,
				    int reg, u16 word)
{
	const struct pmbus_driver_info *info = pmbus_get_driver_info(client);
	struct ltc2978_data *data = to_ltc2978_data(info);
	int ret;

	switch (reg) {
<<<<<<< HEAD
	case PMBUS_VIRT_RESET_IOUT_HISTORY:
		data->iout_max[page] = 0x7c00;
		ret = ltc2978_clear_peaks(client, page, data->id);
		break;
	case PMBUS_VIRT_RESET_TEMP2_HISTORY:
		data->temp2_max = 0x7c00;
		ret = ltc2978_clear_peaks(client, page, data->id);
=======
	case PMBUS_VIRT_RESET_IIN_HISTORY:
		data->iin_max = 0x7c00;
		data->iin_min = 0x7bff;
		ret = ltc2978_clear_peaks(data, client, 0);
		break;
	case PMBUS_VIRT_RESET_PIN_HISTORY:
		data->pin_max = 0x7c00;
		data->pin_min = 0x7bff;
		ret = ltc2978_clear_peaks(data, client, 0);
		break;
	case PMBUS_VIRT_RESET_IOUT_HISTORY:
		data->iout_max[page] = 0x7c00;
		data->iout_min[page] = 0xfbff;
		ret = ltc2978_clear_peaks(data, client, page);
		break;
	case PMBUS_VIRT_RESET_TEMP2_HISTORY:
		data->temp2_max = 0x7c00;
		ret = ltc2978_clear_peaks(data, client, page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case PMBUS_VIRT_RESET_VOUT_HISTORY:
		data->vout_min[page] = 0xffff;
		data->vout_max[page] = 0;
<<<<<<< HEAD
		ret = ltc2978_clear_peaks(client, page, data->id);
=======
		ret = ltc2978_clear_peaks(data, client, page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case PMBUS_VIRT_RESET_VIN_HISTORY:
		data->vin_min = 0x7bff;
		data->vin_max = 0x7c00;
<<<<<<< HEAD
		ret = ltc2978_clear_peaks(client, page, data->id);
		break;
	case PMBUS_VIRT_RESET_TEMP_HISTORY:
		data->temp_min = 0x7bff;
		data->temp_max[page] = 0x7c00;
		ret = ltc2978_clear_peaks(client, page, data->id);
		break;
	default:
=======
		ret = ltc2978_clear_peaks(data, client, page);
		break;
	case PMBUS_VIRT_RESET_TEMP_HISTORY:
		data->temp_min[page] = 0x7bff;
		data->temp_max[page] = 0x7c00;
		ret = ltc2978_clear_peaks(data, client, page);
		break;
	default:
		ret = ltc_wait_ready(client);
		if (ret < 0)
			return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ENODATA;
		break;
	}
	return ret;
}

static const struct i2c_device_id ltc2978_id[] = {
<<<<<<< HEAD
	{"ltc2978", ltc2978},
	{"ltc3880", ltc3880},
=======
	{"ltc2972", ltc2972},
	{"ltc2974", ltc2974},
	{"ltc2975", ltc2975},
	{"ltc2977", ltc2977},
	{"ltc2978", ltc2978},
	{"ltc2979", ltc2979},
	{"ltc2980", ltc2980},
	{"ltc3880", ltc3880},
	{"ltc3882", ltc3882},
	{"ltc3883", ltc3883},
	{"ltc3884", ltc3884},
	{"ltc3886", ltc3886},
	{"ltc3887", ltc3887},
	{"ltc3889", ltc3889},
	{"ltc7132", ltc7132},
	{"ltc7880", ltc7880},
	{"ltm2987", ltm2987},
	{"ltm4664", ltm4664},
	{"ltm4675", ltm4675},
	{"ltm4676", ltm4676},
	{"ltm4677", ltm4677},
	{"ltm4678", ltm4678},
	{"ltm4680", ltm4680},
	{"ltm4686", ltm4686},
	{"ltm4700", ltm4700},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{}
};
MODULE_DEVICE_TABLE(i2c, ltc2978_id);

<<<<<<< HEAD
static int ltc2978_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	int chip_id, i;
	struct ltc2978_data *data;
	struct pmbus_driver_info *info;
=======
#if IS_ENABLED(CONFIG_SENSORS_LTC2978_REGULATOR)
#define LTC2978_ADC_RES	0xFFFF
#define LTC2978_N_ADC	122
#define LTC2978_MAX_UV	(LTC2978_ADC_RES * LTC2978_N_ADC)
#define LTC2978_UV_STEP	1000
#define LTC2978_N_VOLTAGES	((LTC2978_MAX_UV / LTC2978_UV_STEP) + 1)

static const struct regulator_desc ltc2978_reg_desc[] = {
	PMBUS_REGULATOR_STEP("vout", 0, LTC2978_N_VOLTAGES, LTC2978_UV_STEP, 0),
	PMBUS_REGULATOR_STEP("vout", 1, LTC2978_N_VOLTAGES, LTC2978_UV_STEP, 0),
	PMBUS_REGULATOR_STEP("vout", 2, LTC2978_N_VOLTAGES, LTC2978_UV_STEP, 0),
	PMBUS_REGULATOR_STEP("vout", 3, LTC2978_N_VOLTAGES, LTC2978_UV_STEP, 0),
	PMBUS_REGULATOR_STEP("vout", 4, LTC2978_N_VOLTAGES, LTC2978_UV_STEP, 0),
	PMBUS_REGULATOR_STEP("vout", 5, LTC2978_N_VOLTAGES, LTC2978_UV_STEP, 0),
	PMBUS_REGULATOR_STEP("vout", 6, LTC2978_N_VOLTAGES, LTC2978_UV_STEP, 0),
	PMBUS_REGULATOR_STEP("vout", 7, LTC2978_N_VOLTAGES, LTC2978_UV_STEP, 0),
};

static const struct regulator_desc ltc2978_reg_desc_default[] = {
	PMBUS_REGULATOR("vout", 0),
	PMBUS_REGULATOR("vout", 1),
	PMBUS_REGULATOR("vout", 2),
	PMBUS_REGULATOR("vout", 3),
	PMBUS_REGULATOR("vout", 4),
	PMBUS_REGULATOR("vout", 5),
	PMBUS_REGULATOR("vout", 6),
	PMBUS_REGULATOR("vout", 7),
};
#endif /* CONFIG_SENSORS_LTC2978_REGULATOR */

static int ltc2978_get_id(struct i2c_client *client)
{
	int chip_id;

	chip_id = i2c_smbus_read_word_data(client, LTC2978_MFR_SPECIAL_ID);
	if (chip_id < 0) {
		const struct i2c_device_id *id;
		u8 buf[I2C_SMBUS_BLOCK_MAX];
		int ret;

		if (!i2c_check_functionality(client->adapter,
					     I2C_FUNC_SMBUS_READ_BLOCK_DATA))
			return -ENODEV;

		ret = i2c_smbus_read_block_data(client, PMBUS_MFR_ID, buf);
		if (ret < 0)
			return ret;
		if (ret < 3 || strncmp(buf, "LTC", 3))
			return -ENODEV;

		ret = i2c_smbus_read_block_data(client, PMBUS_MFR_MODEL, buf);
		if (ret < 0)
			return ret;
		for (id = &ltc2978_id[0]; strlen(id->name); id++) {
			if (!strncasecmp(id->name, buf, strlen(id->name)))
				return (int)id->driver_data;
		}
		return -ENODEV;
	}

	chip_id &= LTC2978_ID_MASK;

	if (chip_id == LTC2972_ID)
		return ltc2972;
	else if (chip_id == LTC2974_ID)
		return ltc2974;
	else if (chip_id == LTC2975_ID)
		return ltc2975;
	else if (chip_id == LTC2977_ID)
		return ltc2977;
	else if (chip_id == LTC2978_ID_REV1 || chip_id == LTC2978_ID_REV2)
		return ltc2978;
	else if (chip_id == LTC2979_ID_A || chip_id == LTC2979_ID_B)
		return ltc2979;
	else if (chip_id == LTC2980_ID_A || chip_id == LTC2980_ID_B)
		return ltc2980;
	else if (chip_id == LTC3880_ID)
		return ltc3880;
	else if (chip_id == LTC3882_ID || chip_id == LTC3882_ID_D1)
		return ltc3882;
	else if (chip_id == LTC3883_ID)
		return ltc3883;
	else if (chip_id == LTC3884_ID)
		return ltc3884;
	else if (chip_id == LTC3886_ID)
		return ltc3886;
	else if (chip_id == LTC3887_ID)
		return ltc3887;
	else if (chip_id == LTC3889_ID)
		return ltc3889;
	else if (chip_id == LTC7132_ID)
		return ltc7132;
	else if (chip_id == LTC7880_ID)
		return ltc7880;
	else if (chip_id == LTM2987_ID_A || chip_id == LTM2987_ID_B)
		return ltm2987;
	else if (chip_id == LTM4664_ID)
		return ltm4664;
	else if (chip_id == LTM4675_ID)
		return ltm4675;
	else if (chip_id == LTM4676_ID_REV1 || chip_id == LTM4676_ID_REV2 ||
		 chip_id == LTM4676A_ID)
		return ltm4676;
	else if (chip_id == LTM4677_ID_REV1 || chip_id == LTM4677_ID_REV2)
		return ltm4677;
	else if (chip_id == LTM4678_ID_REV1 || chip_id == LTM4678_ID_REV2)
		return ltm4678;
	else if (chip_id == LTM4680_ID)
		return ltm4680;
	else if (chip_id == LTM4686_ID)
		return ltm4686;
	else if (chip_id == LTM4700_ID)
		return ltm4700;

	dev_err(&client->dev, "Unsupported chip ID 0x%x\n", chip_id);
	return -ENODEV;
}

static int ltc2978_probe(struct i2c_client *client)
{
	int i, chip_id;
	struct ltc2978_data *data;
	struct pmbus_driver_info *info;
	const struct i2c_device_id *id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!i2c_check_functionality(client->adapter,
				     I2C_FUNC_SMBUS_READ_WORD_DATA))
		return -ENODEV;

	data = devm_kzalloc(&client->dev, sizeof(struct ltc2978_data),
			    GFP_KERNEL);
	if (!data)
		return -ENOMEM;

<<<<<<< HEAD
	chip_id = i2c_smbus_read_word_data(client, LTC2978_MFR_SPECIAL_ID);
	if (chip_id < 0)
		return chip_id;

	if (chip_id == LTC2978_ID_REV1 || chip_id == LTC2978_ID_REV2) {
		data->id = ltc2978;
	} else if ((chip_id & LTC3880_ID_MASK) == LTC3880_ID) {
		data->id = ltc3880;
	} else {
		dev_err(&client->dev, "Unsupported chip ID 0x%x\n", chip_id);
		return -ENODEV;
	}
	if (data->id != id->driver_data)
		dev_warn(&client->dev,
			 "Device mismatch: Configured %s, detected %s\n",
			 id->name,
			 ltc2978_id[data->id].name);

	info = &data->info;
	info->write_word_data = ltc2978_write_word_data;

	data->vin_min = 0x7bff;
	data->vin_max = 0x7c00;
	data->temp_min = 0x7bff;
=======
	chip_id = ltc2978_get_id(client);
	if (chip_id < 0)
		return chip_id;

	data->id = chip_id;
	id = i2c_match_id(ltc2978_id, client);
	if (data->id != id->driver_data)
		dev_warn(&client->dev,
			 "Device mismatch: Configured %s (%d), detected %d\n",
			 id->name,
			 (int) id->driver_data,
			 chip_id);

	info = &data->info;
	info->write_word_data = ltc2978_write_word_data;
	info->write_byte = ltc_write_byte;
	info->write_byte_data = ltc_write_byte_data;
	info->read_word_data = ltc_read_word_data;
	info->read_byte_data = ltc_read_byte_data;

	data->vin_min = 0x7bff;
	data->vin_max = 0x7c00;
	for (i = 0; i < ARRAY_SIZE(data->vout_min); i++)
		data->vout_min[i] = 0xffff;
	for (i = 0; i < ARRAY_SIZE(data->iout_min); i++)
		data->iout_min[i] = 0xfbff;
	for (i = 0; i < ARRAY_SIZE(data->iout_max); i++)
		data->iout_max[i] = 0x7c00;
	for (i = 0; i < ARRAY_SIZE(data->temp_min); i++)
		data->temp_min[i] = 0x7bff;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < ARRAY_SIZE(data->temp_max); i++)
		data->temp_max[i] = 0x7c00;
	data->temp2_max = 0x7c00;

	switch (data->id) {
<<<<<<< HEAD
	case ltc2978:
		info->read_word_data = ltc2978_read_word_data;
		info->pages = 8;
		info->func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_STATUS_INPUT
		  | PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT
		  | PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP;
		for (i = 1; i < 8; i++) {
=======
	case ltc2972:
		info->read_word_data = ltc2975_read_word_data;
		info->pages = LTC2972_NUM_PAGES;
		info->func[0] = PMBUS_HAVE_IIN | PMBUS_HAVE_PIN
		  | PMBUS_HAVE_VIN | PMBUS_HAVE_STATUS_INPUT
		  | PMBUS_HAVE_TEMP2;
		for (i = 0; i < info->pages; i++) {
			info->func[i] |= PMBUS_HAVE_VOUT
			  | PMBUS_HAVE_STATUS_VOUT | PMBUS_HAVE_POUT
			  | PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP
			  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT;
		}
		break;
	case ltc2974:
		info->read_word_data = ltc2974_read_word_data;
		info->pages = LTC2974_NUM_PAGES;
		info->func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_STATUS_INPUT
		  | PMBUS_HAVE_TEMP2;
		for (i = 0; i < info->pages; i++) {
			info->func[i] |= PMBUS_HAVE_VOUT
			  | PMBUS_HAVE_STATUS_VOUT | PMBUS_HAVE_POUT
			  | PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP
			  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT;
		}
		break;
	case ltc2975:
		info->read_word_data = ltc2975_read_word_data;
		info->pages = LTC2974_NUM_PAGES;
		info->func[0] = PMBUS_HAVE_IIN | PMBUS_HAVE_PIN
		  | PMBUS_HAVE_VIN | PMBUS_HAVE_STATUS_INPUT
		  | PMBUS_HAVE_TEMP2;
		for (i = 0; i < info->pages; i++) {
			info->func[i] |= PMBUS_HAVE_VOUT
			  | PMBUS_HAVE_STATUS_VOUT | PMBUS_HAVE_POUT
			  | PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP
			  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT;
		}
		break;

	case ltc2977:
	case ltc2978:
	case ltc2979:
	case ltc2980:
	case ltm2987:
		info->read_word_data = ltc2978_read_word_data;
		info->pages = LTC2978_NUM_PAGES;
		info->func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_STATUS_INPUT
		  | PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT
		  | PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP;
		for (i = 1; i < LTC2978_NUM_PAGES; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			info->func[i] = PMBUS_HAVE_VOUT
			  | PMBUS_HAVE_STATUS_VOUT;
		}
		break;
	case ltc3880:
<<<<<<< HEAD
		info->read_word_data = ltc3880_read_word_data;
		info->pages = 2;
=======
	case ltc3887:
	case ltm4675:
	case ltm4676:
	case ltm4677:
	case ltm4686:
		data->features |= FEAT_CLEAR_PEAKS | FEAT_NEEDS_POLLING;
		info->read_word_data = ltc3880_read_word_data;
		info->pages = LTC3880_NUM_PAGES;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		info->func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_IIN
		  | PMBUS_HAVE_STATUS_INPUT
		  | PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT
		  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT
		  | PMBUS_HAVE_POUT | PMBUS_HAVE_TEMP
		  | PMBUS_HAVE_TEMP2 | PMBUS_HAVE_STATUS_TEMP;
		info->func[1] = PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT
		  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT
		  | PMBUS_HAVE_POUT
		  | PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP;
<<<<<<< HEAD
		data->iout_max[0] = 0x7c00;
		data->iout_max[1] = 0x7c00;
=======
		break;
	case ltc3882:
		data->features |= FEAT_CLEAR_PEAKS | FEAT_NEEDS_POLLING;
		info->read_word_data = ltc3880_read_word_data;
		info->pages = LTC3880_NUM_PAGES;
		info->func[0] = PMBUS_HAVE_VIN
		  | PMBUS_HAVE_STATUS_INPUT
		  | PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT
		  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT
		  | PMBUS_HAVE_POUT | PMBUS_HAVE_TEMP
		  | PMBUS_HAVE_TEMP2 | PMBUS_HAVE_STATUS_TEMP;
		info->func[1] = PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT
		  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT
		  | PMBUS_HAVE_POUT
		  | PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP;
		break;
	case ltc3883:
		data->features |= FEAT_CLEAR_PEAKS | FEAT_NEEDS_POLLING;
		info->read_word_data = ltc3883_read_word_data;
		info->pages = LTC3883_NUM_PAGES;
		info->func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_IIN
		  | PMBUS_HAVE_STATUS_INPUT
		  | PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT
		  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT
		  | PMBUS_HAVE_PIN | PMBUS_HAVE_POUT | PMBUS_HAVE_TEMP
		  | PMBUS_HAVE_TEMP2 | PMBUS_HAVE_STATUS_TEMP;
		break;
	case ltc3884:
	case ltc3886:
	case ltc3889:
	case ltc7132:
	case ltc7880:
	case ltm4664:
	case ltm4678:
	case ltm4680:
	case ltm4700:
		data->features |= FEAT_CLEAR_PEAKS | FEAT_NEEDS_POLLING;
		info->read_word_data = ltc3883_read_word_data;
		info->pages = LTC3880_NUM_PAGES;
		info->func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_IIN
		  | PMBUS_HAVE_STATUS_INPUT
		  | PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT
		  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT
		  | PMBUS_HAVE_PIN | PMBUS_HAVE_POUT | PMBUS_HAVE_TEMP
		  | PMBUS_HAVE_TEMP2 | PMBUS_HAVE_STATUS_TEMP;
		info->func[1] = PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT
		  | PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT
		  | PMBUS_HAVE_POUT
		  | PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -ENODEV;
	}
<<<<<<< HEAD
	for (i = 0; i < info->pages; i++)
		data->vout_min[i] = 0xffff;

	return pmbus_do_probe(client, id, info);
}

/* This is the driver that will be inserted */
static struct i2c_driver ltc2978_driver = {
	.driver = {
		   .name = "ltc2978",
		   },
	.probe = ltc2978_probe,
	.remove = pmbus_do_remove,
=======

#if IS_ENABLED(CONFIG_SENSORS_LTC2978_REGULATOR)
	info->num_regulators = info->pages;
	switch (data->id) {
	case ltc2972:
	case ltc2974:
	case ltc2975:
	case ltc2977:
	case ltc2978:
	case ltc2979:
	case ltc2980:
	case ltm2987:
		info->reg_desc = ltc2978_reg_desc;
		if (info->num_regulators > ARRAY_SIZE(ltc2978_reg_desc)) {
			dev_warn(&client->dev, "num_regulators too large!");
			info->num_regulators = ARRAY_SIZE(ltc2978_reg_desc);
		}
		break;
	default:
		info->reg_desc = ltc2978_reg_desc_default;
		if (info->num_regulators > ARRAY_SIZE(ltc2978_reg_desc_default)) {
			dev_warn(&client->dev, "num_regulators too large!");
			info->num_regulators =
			    ARRAY_SIZE(ltc2978_reg_desc_default);
		}
		break;
	}
#endif

	return pmbus_do_probe(client, info);
}


#ifdef CONFIG_OF
static const struct of_device_id ltc2978_of_match[] = {
	{ .compatible = "lltc,ltc2972" },
	{ .compatible = "lltc,ltc2974" },
	{ .compatible = "lltc,ltc2975" },
	{ .compatible = "lltc,ltc2977" },
	{ .compatible = "lltc,ltc2978" },
	{ .compatible = "lltc,ltc2979" },
	{ .compatible = "lltc,ltc2980" },
	{ .compatible = "lltc,ltc3880" },
	{ .compatible = "lltc,ltc3882" },
	{ .compatible = "lltc,ltc3883" },
	{ .compatible = "lltc,ltc3884" },
	{ .compatible = "lltc,ltc3886" },
	{ .compatible = "lltc,ltc3887" },
	{ .compatible = "lltc,ltc3889" },
	{ .compatible = "lltc,ltc7132" },
	{ .compatible = "lltc,ltc7880" },
	{ .compatible = "lltc,ltm2987" },
	{ .compatible = "lltc,ltm4664" },
	{ .compatible = "lltc,ltm4675" },
	{ .compatible = "lltc,ltm4676" },
	{ .compatible = "lltc,ltm4677" },
	{ .compatible = "lltc,ltm4678" },
	{ .compatible = "lltc,ltm4680" },
	{ .compatible = "lltc,ltm4686" },
	{ .compatible = "lltc,ltm4700" },
	{ }
};
MODULE_DEVICE_TABLE(of, ltc2978_of_match);
#endif

static struct i2c_driver ltc2978_driver = {
	.driver = {
		   .name = "ltc2978",
		   .of_match_table = of_match_ptr(ltc2978_of_match),
		   },
	.probe = ltc2978_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table = ltc2978_id,
};

module_i2c_driver(ltc2978_driver);

MODULE_AUTHOR("Guenter Roeck");
<<<<<<< HEAD
MODULE_DESCRIPTION("PMBus driver for LTC2978 and LTC3880");
MODULE_LICENSE("GPL");
=======
MODULE_DESCRIPTION("PMBus driver for LTC2978 and compatible chips");
MODULE_LICENSE("GPL");
MODULE_IMPORT_NS(PMBUS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
