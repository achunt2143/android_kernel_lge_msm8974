<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * fixed.h
 *
 * Copyright 2008 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * Copyright (c) 2009 Nokia Corporation
 * Roger Quadros <ext-roger.quadros@nokia.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __REGULATOR_FIXED_H
#define __REGULATOR_FIXED_H

struct regulator_init_data;

/**
 * struct fixed_voltage_config - fixed_voltage_config structure
 * @supply_name:	Name of the regulator supply
<<<<<<< HEAD
 * @microvolts:		Output voltage of regulator
 * @gpio:		GPIO to use for enable control
 * 			set to -EINVAL if not used
 * @startup_delay:	Start-up time in microseconds
 * @enable_high:	Polarity of enable GPIO
 *			1 = Active high, 0 = Active low
=======
 * @input_supply:	Name of the input regulator supply
 * @microvolts:		Output voltage of regulator
 * @startup_delay:	Start-up time in microseconds
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @enabled_at_boot:	Whether regulator has been enabled at
 * 			boot or not. 1 = Yes, 0 = No
 * 			This is used to keep the regulator at
 * 			the default state
 * @init_data:		regulator_init_data
 *
 * This structure contains fixed voltage regulator configuration
 * information that must be passed by platform code to the fixed
 * voltage regulator driver.
 */
struct fixed_voltage_config {
	const char *supply_name;
<<<<<<< HEAD
	int microvolts;
	int gpio;
	unsigned startup_delay;
	unsigned enable_high:1;
=======
	const char *input_supply;
	int microvolts;
	unsigned startup_delay;
	unsigned int off_on_delay;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned enabled_at_boot:1;
	struct regulator_init_data *init_data;
};

struct regulator_consumer_supply;

#if IS_ENABLED(CONFIG_REGULATOR)
<<<<<<< HEAD
struct platform_device *regulator_register_fixed(int id,
		struct regulator_consumer_supply *supplies, int num_supplies);
#else
static inline struct platform_device *regulator_register_fixed(int id,
		struct regulator_consumer_supply *supplies, int num_supplies)
=======
struct platform_device *regulator_register_always_on(int id, const char *name,
		struct regulator_consumer_supply *supplies, int num_supplies, int uv);
#else
static inline struct platform_device *regulator_register_always_on(int id, const char *name,
		struct regulator_consumer_supply *supplies, int num_supplies, int uv)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return NULL;
}
#endif

<<<<<<< HEAD
=======
#define regulator_register_fixed(id, s, ns) regulator_register_always_on(id, \
						"fixed-dummy", s, ns, 0)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
