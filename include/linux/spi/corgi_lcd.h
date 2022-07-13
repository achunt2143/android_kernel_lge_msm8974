<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __LINUX_SPI_CORGI_LCD_H
#define __LINUX_SPI_CORGI_LCD_H

#define CORGI_LCD_MODE_QVGA	1
#define CORGI_LCD_MODE_VGA	2

struct corgi_lcd_platform_data {
	int	init_mode;
	int	max_intensity;
	int	default_intensity;
	int	limit_mask;

<<<<<<< HEAD
	int	gpio_backlight_on;	/* -1 if n/a */
	int	gpio_backlight_cont;	/* -1 if n/a */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void (*notify)(int intensity);
	void (*kick_battery)(void);
};

<<<<<<< HEAD
=======
void corgi_lcd_limit_intensity(int limit);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __LINUX_SPI_CORGI_LCD_H */
