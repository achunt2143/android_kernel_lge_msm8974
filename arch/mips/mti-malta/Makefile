<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Carsten Langgaard, carstenl@mips.com
# Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
#
# Copyright (C) 2008 Wind River Systems, Inc.
#   written by Ralf Baechle <ralf@linux-mips.org>
#
<<<<<<< HEAD
obj-y				:= malta-amon.o malta-cmdline.o \
				   malta-display.o malta-init.o malta-int.o \
				   malta-memory.o malta-platform.o \
				   malta-reset.o malta-setup.o malta-time.o

obj-$(CONFIG_EARLY_PRINTK)	+= malta-console.o
obj-$(CONFIG_PCI)		+= malta-pci.o

# FIXME FIXME FIXME
obj-$(CONFIG_MIPS_MT_SMTC)	+= malta-smtc.o
=======
obj-y				+= malta-dtshim.o
obj-y				+= malta-init.o
obj-y				+= malta-int.o
obj-y				+= malta-memory.o
obj-y				+= malta-platform.o
obj-y				+= malta-setup.o
obj-y				+= malta-time.o

CFLAGS_malta-dtshim.o = -I$(src)/../../../scripts/dtc/libfdt
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
