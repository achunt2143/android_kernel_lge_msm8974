<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * File: rxtx.h
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Purpose:
 *
 * Author: Jerry Chen
 *
 * Date: Jun. 27, 2002
 *
 */

#ifndef __RXTX_H__
#define __RXTX_H__

<<<<<<< HEAD
#include "ttype.h"
#include "device.h"
#include "wcmd.h"

/*---------------------  Export Definitions -------------------------*/

/*---------------------  Export Classes  ----------------------------*/

//
// RTS buffer header
//
typedef struct tagSRTSDataF {
    WORD    wFrameControl;
    WORD    wDurationID;
    BYTE    abyRA[ETH_ALEN];
    BYTE    abyTA[ETH_ALEN];
} SRTSDataF, *PSRTSDataF;

//
// CTS buffer header
//
typedef struct tagSCTSDataF {
    WORD    wFrameControl;
    WORD    wDurationID;
    BYTE    abyRA[ETH_ALEN];
    WORD    wReserved;
} SCTSDataF, *PSCTSDataF;

//
// MICHDR data header
//
typedef struct tagSMICHDR {
    DWORD   adwHDR0[4];
    DWORD   adwHDR1[4];
    DWORD   adwHDR2[4];
} SMICHDR, *PSMICHDR;


typedef struct tagSTX_NAF_G_RTS
{
    //RsvTime
    WORD            wRTSTxRrvTime_ba;
    WORD            wRTSTxRrvTime_aa;
    WORD            wRTSTxRrvTime_bb;
    WORD            wReserved2;
    WORD            wTxRrvTime_b;
    WORD            wTxRrvTime_a;

    //RTS
    BYTE            byRTSSignalField_b;
    BYTE            byRTSServiceField_b;
    WORD            wRTSTransmitLength_b;
    BYTE            byRTSSignalField_a;
    BYTE            byRTSServiceField_a;
    WORD            wRTSTransmitLength_a;
    WORD            wRTSDuration_ba;
    WORD            wRTSDuration_aa;
    WORD            wRTSDuration_bb;
    WORD            wReserved3;
    SRTSDataF       sRTS;

    //Data
    BYTE            bySignalField_b;
    BYTE            byServiceField_b;
    WORD            wTransmitLength_b;
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_b;
    WORD            wDuration_a;
    WORD            wTimeStampOff_b;
    WORD            wTimeStampOff_a;

} TX_NAF_G_RTS, *PTX_NAF_G_RTS;

typedef struct tagSTX_NAF_G_RTS_MIC
{
    //RsvTime
    WORD            wRTSTxRrvTime_ba;
    WORD            wRTSTxRrvTime_aa;
    WORD            wRTSTxRrvTime_bb;
    WORD            wReserved2;
    WORD            wTxRrvTime_b;
    WORD            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //RTS
    BYTE            byRTSSignalField_b;
    BYTE            byRTSServiceField_b;
    WORD            wRTSTransmitLength_b;
    BYTE            byRTSSignalField_a;
    BYTE            byRTSServiceField_a;
    WORD            wRTSTransmitLength_a;
    WORD            wRTSDuration_ba;
    WORD            wRTSDuration_aa;
    WORD            wRTSDuration_bb;
    WORD            wReserved3;
    SRTSDataF       sRTS;

    //Data
    BYTE            bySignalField_b;
    BYTE            byServiceField_b;
    WORD            wTransmitLength_b;
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_b;
    WORD            wDuration_a;
    WORD            wTimeStampOff_b;
    WORD            wTimeStampOff_a;

} TX_NAF_G_RTS_MIC, *PTX_NAF_G_RTS_MIC;

typedef struct tagSTX_NAF_G_CTS
{
    //RsvTime
    WORD            wCTSTxRrvTime_ba;
    WORD            wReserved2;
    WORD            wTxRrvTime_b;
    WORD            wTxRrvTime_a;

    //CTS
    BYTE            byCTSSignalField_b;
    BYTE            byCTSServiceField_b;
    WORD            wCTSTransmitLength_b;
    WORD            wCTSDuration_ba;
    WORD            wReserved3;
    SCTSDataF       sCTS;

    //Data
    BYTE            bySignalField_b;
    BYTE            byServiceField_b;
    WORD            wTransmitLength_b;
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_b;
    WORD            wDuration_a;
    WORD            wTimeStampOff_b;
    WORD            wTimeStampOff_a;

} TX_NAF_G_CTS, *PTX_NAF_G_CTS;


