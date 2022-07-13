<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/mach-pxa/pxa25x.c
 *
 *  Author:	Nicolas Pitre
 *  Created:	Jun 15, 2001
 *  Copyright:	MontaVista Software Inc.
 *
 * Code specific to PXA21x/25x/26x variants.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Since this file should be linked before any other machine specific file,
 * the __initcall() here will be executed first.  This serves as default
 * initialization stuff for PXA machines which can be overridden later if
 * need be.
 */
<<<<<<< HEAD
=======
#include <linux/dmaengine.h>
#include <linux/dma/pxa-dma.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/gpio.h>
#include <linux/gpio-pxa.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/suspend.h>
#include <linux/syscore_ops.h>
#include <linux/irq.h>
<<<<<<< HEAD

#include <asm/mach/map.h>
#include <asm/suspend.h>
#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/pxa25x.h>
#include <mach/reset.h>
#include <mach/pm.h>
#include <mach/dma.h>
#include <mach/smemc.h>

#include "generic.h"
#include "devices.h"
#include "clock.h"
=======
#include <linux/irqchip.h>
#include <linux/platform_data/mmp_dma.h>
#include <linux/soc/pxa/cpu.h>
#include <linux/soc/pxa/smemc.h>

#include <asm/mach/map.h>
#include <asm/suspend.h>
#include "irqs.h"
#include "pxa25x.h"
#include "reset.h"
#include "pm.h"
#include "addr-map.h"
#include "smemc.h"

#include "generic.h"
#include "devices.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Various clock factors driven by the CCCR register.
 */

<<<<<<< HEAD
/* Crystal Frequency to Memory Frequency Multiplier (L) */
static unsigned char L_clk_mult[32] = { 0, 27, 32, 36, 40, 45, 0, };

/* Memory Frequency to Run Mode Frequency Multiplier (M) */
static unsigned char M_clk_mult[4] = { 0, 1, 2, 4 };

/* Run Mode Frequency to Turbo Mode Frequency Multiplier (N) */
/* Note: we store the value N * 2 here. */
static unsigned char N2_clk_mult[8] = { 0, 0, 2, 3, 4, 0, 6, 0 };

/* Crystal clock */
#define BASE_CLK	3686400

/*
 * Get the clock frequency as reflected by CCCR and the turbo flag.
 * We assume these values have been applied via a fcs.
 * If info is not 0 we also display the current settings.
 */
unsigned int pxa25x_get_clk_frequency_khz(int info)
{
	unsigned long cccr, turbo;
	unsigned int l, L, m, M, n2, N;

	cccr = CCCR;
	asm( "mrc\tp14, 0, %0, c6, c0, 0" : "=r" (turbo) );

	l  =  L_clk_mult[(cccr >> 0) & 0x1f];
	m  =  M_clk_mult[(cccr >> 5) & 0x03];
	n2 = N2_clk_mult[(cccr >> 7) & 0x07];

	L = l * BASE_CLK;
	M = m * L;
	N = n2 * M / 2;

	if(info)
	{
		L += 5000;
		printk( KERN_INFO "Memory clock: %d.%02dMHz (*%d)\n",
			L / 1000000, (L % 1000000) / 10000, l );
		M += 5000;
		printk( KERN_INFO "Run Mode clock: %d.%02dMHz (*%d)\n",
			M / 1000000, (M % 1000000) / 10000, m );
		N += 5000;
		printk( KERN_INFO "Turbo Mode clock: %d.%02dMHz (*%d.%d, %sactive)\n",
			N / 1000000, (N % 1000000) / 10000, n2 / 2, (n2 % 2) * 5,
			(turbo & 1) ? "" : "in" );
	}

	return (turbo & 1) ? (N/1000) : (M/1000);
}

static unsigned long clk_pxa25x_mem_getrate(struct clk *clk)
{
	return L_clk_mult[(CCCR >> 0) & 0x1f] * BASE_CLK;
}

