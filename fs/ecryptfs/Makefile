<<<<<<< HEAD
#
# Makefile for the Linux 2.6 eCryptfs
=======
# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile for the Linux eCryptfs
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#

obj-$(CONFIG_ECRYPT_FS) += ecryptfs.o

<<<<<<< HEAD
ecryptfs-objs := dentry.o file.o inode.o main.o super.o mmap.o read_write.o crypto.o keystore.o messaging.o miscdev.o kthread.o debug.o
=======
ecryptfs-y := dentry.o file.o inode.o main.o super.o mmap.o read_write.o \
	      crypto.o keystore.o kthread.o debug.o

ecryptfs-$(CONFIG_ECRYPT_FS_MESSAGING) += messaging.o miscdev.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
