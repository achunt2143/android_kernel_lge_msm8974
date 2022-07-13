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
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "rtllib.h"
#include <linux/etherdevice.h>
#include "rtl819x_TS.h"

<<<<<<< HEAD
static void TsSetupTimeOut(unsigned long data)
{
}

static void TsInactTimeout(unsigned long data)
{
}

static void RxPktPendingTimeout(unsigned long data)
{
	struct rx_ts_record *pRxTs = (struct rx_ts_record *)data;
	struct rtllib_device *ieee = container_of(pRxTs, struct rtllib_device,
						  RxTsRecord[pRxTs->num]);
=======
static void RxPktPendingTimeout(struct timer_list *t)
{
	struct rx_ts_record *ts = from_timer(ts, t, rx_pkt_pending_timer);
	struct rtllib_device *ieee = container_of(ts, struct rtllib_device,
						  rx_ts_records[ts->num]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct rx_reorder_entry *pReorderEntry = NULL;

	unsigned long flags = 0;
	u8 index = 0;
	bool bPktInBuf = false;

	spin_lock_irqsave(&(ieee->reorder_spinlock), flags);
<<<<<<< HEAD
	if (pRxTs->RxTimeoutIndicateSeq != 0xffff) {
		while (!list_empty(&pRxTs->RxPendingPktList)) {
			pReorderEntry = (struct rx_reorder_entry *)
					list_entry(pRxTs->RxPendingPktList.prev,
					struct rx_reorder_entry, List);
			if (index == 0)
				pRxTs->RxIndicateSeq = pReorderEntry->SeqNum;

			if (SN_LESS(pReorderEntry->SeqNum, pRxTs->RxIndicateSeq) ||
				SN_EQUAL(pReorderEntry->SeqNum, pRxTs->RxIndicateSeq)) {
				list_del_init(&pReorderEntry->List);

				if (SN_EQUAL(pReorderEntry->SeqNum,
				    pRxTs->RxIndicateSeq))
					pRxTs->RxIndicateSeq =
					      (pRxTs->RxIndicateSeq + 1) % 4096;

				RTLLIB_DEBUG(RTLLIB_DL_REORDER, "%s(): Indicate"
					     " SeqNum: %d\n", __func__,
					     pReorderEntry->SeqNum);
=======
	if (ts->rx_timeout_indicate_seq != 0xffff) {
		while (!list_empty(&ts->rx_pending_pkt_list)) {
			pReorderEntry = (struct rx_reorder_entry *)
					list_entry(ts->rx_pending_pkt_list.prev,
					struct rx_reorder_entry, list);
			if (index == 0)
				ts->rx_indicate_seq = pReorderEntry->SeqNum;

			if (SN_LESS(pReorderEntry->SeqNum,
				    ts->rx_indicate_seq) ||
			    SN_EQUAL(pReorderEntry->SeqNum,
				     ts->rx_indicate_seq)) {
				list_del_init(&pReorderEntry->list);

				if (SN_EQUAL(pReorderEntry->SeqNum,
				    ts->rx_indicate_seq))
					ts->rx_indicate_seq =
					      (ts->rx_indicate_seq + 1) % 4096;

				netdev_dbg(ieee->dev,
					   "%s(): Indicate SeqNum: %d\n",
					   __func__, pReorderEntry->SeqNum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				ieee->stats_IndicateArray[index] =
							 pReorderEntry->prxb;
				index++;

<<<<<<< HEAD
				list_add_tail(&pReorderEntry->List,
=======
				list_add_tail(&pReorderEntry->list,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					      &ieee->RxReorder_Unused_List);
			} else {
				bPktInBuf = true;
				break;
			}
		}
	}

	if (index > 0) {
<<<<<<< HEAD
		pRxTs->RxTimeoutIndicateSeq = 0xffff;

		if (index > REORDER_WIN_SIZE) {
			RTLLIB_DEBUG(RTLLIB_DL_ERR, "RxReorderIndicatePacket():"
				     " Rx Reorer struct buffer full!!\n");
=======
		ts->rx_timeout_indicate_seq = 0xffff;

		if (index > REORDER_WIN_SIZE) {
			netdev_warn(ieee->dev,
				    "%s(): Rx Reorder struct buffer full\n",
				    __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			spin_unlock_irqrestore(&(ieee->reorder_spinlock),
					       flags);
			return;
		}
		rtllib_indicate_packets(ieee, ieee->stats_IndicateArray, index);
		bPktInBuf = false;
	}

<<<<<<< HEAD
	if (bPktInBuf && (pRxTs->RxTimeoutIndicateSeq == 0xffff)) {
		pRxTs->RxTimeoutIndicateSeq = pRxTs->RxIndicateSeq;
		mod_timer(&pRxTs->RxPktPendingTimer,  jiffies +
			  MSECS(ieee->pHTInfo->RxReorderPendingTime));
=======
	if (bPktInBuf && (ts->rx_timeout_indicate_seq == 0xffff)) {
		ts->rx_timeout_indicate_seq = ts->rx_indicate_seq;
		mod_timer(&ts->rx_pkt_pending_timer,  jiffies +
			  msecs_to_jiffies(ieee->ht_info->rx_reorder_pending_time)
			  );
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	spin_unlock_irqrestore(&(ieee->reorder_spinlock), flags);
}

<<<<<<< HEAD
static void TsAddBaProcess(unsigned long data)
{
	struct tx_ts_record *pTxTs = (struct tx_ts_record *)data;
	u8 num = pTxTs->num;
	struct rtllib_device *ieee = container_of(pTxTs, struct rtllib_device,
				     TxTsRecord[num]);

	TsInitAddBA(ieee, pTxTs, BA_POLICY_IMMEDIATE, false);
	RTLLIB_DEBUG(RTLLIB_DL_BA, "TsAddBaProcess(): ADDBA Req is "
		     "started!!\n");
}

static void ResetTsCommonInfo(struct ts_common_info *pTsCommonInfo)
{
	memset(pTsCommonInfo->Addr, 0, 6);
	memset(&pTsCommonInfo->TSpec, 0, sizeof(union tspec_body));
	memset(&pTsCommonInfo->TClass, 0, sizeof(union qos_tclas)*TCLAS_NUM);
	pTsCommonInfo->TClasProc = 0;
	pTsCommonInfo->TClasNum = 0;
}

static void ResetTxTsEntry(struct tx_ts_record *pTS)
{
	ResetTsCommonInfo(&pTS->TsCommonInfo);
	pTS->TxCurSeq = 0;
	pTS->bAddBaReqInProgress = false;
	pTS->bAddBaReqDelayed = false;
	pTS->bUsingBa = false;
	pTS->bDisable_AddBa = false;
	ResetBaEntry(&pTS->TxAdmittedBARecord);
	ResetBaEntry(&pTS->TxPendingBARecord);
}

static void ResetRxTsEntry(struct rx_ts_record *pTS)
{
	ResetTsCommonInfo(&pTS->TsCommonInfo);
	pTS->RxIndicateSeq = 0xffff;
	pTS->RxTimeoutIndicateSeq = 0xffff;
	ResetBaEntry(&pTS->RxAdmittedBARecord);
}

void TSInitialize(struct rtllib_device *ieee)
{
	struct tx_ts_record *pTxTS  = ieee->TxTsRecord;
	struct rx_ts_record *pRxTS  = ieee->RxTsRecord;
	struct rx_reorder_entry *pRxReorderEntry = ieee->RxReorderEntry;
	u8				count = 0;
	RTLLIB_DEBUG(RTLLIB_DL_TS, "==========>%s()\n", __func__);
=======
static void TsAddBaProcess(struct timer_list *t)
{
	struct tx_ts_record *ts = from_timer(ts, t, ts_add_ba_timer);
	u8 num = ts->num;
	struct rtllib_device *ieee = container_of(ts, struct rtllib_device,
				     tx_ts_records[num]);

	rtllib_ts_init_add_ba(ieee, ts, BA_POLICY_IMMEDIATE, false);
	netdev_dbg(ieee->dev, "%s(): ADDBA Req is started\n", __func__);
}

static void ResetTsCommonInfo(struct ts_common_info *ts_common_info)
{
	eth_zero_addr(ts_common_info->addr);
	memset(&ts_common_info->tspec, 0, sizeof(struct qos_tsinfo));
}

static void ResetTxTsEntry(struct tx_ts_record *ts)
{
	ResetTsCommonInfo(&ts->ts_common_info);
	ts->tx_cur_seq = 0;
	ts->add_ba_req_in_progress = false;
	ts->add_ba_req_delayed = false;
	ts->using_ba = false;
	ts->disable_add_ba = false;
	rtllib_reset_ba_entry(&ts->tx_admitted_ba_record);
	rtllib_reset_ba_entry(&ts->tx_pending_ba_record);
}

static void ResetRxTsEntry(struct rx_ts_record *ts)
{
	ResetTsCommonInfo(&ts->ts_common_info);
	ts->rx_indicate_seq = 0xffff;
	ts->rx_timeout_indicate_seq = 0xffff;
	rtllib_reset_ba_entry(&ts->rx_admitted_ba_record);
}

void rtllib_ts_init(struct rtllib_device *ieee)
{
	struct tx_ts_record *pTxTS  = ieee->tx_ts_records;
	struct rx_ts_record *rxts  = ieee->rx_ts_records;
	struct rx_reorder_entry *pRxReorderEntry = ieee->RxReorderEntry;
	u8				count = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_LIST_HEAD(&ieee->Tx_TS_Admit_List);
	INIT_LIST_HEAD(&ieee->Tx_TS_Pending_List);
	INIT_LIST_HEAD(&ieee->Tx_TS_Unused_List);

	for (count = 0; count < TOTAL_TS_NUM; count++) {
		pTxTS->num = count;
<<<<<<< HEAD
		_setup_timer(&pTxTS->TsCommonInfo.SetupTimer,
			    TsSetupTimeOut,
			    (unsigned long) pTxTS);

		_setup_timer(&pTxTS->TsCommonInfo.InactTimer,
			    TsInactTimeout,
			    (unsigned long) pTxTS);

		_setup_timer(&pTxTS->TsAddBaTimer,
			    TsAddBaProcess,
			    (unsigned long) pTxTS);

		_setup_timer(&pTxTS->TxPendingBARecord.Timer,
			    BaSetupTimeOut,
			    (unsigned long) pTxTS);
		_setup_timer(&pTxTS->TxAdmittedBARecord.Timer,
			    TxBaInactTimeout,
			    (unsigned long) pTxTS);

		ResetTxTsEntry(pTxTS);
		list_add_tail(&pTxTS->TsCommonInfo.List,
=======
		timer_setup(&pTxTS->ts_add_ba_timer, TsAddBaProcess, 0);

		timer_setup(&pTxTS->tx_pending_ba_record.timer, rtllib_ba_setup_timeout,
			    0);
		timer_setup(&pTxTS->tx_admitted_ba_record.timer,
			    rtllib_tx_ba_inact_timeout, 0);

		ResetTxTsEntry(pTxTS);
		list_add_tail(&pTxTS->ts_common_info.list,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				&ieee->Tx_TS_Unused_List);
		pTxTS++;
	}

	INIT_LIST_HEAD(&ieee->Rx_TS_Admit_List);
	INIT_LIST_HEAD(&ieee->Rx_TS_Pending_List);
	INIT_LIST_HEAD(&ieee->Rx_TS_Unused_List);
	for (count = 0; count < TOTAL_TS_NUM; count++) {
<<<<<<< HEAD
		pRxTS->num = count;
		INIT_LIST_HEAD(&pRxTS->RxPendingPktList);

		_setup_timer(&pRxTS->TsCommonInfo.SetupTimer,
			    TsSetupTimeOut,
			    (unsigned long) pRxTS);

		_setup_timer(&pRxTS->TsCommonInfo.InactTimer,
			    TsInactTimeout,
			    (unsigned long) pRxTS);

		_setup_timer(&pRxTS->RxAdmittedBARecord.Timer,
			    RxBaInactTimeout,
			    (unsigned long) pRxTS);

		_setup_timer(&pRxTS->RxPktPendingTimer,
			    RxPktPendingTimeout,
			    (unsigned long) pRxTS);

		ResetRxTsEntry(pRxTS);
		list_add_tail(&pRxTS->TsCommonInfo.List,
			      &ieee->Rx_TS_Unused_List);
		pRxTS++;
	}
	INIT_LIST_HEAD(&ieee->RxReorder_Unused_List);
	for (count = 0; count < REORDER_ENTRY_NUM; count++) {
		list_add_tail(&pRxReorderEntry->List,
			      &ieee->RxReorder_Unused_List);
		if (count == (REORDER_ENTRY_NUM-1))
			break;
		pRxReorderEntry = &ieee->RxReorderEntry[count+1];
	}

}

static void AdmitTS(struct rtllib_device *ieee,
		    struct ts_common_info *pTsCommonInfo, u32 InactTime)
{
	del_timer_sync(&pTsCommonInfo->SetupTimer);
	del_timer_sync(&pTsCommonInfo->InactTimer);

	if (InactTime != 0)
		mod_timer(&pTsCommonInfo->InactTimer, jiffies +
			  MSECS(InactTime));
}

static struct ts_common_info *SearchAdmitTRStream(struct rtllib_device *ieee,
						  u8 *Addr, u8 TID,
						  enum tr_select TxRxSelect)
=======
		rxts->num = count;
		INIT_LIST_HEAD(&rxts->rx_pending_pkt_list);
		timer_setup(&rxts->rx_admitted_ba_record.timer,
			    rtllib_rx_ba_inact_timeout, 0);

		timer_setup(&rxts->rx_pkt_pending_timer, RxPktPendingTimeout, 0);

		ResetRxTsEntry(rxts);
		list_add_tail(&rxts->ts_common_info.list,
			      &ieee->Rx_TS_Unused_List);
		rxts++;
	}
	INIT_LIST_HEAD(&ieee->RxReorder_Unused_List);
	for (count = 0; count < REORDER_ENTRY_NUM; count++) {
		list_add_tail(&pRxReorderEntry->list,
			      &ieee->RxReorder_Unused_List);
		if (count == (REORDER_ENTRY_NUM - 1))
			break;
		pRxReorderEntry = &ieee->RxReorderEntry[count + 1];
	}
}

static struct ts_common_info *SearchAdmitTRStream(struct rtllib_device *ieee,
						  u8 *addr, u8 TID,
						  enum tr_select tx_rx_select)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u8	dir;
	bool	search_dir[4] = {0};
	struct list_head *psearch_list;
	struct ts_common_info *pRet = NULL;
<<<<<<< HEAD
	if (ieee->iw_mode == IW_MODE_MASTER) {
		if (TxRxSelect == TX_DIR) {
			search_dir[DIR_DOWN] = true;
			search_dir[DIR_BI_DIR] = true;
		} else {
			search_dir[DIR_UP] = true;
			search_dir[DIR_BI_DIR] = true;
		}
	} else if (ieee->iw_mode == IW_MODE_ADHOC) {
		if (TxRxSelect == TX_DIR)
			search_dir[DIR_UP] = true;
		else
			search_dir[DIR_DOWN] = true;
	} else {
		if (TxRxSelect == TX_DIR) {
			search_dir[DIR_UP] = true;
			search_dir[DIR_BI_DIR] = true;
			search_dir[DIR_DIRECT] = true;
		} else {
			search_dir[DIR_DOWN] = true;
			search_dir[DIR_BI_DIR] = true;
			search_dir[DIR_DIRECT] = true;
		}
	}

	if (TxRxSelect == TX_DIR)
=======

	if (tx_rx_select == TX_DIR) {
		search_dir[DIR_UP] = true;
		search_dir[DIR_BI_DIR] = true;
		search_dir[DIR_DIRECT] = true;
	} else {
		search_dir[DIR_DOWN] = true;
		search_dir[DIR_BI_DIR] = true;
		search_dir[DIR_DIRECT] = true;
	}

	if (tx_rx_select == TX_DIR)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		psearch_list = &ieee->Tx_TS_Admit_List;
	else
		psearch_list = &ieee->Rx_TS_Admit_List;

	for (dir = 0; dir <= DIR_BI_DIR; dir++) {
<<<<<<< HEAD
		if (search_dir[dir] == false)
			continue;
		list_for_each_entry(pRet, psearch_list, List) {
			if (memcmp(pRet->Addr, Addr, 6) == 0)
				if (pRet->TSpec.f.TSInfo.field.ucTSID == TID)
					if (pRet->TSpec.f.TSInfo.field.ucDirection == dir)
						break;

		}
		if (&pRet->List  != psearch_list)
			break;
	}

	if (pRet && &pRet->List  != psearch_list)
		return pRet ;
	else
		return NULL;
}

static void MakeTSEntry(struct ts_common_info *pTsCommonInfo, u8 *Addr,
			union tspec_body *pTSPEC, union qos_tclas *pTCLAS,
			u8 TCLAS_Num, u8 TCLAS_Proc)
{
	u8	count;

	if (pTsCommonInfo == NULL)
		return;

	memcpy(pTsCommonInfo->Addr, Addr, 6);

	if (pTSPEC != NULL)
		memcpy((u8 *)(&(pTsCommonInfo->TSpec)), (u8 *)pTSPEC,
			sizeof(union tspec_body));

	for (count = 0; count < TCLAS_Num; count++)
		memcpy((u8 *)(&(pTsCommonInfo->TClass[count])),
		       (u8 *)pTCLAS, sizeof(union qos_tclas));

	pTsCommonInfo->TClasProc = TCLAS_Proc;
	pTsCommonInfo->TClasNum = TCLAS_Num;
}

bool GetTs(struct rtllib_device *ieee, struct ts_common_info **ppTS,
	   u8 *Addr, u8 TID, enum tr_select TxRxSelect, bool bAddNewTs)
{
	u8	UP = 0;
	if (is_broadcast_ether_addr(Addr) || is_multicast_ether_addr(Addr)) {
		RTLLIB_DEBUG(RTLLIB_DL_ERR, "ERR! get TS for Broadcast or "
			     "Multicast\n");
=======
		if (!search_dir[dir])
			continue;
		list_for_each_entry(pRet, psearch_list, list) {
			if (memcmp(pRet->addr, addr, 6) == 0 &&
			    pRet->tspec.ts_id == TID &&
			    pRet->tspec.ucDirection == dir)
				break;
		}
		if (&pRet->list  != psearch_list)
			break;
	}

	if (pRet && &pRet->list  != psearch_list)
		return pRet;
	return NULL;
}

static void MakeTSEntry(struct ts_common_info *ts_common_info, u8 *addr,
			struct qos_tsinfo *pTSPEC)
{
	if (!ts_common_info)
		return;

	memcpy(ts_common_info->addr, addr, 6);

	if (pTSPEC)
		memcpy((u8 *)(&(ts_common_info->tspec)), (u8 *)pTSPEC,
			sizeof(struct qos_tsinfo));
}

bool rtllib_get_ts(struct rtllib_device *ieee, struct ts_common_info **ppTS,
	   u8 *addr, u8 TID, enum tr_select tx_rx_select, bool bAddNewTs)
{
	u8	UP = 0;
	struct qos_tsinfo tspec;
	struct qos_tsinfo *ts_info = &tspec;
	struct list_head *pUnusedList;
	struct list_head *pAddmitList;
	enum direction_value Dir;

	if (is_multicast_ether_addr(addr)) {
		netdev_warn(ieee->dev, "Get TS for Broadcast or Multicast\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return false;
	}
	if (ieee->current_network.qos_data.supported == 0) {
		UP = 0;
	} else {
<<<<<<< HEAD
		if (!IsACValid(TID)) {
			RTLLIB_DEBUG(RTLLIB_DL_ERR, "ERR! in %s(), TID(%d) is "
				     "not valid\n", __func__, TID);
			return false;
		}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		switch (TID) {
		case 0:
		case 3:
			UP = 0;
			break;
		case 1:
		case 2:
			UP = 2;
			break;
		case 4:
		case 5:
			UP = 5;
			break;
		case 6:
		case 7:
			UP = 7;
			break;
<<<<<<< HEAD
		}
	}

	*ppTS = SearchAdmitTRStream(ieee, Addr, UP, TxRxSelect);
	if (*ppTS != NULL) {
		return true;
	} else {
		if (bAddNewTs == false) {
			RTLLIB_DEBUG(RTLLIB_DL_TS, "add new TS failed"
				     "(tid:%d)\n", UP);
			return false;
		} else {
			union tspec_body TSpec;
			union qos_tsinfo *pTSInfo = &TSpec.f.TSInfo;
			struct list_head *pUnusedList =
				(TxRxSelect == TX_DIR) ?
				(&ieee->Tx_TS_Unused_List) :
				(&ieee->Rx_TS_Unused_List);

			struct list_head *pAddmitList =
				(TxRxSelect == TX_DIR) ?
				(&ieee->Tx_TS_Admit_List) :
				(&ieee->Rx_TS_Admit_List);

			enum direction_value Dir =
				 (ieee->iw_mode == IW_MODE_MASTER) ?
				 ((TxRxSelect == TX_DIR) ? DIR_DOWN : DIR_UP) :
				 ((TxRxSelect == TX_DIR) ? DIR_UP : DIR_DOWN);
			RTLLIB_DEBUG(RTLLIB_DL_TS, "to add Ts\n");
			if (!list_empty(pUnusedList)) {
				(*ppTS) = list_entry(pUnusedList->next,
					  struct ts_common_info, List);
				list_del_init(&(*ppTS)->List);
				if (TxRxSelect == TX_DIR) {
					struct tx_ts_record *tmp =
						container_of(*ppTS,
						struct tx_ts_record,
						TsCommonInfo);
					ResetTxTsEntry(tmp);
				} else {
					struct rx_ts_record *tmp =
						 container_of(*ppTS,
						 struct rx_ts_record,
						 TsCommonInfo);
					ResetRxTsEntry(tmp);
				}

				RTLLIB_DEBUG(RTLLIB_DL_TS, "to init current TS"
					     ", UP:%d, Dir:%d, addr: %pM"
					     " ppTs=%p\n", UP, Dir,
					      Addr, *ppTS);
				pTSInfo->field.ucTrafficType = 0;
				pTSInfo->field.ucTSID = UP;
				pTSInfo->field.ucDirection = Dir;
				pTSInfo->field.ucAccessPolicy = 1;
				pTSInfo->field.ucAggregation = 0;
				pTSInfo->field.ucPSB = 0;
				pTSInfo->field.ucUP = UP;
				pTSInfo->field.ucTSInfoAckPolicy = 0;
				pTSInfo->field.ucSchedule = 0;

				MakeTSEntry(*ppTS, Addr, &TSpec, NULL, 0, 0);
				AdmitTS(ieee, *ppTS, 0);
				list_add_tail(&((*ppTS)->List), pAddmitList);

				return true;
			} else {
				RTLLIB_DEBUG(RTLLIB_DL_ERR, "ERR!!in function "
					     "%s() There is not enough dir=%d"
					     "(0=up down=1) TS record to be "
					     "used!!", __func__, Dir);
				return false;
			}
		}
	}
}

static void RemoveTsEntry(struct rtllib_device *ieee, struct ts_common_info *pTs,
			  enum tr_select TxRxSelect)
{
	del_timer_sync(&pTs->SetupTimer);
	del_timer_sync(&pTs->InactTimer);
	TsInitDelBA(ieee, pTs, TxRxSelect);

	if (TxRxSelect == RX_DIR) {
		struct rx_reorder_entry *pRxReorderEntry;
		struct rx_ts_record *pRxTS = (struct rx_ts_record *)pTs;

		if (timer_pending(&pRxTS->RxPktPendingTimer))
			del_timer_sync(&pRxTS->RxPktPendingTimer);

		while (!list_empty(&pRxTS->RxPendingPktList)) {
			pRxReorderEntry = (struct rx_reorder_entry *)
					list_entry(pRxTS->RxPendingPktList.prev,
					struct rx_reorder_entry, List);
			RTLLIB_DEBUG(RTLLIB_DL_REORDER, "%s(): Delete SeqNum "
				     "%d!\n", __func__,
				     pRxReorderEntry->SeqNum);
			list_del_init(&pRxReorderEntry->List);
			{
				int i = 0;
				struct rtllib_rxb *prxb = pRxReorderEntry->prxb;
=======
		default:
			netdev_warn(ieee->dev, "%s(): TID(%d) is not valid\n",
				    __func__, TID);
			return false;
		}
	}

	*ppTS = SearchAdmitTRStream(ieee, addr, UP, tx_rx_select);
	if (*ppTS)
		return true;

	if (!bAddNewTs) {
		netdev_dbg(ieee->dev, "add new TS failed(tid:%d)\n", UP);
		return false;
	}

	pUnusedList = (tx_rx_select == TX_DIR) ?
				(&ieee->Tx_TS_Unused_List) :
				(&ieee->Rx_TS_Unused_List);

	pAddmitList = (tx_rx_select == TX_DIR) ?
				(&ieee->Tx_TS_Admit_List) :
				(&ieee->Rx_TS_Admit_List);

	Dir = ((tx_rx_select == TX_DIR) ? DIR_UP : DIR_DOWN);

	if (!list_empty(pUnusedList)) {
		(*ppTS) = list_entry(pUnusedList->next,
			  struct ts_common_info, list);
		list_del_init(&(*ppTS)->list);
		if (tx_rx_select == TX_DIR) {
			struct tx_ts_record *tmp =
				container_of(*ppTS,
				struct tx_ts_record,
				ts_common_info);
			ResetTxTsEntry(tmp);
		} else {
			struct rx_ts_record *ts =
				 container_of(*ppTS,
				 struct rx_ts_record,
				 ts_common_info);
			ResetRxTsEntry(ts);
		}

		netdev_dbg(ieee->dev,
			   "to init current TS, UP:%d, Dir:%d, addr: %pM ppTs=%p\n",
			   UP, Dir, addr, *ppTS);
		ts_info->ts_id = UP;
		ts_info->ucDirection = Dir;

		MakeTSEntry(*ppTS, addr, &tspec);
		list_add_tail(&((*ppTS)->list), pAddmitList);

		return true;
	}

	netdev_warn(ieee->dev,
		    "There is not enough dir=%d(0=up down=1) TS record to be used!",
		    Dir);
	return false;
}

static void RemoveTsEntry(struct rtllib_device *ieee,
			  struct ts_common_info *pTs, enum tr_select tx_rx_select)
{
	rtllib_ts_init_del_ba(ieee, pTs, tx_rx_select);

	if (tx_rx_select == RX_DIR) {
		struct rx_reorder_entry *pRxReorderEntry;
		struct rx_ts_record *ts = (struct rx_ts_record *)pTs;

		if (timer_pending(&ts->rx_pkt_pending_timer))
			del_timer_sync(&ts->rx_pkt_pending_timer);

		while (!list_empty(&ts->rx_pending_pkt_list)) {
			pRxReorderEntry = (struct rx_reorder_entry *)
					list_entry(ts->rx_pending_pkt_list.prev,
					struct rx_reorder_entry, list);
			netdev_dbg(ieee->dev,  "%s(): Delete SeqNum %d!\n",
				   __func__, pRxReorderEntry->SeqNum);
			list_del_init(&pRxReorderEntry->list);
			{
				int i = 0;
				struct rtllib_rxb *prxb = pRxReorderEntry->prxb;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (unlikely(!prxb))
					return;
				for (i = 0; i < prxb->nr_subframes; i++)
					dev_kfree_skb(prxb->subframes[i]);
				kfree(prxb);
				prxb = NULL;
			}
<<<<<<< HEAD
			list_add_tail(&pRxReorderEntry->List,
=======
			list_add_tail(&pRxReorderEntry->list,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				      &ieee->RxReorder_Unused_List);
		}
	} else {
		struct tx_ts_record *pTxTS = (struct tx_ts_record *)pTs;
<<<<<<< HEAD
		del_timer_sync(&pTxTS->TsAddBaTimer);
	}
}

void RemovePeerTS(struct rtllib_device *ieee, u8 *Addr)
{
	struct ts_common_info *pTS, *pTmpTS;
	printk(KERN_INFO "===========>RemovePeerTS, %pM\n", Addr);

	list_for_each_entry_safe(pTS, pTmpTS, &ieee->Tx_TS_Pending_List, List) {
		if (memcmp(pTS->Addr, Addr, 6) == 0) {
			RemoveTsEntry(ieee, pTS, TX_DIR);
			list_del_init(&pTS->List);
			list_add_tail(&pTS->List, &ieee->Tx_TS_Unused_List);
		}
	}

	list_for_each_entry_safe(pTS, pTmpTS, &ieee->Tx_TS_Admit_List, List) {
		if (memcmp(pTS->Addr, Addr, 6) == 0) {
			printk(KERN_INFO "====>remove Tx_TS_admin_list\n");
			RemoveTsEntry(ieee, pTS, TX_DIR);
			list_del_init(&pTS->List);
			list_add_tail(&pTS->List, &ieee->Tx_TS_Unused_List);
		}
	}

	list_for_each_entry_safe(pTS, pTmpTS, &ieee->Rx_TS_Pending_List, List) {
		if (memcmp(pTS->Addr, Addr, 6) == 0) {
			RemoveTsEntry(ieee, pTS, RX_DIR);
			list_del_init(&pTS->List);
			list_add_tail(&pTS->List, &ieee->Rx_TS_Unused_List);
		}
	}

	list_for_each_entry_safe(pTS, pTmpTS, &ieee->Rx_TS_Admit_List, List) {
		if (memcmp(pTS->Addr, Addr, 6) == 0) {
			RemoveTsEntry(ieee, pTS, RX_DIR);
			list_del_init(&pTS->List);
			list_add_tail(&pTS->List, &ieee->Rx_TS_Unused_List);
		}
	}
}
EXPORT_SYMBOL(RemovePeerTS);

void RemoveAllTS(struct rtllib_device *ieee)
{
	struct ts_common_info *pTS, *pTmpTS;

	list_for_each_entry_safe(pTS, pTmpTS, &ieee->Tx_TS_Pending_List, List) {
		RemoveTsEntry(ieee, pTS, TX_DIR);
		list_del_init(&pTS->List);
		list_add_tail(&pTS->List, &ieee->Tx_TS_Unused_List);
	}

	list_for_each_entry_safe(pTS, pTmpTS, &ieee->Tx_TS_Admit_List, List) {
		RemoveTsEntry(ieee, pTS, TX_DIR);
		list_del_init(&pTS->List);
		list_add_tail(&pTS->List, &ieee->Tx_TS_Unused_List);
	}

	list_for_each_entry_safe(pTS, pTmpTS, &ieee->Rx_TS_Pending_List, List) {
		RemoveTsEntry(ieee, pTS, RX_DIR);
		list_del_init(&pTS->List);
		list_add_tail(&pTS->List, &ieee->Rx_TS_Unused_List);
	}

	list_for_each_entry_safe(pTS, pTmpTS, &ieee->Rx_TS_Admit_List, List) {
		RemoveTsEntry(ieee, pTS, RX_DIR);
		list_del_init(&pTS->List);
		list_add_tail(&pTS->List, &ieee->Rx_TS_Unused_List);
=======

		del_timer_sync(&pTxTS->ts_add_ba_timer);
	}
}

void remove_peer_ts(struct rtllib_device *ieee, u8 *addr)
{
	struct ts_common_info *ts, *pTmpTS;

	netdev_info(ieee->dev, "===========>%s, %pM\n", __func__, addr);

	list_for_each_entry_safe(ts, pTmpTS, &ieee->Tx_TS_Pending_List, list) {
		if (memcmp(ts->addr, addr, 6) == 0) {
			RemoveTsEntry(ieee, ts, TX_DIR);
			list_del_init(&ts->list);
			list_add_tail(&ts->list, &ieee->Tx_TS_Unused_List);
		}
	}

	list_for_each_entry_safe(ts, pTmpTS, &ieee->Tx_TS_Admit_List, list) {
		if (memcmp(ts->addr, addr, 6) == 0) {
			netdev_info(ieee->dev,
				    "====>remove Tx_TS_admin_list\n");
			RemoveTsEntry(ieee, ts, TX_DIR);
			list_del_init(&ts->list);
			list_add_tail(&ts->list, &ieee->Tx_TS_Unused_List);
		}
	}

	list_for_each_entry_safe(ts, pTmpTS, &ieee->Rx_TS_Pending_List, list) {
		if (memcmp(ts->addr, addr, 6) == 0) {
			RemoveTsEntry(ieee, ts, RX_DIR);
			list_del_init(&ts->list);
			list_add_tail(&ts->list, &ieee->Rx_TS_Unused_List);
		}
	}

	list_for_each_entry_safe(ts, pTmpTS, &ieee->Rx_TS_Admit_List, list) {
		if (memcmp(ts->addr, addr, 6) == 0) {
			RemoveTsEntry(ieee, ts, RX_DIR);
			list_del_init(&ts->list);
			list_add_tail(&ts->list, &ieee->Rx_TS_Unused_List);
		}
	}
}
EXPORT_SYMBOL(remove_peer_ts);

void remove_all_ts(struct rtllib_device *ieee)
{
	struct ts_common_info *ts, *pTmpTS;

	list_for_each_entry_safe(ts, pTmpTS, &ieee->Tx_TS_Pending_List, list) {
		RemoveTsEntry(ieee, ts, TX_DIR);
		list_del_init(&ts->list);
		list_add_tail(&ts->list, &ieee->Tx_TS_Unused_List);
	}

	list_for_each_entry_safe(ts, pTmpTS, &ieee->Tx_TS_Admit_List, list) {
		RemoveTsEntry(ieee, ts, TX_DIR);
		list_del_init(&ts->list);
		list_add_tail(&ts->list, &ieee->Tx_TS_Unused_List);
	}

	list_for_each_entry_safe(ts, pTmpTS, &ieee->Rx_TS_Pending_List, list) {
		RemoveTsEntry(ieee, ts, RX_DIR);
		list_del_init(&ts->list);
		list_add_tail(&ts->list, &ieee->Rx_TS_Unused_List);
	}

	list_for_each_entry_safe(ts, pTmpTS, &ieee->Rx_TS_Admit_List, list) {
		RemoveTsEntry(ieee, ts, RX_DIR);
		list_del_init(&ts->list);
		list_add_tail(&ts->list, &ieee->Rx_TS_Unused_List);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

void TsStartAddBaProcess(struct rtllib_device *ieee, struct tx_ts_record *pTxTS)
{
<<<<<<< HEAD
	if (pTxTS->bAddBaReqInProgress == false) {
		pTxTS->bAddBaReqInProgress = true;

		if (pTxTS->bAddBaReqDelayed) {
			RTLLIB_DEBUG(RTLLIB_DL_BA, "TsStartAddBaProcess(): "
				     "Delayed Start ADDBA after 60 sec!!\n");
			mod_timer(&pTxTS->TsAddBaTimer, jiffies +
				  MSECS(TS_ADDBA_DELAY));
		} else {
			RTLLIB_DEBUG(RTLLIB_DL_BA, "TsStartAddBaProcess(): "
				     "Immediately Start ADDBA now!!\n");
			mod_timer(&pTxTS->TsAddBaTimer, jiffies+10);
		}
	} else
		RTLLIB_DEBUG(RTLLIB_DL_BA, "%s()==>BA timer is already added\n",
			     __func__);
=======
	if (pTxTS->add_ba_req_in_progress == false) {
		pTxTS->add_ba_req_in_progress = true;

		if (pTxTS->add_ba_req_delayed) {
			netdev_dbg(ieee->dev, "Start ADDBA after 60 sec!!\n");
			mod_timer(&pTxTS->ts_add_ba_timer, jiffies +
				  msecs_to_jiffies(TS_ADDBA_DELAY));
		} else {
			netdev_dbg(ieee->dev, "Immediately Start ADDBA\n");
			mod_timer(&pTxTS->ts_add_ba_timer, jiffies + 10);
		}
	} else {
		netdev_dbg(ieee->dev, "BA timer is already added\n");
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
