<<<<<<< HEAD
/*
 * Netlink inteface for IEEE 802.15.4 stack
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
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Netlink interface for IEEE 802.15.4 stack
 *
 * Copyright 2007, 2008 Siemens AG
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Written by:
 * Sergey Lapin <slapin@ossfans.org>
 * Dmitry Eremin-Solenikov <dbaryshkov@gmail.com>
 * Maxim Osipov <maxim.osipov@siemens.com>
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/if_arp.h>
#include <net/netlink.h>
#include <net/genetlink.h>
<<<<<<< HEAD
#include <net/wpan-phy.h>
=======
#include <net/cfg802154.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/af_ieee802154.h>
#include <net/ieee802154_netdev.h>
#include <net/rtnetlink.h> /* for rtnl_{un,}lock */
#include <linux/nl802154.h>

#include "ieee802154.h"
<<<<<<< HEAD

static int ieee802154_nl_fill_phy(struct sk_buff *msg, u32 pid,
	u32 seq, int flags, struct wpan_phy *phy)
{
	void *hdr;
	int i, pages = 0;
	uint32_t *buf = kzalloc(32 * sizeof(uint32_t), GFP_KERNEL);
=======
#include "rdev-ops.h"
#include "core.h"

static int ieee802154_nl_fill_phy(struct sk_buff *msg, u32 portid,
				  u32 seq, int flags, struct wpan_phy *phy)
{
	void *hdr;
	int i, pages = 0;
	u32 *buf = kcalloc(IEEE802154_MAX_PAGE + 1, sizeof(u32), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_debug("%s\n", __func__);

	if (!buf)
		return -EMSGSIZE;

	hdr = genlmsg_put(msg, 0, seq, &nl802154_family, flags,
<<<<<<< HEAD
		IEEE802154_LIST_PHY);
	if (!hdr)
		goto out;

	mutex_lock(&phy->pib_lock);
	NLA_PUT_STRING(msg, IEEE802154_ATTR_PHY_NAME, wpan_phy_name(phy));

	NLA_PUT_U8(msg, IEEE802154_ATTR_PAGE, phy->current_page);
	NLA_PUT_U8(msg, IEEE802154_ATTR_CHANNEL, phy->current_channel);
	for (i = 0; i < 32; i++) {
		if (phy->channels_supported[i])
			buf[pages++] = phy->channels_supported[i] | (i << 27);
	}
	if (pages)
		NLA_PUT(msg, IEEE802154_ATTR_CHANNEL_PAGE_LIST,
				pages * sizeof(uint32_t), buf);

	mutex_unlock(&phy->pib_lock);
	kfree(buf);
	return genlmsg_end(msg, hdr);

nla_put_failure:
	mutex_unlock(&phy->pib_lock);
=======
			  IEEE802154_LIST_PHY);
	if (!hdr)
		goto out;

	rtnl_lock();
	if (nla_put_string(msg, IEEE802154_ATTR_PHY_NAME, wpan_phy_name(phy)) ||
	    nla_put_u8(msg, IEEE802154_ATTR_PAGE, phy->current_page) ||
	    nla_put_u8(msg, IEEE802154_ATTR_CHANNEL, phy->current_channel))
		goto nla_put_failure;
	for (i = 0; i <= IEEE802154_MAX_PAGE; i++) {
		if (phy->supported.channels[i])
			buf[pages++] = phy->supported.channels[i] | (i << 27);
	}
	if (pages &&
	    nla_put(msg, IEEE802154_ATTR_CHANNEL_PAGE_LIST,
		    pages * sizeof(uint32_t), buf))
		goto nla_put_failure;
	rtnl_unlock();
	kfree(buf);
	genlmsg_end(msg, hdr);
	return 0;

nla_put_failure:
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	genlmsg_cancel(msg, hdr);
out:
	kfree(buf);
	return -EMSGSIZE;
}

<<<<<<< HEAD
static int ieee802154_list_phy(struct sk_buff *skb,
	struct genl_info *info)
{
	/* Request for interface name, index, type, IEEE address,
	   PAN Id, short address */
=======
int ieee802154_list_phy(struct sk_buff *skb, struct genl_info *info)
{
	/* Request for interface name, index, type, IEEE address,
	 * PAN Id, short address
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sk_buff *msg;
	struct wpan_phy *phy;
	const char *name;
	int rc = -ENOBUFS;

	pr_debug("%s\n", __func__);

	if (!info->attrs[IEEE802154_ATTR_PHY_NAME])
		return -EINVAL;

	name = nla_data(info->attrs[IEEE802154_ATTR_PHY_NAME]);
	if (name[nla_len(info->attrs[IEEE802154_ATTR_PHY_NAME]) - 1] != '\0')
		return -EINVAL; /* phy name should be null-terminated */

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	phy = wpan_phy_find(name);
	if (!phy)
		return -ENODEV;

<<<<<<< HEAD
	msg = nlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!msg)
		goto out_dev;

	rc = ieee802154_nl_fill_phy(msg, info->snd_pid, info->snd_seq,
			0, phy);
=======
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		goto out_dev;

	rc = ieee802154_nl_fill_phy(msg, info->snd_portid, info->snd_seq,
				    0, phy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc < 0)
		goto out_free;

	wpan_phy_put(phy);

	return genlmsg_reply(msg, info);
out_free:
	nlmsg_free(msg);
out_dev:
	wpan_phy_put(phy);
	return rc;
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct dump_phy_data {
	struct sk_buff *skb;
	struct netlink_callback *cb;
	int idx, s_idx;
};

static int ieee802154_dump_phy_iter(struct wpan_phy *phy, void *_data)
{
	int rc;
	struct dump_phy_data *data = _data;

	pr_debug("%s\n", __func__);

	if (data->idx++ < data->s_idx)
		return 0;

	rc = ieee802154_nl_fill_phy(data->skb,
<<<<<<< HEAD
			NETLINK_CB(data->cb->skb).pid,
			data->cb->nlh->nlmsg_seq,
			NLM_F_MULTI,
			phy);
=======
				    NETLINK_CB(data->cb->skb).portid,
				    data->cb->nlh->nlmsg_seq,
				    NLM_F_MULTI,
				    phy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (rc < 0) {
		data->idx--;
		return rc;
	}

	return 0;
}

<<<<<<< HEAD
static int ieee802154_dump_phy(struct sk_buff *skb,
	struct netlink_callback *cb)
=======
int ieee802154_dump_phy(struct sk_buff *skb, struct netlink_callback *cb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dump_phy_data data = {
		.cb = cb,
		.skb = skb,
		.s_idx = cb->args[0],
		.idx = 0,
	};

	pr_debug("%s\n", __func__);

	wpan_phy_for_each(ieee802154_dump_phy_iter, &data);

	cb->args[0] = data.idx;

	return skb->len;
}

<<<<<<< HEAD
static int ieee802154_add_iface(struct sk_buff *skb,
		struct genl_info *info)
=======
int ieee802154_add_iface(struct sk_buff *skb, struct genl_info *info)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *msg;
	struct wpan_phy *phy;
	const char *name;
	const char *devname;
	int rc = -ENOBUFS;
	struct net_device *dev;
<<<<<<< HEAD
=======
	int type = __IEEE802154_DEV_INVALID;
	unsigned char name_assign_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_debug("%s\n", __func__);

	if (!info->attrs[IEEE802154_ATTR_PHY_NAME])
		return -EINVAL;

	name = nla_data(info->attrs[IEEE802154_ATTR_PHY_NAME]);
	if (name[nla_len(info->attrs[IEEE802154_ATTR_PHY_NAME]) - 1] != '\0')
		return -EINVAL; /* phy name should be null-terminated */

	if (info->attrs[IEEE802154_ATTR_DEV_NAME]) {
		devname = nla_data(info->attrs[IEEE802154_ATTR_DEV_NAME]);
		if (devname[nla_len(info->attrs[IEEE802154_ATTR_DEV_NAME]) - 1]
				!= '\0')
			return -EINVAL; /* phy name should be null-terminated */
<<<<<<< HEAD
	} else  {
		devname = "wpan%d";
=======
		name_assign_type = NET_NAME_USER;
	} else  {
		devname = "wpan%d";
		name_assign_type = NET_NAME_ENUM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (strlen(devname) >= IFNAMSIZ)
		return -ENAMETOOLONG;

	phy = wpan_phy_find(name);
	if (!phy)
		return -ENODEV;

	msg = ieee802154_nl_new_reply(info, 0, IEEE802154_ADD_IFACE);
	if (!msg)
		goto out_dev;

<<<<<<< HEAD
	if (!phy->add_iface) {
		rc = -EINVAL;
		goto nla_put_failure;
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (info->attrs[IEEE802154_ATTR_HW_ADDR] &&
	    nla_len(info->attrs[IEEE802154_ATTR_HW_ADDR]) !=
			IEEE802154_ADDR_LEN) {
		rc = -EINVAL;
		goto nla_put_failure;
	}

<<<<<<< HEAD
	dev = phy->add_iface(phy, devname);
=======
	if (info->attrs[IEEE802154_ATTR_DEV_TYPE]) {
		type = nla_get_u8(info->attrs[IEEE802154_ATTR_DEV_TYPE]);
		if (type >= __IEEE802154_DEV_MAX) {
			rc = -EINVAL;
			goto nla_put_failure;
		}
	}

	dev = rdev_add_virtual_intf_deprecated(wpan_phy_to_rdev(phy), devname,
					       name_assign_type, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(dev)) {
		rc = PTR_ERR(dev);
		goto nla_put_failure;
	}
<<<<<<< HEAD
=======
	dev_hold(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (info->attrs[IEEE802154_ATTR_HW_ADDR]) {
		struct sockaddr addr;

		addr.sa_family = ARPHRD_IEEE802154;
		nla_memcpy(&addr.sa_data, info->attrs[IEEE802154_ATTR_HW_ADDR],
<<<<<<< HEAD
				IEEE802154_ADDR_LEN);

		/*
		 * strangely enough, some callbacks (inetdev_event) from
		 * dev_set_mac_address require RTNL_LOCK
		 */
		rtnl_lock();
		rc = dev_set_mac_address(dev, &addr);
=======
			   IEEE802154_ADDR_LEN);

		/* strangely enough, some callbacks (inetdev_event) from
		 * dev_set_mac_address require RTNL_LOCK
		 */
		rtnl_lock();
		rc = dev_set_mac_address(dev, &addr, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rtnl_unlock();
		if (rc)
			goto dev_unregister;
	}

<<<<<<< HEAD
	NLA_PUT_STRING(msg, IEEE802154_ATTR_PHY_NAME, wpan_phy_name(phy));
	NLA_PUT_STRING(msg, IEEE802154_ATTR_DEV_NAME, dev->name);

=======
	if (nla_put_string(msg, IEEE802154_ATTR_PHY_NAME, wpan_phy_name(phy)) ||
	    nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name)) {
		rc = -EMSGSIZE;
		goto nla_put_failure;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev_put(dev);

	wpan_phy_put(phy);

	return ieee802154_nl_reply(msg, info);

dev_unregister:
	rtnl_lock(); /* del_iface must be called with RTNL lock */
<<<<<<< HEAD
	phy->del_iface(phy, dev);
=======
	rdev_del_virtual_intf_deprecated(wpan_phy_to_rdev(phy), dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev_put(dev);
	rtnl_unlock();
nla_put_failure:
	nlmsg_free(msg);
out_dev:
	wpan_phy_put(phy);
	return rc;
}

<<<<<<< HEAD
static int ieee802154_del_iface(struct sk_buff *skb,
		struct genl_info *info)
=======
int ieee802154_del_iface(struct sk_buff *skb, struct genl_info *info)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *msg;
	struct wpan_phy *phy;
	const char *name;
	int rc;
	struct net_device *dev;

	pr_debug("%s\n", __func__);

	if (!info->attrs[IEEE802154_ATTR_DEV_NAME])
		return -EINVAL;

	name = nla_data(info->attrs[IEEE802154_ATTR_DEV_NAME]);
	if (name[nla_len(info->attrs[IEEE802154_ATTR_DEV_NAME]) - 1] != '\0')
		return -EINVAL; /* name should be null-terminated */

<<<<<<< HEAD
	dev = dev_get_by_name(genl_info_net(info), name);
	if (!dev)
		return -ENODEV;

	phy = ieee802154_mlme_ops(dev)->get_phy(dev);
	BUG_ON(!phy);
=======
	rc = -ENODEV;
	dev = dev_get_by_name(genl_info_net(info), name);
	if (!dev)
		return rc;
	if (dev->type != ARPHRD_IEEE802154)
		goto out;

	phy = dev->ieee802154_ptr->wpan_phy;
	BUG_ON(!phy);
	get_device(&phy->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rc = -EINVAL;
	/* phy name is optional, but should be checked if it's given */
	if (info->attrs[IEEE802154_ATTR_PHY_NAME]) {
		struct wpan_phy *phy2;

		const char *pname =
			nla_data(info->attrs[IEEE802154_ATTR_PHY_NAME]);
		if (pname[nla_len(info->attrs[IEEE802154_ATTR_PHY_NAME]) - 1]
				!= '\0')
			/* name should be null-terminated */
			goto out_dev;

		phy2 = wpan_phy_find(pname);
		if (!phy2)
			goto out_dev;

		if (phy != phy2) {
			wpan_phy_put(phy2);
			goto out_dev;
		}
	}

	rc = -ENOBUFS;

	msg = ieee802154_nl_new_reply(info, 0, IEEE802154_DEL_IFACE);
	if (!msg)
		goto out_dev;

<<<<<<< HEAD
	if (!phy->del_iface) {
		rc = -EINVAL;
		goto nla_put_failure;
	}

	rtnl_lock();
	phy->del_iface(phy, dev);
=======
	rtnl_lock();
	rdev_del_virtual_intf_deprecated(wpan_phy_to_rdev(phy), dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We don't have device anymore */
	dev_put(dev);
	dev = NULL;

	rtnl_unlock();

<<<<<<< HEAD

	NLA_PUT_STRING(msg, IEEE802154_ATTR_PHY_NAME, wpan_phy_name(phy));
	NLA_PUT_STRING(msg, IEEE802154_ATTR_DEV_NAME, name);

=======
	if (nla_put_string(msg, IEEE802154_ATTR_PHY_NAME, wpan_phy_name(phy)) ||
	    nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, name))
		goto nla_put_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wpan_phy_put(phy);

	return ieee802154_nl_reply(msg, info);

nla_put_failure:
	nlmsg_free(msg);
out_dev:
	wpan_phy_put(phy);
<<<<<<< HEAD
	if (dev)
		dev_put(dev);

	return rc;
}

static struct genl_ops ieee802154_phy_ops[] = {
	IEEE802154_DUMP(IEEE802154_LIST_PHY, ieee802154_list_phy,
							ieee802154_dump_phy),
	IEEE802154_OP(IEEE802154_ADD_IFACE, ieee802154_add_iface),
	IEEE802154_OP(IEEE802154_DEL_IFACE, ieee802154_del_iface),
};

/*
 * No need to unregister as family unregistration will do it.
 */
int nl802154_phy_register(void)
{
	int i;
	int rc;

	for (i = 0; i < ARRAY_SIZE(ieee802154_phy_ops); i++) {
		rc = genl_register_ops(&nl802154_family,
				&ieee802154_phy_ops[i]);
		if (rc)
			return rc;
	}

	return 0;
}
=======
out:
	dev_put(dev);

	return rc;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
