<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2005-2007 by Texas Instruments
 * Some code has been taken from tusb6010.c
 * Copyrights for that are attributable to:
 * Copyright (C) 2006 Nokia Corporation
 * Tony Lindgren <tony@atomide.com>
 *
 * This file is part of the Inventra Controller Driver for Linux.
<<<<<<< HEAD
 *
 * The Inventra Controller Driver for Linux is free software; you
 * can redistribute it and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation.
 *
 * The Inventra Controller Driver for Linux is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Inventra Controller Driver for Linux ; if not,
 * write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330, Boston, MA  02111-1307  USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/io.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
#include <linux/of_irq.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/pm_runtime.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/delay.h>
#include <linux/usb/musb.h>
#include <linux/phy/omap_control_phy.h>
#include <linux/of_platform.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "musb_core.h"
#include "omap2430.h"

struct omap2430_glue {
	struct device		*dev;
	struct platform_device	*musb;
<<<<<<< HEAD
};
#define glue_to_musb(g)		platform_get_drvdata(g->musb)

static struct timer_list musb_idle_timer;

static void musb_do_idle(unsigned long _musb)
{
	struct musb	*musb = (void *)_musb;
	unsigned long	flags;
	u8	power;
	u8	devctl;

	spin_lock_irqsave(&musb->lock, flags);

	switch (musb->xceiv->state) {
	case OTG_STATE_A_WAIT_BCON:

		devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
		if (devctl & MUSB_DEVCTL_BDEVICE) {
			musb->xceiv->state = OTG_STATE_B_IDLE;
			MUSB_DEV_MODE(musb);
		} else {
			musb->xceiv->state = OTG_STATE_A_IDLE;
			MUSB_HST_MODE(musb);
		}
		break;
	case OTG_STATE_A_SUSPEND:
		/* finish RESUME signaling? */
		if (musb->port1_status & MUSB_PORT_STAT_RESUME) {
			power = musb_readb(musb->mregs, MUSB_POWER);
			power &= ~MUSB_POWER_RESUME;
			dev_dbg(musb->controller, "root port resume stopped, power %02x\n", power);
			musb_writeb(musb->mregs, MUSB_POWER, power);
			musb->is_active = 1;
			musb->port1_status &= ~(USB_PORT_STAT_SUSPEND
						| MUSB_PORT_STAT_RESUME);
			musb->port1_status |= USB_PORT_STAT_C_SUSPEND << 16;
			usb_hcd_poll_rh_status(musb_to_hcd(musb));
			/* NOTE: it might really be A_WAIT_BCON ... */
			musb->xceiv->state = OTG_STATE_A_HOST;
		}
		break;
	case OTG_STATE_A_HOST:
		devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
		if (devctl &  MUSB_DEVCTL_BDEVICE)
			musb->xceiv->state = OTG_STATE_B_IDLE;
		else
			musb->xceiv->state = OTG_STATE_A_WAIT_BCON;
	default:
		break;
	}
	spin_unlock_irqrestore(&musb->lock, flags);
}


static void omap2430_musb_try_idle(struct musb *musb, unsigned long timeout)
{
	unsigned long		default_timeout = jiffies + msecs_to_jiffies(3);
	static unsigned long	last_timer;

	if (timeout == 0)
		timeout = default_timeout;

	/* Never idle if active, or when VBUS timeout is not set as host */
	if (musb->is_active || ((musb->a_wait_bcon == 0)
			&& (musb->xceiv->state == OTG_STATE_A_WAIT_BCON))) {
		dev_dbg(musb->controller, "%s active, deleting timer\n",
			otg_state_string(musb->xceiv->state));
		del_timer(&musb_idle_timer);
		last_timer = jiffies;
		return;
	}

	if (time_after(last_timer, timeout)) {
		if (!timer_pending(&musb_idle_timer))
			last_timer = timeout;
		else {
			dev_dbg(musb->controller, "Longer idle timer already pending, ignoring\n");
			return;
		}
	}
	last_timer = timeout;

	dev_dbg(musb->controller, "%s inactive, for idle timer for %lu ms\n",
		otg_state_string(musb->xceiv->state),
		(unsigned long)jiffies_to_msecs(timeout - jiffies));
	mod_timer(&musb_idle_timer, timeout);
}

