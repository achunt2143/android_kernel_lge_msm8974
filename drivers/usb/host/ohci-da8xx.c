<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * OHCI HCD (Host Controller Driver) for USB.
 *
 * TI DA8xx (OMAP-L1x) Bus Glue
 *
 * Derived from: ohci-omap.c and ohci-s3c2410.c
 * Copyright (C) 2008-2009 MontaVista Software, Inc. <source@mvista.com>
<<<<<<< HEAD
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

#include <mach/da8xx.h>
#include <mach/usb.h>

#ifndef CONFIG_ARCH_DAVINCI_DA8XX
#error "This file is DA8xx bus glue.  Define CONFIG_ARCH_DAVINCI_DA8XX."
#endif

#define CFGCHIP2	DA8XX_SYSCFG0_VIRT(DA8XX_CFGCHIP2_REG)

static struct clk *usb11_clk;
static struct clk *usb20_clk;
=======
 */

#include <linux/clk.h>
#include <linux/gpio/consumer.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/phy/phy.h>
#include <linux/platform_data/usb-davinci.h>
#include <linux/regulator/consumer.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <asm/unaligned.h>

#include "ohci.h"

#define DRIVER_DESC "DA8XX"
#define DRV_NAME "ohci-da8xx"

static struct hc_driver __read_mostly ohci_da8xx_hc_driver;

static int (*orig_ohci_hub_control)(struct usb_hcd  *hcd, u16 typeReq,
			u16 wValue, u16 wIndex, char *buf, u16 wLength);
static int (*orig_ohci_hub_status_data)(struct usb_hcd *hcd, char *buf);

struct da8xx_ohci_hcd {
	struct usb_hcd *hcd;
	struct clk *usb11_clk;
	struct phy *usb11_phy;
	struct regulator *vbus_reg;
	struct notifier_block nb;
	struct gpio_desc *oc_gpio;
};

#define to_da8xx_ohci(hcd) (struct da8xx_ohci_hcd *)(hcd_to_ohci(hcd)->priv)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Over-current indicator change bitmask */
static volatile u16 ocic_mask;

<<<<<<< HEAD
static void ohci_da8xx_clock(int on)
{
	u32 cfgchip2;

	cfgchip2 = __raw_readl(CFGCHIP2);
	if (on) {
		clk_enable(usb11_clk);

		/*
		 * If USB 1.1 reference clock is sourced from USB 2.0 PHY, we
		 * need to enable the USB 2.0 module clocking, start its PHY,
		 * and not allow it to stop the clock during USB 2.0 suspend.
		 */
		if (!(cfgchip2 & CFGCHIP2_USB1PHYCLKMUX)) {
			clk_enable(usb20_clk);

			cfgchip2 &= ~(CFGCHIP2_RESET | CFGCHIP2_PHYPWRDN);
			cfgchip2 |= CFGCHIP2_PHY_PLLON;
			__raw_writel(cfgchip2, CFGCHIP2);

			pr_info("Waiting for USB PHY clock good...\n");
			while (!(__raw_readl(CFGCHIP2) & CFGCHIP2_PHYCLKGD))
				cpu_relax();
		}

		/* Enable USB 1.1 PHY */
		cfgchip2 |= CFGCHIP2_USB1SUSPENDM;
	} else {
		clk_disable(usb11_clk);
		if (!(cfgchip2 & CFGCHIP2_USB1PHYCLKMUX))
			clk_disable(usb20_clk);

		/* Disable USB 1.1 PHY */
		cfgchip2 &= ~CFGCHIP2_USB1SUSPENDM;
	}
	__raw_writel(cfgchip2, CFGCHIP2);
}

/*
 * Handle the port over-current indicator change.
 */
static void ohci_da8xx_ocic_handler(struct da8xx_ohci_root_hub *hub,
				    unsigned port)
{
	ocic_mask |= 1 << port;

	/* Once over-current is detected, the port needs to be powered down */
	if (hub->get_oci(port) > 0)
		hub->set_power(port, 0);
}

