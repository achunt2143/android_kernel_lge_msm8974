<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for CEPH filesystem.
#

obj-$(CONFIG_CEPH_FS) += ceph.o

ceph-y := super.o inode.o dir.o file.o locks.o addr.o ioctl.o \
<<<<<<< HEAD
	export.o caps.o snap.o xattr.o \
	mds_client.o mdsmap.o strings.o ceph_frag.o \
	debugfs.o

=======
	export.o caps.o snap.o xattr.o quota.o io.o \
	mds_client.o mdsmap.o strings.o ceph_frag.o \
	debugfs.o util.o metric.o

ceph-$(CONFIG_CEPH_FSCACHE) += cache.o
ceph-$(CONFIG_CEPH_FS_POSIX_ACL) += acl.o
ceph-$(CONFIG_FS_ENCRYPTION) += crypto.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
