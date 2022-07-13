<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SAMSUNG EXYNOS USB HOST OHCI Controller
 *
 * Copyright (C) 2011 Samsung Electronics Co.Ltd
 * Author: Jingoo Han <jg1.han@samsung.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/clk.h>
#include <linux/platform_device.h>
#include <mach/ohci.h>
#include <plat/usb-phy.h>

struct exynos_ohci_hcd {
	struct device *dev;
	struct usb_hcd *hcd;
	struct clk *clk;
};

static int ohci_exynos_start(struct usb_hcd *hcd)
{
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);
	int ret;

	ohci_dbg(ohci, "ohci_exynos_start, ohci:%p", ohci);

	ret = ohci_init(ohci);
	if (ret < 0)
		return ret;

	ret = ohci_run(ohci);
	if (ret < 0) {
		err("can't start %s", hcd->self.bus_name);
		ohci_stop(hcd);
		return ret;
	}

	return 0;
}

static const struct hc_driver exynos_ohci_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "EXYNOS OHCI Host Controller",
	.hcd_priv_size		= sizeof(struct ohci_hcd),

	.irq			= ohci_irq,
	.flags			= HCD_MEMORY|HCD_USB11,

	.start			= ohci_exynos_start,
	.stop			= ohci_stop,
	.shutdown		= ohci_shutdown,

	.get_frame_number	= ohci_get_frame,

	.urb_enqueue		= ohci_urb_enqueue,
	.urb_dequeue		= ohci_urb_dequeue,
	.endpoint_disable	= ohci_endpoint_disable,

	.hub_status_data	= ohci_hub_status_data,
	.hub_control		= ohci_hub_control,
#ifdef	CONFIG_PM
	.bus_suspend		= ohci_bus_suspend,
	.bus_resume		= ohci_bus_resume,
#endif
	.start_port_reset	= ohci_start_port_reset,
};

