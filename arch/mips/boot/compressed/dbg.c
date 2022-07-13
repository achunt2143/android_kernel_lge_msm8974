<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * MIPS-specific debug support for pre-boot environment
 *
 * NOTE: putc() is board specific, if your board have a 16550 compatible uart,
<<<<<<< HEAD
 * please select SYS_SUPPORTS_ZBOOT_UART16550 for your machine. othewise, you
 * need to implement your own putc().
 */
#include <linux/compiler.h>
#include <linux/init.h>
#include <linux/types.h>

=======
 * please select SYS_SUPPORTS_ZBOOT_UART16550 for your machine. otherwise, you
 * need to implement your own putc().
 */
#include <linux/compiler.h>
#include <linux/types.h>

#include "decompress.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void __weak putc(char c)
{
}

void puts(const char *s)
{
	char c;
	while ((c = *s++) != '\0') {
		putc(c);
		if (c == '\n')
			putc('\r');
	}
}

void puthex(unsigned long long val)
{

	unsigned char buf[10];
	int i;
	for (i = 7; i >= 0; i--) {
		buf[i] = "0123456789ABCDEF"[val & 0x0F];
		val >>= 4;
	}
	buf[8] = '\0';
	puts(buf);
}
