<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * cfg80211 wext compat for managed mode.
 *
 * Copyright 2009	Johannes Berg <johannes@sipsolutions.net>
<<<<<<< HEAD
 * Copyright (C) 2009   Intel Corporation. All rights reserved.
=======
 * Copyright (C) 2009, 2020-2023 Intel Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/export.h>
#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/slab.h>
#include <net/cfg80211.h>
#include <net/cfg80211-wext.h>
#include "wext-compat.h"
#include "nl80211.h"

int cfg80211_mgd_wext_connect(struct cfg80211_registered_device *rdev,
			      struct wireless_dev *wdev)
{
	struct cfg80211_cached_keys *ck = NULL;
	const u8 *prev_bssid = NULL;
	int err, i;

<<<<<<< HEAD
	ASSERT_RDEV_LOCK(rdev);
	ASSERT_WDEV_LOCK(wdev);
=======
	ASSERT_RTNL();
	lockdep_assert_wiphy(wdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!netif_running(wdev->netdev))
		return 0;

	wdev->wext.connect.ie = wdev->wext.ie;
	wdev->wext.connect.ie_len = wdev->wext.ie_len;

	/* Use default background scan period */
	wdev->wext.connect.bg_scan_period = -1;

	if (wdev->wext.keys) {
		wdev->wext.keys->def = wdev->wext.default_key;
<<<<<<< HEAD
		wdev->wext.keys->defmgmt = wdev->wext.default_mgmt_key;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (wdev->wext.default_key != -1)
			wdev->wext.connect.privacy = true;
	}

	if (!wdev->wext.connect.ssid_len)
		return 0;

<<<<<<< HEAD
	if (wdev->wext.keys) {
		ck = kmemdup(wdev->wext.keys, sizeof(*ck), GFP_KERNEL);
		if (!ck)
			return -ENOMEM;
		for (i = 0; i < 6; i++)
=======
	if (wdev->wext.keys && wdev->wext.keys->def != -1) {
		ck = kmemdup(wdev->wext.keys, sizeof(*ck), GFP_KERNEL);
		if (!ck)
			return -ENOMEM;
		for (i = 0; i < 4; i++)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ck->params[i].key = ck->data[i];
	}

	if (wdev->wext.prev_bssid_valid)
		prev_bssid = wdev->wext.prev_bssid;

<<<<<<< HEAD
	err = __cfg80211_connect(rdev, wdev->netdev,
				 &wdev->wext.connect, ck, prev_bssid);
	if (err)
		kfree(ck);
=======
	err = cfg80211_connect(rdev, wdev->netdev,
			       &wdev->wext.connect, ck, prev_bssid);
	if (err)
		kfree_sensitive(ck);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

int cfg80211_mgd_wext_siwfreq(struct net_device *dev,
			      struct iw_request_info *info,
			      struct iw_freq *wextfreq, char *extra)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
=======
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ieee80211_channel *chan = NULL;
	int err, freq;

	/* call only for station! */
	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION))
		return -EINVAL;

<<<<<<< HEAD
	freq = cfg80211_wext_freq(wdev->wiphy, wextfreq);
