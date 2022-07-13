<<<<<<< HEAD
# kbuild trick to avoid linker error. Can be omitted if a module is built.
obj- := dummy.o

# List of programs to build
hostprogs-y := uhid-example

# Tell kbuild to always build the programs
always := $(hostprogs-y)

HOSTCFLAGS_uhid-example.o += -I$(objtree)/usr/include
=======
# SPDX-License-Identifier: GPL-2.0-only
userprogs-always-y += uhid-example

userccflags += -I usr/include
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
