<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the memory technology device drivers.
#

# Core functionality.
obj-$(CONFIG_MTD)		+= mtd.o
<<<<<<< HEAD
mtd-y				:= mtdcore.o mtdsuper.o mtdconcat.o mtdpart.o

obj-$(CONFIG_MTD_OF_PARTS)	+= ofpart.o
obj-$(CONFIG_MTD_REDBOOT_PARTS) += redboot.o
obj-$(CONFIG_MTD_CMDLINE_PARTS) += cmdlinepart.o
obj-$(CONFIG_MTD_AFS_PARTS)	+= afs.o
obj-$(CONFIG_MTD_AR7_PARTS)	+= ar7part.o
obj-$(CONFIG_MTD_BCM63XX_PARTS)	+= bcm63xxpart.o

# 'Users' - code which presents functionality to userspace.
obj-$(CONFIG_MTD_CHAR)		+= mtdchar.o
=======
mtd-y				:= mtdcore.o mtdsuper.o mtdconcat.o mtdpart.o mtdchar.o

obj-y				+= parsers/

# 'Users' - code which presents functionality to userspace.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_MTD_BLKDEVS)	+= mtd_blkdevs.o
obj-$(CONFIG_MTD_BLOCK)		+= mtdblock.o
obj-$(CONFIG_MTD_BLOCK_RO)	+= mtdblock_ro.o
obj-$(CONFIG_FTL)		+= ftl.o
obj-$(CONFIG_NFTL)		+= nftl.o
obj-$(CONFIG_INFTL)		+= inftl.o
obj-$(CONFIG_RFD_FTL)		+= rfd_ftl.o
obj-$(CONFIG_SSFDC)		+= ssfdc.o
obj-$(CONFIG_SM_FTL)		+= sm_ftl.o
obj-$(CONFIG_MTD_OOPS)		+= mtdoops.o
<<<<<<< HEAD
=======
obj-$(CONFIG_MTD_PSTORE)	+= mtdpstore.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_MTD_SWAP)		+= mtdswap.o

nftl-objs		:= nftlcore.o nftlmount.o
inftl-objs		:= inftlcore.o inftlmount.o

<<<<<<< HEAD
obj-y		+= chips/ lpddr/ maps/ devices/ nand/ onenand/ tests/

obj-$(CONFIG_MTD_UBI)		+= ubi/
=======
obj-y		+= chips/ lpddr/ maps/ devices/ nand/ tests/

obj-$(CONFIG_MTD_SPI_NOR)	+= spi-nor/
obj-$(CONFIG_MTD_UBI)		+= ubi/
obj-$(CONFIG_MTD_HYPERBUS)	+= hyperbus/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
