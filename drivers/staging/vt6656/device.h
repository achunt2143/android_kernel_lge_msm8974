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
 * File: device.h
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Purpose: MAC Data structure
 *
 * Author: Tevin Chen
 *
 * Date: Mar 17, 1997
 *
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/if_arp.h>
#include <linux/sched.h>
#include <linux/if.h>
#include <linux/rtnetlink.h>//James
#include <linux/proc_fs.h>
#include <linux/inetdevice.h>
#include <linux/reboot.h>
#include <linux/usb.h>
#include <linux/signal.h>
#include <linux/firmware.h>
#include <asm/io.h>
#include <asm/uaccess.h>
=======
#include <linux/bits.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/firmware.h>
#include <linux/suspend.h>
#include <linux/if_arp.h>
#include <linux/wireless.h>
#include <linux/timer.h>
#include <linux/usb.h>
#include <linux/crc32.h>
#include <net/mac80211.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef SIOCETHTOOL
#define DEVICE_ETHTOOL_IOCTL_SUPPORT
#include <linux/ethtool.h>
#else
#undef DEVICE_ETHTOOL_IOCTL_SUPPORT
#endif
<<<<<<< HEAD
/* Include Wireless Extension definition and check version - Jean II */
#include <linux/wireless.h>
#include <net/iw_handler.h>	// New driver API

#ifndef WPA_SUPPLICANT_DRIVER_WEXT_SUPPORT
#define WPA_SUPPLICANT_DRIVER_WEXT_SUPPORT
#endif

//please copy below macro to driver_event.c for API
#define RT_INSMOD_EVENT_FLAG                             0x0101
#define RT_UPDEV_EVENT_FLAG                               0x0102
#define RT_DISCONNECTED_EVENT_FLAG               0x0103
#define RT_WPACONNECTED_EVENT_FLAG             0x0104
#define RT_DOWNDEV_EVENT_FLAG                        0x0105
#define RT_RMMOD_EVENT_FLAG                              0x0106

//
// device specific
//

#include "device_cfg.h"
#include "ttype.h"
#include "80211hdr.h"
#include "tether.h"
#include "wmgr.h"
#include "wcmd.h"
#include "mib.h"
#include "srom.h"
#include "rc4.h"
=======

#define RATE_1M		0
#define RATE_2M		1
#define RATE_5M		2
#define RATE_11M	3
#define RATE_6M		4
#define RATE_9M		5
#define RATE_12M	6
#define RATE_18M	7
#define RATE_24M	8
#define RATE_36M	9
#define RATE_48M	10
#define RATE_54M	11
#define RATE_AUTO	12

#define MAX_RATE			12
#define VNT_B_RATES	(BIT(RATE_1M) | BIT(RATE_2M) |\
			BIT(RATE_5M) | BIT(RATE_11M))

/*
 * device specific
 */

#include "wcmd.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "desc.h"
#include "key.h"
#include "card.h"

<<<<<<< HEAD
/*---------------------  Export Definitions -------------------------*/
#define VNT_USB_VENDOR_ID                     0x160a
#define VNT_USB_PRODUCT_ID                    0x3184

#define MAC_MAX_CONTEXT_REG     (256+128)

#define MAX_MULTICAST_ADDRESS_NUM       32
#define MULTICAST_ADDRESS_LIST_SIZE     (MAX_MULTICAST_ADDRESS_NUM * ETH_ALEN)

//#define OP_MODE_INFRASTRUCTURE  0
//#define OP_MODE_ADHOC           1
//#define OP_MODE_AP              2

#define DUPLICATE_RX_CACHE_LENGTH       5

#define NUM_KEY_ENTRY                   11

#define TX_WEP_NONE                     0
#define TX_WEP_OTF                      1
#define TX_WEP_SW                       2
#define TX_WEP_SWOTP                    3
#define TX_WEP_OTPSW                    4
#define TX_WEP_SW232                    5

#define KEYSEL_WEP40                    0
#define KEYSEL_WEP104                   1
#define KEYSEL_TKIP                     2
#define KEYSEL_CCMP                     3

=======
#define VNT_USB_VENDOR_ID                     0x160a
#define VNT_USB_PRODUCT_ID                    0x3184

#define DEVICE_NAME			"vt6656"
#define DEVICE_FULL_DRV_NAM		"VIA Networking Wireless LAN USB Driver"

#define DEVICE_VERSION			"mac80211"

#define FIRMWARE_VERSION		0x133		/* version 1.51 */
#define FIRMWARE_NAME			"vntwusb.fw"
#define FIRMWARE_CHUNK_SIZE		0x400

#define MAX_UINTS			8
#define OPTION_DEFAULT			{ [0 ... MAX_UINTS - 1] = -1}

#define DUPLICATE_RX_CACHE_LENGTH       5

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define AUTO_FB_NONE            0
#define AUTO_FB_0               1
#define AUTO_FB_1               2

#define FB_RATE0                0
#define FB_RATE1                1

<<<<<<< HEAD
// Antenna Mode
=======
/* Antenna Mode */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ANT_A                   0
#define ANT_B                   1
#define ANT_DIVERSITY           2
#define ANT_RXD_TXA             3
#define ANT_RXD_TXB             4
#define ANT_UNKNOWN             0xFF
#define ANT_TXA                 0
#define ANT_TXB                 1
#define ANT_RXA                 2
#define ANT_RXB                 3

