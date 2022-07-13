<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux unix domain socket layer.
#

obj-$(CONFIG_UNIX)	+= unix.o

unix-y			:= af_unix.o garbage.o
unix-$(CONFIG_SYSCTL)	+= sysctl_net_unix.o
<<<<<<< HEAD
=======
unix-$(CONFIG_BPF_SYSCALL) += unix_bpf.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

obj-$(CONFIG_UNIX_DIAG)	+= unix_diag.o
unix_diag-y		:= diag.o
