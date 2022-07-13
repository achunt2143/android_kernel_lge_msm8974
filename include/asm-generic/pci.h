<<<<<<< HEAD
/*
 * linux/include/asm-generic/pci.h
 *
 *  Copyright (C) 2003 Russell King
 */
#ifndef _ASM_GENERIC_PCI_H
#define _ASM_GENERIC_PCI_H

static inline struct resource *
pcibios_select_root(struct pci_dev *pdev, struct resource *res)
{
	struct resource *root = NULL;

	if (res->flags & IORESOURCE_IO)
		root = &ioport_resource;
	if (res->flags & IORESOURCE_MEM)
		root = &iomem_resource;

	return root;
}

#ifndef HAVE_ARCH_PCI_GET_LEGACY_IDE_IRQ
static inline int pci_get_legacy_ide_irq(struct pci_dev *dev, int channel)
{
	return channel ? 15 : 14;
}
#endif /* HAVE_ARCH_PCI_GET_LEGACY_IDE_IRQ */

/*
 * By default, assume that no iommu is in use and that the PCI
 * space is mapped to address physical 0.
 */
#ifndef PCI_DMA_BUS_IS_PHYS
#define PCI_DMA_BUS_IS_PHYS	(1)
#endif

#endif /* _ASM_GENERIC_PCI_H */
=======
/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __ASM_GENERIC_PCI_H
#define __ASM_GENERIC_PCI_H

#ifndef PCIBIOS_MIN_IO
#define PCIBIOS_MIN_IO		0
#endif

#ifndef PCIBIOS_MIN_MEM
#define PCIBIOS_MIN_MEM		0
#endif

#ifndef pcibios_assign_all_busses
/* For bootloaders that do not initialize the PCI bus */
#define pcibios_assign_all_busses() 1
#endif

/* Enable generic resource mapping code in drivers/pci/ */
#define ARCH_GENERIC_PCI_MMAP_RESOURCE

#ifdef CONFIG_PCI_DOMAINS
static inline int pci_proc_domain(struct pci_bus *bus)
{
	/* always show the domain in /proc */
	return 1;
}
#endif /* CONFIG_PCI_DOMAINS */

#endif /* __ASM_GENERIC_PCI_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