typedef struct tagSTX_NAF_G_CTS_MIC
{
    //RsvTime
    WORD            wCTSTxRrvTime_ba;
    WORD            wReserved2;
    WORD            wTxRrvTime_b;
    WORD            wTxRrvTime_a;


    SMICHDR         sMICHDR;

    //CTS
    BYTE            byCTSSignalField_b;
    BYTE            byCTSServiceField_b;
    WORD            wCTSTransmitLength_b;
    WORD            wCTSDuration_ba;
    WORD            wReserved3;
    SCTSDataF       sCTS;

    //Data
    BYTE            bySignalField_b;
    BYTE            byServiceField_b;
    WORD            wTransmitLength_b;
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_b;
    WORD            wDuration_a;
    WORD            wTimeStampOff_b;
    WORD            wTimeStampOff_a;

} TX_NAF_G_CTS_MIC, *PTX_NAF_G_CTS_MIC;


typedef struct tagSTX_NAF_G_BEACON
{
    WORD            wFIFOCtl;
    WORD            wTimeStamp;

    //CTS
    BYTE            byCTSSignalField_b;
    BYTE            byCTSServiceField_b;
    WORD            wCTSTransmitLength_b;
    WORD            wCTSDuration_ba;
    WORD            wReserved1;
    SCTSDataF       sCTS;

    //Data
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_a;
    WORD            wTimeStampOff_a;


} TX_NAF_G_BEACON, *PTX_NAF_G_BEACON;


typedef struct tagSTX_NAF_AB_RTS
{
    //RsvTime
    WORD            wRTSTxRrvTime_ab;
    WORD            wTxRrvTime_ab;

    //RTS
    BYTE            byRTSSignalField_ab;
    BYTE            byRTSServiceField_ab;
    WORD            wRTSTransmitLength_ab;
    WORD            wRTSDuration_ab;
    WORD            wReserved2;
    SRTSDataF       sRTS;

    //Data
    BYTE            bySignalField_ab;
    BYTE            byServiceField_ab;
    WORD            wTransmitLength_ab;
    WORD            wDuration_ab;
    WORD            wTimeStampOff_ab;


} TX_NAF_AB_RTS, *PTX_NAF_AB_RTS;


typedef struct tagSTX_NAF_AB_RTS_MIC
{
    //RsvTime
    WORD            wRTSTxRrvTime_ab;
    WORD            wTxRrvTime_ab;

    SMICHDR         sMICHDR;

    //RTS
    BYTE            byRTSSignalField_ab;
    BYTE            byRTSServiceField_ab;
    WORD            wRTSTransmitLength_ab;
    WORD            wRTSDuration_ab;
    WORD            wReserved2;
    SRTSDataF       sRTS;

    //Data
    BYTE            bySignalField_ab;
    BYTE            byServiceField_ab;
    WORD            wTransmitLength_ab;
    WORD            wDuration_ab;
    WORD            wTimeStampOff_ab;


} TX_NAF_AB_RTS_MIC, *PTX_NAF_AB_RTS_MIC;



typedef struct tagSTX_NAF_AB_CTS
{
    //RsvTime
    WORD            wReserved2;
    WORD            wTxRrvTime_ab;

    //Data
    BYTE            bySignalField_ab;
    BYTE            byServiceField_ab;
    WORD            wTransmitLength_ab;
    WORD            wDuration_ab;
    WORD            wTimeStampOff_ab;

} TX_NAF_AB_CTS, *PTX_NAF_AB_CTS;

