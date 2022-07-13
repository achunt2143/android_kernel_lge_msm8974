<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Based on sound/arm/pxa2xx-ac97.c and sound/soc/pxa/pxa2xx-ac97.c
 * which contain:
 *
 * Author:	Nicolas Pitre
 * Created:	Dec 02, 2004
 * Copyright:	MontaVista Software Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/gpio.h>
<<<<<<< HEAD

#include <sound/ac97_codec.h>
#include <sound/pxa2xx-lib.h>

#include <mach/irqs.h>
#include <mach/regs-ac97.h>
#include <mach/audio.h>
=======
#include <linux/of_gpio.h>
#include <linux/soc/pxa/cpu.h>

#include <sound/pxa2xx-lib.h>

#include <linux/platform_data/asoc-pxa.h>

#include "pxa2xx-ac97-regs.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_MUTEX(car_mutex);
static DECLARE_WAIT_QUEUE_HEAD(gsr_wq);
static volatile long gsr_bits;
static struct clk *ac97_clk;
static struct clk *ac97conf_clk;
static int reset_gpio;
<<<<<<< HEAD

extern void pxa27x_assert_ac97reset(int reset_gpio, int on);
=======
static void __iomem *ac97_reg_base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Beware PXA27x bugs:
 *
 *   o Slot 12 read from modem space will hang controller.
 *   o CDONE, SDONE interrupt fails after any slot 12 IO.
 *
 * We therefore have an hybrid approach for waiting on SDONE (interrupt or
 * 1 jiffy timeout if interrupt never comes).
 */

