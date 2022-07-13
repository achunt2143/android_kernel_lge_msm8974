<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  lpc_sch.c - LPC interface for Intel Poulsbo SCH
 *
 *  LPC bridge function of the Intel SCH contains many other
 *  functional units, such as Interrupt controllers, Timers,
 *  Power Management, System Management, GPIO, RTC, and LPC
 *  Configuration Registers.
 *
 *  Copyright (c) 2010 CompuLab Ltd
<<<<<<< HEAD
 *  Author: Denis Turischev <denis@compulab.co.il>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License 2 as published
 *  by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
=======
 *  Copyright (c) 2014 Intel Corp.
 *  Author: Denis Turischev <denis@compulab.co.il>
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/acpi.h>
#include <linux/pci.h>
#include <linux/mfd/core.h>

#define SMBASE		0x40
#define SMBUS_IO_SIZE	64

<<<<<<< HEAD
#define GPIOBASE	0x44
#define GPIO_IO_SIZE	64
=======
#define GPIO_BASE	0x44
#define GPIO_IO_SIZE	64
#define GPIO_IO_SIZE_CENTERTON	128
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define WDTBASE		0x84
#define WDT_IO_SIZE	64

<<<<<<< HEAD
static struct resource smbus_sch_resource = {
		.flags = IORESOURCE_IO,
};


static struct resource gpio_sch_resource = {
		.flags = IORESOURCE_IO,
};

static struct mfd_cell lpc_sch_cells[] = {
	{
		.name = "isch_smbus",
		.num_resources = 1,
		.resources = &smbus_sch_resource,
	},
	{
		.name = "sch_gpio",
		.num_resources = 1,
		.resources = &gpio_sch_resource,
	},
};

static struct resource wdt_sch_resource = {
		.flags = IORESOURCE_IO,
};

static struct mfd_cell tunnelcreek_cells[] = {
	{
		.name = "tunnelcreek_wdt",
		.num_resources = 1,
		.resources = &wdt_sch_resource,
	},
};

static DEFINE_PCI_DEVICE_TABLE(lpc_sch_ids) = {
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_SCH_LPC) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_ITC_LPC) },
=======
enum sch_chipsets {
	LPC_SCH = 0,		/* Intel Poulsbo SCH */
	LPC_ITC,		/* Intel Tunnel Creek */
	LPC_CENTERTON,		/* Intel Centerton */
	LPC_QUARK_X1000,	/* Intel Quark X1000 */
};

struct lpc_sch_info {
	unsigned int io_size_smbus;
	unsigned int io_size_gpio;
	unsigned int io_size_wdt;
};

static struct lpc_sch_info sch_chipset_info[] = {
	[LPC_SCH] = {
		.io_size_smbus = SMBUS_IO_SIZE,
		.io_size_gpio = GPIO_IO_SIZE,
	},
	[LPC_ITC] = {
		.io_size_smbus = SMBUS_IO_SIZE,
		.io_size_gpio = GPIO_IO_SIZE,
		.io_size_wdt = WDT_IO_SIZE,
	},
	[LPC_CENTERTON] = {
		.io_size_smbus = SMBUS_IO_SIZE,
		.io_size_gpio = GPIO_IO_SIZE_CENTERTON,
		.io_size_wdt = WDT_IO_SIZE,
	},
	[LPC_QUARK_X1000] = {
		.io_size_gpio = GPIO_IO_SIZE,
		.io_size_wdt = WDT_IO_SIZE,
	},
};

static const struct pci_device_id lpc_sch_ids[] = {
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_SCH_LPC), LPC_SCH },
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_ITC_LPC), LPC_ITC },
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_CENTERTON_ILB), LPC_CENTERTON },
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_QUARK_X1000_ILB), LPC_QUARK_X1000 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, lpc_sch_ids);

<<<<<<< HEAD
static int __devinit lpc_sch_probe(struct pci_dev *dev,
				const struct pci_device_id *id)
{
	unsigned int base_addr_cfg;
	unsigned short base_addr;
	int i;
	int ret;

	pci_read_config_dword(dev, SMBASE, &base_addr_cfg);
	if (!(base_addr_cfg & (1 << 31))) {
		dev_err(&dev->dev, "Decode of the SMBus I/O range disabled\n");
		return -ENODEV;
	}
	base_addr = (unsigned short)base_addr_cfg;
	if (base_addr == 0) {
		dev_err(&dev->dev, "I/O space for SMBus uninitialized\n");
		return -ENODEV;
	}

	smbus_sch_resource.start = base_addr;
	smbus_sch_resource.end = base_addr + SMBUS_IO_SIZE - 1;

	pci_read_config_dword(dev, GPIOBASE, &base_addr_cfg);
	if (!(base_addr_cfg & (1 << 31))) {
		dev_err(&dev->dev, "Decode of the GPIO I/O range disabled\n");
		return -ENODEV;
	}
	base_addr = (unsigned short)base_addr_cfg;
	if (base_addr == 0) {
		dev_err(&dev->dev, "I/O space for GPIO uninitialized\n");
		return -ENODEV;
	}

	gpio_sch_resource.start = base_addr;
	gpio_sch_resource.end = base_addr + GPIO_IO_SIZE - 1;

	for (i=0; i < ARRAY_SIZE(lpc_sch_cells); i++)
		lpc_sch_cells[i].id = id->device;

	ret = mfd_add_devices(&dev->dev, 0,
			lpc_sch_cells, ARRAY_SIZE(lpc_sch_cells), NULL, 0);
	if (ret)
		goto out_dev;

	if (id->device == PCI_DEVICE_ID_INTEL_ITC_LPC) {
		pci_read_config_dword(dev, WDTBASE, &base_addr_cfg);
		if (!(base_addr_cfg & (1 << 31))) {
			dev_err(&dev->dev, "Decode of the WDT I/O range disabled\n");
			ret = -ENODEV;
			goto out_dev;
		}
		base_addr = (unsigned short)base_addr_cfg;
		if (base_addr == 0) {
			dev_err(&dev->dev, "I/O space for WDT uninitialized\n");
			ret = -ENODEV;
			goto out_dev;
		}

		wdt_sch_resource.start = base_addr;
		wdt_sch_resource.end = base_addr + WDT_IO_SIZE - 1;

		for (i = 0; i < ARRAY_SIZE(tunnelcreek_cells); i++)
			tunnelcreek_cells[i].id = id->device;

		ret = mfd_add_devices(&dev->dev, 0, tunnelcreek_cells,
			ARRAY_SIZE(tunnelcreek_cells), NULL, 0);
	}

	return ret;
out_dev:
	mfd_remove_devices(&dev->dev);
	return ret;
}