typedef struct tagSTX_NAF_AB_CTS_MIC
{
    //RsvTime
    WORD            wReserved2;
    WORD            wTxRrvTime_ab;

    SMICHDR         sMICHDR;

    //Data
    BYTE            bySignalField_ab;
    BYTE            byServiceField_ab;
    WORD            wTransmitLength_ab;
    WORD            wDuration_ab;
    WORD            wTimeStampOff_ab;

} TX_NAF_AB_CTS_MIC, *PTX_NAF_AB_CTS_MIC;


typedef struct tagSTX_NAF_AB_BEACON
{
    WORD            wFIFOCtl;
    WORD            wTimeStamp;

   //Data
    BYTE            bySignalField_ab;
    BYTE            byServiceField_ab;
    WORD            wTransmitLength_ab;
    WORD            wDuration_ab;
    WORD            wTimeStampOff_ab;

} TX_NAF_AB_BEACON, *PTX_NAF_AB_BEACON;

typedef struct tagSTX_AF_G_RTS
{
    //RsvTime
    WORD            wRTSTxRrvTime_ba;
    WORD            wRTSTxRrvTime_aa;
    WORD            wRTSTxRrvTime_bb;
    WORD            wReserved2;
    WORD            wTxRrvTime_b;
    WORD            wTxRrvTime_a;

    //RTS
    BYTE            byRTSSignalField_b;
    BYTE            byRTSServiceField_b;
    WORD            wRTSTransmitLength_b;
    BYTE            byRTSSignalField_a;
    BYTE            byRTSServiceField_a;
    WORD            wRTSTransmitLength_a;
    WORD            wRTSDuration_ba;
    WORD            wRTSDuration_aa;
    WORD            wRTSDuration_bb;
    WORD            wReserved3;
    WORD            wRTSDuration_ba_f0;
    WORD            wRTSDuration_aa_f0;
    WORD            wRTSDuration_ba_f1;
    WORD            wRTSDuration_aa_f1;
    SRTSDataF       sRTS;

    //Data
    BYTE            bySignalField_b;
    BYTE            byServiceField_b;
    WORD            wTransmitLength_b;
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_b;
    WORD            wDuration_a;
    WORD            wDuration_a_f0;
    WORD            wDuration_a_f1;
    WORD            wTimeStampOff_b;
    WORD            wTimeStampOff_a;

} TX_AF_G_RTS, *PTX_AF_G_RTS;


typedef struct tagSTX_AF_G_RTS_MIC
{
    //RsvTime
    WORD            wRTSTxRrvTime_ba;
    WORD            wRTSTxRrvTime_aa;
    WORD            wRTSTxRrvTime_bb;
    WORD            wReserved2;
    WORD            wTxRrvTime_b;
    WORD            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //RTS
    BYTE            byRTSSignalField_b;
    BYTE            byRTSServiceField_b;
    WORD            wRTSTransmitLength_b;
    BYTE            byRTSSignalField_a;
    BYTE            byRTSServiceField_a;
    WORD            wRTSTransmitLength_a;
    WORD            wRTSDuration_ba;
    WORD            wRTSDuration_aa;
    WORD            wRTSDuration_bb;
    WORD            wReserved3;
    WORD            wRTSDuration_ba_f0;
    WORD            wRTSDuration_aa_f0;
    WORD            wRTSDuration_ba_f1;
    WORD            wRTSDuration_aa_f1;
    SRTSDataF       sRTS;

    //Data
    BYTE            bySignalField_b;
    BYTE            byServiceField_b;
    WORD            wTransmitLength_b;
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_b;
    WORD            wDuration_a;
    WORD            wDuration_a_f0;
    WORD            wDuration_a_f1;
    WORD            wTimeStampOff_b;
    WORD            wTimeStampOff_a;

} TX_AF_G_RTS_MIC, *PTX_AF_G_RTS_MIC;



