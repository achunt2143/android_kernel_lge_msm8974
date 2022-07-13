<<<<<<< HEAD
/*
 * Copyright (c) 2002-3 Patrick Mochel
 * Copyright (c) 2002-3 Open Source Development Labs
 *
 * This file is released under the GPLv2
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2002-3 Patrick Mochel
 * Copyright (c) 2002-3 Open Source Development Labs
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/device.h>
#include <linux/init.h>
#include <linux/memory.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
#include <linux/backing-dev.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "base.h"

/**
 * driver_init - initialize driver model.
 *
 * Call the driver model init functions to initialize their
 * subsystems. Called early from init/main.c.
 */
void __init driver_init(void)
{
	/* These are the core pieces */
<<<<<<< HEAD
=======
	bdi_init(&noop_backing_dev_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	devtmpfs_init();
	devices_init();
	buses_init();
	classes_init();
	firmware_init();
	hypervisor_init();

	/* These are also core pieces, but must come after the
	 * core core pieces.
	 */
<<<<<<< HEAD
	platform_bus_init();
	cpu_dev_init();
	memory_dev_init();
=======
	of_core_init();
	platform_bus_init();
	auxiliary_bus_init();
	memory_dev_init();
	node_dev_init();
	cpu_dev_init();
	container_dev_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
