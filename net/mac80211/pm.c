<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Portions
 * Copyright (C) 2020-2021, 2023 Intel Corporation
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/mac80211.h>
#include <net/rtnetlink.h>

#include "ieee80211_i.h"
#include "mesh.h"
#include "driver-ops.h"
#include "led.h"

<<<<<<< HEAD
/* return value indicates whether the driver should be further notified */
static bool ieee80211_quiesce(struct ieee80211_sub_if_data *sdata)
{
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_STATION:
		ieee80211_sta_quiesce(sdata);
		return true;
	case NL80211_IFTYPE_ADHOC:
		ieee80211_ibss_quiesce(sdata);
		return true;
	case NL80211_IFTYPE_MESH_POINT:
		ieee80211_mesh_quiesce(sdata);
		return true;
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_MONITOR:
		/* don't tell driver about this */
		return false;
	default:
		return true;
	}
=======
static void ieee80211_sched_scan_cancel(struct ieee80211_local *local)
{
	if (ieee80211_request_sched_scan_stop(local))
		return;
	cfg80211_sched_scan_stopped_locked(local->hw.wiphy, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int __ieee80211_suspend(struct ieee80211_hw *hw, struct cfg80211_wowlan *wowlan)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;
	struct sta_info *sta;

	if (!local->open_count)
		goto suspend;

<<<<<<< HEAD
	ieee80211_scan_cancel(local);

	if (hw->flags & IEEE80211_HW_AMPDU_AGGREGATION) {
		mutex_lock(&local->sta_mtx);
		list_for_each_entry(sta, &local->sta_list, list) {
			set_sta_flag(sta, WLAN_STA_BLOCK_BA);
			ieee80211_sta_tear_down_BA_sessions(sta, true);
		}
		mutex_unlock(&local->sta_mtx);
	}

	ieee80211_stop_queues_by_reason(hw,
			IEEE80211_QUEUE_STOP_REASON_SUSPEND);
=======
	local->suspending = true;
	mb(); /* make suspending visible before any cancellation */

	ieee80211_scan_cancel(local);

	ieee80211_dfs_cac_cancel(local);

	ieee80211_roc_purge(local, NULL);

	ieee80211_del_virtual_monitor(local);

	if (ieee80211_hw_check(hw, AMPDU_AGGREGATION) &&
	    !(wowlan && wowlan->any)) {
		lockdep_assert_wiphy(local->hw.wiphy);
		list_for_each_entry(sta, &local->sta_list, list) {
			set_sta_flag(sta, WLAN_STA_BLOCK_BA);
			ieee80211_sta_tear_down_BA_sessions(
					sta, AGG_STOP_LOCAL_REQUEST);
		}
	}

	/* keep sched_scan only in case of 'any' trigger */
	if (!(wowlan && wowlan->any))
		ieee80211_sched_scan_cancel(local);

	ieee80211_stop_queues_by_reason(hw,
					IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_SUSPEND,
					false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* flush out all packets */
	synchronize_net();

<<<<<<< HEAD
	drv_flush(local, false);
=======
	ieee80211_flush_queues(local, NULL, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	local->quiescing = true;
	/* make quiescing visible to timers everywhere */
	mb();

	flush_workqueue(local->workqueue);

	/* Don't try to run timers while suspended. */
	del_timer_sync(&local->sta_cleanup);

	 /*
	 * Note that this particular timer doesn't need to be
	 * restarted at resume.
	 */
<<<<<<< HEAD
	cancel_work_sync(&local->dynamic_ps_enable_work);
	del_timer_sync(&local->dynamic_ps_timer);

	local->wowlan = wowlan && local->open_count;
	if (local->wowlan) {
		int err = drv_suspend(local, wowlan);
		if (err < 0) {
			local->quiescing = false;
			return err;
		} else if (err > 0) {
			WARN_ON(err != 1);
			local->wowlan = false;
		} else {
			list_for_each_entry(sdata, &local->interfaces, list) {
				cancel_work_sync(&sdata->work);
				ieee80211_quiesce(sdata);
			}
=======
	wiphy_work_cancel(local->hw.wiphy, &local->dynamic_ps_enable_work);
	del_timer_sync(&local->dynamic_ps_timer);

	local->wowlan = wowlan;
	if (local->wowlan) {
		int err;

		/* Drivers don't expect to suspend while some operations like
		 * authenticating or associating are in progress. It doesn't
		 * make sense anyway to accept that, since the authentication
		 * or association would never finish since the driver can't do
		 * that on its own.
		 * Thus, clean up in-progress auth/assoc first.
		 */
		list_for_each_entry(sdata, &local->interfaces, list) {
			if (!ieee80211_sdata_running(sdata))
				continue;
			if (sdata->vif.type != NL80211_IFTYPE_STATION)
				continue;
			ieee80211_mgd_quiesce(sdata);
			/* If suspended during TX in progress, and wowlan
			 * is enabled (connection will be active) there
			 * can be a race where the driver is put out
			 * of power-save due to TX and during suspend
			 * dynamic_ps_timer is cancelled and TX packet
			 * is flushed, leaving the driver in ACTIVE even
			 * after resuming until dynamic_ps_timer puts
			 * driver back in DOZE.
			 */
			if (sdata->u.mgd.associated &&
			    sdata->u.mgd.powersave &&
			     !(local->hw.conf.flags & IEEE80211_CONF_PS)) {
				local->hw.conf.flags |= IEEE80211_CONF_PS;
				ieee80211_hw_config(local,
						    IEEE80211_CONF_CHANGE_PS);
			}
		}

		err = drv_suspend(local, wowlan);
		if (err < 0) {
			local->quiescing = false;
			local->wowlan = false;
			if (ieee80211_hw_check(hw, AMPDU_AGGREGATION)) {
				lockdep_assert_wiphy(local->hw.wiphy);
				list_for_each_entry(sta,
						    &local->sta_list, list) {
					clear_sta_flag(sta, WLAN_STA_BLOCK_BA);
				}
			}
			ieee80211_wake_queues_by_reason(hw,
					IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_SUSPEND,
					false);
			return err;
		} else if (err > 0) {
			WARN_ON(err != 1);
			/* cfg80211 will call back into mac80211 to disconnect
			 * all interfaces, allow that to proceed properly
			 */
			ieee80211_wake_queues_by_reason(hw,
					IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_SUSPEND,
					false);
			return err;
		} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto suspend;
		}
	}

<<<<<<< HEAD
	/* disable keys */
	list_for_each_entry(sdata, &local->interfaces, list)
		ieee80211_disable_keys(sdata);

	/* tear down aggregation sessions and remove STAs */
	mutex_lock(&local->sta_mtx);
	list_for_each_entry(sta, &local->sta_list, list) {
		if (sta->uploaded) {
			enum ieee80211_sta_state state;

			state = sta->sta_state;
			for (; state > IEEE80211_STA_NOTEXIST; state--)
				WARN_ON(drv_sta_state(local, sta->sdata, sta,
						      state, state - 1));
		}

		mesh_plink_quiesce(sta);
	}
	mutex_unlock(&local->sta_mtx);

	/* remove all interfaces */
	list_for_each_entry(sdata, &local->interfaces, list) {
		cancel_work_sync(&sdata->work);

		if (!ieee80211_quiesce(sdata))
			continue;

		if (!ieee80211_sdata_running(sdata))
			continue;

		/* disable beaconing */
		ieee80211_bss_info_change_notify(sdata,
			BSS_CHANGED_BEACON_ENABLED);

		drv_remove_interface(local, sdata);
	}

	/* stop hardware - this must stop RX */
	if (local->open_count)
		ieee80211_stop_device(local);
=======
	/* remove all interfaces that were created in the driver */
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(sdata))
			continue;
		switch (sdata->vif.type) {
		case NL80211_IFTYPE_AP_VLAN:
		case NL80211_IFTYPE_MONITOR:
			continue;
		case NL80211_IFTYPE_STATION:
			ieee80211_mgd_quiesce(sdata);
			break;
		default:
			break;
		}

		wiphy_delayed_work_flush(local->hw.wiphy,
					 &sdata->dec_tailroom_needed_wk);
		drv_remove_interface(local, sdata);
	}

	/*
	 * We disconnected on all interfaces before suspend, all channel
	 * contexts should be released.
	 */
	WARN_ON(!list_empty(&local->chanctx_list));

	/* stop hardware - this must stop RX */
	ieee80211_stop_device(local);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

 suspend:
	local->suspended = true;
	/* need suspended to be visible before quiescing is false */
	barrier();
	local->quiescing = false;
<<<<<<< HEAD
=======
	local->suspending = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
 * __ieee80211_resume() is a static inline which just calls
 * ieee80211_reconfig(), which is also needed for hardware
 * hang/firmware failure/etc. recovery.
 */
<<<<<<< HEAD
=======

void ieee80211_report_wowlan_wakeup(struct ieee80211_vif *vif,
				    struct cfg80211_wowlan_wakeup *wakeup,
				    gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	cfg80211_report_wowlan_wakeup(&sdata->wdev, wakeup, gfp);
}
EXPORT_SYMBOL(ieee80211_report_wowlan_wakeup);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
