<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/mach-pxa/pxa27x.c
 *
 *  Author:	Nicolas Pitre
 *  Created:	Nov 05, 2002
 *  Copyright:	MontaVista Software Inc.
 *
 * Code specific to PXA27x aka Bulverde.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
=======
 */
#include <linux/dmaengine.h>
#include <linux/dma/pxa-dma.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/gpio.h>
#include <linux/gpio-pxa.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
<<<<<<< HEAD
=======
#include <linux/irqchip.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/suspend.h>
#include <linux/platform_device.h>
#include <linux/syscore_ops.h>
#include <linux/io.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/i2c/pxa-i2c.h>

#include <asm/mach/map.h>
#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/suspend.h>
#include <mach/irqs.h>
#include <mach/pxa27x.h>
#include <mach/reset.h>
#include <mach/ohci.h>
#include <mach/pm.h>
#include <mach/dma.h>
#include <mach/smemc.h>

#include "generic.h"
#include "devices.h"
#include "clock.h"
=======
#include <linux/platform_data/i2c-pxa.h>
#include <linux/platform_data/mmp_dma.h>
#include <linux/soc/pxa/cpu.h>
#include <linux/soc/pxa/smemc.h>

#include <asm/mach/map.h>
#include <asm/irq.h>
#include <asm/suspend.h>
#include "irqs.h"
#include "pxa27x.h"
#include "reset.h"
#include <linux/platform_data/pxa2xx_udc.h>
#include <linux/platform_data/usb-ohci-pxa27x.h>
#include <linux/platform_data/asoc-pxa.h>
#include "pm.h"
#include "addr-map.h"
#include "smemc.h"

#include "generic.h"
#include "devices.h"
#include <linux/clk-provider.h>
#include <linux/clkdev.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void pxa27x_clear_otgph(void)
{
	if (cpu_is_pxa27x() && (PSSR & PSSR_OTGPH))
		PSSR |= PSSR_OTGPH;
}
EXPORT_SYMBOL(pxa27x_clear_otgph);

static unsigned long ac97_reset_config[] = {
	GPIO113_AC97_nRESET_GPIO_HIGH,
	GPIO113_AC97_nRESET,
	GPIO95_AC97_nRESET_GPIO_HIGH,
	GPIO95_AC97_nRESET,
};

<<<<<<< HEAD
void pxa27x_assert_ac97reset(int reset_gpio, int on)
{
	if (reset_gpio == 113)
		pxa2xx_mfp_config(on ? &ac97_reset_config[0] :
				       &ac97_reset_config[1], 1);

	if (reset_gpio == 95)
		pxa2xx_mfp_config(on ? &ac97_reset_config[2] :
				       &ac97_reset_config[3], 1);
}
EXPORT_SYMBOL_GPL(pxa27x_assert_ac97reset);

/* Crystal clock: 13MHz */
#define BASE_CLK	13000000

/*
 * Get the clock frequency as reflected by CCSR and the turbo flag.
 * We assume these values have been applied via a fcs.
 * If info is not 0 we also display the current settings.
 */
unsigned int pxa27x_get_clk_frequency_khz(int info)
{
	unsigned long ccsr, clkcfg;
	unsigned int l, L, m, M, n2, N, S;
       	int cccr_a, t, ht, b;

	ccsr = CCSR;
	cccr_a = CCCR & (1 << 25);

	/* Read clkcfg register: it has turbo, b, half-turbo (and f) */
	asm( "mrc\tp14, 0, %0, c6, c0, 0" : "=r" (clkcfg) );
	t  = clkcfg & (1 << 0);
	ht = clkcfg & (1 << 2);
	b  = clkcfg & (1 << 3);

	l  = ccsr & 0x1f;
	n2 = (ccsr>>7) & 0xf;
	m  = (l <= 10) ? 1 : (l <= 20) ? 2 : 4;

	L  = l * BASE_CLK;
	N  = (L * n2) / 2;
	M  = (!cccr_a) ? (L/m) : ((b) ? L : (L/2));
	S  = (b) ? L : (L/2);

	if (info) {
		printk( KERN_INFO "Run Mode clock: %d.%02dMHz (*%d)\n",
			L / 1000000, (L % 1000000) / 10000, l );
		printk( KERN_INFO "Turbo Mode clock: %d.%02dMHz (*%d.%d, %sactive)\n",
			N / 1000000, (N % 1000000)/10000, n2 / 2, (n2 % 2)*5,
			(t) ? "" : "in" );
		printk( KERN_INFO "Memory clock: %d.%02dMHz (/%d)\n",
			M / 1000000, (M % 1000000) / 10000, m );
		printk( KERN_INFO "System bus clock: %d.%02dMHz \n",
			S / 1000000, (S % 1000000) / 10000 );
	}

	return (t) ? (N/1000) : (L/1000);
}