typedef struct tagSTX_AF_G_CTS
{
    //RsvTime
    WORD            wCTSTxRrvTime_ba;
    WORD            wReserved2;
    WORD            wTxRrvTime_b;
    WORD            wTxRrvTime_a;

    //CTS
    BYTE            byCTSSignalField_b;
    BYTE            byCTSServiceField_b;
    WORD            wCTSTransmitLength_b;
    WORD            wCTSDuration_ba;
    WORD            wReserved3;
    WORD            wCTSDuration_ba_f0;
    WORD            wCTSDuration_ba_f1;
    SCTSDataF       sCTS;

    //Data
    BYTE            bySignalField_b;
    BYTE            byServiceField_b;
    WORD            wTransmitLength_b;
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_b;
    WORD            wDuration_a;
    WORD            wDuration_a_f0;
    WORD            wDuration_a_f1;
    WORD            wTimeStampOff_b;
    WORD            wTimeStampOff_a;

} TX_AF_G_CTS, *PTX_AF_G_CTS;


typedef struct tagSTX_AF_G_CTS_MIC
{
    //RsvTime
    WORD            wCTSTxRrvTime_ba;
    WORD            wReserved2;
    WORD            wTxRrvTime_b;
    WORD            wTxRrvTime_a;


    SMICHDR         sMICHDR;

    //CTS
    BYTE            byCTSSignalField_b;
    BYTE            byCTSServiceField_b;
    WORD            wCTSTransmitLength_b;
    WORD            wCTSDuration_ba;
    WORD            wReserved3;
    WORD            wCTSDuration_ba_f0;
    WORD            wCTSDuration_ba_f1;
    SCTSDataF       sCTS;

    //Data
    BYTE            bySignalField_b;
    BYTE            byServiceField_b;
    WORD            wTransmitLength_b;
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_b;
    WORD            wDuration_a;
    WORD            wDuration_a_f0;
    WORD            wDuration_a_f1;
    WORD            wTimeStampOff_b;
    WORD            wTimeStampOff_a;

} TX_AF_G_CTS_MIC, *PTX_AF_G_CTS_MIC;



typedef struct tagSTX_AF_A_RTS
{
    //RsvTime
    WORD            wRTSTxRrvTime_a;
    WORD            wTxRrvTime_a;

    //RTS
    BYTE            byRTSSignalField_a;
    BYTE            byRTSServiceField_a;
    WORD            wRTSTransmitLength_a;
    WORD            wRTSDuration_a;
    WORD            wReserved2;
    WORD            wRTSDuration_a_f0;
    WORD            wRTSDuration_a_f1;
    SRTSDataF       sRTS;

    //Data
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_a;
    WORD            wTimeStampOff_a;
    WORD            wDuration_a_f0;
    WORD            wDuration_a_f1;

} TX_AF_A_RTS, *PTX_AF_A_RTS;


typedef struct tagSTX_AF_A_RTS_MIC
{
    //RsvTime
    WORD            wRTSTxRrvTime_a;
    WORD            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //RTS
    BYTE            byRTSSignalField_a;
    BYTE            byRTSServiceField_a;
    WORD            wRTSTransmitLength_a;
    WORD            wRTSDuration_a;
    WORD            wReserved2;
    WORD            wRTSDuration_a_f0;
    WORD            wRTSDuration_a_f1;
    SRTSDataF       sRTS;

    //Data
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_a;
    WORD            wTimeStampOff_a;
    WORD            wDuration_a_f0;
    WORD            wDuration_a_f1;

} TX_AF_A_RTS_MIC, *PTX_AF_A_RTS_MIC;



typedef struct tagSTX_AF_A_CTS
{
    //RsvTime
    WORD            wReserved2;
    WORD            wTxRrvTime_a;

    //Data
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_a;
    WORD            wTimeStampOff_a;
    WORD            wDuration_a_f0;
    WORD            wDuration_a_f1;

} TX_AF_A_CTS, *PTX_AF_A_CTS;


typedef struct tagSTX_AF_A_CTS_MIC
{
    //RsvTime
    WORD            wReserved2;
    WORD            wTxRrvTime_a;

    SMICHDR         sMICHDR;

    //Data
    BYTE            bySignalField_a;
    BYTE            byServiceField_a;
    WORD            wTransmitLength_a;
    WORD            wDuration_a;
    WORD            wTimeStampOff_a;
    WORD            wDuration_a_f0;
    WORD            wDuration_a_f1;

} TX_AF_A_CTS_MIC, *PTX_AF_A_CTS_MIC;


