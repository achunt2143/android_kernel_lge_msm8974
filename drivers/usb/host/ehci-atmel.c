<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Driver for EHCI UHP on Atmel chips
 *
 *  Copyright (C) 2009 Atmel Corporation,
 *                     Nicolas Ferre <nicolas.ferre@atmel.com>
 *
 *  Based on various ehci-*.c drivers
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 */

#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_platform.h>

/* interface and function clocks */
static struct clk *iclk, *fclk;
static int clocked;

/*-------------------------------------------------------------------------*/

static void atmel_start_clock(void)
{
	clk_enable(iclk);
	clk_enable(fclk);
	clocked = 1;
}

static void atmel_stop_clock(void)
{
	clk_disable(fclk);
	clk_disable(iclk);
	clocked = 0;
=======
 */

#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/usb/phy.h>
#include <linux/usb/of.h>

#include "ehci.h"

#define DRIVER_DESC "EHCI Atmel driver"

#define EHCI_INSNREG(index)			((index) * 4 + 0x90)
#define EHCI_INSNREG08_HSIC_EN			BIT(2)

/* interface and function clocks */
#define hcd_to_atmel_ehci_priv(h) \
	((struct atmel_ehci_priv *)hcd_to_ehci(h)->priv)

struct atmel_ehci_priv {
	struct clk *iclk;
	struct clk *uclk;
	bool clocked;
};

static struct hc_driver __read_mostly ehci_atmel_hc_driver;

static const struct ehci_driver_overrides ehci_atmel_drv_overrides __initconst = {
	.extra_priv_size = sizeof(struct atmel_ehci_priv),
};

/*-------------------------------------------------------------------------*/

static void atmel_start_clock(struct atmel_ehci_priv *atmel_ehci)
{
	if (atmel_ehci->clocked)
		return;

	clk_prepare_enable(atmel_ehci->uclk);
	clk_prepare_enable(atmel_ehci->iclk);
	atmel_ehci->clocked = true;
}

static void atmel_stop_clock(struct atmel_ehci_priv *atmel_ehci)
{
	if (!atmel_ehci->clocked)
		return;

	clk_disable_unprepare(atmel_ehci->iclk);
	clk_disable_unprepare(atmel_ehci->uclk);
	atmel_ehci->clocked = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void atmel_start_ehci(struct platform_device *pdev)
{
<<<<<<< HEAD
	dev_dbg(&pdev->dev, "start\n");
	atmel_start_clock();
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct atmel_ehci_priv *atmel_ehci = hcd_to_atmel_ehci_priv(hcd);

	dev_dbg(&pdev->dev, "start\n");
	atmel_start_clock(atmel_ehci);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void atmel_stop_ehci(struct platform_device *pdev)
{
<<<<<<< HEAD
	dev_dbg(&pdev->dev, "stop\n");
	atmel_stop_clock();
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct atmel_ehci_priv *atmel_ehci = hcd_to_atmel_ehci_priv(hcd);

	dev_dbg(&pdev->dev, "stop\n");
	atmel_stop_clock(atmel_ehci);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
static int ehci_atmel_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval = 0;

	/* registers start at offset 0x0 */
	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs +
		HC_LENGTH(ehci, ehci_readl(ehci, &ehci->caps->hc_capbase));
	dbg_hcs_params(ehci, "reset");
	dbg_hcc_params(ehci, "reset");

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	/* data structure init */
	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci->sbrn = 0x20;

	ehci_reset(ehci);
	ehci_port_power(ehci, 0);

	return retval;
}

static const struct hc_driver ehci_atmel_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "Atmel EHCI UHP HS",
	.hcd_priv_size		= sizeof(struct ehci_hcd),

	/* generic hardware linkage */
	.irq			= ehci_irq,
	.flags			= HCD_MEMORY | HCD_USB2,

	/* basic lifecycle operations */
	.reset			= ehci_atmel_setup,
	.start			= ehci_run,
	.stop			= ehci_stop,
	.shutdown		= ehci_shutdown,

	/* managing i/o requests and associated device resources */
	.urb_enqueue		= ehci_urb_enqueue,
	.urb_dequeue		= ehci_urb_dequeue,
	.endpoint_disable	= ehci_endpoint_disable,
	.endpoint_reset		= ehci_endpoint_reset,

	/* scheduling support */
	.get_frame_number	= ehci_get_frame,

	/* root hub support */
	.hub_status_data	= ehci_hub_status_data,
	.hub_control		= ehci_hub_control,
	.bus_suspend		= ehci_bus_suspend,
	.bus_resume		= ehci_bus_resume,
	.relinquish_port	= ehci_relinquish_port,
	.port_handed_over	= ehci_port_handed_over,

	.clear_tt_buffer_complete	= ehci_clear_tt_buffer_complete,
};

static u64 at91_ehci_dma_mask = DMA_BIT_MASK(32);

static int __devinit ehci_atmel_drv_probe(struct platform_device *pdev)
=======
static int ehci_atmel_drv_probe(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct usb_hcd *hcd;
	const struct hc_driver *driver = &ehci_atmel_hc_driver;
	struct resource *res;
<<<<<<< HEAD
=======
	struct ehci_hcd *ehci;
	struct atmel_ehci_priv *atmel_ehci;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int irq;
	int retval;

	if (usb_disabled())
		return -ENODEV;

	pr_debug("Initializing Atmel-SoC USB Host Controller\n");

	irq = platform_get_irq(pdev, 0);
<<<<<<< HEAD
	if (irq <= 0) {
		dev_err(&pdev->dev,
			"Found HC with no IRQ. Check %s setup!\n",
			dev_name(&pdev->dev));
		retval = -ENODEV;
=======
	if (irq < 0) {
		retval = irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail_create_hcd;
	}

	/* Right now device-tree probed devices don't get dma_mask set.
	 * Since shared usb code relies on it, set it here for now.
	 * Once we have dma capability bindings this can go away.
	 */
<<<<<<< HEAD
	if (!pdev->dev.dma_mask)
		pdev->dev.dma_mask = &at91_ehci_dma_mask;
=======
	retval = dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (retval)
		goto fail_create_hcd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hcd = usb_create_hcd(driver, &pdev->dev, dev_name(&pdev->dev));
	if (!hcd) {
		retval = -ENOMEM;
		goto fail_create_hcd;
	}
<<<<<<< HEAD

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev,
			"Found HC with no register addr. Check %s setup!\n",
			dev_name(&pdev->dev));
		retval = -ENODEV;
		goto fail_request_resource;
	}
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len,
				driver->description)) {
		dev_dbg(&pdev->dev, "controller already in use\n");
		retval = -EBUSY;
		goto fail_request_resource;
	}

	hcd->regs = ioremap_nocache(hcd->rsrc_start, hcd->rsrc_len);
	if (hcd->regs == NULL) {
		dev_dbg(&pdev->dev, "error mapping memory\n");
		retval = -EFAULT;
		goto fail_ioremap;
	}

	iclk = clk_get(&pdev->dev, "ehci_clk");
	if (IS_ERR(iclk)) {
		dev_err(&pdev->dev, "Error getting interface clock\n");
		retval = -ENOENT;
		goto fail_get_iclk;
	}
	fclk = clk_get(&pdev->dev, "uhpck");
	if (IS_ERR(fclk)) {
		dev_err(&pdev->dev, "Error getting function clock\n");
		retval = -ENOENT;
		goto fail_get_fclk;
	}
