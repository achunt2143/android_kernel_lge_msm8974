<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * cs5535-mfd.c - core MFD driver for CS5535/CS5536 southbridges
 *
 * The CS5535 and CS5536 has an ISA bridge on the PCI bus that is
 * used for accessing GPIOs, MFGPTs, ACPI, etc.  Each subdevice has
 * an IO range that's specified in a single BAR.  The BAR order is
 * hardcoded in the CS553x specifications.
 *
 * Copyright (c) 2010  Andres Salomon <dilinger@queued.net>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/kernel.h>
#include <linux/init.h>
=======
 */

#include <linux/kernel.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mfd/core.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <asm/olpc.h>

#define DRV_NAME "cs5535-mfd"

enum cs5535_mfd_bars {
	SMB_BAR = 0,
	GPIO_BAR = 1,
	MFGPT_BAR = 2,
	PMS_BAR = 4,
	ACPI_BAR = 5,
	NR_BARS,
};

<<<<<<< HEAD
static int cs5535_mfd_res_enable(struct platform_device *pdev)
{
	struct resource *res;

	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
	if (!res) {
		dev_err(&pdev->dev, "can't fetch device resource info\n");
		return -EIO;
	}

	if (!request_region(res->start, resource_size(res), DRV_NAME)) {
		dev_err(&pdev->dev, "can't request region\n");
		return -EIO;
	}

	return 0;
}

static int cs5535_mfd_res_disable(struct platform_device *pdev)
{
	struct resource *res;
	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
	if (!res) {
		dev_err(&pdev->dev, "can't fetch device resource info\n");
		return -EIO;
	}

	release_region(res->start, resource_size(res));
	return 0;
}

static __devinitdata struct resource cs5535_mfd_resources[NR_BARS];

static __devinitdata struct mfd_cell cs5535_mfd_cells[] = {
	{
		.id = SMB_BAR,
=======
static struct resource cs5535_mfd_resources[NR_BARS];

static struct mfd_cell cs5535_mfd_cells[] = {
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.name = "cs5535-smb",
		.num_resources = 1,
		.resources = &cs5535_mfd_resources[SMB_BAR],
	},
	{
<<<<<<< HEAD
		.id = GPIO_BAR,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.name = "cs5535-gpio",
		.num_resources = 1,
		.resources = &cs5535_mfd_resources[GPIO_BAR],
	},
	{
<<<<<<< HEAD
		.id = MFGPT_BAR,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.name = "cs5535-mfgpt",
		.num_resources = 1,
		.resources = &cs5535_mfd_resources[MFGPT_BAR],
	},
	{
<<<<<<< HEAD
		.id = PMS_BAR,
		.name = "cs5535-pms",
		.num_resources = 1,
		.resources = &cs5535_mfd_resources[PMS_BAR],

		.enable = cs5535_mfd_res_enable,
		.disable = cs5535_mfd_res_disable,
	},
	{
		.id = ACPI_BAR,
		.name = "cs5535-acpi",
		.num_resources = 1,
		.resources = &cs5535_mfd_resources[ACPI_BAR],

		.enable = cs5535_mfd_res_enable,
		.disable = cs5535_mfd_res_disable,
	},
};

#ifdef CONFIG_OLPC
static void __devinit cs5535_clone_olpc_cells(void)
{
	const char *acpi_clones[] = { "olpc-xo1-pm-acpi", "olpc-xo1-sci-acpi" };

	if (!machine_is_olpc())
		return;

	mfd_clone_cell("cs5535-acpi", acpi_clones, ARRAY_SIZE(acpi_clones));
}
#else
static void cs5535_clone_olpc_cells(void) { }
#endif

static int __devinit cs5535_mfd_probe(struct pci_dev *pdev,
		const struct pci_device_id *id)
{
	int err, i;
=======
		.name = "cs5535-pms",
		.num_resources = 1,
		.resources = &cs5535_mfd_resources[PMS_BAR],
	},
};

static struct mfd_cell cs5535_olpc_mfd_cells[] = {
	{
		.name = "olpc-xo1-pm-acpi",
		.num_resources = 1,
		.resources = &cs5535_mfd_resources[ACPI_BAR],
	},
	{
		.name = "olpc-xo1-sci-acpi",
		.num_resources = 1,
		.resources = &cs5535_mfd_resources[ACPI_BAR],
	},
};

