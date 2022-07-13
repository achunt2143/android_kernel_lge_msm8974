#
# Copyright (C) 2000 Jeff Dike (jdike@karaya.com)
# Licensed under the GPL
#

<<<<<<< HEAD
hostfs-objs := hostfs_kern.o hostfs_user.o

obj-y :=
obj-$(CONFIG_HOSTFS) += hostfs.o

include arch/um/scripts/Makefile.rules
=======
hostfs-objs := hostfs_kern.o

hostfs-builtin-$(CONFIG_HOSTFS) += hostfs_user.o hostfs_user_exp.o

obj-y := $(hostfs-builtin-y) $(hostfs-builtin-m)
obj-$(CONFIG_HOSTFS) += hostfs.o

include $(srctree)/arch/um/scripts/Makefile.rules
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
