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
 *
 * File: usbpipe.c
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Purpose: Handle USB control endpoint
 *
 * Author: Warren Hsu
 *
 * Date: Mar. 29, 2005
 *
 * Functions:
<<<<<<< HEAD
 *      CONTROLnsRequestOut - Write variable length bytes to MEM/BB/MAC/EEPROM
 *      CONTROLnsRequestIn - Read variable length bytes from MEM/BB/MAC/EEPROM
 *      ControlvWriteByte - Write one byte to MEM/BB/MAC/EEPROM
 *      ControlvReadByte - Read one byte from MEM/BB/MAC/EEPROM
 *      ControlvMaskByte - Read one byte from MEM/BB/MAC/EEPROM and clear/set some bits in the same address
 *
 * Revision History:
 *      04-05-2004 Jerry Chen:  Initial release
 *      11-24-2004 Warren Hsu: Add ControlvWriteByte,ControlvReadByte,ControlvMaskByte
 *
 */

#include "int.h"
#include "rxtx.h"
#include "dpc.h"
#include "control.h"
#include "desc.h"
#include "device.h"

/*---------------------  Static Definitions -------------------------*/
//endpoint def
//endpoint 0: control
//endpoint 1: interrupt
//endpoint 2: read bulk
//endpoint 3: write bulk

//RequestType:
//#define REQUEST_OUT       (USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE) // 0x40
//#define REQUEST_IN        (USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE )  //0xc0
//static int          msglevel                =MSG_LEVEL_DEBUG;
static int          msglevel                =MSG_LEVEL_INFO;


#define USB_CTL_WAIT   500 //ms

#ifndef URB_ASYNC_UNLINK
#define URB_ASYNC_UNLINK    0
#endif

/*---------------------  Static Classes  ----------------------------*/

/*---------------------  Static Variables  --------------------------*/

/*---------------------  Static Functions  --------------------------*/
static
void
s_nsInterruptUsbIoCompleteRead(
     struct urb *urb
    );


static
void
s_nsBulkInUsbIoCompleteRead(
     struct urb *urb
    );


static
void
s_nsBulkOutIoCompleteWrite(
     struct urb *urb
    );


static
void
s_nsControlInUsbIoCompleteRead(
     struct urb *urb
    );

static
void
s_nsControlInUsbIoCompleteWrite(
     struct urb *urb
    );

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

int PIPEnsControlOutAsyn(
     PSDevice     pDevice,
     BYTE         byRequest,
     WORD         wValue,
     WORD         wIndex,
     WORD         wLength,
     PBYTE        pbyBuffer
    )
{
	int ntStatus;

    if (pDevice->Flags & fMP_DISCONNECTED)
        return STATUS_FAILURE;

    if (pDevice->Flags & fMP_CONTROL_WRITES)
        return STATUS_FAILURE;

    if (in_interrupt()) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"in_interrupt return ..byRequest %x\n", byRequest);
        return STATUS_FAILURE;
    }

    ntStatus = usb_control_msg(
                            pDevice->usb,
                            usb_sndctrlpipe(pDevice->usb , 0),
                            byRequest,
                            0x40, // RequestType
                            wValue,
                            wIndex,
			    (void *) pbyBuffer,
                            wLength,
                            HZ
                          );
    if (ntStatus >= 0) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"usb_sndctrlpipe ntStatus= %d\n", ntStatus);
        ntStatus = 0;
    } else {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"usb_sndctrlpipe fail, ntStatus= %d\n", ntStatus);
    }

    return ntStatus;
}