/*
 * Return the current mem clock frequency as reflected by CCCR[A], B, and L
 */
static unsigned long clk_pxa27x_mem_getrate(struct clk *clk)
{
	unsigned long ccsr, clkcfg;
	unsigned int l, L, m, M;
       	int cccr_a, b;

	ccsr = CCSR;
	cccr_a = CCCR & (1 << 25);

	/* Read clkcfg register: it has turbo, b, half-turbo (and f) */
	asm( "mrc\tp14, 0, %0, c6, c0, 0" : "=r" (clkcfg) );
	b = clkcfg & (1 << 3);

	l = ccsr & 0x1f;
	m = (l <= 10) ? 1 : (l <= 20) ? 2 : 4;

	L = l * BASE_CLK;
	M = (!cccr_a) ? (L/m) : ((b) ? L : (L/2));

	return M;
}

static const struct clkops clk_pxa27x_mem_ops = {
	.enable		= clk_dummy_enable,
	.disable	= clk_dummy_disable,
	.getrate	= clk_pxa27x_mem_getrate,
};

/*
 * Return the current LCD clock frequency in units of 10kHz as
 */
static unsigned int pxa27x_get_lcdclk_frequency_10khz(void)
{
	unsigned long ccsr;
	unsigned int l, L, k, K;

	ccsr = CCSR;

	l = ccsr & 0x1f;
	k = (l <= 7) ? 1 : (l <= 16) ? 2 : 4;

	L = l * BASE_CLK;
	K = L / k;

	return (K / 10000);
}

static unsigned long clk_pxa27x_lcd_getrate(struct clk *clk)
{
	return pxa27x_get_lcdclk_frequency_10khz() * 10000;
}

static const struct clkops clk_pxa27x_lcd_ops = {
	.enable		= clk_pxa2xx_cken_enable,
	.disable	= clk_pxa2xx_cken_disable,
	.getrate	= clk_pxa27x_lcd_getrate,
};

static DEFINE_PXA2_CKEN(pxa27x_ffuart, FFUART, 14857000, 1);
static DEFINE_PXA2_CKEN(pxa27x_btuart, BTUART, 14857000, 1);
static DEFINE_PXA2_CKEN(pxa27x_stuart, STUART, 14857000, 1);
static DEFINE_PXA2_CKEN(pxa27x_i2s, I2S, 14682000, 0);
static DEFINE_PXA2_CKEN(pxa27x_i2c, I2C, 32842000, 0);
static DEFINE_PXA2_CKEN(pxa27x_usb, USB, 48000000, 5);
static DEFINE_PXA2_CKEN(pxa27x_mmc, MMC, 19500000, 0);
static DEFINE_PXA2_CKEN(pxa27x_ficp, FICP, 48000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_usbhost, USBHOST, 48000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_pwri2c, PWRI2C, 13000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_keypad, KEYPAD, 32768, 0);
static DEFINE_PXA2_CKEN(pxa27x_ssp1, SSP1, 13000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_ssp2, SSP2, 13000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_ssp3, SSP3, 13000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_pwm0, PWM0, 13000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_pwm1, PWM1, 13000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_ac97, AC97, 24576000, 0);
static DEFINE_PXA2_CKEN(pxa27x_ac97conf, AC97CONF, 24576000, 0);
static DEFINE_PXA2_CKEN(pxa27x_msl, MSL, 48000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_usim, USIM, 48000000, 0);
static DEFINE_PXA2_CKEN(pxa27x_memstk, MEMSTK, 19500000, 0);
static DEFINE_PXA2_CKEN(pxa27x_im, IM, 0, 0);
static DEFINE_PXA2_CKEN(pxa27x_memc, MEMC, 0, 0);

