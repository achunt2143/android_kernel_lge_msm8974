<<<<<<< HEAD
/*
 * Generic GPIO driver for logic cells found in the Nomadik SoC
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * GPIO driver for the IP block found in the Nomadik SoC; it is an AMBA device,
 * managing 32 pins with alternate functions. It can also handle the STA2X11
 * block from ST.
 *
 * The GPIO chips are shared with pinctrl-nomadik if used; it needs access for
 * pinmuxing functionality and others.
 *
 * This driver also handles the mobileye,eyeq5-gpio compatible. It is an STA2X11
 * but with only data, direction and interrupts register active. We want to
 * avoid touching SLPM, RWIMSC, FWIMSC, AFSLA and AFSLB registers; that is,
 * wake and alternate function registers. It is NOT compatible with
 * pinctrl-nomadik.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Copyright (C) 2008,2009 STMicroelectronics
 * Copyright (C) 2009 Alessandro Rubini <rubini@unipv.it>
 *   Rewritten based on work by Prafulla WADASKAR <prafulla.wadaskar@st.com>
<<<<<<< HEAD
 * Copyright (C) 2011 Linus Walleij <linus.walleij@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>

#include <asm/mach/irq.h>

#include <plat/pincfg.h>
#include <plat/gpio-nomadik.h>
#include <mach/hardware.h>
#include <asm/gpio.h>

/*
 * The GPIO module in the Nomadik family of Systems-on-Chip is an
 * AMBA device, managing 32 pins and alternate functions.  The logic block
 * is currently used in the Nomadik and ux500.
 *
 * Symbols in this file are called "nmk_gpio" for "nomadik gpio"
 */

#define NMK_GPIO_PER_CHIP	32

struct nmk_gpio_chip {
	struct gpio_chip chip;
	void __iomem *addr;
	struct clk *clk;
	unsigned int bank;
	unsigned int parent_irq;
	int secondary_parent_irq;
	u32 (*get_secondary_status)(unsigned int bank);
	void (*set_ioforce)(bool enable);
	spinlock_t lock;
	bool sleepmode;
	/* Keep track of configured edges */
	u32 edge_rising;
	u32 edge_falling;
	u32 real_wake;
	u32 rwimsc;
	u32 fwimsc;
	u32 slpm;
	u32 pull_up;
};

static struct nmk_gpio_chip *
nmk_gpio_chips[DIV_ROUND_UP(ARCH_NR_GPIOS, NMK_GPIO_PER_CHIP)];

static DEFINE_SPINLOCK(nmk_gpio_slpm_lock);

#define NUM_BANKS ARRAY_SIZE(nmk_gpio_chips)

static void __nmk_gpio_set_mode(struct nmk_gpio_chip *nmk_chip,
				unsigned offset, int gpio_mode)
{
	u32 bit = 1 << offset;
	u32 afunc, bfunc;

	afunc = readl(nmk_chip->addr + NMK_GPIO_AFSLA) & ~bit;
	bfunc = readl(nmk_chip->addr + NMK_GPIO_AFSLB) & ~bit;
	if (gpio_mode & NMK_GPIO_ALT_A)
		afunc |= bit;
	if (gpio_mode & NMK_GPIO_ALT_B)
		bfunc |= bit;
	writel(afunc, nmk_chip->addr + NMK_GPIO_AFSLA);
	writel(bfunc, nmk_chip->addr + NMK_GPIO_AFSLB);
}

static void __nmk_gpio_set_slpm(struct nmk_gpio_chip *nmk_chip,
				unsigned offset, enum nmk_gpio_slpm mode)
{
	u32 bit = 1 << offset;
	u32 slpm;

	slpm = readl(nmk_chip->addr + NMK_GPIO_SLPC);
	if (mode == NMK_GPIO_SLPM_NOCHANGE)
		slpm |= bit;
	else
		slpm &= ~bit;
	writel(slpm, nmk_chip->addr + NMK_GPIO_SLPC);
}

static void __nmk_gpio_set_pull(struct nmk_gpio_chip *nmk_chip,
				unsigned offset, enum nmk_gpio_pull pull)
{
	u32 bit = 1 << offset;
	u32 pdis;

	pdis = readl(nmk_chip->addr + NMK_GPIO_PDIS);
	if (pull == NMK_GPIO_PULL_NONE) {
		pdis |= bit;
		nmk_chip->pull_up &= ~bit;
	} else {
		pdis &= ~bit;
	}

	writel(pdis, nmk_chip->addr + NMK_GPIO_PDIS);

	if (pull == NMK_GPIO_PULL_UP) {
		nmk_chip->pull_up |= bit;
		writel(bit, nmk_chip->addr + NMK_GPIO_DATS);
	} else if (pull == NMK_GPIO_PULL_DOWN) {
		nmk_chip->pull_up &= ~bit;
		writel(bit, nmk_chip->addr + NMK_GPIO_DATC);
	}
}

static void __nmk_gpio_make_input(struct nmk_gpio_chip *nmk_chip,
				  unsigned offset)
{
	writel(1 << offset, nmk_chip->addr + NMK_GPIO_DIRC);
}

static void __nmk_gpio_set_output(struct nmk_gpio_chip *nmk_chip,
				  unsigned offset, int val)
{
	if (val)
		writel(1 << offset, nmk_chip->addr + NMK_GPIO_DATS);
	else
		writel(1 << offset, nmk_chip->addr + NMK_GPIO_DATC);
}

static void __nmk_gpio_make_output(struct nmk_gpio_chip *nmk_chip,
				  unsigned offset, int val)
{
	writel(1 << offset, nmk_chip->addr + NMK_GPIO_DIRS);
	__nmk_gpio_set_output(nmk_chip, offset, val);
}

static void __nmk_gpio_set_mode_safe(struct nmk_gpio_chip *nmk_chip,
				     unsigned offset, int gpio_mode,
				     bool glitch)
{
	u32 rwimsc = readl(nmk_chip->addr + NMK_GPIO_RWIMSC);
	u32 fwimsc = readl(nmk_chip->addr + NMK_GPIO_FWIMSC);

	if (glitch && nmk_chip->set_ioforce) {
		u32 bit = BIT(offset);

		/* Prevent spurious wakeups */
		writel(rwimsc & ~bit, nmk_chip->addr + NMK_GPIO_RWIMSC);
		writel(fwimsc & ~bit, nmk_chip->addr + NMK_GPIO_FWIMSC);

		nmk_chip->set_ioforce(true);
	}

	__nmk_gpio_set_mode(nmk_chip, offset, gpio_mode);

	if (glitch && nmk_chip->set_ioforce) {
		nmk_chip->set_ioforce(false);

		writel(rwimsc, nmk_chip->addr + NMK_GPIO_RWIMSC);
		writel(fwimsc, nmk_chip->addr + NMK_GPIO_FWIMSC);
	}
}

