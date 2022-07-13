<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef ASMARM_DMA_IOMMU_H
#define ASMARM_DMA_IOMMU_H

#ifdef __KERNEL__

#include <linux/mm_types.h>
#include <linux/scatterlist.h>
<<<<<<< HEAD
#include <linux/dma-debug.h>
#include <linux/kmemcheck.h>
=======
#include <linux/kref.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct dma_iommu_mapping {
	/* iommu specific data */
	struct iommu_domain	*domain;

<<<<<<< HEAD
	void			*bitmap;
	size_t			bits;
	unsigned int		order;
=======
	unsigned long		**bitmaps;	/* array of bitmaps */
	unsigned int		nr_bitmaps;	/* nr of elements in array */
	unsigned int		extensions;
	size_t			bitmap_size;	/* size of a single bitmap */
	size_t			bits;		/* per bitmap */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dma_addr_t		base;

	spinlock_t		lock;
	struct kref		kref;
};

struct dma_iommu_mapping *
<<<<<<< HEAD
arm_iommu_create_mapping(struct bus_type *bus, dma_addr_t base, size_t size,
			 int order);
=======
arm_iommu_create_mapping(const struct bus_type *bus, dma_addr_t base, u64 size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void arm_iommu_release_mapping(struct dma_iommu_mapping *mapping);

int arm_iommu_attach_device(struct device *dev,
					struct dma_iommu_mapping *mapping);
<<<<<<< HEAD
=======
void arm_iommu_detach_device(struct device *dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __KERNEL__ */
#endif
