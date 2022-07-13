<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
#  Makefile for the Linux Controller Area Network core.
#

obj-$(CONFIG_CAN)	+= can.o
<<<<<<< HEAD
can-y			:= af_can.o proc.o
=======
can-y			:= af_can.o
can-$(CONFIG_PROC_FS)	+= proc.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

obj-$(CONFIG_CAN_RAW)	+= can-raw.o
can-raw-y		:= raw.o

obj-$(CONFIG_CAN_BCM)	+= can-bcm.o
can-bcm-y		:= bcm.o

obj-$(CONFIG_CAN_GW)	+= can-gw.o
can-gw-y		:= gw.o
<<<<<<< HEAD
=======

obj-$(CONFIG_CAN_J1939)	+= j1939/

obj-$(CONFIG_CAN_ISOTP)	+= can-isotp.o
can-isotp-y		:= isotp.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
