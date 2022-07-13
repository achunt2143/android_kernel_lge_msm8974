<<<<<<< HEAD
/*
 * Copyright (C) 2009-2012 B.A.T.M.A.N. contributors:
 *
 * Marek Lindner
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
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Marek Lindner
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _NET_BATMAN_ADV_GATEWAY_CLIENT_H_
#define _NET_BATMAN_ADV_GATEWAY_CLIENT_H_

<<<<<<< HEAD
void gw_deselect(struct bat_priv *bat_priv);
void gw_election(struct bat_priv *bat_priv);
struct orig_node *gw_get_selected_orig(struct bat_priv *bat_priv);
void gw_check_election(struct bat_priv *bat_priv, struct orig_node *orig_node);
void gw_node_update(struct bat_priv *bat_priv,
		    struct orig_node *orig_node, uint8_t new_gwflags);
void gw_node_delete(struct bat_priv *bat_priv, struct orig_node *orig_node);
void gw_node_purge(struct bat_priv *bat_priv);
int gw_client_seq_print_text(struct seq_file *seq, void *offset);
bool gw_is_dhcp_target(struct sk_buff *skb, unsigned int *header_len);
bool gw_out_of_range(struct bat_priv *bat_priv,
		     struct sk_buff *skb, struct ethhdr *ethhdr);
=======
#include "main.h"

#include <linux/kref.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <uapi/linux/batadv_packet.h>

void batadv_gw_check_client_stop(struct batadv_priv *bat_priv);
void batadv_gw_reselect(struct batadv_priv *bat_priv);
void batadv_gw_election(struct batadv_priv *bat_priv);
struct batadv_orig_node *
batadv_gw_get_selected_orig(struct batadv_priv *bat_priv);
void batadv_gw_check_election(struct batadv_priv *bat_priv,
			      struct batadv_orig_node *orig_node);
void batadv_gw_node_update(struct batadv_priv *bat_priv,
			   struct batadv_orig_node *orig_node,
			   struct batadv_tvlv_gateway_data *gateway);
void batadv_gw_node_delete(struct batadv_priv *bat_priv,
			   struct batadv_orig_node *orig_node);
void batadv_gw_node_free(struct batadv_priv *bat_priv);
void batadv_gw_node_release(struct kref *ref);
struct batadv_gw_node *
batadv_gw_get_selected_gw_node(struct batadv_priv *bat_priv);
int batadv_gw_dump(struct sk_buff *msg, struct netlink_callback *cb);
bool batadv_gw_out_of_range(struct batadv_priv *bat_priv, struct sk_buff *skb);
enum batadv_dhcp_recipient
batadv_gw_dhcp_recipient_get(struct sk_buff *skb, unsigned int *header_len,
			     u8 *chaddr);
struct batadv_gw_node *batadv_gw_node_get(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig_node);

/**
 * batadv_gw_node_put() - decrement the gw_node refcounter and possibly release
 *  it
 * @gw_node: gateway node to free
 */
static inline void batadv_gw_node_put(struct batadv_gw_node *gw_node)
{
	if (!gw_node)
		return;

	kref_put(&gw_node->refcount, batadv_gw_node_release);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _NET_BATMAN_ADV_GATEWAY_CLIENT_H_ */
