<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Common header for intel-gtt.ko and i915.ko */

#ifndef _DRM_INTEL_GTT_H
#define	_DRM_INTEL_GTT_H

<<<<<<< HEAD
const struct intel_gtt {
	/* Size of memory reserved for graphics by the BIOS */
	unsigned int stolen_size;
	/* Total number of gtt entries. */
	unsigned int gtt_total_entries;
	/* Part of the gtt that is mappable by the cpu, for those chips where
	 * this is not the full gtt. */
	unsigned int gtt_mappable_entries;
	/* Whether i915 needs to use the dmar apis or not. */
	unsigned int needs_dmar : 1;
	/* Whether we idle the gpu before mapping/unmapping */
	unsigned int do_idle_maps : 1;
	/* Share the scratch page dma with ppgtts. */
	dma_addr_t scratch_page_dma;
	/* for ppgtt PDE access */
	u32 __iomem *gtt;
} *intel_gtt_get(void);

void intel_gtt_chipset_flush(void);
void intel_gtt_unmap_memory(struct scatterlist *sg_list, int num_sg);
void intel_gtt_clear_range(unsigned int first_entry, unsigned int num_entries);
int intel_gtt_map_memory(struct page **pages, unsigned int num_entries,
			 struct scatterlist **sg_list, int *num_sg);
void intel_gtt_insert_sg_entries(struct scatterlist *sg_list,
				 unsigned int sg_len,
				 unsigned int pg_start,
				 unsigned int flags);
void intel_gtt_insert_pages(unsigned int first_entry, unsigned int num_entries,
			    struct page **pages, unsigned int flags);
=======
#include <linux/types.h>

struct agp_bridge_data;
struct pci_dev;
struct sg_table;

void intel_gmch_gtt_get(u64 *gtt_total,
			phys_addr_t *mappable_base,
			resource_size_t *mappable_end);

int intel_gmch_probe(struct pci_dev *bridge_pdev, struct pci_dev *gpu_pdev,
		     struct agp_bridge_data *bridge);
void intel_gmch_remove(void);

bool intel_gmch_enable_gtt(void);

void intel_gmch_gtt_flush(void);
void intel_gmch_gtt_insert_page(dma_addr_t addr,
				unsigned int pg,
				unsigned int flags);
void intel_gmch_gtt_insert_sg_entries(struct sg_table *st,
				      unsigned int pg_start,
				      unsigned int flags);
void intel_gmch_gtt_clear_range(unsigned int first_entry, unsigned int num_entries);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Special gtt memory types */
#define AGP_DCACHE_MEMORY	1
#define AGP_PHYS_MEMORY		2

<<<<<<< HEAD
/* New caching attributes for gen6/sandybridge */
#define AGP_USER_CACHED_MEMORY_LLC_MLC (AGP_USER_TYPES + 2)
#define AGP_USER_UNCACHED_MEMORY (AGP_USER_TYPES + 4)

/* flag for GFDT type */
#define AGP_USER_CACHED_MEMORY_GFDT (1 << 3)

#ifdef CONFIG_INTEL_IOMMU
extern int intel_iommu_gfx_mapped;
#endif

=======
/* flag for GFDT type */
#define AGP_USER_CACHED_MEMORY_GFDT (1 << 3)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
