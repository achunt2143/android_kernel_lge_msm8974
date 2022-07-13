<<<<<<< HEAD
/**
 * omap-usb-host.c - The USBHS core driver for OMAP EHCI & OHCI
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com
 * Author: Keshava Munegowda <keshava_mgowda@ti.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * omap-usb-host.c - The USBHS core driver for OMAP EHCI & OHCI
 *
 * Copyright (C) 2011-2013 Texas Instruments Incorporated - https://www.ti.com
 * Author: Keshava Munegowda <keshava_mgowda@ti.com>
 * Author: Roger Quadros <rogerq@ti.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/spinlock.h>
#include <plat/cpu.h>
#include <plat/usb.h>
#include <linux/pm_runtime.h>
=======
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/platform_data/usb-omap.h>
#include <linux/pm_runtime.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/err.h>

#include "omap-usb.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define USBHS_DRIVER_NAME	"usbhs_omap"
#define OMAP_EHCI_DEVICE	"ehci-omap"
#define OMAP_OHCI_DEVICE	"ohci-omap3"

/* OMAP USBHOST Register addresses  */

<<<<<<< HEAD
/* TLL Register Set */
#define	OMAP_USBTLL_REVISION				(0x00)
#define	OMAP_USBTLL_SYSCONFIG				(0x10)
#define	OMAP_USBTLL_SYSCONFIG_CACTIVITY			(1 << 8)
#define	OMAP_USBTLL_SYSCONFIG_SIDLEMODE			(1 << 3)
#define	OMAP_USBTLL_SYSCONFIG_ENAWAKEUP			(1 << 2)
#define	OMAP_USBTLL_SYSCONFIG_SOFTRESET			(1 << 1)
#define	OMAP_USBTLL_SYSCONFIG_AUTOIDLE			(1 << 0)

#define	OMAP_USBTLL_SYSSTATUS				(0x14)
#define	OMAP_USBTLL_SYSSTATUS_RESETDONE			(1 << 0)

#define	OMAP_USBTLL_IRQSTATUS				(0x18)
#define	OMAP_USBTLL_IRQENABLE				(0x1C)

#define	OMAP_TLL_SHARED_CONF				(0x30)
#define	OMAP_TLL_SHARED_CONF_USB_90D_DDR_EN		(1 << 6)
#define	OMAP_TLL_SHARED_CONF_USB_180D_SDR_EN		(1 << 5)
#define	OMAP_TLL_SHARED_CONF_USB_DIVRATION		(1 << 2)
#define	OMAP_TLL_SHARED_CONF_FCLK_REQ			(1 << 1)
#define	OMAP_TLL_SHARED_CONF_FCLK_IS_ON			(1 << 0)

#define	OMAP_TLL_CHANNEL_CONF(num)			(0x040 + 0x004 * num)
#define OMAP_TLL_CHANNEL_CONF_FSLSMODE_SHIFT		24
#define	OMAP_TLL_CHANNEL_CONF_ULPINOBITSTUFF		(1 << 11)
#define	OMAP_TLL_CHANNEL_CONF_ULPI_ULPIAUTOIDLE		(1 << 10)
#define	OMAP_TLL_CHANNEL_CONF_UTMIAUTOIDLE		(1 << 9)
#define	OMAP_TLL_CHANNEL_CONF_ULPIDDRMODE		(1 << 8)
#define OMAP_TLL_CHANNEL_CONF_CHANMODE_FSLS		(1 << 1)
#define	OMAP_TLL_CHANNEL_CONF_CHANEN			(1 << 0)

#define OMAP_TLL_FSLSMODE_6PIN_PHY_DAT_SE0		0x0
#define OMAP_TLL_FSLSMODE_6PIN_PHY_DP_DM		0x1
#define OMAP_TLL_FSLSMODE_3PIN_PHY			0x2
#define OMAP_TLL_FSLSMODE_4PIN_PHY			0x3
#define OMAP_TLL_FSLSMODE_6PIN_TLL_DAT_SE0		0x4
#define OMAP_TLL_FSLSMODE_6PIN_TLL_DP_DM		0x5
#define OMAP_TLL_FSLSMODE_3PIN_TLL			0x6
#define OMAP_TLL_FSLSMODE_4PIN_TLL			0x7
#define OMAP_TLL_FSLSMODE_2PIN_TLL_DAT_SE0		0xA
#define OMAP_TLL_FSLSMODE_2PIN_DAT_DP_DM		0xB

#define	OMAP_TLL_ULPI_FUNCTION_CTRL(num)		(0x804 + 0x100 * num)
#define	OMAP_TLL_ULPI_INTERFACE_CTRL(num)		(0x807 + 0x100 * num)
#define	OMAP_TLL_ULPI_OTG_CTRL(num)			(0x80A + 0x100 * num)
#define	OMAP_TLL_ULPI_INT_EN_RISE(num)			(0x80D + 0x100 * num)
#define	OMAP_TLL_ULPI_INT_EN_FALL(num)			(0x810 + 0x100 * num)
#define	OMAP_TLL_ULPI_INT_STATUS(num)			(0x813 + 0x100 * num)
#define	OMAP_TLL_ULPI_INT_LATCH(num)			(0x814 + 0x100 * num)
#define	OMAP_TLL_ULPI_DEBUG(num)			(0x815 + 0x100 * num)
#define	OMAP_TLL_ULPI_SCRATCH_REGISTER(num)		(0x816 + 0x100 * num)

#define OMAP_TLL_CHANNEL_COUNT				3
#define OMAP_TLL_CHANNEL_1_EN_MASK			(1 << 0)
#define OMAP_TLL_CHANNEL_2_EN_MASK			(1 << 1)
#define OMAP_TLL_CHANNEL_3_EN_MASK			(1 << 2)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* UHH Register Set */
#define	OMAP_UHH_REVISION				(0x00)
#define	OMAP_UHH_SYSCONFIG				(0x10)
#define	OMAP_UHH_SYSCONFIG_MIDLEMODE			(1 << 12)
#define	OMAP_UHH_SYSCONFIG_CACTIVITY			(1 << 8)
#define	OMAP_UHH_SYSCONFIG_SIDLEMODE			(1 << 3)
#define	OMAP_UHH_SYSCONFIG_ENAWAKEUP			(1 << 2)
#define	OMAP_UHH_SYSCONFIG_SOFTRESET			(1 << 1)
#define	OMAP_UHH_SYSCONFIG_AUTOIDLE			(1 << 0)

#define	OMAP_UHH_SYSSTATUS				(0x14)
#define	OMAP_UHH_HOSTCONFIG				(0x40)
#define	OMAP_UHH_HOSTCONFIG_ULPI_BYPASS			(1 << 0)
#define	OMAP_UHH_HOSTCONFIG_ULPI_P1_BYPASS		(1 << 0)
#define	OMAP_UHH_HOSTCONFIG_ULPI_P2_BYPASS		(1 << 11)
#define	OMAP_UHH_HOSTCONFIG_ULPI_P3_BYPASS		(1 << 12)
#define OMAP_UHH_HOSTCONFIG_INCR4_BURST_EN		(1 << 2)
#define OMAP_UHH_HOSTCONFIG_INCR8_BURST_EN		(1 << 3)
#define OMAP_UHH_HOSTCONFIG_INCR16_BURST_EN		(1 << 4)
#define OMAP_UHH_HOSTCONFIG_INCRX_ALIGN_EN		(1 << 5)
#define OMAP_UHH_HOSTCONFIG_P1_CONNECT_STATUS		(1 << 8)
#define OMAP_UHH_HOSTCONFIG_P2_CONNECT_STATUS		(1 << 9)
#define OMAP_UHH_HOSTCONFIG_P3_CONNECT_STATUS		(1 << 10)
#define OMAP4_UHH_HOSTCONFIG_APP_START_CLK		(1 << 31)

/* OMAP4-specific defines */
#define OMAP4_UHH_SYSCONFIG_IDLEMODE_CLEAR		(3 << 2)
#define OMAP4_UHH_SYSCONFIG_NOIDLE			(1 << 2)
#define OMAP4_UHH_SYSCONFIG_STDBYMODE_CLEAR		(3 << 4)
#define OMAP4_UHH_SYSCONFIG_NOSTDBY			(1 << 4)
#define OMAP4_UHH_SYSCONFIG_SOFTRESET			(1 << 0)

#define OMAP4_P1_MODE_CLEAR				(3 << 16)
#define OMAP4_P1_MODE_TLL				(1 << 16)
#define OMAP4_P1_MODE_HSIC				(3 << 16)
#define OMAP4_P2_MODE_CLEAR				(3 << 18)
#define OMAP4_P2_MODE_TLL				(1 << 18)
#define OMAP4_P2_MODE_HSIC				(3 << 18)

<<<<<<< HEAD
#define OMAP_REV2_TLL_CHANNEL_COUNT			2

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define	OMAP_UHH_DEBUG_CSR				(0x44)

/* Values of UHH_REVISION - Note: these are not given in the TRM */
#define OMAP_USBHS_REV1		0x00000010	/* OMAP3 */
#define OMAP_USBHS_REV2		0x50700100	/* OMAP4 */

#define is_omap_usbhs_rev1(x)	(x->usbhs_rev == OMAP_USBHS_REV1)
#define is_omap_usbhs_rev2(x)	(x->usbhs_rev == OMAP_USBHS_REV2)

#define is_ehci_phy_mode(x)	(x == OMAP_EHCI_PORT_MODE_PHY)
#define is_ehci_tll_mode(x)	(x == OMAP_EHCI_PORT_MODE_TLL)
#define is_ehci_hsic_mode(x)	(x == OMAP_EHCI_PORT_MODE_HSIC)


struct usbhs_hcd_omap {
<<<<<<< HEAD
	struct clk			*xclk60mhsp1_ck;
	struct clk			*xclk60mhsp2_ck;
	struct clk			*utmi_p1_fck;
	struct clk			*usbhost_p1_fck;
	struct clk			*usbtll_p1_fck;
	struct clk			*utmi_p2_fck;
	struct clk			*usbhost_p2_fck;
	struct clk			*usbtll_p2_fck;
=======
	int				nports;
	struct clk			**utmi_clk;
	struct clk			**hsic60m_clk;
	struct clk			**hsic480m_clk;

	struct clk			*xclk60mhsp1_ck;
	struct clk			*xclk60mhsp2_ck;
	struct clk			*utmi_p1_gfclk;
	struct clk			*utmi_p2_gfclk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct clk			*init_60m_fclk;
	struct clk			*ehci_logic_fck;

	void __iomem			*uhh_base;
<<<<<<< HEAD
	void __iomem			*tll_base;

	struct usbhs_omap_platform_data	platdata;

	u32				usbhs_rev;
	spinlock_t			lock;
};
/*-------------------------------------------------------------------------*/

const char usbhs_driver_name[] = USBHS_DRIVER_NAME;
=======

	struct usbhs_omap_platform_data	*pdata;

	u32				usbhs_rev;
};
/*-------------------------------------------------------------------------*/

static const char usbhs_driver_name[] = USBHS_DRIVER_NAME;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static u64 usbhs_dmamask = DMA_BIT_MASK(32);

/*-------------------------------------------------------------------------*/

static inline void usbhs_write(void __iomem *base, u32 reg, u32 val)
{
<<<<<<< HEAD
	__raw_writel(val, base + reg);
=======
	writel_relaxed(val, base + reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline u32 usbhs_read(void __iomem *base, u32 reg)
{
<<<<<<< HEAD
	return __raw_readl(base + reg);
}

static inline void usbhs_writeb(void __iomem *base, u8 reg, u8 val)
{
	__raw_writeb(val, base + reg);
}

static inline u8 usbhs_readb(void __iomem *base, u8 reg)
{
	return __raw_readb(base + reg);
=======
	return readl_relaxed(base + reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
=======
/*
 * Map 'enum usbhs_omap_port_mode' found in <linux/platform_data/usb-omap.h>
 * to the device tree binding portN-mode found in
 * 'Documentation/devicetree/bindings/mfd/omap-usb-host.txt'
 */
static const char * const port_modes[] = {
	[OMAP_USBHS_PORT_MODE_UNUSED]	= "",
	[OMAP_EHCI_PORT_MODE_PHY]	= "ehci-phy",
	[OMAP_EHCI_PORT_MODE_TLL]	= "ehci-tll",
	[OMAP_EHCI_PORT_MODE_HSIC]	= "ehci-hsic",
	[OMAP_OHCI_PORT_MODE_PHY_6PIN_DATSE0]	= "ohci-phy-6pin-datse0",
	[OMAP_OHCI_PORT_MODE_PHY_6PIN_DPDM]	= "ohci-phy-6pin-dpdm",
	[OMAP_OHCI_PORT_MODE_PHY_3PIN_DATSE0]	= "ohci-phy-3pin-datse0",
	[OMAP_OHCI_PORT_MODE_PHY_4PIN_DPDM]	= "ohci-phy-4pin-dpdm",
	[OMAP_OHCI_PORT_MODE_TLL_6PIN_DATSE0]	= "ohci-tll-6pin-datse0",
	[OMAP_OHCI_PORT_MODE_TLL_6PIN_DPDM]	= "ohci-tll-6pin-dpdm",
	[OMAP_OHCI_PORT_MODE_TLL_3PIN_DATSE0]	= "ohci-tll-3pin-datse0",
	[OMAP_OHCI_PORT_MODE_TLL_4PIN_DPDM]	= "ohci-tll-4pin-dpdm",
	[OMAP_OHCI_PORT_MODE_TLL_2PIN_DATSE0]	= "ohci-tll-2pin-datse0",
	[OMAP_OHCI_PORT_MODE_TLL_2PIN_DPDM]	= "ohci-tll-2pin-dpdm",
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct platform_device *omap_usbhs_alloc_child(const char *name,
			struct resource	*res, int num_resources, void *pdata,
			size_t pdata_size, struct device *dev)
{
	struct platform_device	*child;
	int			ret;

	child = platform_device_alloc(name, 0);

	if (!child) {
		dev_err(dev, "platform_device_alloc %s failed\n", name);
		goto err_end;
	}

	ret = platform_device_add_resources(child, res, num_resources);
	if (ret) {
		dev_err(dev, "platform_device_add_resources failed\n");
		goto err_alloc;
	}

	ret = platform_device_add_data(child, pdata, pdata_size);
	if (ret) {
		dev_err(dev, "platform_device_add_data failed\n");
		goto err_alloc;
	}

	child->dev.dma_mask		= &usbhs_dmamask;
	dma_set_coherent_mask(&child->dev, DMA_BIT_MASK(32));
	child->dev.parent		= dev;

	ret = platform_device_add(child);
	if (ret) {
		dev_err(dev, "platform_device_add failed\n");
		goto err_alloc;
	}

	return child;

err_alloc:
	platform_device_put(child);

err_end:
	return NULL;
}

static int omap_usbhs_alloc_children(struct platform_device *pdev)
{
	struct device				*dev = &pdev->dev;
<<<<<<< HEAD
	struct usbhs_hcd_omap			*omap;
	struct ehci_hcd_omap_platform_data	*ehci_data;
	struct ohci_hcd_omap_platform_data	*ohci_data;
=======
	struct usbhs_omap_platform_data		*pdata = dev_get_platdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct platform_device			*ehci;
	struct platform_device			*ohci;
	struct resource				*res;
	struct resource				resources[2];
	int					ret;

<<<<<<< HEAD
	omap = platform_get_drvdata(pdev);
	ehci_data = omap->platdata.ehci_data;
	ohci_data = omap->platdata.ohci_data;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "ehci");
	if (!res) {
		dev_err(dev, "EHCI get resource IORESOURCE_MEM failed\n");
		ret = -ENODEV;
		goto err_end;
	}
	resources[0] = *res;

	res = platform_get_resource_byname(pdev, IORESOURCE_IRQ, "ehci-irq");
	if (!res) {
		dev_err(dev, " EHCI get resource IORESOURCE_IRQ failed\n");
		ret = -ENODEV;
		goto err_end;
	}
	resources[1] = *res;

<<<<<<< HEAD
	ehci = omap_usbhs_alloc_child(OMAP_EHCI_DEVICE, resources, 2, ehci_data,
		sizeof(*ehci_data), dev);
=======
	ehci = omap_usbhs_alloc_child(OMAP_EHCI_DEVICE, resources, 2, pdata,
		sizeof(*pdata), dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!ehci) {
		dev_err(dev, "omap_usbhs_alloc_child failed\n");
		ret = -ENOMEM;
		goto err_end;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "ohci");
	if (!res) {
		dev_err(dev, "OHCI get resource IORESOURCE_MEM failed\n");
		ret = -ENODEV;
		goto err_ehci;
	}
	resources[0] = *res;

	res = platform_get_resource_byname(pdev, IORESOURCE_IRQ, "ohci-irq");
	if (!res) {
		dev_err(dev, "OHCI get resource IORESOURCE_IRQ failed\n");
		ret = -ENODEV;
		goto err_ehci;
	}
	resources[1] = *res;

<<<<<<< HEAD
	ohci = omap_usbhs_alloc_child(OMAP_OHCI_DEVICE, resources, 2, ohci_data,
		sizeof(*ohci_data), dev);
=======
	ohci = omap_usbhs_alloc_child(OMAP_OHCI_DEVICE, resources, 2, pdata,
		sizeof(*pdata), dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ohci) {
		dev_err(dev, "omap_usbhs_alloc_child failed\n");
		ret = -ENOMEM;
		goto err_ehci;
	}

	return 0;

err_ehci:
	platform_device_unregister(ehci);

err_end:
	return ret;
}

static bool is_ohci_port(enum usbhs_omap_port_mode pmode)
{
	switch (pmode) {
	case OMAP_OHCI_PORT_MODE_PHY_6PIN_DATSE0:
	case OMAP_OHCI_PORT_MODE_PHY_6PIN_DPDM:
	case OMAP_OHCI_PORT_MODE_PHY_3PIN_DATSE0:
	case OMAP_OHCI_PORT_MODE_PHY_4PIN_DPDM:
	case OMAP_OHCI_PORT_MODE_TLL_6PIN_DATSE0:
	case OMAP_OHCI_PORT_MODE_TLL_6PIN_DPDM:
	case OMAP_OHCI_PORT_MODE_TLL_3PIN_DATSE0:
	case OMAP_OHCI_PORT_MODE_TLL_4PIN_DPDM:
	case OMAP_OHCI_PORT_MODE_TLL_2PIN_DATSE0:
	case OMAP_OHCI_PORT_MODE_TLL_2PIN_DPDM:
		return true;

	default:
		return false;
	}
}

<<<<<<< HEAD
/*
 * convert the port-mode enum to a value we can use in the FSLSMODE
 * field of USBTLL_CHANNEL_CONF
 */
static unsigned ohci_omap3_fslsmode(enum usbhs_omap_port_mode mode)
{
	switch (mode) {
	case OMAP_USBHS_PORT_MODE_UNUSED:
	case OMAP_OHCI_PORT_MODE_PHY_6PIN_DATSE0:
		return OMAP_TLL_FSLSMODE_6PIN_PHY_DAT_SE0;

	case OMAP_OHCI_PORT_MODE_PHY_6PIN_DPDM:
		return OMAP_TLL_FSLSMODE_6PIN_PHY_DP_DM;

	case OMAP_OHCI_PORT_MODE_PHY_3PIN_DATSE0:
		return OMAP_TLL_FSLSMODE_3PIN_PHY;

	case OMAP_OHCI_PORT_MODE_PHY_4PIN_DPDM:
		return OMAP_TLL_FSLSMODE_4PIN_PHY;

	case OMAP_OHCI_PORT_MODE_TLL_6PIN_DATSE0:
		return OMAP_TLL_FSLSMODE_6PIN_TLL_DAT_SE0;

	case OMAP_OHCI_PORT_MODE_TLL_6PIN_DPDM:
		return OMAP_TLL_FSLSMODE_6PIN_TLL_DP_DM;

	case OMAP_OHCI_PORT_MODE_TLL_3PIN_DATSE0:
		return OMAP_TLL_FSLSMODE_3PIN_TLL;

	case OMAP_OHCI_PORT_MODE_TLL_4PIN_DPDM:
		return OMAP_TLL_FSLSMODE_4PIN_TLL;

	case OMAP_OHCI_PORT_MODE_TLL_2PIN_DATSE0:
		return OMAP_TLL_FSLSMODE_2PIN_TLL_DAT_SE0;

	case OMAP_OHCI_PORT_MODE_TLL_2PIN_DPDM:
		return OMAP_TLL_FSLSMODE_2PIN_DAT_DP_DM;
	default:
		pr_warning("Invalid port mode, using default\n");
		return OMAP_TLL_FSLSMODE_6PIN_PHY_DAT_SE0;
	}
}

static void usbhs_omap_tll_init(struct device *dev, u8 tll_channel_count)
{
	struct usbhs_hcd_omap		*omap = dev_get_drvdata(dev);
	struct usbhs_omap_platform_data	*pdata = dev->platform_data;
	unsigned			reg;
	int				i;

	/* Program Common TLL register */
	reg = usbhs_read(omap->tll_base, OMAP_TLL_SHARED_CONF);
	reg |= (OMAP_TLL_SHARED_CONF_FCLK_IS_ON
		| OMAP_TLL_SHARED_CONF_USB_DIVRATION);
	reg &= ~OMAP_TLL_SHARED_CONF_USB_90D_DDR_EN;
	reg &= ~OMAP_TLL_SHARED_CONF_USB_180D_SDR_EN;

	usbhs_write(omap->tll_base, OMAP_TLL_SHARED_CONF, reg);

	/* Enable channels now */
	for (i = 0; i < tll_channel_count; i++) {
		reg = usbhs_read(omap->tll_base,
				OMAP_TLL_CHANNEL_CONF(i));

		if (is_ohci_port(pdata->port_mode[i])) {
			reg |= ohci_omap3_fslsmode(pdata->port_mode[i])
				<< OMAP_TLL_CHANNEL_CONF_FSLSMODE_SHIFT;
			reg |= OMAP_TLL_CHANNEL_CONF_CHANMODE_FSLS;
		} else if (pdata->port_mode[i] == OMAP_EHCI_PORT_MODE_TLL) {

			/* Disable AutoIdle, BitStuffing and use SDR Mode */
			reg &= ~(OMAP_TLL_CHANNEL_CONF_UTMIAUTOIDLE
				| OMAP_TLL_CHANNEL_CONF_ULPINOBITSTUFF
				| OMAP_TLL_CHANNEL_CONF_ULPIDDRMODE);

		} else
			continue;

		reg |= OMAP_TLL_CHANNEL_CONF_CHANEN;
		usbhs_write(omap->tll_base,
				OMAP_TLL_CHANNEL_CONF(i), reg);

		usbhs_writeb(omap->tll_base,
				OMAP_TLL_ULPI_SCRATCH_REGISTER(i), 0xbe);
	}
}

static int usbhs_runtime_resume(struct device *dev)
{
	struct usbhs_hcd_omap		*omap = dev_get_drvdata(dev);
	struct usbhs_omap_platform_data	*pdata = &omap->platdata;
	unsigned long			flags;

	dev_dbg(dev, "usbhs_runtime_resume\n");

	if (!pdata) {
		dev_dbg(dev, "missing platform_data\n");
		return  -ENODEV;
	}

	spin_lock_irqsave(&omap->lock, flags);

	if (omap->ehci_logic_fck && !IS_ERR(omap->ehci_logic_fck))
		clk_enable(omap->ehci_logic_fck);

	if (is_ehci_tll_mode(pdata->port_mode[0])) {
		clk_enable(omap->usbhost_p1_fck);
		clk_enable(omap->usbtll_p1_fck);
	}
	if (is_ehci_tll_mode(pdata->port_mode[1])) {
		clk_enable(omap->usbhost_p2_fck);
		clk_enable(omap->usbtll_p2_fck);
	}
	clk_enable(omap->utmi_p1_fck);
	clk_enable(omap->utmi_p2_fck);

	spin_unlock_irqrestore(&omap->lock, flags);

=======
static int usbhs_runtime_resume(struct device *dev)
{
	struct usbhs_hcd_omap		*omap = dev_get_drvdata(dev);
	struct usbhs_omap_platform_data	*pdata = omap->pdata;
	int i, r;

	dev_dbg(dev, "usbhs_runtime_resume\n");

	omap_tll_enable(pdata);

	if (!IS_ERR(omap->ehci_logic_fck))
		clk_prepare_enable(omap->ehci_logic_fck);

	for (i = 0; i < omap->nports; i++) {
		switch (pdata->port_mode[i]) {
		case OMAP_EHCI_PORT_MODE_HSIC:
			if (!IS_ERR(omap->hsic60m_clk[i])) {
				r = clk_prepare_enable(omap->hsic60m_clk[i]);
				if (r) {
					dev_err(dev,
					 "Can't enable port %d hsic60m clk:%d\n",
					 i, r);
				}
			}

			if (!IS_ERR(omap->hsic480m_clk[i])) {
				r = clk_prepare_enable(omap->hsic480m_clk[i]);
				if (r) {
					dev_err(dev,
					 "Can't enable port %d hsic480m clk:%d\n",
					 i, r);
				}
			}
			fallthrough;	/* as HSIC mode needs utmi_clk */

		case OMAP_EHCI_PORT_MODE_TLL:
			if (!IS_ERR(omap->utmi_clk[i])) {
				r = clk_prepare_enable(omap->utmi_clk[i]);
				if (r) {
					dev_err(dev,
					 "Can't enable port %d clk : %d\n",
					 i, r);
				}
			}
			break;
		default:
			break;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int usbhs_runtime_suspend(struct device *dev)
{
	struct usbhs_hcd_omap		*omap = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct usbhs_omap_platform_data	*pdata = &omap->platdata;
	unsigned long			flags;

	dev_dbg(dev, "usbhs_runtime_suspend\n");

	if (!pdata) {
		dev_dbg(dev, "missing platform_data\n");
		return  -ENODEV;
	}

	spin_lock_irqsave(&omap->lock, flags);

	if (is_ehci_tll_mode(pdata->port_mode[0])) {
		clk_disable(omap->usbhost_p1_fck);
		clk_disable(omap->usbtll_p1_fck);
	}
	if (is_ehci_tll_mode(pdata->port_mode[1])) {
		clk_disable(omap->usbhost_p2_fck);
		clk_disable(omap->usbtll_p2_fck);
	}
	clk_disable(omap->utmi_p2_fck);
	clk_disable(omap->utmi_p1_fck);

	if (omap->ehci_logic_fck && !IS_ERR(omap->ehci_logic_fck))
		clk_disable(omap->ehci_logic_fck);

	spin_unlock_irqrestore(&omap->lock, flags);
=======
	struct usbhs_omap_platform_data	*pdata = omap->pdata;
	int i;

	dev_dbg(dev, "usbhs_runtime_suspend\n");

	for (i = 0; i < omap->nports; i++) {
		switch (pdata->port_mode[i]) {
		case OMAP_EHCI_PORT_MODE_HSIC:
			if (!IS_ERR(omap->hsic60m_clk[i]))
				clk_disable_unprepare(omap->hsic60m_clk[i]);

			if (!IS_ERR(omap->hsic480m_clk[i]))
				clk_disable_unprepare(omap->hsic480m_clk[i]);
			fallthrough;	/* as utmi_clks were used in HSIC mode */

		case OMAP_EHCI_PORT_MODE_TLL:
			if (!IS_ERR(omap->utmi_clk[i]))
				clk_disable_unprepare(omap->utmi_clk[i]);
			break;
		default:
			break;
		}
	}

	if (!IS_ERR(omap->ehci_logic_fck))
		clk_disable_unprepare(omap->ehci_logic_fck);

	omap_tll_disable(pdata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void omap_usbhs_init(struct device *dev)
{
	struct usbhs_hcd_omap		*omap = dev_get_drvdata(dev);
	struct usbhs_omap_platform_data	*pdata = &omap->platdata;
	unsigned long			flags;
=======
static unsigned omap_usbhs_rev1_hostconfig(struct usbhs_hcd_omap *omap,
						unsigned reg)
{
	struct usbhs_omap_platform_data	*pdata = omap->pdata;
	int i;

	for (i = 0; i < omap->nports; i++) {
		switch (pdata->port_mode[i]) {
		case OMAP_USBHS_PORT_MODE_UNUSED:
			reg &= ~(OMAP_UHH_HOSTCONFIG_P1_CONNECT_STATUS << i);
			break;
		case OMAP_EHCI_PORT_MODE_PHY:
			if (pdata->single_ulpi_bypass)
				break;

			if (i == 0)
				reg &= ~OMAP_UHH_HOSTCONFIG_ULPI_P1_BYPASS;
			else
				reg &= ~(OMAP_UHH_HOSTCONFIG_ULPI_P2_BYPASS
								<< (i-1));
			break;
		default:
			if (pdata->single_ulpi_bypass)
				break;

			if (i == 0)
				reg |= OMAP_UHH_HOSTCONFIG_ULPI_P1_BYPASS;
			else
				reg |= OMAP_UHH_HOSTCONFIG_ULPI_P2_BYPASS
								<< (i-1);
			break;
		}
	}

	if (pdata->single_ulpi_bypass) {
		/* bypass ULPI only if none of the ports use PHY mode */
		reg |= OMAP_UHH_HOSTCONFIG_ULPI_BYPASS;

		for (i = 0; i < omap->nports; i++) {
			if (is_ehci_phy_mode(pdata->port_mode[i])) {
				reg &= ~OMAP_UHH_HOSTCONFIG_ULPI_BYPASS;
				break;
			}
		}
	}

	return reg;
}

static unsigned omap_usbhs_rev2_hostconfig(struct usbhs_hcd_omap *omap,
						unsigned reg)
{
	struct usbhs_omap_platform_data	*pdata = omap->pdata;
	int i;

	for (i = 0; i < omap->nports; i++) {
		/* Clear port mode fields for PHY mode */
		reg &= ~(OMAP4_P1_MODE_CLEAR << 2 * i);

		if (is_ehci_tll_mode(pdata->port_mode[i]) ||
				(is_ohci_port(pdata->port_mode[i])))
			reg |= OMAP4_P1_MODE_TLL << 2 * i;
		else if (is_ehci_hsic_mode(pdata->port_mode[i]))
			reg |= OMAP4_P1_MODE_HSIC << 2 * i;
	}

	return reg;
}

static void omap_usbhs_init(struct device *dev)
{
	struct usbhs_hcd_omap		*omap = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned			reg;

	dev_dbg(dev, "starting TI HSUSB Controller\n");

	pm_runtime_get_sync(dev);
<<<<<<< HEAD
	spin_lock_irqsave(&omap->lock, flags);

	omap->usbhs_rev = usbhs_read(omap->uhh_base, OMAP_UHH_REVISION);
	dev_dbg(dev, "OMAP UHH_REVISION 0x%x\n", omap->usbhs_rev);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	reg = usbhs_read(omap->uhh_base, OMAP_UHH_HOSTCONFIG);
	/* setup ULPI bypass and burst configurations */
	reg |= (OMAP_UHH_HOSTCONFIG_INCR4_BURST_EN
			| OMAP_UHH_HOSTCONFIG_INCR8_BURST_EN
			| OMAP_UHH_HOSTCONFIG_INCR16_BURST_EN);
	reg |= OMAP4_UHH_HOSTCONFIG_APP_START_CLK;
	reg &= ~OMAP_UHH_HOSTCONFIG_INCRX_ALIGN_EN;

<<<<<<< HEAD
	if (is_omap_usbhs_rev1(omap)) {
		if (pdata->port_mode[0] == OMAP_USBHS_PORT_MODE_UNUSED)
			reg &= ~OMAP_UHH_HOSTCONFIG_P1_CONNECT_STATUS;
		if (pdata->port_mode[1] == OMAP_USBHS_PORT_MODE_UNUSED)
			reg &= ~OMAP_UHH_HOSTCONFIG_P2_CONNECT_STATUS;
		if (pdata->port_mode[2] == OMAP_USBHS_PORT_MODE_UNUSED)
			reg &= ~OMAP_UHH_HOSTCONFIG_P3_CONNECT_STATUS;

		/* Bypass the TLL module for PHY mode operation */
		if (cpu_is_omap3430() && (omap_rev() <= OMAP3430_REV_ES2_1)) {
			dev_dbg(dev, "OMAP3 ES version <= ES2.1\n");
			if (is_ehci_phy_mode(pdata->port_mode[0]) ||
				is_ehci_phy_mode(pdata->port_mode[1]) ||
					is_ehci_phy_mode(pdata->port_mode[2]))
				reg &= ~OMAP_UHH_HOSTCONFIG_ULPI_BYPASS;
			else
				reg |= OMAP_UHH_HOSTCONFIG_ULPI_BYPASS;
		} else {
			dev_dbg(dev, "OMAP3 ES version > ES2.1\n");
			if (is_ehci_phy_mode(pdata->port_mode[0]))
				reg &= ~OMAP_UHH_HOSTCONFIG_ULPI_P1_BYPASS;
			else
				reg |= OMAP_UHH_HOSTCONFIG_ULPI_P1_BYPASS;
			if (is_ehci_phy_mode(pdata->port_mode[1]))
				reg &= ~OMAP_UHH_HOSTCONFIG_ULPI_P2_BYPASS;
			else
				reg |= OMAP_UHH_HOSTCONFIG_ULPI_P2_BYPASS;
			if (is_ehci_phy_mode(pdata->port_mode[2]))
				reg &= ~OMAP_UHH_HOSTCONFIG_ULPI_P3_BYPASS;
			else
				reg |= OMAP_UHH_HOSTCONFIG_ULPI_P3_BYPASS;
		}
	} else if (is_omap_usbhs_rev2(omap)) {
		/* Clear port mode fields for PHY mode*/
		reg &= ~OMAP4_P1_MODE_CLEAR;
		reg &= ~OMAP4_P2_MODE_CLEAR;

		if (is_ehci_tll_mode(pdata->port_mode[0]) ||
			(is_ohci_port(pdata->port_mode[0])))
			reg |= OMAP4_P1_MODE_TLL;
		else if (is_ehci_hsic_mode(pdata->port_mode[0]))
			reg |= OMAP4_P1_MODE_HSIC;

		if (is_ehci_tll_mode(pdata->port_mode[1]) ||
			(is_ohci_port(pdata->port_mode[1])))
			reg |= OMAP4_P2_MODE_TLL;
		else if (is_ehci_hsic_mode(pdata->port_mode[1]))
			reg |= OMAP4_P2_MODE_HSIC;
=======
	switch (omap->usbhs_rev) {
	case OMAP_USBHS_REV1:
		reg = omap_usbhs_rev1_hostconfig(omap, reg);
		break;

	case OMAP_USBHS_REV2:
		reg = omap_usbhs_rev2_hostconfig(omap, reg);
		break;

	default:	/* newer revisions */
		reg = omap_usbhs_rev2_hostconfig(omap, reg);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	usbhs_write(omap->uhh_base, OMAP_UHH_HOSTCONFIG, reg);
	dev_dbg(dev, "UHH setup done, uhh_hostconfig=%x\n", reg);

<<<<<<< HEAD
	if (is_ehci_tll_mode(pdata->port_mode[0]) ||
		is_ehci_tll_mode(pdata->port_mode[1]) ||
		is_ehci_tll_mode(pdata->port_mode[2]) ||
		(is_ohci_port(pdata->port_mode[0])) ||
		(is_ohci_port(pdata->port_mode[1])) ||
		(is_ohci_port(pdata->port_mode[2]))) {

		/* Enable UTMI mode for required TLL channels */
		if (is_omap_usbhs_rev2(omap))
			usbhs_omap_tll_init(dev, OMAP_REV2_TLL_CHANNEL_COUNT);
		else
			usbhs_omap_tll_init(dev, OMAP_TLL_CHANNEL_COUNT);
	}

	spin_unlock_irqrestore(&omap->lock, flags);
	pm_runtime_put_sync(dev);
}

=======
	pm_runtime_put_sync(dev);
}

static int usbhs_omap_get_dt_pdata(struct device *dev,
					struct usbhs_omap_platform_data *pdata)
{
	int ret, i;
	struct device_node *node = dev->of_node;

	ret = of_property_read_u32(node, "num-ports", &pdata->nports);
	if (ret)
		pdata->nports = 0;

	if (pdata->nports > OMAP3_HS_USB_PORTS) {
		dev_warn(dev, "Too many num_ports <%d> in device tree. Max %d\n",
				pdata->nports, OMAP3_HS_USB_PORTS);
		return -ENODEV;
	}

	/* get port modes */
	for (i = 0; i < OMAP3_HS_USB_PORTS; i++) {
		char prop[11];
		const char *mode;

		pdata->port_mode[i] = OMAP_USBHS_PORT_MODE_UNUSED;

		snprintf(prop, sizeof(prop), "port%d-mode", i + 1);
		ret = of_property_read_string(node, prop, &mode);
		if (ret < 0)
			continue;

		/* get 'enum usbhs_omap_port_mode' from port mode string */
		ret = match_string(port_modes, ARRAY_SIZE(port_modes), mode);
		if (ret < 0) {
			dev_warn(dev, "Invalid port%d-mode \"%s\" in device tree\n",
					i, mode);
			return -ENODEV;
		}

		dev_dbg(dev, "port%d-mode: %s -> %d\n", i, mode, ret);
		pdata->port_mode[i] = ret;
	}

	/* get flags */
	pdata->single_ulpi_bypass = of_property_read_bool(node,
						"single-ulpi-bypass");

	return 0;
}

static const struct of_device_id usbhs_child_match_table[] = {
	{ .compatible = "ti,ehci-omap", },
	{ .compatible = "ti,ohci-omap3", },
	{ }
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * usbhs_omap_probe - initialize TI-based HCDs
 *
 * Allocates basic resources for this USB host controller.
<<<<<<< HEAD
 */
static int __devinit usbhs_omap_probe(struct platform_device *pdev)
{
	struct device			*dev =  &pdev->dev;
	struct usbhs_omap_platform_data	*pdata = dev->platform_data;
	struct usbhs_hcd_omap		*omap;
	struct resource			*res;
	int				ret = 0;
	int				i;

	if (!pdata) {
		dev_err(dev, "Missing platform data\n");
		ret = -ENOMEM;
		goto end_probe;
	}

	omap = kzalloc(sizeof(*omap), GFP_KERNEL);
	if (!omap) {
		dev_err(dev, "Memory allocation failed\n");
		ret = -ENOMEM;
		goto end_probe;
	}

	spin_lock_init(&omap->lock);

	for (i = 0; i < OMAP3_HS_USB_PORTS; i++)
		omap->platdata.port_mode[i] = pdata->port_mode[i];

	omap->platdata.ehci_data = pdata->ehci_data;
	omap->platdata.ohci_data = pdata->ohci_data;

	pm_runtime_enable(dev);


	for (i = 0; i < OMAP3_HS_USB_PORTS; i++)
		if (is_ehci_phy_mode(i) || is_ehci_tll_mode(i) ||
			is_ehci_hsic_mode(i)) {
			omap->ehci_logic_fck = clk_get(dev, "ehci_logic_fck");
			if (IS_ERR(omap->ehci_logic_fck)) {
				ret = PTR_ERR(omap->ehci_logic_fck);
				dev_warn(dev, "ehci_logic_fck failed:%d\n",
					 ret);
			}
			break;
		}

	omap->utmi_p1_fck = clk_get(dev, "utmi_p1_gfclk");
	if (IS_ERR(omap->utmi_p1_fck)) {
		ret = PTR_ERR(omap->utmi_p1_fck);
		dev_err(dev, "utmi_p1_gfclk failed error:%d\n",	ret);
		goto err_end;
	}

	omap->xclk60mhsp1_ck = clk_get(dev, "xclk60mhsp1_ck");
	if (IS_ERR(omap->xclk60mhsp1_ck)) {
		ret = PTR_ERR(omap->xclk60mhsp1_ck);
		dev_err(dev, "xclk60mhsp1_ck failed error:%d\n", ret);
		goto err_utmi_p1_fck;
	}

	omap->utmi_p2_fck = clk_get(dev, "utmi_p2_gfclk");
	if (IS_ERR(omap->utmi_p2_fck)) {
		ret = PTR_ERR(omap->utmi_p2_fck);
		dev_err(dev, "utmi_p2_gfclk failed error:%d\n", ret);
		goto err_xclk60mhsp1_ck;
	}

	omap->xclk60mhsp2_ck = clk_get(dev, "xclk60mhsp2_ck");
	if (IS_ERR(omap->xclk60mhsp2_ck)) {
		ret = PTR_ERR(omap->xclk60mhsp2_ck);
		dev_err(dev, "xclk60mhsp2_ck failed error:%d\n", ret);
		goto err_utmi_p2_fck;
	}

	omap->usbhost_p1_fck = clk_get(dev, "usb_host_hs_utmi_p1_clk");
	if (IS_ERR(omap->usbhost_p1_fck)) {
		ret = PTR_ERR(omap->usbhost_p1_fck);
		dev_err(dev, "usbhost_p1_fck failed error:%d\n", ret);
		goto err_xclk60mhsp2_ck;
	}

	omap->usbtll_p1_fck = clk_get(dev, "usb_tll_hs_usb_ch0_clk");
	if (IS_ERR(omap->usbtll_p1_fck)) {
		ret = PTR_ERR(omap->usbtll_p1_fck);
		dev_err(dev, "usbtll_p1_fck failed error:%d\n", ret);
		goto err_usbhost_p1_fck;
	}

	omap->usbhost_p2_fck = clk_get(dev, "usb_host_hs_utmi_p2_clk");
	if (IS_ERR(omap->usbhost_p2_fck)) {
		ret = PTR_ERR(omap->usbhost_p2_fck);
		dev_err(dev, "usbhost_p2_fck failed error:%d\n", ret);
		goto err_usbtll_p1_fck;
	}

	omap->usbtll_p2_fck = clk_get(dev, "usb_tll_hs_usb_ch1_clk");
	if (IS_ERR(omap->usbtll_p2_fck)) {
		ret = PTR_ERR(omap->usbtll_p2_fck);
		dev_err(dev, "usbtll_p2_fck failed error:%d\n", ret);
		goto err_usbhost_p2_fck;
	}

	omap->init_60m_fclk = clk_get(dev, "init_60m_fclk");
	if (IS_ERR(omap->init_60m_fclk)) {
		ret = PTR_ERR(omap->init_60m_fclk);
		dev_err(dev, "init_60m_fclk failed error:%d\n", ret);
		goto err_usbtll_p2_fck;
	}

	if (is_ehci_phy_mode(pdata->port_mode[0])) {
		/* for OMAP3 , the clk set paretn fails */
		ret = clk_set_parent(omap->utmi_p1_fck,
					omap->xclk60mhsp1_ck);
		if (ret != 0)
			dev_err(dev, "xclk60mhsp1_ck set parent"
				"failed error:%d\n", ret);
	} else if (is_ehci_tll_mode(pdata->port_mode[0])) {
		ret = clk_set_parent(omap->utmi_p1_fck,
					omap->init_60m_fclk);
		if (ret != 0)
			dev_err(dev, "init_60m_fclk set parent"
				"failed error:%d\n", ret);
	}

	if (is_ehci_phy_mode(pdata->port_mode[1])) {
		ret = clk_set_parent(omap->utmi_p2_fck,
					omap->xclk60mhsp2_ck);
		if (ret != 0)
			dev_err(dev, "xclk60mhsp2_ck set parent"
					"failed error:%d\n", ret);
	} else if (is_ehci_tll_mode(pdata->port_mode[1])) {
		ret = clk_set_parent(omap->utmi_p2_fck,
						omap->init_60m_fclk);
		if (ret != 0)
			dev_err(dev, "init_60m_fclk set parent"
				"failed error:%d\n", ret);
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "uhh");
	if (!res) {
		dev_err(dev, "UHH EHCI get resource failed\n");
		ret = -ENODEV;
		goto err_init_60m_fclk;
	}

	omap->uhh_base = ioremap(res->start, resource_size(res));
	if (!omap->uhh_base) {
		dev_err(dev, "UHH ioremap failed\n");
		ret = -ENOMEM;
		goto err_init_60m_fclk;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "tll");
	if (!res) {
		dev_err(dev, "UHH EHCI get resource failed\n");
		ret = -ENODEV;
		goto err_tll;
	}

	omap->tll_base = ioremap(res->start, resource_size(res));
	if (!omap->tll_base) {
		dev_err(dev, "TLL ioremap failed\n");
		ret = -ENOMEM;
		goto err_tll;
	}

	platform_set_drvdata(pdev, omap);

	omap_usbhs_init(dev);
	ret = omap_usbhs_alloc_children(pdev);
	if (ret) {
		dev_err(dev, "omap_usbhs_alloc_children failed\n");
		goto err_alloc;
	}

	goto end_probe;

err_alloc:
	iounmap(omap->tll_base);

err_tll:
	iounmap(omap->uhh_base);

err_init_60m_fclk:
	clk_put(omap->init_60m_fclk);

err_usbtll_p2_fck:
	clk_put(omap->usbtll_p2_fck);

err_usbhost_p2_fck:
	clk_put(omap->usbhost_p2_fck);

err_usbtll_p1_fck:
	clk_put(omap->usbtll_p1_fck);

err_usbhost_p1_fck:
	clk_put(omap->usbhost_p1_fck);

err_xclk60mhsp2_ck:
	clk_put(omap->xclk60mhsp2_ck);

err_utmi_p2_fck:
	clk_put(omap->utmi_p2_fck);

err_xclk60mhsp1_ck:
	clk_put(omap->xclk60mhsp1_ck);

err_utmi_p1_fck:
	clk_put(omap->utmi_p1_fck);

err_end:
	clk_put(omap->ehci_logic_fck);
	pm_runtime_disable(dev);
	kfree(omap);

end_probe:
	return ret;
}

=======
 *
 * @pdev: Pointer to this device's platform device structure
 */
static int usbhs_omap_probe(struct platform_device *pdev)
{
	struct device			*dev =  &pdev->dev;
	struct usbhs_omap_platform_data	*pdata = dev_get_platdata(dev);
	struct usbhs_hcd_omap		*omap;
	int				ret = 0;
	int				i;
	bool				need_logic_fck;

	if (dev->of_node) {
		/* For DT boot we populate platform data from OF node */
		pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata)
			return -ENOMEM;

		ret = usbhs_omap_get_dt_pdata(dev, pdata);
		if (ret)
			return ret;

		dev->platform_data = pdata;
	}

	if (!pdata) {
		dev_err(dev, "Missing platform data\n");
		return -ENODEV;
	}

	if (pdata->nports > OMAP3_HS_USB_PORTS) {
		dev_info(dev, "Too many num_ports <%d> in platform_data. Max %d\n",
				pdata->nports, OMAP3_HS_USB_PORTS);
		return -ENODEV;
	}

	omap = devm_kzalloc(dev, sizeof(*omap), GFP_KERNEL);
	if (!omap) {
		dev_err(dev, "Memory allocation failed\n");
		return -ENOMEM;
	}

	omap->uhh_base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(omap->uhh_base))
		return PTR_ERR(omap->uhh_base);

	omap->pdata = pdata;

	/* Initialize the TLL subsystem */
	omap_tll_init(pdata);

	pm_runtime_enable(dev);

	platform_set_drvdata(pdev, omap);
	pm_runtime_get_sync(dev);

	omap->usbhs_rev = usbhs_read(omap->uhh_base, OMAP_UHH_REVISION);

	/* we need to call runtime suspend before we update omap->nports
	 * to prevent unbalanced clk_disable()
	 */
	pm_runtime_put_sync(dev);

	/*
	 * If platform data contains nports then use that
	 * else make out number of ports from USBHS revision
	 */
	if (pdata->nports) {
		omap->nports = pdata->nports;
	} else {
		switch (omap->usbhs_rev) {
		case OMAP_USBHS_REV1:
			omap->nports = 3;
			break;
		case OMAP_USBHS_REV2:
			omap->nports = 2;
			break;
		default:
			omap->nports = OMAP3_HS_USB_PORTS;
			dev_dbg(dev,
			 "USB HOST Rev:0x%x not recognized, assuming %d ports\n",
			 omap->usbhs_rev, omap->nports);
			break;
		}
		pdata->nports = omap->nports;
	}

	i = sizeof(struct clk *) * omap->nports;
	omap->utmi_clk = devm_kzalloc(dev, i, GFP_KERNEL);
	omap->hsic480m_clk = devm_kzalloc(dev, i, GFP_KERNEL);
	omap->hsic60m_clk = devm_kzalloc(dev, i, GFP_KERNEL);

	if (!omap->utmi_clk || !omap->hsic480m_clk || !omap->hsic60m_clk) {
		dev_err(dev, "Memory allocation failed\n");
		ret = -ENOMEM;
		goto err_mem;
	}

	/* Set all clocks as invalid to begin with */
	omap->ehci_logic_fck = ERR_PTR(-ENODEV);
	omap->init_60m_fclk = ERR_PTR(-ENODEV);
	omap->utmi_p1_gfclk = ERR_PTR(-ENODEV);
	omap->utmi_p2_gfclk = ERR_PTR(-ENODEV);
	omap->xclk60mhsp1_ck = ERR_PTR(-ENODEV);
	omap->xclk60mhsp2_ck = ERR_PTR(-ENODEV);

	for (i = 0; i < omap->nports; i++) {
		omap->utmi_clk[i] = ERR_PTR(-ENODEV);
		omap->hsic480m_clk[i] = ERR_PTR(-ENODEV);
		omap->hsic60m_clk[i] = ERR_PTR(-ENODEV);
	}

	/* for OMAP3 i.e. USBHS REV1 */
	if (omap->usbhs_rev == OMAP_USBHS_REV1) {
		need_logic_fck = false;
		for (i = 0; i < omap->nports; i++) {
			if (is_ehci_phy_mode(pdata->port_mode[i]) ||
			    is_ehci_tll_mode(pdata->port_mode[i]) ||
			    is_ehci_hsic_mode(pdata->port_mode[i]))

				need_logic_fck |= true;
		}

		if (need_logic_fck) {
			omap->ehci_logic_fck = devm_clk_get(dev,
							    "usbhost_120m_fck");
			if (IS_ERR(omap->ehci_logic_fck)) {
				ret = PTR_ERR(omap->ehci_logic_fck);
				dev_err(dev, "usbhost_120m_fck failed:%d\n",
					ret);
				goto err_mem;
			}
		}
		goto initialize;
	}

	/* for OMAP4+ i.e. USBHS REV2+ */
	omap->utmi_p1_gfclk = devm_clk_get(dev, "utmi_p1_gfclk");
	if (IS_ERR(omap->utmi_p1_gfclk)) {
		ret = PTR_ERR(omap->utmi_p1_gfclk);
		dev_err(dev, "utmi_p1_gfclk failed error:%d\n", ret);
		goto err_mem;
	}

	omap->utmi_p2_gfclk = devm_clk_get(dev, "utmi_p2_gfclk");
	if (IS_ERR(omap->utmi_p2_gfclk)) {
		ret = PTR_ERR(omap->utmi_p2_gfclk);
		dev_err(dev, "utmi_p2_gfclk failed error:%d\n", ret);
		goto err_mem;
	}

	omap->xclk60mhsp1_ck = devm_clk_get(dev, "refclk_60m_ext_p1");
	if (IS_ERR(omap->xclk60mhsp1_ck)) {
		ret = PTR_ERR(omap->xclk60mhsp1_ck);
		dev_err(dev, "refclk_60m_ext_p1 failed error:%d\n", ret);
		goto err_mem;
	}

	omap->xclk60mhsp2_ck = devm_clk_get(dev, "refclk_60m_ext_p2");
	if (IS_ERR(omap->xclk60mhsp2_ck)) {
		ret = PTR_ERR(omap->xclk60mhsp2_ck);
		dev_err(dev, "refclk_60m_ext_p2 failed error:%d\n", ret);
		goto err_mem;
	}

	omap->init_60m_fclk = devm_clk_get(dev, "refclk_60m_int");
	if (IS_ERR(omap->init_60m_fclk)) {
		ret = PTR_ERR(omap->init_60m_fclk);
		dev_err(dev, "refclk_60m_int failed error:%d\n", ret);
		goto err_mem;
	}

	for (i = 0; i < omap->nports; i++) {
		char clkname[40];

		/* clock names are indexed from 1*/
		snprintf(clkname, sizeof(clkname),
				"usb_host_hs_utmi_p%d_clk", i + 1);

		/* If a clock is not found we won't bail out as not all
		 * platforms have all clocks and we can function without
		 * them
		 */
		omap->utmi_clk[i] = devm_clk_get(dev, clkname);
		if (IS_ERR(omap->utmi_clk[i])) {
			ret = PTR_ERR(omap->utmi_clk[i]);
			dev_err(dev, "Failed to get clock : %s : %d\n",
				clkname, ret);
			goto err_mem;
		}

		snprintf(clkname, sizeof(clkname),
				"usb_host_hs_hsic480m_p%d_clk", i + 1);
		omap->hsic480m_clk[i] = devm_clk_get(dev, clkname);
		if (IS_ERR(omap->hsic480m_clk[i])) {
			ret = PTR_ERR(omap->hsic480m_clk[i]);
			dev_err(dev, "Failed to get clock : %s : %d\n",
				clkname, ret);
			goto err_mem;
		}

		snprintf(clkname, sizeof(clkname),
				"usb_host_hs_hsic60m_p%d_clk", i + 1);
		omap->hsic60m_clk[i] = devm_clk_get(dev, clkname);
		if (IS_ERR(omap->hsic60m_clk[i])) {
			ret = PTR_ERR(omap->hsic60m_clk[i]);
			dev_err(dev, "Failed to get clock : %s : %d\n",
				clkname, ret);
			goto err_mem;
		}
	}

	if (is_ehci_phy_mode(pdata->port_mode[0])) {
		ret = clk_set_parent(omap->utmi_p1_gfclk,
					omap->xclk60mhsp1_ck);
		if (ret != 0) {
			dev_err(dev, "xclk60mhsp1_ck set parent failed: %d\n",
				ret);
			goto err_mem;
		}
	} else if (is_ehci_tll_mode(pdata->port_mode[0])) {
		ret = clk_set_parent(omap->utmi_p1_gfclk,
					omap->init_60m_fclk);
		if (ret != 0) {
			dev_err(dev, "P0 init_60m_fclk set parent failed: %d\n",
				ret);
			goto err_mem;
		}
	}

	if (is_ehci_phy_mode(pdata->port_mode[1])) {
		ret = clk_set_parent(omap->utmi_p2_gfclk,
					omap->xclk60mhsp2_ck);
		if (ret != 0) {
			dev_err(dev, "xclk60mhsp2_ck set parent failed: %d\n",
				ret);
			goto err_mem;
		}
	} else if (is_ehci_tll_mode(pdata->port_mode[1])) {
		ret = clk_set_parent(omap->utmi_p2_gfclk,
						omap->init_60m_fclk);
		if (ret != 0) {
			dev_err(dev, "P1 init_60m_fclk set parent failed: %d\n",
				ret);
			goto err_mem;
		}
	}

initialize:
	omap_usbhs_init(dev);

	if (dev->of_node) {
		ret = of_platform_populate(dev->of_node,
				usbhs_child_match_table, NULL, dev);

		if (ret) {
			dev_err(dev, "Failed to create DT children: %d\n", ret);
			goto err_mem;
		}

	} else {
		ret = omap_usbhs_alloc_children(pdev);
		if (ret) {
			dev_err(dev, "omap_usbhs_alloc_children failed: %d\n",
						ret);
			goto err_mem;
		}
	}

	return 0;

err_mem:
	pm_runtime_disable(dev);

	return ret;
}

static int usbhs_omap_remove_child(struct device *dev, void *data)
{
	dev_info(dev, "unregistering\n");
	platform_device_unregister(to_platform_device(dev));
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * usbhs_omap_remove - shutdown processing for UHH & TLL HCDs
 * @pdev: USB Host Controller being removed
 *
 * Reverses the effect of usbhs_omap_probe().
 */
<<<<<<< HEAD
static int __devexit usbhs_omap_remove(struct platform_device *pdev)
{
	struct usbhs_hcd_omap *omap = platform_get_drvdata(pdev);

	iounmap(omap->tll_base);
	iounmap(omap->uhh_base);
	clk_put(omap->init_60m_fclk);
	clk_put(omap->usbtll_p2_fck);
	clk_put(omap->usbhost_p2_fck);
	clk_put(omap->usbtll_p1_fck);
	clk_put(omap->usbhost_p1_fck);
	clk_put(omap->xclk60mhsp2_ck);
	clk_put(omap->utmi_p2_fck);
	clk_put(omap->xclk60mhsp1_ck);
	clk_put(omap->utmi_p1_fck);
	clk_put(omap->ehci_logic_fck);
	pm_runtime_disable(&pdev->dev);
	kfree(omap);

	return 0;
=======
static void usbhs_omap_remove(struct platform_device *pdev)
{
	pm_runtime_disable(&pdev->dev);

	/* remove children */
	device_for_each_child(&pdev->dev, NULL, usbhs_omap_remove_child);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct dev_pm_ops usbhsomap_dev_pm_ops = {
	.runtime_suspend	= usbhs_runtime_suspend,
	.runtime_resume		= usbhs_runtime_resume,
};

<<<<<<< HEAD
static struct platform_driver usbhs_omap_driver = {
	.driver = {
		.name		= (char *)usbhs_driver_name,
		.owner		= THIS_MODULE,
		.pm		= &usbhsomap_dev_pm_ops,
	},
	.remove		= __exit_p(usbhs_omap_remove),
};

MODULE_AUTHOR("Keshava Munegowda <keshava_mgowda@ti.com>");
MODULE_ALIAS("platform:" USBHS_DRIVER_NAME);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("usb host common core driver for omap EHCI and OHCI");

static int __init omap_usbhs_drvinit(void)
{
	return platform_driver_probe(&usbhs_omap_driver, usbhs_omap_probe);
=======
static const struct of_device_id usbhs_omap_dt_ids[] = {
	{ .compatible = "ti,usbhs-host" },
	{ }
};

MODULE_DEVICE_TABLE(of, usbhs_omap_dt_ids);


static struct platform_driver usbhs_omap_driver = {
	.driver = {
		.name		= usbhs_driver_name,
		.pm		= &usbhsomap_dev_pm_ops,
		.of_match_table = usbhs_omap_dt_ids,
	},
	.probe		= usbhs_omap_probe,
	.remove_new	= usbhs_omap_remove,
};

MODULE_AUTHOR("Keshava Munegowda <keshava_mgowda@ti.com>");
MODULE_AUTHOR("Roger Quadros <rogerq@ti.com>");
MODULE_ALIAS("platform:" USBHS_DRIVER_NAME);
MODULE_DESCRIPTION("usb host common core driver for omap EHCI and OHCI");

static int omap_usbhs_drvinit(void)
{
	return platform_driver_register(&usbhs_omap_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * init before ehci and ohci drivers;
 * The usbhs core driver should be initialized much before
 * the omap ehci and ohci probe functions are called.
<<<<<<< HEAD
 */
fs_initcall(omap_usbhs_drvinit);

static void __exit omap_usbhs_drvexit(void)
=======
 * This usbhs core driver should be initialized after
 * usb tll driver
 */
fs_initcall_sync(omap_usbhs_drvinit);

static void omap_usbhs_drvexit(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	platform_driver_unregister(&usbhs_omap_driver);
}
module_exit(omap_usbhs_drvexit);
