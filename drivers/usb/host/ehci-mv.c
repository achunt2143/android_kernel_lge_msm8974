<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2011 Marvell International Ltd. All rights reserved.
 * Author: Chao Xie <chao.xie@marvell.com>
 *        Neil Zhang <zhangwm@marvell.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
<<<<<<< HEAD
#include <linux/usb/otg.h>
#include <linux/platform_data/mv_usb.h>

#define CAPLENGTH_MASK         (0xff)

struct ehci_hcd_mv {
	struct usb_hcd *hcd;

	/* Which mode does this ehci running OTG/Host ? */
	int mode;

	void __iomem *phy_regs;
=======
#include <linux/err.h>
#include <linux/usb/otg.h>
#include <linux/usb/of.h>
#include <linux/platform_data/mv_usb.h>
#include <linux/io.h>

#include <linux/usb/hcd.h>

#include "ehci.h"

/* registers */
#define U2x_CAPREGS_OFFSET       0x100

#define CAPLENGTH_MASK         (0xff)

#define hcd_to_ehci_hcd_mv(h) ((struct ehci_hcd_mv *)hcd_to_ehci(h)->priv)

struct ehci_hcd_mv {
	/* Which mode does this ehci running OTG/Host ? */
	int mode;

	void __iomem *base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __iomem *cap_regs;
	void __iomem *op_regs;

	struct usb_phy *otg;
<<<<<<< HEAD

	struct mv_usb_platform_data *pdata;

	/* clock source and total clock number */
	unsigned int clknum;
	struct clk *clk[0];
};

static void ehci_clock_enable(struct ehci_hcd_mv *ehci_mv)
{
	unsigned int i;

	for (i = 0; i < ehci_mv->clknum; i++)
		clk_enable(ehci_mv->clk[i]);
}

static void ehci_clock_disable(struct ehci_hcd_mv *ehci_mv)
{
	unsigned int i;

	for (i = 0; i < ehci_mv->clknum; i++)
		clk_disable(ehci_mv->clk[i]);
}

=======
	struct clk *clk;

	struct phy *phy;

