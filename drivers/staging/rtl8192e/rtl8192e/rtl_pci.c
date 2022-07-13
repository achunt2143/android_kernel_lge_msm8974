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
#include "rtl_pci.h"
#include "rtl_core.h"

static void rtl8192_parse_pci_configuration(struct pci_dev *pdev,
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
#include "rtl_pci.h"
#include "rtl_core.h"

static void _rtl92e_parse_pci_configuration(struct pci_dev *pdev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					    struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);

	u8 tmp;
<<<<<<< HEAD
	int pos;
	u8 LinkCtrlReg;

	pos = pci_find_capability(priv->pdev, PCI_CAP_ID_EXP);
	pci_read_config_byte(priv->pdev, pos + PCI_EXP_LNKCTL, &LinkCtrlReg);
	priv->NdisAdapter.LinkCtrlReg = LinkCtrlReg;

	RT_TRACE(COMP_INIT, "Link Control Register =%x\n",
		 priv->NdisAdapter.LinkCtrlReg);

	pci_read_config_byte(pdev, 0x98, &tmp);
	tmp |= BIT4;
=======
	u16 link_ctrl_reg;

	pcie_capability_read_word(priv->pdev, PCI_EXP_LNKCTL, &link_ctrl_reg);

	pci_read_config_byte(pdev, 0x98, &tmp);
	tmp |= BIT(4);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pci_write_config_byte(pdev, 0x98, tmp);

	tmp = 0x17;
	pci_write_config_byte(pdev, 0x70f, tmp);
}

<<<<<<< HEAD
bool rtl8192_pci_findadapter(struct pci_dev *pdev, struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	u16 VenderID;
	u16 DeviceID;
	u8  RevisionID;
	u16 IrqLine;

	VenderID = pdev->vendor;
	DeviceID = pdev->device;
	RevisionID = pdev->revision;
	pci_read_config_word(pdev, 0x3C, &IrqLine);

	priv->card_8192 = priv->ops->nic_type;

	if (DeviceID == 0x8172) {
		switch (RevisionID) {
		case HAL_HW_PCI_REVISION_ID_8192PCIE:
			printk(KERN_INFO "Adapter(8192 PCI-E) is found - "
			       "DeviceID=%x\n", DeviceID);
			priv->card_8192 = NIC_8192E;
			break;
		case HAL_HW_PCI_REVISION_ID_8192SE:
			printk(KERN_INFO "Adapter(8192SE) is found - "
			       "DeviceID=%x\n", DeviceID);
			priv->card_8192 = NIC_8192SE;
			break;
		default:
			printk(KERN_INFO "UNKNOWN nic type(%4x:%4x)\n",
			       pdev->vendor, pdev->device);
=======
bool rtl92e_check_adapter(struct pci_dev *pdev, struct net_device *dev)
{
	struct r8192_priv *priv = (struct r8192_priv *)rtllib_priv(dev);
	u16 device_id;
	u8  revision_id;
	u16 irq_line;

	device_id = pdev->device;
	revision_id = pdev->revision;
	pci_read_config_word(pdev, 0x3C, &irq_line);

	priv->card_8192 = NIC_8192E;

	if (device_id == 0x8192) {
		switch (revision_id) {
		case HAL_HW_PCI_REVISION_ID_8192PCIE:
			dev_info(&pdev->dev,
				 "Adapter(8192 PCI-E) is found - DeviceID=%x\n",
				 device_id);
			priv->card_8192 = NIC_8192E;
			break;
		case HAL_HW_PCI_REVISION_ID_8192SE:
			dev_info(&pdev->dev,
				 "Adapter(8192SE) is found - DeviceID=%x\n",
				 device_id);
			priv->card_8192 = NIC_8192SE;
			break;
		default:
			dev_info(&pdev->dev,
				 "UNKNOWN nic type(%4x:%4x)\n",
				 pdev->vendor, pdev->device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			priv->card_8192 = NIC_UNKNOWN;
			return false;
		}
	}

<<<<<<< HEAD
	if (priv->ops->nic_type != priv->card_8192) {
		printk(KERN_INFO "Detect info(%x) and hardware info(%x) not match!\n",
				priv->ops->nic_type, priv->card_8192);
		printk(KERN_INFO "Please select proper driver before install!!!!\n");
		return false;
	}

	rtl8192_parse_pci_configuration(pdev, dev);
=======
	if (priv->card_8192 != NIC_8192E) {
		dev_info(&pdev->dev,
			 "Detect info(%x) and hardware info(%x) not match!\n",
			 NIC_8192E, priv->card_8192);
		dev_info(&pdev->dev,
			 "Please select proper driver before install!!!!\n");
		return false;
	}

	_rtl92e_parse_pci_configuration(pdev, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return true;
}
