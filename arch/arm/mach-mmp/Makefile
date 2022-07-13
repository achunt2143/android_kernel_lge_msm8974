<<<<<<< HEAD
#
# Makefile for Marvell's PXA168 processors line
#

obj-y				+= common.o clock.o devices.o time.o

# SoC support
obj-$(CONFIG_CPU_PXA168)	+= pxa168.o irq-pxa168.o
obj-$(CONFIG_CPU_PXA910)	+= pxa910.o irq-pxa168.o
obj-$(CONFIG_CPU_MMP2)		+= mmp2.o irq-mmp2.o sram.o

# board support
obj-$(CONFIG_MACH_ASPENITE)	+= aspenite.o
obj-$(CONFIG_MACH_ZYLONITE2)	+= aspenite.o
obj-$(CONFIG_MACH_AVENGERS_LITE)+= avengers_lite.o
obj-$(CONFIG_MACH_TAVOREVB)	+= tavorevb.o
obj-$(CONFIG_MACH_TTC_DKB)	+= ttc_dkb.o
obj-$(CONFIG_MACH_BROWNSTONE)	+= brownstone.o
obj-$(CONFIG_MACH_FLINT)	+= flint.o
obj-$(CONFIG_MACH_MARVELL_JASPER) += jasper.o
obj-$(CONFIG_MACH_MMP_DT)	+= mmp-dt.o
obj-$(CONFIG_MACH_TETON_BGA)	+= teton_bga.o
obj-$(CONFIG_MACH_GPLUGD)	+= gplugd.o
=======
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for Marvell's PXA168 processors line
#
obj-y				+= common.o time.o

ifeq ($(CONFIG_SMP),y)
obj-$(CONFIG_MACH_MMP3_DT)	+= platsmp.o
endif

# board support
obj-$(CONFIG_MACH_MMP_DT)	+= mmp-dt.o
obj-$(CONFIG_MACH_MMP2_DT)	+= mmp2-dt.o
obj-$(CONFIG_MACH_MMP3_DT)	+= mmp3.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