static int __devinit exynos_ohci_probe(struct platform_device *pdev)
{
	struct exynos4_ohci_platdata *pdata;
	struct exynos_ohci_hcd *exynos_ohci;
	struct usb_hcd *hcd;
	struct ohci_hcd *ohci;
=======
 */

#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/phy/phy.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>

#include "ohci.h"

#define DRIVER_DESC "OHCI Exynos driver"

static struct hc_driver __read_mostly exynos_ohci_hc_driver;

#define to_exynos_ohci(hcd) (struct exynos_ohci_hcd *)(hcd_to_ohci(hcd)->priv)

#define PHY_NUMBER 3

struct exynos_ohci_hcd {
	struct clk *clk;
	struct device_node *of_node;
	struct phy *phy[PHY_NUMBER];
	bool legacy_phy;
};

static int exynos_ohci_get_phy(struct device *dev,
				struct exynos_ohci_hcd *exynos_ohci)
{
	struct device_node *child;
	struct phy *phy;
	int phy_number, num_phys;
	int ret;

	/* Get PHYs for the controller */
	num_phys = of_count_phandle_with_args(dev->of_node, "phys",
					      "#phy-cells");
	for (phy_number = 0; phy_number < num_phys; phy_number++) {
		phy = devm_of_phy_get_by_index(dev, dev->of_node, phy_number);
		if (IS_ERR(phy))
			return PTR_ERR(phy);
		exynos_ohci->phy[phy_number] = phy;
	}
	if (num_phys > 0)
		return 0;

	/* Get PHYs using legacy bindings */
	for_each_available_child_of_node(dev->of_node, child) {
		ret = of_property_read_u32(child, "reg", &phy_number);
		if (ret) {
			dev_err(dev, "Failed to parse device tree\n");
			of_node_put(child);
			return ret;
		}

		if (phy_number >= PHY_NUMBER) {
			dev_err(dev, "Invalid number of PHYs\n");
			of_node_put(child);
			return -EINVAL;
		}

		phy = devm_of_phy_optional_get(dev, child, NULL);
		exynos_ohci->phy[phy_number] = phy;
		if (IS_ERR(phy)) {
			of_node_put(child);
			return PTR_ERR(phy);
		}
	}

	exynos_ohci->legacy_phy = true;
	return 0;
}

static int exynos_ohci_phy_enable(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct exynos_ohci_hcd *exynos_ohci = to_exynos_ohci(hcd);
	int i;
	int ret = 0;

	for (i = 0; ret == 0 && i < PHY_NUMBER; i++)
		ret = phy_power_on(exynos_ohci->phy[i]);
	if (ret)
		for (i--; i >= 0; i--)
			phy_power_off(exynos_ohci->phy[i]);

	return ret;
}

static void exynos_ohci_phy_disable(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct exynos_ohci_hcd *exynos_ohci = to_exynos_ohci(hcd);
	int i;

	for (i = 0; i < PHY_NUMBER; i++)
		phy_power_off(exynos_ohci->phy[i]);
}

static int exynos_ohci_probe(struct platform_device *pdev)
{
	struct exynos_ohci_hcd *exynos_ohci;
	struct usb_hcd *hcd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct resource *res;
	int irq;
	int err;

<<<<<<< HEAD
	pdata = pdev->dev.platform_data;
	if (!pdata) {
		dev_err(&pdev->dev, "No platform data defined\n");
		return -EINVAL;
	}

	exynos_ohci = kzalloc(sizeof(struct exynos_ohci_hcd), GFP_KERNEL);
	if (!exynos_ohci)
		return -ENOMEM;

	exynos_ohci->dev = &pdev->dev;

	hcd = usb_create_hcd(&exynos_ohci_hc_driver, &pdev->dev,
					dev_name(&pdev->dev));
	if (!hcd) {
		dev_err(&pdev->dev, "Unable to create HCD\n");
		err = -ENOMEM;
		goto fail_hcd;
	}

	exynos_ohci->hcd = hcd;
	exynos_ohci->clk = clk_get(&pdev->dev, "usbhost");
=======
	/*
	 * Right now device-tree probed devices don't get dma_mask set.
	 * Since shared usb code relies on it, set it here for now.
	 * Once we move to full device tree support this will vanish off.
	 */
	err = dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (err)
		return err;

	hcd = usb_create_hcd(&exynos_ohci_hc_driver,
				&pdev->dev, dev_name(&pdev->dev));
	if (!hcd) {
		dev_err(&pdev->dev, "Unable to create HCD\n");
		return -ENOMEM;
	}

	exynos_ohci = to_exynos_ohci(hcd);

	err = exynos_ohci_get_phy(&pdev->dev, exynos_ohci);
	if (err)
		goto fail_clk;

	exynos_ohci->clk = devm_clk_get(&pdev->dev, "usbhost");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (IS_ERR(exynos_ohci->clk)) {
		dev_err(&pdev->dev, "Failed to get usbhost clock\n");
		err = PTR_ERR(exynos_ohci->clk);
		goto fail_clk;
	}

<<<<<<< HEAD
	err = clk_enable(exynos_ohci->clk);
	if (err)
		goto fail_clken;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "Failed to get I/O memory\n");
		err = -ENXIO;
		goto fail_io;
	}

	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);
	hcd->regs = ioremap(res->start, resource_size(res));
	if (!hcd->regs) {
		dev_err(&pdev->dev, "Failed to remap I/O memory\n");
		err = -ENOMEM;
		goto fail_io;
	}

	irq = platform_get_irq(pdev, 0);
	if (!irq) {
		dev_err(&pdev->dev, "Failed to get IRQ\n");
		err = -ENODEV;
		goto fail;
	}

	if (pdata->phy_init)
		pdata->phy_init(pdev, S5P_USB_PHY_HOST);

	ohci = hcd_to_ohci(hcd);
	ohci_hcd_init(ohci);