int PIPEnsControlOut(
     PSDevice     pDevice,
     BYTE         byRequest,
     WORD         wValue,
     WORD         wIndex,
     WORD         wLength,
     PBYTE        pbyBuffer
    )
{
	int ntStatus = 0;
    int ii;

    if (pDevice->Flags & fMP_DISCONNECTED)
        return STATUS_FAILURE;

    if (pDevice->Flags & fMP_CONTROL_WRITES)
        return STATUS_FAILURE;

	if (pDevice->Flags & fMP_CONTROL_READS)
		return STATUS_FAILURE;

	MP_SET_FLAG(pDevice, fMP_CONTROL_WRITES);

	pDevice->sUsbCtlRequest.bRequestType = 0x40;
	pDevice->sUsbCtlRequest.bRequest = byRequest;
	pDevice->sUsbCtlRequest.wValue = cpu_to_le16p(&wValue);
	pDevice->sUsbCtlRequest.wIndex = cpu_to_le16p(&wIndex);
	pDevice->sUsbCtlRequest.wLength = cpu_to_le16p(&wLength);
	pDevice->pControlURB->transfer_flags |= URB_ASYNC_UNLINK;
    pDevice->pControlURB->actual_length = 0;
    // Notice, pbyBuffer limited point to variable buffer, can't be constant.
  	usb_fill_control_urb(pDevice->pControlURB, pDevice->usb,
			 usb_sndctrlpipe(pDevice->usb , 0), (char *) &pDevice->sUsbCtlRequest,
			 pbyBuffer, wLength, s_nsControlInUsbIoCompleteWrite, pDevice);

	ntStatus = usb_submit_urb(pDevice->pControlURB, GFP_ATOMIC);
	if (ntStatus != 0) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO
			"control send request submission failed: %d\n",
				ntStatus);
		MP_CLEAR_FLAG(pDevice, fMP_CONTROL_WRITES);
		return STATUS_FAILURE;
	}

	spin_unlock_irq(&pDevice->lock);
    for (ii = 0; ii <= USB_CTL_WAIT; ii ++) {

	if (pDevice->Flags & fMP_CONTROL_WRITES)
		mdelay(1);
        else
		break;

        if (ii >= USB_CTL_WAIT) {
		DBG_PRT(MSG_LEVEL_DEBUG,
			KERN_INFO "control send request submission timeout\n");
            spin_lock_irq(&pDevice->lock);
            MP_CLEAR_FLAG(pDevice, fMP_CONTROL_WRITES);
            return STATUS_FAILURE;
        }
    }
	spin_lock_irq(&pDevice->lock);

    return STATUS_SUCCESS;
}

int PIPEnsControlIn(
     PSDevice     pDevice,
     BYTE         byRequest,
     WORD         wValue,
     WORD         wIndex,
     WORD         wLength,
       PBYTE   pbyBuffer
    )
{
	int ntStatus = 0;
    int ii;

    if (pDevice->Flags & fMP_DISCONNECTED)
        return STATUS_FAILURE;

    if (pDevice->Flags & fMP_CONTROL_READS)
	return STATUS_FAILURE;

	if (pDevice->Flags & fMP_CONTROL_WRITES)
		return STATUS_FAILURE;

	MP_SET_FLAG(pDevice, fMP_CONTROL_READS);

	pDevice->sUsbCtlRequest.bRequestType = 0xC0;
	pDevice->sUsbCtlRequest.bRequest = byRequest;
	pDevice->sUsbCtlRequest.wValue = cpu_to_le16p(&wValue);
	pDevice->sUsbCtlRequest.wIndex = cpu_to_le16p(&wIndex);
	pDevice->sUsbCtlRequest.wLength = cpu_to_le16p(&wLength);
	pDevice->pControlURB->transfer_flags |= URB_ASYNC_UNLINK;
    pDevice->pControlURB->actual_length = 0;
	usb_fill_control_urb(pDevice->pControlURB, pDevice->usb,
			 usb_rcvctrlpipe(pDevice->usb , 0), (char *) &pDevice->sUsbCtlRequest,
			 pbyBuffer, wLength, s_nsControlInUsbIoCompleteRead, pDevice);

	ntStatus = usb_submit_urb(pDevice->pControlURB, GFP_ATOMIC);
	if (ntStatus != 0) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO
			"control request submission failed: %d\n", ntStatus);
		MP_CLEAR_FLAG(pDevice, fMP_CONTROL_READS);
		return STATUS_FAILURE;
	}

	spin_unlock_irq(&pDevice->lock);
    for (ii = 0; ii <= USB_CTL_WAIT; ii ++) {

	if (pDevice->Flags & fMP_CONTROL_READS)
		mdelay(1);
	else
		break;

	if (ii >= USB_CTL_WAIT) {
		DBG_PRT(MSG_LEVEL_DEBUG,
			KERN_INFO "control rcv request submission timeout\n");
            spin_lock_irq(&pDevice->lock);
            MP_CLEAR_FLAG(pDevice, fMP_CONTROL_READS);
            return STATUS_FAILURE;
        }
    }
	spin_lock_irq(&pDevice->lock);

    return ntStatus;
}

static
void
s_nsControlInUsbIoCompleteWrite(
     struct urb *urb
    )
{
    PSDevice        pDevice;

	pDevice = urb->context;
	switch (urb->status) {
	case 0:
		break;
	case -EINPROGRESS:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl write urb status EINPROGRESS%d\n", urb->status);
		break;
	case -ENOENT:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl write urb status ENOENT %d\n", urb->status);
		break;
	default:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl write urb status %d\n", urb->status);
	}

    MP_CLEAR_FLAG(pDevice, fMP_CONTROL_WRITES);
}



