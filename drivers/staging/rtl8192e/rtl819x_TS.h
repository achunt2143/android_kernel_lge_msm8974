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
#ifndef _TSTYPE_H_
#define _TSTYPE_H_
#include "rtl819x_Qos.h"
#define TS_SETUP_TIMEOUT	60
#define TS_INACT_TIMEOUT	60
#define TS_ADDBA_DELAY		60

#define TOTAL_TS_NUM		16
#define TCLAS_NUM		4
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
#ifndef _TSTYPE_H_
#define _TSTYPE_H_
#include "rtl819x_Qos.h"
#define TS_ADDBA_DELAY		60

#define TOTAL_TS_NUM		16
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum tr_select {
	TX_DIR = 0,
	RX_DIR = 1,
};

struct ts_common_info {
<<<<<<< HEAD
	struct list_head		List;
	struct timer_list		SetupTimer;
	struct timer_list		InactTimer;
	u8				Addr[6];
	union tspec_body TSpec;
	union qos_tclas TClass[TCLAS_NUM];
	u8				TClasProc;
	u8				TClasNum;
};

struct tx_ts_record {
	struct ts_common_info TsCommonInfo;
	u16				TxCurSeq;
	struct ba_record TxPendingBARecord;
	struct ba_record TxAdmittedBARecord;
	u8				bAddBaReqInProgress;
	u8				bAddBaReqDelayed;
	u8				bUsingBa;
	u8				bDisable_AddBa;
	struct timer_list		TsAddBaTimer;
=======
	struct list_head		list;
	u8				addr[ETH_ALEN];
	struct qos_tsinfo tspec;
};

struct tx_ts_record {
	struct ts_common_info ts_common_info;
	u16				tx_cur_seq;
	struct ba_record tx_pending_ba_record;
	struct ba_record tx_admitted_ba_record;
	u8				add_ba_req_in_progress;
	u8				add_ba_req_delayed;
	u8				using_ba;
	u8				disable_add_ba;
	struct timer_list		ts_add_ba_timer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8				num;
};

struct rx_ts_record {
<<<<<<< HEAD
	struct ts_common_info TsCommonInfo;
	u16				RxIndicateSeq;
	u16				RxTimeoutIndicateSeq;
	struct list_head		RxPendingPktList;
	struct timer_list		RxPktPendingTimer;
	struct ba_record RxAdmittedBARecord;
	u16				RxLastSeqNum;
	u8				RxLastFragNum;
	u8				num;
};

void _setup_timer(struct timer_list *, void *, unsigned long);


=======
	struct ts_common_info ts_common_info;
	u16 rx_indicate_seq;
	u16 rx_timeout_indicate_seq;
	struct list_head rx_pending_pkt_list;
	struct timer_list rx_pkt_pending_timer;
	struct ba_record rx_admitted_ba_record;
	u16 rx_last_seq_num;
	u8 rx_last_frag_num;
	u8 num;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
