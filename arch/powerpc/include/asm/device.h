<<<<<<< HEAD
/*
 * Arch specific extensions to struct device
 *
 * This file is released under the GPLv2
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Arch specific extensions to struct device
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _ASM_POWERPC_DEVICE_H
#define _ASM_POWERPC_DEVICE_H

<<<<<<< HEAD
struct dma_map_ops;
struct device_node;
=======
struct device_node;
#ifdef CONFIG_PPC64
struct pci_dn;
struct iommu_table;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Arch extensions to struct device.
 *
 * When adding fields, consider macio_add_one_device in
 * drivers/macintosh/macio_asic.c
 */
struct dev_archdata {
<<<<<<< HEAD
	/* DMA operations on that device */
	struct dma_map_ops	*dma_ops;

	/*
	 * When an iommu is in use, dma_data is used as a ptr to the base of the
	 * iommu_table.  Otherwise, it is a simple numerical offset.
	 */
	union {
		dma_addr_t	dma_offset;
		void		*iommu_table_base;
	} dma_data;

#ifdef CONFIG_SWIOTLB
	dma_addr_t		max_direct_dma_addr;
=======
	/*
	 * These two used to be a union. However, with the hybrid ops we need
	 * both so here we store both a DMA offset for direct mappings and
	 * an iommu_table for remapped DMA.
	 */
	dma_addr_t		dma_offset;

#ifdef CONFIG_PPC64
	struct iommu_table	*iommu_table_base;
#endif

#ifdef CONFIG_PPC64
	struct pci_dn		*pci_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
#ifdef CONFIG_EEH
	struct eeh_dev		*edev;
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_FAIL_IOMMU
	int fail_iommu;
#endif
#ifdef CONFIG_CXL_BASE
	struct cxl_context	*cxl_ctx;
#endif
#ifdef CONFIG_PCI_IOV
	void *iov_data;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct pdev_archdata {
	u64 dma_mask;
<<<<<<< HEAD
};

#define ARCH_HAS_DMA_GET_REQUIRED_MASK

=======
	/*
	 * Pointer to nvdimm_pmu structure, to handle the unregistering
	 * of pmu device
	 */
	void *priv;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_POWERPC_DEVICE_H */
