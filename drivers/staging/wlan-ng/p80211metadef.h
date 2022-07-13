<<<<<<< HEAD
/* This file is GENERATED AUTOMATICALLY.  DO NOT EDIT OR MODIFY.
* --------------------------------------------------------------------
*
* Copyright (C) 1999 AbsoluteValue Systems, Inc.  All Rights Reserved.
* --------------------------------------------------------------------
*
* linux-wlan
*
*   The contents of this file are subject to the Mozilla Public
*   License Version 1.1 (the "License"); you may not use this file
*   except in compliance with the License. You may obtain a copy of
*   the License at http://www.mozilla.org/MPL/
*
*   Software distributed under the License is distributed on an "AS
*   IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*   implied. See the License for the specific language governing
*   rights and limitations under the License.
*
*   Alternatively, the contents of this file may be used under the
*   terms of the GNU Public License version 2 (the "GPL"), in which
*   case the provisions of the GPL are applicable instead of the
*   above.  If you wish to allow the use of your version of this file
*   only under the terms of the GPL and not to allow others to use
*   your version of this file under the MPL, indicate your decision
*   by deleting the provisions above and replace them with the notice
*   and other provisions required by the GPL.  If you do not delete
*   the provisions above, a recipient may use your version of this
*   file under either the MPL or the GPL.
*
* --------------------------------------------------------------------
*
* Inquiries regarding the linux-wlan Open Source project can be
* made directly to:
*
* AbsoluteValue Systems Inc.
* info@linux-wlan.com
* http://www.linux-wlan.com
*
* --------------------------------------------------------------------
*
* Portions of the development of this software were funded by
* Intersil Corporation as part of PRISM(R) chipset product development.
*
* --------------------------------------------------------------------
*/
=======
/* SPDX-License-Identifier: (GPL-2.0 OR MPL-1.1) */
/* --------------------------------------------------------------------
 *
 * Copyright (C) 1999 AbsoluteValue Systems, Inc.  All Rights Reserved.
 * --------------------------------------------------------------------
 *
 * linux-wlan
 *
 * --------------------------------------------------------------------
 *
 * Inquiries regarding the linux-wlan Open Source project can be
 * made directly to:
 *
 * AbsoluteValue Systems Inc.
 * info@linux-wlan.com
 * http://www.linux-wlan.com
 *
 * --------------------------------------------------------------------
 *
 * Portions of the development of this software were funded by
 * Intersil Corporation as part of PRISM(R) chipset product development.
 *
 * --------------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef _P80211MKMETADEF_H
#define _P80211MKMETADEF_H

<<<<<<< HEAD
#define DIDmsg_dot11req_mibget \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(1))
#define DIDmsg_dot11req_mibget_mibattribute \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDmsg_dot11req_mibget_resultcode \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDmsg_dot11req_mibset \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(2))
#define DIDmsg_dot11req_mibset_mibattribute \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDmsg_dot11req_mibset_resultcode \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDmsg_dot11req_scan \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(4))
#define DIDmsg_dot11req_scan_results \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(5))
#define DIDmsg_dot11req_start \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(13))
#define DIDmsg_dot11ind_authenticate \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1))
#define DIDmsg_dot11ind_associate \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(3))
#define DIDmsg_lnxreq_ifstate \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(1))
#define DIDmsg_lnxreq_wlansniff \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(2))
#define DIDmsg_lnxreq_hostwep \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(3))
#define DIDmsg_lnxreq_commsquality \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(4))
#define DIDmsg_lnxreq_autojoin \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(5))
#define DIDmsg_p2req_readpda \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2))
#define DIDmsg_p2req_readpda_pda \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDmsg_p2req_readpda_resultcode \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDmsg_p2req_ramdl_state \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(11))
#define DIDmsg_p2req_ramdl_state_enable \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(11) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDmsg_p2req_ramdl_state_exeaddr \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(11) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDmsg_p2req_ramdl_state_resultcode \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(11) | \
			P80211DID_MKITEM(3) | 0x00000000)
#define DIDmsg_p2req_ramdl_write \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12))
#define DIDmsg_p2req_ramdl_write_addr \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDmsg_p2req_ramdl_write_len \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDmsg_p2req_ramdl_write_data \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12) | \
			P80211DID_MKITEM(3) | 0x00000000)
#define DIDmsg_p2req_ramdl_write_resultcode \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12) | \
			P80211DID_MKITEM(4) | 0x00000000)
#define DIDmsg_p2req_flashdl_state \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(13))
#define DIDmsg_p2req_flashdl_write \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(14))
#define DIDmib_cat_dot11smt \
			P80211DID_MKSECTION(1)
#define DIDmib_dot11smt_dot11WEPDefaultKeysTable \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(4))
#define DIDmib_dot11smt_dot11WEPDefaultKeysTable_dot11WEPDefaultKey0 \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(4) | \
			P80211DID_MKITEM(1) | 0x0c000000)
#define DIDmib_dot11smt_dot11WEPDefaultKeysTable_dot11WEPDefaultKey1 \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(4) | \
			P80211DID_MKITEM(2) | 0x0c000000)
#define DIDmib_dot11smt_dot11WEPDefaultKeysTable_dot11WEPDefaultKey2 \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(4) | \
			P80211DID_MKITEM(3) | 0x0c000000)
#define DIDmib_dot11smt_dot11WEPDefaultKeysTable_dot11WEPDefaultKey3 \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(4) | \
			P80211DID_MKITEM(4) | 0x0c000000)
#define DIDmib_dot11smt_dot11PrivacyTable \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(6))
#define DIDmib_dot11smt_dot11PrivacyTable_dot11PrivacyInvoked \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(6) | \
			P80211DID_MKITEM(1) | 0x18000000)
#define DIDmib_dot11smt_dot11PrivacyTable_dot11WEPDefaultKeyID \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(6) | \
			P80211DID_MKITEM(2) | 0x18000000)
#define DIDmib_dot11smt_dot11PrivacyTable_dot11ExcludeUnencrypted \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(6) | \
			P80211DID_MKITEM(4) | 0x18000000)
#define DIDmib_dot11mac_dot11OperationTable \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1))
#define DIDmib_dot11mac_dot11OperationTable_dot11MACAddress \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(1) | 0x18000000)
#define DIDmib_dot11mac_dot11OperationTable_dot11RTSThreshold \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(2) | 0x18000000)
#define DIDmib_dot11mac_dot11OperationTable_dot11ShortRetryLimit \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(3) | 0x10000000)
#define DIDmib_dot11mac_dot11OperationTable_dot11LongRetryLimit \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(4) | 0x10000000)
#define DIDmib_dot11mac_dot11OperationTable_dot11FragmentationThreshold \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(5) | 0x18000000)
#define DIDmib_dot11mac_dot11OperationTable_dot11MaxTransmitMSDULifetime \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(6) | 0x10000000)
#define DIDmib_cat_dot11phy \
			P80211DID_MKSECTION(3)
#define DIDmib_dot11phy_dot11PhyOperationTable \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(1))
#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11CurrentTxPowerLevel \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(3) | \
			P80211DID_MKITEM(10) | 0x18000000)
#define DIDmib_dot11phy_dot11PhyDSSSTable \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(5))
#define DIDmib_dot11phy_dot11PhyDSSSTable_dot11CurrentChannel \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(5) | \
			P80211DID_MKITEM(1) | 0x10000000)
#define DIDmib_cat_lnx \
			P80211DID_MKSECTION(4)
#define DIDmib_lnx_lnxConfigTable \
			(P80211DID_MKSECTION(4) | \
			P80211DID_MKGROUP(1))
#define DIDmib_lnx_lnxConfigTable_lnxRSNAIE \
			(P80211DID_MKSECTION(4) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(1) | 0x18000000)
#define DIDmib_cat_p2 \
			P80211DID_MKSECTION(5)
#define DIDmib_p2_p2Static \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2))
#define DIDmib_p2_p2Static_p2CnfPortType \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(1) | 0x18000000)
#define DIDmib_p2_p2NIC_p2PRISupRange \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(5) | \
			P80211DID_MKITEM(6) | 0x10000000)
#define DIDmib_p2_p2MAC \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(6))
#define DIDmib_p2_p2MAC_p2CurrentTxRate \
=======
#define DIDMSG_DOT11REQ_MIBGET \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(1))
#define DIDMSG_DOT11REQ_MIBGET_MIBATTRIBUTE \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDMSG_DOT11REQ_MIBGET_RESULTCODE \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDMSG_DOT11REQ_MIBSET \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(2))
#define DIDMSG_DOT11REQ_MIBSET_MIBATTRIBUTE \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDMSG_DOT11REQ_MIBSET_RESULTCODE \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDMSG_DOT11REQ_SCAN \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(4))
#define DIDMSG_DOT11REQ_SCAN_RESULTS \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(5))
#define DIDMSG_DOT11REQ_START \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(13))
#define DIDMSG_DOT11IND_AUTHENTICATE \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1))
#define DIDMSG_DOT11IND_ASSOCIATE \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(3))
#define DIDMSG_LNXREQ_IFSTATE \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(1))
#define DIDMSG_LNXREQ_WLANSNIFF \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(2))
#define DIDMSG_LNXREQ_HOSTWEP \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(3))
#define DIDMSG_LNXREQ_COMMSQUALITY \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(4))
#define DIDMSG_LNXREQ_AUTOJOIN \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(5))
#define DIDMSG_P2REQ_READPDA \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2))
#define DIDMSG_P2REQ_READPDA_PDA \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDMSG_P2REQ_READPDA_RESULTCODE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDMSG_P2REQ_RAMDL_STATE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(11))
#define DIDMSG_P2REQ_RAMDL_STATE_ENABLE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(11) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDMSG_P2REQ_RAMDL_STATE_EXEADDR \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(11) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDMSG_P2REQ_RAMDL_STATE_RESULTCODE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(11) | \
			P80211DID_MKITEM(3) | 0x00000000)
#define DIDMSG_P2REQ_RAMDL_WRITE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12))
#define DIDMSG_P2REQ_RAMDL_WRITE_ADDR \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12) | \
			P80211DID_MKITEM(1) | 0x00000000)
#define DIDMSG_P2REQ_RAMDL_WRITE_LEN \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12) | \
			P80211DID_MKITEM(2) | 0x00000000)
#define DIDMSG_P2REQ_RAMDL_WRITE_DATA \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12) | \
			P80211DID_MKITEM(3) | 0x00000000)
#define DIDMSG_P2REQ_RAMDL_WRITE_RESULTCODE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(12) | \
			P80211DID_MKITEM(4) | 0x00000000)
#define DIDMSG_P2REQ_FLASHDL_STATE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(13))
#define DIDMSG_P2REQ_FLASHDL_WRITE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(14))
#define DIDMIB_CAT_DOT11SMT \
			P80211DID_MKSECTION(1)
#define DIDMIB_DOT11SMT_WEPDEFAULTKEYSTABLE \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(4))
#define didmib_dot11smt_wepdefaultkeystable_key(_i) \
			(DIDMIB_DOT11SMT_WEPDEFAULTKEYSTABLE | \
			P80211DID_MKITEM(_i) | 0x0c000000)
#define DIDMIB_DOT11SMT_PRIVACYTABLE \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(6))
#define DIDMIB_DOT11SMT_PRIVACYTABLE_PRIVACYINVOKED \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(6) | \
			P80211DID_MKITEM(1) | 0x18000000)
#define DIDMIB_DOT11SMT_PRIVACYTABLE_WEPDEFAULTKEYID \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(6) | \
			P80211DID_MKITEM(2) | 0x18000000)
#define DIDMIB_DOT11SMT_PRIVACYTABLE_EXCLUDEUNENCRYPTED \
			(P80211DID_MKSECTION(1) | \
			P80211DID_MKGROUP(6) | \
			P80211DID_MKITEM(4) | 0x18000000)
#define DIDMIB_DOT11MAC_OPERATIONTABLE \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1))
#define DIDMIB_DOT11MAC_OPERATIONTABLE_MACADDRESS \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(1) | 0x18000000)
#define DIDMIB_DOT11MAC_OPERATIONTABLE_RTSTHRESHOLD \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(2) | 0x18000000)
#define DIDMIB_DOT11MAC_OPERATIONTABLE_SHORTRETRYLIMIT \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(3) | 0x10000000)
#define DIDMIB_DOT11MAC_OPERATIONTABLE_LONGRETRYLIMIT \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(4) | 0x10000000)
#define DIDMIB_DOT11MAC_OPERATIONTABLE_FRAGMENTATIONTHRESHOLD \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(5) | 0x18000000)
#define DIDMIB_DOT11MAC_OPERATIONTABLE_MAXTRANSMITMSDULIFETIME \
			(P80211DID_MKSECTION(2) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(6) | 0x10000000)
#define DIDMIB_CAT_DOT11PHY \
			P80211DID_MKSECTION(3)
#define DIDMIB_DOT11PHY_OPERATIONTABLE \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(1))
#define DIDMIB_DOT11PHY_TXPOWERTABLE_CURRENTTXPOWERLEVEL \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(3) | \
			P80211DID_MKITEM(10) | 0x18000000)
#define DIDMIB_DOT11PHY_DSSSTABLE \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(5))
#define DIDMIB_DOT11PHY_DSSSTABLE_CURRENTCHANNEL \
			(P80211DID_MKSECTION(3) | \
			P80211DID_MKGROUP(5) | \
			P80211DID_MKITEM(1) | 0x10000000)
#define DIDMIB_CAT_LNX \
			P80211DID_MKSECTION(4)
#define DIDMIB_LNX_CONFIGTABLE \
			(P80211DID_MKSECTION(4) | \
			P80211DID_MKGROUP(1))
#define DIDMIB_LNX_CONFIGTABLE_RSNAIE \
			(P80211DID_MKSECTION(4) | \
			P80211DID_MKGROUP(1) | \
			P80211DID_MKITEM(1) | 0x18000000)
#define DIDMIB_CAT_P2 \
			P80211DID_MKSECTION(5)
#define DIDMIB_P2_STATIC \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2))
#define DIDMIB_P2_STATIC_CNFPORTTYPE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(2) | \
			P80211DID_MKITEM(1) | 0x18000000)
#define DIDMIB_P2_NIC_PRISUPRANGE \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(5) | \
			P80211DID_MKITEM(6) | 0x10000000)
#define DIDMIB_P2_MAC \
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(6))
#define DIDMIB_P2_MAC_CURRENTTXRATE \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			(P80211DID_MKSECTION(5) | \
			P80211DID_MKGROUP(6) | \
			P80211DID_MKITEM(12) | 0x10000000)
#endif
