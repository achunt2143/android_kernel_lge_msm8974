<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0

#define pr_fmt(fmt) "PCI: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pci.h>
#include <linux/acpi.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/dmi.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/pci-acpi.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/numa.h>
#include <asm/pci_x86.h>

struct pci_root_info {
<<<<<<< HEAD
	struct acpi_device *bridge;
	char *name;
	unsigned int res_num;
	struct resource *res;
	struct list_head *resources;
	int busnum;
};

static bool pci_use_crs = true;
=======
	struct acpi_pci_root_info common;
	struct pci_sysdata sd;
#ifdef	CONFIG_PCI_MMCONFIG
	bool mcfg_added;
	u8 start_bus;
	u8 end_bus;
#endif
};

bool pci_use_e820 = true;
static bool pci_use_crs = true;
static bool pci_ignore_seg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init set_use_crs(const struct dmi_system_id *id)
{
	pci_use_crs = true;
	return 0;
}

static int __init set_nouse_crs(const struct dmi_system_id *id)
{
	pci_use_crs = false;
	return 0;
}

<<<<<<< HEAD
static const struct dmi_system_id pci_use_crs_table[] __initconst = {
=======
static int __init set_ignore_seg(const struct dmi_system_id *id)
{
	pr_info("%s detected: ignoring ACPI _SEG\n", id->ident);
	pci_ignore_seg = true;
	return 0;
}

static int __init set_no_e820(const struct dmi_system_id *id)
{
	pr_info("%s detected: not clipping E820 regions from _CRS\n",
	        id->ident);
	pci_use_e820 = false;
	return 0;
}

static const struct dmi_system_id pci_crs_quirks[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* http://bugzilla.kernel.org/show_bug.cgi?id=14183 */
	{
		.callback = set_use_crs,
		.ident = "IBM System x3800",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "IBM"),
			DMI_MATCH(DMI_PRODUCT_NAME, "x3800"),
		},
	},
	/* https://bugzilla.kernel.org/show_bug.cgi?id=16007 */
	/* 2006 AMD HT/VIA system with two host bridges */
        {
		.callback = set_use_crs,
		.ident = "ASRock ALiveSATA2-GLAN",
		.matches = {
			DMI_MATCH(DMI_PRODUCT_NAME, "ALiveSATA2-GLAN"),
                },
        },
	/* https://bugzilla.kernel.org/show_bug.cgi?id=30552 */
	/* 2006 AMD HT/VIA system with two host bridges */
	{
		.callback = set_use_crs,
		.ident = "ASUS M2V-MX SE",
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "M2V-MX SE"),
			DMI_MATCH(DMI_BIOS_VENDOR, "American Megatrends Inc."),
		},
	},
	/* https://bugzilla.kernel.org/show_bug.cgi?id=42619 */
	{
		.callback = set_use_crs,
		.ident = "MSI MS-7253",
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "MICRO-STAR INTERNATIONAL CO., LTD"),
			DMI_MATCH(DMI_BOARD_NAME, "MS-7253"),
			DMI_MATCH(DMI_BIOS_VENDOR, "Phoenix Technologies, LTD"),
		},
	},
	/* https://bugs.launchpad.net/ubuntu/+source/alsa-driver/+bug/931368 */
	/* https://bugs.launchpad.net/ubuntu/+source/alsa-driver/+bug/1033299 */
	{
		.callback = set_use_crs,
		.ident = "Foxconn K8M890-8237A",
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "Foxconn"),
			DMI_MATCH(DMI_BOARD_NAME, "K8M890-8237A"),
			DMI_MATCH(DMI_BIOS_VENDOR, "Phoenix Technologies, LTD"),
		},
	},

	/* Now for the blacklist.. */

	/* https://bugzilla.redhat.com/show_bug.cgi?id=769657 */
	{
		.callback = set_nouse_crs,
		.ident = "Dell Studio 1557",
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Studio 1557"),
			DMI_MATCH(DMI_BIOS_VERSION, "A09"),
		},
	},
	/* https://bugzilla.redhat.com/show_bug.cgi?id=769657 */
	{
		.callback = set_nouse_crs,
		.ident = "Thinkpad SL510",
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "LENOVO"),
			DMI_MATCH(DMI_BOARD_NAME, "2847DFG"),
			DMI_MATCH(DMI_BIOS_VERSION, "6JET85WW (1.43 )"),
		},
	},