<<<<<<< HEAD
unsigned short pxa2xx_ac97_read(struct snd_ac97 *ac97, unsigned short reg)
{
	unsigned short val = -1;
	volatile u32 *reg_addr;
=======
int pxa2xx_ac97_read(int slot, unsigned short reg)
{
	int val = -ENODEV;
	u32 __iomem *reg_addr;

	if (slot > 0)
		return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&car_mutex);

	/* set up primary or secondary codec space */
	if (cpu_is_pxa25x() && reg == AC97_GPIO_STATUS)
<<<<<<< HEAD
		reg_addr = ac97->num ? &SMC_REG_BASE : &PMC_REG_BASE;
	else
		reg_addr = ac97->num ? &SAC_REG_BASE : &PAC_REG_BASE;
	reg_addr += (reg >> 1);

	/* start read access across the ac97 link */
	GSR = GSR_CDONE | GSR_SDONE;
	gsr_bits = 0;
	val = *reg_addr;
	if (reg == AC97_GPIO_STATUS)
		goto out;
	if (wait_event_timeout(gsr_wq, (GSR | gsr_bits) & GSR_SDONE, 1) <= 0 &&
	    !((GSR | gsr_bits) & GSR_SDONE)) {
		printk(KERN_ERR "%s: read error (ac97_reg=%d GSR=%#lx)\n",
				__func__, reg, GSR | gsr_bits);
		val = -1;
=======
		reg_addr = ac97_reg_base +
			   (slot ? SMC_REG_BASE : PMC_REG_BASE);
	else
		reg_addr = ac97_reg_base +
			   (slot ? SAC_REG_BASE : PAC_REG_BASE);
	reg_addr += (reg >> 1);

	/* start read access across the ac97 link */
	writel(GSR_CDONE | GSR_SDONE, ac97_reg_base + GSR);
	gsr_bits = 0;
	val = (readl(reg_addr) & 0xffff);
	if (reg == AC97_GPIO_STATUS)
		goto out;
	if (wait_event_timeout(gsr_wq, (readl(ac97_reg_base + GSR) | gsr_bits) & GSR_SDONE, 1) <= 0 &&
	    !((readl(ac97_reg_base + GSR) | gsr_bits) & GSR_SDONE)) {
		printk(KERN_ERR "%s: read error (ac97_reg=%d GSR=%#lx)\n",
				__func__, reg, readl(ac97_reg_base + GSR) | gsr_bits);
		val = -ETIMEDOUT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/* valid data now */
<<<<<<< HEAD
	GSR = GSR_CDONE | GSR_SDONE;
	gsr_bits = 0;
	val = *reg_addr;
	/* but we've just started another cycle... */
	wait_event_timeout(gsr_wq, (GSR | gsr_bits) & GSR_SDONE, 1);
=======
	writel(GSR_CDONE | GSR_SDONE, ac97_reg_base + GSR);
	gsr_bits = 0;
	val = (readl(reg_addr) & 0xffff);
	/* but we've just started another cycle... */
	wait_event_timeout(gsr_wq, (readl(ac97_reg_base + GSR) | gsr_bits) & GSR_SDONE, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:	mutex_unlock(&car_mutex);
	return val;
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_read);

<<<<<<< HEAD
void pxa2xx_ac97_write(struct snd_ac97 *ac97, unsigned short reg,
			unsigned short val)
{
	volatile u32 *reg_addr;
=======
int pxa2xx_ac97_write(int slot, unsigned short reg, unsigned short val)
{
	u32 __iomem *reg_addr;
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&car_mutex);

	/* set up primary or secondary codec space */
	if (cpu_is_pxa25x() && reg == AC97_GPIO_STATUS)
<<<<<<< HEAD
		reg_addr = ac97->num ? &SMC_REG_BASE : &PMC_REG_BASE;
	else
		reg_addr = ac97->num ? &SAC_REG_BASE : &PAC_REG_BASE;
	reg_addr += (reg >> 1);

	GSR = GSR_CDONE | GSR_SDONE;
	gsr_bits = 0;
	*reg_addr = val;
	if (wait_event_timeout(gsr_wq, (GSR | gsr_bits) & GSR_CDONE, 1) <= 0 &&
	    !((GSR | gsr_bits) & GSR_CDONE))
		printk(KERN_ERR "%s: write error (ac97_reg=%d GSR=%#lx)\n",
				__func__, reg, GSR | gsr_bits);

	mutex_unlock(&car_mutex);
=======
		reg_addr = ac97_reg_base +
			   (slot ? SMC_REG_BASE : PMC_REG_BASE);
	else
		reg_addr = ac97_reg_base +
			   (slot ? SAC_REG_BASE : PAC_REG_BASE);
	reg_addr += (reg >> 1);

	writel(GSR_CDONE | GSR_SDONE, ac97_reg_base + GSR);
	gsr_bits = 0;
	writel(val, reg_addr);
	if (wait_event_timeout(gsr_wq, (readl(ac97_reg_base + GSR) | gsr_bits) & GSR_CDONE, 1) <= 0 &&
	    !((readl(ac97_reg_base + GSR) | gsr_bits) & GSR_CDONE)) {
		printk(KERN_ERR "%s: write error (ac97_reg=%d GSR=%#lx)\n",
				__func__, reg, readl(ac97_reg_base + GSR) | gsr_bits);
		ret = -EIO;
	}

	mutex_unlock(&car_mutex);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_write);

#ifdef CONFIG_PXA25x
static inline void pxa_ac97_warm_pxa25x(void)
{
	gsr_bits = 0;

<<<<<<< HEAD
	GCR |= GCR_WARM_RST | GCR_PRIRDY_IEN | GCR_SECRDY_IEN;
	wait_event_timeout(gsr_wq, gsr_bits & (GSR_PCR | GSR_SCR), 1);
=======
	writel(readl(ac97_reg_base + GCR) | (GCR_WARM_RST), ac97_reg_base + GCR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void pxa_ac97_cold_pxa25x(void)
{
<<<<<<< HEAD
	GCR &=  GCR_COLD_RST;  /* clear everything but nCRST */
	GCR &= ~GCR_COLD_RST;  /* then assert nCRST */

	gsr_bits = 0;

	GCR = GCR_COLD_RST;
	GCR |= GCR_CDONE_IE|GCR_SDONE_IE;
	wait_event_timeout(gsr_wq, gsr_bits & (GSR_PCR | GSR_SCR), 1);
=======
	writel(readl(ac97_reg_base + GCR) & ( GCR_COLD_RST), ac97_reg_base + GCR);  /* clear everything but nCRST */
	writel(readl(ac97_reg_base + GCR) & (~GCR_COLD_RST), ac97_reg_base + GCR);  /* then assert nCRST */

	gsr_bits = 0;

	writel(GCR_COLD_RST, ac97_reg_base + GCR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

#ifdef CONFIG_PXA27x
static inline void pxa_ac97_warm_pxa27x(void)
{
	gsr_bits = 0;

	/* warm reset broken on Bulverde, so manually keep AC97 reset high */
<<<<<<< HEAD
	pxa27x_assert_ac97reset(reset_gpio, 1);
	udelay(10);
	GCR |= GCR_WARM_RST;
	pxa27x_assert_ac97reset(reset_gpio, 0);
=======
	pxa27x_configure_ac97reset(reset_gpio, true);
	udelay(10);
	writel(readl(ac97_reg_base + GCR) | (GCR_WARM_RST), ac97_reg_base + GCR);
	pxa27x_configure_ac97reset(reset_gpio, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	udelay(500);
}

static inline void pxa_ac97_cold_pxa27x(void)
{
<<<<<<< HEAD
	unsigned int timeout;

	GCR &=  GCR_COLD_RST;  /* clear everything but nCRST */
	GCR &= ~GCR_COLD_RST;  /* then assert nCRST */
=======
	writel(readl(ac97_reg_base + GCR) & ( GCR_COLD_RST), ac97_reg_base + GCR);  /* clear everything but nCRST */
	writel(readl(ac97_reg_base + GCR) & (~GCR_COLD_RST), ac97_reg_base + GCR);  /* then assert nCRST */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	gsr_bits = 0;

	/* PXA27x Developers Manual section 13.5.2.2.1 */
<<<<<<< HEAD
	clk_enable(ac97conf_clk);
	udelay(5);
	clk_disable(ac97conf_clk);
	GCR = GCR_COLD_RST | GCR_WARM_RST;
	timeout = 100;     /* wait for the codec-ready bit to be set */
	while (!((GSR | gsr_bits) & (GSR_PCR | GSR_SCR)) && timeout--)
		mdelay(1);
=======
	clk_prepare_enable(ac97conf_clk);
	udelay(5);
	clk_disable_unprepare(ac97conf_clk);
	writel(GCR_COLD_RST | GCR_WARM_RST, ac97_reg_base + GCR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

#ifdef CONFIG_PXA3xx
static inline void pxa_ac97_warm_pxa3xx(void)
{
<<<<<<< HEAD
	int timeout = 100;

	gsr_bits = 0;

	/* Can't use interrupts */
	GCR |= GCR_WARM_RST;
	while (!((GSR | gsr_bits) & (GSR_PCR | GSR_SCR)) && timeout--)
		mdelay(1);
=======
	gsr_bits = 0;

	/* Can't use interrupts */
	writel(readl(ac97_reg_base + GCR) | (GCR_WARM_RST), ac97_reg_base + GCR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void pxa_ac97_cold_pxa3xx(void)
{
<<<<<<< HEAD
	int timeout = 1000;

	/* Hold CLKBPB for 100us */
	GCR = 0;
	GCR = GCR_CLKBPB;
	udelay(100);
	GCR = 0;

	GCR &=  GCR_COLD_RST;  /* clear everything but nCRST */
	GCR &= ~GCR_COLD_RST;  /* then assert nCRST */
=======
	/* Hold CLKBPB for 100us */
	writel(0, ac97_reg_base + GCR);
	writel(GCR_CLKBPB, ac97_reg_base + GCR);
	udelay(100);
	writel(0, ac97_reg_base + GCR);

	writel(readl(ac97_reg_base + GCR) & ( GCR_COLD_RST), ac97_reg_base + GCR);  /* clear everything but nCRST */
	writel(readl(ac97_reg_base + GCR) & (~GCR_COLD_RST), ac97_reg_base + GCR);  /* then assert nCRST */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	gsr_bits = 0;

	/* Can't use interrupts on PXA3xx */
<<<<<<< HEAD
	GCR &= ~(GCR_PRIRDY_IEN|GCR_SECRDY_IEN);

	GCR = GCR_WARM_RST | GCR_COLD_RST;
	while (!(GSR & (GSR_PCR | GSR_SCR)) && timeout--)
		mdelay(10);
}
#endif

bool pxa2xx_ac97_try_warm_reset(struct snd_ac97 *ac97)
{
	unsigned long gsr;
=======
	writel(readl(ac97_reg_base + GCR) & (~(GCR_PRIRDY_IEN|GCR_SECRDY_IEN)), ac97_reg_base + GCR);

	writel(GCR_WARM_RST | GCR_COLD_RST, ac97_reg_base + GCR);
}
#endif

bool pxa2xx_ac97_try_warm_reset(void)
{
	unsigned long gsr;
	unsigned int timeout = 100;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PXA25x
	if (cpu_is_pxa25x())
		pxa_ac97_warm_pxa25x();
	else
#endif
#ifdef CONFIG_PXA27x
	if (cpu_is_pxa27x())
		pxa_ac97_warm_pxa27x();
	else
#endif
#ifdef CONFIG_PXA3xx
	if (cpu_is_pxa3xx())
		pxa_ac97_warm_pxa3xx();
	else
#endif
<<<<<<< HEAD
		BUG();
	gsr = GSR | gsr_bits;
=======
		snd_BUG();

	while (!((readl(ac97_reg_base + GSR) | gsr_bits) & (GSR_PCR | GSR_SCR)) && timeout--)
		mdelay(1);

	gsr = readl(ac97_reg_base + GSR) | gsr_bits;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(gsr & (GSR_PCR | GSR_SCR))) {
		printk(KERN_INFO "%s: warm reset timeout (GSR=%#lx)\n",
				 __func__, gsr);

		return false;
	}

	return true;
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_try_warm_reset);

<<<<<<< HEAD
bool pxa2xx_ac97_try_cold_reset(struct snd_ac97 *ac97)
{
	unsigned long gsr;
=======
bool pxa2xx_ac97_try_cold_reset(void)
{
	unsigned long gsr;
	unsigned int timeout = 1000;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PXA25x
	if (cpu_is_pxa25x())
		pxa_ac97_cold_pxa25x();
	else
#endif
#ifdef CONFIG_PXA27x
	if (cpu_is_pxa27x())
		pxa_ac97_cold_pxa27x();
	else
#endif
#ifdef CONFIG_PXA3xx
	if (cpu_is_pxa3xx())
		pxa_ac97_cold_pxa3xx();
	else
#endif
<<<<<<< HEAD
		BUG();

	gsr = GSR | gsr_bits;
=======
		snd_BUG();

	while (!((readl(ac97_reg_base + GSR) | gsr_bits) & (GSR_PCR | GSR_SCR)) && timeout--)
		mdelay(1);

	gsr = readl(ac97_reg_base + GSR) | gsr_bits;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(gsr & (GSR_PCR | GSR_SCR))) {
		printk(KERN_INFO "%s: cold reset timeout (GSR=%#lx)\n",
				 __func__, gsr);

		return false;
	}

	return true;
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_try_cold_reset);


<<<<<<< HEAD
void pxa2xx_ac97_finish_reset(struct snd_ac97 *ac97)
{
	GCR &= ~(GCR_PRIRDY_IEN|GCR_SECRDY_IEN);
	GCR |= GCR_SDONE_IE|GCR_CDONE_IE;
=======
void pxa2xx_ac97_finish_reset(void)
{
	u32 gcr = readl(ac97_reg_base + GCR);
	gcr &= ~(GCR_PRIRDY_IEN|GCR_SECRDY_IEN);
	gcr |= GCR_SDONE_IE|GCR_CDONE_IE;
	writel(gcr, ac97_reg_base + GCR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_finish_reset);

static irqreturn_t pxa2xx_ac97_irq(int irq, void *dev_id)
{
	long status;

<<<<<<< HEAD
	status = GSR;
	if (status) {
		GSR = status;
=======
	status = readl(ac97_reg_base + GSR);
	if (status) {
		writel(status, ac97_reg_base + GSR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		gsr_bits |= status;
		wake_up(&gsr_wq);

		/* Although we don't use those we still need to clear them
		   since they tend to spuriously trigger when MMC is used
		   (hardware bug? go figure)... */
		if (cpu_is_pxa27x()) {
<<<<<<< HEAD
			MISR = MISR_EOC;
			PISR = PISR_EOC;
			MCSR = MCSR_EOC;
=======
			writel(MISR_EOC, ac97_reg_base + MISR);
			writel(PISR_EOC, ac97_reg_base + PISR);
			writel(MCSR_EOC, ac97_reg_base + MCSR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

#ifdef CONFIG_PM
int pxa2xx_ac97_hw_suspend(void)
{
<<<<<<< HEAD
	GCR |= GCR_ACLINK_OFF;
	clk_disable(ac97_clk);
=======
	writel(readl(ac97_reg_base + GCR) | (GCR_ACLINK_OFF), ac97_reg_base + GCR);
	clk_disable_unprepare(ac97_clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_hw_suspend);

int pxa2xx_ac97_hw_resume(void)
{
<<<<<<< HEAD
	clk_enable(ac97_clk);
=======
	clk_prepare_enable(ac97_clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_hw_resume);
#endif

<<<<<<< HEAD
int __devinit pxa2xx_ac97_hw_probe(struct platform_device *dev)
{
	int ret;
	pxa2xx_audio_ops_t *pdata = dev->dev.platform_data;

=======
int pxa2xx_ac97_hw_probe(struct platform_device *dev)
{
	int ret;
	int irq;
	pxa2xx_audio_ops_t *pdata = dev->dev.platform_data;

	ac97_reg_base = devm_platform_ioremap_resource(dev, 0);
	if (IS_ERR(ac97_reg_base)) {
		dev_err(&dev->dev, "Missing MMIO resource\n");
		return PTR_ERR(ac97_reg_base);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pdata) {
		switch (pdata->reset_gpio) {
		case 95:
		case 113:
			reset_gpio = pdata->reset_gpio;
			break;
		case 0:
			reset_gpio = 113;
			break;
		case -1:
			break;
		default:
			dev_err(&dev->dev, "Invalid reset GPIO %d\n",
				pdata->reset_gpio);
		}
<<<<<<< HEAD
=======
	} else if (!pdata && dev->dev.of_node) {
		pdata = devm_kzalloc(&dev->dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata)
			return -ENOMEM;
		pdata->reset_gpio = of_get_named_gpio(dev->dev.of_node,
						      "reset-gpios", 0);
		if (pdata->reset_gpio == -ENOENT)
			pdata->reset_gpio = -1;
		else if (pdata->reset_gpio < 0)
			return pdata->reset_gpio;
		reset_gpio = pdata->reset_gpio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (cpu_is_pxa27x())
			reset_gpio = 113;
	}

	if (cpu_is_pxa27x()) {
		/*
		 * This gpio is needed for a work-around to a bug in the ac97
		 * controller during warm reset.  The direction and level is set
		 * here so that it is an output driven high when switching from
		 * AC97_nRESET alt function to generic gpio.
		 */
		ret = gpio_request_one(reset_gpio, GPIOF_OUT_INIT_HIGH,
				       "pxa27x ac97 reset");
		if (ret < 0) {
			pr_err("%s: gpio_request_one() failed: %d\n",
			       __func__, ret);
			goto err_conf;
		}
<<<<<<< HEAD
		pxa27x_assert_ac97reset(reset_gpio, 0);
=======
		pxa27x_configure_ac97reset(reset_gpio, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ac97conf_clk = clk_get(&dev->dev, "AC97CONFCLK");
		if (IS_ERR(ac97conf_clk)) {
			ret = PTR_ERR(ac97conf_clk);
			ac97conf_clk = NULL;
			goto err_conf;
		}
	}

	ac97_clk = clk_get(&dev->dev, "AC97CLK");
	if (IS_ERR(ac97_clk)) {
		ret = PTR_ERR(ac97_clk);
		ac97_clk = NULL;
		goto err_clk;
	}

<<<<<<< HEAD
	ret = clk_enable(ac97_clk);
	if (ret)
		goto err_clk2;

	ret = request_irq(IRQ_AC97, pxa2xx_ac97_irq, 0, "AC97", NULL);
=======
	ret = clk_prepare_enable(ac97_clk);
	if (ret)
		goto err_clk2;

	irq = platform_get_irq(dev, 0);
	if (irq < 0) {
		ret = irq;
		goto err_irq;
	}

	ret = request_irq(irq, pxa2xx_ac97_irq, 0, "AC97", NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		goto err_irq;

	return 0;

err_irq:
<<<<<<< HEAD
	GCR |= GCR_ACLINK_OFF;
=======
	writel(readl(ac97_reg_base + GCR) | (GCR_ACLINK_OFF), ac97_reg_base + GCR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_clk2:
	clk_put(ac97_clk);
	ac97_clk = NULL;
err_clk:
	if (ac97conf_clk) {
		clk_put(ac97conf_clk);
		ac97conf_clk = NULL;
	}
err_conf:
	return ret;
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_hw_probe);

void pxa2xx_ac97_hw_remove(struct platform_device *dev)
{
	if (cpu_is_pxa27x())
		gpio_free(reset_gpio);
<<<<<<< HEAD
	GCR |= GCR_ACLINK_OFF;
	free_irq(IRQ_AC97, NULL);
=======
	writel(readl(ac97_reg_base + GCR) | (GCR_ACLINK_OFF), ac97_reg_base + GCR);
	free_irq(platform_get_irq(dev, 0), NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ac97conf_clk) {
		clk_put(ac97conf_clk);
		ac97conf_clk = NULL;
	}
<<<<<<< HEAD
	clk_disable(ac97_clk);
=======
	clk_disable_unprepare(ac97_clk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clk_put(ac97_clk);
	ac97_clk = NULL;
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_hw_remove);

<<<<<<< HEAD
=======
u32 pxa2xx_ac97_read_modr(void)
{
	if (!ac97_reg_base)
		return 0;

	return readl(ac97_reg_base + MODR);
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_read_modr);

u32 pxa2xx_ac97_read_misr(void)
{
	if (!ac97_reg_base)
		return 0;

	return readl(ac97_reg_base + MISR);
}
EXPORT_SYMBOL_GPL(pxa2xx_ac97_read_misr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_AUTHOR("Nicolas Pitre");
MODULE_DESCRIPTION("Intel/Marvell PXA sound library");
MODULE_LICENSE("GPL");

