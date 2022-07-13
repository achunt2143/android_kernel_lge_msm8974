<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Synopsys DesignWare Multimedia Card Interface driver
 *
 * Copyright (C) 2009 NXP Semiconductors
 * Copyright (C) 2009, 2010 Imagination Technologies Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

=======
 */

#include <linux/err.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <linux/slab.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/dw_mmc.h>
#include "dw_mmc.h"

static int dw_mci_pltfm_probe(struct platform_device *pdev)
{
	struct dw_mci *host;
	struct resource	*regs;
	int ret;

	host = kzalloc(sizeof(struct dw_mci), GFP_KERNEL);
	if (!host)
		return -ENOMEM;

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!regs) {
		ret = -ENXIO;
		goto err_free;
	}

	host->irq = platform_get_irq(pdev, 0);
	if (host->irq < 0) {
		ret = host->irq;
		goto err_free;
	}

	host->dev = pdev->dev;
	host->irq_flags = 0;
	host->pdata = pdev->dev.platform_data;
	ret = -ENOMEM;
	host->regs = ioremap(regs->start, resource_size(regs));
	if (!host->regs)
		goto err_free;
	platform_set_drvdata(pdev, host);
	ret = dw_mci_probe(host);
	if (ret)
		goto err_out;
	return ret;
err_out:
	iounmap(host->regs);
err_free:
	kfree(host);
	return ret;
}

static int __exit dw_mci_pltfm_remove(struct platform_device *pdev)
{
	struct dw_mci *host = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);
	dw_mci_remove(host);
	iounmap(host->regs);
	kfree(host);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
/*
 * TODO: we should probably disable the clock to the card in the suspend path.
 */
static int dw_mci_pltfm_suspend(struct device *dev)
{
	int ret;
	struct dw_mci *host = dev_get_drvdata(dev);

	ret = dw_mci_suspend(host);
	if (ret)
		return ret;

	return 0;
}

static int dw_mci_pltfm_resume(struct device *dev)
{
	int ret;
	struct dw_mci *host = dev_get_drvdata(dev);

	ret = dw_mci_resume(host);
	if (ret)
		return ret;

	return 0;
}
#else
#define dw_mci_pltfm_suspend	NULL
#define dw_mci_pltfm_resume	NULL
#endif /* CONFIG_PM_SLEEP */

static SIMPLE_DEV_PM_OPS(dw_mci_pltfm_pmops, dw_mci_pltfm_suspend, dw_mci_pltfm_resume);