/*
 * Description:
 *      Complete function of usb Control callback
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
static
void
s_nsControlInUsbIoCompleteRead(
     struct urb *urb
    )
{
    PSDevice        pDevice;

	pDevice = urb->context;
	switch (urb->status) {
	case 0:
		break;
	case -EINPROGRESS:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl read urb status EINPROGRESS%d\n", urb->status);
		break;
	case -ENOENT:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl read urb status = ENOENT %d\n", urb->status);
		break;
	default:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl read urb status %d\n", urb->status);
	}

    MP_CLEAR_FLAG(pDevice, fMP_CONTROL_READS);
}




/*
 * Description:
 *      Allocates an usb interrupt in irp and calls USBD.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
int PIPEnsInterruptRead(PSDevice pDevice)
{
    int ntStatus = STATUS_FAILURE;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsStartInterruptUsbRead()\n");

    if(pDevice->intBuf.bInUse == TRUE){
        return (STATUS_FAILURE);
    }
    pDevice->intBuf.bInUse = TRUE;
//    pDevice->bEventAvailable = FALSE;
    pDevice->ulIntInPosted++;

    //
    // Now that we have created the urb, we will send a
    // request to the USB device object.
    //
    pDevice->pInterruptURB->interval = pDevice->int_interval;

usb_fill_bulk_urb(pDevice->pInterruptURB,
		pDevice->usb,
		usb_rcvbulkpipe(pDevice->usb, 1),
		(void *) pDevice->intBuf.pDataBuf,
		MAX_INTERRUPT_SIZE,
		s_nsInterruptUsbIoCompleteRead,
		pDevice);

	ntStatus = usb_submit_urb(pDevice->pInterruptURB, GFP_ATOMIC);
	if (ntStatus != 0) {
	    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Submit int URB failed %d\n", ntStatus);
    }

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"<----s_nsStartInterruptUsbRead Return(%x)\n",ntStatus);
    return ntStatus;
}


/*
 * Description:
 *      Complete function of usb interrupt in irp.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
static
void
s_nsInterruptUsbIoCompleteRead(
     struct urb *urb
    )

{
    PSDevice        pDevice;
    int ntStatus;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsInterruptUsbIoCompleteRead\n");
    //
    // The context given to IoSetCompletionRoutine is the receive buffer object
    //
    pDevice = (PSDevice)urb->context;

    //
    // We have a number of cases:
    //      1) The USB read timed out and we received no data.
    //      2) The USB read timed out and we received some data.
    //      3) The USB read was successful and fully filled our irp buffer.
    //      4) The irp was cancelled.
    //      5) Some other failure from the USB device object.
    //
    ntStatus = urb->status;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"s_nsInterruptUsbIoCompleteRead Status %d\n", ntStatus);

    // if we were not successful, we need to free the int buffer for future use right here
    // otherwise interrupt data handler will free int buffer after it handle it.
    if (( ntStatus != STATUS_SUCCESS )) {
        pDevice->ulBulkInError++;
        pDevice->intBuf.bInUse = FALSE;

//        if (ntStatus == USBD_STATUS_CRC) {
//            pDevice->ulIntInContCRCError++;
//        }

//        if (ntStatus == STATUS_NOT_CONNECTED )
//        {
            pDevice->fKillEventPollingThread = TRUE;
//        }
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"IntUSBIoCompleteControl STATUS = %d\n", ntStatus );
    } else {
	    pDevice->ulIntInBytesRead += (unsigned long) urb->actual_length;
	    pDevice->ulIntInContCRCError = 0;
	    pDevice->bEventAvailable = TRUE;
	    INTnsProcessData(pDevice);
    }

    STAvUpdateUSBCounter(&pDevice->scStatistic.USB_InterruptStat, ntStatus);


    if (pDevice->fKillEventPollingThread != TRUE) {
       usb_fill_bulk_urb(pDevice->pInterruptURB,
		      pDevice->usb,
		      usb_rcvbulkpipe(pDevice->usb, 1),
		     (void *) pDevice->intBuf.pDataBuf,
		     MAX_INTERRUPT_SIZE,
		     s_nsInterruptUsbIoCompleteRead,
		     pDevice);

	ntStatus = usb_submit_urb(pDevice->pInterruptURB, GFP_ATOMIC);
	if (ntStatus != 0) {
	    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Submit int URB failed %d\n", ntStatus);
           }
    }
    //
    // We return STATUS_MORE_PROCESSING_REQUIRED so that the completion
    // routine (IofCompleteRequest) will stop working on the irp.
    //
    return ;
}

/*
 * Description:
 *      Allocates an usb BulkIn  irp and calls USBD.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
int PIPEnsBulkInUsbRead(PSDevice pDevice, PRCB pRCB)
{
	int ntStatus = 0;
    struct urb          *pUrb;


    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsStartBulkInUsbRead\n");

    if (pDevice->Flags & fMP_DISCONNECTED)
        return STATUS_FAILURE;

    pDevice->ulBulkInPosted++;


	pUrb = pRCB->pUrb;
    //
    // Now that we have created the urb, we will send a
    // request to the USB device object.
    //
    if (pRCB->skb == NULL) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"pRCB->skb is null \n");
        return ntStatus;
    }

	usb_fill_bulk_urb(pUrb,
		pDevice->usb,
		usb_rcvbulkpipe(pDevice->usb, 2),
		(void *) (pRCB->skb->data),
		MAX_TOTAL_SIZE_WITH_ALL_HEADERS,
		s_nsBulkInUsbIoCompleteRead,
		pRCB);

	ntStatus = usb_submit_urb(pUrb, GFP_ATOMIC);
	if (ntStatus != 0) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Submit Rx URB failed %d\n", ntStatus);
		return STATUS_FAILURE ;
	}
    pRCB->Ref = 1;
    pRCB->bBoolInUse= TRUE;

    return ntStatus;
}




/*
 * Description:
 *      Complete function of usb BulkIn irp.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
static
void
s_nsBulkInUsbIoCompleteRead(
     struct urb *urb
    )

{
    PRCB    pRCB = (PRCB)urb->context;
    PSDevice pDevice = (PSDevice)pRCB->pDevice;
    unsigned long   bytesRead;
    BOOL    bIndicateReceive = FALSE;
    BOOL    bReAllocSkb = FALSE;
    int status;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsBulkInUsbIoCompleteRead\n");
    status = urb->status;
    bytesRead = urb->actual_length;

    if (status) {
        pDevice->ulBulkInError++;
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"BULK In failed %d\n", status);

           pDevice->scStatistic.RxFcsErrCnt ++;
//todo...xxxxxx
//        if (status == USBD_STATUS_CRC) {
//            pDevice->ulBulkInContCRCError++;
//        }
//        if (status == STATUS_DEVICE_NOT_CONNECTED )
//        {
//            MP_SET_FLAG(pDevice, fMP_DISCONNECTED);
//        }
    } else {
        bIndicateReceive = TRUE;
        pDevice->ulBulkInContCRCError = 0;
        pDevice->ulBulkInBytesRead += bytesRead;

           pDevice->scStatistic.RxOkCnt ++;
    }


    STAvUpdateUSBCounter(&pDevice->scStatistic.USB_BulkInStat, status);

    if (bIndicateReceive) {
        spin_lock(&pDevice->lock);
        if (RXbBulkInProcessData(pDevice, pRCB, bytesRead) == TRUE)
            bReAllocSkb = TRUE;
        spin_unlock(&pDevice->lock);
    }
    pRCB->Ref--;
    if (pRCB->Ref == 0)
    {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"RxvFreeNormal %d \n",pDevice->NumRecvFreeList);
        spin_lock(&pDevice->lock);
        RXvFreeRCB(pRCB, bReAllocSkb);
        spin_unlock(&pDevice->lock);
    }


    return;
}

/*
 * Description:
 *      Allocates an usb BulkOut  irp and calls USBD.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
int
PIPEnsSendBulkOut(
      PSDevice pDevice,
      PUSB_SEND_CONTEXT pContext
    )
{
    int status;
    struct urb          *pUrb;



    pDevice->bPWBitOn = FALSE;

/*
    if (pDevice->pPendingBulkOutContext != NULL) {
        pDevice->NumContextsQueued++;
        EnqueueContext(pDevice->FirstTxContextQueue, pDevice->LastTxContextQueue, pContext);
        status = STATUS_PENDING;
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Send pending!\n");
        return status;
    }
*/

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"s_nsSendBulkOut\n");

    if (MP_IS_READY(pDevice) && (pDevice->Flags & fMP_POST_WRITES)) {

        pUrb = pContext->pUrb;
        pDevice->ulBulkOutPosted++;
//        pDevice->pPendingBulkOutContext = pContext;
        usb_fill_bulk_urb(
        	    pUrb,
        		pDevice->usb,
		    usb_sndbulkpipe(pDevice->usb, 3),
		    (void *) &(pContext->Data[0]),
        		pContext->uBufLen,
        		s_nsBulkOutIoCompleteWrite,
        		pContext);

    	status = usb_submit_urb(pUrb, GFP_ATOMIC);
    	if (status != 0)
    	{
    		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Submit Tx URB failed %d\n", status);
    		return STATUS_FAILURE;
    	}
        return STATUS_PENDING;
    }
    else {
        pContext->bBoolInUse = FALSE;
        return STATUS_RESOURCES;
    }
}

