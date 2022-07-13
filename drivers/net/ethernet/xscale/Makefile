<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Intel XScale IXP device drivers.
#

<<<<<<< HEAD
obj-$(CONFIG_ENP2611_MSF_NET) += ixp2000/
obj-$(CONFIG_IXP4XX_ETH) += ixp4xx_eth.o
=======
# Keep this link order to avoid deferred probing
ifdef CONFIG_PTP_1588_CLOCK_IXP46X
obj-$(CONFIG_IXP4XX_ETH)		+= ptp_ixp46x.o
endif

obj-$(CONFIG_IXP4XX_ETH)		+= ixp4xx_eth.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
