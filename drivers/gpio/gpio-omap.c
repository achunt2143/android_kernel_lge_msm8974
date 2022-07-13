<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Support functions for OMAP GPIO
 *
 * Copyright (C) 2003-2005 Nokia Corporation
 * Written by Juha Yrjölä <juha.yrjola@nokia.com>
 *
 * Copyright (C) 2009 Texas Instruments
 * Added OMAP4 support - Santosh Shilimkar <santosh.shilimkar@ti.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
=======
#include <linux/seq_file.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/syscore_ops.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
<<<<<<< HEAD
=======
#include <linux/cpu_pm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/device.h>
#include <linux/pm_runtime.h>
#include <linux/pm.h>
#include <linux/of.h>
<<<<<<< HEAD
#include <linux/of_device.h>
#include <linux/irqdomain.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <mach/irqs.h>
#include <asm/gpio.h>
#include <asm/mach/irq.h>

#define OFF_MODE	1

static LIST_HEAD(omap_gpio_list);

struct gpio_regs {
=======
#include <linux/gpio/driver.h>
#include <linux/bitops.h>
#include <linux/platform_data/gpio-omap.h>

#define OMAP4_GPIO_DEBOUNCINGTIME_MASK 0xFF

struct gpio_regs {
	u32 sysconfig;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 irqenable1;
	u32 irqenable2;
	u32 wake_en;
	u32 ctrl;
	u32 oe;
	u32 leveldetect0;
	u32 leveldetect1;
	u32 risingdetect;
	u32 fallingdetect;
	u32 dataout;
	u32 debounce;
	u32 debounce_en;
};

struct gpio_bank {
<<<<<<< HEAD
	struct list_head node;
	void __iomem *base;
	u16 irq;
	int irq_base;
	struct irq_domain *domain;
	u32 suspend_wakeup;
	u32 saved_wakeup;
=======
	void __iomem *base;
	const struct omap_gpio_reg_offs *regs;
	struct device *dev;

	int irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 non_wakeup_gpios;
	u32 enabled_non_wakeup_gpios;
	struct gpio_regs context;
	u32 saved_datain;
<<<<<<< HEAD
	u32 saved_fallingdetect;
	u32 saved_risingdetect;
	u32 level_mask;
	u32 toggle_mask;
	spinlock_t lock;
	struct gpio_chip chip;
	struct clk *dbck;
	u32 mod_usage;
	u32 dbck_enable_mask;
	bool dbck_enabled;
	struct device *dev;
	bool is_mpuio;
	bool dbck_flag;
	bool loses_context;
	int stride;
	u32 width;
	int context_loss_count;
	int power_mode;
	bool workaround_enabled;

	void (*set_dataout)(struct gpio_bank *bank, int gpio, int enable);
	int (*get_context_loss_count)(struct device *dev);

