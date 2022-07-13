<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Consumer interface the pin control subsystem
 *
 * Copyright (C) 2012 ST-Ericsson SA
 * Written on behalf of Linaro for ST-Ericsson
 * Based on bits of regulator core, gpio core and clk core
 *
 * Author: Linus Walleij <linus.walleij@linaro.org>
<<<<<<< HEAD
 *
 * License terms: GNU General Public License (GPL) version 2
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __LINUX_PINCTRL_CONSUMER_H
#define __LINUX_PINCTRL_CONSUMER_H

#include <linux/err.h>
<<<<<<< HEAD
#include <linux/list.h>
#include <linux/seq_file.h>
#include "pinctrl-state.h"
=======
#include <linux/types.h>

#include <linux/pinctrl/pinctrl-state.h>

struct device;
struct gpio_chip;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* This struct is private to the core and should be regarded as a cookie */
struct pinctrl;
struct pinctrl_state;
<<<<<<< HEAD
struct device;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PINCTRL

/* External interface to pin control */
<<<<<<< HEAD
extern int pinctrl_request_gpio(unsigned gpio);
extern void pinctrl_free_gpio(unsigned gpio);
extern int pinctrl_gpio_direction_input(unsigned gpio);
extern int pinctrl_gpio_direction_output(unsigned gpio);

extern struct pinctrl * __must_check pinctrl_get(struct device *dev);
extern void pinctrl_put(struct pinctrl *p);
extern struct pinctrl_state * __must_check pinctrl_lookup_state(
							struct pinctrl *p,
							const char *name);
extern int pinctrl_select_state(struct pinctrl *p, struct pinctrl_state *s);

extern struct pinctrl * __must_check devm_pinctrl_get(struct device *dev);
extern void devm_pinctrl_put(struct pinctrl *p);

#else /* !CONFIG_PINCTRL */

static inline int pinctrl_request_gpio(unsigned gpio)
=======
bool pinctrl_gpio_can_use_line(struct gpio_chip *gc, unsigned int offset);
int pinctrl_gpio_request(struct gpio_chip *gc, unsigned int offset);
void pinctrl_gpio_free(struct gpio_chip *gc, unsigned int offset);
int pinctrl_gpio_direction_input(struct gpio_chip *gc,
				 unsigned int offset);
int pinctrl_gpio_direction_output(struct gpio_chip *gc,
				  unsigned int offset);
int pinctrl_gpio_set_config(struct gpio_chip *gc, unsigned int offset,
				unsigned long config);

struct pinctrl * __must_check pinctrl_get(struct device *dev);
void pinctrl_put(struct pinctrl *p);
struct pinctrl_state * __must_check pinctrl_lookup_state(struct pinctrl *p,
							 const char *name);
int pinctrl_select_state(struct pinctrl *p, struct pinctrl_state *s);

struct pinctrl * __must_check devm_pinctrl_get(struct device *dev);
void devm_pinctrl_put(struct pinctrl *p);
int pinctrl_select_default_state(struct device *dev);

#ifdef CONFIG_PM
int pinctrl_pm_select_default_state(struct device *dev);
int pinctrl_pm_select_sleep_state(struct device *dev);
int pinctrl_pm_select_idle_state(struct device *dev);
#else
static inline int pinctrl_pm_select_default_state(struct device *dev)
{
	return 0;
}
static inline int pinctrl_pm_select_sleep_state(struct device *dev)
{
	return 0;
}
static inline int pinctrl_pm_select_idle_state(struct device *dev)
{
	return 0;
}
#endif

#else /* !CONFIG_PINCTRL */

static inline bool
pinctrl_gpio_can_use_line(struct gpio_chip *gc, unsigned int offset)
{
	return true;
}

