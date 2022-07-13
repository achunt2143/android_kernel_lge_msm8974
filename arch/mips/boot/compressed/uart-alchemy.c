<<<<<<< HEAD
#include <asm/mach-au1x00/au1000.h>

void putc(char c)
{
#ifdef CONFIG_MIPS_DB1300
	alchemy_uart_putchar(AU1300_UART2_PHYS_ADDR, c);
#else
	alchemy_uart_putchar(AU1000_UART0_PHYS_ADDR, c);
#endif
=======
// SPDX-License-Identifier: GPL-2.0
#include <asm/mach-au1x00/au1000.h>

#include "decompress.h"

void putc(char c)
{
	alchemy_uart_putchar(AU1000_UART0_PHYS_ADDR, c);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
