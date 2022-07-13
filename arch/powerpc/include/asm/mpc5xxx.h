<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2007 Freescale Semiconductor, Inc. All rights reserved.
 *
 * Author: John Rigby, <jrigby@freescale.com>, Friday Apr 13 2007
 *
 * Description:
 * MPC5xxx Prototypes and definitions
<<<<<<< HEAD
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ASM_POWERPC_MPC5xxx_H__
#define __ASM_POWERPC_MPC5xxx_H__

<<<<<<< HEAD
extern unsigned long mpc5xxx_get_bus_frequency(struct device_node *node);
=======
#include <linux/property.h>

unsigned long mpc5xxx_fwnode_get_bus_frequency(struct fwnode_handle *fwnode);

static inline unsigned long mpc5xxx_get_bus_frequency(struct device *dev)
{
	return mpc5xxx_fwnode_get_bus_frequency(dev_fwnode(dev));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASM_POWERPC_MPC5xxx_H__ */