static void __nmk_config_pin(struct nmk_gpio_chip *nmk_chip, unsigned offset,
			     pin_cfg_t cfg, bool sleep, unsigned int *slpmregs)
{
	static const char *afnames[] = {
		[NMK_GPIO_ALT_GPIO]	= "GPIO",
		[NMK_GPIO_ALT_A]	= "A",
		[NMK_GPIO_ALT_B]	= "B",
		[NMK_GPIO_ALT_C]	= "C"
	};
	static const char *pullnames[] = {
		[NMK_GPIO_PULL_NONE]	= "none",
		[NMK_GPIO_PULL_UP]	= "up",
		[NMK_GPIO_PULL_DOWN]	= "down",
		[3] /* illegal */	= "??"
	};
	static const char *slpmnames[] = {
		[NMK_GPIO_SLPM_INPUT]		= "input/wakeup",
		[NMK_GPIO_SLPM_NOCHANGE]	= "no-change/no-wakeup",
	};

	int pin = PIN_NUM(cfg);
	int pull = PIN_PULL(cfg);
	int af = PIN_ALT(cfg);
	int slpm = PIN_SLPM(cfg);
	int output = PIN_DIR(cfg);
	int val = PIN_VAL(cfg);
	bool glitch = af == NMK_GPIO_ALT_C;

	dev_dbg(nmk_chip->chip.dev, "pin %d [%#lx]: af %s, pull %s, slpm %s (%s%s)\n",
		pin, cfg, afnames[af], pullnames[pull], slpmnames[slpm],
		output ? "output " : "input",
		output ? (val ? "high" : "low") : "");

	if (sleep) {
		int slpm_pull = PIN_SLPM_PULL(cfg);
		int slpm_output = PIN_SLPM_DIR(cfg);
		int slpm_val = PIN_SLPM_VAL(cfg);

		af = NMK_GPIO_ALT_GPIO;

		/*
		 * The SLPM_* values are normal values + 1 to allow zero to
		 * mean "same as normal".
		 */
		if (slpm_pull)
			pull = slpm_pull - 1;
		if (slpm_output)
			output = slpm_output - 1;
		if (slpm_val)
			val = slpm_val - 1;

		dev_dbg(nmk_chip->chip.dev, "pin %d: sleep pull %s, dir %s, val %s\n",
			pin,
			slpm_pull ? pullnames[pull] : "same",
			slpm_output ? (output ? "output" : "input") : "same",
			slpm_val ? (val ? "high" : "low") : "same");
	}

	if (output)
		__nmk_gpio_make_output(nmk_chip, offset, val);
	else {
		__nmk_gpio_make_input(nmk_chip, offset);
		__nmk_gpio_set_pull(nmk_chip, offset, pull);
	}

	/*
	 * If we've backed up the SLPM registers (glitch workaround), modify
	 * the backups since they will be restored.
	 */
	if (slpmregs) {
		if (slpm == NMK_GPIO_SLPM_NOCHANGE)
			slpmregs[nmk_chip->bank] |= BIT(offset);
		else
			slpmregs[nmk_chip->bank] &= ~BIT(offset);
	} else
		__nmk_gpio_set_slpm(nmk_chip, offset, slpm);

	__nmk_gpio_set_mode_safe(nmk_chip, offset, af, glitch);
}

/*
 * Safe sequence used to switch IOs between GPIO and Alternate-C mode:
 *  - Save SLPM registers
 *  - Set SLPM=0 for the IOs you want to switch and others to 1
 *  - Configure the GPIO registers for the IOs that are being switched
 *  - Set IOFORCE=1
 *  - Modify the AFLSA/B registers for the IOs that are being switched
 *  - Set IOFORCE=0
 *  - Restore SLPM registers
 *  - Any spurious wake up event during switch sequence to be ignored and
 *    cleared
 */
static void nmk_gpio_glitch_slpm_init(unsigned int *slpm)
{
	int i;

	for (i = 0; i < NUM_BANKS; i++) {
		struct nmk_gpio_chip *chip = nmk_gpio_chips[i];
		unsigned int temp = slpm[i];

		if (!chip)
			break;

		clk_enable(chip->clk);

		slpm[i] = readl(chip->addr + NMK_GPIO_SLPC);
		writel(temp, chip->addr + NMK_GPIO_SLPC);
	}
}

static void nmk_gpio_glitch_slpm_restore(unsigned int *slpm)
{
	int i;

	for (i = 0; i < NUM_BANKS; i++) {
		struct nmk_gpio_chip *chip = nmk_gpio_chips[i];

		if (!chip)
			break;

		writel(slpm[i], chip->addr + NMK_GPIO_SLPC);

		clk_disable(chip->clk);
	}
}

static int __nmk_config_pins(pin_cfg_t *cfgs, int num, bool sleep)
{
	static unsigned int slpm[NUM_BANKS];
	unsigned long flags;
	bool glitch = false;
	int ret = 0;
	int i;

	for (i = 0; i < num; i++) {
		if (PIN_ALT(cfgs[i]) == NMK_GPIO_ALT_C) {
			glitch = true;
			break;
		}
	}

	spin_lock_irqsave(&nmk_gpio_slpm_lock, flags);

	if (glitch) {
		memset(slpm, 0xff, sizeof(slpm));

		for (i = 0; i < num; i++) {
			int pin = PIN_NUM(cfgs[i]);
			int offset = pin % NMK_GPIO_PER_CHIP;

			if (PIN_ALT(cfgs[i]) == NMK_GPIO_ALT_C)
				slpm[pin / NMK_GPIO_PER_CHIP] &= ~BIT(offset);
		}

		nmk_gpio_glitch_slpm_init(slpm);
	}

	for (i = 0; i < num; i++) {
		struct nmk_gpio_chip *nmk_chip;
		int pin = PIN_NUM(cfgs[i]);

		nmk_chip = irq_get_chip_data(NOMADIK_GPIO_TO_IRQ(pin));
		if (!nmk_chip) {
			ret = -EINVAL;
			break;
		}

		clk_enable(nmk_chip->clk);
		spin_lock(&nmk_chip->lock);
		__nmk_config_pin(nmk_chip, pin - nmk_chip->chip.base,
				 cfgs[i], sleep, glitch ? slpm : NULL);
		spin_unlock(&nmk_chip->lock);
		clk_disable(nmk_chip->clk);
	}

	if (glitch)
		nmk_gpio_glitch_slpm_restore(slpm);

	spin_unlock_irqrestore(&nmk_gpio_slpm_lock, flags);

	return ret;
}

/**
 * nmk_config_pin - configure a pin's mux attributes
 * @cfg: pin confguration
 *
 * Configures a pin's mode (alternate function or GPIO), its pull up status,
 * and its sleep mode based on the specified configuration.  The @cfg is
 * usually one of the SoC specific macros defined in mach/<soc>-pins.h.  These
 * are constructed using, and can be further enhanced with, the macros in
 * plat/pincfg.h.
 *
 * If a pin's mode is set to GPIO, it is configured as an input to avoid
 * side-effects.  The gpio can be manipulated later using standard GPIO API
 * calls.
 */
int nmk_config_pin(pin_cfg_t cfg, bool sleep)
{
	return __nmk_config_pins(&cfg, 1, sleep);
}
EXPORT_SYMBOL(nmk_config_pin);

/**
 * nmk_config_pins - configure several pins at once
 * @cfgs: array of pin configurations
 * @num: number of elments in the array
 *
 * Configures several pins using nmk_config_pin().  Refer to that function for
 * further information.
 */
int nmk_config_pins(pin_cfg_t *cfgs, int num)
{
	return __nmk_config_pins(cfgs, num, false);
}
EXPORT_SYMBOL(nmk_config_pins);

int nmk_config_pins_sleep(pin_cfg_t *cfgs, int num)
{
	return __nmk_config_pins(cfgs, num, true);
}
EXPORT_SYMBOL(nmk_config_pins_sleep);

/**
 * nmk_gpio_set_slpm() - configure the sleep mode of a pin
 * @gpio: pin number
 * @mode: NMK_GPIO_SLPM_INPUT or NMK_GPIO_SLPM_NOCHANGE,
 *
 * This register is actually in the pinmux layer, not the GPIO block itself.
 * The GPIO1B_SLPM register defines the GPIO mode when SLEEP/DEEP-SLEEP
 * mode is entered (i.e. when signal IOFORCE is HIGH by the platform code).
 * Each GPIO can be configured to be forced into GPIO mode when IOFORCE is
 * HIGH, overriding the normal setting defined by GPIO_AFSELx registers.
 * When IOFORCE returns LOW (by software, after SLEEP/DEEP-SLEEP exit),
 * the GPIOs return to the normal setting defined by GPIO_AFSELx registers.
 *
 * If @mode is NMK_GPIO_SLPM_INPUT, the corresponding GPIO is switched to GPIO
 * mode when signal IOFORCE is HIGH (i.e. when SLEEP/DEEP-SLEEP mode is
 * entered) regardless of the altfunction selected. Also wake-up detection is
 * ENABLED.
 *
 * If @mode is NMK_GPIO_SLPM_NOCHANGE, the corresponding GPIO remains
 * controlled by NMK_GPIO_DATC, NMK_GPIO_DATS, NMK_GPIO_DIR, NMK_GPIO_PDIS
 * (for altfunction GPIO) or respective on-chip peripherals (for other
 * altfuncs) when IOFORCE is HIGH. Also wake-up detection DISABLED.
 *
 * Note that enable_irq_wake() will automatically enable wakeup detection.
 */
