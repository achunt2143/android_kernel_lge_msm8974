<<<<<<< HEAD
/*
 * i2c-smbus.h - SMBus extensions to the I2C protocol
 *
 * Copyright (C) 2010 Jean Delvare <khali@linux-fr.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * i2c-smbus.h - SMBus extensions to the I2C protocol
 *
 * Copyright (C) 2010-2019 Jean Delvare <jdelvare@suse.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _LINUX_I2C_SMBUS_H
#define _LINUX_I2C_SMBUS_H

#include <linux/i2c.h>
<<<<<<< HEAD
=======
#include <linux/spinlock.h>
#include <linux/workqueue.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


/**
 * i2c_smbus_alert_setup - platform data for the smbus_alert i2c client
<<<<<<< HEAD
 * @alert_edge_triggered: whether the alert interrupt is edge (1) or level (0)
 *		triggered
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @irq: IRQ number, if the smbus_alert driver should take care of interrupt
 *		handling
 *
 * If irq is not specified, the smbus_alert driver doesn't take care of
 * interrupt handling. In that case it is up to the I2C bus driver to either
 * handle the interrupts or to poll for alerts.
<<<<<<< HEAD
 *
 * If irq is specified then it it crucial that alert_edge_triggered is
 * properly set.
 */
struct i2c_smbus_alert_setup {
	unsigned int		alert_edge_triggered:1;
	int			irq;
};

struct i2c_client *i2c_setup_smbus_alert(struct i2c_adapter *adapter,
					 struct i2c_smbus_alert_setup *setup);
int i2c_handle_smbus_alert(struct i2c_client *ara);

=======
 */
struct i2c_smbus_alert_setup {
	int			irq;
};

struct i2c_client *i2c_new_smbus_alert_device(struct i2c_adapter *adapter,
					      struct i2c_smbus_alert_setup *setup);
int i2c_handle_smbus_alert(struct i2c_client *ara);

#if IS_ENABLED(CONFIG_I2C_SMBUS) && IS_ENABLED(CONFIG_I2C_SLAVE)
struct i2c_client *i2c_new_slave_host_notify_device(struct i2c_adapter *adapter);
void i2c_free_slave_host_notify_device(struct i2c_client *client);
#else
static inline struct i2c_client *i2c_new_slave_host_notify_device(struct i2c_adapter *adapter)
{
	return ERR_PTR(-ENOSYS);
}
static inline void i2c_free_slave_host_notify_device(struct i2c_client *client)
{
}
#endif

#if IS_ENABLED(CONFIG_I2C_SMBUS) && IS_ENABLED(CONFIG_DMI)
void i2c_register_spd(struct i2c_adapter *adap);
#else
static inline void i2c_register_spd(struct i2c_adapter *adap) { }
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_I2C_SMBUS_H */
