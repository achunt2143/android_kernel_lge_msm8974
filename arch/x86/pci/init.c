<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pci.h>
#include <linux/init.h>
#include <asm/pci_x86.h>
#include <asm/x86_init.h>
<<<<<<< HEAD
=======
#include <asm/irqdomain.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* arch_initcall has too random ordering, so call the initializers
   in the right sequence from here. */
static __init int pci_arch_init(void)
{
<<<<<<< HEAD
#ifdef CONFIG_PCI_DIRECT
	int type = 0;

	type = pci_direct_probe();
#endif
=======
	int type, pcbios = 1;

	type = pci_direct_probe();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!(pci_probe & PCI_PROBE_NOEARLY))
		pci_mmcfg_early_init();

<<<<<<< HEAD
	if (x86_init.pci.arch_init && !x86_init.pci.arch_init())
		return 0;

#ifdef CONFIG_PCI_BIOS
	pci_pcbios_init();
#endif
=======
	if (x86_init.pci.arch_init)
		pcbios = x86_init.pci.arch_init();

	/*
	 * Must happen after x86_init.pci.arch_init(). Xen sets up the
	 * x86_init.irqs.create_pci_msi_domain there.
	 */
	x86_create_pci_msi_domain();

	if (!pcbios)
		return 0;

	pci_pcbios_init();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * don't check for raw_pci_ops here because we want pcbios as last
	 * fallback, yet it's needed to run first to set pcibios_last_bus
	 * in case legacy PCI probing is used. otherwise detecting peer busses
	 * fails.
	 */
<<<<<<< HEAD
#ifdef CONFIG_PCI_DIRECT
	pci_direct_init(type);
#endif
=======
	pci_direct_init(type);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!raw_pci_ops && !raw_pci_ext_ops)
		printk(KERN_ERR
		"PCI: Fatal: No config space access function found\n");

	dmi_check_pciprobe();

	dmi_check_skip_isa_align();

	return 0;
}
arch_initcall(pci_arch_init);
