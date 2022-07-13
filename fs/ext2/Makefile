<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the linux ext2-filesystem routines.
#

obj-$(CONFIG_EXT2_FS) += ext2.o

ext2-y := balloc.o dir.o file.o ialloc.o inode.o \
<<<<<<< HEAD
	  ioctl.o namei.o super.o symlink.o
=======
	  ioctl.o namei.o super.o symlink.o trace.o

# For tracepoints to include our trace.h from tracepoint infrastructure
CFLAGS_trace.o := -I$(src)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

ext2-$(CONFIG_EXT2_FS_XATTR)	 += xattr.o xattr_user.o xattr_trusted.o
ext2-$(CONFIG_EXT2_FS_POSIX_ACL) += acl.o
ext2-$(CONFIG_EXT2_FS_SECURITY)	 += xattr_security.o
<<<<<<< HEAD
ext2-$(CONFIG_EXT2_FS_XIP)	 += xip.o
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
