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
 */

#include "main.h"
#include "gateway_common.h"
#include "gateway_client.h"

/* calculates the gateway class from kbit */
static void kbit_to_gw_bandwidth(int down, int up, long *gw_srv_class)
{
	int mdown = 0, tdown, tup, difference;
	uint8_t sbit, part;

	*gw_srv_class = 0;
	difference = 0x0FFFFFFF;

	/* test all downspeeds */
	for (sbit = 0; sbit < 2; sbit++) {
		for (part = 0; part < 16; part++) {
			tdown = 32 * (sbit + 2) * (1 << part);

			if (abs(tdown - down) < difference) {
				*gw_srv_class = (sbit << 7) + (part << 3);
				difference = abs(tdown - down);
				mdown = tdown;
			}
		}
	}

	/* test all upspeeds */
	difference = 0x0FFFFFFF;

	for (part = 0; part < 8; part++) {
		tup = ((part + 1) * (mdown)) / 8;

		if (abs(tup - up) < difference) {
			*gw_srv_class = (*gw_srv_class & 0xF8) | part;
			difference = abs(tup - up);
		}
	}
}

/* returns the up and downspeeds in kbit, calculated from the class */
void gw_bandwidth_to_kbit(uint8_t gw_srv_class, int *down, int *up)
{
	int sbit = (gw_srv_class & 0x80) >> 7;
	int dpart = (gw_srv_class & 0x78) >> 3;
	int upart = (gw_srv_class & 0x07);

	if (!gw_srv_class) {
		*down = 0;
		*up = 0;
		return;
	}

	*down = 32 * (sbit + 2) * (1 << dpart);
	*up = ((upart + 1) * (*down)) / 8;
}

static bool parse_gw_bandwidth(struct net_device *net_dev, char *buff,
			       int *up, int *down)
{
	int ret, multi = 1;
	char *slash_ptr, *tmp_ptr;
	long ldown, lup;

	slash_ptr = strchr(buff, '/');
	if (slash_ptr)
		*slash_ptr = 0;

	if (strlen(buff) > 4) {
		tmp_ptr = buff + strlen(buff) - 4;

		if (strnicmp(tmp_ptr, "mbit", 4) == 0)
			multi = 1024;

		if ((strnicmp(tmp_ptr, "kbit", 4) == 0) ||
		    (multi > 1))
			*tmp_ptr = '\0';
	}

	ret = kstrtol(buff, 10, &ldown);
	if (ret) {
		bat_err(net_dev,
			"Download speed of gateway mode invalid: %s\n",
			buff);
		return false;
	}

	*down = ldown * multi;

	/* we also got some upload info */
	if (slash_ptr) {
		multi = 1;

		if (strlen(slash_ptr + 1) > 4) {
			tmp_ptr = slash_ptr + 1 - 4 + strlen(slash_ptr + 1);

			if (strnicmp(tmp_ptr, "mbit", 4) == 0)
				multi = 1024;

			if ((strnicmp(tmp_ptr, "kbit", 4) == 0) ||
			    (multi > 1))
				*tmp_ptr = '\0';
		}

		ret = kstrtol(slash_ptr + 1, 10, &lup);
		if (ret) {
			bat_err(net_dev,
				"Upload speed of gateway mode invalid: %s\n",
				slash_ptr + 1);
			return false;
		}

		*up = lup * multi;
	}

	return true;
}

