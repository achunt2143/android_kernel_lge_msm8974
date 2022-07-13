<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * cfg80211 MLME SAP interface
 *
 * Copyright (c) 2009, Jouni Malinen <j@w1.fi>
<<<<<<< HEAD
=======
 * Copyright (c) 2015		Intel Deutschland GmbH
 * Copyright (C) 2019-2020, 2022-2024 Intel Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/etherdevice.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/netdevice.h>
#include <linux/nl80211.h>
#include <linux/slab.h>
#include <linux/wireless.h>
#include <net/cfg80211.h>
#include <net/iw_handler.h>
#include "core.h"
#include "nl80211.h"
<<<<<<< HEAD

void cfg80211_send_rx_auth(struct net_device *dev, const u8 *buf, size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	wdev_lock(wdev);

	nl80211_send_rx_auth(rdev, dev, buf, len, GFP_KERNEL);
	cfg80211_sme_rx_auth(dev, buf, len);

	wdev_unlock(wdev);
}
EXPORT_SYMBOL(cfg80211_send_rx_auth);

void cfg80211_send_rx_assoc(struct net_device *dev, struct cfg80211_bss *bss,
			    const u8 *buf, size_t len)
{
	u16 status_code;
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)buf;
	u8 *ie = mgmt->u.assoc_resp.variable;
	int ieoffs = offsetof(struct ieee80211_mgmt, u.assoc_resp.variable);

	wdev_lock(wdev);

	status_code = le16_to_cpu(mgmt->u.assoc_resp.status_code);
=======
#include "rdev-ops.h"


void cfg80211_rx_assoc_resp(struct net_device *dev,
			    const struct cfg80211_rx_assoc_resp_data *data)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)data->buf;
	struct cfg80211_connect_resp_params cr = {
		.timeout_reason = NL80211_TIMEOUT_UNSPECIFIED,
		.req_ie = data->req_ies,
		.req_ie_len = data->req_ies_len,
		.resp_ie = mgmt->u.assoc_resp.variable,
		.resp_ie_len = data->len -
			       offsetof(struct ieee80211_mgmt,
					u.assoc_resp.variable),
		.status = le16_to_cpu(mgmt->u.assoc_resp.status_code),
		.ap_mld_addr = data->ap_mld_addr,
	};
	unsigned int link_id;

	for (link_id = 0; link_id < ARRAY_SIZE(data->links); link_id++) {
		cr.links[link_id].status = data->links[link_id].status;
		cr.links[link_id].bss = data->links[link_id].bss;

		WARN_ON_ONCE(cr.links[link_id].status != WLAN_STATUS_SUCCESS &&
			     (!cr.ap_mld_addr || !cr.links[link_id].bss));

		if (!cr.links[link_id].bss)
			continue;
		cr.links[link_id].bssid = data->links[link_id].bss->bssid;
		cr.links[link_id].addr = data->links[link_id].addr;
		/* need to have local link addresses for MLO connections */
		WARN_ON(cr.ap_mld_addr &&
			!is_valid_ether_addr(cr.links[link_id].addr));

		BUG_ON(!cr.links[link_id].bss->channel);

		if (cr.links[link_id].bss->channel->band == NL80211_BAND_S1GHZ) {
			WARN_ON(link_id);
			cr.resp_ie = (u8 *)&mgmt->u.s1g_assoc_resp.variable;
			cr.resp_ie_len = data->len -
					 offsetof(struct ieee80211_mgmt,
						  u.s1g_assoc_resp.variable);
		}

		if (cr.ap_mld_addr)
			cr.valid_links |= BIT(link_id);
	}

	trace_cfg80211_send_rx_assoc(dev, data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * This is a bit of a hack, we don't notify userspace of
	 * a (re-)association reply if we tried to send a reassoc
	 * and got a reject -- we only try again with an assoc
	 * frame instead of reassoc.
	 */
<<<<<<< HEAD
	if (status_code != WLAN_STATUS_SUCCESS && wdev->conn &&
	    cfg80211_sme_failed_reassoc(wdev)) {
		cfg80211_put_bss(bss);
		goto out;
	}

	nl80211_send_rx_assoc(rdev, dev, buf, len, GFP_KERNEL);

	if (status_code != WLAN_STATUS_SUCCESS && wdev->conn) {
		cfg80211_sme_failed_assoc(wdev);
		/*
		 * do not call connect_result() now because the
		 * sme will schedule work that does it later.
		 */
		cfg80211_put_bss(bss);
		goto out;
	}

	if (!wdev->conn && wdev->sme_state == CFG80211_SME_IDLE) {
		/*
		 * This is for the userspace SME, the CONNECTING
		 * state will be changed to CONNECTED by
		 * __cfg80211_connect_result() below.
		 */
		wdev->sme_state = CFG80211_SME_CONNECTING;
	}

	/* this consumes the bss reference */
	__cfg80211_connect_result(dev, mgmt->bssid, NULL, 0, ie, len - ieoffs,
				  status_code,
				  status_code == WLAN_STATUS_SUCCESS, bss);
 out:
	wdev_unlock(wdev);
}
EXPORT_SYMBOL(cfg80211_send_rx_assoc);

void __cfg80211_send_deauth(struct net_device *dev,
				   const u8 *buf, size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)buf;
	const u8 *bssid = mgmt->bssid;
	bool was_current = false;

	ASSERT_WDEV_LOCK(wdev);

	if (wdev->current_bss &&
	    memcmp(wdev->current_bss->pub.bssid, bssid, ETH_ALEN) == 0) {
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(&wdev->current_bss->pub);
		wdev->current_bss = NULL;
		was_current = true;
	}

	nl80211_send_deauth(rdev, dev, buf, len, GFP_KERNEL);

	if (wdev->sme_state == CFG80211_SME_CONNECTED && was_current) {
		u16 reason_code;
		bool from_ap;

		reason_code = le16_to_cpu(mgmt->u.deauth.reason_code);

		from_ap = memcmp(mgmt->sa, dev->dev_addr, ETH_ALEN) != 0;
		__cfg80211_disconnected(dev, NULL, 0, reason_code, from_ap);
	} else if (wdev->sme_state == CFG80211_SME_CONNECTING) {
		__cfg80211_connect_result(dev, mgmt->bssid, NULL, 0, NULL, 0,
					  WLAN_STATUS_UNSPECIFIED_FAILURE,
					  false, NULL);
	}
}
EXPORT_SYMBOL(__cfg80211_send_deauth);

void cfg80211_send_deauth(struct net_device *dev, const u8 *buf, size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;

	wdev_lock(wdev);
	__cfg80211_send_deauth(dev, buf, len);
	wdev_unlock(wdev);
}
EXPORT_SYMBOL(cfg80211_send_deauth);

void __cfg80211_send_disassoc(struct net_device *dev,
				     const u8 *buf, size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)buf;
	const u8 *bssid = mgmt->bssid;
	u16 reason_code;
	bool from_ap;

	ASSERT_WDEV_LOCK(wdev);

	nl80211_send_disassoc(rdev, dev, buf, len, GFP_KERNEL);

	if (wdev->sme_state != CFG80211_SME_CONNECTED)
		return;

	if (wdev->current_bss &&
	    memcmp(wdev->current_bss->pub.bssid, bssid, ETH_ALEN) == 0) {
		cfg80211_sme_disassoc(dev, wdev->current_bss);
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(&wdev->current_bss->pub);
		wdev->current_bss = NULL;
	} else
		WARN_ON(1);


	reason_code = le16_to_cpu(mgmt->u.disassoc.reason_code);

	from_ap = memcmp(mgmt->sa, dev->dev_addr, ETH_ALEN) != 0;
	__cfg80211_disconnected(dev, NULL, 0, reason_code, from_ap);
}
EXPORT_SYMBOL(__cfg80211_send_disassoc);

void cfg80211_send_disassoc(struct net_device *dev, const u8 *buf, size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;

	wdev_lock(wdev);
	__cfg80211_send_disassoc(dev, buf, len);
	wdev_unlock(wdev);
}
EXPORT_SYMBOL(cfg80211_send_disassoc);

void cfg80211_send_unprot_deauth(struct net_device *dev, const u8 *buf,
				 size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	nl80211_send_unprot_deauth(rdev, dev, buf, len, GFP_ATOMIC);
}
EXPORT_SYMBOL(cfg80211_send_unprot_deauth);

void cfg80211_send_unprot_disassoc(struct net_device *dev, const u8 *buf,
				   size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	nl80211_send_unprot_disassoc(rdev, dev, buf, len, GFP_ATOMIC);
}
EXPORT_SYMBOL(cfg80211_send_unprot_disassoc);

