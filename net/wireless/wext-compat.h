<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __WEXT_COMPAT
#define __WEXT_COMPAT

#include <net/iw_handler.h>
#include <linux/wireless.h>

<<<<<<< HEAD
int cfg80211_ibss_wext_siwfreq(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_freq *freq, char *extra);
=======
#ifdef CONFIG_CFG80211_WEXT_EXPORT
#define EXPORT_WEXT_HANDLER(h) EXPORT_SYMBOL_GPL(h)
#else
#define EXPORT_WEXT_HANDLER(h)
#endif /* CONFIG_CFG80211_WEXT_EXPORT */

int cfg80211_ibss_wext_siwfreq(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_freq *wextfreq, char *extra);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int cfg80211_ibss_wext_giwfreq(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_freq *freq, char *extra);
int cfg80211_ibss_wext_siwap(struct net_device *dev,
			     struct iw_request_info *info,
			     struct sockaddr *ap_addr, char *extra);
int cfg80211_ibss_wext_giwap(struct net_device *dev,
			     struct iw_request_info *info,
			     struct sockaddr *ap_addr, char *extra);
int cfg80211_ibss_wext_siwessid(struct net_device *dev,
				struct iw_request_info *info,
				struct iw_point *data, char *ssid);
int cfg80211_ibss_wext_giwessid(struct net_device *dev,
				struct iw_request_info *info,
				struct iw_point *data, char *ssid);

int cfg80211_mgd_wext_siwfreq(struct net_device *dev,
			      struct iw_request_info *info,
<<<<<<< HEAD
			      struct iw_freq *freq, char *extra);
=======
			      struct iw_freq *wextfreq, char *extra);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int cfg80211_mgd_wext_giwfreq(struct net_device *dev,
			      struct iw_request_info *info,
			      struct iw_freq *freq, char *extra);
int cfg80211_mgd_wext_siwap(struct net_device *dev,
			    struct iw_request_info *info,
			    struct sockaddr *ap_addr, char *extra);
int cfg80211_mgd_wext_giwap(struct net_device *dev,
			    struct iw_request_info *info,
			    struct sockaddr *ap_addr, char *extra);
int cfg80211_mgd_wext_siwessid(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_point *data, char *ssid);
int cfg80211_mgd_wext_giwessid(struct net_device *dev,
			       struct iw_request_info *info,
			       struct iw_point *data, char *ssid);

int cfg80211_wext_siwmlme(struct net_device *dev,
			  struct iw_request_info *info,
<<<<<<< HEAD
			  struct iw_point *data, char *extra);
int cfg80211_wext_siwgenie(struct net_device *dev,
			   struct iw_request_info *info,
			   struct iw_point *data, char *extra);


int cfg80211_wext_freq(struct wiphy *wiphy, struct iw_freq *freq);
=======
			  union iwreq_data *wrqu, char *extra);
int cfg80211_wext_siwgenie(struct net_device *dev,
			   struct iw_request_info *info,
			   union iwreq_data *wrqu, char *extra);


int cfg80211_wext_freq(struct iw_freq *freq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


extern const struct iw_handler_def cfg80211_wext_handler;
#endif /* __WEXT_COMPAT */
