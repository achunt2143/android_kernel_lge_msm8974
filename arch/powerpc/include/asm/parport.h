<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * parport.h: platform-specific PC-style parport initialisation
 *
 * Copyright (C) 1999, 2000  Tim Waugh <tim@cyberelk.demon.co.uk>
 *
 * This file should only be included by drivers/parport/parport_pc.c.
 */

#ifndef _ASM_POWERPC_PARPORT_H
#define _ASM_POWERPC_PARPORT_H
#ifdef __KERNEL__

<<<<<<< HEAD
#include <asm/prom.h>

static int __devinit parport_pc_find_nonpci_ports (int autoirq, int autodma)
=======
#include <linux/of_irq.h>

static int parport_pc_find_nonpci_ports (int autoirq, int autodma)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct device_node *np;
	const u32 *prop;
	u32 io1, io2;
	int propsize;
	int count = 0;
	int virq;

<<<<<<< HEAD
	for (np = NULL; (np = of_find_compatible_node(np,
						      "parallel",
						      "pnpPNP,400")) != NULL;) {
=======
	for_each_compatible_node(np, "parallel", "pnpPNP,400") {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		prop = of_get_property(np, "reg", &propsize);
		if (!prop || propsize > 6*sizeof(u32))
			continue;
		io1 = prop[1]; io2 = prop[2];

		virq = irq_of_parse_and_map(np, 0);
<<<<<<< HEAD
		if (virq == NO_IRQ)
=======
		if (!virq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;

		if (parport_pc_probe_port(io1, io2, virq, autodma, NULL, 0)
				!= NULL)
			count++;
	}
	return count;
}

#endif /* __KERNEL__ */
#endif /* !(_ASM_POWERPC_PARPORT_H) */
