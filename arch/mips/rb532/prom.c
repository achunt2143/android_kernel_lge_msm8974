<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  RouterBoard 500 specific prom routines
 *
 *  Copyright (C) 2003, Peter Sadik <peter.sadik@idt.com>
 *  Copyright (C) 2005-2006, P.Christeas <p_christ@hol.gr>
 *  Copyright (C) 2007, Gabor Juhos <juhosg@openwrt.org>
 *			Felix Fietkau <nbd@openwrt.org>
 *			Florian Fainelli <florian@openwrt.org>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the
 *  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/string.h>
#include <linux/console.h>
#include <linux/bootmem.h>
#include <linux/ioport.h>
#include <linux/blkdev.h>
=======
#include <linux/export.h>
#include <linux/string.h>
#include <linux/console.h>
#include <linux/memblock.h>
#include <linux/ioport.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/bootinfo.h>
#include <asm/mach-rc32434/ddr.h>
#include <asm/mach-rc32434/prom.h>

unsigned int idt_cpu_freq = 132000000;
EXPORT_SYMBOL(idt_cpu_freq);

static struct resource ddr_reg[] = {
	{
		.name = "ddr-reg",
		.start = DDR0_PHYS_ADDR,
		.end = DDR0_PHYS_ADDR + sizeof(struct ddr_ram),
		.flags = IORESOURCE_MEM,
	}
};

<<<<<<< HEAD
void __init prom_free_prom_memory(void)
{
	/* No prom memory to free */
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int match_tag(char *arg, const char *tag)
{
	return strncmp(arg, tag, strlen(tag)) == 0;
}

static inline unsigned long tag2ul(char *arg, const char *tag)
{
	char *num;

	num = arg + strlen(tag);
	return simple_strtoul(num, 0, 10);
}

void __init prom_setup_cmdline(void)
{
	static char cmd_line[COMMAND_LINE_SIZE] __initdata;
	char *cp, *board;
	int prom_argc;
<<<<<<< HEAD
	char **prom_argv, **prom_envp;
=======
	char **prom_argv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;

	prom_argc = fw_arg0;
	prom_argv = (char **) fw_arg1;
<<<<<<< HEAD
	prom_envp = (char **) fw_arg2;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cp = cmd_line;
		/* Note: it is common that parameters start
		 * at argv[1] and not argv[0],
		 * however, our elf loader starts at [0] */
	for (i = 0; i < prom_argc; i++) {
		if (match_tag(prom_argv[i], FREQ_TAG)) {
			idt_cpu_freq = tag2ul(prom_argv[i], FREQ_TAG);
			continue;
		}
#ifdef IGNORE_CMDLINE_MEM
		/* parses out the "mem=xx" arg */
		if (match_tag(prom_argv[i], MEM_TAG))
			continue;
#endif
		if (i > 0)
			*(cp++) = ' ';
		if (match_tag(prom_argv[i], BOARD_TAG)) {
			board = prom_argv[i] + strlen(BOARD_TAG);

			if (match_tag(board, BOARD_RB532A))
				mips_machtype = MACH_MIKROTIK_RB532A;
			else
				mips_machtype = MACH_MIKROTIK_RB532;
		}

		strcpy(cp, prom_argv[i]);
		cp += strlen(prom_argv[i]);
	}
	*(cp++) = ' ';

	i = strlen(arcs_cmdline);
	if (i > 0) {
		*(cp++) = ' ';
		strcpy(cp, arcs_cmdline);
		cp += strlen(arcs_cmdline);
	}
	cmd_line[COMMAND_LINE_SIZE - 1] = '\0';

	strcpy(arcs_cmdline, cmd_line);
}

void __init prom_init(void)
{
	struct ddr_ram __iomem *ddr;
<<<<<<< HEAD
	phys_t memsize;
	phys_t ddrbase;

	ddr = ioremap_nocache(ddr_reg[0].start,
=======
	phys_addr_t memsize;
	phys_addr_t ddrbase;

	ddr = ioremap(ddr_reg[0].start,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ddr_reg[0].end - ddr_reg[0].start);

	if (!ddr) {
		printk(KERN_ERR "Unable to remap DDR register\n");
		return;
	}

<<<<<<< HEAD
	ddrbase = (phys_t)&ddr->ddrbase;
	memsize = (phys_t)&ddr->ddrmask;
=======
	ddrbase = (phys_addr_t)&ddr->ddrbase;
	memsize = (phys_addr_t)&ddr->ddrmask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memsize = 0 - memsize;

	prom_setup_cmdline();

	/* give all RAM to boot allocator,
	 * except for the first 0x400 and the last 0x200 bytes */
<<<<<<< HEAD
	add_memory_region(ddrbase + 0x400, memsize - 0x600, BOOT_MEM_RAM);
=======
	memblock_add(ddrbase + 0x400, memsize - 0x600);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
