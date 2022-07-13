<<<<<<< HEAD
/* IEEE 802.11 SoftMAC layer
 * Copyright (c) 2005 Andrea Merello <andreamrl@tiscali.it>
=======
// SPDX-License-Identifier: GPL-2.0
/* IEEE 802.11 SoftMAC layer
 * Copyright (c) 2005 Andrea Merello <andrea.merello@gmail.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Mostly extracted from the rtl8180-sa2400 driver for the
 * in-kernel generic ieee802.11 stack.
 *
 * Some pieces of code might be stolen from ipw2100 driver
 * copyright of who own it's copyright ;-)
 *
 * PS wx handler mostly stolen from hostap, copyright who
 * own it's copyright ;-)
<<<<<<< HEAD
 *
 * released under the GPL
 */


#include "rtllib.h"
#include "dot11d.h"
/* FIXME: add A freqs */

const long rtllib_wlan_frequencies[] = {
	2412, 2417, 2422, 2427,
	2432, 2437, 2442, 2447,
	2452, 2457, 2462, 2467,
	2472, 2484
};
EXPORT_SYMBOL(rtllib_wlan_frequencies);

=======
 */
#include <linux/etherdevice.h>

#include "rtllib.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int rtllib_wx_set_freq(struct rtllib_device *ieee, struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	int ret;
	struct iw_freq *fwrq = &wrqu->freq;

<<<<<<< HEAD
	down(&ieee->wx_sem);
