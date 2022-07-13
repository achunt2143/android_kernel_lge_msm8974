<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux isofs filesystem routines.
#

obj-$(CONFIG_ISO9660_FS) += isofs.o

isofs-objs-y 			:= namei.o inode.o dir.o util.o rock.o export.o
isofs-objs-$(CONFIG_JOLIET)	+= joliet.o
isofs-objs-$(CONFIG_ZISOFS)	+= compress.o
isofs-objs			:= $(isofs-objs-y)