static void __devexit lpc_sch_remove(struct pci_dev *dev)
=======
#define LPC_NO_RESOURCE		1
#define LPC_SKIP_RESOURCE	2

static int lpc_sch_get_io(struct pci_dev *pdev, int where, const char *name,
			  struct resource *res, int size)
{
	unsigned int base_addr_cfg;
	unsigned short base_addr;

	if (size == 0)
		return LPC_NO_RESOURCE;

	pci_read_config_dword(pdev, where, &base_addr_cfg);
	base_addr = 0;
	if (!(base_addr_cfg & (1 << 31)))
		dev_warn(&pdev->dev, "Decode of the %s I/O range disabled\n",
			 name);
	else
		base_addr = (unsigned short)base_addr_cfg;

	if (base_addr == 0) {
		dev_warn(&pdev->dev, "I/O space for %s uninitialized\n", name);
		return LPC_SKIP_RESOURCE;
	}

	res->start = base_addr;
	res->end = base_addr + size - 1;
	res->flags = IORESOURCE_IO;

	return 0;
}

static int lpc_sch_populate_cell(struct pci_dev *pdev, int where,
				 const char *name, int size, int id,
				 struct mfd_cell *cell)
{
	struct resource *res;
	int ret;

	res = devm_kzalloc(&pdev->dev, sizeof(*res), GFP_KERNEL);
	if (!res)
		return -ENOMEM;

	ret = lpc_sch_get_io(pdev, where, name, res, size);
	if (ret)
		return ret;

	memset(cell, 0, sizeof(*cell));

	cell->name = name;
	cell->resources = res;
	cell->num_resources = 1;
	cell->ignore_resource_conflicts = true;
	cell->id = id;

	return 0;
}

static int lpc_sch_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	struct mfd_cell lpc_sch_cells[3];
	struct lpc_sch_info *info = &sch_chipset_info[id->driver_data];
	unsigned int cells = 0;
	int ret;

	ret = lpc_sch_populate_cell(dev, SMBASE, "isch_smbus",
				    info->io_size_smbus,
				    id->device, &lpc_sch_cells[cells]);
	if (ret < 0)
		return ret;
	if (ret == 0)
		cells++;

	ret = lpc_sch_populate_cell(dev, GPIO_BASE, "sch_gpio",
				    info->io_size_gpio,
				    id->device, &lpc_sch_cells[cells]);
	if (ret < 0)
		return ret;
	if (ret == 0)
		cells++;

	ret = lpc_sch_populate_cell(dev, WDTBASE, "ie6xx_wdt",
				    info->io_size_wdt,
				    id->device, &lpc_sch_cells[cells]);
	if (ret < 0)
		return ret;
	if (ret == 0)
		cells++;

	if (cells == 0) {
		dev_err(&dev->dev, "All decode registers disabled.\n");
		return -ENODEV;
	}

	return mfd_add_devices(&dev->dev, 0, lpc_sch_cells, cells, NULL, 0, NULL);
}

static void lpc_sch_remove(struct pci_dev *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	mfd_remove_devices(&dev->dev);
}

static struct pci_driver lpc_sch_driver = {
	.name		= "lpc_sch",
	.id_table	= lpc_sch_ids,
	.probe		= lpc_sch_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(lpc_sch_remove),
};

static int __init lpc_sch_init(void)
{
	return pci_register_driver(&lpc_sch_driver);
}

static void __exit lpc_sch_exit(void)
{
	pci_unregister_driver(&lpc_sch_driver);
}

module_init(lpc_sch_init);
module_exit(lpc_sch_exit);
=======
	.remove		= lpc_sch_remove,
};

module_pci_driver(lpc_sch_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Denis Turischev <denis@compulab.co.il>");
MODULE_DESCRIPTION("LPC interface for Intel Poulsbo SCH");
MODULE_LICENSE("GPL");