=======
	freq = cfg80211_wext_freq(wextfreq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (freq < 0)
		return freq;

	if (freq) {
		chan = ieee80211_get_channel(wdev->wiphy, freq);
		if (!chan)
			return -EINVAL;
		if (chan->flags & IEEE80211_CHAN_DISABLED)
			return -EINVAL;
	}

<<<<<<< HEAD
	cfg80211_lock_rdev(rdev);
	mutex_lock(&rdev->devlist_mtx);
	wdev_lock(wdev);

	if (wdev->sme_state != CFG80211_SME_IDLE) {
		bool event = true;

		if (wdev->wext.connect.channel == chan) {
			err = 0;
			goto out;
		}
=======
	if (wdev->conn) {
		bool event = true;

		if (wdev->wext.connect.channel == chan)
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* if SSID set, we'll try right again, avoid event */
		if (wdev->wext.connect.ssid_len)
			event = false;
<<<<<<< HEAD
		err = __cfg80211_disconnect(rdev, dev,
					    WLAN_REASON_DEAUTH_LEAVING, event);
		if (err)
			goto out;
	}


	wdev->wext.connect.channel = chan;

	/* SSID is not set, we just want to switch channel */
	if (chan && !wdev->wext.connect.ssid_len) {
		err = cfg80211_set_freq(rdev, wdev, freq, NL80211_CHAN_NO_HT);
		goto out;
	}

	err = cfg80211_mgd_wext_connect(rdev, wdev);
 out:
	wdev_unlock(wdev);
	mutex_unlock(&rdev->devlist_mtx);
	cfg80211_unlock_rdev(rdev);
	return err;
=======
		err = cfg80211_disconnect(rdev, dev,
					  WLAN_REASON_DEAUTH_LEAVING, event);
		if (err)
			return err;
	}

	wdev->wext.connect.channel = chan;
	return cfg80211_mgd_wext_connect(rdev, wdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int cfg80211_mgd_wext_giwfreq(struct net_device *dev,
			      struct iw_request_info *info,
			      struct iw_freq *freq, char *extra)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct ieee80211_channel *chan = NULL;

	/* call only for station! */
	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION))
		return -EINVAL;

<<<<<<< HEAD
	wdev_lock(wdev);
	if (wdev->current_bss)
		chan = wdev->current_bss->pub.channel;
	else if (wdev->wext.connect.channel)
		chan = wdev->wext.connect.channel;
	wdev_unlock(wdev);
=======
	if (wdev->valid_links)
		return -EOPNOTSUPP;

	if (wdev->links[0].client.current_bss)
		chan = wdev->links[0].client.current_bss->pub.channel;
	else if (wdev->wext.connect.channel)
		chan = wdev->wext.connect.channel;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (chan) {
		freq->m = chan->center_freq;
		freq->e = 6;
		return 0;
	}

	/* no channel if not joining */
	return -EINVAL;
}

int cfg80211_mgd_wext_siwessid(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_point *data, char *ssid)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
=======
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	size_t len = data->length;
	int err;

	/* call only for station! */
	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION))
		return -EINVAL;

	if (!data->flags)
		len = 0;

	/* iwconfig uses nul termination in SSID.. */
	if (len > 0 && ssid[len - 1] == '\0')
		len--;

<<<<<<< HEAD
	cfg80211_lock_rdev(rdev);
	mutex_lock(&rdev->devlist_mtx);
	wdev_lock(wdev);

	err = 0;

	if (wdev->sme_state != CFG80211_SME_IDLE) {
=======
	if (wdev->conn) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bool event = true;

		if (wdev->wext.connect.ssid && len &&
		    len == wdev->wext.connect.ssid_len &&
		    memcmp(wdev->wext.connect.ssid, ssid, len) == 0)
<<<<<<< HEAD
			goto out;
=======
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* if SSID set now, we'll try to connect, avoid event */
		if (len)
			event = false;
<<<<<<< HEAD
		err = __cfg80211_disconnect(rdev, dev,
					    WLAN_REASON_DEAUTH_LEAVING, event);
		if (err)
			goto out;
=======
		err = cfg80211_disconnect(rdev, dev,
					  WLAN_REASON_DEAUTH_LEAVING, event);
		if (err)
			return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	wdev->wext.prev_bssid_valid = false;
	wdev->wext.connect.ssid = wdev->wext.ssid;
	memcpy(wdev->wext.ssid, ssid, len);
	wdev->wext.connect.ssid_len = len;

	wdev->wext.connect.crypto.control_port = false;
	wdev->wext.connect.crypto.control_port_ethertype =
					cpu_to_be16(ETH_P_PAE);

<<<<<<< HEAD
	err = cfg80211_mgd_wext_connect(rdev, wdev);
 out:
	wdev_unlock(wdev);
	mutex_unlock(&rdev->devlist_mtx);
	cfg80211_unlock_rdev(rdev);
	return err;
=======
	return cfg80211_mgd_wext_connect(rdev, wdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int cfg80211_mgd_wext_giwessid(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_point *data, char *ssid)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
=======
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* call only for station! */
	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION))
		return -EINVAL;

