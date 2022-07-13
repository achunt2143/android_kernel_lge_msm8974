<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the linux kernel.
#

ifdef CONFIG_FUNCTION_TRACER
<<<<<<< HEAD
# Don't trace early setup code and tracing code
CFLAGS_REMOVE_early.o = -pg
CFLAGS_REMOVE_ftrace.o = -pg
endif

#
# Passing null pointers is ok for smp code, since we access the lowcore here.
#
CFLAGS_smp.o	:= -Wno-nonnull

#
# Pass UTS_MACHINE for user_regset definition
#
CFLAGS_ptrace.o		+= -DUTS_MACHINE='"$(UTS_MACHINE)"'

CFLAGS_sysinfo.o += -Iinclude/math-emu -Iarch/s390/math-emu -w

obj-y	:=  bitmap.o traps.o time.o process.o base.o early.o setup.o vtime.o \
	    processor.o sys_s390.o ptrace.o signal.o cpcmd.o ebcdic.o nmi.o \
	    debug.o irq.o ipl.o dis.o diag.o mem_detect.o sclp.o vdso.o \
	    sysinfo.o jump_label.o lgr.o os_info.o

obj-y	+= $(if $(CONFIG_64BIT),entry64.o,entry.o)
obj-y	+= $(if $(CONFIG_64BIT),reipl64.o,reipl.o)

extra-y				+= head.o init_task.o vmlinux.lds
extra-y				+= $(if $(CONFIG_64BIT),head64.o,head31.o)

obj-$(CONFIG_MODULES)		+= s390_ksyms.o module.o
obj-$(CONFIG_SMP)		+= smp.o
obj-$(CONFIG_SCHED_BOOK)	+= topology.o
obj-$(CONFIG_HIBERNATION)	+= suspend.o swsusp_asm64.o
obj-$(CONFIG_AUDIT)		+= audit.o
compat-obj-$(CONFIG_AUDIT)	+= compat_audit.o
obj-$(CONFIG_COMPAT)		+= compat_linux.o compat_signal.o \
					compat_wrapper.o compat_exec_domain.o \
					$(compat-obj-y)

obj-$(CONFIG_STACKTRACE)	+= stacktrace.o
obj-$(CONFIG_KPROBES)		+= kprobes.o
obj-$(CONFIG_FUNCTION_TRACER)	+= $(if $(CONFIG_64BIT),mcount64.o,mcount.o)
obj-$(CONFIG_DYNAMIC_FTRACE)	+= ftrace.o
obj-$(CONFIG_FUNCTION_GRAPH_TRACER) += ftrace.o
obj-$(CONFIG_FTRACE_SYSCALLS)  += ftrace.o
obj-$(CONFIG_CRASH_DUMP)	+= crash_dump.o
obj-$(CONFIG_PERF_EVENTS)	+= perf_event.o perf_cpum_cf.o

# Kexec part
S390_KEXEC_OBJS := machine_kexec.o crash.o
S390_KEXEC_OBJS += $(if $(CONFIG_64BIT),relocate_kernel64.o,relocate_kernel.o)
obj-$(CONFIG_KEXEC) += $(S390_KEXEC_OBJS)

# vdso
obj-$(CONFIG_64BIT)		+= vdso64/
obj-$(CONFIG_32BIT)		+= vdso32/
=======

# Do not trace tracer code
CFLAGS_REMOVE_ftrace.o		= $(CC_FLAGS_FTRACE)

# Do not trace early setup code
CFLAGS_REMOVE_early.o		= $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_rethook.o		= $(CC_FLAGS_FTRACE)

endif

GCOV_PROFILE_early.o		:= n
KCOV_INSTRUMENT_early.o		:= n
UBSAN_SANITIZE_early.o		:= n
KASAN_SANITIZE_ipl.o		:= n
KASAN_SANITIZE_machine_kexec.o	:= n

#
# Passing null pointers is ok for smp code, since we access the lowcore here.
#
CFLAGS_smp.o		:= -Wno-nonnull

