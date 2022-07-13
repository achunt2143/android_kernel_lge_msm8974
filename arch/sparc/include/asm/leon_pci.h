<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * asm/leon_pci.h
 *
 * Copyright (C) 2011 Aeroflex Gaisler AB, Daniel Hellstrom
 */

#ifndef _ASM_LEON_PCI_H_
#define _ASM_LEON_PCI_H_

/* PCI related definitions */
struct leon_pci_info {
	struct pci_ops *ops;
	struct resource	io_space;
	struct resource	mem_space;
<<<<<<< HEAD
	int (*map_irq)(const struct pci_dev *dev, u8 slot, u8 pin);
};

extern void leon_pci_init(struct platform_device *ofdev,
				struct leon_pci_info *info);
=======
	struct resource	busn;
	int (*map_irq)(const struct pci_dev *dev, u8 slot, u8 pin);
};

void leon_pci_init(struct platform_device *ofdev,
		   struct leon_pci_info *info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_LEON_PCI_H_ */
