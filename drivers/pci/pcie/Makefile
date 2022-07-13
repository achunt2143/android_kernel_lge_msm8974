<<<<<<< HEAD
#
# Makefile for PCI-Express PORT Driver
#

# Build PCI Express ASPM if needed
obj-$(CONFIG_PCIEASPM)		+= aspm.o

pcieportdrv-y			:= portdrv_core.o portdrv_pci.o portdrv_bus.o
pcieportdrv-$(CONFIG_ACPI)	+= portdrv_acpi.o

obj-$(CONFIG_PCIEPORTBUS)	+= pcieportdrv.o

# Build PCI Express AER if needed
obj-$(CONFIG_PCIEAER)		+= aer/

obj-$(CONFIG_PCIE_PME) += pme.o
=======
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for PCI Express features and port driver

pcieportdrv-y			:= portdrv.o rcec.o

obj-$(CONFIG_PCIEPORTBUS)	+= pcieportdrv.o

obj-y				+= aspm.o
obj-$(CONFIG_PCIEAER)		+= aer.o err.o
obj-$(CONFIG_PCIEAER_INJECT)	+= aer_inject.o
obj-$(CONFIG_PCIE_PME)		+= pme.o
obj-$(CONFIG_PCIE_DPC)		+= dpc.o
obj-$(CONFIG_PCIE_PTM)		+= ptm.o
obj-$(CONFIG_PCIE_EDR)		+= edr.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
