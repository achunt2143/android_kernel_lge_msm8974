<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux NFC subsystem.
#

obj-$(CONFIG_NFC) += nfc.o
obj-$(CONFIG_NFC_NCI) += nci/
<<<<<<< HEAD

nfc-objs := core.o netlink.o af_nfc.o rawsock.o
nfc-$(CONFIG_NFC_LLCP)	+= llcp/llcp.o llcp/commands.o llcp/sock.o
=======
obj-$(CONFIG_NFC_HCI) += hci/
obj-$(CONFIG_NFC_DIGITAL) += nfc_digital.o

nfc-objs := core.o netlink.o af_nfc.o rawsock.o llcp_core.o llcp_commands.o \
		llcp_sock.o

nfc_digital-objs := digital_core.o digital_technology.o digital_dep.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
