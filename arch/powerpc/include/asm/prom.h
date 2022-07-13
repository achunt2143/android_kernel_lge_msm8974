<<<<<<< HEAD
#include <linux/of.h>	/* linux/of.h gets to determine #include ordering */
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _POWERPC_PROM_H
#define _POWERPC_PROM_H
#ifdef __KERNEL__

/*
 * Definitions for talking to the Open Firmware PROM on
 * Power Macintosh computers.
 *
 * Copyright (C) 1996-2005 Paul Mackerras.
 *
 * Updates for PPC64 by Peter Bergner & David Engebretsen, IBM Corp.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <linux/types.h>
#include <asm/irq.h>
#include <linux/atomic.h>

#define HAVE_ARCH_DEVTREE_FIXUPS
=======
 */
#include <linux/types.h>
#include <asm/firmware.h>

struct device_node;
struct property;

#define OF_DT_BEGIN_NODE	0x1		/* Start of node, full name */
#define OF_DT_END_NODE		0x2		/* End node */
#define OF_DT_PROP		0x3		/* Property: name off, size,
						 * content */
#define OF_DT_NOP		0x4		/* nop */
#define OF_DT_END		0x9

#define OF_DT_VERSION		0x10

/*
 * This is what gets passed to the kernel by prom_init or kexec
 *
 * The dt struct contains the device tree structure, full pathes and
 * property contents. The dt strings contain a separate block with just
 * the strings for the property names, and is fully page aligned and
 * self contained in a page, so that it can be kept around by the kernel,
 * each property name appears only once in this page (cheap compression)
 *
 * the mem_rsvmap contains a map of reserved ranges of physical memory,
 * passing it here instead of in the device-tree itself greatly simplifies
 * the job of everybody. It's just a list of u64 pairs (base/size) that
 * ends when size is 0
 */
