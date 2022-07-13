<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * mac80211 configuration hooks for cfg80211
 *
 * Copyright 2006-2010	Johannes Berg <johannes@sipsolutions.net>
<<<<<<< HEAD
 *
 * This file is GPLv2 as found in COPYING.
=======
 * Copyright 2013-2015  Intel Mobile Communications GmbH
 * Copyright (C) 2015-2017 Intel Deutschland GmbH
 * Copyright (C) 2018-2024 Intel Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/ieee80211.h>
#include <linux/nl80211.h>
#include <linux/rtnetlink.h>
#include <linux/slab.h>
#include <net/net_namespace.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD
=======
#include <linux/fips.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/if_ether.h>
#include <net/cfg80211.h>
#include "ieee80211_i.h"
#include "driver-ops.h"
<<<<<<< HEAD
#include "cfg.h"
#include "rate.h"
#include "mesh.h"

static struct net_device *ieee80211_add_iface(struct wiphy *wiphy, char *name,
					      enum nl80211_iftype type,
					      u32 *flags,
					      struct vif_params *params)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct net_device *dev;
	struct ieee80211_sub_if_data *sdata;
	int err;

	err = ieee80211_if_add(local, name, &dev, type, params);
	if (err)
		return ERR_PTR(err);

	if (type == NL80211_IFTYPE_MONITOR && flags) {
		sdata = IEEE80211_DEV_TO_SUB_IF(dev);
		sdata->u.mntr_flags = *flags;
	}

	return dev;
}

static int ieee80211_del_iface(struct wiphy *wiphy, struct net_device *dev)
{
	ieee80211_if_remove(IEEE80211_DEV_TO_SUB_IF(dev));

	return 0;
}

static int ieee80211_change_iface(struct wiphy *wiphy,
				  struct net_device *dev,
				  enum nl80211_iftype type, u32 *flags,
				  struct vif_params *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	int ret;

	ret = ieee80211_if_change_type(sdata, type);
	if (ret)
		return ret;

	if (type == NL80211_IFTYPE_AP_VLAN &&
	    params && params->use_4addr == 0)
		RCU_INIT_POINTER(sdata->u.vlan.sta, NULL);
	else if (type == NL80211_IFTYPE_STATION &&
		 params && params->use_4addr >= 0)
		sdata->u.mgd.use_4addr = params->use_4addr;

	if (sdata->vif.type == NL80211_IFTYPE_MONITOR && flags) {
		struct ieee80211_local *local = sdata->local;

		if (ieee80211_sdata_running(sdata)) {
			/*
			 * Prohibit MONITOR_FLAG_COOK_FRAMES to be
			 * changed while the interface is up.
			 * Else we would need to add a lot of cruft
			 * to update everything:
			 *	cooked_mntrs, monitor and all fif_* counters
			 *	reconfigure hardware
			 */
			if ((*flags & MONITOR_FLAG_COOK_FRAMES) !=
			    (sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES))
				return -EBUSY;

			ieee80211_adjust_monitor_flags(sdata, -1);
			sdata->u.mntr_flags = *flags;
=======
#include "rate.h"
#include "mesh.h"
#include "wme.h"

static struct ieee80211_link_data *
ieee80211_link_or_deflink(struct ieee80211_sub_if_data *sdata, int link_id,
			  bool require_valid)
{
	struct ieee80211_link_data *link;

	if (link_id < 0) {
		/*
		 * For keys, if sdata is not an MLD, we might not use
		 * the return value at all (if it's not a pairwise key),
		 * so in that case (require_valid==false) don't error.
		 */
		if (require_valid && ieee80211_vif_is_mld(&sdata->vif))
			return ERR_PTR(-EINVAL);

		return &sdata->deflink;
	}

	link = sdata_dereference(sdata->link[link_id], sdata);
	if (!link)
		return ERR_PTR(-ENOLINK);
	return link;
}

static void ieee80211_set_mu_mimo_follow(struct ieee80211_sub_if_data *sdata,
					 struct vif_params *params)
{
	bool mu_mimo_groups = false;
	bool mu_mimo_follow = false;

	if (params->vht_mumimo_groups) {
		u64 membership;

		BUILD_BUG_ON(sizeof(membership) != WLAN_MEMBERSHIP_LEN);

		memcpy(sdata->vif.bss_conf.mu_group.membership,
		       params->vht_mumimo_groups, WLAN_MEMBERSHIP_LEN);
		memcpy(sdata->vif.bss_conf.mu_group.position,
		       params->vht_mumimo_groups + WLAN_MEMBERSHIP_LEN,
		       WLAN_USER_POSITION_LEN);
		ieee80211_link_info_change_notify(sdata, &sdata->deflink,
						  BSS_CHANGED_MU_GROUPS);
		/* don't care about endianness - just check for 0 */
		memcpy(&membership, params->vht_mumimo_groups,
		       WLAN_MEMBERSHIP_LEN);
		mu_mimo_groups = membership != 0;
	}

	if (params->vht_mumimo_follow_addr) {
		mu_mimo_follow =
			is_valid_ether_addr(params->vht_mumimo_follow_addr);
		ether_addr_copy(sdata->u.mntr.mu_follow_addr,
				params->vht_mumimo_follow_addr);
	}

	sdata->vif.bss_conf.mu_mimo_owner = mu_mimo_groups || mu_mimo_follow;
}

static int ieee80211_set_mon_options(struct ieee80211_sub_if_data *sdata,
				     struct vif_params *params)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_sub_if_data *monitor_sdata;

	/* check flags first */
	if (params->flags && ieee80211_sdata_running(sdata)) {
		u32 mask = MONITOR_FLAG_COOK_FRAMES | MONITOR_FLAG_ACTIVE;

		/*
		 * Prohibit MONITOR_FLAG_COOK_FRAMES and
		 * MONITOR_FLAG_ACTIVE to be changed while the
		 * interface is up.
		 * Else we would need to add a lot of cruft
		 * to update everything:
		 *	cooked_mntrs, monitor and all fif_* counters
		 *	reconfigure hardware
		 */
		if ((params->flags & mask) != (sdata->u.mntr.flags & mask))
			return -EBUSY;
	}

	/* also validate MU-MIMO change */
	monitor_sdata = wiphy_dereference(local->hw.wiphy,
					  local->monitor_sdata);

	if (!monitor_sdata &&
	    (params->vht_mumimo_groups || params->vht_mumimo_follow_addr))
		return -EOPNOTSUPP;

	/* apply all changes now - no failures allowed */

	if (monitor_sdata)
		ieee80211_set_mu_mimo_follow(monitor_sdata, params);

	if (params->flags) {
		if (ieee80211_sdata_running(sdata)) {
			ieee80211_adjust_monitor_flags(sdata, -1);
			sdata->u.mntr.flags = params->flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ieee80211_adjust_monitor_flags(sdata, 1);

			ieee80211_configure_filter(local);
		} else {
			/*
			 * Because the interface is down, ieee80211_do_stop
			 * and ieee80211_do_open take care of "everything"
			 * mentioned in the comment above.
			 */
<<<<<<< HEAD
			sdata->u.mntr_flags = *flags;
=======
			sdata->u.mntr.flags = params->flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	return 0;
}

<<<<<<< HEAD
=======
static int ieee80211_set_ap_mbssid_options(struct ieee80211_sub_if_data *sdata,
					   struct cfg80211_mbssid_config params,
					   struct ieee80211_bss_conf *link_conf)
{
	struct ieee80211_sub_if_data *tx_sdata;

	sdata->vif.mbssid_tx_vif = NULL;
	link_conf->bssid_index = 0;
	link_conf->nontransmitted = false;
	link_conf->ema_ap = false;
	link_conf->bssid_indicator = 0;

	if (sdata->vif.type != NL80211_IFTYPE_AP || !params.tx_wdev)
		return -EINVAL;

	tx_sdata = IEEE80211_WDEV_TO_SUB_IF(params.tx_wdev);
	if (!tx_sdata)
		return -EINVAL;

	if (tx_sdata == sdata) {
		sdata->vif.mbssid_tx_vif = &sdata->vif;
	} else {
		sdata->vif.mbssid_tx_vif = &tx_sdata->vif;
		link_conf->nontransmitted = true;
		link_conf->bssid_index = params.index;
	}
	if (params.ema)
		link_conf->ema_ap = true;

	return 0;
}

static struct wireless_dev *ieee80211_add_iface(struct wiphy *wiphy,
						const char *name,
						unsigned char name_assign_type,
						enum nl80211_iftype type,
						struct vif_params *params)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct wireless_dev *wdev;
	struct ieee80211_sub_if_data *sdata;
	int err;

	err = ieee80211_if_add(local, name, name_assign_type, &wdev, type, params);
	if (err)
		return ERR_PTR(err);

	sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);

	if (type == NL80211_IFTYPE_MONITOR) {
		err = ieee80211_set_mon_options(sdata, params);
		if (err) {
			ieee80211_if_remove(sdata);
			return NULL;
		}
	}

	return wdev;
}

static int ieee80211_del_iface(struct wiphy *wiphy, struct wireless_dev *wdev)
{
	ieee80211_if_remove(IEEE80211_WDEV_TO_SUB_IF(wdev));

	return 0;
}

static int ieee80211_change_iface(struct wiphy *wiphy,
				  struct net_device *dev,
				  enum nl80211_iftype type,
				  struct vif_params *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
	int ret;

	lockdep_assert_wiphy(local->hw.wiphy);

	ret = ieee80211_if_change_type(sdata, type);
	if (ret)
		return ret;

	if (type == NL80211_IFTYPE_AP_VLAN && params->use_4addr == 0) {
		RCU_INIT_POINTER(sdata->u.vlan.sta, NULL);
		ieee80211_check_fast_rx_iface(sdata);
	} else if (type == NL80211_IFTYPE_STATION && params->use_4addr >= 0) {
		struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

		if (params->use_4addr == ifmgd->use_4addr)
			return 0;

		/* FIXME: no support for 4-addr MLO yet */
		if (ieee80211_vif_is_mld(&sdata->vif))
			return -EOPNOTSUPP;

		sdata->u.mgd.use_4addr = params->use_4addr;
		if (!ifmgd->associated)
			return 0;

		sta = sta_info_get(sdata, sdata->deflink.u.mgd.bssid);
		if (sta)
			drv_sta_set_4addr(local, sdata, &sta->sta,
					  params->use_4addr);

		if (params->use_4addr)
			ieee80211_send_4addr_nullfunc(local, sdata);
	}

	if (sdata->vif.type == NL80211_IFTYPE_MONITOR) {
		ret = ieee80211_set_mon_options(sdata, params);
		if (ret)
			return ret;
	}

	return 0;
}

static int ieee80211_start_p2p_device(struct wiphy *wiphy,
				      struct wireless_dev *wdev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);
	int ret;

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	ret = ieee80211_check_combinations(sdata, NULL, 0, 0);
	if (ret < 0)
		return ret;

	return ieee80211_do_open(wdev, true);
}

static void ieee80211_stop_p2p_device(struct wiphy *wiphy,
				      struct wireless_dev *wdev)
{
	ieee80211_sdata_stop(IEEE80211_WDEV_TO_SUB_IF(wdev));
}

static int ieee80211_start_nan(struct wiphy *wiphy,
			       struct wireless_dev *wdev,
			       struct cfg80211_nan_conf *conf)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);
	int ret;

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	ret = ieee80211_check_combinations(sdata, NULL, 0, 0);
	if (ret < 0)
		return ret;

	ret = ieee80211_do_open(wdev, true);
	if (ret)
		return ret;

	ret = drv_start_nan(sdata->local, sdata, conf);
	if (ret)
		ieee80211_sdata_stop(sdata);

	sdata->u.nan.conf = *conf;

	return ret;
}

static void ieee80211_stop_nan(struct wiphy *wiphy,
			       struct wireless_dev *wdev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);

	drv_stop_nan(sdata->local, sdata);
	ieee80211_sdata_stop(sdata);
}

static int ieee80211_nan_change_conf(struct wiphy *wiphy,
				     struct wireless_dev *wdev,
				     struct cfg80211_nan_conf *conf,
				     u32 changes)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);
	struct cfg80211_nan_conf new_conf;
	int ret = 0;

	if (sdata->vif.type != NL80211_IFTYPE_NAN)
		return -EOPNOTSUPP;

	if (!ieee80211_sdata_running(sdata))
		return -ENETDOWN;

	new_conf = sdata->u.nan.conf;

	if (changes & CFG80211_NAN_CONF_CHANGED_PREF)
		new_conf.master_pref = conf->master_pref;

	if (changes & CFG80211_NAN_CONF_CHANGED_BANDS)
		new_conf.bands = conf->bands;

	ret = drv_nan_change_conf(sdata->local, sdata, &new_conf, changes);
	if (!ret)
		sdata->u.nan.conf = new_conf;

	return ret;
}

static int ieee80211_add_nan_func(struct wiphy *wiphy,
				  struct wireless_dev *wdev,
				  struct cfg80211_nan_func *nan_func)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);
	int ret;

	if (sdata->vif.type != NL80211_IFTYPE_NAN)
		return -EOPNOTSUPP;

	if (!ieee80211_sdata_running(sdata))
		return -ENETDOWN;

	spin_lock_bh(&sdata->u.nan.func_lock);

	ret = idr_alloc(&sdata->u.nan.function_inst_ids,
			nan_func, 1, sdata->local->hw.max_nan_de_entries + 1,
			GFP_ATOMIC);
	spin_unlock_bh(&sdata->u.nan.func_lock);

	if (ret < 0)
		return ret;

	nan_func->instance_id = ret;

	WARN_ON(nan_func->instance_id == 0);

	ret = drv_add_nan_func(sdata->local, sdata, nan_func);
	if (ret) {
		spin_lock_bh(&sdata->u.nan.func_lock);
		idr_remove(&sdata->u.nan.function_inst_ids,
			   nan_func->instance_id);
		spin_unlock_bh(&sdata->u.nan.func_lock);
	}

	return ret;
}

static struct cfg80211_nan_func *
ieee80211_find_nan_func_by_cookie(struct ieee80211_sub_if_data *sdata,
				  u64 cookie)
{
	struct cfg80211_nan_func *func;
	int id;

	lockdep_assert_held(&sdata->u.nan.func_lock);

	idr_for_each_entry(&sdata->u.nan.function_inst_ids, func, id) {
		if (func->cookie == cookie)
			return func;
	}

	return NULL;
}

static void ieee80211_del_nan_func(struct wiphy *wiphy,
				  struct wireless_dev *wdev, u64 cookie)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);
	struct cfg80211_nan_func *func;
	u8 instance_id = 0;

	if (sdata->vif.type != NL80211_IFTYPE_NAN ||
	    !ieee80211_sdata_running(sdata))
		return;

	spin_lock_bh(&sdata->u.nan.func_lock);

	func = ieee80211_find_nan_func_by_cookie(sdata, cookie);
	if (func)
		instance_id = func->instance_id;

	spin_unlock_bh(&sdata->u.nan.func_lock);

	if (instance_id)
		drv_del_nan_func(sdata->local, sdata, instance_id);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ieee80211_set_noack_map(struct wiphy *wiphy,
				  struct net_device *dev,
				  u16 noack_map)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	sdata->noack_map = noack_map;
<<<<<<< HEAD
	return 0;
}

static int ieee80211_add_key(struct wiphy *wiphy, struct net_device *dev,
			     u8 key_idx, bool pairwise, const u8 *mac_addr,
			     struct key_params *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
=======

	ieee80211_check_fast_xmit_iface(sdata);

	return 0;
}

static int ieee80211_set_tx(struct ieee80211_sub_if_data *sdata,
			    const u8 *mac_addr, u8 key_idx)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_key *key;
	struct sta_info *sta;
	int ret = -EINVAL;

	if (!wiphy_ext_feature_isset(local->hw.wiphy,
				     NL80211_EXT_FEATURE_EXT_KEY_ID))
		return -EINVAL;

	sta = sta_info_get_bss(sdata, mac_addr);

	if (!sta)
		return -EINVAL;

	if (sta->ptk_idx == key_idx)
		return 0;

	key = wiphy_dereference(local->hw.wiphy, sta->ptk[key_idx]);

	if (key && key->conf.flags & IEEE80211_KEY_FLAG_NO_AUTO_TX)
		ret = ieee80211_set_tx_key(key);

	return ret;
}

static int ieee80211_add_key(struct wiphy *wiphy, struct net_device *dev,
			     int link_id, u8 key_idx, bool pairwise,
			     const u8 *mac_addr, struct key_params *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_link_data *link =
		ieee80211_link_or_deflink(sdata, link_id, false);
	struct ieee80211_local *local = sdata->local;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sta_info *sta = NULL;
	struct ieee80211_key *key;
	int err;

<<<<<<< HEAD
	if (!ieee80211_sdata_running(sdata))
		return -ENETDOWN;

=======
	lockdep_assert_wiphy(local->hw.wiphy);

	if (!ieee80211_sdata_running(sdata))
		return -ENETDOWN;

	if (IS_ERR(link))
		return PTR_ERR(link);

	if (pairwise && params->mode == NL80211_KEY_SET_TX)
		return ieee80211_set_tx(sdata, mac_addr, key_idx);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* reject WEP and TKIP keys if WEP failed to initialize */
	switch (params->cipher) {
	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_TKIP:
	case WLAN_CIPHER_SUITE_WEP104:
<<<<<<< HEAD
		if (IS_ERR(sdata->local->wep_tx_tfm))
=======
		if (link_id >= 0)
			return -EINVAL;
		if (WARN_ON_ONCE(fips_enabled))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		break;
	default:
		break;
	}

	key = ieee80211_key_alloc(params->cipher, key_idx, params->key_len,
				  params->key, params->seq_len, params->seq);
	if (IS_ERR(key))
		return PTR_ERR(key);

<<<<<<< HEAD
	if (pairwise)
		key->conf.flags |= IEEE80211_KEY_FLAG_PAIRWISE;

	mutex_lock(&sdata->local->sta_mtx);

	if (mac_addr) {
		if (ieee80211_vif_is_mesh(&sdata->vif))
			sta = sta_info_get(sdata, mac_addr);
		else
			sta = sta_info_get_bss(sdata, mac_addr);
=======
	key->conf.link_id = link_id;

	if (pairwise)
		key->conf.flags |= IEEE80211_KEY_FLAG_PAIRWISE;

	if (params->mode == NL80211_KEY_NO_TX)
		key->conf.flags |= IEEE80211_KEY_FLAG_NO_AUTO_TX;

	if (mac_addr) {
		sta = sta_info_get_bss(sdata, mac_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * The ASSOC test makes sure the driver is ready to
		 * receive the key. When wpa_supplicant has roamed
		 * using FT, it attempts to set the key before
		 * association has completed, this rejects that attempt
<<<<<<< HEAD
		 * so it will set the key again after assocation.
=======
		 * so it will set the key again after association.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 *
		 * TODO: accept the key if we have a station entry and
		 *       add it to the device after the station.
		 */
		if (!sta || !test_sta_flag(sta, WLAN_STA_ASSOC)) {
<<<<<<< HEAD
			ieee80211_key_free(sdata->local, key);
			err = -ENOENT;
			goto out_unlock;
		}
	}

	err = ieee80211_key_link(key, sdata, sta);
	if (err)
		ieee80211_key_free(sdata->local, key);

 out_unlock:
	mutex_unlock(&sdata->local->sta_mtx);
=======
			ieee80211_key_free_unused(key);
			return -ENOENT;
		}
	}

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_STATION:
		if (sdata->u.mgd.mfp != IEEE80211_MFP_DISABLED)
			key->conf.flags |= IEEE80211_KEY_FLAG_RX_MGMT;
		break;
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
		/* Keys without a station are used for TX only */
		if (sta && test_sta_flag(sta, WLAN_STA_MFP))
			key->conf.flags |= IEEE80211_KEY_FLAG_RX_MGMT;
		break;
	case NL80211_IFTYPE_ADHOC:
		/* no MFP (yet) */
		break;
	case NL80211_IFTYPE_MESH_POINT:
#ifdef CONFIG_MAC80211_MESH
		if (sdata->u.mesh.security != IEEE80211_MESH_SEC_NONE)
			key->conf.flags |= IEEE80211_KEY_FLAG_RX_MGMT;
		break;
#endif
	case NL80211_IFTYPE_WDS:
	case NL80211_IFTYPE_MONITOR:
	case NL80211_IFTYPE_P2P_DEVICE:
	case NL80211_IFTYPE_NAN:
	case NL80211_IFTYPE_UNSPECIFIED:
	case NUM_NL80211_IFTYPES:
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_P2P_GO:
	case NL80211_IFTYPE_OCB:
		/* shouldn't happen */
		WARN_ON_ONCE(1);
		break;
	}

	err = ieee80211_key_link(key, link, sta);
	/* KRACK protection, shouldn't happen but just silently accept key */
	if (err == -EALREADY)
		err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
static int ieee80211_del_key(struct wiphy *wiphy, struct net_device *dev,
			     u8 key_idx, bool pairwise, const u8 *mac_addr)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
	struct ieee80211_key *key = NULL;
	int ret;

	mutex_lock(&local->sta_mtx);
	mutex_lock(&local->key_mtx);

	if (mac_addr) {
		ret = -ENOENT;

		sta = sta_info_get_bss(sdata, mac_addr);
		if (!sta)
			goto out_unlock;

		if (pairwise)
			key = key_mtx_dereference(local, sta->ptk);
		else
			key = key_mtx_dereference(local, sta->gtk[key_idx]);
	} else
		key = key_mtx_dereference(local, sdata->keys[key_idx]);

	if (!key) {
		ret = -ENOENT;
		goto out_unlock;
	}

	__ieee80211_key_free(key);

	ret = 0;
 out_unlock:
	mutex_unlock(&local->key_mtx);
	mutex_unlock(&local->sta_mtx);

	return ret;
}

static int ieee80211_get_key(struct wiphy *wiphy, struct net_device *dev,
			     u8 key_idx, bool pairwise, const u8 *mac_addr,
			     void *cookie,
=======
static struct ieee80211_key *
ieee80211_lookup_key(struct ieee80211_sub_if_data *sdata, int link_id,
		     u8 key_idx, bool pairwise, const u8 *mac_addr)
{
	struct ieee80211_local *local __maybe_unused = sdata->local;
	struct ieee80211_link_data *link = &sdata->deflink;
	struct ieee80211_key *key;

	if (link_id >= 0) {
		link = sdata_dereference(sdata->link[link_id], sdata);
		if (!link)
			return NULL;
	}

	if (mac_addr) {
		struct sta_info *sta;
		struct link_sta_info *link_sta;

		sta = sta_info_get_bss(sdata, mac_addr);
		if (!sta)
			return NULL;

		if (link_id >= 0) {
			link_sta = rcu_dereference_check(sta->link[link_id],
							 lockdep_is_held(&local->hw.wiphy->mtx));
			if (!link_sta)
				return NULL;
		} else {
			link_sta = &sta->deflink;
		}

		if (pairwise && key_idx < NUM_DEFAULT_KEYS)
			return wiphy_dereference(local->hw.wiphy,
						 sta->ptk[key_idx]);

		if (!pairwise &&
		    key_idx < NUM_DEFAULT_KEYS +
			      NUM_DEFAULT_MGMT_KEYS +
			      NUM_DEFAULT_BEACON_KEYS)
			return wiphy_dereference(local->hw.wiphy,
						 link_sta->gtk[key_idx]);

		return NULL;
	}

	if (pairwise && key_idx < NUM_DEFAULT_KEYS)
		return wiphy_dereference(local->hw.wiphy, sdata->keys[key_idx]);

	key = wiphy_dereference(local->hw.wiphy, link->gtk[key_idx]);
	if (key)
		return key;

	/* or maybe it was a WEP key */
	if (key_idx < NUM_DEFAULT_KEYS)
		return wiphy_dereference(local->hw.wiphy, sdata->keys[key_idx]);

	return NULL;
}

static int ieee80211_del_key(struct wiphy *wiphy, struct net_device *dev,
			     int link_id, u8 key_idx, bool pairwise,
			     const u8 *mac_addr)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_key *key;

	lockdep_assert_wiphy(local->hw.wiphy);

	key = ieee80211_lookup_key(sdata, link_id, key_idx, pairwise, mac_addr);
	if (!key)
		return -ENOENT;

	ieee80211_key_free(key, sdata->vif.type == NL80211_IFTYPE_STATION);

	return 0;
}

static int ieee80211_get_key(struct wiphy *wiphy, struct net_device *dev,
			     int link_id, u8 key_idx, bool pairwise,
			     const u8 *mac_addr, void *cookie,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     void (*callback)(void *cookie,
					      struct key_params *params))
{
	struct ieee80211_sub_if_data *sdata;
<<<<<<< HEAD
	struct sta_info *sta = NULL;
	u8 seq[6] = {0};
	struct key_params params;
	struct ieee80211_key *key = NULL;
=======
	u8 seq[6] = {0};
	struct key_params params;
	struct ieee80211_key *key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u64 pn64;
	u32 iv32;
	u16 iv16;
	int err = -ENOENT;
<<<<<<< HEAD
=======
	struct ieee80211_key_seq kseq = {};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();

<<<<<<< HEAD
	if (mac_addr) {
		sta = sta_info_get_bss(sdata, mac_addr);
		if (!sta)
			goto out;

		if (pairwise)
			key = rcu_dereference(sta->ptk);
		else if (key_idx < NUM_DEFAULT_KEYS)
			key = rcu_dereference(sta->gtk[key_idx]);
	} else
		key = rcu_dereference(sdata->keys[key_idx]);

=======
	key = ieee80211_lookup_key(sdata, link_id, key_idx, pairwise, mac_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!key)
		goto out;

	memset(&params, 0, sizeof(params));

	params.cipher = key->conf.cipher;

	switch (key->conf.cipher) {
	case WLAN_CIPHER_SUITE_TKIP:
<<<<<<< HEAD
		iv32 = key->u.tkip.tx.iv32;
		iv16 = key->u.tkip.tx.iv16;

		if (key->flags & KEY_FLAG_UPLOADED_TO_HARDWARE)
			drv_get_tkip_seq(sdata->local,
					 key->conf.hw_key_idx,
					 &iv32, &iv16);
=======
		pn64 = atomic64_read(&key->conf.tx_pn);
		iv32 = TKIP_PN_TO_IV32(pn64);
		iv16 = TKIP_PN_TO_IV16(pn64);

		if (key->flags & KEY_FLAG_UPLOADED_TO_HARDWARE &&
		    !(key->conf.flags & IEEE80211_KEY_FLAG_GENERATE_IV)) {
			drv_get_key_seq(sdata->local, key, &kseq);
			iv32 = kseq.tkip.iv32;
			iv16 = kseq.tkip.iv16;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		seq[0] = iv16 & 0xff;
		seq[1] = (iv16 >> 8) & 0xff;
		seq[2] = iv32 & 0xff;
		seq[3] = (iv32 >> 8) & 0xff;
		seq[4] = (iv32 >> 16) & 0xff;
		seq[5] = (iv32 >> 24) & 0xff;
		params.seq = seq;
		params.seq_len = 6;
		break;
	case WLAN_CIPHER_SUITE_CCMP:
<<<<<<< HEAD
		pn64 = atomic64_read(&key->u.ccmp.tx_pn);
		seq[0] = pn64;
		seq[1] = pn64 >> 8;
		seq[2] = pn64 >> 16;
		seq[3] = pn64 >> 24;
		seq[4] = pn64 >> 32;
		seq[5] = pn64 >> 40;
		params.seq = seq;
		params.seq_len = 6;
		break;
	case WLAN_CIPHER_SUITE_AES_CMAC:
		pn64 = atomic64_read(&key->u.aes_cmac.tx_pn);
		seq[0] = pn64;
		seq[1] = pn64 >> 8;
		seq[2] = pn64 >> 16;
		seq[3] = pn64 >> 24;
		seq[4] = pn64 >> 32;
		seq[5] = pn64 >> 40;
		params.seq = seq;
		params.seq_len = 6;
=======
	case WLAN_CIPHER_SUITE_CCMP_256:
	case WLAN_CIPHER_SUITE_AES_CMAC:
	case WLAN_CIPHER_SUITE_BIP_CMAC_256:
		BUILD_BUG_ON(offsetof(typeof(kseq), ccmp) !=
			     offsetof(typeof(kseq), aes_cmac));
		fallthrough;
	case WLAN_CIPHER_SUITE_BIP_GMAC_128:
	case WLAN_CIPHER_SUITE_BIP_GMAC_256:
		BUILD_BUG_ON(offsetof(typeof(kseq), ccmp) !=
			     offsetof(typeof(kseq), aes_gmac));
		fallthrough;
	case WLAN_CIPHER_SUITE_GCMP:
	case WLAN_CIPHER_SUITE_GCMP_256:
		BUILD_BUG_ON(offsetof(typeof(kseq), ccmp) !=
			     offsetof(typeof(kseq), gcmp));

		if (key->flags & KEY_FLAG_UPLOADED_TO_HARDWARE &&
		    !(key->conf.flags & IEEE80211_KEY_FLAG_GENERATE_IV)) {
			drv_get_key_seq(sdata->local, key, &kseq);
			memcpy(seq, kseq.ccmp.pn, 6);
		} else {
			pn64 = atomic64_read(&key->conf.tx_pn);
			seq[0] = pn64;
			seq[1] = pn64 >> 8;
			seq[2] = pn64 >> 16;
			seq[3] = pn64 >> 24;
			seq[4] = pn64 >> 32;
			seq[5] = pn64 >> 40;
		}
		params.seq = seq;
		params.seq_len = 6;
		break;
	default:
		if (!(key->flags & KEY_FLAG_UPLOADED_TO_HARDWARE))
			break;
		if (WARN_ON(key->conf.flags & IEEE80211_KEY_FLAG_GENERATE_IV))
			break;
		drv_get_key_seq(sdata->local, key, &kseq);
		params.seq = kseq.hw.seq;
		params.seq_len = kseq.hw.seq_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	params.key = key->conf.key;
	params.key_len = key->conf.keylen;

	callback(cookie, &params);
	err = 0;

 out:
	rcu_read_unlock();
	return err;
}

static int ieee80211_config_default_key(struct wiphy *wiphy,
					struct net_device *dev,
<<<<<<< HEAD
					u8 key_idx, bool uni,
					bool multi)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	ieee80211_set_default_key(sdata, key_idx, uni, multi);
=======
					int link_id, u8 key_idx, bool uni,
					bool multi)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_link_data *link =
		ieee80211_link_or_deflink(sdata, link_id, false);

