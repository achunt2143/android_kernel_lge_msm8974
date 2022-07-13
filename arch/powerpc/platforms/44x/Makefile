<<<<<<< HEAD
obj-$(CONFIG_44x)	+= misc_44x.o
ifneq ($(CONFIG_PPC4xx_CPM),y)
obj-$(CONFIG_44x)	+= idle.o
=======
# SPDX-License-Identifier: GPL-2.0
obj-y	+= misc_44x.o machine_check.o
ifneq ($(CONFIG_PPC4xx_CPM),y)
obj-y	+= idle.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
endif
obj-$(CONFIG_PPC44x_SIMPLE) += ppc44x_simple.o
obj-$(CONFIG_EBONY)	+= ebony.o
obj-$(CONFIG_SAM440EP) 	+= sam440ep.o
obj-$(CONFIG_WARP)	+= warp.o
<<<<<<< HEAD
obj-$(CONFIG_XILINX_VIRTEX_5_FXT) += virtex.o
obj-$(CONFIG_XILINX_ML510) += virtex_ml510.o
obj-$(CONFIG_ISS4xx)	+= iss4xx.o
obj-$(CONFIG_CANYONLANDS)+= canyonlands.o
obj-$(CONFIG_CURRITUCK)	+= currituck.o
=======
obj-$(CONFIG_ISS4xx)	+= iss4xx.o
obj-$(CONFIG_CANYONLANDS)+= canyonlands.o
obj-$(CONFIG_CURRITUCK)	+= ppc476.o
obj-$(CONFIG_AKEBONO)	+= ppc476.o
obj-$(CONFIG_FSP2)	+= fsp2.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
