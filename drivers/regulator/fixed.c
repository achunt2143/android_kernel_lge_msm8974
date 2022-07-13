<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * fixed.c
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
<<<<<<< HEAD
#include <linux/regulator/driver.h>
#include <linux/regulator/fixed.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/machine.h>
=======
#include <linux/pm_domain.h>
#include <linux/pm_opp.h>
#include <linux/reboot.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/fixed.h>
#include <linux/gpio/consumer.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/machine.h>
#include <linux/clk.h>

/* Default time in millisecond to wait for emergency shutdown */
#define FV_DEF_EMERG_SHUTDWN_TMO	10
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct fixed_voltage_data {
	struct regulator_desc desc;
	struct regulator_dev *dev;
<<<<<<< HEAD
	int microvolts;
	int gpio;
	unsigned startup_delay;
	bool enable_high;
	bool is_enabled;
};

=======

	struct clk *enable_clock;
	unsigned int enable_counter;
	int performance_state;
};

struct fixed_dev_type {
	bool has_enable_clock;
	bool has_performance_state;
};

static int reg_clock_enable(struct regulator_dev *rdev)
{
	struct fixed_voltage_data *priv = rdev_get_drvdata(rdev);
	int ret = 0;

	ret = clk_prepare_enable(priv->enable_clock);
	if (ret)
		return ret;

	priv->enable_counter++;

	return ret;
}

static int reg_clock_disable(struct regulator_dev *rdev)
{
	struct fixed_voltage_data *priv = rdev_get_drvdata(rdev);

	clk_disable_unprepare(priv->enable_clock);
	priv->enable_counter--;

	return 0;
}

static int reg_domain_enable(struct regulator_dev *rdev)
{
	struct fixed_voltage_data *priv = rdev_get_drvdata(rdev);
	struct device *dev = rdev->dev.parent;
	int ret;

	ret = dev_pm_genpd_set_performance_state(dev, priv->performance_state);
	if (ret)
		return ret;

	priv->enable_counter++;

	return ret;
}

static int reg_domain_disable(struct regulator_dev *rdev)
{
	struct fixed_voltage_data *priv = rdev_get_drvdata(rdev);
	struct device *dev = rdev->dev.parent;
	int ret;

	ret = dev_pm_genpd_set_performance_state(dev, 0);
	if (ret)
		return ret;

	priv->enable_counter--;

	return 0;
}

static int reg_is_enabled(struct regulator_dev *rdev)
{
	struct fixed_voltage_data *priv = rdev_get_drvdata(rdev);

	return priv->enable_counter > 0;
}

static irqreturn_t reg_fixed_under_voltage_irq_handler(int irq, void *data)
{
	struct fixed_voltage_data *priv = data;
	struct regulator_dev *rdev = priv->dev;

	regulator_notifier_call_chain(rdev, REGULATOR_EVENT_UNDER_VOLTAGE,
				      NULL);

	return IRQ_HANDLED;
}

/**
 * reg_fixed_get_irqs - Get and register the optional IRQ for fixed voltage
 *                      regulator.
 * @dev: Pointer to the device structure.
 * @priv: Pointer to fixed_voltage_data structure containing private data.
 *
 * This function tries to get the IRQ from the device firmware node.
 * If it's an optional IRQ and not found, it returns 0.
 * Otherwise, it attempts to request the threaded IRQ.
 *
 * Return: 0 on success, or error code on failure.
 */
static int reg_fixed_get_irqs(struct device *dev,
			      struct fixed_voltage_data *priv)
{
	int ret;

	ret = fwnode_irq_get(dev_fwnode(dev), 0);
	/* This is optional IRQ. If not found we will get -EINVAL */
	if (ret == -EINVAL)
		return 0;
	if (ret < 0)
		return dev_err_probe(dev, ret, "Failed to get IRQ\n");

