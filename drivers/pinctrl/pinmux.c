<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Core driver for the pin muxing portions of the pin control subsystem
 *
 * Copyright (C) 2011-2012 ST-Ericsson SA
 * Written on behalf of Linaro for ST-Ericsson
 * Based on bits of regulator core, gpio core and clk core
 *
 * Author: Linus Walleij <linus.walleij@linaro.org>
 *
 * Copyright (C) 2012 NVIDIA CORPORATION. All rights reserved.
<<<<<<< HEAD
 *
 * License terms: GNU General Public License (GPL) version 2
 */
#define pr_fmt(fmt) "pinmux core: " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/radix-tree.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/pinctrl/machine.h>
#include <linux/pinctrl/pinmux.h>
=======
 */
#define pr_fmt(fmt) "pinmux core: " fmt

#include <linux/array_size.h>
#include <linux/ctype.h>
#include <linux/debugfs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/radix-tree.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/string.h>

#include <linux/pinctrl/machine.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/pinmux.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "core.h"
#include "pinmux.h"

int pinmux_check_ops(struct pinctrl_dev *pctldev)
{
	const struct pinmux_ops *ops = pctldev->desc->pmxops;
<<<<<<< HEAD
	unsigned nfuncs;
	unsigned selector = 0;
=======
	unsigned int nfuncs;
	unsigned int selector = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check that we implement required operations */
	if (!ops ||
	    !ops->get_functions_count ||
	    !ops->get_function_name ||
	    !ops->get_function_groups ||
<<<<<<< HEAD
	    !ops->enable) {
=======
	    !ops->set_mux) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(pctldev->dev, "pinmux ops lacks necessary functions\n");
		return -EINVAL;
	}
	/* Check that all functions registered have names */
	nfuncs = ops->get_functions_count(pctldev);
	while (selector < nfuncs) {
		const char *fname = ops->get_function_name(pctldev,
							   selector);
		if (!fname) {
			dev_err(pctldev->dev, "pinmux ops has no name for function%u\n",
				selector);
			return -EINVAL;
		}
		selector++;
	}

	return 0;
}

<<<<<<< HEAD
int pinmux_validate_map(struct pinctrl_map const *map, int i)
=======
int pinmux_validate_map(const struct pinctrl_map *map, int i)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (!map->data.mux.function) {
		pr_err("failed to register map %s (%d): no function given\n",
		       map->name, i);
		return -EINVAL;
	}

	return 0;
}

/**
<<<<<<< HEAD
 * pin_request() - request a single pin to be muxed in, typically for GPIO
=======
 * pinmux_can_be_used_for_gpio() - check if a specific pin
 *	is either muxed to a different function or used as gpio.
 *
 * @pctldev: the associated pin controller device
 * @pin: the pin number in the global pin space
 *
 * Controllers not defined as strict will always return true,
 * menaning that the gpio can be used.
 */
bool pinmux_can_be_used_for_gpio(struct pinctrl_dev *pctldev, unsigned int pin)
{
	struct pin_desc *desc = pin_desc_get(pctldev, pin);
	const struct pinmux_ops *ops = pctldev->desc->pmxops;

	/* Can't inspect pin, assume it can be used */
	if (!desc || !ops)
		return true;

	if (ops->strict && desc->mux_usecount)
		return false;

	return !(ops->strict && !!desc->gpio_owner);
}

/**
 * pin_request() - request a single pin to be muxed in, typically for GPIO
 * @pctldev: the associated pin controller device
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @pin: the pin number in the global pin space
 * @owner: a representation of the owner of this pin; typically the device
 *	name that controls its mux function, or the requested GPIO name
 * @gpio_range: the range matching the GPIO pin if this is a request for a
 *	single GPIO pin
 */
static int pin_request(struct pinctrl_dev *pctldev,
		       int pin, const char *owner,
		       struct pinctrl_gpio_range *gpio_range)
{
	struct pin_desc *desc;
	const struct pinmux_ops *ops = pctldev->desc->pmxops;
	int status = -EINVAL;

	desc = pin_desc_get(pctldev, pin);
	if (desc == NULL) {
		dev_err(pctldev->dev,
			"pin %d is not registered so it cannot be requested\n",
			pin);
		goto out;
	}

