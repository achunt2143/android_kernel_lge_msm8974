<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ST1232 Touchscreen Controller Driver
 *
 * Copyright (C) 2010 Renesas Solutions Corp.
 *	Tony SIM <chinyeow.sim.xt@renesas.com>
 *
 * Using code from:
 *  - android.git.kernel.org: projects/kernel/common.git: synaptics_i2c_rmi.c
 *	Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/module.h>
=======
 */

#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/input/touchscreen.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pm_qos.h>
#include <linux/slab.h>
#include <linux/types.h>

#define ST1232_TS_NAME	"st1232-ts"
<<<<<<< HEAD

#define MIN_X		0x00
#define MIN_Y		0x00
#define MAX_X		0x31f	/* (800 - 1) */
#define MAX_Y		0x1df	/* (480 - 1) */
#define MAX_AREA	0xff
#define MAX_FINGERS	2

struct st1232_ts_finger {
	u16 x;
	u16 y;
	u8 t;
	bool is_valid;
=======
#define ST1633_TS_NAME	"st1633-ts"

#define REG_STATUS		0x01	/* Device Status | Error Code */

#define STATUS_NORMAL		0x00
#define STATUS_INIT		0x01
#define STATUS_ERROR		0x02
#define STATUS_AUTO_TUNING	0x03
#define STATUS_IDLE		0x04
#define STATUS_POWER_DOWN	0x05

#define ERROR_NONE		0x00
#define ERROR_INVALID_ADDRESS	0x10
#define ERROR_INVALID_VALUE	0x20
#define ERROR_INVALID_PLATFORM	0x30

#define REG_XY_RESOLUTION	0x04
#define REG_XY_COORDINATES	0x12
#define ST_TS_MAX_FINGERS	10

struct st_chip_info {
	bool	have_z;
	u16	max_area;
	u16	max_fingers;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct st1232_ts_data {
	struct i2c_client *client;
	struct input_dev *input_dev;
<<<<<<< HEAD
	struct st1232_ts_finger finger[MAX_FINGERS];
	struct dev_pm_qos_request low_latency_req;
};

static int st1232_ts_read_data(struct st1232_ts_data *ts)
{
	struct st1232_ts_finger *finger = ts->finger;
	struct i2c_client *client = ts->client;
	struct i2c_msg msg[2];
	int error;
	u8 start_reg;
	u8 buf[10];

	/* read touchscreen data from ST1232 */
	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = &start_reg;
	start_reg = 0x10;

	msg[1].addr = ts->client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = sizeof(buf);
	msg[1].buf = buf;

	error = i2c_transfer(client->adapter, msg, 2);
	if (error < 0)
		return error;

	/* get "valid" bits */
	finger[0].is_valid = buf[2] >> 7;
	finger[1].is_valid = buf[5] >> 7;

	/* get xy coordinate */
	if (finger[0].is_valid) {
		finger[0].x = ((buf[2] & 0x0070) << 4) | buf[3];
		finger[0].y = ((buf[2] & 0x0007) << 8) | buf[4];
		finger[0].t = buf[8];
	}

	if (finger[1].is_valid) {
		finger[1].x = ((buf[5] & 0x0070) << 4) | buf[6];
		finger[1].y = ((buf[5] & 0x0007) << 8) | buf[7];
		finger[1].t = buf[9];
	}
=======
	struct touchscreen_properties prop;
	struct dev_pm_qos_request low_latency_req;
	struct gpio_desc *reset_gpio;
	const struct st_chip_info *chip_info;
	int read_buf_len;
	u8 *read_buf;
};

static int st1232_ts_read_data(struct st1232_ts_data *ts, u8 reg,
			       unsigned int n)
{
	struct i2c_client *client = ts->client;
	struct i2c_msg msg[] = {
		{
			.addr	= client->addr,
			.len	= sizeof(reg),
			.buf	= &reg,
		},
		{
			.addr	= client->addr,
			.flags	= I2C_M_RD | I2C_M_DMA_SAFE,
			.len	= n,
			.buf	= ts->read_buf,
		}
	};
	int ret;

	ret = i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg));
	if (ret != ARRAY_SIZE(msg))
		return ret < 0 ? ret : -EIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static irqreturn_t st1232_ts_irq_handler(int irq, void *dev_id)
{
	struct st1232_ts_data *ts = dev_id;
	struct st1232_ts_finger *finger = ts->finger;
	struct input_dev *input_dev = ts->input_dev;
	int count = 0;
	int i, ret;

	ret = st1232_ts_read_data(ts);
	if (ret < 0)
		goto end;

	/* multi touch protocol */
	for (i = 0; i < MAX_FINGERS; i++) {
		if (!finger[i].is_valid)
			continue;

		input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, finger[i].t);
		input_report_abs(input_dev, ABS_MT_POSITION_X, finger[i].x);
		input_report_abs(input_dev, ABS_MT_POSITION_Y, finger[i].y);
		input_mt_sync(input_dev);
		count++;
	}

