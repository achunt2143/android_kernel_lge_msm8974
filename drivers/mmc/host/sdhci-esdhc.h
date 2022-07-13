<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Freescale eSDHC controller driver generics for OF and pltfm.
 *
 * Copyright (c) 2007 Freescale Semiconductor, Inc.
 * Copyright (c) 2009 MontaVista Software, Inc.
 * Copyright (c) 2010 Pengutronix e.K.
<<<<<<< HEAD
 *   Author: Wolfram Sang <w.sang@pengutronix.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
=======
 * Copyright 2020 NXP
 *   Author: Wolfram Sang <kernel@pengutronix.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _DRIVERS_MMC_SDHCI_ESDHC_H
#define _DRIVERS_MMC_SDHCI_ESDHC_H

/*
 * Ops and quirks for the Freescale eSDHC controller.
 */

#define ESDHC_DEFAULT_QUIRKS	(SDHCI_QUIRK_FORCE_BLK_SZ_2048 | \
<<<<<<< HEAD
				SDHCI_QUIRK_NO_BUSY_IRQ | \
				SDHCI_QUIRK_NONSTANDARD_CLOCK | \
				SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK | \
				SDHCI_QUIRK_PIO_NEEDS_DELAY | \
				SDHCI_QUIRK_RESTORE_IRQS_AFTER_RESET)

#define ESDHC_SYSTEM_CONTROL	0x2c
#define ESDHC_CLOCK_MASK	0x0000fff0
#define ESDHC_PREDIV_SHIFT	8
#define ESDHC_DIVIDER_SHIFT	4
#define ESDHC_CLOCK_PEREN	0x00000004
#define ESDHC_CLOCK_HCKEN	0x00000002
#define ESDHC_CLOCK_IPGEN	0x00000001
=======
				SDHCI_QUIRK_32BIT_DMA_ADDR | \
				SDHCI_QUIRK_NO_BUSY_IRQ | \
				SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK | \
				SDHCI_QUIRK_PIO_NEEDS_DELAY | \
				SDHCI_QUIRK_NO_HISPD_BIT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* pltfm-specific */
#define ESDHC_HOST_CONTROL_LE	0x20

<<<<<<< HEAD
/* OF-specific */
#define ESDHC_DMA_SYSCTL	0x40c
#define ESDHC_DMA_SNOOP		0x00000040

#define ESDHC_HOST_CONTROL_RES	0x05

static inline void esdhc_set_clock(struct sdhci_host *host, unsigned int clock)
{
	int pre_div = 2;
	int div = 1;
	u32 temp;

	if (clock == 0)
		goto out;

	temp = sdhci_readl(host, ESDHC_SYSTEM_CONTROL);
	temp &= ~(ESDHC_CLOCK_IPGEN | ESDHC_CLOCK_HCKEN | ESDHC_CLOCK_PEREN
		| ESDHC_CLOCK_MASK);
	sdhci_writel(host, temp, ESDHC_SYSTEM_CONTROL);

	while (host->max_clk / pre_div / 16 > clock && pre_div < 256)
		pre_div *= 2;

	while (host->max_clk / pre_div / div > clock && div < 16)
		div++;

	dev_dbg(mmc_dev(host->mmc), "desired SD clock: %d, actual: %d\n",
		clock, host->max_clk / pre_div / div);

	pre_div >>= 1;
	div--;

	temp = sdhci_readl(host, ESDHC_SYSTEM_CONTROL);
	temp |= (ESDHC_CLOCK_IPGEN | ESDHC_CLOCK_HCKEN | ESDHC_CLOCK_PEREN
		| (div << ESDHC_DIVIDER_SHIFT)
		| (pre_div << ESDHC_PREDIV_SHIFT));
	sdhci_writel(host, temp, ESDHC_SYSTEM_CONTROL);
	mdelay(1);
out:
	host->clock = clock;
}
=======
/*
 * eSDHC register definition
 */

/* Present State Register */
#define ESDHC_PRSSTAT			0x24
#define ESDHC_CLOCK_GATE_OFF		0x00000080
#define ESDHC_CLOCK_STABLE		0x00000008

/* Protocol Control Register */
#define ESDHC_PROCTL			0x28
#define ESDHC_VOLT_SEL			0x00000400
#define ESDHC_CTRL_4BITBUS		(0x1 << 1)
#define ESDHC_CTRL_8BITBUS		(0x2 << 1)
#define ESDHC_CTRL_BUSWIDTH_MASK	(0x3 << 1)
#define ESDHC_HOST_CONTROL_RES		0x01

/* System Control Register */
#define ESDHC_SYSTEM_CONTROL		0x2c
#define ESDHC_CLOCK_MASK		0x0000fff0
#define ESDHC_PREDIV_SHIFT		8
#define ESDHC_DIVIDER_SHIFT		4
#define ESDHC_CLOCK_SDCLKEN		0x00000008
#define ESDHC_CLOCK_PEREN		0x00000004
#define ESDHC_CLOCK_HCKEN		0x00000002
#define ESDHC_CLOCK_IPGEN		0x00000001

/* System Control 2 Register */
#define ESDHC_SYSTEM_CONTROL_2		0x3c
#define ESDHC_SMPCLKSEL			0x00800000
#define ESDHC_EXTN			0x00400000

/* Host Controller Capabilities Register 2 */
#define ESDHC_CAPABILITIES_1		0x114

/* Tuning Block Control Register */
#define ESDHC_TBCTL			0x120
#define ESDHC_HS400_WNDW_ADJUST		0x00000040
#define ESDHC_HS400_MODE		0x00000010
#define ESDHC_TB_EN			0x00000004
#define ESDHC_TB_MODE_MASK		0x00000003
#define ESDHC_TB_MODE_SW		0x00000003
#define ESDHC_TB_MODE_3			0x00000002

#define ESDHC_TBSTAT			0x124

#define ESDHC_TBPTR			0x128
#define ESDHC_WNDW_STRT_PTR_SHIFT	8
#define ESDHC_WNDW_STRT_PTR_MASK	(0x7f << 8)
#define ESDHC_WNDW_END_PTR_MASK		0x7f

/* SD Clock Control Register */
#define ESDHC_SDCLKCTL			0x144
#define ESDHC_LPBK_CLK_SEL		0x80000000
#define ESDHC_CMD_CLK_CTL		0x00008000

/* SD Timing Control Register */
#define ESDHC_SDTIMNGCTL		0x148
#define ESDHC_FLW_CTL_BG		0x00008000

/* DLL Config 0 Register */
#define ESDHC_DLLCFG0			0x160
#define ESDHC_DLL_ENABLE		0x80000000
#define ESDHC_DLL_RESET			0x40000000
#define ESDHC_DLL_FREQ_SEL		0x08000000

/* DLL Config 1 Register */
#define ESDHC_DLLCFG1			0x164
#define ESDHC_DLL_PD_PULSE_STRETCH_SEL	0x80000000

/* DLL Status 0 Register */
#define ESDHC_DLLSTAT0			0x170
#define ESDHC_DLL_STS_SLV_LOCK		0x08000000

/* Control Register for DMA transfer */
#define ESDHC_DMA_SYSCTL		0x40c
#define ESDHC_PERIPHERAL_CLK_SEL	0x00080000
#define ESDHC_FLUSH_ASYNC_FIFO		0x00040000
#define ESDHC_DMA_SNOOP			0x00000040
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _DRIVERS_MMC_SDHCI_ESDHC_H */
