<<<<<<< HEAD
/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
******************************************************************************/
#include "rtllib.h"
#include "rtl819x_BA.h"

static void ActivateBAEntry(struct rtllib_device *ieee, struct ba_record *pBA,
			    u16 Time)
{
	pBA->bValid = true;
	if (Time != 0)
		mod_timer(&pBA->Timer, jiffies + MSECS(Time));
}

static void DeActivateBAEntry(struct rtllib_device *ieee, struct ba_record *pBA)
{
	pBA->bValid = false;
	del_timer_sync(&pBA->Timer);
}

static u8 TxTsDeleteBA(struct rtllib_device *ieee, struct tx_ts_record *pTxTs)
{
	struct ba_record *pAdmittedBa = &pTxTs->TxAdmittedBARecord;
	struct ba_record *pPendingBa = &pTxTs->TxPendingBARecord;
	u8 bSendDELBA = false;

	if (pPendingBa->bValid) {
		DeActivateBAEntry(ieee, pPendingBa);
		bSendDELBA = true;
	}

	if (pAdmittedBa->bValid) {
		DeActivateBAEntry(ieee, pAdmittedBa);
		bSendDELBA = true;
	}
	return bSendDELBA;
}

static u8 RxTsDeleteBA(struct rtllib_device *ieee, struct rx_ts_record *pRxTs)
{
	struct ba_record *pBa = &pRxTs->RxAdmittedBARecord;
	u8			bSendDELBA = false;

	if (pBa->bValid) {
		DeActivateBAEntry(ieee, pBa);
		bSendDELBA = true;
	}

	return bSendDELBA;
}

void ResetBaEntry(struct ba_record *pBA)
{
	pBA->bValid			= false;
	pBA->BaParamSet.shortData	= 0;
	pBA->BaTimeoutValue		= 0;
	pBA->DialogToken		= 0;
	pBA->BaStartSeqCtrl.ShortData	= 0;
}
static struct sk_buff *rtllib_ADDBA(struct rtllib_device *ieee, u8 *Dst,
				    struct ba_record *pBA,
				    u16 StatusCode, u8 type)
{
	struct sk_buff *skb = NULL;
	 struct rtllib_hdr_3addr *BAReq = NULL;
	u8 *tag = NULL;
	u16 tmp = 0;
	u16 len = ieee->tx_headroom + 9;

	RTLLIB_DEBUG(RTLLIB_DL_TRACE | RTLLIB_DL_BA, "========>%s(), frame(%d)"
		     " sentd to: %pM, ieee->dev:%p\n", __func__,
		     type, Dst, ieee->dev);
	if (pBA == NULL) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "pBA is NULL\n");
		return NULL;
	}
	skb = dev_alloc_skb(len + sizeof(struct rtllib_hdr_3addr));
	if (skb == NULL) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "can't alloc skb for ADDBA_REQ\n");
		return NULL;
	}

	memset(skb->data, 0, sizeof(struct rtllib_hdr_3addr));

	skb_reserve(skb, ieee->tx_headroom);

	BAReq = (struct rtllib_hdr_3addr *)skb_put(skb,
		 sizeof(struct rtllib_hdr_3addr));

	memcpy(BAReq->addr1, Dst, ETH_ALEN);
	memcpy(BAReq->addr2, ieee->dev->dev_addr, ETH_ALEN);

	memcpy(BAReq->addr3, ieee->current_network.bssid, ETH_ALEN);
	BAReq->frame_ctl = cpu_to_le16(RTLLIB_STYPE_MANAGE_ACT);

	tag = (u8 *)skb_put(skb, 9);
	*tag ++= ACT_CAT_BA;
	*tag ++= type;
	*tag ++= pBA->DialogToken;

	if (ACT_ADDBARSP == type) {
		RT_TRACE(COMP_DBG, "====>to send ADDBARSP\n");
		tmp = cpu_to_le16(StatusCode);
		memcpy(tag, (u8 *)&tmp, 2);
		tag += 2;
	}
	tmp = cpu_to_le16(pBA->BaParamSet.shortData);
	memcpy(tag, (u8 *)&tmp, 2);
	tag += 2;
	tmp = cpu_to_le16(pBA->BaTimeoutValue);
	memcpy(tag, (u8 *)&tmp, 2);
	tag += 2;

	if (ACT_ADDBAREQ == type) {
		memcpy(tag, (u8 *)&(pBA->BaStartSeqCtrl), 2);
		tag += 2;
	}

	RTLLIB_DEBUG_DATA(RTLLIB_DL_DATA|RTLLIB_DL_BA, skb->data, skb->len);
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include <linux/etherdevice.h>
#include "rtllib.h"
#include "rtl819x_BA.h"

static void activate_ba_entry(struct ba_record *ba, u16 time)
{
	ba->b_valid = true;
	if (time != 0)
		mod_timer(&ba->timer, jiffies + msecs_to_jiffies(time));
}

static void deactivate_ba_entry(struct rtllib_device *ieee, struct ba_record *ba)
{
	ba->b_valid = false;
	del_timer_sync(&ba->timer);
}

static u8 tx_ts_delete_ba(struct rtllib_device *ieee, struct tx_ts_record *ts)
{
	struct ba_record *admitted_ba = &ts->tx_admitted_ba_record;
	struct ba_record *pending_ba = &ts->tx_pending_ba_record;
	u8 send_del_ba = false;

	if (pending_ba->b_valid) {
		deactivate_ba_entry(ieee, pending_ba);
		send_del_ba = true;
	}

	if (admitted_ba->b_valid) {
		deactivate_ba_entry(ieee, admitted_ba);
		send_del_ba = true;
	}
	return send_del_ba;
}

