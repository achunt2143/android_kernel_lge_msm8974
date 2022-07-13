<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/sh/drivers/pci/ops-snapgear.c
 *
 * Author:  David McCullough <davidm@snapgear.com>
 *
 * Ported to new API by Paul Mundt <lethal@linux-sh.org>
 *
 * Highly leveraged from pci-bigsur.c, written by Dustin McIntire.
 *
<<<<<<< HEAD
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * PCI initialization for the SnapGear boards
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/pci.h>
<<<<<<< HEAD
#include "pci-sh4.h"

int __init pcibios_map_platform_irq(const struct pci_dev *pdev, u8 slot, u8 pin)
=======
#include <linux/sh_intc.h>
#include "pci-sh4.h"

int pcibios_map_platform_irq(const struct pci_dev *pdev, u8 slot, u8 pin)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int irq = -1;

	switch (slot) {
	case 8:  /* the PCI bridge */ break;
<<<<<<< HEAD
	case 11: irq = 8;  break; /* USB    */
	case 12: irq = 11; break; /* PCMCIA */
	case 13: irq = 5;  break; /* eth0   */
	case 14: irq = 8;  break; /* eth1   */
	case 15: irq = 11; break; /* safenet (unused) */
=======
	case 11: irq = evt2irq(0x300); break; /* USB    */
	case 12: irq = evt2irq(0x360); break; /* PCMCIA */
	case 13: irq = evt2irq(0x2a0); break; /* eth0   */
	case 14: irq = evt2irq(0x300); break; /* eth1   */
	case 15: irq = evt2irq(0x360); break; /* safenet (unused) */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	printk("PCI: Mapping SnapGear IRQ for slot %d, pin %c to irq %d\n",
	       slot, pin - 1 + 'A', irq);

	return irq;
}