	struct omap_gpio_reg_offs *regs;
};

#define GPIO_INDEX(bank, gpio) (gpio % bank->width)
#define GPIO_BIT(bank, gpio) (1 << GPIO_INDEX(bank, gpio))
#define GPIO_MOD_CTRL_BIT	BIT(0)

static int irq_to_gpio(struct gpio_bank *bank, unsigned int gpio_irq)
{
	return gpio_irq - bank->irq_base + bank->chip.base;
}

static void _set_gpio_direction(struct gpio_bank *bank, int gpio, int is_input)
{
	void __iomem *reg = bank->base;
	u32 l;

	reg += bank->regs->direction;
	l = __raw_readl(reg);
	if (is_input)
		l |= 1 << gpio;
	else
		l &= ~(1 << gpio);
	__raw_writel(l, reg);
	bank->context.oe = l;
=======
	u32 level_mask;
	u32 toggle_mask;
	raw_spinlock_t lock;
	raw_spinlock_t wa_lock;
	struct gpio_chip chip;
	struct clk *dbck;
	struct notifier_block nb;
	unsigned int is_suspended:1;
	unsigned int needs_resume:1;
	u32 mod_usage;
	u32 irq_usage;
	u32 dbck_enable_mask;
	bool dbck_enabled;
	bool is_mpuio;
	bool dbck_flag;
	bool loses_context;
	bool context_valid;
	int stride;
	u32 width;
	int context_loss_count;

	void (*set_dataout)(struct gpio_bank *bank, unsigned gpio, int enable);
	int (*get_context_loss_count)(struct device *dev);
};

#define GPIO_MOD_CTRL_BIT	BIT(0)

#define BANK_USED(bank) (bank->mod_usage || bank->irq_usage)
#define LINE_USED(line, offset) (line & (BIT(offset)))

static void omap_gpio_unmask_irq(struct irq_data *d);

static inline struct gpio_bank *omap_irq_data_get_bank(struct irq_data *d)
{
	struct gpio_chip *chip = irq_data_get_irq_chip_data(d);
	return gpiochip_get_data(chip);
}

static inline u32 omap_gpio_rmw(void __iomem *reg, u32 mask, bool set)
{
	u32 val = readl_relaxed(reg);

	if (set)
		val |= mask;
	else
		val &= ~mask;

	writel_relaxed(val, reg);

	return val;
}

static void omap_set_gpio_direction(struct gpio_bank *bank, int gpio,
				    int is_input)
{
	bank->context.oe = omap_gpio_rmw(bank->base + bank->regs->direction,
					 BIT(gpio), is_input);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/* set data out value using dedicate set/clear register */
<<<<<<< HEAD
static void _set_gpio_dataout_reg(struct gpio_bank *bank, int gpio, int enable)
{
	void __iomem *reg = bank->base;
	u32 l = GPIO_BIT(bank, gpio);
=======
static void omap_set_gpio_dataout_reg(struct gpio_bank *bank, unsigned offset,
				      int enable)
{
	void __iomem *reg = bank->base;
	u32 l = BIT(offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (enable) {
		reg += bank->regs->set_dataout;
		bank->context.dataout |= l;
	} else {
		reg += bank->regs->clr_dataout;
		bank->context.dataout &= ~l;
	}

<<<<<<< HEAD
	__raw_writel(l, reg);
}

/* set data out value using mask register */
static void _set_gpio_dataout_mask(struct gpio_bank *bank, int gpio, int enable)
{
	void __iomem *reg = bank->base + bank->regs->dataout;
	u32 gpio_bit = GPIO_BIT(bank, gpio);
	u32 l;

	l = __raw_readl(reg);
	if (enable)
		l |= gpio_bit;
	else
		l &= ~gpio_bit;
	__raw_writel(l, reg);
	bank->context.dataout = l;
}

static int _get_gpio_datain(struct gpio_bank *bank, int offset)
{
	void __iomem *reg = bank->base + bank->regs->datain;

	return (__raw_readl(reg) & (1 << offset)) != 0;
}

static int _get_gpio_dataout(struct gpio_bank *bank, int offset)
{
	void __iomem *reg = bank->base + bank->regs->dataout;

	return (__raw_readl(reg) & (1 << offset)) != 0;
}

static inline void _gpio_rmw(void __iomem *base, u32 reg, u32 mask, bool set)
{
	int l = __raw_readl(base + reg);

	if (set)
		l |= mask;
	else
		l &= ~mask;

	__raw_writel(l, base + reg);
}

static inline void _gpio_dbck_enable(struct gpio_bank *bank)
=======
	writel_relaxed(l, reg);
}

/* set data out value using mask register */
static void omap_set_gpio_dataout_mask(struct gpio_bank *bank, unsigned offset,
				       int enable)
{
	bank->context.dataout = omap_gpio_rmw(bank->base + bank->regs->dataout,
					      BIT(offset), enable);
}

static inline void omap_gpio_dbck_enable(struct gpio_bank *bank)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (bank->dbck_enable_mask && !bank->dbck_enabled) {
		clk_enable(bank->dbck);
		bank->dbck_enabled = true;
<<<<<<< HEAD
	}
}

static inline void _gpio_dbck_disable(struct gpio_bank *bank)
{
	if (bank->dbck_enable_mask && bank->dbck_enabled) {
=======

		writel_relaxed(bank->dbck_enable_mask,
			     bank->base + bank->regs->debounce_en);
	}
}

static inline void omap_gpio_dbck_disable(struct gpio_bank *bank)
{
	if (bank->dbck_enable_mask && bank->dbck_enabled) {
		/*
		 * Disable debounce before cutting it's clock. If debounce is
		 * enabled but the clock is not, GPIO module seems to be unable
		 * to detect events and generate interrupts at least on OMAP3.
		 */
		writel_relaxed(0, bank->base + bank->regs->debounce_en);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		clk_disable(bank->dbck);
		bank->dbck_enabled = false;
	}
}

/**
<<<<<<< HEAD
 * _set_gpio_debounce - low level gpio debounce time
 * @bank: the gpio bank we're acting upon
 * @gpio: the gpio number on this @gpio
 * @debounce: debounce time to use
 *
 * OMAP's debounce time is in 31us steps so we need
 * to convert and round up to the closest unit.
 */
static void _set_gpio_debounce(struct gpio_bank *bank, unsigned gpio,
		unsigned debounce)
{
	void __iomem		*reg;
	u32			val;
	u32			l;

	if (!bank->dbck_flag)
		return;

	if (debounce < 32)
		debounce = 0x01;
	else if (debounce > 7936)
		debounce = 0xff;
	else
		debounce = (debounce / 0x1f) - 1;

	l = GPIO_BIT(bank, gpio);

	clk_enable(bank->dbck);
	reg = bank->base + bank->regs->debounce;
	__raw_writel(debounce, reg);

	reg = bank->base + bank->regs->debounce_en;
	val = __raw_readl(reg);

	if (debounce)
		val |= l;
	else
		val &= ~l;
	bank->dbck_enable_mask = val;

	__raw_writel(val, reg);
=======
 * omap2_set_gpio_debounce - low level gpio debounce time
 * @bank: the gpio bank we're acting upon
 * @offset: the gpio number on this @bank
 * @debounce: debounce time to use
 *
 * OMAP's debounce time is in 31us steps
 *   <debounce time> = (GPIO_DEBOUNCINGTIME[7:0].DEBOUNCETIME + 1) x 31
 * so we need to convert and round up to the closest unit.
 *
 * Return: 0 on success, negative error otherwise.
 */
static int omap2_set_gpio_debounce(struct gpio_bank *bank, unsigned offset,
				   unsigned debounce)
{
	u32			val;
	u32			l;
	bool			enable = !!debounce;

	if (!bank->dbck_flag)
		return -ENOTSUPP;

	if (enable) {
		debounce = DIV_ROUND_UP(debounce, 31) - 1;
		if ((debounce & OMAP4_GPIO_DEBOUNCINGTIME_MASK) != debounce)
			return -EINVAL;
	}

	l = BIT(offset);

	clk_enable(bank->dbck);
	writel_relaxed(debounce, bank->base + bank->regs->debounce);

	val = omap_gpio_rmw(bank->base + bank->regs->debounce_en, l, enable);
	bank->dbck_enable_mask = val;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clk_disable(bank->dbck);
	/*
	 * Enable debounce clock per module.
	 * This call is mandatory because in omap_gpio_request() when
	 * *_runtime_get_sync() is called,  _gpio_dbck_enable() within
	 * runtime callbck fails to turn on dbck because dbck_enable_mask
	 * used within _gpio_dbck_enable() is still not initialized at
	 * that point. Therefore we have to enable dbck here.
	 */
<<<<<<< HEAD
	_gpio_dbck_enable(bank);
=======
	omap_gpio_dbck_enable(bank);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (bank->dbck_enable_mask) {
		bank->context.debounce = debounce;
		bank->context.debounce_en = val;
	}
<<<<<<< HEAD
}

static inline void set_gpio_trigger(struct gpio_bank *bank, int gpio,
						unsigned trigger)
{
	void __iomem *base = bank->base;
	u32 gpio_bit = 1 << gpio;

	_gpio_rmw(base, bank->regs->leveldetect0, gpio_bit,
		  trigger & IRQ_TYPE_LEVEL_LOW);
	_gpio_rmw(base, bank->regs->leveldetect1, gpio_bit,
		  trigger & IRQ_TYPE_LEVEL_HIGH);
	_gpio_rmw(base, bank->regs->risingdetect, gpio_bit,
		  trigger & IRQ_TYPE_EDGE_RISING);
	_gpio_rmw(base, bank->regs->fallingdetect, gpio_bit,
		  trigger & IRQ_TYPE_EDGE_FALLING);

	bank->context.leveldetect0 =
			__raw_readl(bank->base + bank->regs->leveldetect0);
	bank->context.leveldetect1 =
			__raw_readl(bank->base + bank->regs->leveldetect1);
	bank->context.risingdetect =
			__raw_readl(bank->base + bank->regs->risingdetect);
	bank->context.fallingdetect =
			__raw_readl(bank->base + bank->regs->fallingdetect);

	if (likely(!(bank->non_wakeup_gpios & gpio_bit))) {
		_gpio_rmw(base, bank->regs->wkup_en, gpio_bit, trigger != 0);
		bank->context.wake_en =
			__raw_readl(bank->base + bank->regs->wkup_en);
	}

	/* This part needs to be executed always for OMAP{34xx, 44xx} */
	if (!bank->regs->irqctrl) {
		/* On omap24xx proceed only when valid GPIO bit is set */
		if (bank->non_wakeup_gpios) {
			if (!(bank->non_wakeup_gpios & gpio_bit))
				goto exit;
		}

=======

	return 0;
}

/**
 * omap_clear_gpio_debounce - clear debounce settings for a gpio
 * @bank: the gpio bank we're acting upon
 * @offset: the gpio number on this @bank
 *
 * If a gpio is using debounce, then clear the debounce enable bit and if
 * this is the only gpio in this bank using debounce, then clear the debounce
 * time too. The debounce clock will also be disabled when calling this function
 * if this is the only gpio in the bank using debounce.
 */
static void omap_clear_gpio_debounce(struct gpio_bank *bank, unsigned offset)
{
	u32 gpio_bit = BIT(offset);

	if (!bank->dbck_flag)
		return;

	if (!(bank->dbck_enable_mask & gpio_bit))
		return;

	bank->dbck_enable_mask &= ~gpio_bit;
	bank->context.debounce_en &= ~gpio_bit;
        writel_relaxed(bank->context.debounce_en,
		     bank->base + bank->regs->debounce_en);

	if (!bank->dbck_enable_mask) {
		bank->context.debounce = 0;
		writel_relaxed(bank->context.debounce, bank->base +
			     bank->regs->debounce);
		clk_disable(bank->dbck);
		bank->dbck_enabled = false;
	}
}

/*
 * Off mode wake-up capable GPIOs in bank(s) that are in the wakeup domain.
 * See TRM section for GPIO for "Wake-Up Generation" for the list of GPIOs
 * in wakeup domain. If bank->non_wakeup_gpios is not configured, assume none
 * are capable waking up the system from off mode.
 */
static bool omap_gpio_is_off_wakeup_capable(struct gpio_bank *bank, u32 gpio_mask)
{
	u32 no_wake = bank->non_wakeup_gpios;

	if (no_wake)
		return !!(~no_wake & gpio_mask);

	return false;
}

static inline void omap_set_gpio_trigger(struct gpio_bank *bank, int gpio,
						unsigned trigger)
{
	void __iomem *base = bank->base;
	u32 gpio_bit = BIT(gpio);

	omap_gpio_rmw(base + bank->regs->leveldetect0, gpio_bit,
		      trigger & IRQ_TYPE_LEVEL_LOW);
	omap_gpio_rmw(base + bank->regs->leveldetect1, gpio_bit,
		      trigger & IRQ_TYPE_LEVEL_HIGH);

	/*
	 * We need the edge detection enabled for to allow the GPIO block
	 * to be woken from idle state.  Set the appropriate edge detection
	 * in addition to the level detection.
	 */
	omap_gpio_rmw(base + bank->regs->risingdetect, gpio_bit,
		      trigger & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_LEVEL_HIGH));
	omap_gpio_rmw(base + bank->regs->fallingdetect, gpio_bit,
		      trigger & (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_LEVEL_LOW));

	bank->context.leveldetect0 =
			readl_relaxed(bank->base + bank->regs->leveldetect0);
	bank->context.leveldetect1 =
			readl_relaxed(bank->base + bank->regs->leveldetect1);
	bank->context.risingdetect =
			readl_relaxed(bank->base + bank->regs->risingdetect);
	bank->context.fallingdetect =
			readl_relaxed(bank->base + bank->regs->fallingdetect);

	bank->level_mask = bank->context.leveldetect0 |
			   bank->context.leveldetect1;

	/* This part needs to be executed always for OMAP{34xx, 44xx} */
	if (!bank->regs->irqctrl && !omap_gpio_is_off_wakeup_capable(bank, gpio)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Log the edge gpio and manually trigger the IRQ
		 * after resume if the input level changes
		 * to avoid irq lost during PER RET/OFF mode
		 * Applies for omap2 non-wakeup gpio and all omap3 gpios
		 */
		if (trigger & IRQ_TYPE_EDGE_BOTH)
			bank->enabled_non_wakeup_gpios |= gpio_bit;
		else
			bank->enabled_non_wakeup_gpios &= ~gpio_bit;
	}
<<<<<<< HEAD

exit:
	bank->level_mask =
		__raw_readl(bank->base + bank->regs->leveldetect0) |
		__raw_readl(bank->base + bank->regs->leveldetect1);
}

#ifdef CONFIG_ARCH_OMAP1
=======
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This only applies to chips that can't do both rising and falling edge
 * detection at once.  For all other chips, this function is a noop.
 */
<<<<<<< HEAD
static void _toggle_gpio_edge_triggering(struct gpio_bank *bank, int gpio)
{
	void __iomem *reg = bank->base;
	u32 l = 0;

	if (!bank->regs->irqctrl)
		return;

	reg += bank->regs->irqctrl;

	l = __raw_readl(reg);
	if ((l >> gpio) & 1)
		l &= ~(1 << gpio);
	else
		l |= 1 << gpio;

	__raw_writel(l, reg);
}
#else
static void _toggle_gpio_edge_triggering(struct gpio_bank *bank, int gpio) {}
#endif

static int _set_gpio_triggering(struct gpio_bank *bank, int gpio,
							unsigned trigger)
{
	void __iomem *reg = bank->base;
	void __iomem *base = bank->base;
	u32 l = 0;

	if (bank->regs->leveldetect0 && bank->regs->wkup_en) {
		set_gpio_trigger(bank, gpio, trigger);
	} else if (bank->regs->irqctrl) {
		reg += bank->regs->irqctrl;

		l = __raw_readl(reg);
		if ((trigger & IRQ_TYPE_SENSE_MASK) == IRQ_TYPE_EDGE_BOTH)
			bank->toggle_mask |= 1 << gpio;
		if (trigger & IRQ_TYPE_EDGE_RISING)
			l |= 1 << gpio;
		else if (trigger & IRQ_TYPE_EDGE_FALLING)
			l &= ~(1 << gpio);
		else
			return -EINVAL;

		__raw_writel(l, reg);
=======
static void omap_toggle_gpio_edge_triggering(struct gpio_bank *bank, int gpio)
{
	if (IS_ENABLED(CONFIG_ARCH_OMAP1) && bank->regs->irqctrl) {
		void __iomem *reg = bank->base + bank->regs->irqctrl;

		writel_relaxed(readl_relaxed(reg) ^ BIT(gpio), reg);
	}
}

static int omap_set_gpio_triggering(struct gpio_bank *bank, int gpio,
				    unsigned trigger)
{
	void __iomem *reg = bank->base;
	u32 l = 0;

	if (bank->regs->leveldetect0 && bank->regs->wkup_en) {
		omap_set_gpio_trigger(bank, gpio, trigger);
	} else if (bank->regs->irqctrl) {
		reg += bank->regs->irqctrl;

		l = readl_relaxed(reg);
		if ((trigger & IRQ_TYPE_SENSE_MASK) == IRQ_TYPE_EDGE_BOTH)
			bank->toggle_mask |= BIT(gpio);
		if (trigger & IRQ_TYPE_EDGE_RISING)
			l |= BIT(gpio);
		else if (trigger & IRQ_TYPE_EDGE_FALLING)
			l &= ~(BIT(gpio));
		else
			return -EINVAL;

		writel_relaxed(l, reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (bank->regs->edgectrl1) {
		if (gpio & 0x08)
			reg += bank->regs->edgectrl2;
		else
			reg += bank->regs->edgectrl1;

		gpio &= 0x07;
<<<<<<< HEAD
		l = __raw_readl(reg);
=======
		l = readl_relaxed(reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		l &= ~(3 << (gpio << 1));
		if (trigger & IRQ_TYPE_EDGE_RISING)
			l |= 2 << (gpio << 1);
		if (trigger & IRQ_TYPE_EDGE_FALLING)
<<<<<<< HEAD
			l |= 1 << (gpio << 1);

		/* Enable wake-up during idle for dynamic tick */
		_gpio_rmw(base, bank->regs->wkup_en, 1 << gpio, trigger);
		bank->context.wake_en =
			__raw_readl(bank->base + bank->regs->wkup_en);
		__raw_writel(l, reg);
=======
			l |= BIT(gpio << 1);
		writel_relaxed(l, reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
static int gpio_irq_type(struct irq_data *d, unsigned type)
{
	struct gpio_bank *bank = irq_data_get_irq_chip_data(d);
	unsigned gpio;
	int retval;
	unsigned long flags;

	if (!cpu_class_is_omap2() && d->irq > IH_MPUIO_BASE)
		gpio = OMAP_MPUIO(d->irq - IH_MPUIO_BASE);
	else
		gpio = irq_to_gpio(bank, d->irq);
=======
static void omap_enable_gpio_module(struct gpio_bank *bank, unsigned offset)
{
	if (bank->regs->pinctrl) {
		void __iomem *reg = bank->base + bank->regs->pinctrl;

		/* Claim the pin for MPU */
		writel_relaxed(readl_relaxed(reg) | (BIT(offset)), reg);
	}

	if (bank->regs->ctrl && !BANK_USED(bank)) {
		void __iomem *reg = bank->base + bank->regs->ctrl;
		u32 ctrl;

		ctrl = readl_relaxed(reg);
		/* Module is enabled, clocks are not gated */
		ctrl &= ~GPIO_MOD_CTRL_BIT;
		writel_relaxed(ctrl, reg);
		bank->context.ctrl = ctrl;
	}
}

static void omap_disable_gpio_module(struct gpio_bank *bank, unsigned offset)
{
	if (bank->regs->ctrl && !BANK_USED(bank)) {
		void __iomem *reg = bank->base + bank->regs->ctrl;
		u32 ctrl;

		ctrl = readl_relaxed(reg);
		/* Module is disabled, clocks are gated */
		ctrl |= GPIO_MOD_CTRL_BIT;
		writel_relaxed(ctrl, reg);
		bank->context.ctrl = ctrl;
	}
}

static int omap_gpio_is_input(struct gpio_bank *bank, unsigned offset)
{
	void __iomem *reg = bank->base + bank->regs->direction;

	return readl_relaxed(reg) & BIT(offset);
}

static void omap_gpio_init_irq(struct gpio_bank *bank, unsigned offset)
{
	if (!LINE_USED(bank->mod_usage, offset)) {
		omap_enable_gpio_module(bank, offset);
		omap_set_gpio_direction(bank, offset, 1);
	}
	bank->irq_usage |= BIT(offset);
}

static int omap_gpio_irq_type(struct irq_data *d, unsigned type)
{
	struct gpio_bank *bank = omap_irq_data_get_bank(d);
	int retval;
	unsigned long flags;
	unsigned offset = d->hwirq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (type & ~IRQ_TYPE_SENSE_MASK)
		return -EINVAL;

	if (!bank->regs->leveldetect0 &&
		(type & (IRQ_TYPE_LEVEL_LOW|IRQ_TYPE_LEVEL_HIGH)))
		return -EINVAL;

<<<<<<< HEAD
	spin_lock_irqsave(&bank->lock, flags);
	retval = _set_gpio_triggering(bank, GPIO_INDEX(bank, gpio), type);
	spin_unlock_irqrestore(&bank->lock, flags);

	if (type & (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH))
		__irq_set_handler_locked(d->irq, handle_level_irq);
	else if (type & (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING))
		__irq_set_handler_locked(d->irq, handle_edge_irq);

	return retval;
}

static void _clear_gpio_irqbank(struct gpio_bank *bank, int gpio_mask)
=======
	raw_spin_lock_irqsave(&bank->lock, flags);
	retval = omap_set_gpio_triggering(bank, offset, type);
	if (retval) {
		raw_spin_unlock_irqrestore(&bank->lock, flags);
		goto error;
	}
	omap_gpio_init_irq(bank, offset);
	if (!omap_gpio_is_input(bank, offset)) {
		raw_spin_unlock_irqrestore(&bank->lock, flags);
		retval = -EINVAL;
		goto error;
	}
	raw_spin_unlock_irqrestore(&bank->lock, flags);

	if (type & (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH))
		irq_set_handler_locked(d, handle_level_irq);
	else if (type & (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING))
		/*
		 * Edge IRQs are already cleared/acked in irq_handler and
		 * not need to be masked, as result handle_edge_irq()
		 * logic is excessed here and may cause lose of interrupts.
		 * So just use handle_simple_irq.
		 */
		irq_set_handler_locked(d, handle_simple_irq);

	return 0;

error:
	return retval;
}

static void omap_clear_gpio_irqbank(struct gpio_bank *bank, int gpio_mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	void __iomem *reg = bank->base;

	reg += bank->regs->irqstatus;
<<<<<<< HEAD
	__raw_writel(gpio_mask, reg);
=======
	writel_relaxed(gpio_mask, reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Workaround for clearing DSP GPIO interrupts to allow retention */
	if (bank->regs->irqstatus2) {
		reg = bank->base + bank->regs->irqstatus2;
<<<<<<< HEAD
		__raw_writel(gpio_mask, reg);
	}

	/* Flush posted write for the irq status to avoid spurious interrupts */
	__raw_readl(reg);
}

static inline void _clear_gpio_irqstatus(struct gpio_bank *bank, int gpio)
{
	_clear_gpio_irqbank(bank, GPIO_BIT(bank, gpio));
}

static u32 _get_gpio_irqbank_mask(struct gpio_bank *bank)
{
	void __iomem *reg = bank->base;
	u32 l;
	u32 mask = (1 << bank->width) - 1;

	reg += bank->regs->irqenable;
	l = __raw_readl(reg);
=======
		writel_relaxed(gpio_mask, reg);
	}

	/* Flush posted write for the irq status to avoid spurious interrupts */
	readl_relaxed(reg);
}

static inline void omap_clear_gpio_irqstatus(struct gpio_bank *bank,
					     unsigned offset)
{
	omap_clear_gpio_irqbank(bank, BIT(offset));
}

static u32 omap_get_gpio_irqbank_mask(struct gpio_bank *bank)
{
	void __iomem *reg = bank->base;
	u32 l;
	u32 mask = (BIT(bank->width)) - 1;

	reg += bank->regs->irqenable;
	l = readl_relaxed(reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (bank->regs->irqenable_inv)
		l = ~l;
	l &= mask;
	return l;
}

<<<<<<< HEAD
static void _enable_gpio_irqbank(struct gpio_bank *bank, int gpio_mask)
{
	void __iomem *reg = bank->base;
	u32 l;

	if (bank->regs->set_irqenable) {
		reg += bank->regs->set_irqenable;
		l = gpio_mask;
		bank->context.irqenable1 |= gpio_mask;
	} else {
		reg += bank->regs->irqenable;
		l = __raw_readl(reg);
		if (bank->regs->irqenable_inv)
			l &= ~gpio_mask;
		else
			l |= gpio_mask;
		bank->context.irqenable1 = l;
	}

	__raw_writel(l, reg);
}

static void _disable_gpio_irqbank(struct gpio_bank *bank, int gpio_mask)
{
	void __iomem *reg = bank->base;
	u32 l;

	if (bank->regs->clr_irqenable) {
		reg += bank->regs->clr_irqenable;
		l = gpio_mask;
		bank->context.irqenable1 &= ~gpio_mask;
	} else {
		reg += bank->regs->irqenable;
		l = __raw_readl(reg);
		if (bank->regs->irqenable_inv)
			l |= gpio_mask;
		else
			l &= ~gpio_mask;
		bank->context.irqenable1 = l;
	}

	__raw_writel(l, reg);
}

static inline void _set_gpio_irqenable(struct gpio_bank *bank, int gpio, int enable)
{
	if (enable)
		_enable_gpio_irqbank(bank, GPIO_BIT(bank, gpio));
	else
		_disable_gpio_irqbank(bank, GPIO_BIT(bank, gpio));
}

/*
 * Note that ENAWAKEUP needs to be enabled in GPIO_SYSCONFIG register.
 * 1510 does not seem to have a wake-up register. If JTAG is connected
 * to the target, system will wake up always on GPIO events. While
 * system is running all registered GPIO interrupts need to have wake-up
 * enabled. When system is suspended, only selected GPIO interrupts need
 * to have wake-up enabled.
 */
static int _set_gpio_wakeup(struct gpio_bank *bank, int gpio, int enable)
{
	u32 gpio_bit = GPIO_BIT(bank, gpio);
	unsigned long flags;

	if (bank->non_wakeup_gpios & gpio_bit) {
		dev_err(bank->dev,
			"Unable to modify wakeup on non-wakeup GPIO%d\n", gpio);
		return -EINVAL;
	}

	spin_lock_irqsave(&bank->lock, flags);
	if (enable)
		bank->suspend_wakeup |= gpio_bit;
	else
		bank->suspend_wakeup &= ~gpio_bit;

	__raw_writel(bank->suspend_wakeup, bank->base + bank->regs->wkup_en);
	spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}

static void _reset_gpio(struct gpio_bank *bank, int gpio)
{
	_set_gpio_direction(bank, GPIO_INDEX(bank, gpio), 1);
	_set_gpio_irqenable(bank, gpio, 0);
	_clear_gpio_irqstatus(bank, gpio);
	_set_gpio_triggering(bank, GPIO_INDEX(bank, gpio), IRQ_TYPE_NONE);
}

/* Use disable_irq_wake() and enable_irq_wake() functions from drivers */
static int gpio_wake_enable(struct irq_data *d, unsigned int enable)
{
	struct gpio_bank *bank = irq_data_get_irq_chip_data(d);
	unsigned int gpio = irq_to_gpio(bank, d->irq);

	return _set_gpio_wakeup(bank, gpio, enable);
}

static int omap_gpio_request(struct gpio_chip *chip, unsigned offset)
{
	struct gpio_bank *bank = container_of(chip, struct gpio_bank, chip);
	unsigned long flags;

	/*
	 * If this is the first gpio_request for the bank,
	 * enable the bank module.
	 */
	if (!bank->mod_usage)
		pm_runtime_get_sync(bank->dev);

	spin_lock_irqsave(&bank->lock, flags);
	/* Set trigger to none. You need to enable the desired trigger with
	 * request_irq() or set_irq_type().
	 */
	_set_gpio_triggering(bank, offset, IRQ_TYPE_NONE);

	if (bank->regs->pinctrl) {
		void __iomem *reg = bank->base + bank->regs->pinctrl;

		/* Claim the pin for MPU */
		__raw_writel(__raw_readl(reg) | (1 << offset), reg);
	}

	if (bank->regs->ctrl && !bank->mod_usage) {
		void __iomem *reg = bank->base + bank->regs->ctrl;
		u32 ctrl;

		ctrl = __raw_readl(reg);
		/* Module is enabled, clocks are not gated */
		ctrl &= ~GPIO_MOD_CTRL_BIT;
		__raw_writel(ctrl, reg);
		bank->context.ctrl = ctrl;
	}

	bank->mod_usage |= 1 << offset;

	spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}

static void omap_gpio_free(struct gpio_chip *chip, unsigned offset)
{
	struct gpio_bank *bank = container_of(chip, struct gpio_bank, chip);
	void __iomem *base = bank->base;
	unsigned long flags;

	spin_lock_irqsave(&bank->lock, flags);

	if (bank->regs->wkup_en) {
		/* Disable wake-up during idle for dynamic tick */
		_gpio_rmw(base, bank->regs->wkup_en, 1 << offset, 0);
		bank->context.wake_en =
			__raw_readl(bank->base + bank->regs->wkup_en);
	}

	bank->mod_usage &= ~(1 << offset);

	if (bank->regs->ctrl && !bank->mod_usage) {
		void __iomem *reg = bank->base + bank->regs->ctrl;
		u32 ctrl;

		ctrl = __raw_readl(reg);
		/* Module is disabled, clocks are gated */
		ctrl |= GPIO_MOD_CTRL_BIT;
		__raw_writel(ctrl, reg);
		bank->context.ctrl = ctrl;
	}

	_reset_gpio(bank, bank->chip.base + offset);
	spin_unlock_irqrestore(&bank->lock, flags);

	/*
	 * If this is the last gpio to be freed in the bank,
	 * disable the bank module.
	 */
	if (!bank->mod_usage)
		pm_runtime_put(bank->dev);
=======
static inline void omap_set_gpio_irqenable(struct gpio_bank *bank,
					   unsigned offset, int enable)
{
	void __iomem *reg = bank->base;
	u32 gpio_mask = BIT(offset);

	if (bank->regs->set_irqenable && bank->regs->clr_irqenable) {
		if (enable) {
			reg += bank->regs->set_irqenable;
			bank->context.irqenable1 |= gpio_mask;
		} else {
			reg += bank->regs->clr_irqenable;
			bank->context.irqenable1 &= ~gpio_mask;
		}
		writel_relaxed(gpio_mask, reg);
	} else {
		bank->context.irqenable1 =
			omap_gpio_rmw(reg + bank->regs->irqenable, gpio_mask,
				      enable ^ bank->regs->irqenable_inv);
	}

	/*
	 * Program GPIO wakeup along with IRQ enable to satisfy OMAP4430 TRM
	 * note requiring correlation between the IRQ enable registers and
	 * the wakeup registers.  In any case, we want wakeup from idle
	 * enabled for the GPIOs which support this feature.
	 */
	if (bank->regs->wkup_en &&
	    (bank->regs->edgectrl1 || !(bank->non_wakeup_gpios & gpio_mask))) {
		bank->context.wake_en =
			omap_gpio_rmw(bank->base + bank->regs->wkup_en,
				      gpio_mask, enable);
	}
}

/* Use disable_irq_wake() and enable_irq_wake() functions from drivers */
static int omap_gpio_wake_enable(struct irq_data *d, unsigned int enable)
{
	struct gpio_bank *bank = omap_irq_data_get_bank(d);

	return irq_set_irq_wake(bank->irq, enable);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * We need to unmask the GPIO bank interrupt as soon as possible to
 * avoid missing GPIO interrupts for other lines in the bank.
 * Then we need to mask-read-clear-unmask the triggered GPIO lines
 * in the bank to avoid missing nested interrupts for a GPIO line.
 * If we wait to unmask individual GPIO lines in the bank after the
 * line's interrupt handler has been run, we may miss some nested
 * interrupts.
 */
<<<<<<< HEAD
static void gpio_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	void __iomem *isr_reg = NULL;
	u32 isr;
	unsigned int gpio_irq, gpio_index;
	struct gpio_bank *bank;
	u32 retrigger = 0;
	int unmasked = 0;
	struct irq_chip *chip = irq_desc_get_chip(desc);

	chained_irq_enter(chip, desc);

	bank = irq_get_handler_data(irq);
	isr_reg = bank->base + bank->regs->irqstatus;
	pm_runtime_get_sync(bank->dev);

	if (WARN_ON(!isr_reg))
		goto exit;

	while(1) {
		u32 isr_saved, level_mask = 0;
		u32 enabled;

		enabled = _get_gpio_irqbank_mask(bank);
		isr_saved = isr = __raw_readl(isr_reg) & enabled;

		if (bank->level_mask)
			level_mask = bank->level_mask & enabled;

		/* clear edge sensitive interrupts before handler(s) are
		called so that we don't miss any interrupt occurred while
		executing them */
		_disable_gpio_irqbank(bank, isr_saved & ~level_mask);
		_clear_gpio_irqbank(bank, isr_saved & ~level_mask);
		_enable_gpio_irqbank(bank, isr_saved & ~level_mask);

		/* if there is only edge sensitive GPIO pin interrupts
		configured, we could unmask GPIO bank interrupt immediately */
		if (!level_mask && !unmasked) {
			unmasked = 1;
			chained_irq_exit(chip, desc);
		}

		isr |= retrigger;
		retrigger = 0;
		if (!isr)
			break;

		gpio_irq = bank->irq_base;
		for (; isr != 0; isr >>= 1, gpio_irq++) {
			int gpio = irq_to_gpio(bank, gpio_irq);

			if (!(isr & 1))
				continue;

			gpio_index = GPIO_INDEX(bank, gpio);

=======
static irqreturn_t omap_gpio_irq_handler(int irq, void *gpiobank)
{
	void __iomem *isr_reg = NULL;
	u32 enabled, isr, edge;
	unsigned int bit;
	struct gpio_bank *bank = gpiobank;
	unsigned long wa_lock_flags;
	unsigned long lock_flags;

	isr_reg = bank->base + bank->regs->irqstatus;
	if (WARN_ON(!isr_reg))
		goto exit;

	if (WARN_ONCE(!pm_runtime_active(bank->chip.parent),
		      "gpio irq%i while runtime suspended?\n", irq))
		return IRQ_NONE;

	while (1) {
		raw_spin_lock_irqsave(&bank->lock, lock_flags);

		enabled = omap_get_gpio_irqbank_mask(bank);
		isr = readl_relaxed(isr_reg) & enabled;

		/*
		 * Clear edge sensitive interrupts before calling handler(s)
		 * so subsequent edge transitions are not missed while the
		 * handlers are running.
		 */
		edge = isr & ~bank->level_mask;
		if (edge)
			omap_clear_gpio_irqbank(bank, edge);

		raw_spin_unlock_irqrestore(&bank->lock, lock_flags);

		if (!isr)
			break;

		while (isr) {
			bit = __ffs(isr);
			isr &= ~(BIT(bit));

			raw_spin_lock_irqsave(&bank->lock, lock_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Some chips can't respond to both rising and falling
			 * at the same time.  If this irq was requested with
			 * both flags, we need to flip the ICR data for the IRQ
			 * to respond to the IRQ for the opposite direction.
			 * This will be indicated in the bank toggle_mask.
			 */
<<<<<<< HEAD
			if (bank->toggle_mask & (1 << gpio_index))
				_toggle_gpio_edge_triggering(bank, gpio_index);

			generic_handle_irq(gpio_irq);
		}
	}
	/* if bank has any level sensitive GPIO pin interrupt
	configured, we must unmask the bank interrupt only after
	handler(s) are executed in order to avoid spurious bank
	interrupt */
exit:
	if (!unmasked)
		chained_irq_exit(chip, desc);
	pm_runtime_put(bank->dev);
}

static void gpio_irq_shutdown(struct irq_data *d)
{
	struct gpio_bank *bank = irq_data_get_irq_chip_data(d);
	unsigned int gpio = irq_to_gpio(bank, d->irq);
	unsigned long flags;

	spin_lock_irqsave(&bank->lock, flags);
	_reset_gpio(bank, gpio);
	spin_unlock_irqrestore(&bank->lock, flags);
}

static void gpio_ack_irq(struct irq_data *d)
{
	struct gpio_bank *bank = irq_data_get_irq_chip_data(d);
	unsigned int gpio = irq_to_gpio(bank, d->irq);

	_clear_gpio_irqstatus(bank, gpio);
}

static void gpio_mask_irq(struct irq_data *d)
{
	struct gpio_bank *bank = irq_data_get_irq_chip_data(d);
	unsigned int gpio = irq_to_gpio(bank, d->irq);
	unsigned long flags;

	spin_lock_irqsave(&bank->lock, flags);
	_set_gpio_irqenable(bank, gpio, 0);
	_set_gpio_triggering(bank, GPIO_INDEX(bank, gpio), IRQ_TYPE_NONE);
	spin_unlock_irqrestore(&bank->lock, flags);
}

static void gpio_unmask_irq(struct irq_data *d)
{
	struct gpio_bank *bank = irq_data_get_irq_chip_data(d);
	unsigned int gpio = irq_to_gpio(bank, d->irq);
	unsigned int irq_mask = GPIO_BIT(bank, gpio);
	u32 trigger = irqd_get_trigger_type(d);
	unsigned long flags;

	spin_lock_irqsave(&bank->lock, flags);
	if (trigger)
		_set_gpio_triggering(bank, GPIO_INDEX(bank, gpio), trigger);

	/* For level-triggered GPIOs, the clearing must be done after
	 * the HW source is cleared, thus after the handler has run */
	if (bank->level_mask & irq_mask) {
		_set_gpio_irqenable(bank, gpio, 0);
		_clear_gpio_irqstatus(bank, gpio);
	}

	_set_gpio_irqenable(bank, gpio, 1);
	spin_unlock_irqrestore(&bank->lock, flags);
}

static struct irq_chip gpio_irq_chip = {
	.name		= "GPIO",
	.irq_shutdown	= gpio_irq_shutdown,
	.irq_ack	= gpio_ack_irq,
	.irq_mask	= gpio_mask_irq,
	.irq_unmask	= gpio_unmask_irq,
	.irq_set_type	= gpio_irq_type,
	.irq_set_wake	= gpio_wake_enable,
=======
			if (bank->toggle_mask & (BIT(bit)))
				omap_toggle_gpio_edge_triggering(bank, bit);

			raw_spin_unlock_irqrestore(&bank->lock, lock_flags);

			raw_spin_lock_irqsave(&bank->wa_lock, wa_lock_flags);

			generic_handle_domain_irq(bank->chip.irq.domain, bit);

			raw_spin_unlock_irqrestore(&bank->wa_lock,
						   wa_lock_flags);
		}
	}
exit:
	return IRQ_HANDLED;
}

static unsigned int omap_gpio_irq_startup(struct irq_data *d)
{
	struct gpio_bank *bank = omap_irq_data_get_bank(d);
	unsigned long flags;
	unsigned offset = d->hwirq;

	raw_spin_lock_irqsave(&bank->lock, flags);

	if (!LINE_USED(bank->mod_usage, offset))
		omap_set_gpio_direction(bank, offset, 1);
	omap_enable_gpio_module(bank, offset);
	bank->irq_usage |= BIT(offset);

	raw_spin_unlock_irqrestore(&bank->lock, flags);
	omap_gpio_unmask_irq(d);

	return 0;
}

static void omap_gpio_irq_shutdown(struct irq_data *d)
{
	struct gpio_bank *bank = omap_irq_data_get_bank(d);
	unsigned long flags;
	unsigned offset = d->hwirq;

	raw_spin_lock_irqsave(&bank->lock, flags);
	bank->irq_usage &= ~(BIT(offset));
	omap_set_gpio_triggering(bank, offset, IRQ_TYPE_NONE);
	omap_clear_gpio_irqstatus(bank, offset);
	omap_set_gpio_irqenable(bank, offset, 0);
	if (!LINE_USED(bank->mod_usage, offset))
		omap_clear_gpio_debounce(bank, offset);
	omap_disable_gpio_module(bank, offset);
	raw_spin_unlock_irqrestore(&bank->lock, flags);
}

static void omap_gpio_irq_bus_lock(struct irq_data *data)
{
	struct gpio_bank *bank = omap_irq_data_get_bank(data);

	pm_runtime_get_sync(bank->chip.parent);
}

static void gpio_irq_bus_sync_unlock(struct irq_data *data)
{
	struct gpio_bank *bank = omap_irq_data_get_bank(data);

	pm_runtime_put(bank->chip.parent);
}

static void omap_gpio_mask_irq(struct irq_data *d)
{
	struct gpio_bank *bank = omap_irq_data_get_bank(d);
	unsigned offset = d->hwirq;
	unsigned long flags;

	raw_spin_lock_irqsave(&bank->lock, flags);
	omap_set_gpio_triggering(bank, offset, IRQ_TYPE_NONE);
	omap_set_gpio_irqenable(bank, offset, 0);
	raw_spin_unlock_irqrestore(&bank->lock, flags);
	gpiochip_disable_irq(&bank->chip, offset);
}

static void omap_gpio_unmask_irq(struct irq_data *d)
{
	struct gpio_bank *bank = omap_irq_data_get_bank(d);
	unsigned offset = d->hwirq;
	u32 trigger = irqd_get_trigger_type(d);
	unsigned long flags;

	gpiochip_enable_irq(&bank->chip, offset);
	raw_spin_lock_irqsave(&bank->lock, flags);
	omap_set_gpio_irqenable(bank, offset, 1);

	/*
	 * For level-triggered GPIOs, clearing must be done after the source
	 * is cleared, thus after the handler has run. OMAP4 needs this done
	 * after enabing the interrupt to clear the wakeup status.
	 */
	if (bank->regs->leveldetect0 && bank->regs->wkup_en &&
	    trigger & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW))
		omap_clear_gpio_irqstatus(bank, offset);

	if (trigger)
		omap_set_gpio_triggering(bank, offset, trigger);

	raw_spin_unlock_irqrestore(&bank->lock, flags);
}

static void omap_gpio_irq_print_chip(struct irq_data *d, struct seq_file *p)
{
	struct gpio_bank *bank = omap_irq_data_get_bank(d);

	seq_printf(p, dev_name(bank->dev));
}

static const struct irq_chip omap_gpio_irq_chip = {
	.irq_startup = omap_gpio_irq_startup,
	.irq_shutdown = omap_gpio_irq_shutdown,
	.irq_mask = omap_gpio_mask_irq,
	.irq_unmask = omap_gpio_unmask_irq,
	.irq_set_type = omap_gpio_irq_type,
	.irq_set_wake = omap_gpio_wake_enable,
	.irq_bus_lock = omap_gpio_irq_bus_lock,
	.irq_bus_sync_unlock = gpio_irq_bus_sync_unlock,
	.irq_print_chip = omap_gpio_irq_print_chip,
	.flags = IRQCHIP_MASK_ON_SUSPEND | IRQCHIP_IMMUTABLE,
	 GPIOCHIP_IRQ_RESOURCE_HELPERS,
};

static const struct irq_chip omap_gpio_irq_chip_nowake = {
	.irq_startup = omap_gpio_irq_startup,
	.irq_shutdown = omap_gpio_irq_shutdown,
	.irq_mask = omap_gpio_mask_irq,
	.irq_unmask = omap_gpio_unmask_irq,
	.irq_set_type = omap_gpio_irq_type,
	.irq_bus_lock = omap_gpio_irq_bus_lock,
	.irq_bus_sync_unlock = gpio_irq_bus_sync_unlock,
	.irq_print_chip = omap_gpio_irq_print_chip,
	.flags = IRQCHIP_MASK_ON_SUSPEND | IRQCHIP_IMMUTABLE,
	 GPIOCHIP_IRQ_RESOURCE_HELPERS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*---------------------------------------------------------------------*/

static int omap_mpuio_suspend_noirq(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_bank	*bank = platform_get_drvdata(pdev);
=======
	struct gpio_bank	*bank = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __iomem		*mask_reg = bank->base +
					OMAP_MPUIO_GPIO_MASKIT / bank->stride;
	unsigned long		flags;

<<<<<<< HEAD
	spin_lock_irqsave(&bank->lock, flags);
	bank->saved_wakeup = __raw_readl(mask_reg);
	__raw_writel(0xffff & ~bank->suspend_wakeup, mask_reg);
	spin_unlock_irqrestore(&bank->lock, flags);
=======
	raw_spin_lock_irqsave(&bank->lock, flags);
	writel_relaxed(0xffff & ~bank->context.wake_en, mask_reg);
	raw_spin_unlock_irqrestore(&bank->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int omap_mpuio_resume_noirq(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_bank	*bank = platform_get_drvdata(pdev);
=======
	struct gpio_bank	*bank = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __iomem		*mask_reg = bank->base +
					OMAP_MPUIO_GPIO_MASKIT / bank->stride;
	unsigned long		flags;

<<<<<<< HEAD
	spin_lock_irqsave(&bank->lock, flags);
	__raw_writel(bank->saved_wakeup, mask_reg);
	spin_unlock_irqrestore(&bank->lock, flags);
=======
	raw_spin_lock_irqsave(&bank->lock, flags);
	writel_relaxed(bank->context.wake_en, mask_reg);
	raw_spin_unlock_irqrestore(&bank->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct dev_pm_ops omap_mpuio_dev_pm_ops = {
	.suspend_noirq = omap_mpuio_suspend_noirq,
	.resume_noirq = omap_mpuio_resume_noirq,
};

/* use platform_driver for this. */
static struct platform_driver omap_mpuio_driver = {
	.driver		= {
		.name	= "mpuio",
		.pm	= &omap_mpuio_dev_pm_ops,
	},
};

static struct platform_device omap_mpuio_device = {
	.name		= "mpuio",
	.id		= -1,
	.dev = {
		.driver = &omap_mpuio_driver.driver,
	}
	/* could list the /proc/iomem resources */
};

<<<<<<< HEAD
static inline void mpuio_init(struct gpio_bank *bank)
=======
static inline void omap_mpuio_init(struct gpio_bank *bank)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	platform_set_drvdata(&omap_mpuio_device, bank);

	if (platform_driver_register(&omap_mpuio_driver) == 0)
		(void) platform_device_register(&omap_mpuio_device);
}

/*---------------------------------------------------------------------*/

<<<<<<< HEAD
static int gpio_input(struct gpio_chip *chip, unsigned offset)
=======
static int omap_gpio_request(struct gpio_chip *chip, unsigned offset)
{
	struct gpio_bank *bank = gpiochip_get_data(chip);
	unsigned long flags;

	pm_runtime_get_sync(chip->parent);

	raw_spin_lock_irqsave(&bank->lock, flags);
	omap_enable_gpio_module(bank, offset);
	bank->mod_usage |= BIT(offset);
	raw_spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}

static void omap_gpio_free(struct gpio_chip *chip, unsigned offset)
{
	struct gpio_bank *bank = gpiochip_get_data(chip);
	unsigned long flags;

	raw_spin_lock_irqsave(&bank->lock, flags);
	bank->mod_usage &= ~(BIT(offset));
	if (!LINE_USED(bank->irq_usage, offset)) {
		omap_set_gpio_direction(bank, offset, 1);
		omap_clear_gpio_debounce(bank, offset);
	}
	omap_disable_gpio_module(bank, offset);
	raw_spin_unlock_irqrestore(&bank->lock, flags);

	pm_runtime_put(chip->parent);
}

static int omap_gpio_get_direction(struct gpio_chip *chip, unsigned offset)
{
	struct gpio_bank *bank = gpiochip_get_data(chip);

	if (readl_relaxed(bank->base + bank->regs->direction) & BIT(offset))
		return GPIO_LINE_DIRECTION_IN;

	return GPIO_LINE_DIRECTION_OUT;
}

static int omap_gpio_input(struct gpio_chip *chip, unsigned offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_bank *bank;
	unsigned long flags;

<<<<<<< HEAD
	bank = container_of(chip, struct gpio_bank, chip);
	spin_lock_irqsave(&bank->lock, flags);
	_set_gpio_direction(bank, offset, 1);
	spin_unlock_irqrestore(&bank->lock, flags);
	return 0;
}

static int gpio_is_input(struct gpio_bank *bank, int mask)
{
	void __iomem *reg = bank->base + bank->regs->direction;

	return __raw_readl(reg) & mask;
}

static int gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct gpio_bank *bank;
	u32 mask;

	bank = container_of(chip, struct gpio_bank, chip);
	mask = (1 << offset);

	if (gpio_is_input(bank, mask))
		return _get_gpio_datain(bank, offset);
	else
		return _get_gpio_dataout(bank, offset);
}

static int gpio_output(struct gpio_chip *chip, unsigned offset, int value)
=======
	bank = gpiochip_get_data(chip);
	raw_spin_lock_irqsave(&bank->lock, flags);
	omap_set_gpio_direction(bank, offset, 1);
	raw_spin_unlock_irqrestore(&bank->lock, flags);
	return 0;
}

static int omap_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct gpio_bank *bank = gpiochip_get_data(chip);
	void __iomem *reg;

	if (omap_gpio_is_input(bank, offset))
		reg = bank->base + bank->regs->datain;
	else
		reg = bank->base + bank->regs->dataout;

	return (readl_relaxed(reg) & BIT(offset)) != 0;
}

static int omap_gpio_output(struct gpio_chip *chip, unsigned offset, int value)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_bank *bank;
	unsigned long flags;

<<<<<<< HEAD
	bank = container_of(chip, struct gpio_bank, chip);
	spin_lock_irqsave(&bank->lock, flags);
	bank->set_dataout(bank, offset, value);
	_set_gpio_direction(bank, offset, 0);
	spin_unlock_irqrestore(&bank->lock, flags);
	return 0;
}

static int gpio_debounce(struct gpio_chip *chip, unsigned offset,
		unsigned debounce)
{
	struct gpio_bank *bank;
	unsigned long flags;

	bank = container_of(chip, struct gpio_bank, chip);

	if (!bank->dbck) {
		bank->dbck = clk_get(bank->dev, "dbclk");
		if (IS_ERR(bank->dbck))
			dev_err(bank->dev, "Could not get gpio dbck\n");
	}

	spin_lock_irqsave(&bank->lock, flags);
	_set_gpio_debounce(bank, offset, debounce);
	spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}

static void gpio_set(struct gpio_chip *chip, unsigned offset, int value)
=======
	bank = gpiochip_get_data(chip);
	raw_spin_lock_irqsave(&bank->lock, flags);
	bank->set_dataout(bank, offset, value);
	omap_set_gpio_direction(bank, offset, 0);
	raw_spin_unlock_irqrestore(&bank->lock, flags);
	return 0;
}

static int omap_gpio_get_multiple(struct gpio_chip *chip, unsigned long *mask,
				  unsigned long *bits)
{
	struct gpio_bank *bank = gpiochip_get_data(chip);
	void __iomem *base = bank->base;
	u32 direction, m, val = 0;

	direction = readl_relaxed(base + bank->regs->direction);

	m = direction & *mask;
	if (m)
		val |= readl_relaxed(base + bank->regs->datain) & m;

	m = ~direction & *mask;
	if (m)
		val |= readl_relaxed(base + bank->regs->dataout) & m;

	*bits = val;

	return 0;
}

static int omap_gpio_debounce(struct gpio_chip *chip, unsigned offset,
			      unsigned debounce)
{
	struct gpio_bank *bank;
	unsigned long flags;
	int ret;

	bank = gpiochip_get_data(chip);

	raw_spin_lock_irqsave(&bank->lock, flags);
	ret = omap2_set_gpio_debounce(bank, offset, debounce);
	raw_spin_unlock_irqrestore(&bank->lock, flags);

	if (ret)
		dev_info(chip->parent,
			 "Could not set line %u debounce to %u microseconds (%d)",
			 offset, debounce, ret);

	return ret;
}

static int omap_gpio_set_config(struct gpio_chip *chip, unsigned offset,
				unsigned long config)
{
	u32 debounce;
	int ret = -ENOTSUPP;

	switch (pinconf_to_config_param(config)) {
	case PIN_CONFIG_BIAS_DISABLE:
	case PIN_CONFIG_BIAS_PULL_UP:
	case PIN_CONFIG_BIAS_PULL_DOWN:
		ret = gpiochip_generic_config(chip, offset, config);
		break;
	case PIN_CONFIG_INPUT_DEBOUNCE:
		debounce = pinconf_to_config_argument(config);
		ret = omap_gpio_debounce(chip, offset, debounce);
		break;
	default:
		break;
	}

	return ret;
}

static void omap_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_bank *bank;
	unsigned long flags;

<<<<<<< HEAD
	bank = container_of(chip, struct gpio_bank, chip);
	spin_lock_irqsave(&bank->lock, flags);
	bank->set_dataout(bank, offset, value);
	spin_unlock_irqrestore(&bank->lock, flags);
}

static int gpio_2irq(struct gpio_chip *chip, unsigned offset)
{
	struct gpio_bank *bank;

	bank = container_of(chip, struct gpio_bank, chip);
	return bank->irq_base + offset;
=======
	bank = gpiochip_get_data(chip);
	raw_spin_lock_irqsave(&bank->lock, flags);
	bank->set_dataout(bank, offset, value);
	raw_spin_unlock_irqrestore(&bank->lock, flags);
}

static void omap_gpio_set_multiple(struct gpio_chip *chip, unsigned long *mask,
				   unsigned long *bits)
{
	struct gpio_bank *bank = gpiochip_get_data(chip);
	void __iomem *reg = bank->base + bank->regs->dataout;
	unsigned long flags;
	u32 l;

	raw_spin_lock_irqsave(&bank->lock, flags);
	l = (readl_relaxed(reg) & ~*mask) | (*bits & *mask);
	writel_relaxed(l, reg);
	bank->context.dataout = l;
	raw_spin_unlock_irqrestore(&bank->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*---------------------------------------------------------------------*/

<<<<<<< HEAD
static void __init omap_gpio_show_rev(struct gpio_bank *bank)
=======
static void omap_gpio_show_rev(struct gpio_bank *bank)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	static bool called;
	u32 rev;

	if (called || bank->regs->revision == USHRT_MAX)
		return;

<<<<<<< HEAD
	rev = __raw_readw(bank->base + bank->regs->revision);
=======
	rev = readw_relaxed(bank->base + bank->regs->revision);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pr_info("OMAP GPIO hardware version %d.%d\n",
		(rev >> 4) & 0x0f, rev & 0x0f);

	called = true;
}

<<<<<<< HEAD
/* This lock class tells lockdep that GPIO irqs are in a different
 * category than their parents, so it won't report false recursion.
 */
static struct lock_class_key gpio_lock_class;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void omap_gpio_mod_init(struct gpio_bank *bank)
{
	void __iomem *base = bank->base;
	u32 l = 0xffffffff;

	if (bank->width == 16)
		l = 0xffff;

	if (bank->is_mpuio) {
<<<<<<< HEAD
		__raw_writel(l, bank->base + bank->regs->irqenable);
		return;
	}

	_gpio_rmw(base, bank->regs->irqenable, l, bank->regs->irqenable_inv);
	_gpio_rmw(base, bank->regs->irqstatus, l, !bank->regs->irqenable_inv);
	if (bank->regs->debounce_en)
		__raw_writel(0, base + bank->regs->debounce_en);

	/* Save OE default value (0xffffffff) in the context */
	bank->context.oe = __raw_readl(bank->base + bank->regs->direction);
	 /* Initialize interface clk ungated, module enabled */
	if (bank->regs->ctrl)
		__raw_writel(0, base + bank->regs->ctrl);
}

static __devinit void
omap_mpuio_alloc_gc(struct gpio_bank *bank, unsigned int irq_start,
		    unsigned int num)
{
	struct irq_chip_generic *gc;
	struct irq_chip_type *ct;

	gc = irq_alloc_generic_chip("MPUIO", 1, irq_start, bank->base,
				    handle_simple_irq);
	if (!gc) {
		dev_err(bank->dev, "Memory alloc failed for gc\n");
		return;
	}

	ct = gc->chip_types;

	/* NOTE: No ack required, reading IRQ status clears it. */
	ct->chip.irq_mask = irq_gc_mask_set_bit;
	ct->chip.irq_unmask = irq_gc_mask_clr_bit;
	ct->chip.irq_set_type = gpio_irq_type;

	if (bank->regs->wkup_en)
		ct->chip.irq_set_wake = gpio_wake_enable,

	ct->regs.mask = OMAP_MPUIO_GPIO_INT / bank->stride;
	irq_setup_generic_chip(gc, IRQ_MSK(num), IRQ_GC_INIT_MASK_CACHE,
			       IRQ_NOREQUEST | IRQ_NOPROBE, 0);
}

static void __devinit omap_gpio_chip_init(struct gpio_bank *bank)
{
	int j;
	static int gpio;
=======
		writel_relaxed(l, bank->base + bank->regs->irqenable);
		return;
	}

	omap_gpio_rmw(base + bank->regs->irqenable, l,
		      bank->regs->irqenable_inv);
	omap_gpio_rmw(base + bank->regs->irqstatus, l,
		      !bank->regs->irqenable_inv);
	if (bank->regs->debounce_en)
		writel_relaxed(0, base + bank->regs->debounce_en);

	/* Save OE default value (0xffffffff) in the context */
	bank->context.oe = readl_relaxed(bank->base + bank->regs->direction);
	 /* Initialize interface clk ungated, module enabled */
	if (bank->regs->ctrl)
		writel_relaxed(0, base + bank->regs->ctrl);
}

static int omap_gpio_chip_init(struct gpio_bank *bank, struct device *pm_dev)
{
	struct gpio_irq_chip *irq;
	static int gpio;
	const char *label;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * REVISIT eventually switch from OMAP-specific gpio structs
	 * over to the generic ones
	 */
	bank->chip.request = omap_gpio_request;
	bank->chip.free = omap_gpio_free;
<<<<<<< HEAD
	bank->chip.direction_input = gpio_input;
	bank->chip.get = gpio_get;
	bank->chip.direction_output = gpio_output;
	bank->chip.set_debounce = gpio_debounce;
	bank->chip.set = gpio_set;
	bank->chip.to_irq = gpio_2irq;
	if (bank->is_mpuio) {
		bank->chip.label = "mpuio";
		if (bank->regs->wkup_en)
			bank->chip.dev = &omap_mpuio_device.dev;
		bank->chip.base = OMAP_MPUIO(0);
	} else {
		bank->chip.label = "gpio";
		bank->chip.base = gpio;
		gpio += bank->width;
	}
	bank->chip.ngpio = bank->width;

	gpiochip_add(&bank->chip);

	for (j = bank->irq_base; j < bank->irq_base + bank->width; j++) {
		irq_set_lockdep_class(j, &gpio_lock_class);
		irq_set_chip_data(j, bank);
		if (bank->is_mpuio) {
			omap_mpuio_alloc_gc(bank, j, bank->width);
		} else {
			irq_set_chip(j, &gpio_irq_chip);
			irq_set_handler(j, handle_simple_irq);
			set_irq_flags(j, IRQF_VALID);
		}
	}
	irq_set_chained_handler(bank->irq, gpio_irq_handler);
	irq_set_handler_data(bank->irq, bank);
}

static const struct of_device_id omap_gpio_match[];

static int __devinit omap_gpio_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *node = dev->of_node;
	const struct of_device_id *match;
	struct omap_gpio_platform_data *pdata;
	struct resource *res;
	struct gpio_bank *bank;
	int ret = 0;

	match = of_match_device(of_match_ptr(omap_gpio_match), dev);

	pdata = match ? match->data : dev->platform_data;
	if (!pdata)
		return -EINVAL;

	bank = devm_kzalloc(&pdev->dev, sizeof(struct gpio_bank), GFP_KERNEL);
	if (!bank) {
		dev_err(dev, "Memory alloc failed\n");
		return -ENOMEM;
	}

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (unlikely(!res)) {
		dev_err(dev, "Invalid IRQ resource\n");
		return -ENODEV;
	}

	bank->irq = res->start;
	bank->dev = dev;
	bank->dbck_flag = pdata->dbck_flag;
	bank->stride = pdata->bank_stride;
	bank->width = pdata->bank_width;
	bank->is_mpuio = pdata->is_mpuio;
	bank->non_wakeup_gpios = pdata->non_wakeup_gpios;
	bank->loses_context = pdata->loses_context;
	bank->get_context_loss_count = pdata->get_context_loss_count;
	bank->regs = pdata->regs;
#ifdef CONFIG_OF_GPIO
	bank->chip.of_node = of_node_get(node);
#endif

	bank->irq_base = irq_alloc_descs(-1, 0, bank->width, 0);
	if (bank->irq_base < 0) {
		dev_err(dev, "Couldn't allocate IRQ numbers\n");
		return -ENODEV;
	}

	bank->domain = irq_domain_add_legacy(node, bank->width, bank->irq_base,
					     0, &irq_domain_simple_ops, NULL);

	if (bank->regs->set_dataout && bank->regs->clr_dataout)
		bank->set_dataout = _set_gpio_dataout_reg;
	else
		bank->set_dataout = _set_gpio_dataout_mask;

	spin_lock_init(&bank->lock);

	/* Static mapping, never released */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!res)) {
		dev_err(dev, "Invalid mem resource\n");
		return -ENODEV;
	}

	if (!devm_request_mem_region(dev, res->start, resource_size(res),
				     pdev->name)) {
		dev_err(dev, "Region already claimed\n");
		return -EBUSY;
	}

	bank->base = devm_ioremap(dev, res->start, resource_size(res));
	if (!bank->base) {
		dev_err(dev, "Could not ioremap\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, bank);

	pm_runtime_enable(bank->dev);
	pm_runtime_irq_safe(bank->dev);
	pm_runtime_get_sync(bank->dev);

	if (bank->is_mpuio)
		mpuio_init(bank);

	omap_gpio_mod_init(bank);
	omap_gpio_chip_init(bank);
	omap_gpio_show_rev(bank);

	pm_runtime_put(bank->dev);

	list_add_tail(&bank->node, &omap_gpio_list);
=======
	bank->chip.get_direction = omap_gpio_get_direction;
	bank->chip.direction_input = omap_gpio_input;
	bank->chip.get = omap_gpio_get;
	bank->chip.get_multiple = omap_gpio_get_multiple;
	bank->chip.direction_output = omap_gpio_output;
	bank->chip.set_config = omap_gpio_set_config;
	bank->chip.set = omap_gpio_set;
	bank->chip.set_multiple = omap_gpio_set_multiple;
	if (bank->is_mpuio) {
		bank->chip.label = "mpuio";
		if (bank->regs->wkup_en)
			bank->chip.parent = &omap_mpuio_device.dev;
	} else {
		label = devm_kasprintf(bank->chip.parent, GFP_KERNEL, "gpio-%d-%d",
				       gpio, gpio + bank->width - 1);
		if (!label)
			return -ENOMEM;
		bank->chip.label = label;
	}
	bank->chip.base = -1;
	bank->chip.ngpio = bank->width;

	irq = &bank->chip.irq;
	/* MPUIO is a bit different, reading IRQ status clears it */
	if (bank->is_mpuio && !bank->regs->wkup_en)
		gpio_irq_chip_set_chip(irq, &omap_gpio_irq_chip_nowake);
	else
		gpio_irq_chip_set_chip(irq, &omap_gpio_irq_chip);
	irq->handler = handle_bad_irq;
	irq->default_type = IRQ_TYPE_NONE;
	irq->num_parents = 1;
	irq->parents = &bank->irq;

	ret = gpiochip_add_data(&bank->chip, bank);
	if (ret)
		return dev_err_probe(bank->chip.parent, ret, "Could not register gpio chip\n");

	irq_domain_set_pm_device(bank->chip.irq.domain, pm_dev);
	ret = devm_request_irq(bank->chip.parent, bank->irq,
			       omap_gpio_irq_handler,
			       0, dev_name(bank->chip.parent), bank);
	if (ret)
		gpiochip_remove(&bank->chip);

	if (!bank->is_mpuio)
		gpio += bank->width;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
#ifdef CONFIG_ARCH_OMAP2PLUS

#if defined(CONFIG_PM_SLEEP)
static int omap_gpio_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_bank *bank = platform_get_drvdata(pdev);
	void __iomem *base = bank->base;
	void __iomem *wakeup_enable;
	unsigned long flags;

	if (!bank->mod_usage || !bank->loses_context)
		return 0;

	if (!bank->regs->wkup_en || !bank->suspend_wakeup)
		return 0;

	wakeup_enable = bank->base + bank->regs->wkup_en;

	spin_lock_irqsave(&bank->lock, flags);
	bank->saved_wakeup = __raw_readl(wakeup_enable);
	_gpio_rmw(base, bank->regs->wkup_en, 0xffffffff, 0);
	_gpio_rmw(base, bank->regs->wkup_en, bank->suspend_wakeup, 1);
	spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}

static int omap_gpio_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_bank *bank = platform_get_drvdata(pdev);
	void __iomem *base = bank->base;
	unsigned long flags;

	if (!bank->mod_usage || !bank->loses_context)
		return 0;

	if (!bank->regs->wkup_en || !bank->saved_wakeup)
		return 0;

	spin_lock_irqsave(&bank->lock, flags);
	_gpio_rmw(base, bank->regs->wkup_en, 0xffffffff, 0);
	_gpio_rmw(base, bank->regs->wkup_en, bank->saved_wakeup, 1);
	spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}
#endif /* CONFIG_PM_SLEEP */

#if defined(CONFIG_PM_RUNTIME)
static void omap_gpio_restore_context(struct gpio_bank *bank);

static int omap_gpio_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_bank *bank = platform_get_drvdata(pdev);
	u32 l1 = 0, l2 = 0;
	unsigned long flags;
	u32 wake_low, wake_hi;

	spin_lock_irqsave(&bank->lock, flags);

	/*
	 * Only edges can generate a wakeup event to the PRCM.
	 *
	 * Therefore, ensure any wake-up capable GPIOs have
	 * edge-detection enabled before going idle to ensure a wakeup
	 * to the PRCM is generated on a GPIO transition. (c.f. 34xx
	 * NDA TRM 25.5.3.1)
	 *
	 * The normal values will be restored upon ->runtime_resume()
	 * by writing back the values saved in bank->context.
	 */
	wake_low = bank->context.leveldetect0 & bank->context.wake_en;
	if (wake_low)
		__raw_writel(wake_low | bank->context.fallingdetect,
			     bank->base + bank->regs->fallingdetect);
	wake_hi = bank->context.leveldetect1 & bank->context.wake_en;
	if (wake_hi)
		__raw_writel(wake_hi | bank->context.risingdetect,
			     bank->base + bank->regs->risingdetect);

	if (bank->power_mode != OFF_MODE) {
		bank->power_mode = 0;
		goto update_gpio_context_count;
	}
	/*
	 * If going to OFF, remove triggering for all
	 * non-wakeup GPIOs.  Otherwise spurious IRQs will be
	 * generated.  See OMAP2420 Errata item 1.101.
	 */
	bank->saved_datain = __raw_readl(bank->base +
						bank->regs->datain);
	l1 = __raw_readl(bank->base + bank->regs->fallingdetect);
	l2 = __raw_readl(bank->base + bank->regs->risingdetect);

	bank->saved_fallingdetect = l1;
	bank->saved_risingdetect = l2;
	l1 &= ~bank->enabled_non_wakeup_gpios;
	l2 &= ~bank->enabled_non_wakeup_gpios;

	__raw_writel(l1, bank->base + bank->regs->fallingdetect);
	__raw_writel(l2, bank->base + bank->regs->risingdetect);

	bank->workaround_enabled = true;
=======
static void omap_gpio_init_context(struct gpio_bank *p)
{
	const struct omap_gpio_reg_offs *regs = p->regs;
	void __iomem *base = p->base;

	p->context.sysconfig	= readl_relaxed(base + regs->sysconfig);
	p->context.ctrl		= readl_relaxed(base + regs->ctrl);
	p->context.oe		= readl_relaxed(base + regs->direction);
	p->context.wake_en	= readl_relaxed(base + regs->wkup_en);
	p->context.leveldetect0	= readl_relaxed(base + regs->leveldetect0);
	p->context.leveldetect1	= readl_relaxed(base + regs->leveldetect1);
	p->context.risingdetect	= readl_relaxed(base + regs->risingdetect);
	p->context.fallingdetect = readl_relaxed(base + regs->fallingdetect);
	p->context.irqenable1	= readl_relaxed(base + regs->irqenable);
	p->context.irqenable2	= readl_relaxed(base + regs->irqenable2);
	p->context.dataout	= readl_relaxed(base + regs->dataout);

	p->context_valid = true;
}

static void omap_gpio_restore_context(struct gpio_bank *bank)
{
	const struct omap_gpio_reg_offs *regs = bank->regs;
	void __iomem *base = bank->base;

	writel_relaxed(bank->context.sysconfig, base + regs->sysconfig);
	writel_relaxed(bank->context.wake_en, base + regs->wkup_en);
	writel_relaxed(bank->context.ctrl, base + regs->ctrl);
	writel_relaxed(bank->context.leveldetect0, base + regs->leveldetect0);
	writel_relaxed(bank->context.leveldetect1, base + regs->leveldetect1);
	writel_relaxed(bank->context.risingdetect, base + regs->risingdetect);
	writel_relaxed(bank->context.fallingdetect, base + regs->fallingdetect);
	writel_relaxed(bank->context.dataout, base + regs->dataout);
	writel_relaxed(bank->context.oe, base + regs->direction);

	if (bank->dbck_enable_mask) {
		writel_relaxed(bank->context.debounce, base + regs->debounce);
		writel_relaxed(bank->context.debounce_en,
			       base + regs->debounce_en);
	}

	writel_relaxed(bank->context.irqenable1, base + regs->irqenable);
	writel_relaxed(bank->context.irqenable2, base + regs->irqenable2);
}

static void omap_gpio_idle(struct gpio_bank *bank, bool may_lose_context)
{
	struct device *dev = bank->chip.parent;
	void __iomem *base = bank->base;
	u32 mask, nowake;

	bank->saved_datain = readl_relaxed(base + bank->regs->datain);

	/* Save syconfig, it's runtime value can be different from init value */
	if (bank->loses_context)
		bank->context.sysconfig = readl_relaxed(base + bank->regs->sysconfig);

	if (!bank->enabled_non_wakeup_gpios)
		goto update_gpio_context_count;

	/* Check for pending EDGE_FALLING, ignore EDGE_BOTH */
	mask = bank->enabled_non_wakeup_gpios & bank->context.fallingdetect;
	mask &= ~bank->context.risingdetect;
	bank->saved_datain |= mask;

	/* Check for pending EDGE_RISING, ignore EDGE_BOTH */
	mask = bank->enabled_non_wakeup_gpios & bank->context.risingdetect;
	mask &= ~bank->context.fallingdetect;
	bank->saved_datain &= ~mask;

	if (!may_lose_context)
		goto update_gpio_context_count;

	/*
	 * If going to OFF, remove triggering for all wkup domain
	 * non-wakeup GPIOs.  Otherwise spurious IRQs will be
	 * generated.  See OMAP2420 Errata item 1.101.
	 */
	if (!bank->loses_context && bank->enabled_non_wakeup_gpios) {
		nowake = bank->enabled_non_wakeup_gpios;
		omap_gpio_rmw(base + bank->regs->fallingdetect, nowake, ~nowake);
		omap_gpio_rmw(base + bank->regs->risingdetect, nowake, ~nowake);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

update_gpio_context_count:
	if (bank->get_context_loss_count)
		bank->context_loss_count =
<<<<<<< HEAD
				bank->get_context_loss_count(bank->dev);

	_gpio_dbck_disable(bank);
	spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}

static int omap_gpio_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_bank *bank = platform_get_drvdata(pdev);
	int context_lost_cnt_after;
	u32 l = 0, gen, gen0, gen1;
	unsigned long flags;

	spin_lock_irqsave(&bank->lock, flags);
	_gpio_dbck_enable(bank);

	/*
	 * In ->runtime_suspend(), level-triggered, wakeup-enabled
	 * GPIOs were set to edge trigger also in order to be able to
	 * generate a PRCM wakeup.  Here we restore the
	 * pre-runtime_suspend() values for edge triggering.
	 */
	__raw_writel(bank->context.fallingdetect,
		     bank->base + bank->regs->fallingdetect);
	__raw_writel(bank->context.risingdetect,
		     bank->base + bank->regs->risingdetect);

	if (!bank->workaround_enabled) {
		spin_unlock_irqrestore(&bank->lock, flags);
		return 0;
	}

	if (bank->get_context_loss_count) {
		context_lost_cnt_after =
			bank->get_context_loss_count(bank->dev);
		if (context_lost_cnt_after != bank->context_loss_count ||
						!context_lost_cnt_after) {
			omap_gpio_restore_context(bank);
		} else {
			spin_unlock_irqrestore(&bank->lock, flags);
			return 0;
		}
	}

	__raw_writel(bank->saved_fallingdetect,
			bank->base + bank->regs->fallingdetect);
	__raw_writel(bank->saved_risingdetect,
			bank->base + bank->regs->risingdetect);
	l = __raw_readl(bank->base + bank->regs->datain);
=======
				bank->get_context_loss_count(dev);

	omap_gpio_dbck_disable(bank);
}

static void omap_gpio_unidle(struct gpio_bank *bank)
{
	struct device *dev = bank->chip.parent;
	u32 l = 0, gen, gen0, gen1;
	int c;

	/*
	 * On the first resume during the probe, the context has not
	 * been initialised and so initialise it now. Also initialise
	 * the context loss count.
	 */
	if (bank->loses_context && !bank->context_valid) {
		omap_gpio_init_context(bank);

		if (bank->get_context_loss_count)
			bank->context_loss_count =
				bank->get_context_loss_count(dev);
	}

	omap_gpio_dbck_enable(bank);

	if (bank->loses_context) {
		if (!bank->get_context_loss_count) {
			omap_gpio_restore_context(bank);
		} else {
			c = bank->get_context_loss_count(dev);
			if (c != bank->context_loss_count) {
				omap_gpio_restore_context(bank);
			} else {
				return;
			}
		}
	} else {
		/* Restore changes done for OMAP2420 errata 1.101 */
		writel_relaxed(bank->context.fallingdetect,
			       bank->base + bank->regs->fallingdetect);
		writel_relaxed(bank->context.risingdetect,
			       bank->base + bank->regs->risingdetect);
	}

	l = readl_relaxed(bank->base + bank->regs->datain);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Check if any of the non-wakeup interrupt GPIOs have changed
	 * state.  If so, generate an IRQ by software.  This is
	 * horribly racy, but it's the best we can do to work around
	 * this silicon bug.
	 */
	l ^= bank->saved_datain;
	l &= bank->enabled_non_wakeup_gpios;

	/*
	 * No need to generate IRQs for the rising edge for gpio IRQs
	 * configured with falling edge only; and vice versa.
	 */
<<<<<<< HEAD
	gen0 = l & bank->saved_fallingdetect;
	gen0 &= bank->saved_datain;

	gen1 = l & bank->saved_risingdetect;
	gen1 &= ~(bank->saved_datain);

	/* FIXME: Consider GPIO IRQs with level detections properly! */
	gen = l & (~(bank->saved_fallingdetect) & ~(bank->saved_risingdetect));
=======
	gen0 = l & bank->context.fallingdetect;
	gen0 &= bank->saved_datain;

	gen1 = l & bank->context.risingdetect;
	gen1 &= ~(bank->saved_datain);

	/* FIXME: Consider GPIO IRQs with level detections properly! */
	gen = l & (~(bank->context.fallingdetect) &
					 ~(bank->context.risingdetect));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Consider all GPIO IRQs needed to be updated */
	gen |= gen0 | gen1;

	if (gen) {
		u32 old0, old1;

<<<<<<< HEAD
		old0 = __raw_readl(bank->base + bank->regs->leveldetect0);
		old1 = __raw_readl(bank->base + bank->regs->leveldetect1);

		if (cpu_is_omap24xx() || cpu_is_omap34xx()) {
			__raw_writel(old0 | gen, bank->base +
						bank->regs->leveldetect0);
			__raw_writel(old1 | gen, bank->base +
						bank->regs->leveldetect1);
		}

		if (cpu_is_omap44xx()) {
			__raw_writel(old0 | l, bank->base +
						bank->regs->leveldetect0);
			__raw_writel(old1 | l, bank->base +
						bank->regs->leveldetect1);
		}
		__raw_writel(old0, bank->base + bank->regs->leveldetect0);
		__raw_writel(old1, bank->base + bank->regs->leveldetect1);
	}

	bank->workaround_enabled = false;
	spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}
#endif /* CONFIG_PM_RUNTIME */

void omap2_gpio_prepare_for_idle(int pwr_mode)
{
	struct gpio_bank *bank;

	list_for_each_entry(bank, &omap_gpio_list, node) {
		if (!bank->mod_usage || !bank->loses_context)
			continue;

		bank->power_mode = pwr_mode;

		pm_runtime_put_sync_suspend(bank->dev);
	}
}

void omap2_gpio_resume_after_idle(void)
{
	struct gpio_bank *bank;

	list_for_each_entry(bank, &omap_gpio_list, node) {
		if (!bank->mod_usage || !bank->loses_context)
			continue;

		pm_runtime_get_sync(bank->dev);
	}
}

#if defined(CONFIG_PM_RUNTIME)
static void omap_gpio_restore_context(struct gpio_bank *bank)
{
	__raw_writel(bank->context.wake_en,
				bank->base + bank->regs->wkup_en);
	__raw_writel(bank->context.ctrl, bank->base + bank->regs->ctrl);
	__raw_writel(bank->context.leveldetect0,
				bank->base + bank->regs->leveldetect0);
	__raw_writel(bank->context.leveldetect1,
				bank->base + bank->regs->leveldetect1);
	__raw_writel(bank->context.risingdetect,
				bank->base + bank->regs->risingdetect);
	__raw_writel(bank->context.fallingdetect,
				bank->base + bank->regs->fallingdetect);
	if (bank->regs->set_dataout && bank->regs->clr_dataout)
		__raw_writel(bank->context.dataout,
				bank->base + bank->regs->set_dataout);
	else
		__raw_writel(bank->context.dataout,
				bank->base + bank->regs->dataout);
	__raw_writel(bank->context.oe, bank->base + bank->regs->direction);

	if (bank->dbck_enable_mask) {
		__raw_writel(bank->context.debounce, bank->base +
					bank->regs->debounce);
		__raw_writel(bank->context.debounce_en,
					bank->base + bank->regs->debounce_en);
	}

	__raw_writel(bank->context.irqenable1,
				bank->base + bank->regs->irqenable);
	__raw_writel(bank->context.irqenable2,
				bank->base + bank->regs->irqenable2);
}
#endif /* CONFIG_PM_RUNTIME */
#else
#define omap_gpio_suspend NULL
#define omap_gpio_resume NULL
#define omap_gpio_runtime_suspend NULL
#define omap_gpio_runtime_resume NULL
#endif

static const struct dev_pm_ops gpio_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(omap_gpio_suspend, omap_gpio_resume)
	SET_RUNTIME_PM_OPS(omap_gpio_runtime_suspend, omap_gpio_runtime_resume,
									NULL)
};

#if defined(CONFIG_OF)
static struct omap_gpio_reg_offs omap2_gpio_regs = {
	.revision =		OMAP24XX_GPIO_REVISION,
=======
		old0 = readl_relaxed(bank->base + bank->regs->leveldetect0);
		old1 = readl_relaxed(bank->base + bank->regs->leveldetect1);

		if (!bank->regs->irqstatus_raw0) {
			writel_relaxed(old0 | gen, bank->base +
						bank->regs->leveldetect0);
			writel_relaxed(old1 | gen, bank->base +
						bank->regs->leveldetect1);
		}

		if (bank->regs->irqstatus_raw0) {
			writel_relaxed(old0 | l, bank->base +
						bank->regs->leveldetect0);
			writel_relaxed(old1 | l, bank->base +
						bank->regs->leveldetect1);
		}
		writel_relaxed(old0, bank->base + bank->regs->leveldetect0);
		writel_relaxed(old1, bank->base + bank->regs->leveldetect1);
	}
}

static int gpio_omap_cpu_notifier(struct notifier_block *nb,
				  unsigned long cmd, void *v)
{
	struct gpio_bank *bank;
	unsigned long flags;
	int ret = NOTIFY_OK;
	u32 isr, mask;

	bank = container_of(nb, struct gpio_bank, nb);

	raw_spin_lock_irqsave(&bank->lock, flags);
	if (bank->is_suspended)
		goto out_unlock;

	switch (cmd) {
	case CPU_CLUSTER_PM_ENTER:
		mask = omap_get_gpio_irqbank_mask(bank);
		isr = readl_relaxed(bank->base + bank->regs->irqstatus) & mask;
		if (isr) {
			ret = NOTIFY_BAD;
			break;
		}
		omap_gpio_idle(bank, true);
		break;
	case CPU_CLUSTER_PM_ENTER_FAILED:
	case CPU_CLUSTER_PM_EXIT:
		omap_gpio_unidle(bank);
		break;
	}

out_unlock:
	raw_spin_unlock_irqrestore(&bank->lock, flags);

	return ret;
}

static const struct omap_gpio_reg_offs omap2_gpio_regs = {
	.revision =		OMAP24XX_GPIO_REVISION,
	.sysconfig =		OMAP24XX_GPIO_SYSCONFIG,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.direction =		OMAP24XX_GPIO_OE,
	.datain =		OMAP24XX_GPIO_DATAIN,
	.dataout =		OMAP24XX_GPIO_DATAOUT,
	.set_dataout =		OMAP24XX_GPIO_SETDATAOUT,
	.clr_dataout =		OMAP24XX_GPIO_CLEARDATAOUT,
	.irqstatus =		OMAP24XX_GPIO_IRQSTATUS1,
	.irqstatus2 =		OMAP24XX_GPIO_IRQSTATUS2,
	.irqenable =		OMAP24XX_GPIO_IRQENABLE1,
	.irqenable2 =		OMAP24XX_GPIO_IRQENABLE2,
	.set_irqenable =	OMAP24XX_GPIO_SETIRQENABLE1,
	.clr_irqenable =	OMAP24XX_GPIO_CLEARIRQENABLE1,
	.debounce =		OMAP24XX_GPIO_DEBOUNCE_VAL,
	.debounce_en =		OMAP24XX_GPIO_DEBOUNCE_EN,
	.ctrl =			OMAP24XX_GPIO_CTRL,
	.wkup_en =		OMAP24XX_GPIO_WAKE_EN,
	.leveldetect0 =		OMAP24XX_GPIO_LEVELDETECT0,
	.leveldetect1 =		OMAP24XX_GPIO_LEVELDETECT1,
	.risingdetect =		OMAP24XX_GPIO_RISINGDETECT,
	.fallingdetect =	OMAP24XX_GPIO_FALLINGDETECT,
};

<<<<<<< HEAD
static struct omap_gpio_reg_offs omap4_gpio_regs = {
	.revision =		OMAP4_GPIO_REVISION,
=======
static const struct omap_gpio_reg_offs omap4_gpio_regs = {
	.revision =		OMAP4_GPIO_REVISION,
	.sysconfig =		OMAP4_GPIO_SYSCONFIG,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.direction =		OMAP4_GPIO_OE,
	.datain =		OMAP4_GPIO_DATAIN,
	.dataout =		OMAP4_GPIO_DATAOUT,
	.set_dataout =		OMAP4_GPIO_SETDATAOUT,
	.clr_dataout =		OMAP4_GPIO_CLEARDATAOUT,
	.irqstatus =		OMAP4_GPIO_IRQSTATUS0,
	.irqstatus2 =		OMAP4_GPIO_IRQSTATUS1,
<<<<<<< HEAD
=======
	.irqstatus_raw0 =	OMAP4_GPIO_IRQSTATUSRAW0,
	.irqstatus_raw1 =	OMAP4_GPIO_IRQSTATUSRAW1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.irqenable =		OMAP4_GPIO_IRQSTATUSSET0,
	.irqenable2 =		OMAP4_GPIO_IRQSTATUSSET1,
	.set_irqenable =	OMAP4_GPIO_IRQSTATUSSET0,
	.clr_irqenable =	OMAP4_GPIO_IRQSTATUSCLR0,
	.debounce =		OMAP4_GPIO_DEBOUNCINGTIME,
	.debounce_en =		OMAP4_GPIO_DEBOUNCENABLE,
	.ctrl =			OMAP4_GPIO_CTRL,
	.wkup_en =		OMAP4_GPIO_IRQWAKEN0,
	.leveldetect0 =		OMAP4_GPIO_LEVELDETECT0,
	.leveldetect1 =		OMAP4_GPIO_LEVELDETECT1,
	.risingdetect =		OMAP4_GPIO_RISINGDETECT,
	.fallingdetect =	OMAP4_GPIO_FALLINGDETECT,
};

<<<<<<< HEAD
static struct omap_gpio_platform_data omap2_pdata = {
=======
static const struct omap_gpio_platform_data omap2_pdata = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.regs = &omap2_gpio_regs,
	.bank_width = 32,
	.dbck_flag = false,
};

<<<<<<< HEAD
static struct omap_gpio_platform_data omap3_pdata = {
=======
static const struct omap_gpio_platform_data omap3_pdata = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.regs = &omap2_gpio_regs,
	.bank_width = 32,
	.dbck_flag = true,
};

<<<<<<< HEAD
static struct omap_gpio_platform_data omap4_pdata = {
=======
static const struct omap_gpio_platform_data omap4_pdata = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.regs = &omap4_gpio_regs,
	.bank_width = 32,
	.dbck_flag = true,
};

static const struct of_device_id omap_gpio_match[] = {
	{
		.compatible = "ti,omap4-gpio",
		.data = &omap4_pdata,
	},
	{
		.compatible = "ti,omap3-gpio",
		.data = &omap3_pdata,
	},
	{
		.compatible = "ti,omap2-gpio",
		.data = &omap2_pdata,
	},
	{ },
};
MODULE_DEVICE_TABLE(of, omap_gpio_match);
<<<<<<< HEAD
#endif

static struct platform_driver omap_gpio_driver = {
	.probe		= omap_gpio_probe,
	.driver		= {
		.name	= "omap_gpio",
		.pm	= &gpio_pm_ops,
		.of_match_table = of_match_ptr(omap_gpio_match),
=======

static int omap_gpio_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *node = dev->of_node;
	const struct omap_gpio_platform_data *pdata;
	struct gpio_bank *bank;
	int ret;

	pdata = device_get_match_data(dev);

	pdata = pdata ?: dev_get_platdata(dev);
	if (!pdata)
		return -EINVAL;

	bank = devm_kzalloc(dev, sizeof(*bank), GFP_KERNEL);
	if (!bank)
		return -ENOMEM;

	bank->dev = dev;

	bank->irq = platform_get_irq(pdev, 0);
	if (bank->irq < 0)
		return bank->irq;

	bank->chip.parent = dev;
	bank->chip.owner = THIS_MODULE;
	bank->dbck_flag = pdata->dbck_flag;
	bank->stride = pdata->bank_stride;
	bank->width = pdata->bank_width;
	bank->is_mpuio = pdata->is_mpuio;
	bank->non_wakeup_gpios = pdata->non_wakeup_gpios;
	bank->regs = pdata->regs;

	if (node) {
		if (!of_property_read_bool(node, "ti,gpio-always-on"))
			bank->loses_context = true;
	} else {
		bank->loses_context = pdata->loses_context;

		if (bank->loses_context)
			bank->get_context_loss_count =
				pdata->get_context_loss_count;
	}

	if (bank->regs->set_dataout && bank->regs->clr_dataout)
		bank->set_dataout = omap_set_gpio_dataout_reg;
	else
		bank->set_dataout = omap_set_gpio_dataout_mask;

	raw_spin_lock_init(&bank->lock);
	raw_spin_lock_init(&bank->wa_lock);

	/* Static mapping, never released */
	bank->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(bank->base)) {
		return PTR_ERR(bank->base);
	}

	if (bank->dbck_flag) {
		bank->dbck = devm_clk_get(dev, "dbclk");
		if (IS_ERR(bank->dbck)) {
			dev_err(dev,
				"Could not get gpio dbck. Disable debounce\n");
			bank->dbck_flag = false;
		} else {
			clk_prepare(bank->dbck);
		}
	}

	platform_set_drvdata(pdev, bank);

	pm_runtime_enable(dev);
	pm_runtime_get_sync(dev);

	if (bank->is_mpuio)
		omap_mpuio_init(bank);

	omap_gpio_mod_init(bank);

	ret = omap_gpio_chip_init(bank, dev);
	if (ret) {
		pm_runtime_put_sync(dev);
		pm_runtime_disable(dev);
		if (bank->dbck_flag)
			clk_unprepare(bank->dbck);
		return ret;
	}

	omap_gpio_show_rev(bank);

	bank->nb.notifier_call = gpio_omap_cpu_notifier;
	cpu_pm_register_notifier(&bank->nb);

	pm_runtime_put(dev);

	return 0;
}

static void omap_gpio_remove(struct platform_device *pdev)
{
	struct gpio_bank *bank = platform_get_drvdata(pdev);

	cpu_pm_unregister_notifier(&bank->nb);
	gpiochip_remove(&bank->chip);
	pm_runtime_disable(&pdev->dev);
	if (bank->dbck_flag)
		clk_unprepare(bank->dbck);
}

static int __maybe_unused omap_gpio_runtime_suspend(struct device *dev)
{
	struct gpio_bank *bank = dev_get_drvdata(dev);
	unsigned long flags;

	raw_spin_lock_irqsave(&bank->lock, flags);
	omap_gpio_idle(bank, true);
	bank->is_suspended = true;
	raw_spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}

static int __maybe_unused omap_gpio_runtime_resume(struct device *dev)
{
	struct gpio_bank *bank = dev_get_drvdata(dev);
	unsigned long flags;

	raw_spin_lock_irqsave(&bank->lock, flags);
	omap_gpio_unidle(bank);
	bank->is_suspended = false;
	raw_spin_unlock_irqrestore(&bank->lock, flags);

	return 0;
}

static int __maybe_unused omap_gpio_suspend(struct device *dev)
{
	struct gpio_bank *bank = dev_get_drvdata(dev);

	if (bank->is_suspended)
		return 0;

	bank->needs_resume = 1;

	return omap_gpio_runtime_suspend(dev);
}

static int __maybe_unused omap_gpio_resume(struct device *dev)
{
	struct gpio_bank *bank = dev_get_drvdata(dev);

	if (!bank->needs_resume)
		return 0;

	bank->needs_resume = 0;

	return omap_gpio_runtime_resume(dev);
}

static const struct dev_pm_ops gpio_pm_ops = {
	SET_RUNTIME_PM_OPS(omap_gpio_runtime_suspend, omap_gpio_runtime_resume,
									NULL)
	SET_LATE_SYSTEM_SLEEP_PM_OPS(omap_gpio_suspend, omap_gpio_resume)
};

static struct platform_driver omap_gpio_driver = {
	.probe		= omap_gpio_probe,
	.remove_new	= omap_gpio_remove,
	.driver		= {
		.name	= "omap_gpio",
		.pm	= &gpio_pm_ops,
		.of_match_table = omap_gpio_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

/*
 * gpio driver register needs to be done before
 * machine_init functions access gpio APIs.
 * Hence omap_gpio_drv_reg() is a postcore_initcall.
 */
static int __init omap_gpio_drv_reg(void)
{
	return platform_driver_register(&omap_gpio_driver);
}
postcore_initcall(omap_gpio_drv_reg);
<<<<<<< HEAD
=======

static void __exit omap_gpio_exit(void)
{
	platform_driver_unregister(&omap_gpio_driver);
}
module_exit(omap_gpio_exit);

MODULE_DESCRIPTION("omap gpio driver");
MODULE_ALIAS("platform:gpio-omap");
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