/*
 * Description: s_nsBulkOutIoCompleteWrite
 *     1a) Indicate to the protocol the status of the write.
 *     1b) Return ownership of the packet to the protocol.
 *
 *     2)  If any more packets are queue for sending, send another packet
 *         to USBD.
 *         If the attempt to send the packet to the driver fails,
 *         return ownership of the packet to the protocol and
 *         try another packet (until one succeeds).
 *
 * Parameters:
 *  In:
 *      pdoUsbDevObj  - pointer to the USB device object which
 *                      completed the irp
 *      pIrp          - the irp which was completed by the
 *                      device object
 *      pContext      - the context given to IoSetCompletionRoutine
 *                      before calling IoCallDriver on the irp
 *                      The pContext is a pointer to the USB device object.
 *  Out:
 *      none
 *
 * Return Value: STATUS_MORE_PROCESSING_REQUIRED - allows the completion routine
 *               (IofCompleteRequest) to stop working on the irp.
 *
 */
static
void
s_nsBulkOutIoCompleteWrite(
     struct urb *urb
    )
{
    PSDevice            pDevice;
    int status;
    CONTEXT_TYPE        ContextType;
    unsigned long               ulBufLen;
    PUSB_SEND_CONTEXT   pContext;


    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsBulkOutIoCompleteWrite\n");
    //
    // The context given to IoSetCompletionRoutine is an USB_CONTEXT struct
    //
    pContext = (PUSB_SEND_CONTEXT) urb->context;
    ASSERT( NULL != pContext );

    pDevice = pContext->pDevice;
    ContextType = pContext->Type;
    ulBufLen = pContext->uBufLen;

    if (!netif_device_present(pDevice->dev))
	    return;

   //
    // Perform various IRP, URB, and buffer 'sanity checks'
    //

    status = urb->status;
    //we should have failed, succeeded, or cancelled, but NOT be pending
    STAvUpdateUSBCounter(&pDevice->scStatistic.USB_BulkOutStat, status);

    if(status == STATUS_SUCCESS) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Write %d bytes\n",(int)ulBufLen);
        pDevice->ulBulkOutBytesWrite += ulBufLen;
        pDevice->ulBulkOutContCRCError = 0;
	pDevice->nTxDataTimeCout = 0;

    } else {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"BULK Out failed %d\n", status);
        pDevice->ulBulkOutError++;
    }

