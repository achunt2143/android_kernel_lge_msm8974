<<<<<<< HEAD
ifdef CONFIG_FUNCTION_TRACER
CFLAGS_REMOVE_core.o = -pg
endif

obj-y := core.o ring_buffer.o callchain.o
obj-$(CONFIG_HAVE_HW_BREAKPOINT) += hw_breakpoint.o
=======
# SPDX-License-Identifier: GPL-2.0
obj-y := core.o ring_buffer.o callchain.o

obj-$(CONFIG_HAVE_HW_BREAKPOINT) += hw_breakpoint.o
obj-$(CONFIG_HW_BREAKPOINT_KUNIT_TEST) += hw_breakpoint_test.o
obj-$(CONFIG_UPROBES) += uprobes.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