<<<<<<< HEAD
	data->flags = 0;

	wdev_lock(wdev);
	if (wdev->current_bss) {
		const u8 *ie = ieee80211_bss_get_ie(&wdev->current_bss->pub,
						    WLAN_EID_SSID);
		if (ie) {
			data->flags = 1;
			data->length = ie[1];
			memcpy(ssid, ie + 2, data->length);
		}
=======
	if (wdev->valid_links)
		return -EINVAL;

	data->flags = 0;

	if (wdev->links[0].client.current_bss) {
		const struct element *ssid_elem;

		rcu_read_lock();
		ssid_elem = ieee80211_bss_get_elem(
				&wdev->links[0].client.current_bss->pub,
				WLAN_EID_SSID);
		if (ssid_elem) {
			data->flags = 1;
			data->length = ssid_elem->datalen;
			if (data->length > IW_ESSID_MAX_SIZE)
				ret = -EINVAL;
			else
				memcpy(ssid, ssid_elem->data, data->length);
		}
		rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (wdev->wext.connect.ssid && wdev->wext.connect.ssid_len) {
		data->flags = 1;
		data->length = wdev->wext.connect.ssid_len;
		memcpy(ssid, wdev->wext.connect.ssid, data->length);
	}
<<<<<<< HEAD
	wdev_unlock(wdev);

	return 0;
=======

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int cfg80211_mgd_wext_siwap(struct net_device *dev,
			    struct iw_request_info *info,
			    struct sockaddr *ap_addr, char *extra)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
=======
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *bssid = ap_addr->sa_data;
	int err;

	/* call only for station! */
	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION))
		return -EINVAL;

	if (ap_addr->sa_family != ARPHRD_ETHER)
		return -EINVAL;

	/* automatic mode */
	if (is_zero_ether_addr(bssid) || is_broadcast_ether_addr(bssid))
		bssid = NULL;

