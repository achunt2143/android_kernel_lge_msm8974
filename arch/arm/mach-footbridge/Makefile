<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the linux kernel.
#

# Object file lists.

<<<<<<< HEAD
obj-y			:= common.o dc21285.o dma.o isa-irq.o
obj-m			:=
obj-n			:=
obj-			:=

pci-$(CONFIG_ARCH_CATS) += cats-pci.o
pci-$(CONFIG_ARCH_EBSA285_HOST) += ebsa285-pci.o
pci-$(CONFIG_ARCH_NETWINDER) += netwinder-pci.o
pci-$(CONFIG_ARCH_PERSONAL_SERVER) += personal-pci.o

leds-$(CONFIG_ARCH_EBSA285) += ebsa285-leds.o
leds-$(CONFIG_ARCH_NETWINDER) += netwinder-leds.o

obj-$(CONFIG_ARCH_CATS) += cats-hw.o isa-timer.o
obj-$(CONFIG_ARCH_EBSA285) += ebsa285.o dc21285-timer.o
obj-$(CONFIG_ARCH_NETWINDER) += netwinder-hw.o isa-timer.o
obj-$(CONFIG_ARCH_PERSONAL_SERVER) += personal.o dc21285-timer.o

obj-$(CONFIG_PCI)	+=$(pci-y)
obj-$(CONFIG_LEDS)	+=$(leds-y)

obj-$(CONFIG_ISA)	+= isa.o isa-rtc.o
=======
obj-y			:= common.o isa-irq.o isa.o isa-rtc.o dma-isa.o

pci-y			+= dc21285.o
pci-$(CONFIG_ARCH_EBSA285) += ebsa285-pci.o
pci-$(CONFIG_ARCH_NETWINDER) += netwinder-pci.o

obj-$(CONFIG_ARCH_EBSA285) += ebsa285.o dc21285-timer.o
obj-$(CONFIG_ARCH_NETWINDER) += netwinder-hw.o isa-timer.o

obj-$(CONFIG_PCI)	+=$(pci-y)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