//    pDevice->ulCheckForHangCount = 0;
//    pDevice->pPendingBulkOutContext = NULL;

    if ( CONTEXT_DATA_PACKET == ContextType ) {
        // Indicate to the protocol the status of the sent packet and return
        // ownership of the packet.
	    if (pContext->pPacket != NULL) {
	        dev_kfree_skb_irq(pContext->pPacket);
	        pContext->pPacket = NULL;
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"tx  %d bytes\n",(int)ulBufLen);
	    }

        pDevice->dev->trans_start = jiffies;


        if (status == STATUS_SUCCESS) {
            pDevice->packetsSent++;
        }
        else {
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Send USB error! [%08xh]\n", status);
            pDevice->packetsSentDropped++;
        }

    }
    if (pDevice->bLinkPass == TRUE) {
        if (netif_queue_stopped(pDevice->dev))
            netif_wake_queue(pDevice->dev);
    }
    pContext->bBoolInUse = FALSE;

    return;
=======
 *	vnt_control_out - Write variable length bytes to MEM/BB/MAC/EEPROM
 *	vnt_control_in - Read variable length bytes from MEM/BB/MAC/EEPROM
 *	vnt_control_out_u8 - Write one byte to MEM/BB/MAC/EEPROM
 *	vnt_control_in_u8 - Read one byte from MEM/BB/MAC/EEPROM
 *
 * Revision History:
 *      04-05-2004 Jerry Chen: Initial release
 *      11-24-2004 Warren Hsu: Add ControlvWriteByte,ControlvReadByte,
 *                             ControlvMaskByte
 *
 */

#include "rxtx.h"
#include "desc.h"
#include "device.h"
#include "usbpipe.h"
#include "mac.h"
#include "rf.h"

#define USB_CTL_WAIT	500 /* ms */

int vnt_control_out(struct vnt_private *priv, u8 request, u16 value,
		    u16 index, u16 length, const u8 *buffer)
{
	int ret = 0;
	u8 *usb_buffer;

	if (test_bit(DEVICE_FLAGS_DISCONNECTED, &priv->flags)) {
		ret = -EINVAL;
		goto end;
	}

	mutex_lock(&priv->usb_lock);

	usb_buffer = kmemdup(buffer, length, GFP_KERNEL);
	if (!usb_buffer) {
		ret = -ENOMEM;
		goto end_unlock;
	}

	ret = usb_control_msg(priv->usb,
			      usb_sndctrlpipe(priv->usb, 0),
			      request, 0x40, value,
			      index, usb_buffer, length, USB_CTL_WAIT);

	kfree(usb_buffer);

	if (ret == (int)length)
		ret = 0;
	else
		ret = -EIO;

end_unlock:
	mutex_unlock(&priv->usb_lock);
end:
	return ret;
}

