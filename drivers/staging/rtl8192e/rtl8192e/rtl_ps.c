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
 *****************************************************************************/
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
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "rtl_ps.h"
#include "rtl_core.h"
#include "r8192E_phy.h"
#include "r8192E_phyreg.h"
#include "r8190P_rtl8256.h" /* RTL8225 Radio frontend */
#include "r8192E_cmdpkt.h"
<<<<<<< HEAD

static void rtl8192_hw_sleep_down(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	unsigned long flags = 0;
	spin_lock_irqsave(&priv->rf_ps_lock, flags);
	if (priv->RFChangeInProgress) {
		spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
		RT_TRACE(COMP_DBG, "rtl8192_hw_sleep_down(): RF Change in "
			 "progress!\n");
		return;
	}
	spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
	RT_TRACE(COMP_DBG, "%s()============>come to sleep down\n", __func__);

	MgntActSet_RF_State(dev, eRfSleep, RF_CHANGE_BY_PS, false);
}

void rtl8192_hw_sleep_wq(void *data)
=======
#include <linux/jiffies.h>

static void _rtl92e_hw_sleep(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	unsigned long flags = 0;

	spin_lock_irqsave(&priv->rf_ps_lock, flags);
	if (priv->rf_change_in_progress) {
		spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
		return;
	}
	spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
	rtl92e_set_rf_state(dev, rf_sleep, RF_CHANGE_BY_PS);
}

void rtl92e_hw_sleep_wq(void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rtllib_device *ieee = container_of_dwork_rsl(data,
				     struct rtllib_device, hw_sleep_wq);
	struct net_device *dev = ieee->dev;
<<<<<<< HEAD
	rtl8192_hw_sleep_down(dev);
}

void rtl8192_hw_wakeup(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	unsigned long flags = 0;
	spin_lock_irqsave(&priv->rf_ps_lock, flags);
	if (priv->RFChangeInProgress) {
		spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
		RT_TRACE(COMP_DBG, "rtl8192_hw_wakeup(): RF Change in "
			 "progress!\n");
		queue_delayed_work_rsl(priv->rtllib->wq,
				       &priv->rtllib->hw_wakeup_wq, MSECS(10));
		return;
	}
	spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
	RT_TRACE(COMP_PS, "%s()============>come to wake up\n", __func__);
	MgntActSet_RF_State(dev, eRfOn, RF_CHANGE_BY_PS, false);
}

void rtl8192_hw_wakeup_wq(void *data)
=======

	_rtl92e_hw_sleep(dev);
}

void rtl92e_hw_wakeup(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	unsigned long flags = 0;

	spin_lock_irqsave(&priv->rf_ps_lock, flags);
	if (priv->rf_change_in_progress) {
		spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
		schedule_delayed_work(&priv->rtllib->hw_wakeup_wq,
				      msecs_to_jiffies(10));
		return;
	}
	spin_unlock_irqrestore(&priv->rf_ps_lock, flags);
	rtl92e_set_rf_state(dev, rf_on, RF_CHANGE_BY_PS);
}

