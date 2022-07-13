<<<<<<< HEAD
CFLAGS_bootx_init.o  		+= -fPIC

ifdef CONFIG_FUNCTION_TRACER
# Do not trace early boot code
CFLAGS_REMOVE_bootx_init.o = -pg -mno-sched-epilog
=======
# SPDX-License-Identifier: GPL-2.0
CFLAGS_bootx_init.o  		+= -fPIC
CFLAGS_bootx_init.o		+= -fno-stack-protector

KASAN_SANITIZE_bootx_init.o := n

ifdef CONFIG_KASAN
CFLAGS_bootx_init.o  		+= -DDISABLE_BRANCH_PROFILING
endif

ifdef CONFIG_FUNCTION_TRACER
# Do not trace early boot code
CFLAGS_REMOVE_bootx_init.o = $(CC_FLAGS_FTRACE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
endif

obj-y				+= pic.o setup.o time.o feature.o pci.o \
				   sleep.o low_i2c.o cache.o pfunc_core.o \
				   pfunc_base.o udbg_scc.o udbg_adb.o
obj-$(CONFIG_PMAC_BACKLIGHT)	+= backlight.o
<<<<<<< HEAD
obj-$(CONFIG_CPU_FREQ_PMAC)	+= cpufreq_32.o
obj-$(CONFIG_CPU_FREQ_PMAC64)	+= cpufreq_64.o
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# CONFIG_NVRAM is an arch. independent tristate symbol, for pmac32 we really
# need this to be a bool.  Cheat here and pretend CONFIG_NVRAM=m is really
# CONFIG_NVRAM=y
obj-$(CONFIG_NVRAM:m=y)		+= nvram.o
<<<<<<< HEAD
# ppc64 pmac doesn't define CONFIG_NVRAM but needs nvram stuff
obj-$(CONFIG_PPC64)		+= nvram.o
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_PPC32)		+= bootx_init.o
obj-$(CONFIG_SMP)		+= smp.o
