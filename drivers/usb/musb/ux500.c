<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2010 ST-Ericsson AB
 * Mian Yousaf Kaukab <mian.yousaf.kaukab@stericsson.com>
 *
 * Based on omap2430.c
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#include <linux/module.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/platform_device.h>

#include "musb_core.h"

=======
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/usb/musb-ux500.h>

#include "musb_core.h"

static const struct musb_hdrc_config ux500_musb_hdrc_config = {
	.multipoint	= true,
	.dyn_fifo	= true,
	.num_eps	= 16,
	.ram_bits	= 16,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct ux500_glue {
	struct device		*dev;
	struct platform_device	*musb;
	struct clk		*clk;
};
#define glue_to_musb(g)	platform_get_drvdata(g->musb)

<<<<<<< HEAD
static int ux500_musb_init(struct musb *musb)
{
	musb->xceiv = usb_get_transceiver();
	if (!musb->xceiv) {
		pr_err("HS USB OTG: no transceiver configured\n");
		return -ENODEV;
	}

=======
static void ux500_musb_set_vbus(struct musb *musb, int is_on)
{
	u8            devctl;
	unsigned long timeout = jiffies + msecs_to_jiffies(1000);
	/* HDRC controls CPEN, but beware current surges during device
	 * connect.  They can trigger transient overcurrent conditions
	 * that must be ignored.
	 */

	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);

	if (is_on) {
		if (musb->xceiv->otg->state == OTG_STATE_A_IDLE) {
			/* start the session */
			devctl |= MUSB_DEVCTL_SESSION;
			musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);
			/*
			 * Wait for the musb to set as A device to enable the
			 * VBUS
			 */
			while (musb_readb(musb->mregs, MUSB_DEVCTL) & 0x80) {

				if (time_after(jiffies, timeout)) {
					dev_err(musb->controller,
					"configured as A device timeout");
					break;
				}
			}

		} else {
			musb->is_active = 1;
			musb->xceiv->otg->state = OTG_STATE_A_WAIT_VRISE;
			devctl |= MUSB_DEVCTL_SESSION;
			MUSB_HST_MODE(musb);
		}
	} else {
		musb->is_active = 0;

		/* NOTE: we're skipping A_WAIT_VFALL -> A_IDLE and jumping
		 * right to B_IDLE...
		 */
		devctl &= ~MUSB_DEVCTL_SESSION;
		MUSB_DEV_MODE(musb);
	}
	musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);

	/*
	 * Devctl values will be updated after vbus goes below
	 * session_valid. The time taken depends on the capacitance
	 * on VBUS line. The max discharge time can be upto 1 sec
	 * as per the spec. Typically on our platform, it is 200ms
	 */
	if (!is_on)
		mdelay(200);

	dev_dbg(musb->controller, "VBUS %s, devctl %02x\n",
		usb_otg_state_string(musb->xceiv->otg->state),
		musb_readb(musb->mregs, MUSB_DEVCTL));
}

static int musb_otg_notifications(struct notifier_block *nb,
		unsigned long event, void *unused)
{
	struct musb *musb = container_of(nb, struct musb, nb);

	dev_dbg(musb->controller, "musb_otg_notifications %ld %s\n",
			event, usb_otg_state_string(musb->xceiv->otg->state));

	switch (event) {
	case UX500_MUSB_ID:
		dev_dbg(musb->controller, "ID GND\n");
		ux500_musb_set_vbus(musb, 1);
		break;
	case UX500_MUSB_VBUS:
		dev_dbg(musb->controller, "VBUS Connect\n");
		break;
	case UX500_MUSB_NONE:
		dev_dbg(musb->controller, "VBUS Disconnect\n");
		if (is_host_active(musb))
			ux500_musb_set_vbus(musb, 0);
		else
			musb->xceiv->otg->state = OTG_STATE_B_IDLE;
		break;
	default:
		dev_dbg(musb->controller, "ID float\n");
		return NOTIFY_DONE;
	}
	return NOTIFY_OK;
}

static irqreturn_t ux500_musb_interrupt(int irq, void *__hci)
{
	unsigned long   flags;
	irqreturn_t     retval = IRQ_NONE;
	struct musb     *musb = __hci;

	spin_lock_irqsave(&musb->lock, flags);

	musb->int_usb = musb_readb(musb->mregs, MUSB_INTRUSB);
	musb->int_tx = musb_readw(musb->mregs, MUSB_INTRTX);
	musb->int_rx = musb_readw(musb->mregs, MUSB_INTRRX);

	if (musb->int_usb || musb->int_tx || musb->int_rx)
		retval = musb_interrupt(musb);

	spin_unlock_irqrestore(&musb->lock, flags);

	return retval;
}

