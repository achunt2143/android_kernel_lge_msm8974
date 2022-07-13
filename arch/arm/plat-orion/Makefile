<<<<<<< HEAD
#
# Makefile for the linux kernel.
#

obj-y	:= irq.o pcie.o time.o common.o mpp.o addr-map.o
obj-m	:=
obj-n	:=
obj-	:=

obj-$(CONFIG_GENERIC_GPIO) += gpio.o
=======
# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile for the linux kernel.
#
ccflags-y := -I$(srctree)/$(src)/include

orion-gpio-$(CONFIG_GPIOLIB)      += gpio.o
obj-$(CONFIG_PLAT_ORION_LEGACY)   += irq.o pcie.o time.o common.o mpp.o
obj-$(CONFIG_PLAT_ORION_LEGACY)   += $(orion-gpio-y)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
