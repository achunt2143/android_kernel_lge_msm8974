<<<<<<< HEAD
# scripts/dtc makefile

hostprogs-y	:= dtc
always		:= $(hostprogs-y)
=======
# SPDX-License-Identifier: GPL-2.0
# scripts/dtc makefile

# *** Also keep .gitignore in sync when changing ***
hostprogs-always-$(CONFIG_DTC)		+= dtc fdtoverlay
hostprogs-always-$(CHECK_DT_BINDING)	+= dtc
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

dtc-objs	:= dtc.o flattree.o fstree.o data.o livetree.o treesource.o \
		   srcpos.o checks.o util.o
dtc-objs	+= dtc-lexer.lex.o dtc-parser.tab.o

<<<<<<< HEAD
# Source files need to get at the userspace version of libfdt_env.h to compile

HOSTCFLAGS_DTC := -I$(src) -I$(src)/libfdt

HOSTCFLAGS_checks.o := $(HOSTCFLAGS_DTC)
HOSTCFLAGS_data.o := $(HOSTCFLAGS_DTC)
HOSTCFLAGS_dtc.o := $(HOSTCFLAGS_DTC)
HOSTCFLAGS_flattree.o := $(HOSTCFLAGS_DTC)
HOSTCFLAGS_fstree.o := $(HOSTCFLAGS_DTC)
HOSTCFLAGS_livetree.o := $(HOSTCFLAGS_DTC)
HOSTCFLAGS_srcpos.o := $(HOSTCFLAGS_DTC)
HOSTCFLAGS_treesource.o := $(HOSTCFLAGS_DTC)
HOSTCFLAGS_util.o := $(HOSTCFLAGS_DTC)

HOSTCFLAGS_dtc-lexer.lex.o := $(HOSTCFLAGS_DTC)
HOSTCFLAGS_dtc-parser.tab.o := $(HOSTCFLAGS_DTC)

# dependencies on generated files need to be listed explicitly
$(obj)/dtc-lexer.lex.o: $(obj)/dtc-parser.tab.h

# generated files need to be cleaned explicitly
clean-files	:= dtc-lexer.lex.c dtc-parser.tab.c dtc-parser.tab.h
=======
# The upstream project builds libfdt as a separate library.  We are choosing to
# instead directly link the libfdt object files into fdtoverlay.
libfdt-objs	:= fdt.o fdt_ro.o fdt_wip.o fdt_sw.o fdt_rw.o fdt_strerror.o fdt_empty_tree.o fdt_addresses.o fdt_overlay.o
libfdt		= $(addprefix libfdt/,$(libfdt-objs))
fdtoverlay-objs	:= $(libfdt) fdtoverlay.o util.o

# Source files need to get at the userspace version of libfdt_env.h to compile
HOST_EXTRACFLAGS += -I $(srctree)/$(src)/libfdt
HOST_EXTRACFLAGS += -DNO_YAML

# Generated files need one more search path to include headers in source tree
HOSTCFLAGS_dtc-lexer.lex.o := -I $(srctree)/$(src)
HOSTCFLAGS_dtc-parser.tab.o := -I $(srctree)/$(src)

# dependencies on generated files need to be listed explicitly
$(obj)/dtc-lexer.lex.o: $(obj)/dtc-parser.tab.h
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
