<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the PPS core.
#

pps_core-y			:= pps.o kapi.o sysfs.o
pps_core-$(CONFIG_NTP_PPS)	+= kc.o
obj-$(CONFIG_PPS)		:= pps_core.o
obj-y				+= clients/ generators/

ccflags-$(CONFIG_PPS_DEBUG) := -DDEBUG
