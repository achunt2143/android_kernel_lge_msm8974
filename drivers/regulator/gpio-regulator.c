<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * gpio-regulator.c
 *
 * Copyright 2011 Heiko Stuebner <heiko@sntech.de>
 *
 * based on fixed.c
 *
 * Copyright 2008 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * Copyright (c) 2009 Nokia Corporation
 * Roger Quadros <ext-roger.quadros@nokia.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This is useful for systems with mixed controllable and
 * non-controllable regulators, as well as for allowing testing on
 * systems with no controllable regulators.
 */

#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
<<<<<<< HEAD
#include <linux/regulator/gpio-regulator.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>

struct gpio_regulator_data {
	struct regulator_desc desc;
	struct regulator_dev *dev;

	int enable_gpio;
	bool enable_high;
	bool is_enabled;
	unsigned startup_delay;

	struct gpio *gpios;
=======
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/gpio-regulator.h>
#include <linux/gpio/consumer.h>
#include <linux/slab.h>
#include <linux/of.h>

struct gpio_regulator_data {
	struct regulator_desc desc;

	struct gpio_desc **gpiods;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int nr_gpios;

	struct gpio_regulator_state *states;
	int nr_states;

	int state;
};

<<<<<<< HEAD
static int gpio_regulator_is_enabled(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	return data->is_enabled;
}

static int gpio_regulator_enable(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	if (gpio_is_valid(data->enable_gpio)) {
		gpio_set_value_cansleep(data->enable_gpio, data->enable_high);
		data->is_enabled = true;
	}

	return 0;
}

static int gpio_regulator_disable(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	if (gpio_is_valid(data->enable_gpio)) {
		gpio_set_value_cansleep(data->enable_gpio, !data->enable_high);
		data->is_enabled = false;
	}

	return 0;
}

static int gpio_regulator_enable_time(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	return data->startup_delay;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int gpio_regulator_get_value(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr;

	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].gpios == data->state)
			return data->states[ptr].value;

	return -EINVAL;
}

<<<<<<< HEAD
static int gpio_regulator_set_value(struct regulator_dev *dev,
					int min, int max)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr, target, state;

	target = -1;
	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].value >= min &&
		    data->states[ptr].value <= max)
			target = data->states[ptr].gpios;

	if (target < 0)
		return -EINVAL;

	for (ptr = 0; ptr < data->nr_gpios; ptr++) {
		state = (target & (1 << ptr)) >> ptr;
		gpio_set_value(data->gpios[ptr].gpio, state);
	}
	data->state = target;

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int gpio_regulator_set_voltage(struct regulator_dev *dev,
					int min_uV, int max_uV,
					unsigned *selector)
{
<<<<<<< HEAD
	return gpio_regulator_set_value(dev, min_uV, max_uV);
=======
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr, target = 0, state, best_val = INT_MAX;

	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].value < best_val &&
		    data->states[ptr].value >= min_uV &&
		    data->states[ptr].value <= max_uV) {
			target = data->states[ptr].gpios;
			best_val = data->states[ptr].value;
			if (selector)
				*selector = ptr;
		}

	if (best_val == INT_MAX)
		return -EINVAL;

	for (ptr = 0; ptr < data->nr_gpios; ptr++) {
		state = (target & (1 << ptr)) >> ptr;
		gpiod_set_value_cansleep(data->gpiods[ptr], state);
	}
	data->state = target;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int gpio_regulator_list_voltage(struct regulator_dev *dev,
				      unsigned selector)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	if (selector >= data->nr_states)
		return -EINVAL;

	return data->states[selector].value;
}

static int gpio_regulator_set_current_limit(struct regulator_dev *dev,
					int min_uA, int max_uA)
{
<<<<<<< HEAD
	return gpio_regulator_set_value(dev, min_uA, max_uA);
}

static struct regulator_ops gpio_regulator_voltage_ops = {
	.is_enabled = gpio_regulator_is_enabled,
	.enable = gpio_regulator_enable,
	.disable = gpio_regulator_disable,
	.enable_time = gpio_regulator_enable_time,
=======
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr, target = 0, state, best_val = 0;

	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].value > best_val &&
		    data->states[ptr].value >= min_uA &&
		    data->states[ptr].value <= max_uA) {
			target = data->states[ptr].gpios;
			best_val = data->states[ptr].value;
		}

	if (best_val == 0)
		return -EINVAL;

	for (ptr = 0; ptr < data->nr_gpios; ptr++) {
		state = (target & (1 << ptr)) >> ptr;
		gpiod_set_value_cansleep(data->gpiods[ptr], state);
	}
	data->state = target;

	return 0;
}

