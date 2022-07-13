<<<<<<< HEAD
#ifndef _ASM_X86_PARPORT_H
#define _ASM_X86_PARPORT_H

static int __devinit parport_pc_find_isa_ports(int autoirq, int autodma);
static int __devinit parport_pc_find_nonpci_ports(int autoirq, int autodma)
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PARPORT_H
#define _ASM_X86_PARPORT_H

static int parport_pc_find_isa_ports(int autoirq, int autodma);
static int parport_pc_find_nonpci_ports(int autoirq, int autodma)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return parport_pc_find_isa_ports(autoirq, autodma);
}

#endif /* _ASM_X86_PARPORT_H */