	if (IS_ERR(link))
		return PTR_ERR(link);

	ieee80211_set_default_key(link, key_idx, uni, multi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int ieee80211_config_default_mgmt_key(struct wiphy *wiphy,
					     struct net_device *dev,
<<<<<<< HEAD
					     u8 key_idx)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	ieee80211_set_default_mgmt_key(sdata, key_idx);
=======
					     int link_id, u8 key_idx)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_link_data *link =
		ieee80211_link_or_deflink(sdata, link_id, true);

	if (IS_ERR(link))
		return PTR_ERR(link);

	ieee80211_set_default_mgmt_key(link, key_idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void rate_idx_to_bitrate(struct rate_info *rate, struct sta_info *sta, int idx)
{
	if (!(rate->flags & RATE_INFO_FLAGS_MCS)) {
		struct ieee80211_supported_band *sband;
		sband = sta->local->hw.wiphy->bands[
				sta->local->hw.conf.channel->band];
		rate->legacy = sband->bitrates[idx].bitrate;
	} else
		rate->mcs = idx;
=======
static int ieee80211_config_default_beacon_key(struct wiphy *wiphy,
					       struct net_device *dev,
					       int link_id, u8 key_idx)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_link_data *link =
		ieee80211_link_or_deflink(sdata, link_id, true);

	if (IS_ERR(link))
		return PTR_ERR(link);

	ieee80211_set_default_beacon_key(link, key_idx);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void sta_set_rate_info_tx(struct sta_info *sta,
			  const struct ieee80211_tx_rate *rate,
			  struct rate_info *rinfo)
{
	rinfo->flags = 0;
<<<<<<< HEAD
	if (rate->flags & IEEE80211_TX_RC_MCS)
		rinfo->flags |= RATE_INFO_FLAGS_MCS;
	if (rate->flags & IEEE80211_TX_RC_40_MHZ_WIDTH)
		rinfo->flags |= RATE_INFO_FLAGS_40_MHZ_WIDTH;
	if (rate->flags & IEEE80211_TX_RC_SHORT_GI)
		rinfo->flags |= RATE_INFO_FLAGS_SHORT_GI;
	rate_idx_to_bitrate(rinfo, sta, rate->idx);
}

static void sta_set_sinfo(struct sta_info *sta, struct station_info *sinfo)
{
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct timespec uptime;

	sinfo->generation = sdata->local->sta_generation;

	sinfo->filled = STATION_INFO_INACTIVE_TIME |
			STATION_INFO_RX_BYTES |
			STATION_INFO_TX_BYTES |
			STATION_INFO_RX_PACKETS |
			STATION_INFO_TX_PACKETS |
			STATION_INFO_TX_RETRIES |
			STATION_INFO_TX_FAILED |
			STATION_INFO_TX_BITRATE |
			STATION_INFO_RX_BITRATE |
			STATION_INFO_RX_DROP_MISC |
			STATION_INFO_BSS_PARAM |
			STATION_INFO_CONNECTED_TIME |
			STATION_INFO_STA_FLAGS |
			STATION_INFO_BEACON_LOSS_COUNT;

	do_posix_clock_monotonic_gettime(&uptime);
	sinfo->connected_time = uptime.tv_sec - sta->last_connected;

	sinfo->inactive_time = jiffies_to_msecs(jiffies - sta->last_rx);
	sinfo->rx_bytes = sta->rx_bytes;
	sinfo->tx_bytes = sta->tx_bytes;
	sinfo->rx_packets = sta->rx_packets;
	sinfo->tx_packets = sta->tx_packets;
	sinfo->tx_retries = sta->tx_retry_count;
	sinfo->tx_failed = sta->tx_retry_failed;
	sinfo->rx_dropped_misc = sta->rx_dropped;
	sinfo->beacon_loss_count = sta->beacon_loss_count;

	if ((sta->local->hw.flags & IEEE80211_HW_SIGNAL_DBM) ||
	    (sta->local->hw.flags & IEEE80211_HW_SIGNAL_UNSPEC)) {
		sinfo->filled |= STATION_INFO_SIGNAL | STATION_INFO_SIGNAL_AVG;
		sinfo->signal = (s8)sta->last_signal;
		sinfo->signal_avg = (s8) -ewma_read(&sta->avg_signal);
	}

	sta_set_rate_info_tx(sta, &sta->last_tx_rate, &sinfo->txrate);

	sinfo->rxrate.flags = 0;
	if (sta->last_rx_rate_flag & RX_FLAG_HT)
		sinfo->rxrate.flags |= RATE_INFO_FLAGS_MCS;
	if (sta->last_rx_rate_flag & RX_FLAG_40MHZ)
		sinfo->rxrate.flags |= RATE_INFO_FLAGS_40_MHZ_WIDTH;
	if (sta->last_rx_rate_flag & RX_FLAG_SHORT_GI)
		sinfo->rxrate.flags |= RATE_INFO_FLAGS_SHORT_GI;
	rate_idx_to_bitrate(&sinfo->rxrate, sta, sta->last_rx_rate_idx);

	if (ieee80211_vif_is_mesh(&sdata->vif)) {
#ifdef CONFIG_MAC80211_MESH
		sinfo->filled |= STATION_INFO_LLID |
				 STATION_INFO_PLID |
				 STATION_INFO_PLINK_STATE;

		sinfo->llid = le16_to_cpu(sta->llid);
		sinfo->plid = le16_to_cpu(sta->plid);
		sinfo->plink_state = sta->plink_state;
#endif
	}

	sinfo->bss_param.flags = 0;
	if (sdata->vif.bss_conf.use_cts_prot)
		sinfo->bss_param.flags |= BSS_PARAM_FLAGS_CTS_PROT;
	if (sdata->vif.bss_conf.use_short_preamble)
		sinfo->bss_param.flags |= BSS_PARAM_FLAGS_SHORT_PREAMBLE;
	if (sdata->vif.bss_conf.use_short_slot)
		sinfo->bss_param.flags |= BSS_PARAM_FLAGS_SHORT_SLOT_TIME;
	sinfo->bss_param.dtim_period = sdata->local->hw.conf.ps_dtim_period;
	sinfo->bss_param.beacon_interval = sdata->vif.bss_conf.beacon_int;

	sinfo->sta_flags.set = 0;
	sinfo->sta_flags.mask = BIT(NL80211_STA_FLAG_AUTHORIZED) |
				BIT(NL80211_STA_FLAG_SHORT_PREAMBLE) |
				BIT(NL80211_STA_FLAG_WME) |
				BIT(NL80211_STA_FLAG_MFP) |
				BIT(NL80211_STA_FLAG_AUTHENTICATED) |
				BIT(NL80211_STA_FLAG_TDLS_PEER);
	if (test_sta_flag(sta, WLAN_STA_AUTHORIZED))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_AUTHORIZED);
	if (test_sta_flag(sta, WLAN_STA_SHORT_PREAMBLE))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_SHORT_PREAMBLE);
	if (test_sta_flag(sta, WLAN_STA_WME))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_WME);
	if (test_sta_flag(sta, WLAN_STA_MFP))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_MFP);
	if (test_sta_flag(sta, WLAN_STA_AUTH))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_AUTHENTICATED);
	if (test_sta_flag(sta, WLAN_STA_TDLS_PEER))
		sinfo->sta_flags.set |= BIT(NL80211_STA_FLAG_TDLS_PEER);
}


static int ieee80211_dump_station(struct wiphy *wiphy, struct net_device *dev,
				 int idx, u8 *mac, struct station_info *sinfo)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct sta_info *sta;
	int ret = -ENOENT;

	rcu_read_lock();
=======
	if (rate->flags & IEEE80211_TX_RC_MCS) {
		rinfo->flags |= RATE_INFO_FLAGS_MCS;
		rinfo->mcs = rate->idx;
	} else if (rate->flags & IEEE80211_TX_RC_VHT_MCS) {
		rinfo->flags |= RATE_INFO_FLAGS_VHT_MCS;
		rinfo->mcs = ieee80211_rate_get_vht_mcs(rate);
		rinfo->nss = ieee80211_rate_get_vht_nss(rate);
	} else {
		struct ieee80211_supported_band *sband;

		sband = ieee80211_get_sband(sta->sdata);
		WARN_ON_ONCE(sband && !sband->bitrates);
		if (sband && sband->bitrates)
			rinfo->legacy = sband->bitrates[rate->idx].bitrate;
	}
	if (rate->flags & IEEE80211_TX_RC_40_MHZ_WIDTH)
		rinfo->bw = RATE_INFO_BW_40;
	else if (rate->flags & IEEE80211_TX_RC_80_MHZ_WIDTH)
		rinfo->bw = RATE_INFO_BW_80;
	else if (rate->flags & IEEE80211_TX_RC_160_MHZ_WIDTH)
		rinfo->bw = RATE_INFO_BW_160;
	else
		rinfo->bw = RATE_INFO_BW_20;
	if (rate->flags & IEEE80211_TX_RC_SHORT_GI)
		rinfo->flags |= RATE_INFO_FLAGS_SHORT_GI;
}

static int ieee80211_dump_station(struct wiphy *wiphy, struct net_device *dev,
				  int idx, u8 *mac, struct station_info *sinfo)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
	int ret = -ENOENT;

	lockdep_assert_wiphy(local->hw.wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sta = sta_info_get_by_idx(sdata, idx);
	if (sta) {
		ret = 0;
		memcpy(mac, sta->sta.addr, ETH_ALEN);
<<<<<<< HEAD
		sta_set_sinfo(sta, sinfo);
	}

	rcu_read_unlock();

=======
		sta_set_sinfo(sta, sinfo, true);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int ieee80211_dump_survey(struct wiphy *wiphy, struct net_device *dev,
				 int idx, struct survey_info *survey)
{
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);

	return drv_get_survey(local, idx, survey);
}

static int ieee80211_get_station(struct wiphy *wiphy, struct net_device *dev,
<<<<<<< HEAD
				 u8 *mac, struct station_info *sinfo)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct sta_info *sta;
	int ret = -ENOENT;

	rcu_read_lock();
=======
				 const u8 *mac, struct station_info *sinfo)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
	int ret = -ENOENT;

	lockdep_assert_wiphy(local->hw.wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sta = sta_info_get_bss(sdata, mac);
	if (sta) {
		ret = 0;
<<<<<<< HEAD
		sta_set_sinfo(sta, sinfo);
	}

	rcu_read_unlock();

	return ret;
}

static int ieee80211_set_probe_resp(struct ieee80211_sub_if_data *sdata,
				    const u8 *resp, size_t resp_len)
{
	struct sk_buff *new, *old;
=======
		sta_set_sinfo(sta, sinfo, true);
	}

	return ret;
}

static int ieee80211_set_monitor_channel(struct wiphy *wiphy,
					 struct cfg80211_chan_def *chandef)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_chan_req chanreq = { .oper = *chandef };
	int ret;

	lockdep_assert_wiphy(local->hw.wiphy);

	if (cfg80211_chandef_identical(&local->monitor_chanreq.oper,
				       &chanreq.oper))
		return 0;

	sdata = wiphy_dereference(local->hw.wiphy,
				  local->monitor_sdata);
	if (!sdata)
		goto done;

	if (cfg80211_chandef_identical(&sdata->vif.bss_conf.chanreq.oper,
				       &chanreq.oper))
		return 0;

	ieee80211_link_release_channel(&sdata->deflink);
	ret = ieee80211_link_use_channel(&sdata->deflink, &chanreq,
					 IEEE80211_CHANCTX_EXCLUSIVE);
	if (ret)
		return ret;
done:
	local->monitor_chanreq = chanreq;
	return 0;
}

static int
ieee80211_set_probe_resp(struct ieee80211_sub_if_data *sdata,
			 const u8 *resp, size_t resp_len,
			 const struct ieee80211_csa_settings *csa,
			 const struct ieee80211_color_change_settings *cca,
			 struct ieee80211_link_data *link)
{
	struct probe_resp *new, *old;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!resp || !resp_len)
		return 1;

<<<<<<< HEAD
	old = rtnl_dereference(sdata->u.ap.probe_resp);

	new = dev_alloc_skb(resp_len);
	if (!new)
		return -ENOMEM;

	memcpy(skb_put(new, resp_len), resp, resp_len);

	rcu_assign_pointer(sdata->u.ap.probe_resp, new);
	if (old) {
		/* TODO: use call_rcu() */
		synchronize_rcu();
		dev_kfree_skb(old);
	}
