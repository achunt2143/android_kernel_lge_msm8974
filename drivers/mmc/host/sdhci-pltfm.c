<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * sdhci-pltfm.c Support for SDHCI platform devices
 * Copyright (c) 2009 Intel Corporation
 *
 * Copyright (c) 2007, 2011 Freescale Semiconductor, Inc.
 * Copyright (c) 2009 MontaVista Software, Inc.
 *
 * Authors: Xiaobo Xie <X.Xie@freescale.com>
 *	    Anton Vorontsov <avorontsov@ru.mvista.com>
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
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/* Supports:
 * SDHCI platform devices
 *
 * Inspired by sdhci-pci.c, by Pierre Ossman
 */

#include <linux/err.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/of.h>
=======
#include <linux/property.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PPC
#include <asm/machdep.h>
#endif
#include "sdhci-pltfm.h"

<<<<<<< HEAD
static struct sdhci_ops sdhci_pltfm_ops = {
};

#ifdef CONFIG_OF
static bool sdhci_of_wp_inverted(struct device_node *np)
{
	if (of_get_property(np, "sdhci,wp-inverted", NULL))
=======
unsigned int sdhci_pltfm_clk_get_max_clock(struct sdhci_host *host)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);

	return clk_get_rate(pltfm_host->clk);
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_clk_get_max_clock);

static const struct sdhci_ops sdhci_pltfm_ops = {
	.set_clock = sdhci_set_clock,
	.set_bus_width = sdhci_set_bus_width,
	.reset = sdhci_reset,
	.set_uhs_signaling = sdhci_set_uhs_signaling,
};

static bool sdhci_wp_inverted(struct device *dev)
{
	if (device_property_present(dev, "sdhci,wp-inverted") ||
	    device_property_present(dev, "wp-inverted"))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return true;

	/* Old device trees don't have the wp-inverted property. */
#ifdef CONFIG_PPC
	return machine_is(mpc837x_rdb) || machine_is(mpc837x_mds);
#else
	return false;
#endif /* CONFIG_PPC */
}

<<<<<<< HEAD
void sdhci_get_of_property(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct sdhci_host *host = platform_get_drvdata(pdev);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	const __be32 *clk;
	int size;

	if (of_device_is_available(np)) {
		if (of_get_property(np, "sdhci,auto-cmd12", NULL))
			host->quirks |= SDHCI_QUIRK_MULTIBLOCK_READ_ACMD12;

		if (of_get_property(np, "sdhci,1-bit-only", NULL))
			host->quirks |= SDHCI_QUIRK_FORCE_1_BIT_DATA;

		if (sdhci_of_wp_inverted(np))
			host->quirks |= SDHCI_QUIRK_INVERTED_WRITE_PROTECT;

		if (of_device_is_compatible(np, "fsl,p2020-rev1-esdhc"))
			host->quirks |= SDHCI_QUIRK_BROKEN_DMA;

		if (of_device_is_compatible(np, "fsl,p2020-esdhc") ||
		    of_device_is_compatible(np, "fsl,p1010-esdhc") ||
		    of_device_is_compatible(np, "fsl,mpc8536-esdhc"))
			host->quirks |= SDHCI_QUIRK_BROKEN_TIMEOUT_VAL;

		clk = of_get_property(np, "clock-frequency", &size);
		if (clk && size == sizeof(*clk) && *clk)
			pltfm_host->clock = be32_to_cpup(clk);
	}
}
#else
void sdhci_get_of_property(struct platform_device *pdev) {}
#endif /* CONFIG_OF */
EXPORT_SYMBOL_GPL(sdhci_get_of_property);

struct sdhci_host *sdhci_pltfm_init(struct platform_device *pdev,
				    struct sdhci_pltfm_data *pdata)
{
	struct sdhci_host *host;
	struct sdhci_pltfm_host *pltfm_host;
	struct device_node *np = pdev->dev.of_node;
	struct resource *iomem;
	int ret;

	iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!iomem) {
		ret = -ENOMEM;
		goto err;
	}

	if (resource_size(iomem) < 0x100)
		dev_err(&pdev->dev, "Invalid iomem size!\n");

	/* Some PCI-based MFD need the parent here */
	if (pdev->dev.parent != &platform_bus && !np)
		host = sdhci_alloc_host(pdev->dev.parent, sizeof(*pltfm_host));
	else
		host = sdhci_alloc_host(&pdev->dev, sizeof(*pltfm_host));

	if (IS_ERR(host)) {
		ret = PTR_ERR(host);
		goto err;
	}

	pltfm_host = sdhci_priv(host);

=======
static void sdhci_get_compatibility(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct sdhci_host *host = platform_get_drvdata(pdev);

	if (device_is_compatible(dev, "fsl,p2020-rev1-esdhc"))
		host->quirks |= SDHCI_QUIRK_BROKEN_DMA;

	if (device_is_compatible(dev, "fsl,p2020-esdhc") ||
	    device_is_compatible(dev, "fsl,p1010-esdhc") ||
	    device_is_compatible(dev, "fsl,t4240-esdhc") ||
	    device_is_compatible(dev, "fsl,mpc8536-esdhc"))
		host->quirks |= SDHCI_QUIRK_BROKEN_TIMEOUT_VAL;
}

void sdhci_get_property(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct sdhci_host *host = platform_get_drvdata(pdev);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	u32 bus_width;

	if (device_property_present(dev, "sdhci,auto-cmd12"))
		host->quirks |= SDHCI_QUIRK_MULTIBLOCK_READ_ACMD12;

	if (device_property_present(dev, "sdhci,1-bit-only") ||
	    (device_property_read_u32(dev, "bus-width", &bus_width) == 0 &&
	    bus_width == 1))
		host->quirks |= SDHCI_QUIRK_FORCE_1_BIT_DATA;

	if (sdhci_wp_inverted(dev))
		host->quirks |= SDHCI_QUIRK_INVERTED_WRITE_PROTECT;

	if (device_property_present(dev, "broken-cd"))
		host->quirks |= SDHCI_QUIRK_BROKEN_CARD_DETECTION;

	if (device_property_present(dev, "no-1-8-v"))
		host->quirks2 |= SDHCI_QUIRK2_NO_1_8_V;

	sdhci_get_compatibility(pdev);

	device_property_read_u32(dev, "clock-frequency", &pltfm_host->clock);

	if (device_property_present(dev, "keep-power-in-suspend"))
		host->mmc->pm_caps |= MMC_PM_KEEP_POWER;

	if (device_property_read_bool(dev, "wakeup-source") ||
	    device_property_read_bool(dev, "enable-sdio-wakeup")) /* legacy */
		host->mmc->pm_caps |= MMC_PM_WAKE_SDIO_IRQ;
}
EXPORT_SYMBOL_GPL(sdhci_get_property);

struct sdhci_host *sdhci_pltfm_init(struct platform_device *pdev,
				    const struct sdhci_pltfm_data *pdata,
				    size_t priv_size)
{
	struct sdhci_host *host;
	void __iomem *ioaddr;
	int irq;

	ioaddr = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(ioaddr))
		return ERR_CAST(ioaddr);

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return ERR_PTR(irq);

	host = sdhci_alloc_host(&pdev->dev,
		sizeof(struct sdhci_pltfm_host) + priv_size);
	if (IS_ERR(host)) {
		dev_err(&pdev->dev, "%s failed %pe\n", __func__, host);
		return ERR_CAST(host);
	}

	host->ioaddr = ioaddr;
	host->irq = irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	host->hw_name = dev_name(&pdev->dev);
	if (pdata && pdata->ops)
		host->ops = pdata->ops;
	else
		host->ops = &sdhci_pltfm_ops;