//
// union with all of the TX Buffer Type
//
typedef union tagUTX_BUFFER_CONTAINER
{
    TX_NAF_G_RTS                    RTS_G;
    TX_NAF_G_RTS_MIC                RTS_G_MIC;
    TX_NAF_G_CTS                    CTS_G;
    TX_NAF_G_CTS_MIC                CTS_G_MIC;
    //TX_NAF_G_BEACON                 Beacon_G;
    TX_NAF_AB_RTS                   RTS_AB;
    TX_NAF_AB_RTS_MIC               RTS_AB_MIC;
    TX_NAF_AB_CTS                   CTS_AB;
    TX_NAF_AB_CTS_MIC               CTS_AB_MIC;
    //TX_NAF_AB_BEACON                Beacon_AB;
    TX_AF_G_RTS                     RTS_G_AutoFB;
    TX_AF_G_RTS_MIC                 RTS_G_AutoFB_MIC;
    TX_AF_G_CTS                     CTS_G_AutoFB;
    TX_AF_G_CTS_MIC                 CTS_G_AutoFB_MIC;
    TX_AF_A_RTS                     RTS_A_AutoFB;
    TX_AF_A_RTS_MIC                 RTS_A_AutoFB_MIC;
    TX_AF_A_CTS                     CTS_A_AutoFB;
    TX_AF_A_CTS_MIC                 CTS_A_AutoFB_MIC;

} TX_BUFFER_CONTAINER, *PTX_BUFFER_CONTAINER;


//
// Remote NDIS message format
//
typedef struct tagSTX_BUFFER
{
    BYTE                            byType;
    BYTE                            byPKTNO;
    WORD                            wTxByteCount;

    DWORD                           adwTxKey[4];
    WORD                            wFIFOCtl;
    WORD                            wTimeStamp;
    WORD                            wFragCtl;
    WORD                            wReserved;


    // Actual message
    TX_BUFFER_CONTAINER             BufferHeader;

} TX_BUFFER, *PTX_BUFFER;


//
// Remote NDIS message format
//
typedef struct tagSBEACON_BUFFER
{
    BYTE                            byType;
    BYTE                            byPKTNO;
    WORD                            wTxByteCount;

    WORD                            wFIFOCtl;
    WORD                            wTimeStamp;

    // Actual message
    TX_BUFFER_CONTAINER             BufferHeader;

} BEACON_BUFFER, *PBEACON_BUFFER;


/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

BOOL
bPacketToWirelessUsb(
      PSDevice         pDevice,
      BYTE             byPktType,
      PBYTE            usbPacketBuf,
      BOOL             bNeedEncrypt,
      unsigned int             cbPayloadSize,
      unsigned int             uDMAIdx,
      PSEthernetHeader psEthHeader,
      PBYTE            pPacket,
      PSKeyItem        pTransmitKey,
      unsigned int             uNodeIndex,
      WORD             wCurrentRate,
     unsigned int             *pcbHeaderLen,
     unsigned int             *pcbTotalLen
    );

void vDMA0_tx_80211(PSDevice  pDevice, struct sk_buff *skb);
int nsDMA_tx_packet(PSDevice pDevice,
		    unsigned int uDMAIdx,
		    struct sk_buff *skb);
CMD_STATUS csMgmt_xmit(PSDevice pDevice, PSTxMgmtPacket pPacket);
CMD_STATUS csBeacon_xmit(PSDevice pDevice, PSTxMgmtPacket pPacket);
BOOL bRelayPacketSend(PSDevice pDevice, PBYTE pbySkbData,
		      unsigned int uDataLen, unsigned int uNodeIndex);
=======
#include "device.h"
#include "wcmd.h"
#include "baseband.h"

#define DEFAULT_MGN_LIFETIME_RES_64us	125  /* 64us */
#define DEFAULT_MSDU_LIFETIME_RES_64us  8000

/* Length, Service, and Signal fields of Phy for Tx */
struct vnt_phy_field {
	u8 signal;
	u8 service;
	__le16 len;
} __packed;

