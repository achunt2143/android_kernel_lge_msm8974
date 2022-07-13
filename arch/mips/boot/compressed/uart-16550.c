<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * 16550 compatible uart based serial debug support for zboot
 */

#include <linux/types.h>
#include <linux/serial_reg.h>
<<<<<<< HEAD
#include <linux/init.h>

#include <asm/addrspace.h>

#if defined(CONFIG_MACH_LOONGSON) || defined(CONFIG_MIPS_MALTA)
=======

#include <asm/addrspace.h>

#include "decompress.h"

#if defined(CONFIG_MACH_LOONGSON64) || defined(CONFIG_MIPS_MALTA)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define UART_BASE 0x1fd003f8
#define PORT(offset) (CKSEG1ADDR(UART_BASE) + (offset))
#endif

<<<<<<< HEAD
#ifdef CONFIG_AR7
#include <ar7.h>
#define PORT(offset) (CKSEG1ADDR(AR7_REGS_UART0) + (4 * offset))
=======
#ifdef CONFIG_MACH_INGENIC
#define INGENIC_UART_BASE_ADDR	(0x10030000 + 0x1000 * CONFIG_ZBOOT_INGENIC_UART)
#define PORT(offset) (CKSEG1ADDR(INGENIC_UART_BASE_ADDR) + (4 * offset))
#endif

#ifndef IOTYPE
#define IOTYPE char
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifndef PORT
#error please define the serial port address for your own machine
#endif

static inline unsigned int serial_in(int offset)
{
<<<<<<< HEAD
	return *((char *)PORT(offset));
=======
	return *((volatile IOTYPE *)PORT(offset)) & 0xFF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void serial_out(int offset, int value)
{
<<<<<<< HEAD
	*((char *)PORT(offset)) = value;
=======
	*((volatile IOTYPE *)PORT(offset)) = value & 0xFF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void putc(char c)
{
<<<<<<< HEAD
	int timeout = 1024;
=======
	int timeout = 1000000;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (((serial_in(UART_LSR) & UART_LSR_THRE) == 0) && (timeout-- > 0))
		;

	serial_out(UART_TX, c);
}