static const struct clkops clk_pxa25x_mem_ops = {
	.enable		= clk_dummy_enable,
	.disable	= clk_dummy_disable,
	.getrate	= clk_pxa25x_mem_getrate,
};

static const struct clkops clk_pxa25x_lcd_ops = {
	.enable		= clk_pxa2xx_cken_enable,
	.disable	= clk_pxa2xx_cken_disable,
	.getrate	= clk_pxa25x_mem_getrate,
};

static unsigned long gpio12_config_32k[] = {
	GPIO12_32KHz,
};

static unsigned long gpio12_config_gpio[] = {
	GPIO12_GPIO,
};

static void clk_gpio12_enable(struct clk *clk)
{
	pxa2xx_mfp_config(gpio12_config_32k, 1);
}

static void clk_gpio12_disable(struct clk *clk)
{
	pxa2xx_mfp_config(gpio12_config_gpio, 1);
}

static const struct clkops clk_pxa25x_gpio12_ops = {
	.enable         = clk_gpio12_enable,
	.disable        = clk_gpio12_disable,
};

static unsigned long gpio11_config_3m6[] = {
	GPIO11_3_6MHz,
};

static unsigned long gpio11_config_gpio[] = {
	GPIO11_GPIO,
};

static void clk_gpio11_enable(struct clk *clk)
{
	pxa2xx_mfp_config(gpio11_config_3m6, 1);
}

static void clk_gpio11_disable(struct clk *clk)
{
	pxa2xx_mfp_config(gpio11_config_gpio, 1);
}

static const struct clkops clk_pxa25x_gpio11_ops = {
	.enable         = clk_gpio11_enable,
	.disable        = clk_gpio11_disable,
};

/*
 * 3.6864MHz -> OST, GPIO, SSP, PWM, PLLs (95.842MHz, 147.456MHz)
 * 95.842MHz -> MMC 19.169MHz, I2C 31.949MHz, FICP 47.923MHz, USB 47.923MHz
 * 147.456MHz -> UART 14.7456MHz, AC97 12.288MHz, I2S 5.672MHz (allegedly)
 */

/*
 * PXA 2xx clock declarations.
 */
static DEFINE_PXA2_CKEN(pxa25x_hwuart, HWUART, 14745600, 1);
static DEFINE_PXA2_CKEN(pxa25x_ffuart, FFUART, 14745600, 1);
static DEFINE_PXA2_CKEN(pxa25x_btuart, BTUART, 14745600, 1);
static DEFINE_PXA2_CKEN(pxa25x_stuart, STUART, 14745600, 1);
static DEFINE_PXA2_CKEN(pxa25x_usb, USB, 47923000, 5);
static DEFINE_PXA2_CKEN(pxa25x_mmc, MMC, 19169000, 0);
static DEFINE_PXA2_CKEN(pxa25x_i2c, I2C, 31949000, 0);
static DEFINE_PXA2_CKEN(pxa25x_ssp, SSP, 3686400, 0);
static DEFINE_PXA2_CKEN(pxa25x_nssp, NSSP, 3686400, 0);
static DEFINE_PXA2_CKEN(pxa25x_assp, ASSP, 3686400, 0);
static DEFINE_PXA2_CKEN(pxa25x_pwm0, PWM0, 3686400, 0);
static DEFINE_PXA2_CKEN(pxa25x_pwm1, PWM1, 3686400, 0);
static DEFINE_PXA2_CKEN(pxa25x_ac97, AC97, 24576000, 0);
static DEFINE_PXA2_CKEN(pxa25x_i2s, I2S, 14745600, 0);
static DEFINE_PXA2_CKEN(pxa25x_ficp, FICP, 47923000, 0);

static DEFINE_CK(pxa25x_lcd, LCD, &clk_pxa25x_lcd_ops);
static DEFINE_CLK(pxa25x_gpio11, &clk_pxa25x_gpio11_ops, 3686400, 0);
static DEFINE_CLK(pxa25x_gpio12, &clk_pxa25x_gpio12_ops, 32768, 0);
static DEFINE_CLK(pxa25x_mem, &clk_pxa25x_mem_ops, 0, 0);

