<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 * Copyright (C) 2010 John Crispin <blogic@openwrt.org>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *
 * Copyright (C) 2010 John Crispin <john@phrozen.org>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/export.h>
#include <linux/clk.h>
<<<<<<< HEAD
#include <asm/bootinfo.h>
#include <asm/time.h>
=======
#include <linux/memblock.h>
#include <linux/of_fdt.h>

#include <asm/bootinfo.h>
#include <asm/time.h>
#include <asm/prom.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <lantiq.h>

#include "prom.h"
#include "clk.h"

<<<<<<< HEAD
static struct ltq_soc_info soc_info;

unsigned int ltq_get_cpu_ver(void)
{
	return soc_info.rev;
}
EXPORT_SYMBOL(ltq_get_cpu_ver);

unsigned int ltq_get_soc_type(void)
{
	return soc_info.type;
}
EXPORT_SYMBOL(ltq_get_soc_type);
=======
/* access to the ebu needs to be locked between different drivers */
DEFINE_SPINLOCK(ebu_lock);
EXPORT_SYMBOL_GPL(ebu_lock);

/*
 * this struct is filled by the soc specific detection code and holds
 * information about the specific soc type, revision and name
 */
static struct ltq_soc_info soc_info;

/*
 * These structs are used to override vsmp_init_secondary()
 */
#if defined(CONFIG_MIPS_MT_SMP)
extern const struct plat_smp_ops vsmp_smp_ops;
static struct plat_smp_ops lantiq_smp_ops;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

const char *get_system_type(void)
{
	return soc_info.sys_type;
}

<<<<<<< HEAD
void prom_free_prom_memory(void)
{
=======
int ltq_soc_type(void)
{
	return soc_info.type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __init prom_init_cmdline(void)
{
	int argc = fw_arg0;
	char **argv = (char **) KSEG1ADDR(fw_arg1);
	int i;

<<<<<<< HEAD
	for (i = 0; i < argc; i++) {
		char *p = (char *)  KSEG1ADDR(argv[i]);

		if (p && *p) {
=======
	arcs_cmdline[0] = '\0';

	for (i = 0; i < argc; i++) {
		char *p = (char *) KSEG1ADDR(argv[i]);

		if (CPHYSADDR(p) && *p) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			strlcat(arcs_cmdline, p, sizeof(arcs_cmdline));
			strlcat(arcs_cmdline, " ", sizeof(arcs_cmdline));
		}
	}
}

<<<<<<< HEAD
void __init prom_init(void)
{
	struct clk *clk;

	ltq_soc_detect(&soc_info);
	clk_init();
	clk = clk_get(0, "cpu");
	snprintf(soc_info.sys_type, LTQ_SYS_TYPE_LEN - 1, "%s rev1.%d",
		soc_info.name, soc_info.rev);
	clk_put(clk);
	soc_info.sys_type[LTQ_SYS_TYPE_LEN - 1] = '\0';
	pr_info("SoC: %s\n", soc_info.sys_type);
	prom_init_cmdline();
=======
void __init plat_mem_setup(void)
{
	void *dtb;

	ioport_resource.start = IOPORT_RESOURCE_START;
	ioport_resource.end = IOPORT_RESOURCE_END;
	iomem_resource.start = IOMEM_RESOURCE_START;
	iomem_resource.end = IOMEM_RESOURCE_END;

	set_io_port_base((unsigned long) KSEG1);

	dtb = get_fdt();
	if (dtb == NULL)
		panic("no dtb found");

	/*
	 * Load the devicetree. This causes the chosen node to be
	 * parsed resulting in our memory appearing
	 */
	__dt_setup_arch(dtb);
}

#if defined(CONFIG_MIPS_MT_SMP)
static void lantiq_init_secondary(void)
{
	/*
	 * MIPS CPU startup function vsmp_init_secondary() will only
	 * enable some of the interrupts for the second CPU/VPE.
	 */
	set_c0_status(ST0_IM);
}
#endif

void __init prom_init(void)
{
	/* call the soc specific detetcion code and get it to fill soc_info */
	ltq_soc_detect(&soc_info);
	snprintf(soc_info.sys_type, LTQ_SYS_TYPE_LEN - 1, "%s rev %s",
		soc_info.name, soc_info.rev_type);
	soc_info.sys_type[LTQ_SYS_TYPE_LEN - 1] = '\0';
	pr_info("SoC: %s\n", soc_info.sys_type);
	prom_init_cmdline();

#if defined(CONFIG_MIPS_MT_SMP)
	lantiq_smp_ops = vsmp_smp_ops;
	if (cpu_has_mipsmt)
		lantiq_smp_ops.init_secondary = lantiq_init_secondary;
	register_smp_ops(&lantiq_smp_ops);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
