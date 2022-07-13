<<<<<<< HEAD
/*
 * Copyright (c) 2007-2011 Nicira Networks.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2007-2011 Nicira, Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef VPORT_INTERNAL_DEV_H
#define VPORT_INTERNAL_DEV_H 1

#include "datapath.h"
#include "vport.h"

int ovs_is_internal_dev(const struct net_device *);
struct vport *ovs_internal_dev_get_vport(struct net_device *);
<<<<<<< HEAD
=======
int ovs_internal_dev_rtnl_link_register(void);
void ovs_internal_dev_rtnl_link_unregister(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* vport-internal_dev.h */