void cfg80211_send_auth_timeout(struct net_device *dev, const u8 *addr)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	wdev_lock(wdev);

	nl80211_send_auth_timeout(rdev, dev, addr, GFP_KERNEL);
	if (wdev->sme_state == CFG80211_SME_CONNECTING)
		__cfg80211_connect_result(dev, addr, NULL, 0, NULL, 0,
					  WLAN_STATUS_UNSPECIFIED_FAILURE,
					  false, NULL);

	wdev_unlock(wdev);
}
EXPORT_SYMBOL(cfg80211_send_auth_timeout);

void cfg80211_send_assoc_timeout(struct net_device *dev, const u8 *addr)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	wdev_lock(wdev);

	nl80211_send_assoc_timeout(rdev, dev, addr, GFP_KERNEL);
	if (wdev->sme_state == CFG80211_SME_CONNECTING)
		__cfg80211_connect_result(dev, addr, NULL, 0, NULL, 0,
					  WLAN_STATUS_UNSPECIFIED_FAILURE,
					  false, NULL);

	wdev_unlock(wdev);
}
EXPORT_SYMBOL(cfg80211_send_assoc_timeout);
=======
	if (cfg80211_sme_rx_assoc_resp(wdev, cr.status)) {
		for (link_id = 0; link_id < ARRAY_SIZE(data->links); link_id++) {
			struct cfg80211_bss *bss = data->links[link_id].bss;

			if (!bss)
				continue;

			cfg80211_unhold_bss(bss_from_pub(bss));
			cfg80211_put_bss(wiphy, bss);
		}
		return;
	}

	nl80211_send_rx_assoc(rdev, dev, data);
	/* update current_bss etc., consumes the bss reference */
	__cfg80211_connect_result(dev, &cr, cr.status == WLAN_STATUS_SUCCESS);
}
EXPORT_SYMBOL(cfg80211_rx_assoc_resp);

static void cfg80211_process_auth(struct wireless_dev *wdev,
				  const u8 *buf, size_t len)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);

	nl80211_send_rx_auth(rdev, wdev->netdev, buf, len, GFP_KERNEL);
	cfg80211_sme_rx_auth(wdev, buf, len);
}

static void cfg80211_process_deauth(struct wireless_dev *wdev,
				    const u8 *buf, size_t len,
				    bool reconnect)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)buf;
	const u8 *bssid = mgmt->bssid;
	u16 reason_code = le16_to_cpu(mgmt->u.deauth.reason_code);
	bool from_ap = !ether_addr_equal(mgmt->sa, wdev->netdev->dev_addr);

	nl80211_send_deauth(rdev, wdev->netdev, buf, len, reconnect, GFP_KERNEL);

	if (!wdev->connected || !ether_addr_equal(wdev->u.client.connected_addr, bssid))
		return;

	__cfg80211_disconnected(wdev->netdev, NULL, 0, reason_code, from_ap);
	cfg80211_sme_deauth(wdev);
}

static void cfg80211_process_disassoc(struct wireless_dev *wdev,
				      const u8 *buf, size_t len,
				      bool reconnect)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)buf;
	const u8 *bssid = mgmt->bssid;
	u16 reason_code = le16_to_cpu(mgmt->u.disassoc.reason_code);
	bool from_ap = !ether_addr_equal(mgmt->sa, wdev->netdev->dev_addr);

	nl80211_send_disassoc(rdev, wdev->netdev, buf, len, reconnect,
			      GFP_KERNEL);

	if (WARN_ON(!wdev->connected ||
		    !ether_addr_equal(wdev->u.client.connected_addr, bssid)))
		return;

	__cfg80211_disconnected(wdev->netdev, NULL, 0, reason_code, from_ap);
	cfg80211_sme_disassoc(wdev);
}

void cfg80211_rx_mlme_mgmt(struct net_device *dev, const u8 *buf, size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct ieee80211_mgmt *mgmt = (void *)buf;

	lockdep_assert_wiphy(wdev->wiphy);

	trace_cfg80211_rx_mlme_mgmt(dev, buf, len);

	if (WARN_ON(len < 2))
		return;

	if (ieee80211_is_auth(mgmt->frame_control))
		cfg80211_process_auth(wdev, buf, len);
	else if (ieee80211_is_deauth(mgmt->frame_control))
		cfg80211_process_deauth(wdev, buf, len, false);
	else if (ieee80211_is_disassoc(mgmt->frame_control))
		cfg80211_process_disassoc(wdev, buf, len, false);
}
EXPORT_SYMBOL(cfg80211_rx_mlme_mgmt);

void cfg80211_auth_timeout(struct net_device *dev, const u8 *addr)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);

	trace_cfg80211_send_auth_timeout(dev, addr);

	nl80211_send_auth_timeout(rdev, dev, addr, GFP_KERNEL);
	cfg80211_sme_auth_timeout(wdev);
}
EXPORT_SYMBOL(cfg80211_auth_timeout);

void cfg80211_assoc_failure(struct net_device *dev,
			    struct cfg80211_assoc_failure *data)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);
	const u8 *addr = data->ap_mld_addr ?: data->bss[0]->bssid;
	int i;

	trace_cfg80211_send_assoc_failure(dev, data);

	if (data->timeout) {
		nl80211_send_assoc_timeout(rdev, dev, addr, GFP_KERNEL);
		cfg80211_sme_assoc_timeout(wdev);
	} else {
		cfg80211_sme_abandon_assoc(wdev);
	}

	for (i = 0; i < ARRAY_SIZE(data->bss); i++) {
		struct cfg80211_bss *bss = data->bss[i];

		if (!bss)
			continue;

		cfg80211_unhold_bss(bss_from_pub(bss));
		cfg80211_put_bss(wiphy, bss);
	}
}
EXPORT_SYMBOL(cfg80211_assoc_failure);

