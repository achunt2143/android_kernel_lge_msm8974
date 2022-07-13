<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Driver for Pixcir I2C touchscreen controllers.
 *
 * Copyright (C) 2010-2011 Pixcir, Inc.
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
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/input/pixcir_ts.h>
=======
 */

#include <asm/unaligned.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/input/touchscreen.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/slab.h>

#define PIXCIR_MAX_SLOTS       5 /* Max fingers supported by driver */

/*
 * Register map
 */
#define PIXCIR_REG_POWER_MODE	51
#define PIXCIR_REG_INT_MODE	52

/*
 * Power modes:
 * active: max scan speed
 * idle: lower scan speed with automatic transition to active on touch
 * halt: datasheet says sleep but this is more like halt as the chip
 *       clocks are cut and it can only be brought out of this mode
 *	 using the RESET pin.
 */
enum pixcir_power_mode {
	PIXCIR_POWER_ACTIVE,
	PIXCIR_POWER_IDLE,
	PIXCIR_POWER_HALT,
};

#define PIXCIR_POWER_MODE_MASK	0x03
#define PIXCIR_POWER_ALLOW_IDLE (1UL << 2)

/*
 * Interrupt modes:
 * periodical: interrupt is asserted periodicaly
 * diff coordinates: interrupt is asserted when coordinates change
 * level on touch: interrupt level asserted during touch
 * pulse on touch: interrupt pulse asserted during touch
 *
 */
enum pixcir_int_mode {
	PIXCIR_INT_PERIODICAL,
	PIXCIR_INT_DIFF_COORD,
	PIXCIR_INT_LEVEL_TOUCH,
	PIXCIR_INT_PULSE_TOUCH,
};

#define PIXCIR_INT_MODE_MASK	0x03
#define PIXCIR_INT_ENABLE	(1UL << 3)
#define PIXCIR_INT_POL_HIGH	(1UL << 2)

/**
 * struct pixcir_i2c_chip_data - chip related data
 * @max_fingers:	Max number of fingers reported simultaneously by h/w
 * @has_hw_ids:		Hardware supports finger tracking IDs
 *
 */
struct pixcir_i2c_chip_data {
	u8 max_fingers;
	bool has_hw_ids;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct pixcir_i2c_ts_data {
	struct i2c_client *client;
	struct input_dev *input;
<<<<<<< HEAD
	const struct pixcir_ts_platform_data *chip;
	bool exiting;
};

static void pixcir_ts_poscheck(struct pixcir_i2c_ts_data *data)
{
	struct pixcir_i2c_ts_data *tsdata = data;
	u8 rdbuf[10], wrbuf[1] = { 0 };
	u8 touch;
	int ret;
=======
	struct gpio_desc *gpio_attb;
	struct gpio_desc *gpio_reset;
	struct gpio_desc *gpio_enable;
	struct gpio_desc *gpio_wake;
	const struct pixcir_i2c_chip_data *chip;
	struct touchscreen_properties prop;
	bool running;
};

struct pixcir_report_data {
	int num_touches;
	struct input_mt_pos pos[PIXCIR_MAX_SLOTS];
	int ids[PIXCIR_MAX_SLOTS];
};

static void pixcir_ts_parse(struct pixcir_i2c_ts_data *tsdata,
			    struct pixcir_report_data *report)
{
	u8 rdbuf[2 + PIXCIR_MAX_SLOTS * 5];
	u8 wrbuf[1] = { 0 };
	u8 *bufptr;
	u8 touch;
	int ret, i;
	int readsize;
	const struct pixcir_i2c_chip_data *chip = tsdata->chip;

	memset(report, 0, sizeof(struct pixcir_report_data));

