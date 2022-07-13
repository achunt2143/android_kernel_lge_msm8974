<<<<<<< HEAD
# Makefile for kernel virtual machines on s390
#
# Copyright IBM Corp. 2008
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License (version 2 only)
# as published by the Free Software Foundation.

common-objs = $(addprefix ../../../virt/kvm/, kvm_main.o)

ccflags-y := -Ivirt/kvm -Iarch/s390/kvm

kvm-objs := $(common-objs) kvm-s390.o intercept.o interrupt.o priv.o sigp.o diag.o
=======
# SPDX-License-Identifier: GPL-2.0
# Makefile for kernel virtual machines on s390
#
# Copyright IBM Corp. 2008

include $(srctree)/virt/kvm/Makefile.kvm

ccflags-y := -Ivirt/kvm -Iarch/s390/kvm

kvm-y += kvm-s390.o intercept.o interrupt.o priv.o sigp.o
kvm-y += diag.o gaccess.o guestdbg.o vsie.o pv.o

kvm-$(CONFIG_VFIO_PCI_ZDEV_KVM) += pci.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_KVM) += kvm.o
