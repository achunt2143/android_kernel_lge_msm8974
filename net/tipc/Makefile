<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux TIPC layer
#

obj-$(CONFIG_TIPC) := tipc.o

<<<<<<< HEAD
tipc-y	+= addr.o bcast.o bearer.o config.o \
	   core.o handler.o link.o discover.o msg.o  \
	   name_distr.o  subscr.o name_table.o net.o  \
	   netlink.o node.o node_subscr.o port.o ref.o  \
	   socket.o log.o eth_media.o

# End of file
=======
tipc-y	+= addr.o bcast.o bearer.o \
	   core.o link.o discover.o msg.o  \
	   name_distr.o  subscr.o monitor.o name_table.o net.o  \
	   netlink.o netlink_compat.o node.o socket.o eth_media.o \
	   topsrv.o group.o trace.o

CFLAGS_trace.o += -I$(src)

tipc-$(CONFIG_TIPC_MEDIA_UDP)	+= udp_media.o
tipc-$(CONFIG_TIPC_MEDIA_IB)	+= ib_media.o
tipc-$(CONFIG_SYSCTL)		+= sysctl.o
tipc-$(CONFIG_TIPC_CRYPTO)	+= crypto.o

obj-$(CONFIG_TIPC_DIAG)	+= tipc_diag.o
tipc_diag-y	+= diag.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
