<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux VLAN layer.
#
obj-$(subst m,y,$(CONFIG_VLAN_8021Q))	+= vlan_core.o
obj-$(CONFIG_VLAN_8021Q)		+= 8021q.o

8021q-y					:= vlan.o vlan_dev.o vlan_netlink.o
8021q-$(CONFIG_VLAN_8021Q_GVRP)		+= vlan_gvrp.o
<<<<<<< HEAD
8021q-$(CONFIG_PROC_FS)			+= vlanproc.o

=======
8021q-$(CONFIG_VLAN_8021Q_MVRP)		+= vlan_mvrp.o
8021q-$(CONFIG_PROC_FS)			+= vlanproc.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
