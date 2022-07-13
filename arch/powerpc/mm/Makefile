<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the linux ppc-specific parts of the memory manager.
#

<<<<<<< HEAD
subdir-ccflags-$(CONFIG_PPC_WERROR) := -Werror

ccflags-$(CONFIG_PPC64)	:= -mno-minimal-toc

obj-y				:= fault.o mem.o pgtable.o gup.o \
				   init_$(CONFIG_WORD_SIZE).o \
				   pgtable_$(CONFIG_WORD_SIZE).o
obj-$(CONFIG_PPC_MMU_NOHASH)	+= mmu_context_nohash.o tlb_nohash.o \
				   tlb_nohash_low.o
obj-$(CONFIG_PPC_BOOK3E)	+= tlb_low_$(CONFIG_WORD_SIZE)e.o
obj-$(CONFIG_PPC64)		+= mmap_64.o
hash64-$(CONFIG_PPC_NATIVE)	:= hash_native_64.o
obj-$(CONFIG_PPC_STD_MMU_64)	+= hash_utils_64.o \
				   slb_low.o slb.o stab.o \
				   mmap_64.o $(hash64-y)
obj-$(CONFIG_PPC_STD_MMU_32)	+= ppc_mmu_32.o
obj-$(CONFIG_PPC_STD_MMU)	+= hash_low_$(CONFIG_WORD_SIZE).o \
				   tlb_hash$(CONFIG_WORD_SIZE).o \
				   mmu_context_hash$(CONFIG_WORD_SIZE).o
obj-$(CONFIG_PPC_ICSWX)		+= icswx.o
obj-$(CONFIG_PPC_ICSWX_PID)	+= icswx_pid.o
obj-$(CONFIG_40x)		+= 40x_mmu.o
obj-$(CONFIG_44x)		+= 44x_mmu.o
obj-$(CONFIG_PPC_FSL_BOOK3E)	+= fsl_booke_mmu.o
obj-$(CONFIG_NEED_MULTIPLE_NODES) += numa.o
obj-$(CONFIG_PPC_MM_SLICES)	+= slice.o
ifeq ($(CONFIG_HUGETLB_PAGE),y)
obj-y				+= hugetlbpage.o
obj-$(CONFIG_PPC_STD_MMU_64)	+= hugetlbpage-hash64.o
obj-$(CONFIG_PPC_BOOK3E_MMU)	+= hugetlbpage-book3e.o
endif
obj-$(CONFIG_PPC_SUBPAGE_PROT)	+= subpage-prot.o
obj-$(CONFIG_NOT_COHERENT_CACHE) += dma-noncoherent.o
obj-$(CONFIG_HIGHMEM)		+= highmem.o
=======
ccflags-$(CONFIG_PPC64)	:= $(NO_MINIMAL_TOC)

obj-y				:= fault.o mem.o pgtable.o maccess.o pageattr.o \
				   init_$(BITS).o pgtable_$(BITS).o \
				   pgtable-frag.o ioremap.o ioremap_$(BITS).o \
				   init-common.o mmu_context.o drmem.o \
				   cacheflush.o
obj-$(CONFIG_PPC_MMU_NOHASH)	+= nohash/
obj-$(CONFIG_PPC_BOOK3S_32)	+= book3s32/
obj-$(CONFIG_PPC_BOOK3S_64)	+= book3s64/
obj-$(CONFIG_NUMA) += numa.o
obj-$(CONFIG_HUGETLB_PAGE)	+= hugetlbpage.o
obj-$(CONFIG_NOT_COHERENT_CACHE) += dma-noncoherent.o
obj-$(CONFIG_PPC_COPRO_BASE)	+= copro_fault.o
obj-$(CONFIG_PTDUMP_CORE)	+= ptdump/
obj-$(CONFIG_KASAN)		+= kasan/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