void cfg80211_tx_mlme_mgmt(struct net_device *dev, const u8 *buf, size_t len,
			   bool reconnect)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct ieee80211_mgmt *mgmt = (void *)buf;

	lockdep_assert_wiphy(wdev->wiphy);

	trace_cfg80211_tx_mlme_mgmt(dev, buf, len, reconnect);

	if (WARN_ON(len < 2))
		return;

	if (ieee80211_is_deauth(mgmt->frame_control))
		cfg80211_process_deauth(wdev, buf, len, reconnect);
	else
		cfg80211_process_disassoc(wdev, buf, len, reconnect);
}
EXPORT_SYMBOL(cfg80211_tx_mlme_mgmt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void cfg80211_michael_mic_failure(struct net_device *dev, const u8 *addr,
				  enum nl80211_key_type key_type, int key_id,
				  const u8 *tsc, gfp_t gfp)
{
	struct wiphy *wiphy = dev->ieee80211_ptr->wiphy;
<<<<<<< HEAD
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
=======
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_CFG80211_WEXT
	union iwreq_data wrqu;
	char *buf = kmalloc(128, gfp);

	if (buf) {
<<<<<<< HEAD
		sprintf(buf, "MLME-MICHAELMICFAILURE.indication("
			"keyid=%d %scast addr=%pM)", key_id,
			key_type == NL80211_KEYTYPE_GROUP ? "broad" : "uni",
			addr);
		memset(&wrqu, 0, sizeof(wrqu));
		wrqu.data.length = strlen(buf);
=======
		memset(&wrqu, 0, sizeof(wrqu));
		wrqu.data.length =
			sprintf(buf, "MLME-MICHAELMICFAILURE."
				"indication(keyid=%d %scast addr=%pM)",
				key_id, key_type == NL80211_KEYTYPE_GROUP
				? "broad" : "uni", addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wireless_send_event(dev, IWEVCUSTOM, &wrqu, buf);
		kfree(buf);
	}
#endif

<<<<<<< HEAD
=======
	trace_cfg80211_michael_mic_failure(dev, addr, key_type, key_id, tsc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nl80211_michael_mic_failure(rdev, dev, addr, key_type, key_id, tsc, gfp);
}
EXPORT_SYMBOL(cfg80211_michael_mic_failure);

/* some MLME handling for userspace SME */
<<<<<<< HEAD
int __cfg80211_mlme_auth(struct cfg80211_registered_device *rdev,
			 struct net_device *dev,
			 struct ieee80211_channel *chan,
			 enum nl80211_auth_type auth_type,
			 const u8 *bssid,
			 const u8 *ssid, int ssid_len,
			 const u8 *ie, int ie_len,
			 const u8 *key, int key_len, int key_idx)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_auth_request req;
	int err;

	ASSERT_WDEV_LOCK(wdev);

	if (auth_type == NL80211_AUTHTYPE_SHARED_KEY)
		if (!key || !key_len || key_idx < 0 || key_idx > 4)
			return -EINVAL;

	if (wdev->current_bss &&
	    memcmp(bssid, wdev->current_bss->pub.bssid, ETH_ALEN) == 0)
		return -EALREADY;

	memset(&req, 0, sizeof(req));

	req.ie = ie;
	req.ie_len = ie_len;
	req.auth_type = auth_type;
	req.bss = cfg80211_get_bss(&rdev->wiphy, chan, bssid, ssid, ssid_len,
				   WLAN_CAPABILITY_ESS, WLAN_CAPABILITY_ESS);
	req.key = key;
	req.key_len = key_len;
	req.key_idx = key_idx;
	if (!req.bss)
		return -ENOENT;

	err = rdev->ops->auth(&rdev->wiphy, dev, &req);

	cfg80211_put_bss(req.bss);
	return err;
}

int cfg80211_mlme_auth(struct cfg80211_registered_device *rdev,
		       struct net_device *dev, struct ieee80211_channel *chan,
		       enum nl80211_auth_type auth_type, const u8 *bssid,
		       const u8 *ssid, int ssid_len,
		       const u8 *ie, int ie_len,
		       const u8 *key, int key_len, int key_idx)
{
	int err;

	wdev_lock(dev->ieee80211_ptr);
	err = __cfg80211_mlme_auth(rdev, dev, chan, auth_type, bssid,
				   ssid, ssid_len, ie, ie_len,
				   key, key_len, key_idx);
	wdev_unlock(dev->ieee80211_ptr);

	return err;
=======
int cfg80211_mlme_auth(struct cfg80211_registered_device *rdev,
		       struct net_device *dev,
		       struct cfg80211_auth_request *req)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;

	lockdep_assert_wiphy(wdev->wiphy);

	if (!req->bss)
		return -ENOENT;

	if (req->link_id >= 0 &&
	    !(wdev->wiphy->flags & WIPHY_FLAG_SUPPORTS_MLO))
		return -EINVAL;

	if (req->auth_type == NL80211_AUTHTYPE_SHARED_KEY) {
		if (!req->key || !req->key_len ||
		    req->key_idx < 0 || req->key_idx > 3)
			return -EINVAL;
	}

	if (wdev->connected &&
	    ether_addr_equal(req->bss->bssid, wdev->u.client.connected_addr))
		return -EALREADY;

	if (ether_addr_equal(req->bss->bssid, dev->dev_addr) ||
	    (req->link_id >= 0 &&
	     ether_addr_equal(req->ap_mld_addr, dev->dev_addr)))
		return -EINVAL;

	return rdev_auth(rdev, dev, req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*  Do a logical ht_capa &= ht_capa_mask.  */
void cfg80211_oper_and_ht_capa(struct ieee80211_ht_cap *ht_capa,
			       const struct ieee80211_ht_cap *ht_capa_mask)
{
	int i;
	u8 *p1, *p2;
	if (!ht_capa_mask) {
		memset(ht_capa, 0, sizeof(*ht_capa));
		return;
	}

	p1 = (u8*)(ht_capa);
	p2 = (u8*)(ht_capa_mask);
<<<<<<< HEAD
	for (i = 0; i<sizeof(*ht_capa); i++)
		p1[i] &= p2[i];
}

int __cfg80211_mlme_assoc(struct cfg80211_registered_device *rdev,
			  struct net_device *dev,
			  struct ieee80211_channel *chan,
			  const u8 *bssid, const u8 *prev_bssid,
			  const u8 *ssid, int ssid_len,
			  const u8 *ie, int ie_len, bool use_mfp,
			  struct cfg80211_crypto_settings *crypt,
			  u32 assoc_flags, struct ieee80211_ht_cap *ht_capa,
			  struct ieee80211_ht_cap *ht_capa_mask)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_assoc_request req;
	int err;
	bool was_connected = false;

	ASSERT_WDEV_LOCK(wdev);

	memset(&req, 0, sizeof(req));

	if (wdev->current_bss && prev_bssid &&
	    memcmp(wdev->current_bss->pub.bssid, prev_bssid, ETH_ALEN) == 0) {
		/*
		 * Trying to reassociate: Allow this to proceed and let the old
		 * association to be dropped when the new one is completed.
		 */
		if (wdev->sme_state == CFG80211_SME_CONNECTED) {
			was_connected = true;
			wdev->sme_state = CFG80211_SME_CONNECTING;
		}
	} else if (wdev->current_bss)
		return -EALREADY;

	req.ie = ie;
	req.ie_len = ie_len;
	memcpy(&req.crypto, crypt, sizeof(req.crypto));
	req.use_mfp = use_mfp;
	req.prev_bssid = prev_bssid;
	req.flags = assoc_flags;
	if (ht_capa)
		memcpy(&req.ht_capa, ht_capa, sizeof(req.ht_capa));
	if (ht_capa_mask)
		memcpy(&req.ht_capa_mask, ht_capa_mask,
		       sizeof(req.ht_capa_mask));
	cfg80211_oper_and_ht_capa(&req.ht_capa_mask,
				  rdev->wiphy.ht_capa_mod_mask);

	req.bss = cfg80211_get_bss(&rdev->wiphy, chan, bssid, ssid, ssid_len,
				   WLAN_CAPABILITY_ESS, WLAN_CAPABILITY_ESS);
	if (!req.bss) {
		if (was_connected)
			wdev->sme_state = CFG80211_SME_CONNECTED;
		return -ENOENT;
	}

	err = rdev->ops->assoc(&rdev->wiphy, dev, &req);

	if (err) {
		if (was_connected)
			wdev->sme_state = CFG80211_SME_CONNECTED;
		cfg80211_put_bss(req.bss);
	}

	return err;
}

int cfg80211_mlme_assoc(struct cfg80211_registered_device *rdev,
			struct net_device *dev,
			struct ieee80211_channel *chan,
			const u8 *bssid, const u8 *prev_bssid,
			const u8 *ssid, int ssid_len,
			const u8 *ie, int ie_len, bool use_mfp,
			struct cfg80211_crypto_settings *crypt,
			u32 assoc_flags, struct ieee80211_ht_cap *ht_capa,
			struct ieee80211_ht_cap *ht_capa_mask)
=======
	for (i = 0; i < sizeof(*ht_capa); i++)
		p1[i] &= p2[i];
}

/*  Do a logical vht_capa &= vht_capa_mask.  */
void cfg80211_oper_and_vht_capa(struct ieee80211_vht_cap *vht_capa,
				const struct ieee80211_vht_cap *vht_capa_mask)
{
	int i;
	u8 *p1, *p2;
	if (!vht_capa_mask) {
		memset(vht_capa, 0, sizeof(*vht_capa));
		return;
	}

	p1 = (u8*)(vht_capa);
	p2 = (u8*)(vht_capa_mask);
	for (i = 0; i < sizeof(*vht_capa); i++)
		p1[i] &= p2[i];
}

static int
cfg80211_mlme_check_mlo_compat(const struct ieee80211_multi_link_elem *mle_a,
			       const struct ieee80211_multi_link_elem *mle_b,
			       struct netlink_ext_ack *extack)
{
	const struct ieee80211_mle_basic_common_info *common_a, *common_b;

	common_a = (const void *)mle_a->variable;
	common_b = (const void *)mle_b->variable;

	if (memcmp(common_a->mld_mac_addr, common_b->mld_mac_addr, ETH_ALEN)) {
		NL_SET_ERR_MSG(extack, "AP MLD address mismatch");
		return -EINVAL;
	}

	if (ieee80211_mle_get_eml_med_sync_delay((const u8 *)mle_a) !=
	    ieee80211_mle_get_eml_med_sync_delay((const u8 *)mle_b)) {
		NL_SET_ERR_MSG(extack, "link EML medium sync delay mismatch");
		return -EINVAL;
	}

	if (ieee80211_mle_get_eml_cap((const u8 *)mle_a) !=
	    ieee80211_mle_get_eml_cap((const u8 *)mle_b)) {
		NL_SET_ERR_MSG(extack, "link EML capabilities mismatch");
		return -EINVAL;
	}

	if (ieee80211_mle_get_mld_capa_op((const u8 *)mle_a) !=
	    ieee80211_mle_get_mld_capa_op((const u8 *)mle_b)) {
		NL_SET_ERR_MSG(extack, "link MLD capabilities/ops mismatch");
		return -EINVAL;
	}

	return 0;
}

static int cfg80211_mlme_check_mlo(struct net_device *dev,
				   struct cfg80211_assoc_request *req,
				   struct netlink_ext_ack *extack)
{
	const struct ieee80211_multi_link_elem *mles[ARRAY_SIZE(req->links)] = {};
	int i;

	if (req->link_id < 0)
		return 0;

	if (!req->links[req->link_id].bss) {
		NL_SET_ERR_MSG(extack, "no BSS for assoc link");
		return -EINVAL;
	}

	rcu_read_lock();
	for (i = 0; i < ARRAY_SIZE(req->links); i++) {
		const struct cfg80211_bss_ies *ies;
		const struct element *ml;

		if (!req->links[i].bss)
			continue;

		if (ether_addr_equal(req->links[i].bss->bssid, dev->dev_addr)) {
			NL_SET_ERR_MSG(extack, "BSSID must not be our address");
			req->links[i].error = -EINVAL;
			goto error;
		}

		ies = rcu_dereference(req->links[i].bss->ies);
		ml = cfg80211_find_ext_elem(WLAN_EID_EXT_EHT_MULTI_LINK,
					    ies->data, ies->len);
		if (!ml) {
			NL_SET_ERR_MSG(extack, "MLO BSS w/o ML element");
			req->links[i].error = -EINVAL;
			goto error;
		}

		if (!ieee80211_mle_type_ok(ml->data + 1,
					   IEEE80211_ML_CONTROL_TYPE_BASIC,
					   ml->datalen - 1)) {
			NL_SET_ERR_MSG(extack, "BSS with invalid ML element");
			req->links[i].error = -EINVAL;
			goto error;
		}

		mles[i] = (const void *)(ml->data + 1);

		if (ieee80211_mle_get_link_id((const u8 *)mles[i]) != i) {
			NL_SET_ERR_MSG(extack, "link ID mismatch");
			req->links[i].error = -EINVAL;
			goto error;
		}
	}

	if (WARN_ON(!mles[req->link_id]))
		goto error;

	for (i = 0; i < ARRAY_SIZE(req->links); i++) {
		if (i == req->link_id || !req->links[i].bss)
			continue;

		if (WARN_ON(!mles[i]))
			goto error;

		if (cfg80211_mlme_check_mlo_compat(mles[req->link_id], mles[i],
						   extack)) {
			req->links[i].error = -EINVAL;
			goto error;
		}
	}

	rcu_read_unlock();
	return 0;
error:
	rcu_read_unlock();
	return -EINVAL;
}

/* Note: caller must cfg80211_put_bss() regardless of result */
int cfg80211_mlme_assoc(struct cfg80211_registered_device *rdev,
			struct net_device *dev,
			struct cfg80211_assoc_request *req,
			struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err;

<<<<<<< HEAD
	wdev_lock(wdev);
	err = __cfg80211_mlme_assoc(rdev, dev, chan, bssid, prev_bssid,
				    ssid, ssid_len, ie, ie_len, use_mfp, crypt,
				    assoc_flags, ht_capa, ht_capa_mask);
	wdev_unlock(wdev);

	return err;
}

int __cfg80211_mlme_deauth(struct cfg80211_registered_device *rdev,
			   struct net_device *dev, const u8 *bssid,
			   const u8 *ie, int ie_len, u16 reason,
			   bool local_state_change)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_deauth_request req = {
		.bssid = bssid,
		.reason_code = reason,
		.ie = ie,
		.ie_len = ie_len,
	};

	ASSERT_WDEV_LOCK(wdev);

	if (local_state_change) {
		if (wdev->current_bss &&
		    memcmp(wdev->current_bss->pub.bssid, bssid, ETH_ALEN) == 0) {
			cfg80211_unhold_bss(wdev->current_bss);
			cfg80211_put_bss(&wdev->current_bss->pub);
			wdev->current_bss = NULL;
		}

		return 0;
	}

	return rdev->ops->deauth(&rdev->wiphy, dev, &req);
=======
	lockdep_assert_wiphy(wdev->wiphy);

	err = cfg80211_mlme_check_mlo(dev, req, extack);
	if (err)
		return err;

	if (wdev->connected &&
	    (!req->prev_bssid ||
	     !ether_addr_equal(wdev->u.client.connected_addr, req->prev_bssid)))
		return -EALREADY;

	if ((req->bss && ether_addr_equal(req->bss->bssid, dev->dev_addr)) ||
	    (req->link_id >= 0 &&
	     ether_addr_equal(req->ap_mld_addr, dev->dev_addr)))
		return -EINVAL;

	cfg80211_oper_and_ht_capa(&req->ht_capa_mask,
				  rdev->wiphy.ht_capa_mod_mask);
	cfg80211_oper_and_vht_capa(&req->vht_capa_mask,
				   rdev->wiphy.vht_capa_mod_mask);

	err = rdev_assoc(rdev, dev, req);
	if (!err) {
		int link_id;

		if (req->bss) {
			cfg80211_ref_bss(&rdev->wiphy, req->bss);
			cfg80211_hold_bss(bss_from_pub(req->bss));
		}

		for (link_id = 0; link_id < ARRAY_SIZE(req->links); link_id++) {
			if (!req->links[link_id].bss)
				continue;
			cfg80211_ref_bss(&rdev->wiphy, req->links[link_id].bss);
			cfg80211_hold_bss(bss_from_pub(req->links[link_id].bss));
		}
	}
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int cfg80211_mlme_deauth(struct cfg80211_registered_device *rdev,
			 struct net_device *dev, const u8 *bssid,
			 const u8 *ie, int ie_len, u16 reason,
			 bool local_state_change)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	int err;

	wdev_lock(wdev);
	err = __cfg80211_mlme_deauth(rdev, dev, bssid, ie, ie_len, reason,
				     local_state_change);
	wdev_unlock(wdev);

	return err;
}

static int __cfg80211_mlme_disassoc(struct cfg80211_registered_device *rdev,
				    struct net_device *dev, const u8 *bssid,
				    const u8 *ie, int ie_len, u16 reason,
				    bool local_state_change)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_disassoc_request req;

	ASSERT_WDEV_LOCK(wdev);

	if (wdev->sme_state != CFG80211_SME_CONNECTED)
		return -ENOTCONN;

	if (WARN_ON(!wdev->current_bss))
		return -ENOTCONN;

	memset(&req, 0, sizeof(req));
	req.reason_code = reason;
	req.local_state_change = local_state_change;
	req.ie = ie;
	req.ie_len = ie_len;
	if (memcmp(wdev->current_bss->pub.bssid, bssid, ETH_ALEN) == 0)
		req.bss = &wdev->current_bss->pub;
	else
		return -ENOTCONN;

	return rdev->ops->disassoc(&rdev->wiphy, dev, &req);
}