static void omap2430_musb_set_vbus(struct musb *musb, int is_on)
{
	struct usb_otg	*otg = musb->xceiv->otg;
	u8		devctl;
	unsigned long timeout = jiffies + msecs_to_jiffies(1000);
	int ret = 1;
	/* HDRC controls CPEN, but beware current surges during device
	 * connect.  They can trigger transient overcurrent conditions
	 * that must be ignored.
	 */

	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);

	if (is_on) {
		if (musb->xceiv->state == OTG_STATE_A_IDLE) {
			/* start the session */
			devctl |= MUSB_DEVCTL_SESSION;
			musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);
			/*
			 * Wait for the musb to set as A device to enable the
			 * VBUS
			 */
			while (musb_readb(musb->mregs, MUSB_DEVCTL) & 0x80) {

				cpu_relax();

				if (time_after(jiffies, timeout)) {
					dev_err(musb->controller,
					"configured as A device timeout");
					ret = -EINVAL;
					break;
				}
			}

			if (ret && otg->set_vbus)
				otg_set_vbus(otg, 1);
		} else {
			musb->is_active = 1;
			otg->default_a = 1;
			musb->xceiv->state = OTG_STATE_A_WAIT_VRISE;
			devctl |= MUSB_DEVCTL_SESSION;
			MUSB_HST_MODE(musb);
		}
	} else {
		musb->is_active = 0;

		/* NOTE:  we're skipping A_WAIT_VFALL -> A_IDLE and
		 * jumping right to B_IDLE...
		 */

		otg->default_a = 0;
		musb->xceiv->state = OTG_STATE_B_IDLE;
		devctl &= ~MUSB_DEVCTL_SESSION;

		MUSB_DEV_MODE(musb);
	}
	musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);

	dev_dbg(musb->controller, "VBUS %s, devctl %02x "
		/* otg %3x conf %08x prcm %08x */ "\n",
		otg_state_string(musb->xceiv->state),
		musb_readb(musb->mregs, MUSB_DEVCTL));
}

static int omap2430_musb_set_mode(struct musb *musb, u8 musb_mode)
{
	u8	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);

	devctl |= MUSB_DEVCTL_SESSION;
	musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);

	return 0;
}
=======
	enum musb_vbus_id_status status;
	struct work_struct	omap_musb_mailbox_work;
	struct device		*control_otghs;
	unsigned int		is_runtime_suspended:1;
	unsigned int		needs_resume:1;
	unsigned int		phy_suspended:1;
};
#define glue_to_musb(g)		platform_get_drvdata(g->musb)

static struct omap2430_glue	*_glue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void omap2430_low_level_exit(struct musb *musb)
{
	u32 l;

	/* in any role */
	l = musb_readl(musb->mregs, OTG_FORCESTDBY);
	l |= ENABLEFORCE;	/* enable MSTANDBY */
	musb_writel(musb->mregs, OTG_FORCESTDBY, l);
}

static inline void omap2430_low_level_init(struct musb *musb)
{
	u32 l;

	l = musb_readl(musb->mregs, OTG_FORCESTDBY);
	l &= ~ENABLEFORCE;	/* disable MSTANDBY */
	musb_writel(musb->mregs, OTG_FORCESTDBY, l);
}

<<<<<<< HEAD
static int musb_otg_notifications(struct notifier_block *nb,
		unsigned long event, void *unused)
{
	struct musb	*musb = container_of(nb, struct musb, nb);

	musb->xceiv_event = event;
	schedule_work(&musb->otg_notifier_work);

	return NOTIFY_OK;
}

static void musb_otg_notifier_work(struct work_struct *data_notifier_work)
{
	struct musb *musb = container_of(data_notifier_work, struct musb, otg_notifier_work);
	struct device *dev = musb->controller;
	struct musb_hdrc_platform_data *pdata = dev->platform_data;
	struct omap_musb_board_data *data = pdata->board_data;

	switch (musb->xceiv_event) {
	case USB_EVENT_ID:
		dev_dbg(musb->controller, "ID GND\n");

		if (!is_otg_enabled(musb) || musb->gadget_driver) {
			pm_runtime_get_sync(musb->controller);
			usb_phy_init(musb->xceiv);
			omap2430_musb_set_vbus(musb, 1);
		}
		break;

	case USB_EVENT_VBUS:
		dev_dbg(musb->controller, "VBUS Connect\n");

		if (musb->gadget_driver)
			pm_runtime_get_sync(musb->controller);
		usb_phy_init(musb->xceiv);
		break;

	case USB_EVENT_NONE:
		dev_dbg(musb->controller, "VBUS Disconnect\n");

		if (is_otg_enabled(musb) || is_peripheral_enabled(musb))
			if (musb->gadget_driver) {
				pm_runtime_mark_last_busy(musb->controller);
				pm_runtime_put_autosuspend(musb->controller);
			}

		if (data->interface_type == MUSB_INTERFACE_UTMI) {
			if (musb->xceiv->otg->set_vbus)
				otg_set_vbus(musb->xceiv->otg, 0);
		}
		usb_phy_shutdown(musb->xceiv);
=======
static int omap2430_musb_mailbox(enum musb_vbus_id_status status)
{
	struct omap2430_glue	*glue = _glue;

	if (!glue) {
		pr_err("%s: musb core is not yet initialized\n", __func__);
		return -EPROBE_DEFER;
	}
	glue->status = status;

	if (!glue_to_musb(glue)) {
		pr_err("%s: musb core is not yet ready\n", __func__);
		return -EPROBE_DEFER;
	}

	schedule_work(&glue->omap_musb_mailbox_work);

	return 0;
}

/*
 * HDRC controls CPEN, but beware current surges during device connect.
 * They can trigger transient overcurrent conditions that must be ignored.
 *
 * Note that we're skipping A_WAIT_VFALL -> A_IDLE and jumping right to B_IDLE
 * as set by musb_set_peripheral().
 */
static void omap_musb_set_mailbox(struct omap2430_glue *glue)
{
	struct musb *musb = glue_to_musb(glue);
	int error;

	pm_runtime_get_sync(musb->controller);

	dev_dbg(musb->controller, "VBUS %s, devctl %02x\n",
		usb_otg_state_string(musb->xceiv->otg->state),
		musb_readb(musb->mregs, MUSB_DEVCTL));

	switch (glue->status) {
	case MUSB_ID_GROUND:
		dev_dbg(musb->controller, "ID GND\n");
		switch (musb->xceiv->otg->state) {
		case OTG_STATE_A_IDLE:
			error = musb_set_host(musb);
			if (error)
				break;
			musb->xceiv->otg->state = OTG_STATE_A_WAIT_VRISE;
			fallthrough;
		case OTG_STATE_A_WAIT_VRISE:
		case OTG_STATE_A_WAIT_BCON:
		case OTG_STATE_A_HOST:
			/*
			 * On multiple ID ground interrupts just keep enabling
			 * VBUS. At least cpcap VBUS shuts down otherwise.
			 */
			otg_set_vbus(musb->xceiv->otg, 1);
			break;
		default:
			musb->xceiv->otg->state = OTG_STATE_A_IDLE;
			musb->xceiv->last_event = USB_EVENT_ID;
			if (musb->gadget_driver) {
				omap_control_usb_set_mode(glue->control_otghs,
							  USB_MODE_HOST);
				otg_set_vbus(musb->xceiv->otg, 1);
			}
			break;
		}
		break;

	case MUSB_VBUS_VALID:
		dev_dbg(musb->controller, "VBUS Connect\n");

		musb->xceiv->otg->state = OTG_STATE_B_IDLE;
		musb->xceiv->last_event = USB_EVENT_VBUS;
		omap_control_usb_set_mode(glue->control_otghs, USB_MODE_DEVICE);
		break;

	case MUSB_ID_FLOAT:
	case MUSB_VBUS_OFF:
		dev_dbg(musb->controller, "VBUS Disconnect\n");

		musb->xceiv->last_event = USB_EVENT_NONE;
		musb_set_peripheral(musb);
		otg_set_vbus(musb->xceiv->otg, 0);
		omap_control_usb_set_mode(glue->control_otghs,
			USB_MODE_DISCONNECT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		dev_dbg(musb->controller, "ID float\n");
	}
<<<<<<< HEAD
=======
	pm_runtime_mark_last_busy(musb->controller);
	pm_runtime_put_autosuspend(musb->controller);
	atomic_notifier_call_chain(&musb->xceiv->notifier,
			musb->xceiv->last_event, NULL);
}


static void omap_musb_mailbox_work(struct work_struct *mailbox_work)
{
	struct omap2430_glue *glue = container_of(mailbox_work,
				struct omap2430_glue, omap_musb_mailbox_work);

	omap_musb_set_mailbox(glue);
}

static irqreturn_t omap2430_musb_interrupt(int irq, void *__hci)
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
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int omap2430_musb_init(struct musb *musb)
{
	u32 l;
	int status = 0;
	struct device *dev = musb->controller;
<<<<<<< HEAD
	struct musb_hdrc_platform_data *plat = dev->platform_data;
=======
	struct musb_hdrc_platform_data *plat = dev_get_platdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct omap_musb_board_data *data = plat->board_data;

	/* We require some kind of external transceiver, hooked
	 * up through ULPI.  TWL4030-family PMICs include one,
	 * which needs a driver, drivers aren't always needed.
	 */
<<<<<<< HEAD
	musb->xceiv = usb_get_transceiver();
	if (!musb->xceiv) {
		pr_err("HS USB OTG: no transceiver configured\n");
		return -ENODEV;
	}

	INIT_WORK(&musb->otg_notifier_work, musb_otg_notifier_work);

	status = pm_runtime_get_sync(dev);
	if (status < 0) {
		dev_err(dev, "pm_runtime_get_sync FAILED %d\n", status);
		goto err1;
	}
=======
	musb->phy = devm_phy_get(dev->parent, "usb2-phy");

	/* We can't totally remove musb->xceiv as of now because
	 * musb core uses xceiv.state and xceiv.otg. Once we have
	 * a separate state machine to handle otg, these can be moved
	 * out of xceiv and then we can start using the generic PHY
	 * framework
	 */
	musb->xceiv = devm_usb_get_phy_by_phandle(dev->parent, "usb-phy", 0);

	if (IS_ERR(musb->xceiv)) {
		status = PTR_ERR(musb->xceiv);

		if (status == -ENXIO)
			return status;

		dev_dbg(dev, "HS USB OTG: no transceiver configured\n");
		return -EPROBE_DEFER;
	}

	if (IS_ERR(musb->phy)) {
		dev_err(dev, "HS USB OTG: no PHY configured\n");
		return PTR_ERR(musb->phy);
	}
	musb->isr = omap2430_musb_interrupt;
	phy_init(musb->phy);
	phy_power_on(musb->phy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	l = musb_readl(musb->mregs, OTG_INTERFSEL);

	if (data->interface_type == MUSB_INTERFACE_UTMI) {
		/* OMAP4 uses Internal PHY GS70 which uses UTMI interface */
		l &= ~ULPI_12PIN;       /* Disable ULPI */
		l |= UTMI_8BIT;         /* Enable UTMI  */
	} else {
		l |= ULPI_12PIN;
	}

	musb_writel(musb->mregs, OTG_INTERFSEL, l);

<<<<<<< HEAD
	pr_debug("HS USB OTG: revision 0x%x, sysconfig 0x%02x, "
=======
	dev_dbg(dev, "HS USB OTG: revision 0x%x, sysconfig 0x%02x, "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			"sysstatus 0x%x, intrfsel 0x%x, simenable  0x%x\n",
			musb_readl(musb->mregs, OTG_REVISION),
			musb_readl(musb->mregs, OTG_SYSCONFIG),
			musb_readl(musb->mregs, OTG_SYSSTATUS),
			musb_readl(musb->mregs, OTG_INTERFSEL),
			musb_readl(musb->mregs, OTG_SIMENABLE));

<<<<<<< HEAD
	musb->nb.notifier_call = musb_otg_notifications;
	status = usb_register_notifier(musb->xceiv, &musb->nb);

	if (status)
		dev_dbg(musb->controller, "notification register failed\n");

	setup_timer(&musb_idle_timer, musb_do_idle, (unsigned long) musb);

	pm_runtime_put_noidle(musb->controller);
	return 0;

err1:
	return status;
=======
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void omap2430_musb_enable(struct musb *musb)
{
<<<<<<< HEAD
	u8		devctl;
	unsigned long timeout = jiffies + msecs_to_jiffies(1000);
	struct device *dev = musb->controller;
	struct musb_hdrc_platform_data *pdata = dev->platform_data;
	struct omap_musb_board_data *data = pdata->board_data;

	switch (musb->xceiv->last_event) {

	case USB_EVENT_ID:
		usb_phy_init(musb->xceiv);
		if (data->interface_type != MUSB_INTERFACE_UTMI)
			break;
		devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
		/* start the session */
		devctl |= MUSB_DEVCTL_SESSION;
		musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);
		while (musb_readb(musb->mregs, MUSB_DEVCTL) &
				MUSB_DEVCTL_BDEVICE) {
			cpu_relax();

			if (time_after(jiffies, timeout)) {
				dev_err(dev, "configured as A device timeout");
				break;
			}
		}
		break;

	case USB_EVENT_VBUS:
		usb_phy_init(musb->xceiv);
		break;

	default:
		break;
	}
=======
	struct device *dev = musb->controller;
	struct omap2430_glue *glue = dev_get_drvdata(dev->parent);

	if (glue->status == MUSB_UNKNOWN)
		glue->status = MUSB_VBUS_OFF;
	omap_musb_set_mailbox(glue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void omap2430_musb_disable(struct musb *musb)
{
<<<<<<< HEAD
	if (musb->xceiv->last_event)
		usb_phy_shutdown(musb->xceiv);
=======
	struct device *dev = musb->controller;
	struct omap2430_glue *glue = dev_get_drvdata(dev->parent);

	if (glue->status != MUSB_UNKNOWN)
		omap_control_usb_set_mode(glue->control_otghs,
			USB_MODE_DISCONNECT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int omap2430_musb_exit(struct musb *musb)
{
<<<<<<< HEAD
	del_timer_sync(&musb_idle_timer);
	cancel_work_sync(&musb->otg_notifier_work);

	omap2430_low_level_exit(musb);
	usb_put_transceiver(musb->xceiv);
=======
	struct device *dev = musb->controller;
	struct omap2430_glue *glue = dev_get_drvdata(dev->parent);

	omap2430_low_level_exit(musb);
	phy_power_off(musb->phy);
	phy_exit(musb->phy);
	musb->phy = NULL;
	cancel_work_sync(&glue->omap_musb_mailbox_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct musb_platform_ops omap2430_ops = {
<<<<<<< HEAD
	.init		= omap2430_musb_init,
	.exit		= omap2430_musb_exit,

	.set_mode	= omap2430_musb_set_mode,
	.try_idle	= omap2430_musb_try_idle,

	.set_vbus	= omap2430_musb_set_vbus,

	.enable		= omap2430_musb_enable,
	.disable	= omap2430_musb_disable,
=======
	.quirks		= MUSB_DMA_INVENTRA,
#ifdef CONFIG_USB_INVENTRA_DMA
	.dma_init	= musbhs_dma_controller_create,
	.dma_exit	= musbhs_dma_controller_destroy,
#endif
	.init		= omap2430_musb_init,
	.exit		= omap2430_musb_exit,

	.enable		= omap2430_musb_enable,
	.disable	= omap2430_musb_disable,

	.phy_callback	= omap2430_musb_mailbox,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static u64 omap2430_dmamask = DMA_BIT_MASK(32);

<<<<<<< HEAD
static int __devinit omap2430_probe(struct platform_device *pdev)
{
	struct musb_hdrc_platform_data	*pdata = pdev->dev.platform_data;
	struct platform_device		*musb;
	struct omap2430_glue		*glue;
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
=======
static int omap2430_probe(struct platform_device *pdev)
{
	struct musb_hdrc_platform_data	*pdata = dev_get_platdata(&pdev->dev);
	struct omap_musb_board_data	*data;
	struct platform_device		*musb;
	struct omap2430_glue		*glue;
	struct device_node		*np = pdev->dev.of_node;
	struct musb_hdrc_config		*config;
	struct device_node		*control_node;
	struct platform_device		*control_pdev;
	int				ret = -ENOMEM, val;
	bool				populate_irqs = false;

	if (!np)
		return -ENODEV;

	glue = devm_kzalloc(&pdev->dev, sizeof(*glue), GFP_KERNEL);
	if (!glue)
		goto err0;

	musb = platform_device_alloc("musb-hdrc", PLATFORM_DEVID_AUTO);
	if (!musb) {
		dev_err(&pdev->dev, "failed to allocate musb device\n");
		goto err0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	musb->dev.parent		= &pdev->dev;
	musb->dev.dma_mask		= &omap2430_dmamask;
	musb->dev.coherent_dma_mask	= omap2430_dmamask;

<<<<<<< HEAD
	glue->dev			= &pdev->dev;
	glue->musb			= musb;
=======
	/*
	 * Legacy SoCs using omap_device get confused if node is moved
	 * because of interconnect properties mixed into the node.
	 */
	if (of_property_present(np, "ti,hwmods")) {
		dev_warn(&pdev->dev, "please update to probe with ti-sysc\n");
		populate_irqs = true;
	} else {
		device_set_of_node_from_dev(&musb->dev, &pdev->dev);
	}
	of_node_put(np);

	glue->dev			= &pdev->dev;
	glue->musb			= musb;
	glue->status			= MUSB_UNKNOWN;
	glue->control_otghs = ERR_PTR(-ENODEV);

	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		goto err2;

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		goto err2;

	config = devm_kzalloc(&pdev->dev, sizeof(*config), GFP_KERNEL);
	if (!config)
		goto err2;

	of_property_read_u32(np, "mode", (u32 *)&pdata->mode);
	of_property_read_u32(np, "interface-type",
			(u32 *)&data->interface_type);
	of_property_read_u32(np, "num-eps", (u32 *)&config->num_eps);
	of_property_read_u32(np, "ram-bits", (u32 *)&config->ram_bits);
	of_property_read_u32(np, "power", (u32 *)&pdata->power);

	ret = of_property_read_u32(np, "multipoint", &val);
	if (!ret && val)
		config->multipoint = true;

	pdata->board_data	= data;
	pdata->config		= config;

	control_node = of_parse_phandle(np, "ctrl-module", 0);
	if (control_node) {
		control_pdev = of_find_device_by_node(control_node);
		of_node_put(control_node);
		if (!control_pdev) {
			dev_err(&pdev->dev, "Failed to get control device\n");
			ret = -EINVAL;
			goto err2;
		}
		glue->control_otghs = &control_pdev->dev;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pdata->platform_ops		= &omap2430_ops;

	platform_set_drvdata(pdev, glue);

<<<<<<< HEAD
	ret = platform_device_add_resources(musb, pdev->resource,
			pdev->num_resources);
=======
	/*
	 * REVISIT if we ever have two instances of the wrapper, we will be
	 * in big trouble
	 */
	_glue	= glue;

	INIT_WORK(&glue->omap_musb_mailbox_work, omap_musb_mailbox_work);

	ret = platform_device_add_resources(musb, pdev->resource, pdev->num_resources);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret) {
		dev_err(&pdev->dev, "failed to add resources\n");
		goto err2;
	}

<<<<<<< HEAD
=======
	if (populate_irqs) {
		struct resource musb_res[3];
		struct resource *res;
		int i = 0;

		memset(musb_res, 0, sizeof(*musb_res) * ARRAY_SIZE(musb_res));

		res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		if (!res) {
			ret = -EINVAL;
			goto err2;
		}

		musb_res[i].start = res->start;
		musb_res[i].end = res->end;
		musb_res[i].flags = res->flags;
		musb_res[i].name = res->name;
		i++;

		ret = of_irq_get_byname(np, "mc");
		if (ret > 0) {
			musb_res[i].start = ret;
			musb_res[i].flags = IORESOURCE_IRQ;
			musb_res[i].name = "mc";
			i++;
		}

		ret = of_irq_get_byname(np, "dma");
		if (ret > 0) {
			musb_res[i].start = ret;
			musb_res[i].flags = IORESOURCE_IRQ;
			musb_res[i].name = "dma";
			i++;
		}

		ret = platform_device_add_resources(musb, musb_res, i);
		if (ret) {
			dev_err(&pdev->dev, "failed to add IRQ resources\n");
			goto err2;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = platform_device_add_data(musb, pdata, sizeof(*pdata));
	if (ret) {
		dev_err(&pdev->dev, "failed to add platform_data\n");
		goto err2;
	}

<<<<<<< HEAD
	pm_runtime_enable(&pdev->dev);
=======
	pm_runtime_enable(glue->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = platform_device_add(musb);
	if (ret) {
		dev_err(&pdev->dev, "failed to register musb device\n");
<<<<<<< HEAD
		goto err2;
=======
		goto err3;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

<<<<<<< HEAD
err2:
	platform_device_put(musb);

err1:
	kfree(glue);

=======
err3:
	pm_runtime_disable(glue->dev);

err2:
	platform_device_put(musb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err0:
	return ret;
}

<<<<<<< HEAD
static int __devexit omap2430_remove(struct platform_device *pdev)
{
	struct omap2430_glue		*glue = platform_get_drvdata(pdev);

	platform_device_del(glue->musb);
	platform_device_put(glue->musb);
	kfree(glue);

	return 0;
=======
static void omap2430_remove(struct platform_device *pdev)
{
	struct omap2430_glue *glue = platform_get_drvdata(pdev);

	platform_device_unregister(glue->musb);
	pm_runtime_disable(glue->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_PM

static int omap2430_runtime_suspend(struct device *dev)
{
	struct omap2430_glue		*glue = dev_get_drvdata(dev);
	struct musb			*musb = glue_to_musb(glue);

<<<<<<< HEAD
	if (musb) {
		musb->context.otg_interfsel = musb_readl(musb->mregs,
				OTG_INTERFSEL);

		omap2430_low_level_exit(musb);
		usb_phy_set_suspend(musb->xceiv, 1);
	}

=======
	if (!musb)
		return 0;

	musb->context.otg_interfsel = musb_readl(musb->mregs,
						 OTG_INTERFSEL);

	omap2430_low_level_exit(musb);

	if (!glue->phy_suspended) {
		phy_power_off(musb->phy);
		phy_exit(musb->phy);
	}

	glue->is_runtime_suspended = 1;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int omap2430_runtime_resume(struct device *dev)
{
	struct omap2430_glue		*glue = dev_get_drvdata(dev);
	struct musb			*musb = glue_to_musb(glue);

<<<<<<< HEAD
	if (musb) {
		omap2430_low_level_init(musb);
		musb_writel(musb->mregs, OTG_INTERFSEL,
				musb->context.otg_interfsel);

		usb_phy_set_suspend(musb->xceiv, 0);
	}

	return 0;
}

static struct dev_pm_ops omap2430_pm_ops = {
	.runtime_suspend = omap2430_runtime_suspend,
	.runtime_resume = omap2430_runtime_resume,
=======
	if (!musb)
		return 0;

	if (!glue->phy_suspended) {
		phy_init(musb->phy);
		phy_power_on(musb->phy);
	}

	omap2430_low_level_init(musb);
	musb_writel(musb->mregs, OTG_INTERFSEL,
		    musb->context.otg_interfsel);

	/* Wait for musb to get oriented. Otherwise we can get babble */
	usleep_range(200000, 250000);

	glue->is_runtime_suspended = 0;

	return 0;
}

/* I2C and SPI PHYs need to be suspended before the glue layer */
static int omap2430_suspend(struct device *dev)
{
	struct omap2430_glue *glue = dev_get_drvdata(dev);
	struct musb *musb = glue_to_musb(glue);

	phy_power_off(musb->phy);
	phy_exit(musb->phy);
	glue->phy_suspended = 1;

	return 0;
}

/* Glue layer needs to be suspended after musb_suspend() */
static int omap2430_suspend_late(struct device *dev)
{
	struct omap2430_glue *glue = dev_get_drvdata(dev);

	if (glue->is_runtime_suspended)
		return 0;

	glue->needs_resume = 1;

	return omap2430_runtime_suspend(dev);
}

static int omap2430_resume_early(struct device *dev)
{
	struct omap2430_glue *glue = dev_get_drvdata(dev);

	if (!glue->needs_resume)
		return 0;

	glue->needs_resume = 0;

	return omap2430_runtime_resume(dev);
}

static int omap2430_resume(struct device *dev)
{
	struct omap2430_glue *glue = dev_get_drvdata(dev);
	struct musb *musb = glue_to_musb(glue);

	phy_init(musb->phy);
	phy_power_on(musb->phy);
	glue->phy_suspended = 0;

	return 0;
}

static const struct dev_pm_ops omap2430_pm_ops = {
	.runtime_suspend = omap2430_runtime_suspend,
	.runtime_resume = omap2430_runtime_resume,
	.suspend = omap2430_suspend,
	.suspend_late = omap2430_suspend_late,
	.resume_early = omap2430_resume_early,
	.resume = omap2430_resume,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define DEV_PM_OPS	(&omap2430_pm_ops)
#else
#define DEV_PM_OPS	NULL
#endif

<<<<<<< HEAD
static struct platform_driver omap2430_driver = {
	.probe		= omap2430_probe,
	.remove		= __devexit_p(omap2430_remove),
	.driver		= {
		.name	= "musb-omap2430",
		.pm	= DEV_PM_OPS,
	},
};

MODULE_DESCRIPTION("OMAP2PLUS MUSB Glue Layer");
MODULE_AUTHOR("Felipe Balbi <balbi@ti.com>");
MODULE_LICENSE("GPL v2");

static int __init omap2430_init(void)
{
	return platform_driver_register(&omap2430_driver);
}
module_init(omap2430_init);

static void __exit omap2430_exit(void)
{
	platform_driver_unregister(&omap2430_driver);
}
module_exit(omap2430_exit);
=======
#ifdef CONFIG_OF
static const struct of_device_id omap2430_id_table[] = {
	{
		.compatible = "ti,omap4-musb"
	},
	{
		.compatible = "ti,omap3-musb"
	},
	{},
};
MODULE_DEVICE_TABLE(of, omap2430_id_table);
#endif

static struct platform_driver omap2430_driver = {
	.probe		= omap2430_probe,
	.remove_new	= omap2430_remove,
	.driver		= {
		.name	= "musb-omap2430",
		.pm	= DEV_PM_OPS,
		.of_match_table = of_match_ptr(omap2430_id_table),
	},
};

module_platform_driver(omap2430_driver);

MODULE_DESCRIPTION("OMAP2PLUS MUSB Glue Layer");
MODULE_AUTHOR("Felipe Balbi <balbi@ti.com>");
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
