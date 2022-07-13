<<<<<<< HEAD
obj-y = base.o
obj-$(CONFIG_OF_FLATTREE) += fdt.o
obj-$(CONFIG_OF_PROMTREE) += pdt.o
obj-$(CONFIG_OF_ADDRESS)  += address.o
obj-$(CONFIG_OF_IRQ)    += irq.o
obj-$(CONFIG_OF_DEVICE) += device.o platform.o
obj-$(CONFIG_OF_GPIO)   += gpio.o
obj-$(CONFIG_OF_I2C)	+= of_i2c.o
obj-$(CONFIG_OF_NET)	+= of_net.o
obj-$(CONFIG_OF_SPI)	+= of_spi.o
obj-$(CONFIG_OF_SELFTEST) += selftest.o
obj-$(CONFIG_OF_MDIO)	+= of_mdio.o
obj-$(CONFIG_OF_PCI)	+= of_pci.o
obj-$(CONFIG_OF_PCI_IRQ)  += of_pci_irq.o
obj-$(CONFIG_OF_SPMI)	+= of_spmi.o
obj-$(CONFIG_OF_MTD)	+= of_mtd.o
obj-$(CONFIG_OF_SLIMBUS)	+= of_slimbus.o
obj-$(CONFIG_OF_CORESIGHT) += of_coresight.o
obj-$(CONFIG_OF_BATTERYDATA)	+= of_batterydata.o
=======
# SPDX-License-Identifier: GPL-2.0
obj-y = base.o cpu.o device.o module.o platform.o property.o
obj-$(CONFIG_OF_KOBJ) += kobj.o
obj-$(CONFIG_OF_DYNAMIC) += dynamic.o
obj-$(CONFIG_OF_FLATTREE) += fdt.o empty_root.dtb.o
obj-$(CONFIG_OF_EARLY_FLATTREE) += fdt_address.o
obj-$(CONFIG_OF_PROMTREE) += pdt.o
obj-$(CONFIG_OF_ADDRESS)  += address.o
obj-$(CONFIG_OF_IRQ)    += irq.o
obj-$(CONFIG_OF_UNITTEST) += unittest.o
obj-$(CONFIG_OF_RESERVED_MEM) += of_reserved_mem.o
obj-$(CONFIG_OF_RESOLVE)  += resolver.o
obj-$(CONFIG_OF_OVERLAY) += overlay.o
obj-$(CONFIG_OF_NUMA) += of_numa.o

ifdef CONFIG_KEXEC_FILE
ifdef CONFIG_OF_FLATTREE
obj-y	+= kexec.o
endif
endif

obj-$(CONFIG_OF_KUNIT_TEST) += of_test.o

obj-$(CONFIG_OF_UNITTEST) += unittest-data/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