static struct clk_lookup pxa25x_clkregs[] = {
	INIT_CLKREG(&clk_pxa25x_lcd, "pxa2xx-fb", NULL),
	INIT_CLKREG(&clk_pxa25x_ffuart, "pxa2xx-uart.0", NULL),
	INIT_CLKREG(&clk_pxa25x_btuart, "pxa2xx-uart.1", NULL),
	INIT_CLKREG(&clk_pxa25x_stuart, "pxa2xx-uart.2", NULL),
	INIT_CLKREG(&clk_pxa25x_usb, "pxa25x-udc", NULL),
	INIT_CLKREG(&clk_pxa25x_mmc, "pxa2xx-mci.0", NULL),
	INIT_CLKREG(&clk_pxa25x_i2c, "pxa2xx-i2c.0", NULL),
	INIT_CLKREG(&clk_pxa25x_ssp, "pxa25x-ssp.0", NULL),
	INIT_CLKREG(&clk_pxa25x_nssp, "pxa25x-nssp.1", NULL),
	INIT_CLKREG(&clk_pxa25x_assp, "pxa25x-nssp.2", NULL),
	INIT_CLKREG(&clk_pxa25x_pwm0, "pxa25x-pwm.0", NULL),
	INIT_CLKREG(&clk_pxa25x_pwm1, "pxa25x-pwm.1", NULL),
	INIT_CLKREG(&clk_pxa25x_i2s, "pxa2xx-i2s", NULL),
	INIT_CLKREG(&clk_pxa25x_stuart, "pxa2xx-ir", "UARTCLK"),
	INIT_CLKREG(&clk_pxa25x_ficp, "pxa2xx-ir", "FICPCLK"),
	INIT_CLKREG(&clk_pxa25x_ac97, NULL, "AC97CLK"),
	INIT_CLKREG(&clk_pxa25x_gpio11, NULL, "GPIO11_CLK"),
	INIT_CLKREG(&clk_pxa25x_gpio12, NULL, "GPIO12_CLK"),
	INIT_CLKREG(&clk_pxa25x_mem, "pxa2xx-pcmcia", NULL),
	INIT_CLKREG(&clk_dummy, "pxa-gpio", NULL),
};

static struct clk_lookup pxa25x_hwuart_clkreg =
	INIT_CLKREG(&clk_pxa25x_hwuart, "pxa2xx-uart.3", NULL);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PM

#define SAVE(x)		sleep_save[SLEEP_SAVE_##x] = x
#define RESTORE(x)	x = sleep_save[SLEEP_SAVE_##x]

/*
 * List of global PXA peripheral registers to preserve.
 * More ones like CP and general purpose register values are preserved
 * with the stack pointer in sleep.S.
 */
enum {
	SLEEP_SAVE_PSTR,
	SLEEP_SAVE_COUNT
};


static void pxa25x_cpu_pm_save(unsigned long *sleep_save)
{
	SAVE(PSTR);
}

static void pxa25x_cpu_pm_restore(unsigned long *sleep_save)
{
	RESTORE(PSTR);
}

static void pxa25x_cpu_pm_enter(suspend_state_t state)
{
	/* Clear reset status */
	RCSR = RCSR_HWR | RCSR_WDR | RCSR_SMR | RCSR_GPR;

	switch (state) {
	case PM_SUSPEND_MEM:
		cpu_suspend(PWRMODE_SLEEP, pxa25x_finish_suspend);
		break;
	}
}