<<<<<<< HEAD
=======
	/* https://bugzilla.kernel.org/show_bug.cgi?id=42606 */
	{
		.callback = set_nouse_crs,
		.ident = "Supermicro X8DTH",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Supermicro"),
			DMI_MATCH(DMI_PRODUCT_NAME, "X8DTH-i/6/iF/6F"),
			DMI_MATCH(DMI_BIOS_VERSION, "2.0a"),
		},
	},

	/* https://bugzilla.kernel.org/show_bug.cgi?id=15362 */
	{
		.callback = set_ignore_seg,
		.ident = "HP xw9300",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Hewlett-Packard"),
			DMI_MATCH(DMI_PRODUCT_NAME, "HP xw9300 Workstation"),
		},
	},

	/*
	 * Many Lenovo models with "IIL" in their DMI_PRODUCT_VERSION have
	 * an E820 reserved region that covers the entire 32-bit host
	 * bridge memory window from _CRS.  Using the E820 region to clip
	 * _CRS means no space is available for hot-added or uninitialized
	 * PCI devices.  This typically breaks I2C controllers for touchpads
	 * and hot-added Thunderbolt devices.  See the commit log for
	 * models known to require this quirk and related bug reports.
	 */
	{
		.callback = set_no_e820,
		.ident = "Lenovo *IIL* product version",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
			DMI_MATCH(DMI_PRODUCT_VERSION, "IIL"),
		},
	},

	/*
	 * The Acer Spin 5 (SP513-54N) has the same E820 reservation covering
	 * the entire _CRS 32-bit window issue as the Lenovo *IIL* models.
	 * See https://bugs.launchpad.net/bugs/1884232
	 */
	{
		.callback = set_no_e820,
		.ident = "Acer Spin 5 (SP513-54N)",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Acer"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Spin SP513-54N"),
		},
	},

	/*
	 * Clevo X170KM-G barebones have the same E820 reservation covering
	 * the entire _CRS 32-bit window issue as the Lenovo *IIL* models.
	 * See https://bugzilla.kernel.org/show_bug.cgi?id=214259
	 */
	{
		.callback = set_no_e820,
		.ident = "Clevo X170KM-G Barebone",
		.matches = {
			DMI_MATCH(DMI_BOARD_NAME, "X170KM-G"),
		},
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{}
};

void __init pci_acpi_crs_quirks(void)
{
<<<<<<< HEAD
	int year;

	if (dmi_get_date(DMI_BIOS_DATE, &year, NULL, NULL) && year < 2008)
		pci_use_crs = false;

	dmi_check_system(pci_use_crs_table);
=======
	int year = dmi_get_bios_year();

	if (year >= 0 && year < 2008 && iomem_resource.end <= 0xffffffff)
		pci_use_crs = false;

	/*
	 * Some firmware includes unusable space (host bridge registers,
	 * hidden PCI device BARs, etc) in PCI host bridge _CRS.  This is a
	 * firmware defect, and 4dc2287c1805 ("x86: avoid E820 regions when
	 * allocating address space") has clipped out the unusable space in
	 * the past.
	 *
	 * But other firmware supplies E820 reserved regions that cover
	 * entire _CRS windows, so clipping throws away the entire window,
	 * leaving none for hot-added or uninitialized devices.  These E820
	 * entries are probably *not* a firmware defect, so disable the
	 * clipping by default for post-2022 machines.
	 *
	 * We already have quirks to disable clipping for pre-2023
	 * machines, and we'll likely need quirks to *enable* clipping for
	 * post-2022 machines that incorrectly include unusable space in
	 * _CRS.
	 */
	if (year >= 2023)
		pci_use_e820 = false;

	dmi_check_system(pci_crs_quirks);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If the user specifies "pci=use_crs" or "pci=nocrs" explicitly, that
	 * takes precedence over anything we figured out above.
	 */
	if (pci_probe & PCI_ROOT_NO_CRS)
		pci_use_crs = false;
	else if (pci_probe & PCI_USE__CRS)
		pci_use_crs = true;

<<<<<<< HEAD
	printk(KERN_INFO "PCI: %s host bridge windows from ACPI; "
	       "if necessary, use \"pci=%s\" and report a bug\n",
	       pci_use_crs ? "Using" : "Ignoring",
	       pci_use_crs ? "nocrs" : "use_crs");
}

