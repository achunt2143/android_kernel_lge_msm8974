<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
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
 * File: card.c
 * Purpose: Provide functions to setup NIC operation mode
 * Functions:
 *      s_vSafeResetTx - Rest Tx
 *      CARDvSetRSPINF - Set RSPINF
 *      vUpdateIFS - Update slotTime,SIFS,DIFS, and EIFS
 *      CARDvUpdateBasicTopRate - Update BasicTopRate
 *      CARDbAddBasicRate - Add to BasicRateSet
 *      CARDbSetBasicRate - Set Basic Tx Rate
 *      CARDbIsOFDMinBasicRate - Check if any OFDM rate is in BasicRateSet
 *      CARDvSetLoopbackMode - Set Loopback mode
 *      CARDbSoftwareReset - Sortware reset NIC
 *      CARDqGetTSFOffset - Caculate TSFOffset
 *      CARDbGetCurrentTSF - Read Current NIC TSF counter
 *      CARDqGetNextTBTT - Caculate Next Beacon TSF counter
 *      CARDvSetFirstNextTBTT - Set NIC Beacon time
 *      CARDvUpdateNextTBTT - Sync. NIC Beacon time
 *      CARDbRadioPowerOff - Turn Off NIC Radio Power
 *      CARDbRadioPowerOn - Turn On NIC Radio Power
 *      CARDbSetWEPMode - Set NIC Wep mode
 *      CARDbSetTxPower - Set NIC tx power
 *
 * Revision History:
 *      06-10-2003 Bryan YC Fan:  Re-write codes to support VT3253 spec.
 *      08-26-2003 Kyle Hsu:      Modify the defination type of dwIoBase.
 *      09-01-2003 Bryan YC Fan:  Add vUpdateIFS().
 *
 */

#include "tmacro.h"
=======
 * Purpose: Provide functions to setup NIC operation mode
 * Functions:
 *      vnt_set_rspinf - Set RSPINF
 *      vnt_update_ifs - Update slotTime,SIFS,DIFS, and EIFS
 *      vnt_update_top_rates - Update BasicTopRate
 *      vnt_add_basic_rate - Add to BasicRateSet
 *      vnt_ofdm_min_rate - Check if any OFDM rate is in BasicRateSet
 *      vnt_get_tsf_offset - Calculate TSFOffset
 *      vnt_get_next_tbtt - Calculate Next Beacon TSF counter
 *      vnt_reset_next_tbtt - Set NIC Beacon time
 *      vnt_update_next_tbtt - Sync. NIC Beacon time
 *      vnt_radio_power_off - Turn Off NIC Radio Power
 *      vnt_radio_power_on - Turn On NIC Radio Power
 *
 * Revision History:
 *      06-10-2003 Bryan YC Fan:  Re-write codes to support VT3253 spec.
 *      08-26-2003 Kyle Hsu:      Modify the definition type of dwIoBase.
 *      09-01-2003 Bryan YC Fan:  Add vnt_update_ifs().
 *
 */

#include <linux/bitops.h>
#include <linux/errno.h>
#include "device.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "card.h"
#include "baseband.h"
#include "mac.h"
#include "desc.h"
#include "rf.h"
#include "power.h"
#include "key.h"
<<<<<<< HEAD
#include "rc4.h"
#include "country.h"
#include "datarate.h"
#include "rndis.h"
#include "control.h"

/*---------------------  Static Definitions -------------------------*/

//static int          msglevel                =MSG_LEVEL_DEBUG;
static int          msglevel                =MSG_LEVEL_INFO;


/*---------------------  Static Definitions -------------------------*/
#define CB_TXPOWER_LEVEL            6

/*---------------------  Static Classes  ----------------------------*/

/*---------------------  Static Variables  --------------------------*/
//const WORD cwRXBCNTSFOff[MAX_RATE] =
//{17, 34, 96, 192, 34, 23, 17, 11, 8, 5, 4, 3};

const WORD cwRXBCNTSFOff[MAX_RATE] =
{192, 96, 34, 17, 34, 23, 17, 11, 8, 5, 4, 3};

/*---------------------  Static Functions  --------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/
/*
 * Description: Set NIC media channel
 *
 * Parameters:
 *  In:
 *      pDevice             - The adapter to be set
 *      uConnectionChannel  - Channel to be set
 *  Out:
 *      none
 */
void CARDbSetMediaChannel(void *pDeviceHandler, unsigned int uConnectionChannel)
{
PSDevice            pDevice = (PSDevice) pDeviceHandler;

    if (pDevice->byBBType == BB_TYPE_11A) { // 15 ~ 38
        if ((uConnectionChannel < (CB_MAX_CHANNEL_24G+1)) || (uConnectionChannel > CB_MAX_CHANNEL))
            uConnectionChannel = (CB_MAX_CHANNEL_24G+1);
    } else {
        if ((uConnectionChannel > CB_MAX_CHANNEL_24G) || (uConnectionChannel == 0)) // 1 ~ 14
            uConnectionChannel = 1;
    }

    // clear NAV
    MACvRegBitsOn(pDevice, MAC_REG_MACCR, MACCR_CLRNAV);

    // Set Channel[7] = 0 to tell H/W channel is changing now.
    MACvRegBitsOff(pDevice, MAC_REG_CHANNEL, 0x80);

    //if (pMgmt->uCurrChannel == uConnectionChannel)
    //    return bResult;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_SELECT_CHANNLE,
                        (WORD) uConnectionChannel,
                        0,
                        0,
                        NULL
                        );

    //{{ RobertYu: 20041202
    //// TX_PE will reserve 3 us for MAX2829 A mode only, it is for better TX throughput

    if (pDevice->byBBType == BB_TYPE_11A) {
        pDevice->byCurPwr = 0xFF;
        RFbRawSetPower(pDevice, pDevice->abyOFDMAPwrTbl[uConnectionChannel-15], RATE_54M);
    } else if (pDevice->byBBType == BB_TYPE_11G) {
        pDevice->byCurPwr = 0xFF;
        RFbRawSetPower(pDevice, pDevice->abyOFDMPwrTbl[uConnectionChannel-1], RATE_54M);
    } else {
        pDevice->byCurPwr = 0xFF;
        RFbRawSetPower(pDevice, pDevice->abyCCKPwrTbl[uConnectionChannel-1], RATE_1M);
    }
    ControlvWriteByte(pDevice,MESSAGE_REQUEST_MACREG,MAC_REG_CHANNEL,(BYTE)(uConnectionChannel|0x80));
}