=======
	old = sdata_dereference(link->u.ap.probe_resp, sdata);

	new = kzalloc(sizeof(struct probe_resp) + resp_len, GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	new->len = resp_len;
	memcpy(new->data, resp, resp_len);

	if (csa)
		memcpy(new->cntdwn_counter_offsets, csa->counter_offsets_presp,
		       csa->n_counter_offsets_presp *
		       sizeof(new->cntdwn_counter_offsets[0]));
	else if (cca)
		new->cntdwn_counter_offsets[0] = cca->counter_offset_presp;

	rcu_assign_pointer(link->u.ap.probe_resp, new);
	if (old)
		kfree_rcu(old, rcu_head);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int ieee80211_assign_beacon(struct ieee80211_sub_if_data *sdata,
				   struct cfg80211_beacon_data *params)
{
	struct beacon_data *new, *old;
	int new_head_len, new_tail_len;
	int size, err;
	u32 changed = BSS_CHANGED_BEACON;

	old = rtnl_dereference(sdata->u.ap.beacon);
=======
static int ieee80211_set_fils_discovery(struct ieee80211_sub_if_data *sdata,
					struct cfg80211_fils_discovery *params,
					struct ieee80211_link_data *link,
					struct ieee80211_bss_conf *link_conf,
					u64 *changed)
{
	struct fils_discovery_data *new, *old = NULL;
	struct ieee80211_fils_discovery *fd;

	if (!params->update)
		return 0;

	fd = &link_conf->fils_discovery;
	fd->min_interval = params->min_interval;
	fd->max_interval = params->max_interval;

	old = sdata_dereference(link->u.ap.fils_discovery, sdata);
	if (old)
		kfree_rcu(old, rcu_head);

	if (params->tmpl && params->tmpl_len) {
		new = kzalloc(sizeof(*new) + params->tmpl_len, GFP_KERNEL);
		if (!new)
			return -ENOMEM;
		new->len = params->tmpl_len;
		memcpy(new->data, params->tmpl, params->tmpl_len);
		rcu_assign_pointer(link->u.ap.fils_discovery, new);
	} else {
		RCU_INIT_POINTER(link->u.ap.fils_discovery, NULL);
	}

	*changed |= BSS_CHANGED_FILS_DISCOVERY;
	return 0;
}

static int
ieee80211_set_unsol_bcast_probe_resp(struct ieee80211_sub_if_data *sdata,
				     struct cfg80211_unsol_bcast_probe_resp *params,
				     struct ieee80211_link_data *link,
				     struct ieee80211_bss_conf *link_conf,
				     u64 *changed)
{
	struct unsol_bcast_probe_resp_data *new, *old = NULL;

	if (!params->update)
		return 0;

	link_conf->unsol_bcast_probe_resp_interval = params->interval;

	old = sdata_dereference(link->u.ap.unsol_bcast_probe_resp, sdata);
	if (old)
		kfree_rcu(old, rcu_head);

	if (params->tmpl && params->tmpl_len) {
		new = kzalloc(sizeof(*new) + params->tmpl_len, GFP_KERNEL);
		if (!new)
			return -ENOMEM;
		new->len = params->tmpl_len;
		memcpy(new->data, params->tmpl, params->tmpl_len);
		rcu_assign_pointer(link->u.ap.unsol_bcast_probe_resp, new);
	} else {
		RCU_INIT_POINTER(link->u.ap.unsol_bcast_probe_resp, NULL);
	}

	*changed |= BSS_CHANGED_UNSOL_BCAST_PROBE_RESP;
	return 0;
}

static int ieee80211_set_ftm_responder_params(
				struct ieee80211_sub_if_data *sdata,
				const u8 *lci, size_t lci_len,
				const u8 *civicloc, size_t civicloc_len,
				struct ieee80211_bss_conf *link_conf)
{
	struct ieee80211_ftm_responder_params *new, *old;
	u8 *pos;
	int len;

	if (!lci_len && !civicloc_len)
		return 0;

	old = link_conf->ftmr_params;
	len = lci_len + civicloc_len;

	new = kzalloc(sizeof(*new) + len, GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	pos = (u8 *)(new + 1);
	if (lci_len) {
		new->lci_len = lci_len;
		new->lci = pos;
		memcpy(pos, lci, lci_len);
		pos += lci_len;
	}

	if (civicloc_len) {
		new->civicloc_len = civicloc_len;
		new->civicloc = pos;
		memcpy(pos, civicloc, civicloc_len);
		pos += civicloc_len;
	}

	link_conf->ftmr_params = new;
	kfree(old);

	return 0;
}

static int
ieee80211_copy_mbssid_beacon(u8 *pos, struct cfg80211_mbssid_elems *dst,
			     struct cfg80211_mbssid_elems *src)
{
	int i, offset = 0;

	for (i = 0; i < src->cnt; i++) {
		memcpy(pos + offset, src->elem[i].data, src->elem[i].len);
		dst->elem[i].len = src->elem[i].len;
		dst->elem[i].data = pos + offset;
		offset += dst->elem[i].len;
	}
	dst->cnt = src->cnt;

	return offset;
}

static int
ieee80211_copy_rnr_beacon(u8 *pos, struct cfg80211_rnr_elems *dst,
			  struct cfg80211_rnr_elems *src)
{
	int i, offset = 0;

	for (i = 0; i < src->cnt; i++) {
		memcpy(pos + offset, src->elem[i].data, src->elem[i].len);
		dst->elem[i].len = src->elem[i].len;
		dst->elem[i].data = pos + offset;
		offset += dst->elem[i].len;
	}
	dst->cnt = src->cnt;

	return offset;
}

static int
ieee80211_assign_beacon(struct ieee80211_sub_if_data *sdata,
			struct ieee80211_link_data *link,
			struct cfg80211_beacon_data *params,
			const struct ieee80211_csa_settings *csa,
			const struct ieee80211_color_change_settings *cca,
			u64 *changed)
{
	struct cfg80211_mbssid_elems *mbssid = NULL;
	struct cfg80211_rnr_elems *rnr = NULL;
	struct beacon_data *new, *old;
	int new_head_len, new_tail_len;
	int size, err;
	u64 _changed = BSS_CHANGED_BEACON;
	struct ieee80211_bss_conf *link_conf = link->conf;

	old = sdata_dereference(link->u.ap.beacon, sdata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Need to have a beacon head if we don't have one yet */
	if (!params->head && !old)
		return -EINVAL;

	/* new or old head? */
	if (params->head)
		new_head_len = params->head_len;
	else
		new_head_len = old->head_len;

	/* new or old tail? */
	if (params->tail || !old)
		/* params->tail_len will be zero for !params->tail */
		new_tail_len = params->tail_len;
	else
		new_tail_len = old->tail_len;

	size = sizeof(*new) + new_head_len + new_tail_len;

<<<<<<< HEAD
=======
	/* new or old multiple BSSID elements? */
	if (params->mbssid_ies) {
		mbssid = params->mbssid_ies;
		size += struct_size(new->mbssid_ies, elem, mbssid->cnt);
		if (params->rnr_ies) {
			rnr = params->rnr_ies;
			size += struct_size(new->rnr_ies, elem, rnr->cnt);
		}
		size += ieee80211_get_mbssid_beacon_len(mbssid, rnr,
							mbssid->cnt);
	} else if (old && old->mbssid_ies) {
		mbssid = old->mbssid_ies;
		size += struct_size(new->mbssid_ies, elem, mbssid->cnt);
		if (old && old->rnr_ies) {
			rnr = old->rnr_ies;
			size += struct_size(new->rnr_ies, elem, rnr->cnt);
		}
		size += ieee80211_get_mbssid_beacon_len(mbssid, rnr,
							mbssid->cnt);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	new = kzalloc(size, GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	/* start filling the new info now */

	/*
	 * pointers go into the block we allocated,
<<<<<<< HEAD
	 * memory is | beacon_data | head | tail |
=======
	 * memory is | beacon_data | head | tail | mbssid_ies | rnr_ies
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	new->head = ((u8 *) new) + sizeof(*new);
	new->tail = new->head + new_head_len;
	new->head_len = new_head_len;
	new->tail_len = new_tail_len;
<<<<<<< HEAD
=======
	/* copy in optional mbssid_ies */
	if (mbssid) {
		u8 *pos = new->tail + new->tail_len;

		new->mbssid_ies = (void *)pos;
		pos += struct_size(new->mbssid_ies, elem, mbssid->cnt);
		pos += ieee80211_copy_mbssid_beacon(pos, new->mbssid_ies,
						    mbssid);
		if (rnr) {
			new->rnr_ies = (void *)pos;
			pos += struct_size(new->rnr_ies, elem, rnr->cnt);
			ieee80211_copy_rnr_beacon(pos, new->rnr_ies, rnr);
		}
		/* update bssid_indicator */
		link_conf->bssid_indicator =
			ilog2(__roundup_pow_of_two(mbssid->cnt + 1));
	}

	if (csa) {
		new->cntdwn_current_counter = csa->count;
		memcpy(new->cntdwn_counter_offsets, csa->counter_offsets_beacon,
		       csa->n_counter_offsets_beacon *
		       sizeof(new->cntdwn_counter_offsets[0]));
	} else if (cca) {
		new->cntdwn_current_counter = cca->count;
		new->cntdwn_counter_offsets[0] = cca->counter_offset_beacon;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* copy in head */
	if (params->head)
		memcpy(new->head, params->head, new_head_len);
	else
		memcpy(new->head, old->head, new_head_len);

	/* copy in optional tail */
	if (params->tail)
		memcpy(new->tail, params->tail, new_tail_len);
	else
		if (old)
			memcpy(new->tail, old->tail, new_tail_len);

	err = ieee80211_set_probe_resp(sdata, params->probe_resp,
<<<<<<< HEAD
				       params->probe_resp_len);
	if (err < 0)
		return err;
	if (err == 0)
		changed |= BSS_CHANGED_AP_PROBE_RESP;

	rcu_assign_pointer(sdata->u.ap.beacon, new);
=======
				       params->probe_resp_len, csa, cca, link);
	if (err < 0) {
		kfree(new);
		return err;
	}
	if (err == 0)
		_changed |= BSS_CHANGED_AP_PROBE_RESP;

	if (params->ftm_responder != -1) {
		link_conf->ftm_responder = params->ftm_responder;
		err = ieee80211_set_ftm_responder_params(sdata,
							 params->lci,
							 params->lci_len,
							 params->civicloc,
							 params->civicloc_len,
							 link_conf);

		if (err < 0) {
			kfree(new);
			return err;
		}

		_changed |= BSS_CHANGED_FTM_RESPONDER;
	}

	rcu_assign_pointer(link->u.ap.beacon, new);
	sdata->u.ap.active = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (old)
		kfree_rcu(old, rcu_head);

<<<<<<< HEAD
	return changed;
=======
	*changed |= _changed;
	return 0;
}

static u8 ieee80211_num_beaconing_links(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_link_data *link;
	u8 link_id, num = 0;

	if (sdata->vif.type != NL80211_IFTYPE_AP &&
	    sdata->vif.type != NL80211_IFTYPE_P2P_GO)
		return num;

	if (!sdata->vif.valid_links)
		return num;

	for (link_id = 0; link_id < IEEE80211_MLD_MAX_NUM_LINKS; link_id++) {
		link = sdata_dereference(sdata->link[link_id], sdata);
		if (!link)
			continue;

		if (sdata_dereference(link->u.ap.beacon, sdata))
			num++;
	}

	return num;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ieee80211_start_ap(struct wiphy *wiphy, struct net_device *dev,
			      struct cfg80211_ap_settings *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
<<<<<<< HEAD
	struct beacon_data *old;
	struct ieee80211_sub_if_data *vlan;
	u32 changed = BSS_CHANGED_BEACON_INT |
		      BSS_CHANGED_BEACON_ENABLED |
		      BSS_CHANGED_BEACON |
		      BSS_CHANGED_SSID;
	int err;

	old = rtnl_dereference(sdata->u.ap.beacon);
	if (old)
		return -EALREADY;

=======
	struct ieee80211_local *local = sdata->local;
	struct beacon_data *old;
	struct ieee80211_sub_if_data *vlan;
	u64 changed = BSS_CHANGED_BEACON_INT |
		      BSS_CHANGED_BEACON_ENABLED |
		      BSS_CHANGED_BEACON |
		      BSS_CHANGED_P2P_PS |
		      BSS_CHANGED_TXPOWER |
		      BSS_CHANGED_TWT;
	int i, err;
	int prev_beacon_int;
	unsigned int link_id = params->beacon.link_id;
	struct ieee80211_link_data *link;
	struct ieee80211_bss_conf *link_conf;
	struct ieee80211_chan_req chanreq = { .oper = params->chandef };

	lockdep_assert_wiphy(local->hw.wiphy);

	link = sdata_dereference(sdata->link[link_id], sdata);
	if (!link)
		return -ENOLINK;

	link_conf = link->conf;

	old = sdata_dereference(link->u.ap.beacon, sdata);
	if (old)
		return -EALREADY;

	if (params->smps_mode != NL80211_SMPS_OFF)
		return -EOPNOTSUPP;

	link->smps_mode = IEEE80211_SMPS_OFF;

	link->needed_rx_chains = sdata->local->rx_chains;

	prev_beacon_int = link_conf->beacon_int;
	link_conf->beacon_int = params->beacon_interval;

	if (params->ht_cap)
		link_conf->ht_ldpc =
			params->ht_cap->cap_info &
				cpu_to_le16(IEEE80211_HT_CAP_LDPC_CODING);

	if (params->vht_cap) {
		link_conf->vht_ldpc =
			params->vht_cap->vht_cap_info &
				cpu_to_le32(IEEE80211_VHT_CAP_RXLDPC);
		link_conf->vht_su_beamformer =
			params->vht_cap->vht_cap_info &
				cpu_to_le32(IEEE80211_VHT_CAP_SU_BEAMFORMER_CAPABLE);
		link_conf->vht_su_beamformee =
			params->vht_cap->vht_cap_info &
				cpu_to_le32(IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE);
		link_conf->vht_mu_beamformer =
			params->vht_cap->vht_cap_info &
				cpu_to_le32(IEEE80211_VHT_CAP_MU_BEAMFORMER_CAPABLE);
		link_conf->vht_mu_beamformee =
			params->vht_cap->vht_cap_info &
				cpu_to_le32(IEEE80211_VHT_CAP_MU_BEAMFORMEE_CAPABLE);
	}

	if (params->he_cap && params->he_oper) {
		link_conf->he_support = true;
		link_conf->htc_trig_based_pkt_ext =
			le32_get_bits(params->he_oper->he_oper_params,
			      IEEE80211_HE_OPERATION_DFLT_PE_DURATION_MASK);
		link_conf->frame_time_rts_th =
			le32_get_bits(params->he_oper->he_oper_params,
			      IEEE80211_HE_OPERATION_RTS_THRESHOLD_MASK);
		changed |= BSS_CHANGED_HE_OBSS_PD;

		if (params->beacon.he_bss_color.enabled)
			changed |= BSS_CHANGED_HE_BSS_COLOR;
	}

	if (params->he_cap) {
		link_conf->he_ldpc =
			params->he_cap->phy_cap_info[1] &
				IEEE80211_HE_PHY_CAP1_LDPC_CODING_IN_PAYLOAD;
		link_conf->he_su_beamformer =
			params->he_cap->phy_cap_info[3] &
				IEEE80211_HE_PHY_CAP3_SU_BEAMFORMER;
		link_conf->he_su_beamformee =
			params->he_cap->phy_cap_info[4] &
				IEEE80211_HE_PHY_CAP4_SU_BEAMFORMEE;
		link_conf->he_mu_beamformer =
			params->he_cap->phy_cap_info[4] &
				IEEE80211_HE_PHY_CAP4_MU_BEAMFORMER;
		link_conf->he_full_ul_mumimo =
			params->he_cap->phy_cap_info[2] &
				IEEE80211_HE_PHY_CAP2_UL_MU_FULL_MU_MIMO;
	}

	if (params->eht_cap) {
		if (!link_conf->he_support)
			return -EOPNOTSUPP;

		link_conf->eht_support = true;

		link_conf->eht_su_beamformer =
			params->eht_cap->fixed.phy_cap_info[0] &
				IEEE80211_EHT_PHY_CAP0_SU_BEAMFORMER;
		link_conf->eht_su_beamformee =
			params->eht_cap->fixed.phy_cap_info[0] &
				IEEE80211_EHT_PHY_CAP0_SU_BEAMFORMEE;
		link_conf->eht_mu_beamformer =
			params->eht_cap->fixed.phy_cap_info[7] &
				(IEEE80211_EHT_PHY_CAP7_MU_BEAMFORMER_80MHZ |
				 IEEE80211_EHT_PHY_CAP7_MU_BEAMFORMER_160MHZ |
				 IEEE80211_EHT_PHY_CAP7_MU_BEAMFORMER_320MHZ);
	} else {
		link_conf->eht_su_beamformer = false;
		link_conf->eht_su_beamformee = false;
		link_conf->eht_mu_beamformer = false;
	}

	if (sdata->vif.type == NL80211_IFTYPE_AP &&
	    params->mbssid_config.tx_wdev) {
		err = ieee80211_set_ap_mbssid_options(sdata,
						      params->mbssid_config,
						      link_conf);
		if (err)
			return err;
	}

	err = ieee80211_link_use_channel(link, &chanreq,
					 IEEE80211_CHANCTX_SHARED);
	if (!err)
		ieee80211_link_copy_chanctx_to_vlans(link, false);
	if (err) {
		link_conf->beacon_int = prev_beacon_int;
		return err;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Apply control port protocol, this allows us to
	 * not encrypt dynamic WEP control frames.
	 */
	sdata->control_port_protocol = params->crypto.control_port_ethertype;
	sdata->control_port_no_encrypt = params->crypto.control_port_no_encrypt;
<<<<<<< HEAD
=======
	sdata->control_port_over_nl80211 =
				params->crypto.control_port_over_nl80211;
	sdata->control_port_no_preauth =
				params->crypto.control_port_no_preauth;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry(vlan, &sdata->u.ap.vlans, u.vlan.list) {
		vlan->control_port_protocol =
			params->crypto.control_port_ethertype;
		vlan->control_port_no_encrypt =
			params->crypto.control_port_no_encrypt;
<<<<<<< HEAD
	}

	sdata->vif.bss_conf.beacon_int = params->beacon_interval;
	sdata->vif.bss_conf.dtim_period = params->dtim_period;

	sdata->vif.bss_conf.ssid_len = params->ssid_len;
	if (params->ssid_len)
		memcpy(sdata->vif.bss_conf.ssid, params->ssid,
		       params->ssid_len);
	sdata->vif.bss_conf.hidden_ssid =
		(params->hidden_ssid != NL80211_HIDDEN_SSID_NOT_IN_USE);

	err = ieee80211_assign_beacon(sdata, &params->beacon);
	if (err < 0)
		return err;
	changed |= err;

	ieee80211_bss_info_change_notify(sdata, changed);

	return 0;
}

static int ieee80211_change_beacon(struct wiphy *wiphy, struct net_device *dev,
				   struct cfg80211_beacon_data *params)
{
	struct ieee80211_sub_if_data *sdata;
	struct beacon_data *old;
	int err;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	old = rtnl_dereference(sdata->u.ap.beacon);
	if (!old)
		return -ENOENT;

	err = ieee80211_assign_beacon(sdata, params);
	if (err < 0)
		return err;
	ieee80211_bss_info_change_notify(sdata, err);
	return 0;
}

static int ieee80211_stop_ap(struct wiphy *wiphy, struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata;
	struct beacon_data *old;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	old = rtnl_dereference(sdata->u.ap.beacon);
	if (!old)
		return -ENOENT;

	RCU_INIT_POINTER(sdata->u.ap.beacon, NULL);

	kfree_rcu(old, rcu_head);

	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_BEACON_ENABLED);

	return 0;
}

/* Layer 2 Update frame (802.2 Type 1 LLC XID Update response) */
struct iapp_layer2_update {
	u8 da[ETH_ALEN];	/* broadcast */
	u8 sa[ETH_ALEN];	/* STA addr */
	__be16 len;		/* 6 */
	u8 dsap;		/* 0 */
	u8 ssap;		/* 0 */
	u8 control;
	u8 xid_info[3];
} __packed;

static void ieee80211_send_layer2_update(struct sta_info *sta)
{
	struct iapp_layer2_update *msg;
	struct sk_buff *skb;

	/* Send Level 2 Update Frame to update forwarding tables in layer 2
	 * bridge devices */

	skb = dev_alloc_skb(sizeof(*msg));
	if (!skb)
		return;
	msg = (struct iapp_layer2_update *)skb_put(skb, sizeof(*msg));

	/* 802.2 Type 1 Logical Link Control (LLC) Exchange Identifier (XID)
	 * Update response frame; IEEE Std 802.2-1998, 5.4.1.2.1 */

	memset(msg->da, 0xff, ETH_ALEN);
	memcpy(msg->sa, sta->sta.addr, ETH_ALEN);
	msg->len = htons(6);
	msg->dsap = 0;
	msg->ssap = 0x01;	/* NULL LSAP, CR Bit: Response */
	msg->control = 0xaf;	/* XID response lsb.1111F101.
				 * F=0 (no poll command; unsolicited frame) */
	msg->xid_info[0] = 0x81;	/* XID format identifier */
	msg->xid_info[1] = 1;	/* LLC types/classes: Type 1 LLC */
	msg->xid_info[2] = 0;	/* XID sender's receive window size (RW) */

	skb->dev = sta->sdata->dev;
	skb->protocol = eth_type_trans(skb, sta->sdata->dev);
	memset(skb->cb, 0, sizeof(skb->cb));
	netif_rx_ni(skb);
}

static int sta_apply_parameters(struct ieee80211_local *local,
				struct sta_info *sta,
				struct station_parameters *params)
{
	int ret = 0;
	u32 rates;
	int i, j;
	struct ieee80211_supported_band *sband;
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	u32 mask, set;

	sband = local->hw.wiphy->bands[local->oper_channel->band];

	mask = params->sta_flags_mask;
	set = params->sta_flags_set;

	/*
	 * In mesh mode, we can clear AUTHENTICATED flag but must
	 * also make ASSOCIATED follow appropriately for the driver
	 * API. See also below, after AUTHORIZED changes.
	 */
	if (mask & BIT(NL80211_STA_FLAG_AUTHENTICATED)) {
		/* cfg80211 should not allow this in non-mesh modes */
		if (WARN_ON(!ieee80211_vif_is_mesh(&sdata->vif)))
			return -EINVAL;

		if (set & BIT(NL80211_STA_FLAG_AUTHENTICATED) &&
		    !test_sta_flag(sta, WLAN_STA_AUTH)) {
			ret = sta_info_move_state(sta, IEEE80211_STA_AUTH);
			if (ret)
				return ret;
			ret = sta_info_move_state(sta, IEEE80211_STA_ASSOC);
			if (ret)
				return ret;
		}
=======
		vlan->control_port_over_nl80211 =
			params->crypto.control_port_over_nl80211;
		vlan->control_port_no_preauth =
			params->crypto.control_port_no_preauth;
	}

	link_conf->dtim_period = params->dtim_period;
	link_conf->enable_beacon = true;
	link_conf->allow_p2p_go_ps = sdata->vif.p2p;
	link_conf->twt_responder = params->twt_responder;
	link_conf->he_obss_pd = params->he_obss_pd;
	link_conf->he_bss_color = params->beacon.he_bss_color;
	sdata->vif.cfg.s1g = params->chandef.chan->band ==
				  NL80211_BAND_S1GHZ;

	sdata->vif.cfg.ssid_len = params->ssid_len;
	if (params->ssid_len)
		memcpy(sdata->vif.cfg.ssid, params->ssid,
		       params->ssid_len);
	link_conf->hidden_ssid =
		(params->hidden_ssid != NL80211_HIDDEN_SSID_NOT_IN_USE);

	memset(&link_conf->p2p_noa_attr, 0,
	       sizeof(link_conf->p2p_noa_attr));
	link_conf->p2p_noa_attr.oppps_ctwindow =
		params->p2p_ctwindow & IEEE80211_P2P_OPPPS_CTWINDOW_MASK;
	if (params->p2p_opp_ps)
		link_conf->p2p_noa_attr.oppps_ctwindow |=
					IEEE80211_P2P_OPPPS_ENABLE_BIT;

	sdata->beacon_rate_set = false;
	if (wiphy_ext_feature_isset(local->hw.wiphy,
				    NL80211_EXT_FEATURE_BEACON_RATE_LEGACY)) {
		for (i = 0; i < NUM_NL80211_BANDS; i++) {
			sdata->beacon_rateidx_mask[i] =
				params->beacon_rate.control[i].legacy;
			if (sdata->beacon_rateidx_mask[i])
				sdata->beacon_rate_set = true;
		}
	}

	if (ieee80211_hw_check(&local->hw, HAS_RATE_CONTROL))
		link_conf->beacon_tx_rate = params->beacon_rate;

	err = ieee80211_assign_beacon(sdata, link, &params->beacon, NULL, NULL,
				      &changed);
	if (err < 0)
		goto error;

	err = ieee80211_set_fils_discovery(sdata, &params->fils_discovery,
					   link, link_conf, &changed);
	if (err < 0)
		goto error;

	err = ieee80211_set_unsol_bcast_probe_resp(sdata,
						   &params->unsol_bcast_probe_resp,
						   link, link_conf, &changed);
	if (err < 0)
		goto error;

	err = drv_start_ap(sdata->local, sdata, link_conf);
	if (err) {
		old = sdata_dereference(link->u.ap.beacon, sdata);

		if (old)
			kfree_rcu(old, rcu_head);
		RCU_INIT_POINTER(link->u.ap.beacon, NULL);
		sdata->u.ap.active = false;
		goto error;
	}

	ieee80211_recalc_dtim(local, sdata);
	ieee80211_vif_cfg_change_notify(sdata, BSS_CHANGED_SSID);
	ieee80211_link_info_change_notify(sdata, link, changed);

	if (ieee80211_num_beaconing_links(sdata) <= 1)
		netif_carrier_on(dev);

	list_for_each_entry(vlan, &sdata->u.ap.vlans, u.vlan.list)
		netif_carrier_on(vlan->dev);

	return 0;

error:
	ieee80211_link_release_channel(link);

	return err;
}

static int ieee80211_change_beacon(struct wiphy *wiphy, struct net_device *dev,
				   struct cfg80211_ap_update *params)

{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_link_data *link;
	struct cfg80211_beacon_data *beacon = &params->beacon;
	struct beacon_data *old;
	int err;
	struct ieee80211_bss_conf *link_conf;
	u64 changed = 0;

	lockdep_assert_wiphy(wiphy);

	link = sdata_dereference(sdata->link[beacon->link_id], sdata);
	if (!link)
		return -ENOLINK;

	link_conf = link->conf;

	/* don't allow changing the beacon while a countdown is in place - offset
	 * of channel switch counter may change
	 */
	if (link_conf->csa_active || link_conf->color_change_active)
		return -EBUSY;

	old = sdata_dereference(link->u.ap.beacon, sdata);
	if (!old)
		return -ENOENT;

	err = ieee80211_assign_beacon(sdata, link, beacon, NULL, NULL,
				      &changed);
	if (err < 0)
		return err;

	err = ieee80211_set_fils_discovery(sdata, &params->fils_discovery,
					   link, link_conf, &changed);
	if (err < 0)
		return err;

	err = ieee80211_set_unsol_bcast_probe_resp(sdata,
						   &params->unsol_bcast_probe_resp,
						   link, link_conf, &changed);
	if (err < 0)
		return err;

	if (beacon->he_bss_color_valid &&
	    beacon->he_bss_color.enabled != link_conf->he_bss_color.enabled) {
		link_conf->he_bss_color.enabled = beacon->he_bss_color.enabled;
		changed |= BSS_CHANGED_HE_BSS_COLOR;
	}

	ieee80211_link_info_change_notify(sdata, link, changed);
	return 0;
}

static void ieee80211_free_next_beacon(struct ieee80211_link_data *link)
{
	if (!link->u.ap.next_beacon)
		return;

	kfree(link->u.ap.next_beacon->mbssid_ies);
	kfree(link->u.ap.next_beacon->rnr_ies);
	kfree(link->u.ap.next_beacon);
	link->u.ap.next_beacon = NULL;
}

static int ieee80211_stop_ap(struct wiphy *wiphy, struct net_device *dev,
			     unsigned int link_id)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_sub_if_data *vlan;
	struct ieee80211_local *local = sdata->local;
	struct beacon_data *old_beacon;
	struct probe_resp *old_probe_resp;
	struct fils_discovery_data *old_fils_discovery;
	struct unsol_bcast_probe_resp_data *old_unsol_bcast_probe_resp;
	struct cfg80211_chan_def chandef;
	struct ieee80211_link_data *link =
		sdata_dereference(sdata->link[link_id], sdata);
	struct ieee80211_bss_conf *link_conf = link->conf;
	LIST_HEAD(keys);

	lockdep_assert_wiphy(local->hw.wiphy);

	old_beacon = sdata_dereference(link->u.ap.beacon, sdata);
	if (!old_beacon)
		return -ENOENT;
	old_probe_resp = sdata_dereference(link->u.ap.probe_resp,
					   sdata);
	old_fils_discovery = sdata_dereference(link->u.ap.fils_discovery,
					       sdata);
	old_unsol_bcast_probe_resp =
		sdata_dereference(link->u.ap.unsol_bcast_probe_resp,
				  sdata);

	/* abort any running channel switch or color change */
	link_conf->csa_active = false;
	link_conf->color_change_active = false;
	if (sdata->csa_blocked_tx) {
		ieee80211_wake_vif_queues(local, sdata,
					  IEEE80211_QUEUE_STOP_REASON_CSA);
		sdata->csa_blocked_tx = false;
	}

	ieee80211_free_next_beacon(link);

	/* turn off carrier for this interface and dependent VLANs */
	list_for_each_entry(vlan, &sdata->u.ap.vlans, u.vlan.list)
		netif_carrier_off(vlan->dev);

	if (ieee80211_num_beaconing_links(sdata) <= 1)
		netif_carrier_off(dev);

	/* remove beacon and probe response */
	sdata->u.ap.active = false;
	RCU_INIT_POINTER(link->u.ap.beacon, NULL);
	RCU_INIT_POINTER(link->u.ap.probe_resp, NULL);
	RCU_INIT_POINTER(link->u.ap.fils_discovery, NULL);
	RCU_INIT_POINTER(link->u.ap.unsol_bcast_probe_resp, NULL);
	kfree_rcu(old_beacon, rcu_head);
	if (old_probe_resp)
		kfree_rcu(old_probe_resp, rcu_head);
	if (old_fils_discovery)
		kfree_rcu(old_fils_discovery, rcu_head);
	if (old_unsol_bcast_probe_resp)
		kfree_rcu(old_unsol_bcast_probe_resp, rcu_head);

	kfree(link_conf->ftmr_params);
	link_conf->ftmr_params = NULL;

	sdata->vif.mbssid_tx_vif = NULL;
	link_conf->bssid_index = 0;
	link_conf->nontransmitted = false;
	link_conf->ema_ap = false;
	link_conf->bssid_indicator = 0;

	__sta_info_flush(sdata, true, link_id);

	ieee80211_remove_link_keys(link, &keys);
	if (!list_empty(&keys)) {
		synchronize_net();
		ieee80211_free_key_list(local, &keys);
	}

	link_conf->enable_beacon = false;
	sdata->beacon_rate_set = false;
	sdata->vif.cfg.ssid_len = 0;
	clear_bit(SDATA_STATE_OFFCHANNEL_BEACON_STOPPED, &sdata->state);
	ieee80211_link_info_change_notify(sdata, link,
					  BSS_CHANGED_BEACON_ENABLED);

	if (sdata->wdev.cac_started) {
		chandef = link_conf->chanreq.oper;
		wiphy_delayed_work_cancel(wiphy, &link->dfs_cac_timer_work);
		cfg80211_cac_event(sdata->dev, &chandef,
				   NL80211_RADAR_CAC_ABORTED,
				   GFP_KERNEL);
	}

	drv_stop_ap(sdata->local, sdata, link_conf);

	/* free all potentially still buffered bcast frames */
	local->total_ps_buffered -= skb_queue_len(&sdata->u.ap.ps.bc_buf);
	ieee80211_purge_tx_queue(&local->hw, &sdata->u.ap.ps.bc_buf);

	ieee80211_link_copy_chanctx_to_vlans(link, true);
	ieee80211_link_release_channel(link);

	return 0;
}

static int sta_apply_auth_flags(struct ieee80211_local *local,
				struct sta_info *sta,
				u32 mask, u32 set)
{
	int ret;

	if (mask & BIT(NL80211_STA_FLAG_AUTHENTICATED) &&
	    set & BIT(NL80211_STA_FLAG_AUTHENTICATED) &&
	    !test_sta_flag(sta, WLAN_STA_AUTH)) {
		ret = sta_info_move_state(sta, IEEE80211_STA_AUTH);
		if (ret)
			return ret;
	}

	if (mask & BIT(NL80211_STA_FLAG_ASSOCIATED) &&
	    set & BIT(NL80211_STA_FLAG_ASSOCIATED) &&
	    !test_sta_flag(sta, WLAN_STA_ASSOC)) {
		/*
		 * When peer becomes associated, init rate control as
		 * well. Some drivers require rate control initialized
		 * before drv_sta_state() is called.
		 */
		if (!test_sta_flag(sta, WLAN_STA_RATE_CONTROL))
			rate_control_rate_init(sta);

		ret = sta_info_move_state(sta, IEEE80211_STA_ASSOC);
		if (ret)
			return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (mask & BIT(NL80211_STA_FLAG_AUTHORIZED)) {
		if (set & BIT(NL80211_STA_FLAG_AUTHORIZED))
			ret = sta_info_move_state(sta, IEEE80211_STA_AUTHORIZED);
		else if (test_sta_flag(sta, WLAN_STA_AUTHORIZED))
			ret = sta_info_move_state(sta, IEEE80211_STA_ASSOC);
<<<<<<< HEAD
=======
		else
			ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	if (mask & BIT(NL80211_STA_FLAG_AUTHENTICATED)) {
		/* cfg80211 should not allow this in non-mesh modes */
		if (WARN_ON(!ieee80211_vif_is_mesh(&sdata->vif)))
			return -EINVAL;

		if (!(set & BIT(NL80211_STA_FLAG_AUTHENTICATED)) &&
		    test_sta_flag(sta, WLAN_STA_AUTH)) {
			ret = sta_info_move_state(sta, IEEE80211_STA_AUTH);
			if (ret)
				return ret;
			ret = sta_info_move_state(sta, IEEE80211_STA_NONE);
			if (ret)
				return ret;
		}
	}

=======
	if (mask & BIT(NL80211_STA_FLAG_ASSOCIATED) &&
	    !(set & BIT(NL80211_STA_FLAG_ASSOCIATED)) &&
	    test_sta_flag(sta, WLAN_STA_ASSOC)) {
		ret = sta_info_move_state(sta, IEEE80211_STA_AUTH);
		if (ret)
			return ret;
	}

	if (mask & BIT(NL80211_STA_FLAG_AUTHENTICATED) &&
	    !(set & BIT(NL80211_STA_FLAG_AUTHENTICATED)) &&
	    test_sta_flag(sta, WLAN_STA_AUTH)) {
		ret = sta_info_move_state(sta, IEEE80211_STA_NONE);
		if (ret)
			return ret;
	}

	return 0;
}

static void sta_apply_mesh_params(struct ieee80211_local *local,
				  struct sta_info *sta,
				  struct station_parameters *params)
{
#ifdef CONFIG_MAC80211_MESH
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	u64 changed = 0;

	if (params->sta_modify_mask & STATION_PARAM_APPLY_PLINK_STATE) {
		switch (params->plink_state) {
		case NL80211_PLINK_ESTAB:
			if (sta->mesh->plink_state != NL80211_PLINK_ESTAB)
				changed = mesh_plink_inc_estab_count(sdata);
			sta->mesh->plink_state = params->plink_state;
			sta->mesh->aid = params->peer_aid;

			ieee80211_mps_sta_status_update(sta);
			changed |= ieee80211_mps_set_sta_local_pm(sta,
				      sdata->u.mesh.mshcfg.power_mode);

			ewma_mesh_tx_rate_avg_init(&sta->mesh->tx_rate_avg);
			/* init at low value */
			ewma_mesh_tx_rate_avg_add(&sta->mesh->tx_rate_avg, 10);

			break;
		case NL80211_PLINK_LISTEN:
		case NL80211_PLINK_BLOCKED:
		case NL80211_PLINK_OPN_SNT:
		case NL80211_PLINK_OPN_RCVD:
		case NL80211_PLINK_CNF_RCVD:
		case NL80211_PLINK_HOLDING:
			if (sta->mesh->plink_state == NL80211_PLINK_ESTAB)
				changed = mesh_plink_dec_estab_count(sdata);
			sta->mesh->plink_state = params->plink_state;

			ieee80211_mps_sta_status_update(sta);
			changed |= ieee80211_mps_set_sta_local_pm(sta,
					NL80211_MESH_POWER_UNKNOWN);
			break;
		default:
			/*  nothing  */
			break;
		}
	}

	switch (params->plink_action) {
	case NL80211_PLINK_ACTION_NO_ACTION:
		/* nothing */
		break;
	case NL80211_PLINK_ACTION_OPEN:
		changed |= mesh_plink_open(sta);
		break;
	case NL80211_PLINK_ACTION_BLOCK:
		changed |= mesh_plink_block(sta);
		break;
	}

	if (params->local_pm)
		changed |= ieee80211_mps_set_sta_local_pm(sta,
							  params->local_pm);

	ieee80211_mbss_info_change_notify(sdata, changed);
#endif
}

static int sta_link_apply_parameters(struct ieee80211_local *local,
				     struct sta_info *sta, bool new_link,
				     struct link_station_parameters *params)
{
	int ret = 0;
	struct ieee80211_supported_band *sband;
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	u32 link_id = params->link_id < 0 ? 0 : params->link_id;
	struct ieee80211_link_data *link =
		sdata_dereference(sdata->link[link_id], sdata);
	struct link_sta_info *link_sta =
		rcu_dereference_protected(sta->link[link_id],
					  lockdep_is_held(&local->hw.wiphy->mtx));

	/*
	 * If there are no changes, then accept a link that exist,
	 * unless it's a new link.
	 */
	if (params->link_id >= 0 && !new_link &&
	    !params->link_mac && !params->txpwr_set &&
	    !params->supported_rates_len &&
	    !params->ht_capa && !params->vht_capa &&
	    !params->he_capa && !params->eht_capa &&
	    !params->opmode_notif_used)
		return 0;

	if (!link || !link_sta)
		return -EINVAL;

	sband = ieee80211_get_link_sband(link);
	if (!sband)
		return -EINVAL;

	if (params->link_mac) {
		if (new_link) {
			memcpy(link_sta->addr, params->link_mac, ETH_ALEN);
			memcpy(link_sta->pub->addr, params->link_mac, ETH_ALEN);
		} else if (!ether_addr_equal(link_sta->addr,
					     params->link_mac)) {
			return -EINVAL;
		}
	} else if (new_link) {
		return -EINVAL;
	}

	if (params->txpwr_set) {
		link_sta->pub->txpwr.type = params->txpwr.type;
		if (params->txpwr.type == NL80211_TX_POWER_LIMITED)
			link_sta->pub->txpwr.power = params->txpwr.power;
		ret = drv_sta_set_txpwr(local, sdata, sta);
		if (ret)
			return ret;
	}

	if (params->supported_rates &&
	    params->supported_rates_len) {
		ieee80211_parse_bitrates(link->conf->chanreq.oper.width,
					 sband, params->supported_rates,
					 params->supported_rates_len,
					 &link_sta->pub->supp_rates[sband->band]);
	}

	if (params->ht_capa)
		ieee80211_ht_cap_ie_to_sta_ht_cap(sdata, sband,
						  params->ht_capa, link_sta);

	/* VHT can override some HT caps such as the A-MSDU max length */
	if (params->vht_capa)
		ieee80211_vht_cap_ie_to_sta_vht_cap(sdata, sband,
						    params->vht_capa, NULL,
						    link_sta);

	if (params->he_capa)
		ieee80211_he_cap_ie_to_sta_he_cap(sdata, sband,
						  (void *)params->he_capa,
						  params->he_capa_len,
						  (void *)params->he_6ghz_capa,
						  link_sta);

	if (params->he_capa && params->eht_capa)
		ieee80211_eht_cap_ie_to_sta_eht_cap(sdata, sband,
						    (u8 *)params->he_capa,
						    params->he_capa_len,
						    params->eht_capa,
						    params->eht_capa_len,
						    link_sta);

	if (params->opmode_notif_used) {
		/* returned value is only needed for rc update, but the
		 * rc isn't initialized here yet, so ignore it
		 */
		__ieee80211_vht_handle_opmode(sdata, link_sta,
					      params->opmode_notif,
					      sband->band);
	}

	ieee80211_sta_init_nss(link_sta);

	return ret;
}

static int sta_apply_parameters(struct ieee80211_local *local,
				struct sta_info *sta,
				struct station_parameters *params)
{
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	u32 mask, set;
	int ret = 0;

	mask = params->sta_flags_mask;
	set = params->sta_flags_set;

	if (ieee80211_vif_is_mesh(&sdata->vif)) {
		/*
		 * In mesh mode, ASSOCIATED isn't part of the nl80211
		 * API but must follow AUTHENTICATED for driver state.
		 */
		if (mask & BIT(NL80211_STA_FLAG_AUTHENTICATED))
			mask |= BIT(NL80211_STA_FLAG_ASSOCIATED);
		if (set & BIT(NL80211_STA_FLAG_AUTHENTICATED))
			set |= BIT(NL80211_STA_FLAG_ASSOCIATED);
	} else if (test_sta_flag(sta, WLAN_STA_TDLS_PEER)) {
		/*
		 * TDLS -- everything follows authorized, but
		 * only becoming authorized is possible, not
		 * going back
		 */
		if (set & BIT(NL80211_STA_FLAG_AUTHORIZED)) {
			set |= BIT(NL80211_STA_FLAG_AUTHENTICATED) |
			       BIT(NL80211_STA_FLAG_ASSOCIATED);
			mask |= BIT(NL80211_STA_FLAG_AUTHENTICATED) |
				BIT(NL80211_STA_FLAG_ASSOCIATED);
		}
	}

	if (mask & BIT(NL80211_STA_FLAG_WME) &&
	    local->hw.queues >= IEEE80211_NUM_ACS)
		sta->sta.wme = set & BIT(NL80211_STA_FLAG_WME);

	/* auth flags will be set later for TDLS,
	 * and for unassociated stations that move to associated */
	if (!test_sta_flag(sta, WLAN_STA_TDLS_PEER) &&
	    !((mask & BIT(NL80211_STA_FLAG_ASSOCIATED)) &&
	      (set & BIT(NL80211_STA_FLAG_ASSOCIATED)))) {
		ret = sta_apply_auth_flags(local, sta, mask, set);
		if (ret)
			return ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mask & BIT(NL80211_STA_FLAG_SHORT_PREAMBLE)) {
		if (set & BIT(NL80211_STA_FLAG_SHORT_PREAMBLE))
			set_sta_flag(sta, WLAN_STA_SHORT_PREAMBLE);
		else
			clear_sta_flag(sta, WLAN_STA_SHORT_PREAMBLE);
	}

<<<<<<< HEAD
	if (mask & BIT(NL80211_STA_FLAG_WME)) {
		if (set & BIT(NL80211_STA_FLAG_WME)) {
			set_sta_flag(sta, WLAN_STA_WME);
			sta->sta.wme = true;
		} else {
			clear_sta_flag(sta, WLAN_STA_WME);
			sta->sta.wme = false;
		}
	}

	if (mask & BIT(NL80211_STA_FLAG_MFP)) {
=======
	if (mask & BIT(NL80211_STA_FLAG_MFP)) {
		sta->sta.mfp = !!(set & BIT(NL80211_STA_FLAG_MFP));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (set & BIT(NL80211_STA_FLAG_MFP))
			set_sta_flag(sta, WLAN_STA_MFP);
		else
			clear_sta_flag(sta, WLAN_STA_MFP);
	}

	if (mask & BIT(NL80211_STA_FLAG_TDLS_PEER)) {
		if (set & BIT(NL80211_STA_FLAG_TDLS_PEER))
			set_sta_flag(sta, WLAN_STA_TDLS_PEER);
		else
			clear_sta_flag(sta, WLAN_STA_TDLS_PEER);
	}

<<<<<<< HEAD
=======
	if (mask & BIT(NL80211_STA_FLAG_SPP_AMSDU))
		sta->sta.spp_amsdu = set & BIT(NL80211_STA_FLAG_SPP_AMSDU);

	/* mark TDLS channel switch support, if the AP allows it */
	if (test_sta_flag(sta, WLAN_STA_TDLS_PEER) &&
	    !sdata->deflink.u.mgd.tdls_chan_switch_prohibited &&
	    params->ext_capab_len >= 4 &&
	    params->ext_capab[3] & WLAN_EXT_CAPA4_TDLS_CHAN_SWITCH)
		set_sta_flag(sta, WLAN_STA_TDLS_CHAN_SWITCH);

	if (test_sta_flag(sta, WLAN_STA_TDLS_PEER) &&
	    !sdata->u.mgd.tdls_wider_bw_prohibited &&
	    ieee80211_hw_check(&local->hw, TDLS_WIDER_BW) &&
	    params->ext_capab_len >= 8 &&
	    params->ext_capab[7] & WLAN_EXT_CAPA8_TDLS_WIDE_BW_ENABLED)
		set_sta_flag(sta, WLAN_STA_TDLS_WIDER_BW);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (params->sta_modify_mask & STATION_PARAM_APPLY_UAPSD) {
		sta->sta.uapsd_queues = params->uapsd_queues;
		sta->sta.max_sp = params->max_sp;
	}

<<<<<<< HEAD
=======
	ieee80211_sta_set_max_amsdu_subframes(sta, params->ext_capab,
					      params->ext_capab_len);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * cfg80211 validates this (1-2007) and allows setting the AID
	 * only when creating a new station entry
	 */
	if (params->aid)
		sta->sta.aid = params->aid;

	/*
<<<<<<< HEAD
	 * FIXME: updating the following information is racy when this
	 *	  function is called from ieee80211_change_station().
	 *	  However, all this information should be static so
	 *	  maybe we should just reject attemps to change it.
=======
	 * Some of the following updates would be racy if called on an
	 * existing station, via ieee80211_change_station(). However,
	 * all such changes are rejected by cfg80211 except for updates
	 * changing the supported rates on an existing but not yet used
	 * TDLS peer.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */

	if (params->listen_interval >= 0)
		sta->listen_interval = params->listen_interval;

<<<<<<< HEAD
	if (params->supported_rates) {
		rates = 0;

		for (i = 0; i < params->supported_rates_len; i++) {
			int rate = (params->supported_rates[i] & 0x7f) * 5;
			for (j = 0; j < sband->n_bitrates; j++) {
				if (sband->bitrates[j].bitrate == rate)
					rates |= BIT(j);
			}
		}
		sta->sta.supp_rates[local->oper_channel->band] = rates;
	}

	if (params->ht_capa)
		ieee80211_ht_cap_ie_to_sta_ht_cap(sdata, sband,
						  params->ht_capa,
						  &sta->sta.ht_cap);

	if (ieee80211_vif_is_mesh(&sdata->vif)) {
#ifdef CONFIG_MAC80211_MESH
		if (sdata->u.mesh.security & IEEE80211_MESH_SEC_SECURED)
			switch (params->plink_state) {
			case NL80211_PLINK_LISTEN:
			case NL80211_PLINK_ESTAB:
			case NL80211_PLINK_BLOCKED:
				sta->plink_state = params->plink_state;
				break;
			default:
				/*  nothing  */
				break;
			}
		else
			switch (params->plink_action) {
			case PLINK_ACTION_OPEN:
				mesh_plink_open(sta);
				break;
			case PLINK_ACTION_BLOCK:
				mesh_plink_block(sta);
				break;
			}
#endif
	}
=======
	ret = sta_link_apply_parameters(local, sta, false,
					&params->link_sta_params);
	if (ret)
		return ret;

	if (params->support_p2p_ps >= 0)
		sta->sta.support_p2p_ps = params->support_p2p_ps;

	if (ieee80211_vif_is_mesh(&sdata->vif))
		sta_apply_mesh_params(local, sta, params);

	if (params->airtime_weight)
		sta->airtime_weight = params->airtime_weight;

	/* set the STA state after all sta info from usermode has been set */
	if (test_sta_flag(sta, WLAN_STA_TDLS_PEER) ||
	    set & BIT(NL80211_STA_FLAG_ASSOCIATED)) {
		ret = sta_apply_auth_flags(local, sta, mask, set);
		if (ret)
			return ret;
	}

	/* Mark the STA as MLO if MLD MAC address is available */
	if (params->link_sta_params.mld_mac)
		sta->sta.mlo = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int ieee80211_add_station(struct wiphy *wiphy, struct net_device *dev,
<<<<<<< HEAD
				 u8 *mac, struct station_parameters *params)
=======
				 const u8 *mac,
				 struct station_parameters *params)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct sta_info *sta;
	struct ieee80211_sub_if_data *sdata;
	int err;
<<<<<<< HEAD
	int layer2_update;
=======

	lockdep_assert_wiphy(local->hw.wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (params->vlan) {
		sdata = IEEE80211_DEV_TO_SUB_IF(params->vlan);

		if (sdata->vif.type != NL80211_IFTYPE_AP_VLAN &&
		    sdata->vif.type != NL80211_IFTYPE_AP)
			return -EINVAL;
	} else
		sdata = IEEE80211_DEV_TO_SUB_IF(dev);

<<<<<<< HEAD
	if (compare_ether_addr(mac, sdata->vif.addr) == 0)
		return -EINVAL;

	if (is_multicast_ether_addr(mac))
		return -EINVAL;

	sta = sta_info_alloc(sdata, mac, GFP_KERNEL);
	if (!sta)
		return -ENOMEM;

	sta_info_pre_move_state(sta, IEEE80211_STA_AUTH);
	sta_info_pre_move_state(sta, IEEE80211_STA_ASSOC);

=======
	if (ether_addr_equal(mac, sdata->vif.addr))
		return -EINVAL;

	if (!is_valid_ether_addr(mac))
		return -EINVAL;

	if (params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER) &&
	    sdata->vif.type == NL80211_IFTYPE_STATION &&
	    !sdata->u.mgd.associated)
		return -EINVAL;

	/*
	 * If we have a link ID, it can be a non-MLO station on an AP MLD,
	 * but we need to have a link_mac in that case as well, so use the
	 * STA's MAC address in that case.
	 */
	if (params->link_sta_params.link_id >= 0)
		sta = sta_info_alloc_with_link(sdata, mac,
					       params->link_sta_params.link_id,
					       params->link_sta_params.link_mac ?: mac,
					       GFP_KERNEL);
	else
		sta = sta_info_alloc(sdata, mac, GFP_KERNEL);

	if (!sta)
		return -ENOMEM;

	if (params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER))
		sta->sta.tdls = true;

	/* Though the mutex is not needed here (since the station is not
	 * visible yet), sta_apply_parameters (and inner functions) require
	 * the mutex due to other paths.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = sta_apply_parameters(local, sta, params);
	if (err) {
		sta_info_free(local, sta);
		return err;
	}

	/*
<<<<<<< HEAD
	 * for TDLS, rate control should be initialized only when supported
	 * rates are known.
	 */
	if (!test_sta_flag(sta, WLAN_STA_TDLS_PEER))
		rate_control_rate_init(sta);

	layer2_update = sdata->vif.type == NL80211_IFTYPE_AP_VLAN ||
		sdata->vif.type == NL80211_IFTYPE_AP;

	err = sta_info_insert_rcu(sta);
	if (err) {
		rcu_read_unlock();
		return err;
	}

	if (layer2_update)
		ieee80211_send_layer2_update(sta);

	rcu_read_unlock();

	return 0;
}

static int ieee80211_del_station(struct wiphy *wiphy, struct net_device *dev,
				 u8 *mac)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
=======
	 * for TDLS and for unassociated station, rate control should be
	 * initialized only when rates are known and station is marked
	 * authorized/associated
	 */
	if (!test_sta_flag(sta, WLAN_STA_TDLS_PEER) &&
	    test_sta_flag(sta, WLAN_STA_ASSOC))
		rate_control_rate_init(sta);

	return sta_info_insert(sta);
}

static int ieee80211_del_station(struct wiphy *wiphy, struct net_device *dev,
				 struct station_del_parameters *params)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ieee80211_sub_if_data *sdata;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

<<<<<<< HEAD
	if (mac)
		return sta_info_destroy_addr_bss(sdata, mac);

	sta_info_flush(local, sdata);
=======
	if (params->mac)
		return sta_info_destroy_addr_bss(sdata, params->mac);

	sta_info_flush(sdata, params->link_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int ieee80211_change_station(struct wiphy *wiphy,
<<<<<<< HEAD
				    struct net_device *dev,
				    u8 *mac,
=======
				    struct net_device *dev, const u8 *mac,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    struct station_parameters *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct sta_info *sta;
	struct ieee80211_sub_if_data *vlansdata;
<<<<<<< HEAD
	int err;

	mutex_lock(&local->sta_mtx);

	sta = sta_info_get_bss(sdata, mac);
	if (!sta) {
		mutex_unlock(&local->sta_mtx);
		return -ENOENT;
	}

	/* in station mode, some updates are only valid with TDLS */
	if (sdata->vif.type == NL80211_IFTYPE_STATION &&
	    (params->supported_rates || params->ht_capa || params->vht_capa ||
	     params->sta_modify_mask ||
	     (params->sta_flags_mask & BIT(NL80211_STA_FLAG_WME))) &&
	    !test_sta_flag(sta, WLAN_STA_TDLS_PEER)) {
		mutex_unlock(&local->sta_mtx);
		return -EINVAL;
	}
=======
	enum cfg80211_station_type statype;
	int err;

	lockdep_assert_wiphy(local->hw.wiphy);

	sta = sta_info_get_bss(sdata, mac);
	if (!sta)
		return -ENOENT;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_MESH_POINT:
		if (sdata->u.mesh.user_mpm)
			statype = CFG80211_STA_MESH_PEER_USER;
		else
			statype = CFG80211_STA_MESH_PEER_KERNEL;
		break;
	case NL80211_IFTYPE_ADHOC:
		statype = CFG80211_STA_IBSS;
		break;
	case NL80211_IFTYPE_STATION:
		if (!test_sta_flag(sta, WLAN_STA_TDLS_PEER)) {
			statype = CFG80211_STA_AP_STA;
			break;
		}
		if (test_sta_flag(sta, WLAN_STA_AUTHORIZED))
			statype = CFG80211_STA_TDLS_PEER_ACTIVE;
		else
			statype = CFG80211_STA_TDLS_PEER_SETUP;
		break;
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
		if (test_sta_flag(sta, WLAN_STA_ASSOC))
			statype = CFG80211_STA_AP_CLIENT;
		else
			statype = CFG80211_STA_AP_CLIENT_UNASSOC;
		break;
	default:
		return -EOPNOTSUPP;
	}

	err = cfg80211_check_station_change(wiphy, params, statype);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (params->vlan && params->vlan != sta->sdata->dev) {
		vlansdata = IEEE80211_DEV_TO_SUB_IF(params->vlan);

<<<<<<< HEAD
		if (vlansdata->vif.type != NL80211_IFTYPE_AP_VLAN &&
		    vlansdata->vif.type != NL80211_IFTYPE_AP) {
			mutex_unlock(&local->sta_mtx);
			return -EINVAL;
		}

		if (params->vlan->ieee80211_ptr->use_4addr) {
			if (vlansdata->u.vlan.sta) {
				mutex_unlock(&local->sta_mtx);
				return -EBUSY;
			}

			rcu_assign_pointer(vlansdata->u.vlan.sta, sta);
		}

		sta->sdata = vlansdata;
		ieee80211_send_layer2_update(sta);
	}

	err = sta_apply_parameters(local, sta, params);
	if (err) {
		mutex_unlock(&local->sta_mtx);
		return err;
	}

	if (test_sta_flag(sta, WLAN_STA_TDLS_PEER) && params->supported_rates)
		rate_control_rate_init(sta);

	mutex_unlock(&local->sta_mtx);

	if (sdata->vif.type == NL80211_IFTYPE_STATION &&
	    params->sta_flags_mask & BIT(NL80211_STA_FLAG_AUTHORIZED))
		ieee80211_recalc_ps(local, -1);
=======
		if (params->vlan->ieee80211_ptr->use_4addr) {
			if (vlansdata->u.vlan.sta)
				return -EBUSY;

			rcu_assign_pointer(vlansdata->u.vlan.sta, sta);
			__ieee80211_check_fast_rx_iface(vlansdata);
			drv_sta_set_4addr(local, sta->sdata, &sta->sta, true);
		}

		if (sta->sdata->vif.type == NL80211_IFTYPE_AP_VLAN &&
		    sta->sdata->u.vlan.sta)
			RCU_INIT_POINTER(sta->sdata->u.vlan.sta, NULL);

		if (test_sta_flag(sta, WLAN_STA_AUTHORIZED))
			ieee80211_vif_dec_num_mcast(sta->sdata);

		sta->sdata = vlansdata;
		ieee80211_check_fast_rx(sta);
		ieee80211_check_fast_xmit(sta);

		if (test_sta_flag(sta, WLAN_STA_AUTHORIZED)) {
			ieee80211_vif_inc_num_mcast(sta->sdata);
			cfg80211_send_layer2_update(sta->sdata->dev,
						    sta->sta.addr);
		}
	}

	err = sta_apply_parameters(local, sta, params);
	if (err)
		return err;

	if (sdata->vif.type == NL80211_IFTYPE_STATION &&
	    params->sta_flags_mask & BIT(NL80211_STA_FLAG_AUTHORIZED)) {
		ieee80211_recalc_ps(local);
		ieee80211_recalc_ps_vif(sdata);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

#ifdef CONFIG_MAC80211_MESH
static int ieee80211_add_mpath(struct wiphy *wiphy, struct net_device *dev,
<<<<<<< HEAD
				 u8 *dst, u8 *next_hop)
=======
			       const u8 *dst, const u8 *next_hop)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;
	struct sta_info *sta;
<<<<<<< HEAD
	int err;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();
	sta = sta_info_get(sdata, next_hop);
	if (!sta) {
		rcu_read_unlock();
		return -ENOENT;
	}

<<<<<<< HEAD
	err = mesh_path_add(dst, sdata);
	if (err) {
		rcu_read_unlock();
		return err;
	}

	mpath = mesh_path_lookup(dst, sdata);
	if (!mpath) {
		rcu_read_unlock();
		return -ENXIO;
	}
=======
	mpath = mesh_path_add(sdata, dst);
	if (IS_ERR(mpath)) {
		rcu_read_unlock();
		return PTR_ERR(mpath);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mesh_path_fix_nexthop(mpath, sta);

	rcu_read_unlock();
	return 0;
}

static int ieee80211_del_mpath(struct wiphy *wiphy, struct net_device *dev,
<<<<<<< HEAD
				 u8 *dst)
=======
			       const u8 *dst)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (dst)
<<<<<<< HEAD
		return mesh_path_del(dst, sdata);
=======
		return mesh_path_del(sdata, dst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mesh_path_flush_by_iface(sdata);
	return 0;
}

<<<<<<< HEAD
static int ieee80211_change_mpath(struct wiphy *wiphy,
				    struct net_device *dev,
				    u8 *dst, u8 *next_hop)
=======
static int ieee80211_change_mpath(struct wiphy *wiphy, struct net_device *dev,
				  const u8 *dst, const u8 *next_hop)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;
	struct sta_info *sta;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();

	sta = sta_info_get(sdata, next_hop);
	if (!sta) {
		rcu_read_unlock();
		return -ENOENT;
	}

<<<<<<< HEAD
	mpath = mesh_path_lookup(dst, sdata);
=======
	mpath = mesh_path_lookup(sdata, dst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!mpath) {
		rcu_read_unlock();
		return -ENOENT;
	}

	mesh_path_fix_nexthop(mpath, sta);

	rcu_read_unlock();
	return 0;
}

static void mpath_set_pinfo(struct mesh_path *mpath, u8 *next_hop,
			    struct mpath_info *pinfo)
{
	struct sta_info *next_hop_sta = rcu_dereference(mpath->next_hop);

	if (next_hop_sta)
		memcpy(next_hop, next_hop_sta->sta.addr, ETH_ALEN);
	else
<<<<<<< HEAD
		memset(next_hop, 0, ETH_ALEN);

	pinfo->generation = mesh_paths_generation;
=======
		eth_zero_addr(next_hop);

	memset(pinfo, 0, sizeof(*pinfo));

	pinfo->generation = mpath->sdata->u.mesh.mesh_paths_generation;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pinfo->filled = MPATH_INFO_FRAME_QLEN |
			MPATH_INFO_SN |
			MPATH_INFO_METRIC |
			MPATH_INFO_EXPTIME |
			MPATH_INFO_DISCOVERY_TIMEOUT |
			MPATH_INFO_DISCOVERY_RETRIES |
<<<<<<< HEAD
			MPATH_INFO_FLAGS;
=======
			MPATH_INFO_FLAGS |
			MPATH_INFO_HOP_COUNT |
			MPATH_INFO_PATH_CHANGE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pinfo->frame_qlen = mpath->frame_queue.qlen;
	pinfo->sn = mpath->sn;
	pinfo->metric = mpath->metric;
	if (time_before(jiffies, mpath->exp_time))
		pinfo->exptime = jiffies_to_msecs(mpath->exp_time - jiffies);
	pinfo->discovery_timeout =
			jiffies_to_msecs(mpath->discovery_timeout);
	pinfo->discovery_retries = mpath->discovery_retries;
<<<<<<< HEAD
	pinfo->flags = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mpath->flags & MESH_PATH_ACTIVE)
		pinfo->flags |= NL80211_MPATH_FLAG_ACTIVE;
	if (mpath->flags & MESH_PATH_RESOLVING)
		pinfo->flags |= NL80211_MPATH_FLAG_RESOLVING;
	if (mpath->flags & MESH_PATH_SN_VALID)
		pinfo->flags |= NL80211_MPATH_FLAG_SN_VALID;
	if (mpath->flags & MESH_PATH_FIXED)
		pinfo->flags |= NL80211_MPATH_FLAG_FIXED;
<<<<<<< HEAD
	if (mpath->flags & MESH_PATH_RESOLVING)
		pinfo->flags |= NL80211_MPATH_FLAG_RESOLVING;

	pinfo->flags = mpath->flags;
=======
	if (mpath->flags & MESH_PATH_RESOLVED)
		pinfo->flags |= NL80211_MPATH_FLAG_RESOLVED;
	pinfo->hop_count = mpath->hop_count;
	pinfo->path_change_count = mpath->path_change_count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ieee80211_get_mpath(struct wiphy *wiphy, struct net_device *dev,
			       u8 *dst, u8 *next_hop, struct mpath_info *pinfo)

{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();
<<<<<<< HEAD
	mpath = mesh_path_lookup(dst, sdata);
=======
	mpath = mesh_path_lookup(sdata, dst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!mpath) {
		rcu_read_unlock();
		return -ENOENT;
	}
	memcpy(dst, mpath->dst, ETH_ALEN);
	mpath_set_pinfo(mpath, next_hop, pinfo);
	rcu_read_unlock();
	return 0;
}

static int ieee80211_dump_mpath(struct wiphy *wiphy, struct net_device *dev,
<<<<<<< HEAD
				 int idx, u8 *dst, u8 *next_hop,
				 struct mpath_info *pinfo)
=======
				int idx, u8 *dst, u8 *next_hop,
				struct mpath_info *pinfo)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();
<<<<<<< HEAD
	mpath = mesh_path_lookup_by_idx(idx, sdata);
=======
	mpath = mesh_path_lookup_by_idx(sdata, idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!mpath) {
		rcu_read_unlock();
		return -ENOENT;
	}
	memcpy(dst, mpath->dst, ETH_ALEN);
	mpath_set_pinfo(mpath, next_hop, pinfo);
	rcu_read_unlock();
	return 0;
}

<<<<<<< HEAD
=======
static void mpp_set_pinfo(struct mesh_path *mpath, u8 *mpp,
			  struct mpath_info *pinfo)
{
	memset(pinfo, 0, sizeof(*pinfo));
	memcpy(mpp, mpath->mpp, ETH_ALEN);

	pinfo->generation = mpath->sdata->u.mesh.mpp_paths_generation;
}

static int ieee80211_get_mpp(struct wiphy *wiphy, struct net_device *dev,
			     u8 *dst, u8 *mpp, struct mpath_info *pinfo)

{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();
	mpath = mpp_path_lookup(sdata, dst);
	if (!mpath) {
		rcu_read_unlock();
		return -ENOENT;
	}
	memcpy(dst, mpath->dst, ETH_ALEN);
	mpp_set_pinfo(mpath, mpp, pinfo);
	rcu_read_unlock();
	return 0;
}

static int ieee80211_dump_mpp(struct wiphy *wiphy, struct net_device *dev,
			      int idx, u8 *dst, u8 *mpp,
			      struct mpath_info *pinfo)
{
	struct ieee80211_sub_if_data *sdata;
	struct mesh_path *mpath;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	rcu_read_lock();
	mpath = mpp_path_lookup_by_idx(sdata, idx);
	if (!mpath) {
		rcu_read_unlock();
		return -ENOENT;
	}
	memcpy(dst, mpath->dst, ETH_ALEN);
	mpp_set_pinfo(mpath, mpp, pinfo);
	rcu_read_unlock();
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ieee80211_get_mesh_config(struct wiphy *wiphy,
				struct net_device *dev,
				struct mesh_config *conf)
{
	struct ieee80211_sub_if_data *sdata;
	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	memcpy(conf, &(sdata->u.mesh.mshcfg), sizeof(struct mesh_config));
	return 0;
}

static inline bool _chg_mesh_attr(enum nl80211_meshconf_params parm, u32 mask)
{
	return (mask >> (parm-1)) & 0x1;
}

static int copy_mesh_setup(struct ieee80211_if_mesh *ifmsh,
		const struct mesh_setup *setup)
{
	u8 *new_ie;
<<<<<<< HEAD
	const u8 *old_ie;
	struct ieee80211_sub_if_data *sdata = container_of(ifmsh,
					struct ieee80211_sub_if_data, u.mesh);

	/* allocate information elements */
	new_ie = NULL;
	old_ie = ifmsh->ie;
=======
	struct ieee80211_sub_if_data *sdata = container_of(ifmsh,
					struct ieee80211_sub_if_data, u.mesh);
	int i;

	/* allocate information elements */
	new_ie = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (setup->ie_len) {
		new_ie = kmemdup(setup->ie, setup->ie_len,
				GFP_KERNEL);
		if (!new_ie)
			return -ENOMEM;
	}
	ifmsh->ie_len = setup->ie_len;
	ifmsh->ie = new_ie;
<<<<<<< HEAD
	kfree(old_ie);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* now copy the rest of the setup parameters */
	ifmsh->mesh_id_len = setup->mesh_id_len;
	memcpy(ifmsh->mesh_id, setup->mesh_id, ifmsh->mesh_id_len);
<<<<<<< HEAD
	ifmsh->mesh_pp_id = setup->path_sel_proto;
	ifmsh->mesh_pm_id = setup->path_metric;
	ifmsh->security = IEEE80211_MESH_SEC_NONE;
=======
	ifmsh->mesh_sp_id = setup->sync_method;
	ifmsh->mesh_pp_id = setup->path_sel_proto;
	ifmsh->mesh_pm_id = setup->path_metric;
	ifmsh->user_mpm = setup->user_mpm;
	ifmsh->mesh_auth_id = setup->auth_id;
	ifmsh->security = IEEE80211_MESH_SEC_NONE;
	ifmsh->userspace_handles_dfs = setup->userspace_handles_dfs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (setup->is_authenticated)
		ifmsh->security |= IEEE80211_MESH_SEC_AUTHED;
	if (setup->is_secure)
		ifmsh->security |= IEEE80211_MESH_SEC_SECURED;

	/* mcast rate setting in Mesh Node */
	memcpy(sdata->vif.bss_conf.mcast_rate, setup->mcast_rate,
						sizeof(setup->mcast_rate));
<<<<<<< HEAD
=======
	sdata->vif.bss_conf.basic_rates = setup->basic_rates;

	sdata->vif.bss_conf.beacon_int = setup->beacon_interval;
	sdata->vif.bss_conf.dtim_period = setup->dtim_period;

	sdata->beacon_rate_set = false;
	if (wiphy_ext_feature_isset(sdata->local->hw.wiphy,
				    NL80211_EXT_FEATURE_BEACON_RATE_LEGACY)) {
		for (i = 0; i < NUM_NL80211_BANDS; i++) {
			sdata->beacon_rateidx_mask[i] =
				setup->beacon_rate.control[i].legacy;
			if (sdata->beacon_rateidx_mask[i])
				sdata->beacon_rate_set = true;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int ieee80211_update_mesh_config(struct wiphy *wiphy,
					struct net_device *dev, u32 mask,
					const struct mesh_config *nconf)
{
	struct mesh_config *conf;
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_if_mesh *ifmsh;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	ifmsh = &sdata->u.mesh;

	/* Set the config options which we are interested in setting */
	conf = &(sdata->u.mesh.mshcfg);
	if (_chg_mesh_attr(NL80211_MESHCONF_RETRY_TIMEOUT, mask))
		conf->dot11MeshRetryTimeout = nconf->dot11MeshRetryTimeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_CONFIRM_TIMEOUT, mask))
		conf->dot11MeshConfirmTimeout = nconf->dot11MeshConfirmTimeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_HOLDING_TIMEOUT, mask))
		conf->dot11MeshHoldingTimeout = nconf->dot11MeshHoldingTimeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_MAX_PEER_LINKS, mask))
		conf->dot11MeshMaxPeerLinks = nconf->dot11MeshMaxPeerLinks;
	if (_chg_mesh_attr(NL80211_MESHCONF_MAX_RETRIES, mask))
		conf->dot11MeshMaxRetries = nconf->dot11MeshMaxRetries;
	if (_chg_mesh_attr(NL80211_MESHCONF_TTL, mask))
		conf->dot11MeshTTL = nconf->dot11MeshTTL;
	if (_chg_mesh_attr(NL80211_MESHCONF_ELEMENT_TTL, mask))
<<<<<<< HEAD
		conf->dot11MeshTTL = nconf->element_ttl;
	if (_chg_mesh_attr(NL80211_MESHCONF_AUTO_OPEN_PLINKS, mask))
		conf->auto_open_plinks = nconf->auto_open_plinks;
=======
		conf->element_ttl = nconf->element_ttl;
	if (_chg_mesh_attr(NL80211_MESHCONF_AUTO_OPEN_PLINKS, mask)) {
		if (ifmsh->user_mpm)
			return -EBUSY;
		conf->auto_open_plinks = nconf->auto_open_plinks;
	}
	if (_chg_mesh_attr(NL80211_MESHCONF_SYNC_OFFSET_MAX_NEIGHBOR, mask))
		conf->dot11MeshNbrOffsetMaxNeighbor =
			nconf->dot11MeshNbrOffsetMaxNeighbor;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_MAX_PREQ_RETRIES, mask))
		conf->dot11MeshHWMPmaxPREQretries =
			nconf->dot11MeshHWMPmaxPREQretries;
	if (_chg_mesh_attr(NL80211_MESHCONF_PATH_REFRESH_TIME, mask))
		conf->path_refresh_time = nconf->path_refresh_time;
	if (_chg_mesh_attr(NL80211_MESHCONF_MIN_DISCOVERY_TIMEOUT, mask))
		conf->min_discovery_timeout = nconf->min_discovery_timeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_ACTIVE_PATH_TIMEOUT, mask))
		conf->dot11MeshHWMPactivePathTimeout =
			nconf->dot11MeshHWMPactivePathTimeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_PREQ_MIN_INTERVAL, mask))
		conf->dot11MeshHWMPpreqMinInterval =
			nconf->dot11MeshHWMPpreqMinInterval;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_PERR_MIN_INTERVAL, mask))
		conf->dot11MeshHWMPperrMinInterval =
			nconf->dot11MeshHWMPperrMinInterval;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_NET_DIAM_TRVS_TIME,
			   mask))
		conf->dot11MeshHWMPnetDiameterTraversalTime =
			nconf->dot11MeshHWMPnetDiameterTraversalTime;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_ROOTMODE, mask)) {
		conf->dot11MeshHWMPRootMode = nconf->dot11MeshHWMPRootMode;
		ieee80211_mesh_root_setup(ifmsh);
	}
	if (_chg_mesh_attr(NL80211_MESHCONF_GATE_ANNOUNCEMENTS, mask)) {
		/* our current gate announcement implementation rides on root
		 * announcements, so require this ifmsh to also be a root node
		 * */
		if (nconf->dot11MeshGateAnnouncementProtocol &&
<<<<<<< HEAD
		    !conf->dot11MeshHWMPRootMode) {
			conf->dot11MeshHWMPRootMode = 1;
=======
		    !(conf->dot11MeshHWMPRootMode > IEEE80211_ROOTMODE_ROOT)) {
			conf->dot11MeshHWMPRootMode = IEEE80211_PROACTIVE_RANN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ieee80211_mesh_root_setup(ifmsh);
		}
		conf->dot11MeshGateAnnouncementProtocol =
			nconf->dot11MeshGateAnnouncementProtocol;
	}
