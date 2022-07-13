<<<<<<< HEAD
hostprogs-y	:= genheaders
HOST_EXTRACFLAGS += -Isecurity/selinux/include

always		:= $(hostprogs-y)
clean-files	:= $(hostprogs-y)
=======
# SPDX-License-Identifier: GPL-2.0
hostprogs-always-y += genheaders
HOST_EXTRACFLAGS += \
	-I$(srctree)/include/uapi -I$(srctree)/include \
	-I$(srctree)/security/selinux/include
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
