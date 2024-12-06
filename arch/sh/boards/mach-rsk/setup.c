<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Renesas Technology Europe RSK+ Support.
 *
 * Copyright (C) 2008 Paul Mundt
 * Copyright (C) 2008 Peter Griffin <pgriffin@mpc-data.co.uk>
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/init.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>
#include <linux/mtd/map.h>
<<<<<<< HEAD
#include <asm/machvec.h>
#include <asm/io.h>

static const char *part_probes[] = { "cmdlinepart", NULL };
=======
#include <linux/regulator/fixed.h>
#include <linux/regulator/machine.h>
#include <asm/machvec.h>
#include <asm/io.h>

/* Dummy supplies, where voltage doesn't matter */
static struct regulator_consumer_supply dummy_supplies[] = {
	REGULATOR_SUPPLY("vddvario", "smsc911x"),
	REGULATOR_SUPPLY("vdd33a", "smsc911x"),
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct mtd_partition rsk_partitions[] = {
	{
		.name		= "Bootloader",
		.offset		= 0x00000000,
		.size		= 0x00040000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "Kernel",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= 0x001c0000,
	}, {
		.name		= "Flash_FS",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= MTDPART_SIZ_FULL,
	}
};

static struct physmap_flash_data flash_data = {
	.parts			= rsk_partitions,
	.nr_parts		= ARRAY_SIZE(rsk_partitions),
	.width			= 2,
<<<<<<< HEAD
	.part_probe_types	= part_probes,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct resource flash_resource = {
	.start		= 0x20000000,
	.end		= 0x20400000,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device flash_device = {
	.name		= "physmap-flash",
	.id		= -1,
	.resource	= &flash_resource,
	.num_resources	= 1,
	.dev		= {
		.platform_data = &flash_data,
	},
};

static struct platform_device *rsk_devices[] __initdata = {
	&flash_device,
};

static int __init rsk_devices_setup(void)
{
<<<<<<< HEAD
=======
	regulator_register_fixed(0, dummy_supplies, ARRAY_SIZE(dummy_supplies));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return platform_add_devices(rsk_devices,
				    ARRAY_SIZE(rsk_devices));
}
device_initcall(rsk_devices_setup);

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_rsk __initmv = {
	.mv_name        = "RSK+",
};