static int ohci_da8xx_init(struct usb_hcd *hcd)
{
	struct device *dev		= hcd->self.controller;
	struct da8xx_ohci_root_hub *hub	= dev->platform_data;
=======
static int ohci_da8xx_enable(struct usb_hcd *hcd)
{
	struct da8xx_ohci_hcd *da8xx_ohci = to_da8xx_ohci(hcd);
	int ret;

	ret = clk_prepare_enable(da8xx_ohci->usb11_clk);
	if (ret)
		return ret;

	ret = phy_init(da8xx_ohci->usb11_phy);
	if (ret)
		goto err_phy_init;

	ret = phy_power_on(da8xx_ohci->usb11_phy);
	if (ret)
		goto err_phy_power_on;

	return 0;

err_phy_power_on:
	phy_exit(da8xx_ohci->usb11_phy);
err_phy_init:
	clk_disable_unprepare(da8xx_ohci->usb11_clk);

	return ret;
}

static void ohci_da8xx_disable(struct usb_hcd *hcd)
{
	struct da8xx_ohci_hcd *da8xx_ohci = to_da8xx_ohci(hcd);

	phy_power_off(da8xx_ohci->usb11_phy);
	phy_exit(da8xx_ohci->usb11_phy);
	clk_disable_unprepare(da8xx_ohci->usb11_clk);
}

static int ohci_da8xx_set_power(struct usb_hcd *hcd, int on)
{
	struct da8xx_ohci_hcd *da8xx_ohci = to_da8xx_ohci(hcd);
	struct device *dev = hcd->self.controller;
	int ret;

	if (!da8xx_ohci->vbus_reg)
		return 0;

	if (on) {
		ret = regulator_enable(da8xx_ohci->vbus_reg);
		if (ret) {
			dev_err(dev, "Failed to enable regulator: %d\n", ret);
			return ret;
		}
	} else {
		ret = regulator_disable(da8xx_ohci->vbus_reg);
		if (ret) {
			dev_err(dev, "Failed  to disable regulator: %d\n", ret);
			return ret;
		}
	}

	return 0;
}

static int ohci_da8xx_get_power(struct usb_hcd *hcd)
{
	struct da8xx_ohci_hcd *da8xx_ohci = to_da8xx_ohci(hcd);

	if (da8xx_ohci->vbus_reg)
		return regulator_is_enabled(da8xx_ohci->vbus_reg);

	return 1;
}

static int ohci_da8xx_get_oci(struct usb_hcd *hcd)
{
	struct da8xx_ohci_hcd *da8xx_ohci = to_da8xx_ohci(hcd);
	unsigned int flags;
	int ret;

	if (da8xx_ohci->oc_gpio)
		return gpiod_get_value_cansleep(da8xx_ohci->oc_gpio);

	if (!da8xx_ohci->vbus_reg)
		return 0;

	ret = regulator_get_error_flags(da8xx_ohci->vbus_reg, &flags);
	if (ret)
		return ret;

	if (flags & REGULATOR_ERROR_OVER_CURRENT)
		return 1;

	return 0;
}

static int ohci_da8xx_has_set_power(struct usb_hcd *hcd)
{
	struct da8xx_ohci_hcd *da8xx_ohci = to_da8xx_ohci(hcd);

	if (da8xx_ohci->vbus_reg)
		return 1;

	return 0;
}

static int ohci_da8xx_has_oci(struct usb_hcd *hcd)
{
	struct da8xx_ohci_hcd *da8xx_ohci = to_da8xx_ohci(hcd);

	if (da8xx_ohci->oc_gpio)
		return 1;

	if (da8xx_ohci->vbus_reg)
		return 1;

	return 0;
}

static int ohci_da8xx_has_potpgt(struct usb_hcd *hcd)
{
	struct device *dev		= hcd->self.controller;
	struct da8xx_ohci_root_hub *hub	= dev_get_platdata(dev);

	if (hub && hub->potpgt)
		return 1;

	return 0;
}

static int ohci_da8xx_regulator_event(struct notifier_block *nb,
				unsigned long event, void *data)
{
	struct da8xx_ohci_hcd *da8xx_ohci =
				container_of(nb, struct da8xx_ohci_hcd, nb);

	if (event & REGULATOR_EVENT_OVER_CURRENT) {
		ocic_mask |= 1 << 1;
		ohci_da8xx_set_power(da8xx_ohci->hcd, 0);
	}

	return 0;
}

static irqreturn_t ohci_da8xx_oc_thread(int irq, void *data)
{
	struct da8xx_ohci_hcd *da8xx_ohci = data;
	struct device *dev = da8xx_ohci->hcd->self.controller;
	int ret;

	if (gpiod_get_value_cansleep(da8xx_ohci->oc_gpio) &&
	    da8xx_ohci->vbus_reg) {
		ret = regulator_disable(da8xx_ohci->vbus_reg);
		if (ret)
			dev_err(dev, "Failed to disable regulator: %d\n", ret);
	}

	return IRQ_HANDLED;
}

static int ohci_da8xx_register_notify(struct usb_hcd *hcd)
{
	struct da8xx_ohci_hcd *da8xx_ohci = to_da8xx_ohci(hcd);
	struct device *dev		= hcd->self.controller;
	int ret = 0;

	if (!da8xx_ohci->oc_gpio && da8xx_ohci->vbus_reg) {
		da8xx_ohci->nb.notifier_call = ohci_da8xx_regulator_event;
		ret = devm_regulator_register_notifier(da8xx_ohci->vbus_reg,
						&da8xx_ohci->nb);
	}

	if (ret)
		dev_err(dev, "Failed to register notifier: %d\n", ret);

	return ret;
}

static int ohci_da8xx_reset(struct usb_hcd *hcd)
{
	struct device *dev		= hcd->self.controller;
	struct da8xx_ohci_root_hub *hub	= dev_get_platdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ohci_hcd	*ohci		= hcd_to_ohci(hcd);
	int result;
	u32 rh_a;

	dev_dbg(dev, "starting USB controller\n");

<<<<<<< HEAD
	ohci_da8xx_clock(1);
=======
	result = ohci_da8xx_enable(hcd);
	if (result < 0)
		return result;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * DA8xx only have 1 port connected to the pins but the HC root hub
	 * register A reports 2 ports, thus we'll have to override it...
	 */
	ohci->num_ports = 1;

<<<<<<< HEAD
	result = ohci_init(ohci);
	if (result < 0)
		return result;
=======
	result = ohci_setup(hcd);
	if (result < 0) {
		ohci_da8xx_disable(hcd);
		return result;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Since we're providing a board-specific root hub port power control
	 * and over-current reporting, we have to override the HC root hub A
	 * register's default value, so that ohci_hub_control() could return
	 * the correct hub descriptor...
	 */
	rh_a = ohci_readl(ohci, &ohci->regs->roothub.a);
<<<<<<< HEAD
	if (hub->set_power) {
		rh_a &= ~RH_A_NPS;
		rh_a |=  RH_A_PSM;
	}
	if (hub->get_oci) {
		rh_a &= ~RH_A_NOCP;
		rh_a |=  RH_A_OCPM;
	}
	rh_a &= ~RH_A_POTPGT;
	rh_a |= hub->potpgt << 24;
=======
	if (ohci_da8xx_has_set_power(hcd)) {
		rh_a &= ~RH_A_NPS;
		rh_a |=  RH_A_PSM;
	}
	if (ohci_da8xx_has_oci(hcd)) {
		rh_a &= ~RH_A_NOCP;
		rh_a |=  RH_A_OCPM;
	}
	if (ohci_da8xx_has_potpgt(hcd)) {
		rh_a &= ~RH_A_POTPGT;
		rh_a |= hub->potpgt << 24;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ohci_writel(ohci, rh_a, &ohci->regs->roothub.a);

	return result;
}

<<<<<<< HEAD
static void ohci_da8xx_stop(struct usb_hcd *hcd)
{
	ohci_stop(hcd);
	ohci_da8xx_clock(0);
}

static int ohci_da8xx_start(struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci		= hcd_to_ohci(hcd);
	int result;

	result = ohci_run(ohci);
	if (result < 0)
		ohci_da8xx_stop(hcd);

	return result;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Update the status data from the hub with the over-current indicator change.
 */
static int ohci_da8xx_hub_status_data(struct usb_hcd *hcd, char *buf)
{
<<<<<<< HEAD
	int length		= ohci_hub_status_data(hcd, buf);
=======
	int length		= orig_ohci_hub_status_data(hcd, buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* See if we have OCIC bit set on port 1 */
	if (ocic_mask & (1 << 1)) {
		dev_dbg(hcd->self.controller, "over-current indicator change "
			"on port 1\n");

		if (!length)
			length = 1;

		buf[0] |= 1 << 1;
	}
	return length;
}

/*
 * Look at the control requests to the root hub and see if we need to override.
 */
static int ohci_da8xx_hub_control(struct usb_hcd *hcd, u16 typeReq, u16 wValue,
				  u16 wIndex, char *buf, u16 wLength)
{
	struct device *dev		= hcd->self.controller;
<<<<<<< HEAD
	struct da8xx_ohci_root_hub *hub	= dev->platform_data;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int temp;

	switch (typeReq) {
	case GetPortStatus:
		/* Check the port number */
		if (wIndex != 1)
			break;

		dev_dbg(dev, "GetPortStatus(%u)\n", wIndex);

		temp = roothub_portstatus(hcd_to_ohci(hcd), wIndex - 1);

		/* The port power status (PPS) bit defaults to 1 */
<<<<<<< HEAD
		if (hub->get_power && hub->get_power(wIndex) == 0)
			temp &= ~RH_PS_PPS;

		/* The port over-current indicator (POCI) bit is always 0 */
		if (hub->get_oci && hub->get_oci(wIndex) > 0)
=======
		if (!ohci_da8xx_get_power(hcd))
			temp &= ~RH_PS_PPS;

		/* The port over-current indicator (POCI) bit is always 0 */
		if (ohci_da8xx_get_oci(hcd) > 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			temp |=  RH_PS_POCI;

		/* The over-current indicator change (OCIC) bit is 0 too */
		if (ocic_mask & (1 << wIndex))
			temp |=  RH_PS_OCIC;

		put_unaligned(cpu_to_le32(temp), (__le32 *)buf);
		return 0;
	case SetPortFeature:
		temp = 1;
		goto check_port;
	case ClearPortFeature:
		temp = 0;

check_port:
		/* Check the port number */
		if (wIndex != 1)
			break;

		switch (wValue) {
		case USB_PORT_FEAT_POWER:
			dev_dbg(dev, "%sPortFeature(%u): %s\n",
				temp ? "Set" : "Clear", wIndex, "POWER");

<<<<<<< HEAD
			if (!hub->set_power)
				return -EPIPE;

			return hub->set_power(wIndex, temp) ? -EPIPE : 0;
=======
			return ohci_da8xx_set_power(hcd, temp) ? -EPIPE : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case USB_PORT_FEAT_C_OVER_CURRENT:
			dev_dbg(dev, "%sPortFeature(%u): %s\n",
				temp ? "Set" : "Clear", wIndex,
				"C_OVER_CURRENT");

			if (temp)
				ocic_mask |= 1 << wIndex;
			else
				ocic_mask &= ~(1 << wIndex);
			return 0;
		}
	}

<<<<<<< HEAD
	return ohci_hub_control(hcd, typeReq, wValue, wIndex, buf, wLength);
}

static const struct hc_driver ohci_da8xx_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "DA8xx OHCI",
	.hcd_priv_size		= sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq			= ohci_irq,
	.flags			= HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.reset			= ohci_da8xx_init,
	.start			= ohci_da8xx_start,
	.stop			= ohci_da8xx_stop,
	.shutdown		= ohci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue		= ohci_urb_enqueue,
	.urb_dequeue		= ohci_urb_dequeue,
	.endpoint_disable	= ohci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number	= ohci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data	= ohci_da8xx_hub_status_data,
	.hub_control		= ohci_da8xx_hub_control,

#ifdef	CONFIG_PM
	.bus_suspend		= ohci_bus_suspend,
	.bus_resume		= ohci_bus_resume,
#endif
	.start_port_reset	= ohci_start_port_reset,
};

/*-------------------------------------------------------------------------*/


/**
 * usb_hcd_da8xx_probe - initialize DA8xx-based HCDs
 * Context: !in_interrupt()
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 */
static int usb_hcd_da8xx_probe(const struct hc_driver *driver,
			       struct platform_device *pdev)
{
	struct da8xx_ohci_root_hub *hub	= pdev->dev.platform_data;
	struct usb_hcd	*hcd;
	struct resource *mem;
	int error, irq;

	if (hub == NULL)
		return -ENODEV;

	usb11_clk = clk_get(&pdev->dev, "usb11");
	if (IS_ERR(usb11_clk))
		return PTR_ERR(usb11_clk);

	usb20_clk = clk_get(&pdev->dev, "usb20");
	if (IS_ERR(usb20_clk)) {
		error = PTR_ERR(usb20_clk);
		goto err0;
	}

	hcd = usb_create_hcd(driver, &pdev->dev, dev_name(&pdev->dev));
	if (!hcd) {
		error = -ENOMEM;
		goto err1;
	}

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem) {
		error = -ENODEV;
		goto err2;
=======
	return orig_ohci_hub_control(hcd, typeReq, wValue,
			wIndex, buf, wLength);
}

/*-------------------------------------------------------------------------*/
#ifdef CONFIG_OF
static const struct of_device_id da8xx_ohci_ids[] = {
	{ .compatible = "ti,da830-ohci" },
	{ }
};
MODULE_DEVICE_TABLE(of, da8xx_ohci_ids);
#endif

static int ohci_da8xx_probe(struct platform_device *pdev)
{
	struct da8xx_ohci_hcd *da8xx_ohci;
	struct device *dev = &pdev->dev;
	int error, hcd_irq, oc_irq;
	struct usb_hcd	*hcd;
	struct resource *mem;

	hcd = usb_create_hcd(&ohci_da8xx_hc_driver, dev, dev_name(dev));
	if (!hcd)
		return -ENOMEM;

	da8xx_ohci = to_da8xx_ohci(hcd);
	da8xx_ohci->hcd = hcd;

	da8xx_ohci->usb11_clk = devm_clk_get(dev, NULL);
	if (IS_ERR(da8xx_ohci->usb11_clk)) {
		error = PTR_ERR(da8xx_ohci->usb11_clk);
		if (error != -EPROBE_DEFER)
			dev_err(dev, "Failed to get clock.\n");
		goto err;
	}

	da8xx_ohci->usb11_phy = devm_phy_get(dev, "usb-phy");
	if (IS_ERR(da8xx_ohci->usb11_phy)) {
		error = PTR_ERR(da8xx_ohci->usb11_phy);
		if (error != -EPROBE_DEFER)
			dev_err(dev, "Failed to get phy.\n");
		goto err;
	}

	da8xx_ohci->vbus_reg = devm_regulator_get_optional(dev, "vbus");
	if (IS_ERR(da8xx_ohci->vbus_reg)) {
		error = PTR_ERR(da8xx_ohci->vbus_reg);
		if (error == -ENODEV) {
			da8xx_ohci->vbus_reg = NULL;
		} else if (error == -EPROBE_DEFER) {
			goto err;
		} else {
			dev_err(dev, "Failed to get regulator\n");
			goto err;
		}
	}

	da8xx_ohci->oc_gpio = devm_gpiod_get_optional(dev, "oc", GPIOD_IN);
	if (IS_ERR(da8xx_ohci->oc_gpio)) {
		error = PTR_ERR(da8xx_ohci->oc_gpio);
		goto err;
	}

	if (da8xx_ohci->oc_gpio) {
		oc_irq = gpiod_to_irq(da8xx_ohci->oc_gpio);
		if (oc_irq < 0) {
			error = oc_irq;
			goto err;
		}

		error = devm_request_threaded_irq(dev, oc_irq, NULL,
				ohci_da8xx_oc_thread, IRQF_TRIGGER_RISING |
				IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
				"OHCI over-current indicator", da8xx_ohci);
		if (error)
			goto err;
	}

	hcd->regs = devm_platform_get_and_ioremap_resource(pdev, 0, &mem);
	if (IS_ERR(hcd->regs)) {
		error = PTR_ERR(hcd->regs);
		goto err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	hcd->rsrc_start = mem->start;
	hcd->rsrc_len = resource_size(mem);

<<<<<<< HEAD
	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		dev_dbg(&pdev->dev, "request_mem_region failed\n");
		error = -EBUSY;
		goto err2;
	}

	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (!hcd->regs) {
		dev_err(&pdev->dev, "ioremap failed\n");
		error = -ENOMEM;
		goto err3;
	}

	ohci_hcd_init(hcd_to_ohci(hcd));

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		error = -ENODEV;
		goto err4;
	}
	error = usb_add_hcd(hcd, irq, 0);
	if (error)
		goto err4;

	if (hub->ocic_notify) {
		error = hub->ocic_notify(ohci_da8xx_ocic_handler);
		if (!error)
			return 0;
	}

	usb_remove_hcd(hcd);
err4:
	iounmap(hcd->regs);
err3:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err2:
	usb_put_hcd(hcd);
err1:
	clk_put(usb20_clk);
err0:
	clk_put(usb11_clk);
	return error;
}

/**
 * usb_hcd_da8xx_remove - shutdown processing for DA8xx-based HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_hcd_da8xx_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 */
static inline void
usb_hcd_da8xx_remove(struct usb_hcd *hcd, struct platform_device *pdev)
{
	struct da8xx_ohci_root_hub *hub	= pdev->dev.platform_data;

	hub->ocic_notify(NULL);
	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
	clk_put(usb20_clk);
	clk_put(usb11_clk);
}

static int ohci_hcd_da8xx_drv_probe(struct platform_device *dev)
{
	return usb_hcd_da8xx_probe(&ohci_da8xx_hc_driver, dev);
}

static int ohci_hcd_da8xx_drv_remove(struct platform_device *dev)
{
	struct usb_hcd	*hcd = platform_get_drvdata(dev);

	usb_hcd_da8xx_remove(hcd, dev);
	platform_set_drvdata(dev, NULL);

	return 0;
}

#ifdef CONFIG_PM
static int ohci_da8xx_suspend(struct platform_device *dev, pm_message_t message)
{
	struct usb_hcd	*hcd	= platform_get_drvdata(dev);
	struct ohci_hcd	*ohci	= hcd_to_ohci(hcd);
=======
	hcd_irq = platform_get_irq(pdev, 0);
	if (hcd_irq < 0) {
		error = -ENODEV;
		goto err;
	}

	error = usb_add_hcd(hcd, hcd_irq, 0);
	if (error)
		goto err;

	device_wakeup_enable(hcd->self.controller);

	error = ohci_da8xx_register_notify(hcd);
	if (error)
		goto err_remove_hcd;

	return 0;

err_remove_hcd:
	usb_remove_hcd(hcd);
err:
	usb_put_hcd(hcd);
	return error;
}

static void ohci_da8xx_remove(struct platform_device *pdev)
{
	struct usb_hcd	*hcd = platform_get_drvdata(pdev);

	usb_remove_hcd(hcd);
	usb_put_hcd(hcd);
}

#ifdef CONFIG_PM
static int ohci_da8xx_suspend(struct platform_device *pdev,
				pm_message_t message)
{
	struct usb_hcd	*hcd	= platform_get_drvdata(pdev);
	struct ohci_hcd	*ohci	= hcd_to_ohci(hcd);
	bool		do_wakeup	= device_may_wakeup(&pdev->dev);
	int		ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (time_before(jiffies, ohci->next_statechange))
		msleep(5);
	ohci->next_statechange = jiffies;

<<<<<<< HEAD
	ohci_da8xx_clock(0);
	hcd->state = HC_STATE_SUSPENDED;
	dev->dev.power.power_state = PMSG_SUSPEND;
	return 0;
=======
	ret = ohci_suspend(hcd, do_wakeup);
	if (ret)
		return ret;

	ohci_da8xx_disable(hcd);
	hcd->state = HC_STATE_SUSPENDED;

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ohci_da8xx_resume(struct platform_device *dev)
{
	struct usb_hcd	*hcd	= platform_get_drvdata(dev);
	struct ohci_hcd	*ohci	= hcd_to_ohci(hcd);
<<<<<<< HEAD
=======
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (time_before(jiffies, ohci->next_statechange))
		msleep(5);
	ohci->next_statechange = jiffies;

<<<<<<< HEAD
	ohci_da8xx_clock(1);
	dev->dev.power.power_state = PMSG_ON;
	usb_hcd_resume_root_hub(hcd);
=======
	ret = ohci_da8xx_enable(hcd);
	if (ret)
		return ret;

	ohci_resume(hcd, false);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
#endif

<<<<<<< HEAD
=======
static const struct ohci_driver_overrides da8xx_overrides __initconst = {
	.reset		 = ohci_da8xx_reset,
	.extra_priv_size = sizeof(struct da8xx_ohci_hcd),
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Driver definition to register with platform structure.
 */
static struct platform_driver ohci_hcd_da8xx_driver = {
<<<<<<< HEAD
	.probe		= ohci_hcd_da8xx_drv_probe,
	.remove		= ohci_hcd_da8xx_drv_remove,
=======
	.probe		= ohci_da8xx_probe,
	.remove_new	= ohci_da8xx_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.shutdown 	= usb_hcd_platform_shutdown,
#ifdef	CONFIG_PM
	.suspend	= ohci_da8xx_suspend,
	.resume		= ohci_da8xx_resume,
#endif
	.driver		= {
<<<<<<< HEAD
		.owner	= THIS_MODULE,
		.name	= "ohci",
	},
};
=======
		.name	= DRV_NAME,
		.of_match_table = of_match_ptr(da8xx_ohci_ids),
	},
};

static int __init ohci_da8xx_init(void)
{

	if (usb_disabled())
		return -ENODEV;

	ohci_init_driver(&ohci_da8xx_hc_driver, &da8xx_overrides);

	/*
	 * The Davinci da8xx HW has some unusual quirks, which require
	 * da8xx-specific workarounds. We override certain hc_driver
	 * functions here to achieve that. We explicitly do not enhance
	 * ohci_driver_overrides to allow this more easily, since this
	 * is an unusual case, and we don't want to encourage others to
	 * override these functions by making it too easy.
	 */

	orig_ohci_hub_control = ohci_da8xx_hc_driver.hub_control;
	orig_ohci_hub_status_data = ohci_da8xx_hc_driver.hub_status_data;

	ohci_da8xx_hc_driver.hub_status_data     = ohci_da8xx_hub_status_data;
	ohci_da8xx_hc_driver.hub_control         = ohci_da8xx_hub_control;

	return platform_driver_register(&ohci_hcd_da8xx_driver);
}
module_init(ohci_da8xx_init);

static void __exit ohci_da8xx_exit(void)
{
	platform_driver_unregister(&ohci_hcd_da8xx_driver);
}
module_exit(ohci_da8xx_exit);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
