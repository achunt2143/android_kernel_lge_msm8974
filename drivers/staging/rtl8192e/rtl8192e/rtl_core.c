<<<<<<< HEAD
/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Based on the r8180 driver, which is:
 * Copyright 2004-2005 Andrea Merello <andreamrl@tiscali.it>, et al.
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
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
#undef RX_DONT_PASS_UL
#undef DEBUG_EPROM
#undef DEBUG_RX_VERBOSE
#undef DUMMY_RX
#undef DEBUG_ZERO_RX
#undef DEBUG_RX_SKB
#undef DEBUG_TX_FRAG
#undef DEBUG_RX_FRAG
#undef DEBUG_TX_FILLDESC
#undef DEBUG_TX
#undef DEBUG_IRQ
#undef DEBUG_RX
#undef DEBUG_RXALLOC
#undef DEBUG_REGISTERS
#undef DEBUG_RING
#undef DEBUG_IRQ_TASKLET
#undef DEBUG_TX_ALLOC
#undef DEBUG_TX_DESC

#include <linux/uaccess.h>
#include <linux/pci.h>
#include <linux/vmalloc.h>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Based on the r8180 driver, which is:
 * Copyright 2004-2005 Andrea Merello <andrea.merello@gmail.com>, et al.
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
#include <linux/uaccess.h>
#include <linux/pci.h>
#include <linux/vmalloc.h>
#include <linux/ieee80211.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "rtl_core.h"
#include "r8192E_phy.h"
#include "r8192E_phyreg.h"
#include "r8190P_rtl8256.h"
#include "r8192E_cmdpkt.h"

#include "rtl_wx.h"
#include "rtl_dm.h"

#include "rtl_pm.h"

int hwwep = 1;
<<<<<<< HEAD
static int channels = 0x3fff;
static char *ifname = "wlan%d";


static struct rtl819x_ops rtl819xp_ops = {
	.nic_type			= NIC_8192E,
	.get_eeprom_size		= rtl8192_get_eeprom_size,
	.init_adapter_variable		= rtl8192_InitializeVariables,
	.initialize_adapter		= rtl8192_adapter_start,
	.link_change			= rtl8192_link_change,
	.tx_fill_descriptor		= rtl8192_tx_fill_desc,
	.tx_fill_cmd_descriptor		= rtl8192_tx_fill_cmd_desc,
	.rx_query_status_descriptor	= rtl8192_rx_query_status_desc,
	.rx_command_packet_handler = NULL,
	.stop_adapter			= rtl8192_halt_adapter,
	.update_ratr_table		= rtl8192_update_ratr_table,
	.irq_enable			= rtl8192_EnableInterrupt,
	.irq_disable			= rtl8192_DisableInterrupt,
	.irq_clear			= rtl8192_ClearInterrupt,
	.rx_enable			= rtl8192_enable_rx,
	.tx_enable			= rtl8192_enable_tx,
	.interrupt_recognized		= rtl8192_interrupt_recognized,
	.TxCheckStuckHandler		= rtl8192_HalTxCheckStuck,
	.RxCheckStuckHandler		= rtl8192_HalRxCheckStuck,
};

static struct pci_device_id rtl8192_pci_id_tbl[] __devinitdata = {
	{RTL_PCI_DEVICE(0x10ec, 0x8192, rtl819xp_ops)},
	{RTL_PCI_DEVICE(0x07aa, 0x0044, rtl819xp_ops)},
	{RTL_PCI_DEVICE(0x07aa, 0x0047, rtl819xp_ops)},
=======
static char *ifname = "wlan%d";

static struct pci_device_id rtl8192_pci_id_tbl[] = {
	{PCI_DEVICE(0x10ec, 0x8192)},
	{PCI_DEVICE(0x07aa, 0x0044)},
	{PCI_DEVICE(0x07aa, 0x0047)},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{}
};

MODULE_DEVICE_TABLE(pci, rtl8192_pci_id_tbl);

<<<<<<< HEAD
static int __devinit rtl8192_pci_probe(struct pci_dev *pdev,
			const struct pci_device_id *id);
static void __devexit rtl8192_pci_disconnect(struct pci_dev *pdev);
=======
static int _rtl92e_pci_probe(struct pci_dev *pdev,
			     const struct pci_device_id *id);
static void _rtl92e_pci_disconnect(struct pci_dev *pdev);
static irqreturn_t _rtl92e_irq(int irq, void *netdev);

static SIMPLE_DEV_PM_OPS(rtl92e_pm_ops, rtl92e_suspend, rtl92e_resume);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct pci_driver rtl8192_pci_driver = {
	.name = DRV_NAME,	/* Driver name   */
	.id_table = rtl8192_pci_id_tbl,	/* PCI_ID table  */
<<<<<<< HEAD
	.probe	= rtl8192_pci_probe,	/* probe fn      */
	.remove	 = __devexit_p(rtl8192_pci_disconnect),	/* remove fn */
	.suspend = rtl8192E_suspend,	/* PM suspend fn */
	.resume = rtl8192E_resume,                 /* PM resume fn  */
};

/****************************************************************************
   -----------------------------IO STUFF-------------------------
*****************************************************************************/
static bool PlatformIOCheckPageLegalAndGetRegMask(u32 u4bPage, u8 *pu1bPageMask)
{
	bool		bReturn = false;

	*pu1bPageMask = 0xfe;

	switch (u4bPage) {
	case 1: case 2: case 3: case 4:
	case 8: case 9: case 10: case 12: case 13:
		bReturn = true;
		*pu1bPageMask = 0xf0;
		break;

	default:
		bReturn = false;
		break;
	}

	return bReturn;
}

void write_nic_io_byte(struct net_device *dev, int x, u8 y)
{
	u32 u4bPage = (x >> 8);
	u8 u1PageMask = 0;
	bool	bIsLegalPage = false;

	if (u4bPage == 0) {
		outb(y&0xff, dev->base_addr + x);

	} else {
		bIsLegalPage = PlatformIOCheckPageLegalAndGetRegMask(u4bPage,
			       &u1PageMask);
		if (bIsLegalPage) {
			u8 u1bPsr = read_nic_io_byte(dev, PSR);

			write_nic_io_byte(dev, PSR, ((u1bPsr & u1PageMask) |
					  (u8)u4bPage));
			write_nic_io_byte(dev, (x & 0xff), y);
			write_nic_io_byte(dev, PSR, (u1bPsr & u1PageMask));
		}
	}
}

void write_nic_io_word(struct net_device *dev, int x, u16 y)
{
	u32 u4bPage = (x >> 8);
	u8 u1PageMask = 0;
	bool	bIsLegalPage = false;

	if (u4bPage == 0) {
		outw(y, dev->base_addr + x);
	} else {
		bIsLegalPage = PlatformIOCheckPageLegalAndGetRegMask(u4bPage,
							 &u1PageMask);
		if (bIsLegalPage) {
			u8 u1bPsr = read_nic_io_byte(dev, PSR);

			write_nic_io_byte(dev, PSR, ((u1bPsr & u1PageMask) |
					  (u8)u4bPage));
			write_nic_io_word(dev, (x & 0xff), y);
			write_nic_io_byte(dev, PSR, (u1bPsr & u1PageMask));

		}
	}
}

void write_nic_io_dword(struct net_device *dev, int x, u32 y)
{
	u32 u4bPage = (x >> 8);
	u8 u1PageMask = 0;
	bool	bIsLegalPage = false;

	if (u4bPage == 0) {
		outl(y, dev->base_addr + x);
	} else {
		bIsLegalPage = PlatformIOCheckPageLegalAndGetRegMask(u4bPage,
						 &u1PageMask);
		if (bIsLegalPage) {
			u8 u1bPsr = read_nic_io_byte(dev, PSR);

			write_nic_io_byte(dev, PSR, ((u1bPsr & u1PageMask) |
					  (u8)u4bPage));
			write_nic_io_dword(dev, (x & 0xff), y);
			write_nic_io_byte(dev, PSR, (u1bPsr & u1PageMask));
		}
	}
}

u8 read_nic_io_byte(struct net_device *dev, int x)
{
	u32 u4bPage = (x >> 8);
	u8 u1PageMask = 0;
	bool	bIsLegalPage = false;
	u8	Data = 0;

	if (u4bPage == 0) {
		return 0xff&inb(dev->base_addr + x);
	} else {
		bIsLegalPage = PlatformIOCheckPageLegalAndGetRegMask(u4bPage,
							&u1PageMask);
		if (bIsLegalPage) {
			u8 u1bPsr = read_nic_io_byte(dev, PSR);

			write_nic_io_byte(dev, PSR, ((u1bPsr & u1PageMask) |
					  (u8)u4bPage));
			Data = read_nic_io_byte(dev, (x & 0xff));
			write_nic_io_byte(dev, PSR, (u1bPsr & u1PageMask));
		}
	}

	return Data;
}

u16 read_nic_io_word(struct net_device *dev, int x)
{
	u32 u4bPage = (x >> 8);
	u8 u1PageMask = 0;
	bool	bIsLegalPage = false;
	u16	Data = 0;

	if (u4bPage == 0) {
		return inw(dev->base_addr + x);
	} else {
		bIsLegalPage = PlatformIOCheckPageLegalAndGetRegMask(u4bPage,
			       &u1PageMask);
		if (bIsLegalPage) {
			u8 u1bPsr = read_nic_io_byte(dev, PSR);

			write_nic_io_byte(dev, PSR, ((u1bPsr & u1PageMask) |
					  (u8)u4bPage));
			Data = read_nic_io_word(dev, (x & 0xff));
			write_nic_io_byte(dev, PSR, (u1bPsr & u1PageMask));

		}
	}

	return Data;
}

u32 read_nic_io_dword(struct net_device *dev, int x)
{
	u32 u4bPage = (x >> 8);
	u8 u1PageMask = 0;
	bool	bIsLegalPage = false;
	u32	Data = 0;

	if (u4bPage == 0) {
		return inl(dev->base_addr + x);
	} else {
		bIsLegalPage = PlatformIOCheckPageLegalAndGetRegMask(u4bPage,
			       &u1PageMask);
		if (bIsLegalPage) {
			u8 u1bPsr = read_nic_io_byte(dev, PSR);

			write_nic_io_byte(dev, PSR, ((u1bPsr & u1PageMask) |
					  (u8)u4bPage));
			Data = read_nic_io_dword(dev, (x & 0xff));
			write_nic_io_byte(dev, PSR, (u1bPsr & u1PageMask));

		}
	}

	return Data;
}

u8 read_nic_byte(struct net_device *dev, int x)
=======
	.probe	= _rtl92e_pci_probe,	/* probe fn      */
	.remove	 = _rtl92e_pci_disconnect,	/* remove fn */
	.driver.pm = &rtl92e_pm_ops,
};

static short _rtl92e_is_tx_queue_empty(struct net_device *dev);
static void _rtl92e_watchdog_wq_cb(void *data);
static void _rtl92e_watchdog_timer_cb(struct timer_list *t);
static void _rtl92e_hard_data_xmit(struct sk_buff *skb, struct net_device *dev,
				   int rate);
static int _rtl92e_hard_start_xmit(struct sk_buff *skb, struct net_device *dev);
static void _rtl92e_tx_cmd(struct net_device *dev, struct sk_buff *skb);
static short _rtl92e_tx(struct net_device *dev, struct sk_buff *skb);
static short _rtl92e_pci_initdescring(struct net_device *dev);
static void _rtl92e_irq_tx_tasklet(struct tasklet_struct *t);
static void _rtl92e_irq_rx_tasklet(struct tasklet_struct *t);
static void _rtl92e_cancel_deferred_work(struct r8192_priv *priv);
static int _rtl92e_up(struct net_device *dev);
static int _rtl92e_try_up(struct net_device *dev);
static int _rtl92e_down(struct net_device *dev, bool shutdownrf);
static void _rtl92e_restart(void *data);

/****************************************************************************
 *  -----------------------------IO STUFF-------------------------
 ****************************************************************************/

u8 rtl92e_readb(struct net_device *dev, int x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0xff & readb((u8 __iomem *)dev->mem_start + x);
}

<<<<<<< HEAD
u32 read_nic_dword(struct net_device *dev, int x)
=======
u32 rtl92e_readl(struct net_device *dev, int x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return readl((u8 __iomem *)dev->mem_start + x);
}

<<<<<<< HEAD
u16 read_nic_word(struct net_device *dev, int x)
=======
u16 rtl92e_readw(struct net_device *dev, int x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return readw((u8 __iomem *)dev->mem_start + x);
}

<<<<<<< HEAD
void write_nic_byte(struct net_device *dev, int x, u8 y)
=======
void rtl92e_writeb(struct net_device *dev, int x, u8 y)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	writeb(y, (u8 __iomem *)dev->mem_start + x);

	udelay(20);
}

<<<<<<< HEAD
void write_nic_dword(struct net_device *dev, int x, u32 y)
=======
void rtl92e_writel(struct net_device *dev, int x, u32 y)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	writel(y, (u8 __iomem *)dev->mem_start + x);

	udelay(20);
}

<<<<<<< HEAD
void write_nic_word(struct net_device *dev, int x, u16 y)
=======
void rtl92e_writew(struct net_device *dev, int x, u16 y)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	writew(y, (u8 __iomem *)dev->mem_start + x);

	udelay(20);
}

/****************************************************************************
<<<<<<< HEAD
   -----------------------------GENERAL FUNCTION-------------------------
*****************************************************************************/
bool MgntActSet_RF_State(struct net_device *dev,
			 enum rt_rf_power_state StateToSet,
			 RT_RF_CHANGE_SOURCE ChangeSource,
			 bool	ProtectOrNot)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rtllib_device *ieee = priv->rtllib;
	bool			bActionAllowed = false;
	bool			bConnectBySSID = false;
	enum rt_rf_power_state rtState;
	u16			RFWaitCounter = 0;
	unsigned long flag;
	RT_TRACE((COMP_PS | COMP_RF), "===>MgntActSet_RF_State(): "
		 "StateToSet(%d)\n", StateToSet);

	ProtectOrNot = false;


	if (!ProtectOrNot) {
		while (true) {
			spin_lock_irqsave(&priv->rf_ps_lock, flag);
			if (priv->RFChangeInProgress) {
				spin_unlock_irqrestore(&priv->rf_ps_lock, flag);
				RT_TRACE((COMP_PS | COMP_RF),
					 "MgntActSet_RF_State(): RF Change in "
					 "progress! Wait to set..StateToSet"
					 "(%d).\n", StateToSet);

				while (priv->RFChangeInProgress) {
					RFWaitCounter++;
					RT_TRACE((COMP_PS | COMP_RF),
						 "MgntActSet_RF_State(): Wait 1"
						 " ms (%d times)...\n",
						 RFWaitCounter);
					mdelay(1);

					if (RFWaitCounter > 100) {
						RT_TRACE(COMP_ERR, "MgntActSet_"
							 "RF_State(): Wait too "
							 "logn to set RF\n");
						return false;
					}
				}
			} else {
				priv->RFChangeInProgress = true;
				spin_unlock_irqrestore(&priv->rf_ps_lock, flag);
				break;
			}
		}
	}

	rtState = priv->rtllib->eRFPowerState;

	switch (StateToSet) {
	case eRfOn:
		priv->rtllib->RfOffReason &= (~ChangeSource);

		if ((ChangeSource == RF_CHANGE_BY_HW) &&
		    (priv->bHwRadioOff == true))
			priv->bHwRadioOff = false;

		if (!priv->rtllib->RfOffReason) {
			priv->rtllib->RfOffReason = 0;
			bActionAllowed = true;


			if (rtState == eRfOff &&
			    ChangeSource >= RF_CHANGE_BY_HW)
				bConnectBySSID = true;
		} else {
			RT_TRACE((COMP_PS | COMP_RF), "MgntActSet_RF_State - "
				 "eRfon reject pMgntInfo->RfOffReason= 0x%x,"
				 " ChangeSource=0x%X\n",
				  priv->rtllib->RfOffReason, ChangeSource);
	}

		break;

	case eRfOff:

		if ((priv->rtllib->iw_mode == IW_MODE_INFRA) ||
		    (priv->rtllib->iw_mode == IW_MODE_ADHOC)) {
			if ((priv->rtllib->RfOffReason > RF_CHANGE_BY_IPS) ||
			    (ChangeSource > RF_CHANGE_BY_IPS)) {
				if (ieee->state == RTLLIB_LINKED)
					priv->blinked_ingpio = true;
				else
					priv->blinked_ingpio = false;
				rtllib_MgntDisconnect(priv->rtllib,
						      disas_lv_ss);
			}
		}
		if ((ChangeSource == RF_CHANGE_BY_HW) &&
		     (priv->bHwRadioOff == false))
			priv->bHwRadioOff = true;
		priv->rtllib->RfOffReason |= ChangeSource;
		bActionAllowed = true;
		break;

	case eRfSleep:
		priv->rtllib->RfOffReason |= ChangeSource;
		bActionAllowed = true;
=======
 *  -----------------------------GENERAL FUNCTION-------------------------
 ****************************************************************************/
bool rtl92e_set_rf_state(struct net_device *dev,
			 enum rt_rf_power_state state_to_set,
			 RT_RF_CHANGE_SOURCE change_source)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rtllib_device *ieee = priv->rtllib;
	bool action_allowed = false;
	bool connect_by_ssid = false;
	enum rt_rf_power_state rt_state;
	u16 rf_wait_counter = 0;
	unsigned long flag;

	while (true) {
		spin_lock_irqsave(&priv->rf_ps_lock, flag);
		if (priv->rf_change_in_progress) {
			spin_unlock_irqrestore(&priv->rf_ps_lock, flag);

			while (priv->rf_change_in_progress) {
				rf_wait_counter++;
				mdelay(1);

				if (rf_wait_counter > 100) {
					netdev_warn(dev,
						    "%s(): Timeout waiting for RF change.\n",
						    __func__);
					return false;
				}
			}
		} else {
			priv->rf_change_in_progress = true;
			spin_unlock_irqrestore(&priv->rf_ps_lock, flag);
			break;
		}
	}

	rt_state = priv->rtllib->rf_power_state;

	switch (state_to_set) {
	case rf_on:
		priv->rtllib->rf_off_reason &= (~change_source);

		if ((change_source == RF_CHANGE_BY_HW) && priv->hw_radio_off)
			priv->hw_radio_off = false;

		if (!priv->rtllib->rf_off_reason) {
			priv->rtllib->rf_off_reason = 0;
			action_allowed = true;

			if (rt_state == rf_off &&
			    change_source >= RF_CHANGE_BY_HW)
				connect_by_ssid = true;
		}
		break;

	case rf_off:

		if (priv->rtllib->iw_mode == IW_MODE_INFRA) {
			if ((priv->rtllib->rf_off_reason > RF_CHANGE_BY_IPS) ||
			    (change_source > RF_CHANGE_BY_IPS)) {
				if (ieee->link_state == MAC80211_LINKED)
					priv->blinked_ingpio = true;
				else
					priv->blinked_ingpio = false;
				rtllib_mgnt_disconnect(priv->rtllib,
						      WLAN_REASON_DISASSOC_STA_HAS_LEFT);
			}
		}
		if ((change_source == RF_CHANGE_BY_HW) && !priv->hw_radio_off)
			priv->hw_radio_off = true;
		priv->rtllib->rf_off_reason |= change_source;
		action_allowed = true;
		break;

	case rf_sleep:
		priv->rtllib->rf_off_reason |= change_source;
		action_allowed = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	default:
		break;
	}

<<<<<<< HEAD
	if (bActionAllowed) {
		RT_TRACE((COMP_PS | COMP_RF), "MgntActSet_RF_State(): Action is"
			 " allowed.... StateToSet(%d), RfOffReason(%#X)\n",
			 StateToSet, priv->rtllib->RfOffReason);
		PHY_SetRFPowerState(dev, StateToSet);
		if (StateToSet == eRfOn) {

			if (bConnectBySSID && (priv->blinked_ingpio == true)) {
				queue_delayed_work_rsl(ieee->wq,
=======
	if (action_allowed) {
		rtl92e_set_rf_power_state(dev, state_to_set);
		if (state_to_set == rf_on) {
			if (connect_by_ssid && priv->blinked_ingpio) {
				schedule_delayed_work(
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					 &ieee->associate_procedure_wq, 0);
				priv->blinked_ingpio = false;
			}
		}
<<<<<<< HEAD
	} else {
		RT_TRACE((COMP_PS | COMP_RF), "MgntActSet_RF_State(): "
			 "Action is rejected.... StateToSet(%d), ChangeSource"
			 "(%#X), RfOffReason(%#X)\n", StateToSet, ChangeSource,
			 priv->rtllib->RfOffReason);
	}

	if (!ProtectOrNot) {
		spin_lock_irqsave(&priv->rf_ps_lock, flag);
		priv->RFChangeInProgress = false;
		spin_unlock_irqrestore(&priv->rf_ps_lock, flag);
	}

	RT_TRACE((COMP_PS | COMP_RF), "<===MgntActSet_RF_State()\n");
	return bActionAllowed;
}


static short rtl8192_get_nic_desc_num(struct net_device *dev, int prio)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rtl8192_tx_ring *ring = &priv->tx_ring[prio];

	/* For now, we reserved two free descriptor as a safety boundary
	* between the tail and the head
	*/
	if ((prio == MGNT_QUEUE) && (skb_queue_len(&ring->queue) > 10))
		RT_TRACE(COMP_DBG, "-----[%d]---------ring->idx=%d "
			 "queue_len=%d---------\n", prio, ring->idx,
			 skb_queue_len(&ring->queue));
	return skb_queue_len(&ring->queue);
}

static short rtl8192_check_nic_enough_desc(struct net_device *dev, int prio)
=======
	}

	spin_lock_irqsave(&priv->rf_ps_lock, flag);
	priv->rf_change_in_progress = false;
	spin_unlock_irqrestore(&priv->rf_ps_lock, flag);
	return action_allowed;
}

static short _rtl92e_check_nic_enough_desc(struct net_device *dev, int prio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rtl8192_tx_ring *ring = &priv->tx_ring[prio];

	if (ring->entries - skb_queue_len(&ring->queue) >= 2)
		return 1;
	return 0;
}

<<<<<<< HEAD
void rtl8192_tx_timeout(struct net_device *dev)
=======
static void _rtl92e_tx_timeout(struct net_device *dev, unsigned int txqueue)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	schedule_work(&priv->reset_wq);
<<<<<<< HEAD
	printk(KERN_INFO "TXTIMEOUT");
}