int vnt_control_out_u8(struct vnt_private *priv, u8 reg, u8 reg_off, u8 data)
{
	return vnt_control_out(priv, MESSAGE_TYPE_WRITE,
			       reg_off, reg, sizeof(u8), &data);
}

int vnt_control_out_blocks(struct vnt_private *priv,
			   u16 block, u8 reg, u16 length, const u8 *data)
{
	int ret = 0, i;

	for (i = 0; i < length; i += block) {
		u16 len = min_t(int, length - i, block);

		ret = vnt_control_out(priv, MESSAGE_TYPE_WRITE,
				      i, reg, len, data + i);
		if (ret)
			goto end;
	}
end:
	return ret;
}

int vnt_control_in(struct vnt_private *priv, u8 request, u16 value,
		   u16 index, u16 length, u8 *buffer)
{
	int ret = 0;
	u8 *usb_buffer;

	if (test_bit(DEVICE_FLAGS_DISCONNECTED, &priv->flags)) {
		ret = -EINVAL;
		goto end;
	}

	mutex_lock(&priv->usb_lock);

	usb_buffer = kmalloc(length, GFP_KERNEL);
	if (!usb_buffer) {
		ret = -ENOMEM;
		goto end_unlock;
	}

	ret = usb_control_msg(priv->usb,
			      usb_rcvctrlpipe(priv->usb, 0),
			      request, 0xc0, value,
			      index, usb_buffer, length, USB_CTL_WAIT);

	if (ret == length)
		memcpy(buffer, usb_buffer, length);

	kfree(usb_buffer);

	if (ret == (int)length)
		ret = 0;
	else
		ret = -EIO;

end_unlock:
	mutex_unlock(&priv->usb_lock);
end:
	return ret;
}

int vnt_control_in_u8(struct vnt_private *priv, u8 reg, u8 reg_off, u8 *data)
{
	return vnt_control_in(priv, MESSAGE_TYPE_READ,
			      reg_off, reg, sizeof(u8), data);
}

static int vnt_int_report_rate(struct vnt_private *priv, u8 pkt_no, u8 tsr)
{
	struct vnt_usb_send_context *context;
	struct ieee80211_tx_info *info;
	u8 tx_retry = (tsr & 0xf0) >> 4;
	s8 idx;

	if (pkt_no >= priv->num_tx_context)
		return -EINVAL;

	context = priv->tx_context[pkt_no];

	if (!context->skb)
		return -EINVAL;

	info = IEEE80211_SKB_CB(context->skb);
	idx = info->control.rates[0].idx;

	ieee80211_tx_info_clear_status(info);

	info->status.rates[0].count = tx_retry;

	if (!(tsr & TSR_TMO)) {
		info->status.rates[0].idx = idx;

		if (!(info->flags & IEEE80211_TX_CTL_NO_ACK))
			info->flags |= IEEE80211_TX_STAT_ACK;
	}

	ieee80211_tx_status_irqsafe(priv->hw, context->skb);

	context->in_use = false;

	return 0;
}

static void vnt_int_process_data(struct vnt_private *priv)
{
	struct vnt_interrupt_data *int_data;
	struct ieee80211_low_level_stats *low_stats = &priv->low_stats;

	dev_dbg(&priv->usb->dev, "---->s_nsInterruptProcessData\n");

	int_data = (struct vnt_interrupt_data *)priv->int_buf.data_buf;

	if (int_data->tsr0 & TSR_VALID)
		vnt_int_report_rate(priv, int_data->pkt0, int_data->tsr0);

	if (int_data->tsr1 & TSR_VALID)
		vnt_int_report_rate(priv, int_data->pkt1, int_data->tsr1);

	if (int_data->tsr2 & TSR_VALID)
		vnt_int_report_rate(priv, int_data->pkt2, int_data->tsr2);

	if (int_data->tsr3 & TSR_VALID)
		vnt_int_report_rate(priv, int_data->pkt3, int_data->tsr3);

	if (!int_data->isr0)
		return;

	if (int_data->isr0 & ISR_BNTX && priv->op_mode == NL80211_IFTYPE_AP)
		vnt_schedule_command(priv, WLAN_CMD_BECON_SEND);

	priv->current_tsf = le64_to_cpu(int_data->tsf);

	low_stats->dot11RTSSuccessCount += int_data->rts_success;
	low_stats->dot11RTSFailureCount += int_data->rts_fail;
	low_stats->dot11ACKFailureCount += int_data->ack_fail;
	low_stats->dot11FCSErrorCount += int_data->fcs_err;
}

