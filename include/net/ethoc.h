<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/include/net/ethoc.h
 *
 * Copyright (C) 2008-2009 Avionic Design GmbH
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Written by Thierry Reding <thierry.reding@avionic-design.de>
 */

#ifndef LINUX_NET_ETHOC_H
#define LINUX_NET_ETHOC_H 1

<<<<<<< HEAD
struct ethoc_platform_data {
	u8 hwaddr[IFHWADDRLEN];
	s8 phy_id;
};

#endif /* !LINUX_NET_ETHOC_H */

=======
#include <linux/if.h>
#include <linux/types.h>

struct ethoc_platform_data {
	u8 hwaddr[IFHWADDRLEN];
	s8 phy_id;
	u32 eth_clkfreq;
	bool big_endian;
};

#endif /* !LINUX_NET_ETHOC_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