	int (*set_vbus)(unsigned int vbus);
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int mv_ehci_enable(struct ehci_hcd_mv *ehci_mv)
{
	int retval;

<<<<<<< HEAD
	ehci_clock_enable(ehci_mv);
	if (ehci_mv->pdata->phy_init) {
		retval = ehci_mv->pdata->phy_init(ehci_mv->phy_regs);
		if (retval)
			return retval;
	}

	return 0;
=======
	retval = clk_prepare_enable(ehci_mv->clk);
	if (retval)
		return retval;

	retval = phy_init(ehci_mv->phy);
	if (retval)
		clk_disable_unprepare(ehci_mv->clk);

	return retval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void mv_ehci_disable(struct ehci_hcd_mv *ehci_mv)
{
<<<<<<< HEAD
	if (ehci_mv->pdata->phy_deinit)
		ehci_mv->pdata->phy_deinit(ehci_mv->phy_regs);
	ehci_clock_disable(ehci_mv);
=======
	phy_exit(ehci_mv->phy);
	clk_disable_unprepare(ehci_mv->clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mv_ehci_reset(struct usb_hcd *hcd)
{
<<<<<<< HEAD
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	struct device *dev = hcd->self.controller;
	struct ehci_hcd_mv *ehci_mv = dev_get_drvdata(dev);
=======
	struct device *dev = hcd->self.controller;
	struct ehci_hcd_mv *ehci_mv = hcd_to_ehci_hcd_mv(hcd);
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	u32 status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int retval;

	if (ehci_mv == NULL) {
		dev_err(dev, "Can not find private ehci data\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	/*
	 * data structure init
	 */
	retval = ehci_init(hcd);
	if (retval) {
		dev_err(dev, "ehci_init failed %d\n", retval);
		return retval;
	}

	hcd->has_tt = 1;
	ehci->sbrn = 0x20;

	retval = ehci_reset(ehci);
	if (retval) {
		dev_err(dev, "ehci_reset failed %d\n", retval);
		return retval;
	}

	return 0;
}

static const struct hc_driver mv_ehci_hc_driver = {
	.description = hcd_name,
	.product_desc = "Marvell EHCI",
	.hcd_priv_size = sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq = ehci_irq,
	.flags = HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 */
	.reset = mv_ehci_reset,
	.start = ehci_run,
	.stop = ehci_stop,
	.shutdown = ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue = ehci_urb_enqueue,
	.urb_dequeue = ehci_urb_dequeue,
	.endpoint_disable = ehci_endpoint_disable,
	.endpoint_reset = ehci_endpoint_reset,
	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,

	/*
	 * scheduling support
	 */
	.get_frame_number = ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data = ehci_hub_status_data,
	.hub_control = ehci_hub_control,
	.bus_suspend = ehci_bus_suspend,
	.bus_resume = ehci_bus_resume,
=======
	hcd->has_tt = 1;

	retval = ehci_setup(hcd);
	if (retval)
		dev_err(dev, "ehci_setup failed %d\n", retval);

	if (of_usb_get_phy_mode(dev->of_node) == USBPHY_INTERFACE_MODE_HSIC) {
		status = ehci_readl(ehci, &ehci->regs->port_status[0]);
		status |= PORT_TEST_FORCE;
		ehci_writel(ehci, status, &ehci->regs->port_status[0]);
		status &= ~PORT_TEST_FORCE;
		ehci_writel(ehci, status, &ehci->regs->port_status[0]);
	}

	return retval;
}

static struct hc_driver __read_mostly ehci_platform_hc_driver;

static const struct ehci_driver_overrides platform_overrides __initconst = {
	.reset =		mv_ehci_reset,
	.extra_priv_size =	sizeof(struct ehci_hcd_mv),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int mv_ehci_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct mv_usb_platform_data *pdata = pdev->dev.platform_data;
=======
	struct mv_usb_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct usb_hcd *hcd;
	struct ehci_hcd *ehci;
	struct ehci_hcd_mv *ehci_mv;
	struct resource *r;
<<<<<<< HEAD
	int clk_i, retval = -ENODEV;
	u32 offset;
	size_t size;

	if (!pdata) {
		dev_err(&pdev->dev, "missing platform_data\n");
		return -ENODEV;
	}
=======
	int retval;
	u32 offset;
	u32 status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (usb_disabled())
		return -ENODEV;

<<<<<<< HEAD
	hcd = usb_create_hcd(&mv_ehci_hc_driver, &pdev->dev, "mv ehci");
	if (!hcd)
		return -ENOMEM;

	size = sizeof(*ehci_mv) + sizeof(struct clk *) * pdata->clknum;
	ehci_mv = kzalloc(size, GFP_KERNEL);
	if (ehci_mv == NULL) {
		dev_err(&pdev->dev, "cannot allocate ehci_hcd_mv\n");
		retval = -ENOMEM;
		goto err_put_hcd;
	}

	platform_set_drvdata(pdev, ehci_mv);
	ehci_mv->pdata = pdata;
	ehci_mv->hcd = hcd;

	ehci_mv->clknum = pdata->clknum;
	for (clk_i = 0; clk_i < ehci_mv->clknum; clk_i++) {
		ehci_mv->clk[clk_i] =
		    clk_get(&pdev->dev, pdata->clkname[clk_i]);
		if (IS_ERR(ehci_mv->clk[clk_i])) {
			dev_err(&pdev->dev, "error get clck \"%s\"\n",
				pdata->clkname[clk_i]);
			retval = PTR_ERR(ehci_mv->clk[clk_i]);
			goto err_put_clk;
		}
	}

	r = platform_get_resource_byname(pdev, IORESOURCE_MEM, "phyregs");
	if (r == NULL) {
		dev_err(&pdev->dev, "no phy I/O memory resource defined\n");
		retval = -ENODEV;
		goto err_put_clk;
	}

	ehci_mv->phy_regs = ioremap(r->start, resource_size(r));
	if (ehci_mv->phy_regs == 0) {
		dev_err(&pdev->dev, "failed to map phy I/O memory\n");
		retval = -EFAULT;
		goto err_put_clk;
	}

	r = platform_get_resource_byname(pdev, IORESOURCE_MEM, "capregs");
	if (!r) {
		dev_err(&pdev->dev, "no I/O memory resource defined\n");
		retval = -ENODEV;
		goto err_iounmap_phyreg;
	}

	ehci_mv->cap_regs = ioremap(r->start, resource_size(r));
	if (ehci_mv->cap_regs == NULL) {
		dev_err(&pdev->dev, "failed to map I/O memory\n");
		retval = -EFAULT;
		goto err_iounmap_phyreg;
=======
	hcd = usb_create_hcd(&ehci_platform_hc_driver, &pdev->dev, dev_name(&pdev->dev));
	if (!hcd)
		return -ENOMEM;

	platform_set_drvdata(pdev, hcd);
	ehci_mv = hcd_to_ehci_hcd_mv(hcd);

	ehci_mv->mode = MV_USB_MODE_HOST;
	if (pdata) {
		ehci_mv->mode = pdata->mode;
		ehci_mv->set_vbus = pdata->set_vbus;
	}

	ehci_mv->phy = devm_phy_optional_get(&pdev->dev, "usb");
	if (IS_ERR(ehci_mv->phy)) {
		retval = PTR_ERR(ehci_mv->phy);
		if (retval != -EPROBE_DEFER)
			dev_err(&pdev->dev, "Failed to get phy.\n");
		goto err_put_hcd;
	}

	ehci_mv->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(ehci_mv->clk)) {
		dev_err(&pdev->dev, "error getting clock\n");
		retval = PTR_ERR(ehci_mv->clk);
		goto err_put_hcd;
	}

	ehci_mv->base = devm_platform_get_and_ioremap_resource(pdev, 0, &r);
	if (IS_ERR(ehci_mv->base)) {
		retval = PTR_ERR(ehci_mv->base);
		goto err_put_hcd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	retval = mv_ehci_enable(ehci_mv);
	if (retval) {
		dev_err(&pdev->dev, "init phy error %d\n", retval);
<<<<<<< HEAD
		goto err_iounmap_capreg;
	}

=======
		goto err_put_hcd;
	}

	ehci_mv->cap_regs =
		(void __iomem *) ((unsigned long) ehci_mv->base + U2x_CAPREGS_OFFSET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	offset = readl(ehci_mv->cap_regs) & CAPLENGTH_MASK;
	ehci_mv->op_regs =
		(void __iomem *) ((unsigned long) ehci_mv->cap_regs + offset);

	hcd->rsrc_start = r->start;
<<<<<<< HEAD
	hcd->rsrc_len = r->end - r->start + 1;
	hcd->regs = ehci_mv->op_regs;

	hcd->irq = platform_get_irq(pdev, 0);
	if (!hcd->irq) {
		dev_err(&pdev->dev, "Cannot get irq.");
		retval = -ENODEV;
		goto err_disable_clk;
	}

	ehci = hcd_to_ehci(hcd);
	ehci->caps = (struct ehci_caps *) ehci_mv->cap_regs;
	ehci->regs = (struct ehci_regs *) ehci_mv->op_regs;
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	ehci_mv->mode = pdata->mode;
	if (ehci_mv->mode == MV_USB_MODE_OTG) {
#ifdef CONFIG_USB_OTG_UTILS
		ehci_mv->otg = usb_get_transceiver();
		if (!ehci_mv->otg) {
			dev_err(&pdev->dev,
				"unable to find transceiver\n");
			retval = -ENODEV;
=======
	hcd->rsrc_len = resource_size(r);
	hcd->regs = ehci_mv->op_regs;

	retval = platform_get_irq(pdev, 0);
	if (retval < 0)
		goto err_disable_clk;
	hcd->irq = retval;

	ehci = hcd_to_ehci(hcd);
	ehci->caps = (struct ehci_caps __iomem *) ehci_mv->cap_regs;

	if (ehci_mv->mode == MV_USB_MODE_OTG) {
		ehci_mv->otg = devm_usb_get_phy(&pdev->dev, USB_PHY_TYPE_USB2);
		if (IS_ERR(ehci_mv->otg)) {
			retval = PTR_ERR(ehci_mv->otg);

			if (retval == -ENXIO)
				dev_info(&pdev->dev, "MV_USB_MODE_OTG "
						"must have CONFIG_USB_PHY enabled\n");
			else
				dev_err(&pdev->dev,
						"unable to find transceiver\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto err_disable_clk;
		}

		retval = otg_set_host(ehci_mv->otg->otg, &hcd->self);
		if (retval < 0) {
			dev_err(&pdev->dev,
				"unable to register with transceiver\n");
			retval = -ENODEV;
<<<<<<< HEAD
			goto err_put_transceiver;
		}
		/* otg will enable clock before use as host */
		mv_ehci_disable(ehci_mv);
#else
		dev_info(&pdev->dev, "MV_USB_MODE_OTG "
			 "must have CONFIG_USB_OTG_UTILS enabled\n");
		goto err_disable_clk;
#endif
	} else {
		if (pdata->set_vbus)
			pdata->set_vbus(1);
=======
			goto err_disable_clk;
		}
		/* otg will enable clock before use as host */
		mv_ehci_disable(ehci_mv);
	} else {
		if (ehci_mv->set_vbus)
			ehci_mv->set_vbus(1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		retval = usb_add_hcd(hcd, hcd->irq, IRQF_SHARED);
		if (retval) {
			dev_err(&pdev->dev,
				"failed to add hcd with err %d\n", retval);
			goto err_set_vbus;
		}
<<<<<<< HEAD
	}

	if (pdata->private_init)
		pdata->private_init(ehci_mv->op_regs, ehci_mv->phy_regs);
=======
		device_wakeup_enable(hcd->self.controller);
	}

	if (of_usb_get_phy_mode(pdev->dev.of_node) == USBPHY_INTERFACE_MODE_HSIC) {
		status = ehci_readl(ehci, &ehci->regs->port_status[0]);
		/* These "reserved" bits actually enable HSIC mode. */
		status |= BIT(25);
		status &= ~GENMASK(31, 30);
		ehci_writel(ehci, status, &ehci->regs->port_status[0]);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_info(&pdev->dev,
		 "successful find EHCI device with regs 0x%p irq %d"
		 " working in %s mode\n", hcd->regs, hcd->irq,
		 ehci_mv->mode == MV_USB_MODE_OTG ? "OTG" : "Host");

	return 0;

err_set_vbus:
<<<<<<< HEAD
	if (pdata->set_vbus)
		pdata->set_vbus(0);
#ifdef CONFIG_USB_OTG_UTILS
err_put_transceiver:
	if (ehci_mv->otg)
		usb_put_transceiver(ehci_mv->otg);
#endif
err_disable_clk:
	mv_ehci_disable(ehci_mv);
err_iounmap_capreg:
	iounmap(ehci_mv->cap_regs);
err_iounmap_phyreg:
	iounmap(ehci_mv->phy_regs);
err_put_clk:
	for (clk_i--; clk_i >= 0; clk_i--)
		clk_put(ehci_mv->clk[clk_i]);
	platform_set_drvdata(pdev, NULL);
	kfree(ehci_mv);
=======
	if (ehci_mv->set_vbus)
		ehci_mv->set_vbus(0);
err_disable_clk:
	mv_ehci_disable(ehci_mv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_put_hcd:
	usb_put_hcd(hcd);

	return retval;
}

<<<<<<< HEAD
static int mv_ehci_remove(struct platform_device *pdev)
{
	struct ehci_hcd_mv *ehci_mv = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_mv->hcd;
	int clk_i;
=======
static void mv_ehci_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ehci_hcd_mv *ehci_mv = hcd_to_ehci_hcd_mv(hcd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (hcd->rh_registered)
		usb_remove_hcd(hcd);

<<<<<<< HEAD
	if (ehci_mv->otg) {
		otg_set_host(ehci_mv->otg->otg, NULL);
		usb_put_transceiver(ehci_mv->otg);
	}

	if (ehci_mv->mode == MV_USB_MODE_HOST) {
		if (ehci_mv->pdata->set_vbus)
			ehci_mv->pdata->set_vbus(0);
=======
	if (!IS_ERR_OR_NULL(ehci_mv->otg))
		otg_set_host(ehci_mv->otg->otg, NULL);

	if (ehci_mv->mode == MV_USB_MODE_HOST) {
		if (ehci_mv->set_vbus)
			ehci_mv->set_vbus(0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		mv_ehci_disable(ehci_mv);
	}

<<<<<<< HEAD
	iounmap(ehci_mv->cap_regs);
	iounmap(ehci_mv->phy_regs);

	for (clk_i = 0; clk_i < ehci_mv->clknum; clk_i++)
		clk_put(ehci_mv->clk[clk_i]);

	platform_set_drvdata(pdev, NULL);

	kfree(ehci_mv);
	usb_put_hcd(hcd);

	return 0;
}

MODULE_ALIAS("mv-ehci");

static const struct platform_device_id ehci_id_table[] = {
	{"pxa-u2oehci", PXA_U2OEHCI},
	{"pxa-sph", PXA_SPH},
	{"mmp3-hsic", MMP3_HSIC},
	{"mmp3-fsic", MMP3_FSIC},
=======
	usb_put_hcd(hcd);
}

static const struct platform_device_id ehci_id_table[] = {
	{"pxa-u2oehci", 0},
	{"pxa-sph", 0},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{},
};

static void mv_ehci_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct ehci_hcd_mv *ehci_mv = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_mv->hcd;
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!hcd->rh_registered)
		return;

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);
}

<<<<<<< HEAD
static struct platform_driver ehci_mv_driver = {
	.probe = mv_ehci_probe,
	.remove = mv_ehci_remove,
	.shutdown = mv_ehci_shutdown,
	.driver = {
		   .name = "mv-ehci",
		   .bus = &platform_bus_type,
		   },
	.id_table = ehci_id_table,
};
=======
static const struct of_device_id ehci_mv_dt_ids[] = {
	{ .compatible = "marvell,pxau2o-ehci", },
	{},
};

static struct platform_driver ehci_mv_driver = {
	.probe = mv_ehci_probe,
	.remove_new = mv_ehci_remove,
	.shutdown = mv_ehci_shutdown,
	.driver = {
		.name = "mv-ehci",
		.bus = &platform_bus_type,
		.of_match_table = ehci_mv_dt_ids,
	},
	.id_table = ehci_id_table,
};

static int __init ehci_platform_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	ehci_init_driver(&ehci_platform_hc_driver, &platform_overrides);
	return platform_driver_register(&ehci_mv_driver);
}
module_init(ehci_platform_init);

static void __exit ehci_platform_cleanup(void)
{
	platform_driver_unregister(&ehci_mv_driver);
}
module_exit(ehci_platform_cleanup);

MODULE_DESCRIPTION("Marvell EHCI driver");
MODULE_AUTHOR("Chao Xie <chao.xie@marvell.com>");
MODULE_AUTHOR("Neil Zhang <zhangwm@marvell.com>");
MODULE_ALIAS("mv-ehci");
MODULE_LICENSE("GPL");
MODULE_DEVICE_TABLE(of, ehci_mv_dt_ids);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