/*
 * Description: Get CCK mode basic rate
 *
 * Parameters:
 *  In:
 *      pDevice             - The adapter to be set
 *      wRateIdx            - Receiving data rate
 *  Out:
 *      none
 *
 * Return Value: response Control frame rate
 *
 */
static WORD swGetCCKControlRate(void *pDeviceHandler, WORD wRateIdx)
{
    PSDevice    pDevice = (PSDevice) pDeviceHandler;
    unsigned int ui = (unsigned int)wRateIdx;
    while (ui > RATE_1M) {
        if (pDevice->wBasicRate & ((WORD)1 << ui)) {
            return (WORD)ui;
        }
        ui --;
    }
    return (WORD)RATE_1M;
}

/*
 * Description: Get OFDM mode basic rate
 *
 * Parameters:
 *  In:
 *      pDevice             - The adapter to be set
 *      wRateIdx            - Receiving data rate
 *  Out:
 *      none
 *
 * Return Value: response Control frame rate
 *
 */
static WORD swGetOFDMControlRate(void *pDeviceHandler, WORD wRateIdx)
{
    PSDevice    pDevice = (PSDevice) pDeviceHandler;
    unsigned int ui = (unsigned int)wRateIdx;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"BASIC RATE: %X\n", pDevice->wBasicRate);

    if (!CARDbIsOFDMinBasicRate(pDevice)) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"swGetOFDMControlRate:(NO OFDM) %d\n", wRateIdx);
        if (wRateIdx > RATE_24M)
            wRateIdx = RATE_24M;
        return wRateIdx;
    }
    while (ui > RATE_11M) {
        if (pDevice->wBasicRate & ((WORD)1 << ui)) {
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"swGetOFDMControlRate : %d\n", ui);
            return (WORD)ui;
        }
        ui --;
    }
    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"swGetOFDMControlRate: 6M\n");
    return (WORD)RATE_24M;
}

/*
 * Description: Caculate TxRate and RsvTime fields for RSPINF in OFDM mode.
 *
 * Parameters:
 *  In:
 *      wRate           - Tx Rate
 *      byPktType       - Tx Packet type
 *  Out:
 *      pbyTxRate       - pointer to RSPINF TxRate field
 *      pbyRsvTime      - pointer to RSPINF RsvTime field
 *
 * Return Value: none
 *
 */
void
CARDvCaculateOFDMRParameter (
      WORD wRate,
      BYTE byBBType,
     PBYTE pbyTxRate,
     PBYTE pbyRsvTime
    )
{
    switch (wRate) {
    case RATE_6M :
        if (byBBType == BB_TYPE_11A) {//5GHZ
            *pbyTxRate = 0x9B;
            *pbyRsvTime = 24;
        }
        else {
            *pbyTxRate = 0x8B;
            *pbyRsvTime = 30;
        }
        break;

    case RATE_9M :
        if (byBBType == BB_TYPE_11A) {//5GHZ
            *pbyTxRate = 0x9F;
            *pbyRsvTime = 16;
        }
        else {
            *pbyTxRate = 0x8F;
            *pbyRsvTime = 22;
        }
        break;

   case RATE_12M :
        if (byBBType == BB_TYPE_11A) {//5GHZ
            *pbyTxRate = 0x9A;
            *pbyRsvTime = 12;
        }
        else {
            *pbyTxRate = 0x8A;
            *pbyRsvTime = 18;
        }
        break;

   case RATE_18M :
        if (byBBType == BB_TYPE_11A) {//5GHZ
            *pbyTxRate = 0x9E;
            *pbyRsvTime = 8;
        }
        else {
            *pbyTxRate = 0x8E;
            *pbyRsvTime = 14;
        }
        break;

    case RATE_36M :
        if (byBBType == BB_TYPE_11A) {//5GHZ
            *pbyTxRate = 0x9D;
            *pbyRsvTime = 4;
        }
        else {
            *pbyTxRate = 0x8D;
            *pbyRsvTime = 10;
        }
        break;

    case RATE_48M :
        if (byBBType == BB_TYPE_11A) {//5GHZ
            *pbyTxRate = 0x98;
            *pbyRsvTime = 4;
        }
        else {
            *pbyTxRate = 0x88;
            *pbyRsvTime = 10;
        }
        break;

    case RATE_54M :
        if (byBBType == BB_TYPE_11A) {//5GHZ
            *pbyTxRate = 0x9C;
            *pbyRsvTime = 4;
        }
        else {
            *pbyTxRate = 0x8C;
            *pbyRsvTime = 10;
        }
        break;

    case RATE_24M :
    default :
        if (byBBType == BB_TYPE_11A) {//5GHZ
            *pbyTxRate = 0x99;
            *pbyRsvTime = 8;
        }
        else {
            *pbyTxRate = 0x89;
            *pbyRsvTime = 14;
        }
        break;
    }
}

/*
 * Description: Set RSPINF
 *
 * Parameters:
 *  In:
 *      pDevice             - The adapter to be set
 *  Out:
 *      none
 *
 * Return Value: None.
 *
 */