static DEFINE_CK(pxa27x_lcd, LCD, &clk_pxa27x_lcd_ops);
static DEFINE_CK(pxa27x_camera, CAMERA, &clk_pxa27x_lcd_ops);
static DEFINE_CLK(pxa27x_mem, &clk_pxa27x_mem_ops, 0, 0);

static struct clk_lookup pxa27x_clkregs[] = {
	INIT_CLKREG(&clk_pxa27x_lcd, "pxa2xx-fb", NULL),
	INIT_CLKREG(&clk_pxa27x_camera, "pxa27x-camera.0", NULL),
	INIT_CLKREG(&clk_pxa27x_ffuart, "pxa2xx-uart.0", NULL),
	INIT_CLKREG(&clk_pxa27x_btuart, "pxa2xx-uart.1", NULL),
	INIT_CLKREG(&clk_pxa27x_stuart, "pxa2xx-uart.2", NULL),
	INIT_CLKREG(&clk_pxa27x_i2s, "pxa2xx-i2s", NULL),
	INIT_CLKREG(&clk_pxa27x_i2c, "pxa2xx-i2c.0", NULL),
	INIT_CLKREG(&clk_pxa27x_usb, "pxa27x-udc", NULL),
	INIT_CLKREG(&clk_pxa27x_mmc, "pxa2xx-mci.0", NULL),
	INIT_CLKREG(&clk_pxa27x_stuart, "pxa2xx-ir", "UARTCLK"),
	INIT_CLKREG(&clk_pxa27x_ficp, "pxa2xx-ir", "FICPCLK"),
	INIT_CLKREG(&clk_pxa27x_usbhost, "pxa27x-ohci", NULL),
	INIT_CLKREG(&clk_pxa27x_pwri2c, "pxa2xx-i2c.1", NULL),
	INIT_CLKREG(&clk_pxa27x_keypad, "pxa27x-keypad", NULL),
	INIT_CLKREG(&clk_pxa27x_ssp1, "pxa27x-ssp.0", NULL),
	INIT_CLKREG(&clk_pxa27x_ssp2, "pxa27x-ssp.1", NULL),
	INIT_CLKREG(&clk_pxa27x_ssp3, "pxa27x-ssp.2", NULL),
	INIT_CLKREG(&clk_pxa27x_pwm0, "pxa27x-pwm.0", NULL),
	INIT_CLKREG(&clk_pxa27x_pwm1, "pxa27x-pwm.1", NULL),
	INIT_CLKREG(&clk_pxa27x_ac97, NULL, "AC97CLK"),
	INIT_CLKREG(&clk_pxa27x_ac97conf, NULL, "AC97CONFCLK"),
	INIT_CLKREG(&clk_pxa27x_msl, NULL, "MSLCLK"),
	INIT_CLKREG(&clk_pxa27x_usim, NULL, "USIMCLK"),
	INIT_CLKREG(&clk_pxa27x_memstk, NULL, "MSTKCLK"),
	INIT_CLKREG(&clk_pxa27x_im, NULL, "IMCLK"),
	INIT_CLKREG(&clk_pxa27x_memc, NULL, "MEMCLK"),
	INIT_CLKREG(&clk_pxa27x_mem, "pxa2xx-pcmcia", NULL),
	INIT_CLKREG(&clk_dummy, "pxa-gpio", NULL),
};
=======
void pxa27x_configure_ac97reset(int reset_gpio, bool to_gpio)
{
	/*
	 * This helper function is used to work around a bug in the pxa27x's
	 * ac97 controller during a warm reset.  The configuration of the
	 * reset_gpio is changed as follows:
	 * to_gpio == true: configured to generic output gpio and driven high
	 * to_gpio == false: configured to ac97 controller alt fn AC97_nRESET
	 */

	if (reset_gpio == 113)
		pxa2xx_mfp_config(to_gpio ? &ac97_reset_config[0] :
				  &ac97_reset_config[1], 1);

	if (reset_gpio == 95)
		pxa2xx_mfp_config(to_gpio ? &ac97_reset_config[2] :
				  &ac97_reset_config[3], 1);
}
EXPORT_SYMBOL_GPL(pxa27x_configure_ac97reset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PM

#define SAVE(x)		sleep_save[SLEEP_SAVE_##x] = x
#define RESTORE(x)	x = sleep_save[SLEEP_SAVE_##x]

/*
 * allow platforms to override default PWRMODE setting used for PM_SUSPEND_MEM
 */
static unsigned int pwrmode = PWRMODE_SLEEP;

<<<<<<< HEAD
int pxa27x_set_pwrmode(unsigned int mode)
{
	switch (mode) {
	case PWRMODE_SLEEP:
	case PWRMODE_DEEPSLEEP:
		pwrmode = mode;
		return 0;
	}

	return -EINVAL;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * List of global PXA peripheral registers to preserve.
 * More ones like CP and general purpose register values are preserved
 * with the stack pointer in sleep.S.
 */
enum {
	SLEEP_SAVE_PSTR,
	SLEEP_SAVE_MDREFR,
	SLEEP_SAVE_PCFR,
	SLEEP_SAVE_COUNT
};

<<<<<<< HEAD
void pxa27x_cpu_pm_save(unsigned long *sleep_save)
=======
static void pxa27x_cpu_pm_save(unsigned long *sleep_save)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	sleep_save[SLEEP_SAVE_MDREFR] = __raw_readl(MDREFR);
	SAVE(PCFR);

	SAVE(PSTR);
}

<<<<<<< HEAD
void pxa27x_cpu_pm_restore(unsigned long *sleep_save)
=======
static void pxa27x_cpu_pm_restore(unsigned long *sleep_save)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__raw_writel(sleep_save[SLEEP_SAVE_MDREFR], MDREFR);
	RESTORE(PCFR);

	PSSR = PSSR_RDH | PSSR_PH;

	RESTORE(PSTR);
}