struct boot_param_header {
	__be32	magic;			/* magic word OF_DT_HEADER */
	__be32	totalsize;		/* total size of DT block */
	__be32	off_dt_struct;		/* offset to structure */
	__be32	off_dt_strings;		/* offset to strings */
	__be32	off_mem_rsvmap;		/* offset to memory reserve map */
	__be32	version;		/* format version */
	__be32	last_comp_version;	/* last compatible version */
	/* version 2 fields below */
	__be32	boot_cpuid_phys;	/* Physical CPU id we're booting on */
	/* version 3 fields below */
	__be32	dt_strings_size;	/* size of the DT strings block */
	/* version 17 fields below */
	__be32	dt_struct_size;		/* size of the DT structure block */
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * OF address retreival & translation
 */

<<<<<<< HEAD
/* Translate a DMA address from device space to CPU space */
extern u64 of_translate_dma_address(struct device_node *dev,
				    const __be32 *in_addr);

#ifdef CONFIG_PCI
extern unsigned long pci_address_to_pio(phys_addr_t address);
#define pci_address_to_pio pci_address_to_pio
#endif	/* CONFIG_PCI */

/* Parse the ibm,dma-window property of an OF node into the busno, phys and
 * size parameters.
 */
void of_parse_dma_window(struct device_node *dn, const void *dma_window_prop,
		unsigned long *busno, unsigned long *phys, unsigned long *size);

extern void kdump_move_device_tree(void);

/* CPU OF node matching */
struct device_node *of_get_cpu_node(int cpu, unsigned int *thread);

/* cache lookup */
struct device_node *of_find_next_cache_node(struct device_node *np);

#ifdef CONFIG_NUMA
extern int of_node_to_nid(struct device_node *device);
#else
static inline int of_node_to_nid(struct device_node *device) { return 0; }
#endif
#define of_node_to_nid of_node_to_nid

extern void of_instantiate_rtc(void);

/* These includes are put at the bottom because they may contain things
 * that are overridden by this file.  Ideally they shouldn't be included
 * by this file, but there are a bunch of .c files that currently depend
 * on it.  Eventually they will be cleaned up. */
#include <linux/of_fdt.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
=======
/* Parse the ibm,dma-window property of an OF node into the busno, phys and
 * size parameters.
 */
void of_parse_dma_window(struct device_node *dn, const __be32 *dma_window,
			 unsigned long *busno, unsigned long *phys,
			 unsigned long *size);

extern void of_instantiate_rtc(void);

extern int of_get_ibm_chip_id(struct device_node *np);

struct of_drc_info {
	char *drc_type;
	char *drc_name_prefix;
	u32 drc_index_start;
	u32 drc_name_suffix_start;
	u32 num_sequential_elems;
	u32 sequential_inc;
	u32 drc_power_domain;
	u32 last_drc_index;
};

extern int of_read_drc_info_cell(struct property **prop,
			const __be32 **curval, struct of_drc_info *data);

extern unsigned int boot_cpu_node_count;

/*
 * There are two methods for telling firmware what our capabilities are.
 * Newer machines have an "ibm,client-architecture-support" method on the
 * root node.  For older machines, we have to call the "process-elf-header"
 * method in the /packages/elf-loader node, passing it a fake 32-bit
 * ELF header containing a couple of PT_NOTE sections that contain
 * structures that contain various information.
 */

/* New method - extensible architecture description vector. */

/* Option vector bits - generic bits in byte 1 */
#define OV_IGNORE		0x80	/* ignore this vector */
#define OV_CESSATION_POLICY	0x40	/* halt if unsupported option present*/

/* Option vector 1: processor architectures supported */
#define OV1_PPC_2_00		0x80	/* set if we support PowerPC 2.00 */
#define OV1_PPC_2_01		0x40	/* set if we support PowerPC 2.01 */
#define OV1_PPC_2_02		0x20	/* set if we support PowerPC 2.02 */
#define OV1_PPC_2_03		0x10	/* set if we support PowerPC 2.03 */
#define OV1_PPC_2_04		0x08	/* set if we support PowerPC 2.04 */
#define OV1_PPC_2_05		0x04	/* set if we support PowerPC 2.05 */
#define OV1_PPC_2_06		0x02	/* set if we support PowerPC 2.06 */
#define OV1_PPC_2_07		0x01	/* set if we support PowerPC 2.07 */

#define OV1_PPC_3_00		0x80	/* set if we support PowerPC 3.00 */
#define OV1_PPC_3_1			0x40	/* set if we support PowerPC 3.1 */

/* Option vector 2: Open Firmware options supported */
#define OV2_REAL_MODE		0x20	/* set if we want OF in real mode */

/* Option vector 3: processor options supported */
#define OV3_FP			0x80	/* floating point */
#define OV3_VMX			0x40	/* VMX/Altivec */
#define OV3_DFP			0x20	/* decimal FP */

/* Option vector 4: IBM PAPR implementation */
#define OV4_MIN_ENT_CAP		0x01	/* minimum VP entitled capacity */

/* Option vector 5: PAPR/OF options supported
 * These bits are also used in firmware_has_feature() to validate
 * the capabilities reported for vector 5 in the device tree so we
 * encode the vector index in the define and use the OV5_FEAT()
 * and OV5_INDX() macros to extract the desired information.
 */
#define OV5_FEAT(x)	((x) & 0xff)
#define OV5_INDX(x)	((x) >> 8)
#define OV5_LPAR		0x0280	/* logical partitioning supported */
#define OV5_SPLPAR		0x0240	/* shared-processor LPAR supported */
/* ibm,dynamic-reconfiguration-memory property supported */
#define OV5_DRCONF_MEMORY	0x0220
#define OV5_LARGE_PAGES		0x0210	/* large pages supported */
#define OV5_DONATE_DEDICATE_CPU	0x0202	/* donate dedicated CPU support */
#define OV5_MSI			0x0201	/* PCIe/MSI support */
#define OV5_CMO			0x0480	/* Cooperative Memory Overcommitment */
#define OV5_XCMO		0x0440	/* Page Coalescing */
#define OV5_FORM1_AFFINITY	0x0580	/* FORM1 NUMA affinity */
#define OV5_PRRN		0x0540	/* Platform Resource Reassignment */
#define OV5_FORM2_AFFINITY	0x0520	/* Form2 NUMA affinity */
#define OV5_HP_EVT		0x0604	/* Hot Plug Event support */
#define OV5_RESIZE_HPT		0x0601	/* Hash Page Table resizing */
#define OV5_PFO_HW_RNG		0x1180	/* PFO Random Number Generator */
#define OV5_PFO_HW_842		0x1140	/* PFO Compression Accelerator */
#define OV5_PFO_HW_ENCR		0x1120	/* PFO Encryption Accelerator */
#define OV5_SUB_PROCESSORS	0x1501	/* 1,2,or 4 Sub-Processors supported */
#define OV5_DRMEM_V2		0x1680	/* ibm,dynamic-reconfiguration-v2 */
#define OV5_XIVE_SUPPORT	0x17C0	/* XIVE Exploitation Support Mask */
#define OV5_XIVE_LEGACY		0x1700	/* XIVE legacy mode Only */
#define OV5_XIVE_EXPLOIT	0x1740	/* XIVE exploitation mode Only */
#define OV5_XIVE_EITHER		0x1780	/* XIVE legacy or exploitation mode */
/* MMU Base Architecture */
#define OV5_MMU_SUPPORT		0x18C0	/* MMU Mode Support Mask */
#define OV5_MMU_HASH		0x1800	/* Hash MMU Only */
#define OV5_MMU_RADIX		0x1840	/* Radix MMU Only */
#define OV5_MMU_EITHER		0x1880	/* Hash or Radix Supported */
#define OV5_MMU_DYNAMIC		0x18C0	/* Hash or Radix Can Switch Later */
#define OV5_NMMU		0x1820	/* Nest MMU Available */
/* Hash Table Extensions */
#define OV5_HASH_SEG_TBL	0x1980	/* In Memory Segment Tables Available */
#define OV5_HASH_GTSE		0x1940	/* Guest Translation Shoot Down Avail */
/* Radix Table Extensions */
#define OV5_RADIX_GTSE		0x1A40	/* Guest Translation Shoot Down Avail */
#define OV5_DRC_INFO		0x1640	/* Redef Prop Structures: drc-info   */

/* Option Vector 6: IBM PAPR hints */
#define OV6_LINUX		0x02	/* Linux is our OS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __KERNEL__ */
#endif /* _POWERPC_PROM_H */
