<<<<<<< HEAD
# Makefile for xmon

subdir-ccflags-$(CONFIG_PPC_WERROR) := -Werror

GCOV_PROFILE := n

ccflags-$(CONFIG_PPC64) := -mno-minimal-toc

obj-y			+= xmon.o start.o nonstdio.o
=======
# SPDX-License-Identifier: GPL-2.0
# Makefile for xmon

GCOV_PROFILE := n
KCOV_INSTRUMENT := n
UBSAN_SANITIZE := n
KASAN_SANITIZE := n
KCSAN_SANITIZE := n

# Disable ftrace for the entire directory
ccflags-remove-$(CONFIG_FUNCTION_TRACER) += $(CC_FLAGS_FTRACE)

ccflags-$(CONFIG_PPC64) := $(NO_MINIMAL_TOC)

# Clang stores addresses on the stack causing the frame size to blow
# out. See https://github.com/ClangBuiltLinux/linux/issues/252
ccflags-$(CONFIG_CC_IS_CLANG) += -Wframe-larger-than=4096

obj-y			+= xmon.o nonstdio.o spr_access.o xmon_bpts.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

ifdef CONFIG_XMON_DISASSEMBLY
obj-y			+= ppc-dis.o ppc-opc.o
obj-$(CONFIG_SPU_BASE)	+= spu-dis.o spu-opc.o
endif
