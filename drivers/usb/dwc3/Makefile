<<<<<<< HEAD
ccflags-$(CONFIG_USB_DWC3_DEBUG)	:= -DDEBUG
ccflags-$(CONFIG_USB_DWC3_VERBOSE)	+= -DVERBOSE_DEBUG
ccflags-y += -Idrivers/usb/host
ccflags-y += -Idrivers/base/power
=======
# SPDX-License-Identifier: GPL-2.0
# define_trace.h needs to know how to find our header
CFLAGS_trace.o				:= -I$(src)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

obj-$(CONFIG_USB_DWC3)			+= dwc3.o

dwc3-y					:= core.o
<<<<<<< HEAD
dwc3-y					+= host.o
dwc3-y					+= gadget.o ep0.o
dwc3-y					+= dwc3_otg.o
=======

ifneq ($(CONFIG_TRACING),)
	dwc3-y				+= trace.o
endif

ifneq ($(filter y,$(CONFIG_USB_DWC3_HOST) $(CONFIG_USB_DWC3_DUAL_ROLE)),)
	dwc3-y				+= host.o
endif

ifneq ($(filter y,$(CONFIG_USB_DWC3_GADGET) $(CONFIG_USB_DWC3_DUAL_ROLE)),)
	dwc3-y				+= gadget.o ep0.o
endif

ifneq ($(CONFIG_USB_DWC3_DUAL_ROLE),)
	dwc3-y				+= drd.o
endif

ifneq ($(CONFIG_USB_DWC3_ULPI),)
	dwc3-y				+= ulpi.o
endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

ifneq ($(CONFIG_DEBUG_FS),)
	dwc3-y				+= debugfs.o
endif

##
# Platform-specific glue layers go here
#
# NOTICE: Make sure your glue layer doesn't depend on anything
# which is arch-specific and that it compiles on all situations.
#
# We want to keep this requirement in order to be able to compile
# the entire driver (with all its glue layers) on several architectures
# and make sure it compiles fine. This will also help with allmodconfig
# and allyesconfig builds.
<<<<<<< HEAD
#
# The only exception is the PCI glue layer, but that's only because
# PCI doesn't provide nops if CONFIG_PCI isn't enabled.
##


obj-$(CONFIG_USB_DWC3_OMAP)	+= dwc3-omap.o
obj-$(CONFIG_USB_DWC3_MSM)	+= dwc3-msm.o

##
# REVISIT Samsung Exynos platform needs the clk API which isn't
# defined on all architectures. If we allow dwc3-exynos.c compile
# always we will fail the linking phase on those architectures
# which don't provide clk api implementation and that's unnaceptable.
#
# When Samsung's platform start supporting pm_runtime, this check
# for HAVE_CLK should be removed.
##
ifneq ($(CONFIG_HAVE_CLK),)
	obj-$(CONFIG_USB_DWC3)		+= dwc3-exynos.o
endif

ifneq ($(CONFIG_PCI),)
	obj-$(CONFIG_USB_DWC3)		+= dwc3-pci.o
endif

=======
##

obj-$(CONFIG_USB_DWC3_AM62)		+= dwc3-am62.o
obj-$(CONFIG_USB_DWC3_OMAP)		+= dwc3-omap.o
obj-$(CONFIG_USB_DWC3_EXYNOS)		+= dwc3-exynos.o
obj-$(CONFIG_USB_DWC3_PCI)		+= dwc3-pci.o
obj-$(CONFIG_USB_DWC3_HAPS)		+= dwc3-haps.o
obj-$(CONFIG_USB_DWC3_KEYSTONE)		+= dwc3-keystone.o
obj-$(CONFIG_USB_DWC3_MESON_G12A)	+= dwc3-meson-g12a.o
obj-$(CONFIG_USB_DWC3_OF_SIMPLE)	+= dwc3-of-simple.o
obj-$(CONFIG_USB_DWC3_ST)		+= dwc3-st.o
obj-$(CONFIG_USB_DWC3_QCOM)		+= dwc3-qcom.o
obj-$(CONFIG_USB_DWC3_IMX8MP)		+= dwc3-imx8mp.o
obj-$(CONFIG_USB_DWC3_XILINX)		+= dwc3-xilinx.o
obj-$(CONFIG_USB_DWC3_OCTEON)		+= dwc3-octeon.o
obj-$(CONFIG_USB_DWC3_RTK)		+= dwc3-rtk.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