	dev_dbg(pctldev->dev, "request pin %d (%s) for %s\n",
		pin, desc->name, owner);

<<<<<<< HEAD
	if (gpio_range) {
		/* There's no need to support multiple GPIO requests */
		if (desc->gpio_owner) {
			dev_err(pctldev->dev,
				"pin %s already requested by %s; cannot claim for %s\n",
				desc->name, desc->gpio_owner, owner);
			goto out;
		}

		desc->gpio_owner = owner;
	} else {
		if (desc->mux_usecount && strcmp(desc->mux_owner, owner)) {
			dev_err(pctldev->dev,
				"pin %s already requested by %s; cannot claim for %s\n",
				desc->name, desc->mux_owner, owner);
			goto out;
		}

=======
	if ((!gpio_range || ops->strict) &&
	    desc->mux_usecount && strcmp(desc->mux_owner, owner)) {
		dev_err(pctldev->dev,
			"pin %s already requested by %s; cannot claim for %s\n",
			desc->name, desc->mux_owner, owner);
		goto out;
	}

	if ((gpio_range || ops->strict) && desc->gpio_owner) {
		dev_err(pctldev->dev,
			"pin %s already requested by %s; cannot claim for %s\n",
			desc->name, desc->gpio_owner, owner);
		goto out;
	}

	if (gpio_range) {
		desc->gpio_owner = owner;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		desc->mux_usecount++;
		if (desc->mux_usecount > 1)
			return 0;

		desc->mux_owner = owner;
	}

	/* Let each pin increase references to this module */
	if (!try_module_get(pctldev->owner)) {
		dev_err(pctldev->dev,
			"could not increase module refcount for pin %d\n",
			pin);
		status = -EINVAL;
		goto out_free_pin;
	}

	/*
	 * If there is no kind of request function for the pin we just assume
	 * we got it by default and proceed.
	 */
	if (gpio_range && ops->gpio_request_enable)
		/* This requests and enables a single GPIO pin */
		status = ops->gpio_request_enable(pctldev, gpio_range, pin);
	else if (ops->request)
		status = ops->request(pctldev, pin);
	else
		status = 0;

<<<<<<< HEAD
	if (status) {
		dev_err(pctldev->dev, "request() failed for pin %d\n", pin);
		module_put(pctldev->owner);
	}
=======
	if (status)
		module_put(pctldev->owner);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out_free_pin:
	if (status) {
		if (gpio_range) {
			desc->gpio_owner = NULL;
		} else {
			desc->mux_usecount--;
			if (!desc->mux_usecount)
				desc->mux_owner = NULL;
		}
	}
out:
	if (status)
<<<<<<< HEAD
		dev_err(pctldev->dev, "pin-%d (%s) status %d\n",
			pin, owner, status);
=======
		dev_err_probe(pctldev->dev, status, "pin-%d (%s)\n",
			      pin, owner);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return status;
}

/**
 * pin_free() - release a single muxed in pin so something else can be muxed
 * @pctldev: pin controller device handling this pin
 * @pin: the pin to free
 * @gpio_range: the range matching the GPIO pin if this is a request for a
 *	single GPIO pin
 *
 * This function returns a pointer to the previous owner. This is used
 * for callers that dynamically allocate an owner name so it can be freed
 * once the pin is free. This is done for GPIO request functions.
 */
static const char *pin_free(struct pinctrl_dev *pctldev, int pin,
			    struct pinctrl_gpio_range *gpio_range)
{
	const struct pinmux_ops *ops = pctldev->desc->pmxops;
	struct pin_desc *desc;
	const char *owner;

	desc = pin_desc_get(pctldev, pin);
	if (desc == NULL) {
		dev_err(pctldev->dev,
			"pin is not registered so it cannot be freed\n");
		return NULL;
	}

	if (!gpio_range) {
		/*
		 * A pin should not be freed more times than allocated.
		 */
		if (WARN_ON(!desc->mux_usecount))
			return NULL;
		desc->mux_usecount--;
		if (desc->mux_usecount)
			return NULL;
	}

	/*
	 * If there is no kind of request function for the pin we just assume
	 * we got it by default and proceed.
	 */
	if (gpio_range && ops->gpio_disable_free)
		ops->gpio_disable_free(pctldev, gpio_range, pin);
	else if (ops->free)
		ops->free(pctldev, pin);

	if (gpio_range) {
		owner = desc->gpio_owner;
		desc->gpio_owner = NULL;
	} else {
		owner = desc->mux_owner;
		desc->mux_owner = NULL;
		desc->mux_setting = NULL;
	}

	module_put(pctldev->owner);

