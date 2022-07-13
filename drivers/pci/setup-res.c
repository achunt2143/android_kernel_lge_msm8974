<<<<<<< HEAD
/*
 *	drivers/pci/setup-res.c
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Support routines for initializing a PCI subsystem
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Extruded from code written by
 *      Dave Rusling (david.rusling@reo.mts.dec.com)
 *      David Mosberger (davidm@cs.arizona.edu)
 *	David Miller (davem@redhat.com)
 *
<<<<<<< HEAD
 * Support routines for initializing a PCI subsystem.
 */

/* fixed for multiple pci buses, 1999 Andrea Arcangeli <andrea@suse.de> */

/*
=======
 * Fixed for multiple PCI buses, 1999 Andrea Arcangeli <andrea@suse.de>
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Nov 2000, Ivan Kokshaysky <ink@jurassic.park.msu.ru>
 *	     Resource sorting
 */

<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/pci.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/cache.h>
#include <linux/slab.h>
#include "pci.h"

<<<<<<< HEAD

void pci_update_resource(struct pci_dev *dev, int resno)
{
	struct pci_bus_region region;
	u32 new, check, mask;
	int reg;
	enum pci_bar_type type;
	struct resource *res = dev->resource + resno;
=======
static void pci_std_update_resource(struct pci_dev *dev, int resno)
{
	struct pci_bus_region region;
	bool disable;
	u16 cmd;
	u32 new, check, mask;
	int reg;
	struct resource *res = dev->resource + resno;
	const char *res_name = pci_resource_name(dev, resno);

	/* Per SR-IOV spec 3.4.1.11, VF BARs are RO zero */
	if (dev->is_virtfn)
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Ignore resources for unimplemented BARs and unused resource slots
	 * for 64 bit BARs.
	 */
	if (!res->flags)
		return;

<<<<<<< HEAD
=======
	if (res->flags & IORESOURCE_UNSET)
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Ignore non-moveable resources.  This might be legacy resources for
	 * which no functional BAR register exists or another important
	 * system resource we shouldn't move around.
	 */
	if (res->flags & IORESOURCE_PCI_FIXED)
		return;

	pcibios_resource_to_bus(dev->bus, &region, res);
<<<<<<< HEAD

	new = region.start | (res->flags & PCI_REGION_FLAG_MASK);
	if (res->flags & IORESOURCE_IO)
		mask = (u32)PCI_BASE_ADDRESS_IO_MASK;
	else
		mask = (u32)PCI_BASE_ADDRESS_MEM_MASK;

	reg = pci_resource_bar(dev, resno, &type);
	if (!reg)
		return;
	if (type != pci_bar_unknown) {
		if (!(res->flags & IORESOURCE_ROM_ENABLE))
			return;
		new |= PCI_ROM_ADDRESS_ENABLE;
=======
	new = region.start;

	if (res->flags & IORESOURCE_IO) {
		mask = (u32)PCI_BASE_ADDRESS_IO_MASK;
		new |= res->flags & ~PCI_BASE_ADDRESS_IO_MASK;
	} else if (resno == PCI_ROM_RESOURCE) {
		mask = PCI_ROM_ADDRESS_MASK;
	} else {
		mask = (u32)PCI_BASE_ADDRESS_MEM_MASK;
		new |= res->flags & ~PCI_BASE_ADDRESS_MEM_MASK;
	}

	if (resno < PCI_ROM_RESOURCE) {
		reg = PCI_BASE_ADDRESS_0 + 4 * resno;
	} else if (resno == PCI_ROM_RESOURCE) {

		/*
		 * Apparently some Matrox devices have ROM BARs that read
		 * as zero when disabled, so don't update ROM BARs unless
		 * they're enabled.  See
		 * https://lore.kernel.org/r/43147B3D.1030309@vc.cvut.cz/
		 * But we must update ROM BAR for buggy devices where even a
		 * disabled ROM can conflict with other BARs.
		 */
		if (!(res->flags & IORESOURCE_ROM_ENABLE) &&
		    !dev->rom_bar_overlap)
			return;

		reg = dev->rom_base_reg;
		if (res->flags & IORESOURCE_ROM_ENABLE)
			new |= PCI_ROM_ADDRESS_ENABLE;
	} else
		return;

	/*
	 * We can't update a 64-bit BAR atomically, so when possible,
	 * disable decoding so that a half-updated BAR won't conflict
	 * with another device.
	 */
	disable = (res->flags & IORESOURCE_MEM_64) && !dev->mmio_always_on;
	if (disable) {
		pci_read_config_word(dev, PCI_COMMAND, &cmd);
		pci_write_config_word(dev, PCI_COMMAND,
				      cmd & ~PCI_COMMAND_MEMORY);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	pci_write_config_dword(dev, reg, new);
	pci_read_config_dword(dev, reg, &check);

	if ((new ^ check) & mask) {
<<<<<<< HEAD
		dev_err(&dev->dev, "BAR %d: error updating (%#08x != %#08x)\n",
			resno, new, check);
=======
		pci_err(dev, "%s: error updating (%#010x != %#010x)\n",
			res_name, new, check);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (res->flags & IORESOURCE_MEM_64) {
		new = region.start >> 16 >> 16;
		pci_write_config_dword(dev, reg + 4, new);
		pci_read_config_dword(dev, reg + 4, &check);
		if (check != new) {
<<<<<<< HEAD
			dev_err(&dev->dev, "BAR %d: error updating "
			       "(high %#08x != %#08x)\n", resno, new, check);
		}
	}
	res->flags &= ~IORESOURCE_UNSET;
	dev_dbg(&dev->dev, "BAR %d: set to %pR (PCI address [%#llx-%#llx])\n",
		resno, res, (unsigned long long)region.start,
		(unsigned long long)region.end);
=======
			pci_err(dev, "%s: error updating (high %#010x != %#010x)\n",
				res_name, new, check);
		}
	}

	if (disable)
		pci_write_config_word(dev, PCI_COMMAND, cmd);
}

void pci_update_resource(struct pci_dev *dev, int resno)
{
	if (resno <= PCI_ROM_RESOURCE)
		pci_std_update_resource(dev, resno);
#ifdef CONFIG_PCI_IOV
	else if (resno >= PCI_IOV_RESOURCES && resno <= PCI_IOV_RESOURCE_END)
		pci_iov_update_resource(dev, resno);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int pci_claim_resource(struct pci_dev *dev, int resource)
{
	struct resource *res = &dev->resource[resource];
<<<<<<< HEAD
	struct resource *root, *conflict;

	root = pci_find_parent_resource(dev, res);
	if (!root) {
		dev_info(&dev->dev, "no compatible bridge window for %pR\n",
			 res);
=======
	const char *res_name = pci_resource_name(dev, resource);
	struct resource *root, *conflict;

	if (res->flags & IORESOURCE_UNSET) {
		pci_info(dev, "%s %pR: can't claim; no address assigned\n",
			 res_name, res);
		return -EINVAL;
	}

	/*
	 * If we have a shadow copy in RAM, the PCI device doesn't respond
	 * to the shadow range, so we don't need to claim it, and upstream
	 * bridges don't need to route the range to the device.
	 */
	if (res->flags & IORESOURCE_ROM_SHADOW)
		return 0;

	root = pci_find_parent_resource(dev, res);
	if (!root) {
		pci_info(dev, "%s %pR: can't claim; no compatible bridge window\n",
			 res_name, res);
		res->flags |= IORESOURCE_UNSET;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	conflict = request_resource_conflict(root, res);
	if (conflict) {
<<<<<<< HEAD
		dev_info(&dev->dev,
			 "address space collision: %pR conflicts with %s %pR\n",
			 res, conflict->name, conflict);
=======
		pci_info(dev, "%s %pR: can't claim; address conflict with %s %pR\n",
			 res_name, res, conflict->name, conflict);
		res->flags |= IORESOURCE_UNSET;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EBUSY;
	}

	return 0;
}
EXPORT_SYMBOL(pci_claim_resource);

void pci_disable_bridge_window(struct pci_dev *dev)
{
<<<<<<< HEAD
	dev_info(&dev->dev, "disabling bridge mem windows\n");

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* MMIO Base/Limit */
	pci_write_config_dword(dev, PCI_MEMORY_BASE, 0x0000fff0);

	/* Prefetchable MMIO Base/Limit */
	pci_write_config_dword(dev, PCI_PREF_LIMIT_UPPER32, 0);
	pci_write_config_dword(dev, PCI_PREF_MEMORY_BASE, 0x0000fff0);
	pci_write_config_dword(dev, PCI_PREF_BASE_UPPER32, 0xffffffff);
}

<<<<<<< HEAD
static int __pci_assign_resource(struct pci_bus *bus, struct pci_dev *dev,
		int resno, resource_size_t size, resource_size_t align)
{
	struct resource *res = dev->resource + resno;
	resource_size_t min;
	int ret;

	min = (res->flags & IORESOURCE_IO) ? PCIBIOS_MIN_IO : PCIBIOS_MIN_MEM;

	/* First, try exact prefetching match.. */
	ret = pci_bus_alloc_resource(bus, res, size, align, min,
				     IORESOURCE_PREFETCH,
				     pcibios_align_resource, dev);

	if (ret < 0 && (res->flags & IORESOURCE_PREFETCH)) {
		/*
		 * That failed.
		 *
		 * But a prefetching area can handle a non-prefetching
		 * window (it will just not perform as well).
		 */
		ret = pci_bus_alloc_resource(bus, res, size, align, min, 0,
					     pcibios_align_resource, dev);
	}
	return ret;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generic function that returns a value indicating that the device's
 * original BIOS BAR address was not saved and so is not available for
 * reinstatement.
 *
 * Can be over-ridden by architecture specific code that implements
 * reinstatement functionality rather than leaving it disabled when
 * normal allocation attempts fail.
 */
resource_size_t __weak pcibios_retrieve_fw_addr(struct pci_dev *dev, int idx)
{
	return 0;
}

<<<<<<< HEAD
static int pci_revert_fw_address(struct resource *res, struct pci_dev *dev, 
=======
static int pci_revert_fw_address(struct resource *res, struct pci_dev *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int resno, resource_size_t size)
{
	struct resource *root, *conflict;
	resource_size_t fw_addr, start, end;
<<<<<<< HEAD
	int ret = 0;

	fw_addr = pcibios_retrieve_fw_addr(dev, resno);
	if (!fw_addr)
		return 1;
=======
	const char *res_name = pci_resource_name(dev, resno);

	fw_addr = pcibios_retrieve_fw_addr(dev, resno);
	if (!fw_addr)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	start = res->start;
	end = res->end;
	res->start = fw_addr;
	res->end = res->start + size - 1;
<<<<<<< HEAD

	root = pci_find_parent_resource(dev, res);
	if (!root) {
=======
	res->flags &= ~IORESOURCE_UNSET;

	root = pci_find_parent_resource(dev, res);
	if (!root) {
		/*
		 * If dev is behind a bridge, accesses will only reach it
		 * if res is inside the relevant bridge window.
		 */
		if (pci_upstream_bridge(dev))
			return -ENXIO;

		/*
		 * On the root bus, assume the host bridge will forward
		 * everything.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (res->flags & IORESOURCE_IO)
			root = &ioport_resource;
		else
			root = &iomem_resource;
	}

<<<<<<< HEAD
	dev_info(&dev->dev, "BAR %d: trying firmware assignment %pR\n",
		 resno, res);
	conflict = request_resource_conflict(root, res);
	if (conflict) {
		dev_info(&dev->dev,
			 "BAR %d: %pR conflicts with %s %pR\n", resno,
			 res, conflict->name, conflict);
		res->start = start;
		res->end = end;
		ret = 1;
	}
=======
	pci_info(dev, "%s: trying firmware assignment %pR\n", res_name, res);
	conflict = request_resource_conflict(root, res);
	if (conflict) {
		pci_info(dev, "%s %pR: conflicts with %s %pR\n", res_name, res,
			 conflict->name, conflict);
		res->start = start;
		res->end = end;
		res->flags |= IORESOURCE_UNSET;
		return -EBUSY;
	}
	return 0;
}

/*
 * We don't have to worry about legacy ISA devices, so nothing to do here.
 * This is marked as __weak because multiple architectures define it; it should
 * eventually go away.
 */
resource_size_t __weak pcibios_align_resource(void *data,
					      const struct resource *res,
					      resource_size_t size,
					      resource_size_t align)
{
       return res->start;
}

static int __pci_assign_resource(struct pci_bus *bus, struct pci_dev *dev,
		int resno, resource_size_t size, resource_size_t align)
{
	struct resource *res = dev->resource + resno;
	resource_size_t min;
	int ret;

	min = (res->flags & IORESOURCE_IO) ? PCIBIOS_MIN_IO : PCIBIOS_MIN_MEM;

	/*
	 * First, try exact prefetching match.  Even if a 64-bit
	 * prefetchable bridge window is below 4GB, we can't put a 32-bit
	 * prefetchable resource in it because pbus_size_mem() assumes a
	 * 64-bit window will contain no 32-bit resources.  If we assign
	 * things differently than they were sized, not everything will fit.
	 */
	ret = pci_bus_alloc_resource(bus, res, size, align, min,
				     IORESOURCE_PREFETCH | IORESOURCE_MEM_64,
				     pcibios_align_resource, dev);
	if (ret == 0)
		return 0;

	/*
	 * If the prefetchable window is only 32 bits wide, we can put
	 * 64-bit prefetchable resources in it.
	 */
	if ((res->flags & (IORESOURCE_PREFETCH | IORESOURCE_MEM_64)) ==
	     (IORESOURCE_PREFETCH | IORESOURCE_MEM_64)) {
		ret = pci_bus_alloc_resource(bus, res, size, align, min,
					     IORESOURCE_PREFETCH,
					     pcibios_align_resource, dev);
		if (ret == 0)
			return 0;
	}

	/*
	 * If we didn't find a better match, we can put any memory resource
	 * in a non-prefetchable window.  If this resource is 32 bits and
	 * non-prefetchable, the first call already tried the only possibility
	 * so we don't need to try again.
	 */
	if (res->flags & (IORESOURCE_PREFETCH | IORESOURCE_MEM_64))
		ret = pci_bus_alloc_resource(bus, res, size, align, min, 0,
					     pcibios_align_resource, dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int _pci_assign_resource(struct pci_dev *dev, int resno,
				resource_size_t size, resource_size_t min_align)
{
<<<<<<< HEAD
	struct resource *res = dev->resource + resno;
	struct pci_bus *bus;
	int ret;
	char *type;
=======
	struct pci_bus *bus;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bus = dev->bus;
	while ((ret = __pci_assign_resource(bus, dev, resno, size, min_align))) {
		if (!bus->parent || !bus->self->transparent)
			break;
		bus = bus->parent;
	}

<<<<<<< HEAD
	if (ret) {
		if (res->flags & IORESOURCE_MEM)
			if (res->flags & IORESOURCE_PREFETCH)
				type = "mem pref";
			else
				type = "mem";
		else if (res->flags & IORESOURCE_IO)
			type = "io";
		else
			type = "unknown";
		dev_info(&dev->dev,
			 "BAR %d: can't assign %s (size %#llx)\n",
			 resno, type, (unsigned long long) resource_size(res));
	}

	return ret;
}

int pci_reassign_resource(struct pci_dev *dev, int resno, resource_size_t addsize,
			resource_size_t min_align)
{
	struct resource *res = dev->resource + resno;
	resource_size_t new_size;
	int ret;

	if (!res->parent) {
		dev_info(&dev->dev, "BAR %d: can't reassign an unassigned resource %pR "
			 "\n", resno, res);
		return -EINVAL;
	}

	/* already aligned with min_align */
	new_size = resource_size(res) + addsize;
	ret = _pci_assign_resource(dev, resno, new_size, min_align);
	if (!ret) {
		res->flags &= ~IORESOURCE_STARTALIGN;
		dev_info(&dev->dev, "BAR %d: reassigned %pR\n", resno, res);
		if (resno < PCI_BRIDGE_RESOURCES)
			pci_update_resource(dev, resno);
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

int pci_assign_resource(struct pci_dev *dev, int resno)
{
	struct resource *res = dev->resource + resno;
<<<<<<< HEAD
	resource_size_t align, size;
	struct pci_bus *bus;
	int ret;

	align = pci_resource_alignment(dev, res);
	if (!align) {
		dev_info(&dev->dev, "BAR %d: can't assign %pR "
			 "(bogus alignment)\n", resno, res);
		return -EINVAL;
	}

	bus = dev->bus;
=======
	const char *res_name = pci_resource_name(dev, resno);
	resource_size_t align, size;
	int ret;

	if (res->flags & IORESOURCE_PCI_FIXED)
		return 0;

	res->flags |= IORESOURCE_UNSET;
	align = pci_resource_alignment(dev, res);
	if (!align) {
		pci_info(dev, "%s %pR: can't assign; bogus alignment\n",
			 res_name, res);
		return -EINVAL;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	size = resource_size(res);
	ret = _pci_assign_resource(dev, resno, size, align);

	/*
	 * If we failed to assign anything, let's try the address
	 * where firmware left it.  That at least has a chance of
	 * working, which is better than just leaving it disabled.
	 */
<<<<<<< HEAD
	if (ret < 0)
		ret = pci_revert_fw_address(res, dev, resno, size);

	if (!ret) {
		res->flags &= ~IORESOURCE_STARTALIGN;
		dev_info(&dev->dev, "BAR %d: assigned %pR\n", resno, res);
		if (resno < PCI_BRIDGE_RESOURCES)
			pci_update_resource(dev, resno);
	}
	return ret;
}
=======
	if (ret < 0) {
		pci_info(dev, "%s %pR: can't assign; no space\n", res_name, res);
		ret = pci_revert_fw_address(res, dev, resno, size);
	}

	if (ret < 0) {
		pci_info(dev, "%s %pR: failed to assign\n", res_name, res);
		return ret;
	}

	res->flags &= ~IORESOURCE_UNSET;
	res->flags &= ~IORESOURCE_STARTALIGN;
	pci_info(dev, "%s %pR: assigned\n", res_name, res);
	if (resno < PCI_BRIDGE_RESOURCES)
		pci_update_resource(dev, resno);

	return 0;
}
EXPORT_SYMBOL(pci_assign_resource);

int pci_reassign_resource(struct pci_dev *dev, int resno,
			  resource_size_t addsize, resource_size_t min_align)
{
	struct resource *res = dev->resource + resno;
	const char *res_name = pci_resource_name(dev, resno);
	unsigned long flags;
	resource_size_t new_size;
	int ret;

	if (res->flags & IORESOURCE_PCI_FIXED)
		return 0;

	flags = res->flags;
	res->flags |= IORESOURCE_UNSET;
	if (!res->parent) {
		pci_info(dev, "%s %pR: can't reassign; unassigned resource\n",
			 res_name, res);
		return -EINVAL;
	}

	/* already aligned with min_align */
	new_size = resource_size(res) + addsize;
	ret = _pci_assign_resource(dev, resno, new_size, min_align);
	if (ret) {
		res->flags = flags;
		pci_info(dev, "%s %pR: failed to expand by %#llx\n",
			 res_name, res, (unsigned long long) addsize);
		return ret;
	}

	res->flags &= ~IORESOURCE_UNSET;
	res->flags &= ~IORESOURCE_STARTALIGN;
	pci_info(dev, "%s %pR: reassigned; expanded by %#llx\n",
		 res_name, res, (unsigned long long) addsize);
	if (resno < PCI_BRIDGE_RESOURCES)
		pci_update_resource(dev, resno);

	return 0;
}

void pci_release_resource(struct pci_dev *dev, int resno)
{
	struct resource *res = dev->resource + resno;
	const char *res_name = pci_resource_name(dev, resno);

	pci_info(dev, "%s %pR: releasing\n", res_name, res);

	if (!res->parent)
		return;

	release_resource(res);
	res->end = resource_size(res) - 1;
	res->start = 0;
	res->flags |= IORESOURCE_UNSET;
}
EXPORT_SYMBOL(pci_release_resource);

int pci_resize_resource(struct pci_dev *dev, int resno, int size)
{
	struct resource *res = dev->resource + resno;
	struct pci_host_bridge *host;
	int old, ret;
	u32 sizes;
	u16 cmd;

	/* Check if we must preserve the firmware's resource assignment */
	host = pci_find_host_bridge(dev->bus);
	if (host->preserve_config)
		return -ENOTSUPP;

	/* Make sure the resource isn't assigned before resizing it. */
	if (!(res->flags & IORESOURCE_UNSET))
		return -EBUSY;

	pci_read_config_word(dev, PCI_COMMAND, &cmd);
	if (cmd & PCI_COMMAND_MEMORY)
		return -EBUSY;

	sizes = pci_rebar_get_possible_sizes(dev, resno);
	if (!sizes)
		return -ENOTSUPP;

	if (!(sizes & BIT(size)))
		return -EINVAL;

	old = pci_rebar_get_current_size(dev, resno);
	if (old < 0)
		return old;

	ret = pci_rebar_set_size(dev, resno, size);
	if (ret)
		return ret;

	res->end = res->start + pci_rebar_size_to_bytes(size) - 1;

	/* Check if the new config works by trying to assign everything. */
	if (dev->bus->self) {
		ret = pci_reassign_bridge_resources(dev->bus->self, res->flags);
		if (ret)
			goto error_resize;
	}
	return 0;

error_resize:
	pci_rebar_set_size(dev, resno, old);
	res->end = res->start + pci_rebar_size_to_bytes(old) - 1;
	return ret;
}
EXPORT_SYMBOL(pci_resize_resource);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int pci_enable_resources(struct pci_dev *dev, int mask)
{
	u16 cmd, old_cmd;
	int i;
	struct resource *r;
<<<<<<< HEAD
=======
	const char *r_name;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pci_read_config_word(dev, PCI_COMMAND, &cmd);
	old_cmd = cmd;

<<<<<<< HEAD
	for (i = 0; i < PCI_NUM_RESOURCES; i++) {
		if (!(mask & (1 << i)))
			continue;

		r = &dev->resource[i];
=======
	pci_dev_for_each_resource(dev, r, i) {
		if (!(mask & (1 << i)))
			continue;

		r_name = pci_resource_name(dev, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!(r->flags & (IORESOURCE_IO | IORESOURCE_MEM)))
			continue;
		if ((i == PCI_ROM_RESOURCE) &&
				(!(r->flags & IORESOURCE_ROM_ENABLE)))
			continue;

<<<<<<< HEAD
		if (!r->parent) {
			dev_err(&dev->dev, "device not available "
				"(can't reserve %pR)\n", r);
=======
		if (r->flags & IORESOURCE_UNSET) {
			pci_err(dev, "%s %pR: not assigned; can't enable device\n",
				r_name, r);
			return -EINVAL;
		}

		if (!r->parent) {
			pci_err(dev, "%s %pR: not claimed; can't enable device\n",
				r_name, r);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}

		if (r->flags & IORESOURCE_IO)
			cmd |= PCI_COMMAND_IO;
		if (r->flags & IORESOURCE_MEM)
			cmd |= PCI_COMMAND_MEMORY;
	}

	if (cmd != old_cmd) {
<<<<<<< HEAD
		dev_info(&dev->dev, "enabling device (%04x -> %04x)\n",
			 old_cmd, cmd);
=======
		pci_info(dev, "enabling device (%04x -> %04x)\n", old_cmd, cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pci_write_config_word(dev, PCI_COMMAND, cmd);
	}
	return 0;
}
