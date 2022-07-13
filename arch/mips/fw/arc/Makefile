<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the ARC prom monitor library routines under Linux.
#

<<<<<<< HEAD
lib-y				+= cmdline.o env.o file.o identify.o init.o \
				   misc.o salone.o time.o tree.o
=======
ifdef CONFIG_ARC_CMDLINE_ONLY
lib-y				+= cmdline.o
else
lib-y				+= cmdline.o env.o file.o identify.o init.o \
				   misc.o
endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

lib-$(CONFIG_ARC_MEMORY)	+= memory.o
lib-$(CONFIG_ARC_CONSOLE)	+= arc_con.o
lib-$(CONFIG_ARC_PROMLIB)	+= promlib.o
<<<<<<< HEAD

ccflags-y			:= -Werror
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
