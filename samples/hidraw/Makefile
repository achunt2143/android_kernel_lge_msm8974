<<<<<<< HEAD
# kbuild trick to avoid linker error. Can be omitted if a module is built.
obj- := dummy.o

# List of programs to build
hostprogs-y := hid-example

# Tell kbuild to always build the programs
always := $(hostprogs-y)

HOSTCFLAGS_hid-example.o += -I$(objtree)/usr/include
=======
# SPDX-License-Identifier: GPL-2.0
userprogs-always-y += hid-example

userccflags += -I usr/include
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
