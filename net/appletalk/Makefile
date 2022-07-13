<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux AppleTalk layer.
#

obj-$(CONFIG_ATALK) += appletalk.o

appletalk-y			:= aarp.o ddp.o dev.o
appletalk-$(CONFIG_PROC_FS)	+= atalk_proc.o
appletalk-$(CONFIG_SYSCTL)	+= sysctl_net_atalk.o
