<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * drivers/mfd/mfd-core.c
 *
 * core MFD support
 * Copyright (c) 2006 Ian Molton
 * Copyright (c) 2007,2008 Dmitry Baryshkov
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/acpi.h>
<<<<<<< HEAD
=======
#include <linux/list.h>
#include <linux/property.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mfd/core.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/module.h>
<<<<<<< HEAD
#ifdef CONFIG_MACH_LGE
#include <linux/of.h>
#endif

static struct device_type mfd_dev_type = {
	.name	= "mfd_device",
};

int mfd_cell_enable(struct platform_device *pdev)
{
	const struct mfd_cell *cell = mfd_get_cell(pdev);
	int err = 0;

	/* only call enable hook if the cell wasn't previously enabled */
	if (atomic_inc_return(cell->usage_count) == 1)
		err = cell->enable(pdev);

	/* if the enable hook failed, decrement counter to allow retries */
	if (err)
		atomic_dec(cell->usage_count);

	return err;
}
EXPORT_SYMBOL(mfd_cell_enable);

int mfd_cell_disable(struct platform_device *pdev)
{
	const struct mfd_cell *cell = mfd_get_cell(pdev);
	int err = 0;

	/* only disable if no other clients are using it */
	if (atomic_dec_return(cell->usage_count) == 0)
		err = cell->disable(pdev);

	/* if the disable hook failed, increment to allow retries */
	if (err)
		atomic_inc(cell->usage_count);

	/* sanity check; did someone call disable too many times? */
	WARN_ON(atomic_read(cell->usage_count) < 0);

	return err;
}
EXPORT_SYMBOL(mfd_cell_disable);