	i = chip->has_hw_ids ? 1 : 0;
	readsize = 2 + tsdata->chip->max_fingers * (4 + i);
	if (readsize > sizeof(rdbuf))
		readsize = sizeof(rdbuf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = i2c_master_send(tsdata->client, wrbuf, sizeof(wrbuf));
	if (ret != sizeof(wrbuf)) {
		dev_err(&tsdata->client->dev,
			"%s: i2c_master_send failed(), ret=%d\n",
			__func__, ret);
		return;
	}

<<<<<<< HEAD
	ret = i2c_master_recv(tsdata->client, rdbuf, sizeof(rdbuf));
	if (ret != sizeof(rdbuf)) {
=======
	ret = i2c_master_recv(tsdata->client, rdbuf, readsize);
	if (ret != readsize) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&tsdata->client->dev,
			"%s: i2c_master_recv failed(), ret=%d\n",
			__func__, ret);
		return;
	}

<<<<<<< HEAD
	touch = rdbuf[0];
	if (touch) {
		u16 posx1 = (rdbuf[3] << 8) | rdbuf[2];
		u16 posy1 = (rdbuf[5] << 8) | rdbuf[4];
		u16 posx2 = (rdbuf[7] << 8) | rdbuf[6];
		u16 posy2 = (rdbuf[9] << 8) | rdbuf[8];

		input_report_key(tsdata->input, BTN_TOUCH, 1);
		input_report_abs(tsdata->input, ABS_X, posx1);
		input_report_abs(tsdata->input, ABS_Y, posy1);

		input_report_abs(tsdata->input, ABS_MT_POSITION_X, posx1);
		input_report_abs(tsdata->input, ABS_MT_POSITION_Y, posy1);
		input_mt_sync(tsdata->input);

		if (touch == 2) {
			input_report_abs(tsdata->input,
					 ABS_MT_POSITION_X, posx2);
			input_report_abs(tsdata->input,
					 ABS_MT_POSITION_Y, posy2);
			input_mt_sync(tsdata->input);
		}
	} else {
		input_report_key(tsdata->input, BTN_TOUCH, 0);
	}

	input_sync(tsdata->input);
=======
	touch = rdbuf[0] & 0x7;
	if (touch > tsdata->chip->max_fingers)
		touch = tsdata->chip->max_fingers;

	report->num_touches = touch;
	bufptr = &rdbuf[2];

	for (i = 0; i < touch; i++) {
		touchscreen_set_mt_pos(&report->pos[i], &tsdata->prop,
				       get_unaligned_le16(bufptr),
				       get_unaligned_le16(bufptr + 2));
		if (chip->has_hw_ids) {
			report->ids[i] = bufptr[4];
			bufptr = bufptr + 5;
		} else {
			bufptr = bufptr + 4;
		}
	}
}

static void pixcir_ts_report(struct pixcir_i2c_ts_data *ts,
			     struct pixcir_report_data *report)
{
	int slots[PIXCIR_MAX_SLOTS];
	int n, i, slot;
	struct device *dev = &ts->client->dev;
	const struct pixcir_i2c_chip_data *chip = ts->chip;

	n = report->num_touches;
	if (n > PIXCIR_MAX_SLOTS)
		n = PIXCIR_MAX_SLOTS;

	if (!ts->chip->has_hw_ids)
		input_mt_assign_slots(ts->input, slots, report->pos, n, 0);

	for (i = 0; i < n; i++) {
		if (chip->has_hw_ids) {
			slot = input_mt_get_slot_by_key(ts->input,
							report->ids[i]);
			if (slot < 0) {
				dev_dbg(dev, "no free slot for id 0x%x\n",
					report->ids[i]);
				continue;
			}
		} else {
			slot = slots[i];
		}

		input_mt_slot(ts->input, slot);
		input_mt_report_slot_state(ts->input, MT_TOOL_FINGER, true);

		input_report_abs(ts->input, ABS_MT_POSITION_X,
				 report->pos[i].x);
		input_report_abs(ts->input, ABS_MT_POSITION_Y,
				 report->pos[i].y);

		dev_dbg(dev, "%d: slot %d, x %d, y %d\n",
			i, slot, report->pos[i].x, report->pos[i].y);
	}

