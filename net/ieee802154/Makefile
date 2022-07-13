<<<<<<< HEAD
obj-$(CONFIG_IEEE802154) += ieee802154.o af_802154.o
obj-$(CONFIG_IEEE802154_6LOWPAN) += 6lowpan.o

ieee802154-y := netlink.o nl-mac.o nl-phy.o nl_policy.o wpan-class.o
af_802154-y := af_ieee802154.o raw.o dgram.o
=======
# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_IEEE802154) += ieee802154.o
obj-$(CONFIG_IEEE802154_SOCKET) += ieee802154_socket.o
obj-y += 6lowpan/

ieee802154-y := netlink.o nl-mac.o nl-phy.o nl_policy.o core.o \
                header_ops.o sysfs.o nl802154.o trace.o pan.o
ieee802154_socket-y := socket.o

CFLAGS_trace.o := -I$(src)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