static int mfd_platform_add_cell(struct platform_device *pdev,
				 const struct mfd_cell *cell)
{
	if (!cell)
		return 0;

	pdev->mfd_cell = kmemdup(cell, sizeof(*cell), GFP_KERNEL);
	if (!pdev->mfd_cell)
		return -ENOMEM;

=======
#include <linux/irqdomain.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/regulator/consumer.h>

static LIST_HEAD(mfd_of_node_list);

struct mfd_of_node_entry {
	struct list_head list;
	struct device *dev;
	struct device_node *np;
};

static const struct device_type mfd_dev_type = {
	.name	= "mfd_device",
};

#if IS_ENABLED(CONFIG_ACPI)
struct match_ids_walk_data {
	struct acpi_device_id *ids;
	struct acpi_device *adev;
};

static int match_device_ids(struct acpi_device *adev, void *data)
{
	struct match_ids_walk_data *wd = data;

	if (!acpi_match_device_ids(adev, wd->ids)) {
		wd->adev = adev;
		return 1;
	}

	return 0;
}

static void mfd_acpi_add_device(const struct mfd_cell *cell,
				struct platform_device *pdev)
{
	const struct mfd_cell_acpi_match *match = cell->acpi_match;
	struct acpi_device *adev = NULL;
	struct acpi_device *parent;

	parent = ACPI_COMPANION(pdev->dev.parent);
	if (!parent)
		return;

	/*
	 * MFD child device gets its ACPI handle either from the ACPI device
	 * directly under the parent that matches the either _HID or _CID, or
	 * _ADR or it will use the parent handle if is no ID is given.
	 *
	 * Note that use of _ADR is a grey area in the ACPI specification,
	 * though at least Intel Galileo Gen 2 is using it to distinguish
	 * the children devices.
	 */
	if (match) {
		if (match->pnpid) {
			struct acpi_device_id ids[2] = {};
			struct match_ids_walk_data wd = {
				.adev = NULL,
				.ids = ids,
			};

			strscpy(ids[0].id, match->pnpid, sizeof(ids[0].id));
			acpi_dev_for_each_child(parent, match_device_ids, &wd);
			adev = wd.adev;
		} else {
			adev = acpi_find_child_device(parent, match->adr, false);
		}
	}

	ACPI_COMPANION_SET(&pdev->dev, adev ?: parent);
}
#else
static inline void mfd_acpi_add_device(const struct mfd_cell *cell,
				       struct platform_device *pdev)
{
}
#endif

static int mfd_match_of_node_to_dev(struct platform_device *pdev,
				    struct device_node *np,
				    const struct mfd_cell *cell)
{
#if IS_ENABLED(CONFIG_OF)
	struct mfd_of_node_entry *of_entry;
	u64 of_node_addr;

	/* Skip if OF node has previously been allocated to a device */
	list_for_each_entry(of_entry, &mfd_of_node_list, list)
		if (of_entry->np == np)
			return -EAGAIN;

	if (!cell->use_of_reg)
		/* No of_reg defined - allocate first free compatible match */
		goto allocate_of_node;

	/* We only care about each node's first defined address */
	if (of_property_read_reg(np, 0, &of_node_addr, NULL))
		/* OF node does not contatin a 'reg' property to match to */
		return -EAGAIN;

	if (cell->of_reg != of_node_addr)
		/* No match */
		return -EAGAIN;

allocate_of_node:
	of_entry = kzalloc(sizeof(*of_entry), GFP_KERNEL);
	if (!of_entry)
		return -ENOMEM;

	of_entry->dev = &pdev->dev;
	of_entry->np = np;
	list_add_tail(&of_entry->list, &mfd_of_node_list);

	pdev->dev.of_node = np;
	pdev->dev.fwnode = &np->fwnode;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int mfd_add_device(struct device *parent, int id,
			  const struct mfd_cell *cell,
			  struct resource *mem_base,
<<<<<<< HEAD
			  int irq_base)
{
	struct resource *res;
	struct platform_device *pdev;
#ifdef CONFIG_MACH_LGE
	struct device_node *np = NULL;
#endif
	int ret = -ENOMEM;
	int r;

	pdev = platform_device_alloc(cell->name, id + cell->id);
	if (!pdev)
		goto fail_alloc;

	res = kzalloc(sizeof(*res) * cell->num_resources, GFP_KERNEL);
=======
			  int irq_base, struct irq_domain *domain)
{
	struct resource *res;
	struct platform_device *pdev;
	struct device_node *np = NULL;
	struct mfd_of_node_entry *of_entry, *tmp;
	bool disabled = false;
	int ret = -ENOMEM;
	int platform_id;
	int r;

	if (id == PLATFORM_DEVID_AUTO)
		platform_id = id;
	else
		platform_id = id + cell->id;

	pdev = platform_device_alloc(cell->name, platform_id);
	if (!pdev)
		goto fail_alloc;

	pdev->mfd_cell = kmemdup(cell, sizeof(*cell), GFP_KERNEL);
	if (!pdev->mfd_cell)
		goto fail_device;

	res = kcalloc(cell->num_resources, sizeof(*res), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!res)
		goto fail_device;

	pdev->dev.parent = parent;
<<<<<<< HEAD
#ifdef CONFIG_MACH_LGE
	if (parent->of_node && cell->of_compatible) {
		for_each_child_of_node(parent->of_node, np) {
			if (of_device_is_compatible(np, cell->of_compatible)) {
				pdev->dev.of_node = np;
				break;
			}
		}
	}
#endif
	pdev->dev.type = &mfd_dev_type;
=======
	pdev->dev.type = &mfd_dev_type;
	pdev->dev.dma_mask = parent->dma_mask;
	pdev->dev.dma_parms = parent->dma_parms;
	pdev->dev.coherent_dma_mask = parent->coherent_dma_mask;

	ret = regulator_bulk_register_supply_alias(
			&pdev->dev, cell->parent_supplies,
			parent, cell->parent_supplies,
			cell->num_parent_supplies);
	if (ret < 0)
		goto fail_res;

	if (IS_ENABLED(CONFIG_OF) && parent->of_node && cell->of_compatible) {
		for_each_child_of_node(parent->of_node, np) {
			if (of_device_is_compatible(np, cell->of_compatible)) {
				/* Skip 'disabled' devices */
				if (!of_device_is_available(np)) {
					disabled = true;
					continue;
				}

				ret = mfd_match_of_node_to_dev(pdev, np, cell);
				if (ret == -EAGAIN)
					continue;
				of_node_put(np);
				if (ret)
					goto fail_alias;

				goto match;
			}
		}

		if (disabled) {
			/* Ignore 'disabled' devices error free */
			ret = 0;
			goto fail_alias;
		}

match:
		if (!pdev->dev.of_node)
			pr_warn("%s: Failed to locate of_node [id: %d]\n",
				cell->name, platform_id);
	}

	mfd_acpi_add_device(cell, pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cell->pdata_size) {
		ret = platform_device_add_data(pdev,
					cell->platform_data, cell->pdata_size);
		if (ret)
<<<<<<< HEAD
			goto fail_res;
	}

	ret = mfd_platform_add_cell(pdev, cell);
	if (ret)
		goto fail_res;
=======
			goto fail_of_entry;
	}

	if (cell->swnode) {
		ret = device_add_software_node(&pdev->dev, cell->swnode);
		if (ret)
			goto fail_of_entry;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (r = 0; r < cell->num_resources; r++) {
		res[r].name = cell->resources[r].name;
		res[r].flags = cell->resources[r].flags;

		/* Find out base to use */
		if ((cell->resources[r].flags & IORESOURCE_MEM) && mem_base) {
			res[r].parent = mem_base;
			res[r].start = mem_base->start +
				cell->resources[r].start;
			res[r].end = mem_base->start +
				cell->resources[r].end;
		} else if (cell->resources[r].flags & IORESOURCE_IRQ) {
<<<<<<< HEAD
			res[r].start = irq_base +
				cell->resources[r].start;
			res[r].end   = irq_base +
				cell->resources[r].end;
=======
			if (domain) {
				/* Unable to create mappings for IRQ ranges. */
				WARN_ON(cell->resources[r].start !=
					cell->resources[r].end);
				res[r].start = res[r].end = irq_create_mapping(
					domain, cell->resources[r].start);
			} else {
				res[r].start = irq_base +
					cell->resources[r].start;
				res[r].end   = irq_base +
					cell->resources[r].end;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			res[r].parent = cell->resources[r].parent;
			res[r].start = cell->resources[r].start;
			res[r].end   = cell->resources[r].end;
		}

		if (!cell->ignore_resource_conflicts) {
<<<<<<< HEAD
			ret = acpi_check_resource_conflict(&res[r]);
			if (ret)
				goto fail_res;
=======
			if (has_acpi_companion(&pdev->dev)) {
				ret = acpi_check_resource_conflict(&res[r]);
				if (ret)
					goto fail_res_conflict;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	ret = platform_device_add_resources(pdev, res, cell->num_resources);
	if (ret)
<<<<<<< HEAD
		goto fail_res;

	ret = platform_device_add(pdev);
	if (ret)
		goto fail_res;
=======
		goto fail_res_conflict;

	ret = platform_device_add(pdev);
	if (ret)
		goto fail_res_conflict;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cell->pm_runtime_no_callbacks)
		pm_runtime_no_callbacks(&pdev->dev);

	kfree(res);

	return 0;

<<<<<<< HEAD
=======
fail_res_conflict:
	if (cell->swnode)
		device_remove_software_node(&pdev->dev);
fail_of_entry:
	list_for_each_entry_safe(of_entry, tmp, &mfd_of_node_list, list)
		if (of_entry->dev == &pdev->dev) {
			list_del(&of_entry->list);
			kfree(of_entry);
		}
fail_alias:
	regulator_bulk_unregister_supply_alias(&pdev->dev,
					       cell->parent_supplies,
					       cell->num_parent_supplies);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
fail_res:
	kfree(res);
fail_device:
	platform_device_put(pdev);
fail_alloc:
	return ret;
}

<<<<<<< HEAD
int mfd_add_devices(struct device *parent, int id,
		    struct mfd_cell *cells, int n_devs,
		    struct resource *mem_base,
		    int irq_base)
{
	int i;
	int ret = 0;
	atomic_t *cnts;

	/* initialize reference counting for all cells */
	cnts = kcalloc(n_devs, sizeof(*cnts), GFP_KERNEL);
	if (!cnts)
		return -ENOMEM;

	for (i = 0; i < n_devs; i++) {
		atomic_set(&cnts[i], 0);
		cells[i].usage_count = &cnts[i];
		ret = mfd_add_device(parent, id, cells + i, mem_base, irq_base);
		if (ret)
			break;
	}

	if (ret)
=======
/**
 * mfd_add_devices - register child devices
 *
 * @parent:	Pointer to parent device.
 * @id:		Can be PLATFORM_DEVID_AUTO to let the Platform API take care
 *		of device numbering, or will be added to a device's cell_id.
 * @cells:	Array of (struct mfd_cell)s describing child devices.
 * @n_devs:	Number of child devices to register.
 * @mem_base:	Parent register range resource for child devices.
 * @irq_base:	Base of the range of virtual interrupt numbers allocated for
 *		this MFD device. Unused if @domain is specified.
 * @domain:	Interrupt domain to create mappings for hardware interrupts.
 */
int mfd_add_devices(struct device *parent, int id,
		    const struct mfd_cell *cells, int n_devs,
		    struct resource *mem_base,
		    int irq_base, struct irq_domain *domain)
{
	int i;
	int ret;

	for (i = 0; i < n_devs; i++) {
		ret = mfd_add_device(parent, id, cells + i, mem_base,
				     irq_base, domain);
		if (ret)
			goto fail;
	}

	return 0;

fail:
	if (i)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mfd_remove_devices(parent);

	return ret;
}
EXPORT_SYMBOL(mfd_add_devices);

<<<<<<< HEAD
static int mfd_remove_devices_fn(struct device *dev, void *c)
{
	struct platform_device *pdev;
	const struct mfd_cell *cell;
	atomic_t **usage_count = c;
=======
static int mfd_remove_devices_fn(struct device *dev, void *data)
{
	struct platform_device *pdev;
	const struct mfd_cell *cell;
	struct mfd_of_node_entry *of_entry, *tmp;
	int *level = data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (dev->type != &mfd_dev_type)
		return 0;

	pdev = to_platform_device(dev);
	cell = mfd_get_cell(pdev);

<<<<<<< HEAD
	/* find the base address of usage_count pointers (for freeing) */
	if (!*usage_count || (cell->usage_count < *usage_count))
		*usage_count = cell->usage_count;
=======
	if (level && cell->level > *level)
		return 0;

	if (cell->swnode)
		device_remove_software_node(&pdev->dev);

	list_for_each_entry_safe(of_entry, tmp, &mfd_of_node_list, list)
		if (of_entry->dev == &pdev->dev) {
			list_del(&of_entry->list);
			kfree(of_entry);
		}

	regulator_bulk_unregister_supply_alias(dev, cell->parent_supplies,
					       cell->num_parent_supplies);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	platform_device_unregister(pdev);
	return 0;
}

<<<<<<< HEAD
void mfd_remove_devices(struct device *parent)
{
	atomic_t *cnts = NULL;

	device_for_each_child(parent, &cnts, mfd_remove_devices_fn);
	kfree(cnts);
}
EXPORT_SYMBOL(mfd_remove_devices);

int mfd_clone_cell(const char *cell, const char **clones, size_t n_clones)
{
	struct mfd_cell cell_entry;
	struct device *dev;
	struct platform_device *pdev;
	int i;

	/* fetch the parent cell's device (should already be registered!) */
	dev = bus_find_device_by_name(&platform_bus_type, NULL, cell);
	if (!dev) {
		printk(KERN_ERR "failed to find device for cell %s\n", cell);
		return -ENODEV;
	}
	pdev = to_platform_device(dev);
	memcpy(&cell_entry, mfd_get_cell(pdev), sizeof(cell_entry));

	WARN_ON(!cell_entry.enable);

	for (i = 0; i < n_clones; i++) {
		cell_entry.name = clones[i];
		/* don't give up if a single call fails; just report error */
		if (mfd_add_device(pdev->dev.parent, -1, &cell_entry, NULL, 0))
			dev_err(dev, "failed to create platform device '%s'\n",
					clones[i]);
	}

	return 0;
}
EXPORT_SYMBOL(mfd_clone_cell);
=======
void mfd_remove_devices_late(struct device *parent)
{
	int level = MFD_DEP_LEVEL_HIGH;

	device_for_each_child_reverse(parent, &level, mfd_remove_devices_fn);
}
EXPORT_SYMBOL(mfd_remove_devices_late);

void mfd_remove_devices(struct device *parent)
{
	int level = MFD_DEP_LEVEL_NORMAL;

	device_for_each_child_reverse(parent, &level, mfd_remove_devices_fn);
}
EXPORT_SYMBOL(mfd_remove_devices);

static void devm_mfd_dev_release(struct device *dev, void *res)
{
	mfd_remove_devices(dev);
}

/**
 * devm_mfd_add_devices - Resource managed version of mfd_add_devices()
 *
 * Returns 0 on success or an appropriate negative error number on failure.
 * All child-devices of the MFD will automatically be removed when it gets
 * unbinded.
 *
 * @dev:	Pointer to parent device.
 * @id:		Can be PLATFORM_DEVID_AUTO to let the Platform API take care
 *		of device numbering, or will be added to a device's cell_id.
 * @cells:	Array of (struct mfd_cell)s describing child devices.
 * @n_devs:	Number of child devices to register.
 * @mem_base:	Parent register range resource for child devices.
 * @irq_base:	Base of the range of virtual interrupt numbers allocated for
 *		this MFD device. Unused if @domain is specified.
 * @domain:	Interrupt domain to create mappings for hardware interrupts.
 */
int devm_mfd_add_devices(struct device *dev, int id,
			 const struct mfd_cell *cells, int n_devs,
			 struct resource *mem_base,
			 int irq_base, struct irq_domain *domain)
{
	struct device **ptr;
	int ret;

	ptr = devres_alloc(devm_mfd_dev_release, sizeof(*ptr), GFP_KERNEL);
	if (!ptr)
		return -ENOMEM;

	ret = mfd_add_devices(dev, id, cells, n_devs, mem_base,
			      irq_base, domain);
	if (ret < 0) {
		devres_free(ptr);
		return ret;
	}

	*ptr = dev;
	devres_add(dev, ptr);

	return ret;
}
EXPORT_SYMBOL(devm_mfd_add_devices);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ian Molton, Dmitry Baryshkov");