	return owner;
}

/**
 * pinmux_request_gpio() - request pinmuxing for a GPIO pin
 * @pctldev: pin controller device affected
 * @pin: the pin to mux in for GPIO
 * @range: the applicable GPIO range
<<<<<<< HEAD
 */
int pinmux_request_gpio(struct pinctrl_dev *pctldev,
			struct pinctrl_gpio_range *range,
			unsigned pin, unsigned gpio)
=======
 * @gpio: number of requested GPIO
 */
int pinmux_request_gpio(struct pinctrl_dev *pctldev,
			struct pinctrl_gpio_range *range,
			unsigned int pin, unsigned int gpio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const char *owner;
	int ret;

	/* Conjure some name stating what chip and pin this is taken by */
	owner = kasprintf(GFP_KERNEL, "%s:%d", range->name, gpio);
	if (!owner)
<<<<<<< HEAD
		return -EINVAL;
=======
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = pin_request(pctldev, pin, owner, range);
	if (ret < 0)
		kfree(owner);

	return ret;
}

/**
 * pinmux_free_gpio() - release a pin from GPIO muxing
 * @pctldev: the pin controller device for the pin
 * @pin: the affected currently GPIO-muxed in pin
 * @range: applicable GPIO range
 */
<<<<<<< HEAD
void pinmux_free_gpio(struct pinctrl_dev *pctldev, unsigned pin,
=======
void pinmux_free_gpio(struct pinctrl_dev *pctldev, unsigned int pin,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		      struct pinctrl_gpio_range *range)
{
	const char *owner;

	owner = pin_free(pctldev, pin, range);
	kfree(owner);
}

/**
 * pinmux_gpio_direction() - set the direction of a single muxed-in GPIO pin
 * @pctldev: the pin controller handling this pin
 * @range: applicable GPIO range
 * @pin: the affected GPIO pin in this controller
 * @input: true if we set the pin as input, false for output
 */
int pinmux_gpio_direction(struct pinctrl_dev *pctldev,
			  struct pinctrl_gpio_range *range,
<<<<<<< HEAD
			  unsigned pin, bool input)
=======
			  unsigned int pin, bool input)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct pinmux_ops *ops;
	int ret;

	ops = pctldev->desc->pmxops;

	if (ops->gpio_set_direction)
		ret = ops->gpio_set_direction(pctldev, range, pin, input);
	else
		ret = 0;

	return ret;
}

