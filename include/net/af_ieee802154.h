<<<<<<< HEAD
/*
 * IEEE 802.15.4 inteface for userspace
 *
 * Copyright 2007, 2008 Siemens AG
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * IEEE 802.15.4 interface for userspace
 *
 * Copyright 2007, 2008 Siemens AG
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Written by:
 * Sergey Lapin <slapin@ossfans.org>
 * Dmitry Eremin-Solenikov <dbaryshkov@gmail.com>
 */

#ifndef _AF_IEEE802154_H
#define _AF_IEEE802154_H

#include <linux/socket.h> /* for sa_family_t */

enum {
	IEEE802154_ADDR_NONE = 0x0,
	/* RESERVED = 0x01, */
	IEEE802154_ADDR_SHORT = 0x2, /* 16-bit address + PANid */
	IEEE802154_ADDR_LONG = 0x3, /* 64-bit address + PANid */
};

/* address length, octets */
#define IEEE802154_ADDR_LEN	8

<<<<<<< HEAD
struct ieee802154_addr {
=======
struct ieee802154_addr_sa {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int addr_type;
	u16 pan_id;
	union {
		u8 hwaddr[IEEE802154_ADDR_LEN];
		u16 short_addr;
	};
};

#define IEEE802154_PANID_BROADCAST	0xffff
#define IEEE802154_ADDR_BROADCAST	0xffff
#define IEEE802154_ADDR_UNDEF		0xfffe

struct sockaddr_ieee802154 {
	sa_family_t family; /* AF_IEEE802154 */
<<<<<<< HEAD
	struct ieee802154_addr addr;
=======
	struct ieee802154_addr_sa addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* get/setsockopt */
#define SOL_IEEE802154	0

<<<<<<< HEAD
#define WPAN_WANTACK	0
=======
#define WPAN_WANTACK		0
#define WPAN_SECURITY		1
#define WPAN_SECURITY_LEVEL	2
#define WPAN_WANTLQI		3

#define WPAN_SECURITY_DEFAULT	0
#define WPAN_SECURITY_OFF	1
#define WPAN_SECURITY_ON	2

#define WPAN_SECURITY_LEVEL_DEFAULT	(-1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