static acpi_status
resource_to_addr(struct acpi_resource *resource,
			struct acpi_resource_address64 *addr)
{
	acpi_status status;
	struct acpi_resource_memory24 *memory24;
	struct acpi_resource_memory32 *memory32;
	struct acpi_resource_fixed_memory32 *fixed_memory32;

	memset(addr, 0, sizeof(*addr));
	switch (resource->type) {
	case ACPI_RESOURCE_TYPE_MEMORY24:
		memory24 = &resource->data.memory24;
		addr->resource_type = ACPI_MEMORY_RANGE;
		addr->minimum = memory24->minimum;
		addr->address_length = memory24->address_length;
		addr->maximum = addr->minimum + addr->address_length - 1;
		return AE_OK;
	case ACPI_RESOURCE_TYPE_MEMORY32:
		memory32 = &resource->data.memory32;
		addr->resource_type = ACPI_MEMORY_RANGE;
		addr->minimum = memory32->minimum;
		addr->address_length = memory32->address_length;
		addr->maximum = addr->minimum + addr->address_length - 1;
		return AE_OK;
	case ACPI_RESOURCE_TYPE_FIXED_MEMORY32:
		fixed_memory32 = &resource->data.fixed_memory32;
		addr->resource_type = ACPI_MEMORY_RANGE;
		addr->minimum = fixed_memory32->address;
		addr->address_length = fixed_memory32->address_length;
		addr->maximum = addr->minimum + addr->address_length - 1;
		return AE_OK;
	case ACPI_RESOURCE_TYPE_ADDRESS16:
	case ACPI_RESOURCE_TYPE_ADDRESS32:
	case ACPI_RESOURCE_TYPE_ADDRESS64:
		status = acpi_resource_to_address64(resource, addr);
		if (ACPI_SUCCESS(status) &&
		    (addr->resource_type == ACPI_MEMORY_RANGE ||
		    addr->resource_type == ACPI_IO_RANGE) &&
		    addr->address_length > 0) {
			return AE_OK;
		}
		break;
	}
	return AE_ERROR;
}

static acpi_status
count_resource(struct acpi_resource *acpi_res, void *data)
{
	struct pci_root_info *info = data;
	struct acpi_resource_address64 addr;
	acpi_status status;

	status = resource_to_addr(acpi_res, &addr);
	if (ACPI_SUCCESS(status))
		info->res_num++;
	return AE_OK;
}

static acpi_status
setup_resource(struct acpi_resource *acpi_res, void *data)
{
	struct pci_root_info *info = data;
	struct resource *res;
	struct acpi_resource_address64 addr;
	acpi_status status;
	unsigned long flags;
	u64 start, orig_end, end;

	status = resource_to_addr(acpi_res, &addr);
	if (!ACPI_SUCCESS(status))
		return AE_OK;

	if (addr.resource_type == ACPI_MEMORY_RANGE) {
		flags = IORESOURCE_MEM;
		if (addr.info.mem.caching == ACPI_PREFETCHABLE_MEMORY)
			flags |= IORESOURCE_PREFETCH;
	} else if (addr.resource_type == ACPI_IO_RANGE) {
		flags = IORESOURCE_IO;
	} else
		return AE_OK;

	start = addr.minimum + addr.translation_offset;
	orig_end = end = addr.maximum + addr.translation_offset;

	/* Exclude non-addressable range or non-addressable portion of range */
	end = min(end, (u64)iomem_resource.end);
	if (end <= start) {
		dev_info(&info->bridge->dev,
			"host bridge window [%#llx-%#llx] "
			"(ignored, not CPU addressable)\n", start, orig_end);
		return AE_OK;
	} else if (orig_end != end) {
		dev_info(&info->bridge->dev,
			"host bridge window [%#llx-%#llx] "
			"([%#llx-%#llx] ignored, not CPU addressable)\n", 
			start, orig_end, end + 1, orig_end);
	}

	res = &info->res[info->res_num];
	res->name = info->name;
	res->flags = flags;
	res->start = start;
	res->end = end;
	res->child = NULL;

	if (!pci_use_crs) {
		dev_printk(KERN_DEBUG, &info->bridge->dev,
			   "host bridge window %pR (ignored)\n", res);
		return AE_OK;
	}

	info->res_num++;
	if (addr.translation_offset)
		dev_info(&info->bridge->dev, "host bridge window %pR "
			 "(PCI address [%#llx-%#llx])\n",
			 res, res->start - addr.translation_offset,
			 res->end - addr.translation_offset);
	else
		dev_info(&info->bridge->dev, "host bridge window %pR\n", res);

	return AE_OK;
}

