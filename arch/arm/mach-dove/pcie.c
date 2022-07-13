<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/mach-dove/pcie.c
 *
 * PCIe functions for Marvell Dove 88AP510 SoC
<<<<<<< HEAD
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/pci.h>
<<<<<<< HEAD
=======
#include <linux/clk.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <video/vga.h>
#include <asm/mach/pci.h>
#include <asm/mach/arch.h>
#include <asm/setup.h>
#include <asm/delay.h>
#include <plat/pcie.h>
<<<<<<< HEAD
#include <mach/irqs.h>
#include <mach/bridge-regs.h>
#include <plat/addr-map.h>
=======
#include <plat/addr-map.h>
#include "irqs.h"
#include "bridge-regs.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "common.h"

struct pcie_port {
	u8			index;
	u8			root_bus_nr;
	void __iomem		*base;
	spinlock_t		conf_lock;
<<<<<<< HEAD
	char			io_space_name[16];
	char			mem_space_name[16];
	struct resource		res[2];
=======
	char			mem_space_name[16];
	struct resource		res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct pcie_port pcie_port[2];
static int num_pcie_ports;


static int __init dove_pcie_setup(int nr, struct pci_sys_data *sys)
{
	struct pcie_port *pp;
<<<<<<< HEAD
=======
	struct resource realio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (nr >= num_pcie_ports)
		return 0;

	pp = &pcie_port[nr];
<<<<<<< HEAD
=======
	sys->private_data = pp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pp->root_bus_nr = sys->busnr;

	/*
	 * Generic PCIe unit setup.
	 */
	orion_pcie_set_local_bus_nr(pp->base, sys->busnr);

	orion_pcie_setup(pp->base);

<<<<<<< HEAD
	/*
	 * IORESOURCE_IO
	 */
	snprintf(pp->io_space_name, sizeof(pp->io_space_name),
		 "PCIe %d I/O", pp->index);
	pp->io_space_name[sizeof(pp->io_space_name) - 1] = 0;
	pp->res[0].name = pp->io_space_name;
	if (pp->index == 0) {
		pp->res[0].start = DOVE_PCIE0_IO_PHYS_BASE;
		pp->res[0].end = pp->res[0].start + DOVE_PCIE0_IO_SIZE - 1;
	} else {
		pp->res[0].start = DOVE_PCIE1_IO_PHYS_BASE;
		pp->res[0].end = pp->res[0].start + DOVE_PCIE1_IO_SIZE - 1;
	}
	pp->res[0].flags = IORESOURCE_IO;
	if (request_resource(&ioport_resource, &pp->res[0]))
		panic("Request PCIe IO resource failed\n");
	pci_add_resource_offset(&sys->resources, &pp->res[0], sys->io_offset);
=======
	realio.start = sys->busnr * SZ_64K;
	realio.end = realio.start + SZ_64K - 1;
	pci_remap_iospace(&realio, pp->index == 0 ? DOVE_PCIE0_IO_PHYS_BASE :
						    DOVE_PCIE1_IO_PHYS_BASE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * IORESOURCE_MEM
	 */
	snprintf(pp->mem_space_name, sizeof(pp->mem_space_name),
		 "PCIe %d MEM", pp->index);
	pp->mem_space_name[sizeof(pp->mem_space_name) - 1] = 0;
<<<<<<< HEAD
	pp->res[1].name = pp->mem_space_name;
	if (pp->index == 0) {
		pp->res[1].start = DOVE_PCIE0_MEM_PHYS_BASE;
		pp->res[1].end = pp->res[1].start + DOVE_PCIE0_MEM_SIZE - 1;
	} else {
		pp->res[1].start = DOVE_PCIE1_MEM_PHYS_BASE;
		pp->res[1].end = pp->res[1].start + DOVE_PCIE1_MEM_SIZE - 1;
	}
	pp->res[1].flags = IORESOURCE_MEM;
	if (request_resource(&iomem_resource, &pp->res[1]))
		panic("Request PCIe Memory resource failed\n");
	pci_add_resource_offset(&sys->resources, &pp->res[1], sys->mem_offset);
=======
	pp->res.name = pp->mem_space_name;
	if (pp->index == 0) {
		pp->res.start = DOVE_PCIE0_MEM_PHYS_BASE;
		pp->res.end = pp->res.start + DOVE_PCIE0_MEM_SIZE - 1;
	} else {
		pp->res.start = DOVE_PCIE1_MEM_PHYS_BASE;
		pp->res.end = pp->res.start + DOVE_PCIE1_MEM_SIZE - 1;
	}
	pp->res.flags = IORESOURCE_MEM;
	if (request_resource(&iomem_resource, &pp->res))
		panic("Request PCIe Memory resource failed\n");
	pci_add_resource_offset(&sys->resources, &pp->res, sys->mem_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}

<<<<<<< HEAD
static struct pcie_port *bus_to_port(int bus)
{
	int i;

	for (i = num_pcie_ports - 1; i >= 0; i--) {
		int rbus = pcie_port[i].root_bus_nr;
		if (rbus != -1 && rbus <= bus)
			break;
	}

	return i >= 0 ? pcie_port + i : NULL;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int pcie_valid_config(struct pcie_port *pp, int bus, int dev)
{
	/*
	 * Don't go out when trying to access nonexisting devices
	 * on the local bus.
	 */
	if (bus == pp->root_bus_nr && dev > 1)
		return 0;

	return 1;
}

static int pcie_rd_conf(struct pci_bus *bus, u32 devfn, int where,
			int size, u32 *val)
{
<<<<<<< HEAD
	struct pcie_port *pp = bus_to_port(bus->number);
=======
	struct pci_sys_data *sys = bus->sysdata;
	struct pcie_port *pp = sys->private_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	int ret;

	if (pcie_valid_config(pp, bus->number, PCI_SLOT(devfn)) == 0) {
		*val = 0xffffffff;
		return PCIBIOS_DEVICE_NOT_FOUND;
	}

	spin_lock_irqsave(&pp->conf_lock, flags);
	ret = orion_pcie_rd_conf(pp->base, bus, devfn, where, size, val);
	spin_unlock_irqrestore(&pp->conf_lock, flags);

	return ret;
}

static int pcie_wr_conf(struct pci_bus *bus, u32 devfn,
			int where, int size, u32 val)
{
<<<<<<< HEAD
	struct pcie_port *pp = bus_to_port(bus->number);
=======
	struct pci_sys_data *sys = bus->sysdata;
	struct pcie_port *pp = sys->private_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	int ret;

	if (pcie_valid_config(pp, bus->number, PCI_SLOT(devfn)) == 0)
		return PCIBIOS_DEVICE_NOT_FOUND;

	spin_lock_irqsave(&pp->conf_lock, flags);
	ret = orion_pcie_wr_conf(pp->base, bus, devfn, where, size, val);
	spin_unlock_irqrestore(&pp->conf_lock, flags);

	return ret;
}

static struct pci_ops pcie_ops = {
	.read = pcie_rd_conf,
	.write = pcie_wr_conf,
};

<<<<<<< HEAD
static void __devinit rc_pci_fixup(struct pci_dev *dev)
{
	/*
	 * Prevent enumeration of root complex.
	 */
	if (dev->bus->parent == NULL && dev->devfn == 0) {
		int i;

		for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) {
			dev->resource[i].start = 0;
			dev->resource[i].end   = 0;
			dev->resource[i].flags = 0;
=======
/*
 * The root complex has a hardwired class of PCI_CLASS_MEMORY_OTHER, when it
 * is operating as a root complex this needs to be switched to
 * PCI_CLASS_BRIDGE_HOST or Linux will errantly try to process the BAR's on
 * the device. Decoding setup is handled by the orion code.
 */
static void rc_pci_fixup(struct pci_dev *dev)
{
	if (dev->bus->parent == NULL && dev->devfn == 0) {
		struct resource *r;

		dev->class &= 0xff;
		dev->class |= PCI_CLASS_BRIDGE_HOST << 8;
		pci_dev_for_each_resource(dev, r) {
			r->start = 0;
			r->end   = 0;
			r->flags = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}
DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_MARVELL, PCI_ANY_ID, rc_pci_fixup);

<<<<<<< HEAD
static struct pci_bus __init *
dove_pcie_scan_bus(int nr, struct pci_sys_data *sys)
{
	struct pci_bus *bus;

	if (nr < num_pcie_ports) {
		bus = pci_scan_root_bus(NULL, sys->busnr, &pcie_ops, sys,
					&sys->resources);
	} else {
		bus = NULL;
		BUG();
	}

	return bus;
=======
static int __init
dove_pcie_scan_bus(int nr, struct pci_host_bridge *bridge)
{
	struct pci_sys_data *sys = pci_host_bridge_priv(bridge);

	if (nr >= num_pcie_ports) {
		BUG();
		return -EINVAL;
	}

	list_splice_init(&sys->resources, &bridge->windows);
	bridge->dev.parent = NULL;
	bridge->sysdata = sys;
	bridge->busnr = sys->busnr;
	bridge->ops = &pcie_ops;

	return pci_scan_root_bus_bridge(bridge);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init dove_pcie_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
<<<<<<< HEAD
	struct pcie_port *pp = bus_to_port(dev->bus->number);
=======
	struct pci_sys_data *sys = dev->sysdata;
	struct pcie_port *pp = sys->private_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return pp->index ? IRQ_DOVE_PCIE1 : IRQ_DOVE_PCIE0;
}

static struct hw_pci dove_pci __initdata = {
	.nr_controllers	= 2,
<<<<<<< HEAD
	.swizzle	= pci_std_swizzle,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.setup		= dove_pcie_setup,
	.scan		= dove_pcie_scan_bus,
	.map_irq	= dove_pcie_map_irq,
};

<<<<<<< HEAD
static void __init add_pcie_port(int index, unsigned long base)
{
	printk(KERN_INFO "Dove PCIe port %d: ", index);

	if (orion_pcie_link_up((void __iomem *)base)) {
		struct pcie_port *pp = &pcie_port[num_pcie_ports++];
=======
static void __init add_pcie_port(int index, void __iomem *base)
{
	printk(KERN_INFO "Dove PCIe port %d: ", index);

	if (orion_pcie_link_up(base)) {
		struct pcie_port *pp = &pcie_port[num_pcie_ports++];
		struct clk *clk = clk_get_sys("pcie", (index ? "1" : "0"));

		if (!IS_ERR(clk))
			clk_prepare_enable(clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		printk(KERN_INFO "link up\n");

		pp->index = index;
		pp->root_bus_nr = -1;
<<<<<<< HEAD
		pp->base = (void __iomem *)base;
		spin_lock_init(&pp->conf_lock);
		memset(pp->res, 0, sizeof(pp->res));
=======
		pp->base = base;
		spin_lock_init(&pp->conf_lock);
		memset(&pp->res, 0, sizeof(pp->res));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		printk(KERN_INFO "link down, ignoring\n");
	}
}

void __init dove_pcie_init(int init_port0, int init_port1)
{
	vga_base = DOVE_PCIE0_MEM_PHYS_BASE;

	if (init_port0)
		add_pcie_port(0, DOVE_PCIE0_VIRT_BASE);

	if (init_port1)
		add_pcie_port(1, DOVE_PCIE1_VIRT_BASE);

	pci_common_init(&dove_pci);
}