int cfg80211_mlme_disassoc(struct cfg80211_registered_device *rdev,
			   struct net_device *dev, const u8 *bssid,
=======
	struct cfg80211_deauth_request req = {
		.bssid = bssid,
		.reason_code = reason,
		.ie = ie,
		.ie_len = ie_len,
		.local_state_change = local_state_change,
	};

	lockdep_assert_wiphy(wdev->wiphy);

	if (local_state_change &&
	    (!wdev->connected ||
	     !ether_addr_equal(wdev->u.client.connected_addr, bssid)))
		return 0;

	if (ether_addr_equal(wdev->disconnect_bssid, bssid) ||
	    (wdev->connected &&
	     ether_addr_equal(wdev->u.client.connected_addr, bssid)))
		wdev->conn_owner_nlportid = 0;

	return rdev_deauth(rdev, dev, &req);
}

int cfg80211_mlme_disassoc(struct cfg80211_registered_device *rdev,
			   struct net_device *dev, const u8 *ap_addr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   const u8 *ie, int ie_len, u16 reason,
			   bool local_state_change)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	int err;

	wdev_lock(wdev);
	err = __cfg80211_mlme_disassoc(rdev, dev, bssid, ie, ie_len, reason,
				       local_state_change);
	wdev_unlock(wdev);

	return err;
=======
	struct cfg80211_disassoc_request req = {
		.reason_code = reason,
		.local_state_change = local_state_change,
		.ie = ie,
		.ie_len = ie_len,
		.ap_addr = ap_addr,
	};
	int err;

	lockdep_assert_wiphy(wdev->wiphy);

	if (!wdev->connected)
		return -ENOTCONN;

	if (memcmp(wdev->u.client.connected_addr, ap_addr, ETH_ALEN))
		return -ENOTCONN;

	err = rdev_disassoc(rdev, dev, &req);
	if (err)
		return err;

	/* driver should have reported the disassoc */
	WARN_ON(wdev->connected);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void cfg80211_mlme_down(struct cfg80211_registered_device *rdev,
			struct net_device *dev)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	struct cfg80211_deauth_request req;
	u8 bssid[ETH_ALEN];

	ASSERT_WDEV_LOCK(wdev);
=======
	u8 bssid[ETH_ALEN];

	lockdep_assert_wiphy(wdev->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!rdev->ops->deauth)
		return;