=======
	err = clk_prepare_enable(exynos_ohci->clk);
	if (err)
		goto fail_clk;

	hcd->regs = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(hcd->regs)) {
		err = PTR_ERR(hcd->regs);
		goto fail_io;
	}
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		err = irq;
		goto fail_io;
	}

	platform_set_drvdata(pdev, hcd);

	err = exynos_ohci_phy_enable(&pdev->dev);
	if (err) {
		dev_err(&pdev->dev, "Failed to enable USB phy\n");
		goto fail_io;
	}

	/*
	 * Workaround: reset of_node pointer to avoid conflict between legacy
	 * Exynos OHCI port subnodes and generic USB device bindings
	 */
	exynos_ohci->of_node = pdev->dev.of_node;
	if (exynos_ohci->legacy_phy)
		pdev->dev.of_node = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (err) {
		dev_err(&pdev->dev, "Failed to add USB HCD\n");
<<<<<<< HEAD
		goto fail;
	}

	platform_set_drvdata(pdev, exynos_ohci);

	return 0;

fail:
	iounmap(hcd->regs);
fail_io:
	clk_disable(exynos_ohci->clk);
fail_clken:
	clk_put(exynos_ohci->clk);
fail_clk:
	usb_put_hcd(hcd);
fail_hcd:
	kfree(exynos_ohci);
	return err;
}

static int __devexit exynos_ohci_remove(struct platform_device *pdev)
{
	struct exynos4_ohci_platdata *pdata = pdev->dev.platform_data;
	struct exynos_ohci_hcd *exynos_ohci = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = exynos_ohci->hcd;

	usb_remove_hcd(hcd);

	if (pdata && pdata->phy_exit)
		pdata->phy_exit(pdev, S5P_USB_PHY_HOST);

	iounmap(hcd->regs);

	clk_disable(exynos_ohci->clk);
	clk_put(exynos_ohci->clk);

	usb_put_hcd(hcd);
	kfree(exynos_ohci);

	return 0;
=======
		goto fail_add_hcd;
	}
	device_wakeup_enable(hcd->self.controller);
	return 0;

fail_add_hcd:
	exynos_ohci_phy_disable(&pdev->dev);
	pdev->dev.of_node = exynos_ohci->of_node;
fail_io:
	clk_disable_unprepare(exynos_ohci->clk);
fail_clk:
	usb_put_hcd(hcd);
	return err;
}

static void exynos_ohci_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct exynos_ohci_hcd *exynos_ohci = to_exynos_ohci(hcd);

	pdev->dev.of_node = exynos_ohci->of_node;

	usb_remove_hcd(hcd);

	exynos_ohci_phy_disable(&pdev->dev);

	clk_disable_unprepare(exynos_ohci->clk);

	usb_put_hcd(hcd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void exynos_ohci_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct exynos_ohci_hcd *exynos_ohci = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = exynos_ohci->hcd;
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);
}

#ifdef CONFIG_PM
static int exynos_ohci_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct exynos_ohci_hcd *exynos_ohci = dev_get_drvdata(dev);
	struct usb_hcd *hcd = exynos_ohci->hcd;
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);
	struct platform_device *pdev = to_platform_device(dev);
	struct exynos4_ohci_platdata *pdata = pdev->dev.platform_data;
	unsigned long flags;
	int rc = 0;

	/*
	 * Root hub was already suspended. Disable irq emission and
	 * mark HW unaccessible, bail out if RH has been resumed. Use
	 * the spinlock to properly synchronize with possible pending
	 * RH suspend or resume activity.
	 */
	spin_lock_irqsave(&ohci->lock, flags);
	if (ohci->rh_state != OHCI_RH_SUSPENDED &&
			ohci->rh_state != OHCI_RH_HALTED) {
		rc = -EINVAL;
		goto fail;
	}

	clear_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);

	if (pdata && pdata->phy_exit)
		pdata->phy_exit(pdev, S5P_USB_PHY_HOST);
