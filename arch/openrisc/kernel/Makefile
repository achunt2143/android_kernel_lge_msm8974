<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the linux kernel.
#

<<<<<<< HEAD
extra-y	:= head.o vmlinux.lds init_task.o

obj-y	:= setup.o idle.o or32_ksyms.o process.o dma.o \
	   traps.o time.o irq.o entry.o ptrace.o signal.o sys_or32.o \
	   sys_call_table.o

=======
extra-y	:= vmlinux.lds

obj-y	:= head.o setup.o or32_ksyms.o process.o dma.o \
	   traps.o time.o irq.o entry.o ptrace.o signal.o \
	   sys_call_table.o unwinder.o

obj-$(CONFIG_SMP)		+= smp.o sync-timer.o
obj-$(CONFIG_STACKTRACE)	+= stacktrace.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_MODULES)		+= module.o
obj-$(CONFIG_OF)		+= prom.o

clean:
