<<<<<<< HEAD
/*
 * PCI interface driver for DW SPI Core
 *
 * Copyright (c) 2009, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/interrupt.h>
#include <linux/pci.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * PCI interface driver for DW SPI Core
 *
 * Copyright (c) 2009, 2014 Intel Corporation.
 */

#include <linux/pci.h>
#include <linux/pm_runtime.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/spi/spi.h>
#include <linux/module.h>

#include "spi-dw.h"

#define DRIVER_NAME "dw_spi_pci"

<<<<<<< HEAD
struct dw_spi_pci {
	struct pci_dev	*pdev;
	struct dw_spi	dws;
};

static int __devinit spi_pci_probe(struct pci_dev *pdev,
	const struct pci_device_id *ent)
{
	struct dw_spi_pci *dwpci;
=======
/* HW info for MRST Clk Control Unit, 32b reg per controller */
#define MRST_SPI_CLK_BASE	100000000	/* 100m */
#define MRST_CLK_SPI_REG	0xff11d86c
#define CLK_SPI_BDIV_OFFSET	0
#define CLK_SPI_BDIV_MASK	0x00000007
#define CLK_SPI_CDIV_OFFSET	9
#define CLK_SPI_CDIV_MASK	0x00000e00
#define CLK_SPI_DISABLE_OFFSET	8

struct dw_spi_pci_desc {
	int	(*setup)(struct dw_spi *);
	u16	num_cs;
	u16	bus_num;
	u32	max_freq;
};

static int dw_spi_pci_mid_init(struct dw_spi *dws)
{
	void __iomem *clk_reg;
	u32 clk_cdiv;

	clk_reg = ioremap(MRST_CLK_SPI_REG, 16);
	if (!clk_reg)
		return -ENOMEM;

	/* Get SPI controller operating freq info */
	clk_cdiv = readl(clk_reg + dws->bus_num * sizeof(u32));
	clk_cdiv &= CLK_SPI_CDIV_MASK;
	clk_cdiv >>= CLK_SPI_CDIV_OFFSET;
	dws->max_freq = MRST_SPI_CLK_BASE / (clk_cdiv + 1);

	iounmap(clk_reg);

	dw_spi_dma_setup_mfld(dws);

	return 0;
}

static int dw_spi_pci_generic_init(struct dw_spi *dws)
{
	dw_spi_dma_setup_generic(dws);

	return 0;
}

static struct dw_spi_pci_desc dw_spi_pci_mid_desc_1 = {
	.setup = dw_spi_pci_mid_init,
	.num_cs = 5,
	.bus_num = 0,
};

static struct dw_spi_pci_desc dw_spi_pci_mid_desc_2 = {
	.setup = dw_spi_pci_mid_init,
	.num_cs = 2,
	.bus_num = 1,
};

static struct dw_spi_pci_desc dw_spi_pci_ehl_desc = {
	.setup = dw_spi_pci_generic_init,
	.num_cs = 2,
	.bus_num = -1,
	.max_freq = 100000000,
};

static int dw_spi_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct dw_spi_pci_desc *desc = (struct dw_spi_pci_desc *)ent->driver_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct dw_spi *dws;
	int pci_bar = 0;
	int ret;

<<<<<<< HEAD
	printk(KERN_INFO "DW: found PCI SPI controller(ID: %04x:%04x)\n",
		pdev->vendor, pdev->device);

	ret = pci_enable_device(pdev);
	if (ret)
		return ret;

	dwpci = kzalloc(sizeof(struct dw_spi_pci), GFP_KERNEL);
	if (!dwpci) {
		ret = -ENOMEM;
		goto err_disable;
	}

	dwpci->pdev = pdev;
	dws = &dwpci->dws;

	/* Get basic io resource and map it */
	dws->paddr = pci_resource_start(pdev, pci_bar);
	dws->iolen = pci_resource_len(pdev, pci_bar);

	ret = pci_request_region(pdev, pci_bar, dev_name(&pdev->dev));
	if (ret)
		goto err_kfree;

	dws->regs = ioremap_nocache((unsigned long)dws->paddr,
				pci_resource_len(pdev, pci_bar));
	if (!dws->regs) {
		ret = -ENOMEM;
		goto err_release_reg;
	}

	dws->parent_dev = &pdev->dev;
	dws->bus_num = 0;
	dws->num_cs = 4;
	dws->irq = pdev->irq;

	/*
	 * Specific handling for Intel MID paltforms, like dma setup,
	 * clock rate, FIFO depth.
	 */
	if (pdev->device == 0x0800) {
		ret = dw_spi_mid_init(dws);
		if (ret)
			goto err_unmap;
	}

	ret = dw_spi_add_host(dws);
	if (ret)
		goto err_unmap;

	/* PCI hook and SPI hook use the same drv data */
	pci_set_drvdata(pdev, dwpci);
	return 0;

err_unmap:
	iounmap(dws->regs);
err_release_reg:
	pci_release_region(pdev, pci_bar);
err_kfree:
	kfree(dwpci);
err_disable:
	pci_disable_device(pdev);
	return ret;
}

