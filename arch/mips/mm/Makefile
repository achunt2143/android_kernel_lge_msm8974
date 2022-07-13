<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux/MIPS-specific parts of the memory manager.
#

<<<<<<< HEAD
obj-y				+= cache.o dma-default.o extable.o fault.o \
				   gup.o init.o mmap.o page.o tlbex.o \
				   tlbex-fault.o uasm.o

obj-$(CONFIG_32BIT)		+= ioremap.o pgtable-32.o
obj-$(CONFIG_64BIT)		+= pgtable-64.o
obj-$(CONFIG_HIGHMEM)		+= highmem.o
obj-$(CONFIG_HUGETLB_PAGE)	+= hugetlbpage.o

obj-$(CONFIG_CPU_LOONGSON2)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_MIPS32)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_MIPS64)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_NEVADA)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R10000)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R3000)		+= c-r3k.o tlb-r3k.o
obj-$(CONFIG_CPU_R4300)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R4X00)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R5000)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R5432)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R5500)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R8000)		+= c-r4k.o cex-gen.o tlb-r8k.o
obj-$(CONFIG_CPU_RM7000)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_RM9000)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_SB1)		+= c-r4k.o cerr-sb1.o cex-sb1.o tlb-r4k.o
obj-$(CONFIG_CPU_TX39XX)	+= c-tx39.o tlb-r3k.o
obj-$(CONFIG_CPU_TX49XX)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_VR41XX)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_CAVIUM_OCTEON)	+= c-octeon.o cex-oct.o tlb-r4k.o
obj-$(CONFIG_CPU_XLR)		+= c-r4k.o tlb-r4k.o cex-gen.o
obj-$(CONFIG_CPU_XLP)		+= c-r4k.o tlb-r4k.o cex-gen.o

obj-$(CONFIG_IP22_CPU_SCACHE)	+= sc-ip22.o
obj-$(CONFIG_R5000_CPU_SCACHE)  += sc-r5k.o
obj-$(CONFIG_RM7000_CPU_SCACHE)	+= sc-rm7k.o
obj-$(CONFIG_MIPS_CPU_SCACHE)	+= sc-mips.o
=======
obj-y				+= cache.o
obj-y				+= context.o
obj-y				+= extable.o
obj-y				+= fault.o
obj-y				+= init.o
obj-y				+= mmap.o
obj-y				+= page.o
obj-y				+= page-funcs.o
obj-y				+= pgtable.o
obj-y				+= tlbex.o
obj-y				+= tlbex-fault.o
obj-y				+= tlb-funcs.o

ifdef CONFIG_CPU_MICROMIPS
obj-y				+= uasm-micromips.o
else
obj-y				+= uasm-mips.o
endif

ifndef CONFIG_EVA
obj-y				+= maccess.o
endif

obj-$(CONFIG_32BIT)		+= ioremap.o pgtable-32.o
obj-$(CONFIG_64BIT)		+= ioremap64.o pgtable-64.o
obj-$(CONFIG_HIGHMEM)		+= highmem.o
obj-$(CONFIG_HUGETLB_PAGE)	+= hugetlbpage.o
obj-$(CONFIG_DMA_NONCOHERENT)	+= dma-noncoherent.o

obj-$(CONFIG_CPU_R3K_TLB)	+= tlb-r3k.o
obj-$(CONFIG_CPU_R4K_CACHE_TLB) += c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R3000)		+= c-r3k.o
obj-$(CONFIG_CPU_SB1)		+= c-r4k.o cerr-sb1.o cex-sb1.o tlb-r4k.o
obj-$(CONFIG_CPU_CAVIUM_OCTEON) += c-octeon.o cex-oct.o tlb-r4k.o

obj-$(CONFIG_IP22_CPU_SCACHE)	+= sc-ip22.o
obj-$(CONFIG_R5000_CPU_SCACHE)	+= sc-r5k.o
obj-$(CONFIG_RM7000_CPU_SCACHE) += sc-rm7k.o
obj-$(CONFIG_MIPS_CPU_SCACHE)	+= sc-mips.o
obj-$(CONFIG_SCACHE_DEBUGFS)	+= sc-debugfs.o

obj-$(CONFIG_DEBUG_VIRTUAL)	+= physaddr.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