int nmk_gpio_set_slpm(int gpio, enum nmk_gpio_slpm mode)
{
	struct nmk_gpio_chip *nmk_chip;
	unsigned long flags;

	nmk_chip = irq_get_chip_data(NOMADIK_GPIO_TO_IRQ(gpio));
	if (!nmk_chip)
		return -EINVAL;

	clk_enable(nmk_chip->clk);
	spin_lock_irqsave(&nmk_gpio_slpm_lock, flags);
	spin_lock(&nmk_chip->lock);

	__nmk_gpio_set_slpm(nmk_chip, gpio - nmk_chip->chip.base, mode);

	spin_unlock(&nmk_chip->lock);
	spin_unlock_irqrestore(&nmk_gpio_slpm_lock, flags);
	clk_disable(nmk_chip->clk);

	return 0;
}

/**
 * nmk_gpio_set_pull() - enable/disable pull up/down on a gpio
 * @gpio: pin number
 * @pull: one of NMK_GPIO_PULL_DOWN, NMK_GPIO_PULL_UP, and NMK_GPIO_PULL_NONE
 *
 * Enables/disables pull up/down on a specified pin.  This only takes effect if
 * the pin is configured as an input (either explicitly or by the alternate
 * function).
 *
 * NOTE: If enabling the pull up/down, the caller must ensure that the GPIO is
 * configured as an input.  Otherwise, due to the way the controller registers
 * work, this function will change the value output on the pin.
 */
int nmk_gpio_set_pull(int gpio, enum nmk_gpio_pull pull)
{
	struct nmk_gpio_chip *nmk_chip;
	unsigned long flags;

	nmk_chip = irq_get_chip_data(NOMADIK_GPIO_TO_IRQ(gpio));
	if (!nmk_chip)
		return -EINVAL;

	clk_enable(nmk_chip->clk);
	spin_lock_irqsave(&nmk_chip->lock, flags);
	__nmk_gpio_set_pull(nmk_chip, gpio - nmk_chip->chip.base, pull);
	spin_unlock_irqrestore(&nmk_chip->lock, flags);
	clk_disable(nmk_chip->clk);

	return 0;
}

/* Mode functions */
/**
 * nmk_gpio_set_mode() - set the mux mode of a gpio pin
 * @gpio: pin number
 * @gpio_mode: one of NMK_GPIO_ALT_GPIO, NMK_GPIO_ALT_A,
 *	       NMK_GPIO_ALT_B, and NMK_GPIO_ALT_C
 *
 * Sets the mode of the specified pin to one of the alternate functions or
 * plain GPIO.
 */
int nmk_gpio_set_mode(int gpio, int gpio_mode)
{
	struct nmk_gpio_chip *nmk_chip;
	unsigned long flags;

	nmk_chip = irq_get_chip_data(NOMADIK_GPIO_TO_IRQ(gpio));
	if (!nmk_chip)
		return -EINVAL;

	clk_enable(nmk_chip->clk);
	spin_lock_irqsave(&nmk_chip->lock, flags);
	__nmk_gpio_set_mode(nmk_chip, gpio - nmk_chip->chip.base, gpio_mode);
	spin_unlock_irqrestore(&nmk_chip->lock, flags);
	clk_disable(nmk_chip->clk);

	return 0;
}
EXPORT_SYMBOL(nmk_gpio_set_mode);

int nmk_gpio_get_mode(int gpio)
{
	struct nmk_gpio_chip *nmk_chip;
	u32 afunc, bfunc, bit;

	nmk_chip = irq_get_chip_data(NOMADIK_GPIO_TO_IRQ(gpio));
	if (!nmk_chip)
		return -EINVAL;

	bit = 1 << (gpio - nmk_chip->chip.base);

	clk_enable(nmk_chip->clk);

	afunc = readl(nmk_chip->addr + NMK_GPIO_AFSLA) & bit;
	bfunc = readl(nmk_chip->addr + NMK_GPIO_AFSLB) & bit;

	clk_disable(nmk_chip->clk);

	return (afunc ? NMK_GPIO_ALT_A : 0) | (bfunc ? NMK_GPIO_ALT_B : 0);
}
EXPORT_SYMBOL(nmk_gpio_get_mode);


/* IRQ functions */
static inline int nmk_gpio_get_bitmask(int gpio)
{
	return 1 << (gpio % 32);
}

