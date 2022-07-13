<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile
#

ifdef CONFIG_FUNCTION_TRACER
# Do not trace early boot code and low level code
CFLAGS_REMOVE_timer.o = -pg
CFLAGS_REMOVE_intc.o = -pg
CFLAGS_REMOVE_early_printk.o = -pg
<<<<<<< HEAD
CFLAGS_REMOVE_selfmod.o = -pg
CFLAGS_REMOVE_heartbeat.o = -pg
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
CFLAGS_REMOVE_ftrace.o = -pg
CFLAGS_REMOVE_process.o = -pg
endif

<<<<<<< HEAD
extra-y := head.o vmlinux.lds

obj-y += dma.o exceptions.o \
	hw_exception_handler.o init_task.o intc.o irq.o \
	process.o prom.o prom_parse.o ptrace.o \
=======
extra-y := vmlinux.lds

obj-y += head.o dma.o exceptions.o \
	hw_exception_handler.o irq.o \
	process.o prom.o ptrace.o \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	reset.o setup.o signal.o sys_microblaze.o timer.o traps.o unwind.o

obj-y += cpu/

<<<<<<< HEAD
obj-$(CONFIG_EARLY_PRINTK)	+= early_printk.o
obj-$(CONFIG_SELFMOD)		+= selfmod.o
obj-$(CONFIG_HEART_BEAT)	+= heartbeat.o
obj-$(CONFIG_MODULES)		+= microblaze_ksyms.o module.o
obj-$(CONFIG_MMU)		+= misc.o
=======
obj-$(CONFIG_MODULES)		+= microblaze_ksyms.o module.o
obj-y				+= misc.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_STACKTRACE)	+= stacktrace.o
obj-$(CONFIG_FUNCTION_TRACER)	+= ftrace.o mcount.o
obj-$(CONFIG_KGDB)		+= kgdb.o

<<<<<<< HEAD
obj-y	+= entry$(MMU).o
=======
obj-y	+= entry.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