=======
	mutex_lock(&ieee->wx_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ieee->iw_mode == IW_MODE_INFRA) {
		ret = 0;
		goto out;
	}

	/* if setting by freq convert to channel */
	if (fwrq->e == 1) {
<<<<<<< HEAD
		if ((fwrq->m >= (int) 2.412e8 &&
		     fwrq->m <= (int) 2.487e8)) {
			int f = fwrq->m / 100000;
			int c = 0;

			while ((c < 14) && (f != rtllib_wlan_frequencies[c]))
				c++;

			/* hack to fall through */
			fwrq->e = 0;
			fwrq->m = c + 1;
=======
		if ((fwrq->m >= (int)2.412e8 &&
		     fwrq->m <= (int)2.487e8)) {
			fwrq->m = ieee80211_freq_khz_to_channel(fwrq->m / 100);
			fwrq->e = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	if (fwrq->e > 0 || fwrq->m > 14 || fwrq->m < 1) {
		ret = -EOPNOTSUPP;
		goto out;

	} else { /* Set the channel */

		if (ieee->active_channel_map[fwrq->m] != 1) {
			ret = -EINVAL;
			goto out;
		}
		ieee->current_network.channel = fwrq->m;
		ieee->set_chan(ieee->dev, ieee->current_network.channel);
<<<<<<< HEAD

		if (ieee->iw_mode == IW_MODE_ADHOC ||
		    ieee->iw_mode == IW_MODE_MASTER)
			if (ieee->state == RTLLIB_LINKED) {
				rtllib_stop_send_beacons(ieee);
				rtllib_start_send_beacons(ieee);
			}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ret = 0;
out:
<<<<<<< HEAD
	up(&ieee->wx_sem);
=======
	mutex_unlock(&ieee->wx_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(rtllib_wx_set_freq);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int rtllib_wx_get_freq(struct rtllib_device *ieee,
			     struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	struct iw_freq *fwrq = &wrqu->freq;

	if (ieee->current_network.channel == 0)
		return -1;
<<<<<<< HEAD
	fwrq->m = rtllib_wlan_frequencies[ieee->current_network.channel-1] *
		  100000;
=======
	fwrq->m = ieee80211_channel_to_freq_khz(ieee->current_network.channel,
						NL80211_BAND_2GHZ) * 100;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fwrq->e = 1;
	return 0;
}
EXPORT_SYMBOL(rtllib_wx_get_freq);

int rtllib_wx_get_wap(struct rtllib_device *ieee,
			    struct iw_request_info *info,
			    union iwreq_data *wrqu, char *extra)
{
	unsigned long flags;

	wrqu->ap_addr.sa_family = ARPHRD_ETHER;

	if (ieee->iw_mode == IW_MODE_MONITOR)
		return -1;

	/* We want avoid to give to the user inconsistent infos*/
	spin_lock_irqsave(&ieee->lock, flags);

<<<<<<< HEAD
	if (ieee->state != RTLLIB_LINKED &&
		ieee->state != RTLLIB_LINKED_SCANNING &&
		ieee->wap_set == 0)

		memset(wrqu->ap_addr.sa_data, 0, ETH_ALEN);
=======
	if (ieee->link_state != MAC80211_LINKED &&
		ieee->link_state != MAC80211_LINKED_SCANNING &&
		ieee->wap_set == 0)

		eth_zero_addr(wrqu->ap_addr.sa_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		memcpy(wrqu->ap_addr.sa_data,
		       ieee->current_network.bssid, ETH_ALEN);

	spin_unlock_irqrestore(&ieee->lock, flags);

	return 0;
}
EXPORT_SYMBOL(rtllib_wx_get_wap);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int rtllib_wx_set_wap(struct rtllib_device *ieee,
			 struct iw_request_info *info,
			 union iwreq_data *awrq,
			 char *extra)
{
<<<<<<< HEAD

	int ret = 0;
	u8 zero[] = {0, 0, 0, 0, 0, 0};
=======
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;

	short ifup = ieee->proto_started;
	struct sockaddr *temp = (struct sockaddr *)awrq;

	rtllib_stop_scan_syncro(ieee);

<<<<<<< HEAD
	down(&ieee->wx_sem);
	/* use ifconfig hw ether */
	if (ieee->iw_mode == IW_MODE_MASTER) {
		ret = -1;
		goto out;
	}
=======
	mutex_lock(&ieee->wx_mutex);
	/* use ifconfig hw ether */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (temp->sa_family != ARPHRD_ETHER) {
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	if (memcmp(temp->sa_data, zero, ETH_ALEN) == 0) {
		spin_lock_irqsave(&ieee->lock, flags);
		memcpy(ieee->current_network.bssid, temp->sa_data, ETH_ALEN);
=======
	if (is_zero_ether_addr(temp->sa_data)) {
		spin_lock_irqsave(&ieee->lock, flags);
		ether_addr_copy(ieee->current_network.bssid, temp->sa_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ieee->wap_set = 0;
		spin_unlock_irqrestore(&ieee->lock, flags);
		ret = -1;
		goto out;
	}

<<<<<<< HEAD

	if (ifup)
		rtllib_stop_protocol(ieee, true);
=======
	if (ifup)
		rtllib_stop_protocol(ieee);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* just to avoid to give inconsistent infos in the
	 * get wx method. not really needed otherwise
	 */
	spin_lock_irqsave(&ieee->lock, flags);

	ieee->cannot_notify = false;
<<<<<<< HEAD
	memcpy(ieee->current_network.bssid, temp->sa_data, ETH_ALEN);
	ieee->wap_set = (memcmp(temp->sa_data, zero, ETH_ALEN) != 0);
=======
	ether_addr_copy(ieee->current_network.bssid, temp->sa_data);
	ieee->wap_set = !is_zero_ether_addr(temp->sa_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock_irqrestore(&ieee->lock, flags);

	if (ifup)
		rtllib_start_protocol(ieee);
out:
<<<<<<< HEAD
	up(&ieee->wx_sem);
=======
	mutex_unlock(&ieee->wx_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(rtllib_wx_set_wap);

int rtllib_wx_get_essid(struct rtllib_device *ieee, struct iw_request_info *a,
			 union iwreq_data *wrqu, char *b)
{
	int len, ret = 0;
	unsigned long flags;

	if (ieee->iw_mode == IW_MODE_MONITOR)
		return -1;

	/* We want avoid to give to the user inconsistent infos*/
	spin_lock_irqsave(&ieee->lock, flags);

	if (ieee->current_network.ssid[0] == '\0' ||
		ieee->current_network.ssid_len == 0) {
		ret = -1;
		goto out;
	}

<<<<<<< HEAD
	if (ieee->state != RTLLIB_LINKED &&
		ieee->state != RTLLIB_LINKED_SCANNING &&
=======
	if (ieee->link_state != MAC80211_LINKED &&
		ieee->link_state != MAC80211_LINKED_SCANNING &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ieee->ssid_set == 0) {
		ret = -1;
		goto out;
	}
	len = ieee->current_network.ssid_len;
	wrqu->essid.length = len;
	strncpy(b, ieee->current_network.ssid, len);
	wrqu->essid.flags = 1;

out:
	spin_unlock_irqrestore(&ieee->lock, flags);

	return ret;
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(rtllib_wx_get_essid);

int rtllib_wx_set_rate(struct rtllib_device *ieee,
			     struct iw_request_info *info,
			     union iwreq_data *wrqu, char *extra)
{
<<<<<<< HEAD

	u32 target_rate = wrqu->bitrate.value;

	ieee->rate = target_rate/100000;
=======
	u32 target_rate = wrqu->bitrate.value;

	ieee->rate = target_rate / 100000;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL(rtllib_wx_set_rate);

int rtllib_wx_get_rate(struct rtllib_device *ieee,
			     struct iw_request_info *info,
			     union iwreq_data *wrqu, char *extra)
{
<<<<<<< HEAD
	u32 tmp_rate = 0;
	tmp_rate = TxCountToDataRate(ieee,
=======
	u32 tmp_rate;

	tmp_rate = tx_count_to_data_rate(ieee,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				     ieee->softmac_stats.CurrentShowTxate);
	wrqu->bitrate.value = tmp_rate * 500000;

	return 0;
}
EXPORT_SYMBOL(rtllib_wx_get_rate);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int rtllib_wx_set_rts(struct rtllib_device *ieee,
			     struct iw_request_info *info,
			     union iwreq_data *wrqu, char *extra)
{
<<<<<<< HEAD
	if (wrqu->rts.disabled || !wrqu->rts.fixed)
		ieee->rts = DEFAULT_RTS_THRESHOLD;
	else {
=======
	if (wrqu->rts.disabled || !wrqu->rts.fixed) {
		ieee->rts = DEFAULT_RTS_THRESHOLD;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (wrqu->rts.value < MIN_RTS_THRESHOLD ||
				wrqu->rts.value > MAX_RTS_THRESHOLD)
			return -EINVAL;
		ieee->rts = wrqu->rts.value;
	}
	return 0;
}
EXPORT_SYMBOL(rtllib_wx_set_rts);

int rtllib_wx_get_rts(struct rtllib_device *ieee,
			     struct iw_request_info *info,
			     union iwreq_data *wrqu, char *extra)
{
	wrqu->rts.value = ieee->rts;
	wrqu->rts.fixed = 0;	/* no auto select */
	wrqu->rts.disabled = (wrqu->rts.value == DEFAULT_RTS_THRESHOLD);
	return 0;
}
EXPORT_SYMBOL(rtllib_wx_get_rts);

int rtllib_wx_set_mode(struct rtllib_device *ieee, struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	int set_mode_status = 0;

	rtllib_stop_scan_syncro(ieee);
<<<<<<< HEAD
	down(&ieee->wx_sem);
	switch (wrqu->mode) {
	case IW_MODE_MONITOR:
	case IW_MODE_ADHOC:
=======
	mutex_lock(&ieee->wx_mutex);
	switch (wrqu->mode) {
	case IW_MODE_MONITOR:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IW_MODE_INFRA:
		break;
	case IW_MODE_AUTO:
		wrqu->mode = IW_MODE_INFRA;
		break;
	default:
		set_mode_status = -EINVAL;
		goto out;
	}

	if (wrqu->mode == ieee->iw_mode)
		goto out;

	if (wrqu->mode == IW_MODE_MONITOR) {
		ieee->dev->type = ARPHRD_IEEE80211;
<<<<<<< HEAD
		rtllib_EnableNetMonitorMode(ieee->dev, false);
	} else {
		ieee->dev->type = ARPHRD_ETHER;
		if (ieee->iw_mode == IW_MODE_MONITOR)
			rtllib_DisableNetMonitorMode(ieee->dev, false);
=======
		rtllib_enable_net_monitor_mode(ieee->dev, false);
	} else {
		ieee->dev->type = ARPHRD_ETHER;
		if (ieee->iw_mode == IW_MODE_MONITOR)
			rtllib_disable_net_monitor_mode(ieee->dev, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!ieee->proto_started) {
		ieee->iw_mode = wrqu->mode;
	} else {
<<<<<<< HEAD
		rtllib_stop_protocol(ieee, true);
=======
		rtllib_stop_protocol(ieee);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ieee->iw_mode = wrqu->mode;
		rtllib_start_protocol(ieee);
	}

out:
<<<<<<< HEAD
	up(&ieee->wx_sem);
=======
	mutex_unlock(&ieee->wx_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return set_mode_status;
}
EXPORT_SYMBOL(rtllib_wx_set_mode);

void rtllib_wx_sync_scan_wq(void *data)
{
<<<<<<< HEAD
	struct rtllib_device *ieee = container_of_work_rsl(data,
				     struct rtllib_device, wx_sync_scan_wq);
=======
	struct rtllib_device *ieee = container_of(data, struct rtllib_device, wx_sync_scan_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	short chan;
	enum ht_extchnl_offset chan_offset = 0;
	enum ht_channel_width bandwidth = 0;
	int b40M = 0;
<<<<<<< HEAD
	static int count;

	if (!(ieee->softmac_features & IEEE_SOFTMAC_SCAN)) {
		rtllib_start_scan_syncro(ieee, 0);
=======

	mutex_lock(&ieee->wx_mutex);
	if (!(ieee->softmac_features & IEEE_SOFTMAC_SCAN)) {
		rtllib_start_scan_syncro(ieee);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	chan = ieee->current_network.channel;

<<<<<<< HEAD
	if (ieee->LeisurePSLeave)
		ieee->LeisurePSLeave(ieee->dev);
=======
	ieee->leisure_ps_leave(ieee->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* notify AP to be in PS mode */
	rtllib_sta_ps_send_null_frame(ieee, 1);
	rtllib_sta_ps_send_null_frame(ieee, 1);

	rtllib_stop_all_queues(ieee);
<<<<<<< HEAD

	if (ieee->data_hard_stop)
		ieee->data_hard_stop(ieee->dev);
	rtllib_stop_send_beacons(ieee);
	ieee->state = RTLLIB_LINKED_SCANNING;
=======
	ieee->link_state = MAC80211_LINKED_SCANNING;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ieee->link_change(ieee->dev);
	/* wait for ps packet to be kicked out successfully */
	msleep(50);

<<<<<<< HEAD
	if (ieee->ScanOperationBackupHandler)
		ieee->ScanOperationBackupHandler(ieee->dev, SCAN_OPT_BACKUP);

	if (ieee->pHTInfo->bCurrentHTSupport && ieee->pHTInfo->bEnableHT &&
	    ieee->pHTInfo->bCurBW40MHz) {
		b40M = 1;
		chan_offset = ieee->pHTInfo->CurSTAExtChnlOffset;
		bandwidth = (enum ht_channel_width)ieee->pHTInfo->bCurBW40MHz;
		RT_TRACE(COMP_DBG, "Scan in 40M, force to 20M first:%d, %d\n",
			 chan_offset, bandwidth);
		ieee->SetBWModeHandler(ieee->dev, HT_CHANNEL_WIDTH_20,
				       HT_EXTCHNL_OFFSET_NO_EXT);
	}

	rtllib_start_scan_syncro(ieee, 0);

	if (b40M) {
		RT_TRACE(COMP_DBG, "Scan in 20M, back to 40M\n");
=======
	ieee->ScanOperationBackupHandler(ieee->dev, SCAN_OPT_BACKUP);

	if (ieee->ht_info->current_ht_support && ieee->ht_info->enable_ht &&
	    ieee->ht_info->cur_bw_40mhz) {
		b40M = 1;
		chan_offset = ieee->ht_info->CurSTAExtChnlOffset;
		bandwidth = (enum ht_channel_width)ieee->ht_info->cur_bw_40mhz;
		ieee->set_bw_mode_handler(ieee->dev, HT_CHANNEL_WIDTH_20,
				       HT_EXTCHNL_OFFSET_NO_EXT);
	}

	rtllib_start_scan_syncro(ieee);

	if (b40M) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (chan_offset == HT_EXTCHNL_OFFSET_UPPER)
			ieee->set_chan(ieee->dev, chan + 2);
		else if (chan_offset == HT_EXTCHNL_OFFSET_LOWER)
			ieee->set_chan(ieee->dev, chan - 2);
		else
			ieee->set_chan(ieee->dev, chan);
<<<<<<< HEAD
		ieee->SetBWModeHandler(ieee->dev, bandwidth, chan_offset);
=======
		ieee->set_bw_mode_handler(ieee->dev, bandwidth, chan_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		ieee->set_chan(ieee->dev, chan);
	}

<<<<<<< HEAD
	if (ieee->ScanOperationBackupHandler)
		ieee->ScanOperationBackupHandler(ieee->dev, SCAN_OPT_RESTORE);

	ieee->state = RTLLIB_LINKED;
=======
	ieee->ScanOperationBackupHandler(ieee->dev, SCAN_OPT_RESTORE);

	ieee->link_state = MAC80211_LINKED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ieee->link_change(ieee->dev);

	/* Notify AP that I wake up again */
	rtllib_sta_ps_send_null_frame(ieee, 0);

<<<<<<< HEAD
	if (ieee->LinkDetectInfo.NumRecvBcnInPeriod == 0 ||
	    ieee->LinkDetectInfo.NumRecvDataInPeriod == 0) {
		ieee->LinkDetectInfo.NumRecvBcnInPeriod = 1;
		ieee->LinkDetectInfo.NumRecvDataInPeriod = 1;
	}

	if (ieee->data_hard_resume)
		ieee->data_hard_resume(ieee->dev);

	if (ieee->iw_mode == IW_MODE_ADHOC || ieee->iw_mode == IW_MODE_MASTER)
		rtllib_start_send_beacons(ieee);

	rtllib_wake_all_queues(ieee);

	count = 0;
out:
	up(&ieee->wx_sem);

=======
	if (ieee->link_detect_info.num_recv_bcn_in_period == 0 ||
	    ieee->link_detect_info.num_recv_data_in_period == 0) {
		ieee->link_detect_info.num_recv_bcn_in_period = 1;
		ieee->link_detect_info.num_recv_data_in_period = 1;
	}
	rtllib_wake_all_queues(ieee);

out:
	mutex_unlock(&ieee->wx_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int rtllib_wx_set_scan(struct rtllib_device *ieee, struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	int ret = 0;

<<<<<<< HEAD
	down(&ieee->wx_sem);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ieee->iw_mode == IW_MODE_MONITOR || !(ieee->proto_started)) {
		ret = -1;
		goto out;
	}

<<<<<<< HEAD
	if (ieee->state == RTLLIB_LINKED) {
		queue_work_rsl(ieee->wq, &ieee->wx_sync_scan_wq);
=======
	if (ieee->link_state == MAC80211_LINKED) {
		schedule_work(&ieee->wx_sync_scan_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* intentionally forget to up sem */
		return 0;
	}

out:
<<<<<<< HEAD
	up(&ieee->wx_sem);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(rtllib_wx_set_scan);

int rtllib_wx_set_essid(struct rtllib_device *ieee,
			struct iw_request_info *a,
			union iwreq_data *wrqu, char *extra)
{
<<<<<<< HEAD

	int ret = 0, len, i;
=======
	int ret = 0, len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	short proto_started;
	unsigned long flags;

	rtllib_stop_scan_syncro(ieee);
<<<<<<< HEAD
	down(&ieee->wx_sem);

	proto_started = ieee->proto_started;

	len = (wrqu->essid.length < IW_ESSID_MAX_SIZE) ? wrqu->essid.length :
	       IW_ESSID_MAX_SIZE;

	if (len > IW_ESSID_MAX_SIZE) {
		ret = -E2BIG;
		goto out;
	}
=======
	mutex_lock(&ieee->wx_mutex);

	proto_started = ieee->proto_started;

	len = min_t(__u16, wrqu->essid.length, IW_ESSID_MAX_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ieee->iw_mode == IW_MODE_MONITOR) {
		ret = -1;
		goto out;
	}

<<<<<<< HEAD
	for (i = 0; i < len; i++) {
		if (extra[i] < 0) {
			ret = -1;
			goto out;
		}
	}

	if (proto_started)
		rtllib_stop_protocol(ieee, true);


	/* this is just to be sure that the GET wx callback
	 * has consisten infos. not needed otherwise
=======
	if (proto_started)
		rtllib_stop_protocol(ieee);

	/* this is just to be sure that the GET wx callback
	 * has consistent infos. not needed otherwise
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	spin_lock_irqsave(&ieee->lock, flags);

	if (wrqu->essid.flags && wrqu->essid.length) {
		strncpy(ieee->current_network.ssid, extra, len);
		ieee->current_network.ssid_len = len;
		ieee->cannot_notify = false;
		ieee->ssid_set = 1;
	} else {
		ieee->ssid_set = 0;
		ieee->current_network.ssid[0] = '\0';
		ieee->current_network.ssid_len = 0;
	}
	spin_unlock_irqrestore(&ieee->lock, flags);

	if (proto_started)
		rtllib_start_protocol(ieee);
out:
<<<<<<< HEAD
	up(&ieee->wx_sem);
=======
	mutex_unlock(&ieee->wx_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(rtllib_wx_set_essid);

int rtllib_wx_get_mode(struct rtllib_device *ieee, struct iw_request_info *a,
		       union iwreq_data *wrqu, char *b)
{
	wrqu->mode = ieee->iw_mode;
	return 0;
}
EXPORT_SYMBOL(rtllib_wx_get_mode);

<<<<<<< HEAD
int rtllib_wx_set_rawtx(struct rtllib_device *ieee,
			struct iw_request_info *info,
			union iwreq_data *wrqu, char *extra)
{

	int *parms = (int *)extra;
	int enable = (parms[0] > 0);
	short prev = ieee->raw_tx;

	down(&ieee->wx_sem);

	if (enable)
		ieee->raw_tx = 1;
	else
		ieee->raw_tx = 0;

	printk(KERN_INFO"raw TX is %s\n",
	      ieee->raw_tx ? "enabled" : "disabled");

	if (ieee->iw_mode == IW_MODE_MONITOR) {
		if (prev == 0 && ieee->raw_tx) {
			if (ieee->data_hard_resume)
				ieee->data_hard_resume(ieee->dev);

			netif_carrier_on(ieee->dev);
		}

		if (prev && ieee->raw_tx == 1)
			netif_carrier_off(ieee->dev);
	}

	up(&ieee->wx_sem);

	return 0;
}
EXPORT_SYMBOL(rtllib_wx_set_rawtx);

int rtllib_wx_get_name(struct rtllib_device *ieee,
			     struct iw_request_info *info,
			     union iwreq_data *wrqu, char *extra)
{
	strcpy(wrqu->name, "802.11");

	if (ieee->modulation & RTLLIB_CCK_MODULATION)
		strcat(wrqu->name, "b");
	if (ieee->modulation & RTLLIB_OFDM_MODULATION)
		strcat(wrqu->name, "g");
	if (ieee->mode & (IEEE_N_24G | IEEE_N_5G))
		strcat(wrqu->name, "n");
=======
int rtllib_wx_get_name(struct rtllib_device *ieee, struct iw_request_info *info,
		       union iwreq_data *wrqu, char *extra)
{
	const char *n = ieee->mode & (WIRELESS_MODE_N_24G) ? "n" : "";

	scnprintf(wrqu->name, sizeof(wrqu->name), "802.11bg%s", n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL(rtllib_wx_get_name);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* this is mostly stolen from hostap */
int rtllib_wx_set_power(struct rtllib_device *ieee,
				 struct iw_request_info *info,
				 union iwreq_data *wrqu, char *extra)
{
	int ret = 0;

	if ((!ieee->sta_wake_up) ||
	    (!ieee->enter_sleep_state) ||
	    (!ieee->ps_is_queue_empty)) {
<<<<<<< HEAD
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "%s(): PS mode is tryied to be use "
			     "but driver missed a callback\n\n", __func__);
		return -1;
	}

	down(&ieee->wx_sem);

	if (wrqu->power.disabled) {
		RT_TRACE(COMP_DBG, "===>%s(): power disable\n", __func__);
		ieee->ps = RTLLIB_PS_DISABLED;
		goto exit;
	}
	if (wrqu->power.flags & IW_POWER_TIMEOUT) {
		ieee->ps_timeout = wrqu->power.value / 1000;
		RT_TRACE(COMP_DBG, "===>%s():ps_timeout is %d\n", __func__,
			 ieee->ps_timeout);
	}
=======
		netdev_warn(ieee->dev,
			    "%s(): PS mode is tried to be use but driver missed a callback\n",
			    __func__);
		return -1;
	}

	mutex_lock(&ieee->wx_mutex);

	if (wrqu->power.disabled) {
		ieee->ps = RTLLIB_PS_DISABLED;
		goto exit;
	}
	if (wrqu->power.flags & IW_POWER_TIMEOUT)
		ieee->ps_timeout = wrqu->power.value / 1000;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (wrqu->power.flags & IW_POWER_PERIOD)
		ieee->ps_period = wrqu->power.value / 1000;

	switch (wrqu->power.flags & IW_POWER_MODE) {
	case IW_POWER_UNICAST_R:
		ieee->ps = RTLLIB_PS_UNICAST;
		break;
	case IW_POWER_MULTICAST_R:
		ieee->ps = RTLLIB_PS_MBCAST;
		break;
	case IW_POWER_ALL_R:
		ieee->ps = RTLLIB_PS_UNICAST | RTLLIB_PS_MBCAST;
		break;

	case IW_POWER_ON:
		break;

	default:
		ret = -EINVAL;
		goto exit;
<<<<<<< HEAD

	}
exit:
	up(&ieee->wx_sem);
	return ret;

=======
	}
exit:
	mutex_unlock(&ieee->wx_mutex);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(rtllib_wx_set_power);

/* this is stolen from hostap */
int rtllib_wx_get_power(struct rtllib_device *ieee,
				 struct iw_request_info *info,
				 union iwreq_data *wrqu, char *extra)
{
<<<<<<< HEAD
	int ret = 0;

	down(&ieee->wx_sem);
=======
	mutex_lock(&ieee->wx_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ieee->ps == RTLLIB_PS_DISABLED) {
		wrqu->power.disabled = 1;
		goto exit;
	}

	wrqu->power.disabled = 0;

	if ((wrqu->power.flags & IW_POWER_TYPE) == IW_POWER_TIMEOUT) {
		wrqu->power.flags = IW_POWER_TIMEOUT;
		wrqu->power.value = ieee->ps_timeout * 1000;
	} else {
		wrqu->power.flags = IW_POWER_PERIOD;
		wrqu->power.value = ieee->ps_period * 1000;
	}

	if ((ieee->ps & (RTLLIB_PS_MBCAST | RTLLIB_PS_UNICAST)) ==
	    (RTLLIB_PS_MBCAST | RTLLIB_PS_UNICAST))
		wrqu->power.flags |= IW_POWER_ALL_R;
	else if (ieee->ps & RTLLIB_PS_MBCAST)
		wrqu->power.flags |= IW_POWER_MULTICAST_R;
	else
		wrqu->power.flags |= IW_POWER_UNICAST_R;

exit:
<<<<<<< HEAD
	up(&ieee->wx_sem);
	return ret;

=======
	mutex_unlock(&ieee->wx_mutex);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(rtllib_wx_get_power);
