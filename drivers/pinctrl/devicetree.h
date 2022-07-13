<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Internal interface to pinctrl device tree integration
 *
 * Copyright (C) 2012 NVIDIA CORPORATION. All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef CONFIG_OF

void pinctrl_dt_free_maps(struct pinctrl *p);
int pinctrl_dt_to_map(struct pinctrl *p);

#else

static inline int pinctrl_dt_to_map(struct pinctrl *p)
=======
 */

#include <linux/errno.h>

struct device_node;
struct of_phandle_args;

struct pinctrl;
struct pinctrl_dev;

#ifdef CONFIG_OF

void pinctrl_dt_free_maps(struct pinctrl *p);
int pinctrl_dt_to_map(struct pinctrl *p, struct pinctrl_dev *pctldev);

int pinctrl_count_index_with_args(const struct device_node *np,
				  const char *list_name);

int pinctrl_parse_index_with_args(const struct device_node *np,
				  const char *list_name, int index,
				  struct of_phandle_args *out_args);

#else

static inline int pinctrl_dt_to_map(struct pinctrl *p,
				    struct pinctrl_dev *pctldev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

static inline void pinctrl_dt_free_maps(struct pinctrl *p)
{
}

<<<<<<< HEAD
=======
static inline int pinctrl_count_index_with_args(const struct device_node *np,
						const char *list_name)
{
	return -ENODEV;
}

static inline int
pinctrl_parse_index_with_args(const struct device_node *np,
			      const char *list_name, int index,
			      struct of_phandle_args *out_args)
{
	return -ENODEV;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
