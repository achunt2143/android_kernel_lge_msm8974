<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the linux m68k-specific parts of the memory manager.
#

obj-y	:= init.o

obj-$(CONFIG_MMU)		+= cache.o fault.o
obj-$(CONFIG_MMU_MOTOROLA)	+= kmap.o memory.o motorola.o hwtest.o
obj-$(CONFIG_MMU_SUN3)		+= sun3kmap.o sun3mmu.o hwtest.o
obj-$(CONFIG_MMU_COLDFIRE)	+= kmap.o memory.o mcfmmu.o