	/* SYN_MT_REPORT only if no contact */
	if (!count) {
		input_mt_sync(input_dev);
=======
static int st1232_ts_wait_ready(struct st1232_ts_data *ts)
{
	unsigned int retries;
	int error;

	for (retries = 100; retries; retries--) {
		error = st1232_ts_read_data(ts, REG_STATUS, 1);
		if (!error) {
			switch (ts->read_buf[0]) {
			case STATUS_NORMAL | ERROR_NONE:
			case STATUS_IDLE | ERROR_NONE:
				return 0;
			}
		}

		usleep_range(1000, 2000);
	}

	return -ENXIO;
}

static int st1232_ts_read_resolution(struct st1232_ts_data *ts, u16 *max_x,
				     u16 *max_y)
{
	u8 *buf;
	int error;

	/* select resolution register */
	error = st1232_ts_read_data(ts, REG_XY_RESOLUTION, 3);
	if (error)
		return error;

	buf = ts->read_buf;

	*max_x = (((buf[0] & 0x0070) << 4) | buf[1]) - 1;
	*max_y = (((buf[0] & 0x0007) << 8) | buf[2]) - 1;

	return 0;
}

static int st1232_ts_parse_and_report(struct st1232_ts_data *ts)
{
	struct input_dev *input = ts->input_dev;
	struct input_mt_pos pos[ST_TS_MAX_FINGERS];
	u8 z[ST_TS_MAX_FINGERS];
	int slots[ST_TS_MAX_FINGERS];
	int n_contacts = 0;
	int i;

	for (i = 0; i < ts->chip_info->max_fingers; i++) {
		u8 *buf = &ts->read_buf[i * 4];

		if (buf[0] & BIT(7)) {
			unsigned int x = ((buf[0] & 0x70) << 4) | buf[1];
			unsigned int y = ((buf[0] & 0x07) << 8) | buf[2];

			touchscreen_set_mt_pos(&pos[n_contacts],
					       &ts->prop, x, y);

			/* st1232 includes a z-axis / touch strength */
			if (ts->chip_info->have_z)
				z[n_contacts] = ts->read_buf[i + 6];

			n_contacts++;
		}
	}

	input_mt_assign_slots(input, slots, pos, n_contacts, 0);
	for (i = 0; i < n_contacts; i++) {
		input_mt_slot(input, slots[i]);
		input_mt_report_slot_state(input, MT_TOOL_FINGER, true);
		input_report_abs(input, ABS_MT_POSITION_X, pos[i].x);
		input_report_abs(input, ABS_MT_POSITION_Y, pos[i].y);
		if (ts->chip_info->have_z)
			input_report_abs(input, ABS_MT_TOUCH_MAJOR, z[i]);
	}

	input_mt_sync_frame(input);
	input_sync(input);

	return n_contacts;
}

static irqreturn_t st1232_ts_irq_handler(int irq, void *dev_id)
{
	struct st1232_ts_data *ts = dev_id;
	int count;
	int error;

	error = st1232_ts_read_data(ts, REG_XY_COORDINATES, ts->read_buf_len);
	if (error)
		goto out;

	count = st1232_ts_parse_and_report(ts);
	if (!count) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ts->low_latency_req.dev) {
			dev_pm_qos_remove_request(&ts->low_latency_req);
			ts->low_latency_req.dev = NULL;
		}
	} else if (!ts->low_latency_req.dev) {
		/* First contact, request 100 us latency. */
		dev_pm_qos_add_ancestor_request(&ts->client->dev,
<<<<<<< HEAD
						&ts->low_latency_req, 100);
	}

	/* SYN_REPORT */
	input_sync(input_dev);

end:
	return IRQ_HANDLED;
}

static int __devinit st1232_ts_probe(struct i2c_client *client,
					const struct i2c_device_id *id)
{
	struct st1232_ts_data *ts;
	struct input_dev *input_dev;
	int error;

=======
						&ts->low_latency_req,
						DEV_PM_QOS_RESUME_LATENCY, 100);
	}

