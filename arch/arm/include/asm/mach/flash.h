<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  arch/arm/include/asm/mach/flash.h
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef ASMARM_MACH_FLASH_H
#define ASMARM_MACH_FLASH_H

struct mtd_partition;
struct mtd_info;

<<<<<<< HEAD
enum sw_version {
	VERSION_1 = 0,
	VERSION_2,
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * map_name:	the map probe function name
 * name:	flash device name (eg, as used with mtdparts=)
 * width:	width of mapped device
<<<<<<< HEAD
 * interleave:  interleave mode feature support
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * init:	method called at driver/device initialisation
 * exit:	method called at driver/device removal
 * set_vpp:	method called to enable or disable VPP
 * mmcontrol:	method called to enable or disable Sync. Burst Read in OneNAND
 * parts:	optional array of mtd_partitions for static partitioning
<<<<<<< HEAD
 * nr_parts:	number of mtd_partitions for static partitoning
 * version:	software register interface version
=======
 * nr_parts:	number of mtd_partitions for static partitioning
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct flash_platform_data {
	const char	*map_name;
	const char	*name;
	unsigned int	width;
<<<<<<< HEAD
	unsigned int    interleave;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int		(*init)(void);
	void		(*exit)(void);
	void		(*set_vpp)(int on);
	void		(*mmcontrol)(struct mtd_info *mtd, int sync_read);
	struct mtd_partition *parts;
	unsigned int	nr_parts;
<<<<<<< HEAD
	enum sw_version	version;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif
