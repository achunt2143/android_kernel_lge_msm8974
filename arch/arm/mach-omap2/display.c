<<<<<<< HEAD
/*
 * OMAP2plus display device setup / initialization.
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *	Senthilvadivu Guruswamy
 *	Sumit Semwal
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * OMAP2plus display device setup / initialization.
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - https://www.ti.com/
 *	Senthilvadivu Guruswamy
 *	Sumit Semwal
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/delay.h>
<<<<<<< HEAD

#include <video/omapdss.h>
#include <plat/omap_hwmod.h>
#include <plat/omap_device.h>
#include <plat/omap-pm.h>
#include "common.h"

#include "iomap.h"
#include "mux.h"
#include "control.h"
#include "display.h"

#define DISPC_CONTROL		0x0040
#define DISPC_CONTROL2		0x0238
#define DISPC_IRQSTATUS		0x0018

#define DSS_SYSCONFIG		0x10
#define DSS_SYSSTATUS		0x14
=======
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/slab.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>

#include <linux/platform_data/omapdss.h>
#include "omap_hwmod.h"
#include "omap_device.h"
#include "common.h"

#include "soc.h"
#include "iomap.h"
#include "control.h"
#include "display.h"
#include "prm.h"

#define DISPC_CONTROL		0x0040
#define DISPC_CONTROL2		0x0238
#define DISPC_CONTROL3		0x0848
#define DISPC_IRQSTATUS		0x0018

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DSS_CONTROL		0x40
#define DSS_SDI_CONTROL		0x44
#define DSS_PLL_CONTROL		0x48

#define LCD_EN_MASK		(0x1 << 0)
#define DIGIT_EN_MASK		(0x1 << 1)

#define FRAMEDONE_IRQ_SHIFT	0
#define EVSYNC_EVEN_IRQ_SHIFT	2
#define EVSYNC_ODD_IRQ_SHIFT	3
#define FRAMEDONE2_IRQ_SHIFT	22
<<<<<<< HEAD
=======
#define FRAMEDONE3_IRQ_SHIFT	30
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define FRAMEDONETV_IRQ_SHIFT	24

/*
 * FRAMEDONE_IRQ_TIMEOUT: how long (in milliseconds) to wait during DISPC
 *     reset before deciding that something has gone wrong
 */
#define FRAMEDONE_IRQ_TIMEOUT		100

<<<<<<< HEAD
=======
#if defined(CONFIG_FB_OMAP2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct platform_device omap_display_device = {
	.name          = "omapdss",
	.id            = -1,
	.dev            = {
		.platform_data = NULL,
	},
};

<<<<<<< HEAD
struct omap_dss_hwmod_data {
	const char *oh_name;
	const char *dev_name;
	const int id;
};

static const struct omap_dss_hwmod_data omap2_dss_hwmod_data[] __initdata = {
	{ "dss_core", "omapdss_dss", -1 },
	{ "dss_dispc", "omapdss_dispc", -1 },
	{ "dss_rfbi", "omapdss_rfbi", -1 },
	{ "dss_venc", "omapdss_venc", -1 },
};

static const struct omap_dss_hwmod_data omap3_dss_hwmod_data[] __initdata = {
	{ "dss_core", "omapdss_dss", -1 },
	{ "dss_dispc", "omapdss_dispc", -1 },
	{ "dss_rfbi", "omapdss_rfbi", -1 },
	{ "dss_venc", "omapdss_venc", -1 },
	{ "dss_dsi1", "omapdss_dsi", 0 },
};

static const struct omap_dss_hwmod_data omap4_dss_hwmod_data[] __initdata = {
	{ "dss_core", "omapdss_dss", -1 },
	{ "dss_dispc", "omapdss_dispc", -1 },
	{ "dss_rfbi", "omapdss_rfbi", -1 },
	{ "dss_venc", "omapdss_venc", -1 },
	{ "dss_dsi1", "omapdss_dsi", 0 },
	{ "dss_dsi2", "omapdss_dsi", 1 },
	{ "dss_hdmi", "omapdss_hdmi", -1 },
};

static void __init omap4_hdmi_mux_pads(enum omap_hdmi_flags flags)
{
	u32 reg;
	u16 control_i2c_1;

	omap_mux_init_signal("hdmi_cec",
			OMAP_PIN_INPUT_PULLUP);
	omap_mux_init_signal("hdmi_ddc_scl",
			OMAP_PIN_INPUT_PULLUP);
	omap_mux_init_signal("hdmi_ddc_sda",
			OMAP_PIN_INPUT_PULLUP);

	/*
	 * CONTROL_I2C_1: HDMI_DDC_SDA_PULLUPRESX (bit 28) and
	 * HDMI_DDC_SCL_PULLUPRESX (bit 24) are set to disable
	 * internal pull up resistor.
	 */
	if (flags & OMAP_HDMI_SDA_SCL_EXTERNAL_PULLUP) {
		control_i2c_1 = OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_I2C_1;
		reg = omap4_ctrl_pad_readl(control_i2c_1);
		reg |= (OMAP4_HDMI_DDC_SDA_PULLUPRESX_MASK |
			OMAP4_HDMI_DDC_SCL_PULLUPRESX_MASK);
			omap4_ctrl_pad_writel(reg, control_i2c_1);
	}
}
=======
#define OMAP4_DSIPHY_SYSCON_OFFSET		0x78

static struct regmap *omap4_dsi_mux_syscon;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int omap4_dsi_mux_pads(int dsi_id, unsigned lanes)
{
	u32 enable_mask, enable_shift;
	u32 pipd_mask, pipd_shift;
	u32 reg;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (dsi_id == 0) {
		enable_mask = OMAP4_DSI1_LANEENABLE_MASK;
		enable_shift = OMAP4_DSI1_LANEENABLE_SHIFT;
		pipd_mask = OMAP4_DSI1_PIPD_MASK;
		pipd_shift = OMAP4_DSI1_PIPD_SHIFT;
	} else if (dsi_id == 1) {
		enable_mask = OMAP4_DSI2_LANEENABLE_MASK;
		enable_shift = OMAP4_DSI2_LANEENABLE_SHIFT;
		pipd_mask = OMAP4_DSI2_PIPD_MASK;
		pipd_shift = OMAP4_DSI2_PIPD_SHIFT;
	} else {
		return -ENODEV;
	}

<<<<<<< HEAD
	reg = omap4_ctrl_pad_readl(OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_DSIPHY);
=======
	ret = regmap_read(omap4_dsi_mux_syscon,
					  OMAP4_DSIPHY_SYSCON_OFFSET,
					  &reg);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	reg &= ~enable_mask;
	reg &= ~pipd_mask;

	reg |= (lanes << enable_shift) & enable_mask;
	reg |= (lanes << pipd_shift) & pipd_mask;

<<<<<<< HEAD
	omap4_ctrl_pad_writel(reg, OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_DSIPHY);

	return 0;
}

