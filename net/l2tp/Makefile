<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the L2TP.
#

obj-$(CONFIG_L2TP) += l2tp_core.o

<<<<<<< HEAD
=======
CFLAGS_l2tp_core.o += -I$(src)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# Build l2tp as modules if L2TP is M
obj-$(subst y,$(CONFIG_L2TP),$(CONFIG_PPPOL2TP)) += l2tp_ppp.o
obj-$(subst y,$(CONFIG_L2TP),$(CONFIG_L2TP_IP)) += l2tp_ip.o
obj-$(subst y,$(CONFIG_L2TP),$(CONFIG_L2TP_V3)) += l2tp_netlink.o
obj-$(subst y,$(CONFIG_L2TP),$(CONFIG_L2TP_ETH)) += l2tp_eth.o
obj-$(subst y,$(CONFIG_L2TP),$(CONFIG_L2TP_DEBUGFS)) += l2tp_debugfs.o
<<<<<<< HEAD
=======
ifneq ($(CONFIG_IPV6),)
obj-$(subst y,$(CONFIG_L2TP),$(CONFIG_L2TP_IP)) += l2tp_ip6.o
endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