static void vnt_start_interrupt_urb_complete(struct urb *urb)
{
	struct vnt_private *priv = urb->context;
	int status = urb->status;

	switch (status) {
	case 0:
	case -ETIMEDOUT:
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		return;
	default:
		break;
	}

	if (status)
		dev_dbg(&priv->usb->dev, "%s status = %d\n", __func__, status);
	else
		vnt_int_process_data(priv);

	if (!test_bit(DEVICE_FLAGS_DISCONNECTED, &priv->flags))
		status = usb_submit_urb(priv->interrupt_urb, GFP_ATOMIC);

	if (status)
		dev_dbg(&priv->usb->dev, "Submit int URB failed %d\n", status);
}

int vnt_start_interrupt_urb(struct vnt_private *priv)
{
	int ret = 0;

	dev_dbg(&priv->usb->dev, "---->Interrupt Polling Thread\n");

	usb_fill_int_urb(priv->interrupt_urb,
			 priv->usb,
			 usb_rcvintpipe(priv->usb, 1),
			 priv->int_buf.data_buf,
			 MAX_INTERRUPT_SIZE,
			 vnt_start_interrupt_urb_complete,
			 priv,
			 priv->int_interval);

	ret = usb_submit_urb(priv->interrupt_urb, GFP_ATOMIC);
	if (ret)
		dev_dbg(&priv->usb->dev, "Submit int URB failed %d\n", ret);

	return ret;
}

static int vnt_rx_data(struct vnt_private *priv, struct vnt_rcb *ptr_rcb,
		       unsigned long bytes_received)
{
	struct ieee80211_hw *hw = priv->hw;
	struct ieee80211_supported_band *sband;
	struct sk_buff *skb;
	struct ieee80211_rx_status *rx_status;
	struct vnt_rx_header *head;
	struct vnt_rx_tail *tail;
	u32 frame_size;
	int ii;
	u16 rx_bitrate, pay_load_with_padding;
	u8 rate_idx = 0;
	long rx_dbm;

	skb = ptr_rcb->skb;
	rx_status = IEEE80211_SKB_RXCB(skb);

	/* [31:16]RcvByteCount ( not include 4-byte Status ) */
	head = (struct vnt_rx_header *)skb->data;
	frame_size = head->wbk_status >> 16;
	frame_size += 4;

	if (bytes_received != frame_size) {
		dev_dbg(&priv->usb->dev, "------- WRONG Length 1\n");
		return false;
	}

	if ((bytes_received > 2372) || (bytes_received <= 40)) {
		/* Frame Size error drop this packet.*/
		dev_dbg(&priv->usb->dev, "------ WRONG Length 2\n");
		return false;
	}

	/* real Frame Size = USBframe_size -4WbkStatus - 4RxStatus */
	/* -8TSF - 4RSR - 4SQ3 - ?Padding */

	/* if SQ3 the range is 24~27, if no SQ3 the range is 20~23 */

	/*Fix hardware bug => PLCP_Length error */
	if (((bytes_received - head->pay_load_len) > 27) ||
	    ((bytes_received - head->pay_load_len) < 24) ||
	    (bytes_received < head->pay_load_len)) {
		dev_dbg(&priv->usb->dev, "Wrong PLCP Length %x\n",
			head->pay_load_len);
		return false;
	}

	sband = hw->wiphy->bands[hw->conf.chandef.chan->band];
	rx_bitrate = head->rx_rate * 5; /* rx_rate * 5 */

	for (ii = 0; ii < sband->n_bitrates; ii++) {
		if (sband->bitrates[ii].bitrate == rx_bitrate) {
			rate_idx = ii;
				break;
		}
	}

	if (ii == sband->n_bitrates) {
		dev_dbg(&priv->usb->dev, "Wrong Rx Bit Rate %d\n", rx_bitrate);
		return false;
	}

	pay_load_with_padding = ((head->pay_load_len / 4) +
		((head->pay_load_len % 4) ? 1 : 0)) * 4;

	tail = (struct vnt_rx_tail *)(skb->data +
				      sizeof(*head) + pay_load_with_padding);
	priv->tsf_time = le64_to_cpu(tail->tsf_time);

	if (tail->rsr & (RSR_IVLDTYP | RSR_IVLDLEN))
		return false;

	vnt_rf_rssi_to_dbm(priv, tail->rssi, &rx_dbm);

	priv->bb_pre_ed_rssi = (u8)-rx_dbm + 1;
	priv->current_rssi = priv->bb_pre_ed_rssi;

	skb_pull(skb, sizeof(*head));
	skb_trim(skb, head->pay_load_len);

	rx_status->mactime = priv->tsf_time;
	rx_status->band = hw->conf.chandef.chan->band;
	rx_status->signal = rx_dbm;
	rx_status->flag = 0;
	rx_status->freq = hw->conf.chandef.chan->center_freq;

	if (!(tail->rsr & RSR_CRCOK))
		rx_status->flag |= RX_FLAG_FAILED_FCS_CRC;

	rx_status->rate_idx = rate_idx;

	if (tail->new_rsr & NEWRSR_DECRYPTOK)
		rx_status->flag |= RX_FLAG_DECRYPTED;

	ieee80211_rx_irqsafe(priv->hw, skb);

	return true;
}