static int cs5535_mfd_probe(struct pci_dev *pdev,
		const struct pci_device_id *id)
{
	int err, bar;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = pci_enable_device(pdev);
	if (err)
		return err;

<<<<<<< HEAD
	/* fill in IO range for each cell; subdrivers handle the region */
	for (i = 0; i < ARRAY_SIZE(cs5535_mfd_cells); i++) {
		int bar = cs5535_mfd_cells[i].id;
=======
	for (bar = 0; bar < NR_BARS; bar++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct resource *r = &cs5535_mfd_resources[bar];

		r->flags = IORESOURCE_IO;
		r->start = pci_resource_start(pdev, bar);
		r->end = pci_resource_end(pdev, bar);
<<<<<<< HEAD

		/* id is used for temporarily storing BAR; unset it now */
		cs5535_mfd_cells[i].id = 0;
	}

	err = mfd_add_devices(&pdev->dev, -1, cs5535_mfd_cells,
			ARRAY_SIZE(cs5535_mfd_cells), NULL, 0);
	if (err) {
		dev_err(&pdev->dev, "MFD add devices failed: %d\n", err);
		goto err_disable;
	}
	cs5535_clone_olpc_cells();
=======
	}

	err = pci_request_region(pdev, PMS_BAR, DRV_NAME);
	if (err) {
		dev_err(&pdev->dev, "Failed to request PMS_BAR's IO region\n");
		goto err_disable;
	}

	err = mfd_add_devices(&pdev->dev, PLATFORM_DEVID_NONE, cs5535_mfd_cells,
			      ARRAY_SIZE(cs5535_mfd_cells), NULL, 0, NULL);
	if (err) {
		dev_err(&pdev->dev,
			"Failed to add CS5535 sub-devices: %d\n", err);
		goto err_release_pms;
	}

	if (machine_is_olpc()) {
		err = pci_request_region(pdev, ACPI_BAR, DRV_NAME);
		if (err) {
			dev_err(&pdev->dev,
				"Failed to request ACPI_BAR's IO region\n");
			goto err_remove_devices;
		}

		err = mfd_add_devices(&pdev->dev, PLATFORM_DEVID_NONE,
				      cs5535_olpc_mfd_cells,
				      ARRAY_SIZE(cs5535_olpc_mfd_cells),
				      NULL, 0, NULL);
		if (err) {
			dev_err(&pdev->dev,
				"Failed to add CS5535 OLPC sub-devices: %d\n",
				err);
			goto err_release_acpi;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_info(&pdev->dev, "%zu devices registered.\n",
			ARRAY_SIZE(cs5535_mfd_cells));

	return 0;

<<<<<<< HEAD
=======
err_release_acpi:
	pci_release_region(pdev, ACPI_BAR);
err_remove_devices:
	mfd_remove_devices(&pdev->dev);
err_release_pms:
	pci_release_region(pdev, PMS_BAR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_disable:
	pci_disable_device(pdev);
	return err;
}

<<<<<<< HEAD
static void __devexit cs5535_mfd_remove(struct pci_dev *pdev)
{
	mfd_remove_devices(&pdev->dev);
	pci_disable_device(pdev);
}

static DEFINE_PCI_DEVICE_TABLE(cs5535_mfd_pci_tbl) = {
=======
static void cs5535_mfd_remove(struct pci_dev *pdev)
{
	mfd_remove_devices(&pdev->dev);

	if (machine_is_olpc())
		pci_release_region(pdev, ACPI_BAR);

	pci_release_region(pdev, PMS_BAR);
	pci_disable_device(pdev);
}

static const struct pci_device_id cs5535_mfd_pci_tbl[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ PCI_DEVICE(PCI_VENDOR_ID_NS, PCI_DEVICE_ID_NS_CS5535_ISA) },
	{ PCI_DEVICE(PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_AMD_CS5536_ISA) },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, cs5535_mfd_pci_tbl);

static struct pci_driver cs5535_mfd_driver = {
	.name = DRV_NAME,
	.id_table = cs5535_mfd_pci_tbl,
	.probe = cs5535_mfd_probe,
<<<<<<< HEAD
	.remove = __devexit_p(cs5535_mfd_remove),
};

static int __init cs5535_mfd_init(void)
{
	return pci_register_driver(&cs5535_mfd_driver);
}

static void __exit cs5535_mfd_exit(void)
{
	pci_unregister_driver(&cs5535_mfd_driver);
}

module_init(cs5535_mfd_init);
module_exit(cs5535_mfd_exit);
=======
	.remove = cs5535_mfd_remove,
};

module_pci_driver(cs5535_mfd_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Andres Salomon <dilinger@queued.net>");
MODULE_DESCRIPTION("MFD driver for CS5535/CS5536 southbridge's ISA PCI device");
MODULE_LICENSE("GPL");
