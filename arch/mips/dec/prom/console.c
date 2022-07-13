<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	DECstation PROM-based early console support.
 *
 *	Copyright (C) 2004, 2007  Maciej W. Rozycki
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/console.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>

#include <asm/dec/prom.h>

static void __init prom_console_write(struct console *con, const char *s,
				      unsigned int c)
{
	char buf[81];
	unsigned int chunk = sizeof(buf) - 1;

	while (c > 0) {
		if (chunk > c)
			chunk = c;
		memcpy(buf, s, chunk);
		buf[chunk] = '\0';
		prom_printf("%s", buf);
		s += chunk;
		c -= chunk;
	}
}

static struct console promcons __initdata = {
	.name	= "prom",
	.write	= prom_console_write,
	.flags	= CON_BOOT | CON_PRINTBUFFER,
	.index	= -1,
};

void __init register_prom_console(void)
{
	register_console(&promcons);
}