void CARDvSetRSPINF(void *pDeviceHandler, BYTE byBBType)
{
    PSDevice    pDevice = (PSDevice) pDeviceHandler;
    BYTE    abyServ[4] = {0,0,0,0};             // For CCK
    BYTE    abySignal[4] = {0,0,0,0};
    WORD    awLen[4] = {0,0,0,0};
    BYTE    abyTxRate[9] = {0,0,0,0,0,0,0,0,0}; // For OFDM
    BYTE    abyRsvTime[9] = {0,0,0,0,0,0,0,0,0};
    BYTE    abyData[34];
    int     i;

    //RSPINF_b_1
    BBvCaculateParameter(pDevice,
                         14,
                         swGetCCKControlRate(pDevice, RATE_1M),
                         PK_TYPE_11B,
                         &awLen[0],
                         &abyServ[0],
                         &abySignal[0]
    );

    ///RSPINF_b_2
    BBvCaculateParameter(pDevice,
                         14,
                         swGetCCKControlRate(pDevice, RATE_2M),
                         PK_TYPE_11B,
                         &awLen[1],
                         &abyServ[1],
                         &abySignal[1]
    );

    //RSPINF_b_5
    BBvCaculateParameter(pDevice,
                         14,
                         swGetCCKControlRate(pDevice, RATE_5M),
                         PK_TYPE_11B,
                         &awLen[2],
                         &abyServ[2],
                         &abySignal[2]
    );

    //RSPINF_b_11
    BBvCaculateParameter(pDevice,
                         14,
                         swGetCCKControlRate(pDevice, RATE_11M),
                         PK_TYPE_11B,
                         &awLen[3],
                         &abyServ[3],
                         &abySignal[3]
    );

    //RSPINF_a_6
    CARDvCaculateOFDMRParameter (RATE_6M,
                                 byBBType,
                                 &abyTxRate[0],
                                 &abyRsvTime[0]);

    //RSPINF_a_9
    CARDvCaculateOFDMRParameter (RATE_9M,
                                 byBBType,
                                 &abyTxRate[1],
                                 &abyRsvTime[1]);

    //RSPINF_a_12
    CARDvCaculateOFDMRParameter (RATE_12M,
                                 byBBType,
                                 &abyTxRate[2],
                                 &abyRsvTime[2]);

    //RSPINF_a_18
    CARDvCaculateOFDMRParameter (RATE_18M,
                                 byBBType,
                                 &abyTxRate[3],
                                 &abyRsvTime[3]);

    //RSPINF_a_24
    CARDvCaculateOFDMRParameter (RATE_24M,
                                 byBBType,
                                 &abyTxRate[4],
                                 &abyRsvTime[4]);

    //RSPINF_a_36
    CARDvCaculateOFDMRParameter (swGetOFDMControlRate(pDevice, RATE_36M),
                                 byBBType,
                                 &abyTxRate[5],
                                 &abyRsvTime[5]);

    //RSPINF_a_48
    CARDvCaculateOFDMRParameter (swGetOFDMControlRate(pDevice, RATE_48M),
                                 byBBType,
                                 &abyTxRate[6],
                                 &abyRsvTime[6]);

    //RSPINF_a_54
    CARDvCaculateOFDMRParameter (swGetOFDMControlRate(pDevice, RATE_54M),
                                 byBBType,
                                 &abyTxRate[7],
                                 &abyRsvTime[7]);

    //RSPINF_a_72
    CARDvCaculateOFDMRParameter (swGetOFDMControlRate(pDevice, RATE_54M),
                                 byBBType,
                                 &abyTxRate[8],
                                 &abyRsvTime[8]);

    abyData[0] = (BYTE)(awLen[0]&0xFF);
    abyData[1] = (BYTE)(awLen[0]>>8);
    abyData[2] = abySignal[0];
    abyData[3] = abyServ[0];

    abyData[4] = (BYTE)(awLen[1]&0xFF);
    abyData[5] = (BYTE)(awLen[1]>>8);
    abyData[6] = abySignal[1];
    abyData[7] = abyServ[1];

    abyData[8] = (BYTE)(awLen[2]&0xFF);
    abyData[9] = (BYTE)(awLen[2]>>8);
    abyData[10] = abySignal[2];
    abyData[11] = abyServ[2];

    abyData[12] = (BYTE)(awLen[3]&0xFF);
    abyData[13] = (BYTE)(awLen[3]>>8);
    abyData[14] = abySignal[3];
    abyData[15] = abyServ[3];

    for (i = 0; i < 9; i++) {
	abyData[16+i*2] = abyTxRate[i];
	abyData[16+i*2+1] = abyRsvTime[i];
    }

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE,
                        MAC_REG_RSPINF_B_1,
                        MESSAGE_REQUEST_MACREG,
                        34,
                        &abyData[0]);

}

/*
 * Description: Update IFS
 *
 * Parameters:
 *  In:
 *      pDevice             - The adapter to be set
 *  Out:
 *      none
 *
 * Return Value: None.
 *
 */
void vUpdateIFS(void *pDeviceHandler)
{
    PSDevice    pDevice = (PSDevice) pDeviceHandler;
    //Set SIFS, DIFS, EIFS, SlotTime, CwMin
    BYTE byMaxMin = 0;
    BYTE byData[4];

    if (pDevice->byPacketType==PK_TYPE_11A) {//0000 0000 0000 0000,11a
        pDevice->uSlot = C_SLOT_SHORT;
        pDevice->uSIFS = C_SIFS_A;
        pDevice->uDIFS = C_SIFS_A + 2*C_SLOT_SHORT;
        pDevice->uCwMin = C_CWMIN_A;
        byMaxMin = 4;
    }
    else if (pDevice->byPacketType==PK_TYPE_11B) {//0000 0001 0000 0000,11b
        pDevice->uSlot = C_SLOT_LONG;
        pDevice->uSIFS = C_SIFS_BG;
        pDevice->uDIFS = C_SIFS_BG + 2*C_SLOT_LONG;
          pDevice->uCwMin = C_CWMIN_B;
        byMaxMin = 5;
    }
    else {// PK_TYPE_11GA & PK_TYPE_11GB
        BYTE byRate = 0;
        BOOL bOFDMRate = FALSE;
	unsigned int ii = 0;
        PWLAN_IE_SUPP_RATES pItemRates = NULL;

        pDevice->uSIFS = C_SIFS_BG;
        if (pDevice->bShortSlotTime) {
            pDevice->uSlot = C_SLOT_SHORT;
        } else {
            pDevice->uSlot = C_SLOT_LONG;
        }
        pDevice->uDIFS = C_SIFS_BG + 2*pDevice->uSlot;

        pItemRates = (PWLAN_IE_SUPP_RATES)pDevice->sMgmtObj.abyCurrSuppRates;
        for (ii = 0; ii < pItemRates->len; ii++) {
            byRate = (BYTE)(pItemRates->abyRates[ii]&0x7F);
            if (RATEwGetRateIdx(byRate) > RATE_11M) {
                bOFDMRate = TRUE;
                break;
            }
        }
        if (bOFDMRate == FALSE) {
            pItemRates = (PWLAN_IE_SUPP_RATES)pDevice->sMgmtObj.abyCurrExtSuppRates;
            for (ii = 0; ii < pItemRates->len; ii++) {
                byRate = (BYTE)(pItemRates->abyRates[ii]&0x7F);
                if (RATEwGetRateIdx(byRate) > RATE_11M) {
                    bOFDMRate = TRUE;
                    break;
                }
            }
        }
        if (bOFDMRate == TRUE) {
            pDevice->uCwMin = C_CWMIN_A;
            byMaxMin = 4;
        } else {
            pDevice->uCwMin = C_CWMIN_B;
            byMaxMin = 5;
        }
    }

    pDevice->uCwMax = C_CWMAX;
    pDevice->uEIFS = C_EIFS;

    byData[0] = (BYTE)pDevice->uSIFS;
    byData[1] = (BYTE)pDevice->uDIFS;
    byData[2] = (BYTE)pDevice->uEIFS;
    byData[3] = (BYTE)pDevice->uSlot;
    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE,
                        MAC_REG_SIFS,
                        MESSAGE_REQUEST_MACREG,
                        4,
                        &byData[0]);

    byMaxMin |= 0xA0;//1010 1111,C_CWMAX = 1023
    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE,
                        MAC_REG_CWMAXMIN0,
                        MESSAGE_REQUEST_MACREG,
                        1,
                        &byMaxMin);
}

