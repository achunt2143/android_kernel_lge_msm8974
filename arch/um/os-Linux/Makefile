<<<<<<< HEAD
# 
# Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
# Licensed under the GPL
#

obj-y = aio.o execvp.o file.o helper.o irq.o main.o mem.o process.o \
	registers.o sigio.o signal.o start_up.o time.o tty.o \
	umid.o user_syms.o util.o drivers/ skas/

obj-$(CONFIG_ARCH_REUSE_HOST_VSYSCALL_AREA) += elf_aux.o

USER_OBJS := $(user-objs-y) aio.o elf_aux.o execvp.o file.o helper.o irq.o \
	main.o mem.o process.o registers.o sigio.o signal.o start_up.o time.o \
	tty.o umid.o util.o

HAVE_AIO_ABI := $(shell [ -r /usr/include/linux/aio_abi.h ] && \
	echo -DHAVE_AIO_ABI )
CFLAGS_aio.o += $(HAVE_AIO_ABI)

include arch/um/scripts/Makefile.rules
=======
# SPDX-License-Identifier: GPL-2.0
# 
# Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
#

# Don't instrument UML-specific code
KCOV_INSTRUMENT                := n

obj-y = execvp.o file.o helper.o irq.o main.o mem.o process.o \
	registers.o sigio.o signal.o start_up.o time.o tty.o \
	umid.o user_syms.o util.o drivers/ skas/

CFLAGS_signal.o += -Wframe-larger-than=4096

obj-$(CONFIG_ARCH_REUSE_HOST_VSYSCALL_AREA) += elf_aux.o

USER_OBJS := $(user-objs-y) elf_aux.o execvp.o file.o helper.o irq.o \
	main.o mem.o process.o registers.o sigio.o signal.o start_up.o time.o \
	tty.o umid.o util.o

include $(srctree)/arch/um/scripts/Makefile.rules
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