static void vnt_submit_rx_urb_complete(struct urb *urb)
{
	struct vnt_rcb *rcb = urb->context;
	struct vnt_private *priv = rcb->priv;

	switch (urb->status) {
	case 0:
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		return;
	case -ETIMEDOUT:
	default:
		dev_dbg(&priv->usb->dev, "BULK In failed %d\n", urb->status);
		break;
	}

	if (urb->actual_length) {
		if (vnt_rx_data(priv, rcb, urb->actual_length)) {
			rcb->skb = dev_alloc_skb(priv->rx_buf_sz);
			if (!rcb->skb)
				return;
		} else {
			skb_push(rcb->skb, skb_headroom(rcb->skb));
			skb_trim(rcb->skb, 0);
		}

		urb->transfer_buffer = skb_put(rcb->skb,
					       skb_tailroom(rcb->skb));
	}

	if (usb_submit_urb(urb, GFP_ATOMIC))
		dev_dbg(&priv->usb->dev, "Failed to re submit rx skb\n");
}

int vnt_submit_rx_urb(struct vnt_private *priv, struct vnt_rcb *rcb)
{
	int ret = 0;
	struct urb *urb = rcb->urb;

	if (!rcb->skb) {
		dev_dbg(&priv->usb->dev, "rcb->skb is null\n");
		ret = -EINVAL;
		goto end;
	}

	usb_fill_bulk_urb(urb,
			  priv->usb,
			  usb_rcvbulkpipe(priv->usb, 2),
			  skb_put(rcb->skb, skb_tailroom(rcb->skb)),
			  MAX_TOTAL_SIZE_WITH_ALL_HEADERS,
			  vnt_submit_rx_urb_complete,
			  rcb);

	ret = usb_submit_urb(urb, GFP_ATOMIC);
	if (ret)
		dev_dbg(&priv->usb->dev, "Submit Rx URB failed %d\n", ret);
end:
	return ret;
}

static void vnt_tx_context_complete(struct urb *urb)
{
	struct vnt_usb_send_context *context = urb->context;
	struct vnt_private *priv = context->priv;

	switch (urb->status) {
	case 0:
		dev_dbg(&priv->usb->dev,
			"Write %d bytes\n", urb->actual_length);
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		context->in_use = false;
		return;
	case -ETIMEDOUT:
	default:
		dev_dbg(&priv->usb->dev, "BULK Out failed %d\n", urb->status);
		break;
	}

	if (context->type == CONTEXT_DATA_PACKET)
		ieee80211_wake_queues(priv->hw);

	if (urb->status || context->type == CONTEXT_BEACON_PACKET) {
		if (context->skb)
			ieee80211_free_txskb(priv->hw, context->skb);

		context->in_use = false;
	}
}

int vnt_tx_context(struct vnt_private *priv,
		   struct vnt_usb_send_context *context,
		   struct sk_buff *skb)
{
	struct vnt_tx_usb_header *usb;
	struct urb *urb;
	int status;
	u16 count = skb->len;

	usb = skb_push(skb, sizeof(*usb));
	usb->tx_byte_count = cpu_to_le16(count);
	usb->pkt_no = context->pkt_no;
	usb->type = context->type;

	if (test_bit(DEVICE_FLAGS_DISCONNECTED, &priv->flags)) {
		context->in_use = false;
		return -ENODEV;
	}

	if (skb->len > MAX_TOTAL_SIZE_WITH_ALL_HEADERS) {
		context->in_use = false;
		return -E2BIG;
	}

	urb = usb_alloc_urb(0, GFP_ATOMIC);
	if (!urb) {
		context->in_use = false;
		return -ENOMEM;
	}

	usb_fill_bulk_urb(urb,
			  priv->usb,
			  usb_sndbulkpipe(priv->usb, 3),
			  skb->data,
			  skb->len,
			  vnt_tx_context_complete,
			  context);

	usb_anchor_urb(urb, &priv->tx_submitted);

	status = usb_submit_urb(urb, GFP_ATOMIC);
	if (status) {
		dev_dbg(&priv->usb->dev, "Submit Tx URB failed %d\n", status);
		usb_unanchor_urb(urb);
		context->in_use = false;
	}

	usb_free_urb(urb);

	return status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