ssize_t gw_bandwidth_set(struct net_device *net_dev, char *buff, size_t count)
{
	struct bat_priv *bat_priv = netdev_priv(net_dev);
	long gw_bandwidth_tmp = 0;
	int up = 0, down = 0;
	bool ret;

	ret = parse_gw_bandwidth(net_dev, buff, &up, &down);
	if (!ret)
		goto end;

	if ((!down) || (down < 256))
		down = 2000;

	if (!up)
		up = down / 5;

	kbit_to_gw_bandwidth(down, up, &gw_bandwidth_tmp);

	/**
	 * the gw bandwidth we guessed above might not match the given
	 * speeds, hence we need to calculate it back to show the number
	 * that is going to be propagated
	 **/
	gw_bandwidth_to_kbit((uint8_t)gw_bandwidth_tmp, &down, &up);

	gw_deselect(bat_priv);
	bat_info(net_dev,
		 "Changing gateway bandwidth from: '%i' to: '%ld' (propagating: %d%s/%d%s)\n",
		 atomic_read(&bat_priv->gw_bandwidth), gw_bandwidth_tmp,
		 (down > 2048 ? down / 1024 : down),
		 (down > 2048 ? "MBit" : "KBit"),
		 (up > 2048 ? up / 1024 : up),
		 (up > 2048 ? "MBit" : "KBit"));

	atomic_set(&bat_priv->gw_bandwidth, gw_bandwidth_tmp);

end:
	return count;
=======
// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Marek Lindner
 */

#include "gateway_common.h"
#include "main.h"

#include <linux/atomic.h>
#include <linux/byteorder/generic.h>
#include <linux/stddef.h>
#include <linux/types.h>
#include <uapi/linux/batadv_packet.h>
#include <uapi/linux/batman_adv.h>

#include "gateway_client.h"
#include "tvlv.h"

/**
 * batadv_gw_tvlv_container_update() - update the gw tvlv container after
 *  gateway setting change
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_gw_tvlv_container_update(struct batadv_priv *bat_priv)
{
	struct batadv_tvlv_gateway_data gw;
	u32 down, up;
	char gw_mode;

	gw_mode = atomic_read(&bat_priv->gw.mode);

	switch (gw_mode) {
	case BATADV_GW_MODE_OFF:
	case BATADV_GW_MODE_CLIENT:
		batadv_tvlv_container_unregister(bat_priv, BATADV_TVLV_GW, 1);
		break;
	case BATADV_GW_MODE_SERVER:
		down = atomic_read(&bat_priv->gw.bandwidth_down);
		up = atomic_read(&bat_priv->gw.bandwidth_up);
		gw.bandwidth_down = htonl(down);
		gw.bandwidth_up = htonl(up);
		batadv_tvlv_container_register(bat_priv, BATADV_TVLV_GW, 1,
					       &gw, sizeof(gw));
		break;
	}
}

/**
 * batadv_gw_tvlv_ogm_handler_v1() - process incoming gateway tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @flags: flags indicating the tvlv state (see batadv_tvlv_handler_flags)
 * @tvlv_value: tvlv buffer containing the gateway data
 * @tvlv_value_len: tvlv buffer length
 */
static void batadv_gw_tvlv_ogm_handler_v1(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig,
					  u8 flags,
					  void *tvlv_value, u16 tvlv_value_len)
{
	struct batadv_tvlv_gateway_data gateway, *gateway_ptr;

	/* only fetch the tvlv value if the handler wasn't called via the
	 * CIFNOTFND flag and if there is data to fetch
	 */
	if (flags & BATADV_TVLV_HANDLER_OGM_CIFNOTFND ||
	    tvlv_value_len < sizeof(gateway)) {
		gateway.bandwidth_down = 0;
		gateway.bandwidth_up = 0;
	} else {
		gateway_ptr = tvlv_value;
		gateway.bandwidth_down = gateway_ptr->bandwidth_down;
		gateway.bandwidth_up = gateway_ptr->bandwidth_up;
		if (gateway.bandwidth_down == 0 ||
		    gateway.bandwidth_up == 0) {
			gateway.bandwidth_down = 0;
			gateway.bandwidth_up = 0;
		}
	}

	batadv_gw_node_update(bat_priv, orig, &gateway);

	/* restart gateway selection */
	if (gateway.bandwidth_down != 0 &&
	    atomic_read(&bat_priv->gw.mode) == BATADV_GW_MODE_CLIENT)
		batadv_gw_check_election(bat_priv, orig);
}

/**
 * batadv_gw_init() - initialise the gateway handling internals
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_gw_init(struct batadv_priv *bat_priv)
{
	if (bat_priv->algo_ops->gw.init_sel_class)
		bat_priv->algo_ops->gw.init_sel_class(bat_priv);
	else
		atomic_set(&bat_priv->gw.sel_class, 1);

	batadv_tvlv_handler_register(bat_priv, batadv_gw_tvlv_ogm_handler_v1,
				     NULL, NULL, BATADV_TVLV_GW, 1,
				     BATADV_TVLV_HANDLER_OGM_CIFNOTFND);
}

/**
 * batadv_gw_free() - free the gateway handling internals
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_gw_free(struct batadv_priv *bat_priv)
{
	batadv_tvlv_container_unregister(bat_priv, BATADV_TVLV_GW, 1);
	batadv_tvlv_handler_unregister(bat_priv, BATADV_TVLV_GW, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