void CARDvUpdateBasicTopRate(void *pDeviceHandler)
{
PSDevice    pDevice = (PSDevice) pDeviceHandler;
BYTE byTopOFDM = RATE_24M, byTopCCK = RATE_1M;
BYTE ii;

     //Determines the highest basic rate.
     for (ii = RATE_54M; ii >= RATE_6M; ii --) {
         if ( (pDevice->wBasicRate) & ((WORD)(1<<ii)) ) {
             byTopOFDM = ii;
             break;
         }
     }
     pDevice->byTopOFDMBasicRate = byTopOFDM;

     for (ii = RATE_11M;; ii --) {
         if ( (pDevice->wBasicRate) & ((WORD)(1<<ii)) ) {
             byTopCCK = ii;
             break;
         }
         if (ii == RATE_1M)
            break;
     }
     pDevice->byTopCCKBasicRate = byTopCCK;
 }

/*
 * Description: Set NIC Tx Basic Rate
 *
 * Parameters:
 *  In:
 *      pDevice         - The adapter to be set
 *      wBasicRate      - Basic Rate to be set
 *  Out:
 *      none
 *
 * Return Value: TRUE if succeeded; FALSE if failed.
 *
 */
void CARDbAddBasicRate(void *pDeviceHandler, WORD wRateIdx)
{
PSDevice    pDevice = (PSDevice) pDeviceHandler;
WORD wRate = (WORD)(1<<wRateIdx);

    pDevice->wBasicRate |= wRate;

    //Determines the highest basic rate.
    CARDvUpdateBasicTopRate(pDevice);
}

BOOL CARDbIsOFDMinBasicRate(void *pDeviceHandler)
{
PSDevice    pDevice = (PSDevice) pDeviceHandler;
int ii;

    for (ii = RATE_54M; ii >= RATE_6M; ii --) {
        if ((pDevice->wBasicRate) & ((WORD)(1<<ii)))
            return TRUE;
    }
    return FALSE;
}

BYTE CARDbyGetPktType(void *pDeviceHandler)
{
    PSDevice    pDevice = (PSDevice) pDeviceHandler;

    if (pDevice->byBBType == BB_TYPE_11A || pDevice->byBBType == BB_TYPE_11B) {
        return (BYTE)pDevice->byBBType;
    }
    else if (CARDbIsOFDMinBasicRate(pDevice)) {
        return PK_TYPE_11GA;
    }
    else {
        return PK_TYPE_11GB;
    }
}


/*
 * Description: Caculate TSF offset of two TSF input
=======
#include "usbpipe.h"

/* const u16 cw_rxbcntsf_off[MAX_RATE] =
 *   {17, 34, 96, 192, 34, 23, 17, 11, 8, 5, 4, 3};
 */

static const u16 cw_rxbcntsf_off[MAX_RATE] = {
	192, 96, 34, 17, 34, 23, 17, 11, 8, 5, 4, 3
};

int vnt_set_channel(struct vnt_private *priv, u32 connection_channel)
{
	int ret;

	if (connection_channel > CB_MAX_CHANNEL || !connection_channel)
		return -EINVAL;

	/* clear NAV */
	vnt_mac_reg_bits_on(priv, MAC_REG_MACCR, MACCR_CLRNAV);

	/* Set Channel[7] = 0 to tell H/W channel is changing now. */
	vnt_mac_reg_bits_off(priv, MAC_REG_CHANNEL,
			     (BIT(7) | BIT(5) | BIT(4)));

	ret = vnt_control_out(priv, MESSAGE_TYPE_SELECT_CHANNEL,
			      connection_channel, 0, 0, NULL);
	if (ret)
		return ret;

	return vnt_control_out_u8(priv, MESSAGE_REQUEST_MACREG, MAC_REG_CHANNEL,
				  (u8)(connection_channel | 0x80));
}

static const u8 vnt_rspinf_b_short_table[] = {
	0x70, 0x00, 0x00, 0x00, 0x38, 0x00, 0x09, 0x00,
	0x15, 0x00, 0x0a, 0x00, 0x0b, 0x00, 0x0b, 0x80
};

static const u8 vnt_rspinf_b_long_table[] = {
	0x70, 0x00, 0x00, 0x00, 0x38, 0x00, 0x01, 0x00,
	0x15, 0x00, 0x02, 0x00, 0x0b, 0x00, 0x03, 0x80
};

static const u8 vnt_rspinf_a_table[] = {
	0x9b, 0x18, 0x9f, 0x10, 0x9a, 0x0a, 0x9e, 0x08, 0x99,
	0x08, 0x9d, 0x04, 0x98, 0x04, 0x9c, 0x04, 0x9c, 0x04
};

static const u8 vnt_rspinf_gb_table[] = {
	0x8b, 0x1e, 0x8f, 0x16, 0x8a, 0x12, 0x8e, 0x0e, 0x89,
	0x0e, 0x8d, 0x0a, 0x88, 0x0a, 0x8c, 0x0a, 0x8c, 0x0a
};

int vnt_set_rspinf(struct vnt_private *priv, u8 bb_type)
{
	const u8 *data;
	u16 len;
	int ret;

	if (priv->preamble_type) {
		data = vnt_rspinf_b_short_table;
		len = ARRAY_SIZE(vnt_rspinf_b_short_table);
	} else {
		data = vnt_rspinf_b_long_table;
		len = ARRAY_SIZE(vnt_rspinf_b_long_table);
	}

	 /* RSPINF_b_1 to RSPINF_b_11 */
	ret = vnt_control_out(priv, MESSAGE_TYPE_WRITE, MAC_REG_RSPINF_B_1,
			      MESSAGE_REQUEST_MACREG, len, data);
	if (ret)
		return ret;

	if (bb_type == BB_TYPE_11A) {
		data = vnt_rspinf_a_table;
		len = ARRAY_SIZE(vnt_rspinf_a_table);
	} else {
		data = vnt_rspinf_gb_table;
		len = ARRAY_SIZE(vnt_rspinf_gb_table);
	}

	/* RSPINF_a_6 to RSPINF_a_72 */
	return vnt_control_out(priv, MESSAGE_TYPE_WRITE, MAC_REG_RSPINF_A_6,
			       MESSAGE_REQUEST_MACREG, len, data);
}