void rtl8192_irq_enable(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	priv->irq_enabled = 1;

	priv->ops->irq_enable(dev);
}

void rtl8192_irq_disable(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);

	priv->ops->irq_disable(dev);

	priv->irq_enabled = 0;
}

void rtl8192_set_chan(struct net_device *dev, short ch)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);

	RT_TRACE(COMP_CH, "=====>%s()====ch:%d\n", __func__, ch);
	if (priv->chan_forced)
		return;

	priv->chan = ch;

	if (priv->rf_set_chan)
		priv->rf_set_chan(dev, priv->chan);
}

void rtl8192_update_cap(struct net_device *dev, u16 cap)
=======
	netdev_info(dev, "TXTIMEOUT");
}

static void _rtl92e_update_cap(struct net_device *dev, u16 cap)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rtllib_network *net = &priv->rtllib->current_network;
	bool		ShortPreamble;

	if (cap & WLAN_CAPABILITY_SHORT_PREAMBLE) {
<<<<<<< HEAD
		if (priv->dot11CurrentPreambleMode != PREAMBLE_SHORT) {
			ShortPreamble = true;
			priv->dot11CurrentPreambleMode = PREAMBLE_SHORT;
			RT_TRACE(COMP_DBG, "%s(): WLAN_CAPABILITY_SHORT_"
				 "PREAMBLE\n", __func__);
			priv->rtllib->SetHwRegHandler(dev, HW_VAR_ACK_PREAMBLE,
					(unsigned char *)&ShortPreamble);
		}
	} else {
		if (priv->dot11CurrentPreambleMode != PREAMBLE_LONG) {
			ShortPreamble = false;
			priv->dot11CurrentPreambleMode = PREAMBLE_LONG;
			RT_TRACE(COMP_DBG, "%s(): WLAN_CAPABILITY_LONG_"
				 "PREAMBLE\n", __func__);
			priv->rtllib->SetHwRegHandler(dev, HW_VAR_ACK_PREAMBLE,
=======
		if (priv->dot11_current_preamble_mode != PREAMBLE_SHORT) {
			ShortPreamble = true;
			priv->dot11_current_preamble_mode = PREAMBLE_SHORT;
			priv->rtllib->set_hw_reg_handler(dev, HW_VAR_ACK_PREAMBLE,
					(unsigned char *)&ShortPreamble);
		}
	} else {
		if (priv->dot11_current_preamble_mode != PREAMBLE_LONG) {
			ShortPreamble = false;
			priv->dot11_current_preamble_mode = PREAMBLE_LONG;
			priv->rtllib->set_hw_reg_handler(dev, HW_VAR_ACK_PREAMBLE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					      (unsigned char *)&ShortPreamble);
		}
	}

<<<<<<< HEAD
	if (net->mode & (IEEE_G|IEEE_N_24G)) {
		u8	slot_time_val;
		u8	CurSlotTime = priv->slot_time;

		if ((cap & WLAN_CAPABILITY_SHORT_SLOT_TIME) &&
		   (!priv->rtllib->pHTInfo->bCurrentRT2RTLongSlotTime)) {
			if (CurSlotTime != SHORT_SLOT_TIME) {
				slot_time_val = SHORT_SLOT_TIME;
				priv->rtllib->SetHwRegHandler(dev,
					 HW_VAR_SLOT_TIME, &slot_time_val);
			}
		} else {
			if (CurSlotTime != NON_SHORT_SLOT_TIME) {
				slot_time_val = NON_SHORT_SLOT_TIME;
				priv->rtllib->SetHwRegHandler(dev,
=======
	if (net->mode & (WIRELESS_MODE_G | WIRELESS_MODE_N_24G)) {
		u8	slot_time_val;
		u8	cur_slot_time = priv->slot_time;

		if ((cap & WLAN_CAPABILITY_SHORT_SLOT_TIME) &&
		   (!priv->rtllib->ht_info->current_rt2rt_long_slot_time)) {
			if (cur_slot_time != SHORT_SLOT_TIME) {
				slot_time_val = SHORT_SLOT_TIME;
				priv->rtllib->set_hw_reg_handler(dev,
					 HW_VAR_SLOT_TIME, &slot_time_val);
			}
		} else {
			if (cur_slot_time != NON_SHORT_SLOT_TIME) {
				slot_time_val = NON_SHORT_SLOT_TIME;
				priv->rtllib->set_hw_reg_handler(dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					 HW_VAR_SLOT_TIME, &slot_time_val);
			}
		}
	}
}

<<<<<<< HEAD
static struct rtllib_qos_parameters def_qos_parameters = {
	{3, 3, 3, 3},
	{7, 7, 7, 7},
=======
static const struct rtllib_qos_parameters def_qos_parameters = {
	{cpu_to_le16(3), cpu_to_le16(3), cpu_to_le16(3), cpu_to_le16(3)},
	{cpu_to_le16(7), cpu_to_le16(7), cpu_to_le16(7), cpu_to_le16(7)},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{2, 2, 2, 2},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

<<<<<<< HEAD
static void rtl8192_update_beacon(void *data)
{
	struct r8192_priv *priv = container_of_work_rsl(data, struct r8192_priv,
				  update_beacon_wq.work);
=======
static void _rtl92e_update_beacon(void *data)
{
	struct r8192_priv *priv = container_of(data, struct r8192_priv, update_beacon_wq.work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_device *dev = priv->rtllib->dev;
	struct rtllib_device *ieee = priv->rtllib;
	struct rtllib_network *net = &ieee->current_network;

<<<<<<< HEAD
	if (ieee->pHTInfo->bCurrentHTSupport)
		HT_update_self_and_peer_setting(ieee, net);
	ieee->pHTInfo->bCurrentRT2RTLongSlotTime =
		 net->bssht.bdRT2RTLongSlotTime;
	ieee->pHTInfo->RT2RT_HT_Mode = net->bssht.RT2RT_HT_Mode;
	rtl8192_update_cap(dev, net->capability);
}

int WDCAPARA_ADD[] = {EDCAPARA_BE, EDCAPARA_BK, EDCAPARA_VI, EDCAPARA_VO};

static void rtl8192_qos_activate(void *data)
{
	struct r8192_priv *priv = container_of_work_rsl(data, struct r8192_priv,
				  qos_activate);
=======
	if (ieee->ht_info->current_ht_support)
		HT_update_self_and_peer_setting(ieee, net);
	ieee->ht_info->current_rt2rt_long_slot_time = net->bssht.bd_rt2rt_long_slot_time;
	_rtl92e_update_cap(dev, net->capability);
}

static void _rtl92e_qos_activate(void *data)
{
	struct r8192_priv *priv = container_of(data, struct r8192_priv, qos_activate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_device *dev = priv->rtllib->dev;
	int i;

	mutex_lock(&priv->mutex);
<<<<<<< HEAD
	if (priv->rtllib->state != RTLLIB_LINKED)
		goto success;
	RT_TRACE(COMP_QOS, "qos active process with associate response "
		 "received\n");

	for (i = 0; i <  QOS_QUEUE_NUM; i++) {
		priv->rtllib->SetHwRegHandler(dev, HW_VAR_AC_PARAM, (u8 *)(&i));
	}
=======
	if (priv->rtllib->link_state != MAC80211_LINKED)
		goto success;

	for (i = 0; i <  QOS_QUEUE_NUM; i++)
		priv->rtllib->set_hw_reg_handler(dev, HW_VAR_AC_PARAM, (u8 *)(&i));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

success:
	mutex_unlock(&priv->mutex);
}

<<<<<<< HEAD
static int rtl8192_qos_handle_probe_response(struct r8192_priv *priv,
		int active_network,
		struct rtllib_network *network)
=======
static int _rtl92e_qos_handle_probe_response(struct r8192_priv *priv,
					     int active_network,
					     struct rtllib_network *network)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret = 0;
	u32 size = sizeof(struct rtllib_qos_parameters);

<<<<<<< HEAD
	if (priv->rtllib->state != RTLLIB_LINKED)
		return ret;

	if ((priv->rtllib->iw_mode != IW_MODE_INFRA))
=======
	if (priv->rtllib->link_state != MAC80211_LINKED)
		return ret;

	if (priv->rtllib->iw_mode != IW_MODE_INFRA)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;

	if (network->flags & NETWORK_HAS_QOS_MASK) {
		if (active_network &&
				(network->flags & NETWORK_HAS_QOS_PARAMETERS))
			network->qos_data.active = network->qos_data.supported;

		if ((network->qos_data.active == 1) && (active_network == 1) &&
				(network->flags & NETWORK_HAS_QOS_PARAMETERS) &&
				(network->qos_data.old_param_count !=
				network->qos_data.param_count)) {
			network->qos_data.old_param_count =
				network->qos_data.param_count;
<<<<<<< HEAD
	priv->rtllib->wmm_acm = network->qos_data.wmm_acm;
			queue_work_rsl(priv->priv_wq, &priv->qos_activate);
			RT_TRACE(COMP_QOS, "QoS parameters change call "
					"qos_activate\n");
=======
			priv->rtllib->wmm_acm = network->qos_data.wmm_acm;
			schedule_work(&priv->qos_activate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	} else {
		memcpy(&priv->rtllib->current_network.qos_data.parameters,
		       &def_qos_parameters, size);

<<<<<<< HEAD
		if ((network->qos_data.active == 1) && (active_network == 1)) {
			queue_work_rsl(priv->priv_wq, &priv->qos_activate);
			RT_TRACE(COMP_QOS, "QoS was disabled call qos_"
				 "activate\n");
		}
=======
		if ((network->qos_data.active == 1) && (active_network == 1))
			schedule_work(&priv->qos_activate);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		network->qos_data.active = 0;
		network->qos_data.supported = 0;
	}

	return 0;
}

<<<<<<< HEAD
static int rtl8192_handle_beacon(struct net_device *dev,
	struct rtllib_beacon *beacon,
	struct rtllib_network *network)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	rtl8192_qos_handle_probe_response(priv, 1, network);

	queue_delayed_work_rsl(priv->priv_wq, &priv->update_beacon_wq, 0);
	return 0;

}

static int rtl8192_qos_association_resp(struct r8192_priv *priv,
	struct rtllib_network *network)
{
	int ret = 0;
=======
static int _rtl92e_handle_beacon(struct net_device *dev,
				 struct rtllib_beacon *beacon,
				 struct rtllib_network *network)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	_rtl92e_qos_handle_probe_response(priv, 1, network);

	schedule_delayed_work(&priv->update_beacon_wq, 0);
	return 0;
}

static int _rtl92e_qos_assoc_resp(struct r8192_priv *priv,
				  struct rtllib_network *network)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	u32 size = sizeof(struct rtllib_qos_parameters);
	int set_qos_param = 0;

<<<<<<< HEAD
	if ((priv == NULL) || (network == NULL))
		return ret;

	if (priv->rtllib->state != RTLLIB_LINKED)
		return ret;

	if ((priv->rtllib->iw_mode != IW_MODE_INFRA))
		return ret;
=======
	if (!priv || !network)
		return 0;

	if (priv->rtllib->link_state != MAC80211_LINKED)
		return 0;

	if (priv->rtllib->iw_mode != IW_MODE_INFRA)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&priv->rtllib->lock, flags);
	if (network->flags & NETWORK_HAS_QOS_PARAMETERS) {
		memcpy(&priv->rtllib->current_network.qos_data.parameters,
		       &network->qos_data.parameters,
		       sizeof(struct rtllib_qos_parameters));
		priv->rtllib->current_network.qos_data.active = 1;
		priv->rtllib->wmm_acm = network->qos_data.wmm_acm;
		set_qos_param = 1;
		priv->rtllib->current_network.qos_data.old_param_count =
			priv->rtllib->current_network.qos_data.param_count;
		priv->rtllib->current_network.qos_data.param_count =
			network->qos_data.param_count;
	} else {
		memcpy(&priv->rtllib->current_network.qos_data.parameters,
		&def_qos_parameters, size);
		priv->rtllib->current_network.qos_data.active = 0;
		priv->rtllib->current_network.qos_data.supported = 0;
		set_qos_param = 1;
	}

	spin_unlock_irqrestore(&priv->rtllib->lock, flags);

<<<<<<< HEAD
	RT_TRACE(COMP_QOS, "%s: network->flags = %d,%d\n", __func__,
		 network->flags, priv->rtllib->current_network.qos_data.active);
	if (set_qos_param == 1) {
		dm_init_edca_turbo(priv->rtllib->dev);
		queue_work_rsl(priv->priv_wq, &priv->qos_activate);
	}
	return ret;
}

static int rtl8192_handle_assoc_response(struct net_device *dev,
=======
	if (set_qos_param == 1) {
		rtl92e_dm_init_edca_turbo(priv->rtllib->dev);
		schedule_work(&priv->qos_activate);
	}
	return 0;
}

static int _rtl92e_handle_assoc_response(struct net_device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 struct rtllib_assoc_response_frame *resp,
				 struct rtllib_network *network)
{
	struct r8192_priv *priv = rtllib_priv(dev);
<<<<<<< HEAD
	rtl8192_qos_association_resp(priv, network);
	return 0;
}

static void rtl8192_prepare_beacon(struct r8192_priv *priv)
{
	struct net_device *dev = priv->rtllib->dev;
	struct sk_buff *pskb = NULL, *pnewskb = NULL;
	struct cb_desc *tcb_desc = NULL;
	struct rtl8192_tx_ring *ring = NULL;
	struct tx_desc *pdesc = NULL;

	ring = &priv->tx_ring[BEACON_QUEUE];
	pskb = __skb_dequeue(&ring->queue);
	if (pskb)
		kfree_skb(pskb);

	pnewskb = rtllib_get_beacon(priv->rtllib);
	if (!pnewskb)
		return;

	tcb_desc = (struct cb_desc *)(pnewskb->cb + 8);
	tcb_desc->queue_index = BEACON_QUEUE;
	tcb_desc->data_rate = 2;
	tcb_desc->RATRIndex = 7;
	tcb_desc->bTxDisableRateFallBack = 1;
	tcb_desc->bTxUseDriverAssingedRate = 1;
	skb_push(pnewskb, priv->rtllib->tx_headroom);

	pdesc = &ring->desc[0];
	priv->ops->tx_fill_descriptor(dev, pdesc, tcb_desc, pnewskb);
	__skb_queue_tail(&ring->queue, pnewskb);
	pdesc->OWN = 1;

	return;
}

static void rtl8192_stop_beacon(struct net_device *dev)
{
}

void rtl8192_config_rate(struct net_device *dev, u16 *rate_config)
=======

	_rtl92e_qos_assoc_resp(priv, network);
	return 0;
}

void rtl92e_config_rate(struct net_device *dev, u16 *rate_config)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rtllib_network *net;
	u8 i = 0, basic_rate = 0;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	net = &priv->rtllib->current_network;

	for (i = 0; i < net->rates_len; i++) {
		basic_rate = net->rates[i] & 0x7f;
		switch (basic_rate) {
		case MGN_1M:
			*rate_config |= RRSR_1M;
			break;
		case MGN_2M:
			*rate_config |= RRSR_2M;
			break;
		case MGN_5_5M:
			*rate_config |= RRSR_5_5M;
			break;
		case MGN_11M:
			*rate_config |= RRSR_11M;
			break;
		case MGN_6M:
			*rate_config |= RRSR_6M;
			break;
		case MGN_9M:
			*rate_config |= RRSR_9M;
			break;
		case MGN_12M:
			*rate_config |= RRSR_12M;
			break;
		case MGN_18M:
			*rate_config |= RRSR_18M;
			break;
		case MGN_24M:
			*rate_config |= RRSR_24M;
			break;
		case MGN_36M:
			*rate_config |= RRSR_36M;
			break;
		case MGN_48M:
			*rate_config |= RRSR_48M;
			break;
		case MGN_54M:
			*rate_config |= RRSR_54M;
			break;
		}
	}

	for (i = 0; i < net->rates_ex_len; i++) {
		basic_rate = net->rates_ex[i] & 0x7f;
		switch (basic_rate) {
		case MGN_1M:
			*rate_config |= RRSR_1M;
			break;
		case MGN_2M:
			*rate_config |= RRSR_2M;
			break;
		case MGN_5_5M:
			*rate_config |= RRSR_5_5M;
			break;
		case MGN_11M:
			*rate_config |= RRSR_11M;
			break;
		case MGN_6M:
			*rate_config |= RRSR_6M;
			break;
		case MGN_9M:
			*rate_config |= RRSR_9M;
			break;
		case MGN_12M:
			*rate_config |= RRSR_12M;
			break;
		case MGN_18M:
			*rate_config |= RRSR_18M;
			break;
		case MGN_24M:
			*rate_config |= RRSR_24M;
			break;
		case MGN_36M:
			*rate_config |= RRSR_36M;
			break;
		case MGN_48M:
			*rate_config |= RRSR_48M;
			break;
		case MGN_54M:
			*rate_config |= RRSR_54M;
			break;
		}
	}
}

<<<<<<< HEAD
static void rtl8192_refresh_supportrate(struct r8192_priv *priv)
{
	struct rtllib_device *ieee = priv->rtllib;
	if (ieee->mode == WIRELESS_MODE_N_24G ||
	    ieee->mode == WIRELESS_MODE_N_5G) {
		memcpy(ieee->Regdot11HTOperationalRateSet,
		       ieee->RegHTSuppRateSet, 16);
		memcpy(ieee->Regdot11TxHTOperationalRateSet,
		       ieee->RegHTSuppRateSet, 16);

	} else {
		memset(ieee->Regdot11HTOperationalRateSet, 0, 16);
	}
	return;
}

static u8 rtl8192_getSupportedWireleeMode(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	u8 ret = 0;

	switch (priv->rf_chip) {
	case RF_8225:
	case RF_8256:
	case RF_6052:
	case RF_PSEUDO_11N:
		ret = (WIRELESS_MODE_N_24G|WIRELESS_MODE_G | WIRELESS_MODE_B);
		break;
	case RF_8258:
		ret = (WIRELESS_MODE_A | WIRELESS_MODE_N_5G);
		break;
	default:
		ret = WIRELESS_MODE_B;
		break;
	}
	return ret;
}

void rtl8192_SetWirelessMode(struct net_device *dev, u8 wireless_mode)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	u8 bSupportMode = rtl8192_getSupportedWireleeMode(dev);

	if ((wireless_mode == WIRELESS_MODE_AUTO) ||
	    ((wireless_mode & bSupportMode) == 0)) {
		if (bSupportMode & WIRELESS_MODE_N_24G) {
			wireless_mode = WIRELESS_MODE_N_24G;
		} else if (bSupportMode & WIRELESS_MODE_N_5G) {
			wireless_mode = WIRELESS_MODE_N_5G;
		} else if ((bSupportMode & WIRELESS_MODE_A)) {
			wireless_mode = WIRELESS_MODE_A;
		} else if ((bSupportMode & WIRELESS_MODE_G)) {
			wireless_mode = WIRELESS_MODE_G;
		} else if ((bSupportMode & WIRELESS_MODE_B)) {
			wireless_mode = WIRELESS_MODE_B;
		} else {
			RT_TRACE(COMP_ERR, "%s(), No valid wireless mode "
				 "supported (%x)!!!\n", __func__, bSupportMode);
			wireless_mode = WIRELESS_MODE_B;
		}
	}
=======
static void _rtl92e_refresh_support_rate(struct r8192_priv *priv)
{
	struct rtllib_device *ieee = priv->rtllib;

	if (ieee->mode == WIRELESS_MODE_N_24G) {
		memcpy(ieee->reg_dot11ht_oper_rate_set,
		       ieee->reg_ht_supp_rate_set, 16);
		memcpy(ieee->reg_dot11tx_ht_oper_rate_set,
		       ieee->reg_ht_supp_rate_set, 16);

	} else {
		memset(ieee->reg_dot11ht_oper_rate_set, 0, 16);
	}
}

void rtl92e_set_wireless_mode(struct net_device *dev, u8 wireless_mode)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	u8 support_mode = (WIRELESS_MODE_N_24G | WIRELESS_MODE_G | WIRELESS_MODE_B);

	if ((wireless_mode == WIRELESS_MODE_AUTO) || ((wireless_mode & support_mode) == 0))
		wireless_mode = WIRELESS_MODE_N_24G;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((wireless_mode & (WIRELESS_MODE_B | WIRELESS_MODE_G)) ==
	    (WIRELESS_MODE_G | WIRELESS_MODE_B))
		wireless_mode = WIRELESS_MODE_G;

	priv->rtllib->mode = wireless_mode;

<<<<<<< HEAD
	ActUpdateChannelAccessSetting(dev, wireless_mode,
				      &priv->ChannelAccessSetting);

	if ((wireless_mode == WIRELESS_MODE_N_24G) ||
	    (wireless_mode == WIRELESS_MODE_N_5G)) {
		priv->rtllib->pHTInfo->bEnableHT = 1;
	RT_TRACE(COMP_DBG, "%s(), wireless_mode:%x, bEnableHT = 1\n",
		 __func__, wireless_mode);
	} else {
		priv->rtllib->pHTInfo->bEnableHT = 0;
		RT_TRACE(COMP_DBG, "%s(), wireless_mode:%x, bEnableHT = 0\n",
			 __func__, wireless_mode);
	}

	RT_TRACE(COMP_INIT, "Current Wireless Mode is %x\n", wireless_mode);
	rtl8192_refresh_supportrate(priv);
}

static int _rtl8192_sta_up(struct net_device *dev, bool is_silent_reset)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *pPSC = (struct rt_pwr_save_ctrl *)
					(&(priv->rtllib->PowerSaveControl));
	bool init_status = true;
	priv->bDriverIsGoingToUnload = false;
	priv->bdisable_nic = false;
=======
	if (wireless_mode == WIRELESS_MODE_N_24G)
		priv->rtllib->ht_info->enable_ht = 1;
	else
		priv->rtllib->ht_info->enable_ht = 0;

	_rtl92e_refresh_support_rate(priv);
}

static int _rtl92e_sta_up(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *psc = (struct rt_pwr_save_ctrl *)
					(&priv->rtllib->pwr_save_ctrl);
	bool init_status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	priv->up = 1;
	priv->rtllib->ieee_up = 1;

	priv->up_first_time = 0;
<<<<<<< HEAD
	RT_TRACE(COMP_INIT, "Bringing up iface");
	priv->bfirst_init = true;
	init_status = priv->ops->initialize_adapter(dev);
	if (init_status != true) {
		RT_TRACE(COMP_ERR, "ERR!!! %s(): initialization is failed!\n",
			 __func__);
		priv->bfirst_init = false;
		return -1;
	}

	RT_TRACE(COMP_INIT, "start adapter finished\n");
	RT_CLEAR_PS_LEVEL(pPSC, RT_RF_OFF_LEVL_HALT_NIC);
	priv->bfirst_init = false;

	if (priv->polling_timer_on == 0)
		check_rfctrl_gpio_timer((unsigned long)dev);

	if (priv->rtllib->state != RTLLIB_LINKED)
		rtllib_softmac_start_protocol(priv->rtllib, 0);
	rtllib_reset_queue(priv->rtllib);
	watch_dog_timer_callback((unsigned long) dev);
=======
	init_status = rtl92e_start_adapter(dev);
	if (!init_status) {
		netdev_err(dev, "%s(): Initialization failed!\n", __func__);
		return -1;
	}

	RT_CLEAR_PS_LEVEL(psc, RT_RF_OFF_LEVL_HALT_NIC);

	if (priv->polling_timer_on == 0)
		rtl92e_check_rfctrl_gpio_timer(&priv->gpio_polling_timer);

	if (priv->rtllib->link_state != MAC80211_LINKED)
		rtllib_softmac_start_protocol(priv->rtllib);
	rtllib_reset_queue(priv->rtllib);
	_rtl92e_watchdog_timer_cb(&priv->watch_dog_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!netif_queue_stopped(dev))
		netif_start_queue(dev);
	else
		netif_wake_queue(dev);

<<<<<<< HEAD
	return 0;
}

static int rtl8192_sta_down(struct net_device *dev, bool shutdownrf)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	unsigned long flags = 0;
	u8 RFInProgressTimeOut = 0;
=======
	priv->bfirst_after_down = false;
	return 0;
}

static int _rtl92e_sta_down(struct net_device *dev, bool shutdownrf)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	unsigned long flags = 0;
	u8 rf_in_progress_timeout = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (priv->up == 0)
		return -1;

<<<<<<< HEAD
	if (priv->rtllib->rtllib_ips_leave != NULL)
		priv->rtllib->rtllib_ips_leave(dev);

	if (priv->rtllib->state == RTLLIB_LINKED)
		LeisurePSLeave(dev);

	priv->bDriverIsGoingToUnload = true;
	priv->up = 0;
	priv->rtllib->ieee_up = 0;
	priv->bfirst_after_down = 1;
	RT_TRACE(COMP_DOWN, "==========>%s()\n", __func__);
=======
	priv->rtllib->rtllib_ips_leave(dev);

	if (priv->rtllib->link_state == MAC80211_LINKED)
		rtl92e_leisure_ps_leave(dev);

	priv->up = 0;
	priv->rtllib->ieee_up = 0;
	priv->bfirst_after_down = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!netif_queue_stopped(dev))
		netif_stop_queue(dev);

	priv->rtllib->wpa_ie_len = 0;
	kfree(priv->rtllib->wpa_ie);
	priv->rtllib->wpa_ie = NULL;
<<<<<<< HEAD
	CamResetAllEntry(dev);
	memset(priv->rtllib->swcamtable, 0, sizeof(struct sw_cam_table) * 32);
	rtl8192_irq_disable(dev);

	del_timer_sync(&priv->watch_dog_timer);
	rtl8192_cancel_deferred_work(priv);
	cancel_delayed_work(&priv->rtllib->hw_wakeup_wq);

	rtllib_softmac_stop_protocol(priv->rtllib, 0, true);
	spin_lock_irqsave(&priv->rf_ps_lock, flags);
	while (priv->RFChangeInProgress) {
		spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
		if (RFInProgressTimeOut > 100) {
			spin_lock_irqsave(&priv->rf_ps_lock, flags);
			break;
		}
		RT_TRACE(COMP_DBG, "===>%s():RF is in progress, need to wait "
			 "until rf chang is done.\n", __func__);
		mdelay(1);
		RFInProgressTimeOut++;
		spin_lock_irqsave(&priv->rf_ps_lock, flags);
	}
	priv->RFChangeInProgress = true;
	spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
	priv->ops->stop_adapter(dev, false);
	spin_lock_irqsave(&priv->rf_ps_lock, flags);
	priv->RFChangeInProgress = false;
=======
	rtl92e_cam_reset(dev);
	memset(priv->rtllib->swcamtable, 0, sizeof(struct sw_cam_table) * 32);
	rtl92e_irq_disable(dev);

	del_timer_sync(&priv->watch_dog_timer);
	_rtl92e_cancel_deferred_work(priv);
	cancel_delayed_work(&priv->rtllib->hw_wakeup_wq);

	rtllib_softmac_stop_protocol(priv->rtllib);
	spin_lock_irqsave(&priv->rf_ps_lock, flags);
	while (priv->rf_change_in_progress) {
		spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
		if (rf_in_progress_timeout > 100) {
			spin_lock_irqsave(&priv->rf_ps_lock, flags);
			break;
		}
		mdelay(1);
		rf_in_progress_timeout++;
		spin_lock_irqsave(&priv->rf_ps_lock, flags);
	}
	priv->rf_change_in_progress = true;
	spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
	rtl92e_stop_adapter(dev, false);
	spin_lock_irqsave(&priv->rf_ps_lock, flags);
	priv->rf_change_in_progress = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
	udelay(100);
	memset(&priv->rtllib->current_network, 0,
	       offsetof(struct rtllib_network, list));
<<<<<<< HEAD
	RT_TRACE(COMP_DOWN, "<==========%s()\n", __func__);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void rtl8192_init_priv_handler(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	priv->rtllib->softmac_hard_start_xmit	= rtl8192_hard_start_xmit;
	priv->rtllib->set_chan			= rtl8192_set_chan;
	priv->rtllib->link_change		= priv->ops->link_change;
	priv->rtllib->softmac_data_hard_start_xmit = rtl8192_hard_data_xmit;
	priv->rtllib->data_hard_stop		= rtl8192_data_hard_stop;
	priv->rtllib->data_hard_resume		= rtl8192_data_hard_resume;
	priv->rtllib->check_nic_enough_desc	= rtl8192_check_nic_enough_desc;
	priv->rtllib->get_nic_desc_num		= rtl8192_get_nic_desc_num;
	priv->rtllib->handle_assoc_response	= rtl8192_handle_assoc_response;
	priv->rtllib->handle_beacon		= rtl8192_handle_beacon;
	priv->rtllib->SetWirelessMode		= rtl8192_SetWirelessMode;
	priv->rtllib->LeisurePSLeave		= LeisurePSLeave;
	priv->rtllib->SetBWModeHandler		= rtl8192_SetBWMode;
	priv->rf_set_chan			= rtl8192_phy_SwChnl;

	priv->rtllib->start_send_beacons = rtl8192e_start_beacon;
	priv->rtllib->stop_send_beacons = rtl8192_stop_beacon;

	priv->rtllib->sta_wake_up = rtl8192_hw_wakeup;
	priv->rtllib->enter_sleep_state = rtl8192_hw_to_sleep;
	priv->rtllib->ps_is_queue_empty = rtl8192_is_tx_queue_empty;

	priv->rtllib->GetNmodeSupportBySecCfg = rtl8192_GetNmodeSupportBySecCfg;
	priv->rtllib->GetHalfNmodeSupportByAPsHandler =
					 rtl8192_GetHalfNmodeSupportByAPs;

	priv->rtllib->SetHwRegHandler = rtl8192e_SetHwReg;
	priv->rtllib->AllowAllDestAddrHandler = rtl8192_AllowAllDestAddr;
	priv->rtllib->SetFwCmdHandler = NULL;
	priv->rtllib->InitialGainHandler = InitialGain819xPci;
	priv->rtllib->rtllib_ips_leave_wq = rtllib_ips_leave_wq;
	priv->rtllib->rtllib_ips_leave = rtllib_ips_leave;

	priv->rtllib->LedControlHandler = NULL;
	priv->rtllib->UpdateBeaconInterruptHandler = NULL;

	priv->rtllib->ScanOperationBackupHandler = PHY_ScanOperationBackup8192;

	priv->rtllib->rtllib_rfkill_poll = NULL;
}

static void rtl8192_init_priv_constant(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *pPSC = (struct rt_pwr_save_ctrl *)
					&(priv->rtllib->PowerSaveControl);

	pPSC->RegMaxLPSAwakeIntvl = 5;

	priv->RegPciASPM = 2;

	priv->RegDevicePciASPMSetting = 0x03;

	priv->RegHostPciASPMSetting = 0x02;

	priv->RegHwSwRfOffD3 = 2;

	priv->RegSupportPciASPM = 2;
}


static void rtl8192_init_priv_variable(struct net_device *dev)
=======
static void _rtl92e_init_priv_handler(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	priv->rtllib->softmac_hard_start_xmit	= _rtl92e_hard_start_xmit;
	priv->rtllib->set_chan			= rtl92e_set_channel;
	priv->rtllib->link_change		= rtl92e_link_change;
	priv->rtllib->softmac_data_hard_start_xmit = _rtl92e_hard_data_xmit;
	priv->rtllib->check_nic_enough_desc	= _rtl92e_check_nic_enough_desc;
	priv->rtllib->handle_assoc_response	= _rtl92e_handle_assoc_response;
	priv->rtllib->handle_beacon		= _rtl92e_handle_beacon;
	priv->rtllib->set_wireless_mode		= rtl92e_set_wireless_mode;
	priv->rtllib->leisure_ps_leave		= rtl92e_leisure_ps_leave;
	priv->rtllib->set_bw_mode_handler	= rtl92e_set_bw_mode;

	priv->rtllib->sta_wake_up = rtl92e_hw_wakeup;
	priv->rtllib->enter_sleep_state = rtl92e_enter_sleep;
	priv->rtllib->ps_is_queue_empty = _rtl92e_is_tx_queue_empty;

	priv->rtllib->get_nmode_support_by_sec_cfg = rtl92e_get_nmode_support_by_sec;
	priv->rtllib->get_half_nmode_support_by_aps_handler =
						rtl92e_is_halfn_supported_by_ap;

	priv->rtllib->set_hw_reg_handler = rtl92e_set_reg;
	priv->rtllib->allow_all_dest_addr_handler = rtl92e_set_monitor_mode;
	priv->rtllib->init_gain_handler = rtl92e_init_gain;
	priv->rtllib->rtllib_ips_leave_wq = rtl92e_rtllib_ips_leave_wq;
	priv->rtllib->rtllib_ips_leave = rtl92e_rtllib_ips_leave;
	priv->rtllib->ScanOperationBackupHandler = rtl92e_scan_op_backup;
}

static void _rtl92e_init_priv_variable(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	u8 i;

<<<<<<< HEAD
	priv->AcmMethod = eAcmWay2_SW;
	priv->dot11CurrentPreambleMode = PREAMBLE_AUTO;
	priv->rtllib->hwscan_sem_up = 1;
	priv->rtllib->status = 0;
	priv->H2CTxCmdSeq = 0;
	priv->bDisableFrameBursting = 0;
	priv->bDMInitialGainEnable = 1;
	priv->polling_timer_on = 0;
	priv->up_first_time = 1;
	priv->blinked_ingpio = false;
	priv->bDriverIsGoingToUnload = false;
	priv->being_init_adapter = false;
	priv->initialized_at_probe = false;
	priv->sw_radio_on = true;
	priv->bdisable_nic = false;
	priv->bfirst_init = false;
=======
	priv->dot11_current_preamble_mode = PREAMBLE_AUTO;
	priv->rtllib->status = 0;
	priv->polling_timer_on = 0;
	priv->up_first_time = 1;
	priv->blinked_ingpio = false;
	priv->being_init_adapter = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	priv->txringcount = 64;
	priv->rxbuffersize = 9100;
	priv->rxringcount = MAX_RX_COUNT;
	priv->irq_enabled = 0;
	priv->chan = 1;
<<<<<<< HEAD
	priv->RegWirelessMode = WIRELESS_MODE_AUTO;
	priv->RegChannelPlan = 0xf;
	priv->nrxAMPDU_size = 0;
	priv->nrxAMPDU_aggr_num = 0;
	priv->last_rxdesc_tsf_high = 0;
	priv->last_rxdesc_tsf_low = 0;
	priv->rtllib->mode = WIRELESS_MODE_AUTO;
	priv->rtllib->iw_mode = IW_MODE_INFRA;
	priv->rtllib->bNetPromiscuousMode = false;
	priv->rtllib->IntelPromiscuousModeInfo.bPromiscuousOn = false;
	priv->rtllib->IntelPromiscuousModeInfo.bFilterSourceStationFrame =
								 false;
=======
	priv->rtllib->mode = WIRELESS_MODE_AUTO;
	priv->rtllib->iw_mode = IW_MODE_INFRA;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	priv->rtllib->ieee_up = 0;
	priv->retry_rts = DEFAULT_RETRY_RTS;
	priv->retry_data = DEFAULT_RETRY_DATA;
	priv->rtllib->rts = DEFAULT_RTS_THRESHOLD;
	priv->rtllib->rate = 110;
<<<<<<< HEAD
	priv->rtllib->short_slot = 1;
	priv->promisc = (dev->flags & IFF_PROMISC) ? 1 : 0;
	priv->bcck_in_ch14 = false;
	priv->bfsync_processing  = false;
	priv->CCKPresentAttentuation = 0;
	priv->rfa_txpowertrackingindex = 0;
	priv->rfc_txpowertrackingindex = 0;
	priv->CckPwEnl = 6;
	priv->ScanDelay = 50;
	priv->ResetProgress = RESET_TYPE_NORESET;
	priv->bForcedSilentReset = 0;
	priv->bDisableNormalResetCheck = false;
	priv->force_reset = false;
	memset(priv->rtllib->swcamtable, 0, sizeof(struct sw_cam_table) * 32);

	memset(&priv->InterruptLog, 0, sizeof(struct log_int_8190));
	priv->RxCounter = 0;
	priv->rtllib->wx_set_enc = 0;
	priv->bHwRadioOff = false;
	priv->RegRfOff = 0;
	priv->isRFOff = false;
	priv->bInPowerSaveMode = false;
	priv->rtllib->RfOffReason = 0;
	priv->RFChangeInProgress = false;
	priv->bHwRfOffAction = 0;
	priv->SetRFPowerStateInProgress = false;
	priv->rtllib->PowerSaveControl.bInactivePs = true;
	priv->rtllib->PowerSaveControl.bIPSModeBackup = false;
	priv->rtllib->PowerSaveControl.bLeisurePs = true;
	priv->rtllib->PowerSaveControl.bFwCtrlLPS = false;
	priv->rtllib->LPSDelayCnt = 0;
	priv->rtllib->sta_sleep = LPS_IS_WAKE;
	priv->rtllib->eRFPowerState = eRfOn;

	priv->txpower_checkcnt = 0;
	priv->thermal_readback_index = 0;
	priv->txpower_tracking_callback_cnt = 0;
	priv->ccktxpower_adjustcnt_ch14 = 0;
	priv->ccktxpower_adjustcnt_not_ch14 = 0;

	priv->rtllib->current_network.beacon_interval = DEFAULT_BEACONINTERVAL;
	priv->rtllib->iw_mode = IW_MODE_INFRA;
	priv->rtllib->active_scan = 1;
	priv->rtllib->be_scan_inprogress = false;
	priv->rtllib->modulation = RTLLIB_CCK_MODULATION |
				   RTLLIB_OFDM_MODULATION;
	priv->rtllib->host_encrypt = 1;
	priv->rtllib->host_decrypt = 1;

	priv->rtllib->dot11PowerSaveMode = eActive;
	priv->rtllib->fts = DEFAULT_FRAG_THRESHOLD;
	priv->rtllib->MaxMssDensity = 0;
	priv->rtllib->MinSpaceCfg = 0;

	priv->card_type = PCI;

	priv->AcmControl = 0;
	priv->pFirmware = vzalloc(sizeof(struct rt_firmware));
	if (!priv->pFirmware)
		printk(KERN_ERR "rtl8193e: Unable to allocate space "
		       "for firmware\n");

	skb_queue_head_init(&priv->rx_queue);
	skb_queue_head_init(&priv->skb_queue);

	for (i = 0; i < MAX_QUEUE_SIZE; i++)
		skb_queue_head_init(&priv->rtllib->skb_waitQ[i]);
	for (i = 0; i < MAX_QUEUE_SIZE; i++)
		skb_queue_head_init(&priv->rtllib->skb_aggQ[i]);
}

static void rtl8192_init_priv_lock(struct r8192_priv *priv)
{
	spin_lock_init(&priv->fw_scan_lock);
	spin_lock_init(&priv->tx_lock);
	spin_lock_init(&priv->irq_lock);
	spin_lock_init(&priv->irq_th_lock);
	spin_lock_init(&priv->rf_ps_lock);
	spin_lock_init(&priv->ps_lock);
	spin_lock_init(&priv->rf_lock);
	spin_lock_init(&priv->rt_h2c_lock);
	sema_init(&priv->wx_sem, 1);
	sema_init(&priv->rf_sem, 1);
	mutex_init(&priv->mutex);
}

static void rtl8192_init_priv_task(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	priv->priv_wq = create_workqueue(DRV_NAME);
	INIT_WORK_RSL(&priv->reset_wq, (void *)rtl8192_restart, dev);
	INIT_WORK_RSL(&priv->rtllib->ips_leave_wq, (void *)IPSLeave_wq, dev);
	INIT_DELAYED_WORK_RSL(&priv->watch_dog_wq,
			      (void *)rtl819x_watchdog_wqcallback, dev);
	INIT_DELAYED_WORK_RSL(&priv->txpower_tracking_wq,
			      (void *)dm_txpower_trackingcallback, dev);
	INIT_DELAYED_WORK_RSL(&priv->rfpath_check_wq,
			      (void *)dm_rf_pathcheck_workitemcallback, dev);
	INIT_DELAYED_WORK_RSL(&priv->update_beacon_wq,
			      (void *)rtl8192_update_beacon, dev);
	INIT_WORK_RSL(&priv->qos_activate, (void *)rtl8192_qos_activate, dev);
	INIT_DELAYED_WORK_RSL(&priv->rtllib->hw_wakeup_wq,
			      (void *) rtl8192_hw_wakeup_wq, dev);
	INIT_DELAYED_WORK_RSL(&priv->rtllib->hw_sleep_wq,
			      (void *) rtl8192_hw_sleep_wq, dev);
	tasklet_init(&priv->irq_rx_tasklet,
		     (void(*)(unsigned long))rtl8192_irq_rx_tasklet,
		     (unsigned long)priv);
	tasklet_init(&priv->irq_tx_tasklet,
		     (void(*)(unsigned long))rtl8192_irq_tx_tasklet,
		     (unsigned long)priv);
	tasklet_init(&priv->irq_prepare_beacon_tasklet,
		     (void(*)(unsigned long))rtl8192_prepare_beacon,
		     (unsigned long)priv);
}

static short rtl8192_get_channel_map(struct net_device *dev)
=======
	priv->promisc = (dev->flags & IFF_PROMISC) ? 1 : 0;
	priv->bcck_in_ch14 = false;
	priv->cck_present_attn = 0;
	priv->rfa_txpowertrackingindex = 0;
	priv->cck_pwr_enl = 6;
	memset(priv->rtllib->swcamtable, 0, sizeof(struct sw_cam_table) * 32);
	priv->rx_ctr = 0;
	priv->rtllib->wx_set_enc = 0;
	priv->hw_radio_off = false;
	priv->rtllib->rf_off_reason = 0;
	priv->rf_change_in_progress = false;
	priv->hw_rf_off_action = 0;
	priv->set_rf_pwr_state_in_progress = false;
	priv->rtllib->pwr_save_ctrl.bLeisurePs = true;
	priv->rtllib->lps_delay_cnt = 0;
	priv->rtllib->sta_sleep = LPS_IS_WAKE;
	priv->rtllib->rf_power_state = rf_on;

	priv->rtllib->current_network.beacon_interval = DEFAULT_BEACONINTERVAL;
	priv->rtllib->iw_mode = IW_MODE_INFRA;
	priv->rtllib->be_scan_inprogress = false;

	priv->rtllib->fts = DEFAULT_FRAG_THRESHOLD;

	priv->fw_info = vzalloc(sizeof(struct rt_firmware));
	if (!priv->fw_info)
		netdev_err(dev,
			   "rtl8192e: Unable to allocate space for firmware\n");

	skb_queue_head_init(&priv->skb_queue);

	for (i = 0; i < MAX_QUEUE_SIZE; i++)
		skb_queue_head_init(&priv->rtllib->skb_waitq[i]);
}

static void _rtl92e_init_priv_lock(struct r8192_priv *priv)
{
	spin_lock_init(&priv->tx_lock);
	spin_lock_init(&priv->irq_th_lock);
	spin_lock_init(&priv->rf_ps_lock);
	spin_lock_init(&priv->ps_lock);
	mutex_init(&priv->wx_mutex);
	mutex_init(&priv->rf_mutex);
	mutex_init(&priv->mutex);
}

static void _rtl92e_init_priv_task(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	INIT_WORK(&priv->reset_wq, (void *)_rtl92e_restart);
	INIT_WORK(&priv->rtllib->ips_leave_wq, (void *)rtl92e_ips_leave_wq);
	INIT_DELAYED_WORK(&priv->watch_dog_wq, (void *)_rtl92e_watchdog_wq_cb);
	INIT_DELAYED_WORK(&priv->txpower_tracking_wq, (void *)rtl92e_dm_txpower_tracking_wq);
	INIT_DELAYED_WORK(&priv->rfpath_check_wq, (void *)rtl92e_dm_rf_pathcheck_wq);
	INIT_DELAYED_WORK(&priv->update_beacon_wq, (void *)_rtl92e_update_beacon);
	INIT_WORK(&priv->qos_activate, (void *)_rtl92e_qos_activate);
	INIT_DELAYED_WORK(&priv->rtllib->hw_wakeup_wq, (void *)rtl92e_hw_wakeup_wq);
	INIT_DELAYED_WORK(&priv->rtllib->hw_sleep_wq, (void *)rtl92e_hw_sleep_wq);
	tasklet_setup(&priv->irq_rx_tasklet, _rtl92e_irq_rx_tasklet);
	tasklet_setup(&priv->irq_tx_tasklet, _rtl92e_irq_tx_tasklet);
}

static short _rtl92e_get_channel_map(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;

	struct r8192_priv *priv = rtllib_priv(dev);
<<<<<<< HEAD
	if ((priv->rf_chip != RF_8225) && (priv->rf_chip != RF_8256)
			&& (priv->rf_chip != RF_6052)) {
		RT_TRACE(COMP_ERR, "%s: unknown rf chip, can't set channel "
			 "map\n", __func__);
		return -1;
	}

	if (priv->ChannelPlan >= COUNTRY_CODE_MAX) {
		printk(KERN_INFO "rtl819x_init:Error channel plan! Set to "
		       "default.\n");
		priv->ChannelPlan = COUNTRY_CODE_FCC;
	}
	RT_TRACE(COMP_INIT, "Channel plan is %d\n", priv->ChannelPlan);
	dot11d_init(priv->rtllib);
	Dot11d_Channelmap(priv->ChannelPlan, priv->rtllib);
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 1; i <= 11; i++)
		(priv->rtllib->active_channel_map)[i] = 1;
	(priv->rtllib->active_channel_map)[12] = 2;
	(priv->rtllib->active_channel_map)[13] = 2;

	return 0;
}

