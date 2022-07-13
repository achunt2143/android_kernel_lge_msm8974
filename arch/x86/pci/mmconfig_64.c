<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * mmconfig.c - Low-level direct PCI config space access via MMCONFIG
 *
 * This is an 64bit optimized version that always keeps the full mmconfig
 * space mapped. This allows lockless config space operation.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) "PCI: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/acpi.h>
#include <linux/bitmap.h>
<<<<<<< HEAD
#include <asm/e820.h>
#include <asm/pci_x86.h>

#define PREFIX "PCI: "

=======
#include <linux/rcupdate.h>
#include <asm/e820/api.h>
#include <asm/pci_x86.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static char __iomem *pci_dev_base(unsigned int seg, unsigned int bus, unsigned int devfn)
{
	struct pci_mmcfg_region *cfg = pci_mmconfig_lookup(seg, bus);

	if (cfg && cfg->virt)
		return cfg->virt + (PCI_MMCFG_BUS_OFFSET(bus) | (devfn << 12));
	return NULL;
}

static int pci_mmcfg_read(unsigned int seg, unsigned int bus,
			  unsigned int devfn, int reg, int len, u32 *value)
{
	char __iomem *addr;

	/* Why do we have this when nobody checks it. How about a BUG()!? -AK */
	if (unlikely((bus > 255) || (devfn > 255) || (reg > 4095))) {
err:		*value = -1;
		return -EINVAL;
	}

<<<<<<< HEAD
	addr = pci_dev_base(seg, bus, devfn);
	if (!addr)
		goto err;
=======
	rcu_read_lock();
	addr = pci_dev_base(seg, bus, devfn);
	if (!addr) {
		rcu_read_unlock();
		goto err;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (len) {
	case 1:
		*value = mmio_config_readb(addr + reg);
		break;
	case 2:
		*value = mmio_config_readw(addr + reg);
		break;
	case 4:
		*value = mmio_config_readl(addr + reg);
		break;
	}
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int pci_mmcfg_write(unsigned int seg, unsigned int bus,
			   unsigned int devfn, int reg, int len, u32 value)
{
	char __iomem *addr;

	/* Why do we have this when nobody checks it. How about a BUG()!? -AK */
	if (unlikely((bus > 255) || (devfn > 255) || (reg > 4095)))
		return -EINVAL;

<<<<<<< HEAD
	addr = pci_dev_base(seg, bus, devfn);
	if (!addr)
		return -EINVAL;
=======
	rcu_read_lock();
	addr = pci_dev_base(seg, bus, devfn);
	if (!addr) {
		rcu_read_unlock();
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (len) {
	case 1:
		mmio_config_writeb(addr + reg, value);
		break;
	case 2:
		mmio_config_writew(addr + reg, value);
		break;
	case 4:
		mmio_config_writel(addr + reg, value);
		break;
	}
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct pci_raw_ops pci_mmcfg = {
=======
const struct pci_raw_ops pci_mmcfg = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read =		pci_mmcfg_read,
	.write =	pci_mmcfg_write,
};

<<<<<<< HEAD
static void __iomem * __init mcfg_ioremap(struct pci_mmcfg_region *cfg)
=======
static void __iomem *mcfg_ioremap(struct pci_mmcfg_region *cfg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	void __iomem *addr;
	u64 start, size;
	int num_buses;

	start = cfg->address + PCI_MMCFG_BUS_OFFSET(cfg->start_bus);
	num_buses = cfg->end_bus - cfg->start_bus + 1;
	size = PCI_MMCFG_BUS_OFFSET(num_buses);
<<<<<<< HEAD
	addr = ioremap_nocache(start, size);
=======
	addr = ioremap(start, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (addr)
		addr -= PCI_MMCFG_BUS_OFFSET(cfg->start_bus);
	return addr;
}

<<<<<<< HEAD
=======
int pci_mmcfg_arch_map(struct pci_mmcfg_region *cfg)
{
	cfg->virt = mcfg_ioremap(cfg);
	if (!cfg->virt) {
		pr_err("can't map ECAM at %pR\n", &cfg->res);
		return -ENOMEM;
	}

	return 0;
}

void pci_mmcfg_arch_unmap(struct pci_mmcfg_region *cfg)
{
	if (cfg && cfg->virt) {
		iounmap(cfg->virt + PCI_MMCFG_BUS_OFFSET(cfg->start_bus));
		cfg->virt = NULL;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __init pci_mmcfg_arch_init(void)
{
	struct pci_mmcfg_region *cfg;

<<<<<<< HEAD
	list_for_each_entry(cfg, &pci_mmcfg_list, list) {
		cfg->virt = mcfg_ioremap(cfg);
		if (!cfg->virt) {
			printk(KERN_ERR PREFIX "can't map MMCONFIG at %pR\n",
			       &cfg->res);
			pci_mmcfg_arch_free();
			return 0;
		}
	}
	raw_pci_ext_ops = &pci_mmcfg;
=======
	list_for_each_entry(cfg, &pci_mmcfg_list, list)
		if (pci_mmcfg_arch_map(cfg)) {
			pci_mmcfg_arch_free();
			return 0;
		}

	raw_pci_ext_ops = &pci_mmcfg;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}

void __init pci_mmcfg_arch_free(void)
{
	struct pci_mmcfg_region *cfg;

<<<<<<< HEAD
	list_for_each_entry(cfg, &pci_mmcfg_list, list) {
		if (cfg->virt) {
			iounmap(cfg->virt + PCI_MMCFG_BUS_OFFSET(cfg->start_bus));
			cfg->virt = NULL;
		}
	}
=======
	list_for_each_entry(cfg, &pci_mmcfg_list, list)
		pci_mmcfg_arch_unmap(cfg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
