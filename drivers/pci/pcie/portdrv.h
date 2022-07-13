<<<<<<< HEAD
/*
 * File:	portdrv.h
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Purpose:	PCI Express Port Bus Driver's Internal Data Structures
 *
 * Copyright (C) 2004 Intel
 * Copyright (C) Tom Long Nguyen (tom.l.nguyen@intel.com)
 */

#ifndef _PORTDRV_H_
#define _PORTDRV_H_

#include <linux/compiler.h>

<<<<<<< HEAD
#define PCIE_PORT_DEVICE_MAXSERVICES   4
/*
 * According to the PCI Express Base Specification 2.0, the indices of
 * the MSI-X table entires used by port services must not exceed 31
 */
#define PCIE_PORT_MAX_MSIX_ENTRIES	32

#define get_descriptor_id(type, service) (((type - 4) << 4) | service)

extern struct bus_type pcie_port_bus_type;
extern int pcie_port_device_register(struct pci_dev *dev);
#ifdef CONFIG_PM
extern int pcie_port_device_suspend(struct device *dev);
extern int pcie_port_device_resume(struct device *dev);
#endif
extern void pcie_port_device_remove(struct pci_dev *dev);
extern int __must_check pcie_port_bus_register(void);
extern void pcie_port_bus_unregister(void);

struct pci_dev;

extern void pcie_clear_root_pme_status(struct pci_dev *dev);

#ifdef CONFIG_HOTPLUG_PCI_PCIE
extern bool pciehp_msi_disabled;

static inline bool pciehp_no_msi(void)
{
	return pciehp_msi_disabled;
}

#else  /* !CONFIG_HOTPLUG_PCI_PCIE */
static inline bool pciehp_no_msi(void) { return false; }
#endif /* !CONFIG_HOTPLUG_PCI_PCIE */
=======
/* Service Type */
#define PCIE_PORT_SERVICE_PME_SHIFT	0	/* Power Management Event */
#define PCIE_PORT_SERVICE_PME		(1 << PCIE_PORT_SERVICE_PME_SHIFT)
#define PCIE_PORT_SERVICE_AER_SHIFT	1	/* Advanced Error Reporting */
#define PCIE_PORT_SERVICE_AER		(1 << PCIE_PORT_SERVICE_AER_SHIFT)
#define PCIE_PORT_SERVICE_HP_SHIFT	2	/* Native Hotplug */
#define PCIE_PORT_SERVICE_HP		(1 << PCIE_PORT_SERVICE_HP_SHIFT)
#define PCIE_PORT_SERVICE_DPC_SHIFT	3	/* Downstream Port Containment */
#define PCIE_PORT_SERVICE_DPC		(1 << PCIE_PORT_SERVICE_DPC_SHIFT)
#define PCIE_PORT_SERVICE_BWNOTIF_SHIFT	4	/* Bandwidth notification */
#define PCIE_PORT_SERVICE_BWNOTIF	(1 << PCIE_PORT_SERVICE_BWNOTIF_SHIFT)

#define PCIE_PORT_DEVICE_MAXSERVICES   5

extern bool pcie_ports_dpc_native;

#ifdef CONFIG_PCIEAER
int pcie_aer_init(void);
#else
static inline int pcie_aer_init(void) { return 0; }
#endif

#ifdef CONFIG_HOTPLUG_PCI_PCIE
int pcie_hp_init(void);
#else
static inline int pcie_hp_init(void) { return 0; }
#endif

#ifdef CONFIG_PCIE_PME
int pcie_pme_init(void);
#else
static inline int pcie_pme_init(void) { return 0; }
#endif

#ifdef CONFIG_PCIE_DPC
int pcie_dpc_init(void);
#else
static inline int pcie_dpc_init(void) { return 0; }
#endif

/* Port Type */
#define PCIE_ANY_PORT			(~0)

struct pcie_device {
	int		irq;	    /* Service IRQ/MSI/MSI-X Vector */
	struct pci_dev *port;	    /* Root/Upstream/Downstream Port */
	u32		service;    /* Port service this device represents */
	void		*priv_data; /* Service Private Data */
	struct device	device;     /* Generic Device Interface */
};
#define to_pcie_device(d) container_of(d, struct pcie_device, device)

static inline void set_service_data(struct pcie_device *dev, void *data)
{
	dev->priv_data = data;
}

static inline void *get_service_data(struct pcie_device *dev)
{
	return dev->priv_data;
}

struct pcie_port_service_driver {
	const char *name;
	int (*probe)(struct pcie_device *dev);
	void (*remove)(struct pcie_device *dev);
	int (*suspend)(struct pcie_device *dev);
	int (*resume_noirq)(struct pcie_device *dev);
	int (*resume)(struct pcie_device *dev);
	int (*runtime_suspend)(struct pcie_device *dev);
	int (*runtime_resume)(struct pcie_device *dev);

	int (*slot_reset)(struct pcie_device *dev);

	int port_type;  /* Type of the port this driver can handle */
	u32 service;    /* Port service this device represents */

	struct device_driver driver;
};
#define to_service_driver(d) \
	container_of(d, struct pcie_port_service_driver, driver)

int pcie_port_service_register(struct pcie_port_service_driver *new);
void pcie_port_service_unregister(struct pcie_port_service_driver *new);

extern const struct bus_type pcie_port_bus_type;

struct pci_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PCIE_PME
extern bool pcie_pme_msi_disabled;

static inline void pcie_pme_disable_msi(void)
{
	pcie_pme_msi_disabled = true;
}

static inline bool pcie_pme_no_msi(void)
{
	return pcie_pme_msi_disabled;
}

<<<<<<< HEAD
extern void pcie_pme_interrupt_enable(struct pci_dev *dev, bool enable);
=======
void pcie_pme_interrupt_enable(struct pci_dev *dev, bool enable);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else /* !CONFIG_PCIE_PME */
static inline void pcie_pme_disable_msi(void) {}
static inline bool pcie_pme_no_msi(void) { return false; }
static inline void pcie_pme_interrupt_enable(struct pci_dev *dev, bool en) {}
#endif /* !CONFIG_PCIE_PME */

<<<<<<< HEAD
#ifdef CONFIG_ACPI
extern int pcie_port_acpi_setup(struct pci_dev *port, int *mask);

static inline int pcie_port_platform_notify(struct pci_dev *port, int *mask)
{
	return pcie_port_acpi_setup(port, mask);
}
#else /* !CONFIG_ACPI */
static inline int pcie_port_platform_notify(struct pci_dev *port, int *mask)
{
	return 0;
}
#endif /* !CONFIG_ACPI */

=======
struct device *pcie_port_find_device(struct pci_dev *dev, u32 service);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _PORTDRV_H_ */