<<<<<<< HEAD

#define MAXCHECKHANGCNT         4

//Packet type
#define TX_PKT_UNI              0x00
#define TX_PKT_MULTI            0x01
#define TX_PKT_BROAD            0x02

#define BB_VGA_LEVEL            4
#define BB_VGA_CHANGE_THRESHOLD 3

#ifndef RUN_AT
#define RUN_AT(x)                       (jiffies+(x))
#endif

// DMA related
#define RESERV_AC0DMA                   4

#define PRIVATE_Message                 0

/*---------------------  Export Types  ------------------------------*/

#define DBG_PRT(l, p, args...) { if (l <= msglevel) printk(p, ##args); }
#define PRINT_K(p, args...) { if (PRIVATE_Message) printk(p, ##args); }

typedef enum __device_msg_level {
	MSG_LEVEL_ERR = 0,            /* Errors causing abnormal operation */
	MSG_LEVEL_NOTICE = 1,         /* Errors needing user notification */
	MSG_LEVEL_INFO = 2,           /* Normal message. */
	MSG_LEVEL_VERBOSE = 3,        /* Will report all trival errors. */
	MSG_LEVEL_DEBUG = 4           /* Only for debug purpose. */
} DEVICE_MSG_LEVEL, *PDEVICE_MSG_LEVEL;

typedef enum __device_init_type {
	DEVICE_INIT_COLD = 0,       /* cold init */
	DEVICE_INIT_RESET,          /* reset init or Dx to D0 power remain */
	DEVICE_INIT_DXPL            /* Dx to D0 power lost init */
} DEVICE_INIT_TYPE, *PDEVICE_INIT_TYPE;

//USB

//
// Enum of context types for SendPacket
//
typedef enum _CONTEXT_TYPE {
    CONTEXT_DATA_PACKET = 1,
    CONTEXT_MGMT_PACKET
} CONTEXT_TYPE;

// RCB (Receive Control Block)
typedef struct _RCB
{
    void *Next;
    signed long                    Ref;
    void *pDevice;
    struct urb              *pUrb;
    SRxMgmtPacket           sMngPacket;
    struct sk_buff*         skb;
    BOOL                    bBoolInUse;

} RCB, *PRCB;

// used to track bulk out irps
typedef struct _USB_SEND_CONTEXT {
    void *pDevice;
    struct sk_buff *pPacket;
    struct urb      *pUrb;
    unsigned int            uBufLen;
    CONTEXT_TYPE    Type;
    SEthernetHeader sEthHeader;
    void *Next;
    BOOL            bBoolInUse;
    unsigned char           Data[MAX_TOTAL_SIZE_WITH_ALL_HEADERS];
} USB_SEND_CONTEXT, *PUSB_SEND_CONTEXT;

/* structure got from configuration file as user-desired default settings */
typedef struct _DEFAULT_CONFIG {
	signed int    ZoneType;
	signed int    eConfigMode;
	signed int    eAuthenMode;        /* open/wep/wpa */
	signed int    bShareKeyAlgorithm; /* open-open/{open,wep}-sharekey */
	signed int    keyidx;             /* wepkey index */
	signed int    eEncryptionStatus;
} DEFAULT_CONFIG, *PDEFAULT_CONFIG;

//
// Structure to keep track of usb interrupt packets
//
typedef struct {
    unsigned int            uDataLen;
    PBYTE           pDataBuf;
//    struct urb      *pUrb;
    BOOL            bInUse;
} INT_BUFFER, *PINT_BUFFER;

//0:11A 1:11B 2:11G
typedef enum _VIA_BB_TYPE
{
    BB_TYPE_11A = 0,
    BB_TYPE_11B,
    BB_TYPE_11G
} VIA_BB_TYPE, *PVIA_BB_TYPE;

//0:11a,1:11b,2:11gb(only CCK in BasicRate),3:11ga(OFDM in Basic Rate)
typedef enum _VIA_PKT_TYPE
{
    PK_TYPE_11A = 0,
    PK_TYPE_11B,
    PK_TYPE_11GB,
    PK_TYPE_11GA
} VIA_PKT_TYPE, *PVIA_PKT_TYPE;

//++ NDIS related

typedef enum __DEVICE_NDIS_STATUS {
    STATUS_SUCCESS = 0,
    STATUS_FAILURE,
    STATUS_RESOURCES,
    STATUS_PENDING,
} DEVICE_NDIS_STATUS, *PDEVICE_NDIS_STATUS;

#define MAX_BSSIDINFO_4_PMKID   16
#define MAX_PMKIDLIST           5
//Flags for PMKID Candidate list structure
#define NDIS_802_11_PMKID_CANDIDATE_PREAUTH_ENABLED	0x01

// PMKID Structures
typedef unsigned char   NDIS_802_11_PMKID_VALUE[16];


typedef enum _NDIS_802_11_WEP_STATUS
{
    Ndis802_11WEPEnabled,
    Ndis802_11Encryption1Enabled = Ndis802_11WEPEnabled,
    Ndis802_11WEPDisabled,
    Ndis802_11EncryptionDisabled = Ndis802_11WEPDisabled,
    Ndis802_11WEPKeyAbsent,
    Ndis802_11Encryption1KeyAbsent = Ndis802_11WEPKeyAbsent,
    Ndis802_11WEPNotSupported,
    Ndis802_11EncryptionNotSupported = Ndis802_11WEPNotSupported,
    Ndis802_11Encryption2Enabled,
    Ndis802_11Encryption2KeyAbsent,
    Ndis802_11Encryption3Enabled,
    Ndis802_11Encryption3KeyAbsent
} NDIS_802_11_WEP_STATUS, *PNDIS_802_11_WEP_STATUS,
  NDIS_802_11_ENCRYPTION_STATUS, *PNDIS_802_11_ENCRYPTION_STATUS;


