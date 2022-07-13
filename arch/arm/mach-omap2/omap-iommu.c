<<<<<<< HEAD
/*
 * omap iommu: omap device registration
 *
 * Copyright (C) 2008-2009 Nokia Corporation
 *
 * Written by Hiroshi DOYU <Hiroshi.DOYU@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/platform_device.h>

#include <plat/iommu.h>
#include <plat/irqs.h>

struct iommu_device {
	resource_size_t base;
	int irq;
	struct iommu_platform_data pdata;
	struct resource res[2];
};
static struct iommu_device *devices;
static int num_iommu_devices;

#ifdef CONFIG_ARCH_OMAP3
static struct iommu_device omap3_devices[] = {
	{
		.base = 0x480bd400,
		.irq = 24,
		.pdata = {
			.name = "isp",
			.nr_tlb_entries = 8,
			.clk_name = "cam_ick",
			.da_start = 0x0,
			.da_end = 0xFFFFF000,
		},
	},
#if defined(CONFIG_OMAP_IOMMU_IVA2)
	{
		.base = 0x5d000000,
		.irq = 28,
		.pdata = {
			.name = "iva2",
			.nr_tlb_entries = 32,
			.clk_name = "iva2_ck",
			.da_start = 0x11000000,
			.da_end = 0xFFFFF000,
		},
	},
#endif
};
#define NR_OMAP3_IOMMU_DEVICES ARRAY_SIZE(omap3_devices)
static struct platform_device *omap3_iommu_pdev[NR_OMAP3_IOMMU_DEVICES];
#else
#define omap3_devices		NULL
#define NR_OMAP3_IOMMU_DEVICES	0
#define omap3_iommu_pdev	NULL
#endif

#ifdef CONFIG_ARCH_OMAP4
static struct iommu_device omap4_devices[] = {
	{
		.base = OMAP4_MMU1_BASE,
		.irq = OMAP44XX_IRQ_DUCATI_MMU,
		.pdata = {
			.name = "ducati",
			.nr_tlb_entries = 32,
			.clk_name = "ipu_fck",
			.da_start = 0x0,
			.da_end = 0xFFFFF000,
		},
	},
#if defined(CONFIG_MPU_TESLA_IOMMU)
	{
		.base = OMAP4_MMU2_BASE,
		.irq = INT_44XX_DSP_MMU,
		.pdata = {
			.name = "tesla",
			.nr_tlb_entries = 32,
			.clk_name = "tesla_ick",
			.da_start = 0x0,
			.da_end = 0xFFFFF000,
		},
	},
#endif
};
#define NR_OMAP4_IOMMU_DEVICES ARRAY_SIZE(omap4_devices)
static struct platform_device *omap4_iommu_pdev[NR_OMAP4_IOMMU_DEVICES];
#else
#define omap4_devices		NULL
#define NR_OMAP4_IOMMU_DEVICES	0
#define omap4_iommu_pdev	NULL
#endif

static struct platform_device **omap_iommu_pdev;

static int __init omap_iommu_init(void)
{
	int i, err;
	struct resource res[] = {
		{ .flags = IORESOURCE_MEM },
		{ .flags = IORESOURCE_IRQ },
	};

	if (cpu_is_omap34xx()) {
		devices = omap3_devices;
		omap_iommu_pdev = omap3_iommu_pdev;
		num_iommu_devices = NR_OMAP3_IOMMU_DEVICES;
	} else if (cpu_is_omap44xx()) {
		devices = omap4_devices;
		omap_iommu_pdev = omap4_iommu_pdev;
		num_iommu_devices = NR_OMAP4_IOMMU_DEVICES;
	} else
		return -ENODEV;

	for (i = 0; i < num_iommu_devices; i++) {
		struct platform_device *pdev;
		const struct iommu_device *d = &devices[i];

		pdev = platform_device_alloc("omap-iommu", i);
		if (!pdev) {
			err = -ENOMEM;
			goto err_out;
		}

		res[0].start = d->base;
		res[0].end = d->base + MMU_REG_SIZE - 1;
		res[1].start = res[1].end = d->irq;

		err = platform_device_add_resources(pdev, res,
						    ARRAY_SIZE(res));
		if (err)
			goto err_out;
		err = platform_device_add_data(pdev, &d->pdata,
					       sizeof(d->pdata));
		if (err)
			goto err_out;
		err = platform_device_add(pdev);
		if (err)
			goto err_out;
		omap_iommu_pdev[i] = pdev;
	}
	return 0;

err_out:
	while (i--)
		platform_device_put(omap_iommu_pdev[i]);
	return err;
}
/* must be ready before omap3isp is probed */
subsys_initcall(omap_iommu_init);

