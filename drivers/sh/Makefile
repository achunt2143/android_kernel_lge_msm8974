<<<<<<< HEAD
#
# Makefile for the SuperH specific drivers.
#
obj-y	:= intc/

obj-$(CONFIG_HAVE_CLK)		+= clk/
obj-$(CONFIG_MAPLE)		+= maple/
obj-$(CONFIG_SUPERHYWAY)	+= superhyway/
obj-$(CONFIG_GENERIC_GPIO)	+= pfc.o
obj-y				+= pm_runtime.o
=======
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the SuperH specific drivers.
#
obj-$(CONFIG_SH_INTC)			+= intc/
ifneq ($(CONFIG_COMMON_CLK),y)
obj-$(CONFIG_HAVE_CLK)			+= clk/
endif
obj-$(CONFIG_MAPLE)			+= maple/

obj-y					+= pm_runtime.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