<<<<<<< HEAD
static short rtl8192_init(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	memset(&(priv->stats), 0, sizeof(struct rt_stats));

	rtl8192_init_priv_handler(dev);
	rtl8192_init_priv_constant(dev);
	rtl8192_init_priv_variable(dev);
	rtl8192_init_priv_lock(priv);
	rtl8192_init_priv_task(dev);
	priv->ops->get_eeprom_size(dev);
	priv->ops->init_adapter_variable(dev);
	rtl8192_get_channel_map(dev);

	init_hal_dm(dev);

	init_timer(&priv->watch_dog_timer);
	setup_timer(&priv->watch_dog_timer,
		    watch_dog_timer_callback,
		    (unsigned long) dev);

	init_timer(&priv->gpio_polling_timer);
	setup_timer(&priv->gpio_polling_timer,
		    check_rfctrl_gpio_timer,
		    (unsigned long)dev);

	rtl8192_irq_disable(dev);
	if (request_irq(dev->irq, (void *)rtl8192_interrupt_rsl, IRQF_SHARED,
	    dev->name, dev)) {
		printk(KERN_ERR "Error allocating IRQ %d", dev->irq);
		return -1;
	} else {
		priv->irq = dev->irq;
		RT_TRACE(COMP_INIT, "IRQ %d\n", dev->irq);
	}

	if (rtl8192_pci_initdescring(dev) != 0) {
		printk(KERN_ERR "Endopoints initialization failed");
=======
static short _rtl92e_init(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	memset(&priv->stats, 0, sizeof(struct rt_stats));

	_rtl92e_init_priv_handler(dev);
	_rtl92e_init_priv_variable(dev);
	_rtl92e_init_priv_lock(priv);
	_rtl92e_init_priv_task(dev);
	rtl92e_get_eeprom_size(dev);
	rtl92e_init_variables(dev);
	_rtl92e_get_channel_map(dev);

	rtl92e_dm_init(dev);

	timer_setup(&priv->watch_dog_timer, _rtl92e_watchdog_timer_cb, 0);

	timer_setup(&priv->gpio_polling_timer, rtl92e_check_rfctrl_gpio_timer,
		    0);

	rtl92e_irq_disable(dev);
	if (request_irq(dev->irq, _rtl92e_irq, IRQF_SHARED, dev->name, dev)) {
		netdev_err(dev, "Error allocating IRQ %d", dev->irq);
		return -1;
	}

	priv->irq = dev->irq;

	if (_rtl92e_pci_initdescring(dev) != 0) {
		netdev_err(dev, "Endopoints initialization failed");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		free_irq(dev->irq, dev);
		return -1;
	}

	return 0;
}

/***************************************************************************
<<<<<<< HEAD
	-------------------------------WATCHDOG STUFF---------------------------
***************************************************************************/
short rtl8192_is_tx_queue_empty(struct net_device *dev)
{
	int i = 0;
	struct r8192_priv *priv = rtllib_priv(dev);
=======
 * -------------------------------WATCHDOG STUFF---------------------------
 **************************************************************************/
static short _rtl92e_is_tx_queue_empty(struct net_device *dev)
{
	int i = 0;
	struct r8192_priv *priv = rtllib_priv(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i <= MGNT_QUEUE; i++) {
		if ((i == TXCMD_QUEUE) || (i == HCCA_QUEUE))
			continue;
		if (skb_queue_len(&(&priv->tx_ring[i])->queue) > 0) {
<<<<<<< HEAD
			printk(KERN_INFO "===>tx queue is not empty:%d, %d\n",
=======
			netdev_info(dev, "===>tx queue is not empty:%d, %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       i, skb_queue_len(&(&priv->tx_ring[i])->queue));
			return 0;
		}
	}
	return 1;
}

<<<<<<< HEAD
static enum reset_type rtl819x_TxCheckStuck(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	u8	QueueID;
	u8	ResetThreshold = NIC_SEND_HANG_THRESHOLD_POWERSAVE;
=======
static enum reset_type _rtl92e_tx_check_stuck(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	u8	QueueID;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool	bCheckFwTxCnt = false;
	struct rtl8192_tx_ring  *ring = NULL;
	struct sk_buff *skb = NULL;
	struct cb_desc *tcb_desc = NULL;
	unsigned long flags = 0;

	switch (priv->rtllib->ps) {
	case RTLLIB_PS_DISABLED:
<<<<<<< HEAD
		ResetThreshold = NIC_SEND_HANG_THRESHOLD_NORMAL;
		break;
	case (RTLLIB_PS_MBCAST|RTLLIB_PS_UNICAST):
		ResetThreshold = NIC_SEND_HANG_THRESHOLD_POWERSAVE;
		break;
	default:
		ResetThreshold = NIC_SEND_HANG_THRESHOLD_POWERSAVE;
=======
		break;
	case (RTLLIB_PS_MBCAST | RTLLIB_PS_UNICAST):
		break;
	default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	spin_lock_irqsave(&priv->irq_th_lock, flags);
	for (QueueID = 0; QueueID < MAX_TX_QUEUE; QueueID++) {
		if (QueueID == TXCMD_QUEUE)
			continue;

		if (QueueID == BEACON_QUEUE)
			continue;

		ring = &priv->tx_ring[QueueID];

		if (skb_queue_len(&ring->queue) == 0) {
			continue;
		} else {
<<<<<<< HEAD
			skb = (&ring->queue)->next;
=======
			skb = __skb_peek(&ring->queue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			tcb_desc = (struct cb_desc *)(skb->cb +
				    MAX_DEV_ADDR_SIZE);
			tcb_desc->nStuckCount++;
			bCheckFwTxCnt = true;
			if (tcb_desc->nStuckCount > 1)
<<<<<<< HEAD
				printk(KERN_INFO "%s: QueueID=%d tcb_desc->n"
				       "StuckCount=%d\n", __func__, QueueID,
				       tcb_desc->nStuckCount);
=======
				netdev_info(dev,
					    "%s: QueueID=%d tcb_desc->nStuckCount=%d\n",
					    __func__, QueueID,
					    tcb_desc->nStuckCount);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	spin_unlock_irqrestore(&priv->irq_th_lock, flags);

	if (bCheckFwTxCnt) {
<<<<<<< HEAD
		if (priv->ops->TxCheckStuckHandler(dev)) {
			RT_TRACE(COMP_RESET, "TxCheckStuck(): Fw indicates no"
				 " Tx condition!\n");
			return RESET_TYPE_SILENT;
		}
=======
		if (rtl92e_is_tx_stuck(dev))
			return RESET_TYPE_SILENT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return RESET_TYPE_NORESET;
}

<<<<<<< HEAD
static enum reset_type rtl819x_RxCheckStuck(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	if (priv->ops->RxCheckStuckHandler(dev)) {
		RT_TRACE(COMP_RESET, "RxStuck Condition\n");
		return RESET_TYPE_SILENT;
	}
=======
static enum reset_type _rtl92e_rx_check_stuck(struct net_device *dev)
{
	if (rtl92e_is_rx_stuck(dev))
		return RESET_TYPE_SILENT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return RESET_TYPE_NORESET;
}

<<<<<<< HEAD
static enum reset_type rtl819x_ifcheck_resetornot(struct net_device *dev)
=======
static void _rtl92e_if_check_reset(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	enum reset_type TxResetType = RESET_TYPE_NORESET;
	enum reset_type RxResetType = RESET_TYPE_NORESET;
	enum rt_rf_power_state rfState;

<<<<<<< HEAD
	rfState = priv->rtllib->eRFPowerState;

	if (rfState == eRfOn)
		TxResetType = rtl819x_TxCheckStuck(dev);

	if (rfState == eRfOn &&
	    (priv->rtllib->iw_mode == IW_MODE_INFRA) &&
	    (priv->rtllib->state == RTLLIB_LINKED))
		RxResetType = rtl819x_RxCheckStuck(dev);

	if (TxResetType == RESET_TYPE_NORMAL ||
	    RxResetType == RESET_TYPE_NORMAL) {
		printk(KERN_INFO "%s(): TxResetType is %d, RxResetType is %d\n",
		       __func__, TxResetType, RxResetType);
		return RESET_TYPE_NORMAL;
	} else if (TxResetType == RESET_TYPE_SILENT ||
		   RxResetType == RESET_TYPE_SILENT) {
		printk(KERN_INFO "%s(): TxResetType is %d, RxResetType is %d\n",
		       __func__, TxResetType, RxResetType);
		return RESET_TYPE_SILENT;
	} else {
		return RESET_TYPE_NORESET;
	}

}

static void rtl819x_silentreset_mesh_bk(struct net_device *dev, u8 IsPortal)
{
}

static void rtl819x_ifsilentreset(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	u8	reset_times = 0;
	int reset_status = 0;
	struct rtllib_device *ieee = priv->rtllib;
	unsigned long flag;

	u8 IsPortal = 0;


	if (priv->ResetProgress == RESET_TYPE_NORESET) {

		RT_TRACE(COMP_RESET, "=========>Reset progress!!\n");

		priv->ResetProgress = RESET_TYPE_SILENT;

		spin_lock_irqsave(&priv->rf_ps_lock, flag);
		if (priv->RFChangeInProgress) {
			spin_unlock_irqrestore(&priv->rf_ps_lock, flag);
			goto END;
		}
		priv->RFChangeInProgress = true;
		priv->bResetInProgress = true;
		spin_unlock_irqrestore(&priv->rf_ps_lock, flag);

RESET_START:

		down(&priv->wx_sem);

		if (priv->rtllib->state == RTLLIB_LINKED)
			LeisurePSLeave(dev);

		if (IS_NIC_DOWN(priv)) {
			RT_TRACE(COMP_ERR, "%s():the driver is not up! "
				 "return\n", __func__);
			up(&priv->wx_sem);
			return ;
		}
		priv->up = 0;

		RT_TRACE(COMP_RESET, "%s():======>start to down the driver\n",
			  __func__);
		mdelay(1000);
		RT_TRACE(COMP_RESET, "%s():111111111111111111111111======>start"
			 " to down the driver\n", __func__);

		if (!netif_queue_stopped(dev))
			netif_stop_queue(dev);

		rtl8192_irq_disable(dev);
		del_timer_sync(&priv->watch_dog_timer);
		rtl8192_cancel_deferred_work(priv);
		deinit_hal_dm(dev);
		rtllib_stop_scan_syncro(ieee);

		if (ieee->state == RTLLIB_LINKED) {
			SEM_DOWN_IEEE_WX(&ieee->wx_sem);
			printk(KERN_INFO "ieee->state is RTLLIB_LINKED\n");
			rtllib_stop_send_beacons(priv->rtllib);
			del_timer_sync(&ieee->associate_timer);
			cancel_delayed_work(&ieee->associate_retry_wq);
			rtllib_stop_scan(ieee);
			netif_carrier_off(dev);
			SEM_UP_IEEE_WX(&ieee->wx_sem);
		} else {
			printk(KERN_INFO "ieee->state is NOT LINKED\n");
			rtllib_softmac_stop_protocol(priv->rtllib, 0 , true);
		}

		dm_backup_dynamic_mechanism_state(dev);

		up(&priv->wx_sem);
		RT_TRACE(COMP_RESET, "%s():<==========down process is "
			 "finished\n", __func__);

		RT_TRACE(COMP_RESET, "%s():<===========up process start\n",
			 __func__);
		reset_status = _rtl8192_up(dev, true);

		RT_TRACE(COMP_RESET, "%s():<===========up process is "
			 "finished\n", __func__);
		if (reset_status == -1) {
			if (reset_times < 3) {
				reset_times++;
				goto RESET_START;
			} else {
				RT_TRACE(COMP_ERR, " ERR!!! %s():  Reset "
					 "Failed!!\n", __func__);
			}
		}

		ieee->is_silent_reset = 1;

		spin_lock_irqsave(&priv->rf_ps_lock, flag);
		priv->RFChangeInProgress = false;
		spin_unlock_irqrestore(&priv->rf_ps_lock, flag);

		EnableHWSecurityConfig8192(dev);

		if (ieee->state == RTLLIB_LINKED && ieee->iw_mode ==
		    IW_MODE_INFRA) {
			ieee->set_chan(ieee->dev,
				       ieee->current_network.channel);

			queue_work_rsl(ieee->wq, &ieee->associate_complete_wq);

		} else if (ieee->state == RTLLIB_LINKED && ieee->iw_mode ==
			   IW_MODE_ADHOC) {
			ieee->set_chan(ieee->dev,
				       ieee->current_network.channel);
			ieee->link_change(ieee->dev);

			notify_wx_assoc_event(ieee);

			rtllib_start_send_beacons(ieee);

			if (ieee->data_hard_resume)
				ieee->data_hard_resume(ieee->dev);
			netif_carrier_on(ieee->dev);
		} else if (ieee->iw_mode == IW_MODE_MESH) {
			rtl819x_silentreset_mesh_bk(dev, IsPortal);
		}

		CamRestoreAllEntry(dev);
		dm_restore_dynamic_mechanism_state(dev);
END:
		priv->ResetProgress = RESET_TYPE_NORESET;
		priv->reset_count++;

		priv->bForcedSilentReset = false;
		priv->bResetInProgress = false;

		write_nic_byte(dev, UFWP, 1);
		RT_TRACE(COMP_RESET, "Reset finished!! ====>[%d]\n",
			 priv->reset_count);
	}
}

static void rtl819x_update_rxcounts(struct r8192_priv *priv, u32 *TotalRxBcnNum,
				    u32 *TotalRxDataNum)
{
	u16	SlotIndex;
=======
	rfState = priv->rtllib->rf_power_state;

	if (rfState == rf_on)
		TxResetType = _rtl92e_tx_check_stuck(dev);

	if (rfState == rf_on &&
	    (priv->rtllib->iw_mode == IW_MODE_INFRA) &&
	    (priv->rtllib->link_state == MAC80211_LINKED))
		RxResetType = _rtl92e_rx_check_stuck(dev);

	if (TxResetType == RESET_TYPE_SILENT ||
		   RxResetType == RESET_TYPE_SILENT) {
		netdev_info(dev, "%s(): TxResetType is %d, RxResetType is %d\n",
			    __func__, TxResetType, RxResetType);
	}
}

static void _rtl92e_update_rxcounts(struct r8192_priv *priv, u32 *TotalRxBcnNum,
				    u32 *TotalRxDataNum)
{
	u16	slot_index;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8	i;

	*TotalRxBcnNum = 0;
	*TotalRxDataNum = 0;

<<<<<<< HEAD
	SlotIndex = (priv->rtllib->LinkDetectInfo.SlotIndex++) %
			(priv->rtllib->LinkDetectInfo.SlotNum);
	priv->rtllib->LinkDetectInfo.RxBcnNum[SlotIndex] =
			priv->rtllib->LinkDetectInfo.NumRecvBcnInPeriod;
	priv->rtllib->LinkDetectInfo.RxDataNum[SlotIndex] =
			priv->rtllib->LinkDetectInfo.NumRecvDataInPeriod;
	for (i = 0; i < priv->rtllib->LinkDetectInfo.SlotNum; i++) {
		*TotalRxBcnNum += priv->rtllib->LinkDetectInfo.RxBcnNum[i];
		*TotalRxDataNum += priv->rtllib->LinkDetectInfo.RxDataNum[i];
	}
}


void	rtl819x_watchdog_wqcallback(void *data)
=======
	slot_index = (priv->rtllib->link_detect_info.slot_index++) %
			(priv->rtllib->link_detect_info.slot_num);
	priv->rtllib->link_detect_info.RxBcnNum[slot_index] =
			priv->rtllib->link_detect_info.num_recv_bcn_in_period;
	priv->rtllib->link_detect_info.RxDataNum[slot_index] =
			priv->rtllib->link_detect_info.num_recv_data_in_period;
	for (i = 0; i < priv->rtllib->link_detect_info.slot_num; i++) {
		*TotalRxBcnNum += priv->rtllib->link_detect_info.RxBcnNum[i];
		*TotalRxDataNum += priv->rtllib->link_detect_info.RxDataNum[i];
	}
}

static void _rtl92e_watchdog_wq_cb(void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = container_of_dwork_rsl(data,
				  struct r8192_priv, watch_dog_wq);
	struct net_device *dev = priv->rtllib->dev;
	struct rtllib_device *ieee = priv->rtllib;
<<<<<<< HEAD
	enum reset_type ResetType = RESET_TYPE_NORESET;
	static u8 check_reset_cnt;
	unsigned long flags;
	struct rt_pwr_save_ctrl *pPSC = (struct rt_pwr_save_ctrl *)
					(&(priv->rtllib->PowerSaveControl));
	bool bBusyTraffic = false;
=======
	static u8 check_reset_cnt;
	unsigned long flags;
	struct rt_pwr_save_ctrl *psc = (struct rt_pwr_save_ctrl *)
					(&priv->rtllib->pwr_save_ctrl);
	bool busy_traffic = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool	bHigherBusyTraffic = false;
	bool	bHigherBusyRxTraffic = false;
	bool bEnterPS = false;

<<<<<<< HEAD
	if (IS_NIC_DOWN(priv) || (priv->bHwRadioOff == true))
		return;

	if (priv->rtllib->state >= RTLLIB_LINKED) {
=======
	if (!priv->up || priv->hw_radio_off)
		return;

	if (priv->rtllib->link_state >= MAC80211_LINKED) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (priv->rtllib->CntAfterLink < 2)
			priv->rtllib->CntAfterLink++;
	} else {
		priv->rtllib->CntAfterLink = 0;
	}

<<<<<<< HEAD
	hal_dm_watchdog(dev);

	if (rtllib_act_scanning(priv->rtllib, false) == false) {
		if ((ieee->iw_mode == IW_MODE_INFRA) && (ieee->state ==
		     RTLLIB_NOLINK) &&
		     (ieee->eRFPowerState == eRfOn) && !ieee->is_set_key &&
		     (!ieee->proto_stoppping) && !ieee->wx_set_enc) {
			if ((ieee->PowerSaveControl.ReturnPoint ==
			     IPS_CALLBACK_NONE) &&
			     (!ieee->bNetPromiscuousMode)) {
				RT_TRACE(COMP_PS, "====================>haha: "
					 "IPSEnter()\n");
				IPSEnter(dev);
			}
		}
	}
	if ((ieee->state == RTLLIB_LINKED) && (ieee->iw_mode ==
	     IW_MODE_INFRA) && (!ieee->bNetPromiscuousMode)) {
		if (ieee->LinkDetectInfo.NumRxOkInPeriod > 100 ||
		ieee->LinkDetectInfo.NumTxOkInPeriod > 100)
			bBusyTraffic = true;


		if (ieee->LinkDetectInfo.NumRxOkInPeriod > 4000 ||
		    ieee->LinkDetectInfo.NumTxOkInPeriod > 4000) {
			bHigherBusyTraffic = true;
			if (ieee->LinkDetectInfo.NumRxOkInPeriod > 5000)
=======
	rtl92e_dm_watchdog(dev);

	if (!rtllib_act_scanning(priv->rtllib, false)) {
		if ((ieee->iw_mode == IW_MODE_INFRA) && (ieee->link_state ==
		     MAC80211_NOLINK) &&
		     (ieee->rf_power_state == rf_on) && !ieee->is_set_key &&
		     (!ieee->proto_stoppping) && !ieee->wx_set_enc) {
			if (ieee->pwr_save_ctrl.ReturnPoint == IPS_CALLBACK_NONE)
				rtl92e_ips_enter(dev);
		}
	}
	if ((ieee->link_state == MAC80211_LINKED) && (ieee->iw_mode == IW_MODE_INFRA)) {
		if (ieee->link_detect_info.num_rx_ok_in_period > 100 ||
		ieee->link_detect_info.num_tx_ok_in_period > 100)
			busy_traffic = true;

		if (ieee->link_detect_info.num_rx_ok_in_period > 4000 ||
		    ieee->link_detect_info.num_tx_ok_in_period > 4000) {
			bHigherBusyTraffic = true;
			if (ieee->link_detect_info.num_rx_ok_in_period > 5000)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				bHigherBusyRxTraffic = true;
			else
				bHigherBusyRxTraffic = false;
		}

<<<<<<< HEAD
		if (((ieee->LinkDetectInfo.NumRxUnicastOkInPeriod +
		    ieee->LinkDetectInfo.NumTxOkInPeriod) > 8) ||
		    (ieee->LinkDetectInfo.NumRxUnicastOkInPeriod > 2))
=======
		if (((ieee->link_detect_info.num_rx_unicast_ok_in_period +
		    ieee->link_detect_info.num_tx_ok_in_period) > 8) ||
		    (ieee->link_detect_info.num_rx_unicast_ok_in_period > 2))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			bEnterPS = false;
		else
			bEnterPS = true;

		if (ieee->current_network.beacon_interval < 95)
			bEnterPS = false;

		if (bEnterPS)
<<<<<<< HEAD
			LeisurePSEnter(dev);
		else
			LeisurePSLeave(dev);

	} else {
		RT_TRACE(COMP_LPS, "====>no link LPS leave\n");
		LeisurePSLeave(dev);
	}

	ieee->LinkDetectInfo.NumRxOkInPeriod = 0;
	ieee->LinkDetectInfo.NumTxOkInPeriod = 0;
	ieee->LinkDetectInfo.NumRxUnicastOkInPeriod = 0;
	ieee->LinkDetectInfo.bBusyTraffic = bBusyTraffic;

	ieee->LinkDetectInfo.bHigherBusyTraffic = bHigherBusyTraffic;
	ieee->LinkDetectInfo.bHigherBusyRxTraffic = bHigherBusyRxTraffic;

	if (ieee->state == RTLLIB_LINKED && ieee->iw_mode == IW_MODE_INFRA) {
		u32	TotalRxBcnNum = 0;
		u32	TotalRxDataNum = 0;

		rtl819x_update_rxcounts(priv, &TotalRxBcnNum, &TotalRxDataNum);

		if ((TotalRxBcnNum+TotalRxDataNum) == 0)
=======
			rtl92e_leisure_ps_enter(dev);
		else
			rtl92e_leisure_ps_leave(dev);

	} else {
		rtl92e_leisure_ps_leave(dev);
	}

	ieee->link_detect_info.num_rx_ok_in_period = 0;
	ieee->link_detect_info.num_tx_ok_in_period = 0;
	ieee->link_detect_info.num_rx_unicast_ok_in_period = 0;
	ieee->link_detect_info.busy_traffic = busy_traffic;

	ieee->link_detect_info.bHigherBusyTraffic = bHigherBusyTraffic;
	ieee->link_detect_info.bHigherBusyRxTraffic = bHigherBusyRxTraffic;

	if (ieee->link_state == MAC80211_LINKED && ieee->iw_mode == IW_MODE_INFRA) {
		u32	TotalRxBcnNum = 0;
		u32	TotalRxDataNum = 0;

		_rtl92e_update_rxcounts(priv, &TotalRxBcnNum, &TotalRxDataNum);

		if ((TotalRxBcnNum + TotalRxDataNum) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			priv->check_roaming_cnt++;
		else
			priv->check_roaming_cnt = 0;

<<<<<<< HEAD

		if (priv->check_roaming_cnt > 0) {
			if (ieee->eRFPowerState == eRfOff)
				RT_TRACE(COMP_ERR, "========>%s()\n", __func__);

			printk(KERN_INFO "===>%s(): AP is power off, chan:%d,"
			       " connect another one\n", __func__, priv->chan);

			ieee->state = RTLLIB_ASSOCIATING;

			RemovePeerTS(priv->rtllib,
=======
		if (priv->check_roaming_cnt > 0) {
			if (ieee->rf_power_state == rf_off)
				netdev_info(dev, "%s(): RF is off\n", __func__);

			netdev_info(dev,
				    "===>%s(): AP is power off, chan:%d, connect another one\n",
				    __func__, priv->chan);

			ieee->link_state = RTLLIB_ASSOCIATING;

			remove_peer_ts(priv->rtllib,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				     priv->rtllib->current_network.bssid);
			ieee->is_roaming = true;
			ieee->is_set_key = false;
			ieee->link_change(dev);
<<<<<<< HEAD
			if (ieee->LedControlHandler)
				ieee->LedControlHandler(ieee->dev,
							LED_CTL_START_TO_LINK);

			notify_wx_assoc_event(ieee);

			if (!(ieee->rtllib_ap_sec_type(ieee) &
			     (SEC_ALG_CCMP|SEC_ALG_TKIP)))
				queue_delayed_work_rsl(ieee->wq,
=======
			notify_wx_assoc_event(ieee);

			if (!(ieee->rtllib_ap_sec_type(ieee) &
			     (SEC_ALG_CCMP | SEC_ALG_TKIP)))
				schedule_delayed_work(
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					&ieee->associate_procedure_wq, 0);

			priv->check_roaming_cnt = 0;
		}
<<<<<<< HEAD
		ieee->LinkDetectInfo.NumRecvBcnInPeriod = 0;
		ieee->LinkDetectInfo.NumRecvDataInPeriod = 0;

=======
		ieee->link_detect_info.num_recv_bcn_in_period = 0;
		ieee->link_detect_info.num_recv_data_in_period = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	spin_lock_irqsave(&priv->tx_lock, flags);
	if ((check_reset_cnt++ >= 3) && (!ieee->is_roaming) &&
<<<<<<< HEAD
	    (!priv->RFChangeInProgress) && (!pPSC->bSwRfProcessing)) {
		ResetType = rtl819x_ifcheck_resetornot(dev);
		check_reset_cnt = 3;
	}
	spin_unlock_irqrestore(&priv->tx_lock, flags);

	if (!priv->bDisableNormalResetCheck && ResetType == RESET_TYPE_NORMAL) {
		priv->ResetProgress = RESET_TYPE_NORMAL;
		RT_TRACE(COMP_RESET, "%s(): NOMAL RESET\n", __func__);
		return;
	}

	if (((priv->force_reset) || (!priv->bDisableNormalResetCheck &&
	      ResetType == RESET_TYPE_SILENT)))
		rtl819x_ifsilentreset(dev);
	priv->force_reset = false;
	priv->bForcedSilentReset = false;
	priv->bResetInProgress = false;
	RT_TRACE(COMP_TRACE, " <==RtUsbCheckForHangWorkItemCallback()\n");
}

void watch_dog_timer_callback(unsigned long data)
{
	struct r8192_priv *priv = rtllib_priv((struct net_device *)data);
	queue_delayed_work_rsl(priv->priv_wq, &priv->watch_dog_wq, 0);
	mod_timer(&priv->watch_dog_timer, jiffies +
		  MSECS(RTLLIB_WATCH_DOG_TIME));
}

/****************************************************************************
 ---------------------------- NIC TX/RX STUFF---------------------------
*****************************************************************************/
void rtl8192_rx_enable(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	priv->ops->rx_enable(dev);
}

void rtl8192_tx_enable(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);

	priv->ops->tx_enable(dev);
=======
	    (!priv->rf_change_in_progress) && (!psc->bSwRfProcessing)) {
		_rtl92e_if_check_reset(dev);
		check_reset_cnt = 3;
	}
	spin_unlock_irqrestore(&priv->tx_lock, flags);
}

static void _rtl92e_watchdog_timer_cb(struct timer_list *t)
{
	struct r8192_priv *priv = from_timer(priv, t, watch_dog_timer);

	schedule_delayed_work(&priv->watch_dog_wq, 0);
	mod_timer(&priv->watch_dog_timer, jiffies +
		  msecs_to_jiffies(RTLLIB_WATCH_DOG_TIME));
}

/****************************************************************************
 * ---------------------------- NIC TX/RX STUFF---------------------------
 ****************************************************************************/
void rtl92e_rx_enable(struct net_device *dev)
{
	rtl92e_enable_rx(dev);
}

void rtl92e_tx_enable(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	rtl92e_enable_tx(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rtllib_reset_queue(priv->rtllib);
}

<<<<<<< HEAD

static void rtl8192_free_rx_ring(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	int i, rx_queue_idx;

	for (rx_queue_idx = 0; rx_queue_idx < MAX_RX_QUEUE;
	     rx_queue_idx++) {
		for (i = 0; i < priv->rxringcount; i++) {
			struct sk_buff *skb = priv->rx_buf[rx_queue_idx][i];
			if (!skb)
				continue;

			pci_unmap_single(priv->pdev,
				*((dma_addr_t *)skb->cb),
				priv->rxbuffersize, PCI_DMA_FROMDEVICE);
				kfree_skb(skb);
		}

		pci_free_consistent(priv->pdev,
			sizeof(*priv->rx_ring[rx_queue_idx]) *
			priv->rxringcount,
			priv->rx_ring[rx_queue_idx],
			priv->rx_ring_dma[rx_queue_idx]);
		priv->rx_ring[rx_queue_idx] = NULL;
	}
}

static void rtl8192_free_tx_ring(struct net_device *dev, unsigned int prio)
=======
static void _rtl92e_free_rx_ring(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	int i;

	for (i = 0; i < priv->rxringcount; i++) {
		struct sk_buff *skb = priv->rx_buf[i];

		if (!skb)
			continue;

		dma_unmap_single(&priv->pdev->dev,
				 *((dma_addr_t *)skb->cb),
				 priv->rxbuffersize, DMA_FROM_DEVICE);
		kfree_skb(skb);
	}

	dma_free_coherent(&priv->pdev->dev,
			  sizeof(*priv->rx_ring) * priv->rxringcount,
			  priv->rx_ring,
			  priv->rx_ring_dma);
	priv->rx_ring = NULL;
}

static void _rtl92e_free_tx_ring(struct net_device *dev, unsigned int prio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rtl8192_tx_ring *ring = &priv->tx_ring[prio];

	while (skb_queue_len(&ring->queue)) {
		struct tx_desc *entry = &ring->desc[ring->idx];
		struct sk_buff *skb = __skb_dequeue(&ring->queue);

<<<<<<< HEAD
		pci_unmap_single(priv->pdev, le32_to_cpu(entry->TxBuffAddr),
			skb->len, PCI_DMA_TODEVICE);
=======
		dma_unmap_single(&priv->pdev->dev, entry->TxBuffAddr,
				 skb->len, DMA_TO_DEVICE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree_skb(skb);
		ring->idx = (ring->idx + 1) % ring->entries;
	}

<<<<<<< HEAD
	pci_free_consistent(priv->pdev, sizeof(*ring->desc)*ring->entries,
	ring->desc, ring->dma);
	ring->desc = NULL;
}

void rtl8192_data_hard_stop(struct net_device *dev)
{
}


void rtl8192_data_hard_resume(struct net_device *dev)
{
}

void rtl8192_hard_data_xmit(struct sk_buff *skb, struct net_device *dev,
			    int rate)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
=======
	dma_free_coherent(&priv->pdev->dev,
			  sizeof(*ring->desc) * ring->entries, ring->desc,
			  ring->dma);
	ring->desc = NULL;
}

static void _rtl92e_hard_data_xmit(struct sk_buff *skb, struct net_device *dev,
				   int rate)
{
	struct r8192_priv *priv = rtllib_priv(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;
	struct cb_desc *tcb_desc = (struct cb_desc *)(skb->cb +
				    MAX_DEV_ADDR_SIZE);
	u8 queue_index = tcb_desc->queue_index;

<<<<<<< HEAD
	if ((priv->rtllib->eRFPowerState == eRfOff) || IS_NIC_DOWN(priv) ||
	     priv->bResetInProgress) {
=======
	if ((priv->rtllib->rf_power_state == rf_off) || !priv->up) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree_skb(skb);
		return;
	}

<<<<<<< HEAD
	assert(queue_index != TXCMD_QUEUE);


	memcpy((unsigned char *)(skb->cb), &dev, sizeof(dev));
	skb_push(skb, priv->rtllib->tx_headroom);
	ret = rtl8192_tx(dev, skb);
	if (ret != 0) {
		kfree_skb(skb);
	};
=======
	if (queue_index == TXCMD_QUEUE)
		netdev_warn(dev, "%s(): queue index == TXCMD_QUEUE\n",
			    __func__);

	memcpy((unsigned char *)(skb->cb), &dev, sizeof(dev));
	skb_push(skb, priv->rtllib->tx_headroom);
	ret = _rtl92e_tx(dev, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (queue_index != MGNT_QUEUE) {
		priv->rtllib->stats.tx_bytes += (skb->len -
						 priv->rtllib->tx_headroom);
		priv->rtllib->stats.tx_packets++;
	}

<<<<<<< HEAD

	return;
}

int rtl8192_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
=======
	if (ret != 0)
		kfree_skb(skb);
}

static int _rtl92e_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;
	struct cb_desc *tcb_desc = (struct cb_desc *)(skb->cb +
				    MAX_DEV_ADDR_SIZE);
	u8 queue_index = tcb_desc->queue_index;

	if (queue_index != TXCMD_QUEUE) {
<<<<<<< HEAD
		if ((priv->rtllib->eRFPowerState == eRfOff) ||
		     IS_NIC_DOWN(priv) || priv->bResetInProgress) {
=======
		if ((priv->rtllib->rf_power_state == rf_off) ||
		     !priv->up) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			kfree_skb(skb);
			return 0;
		}
	}

	memcpy((unsigned char *)(skb->cb), &dev, sizeof(dev));
	if (queue_index == TXCMD_QUEUE) {
<<<<<<< HEAD
		rtl8192_tx_cmd(dev, skb);
		ret = 0;
		return ret;
	} else {
		tcb_desc->RATRIndex = 7;
		tcb_desc->bTxDisableRateFallBack = 1;
		tcb_desc->bTxUseDriverAssingedRate = 1;
		tcb_desc->bTxEnableFwCalcDur = 1;
		skb_push(skb, priv->rtllib->tx_headroom);
		ret = rtl8192_tx(dev, skb);
		if (ret != 0) {
			kfree_skb(skb);
		};
	}



	return ret;

}

static void rtl8192_tx_isr(struct net_device *dev, int prio)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
=======
		_rtl92e_tx_cmd(dev, skb);
		return 0;
	}

	tcb_desc->ratr_index = 7;
	tcb_desc->tx_dis_rate_fallback = 1;
	tcb_desc->tx_use_drv_assinged_rate = 1;
	tcb_desc->tx_enable_fw_calc_dur = 1;
	skb_push(skb, priv->rtllib->tx_headroom);
	ret = _rtl92e_tx(dev, skb);
	if (ret != 0)
		kfree_skb(skb);
	return ret;
}

static void _rtl92e_tx_isr(struct net_device *dev, int prio)
{
	struct r8192_priv *priv = rtllib_priv(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct rtl8192_tx_ring *ring = &priv->tx_ring[prio];

	while (skb_queue_len(&ring->queue)) {
		struct tx_desc *entry = &ring->desc[ring->idx];
		struct sk_buff *skb;

		if (prio != BEACON_QUEUE) {
			if (entry->OWN)
				return;
			ring->idx = (ring->idx + 1) % ring->entries;
		}

		skb = __skb_dequeue(&ring->queue);
<<<<<<< HEAD
		pci_unmap_single(priv->pdev, le32_to_cpu(entry->TxBuffAddr),
		skb->len, PCI_DMA_TODEVICE);
=======
		dma_unmap_single(&priv->pdev->dev, entry->TxBuffAddr,
				 skb->len, DMA_TO_DEVICE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		kfree_skb(skb);
	}
	if (prio != BEACON_QUEUE)
		tasklet_schedule(&priv->irq_tx_tasklet);
}

<<<<<<< HEAD
void rtl8192_tx_cmd(struct net_device *dev, struct sk_buff *skb)
=======
static void _rtl92e_tx_cmd(struct net_device *dev, struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rtl8192_tx_ring *ring;
	struct tx_desc_cmd *entry;
	unsigned int idx;
	struct cb_desc *tcb_desc;
	unsigned long flags;

	spin_lock_irqsave(&priv->irq_th_lock, flags);
	ring = &priv->tx_ring[TXCMD_QUEUE];

	idx = (ring->idx + skb_queue_len(&ring->queue)) % ring->entries;
<<<<<<< HEAD
	entry = (struct tx_desc_cmd *) &ring->desc[idx];

	tcb_desc = (struct cb_desc *)(skb->cb + MAX_DEV_ADDR_SIZE);

	priv->ops->tx_fill_cmd_descriptor(dev, entry, tcb_desc, skb);

	__skb_queue_tail(&ring->queue, skb);
	spin_unlock_irqrestore(&priv->irq_th_lock, flags);

	return;
}

short rtl8192_tx(struct net_device *dev, struct sk_buff *skb)
=======
	entry = (struct tx_desc_cmd *)&ring->desc[idx];

	tcb_desc = (struct cb_desc *)(skb->cb + MAX_DEV_ADDR_SIZE);

	rtl92e_fill_tx_cmd_desc(dev, entry, tcb_desc, skb);

	__skb_queue_tail(&ring->queue, skb);
	spin_unlock_irqrestore(&priv->irq_th_lock, flags);
}

static short _rtl92e_tx(struct net_device *dev, struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rtl8192_tx_ring  *ring;
	unsigned long flags;
	struct cb_desc *tcb_desc = (struct cb_desc *)(skb->cb +
				    MAX_DEV_ADDR_SIZE);
	struct tx_desc *pdesc = NULL;
<<<<<<< HEAD
	struct rtllib_hdr_1addr *header = NULL;
	u16 fc = 0, type = 0, stype = 0;
	bool  multi_addr = false, broad_addr = false, uni_addr = false;
=======
	struct ieee80211_hdr *header = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *pda_addr = NULL;
	int   idx;
	u32 fwinfo_size = 0;

<<<<<<< HEAD
	if (priv->bdisable_nic) {
		RT_TRACE(COMP_ERR, "%s: ERR!! Nic is disabled! Can't tx packet"
			 " len=%d qidx=%d!!!\n", __func__, skb->len,
			 tcb_desc->queue_index);
		return skb->len;
	}

	priv->rtllib->bAwakePktSent = true;

	fwinfo_size = sizeof(struct tx_fwinfo_8190pci);

	header = (struct rtllib_hdr_1addr *)(((u8 *)skb->data) + fwinfo_size);
	fc = header->frame_ctl;
	type = WLAN_FC_GET_TYPE(fc);
	stype = WLAN_FC_GET_STYPE(fc);
	pda_addr = header->addr1;

	if (is_multicast_ether_addr(pda_addr))
		multi_addr = true;
	else if (is_broadcast_ether_addr(pda_addr))
		broad_addr = true;
	else
		uni_addr = true;

	if (uni_addr)
		priv->stats.txbytesunicast += skb->len - fwinfo_size;
	else if (multi_addr)
		priv->stats.txbytesmulticast += skb->len - fwinfo_size;
	else
		priv->stats.txbytesbroadcast += skb->len - fwinfo_size;
=======
	priv->rtllib->awake_pkt_sent = true;

	fwinfo_size = sizeof(struct tx_fwinfo_8190pci);

	header = (struct ieee80211_hdr *)(((u8 *)skb->data) + fwinfo_size);
	pda_addr = header->addr1;

	if (!is_broadcast_ether_addr(pda_addr) && !is_multicast_ether_addr(pda_addr))
		priv->stats.txbytesunicast += skb->len - fwinfo_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&priv->irq_th_lock, flags);
	ring = &priv->tx_ring[tcb_desc->queue_index];
	if (tcb_desc->queue_index != BEACON_QUEUE)
		idx = (ring->idx + skb_queue_len(&ring->queue)) % ring->entries;
	else
		idx = 0;

	pdesc = &ring->desc[idx];
	if ((pdesc->OWN == 1) && (tcb_desc->queue_index != BEACON_QUEUE)) {
<<<<<<< HEAD
		RT_TRACE(COMP_ERR, "No more TX desc@%d, ring->idx = %d, idx = "
			 "%d, skblen = 0x%x queuelen=%d",
			 tcb_desc->queue_index, ring->idx, idx, skb->len,
			 skb_queue_len(&ring->queue));
		spin_unlock_irqrestore(&priv->irq_th_lock, flags);
		return skb->len;
	}

	if (type == RTLLIB_FTYPE_DATA) {
		if (priv->rtllib->LedControlHandler)
			priv->rtllib->LedControlHandler(dev, LED_CTL_TX);
	}
	priv->ops->tx_fill_descriptor(dev, pdesc, tcb_desc, skb);
	__skb_queue_tail(&ring->queue, skb);
	pdesc->OWN = 1;
	spin_unlock_irqrestore(&priv->irq_th_lock, flags);
	dev->trans_start = jiffies;

	write_nic_word(dev, TPPoll, 0x01 << tcb_desc->queue_index);
	return 0;
}

static short rtl8192_alloc_rx_desc_ring(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rx_desc *entry = NULL;
	int i, rx_queue_idx;

	for (rx_queue_idx = 0; rx_queue_idx < MAX_RX_QUEUE; rx_queue_idx++) {
		priv->rx_ring[rx_queue_idx] = pci_alloc_consistent(priv->pdev,
					sizeof(*priv->rx_ring[rx_queue_idx]) *
					priv->rxringcount,
					&priv->rx_ring_dma[rx_queue_idx]);

		if (!priv->rx_ring[rx_queue_idx] ||
		    (unsigned long)priv->rx_ring[rx_queue_idx] & 0xFF) {
			RT_TRACE(COMP_ERR, "Cannot allocate RX ring\n");
			return -ENOMEM;
		}

		memset(priv->rx_ring[rx_queue_idx], 0,
		       sizeof(*priv->rx_ring[rx_queue_idx]) *
		       priv->rxringcount);
		priv->rx_idx[rx_queue_idx] = 0;

		for (i = 0; i < priv->rxringcount; i++) {
			struct sk_buff *skb = dev_alloc_skb(priv->rxbuffersize);
			dma_addr_t *mapping;
			entry = &priv->rx_ring[rx_queue_idx][i];
			if (!skb)
				return 0;
			skb->dev = dev;
			priv->rx_buf[rx_queue_idx][i] = skb;
			mapping = (dma_addr_t *)skb->cb;
			*mapping = pci_map_single(priv->pdev,
						  skb_tail_pointer_rsl(skb),
						  priv->rxbuffersize,
						  PCI_DMA_FROMDEVICE);

			entry->BufferAddress = cpu_to_le32(*mapping);

			entry->Length = priv->rxbuffersize;
			entry->OWN = 1;
		}

		if(entry)
			entry->EOR = 1;
	}
	return 0;
}

static int rtl8192_alloc_tx_desc_ring(struct net_device *dev,
	unsigned int prio, unsigned int entries)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
=======
		netdev_warn(dev,
			    "No more TX desc@%d, ring->idx = %d, idx = %d, skblen = 0x%x queuelen=%d",
			    tcb_desc->queue_index, ring->idx, idx, skb->len,
			    skb_queue_len(&ring->queue));
		spin_unlock_irqrestore(&priv->irq_th_lock, flags);
		return skb->len;
	}
	rtl92e_fill_tx_desc(dev, pdesc, tcb_desc, skb);
	__skb_queue_tail(&ring->queue, skb);
	pdesc->OWN = 1;
	spin_unlock_irqrestore(&priv->irq_th_lock, flags);
	netif_trans_update(dev);

	rtl92e_writew(dev, TP_POLL, 0x01 << tcb_desc->queue_index);
	return 0;
}

static short _rtl92e_alloc_rx_ring(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rx_desc *entry = NULL;
	int i;

	priv->rx_ring = dma_alloc_coherent(&priv->pdev->dev,
					   sizeof(*priv->rx_ring) * priv->rxringcount,
					   &priv->rx_ring_dma,
					   GFP_ATOMIC);
	if (!priv->rx_ring || (unsigned long)priv->rx_ring & 0xFF) {
		netdev_warn(dev, "Cannot allocate RX ring\n");
		return -ENOMEM;
	}

	priv->rx_idx = 0;

	for (i = 0; i < priv->rxringcount; i++) {
		struct sk_buff *skb = dev_alloc_skb(priv->rxbuffersize);
		dma_addr_t *mapping;

		entry = &priv->rx_ring[i];
		if (!skb)
			return 0;
		skb->dev = dev;
		priv->rx_buf[i] = skb;
		mapping = (dma_addr_t *)skb->cb;
		*mapping = dma_map_single(&priv->pdev->dev,
					  skb_tail_pointer(skb),
					  priv->rxbuffersize, DMA_FROM_DEVICE);
		if (dma_mapping_error(&priv->pdev->dev, *mapping)) {
			dev_kfree_skb_any(skb);
			return -1;
		}
		entry->BufferAddress = *mapping;

		entry->Length = priv->rxbuffersize;
		entry->OWN = 1;
	}

	if (entry)
		entry->EOR = 1;
	return 0;
}

static int _rtl92e_alloc_tx_ring(struct net_device *dev, unsigned int prio,
				 unsigned int entries)
{
	struct r8192_priv *priv = rtllib_priv(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tx_desc *ring;
	dma_addr_t dma;
	int i;

<<<<<<< HEAD
	ring = pci_alloc_consistent(priv->pdev, sizeof(*ring) * entries, &dma);
	if (!ring || (unsigned long)ring & 0xFF) {
		RT_TRACE(COMP_ERR, "Cannot allocate TX ring (prio = %d)\n",
			 prio);
		return -ENOMEM;
	}

	memset(ring, 0, sizeof(*ring)*entries);
=======
	ring = dma_alloc_coherent(&priv->pdev->dev, sizeof(*ring) * entries,
				  &dma, GFP_ATOMIC);
	if (!ring || (unsigned long)ring & 0xFF) {
		netdev_warn(dev, "Cannot allocate TX ring (prio = %d)\n", prio);
		return -ENOMEM;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	priv->tx_ring[prio].desc = ring;
	priv->tx_ring[prio].dma = dma;
	priv->tx_ring[prio].idx = 0;
	priv->tx_ring[prio].entries = entries;
	skb_queue_head_init(&priv->tx_ring[prio].queue);

	for (i = 0; i < entries; i++)
		ring[i].NextDescAddress =
<<<<<<< HEAD
			cpu_to_le32((u32)dma + ((i + 1) % entries) *
			sizeof(*ring));
=======
			(u32)dma + ((i + 1) % entries) *
			sizeof(*ring);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD

short rtl8192_pci_initdescring(struct net_device *dev)
=======
static short _rtl92e_pci_initdescring(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 ret;
	int i;
	struct r8192_priv *priv = rtllib_priv(dev);

<<<<<<< HEAD
	ret = rtl8192_alloc_rx_desc_ring(dev);
=======
	ret = _rtl92e_alloc_rx_ring(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		return ret;

	for (i = 0; i < MAX_TX_QUEUE_COUNT; i++) {
<<<<<<< HEAD
		ret = rtl8192_alloc_tx_desc_ring(dev, i, priv->txringcount);
=======
		ret = _rtl92e_alloc_tx_ring(dev, i, priv->txringcount);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			goto err_free_rings;
	}

	return 0;

err_free_rings:
<<<<<<< HEAD
	rtl8192_free_rx_ring(dev);
	for (i = 0; i < MAX_TX_QUEUE_COUNT; i++)
		if (priv->tx_ring[i].desc)
			rtl8192_free_tx_ring(dev, i);
	return 1;
}

void rtl8192_pci_resetdescring(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	int i, rx_queue_idx;
	unsigned long flags = 0;

	for (rx_queue_idx = 0; rx_queue_idx < MAX_RX_QUEUE; rx_queue_idx++) {
		if (priv->rx_ring[rx_queue_idx]) {
			struct rx_desc *entry = NULL;
			for (i = 0; i < priv->rxringcount; i++) {
				entry = &priv->rx_ring[rx_queue_idx][i];
				entry->OWN = 1;
			}
			priv->rx_idx[rx_queue_idx] = 0;
		}
=======
	_rtl92e_free_rx_ring(dev);
	for (i = 0; i < MAX_TX_QUEUE_COUNT; i++)
		if (priv->tx_ring[i].desc)
			_rtl92e_free_tx_ring(dev, i);
	return 1;
}

void rtl92e_reset_desc_ring(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	int i;
	unsigned long flags = 0;

	if (priv->rx_ring) {
		struct rx_desc *entry = NULL;

		for (i = 0; i < priv->rxringcount; i++) {
			entry = &priv->rx_ring[i];
			entry->OWN = 1;
		}
		priv->rx_idx = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	spin_lock_irqsave(&priv->irq_th_lock, flags);
	for (i = 0; i < MAX_TX_QUEUE_COUNT; i++) {
		if (priv->tx_ring[i].desc) {
			struct rtl8192_tx_ring *ring = &priv->tx_ring[i];

			while (skb_queue_len(&ring->queue)) {
				struct tx_desc *entry = &ring->desc[ring->idx];
				struct sk_buff *skb =
						 __skb_dequeue(&ring->queue);

<<<<<<< HEAD
				pci_unmap_single(priv->pdev,
						 le32_to_cpu(entry->TxBuffAddr),
						 skb->len, PCI_DMA_TODEVICE);
=======
				dma_unmap_single(&priv->pdev->dev,
						 entry->TxBuffAddr, skb->len,
						 DMA_TO_DEVICE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				kfree_skb(skb);
				ring->idx = (ring->idx + 1) % ring->entries;
			}
			ring->idx = 0;
		}
	}
	spin_unlock_irqrestore(&priv->irq_th_lock, flags);
}

<<<<<<< HEAD
void rtl819x_UpdateRxPktTimeStamp(struct net_device *dev,
				  struct rtllib_rx_stats *stats)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);

	if (stats->bIsAMPDU && !stats->bFirstMPDU)
		stats->mac_time = priv->LastRxDescTSF;
	else
		priv->LastRxDescTSF = stats->mac_time;
}

long rtl819x_translate_todbm(struct r8192_priv *priv, u8 signal_strength_index)
=======
long rtl92e_translate_to_dbm(struct r8192_priv *priv, u8 signal_strength_index)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	long	signal_power;

	signal_power = (long)((signal_strength_index + 1) >> 1);
	signal_power -= 95;

	return signal_power;
}

<<<<<<< HEAD

void
rtl819x_update_rxsignalstatistics8190pci(
	struct r8192_priv *priv,
	struct rtllib_rx_stats *pprevious_stats
	)
{
	int weighting = 0;


=======
void rtl92e_update_rx_statistics(struct r8192_priv *priv,
				 struct rtllib_rx_stats *pprevious_stats)
{
	int weighting = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (priv->stats.recv_signal_power == 0)
		priv->stats.recv_signal_power =
					 pprevious_stats->RecvSignalPower;

	if (pprevious_stats->RecvSignalPower > priv->stats.recv_signal_power)
		weighting = 5;
	else if (pprevious_stats->RecvSignalPower <
		 priv->stats.recv_signal_power)
		weighting = (-5);
	priv->stats.recv_signal_power = (priv->stats.recv_signal_power * 5 +
					pprevious_stats->RecvSignalPower +
					weighting) / 6;
}

<<<<<<< HEAD
void rtl819x_process_cck_rxpathsel(struct r8192_priv *priv,
				   struct rtllib_rx_stats *pprevious_stats)
{
}


u8 rtl819x_query_rxpwrpercentage(char antpower)
=======
u8 rtl92e_rx_db_to_percent(s8 antpower)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if ((antpower <= -100) || (antpower >= 20))
		return	0;
	else if (antpower >= 0)
		return	100;
	else
		return	100 + antpower;

}	/* QueryRxPwrPercentage */

<<<<<<< HEAD
u8
rtl819x_evm_dbtopercentage(
	char value
	)
{
	char ret_val;

	ret_val = value;

	if (ret_val >= 0)
		ret_val = 0;
	if (ret_val <= -33)
		ret_val = -33;
	ret_val = 0 - ret_val;
	ret_val *= 3;
	if (ret_val == 99)
		ret_val = 100;
	return ret_val;
}

void
rtl8192_record_rxdesc_forlateruse(
	struct rtllib_rx_stats *psrc_stats,
	struct rtllib_rx_stats *ptarget_stats
)
=======
u8 rtl92e_evm_db_to_percent(s8 value)
{
	s8 ret_val = clamp(-value, 0, 33) * 3;

	if (ret_val == 99)
		ret_val = 100;

	return ret_val;
}

void rtl92e_copy_mpdu_stats(struct rtllib_rx_stats *psrc_stats,
			    struct rtllib_rx_stats *ptarget_stats)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	ptarget_stats->bIsAMPDU = psrc_stats->bIsAMPDU;
	ptarget_stats->bFirstMPDU = psrc_stats->bFirstMPDU;
}

<<<<<<< HEAD


static void rtl8192_rx_normal(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	struct rtllib_hdr_1addr *rtllib_hdr = NULL;
	bool unicast_packet = false;
	bool bLedBlinking = true;
	u16 fc = 0, type = 0;
	u32 skb_len = 0;
	int rx_queue_idx = RX_MPDU_QUEUE;

	struct rtllib_rx_stats stats = {
		.signal = 0,
		.noise = -98,
		.rate = 0,
		.freq = RTLLIB_24GHZ_BAND,
	};
	unsigned int count = priv->rxringcount;

	stats.nic_type = NIC_8192E;

	while (count--) {
		struct rx_desc *pdesc = &priv->rx_ring[rx_queue_idx]
					[priv->rx_idx[rx_queue_idx]];
		struct sk_buff *skb = priv->rx_buf[rx_queue_idx]
				      [priv->rx_idx[rx_queue_idx]];

		if (pdesc->OWN) {
			return;
		} else {
			struct sk_buff *new_skb;

			if (!priv->ops->rx_query_status_descriptor(dev, &stats,
			pdesc, skb))
				goto done;
			new_skb = dev_alloc_skb(priv->rxbuffersize);
			/* if allocation of new skb failed - drop current packet
			* and reuse skb */
			if (unlikely(!new_skb))
				goto done;

			pci_unmap_single(priv->pdev,
					*((dma_addr_t *)skb->cb),
					priv->rxbuffersize,
					PCI_DMA_FROMDEVICE);

			skb_put(skb, pdesc->Length);
			skb_reserve(skb, stats.RxDrvInfoSize +
				stats.RxBufShift);
			skb_trim(skb, skb->len - 4/*sCrcLng*/);
			rtllib_hdr = (struct rtllib_hdr_1addr *)skb->data;
			if (!is_broadcast_ether_addr(rtllib_hdr->addr1) &&
			!is_multicast_ether_addr(rtllib_hdr->addr1)) {
				/* unicast packet */
				unicast_packet = true;
			}
			fc = le16_to_cpu(rtllib_hdr->frame_ctl);
			type = WLAN_FC_GET_TYPE(fc);
			if (type == RTLLIB_FTYPE_MGMT)
				bLedBlinking = false;

			if (bLedBlinking)
				if (priv->rtllib->LedControlHandler)
					priv->rtllib->LedControlHandler(dev,
								LED_CTL_RX);

			if (stats.bCRC) {
				if (type != RTLLIB_FTYPE_MGMT)
					priv->stats.rxdatacrcerr++;
				else
					priv->stats.rxmgmtcrcerr++;
			}

			skb_len = skb->len;

			if (!rtllib_rx(priv->rtllib, skb, &stats)) {
				dev_kfree_skb_any(skb);
			} else {
				priv->stats.rxok++;
				if (unicast_packet)
					priv->stats.rxbytesunicast += skb_len;
			}

			skb = new_skb;
			skb->dev = dev;

			priv->rx_buf[rx_queue_idx][priv->rx_idx[rx_queue_idx]] =
									 skb;
			*((dma_addr_t *) skb->cb) = pci_map_single(priv->pdev,
						    skb_tail_pointer_rsl(skb),
						    priv->rxbuffersize,
						    PCI_DMA_FROMDEVICE);

		}
done:
		pdesc->BufferAddress = cpu_to_le32(*((dma_addr_t *)skb->cb));
		pdesc->OWN = 1;
		pdesc->Length = priv->rxbuffersize;
		if (priv->rx_idx[rx_queue_idx] == priv->rxringcount-1)
			pdesc->EOR = 1;
		priv->rx_idx[rx_queue_idx] = (priv->rx_idx[rx_queue_idx] + 1) %
					      priv->rxringcount;
	}

}

static void rtl8192_rx_cmd(struct net_device *dev)
{
}


static void rtl8192_tx_resume(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
=======
static void _rtl92e_rx_normal(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct ieee80211_hdr *rtllib_hdr = NULL;
	bool unicast_packet = false;
	u32 skb_len = 0;

	struct rtllib_rx_stats stats = {
		.signal = 0,
		.noise = (u8)-98,
		.rate = 0,
	};
	unsigned int count = priv->rxringcount;

	while (count--) {
		struct rx_desc *pdesc = &priv->rx_ring
					[priv->rx_idx];
		struct sk_buff *skb = priv->rx_buf
				      [priv->rx_idx];
		struct sk_buff *new_skb;

		if (pdesc->OWN)
			return;
		if (!rtl92e_get_rx_stats(dev, &stats, pdesc, skb))
			goto done;
		new_skb = dev_alloc_skb(priv->rxbuffersize);
		/* if allocation of new skb failed - drop current packet
		 * and reuse skb
		 */
		if (unlikely(!new_skb))
			goto done;

		dma_unmap_single(&priv->pdev->dev, *((dma_addr_t *)skb->cb),
				 priv->rxbuffersize, DMA_FROM_DEVICE);

		skb_put(skb, pdesc->Length);
		skb_reserve(skb, stats.RxDrvInfoSize +
			stats.RxBufShift);
		skb_trim(skb, skb->len - S_CRC_LEN);
		rtllib_hdr = (struct ieee80211_hdr *)skb->data;
		if (!is_multicast_ether_addr(rtllib_hdr->addr1)) {
			/* unicast packet */
			unicast_packet = true;
		}
		skb_len = skb->len;

		if (!rtllib_rx(priv->rtllib, skb, &stats)) {
			dev_kfree_skb_any(skb);
		} else {
			if (unicast_packet)
				priv->stats.rxbytesunicast += skb_len;
		}

		skb = new_skb;
		skb->dev = dev;

		priv->rx_buf[priv->rx_idx] = skb;
		*((dma_addr_t *)skb->cb) = dma_map_single(&priv->pdev->dev,
							  skb_tail_pointer(skb),
							  priv->rxbuffersize, DMA_FROM_DEVICE);
		if (dma_mapping_error(&priv->pdev->dev, *((dma_addr_t *)skb->cb))) {
			dev_kfree_skb_any(skb);
			return;
		}
done:
		pdesc->BufferAddress = *((dma_addr_t *)skb->cb);
		pdesc->OWN = 1;
		pdesc->Length = priv->rxbuffersize;
		if (priv->rx_idx == priv->rxringcount - 1)
			pdesc->EOR = 1;
		priv->rx_idx = (priv->rx_idx + 1) %
					      priv->rxringcount;
	}
}

static void _rtl92e_tx_resume(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rtllib_device *ieee = priv->rtllib;
	struct sk_buff *skb;
	int queue_index;

	for (queue_index = BK_QUEUE;
	     queue_index < MAX_QUEUE_SIZE; queue_index++) {
<<<<<<< HEAD
		while ((!skb_queue_empty(&ieee->skb_waitQ[queue_index])) &&
		(priv->rtllib->check_nic_enough_desc(dev, queue_index) > 0)) {
			skb = skb_dequeue(&ieee->skb_waitQ[queue_index]);
=======
		while ((!skb_queue_empty(&ieee->skb_waitq[queue_index])) &&
		(priv->rtllib->check_nic_enough_desc(dev, queue_index) > 0)) {
			skb = skb_dequeue(&ieee->skb_waitq[queue_index]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ieee->softmac_data_hard_start_xmit(skb, dev, 0);
		}
	}
}

<<<<<<< HEAD
void rtl8192_irq_tx_tasklet(struct r8192_priv *priv)
{
	rtl8192_tx_resume(priv->rtllib->dev);
}

void rtl8192_irq_rx_tasklet(struct r8192_priv *priv)
{
	rtl8192_rx_normal(priv->rtllib->dev);

	if (MAX_RX_QUEUE > 1)
		rtl8192_rx_cmd(priv->rtllib->dev);

	write_nic_dword(priv->rtllib->dev, INTA_MASK,
			read_nic_dword(priv->rtllib->dev, INTA_MASK) | IMR_RDU);
}

/****************************************************************************
 ---------------------------- NIC START/CLOSE STUFF---------------------------
*****************************************************************************/
void rtl8192_cancel_deferred_work(struct r8192_priv *priv)
{
	cancel_delayed_work(&priv->watch_dog_wq);
	cancel_delayed_work(&priv->update_beacon_wq);
=======
static void _rtl92e_irq_tx_tasklet(struct tasklet_struct *t)
{
	struct r8192_priv *priv = from_tasklet(priv, t, irq_tx_tasklet);

	_rtl92e_tx_resume(priv->rtllib->dev);
}

static void _rtl92e_irq_rx_tasklet(struct tasklet_struct *t)
{
	struct r8192_priv *priv = from_tasklet(priv, t, irq_rx_tasklet);

	_rtl92e_rx_normal(priv->rtllib->dev);

	rtl92e_writel(priv->rtllib->dev, INTA_MASK,
		      rtl92e_readl(priv->rtllib->dev, INTA_MASK) | IMR_RDU);
}

/****************************************************************************
 * ---------------------------- NIC START/CLOSE STUFF---------------------------
 ****************************************************************************/
static void _rtl92e_cancel_deferred_work(struct r8192_priv *priv)
{
	cancel_delayed_work_sync(&priv->watch_dog_wq);
	cancel_delayed_work_sync(&priv->update_beacon_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cancel_delayed_work(&priv->rtllib->hw_sleep_wq);
	cancel_work_sync(&priv->reset_wq);
	cancel_work_sync(&priv->qos_activate);
}

<<<<<<< HEAD
int _rtl8192_up(struct net_device *dev, bool is_silent_reset)
{
	if (_rtl8192_sta_up(dev, is_silent_reset) == -1)
=======
static int _rtl92e_up(struct net_device *dev)
{
	if (_rtl92e_sta_up(dev) == -1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;
	return 0;
}

<<<<<<< HEAD

static int rtl8192_open(struct net_device *dev)
=======
static int _rtl92e_open(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	int ret;

<<<<<<< HEAD
	down(&priv->wx_sem);
	ret = rtl8192_up(dev);
	up(&priv->wx_sem);
	return ret;

}


int rtl8192_up(struct net_device *dev)
=======
	mutex_lock(&priv->wx_mutex);
	ret = _rtl92e_try_up(dev);
	mutex_unlock(&priv->wx_mutex);
	return ret;
}

static int _rtl92e_try_up(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	if (priv->up == 1)
		return -1;
<<<<<<< HEAD
	return _rtl8192_up(dev, false);
}


static int rtl8192_close(struct net_device *dev)
=======
	return _rtl92e_up(dev);
}

static int _rtl92e_close(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	int ret;

	if ((rtllib_act_scanning(priv->rtllib, false)) &&
		!(priv->rtllib->softmac_features & IEEE_SOFTMAC_SCAN)) {
		rtllib_stop_scan(priv->rtllib);
	}

<<<<<<< HEAD
	down(&priv->wx_sem);

	ret = rtl8192_down(dev, true);

	up(&priv->wx_sem);

	return ret;

}

int rtl8192_down(struct net_device *dev, bool shutdownrf)
{
	if (rtl8192_sta_down(dev, shutdownrf) == -1)
=======
	mutex_lock(&priv->wx_mutex);

	ret = _rtl92e_down(dev, true);

	mutex_unlock(&priv->wx_mutex);

	return ret;
}

static int _rtl92e_down(struct net_device *dev, bool shutdownrf)
{
	if (_rtl92e_sta_down(dev, shutdownrf) == -1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;

	return 0;
}

<<<<<<< HEAD
void rtl8192_commit(struct net_device *dev)
=======
void rtl92e_commit(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	if (priv->up == 0)
		return;
<<<<<<< HEAD
	rtllib_softmac_stop_protocol(priv->rtllib, 0 , true);
	rtl8192_irq_disable(dev);
	priv->ops->stop_adapter(dev, true);
	_rtl8192_up(dev, false);
}

void rtl8192_restart(void *data)
{
	struct r8192_priv *priv = container_of_work_rsl(data, struct r8192_priv,
				  reset_wq);
	struct net_device *dev = priv->rtllib->dev;

	down(&priv->wx_sem);

	rtl8192_commit(dev);

	up(&priv->wx_sem);
}

static void r8192_set_multicast(struct net_device *dev)
=======
	rtllib_softmac_stop_protocol(priv->rtllib);
	rtl92e_irq_disable(dev);
	rtl92e_stop_adapter(dev, true);
	_rtl92e_up(dev);
}

static void _rtl92e_restart(void *data)
{
	struct r8192_priv *priv = container_of(data, struct r8192_priv, reset_wq);
	struct net_device *dev = priv->rtllib->dev;

	mutex_lock(&priv->wx_mutex);

	rtl92e_commit(dev);

	mutex_unlock(&priv->wx_mutex);
}

static void _rtl92e_set_multicast(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	short promisc;

	promisc = (dev->flags & IFF_PROMISC) ? 1 : 0;
	priv->promisc = promisc;
<<<<<<< HEAD

}


static int r8192_set_mac_adr(struct net_device *dev, void *mac)
=======
}

static int _rtl92e_set_mac_adr(struct net_device *dev, void *mac)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct sockaddr *addr = mac;

<<<<<<< HEAD
	down(&priv->wx_sem);

	memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);

	schedule_work(&priv->reset_wq);
	up(&priv->wx_sem);
=======
	mutex_lock(&priv->wx_mutex);

	eth_hw_addr_set(dev, addr->sa_data);

	schedule_work(&priv->reset_wq);
	mutex_unlock(&priv->wx_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
/* based on ipw2200 driver */
static int rtl8192_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	struct iwreq *wrq = (struct iwreq *)rq;
	int ret = -1;
	struct rtllib_device *ieee = priv->rtllib;
	u32 key[4];
	u8 broadcast_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	u8 zero_addr[6] = {0};
	struct iw_point *p = &wrq->u.data;
	struct ieee_param *ipw = NULL;

	down(&priv->wx_sem);

	switch (cmd) {
	case RTL_IOCTL_WPA_SUPPLICANT:
		if (p->length < sizeof(struct ieee_param) || !p->pointer) {
			ret = -EINVAL;
			goto out;
		}

		ipw = kmalloc(p->length, GFP_KERNEL);
		if (ipw == NULL) {
			ret = -ENOMEM;
			goto out;
		}
		if (copy_from_user(ipw, p->pointer, p->length)) {
			kfree(ipw);
			ret = -EFAULT;
			goto out;
		}

		if (ipw->cmd == IEEE_CMD_SET_ENCRYPTION) {
			if (ipw->u.crypt.set_tx) {
				if (strcmp(ipw->u.crypt.alg, "CCMP") == 0)
					ieee->pairwise_key_type = KEY_TYPE_CCMP;
				else if (strcmp(ipw->u.crypt.alg, "TKIP") == 0)
					ieee->pairwise_key_type = KEY_TYPE_TKIP;
				else if (strcmp(ipw->u.crypt.alg, "WEP") == 0) {
					if (ipw->u.crypt.key_len == 13)
						ieee->pairwise_key_type =
							 KEY_TYPE_WEP104;
					else if (ipw->u.crypt.key_len == 5)
						ieee->pairwise_key_type =
							 KEY_TYPE_WEP40;
				} else {
					ieee->pairwise_key_type = KEY_TYPE_NA;
				}

				if (ieee->pairwise_key_type) {
					if (memcmp(ieee->ap_mac_addr, zero_addr,
					    6) == 0)
						ieee->iw_mode = IW_MODE_ADHOC;
					memcpy((u8 *)key, ipw->u.crypt.key, 16);
					EnableHWSecurityConfig8192(dev);
					set_swcam(dev, 4, ipw->u.crypt.idx,
						  ieee->pairwise_key_type,
						  (u8 *)ieee->ap_mac_addr,
						  0, key, 0);
					setKey(dev, 4, ipw->u.crypt.idx,
					       ieee->pairwise_key_type,
					       (u8 *)ieee->ap_mac_addr, 0, key);
					if (ieee->iw_mode == IW_MODE_ADHOC) {
						set_swcam(dev, ipw->u.crypt.idx,
							ipw->u.crypt.idx,
							ieee->pairwise_key_type,
							(u8 *)ieee->ap_mac_addr,
							0, key, 0);
						setKey(dev, ipw->u.crypt.idx,
						       ipw->u.crypt.idx,
						       ieee->pairwise_key_type,
						       (u8 *)ieee->ap_mac_addr,
						       0, key);
					}
				}
				if ((ieee->pairwise_key_type == KEY_TYPE_CCMP)
				     && ieee->pHTInfo->bCurrentHTSupport) {
					write_nic_byte(dev, 0x173, 1);
				}

			} else {
				memcpy((u8 *)key, ipw->u.crypt.key, 16);
				if (strcmp(ipw->u.crypt.alg, "CCMP") == 0)
					ieee->group_key_type = KEY_TYPE_CCMP;
				else if (strcmp(ipw->u.crypt.alg, "TKIP") == 0)
					ieee->group_key_type = KEY_TYPE_TKIP;
				else if (strcmp(ipw->u.crypt.alg, "WEP") == 0) {
					if (ipw->u.crypt.key_len == 13)
						ieee->group_key_type =
							 KEY_TYPE_WEP104;
					else if (ipw->u.crypt.key_len == 5)
						ieee->group_key_type =
							 KEY_TYPE_WEP40;
				} else
					ieee->group_key_type = KEY_TYPE_NA;

				if (ieee->group_key_type) {
					set_swcam(dev, ipw->u.crypt.idx,
						  ipw->u.crypt.idx,
						  ieee->group_key_type,
						  broadcast_addr, 0, key, 0);
					setKey(dev, ipw->u.crypt.idx,
					       ipw->u.crypt.idx,
					       ieee->group_key_type,
					       broadcast_addr, 0, key);
				}
			}
		}

		ret = rtllib_wpa_supplicant_ioctl(priv->rtllib, &wrq->u.data,
						  0);
		kfree(ipw);
		break;
	default:
		ret = -EOPNOTSUPP;
		break;
	}

out:
	up(&priv->wx_sem);

	return ret;
}


irqreturn_type rtl8192_interrupt(int irq, void *netdev, struct pt_regs *regs)
{
	struct net_device *dev = (struct net_device *) netdev;
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	unsigned long flags;
	u32 inta;
	u32 intb;
	intb = 0;
=======
static irqreturn_t _rtl92e_irq(int irq, void *netdev)
{
	struct net_device *dev = netdev;
	struct r8192_priv *priv = rtllib_priv(dev);
	unsigned long flags;
	u32 inta;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (priv->irq_enabled == 0)
		goto done;

	spin_lock_irqsave(&priv->irq_th_lock, flags);

<<<<<<< HEAD
	priv->ops->interrupt_recognized(dev, &inta, &intb);
	priv->stats.shints++;
=======
	rtl92e_ack_irq(dev, &inta);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!inta) {
		spin_unlock_irqrestore(&priv->irq_th_lock, flags);
		goto done;
	}

	if (inta == 0xffff) {
		spin_unlock_irqrestore(&priv->irq_th_lock, flags);
		goto done;
	}

<<<<<<< HEAD
	priv->stats.ints++;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!netif_running(dev)) {
		spin_unlock_irqrestore(&priv->irq_th_lock, flags);
		goto done;
	}

<<<<<<< HEAD
	if (inta & IMR_TBDOK) {
		RT_TRACE(COMP_INTR, "beacon ok interrupt!\n");
		priv->stats.txbeaconokint++;
	}

	if (inta & IMR_TBDER) {
		RT_TRACE(COMP_INTR, "beacon ok interrupt!\n");
		priv->stats.txbeaconerr++;
	}

	if (inta & IMR_BDOK)
		RT_TRACE(COMP_INTR, "beacon interrupt!\n");

	if (inta  & IMR_MGNTDOK) {
		RT_TRACE(COMP_INTR, "Manage ok interrupt!\n");
		priv->stats.txmanageokint++;
		rtl8192_tx_isr(dev, MGNT_QUEUE);
		spin_unlock_irqrestore(&priv->irq_th_lock, flags);
		if (priv->rtllib->ack_tx_to_ieee) {
			if (rtl8192_is_tx_queue_empty(dev)) {
=======
	if (inta  & IMR_MGNTDOK) {
		_rtl92e_tx_isr(dev, MGNT_QUEUE);
		spin_unlock_irqrestore(&priv->irq_th_lock, flags);
		if (priv->rtllib->ack_tx_to_ieee) {
			if (_rtl92e_is_tx_queue_empty(dev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				priv->rtllib->ack_tx_to_ieee = 0;
				rtllib_ps_tx_ack(priv->rtllib, 1);
			}
		}
		spin_lock_irqsave(&priv->irq_th_lock, flags);
	}

<<<<<<< HEAD
	if (inta & IMR_COMDOK) {
		priv->stats.txcmdpktokint++;
		rtl8192_tx_isr(dev, TXCMD_QUEUE);
	}

	if (inta & IMR_HIGHDOK)
		rtl8192_tx_isr(dev, HIGH_QUEUE);

	if (inta & IMR_ROK) {
		priv->stats.rxint++;
		priv->InterruptLog.nIMR_ROK++;
		tasklet_schedule(&priv->irq_rx_tasklet);
	}

	if (inta & IMR_BcnInt) {
		RT_TRACE(COMP_INTR, "prepare beacon for interrupt!\n");
		tasklet_schedule(&priv->irq_prepare_beacon_tasklet);
	}

	if (inta & IMR_RDU) {
		RT_TRACE(COMP_INTR, "rx descriptor unavailable!\n");
		priv->stats.rxrdu++;
		write_nic_dword(dev, INTA_MASK,
				read_nic_dword(dev, INTA_MASK) & ~IMR_RDU);
		tasklet_schedule(&priv->irq_rx_tasklet);
	}

	if (inta & IMR_RXFOVW) {
		RT_TRACE(COMP_INTR, "rx overflow !\n");
		priv->stats.rxoverflow++;
		tasklet_schedule(&priv->irq_rx_tasklet);
	}

	if (inta & IMR_TXFOVW)
		priv->stats.txoverflow++;

	if (inta & IMR_BKDOK) {
		RT_TRACE(COMP_INTR, "BK Tx OK interrupt!\n");
		priv->stats.txbkokint++;
		priv->rtllib->LinkDetectInfo.NumTxOkInPeriod++;
		rtl8192_tx_isr(dev, BK_QUEUE);
	}

	if (inta & IMR_BEDOK) {
		RT_TRACE(COMP_INTR, "BE TX OK interrupt!\n");
		priv->stats.txbeokint++;
		priv->rtllib->LinkDetectInfo.NumTxOkInPeriod++;
		rtl8192_tx_isr(dev, BE_QUEUE);
	}

	if (inta & IMR_VIDOK) {
		RT_TRACE(COMP_INTR, "VI TX OK interrupt!\n");
		priv->stats.txviokint++;
		priv->rtllib->LinkDetectInfo.NumTxOkInPeriod++;
		rtl8192_tx_isr(dev, VI_QUEUE);
	}

	if (inta & IMR_VODOK) {
		priv->stats.txvookint++;
		RT_TRACE(COMP_INTR, "Vo TX OK interrupt!\n");
		priv->rtllib->LinkDetectInfo.NumTxOkInPeriod++;
		rtl8192_tx_isr(dev, VO_QUEUE);
=======
	if (inta & IMR_COMDOK)
		_rtl92e_tx_isr(dev, TXCMD_QUEUE);

	if (inta & IMR_HIGHDOK)
		_rtl92e_tx_isr(dev, HIGH_QUEUE);

	if (inta & IMR_ROK)
		tasklet_schedule(&priv->irq_rx_tasklet);

	if (inta & IMR_RDU) {
		rtl92e_writel(dev, INTA_MASK,
			      rtl92e_readl(dev, INTA_MASK) & ~IMR_RDU);
		tasklet_schedule(&priv->irq_rx_tasklet);
	}

	if (inta & IMR_RXFOVW)
		tasklet_schedule(&priv->irq_rx_tasklet);

	if (inta & IMR_BKDOK) {
		priv->rtllib->link_detect_info.num_tx_ok_in_period++;
		_rtl92e_tx_isr(dev, BK_QUEUE);
	}

	if (inta & IMR_BEDOK) {
		priv->rtllib->link_detect_info.num_tx_ok_in_period++;
		_rtl92e_tx_isr(dev, BE_QUEUE);
	}

	if (inta & IMR_VIDOK) {
		priv->rtllib->link_detect_info.num_tx_ok_in_period++;
		_rtl92e_tx_isr(dev, VI_QUEUE);
	}

	if (inta & IMR_VODOK) {
		priv->rtllib->link_detect_info.num_tx_ok_in_period++;
		_rtl92e_tx_isr(dev, VO_QUEUE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	spin_unlock_irqrestore(&priv->irq_th_lock, flags);

done:

	return IRQ_HANDLED;
}

<<<<<<< HEAD


/****************************************************************************
	---------------------------- PCI_STUFF---------------------------
*****************************************************************************/
static const struct net_device_ops rtl8192_netdev_ops = {
	.ndo_open = rtl8192_open,
	.ndo_stop = rtl8192_close,
	.ndo_tx_timeout = rtl8192_tx_timeout,
	.ndo_do_ioctl = rtl8192_ioctl,
	.ndo_set_rx_mode = r8192_set_multicast,
	.ndo_set_mac_address = r8192_set_mac_adr,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_change_mtu = eth_change_mtu,
	.ndo_start_xmit = rtllib_xmit,
};

static int __devinit rtl8192_pci_probe(struct pci_dev *pdev,
			const struct pci_device_id *id)
=======
/****************************************************************************
 * ---------------------------- PCI_STUFF---------------------------
 ****************************************************************************/
static const struct net_device_ops rtl8192_netdev_ops = {
	.ndo_open = _rtl92e_open,
	.ndo_stop = _rtl92e_close,
	.ndo_tx_timeout = _rtl92e_tx_timeout,
	.ndo_set_rx_mode = _rtl92e_set_multicast,
	.ndo_set_mac_address = _rtl92e_set_mac_adr,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_start_xmit = rtllib_xmit,
};

static int _rtl92e_pci_probe(struct pci_dev *pdev,
			     const struct pci_device_id *id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long ioaddr = 0;
	struct net_device *dev = NULL;
	struct r8192_priv *priv = NULL;
<<<<<<< HEAD
	struct rtl819x_ops *ops = (struct rtl819x_ops *)(id->driver_data);
	unsigned long pmem_start, pmem_len, pmem_flags;
	int err = -ENOMEM;
	bool bdma64 = false;
	u8 revision_id;

	RT_TRACE(COMP_INIT, "Configuring chip resources");

	if (pci_enable_device(pdev)) {
		RT_TRACE(COMP_ERR, "Failed to enable PCI device");
=======
	unsigned long pmem_start, pmem_len, pmem_flags;
	int err = -ENOMEM;

	if (pci_enable_device(pdev)) {
		dev_err(&pdev->dev, "Failed to enable PCI device");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EIO;
	}

	pci_set_master(pdev);

<<<<<<< HEAD
	if (!pci_set_dma_mask(pdev, DMA_BIT_MASK(32))) {
		if (pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(32))) {
			printk(KERN_INFO "Unable to obtain 32bit DMA for consistent allocations\n");
=======
	if (!dma_set_mask(&pdev->dev, DMA_BIT_MASK(32))) {
		if (dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(32))) {
			dev_info(&pdev->dev,
				 "Unable to obtain 32bit DMA for consistent allocations\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto err_pci_disable;
		}
	}
	dev = alloc_rtllib(sizeof(struct r8192_priv));
	if (!dev)
		goto err_pci_disable;

	err = -ENODEV;
<<<<<<< HEAD
	if (bdma64)
		dev->features |= NETIF_F_HIGHDMA;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pci_set_drvdata(pdev, dev);
	SET_NETDEV_DEV(dev, &pdev->dev);
	priv = rtllib_priv(dev);
	priv->rtllib = (struct rtllib_device *)netdev_priv_rsl(dev);
	priv->pdev = pdev;
	priv->rtllib->pdev = pdev;
	if ((pdev->subsystem_vendor == PCI_VENDOR_ID_DLINK) &&
	    (pdev->subsystem_device == 0x3304))
		priv->rtllib->bSupportRemoteWakeUp = 1;
	else
		priv->rtllib->bSupportRemoteWakeUp = 0;

	pmem_start = pci_resource_start(pdev, 1);
	pmem_len = pci_resource_len(pdev, 1);
	pmem_flags = pci_resource_flags(pdev, 1);

	if (!(pmem_flags & IORESOURCE_MEM)) {
<<<<<<< HEAD
		RT_TRACE(COMP_ERR, "region #1 not a MMIO resource, aborting");
		goto err_rel_rtllib;
	}

	printk(KERN_INFO "Memory mapped space start: 0x%08lx\n", pmem_start);
	if (!request_mem_region(pmem_start, pmem_len, DRV_NAME)) {
		RT_TRACE(COMP_ERR, "request_mem_region failed!");
		goto err_rel_rtllib;
	}


	ioaddr = (unsigned long)ioremap_nocache(pmem_start, pmem_len);
	if (ioaddr == (unsigned long)NULL) {
		RT_TRACE(COMP_ERR, "ioremap failed!");
=======
		netdev_err(dev, "region #1 not a MMIO resource, aborting");
		goto err_rel_rtllib;
	}

	dev_info(&pdev->dev, "Memory mapped space start: 0x%08lx\n",
		 pmem_start);
	if (!request_mem_region(pmem_start, pmem_len, DRV_NAME)) {
		netdev_err(dev, "request_mem_region failed!");
		goto err_rel_rtllib;
	}

	ioaddr = (unsigned long)ioremap(pmem_start, pmem_len);
	if (ioaddr == (unsigned long)NULL) {
		netdev_err(dev, "ioremap failed!");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_rel_mem;
	}

	dev->mem_start = ioaddr;
	dev->mem_end = ioaddr + pci_resource_len(pdev, 0);

<<<<<<< HEAD
	pci_read_config_byte(pdev, 0x08, &revision_id);
	/* If the revisionid is 0x10, the device uses rtl8192se. */
	if (pdev->device == 0x8192 && revision_id == 0x10)
		goto err_rel_mem;

	priv->ops = ops;

	if (rtl8192_pci_findadapter(pdev, dev) == false)
		goto err_rel_mem;
=======
	if (!rtl92e_check_adapter(pdev, dev))
		goto err_unmap;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev->irq = pdev->irq;
	priv->irq = 0;

	dev->netdev_ops = &rtl8192_netdev_ops;

<<<<<<< HEAD
	dev->wireless_handlers = (struct iw_handler_def *)
				 &r8192_wx_handlers_def;
=======
	dev->wireless_handlers = &r8192_wx_handlers_def;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev->ethtool_ops = &rtl819x_ethtool_ops;

	dev->type = ARPHRD_ETHER;
	dev->watchdog_timeo = HZ * 3;

<<<<<<< HEAD
	if (dev_alloc_name(dev, ifname) < 0) {
		RT_TRACE(COMP_INIT, "Oops: devname already taken! Trying "
			 "wlan%%d...\n");
			dev_alloc_name(dev, ifname);
	}

	RT_TRACE(COMP_INIT, "Driver probe completed1\n");
	if (rtl8192_init(dev) != 0) {
		RT_TRACE(COMP_ERR, "Initialization failed");
=======
	if (dev_alloc_name(dev, ifname) < 0)
		dev_alloc_name(dev, ifname);

	if (_rtl92e_init(dev) != 0) {
		netdev_warn(dev, "Initialization failed");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_free_irq;
	}

	netif_carrier_off(dev);
	netif_stop_queue(dev);

<<<<<<< HEAD
	register_netdev(dev);
	RT_TRACE(COMP_INIT, "dev name: %s\n", dev->name);

	rtl8192_proc_init_one(dev);

	if (priv->polling_timer_on == 0)
		check_rfctrl_gpio_timer((unsigned long)dev);

	RT_TRACE(COMP_INIT, "Driver probe completed\n");
=======
	if (register_netdev(dev))
		goto err_free_irq;

	if (priv->polling_timer_on == 0)
		rtl92e_check_rfctrl_gpio_timer(&priv->gpio_polling_timer);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

err_free_irq:
	free_irq(dev->irq, dev);
	priv->irq = 0;
<<<<<<< HEAD
=======
err_unmap:
	iounmap((void __iomem *)ioaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_rel_mem:
	release_mem_region(pmem_start, pmem_len);
err_rel_rtllib:
	free_rtllib(dev);
<<<<<<< HEAD

	DMESG("wlan driver load failed\n");
	pci_set_drvdata(pdev, NULL);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_pci_disable:
	pci_disable_device(pdev);
	return err;
}

<<<<<<< HEAD
static void __devexit rtl8192_pci_disconnect(struct pci_dev *pdev)
{
	struct net_device *dev = pci_get_drvdata(pdev);
	struct r8192_priv *priv ;
=======
static void _rtl92e_pci_disconnect(struct pci_dev *pdev)
{
	struct net_device *dev = pci_get_drvdata(pdev);
	struct r8192_priv *priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 i;

	if (dev) {
		unregister_netdev(dev);

		priv = rtllib_priv(dev);

		del_timer_sync(&priv->gpio_polling_timer);
<<<<<<< HEAD
		cancel_delayed_work(&priv->gpio_change_rf_wq);
		priv->polling_timer_on = 0;
		rtl8192_proc_remove_one(dev);
		rtl8192_down(dev, true);
		deinit_hal_dm(dev);
		if (priv->pFirmware) {
			vfree(priv->pFirmware);
			priv->pFirmware = NULL;
		}
		destroy_workqueue(priv->priv_wq);
		rtl8192_free_rx_ring(dev);
		for (i = 0; i < MAX_TX_QUEUE_COUNT; i++)
			rtl8192_free_tx_ring(dev, i);

		if (priv->irq) {
			printk(KERN_INFO "Freeing irq %d\n", dev->irq);
			free_irq(dev->irq, dev);
			priv->irq = 0;
		}
		free_rtllib(dev);

		kfree(priv->scan_cmd);
=======
		cancel_delayed_work_sync(&priv->gpio_change_rf_wq);
		priv->polling_timer_on = 0;
		_rtl92e_down(dev, true);
		rtl92e_dm_deinit(dev);
		vfree(priv->fw_info);
		priv->fw_info = NULL;
		_rtl92e_free_rx_ring(dev);
		for (i = 0; i < MAX_TX_QUEUE_COUNT; i++)
			_rtl92e_free_tx_ring(dev, i);

		if (priv->irq) {
			dev_info(&pdev->dev, "Freeing irq %d\n", dev->irq);
			free_irq(dev->irq, dev);
			priv->irq = 0;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (dev->mem_start != 0) {
			iounmap((void __iomem *)dev->mem_start);
			release_mem_region(pci_resource_start(pdev, 1),
					pci_resource_len(pdev, 1));
		}
<<<<<<< HEAD
	} else {
		priv = rtllib_priv(dev);
	}

	pci_disable_device(pdev);
	RT_TRACE(COMP_DOWN, "wlan driver removed\n");
}

bool NicIFEnableNIC(struct net_device *dev)
{
	bool init_status = true;
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *pPSC = (struct rt_pwr_save_ctrl *)
					(&(priv->rtllib->PowerSaveControl));

	if (IS_NIC_DOWN(priv)) {
		RT_TRACE(COMP_ERR, "ERR!!! %s(): Driver is already down!\n",
			 __func__);
		priv->bdisable_nic = false;
		return RT_STATUS_FAILURE;
	}

	RT_TRACE(COMP_PS, "===========>%s()\n", __func__);
	priv->bfirst_init = true;
	init_status = priv->ops->initialize_adapter(dev);
	if (init_status != true) {
		RT_TRACE(COMP_ERR, "ERR!!! %s(): initialization is failed!\n",
			 __func__);
		priv->bdisable_nic = false;
		return -1;
	}
	RT_TRACE(COMP_INIT, "start adapter finished\n");
	RT_CLEAR_PS_LEVEL(pPSC, RT_RF_OFF_LEVL_HALT_NIC);
	priv->bfirst_init = false;

	rtl8192_irq_enable(dev);
	priv->bdisable_nic = false;
	RT_TRACE(COMP_PS, "<===========%s()\n", __func__);
	return init_status;
}
bool NicIFDisableNIC(struct net_device *dev)
{
	bool	status = true;
	struct r8192_priv *priv = rtllib_priv(dev);
	u8 tmp_state = 0;
	RT_TRACE(COMP_PS, "=========>%s()\n", __func__);
	priv->bdisable_nic = true;
	tmp_state = priv->rtllib->state;
	rtllib_softmac_stop_protocol(priv->rtllib, 0, false);
	priv->rtllib->state = tmp_state;
	rtl8192_cancel_deferred_work(priv);
	rtl8192_irq_disable(dev);

	priv->ops->stop_adapter(dev, false);
	RT_TRACE(COMP_PS, "<=========%s()\n", __func__);

	return status;
}

static int __init rtl8192_pci_module_init(void)
{
	printk(KERN_INFO "\nLinux kernel driver for RTL8192E WLAN cards\n");
	printk(KERN_INFO "Copyright (c) 2007-2008, Realsil Wlan Driver\n");

	rtl8192_proc_module_init();
	if (0 != pci_register_driver(&rtl8192_pci_driver)) {
		DMESG("No device found");
		/*pci_unregister_driver (&rtl8192_pci_driver);*/
		return -ENODEV;
	}
	return 0;
}

static void __exit rtl8192_pci_module_exit(void)
{
	pci_unregister_driver(&rtl8192_pci_driver);

	RT_TRACE(COMP_DOWN, "Exiting");
	rtl8192_proc_module_remove();
}

void check_rfctrl_gpio_timer(unsigned long data)
{
	struct r8192_priv *priv = rtllib_priv((struct net_device *)data);

	priv->polling_timer_on = 1;

	queue_delayed_work_rsl(priv->priv_wq, &priv->gpio_change_rf_wq, 0);

	mod_timer(&priv->gpio_polling_timer, jiffies +
		  MSECS(RTLLIB_WATCH_DOG_TIME));
}

/***************************************************************************
	------------------- module init / exit stubs ----------------
****************************************************************************/
module_init(rtl8192_pci_module_init);
module_exit(rtl8192_pci_module_exit);

=======

		free_rtllib(dev);
	}

	pci_disable_device(pdev);
}

bool rtl92e_enable_nic(struct net_device *dev)
{
	bool init_status = true;
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *psc = (struct rt_pwr_save_ctrl *)
					(&priv->rtllib->pwr_save_ctrl);

	if (!priv->up) {
		netdev_warn(dev, "%s(): Driver is already down!\n", __func__);
		return false;
	}

	init_status = rtl92e_start_adapter(dev);
	if (!init_status) {
		netdev_warn(dev, "%s(): Initialization failed!\n", __func__);
		return false;
	}
	RT_CLEAR_PS_LEVEL(psc, RT_RF_OFF_LEVL_HALT_NIC);

	rtl92e_irq_enable(dev);
	return init_status;
}

module_pci_driver(rtl8192_pci_driver);

void rtl92e_check_rfctrl_gpio_timer(struct timer_list *t)
{
	struct r8192_priv *priv = from_timer(priv, t, gpio_polling_timer);

	priv->polling_timer_on = 1;

	schedule_delayed_work(&priv->gpio_change_rf_wq, 0);

	mod_timer(&priv->gpio_polling_timer, jiffies +
		  msecs_to_jiffies(RTLLIB_WATCH_DOG_TIME));
}

/***************************************************************************
 * ------------------- module init / exit stubs ----------------
 ***************************************************************************/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION("Linux driver for Realtek RTL819x WiFi cards");
MODULE_AUTHOR(DRV_COPYRIGHT " " DRV_AUTHOR);
MODULE_VERSION(DRV_VERSION);
MODULE_LICENSE("GPL");
<<<<<<< HEAD

module_param(ifname, charp, S_IRUGO|S_IWUSR);
module_param(hwwep, int, S_IRUGO|S_IWUSR);
module_param(channels, int, S_IRUGO|S_IWUSR);

MODULE_PARM_DESC(ifname, " Net interface name, wlan%d=default");
MODULE_PARM_DESC(hwwep, " Try to use hardware WEP support(default use hw. set 0 to use software security)");
MODULE_PARM_DESC(channels, " Channel bitmask for specific locales. NYI");
=======
MODULE_FIRMWARE(RTL8192E_BOOT_IMG_FW);
MODULE_FIRMWARE(RTL8192E_MAIN_IMG_FW);
MODULE_FIRMWARE(RTL8192E_DATA_IMG_FW);

module_param(ifname, charp, 0644);
module_param(hwwep, int, 0644);

MODULE_PARM_DESC(ifname, " Net interface name, wlan%d=default");
MODULE_PARM_DESC(hwwep, " Try to use hardware WEP support(default use hw. set 0 to use software security)");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