<<<<<<< HEAD
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_RANN_INTERVAL, mask)) {
		conf->dot11MeshHWMPRannInterval =
			nconf->dot11MeshHWMPRannInterval;
	}
=======
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_RANN_INTERVAL, mask))
		conf->dot11MeshHWMPRannInterval =
			nconf->dot11MeshHWMPRannInterval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (_chg_mesh_attr(NL80211_MESHCONF_FORWARDING, mask))
		conf->dot11MeshForwarding = nconf->dot11MeshForwarding;
	if (_chg_mesh_attr(NL80211_MESHCONF_RSSI_THRESHOLD, mask)) {
		/* our RSSI threshold implementation is supported only for
		 * devices that report signal in dBm.
		 */
<<<<<<< HEAD
		if (!(sdata->local->hw.flags & IEEE80211_HW_SIGNAL_DBM))
			return -ENOTSUPP;
		conf->rssi_threshold = nconf->rssi_threshold;
	}
=======
		if (!ieee80211_hw_check(&sdata->local->hw, SIGNAL_DBM))
			return -EOPNOTSUPP;
		conf->rssi_threshold = nconf->rssi_threshold;
	}
	if (_chg_mesh_attr(NL80211_MESHCONF_HT_OPMODE, mask)) {
		conf->ht_opmode = nconf->ht_opmode;
		sdata->vif.bss_conf.ht_operation_mode = nconf->ht_opmode;
		ieee80211_link_info_change_notify(sdata, &sdata->deflink,
						  BSS_CHANGED_HT);
	}
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_PATH_TO_ROOT_TIMEOUT, mask))
		conf->dot11MeshHWMPactivePathToRootTimeout =
			nconf->dot11MeshHWMPactivePathToRootTimeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_ROOT_INTERVAL, mask))
		conf->dot11MeshHWMProotInterval =
			nconf->dot11MeshHWMProotInterval;
	if (_chg_mesh_attr(NL80211_MESHCONF_HWMP_CONFIRMATION_INTERVAL, mask))
		conf->dot11MeshHWMPconfirmationInterval =
			nconf->dot11MeshHWMPconfirmationInterval;
	if (_chg_mesh_attr(NL80211_MESHCONF_POWER_MODE, mask)) {
		conf->power_mode = nconf->power_mode;
		ieee80211_mps_local_status_update(sdata);
	}
	if (_chg_mesh_attr(NL80211_MESHCONF_AWAKE_WINDOW, mask))
		conf->dot11MeshAwakeWindowDuration =
			nconf->dot11MeshAwakeWindowDuration;
	if (_chg_mesh_attr(NL80211_MESHCONF_PLINK_TIMEOUT, mask))
		conf->plink_timeout = nconf->plink_timeout;
	if (_chg_mesh_attr(NL80211_MESHCONF_CONNECTED_TO_GATE, mask))
		conf->dot11MeshConnectedToMeshGate =
			nconf->dot11MeshConnectedToMeshGate;
	if (_chg_mesh_attr(NL80211_MESHCONF_NOLEARN, mask))
		conf->dot11MeshNolearn = nconf->dot11MeshNolearn;
	if (_chg_mesh_attr(NL80211_MESHCONF_CONNECTED_TO_AS, mask))
		conf->dot11MeshConnectedToAuthServer =
			nconf->dot11MeshConnectedToAuthServer;
	ieee80211_mbss_info_change_notify(sdata, BSS_CHANGED_BEACON);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int ieee80211_join_mesh(struct wiphy *wiphy, struct net_device *dev,
			       const struct mesh_config *conf,
			       const struct mesh_setup *setup)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
