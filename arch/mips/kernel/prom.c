<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * MIPS support for CONFIG_OF device tree support
 *
 * Copyright (C) 2010 Cisco Systems Inc. <dediao@cisco.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/export.h>
#include <linux/errno.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
#include <linux/initrd.h>
#include <linux/debugfs.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>

#include <asm/page.h>
#include <asm/prom.h>

int __init early_init_dt_scan_memory_arch(unsigned long node,
					  const char *uname, int depth,
					  void *data)
{
	return early_init_dt_scan_memory(node, uname, depth, data);
}

void __init early_init_dt_add_memory_arch(u64 base, u64 size)
{
	return add_memory_region(base, size, BOOT_MEM_RAM);
}

int __init reserve_mem_mach(unsigned long addr, unsigned long size)
{
	return reserve_bootmem(addr, size, BOOTMEM_DEFAULT);
}

void __init free_mem_mach(unsigned long addr, unsigned long size)
{
	return free_bootmem(addr, size);
}

void * __init early_init_dt_alloc_memory_arch(u64 size, u64 align)
{
	return __alloc_bootmem(size, align, __pa(MAX_DMA_ADDRESS));
}

#ifdef CONFIG_BLK_DEV_INITRD
void __init early_init_dt_setup_initrd_arch(unsigned long start,
					    unsigned long end)
{
	initrd_start = (unsigned long)__va(start);
	initrd_end = (unsigned long)__va(end);
	initrd_below_start_ok = 1;
}
#endif

void __init early_init_devtree(void *params)
{
	/* Setup flat device-tree pointer */
	initial_boot_params = params;

	/* Retrieve various informations from the /chosen node of the
	 * device-tree, including the platform type, initrd location and
	 * size, and more ...
	 */
	of_scan_flat_dt(early_init_dt_scan_chosen, arcs_cmdline);


	/* Scan memory nodes */
	of_scan_flat_dt(early_init_dt_scan_root, NULL);
	of_scan_flat_dt(early_init_dt_scan_memory_arch, NULL);
}

void __init device_tree_init(void)
{
	unsigned long base, size;

	if (!initial_boot_params)
		return;

	base = virt_to_phys((void *)initial_boot_params);
	size = be32_to_cpu(initial_boot_params->totalsize);

	/* Before we do anything, lets reserve the dt blob */
	reserve_mem_mach(base, size);

	unflatten_device_tree();

	/* free the space reserved for the dt blob */
	free_mem_mach(base, size);
}
=======
#include <linux/memblock.h>
#include <linux/debugfs.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>

#include <asm/bootinfo.h>
#include <asm/page.h>
#include <asm/prom.h>

static char mips_machine_name[64] = "Unknown";

__init void mips_set_machine_name(const char *name)
{
	if (name == NULL)
		return;

	strscpy(mips_machine_name, name, sizeof(mips_machine_name));
	pr_info("MIPS: machine is %s\n", mips_get_machine_name());

	dump_stack_set_arch_desc(name);
}

char *mips_get_machine_name(void)
{
	return mips_machine_name;
}

#ifdef CONFIG_USE_OF

void __init __dt_setup_arch(void *bph)
{
	if (!early_init_dt_scan(bph))
		return;

	mips_set_machine_name(of_flat_dt_get_machine_name());
}

int __init __dt_register_buses(const char *bus0, const char *bus1)
{
	static struct of_device_id of_ids[3];

	if (!of_have_populated_dt())
		panic("device tree not present");

	strscpy(of_ids[0].compatible, bus0, sizeof(of_ids[0].compatible));
	if (bus1) {
		strscpy(of_ids[1].compatible, bus1,
			sizeof(of_ids[1].compatible));
	}

	if (of_platform_populate(NULL, of_ids, NULL, NULL))
		panic("failed to populate DT");

	return 0;
}

void __weak __init device_tree_init(void)
{
	unflatten_and_copy_device_tree();
}

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