static inline int
pinctrl_gpio_request(struct gpio_chip *gc, unsigned int offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static inline void pinctrl_free_gpio(unsigned gpio)
{
}

static inline int pinctrl_gpio_direction_input(unsigned gpio)
=======
static inline void
pinctrl_gpio_free(struct gpio_chip *gc, unsigned int offset)
{
}

static inline int
pinctrl_gpio_direction_input(struct gpio_chip *gc, unsigned int offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static inline int pinctrl_gpio_direction_output(unsigned gpio)
=======
static inline int
pinctrl_gpio_direction_output(struct gpio_chip *gc, unsigned int offset)
{
	return 0;
}

static inline int
pinctrl_gpio_set_config(struct gpio_chip *gc, unsigned int offset,
			    unsigned long config)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

static inline struct pinctrl * __must_check pinctrl_get(struct device *dev)
{
	return NULL;
}

static inline void pinctrl_put(struct pinctrl *p)
{
}

<<<<<<< HEAD
static inline struct pinctrl_state * __must_check pinctrl_lookup_state(
							struct pinctrl *p,
							const char *name)
=======
static inline struct pinctrl_state * __must_check pinctrl_lookup_state(struct pinctrl *p,
								       const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return NULL;
}

static inline int pinctrl_select_state(struct pinctrl *p,
				       struct pinctrl_state *s)
{
	return 0;
}

static inline struct pinctrl * __must_check devm_pinctrl_get(struct device *dev)
{
	return NULL;
}

static inline void devm_pinctrl_put(struct pinctrl *p)
{
}

<<<<<<< HEAD
#endif /* CONFIG_PINCTRL */

static inline struct pinctrl * __must_check pinctrl_get_select(
					struct device *dev, const char *name)
=======
static inline int pinctrl_select_default_state(struct device *dev)
{
	return 0;
}

static inline int pinctrl_pm_select_default_state(struct device *dev)
{
	return 0;
}

static inline int pinctrl_pm_select_sleep_state(struct device *dev)
{
	return 0;
}

static inline int pinctrl_pm_select_idle_state(struct device *dev)
{
	return 0;
}

#endif /* CONFIG_PINCTRL */

static inline struct pinctrl * __must_check pinctrl_get_select(struct device *dev,
							       const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pinctrl *p;
	struct pinctrl_state *s;
	int ret;

	p = pinctrl_get(dev);
	if (IS_ERR(p))
		return p;

	s = pinctrl_lookup_state(p, name);
	if (IS_ERR(s)) {
		pinctrl_put(p);
<<<<<<< HEAD
		return ERR_PTR(PTR_ERR(s));
=======
		return ERR_CAST(s);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ret = pinctrl_select_state(p, s);
	if (ret < 0) {
		pinctrl_put(p);
		return ERR_PTR(ret);
	}

	return p;
}

<<<<<<< HEAD
static inline struct pinctrl * __must_check pinctrl_get_select_default(
					struct device *dev)
=======
static inline struct pinctrl * __must_check pinctrl_get_select_default(struct device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return pinctrl_get_select(dev, PINCTRL_STATE_DEFAULT);
}

<<<<<<< HEAD
static inline struct pinctrl * __must_check devm_pinctrl_get_select(
					struct device *dev, const char *name)
=======
static inline struct pinctrl * __must_check devm_pinctrl_get_select(struct device *dev,
								    const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pinctrl *p;
	struct pinctrl_state *s;
	int ret;

	p = devm_pinctrl_get(dev);
	if (IS_ERR(p))
		return p;

	s = pinctrl_lookup_state(p, name);
	if (IS_ERR(s)) {
		devm_pinctrl_put(p);
		return ERR_CAST(s);
	}

	ret = pinctrl_select_state(p, s);
	if (ret < 0) {
		devm_pinctrl_put(p);
		return ERR_PTR(ret);
	}

	return p;
}

<<<<<<< HEAD
static inline struct pinctrl * __must_check devm_pinctrl_get_select_default(
					struct device *dev)
=======
static inline struct pinctrl * __must_check devm_pinctrl_get_select_default(struct device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return devm_pinctrl_get_select(dev, PINCTRL_STATE_DEFAULT);
}

<<<<<<< HEAD
#ifdef CONFIG_PINCONF

extern int pin_config_get(const char *dev_name, const char *name,
			  unsigned long *config);
extern int pin_config_set(const char *dev_name, const char *name,
			  unsigned long config);
extern int pin_config_group_get(const char *dev_name,
				const char *pin_group,
				unsigned long *config);
extern int pin_config_group_set(const char *dev_name,
				const char *pin_group,
				unsigned long config);

#else

static inline int pin_config_get(const char *dev_name, const char *name,
				 unsigned long *config)
{
	return 0;
}

static inline int pin_config_set(const char *dev_name, const char *name,
				 unsigned long config)
{
	return 0;
}

static inline int pin_config_group_get(const char *dev_name,
				       const char *pin_group,
				       unsigned long *config)
{
	return 0;
}

static inline int pin_config_group_set(const char *dev_name,
				       const char *pin_group,
				       unsigned long config)
{
	return 0;
}

#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __LINUX_PINCTRL_CONSUMER_H */
