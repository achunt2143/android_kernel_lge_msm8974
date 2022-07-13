<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2010-2011 Canonical Ltd <jeremy.kerr@canonical.com>
 * Copyright (C) 2011-2012 Mike Turquette, Linaro Ltd <mturquette@linaro.org>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Gated clock implementation
 */

#include <linux/clk-provider.h>
<<<<<<< HEAD
=======
#include <linux/device.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/string.h>

/**
<<<<<<< HEAD
 * DOC: basic gatable clock which can gate and ungate it's ouput
=======
 * DOC: basic gatable clock which can gate and ungate its output
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Traits of this clock:
 * prepare - clk_(un)prepare only ensures parent is (un)prepared
 * enable - clk_enable and clk_disable are functional & control gating
 * rate - inherits rate from parent.  No clk_set_rate support
 * parent - fixed parent.  No clk_set_parent support
 */

<<<<<<< HEAD
#define to_clk_gate(_hw) container_of(_hw, struct clk_gate, hw)

static void clk_gate_set_bit(struct clk_gate *gate)
{
	u32 reg;
	unsigned long flags = 0;

	if (gate->lock)
		spin_lock_irqsave(gate->lock, flags);

	reg = readl(gate->reg);
	reg |= BIT(gate->bit_idx);
	writel(reg, gate->reg);

	if (gate->lock)
		spin_unlock_irqrestore(gate->lock, flags);
}

static void clk_gate_clear_bit(struct clk_gate *gate)
{
	u32 reg;
	unsigned long flags = 0;

	if (gate->lock)
		spin_lock_irqsave(gate->lock, flags);

	reg = readl(gate->reg);
	reg &= ~BIT(gate->bit_idx);
	writel(reg, gate->reg);

	if (gate->lock)
		spin_unlock_irqrestore(gate->lock, flags);
=======
static inline u32 clk_gate_readl(struct clk_gate *gate)
{
	if (gate->flags & CLK_GATE_BIG_ENDIAN)
		return ioread32be(gate->reg);

	return readl(gate->reg);
}

static inline void clk_gate_writel(struct clk_gate *gate, u32 val)
{
	if (gate->flags & CLK_GATE_BIG_ENDIAN)
		iowrite32be(val, gate->reg);
	else
		writel(val, gate->reg);
}

/*
 * It works on following logic:
 *
 * For enabling clock, enable = 1
 *	set2dis = 1	-> clear bit	-> set = 0
 *	set2dis = 0	-> set bit	-> set = 1
 *
 * For disabling clock, enable = 0
 *	set2dis = 1	-> set bit	-> set = 1
 *	set2dis = 0	-> clear bit	-> set = 0
 *
 * So, result is always: enable xor set2dis.
 */
static void clk_gate_endisable(struct clk_hw *hw, int enable)
{
	struct clk_gate *gate = to_clk_gate(hw);
	int set = gate->flags & CLK_GATE_SET_TO_DISABLE ? 1 : 0;
	unsigned long flags;
	u32 reg;

	set ^= enable;

	if (gate->lock)
		spin_lock_irqsave(gate->lock, flags);
	else
		__acquire(gate->lock);

	if (gate->flags & CLK_GATE_HIWORD_MASK) {
		reg = BIT(gate->bit_idx + 16);
		if (set)
			reg |= BIT(gate->bit_idx);
	} else {
		reg = clk_gate_readl(gate);

		if (set)
			reg |= BIT(gate->bit_idx);
		else
			reg &= ~BIT(gate->bit_idx);
	}

	clk_gate_writel(gate, reg);

	if (gate->lock)
		spin_unlock_irqrestore(gate->lock, flags);
	else
		__release(gate->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int clk_gate_enable(struct clk_hw *hw)
{
<<<<<<< HEAD
	struct clk_gate *gate = to_clk_gate(hw);

	if (gate->flags & CLK_GATE_SET_TO_DISABLE)
		clk_gate_clear_bit(gate);
	else
		clk_gate_set_bit(gate);

	return 0;
}
EXPORT_SYMBOL_GPL(clk_gate_enable);

static void clk_gate_disable(struct clk_hw *hw)
{
	struct clk_gate *gate = to_clk_gate(hw);

	if (gate->flags & CLK_GATE_SET_TO_DISABLE)
		clk_gate_set_bit(gate);
	else
		clk_gate_clear_bit(gate);
}
EXPORT_SYMBOL_GPL(clk_gate_disable);

static int clk_gate_is_enabled(struct clk_hw *hw)
=======
	clk_gate_endisable(hw, 1);

	return 0;
}

static void clk_gate_disable(struct clk_hw *hw)
{
	clk_gate_endisable(hw, 0);
}

int clk_gate_is_enabled(struct clk_hw *hw)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 reg;
	struct clk_gate *gate = to_clk_gate(hw);

<<<<<<< HEAD
	reg = readl(gate->reg);
=======
	reg = clk_gate_readl(gate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* if a set bit disables this clk, flip it before masking */
	if (gate->flags & CLK_GATE_SET_TO_DISABLE)
		reg ^= BIT(gate->bit_idx);

	reg &= BIT(gate->bit_idx);

	return reg ? 1 : 0;
}
EXPORT_SYMBOL_GPL(clk_gate_is_enabled);

<<<<<<< HEAD
struct clk_ops clk_gate_ops = {
=======
const struct clk_ops clk_gate_ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.enable = clk_gate_enable,
	.disable = clk_gate_disable,
	.is_enabled = clk_gate_is_enabled,
};
EXPORT_SYMBOL_GPL(clk_gate_ops);

<<<<<<< HEAD
struct clk *clk_register_gate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
=======
struct clk_hw *__clk_hw_register_gate(struct device *dev,
		struct device_node *np, const char *name,
		const char *parent_name, const struct clk_hw *parent_hw,
		const struct clk_parent_data *parent_data,
		unsigned long flags,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		void __iomem *reg, u8 bit_idx,
		u8 clk_gate_flags, spinlock_t *lock)
{
	struct clk_gate *gate;
<<<<<<< HEAD
	struct clk *clk;

	gate = kzalloc(sizeof(struct clk_gate), GFP_KERNEL);

	if (!gate) {
		pr_err("%s: could not allocate gated clk\n", __func__);
		return NULL;
	}

=======
	struct clk_hw *hw;
	struct clk_init_data init = {};
	int ret = -EINVAL;

	if (clk_gate_flags & CLK_GATE_HIWORD_MASK) {
		if (bit_idx > 15) {
			pr_err("gate bit exceeds LOWORD field\n");
			return ERR_PTR(-EINVAL);
		}
	}

	/* allocate the gate */
	gate = kzalloc(sizeof(*gate), GFP_KERNEL);
	if (!gate)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &clk_gate_ops;
	init.flags = flags;
	init.parent_names = parent_name ? &parent_name : NULL;
	init.parent_hws = parent_hw ? &parent_hw : NULL;
	init.parent_data = parent_data;
	if (parent_name || parent_hw || parent_data)
		init.num_parents = 1;
	else
		init.num_parents = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* struct clk_gate assignments */
	gate->reg = reg;
	gate->bit_idx = bit_idx;
	gate->flags = clk_gate_flags;
	gate->lock = lock;
<<<<<<< HEAD

	if (parent_name) {
		gate->parent[0] = kstrdup(parent_name, GFP_KERNEL);
		if (!gate->parent[0])
			goto out;
	}

	clk = clk_register(dev, name,
			&clk_gate_ops, &gate->hw,
			gate->parent,
			(parent_name ? 1 : 0),
			flags);
	if (clk)
		return clk;
out:
	kfree(gate->parent[0]);
	kfree(gate);

	return NULL;
}
=======
	gate->hw.init = &init;

	hw = &gate->hw;
	if (dev || !np)
		ret = clk_hw_register(dev, hw);
	else if (np)
		ret = of_clk_hw_register(np, hw);
	if (ret) {
		kfree(gate);
		hw = ERR_PTR(ret);
	}

	return hw;

}
EXPORT_SYMBOL_GPL(__clk_hw_register_gate);

struct clk *clk_register_gate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 bit_idx,
		u8 clk_gate_flags, spinlock_t *lock)
{
	struct clk_hw *hw;

	hw = clk_hw_register_gate(dev, name, parent_name, flags, reg,
				  bit_idx, clk_gate_flags, lock);
	if (IS_ERR(hw))
		return ERR_CAST(hw);
	return hw->clk;
}
EXPORT_SYMBOL_GPL(clk_register_gate);