=======
	atmel_ehci = hcd_to_atmel_ehci_priv(hcd);

	hcd->regs = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(hcd->regs)) {
		retval = PTR_ERR(hcd->regs);
		goto fail_request_resource;
	}

	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

	atmel_ehci->iclk = devm_clk_get(&pdev->dev, "ehci_clk");
	if (IS_ERR(atmel_ehci->iclk)) {
		dev_err(&pdev->dev, "Error getting interface clock\n");
		retval = -ENOENT;
		goto fail_request_resource;
	}

	atmel_ehci->uclk = devm_clk_get(&pdev->dev, "usb_clk");
	if (IS_ERR(atmel_ehci->uclk)) {
		dev_err(&pdev->dev, "failed to get uclk\n");
		retval = PTR_ERR(atmel_ehci->uclk);
		goto fail_request_resource;
	}

	ehci = hcd_to_ehci(hcd);
	/* registers start at offset 0x0 */
	ehci->caps = hcd->regs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	atmel_start_ehci(pdev);

	retval = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (retval)
		goto fail_add_hcd;
<<<<<<< HEAD
=======
	device_wakeup_enable(hcd->self.controller);

	if (of_usb_get_phy_mode(pdev->dev.of_node) == USBPHY_INTERFACE_MODE_HSIC)
		writel(EHCI_INSNREG08_HSIC_EN, hcd->regs + EHCI_INSNREG(8));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return retval;