typedef enum _NDIS_802_11_STATUS_TYPE
{
    Ndis802_11StatusType_Authentication,
    Ndis802_11StatusType_MediaStreamMode,
    Ndis802_11StatusType_PMKID_CandidateList,
    Ndis802_11StatusTypeMax    // not a real type, defined as an upper bound
} NDIS_802_11_STATUS_TYPE, *PNDIS_802_11_STATUS_TYPE;

//Added new types for PMKID Candidate lists.
typedef struct _PMKID_CANDIDATE {
    NDIS_802_11_MAC_ADDRESS BSSID;
    unsigned long Flags;
} PMKID_CANDIDATE, *PPMKID_CANDIDATE;


typedef struct _BSSID_INFO
{
    NDIS_802_11_MAC_ADDRESS BSSID;
    NDIS_802_11_PMKID_VALUE PMKID;
} BSSID_INFO, *PBSSID_INFO;

typedef struct tagSPMKID {
    unsigned long Length;
    unsigned long BSSIDInfoCount;
    BSSID_INFO BSSIDInfo[MAX_BSSIDINFO_4_PMKID];
} SPMKID, *PSPMKID;

typedef struct tagSPMKIDCandidateEvent {
    NDIS_802_11_STATUS_TYPE     StatusType;
	unsigned long Version;       /* Version of the structure */
	unsigned long NumCandidates; /* No. of pmkid candidates */
    PMKID_CANDIDATE CandidateList[MAX_PMKIDLIST];
} SPMKIDCandidateEvent, *PSPMKIDCandidateEvent;

//--

//++ 802.11h related
#define MAX_QUIET_COUNT     8

typedef struct tagSQuietControl {
    BOOL        bEnable;
    DWORD       dwStartTime;
    BYTE        byPeriod;
    WORD        wDuration;
} SQuietControl, *PSQuietControl;

//--


// The receive duplicate detection cache entry
typedef struct tagSCacheEntry{
    WORD        wFmSequence;
    BYTE        abyAddr2[ETH_ALEN];
    WORD        wFrameCtl;
} SCacheEntry, *PSCacheEntry;

typedef struct tagSCache{
/* The receive cache is updated circularly.  The next entry to be written is
 * indexed by the "InPtr".
*/
	unsigned int uInPtr; /* Place to use next */
    SCacheEntry     asCacheEntry[DUPLICATE_RX_CACHE_LENGTH];
} SCache, *PSCache;

#define CB_MAX_RX_FRAG                 64
// DeFragment Control Block, used for collecting fragments prior to reassembly
typedef struct tagSDeFragControlBlock
{
    WORD            wSequence;
    WORD            wFragNum;
    BYTE            abyAddr2[ETH_ALEN];
	unsigned int            uLifetime;
    struct sk_buff* skb;
    PBYTE           pbyRxBuffer;
    unsigned int            cbFrameLength;
    BOOL            bInUse;
} SDeFragControlBlock, *PSDeFragControlBlock;



//flags for options
#define     DEVICE_FLAGS_UNPLUG          0x00000001UL
#define     DEVICE_FLAGS_PREAMBLE_TYPE   0x00000002UL
#define     DEVICE_FLAGS_OP_MODE         0x00000004UL
#define     DEVICE_FLAGS_PS_MODE         0x00000008UL
#define		DEVICE_FLAGS_80211h_MODE	 0x00000010UL

//flags for driver status
#define     DEVICE_FLAGS_OPENED          0x00010000UL
#define     DEVICE_FLAGS_WOL_ENABLED     0x00080000UL
//flags for capbilities
#define     DEVICE_FLAGS_TX_ALIGN        0x01000000UL
#define     DEVICE_FLAGS_HAVE_CAM        0x02000000UL
#define     DEVICE_FLAGS_FLOW_CTRL       0x04000000UL

//flags for MII status
#define     DEVICE_LINK_FAIL             0x00000001UL
#define     DEVICE_SPEED_10              0x00000002UL
#define     DEVICE_SPEED_100             0x00000004UL
#define     DEVICE_SPEED_1000            0x00000008UL
#define     DEVICE_DUPLEX_FULL           0x00000010UL
#define     DEVICE_AUTONEG_ENABLE        0x00000020UL
#define     DEVICE_FORCED_BY_EEPROM      0x00000040UL
//for device_set_media_duplex
#define     DEVICE_LINK_CHANGE           0x00000001UL


typedef struct __device_opt {
    int         nRxDescs0;    //Number of RX descriptors0
    int         nTxDescs0;    //Number of TX descriptors 0, 1, 2, 3
    int         rts_thresh;   //rts threshold
    int         frag_thresh;
    int         OpMode;
    int         data_rate;
    int         channel_num;
    int         short_retry;
    int         long_retry;
    int         bbp_type;
    u32         flags;
} OPTIONS, *POPTIONS;


