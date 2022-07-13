<<<<<<< HEAD
/*
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __DRIVERS_UMCAST_H
#define __DRIVERS_UMCAST_H

<<<<<<< HEAD
#include "net_user.h"
=======
#include <net_user.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct umcast_data {
	char *addr;
	unsigned short lport;
	unsigned short rport;
	void *listen_addr;
	void *remote_addr;
	int ttl;
	int unicast;
	void *dev;
};

extern const struct net_user_info umcast_user_info;

extern int umcast_user_write(int fd, void *buf, int len,
			     struct umcast_data *pri);

#endif
