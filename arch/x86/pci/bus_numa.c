<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/range.h>

#include "bus_numa.h"

<<<<<<< HEAD
int pci_root_num;
struct pci_root_info pci_root_info[PCI_ROOT_NR];

void x86_pci_root_bus_resources(int bus, struct list_head *resources)
{
	int i;
	int j;
	struct pci_root_info *info;

	if (!pci_root_num)
		goto default_resources;

	for (i = 0; i < pci_root_num; i++) {
		if (pci_root_info[i].bus_min == bus)
			break;
	}

	if (i == pci_root_num)
=======
LIST_HEAD(pci_root_infos);

static struct pci_root_info *x86_find_pci_root_info(int bus)
{
	struct pci_root_info *info;

	list_for_each_entry(info, &pci_root_infos, list)
		if (info->busn.start == bus)
			return info;

	return NULL;
}

int x86_pci_root_bus_node(int bus)
{
	struct pci_root_info *info = x86_find_pci_root_info(bus);

	if (!info)
		return NUMA_NO_NODE;

	return info->node;
}

void x86_pci_root_bus_resources(int bus, struct list_head *resources)
{
	struct pci_root_info *info = x86_find_pci_root_info(bus);
	struct pci_root_res *root_res;
	struct resource_entry *window;
	bool found = false;

	if (!info)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto default_resources;

	printk(KERN_DEBUG "PCI: root bus %02x: hardware-probed resources\n",
	       bus);

<<<<<<< HEAD
	info = &pci_root_info[i];
	for (j = 0; j < info->res_num; j++) {
		struct resource *res;
		struct resource *root;

		res = &info->res[j];
		pci_add_resource(resources, res);
		if (res->flags & IORESOURCE_IO)
			root = &ioport_resource;
		else
			root = &iomem_resource;
		insert_resource(root, res);
	}
=======
	/* already added by acpi ? */
	resource_list_for_each_entry(window, resources)
		if (window->res->flags & IORESOURCE_BUS) {
			found = true;
			break;
		}

	if (!found)
		pci_add_resource(resources, &info->busn);

	list_for_each_entry(root_res, &info->resources, list)
		pci_add_resource(resources, &root_res->res);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return;

default_resources:
	/*
	 * We don't have any host bridge aperture information from the
	 * "native host bridge drivers," e.g., amd_bus or broadcom_bus,
	 * so fall back to the defaults historically used by pci_create_bus().
	 */
	printk(KERN_DEBUG "PCI: root bus %02x: using default resources\n", bus);
	pci_add_resource(resources, &ioport_resource);
	pci_add_resource(resources, &iomem_resource);
}

<<<<<<< HEAD
void __devinit update_res(struct pci_root_info *info, resource_size_t start,
			  resource_size_t end, unsigned long flags, int merge)
{
	int i;
	struct resource *res;
=======
struct pci_root_info __init *alloc_pci_root_info(int bus_min, int bus_max,
						 int node, int link)
{
	struct pci_root_info *info;

	info = kzalloc(sizeof(*info), GFP_KERNEL);

	if (!info)
		return info;

	sprintf(info->name, "PCI Bus #%02x", bus_min);

	INIT_LIST_HEAD(&info->resources);
	info->busn.name  = info->name;
	info->busn.start = bus_min;
	info->busn.end   = bus_max;
	info->busn.flags = IORESOURCE_BUS;
	info->node = node;
	info->link = link;

	list_add_tail(&info->list, &pci_root_infos);

	return info;
}

void update_res(struct pci_root_info *info, resource_size_t start,
		resource_size_t end, unsigned long flags, int merge)
{
	struct resource *res;
	struct pci_root_res *root_res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (start > end)
		return;

<<<<<<< HEAD
	if (start == MAX_RESOURCE)
=======
	if (start == RESOURCE_SIZE_MAX)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	if (!merge)
		goto addit;

	/* try to merge it with old one */
<<<<<<< HEAD
	for (i = 0; i < info->res_num; i++) {
		resource_size_t final_start, final_end;
		resource_size_t common_start, common_end;

		res = &info->res[i];
=======
	list_for_each_entry(root_res, &info->resources, list) {
		resource_size_t final_start, final_end;
		resource_size_t common_start, common_end;

		res = &root_res->res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (res->flags != flags)
			continue;

		common_start = max(res->start, start);
		common_end = min(res->end, end);
		if (common_start > common_end + 1)
			continue;

		final_start = min(res->start, start);
		final_end = max(res->end, end);

		res->start = final_start;
		res->end = final_end;
		return;
	}

addit:

	/* need to add that */
<<<<<<< HEAD
	if (info->res_num >= RES_NUM)
		return;

	res = &info->res[info->res_num];
=======
	root_res = kzalloc(sizeof(*root_res), GFP_KERNEL);
	if (!root_res)
		return;

	res = &root_res->res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	res->name = info->name;
	res->flags = flags;
	res->start = start;
	res->end = end;
<<<<<<< HEAD
	res->child = NULL;
	info->res_num++;
=======

	list_add_tail(&root_res->list, &info->resources);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