typedef struct __device_info {

// netdev
	struct usb_device*          usb;
    struct net_device*          dev;
    struct net_device_stats     stats;

    const struct firmware	*firmware;

    OPTIONS                     sOpts;

	struct tasklet_struct       CmdWorkItem;
	struct tasklet_struct       EventWorkItem;
	struct tasklet_struct       ReadWorkItem;
	struct tasklet_struct       RxMngWorkItem;

    u32                         rx_buf_sz;
    int                         multicast_limit;
    BYTE                        byRxMode;

    spinlock_t                  lock;

    u32                         rx_bytes;

    BYTE                        byRevId;

    u32                         flags;
    unsigned long                       Flags;

    SCache                      sDupRxCache;

    SDeFragControlBlock         sRxDFCB[CB_MAX_RX_FRAG];
    unsigned int                        cbDFCB;
    unsigned int                        cbFreeDFCB;
    unsigned int                        uCurrentDFCBIdx;

    // +++USB

    struct urb                  *pControlURB;
    struct urb                  *pInterruptURB;
	struct usb_ctrlrequest      sUsbCtlRequest;

    unsigned int                        int_interval;
    //
    // Variables to track resources for the BULK In Pipe
    //
    PRCB                        pRCBMem;
    PRCB                        apRCB[CB_MAX_RX_DESC];
    unsigned int                        cbRD;
    PRCB                        FirstRecvFreeList;
    PRCB                        LastRecvFreeList;
    unsigned int                        NumRecvFreeList;
    PRCB                        FirstRecvMngList;
    PRCB                        LastRecvMngList;
    unsigned int                        NumRecvMngList;
    BOOL                        bIsRxWorkItemQueued;
    BOOL                        bIsRxMngWorkItemQueued;
	unsigned long ulRcvRefCount; /* packets that have not returned back */

    //
    //  Variables to track resources for the BULK Out Pipe
    //

    PUSB_SEND_CONTEXT           apTD[CB_MAX_TX_DESC];
    unsigned int                        cbTD;

    //
    //  Variables to track resources for the Interript In Pipe
    //
    INT_BUFFER                  intBuf;
    BOOL                        fKillEventPollingThread;
    BOOL                        bEventAvailable;


  //default config from file by user setting
    DEFAULT_CONFIG    config_file;


    //
    // Statistic for USB
    // protect with spinlock
    unsigned long                       ulBulkInPosted;
    unsigned long                       ulBulkInError;
    unsigned long                       ulBulkInContCRCError;
    unsigned long                       ulBulkInBytesRead;

    unsigned long                       ulBulkOutPosted;
    unsigned long                       ulBulkOutError;
    unsigned long                       ulBulkOutContCRCError;
    unsigned long                       ulBulkOutBytesWrite;

    unsigned long                       ulIntInPosted;
    unsigned long                       ulIntInError;
    unsigned long                       ulIntInContCRCError;
    unsigned long                       ulIntInBytesRead;


    // Version control
    WORD                        wFirmwareVersion;
    BYTE                        byLocalID;
    BYTE                        byRFType;
    BYTE                        byBBRxConf;


    BYTE                        byZoneType;
    BOOL                        bZoneRegExist;

    BYTE                        byOriginalZonetype;

    BOOL                        bLinkPass;          // link status: OK or fail
    BYTE                        abyCurrentNetAddr[ETH_ALEN];
    BYTE                        abyPermanentNetAddr[ETH_ALEN];
    // SW network address
	/* u8 abySoftwareNetAddr[ETH_ALEN]; */
    BOOL                        bExistSWNetAddr;

    // Adapter statistics
    SStatCounter                scStatistic;
    // 802.11 counter
    SDot11Counters              s802_11Counter;

    //
    // Maintain statistical debug info.
    //
    unsigned long                       packetsReceived;
    unsigned long                       packetsReceivedDropped;
    unsigned long                       packetsReceivedOverflow;
    unsigned long                       packetsSent;
    unsigned long                       packetsSentDropped;
    unsigned long                       SendContextsInUse;
    unsigned long                       RcvBuffersInUse;


    // 802.11 management
    SMgmtObject                 sMgmtObj;

    QWORD                       qwCurrTSF;
    unsigned int                        cbBulkInMax;
    BOOL                        bPSRxBeacon;

    // 802.11 MAC specific
    unsigned int                        uCurrRSSI;
    BYTE                        byCurrSQ;


    //Antenna Diversity
    BOOL                        bTxRxAntInv;
    DWORD                       dwRxAntennaSel;
    DWORD                       dwTxAntennaSel;
    BYTE                        byAntennaCount;
    BYTE                        byRxAntennaMode;
    BYTE                        byTxAntennaMode;
    BYTE                        byRadioCtl;
    BYTE                        bHWRadioOff;

    //SQ3 functions for antenna diversity
    struct timer_list           TimerSQ3Tmax1;
    struct timer_list           TimerSQ3Tmax2;
    struct timer_list           TimerSQ3Tmax3;

    BOOL                        bDiversityRegCtlON;
    BOOL                        bDiversityEnable;
    unsigned long                       ulDiversityNValue;
    unsigned long                       ulDiversityMValue;
    BYTE                        byTMax;
    BYTE                        byTMax2;
    BYTE                        byTMax3;
    unsigned long                       ulSQ3TH;

    unsigned long                       uDiversityCnt;
    BYTE                        byAntennaState;
    unsigned long                       ulRatio_State0;
    unsigned long                       ulRatio_State1;
    unsigned long                       ulSQ3_State0;
    unsigned long                       ulSQ3_State1;

    unsigned long                       aulSQ3Val[MAX_RATE];
    unsigned long                       aulPktNum[MAX_RATE];

	/* IFS & Cw */
	unsigned int uSIFS;  /* Current SIFS */
	unsigned int uDIFS;  /* Current DIFS */
	unsigned int uEIFS;  /* Current EIFS */
	unsigned int uSlot;  /* Current SlotTime */
	unsigned int uCwMin; /* Current CwMin */
	unsigned int uCwMax; /* CwMax is fixed on 1023 */

    // PHY parameter
    BYTE                        bySIFS;
    BYTE                        byDIFS;
    BYTE                        byEIFS;
    BYTE                        bySlot;
    BYTE                        byCWMaxMin;

    // Rate
    VIA_BB_TYPE                 byBBType; //0: 11A, 1:11B, 2:11G
    VIA_PKT_TYPE                byPacketType; //0:11a,1:11b,2:11gb(only CCK in BasicRate),3:11ga(OFDM in Basic Rate)
    WORD                        wBasicRate;
    BYTE                        byACKRate;
    BYTE                        byTopOFDMBasicRate;
    BYTE                        byTopCCKBasicRate;


    DWORD                       dwAotoRateTxOkCnt;
    DWORD                       dwAotoRateTxFailCnt;
    DWORD                       dwErrorRateThreshold[13];
    DWORD                       dwTPTable[MAX_RATE];
    BYTE                        abyEEPROM[EEP_MAX_CONTEXT_SIZE];  //DWORD alignment

    BYTE                        byMinChannel;
    BYTE                        byMaxChannel;
    unsigned int                        uConnectionRate;

    BYTE                        byPreambleType;
    BYTE                        byShortPreamble;
    // CARD_PHY_TYPE
    BYTE                        eConfigPHYMode;

    // For RF Power table
    BYTE                        byCCKPwr;
    BYTE                        byOFDMPwrG;
    BYTE                        byOFDMPwrA;
    BYTE                        byCurPwr;
    BYTE                        abyCCKPwrTbl[14];
    BYTE                        abyOFDMPwrTbl[14];
    BYTE                        abyOFDMAPwrTbl[42];

    WORD                        wCurrentRate;
    WORD                        wRTSThreshold;
    WORD                        wFragmentationThreshold;
    BYTE                        byShortRetryLimit;
    BYTE                        byLongRetryLimit;
    CARD_OP_MODE                eOPMode;
    BOOL                        bBSSIDFilter;
    WORD                        wMaxTransmitMSDULifetime;
    BYTE                        abyBSSID[ETH_ALEN];
    BYTE                        abyDesireBSSID[ETH_ALEN];
    WORD                        wCTSDuration;       // update while speed change
    WORD                        wACKDuration;       // update while speed change
    WORD                        wRTSTransmitLen;    // update while speed change
    BYTE                        byRTSServiceField;  // update while speed change
    BYTE                        byRTSSignalField;   // update while speed change

    DWORD                       dwMaxReceiveLifetime;       // dot11MaxReceiveLifetime

    BOOL                        bCCK;
    BOOL                        bEncryptionEnable;
    BOOL                        bLongHeader;
    BOOL                        bSoftwareGenCrcErr;
    BOOL                        bShortSlotTime;
    BOOL                        bProtectMode;
    BOOL                        bNonERPPresent;
    BOOL                        bBarkerPreambleMd;

    BYTE                        byERPFlag;
    WORD                        wUseProtectCntDown;

    BOOL                        bRadioControlOff;
    BOOL                        bRadioOff;

    // Power save
    BOOL                        bEnablePSMode;
    WORD                        wListenInterval;
    BOOL                        bPWBitOn;
    WMAC_POWER_MODE             ePSMode;
    unsigned long                       ulPSModeWaitTx;
    BOOL                        bPSModeTxBurst;

    // Beacon releated
    WORD                    wSeqCounter;
    BOOL                    bBeaconBufReady;
    BOOL                    bBeaconSent;
    BOOL                    bFixRate;
    BYTE                    byCurrentCh;
    unsigned int                    uScanTime;

    CMD_STATE               eCommandState;

    CMD_CODE                eCommand;
    BOOL                    bBeaconTx;
    BYTE                    byScanBBType;

    BOOL                    bStopBeacon;
    BOOL                    bStopDataPkt;
    BOOL                    bStopTx0Pkt;
    unsigned int                    uAutoReConnectTime;
    unsigned int                    uIsroamingTime;

    // 802.11 counter

    CMD_ITEM                eCmdQueue[CMD_Q_SIZE];
    unsigned int                    uCmdDequeueIdx;
    unsigned int                    uCmdEnqueueIdx;
    unsigned int                    cbFreeCmdQueue;
    BOOL                    bCmdRunning;
    BOOL                    bCmdClear;
    BOOL                    bNeedRadioOFF;

    BOOL                    bEnableRoaming;
    BOOL                    bIsRoaming;
    BOOL                    bFastRoaming;
    BYTE                    bSameBSSMaxNum;
    BYTE                    bSameBSSCurNum;
    BOOL                    bRoaming;
    BOOL                    b11hEable;
    unsigned long                   ulTxPower;

    // Encryption
    NDIS_802_11_WEP_STATUS  eEncryptionStatus;
    BOOL                    bTransmitKey;

//mike add :save old Encryption
    NDIS_802_11_WEP_STATUS  eOldEncryptionStatus;

    SKeyManagement          sKey;
    DWORD                   dwIVCounter;


    RC4Ext                  SBox;
    BYTE                    abyPRNG[WLAN_WEPMAX_KEYLEN+3];
    BYTE                    byKeyIndex;

    BOOL                    bAES;
    BYTE                    byCntMeasure;

    unsigned int                    uKeyLength;
    BYTE                    abyKey[WLAN_WEP232_KEYLEN];

    // for AP mode
    unsigned int                    uAssocCount;
    BOOL                    bMoreData;

    // QoS
    BOOL                    bGrpAckPolicy;


    BYTE                    byAutoFBCtrl;

    BOOL                    bTxMICFail;
    BOOL                    bRxMICFail;


    // For Update BaseBand VGA Gain Offset
    BOOL                    bUpdateBBVGA;
    unsigned int                    uBBVGADiffCount;
    BYTE                    byBBVGANew;
    BYTE                    byBBVGACurrent;
    BYTE                    abyBBVGA[BB_VGA_LEVEL];
    signed long                    ldBmThreshold[BB_VGA_LEVEL];

    BYTE                    byBBPreEDRSSI;
    BYTE                    byBBPreEDIndex;


    BOOL                    bRadioCmd;
    DWORD                   dwDiagRefCount;

    // For FOE Tuning
    BYTE                    byFOETuning;

    // For Auto Power Tunning

    BYTE                    byAutoPwrTunning;

    // BaseBand Loopback Use
    BYTE                    byBBCR4d;
    BYTE                    byBBCRc9;
    BYTE                    byBBCR88;
    BYTE                    byBBCR09;

    // command timer
    struct timer_list       sTimerCommand;

     struct timer_list       sTimerTxData;
     unsigned long                       nTxDataTimeCout;
     BOOL  fTxDataInSleep;
     BOOL  IsTxDataTrigger;

    BOOL  fWPA_Authened;           //is WPA/WPA-PSK or WPA2/WPA2-PSK authen??
    BYTE            byReAssocCount;   //mike add:re-association retry times!
    BYTE            byLinkWaitCount;

    SEthernetHeader         sTxEthHeader;
    SEthernetHeader         sRxEthHeader;
    BYTE                    abyBroadcastAddr[ETH_ALEN];
    BYTE                    abySNAP_RFC1042[ETH_ALEN];
    BYTE                    abySNAP_Bridgetunnel[ETH_ALEN];

    // Pre-Authentication & PMK cache
    SPMKID                  gsPMKID;
    SPMKIDCandidateEvent    gsPMKIDCandidate;


    // for 802.11h
    BOOL                    b11hEnable;

    BOOL                    bChannelSwitch;
    BYTE                    byNewChannel;
    BYTE                    byChannelSwitchCount;

    //WPA supplicant daemon
	struct net_device       *wpadev;
	BOOL                    bWPADEVUp;
    struct sk_buff          *skb;
    //--

#ifdef WPA_SUPPLICANT_DRIVER_WEXT_SUPPORT
        BOOL                 bwextstep0;
        BOOL                 bwextstep1;
        BOOL                 bwextstep2;
        BOOL                 bwextstep3;
        BOOL                 bWPASuppWextEnabled;
#endif

#ifdef HOSTAP
    // user space daemon: hostapd, is used for HOSTAP
	BOOL                    bEnableHostapd;
	BOOL                    bEnable8021x;
	BOOL                    bEnableHostWEP;
	struct net_device       *apdev;
	int (*tx_80211)(struct sk_buff *skb, struct net_device *dev);
#endif
    unsigned int                    uChannel;

	struct iw_statistics	wstats;		// wireless stats
    BOOL                    bCommit;

} DEVICE_INFO, *PSDevice;