void rtl92e_hw_wakeup_wq(void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rtllib_device *ieee = container_of_dwork_rsl(data,
				     struct rtllib_device, hw_wakeup_wq);
	struct net_device *dev = ieee->dev;
<<<<<<< HEAD
	rtl8192_hw_wakeup(dev);

=======

	rtl92e_hw_wakeup(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define MIN_SLEEP_TIME 50
#define MAX_SLEEP_TIME 10000
<<<<<<< HEAD
void rtl8192_hw_to_sleep(struct net_device *dev, u64 time)
=======
void rtl92e_enter_sleep(struct net_device *dev, u64 time)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	u32 tmp;
	unsigned long flags;
<<<<<<< HEAD

	spin_lock_irqsave(&priv->ps_lock, flags);

	time -= MSECS(8+16+7);

	if ((time - jiffies) <= MSECS(MIN_SLEEP_TIME)) {
		spin_unlock_irqrestore(&priv->ps_lock, flags);
		printk(KERN_INFO "too short to sleep::%lld < %ld\n",
		       time - jiffies, MSECS(MIN_SLEEP_TIME));
		return;
	}

	if ((time - jiffies) > MSECS(MAX_SLEEP_TIME)) {
		printk(KERN_INFO "========>too long to sleep:%lld > %ld\n",
		       time - jiffies,  MSECS(MAX_SLEEP_TIME));
=======
	unsigned long timeout;

	spin_lock_irqsave(&priv->ps_lock, flags);

	time -= msecs_to_jiffies(8 + 16 + 7);

	timeout = jiffies + msecs_to_jiffies(MIN_SLEEP_TIME);
	if (time_before((unsigned long)time, timeout)) {
		spin_unlock_irqrestore(&priv->ps_lock, flags);
		netdev_info(dev, "too short to sleep::%lld < %ld\n",
			    time - jiffies, msecs_to_jiffies(MIN_SLEEP_TIME));
		return;
	}
	timeout = jiffies + msecs_to_jiffies(MAX_SLEEP_TIME);
	if (time_after((unsigned long)time, timeout)) {
		netdev_info(dev, "========>too long to sleep:%lld > %ld\n",
			    time - jiffies, msecs_to_jiffies(MAX_SLEEP_TIME));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock_irqrestore(&priv->ps_lock, flags);
		return;
	}
	tmp = time - jiffies;
<<<<<<< HEAD
	queue_delayed_work_rsl(priv->rtllib->wq,
			&priv->rtllib->hw_wakeup_wq, tmp);
	queue_delayed_work_rsl(priv->rtllib->wq,
			(void *)&priv->rtllib->hw_sleep_wq, 0);
	spin_unlock_irqrestore(&priv->ps_lock, flags);
}

static void InactivePsWorkItemCallback(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *pPSC = (struct rt_pwr_save_ctrl *)
					&(priv->rtllib->PowerSaveControl);

	RT_TRACE(COMP_PS, "InactivePsWorkItemCallback() --------->\n");
	pPSC->bSwRfProcessing = true;

	RT_TRACE(COMP_PS, "InactivePsWorkItemCallback(): Set RF to %s.\n",
		 pPSC->eInactivePowerState == eRfOff ? "OFF" : "ON");
	MgntActSet_RF_State(dev, pPSC->eInactivePowerState, RF_CHANGE_BY_IPS,
			    false);

	pPSC->bSwRfProcessing = false;
	RT_TRACE(COMP_PS, "InactivePsWorkItemCallback() <---------\n");
}

void IPSEnter(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *pPSC = (struct rt_pwr_save_ctrl *)
					&(priv->rtllib->PowerSaveControl);
	enum rt_rf_power_state rtState;

	if (pPSC->bInactivePs) {
		rtState = priv->rtllib->eRFPowerState;
		if (rtState == eRfOn && !pPSC->bSwRfProcessing &&
			(priv->rtllib->state != RTLLIB_LINKED) &&
			(priv->rtllib->iw_mode != IW_MODE_MASTER)) {
			RT_TRACE(COMP_PS, "IPSEnter(): Turn off RF.\n");
			pPSC->eInactivePowerState = eRfOff;
			priv->isRFOff = true;
			priv->bInPowerSaveMode = true;
			InactivePsWorkItemCallback(dev);
		}
	}
}

void IPSLeave(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *pPSC = (struct rt_pwr_save_ctrl *)
					&(priv->rtllib->PowerSaveControl);
	enum rt_rf_power_state rtState;

	if (pPSC->bInactivePs) {
		rtState = priv->rtllib->eRFPowerState;
		if (rtState != eRfOn  && !pPSC->bSwRfProcessing &&
		    priv->rtllib->RfOffReason <= RF_CHANGE_BY_IPS) {
			RT_TRACE(COMP_PS, "IPSLeave(): Turn on RF.\n");
			pPSC->eInactivePowerState = eRfOn;
			priv->bInPowerSaveMode = false;
			InactivePsWorkItemCallback(dev);
		}
	}
}

void IPSLeave_wq(void *data)
{
	struct rtllib_device *ieee = container_of_work_rsl(data,
				     struct rtllib_device, ips_leave_wq);
	struct net_device *dev = ieee->dev;
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	down(&priv->rtllib->ips_sem);
	IPSLeave(dev);
	up(&priv->rtllib->ips_sem);
}

void rtllib_ips_leave_wq(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	enum rt_rf_power_state rtState;
	rtState = priv->rtllib->eRFPowerState;

	if (priv->rtllib->PowerSaveControl.bInactivePs) {
		if (rtState == eRfOff) {
			if (priv->rtllib->RfOffReason > RF_CHANGE_BY_IPS) {
				RT_TRACE(COMP_ERR, "%s(): RF is OFF.\n",
					 __func__);
				return;
			} else {
				printk(KERN_INFO "=========>%s(): IPSLeave\n",
				       __func__);
				queue_work_rsl(priv->rtllib->wq,
					       &priv->rtllib->ips_leave_wq);
			}
		}
	}
}

void rtllib_ips_leave(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	down(&priv->rtllib->ips_sem);
	IPSLeave(dev);
	up(&priv->rtllib->ips_sem);
}

static bool MgntActSet_802_11_PowerSaveMode(struct net_device *dev,
					    u8 rtPsMode)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	if (priv->rtllib->iw_mode == IW_MODE_ADHOC)
		return false;

	RT_TRACE(COMP_LPS, "%s(): set ieee->ps = %x\n", __func__, rtPsMode);
	if (!priv->ps_force)
		priv->rtllib->ps = rtPsMode;
=======
	schedule_delayed_work(&priv->rtllib->hw_wakeup_wq, tmp);
	schedule_delayed_work(&priv->rtllib->hw_sleep_wq, 0);
	spin_unlock_irqrestore(&priv->ps_lock, flags);
}

static void _rtl92e_ps_update_rf_state(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *psc = (struct rt_pwr_save_ctrl *)
					&priv->rtllib->pwr_save_ctrl;

	psc->bSwRfProcessing = true;
	rtl92e_set_rf_state(dev, psc->eInactivePowerState, RF_CHANGE_BY_IPS);

	psc->bSwRfProcessing = false;
}

void rtl92e_ips_enter(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *psc = (struct rt_pwr_save_ctrl *)
					&priv->rtllib->pwr_save_ctrl;
	enum rt_rf_power_state rt_state;

	rt_state = priv->rtllib->rf_power_state;
	if (rt_state == rf_on && !psc->bSwRfProcessing &&
		(priv->rtllib->link_state != MAC80211_LINKED)) {
		psc->eInactivePowerState = rf_off;
		_rtl92e_ps_update_rf_state(dev);
	}
}

void rtl92e_ips_leave(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *psc = (struct rt_pwr_save_ctrl *)
					&priv->rtllib->pwr_save_ctrl;
	enum rt_rf_power_state rt_state;

	rt_state = priv->rtllib->rf_power_state;
	if (rt_state != rf_on  && !psc->bSwRfProcessing &&
	    priv->rtllib->rf_off_reason <= RF_CHANGE_BY_IPS) {
		psc->eInactivePowerState = rf_on;
		_rtl92e_ps_update_rf_state(dev);
	}
}

void rtl92e_ips_leave_wq(void *data)
{
	struct rtllib_device *ieee = container_of(data, struct rtllib_device, ips_leave_wq);
	struct net_device *dev = ieee->dev;
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);

	mutex_lock(&priv->rtllib->ips_mutex);
	rtl92e_ips_leave(dev);
	mutex_unlock(&priv->rtllib->ips_mutex);
}