<<<<<<< HEAD
	memset(&req, 0, sizeof(req));
	req.reason_code = WLAN_REASON_DEAUTH_LEAVING;
	req.ie = NULL;
	req.ie_len = 0;

	if (!wdev->current_bss)
		return;

	memcpy(bssid, wdev->current_bss->pub.bssid, ETH_ALEN);
	req.bssid = bssid;
	rdev->ops->deauth(&rdev->wiphy, dev, &req);

	if (wdev->current_bss) {
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(&wdev->current_bss->pub);
		wdev->current_bss = NULL;
	}
}

void cfg80211_ready_on_channel(struct net_device *dev, u64 cookie,
			       struct ieee80211_channel *chan,
			       enum nl80211_channel_type channel_type,
			       unsigned int duration, gfp_t gfp)
{
	struct wiphy *wiphy = dev->ieee80211_ptr->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	nl80211_send_remain_on_channel(rdev, dev, cookie, chan, channel_type,
				       duration, gfp);
}
EXPORT_SYMBOL(cfg80211_ready_on_channel);

void cfg80211_remain_on_channel_expired(struct net_device *dev,
					u64 cookie,
					struct ieee80211_channel *chan,
					enum nl80211_channel_type channel_type,
					gfp_t gfp)
{
	struct wiphy *wiphy = dev->ieee80211_ptr->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	nl80211_send_remain_on_channel_cancel(rdev, dev, cookie, chan,
					      channel_type, gfp);
}
EXPORT_SYMBOL(cfg80211_remain_on_channel_expired);

void cfg80211_new_sta(struct net_device *dev, const u8 *mac_addr,
		      struct station_info *sinfo, gfp_t gfp)
{
	struct wiphy *wiphy = dev->ieee80211_ptr->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	nl80211_send_sta_event(rdev, dev, mac_addr, sinfo, gfp);
}
EXPORT_SYMBOL(cfg80211_new_sta);

void cfg80211_del_sta(struct net_device *dev, const u8 *mac_addr, gfp_t gfp)
{
	struct wiphy *wiphy = dev->ieee80211_ptr->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	nl80211_send_sta_del_event(rdev, dev, mac_addr, gfp);
}
EXPORT_SYMBOL(cfg80211_del_sta);

struct cfg80211_mgmt_registration {
	struct list_head list;

	u32 nlpid;
=======
	if (!wdev->connected)
		return;

	memcpy(bssid, wdev->u.client.connected_addr, ETH_ALEN);
	cfg80211_mlme_deauth(rdev, dev, bssid, NULL, 0,
			     WLAN_REASON_DEAUTH_LEAVING, false);
}

struct cfg80211_mgmt_registration {
	struct list_head list;
	struct wireless_dev *wdev;

	u32 nlportid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	int match_len;

	__le16 frame_type;

<<<<<<< HEAD
	u8 match[];
};

int cfg80211_mlme_register_mgmt(struct wireless_dev *wdev, u32 snd_pid,
				u16 frame_type, const u8 *match_data,
				int match_len)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct cfg80211_mgmt_registration *reg, *nreg;
	int err = 0;
	u16 mgmt_type;
=======
	bool multicast_rx;

	u8 match[];
};

static void cfg80211_mgmt_registrations_update(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct wireless_dev *tmp;
	struct cfg80211_mgmt_registration *reg;
	struct mgmt_frame_regs upd = {};

	lockdep_assert_held(&rdev->wiphy.mtx);

	spin_lock_bh(&rdev->mgmt_registrations_lock);
	if (!wdev->mgmt_registrations_need_update) {
		spin_unlock_bh(&rdev->mgmt_registrations_lock);
		return;
	}

	rcu_read_lock();
	list_for_each_entry_rcu(tmp, &rdev->wiphy.wdev_list, list) {
		list_for_each_entry(reg, &tmp->mgmt_registrations, list) {
			u32 mask = BIT(le16_to_cpu(reg->frame_type) >> 4);
			u32 mcast_mask = 0;

			if (reg->multicast_rx)
				mcast_mask = mask;

			upd.global_stypes |= mask;
			upd.global_mcast_stypes |= mcast_mask;

			if (tmp == wdev) {
				upd.interface_stypes |= mask;
				upd.interface_mcast_stypes |= mcast_mask;
			}
		}
	}
	rcu_read_unlock();

	wdev->mgmt_registrations_need_update = 0;
	spin_unlock_bh(&rdev->mgmt_registrations_lock);

	rdev_update_mgmt_frame_registrations(rdev, wdev, &upd);
}

void cfg80211_mgmt_registrations_update_wk(struct work_struct *wk)
{
	struct cfg80211_registered_device *rdev;
	struct wireless_dev *wdev;

	rdev = container_of(wk, struct cfg80211_registered_device,
			    mgmt_registrations_update_wk);

	wiphy_lock(&rdev->wiphy);
	list_for_each_entry(wdev, &rdev->wiphy.wdev_list, list)
		cfg80211_mgmt_registrations_update(wdev);
	wiphy_unlock(&rdev->wiphy);
}

int cfg80211_mlme_register_mgmt(struct wireless_dev *wdev, u32 snd_portid,
				u16 frame_type, const u8 *match_data,
				int match_len, bool multicast_rx,
				struct netlink_ext_ack *extack)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct cfg80211_mgmt_registration *reg, *nreg;
	int err = 0;
	u16 mgmt_type;
	bool update_multicast = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!wdev->wiphy->mgmt_stypes)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if ((frame_type & IEEE80211_FCTL_FTYPE) != IEEE80211_FTYPE_MGMT)
		return -EINVAL;

	if (frame_type & ~(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE))
		return -EINVAL;

	mgmt_type = (frame_type & IEEE80211_FCTL_STYPE) >> 4;
	if (!(wdev->wiphy->mgmt_stypes[wdev->iftype].rx & BIT(mgmt_type)))
		return -EINVAL;
