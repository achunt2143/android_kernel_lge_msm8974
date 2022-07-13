<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# Makefile for the linux Sparc-specific parts of the memory manager.
#

asflags-y := -ansi
<<<<<<< HEAD
ccflags-y := -Werror

obj-$(CONFIG_SPARC64)   += ultra.o tlb.o tsb.o gup.o
obj-y                   += fault_$(BITS).o
obj-y                   += init_$(BITS).o
obj-$(CONFIG_SPARC32)   += loadmmu.o
obj-$(CONFIG_SPARC32)   += extable.o btfixup.o srmmu.o iommu.o io-unit.o
obj-$(CONFIG_SPARC32)   += hypersparc.o viking.o tsunami.o swift.o
obj-$(CONFIG_SPARC_LEON)+= leon_mm.o

# Only used by sparc64
obj-$(CONFIG_HUGETLB_PAGE) += hugetlbpage.o

# Only used by sparc32
obj-$(CONFIG_HIGHMEM)   += highmem.o

ifdef CONFIG_SMP
obj-$(CONFIG_SPARC32) += nosun4c.o
else
obj-$(CONFIG_SPARC32) += sun4c.o
endif
=======

obj-$(CONFIG_SPARC64)   += ultra.o tlb.o tsb.o
obj-y                   += fault_$(BITS).o
obj-y                   += init_$(BITS).o
obj-$(CONFIG_SPARC32)   += srmmu.o iommu.o io-unit.o
obj-$(CONFIG_SPARC32)   += srmmu_access.o
obj-$(CONFIG_SPARC32)   += hypersparc.o viking.o tsunami.o swift.o
obj-$(CONFIG_SPARC32)   += leon_mm.o

# Only used by sparc64
obj-$(CONFIG_HUGETLB_PAGE) += hugetlbpage.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
