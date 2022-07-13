<<<<<<< HEAD
#ifndef _LINUX_DMA_DIRECTION_H
#define _LINUX_DMA_DIRECTION_H
/*
 * These definitions mirror those in pci.h, so they can be used
 * interchangeably with their PCI_ counterparts.
 */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DMA_DIRECTION_H
#define _LINUX_DMA_DIRECTION_H

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
enum dma_data_direction {
	DMA_BIDIRECTIONAL = 0,
	DMA_TO_DEVICE = 1,
	DMA_FROM_DEVICE = 2,
	DMA_NONE = 3,
};
<<<<<<< HEAD
#endif
=======

static inline int valid_dma_direction(enum dma_data_direction dir)
{
	return dir == DMA_BIDIRECTIONAL || dir == DMA_TO_DEVICE ||
		dir == DMA_FROM_DEVICE;
}

#endif /* _LINUX_DMA_DIRECTION_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