static int pxa25x_cpu_pm_prepare(void)
{
	/* set resume return address */
<<<<<<< HEAD
	PSPR = virt_to_phys(cpu_resume);
=======
	PSPR = __pa_symbol(cpu_resume);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void pxa25x_cpu_pm_finish(void)
{
	/* ensure not to come back here if it wasn't intended */
	PSPR = 0;
}

static struct pxa_cpu_pm_fns pxa25x_cpu_pm_fns = {
	.save_count	= SLEEP_SAVE_COUNT,
	.valid		= suspend_valid_only_mem,
	.save		= pxa25x_cpu_pm_save,
	.restore	= pxa25x_cpu_pm_restore,
	.enter		= pxa25x_cpu_pm_enter,
	.prepare	= pxa25x_cpu_pm_prepare,
	.finish		= pxa25x_cpu_pm_finish,
};

static void __init pxa25x_init_pm(void)
{
	pxa_cpu_pm_fns = &pxa25x_cpu_pm_fns;
}
#else
static inline void pxa25x_init_pm(void) {}
#endif

/* PXA25x: supports wakeup from GPIO0..GPIO15 and RTC alarm
 */

static int pxa25x_set_wake(struct irq_data *d, unsigned int on)
{
	int gpio = pxa_irq_to_gpio(d->irq);
	uint32_t mask = 0;

	if (gpio >= 0 && gpio < 85)
		return gpio_set_wake(gpio, on);

	if (d->irq == IRQ_RTCAlrm) {
		mask = PWER_RTC;
		goto set_pwer;
	}

	return -EINVAL;

set_pwer:
	if (on)
		PWER |= mask;
	else
		PWER &=~mask;

	return 0;
}

void __init pxa25x_init_irq(void)
{
	pxa_init_irq(32, pxa25x_set_wake);
<<<<<<< HEAD
}

#ifdef CONFIG_CPU_PXA26x
void __init pxa26x_init_irq(void)
{
	pxa_init_irq(32, pxa25x_set_wake);
}
#endif
=======
	set_handle_irq(pxa25x_handle_irq);
}