static void __exit omap_iommu_exit(void)
{
	int i;

	for (i = 0; i < num_iommu_devices; i++)
		platform_device_unregister(omap_iommu_pdev[i]);
}
module_exit(omap_iommu_exit);

MODULE_AUTHOR("Hiroshi DOYU");
MODULE_DESCRIPTION("omap iommu: omap device registration");
MODULE_LICENSE("GPL v2");
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * OMAP IOMMU quirks for various TI SoCs
 *
 * Copyright (C) 2015-2019 Texas Instruments Incorporated - https://www.ti.com/
 *      Suman Anna <s-anna@ti.com>
 */

#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/list.h>

#include "clockdomain.h"
#include "powerdomain.h"
#include "common.h"

struct pwrdm_link {
	struct device *dev;
	struct powerdomain *pwrdm;
	struct list_head node;
};

static DEFINE_SPINLOCK(iommu_lock);
static struct clockdomain *emu_clkdm;
static atomic_t emu_count;

static void omap_iommu_dra7_emu_swsup_config(struct platform_device *pdev,
					     bool enable)
{
	struct device_node *np = pdev->dev.of_node;
	unsigned long flags;

	if (!of_device_is_compatible(np, "ti,dra7-dsp-iommu"))
		return;

	if (!emu_clkdm) {
		emu_clkdm = clkdm_lookup("emu_clkdm");
		if (WARN_ON_ONCE(!emu_clkdm))
			return;
	}

	spin_lock_irqsave(&iommu_lock, flags);

	if (enable && (atomic_inc_return(&emu_count) == 1))
		clkdm_deny_idle(emu_clkdm);
	else if (!enable && (atomic_dec_return(&emu_count) == 0))
		clkdm_allow_idle(emu_clkdm);

	spin_unlock_irqrestore(&iommu_lock, flags);
}

static struct powerdomain *_get_pwrdm(struct device *dev)
{
	struct clk *clk;
	struct clk_hw_omap *hwclk;
	struct clockdomain *clkdm;
	struct powerdomain *pwrdm = NULL;
	struct pwrdm_link *entry;
	unsigned long flags;
	static LIST_HEAD(cache);

	spin_lock_irqsave(&iommu_lock, flags);

	list_for_each_entry(entry, &cache, node) {
		if (entry->dev == dev) {
			pwrdm = entry->pwrdm;
			break;
		}
	}

	spin_unlock_irqrestore(&iommu_lock, flags);

	if (pwrdm)
		return pwrdm;

	clk = of_clk_get(dev->of_node->parent, 0);
	if (IS_ERR(clk)) {
		dev_err(dev, "no fck found\n");
		return NULL;
	}

	hwclk = to_clk_hw_omap(__clk_get_hw(clk));
	clk_put(clk);
	if (!hwclk || !hwclk->clkdm_name) {
		dev_err(dev, "no hwclk data\n");
		return NULL;
	}

	clkdm = clkdm_lookup(hwclk->clkdm_name);
	if (!clkdm) {
		dev_err(dev, "clkdm not found: %s\n", hwclk->clkdm_name);
		return NULL;
	}

	pwrdm = clkdm_get_pwrdm(clkdm);
	if (!pwrdm) {
		dev_err(dev, "pwrdm not found: %s\n", clkdm->name);
		return NULL;
	}

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (entry) {
		entry->dev = dev;
		entry->pwrdm = pwrdm;
		spin_lock_irqsave(&iommu_lock, flags);
		list_add(&entry->node, &cache);
		spin_unlock_irqrestore(&iommu_lock, flags);
	}

	return pwrdm;
}

int omap_iommu_set_pwrdm_constraint(struct platform_device *pdev, bool request,
				    u8 *pwrst)
{
	struct powerdomain *pwrdm;
	u8 next_pwrst;
	int ret = 0;

	pwrdm = _get_pwrdm(&pdev->dev);
	if (!pwrdm)
		return -ENODEV;

	if (request) {
		*pwrst = pwrdm_read_next_pwrst(pwrdm);
		omap_iommu_dra7_emu_swsup_config(pdev, true);
	}

	if (*pwrst > PWRDM_POWER_RET)
		goto out;

	next_pwrst = request ? PWRDM_POWER_ON : *pwrst;

	ret = pwrdm_set_next_pwrst(pwrdm, next_pwrst);

out:
	if (!request)
		omap_iommu_dra7_emu_swsup_config(pdev, false);

	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
