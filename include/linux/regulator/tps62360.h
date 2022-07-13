<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * tps62360.h -- TI tps62360
 *
 * Interface for regulator driver for TI TPS62360 Processor core supply
 *
 * Copyright (C) 2012 NVIDIA Corporation

 * Author: Laxman Dewangan <ldewangan@nvidia.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA	02110-1301, USA.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __LINUX_REGULATOR_TPS62360_H
#define __LINUX_REGULATOR_TPS62360_H

<<<<<<< HEAD
#include <linux/regulator/machine.h>

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * struct tps62360_regulator_platform_data - tps62360 regulator platform data.
 *
 * @reg_init_data: The regulator init data.
<<<<<<< HEAD
 * @en_force_pwm: Enable force pwm or not.
 * @en_discharge: Enable discharge the output capacitor via internal
 *                register.
 * @en_internal_pulldn: internal pull down enable or not.
 * @vsel0_gpio: Gpio number for vsel0. It should be -1 if this is tied with
 *              fixed logic.
 * @vsel1_gpio: Gpio number for vsel1. It should be -1 if this is tied with
 *              fixed logic.
=======
 * @en_discharge: Enable discharge the output capacitor via internal
 *                register.
 * @en_internal_pulldn: internal pull down enable or not.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @vsel0_def_state: Default state of vsel0. 1 if it is high else 0.
 * @vsel1_def_state: Default state of vsel1. 1 if it is high else 0.
 */
struct tps62360_regulator_platform_data {
<<<<<<< HEAD
	struct regulator_init_data reg_init_data;
	bool en_force_pwm;
	bool en_discharge;
	bool en_internal_pulldn;
	int vsel0_gpio;
	int vsel1_gpio;
=======
	struct regulator_init_data *reg_init_data;
	bool en_discharge;
	bool en_internal_pulldn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int vsel0_def_state;
	int vsel1_def_state;
};

#endif /* __LINUX_REGULATOR_TPS62360_H */