static void __devexit spi_pci_remove(struct pci_dev *pdev)
{
	struct dw_spi_pci *dwpci = pci_get_drvdata(pdev);

	pci_set_drvdata(pdev, NULL);
	dw_spi_remove_host(&dwpci->dws);
	iounmap(dwpci->dws.regs);
	pci_release_region(pdev, 0);
	kfree(dwpci);
	pci_disable_device(pdev);
}

#ifdef CONFIG_PM
static int spi_suspend(struct pci_dev *pdev, pm_message_t state)
{
	struct dw_spi_pci *dwpci = pci_get_drvdata(pdev);
	int ret;

	ret = dw_spi_suspend_host(&dwpci->dws);
	if (ret)
		return ret;
	pci_save_state(pdev);
	pci_disable_device(pdev);
	pci_set_power_state(pdev, pci_choose_state(pdev, state));
	return ret;
}

static int spi_resume(struct pci_dev *pdev)
{
	struct dw_spi_pci *dwpci = pci_get_drvdata(pdev);
	int ret;

	pci_set_power_state(pdev, PCI_D0);
	pci_restore_state(pdev);
	ret = pci_enable_device(pdev);
	if (ret)
		return ret;
	return dw_spi_resume_host(&dwpci->dws);
}
#else
#define spi_suspend	NULL
#define spi_resume	NULL
#endif

static DEFINE_PCI_DEVICE_TABLE(pci_ids) = {
	/* Intel MID platform SPI controller 0 */
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x0800) },
	{},
};

static struct pci_driver dw_spi_driver = {
	.name =		DRIVER_NAME,
	.id_table =	pci_ids,
	.probe =	spi_pci_probe,
	.remove =	__devexit_p(spi_pci_remove),
	.suspend =	spi_suspend,
	.resume	=	spi_resume,
};

static int __init mrst_spi_init(void)
{
	return pci_register_driver(&dw_spi_driver);
}

static void __exit mrst_spi_exit(void)
{
	pci_unregister_driver(&dw_spi_driver);
}