out:
	return IRQ_HANDLED;
}

static void st1232_ts_power(struct st1232_ts_data *ts, bool poweron)
{
	if (ts->reset_gpio)
		gpiod_set_value_cansleep(ts->reset_gpio, !poweron);
}

static void st1232_ts_power_off(void *data)
{
	st1232_ts_power(data, false);
}

static const struct st_chip_info st1232_chip_info = {
	.have_z		= true,
	.max_area	= 0xff,
	.max_fingers	= 2,
};

static const struct st_chip_info st1633_chip_info = {
	.have_z		= false,
	.max_area	= 0x00,
	.max_fingers	= 5,
};

static int st1232_ts_probe(struct i2c_client *client)
{
	const struct i2c_device_id *id = i2c_client_get_device_id(client);
	const struct st_chip_info *match;
	struct st1232_ts_data *ts;
	struct input_dev *input_dev;
	u16 max_x, max_y;
	int error;

	match = device_get_match_data(&client->dev);
	if (!match && id)
		match = (const void *)id->driver_data;
	if (!match) {
		dev_err(&client->dev, "unknown device model\n");
		return -ENODEV;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "need I2C_FUNC_I2C\n");
		return -EIO;
	}

	if (!client->irq) {
		dev_err(&client->dev, "no IRQ?\n");
		return -EINVAL;
	}

<<<<<<< HEAD

	ts = kzalloc(sizeof(struct st1232_ts_data), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!ts || !input_dev) {
		error = -ENOMEM;
		goto err_free_mem;
	}
=======
	ts = devm_kzalloc(&client->dev, sizeof(*ts), GFP_KERNEL);
	if (!ts)
		return -ENOMEM;

	ts->chip_info = match;

	/* allocate a buffer according to the number of registers to read */
	ts->read_buf_len = ts->chip_info->max_fingers * 4;
	ts->read_buf = devm_kzalloc(&client->dev, ts->read_buf_len, GFP_KERNEL);
	if (!ts->read_buf)
		return -ENOMEM;

	input_dev = devm_input_allocate_device(&client->dev);
	if (!input_dev)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ts->client = client;
	ts->input_dev = input_dev;

<<<<<<< HEAD
	input_dev->name = "st1232-touchscreen";
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = &client->dev;

	__set_bit(EV_SYN, input_dev->evbit);
	__set_bit(EV_KEY, input_dev->evbit);
	__set_bit(EV_ABS, input_dev->evbit);

	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, MAX_AREA, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X, MIN_X, MAX_X, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, MIN_Y, MAX_Y, 0, 0);

	error = request_threaded_irq(client->irq, NULL, st1232_ts_irq_handler,
				     IRQF_ONESHOT, client->name, ts);
	if (error) {
		dev_err(&client->dev, "Failed to register interrupt\n");
		goto err_free_mem;
=======
	ts->reset_gpio = devm_gpiod_get_optional(&client->dev, NULL,
						 GPIOD_OUT_HIGH);
	if (IS_ERR(ts->reset_gpio)) {
		error = PTR_ERR(ts->reset_gpio);
		dev_err(&client->dev, "Unable to request GPIO pin: %d.\n",
			error);
		return error;
	}

	st1232_ts_power(ts, true);

	error = devm_add_action_or_reset(&client->dev, st1232_ts_power_off, ts);
	if (error) {
		dev_err(&client->dev,
			"Failed to install power off action: %d\n", error);
		return error;
	}

	input_dev->name = "st1232-touchscreen";
	input_dev->id.bustype = BUS_I2C;

	/* Wait until device is ready */
	error = st1232_ts_wait_ready(ts);
	if (error)
		return error;

	/* Read resolution from the chip */
	error = st1232_ts_read_resolution(ts, &max_x, &max_y);
	if (error) {
		dev_err(&client->dev,
			"Failed to read resolution: %d\n", error);
		return error;
	}

	if (ts->chip_info->have_z)
		input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0,
				     ts->chip_info->max_area, 0, 0);

	input_set_abs_params(input_dev, ABS_MT_POSITION_X,
			     0, max_x, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y,
			     0, max_y, 0, 0);

	touchscreen_parse_properties(input_dev, true, &ts->prop);

	error = input_mt_init_slots(input_dev, ts->chip_info->max_fingers,
				    INPUT_MT_DIRECT | INPUT_MT_TRACK |
					INPUT_MT_DROP_UNUSED);
	if (error) {
		dev_err(&client->dev, "failed to initialize MT slots\n");
		return error;
	}

	error = devm_request_threaded_irq(&client->dev, client->irq,
					  NULL, st1232_ts_irq_handler,
					  IRQF_ONESHOT,
					  client->name, ts);
	if (error) {
		dev_err(&client->dev, "Failed to register interrupt\n");
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	error = input_register_device(ts->input_dev);
	if (error) {
		dev_err(&client->dev, "Unable to register %s input device\n",
			input_dev->name);
<<<<<<< HEAD
		goto err_free_irq;
	}

	i2c_set_clientdata(client, ts);
	device_init_wakeup(&client->dev, 1);

	return 0;

err_free_irq:
	free_irq(client->irq, ts);
err_free_mem:
	input_free_device(input_dev);
	kfree(ts);
	return error;
}