static const struct regulator_ops gpio_regulator_voltage_ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get_voltage = gpio_regulator_get_value,
	.set_voltage = gpio_regulator_set_voltage,
	.list_voltage = gpio_regulator_list_voltage,
};

<<<<<<< HEAD
static struct regulator_ops gpio_regulator_current_ops = {
	.is_enabled = gpio_regulator_is_enabled,
	.enable = gpio_regulator_enable,
	.disable = gpio_regulator_disable,
	.enable_time = gpio_regulator_enable_time,
=======
static struct gpio_regulator_config *
of_get_gpio_regulator_config(struct device *dev, struct device_node *np,
			     const struct regulator_desc *desc)
{
	struct gpio_regulator_config *config;
	const char *regtype;
	int proplen, i;
	int ngpios;
	int ret;

	config = devm_kzalloc(dev,
			sizeof(struct gpio_regulator_config),
			GFP_KERNEL);
	if (!config)
		return ERR_PTR(-ENOMEM);

	config->init_data = of_get_regulator_init_data(dev, np, desc);
	if (!config->init_data)
		return ERR_PTR(-EINVAL);

	config->supply_name = config->init_data->constraints.name;

	if (config->init_data->constraints.boot_on)
		config->enabled_at_boot = true;

	/*
	 * Do not use: undocumented device tree property.
	 * This is kept around solely for device tree ABI stability.
	 */
	if (of_property_read_bool(np, "enable-at-boot"))
		config->enabled_at_boot = true;

	of_property_read_u32(np, "startup-delay-us", &config->startup_delay);

	/* Fetch GPIO init levels */
	ngpios = gpiod_count(dev, NULL);
	if (ngpios > 0) {
		config->gflags = devm_kzalloc(dev,
					      sizeof(enum gpiod_flags)
					      * ngpios,
					      GFP_KERNEL);
		if (!config->gflags)
			return ERR_PTR(-ENOMEM);

		for (i = 0; i < ngpios; i++) {
			u32 val;

			ret = of_property_read_u32_index(np, "gpios-states", i,
							 &val);

			/* Default to high per specification */
			if (ret)
				config->gflags[i] = GPIOD_OUT_HIGH;
			else
				config->gflags[i] =
					val ? GPIOD_OUT_HIGH : GPIOD_OUT_LOW;
		}
	}
	config->ngpios = ngpios;

	/* Fetch states. */
	proplen = of_property_count_u32_elems(np, "states");
	if (proplen < 0) {
		dev_err(dev, "No 'states' property found\n");
		return ERR_PTR(-EINVAL);
	}

	config->states = devm_kcalloc(dev,
				proplen / 2,
				sizeof(struct gpio_regulator_state),
				GFP_KERNEL);
	if (!config->states)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < proplen / 2; i++) {
		of_property_read_u32_index(np, "states", i * 2,
					   &config->states[i].value);
		of_property_read_u32_index(np, "states", i * 2 + 1,
					   &config->states[i].gpios);
	}
	config->nr_states = i;

	config->type = REGULATOR_VOLTAGE;
	ret = of_property_read_string(np, "regulator-type", &regtype);
	if (ret >= 0) {
		if (!strncmp("voltage", regtype, 7))
			config->type = REGULATOR_VOLTAGE;
		else if (!strncmp("current", regtype, 7))
			config->type = REGULATOR_CURRENT;
		else
			dev_warn(dev, "Unknown regulator-type '%s'\n",
				 regtype);
	}

	if (of_property_present(np, "vin-supply"))
		config->input_supply = "vin";

	return config;
}

static const struct regulator_ops gpio_regulator_current_ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get_current_limit = gpio_regulator_get_value,
	.set_current_limit = gpio_regulator_set_current_limit,
};

