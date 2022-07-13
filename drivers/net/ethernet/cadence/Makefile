<<<<<<< HEAD
#
# Makefile for the Atmel network device drivers.
#

obj-$(CONFIG_ARM_AT91_ETHER) += at91_ether.o
obj-$(CONFIG_MACB) += macb.o
=======
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the Atmel network device drivers.
#
macb-y	:= macb_main.o

ifeq ($(CONFIG_MACB_USE_HWSTAMP),y)
macb-y	+= macb_ptp.o
endif

obj-$(CONFIG_MACB) += macb.o
obj-$(CONFIG_MACB_PCI) += macb_pci.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
