<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for arch/parisc/kernel
#

<<<<<<< HEAD
extra-y			:= init_task.o head.o vmlinux.lds

obj-y	     	:= cache.o pacache.o setup.o traps.o time.o irq.o \
		   pa7300lc.o syscall.o entry.o sys_parisc.o firmware.o \
		   ptrace.o hardware.o inventory.o drivers.o \
		   signal.o hpmc.o real2.o parisc_ksyms.o unaligned.o \
		   process.o processor.o pdc_cons.o pdc_chassis.o unwind.o \
		   topology.o

ifdef CONFIG_FUNCTION_TRACER
# Do not profile debug and lowlevel utilities
CFLAGS_REMOVE_ftrace.o = -pg
CFLAGS_REMOVE_cache.o = -pg
CFLAGS_REMOVE_irq.o = -pg
CFLAGS_REMOVE_pacache.o = -pg
CFLAGS_REMOVE_perf.o = -pg
CFLAGS_REMOVE_traps.o = -pg
CFLAGS_REMOVE_unaligned.o = -pg
CFLAGS_REMOVE_unwind.o = -pg
endif

=======
extra-y		:= vmlinux.lds

obj-y		:= head.o cache.o pacache.o setup.o pdt.o traps.o time.o irq.o \
		   syscall.o entry.o sys_parisc.o firmware.o \
		   ptrace.o hardware.o inventory.o drivers.o alternative.o \
		   signal.o hpmc.o real2.o parisc_ksyms.o unaligned.o \
		   process.o processor.o pdc_cons.o pdc_chassis.o unwind.o \
		   patch.o toc.o toc_asm.o

ifdef CONFIG_FUNCTION_TRACER
# Do not profile debug and lowlevel utilities
CFLAGS_REMOVE_ftrace.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_cache.o =  $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_perf.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_unwind.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_patch.o = $(CC_FLAGS_FTRACE)
endif

CFLAGS_REMOVE_sys_parisc.o = -Wmissing-prototypes -Wmissing-declarations
CFLAGS_REMOVE_sys_parisc32.o = -Wmissing-prototypes -Wmissing-declarations

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_SMP)	+= smp.o
obj-$(CONFIG_PA11)	+= pci-dma.o
obj-$(CONFIG_PCI)	+= pci.o
obj-$(CONFIG_MODULES)	+= module.o
<<<<<<< HEAD
obj-$(CONFIG_64BIT)	+= binfmt_elf32.o sys_parisc32.o signal32.o
obj-$(CONFIG_STACKTRACE)+= stacktrace.o
# only supported for PCX-W/U in 64-bit mode at the moment
obj-$(CONFIG_64BIT)	+= perf.o perf_asm.o
obj-$(CONFIG_FUNCTION_TRACER)		+= ftrace.o
obj-$(CONFIG_FUNCTION_GRAPH_TRACER)	+= ftrace.o
=======
obj-$(CONFIG_64BIT)	+= sys_parisc32.o signal32.o
obj-$(CONFIG_STACKTRACE)+= stacktrace.o
obj-$(CONFIG_AUDIT)	+= audit.o
obj64-$(CONFIG_AUDIT)	+= compat_audit.o
# only supported for PCX-W/U in 64-bit mode at the moment
obj-$(CONFIG_64BIT)	+= perf.o perf_asm.o $(obj64-y)
obj-$(CONFIG_GENERIC_ARCH_TOPOLOGY)	+= topology.o
obj-$(CONFIG_FUNCTION_TRACER)		+= ftrace.o
obj-$(CONFIG_FUNCTION_GRAPH_TRACER)	+= ftrace.o
obj-$(CONFIG_JUMP_LABEL)		+= jump_label.o
obj-$(CONFIG_KGDB)			+= kgdb.o
obj-$(CONFIG_KPROBES)			+= kprobes.o
obj-$(CONFIG_KEXEC_CORE)		+= kexec.o relocate_kernel.o
obj-$(CONFIG_KEXEC_FILE)		+= kexec_file.o

# vdso
obj-y			+= vdso.o
obj-$(CONFIG_64BIT)	+= vdso64/
obj-y			+= vdso32/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