<<<<<<< HEAD
void pxa27x_cpu_pm_enter(suspend_state_t state)
=======
static void pxa27x_cpu_pm_enter(suspend_state_t state)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	extern void pxa_cpu_standby(void);
#ifndef CONFIG_IWMMXT
	u64 acc0;

<<<<<<< HEAD
	asm volatile("mra %Q0, %R0, acc0" : "=r" (acc0));
=======
#ifndef CONFIG_AS_IS_LLVM
	asm volatile(".arch_extension xscale\n\t"
		     "mra %Q0, %R0, acc0" : "=r" (acc0));
#else
	asm volatile("mrrc p0, 0, %Q0, %R0, c0" : "=r" (acc0));
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	/* ensure voltage-change sequencer not initiated, which hangs */
	PCFR &= ~PCFR_FVC;

	/* Clear edge-detect status register. */
	PEDR = 0xDF12FE1B;

	/* Clear reset status */
	RCSR = RCSR_HWR | RCSR_WDR | RCSR_SMR | RCSR_GPR;

	switch (state) {
	case PM_SUSPEND_STANDBY:
		pxa_cpu_standby();
		break;
	case PM_SUSPEND_MEM:
		cpu_suspend(pwrmode, pxa27x_finish_suspend);
#ifndef CONFIG_IWMMXT
<<<<<<< HEAD
		asm volatile("mar acc0, %Q0, %R0" : "=r" (acc0));
=======
#ifndef CONFIG_AS_IS_LLVM
		asm volatile(".arch_extension xscale\n\t"
			     "mar acc0, %Q0, %R0" : "=r" (acc0));
#else
		asm volatile("mcrr p0, 0, %Q0, %R0, c0" :: "r" (acc0));
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
		break;
	}
}

static int pxa27x_cpu_pm_valid(suspend_state_t state)
{
	return state == PM_SUSPEND_MEM || state == PM_SUSPEND_STANDBY;
}

static int pxa27x_cpu_pm_prepare(void)
{
	/* set resume return address */
<<<<<<< HEAD
	PSPR = virt_to_phys(cpu_resume);
=======
	PSPR = __pa_symbol(cpu_resume);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void pxa27x_cpu_pm_finish(void)
{
	/* ensure not to come back here if it wasn't intended */
	PSPR = 0;
}

static struct pxa_cpu_pm_fns pxa27x_cpu_pm_fns = {
	.save_count	= SLEEP_SAVE_COUNT,
	.save		= pxa27x_cpu_pm_save,
	.restore	= pxa27x_cpu_pm_restore,
	.valid		= pxa27x_cpu_pm_valid,
	.enter		= pxa27x_cpu_pm_enter,
	.prepare	= pxa27x_cpu_pm_prepare,
	.finish		= pxa27x_cpu_pm_finish,
};

static void __init pxa27x_init_pm(void)
{
	pxa_cpu_pm_fns = &pxa27x_cpu_pm_fns;
}
#else
static inline void pxa27x_init_pm(void) {}
#endif

/* PXA27x:  Various gpios can issue wakeup events.  This logic only
 * handles the simple cases, not the WEMUX2 and WEMUX3 options
 */
static int pxa27x_set_wake(struct irq_data *d, unsigned int on)
{
	int gpio = pxa_irq_to_gpio(d->irq);
	uint32_t mask;

	if (gpio >= 0 && gpio < 128)
		return gpio_set_wake(gpio, on);

	if (d->irq == IRQ_KEYPAD)
		return keypad_set_wake(on);

	switch (d->irq) {
	case IRQ_RTCAlrm:
		mask = PWER_RTC;
		break;
	case IRQ_USB:
		mask = 1u << 26;
		break;
	default:
		return -EINVAL;
	}

	if (on)
		PWER |= mask;
	else
		PWER &=~mask;

	return 0;
}

void __init pxa27x_init_irq(void)
{
	pxa_init_irq(34, pxa27x_set_wake);
<<<<<<< HEAD
}

=======
	set_handle_irq(pxa27x_handle_irq);
}