static u8 rx_ts_delete_ba(struct rtllib_device *ieee, struct rx_ts_record *ts)
{
	struct ba_record *ba = &ts->rx_admitted_ba_record;
	u8			send_del_ba = false;

	if (ba->b_valid) {
		deactivate_ba_entry(ieee, ba);
		send_del_ba = true;
	}

	return send_del_ba;
}

void rtllib_reset_ba_entry(struct ba_record *ba)
{
	ba->b_valid                      = false;
	ba->ba_param_set.short_data      = 0;
	ba->ba_timeout_value             = 0;
	ba->dialog_token                 = 0;
	ba->ba_start_seq_ctrl.short_data = 0;
}

static struct sk_buff *rtllib_ADDBA(struct rtllib_device *ieee, u8 *dst,
				    struct ba_record *ba,
				    u16 status_code, u8 type)
{
	struct sk_buff *skb = NULL;
	struct ieee80211_hdr_3addr *ba_req = NULL;
	u8 *tag = NULL;
	u16 len = ieee->tx_headroom + 9;

	netdev_dbg(ieee->dev, "%s(): frame(%d) sentd to: %pM, ieee->dev:%p\n",
		   __func__, type, dst, ieee->dev);

	if (!ba) {
		netdev_warn(ieee->dev, "ba is NULL\n");
		return NULL;
	}
	skb = dev_alloc_skb(len + sizeof(struct ieee80211_hdr_3addr));
	if (!skb)
		return NULL;

	memset(skb->data, 0, sizeof(struct ieee80211_hdr_3addr));

	skb_reserve(skb, ieee->tx_headroom);

	ba_req = skb_put(skb, sizeof(struct ieee80211_hdr_3addr));

	ether_addr_copy(ba_req->addr1, dst);
	ether_addr_copy(ba_req->addr2, ieee->dev->dev_addr);

	ether_addr_copy(ba_req->addr3, ieee->current_network.bssid);
	ba_req->frame_control = cpu_to_le16(IEEE80211_STYPE_ACTION);

	tag = skb_put(skb, 9);
	*tag++ = ACT_CAT_BA;
	*tag++ = type;
	*tag++ = ba->dialog_token;

	if (type == ACT_ADDBARSP) {
		put_unaligned_le16(status_code, tag);
		tag += 2;
	}

	put_unaligned_le16(ba->ba_param_set.short_data, tag);
	tag += 2;

	put_unaligned_le16(ba->ba_timeout_value, tag);
	tag += 2;

	if (type == ACT_ADDBAREQ) {
		memcpy(tag, (u8 *)&ba->ba_start_seq_ctrl, 2);
		tag += 2;
	}

#ifdef VERBOSE_DEBUG
	print_hex_dump_bytes("%s: ", DUMP_PREFIX_NONE, skb->data,
			     __func__, skb->len);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return skb;
}

static struct sk_buff *rtllib_DELBA(struct rtllib_device *ieee, u8 *dst,
<<<<<<< HEAD
				    struct ba_record *pBA,
				    enum tr_select TxRxSelect, u16 ReasonCode)
{
	union delba_param_set DelbaParamSet;
	struct sk_buff *skb = NULL;
	 struct rtllib_hdr_3addr *Delba = NULL;
	u8 *tag = NULL;
	u16 tmp = 0;
	u16 len = 6 + ieee->tx_headroom;

	if (net_ratelimit())
		RTLLIB_DEBUG(RTLLIB_DL_TRACE | RTLLIB_DL_BA,
			     "========>%s(), Reason"
			     "Code(%d) sentd to: %pM\n", __func__,
			     ReasonCode, dst);

	memset(&DelbaParamSet, 0, 2);

	DelbaParamSet.field.Initiator = (TxRxSelect == TX_DIR) ? 1 : 0;
	DelbaParamSet.field.TID	= pBA->BaParamSet.field.TID;

	skb = dev_alloc_skb(len + sizeof(struct rtllib_hdr_3addr));
	if (skb == NULL) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "can't alloc skb for ADDBA_REQ\n");
		return NULL;
	}

	skb_reserve(skb, ieee->tx_headroom);

	Delba = (struct rtllib_hdr_3addr *) skb_put(skb,
		 sizeof(struct rtllib_hdr_3addr));

	memcpy(Delba->addr1, dst, ETH_ALEN);
	memcpy(Delba->addr2, ieee->dev->dev_addr, ETH_ALEN);
	memcpy(Delba->addr3, ieee->current_network.bssid, ETH_ALEN);
	Delba->frame_ctl = cpu_to_le16(RTLLIB_STYPE_MANAGE_ACT);

	tag = (u8 *)skb_put(skb, 6);

	*tag ++= ACT_CAT_BA;
	*tag ++= ACT_DELBA;

	tmp = cpu_to_le16(DelbaParamSet.shortData);
	memcpy(tag, (u8 *)&tmp, 2);
	tag += 2;
	tmp = cpu_to_le16(ReasonCode);
	memcpy(tag, (u8 *)&tmp, 2);
	tag += 2;

	RTLLIB_DEBUG_DATA(RTLLIB_DL_DATA|RTLLIB_DL_BA, skb->data, skb->len);
	if (net_ratelimit())
		RTLLIB_DEBUG(RTLLIB_DL_TRACE | RTLLIB_DL_BA, "<=====%s()\n",
			     __func__);
	return skb;
}

static void rtllib_send_ADDBAReq(struct rtllib_device *ieee, u8 *dst,
				 struct ba_record *pBA)
{
	struct sk_buff *skb = NULL;
	skb = rtllib_ADDBA(ieee, dst, pBA, 0, ACT_ADDBAREQ);

