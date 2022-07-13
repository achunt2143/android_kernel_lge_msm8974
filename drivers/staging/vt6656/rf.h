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
 *
 * File: rf.h
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Purpose:
 *
 * Author: Jerry Chen
 *
 * Date: Feb. 19, 2004
 *
 */

#ifndef __RF_H__
#define __RF_H__

<<<<<<< HEAD
#include "ttype.h"
#include "device.h"

/*---------------------  Export Definitions -------------------------*/
//
// Baseband RF pair definition in eeprom (Bits 6..0)
//
=======
#include "device.h"

/* Baseband RF pair definition in eeprom (Bits 6..0) */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define RF_RFMD2959         0x01
#define RF_MAXIMAG          0x02
#define RF_AL2230           0x03
#define RF_GCT5103          0x04
#define RF_UW2451           0x05
#define RF_MAXIMG           0x06
#define RF_MAXIM2829        0x07
#define RF_UW2452           0x08
#define RF_VT3226           0x09
#define RF_AIROHA7230       0x0a
#define RF_UW2453           0x0b
<<<<<<< HEAD
#define RF_VT3226D0         0x0c //RobertYu:20051114
#define RF_VT3342A0         0x0d //RobertYu:20060609
=======
#define RF_VT3226D0         0x0c /* RobertYu:20051114 */
#define RF_VT3342A0         0x0d /* RobertYu:20060609 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define RF_AL2230S          0x0e

#define RF_EMU              0x80
#define RF_MASK             0x7F

<<<<<<< HEAD


/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/
extern const BYTE RFaby11aChannelIndex[200];
/*---------------------  Export Functions  --------------------------*/

BOOL IFRFbWriteEmbeded(PSDevice pDevice, DWORD dwData);
BOOL RFbSetPower(PSDevice pDevice, unsigned int uRATE, unsigned int uCH);

BOOL RFbRawSetPower(
      PSDevice  pDevice,
      BYTE      byPwr,
      unsigned int      uRATE
    );

void RFvRSSITodBm(PSDevice pDevice, BYTE byCurrRSSI, long *pldBm);
void RFbRFTableDownload(PSDevice pDevice);

BOOL s_bVT3226D0_11bLoCurrentAdjust(
      PSDevice    pDevice,
      BYTE        byChannel,
      BOOL        b11bMode
    );
=======
#define VNT_RF_MAX_POWER    0x3f
#define	VNT_RF_REG_LEN      0x17 /* 24 bit length */

int vnt_rf_write_embedded(struct vnt_private *priv, u32 data);
int vnt_rf_setpower(struct vnt_private *priv, struct ieee80211_channel *ch);
void vnt_rf_rssi_to_dbm(struct vnt_private *priv, u8 rssi, long *dbm);
int vnt_rf_table_download(struct vnt_private *priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __RF_H__ */
