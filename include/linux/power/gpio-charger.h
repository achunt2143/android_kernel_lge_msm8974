<<<<<<< HEAD
/*
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
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
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __LINUX_POWER_GPIO_CHARGER_H__
#define __LINUX_POWER_GPIO_CHARGER_H__

#include <linux/power_supply.h>
#include <linux/types.h>

/**
 * struct gpio_charger_platform_data - platform_data for gpio_charger devices
 * @name:		Name for the chargers power_supply device
 * @type:		Type of the charger
<<<<<<< HEAD
 * @gpio:		GPIO which is used to indicate the chargers status
 * @gpio_active_low:	Should be set to 1 if the GPIO is active low otherwise 0
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @supplied_to:	Array of battery names to which this chargers supplies power
 * @num_supplicants:	Number of entries in the supplied_to array
 */
struct gpio_charger_platform_data {
	const char *name;
	enum power_supply_type type;
<<<<<<< HEAD

	int gpio;
	int gpio_active_low;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char **supplied_to;
	size_t num_supplicants;
};

#endif
