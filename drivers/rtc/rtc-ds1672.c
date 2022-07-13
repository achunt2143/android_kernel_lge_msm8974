<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * An rtc/i2c driver for the Dallas DS1672
 * Copyright 2005-06 Tower Technologies
 *
 * Author: Alessandro Zummo <a.zummo@towertech.it>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/i2c.h>
#include <linux/rtc.h>
#include <linux/module.h>

<<<<<<< HEAD
#define DRV_VERSION "0.4"

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Registers */

#define DS1672_REG_CNT_BASE	0
#define DS1672_REG_CONTROL	4
#define DS1672_REG_TRICKLE	5

#define DS1672_REG_CONTROL_EOSC	0x80

<<<<<<< HEAD
static struct i2c_driver ds1672_driver;

/*
 * In the routines that deal directly with the ds1672 hardware, we use
 * rtc_time -- month 0-11, hour 0-23, yr = calendar year-epoch
 * Epoch is initialized as 2000. Time is set to UTC.
 */
static int ds1672_get_datetime(struct i2c_client *client, struct rtc_time *tm)
{
	unsigned long time;
	unsigned char addr = DS1672_REG_CNT_BASE;
	unsigned char buf[4];

	struct i2c_msg msgs[] = {
		{client->addr, 0, 1, &addr},	/* setup read ptr */
		{client->addr, I2C_M_RD, 4, buf},	/* read date */
	};

=======
/*
 * In the routines that deal directly with the ds1672 hardware, we use
 * rtc_time -- month 0-11, hour 0-23, yr = calendar year-epoch
 * Time is set to UTC.
 */
static int ds1672_read_time(struct device *dev, struct rtc_time *tm)
{
	struct i2c_client *client = to_i2c_client(dev);
	unsigned long time;
	unsigned char addr = DS1672_REG_CONTROL;
	unsigned char buf[4];

	struct i2c_msg msgs[] = {
		{/* setup read ptr */
			.addr = client->addr,
			.len = 1,
			.buf = &addr
		},
		{/* read date */
			.addr = client->addr,
			.flags = I2C_M_RD,
			.len = 1,
			.buf = buf
		},
	};

	/* read control register */
	if ((i2c_transfer(client->adapter, &msgs[0], 2)) != 2) {
		dev_warn(&client->dev, "Unable to read the control register\n");
		return -EIO;
	}

	if (buf[0] & DS1672_REG_CONTROL_EOSC) {
		dev_warn(&client->dev, "Oscillator not enabled. Set time to enable.\n");
		return -EINVAL;
	}

	addr = DS1672_REG_CNT_BASE;
	msgs[1].len = 4;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* read date registers */
	if ((i2c_transfer(client->adapter, &msgs[0], 2)) != 2) {
		dev_err(&client->dev, "%s: read error\n", __func__);
		return -EIO;
	}

	dev_dbg(&client->dev,
		"%s: raw read data - counters=%02x,%02x,%02x,%02x\n",
		__func__, buf[0], buf[1], buf[2], buf[3]);

<<<<<<< HEAD
	time = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];

	rtc_time_to_tm(time, tm);

	dev_dbg(&client->dev, "%s: tm is secs=%d, mins=%d, hours=%d, "
		"mday=%d, mon=%d, year=%d, wday=%d\n",
		__func__, tm->tm_sec, tm->tm_min, tm->tm_hour,
		tm->tm_mday, tm->tm_mon, tm->tm_year, tm->tm_wday);
=======
	time = ((unsigned long)buf[3] << 24) | (buf[2] << 16) |
	       (buf[1] << 8) | buf[0];

	rtc_time64_to_tm(time, tm);

	dev_dbg(&client->dev, "%s: tm is %ptR\n", __func__, tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int ds1672_set_mmss(struct i2c_client *client, unsigned long secs)
{
	int xfer;
	unsigned char buf[6];
=======
static int ds1672_set_time(struct device *dev, struct rtc_time *tm)
{
	struct i2c_client *client = to_i2c_client(dev);
	int xfer;
	unsigned char buf[6];
	unsigned long secs = rtc_tm_to_time64(tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	buf[0] = DS1672_REG_CNT_BASE;
	buf[1] = secs & 0x000000FF;
	buf[2] = (secs & 0x0000FF00) >> 8;
	buf[3] = (secs & 0x00FF0000) >> 16;
	buf[4] = (secs & 0xFF000000) >> 24;
	buf[5] = 0;		/* set control reg to enable counting */

	xfer = i2c_master_send(client, buf, 6);
	if (xfer != 6) {
		dev_err(&client->dev, "%s: send: %d\n", __func__, xfer);
		return -EIO;
	}

	return 0;
}

<<<<<<< HEAD
static int ds1672_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	return ds1672_get_datetime(to_i2c_client(dev), tm);
}

static int ds1672_rtc_set_mmss(struct device *dev, unsigned long secs)
{
	return ds1672_set_mmss(to_i2c_client(dev), secs);
}

static int ds1672_get_control(struct i2c_client *client, u8 *status)
{
	unsigned char addr = DS1672_REG_CONTROL;

	struct i2c_msg msgs[] = {
		{client->addr, 0, 1, &addr},	/* setup read ptr */
		{client->addr, I2C_M_RD, 1, status},	/* read control */
	};

	/* read control register */
	if ((i2c_transfer(client->adapter, &msgs[0], 2)) != 2) {
		dev_err(&client->dev, "%s: read error\n", __func__);
		return -EIO;
	}

	return 0;
}

/* following are the sysfs callback functions */
static ssize_t show_control(struct device *dev, struct device_attribute *attr,
			    char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 control;
	int err;

	err = ds1672_get_control(client, &control);
	if (err)
		return err;

	return sprintf(buf, "%s\n", (control & DS1672_REG_CONTROL_EOSC)
		       ? "disabled" : "enabled");
}

static DEVICE_ATTR(control, S_IRUGO, show_control, NULL);

static const struct rtc_class_ops ds1672_rtc_ops = {
	.read_time = ds1672_rtc_read_time,
	.set_mmss = ds1672_rtc_set_mmss,
};

static int ds1672_remove(struct i2c_client *client)
{
	struct rtc_device *rtc = i2c_get_clientdata(client);

	if (rtc)
		rtc_device_unregister(rtc);

	return 0;
}

static int ds1672_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int err = 0;
	u8 control;
=======
static const struct rtc_class_ops ds1672_rtc_ops = {
	.read_time = ds1672_read_time,
	.set_time = ds1672_set_time,
};

static int ds1672_probe(struct i2c_client *client)
{
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rtc_device *rtc;

	dev_dbg(&client->dev, "%s\n", __func__);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
		return -ENODEV;

<<<<<<< HEAD
	dev_info(&client->dev, "chip found, driver version " DRV_VERSION "\n");

	rtc = rtc_device_register(ds1672_driver.driver.name, &client->dev,
				  &ds1672_rtc_ops, THIS_MODULE);

	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	i2c_set_clientdata(client, rtc);

	/* read control register */
	err = ds1672_get_control(client, &control);
	if (err)
		goto exit_devreg;

	if (control & DS1672_REG_CONTROL_EOSC)
		dev_warn(&client->dev, "Oscillator not enabled. "
			 "Set time to enable.\n");

	/* Register sysfs hooks */
	err = device_create_file(&client->dev, &dev_attr_control);
	if (err)
		goto exit_devreg;

	return 0;

 exit_devreg:
	rtc_device_unregister(rtc);
	return err;
}

static struct i2c_device_id ds1672_id[] = {
	{ "ds1672", 0 },
	{ }
};
=======
	rtc = devm_rtc_allocate_device(&client->dev);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	rtc->ops = &ds1672_rtc_ops;
	rtc->range_max = U32_MAX;

	err = devm_rtc_register_device(rtc);
	if (err)
		return err;

	i2c_set_clientdata(client, rtc);

	return 0;
}

static const struct i2c_device_id ds1672_id[] = {
	{ "ds1672", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ds1672_id);

static const __maybe_unused struct of_device_id ds1672_of_match[] = {
	{ .compatible = "dallas,ds1672" },
	{ }
};
MODULE_DEVICE_TABLE(of, ds1672_of_match);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct i2c_driver ds1672_driver = {
	.driver = {
		   .name = "rtc-ds1672",
<<<<<<< HEAD
		   },
	.probe = &ds1672_probe,
	.remove = &ds1672_remove,
=======
		   .of_match_table = of_match_ptr(ds1672_of_match),
	},
	.probe = ds1672_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table = ds1672_id,
};

module_i2c_driver(ds1672_driver);

MODULE_AUTHOR("Alessandro Zummo <a.zummo@towertech.it>");
MODULE_DESCRIPTION("Dallas/Maxim DS1672 timekeeper driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_VERSION(DRV_VERSION);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