<<<<<<< HEAD
	struct ieee80211_if_mesh *ifmsh = &sdata->u.mesh;
	int err;

=======
	struct ieee80211_chan_req chanreq = { .oper = setup->chandef };
	struct ieee80211_if_mesh *ifmsh = &sdata->u.mesh;
	int err;

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(&ifmsh->mshcfg, conf, sizeof(struct mesh_config));
	err = copy_mesh_setup(ifmsh, setup);
	if (err)
		return err;
<<<<<<< HEAD
	ieee80211_start_mesh(sdata);

	return 0;
=======

	sdata->control_port_over_nl80211 = setup->control_port_over_nl80211;

	/* can mesh use other SMPS modes? */
	sdata->deflink.smps_mode = IEEE80211_SMPS_OFF;
	sdata->deflink.needed_rx_chains = sdata->local->rx_chains;

	err = ieee80211_link_use_channel(&sdata->deflink, &chanreq,
					 IEEE80211_CHANCTX_SHARED);
	if (err)
		return err;

	return ieee80211_start_mesh(sdata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ieee80211_leave_mesh(struct wiphy *wiphy, struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

<<<<<<< HEAD
	ieee80211_stop_mesh(sdata);
=======
	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	ieee80211_stop_mesh(sdata);
	ieee80211_link_release_channel(&sdata->deflink);
	kfree(sdata->u.mesh.ie);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
#endif

static int ieee80211_change_bss(struct wiphy *wiphy,
				struct net_device *dev,
				struct bss_parameters *params)
{
<<<<<<< HEAD
	struct ieee80211_sub_if_data *sdata;
	u32 changed = 0;

	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (params->use_cts_prot >= 0) {
		sdata->vif.bss_conf.use_cts_prot = params->use_cts_prot;
		changed |= BSS_CHANGED_ERP_CTS_PROT;
	}
	if (params->use_short_preamble >= 0) {
		sdata->vif.bss_conf.use_short_preamble =
			params->use_short_preamble;
		changed |= BSS_CHANGED_ERP_PREAMBLE;
	}

	if (!sdata->vif.bss_conf.use_short_slot &&
	    sdata->local->hw.conf.channel->band == IEEE80211_BAND_5GHZ) {
		sdata->vif.bss_conf.use_short_slot = true;
=======
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_link_data *link;
	struct ieee80211_supported_band *sband;
	u64 changed = 0;

	link = ieee80211_link_or_deflink(sdata, params->link_id, true);
	if (IS_ERR(link))
		return PTR_ERR(link);

	if (!sdata_dereference(link->u.ap.beacon, sdata))
		return -ENOENT;

	sband = ieee80211_get_link_sband(link);
	if (!sband)
		return -EINVAL;

	if (params->basic_rates) {
		if (!ieee80211_parse_bitrates(link->conf->chanreq.oper.width,
					      wiphy->bands[sband->band],
					      params->basic_rates,
					      params->basic_rates_len,
					      &link->conf->basic_rates))
			return -EINVAL;
		changed |= BSS_CHANGED_BASIC_RATES;
		ieee80211_check_rate_mask(link);
	}

	if (params->use_cts_prot >= 0) {
		link->conf->use_cts_prot = params->use_cts_prot;
		changed |= BSS_CHANGED_ERP_CTS_PROT;
	}
	if (params->use_short_preamble >= 0) {
		link->conf->use_short_preamble = params->use_short_preamble;
		changed |= BSS_CHANGED_ERP_PREAMBLE;
	}

	if (!link->conf->use_short_slot &&
	    (sband->band == NL80211_BAND_5GHZ ||
	     sband->band == NL80211_BAND_6GHZ)) {
		link->conf->use_short_slot = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		changed |= BSS_CHANGED_ERP_SLOT;
	}

	if (params->use_short_slot_time >= 0) {
<<<<<<< HEAD
		sdata->vif.bss_conf.use_short_slot =
			params->use_short_slot_time;
		changed |= BSS_CHANGED_ERP_SLOT;
	}

	if (params->basic_rates) {
		int i, j;
		u32 rates = 0;
		struct ieee80211_local *local = wiphy_priv(wiphy);
		struct ieee80211_supported_band *sband =
			wiphy->bands[local->oper_channel->band];

		for (i = 0; i < params->basic_rates_len; i++) {
			int rate = (params->basic_rates[i] & 0x7f) * 5;
			for (j = 0; j < sband->n_bitrates; j++) {
				if (sband->bitrates[j].bitrate == rate)
					rates |= BIT(j);
			}
		}
		sdata->vif.bss_conf.basic_rates = rates;
		changed |= BSS_CHANGED_BASIC_RATES;
	}

=======
		link->conf->use_short_slot = params->use_short_slot_time;
		changed |= BSS_CHANGED_ERP_SLOT;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (params->ap_isolate >= 0) {
		if (params->ap_isolate)
			sdata->flags |= IEEE80211_SDATA_DONT_BRIDGE_PACKETS;
		else
			sdata->flags &= ~IEEE80211_SDATA_DONT_BRIDGE_PACKETS;
<<<<<<< HEAD
	}

	if (params->ht_opmode >= 0) {
		sdata->vif.bss_conf.ht_operation_mode =
			(u16) params->ht_opmode;
		changed |= BSS_CHANGED_HT;
	}

	ieee80211_bss_info_change_notify(sdata, changed);
=======
		ieee80211_check_fast_rx_iface(sdata);
	}

	if (params->ht_opmode >= 0) {
		link->conf->ht_operation_mode = (u16)params->ht_opmode;
		changed |= BSS_CHANGED_HT;
	}

	if (params->p2p_ctwindow >= 0) {
		link->conf->p2p_noa_attr.oppps_ctwindow &=
					~IEEE80211_P2P_OPPPS_CTWINDOW_MASK;
		link->conf->p2p_noa_attr.oppps_ctwindow |=
			params->p2p_ctwindow & IEEE80211_P2P_OPPPS_CTWINDOW_MASK;
		changed |= BSS_CHANGED_P2P_PS;
	}

	if (params->p2p_opp_ps > 0) {
		link->conf->p2p_noa_attr.oppps_ctwindow |=
					IEEE80211_P2P_OPPPS_ENABLE_BIT;
		changed |= BSS_CHANGED_P2P_PS;
	} else if (params->p2p_opp_ps == 0) {
		link->conf->p2p_noa_attr.oppps_ctwindow &=
					~IEEE80211_P2P_OPPPS_ENABLE_BIT;
		changed |= BSS_CHANGED_P2P_PS;
	}

	ieee80211_link_info_change_notify(sdata, link, changed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int ieee80211_set_txq_params(struct wiphy *wiphy,
				    struct net_device *dev,
				    struct ieee80211_txq_params *params)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
<<<<<<< HEAD
=======
	struct ieee80211_link_data *link =
		ieee80211_link_or_deflink(sdata, params->link_id, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ieee80211_tx_queue_params p;

	if (!local->ops->conf_tx)
		return -EOPNOTSUPP;

<<<<<<< HEAD
=======
	if (local->hw.queues < IEEE80211_NUM_ACS)
		return -EOPNOTSUPP;

	if (IS_ERR(link))
		return PTR_ERR(link);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memset(&p, 0, sizeof(p));
	p.aifs = params->aifs;
	p.cw_max = params->cwmax;
	p.cw_min = params->cwmin;
	p.txop = params->txop;

	/*
	 * Setting tx queue params disables u-apsd because it's only
	 * called in master mode.
	 */
	p.uapsd = false;

<<<<<<< HEAD
	if (params->queue >= local->hw.queues)
		return -EINVAL;

	sdata->tx_conf[params->queue] = p;
	if (drv_conf_tx(local, sdata, params->queue, &p)) {
		wiphy_debug(local->hw.wiphy,
			    "failed to set TX queue parameters for queue %d\n",
			    params->queue);
		return -EINVAL;
	}

	return 0;
}

static int ieee80211_set_channel(struct wiphy *wiphy,
				 struct net_device *netdev,
				 struct ieee80211_channel *chan,
				 enum nl80211_channel_type channel_type)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = NULL;
	struct ieee80211_channel *old_oper;
	enum nl80211_channel_type old_oper_type;
	enum nl80211_channel_type old_vif_oper_type= NL80211_CHAN_NO_HT;

	if (netdev)
		sdata = IEEE80211_DEV_TO_SUB_IF(netdev);

	switch (ieee80211_get_channel_mode(local, NULL)) {
	case CHAN_MODE_HOPPING:
		return -EBUSY;
	case CHAN_MODE_FIXED:
		if (local->oper_channel != chan)
			return -EBUSY;
		if (!sdata && local->_oper_channel_type == channel_type)
			return 0;
		break;
	case CHAN_MODE_UNDEFINED:
		break;
	}

	if (sdata)
		old_vif_oper_type = sdata->vif.bss_conf.channel_type;
	old_oper_type = local->_oper_channel_type;

	if (!ieee80211_set_channel_type(local, sdata, channel_type))
		return -EBUSY;

	old_oper = local->oper_channel;
	local->oper_channel = chan;

	/* Update driver if changes were actually made. */
	if ((old_oper != local->oper_channel) ||
	    (old_oper_type != local->_oper_channel_type))
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_CHANNEL);

	if (sdata && sdata->vif.type != NL80211_IFTYPE_MONITOR &&
	    old_vif_oper_type != sdata->vif.bss_conf.channel_type)
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_HT);
=======
	ieee80211_regulatory_limit_wmm_params(sdata, &p, params->ac);

	link->tx_conf[params->ac] = p;
	if (drv_conf_tx(local, link, params->ac, &p)) {
		wiphy_debug(local->hw.wiphy,
			    "failed to set TX queue parameters for AC %d\n",
			    params->ac);
		return -EINVAL;
	}

	ieee80211_link_info_change_notify(sdata, link,
					  BSS_CHANGED_QOS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

#ifdef CONFIG_PM
static int ieee80211_suspend(struct wiphy *wiphy,
			     struct cfg80211_wowlan *wowlan)
{
	return __ieee80211_suspend(wiphy_priv(wiphy), wowlan);
}

static int ieee80211_resume(struct wiphy *wiphy)
{
	return __ieee80211_resume(wiphy_priv(wiphy));
}
#else
#define ieee80211_suspend NULL
#define ieee80211_resume NULL
#endif

static int ieee80211_scan(struct wiphy *wiphy,
<<<<<<< HEAD
			  struct net_device *dev,
			  struct cfg80211_scan_request *req)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
=======
			  struct cfg80211_scan_request *req)
{
	struct ieee80211_sub_if_data *sdata;

	sdata = IEEE80211_WDEV_TO_SUB_IF(req->wdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (ieee80211_vif_type_p2p(&sdata->vif)) {
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_MESH_POINT:
	case NL80211_IFTYPE_P2P_CLIENT:
<<<<<<< HEAD
=======
	case NL80211_IFTYPE_P2P_DEVICE:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case NL80211_IFTYPE_P2P_GO:
		if (sdata->local->ops->hw_scan)
			break;
		/*
		 * FIXME: implement NoA while scanning in software,
		 * for now fall through to allow scanning only when
		 * beaconing hasn't been configured yet
		 */
<<<<<<< HEAD
	case NL80211_IFTYPE_AP:
		if (sdata->u.ap.beacon)
			return -EOPNOTSUPP;
		break;
=======
		fallthrough;
	case NL80211_IFTYPE_AP:
		/*
		 * If the scan has been forced (and the driver supports
		 * forcing), don't care about being beaconing already.
		 * This will create problems to the attached stations (e.g. all
		 * the frames sent while scanning on other channel will be
		 * lost)
		 */
		if (sdata->deflink.u.ap.beacon &&
		    (!(wiphy->features & NL80211_FEATURE_AP_SCAN) ||
		     !(req->flags & NL80211_SCAN_FLAG_AP)))
			return -EOPNOTSUPP;
		break;
	case NL80211_IFTYPE_NAN:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -EOPNOTSUPP;
	}

	return ieee80211_request_scan(sdata, req);
}

<<<<<<< HEAD
=======
static void ieee80211_abort_scan(struct wiphy *wiphy, struct wireless_dev *wdev)
{
	ieee80211_scan_cancel(wiphy_priv(wiphy));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int
ieee80211_sched_scan_start(struct wiphy *wiphy,
			   struct net_device *dev,
			   struct cfg80211_sched_scan_request *req)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (!sdata->local->ops->sched_scan_start)
		return -EOPNOTSUPP;

	return ieee80211_request_sched_scan_start(sdata, req);
}

static int
<<<<<<< HEAD
ieee80211_sched_scan_stop(struct wiphy *wiphy, struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (!sdata->local->ops->sched_scan_stop)
		return -EOPNOTSUPP;

	return ieee80211_request_sched_scan_stop(sdata);
=======
ieee80211_sched_scan_stop(struct wiphy *wiphy, struct net_device *dev,
			  u64 reqid)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	if (!local->ops->sched_scan_stop)
		return -EOPNOTSUPP;

	return ieee80211_request_sched_scan_stop(local);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ieee80211_auth(struct wiphy *wiphy, struct net_device *dev,
			  struct cfg80211_auth_request *req)
{
	return ieee80211_mgd_auth(IEEE80211_DEV_TO_SUB_IF(dev), req);
}

static int ieee80211_assoc(struct wiphy *wiphy, struct net_device *dev,
			   struct cfg80211_assoc_request *req)
{
<<<<<<< HEAD
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	switch (ieee80211_get_channel_mode(local, sdata)) {
	case CHAN_MODE_HOPPING:
		return -EBUSY;
	case CHAN_MODE_FIXED:
		if (local->oper_channel == req->bss->channel)
			break;
		return -EBUSY;
	case CHAN_MODE_UNDEFINED:
		break;
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ieee80211_mgd_assoc(IEEE80211_DEV_TO_SUB_IF(dev), req);
}

static int ieee80211_deauth(struct wiphy *wiphy, struct net_device *dev,
			    struct cfg80211_deauth_request *req)
{
	return ieee80211_mgd_deauth(IEEE80211_DEV_TO_SUB_IF(dev), req);
}

static int ieee80211_disassoc(struct wiphy *wiphy, struct net_device *dev,
			      struct cfg80211_disassoc_request *req)
{
	return ieee80211_mgd_disassoc(IEEE80211_DEV_TO_SUB_IF(dev), req);
}

static int ieee80211_join_ibss(struct wiphy *wiphy, struct net_device *dev,
			       struct cfg80211_ibss_params *params)
{
<<<<<<< HEAD
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	switch (ieee80211_get_channel_mode(local, sdata)) {
	case CHAN_MODE_HOPPING:
		return -EBUSY;
	case CHAN_MODE_FIXED:
		if (!params->channel_fixed)
			return -EBUSY;
		if (local->oper_channel == params->channel)
			break;
		return -EBUSY;
	case CHAN_MODE_UNDEFINED:
		break;
	}

	return ieee80211_ibss_join(sdata, params);
=======
	return ieee80211_ibss_join(IEEE80211_DEV_TO_SUB_IF(dev), params);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ieee80211_leave_ibss(struct wiphy *wiphy, struct net_device *dev)
{
<<<<<<< HEAD
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	return ieee80211_ibss_leave(sdata);
=======
	return ieee80211_ibss_leave(IEEE80211_DEV_TO_SUB_IF(dev));
}

static int ieee80211_join_ocb(struct wiphy *wiphy, struct net_device *dev,
			      struct ocb_setup *setup)
{
	return ieee80211_ocb_join(IEEE80211_DEV_TO_SUB_IF(dev), setup);
}

static int ieee80211_leave_ocb(struct wiphy *wiphy, struct net_device *dev)
{
	return ieee80211_ocb_leave(IEEE80211_DEV_TO_SUB_IF(dev));
}

static int ieee80211_set_mcast_rate(struct wiphy *wiphy, struct net_device *dev,
				    int rate[NUM_NL80211_BANDS])
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	memcpy(sdata->vif.bss_conf.mcast_rate, rate,
	       sizeof(int) * NUM_NL80211_BANDS);

	ieee80211_link_info_change_notify(sdata, &sdata->deflink,
					  BSS_CHANGED_MCAST_RATE);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ieee80211_set_wiphy_params(struct wiphy *wiphy, u32 changed)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	int err;

	if (changed & WIPHY_PARAM_FRAG_THRESHOLD) {
<<<<<<< HEAD
		err = drv_set_frag_threshold(local, wiphy->frag_threshold);

		if (err)
			return err;
	}

	if (changed & WIPHY_PARAM_COVERAGE_CLASS) {
		err = drv_set_coverage_class(local, wiphy->coverage_class);
=======
		ieee80211_check_fast_xmit_all(local);

		err = drv_set_frag_threshold(local, wiphy->frag_threshold);

		if (err) {
			ieee80211_check_fast_xmit_all(local);
			return err;
		}
	}

	if ((changed & WIPHY_PARAM_COVERAGE_CLASS) ||
	    (changed & WIPHY_PARAM_DYN_ACK)) {
		s16 coverage_class;

		coverage_class = changed & WIPHY_PARAM_COVERAGE_CLASS ?
					wiphy->coverage_class : -1;
		err = drv_set_coverage_class(local, coverage_class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (err)
			return err;
	}

	if (changed & WIPHY_PARAM_RTS_THRESHOLD) {
		err = drv_set_rts_threshold(local, wiphy->rts_threshold);

		if (err)
			return err;
	}

<<<<<<< HEAD
	if (changed & WIPHY_PARAM_RETRY_SHORT)
		local->hw.conf.short_frame_max_tx_count = wiphy->retry_short;
	if (changed & WIPHY_PARAM_RETRY_LONG)
		local->hw.conf.long_frame_max_tx_count = wiphy->retry_long;
=======
	if (changed & WIPHY_PARAM_RETRY_SHORT) {
		if (wiphy->retry_short > IEEE80211_MAX_TX_RETRY)
			return -EINVAL;
		local->hw.conf.short_frame_max_tx_count = wiphy->retry_short;
	}
	if (changed & WIPHY_PARAM_RETRY_LONG) {
		if (wiphy->retry_long > IEEE80211_MAX_TX_RETRY)
			return -EINVAL;
		local->hw.conf.long_frame_max_tx_count = wiphy->retry_long;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (changed &
	    (WIPHY_PARAM_RETRY_SHORT | WIPHY_PARAM_RETRY_LONG))
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_RETRY_LIMITS);

<<<<<<< HEAD
=======
	if (changed & (WIPHY_PARAM_TXQ_LIMIT |
		       WIPHY_PARAM_TXQ_MEMORY_LIMIT |
		       WIPHY_PARAM_TXQ_QUANTUM))
		ieee80211_txq_set_params(local);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int ieee80211_set_tx_power(struct wiphy *wiphy,
<<<<<<< HEAD
				  enum nl80211_tx_power_setting type, int mbm)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_channel *chan = local->hw.conf.channel;
	u32 changes = 0;

	switch (type) {
	case NL80211_TX_POWER_AUTOMATIC:
		local->user_power_level = -1;
		break;
	case NL80211_TX_POWER_LIMITED:
		if (mbm < 0 || (mbm % 100))
			return -EOPNOTSUPP;
		local->user_power_level = MBM_TO_DBM(mbm);
		break;
	case NL80211_TX_POWER_FIXED:
		if (mbm < 0 || (mbm % 100))
			return -EOPNOTSUPP;
		/* TODO: move to cfg80211 when it knows the channel */
		if (MBM_TO_DBM(mbm) > chan->max_power)
			return -EINVAL;
=======
				  struct wireless_dev *wdev,
				  enum nl80211_tx_power_setting type, int mbm)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata;
	enum nl80211_tx_power_setting txp_type = type;
	bool update_txp_type = false;
	bool has_monitor = false;

	lockdep_assert_wiphy(local->hw.wiphy);

	if (wdev) {
		sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);

		if (sdata->vif.type == NL80211_IFTYPE_MONITOR) {
			sdata = wiphy_dereference(local->hw.wiphy,
						  local->monitor_sdata);
			if (!sdata)
				return -EOPNOTSUPP;
		}

		switch (type) {
		case NL80211_TX_POWER_AUTOMATIC:
			sdata->deflink.user_power_level =
				IEEE80211_UNSET_POWER_LEVEL;
			txp_type = NL80211_TX_POWER_LIMITED;
			break;
		case NL80211_TX_POWER_LIMITED:
		case NL80211_TX_POWER_FIXED:
			if (mbm < 0 || (mbm % 100))
				return -EOPNOTSUPP;
			sdata->deflink.user_power_level = MBM_TO_DBM(mbm);
			break;
		}

		if (txp_type != sdata->vif.bss_conf.txpower_type) {
			update_txp_type = true;
			sdata->vif.bss_conf.txpower_type = txp_type;
		}

		ieee80211_recalc_txpower(sdata, update_txp_type);

		return 0;
	}

	switch (type) {
	case NL80211_TX_POWER_AUTOMATIC:
		local->user_power_level = IEEE80211_UNSET_POWER_LEVEL;
		txp_type = NL80211_TX_POWER_LIMITED;
		break;
	case NL80211_TX_POWER_LIMITED:
	case NL80211_TX_POWER_FIXED:
		if (mbm < 0 || (mbm % 100))
			return -EOPNOTSUPP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		local->user_power_level = MBM_TO_DBM(mbm);
		break;
	}

<<<<<<< HEAD
	ieee80211_hw_config(local, changes);
=======
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (sdata->vif.type == NL80211_IFTYPE_MONITOR) {
			has_monitor = true;
			continue;
		}
		sdata->deflink.user_power_level = local->user_power_level;
		if (txp_type != sdata->vif.bss_conf.txpower_type)
			update_txp_type = true;
		sdata->vif.bss_conf.txpower_type = txp_type;
	}
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (sdata->vif.type == NL80211_IFTYPE_MONITOR)
			continue;
		ieee80211_recalc_txpower(sdata, update_txp_type);
	}

	if (has_monitor) {
		sdata = wiphy_dereference(local->hw.wiphy,
					  local->monitor_sdata);
		if (sdata) {
			sdata->deflink.user_power_level = local->user_power_level;
			if (txp_type != sdata->vif.bss_conf.txpower_type)
				update_txp_type = true;
			sdata->vif.bss_conf.txpower_type = txp_type;

			ieee80211_recalc_txpower(sdata, update_txp_type);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int ieee80211_get_tx_power(struct wiphy *wiphy, int *dbm)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	*dbm = local->hw.conf.power_level;

	return 0;
}

static int ieee80211_set_wds_peer(struct wiphy *wiphy, struct net_device *dev,
				  const u8 *addr)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	memcpy(&sdata->u.wds.remote_addr, addr, ETH_ALEN);
=======
static int ieee80211_get_tx_power(struct wiphy *wiphy,
				  struct wireless_dev *wdev,
				  int *dbm)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);

	if (local->ops->get_txpower)
		return drv_get_txpower(local, sdata, dbm);

	if (local->emulate_chanctx)
		*dbm = local->hw.conf.power_level;
	else
		*dbm = sdata->vif.bss_conf.txpower;

	/* INT_MIN indicates no power level was set yet */
	if (*dbm == INT_MIN)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void ieee80211_rfkill_poll(struct wiphy *wiphy)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	drv_rfkill_poll(local);
}

#ifdef CONFIG_NL80211_TESTMODE
<<<<<<< HEAD
static int ieee80211_testmode_cmd(struct wiphy *wiphy, void *data, int len)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
=======
static int ieee80211_testmode_cmd(struct wiphy *wiphy,
				  struct wireless_dev *wdev,
				  void *data, int len)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_vif *vif = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!local->ops->testmode_cmd)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return local->ops->testmode_cmd(&local->hw, data, len);
=======
	if (wdev) {
		struct ieee80211_sub_if_data *sdata;

		sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);
		if (sdata->flags & IEEE80211_SDATA_IN_DRIVER)
			vif = &sdata->vif;
	}

	return local->ops->testmode_cmd(&local->hw, vif, data, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ieee80211_testmode_dump(struct wiphy *wiphy,
				   struct sk_buff *skb,
				   struct netlink_callback *cb,
				   void *data, int len)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	if (!local->ops->testmode_dump)
		return -EOPNOTSUPP;

	return local->ops->testmode_dump(&local->hw, skb, cb, data, len);
}
#endif

<<<<<<< HEAD
int __ieee80211_request_smps(struct ieee80211_sub_if_data *sdata,
			     enum ieee80211_smps_mode smps_mode)
=======
int __ieee80211_request_smps_mgd(struct ieee80211_sub_if_data *sdata,
				 struct ieee80211_link_data *link,
				 enum ieee80211_smps_mode smps_mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const u8 *ap;
	enum ieee80211_smps_mode old_req;
	int err;
<<<<<<< HEAD

	lockdep_assert_held(&sdata->u.mgd.mtx);

	old_req = sdata->u.mgd.req_smps;
	sdata->u.mgd.req_smps = smps_mode;
=======
	struct sta_info *sta;
	bool tdls_peer_found = false;

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	if (WARN_ON_ONCE(sdata->vif.type != NL80211_IFTYPE_STATION))
		return -EINVAL;

	if (!ieee80211_vif_link_active(&sdata->vif, link->link_id))
		return 0;

	old_req = link->u.mgd.req_smps;
	link->u.mgd.req_smps = smps_mode;

	/* The driver indicated that EML is enabled for the interface, which
	 * implies that SMPS flows towards the AP should be stopped.
	 */
	if (sdata->vif.driver_flags & IEEE80211_VIF_EML_ACTIVE)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (old_req == smps_mode &&
	    smps_mode != IEEE80211_SMPS_AUTOMATIC)
		return 0;

	/*
	 * If not associated, or current association is not an HT
<<<<<<< HEAD
	 * association, there's no need to send an action frame.
	 */
	if (!sdata->u.mgd.associated ||
	    sdata->vif.bss_conf.channel_type == NL80211_CHAN_NO_HT) {
		mutex_lock(&sdata->local->iflist_mtx);
		ieee80211_recalc_smps(sdata->local);
		mutex_unlock(&sdata->local->iflist_mtx);
		return 0;
	}

	ap = sdata->u.mgd.associated->bssid;

	if (smps_mode == IEEE80211_SMPS_AUTOMATIC) {
		if (sdata->u.mgd.powersave)
			smps_mode = IEEE80211_SMPS_DYNAMIC;
		else
			smps_mode = IEEE80211_SMPS_OFF;
=======
	 * association, there's no need to do anything, just store
	 * the new value until we associate.
	 */
	if (!sdata->u.mgd.associated ||
	    link->conf->chanreq.oper.width == NL80211_CHAN_WIDTH_20_NOHT)
		return 0;

	ap = sdata->vif.cfg.ap_addr;

	rcu_read_lock();
	list_for_each_entry_rcu(sta, &sdata->local->sta_list, list) {
		if (!sta->sta.tdls || sta->sdata != sdata || !sta->uploaded ||
		    !test_sta_flag(sta, WLAN_STA_AUTHORIZED))
			continue;

		tdls_peer_found = true;
		break;
	}
	rcu_read_unlock();

	if (smps_mode == IEEE80211_SMPS_AUTOMATIC) {
		if (tdls_peer_found || !sdata->u.mgd.powersave)
			smps_mode = IEEE80211_SMPS_OFF;
		else
			smps_mode = IEEE80211_SMPS_DYNAMIC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* send SM PS frame to AP */
	err = ieee80211_send_smps_action(sdata, smps_mode,
<<<<<<< HEAD
					 ap, ap);
	if (err)
		sdata->u.mgd.req_smps = old_req;
=======
					 ap, ap,
					 ieee80211_vif_is_mld(&sdata->vif) ?
					 link->link_id : -1);
	if (err)
		link->u.mgd.req_smps = old_req;
	else if (smps_mode != IEEE80211_SMPS_OFF && tdls_peer_found)
		ieee80211_teardown_tdls_peers(link);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

static int ieee80211_set_power_mgmt(struct wiphy *wiphy, struct net_device *dev,
				    bool enabled, int timeout)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);
<<<<<<< HEAD
=======
	unsigned int link_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sdata->vif.type != NL80211_IFTYPE_STATION)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS))
