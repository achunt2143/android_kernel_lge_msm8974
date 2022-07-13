<<<<<<< HEAD
/***************************************************************************
 *   Copyright (C) 2010-2012 Hans de Goede <hdegoede@redhat.com>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <linux/mutex.h>

struct sch56xx_watchdog_data;

=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/***************************************************************************
 *   Copyright (C) 2010-2012 Hans de Goede <hdegoede@redhat.com>           *
 *                                                                         *
 ***************************************************************************/

#include <linux/mutex.h>
#include <linux/regmap.h>

struct sch56xx_watchdog_data;

struct regmap *devm_regmap_init_sch56xx(struct device *dev, struct mutex *lock, u16 addr,
					const struct regmap_config *config);
int sch56xx_regmap_read16(struct regmap *map, unsigned int reg, unsigned int *val);
int sch56xx_regmap_write16(struct regmap *map, unsigned int reg, unsigned int val);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int sch56xx_read_virtual_reg(u16 addr, u16 reg);
int sch56xx_write_virtual_reg(u16 addr, u16 reg, u8 val);
int sch56xx_read_virtual_reg16(u16 addr, u16 reg);
int sch56xx_read_virtual_reg12(u16 addr, u16 msb_reg, u16 lsn_reg,
			       int high_nibble);

<<<<<<< HEAD
struct sch56xx_watchdog_data *sch56xx_watchdog_register(
	u16 addr, u32 revision, struct mutex *io_lock, int check_enabled);
=======
void sch56xx_watchdog_register(struct device *parent, u16 addr, u32 revision,
			       struct mutex *io_lock, int check_enabled);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void sch56xx_watchdog_unregister(struct sch56xx_watchdog_data *data);
