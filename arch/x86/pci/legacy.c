<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * legacy.c - traditional, old school PCI bus probing
 */
#include <linux/init.h>
#include <linux/export.h>
#include <linux/pci.h>
<<<<<<< HEAD
=======
#include <asm/jailhouse_para.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/pci_x86.h>

/*
 * Discover remaining PCI buses in case there are peer host bridges.
 * We use the number of last PCI bus provided by the PCI BIOS.
 */
<<<<<<< HEAD
static void __devinit pcibios_fixup_peer_bridges(void)
=======
static void pcibios_fixup_peer_bridges(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int n;

	if (pcibios_last_bus <= 0 || pcibios_last_bus > 0xff)
		return;
	DBG("PCI: Peer bridge fixup\n");

	for (n=0; n <= pcibios_last_bus; n++)
		pcibios_scan_specific_bus(n);
}

int __init pci_legacy_init(void)
{
<<<<<<< HEAD
	if (!raw_pci_ops) {
		printk("PCI: System does not support PCI\n");
		return 0;
	}

	printk("PCI: Probing PCI hardware\n");
	pci_root_bus = pcibios_scan_root(0);
	return 0;
}

void __devinit pcibios_scan_specific_bus(int busn)
{
	int devfn;
	long node;
=======
	if (!raw_pci_ops)
		return 1;

	pr_info("PCI: Probing PCI hardware\n");
	pcibios_scan_root(0);
	return 0;
}

void pcibios_scan_specific_bus(int busn)
{
	int stride = jailhouse_paravirt() ? 1 : 8;
	int devfn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 l;

	if (pci_find_bus(0, busn))
		return;

<<<<<<< HEAD
	node = get_mp_bus_to_node(busn);
	for (devfn = 0; devfn < 256; devfn += 8) {
		if (!raw_pci_read(0, busn, devfn, PCI_VENDOR_ID, 2, &l) &&
		    l != 0x0000 && l != 0xffff) {
			DBG("Found device at %02x:%02x [%04x]\n", busn, devfn, l);
			printk(KERN_INFO "PCI: Discovered peer bus %02x\n", busn);
			pci_scan_bus_on_node(busn, &pci_root_ops, node);
=======
	for (devfn = 0; devfn < 256; devfn += stride) {
		if (!raw_pci_read(0, busn, devfn, PCI_VENDOR_ID, 2, &l) &&
		    l != 0x0000 && l != 0xffff) {
			DBG("Found device at %02x:%02x [%04x]\n", busn, devfn, l);
			pr_info("PCI: Discovered peer bus %02x\n", busn);
			pcibios_scan_root(busn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return;
		}
	}
}
EXPORT_SYMBOL_GPL(pcibios_scan_specific_bus);

<<<<<<< HEAD
int __init pci_subsys_init(void)
=======
static int __init pci_subsys_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * The init function returns an non zero value when
	 * pci_legacy_init should be invoked.
	 */
<<<<<<< HEAD
	if (x86_init.pci.init())
		pci_legacy_init();
=======
	if (x86_init.pci.init()) {
		if (pci_legacy_init()) {
			pr_info("PCI: System does not support PCI\n");
			return -ENODEV;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pcibios_fixup_peer_bridges();
	x86_init.pci.init_irq();
	pcibios_init();

	return 0;
}
subsys_initcall(pci_subsys_init);