fail_add_hcd:
	atmel_stop_ehci(pdev);
<<<<<<< HEAD
	clk_put(fclk);
fail_get_fclk:
	clk_put(iclk);
fail_get_iclk:
	iounmap(hcd->regs);
fail_ioremap:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
fail_request_resource:
	usb_put_hcd(hcd);
fail_create_hcd:
	dev_err(&pdev->dev, "init %s fail, %d\n",
		dev_name(&pdev->dev), retval);

	return retval;
}

<<<<<<< HEAD
static int __devexit ehci_atmel_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	ehci_shutdown(hcd);
	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);

	atmel_stop_ehci(pdev);
	clk_put(fclk);
	clk_put(iclk);
	fclk = iclk = NULL;

=======
static void ehci_atmel_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	usb_remove_hcd(hcd);
	usb_put_hcd(hcd);

	atmel_stop_ehci(pdev);
}

static int __maybe_unused ehci_atmel_drv_suspend(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct atmel_ehci_priv *atmel_ehci = hcd_to_atmel_ehci_priv(hcd);
	int ret;

	ret = ehci_suspend(hcd, false);
	if (ret)
		return ret;

	atmel_stop_clock(atmel_ehci);
	return 0;
}

static int __maybe_unused ehci_atmel_drv_resume(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct atmel_ehci_priv *atmel_ehci = hcd_to_atmel_ehci_priv(hcd);

	atmel_start_clock(atmel_ehci);
	ehci_resume(hcd, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id atmel_ehci_dt_ids[] = {
	{ .compatible = "atmel,at91sam9g45-ehci" },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, atmel_ehci_dt_ids);
#endif

<<<<<<< HEAD
static struct platform_driver ehci_atmel_driver = {
	.probe		= ehci_atmel_drv_probe,
	.remove		= __devexit_p(ehci_atmel_drv_remove),
	.shutdown	= usb_hcd_platform_shutdown,
	.driver		= {
		.name	= "atmel-ehci",
		.of_match_table	= of_match_ptr(atmel_ehci_dt_ids),
	},
};
=======
static SIMPLE_DEV_PM_OPS(ehci_atmel_pm_ops, ehci_atmel_drv_suspend,
					ehci_atmel_drv_resume);

static struct platform_driver ehci_atmel_driver = {
	.probe		= ehci_atmel_drv_probe,
	.remove_new	= ehci_atmel_drv_remove,
	.shutdown	= usb_hcd_platform_shutdown,
	.driver		= {
		.name	= "atmel-ehci",
		.pm	= &ehci_atmel_pm_ops,
		.of_match_table	= of_match_ptr(atmel_ehci_dt_ids),
	},
};

static int __init ehci_atmel_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	ehci_init_driver(&ehci_atmel_hc_driver, &ehci_atmel_drv_overrides);
	return platform_driver_register(&ehci_atmel_driver);
}
module_init(ehci_atmel_init);

static void __exit ehci_atmel_cleanup(void)
{
	platform_driver_unregister(&ehci_atmel_driver);
}
module_exit(ehci_atmel_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_ALIAS("platform:atmel-ehci");
MODULE_AUTHOR("Nicolas Ferre");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
