<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the Linux Bluetooth HCI device drivers.
#

<<<<<<< HEAD
obj-$(CONFIG_BT_HCISMD)		+= hci_smd.o
obj-$(CONFIG_BT_HCIVHCI)	+= hci_vhci.o
obj-$(CONFIG_BT_HCIUART)	+= hci_uart.o
obj-$(CONFIG_BT_HCIBCM203X)	+= bcm203x.o
=======
obj-$(CONFIG_BT_HCIVHCI)	+= hci_vhci.o
obj-$(CONFIG_BT_HCIUART)	+= hci_uart.o
obj-$(CONFIG_BT_HCIBCM203X)	+= bcm203x.o
obj-$(CONFIG_BT_HCIBCM4377)	+= hci_bcm4377.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-$(CONFIG_BT_HCIBPA10X)	+= bpa10x.o
obj-$(CONFIG_BT_HCIBFUSB)	+= bfusb.o
obj-$(CONFIG_BT_HCIDTL1)	+= dtl1_cs.o
obj-$(CONFIG_BT_HCIBT3C)	+= bt3c_cs.o
obj-$(CONFIG_BT_HCIBLUECARD)	+= bluecard_cs.o
<<<<<<< HEAD
obj-$(CONFIG_BT_HCIBTUART)	+= btuart_cs.o
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

obj-$(CONFIG_BT_HCIBTUSB)	+= btusb.o
obj-$(CONFIG_BT_HCIBTSDIO)	+= btsdio.o

<<<<<<< HEAD
obj-$(CONFIG_BT_ATH3K)		+= ath3k.o
obj-$(CONFIG_BT_MRVL)		+= btmrvl.o
obj-$(CONFIG_BT_MRVL_SDIO)	+= btmrvl_sdio.o
obj-$(CONFIG_BT_WILINK)		+= btwilink.o
=======
obj-$(CONFIG_BT_INTEL)		+= btintel.o
obj-$(CONFIG_BT_ATH3K)		+= ath3k.o
obj-$(CONFIG_BT_MRVL)		+= btmrvl.o
obj-$(CONFIG_BT_MRVL_SDIO)	+= btmrvl_sdio.o
obj-$(CONFIG_BT_MTKSDIO)	+= btmtksdio.o
obj-$(CONFIG_BT_MTKUART)	+= btmtkuart.o
obj-$(CONFIG_BT_QCOMSMD)	+= btqcomsmd.o
obj-$(CONFIG_BT_BCM)		+= btbcm.o
obj-$(CONFIG_BT_RTL)		+= btrtl.o
obj-$(CONFIG_BT_QCA)		+= btqca.o
obj-$(CONFIG_BT_MTK)		+= btmtk.o

obj-$(CONFIG_BT_VIRTIO)		+= virtio_bt.o
obj-$(CONFIG_BT_NXPUART)	+= btnxpuart.o

obj-$(CONFIG_BT_HCIUART_NOKIA)	+= hci_nokia.o

obj-$(CONFIG_BT_HCIRSI)		+= btrsi.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

btmrvl-y			:= btmrvl_main.o
btmrvl-$(CONFIG_DEBUG_FS)	+= btmrvl_debugfs.o

hci_uart-y				:= hci_ldisc.o
<<<<<<< HEAD
=======
hci_uart-$(CONFIG_BT_HCIUART_SERDEV)	+= hci_serdev.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
hci_uart-$(CONFIG_BT_HCIUART_H4)	+= hci_h4.o
hci_uart-$(CONFIG_BT_HCIUART_BCSP)	+= hci_bcsp.o
hci_uart-$(CONFIG_BT_HCIUART_LL)	+= hci_ll.o
hci_uart-$(CONFIG_BT_HCIUART_ATH3K)	+= hci_ath.o
<<<<<<< HEAD
hci_uart-$(CONFIG_BT_HCIUART_IBS)	+= hci_ibs.o
hci_uart-objs				:= $(hci_uart-y)
obj-$(CONFIG_BT_MSM_SLEEP)              += msm_bt_sleep.o
msm_bt_sleep-objs                       := bluesleep.o
obj-$(CONFIG_MSM_BT_POWER)		+= bluetooth-power.o
=======
hci_uart-$(CONFIG_BT_HCIUART_3WIRE)	+= hci_h5.o
hci_uart-$(CONFIG_BT_HCIUART_INTEL)	+= hci_intel.o
hci_uart-$(CONFIG_BT_HCIUART_BCM)	+= hci_bcm.o
hci_uart-$(CONFIG_BT_HCIUART_QCA)	+= hci_qca.o
hci_uart-$(CONFIG_BT_HCIUART_AG6XX)	+= hci_ag6xx.o
hci_uart-$(CONFIG_BT_HCIUART_MRVL)	+= hci_mrvl.o
hci_uart-objs				:= $(hci_uart-y)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