static int ux500_musb_init(struct musb *musb)
{
	int status;

	musb->xceiv = usb_get_phy(USB_PHY_TYPE_USB2);
	if (IS_ERR_OR_NULL(musb->xceiv)) {
		pr_err("HS USB OTG: no transceiver configured\n");
		return -EPROBE_DEFER;
	}

	musb->nb.notifier_call = musb_otg_notifications;
	status = usb_register_notifier(musb->xceiv, &musb->nb);
	if (status < 0) {
		dev_dbg(musb->controller, "notification register failed\n");
		return status;
	}

	musb->isr = ux500_musb_interrupt;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int ux500_musb_exit(struct musb *musb)
{
<<<<<<< HEAD
	usb_put_transceiver(musb->xceiv);
=======
	usb_unregister_notifier(musb->xceiv, &musb->nb);

	usb_put_phy(musb->xceiv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct musb_platform_ops ux500_ops = {
<<<<<<< HEAD
	.init		= ux500_musb_init,
	.exit		= ux500_musb_exit,
};

static int __devinit ux500_probe(struct platform_device *pdev)
{
	struct musb_hdrc_platform_data	*pdata = pdev->dev.platform_data;
	struct platform_device		*musb;
	struct ux500_glue		*glue;
	struct clk			*clk;

	int				ret = -ENOMEM;

	glue = kzalloc(sizeof(*glue), GFP_KERNEL);
	if (!glue) {
		dev_err(&pdev->dev, "failed to allocate glue context\n");
		goto err0;
	}

	musb = platform_device_alloc("musb-hdrc", -1);
	if (!musb) {
		dev_err(&pdev->dev, "failed to allocate musb device\n");
		goto err1;
	}

	clk = clk_get(&pdev->dev, "usb");
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "failed to get clock\n");
		ret = PTR_ERR(clk);
		goto err2;
	}

	ret = clk_enable(clk);
	if (ret) {
		dev_err(&pdev->dev, "failed to enable clock\n");
		goto err3;
	}

	musb->dev.parent		= &pdev->dev;
	musb->dev.dma_mask		= pdev->dev.dma_mask;
	musb->dev.coherent_dma_mask	= pdev->dev.coherent_dma_mask;
=======
	.quirks		= MUSB_DMA_UX500 | MUSB_INDEXED_EP,
#ifdef CONFIG_USB_UX500_DMA
	.dma_init	= ux500_dma_controller_create,
	.dma_exit	= ux500_dma_controller_destroy,
#endif
	.init		= ux500_musb_init,
	.exit		= ux500_musb_exit,
	.fifo_mode	= 5,

	.set_vbus	= ux500_musb_set_vbus,
};

static struct musb_hdrc_platform_data *
ux500_of_probe(struct platform_device *pdev, struct device_node *np)
{
	struct musb_hdrc_platform_data *pdata;
	const char *mode;
	int strlen;

	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return NULL;

	mode = of_get_property(np, "dr_mode", &strlen);
	if (!mode) {
		dev_err(&pdev->dev, "No 'dr_mode' property found\n");
		return NULL;
	}

	if (strlen > 0) {
		if (!strcmp(mode, "host"))
			pdata->mode = MUSB_HOST;
		if (!strcmp(mode, "otg"))
			pdata->mode = MUSB_OTG;
		if (!strcmp(mode, "peripheral"))
			pdata->mode = MUSB_PERIPHERAL;
	}

	return pdata;
}

static int ux500_probe(struct platform_device *pdev)
{
	struct musb_hdrc_platform_data	*pdata = dev_get_platdata(&pdev->dev);
	struct device_node		*np = pdev->dev.of_node;
	struct platform_device		*musb;
	struct ux500_glue		*glue;
	struct clk			*clk;
	int				ret = -ENOMEM;

	if (!pdata) {
		if (np) {
			pdata = ux500_of_probe(pdev, np);
			if (!pdata)
				goto err0;

			pdev->dev.platform_data = pdata;
		} else {
			dev_err(&pdev->dev, "no pdata or device tree found\n");
			goto err0;
		}
	}

	glue = devm_kzalloc(&pdev->dev, sizeof(*glue), GFP_KERNEL);
	if (!glue)
		goto err0;

	musb = platform_device_alloc("musb-hdrc", PLATFORM_DEVID_AUTO);
	if (!musb) {
		dev_err(&pdev->dev, "failed to allocate musb device\n");
		goto err0;
	}

	clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "failed to get clock\n");
		ret = PTR_ERR(clk);
		goto err1;
	}

	ret = clk_prepare_enable(clk);
	if (ret) {
		dev_err(&pdev->dev, "failed to enable clock\n");
		goto err1;
	}

	musb->dev.parent		= &pdev->dev;
	musb->dev.dma_mask		= &pdev->dev.coherent_dma_mask;
	musb->dev.coherent_dma_mask	= pdev->dev.coherent_dma_mask;
	device_set_of_node_from_dev(&musb->dev, &pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	glue->dev			= &pdev->dev;
	glue->musb			= musb;
	glue->clk			= clk;

	pdata->platform_ops		= &ux500_ops;
<<<<<<< HEAD

	platform_set_drvdata(pdev, glue);

	ret = platform_device_add_resources(musb, pdev->resource,
			pdev->num_resources);
	if (ret) {
		dev_err(&pdev->dev, "failed to add resources\n");
		goto err4;
=======
	pdata->config 			= &ux500_musb_hdrc_config;

	platform_set_drvdata(pdev, glue);

	ret = platform_device_add_resources(musb, pdev->resource, pdev->num_resources);
	if (ret) {
		dev_err(&pdev->dev, "failed to add resources\n");
		goto err2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ret = platform_device_add_data(musb, pdata, sizeof(*pdata));
	if (ret) {
		dev_err(&pdev->dev, "failed to add platform_data\n");
<<<<<<< HEAD
		goto err4;
=======
		goto err2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ret = platform_device_add(musb);
	if (ret) {
		dev_err(&pdev->dev, "failed to register musb device\n");
<<<<<<< HEAD
		goto err4;
=======
		goto err2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

<<<<<<< HEAD
err4:
	clk_disable(clk);

err3:
	clk_put(clk);

err2:
	platform_device_put(musb);

err1:
	kfree(glue);
=======
err2:
	clk_disable_unprepare(clk);

err1:
	platform_device_put(musb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

err0:
	return ret;
}

<<<<<<< HEAD
static int __devexit ux500_remove(struct platform_device *pdev)
{
	struct ux500_glue	*glue = platform_get_drvdata(pdev);

	platform_device_del(glue->musb);
	platform_device_put(glue->musb);
	clk_disable(glue->clk);
	clk_put(glue->clk);
	kfree(glue);

	return 0;
}

#ifdef CONFIG_PM
=======
static void ux500_remove(struct platform_device *pdev)
{
	struct ux500_glue	*glue = platform_get_drvdata(pdev);

	platform_device_unregister(glue->musb);
	clk_disable_unprepare(glue->clk);
}

#ifdef CONFIG_PM_SLEEP
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ux500_suspend(struct device *dev)
{
	struct ux500_glue	*glue = dev_get_drvdata(dev);
	struct musb		*musb = glue_to_musb(glue);

<<<<<<< HEAD
	usb_phy_set_suspend(musb->xceiv, 1);
	clk_disable(glue->clk);
=======
	if (musb)
		usb_phy_set_suspend(musb->xceiv, 1);

	clk_disable_unprepare(glue->clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int ux500_resume(struct device *dev)
{
	struct ux500_glue	*glue = dev_get_drvdata(dev);
	struct musb		*musb = glue_to_musb(glue);
	int			ret;

<<<<<<< HEAD
	ret = clk_enable(glue->clk);
=======
	ret = clk_prepare_enable(glue->clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret) {
		dev_err(dev, "failed to enable clock\n");
		return ret;
	}

<<<<<<< HEAD
	usb_phy_set_suspend(musb->xceiv, 0);

	return 0;
}

static const struct dev_pm_ops ux500_pm_ops = {
	.suspend	= ux500_suspend,
	.resume		= ux500_resume,
};

#define DEV_PM_OPS	(&ux500_pm_ops)
#else
#define DEV_PM_OPS	NULL
#endif

static struct platform_driver ux500_driver = {
	.probe		= ux500_probe,
	.remove		= __devexit_p(ux500_remove),
	.driver		= {
		.name	= "musb-ux500",
		.pm	= DEV_PM_OPS,
=======
	if (musb)
		usb_phy_set_suspend(musb->xceiv, 0);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(ux500_pm_ops, ux500_suspend, ux500_resume);

static const struct of_device_id ux500_match[] = {
        { .compatible = "stericsson,db8500-musb", },
        {}
};

MODULE_DEVICE_TABLE(of, ux500_match);

static struct platform_driver ux500_driver = {
	.probe		= ux500_probe,
	.remove_new	= ux500_remove,
	.driver		= {
		.name	= "musb-ux500",
		.pm	= &ux500_pm_ops,
		.of_match_table = ux500_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

MODULE_DESCRIPTION("UX500 MUSB Glue Layer");
MODULE_AUTHOR("Mian Yousaf Kaukab <mian.yousaf.kaukab@stericsson.com>");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD

static int __init ux500_init(void)
{
	return platform_driver_register(&ux500_driver);
}
module_init(ux500_init);

static void __exit ux500_exit(void)
{
	platform_driver_unregister(&ux500_driver);
}
module_exit(ux500_exit);
=======
module_platform_driver(ux500_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
