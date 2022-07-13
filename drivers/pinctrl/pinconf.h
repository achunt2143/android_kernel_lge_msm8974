<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Internal interface between the core pin control system and the
 * pin config portions
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

#ifdef CONFIG_PINCONF

int pinconf_check_ops(struct pinctrl_dev *pctldev);
int pinconf_validate_map(struct pinctrl_map const *map, int i);
int pinconf_map_to_setting(struct pinctrl_map const *map,
			  struct pinctrl_setting *setting);
void pinconf_free_setting(struct pinctrl_setting const *setting);
int pinconf_apply_setting(struct pinctrl_setting const *setting);
=======
 */

#include <linux/errno.h>

struct dentry;
struct device_node;
struct seq_file;

struct pinctrl_dev;
struct pinctrl_map;
struct pinctrl_setting;

#ifdef CONFIG_PINCONF

int pinconf_check_ops(struct pinctrl_dev *pctldev);
int pinconf_validate_map(const struct pinctrl_map *map, int i);
int pinconf_map_to_setting(const struct pinctrl_map *map,
			  struct pinctrl_setting *setting);
void pinconf_free_setting(const struct pinctrl_setting *setting);
int pinconf_apply_setting(const struct pinctrl_setting *setting);

int pinconf_set_config(struct pinctrl_dev *pctldev, unsigned int pin,
		       unsigned long *configs, size_t nconfigs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * You will only be interested in these if you're using PINCONF
 * so don't supply any stubs for these.
 */
<<<<<<< HEAD
int pin_config_get_for_pin(struct pinctrl_dev *pctldev, unsigned pin,
=======
int pin_config_get_for_pin(struct pinctrl_dev *pctldev, unsigned int pin,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   unsigned long *config);
int pin_config_group_get(const char *dev_name, const char *pin_group,
			 unsigned long *config);

#else

static inline int pinconf_check_ops(struct pinctrl_dev *pctldev)
{
	return 0;
}

<<<<<<< HEAD
static inline int pinconf_validate_map(struct pinctrl_map const *map, int i)
=======
static inline int pinconf_validate_map(const struct pinctrl_map *map, int i)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static inline int pinconf_map_to_setting(struct pinctrl_map const *map,
=======
static inline int pinconf_map_to_setting(const struct pinctrl_map *map,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  struct pinctrl_setting *setting)
{
	return 0;
}

<<<<<<< HEAD
static inline void pinconf_free_setting(struct pinctrl_setting const *setting)
{
}

static inline int pinconf_apply_setting(struct pinctrl_setting const *setting)
=======
static inline void pinconf_free_setting(const struct pinctrl_setting *setting)
{
}

static inline int pinconf_apply_setting(const struct pinctrl_setting *setting)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
=======
static inline int pinconf_set_config(struct pinctrl_dev *pctldev, unsigned int pin,
				     unsigned long *configs, size_t nconfigs)
{
	return -ENOTSUPP;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#if defined(CONFIG_PINCONF) && defined(CONFIG_DEBUG_FS)

<<<<<<< HEAD
void pinconf_show_map(struct seq_file *s, struct pinctrl_map const *map);
void pinconf_show_setting(struct seq_file *s,
			  struct pinctrl_setting const *setting);
=======
void pinconf_show_map(struct seq_file *s, const struct pinctrl_map *map);
void pinconf_show_setting(struct seq_file *s,
			  const struct pinctrl_setting *setting);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void pinconf_init_device_debugfs(struct dentry *devroot,
				 struct pinctrl_dev *pctldev);

#else

static inline void pinconf_show_map(struct seq_file *s,
<<<<<<< HEAD
				    struct pinctrl_map const *map)
=======
				    const struct pinctrl_map *map)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

static inline void pinconf_show_setting(struct seq_file *s,
<<<<<<< HEAD
			  struct pinctrl_setting const *setting)
=======
					const struct pinctrl_setting *setting)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

static inline void pinconf_init_device_debugfs(struct dentry *devroot,
					       struct pinctrl_dev *pctldev)
{
}

#endif

/*
 * The following functions are available if the driver uses the generic
 * pin config.
 */

#if defined(CONFIG_GENERIC_PINCONF) && defined(CONFIG_DEBUG_FS)

<<<<<<< HEAD
void pinconf_generic_dump_pin(struct pinctrl_dev *pctldev,
			      struct seq_file *s, unsigned pin);

void pinconf_generic_dump_group(struct pinctrl_dev *pctldev,
			      struct seq_file *s, const char *gname);

#else

static inline void pinconf_generic_dump_pin(struct pinctrl_dev *pctldev,
					    struct seq_file *s,
					    unsigned pin)
=======
void pinconf_generic_dump_pins(struct pinctrl_dev *pctldev,
			       struct seq_file *s, const char *gname,
			       unsigned int pin);

void pinconf_generic_dump_config(struct pinctrl_dev *pctldev,
				 struct seq_file *s, unsigned long config);
#else

static inline void pinconf_generic_dump_pins(struct pinctrl_dev *pctldev,
					     struct seq_file *s,
					     const char *gname, unsigned int pin)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return;
}

<<<<<<< HEAD
static inline void pinconf_generic_dump_group(struct pinctrl_dev *pctldev,
					      struct seq_file *s,
					      const char *gname)
{
	return;
}

=======
static inline void pinconf_generic_dump_config(struct pinctrl_dev *pctldev,
					       struct seq_file *s,
					       unsigned long config)
{
	return;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#if defined(CONFIG_GENERIC_PINCONF) && defined(CONFIG_OF)
int pinconf_generic_parse_dt_config(struct device_node *np,
<<<<<<< HEAD
=======
				    struct pinctrl_dev *pctldev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    unsigned long **configs,
				    unsigned int *nconfigs);
#endif