	if (skb) {
		RT_TRACE(COMP_DBG, "====>to send ADDBAREQ!!!!!\n");
		softmac_mgmt_xmit(skb, ieee);
	} else {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "alloc skb error in function"
			     " %s()\n", __func__);
	}
	return;
}

static void rtllib_send_ADDBARsp(struct rtllib_device *ieee, u8 *dst,
				 struct ba_record *pBA, u16 StatusCode)
{
	struct sk_buff *skb = NULL;
	skb = rtllib_ADDBA(ieee, dst, pBA, StatusCode, ACT_ADDBARSP);
	if (skb)
		softmac_mgmt_xmit(skb, ieee);
	else
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "alloc skb error in function"
			     " %s()\n", __func__);
	return;
}

static void rtllib_send_DELBA(struct rtllib_device *ieee, u8 *dst,
			      struct ba_record *pBA, enum tr_select TxRxSelect,
			      u16 ReasonCode)
{
	struct sk_buff *skb = NULL;
	skb = rtllib_DELBA(ieee, dst, pBA, TxRxSelect, ReasonCode);
	if (skb)
		softmac_mgmt_xmit(skb, ieee);
	else
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "alloc skb error in func"
			     "tion %s()\n", __func__);
	return ;
}

int rtllib_rx_ADDBAReq(struct rtllib_device *ieee, struct sk_buff *skb)
{
	struct rtllib_hdr_3addr *req = NULL;
	u16 rc = 0;
	u8 *dst = NULL, *pDialogToken = NULL, *tag = NULL;
	struct ba_record *pBA = NULL;
	union ba_param_set *pBaParamSet = NULL;
	u16 *pBaTimeoutVal = NULL;
	union sequence_control *pBaStartSeqCtrl = NULL;
	struct rx_ts_record *pTS = NULL;

	if (skb->len < sizeof(struct rtllib_hdr_3addr) + 9) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, " Invalid skb len in BAREQ(%d / "
			     "%d)\n", (int)skb->len,
			     (int)(sizeof(struct rtllib_hdr_3addr) + 9));
		return -1;
	}

	RTLLIB_DEBUG_DATA(RTLLIB_DL_DATA|RTLLIB_DL_BA, skb->data, skb->len);

	req = (struct rtllib_hdr_3addr *) skb->data;
	tag = (u8 *)req;
	dst = (u8 *)(&req->addr2[0]);
	tag += sizeof(struct rtllib_hdr_3addr);
	pDialogToken = tag + 2;
	pBaParamSet = (union ba_param_set *)(tag + 3);
	pBaTimeoutVal = (u16 *)(tag + 5);
	pBaStartSeqCtrl = (union sequence_control *)(req + 7);

	RT_TRACE(COMP_DBG, "====>rx ADDBAREQ from : %pM\n", dst);
	if (ieee->current_network.qos_data.active == 0  ||
	    (ieee->pHTInfo->bCurrentHTSupport == false) ||
	    (ieee->pHTInfo->IOTAction & HT_IOT_ACT_REJECT_ADDBA_REQ)) {
		rc = ADDBA_STATUS_REFUSED;
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "Failed to reply on ADDBA_REQ as "
			     "some capability is not ready(%d, %d)\n",
			     ieee->current_network.qos_data.active,
			     ieee->pHTInfo->bCurrentHTSupport);
		goto OnADDBAReq_Fail;
	}
	if (!GetTs(ieee, (struct ts_common_info **)(&pTS), dst,
	    (u8)(pBaParamSet->field.TID), RX_DIR, true)) {
		rc = ADDBA_STATUS_REFUSED;
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "can't get TS in %s()\n", __func__);
		goto OnADDBAReq_Fail;
	}
	pBA = &pTS->RxAdmittedBARecord;

	if (pBaParamSet->field.BAPolicy == BA_POLICY_DELAYED) {
		rc = ADDBA_STATUS_INVALID_PARAM;
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "BA Policy is not correct in "
			     "%s()\n", __func__);
		goto OnADDBAReq_Fail;
	}

	rtllib_FlushRxTsPendingPkts(ieee, pTS);

	DeActivateBAEntry(ieee, pBA);
	pBA->DialogToken = *pDialogToken;
	pBA->BaParamSet = *pBaParamSet;
	pBA->BaTimeoutValue = *pBaTimeoutVal;
	pBA->BaStartSeqCtrl = *pBaStartSeqCtrl;

	if (ieee->GetHalfNmodeSupportByAPsHandler(ieee->dev) ||
	   (ieee->pHTInfo->IOTAction & HT_IOT_ACT_ALLOW_PEER_AGG_ONE_PKT))
		pBA->BaParamSet.field.BufferSize = 1;
	else
		pBA->BaParamSet.field.BufferSize = 32;

	ActivateBAEntry(ieee, pBA, 0);
	rtllib_send_ADDBARsp(ieee, dst, pBA, ADDBA_STATUS_SUCCESS);

	return 0;

OnADDBAReq_Fail:
	{
		struct ba_record BA;
		BA.BaParamSet = *pBaParamSet;
		BA.BaTimeoutValue = *pBaTimeoutVal;
		BA.DialogToken = *pDialogToken;
		BA.BaParamSet.field.BAPolicy = BA_POLICY_IMMEDIATE;
		rtllib_send_ADDBARsp(ieee, dst, &BA, rc);
=======
				    struct ba_record *ba,
				    enum tr_select tx_rx_select, u16 reason_code)
{
	union delba_param_set del_ba_param_set;
	struct sk_buff *skb = NULL;
	struct ieee80211_hdr_3addr *del_ba = NULL;
	u8 *tag = NULL;
	u16 len = 6 + ieee->tx_headroom;