<<<<<<< HEAD
	if (pdata)
		host->quirks = pdata->quirks;
	host->irq = platform_get_irq(pdev, 0);

	if (!request_mem_region(iomem->start, resource_size(iomem),
		mmc_hostname(host->mmc))) {
		dev_err(&pdev->dev, "cannot request region\n");
		ret = -EBUSY;
		goto err_request;
	}

	host->ioaddr = ioremap(iomem->start, resource_size(iomem));
	if (!host->ioaddr) {
		dev_err(&pdev->dev, "failed to remap registers\n");
		ret = -ENOMEM;
		goto err_remap;
=======
	if (pdata) {
		host->quirks = pdata->quirks;
		host->quirks2 = pdata->quirks2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	platform_set_drvdata(pdev, host);

	return host;
<<<<<<< HEAD

err_remap:
	release_mem_region(iomem->start, resource_size(iomem));
err_request:
	sdhci_free_host(host);
err:
	dev_err(&pdev->dev, "%s failed %d\n", __func__, ret);
	return ERR_PTR(ret);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_init);

void sdhci_pltfm_free(struct platform_device *pdev)
{
	struct sdhci_host *host = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct resource *iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	iounmap(host->ioaddr);
	release_mem_region(iomem->start, resource_size(iomem));
	sdhci_free_host(host);
	platform_set_drvdata(pdev, NULL);
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_free);

int sdhci_pltfm_register(struct platform_device *pdev,
			 struct sdhci_pltfm_data *pdata)
=======

	sdhci_free_host(host);
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_free);

int sdhci_pltfm_init_and_add_host(struct platform_device *pdev,
				  const struct sdhci_pltfm_data *pdata,
				  size_t priv_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sdhci_host *host;
	int ret = 0;

<<<<<<< HEAD
	host = sdhci_pltfm_init(pdev, pdata);
	if (IS_ERR(host))
		return PTR_ERR(host);

	sdhci_get_of_property(pdev);
=======
	host = sdhci_pltfm_init(pdev, pdata, priv_size);
	if (IS_ERR(host))
		return PTR_ERR(host);

	sdhci_get_property(pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = sdhci_add_host(host);
	if (ret)
		sdhci_pltfm_free(pdev);

	return ret;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(sdhci_pltfm_register);

int sdhci_pltfm_unregister(struct platform_device *pdev)
=======
EXPORT_SYMBOL_GPL(sdhci_pltfm_init_and_add_host);

void sdhci_pltfm_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sdhci_host *host = platform_get_drvdata(pdev);
	int dead = (readl(host->ioaddr + SDHCI_INT_STATUS) == 0xffffffff);

	sdhci_remove_host(host, dead);
	sdhci_pltfm_free(pdev);
<<<<<<< HEAD

	return 0;
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_unregister);

#ifdef CONFIG_PM
static int sdhci_pltfm_suspend(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);

	return sdhci_suspend_host(host);
}

static int sdhci_pltfm_resume(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);

	return sdhci_resume_host(host);
}

const struct dev_pm_ops sdhci_pltfm_pmops = {
	.suspend	= sdhci_pltfm_suspend,
	.resume		= sdhci_pltfm_resume,
};
EXPORT_SYMBOL_GPL(sdhci_pltfm_pmops);
#endif	/* CONFIG_PM */
=======
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_remove);

#ifdef CONFIG_PM_SLEEP
int sdhci_pltfm_suspend(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	int ret;

	if (host->tuning_mode != SDHCI_TUNING_MODE_3)
		mmc_retune_needed(host->mmc);

	ret = sdhci_suspend_host(host);
	if (ret)
		return ret;

	clk_disable_unprepare(pltfm_host->clk);

	return 0;
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_suspend);

int sdhci_pltfm_resume(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	int ret;

	ret = clk_prepare_enable(pltfm_host->clk);
	if (ret)
		return ret;

	ret = sdhci_resume_host(host);
	if (ret)
		clk_disable_unprepare(pltfm_host->clk);

	return ret;
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_resume);
#endif

const struct dev_pm_ops sdhci_pltfm_pmops = {
	SET_SYSTEM_SLEEP_PM_OPS(sdhci_pltfm_suspend, sdhci_pltfm_resume)
};
EXPORT_SYMBOL_GPL(sdhci_pltfm_pmops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init sdhci_pltfm_drv_init(void)
{
	pr_info("sdhci-pltfm: SDHCI platform and OF driver helper\n");

	return 0;
}
module_init(sdhci_pltfm_drv_init);

static void __exit sdhci_pltfm_drv_exit(void)
{
}
module_exit(sdhci_pltfm_drv_exit);

MODULE_DESCRIPTION("SDHCI platform and OF driver helper");
MODULE_AUTHOR("Intel Corporation");
MODULE_LICENSE("GPL v2");
