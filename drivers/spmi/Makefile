<<<<<<< HEAD
#
# Makefile for kernel SPMI framework.
#
obj-$(CONFIG_SPMI)			+= spmi.o spmi-resources.o
obj-$(CONFIG_SPMI_MSM_PMIC_ARB)		+= spmi-pmic-arb.o
obj-$(CONFIG_MSM_QPNP_INT)		+= qpnp-int.o

ifdef CONFIG_DEBUG_FS
obj-$(CONFIG_SPMI)			+= spmi-dbgfs.o
endif
=======
# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile for kernel SPMI framework.
#
obj-$(CONFIG_SPMI)	+= spmi.o spmi-devres.o

obj-$(CONFIG_SPMI_HISI3670)	+= hisi-spmi-controller.o
obj-$(CONFIG_SPMI_MSM_PMIC_ARB)	+= spmi-pmic-arb.o
obj-$(CONFIG_SPMI_MTK_PMIF)	+= spmi-mtk-pmif.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
