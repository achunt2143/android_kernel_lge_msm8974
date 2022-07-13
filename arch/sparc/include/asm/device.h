<<<<<<< HEAD
/*
 * Arch specific extensions to struct device
 *
 * This file is released under the GPLv2
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Arch specific extensions to struct device
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _ASM_SPARC_DEVICE_H
#define _ASM_SPARC_DEVICE_H

#include <asm/openprom.h>

struct device_node;
struct platform_device;

struct dev_archdata {
	void			*iommu;
	void			*stc;
	void			*host_controller;
	struct platform_device	*op;
	int			numa_node;
};

<<<<<<< HEAD
extern void of_propagate_archdata(struct platform_device *bus);
=======
void of_propagate_archdata(struct platform_device *bus);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct pdev_archdata {
	struct resource		resource[PROMREG_MAX];
	unsigned int		irqs[PROMINTR_MAX];
	int			num_irqs;
};

#endif /* _ASM_SPARC_DEVICE_H */
