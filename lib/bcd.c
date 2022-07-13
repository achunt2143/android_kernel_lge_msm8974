<<<<<<< HEAD
#include <linux/bcd.h>
#include <linux/export.h>

unsigned bcd2bin(unsigned char val)
{
	return (val & 0x0f) + (val >> 4) * 10;
}
EXPORT_SYMBOL(bcd2bin);

unsigned char bin2bcd(unsigned val)
{
	return ((val / 10) << 4) + val % 10;
}
EXPORT_SYMBOL(bin2bcd);
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/bcd.h>
#include <linux/export.h>

unsigned _bcd2bin(unsigned char val)
{
	return (val & 0x0f) + (val >> 4) * 10;
}
EXPORT_SYMBOL(_bcd2bin);

unsigned char _bin2bcd(unsigned val)
{
	return ((val / 10) << 4) + val % 10;
}
EXPORT_SYMBOL(_bin2bcd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