int vnt_update_ifs(struct vnt_private *priv)
{
	u8 max_min = 0;
	u8 data[4];
	int ret;

	if (priv->packet_type == PK_TYPE_11A) {
		priv->slot = C_SLOT_SHORT;
		priv->sifs = C_SIFS_A;
		priv->difs = C_SIFS_A + 2 * C_SLOT_SHORT;
		max_min = 4;
	} else {
		priv->sifs = C_SIFS_BG;

		if (priv->short_slot_time) {
			priv->slot = C_SLOT_SHORT;
			max_min = 4;
		} else {
			priv->slot = C_SLOT_LONG;
			max_min = 5;
		}

		priv->difs = C_SIFS_BG + 2 * priv->slot;
	}

	priv->eifs = C_EIFS;

	data[0] = (u8)priv->sifs;
	data[1] = (u8)priv->difs;
	data[2] = (u8)priv->eifs;
	data[3] = (u8)priv->slot;

	ret = vnt_control_out(priv, MESSAGE_TYPE_WRITE, MAC_REG_SIFS,
			      MESSAGE_REQUEST_MACREG, 4, &data[0]);
	if (ret)
		return ret;

	max_min |= 0xa0;

	return vnt_control_out(priv, MESSAGE_TYPE_WRITE, MAC_REG_CWMAXMIN0,
			       MESSAGE_REQUEST_MACREG, 1, &max_min);
}

void vnt_update_top_rates(struct vnt_private *priv)
{
	int pos;

	pos = fls(priv->basic_rates & GENMASK(RATE_54M, RATE_6M));
	priv->top_ofdm_basic_rate = pos ? (pos - 1) : RATE_24M;

	pos = fls(priv->basic_rates & GENMASK(RATE_11M, RATE_1M));
	priv->top_cck_basic_rate = pos ? (pos - 1) : RATE_1M;
}

bool vnt_ofdm_min_rate(struct vnt_private *priv)
{
	return priv->basic_rates & GENMASK(RATE_54M, RATE_6M) ? true : false;
}

u8 vnt_get_pkt_type(struct vnt_private *priv)
{
	if (priv->bb_type == BB_TYPE_11A || priv->bb_type == BB_TYPE_11B)
		return (u8)priv->bb_type;
	else if (vnt_ofdm_min_rate(priv))
		return PK_TYPE_11GA;
	return PK_TYPE_11GB;
}

/*
 * Description: Calculate TSF offset of two TSF input
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              Get TSF Offset from RxBCN's TSF and local TSF
 *
 * Parameters:
 *  In:
<<<<<<< HEAD
 *      pDevice         - The adapter to be sync.
 *      qwTSF1          - Rx BCN's TSF
 *      qwTSF2          - Local TSF
=======
 *      rx_rate	- rx rate.
 *      tsf1	- Rx BCN's TSF
 *      tsf2	- Local TSF
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  Out:
 *      none
 *
 * Return Value: TSF Offset value
 *
 */
<<<<<<< HEAD
QWORD CARDqGetTSFOffset (BYTE byRxRate, QWORD qwTSF1, QWORD qwTSF2)
{
    QWORD   qwTSFOffset;
    WORD    wRxBcnTSFOffst = 0;

    HIDWORD(qwTSFOffset) = 0;
    LODWORD(qwTSFOffset) = 0;

    wRxBcnTSFOffst = cwRXBCNTSFOff[byRxRate%MAX_RATE];
    (qwTSF2).u.dwLowDword += (DWORD)(wRxBcnTSFOffst);
    if ((qwTSF2).u.dwLowDword < (DWORD)(wRxBcnTSFOffst)) {
        (qwTSF2).u.dwHighDword++;
    }
    LODWORD(qwTSFOffset) = LODWORD(qwTSF1) - LODWORD(qwTSF2);
    if (LODWORD(qwTSF1) < LODWORD(qwTSF2)) {
        // if borrow needed
        HIDWORD(qwTSFOffset) = HIDWORD(qwTSF1) - HIDWORD(qwTSF2) - 1 ;
    }
    else {
        HIDWORD(qwTSFOffset) = HIDWORD(qwTSF1) - HIDWORD(qwTSF2);
    };
    return (qwTSFOffset);
}



/*
 * Description: Sync. TSF counter to BSS
 *              Get TSF offset and write to HW
 *
 * Parameters:
 *  In:
 *      pDevice         - The adapter to be sync.
 *      qwBSSTimestamp  - Rx BCN's TSF
 *      qwLocalTSF      - Local TSF
 *  Out:
 *      none
 *
 * Return Value: none
 *
 */
void CARDvAdjustTSF(void *pDeviceHandler, BYTE byRxRate,
		    QWORD qwBSSTimestamp, QWORD qwLocalTSF)
{

    PSDevice        pDevice = (PSDevice) pDeviceHandler;
    QWORD           qwTSFOffset;
    DWORD           dwTSFOffset1,dwTSFOffset2;
    BYTE            pbyData[8];

    HIDWORD(qwTSFOffset) = 0;
    LODWORD(qwTSFOffset) = 0;

    qwTSFOffset = CARDqGetTSFOffset(byRxRate, qwBSSTimestamp, qwLocalTSF);
    // adjust TSF
    // HW's TSF add TSF Offset reg
    dwTSFOffset1 = LODWORD(qwTSFOffset);
    dwTSFOffset2 = HIDWORD(qwTSFOffset);


    pbyData[0] = (BYTE)dwTSFOffset1;
    pbyData[1] = (BYTE)(dwTSFOffset1>>8);
    pbyData[2] = (BYTE)(dwTSFOffset1>>16);
    pbyData[3] = (BYTE)(dwTSFOffset1>>24);
    pbyData[4] = (BYTE)dwTSFOffset2;
    pbyData[5] = (BYTE)(dwTSFOffset2>>8);
    pbyData[6] = (BYTE)(dwTSFOffset2>>16);
    pbyData[7] = (BYTE)(dwTSFOffset2>>24);

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_SET_TSFTBTT,
                        MESSAGE_REQUEST_TSF,
                        0,
                        8,
                        pbyData
                        );

}
/*
 * Description: Read NIC TSF counter
 *              Get local TSF counter
 *
 * Parameters:
 *  In:
 *      pDevice         - The adapter to be read
 *  Out:
 *      qwCurrTSF       - Current TSF counter
 *
 * Return Value: TRUE if success; otherwise FALSE
 *
 */
