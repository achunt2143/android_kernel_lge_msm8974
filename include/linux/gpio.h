<<<<<<< HEAD
#ifndef __LINUX_GPIO_H
#define __LINUX_GPIO_H

/* see Documentation/gpio.txt */
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * <linux/gpio.h>
 *
 * This is the LEGACY GPIO bulk include file, including legacy APIs. It is
 * used for GPIO drivers still referencing the global GPIO numberspace,
 * and should not be included in new code.
 *
 * If you're implementing a GPIO driver, only include <linux/gpio/driver.h>
 * If you're implementing a GPIO consumer, only include <linux/gpio/consumer.h>
 */
#ifndef __LINUX_GPIO_H
#define __LINUX_GPIO_H

#include <linux/types.h>

struct device;

/* see Documentation/driver-api/gpio/legacy.rst */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* make these flag values available regardless of GPIO kconfig options */
#define GPIOF_DIR_OUT	(0 << 0)
#define GPIOF_DIR_IN	(1 << 0)

#define GPIOF_INIT_LOW	(0 << 1)
#define GPIOF_INIT_HIGH	(1 << 1)

#define GPIOF_IN		(GPIOF_DIR_IN)
#define GPIOF_OUT_INIT_LOW	(GPIOF_DIR_OUT | GPIOF_INIT_LOW)
#define GPIOF_OUT_INIT_HIGH	(GPIOF_DIR_OUT | GPIOF_INIT_HIGH)

<<<<<<< HEAD
/* Gpio pin is open drain */
#define GPIOF_OPEN_DRAIN	(1 << 2)

/* Gpio pin is open source */
#define GPIOF_OPEN_SOURCE	(1 << 3)
=======
/* Gpio pin is active-low */
#define GPIOF_ACTIVE_LOW        (1 << 2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * struct gpio - a structure describing a GPIO with configuration
 * @gpio:	the GPIO number
 * @flags:	GPIO configuration as specified by GPIOF_*
 * @label:	a literal description string of this GPIO
 */
struct gpio {
	unsigned	gpio;
	unsigned long	flags;
	const char	*label;
};

<<<<<<< HEAD
#ifdef CONFIG_GENERIC_GPIO
#include <asm/gpio.h>

#else

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/bug.h>

struct device;
struct gpio_chip;
=======
#ifdef CONFIG_GPIOLIB

#include <linux/gpio/consumer.h>

/*
 * "valid" GPIO numbers are nonnegative and may be passed to
 * setup routines like gpio_request().  Only some valid numbers
 * can successfully be requested and used.
 *
 * Invalid GPIO numbers are useful for indicating no-such-GPIO in
 * platform data and other tables.
 */
static inline bool gpio_is_valid(int number)
{
	/* only non-negative numbers are valid */
	return number >= 0;
}

/*
 * Platforms may implement their GPIO interface with library code,
 * at a small performance cost for non-inlined operations and some
 * extra memory (for code and for per-GPIO table entries).
 */

/*
 * At the end we want all GPIOs to be dynamically allocated from 0.
 * However, some legacy drivers still perform fixed allocation.
 * Until they are all fixed, leave 0-512 space for them.
 */
#define GPIO_DYNAMIC_BASE	512

/* Always use the library code for GPIO management calls,
 * or when sleeping may be involved.
 */
int gpio_request(unsigned gpio, const char *label);
void gpio_free(unsigned gpio);

static inline int gpio_direction_input(unsigned gpio)
{
	return gpiod_direction_input(gpio_to_desc(gpio));
}
static inline int gpio_direction_output(unsigned gpio, int value)
{
	return gpiod_direction_output_raw(gpio_to_desc(gpio), value);
}

static inline int gpio_get_value_cansleep(unsigned gpio)
{
	return gpiod_get_raw_value_cansleep(gpio_to_desc(gpio));
}
static inline void gpio_set_value_cansleep(unsigned gpio, int value)
{
	return gpiod_set_raw_value_cansleep(gpio_to_desc(gpio), value);
}

static inline int gpio_get_value(unsigned gpio)
{
	return gpiod_get_raw_value(gpio_to_desc(gpio));
}
static inline void gpio_set_value(unsigned gpio, int value)
{
	return gpiod_set_raw_value(gpio_to_desc(gpio), value);
}

static inline int gpio_to_irq(unsigned gpio)
{
	return gpiod_to_irq(gpio_to_desc(gpio));
}