void rtl92e_rtllib_ips_leave_wq(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	enum rt_rf_power_state rt_state;

	rt_state = priv->rtllib->rf_power_state;
	if (rt_state == rf_off) {
		if (priv->rtllib->rf_off_reason > RF_CHANGE_BY_IPS) {
			netdev_warn(dev, "%s(): RF is OFF.\n",
				    __func__);
			return;
		}
		netdev_info(dev, "=========>%s(): rtl92e_ips_leave\n",
			    __func__);
		schedule_work(&priv->rtllib->ips_leave_wq);
	}
}

void rtl92e_rtllib_ips_leave(struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);

	mutex_lock(&priv->rtllib->ips_mutex);
	rtl92e_ips_leave(dev);
	mutex_unlock(&priv->rtllib->ips_mutex);
}

static bool _rtl92e_ps_set_mode(struct net_device *dev, u8 rtPsMode)
{
	struct r8192_priv *priv = rtllib_priv(dev);

	priv->rtllib->ps = rtPsMode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (priv->rtllib->sta_sleep != LPS_IS_WAKE &&
	    rtPsMode == RTLLIB_PS_DISABLED) {
		unsigned long flags;

<<<<<<< HEAD
		rtl8192_hw_wakeup(dev);
		priv->rtllib->sta_sleep = LPS_IS_WAKE;

		spin_lock_irqsave(&(priv->rtllib->mgmt_tx_lock), flags);
		RT_TRACE(COMP_DBG, "LPS leave: notify AP we are awaked"
			 " ++++++++++ SendNullFunctionData\n");
=======
		rtl92e_hw_wakeup(dev);
		priv->rtllib->sta_sleep = LPS_IS_WAKE;

		spin_lock_irqsave(&(priv->rtllib->mgmt_tx_lock), flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rtllib_sta_ps_send_null_frame(priv->rtllib, 0);
		spin_unlock_irqrestore(&(priv->rtllib->mgmt_tx_lock), flags);
	}

	return true;
}

<<<<<<< HEAD
void LeisurePSEnter(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *pPSC = (struct rt_pwr_save_ctrl *)
					&(priv->rtllib->PowerSaveControl);

	RT_TRACE(COMP_PS, "LeisurePSEnter()...\n");
	RT_TRACE(COMP_PS, "pPSC->bLeisurePs = %d, ieee->ps = %d,pPSC->LpsIdle"
		 "Count is %d,RT_CHECK_FOR_HANG_PERIOD is %d\n",
		 pPSC->bLeisurePs, priv->rtllib->ps, pPSC->LpsIdleCount,
		 RT_CHECK_FOR_HANG_PERIOD);

	if (!((priv->rtllib->iw_mode == IW_MODE_INFRA) &&
	    (priv->rtllib->state == RTLLIB_LINKED))
	    || (priv->rtllib->iw_mode == IW_MODE_ADHOC) ||
	    (priv->rtllib->iw_mode == IW_MODE_MASTER))
		return;

	if (pPSC->bLeisurePs) {
		if (pPSC->LpsIdleCount >= RT_CHECK_FOR_HANG_PERIOD) {

			if (priv->rtllib->ps == RTLLIB_PS_DISABLED) {

				RT_TRACE(COMP_LPS, "LeisurePSEnter(): Enter "
					 "802.11 power save mode...\n");

				if (!pPSC->bFwCtrlLPS) {
					if (priv->rtllib->SetFwCmdHandler)
						priv->rtllib->SetFwCmdHandler(
							dev, FW_CMD_LPS_ENTER);
				}
				MgntActSet_802_11_PowerSaveMode(dev,
							 RTLLIB_PS_MBCAST |
							 RTLLIB_PS_UNICAST);
			}
		} else
			pPSC->LpsIdleCount++;
	}
}

void LeisurePSLeave(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *pPSC = (struct rt_pwr_save_ctrl *)
					&(priv->rtllib->PowerSaveControl);


	RT_TRACE(COMP_PS, "LeisurePSLeave()...\n");
	RT_TRACE(COMP_PS, "pPSC->bLeisurePs = %d, ieee->ps = %d\n",
		pPSC->bLeisurePs, priv->rtllib->ps);

	if (pPSC->bLeisurePs) {
		if (priv->rtllib->ps != RTLLIB_PS_DISABLED) {
			RT_TRACE(COMP_LPS, "LeisurePSLeave(): Busy Traffic , "
				 "Leave 802.11 power save..\n");
			MgntActSet_802_11_PowerSaveMode(dev,
					 RTLLIB_PS_DISABLED);

			if (!pPSC->bFwCtrlLPS) {
				if (priv->rtllib->SetFwCmdHandler)
					priv->rtllib->SetFwCmdHandler(dev,
							 FW_CMD_LPS_LEAVE);
		    }
		}
	}
}
=======
void rtl92e_leisure_ps_enter(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *psc = (struct rt_pwr_save_ctrl *)
					&priv->rtllib->pwr_save_ctrl;

	if (!((priv->rtllib->iw_mode == IW_MODE_INFRA) &&
	    (priv->rtllib->link_state == MAC80211_LINKED)))
		return;

	if (psc->bLeisurePs) {
		if (psc->lps_idle_count >= RT_CHECK_FOR_HANG_PERIOD) {

			if (priv->rtllib->ps == RTLLIB_PS_DISABLED)
				_rtl92e_ps_set_mode(dev, RTLLIB_PS_MBCAST | RTLLIB_PS_UNICAST);
		} else {
			psc->lps_idle_count++;
		}
	}
}

void rtl92e_leisure_ps_leave(struct net_device *dev)
{
	struct r8192_priv *priv = rtllib_priv(dev);
	struct rt_pwr_save_ctrl *psc = (struct rt_pwr_save_ctrl *)
					&priv->rtllib->pwr_save_ctrl;

	if (psc->bLeisurePs) {
		if (priv->rtllib->ps != RTLLIB_PS_DISABLED)
			_rtl92e_ps_set_mode(dev, RTLLIB_PS_DISABLED);
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