static int pinmux_func_name_to_selector(struct pinctrl_dev *pctldev,
					const char *function)
{
	const struct pinmux_ops *ops = pctldev->desc->pmxops;
<<<<<<< HEAD
	unsigned nfuncs = ops->get_functions_count(pctldev);
	unsigned selector = 0;

	/* See if this pctldev has this function */
	while (selector < nfuncs) {
		const char *fname = ops->get_function_name(pctldev,
							   selector);
=======
	unsigned int nfuncs = ops->get_functions_count(pctldev);
	unsigned int selector = 0;

	/* See if this pctldev has this function */
	while (selector < nfuncs) {
		const char *fname = ops->get_function_name(pctldev, selector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!strcmp(function, fname))
			return selector;

		selector++;
	}

<<<<<<< HEAD
	pr_err("%s does not support function %s\n",
	       pinctrl_dev_get_name(pctldev), function);
	return -EINVAL;
}

int pinmux_map_to_setting(struct pinctrl_map const *map,
=======
	return -EINVAL;
}

int pinmux_map_to_setting(const struct pinctrl_map *map,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  struct pinctrl_setting *setting)
{
	struct pinctrl_dev *pctldev = setting->pctldev;
	const struct pinmux_ops *pmxops = pctldev->desc->pmxops;
	char const * const *groups;
<<<<<<< HEAD
	unsigned num_groups;
	int ret;
	const char *group;
	int i;
=======
	unsigned int num_groups;
	int ret;
	const char *group;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!pmxops) {
		dev_err(pctldev->dev, "does not support mux function\n");
		return -EINVAL;
	}

	ret = pinmux_func_name_to_selector(pctldev, map->data.mux.function);
	if (ret < 0) {
		dev_err(pctldev->dev, "invalid function %s in map table\n",
			map->data.mux.function);
		return ret;
	}
	setting->data.mux.func = ret;

	ret = pmxops->get_function_groups(pctldev, setting->data.mux.func,
					  &groups, &num_groups);
	if (ret < 0) {
		dev_err(pctldev->dev, "can't query groups for function %s\n",
			map->data.mux.function);
		return ret;
	}
	if (!num_groups) {
		dev_err(pctldev->dev,
			"function %s can't be selected on any group\n",
			map->data.mux.function);
		return -EINVAL;
	}
	if (map->data.mux.group) {
<<<<<<< HEAD
		bool found = false;
		group = map->data.mux.group;
		for (i = 0; i < num_groups; i++) {
			if (!strcmp(group, groups[i])) {
				found = true;
				break;
			}
		}
		if (!found) {
			dev_err(pctldev->dev,
				"invalid group \"%s\" for function \"%s\"\n",
				group, map->data.mux.function);
			return -EINVAL;
=======
		group = map->data.mux.group;
		ret = match_string(groups, num_groups, group);
		if (ret < 0) {
			dev_err(pctldev->dev,
				"invalid group \"%s\" for function \"%s\"\n",
				group, map->data.mux.function);
			return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	} else {
		group = groups[0];
	}

	ret = pinctrl_get_group_selector(pctldev, group);
	if (ret < 0) {
		dev_err(pctldev->dev, "invalid group %s in map table\n",
			map->data.mux.group);
		return ret;
	}
	setting->data.mux.group = ret;

	return 0;
}

<<<<<<< HEAD
void pinmux_free_setting(struct pinctrl_setting const *setting)
=======
void pinmux_free_setting(const struct pinctrl_setting *setting)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* This function is currently unused */
}

<<<<<<< HEAD
int pinmux_enable_setting(struct pinctrl_setting const *setting)
=======
int pinmux_enable_setting(const struct pinctrl_setting *setting)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pinctrl_dev *pctldev = setting->pctldev;
	const struct pinctrl_ops *pctlops = pctldev->desc->pctlops;
	const struct pinmux_ops *ops = pctldev->desc->pmxops;
<<<<<<< HEAD
	int ret;
	const unsigned *pins;
	unsigned num_pins;
	int i;
	struct pin_desc *desc;

	ret = pctlops->get_group_pins(pctldev, setting->data.mux.group,
				      &pins, &num_pins);
	if (ret) {
		/* errors only affect debug data, so just warn */
		dev_warn(pctldev->dev,
			 "could not get pins for group selector %d\n",
			 setting->data.mux.group);
=======
	int ret = 0;
	const unsigned int *pins = NULL;
	unsigned int num_pins = 0;
	int i;
	struct pin_desc *desc;

	if (pctlops->get_group_pins)
		ret = pctlops->get_group_pins(pctldev, setting->data.mux.group,
					      &pins, &num_pins);

	if (ret) {
		const char *gname;

		/* errors only affect debug data, so just warn */
		gname = pctlops->get_group_name(pctldev,
						setting->data.mux.group);
		dev_warn(pctldev->dev,
			 "could not get pins for group %s\n",
			 gname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		num_pins = 0;
	}

	/* Try to allocate all pins in this group, one by one */
	for (i = 0; i < num_pins; i++) {
		ret = pin_request(pctldev, pins[i], setting->dev_name, NULL);
		if (ret) {
<<<<<<< HEAD
			dev_err(pctldev->dev,
				"could not request pin %d on device %s\n",
				pins[i], pinctrl_dev_get_name(pctldev));
=======
			const char *gname;
			const char *pname;

			desc = pin_desc_get(pctldev, pins[i]);
			pname = desc ? desc->name : "non-existing";
			gname = pctlops->get_group_name(pctldev,
						setting->data.mux.group);
			dev_err_probe(pctldev->dev, ret,
				"could not request pin %d (%s) from group %s "
				" on device %s\n",
				pins[i], pname, gname,
				pinctrl_dev_get_name(pctldev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto err_pin_request;
		}
	}

	/* Now that we have acquired the pins, encode the mux setting */
	for (i = 0; i < num_pins; i++) {
		desc = pin_desc_get(pctldev, pins[i]);
		if (desc == NULL) {
			dev_warn(pctldev->dev,
				 "could not get pin desc for pin %d\n",
				 pins[i]);
			continue;
		}
		desc->mux_setting = &(setting->data.mux);
	}

<<<<<<< HEAD
	ret = ops->enable(pctldev, setting->data.mux.func,
			  setting->data.mux.group);

	if (ret)
		goto err_enable;

	return 0;

err_enable:
=======
	ret = ops->set_mux(pctldev, setting->data.mux.func,
			   setting->data.mux.group);

	if (ret)
		goto err_set_mux;

	return 0;

err_set_mux:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < num_pins; i++) {
		desc = pin_desc_get(pctldev, pins[i]);
		if (desc)
			desc->mux_setting = NULL;
	}
err_pin_request:
	/* On error release all taken pins */
	while (--i >= 0)
		pin_free(pctldev, pins[i], NULL);

	return ret;
}

<<<<<<< HEAD
void pinmux_disable_setting(struct pinctrl_setting const *setting)
{
	struct pinctrl_dev *pctldev = setting->pctldev;
	const struct pinctrl_ops *pctlops = pctldev->desc->pctlops;
	const struct pinmux_ops *ops = pctldev->desc->pmxops;
	int ret;
	const unsigned *pins;
	unsigned num_pins;
	int i;
	struct pin_desc *desc;

	ret = pctlops->get_group_pins(pctldev, setting->data.mux.group,
				      &pins, &num_pins);
	if (ret) {
		/* errors only affect debug data, so just warn */
		dev_warn(pctldev->dev,
			 "could not get pins for group selector %d\n",
			 setting->data.mux.group);
=======
void pinmux_disable_setting(const struct pinctrl_setting *setting)
{
	struct pinctrl_dev *pctldev = setting->pctldev;
	const struct pinctrl_ops *pctlops = pctldev->desc->pctlops;
	int ret = 0;
	const unsigned int *pins = NULL;
	unsigned int num_pins = 0;
	int i;
	struct pin_desc *desc;

	if (pctlops->get_group_pins)
		ret = pctlops->get_group_pins(pctldev, setting->data.mux.group,
					      &pins, &num_pins);
	if (ret) {
		const char *gname;

		/* errors only affect debug data, so just warn */
		gname = pctlops->get_group_name(pctldev,
						setting->data.mux.group);
		dev_warn(pctldev->dev,
			 "could not get pins for group %s\n",
			 gname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		num_pins = 0;
	}

	/* Flag the descs that no setting is active */
	for (i = 0; i < num_pins; i++) {
		desc = pin_desc_get(pctldev, pins[i]);
		if (desc == NULL) {
			dev_warn(pctldev->dev,
				 "could not get pin desc for pin %d\n",
				 pins[i]);
			continue;
		}
<<<<<<< HEAD
		desc->mux_setting = NULL;
	}

	/* And release the pins */
	for (i = 0; i < num_pins; i++)
		pin_free(pctldev, pins[i], NULL);

	if (ops->disable)
		ops->disable(pctldev, setting->data.mux.func, setting->data.mux.group);
=======
		if (desc->mux_setting == &(setting->data.mux)) {
			pin_free(pctldev, pins[i], NULL);
		} else {
			const char *gname;

			gname = pctlops->get_group_name(pctldev,
						setting->data.mux.group);
			dev_warn(pctldev->dev,
				 "not freeing pin %d (%s) as part of "
				 "deactivating group %s - it is already "
				 "used for some other setting",
				 pins[i], desc->name, gname);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_DEBUG_FS

/* Called from pincontrol core */
static int pinmux_functions_show(struct seq_file *s, void *what)
{
	struct pinctrl_dev *pctldev = s->private;
	const struct pinmux_ops *pmxops = pctldev->desc->pmxops;
<<<<<<< HEAD
	unsigned nfuncs;
	unsigned func_selector = 0;
=======
	unsigned int nfuncs;
	unsigned int func_selector = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!pmxops)
		return 0;

<<<<<<< HEAD
	mutex_lock(&pinctrl_mutex);
=======
	mutex_lock(&pctldev->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nfuncs = pmxops->get_functions_count(pctldev);
	while (func_selector < nfuncs) {
		const char *func = pmxops->get_function_name(pctldev,
							  func_selector);
		const char * const *groups;
<<<<<<< HEAD
		unsigned num_groups;
=======
		unsigned int num_groups;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int ret;
		int i;

		ret = pmxops->get_function_groups(pctldev, func_selector,
						  &groups, &num_groups);
<<<<<<< HEAD
		if (ret)
			seq_printf(s, "function %s: COULD NOT GET GROUPS\n",
				   func);

		seq_printf(s, "function: %s, groups = [ ", func);
=======
		if (ret) {
			seq_printf(s, "function %s: COULD NOT GET GROUPS\n",
				   func);
			func_selector++;
			continue;
		}

		seq_printf(s, "function %d: %s, groups = [ ", func_selector, func);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (i = 0; i < num_groups; i++)
			seq_printf(s, "%s ", groups[i]);
		seq_puts(s, "]\n");

		func_selector++;
	}

<<<<<<< HEAD
	mutex_unlock(&pinctrl_mutex);
=======
	mutex_unlock(&pctldev->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int pinmux_pins_show(struct seq_file *s, void *what)
{
	struct pinctrl_dev *pctldev = s->private;
	const struct pinctrl_ops *pctlops = pctldev->desc->pctlops;
	const struct pinmux_ops *pmxops = pctldev->desc->pmxops;
<<<<<<< HEAD
	unsigned i, pin;
=======
	unsigned int i, pin;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!pmxops)
		return 0;

	seq_puts(s, "Pinmux settings per pin\n");
<<<<<<< HEAD
	seq_puts(s, "Format: pin (name): mux_owner gpio_owner hog?\n");

	mutex_lock(&pinctrl_mutex);
=======
	if (pmxops->strict)
		seq_puts(s,
		 "Format: pin (name): mux_owner|gpio_owner (strict) hog?\n");
	else
		seq_puts(s,
		"Format: pin (name): mux_owner gpio_owner hog?\n");

	mutex_lock(&pctldev->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* The pin number can be retrived from the pin controller descriptor */
	for (i = 0; i < pctldev->desc->npins; i++) {
		struct pin_desc *desc;
		bool is_hog = false;

		pin = pctldev->desc->pins[i].number;
		desc = pin_desc_get(pctldev, pin);
		/* Skip if we cannot search the pin */
		if (desc == NULL)
			continue;

		if (desc->mux_owner &&
		    !strcmp(desc->mux_owner, pinctrl_dev_get_name(pctldev)))
			is_hog = true;

<<<<<<< HEAD
		seq_printf(s, "pin %d (%s): %s %s%s", pin,
			   desc->name ? desc->name : "unnamed",
			   desc->mux_owner ? desc->mux_owner
				: "(MUX UNCLAIMED)",
			   desc->gpio_owner ? desc->gpio_owner
				: "(GPIO UNCLAIMED)",
			   is_hog ? " (HOG)" : "");

=======
		if (pmxops->strict) {
			if (desc->mux_owner)
				seq_printf(s, "pin %d (%s): device %s%s",
					   pin, desc->name, desc->mux_owner,
					   is_hog ? " (HOG)" : "");
			else if (desc->gpio_owner)
				seq_printf(s, "pin %d (%s): GPIO %s",
					   pin, desc->name, desc->gpio_owner);
			else
				seq_printf(s, "pin %d (%s): UNCLAIMED",
					   pin, desc->name);
		} else {
			/* For non-strict controllers */
			seq_printf(s, "pin %d (%s): %s %s%s", pin, desc->name,
				   desc->mux_owner ? desc->mux_owner
				   : "(MUX UNCLAIMED)",
				   desc->gpio_owner ? desc->gpio_owner
				   : "(GPIO UNCLAIMED)",
				   is_hog ? " (HOG)" : "");
		}

		/* If mux: print function+group claiming the pin */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (desc->mux_setting)
			seq_printf(s, " function %s group %s\n",
				   pmxops->get_function_name(pctldev,
					desc->mux_setting->func),
				   pctlops->get_group_name(pctldev,
					desc->mux_setting->group));
		else
<<<<<<< HEAD
			seq_printf(s, "\n");
	}

	mutex_unlock(&pinctrl_mutex);
=======
			seq_putc(s, '\n');
	}

	mutex_unlock(&pctldev->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
void pinmux_show_map(struct seq_file *s, struct pinctrl_map const *map)
=======
void pinmux_show_map(struct seq_file *s, const struct pinctrl_map *map)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	seq_printf(s, "group %s\nfunction %s\n",
		map->data.mux.group ? map->data.mux.group : "(default)",
		map->data.mux.function);
}

void pinmux_show_setting(struct seq_file *s,
<<<<<<< HEAD
			 struct pinctrl_setting const *setting)
=======
			 const struct pinctrl_setting *setting)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pinctrl_dev *pctldev = setting->pctldev;
	const struct pinmux_ops *pmxops = pctldev->desc->pmxops;
	const struct pinctrl_ops *pctlops = pctldev->desc->pctlops;

	seq_printf(s, "group: %s (%u) function: %s (%u)\n",
		   pctlops->get_group_name(pctldev, setting->data.mux.group),
		   setting->data.mux.group,
		   pmxops->get_function_name(pctldev, setting->data.mux.func),
		   setting->data.mux.func);
}

<<<<<<< HEAD
static int pinmux_functions_open(struct inode *inode, struct file *file)
{
	return single_open(file, pinmux_functions_show, inode->i_private);
}

static int pinmux_pins_open(struct inode *inode, struct file *file)
{
	return single_open(file, pinmux_pins_show, inode->i_private);
}

static const struct file_operations pinmux_functions_ops = {
	.open		= pinmux_functions_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations pinmux_pins_ops = {
	.open		= pinmux_pins_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
=======
DEFINE_SHOW_ATTRIBUTE(pinmux_functions);
DEFINE_SHOW_ATTRIBUTE(pinmux_pins);

static ssize_t pinmux_select(struct file *file, const char __user *user_buf,
				   size_t len, loff_t *ppos)
{
	struct seq_file *sfile = file->private_data;
	struct pinctrl_dev *pctldev = sfile->private;
	const struct pinmux_ops *pmxops = pctldev->desc->pmxops;
	const char *const *groups;
	char *buf, *gname, *fname;
	unsigned int num_groups;
	int fsel, gsel, ret;

	buf = memdup_user_nul(user_buf, len);
	if (IS_ERR(buf))
		return PTR_ERR(buf);

	/* remove leading and trailing spaces of input buffer */
	gname = strstrip(buf);
	if (*gname == '\0') {
		ret = -EINVAL;
		goto exit_free_buf;
	}

	/* find a separator which is a spacelike character */
	for (fname = gname; !isspace(*fname); fname++) {
		if (*fname == '\0') {
			ret = -EINVAL;
			goto exit_free_buf;
		}
	}
	*fname = '\0';

	/* drop extra spaces between function and group names */
	fname = skip_spaces(fname + 1);
	if (*fname == '\0') {
		ret = -EINVAL;
		goto exit_free_buf;
	}

	ret = pinmux_func_name_to_selector(pctldev, fname);
	if (ret < 0) {
		dev_err(pctldev->dev, "invalid function %s in map table\n", fname);
		goto exit_free_buf;
	}
	fsel = ret;

	ret = pmxops->get_function_groups(pctldev, fsel, &groups, &num_groups);
	if (ret) {
		dev_err(pctldev->dev, "no groups for function %d (%s)", fsel, fname);
		goto exit_free_buf;
	}

	ret = match_string(groups, num_groups, gname);
	if (ret < 0) {
		dev_err(pctldev->dev, "invalid group %s", gname);
		goto exit_free_buf;
	}

	ret = pinctrl_get_group_selector(pctldev, gname);
	if (ret < 0)
		goto exit_free_buf;
	gsel = ret;

	ret = pmxops->set_mux(pctldev, fsel, gsel);
	if (ret) {
		dev_err(pctldev->dev, "set_mux() failed: %d", ret);
		goto exit_free_buf;
	}
	ret = len;

exit_free_buf:
	kfree(buf);

	return ret;
}

static int pinmux_select_open(struct inode *inode, struct file *file)
{
	return single_open(file, NULL, inode->i_private);
}

static const struct file_operations pinmux_select_ops = {
	.owner = THIS_MODULE,
	.open = pinmux_select_open,
	.write = pinmux_select,
	.llseek = no_llseek,
	.release = single_release,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

void pinmux_init_device_debugfs(struct dentry *devroot,
			 struct pinctrl_dev *pctldev)
{
<<<<<<< HEAD
	debugfs_create_file("pinmux-functions", S_IFREG | S_IRUGO,
			    devroot, pctldev, &pinmux_functions_ops);
	debugfs_create_file("pinmux-pins", S_IFREG | S_IRUGO,
			    devroot, pctldev, &pinmux_pins_ops);
}

#endif /* CONFIG_DEBUG_FS */
=======
	debugfs_create_file("pinmux-functions", 0444,
			    devroot, pctldev, &pinmux_functions_fops);
	debugfs_create_file("pinmux-pins", 0444,
			    devroot, pctldev, &pinmux_pins_fops);
	debugfs_create_file("pinmux-select", 0200,
			    devroot, pctldev, &pinmux_select_ops);
}

#endif /* CONFIG_DEBUG_FS */

#ifdef CONFIG_GENERIC_PINMUX_FUNCTIONS

/**
 * pinmux_generic_get_function_count() - returns number of functions
 * @pctldev: pin controller device
 */
int pinmux_generic_get_function_count(struct pinctrl_dev *pctldev)
{
	return pctldev->num_functions;
}
EXPORT_SYMBOL_GPL(pinmux_generic_get_function_count);

/**
 * pinmux_generic_get_function_name() - returns the function name
 * @pctldev: pin controller device
 * @selector: function number
 */
const char *
pinmux_generic_get_function_name(struct pinctrl_dev *pctldev,
				 unsigned int selector)
{
	struct function_desc *function;

	function = radix_tree_lookup(&pctldev->pin_function_tree,
				     selector);
	if (!function)
		return NULL;

	return function->name;
}
EXPORT_SYMBOL_GPL(pinmux_generic_get_function_name);

/**
 * pinmux_generic_get_function_groups() - gets the function groups
 * @pctldev: pin controller device
 * @selector: function number
 * @groups: array of pin groups
 * @num_groups: number of pin groups
 */
int pinmux_generic_get_function_groups(struct pinctrl_dev *pctldev,
				       unsigned int selector,
				       const char * const **groups,
				       unsigned int * const num_groups)
{
	struct function_desc *function;

	function = radix_tree_lookup(&pctldev->pin_function_tree,
				     selector);
	if (!function) {
		dev_err(pctldev->dev, "%s could not find function%i\n",
			__func__, selector);
		return -EINVAL;
	}
	*groups = function->group_names;
	*num_groups = function->num_group_names;

	return 0;
}
EXPORT_SYMBOL_GPL(pinmux_generic_get_function_groups);

/**
 * pinmux_generic_get_function() - returns a function based on the number
 * @pctldev: pin controller device
 * @selector: function number
 */
struct function_desc *pinmux_generic_get_function(struct pinctrl_dev *pctldev,
						  unsigned int selector)
{
	struct function_desc *function;

	function = radix_tree_lookup(&pctldev->pin_function_tree,
				     selector);
	if (!function)
		return NULL;

	return function;
}
EXPORT_SYMBOL_GPL(pinmux_generic_get_function);

/**
 * pinmux_generic_add_function() - adds a function group
 * @pctldev: pin controller device
 * @name: name of the function
 * @groups: array of pin groups
 * @num_groups: number of pin groups
 * @data: pin controller driver specific data
 */
int pinmux_generic_add_function(struct pinctrl_dev *pctldev,
				const char *name,
				const char * const *groups,
				const unsigned int num_groups,
				void *data)
{
	struct function_desc *function;
	int selector, error;

	if (!name)
		return -EINVAL;

	selector = pinmux_func_name_to_selector(pctldev, name);
	if (selector >= 0)
		return selector;

	selector = pctldev->num_functions;

	function = devm_kzalloc(pctldev->dev, sizeof(*function), GFP_KERNEL);
	if (!function)
		return -ENOMEM;

	function->name = name;
	function->group_names = groups;
	function->num_group_names = num_groups;
	function->data = data;

	error = radix_tree_insert(&pctldev->pin_function_tree, selector, function);
	if (error)
		return error;

	pctldev->num_functions++;

	return selector;
}
EXPORT_SYMBOL_GPL(pinmux_generic_add_function);

/**
 * pinmux_generic_remove_function() - removes a numbered function
 * @pctldev: pin controller device
 * @selector: function number
 *
 * Note that the caller must take care of locking.
 */
int pinmux_generic_remove_function(struct pinctrl_dev *pctldev,
				   unsigned int selector)
{
	struct function_desc *function;

	function = radix_tree_lookup(&pctldev->pin_function_tree,
				     selector);
	if (!function)
		return -ENOENT;

	radix_tree_delete(&pctldev->pin_function_tree, selector);
	devm_kfree(pctldev->dev, function);

	pctldev->num_functions--;

	return 0;
}
EXPORT_SYMBOL_GPL(pinmux_generic_remove_function);

/**
 * pinmux_generic_free_functions() - removes all functions
 * @pctldev: pin controller device
 *
 * Note that the caller must take care of locking. The pinctrl
 * functions are allocated with devm_kzalloc() so no need to free
 * them here.
 */
void pinmux_generic_free_functions(struct pinctrl_dev *pctldev)
{
	struct radix_tree_iter iter;
	void __rcu **slot;

	radix_tree_for_each_slot(slot, &pctldev->pin_function_tree, &iter, 0)
		radix_tree_delete(&pctldev->pin_function_tree, iter.index);

	pctldev->num_functions = 0;
}

#endif /* CONFIG_GENERIC_PINMUX_FUNCTIONS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