#define EnqueueRCB(_Head, _Tail, _RCB)                  \
{                                                       \
    if (!_Head) {                                       \
        _Head = _RCB;                                   \
    }                                                   \
    else {                                              \
        _Tail->Next = _RCB;                             \
    }                                                   \
    _RCB->Next = NULL;                                  \
    _Tail = _RCB;                                       \
}

#define DequeueRCB(Head, Tail)                          \
{                                                       \
    PRCB   RCB = Head;                                  \
    if (!RCB->Next) {                                   \
        Tail = NULL;                                    \
    }                                                   \
    Head = RCB->Next;                                   \
}


#define ADD_ONE_WITH_WRAP_AROUND(uVar, uModulo) {   \
    if ((uVar) >= ((uModulo) - 1))                  \
        (uVar) = 0;                                 \
    else                                            \
        (uVar)++;                                   \
}


#define fMP_RESET_IN_PROGRESS               0x00000001
#define fMP_DISCONNECTED                    0x00000002
#define fMP_HALT_IN_PROGRESS                0x00000004
#define fMP_SURPRISE_REMOVED                0x00000008
#define fMP_RECV_LOOKASIDE                  0x00000010
#define fMP_INIT_IN_PROGRESS                0x00000020
#define fMP_SEND_SIDE_RESOURCE_ALLOCATED    0x00000040
#define fMP_RECV_SIDE_RESOURCE_ALLOCATED    0x00000080
#define fMP_POST_READS                      0x00000100
#define fMP_POST_WRITES                     0x00000200
#define fMP_CONTROL_READS                   0x00000400
#define fMP_CONTROL_WRITES                  0x00000800

