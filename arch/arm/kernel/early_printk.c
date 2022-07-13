<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/kernel/early_printk.c
 *
 *  Copyright (C) 2009 Sascha Hauer <s.hauer@pengutronix.de>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/console.h>
#include <linux/init.h>
<<<<<<< HEAD

extern void printch(int);

static void early_write(const char *s, unsigned n)
{
	while (n-- > 0) {
		if (*s == '\n')
			printch('\r');
		printch(*s);
		s++;
=======
#include <linux/string.h>

extern void printascii(const char *);

static void early_write(const char *s, unsigned n)
{
	char buf[128];
	while (n) {
		unsigned l = min(n, sizeof(buf)-1);
		memcpy(buf, s, l);
		buf[l] = 0;
		s += l;
		n -= l;
		printascii(buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void early_console_write(struct console *con, const char *s, unsigned n)
{
	early_write(s, n);
}

<<<<<<< HEAD
static struct console early_console = {
=======
static struct console early_console_dev = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name =		"earlycon",
	.write =	early_console_write,
	.flags =	CON_PRINTBUFFER | CON_BOOT,
	.index =	-1,
};

<<<<<<< HEAD
asmlinkage void early_printk(const char *fmt, ...)
{
	char buf[512];
	int n;
	va_list ap;

	va_start(ap, fmt);
	n = vscnprintf(buf, sizeof(buf), fmt, ap);
	early_write(buf, n);
	va_end(ap);
}

static int __init setup_early_printk(char *buf)
{
	register_console(&early_console);
=======
static int __init setup_early_printk(char *buf)
{
	early_console = &early_console_dev;
	register_console(&early_console_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

early_param("earlyprintk", setup_early_printk);
