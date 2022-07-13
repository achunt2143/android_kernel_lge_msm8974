<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Internal interface between the core pin control system and the
 * pinmux portions
 *
 * Copyright (C) 2011 ST-Ericsson SA
 * Written on behalf of Linaro for ST-Ericsson
 * Based on bits of regulator core, gpio core and clk core
 *
 * Author: Linus Walleij <linus.walleij@linaro.org>
<<<<<<< HEAD
 *
 * License terms: GNU General Public License (GPL) version 2
 */
=======
 */

#include <linux/types.h>

struct dentry;
struct seq_file;

struct pinctrl_dev;
struct pinctrl_gpio_range;
struct pinctrl_map;
struct pinctrl_setting;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PINMUX

int pinmux_check_ops(struct pinctrl_dev *pctldev);

<<<<<<< HEAD
int pinmux_validate_map(struct pinctrl_map const *map, int i);

int pinmux_request_gpio(struct pinctrl_dev *pctldev,
			struct pinctrl_gpio_range *range,
			unsigned pin, unsigned gpio);
void pinmux_free_gpio(struct pinctrl_dev *pctldev, unsigned pin,
		      struct pinctrl_gpio_range *range);
int pinmux_gpio_direction(struct pinctrl_dev *pctldev,
			  struct pinctrl_gpio_range *range,
			  unsigned pin, bool input);

int pinmux_map_to_setting(struct pinctrl_map const *map,
			  struct pinctrl_setting *setting);
void pinmux_free_setting(struct pinctrl_setting const *setting);
int pinmux_enable_setting(struct pinctrl_setting const *setting);
void pinmux_disable_setting(struct pinctrl_setting const *setting);
=======
int pinmux_validate_map(const struct pinctrl_map *map, int i);

bool pinmux_can_be_used_for_gpio(struct pinctrl_dev *pctldev, unsigned int pin);

int pinmux_request_gpio(struct pinctrl_dev *pctldev,
			struct pinctrl_gpio_range *range,
			unsigned int pin, unsigned int gpio);
void pinmux_free_gpio(struct pinctrl_dev *pctldev, unsigned int pin,
		      struct pinctrl_gpio_range *range);
int pinmux_gpio_direction(struct pinctrl_dev *pctldev,
			  struct pinctrl_gpio_range *range,
			  unsigned int pin, bool input);

int pinmux_map_to_setting(const struct pinctrl_map *map,
			  struct pinctrl_setting *setting);
void pinmux_free_setting(const struct pinctrl_setting *setting);
int pinmux_enable_setting(const struct pinctrl_setting *setting);
void pinmux_disable_setting(const struct pinctrl_setting *setting);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#else

static inline int pinmux_check_ops(struct pinctrl_dev *pctldev)
{
	return 0;
}

<<<<<<< HEAD
static inline int pinmux_validate_map(struct pinctrl_map const *map, int i)
=======
static inline int pinmux_validate_map(const struct pinctrl_map *map, int i)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static inline int pinmux_request_gpio(struct pinctrl_dev *pctldev,
			struct pinctrl_gpio_range *range,
			unsigned pin, unsigned gpio)
=======
static inline bool pinmux_can_be_used_for_gpio(struct pinctrl_dev *pctldev,
					       unsigned int pin)
{
	return true;
}

static inline int pinmux_request_gpio(struct pinctrl_dev *pctldev,
			struct pinctrl_gpio_range *range,
			unsigned int pin, unsigned int gpio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

static inline void pinmux_free_gpio(struct pinctrl_dev *pctldev,
<<<<<<< HEAD
				    unsigned pin,
=======
				    unsigned int pin,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    struct pinctrl_gpio_range *range)
{
}

static inline int pinmux_gpio_direction(struct pinctrl_dev *pctldev,
					struct pinctrl_gpio_range *range,
<<<<<<< HEAD
					unsigned pin, bool input)
=======
					unsigned int pin, bool input)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static inline int pinmux_map_to_setting(struct pinctrl_map const *map,
=======
static inline int pinmux_map_to_setting(const struct pinctrl_map *map,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  struct pinctrl_setting *setting)
{
	return 0;
}

<<<<<<< HEAD
static inline void pinmux_free_setting(struct pinctrl_setting const *setting)
{
}

static inline int pinmux_enable_setting(struct pinctrl_setting const *setting)
=======
static inline void pinmux_free_setting(const struct pinctrl_setting *setting)
{
}

static inline int pinmux_enable_setting(const struct pinctrl_setting *setting)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static inline void pinmux_disable_setting(
			struct pinctrl_setting const *setting)
=======
static inline void pinmux_disable_setting(const struct pinctrl_setting *setting)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

#endif

#if defined(CONFIG_PINMUX) && defined(CONFIG_DEBUG_FS)

<<<<<<< HEAD
void pinmux_show_map(struct seq_file *s, struct pinctrl_map const *map);
void pinmux_show_setting(struct seq_file *s,
			 struct pinctrl_setting const *setting);
=======
void pinmux_show_map(struct seq_file *s, const struct pinctrl_map *map);
void pinmux_show_setting(struct seq_file *s,
			 const struct pinctrl_setting *setting);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void pinmux_init_device_debugfs(struct dentry *devroot,
				struct pinctrl_dev *pctldev);

#else

static inline void pinmux_show_map(struct seq_file *s,
<<<<<<< HEAD
				   struct pinctrl_map const *map)
=======
				   const struct pinctrl_map *map)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

static inline void pinmux_show_setting(struct seq_file *s,
<<<<<<< HEAD
				       struct pinctrl_setting const *setting)
=======
				       const struct pinctrl_setting *setting)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

static inline void pinmux_init_device_debugfs(struct dentry *devroot,
					      struct pinctrl_dev *pctldev)
{
}

#endif
<<<<<<< HEAD
=======

#ifdef CONFIG_GENERIC_PINMUX_FUNCTIONS

/**
 * struct function_desc - generic function descriptor
 * @name: name of the function
 * @group_names: array of pin group names
 * @num_group_names: number of pin group names
 * @data: pin controller driver specific data
 */
struct function_desc {
	const char *name;
	const char * const *group_names;
	int num_group_names;
	void *data;
};

int pinmux_generic_get_function_count(struct pinctrl_dev *pctldev);

const char *
pinmux_generic_get_function_name(struct pinctrl_dev *pctldev,
				 unsigned int selector);

int pinmux_generic_get_function_groups(struct pinctrl_dev *pctldev,
				       unsigned int selector,
				       const char * const **groups,
				       unsigned int * const num_groups);

struct function_desc *pinmux_generic_get_function(struct pinctrl_dev *pctldev,
						  unsigned int selector);

int pinmux_generic_add_function(struct pinctrl_dev *pctldev,
				const char *name,
				const char * const *groups,
				unsigned int const num_groups,
				void *data);

int pinmux_generic_remove_function(struct pinctrl_dev *pctldev,
				   unsigned int selector);

void pinmux_generic_free_functions(struct pinctrl_dev *pctldev);

#else

static inline void pinmux_generic_free_functions(struct pinctrl_dev *pctldev)
{
}

#endif /* CONFIG_GENERIC_PINMUX_FUNCTIONS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