/* MIC HDR data header */
struct vnt_mic_hdr {
	u8 id;
	u8 tx_priority;
	u8 mic_addr2[6];
	u8 ccmp_pn[IEEE80211_CCMP_PN_LEN];
	__be16 payload_len;
	__be16 hlen;
	__le16 frame_control;
	u8 addr1[6];
	u8 addr2[6];
	u8 addr3[6];
	__le16 seq_ctrl;
	u8 addr4[6];
	u16 packing; /* packing to 48 bytes */
} __packed;

/* RsvTime buffer header */
struct vnt_rrv_time_rts {
	__le16 rts_rrv_time_ba;
	__le16 rts_rrv_time_aa;
	__le16 rts_rrv_time_bb;
	u16 wReserved;
	__le16 rrv_time_b;
	__le16 rrv_time_a;
} __packed;

struct vnt_rrv_time_cts {
	__le16 cts_rrv_time_ba;
	u16 wReserved;
	__le16 rrv_time_b;
	__le16 rrv_time_a;
} __packed;

struct vnt_rrv_time_ab {
	__le16 rts_rrv_time;
	__le16 rrv_time;
} __packed;

/* TX data header */
struct vnt_tx_datahead_g {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_b;
	__le16 duration_a;
	__le16 time_stamp_off_b;
	__le16 time_stamp_off_a;
} __packed;

struct vnt_tx_datahead_ab {
	struct vnt_phy_field ab;
	__le16 duration;
	__le16 time_stamp_off;
} __packed;

/* RTS buffer header */
struct vnt_rts_g {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_ba;
	__le16 duration_aa;
	__le16 duration_bb;
	u16 wReserved;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_g data_head;
} __packed __aligned(2);

struct vnt_rts_ab {
	struct vnt_phy_field ab;
	__le16 duration;
	u16 wReserved;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_ab data_head;
} __packed __aligned(2);

/* CTS buffer header */
struct vnt_cts {
	struct vnt_phy_field b;
	__le16 duration_ba;
	u16 wReserved;
	struct ieee80211_cts data;
	u16 reserved2;
	struct vnt_tx_datahead_g data_head;
} __packed __aligned(2);

union vnt_tx_data_head {
	/* rts g */
	struct vnt_rts_g rts_g;
	/* rts a/b */
	struct vnt_rts_ab rts_ab;
	/* cts g */
	struct vnt_cts cts_g;
	/* no rts/cts */
	struct vnt_tx_datahead_ab data_head_ab;
};

struct vnt_tx_mic_hdr {
	struct vnt_mic_hdr hdr;
	union vnt_tx_data_head head;
} __packed;

union vnt_tx {
	struct vnt_tx_mic_hdr mic;
	union vnt_tx_data_head head;
};

union vnt_tx_head {
	struct {
		struct vnt_rrv_time_rts rts;
		union vnt_tx tx;
	} __packed tx_rts;
	struct {
		struct vnt_rrv_time_cts cts;
		union vnt_tx tx;
	} __packed tx_cts;
	struct {
		struct vnt_rrv_time_ab ab;
		union vnt_tx tx;
	} __packed tx_ab;
};

struct vnt_tx_fifo_head {
	u8 tx_key[WLAN_KEY_LEN_CCMP];
	__le16 fifo_ctl;
	__le16 time_stamp;
	__le16 frag_ctl;
	__le16 current_rate;
} __packed;

struct vnt_tx_buffer {
	struct vnt_tx_fifo_head fifo_head;
	union vnt_tx_head tx_head;
} __packed;

struct vnt_tx_short_buf_head {
	__le16 fifo_ctl;
	u16 time_stamp;
	struct vnt_phy_field ab;
	__le16 duration;
	__le16 time_stamp_off;
} __packed;

int vnt_tx_packet(struct vnt_private *priv, struct sk_buff *skb);
int vnt_beacon_make(struct vnt_private *priv, struct ieee80211_vif *vif);
int vnt_beacon_enable(struct vnt_private *priv, struct ieee80211_vif *vif,
		      struct ieee80211_bss_conf *conf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __RXTX_H__ */