void clk_unregister_gate(struct clk *clk)
{
	struct clk_gate *gate;
	struct clk_hw *hw;

	hw = __clk_get_hw(clk);
	if (!hw)
		return;

	gate = to_clk_gate(hw);

	clk_unregister(clk);
	kfree(gate);
}
EXPORT_SYMBOL_GPL(clk_unregister_gate);

void clk_hw_unregister_gate(struct clk_hw *hw)
{
	struct clk_gate *gate;

	gate = to_clk_gate(hw);

	clk_hw_unregister(hw);
	kfree(gate);
}
EXPORT_SYMBOL_GPL(clk_hw_unregister_gate);

static void devm_clk_hw_release_gate(struct device *dev, void *res)
{
	clk_hw_unregister_gate(*(struct clk_hw **)res);
}

struct clk_hw *__devm_clk_hw_register_gate(struct device *dev,
		struct device_node *np, const char *name,
		const char *parent_name, const struct clk_hw *parent_hw,
		const struct clk_parent_data *parent_data,
		unsigned long flags,
		void __iomem *reg, u8 bit_idx,
		u8 clk_gate_flags, spinlock_t *lock)
{
	struct clk_hw **ptr, *hw;

	ptr = devres_alloc(devm_clk_hw_release_gate, sizeof(*ptr), GFP_KERNEL);
	if (!ptr)
		return ERR_PTR(-ENOMEM);

	hw = __clk_hw_register_gate(dev, np, name, parent_name, parent_hw,
				    parent_data, flags, reg, bit_idx,
				    clk_gate_flags, lock);

	if (!IS_ERR(hw)) {
		*ptr = hw;
		devres_add(dev, ptr);
	} else {
		devres_free(ptr);
	}

	return hw;
}
EXPORT_SYMBOL_GPL(__devm_clk_hw_register_gate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