int gpio_request_one(unsigned gpio, unsigned long flags, const char *label);
int gpio_request_array(const struct gpio *array, size_t num);
void gpio_free_array(const struct gpio *array, size_t num);

/* CONFIG_GPIOLIB: bindings for managed devices that want to request gpios */

int devm_gpio_request(struct device *dev, unsigned gpio, const char *label);
int devm_gpio_request_one(struct device *dev, unsigned gpio,
			  unsigned long flags, const char *label);

#else /* ! CONFIG_GPIOLIB */

#include <linux/kernel.h>

#include <asm/bug.h>
#include <asm/errno.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline bool gpio_is_valid(int number)
{
	return false;
}

static inline int gpio_request(unsigned gpio, const char *label)
{
	return -ENOSYS;
}

static inline int gpio_request_one(unsigned gpio,
					unsigned long flags, const char *label)
{
	return -ENOSYS;
}

static inline int gpio_request_array(const struct gpio *array, size_t num)
{
	return -ENOSYS;
}

static inline void gpio_free(unsigned gpio)
{
	might_sleep();

	/* GPIO can never have been requested */
	WARN_ON(1);
}

static inline void gpio_free_array(const struct gpio *array, size_t num)
{
	might_sleep();

	/* GPIO can never have been requested */
	WARN_ON(1);
}

static inline int gpio_direction_input(unsigned gpio)
{
	return -ENOSYS;
}

static inline int gpio_direction_output(unsigned gpio, int value)
{
	return -ENOSYS;
}

<<<<<<< HEAD
static inline int gpio_set_debounce(unsigned gpio, unsigned debounce)
{
	return -ENOSYS;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int gpio_get_value(unsigned gpio)
{
	/* GPIO can never have been requested or set as {in,out}put */
	WARN_ON(1);
	return 0;
}

static inline void gpio_set_value(unsigned gpio, int value)
{
	/* GPIO can never have been requested or set as output */
	WARN_ON(1);
}

<<<<<<< HEAD
static inline int gpio_cansleep(unsigned gpio)
{
	/* GPIO can never have been requested or set as {in,out}put */
	WARN_ON(1);
	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int gpio_get_value_cansleep(unsigned gpio)
{
	/* GPIO can never have been requested or set as {in,out}put */
	WARN_ON(1);
	return 0;
}

static inline void gpio_set_value_cansleep(unsigned gpio, int value)
{
	/* GPIO can never have been requested or set as output */
	WARN_ON(1);
}

<<<<<<< HEAD
static inline int gpio_export(unsigned gpio, bool direction_may_change)
{
	/* GPIO can never have been requested or set as {in,out}put */
	WARN_ON(1);
	return -EINVAL;
}

static inline int gpio_export_link(struct device *dev, const char *name,
				unsigned gpio)
{
	/* GPIO can never have been exported */
	WARN_ON(1);
	return -EINVAL;
}

static inline int gpio_sysfs_set_active_low(unsigned gpio, int value)
{
	/* GPIO can never have been requested */
	WARN_ON(1);
	return -EINVAL;
}

static inline void gpio_unexport(unsigned gpio)
{
	/* GPIO can never have been exported */
	WARN_ON(1);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int gpio_to_irq(unsigned gpio)
{
	/* GPIO can never have been requested or set as input */
	WARN_ON(1);
	return -EINVAL;
}

<<<<<<< HEAD
static inline int irq_to_gpio(unsigned irq)
{
	/* irq can never have been returned from gpio_to_irq() */
=======
static inline int devm_gpio_request(struct device *dev, unsigned gpio,
				    const char *label)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	WARN_ON(1);
	return -EINVAL;
}

<<<<<<< HEAD
#ifdef CONFIG_PINCTRL

static inline int
gpiochip_add_pin_range(struct gpio_chip *chip, const char *pinctl_name,
		       unsigned int pin_base, unsigned int npins)
{
}

static inline void
gpiochip_remove_pin_ranges(struct gpio_chip *chip)
{
}

#endif /* CONFIG_PINCTRL */

#endif /* ! CONFIG_GENERIC_GPIO */
=======
static inline int devm_gpio_request_one(struct device *dev, unsigned gpio,
					unsigned long flags, const char *label)
{
	WARN_ON(1);
	return -EINVAL;
}

#endif /* ! CONFIG_GPIOLIB */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __LINUX_GPIO_H */
