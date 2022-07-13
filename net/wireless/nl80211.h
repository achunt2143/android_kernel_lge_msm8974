<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Portions of this file
 * Copyright (C) 2018, 2020-2022 Intel Corporation
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __NET_WIRELESS_NL80211_H
#define __NET_WIRELESS_NL80211_H

#include "core.h"

int nl80211_init(void);
void nl80211_exit(void);
<<<<<<< HEAD
void nl80211_notify_dev_rename(struct cfg80211_registered_device *rdev);
void nl80211_send_scan_start(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev);
void nl80211_send_scan_done(struct cfg80211_registered_device *rdev,
			    struct net_device *netdev);
void nl80211_send_scan_aborted(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev);
void nl80211_send_sched_scan(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev, u32 cmd);
void nl80211_send_sched_scan_results(struct cfg80211_registered_device *rdev,
				     struct net_device *netdev);
void nl80211_send_reg_change_event(struct regulatory_request *request);
=======

void *nl80211hdr_put(struct sk_buff *skb, u32 portid, u32 seq,
		     int flags, u8 cmd);
bool nl80211_put_sta_rate(struct sk_buff *msg, struct rate_info *info,
			  int attr);

static inline u64 wdev_id(struct wireless_dev *wdev)
{
	return (u64)wdev->identifier |
	       ((u64)wiphy_to_rdev(wdev->wiphy)->wiphy_idx << 32);
}

int nl80211_parse_chandef(struct cfg80211_registered_device *rdev,
			  struct genl_info *info,
			  struct cfg80211_chan_def *chandef);
int nl80211_parse_random_mac(struct nlattr **attrs,
			     u8 *mac_addr, u8 *mac_addr_mask);

void nl80211_notify_wiphy(struct cfg80211_registered_device *rdev,
			  enum nl80211_commands cmd);
void nl80211_notify_iface(struct cfg80211_registered_device *rdev,
			  struct wireless_dev *wdev,
			  enum nl80211_commands cmd);
void nl80211_send_scan_start(struct cfg80211_registered_device *rdev,
			     struct wireless_dev *wdev);
struct sk_buff *nl80211_build_scan_msg(struct cfg80211_registered_device *rdev,
				       struct wireless_dev *wdev, bool aborted);
void nl80211_send_scan_msg(struct cfg80211_registered_device *rdev,
			   struct sk_buff *msg);
void nl80211_send_sched_scan(struct cfg80211_sched_scan_request *req, u32 cmd);
void nl80211_common_reg_change_event(enum nl80211_commands cmd_id,
				     struct regulatory_request *request);

static inline void
nl80211_send_reg_change_event(struct regulatory_request *request)
{
	nl80211_common_reg_change_event(NL80211_CMD_REG_CHANGE, request);
}

static inline void
nl80211_send_wiphy_reg_change_event(struct regulatory_request *request)
{
	nl80211_common_reg_change_event(NL80211_CMD_WIPHY_REG_CHANGE, request);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void nl80211_send_rx_auth(struct cfg80211_registered_device *rdev,
			  struct net_device *netdev,
			  const u8 *buf, size_t len, gfp_t gfp);
void nl80211_send_rx_assoc(struct cfg80211_registered_device *rdev,
			   struct net_device *netdev,
<<<<<<< HEAD
			   const u8 *buf, size_t len, gfp_t gfp);
void nl80211_send_deauth(struct cfg80211_registered_device *rdev,
			 struct net_device *netdev,
			 const u8 *buf, size_t len, gfp_t gfp);
void nl80211_send_disassoc(struct cfg80211_registered_device *rdev,
			   struct net_device *netdev,
			   const u8 *buf, size_t len, gfp_t gfp);
void nl80211_send_unprot_deauth(struct cfg80211_registered_device *rdev,
				struct net_device *netdev,
				const u8 *buf, size_t len, gfp_t gfp);
void nl80211_send_unprot_disassoc(struct cfg80211_registered_device *rdev,
				  struct net_device *netdev,
				  const u8 *buf, size_t len, gfp_t gfp);
=======
			   const struct cfg80211_rx_assoc_resp_data *data);
void nl80211_send_deauth(struct cfg80211_registered_device *rdev,
			 struct net_device *netdev,
			 const u8 *buf, size_t len,
			 bool reconnect, gfp_t gfp);
void nl80211_send_disassoc(struct cfg80211_registered_device *rdev,
			   struct net_device *netdev,
			   const u8 *buf, size_t len,
			   bool reconnect, gfp_t gfp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void nl80211_send_auth_timeout(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev,
			       const u8 *addr, gfp_t gfp);
void nl80211_send_assoc_timeout(struct cfg80211_registered_device *rdev,
				struct net_device *netdev,
				const u8 *addr, gfp_t gfp);
void nl80211_send_connect_result(struct cfg80211_registered_device *rdev,
<<<<<<< HEAD
				 struct net_device *netdev, const u8 *bssid,
				 const u8 *req_ie, size_t req_ie_len,
				 const u8 *resp_ie, size_t resp_ie_len,
				 u16 status, gfp_t gfp);
void nl80211_send_roamed(struct cfg80211_registered_device *rdev,
			 struct net_device *netdev, const u8 *bssid,
			 const u8 *req_ie, size_t req_ie_len,
			 const u8 *resp_ie, size_t resp_ie_len, gfp_t gfp);
=======
				 struct net_device *netdev,
				 struct cfg80211_connect_resp_params *params,
				 gfp_t gfp);
