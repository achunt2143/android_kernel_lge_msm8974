<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux Coda filesystem routines.
#

obj-$(CONFIG_CODA_FS) += coda.o

coda-objs := psdev.o cache.o cnode.o inode.o dir.o file.o upcall.o \
<<<<<<< HEAD
	     coda_linux.o symlink.o pioctl.o sysctl.o 
=======
	     coda_linux.o symlink.o pioctl.o
coda-$(CONFIG_SYSCTL) += sysctl.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# If you want debugging output, please uncomment the following line.

# ccflags-y := -DDEBUG -DDEBUG_SMB_MALLOC=1