=======
	if ((frame_type & IEEE80211_FCTL_FTYPE) != IEEE80211_FTYPE_MGMT) {
		NL_SET_ERR_MSG(extack, "frame type not management");
		return -EINVAL;
	}

	if (frame_type & ~(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) {
		NL_SET_ERR_MSG(extack, "Invalid frame type");
		return -EINVAL;
	}

	mgmt_type = (frame_type & IEEE80211_FCTL_STYPE) >> 4;
	if (!(wdev->wiphy->mgmt_stypes[wdev->iftype].rx & BIT(mgmt_type))) {
		NL_SET_ERR_MSG(extack,
			       "Registration to specific type not supported");
		return -EINVAL;
	}

	/*
	 * To support Pre Association Security Negotiation (PASN), registration
	 * for authentication frames should be supported. However, as some
	 * versions of the user space daemons wrongly register to all types of
	 * authentication frames (which might result in unexpected behavior)
	 * allow such registration if the request is for a specific
	 * authentication algorithm number.
	 */
	if (wdev->iftype == NL80211_IFTYPE_STATION &&
	    (frame_type & IEEE80211_FCTL_STYPE) == IEEE80211_STYPE_AUTH &&
	    !(match_data && match_len >= 2)) {
		NL_SET_ERR_MSG(extack,
			       "Authentication algorithm number required");
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	nreg = kzalloc(sizeof(*reg) + match_len, GFP_KERNEL);
	if (!nreg)
		return -ENOMEM;

<<<<<<< HEAD
	spin_lock_bh(&wdev->mgmt_registrations_lock);
=======
	spin_lock_bh(&rdev->mgmt_registrations_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	list_for_each_entry(reg, &wdev->mgmt_registrations, list) {
		int mlen = min(match_len, reg->match_len);

		if (frame_type != le16_to_cpu(reg->frame_type))
			continue;

		if (memcmp(reg->match, match_data, mlen) == 0) {
<<<<<<< HEAD
=======
			if (reg->multicast_rx != multicast_rx) {
				update_multicast = true;
				reg->multicast_rx = multicast_rx;
				break;
			}
			NL_SET_ERR_MSG(extack, "Match already configured");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EALREADY;
			break;
		}
	}

<<<<<<< HEAD
	if (err) {
		kfree(nreg);
		goto out;
	}

	memcpy(nreg->match, match_data, match_len);
	nreg->match_len = match_len;
	nreg->nlpid = snd_pid;
	nreg->frame_type = cpu_to_le16(frame_type);
	list_add(&nreg->list, &wdev->mgmt_registrations);

	if (rdev->ops->mgmt_frame_register)
		rdev->ops->mgmt_frame_register(wiphy, wdev->netdev,
					       frame_type, true);

 out:
	spin_unlock_bh(&wdev->mgmt_registrations_lock);
=======
	if (err)
		goto out;

	if (update_multicast) {
		kfree(nreg);
	} else {
		memcpy(nreg->match, match_data, match_len);
		nreg->match_len = match_len;
		nreg->nlportid = snd_portid;
		nreg->frame_type = cpu_to_le16(frame_type);
		nreg->wdev = wdev;
		nreg->multicast_rx = multicast_rx;
		list_add(&nreg->list, &wdev->mgmt_registrations);
	}
	wdev->mgmt_registrations_need_update = 1;
	spin_unlock_bh(&rdev->mgmt_registrations_lock);

	cfg80211_mgmt_registrations_update(wdev);

	return 0;

 out:
	kfree(nreg);
	spin_unlock_bh(&rdev->mgmt_registrations_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
void cfg80211_mlme_unregister_socket(struct wireless_dev *wdev, u32 nlpid)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct cfg80211_mgmt_registration *reg, *tmp;

	spin_lock_bh(&wdev->mgmt_registrations_lock);

	list_for_each_entry_safe(reg, tmp, &wdev->mgmt_registrations, list) {
		if (reg->nlpid != nlpid)
			continue;

		if (rdev->ops->mgmt_frame_register) {
			u16 frame_type = le16_to_cpu(reg->frame_type);

			rdev->ops->mgmt_frame_register(wiphy, wdev->netdev,
						       frame_type, false);
		}

		list_del(&reg->list);
		kfree(reg);
	}

	spin_unlock_bh(&wdev->mgmt_registrations_lock);

	if (nlpid == wdev->ap_unexpected_nlpid)
		wdev->ap_unexpected_nlpid = 0;
=======
void cfg80211_mlme_unregister_socket(struct wireless_dev *wdev, u32 nlportid)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);
	struct cfg80211_mgmt_registration *reg, *tmp;

	spin_lock_bh(&rdev->mgmt_registrations_lock);

	list_for_each_entry_safe(reg, tmp, &wdev->mgmt_registrations, list) {
		if (reg->nlportid != nlportid)
			continue;

		list_del(&reg->list);
		kfree(reg);

		wdev->mgmt_registrations_need_update = 1;
		schedule_work(&rdev->mgmt_registrations_update_wk);
	}

	spin_unlock_bh(&rdev->mgmt_registrations_lock);

	if (nlportid && rdev->crit_proto_nlportid == nlportid) {
		rdev->crit_proto_nlportid = 0;
		rdev_crit_proto_stop(rdev, wdev);
	}

	if (nlportid == wdev->ap_unexpected_nlportid)
		wdev->ap_unexpected_nlportid = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void cfg80211_mlme_purge_registrations(struct wireless_dev *wdev)
{
<<<<<<< HEAD
	struct cfg80211_mgmt_registration *reg, *tmp;

	spin_lock_bh(&wdev->mgmt_registrations_lock);

=======
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wdev->wiphy);
	struct cfg80211_mgmt_registration *reg, *tmp;

	spin_lock_bh(&rdev->mgmt_registrations_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry_safe(reg, tmp, &wdev->mgmt_registrations, list) {
		list_del(&reg->list);
		kfree(reg);
	}
<<<<<<< HEAD

	spin_unlock_bh(&wdev->mgmt_registrations_lock);
}

int cfg80211_mlme_mgmt_tx(struct cfg80211_registered_device *rdev,
			  struct net_device *dev,
			  struct ieee80211_channel *chan, bool offchan,
			  enum nl80211_channel_type channel_type,
			  bool channel_type_valid, unsigned int wait,
			  const u8 *buf, size_t len, bool no_cck,
			  bool dont_wait_for_ack, u64 *cookie)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	const struct ieee80211_mgmt *mgmt;
	u16 stype;

=======
	wdev->mgmt_registrations_need_update = 1;
	spin_unlock_bh(&rdev->mgmt_registrations_lock);

	cfg80211_mgmt_registrations_update(wdev);
}

static bool cfg80211_allowed_address(struct wireless_dev *wdev, const u8 *addr)
{
	int i;

	for_each_valid_link(wdev, i) {
		if (ether_addr_equal(addr, wdev->links[i].addr))
			return true;
	}

	return ether_addr_equal(addr, wdev_address(wdev));
}

static bool cfg80211_allowed_random_address(struct wireless_dev *wdev,
					    const struct ieee80211_mgmt *mgmt)
{
	if (ieee80211_is_auth(mgmt->frame_control) ||
	    ieee80211_is_deauth(mgmt->frame_control)) {
		/* Allow random TA to be used with authentication and
		 * deauthentication frames if the driver has indicated support.
		 */
		if (wiphy_ext_feature_isset(
			    wdev->wiphy,
			    NL80211_EXT_FEATURE_AUTH_AND_DEAUTH_RANDOM_TA))
			return true;
	} else if (ieee80211_is_action(mgmt->frame_control) &&
		   mgmt->u.action.category == WLAN_CATEGORY_PUBLIC) {
		/* Allow random TA to be used with Public Action frames if the
		 * driver has indicated support.
		 */
		if (!wdev->connected &&
		    wiphy_ext_feature_isset(
			    wdev->wiphy,
			    NL80211_EXT_FEATURE_MGMT_TX_RANDOM_TA))
			return true;

		if (wdev->connected &&
		    wiphy_ext_feature_isset(
			    wdev->wiphy,
			    NL80211_EXT_FEATURE_MGMT_TX_RANDOM_TA_CONNECTED))
			return true;
	}

	return false;
}

int cfg80211_mlme_mgmt_tx(struct cfg80211_registered_device *rdev,
			  struct wireless_dev *wdev,
			  struct cfg80211_mgmt_tx_params *params, u64 *cookie)
{
	const struct ieee80211_mgmt *mgmt;
	u16 stype;

	lockdep_assert_wiphy(&rdev->wiphy);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!wdev->wiphy->mgmt_stypes)
		return -EOPNOTSUPP;

	if (!rdev->ops->mgmt_tx)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (len < 24 + 1)
		return -EINVAL;

	mgmt = (const struct ieee80211_mgmt *) buf;
=======
	if (params->len < 24 + 1)
		return -EINVAL;

	mgmt = (const struct ieee80211_mgmt *)params->buf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!ieee80211_is_mgmt(mgmt->frame_control))
		return -EINVAL;

	stype = le16_to_cpu(mgmt->frame_control) & IEEE80211_FCTL_STYPE;
	if (!(wdev->wiphy->mgmt_stypes[wdev->iftype].tx & BIT(stype >> 4)))
		return -EINVAL;

	if (ieee80211_is_action(mgmt->frame_control) &&
	    mgmt->u.action.category != WLAN_CATEGORY_PUBLIC) {
		int err = 0;

<<<<<<< HEAD
		wdev_lock(wdev);

		switch (wdev->iftype) {
		case NL80211_IFTYPE_ADHOC:
		case NL80211_IFTYPE_STATION:
		case NL80211_IFTYPE_P2P_CLIENT:
			if (!wdev->current_bss) {
				err = -ENOTCONN;
				break;
			}

			if (memcmp(wdev->current_bss->pub.bssid,
				   mgmt->bssid, ETH_ALEN)) {
				err = -ENOTCONN;
				break;
			}

=======
		switch (wdev->iftype) {
		case NL80211_IFTYPE_ADHOC:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * check for IBSS DA must be done by driver as
			 * cfg80211 doesn't track the stations
			 */
<<<<<<< HEAD
			if (wdev->iftype == NL80211_IFTYPE_ADHOC)
				break;

			/* for station, check that DA is the AP */
			if (memcmp(wdev->current_bss->pub.bssid,
				   mgmt->da, ETH_ALEN)) {
=======
			if (!wdev->u.ibss.current_bss ||
			    !ether_addr_equal(wdev->u.ibss.current_bss->pub.bssid,
					      mgmt->bssid)) {
				err = -ENOTCONN;
				break;
			}
			break;
		case NL80211_IFTYPE_STATION:
		case NL80211_IFTYPE_P2P_CLIENT:
			if (!wdev->connected) {
				err = -ENOTCONN;
				break;
			}

			/* FIXME: MLD may address this differently */

			if (!ether_addr_equal(wdev->u.client.connected_addr,
					      mgmt->bssid)) {
				err = -ENOTCONN;
				break;
			}

			/* for station, check that DA is the AP */
			if (!ether_addr_equal(wdev->u.client.connected_addr,
					      mgmt->da)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				err = -ENOTCONN;
				break;
			}
			break;
		case NL80211_IFTYPE_AP:
		case NL80211_IFTYPE_P2P_GO:
		case NL80211_IFTYPE_AP_VLAN:
<<<<<<< HEAD
			if (memcmp(mgmt->bssid, dev->dev_addr, ETH_ALEN))
				err = -EINVAL;
			break;
		case NL80211_IFTYPE_MESH_POINT:
			if (memcmp(mgmt->sa, mgmt->bssid, ETH_ALEN)) {
=======
			if (!ether_addr_equal(mgmt->bssid, wdev_address(wdev)) &&
			    (params->link_id < 0 ||
			     !ether_addr_equal(mgmt->bssid,
					       wdev->links[params->link_id].addr)))
				err = -EINVAL;
			break;
		case NL80211_IFTYPE_MESH_POINT:
			if (!ether_addr_equal(mgmt->sa, mgmt->bssid)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				err = -EINVAL;
				break;
			}
			/*
			 * check for mesh DA must be done by driver as
			 * cfg80211 doesn't track the stations
			 */
			break;
<<<<<<< HEAD
=======
		case NL80211_IFTYPE_P2P_DEVICE:
			/*
			 * fall through, P2P device only supports
			 * public action frames
			 */
		case NL80211_IFTYPE_NAN:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			err = -EOPNOTSUPP;
			break;
		}
<<<<<<< HEAD
		wdev_unlock(wdev);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (err)
			return err;
	}

<<<<<<< HEAD
	if (memcmp(mgmt->sa, dev->dev_addr, ETH_ALEN) != 0)
		return -EINVAL;

	/* Transmit the Action frame as requested by user space */
	return rdev->ops->mgmt_tx(&rdev->wiphy, dev, chan, offchan,
				  channel_type, channel_type_valid,
				  wait, buf, len, no_cck, dont_wait_for_ack,
				  cookie);
}

bool cfg80211_rx_mgmt(struct net_device *dev, int freq, int sig_mbm,
		      const u8 *buf, size_t len, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct cfg80211_mgmt_registration *reg;
	const struct ieee80211_txrx_stypes *stypes =
		&wiphy->mgmt_stypes[wdev->iftype];
	struct ieee80211_mgmt *mgmt = (void *)buf;
=======
	if (!cfg80211_allowed_address(wdev, mgmt->sa) &&
	    !cfg80211_allowed_random_address(wdev, mgmt))
		return -EINVAL;

	/* Transmit the management frame as requested by user space */
	return rdev_mgmt_tx(rdev, wdev, params, cookie);
}

bool cfg80211_rx_mgmt_ext(struct wireless_dev *wdev,
			  struct cfg80211_rx_info *info)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);
	struct cfg80211_mgmt_registration *reg;
	const struct ieee80211_txrx_stypes *stypes =
		&wiphy->mgmt_stypes[wdev->iftype];
	struct ieee80211_mgmt *mgmt = (void *)info->buf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const u8 *data;
	int data_len;
	bool result = false;
	__le16 ftype = mgmt->frame_control &
		cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE);
	u16 stype;

<<<<<<< HEAD
	stype = (le16_to_cpu(mgmt->frame_control) & IEEE80211_FCTL_STYPE) >> 4;

	if (!(stypes->rx & BIT(stype)))
		return false;

	data = buf + ieee80211_hdrlen(mgmt->frame_control);
	data_len = len - ieee80211_hdrlen(mgmt->frame_control);

	spin_lock_bh(&wdev->mgmt_registrations_lock);
=======
	trace_cfg80211_rx_mgmt(wdev, info);
	stype = (le16_to_cpu(mgmt->frame_control) & IEEE80211_FCTL_STYPE) >> 4;

	if (!(stypes->rx & BIT(stype))) {
		trace_cfg80211_return_bool(false);
		return false;
	}

	data = info->buf + ieee80211_hdrlen(mgmt->frame_control);
	data_len = info->len - ieee80211_hdrlen(mgmt->frame_control);

	spin_lock_bh(&rdev->mgmt_registrations_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	list_for_each_entry(reg, &wdev->mgmt_registrations, list) {
		if (reg->frame_type != ftype)
			continue;

		if (reg->match_len > data_len)
			continue;

		if (memcmp(reg->match, data, reg->match_len))
			continue;

		/* found match! */

		/* Indicate the received Action frame to user space */
<<<<<<< HEAD
		if (nl80211_send_mgmt(rdev, dev, reg->nlpid,
				      freq, sig_mbm,
				      buf, len, gfp))
=======
		if (nl80211_send_mgmt(rdev, wdev, reg->nlportid, info,
				      GFP_ATOMIC))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;

		result = true;
		break;
	}

<<<<<<< HEAD
	spin_unlock_bh(&wdev->mgmt_registrations_lock);

	return result;
}
EXPORT_SYMBOL(cfg80211_rx_mgmt);

void cfg80211_mgmt_tx_status(struct net_device *dev, u64 cookie,
			     const u8 *buf, size_t len, bool ack, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	/* Indicate TX status of the Action frame to user space */
	nl80211_send_mgmt_tx_status(rdev, dev, cookie, buf, len, ack, gfp);
}
EXPORT_SYMBOL(cfg80211_mgmt_tx_status);

void cfg80211_cqm_rssi_notify(struct net_device *dev,
			      enum nl80211_cqm_rssi_threshold_event rssi_event,
			      gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	/* Indicate roaming trigger event to user space */
	nl80211_send_cqm_rssi_notify(rdev, dev, rssi_event, gfp);
}
EXPORT_SYMBOL(cfg80211_cqm_rssi_notify);

void cfg80211_cqm_pktloss_notify(struct net_device *dev,
				 const u8 *peer, u32 num_packets, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	/* Indicate roaming trigger event to user space */
	nl80211_send_cqm_pktloss_notify(rdev, dev, peer, num_packets, gfp);
}
EXPORT_SYMBOL(cfg80211_cqm_pktloss_notify);

void cfg80211_gtk_rekey_notify(struct net_device *dev, const u8 *bssid,
			       const u8 *replay_ctr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	nl80211_gtk_rekey_notify(rdev, dev, bssid, replay_ctr, gfp);
}
EXPORT_SYMBOL(cfg80211_gtk_rekey_notify);

void cfg80211_pmksa_candidate_notify(struct net_device *dev, int index,
				     const u8 *bssid, bool preauth, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	nl80211_pmksa_candidate_notify(rdev, dev, index, bssid, preauth, gfp);
}
EXPORT_SYMBOL(cfg80211_pmksa_candidate_notify);

bool cfg80211_rx_spurious_frame(struct net_device *dev,
				const u8 *addr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_AP &&
		    wdev->iftype != NL80211_IFTYPE_P2P_GO))
		return false;

	return nl80211_unexpected_frame(dev, addr, gfp);
}
EXPORT_SYMBOL(cfg80211_rx_spurious_frame);