module_init(mrst_spi_init);
module_exit(mrst_spi_exit);
=======
	ret = pcim_enable_device(pdev);
	if (ret)
		return ret;

	dws = devm_kzalloc(&pdev->dev, sizeof(*dws), GFP_KERNEL);
	if (!dws)
		return -ENOMEM;

	/* Get basic io resource and map it */
	dws->paddr = pci_resource_start(pdev, pci_bar);
	pci_set_master(pdev);

	ret = pcim_iomap_regions(pdev, 1 << pci_bar, pci_name(pdev));
	if (ret)
		return ret;

	ret = pci_alloc_irq_vectors(pdev, 1, 1, PCI_IRQ_ALL_TYPES);
	if (ret < 0)
		return ret;

	dws->regs = pcim_iomap_table(pdev)[pci_bar];
	dws->irq = pci_irq_vector(pdev, 0);

	/*
	 * Specific handling for platforms, like dma setup,
	 * clock rate, FIFO depth.
	 */
	if (desc) {
		dws->num_cs = desc->num_cs;
		dws->bus_num = desc->bus_num;
		dws->max_freq = desc->max_freq;

		if (desc->setup) {
			ret = desc->setup(dws);
			if (ret)
				goto err_free_irq_vectors;
		}
	} else {
		ret = -ENODEV;
		goto err_free_irq_vectors;
	}

	ret = dw_spi_add_host(&pdev->dev, dws);
	if (ret)
		goto err_free_irq_vectors;

	/* PCI hook and SPI hook use the same drv data */
	pci_set_drvdata(pdev, dws);

	dev_info(&pdev->dev, "found PCI SPI controller(ID: %04x:%04x)\n",
		pdev->vendor, pdev->device);

	pm_runtime_set_autosuspend_delay(&pdev->dev, 1000);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_put_autosuspend(&pdev->dev);
	pm_runtime_allow(&pdev->dev);

	return 0;

err_free_irq_vectors:
	pci_free_irq_vectors(pdev);
	return ret;
}

static void dw_spi_pci_remove(struct pci_dev *pdev)
{
	struct dw_spi *dws = pci_get_drvdata(pdev);

	pm_runtime_forbid(&pdev->dev);
	pm_runtime_get_noresume(&pdev->dev);

	dw_spi_remove_host(dws);
	pci_free_irq_vectors(pdev);
}

#ifdef CONFIG_PM_SLEEP
static int dw_spi_pci_suspend(struct device *dev)
{
	struct dw_spi *dws = dev_get_drvdata(dev);

	return dw_spi_suspend_host(dws);
}

static int dw_spi_pci_resume(struct device *dev)
{
	struct dw_spi *dws = dev_get_drvdata(dev);

	return dw_spi_resume_host(dws);
}
#endif

static SIMPLE_DEV_PM_OPS(dw_spi_pci_pm_ops, dw_spi_pci_suspend, dw_spi_pci_resume);

static const struct pci_device_id dw_spi_pci_ids[] = {
	/* Intel MID platform SPI controller 0 */
	/*
	 * The access to the device 8086:0801 is disabled by HW, since it's
	 * exclusively used by SCU to communicate with MSIC.
	 */
	/* Intel MID platform SPI controller 1 */
	{ PCI_VDEVICE(INTEL, 0x0800), (kernel_ulong_t)&dw_spi_pci_mid_desc_1},
	/* Intel MID platform SPI controller 2 */
	{ PCI_VDEVICE(INTEL, 0x0812), (kernel_ulong_t)&dw_spi_pci_mid_desc_2},
	/* Intel Elkhart Lake PSE SPI controllers */
	{ PCI_VDEVICE(INTEL, 0x4b84), (kernel_ulong_t)&dw_spi_pci_ehl_desc},
	{ PCI_VDEVICE(INTEL, 0x4b85), (kernel_ulong_t)&dw_spi_pci_ehl_desc},
	{ PCI_VDEVICE(INTEL, 0x4b86), (kernel_ulong_t)&dw_spi_pci_ehl_desc},
	{ PCI_VDEVICE(INTEL, 0x4b87), (kernel_ulong_t)&dw_spi_pci_ehl_desc},
	{},
};
MODULE_DEVICE_TABLE(pci, dw_spi_pci_ids);

static struct pci_driver dw_spi_pci_driver = {
	.name =		DRIVER_NAME,
	.id_table =	dw_spi_pci_ids,
	.probe =	dw_spi_pci_probe,
	.remove =	dw_spi_pci_remove,
	.driver         = {
		.pm     = &dw_spi_pci_pm_ops,
	},
};
module_pci_driver(dw_spi_pci_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Feng Tang <feng.tang@intel.com>");
MODULE_DESCRIPTION("PCI interface driver for DW SPI Core");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
=======
MODULE_IMPORT_NS(SPI_DW_CORE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