=======
	if (!ieee80211_hw_check(&local->hw, SUPPORTS_PS))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EOPNOTSUPP;

	if (enabled == sdata->u.mgd.powersave &&
	    timeout == local->dynamic_ps_forced_timeout)
		return 0;

	sdata->u.mgd.powersave = enabled;
	local->dynamic_ps_forced_timeout = timeout;

	/* no change, but if automatic follow powersave */
<<<<<<< HEAD
	mutex_lock(&sdata->u.mgd.mtx);
	__ieee80211_request_smps(sdata, sdata->u.mgd.req_smps);
	mutex_unlock(&sdata->u.mgd.mtx);

	if (local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS)
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_PS);

	ieee80211_recalc_ps(local, -1);
=======
	for (link_id = 0; link_id < ARRAY_SIZE(sdata->link); link_id++) {
		struct ieee80211_link_data *link;

		link = sdata_dereference(sdata->link[link_id], sdata);

		if (!link)
			continue;
		__ieee80211_request_smps_mgd(sdata, link,
					     link->u.mgd.req_smps);
	}

	if (ieee80211_hw_check(&local->hw, SUPPORTS_DYNAMIC_PS))
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_PS);

	ieee80211_recalc_ps(local);
	ieee80211_recalc_ps_vif(sdata);
	ieee80211_check_fast_rx_iface(sdata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
=======
static void ieee80211_set_cqm_rssi_link(struct ieee80211_sub_if_data *sdata,
					struct ieee80211_link_data *link,
					s32 rssi_thold, u32 rssi_hyst,
					s32 rssi_low, s32 rssi_high)
{
	struct ieee80211_bss_conf *conf;

	if (!link || !link->conf)
		return;

	conf = link->conf;

	if (rssi_thold && rssi_hyst &&
	    rssi_thold == conf->cqm_rssi_thold &&
	    rssi_hyst == conf->cqm_rssi_hyst)
		return;

	conf->cqm_rssi_thold = rssi_thold;
	conf->cqm_rssi_hyst = rssi_hyst;
	conf->cqm_rssi_low = rssi_low;
	conf->cqm_rssi_high = rssi_high;
	link->u.mgd.last_cqm_event_signal = 0;

	if (!ieee80211_vif_link_active(&sdata->vif, link->link_id))
		return;

	if (sdata->u.mgd.associated &&
	    (sdata->vif.driver_flags & IEEE80211_VIF_SUPPORTS_CQM_RSSI))
		ieee80211_link_info_change_notify(sdata, link, BSS_CHANGED_CQM);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ieee80211_set_cqm_rssi_config(struct wiphy *wiphy,
					 struct net_device *dev,
					 s32 rssi_thold, u32 rssi_hyst)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_vif *vif = &sdata->vif;
<<<<<<< HEAD
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;

	if (rssi_thold == bss_conf->cqm_rssi_thold &&
	    rssi_hyst == bss_conf->cqm_rssi_hyst)
		return 0;

	bss_conf->cqm_rssi_thold = rssi_thold;
	bss_conf->cqm_rssi_hyst = rssi_hyst;

	/* tell the driver upon association, unless already associated */
	if (sdata->u.mgd.associated &&
	    sdata->vif.driver_flags & IEEE80211_VIF_SUPPORTS_CQM_RSSI)
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_CQM);
=======
	int link_id;

	if (vif->driver_flags & IEEE80211_VIF_BEACON_FILTER &&
	    !(vif->driver_flags & IEEE80211_VIF_SUPPORTS_CQM_RSSI))
		return -EOPNOTSUPP;

	/* For MLD, handle CQM change on all the active links */
	for (link_id = 0; link_id < IEEE80211_MLD_MAX_NUM_LINKS; link_id++) {
		struct ieee80211_link_data *link =
			sdata_dereference(sdata->link[link_id], sdata);

		ieee80211_set_cqm_rssi_link(sdata, link, rssi_thold, rssi_hyst,
					    0, 0);
	}

	return 0;
}

static int ieee80211_set_cqm_rssi_range_config(struct wiphy *wiphy,
					       struct net_device *dev,
					       s32 rssi_low, s32 rssi_high)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_vif *vif = &sdata->vif;
	int link_id;

	if (vif->driver_flags & IEEE80211_VIF_BEACON_FILTER)
		return -EOPNOTSUPP;

	/* For MLD, handle CQM change on all the active links */
	for (link_id = 0; link_id < IEEE80211_MLD_MAX_NUM_LINKS; link_id++) {
		struct ieee80211_link_data *link =
			sdata_dereference(sdata->link[link_id], sdata);

		ieee80211_set_cqm_rssi_link(sdata, link, 0, 0,
					    rssi_low, rssi_high);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int ieee80211_set_bitrate_mask(struct wiphy *wiphy,
				      struct net_device *dev,
<<<<<<< HEAD
=======
				      unsigned int link_id,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				      const u8 *addr,
				      const struct cfg80211_bitrate_mask *mask)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);
	int i, ret;

<<<<<<< HEAD
	if (local->hw.flags & IEEE80211_HW_HAS_RATE_CONTROL) {
=======
	if (!ieee80211_sdata_running(sdata))
		return -ENETDOWN;

	/*
	 * If active validate the setting and reject it if it doesn't leave
	 * at least one basic rate usable, since we really have to be able
	 * to send something, and if we're an AP we have to be able to do
	 * so at a basic rate so that all clients can receive it.
	 */
	if (rcu_access_pointer(sdata->vif.bss_conf.chanctx_conf) &&
	    sdata->vif.bss_conf.chanreq.oper.chan) {
		u32 basic_rates = sdata->vif.bss_conf.basic_rates;
		enum nl80211_band band;

		band = sdata->vif.bss_conf.chanreq.oper.chan->band;

		if (!(mask->control[band].legacy & basic_rates))
			return -EINVAL;
	}

	if (ieee80211_hw_check(&local->hw, HAS_RATE_CONTROL)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = drv_set_bitrate_mask(local, sdata, mask);
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	for (i = 0; i < IEEE80211_NUM_BANDS; i++) {
		sdata->rc_rateidx_mask[i] = mask->control[i].legacy;
		memcpy(sdata->rc_rateidx_mcs_mask[i], mask->control[i].mcs,
		       sizeof(mask->control[i].mcs));
=======
	for (i = 0; i < NUM_NL80211_BANDS; i++) {
		struct ieee80211_supported_band *sband = wiphy->bands[i];
		int j;

		sdata->rc_rateidx_mask[i] = mask->control[i].legacy;
		memcpy(sdata->rc_rateidx_mcs_mask[i], mask->control[i].ht_mcs,
		       sizeof(mask->control[i].ht_mcs));
		memcpy(sdata->rc_rateidx_vht_mcs_mask[i],
		       mask->control[i].vht_mcs,
		       sizeof(mask->control[i].vht_mcs));

		sdata->rc_has_mcs_mask[i] = false;
		sdata->rc_has_vht_mcs_mask[i] = false;
		if (!sband)
			continue;

		for (j = 0; j < IEEE80211_HT_MCS_MASK_LEN; j++) {
			if (sdata->rc_rateidx_mcs_mask[i][j] != 0xff) {
				sdata->rc_has_mcs_mask[i] = true;
				break;
			}
		}

		for (j = 0; j < NL80211_VHT_NSS_MAX; j++) {
			if (sdata->rc_rateidx_vht_mcs_mask[i][j] != 0xffff) {
				sdata->rc_has_vht_mcs_mask[i] = true;
				break;
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static int ieee80211_remain_on_channel_hw(struct ieee80211_local *local,
					  struct net_device *dev,
					  struct ieee80211_channel *chan,
					  enum nl80211_channel_type chantype,
					  unsigned int duration, u64 *cookie)
{
	int ret;
	u32 random_cookie;

	lockdep_assert_held(&local->mtx);

	if (local->hw_roc_cookie)
		return -EBUSY;
	/* must be nonzero */
	random_cookie = random32() | 1;

	*cookie = random_cookie;
	local->hw_roc_dev = dev;
	local->hw_roc_cookie = random_cookie;
	local->hw_roc_channel = chan;
	local->hw_roc_channel_type = chantype;
	local->hw_roc_duration = duration;
	ret = drv_remain_on_channel(local, chan, chantype, duration);
	if (ret) {
		local->hw_roc_channel = NULL;
		local->hw_roc_cookie = 0;
	}

	return ret;
}

static int ieee80211_remain_on_channel(struct wiphy *wiphy,
				       struct net_device *dev,
				       struct ieee80211_channel *chan,
				       enum nl80211_channel_type channel_type,
				       unsigned int duration,
				       u64 *cookie)
=======
static int ieee80211_start_radar_detection(struct wiphy *wiphy,
					   struct net_device *dev,
					   struct cfg80211_chan_def *chandef,
					   u32 cac_time_ms)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_chan_req chanreq = { .oper = *chandef };
	struct ieee80211_local *local = sdata->local;
	int err;

	lockdep_assert_wiphy(local->hw.wiphy);

	if (!list_empty(&local->roc_list) || local->scanning) {
		err = -EBUSY;
		goto out_unlock;
	}

	/* whatever, but channel contexts should not complain about that one */
	sdata->deflink.smps_mode = IEEE80211_SMPS_OFF;
	sdata->deflink.needed_rx_chains = local->rx_chains;

	err = ieee80211_link_use_channel(&sdata->deflink, &chanreq,
					 IEEE80211_CHANCTX_SHARED);
	if (err)
		goto out_unlock;

	wiphy_delayed_work_queue(wiphy, &sdata->deflink.dfs_cac_timer_work,
				 msecs_to_jiffies(cac_time_ms));

 out_unlock:
	return err;
}

static void ieee80211_end_cac(struct wiphy *wiphy,
			      struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;

<<<<<<< HEAD
	if (local->ops->remain_on_channel) {
		int ret;

		mutex_lock(&local->mtx);
		ret = ieee80211_remain_on_channel_hw(local, dev,
						     chan, channel_type,
						     duration, cookie);
		local->hw_roc_for_tx = false;
		mutex_unlock(&local->mtx);

		return ret;
	}

	return ieee80211_wk_remain_on_channel(sdata, chan, channel_type,
					      duration, cookie);
}

static int ieee80211_cancel_remain_on_channel_hw(struct ieee80211_local *local,
						 u64 cookie)
{
	int ret;

	lockdep_assert_held(&local->mtx);

	if (local->hw_roc_cookie != cookie)
		return -ENOENT;

	ret = drv_cancel_remain_on_channel(local);
	if (ret)
		return ret;

	local->hw_roc_cookie = 0;
	local->hw_roc_channel = NULL;

	ieee80211_recalc_idle(local);

	return 0;
}

static int ieee80211_cancel_remain_on_channel(struct wiphy *wiphy,
					      struct net_device *dev,
					      u64 cookie)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;

	if (local->ops->cancel_remain_on_channel) {
		int ret;

		mutex_lock(&local->mtx);
		ret = ieee80211_cancel_remain_on_channel_hw(local, cookie);
		mutex_unlock(&local->mtx);

		return ret;
	}

	return ieee80211_wk_cancel_remain_on_channel(sdata, cookie);
}

static enum work_done_result
ieee80211_offchan_tx_done(struct ieee80211_work *wk, struct sk_buff *skb)
{
	/*
	 * Use the data embedded in the work struct for reporting
	 * here so if the driver mangled the SKB before dropping
	 * it (which is the only way we really should get here)
	 * then we don't report mangled data.
	 *
	 * If there was no wait time, then by the time we get here
	 * the driver will likely not have reported the status yet,
	 * so in that case userspace will have to deal with it.
	 */

	if (wk->offchan_tx.wait && !wk->offchan_tx.status)
		cfg80211_mgmt_tx_status(wk->sdata->dev,
					(unsigned long) wk->offchan_tx.frame,
					wk->data, wk->data_len, false, GFP_KERNEL);

	return WORK_DONE_DESTROY;
}

static int ieee80211_mgmt_tx(struct wiphy *wiphy, struct net_device *dev,
			     struct ieee80211_channel *chan, bool offchan,
			     enum nl80211_channel_type channel_type,
			     bool channel_type_valid, unsigned int wait,
			     const u8 *buf, size_t len, bool no_cck,
			     bool dont_wait_for_ack, u64 *cookie)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct sta_info *sta;
	struct ieee80211_work *wk;
	const struct ieee80211_mgmt *mgmt = (void *)buf;
	u32 flags;
	bool is_offchan = false;

	if (dont_wait_for_ack)
		flags = IEEE80211_TX_CTL_NO_ACK;
	else
		flags = IEEE80211_TX_INTFL_NL80211_FRAME_TX |
			IEEE80211_TX_CTL_REQ_TX_STATUS;

	/* Check that we are on the requested channel for transmission */
	if (chan != local->tmp_channel &&
	    chan != local->oper_channel)
		is_offchan = true;
	if (channel_type_valid &&
	    (channel_type != local->tmp_channel_type &&
	     channel_type != local->_oper_channel_type))
		is_offchan = true;

	if (chan == local->hw_roc_channel) {
		/* TODO: check channel type? */
		is_offchan = false;
		flags |= IEEE80211_TX_CTL_TX_OFFCHAN;
	}

	if (no_cck)
		flags |= IEEE80211_TX_CTL_NO_CCK_RATE;

	if (is_offchan && !offchan)
		return -EBUSY;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_P2P_GO:
	case NL80211_IFTYPE_MESH_POINT:
		if (!ieee80211_is_action(mgmt->frame_control) ||
		    mgmt->u.action.category == WLAN_CATEGORY_PUBLIC)
			break;
		rcu_read_lock();
		sta = sta_info_get(sdata, mgmt->da);
		rcu_read_unlock();
		if (!sta)
			return -ENOLINK;
		break;
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_P2P_CLIENT:
		break;
=======
	lockdep_assert_wiphy(local->hw.wiphy);

	list_for_each_entry(sdata, &local->interfaces, list) {
		/* it might be waiting for the local->mtx, but then
		 * by the time it gets it, sdata->wdev.cac_started
		 * will no longer be true
		 */
		wiphy_delayed_work_cancel(wiphy,
					  &sdata->deflink.dfs_cac_timer_work);

		if (sdata->wdev.cac_started) {
			ieee80211_link_release_channel(&sdata->deflink);
			sdata->wdev.cac_started = false;
		}
	}
}

static struct cfg80211_beacon_data *
cfg80211_beacon_dup(struct cfg80211_beacon_data *beacon)
{
	struct cfg80211_beacon_data *new_beacon;
	u8 *pos;
	int len;

	len = beacon->head_len + beacon->tail_len + beacon->beacon_ies_len +
	      beacon->proberesp_ies_len + beacon->assocresp_ies_len +
	      beacon->probe_resp_len + beacon->lci_len + beacon->civicloc_len;

	if (beacon->mbssid_ies)
		len += ieee80211_get_mbssid_beacon_len(beacon->mbssid_ies,
						       beacon->rnr_ies,
						       beacon->mbssid_ies->cnt);

	new_beacon = kzalloc(sizeof(*new_beacon) + len, GFP_KERNEL);
	if (!new_beacon)
		return NULL;

	if (beacon->mbssid_ies && beacon->mbssid_ies->cnt) {
		new_beacon->mbssid_ies =
			kzalloc(struct_size(new_beacon->mbssid_ies,
					    elem, beacon->mbssid_ies->cnt),
				GFP_KERNEL);
		if (!new_beacon->mbssid_ies) {
			kfree(new_beacon);
			return NULL;
		}

		if (beacon->rnr_ies && beacon->rnr_ies->cnt) {
			new_beacon->rnr_ies =
				kzalloc(struct_size(new_beacon->rnr_ies,
						    elem, beacon->rnr_ies->cnt),
					GFP_KERNEL);
			if (!new_beacon->rnr_ies) {
				kfree(new_beacon->mbssid_ies);
				kfree(new_beacon);
				return NULL;
			}
		}
	}

	pos = (u8 *)(new_beacon + 1);
	if (beacon->head_len) {
		new_beacon->head_len = beacon->head_len;
		new_beacon->head = pos;
		memcpy(pos, beacon->head, beacon->head_len);
		pos += beacon->head_len;
	}
	if (beacon->tail_len) {
		new_beacon->tail_len = beacon->tail_len;
		new_beacon->tail = pos;
		memcpy(pos, beacon->tail, beacon->tail_len);
		pos += beacon->tail_len;
	}
	if (beacon->beacon_ies_len) {
		new_beacon->beacon_ies_len = beacon->beacon_ies_len;
		new_beacon->beacon_ies = pos;
		memcpy(pos, beacon->beacon_ies, beacon->beacon_ies_len);
		pos += beacon->beacon_ies_len;
	}
	if (beacon->proberesp_ies_len) {
		new_beacon->proberesp_ies_len = beacon->proberesp_ies_len;
		new_beacon->proberesp_ies = pos;
		memcpy(pos, beacon->proberesp_ies, beacon->proberesp_ies_len);
		pos += beacon->proberesp_ies_len;
	}
	if (beacon->assocresp_ies_len) {
		new_beacon->assocresp_ies_len = beacon->assocresp_ies_len;
		new_beacon->assocresp_ies = pos;
		memcpy(pos, beacon->assocresp_ies, beacon->assocresp_ies_len);
		pos += beacon->assocresp_ies_len;
	}
	if (beacon->probe_resp_len) {
		new_beacon->probe_resp_len = beacon->probe_resp_len;
		new_beacon->probe_resp = pos;
		memcpy(pos, beacon->probe_resp, beacon->probe_resp_len);
		pos += beacon->probe_resp_len;
	}
	if (beacon->mbssid_ies && beacon->mbssid_ies->cnt) {
		pos += ieee80211_copy_mbssid_beacon(pos,
						    new_beacon->mbssid_ies,
						    beacon->mbssid_ies);
		if (beacon->rnr_ies && beacon->rnr_ies->cnt)
			pos += ieee80211_copy_rnr_beacon(pos,
							 new_beacon->rnr_ies,
							 beacon->rnr_ies);
	}

	/* might copy -1, meaning no changes requested */
	new_beacon->ftm_responder = beacon->ftm_responder;
	if (beacon->lci) {
		new_beacon->lci_len = beacon->lci_len;
		new_beacon->lci = pos;
		memcpy(pos, beacon->lci, beacon->lci_len);
		pos += beacon->lci_len;
	}
	if (beacon->civicloc) {
		new_beacon->civicloc_len = beacon->civicloc_len;
		new_beacon->civicloc = pos;
		memcpy(pos, beacon->civicloc, beacon->civicloc_len);
		pos += beacon->civicloc_len;
	}

	return new_beacon;
}

void ieee80211_csa_finish(struct ieee80211_vif *vif, unsigned int link_id)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_link_data *link_data;

	if (WARN_ON(link_id >= IEEE80211_MLD_MAX_NUM_LINKS))
		return;

	rcu_read_lock();

	link_data = rcu_dereference(sdata->link[link_id]);
	if (WARN_ON(!link_data)) {
		rcu_read_unlock();
		return;
	}

	/* TODO: MBSSID with MLO changes */
	if (vif->mbssid_tx_vif == vif) {
		/* Trigger ieee80211_csa_finish() on the non-transmitting
		 * interfaces when channel switch is received on
		 * transmitting interface
		 */
		struct ieee80211_sub_if_data *iter;

		list_for_each_entry_rcu(iter, &local->interfaces, list) {
			if (!ieee80211_sdata_running(iter))
				continue;

			if (iter == sdata || iter->vif.mbssid_tx_vif != vif)
				continue;

			wiphy_work_queue(iter->local->hw.wiphy,
					 &iter->deflink.csa_finalize_work);
		}
	}
	wiphy_work_queue(local->hw.wiphy, &link_data->csa_finalize_work);

	rcu_read_unlock();
}
EXPORT_SYMBOL(ieee80211_csa_finish);

void ieee80211_channel_switch_disconnect(struct ieee80211_vif *vif, bool block_tx)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;

	sdata->csa_blocked_tx = block_tx;
	sdata_info(sdata, "channel switch failed, disconnecting\n");
	wiphy_work_queue(local->hw.wiphy, &ifmgd->csa_connection_drop_work);
}
EXPORT_SYMBOL(ieee80211_channel_switch_disconnect);

static int ieee80211_set_after_csa_beacon(struct ieee80211_link_data *link_data,
					  u64 *changed)
{
	struct ieee80211_sub_if_data *sdata = link_data->sdata;
	int err;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP:
		if (!link_data->u.ap.next_beacon)
			return -EINVAL;

		err = ieee80211_assign_beacon(sdata, link_data,
					      link_data->u.ap.next_beacon,
					      NULL, NULL, changed);
		ieee80211_free_next_beacon(link_data);

		if (err < 0)
			return err;
		break;
	case NL80211_IFTYPE_ADHOC:
		err = ieee80211_ibss_finish_csa(sdata, changed);
		if (err < 0)
			return err;
		break;
#ifdef CONFIG_MAC80211_MESH
	case NL80211_IFTYPE_MESH_POINT:
		err = ieee80211_mesh_finish_csa(sdata, changed);
		if (err < 0)
			return err;
		break;
#endif
	default:
		WARN_ON(1);
		return -EINVAL;
	}

	return 0;
}

static int __ieee80211_csa_finalize(struct ieee80211_link_data *link_data)
{
	struct ieee80211_sub_if_data *sdata = link_data->sdata;
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_bss_conf *link_conf = link_data->conf;
	u64 changed = 0;
	int err;

	lockdep_assert_wiphy(local->hw.wiphy);

	/*
	 * using reservation isn't immediate as it may be deferred until later
	 * with multi-vif. once reservation is complete it will re-schedule the
	 * work with no reserved_chanctx so verify chandef to check if it
	 * completed successfully
	 */

	if (link_data->reserved_chanctx) {
		/*
		 * with multi-vif csa driver may call ieee80211_csa_finish()
		 * many times while waiting for other interfaces to use their
		 * reservations
		 */
		if (link_data->reserved_ready)
			return 0;

		return ieee80211_link_use_reserved_context(link_data);
	}

	if (!cfg80211_chandef_identical(&link_conf->chanreq.oper,
					&link_data->csa_chanreq.oper))
		return -EINVAL;

	link_conf->csa_active = false;

	err = ieee80211_set_after_csa_beacon(link_data, &changed);
	if (err)
		return err;

	ieee80211_link_info_change_notify(sdata, link_data, changed);

	if (sdata->csa_blocked_tx) {
		ieee80211_wake_vif_queues(local, sdata,
					  IEEE80211_QUEUE_STOP_REASON_CSA);
		sdata->csa_blocked_tx = false;
	}

	err = drv_post_channel_switch(link_data);
	if (err)
		return err;

	cfg80211_ch_switch_notify(sdata->dev, &link_data->csa_chanreq.oper,
				  link_data->link_id);

	return 0;
}

static void ieee80211_csa_finalize(struct ieee80211_link_data *link_data)
{
	struct ieee80211_sub_if_data *sdata = link_data->sdata;

	if (__ieee80211_csa_finalize(link_data)) {
		sdata_info(sdata, "failed to finalize CSA on link %d, disconnecting\n",
			   link_data->link_id);
		cfg80211_stop_iface(sdata->local->hw.wiphy, &sdata->wdev,
				    GFP_KERNEL);
	}
}

void ieee80211_csa_finalize_work(struct wiphy *wiphy, struct wiphy_work *work)
{
	struct ieee80211_link_data *link =
		container_of(work, struct ieee80211_link_data, csa_finalize_work);
	struct ieee80211_sub_if_data *sdata = link->sdata;
	struct ieee80211_local *local = sdata->local;

	lockdep_assert_wiphy(local->hw.wiphy);

	/* AP might have been stopped while waiting for the lock. */
	if (!link->conf->csa_active)
		return;

	if (!ieee80211_sdata_running(sdata))
		return;

	ieee80211_csa_finalize(link);
}

static int ieee80211_set_csa_beacon(struct ieee80211_link_data *link_data,
				    struct cfg80211_csa_settings *params,
				    u64 *changed)
{
	struct ieee80211_sub_if_data *sdata = link_data->sdata;
	struct ieee80211_csa_settings csa = {};
	int err;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP:
		link_data->u.ap.next_beacon =
			cfg80211_beacon_dup(&params->beacon_after);
		if (!link_data->u.ap.next_beacon)
			return -ENOMEM;

		/*
		 * With a count of 0, we don't have to wait for any
		 * TBTT before switching, so complete the CSA
		 * immediately.  In theory, with a count == 1 we
		 * should delay the switch until just before the next
		 * TBTT, but that would complicate things so we switch
		 * immediately too.  If we would delay the switch
		 * until the next TBTT, we would have to set the probe
		 * response here.
		 *
		 * TODO: A channel switch with count <= 1 without
		 * sending a CSA action frame is kind of useless,
		 * because the clients won't know we're changing
		 * channels.  The action frame must be implemented
		 * either here or in the userspace.
		 */
		if (params->count <= 1)
			break;

		if ((params->n_counter_offsets_beacon >
		     IEEE80211_MAX_CNTDWN_COUNTERS_NUM) ||
		    (params->n_counter_offsets_presp >
		     IEEE80211_MAX_CNTDWN_COUNTERS_NUM)) {
			ieee80211_free_next_beacon(link_data);
			return -EINVAL;
		}

		csa.counter_offsets_beacon = params->counter_offsets_beacon;
		csa.counter_offsets_presp = params->counter_offsets_presp;
		csa.n_counter_offsets_beacon = params->n_counter_offsets_beacon;
		csa.n_counter_offsets_presp = params->n_counter_offsets_presp;
		csa.count = params->count;

		err = ieee80211_assign_beacon(sdata, link_data,
					      &params->beacon_csa, &csa,
					      NULL, changed);
		if (err < 0) {
			ieee80211_free_next_beacon(link_data);
			return err;
		}

		break;
	case NL80211_IFTYPE_ADHOC:
		if (!sdata->vif.cfg.ibss_joined)
			return -EINVAL;

		if (params->chandef.width != sdata->u.ibss.chandef.width)
			return -EINVAL;

		switch (params->chandef.width) {
		case NL80211_CHAN_WIDTH_40:
			if (cfg80211_get_chandef_type(&params->chandef) !=
			    cfg80211_get_chandef_type(&sdata->u.ibss.chandef))
				return -EINVAL;
			break;
		case NL80211_CHAN_WIDTH_5:
		case NL80211_CHAN_WIDTH_10:
		case NL80211_CHAN_WIDTH_20_NOHT:
		case NL80211_CHAN_WIDTH_20:
			break;
		default:
			return -EINVAL;
		}

		/* changes into another band are not supported */
		if (sdata->u.ibss.chandef.chan->band !=
		    params->chandef.chan->band)
			return -EINVAL;

		/* see comments in the NL80211_IFTYPE_AP block */
		if (params->count > 1) {
			err = ieee80211_ibss_csa_beacon(sdata, params, changed);
			if (err < 0)
				return err;
		}

		ieee80211_send_action_csa(sdata, params);

		break;
#ifdef CONFIG_MAC80211_MESH
	case NL80211_IFTYPE_MESH_POINT: {
		struct ieee80211_if_mesh *ifmsh = &sdata->u.mesh;

		/* changes into another band are not supported */
		if (sdata->vif.bss_conf.chanreq.oper.chan->band !=
		    params->chandef.chan->band)
			return -EINVAL;

		if (ifmsh->csa_role == IEEE80211_MESH_CSA_ROLE_NONE) {
			ifmsh->csa_role = IEEE80211_MESH_CSA_ROLE_INIT;
			if (!ifmsh->pre_value)
				ifmsh->pre_value = 1;
			else
				ifmsh->pre_value++;
		}

		/* see comments in the NL80211_IFTYPE_AP block */
		if (params->count > 1) {
			err = ieee80211_mesh_csa_beacon(sdata, params, changed);
			if (err < 0) {
				ifmsh->csa_role = IEEE80211_MESH_CSA_ROLE_NONE;
				return err;
			}
		}

		if (ifmsh->csa_role == IEEE80211_MESH_CSA_ROLE_INIT)
			ieee80211_send_action_csa(sdata, params);

		break;
		}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -EOPNOTSUPP;
	}