	ret = devm_request_threaded_irq(dev, ret, NULL,
					reg_fixed_under_voltage_irq_handler,
					IRQF_ONESHOT, "under-voltage", priv);
	if (ret)
		return dev_err_probe(dev, ret, "Failed to request IRQ\n");

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * of_get_fixed_voltage_config - extract fixed_voltage_config structure info
 * @dev: device requesting for fixed_voltage_config
<<<<<<< HEAD
=======
 * @desc: regulator description
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Populates fixed_voltage_config structure by extracting data from device
 * tree node, returns a pointer to the populated structure of NULL if memory
 * alloc fails.
 */
static struct fixed_voltage_config *
<<<<<<< HEAD
of_get_fixed_voltage_config(struct device *dev)
{
	struct fixed_voltage_config *config;
	struct device_node *np = dev->of_node;
	const __be32 *delay;
=======
of_get_fixed_voltage_config(struct device *dev,
			    const struct regulator_desc *desc)
{
	struct fixed_voltage_config *config;
	struct device_node *np = dev->of_node;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct regulator_init_data *init_data;

	config = devm_kzalloc(dev, sizeof(struct fixed_voltage_config),
								 GFP_KERNEL);
	if (!config)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	config->init_data = of_get_regulator_init_data(dev, dev->of_node);
=======
	config->init_data = of_get_regulator_init_data(dev, dev->of_node, desc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!config->init_data)
		return ERR_PTR(-EINVAL);

	init_data = config->init_data;
	init_data->constraints.apply_uV = 0;

	config->supply_name = init_data->constraints.name;
	if (init_data->constraints.min_uV == init_data->constraints.max_uV) {
		config->microvolts = init_data->constraints.min_uV;
	} else {
		dev_err(dev,
			 "Fixed regulator specified with variable voltages\n");
		return ERR_PTR(-EINVAL);
	}

	if (init_data->constraints.boot_on)
		config->enabled_at_boot = true;

<<<<<<< HEAD
	config->gpio = of_get_named_gpio(np, "gpio", 0);
	/*
	 * of_get_named_gpio() currently returns ENODEV rather than
	 * EPROBE_DEFER. This code attempts to be compatible with both
	 * for now; the ENODEV check can be removed once the API is fixed.
	 * of_get_named_gpio() doesn't differentiate between a missing
	 * property (which would be fine here, since the GPIO is optional)
	 * and some other error. Patches have been posted for both issues.
	 * Once they are check in, we should replace this with:
	 * if (config->gpio < 0 && config->gpio != -ENOENT)
	 */
	if ((config->gpio == -ENODEV) || (config->gpio == -EPROBE_DEFER))
		return ERR_PTR(-EPROBE_DEFER);

	delay = of_get_property(np, "startup-delay-us", NULL);
	if (delay)
		config->startup_delay = be32_to_cpu(*delay);

	if (of_find_property(np, "enable-active-high", NULL))
		config->enable_high = true;

	if (of_find_property(np, "parent-supply", NULL))
		init_data->supply_regulator = "parent";
=======
	of_property_read_u32(np, "startup-delay-us", &config->startup_delay);
	of_property_read_u32(np, "off-on-delay-us", &config->off_on_delay);

	if (of_property_present(np, "vin-supply"))
		config->input_supply = "vin";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return config;
}

<<<<<<< HEAD
static int fixed_voltage_is_enabled(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	return data->is_enabled;
}

static int fixed_voltage_enable(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	if (gpio_is_valid(data->gpio)) {
		gpio_set_value_cansleep(data->gpio, data->enable_high);
		data->is_enabled = true;
	}

	return 0;
}

static int fixed_voltage_disable(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	if (gpio_is_valid(data->gpio)) {
		gpio_set_value_cansleep(data->gpio, !data->enable_high);
		data->is_enabled = false;
	}

	return 0;
}

static int fixed_voltage_enable_time(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	return data->startup_delay;
}

static int fixed_voltage_get_voltage(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	if (data->microvolts)
		return data->microvolts;
	else
		return -EINVAL;
}

static int fixed_voltage_list_voltage(struct regulator_dev *dev,
				      unsigned selector)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	if (selector != 0)
		return -EINVAL;

	return data->microvolts;
}

static struct regulator_ops fixed_voltage_ops = {
	.is_enabled = fixed_voltage_is_enabled,
	.enable = fixed_voltage_enable,
	.disable = fixed_voltage_disable,
	.enable_time = fixed_voltage_enable_time,
	.get_voltage = fixed_voltage_get_voltage,
	.list_voltage = fixed_voltage_list_voltage,
};

static int __devinit reg_fixed_voltage_probe(struct platform_device *pdev)
{
	struct fixed_voltage_config *config;
	struct fixed_voltage_data *drvdata;
	int ret;

	if (pdev->dev.of_node) {
		config = of_get_fixed_voltage_config(&pdev->dev);
		if (IS_ERR(config))
			return PTR_ERR(config);
	} else {
		config = pdev->dev.platform_data;
=======
static const struct regulator_ops fixed_voltage_ops = {
};

static const struct regulator_ops fixed_voltage_clkenabled_ops = {
	.enable = reg_clock_enable,
	.disable = reg_clock_disable,
	.is_enabled = reg_is_enabled,
};

static const struct regulator_ops fixed_voltage_domain_ops = {
	.enable = reg_domain_enable,
	.disable = reg_domain_disable,
	.is_enabled = reg_is_enabled,
};

static int reg_fixed_voltage_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct fixed_voltage_config *config;
	struct fixed_voltage_data *drvdata;
	const struct fixed_dev_type *drvtype = of_device_get_match_data(dev);
	struct regulator_config cfg = { };
	enum gpiod_flags gflags;
	int ret;

	drvdata = devm_kzalloc(&pdev->dev, sizeof(struct fixed_voltage_data),
			       GFP_KERNEL);
	if (!drvdata)
		return -ENOMEM;

	if (pdev->dev.of_node) {
		config = of_get_fixed_voltage_config(&pdev->dev,
						     &drvdata->desc);
		if (IS_ERR(config))
			return PTR_ERR(config);
	} else {
		config = dev_get_platdata(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!config)
		return -ENOMEM;

<<<<<<< HEAD
	drvdata = kzalloc(sizeof(struct fixed_voltage_data), GFP_KERNEL);
	if (drvdata == NULL) {
		dev_err(&pdev->dev, "Failed to allocate device data\n");
		ret = -ENOMEM;
		goto err;
	}

	drvdata->desc.name = kstrdup(config->supply_name, GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		dev_err(&pdev->dev, "Failed to allocate supply name\n");
		ret = -ENOMEM;
		goto err;
	}
	drvdata->desc.type = REGULATOR_VOLTAGE;
	drvdata->desc.owner = THIS_MODULE;
	drvdata->desc.ops = &fixed_voltage_ops;
=======
	drvdata->desc.name = devm_kstrdup(&pdev->dev,
					  config->supply_name,
					  GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		dev_err(&pdev->dev, "Failed to allocate supply name\n");
		return -ENOMEM;
	}
	drvdata->desc.type = REGULATOR_VOLTAGE;
	drvdata->desc.owner = THIS_MODULE;

	if (drvtype && drvtype->has_enable_clock) {
		drvdata->desc.ops = &fixed_voltage_clkenabled_ops;

		drvdata->enable_clock = devm_clk_get(dev, NULL);
		if (IS_ERR(drvdata->enable_clock)) {
			dev_err(dev, "Can't get enable-clock from devicetree\n");
			return PTR_ERR(drvdata->enable_clock);
		}
	} else if (drvtype && drvtype->has_performance_state) {
		drvdata->desc.ops = &fixed_voltage_domain_ops;

		drvdata->performance_state = of_get_required_opp_performance_state(dev->of_node, 0);
		if (drvdata->performance_state < 0) {
			dev_err(dev, "Can't get performance state from devicetree\n");
			return drvdata->performance_state;
		}
	} else {
		drvdata->desc.ops = &fixed_voltage_ops;
	}

	drvdata->desc.enable_time = config->startup_delay;
	drvdata->desc.off_on_delay = config->off_on_delay;

	if (config->input_supply) {
		drvdata->desc.supply_name = devm_kstrdup(&pdev->dev,
					    config->input_supply,
					    GFP_KERNEL);
		if (!drvdata->desc.supply_name)
			return -ENOMEM;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (config->microvolts)
		drvdata->desc.n_voltages = 1;

<<<<<<< HEAD
	drvdata->microvolts = config->microvolts;
	drvdata->gpio = config->gpio;
	drvdata->startup_delay = config->startup_delay;

	if (gpio_is_valid(config->gpio)) {
		drvdata->enable_high = config->enable_high;

		/* FIXME: Remove below print warning
		 *
		 * config->gpio must be set to -EINVAL by platform code if
		 * GPIO control is not required. However, early adopters
		 * not requiring GPIO control may forget to initialize
		 * config->gpio to -EINVAL. This will cause GPIO 0 to be used
		 * for GPIO control.
		 *
		 * This warning will be removed once there are a couple of users
		 * for this driver.
		 */
		if (!config->gpio)
			dev_warn(&pdev->dev,
				"using GPIO 0 for regulator enable control\n");

		ret = gpio_request(config->gpio, config->supply_name);
		if (ret) {
			dev_err(&pdev->dev,
			   "Could not obtain regulator enable GPIO %d: %d\n",
							config->gpio, ret);
			goto err_name;
		}

		/* set output direction without changing state
		 * to prevent glitch
		 */
		drvdata->is_enabled = config->enabled_at_boot;
		ret = drvdata->is_enabled ?
				config->enable_high : !config->enable_high;

		ret = gpio_direction_output(config->gpio, ret);
		if (ret) {
			dev_err(&pdev->dev,
			   "Could not configure regulator enable GPIO %d direction: %d\n",
							config->gpio, ret);
			goto err_gpio;
		}

	} else {
		/* Regulator without GPIO control is considered
		 * always enabled
		 */
		drvdata->is_enabled = true;
	}

	drvdata->dev = regulator_register(&drvdata->desc, &pdev->dev,
					  config->init_data, drvdata,
					  pdev->dev.of_node);
	if (IS_ERR(drvdata->dev)) {
		ret = PTR_ERR(drvdata->dev);
		dev_err(&pdev->dev, "Failed to register regulator: %d\n", ret);
		goto err_gpio;
=======
	drvdata->desc.fixed_uV = config->microvolts;

	/*
	 * The signal will be inverted by the GPIO core if flagged so in the
	 * descriptor.
	 */
	if (config->enabled_at_boot)
		gflags = GPIOD_OUT_HIGH;
	else
		gflags = GPIOD_OUT_LOW;

	/*
	 * Some fixed regulators share the enable line between two
	 * regulators which makes it necessary to get a handle on the
	 * same descriptor for two different consumers. This will get
	 * the GPIO descriptor, but only the first call will initialize
	 * it so any flags such as inversion or open drain will only
	 * be set up by the first caller and assumed identical on the
	 * next caller.
	 *
	 * FIXME: find a better way to deal with this.
	 */
	gflags |= GPIOD_FLAGS_BIT_NONEXCLUSIVE;

	/*
	 * Do not use devm* here: the regulator core takes over the
	 * lifecycle management of the GPIO descriptor.
	 */
	cfg.ena_gpiod = gpiod_get_optional(&pdev->dev, NULL, gflags);
	if (IS_ERR(cfg.ena_gpiod))
		return dev_err_probe(&pdev->dev, PTR_ERR(cfg.ena_gpiod),
				     "can't get GPIO\n");

	cfg.dev = &pdev->dev;
	cfg.init_data = config->init_data;
	cfg.driver_data = drvdata;
	cfg.of_node = pdev->dev.of_node;

	drvdata->dev = devm_regulator_register(&pdev->dev, &drvdata->desc,
					       &cfg);
	if (IS_ERR(drvdata->dev)) {
		ret = dev_err_probe(&pdev->dev, PTR_ERR(drvdata->dev),
				    "Failed to register regulator: %ld\n",
				    PTR_ERR(drvdata->dev));
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	platform_set_drvdata(pdev, drvdata);

	dev_dbg(&pdev->dev, "%s supplying %duV\n", drvdata->desc.name,
<<<<<<< HEAD
		drvdata->microvolts);

	return 0;

err_gpio:
	if (gpio_is_valid(config->gpio))
		gpio_free(config->gpio);
err_name:
	kfree(drvdata->desc.name);
err:
	kfree(drvdata);
	return ret;
}

static int __devexit reg_fixed_voltage_remove(struct platform_device *pdev)
{
	struct fixed_voltage_data *drvdata = platform_get_drvdata(pdev);

	regulator_unregister(drvdata->dev);
	if (gpio_is_valid(drvdata->gpio))
		gpio_free(drvdata->gpio);
	kfree(drvdata->desc.name);
	kfree(drvdata);
=======
		drvdata->desc.fixed_uV);

	ret = reg_fixed_get_irqs(dev, drvdata);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

#if defined(CONFIG_OF)
<<<<<<< HEAD
static const struct of_device_id fixed_of_match[] __devinitconst = {
	{ .compatible = "regulator-fixed", },
	{},
};
MODULE_DEVICE_TABLE(of, fixed_of_match);
#else
#define fixed_of_match NULL
=======
static const struct fixed_dev_type fixed_voltage_data = {
	.has_enable_clock = false,
};

static const struct fixed_dev_type fixed_clkenable_data = {
	.has_enable_clock = true,
};

static const struct fixed_dev_type fixed_domain_data = {
	.has_performance_state = true,
};

static const struct of_device_id fixed_of_match[] = {
	{
		.compatible = "regulator-fixed",
		.data = &fixed_voltage_data,
	},
	{
		.compatible = "regulator-fixed-clock",
		.data = &fixed_clkenable_data,
	},
	{
		.compatible = "regulator-fixed-domain",
		.data = &fixed_domain_data,
	},
	{
	},
};
MODULE_DEVICE_TABLE(of, fixed_of_match);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static struct platform_driver regulator_fixed_voltage_driver = {
	.probe		= reg_fixed_voltage_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(reg_fixed_voltage_remove),
	.driver		= {
		.name		= "reg-fixed-voltage",
		.owner		= THIS_MODULE,
		.of_match_table = fixed_of_match,
=======
	.driver		= {
		.name		= "reg-fixed-voltage",
		.probe_type	= PROBE_PREFER_ASYNCHRONOUS,
		.of_match_table = of_match_ptr(fixed_of_match),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static int __init regulator_fixed_voltage_init(void)
{
	return platform_driver_register(&regulator_fixed_voltage_driver);
}
subsys_initcall(regulator_fixed_voltage_init);

static void __exit regulator_fixed_voltage_exit(void)
{
	platform_driver_unregister(&regulator_fixed_voltage_driver);
}
module_exit(regulator_fixed_voltage_exit);

MODULE_AUTHOR("Mark Brown <broonie@opensource.wolfsonmicro.com>");
MODULE_DESCRIPTION("Fixed voltage regulator");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:reg-fixed-voltage");