int __init omap_hdmi_init(enum omap_hdmi_flags flags)
{
	if (cpu_is_omap44xx())
		omap4_hdmi_mux_pads(flags);
=======
	regmap_write(omap4_dsi_mux_syscon, OMAP4_DSIPHY_SYSCON_OFFSET, reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int omap_dsi_enable_pads(int dsi_id, unsigned lane_mask)
{
	if (cpu_is_omap44xx())
		return omap4_dsi_mux_pads(dsi_id, lane_mask);

	return 0;
}

static void omap_dsi_disable_pads(int dsi_id, unsigned lane_mask)
{
	if (cpu_is_omap44xx())
		omap4_dsi_mux_pads(dsi_id, 0);
}

<<<<<<< HEAD
int __init omap_display_init(struct omap_dss_board_info *board_data)
{
	int r = 0;
	struct omap_hwmod *oh;
	struct platform_device *pdev;
	int i, oh_count;
	struct omap_display_platform_data pdata;
	const struct omap_dss_hwmod_data *curr_dss_hwmod;

	memset(&pdata, 0, sizeof(pdata));

	if (cpu_is_omap24xx()) {
		curr_dss_hwmod = omap2_dss_hwmod_data;
		oh_count = ARRAY_SIZE(omap2_dss_hwmod_data);
	} else if (cpu_is_omap34xx()) {
		curr_dss_hwmod = omap3_dss_hwmod_data;
		oh_count = ARRAY_SIZE(omap3_dss_hwmod_data);
	} else {
		curr_dss_hwmod = omap4_dss_hwmod_data;
		oh_count = ARRAY_SIZE(omap4_dss_hwmod_data);
	}

	if (board_data->dsi_enable_pads == NULL)
		board_data->dsi_enable_pads = omap_dsi_enable_pads;
	if (board_data->dsi_disable_pads == NULL)
		board_data->dsi_disable_pads = omap_dsi_disable_pads;

	pdata.board_data = board_data;
	pdata.board_data->get_context_loss_count =
		omap_pm_get_dev_context_loss_count;

	for (i = 0; i < oh_count; i++) {
		oh = omap_hwmod_lookup(curr_dss_hwmod[i].oh_name);
		if (!oh) {
			pr_err("Could not look up %s\n",
				curr_dss_hwmod[i].oh_name);
			return -ENODEV;
		}

		pdev = omap_device_build(curr_dss_hwmod[i].dev_name,
				curr_dss_hwmod[i].id, oh, &pdata,
				sizeof(struct omap_display_platform_data),
				NULL, 0, 0);

		if (WARN((IS_ERR(pdev)), "Could not build omap_device for %s\n",
				curr_dss_hwmod[i].oh_name))
			return -ENODEV;
	}
	omap_display_device.dev.platform_data = board_data;

	r = platform_device_register(&omap_display_device);
	if (r < 0)
		printk(KERN_ERR "Unable to register OMAP-Display device\n");

	return r;
}

static void dispc_disable_outputs(void)
{
	u32 v, irq_mask = 0;
	bool lcd_en, digit_en, lcd2_en = false;
=======
static enum omapdss_version __init omap_display_get_version(void)
{
	if (cpu_is_omap24xx())
		return OMAPDSS_VER_OMAP24xx;
	else if (cpu_is_omap3630())
		return OMAPDSS_VER_OMAP3630;
	else if (cpu_is_omap34xx()) {
		if (soc_is_am35xx()) {
			return OMAPDSS_VER_AM35xx;
		} else {
			if (omap_rev() < OMAP3430_REV_ES3_0)
				return OMAPDSS_VER_OMAP34xx_ES1;
			else
				return OMAPDSS_VER_OMAP34xx_ES3;
		}
	} else if (omap_rev() == OMAP4430_REV_ES1_0)
		return OMAPDSS_VER_OMAP4430_ES1;
	else if (omap_rev() == OMAP4430_REV_ES2_0 ||
			omap_rev() == OMAP4430_REV_ES2_1 ||
			omap_rev() == OMAP4430_REV_ES2_2)
		return OMAPDSS_VER_OMAP4430_ES2;
	else if (cpu_is_omap44xx())
		return OMAPDSS_VER_OMAP4;
	else if (soc_is_omap54xx())
		return OMAPDSS_VER_OMAP5;
	else if (soc_is_am43xx())
		return OMAPDSS_VER_AM43xx;
	else if (soc_is_dra7xx())
		return OMAPDSS_VER_DRA7xx;
	else
		return OMAPDSS_VER_UNKNOWN;
}

static int __init omapdss_init_fbdev(void)
{
	static struct omap_dss_board_info board_data = {
		.dsi_enable_pads = omap_dsi_enable_pads,
		.dsi_disable_pads = omap_dsi_disable_pads,
	};
	struct device_node *node;
	int r;

	board_data.version = omap_display_get_version();
	if (board_data.version == OMAPDSS_VER_UNKNOWN) {
		pr_err("DSS not supported on this SoC\n");
		return -ENODEV;
	}

	omap_display_device.dev.platform_data = &board_data;

	r = platform_device_register(&omap_display_device);
	if (r < 0) {
		pr_err("Unable to register omapdss device\n");
		return r;
	}

	/* create vrfb device */
	r = omap_init_vrfb();
	if (r < 0) {
		pr_err("Unable to register omapvrfb device\n");
		return r;
	}

	/* create FB device */
	r = omap_init_fb();
	if (r < 0) {
		pr_err("Unable to register omapfb device\n");
		return r;
	}

	/* create V4L2 display device */
	r = omap_init_vout();
	if (r < 0) {
		pr_err("Unable to register omap_vout device\n");
		return r;
	}

	/* add DSI info for omap4 */
	node = of_find_node_by_name(NULL, "omap4_padconf_global");
	if (node)
		omap4_dsi_mux_syscon = syscon_node_to_regmap(node);
	of_node_put(node);

	return 0;
}

static const char * const omapdss_compat_names[] __initconst = {
	"ti,omap2-dss",
	"ti,omap3-dss",
	"ti,omap4-dss",
	"ti,omap5-dss",
	"ti,dra7-dss",
};

static struct device_node * __init omapdss_find_dss_of_node(void)
{
	struct device_node *node;
	int i;

	for (i = 0; i < ARRAY_SIZE(omapdss_compat_names); ++i) {
		node = of_find_compatible_node(NULL, NULL,
			omapdss_compat_names[i]);
		if (node)
			return node;
	}

	return NULL;
}

static int __init omapdss_init_of(void)
{
	int r;
	struct device_node *node;
	struct platform_device *pdev;

	/* only create dss helper devices if dss is enabled in the .dts */

	node = omapdss_find_dss_of_node();
	if (!node)
		return 0;

	if (!of_device_is_available(node)) {
		of_node_put(node);
		return 0;
	}

	pdev = of_find_device_by_node(node);

	if (!pdev) {
		pr_err("Unable to find DSS platform device\n");
		of_node_put(node);
		return -ENODEV;
	}

	r = of_platform_populate(node, NULL, NULL, &pdev->dev);
	put_device(&pdev->dev);
	of_node_put(node);
	if (r) {
		pr_err("Unable to populate DSS submodule devices\n");
		return r;
	}

	return omapdss_init_fbdev();
}
omap_device_initcall(omapdss_init_of);
#endif /* CONFIG_FB_OMAP2 */

static void dispc_disable_outputs(void)
{
	u32 v, irq_mask = 0;
	bool lcd_en, digit_en, lcd2_en = false, lcd3_en = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;
	struct omap_dss_dispc_dev_attr *da;
	struct omap_hwmod *oh;

	oh = omap_hwmod_lookup("dss_dispc");
	if (!oh) {
		WARN(1, "display: could not disable outputs during reset - could not find dss_dispc hwmod\n");
		return;
	}

	if (!oh->dev_attr) {
		pr_err("display: could not disable outputs during reset due to missing dev_attr\n");
		return;
	}

	da = (struct omap_dss_dispc_dev_attr *)oh->dev_attr;

	/* store value of LCDENABLE and DIGITENABLE bits */
	v = omap_hwmod_read(oh, DISPC_CONTROL);
	lcd_en = v & LCD_EN_MASK;
	digit_en = v & DIGIT_EN_MASK;

	/* store value of LCDENABLE for LCD2 */
	if (da->manager_count > 2) {
		v = omap_hwmod_read(oh, DISPC_CONTROL2);
		lcd2_en = v & LCD_EN_MASK;
	}

<<<<<<< HEAD
	if (!(lcd_en | digit_en | lcd2_en))
=======
	/* store value of LCDENABLE for LCD3 */
	if (da->manager_count > 3) {
		v = omap_hwmod_read(oh, DISPC_CONTROL3);
		lcd3_en = v & LCD_EN_MASK;
	}

	if (!(lcd_en | digit_en | lcd2_en | lcd3_en))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return; /* no managers currently enabled */

	/*
	 * If any manager was enabled, we need to disable it before
	 * DSS clocks are disabled or DISPC module is reset
	 */
	if (lcd_en)
		irq_mask |= 1 << FRAMEDONE_IRQ_SHIFT;

	if (digit_en) {
		if (da->has_framedonetv_irq) {
			irq_mask |= 1 << FRAMEDONETV_IRQ_SHIFT;
		} else {
			irq_mask |= 1 << EVSYNC_EVEN_IRQ_SHIFT |
				1 << EVSYNC_ODD_IRQ_SHIFT;
		}
	}

	if (lcd2_en)
		irq_mask |= 1 << FRAMEDONE2_IRQ_SHIFT;
<<<<<<< HEAD

	/*
	 * clear any previous FRAMEDONE, FRAMEDONETV,
	 * EVSYNC_EVEN/ODD or FRAMEDONE2 interrupts
=======
	if (lcd3_en)
		irq_mask |= 1 << FRAMEDONE3_IRQ_SHIFT;

	/*
	 * clear any previous FRAMEDONE, FRAMEDONETV,
	 * EVSYNC_EVEN/ODD, FRAMEDONE2 or FRAMEDONE3 interrupts
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	omap_hwmod_write(irq_mask, oh, DISPC_IRQSTATUS);

	/* disable LCD and TV managers */
	v = omap_hwmod_read(oh, DISPC_CONTROL);
	v &= ~(LCD_EN_MASK | DIGIT_EN_MASK);
	omap_hwmod_write(v, oh, DISPC_CONTROL);

	/* disable LCD2 manager */
	if (da->manager_count > 2) {
		v = omap_hwmod_read(oh, DISPC_CONTROL2);
		v &= ~LCD_EN_MASK;
		omap_hwmod_write(v, oh, DISPC_CONTROL2);
	}

<<<<<<< HEAD
=======
	/* disable LCD3 manager */
	if (da->manager_count > 3) {
		v = omap_hwmod_read(oh, DISPC_CONTROL3);
		v &= ~LCD_EN_MASK;
		omap_hwmod_write(v, oh, DISPC_CONTROL3);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	i = 0;
	while ((omap_hwmod_read(oh, DISPC_IRQSTATUS) & irq_mask) !=
	       irq_mask) {
		i++;
		if (i > FRAMEDONE_IRQ_TIMEOUT) {
<<<<<<< HEAD
			pr_err("didn't get FRAMEDONE1/2 or TV interrupt\n");
=======
			pr_err("didn't get FRAMEDONE1/2/3 or TV interrupt\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		mdelay(1);
	}
}

<<<<<<< HEAD
#define MAX_MODULE_SOFTRESET_WAIT	10000
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int omap_dss_reset(struct omap_hwmod *oh)
{
	struct omap_hwmod_opt_clk *oc;
	int c = 0;
	int i, r;

	if (!(oh->class->sysc->sysc_flags & SYSS_HAS_RESET_STATUS)) {
		pr_err("dss_core: hwmod data doesn't contain reset data\n");
		return -EINVAL;
	}

	for (i = oh->opt_clks_cnt, oc = oh->opt_clks; i > 0; i--, oc++)
<<<<<<< HEAD
		if (oc->_clk)
			clk_enable(oc->_clk);
=======
		clk_prepare_enable(oc->_clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dispc_disable_outputs();

	/* clear SDI registers */
	if (cpu_is_omap3430()) {
		omap_hwmod_write(0x0, oh, DSS_SDI_CONTROL);
		omap_hwmod_write(0x0, oh, DSS_PLL_CONTROL);
	}

	/*
	 * clear DSS_CONTROL register to switch DSS clock sources to
	 * PRCM clock, if any
	 */
	omap_hwmod_write(0x0, oh, DSS_CONTROL);

	omap_test_timeout((omap_hwmod_read(oh, oh->class->sysc->syss_offs)
				& SYSS_RESETDONE_MASK),
			MAX_MODULE_SOFTRESET_WAIT, c);

	if (c == MAX_MODULE_SOFTRESET_WAIT)
<<<<<<< HEAD
		pr_warning("dss_core: waiting for reset to finish failed\n");
=======
		pr_warn("dss_core: waiting for reset to finish failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		pr_debug("dss_core: softreset done\n");

	for (i = oh->opt_clks_cnt, oc = oh->opt_clks; i > 0; i--, oc++)
<<<<<<< HEAD
		if (oc->_clk)
			clk_disable(oc->_clk);
=======
		clk_disable_unprepare(oc->_clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	r = (c == MAX_MODULE_SOFTRESET_WAIT) ? -ETIMEDOUT : 0;

	return r;
}