static void nmk_gpio_irq_ack(struct irq_data *d)
{
	int gpio;
	struct nmk_gpio_chip *nmk_chip;

	gpio = NOMADIK_IRQ_TO_GPIO(d->irq);
	nmk_chip = irq_data_get_irq_chip_data(d);
	if (!nmk_chip)
		return;

	clk_enable(nmk_chip->clk);
	writel(nmk_gpio_get_bitmask(gpio), nmk_chip->addr + NMK_GPIO_IC);
=======
 * Copyright (C) 2011-2013 Linus Walleij <linus.walleij@linaro.org>
 */
#include <linux/cleanup.h>
#include <linux/clk.h>
#include <linux/gpio/driver.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/mod_devicetable.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/reset.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <linux/gpio/gpio-nomadik.h>

#ifndef CONFIG_PINCTRL_NOMADIK
static DEFINE_SPINLOCK(nmk_gpio_slpm_lock);
#endif

void __nmk_gpio_set_slpm(struct nmk_gpio_chip *nmk_chip, unsigned int offset,
			 enum nmk_gpio_slpm mode)
{
	u32 slpm;

	/* We should NOT have been called. */
	if (WARN_ON(nmk_chip->is_mobileye_soc))
		return;

	slpm = readl(nmk_chip->addr + NMK_GPIO_SLPC);
	if (mode == NMK_GPIO_SLPM_NOCHANGE)
		slpm |= BIT(offset);
	else
		slpm &= ~BIT(offset);
	writel(slpm, nmk_chip->addr + NMK_GPIO_SLPC);
}

static void __nmk_gpio_set_output(struct nmk_gpio_chip *nmk_chip,
				  unsigned int offset, int val)
{
	if (val)
		writel(BIT(offset), nmk_chip->addr + NMK_GPIO_DATS);
	else
		writel(BIT(offset), nmk_chip->addr + NMK_GPIO_DATC);
}

void __nmk_gpio_make_output(struct nmk_gpio_chip *nmk_chip,
			    unsigned int offset, int val)
{
	writel(BIT(offset), nmk_chip->addr + NMK_GPIO_DIRS);
	__nmk_gpio_set_output(nmk_chip, offset, val);
}

/* IRQ functions */

static void nmk_gpio_irq_ack(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(gc);

	clk_enable(nmk_chip->clk);
	writel(BIT(d->hwirq), nmk_chip->addr + NMK_GPIO_IC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clk_disable(nmk_chip->clk);
}

enum nmk_gpio_irq_type {
	NORMAL,
	WAKE,
};

static void __nmk_gpio_irq_modify(struct nmk_gpio_chip *nmk_chip,
<<<<<<< HEAD
				  int gpio, enum nmk_gpio_irq_type which,
				  bool enable)
{
	u32 rimsc = which == WAKE ? NMK_GPIO_RWIMSC : NMK_GPIO_RIMSC;
	u32 fimsc = which == WAKE ? NMK_GPIO_FWIMSC : NMK_GPIO_FIMSC;
	u32 bitmask = nmk_gpio_get_bitmask(gpio);
	u32 reg;

	/* we must individually set/clear the two edges */
	if (nmk_chip->edge_rising & bitmask) {
		reg = readl(nmk_chip->addr + rimsc);
		if (enable)
			reg |= bitmask;
		else
			reg &= ~bitmask;
		writel(reg, nmk_chip->addr + rimsc);
	}
	if (nmk_chip->edge_falling & bitmask) {
		reg = readl(nmk_chip->addr + fimsc);
		if (enable)
			reg |= bitmask;
		else
			reg &= ~bitmask;
		writel(reg, nmk_chip->addr + fimsc);
=======
				  int offset, enum nmk_gpio_irq_type which,
				  bool enable)
{
	u32 *rimscval;
	u32 *fimscval;
	u32 rimscreg;
	u32 fimscreg;

	if (which == NORMAL) {
		rimscreg = NMK_GPIO_RIMSC;
		fimscreg = NMK_GPIO_FIMSC;
		rimscval = &nmk_chip->rimsc;
		fimscval = &nmk_chip->fimsc;
	} else  {
		/* We should NOT have been called. */
		if (WARN_ON(nmk_chip->is_mobileye_soc))
			return;
		rimscreg = NMK_GPIO_RWIMSC;
		fimscreg = NMK_GPIO_FWIMSC;
		rimscval = &nmk_chip->rwimsc;
		fimscval = &nmk_chip->fwimsc;
	}

	/* we must individually set/clear the two edges */
	if (nmk_chip->edge_rising & BIT(offset)) {
		if (enable)
			*rimscval |= BIT(offset);
		else
			*rimscval &= ~BIT(offset);
		writel(*rimscval, nmk_chip->addr + rimscreg);
	}
	if (nmk_chip->edge_falling & BIT(offset)) {
		if (enable)
			*fimscval |= BIT(offset);
		else
			*fimscval &= ~BIT(offset);
		writel(*fimscval, nmk_chip->addr + fimscreg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void __nmk_gpio_set_wake(struct nmk_gpio_chip *nmk_chip,
<<<<<<< HEAD
				int gpio, bool on)
{
	if (nmk_chip->sleepmode) {
		__nmk_gpio_set_slpm(nmk_chip, gpio - nmk_chip->chip.base,
				    on ? NMK_GPIO_SLPM_WAKEUP_ENABLE
				    : NMK_GPIO_SLPM_WAKEUP_DISABLE);
	}

	__nmk_gpio_irq_modify(nmk_chip, gpio, WAKE, on);
}

static int nmk_gpio_irq_maskunmask(struct irq_data *d, bool enable)
{
	int gpio;
	struct nmk_gpio_chip *nmk_chip;
	unsigned long flags;
	u32 bitmask;

	gpio = NOMADIK_IRQ_TO_GPIO(d->irq);
	nmk_chip = irq_data_get_irq_chip_data(d);
	bitmask = nmk_gpio_get_bitmask(gpio);
	if (!nmk_chip)
		return -EINVAL;
=======
				int offset, bool on)
{
	/* We should NOT have been called. */
	if (WARN_ON(nmk_chip->is_mobileye_soc))
		return;

	/*
	 * Ensure WAKEUP_ENABLE is on.  No need to disable it if wakeup is
	 * disabled, since setting SLPM to 1 increases power consumption, and
	 * wakeup is anyhow controlled by the RIMSC and FIMSC registers.
	 */
	if (nmk_chip->sleepmode && on) {
		__nmk_gpio_set_slpm(nmk_chip, offset,
				    NMK_GPIO_SLPM_WAKEUP_ENABLE);
	}

	__nmk_gpio_irq_modify(nmk_chip, offset, WAKE, on);
}

static void nmk_gpio_irq_maskunmask(struct nmk_gpio_chip *nmk_chip,
				    struct irq_data *d, bool enable)
{
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clk_enable(nmk_chip->clk);
	spin_lock_irqsave(&nmk_gpio_slpm_lock, flags);
	spin_lock(&nmk_chip->lock);

<<<<<<< HEAD
	__nmk_gpio_irq_modify(nmk_chip, gpio, NORMAL, enable);

	if (!(nmk_chip->real_wake & bitmask))
		__nmk_gpio_set_wake(nmk_chip, gpio, enable);
=======
	__nmk_gpio_irq_modify(nmk_chip, d->hwirq, NORMAL, enable);

	if (!nmk_chip->is_mobileye_soc && !(nmk_chip->real_wake & BIT(d->hwirq)))
		__nmk_gpio_set_wake(nmk_chip, d->hwirq, enable);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock(&nmk_chip->lock);
	spin_unlock_irqrestore(&nmk_gpio_slpm_lock, flags);
	clk_disable(nmk_chip->clk);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void nmk_gpio_irq_mask(struct irq_data *d)
{
<<<<<<< HEAD
	nmk_gpio_irq_maskunmask(d, false);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(gc);

	nmk_gpio_irq_maskunmask(nmk_chip, d, false);
	gpiochip_disable_irq(gc, irqd_to_hwirq(d));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void nmk_gpio_irq_unmask(struct irq_data *d)
{
<<<<<<< HEAD
	nmk_gpio_irq_maskunmask(d, true);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(gc);

	gpiochip_enable_irq(gc, irqd_to_hwirq(d));
	nmk_gpio_irq_maskunmask(nmk_chip, d, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int nmk_gpio_irq_set_wake(struct irq_data *d, unsigned int on)
{
<<<<<<< HEAD
	struct nmk_gpio_chip *nmk_chip;
	unsigned long flags;
	u32 bitmask;
	int gpio;

	gpio = NOMADIK_IRQ_TO_GPIO(d->irq);
	nmk_chip = irq_data_get_irq_chip_data(d);
	if (!nmk_chip)
		return -EINVAL;
	bitmask = nmk_gpio_get_bitmask(gpio);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(gc);
	unsigned long flags;

	/* Handler is registered in all cases. */
	if (nmk_chip->is_mobileye_soc)
		return -ENXIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clk_enable(nmk_chip->clk);
	spin_lock_irqsave(&nmk_gpio_slpm_lock, flags);
	spin_lock(&nmk_chip->lock);

	if (irqd_irq_disabled(d))
<<<<<<< HEAD
		__nmk_gpio_set_wake(nmk_chip, gpio, on);

	if (on)
		nmk_chip->real_wake |= bitmask;
	else
		nmk_chip->real_wake &= ~bitmask;
=======
		__nmk_gpio_set_wake(nmk_chip, d->hwirq, on);

	if (on)
		nmk_chip->real_wake |= BIT(d->hwirq);
	else
		nmk_chip->real_wake &= ~BIT(d->hwirq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock(&nmk_chip->lock);
	spin_unlock_irqrestore(&nmk_gpio_slpm_lock, flags);
	clk_disable(nmk_chip->clk);

	return 0;
}

static int nmk_gpio_irq_set_type(struct irq_data *d, unsigned int type)
{
<<<<<<< HEAD
	bool enabled = !irqd_irq_disabled(d);
	bool wake = irqd_is_wakeup_set(d);
	int gpio;
	struct nmk_gpio_chip *nmk_chip;
	unsigned long flags;
	u32 bitmask;

	gpio = NOMADIK_IRQ_TO_GPIO(d->irq);
	nmk_chip = irq_data_get_irq_chip_data(d);
	bitmask = nmk_gpio_get_bitmask(gpio);
	if (!nmk_chip)
		return -EINVAL;
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(gc);
	bool enabled = !irqd_irq_disabled(d);
	bool wake = irqd_is_wakeup_set(d);
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (type & IRQ_TYPE_LEVEL_HIGH)
		return -EINVAL;
	if (type & IRQ_TYPE_LEVEL_LOW)
		return -EINVAL;

	clk_enable(nmk_chip->clk);
	spin_lock_irqsave(&nmk_chip->lock, flags);

	if (enabled)
<<<<<<< HEAD
		__nmk_gpio_irq_modify(nmk_chip, gpio, NORMAL, false);

	if (enabled || wake)
		__nmk_gpio_irq_modify(nmk_chip, gpio, WAKE, false);

	nmk_chip->edge_rising &= ~bitmask;
	if (type & IRQ_TYPE_EDGE_RISING)
		nmk_chip->edge_rising |= bitmask;

	nmk_chip->edge_falling &= ~bitmask;
	if (type & IRQ_TYPE_EDGE_FALLING)
		nmk_chip->edge_falling |= bitmask;

	if (enabled)
		__nmk_gpio_irq_modify(nmk_chip, gpio, NORMAL, true);

	if (enabled || wake)
		__nmk_gpio_irq_modify(nmk_chip, gpio, WAKE, true);
=======
		__nmk_gpio_irq_modify(nmk_chip, d->hwirq, NORMAL, false);

	if (!nmk_chip->is_mobileye_soc && (enabled || wake))
		__nmk_gpio_irq_modify(nmk_chip, d->hwirq, WAKE, false);

	nmk_chip->edge_rising &= ~BIT(d->hwirq);
	if (type & IRQ_TYPE_EDGE_RISING)
		nmk_chip->edge_rising |= BIT(d->hwirq);

	nmk_chip->edge_falling &= ~BIT(d->hwirq);
	if (type & IRQ_TYPE_EDGE_FALLING)
		nmk_chip->edge_falling |= BIT(d->hwirq);

	if (enabled)
		__nmk_gpio_irq_modify(nmk_chip, d->hwirq, NORMAL, true);

	if (!nmk_chip->is_mobileye_soc && (enabled || wake))
		__nmk_gpio_irq_modify(nmk_chip, d->hwirq, WAKE, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock_irqrestore(&nmk_chip->lock, flags);
	clk_disable(nmk_chip->clk);

	return 0;
}

static unsigned int nmk_gpio_irq_startup(struct irq_data *d)
{
<<<<<<< HEAD
	struct nmk_gpio_chip *nmk_chip = irq_data_get_irq_chip_data(d);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(gc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clk_enable(nmk_chip->clk);
	nmk_gpio_irq_unmask(d);
	return 0;
}

static void nmk_gpio_irq_shutdown(struct irq_data *d)
{
<<<<<<< HEAD
	struct nmk_gpio_chip *nmk_chip = irq_data_get_irq_chip_data(d);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(gc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	nmk_gpio_irq_mask(d);
	clk_disable(nmk_chip->clk);
}

<<<<<<< HEAD
static struct irq_chip nmk_gpio_irq_chip = {
	.name		= "Nomadik-GPIO",
	.irq_ack	= nmk_gpio_irq_ack,
	.irq_mask	= nmk_gpio_irq_mask,
	.irq_unmask	= nmk_gpio_irq_unmask,
	.irq_set_type	= nmk_gpio_irq_set_type,
	.irq_set_wake	= nmk_gpio_irq_set_wake,
	.irq_startup	= nmk_gpio_irq_startup,
	.irq_shutdown	= nmk_gpio_irq_shutdown,
};

static void __nmk_gpio_irq_handler(unsigned int irq, struct irq_desc *desc,
				   u32 status)
{
	struct nmk_gpio_chip *nmk_chip;
	struct irq_chip *host_chip = irq_get_chip(irq);
	unsigned int first_irq;

	chained_irq_enter(host_chip, desc);

	nmk_chip = irq_get_handler_data(irq);
	first_irq = NOMADIK_GPIO_TO_IRQ(nmk_chip->chip.base);
	while (status) {
		int bit = __ffs(status);

		generic_handle_irq(first_irq + bit);
		status &= ~BIT(bit);
	}

	chained_irq_exit(host_chip, desc);
}

static void nmk_gpio_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	struct nmk_gpio_chip *nmk_chip = irq_get_handler_data(irq);
	u32 status;

	clk_enable(nmk_chip->clk);
	status = readl(nmk_chip->addr + NMK_GPIO_IS);
	clk_disable(nmk_chip->clk);

	__nmk_gpio_irq_handler(irq, desc, status);
}

static void nmk_gpio_secondary_irq_handler(unsigned int irq,
					   struct irq_desc *desc)
{
	struct nmk_gpio_chip *nmk_chip = irq_get_handler_data(irq);
	u32 status = nmk_chip->get_secondary_status(nmk_chip->bank);

	__nmk_gpio_irq_handler(irq, desc, status);
}

static int nmk_gpio_init_irq(struct nmk_gpio_chip *nmk_chip)
{
	unsigned int first_irq;
	int i;

	first_irq = NOMADIK_GPIO_TO_IRQ(nmk_chip->chip.base);
	for (i = first_irq; i < first_irq + nmk_chip->chip.ngpio; i++) {
		irq_set_chip_and_handler(i, &nmk_gpio_irq_chip,
					 handle_edge_irq);
		set_irq_flags(i, IRQF_VALID);
		irq_set_chip_data(i, nmk_chip);
		irq_set_irq_type(i, IRQ_TYPE_EDGE_FALLING);
	}

	irq_set_chained_handler(nmk_chip->parent_irq, nmk_gpio_irq_handler);
	irq_set_handler_data(nmk_chip->parent_irq, nmk_chip);

	if (nmk_chip->secondary_parent_irq >= 0) {
		irq_set_chained_handler(nmk_chip->secondary_parent_irq,
					nmk_gpio_secondary_irq_handler);
		irq_set_handler_data(nmk_chip->secondary_parent_irq, nmk_chip);
	}

	return 0;
}

/* I/O Functions */
static int nmk_gpio_make_input(struct gpio_chip *chip, unsigned offset)
{
	struct nmk_gpio_chip *nmk_chip =
		container_of(chip, struct nmk_gpio_chip, chip);

	clk_enable(nmk_chip->clk);

	writel(1 << offset, nmk_chip->addr + NMK_GPIO_DIRC);
=======
static irqreturn_t nmk_gpio_irq_handler(int irq, void *dev_id)
{
	struct nmk_gpio_chip *nmk_chip = dev_id;
	struct gpio_chip *chip = &nmk_chip->chip;
	unsigned long mask = GENMASK(chip->ngpio - 1, 0);
	unsigned long status;
	int bit;

	clk_enable(nmk_chip->clk);

	status = readl(nmk_chip->addr + NMK_GPIO_IS);

	/* Ensure we cannot leave pending bits; this should never occur. */
	if (unlikely(status & ~mask))
		writel(status & ~mask, nmk_chip->addr + NMK_GPIO_IC);

	clk_disable(nmk_chip->clk);

	for_each_set_bit(bit, &status, chip->ngpio)
		generic_handle_domain_irq_safe(chip->irq.domain, bit);

	return IRQ_RETVAL((status & mask) != 0);
}

/* I/O Functions */

static int nmk_gpio_get_dir(struct gpio_chip *chip, unsigned int offset)
{
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(chip);
	int dir;

	clk_enable(nmk_chip->clk);

	dir = readl(nmk_chip->addr + NMK_GPIO_DIR) & BIT(offset);

	clk_disable(nmk_chip->clk);

	if (dir)
		return GPIO_LINE_DIRECTION_OUT;

	return GPIO_LINE_DIRECTION_IN;
}

static int nmk_gpio_make_input(struct gpio_chip *chip, unsigned int offset)
{
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(chip);

	clk_enable(nmk_chip->clk);

	writel(BIT(offset), nmk_chip->addr + NMK_GPIO_DIRC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clk_disable(nmk_chip->clk);

	return 0;
}

<<<<<<< HEAD
static int nmk_gpio_get_input(struct gpio_chip *chip, unsigned offset)
{
	struct nmk_gpio_chip *nmk_chip =
		container_of(chip, struct nmk_gpio_chip, chip);
	u32 bit = 1 << offset;
=======
static int nmk_gpio_get_input(struct gpio_chip *chip, unsigned int offset)
{
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(chip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int value;

	clk_enable(nmk_chip->clk);

<<<<<<< HEAD
	value = (readl(nmk_chip->addr + NMK_GPIO_DAT) & bit) != 0;
=======
	value = !!(readl(nmk_chip->addr + NMK_GPIO_DAT) & BIT(offset));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clk_disable(nmk_chip->clk);

	return value;
}

<<<<<<< HEAD
static void nmk_gpio_set_output(struct gpio_chip *chip, unsigned offset,
				int val)
{
	struct nmk_gpio_chip *nmk_chip =
		container_of(chip, struct nmk_gpio_chip, chip);
=======
static void nmk_gpio_set_output(struct gpio_chip *chip, unsigned int offset,
				int val)
{
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(chip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clk_enable(nmk_chip->clk);

	__nmk_gpio_set_output(nmk_chip, offset, val);

	clk_disable(nmk_chip->clk);
}

<<<<<<< HEAD
static int nmk_gpio_make_output(struct gpio_chip *chip, unsigned offset,
				int val)
{
	struct nmk_gpio_chip *nmk_chip =
		container_of(chip, struct nmk_gpio_chip, chip);
=======
static int nmk_gpio_make_output(struct gpio_chip *chip, unsigned int offset,
				int val)
{
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(chip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clk_enable(nmk_chip->clk);

	__nmk_gpio_make_output(nmk_chip, offset, val);

	clk_disable(nmk_chip->clk);

	return 0;
}

<<<<<<< HEAD
static int nmk_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct nmk_gpio_chip *nmk_chip =
		container_of(chip, struct nmk_gpio_chip, chip);

	return NOMADIK_GPIO_TO_IRQ(nmk_chip->chip.base) + offset;
}

#ifdef CONFIG_DEBUG_FS

#include <linux/seq_file.h>

static void nmk_gpio_dbg_show(struct seq_file *s, struct gpio_chip *chip)
{
	int mode;
	unsigned		i;
	unsigned		gpio = chip->base;
	int			is_out;
	struct nmk_gpio_chip *nmk_chip =
		container_of(chip, struct nmk_gpio_chip, chip);
	const char *modes[] = {
=======
#ifdef CONFIG_DEBUG_FS

static int nmk_gpio_get_mode(struct nmk_gpio_chip *nmk_chip, int offset)
{
	u32 afunc, bfunc;

	/* We don't support modes. */
	if (nmk_chip->is_mobileye_soc)
		return NMK_GPIO_ALT_GPIO;

	clk_enable(nmk_chip->clk);

	afunc = readl(nmk_chip->addr + NMK_GPIO_AFSLA) & BIT(offset);
	bfunc = readl(nmk_chip->addr + NMK_GPIO_AFSLB) & BIT(offset);

	clk_disable(nmk_chip->clk);

	return (afunc ? NMK_GPIO_ALT_A : 0) | (bfunc ? NMK_GPIO_ALT_B : 0);
}

void nmk_gpio_dbg_show_one(struct seq_file *s, struct pinctrl_dev *pctldev,
			   struct gpio_chip *chip, unsigned int offset,
			   unsigned int gpio)
{
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(chip);
	int mode;
	bool is_out;
	bool data_out;
	bool pull;
	static const char * const modes[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		[NMK_GPIO_ALT_GPIO]	= "gpio",
		[NMK_GPIO_ALT_A]	= "altA",
		[NMK_GPIO_ALT_B]	= "altB",
		[NMK_GPIO_ALT_C]	= "altC",
<<<<<<< HEAD
	};

	clk_enable(nmk_chip->clk);

	for (i = 0; i < chip->ngpio; i++, gpio++) {
		const char *label = gpiochip_is_requested(chip, i);
		bool pull;
		u32 bit = 1 << i;

		is_out = readl(nmk_chip->addr + NMK_GPIO_DIR) & bit;
		pull = !(readl(nmk_chip->addr + NMK_GPIO_PDIS) & bit);
		mode = nmk_gpio_get_mode(gpio);
		seq_printf(s, " gpio-%-3d (%-20.20s) %s %s %s %s",
			gpio, label ?: "(none)",
			is_out ? "out" : "in ",
			chip->get
				? (chip->get(chip, i) ? "hi" : "lo")
				: "?  ",
			(mode < 0) ? "unknown" : modes[mode],
			pull ? "pull" : "none");

		if (label && !is_out) {
			int		irq = gpio_to_irq(gpio);
			struct irq_desc	*desc = irq_to_desc(irq);

			/* This races with request_irq(), set_irq_type(),
			 * and set_irq_wake() ... but those are "rare".
			 */
			if (irq >= 0 && desc->action) {
				char *trigger;
				u32 bitmask = nmk_gpio_get_bitmask(gpio);

				if (nmk_chip->edge_rising & bitmask)
					trigger = "edge-rising";
				else if (nmk_chip->edge_falling & bitmask)
					trigger = "edge-falling";
				else
					trigger = "edge-undefined";

				seq_printf(s, " irq-%d %s%s",
					irq, trigger,
					irqd_is_wakeup_set(&desc->irq_data)
						? " wakeup" : "");
			}
		}

		seq_printf(s, "\n");
	}

	clk_disable(nmk_chip->clk);
}

#else
#define nmk_gpio_dbg_show	NULL
#endif

/* This structure is replicated for each GPIO block allocated at probe time */
static struct gpio_chip nmk_gpio_template = {
	.direction_input	= nmk_gpio_make_input,
	.get			= nmk_gpio_get_input,
	.direction_output	= nmk_gpio_make_output,
	.set			= nmk_gpio_set_output,
	.to_irq			= nmk_gpio_to_irq,
	.dbg_show		= nmk_gpio_dbg_show,
	.can_sleep		= 0,
};

void nmk_gpio_clocks_enable(void)
{
	int i;

	for (i = 0; i < NUM_BANKS; i++) {
		struct nmk_gpio_chip *chip = nmk_gpio_chips[i];

		if (!chip)
			continue;

		clk_enable(chip->clk);
	}
}

void nmk_gpio_clocks_disable(void)
{
	int i;

	for (i = 0; i < NUM_BANKS; i++) {
		struct nmk_gpio_chip *chip = nmk_gpio_chips[i];

		if (!chip)
			continue;

		clk_disable(chip->clk);
	}
}

/*
 * Called from the suspend/resume path to only keep the real wakeup interrupts
 * (those that have had set_irq_wake() called on them) as wakeup interrupts,
 * and not the rest of the interrupts which we needed to have as wakeups for
 * cpuidle.
 *
 * PM ops are not used since this needs to be done at the end, after all the
 * other drivers are done with their suspend callbacks.
 */
void nmk_gpio_wakeups_suspend(void)
{
	int i;

	for (i = 0; i < NUM_BANKS; i++) {
		struct nmk_gpio_chip *chip = nmk_gpio_chips[i];

		if (!chip)
			break;

		clk_enable(chip->clk);

		chip->rwimsc = readl(chip->addr + NMK_GPIO_RWIMSC);
		chip->fwimsc = readl(chip->addr + NMK_GPIO_FWIMSC);

		writel(chip->rwimsc & chip->real_wake,
		       chip->addr + NMK_GPIO_RWIMSC);
		writel(chip->fwimsc & chip->real_wake,
		       chip->addr + NMK_GPIO_FWIMSC);

		if (chip->sleepmode) {
			chip->slpm = readl(chip->addr + NMK_GPIO_SLPC);

			/* 0 -> wakeup enable */
			writel(~chip->real_wake, chip->addr + NMK_GPIO_SLPC);
		}

		clk_disable(chip->clk);
	}
}

void nmk_gpio_wakeups_resume(void)
{
	int i;

	for (i = 0; i < NUM_BANKS; i++) {
		struct nmk_gpio_chip *chip = nmk_gpio_chips[i];

		if (!chip)
			break;

		clk_enable(chip->clk);

		writel(chip->rwimsc, chip->addr + NMK_GPIO_RWIMSC);
		writel(chip->fwimsc, chip->addr + NMK_GPIO_FWIMSC);

		if (chip->sleepmode)
			writel(chip->slpm, chip->addr + NMK_GPIO_SLPC);

		clk_disable(chip->clk);
	}
}

/*
 * Read the pull up/pull down status.
 * A bit set in 'pull_up' means that pull up
 * is selected if pull is enabled in PDIS register.
 * Note: only pull up/down set via this driver can
 * be detected due to HW limitations.
 */
void nmk_gpio_read_pull(int gpio_bank, u32 *pull_up)
{
	if (gpio_bank < NUM_BANKS) {
		struct nmk_gpio_chip *chip = nmk_gpio_chips[gpio_bank];

		if (!chip)
			return;

		*pull_up = chip->pull_up;
	}
}

static int __devinit nmk_gpio_probe(struct platform_device *dev)
{
	struct nmk_gpio_platform_data *pdata = dev->dev.platform_data;
	struct nmk_gpio_chip *nmk_chip;
	struct gpio_chip *chip;
	struct resource *res;
	struct clk *clk;
	int secondary_irq;
	int irq;
	int ret;

	if (!pdata)
		return -ENODEV;

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!res) {
		ret = -ENOENT;
		goto out;
	}

	irq = platform_get_irq(dev, 0);
	if (irq < 0) {
		ret = irq;
		goto out;
	}

	secondary_irq = platform_get_irq(dev, 1);
	if (secondary_irq >= 0 && !pdata->get_secondary_status) {
		ret = -EINVAL;
		goto out;
	}

	if (request_mem_region(res->start, resource_size(res),
			       dev_name(&dev->dev)) == NULL) {
		ret = -EBUSY;
		goto out;
	}

	clk = clk_get(&dev->dev, NULL);
	if (IS_ERR(clk)) {
		ret = PTR_ERR(clk);
		goto out_release;
	}

	nmk_chip = kzalloc(sizeof(*nmk_chip), GFP_KERNEL);
	if (!nmk_chip) {
		ret = -ENOMEM;
		goto out_clk;
	}
=======
		[NMK_GPIO_ALT_C + 1]	= "altC1",
		[NMK_GPIO_ALT_C + 2]	= "altC2",
		[NMK_GPIO_ALT_C + 3]	= "altC3",
		[NMK_GPIO_ALT_C + 4]	= "altC4",
	};

	char *label = gpiochip_dup_line_label(chip, offset);
	if (IS_ERR(label))
		return;

	clk_enable(nmk_chip->clk);
	is_out = !!(readl(nmk_chip->addr + NMK_GPIO_DIR) & BIT(offset));
	pull = !(readl(nmk_chip->addr + NMK_GPIO_PDIS) & BIT(offset));
	data_out = !!(readl(nmk_chip->addr + NMK_GPIO_DAT) & BIT(offset));
	mode = nmk_gpio_get_mode(nmk_chip, offset);
#ifdef CONFIG_PINCTRL_NOMADIK
	if (mode == NMK_GPIO_ALT_C && pctldev)
		mode = nmk_prcm_gpiocr_get_mode(pctldev, gpio);
#endif

	if (is_out) {
		seq_printf(s, " gpio-%-3d (%-20.20s) out %s           %s",
			   gpio,
			   label ?: "(none)",
			   data_out ? "hi" : "lo",
			   (mode < 0) ? "unknown" : modes[mode]);
	} else {
		int irq = chip->to_irq(chip, offset);
		const int pullidx = pull ? 1 : 0;
		int val;
		static const char * const pulls[] = {
			"none        ",
			"pull enabled",
		};

		seq_printf(s, " gpio-%-3d (%-20.20s) in  %s %s",
			   gpio,
			   label ?: "(none)",
			   pulls[pullidx],
			   (mode < 0) ? "unknown" : modes[mode]);

		val = nmk_gpio_get_input(chip, offset);
		seq_printf(s, " VAL %d", val);

		/*
		 * This races with request_irq(), set_irq_type(),
		 * and set_irq_wake() ... but those are "rare".
		 */
		if (irq > 0 && irq_has_action(irq)) {
			char *trigger;
			bool wake;

			if (nmk_chip->edge_rising & BIT(offset))
				trigger = "edge-rising";
			else if (nmk_chip->edge_falling & BIT(offset))
				trigger = "edge-falling";
			else
				trigger = "edge-undefined";

			wake = !!(nmk_chip->real_wake & BIT(offset));

			seq_printf(s, " irq-%d %s%s",
				   irq, trigger, wake ? " wakeup" : "");
		}
	}
	clk_disable(nmk_chip->clk);
}

static void nmk_gpio_dbg_show(struct seq_file *s, struct gpio_chip *chip)
{
	unsigned int i, gpio = chip->base;

	for (i = 0; i < chip->ngpio; i++, gpio++) {
		nmk_gpio_dbg_show_one(s, NULL, chip, i, gpio);
		seq_puts(s, "\n");
	}
}

#else

#define nmk_gpio_dbg_show	NULL

#endif

/*
 * We will allocate memory for the state container using devm* allocators
 * binding to the first device reaching this point, it doesn't matter if
 * it is the pin controller or GPIO driver. However we need to use the right
 * platform device when looking up resources so pay attention to pdev.
 */
struct nmk_gpio_chip *nmk_gpio_populate_chip(struct fwnode_handle *fwnode,
					     struct platform_device *pdev)
{
	struct nmk_gpio_chip *nmk_chip;
	struct platform_device *gpio_pdev;
	struct device *dev = &pdev->dev;
	struct reset_control *reset;
	struct device *gpio_dev;
	struct gpio_chip *chip;
	struct resource *res;
	struct clk *clk;
	void __iomem *base;
	u32 id, ngpio;
	int ret;

	gpio_dev = bus_find_device_by_fwnode(&platform_bus_type, fwnode);
	if (!gpio_dev) {
		dev_err(dev, "populate \"%pfwP\": device not found\n", fwnode);
		return ERR_PTR(-ENODEV);
	}
	gpio_pdev = to_platform_device(gpio_dev);

	if (device_property_read_u32(gpio_dev, "gpio-bank", &id)) {
		dev_err(dev, "populate: gpio-bank property not found\n");
		platform_device_put(gpio_pdev);
		return ERR_PTR(-EINVAL);
	}

#ifdef CONFIG_PINCTRL_NOMADIK
	if (id >= ARRAY_SIZE(nmk_gpio_chips)) {
		dev_err(dev, "populate: invalid id: %u\n", id);
		platform_device_put(gpio_pdev);
		return ERR_PTR(-EINVAL);
	}
	/* Already populated? */
	nmk_chip = nmk_gpio_chips[id];
	if (nmk_chip) {
		platform_device_put(gpio_pdev);
		return nmk_chip;
	}
#endif

	nmk_chip = devm_kzalloc(dev, sizeof(*nmk_chip), GFP_KERNEL);
	if (!nmk_chip) {
		platform_device_put(gpio_pdev);
		return ERR_PTR(-ENOMEM);
	}

	if (device_property_read_u32(gpio_dev, "ngpios", &ngpio)) {
		ngpio = NMK_GPIO_PER_CHIP;
		dev_dbg(dev, "populate: using default ngpio (%u)\n", ngpio);
	}

	nmk_chip->is_mobileye_soc = device_is_compatible(gpio_dev,
							 "mobileye,eyeq5-gpio");
	nmk_chip->bank = id;
	chip = &nmk_chip->chip;
	chip->base = -1;
	chip->ngpio = ngpio;
	chip->label = dev_name(gpio_dev);
	chip->parent = gpio_dev;

	/* NOTE: different devices! No devm_platform_ioremap_resource() here! */
	res = platform_get_resource(gpio_pdev, IORESOURCE_MEM, 0);
	base = devm_ioremap_resource(dev, res);
	if (IS_ERR(base)) {
		platform_device_put(gpio_pdev);
		return ERR_CAST(base);
	}
	nmk_chip->addr = base;

	/* NOTE: do not use devm_ here! */
	clk = clk_get_optional(gpio_dev, NULL);
	if (IS_ERR(clk)) {
		platform_device_put(gpio_pdev);
		return ERR_CAST(clk);
	}
	clk_prepare(clk);
	nmk_chip->clk = clk;

	/* NOTE: do not use devm_ here! */
	reset = reset_control_get_optional_shared(gpio_dev, NULL);
	if (IS_ERR(reset)) {
		clk_unprepare(clk);
		clk_put(clk);
		platform_device_put(gpio_pdev);
		dev_err(dev, "failed getting reset control: %pe\n",
			reset);
		return ERR_CAST(reset);
	}

	/*
	 * Reset might be shared and asserts/deasserts calls are unbalanced. We
	 * only support sharing this reset with other gpio-nomadik devices that
	 * use this reset to ensure deassertion at probe.
	 */
	ret = reset_control_deassert(reset);
	if (ret) {
		reset_control_put(reset);
		clk_unprepare(clk);
		clk_put(clk);
		platform_device_put(gpio_pdev);
		dev_err(dev, "failed reset deassert: %d\n", ret);
		return ERR_PTR(ret);
	}

#ifdef CONFIG_PINCTRL_NOMADIK
	nmk_gpio_chips[id] = nmk_chip;
#endif
	return nmk_chip;
}

static void nmk_gpio_irq_print_chip(struct irq_data *d, struct seq_file *p)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct nmk_gpio_chip *nmk_chip = gpiochip_get_data(gc);

	seq_printf(p, "nmk%u-%u-%u", nmk_chip->bank,
		   gc->base, gc->base + gc->ngpio - 1);
}

static const struct irq_chip nmk_irq_chip = {
	.irq_ack = nmk_gpio_irq_ack,
	.irq_mask = nmk_gpio_irq_mask,
	.irq_unmask = nmk_gpio_irq_unmask,
	.irq_set_type = nmk_gpio_irq_set_type,
	.irq_set_wake = nmk_gpio_irq_set_wake,
	.irq_startup = nmk_gpio_irq_startup,
	.irq_shutdown = nmk_gpio_irq_shutdown,
	.irq_print_chip = nmk_gpio_irq_print_chip,
	.flags = IRQCHIP_MASK_ON_SUSPEND | IRQCHIP_IMMUTABLE,
	GPIOCHIP_IRQ_RESOURCE_HELPERS,
};

static int nmk_gpio_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct nmk_gpio_chip *nmk_chip;
	struct gpio_irq_chip *girq;
	bool supports_sleepmode;
	struct gpio_chip *chip;
	int irq;
	int ret;

	nmk_chip = nmk_gpio_populate_chip(dev_fwnode(dev), pdev);
	if (IS_ERR(nmk_chip)) {
		dev_err(dev, "could not populate nmk chip struct\n");
		return PTR_ERR(nmk_chip);
	}

	supports_sleepmode =
		device_property_read_bool(dev, "st,supports-sleepmode");

	/* Correct platform device ID */
	pdev->id = nmk_chip->bank;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * The virt address in nmk_chip->addr is in the nomadik register space,
	 * so we can simply convert the resource address, without remapping
	 */
<<<<<<< HEAD
	nmk_chip->bank = dev->id;
	nmk_chip->clk = clk;
	nmk_chip->addr = io_p2v(res->start);
	nmk_chip->chip = nmk_gpio_template;
	nmk_chip->parent_irq = irq;
	nmk_chip->secondary_parent_irq = secondary_irq;
	nmk_chip->get_secondary_status = pdata->get_secondary_status;
	nmk_chip->set_ioforce = pdata->set_ioforce;
	nmk_chip->sleepmode = pdata->supports_sleepmode;
	spin_lock_init(&nmk_chip->lock);

	chip = &nmk_chip->chip;
	chip->base = pdata->first_gpio;
	chip->ngpio = pdata->num_gpio;
	chip->label = pdata->name ?: dev_name(&dev->dev);
	chip->dev = &dev->dev;
	chip->owner = THIS_MODULE;

	ret = gpiochip_add(&nmk_chip->chip);
	if (ret)
		goto out_free;

	BUG_ON(nmk_chip->bank >= ARRAY_SIZE(nmk_gpio_chips));

	nmk_gpio_chips[nmk_chip->bank] = nmk_chip;
	platform_set_drvdata(dev, nmk_chip);

	nmk_gpio_init_irq(nmk_chip);

	dev_info(&dev->dev, "at address %p\n",
		 nmk_chip->addr);
	return 0;

out_free:
	kfree(nmk_chip);
out_clk:
	clk_disable(clk);
	clk_put(clk);
out_release:
	release_mem_region(res->start, resource_size(res));
out:
	dev_err(&dev->dev, "Failure %i for GPIO %i-%i\n", ret,
		  pdata->first_gpio, pdata->first_gpio+31);
	return ret;
}

static struct platform_driver nmk_gpio_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "gpio",
=======
	nmk_chip->sleepmode = supports_sleepmode;
	spin_lock_init(&nmk_chip->lock);

	chip = &nmk_chip->chip;
	chip->parent = dev;
	chip->request = gpiochip_generic_request;
	chip->free = gpiochip_generic_free;
	chip->get_direction = nmk_gpio_get_dir;
	chip->direction_input = nmk_gpio_make_input;
	chip->get = nmk_gpio_get_input;
	chip->direction_output = nmk_gpio_make_output;
	chip->set = nmk_gpio_set_output;
	chip->dbg_show = nmk_gpio_dbg_show;
	chip->can_sleep = false;
	chip->owner = THIS_MODULE;

	girq = &chip->irq;
	gpio_irq_chip_set_chip(girq, &nmk_irq_chip);
	girq->parent_handler = NULL;
	girq->num_parents = 0;
	girq->parents = NULL;
	girq->default_type = IRQ_TYPE_NONE;
	girq->handler = handle_edge_irq;

	ret = devm_request_irq(dev, irq, nmk_gpio_irq_handler, IRQF_SHARED,
			       dev_name(dev), nmk_chip);
	if (ret) {
		dev_err(dev, "failed requesting IRQ\n");
		return ret;
	}

	if (!nmk_chip->is_mobileye_soc) {
		clk_enable(nmk_chip->clk);
		nmk_chip->lowemi = readl_relaxed(nmk_chip->addr + NMK_GPIO_LOWEMI);
		clk_disable(nmk_chip->clk);
	}

	ret = gpiochip_add_data(chip, nmk_chip);
	if (ret)
		return ret;

	platform_set_drvdata(pdev, nmk_chip);

	dev_info(dev, "chip registered\n");

	return 0;
}

static const struct of_device_id nmk_gpio_match[] = {
	{ .compatible = "st,nomadik-gpio", },
	{ .compatible = "mobileye,eyeq5-gpio", },
	{}
};

static struct platform_driver nmk_gpio_driver = {
	.driver = {
		.name = "nomadik-gpio",
		.of_match_table = nmk_gpio_match,
		.suppress_bind_attrs = true,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	.probe = nmk_gpio_probe,
};

static int __init nmk_gpio_init(void)
{
	return platform_driver_register(&nmk_gpio_driver);
}
<<<<<<< HEAD

core_initcall(nmk_gpio_init);

MODULE_AUTHOR("Prafulla WADASKAR and Alessandro Rubini");
MODULE_DESCRIPTION("Nomadik GPIO Driver");
MODULE_LICENSE("GPL");
=======
subsys_initcall(nmk_gpio_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
