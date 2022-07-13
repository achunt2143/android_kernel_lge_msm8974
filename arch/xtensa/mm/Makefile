<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux/Xtensa-specific parts of the memory manager.
#

<<<<<<< HEAD
obj-y			:= init.o cache.o misc.o
obj-$(CONFIG_MMU)	+= fault.o mmu.o tlb.o
=======
obj-y			:= init.o misc.o
obj-$(CONFIG_PFAULT)	+= fault.o
obj-$(CONFIG_MMU)	+= cache.o ioremap.o mmu.o tlb.o
obj-$(CONFIG_HIGHMEM)	+= highmem.o
obj-$(CONFIG_KASAN)	+= kasan_init.o

KASAN_SANITIZE_fault.o := n
KASAN_SANITIZE_kasan_init.o := n
KASAN_SANITIZE_mmu.o := n
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