void nl80211_send_roamed(struct cfg80211_registered_device *rdev,
			 struct net_device *netdev,
			 struct cfg80211_roam_info *info, gfp_t gfp);
/* For STA/GC, indicate port authorized with AP/GO bssid.
 * For GO/AP, use peer GC/STA mac_addr.
 */
void nl80211_send_port_authorized(struct cfg80211_registered_device *rdev,
				  struct net_device *netdev, const u8 *peer_addr,
				  const u8 *td_bitmap, u8 td_bitmap_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void nl80211_send_disconnected(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev, u16 reason,
			       const u8 *ie, size_t ie_len, bool from_ap);

<<<<<<< HEAD
void nl80211_send_new_peer_candidate(struct cfg80211_registered_device *rdev,
				     struct net_device *netdev,
				     const u8 *macaddr, const u8* ie, u8 ie_len,
				     gfp_t gfp);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void
nl80211_michael_mic_failure(struct cfg80211_registered_device *rdev,
			    struct net_device *netdev, const u8 *addr,
			    enum nl80211_key_type key_type,
			    int key_id, const u8 *tsc, gfp_t gfp);

void
nl80211_send_beacon_hint_event(struct wiphy *wiphy,
			       struct ieee80211_channel *channel_before,
			       struct ieee80211_channel *channel_after);

void nl80211_send_ibss_bssid(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev, const u8 *bssid,
			     gfp_t gfp);

<<<<<<< HEAD
void nl80211_send_remain_on_channel(struct cfg80211_registered_device *rdev,
				    struct net_device *netdev,
				    u64 cookie,
				    struct ieee80211_channel *chan,
				    enum nl80211_channel_type channel_type,
				    unsigned int duration, gfp_t gfp);
void nl80211_send_remain_on_channel_cancel(
	struct cfg80211_registered_device *rdev, struct net_device *netdev,
	u64 cookie, struct ieee80211_channel *chan,
	enum nl80211_channel_type channel_type, gfp_t gfp);

void nl80211_send_sta_event(struct cfg80211_registered_device *rdev,
			    struct net_device *dev, const u8 *mac_addr,
			    struct station_info *sinfo, gfp_t gfp);
void nl80211_send_sta_del_event(struct cfg80211_registered_device *rdev,
				struct net_device *dev, const u8 *mac_addr,
				gfp_t gfp);

int nl80211_send_mgmt(struct cfg80211_registered_device *rdev,
		      struct net_device *netdev, u32 nlpid,
		      int freq, int sig_dbm,
		      const u8 *buf, size_t len, gfp_t gfp);
void nl80211_send_mgmt_tx_status(struct cfg80211_registered_device *rdev,
				 struct net_device *netdev, u64 cookie,
				 const u8 *buf, size_t len, bool ack,
				 gfp_t gfp);

void
nl80211_send_cqm_rssi_notify(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev,
			     enum nl80211_cqm_rssi_threshold_event rssi_event,
			     gfp_t gfp);
void
nl80211_send_cqm_pktloss_notify(struct cfg80211_registered_device *rdev,
				struct net_device *netdev, const u8 *peer,
				u32 num_packets, gfp_t gfp);

void nl80211_gtk_rekey_notify(struct cfg80211_registered_device *rdev,
			      struct net_device *netdev, const u8 *bssid,
			      const u8 *replay_ctr, gfp_t gfp);

void nl80211_pmksa_candidate_notify(struct cfg80211_registered_device *rdev,
				    struct net_device *netdev, int index,
				    const u8 *bssid, bool preauth, gfp_t gfp);

bool nl80211_unexpected_frame(struct net_device *dev,
			      const u8 *addr, gfp_t gfp);
bool nl80211_unexpected_4addr_frame(struct net_device *dev,
				    const u8 *addr, gfp_t gfp);
=======
int nl80211_send_mgmt(struct cfg80211_registered_device *rdev,
		      struct wireless_dev *wdev, u32 nlpid,
		      struct cfg80211_rx_info *info, gfp_t gfp);

void
nl80211_radar_notify(struct cfg80211_registered_device *rdev,
		     const struct cfg80211_chan_def *chandef,
		     enum nl80211_radar_event event,
		     struct net_device *netdev, gfp_t gfp);

void nl80211_send_ap_stopped(struct wireless_dev *wdev, unsigned int link_id);

void cfg80211_rdev_free_coalesce(struct cfg80211_registered_device *rdev);

/* peer measurement */
int nl80211_pmsr_start(struct sk_buff *skb, struct genl_info *info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __NET_WIRELESS_NL80211_H */