static int __devexit st1232_ts_remove(struct i2c_client *client)
{
	struct st1232_ts_data *ts = i2c_get_clientdata(client);

	device_init_wakeup(&client->dev, 0);
	free_irq(client->irq, ts);
	input_unregister_device(ts->input_dev);
	kfree(ts);
=======
		return error;
	}

	i2c_set_clientdata(client, ts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int st1232_ts_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);

	if (device_may_wakeup(&client->dev))
		enable_irq_wake(client->irq);
	else
		disable_irq(client->irq);
=======
static int st1232_ts_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct st1232_ts_data *ts = i2c_get_clientdata(client);

	disable_irq(client->irq);

	if (!device_may_wakeup(&client->dev))
		st1232_ts_power(ts, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int st1232_ts_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
<<<<<<< HEAD

	if (device_may_wakeup(&client->dev))
		disable_irq_wake(client->irq);
	else
		enable_irq(client->irq);
=======
	struct st1232_ts_data *ts = i2c_get_clientdata(client);

	if (!device_may_wakeup(&client->dev))
		st1232_ts_power(ts, true);

	enable_irq(client->irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct dev_pm_ops st1232_ts_pm_ops = {
	.suspend	= st1232_ts_suspend,
	.resume		= st1232_ts_resume,
};
#endif

static const struct i2c_device_id st1232_ts_id[] = {
	{ ST1232_TS_NAME, 0 },
=======
static DEFINE_SIMPLE_DEV_PM_OPS(st1232_ts_pm_ops,
				st1232_ts_suspend, st1232_ts_resume);

static const struct i2c_device_id st1232_ts_id[] = {
	{ ST1232_TS_NAME, (unsigned long)&st1232_chip_info },
	{ ST1633_TS_NAME, (unsigned long)&st1633_chip_info },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ }
};
MODULE_DEVICE_TABLE(i2c, st1232_ts_id);

<<<<<<< HEAD
static struct i2c_driver st1232_ts_driver = {
	.probe		= st1232_ts_probe,
	.remove		= __devexit_p(st1232_ts_remove),
	.id_table	= st1232_ts_id,
	.driver = {
		.name	= ST1232_TS_NAME,
		.owner	= THIS_MODULE,
#ifdef CONFIG_PM
		.pm	= &st1232_ts_pm_ops,
#endif
=======
static const struct of_device_id st1232_ts_dt_ids[] = {
	{ .compatible = "sitronix,st1232", .data = &st1232_chip_info },
	{ .compatible = "sitronix,st1633", .data = &st1633_chip_info },
	{ }
};
MODULE_DEVICE_TABLE(of, st1232_ts_dt_ids);

static struct i2c_driver st1232_ts_driver = {
	.probe		= st1232_ts_probe,
	.id_table	= st1232_ts_id,
	.driver = {
		.name	= ST1232_TS_NAME,
		.of_match_table = st1232_ts_dt_ids,
		.probe_type	= PROBE_PREFER_ASYNCHRONOUS,
		.pm	= pm_sleep_ptr(&st1232_ts_pm_ops),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_i2c_driver(st1232_ts_driver);

MODULE_AUTHOR("Tony SIM <chinyeow.sim.xt@renesas.com>");
<<<<<<< HEAD
MODULE_DESCRIPTION("SITRONIX ST1232 Touchscreen Controller Driver");
MODULE_LICENSE("GPL");
=======
MODULE_AUTHOR("Martin Kepplinger <martin.kepplinger@ginzinger.com>");
MODULE_DESCRIPTION("SITRONIX ST1232 Touchscreen Controller Driver");
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
