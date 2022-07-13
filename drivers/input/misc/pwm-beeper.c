<<<<<<< HEAD
/*
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
 *  PWM beeper driver
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <linux/input.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
 *  PWM beeper driver
 */

#include <linux/input.h>
#include <linux/regulator/consumer.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct pwm_beeper {
	struct input_dev *input;
	struct pwm_device *pwm;
<<<<<<< HEAD
	unsigned long period;
=======
	struct regulator *amplifier;
	struct work_struct work;
	unsigned long period;
	unsigned int bell_frequency;
	bool suspended;
	bool amplifier_on;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define HZ_TO_NANOSECONDS(x) (1000000000UL/(x))

<<<<<<< HEAD
static int pwm_beeper_event(struct input_dev *input,
			    unsigned int type, unsigned int code, int value)
{
	int ret = 0;
	struct pwm_beeper *beeper = input_get_drvdata(input);
	unsigned long period;
=======
static int pwm_beeper_on(struct pwm_beeper *beeper, unsigned long period)
{
	struct pwm_state state;
	int error;

	pwm_get_state(beeper->pwm, &state);

	state.enabled = true;
	state.period = period;
	pwm_set_relative_duty_cycle(&state, 50, 100);

	error = pwm_apply_might_sleep(beeper->pwm, &state);
	if (error)
		return error;

	if (!beeper->amplifier_on) {
		error = regulator_enable(beeper->amplifier);
		if (error) {
			pwm_disable(beeper->pwm);
			return error;
		}

		beeper->amplifier_on = true;
	}

	return 0;
}

static void pwm_beeper_off(struct pwm_beeper *beeper)
{
	if (beeper->amplifier_on) {
		regulator_disable(beeper->amplifier);
		beeper->amplifier_on = false;
	}

	pwm_disable(beeper->pwm);
}

static void pwm_beeper_work(struct work_struct *work)
{
	struct pwm_beeper *beeper = container_of(work, struct pwm_beeper, work);
	unsigned long period = READ_ONCE(beeper->period);

	if (period)
		pwm_beeper_on(beeper, period);
	else
		pwm_beeper_off(beeper);
}

static int pwm_beeper_event(struct input_dev *input,
			    unsigned int type, unsigned int code, int value)
{
	struct pwm_beeper *beeper = input_get_drvdata(input);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (type != EV_SND || value < 0)
		return -EINVAL;

	switch (code) {
	case SND_BELL:
<<<<<<< HEAD
		value = value ? 1000 : 0;
=======
		value = value ? beeper->bell_frequency : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case SND_TONE:
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	if (value == 0) {
		pwm_config(beeper->pwm, 0, 0);
		pwm_disable(beeper->pwm);
	} else {
		period = HZ_TO_NANOSECONDS(value);
		ret = pwm_config(beeper->pwm, period / 2, period);
		if (ret)
			return ret;
		ret = pwm_enable(beeper->pwm);
		if (ret)
			return ret;
		beeper->period = period;
	}
=======
	if (value == 0)
		beeper->period = 0;
	else
		beeper->period = HZ_TO_NANOSECONDS(value);

	if (!beeper->suspended)
		schedule_work(&beeper->work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int __devinit pwm_beeper_probe(struct platform_device *pdev)
{
	unsigned long pwm_id = (unsigned long)pdev->dev.platform_data;
	struct pwm_beeper *beeper;
	int error;

	beeper = kzalloc(sizeof(*beeper), GFP_KERNEL);
	if (!beeper)
		return -ENOMEM;

	beeper->pwm = pwm_request(pwm_id, "pwm beeper");

	if (IS_ERR(beeper->pwm)) {
		error = PTR_ERR(beeper->pwm);
		dev_err(&pdev->dev, "Failed to request pwm device: %d\n", error);
		goto err_free;
	}

	beeper->input = input_allocate_device();
	if (!beeper->input) {
		dev_err(&pdev->dev, "Failed to allocate input device\n");
		error = -ENOMEM;
		goto err_pwm_free;
	}
	beeper->input->dev.parent = &pdev->dev;
=======
static void pwm_beeper_stop(struct pwm_beeper *beeper)
{
	cancel_work_sync(&beeper->work);
	pwm_beeper_off(beeper);
}

static void pwm_beeper_close(struct input_dev *input)
{
	struct pwm_beeper *beeper = input_get_drvdata(input);

	pwm_beeper_stop(beeper);
}

static int pwm_beeper_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pwm_beeper *beeper;
	struct pwm_state state;
	u32 bell_frequency;
	int error;

	beeper = devm_kzalloc(dev, sizeof(*beeper), GFP_KERNEL);
	if (!beeper)
		return -ENOMEM;

	beeper->pwm = devm_pwm_get(dev, NULL);
	if (IS_ERR(beeper->pwm))
		return dev_err_probe(dev, PTR_ERR(beeper->pwm), "Failed to request PWM device\n");

	/* Sync up PWM state and ensure it is off. */
	pwm_init_state(beeper->pwm, &state);
	state.enabled = false;
	error = pwm_apply_might_sleep(beeper->pwm, &state);
	if (error) {
		dev_err(dev, "failed to apply initial PWM state: %d\n",
			error);
		return error;
	}

	beeper->amplifier = devm_regulator_get(dev, "amp");
	if (IS_ERR(beeper->amplifier))
		return dev_err_probe(dev, PTR_ERR(beeper->amplifier),
				     "Failed to get 'amp' regulator\n");

	INIT_WORK(&beeper->work, pwm_beeper_work);

	error = device_property_read_u32(dev, "beeper-hz", &bell_frequency);
	if (error) {
		bell_frequency = 1000;
		dev_dbg(dev,
			"failed to parse 'beeper-hz' property, using default: %uHz\n",
			bell_frequency);
	}

	beeper->bell_frequency = bell_frequency;

	beeper->input = devm_input_allocate_device(dev);
	if (!beeper->input) {
		dev_err(dev, "Failed to allocate input device\n");
		return -ENOMEM;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	beeper->input->name = "pwm-beeper";
	beeper->input->phys = "pwm/input0";
	beeper->input->id.bustype = BUS_HOST;
	beeper->input->id.vendor = 0x001f;
	beeper->input->id.product = 0x0001;
	beeper->input->id.version = 0x0100;

<<<<<<< HEAD
	beeper->input->evbit[0] = BIT(EV_SND);
	beeper->input->sndbit[0] = BIT(SND_TONE) | BIT(SND_BELL);

	beeper->input->event = pwm_beeper_event;
=======
	input_set_capability(beeper->input, EV_SND, SND_TONE);
	input_set_capability(beeper->input, EV_SND, SND_BELL);

	beeper->input->event = pwm_beeper_event;
	beeper->input->close = pwm_beeper_close;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	input_set_drvdata(beeper->input, beeper);

	error = input_register_device(beeper->input);
	if (error) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "Failed to register input device: %d\n", error);
		goto err_input_free;
=======
		dev_err(dev, "Failed to register input device: %d\n", error);
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	platform_set_drvdata(pdev, beeper);

	return 0;
<<<<<<< HEAD

err_input_free:
	input_free_device(beeper->input);
err_pwm_free:
	pwm_free(beeper->pwm);
err_free:
	kfree(beeper);

	return error;
}

static int __devexit pwm_beeper_remove(struct platform_device *pdev)
{
	struct pwm_beeper *beeper = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);
	input_unregister_device(beeper->input);

	pwm_disable(beeper->pwm);
	pwm_free(beeper->pwm);

	kfree(beeper);

	return 0;
}

#ifdef CONFIG_PM
=======
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int pwm_beeper_suspend(struct device *dev)
{
	struct pwm_beeper *beeper = dev_get_drvdata(dev);

<<<<<<< HEAD
	if (beeper->period)
		pwm_disable(beeper->pwm);
=======
	/*
	 * Spinlock is taken here is not to protect write to
	 * beeper->suspended, but to ensure that pwm_beeper_event
	 * does not re-submit work once flag is set.
	 */
	spin_lock_irq(&beeper->input->event_lock);
	beeper->suspended = true;
	spin_unlock_irq(&beeper->input->event_lock);

	pwm_beeper_stop(beeper);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int pwm_beeper_resume(struct device *dev)
{
	struct pwm_beeper *beeper = dev_get_drvdata(dev);

<<<<<<< HEAD
	if (beeper->period) {
		pwm_config(beeper->pwm, beeper->period / 2, beeper->period);
		pwm_enable(beeper->pwm);
	}
=======
	spin_lock_irq(&beeper->input->event_lock);
	beeper->suspended = false;
	spin_unlock_irq(&beeper->input->event_lock);

	/* Let worker figure out if we should resume beeping */
	schedule_work(&beeper->work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static SIMPLE_DEV_PM_OPS(pwm_beeper_pm_ops,
			 pwm_beeper_suspend, pwm_beeper_resume);

#define PWM_BEEPER_PM_OPS (&pwm_beeper_pm_ops)
#else
#define PWM_BEEPER_PM_OPS NULL
=======
static DEFINE_SIMPLE_DEV_PM_OPS(pwm_beeper_pm_ops,
				pwm_beeper_suspend, pwm_beeper_resume);

#ifdef CONFIG_OF
static const struct of_device_id pwm_beeper_match[] = {
	{ .compatible = "pwm-beeper", },
	{ },
};
MODULE_DEVICE_TABLE(of, pwm_beeper_match);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static struct platform_driver pwm_beeper_driver = {
	.probe	= pwm_beeper_probe,
<<<<<<< HEAD
	.remove = __devexit_p(pwm_beeper_remove),
	.driver = {
		.name	= "pwm-beeper",
		.owner	= THIS_MODULE,
		.pm	= PWM_BEEPER_PM_OPS,
=======
	.driver = {
		.name	= "pwm-beeper",
		.pm	= pm_sleep_ptr(&pwm_beeper_pm_ops),
		.of_match_table = of_match_ptr(pwm_beeper_match),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};
module_platform_driver(pwm_beeper_driver);

MODULE_AUTHOR("Lars-Peter Clausen <lars@metafoo.de>");
MODULE_DESCRIPTION("PWM beeper driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:pwm-beeper");