<<<<<<< HEAD
static int __devinit gpio_regulator_probe(struct platform_device *pdev)
{
	struct gpio_regulator_config *config = pdev->dev.platform_data;
	struct gpio_regulator_data *drvdata;
	int ptr, ret, state;

	drvdata = kzalloc(sizeof(struct gpio_regulator_data), GFP_KERNEL);
	if (drvdata == NULL) {
		dev_err(&pdev->dev, "Failed to allocate device data\n");
		return -ENOMEM;
	}

	drvdata->desc.name = kstrdup(config->supply_name, GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		dev_err(&pdev->dev, "Failed to allocate supply name\n");
		ret = -ENOMEM;
		goto err;
	}

	drvdata->gpios = kmemdup(config->gpios,
				 config->nr_gpios * sizeof(struct gpio),
				 GFP_KERNEL);
	if (drvdata->gpios == NULL) {
		dev_err(&pdev->dev, "Failed to allocate gpio data\n");
		ret = -ENOMEM;
		goto err_name;
	}

	drvdata->states = kmemdup(config->states,
				  config->nr_states *
					 sizeof(struct gpio_regulator_state),
				  GFP_KERNEL);
	if (drvdata->states == NULL) {
		dev_err(&pdev->dev, "Failed to allocate state data\n");
		ret = -ENOMEM;
		goto err_memgpio;
=======
static int gpio_regulator_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct gpio_regulator_config *config = dev_get_platdata(dev);
	struct device_node *np = dev->of_node;
	struct gpio_regulator_data *drvdata;
	struct regulator_config cfg = { };
	struct regulator_dev *rdev;
	enum gpiod_flags gflags;
	int ptr, ret, state, i;

	drvdata = devm_kzalloc(dev, sizeof(struct gpio_regulator_data),
			       GFP_KERNEL);
	if (drvdata == NULL)
		return -ENOMEM;

	if (np) {
		config = of_get_gpio_regulator_config(dev, np,
						      &drvdata->desc);
		if (IS_ERR(config))
			return PTR_ERR(config);
	}

	drvdata->desc.name = devm_kstrdup(dev, config->supply_name, GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		dev_err(dev, "Failed to allocate supply name\n");
		return -ENOMEM;
	}

	drvdata->gpiods = devm_kzalloc(dev, sizeof(struct gpio_desc *),
				       GFP_KERNEL);

	if (config->input_supply) {
		drvdata->desc.supply_name = devm_kstrdup(&pdev->dev,
							 config->input_supply,
							 GFP_KERNEL);
		if (!drvdata->desc.supply_name) {
			dev_err(&pdev->dev,
				"Failed to allocate input supply\n");
			return -ENOMEM;
		}
	}

	if (!drvdata->gpiods)
		return -ENOMEM;
	for (i = 0; i < config->ngpios; i++) {
		drvdata->gpiods[i] = devm_gpiod_get_index(dev,
							  NULL,
							  i,
							  config->gflags[i]);
		if (IS_ERR(drvdata->gpiods[i]))
			return PTR_ERR(drvdata->gpiods[i]);
		/* This is good to know */
		gpiod_set_consumer_name(drvdata->gpiods[i], drvdata->desc.name);
	}
	drvdata->nr_gpios = config->ngpios;

	drvdata->states = devm_kmemdup(dev,
				       config->states,
				       config->nr_states *
				       sizeof(struct gpio_regulator_state),
				       GFP_KERNEL);
	if (drvdata->states == NULL) {
		dev_err(dev, "Failed to allocate state data\n");
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	drvdata->nr_states = config->nr_states;

	drvdata->desc.owner = THIS_MODULE;
<<<<<<< HEAD
=======
	drvdata->desc.enable_time = config->startup_delay;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* handle regulator type*/
	switch (config->type) {
	case REGULATOR_VOLTAGE:
		drvdata->desc.type = REGULATOR_VOLTAGE;
		drvdata->desc.ops = &gpio_regulator_voltage_ops;
		drvdata->desc.n_voltages = config->nr_states;
		break;
	case REGULATOR_CURRENT:
		drvdata->desc.type = REGULATOR_CURRENT;
		drvdata->desc.ops = &gpio_regulator_current_ops;
		break;
	default:
<<<<<<< HEAD
		dev_err(&pdev->dev, "No regulator type set\n");
		ret = -EINVAL;
		goto err_memgpio;
		break;
	}

	drvdata->enable_gpio = config->enable_gpio;
	drvdata->startup_delay = config->startup_delay;

	if (gpio_is_valid(config->enable_gpio)) {
		drvdata->enable_high = config->enable_high;

		ret = gpio_request(config->enable_gpio, config->supply_name);
		if (ret) {
			dev_err(&pdev->dev,
			   "Could not obtain regulator enable GPIO %d: %d\n",
						config->enable_gpio, ret);
			goto err_memstate;
		}

		/* set output direction without changing state
		 * to prevent glitch
		 */
		if (config->enabled_at_boot) {
			drvdata->is_enabled = true;
			ret = gpio_direction_output(config->enable_gpio,
						    config->enable_high);
		} else {
			drvdata->is_enabled = false;
			ret = gpio_direction_output(config->enable_gpio,
						    !config->enable_high);
		}

		if (ret) {
			dev_err(&pdev->dev,
			   "Could not configure regulator enable GPIO %d direction: %d\n",
						config->enable_gpio, ret);
			goto err_enablegpio;
		}
	} else {
		/* Regulator without GPIO control is considered
		 * always enabled
		 */
		drvdata->is_enabled = true;
	}

	drvdata->nr_gpios = config->nr_gpios;
	ret = gpio_request_array(drvdata->gpios, drvdata->nr_gpios);
	if (ret) {
		dev_err(&pdev->dev,
		   "Could not obtain regulator setting GPIOs: %d\n", ret);
		goto err_enablegpio;
=======
		dev_err(dev, "No regulator type set\n");
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* build initial state from gpio init data. */
	state = 0;
	for (ptr = 0; ptr < drvdata->nr_gpios; ptr++) {
<<<<<<< HEAD
		if (config->gpios[ptr].flags & GPIOF_OUT_INIT_HIGH)
=======
		if (config->gflags[ptr] == GPIOD_OUT_HIGH)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			state |= (1 << ptr);
	}
	drvdata->state = state;

<<<<<<< HEAD
	drvdata->dev = regulator_register(&drvdata->desc, &pdev->dev,
					  config->init_data, drvdata, NULL);
	if (IS_ERR(drvdata->dev)) {
		ret = PTR_ERR(drvdata->dev);
		dev_err(&pdev->dev, "Failed to register regulator: %d\n", ret);
		goto err_stategpio;
=======
	cfg.dev = dev;
	cfg.init_data = config->init_data;
	cfg.driver_data = drvdata;
	cfg.of_node = np;

	/*
	 * The signal will be inverted by the GPIO core if flagged so in the
	 * descriptor.
	 */
	if (config->enabled_at_boot)
		gflags = GPIOD_OUT_HIGH | GPIOD_FLAGS_BIT_NONEXCLUSIVE;
	else
		gflags = GPIOD_OUT_LOW | GPIOD_FLAGS_BIT_NONEXCLUSIVE;

	cfg.ena_gpiod = gpiod_get_optional(dev, "enable", gflags);
	if (IS_ERR(cfg.ena_gpiod))
		return PTR_ERR(cfg.ena_gpiod);

	rdev = devm_regulator_register(dev, &drvdata->desc, &cfg);
	if (IS_ERR(rdev)) {
		ret = PTR_ERR(rdev);
		dev_err(dev, "Failed to register regulator: %d\n", ret);
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	platform_set_drvdata(pdev, drvdata);

	return 0;
<<<<<<< HEAD

err_stategpio:
	gpio_free_array(drvdata->gpios, drvdata->nr_gpios);
err_enablegpio:
	if (gpio_is_valid(config->enable_gpio))
		gpio_free(config->enable_gpio);
err_memstate:
	kfree(drvdata->states);
err_memgpio:
	kfree(drvdata->gpios);
err_name:
	kfree(drvdata->desc.name);
err:
	kfree(drvdata);
	return ret;
}

static int __devexit gpio_regulator_remove(struct platform_device *pdev)
{
	struct gpio_regulator_data *drvdata = platform_get_drvdata(pdev);

	regulator_unregister(drvdata->dev);

	gpio_free_array(drvdata->gpios, drvdata->nr_gpios);

	kfree(drvdata->states);
	kfree(drvdata->gpios);

	if (gpio_is_valid(drvdata->enable_gpio))
		gpio_free(drvdata->enable_gpio);

	kfree(drvdata->desc.name);
	kfree(drvdata);

	return 0;
}

static struct platform_driver gpio_regulator_driver = {
	.probe		= gpio_regulator_probe,
	.remove		= __devexit_p(gpio_regulator_remove),
	.driver		= {
		.name		= "gpio-regulator",
		.owner		= THIS_MODULE,
=======
}

#if defined(CONFIG_OF)
static const struct of_device_id regulator_gpio_of_match[] = {
	{ .compatible = "regulator-gpio", },
	{},
};
MODULE_DEVICE_TABLE(of, regulator_gpio_of_match);
#endif

static struct platform_driver gpio_regulator_driver = {
	.probe		= gpio_regulator_probe,
	.driver		= {
		.name		= "gpio-regulator",
		.probe_type	= PROBE_PREFER_ASYNCHRONOUS,
		.of_match_table = of_match_ptr(regulator_gpio_of_match),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static int __init gpio_regulator_init(void)
{
	return platform_driver_register(&gpio_regulator_driver);
}
subsys_initcall(gpio_regulator_init);

static void __exit gpio_regulator_exit(void)
{
	platform_driver_unregister(&gpio_regulator_driver);
}
module_exit(gpio_regulator_exit);

MODULE_AUTHOR("Heiko Stuebner <heiko@sntech.de>");
MODULE_DESCRIPTION("gpio voltage regulator");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:gpio-regulator");
