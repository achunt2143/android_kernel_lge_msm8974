<<<<<<< HEAD
/*
 * Arch specific extensions to struct device
 *
 * This file is released under the GPLv2
=======
/* SPDX-License-Identifier: GPL-2.0
 *
 * Arch specific extensions to struct device
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __ASM_SH_DEVICE_H
#define __ASM_SH_DEVICE_H

#include <asm-generic/device.h>

struct platform_device;
/* allocate contiguous memory chunk and fill in struct resource */
int platform_resource_setup_memory(struct platform_device *pdev,
				   char *name, unsigned long memsize);

void plat_early_device_setup(void);

#endif /* __ASM_SH_DEVICE_H */
