<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for Red Hat Linux AFS client.
#

<<<<<<< HEAD
afs-cache-$(CONFIG_AFS_FSCACHE) := cache.o

kafs-objs := \
	$(afs-cache-y) \
=======
kafs-y := \
	addr_list.o \
	addr_prefs.o \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	callback.o \
	cell.o \
	cmservice.o \
	dir.o \
<<<<<<< HEAD
	file.o \
	flock.o \
	fsclient.o \
=======
	dir_edit.o \
	dir_silly.o \
	dynroot.o \
	file.o \
	flock.o \
	fsclient.o \
	fs_operation.o \
	fs_probe.o \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inode.o \
	main.o \
	misc.o \
	mntpt.o \
<<<<<<< HEAD
	proc.o \
	rxrpc.o \
	security.o \
	server.o \
	super.o \
	netdevices.o \
	vlclient.o \
	vlocation.o \
	vnode.o \
	volume.o \
	write.o

=======
	rotate.o \
	rxrpc.o \
	security.o \
	server.o \
	server_list.o \
	super.o \
	validation.o \
	vlclient.o \
	vl_alias.o \
	vl_list.o \
	vl_probe.o \
	vl_rotate.o \
	volume.o \
	write.o \
	xattr.o \
	yfsclient.o

kafs-$(CONFIG_PROC_FS) += proc.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_AFS_FS)  := kafs.o
