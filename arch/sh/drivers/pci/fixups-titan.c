<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/sh/drivers/pci/ops-titan.c
 *
 * Ported to new API by Paul Mundt <lethal@linux-sh.org>
 *
 * Modified from ops-snapgear.c written by  David McCullough
 * Highly leveraged from pci-bigsur.c, written by Dustin McIntire.
 *
<<<<<<< HEAD
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * PCI initialization for the Titan boards
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/io.h>
#include <mach/titan.h>
#include "pci-sh4.h"

<<<<<<< HEAD
static char titan_irq_tab[] __initdata = {
=======
static char titan_irq_tab[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	TITAN_IRQ_WAN,
	TITAN_IRQ_LAN,
	TITAN_IRQ_MPCIA,
	TITAN_IRQ_MPCIB,
	TITAN_IRQ_USB,
};

<<<<<<< HEAD
int __init pcibios_map_platform_irq(const struct pci_dev *pdev, u8 slot, u8 pin)
=======
int pcibios_map_platform_irq(const struct pci_dev *pdev, u8 slot, u8 pin)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int irq = titan_irq_tab[slot];

	printk("PCI: Mapping TITAN IRQ for slot %d, pin %c to irq %d\n",
		slot, pin - 1 + 'A', irq);

	return irq;
}