bool cfg80211_rx_unexpected_4addr_frame(struct net_device *dev,
					const u8 *addr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_AP &&
		    wdev->iftype != NL80211_IFTYPE_P2P_GO &&
		    wdev->iftype != NL80211_IFTYPE_AP_VLAN))
		return false;

	return nl80211_unexpected_4addr_frame(dev, addr, gfp);
}
EXPORT_SYMBOL(cfg80211_rx_unexpected_4addr_frame);
=======
	spin_unlock_bh(&rdev->mgmt_registrations_lock);

	trace_cfg80211_return_bool(result);
	return result;
}
EXPORT_SYMBOL(cfg80211_rx_mgmt_ext);

void cfg80211_sched_dfs_chan_update(struct cfg80211_registered_device *rdev)
{
	cancel_delayed_work(&rdev->dfs_update_channels_wk);
	queue_delayed_work(cfg80211_wq, &rdev->dfs_update_channels_wk, 0);
}

void cfg80211_dfs_channels_update_work(struct work_struct *work)
{
	struct delayed_work *delayed_work = to_delayed_work(work);
	struct cfg80211_registered_device *rdev;
	struct cfg80211_chan_def chandef;
	struct ieee80211_supported_band *sband;
	struct ieee80211_channel *c;
	struct wiphy *wiphy;
	bool check_again = false;
	unsigned long timeout, next_time = 0;
	unsigned long time_dfs_update;
	enum nl80211_radar_event radar_event;
	int bandid, i;

	rdev = container_of(delayed_work, struct cfg80211_registered_device,
			    dfs_update_channels_wk);
	wiphy = &rdev->wiphy;

	rtnl_lock();
	for (bandid = 0; bandid < NUM_NL80211_BANDS; bandid++) {
		sband = wiphy->bands[bandid];
		if (!sband)
			continue;

		for (i = 0; i < sband->n_channels; i++) {
			c = &sband->channels[i];

			if (!(c->flags & IEEE80211_CHAN_RADAR))
				continue;

			if (c->dfs_state != NL80211_DFS_UNAVAILABLE &&
			    c->dfs_state != NL80211_DFS_AVAILABLE)
				continue;

			if (c->dfs_state == NL80211_DFS_UNAVAILABLE) {
				time_dfs_update = IEEE80211_DFS_MIN_NOP_TIME_MS;
				radar_event = NL80211_RADAR_NOP_FINISHED;
			} else {
				if (regulatory_pre_cac_allowed(wiphy) ||
				    cfg80211_any_wiphy_oper_chan(wiphy, c))
					continue;

				time_dfs_update = REG_PRE_CAC_EXPIRY_GRACE_MS;
				radar_event = NL80211_RADAR_PRE_CAC_EXPIRED;
			}

			timeout = c->dfs_state_entered +
				  msecs_to_jiffies(time_dfs_update);

			if (time_after_eq(jiffies, timeout)) {
				c->dfs_state = NL80211_DFS_USABLE;
				c->dfs_state_entered = jiffies;

				cfg80211_chandef_create(&chandef, c,
							NL80211_CHAN_NO_HT);

				nl80211_radar_notify(rdev, &chandef,
						     radar_event, NULL,
						     GFP_ATOMIC);

				regulatory_propagate_dfs_state(wiphy, &chandef,
							       c->dfs_state,
							       radar_event);
				continue;
			}

			if (!check_again)
				next_time = timeout - jiffies;
			else
				next_time = min(next_time, timeout - jiffies);
			check_again = true;
		}
	}
	rtnl_unlock();

	/* reschedule if there are other channels waiting to be cleared again */
	if (check_again)
		queue_delayed_work(cfg80211_wq, &rdev->dfs_update_channels_wk,
				   next_time);
}