static bool resource_contains(struct resource *res, resource_size_t point)
{
	if (res->start <= point && point <= res->end)
		return true;
	return false;
}

static void coalesce_windows(struct pci_root_info *info, unsigned long type)
{
	int i, j;
	struct resource *res1, *res2;

	for (i = 0; i < info->res_num; i++) {
		res1 = &info->res[i];
		if (!(res1->flags & type))
			continue;

		for (j = i + 1; j < info->res_num; j++) {
			res2 = &info->res[j];
			if (!(res2->flags & type))
				continue;

			/*
			 * I don't like throwing away windows because then
			 * our resources no longer match the ACPI _CRS, but
			 * the kernel resource tree doesn't allow overlaps.
			 */
			if (resource_contains(res1, res2->start) ||
			    resource_contains(res1, res2->end) ||
			    resource_contains(res2, res1->start) ||
			    resource_contains(res2, res1->end)) {
				res1->start = min(res1->start, res2->start);
				res1->end = max(res1->end, res2->end);
				dev_info(&info->bridge->dev,
					 "host bridge window expanded to %pR; %pR ignored\n",
					 res1, res2);
				res2->flags = 0;
			}
		}
	}
}

static void add_resources(struct pci_root_info *info)
{
	int i;
	struct resource *res, *root, *conflict;

	coalesce_windows(info, IORESOURCE_MEM);
	coalesce_windows(info, IORESOURCE_IO);

	for (i = 0; i < info->res_num; i++) {
		res = &info->res[i];

		if (res->flags & IORESOURCE_MEM)
			root = &iomem_resource;
		else if (res->flags & IORESOURCE_IO)
			root = &ioport_resource;
		else
			continue;

		conflict = insert_resource_conflict(root, res);
		if (conflict)
			dev_info(&info->bridge->dev,
				 "ignoring host bridge window %pR (conflicts with %s %pR)\n",
				 res, conflict->name, conflict);
		else
			pci_add_resource(info->resources, res);
	}
}

static void
get_current_resources(struct acpi_device *device, int busnum,
		      int domain, struct list_head *resources)
{
	struct pci_root_info info;
	size_t size;

	info.bridge = device;
	info.res_num = 0;
	info.resources = resources;
	acpi_walk_resources(device->handle, METHOD_NAME__CRS, count_resource,
				&info);
	if (!info.res_num)
		return;

	size = sizeof(*info.res) * info.res_num;
	info.res = kmalloc(size, GFP_KERNEL);
	if (!info.res)
		return;

	info.name = kasprintf(GFP_KERNEL, "PCI Bus %04x:%02x", domain, busnum);
	if (!info.name)
		goto name_alloc_fail;

	info.res_num = 0;
	acpi_walk_resources(device->handle, METHOD_NAME__CRS, setup_resource,
				&info);

	if (pci_use_crs) {
		add_resources(&info);

		return;
	}

	kfree(info.name);

name_alloc_fail:
	kfree(info.res);
}

struct pci_bus * __devinit pci_acpi_scan_root(struct acpi_pci_root *root)
{
	struct acpi_device *device = root->device;
	int domain = root->segment;
	int busnum = root->secondary.start;
	LIST_HEAD(resources);
	struct pci_bus *bus;
	struct pci_sysdata *sd;
	int node;
#ifdef CONFIG_ACPI_NUMA
	int pxm;
#endif