<<<<<<< HEAD
	skb = dev_alloc_skb(local->hw.extra_tx_headroom + len);
	if (!skb)
		return -ENOMEM;
	skb_reserve(skb, local->hw.extra_tx_headroom);

	memcpy(skb_put(skb, len), buf, len);

	IEEE80211_SKB_CB(skb)->flags = flags;

	skb->dev = sdata->dev;

	*cookie = (unsigned long) skb;

	if (is_offchan && local->ops->remain_on_channel) {
		unsigned int duration;
		int ret;

		mutex_lock(&local->mtx);
		/*
		 * If the duration is zero, then the driver
		 * wouldn't actually do anything. Set it to
		 * 100 for now.
		 *
		 * TODO: cancel the off-channel operation
		 *       when we get the SKB's TX status and
		 *       the wait time was zero before.
		 */
		duration = 100;
		if (wait)
			duration = wait;
		ret = ieee80211_remain_on_channel_hw(local, dev, chan,
						     channel_type,
						     duration, cookie);
		if (ret) {
			kfree_skb(skb);
			mutex_unlock(&local->mtx);
			return ret;
		}

		local->hw_roc_for_tx = true;
		local->hw_roc_duration = wait;

		/*
		 * queue up frame for transmission after
		 * ieee80211_ready_on_channel call
		 */

		/* modify cookie to prevent API mismatches */
		*cookie ^= 2;
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_TX_OFFCHAN;
		local->hw_roc_skb = skb;
		local->hw_roc_skb_for_status = skb;
		mutex_unlock(&local->mtx);

		return 0;
	}

	/*
	 * Can transmit right away if the channel was the
	 * right one and there's no wait involved... If a
	 * wait is involved, we might otherwise not be on
	 * the right channel for long enough!
	 */
	if (!is_offchan && !wait && !sdata->vif.bss_conf.idle) {
		ieee80211_tx_skb(sdata, skb);
		return 0;
	}

	wk = kzalloc(sizeof(*wk) + len, GFP_KERNEL);
	if (!wk) {
		kfree_skb(skb);
		return -ENOMEM;
	}

	wk->type = IEEE80211_WORK_OFFCHANNEL_TX;
	wk->chan = chan;
	wk->chan_type = channel_type;
	wk->sdata = sdata;
	wk->done = ieee80211_offchan_tx_done;
	wk->offchan_tx.frame = skb;
	wk->offchan_tx.wait = wait;
	wk->data_len = len;
	memcpy(wk->data, buf, len);

	ieee80211_add_work(wk);
	return 0;
}

static int ieee80211_mgmt_tx_cancel_wait(struct wiphy *wiphy,
					 struct net_device *dev,
					 u64 cookie)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_work *wk;
	int ret = -ENOENT;

	mutex_lock(&local->mtx);

	if (local->ops->cancel_remain_on_channel) {
		cookie ^= 2;
		ret = ieee80211_cancel_remain_on_channel_hw(local, cookie);

		if (ret == 0) {
			kfree_skb(local->hw_roc_skb);
			local->hw_roc_skb = NULL;
			local->hw_roc_skb_for_status = NULL;
		}

		mutex_unlock(&local->mtx);

		return ret;
	}

	list_for_each_entry(wk, &local->work_list, list) {
		if (wk->sdata != sdata)
			continue;

		if (wk->type != IEEE80211_WORK_OFFCHANNEL_TX)
			continue;

		if (cookie != (unsigned long) wk->offchan_tx.frame)
			continue;

		wk->timeout = jiffies;

		ieee80211_queue_work(&local->hw, &local->work_work);
		ret = 0;
		break;
	}
	mutex_unlock(&local->mtx);

	return ret;
}

static void ieee80211_mgmt_frame_register(struct wiphy *wiphy,
					  struct net_device *dev,
					  u16 frame_type, bool reg)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	if (frame_type != (IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_PROBE_REQ))
		return;

	if (reg)
		local->probe_req_reg++;
	else
		local->probe_req_reg--;

	ieee80211_queue_work(&local->hw, &local->reconfig_filter);
=======
	return 0;
}

static void ieee80211_color_change_abort(struct ieee80211_sub_if_data  *sdata)
{
	sdata->vif.bss_conf.color_change_active = false;

	ieee80211_free_next_beacon(&sdata->deflink);

	cfg80211_color_change_aborted_notify(sdata->dev);
}

static int
__ieee80211_channel_switch(struct wiphy *wiphy, struct net_device *dev,
			   struct cfg80211_csa_settings *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_chan_req chanreq = { .oper = params->chandef };
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_switch ch_switch = {
		.link_id = params->link_id,
	};
	struct ieee80211_chanctx_conf *conf;
	struct ieee80211_chanctx *chanctx;
	struct ieee80211_bss_conf *link_conf;
	struct ieee80211_link_data *link_data;
	u64 changed = 0;
	u8 link_id = params->link_id;
	int err;

	lockdep_assert_wiphy(local->hw.wiphy);

	if (!list_empty(&local->roc_list) || local->scanning)
		return -EBUSY;

	if (sdata->wdev.cac_started)
		return -EBUSY;

	if (WARN_ON(link_id >= IEEE80211_MLD_MAX_NUM_LINKS))
		return -EINVAL;

	link_data = wiphy_dereference(wiphy, sdata->link[link_id]);
	if (!link_data)
		return -ENOLINK;

	link_conf = link_data->conf;

	if (chanreq.oper.punctured && !link_conf->eht_support)
		return -EINVAL;

	/* don't allow another channel switch if one is already active. */
	if (link_conf->csa_active)
		return -EBUSY;

	conf = wiphy_dereference(wiphy, link_conf->chanctx_conf);
	if (!conf) {
		err = -EBUSY;
		goto out;
	}

	if (params->chandef.chan->freq_offset) {
		/* this may work, but is untested */
		err = -EOPNOTSUPP;
		goto out;
	}

	chanctx = container_of(conf, struct ieee80211_chanctx, conf);

	ch_switch.timestamp = 0;
	ch_switch.device_timestamp = 0;
	ch_switch.block_tx = params->block_tx;
	ch_switch.chandef = chanreq.oper;
	ch_switch.count = params->count;

	err = drv_pre_channel_switch(sdata, &ch_switch);
	if (err)
		goto out;

	err = ieee80211_link_reserve_chanctx(link_data, &chanreq,
					     chanctx->mode,
					     params->radar_required);
	if (err)
		goto out;

	/* if reservation is invalid then this will fail */
	err = ieee80211_check_combinations(sdata, NULL, chanctx->mode, 0);
	if (err) {
		ieee80211_link_unreserve_chanctx(link_data);
		goto out;
	}

	/* if there is a color change in progress, abort it */
	if (link_conf->color_change_active)
		ieee80211_color_change_abort(sdata);

	err = ieee80211_set_csa_beacon(link_data, params, &changed);
	if (err) {
		ieee80211_link_unreserve_chanctx(link_data);
		goto out;
	}

	link_data->csa_chanreq = chanreq; 
	link_conf->csa_active = true;

	if (params->block_tx &&
	    !ieee80211_hw_check(&local->hw, HANDLES_QUIET_CSA)) {
		ieee80211_stop_vif_queues(local, sdata,
					  IEEE80211_QUEUE_STOP_REASON_CSA);
		sdata->csa_blocked_tx = true;
	}

	cfg80211_ch_switch_started_notify(sdata->dev,
					  &link_data->csa_chanreq.oper, 0,
					  params->count, params->block_tx);

	if (changed) {
		ieee80211_link_info_change_notify(sdata, link_data, changed);
		drv_channel_switch_beacon(sdata, &link_data->csa_chanreq.oper);
	} else {
		/* if the beacon didn't change, we can finalize immediately */
		ieee80211_csa_finalize(link_data);
	}

out:
	return err;
}

int ieee80211_channel_switch(struct wiphy *wiphy, struct net_device *dev,
			     struct cfg80211_csa_settings *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;

	lockdep_assert_wiphy(local->hw.wiphy);

	return __ieee80211_channel_switch(wiphy, dev, params);
}

u64 ieee80211_mgmt_tx_cookie(struct ieee80211_local *local)
{
	lockdep_assert_wiphy(local->hw.wiphy);

	local->roc_cookie_counter++;

	/* wow, you wrapped 64 bits ... more likely a bug */
	if (WARN_ON(local->roc_cookie_counter == 0))
		local->roc_cookie_counter++;

	return local->roc_cookie_counter;
}

int ieee80211_attach_ack_skb(struct ieee80211_local *local, struct sk_buff *skb,
			     u64 *cookie, gfp_t gfp)
{
	unsigned long spin_flags;
	struct sk_buff *ack_skb;
	int id;

	ack_skb = skb_copy(skb, gfp);
	if (!ack_skb)
		return -ENOMEM;

	spin_lock_irqsave(&local->ack_status_lock, spin_flags);
	id = idr_alloc(&local->ack_status_frames, ack_skb,
		       1, 0x2000, GFP_ATOMIC);
	spin_unlock_irqrestore(&local->ack_status_lock, spin_flags);

	if (id < 0) {
		kfree_skb(ack_skb);
		return -ENOMEM;
	}

	IEEE80211_SKB_CB(skb)->status_data_idr = 1;
	IEEE80211_SKB_CB(skb)->status_data = id;

	*cookie = ieee80211_mgmt_tx_cookie(local);
	IEEE80211_SKB_CB(ack_skb)->ack.cookie = *cookie;

	return 0;
}

static void
ieee80211_update_mgmt_frame_registrations(struct wiphy *wiphy,
					  struct wireless_dev *wdev,
					  struct mgmt_frame_regs *upd)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);
	u32 preq_mask = BIT(IEEE80211_STYPE_PROBE_REQ >> 4);
	u32 action_mask = BIT(IEEE80211_STYPE_ACTION >> 4);
	bool global_change, intf_change;

	global_change =
		(local->probe_req_reg != !!(upd->global_stypes & preq_mask)) ||
		(local->rx_mcast_action_reg !=
		 !!(upd->global_mcast_stypes & action_mask));
	local->probe_req_reg = upd->global_stypes & preq_mask;
	local->rx_mcast_action_reg = upd->global_mcast_stypes & action_mask;

	intf_change = (sdata->vif.probe_req_reg !=
		       !!(upd->interface_stypes & preq_mask)) ||
		(sdata->vif.rx_mcast_action_reg !=
		 !!(upd->interface_mcast_stypes & action_mask));
	sdata->vif.probe_req_reg = upd->interface_stypes & preq_mask;
	sdata->vif.rx_mcast_action_reg =
		upd->interface_mcast_stypes & action_mask;

	if (!local->open_count)
		return;

	if (intf_change && ieee80211_sdata_running(sdata))
		drv_config_iface_filter(local, sdata,
					sdata->vif.probe_req_reg ?
						FIF_PROBE_REQ : 0,
					FIF_PROBE_REQ);

	if (global_change)
		ieee80211_configure_filter(local);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ieee80211_set_antenna(struct wiphy *wiphy, u32 tx_ant, u32 rx_ant)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
<<<<<<< HEAD
=======
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (local->started)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return drv_set_antenna(local, tx_ant, rx_ant);
=======
	ret = drv_set_antenna(local, tx_ant, rx_ant);
	if (ret)
		return ret;

	local->rx_chains = hweight8(rx_ant);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ieee80211_get_antenna(struct wiphy *wiphy, u32 *tx_ant, u32 *rx_ant)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	return drv_get_antenna(local, tx_ant, rx_ant);
}

<<<<<<< HEAD
static int ieee80211_set_ringparam(struct wiphy *wiphy, u32 tx, u32 rx)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	return drv_set_ringparam(local, tx, rx);
}

static void ieee80211_get_ringparam(struct wiphy *wiphy,
				    u32 *tx, u32 *tx_max, u32 *rx, u32 *rx_max)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	drv_get_ringparam(local, tx, tx_max, rx, rx_max);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ieee80211_set_rekey_data(struct wiphy *wiphy,
				    struct net_device *dev,
				    struct cfg80211_gtk_rekey_data *data)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (!local->ops->set_rekey_data)
		return -EOPNOTSUPP;

	drv_set_rekey_data(local, sdata, data);

	return 0;
}

<<<<<<< HEAD
static void ieee80211_tdls_add_ext_capab(struct sk_buff *skb)
{
	u8 *pos = (void *)skb_put(skb, 7);

	*pos++ = WLAN_EID_EXT_CAPABILITY;
	*pos++ = 5; /* len */
	*pos++ = 0x0;
	*pos++ = 0x0;
	*pos++ = 0x0;
	*pos++ = 0x0;
	*pos++ = WLAN_EXT_CAPA5_TDLS_ENABLED;
}

static u16 ieee80211_get_tdls_sta_capab(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	u16 capab;

	capab = 0;
	if (local->oper_channel->band != IEEE80211_BAND_2GHZ)
		return capab;

	if (!(local->hw.flags & IEEE80211_HW_2GHZ_SHORT_SLOT_INCAPABLE))
		capab |= WLAN_CAPABILITY_SHORT_SLOT_TIME;
	if (!(local->hw.flags & IEEE80211_HW_2GHZ_SHORT_PREAMBLE_INCAPABLE))
		capab |= WLAN_CAPABILITY_SHORT_PREAMBLE;

	return capab;
}

static void ieee80211_tdls_add_link_ie(struct sk_buff *skb, u8 *src_addr,
				       u8 *peer, u8 *bssid)
{
	struct ieee80211_tdls_lnkie *lnkid;

	lnkid = (void *)skb_put(skb, sizeof(struct ieee80211_tdls_lnkie));

	lnkid->ie_type = WLAN_EID_LINK_ID;
	lnkid->ie_len = sizeof(struct ieee80211_tdls_lnkie) - 2;

	memcpy(lnkid->bssid, bssid, ETH_ALEN);
	memcpy(lnkid->init_sta, src_addr, ETH_ALEN);
	memcpy(lnkid->resp_sta, peer, ETH_ALEN);
}

static int
ieee80211_prep_tdls_encap_data(struct wiphy *wiphy, struct net_device *dev,
			       u8 *peer, u8 action_code, u8 dialog_token,
			       u16 status_code, struct sk_buff *skb)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_tdls_data *tf;

	tf = (void *)skb_put(skb, offsetof(struct ieee80211_tdls_data, u));

	memcpy(tf->da, peer, ETH_ALEN);
	memcpy(tf->sa, sdata->vif.addr, ETH_ALEN);
	tf->ether_type = cpu_to_be16(ETH_P_TDLS);
	tf->payload_type = WLAN_TDLS_SNAP_RFTYPE;

	switch (action_code) {
	case WLAN_TDLS_SETUP_REQUEST:
		tf->category = WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_SETUP_REQUEST;

		skb_put(skb, sizeof(tf->u.setup_req));
		tf->u.setup_req.dialog_token = dialog_token;
		tf->u.setup_req.capability =
			cpu_to_le16(ieee80211_get_tdls_sta_capab(sdata));

		ieee80211_add_srates_ie(&sdata->vif, skb);
		ieee80211_add_ext_srates_ie(&sdata->vif, skb);
		ieee80211_tdls_add_ext_capab(skb);
		break;
	case WLAN_TDLS_SETUP_RESPONSE:
		tf->category = WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_SETUP_RESPONSE;

		skb_put(skb, sizeof(tf->u.setup_resp));
		tf->u.setup_resp.status_code = cpu_to_le16(status_code);
		tf->u.setup_resp.dialog_token = dialog_token;
		tf->u.setup_resp.capability =
			cpu_to_le16(ieee80211_get_tdls_sta_capab(sdata));

		ieee80211_add_srates_ie(&sdata->vif, skb);
		ieee80211_add_ext_srates_ie(&sdata->vif, skb);
		ieee80211_tdls_add_ext_capab(skb);
		break;
	case WLAN_TDLS_SETUP_CONFIRM:
		tf->category = WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_SETUP_CONFIRM;

		skb_put(skb, sizeof(tf->u.setup_cfm));
		tf->u.setup_cfm.status_code = cpu_to_le16(status_code);
		tf->u.setup_cfm.dialog_token = dialog_token;
		break;
	case WLAN_TDLS_TEARDOWN:
		tf->category = WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_TEARDOWN;

		skb_put(skb, sizeof(tf->u.teardown));
		tf->u.teardown.reason_code = cpu_to_le16(status_code);
		break;
	case WLAN_TDLS_DISCOVERY_REQUEST:
		tf->category = WLAN_CATEGORY_TDLS;
		tf->action_code = WLAN_TDLS_DISCOVERY_REQUEST;

		skb_put(skb, sizeof(tf->u.discover_req));
		tf->u.discover_req.dialog_token = dialog_token;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int
ieee80211_prep_tdls_direct(struct wiphy *wiphy, struct net_device *dev,
			   u8 *peer, u8 action_code, u8 dialog_token,
			   u16 status_code, struct sk_buff *skb)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_mgmt *mgmt;

	mgmt = (void *)skb_put(skb, 24);
	memset(mgmt, 0, 24);
	memcpy(mgmt->da, peer, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, sdata->u.mgd.bssid, ETH_ALEN);

	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  IEEE80211_STYPE_ACTION);

	switch (action_code) {
	case WLAN_PUB_ACTION_TDLS_DISCOVER_RES:
		skb_put(skb, 1 + sizeof(mgmt->u.action.u.tdls_discover_resp));
		mgmt->u.action.category = WLAN_CATEGORY_PUBLIC;
		mgmt->u.action.u.tdls_discover_resp.action_code =
			WLAN_PUB_ACTION_TDLS_DISCOVER_RES;
		mgmt->u.action.u.tdls_discover_resp.dialog_token =
			dialog_token;
		mgmt->u.action.u.tdls_discover_resp.capability =
			cpu_to_le16(ieee80211_get_tdls_sta_capab(sdata));

		ieee80211_add_srates_ie(&sdata->vif, skb);
		ieee80211_add_ext_srates_ie(&sdata->vif, skb);
		ieee80211_tdls_add_ext_capab(skb);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int ieee80211_tdls_mgmt(struct wiphy *wiphy, struct net_device *dev,
			       u8 *peer, u8 action_code, u8 dialog_token,
			       u16 status_code, u32 peer_capability,
			       const u8 *extra_ies, size_t extra_ies_len)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_tx_info *info;
	struct sk_buff *skb = NULL;
	bool send_direct;
	int ret;

	if (!(wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS))
		return -ENOTSUPP;

	/* make sure we are in managed mode, and associated */
	if (sdata->vif.type != NL80211_IFTYPE_STATION ||
	    !sdata->u.mgd.associated)
		return -EINVAL;

#ifdef CONFIG_MAC80211_VERBOSE_TDLS_DEBUG
	printk(KERN_DEBUG "TDLS mgmt action %d peer %pM\n", action_code, peer);
#endif

	skb = dev_alloc_skb(local->hw.extra_tx_headroom +
			    max(sizeof(struct ieee80211_mgmt),
				sizeof(struct ieee80211_tdls_data)) +
			    50 + /* supported rates */
			    7 + /* ext capab */
			    extra_ies_len +
			    sizeof(struct ieee80211_tdls_lnkie));
	if (!skb)
		return -ENOMEM;

	info = IEEE80211_SKB_CB(skb);
	skb_reserve(skb, local->hw.extra_tx_headroom);

	switch (action_code) {
	case WLAN_TDLS_SETUP_REQUEST:
	case WLAN_TDLS_SETUP_RESPONSE:
	case WLAN_TDLS_SETUP_CONFIRM:
	case WLAN_TDLS_TEARDOWN:
	case WLAN_TDLS_DISCOVERY_REQUEST:
		ret = ieee80211_prep_tdls_encap_data(wiphy, dev, peer,
						     action_code, dialog_token,
						     status_code, skb);
		send_direct = false;
		break;
	case WLAN_PUB_ACTION_TDLS_DISCOVER_RES:
		ret = ieee80211_prep_tdls_direct(wiphy, dev, peer, action_code,
						 dialog_token, status_code,
						 skb);
		send_direct = true;
		break;
	default:
		ret = -ENOTSUPP;
		break;
	}

	if (ret < 0)
		goto fail;

	if (extra_ies_len)
		memcpy(skb_put(skb, extra_ies_len), extra_ies, extra_ies_len);

	/* the TDLS link IE is always added last */
	switch (action_code) {
	case WLAN_TDLS_SETUP_REQUEST:
	case WLAN_TDLS_SETUP_CONFIRM:
	case WLAN_TDLS_TEARDOWN:
	case WLAN_TDLS_DISCOVERY_REQUEST:
		/* we are the initiator */
		ieee80211_tdls_add_link_ie(skb, sdata->vif.addr, peer,
					   sdata->u.mgd.bssid);
		break;
	case WLAN_TDLS_SETUP_RESPONSE:
	case WLAN_PUB_ACTION_TDLS_DISCOVER_RES:
		/* we are the responder */
		ieee80211_tdls_add_link_ie(skb, peer, sdata->vif.addr,
					   sdata->u.mgd.bssid);
		break;
	default:
		ret = -ENOTSUPP;
		goto fail;
	}

	if (send_direct) {
		ieee80211_tx_skb(sdata, skb);
		return 0;
	}

	/*
	 * According to 802.11z: Setup req/resp are sent in AC_BK, otherwise
	 * we should default to AC_VI.
	 */
	switch (action_code) {
	case WLAN_TDLS_SETUP_REQUEST:
	case WLAN_TDLS_SETUP_RESPONSE:
		skb_set_queue_mapping(skb, IEEE80211_AC_BK);
		skb->priority = 2;
		break;
	default:
		skb_set_queue_mapping(skb, IEEE80211_AC_VI);
		skb->priority = 5;
		break;
	}

	/* disable bottom halves when entering the Tx path */
	local_bh_disable();
	ret = ieee80211_subif_start_xmit(skb, dev);
	local_bh_enable();

	return ret;

fail:
	dev_kfree_skb(skb);
	return ret;
}

static int ieee80211_tdls_oper(struct wiphy *wiphy, struct net_device *dev,
			       u8 *peer, enum nl80211_tdls_operation oper)
{
	struct sta_info *sta;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	if (!(wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS))
		return -ENOTSUPP;

	if (sdata->vif.type != NL80211_IFTYPE_STATION)
		return -EINVAL;

#ifdef CONFIG_MAC80211_VERBOSE_TDLS_DEBUG
	printk(KERN_DEBUG "TDLS oper %d peer %pM\n", oper, peer);
#endif

	switch (oper) {
	case NL80211_TDLS_ENABLE_LINK:
		rcu_read_lock();
		sta = sta_info_get(sdata, peer);
		if (!sta) {
			rcu_read_unlock();
			return -ENOLINK;
		}

		set_sta_flag(sta, WLAN_STA_TDLS_PEER_AUTH);
		rcu_read_unlock();
		break;
	case NL80211_TDLS_DISABLE_LINK:
		return sta_info_destroy_addr(sdata, peer);
	case NL80211_TDLS_TEARDOWN:
	case NL80211_TDLS_SETUP:
	case NL80211_TDLS_DISCOVERY_REQ:
		/* We don't support in-driver setup/teardown/discovery */
		return -ENOTSUPP;
	default:
		return -ENOTSUPP;
	}

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ieee80211_probe_client(struct wiphy *wiphy, struct net_device *dev,
				  const u8 *peer, u64 *cookie)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_qos_hdr *nullfunc;
	struct sk_buff *skb;
	int size = sizeof(*nullfunc);
	__le16 fc;
	bool qos;
	struct ieee80211_tx_info *info;
	struct sta_info *sta;
<<<<<<< HEAD

	rcu_read_lock();
	sta = sta_info_get(sdata, peer);
	if (sta) {
		qos = test_sta_flag(sta, WLAN_STA_WME);
		rcu_read_unlock();
	} else {
		rcu_read_unlock();
		return -ENOLINK;
	}

=======
	struct ieee80211_chanctx_conf *chanctx_conf;
	enum nl80211_band band;
	int ret;

	/* the lock is needed to assign the cookie later */
	lockdep_assert_wiphy(local->hw.wiphy);

	rcu_read_lock();
	sta = sta_info_get_bss(sdata, peer);
	if (!sta) {
		ret = -ENOLINK;
		goto unlock;
	}

	qos = sta->sta.wme;

	chanctx_conf = rcu_dereference(sdata->vif.bss_conf.chanctx_conf);
	if (WARN_ON(!chanctx_conf)) {
		ret = -EINVAL;
		goto unlock;
	}
	band = chanctx_conf->def.chan->band;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (qos) {
		fc = cpu_to_le16(IEEE80211_FTYPE_DATA |
				 IEEE80211_STYPE_QOS_NULLFUNC |
				 IEEE80211_FCTL_FROMDS);
	} else {
		size -= 2;
		fc = cpu_to_le16(IEEE80211_FTYPE_DATA |
				 IEEE80211_STYPE_NULLFUNC |
				 IEEE80211_FCTL_FROMDS);
	}

	skb = dev_alloc_skb(local->hw.extra_tx_headroom + size);
<<<<<<< HEAD
	if (!skb)
		return -ENOMEM;
=======
	if (!skb) {
		ret = -ENOMEM;
		goto unlock;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	skb->dev = dev;

	skb_reserve(skb, local->hw.extra_tx_headroom);

<<<<<<< HEAD
	nullfunc = (void *) skb_put(skb, size);
=======
	nullfunc = skb_put(skb, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nullfunc->frame_control = fc;
	nullfunc->duration_id = 0;
	memcpy(nullfunc->addr1, sta->sta.addr, ETH_ALEN);
	memcpy(nullfunc->addr2, sdata->vif.addr, ETH_ALEN);
	memcpy(nullfunc->addr3, sdata->vif.addr, ETH_ALEN);
	nullfunc->seq_ctrl = 0;

	info = IEEE80211_SKB_CB(skb);

	info->flags |= IEEE80211_TX_CTL_REQ_TX_STATUS |
		       IEEE80211_TX_INTFL_NL80211_FRAME_TX;
<<<<<<< HEAD
=======
	info->band = band;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	skb_set_queue_mapping(skb, IEEE80211_AC_VO);
	skb->priority = 7;
	if (qos)
		nullfunc->qos_ctrl = cpu_to_le16(7);

<<<<<<< HEAD
	local_bh_disable();
	ieee80211_xmit(sdata, skb);
	local_bh_enable();

	*cookie = (unsigned long) skb;
	return 0;
}

static struct ieee80211_channel *
ieee80211_wiphy_get_channel(struct wiphy *wiphy)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	return local->oper_channel;
}

struct cfg80211_ops mac80211_config_ops = {
	.add_virtual_intf = ieee80211_add_iface,
	.del_virtual_intf = ieee80211_del_iface,
	.change_virtual_intf = ieee80211_change_iface,
=======
	ret = ieee80211_attach_ack_skb(local, skb, cookie, GFP_ATOMIC);
	if (ret) {
		kfree_skb(skb);
		goto unlock;
	}

	local_bh_disable();
	ieee80211_xmit(sdata, sta, skb);
	local_bh_enable();

	ret = 0;
unlock:
	rcu_read_unlock();

	return ret;
}

static int ieee80211_cfg_get_channel(struct wiphy *wiphy,
				     struct wireless_dev *wdev,
				     unsigned int link_id,
				     struct cfg80211_chan_def *chandef)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_chanctx_conf *chanctx_conf;
	struct ieee80211_link_data *link;
	int ret = -ENODATA;

	rcu_read_lock();
	link = rcu_dereference(sdata->link[link_id]);
	if (!link) {
		ret = -ENOLINK;
		goto out;
	}

	chanctx_conf = rcu_dereference(link->conf->chanctx_conf);
	if (chanctx_conf) {
		*chandef = link->conf->chanreq.oper;
		ret = 0;
	} else if (local->open_count > 0 &&
		   local->open_count == local->monitors &&
		   sdata->vif.type == NL80211_IFTYPE_MONITOR) {
		*chandef = local->monitor_chanreq.oper;
		ret = 0;
	}
out:
	rcu_read_unlock();

	return ret;
}