	if (net_ratelimit())
		netdev_dbg(ieee->dev, "%s(): reason_code(%d) sentd to: %pM\n",
			   __func__, reason_code, dst);

	memset(&del_ba_param_set, 0, 2);

	del_ba_param_set.field.initiator = (tx_rx_select == TX_DIR) ? 1 : 0;
	del_ba_param_set.field.tid	= ba->ba_param_set.field.tid;

	skb = dev_alloc_skb(len + sizeof(struct ieee80211_hdr_3addr));
	if (!skb)
		return NULL;

	skb_reserve(skb, ieee->tx_headroom);

	del_ba = skb_put(skb, sizeof(struct ieee80211_hdr_3addr));

	ether_addr_copy(del_ba->addr1, dst);
	ether_addr_copy(del_ba->addr2, ieee->dev->dev_addr);
	ether_addr_copy(del_ba->addr3, ieee->current_network.bssid);
	del_ba->frame_control = cpu_to_le16(IEEE80211_STYPE_ACTION);

	tag = skb_put(skb, 6);

	*tag++ = ACT_CAT_BA;
	*tag++ = ACT_DELBA;

	put_unaligned_le16(del_ba_param_set.short_data, tag);
	tag += 2;

	put_unaligned_le16(reason_code, tag);
	tag += 2;

#ifdef VERBOSE_DEBUG
	print_hex_dump_bytes("%s: ", DUMP_PREFIX_NONE, skb->data,
			     __func__, skb->len);
#endif
	return skb;
}

static void rtllib_send_add_ba_req(struct rtllib_device *ieee, u8 *dst,
				   struct ba_record *ba)
{
	struct sk_buff *skb;

	skb = rtllib_ADDBA(ieee, dst, ba, 0, ACT_ADDBAREQ);

	if (skb)
		softmac_mgmt_xmit(skb, ieee);
	else
		netdev_dbg(ieee->dev, "Failed to generate ADDBAReq packet.\n");
}

static void rtllib_send_add_ba_rsp(struct rtllib_device *ieee, u8 *dst,
				   struct ba_record *ba, u16 status_code)
{
	struct sk_buff *skb;

	skb = rtllib_ADDBA(ieee, dst, ba, status_code, ACT_ADDBARSP);
	if (skb)
		softmac_mgmt_xmit(skb, ieee);
	else
		netdev_dbg(ieee->dev, "Failed to generate ADDBARsp packet.\n");
}

static void rtllib_send_DELBA(struct rtllib_device *ieee, u8 *dst,
			      struct ba_record *ba, enum tr_select tx_rx_select,
			      u16 reason_code)
{
	struct sk_buff *skb;

	skb = rtllib_DELBA(ieee, dst, ba, tx_rx_select, reason_code);
	if (skb)
		softmac_mgmt_xmit(skb, ieee);
	else
		netdev_dbg(ieee->dev, "Failed to generate DELBA packet.\n");
}

