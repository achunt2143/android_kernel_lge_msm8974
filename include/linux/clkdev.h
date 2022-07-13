<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  include/linux/clkdev.h
 *
 *  Copyright (C) 2008 Russell King.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Helper for the clk API to assist looking up a struct clk.
 */
#ifndef __CLKDEV_H
#define __CLKDEV_H

<<<<<<< HEAD
#include <asm/clkdev.h>

struct clk;
=======
#include <linux/slab.h>

struct clk;
struct clk_hw;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct device;

struct clk_lookup {
	struct list_head	node;
	const char		*dev_id;
	const char		*con_id;
	struct clk		*clk;
<<<<<<< HEAD
=======
	struct clk_hw		*clk_hw;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define CLKDEV_INIT(d, n, c)	\
	{			\
		.dev_id = d,	\
		.con_id = n,	\
		.clk = c,	\
	}

<<<<<<< HEAD
struct clk_lookup *clkdev_alloc(struct clk *clk, const char *con_id,
	const char *dev_fmt, ...);

void clkdev_add(struct clk_lookup *cl);
void clkdev_drop(struct clk_lookup *cl);

void clkdev_add_table(struct clk_lookup *, size_t);
int clk_add_alias(const char *, const char *, char *, struct device *);

=======
void clkdev_add(struct clk_lookup *cl);
void clkdev_drop(struct clk_lookup *cl);

struct clk_lookup *clkdev_create(struct clk *clk, const char *con_id,
	const char *dev_fmt, ...) __printf(3, 4);
struct clk_lookup *clkdev_hw_create(struct clk_hw *hw, const char *con_id,
	const char *dev_fmt, ...) __printf(3, 4);

void clkdev_add_table(struct clk_lookup *, size_t);
int clk_add_alias(const char *, const char *, const char *, struct device *);

int clk_register_clkdev(struct clk *, const char *, const char *);
int clk_hw_register_clkdev(struct clk_hw *, const char *, const char *);

int devm_clk_hw_register_clkdev(struct device *dev, struct clk_hw *hw,
				const char *con_id, const char *dev_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
