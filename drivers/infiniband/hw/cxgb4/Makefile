<<<<<<< HEAD
ccflags-y := -Idrivers/net/ethernet/chelsio/cxgb4

obj-$(CONFIG_INFINIBAND_CXGB4) += iw_cxgb4.o

iw_cxgb4-y :=  device.o cm.o provider.o mem.o cq.o qp.o resource.o ev.o
=======
# SPDX-License-Identifier: GPL-2.0-only
ccflags-y := -I $(srctree)/drivers/net/ethernet/chelsio/cxgb4
ccflags-y += -I $(srctree)/drivers/net/ethernet/chelsio/libcxgb

obj-$(CONFIG_INFINIBAND_CXGB4) += iw_cxgb4.o

iw_cxgb4-y :=  device.o cm.o provider.o mem.o cq.o qp.o resource.o ev.o id_table.o \
	       restrack.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