	if (domain && !pci_domains_supported) {
		printk(KERN_WARNING "pci_bus %04x:%02x: "
		       "ignored (multiple domains not supported)\n",
		       domain, busnum);
		return NULL;
	}

	node = -1;
#ifdef CONFIG_ACPI_NUMA
	pxm = acpi_get_pxm(device->handle);
	if (pxm >= 0)
		node = pxm_to_node(pxm);
	if (node != -1)
		set_mp_bus_to_node(busnum, node);
	else
#endif
		node = get_mp_bus_to_node(busnum);

	if (node != -1 && !node_online(node))
		node = -1;

	/* Allocate per-root-bus (not per bus) arch-specific data.
	 * TODO: leak; this memory is never freed.
	 * It's arguable whether it's worth the trouble to care.
	 */
	sd = kzalloc(sizeof(*sd), GFP_KERNEL);
	if (!sd) {
		printk(KERN_WARNING "pci_bus %04x:%02x: "
		       "ignored (out of memory)\n", domain, busnum);
		return NULL;
	}

	sd->domain = domain;
	sd->node = node;
	/*
	 * Maybe the desired pci bus has been already scanned. In such case
	 * it is unnecessary to scan the pci bus with the given domain,busnum.
	 */
	bus = pci_find_bus(domain, busnum);
	if (bus) {
		/*
		 * If the desired bus exits, the content of bus->sysdata will
		 * be replaced by sd.
		 */
		memcpy(bus->sysdata, sd, sizeof(*sd));
		kfree(sd);
	} else {
		get_current_resources(device, busnum, domain, &resources);

		/*
		 * _CRS with no apertures is normal, so only fall back to
		 * defaults or native bridge info if we're ignoring _CRS.
		 */
		if (!pci_use_crs)
			x86_pci_root_bus_resources(busnum, &resources);
		bus = pci_create_root_bus(NULL, busnum, &pci_root_ops, sd,
					  &resources);
		if (bus)
			bus->subordinate = pci_scan_child_bus(bus);
		else
			pci_free_resource_list(&resources);
=======
	pr_info("%s host bridge windows from ACPI; if necessary, use \"pci=%s\" and report a bug\n",
	        pci_use_crs ? "Using" : "Ignoring",
	        pci_use_crs ? "nocrs" : "use_crs");

	/* "pci=use_e820"/"pci=no_e820" on the kernel cmdline takes precedence */
	if (pci_probe & PCI_NO_E820)
		pci_use_e820 = false;
	else if (pci_probe & PCI_USE_E820)
		pci_use_e820 = true;

	pr_info("%s E820 reservations for host bridge windows\n",
	        pci_use_e820 ? "Using" : "Ignoring");
	if (pci_probe & (PCI_NO_E820 | PCI_USE_E820))
		pr_info("Please notify linux-pci@vger.kernel.org so future kernels can do this automatically\n");
}

#ifdef	CONFIG_PCI_MMCONFIG
static int check_segment(u16 seg, struct device *dev, char *estr)
{
	if (seg) {
		dev_err(dev, "%s can't access configuration space under this host bridge\n",
			estr);
		return -EIO;
	}

	/*
	 * Failure in adding MMCFG information is not fatal,
	 * just can't access extended configuration space of
	 * devices under this host bridge.
	 */
	dev_warn(dev, "%s can't access extended configuration space under this bridge\n",
		 estr);

	return 0;
}

static int setup_mcfg_map(struct acpi_pci_root_info *ci)
{
	int result, seg;
	struct pci_root_info *info;
	struct acpi_pci_root *root = ci->root;
	struct device *dev = &ci->bridge->dev;

	info = container_of(ci, struct pci_root_info, common);
	info->start_bus = (u8)root->secondary.start;
	info->end_bus = (u8)root->secondary.end;
	info->mcfg_added = false;
	seg = info->sd.domain;

	dev_dbg(dev, "%s(%04x %pR ECAM %pa)\n", __func__, seg,
		&root->secondary, &root->mcfg_addr);

	/* return success if MMCFG is not in use */
	if (raw_pci_ext_ops && raw_pci_ext_ops != &pci_mmcfg)
		return 0;

	if (!(pci_probe & PCI_PROBE_MMCONF))
		return check_segment(seg, dev, "MMCONFIG is disabled,");

	result = pci_mmconfig_insert(dev, seg, info->start_bus, info->end_bus,
				     root->mcfg_addr);
	if (result == 0) {
		/* enable MMCFG if it hasn't been enabled yet */
		if (raw_pci_ext_ops == NULL)
			raw_pci_ext_ops = &pci_mmcfg;
		info->mcfg_added = true;
	} else if (result != -EEXIST)
		return check_segment(seg, dev,
			 "fail to add MMCONFIG information,");

	return 0;
}

static void teardown_mcfg_map(struct acpi_pci_root_info *ci)
{
	struct pci_root_info *info;

	info = container_of(ci, struct pci_root_info, common);
	if (info->mcfg_added) {
		pci_mmconfig_delete(info->sd.domain,
				    info->start_bus, info->end_bus);
		info->mcfg_added = false;
	}
}
#else
static int setup_mcfg_map(struct acpi_pci_root_info *ci)
{
	return 0;
}

static void teardown_mcfg_map(struct acpi_pci_root_info *ci)
{
}
#endif

static int pci_acpi_root_get_node(struct acpi_pci_root *root)
{
	int busnum = root->secondary.start;
	struct acpi_device *device = root->device;
	int node = acpi_get_node(device->handle);

	if (node == NUMA_NO_NODE) {
		node = x86_pci_root_bus_node(busnum);
		if (node != 0 && node != NUMA_NO_NODE)
			dev_info(&device->dev, FW_BUG "no _PXM; falling back to node %d from hardware (may be inconsistent with ACPI node numbers)\n",
				node);
	}
	if (node != NUMA_NO_NODE && !node_online(node))
		node = NUMA_NO_NODE;

	return node;
}

static int pci_acpi_root_init_info(struct acpi_pci_root_info *ci)
{
	return setup_mcfg_map(ci);
}

static void pci_acpi_root_release_info(struct acpi_pci_root_info *ci)
{
	teardown_mcfg_map(ci);
	kfree(container_of(ci, struct pci_root_info, common));
}

/*
 * An IO port or MMIO resource assigned to a PCI host bridge may be
 * consumed by the host bridge itself or available to its child
 * bus/devices. The ACPI specification defines a bit (Producer/Consumer)
 * to tell whether the resource is consumed by the host bridge itself,
 * but firmware hasn't used that bit consistently, so we can't rely on it.
 *
 * On x86 and IA64 platforms, all IO port and MMIO resources are assumed
 * to be available to child bus/devices except one special case:
 *     IO port [0xCF8-0xCFF] is consumed by the host bridge itself
 *     to access PCI configuration space.
 *
 * So explicitly filter out PCI CFG IO ports[0xCF8-0xCFF].
 */
static bool resource_is_pcicfg_ioport(struct resource *res)
{
	return (res->flags & IORESOURCE_IO) &&
		res->start == 0xCF8 && res->end == 0xCFF;
}

static int pci_acpi_root_prepare_resources(struct acpi_pci_root_info *ci)
{
	struct acpi_device *device = ci->bridge;
	int busnum = ci->root->secondary.start;
	struct resource_entry *entry, *tmp;
	int status;

	status = acpi_pci_probe_root_resources(ci);

	if (pci_use_crs) {
		resource_list_for_each_entry_safe(entry, tmp, &ci->resources)
			if (resource_is_pcicfg_ioport(entry->res))
				resource_list_destroy_entry(entry);
		return status;
	}

	resource_list_for_each_entry_safe(entry, tmp, &ci->resources) {
		dev_printk(KERN_DEBUG, &device->dev,
			   "host bridge window %pR (ignored)\n", entry->res);
		resource_list_destroy_entry(entry);
	}
	x86_pci_root_bus_resources(busnum, &ci->resources);

	return 0;
}

static struct acpi_pci_root_ops acpi_pci_root_ops = {
	.pci_ops = &pci_root_ops,
	.init_info = pci_acpi_root_init_info,
	.release_info = pci_acpi_root_release_info,
	.prepare_resources = pci_acpi_root_prepare_resources,
};

struct pci_bus *pci_acpi_scan_root(struct acpi_pci_root *root)
{
	int domain = root->segment;
	int busnum = root->secondary.start;
	int node = pci_acpi_root_get_node(root);
	struct pci_bus *bus;