fail:
	spin_unlock_irqrestore(&ohci->lock, flags);

	return rc;
=======
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct exynos_ohci_hcd *exynos_ohci = to_exynos_ohci(hcd);
	bool do_wakeup = device_may_wakeup(dev);
	int rc = ohci_suspend(hcd, do_wakeup);

	if (rc)
		return rc;

	exynos_ohci_phy_disable(dev);

	clk_disable_unprepare(exynos_ohci->clk);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int exynos_ohci_resume(struct device *dev)
{
<<<<<<< HEAD
	struct exynos_ohci_hcd *exynos_ohci = dev_get_drvdata(dev);
	struct usb_hcd *hcd = exynos_ohci->hcd;
	struct platform_device *pdev = to_platform_device(dev);
	struct exynos4_ohci_platdata *pdata = pdev->dev.platform_data;

	if (pdata && pdata->phy_init)
		pdata->phy_init(pdev, S5P_USB_PHY_HOST);

	/* Mark hardware accessible again as we are out of D3 state by now */
	set_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);

	ohci_finish_controller_resume(hcd);
=======
	struct usb_hcd *hcd			= dev_get_drvdata(dev);
	struct exynos_ohci_hcd *exynos_ohci	= to_exynos_ohci(hcd);
	int ret;

	clk_prepare_enable(exynos_ohci->clk);

	ret = exynos_ohci_phy_enable(dev);
	if (ret) {
		dev_err(dev, "Failed to enable USB phy\n");
		clk_disable_unprepare(exynos_ohci->clk);
		return ret;
	}

	ohci_resume(hcd, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
#else
#define exynos_ohci_suspend	NULL
#define exynos_ohci_resume	NULL
#endif

<<<<<<< HEAD
=======
static const struct ohci_driver_overrides exynos_overrides __initconst = {
	.extra_priv_size =	sizeof(struct exynos_ohci_hcd),
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct dev_pm_ops exynos_ohci_pm_ops = {
	.suspend	= exynos_ohci_suspend,
	.resume		= exynos_ohci_resume,
};

<<<<<<< HEAD
static struct platform_driver exynos_ohci_driver = {
	.probe		= exynos_ohci_probe,
	.remove		= __devexit_p(exynos_ohci_remove),
	.shutdown	= exynos_ohci_shutdown,
	.driver = {
		.name	= "exynos-ohci",
		.owner	= THIS_MODULE,
		.pm	= &exynos_ohci_pm_ops,
	}
};

MODULE_ALIAS("platform:exynos-ohci");
MODULE_AUTHOR("Jingoo Han <jg1.han@samsung.com>");
=======
#ifdef CONFIG_OF
static const struct of_device_id exynos_ohci_match[] = {
	{ .compatible = "samsung,exynos4210-ohci" },
	{},
};
MODULE_DEVICE_TABLE(of, exynos_ohci_match);
#endif

static struct platform_driver exynos_ohci_driver = {
	.probe		= exynos_ohci_probe,
	.remove_new	= exynos_ohci_remove,
	.shutdown	= exynos_ohci_shutdown,
	.driver = {
		.name	= "exynos-ohci",
		.pm	= &exynos_ohci_pm_ops,
		.of_match_table	= of_match_ptr(exynos_ohci_match),
	}
};
static int __init ohci_exynos_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	ohci_init_driver(&exynos_ohci_hc_driver, &exynos_overrides);
	return platform_driver_register(&exynos_ohci_driver);
}
module_init(ohci_exynos_init);

static void __exit ohci_exynos_cleanup(void)
{
	platform_driver_unregister(&exynos_ohci_driver);
}
module_exit(ohci_exynos_cleanup);

MODULE_ALIAS("platform:exynos-ohci");
MODULE_AUTHOR("Jingoo Han <jg1.han@samsung.com>");
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