static int __init __init
pxa25x_dt_init_irq(struct device_node *node, struct device_node *parent)
{
	pxa_dt_irq_init(pxa25x_set_wake);
	set_handle_irq(icip_handle_irq);

	return 0;
}
IRQCHIP_DECLARE(pxa25x_intc, "marvell,pxa-intc", pxa25x_dt_init_irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct map_desc pxa25x_io_desc[] __initdata = {
	{	/* Mem Ctl */
		.virtual	= (unsigned long)SMEMC_VIRT,
		.pfn		= __phys_to_pfn(PXA2XX_SMEMC_BASE),
<<<<<<< HEAD
		.length		= 0x00200000,
=======
		.length		= SMEMC_SIZE,
		.type		= MT_DEVICE
	}, {	/* UNCACHED_PHYS_0 */
		.virtual	= UNCACHED_PHYS_0,
		.pfn		= __phys_to_pfn(0x00000000),
		.length		= UNCACHED_PHYS_0_SIZE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.type		= MT_DEVICE
	},
};

void __init pxa25x_map_io(void)
{
	pxa_map_io();
	iotable_init(ARRAY_AND_SIZE(pxa25x_io_desc));
	pxa25x_get_clk_frequency_khz(1);
}

<<<<<<< HEAD
=======
static struct pxa_gpio_platform_data pxa25x_gpio_info __initdata = {
	.irq_base	= PXA_GPIO_TO_IRQ(0),
	.gpio_set_wake	= gpio_set_wake,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct platform_device *pxa25x_devices[] __initdata = {
	&pxa25x_device_udc,
	&pxa_device_pmu,
	&pxa_device_i2s,
	&sa1100_device_rtc,
	&pxa25x_device_ssp,
	&pxa25x_device_nssp,
	&pxa25x_device_assp,
	&pxa25x_device_pwm0,
	&pxa25x_device_pwm1,
	&pxa_device_asoc_platform,
};

<<<<<<< HEAD
=======
static const struct dma_slave_map pxa25x_slave_map[] = {
	/* PXA25x, PXA27x and PXA3xx common entries */
	{ "pxa2xx-ac97", "pcm_pcm_mic_mono", PDMA_FILTER_PARAM(LOWEST, 8) },
	{ "pxa2xx-ac97", "pcm_pcm_aux_mono_in", PDMA_FILTER_PARAM(LOWEST, 9) },
	{ "pxa2xx-ac97", "pcm_pcm_aux_mono_out",
	  PDMA_FILTER_PARAM(LOWEST, 10) },
	{ "pxa2xx-ac97", "pcm_pcm_stereo_in", PDMA_FILTER_PARAM(LOWEST, 11) },
	{ "pxa2xx-ac97", "pcm_pcm_stereo_out", PDMA_FILTER_PARAM(LOWEST, 12) },
	{ "pxa-ssp-dai.1", "rx", PDMA_FILTER_PARAM(LOWEST, 13) },
	{ "pxa-ssp-dai.1", "tx", PDMA_FILTER_PARAM(LOWEST, 14) },
	{ "pxa-ssp-dai.2", "rx", PDMA_FILTER_PARAM(LOWEST, 15) },
	{ "pxa-ssp-dai.2", "tx", PDMA_FILTER_PARAM(LOWEST, 16) },
	{ "pxa2xx-ir", "rx", PDMA_FILTER_PARAM(LOWEST, 17) },
	{ "pxa2xx-ir", "tx", PDMA_FILTER_PARAM(LOWEST, 18) },
	{ "pxa2xx-mci.0", "rx", PDMA_FILTER_PARAM(LOWEST, 21) },
	{ "pxa2xx-mci.0", "tx", PDMA_FILTER_PARAM(LOWEST, 22) },

	/* PXA25x specific map */
	{ "pxa25x-ssp.0", "rx", PDMA_FILTER_PARAM(LOWEST, 13) },
	{ "pxa25x-ssp.0", "tx", PDMA_FILTER_PARAM(LOWEST, 14) },
	{ "pxa25x-nssp.1", "rx", PDMA_FILTER_PARAM(LOWEST, 15) },
	{ "pxa25x-nssp.1", "tx", PDMA_FILTER_PARAM(LOWEST, 16) },
	{ "pxa25x-nssp.2", "rx", PDMA_FILTER_PARAM(LOWEST, 23) },
	{ "pxa25x-nssp.2", "tx", PDMA_FILTER_PARAM(LOWEST, 24) },
};

static struct mmp_dma_platdata pxa25x_dma_pdata = {
	.dma_channels	= 16,
	.nb_requestors	= 40,
	.slave_map	= pxa25x_slave_map,
	.slave_map_cnt	= ARRAY_SIZE(pxa25x_slave_map),
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init pxa25x_init(void)
{
	int ret = 0;

	if (cpu_is_pxa25x()) {

<<<<<<< HEAD
		reset_status = RCSR;

		clkdev_add_table(pxa25x_clkregs, ARRAY_SIZE(pxa25x_clkregs));

		if ((ret = pxa_init_dma(IRQ_DMA, 16)))
			return ret;
=======
		pxa_register_wdt(RCSR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		pxa25x_init_pm();

		register_syscore_ops(&pxa_irq_syscore_ops);
		register_syscore_ops(&pxa2xx_mfp_syscore_ops);
<<<<<<< HEAD
		register_syscore_ops(&pxa2xx_clock_syscore_ops);

		ret = platform_add_devices(pxa25x_devices,
					   ARRAY_SIZE(pxa25x_devices));
		if (ret)
			return ret;
	}

	/* Only add HWUART for PXA255/26x; PXA210/250 do not have it. */
	if (cpu_is_pxa255())
		clkdev_add(&pxa25x_hwuart_clkreg);

=======

		if (!of_have_populated_dt()) {
			pxa2xx_set_dmac_info(&pxa25x_dma_pdata);
			pxa_register_device(&pxa25x_device_gpio, &pxa25x_gpio_info);
			ret = platform_add_devices(pxa25x_devices,
						   ARRAY_SIZE(pxa25x_devices));
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

postcore_initcall(pxa25x_init);