#
# Disable tailcall optimizations for stack / callchain walking functions
# since this might generate broken code when accessing register 15 and
# passing its content to other functions.
#
CFLAGS_stacktrace.o	+= -fno-optimize-sibling-calls
CFLAGS_dumpstack.o	+= -fno-optimize-sibling-calls
CFLAGS_unwind_bc.o	+= -fno-optimize-sibling-calls

obj-y	:= head64.o traps.o time.o process.o earlypgm.o early.o setup.o idle.o vtime.o
obj-y	+= processor.o syscall.o ptrace.o signal.o cpcmd.o ebcdic.o nmi.o
obj-y	+= debug.o irq.o ipl.o dis.o diag.o vdso.o cpufeature.o
obj-y	+= sysinfo.o lgr.o os_info.o ctlreg.o
obj-y	+= runtime_instr.o cache.o fpu.o dumpstack.o guarded_storage.o sthyi.o
obj-y	+= entry.o reipl.o kdebugfs.o alternative.o
obj-y	+= nospec-branch.o ipl_vmparm.o machine_kexec_reloc.o unwind_bc.o
obj-y	+= smp.o text_amode31.o stacktrace.o abs_lowcore.o facility.o

extra-y				+= vmlinux.lds

obj-$(CONFIG_SYSFS)		+= nospec-sysfs.o
CFLAGS_REMOVE_nospec-branch.o	+= $(CC_FLAGS_EXPOLINE)

obj-$(CONFIG_MODULES)		+= module.o
obj-$(CONFIG_SCHED_TOPOLOGY)	+= topology.o
obj-$(CONFIG_NUMA)		+= numa.o
obj-$(CONFIG_AUDIT)		+= audit.o
compat-obj-$(CONFIG_AUDIT)	+= compat_audit.o
obj-$(CONFIG_COMPAT)		+= compat_linux.o compat_signal.o
obj-$(CONFIG_COMPAT)		+= $(compat-obj-y)
obj-$(CONFIG_EARLY_PRINTK)	+= early_printk.o
obj-$(CONFIG_KPROBES)		+= kprobes.o
obj-$(CONFIG_KPROBES)		+= kprobes_insn_page.o
obj-$(CONFIG_KPROBES)		+= mcount.o
obj-$(CONFIG_RETHOOK)		+= rethook.o
obj-$(CONFIG_FUNCTION_TRACER)	+= ftrace.o
obj-$(CONFIG_FUNCTION_TRACER)	+= mcount.o
obj-$(CONFIG_CRASH_DUMP)	+= crash_dump.o
obj-$(CONFIG_KEXEC_CORE)	+= machine_kexec.o relocate_kernel.o
obj-$(CONFIG_VMCORE_INFO)	+= vmcore_info.o
obj-$(CONFIG_UPROBES)		+= uprobes.o
obj-$(CONFIG_JUMP_LABEL)	+= jump_label.o

obj-$(CONFIG_KEXEC_FILE)	+= machine_kexec_file.o kexec_image.o
obj-$(CONFIG_KEXEC_FILE)	+= kexec_elf.o
obj-$(CONFIG_CERT_STORE)	+= cert_store.o
obj-$(CONFIG_IMA_SECURE_AND_OR_TRUSTED_BOOT)	+= ima_arch.o

obj-$(CONFIG_PERF_EVENTS)	+= perf_event.o
obj-$(CONFIG_PERF_EVENTS)	+= perf_cpum_cf.o perf_cpum_sf.o
obj-$(CONFIG_PERF_EVENTS)	+= perf_cpum_cf_events.o perf_regs.o
obj-$(CONFIG_PERF_EVENTS)	+= perf_pai_crypto.o perf_pai_ext.o

obj-$(CONFIG_TRACEPOINTS)	+= trace.o
obj-$(findstring y, $(CONFIG_PROTECTED_VIRTUALIZATION_GUEST) $(CONFIG_PGSTE))	+= uv.o

# vdso
obj-y				+= vdso64/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_COMPAT)		+= vdso32/