#define MP_SET_FLAG(_M, _F)             ((_M)->Flags |= (_F))
#define MP_CLEAR_FLAG(_M, _F)            ((_M)->Flags &= ~(_F))
#define MP_TEST_FLAGS(_M, _F)            (((_M)->Flags & (_F)) == (_F))

#define MP_IS_READY(_M)        (((_M)->Flags & \
                                 (fMP_DISCONNECTED | fMP_RESET_IN_PROGRESS | fMP_HALT_IN_PROGRESS | fMP_INIT_IN_PROGRESS | fMP_SURPRISE_REMOVED)) == 0)

/*---------------------  Export Functions  --------------------------*/

/* BOOL device_dma0_xmit(PSDevice pDevice, struct sk_buff *skb,
 *                       unsigned int uNodeIndex);
 */
BOOL device_alloc_frag_buf(PSDevice pDevice, PSDeFragControlBlock pDeF);
=======
#define BB_VGA_LEVEL            4
#define BB_VGA_CHANGE_THRESHOLD 3

#define EEP_MAX_CONTEXT_SIZE    256

/* Contents in the EEPROM */
#define EEP_OFS_PAR		0x0
#define EEP_OFS_ANTENNA		0x17
#define EEP_OFS_RADIOCTL	0x18
#define EEP_OFS_RFTYPE		0x1b
#define EEP_OFS_MINCHANNEL	0x1c
#define EEP_OFS_MAXCHANNEL	0x1d
#define EEP_OFS_SIGNATURE	0x1e
#define EEP_OFS_ZONETYPE	0x1f
#define EEP_OFS_RFTABLE		0x20
#define EEP_OFS_PWR_CCK		0x20
#define EEP_OFS_SETPT_CCK	0x21
#define EEP_OFS_PWR_OFDMG	0x23