#ifdef CONFIG_PM
static void ieee80211_set_wakeup(struct wiphy *wiphy, bool enabled)
{
	drv_set_wakeup(wiphy_priv(wiphy), enabled);
}
#endif

static int ieee80211_set_qos_map(struct wiphy *wiphy,
				 struct net_device *dev,
				 struct cfg80211_qos_map *qos_map)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct mac80211_qos_map *new_qos_map, *old_qos_map;

	if (qos_map) {
		new_qos_map = kzalloc(sizeof(*new_qos_map), GFP_KERNEL);
		if (!new_qos_map)
			return -ENOMEM;
		memcpy(&new_qos_map->qos_map, qos_map, sizeof(*qos_map));
	} else {
		/* A NULL qos_map was passed to disable QoS mapping */
		new_qos_map = NULL;
	}

	old_qos_map = sdata_dereference(sdata->qos_map, sdata);
	rcu_assign_pointer(sdata->qos_map, new_qos_map);
	if (old_qos_map)
		kfree_rcu(old_qos_map, rcu_head);

	return 0;
}

static int ieee80211_set_ap_chanwidth(struct wiphy *wiphy,
				      struct net_device *dev,
				      unsigned int link_id,
				      struct cfg80211_chan_def *chandef)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_link_data *link;
	struct ieee80211_chan_req chanreq = { .oper = *chandef };
	int ret;
	u64 changed = 0;

	link = sdata_dereference(sdata->link[link_id], sdata);

	ret = ieee80211_link_change_chanreq(link, &chanreq, &changed);
	if (ret == 0)
		ieee80211_link_info_change_notify(sdata, link, changed);

	return ret;
}

static int ieee80211_add_tx_ts(struct wiphy *wiphy, struct net_device *dev,
			       u8 tsid, const u8 *peer, u8 up,
			       u16 admitted_time)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	int ac = ieee802_1d_to_ac[up];

	if (sdata->vif.type != NL80211_IFTYPE_STATION)
		return -EOPNOTSUPP;

	if (!(sdata->wmm_acm & BIT(up)))
		return -EINVAL;

	if (ifmgd->tx_tspec[ac].admitted_time)
		return -EBUSY;

	if (admitted_time) {
		ifmgd->tx_tspec[ac].admitted_time = 32 * admitted_time;
		ifmgd->tx_tspec[ac].tsid = tsid;
		ifmgd->tx_tspec[ac].up = up;
	}

	return 0;
}

static int ieee80211_del_tx_ts(struct wiphy *wiphy, struct net_device *dev,
			       u8 tsid, const u8 *peer)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = wiphy_priv(wiphy);
	int ac;

	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		struct ieee80211_sta_tx_tspec *tx_tspec = &ifmgd->tx_tspec[ac];

		/* skip unused entries */
		if (!tx_tspec->admitted_time)
			continue;

		if (tx_tspec->tsid != tsid)
			continue;

		/* due to this new packets will be reassigned to non-ACM ACs */
		tx_tspec->up = -1;

		/* Make sure that all packets have been sent to avoid to
		 * restore the QoS params on packets that are still on the
		 * queues.
		 */
		synchronize_net();
		ieee80211_flush_queues(local, sdata, false);

		/* restore the normal QoS parameters
		 * (unconditionally to avoid races)
		 */
		tx_tspec->action = TX_TSPEC_ACTION_STOP_DOWNGRADE;
		tx_tspec->downgraded = false;
		ieee80211_sta_handle_tspec_ac_params(sdata);

		/* finally clear all the data */
		memset(tx_tspec, 0, sizeof(*tx_tspec));

		return 0;
	}

	return -ENOENT;
}

void ieee80211_nan_func_terminated(struct ieee80211_vif *vif,
				   u8 inst_id,
				   enum nl80211_nan_func_term_reason reason,
				   gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct cfg80211_nan_func *func;
	u64 cookie;

	if (WARN_ON(vif->type != NL80211_IFTYPE_NAN))
		return;

	spin_lock_bh(&sdata->u.nan.func_lock);

	func = idr_find(&sdata->u.nan.function_inst_ids, inst_id);
	if (WARN_ON(!func)) {
		spin_unlock_bh(&sdata->u.nan.func_lock);
		return;
	}

	cookie = func->cookie;
	idr_remove(&sdata->u.nan.function_inst_ids, inst_id);

	spin_unlock_bh(&sdata->u.nan.func_lock);

	cfg80211_free_nan_func(func);

	cfg80211_nan_func_terminated(ieee80211_vif_to_wdev(vif), inst_id,
				     reason, cookie, gfp);
}
EXPORT_SYMBOL(ieee80211_nan_func_terminated);

void ieee80211_nan_func_match(struct ieee80211_vif *vif,
			      struct cfg80211_nan_match_params *match,
			      gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct cfg80211_nan_func *func;

	if (WARN_ON(vif->type != NL80211_IFTYPE_NAN))
		return;

	spin_lock_bh(&sdata->u.nan.func_lock);

	func = idr_find(&sdata->u.nan.function_inst_ids,  match->inst_id);
	if (WARN_ON(!func)) {
		spin_unlock_bh(&sdata->u.nan.func_lock);
		return;
	}
	match->cookie = func->cookie;

	spin_unlock_bh(&sdata->u.nan.func_lock);

	cfg80211_nan_match(ieee80211_vif_to_wdev(vif), match, gfp);
}
EXPORT_SYMBOL(ieee80211_nan_func_match);

static int ieee80211_set_multicast_to_unicast(struct wiphy *wiphy,
					      struct net_device *dev,
					      const bool enabled)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	sdata->u.ap.multicast_to_unicast = enabled;

	return 0;
}

void ieee80211_fill_txq_stats(struct cfg80211_txq_stats *txqstats,
			      struct txq_info *txqi)
{
	if (!(txqstats->filled & BIT(NL80211_TXQ_STATS_BACKLOG_BYTES))) {
		txqstats->filled |= BIT(NL80211_TXQ_STATS_BACKLOG_BYTES);
		txqstats->backlog_bytes = txqi->tin.backlog_bytes;
	}

	if (!(txqstats->filled & BIT(NL80211_TXQ_STATS_BACKLOG_PACKETS))) {
		txqstats->filled |= BIT(NL80211_TXQ_STATS_BACKLOG_PACKETS);
		txqstats->backlog_packets = txqi->tin.backlog_packets;
	}

	if (!(txqstats->filled & BIT(NL80211_TXQ_STATS_FLOWS))) {
		txqstats->filled |= BIT(NL80211_TXQ_STATS_FLOWS);
		txqstats->flows = txqi->tin.flows;
	}

	if (!(txqstats->filled & BIT(NL80211_TXQ_STATS_DROPS))) {
		txqstats->filled |= BIT(NL80211_TXQ_STATS_DROPS);
		txqstats->drops = txqi->cstats.drop_count;
	}

	if (!(txqstats->filled & BIT(NL80211_TXQ_STATS_ECN_MARKS))) {
		txqstats->filled |= BIT(NL80211_TXQ_STATS_ECN_MARKS);
		txqstats->ecn_marks = txqi->cstats.ecn_mark;
	}

	if (!(txqstats->filled & BIT(NL80211_TXQ_STATS_OVERLIMIT))) {
		txqstats->filled |= BIT(NL80211_TXQ_STATS_OVERLIMIT);
		txqstats->overlimit = txqi->tin.overlimit;
	}

	if (!(txqstats->filled & BIT(NL80211_TXQ_STATS_COLLISIONS))) {
		txqstats->filled |= BIT(NL80211_TXQ_STATS_COLLISIONS);
		txqstats->collisions = txqi->tin.collisions;
	}

	if (!(txqstats->filled & BIT(NL80211_TXQ_STATS_TX_BYTES))) {
		txqstats->filled |= BIT(NL80211_TXQ_STATS_TX_BYTES);
		txqstats->tx_bytes = txqi->tin.tx_bytes;
	}

	if (!(txqstats->filled & BIT(NL80211_TXQ_STATS_TX_PACKETS))) {
		txqstats->filled |= BIT(NL80211_TXQ_STATS_TX_PACKETS);
		txqstats->tx_packets = txqi->tin.tx_packets;
	}
}

static int ieee80211_get_txq_stats(struct wiphy *wiphy,
				   struct wireless_dev *wdev,
				   struct cfg80211_txq_stats *txqstats)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata;
	int ret = 0;

	spin_lock_bh(&local->fq.lock);
	rcu_read_lock();

	if (wdev) {
		sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);
		if (!sdata->vif.txq) {
			ret = 1;
			goto out;
		}
		ieee80211_fill_txq_stats(txqstats, to_txq_info(sdata->vif.txq));
	} else {
		/* phy stats */
		txqstats->filled |= BIT(NL80211_TXQ_STATS_BACKLOG_PACKETS) |
				    BIT(NL80211_TXQ_STATS_BACKLOG_BYTES) |
				    BIT(NL80211_TXQ_STATS_OVERLIMIT) |
				    BIT(NL80211_TXQ_STATS_OVERMEMORY) |
				    BIT(NL80211_TXQ_STATS_COLLISIONS) |
				    BIT(NL80211_TXQ_STATS_MAX_FLOWS);
		txqstats->backlog_packets = local->fq.backlog;
		txqstats->backlog_bytes = local->fq.memory_usage;
		txqstats->overlimit = local->fq.overlimit;
		txqstats->overmemory = local->fq.overmemory;
		txqstats->collisions = local->fq.collisions;
		txqstats->max_flows = local->fq.flows_cnt;
	}

out:
	rcu_read_unlock();
	spin_unlock_bh(&local->fq.lock);

	return ret;
}

static int
ieee80211_get_ftm_responder_stats(struct wiphy *wiphy,
				  struct net_device *dev,
				  struct cfg80211_ftm_responder_stats *ftm_stats)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	return drv_get_ftm_responder_stats(local, sdata, ftm_stats);
}

static int
ieee80211_start_pmsr(struct wiphy *wiphy, struct wireless_dev *dev,
		     struct cfg80211_pmsr_request *request)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(dev);

	return drv_start_pmsr(local, sdata, request);
}

static void
ieee80211_abort_pmsr(struct wiphy *wiphy, struct wireless_dev *dev,
		     struct cfg80211_pmsr_request *request)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(dev);

	return drv_abort_pmsr(local, sdata, request);
}

static int ieee80211_set_tid_config(struct wiphy *wiphy,
				    struct net_device *dev,
				    struct cfg80211_tid_config *tid_conf)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct sta_info *sta;

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	if (!sdata->local->ops->set_tid_config)
		return -EOPNOTSUPP;

	if (!tid_conf->peer)
		return drv_set_tid_config(sdata->local, sdata, NULL, tid_conf);

	sta = sta_info_get_bss(sdata, tid_conf->peer);
	if (!sta)
		return -ENOENT;

	return drv_set_tid_config(sdata->local, sdata, &sta->sta, tid_conf);
}

static int ieee80211_reset_tid_config(struct wiphy *wiphy,
				      struct net_device *dev,
				      const u8 *peer, u8 tids)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct sta_info *sta;

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	if (!sdata->local->ops->reset_tid_config)
		return -EOPNOTSUPP;

	if (!peer)
		return drv_reset_tid_config(sdata->local, sdata, NULL, tids);

	sta = sta_info_get_bss(sdata, peer);
	if (!sta)
		return -ENOENT;

	return drv_reset_tid_config(sdata->local, sdata, &sta->sta, tids);
}

static int ieee80211_set_sar_specs(struct wiphy *wiphy,
				   struct cfg80211_sar_specs *sar)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	if (!local->ops->set_sar_specs)
		return -EOPNOTSUPP;

	return local->ops->set_sar_specs(&local->hw, sar);
}

static int
ieee80211_set_after_color_change_beacon(struct ieee80211_sub_if_data *sdata,
					u64 *changed)
{
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP: {
		int ret;

		if (!sdata->deflink.u.ap.next_beacon)
			return -EINVAL;

		ret = ieee80211_assign_beacon(sdata, &sdata->deflink,
					      sdata->deflink.u.ap.next_beacon,
					      NULL, NULL, changed);
		ieee80211_free_next_beacon(&sdata->deflink);

		if (ret < 0)
			return ret;

		break;
	}
	default:
		WARN_ON_ONCE(1);
		return -EINVAL;
	}

	return 0;
}

static int
ieee80211_set_color_change_beacon(struct ieee80211_sub_if_data *sdata,
				  struct cfg80211_color_change_settings *params,
				  u64 *changed)
{
	struct ieee80211_color_change_settings color_change = {};
	int err;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP:
		sdata->deflink.u.ap.next_beacon =
			cfg80211_beacon_dup(&params->beacon_next);
		if (!sdata->deflink.u.ap.next_beacon)
			return -ENOMEM;

		if (params->count <= 1)
			break;

		color_change.counter_offset_beacon =
			params->counter_offset_beacon;
		color_change.counter_offset_presp =
			params->counter_offset_presp;
		color_change.count = params->count;

		err = ieee80211_assign_beacon(sdata, &sdata->deflink,
					      &params->beacon_color_change,
					      NULL, &color_change, changed);
		if (err < 0) {
			ieee80211_free_next_beacon(&sdata->deflink);
			return err;
		}
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static void
ieee80211_color_change_bss_config_notify(struct ieee80211_sub_if_data *sdata,
					 u8 color, int enable, u64 changed)
{
	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	sdata->vif.bss_conf.he_bss_color.color = color;
	sdata->vif.bss_conf.he_bss_color.enabled = enable;
	changed |= BSS_CHANGED_HE_BSS_COLOR;

	ieee80211_link_info_change_notify(sdata, &sdata->deflink, changed);

	if (!sdata->vif.bss_conf.nontransmitted && sdata->vif.mbssid_tx_vif) {
		struct ieee80211_sub_if_data *child;

		list_for_each_entry(child, &sdata->local->interfaces, list) {
			if (child != sdata && child->vif.mbssid_tx_vif == &sdata->vif) {
				child->vif.bss_conf.he_bss_color.color = color;
				child->vif.bss_conf.he_bss_color.enabled = enable;
				ieee80211_link_info_change_notify(child,
								  &child->deflink,
								  BSS_CHANGED_HE_BSS_COLOR);
			}
		}
	}
}

static int ieee80211_color_change_finalize(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	u64 changed = 0;
	int err;

	lockdep_assert_wiphy(local->hw.wiphy);

	sdata->vif.bss_conf.color_change_active = false;

	err = ieee80211_set_after_color_change_beacon(sdata, &changed);
	if (err) {
		cfg80211_color_change_aborted_notify(sdata->dev);
		return err;
	}

	ieee80211_color_change_bss_config_notify(sdata,
						 sdata->vif.bss_conf.color_change_color,
						 1, changed);
	cfg80211_color_change_notify(sdata->dev);

	return 0;
}

void ieee80211_color_change_finalize_work(struct wiphy *wiphy,
					  struct wiphy_work *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data,
			     deflink.color_change_finalize_work);
	struct ieee80211_local *local = sdata->local;

	lockdep_assert_wiphy(local->hw.wiphy);

	/* AP might have been stopped while waiting for the lock. */
	if (!sdata->vif.bss_conf.color_change_active)
		return;

	if (!ieee80211_sdata_running(sdata))
		return;

	ieee80211_color_change_finalize(sdata);
}

void ieee80211_color_collision_detection_work(struct work_struct *work)
{
	struct delayed_work *delayed_work = to_delayed_work(work);
	struct ieee80211_link_data *link =
		container_of(delayed_work, struct ieee80211_link_data,
			     color_collision_detect_work);
	struct ieee80211_sub_if_data *sdata = link->sdata;

	cfg80211_obss_color_collision_notify(sdata->dev, link->color_bitmap);
}

void ieee80211_color_change_finish(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	wiphy_work_queue(sdata->local->hw.wiphy,
			 &sdata->deflink.color_change_finalize_work);
}
EXPORT_SYMBOL_GPL(ieee80211_color_change_finish);

void
ieee80211_obss_color_collision_notify(struct ieee80211_vif *vif,
				      u64 color_bitmap)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_link_data *link = &sdata->deflink;

	if (sdata->vif.bss_conf.color_change_active || sdata->vif.bss_conf.csa_active)
		return;

	if (delayed_work_pending(&link->color_collision_detect_work))
		return;

	link->color_bitmap = color_bitmap;
	/* queue the color collision detection event every 500 ms in order to
	 * avoid sending too much netlink messages to userspace.
	 */
	ieee80211_queue_delayed_work(&sdata->local->hw,
				     &link->color_collision_detect_work,
				     msecs_to_jiffies(500));
}
EXPORT_SYMBOL_GPL(ieee80211_obss_color_collision_notify);

static int
ieee80211_color_change(struct wiphy *wiphy, struct net_device *dev,
		       struct cfg80211_color_change_settings *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	u64 changed = 0;
	int err;

	lockdep_assert_wiphy(local->hw.wiphy);

	if (sdata->vif.bss_conf.nontransmitted)
		return -EINVAL;

	/* don't allow another color change if one is already active or if csa
	 * is active
	 */
	if (sdata->vif.bss_conf.color_change_active || sdata->vif.bss_conf.csa_active) {
		err = -EBUSY;
		goto out;
	}

	err = ieee80211_set_color_change_beacon(sdata, params, &changed);
	if (err)
		goto out;

	sdata->vif.bss_conf.color_change_active = true;
	sdata->vif.bss_conf.color_change_color = params->color;

	cfg80211_color_change_started_notify(sdata->dev, params->count);

	if (changed)
		ieee80211_color_change_bss_config_notify(sdata, 0, 0, changed);
	else
		/* if the beacon didn't change, we can finalize immediately */
		ieee80211_color_change_finalize(sdata);

out:

	return err;
}

static int
ieee80211_set_radar_background(struct wiphy *wiphy,
			       struct cfg80211_chan_def *chandef)
{
	struct ieee80211_local *local = wiphy_priv(wiphy);

	if (!local->ops->set_radar_background)
		return -EOPNOTSUPP;

	return local->ops->set_radar_background(&local->hw, chandef);
}

static int ieee80211_add_intf_link(struct wiphy *wiphy,
				   struct wireless_dev *wdev,
				   unsigned int link_id)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	if (wdev->use_4addr)
		return -EOPNOTSUPP;

	return ieee80211_vif_set_links(sdata, wdev->valid_links, 0);
}

static void ieee80211_del_intf_link(struct wiphy *wiphy,
				    struct wireless_dev *wdev,
				    unsigned int link_id)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_WDEV_TO_SUB_IF(wdev);

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	ieee80211_vif_set_links(sdata, wdev->valid_links, 0);
}

static int sta_add_link_station(struct ieee80211_local *local,
				struct ieee80211_sub_if_data *sdata,
				struct link_station_parameters *params)
{
	struct sta_info *sta;
	int ret;

	sta = sta_info_get_bss(sdata, params->mld_mac);
	if (!sta)
		return -ENOENT;

	if (!sta->sta.valid_links)
		return -EINVAL;

	if (sta->sta.valid_links & BIT(params->link_id))
		return -EALREADY;

	ret = ieee80211_sta_allocate_link(sta, params->link_id);
	if (ret)
		return ret;

	ret = sta_link_apply_parameters(local, sta, true, params);
	if (ret) {
		ieee80211_sta_free_link(sta, params->link_id);
		return ret;
	}

	/* ieee80211_sta_activate_link frees the link upon failure */
	return ieee80211_sta_activate_link(sta, params->link_id);
}

static int
ieee80211_add_link_station(struct wiphy *wiphy, struct net_device *dev,
			   struct link_station_parameters *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wiphy_priv(wiphy);

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	return sta_add_link_station(local, sdata, params);
}

static int sta_mod_link_station(struct ieee80211_local *local,
				struct ieee80211_sub_if_data *sdata,
				struct link_station_parameters *params)
{
	struct sta_info *sta;

	sta = sta_info_get_bss(sdata, params->mld_mac);
	if (!sta)
		return -ENOENT;

	if (!(sta->sta.valid_links & BIT(params->link_id)))
		return -EINVAL;

	return sta_link_apply_parameters(local, sta, false, params);
}

static int
ieee80211_mod_link_station(struct wiphy *wiphy, struct net_device *dev,
			   struct link_station_parameters *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = wiphy_priv(wiphy);

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	return sta_mod_link_station(local, sdata, params);
}

static int sta_del_link_station(struct ieee80211_sub_if_data *sdata,
				struct link_station_del_parameters *params)
{
	struct sta_info *sta;

	sta = sta_info_get_bss(sdata, params->mld_mac);
	if (!sta)
		return -ENOENT;

	if (!(sta->sta.valid_links & BIT(params->link_id)))
		return -EINVAL;

	/* must not create a STA without links */
	if (sta->sta.valid_links == BIT(params->link_id))
		return -EINVAL;

	ieee80211_sta_remove_link(sta, params->link_id);

	return 0;
}

static int
ieee80211_del_link_station(struct wiphy *wiphy, struct net_device *dev,
			   struct link_station_del_parameters *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	return sta_del_link_station(sdata, params);
}

static int ieee80211_set_hw_timestamp(struct wiphy *wiphy,
				      struct net_device *dev,
				      struct cfg80211_set_hw_timestamp *hwts)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;

	if (!local->ops->set_hw_timestamp)
		return -EOPNOTSUPP;

	if (!check_sdata_in_driver(sdata))
		return -EIO;

	return local->ops->set_hw_timestamp(&local->hw, &sdata->vif, hwts);
}

static int
ieee80211_set_ttlm(struct wiphy *wiphy, struct net_device *dev,
		   struct cfg80211_ttlm_params *params)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	lockdep_assert_wiphy(sdata->local->hw.wiphy);

	return ieee80211_req_neg_ttlm(sdata, params);
}

const struct cfg80211_ops mac80211_config_ops = {
	.add_virtual_intf = ieee80211_add_iface,
	.del_virtual_intf = ieee80211_del_iface,
	.change_virtual_intf = ieee80211_change_iface,
	.start_p2p_device = ieee80211_start_p2p_device,
	.stop_p2p_device = ieee80211_stop_p2p_device,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.add_key = ieee80211_add_key,
	.del_key = ieee80211_del_key,
	.get_key = ieee80211_get_key,
	.set_default_key = ieee80211_config_default_key,
	.set_default_mgmt_key = ieee80211_config_default_mgmt_key,
<<<<<<< HEAD
=======
	.set_default_beacon_key = ieee80211_config_default_beacon_key,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.start_ap = ieee80211_start_ap,
	.change_beacon = ieee80211_change_beacon,
	.stop_ap = ieee80211_stop_ap,
	.add_station = ieee80211_add_station,
	.del_station = ieee80211_del_station,
	.change_station = ieee80211_change_station,
	.get_station = ieee80211_get_station,
	.dump_station = ieee80211_dump_station,
	.dump_survey = ieee80211_dump_survey,
#ifdef CONFIG_MAC80211_MESH
	.add_mpath = ieee80211_add_mpath,
	.del_mpath = ieee80211_del_mpath,
	.change_mpath = ieee80211_change_mpath,
	.get_mpath = ieee80211_get_mpath,
	.dump_mpath = ieee80211_dump_mpath,
<<<<<<< HEAD
=======
	.get_mpp = ieee80211_get_mpp,
	.dump_mpp = ieee80211_dump_mpp,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.update_mesh_config = ieee80211_update_mesh_config,
	.get_mesh_config = ieee80211_get_mesh_config,
	.join_mesh = ieee80211_join_mesh,
	.leave_mesh = ieee80211_leave_mesh,
#endif
<<<<<<< HEAD
	.change_bss = ieee80211_change_bss,
	.set_txq_params = ieee80211_set_txq_params,
	.set_channel = ieee80211_set_channel,
	.suspend = ieee80211_suspend,
	.resume = ieee80211_resume,
	.scan = ieee80211_scan,
=======
	.join_ocb = ieee80211_join_ocb,
	.leave_ocb = ieee80211_leave_ocb,
	.change_bss = ieee80211_change_bss,
	.inform_bss = ieee80211_inform_bss,
	.set_txq_params = ieee80211_set_txq_params,
	.set_monitor_channel = ieee80211_set_monitor_channel,
	.suspend = ieee80211_suspend,
	.resume = ieee80211_resume,
	.scan = ieee80211_scan,
	.abort_scan = ieee80211_abort_scan,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.sched_scan_start = ieee80211_sched_scan_start,
	.sched_scan_stop = ieee80211_sched_scan_stop,
	.auth = ieee80211_auth,
	.assoc = ieee80211_assoc,
	.deauth = ieee80211_deauth,
	.disassoc = ieee80211_disassoc,
	.join_ibss = ieee80211_join_ibss,
	.leave_ibss = ieee80211_leave_ibss,
<<<<<<< HEAD
	.set_wiphy_params = ieee80211_set_wiphy_params,
	.set_tx_power = ieee80211_set_tx_power,
	.get_tx_power = ieee80211_get_tx_power,
	.set_wds_peer = ieee80211_set_wds_peer,
=======
	.set_mcast_rate = ieee80211_set_mcast_rate,
	.set_wiphy_params = ieee80211_set_wiphy_params,
	.set_tx_power = ieee80211_set_tx_power,
	.get_tx_power = ieee80211_get_tx_power,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.rfkill_poll = ieee80211_rfkill_poll,
	CFG80211_TESTMODE_CMD(ieee80211_testmode_cmd)
	CFG80211_TESTMODE_DUMP(ieee80211_testmode_dump)
	.set_power_mgmt = ieee80211_set_power_mgmt,
	.set_bitrate_mask = ieee80211_set_bitrate_mask,
	.remain_on_channel = ieee80211_remain_on_channel,
	.cancel_remain_on_channel = ieee80211_cancel_remain_on_channel,
	.mgmt_tx = ieee80211_mgmt_tx,
	.mgmt_tx_cancel_wait = ieee80211_mgmt_tx_cancel_wait,
	.set_cqm_rssi_config = ieee80211_set_cqm_rssi_config,
<<<<<<< HEAD
	.mgmt_frame_register = ieee80211_mgmt_frame_register,
	.set_antenna = ieee80211_set_antenna,
	.get_antenna = ieee80211_get_antenna,
	.set_ringparam = ieee80211_set_ringparam,
	.get_ringparam = ieee80211_get_ringparam,
	.set_rekey_data = ieee80211_set_rekey_data,
	.tdls_oper = ieee80211_tdls_oper,
	.tdls_mgmt = ieee80211_tdls_mgmt,
	.probe_client = ieee80211_probe_client,
	.get_channel = ieee80211_wiphy_get_channel,
	.set_noack_map = ieee80211_set_noack_map,
=======
	.set_cqm_rssi_range_config = ieee80211_set_cqm_rssi_range_config,
	.update_mgmt_frame_registrations =
		ieee80211_update_mgmt_frame_registrations,
	.set_antenna = ieee80211_set_antenna,
	.get_antenna = ieee80211_get_antenna,
	.set_rekey_data = ieee80211_set_rekey_data,
	.tdls_oper = ieee80211_tdls_oper,
	.tdls_mgmt = ieee80211_tdls_mgmt,
	.tdls_channel_switch = ieee80211_tdls_channel_switch,
	.tdls_cancel_channel_switch = ieee80211_tdls_cancel_channel_switch,
	.probe_client = ieee80211_probe_client,
	.set_noack_map = ieee80211_set_noack_map,
#ifdef CONFIG_PM
	.set_wakeup = ieee80211_set_wakeup,
#endif
	.get_channel = ieee80211_cfg_get_channel,
	.start_radar_detection = ieee80211_start_radar_detection,
	.end_cac = ieee80211_end_cac,
	.channel_switch = ieee80211_channel_switch,
	.set_qos_map = ieee80211_set_qos_map,
	.set_ap_chanwidth = ieee80211_set_ap_chanwidth,
	.add_tx_ts = ieee80211_add_tx_ts,
	.del_tx_ts = ieee80211_del_tx_ts,
	.start_nan = ieee80211_start_nan,
	.stop_nan = ieee80211_stop_nan,
	.nan_change_conf = ieee80211_nan_change_conf,
	.add_nan_func = ieee80211_add_nan_func,
	.del_nan_func = ieee80211_del_nan_func,
	.set_multicast_to_unicast = ieee80211_set_multicast_to_unicast,
	.tx_control_port = ieee80211_tx_control_port,
	.get_txq_stats = ieee80211_get_txq_stats,
	.get_ftm_responder_stats = ieee80211_get_ftm_responder_stats,
	.start_pmsr = ieee80211_start_pmsr,
	.abort_pmsr = ieee80211_abort_pmsr,
	.probe_mesh_link = ieee80211_probe_mesh_link,
	.set_tid_config = ieee80211_set_tid_config,
	.reset_tid_config = ieee80211_reset_tid_config,
	.set_sar_specs = ieee80211_set_sar_specs,
	.color_change = ieee80211_color_change,
	.set_radar_background = ieee80211_set_radar_background,
	.add_intf_link = ieee80211_add_intf_link,
	.del_intf_link = ieee80211_del_intf_link,
	.add_link_station = ieee80211_add_link_station,
	.mod_link_station = ieee80211_mod_link_station,
	.del_link_station = ieee80211_del_link_station,
	.set_hw_timestamp = ieee80211_set_hw_timestamp,
	.set_ttlm = ieee80211_set_ttlm,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