void __cfg80211_radar_event(struct wiphy *wiphy,
			    struct cfg80211_chan_def *chandef,
			    bool offchan, gfp_t gfp)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);

	trace_cfg80211_radar_event(wiphy, chandef, offchan);

	/* only set the chandef supplied channel to unavailable, in
	 * case the radar is detected on only one of multiple channels
	 * spanned by the chandef.
	 */
	cfg80211_set_dfs_state(wiphy, chandef, NL80211_DFS_UNAVAILABLE);

	if (offchan)
		queue_work(cfg80211_wq, &rdev->background_cac_abort_wk);

	cfg80211_sched_dfs_chan_update(rdev);

	nl80211_radar_notify(rdev, chandef, NL80211_RADAR_DETECTED, NULL, gfp);

	memcpy(&rdev->radar_chandef, chandef, sizeof(struct cfg80211_chan_def));
	queue_work(cfg80211_wq, &rdev->propagate_radar_detect_wk);
}
EXPORT_SYMBOL(__cfg80211_radar_event);

void cfg80211_cac_event(struct net_device *netdev,
			const struct cfg80211_chan_def *chandef,
			enum nl80211_radar_event event, gfp_t gfp)
{
	struct wireless_dev *wdev = netdev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);
	unsigned long timeout;

	/* not yet supported */
	if (wdev->valid_links)
		return;

	trace_cfg80211_cac_event(netdev, event);

	if (WARN_ON(!wdev->cac_started && event != NL80211_RADAR_CAC_STARTED))
		return;

	switch (event) {
	case NL80211_RADAR_CAC_FINISHED:
		timeout = wdev->cac_start_time +
			  msecs_to_jiffies(wdev->cac_time_ms);
		WARN_ON(!time_after_eq(jiffies, timeout));
		cfg80211_set_dfs_state(wiphy, chandef, NL80211_DFS_AVAILABLE);
		memcpy(&rdev->cac_done_chandef, chandef,
		       sizeof(struct cfg80211_chan_def));
		queue_work(cfg80211_wq, &rdev->propagate_cac_done_wk);
		cfg80211_sched_dfs_chan_update(rdev);
		fallthrough;
	case NL80211_RADAR_CAC_ABORTED:
		wdev->cac_started = false;
		break;
	case NL80211_RADAR_CAC_STARTED:
		wdev->cac_started = true;
		break;
	default:
		WARN_ON(1);
		return;
	}

	nl80211_radar_notify(rdev, chandef, event, netdev, gfp);
}
EXPORT_SYMBOL(cfg80211_cac_event);

static void
__cfg80211_background_cac_event(struct cfg80211_registered_device *rdev,
				struct wireless_dev *wdev,
				const struct cfg80211_chan_def *chandef,
				enum nl80211_radar_event event)
{
	struct wiphy *wiphy = &rdev->wiphy;
	struct net_device *netdev;

	lockdep_assert_wiphy(&rdev->wiphy);

	if (!cfg80211_chandef_valid(chandef))
		return;

	if (!rdev->background_radar_wdev)
		return;

	switch (event) {
	case NL80211_RADAR_CAC_FINISHED:
		cfg80211_set_dfs_state(wiphy, chandef, NL80211_DFS_AVAILABLE);
		memcpy(&rdev->cac_done_chandef, chandef, sizeof(*chandef));
		queue_work(cfg80211_wq, &rdev->propagate_cac_done_wk);
		cfg80211_sched_dfs_chan_update(rdev);
		wdev = rdev->background_radar_wdev;
		break;
	case NL80211_RADAR_CAC_ABORTED:
		if (!cancel_delayed_work(&rdev->background_cac_done_wk))
			return;
		wdev = rdev->background_radar_wdev;
		break;
	case NL80211_RADAR_CAC_STARTED:
		break;
	default:
		return;
	}

	netdev = wdev ? wdev->netdev : NULL;
	nl80211_radar_notify(rdev, chandef, event, netdev, GFP_KERNEL);
}

static void
cfg80211_background_cac_event(struct cfg80211_registered_device *rdev,
			      const struct cfg80211_chan_def *chandef,
			      enum nl80211_radar_event event)
{
	wiphy_lock(&rdev->wiphy);
	__cfg80211_background_cac_event(rdev, rdev->background_radar_wdev,
					chandef, event);
	wiphy_unlock(&rdev->wiphy);
}

void cfg80211_background_cac_done_wk(struct work_struct *work)
{
	struct delayed_work *delayed_work = to_delayed_work(work);
	struct cfg80211_registered_device *rdev;

	rdev = container_of(delayed_work, struct cfg80211_registered_device,
			    background_cac_done_wk);
	cfg80211_background_cac_event(rdev, &rdev->background_radar_chandef,
				      NL80211_RADAR_CAC_FINISHED);
}

void cfg80211_background_cac_abort_wk(struct work_struct *work)
{
	struct cfg80211_registered_device *rdev;

	rdev = container_of(work, struct cfg80211_registered_device,
			    background_cac_abort_wk);
	cfg80211_background_cac_event(rdev, &rdev->background_radar_chandef,
				      NL80211_RADAR_CAC_ABORTED);
}

void cfg80211_background_cac_abort(struct wiphy *wiphy)
{
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);

	queue_work(cfg80211_wq, &rdev->background_cac_abort_wk);
}
EXPORT_SYMBOL(cfg80211_background_cac_abort);

int
cfg80211_start_background_radar_detection(struct cfg80211_registered_device *rdev,
					  struct wireless_dev *wdev,
					  struct cfg80211_chan_def *chandef)
{
	unsigned int cac_time_ms;
	int err;

	lockdep_assert_wiphy(&rdev->wiphy);

	if (!wiphy_ext_feature_isset(&rdev->wiphy,
				     NL80211_EXT_FEATURE_RADAR_BACKGROUND))
		return -EOPNOTSUPP;

	/* Offchannel chain already locked by another wdev */
	if (rdev->background_radar_wdev && rdev->background_radar_wdev != wdev)
		return -EBUSY;

	/* CAC already in progress on the offchannel chain */
	if (rdev->background_radar_wdev == wdev &&
	    delayed_work_pending(&rdev->background_cac_done_wk))
		return -EBUSY;

	err = rdev_set_radar_background(rdev, chandef);
	if (err)
		return err;

	cac_time_ms = cfg80211_chandef_dfs_cac_time(&rdev->wiphy, chandef);
	if (!cac_time_ms)
		cac_time_ms = IEEE80211_DFS_MIN_CAC_TIME_MS;

	rdev->background_radar_chandef = *chandef;
	rdev->background_radar_wdev = wdev; /* Get offchain ownership */

	__cfg80211_background_cac_event(rdev, wdev, chandef,
					NL80211_RADAR_CAC_STARTED);
	queue_delayed_work(cfg80211_wq, &rdev->background_cac_done_wk,
			   msecs_to_jiffies(cac_time_ms));

	return 0;
}

void cfg80211_stop_background_radar_detection(struct wireless_dev *wdev)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_rdev(wiphy);

	lockdep_assert_wiphy(wiphy);

	if (wdev != rdev->background_radar_wdev)
		return;

	rdev_set_radar_background(rdev, NULL);
	rdev->background_radar_wdev = NULL; /* Release offchain ownership */

	__cfg80211_background_cac_event(rdev, wdev,
					&rdev->background_radar_chandef,
					NL80211_RADAR_CAC_ABORTED);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
