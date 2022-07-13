<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generic PWM backlight driver data - see drivers/video/backlight/pwm_bl.c
 */
#ifndef __LINUX_PWM_BACKLIGHT_H
#define __LINUX_PWM_BACKLIGHT_H

#include <linux/backlight.h>

struct platform_pwm_backlight_data {
<<<<<<< HEAD
	int pwm_id;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int max_brightness;
	unsigned int dft_brightness;
	unsigned int lth_brightness;
	unsigned int pwm_period_ns;
<<<<<<< HEAD
=======
	unsigned int *levels;
	unsigned int post_pwm_on_delay;
	unsigned int pwm_off_delay;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*init)(struct device *dev);
	int (*notify)(struct device *dev, int brightness);
	void (*notify_after)(struct device *dev, int brightness);
	void (*exit)(struct device *dev);
	int (*check_fb)(struct device *dev, struct fb_info *info);
};

#endif