#define EEP_OFS_CALIB_TX_IQ	0x24
#define EEP_OFS_CALIB_TX_DC	0x25
#define EEP_OFS_CALIB_RX_IQ	0x26

#define EEP_OFS_MAJOR_VER	0x2e
#define EEP_OFS_MINOR_VER	0x2f

#define EEP_OFS_CCK_PWR_TBL	0x30
#define EEP_OFS_OFDM_PWR_TBL	0x40
#define EEP_OFS_OFDMA_PWR_TBL	0x50

/* Bits in EEP_OFS_ANTENNA */
#define EEP_ANTENNA_MAIN	BIT(0)
#define EEP_ANTENNA_AUX		BIT(1)
#define EEP_ANTINV		BIT(2)

/* Bits in EEP_OFS_RADIOCTL */
#define EEP_RADIOCTL_ENABLE	BIT(7)

/* control commands */
#define MESSAGE_TYPE_READ		0x1
#define MESSAGE_TYPE_WRITE		0x0
#define MESSAGE_TYPE_LOCK_OR		0x2
#define MESSAGE_TYPE_LOCK_AND		0x3
#define MESSAGE_TYPE_WRITE_MASK		0x4
#define MESSAGE_TYPE_CARDINIT		0x5
#define MESSAGE_TYPE_INIT_RSP		0x6
#define MESSAGE_TYPE_MACSHUTDOWN	0x7
#define MESSAGE_TYPE_SETKEY		0x8
#define MESSAGE_TYPE_CLRKEYENTRY	0x9
#define MESSAGE_TYPE_WRITE_MISCFF	0xa
#define MESSAGE_TYPE_SET_ANTMD		0xb
#define MESSAGE_TYPE_SELECT_CHANNEL	0xc
#define MESSAGE_TYPE_SET_TSFTBTT	0xd
#define MESSAGE_TYPE_SET_SSTIFS		0xe
#define MESSAGE_TYPE_CHANGE_BBTYPE	0xf
#define MESSAGE_TYPE_DISABLE_PS		0x10
#define MESSAGE_TYPE_WRITE_IFRF		0x11

/* command read/write(index) */
#define MESSAGE_REQUEST_MEM		0x1
#define MESSAGE_REQUEST_BBREG		0x2
#define MESSAGE_REQUEST_MACREG		0x3
#define MESSAGE_REQUEST_EEPROM		0x4
#define MESSAGE_REQUEST_TSF		0x5
#define MESSAGE_REQUEST_TBTT		0x6
#define MESSAGE_REQUEST_BBAGC		0x7
#define MESSAGE_REQUEST_VERSION		0x8
#define MESSAGE_REQUEST_RF_INIT		0x9
#define MESSAGE_REQUEST_RF_INIT2	0xa
#define MESSAGE_REQUEST_RF_CH0		0xb
#define MESSAGE_REQUEST_RF_CH1		0xc
#define MESSAGE_REQUEST_RF_CH2		0xd

/* USB registers */
#define USB_REG4			0x604

#define DEVICE_INIT_COLD	0x0 /* cold init */
#define DEVICE_INIT_RESET	0x1 /* reset init or Dx to D0 power remain */
#define DEVICE_INIT_DXPL	0x2 /* Dx to D0 power lost init */