static int __init
pxa27x_dt_init_irq(struct device_node *node, struct device_node *parent)
{
	pxa_dt_irq_init(pxa27x_set_wake);
	set_handle_irq(ichp_handle_irq);

	return 0;
}
IRQCHIP_DECLARE(pxa27x_intc, "marvell,pxa-intc", pxa27x_dt_init_irq);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct map_desc pxa27x_io_desc[] __initdata = {
	{	/* Mem Ctl */
		.virtual	= (unsigned long)SMEMC_VIRT,
		.pfn		= __phys_to_pfn(PXA2XX_SMEMC_BASE),
<<<<<<< HEAD
		.length		= 0x00200000,
		.type		= MT_DEVICE
	}, {	/* IMem ctl */
		.virtual	=  0xfe000000,
		.pfn		= __phys_to_pfn(0x58000000),
		.length		= 0x00100000,
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

void __init pxa27x_map_io(void)
{
	pxa_map_io();
	iotable_init(ARRAY_AND_SIZE(pxa27x_io_desc));
	pxa27x_get_clk_frequency_khz(1);
}

/*
 * device registration specific to PXA27x.
 */
void __init pxa27x_set_i2c_power_info(struct i2c_pxa_platform_data *info)
{
	local_irq_disable();
	PCFR |= PCFR_PI2CEN;
	local_irq_enable();
	pxa_register_device(&pxa27x_device_i2c_power, info);
}

static struct pxa_gpio_platform_data pxa27x_gpio_info __initdata = {
<<<<<<< HEAD
	.gpio_set_wake = gpio_set_wake,
=======
	.irq_base	= PXA_GPIO_TO_IRQ(0),
	.gpio_set_wake	= gpio_set_wake,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct platform_device *devices[] __initdata = {
	&pxa27x_device_udc,
	&pxa_device_pmu,
	&pxa_device_i2s,
	&pxa_device_asoc_ssp1,
	&pxa_device_asoc_ssp2,
	&pxa_device_asoc_ssp3,
	&pxa_device_asoc_platform,
<<<<<<< HEAD
	&sa1100_device_rtc,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&pxa_device_rtc,
	&pxa27x_device_ssp1,
	&pxa27x_device_ssp2,
	&pxa27x_device_ssp3,
	&pxa27x_device_pwm0,
	&pxa27x_device_pwm1,
};

<<<<<<< HEAD
=======
static const struct dma_slave_map pxa27x_slave_map[] = {
	/* PXA25x, PXA27x and PXA3xx common entries */
	{ "pxa2xx-ac97", "pcm_pcm_mic_mono", PDMA_FILTER_PARAM(LOWEST, 8) },
	{ "pxa2xx-ac97", "pcm_pcm_aux_mono_in", PDMA_FILTER_PARAM(LOWEST, 9) },
	{ "pxa2xx-ac97", "pcm_pcm_aux_mono_out",
	  PDMA_FILTER_PARAM(LOWEST, 10) },
	{ "pxa2xx-ac97", "pcm_pcm_stereo_in", PDMA_FILTER_PARAM(LOWEST, 11) },
	{ "pxa2xx-ac97", "pcm_pcm_stereo_out", PDMA_FILTER_PARAM(LOWEST, 12) },
	{ "pxa-ssp-dai.0", "rx", PDMA_FILTER_PARAM(LOWEST, 13) },
	{ "pxa-ssp-dai.0", "tx", PDMA_FILTER_PARAM(LOWEST, 14) },
	{ "pxa-ssp-dai.1", "rx", PDMA_FILTER_PARAM(LOWEST, 15) },
	{ "pxa-ssp-dai.1", "tx", PDMA_FILTER_PARAM(LOWEST, 16) },
	{ "pxa2xx-ir", "rx", PDMA_FILTER_PARAM(LOWEST, 17) },
	{ "pxa2xx-ir", "tx", PDMA_FILTER_PARAM(LOWEST, 18) },
	{ "pxa2xx-mci.0", "rx", PDMA_FILTER_PARAM(LOWEST, 21) },
	{ "pxa2xx-mci.0", "tx", PDMA_FILTER_PARAM(LOWEST, 22) },
	{ "pxa-ssp-dai.2", "rx", PDMA_FILTER_PARAM(LOWEST, 66) },
	{ "pxa-ssp-dai.2", "tx", PDMA_FILTER_PARAM(LOWEST, 67) },

	/* PXA27x specific map */
	{ "pxa2xx-i2s", "rx", PDMA_FILTER_PARAM(LOWEST, 2) },
	{ "pxa2xx-i2s", "tx", PDMA_FILTER_PARAM(LOWEST, 3) },
	{ "pxa27x-camera.0", "CI_Y", PDMA_FILTER_PARAM(HIGHEST, 68) },
	{ "pxa27x-camera.0", "CI_U", PDMA_FILTER_PARAM(HIGHEST, 69) },
	{ "pxa27x-camera.0", "CI_V", PDMA_FILTER_PARAM(HIGHEST, 70) },
};

static struct mmp_dma_platdata pxa27x_dma_pdata = {
	.dma_channels	= 32,
	.nb_requestors	= 75,
	.slave_map	= pxa27x_slave_map,
	.slave_map_cnt	= ARRAY_SIZE(pxa27x_slave_map),
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init pxa27x_init(void)
{
	int ret = 0;

	if (cpu_is_pxa27x()) {

<<<<<<< HEAD
		reset_status = RCSR;

		clkdev_add_table(pxa27x_clkregs, ARRAY_SIZE(pxa27x_clkregs));

		if ((ret = pxa_init_dma(IRQ_DMA, 32)))
			return ret;
=======
		pxa_register_wdt(RCSR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		pxa27x_init_pm();

		register_syscore_ops(&pxa_irq_syscore_ops);
		register_syscore_ops(&pxa2xx_mfp_syscore_ops);
<<<<<<< HEAD
		register_syscore_ops(&pxa2xx_clock_syscore_ops);

		pxa_register_device(&pxa_device_gpio, &pxa27x_gpio_info);
		ret = platform_add_devices(devices, ARRAY_SIZE(devices));
=======

		if (!of_have_populated_dt()) {
			pxa_register_device(&pxa27x_device_gpio,
					    &pxa27x_gpio_info);
			pxa2xx_set_dmac_info(&pxa27x_dma_pdata);
			ret = platform_add_devices(devices,
						   ARRAY_SIZE(devices));
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return ret;
}

postcore_initcall(pxa27x_init);