	input_mt_sync_frame(ts->input);
	input_sync(ts->input);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static irqreturn_t pixcir_ts_isr(int irq, void *dev_id)
{
	struct pixcir_i2c_ts_data *tsdata = dev_id;
<<<<<<< HEAD

	while (!tsdata->exiting) {
		pixcir_ts_poscheck(tsdata);

		if (tsdata->chip->attb_read_val())
			break;
=======
	struct pixcir_report_data report;

	while (tsdata->running) {
		/* parse packet */
		pixcir_ts_parse(tsdata, &report);

		/* report it */
		pixcir_ts_report(tsdata, &report);

		if (gpiod_get_value_cansleep(tsdata->gpio_attb)) {
			if (report.num_touches) {
				/*
				 * Last report with no finger up?
				 * Do it now then.
				 */
				input_mt_sync_frame(tsdata->input);
				input_sync(tsdata->input);
			}
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		msleep(20);
	}

	return IRQ_HANDLED;
}

<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP
static int pixcir_i2c_ts_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);

	if (device_may_wakeup(&client->dev))
		enable_irq_wake(client->irq);

	return 0;
=======
static void pixcir_reset(struct pixcir_i2c_ts_data *tsdata)
{
	if (!IS_ERR_OR_NULL(tsdata->gpio_reset)) {
		gpiod_set_value_cansleep(tsdata->gpio_reset, 1);
		ndelay(100);	/* datasheet section 1.2.3 says 80ns min. */
		gpiod_set_value_cansleep(tsdata->gpio_reset, 0);
		/* wait for controller ready. 100ms guess. */
		msleep(100);
	}
}

static int pixcir_set_power_mode(struct pixcir_i2c_ts_data *ts,
				 enum pixcir_power_mode mode)
{
	struct device *dev = &ts->client->dev;
	int ret;

	if (mode == PIXCIR_POWER_ACTIVE || mode == PIXCIR_POWER_IDLE) {
		if (ts->gpio_wake)
			gpiod_set_value_cansleep(ts->gpio_wake, 1);
	}

	ret = i2c_smbus_read_byte_data(ts->client, PIXCIR_REG_POWER_MODE);
	if (ret < 0) {
		dev_err(dev, "%s: can't read reg %d : %d\n",
			__func__, PIXCIR_REG_POWER_MODE, ret);
		return ret;
	}

	ret &= ~PIXCIR_POWER_MODE_MASK;
	ret |= mode;

	/* Always AUTO_IDLE */
	ret |= PIXCIR_POWER_ALLOW_IDLE;

	ret = i2c_smbus_write_byte_data(ts->client, PIXCIR_REG_POWER_MODE, ret);
	if (ret < 0) {
		dev_err(dev, "%s: can't write reg %d : %d\n",
			__func__, PIXCIR_REG_POWER_MODE, ret);
		return ret;
	}

	if (mode == PIXCIR_POWER_HALT) {
		if (ts->gpio_wake)
			gpiod_set_value_cansleep(ts->gpio_wake, 0);
	}

	return 0;
}

/*
 * Set the interrupt mode for the device i.e. ATTB line behaviour
 *
 * @polarity : 1 for active high, 0 for active low.
 */
static int pixcir_set_int_mode(struct pixcir_i2c_ts_data *ts,
			       enum pixcir_int_mode mode, bool polarity)
{
	struct device *dev = &ts->client->dev;
	int ret;

	ret = i2c_smbus_read_byte_data(ts->client, PIXCIR_REG_INT_MODE);
	if (ret < 0) {
		dev_err(dev, "%s: can't read reg %d : %d\n",
			__func__, PIXCIR_REG_INT_MODE, ret);
		return ret;
	}

	ret &= ~PIXCIR_INT_MODE_MASK;
	ret |= mode;

	if (polarity)
		ret |= PIXCIR_INT_POL_HIGH;
	else
		ret &= ~PIXCIR_INT_POL_HIGH;

	ret = i2c_smbus_write_byte_data(ts->client, PIXCIR_REG_INT_MODE, ret);
	if (ret < 0) {
		dev_err(dev, "%s: can't write reg %d : %d\n",
			__func__, PIXCIR_REG_INT_MODE, ret);
		return ret;
	}

	return 0;
}

/*
 * Enable/disable interrupt generation
 */
static int pixcir_int_enable(struct pixcir_i2c_ts_data *ts, bool enable)
{
	struct device *dev = &ts->client->dev;
	int ret;

	ret = i2c_smbus_read_byte_data(ts->client, PIXCIR_REG_INT_MODE);
	if (ret < 0) {
		dev_err(dev, "%s: can't read reg %d : %d\n",
			__func__, PIXCIR_REG_INT_MODE, ret);
		return ret;
	}

	if (enable)
		ret |= PIXCIR_INT_ENABLE;
	else
		ret &= ~PIXCIR_INT_ENABLE;

	ret = i2c_smbus_write_byte_data(ts->client, PIXCIR_REG_INT_MODE, ret);
	if (ret < 0) {
		dev_err(dev, "%s: can't write reg %d : %d\n",
			__func__, PIXCIR_REG_INT_MODE, ret);
		return ret;
	}

	return 0;
}

static int pixcir_start(struct pixcir_i2c_ts_data *ts)
{
	struct device *dev = &ts->client->dev;
	int error;

	if (ts->gpio_enable) {
		gpiod_set_value_cansleep(ts->gpio_enable, 1);
		msleep(100);
	}

	/* LEVEL_TOUCH interrupt with active low polarity */
	error = pixcir_set_int_mode(ts, PIXCIR_INT_LEVEL_TOUCH, 0);
	if (error) {
		dev_err(dev, "Failed to set interrupt mode: %d\n", error);
		return error;
	}

	ts->running = true;
	mb();	/* Update status before IRQ can fire */

	/* enable interrupt generation */
	error = pixcir_int_enable(ts, true);
	if (error) {
		dev_err(dev, "Failed to enable interrupt generation: %d\n",
			error);
		return error;
	}

	return 0;
}

static int pixcir_stop(struct pixcir_i2c_ts_data *ts)
{
	int error;

	/* Disable interrupt generation */
	error = pixcir_int_enable(ts, false);
	if (error) {
		dev_err(&ts->client->dev,
			"Failed to disable interrupt generation: %d\n",
			error);
		return error;
	}

	/* Exit ISR if running, no more report parsing */
	ts->running = false;
	mb();	/* update status before we synchronize irq */

	/* Wait till running ISR is complete */
	synchronize_irq(ts->client->irq);

	if (ts->gpio_enable)
		gpiod_set_value_cansleep(ts->gpio_enable, 0);

	return 0;
}

static int pixcir_input_open(struct input_dev *dev)
{
	struct pixcir_i2c_ts_data *ts = input_get_drvdata(dev);

	return pixcir_start(ts);
}

static void pixcir_input_close(struct input_dev *dev)
{
	struct pixcir_i2c_ts_data *ts = input_get_drvdata(dev);

	pixcir_stop(ts);
}

static int pixcir_i2c_ts_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct pixcir_i2c_ts_data *ts = i2c_get_clientdata(client);
	struct input_dev *input = ts->input;
	int ret = 0;

	mutex_lock(&input->mutex);

	if (device_may_wakeup(&client->dev)) {
		if (!input_device_enabled(input)) {
			ret = pixcir_start(ts);
			if (ret) {
				dev_err(dev, "Failed to start\n");
				goto unlock;
			}
		}
	} else if (input_device_enabled(input)) {
		ret = pixcir_stop(ts);
	}

unlock:
	mutex_unlock(&input->mutex);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pixcir_i2c_ts_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
<<<<<<< HEAD

	if (device_may_wakeup(&client->dev))
		disable_irq_wake(client->irq);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(pixcir_dev_pm_ops,
			 pixcir_i2c_ts_suspend, pixcir_i2c_ts_resume);

static int __devinit pixcir_i2c_ts_probe(struct i2c_client *client,
					 const struct i2c_device_id *id)
{
	const struct pixcir_ts_platform_data *pdata = client->dev.platform_data;
=======
	struct pixcir_i2c_ts_data *ts = i2c_get_clientdata(client);
	struct input_dev *input = ts->input;
	int ret = 0;

	mutex_lock(&input->mutex);

	if (device_may_wakeup(&client->dev)) {
		if (!input_device_enabled(input)) {
			ret = pixcir_stop(ts);
			if (ret) {
				dev_err(dev, "Failed to stop\n");
				goto unlock;
			}
		}
	} else if (input_device_enabled(input)) {
		ret = pixcir_start(ts);
	}

unlock:
	mutex_unlock(&input->mutex);

	return ret;
}

static DEFINE_SIMPLE_DEV_PM_OPS(pixcir_dev_pm_ops,
				pixcir_i2c_ts_suspend, pixcir_i2c_ts_resume);

static int pixcir_i2c_ts_probe(struct i2c_client *client)
{
	const struct i2c_device_id *id = i2c_client_get_device_id(client);
	struct device *dev = &client->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct pixcir_i2c_ts_data *tsdata;
	struct input_dev *input;
	int error;

<<<<<<< HEAD
	if (!pdata) {
		dev_err(&client->dev, "platform data not defined\n");
		return -EINVAL;
	}

	tsdata = kzalloc(sizeof(*tsdata), GFP_KERNEL);
	input = input_allocate_device();
	if (!tsdata || !input) {
		dev_err(&client->dev, "Failed to allocate driver data!\n");
		error = -ENOMEM;
		goto err_free_mem;
=======
	tsdata = devm_kzalloc(dev, sizeof(*tsdata), GFP_KERNEL);
	if (!tsdata)
		return -ENOMEM;

	tsdata->chip = device_get_match_data(dev);
	if (!tsdata->chip && id)
		tsdata->chip = (const void *)id->driver_data;
	if (!tsdata->chip) {
		dev_err(dev, "can't locate chip data\n");
		return -EINVAL;
	}

	input = devm_input_allocate_device(dev);
	if (!input) {
		dev_err(dev, "Failed to allocate input device\n");
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	tsdata->client = client;
	tsdata->input = input;
<<<<<<< HEAD
	tsdata->chip = pdata;

	input->name = client->name;
	input->id.bustype = BUS_I2C;
	input->dev.parent = &client->dev;

	__set_bit(EV_KEY, input->evbit);
	__set_bit(EV_ABS, input->evbit);
	__set_bit(BTN_TOUCH, input->keybit);
	input_set_abs_params(input, ABS_X, 0, pdata->x_max, 0, 0);
	input_set_abs_params(input, ABS_Y, 0, pdata->y_max, 0, 0);
	input_set_abs_params(input, ABS_MT_POSITION_X, 0, pdata->x_max, 0, 0);
	input_set_abs_params(input, ABS_MT_POSITION_Y, 0, pdata->y_max, 0, 0);

	input_set_drvdata(input, tsdata);

	error = request_threaded_irq(client->irq, NULL, pixcir_ts_isr,
				     IRQF_TRIGGER_FALLING,
				     client->name, tsdata);
	if (error) {
		dev_err(&client->dev, "Unable to request touchscreen IRQ.\n");
		goto err_free_mem;
	}

	error = input_register_device(input);
	if (error)
		goto err_free_irq;

	i2c_set_clientdata(client, tsdata);
	device_init_wakeup(&client->dev, 1);

	return 0;

err_free_irq:
	free_irq(client->irq, tsdata);
err_free_mem:
	input_free_device(input);
	kfree(tsdata);
	return error;
}

static int __devexit pixcir_i2c_ts_remove(struct i2c_client *client)
{
	struct pixcir_i2c_ts_data *tsdata = i2c_get_clientdata(client);

	device_init_wakeup(&client->dev, 0);

	tsdata->exiting = true;
	mb();
	free_irq(client->irq, tsdata);

	input_unregister_device(tsdata->input);
	kfree(tsdata);
=======

	input->name = client->name;
	input->id.bustype = BUS_I2C;
	input->open = pixcir_input_open;
	input->close = pixcir_input_close;

	input_set_capability(input, EV_ABS, ABS_MT_POSITION_X);
	input_set_capability(input, EV_ABS, ABS_MT_POSITION_Y);
	touchscreen_parse_properties(input, true, &tsdata->prop);
	if (!input_abs_get_max(input, ABS_MT_POSITION_X) ||
	    !input_abs_get_max(input, ABS_MT_POSITION_Y)) {
		dev_err(dev, "Touchscreen size is not specified\n");
		return -EINVAL;
	}

	error = input_mt_init_slots(input, tsdata->chip->max_fingers,
				    INPUT_MT_DIRECT | INPUT_MT_DROP_UNUSED);
	if (error) {
		dev_err(dev, "Error initializing Multi-Touch slots\n");
		return error;
	}

	input_set_drvdata(input, tsdata);

	tsdata->gpio_attb = devm_gpiod_get(dev, "attb", GPIOD_IN);
	if (IS_ERR(tsdata->gpio_attb))
		return dev_err_probe(dev, PTR_ERR(tsdata->gpio_attb),
				     "Failed to request ATTB gpio\n");

	tsdata->gpio_reset = devm_gpiod_get_optional(dev, "reset",
						     GPIOD_OUT_LOW);
	if (IS_ERR(tsdata->gpio_reset))
		return dev_err_probe(dev, PTR_ERR(tsdata->gpio_reset),
				     "Failed to request RESET gpio\n");

	tsdata->gpio_wake = devm_gpiod_get_optional(dev, "wake",
						    GPIOD_OUT_HIGH);
	if (IS_ERR(tsdata->gpio_wake))
		return dev_err_probe(dev, PTR_ERR(tsdata->gpio_wake),
				     "Failed to get wake gpio\n");

	tsdata->gpio_enable = devm_gpiod_get_optional(dev, "enable",
						      GPIOD_OUT_HIGH);
	if (IS_ERR(tsdata->gpio_enable))
		return dev_err_probe(dev, PTR_ERR(tsdata->gpio_enable),
				     "Failed to get enable gpio\n");

	if (tsdata->gpio_enable)
		msleep(100);

	error = devm_request_threaded_irq(dev, client->irq, NULL, pixcir_ts_isr,
					  IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
					  client->name, tsdata);
	if (error) {
		dev_err(dev, "failed to request irq %d\n", client->irq);
		return error;
	}

	pixcir_reset(tsdata);

	/* Always be in IDLE mode to save power, device supports auto wake */
	error = pixcir_set_power_mode(tsdata, PIXCIR_POWER_IDLE);
	if (error) {
		dev_err(dev, "Failed to set IDLE mode\n");
		return error;
	}

	/* Stop device till opened */
	error = pixcir_stop(tsdata);
	if (error)
		return error;

	error = input_register_device(input);
	if (error)
		return error;

	i2c_set_clientdata(client, tsdata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct i2c_device_id pixcir_i2c_ts_id[] = {
	{ "pixcir_ts", 0 },
=======
static const struct pixcir_i2c_chip_data pixcir_ts_data = {
	.max_fingers = 2,
	/* no hw id support */
};

static const struct pixcir_i2c_chip_data pixcir_tangoc_data = {
	.max_fingers = 5,
	.has_hw_ids = true,
};

static const struct i2c_device_id pixcir_i2c_ts_id[] = {
	{ "pixcir_ts", (unsigned long) &pixcir_ts_data },
	{ "pixcir_tangoc", (unsigned long) &pixcir_tangoc_data },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ }
};
MODULE_DEVICE_TABLE(i2c, pixcir_i2c_ts_id);

<<<<<<< HEAD
static struct i2c_driver pixcir_i2c_ts_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "pixcir_ts",
		.pm	= &pixcir_dev_pm_ops,
	},
	.probe		= pixcir_i2c_ts_probe,
	.remove		= __devexit_p(pixcir_i2c_ts_remove),
=======
#ifdef CONFIG_OF
static const struct of_device_id pixcir_of_match[] = {
	{ .compatible = "pixcir,pixcir_ts", .data = &pixcir_ts_data },
	{ .compatible = "pixcir,pixcir_tangoc", .data = &pixcir_tangoc_data },
	{ }
};
MODULE_DEVICE_TABLE(of, pixcir_of_match);
#endif

static struct i2c_driver pixcir_i2c_ts_driver = {
	.driver = {
		.name	= "pixcir_ts",
		.pm	= pm_sleep_ptr(&pixcir_dev_pm_ops),
		.of_match_table = of_match_ptr(pixcir_of_match),
	},
	.probe		= pixcir_i2c_ts_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table	= pixcir_i2c_ts_id,
};

module_i2c_driver(pixcir_i2c_ts_driver);

MODULE_AUTHOR("Jianchun Bian <jcbian@pixcir.com.cn>, Dequan Meng <dqmeng@pixcir.com.cn>");
MODULE_DESCRIPTION("Pixcir I2C Touchscreen Driver");
MODULE_LICENSE("GPL");