<<<<<<< HEAD
	cfg80211_lock_rdev(rdev);
	mutex_lock(&rdev->devlist_mtx);
	wdev_lock(wdev);

	if (wdev->sme_state != CFG80211_SME_IDLE) {
		err = 0;
		/* both automatic */
		if (!bssid && !wdev->wext.connect.bssid)
			goto out;

		/* fixed already - and no change */
		if (wdev->wext.connect.bssid && bssid &&
		    compare_ether_addr(bssid, wdev->wext.connect.bssid) == 0)
			goto out;

		err = __cfg80211_disconnect(rdev, dev,
					    WLAN_REASON_DEAUTH_LEAVING, false);
		if (err)
			goto out;
=======
	if (wdev->conn) {
		/* both automatic */
		if (!bssid && !wdev->wext.connect.bssid)
			return 0;

		/* fixed already - and no change */
		if (wdev->wext.connect.bssid && bssid &&
		    ether_addr_equal(bssid, wdev->wext.connect.bssid))
			return 0;

		err = cfg80211_disconnect(rdev, dev,
					  WLAN_REASON_DEAUTH_LEAVING, false);
		if (err)
			return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (bssid) {
		memcpy(wdev->wext.bssid, bssid, ETH_ALEN);
		wdev->wext.connect.bssid = wdev->wext.bssid;
	} else
		wdev->wext.connect.bssid = NULL;

<<<<<<< HEAD
	err = cfg80211_mgd_wext_connect(rdev, wdev);
 out:
	wdev_unlock(wdev);
	mutex_unlock(&rdev->devlist_mtx);
	cfg80211_unlock_rdev(rdev);
	return err;
=======
	return cfg80211_mgd_wext_connect(rdev, wdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int cfg80211_mgd_wext_giwap(struct net_device *dev,
			    struct iw_request_info *info,
			    struct sockaddr *ap_addr, char *extra)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;

	/* call only for station! */
	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION))
		return -EINVAL;

	ap_addr->sa_family = ARPHRD_ETHER;

<<<<<<< HEAD
	wdev_lock(wdev);
	if (wdev->current_bss)
		memcpy(ap_addr->sa_data, wdev->current_bss->pub.bssid, ETH_ALEN);
	else
		memset(ap_addr->sa_data, 0, ETH_ALEN);
	wdev_unlock(wdev);
=======
	if (wdev->valid_links)
		return -EOPNOTSUPP;

	if (wdev->links[0].client.current_bss)
		memcpy(ap_addr->sa_data,
		       wdev->links[0].client.current_bss->pub.bssid,
		       ETH_ALEN);
	else
		eth_zero_addr(ap_addr->sa_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

int cfg80211_wext_siwgenie(struct net_device *dev,
			   struct iw_request_info *info,
<<<<<<< HEAD
			   struct iw_point *data, char *extra)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
=======
			   union iwreq_data *wrqu, char *extra)
{
	struct iw_point *data = &wrqu->data;
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *ie = extra;
	int ie_len = data->length, err;

	if (wdev->iftype != NL80211_IFTYPE_STATION)
		return -EOPNOTSUPP;

	if (!ie_len)
		ie = NULL;

<<<<<<< HEAD
	wdev_lock(wdev);
=======
	wiphy_lock(wdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* no change */
	err = 0;
	if (wdev->wext.ie_len == ie_len &&
	    memcmp(wdev->wext.ie, ie, ie_len) == 0)
		goto out;

	if (ie_len) {
		ie = kmemdup(extra, ie_len, GFP_KERNEL);
		if (!ie) {
			err = -ENOMEM;
			goto out;
		}
	} else
		ie = NULL;

	kfree(wdev->wext.ie);
	wdev->wext.ie = ie;
	wdev->wext.ie_len = ie_len;

<<<<<<< HEAD
	if (wdev->sme_state != CFG80211_SME_IDLE) {
		err = __cfg80211_disconnect(rdev, dev,
					    WLAN_REASON_DEAUTH_LEAVING, false);
=======
	if (wdev->conn) {
		err = cfg80211_disconnect(rdev, dev,
					  WLAN_REASON_DEAUTH_LEAVING, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			goto out;
	}

	/* userspace better not think we'll reconnect */
	err = 0;
 out:
<<<<<<< HEAD
	wdev_unlock(wdev);
=======
	wiphy_unlock(wdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

int cfg80211_wext_siwmlme(struct net_device *dev,
			  struct iw_request_info *info,
<<<<<<< HEAD
			  struct iw_point *data, char *extra)
=======
			  union iwreq_data *wrqu, char *extra)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct iw_mlme *mlme = (struct iw_mlme *)extra;
	struct cfg80211_registered_device *rdev;
	int err;

	if (!wdev)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	rdev = wiphy_to_dev(wdev->wiphy);
=======
	rdev = wiphy_to_rdev(wdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (wdev->iftype != NL80211_IFTYPE_STATION)
		return -EINVAL;

	if (mlme->addr.sa_family != ARPHRD_ETHER)
		return -EINVAL;

<<<<<<< HEAD
	wdev_lock(wdev);
	switch (mlme->cmd) {
	case IW_MLME_DEAUTH:
	case IW_MLME_DISASSOC:
		err = __cfg80211_disconnect(rdev, dev, mlme->reason_code,
					    true);
=======
	wiphy_lock(&rdev->wiphy);
	switch (mlme->cmd) {
	case IW_MLME_DEAUTH:
	case IW_MLME_DISASSOC:
		err = cfg80211_disconnect(rdev, dev, mlme->reason_code, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		err = -EOPNOTSUPP;
		break;
	}
<<<<<<< HEAD
	wdev_unlock(wdev);
=======
	wiphy_unlock(&rdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}
