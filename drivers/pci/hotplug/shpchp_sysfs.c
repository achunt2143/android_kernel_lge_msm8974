<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Compaq Hot Plug Controller Driver
 *
 * Copyright (c) 1995,2001 Compaq Computer Corporation
 * Copyright (c) 2001,2003 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (c) 2001 IBM Corp.
 *
 * All rights reserved.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Send feedback to <greg@kroah.com>
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pci.h>
#include "shpchp.h"


/* A few routines that create sysfs entries for the hot plug controller */

<<<<<<< HEAD
static ssize_t show_ctrl (struct device *dev, struct device_attribute *attr, char *buf)
{
	struct pci_dev *pdev;
	char * out = buf;
	int index, busnr;
	struct resource *res;
	struct pci_bus *bus;

	pdev = container_of (dev, struct pci_dev, dev);
	bus = pdev->subordinate;

	out += sprintf(buf, "Free resources: memory\n");
	pci_bus_for_each_resource(bus, res, index) {
		if (res && (res->flags & IORESOURCE_MEM) &&
				!(res->flags & IORESOURCE_PREFETCH)) {
			out += sprintf(out, "start = %8.8llx, length = %8.8llx\n",
				       (unsigned long long)res->start,
				       (unsigned long long)resource_size(res));
		}
	}
	out += sprintf(out, "Free resources: prefetchable memory\n");
	pci_bus_for_each_resource(bus, res, index) {
		if (res && (res->flags & IORESOURCE_MEM) &&
			       (res->flags & IORESOURCE_PREFETCH)) {
			out += sprintf(out, "start = %8.8llx, length = %8.8llx\n",
				       (unsigned long long)res->start,
				       (unsigned long long)resource_size(res));
		}
	}
	out += sprintf(out, "Free resources: IO\n");
	pci_bus_for_each_resource(bus, res, index) {
		if (res && (res->flags & IORESOURCE_IO)) {
			out += sprintf(out, "start = %8.8llx, length = %8.8llx\n",
				       (unsigned long long)res->start,
				       (unsigned long long)resource_size(res));
		}
	}
	out += sprintf(out, "Free resources: bus numbers\n");
	for (busnr = bus->secondary; busnr <= bus->subordinate; busnr++) {
		if (!pci_find_bus(pci_domain_nr(bus), busnr))
			break;
	}
	if (busnr < bus->subordinate)
		out += sprintf(out, "start = %8.8x, length = %8.8x\n",
				busnr, (bus->subordinate - busnr));

	return out - buf;
}
static DEVICE_ATTR (ctrl, S_IRUGO, show_ctrl, NULL);

int __must_check shpchp_create_ctrl_files (struct controller *ctrl)
{
	return device_create_file (&ctrl->pci_dev->dev, &dev_attr_ctrl);
=======
static ssize_t show_ctrl(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct pci_dev *pdev;
	struct resource *res;
	struct pci_bus *bus;
	size_t len = 0;
	int busnr;

	pdev = to_pci_dev(dev);
	bus = pdev->subordinate;

	len += sysfs_emit_at(buf, len, "Free resources: memory\n");
	pci_bus_for_each_resource(bus, res) {
		if (res && (res->flags & IORESOURCE_MEM) &&
				!(res->flags & IORESOURCE_PREFETCH)) {
			len += sysfs_emit_at(buf, len,
					     "start = %8.8llx, length = %8.8llx\n",
					     (unsigned long long)res->start,
					     (unsigned long long)resource_size(res));
		}
	}
	len += sysfs_emit_at(buf, len, "Free resources: prefetchable memory\n");
	pci_bus_for_each_resource(bus, res) {
		if (res && (res->flags & IORESOURCE_MEM) &&
			       (res->flags & IORESOURCE_PREFETCH)) {
			len += sysfs_emit_at(buf, len,
					     "start = %8.8llx, length = %8.8llx\n",
					     (unsigned long long)res->start,
					     (unsigned long long)resource_size(res));
		}
	}
	len += sysfs_emit_at(buf, len, "Free resources: IO\n");
	pci_bus_for_each_resource(bus, res) {
		if (res && (res->flags & IORESOURCE_IO)) {
			len += sysfs_emit_at(buf, len,
					     "start = %8.8llx, length = %8.8llx\n",
					     (unsigned long long)res->start,
					     (unsigned long long)resource_size(res));
		}
	}
	len += sysfs_emit_at(buf, len, "Free resources: bus numbers\n");
	for (busnr = bus->busn_res.start; busnr <= bus->busn_res.end; busnr++) {
		if (!pci_find_bus(pci_domain_nr(bus), busnr))
			break;
	}
	if (busnr < bus->busn_res.end)
		len += sysfs_emit_at(buf, len,
				     "start = %8.8x, length = %8.8x\n",
				     busnr, (int)(bus->busn_res.end - busnr));

	return len;
}
static DEVICE_ATTR(ctrl, S_IRUGO, show_ctrl, NULL);

int shpchp_create_ctrl_files(struct controller *ctrl)
{
	return device_create_file(&ctrl->pci_dev->dev, &dev_attr_ctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void shpchp_remove_ctrl_files(struct controller *ctrl)
{
	device_remove_file(&ctrl->pci_dev->dev, &dev_attr_ctrl);
}