BOOL CARDbGetCurrentTSF(void *pDeviceHandler, PQWORD pqwCurrTSF)
{
    PSDevice    pDevice = (PSDevice) pDeviceHandler;

    LODWORD(*pqwCurrTSF) = LODWORD(pDevice->qwCurrTSF);
    HIDWORD(*pqwCurrTSF) = HIDWORD(pDevice->qwCurrTSF);

    return(TRUE);
}

=======
u64 vnt_get_tsf_offset(u8 rx_rate, u64 tsf1, u64 tsf2)
{
	return tsf1 - tsf2 - (u64)cw_rxbcntsf_off[rx_rate % MAX_RATE];
}

int vnt_adjust_tsf(struct vnt_private *priv, u8 rx_rate,
		   u64 time_stamp, u64 local_tsf)
{
	u64 tsf_offset = 0;
	u8 data[8];

	tsf_offset = vnt_get_tsf_offset(rx_rate, time_stamp, local_tsf);

	data[0] = (u8)tsf_offset;
	data[1] = (u8)(tsf_offset >> 8);
	data[2] = (u8)(tsf_offset >> 16);
	data[3] = (u8)(tsf_offset >> 24);
	data[4] = (u8)(tsf_offset >> 32);
	data[5] = (u8)(tsf_offset >> 40);
	data[6] = (u8)(tsf_offset >> 48);
	data[7] = (u8)(tsf_offset >> 56);

	return vnt_control_out(priv, MESSAGE_TYPE_SET_TSFTBTT,
			       MESSAGE_REQUEST_TSF, 0, 8, data);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Description: Clear NIC TSF counter
 *              Clear local TSF counter
 *
 * Parameters:
 *  In:
<<<<<<< HEAD
 *      pDevice         - The adapter to be read
 *
 * Return Value: TRUE if success; otherwise FALSE
 *
 */
BOOL CARDbClearCurrentTSF(void *pDeviceHandler)
{
    PSDevice    pDevice = (PSDevice) pDeviceHandler;

    MACvRegBitsOn(pDevice,MAC_REG_TFTCTL,TFTCTL_TSFCNTRST);

    LODWORD(pDevice->qwCurrTSF) = 0;
    HIDWORD(pDevice->qwCurrTSF) = 0;

    return(TRUE);
=======
 *      priv	- The adapter to be read
 *
 * Return Value: true if success; otherwise false
 *
 */
bool vnt_clear_current_tsf(struct vnt_private *priv)
{
	vnt_mac_reg_bits_on(priv, MAC_REG_TFTCTL, TFTCTL_TSFCNTRST);

	priv->current_tsf = 0;

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Description: Read NIC TSF counter
 *              Get NEXTTBTT from adjusted TSF and Beacon Interval
 *
 * Parameters:
 *  In:
<<<<<<< HEAD
 *      qwTSF           - Current TSF counter
 *      wbeaconInterval - Beacon Interval
 *  Out:
 *      qwCurrTSF       - Current TSF counter
=======
 *      tsf		- Current TSF counter
 *      beacon_interval - Beacon Interval
 *  Out:
 *      tsf		- Current TSF counter
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Return Value: TSF value of next Beacon
 *
 */
<<<<<<< HEAD
QWORD CARDqGetNextTBTT (QWORD qwTSF, WORD wBeaconInterval)
{

    unsigned int    uLowNextTBTT;
    unsigned int    uHighRemain, uLowRemain;
    unsigned int    uBeaconInterval;

    uBeaconInterval = wBeaconInterval * 1024;
    // Next TBTT = ((local_current_TSF / beacon_interval) + 1 ) * beacon_interval
    uLowNextTBTT = (LODWORD(qwTSF) >> 10) << 10;
    uLowRemain = (uLowNextTBTT) % uBeaconInterval;
    uHighRemain = ((0x80000000 % uBeaconInterval)* 2 * HIDWORD(qwTSF))
                  % uBeaconInterval;
    uLowRemain = (uHighRemain + uLowRemain) % uBeaconInterval;
    uLowRemain = uBeaconInterval - uLowRemain;

    // check if carry when add one beacon interval
    if ((~uLowNextTBTT) < uLowRemain)
        HIDWORD(qwTSF) ++ ;

    LODWORD(qwTSF) = uLowNextTBTT + uLowRemain;

    return (qwTSF);
}


/*
 * Description: Set NIC TSF counter for first Beacon time
 *              Get NEXTTBTT from adjusted TSF and Beacon Interval
 *
 * Parameters:
 *  In:
 *      dwIoBase        - IO Base
 *      wBeaconInterval - Beacon Interval
 *  Out:
 *      none
 *
 * Return Value: none
 *
 */
void CARDvSetFirstNextTBTT(void *pDeviceHandler, WORD wBeaconInterval)
{

    PSDevice        pDevice = (PSDevice) pDeviceHandler;
    QWORD           qwNextTBTT;
    DWORD           dwLoTBTT,dwHiTBTT;
    BYTE            pbyData[8];

    HIDWORD(qwNextTBTT) = 0;
    LODWORD(qwNextTBTT) = 0;
    CARDbClearCurrentTSF(pDevice);
    //CARDbGetCurrentTSF(pDevice, &qwNextTBTT); //Get Local TSF counter
    qwNextTBTT = CARDqGetNextTBTT(qwNextTBTT, wBeaconInterval);
    // Set NextTBTT

    dwLoTBTT = LODWORD(qwNextTBTT);
    dwHiTBTT = HIDWORD(qwNextTBTT);

    pbyData[0] = (BYTE)dwLoTBTT;
    pbyData[1] = (BYTE)(dwLoTBTT>>8);
    pbyData[2] = (BYTE)(dwLoTBTT>>16);
    pbyData[3] = (BYTE)(dwLoTBTT>>24);
    pbyData[4] = (BYTE)dwHiTBTT;
    pbyData[5] = (BYTE)(dwHiTBTT>>8);
    pbyData[6] = (BYTE)(dwHiTBTT>>16);
    pbyData[7] = (BYTE)(dwHiTBTT>>24);

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_SET_TSFTBTT,
                        MESSAGE_REQUEST_TBTT,
                        0,
                        8,
                        pbyData
                        );

    return;
}


/*
 * Description: Sync NIC TSF counter for Beacon time
 *              Get NEXTTBTT and write to HW
 *
 * Parameters:
 *  In:
 *      pDevice         - The adapter to be set
 *      qwTSF           - Current TSF counter
 *      wBeaconInterval - Beacon Interval
 *  Out:
 *      none
 *
 * Return Value: none
 *
 */
void CARDvUpdateNextTBTT(void *pDeviceHandler, QWORD qwTSF,
			 WORD wBeaconInterval)
{
    PSDevice        pDevice = (PSDevice) pDeviceHandler;
    DWORD           dwLoTBTT,dwHiTBTT;
    BYTE            pbyData[8];

    qwTSF = CARDqGetNextTBTT(qwTSF, wBeaconInterval);

    // Set NextTBTT
    dwLoTBTT = LODWORD(qwTSF);
    dwHiTBTT = HIDWORD(qwTSF);

    pbyData[0] = (BYTE)dwLoTBTT;
    pbyData[1] = (BYTE)(dwLoTBTT>>8);
    pbyData[2] = (BYTE)(dwLoTBTT>>16);
    pbyData[3] = (BYTE)(dwLoTBTT>>24);
    pbyData[4] = (BYTE)dwHiTBTT;
    pbyData[5] = (BYTE)(dwHiTBTT>>8);
    pbyData[6] = (BYTE)(dwHiTBTT>>16);
    pbyData[7] = (BYTE)(dwHiTBTT>>24);

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_SET_TSFTBTT,
                        MESSAGE_REQUEST_TBTT,
                        0,
                        8,
                        pbyData
                        );


    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Card:Update Next TBTT[%8xh:%8xh] \n",(int)HIDWORD(qwTSF), (int)LODWORD(qwTSF));

    return;
=======
u64 vnt_get_next_tbtt(u64 tsf, u16 beacon_interval)
{
	u32 beacon_int;

	beacon_int = beacon_interval * 1024;

	/* Next TBTT =
	 *	((local_current_TSF / beacon_interval) + 1) * beacon_interval
	 */
	if (beacon_int) {
		do_div(tsf, beacon_int);
		tsf += 1;
		tsf *= beacon_int;
	}

	return tsf;
}

int vnt_reset_next_tbtt(struct vnt_private *priv, u16 beacon_interval)
{
	u64 next_tbtt = 0;
	u8 data[8];

	vnt_clear_current_tsf(priv);

	next_tbtt = vnt_get_next_tbtt(next_tbtt, beacon_interval);

	data[0] = (u8)next_tbtt;
	data[1] = (u8)(next_tbtt >> 8);
	data[2] = (u8)(next_tbtt >> 16);
	data[3] = (u8)(next_tbtt >> 24);
	data[4] = (u8)(next_tbtt >> 32);
	data[5] = (u8)(next_tbtt >> 40);
	data[6] = (u8)(next_tbtt >> 48);
	data[7] = (u8)(next_tbtt >> 56);

	return vnt_control_out(priv, MESSAGE_TYPE_SET_TSFTBTT,
			       MESSAGE_REQUEST_TBTT, 0, 8, data);
}

int vnt_update_next_tbtt(struct vnt_private *priv, u64 tsf,
			 u16 beacon_interval)
{
	u8 data[8];
	int ret;

	tsf = vnt_get_next_tbtt(tsf, beacon_interval);

	data[0] = (u8)tsf;
	data[1] = (u8)(tsf >> 8);
	data[2] = (u8)(tsf >> 16);
	data[3] = (u8)(tsf >> 24);
	data[4] = (u8)(tsf >> 32);
	data[5] = (u8)(tsf >> 40);
	data[6] = (u8)(tsf >> 48);
	data[7] = (u8)(tsf >> 56);

	ret = vnt_control_out(priv, MESSAGE_TYPE_SET_TSFTBTT,
			      MESSAGE_REQUEST_TBTT, 0, 8, data);
	if (ret)
		return ret;

	dev_dbg(&priv->usb->dev, "%s TBTT: %8llx\n", __func__, tsf);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Description: Turn off Radio power
 *
 * Parameters:
 *  In:
<<<<<<< HEAD
 *      pDevice         - The adapter to be turned off
 *  Out:
 *      none
 *
 * Return Value: TRUE if success; otherwise FALSE
 *
 */
BOOL CARDbRadioPowerOff(void *pDeviceHandler)
{
PSDevice    pDevice = (PSDevice) pDeviceHandler;
BOOL bResult = TRUE;

    //if (pDevice->bRadioOff == TRUE)
    //    return TRUE;

    pDevice->bRadioOff = TRUE;

    switch (pDevice->byRFType) {
        case RF_AL2230:
        case RF_AL2230S:
        case RF_AIROHA7230:
        case RF_VT3226:     //RobertYu:20051111
        case RF_VT3226D0:
        case RF_VT3342A0:   //RobertYu:20060609
            MACvRegBitsOff(pDevice, MAC_REG_SOFTPWRCTL, (SOFTPWRCTL_SWPE2 | SOFTPWRCTL_SWPE3));
            break;
    }

    MACvRegBitsOff(pDevice, MAC_REG_HOSTCR, HOSTCR_RXON);

    BBvSetDeepSleep(pDevice);

    return bResult;
}


=======
 *      priv         - The adapter to be turned off
 *  Out:
 *      none
 *
 * Return Value: true if success; otherwise false
 *
 */
int vnt_radio_power_off(struct vnt_private *priv)
{
	int ret = 0;

	switch (priv->rf_type) {
	case RF_AL2230:
	case RF_AL2230S:
	case RF_VT3226:
	case RF_VT3226D0:
		ret = vnt_mac_reg_bits_off(priv, MAC_REG_SOFTPWRCTL,
					   (SOFTPWRCTL_SWPE2 |
					    SOFTPWRCTL_SWPE3));
		break;
	}

	if (ret)
		goto end;

	ret = vnt_mac_reg_bits_off(priv, MAC_REG_HOSTCR, HOSTCR_RXON);
	if (ret)
		goto end;

	ret = vnt_set_deep_sleep(priv);
	if (ret)
		goto end;

	ret = vnt_mac_reg_bits_on(priv, MAC_REG_GPIOCTL1, GPIO3_INTMD);

end:
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Description: Turn on Radio power
 *
 * Parameters:
 *  In:
<<<<<<< HEAD
 *      pDevice         - The adapter to be turned on
 *  Out:
 *      none
 *
 * Return Value: TRUE if success; otherwise FALSE
 *
 */
BOOL CARDbRadioPowerOn(void *pDeviceHandler)
{
PSDevice    pDevice = (PSDevice) pDeviceHandler;
BOOL bResult = TRUE;


    if ((pDevice->bHWRadioOff == TRUE) || (pDevice->bRadioControlOff == TRUE)) {
        return FALSE;
    }

    //if (pDevice->bRadioOff == FALSE)
    //    return TRUE;

    pDevice->bRadioOff = FALSE;

    BBvExitDeepSleep(pDevice);

    MACvRegBitsOn(pDevice, MAC_REG_HOSTCR, HOSTCR_RXON);

    switch (pDevice->byRFType) {
        case RF_AL2230:
        case RF_AL2230S:
        case RF_AIROHA7230:
        case RF_VT3226:     //RobertYu:20051111
        case RF_VT3226D0:
        case RF_VT3342A0:   //RobertYu:20060609
            MACvRegBitsOn(pDevice, MAC_REG_SOFTPWRCTL, (SOFTPWRCTL_SWPE2 | SOFTPWRCTL_SWPE3));
            break;
    }

    return bResult;
}

void CARDvSetBSSMode(void *pDeviceHandler)
{
    PSDevice    pDevice = (PSDevice) pDeviceHandler;
    // Set BB and packet type at the same time.//{{RobertYu:20050222, AL7230 have two TX PA output, only connet to b/g now
    // so in 11a mode need to set the MAC Reg0x4C to 11b/g mode to turn on PA
    if( (pDevice->byRFType == RF_AIROHA7230 ) && (pDevice->byBBType == BB_TYPE_11A) )
    {
        MACvSetBBType(pDevice, BB_TYPE_11G);
    }
    else
    {
        MACvSetBBType(pDevice, pDevice->byBBType);
    }
    pDevice->byPacketType = CARDbyGetPktType(pDevice);

    if (pDevice->byBBType == BB_TYPE_11A) {
        ControlvWriteByte(pDevice, MESSAGE_REQUEST_BBREG, 0x88, 0x03);
    } else if (pDevice->byBBType == BB_TYPE_11B) {
        ControlvWriteByte(pDevice, MESSAGE_REQUEST_BBREG, 0x88, 0x02);
    } else if (pDevice->byBBType == BB_TYPE_11G) {
        ControlvWriteByte(pDevice, MESSAGE_REQUEST_BBREG, 0x88, 0x08);
    }

    vUpdateIFS(pDevice);
    CARDvSetRSPINF(pDevice, (BYTE)pDevice->byBBType);

    if ( pDevice->byBBType == BB_TYPE_11A ) {
        //request by Jack 2005-04-26
        if (pDevice->byRFType == RF_AIROHA7230) {
            pDevice->abyBBVGA[0] = 0x20;
            ControlvWriteByte(pDevice, MESSAGE_REQUEST_BBREG, 0xE7, pDevice->abyBBVGA[0]);
        }
        pDevice->abyBBVGA[2] = 0x10;
        pDevice->abyBBVGA[3] = 0x10;
    } else {
        //request by Jack 2005-04-26
        if (pDevice->byRFType == RF_AIROHA7230) {
            pDevice->abyBBVGA[0] = 0x1C;
            ControlvWriteByte(pDevice, MESSAGE_REQUEST_BBREG, 0xE7, pDevice->abyBBVGA[0]);
        }
        pDevice->abyBBVGA[2] = 0x0;
        pDevice->abyBBVGA[3] = 0x0;
    }
}

/*
 *
 * Description:
 *    Do Channel Switch defined in 802.11h
 *
 * Parameters:
 *  In:
 *      hDeviceContext - device structure point
 *  Out:
 *      none
 *
 * Return Value: none.
 *
-*/
BOOL
CARDbChannelSwitch (
     void *pDeviceHandler,
     BYTE             byMode,
     BYTE             byNewChannel,
     BYTE             byCount
    )
{
    PSDevice    pDevice = (PSDevice) pDeviceHandler;
    BOOL        bResult = TRUE;

    if (byCount == 0) {
        pDevice->sMgmtObj.uCurrChannel = byNewChannel;
	CARDbSetMediaChannel(pDevice, byNewChannel);

	return bResult;
    }
    pDevice->byChannelSwitchCount = byCount;
    pDevice->byNewChannel = byNewChannel;
    pDevice->bChannelSwitch = TRUE;

    if (byMode == 1) {
        //bResult=CARDbStopTxPacket(pDevice, PKT_TYPE_802_11_ALL);
        pDevice->bStopDataPkt = TRUE;
    }
	return bResult;
}






=======
 *      priv         - The adapter to be turned on
 *  Out:
 *      none
 *
 * Return Value: true if success; otherwise false
 *
 */
int vnt_radio_power_on(struct vnt_private *priv)
{
	int ret = 0;

	ret = vnt_exit_deep_sleep(priv);
	if (ret)
		return ret;

	ret = vnt_mac_reg_bits_on(priv, MAC_REG_HOSTCR, HOSTCR_RXON);
	if (ret)
		return ret;

	switch (priv->rf_type) {
	case RF_AL2230:
	case RF_AL2230S:
	case RF_VT3226:
	case RF_VT3226D0:
		ret = vnt_mac_reg_bits_on(priv, MAC_REG_SOFTPWRCTL,
					  (SOFTPWRCTL_SWPE2 |
					   SOFTPWRCTL_SWPE3));
		if (ret)
			return ret;
	}

	return vnt_mac_reg_bits_off(priv, MAC_REG_GPIOCTL1, GPIO3_INTMD);
}

int vnt_set_bss_mode(struct vnt_private *priv)
{
	int ret;
	unsigned char type = priv->bb_type;
	unsigned char data = 0;
	unsigned char bb_vga_2_3 = 0x00;

	ret = vnt_mac_set_bb_type(priv, type);
	if (ret)
		return ret;

	priv->packet_type = vnt_get_pkt_type(priv);

	if (priv->bb_type == BB_TYPE_11A) {
		data = 0x03;
		bb_vga_2_3 = 0x10;
	} else if (priv->bb_type == BB_TYPE_11B) {
		data = 0x02;
	} else if (priv->bb_type == BB_TYPE_11G) {
		data = 0x08;
	}

	if (data) {
		ret = vnt_control_out_u8(priv, MESSAGE_REQUEST_BBREG,
					 0x88, data);
		if (ret)
			return ret;
	}

	ret = vnt_update_ifs(priv);
	if (ret)
		return ret;

	ret = vnt_set_rspinf(priv, priv->bb_type);
	if (ret)
		return ret;

	priv->bb_vga[2] = bb_vga_2_3;
	priv->bb_vga[3] = bb_vga_2_3;

	return vnt_set_vga_gain_offset(priv, priv->bb_vga[0]);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