int rtllib_rx_add_ba_req(struct rtllib_device *ieee, struct sk_buff *skb)
{
	struct ieee80211_hdr_3addr *req = NULL;
	u16 rc = 0;
	u8 *dst = NULL, *dialog_token = NULL, *tag = NULL;
	struct ba_record *ba = NULL;
	union ba_param_set *ba_param_set = NULL;
	u16 *ba_timeout_value = NULL;
	union sequence_control *ba_start_seq_ctrl = NULL;
	struct rx_ts_record *ts = NULL;

	if (skb->len < sizeof(struct ieee80211_hdr_3addr) + 9) {
		netdev_warn(ieee->dev, "Invalid skb len in BAREQ(%d / %d)\n",
			    (int)skb->len,
			    (int)(sizeof(struct ieee80211_hdr_3addr) + 9));
		return -1;
	}

#ifdef VERBOSE_DEBUG
	print_hex_dump_bytes("%s: ", DUMP_PREFIX_NONE, __func__,
			     skb->data, skb->len);
#endif

	req = (struct ieee80211_hdr_3addr *)skb->data;
	tag = (u8 *)req;
	dst = (u8 *)(&req->addr2[0]);
	tag += sizeof(struct ieee80211_hdr_3addr);
	dialog_token = tag + 2;
	ba_param_set = (union ba_param_set *)(tag + 3);
	ba_timeout_value = (u16 *)(tag + 5);
	ba_start_seq_ctrl = (union sequence_control *)(req + 7);

	if (!ieee->current_network.qos_data.active ||
	    !ieee->ht_info->current_ht_support ||
	    (ieee->ht_info->iot_action & HT_IOT_ACT_REJECT_ADDBA_REQ)) {
		rc = ADDBA_STATUS_REFUSED;
		netdev_warn(ieee->dev,
			    "Failed to reply on ADDBA_REQ as some capability is not ready(%d, %d)\n",
			    ieee->current_network.qos_data.active,
			    ieee->ht_info->current_ht_support);
		goto on_add_ba_req_fail;
	}
	if (!rtllib_get_ts(ieee, (struct ts_common_info **)&ts, dst,
			   (u8)(ba_param_set->field.tid), RX_DIR, true)) {
		rc = ADDBA_STATUS_REFUSED;
		netdev_warn(ieee->dev, "%s(): can't get TS\n", __func__);
		goto on_add_ba_req_fail;
	}
	ba = &ts->rx_admitted_ba_record;

	if (ba_param_set->field.ba_policy == BA_POLICY_DELAYED) {
		rc = ADDBA_STATUS_INVALID_PARAM;
		netdev_warn(ieee->dev, "%s(): BA Policy is not correct\n",
			    __func__);
		goto on_add_ba_req_fail;
	}

	rtllib_flush_rx_ts_pending_pkts(ieee, ts);

	deactivate_ba_entry(ieee, ba);
	ba->dialog_token = *dialog_token;
	ba->ba_param_set = *ba_param_set;
	ba->ba_timeout_value = *ba_timeout_value;
	ba->ba_start_seq_ctrl = *ba_start_seq_ctrl;

	if (ieee->get_half_nmode_support_by_aps_handler(ieee->dev) ||
	    (ieee->ht_info->iot_action & HT_IOT_ACT_ALLOW_PEER_AGG_ONE_PKT))
		ba->ba_param_set.field.buffer_size = 1;
	else
		ba->ba_param_set.field.buffer_size = 32;

	activate_ba_entry(ba, 0);
	rtllib_send_add_ba_rsp(ieee, dst, ba, ADDBA_STATUS_SUCCESS);

	return 0;

on_add_ba_req_fail:
	{
		struct ba_record BA;

		BA.ba_param_set = *ba_param_set;
		BA.ba_timeout_value = *ba_timeout_value;
		BA.dialog_token = *dialog_token;
		BA.ba_param_set.field.ba_policy = BA_POLICY_IMMEDIATE;
		rtllib_send_add_ba_rsp(ieee, dst, &BA, rc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
}

<<<<<<< HEAD
int rtllib_rx_ADDBARsp(struct rtllib_device *ieee, struct sk_buff *skb)
{
	 struct rtllib_hdr_3addr *rsp = NULL;
	struct ba_record *pPendingBA, *pAdmittedBA;
	struct tx_ts_record *pTS = NULL;
	u8 *dst = NULL, *pDialogToken = NULL, *tag = NULL;
	u16 *pStatusCode = NULL, *pBaTimeoutVal = NULL;
	union ba_param_set *pBaParamSet = NULL;
	u16			ReasonCode;

	if (skb->len < sizeof(struct rtllib_hdr_3addr) + 9) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, " Invalid skb len in BARSP(%d / "
			     "%d)\n", (int)skb->len,
			     (int)(sizeof(struct rtllib_hdr_3addr) + 9));
		return -1;
	}
	rsp = (struct rtllib_hdr_3addr *)skb->data;
	tag = (u8 *)rsp;
	dst = (u8 *)(&rsp->addr2[0]);
	tag += sizeof(struct rtllib_hdr_3addr);
	pDialogToken = tag + 2;
	pStatusCode = (u16 *)(tag + 3);
	pBaParamSet = (union ba_param_set *)(tag + 5);
	pBaTimeoutVal = (u16 *)(tag + 7);

	RT_TRACE(COMP_DBG, "====>rx ADDBARSP from : %pM\n", dst);
	if (ieee->current_network.qos_data.active == 0  ||
	    ieee->pHTInfo->bCurrentHTSupport == false ||
	    ieee->pHTInfo->bCurrentAMPDUEnable == false) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "reject to ADDBA_RSP as some capab"
			     "ility is not ready(%d, %d, %d)\n",
			     ieee->current_network.qos_data.active,
			     ieee->pHTInfo->bCurrentHTSupport,
			     ieee->pHTInfo->bCurrentAMPDUEnable);
		ReasonCode = DELBA_REASON_UNKNOWN_BA;
		goto OnADDBARsp_Reject;
	}


	if (!GetTs(ieee, (struct ts_common_info **)(&pTS), dst,
		   (u8)(pBaParamSet->field.TID), TX_DIR, false)) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "can't get TS in %s()\n", __func__);
		ReasonCode = DELBA_REASON_UNKNOWN_BA;
		goto OnADDBARsp_Reject;
	}

	pTS->bAddBaReqInProgress = false;
	pPendingBA = &pTS->TxPendingBARecord;
	pAdmittedBA = &pTS->TxAdmittedBARecord;


	if ((pAdmittedBA->bValid == true)) {
		RTLLIB_DEBUG(RTLLIB_DL_BA, "OnADDBARsp(): Recv ADDBA Rsp."
			     " Drop because already admit it!\n");
		return -1;
	} else if ((pPendingBA->bValid == false) ||
		   (*pDialogToken != pPendingBA->DialogToken)) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR,  "OnADDBARsp(): Recv ADDBA Rsp. "
			     "BA invalid, DELBA!\n");
		ReasonCode = DELBA_REASON_UNKNOWN_BA;
		goto OnADDBARsp_Reject;
	} else {
		RTLLIB_DEBUG(RTLLIB_DL_BA, "OnADDBARsp(): Recv ADDBA Rsp. BA "
			     "is admitted! Status code:%X\n", *pStatusCode);
		DeActivateBAEntry(ieee, pPendingBA);
	}


	if (*pStatusCode == ADDBA_STATUS_SUCCESS) {
		if (pBaParamSet->field.BAPolicy == BA_POLICY_DELAYED) {
			pTS->bAddBaReqDelayed = true;
			DeActivateBAEntry(ieee, pAdmittedBA);
			ReasonCode = DELBA_REASON_END_BA;
			goto OnADDBARsp_Reject;
		}


		pAdmittedBA->DialogToken = *pDialogToken;
		pAdmittedBA->BaTimeoutValue = *pBaTimeoutVal;
		pAdmittedBA->BaStartSeqCtrl = pPendingBA->BaStartSeqCtrl;
		pAdmittedBA->BaParamSet = *pBaParamSet;
		DeActivateBAEntry(ieee, pAdmittedBA);
		ActivateBAEntry(ieee, pAdmittedBA, *pBaTimeoutVal);
	} else {
		pTS->bAddBaReqDelayed = true;
		pTS->bDisable_AddBa = true;
		ReasonCode = DELBA_REASON_END_BA;
		goto OnADDBARsp_Reject;
=======
int rtllib_rx_add_ba_rsp(struct rtllib_device *ieee, struct sk_buff *skb)
{
	struct ieee80211_hdr_3addr *rsp = NULL;
	struct ba_record *pending_ba, *admitted_ba;
	struct tx_ts_record *ts = NULL;
	u8 *dst = NULL, *dialog_token = NULL, *tag = NULL;
	u16 *status_code = NULL, *ba_timeout_value = NULL;
	union ba_param_set *ba_param_set = NULL;
	u16			reason_code;

	if (skb->len < sizeof(struct ieee80211_hdr_3addr) + 9) {
		netdev_warn(ieee->dev, "Invalid skb len in BARSP(%d / %d)\n",
			    (int)skb->len,
			    (int)(sizeof(struct ieee80211_hdr_3addr) + 9));
		return -1;
	}
	rsp = (struct ieee80211_hdr_3addr *)skb->data;
	tag = (u8 *)rsp;
	dst = (u8 *)(&rsp->addr2[0]);
	tag += sizeof(struct ieee80211_hdr_3addr);
	dialog_token = tag + 2;
	status_code = (u16 *)(tag + 3);
	ba_param_set = (union ba_param_set *)(tag + 5);
	ba_timeout_value = (u16 *)(tag + 7);

	if (!ieee->current_network.qos_data.active ||
	    !ieee->ht_info->current_ht_support ||
	    !ieee->ht_info->current_ampdu_enable) {
		netdev_warn(ieee->dev,
			    "reject to ADDBA_RSP as some capability is not ready(%d, %d, %d)\n",
			    ieee->current_network.qos_data.active,
			    ieee->ht_info->current_ht_support,
			    ieee->ht_info->current_ampdu_enable);
		reason_code = DELBA_REASON_UNKNOWN_BA;
		goto on_add_ba_rsp_reject;
	}

	if (!rtllib_get_ts(ieee, (struct ts_common_info **)&ts, dst,
			   (u8)(ba_param_set->field.tid), TX_DIR, false)) {
		netdev_warn(ieee->dev, "%s(): can't get TS\n", __func__);
		reason_code = DELBA_REASON_UNKNOWN_BA;
		goto on_add_ba_rsp_reject;
	}

	ts->add_ba_req_in_progress = false;
	pending_ba = &ts->tx_pending_ba_record;
	admitted_ba = &ts->tx_admitted_ba_record;

	if (admitted_ba->b_valid) {
		netdev_dbg(ieee->dev, "%s(): ADDBA response already admitted\n",
			   __func__);
		return -1;
	} else if (!pending_ba->b_valid ||
		   (*dialog_token != pending_ba->dialog_token)) {
		netdev_warn(ieee->dev,
			    "%s(): ADDBA Rsp. BA invalid, DELBA!\n",
			    __func__);
		reason_code = DELBA_REASON_UNKNOWN_BA;
		goto on_add_ba_rsp_reject;
	} else {
		netdev_dbg(ieee->dev,
			   "%s(): Recv ADDBA Rsp. BA is admitted! Status code:%X\n",
			   __func__, *status_code);
		deactivate_ba_entry(ieee, pending_ba);
	}

	if (*status_code == ADDBA_STATUS_SUCCESS) {
		if (ba_param_set->field.ba_policy == BA_POLICY_DELAYED) {
			ts->add_ba_req_delayed = true;
			deactivate_ba_entry(ieee, admitted_ba);
			reason_code = DELBA_REASON_END_BA;
			goto on_add_ba_rsp_reject;
		}

		admitted_ba->dialog_token = *dialog_token;
		admitted_ba->ba_timeout_value = *ba_timeout_value;
		admitted_ba->ba_start_seq_ctrl = pending_ba->ba_start_seq_ctrl;
		admitted_ba->ba_param_set = *ba_param_set;
		deactivate_ba_entry(ieee, admitted_ba);
		activate_ba_entry(admitted_ba, *ba_timeout_value);
	} else {
		ts->add_ba_req_delayed = true;
		ts->disable_add_ba = true;
		reason_code = DELBA_REASON_END_BA;
		goto on_add_ba_rsp_reject;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

<<<<<<< HEAD
OnADDBARsp_Reject:
	{
		struct ba_record BA;
		BA.BaParamSet = *pBaParamSet;
		rtllib_send_DELBA(ieee, dst, &BA, TX_DIR, ReasonCode);
=======
on_add_ba_rsp_reject:
	{
		struct ba_record BA;

		BA.ba_param_set = *ba_param_set;
		rtllib_send_DELBA(ieee, dst, &BA, TX_DIR, reason_code);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
}

int rtllib_rx_DELBA(struct rtllib_device *ieee, struct sk_buff *skb)
{
<<<<<<< HEAD
	 struct rtllib_hdr_3addr *delba = NULL;
	union delba_param_set *pDelBaParamSet = NULL;
	u16 *pReasonCode = NULL;
	u8 *dst = NULL;

	if (skb->len < sizeof(struct rtllib_hdr_3addr) + 6) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, " Invalid skb len in DELBA(%d /"
			     " %d)\n", (int)skb->len,
			     (int)(sizeof(struct rtllib_hdr_3addr) + 6));
		return -1;
	}

	if (ieee->current_network.qos_data.active == 0  ||
		ieee->pHTInfo->bCurrentHTSupport == false) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "received DELBA while QOS or HT "
			     "is not supported(%d, %d)\n",
			     ieee->current_network. qos_data.active,
			     ieee->pHTInfo->bCurrentHTSupport);
		return -1;
	}

	RTLLIB_DEBUG_DATA(RTLLIB_DL_DATA|RTLLIB_DL_BA, skb->data, skb->len);
	delba = (struct rtllib_hdr_3addr *)skb->data;
	dst = (u8 *)(&delba->addr2[0]);
	delba += sizeof(struct rtllib_hdr_3addr);
	pDelBaParamSet = (union delba_param_set *)(delba+2);
	pReasonCode = (u16 *)(delba+4);

	if (pDelBaParamSet->field.Initiator == 1) {
		struct rx_ts_record *pRxTs;

		if (!GetTs(ieee, (struct ts_common_info **)&pRxTs, dst,
		    (u8)pDelBaParamSet->field.TID, RX_DIR, false)) {
			RTLLIB_DEBUG(RTLLIB_DL_ERR,  "can't get TS for RXTS in "
				     "%s().dst: %pM TID:%d\n", __func__, dst,
				     (u8)pDelBaParamSet->field.TID);
			return -1;
		}

		RxTsDeleteBA(ieee, pRxTs);
	} else {
		struct tx_ts_record *pTxTs;

		if (!GetTs(ieee, (struct ts_common_info **)&pTxTs, dst,
			   (u8)pDelBaParamSet->field.TID, TX_DIR, false)) {
			RTLLIB_DEBUG(RTLLIB_DL_ERR,  "can't get TS for TXTS in "
				     "%s()\n", __func__);
			return -1;
		}

		pTxTs->bUsingBa = false;
		pTxTs->bAddBaReqInProgress = false;
		pTxTs->bAddBaReqDelayed = false;
		del_timer_sync(&pTxTs->TsAddBaTimer);
		TxTsDeleteBA(ieee, pTxTs);
=======
	struct ieee80211_hdr_3addr *delba = NULL;
	union delba_param_set *del_ba_param_set = NULL;
	u8 *dst = NULL;

	if (skb->len < sizeof(struct ieee80211_hdr_3addr) + 6) {
		netdev_warn(ieee->dev, "Invalid skb len in DELBA(%d / %d)\n",
			    (int)skb->len,
			    (int)(sizeof(struct ieee80211_hdr_3addr) + 6));
		return -1;
	}

	if (!ieee->current_network.qos_data.active ||
	    !ieee->ht_info->current_ht_support) {
		netdev_warn(ieee->dev,
			    "received DELBA while QOS or HT is not supported(%d, %d)\n",
			    ieee->current_network. qos_data.active,
			    ieee->ht_info->current_ht_support);
		return -1;
	}

#ifdef VERBOSE_DEBUG
	print_hex_dump_bytes("%s: ", DUMP_PREFIX_NONE, skb->data,
			     __func__, skb->len);
#endif
	delba = (struct ieee80211_hdr_3addr *)skb->data;
	dst = (u8 *)(&delba->addr2[0]);
	del_ba_param_set = (union delba_param_set *)&delba->seq_ctrl + 2;

	if (del_ba_param_set->field.initiator == 1) {
		struct rx_ts_record *ts;

		if (!rtllib_get_ts(ieee, (struct ts_common_info **)&ts, dst,
				   (u8)del_ba_param_set->field.tid, RX_DIR, false)) {
			netdev_warn(ieee->dev,
				    "%s(): can't get TS for RXTS. dst:%pM TID:%d\n",
				    __func__, dst,
				    (u8)del_ba_param_set->field.tid);
			return -1;
		}

		rx_ts_delete_ba(ieee, ts);
	} else {
		struct tx_ts_record *ts;

		if (!rtllib_get_ts(ieee, (struct ts_common_info **)&ts, dst,
				   (u8)del_ba_param_set->field.tid, TX_DIR, false)) {
			netdev_warn(ieee->dev, "%s(): can't get TS for TXTS\n",
				    __func__);
			return -1;
		}

		ts->using_ba = false;
		ts->add_ba_req_in_progress = false;
		ts->add_ba_req_delayed = false;
		del_timer_sync(&ts->ts_add_ba_timer);
		tx_ts_delete_ba(ieee, ts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
void TsInitAddBA(struct rtllib_device *ieee, struct tx_ts_record *pTS,
		 u8 Policy, u8	bOverwritePending)
{
	struct ba_record *pBA = &pTS->TxPendingBARecord;

	if (pBA->bValid == true && bOverwritePending == false)
		return;

	DeActivateBAEntry(ieee, pBA);

	pBA->DialogToken++;
	pBA->BaParamSet.field.AMSDU_Support = 0;
	pBA->BaParamSet.field.BAPolicy = Policy;
	pBA->BaParamSet.field.TID =
			 pTS->TsCommonInfo.TSpec.f.TSInfo.field.ucTSID;
	pBA->BaParamSet.field.BufferSize = 32;
	pBA->BaTimeoutValue = 0;
	pBA->BaStartSeqCtrl.field.SeqNum = (pTS->TxCurSeq + 3) % 4096;

	ActivateBAEntry(ieee, pBA, BA_SETUP_TIMEOUT);

	rtllib_send_ADDBAReq(ieee, pTS->TsCommonInfo.Addr, pBA);
}

void TsInitDelBA(struct rtllib_device *ieee,
		 struct ts_common_info *pTsCommonInfo,
		 enum tr_select TxRxSelect)
{
	if (TxRxSelect == TX_DIR) {
		struct tx_ts_record *pTxTs =
			 (struct tx_ts_record *)pTsCommonInfo;

		if (TxTsDeleteBA(ieee, pTxTs))
			rtllib_send_DELBA(ieee, pTsCommonInfo->Addr,
					  (pTxTs->TxAdmittedBARecord.bValid) ?
					 (&pTxTs->TxAdmittedBARecord) :
					(&pTxTs->TxPendingBARecord),
					 TxRxSelect, DELBA_REASON_END_BA);
	} else if (TxRxSelect == RX_DIR) {
		struct rx_ts_record *pRxTs =
				 (struct rx_ts_record *)pTsCommonInfo;
		if (RxTsDeleteBA(ieee, pRxTs))
			rtllib_send_DELBA(ieee, pTsCommonInfo->Addr,
					  &pRxTs->RxAdmittedBARecord,
					  TxRxSelect, DELBA_REASON_END_BA);
	}
}

void BaSetupTimeOut(unsigned long data)
{
	struct tx_ts_record *pTxTs = (struct tx_ts_record *)data;

	pTxTs->bAddBaReqInProgress = false;
	pTxTs->bAddBaReqDelayed = true;
	pTxTs->TxPendingBARecord.bValid = false;
}

void TxBaInactTimeout(unsigned long data)
{
	struct tx_ts_record *pTxTs = (struct tx_ts_record *)data;
	struct rtllib_device *ieee = container_of(pTxTs, struct rtllib_device,
				     TxTsRecord[pTxTs->num]);
	TxTsDeleteBA(ieee, pTxTs);
	rtllib_send_DELBA(ieee, pTxTs->TsCommonInfo.Addr,
			  &pTxTs->TxAdmittedBARecord, TX_DIR,
			  DELBA_REASON_TIMEOUT);
}

void RxBaInactTimeout(unsigned long data)
{
	struct rx_ts_record *pRxTs = (struct rx_ts_record *)data;
	struct rtllib_device *ieee = container_of(pRxTs, struct rtllib_device,
				     RxTsRecord[pRxTs->num]);

	RxTsDeleteBA(ieee, pRxTs);
	rtllib_send_DELBA(ieee, pRxTs->TsCommonInfo.Addr,
			  &pRxTs->RxAdmittedBARecord, RX_DIR,
			  DELBA_REASON_TIMEOUT);
	return ;
=======
void rtllib_ts_init_add_ba(struct rtllib_device *ieee, struct tx_ts_record *ts,
			   u8 policy, u8	overwrite_pending)
{
	struct ba_record *ba = &ts->tx_pending_ba_record;

	if (ba->b_valid && !overwrite_pending)
		return;

	deactivate_ba_entry(ieee, ba);

	ba->dialog_token++;
	ba->ba_param_set.field.amsdu_support = 0;
	ba->ba_param_set.field.ba_policy = policy;
	ba->ba_param_set.field.tid = ts->ts_common_info.tspec.ts_id;
	ba->ba_param_set.field.buffer_size = 32;
	ba->ba_timeout_value = 0;
	ba->ba_start_seq_ctrl.field.seq_num = (ts->tx_cur_seq + 3) % 4096;

	activate_ba_entry(ba, BA_SETUP_TIMEOUT);

	rtllib_send_add_ba_req(ieee, ts->ts_common_info.addr, ba);
}

void rtllib_ts_init_del_ba(struct rtllib_device *ieee,
			   struct ts_common_info *ts_common_info,
			   enum tr_select tx_rx_select)
{
	if (tx_rx_select == TX_DIR) {
		struct tx_ts_record *ts =
			 (struct tx_ts_record *)ts_common_info;

		if (tx_ts_delete_ba(ieee, ts))
			rtllib_send_DELBA(ieee, ts_common_info->addr,
					  (ts->tx_admitted_ba_record.b_valid) ?
					 (&ts->tx_admitted_ba_record) :
					(&ts->tx_pending_ba_record),
					 tx_rx_select, DELBA_REASON_END_BA);
	} else if (tx_rx_select == RX_DIR) {
		struct rx_ts_record *ts =
				 (struct rx_ts_record *)ts_common_info;
		if (rx_ts_delete_ba(ieee, ts))
			rtllib_send_DELBA(ieee, ts_common_info->addr,
					  &ts->rx_admitted_ba_record,
					  tx_rx_select, DELBA_REASON_END_BA);
	}
}

void rtllib_ba_setup_timeout(struct timer_list *t)
{
	struct tx_ts_record *ts = from_timer(ts, t,
					      tx_pending_ba_record.timer);

	ts->add_ba_req_in_progress = false;
	ts->add_ba_req_delayed = true;
	ts->tx_pending_ba_record.b_valid = false;
}

void rtllib_tx_ba_inact_timeout(struct timer_list *t)
{
	struct tx_ts_record *ts = from_timer(ts, t,
					      tx_admitted_ba_record.timer);
	struct rtllib_device *ieee = container_of(ts, struct rtllib_device,
				     tx_ts_records[ts->num]);
	tx_ts_delete_ba(ieee, ts);
	rtllib_send_DELBA(ieee, ts->ts_common_info.addr,
			  &ts->tx_admitted_ba_record, TX_DIR,
			  DELBA_REASON_TIMEOUT);
}

void rtllib_rx_ba_inact_timeout(struct timer_list *t)
{
	struct rx_ts_record *ts = from_timer(ts, t,
					      rx_admitted_ba_record.timer);
	struct rtllib_device *ieee = container_of(ts, struct rtllib_device,
				     rx_ts_records[ts->num]);

	rx_ts_delete_ba(ieee, ts);
	rtllib_send_DELBA(ieee, ts->ts_common_info.addr,
			  &ts->rx_admitted_ba_record, RX_DIR,
			  DELBA_REASON_TIMEOUT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