static struct platform_driver dw_mci_pltfm_driver = {
	.remove		= __exit_p(dw_mci_pltfm_remove),
	.driver		= {
		.name		= "dw_mmc",
=======
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/of.h>
#include <linux/mfd/altera-sysmgr.h>
#include <linux/regmap.h>

#include "dw_mmc.h"
#include "dw_mmc-pltfm.h"

#define SOCFPGA_DW_MMC_CLK_PHASE_STEP	45
#define SYSMGR_SDMMC_CTRL_SET(smplsel, drvsel, reg_shift) \
	((((smplsel) & 0x7) << reg_shift) | (((drvsel) & 0x7) << 0))

int dw_mci_pltfm_register(struct platform_device *pdev,
			  const struct dw_mci_drv_data *drv_data)
{
	struct dw_mci *host;
	struct resource	*regs;

	host = devm_kzalloc(&pdev->dev, sizeof(struct dw_mci), GFP_KERNEL);
	if (!host)
		return -ENOMEM;

	host->irq = platform_get_irq(pdev, 0);
	if (host->irq < 0)
		return host->irq;

	host->drv_data = drv_data;
	host->dev = &pdev->dev;
	host->irq_flags = 0;
	host->pdata = pdev->dev.platform_data;

	host->regs = devm_platform_get_and_ioremap_resource(pdev, 0, &regs);
	if (IS_ERR(host->regs))
		return PTR_ERR(host->regs);

	/* Get registers' physical base address */
	host->phy_regs = regs->start;

	platform_set_drvdata(pdev, host);
	return dw_mci_probe(host);
}
EXPORT_SYMBOL_GPL(dw_mci_pltfm_register);

const struct dev_pm_ops dw_mci_pltfm_pmops = {
	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
				pm_runtime_force_resume)
	SET_RUNTIME_PM_OPS(dw_mci_runtime_suspend,
			   dw_mci_runtime_resume,
			   NULL)
};
EXPORT_SYMBOL_GPL(dw_mci_pltfm_pmops);

static int dw_mci_socfpga_priv_init(struct dw_mci *host)
{
	struct device_node *np = host->dev->of_node;
	struct regmap *sys_mgr_base_addr;
	u32 clk_phase[2] = {0}, reg_offset, reg_shift;
	int i, rc, hs_timing;

	rc = of_property_read_variable_u32_array(np, "clk-phase-sd-hs", &clk_phase[0], 2, 0);
	if (rc < 0)
		return 0;

	sys_mgr_base_addr = altr_sysmgr_regmap_lookup_by_phandle(np, "altr,sysmgr-syscon");
	if (IS_ERR(sys_mgr_base_addr)) {
		dev_warn(host->dev, "clk-phase-sd-hs was specified, but failed to find altr,sys-mgr regmap!\n");
		return 0;
	}

	of_property_read_u32_index(np, "altr,sysmgr-syscon", 1, &reg_offset);
	of_property_read_u32_index(np, "altr,sysmgr-syscon", 2, &reg_shift);

	for (i = 0; i < ARRAY_SIZE(clk_phase); i++)
		clk_phase[i] /= SOCFPGA_DW_MMC_CLK_PHASE_STEP;

	hs_timing = SYSMGR_SDMMC_CTRL_SET(clk_phase[0], clk_phase[1], reg_shift);
	regmap_write(sys_mgr_base_addr, reg_offset, hs_timing);

	return 0;
}

static const struct dw_mci_drv_data socfpga_drv_data = {
	.init		= dw_mci_socfpga_priv_init,
};

static const struct of_device_id dw_mci_pltfm_match[] = {
	{ .compatible = "snps,dw-mshc", },
	{ .compatible = "altr,socfpga-dw-mshc", .data = &socfpga_drv_data, },
	{ .compatible = "img,pistachio-dw-mshc", },
	{},
};
MODULE_DEVICE_TABLE(of, dw_mci_pltfm_match);

static int dw_mci_pltfm_probe(struct platform_device *pdev)
{
	const struct dw_mci_drv_data *drv_data = NULL;
	const struct of_device_id *match;

	if (pdev->dev.of_node) {
		match = of_match_node(dw_mci_pltfm_match, pdev->dev.of_node);
		drv_data = match->data;
	}

	return dw_mci_pltfm_register(pdev, drv_data);
}

void dw_mci_pltfm_remove(struct platform_device *pdev)
{
	struct dw_mci *host = platform_get_drvdata(pdev);

	dw_mci_remove(host);
}
EXPORT_SYMBOL_GPL(dw_mci_pltfm_remove);

static struct platform_driver dw_mci_pltfm_driver = {
	.probe		= dw_mci_pltfm_probe,
	.remove_new	= dw_mci_pltfm_remove,
	.driver		= {
		.name		= "dw_mmc",
		.probe_type	= PROBE_PREFER_ASYNCHRONOUS,
		.of_match_table	= dw_mci_pltfm_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pm		= &dw_mci_pltfm_pmops,
	},
};

<<<<<<< HEAD
static int __init dw_mci_init(void)
{
	return platform_driver_probe(&dw_mci_pltfm_driver, dw_mci_pltfm_probe);
}

static void __exit dw_mci_exit(void)
{
	platform_driver_unregister(&dw_mci_pltfm_driver);
}

module_init(dw_mci_init);
module_exit(dw_mci_exit);
=======
module_platform_driver(dw_mci_pltfm_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_DESCRIPTION("DW Multimedia Card Interface driver");
MODULE_AUTHOR("NXP Semiconductor VietNam");
MODULE_AUTHOR("Imagination Technologies Ltd");
MODULE_LICENSE("GPL v2");