	if (pci_ignore_seg)
		root->segment = domain = 0;

	if (domain && !pci_domains_supported) {
		pr_warn("pci_bus %04x:%02x: ignored (multiple domains not supported)\n",
		        domain, busnum);
		return NULL;
	}

	bus = pci_find_bus(domain, busnum);
	if (bus) {
		/*
		 * If the desired bus has been scanned already, replace
		 * its bus->sysdata.
		 */
		struct pci_sysdata sd = {
			.domain = domain,
			.node = node,
			.companion = root->device
		};

		memcpy(bus->sysdata, &sd, sizeof(sd));
	} else {
		struct pci_root_info *info;

		info = kzalloc(sizeof(*info), GFP_KERNEL);
		if (!info)
			dev_err(&root->device->dev,
				"pci_bus %04x:%02x: ignored (out of memory)\n",
				domain, busnum);
		else {
			info->sd.domain = domain;
			info->sd.node = node;
			info->sd.companion = root->device;
			bus = acpi_pci_root_create(root, &acpi_pci_root_ops,
						   &info->common, &info->sd);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* After the PCI-E bus has been walked and all devices discovered,
	 * configure any settings of the fabric that might be necessary.
	 */
	if (bus) {
		struct pci_bus *child;
<<<<<<< HEAD
		list_for_each_entry(child, &bus->children, node) {
			struct pci_dev *self = child->self;
			if (!self)
				continue;

			pcie_bus_configure_settings(child, self->pcie_mpss);
		}
	}

	if (!bus)
		kfree(sd);

	if (bus && node != -1) {
#ifdef CONFIG_ACPI_NUMA
		if (pxm >= 0)
			dev_printk(KERN_DEBUG, &bus->dev,
				   "on NUMA node %d (pxm %d)\n", node, pxm);
#else
		dev_printk(KERN_DEBUG, &bus->dev, "on NUMA node %d\n", node);
#endif
=======
		list_for_each_entry(child, &bus->children, node)
			pcie_bus_configure_settings(child);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return bus;
}

<<<<<<< HEAD
=======
int pcibios_root_bridge_prepare(struct pci_host_bridge *bridge)
{
	/*
	 * We pass NULL as parent to pci_create_root_bus(), so if it is not NULL
	 * here, pci_create_root_bus() has been called by someone else and
	 * sysdata is likely to be different from what we expect.  Let it go in
	 * that case.
	 */
	if (!bridge->dev.parent) {
		struct pci_sysdata *sd = bridge->bus->sysdata;
		ACPI_COMPANION_SET(&bridge->dev, sd->companion);
	}
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __init pci_acpi_init(void)
{
	struct pci_dev *dev = NULL;

	if (acpi_noirq)
		return -ENODEV;

<<<<<<< HEAD
	printk(KERN_INFO "PCI: Using ACPI for IRQ routing\n");
=======
	pr_info("Using ACPI for IRQ routing\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acpi_irq_penalty_init();
	pcibios_enable_irq = acpi_pci_irq_enable;
	pcibios_disable_irq = acpi_pci_irq_disable;
	x86_init.pci.init_irq = x86_init_noop;

	if (pci_routeirq) {
		/*
		 * PCI IRQ routing is set up by pci_enable_device(), but we
		 * also do it here in case there are still broken drivers that
		 * don't use pci_enable_device().
		 */
<<<<<<< HEAD
		printk(KERN_INFO "PCI: Routing PCI interrupts for all devices because \"pci=routeirq\" specified\n");
=======
		pr_info("Routing PCI interrupts for all devices because \"pci=routeirq\" specified\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for_each_pci_dev(dev)
			acpi_pci_irq_enable(dev);
	}

	return 0;
}