/* Device init */
struct vnt_cmd_card_init {
	u8 init_class;
	u8 exist_sw_net_addr;
	u8 sw_net_addr[6];
	u8 short_retry_limit;
	u8 long_retry_limit;
};

struct vnt_rsp_card_init {
	u8 status;
	u8 net_addr[6];
	u8 rf_type;
	u8 min_channel;
	u8 max_channel;
};

/* USB */

/*
 * Enum of context types for SendPacket
 */
enum {
	CONTEXT_DATA_PACKET = 0,
	CONTEXT_BEACON_PACKET
};

struct vnt_rx_header {
	u32 wbk_status;
	u8 rx_sts;
	u8 rx_rate;
	u16 pay_load_len;
} __packed;

struct vnt_rx_tail {
	__le64 tsf_time;
	u8 sq;
	u8 new_rsr;
	u8 rssi;
	u8 rsr;
	u8 sq_3;
} __packed;

/* RCB (Receive Control Block) */
struct vnt_rcb {
	void *priv;
	struct urb *urb;
	struct sk_buff *skb;
};

/* used to track bulk out irps */
struct vnt_usb_send_context {
	void *priv;
	struct sk_buff *skb;
	void *tx_buffer;
	u32 frame_len;
	u16 tx_hdr_size;
	u16 tx_rate;
	u8 type;
	u8 pkt_no;
	u8 pkt_type;
	bool in_use;
};

/*
 * Structure to keep track of USB interrupt packets
 */
struct vnt_interrupt_buffer {
	u8 *data_buf;
};

/* flags for options */
#define DEVICE_FLAGS_UNPLUG		0
#define DEVICE_FLAGS_DISCONNECTED	1

struct vnt_private {
	/* mac80211 */
	struct ieee80211_hw *hw;
	struct ieee80211_vif *vif;
	u8 mac_hw;
	/* netdev */
	struct usb_device *usb;
	struct usb_interface *intf;

	u64 tsf_time;

	u32 rx_buf_sz;
	int mc_list_count;

	spinlock_t lock;		/* prepare tx USB URB */
	struct mutex usb_lock;		/* USB control messages */

	unsigned long flags;

	/* USB */
	struct urb *interrupt_urb;
	u32 int_interval;

	/* Variables to track resources for the BULK In Pipe */
	struct vnt_rcb *rcb[CB_MAX_RX_DESC];
	u32 num_rcb;

	/* Variables to track resources for the BULK Out Pipe */
	struct vnt_usb_send_context *tx_context[CB_MAX_TX_DESC];
	struct usb_anchor tx_submitted;
	u32 num_tx_context;

	/* Variables to track resources for the Interrupt In Pipe */
	struct vnt_interrupt_buffer int_buf;

	/* Version control */
	u16 firmware_version;
	u8 local_id;
	u8 rf_type;
	u8 bb_rx_conf;

	struct vnt_cmd_card_init init_command;
	struct vnt_rsp_card_init init_response;
	u8 current_net_addr[ETH_ALEN] __aligned(2);
	u8 permanent_net_addr[ETH_ALEN] __aligned(2);

	u8 exist_sw_net_addr;

	u64 current_tsf;

	/* 802.11 MAC specific */
	u32 current_rssi;

	/* Antenna Diversity */
	int tx_rx_ant_inv;
	u32 rx_antenna_sel;
	u8 rx_antenna_mode;
	u8 tx_antenna_mode;
	u8 radio_ctl;

	/* IFS & Cw */
	u32 sifs;  /* Current SIFS */
	u32 difs;  /* Current DIFS */
	u32 eifs;  /* Current EIFS */
	u32 slot;  /* Current SlotTime */

	/* Rate */
	u8 bb_type; /* 0: 11A, 1:11B, 2:11G */
	u8 packet_type; /* 0:11a 1:11b 2:11gb 3:11ga */
	u32 basic_rates;
	u8 top_ofdm_basic_rate;
	u8 top_cck_basic_rate;

	u8 eeprom[EEP_MAX_CONTEXT_SIZE];  /*u32 alignment */

	u8 preamble_type;

	/* For RF Power table */
	u8 cck_pwr;
	u8 ofdm_pwr_g;
	u8 ofdm_pwr_a;
	u8 power;
	u8 cck_pwr_tbl[14];
	u8 ofdm_pwr_tbl[14];
	u8 ofdm_a_pwr_tbl[42];

	u16 tx_rate_fb0;
	u16 tx_rate_fb1;

	enum nl80211_iftype op_mode;

	int short_slot_time;

	/* Power save */
	u16 current_aid;

	/* Beacon related */
	u16 seq_counter;

	enum vnt_cmd_state command_state;

	enum vnt_cmd command;

	/* 802.11 counter */

	enum vnt_cmd cmd_queue[CMD_Q_SIZE];
	u32 cmd_dequeue_idx;
	u32 cmd_enqueue_idx;
	u32 free_cmd_queue;
	int cmd_running;

	unsigned long key_entry_inuse;

	u8 auto_fb_ctrl;

	/* For Update BaseBand VGA Gain Offset */
	u8 bb_vga[BB_VGA_LEVEL];

	u8 bb_pre_ed_rssi;
	u8 bb_pre_ed_index;

	/* command timer */
	struct delayed_work run_command_work;

	struct ieee80211_low_level_stats low_stats;
};

int vnt_init(struct vnt_private *priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
